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
#include <MemIf.h>
#include <TSMem.h>                /* Autosar Module  Base */
/* !LINKSTO SWS_NvM_00554,2 */
#include <NvM.h>                  /* Extern NVRAM Manager API definitions   */

#include <NvM_FunctionIntern.h>   /* NvM functions which are used           */
                                  /* only within the NvM module.            */
#include <NvM_DataIntern.h>       /* NvM data which is used only            */
                                  /* within the NvM module.                 */
#include <NvM_Queue.h>            /* queue external definitions.            */
#include <NvM_StateMachine.h>     /* External defintions for unit           */
                                  /* StateMachine                           */
#include <NvM_CalcCrc.h>
#include <NvM_Trace.h>
#if ( NVM_DEV_ERROR_DETECT == STD_ON )
/* !LINKSTO NVM556,1 */
#include <Det.h>                 /* API of module Det.                     */
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

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/
#if (NVM_NUMBER_OF_SELECT_BLOCK_FOR_FIRSTINITALL_BLOCKS > 0)
#define NVM_START_SEC_VAR_CLEARED_8
#include <NvM_MemMap.h>
/**  \brief Temporary DataSetIndex for processing.
 **/
STATIC VAR( uint8, NVM_VAR_CLEARED ) FirstInitNvMDatasetBlockIndex;
#define NVM_STOP_SEC_VAR_CLEARED_8
#include <NvM_MemMap.h>

#define NVM_START_SEC_VAR_APPL_DATA
#include <NvM_MemMap.h>

STATIC P2VAR(uint8, NVM_VAR_CLEARED, NVM_APPL_DATA) NvM_FirstInitAllWriteAddress;
STATIC P2VAR(uint8, NVM_VAR_CLEARED, NVM_APPL_DATA) NvM_FirstInitAllDataAddress;

#define NVM_STOP_SEC_VAR_APPL_DATA
#include <NvM_MemMap.h>
#endif /*#if (NVM_NUMBER_OF_SELECT_BLOCK_FOR_FIRSTINITALL_BLOCKS > 0)*/
/*==================[external function definitions]==========================*/

#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>
#if (NVM_NUMBER_OF_SELECT_BLOCK_FOR_FIRSTINITALL_BLOCKS > 0)
FUNC(void,NVM_CODE) NvM_InitFirstInitAll(void)
{
FirstInitNvMDatasetBlockIndex = (uint8)0U;
NvM_FirstInitAllWriteAddress = NULL_PTR;
NvM_FirstInitAllDataAddress = NULL_PTR;
}
#endif
#if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3)
FUNC(void,NVM_CODE) NvM_FirstInitAll(void)
{
#if (NVM_NUMBER_OF_SELECT_BLOCK_FOR_FIRSTINITALL_BLOCKS > 0)
  Std_ReturnType Result = E_NOT_OK;
#endif /*#if (NVM_NUMBER_OF_SELECT_BLOCK_FOR_FIRSTINITALL_BLOCKS > 0)*/

  DBG_NVM_FIRSTINITALL_ENTRY();
#if (NVM_NUMBER_OF_SELECT_BLOCK_FOR_FIRSTINITALL_BLOCKS > 0)
#if ( NVM_DEV_ERROR_DETECT == STD_ON )
  if ( NVM_CHECK_GLOBAL_GENERIC_STATUS_FALSE( NVM_GENERIC_INIT_FLAG_MASK ) )
  {
    /* !LINKSTO SWS_NvM_00938,1 */
    NVM_DET_REPORT_ERROR( NVM_FIRST_INIT_ALL_API_ID, NVM_E_NOT_INITIALIZED );
  }
  else if ( NvM_AdminBlockTable[0U].NvMResult == NVM_REQ_PENDING )
  {
    /* !LINKSTO SWS_NvM_00939,1 */
    NVM_DET_REPORT_ERROR( NVM_FIRST_INIT_ALL_API_ID, NVM_E_BLOCK_PENDING );
  }
  else
#endif
  {
    /* protect concurrent access to the standard queue */
    SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

    if (NvM_AdminBlockTable[0U].NvMResult != NVM_REQ_PENDING)
    {
      /* Insert the request in the queue.
       * BlockId 0 for Multi Block Request; NULL_PTR as data address
       */
      Result = NvM_Queue_InsertStandard(0U, NULL_PTR, &NvM_FirstInitAll_Async);
    }

    if (E_OK == Result)
    {
      /* Insert the API ID into NvM_AdminBlockTable.
       * The information shall be available in case the request has been interrupted
       * by an immediate write request.
       */
      NvM_AdminBlockTable[0U].NvMCurrentServiceId = NVM_FIRST_INIT_ALL_API_ID;
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
      NVM_DET_REPORT_ERROR(NVM_FIRST_INIT_ALL_API_ID, NVM_E_DEMTODET_QUEUE_OVERFLOW);
#elif (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW == TS_PROD_ERR_REP_TO_USER)
      NVM_PRODUCTION_ERRORS_FUNCTION_CALLOUT(NVM_BLOCK_MULTI, NVM_FIRST_INIT_ALL_API_ID, NVM_E_USER_CALLOUT_QUEUE_OVERFLOW);
#endif
    }


    /* Check if write-all queued */
    if (E_OK == Result)
    {
#if (NVM_BSWM_MULTI_BLOCK_JOB_STATUS_INFO == STD_ON)
      /* FirstInitAll request status has changed, report the request status to BswM */
      BswM_NvM_CurrentJobMode(NVM_FIRST_INIT_ALL_API_ID, NVM_REQ_PENDING);
#endif
    }
  }
#endif /* (NVM_NUMBER_OF_SELECT_BLOCK_FOR_FIRSTINITALL_BLOCKS > 0) */
  DBG_NVM_FIRSTINITALL_EXIT();
}
#endif /* (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3)*/
/*==================[internal function definitions]==========================*/
#if (NVM_NUMBER_OF_SELECT_BLOCK_FOR_FIRSTINITALL_BLOCKS > 0)
FUNC(void,NVM_CODE) NvM_FirstInitAll_Async(void)
{
  /* STEP_1: set service ID of NvM_WriteAll */
  /* Since the processing of NvM_WriteAll() has
   * been started, NvM_CurrentServiceId needs to be set
   */
  NvM_CurrentServiceId = NVM_FIRST_INIT_ALL_API_ID;

  /* STEP_2: start with last configured block Id.
   * Block ID 1 (configuration ID) shall be initialized only at the end.
   */
  NvM_GlobalBlockNumber = NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS;

  /* STEP_3: set interim result of write all to NVM_REQ_OK.
   *         Note: the real result is set to NVM_REQ_PENDING.
   */
  NVM_REQUEST_INTERIM_RESULT = NVM_REQ_OK;

  /* STEP_4: switch to fast mode if configured */
#if ( NVM_DRV_MODE_SWITCH == STD_ON )

  MemIf_SetMode( MEMIF_MODE_FAST );

#endif

  /* proceed with state 1 */
  NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_FirstInitAll_Async_State1;
}

FUNC(void,NVM_CODE) NvM_FirstInitAll_Async_State1(void)
{
  /* In case NvM_FirstInitAll() was interrupted by an immediate write job, the multi-block
  * request shall be resumed from the current block number. Otherwise NvM_FirstInitAll()
  * must proceed with the next block.
  */
  if (NvM_Queue_Standard[NvM_Queue_IndexStandard].NvMBlockDescriptorIndex == NVM_BLOCK_MULTI)
  {
    /* proceed with next block */
    NvM_GlobalBlockNumber--;
  }
  /* !LINKSTO SWS_NvM_00912,1 */
  /* Check if last block is written then NvM_WriteAll must be terminated. */
  if (NvM_GlobalBlockNumber == NVM_BLOCK_CONFIG_ID)
  {
    /* set pointer to next state */
    NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_FirstInitAll_Async_End;
  }
  NvM_CurrentBlockDescriptorPtr = &( NvM_BlockDescriptorTable[NvM_GlobalBlockNumber] );

  if (NVM_BD_FIRSTINITALL(NvM_CurrentBlockDescriptorPtr->blockDesc))
  {
    /*Whatever operation may be we start with the last block*/
    FirstInitNvMDatasetBlockIndex = NvM_CurrentBlockDescriptorPtr->nvBlockNum - 1;
    NvM_GlobalCallLevel++;
    /* !LINKSTO SWS_NvM_00913,1 */
    /* !LINKSTO SWS_NvM_00915,1 */
    /* !LINKSTO SWS_NvM_00919,1 */
    /* !LINKSTO SWS_NvM_00920,1 */
    /* !LINKSTO SWS_NvM_00923,1 */
    if(( NVM_BD_BLOCKMNGTTYPE(NvM_CurrentBlockDescriptorPtr->blockDesc) != NVM_BD_BLOCKMNGTTYPE_DATASET) &&
       ((NvM_CurrentBlockDescriptorPtr->romBlockDataAddress != NULL_PTR) || (NvM_CurrentBlockDescriptorPtr->initBlockCallback != NULL_PTR))
      )
    {
      NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_FirstInitAll_Async_PrepareDefData;
    }
    else
    {
      NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_FirstInitAll_Async_Invalidate;
    }
  }
    /* Restore the function pointer and block number in the queue entry because
   * NvM_FirstInitAll() might have been interrupted by an immediate write request.
   */
  SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
  NvM_Queue_Standard[NvM_Queue_IndexStandard].NvMRequestAsyncPtr = &NvM_FirstInitAll_Async;
  NvM_Queue_Standard[NvM_Queue_IndexStandard].NvMBlockDescriptorIndex = NVM_BLOCK_MULTI;
  SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
}

FUNC(void,NVM_CODE) NvM_FirstInitAll_Async_PrepareDefData(void)
{
#if ( NVM_NUMBER_OF_INIT_BLOCK_CALLBACK > 0U )
  Std_ReturnType InitCallbackResult = E_NOT_OK;
  boolean Invalidate = FALSE;
#endif
  /* CurrentRamAddress: address of the target user RAM data buffer */
  NvM_PtrToApplDataType CurrentRamAddress;
    /* CurrentRamAddress: address of the target user RAM data buffer */
  NvM_PtrToApplDataType CurrentDataAddress;

  if (NvM_CurrentBlockDescriptorPtr->ramBlockDataAddress != NULL_PTR)
  {
    /* return address of permanent RAM block */
    CurrentRamAddress = (NvM_PtrToApplDataType)NvM_BlockDescriptorTable[NvM_GlobalBlockNumber].ramBlockDataAddress;
    CurrentDataAddress = CurrentRamAddress;
  }
  else
  {
    /* !LINKSTO SWS_NvM_00914,1 */
    /* !LINKSTO SWS_NvM_00917,1 */
    CurrentRamAddress = NVM_GET_INTERNAL_BUFFER();
#if (NVM_STATIC_BLOCKID_CHECK == STD_ON)
    /* If Static Block Id is enabled, exclude Block Header. */
    if (NVM_BD_USESTATICBLOCKID(NvM_CurrentBlockDescriptorPtr->blockDesc))
    {
      TS_MemCpy(CurrentRamAddress, NVM_STATIC_BLOCKID(), NVM_BLOCKID_LENGTH);
      CurrentDataAddress = &(CurrentRamAddress[NVM_BLOCKID_LENGTH]);
    }
    else
    {
      CurrentDataAddress = CurrentRamAddress;
    }
#else
    CurrentDataAddress = CurrentRamAddress;
#endif
  }

    /* !LINKSTO SWS_NvM_00915,1 */
    /* !LINKSTO SWS_NvM_00918,1 */
  if(NvM_CurrentBlockDescriptorPtr->romBlockDataAddress != NULL_PTR)
  {
    TS_MemCpy( CurrentDataAddress,
       NvM_CurrentBlockDescriptorPtr->romBlockDataAddress,
       NvM_CurrentBlockDescriptorPtr->nvBlockLength
     );
  }
  else
  {
#if ( NVM_NUMBER_OF_INIT_BLOCK_CALLBACK > 0U )
    InitCallbackResult = NvM_CurrentBlockDescriptorPtr->initBlockCallback();
    /* !LINKSTO SWS_NvM_00921,1 */
    /* !LINKSTO SWS_NvM_00922,1 */
    if (InitCallbackResult == E_OK)
    {
#if (NVM_SYNC_MECHANISM == STD_ON)
      if (NvM_CurrentBlockDescriptorPtr->writeRamToNvCallback != NULL_PTR)
      {
        (void)NvM_CurrentBlockDescriptorPtr->writeRamToNvCallback(CurrentDataAddress);
      }
#endif/* NVM_SYNC_MECHANISM == STD_ON*/
    }
    else
    {
      NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_FirstInitAll_Async_Invalidate;
      Invalidate = TRUE;
    }
  }
  if(Invalidate == FALSE)
#else
  }/*Unreachable code, This function will always be called with a default solution*/
#endif/* NVM_NUMBER_OF_INIT_BLOCK_CALLBACK >0*/
  {
    /*Fill internal buffer if still needed for PRAM with internal buffer usage*/
    if(((NvM_CurrentBlockDescriptorPtr->ramBlockDataAddress != NULL_PTR)) && (NVM_BD_USEINTERNALBUFFER(NvM_CurrentBlockDescriptorPtr->blockDesc)))
    {
      CurrentRamAddress = NVM_GET_INTERNAL_BUFFER();
#if (NVM_STATIC_BLOCKID_CHECK == STD_ON)
      /* If Static Block Id is enabled,
       * Copy the Block Identifier to internal buffer (NVM522, NVM523).
       */
      if (NVM_BD_USESTATICBLOCKID(NvM_CurrentBlockDescriptorPtr->blockDesc))
      {
        TS_MemCpy(CurrentRamAddress, NVM_STATIC_BLOCKID(), NVM_BLOCKID_LENGTH);
        TS_MemCpy(&CurrentRamAddress[NVM_BLOCKID_LENGTH], CurrentDataAddress,
              NvM_CurrentBlockDescriptorPtr->nvBlockLength);
          CurrentDataAddress = &(CurrentRamAddress[NVM_BLOCKID_LENGTH]);
      }
      else
#endif
      {
        TS_MemCpy(CurrentRamAddress, CurrentDataAddress,
                NvM_CurrentBlockDescriptorPtr->nvBlockLength);
        CurrentDataAddress = CurrentRamAddress;
      }
    }
    NvM_FirstInitAllDataAddress = CurrentDataAddress;
    NvM_FirstInitAllWriteAddress = CurrentRamAddress;
    /* Default data was copied successfully. */
    if ( NVM_BD_CRCTYPE(NvM_CurrentBlockDescriptorPtr->blockDesc) != NVM_BD_CRCTYPE_NONE )
    {
      NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_FirstInitAll_Async_CopyCrc;
      NvM_CalculateCrc( CurrentDataAddress );
    }
    else
    {
      NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_FirstInitAll_Async_CopyToNv;
    }
  }
}
FUNC(void,NVM_CODE) NvM_FirstInitAll_Async_CopyCrc(void)
{
  NvM_CopyCrc( &(NvM_FirstInitAllDataAddress[NvM_CurrentBlockDescriptorPtr->nvBlockLength]),
              (NvM_PtrToApplDataType)&NvM_CalcCrc_CalcBuffer,
               NvM_CurrentBlockDescriptorPtr->blockDesc);
  NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_FirstInitAll_Async_CopyToNv;
}
FUNC(void,NVM_CODE) NvM_FirstInitAll_Async_CopyToNv(void)
{
  uint16 MergedBlockNr = ( uint16 )( ( uint16 )
    ( NvM_BlockDescriptorTable[NvM_GlobalBlockNumber].nvBlockBaseNum ) |
    ( ( uint16 )FirstInitNvMDatasetBlockIndex ) );
  NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMResult = NVM_REQ_PENDING;

  NvM_GlobalErrorStatus = NVM_REQ_NOT_OK;
  /* For blocks with standard or immediate priority,
   * job cannot be started if MemIf is busy.
   */
  if (MEMIF_BUSY != MemIf_GetStatus( NVM_BD_DEVICEID(NvM_CurrentBlockDescriptorPtr->blockDesc)))
  {
    /* call MemIf_Write() and evaluate result. */
    if ( MemIf_Write(
          NVM_BD_DEVICEID(NvM_CurrentBlockDescriptorPtr->blockDesc),
          MergedBlockNr,
          NvM_FirstInitAllWriteAddress) == E_OK
       )
    {
      NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_FirstInitAll_ProcessJobRestore_Async;
    }
    else
    {
    /* !LINKSTO SWS_NvM_00925,1 */
      NvM_StateMachine_TerminateCurrentLevel();
      NvM_GlobalErrorStatus = NVM_REQ_NOT_OK;
      NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMResult = NvM_GlobalErrorStatus;
    /* !LINKSTO SWS_NvM_00940,1 */
      NVM_REQUEST_INTERIM_RESULT = NvM_GlobalErrorStatus;
    }
  }
  else
  {
    /* If the MemIf returns busy, return from MainFunction */

    DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_PENDING));
    NvM_GlobalErrorStatus = NVM_REQ_PENDING;
  }
}

FUNC(void,NVM_CODE) NvM_FirstInitAll_ProcessJobRestore_Async(void)
{
  const MemIf_JobResultType Result =
    MemIf_GetJobResult(NVM_BD_DEVICEID(NvM_CurrentBlockDescriptorPtr->blockDesc));
  boolean finishedSingleBlock = FALSE;
  /* evaluate Result */
  if (Result == MEMIF_JOB_OK)
  {
      /* Write was successful */
    NvM_GlobalErrorStatus = NVM_REQ_OK;
    if ( NVM_BD_BLOCKMNGTTYPE(NvM_CurrentBlockDescriptorPtr->blockDesc) ==
           NVM_BD_BLOCKMNGTTYPE_REDUNDANT
       )
    {
      /* !LINKSTO SWS_NvM_00916,1 */
      if (FirstInitNvMDatasetBlockIndex == 1)
      {
        FirstInitNvMDatasetBlockIndex--;
        /* Clear CRC mismatch flag */
        NVM_CLEAR_EXT_STATUS(NVM_ES_REDUNDANT_CRC_ERROR_MASK);
        NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_FirstInitAll_Async_CopyToNv;
      }
      else
      {
        NvM_StateMachine_TerminateCurrentLevel();
        finishedSingleBlock = TRUE;
      }
    }
    else
    {
      NvM_StateMachine_TerminateCurrentLevel();
      finishedSingleBlock = TRUE;
    }
#if (NVM_CRC_COMP_MECH == STD_ON)
    /* Update local CRC mirror with the one written in NV*/
    if ( NVM_BD_ENCRCCOMP(NvM_CurrentBlockDescriptorPtr->blockDesc) )
    {
      NvM_CopyCrc( NvM_GetStoredNvCRCAddress(),
                  (NvM_PtrToApplDataType)&NvM_CalcCrc_CalcBuffer,
                   NvM_CurrentBlockDescriptorPtr->blockDesc);
    }
#endif /* NVM_CRC_COMP_MECH == STD_ON */
    if ( NVM_BD_CALCRAMBLOCKCRC(NvM_CurrentBlockDescriptorPtr->blockDesc) )
    {
      /*Copies CRC also to RAM_CRC buffer*/
      NvM_CopyCrc( NvM_GetStoredRamCRCAddress(),
                  (NvM_PtrToApplDataType)&NvM_CalcCrc_CalcBuffer,
                   NvM_CurrentBlockDescriptorPtr->blockDesc);
    }
    /* Set permanent RAM Block to unchanged so that the data is not
     * written again by NvM_WriteAll().
     */
    /* !LINKSTO SWS_NvM_00934,1 */
    NvM_SetPermanentRamBlockUnchanged();
  }
  else if (Result == MEMIF_JOB_PENDING)
  {

    DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_PENDING));
    NvM_GlobalErrorStatus = NVM_REQ_PENDING;
  }
  else
  {
    /* !LINKSTO SWS_NvM_00925,1 */
    /* !LINKSTO SWS_NvM_00926,1 */
    NvM_GlobalErrorStatus = NVM_REQ_NOT_OK;
    NvM_StateMachine_TerminateCurrentLevel();
    finishedSingleBlock = TRUE;
  }/* evaluate Result */

  if(finishedSingleBlock == TRUE)
  {
    NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMResult = NvM_GlobalErrorStatus;
    if(NvM_GlobalErrorStatus != NVM_REQ_OK)
    {
    /* !LINKSTO SWS_NvM_00940,1 */
      NVM_REQUEST_INTERIM_RESULT = NVM_REQ_NOT_OK;
    }
  }
}

FUNC(void,NVM_CODE) NvM_FirstInitAll_Async_Invalidate(void)
{
  VAR( MemIf_StatusType, AUTOMATIC ) Status;
  uint16 MergedBlockNr = ( uint16 )( ( uint16 )
    ( NvM_BlockDescriptorTable[NvM_GlobalBlockNumber].nvBlockBaseNum ) |
    ( ( uint16 )FirstInitNvMDatasetBlockIndex ) );
  NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMResult = NVM_REQ_PENDING;
  /* Check if the device is not MEMIF_BUSY */
  /* In case of a redundant block this shall detect if the second copy (DATASET=1)
     is completed before starting processing first copy (DATASET=0) */
  Status = MemIf_GetStatus( NVM_BD_DEVICEID(NvM_CurrentBlockDescriptorPtr->blockDesc));

  if ( MEMIF_BUSY != Status )
  {
    if( MemIf_InvalidateBlock( NVM_BD_DEVICEID(NvM_CurrentBlockDescriptorPtr->blockDesc),
                               MergedBlockNr ) == E_OK
      )
    {
      /* Poll job result. */
      NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_FirstInitAll_ProcessJobInvalidate_Async;
    }
    else
    {
      /* !LINKSTO SWS_NvM_00927,1 */
      /* !LINKSTO SWS_NvM_00928,1 */
      /* !LINKSTO SWS_NvM_00929,1 */
      NvM_StateMachine_TerminateCurrentLevel();
      NvM_GlobalErrorStatus = NVM_REQ_NOT_OK;
      NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMResult = NvM_GlobalErrorStatus;
      /* !LINKSTO SWS_NvM_00940,1 */
      NVM_REQUEST_INTERIM_RESULT = NvM_GlobalErrorStatus;
    }
  }
  else
  {
    /* If the MemIf returns busy, return from MainFunction */

    DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_PENDING));
    NvM_GlobalErrorStatus = NVM_REQ_PENDING;
  }
}

FUNC(void,NVM_CODE) NvM_FirstInitAll_ProcessJobInvalidate_Async(void)
{
  const MemIf_JobResultType Result =
    MemIf_GetJobResult(NVM_BD_DEVICEID(NvM_CurrentBlockDescriptorPtr->blockDesc));

  /* evaluate Result */
  if (Result == MEMIF_JOB_OK)
  {
      /* Invalidate was successful */
    NvM_GlobalErrorStatus = NVM_REQ_OK;
    /* !LINKSTO SWS_NvM_00924,1 */
    if (FirstInitNvMDatasetBlockIndex != 0)
    {
      FirstInitNvMDatasetBlockIndex--;
      NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_FirstInitAll_Async_Invalidate;
    }
    else
    {
      /* !LINKSTO SWS_NvM_00935,1 */
      NvM_StateMachine_TerminateCurrentLevel();
      NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMResult = NvM_GlobalErrorStatus;
    }
  }
  else if (Result == MEMIF_JOB_PENDING)
  {

    DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_PENDING));
    NvM_GlobalErrorStatus = NVM_REQ_PENDING;
  }
  else
  {
    /* !LINKSTO SWS_NvM_00927,1 */
    /* !LINKSTO SWS_NvM_00928,1 */
    /* !LINKSTO SWS_NvM_00929,1 */
    NvM_StateMachine_TerminateCurrentLevel();
    NvM_GlobalErrorStatus = NVM_REQ_NOT_OK;
    /* !LINKSTO SWS_NvM_00940,1 */
    NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMResult = NvM_GlobalErrorStatus;
    /* !LINKSTO SWS_NvM_00940,1 */
    NVM_REQUEST_INTERIM_RESULT = NvM_GlobalErrorStatus;
  }/* evaluate Result */
}
FUNC(void,NVM_CODE) NvM_FirstInitAll_Async_End(void)
{
  MemIf_StatusType Result;

  /* get status of underlying drivers */
  Result = MemIf_GetStatus(MEMIF_BROADCAST_ID);

  if ((Result != MEMIF_BUSY) && (Result != MEMIF_BUSY_INTERNAL))
  {
    /* The underlying drivers are all idle */
    /* --> FirstInitAll() can be terminated    */

    DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQUEST_INTERIM_RESULT));
    NvM_GlobalErrorStatus = NVM_REQUEST_INTERIM_RESULT;
    NvM_MultiPostService_Function();
  }
  else
  {
    /* If the MemIf returns busy, return from MainFunction */

    DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_PENDING));
    NvM_GlobalErrorStatus = NVM_REQ_PENDING;
  }
}
#endif /* (NVM_NUMBER_OF_SELECT_BLOCK_FOR_FIRSTINITALL_BLOCKS > 0) */
#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>

/*==================[end of file]============================================*/
