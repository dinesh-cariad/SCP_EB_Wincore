/**
 * \file
 *
 * \brief AUTOSAR NvM
 *
 * This file contains the implementation of the AUTOSAR
 * module NvM.
 *
 * \version 6.17.22
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*  MISRA-C:2004 Deviation List

 *
 */

/* !LINKSTO NVM076,1 */
/*==================[inclusions]============================================*/

#if (defined NVM_INTERNAL_USE)
#error NVM_INTERNAL_USE is already defined
#endif
/* prevent redundant declarations of RTE types */
#define NVM_INTERNAL_USE

#include <SchM_NvM.h>             /* module specific header for             */
                                  /* Schedule Manager services              */

#include <NvM_Trace.h>
#include <TSAutosar.h>            /* EB specific standard types             */

/* !LINKSTO SWS_NvM_00554,2 */
#include <NvM.h>                  /* Extern NVRAM Manager API definitions   */

#include <NvM_FunctionIntern.h>   /* NvM functions which are used           */
                                  /* only within the NvM module.            */
#include <NvM_DataIntern.h>       /* NvM data which is used only            */
                                  /* within the NvM module.                 */
#include <NvM_Queue.h>            /* queue external definitions.            */
#include <NvM_CalcCrc.h>

#if ( NVM_DEV_ERROR_DETECT == STD_ON )
/* !LINKSTO NVM556,1 */
#include <Det.h>                  /* API of module Det.                     */
#endif

/* if any DEM event is switched on, include Dem.h */
#if (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW == TS_PROD_ERR_REP_TO_DEM)
/* !LINKSTO SWS_NvM_00554,2 */
#include <Dem.h>
#endif

/* if multi block job status reporting is switched on, include BswM_NvM.h */
#if (NVM_BSWM_MULTI_BLOCK_JOB_STATUS_INFO == STD_ON)
#include <BswM_NvM.h>
#endif

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>

/* !LINKSTO SWS_NvM_00855,1 */
FUNC(void,NVM_CODE) NvM_ValidateAll(void)
{
  Std_ReturnType Result = E_NOT_OK;
  DBG_NVM_VALIDATEALL_ENTRY();

/*                         ( NVM_DEV_ERROR_DETECT == STD_ON )               */
#if ( NVM_DEV_ERROR_DETECT == STD_ON )

  /* Check if module is initialized by NvM_Init() */
  if ( NVM_CHECK_GLOBAL_GENERIC_STATUS_FALSE( NVM_GENERIC_INIT_FLAG_MASK ) )
  {
    /* !LINKSTO SWS_NvM_00863,1 */
    NVM_DET_REPORT_ERROR( NVM_VALIDATE_ALL_API_ID, NVM_E_NOT_INITIALIZED );
  }
  else if ( NvM_AdminBlockTable[NVM_BLOCK_MULTI].NvMResult == NVM_REQ_PENDING )
  {
    NVM_DET_REPORT_ERROR( NVM_VALIDATE_ALL_API_ID, NVM_E_BLOCK_PENDING );
  }
  else
#endif /* NVM_DEV_ERROR_DETECT == STD_ON */
  {
    /* protect concurrent access to the standard queue */
    SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

    if (NvM_AdminBlockTable[NVM_BLOCK_MULTI].NvMResult != NVM_REQ_PENDING)
    {
      /* !LINKSTO SWS_NvM_00858,1, SWS_NvM_00861,1 */
      /* Insert the request in the queue.
       * BlockId 0 for Multi Block Request; NULL_PTR as data address
       */
      Result = NvM_Queue_InsertStandard(NVM_BLOCK_MULTI, NULL_PTR, &NvM_ValidateAll_Async);
    }

    SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

    /* Check if standard queue is already full */
    if (E_OK != Result)
    {
#if (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW == TS_PROD_ERR_REP_TO_DEM)
#if (NVM_QUEUE_OVERFLOW_DEM_EVENT_ID != NVM_INVALID_DEM_EVENTID )
      NVM_DEM_REPORT_ERROR(NVM_QUEUE_OVERFLOW_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#endif
#elif (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW == TS_PROD_ERR_REP_TO_DET)
      NVM_DET_REPORT_ERROR(NVM_VALIDATE_ALL_API_ID, NVM_E_DEMTODET_QUEUE_OVERFLOW);
#elif (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW == TS_PROD_ERR_REP_TO_USER)
      NVM_PRODUCTION_ERRORS_FUNCTION_CALLOUT(NVM_BLOCK_MULTI, NVM_VALIDATE_ALL_API_ID, NVM_E_USER_CALLOUT_QUEUE_OVERFLOW);
#endif
    }
    else
    { /* if validate-all queued (E_OK == Result) */
      SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

      /* Insert the API ID into NvM_AdminBlockTable.
       * The information shall be available in case the request has been interrupted
       * by an immediate write request.
       */
      NvM_AdminBlockTable[NVM_BLOCK_MULTI].NvMCurrentServiceId = NVM_VALIDATE_ALL_API_ID;

      SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

#if (NVM_BSWM_MULTI_BLOCK_JOB_STATUS_INFO == STD_ON)
      /* ValidateAll request status has changed, report the request status to BswM */
      BswM_NvM_CurrentJobMode(NVM_VALIDATE_ALL_API_ID, NVM_REQ_PENDING);
#endif
    }
  }
  DBG_NVM_VALIDATEALL_EXIT();
}

FUNC(void,NVM_CODE) NvM_ValidateAll_Async(void)
{
  /* First state of the asynchronous state machine of API function
   * NvM_ValidateAll.
   */

  /* set NvM_CurrentServiceId */
  NvM_CurrentServiceId = NVM_VALIDATE_ALL_API_ID;

  /* ValidateAll is triggered, init NvM_GlobalBlockNumber,
   * which is incremented in the bellow call of NvM_ValidateAll_Async_State0, so:
   * the first checked block is in fact block with id 2.
   */
  /* In case of immediate write request, NvM_CurrentFunctionPtr restored is at NvM_ValidateAll_Async_State0.
   * NvM_SetNextRequest restores from queue the service Id and the block number.
   */
  NvM_GlobalBlockNumber = NVM_BLOCK_CONFIG_ID;

  /* proceed with single block operation */
  NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ValidateAll_Async_State0;
}

FUNC(void,NVM_CODE) NvM_ValidateAll_Async_State0(void)
{
  uint16 TempBlockIndex;

  SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
  TempBlockIndex = NvM_Queue_Standard[NvM_Queue_IndexStandard].NvMBlockDescriptorIndex;
  SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
  /* Second state of NvM_ValidateAll API.
   * check if there are more blocks to process
   */
  if ( NvM_GlobalBlockNumber < ( NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS - 1U ) )
  {
    /* In case NvM_ValidatedAll() was interrupted by an immediate write job, the multi-block
     * request shall be resumed from the current block number. Otherwise NvM_ValidateAll()
     * must proceed with the next block.
     */
    if (TempBlockIndex == NVM_BLOCK_MULTI)
    {
      /* proceed with next block.  .NvMBlockDescriptorIndex is set to NVM_BLOCK_MULTI at the end of this function.
       * it is set to NvM_GlobalBlockNumber by NvM_Queue_UpdateMultiblockQueueItem if API is interrupted by imm. write
       */
      NvM_GlobalBlockNumber++;
    }

    /* set block descriptor pointer and global working status */
    NvM_PreService_Function();

    /* set NvM_CurrentFunctionPtr to current function */
    NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ValidateAll_Async_State0;

#if ( NVM_NUMBER_OF_SELECT_BLOCK_FOR_VALIDATEALL > 0U )
    /* Process only blocks who have the configuration parameter 'BlockUseAutoValidation' enabled
     * and permanent RAM block or explicit synchronization mechanism enabled.
     * All block management types are processed.
     */
    /* !LINKSTO SWS_NvM_00856,1, SWS_NvM_00860,1 */
#if (NVM_SYNC_MECHANISM == STD_ON)
    if (((NvM_CurrentBlockDescriptorPtr->ramBlockDataAddress != NULL_PTR) ||
         (NVM_BD_USEMIRROR(NvM_CurrentBlockDescriptorPtr->blockDesc))) &&
        (NVM_BD_VALIDATEALL(NvM_CurrentBlockDescriptorPtr->blockDesc)))
#else
    if ( (NVM_BD_VALIDATEALL(NvM_CurrentBlockDescriptorPtr->blockDesc))
         &&
         (NvM_CurrentBlockDescriptorPtr->ramBlockDataAddress != NULL_PTR)
       )
#endif /* NVM_SYNC_MECHANISM = STD_ON */
    {
      /* set NvM_CurrentFunctionPtr to current function */
      NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ValidateAll_Async_State1;

      /* Block data has changed --> validate block. */
      SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
      NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMValid = NVM_BLOCK_VALID;
      /* set block to changed --> it's written by NvM_WriteAll() */
      NVM_SET_DYN_STATUS(NvM_GlobalBlockNumber, NVM_DYN_CHANGE_MASK);
      SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
      NvM_GlobalWorkingStatus |= NVM_DYN_CHANGE_MASK;
      /* !LINKSTO SWS_NvM_00862,1 */
      /* Set the current block's result to NVM_REQ_OK so that normal
       * request for the same block shall be accepted */
      DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_OK));
      NvM_GlobalErrorStatus = NVM_REQ_OK;
      /* request RAM CRC recalculation if configured */
 #if (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U)
      /* Check if RAM CRC calculation is enabled for this block and no other
       * RAM CRC calculation is pending for the same block */
      /* !LINKSTO SWS_NvM_00857,1 */
      if (NVM_BD_CALCRAMBLOCKCRC(NvM_CurrentBlockDescriptorPtr->blockDesc))
      {
        /* RAM CRC calculation is enabled for this block. Start CRC calculation
         * over the RAM block. If RAM crc recalculation was already requested,
         * NvM_RamBlockCrcCalcCount shall not be incremented, because action is already memorized
         * and shall be started here.
         */
        if((NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMExtendedStatus & NVM_ES_RAM_BLOCK_CRC_CALC_MASK)
                       != NVM_ES_RAM_BLOCK_CRC_CALC_MASK)
        {
          SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
          NvM_CalcCrc_EnqueueElement(NvM_GlobalBlockNumber);
          SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
        } /* else, a ram crc recalculation was already requested and memorized. */

#if (NVM_SYNC_MECHANISM == STD_ON)
        /* Reset mirror counter if explicit synchronization is used */
        /* This is done every time the asynchronous CRC calculation is started */
        NvM_MirrorRetryCount = 0U;
#endif

        NvM_GlobalCallLevel++;
        NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_SetRamBlockStatus_Async_CalcCrc_Start;
      }
#endif /* (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U) */
    }/* if block is selected for validate all */
    else
#endif /* if ( NVM_NUMBER_OF_SELECT_BLOCK_FOR_VALIDATEALL > 0U ) */
    {
      /* Check if another job is pending or cancelled for the processing
       * block ID and set the Multi Request Keep Job Pending flag and
       * Block request Canceled Flag accordingly. */
      NvM_CheckBlockStatus();

    }/* block not selected for validate all */

    /* Restore the function pointer and block number in the queue entry because
     * NvM_ValidateAll() might have been interrupted by an immediate write request.
     */
    SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
    NvM_Queue_Standard[NvM_Queue_IndexStandard].NvMRequestAsyncPtr = &NvM_ValidateAll_Async;
    NvM_Queue_Standard[NvM_Queue_IndexStandard].NvMBlockDescriptorIndex = NVM_BLOCK_MULTI;
    SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
  } /* if ( NvM_GlobalBlockNumber < ( NvM_TotalNumberOfNvramBlocks - 1 ) ) */
  else
  {
    /* All blocks finished */
    /* Set final result of multi block request */

    DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_OK));
    NvM_GlobalErrorStatus = NVM_REQ_OK;
    NvM_MultiPostService_Function();
  }/* else-if ( NvM_GlobalBlockNumber < ( NvM_TotalNumberOfNvramBlocks - 1 ) ) */
}

FUNC(void,NVM_CODE) NvM_ValidateAll_Async_State1(void)
{
  /* Third state of NvM_ValidateAll API.
   * Finishes the single block operation of NvM_ValidateAll()
   */
  NvM_CheckBlockStatus();
  NvM_PostService_Function();

  /* set pointer to next state */
  NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ValidateAll_Async_State0;
}
#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>

/*==================[end of file]============================================*/
