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
#include <MemIf.h>                /* for MemIf_SetMode()                    */

/* !LINKSTO SWS_NvM_00554,2 */
#include <NvM.h>                  /* Extern NVRAM Manager API definitions   */

#include <NvM_FunctionIntern.h>   /* NvM functions which are used           */
                                  /* only within the NvM module.            */
#include <NvM_DataIntern.h>       /* NvM data which is used only            */
                                  /* within the NvM module.                 */
#include <NvM_Queue.h>
#include <NvM_StateMachine.h>     /* Contains extern definitions for unit   */
                                  /* 'StateMachine'. It can be included in  */
                                  /* library source code files.             */
#include <NvM_CalcCrc.h>
/* if any DEM event is switched on, include Dem.h */
#if (NVM_PROD_ERR_HANDLING_REQ_FAILED == TS_PROD_ERR_REP_TO_DEM)
/* !LINKSTO SWS_NvM_00554,2 */
#include <Dem.h>
#endif

#if ( NVM_DEV_ERROR_DETECT == STD_ON )
/* !LINKSTO NVM556,1 */
#include <Det.h>                  /* API of module Det.                     */
#endif

#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

FUNC(void,NVM_CODE) NvM_StateMachine_ProcessRedundantBlock
(
  NvM_CurrentFunctionPtrType NextState
)
{
  NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = NextState;
  /* reset error counter */
  NVM_REDUNDANT_ERROR_COUNTER = 0U;

  if ( NVM_BD_BLOCKMNGTTYPE(NvM_CurrentBlockDescriptorPtr->blockDesc) ==
         NVM_BD_BLOCKMNGTTYPE_REDUNDANT
     )
  {
    /* redundant block is used */
    /* start service with second block */
    NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMDatasetBlockIndex = 1U;

    DBG_NVM_GLOBALCALLLEVEL((NvM_GlobalCallLevel),((NvM_GlobalCallLevel)+1U));
    NvM_GlobalCallLevel++;
    NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = NextState;
  }
  /* if ( NVM_BD_BLOCKMNGTTYPE(NvM_CurrentBlockDescriptorPtr->blockDesc) ==
           NVM_BD_BLOCKMNGTTYPE_REDUNDANT ) */

}/* end of function NvM_StateMachine_ProcessRedundantBlock */

FUNC(void,NVM_CODE) NvM_StateMachine_RedundantBlockError(void)
{
  NVM_REDUNDANT_ERROR_COUNTER++;

  if ( ( NVM_BD_BLOCKMNGTTYPE(NvM_CurrentBlockDescriptorPtr->blockDesc) ==
           NVM_BD_BLOCKMNGTTYPE_REDUNDANT
       )  &&
       ( NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMDatasetBlockIndex == 1U )
     )
  {
    /* set DataIndex to zero --> Dem shall be called only at the end of
     * the service.
     */
    NvM_ResetRedundantBlockDataIndex();
  }
  else
  {
#if ( NVM_PROD_ERR_HANDLING_REQ_FAILED == TS_PROD_ERR_REP_TO_DEM )
#if (NVM_REQ_FAILED_DEM_EVENT_ID != NVM_INVALID_DEM_EVENTID )
    NVM_DEM_REPORT_ERROR(NVM_REQ_FAILED_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#endif
#elif ( NVM_PROD_ERR_HANDLING_REQ_FAILED == TS_PROD_ERR_REP_TO_DET )
    NVM_DET_REPORT_ERROR( NvM_CurrentServiceId, NVM_E_DEMTODET_REQ_FAILED );
#elif ( NVM_PROD_ERR_HANDLING_REQ_FAILED == TS_PROD_ERR_REP_TO_USER )
    NVM_PRODUCTION_ERRORS_FUNCTION_CALLOUT(NvM_BlockDescriptorTable[NvM_GlobalBlockNumber].nvBlockIdentifier, NvM_CurrentServiceId, NVM_E_USER_CALLOUT_REQ_FAILED);
#endif

    DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_NOT_OK));
    NvM_GlobalErrorStatus = NVM_REQ_NOT_OK;
  }

  NvM_StateMachine_TerminateCurrentLevel();
}/* end of function NvM_StateMachine_RedundantBlockError */

FUNC(void,NVM_CODE) NvM_StateMachine_TerminateCurrentLevel(void)
{
  NVM_PRECONDITION_ASSERT( ( NvM_GlobalCallLevel > 0U ), NVM_INTERNAL_API_ID);
  DBG_NVM_GLOBALCALLLEVEL((NvM_GlobalCallLevel),((NvM_GlobalCallLevel)-1U));
  NvM_GlobalCallLevel--;
}/* end of function NvM_StateMachine_TerminateCurrentLevel.c */

FUNC(void,NVM_CODE) NvM_StateMachine_TerminateSubLevel(void)
{
  NVM_PRECONDITION_ASSERT( ( NvM_GlobalCallLevel > 1U ), NVM_INTERNAL_API_ID);
  DBG_NVM_GLOBALCALLLEVEL((NvM_GlobalCallLevel),((NvM_GlobalCallLevel)-(2U)));
  NvM_GlobalCallLevel-= 2U;
}/* end of function NvM_StateMachine_TerminateSubLevel */

FUNC(void,NVM_CODE) NvM_StateMachine_ProcessJobResult(void)
{
  const MemIf_JobResultType Result
    = MemIf_GetJobResult( NVM_BD_DEVICEID(NvM_CurrentBlockDescriptorPtr->blockDesc) );

  /* evaluate Result */
  if ( ( Result == MEMIF_JOB_OK ) && ( NVM_REDUNDANT_ERROR_COUNTER == 0U ) )
  {
    /* Job Result from sub-layer is OK and no error in erasing or
     * invalidating a Redundant Block occurred.
     * In case of a write job one error for a Redundant
     * Block is allowed else no error is allowed.
     */

    DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_OK));
    NvM_GlobalErrorStatus = NVM_REQ_OK;

    NvM_ResetRedundantBlockDataIndex();
    /* Set permanent RAM Block to unchanged so that the data is not
     * written again by NvM_WriteAll().
     */
    NvM_SetPermanentRamBlockUnchanged();

    NvM_StateMachine_TerminateCurrentLevel();
  }
  else if ( Result == MEMIF_JOB_CANCELED )
  {

    DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_CANCELLED));
    NvM_GlobalErrorStatus = NVM_REQ_CANCELLED;
    /* set CallLevel to zero to terminate this service completely */

    DBG_NVM_GLOBALCALLLEVEL((NvM_GlobalCallLevel),(0U));
    NvM_GlobalCallLevel = 0U;
    NvM_ResetRedundantBlockDataIndex();
  }
  else if ( Result == MEMIF_JOB_PENDING )
  {

    DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_PENDING));
    NvM_GlobalErrorStatus = NVM_REQ_PENDING;
  }
  else
  {
    NvM_StateMachine_RedundantBlockError();
  }/* evaluate Result */
}/* end of function NvM_StateMachine_ProcessJobResult */

FUNC(void,NVM_CODE) NvM_StateMachine_SetNextRequest(void)
{
#if ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U))
  boolean IsQueueEmpty = TRUE;
#endif

  /* Protect concurrent access to queues including the check
   * for emptiness
   */
  SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

#if ( ( NVM_JOB_PRIORITIZATION == STD_ON ) && ( NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U ) )
  /* First check the Immediate Queue */
  if ( NVM_IMMEDIATE_QUEUE_NOT_EMPTY() )
  {
    /* A new immediate request is available */
    NvM_GlobalCallLevel = 1U;

    NvM_GlobalBlockNumber = NvM_Queue_Immediate[NvM_Queue_IndexImmediate].NvMBlockDescriptorIndex;
    NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] =
      NvM_Queue_Immediate[NvM_Queue_IndexImmediate].NvMRequestAsyncPtr;

    /* Get the temporary RAM block address stored in the immediate queue */
    NvM_TempRamBlockAddress =
      NvM_Queue_Immediate[NvM_Queue_IndexImmediate].NvMTempRamBlockDataAddress;

#if ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U))
    IsQueueEmpty = FALSE;
#endif
  }
  else
#endif
  {
    if ( NVM_STD_QUEUE_NOT_EMPTY() )
    {
      /* A new standard request is available */

      DBG_NVM_GLOBALCALLLEVEL((NvM_GlobalCallLevel),(1U));
      NvM_GlobalCallLevel = 1U;

      NvM_GlobalBlockNumber = NvM_Queue_Standard[NvM_Queue_IndexStandard].NvMBlockDescriptorIndex;
      NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] =
        NvM_Queue_Standard[NvM_Queue_IndexStandard].NvMRequestAsyncPtr;

      /* Restore the current service Id from AdminBlockTable for multiblock requests*/
      if (((NVM_WRITE_ALL_API_ID == NvM_AdminBlockTable[NVM_BLOCK_MULTI].NvMCurrentServiceId) ||
          (NVM_READ_ALL_API_ID == NvM_AdminBlockTable[NVM_BLOCK_MULTI].NvMCurrentServiceId)) ||
          (NVM_VALIDATE_ALL_API_ID == NvM_AdminBlockTable[NVM_BLOCK_MULTI].NvMCurrentServiceId))
      {
        NvM_CurrentServiceId = NvM_AdminBlockTable[NVM_BLOCK_MULTI].NvMCurrentServiceId;
      }

      /* Get the temporary RAM block address stored in the standard queue */
      NvM_TempRamBlockAddress =
        NvM_Queue_Standard[NvM_Queue_IndexStandard].NvMTempRamBlockDataAddress;

#if ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U))
      IsQueueEmpty = FALSE;
#endif
    }
  }

  SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

#if ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U))
  /* Check if RAM block CRC calculation is pending for any blocks and both queues are empty .*/
  if ((NvM_RamBlockCrcCalcCount > 0U) && (IsQueueEmpty == TRUE))
  {
    /* Get next pending block */
    NvM_GlobalBlockNumber = NvM_CalcCrc_PeekElement();

    /* A RAM block crc calculation request is available */
    NvM_GlobalCallLevel = 1U;

    NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] =
                        &NvM_SetRamBlockStatus_Async_CalcCrc_Start;
#if (NVM_SYNC_MECHANISM == STD_ON)
    /* Reset mirror counter if explicit synchronization is used */
    /* This is done every time the asynchronous CRC calculation is started */
    NvM_MirrorRetryCount = 0U;
#endif

    /* Protect concurrent access to NvM_AdminBlockTable */
    SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

    /* Set block status to NVM_REQ_PENDING, since RAM block CRC calculation
    * is started. */
    NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMResult = NVM_REQ_PENDING;

    SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
  }
#endif  /* ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U)) */
}/* end of function NvM_StateMachine_SetNextRequest  */

#if ( ( NVM_JOB_PRIORITIZATION == STD_ON ) && ( NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U ) )

FUNC(void,NVM_CODE) NvM_StateMachine_WriteImmediateBlock(void)
{
  /* Current standard priority job is cancelled --> start the immediate priority job. */
  NvM_StateMachine_SetNextRequest();
  NvM_PreService_Function();
}/* end of function NvM_StateMachine_WriteImmediateBlock */

#endif /* #if ( ( NVM_JOB_PRIORITIZATION == STD_ON ) &&
                ( NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U ) ) */

#if ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && \
     (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U))
FUNC(void, NVM_CODE) NvM_ProcessStandardPriorityJob(void)
{
  /* Protect concurrent access to NvM_AdminBlockTable */
  SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

  /* Set block status to NVM_REQ_OK, since RAM block CRC calculation
   * is interrupted. So other single block request can be queued. */
  NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMResult = NVM_REQ_OK;

  SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

  /* Ongoing NvM_SetRamBlockStatus job is interrupted and an
   * standard priority job is started. */
  NvM_StateMachine_SetNextRequest();
  NvM_PreService_Function();
}
#endif  /* ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) &&
           (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U)) */

#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>

/*==================[end of file NvM_StateMachine.c]========================*/
