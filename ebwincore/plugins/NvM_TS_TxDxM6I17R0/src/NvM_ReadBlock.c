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

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 11.3 (advisory)
 * A cast shall not be performed between a pointer to object type and a pointer to a different object type.
 *
 * Reason: The portion of code is necessary and it has no alignment problems.
 *
 * MISRAC2012-2) Deviated Rule: 8.9 (advisory)
 * An object should be defined at block scope if its identifier only appears in a single function.
 *
 * Reason: Even thou is only used once the object needs a global scope.
 *
 * MISRAC2012-3) Deviated Rule: 11.5 (advisory)
 * A conversion should not be performed from pointer to void into pointer to object.
 *
 * Reason: The portion of code is necessary and it has no alignment problems.
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

#include <TSMem.h>                /* Autosar Module  Base */
/* !LINKSTO SWS_NvM_00554,2 */
#include <MemIf.h>                /* for MemIf_GetStatus() */

/* !LINKSTO SWS_NvM_00554,2 */
#include <NvM.h>                  /* Extern NVRAM Manager API definitions   */

#include <NvM_FunctionIntern.h>   /* NvM functions which are used           */
                                  /* only within the NvM module.            */
#include <NvM_DataIntern.h>       /* NvM data which is used only            */
                                  /* within the NvM module.                 */
#include <NvM_Queue.h>            /* queue external definitions.            */
#include <NvM_CalcCrc.h>
#include <NvM_StateMachine.h>     /* Contains extern definitions for unit   */
                                  /* 'StateMachine'. It can be included in  */
                                  /* library source code files.             */

#if ( NVM_INCLUDE_RTE == STD_ON )
#include <Rte_NvM.h>             /* Rte type definitions. */
#endif

/* if any DEM event is switched on, include Dem.h */
#if ((NVM_PROD_ERR_HANDLING_INTEGRITY_FAILED == TS_PROD_ERR_REP_TO_DEM) || \
     (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW == TS_PROD_ERR_REP_TO_DEM) || \
     (NVM_PROD_ERR_HANDLING_REQ_FAILED == TS_PROD_ERR_REP_TO_DEM) || \
     (NVM_PROD_ERR_HANDLING_LOSS_OF_REDUNDANCY == TS_PROD_ERR_REP_TO_DEM ))
/* !LINKSTO SWS_NvM_00554,2 */
#include <Dem.h>
#endif

#if ( NVM_DEV_ERROR_DETECT == STD_ON )
/* !LINKSTO NVM556,1 */
#include <Det.h>               /* API of module Det. */
#endif

/* if single block job status reporting is switched on, include BswM_NvM.h */
#if (NVM_BSWM_BLOCK_STATUS_INFO == STD_ON)
#include <BswM_NvM.h>
#endif

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/
#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>

/** \brief calls function MemIf_Read
 **
 ** \param[in] DataBufferPtr: address of the data buffer the NVRAM data must
 **                           be copied to.
 **                           range: 0.. size of NVM_APPL_DATA
 ** \param[in] Length: length of the data to be read. range: 0..2^16-1
 **
 **/
STATIC FUNC(void,NVM_CODE) NvM_MemIfRead
(
  P2VAR( uint8, AUTOMATIC, NVM_APPL_DATA ) DataBufferPtr,
  uint16                                   Length
);
#if(defined(NVM_VARIABLE_BLOCK_LENGTH_EN))
#if (NVM_VARIABLE_BLOCK_LENGTH_EN == STD_ON)
/** \brief calls function NvM_IsolateDataFromCrc
 **
 ** \param[in] DataBufferPtr: address of the data buffer
 **                           range: 0.. size of NVM_APPL_DATA
 ** \param[in] BlkLength: length of the data read. range: 0..2^16-1
 **
 ** \param[in] CrcLength: length of the Crc
 **
 **/
STATIC FUNC(void,NVM_CODE) NvM_IsolateDataFromCrc
(
  P2VAR( uint8, AUTOMATIC, NVM_APPL_DATA ) DataBufferPtr,
  uint16 BlkLength,
  uint8 CrcLength
);
#endif
#endif /*#if(defined(NVM_VARIABLE_BLOCK_LENGTH_EN))*/
/** \brief Sets the global error status
 **        in case of Loss of Redundancy.
 **
 **/
STATIC FUNC(void,NVM_CODE) NvM_SetRedundantResult(void);


/** \brief Reports production errors for read block
 **
 **/
STATIC FUNC(void, NVM_CODE) NvM_ReportReadProductionErrors(void);

#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

#define NVM_START_SEC_VAR_CLEARED_8
#include <NvM_MemMap.h>
/* Deviation MISRAC2012-2 */
STATIC VAR(uint8, NVM_VAR_CLEARED) NvM_StoredCrcBuf[NVM_STORED_CRC_SIZE];

#define NVM_STOP_SEC_VAR_CLEARED_8
#include <NvM_MemMap.h>

/*==================[external function definitions]==========================*/

#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>

FUNC(void,NVM_CODE) NvM_InitStoredCrcBuf(void)
{
    uint16 i=0;
    for(i=0;i<NVM_STORED_CRC_SIZE;i++)
    {
    NvM_StoredCrcBuf[i] = 0;
    }
}



#if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)

FUNC(Std_ReturnType,NVM_CODE) NvM_ASR40_ReadBlock
(
  NvM_ASR40_BlockIdType                 BlockId,
  P2VAR(void, AUTOMATIC, NVM_APPL_DATA) NvM_DstPtr
)
{
  uint16 BlockIndex;
  Std_ReturnType Result = E_NOT_OK; /* Result of this function */
  DBG_NVM_ASR40_READBLOCK_ENTRY(BlockId, NvM_DstPtr);
#ifndef NVM_READBLOCK_CALLOUT
  BlockIndex = NvM_CheckCondReadBlock(BlockId, NvM_DstPtr);
  if(BlockIndex != NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS)
#else
  /* If multi-core wrapper is active the parameter BlockId is actually the block index */
  BlockIndex = (uint16)BlockId;
#endif
  {
    /* protect concurrent access to the standard queue and
     * admin block
     */
    SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
    if (NvM_AdminBlockTable[BlockIndex].NvMResult != NVM_REQ_PENDING)
    {
      /* Deviation MISRAC2012-3 */
      Result = NvM_Queue_InsertStandard( BlockIndex, (P2VAR(uint8, AUTOMATIC, NVM_APPL_DATA))NvM_DstPtr, &NvM_ReadBlock_Async);
    }

    if (E_OK == Result)
    {
      /* Insert the API ID into NvM_AdminBlockTable.
       * The information shall be available in case the request has only been queued,
       * but the processing has not been started and a cancel request was issued.
       */
      NvM_AdminBlockTable[BlockIndex].NvMCurrentServiceId = NVM_READ_BLOCK_API_ID;
    }

    SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

#if (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW != TS_PROD_ERR_DISABLE)
  if (E_OK != Result)
  {
#if (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW == TS_PROD_ERR_REP_TO_DEM)
#if (NVM_QUEUE_OVERFLOW_DEM_EVENT_ID != NVM_INVALID_DEM_EVENTID )
        NVM_DEM_REPORT_ERROR(NVM_QUEUE_OVERFLOW_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#endif
#elif (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW == TS_PROD_ERR_REP_TO_DET)
        NVM_DET_REPORT_ERROR(NVM_READ_BLOCK_API_ID, NVM_E_DEMTODET_QUEUE_OVERFLOW);
#elif (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW == TS_PROD_ERR_REP_TO_USER)
        NVM_PRODUCTION_ERRORS_FUNCTION_CALLOUT(BlockId, NVM_READ_BLOCK_API_ID, NVM_E_USER_CALLOUT_QUEUE_OVERFLOW);
#endif
  }
#endif

#if (NVM_BSWM_BLOCK_STATUS_INFO == STD_ON)
    /* Check if block status changed and block status reporting to BswM is
     * enabled for this block */
    if ((E_OK == Result) &&
        (NVM_BD_BLOCKSTATUSINFO(NvM_BlockDescriptorTable[BlockIndex].blockDesc)))
    {
      /* Block status changed due to queuing read request, report the
       * block status to BswM */
      BswM_NvM_CurrentBlockMode(BlockId, NVM_REQ_PENDING);
    }
#endif
#if ( defined(NVM_MULTICORE_ENABLED) )
    NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus &= (uint8)(~(NVM_ES_MULTICORE_LOCK_MASK));
#endif
  }

  DBG_NVM_ASR40_READBLOCK_EXIT(Result, BlockId, NvM_DstPtr);
  return Result;
}/* end of function NvM_ReadBlock */

/* !LINKSTO SWS_NvM_00764,1 */
FUNC(Std_ReturnType,NVM_CODE) NvM_ASR42_ReadPRAMBlock
(
  NvM_ASR42_BlockIdType BlockId
)
{
  uint16 BlockIndex;
  Std_ReturnType Result = E_NOT_OK; /* Result of this function */
  DBG_NVM_ASR42_READPRAMBLOCK_ENTRY(BlockId);

#ifndef NVM_READPRAMBLOCK_CALLOUT
  BlockIndex = NvM_CheckCondReadPRAMBlock(BlockId);
  if(BlockIndex != NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS)
#else
  /* If multi-core wrapper is active the parameter BlockId is actually the block index */
  BlockIndex = (uint16)BlockId;
#endif
  {
    /* protect concurrent access to the standard queue and
     * admin block
     */
    SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
    if (NvM_AdminBlockTable[BlockIndex].NvMResult != NVM_REQ_PENDING)
    {
      /* !LINKSTO SWS_NvM_00766,1 */
      Result = NvM_Queue_InsertStandard(BlockIndex, NULL_PTR, &NvM_ReadPRAMBlock_Async);
    }

    if (E_OK == Result)
    {
      /* Insert the API ID into NvM_AdminBlockTable.
       * The information shall be available in case the request has only been queued,
       * but the processing has not been started and a cancel request was issued.
       */
      NvM_AdminBlockTable[BlockIndex].NvMCurrentServiceId = NVM_READ_PRAM_BLOCK_API_ID;
    }

    SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

#if (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW != TS_PROD_ERR_DISABLE)
  if (E_OK != Result)
  {
#if (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW == TS_PROD_ERR_REP_TO_DEM)
#if (NVM_QUEUE_OVERFLOW_DEM_EVENT_ID != NVM_INVALID_DEM_EVENTID )
        NVM_DEM_REPORT_ERROR(NVM_QUEUE_OVERFLOW_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#endif
#elif (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW == TS_PROD_ERR_REP_TO_DET)
        NVM_DET_REPORT_ERROR(NVM_READ_PRAM_BLOCK_API_ID, NVM_E_DEMTODET_QUEUE_OVERFLOW);
#elif (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW == TS_PROD_ERR_REP_TO_USER)
        NVM_PRODUCTION_ERRORS_FUNCTION_CALLOUT(BlockId, NVM_READ_PRAM_BLOCK_API_ID, NVM_E_USER_CALLOUT_QUEUE_OVERFLOW);
#endif
  }
#endif

#if (NVM_BSWM_BLOCK_STATUS_INFO == STD_ON)
    /* Check if block status changed and block status reporting to BswM is
     * enabled for this block */
    if ((E_OK == Result) &&
        (NVM_BD_BLOCKSTATUSINFO(NvM_BlockDescriptorTable[BlockIndex].blockDesc)))
    {
      /* Block status changed due to queuing read request, report the
       * block status to BswM */
      BswM_NvM_CurrentBlockMode(BlockId, NVM_REQ_PENDING);
    }
#endif
#if ( defined(NVM_MULTICORE_ENABLED) )
    NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus &= (uint8)(~(NVM_ES_MULTICORE_LOCK_MASK));
#endif
  }

  DBG_NVM_ASR42_READPRAMBLOCK_EXIT(Result, BlockId);
  return Result;
}/* end of function NvM_ASR42_ReadPRAMBlock */

FUNC(void,NVM_CODE) NvM_ReadBlock_Async(void)
{
  /* Since the processing of NvM_ReadBlock() has
   * been started, NvM_CurrentServiceId needs to be set
   */
  NvM_CurrentServiceId = NVM_READ_BLOCK_API_ID;

#if ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U))

   if ((NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMExtendedStatus &
         NVM_ES_RAM_BLOCK_CRC_CALC_MASK) == NVM_ES_RAM_BLOCK_CRC_CALC_MASK
      )
  {
     /* Reset the RAM block CRC calculation flag */
    NvM_CalcCrc_RemoveElement();
    /* Ram block CRC will be recalculated again */
  }
#endif

  /* if a permanent RAM block is used then it must be set invalid
   * and unchanged.
   */
  /* !LINKSTO SWS_NvM_00198,1 */
#if (NVM_SYNC_MECHANISM == STD_ON)
  if (!(NVM_CHECK_WORKING_STATUS(NVM_DYN_MIRROR_MASK)))
#endif
  {
    NvM_SetPermanentRamBlockInvalid();
    NvM_SetPermanentRamBlockUnchanged();
  }

  NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadBlock_Async_State0;
}/* end of function NvM_ReadBlock_Async */

FUNC(void,NVM_CODE) NvM_ReadPRAMBlock_Async(void)
{
  /* Since the processing of NvM_ReadPRAMBlock() has
   * been started, NvM_CurrentServiceId needs to be set
   */
  NvM_CurrentServiceId = NVM_READ_PRAM_BLOCK_API_ID;

#if ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U))

   if ((NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMExtendedStatus &
         NVM_ES_RAM_BLOCK_CRC_CALC_MASK) == NVM_ES_RAM_BLOCK_CRC_CALC_MASK
      )
  {
     /* Reset the RAM block CRC calculation flag */
    NvM_CalcCrc_RemoveElement();
    /* Ram block CRC will be recalculated again */
  }
#endif

  /* if a permanent RAM block is used then it must be set invalid
   * and unchanged.
   */
  /* !LINKSTO SWS_NvM_00767,1 */
#if (NVM_SYNC_MECHANISM == STD_ON)
  if (!(NVM_CHECK_WORKING_STATUS(NVM_DYN_MIRROR_MASK)))
#endif
  {
    NvM_SetPermanentRamBlockInvalid();
    NvM_SetPermanentRamBlockUnchanged();
  }

  NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadBlock_Async_State0;
}/* end of function NvM_ReadPRAMBlock_Async */

#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1) */

FUNC(void,NVM_CODE) NvM_ReadBlock_Async_State0(void)
{
  NvM_PtrToApplDataType TempPtr;
#if (NVM_SYNC_MECHANISM == STD_ON)
  NvM_MirrorRetryCount = 0U;
#endif
  TempPtr = NvM_GetUserRamBlockAddress();
  if (TempPtr != NULL_PTR)
  {
/* !LINKSTO NVM150,1 */
#if ( NVM_NUMBER_OF_DATASET_BLOCKS > 0U )
    if ( ( NVM_BD_BLOCKMNGTTYPE(NvM_CurrentBlockDescriptorPtr->blockDesc) ==
           NVM_BD_BLOCKMNGTTYPE_DATASET) &&
         ( NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMDatasetBlockIndex
             >= NvM_CurrentBlockDescriptorPtr->nvBlockNum
         )
       )
    {
      /* A dataset block is requested whereas the NvBlock is located in ROM.
       * This is a job for NvM_RestoreBlockDefaults.
       */
      NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_RestoreBlockDefaults_AscSt0;
    }
    else
#endif /* NVM_NUMBER_OF_DATASET_BLOCKS > 0U */

    {
      if ( NVM_BD_BLOCKMNGTTYPE(NvM_CurrentBlockDescriptorPtr->blockDesc) ==
           NVM_BD_BLOCKMNGTTYPE_REDUNDANT )
      {
        /* if a redundant block is requested then set the dataset index
         * to zero.
         */
        NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMDatasetBlockIndex = 0U;
        /* every time a redundant NvM block is being read the bit
         * indicating redundancy failure is cleared
         */
        NVM_CLEAR_WORKING_STATUS(NVM_DYN_REDUNDANCY_MASK);
      }
      /* Set CRC calculation or block copy state */
      NvM_ReadBlock_Async_State2();

      DBG_NVM_GLOBALCALLLEVEL((NvM_GlobalCallLevel),((NvM_GlobalCallLevel)+1U));
      NvM_GlobalCallLevel++;
      NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadBlock_Async_State10;

#if (NVM_NUMBER_OF_READ_RETRY_BLOCKS > 0U)
      /* Reset Read Retry Count to perform block read retry */
      NvM_ReadRetryCount = 1U;
#endif
    }
  }
}/* end of function NvM_ReadBlock_Async_State0 */

FUNC(void,NVM_CODE) NvM_ReadBlock_Async_State2(void)
{
  /* !LINKSTO NVM127,1 */
  if (NVM_BD_CRCTYPE(NvM_CurrentBlockDescriptorPtr->blockDesc) != NVM_BD_CRCTYPE_NONE)
  {
    /* CRC must be calculated */
    NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadBlock_Async_State4;
  }
  else
  {
    /* CRC must not be calculated */
    NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadBlock_Async_State6;
  }
}/* end of function NvM_ReadBlock_Async_State2 */

FUNC(void,NVM_CODE) NvM_ReadBlock_Async_State4(void)
{
  NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadBlock_Async_State5;

  /* calculate CRC */
  NvM_CalculateCrc(NvM_GetDataAddress());
}/* end of function NvM_ReadBlock_Async_State4 */

FUNC(void,NVM_CODE) NvM_ReadBlock_Async_State5(void)
{
  /* Do CRC check */
  NvM_GlobalErrorStatus = NvM_CheckCrc(NvM_CalcCrc_GetBlockCrcAddress(),
                                      (NvM_PtrToApplDataType)(&NvM_CalcCrc_CalcBuffer),
                                       NvM_CurrentBlockDescriptorPtr->blockDesc);

  if (NvM_GlobalErrorStatus == NVM_REQ_OK)
  {
    /* CRC check successful
     * Set the state machine pointer of this level to State6
     * to copy the read data to user ram block
     */
    NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadBlock_Async_State6;

    /* Compare CRC of both blocks additionally for Redundant blocks */
    if ( ( NVM_BD_BLOCKMNGTTYPE(NvM_CurrentBlockDescriptorPtr->blockDesc) ==
           NVM_BD_BLOCKMNGTTYPE_REDUNDANT
         ) &&
         (!NVM_CHECK_WORKING_STATUS(NVM_DYN_REDUNDANCY_MASK))
       )
    {
      /* checks if the current CRC calculation finished is of first redundant block */
      if ( NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMDatasetBlockIndex == 0U )
      {
        /* Copies the CRC value of the first redundant block */
        NvM_CopyCrc(NvM_StoredCrcBuf,
                   (NvM_PtrToApplDataType)&NvM_CalcCrc_CalcBuffer,
                    NvM_CurrentBlockDescriptorPtr->blockDesc);
      }
      else
      {
        /* compares the CRC values of the second redundant block with the stored first block CRC */
        if (NvM_CheckCrc(NvM_StoredCrcBuf,
                        (NvM_PtrToApplDataType)(&NvM_CalcCrc_CalcBuffer),
                         NvM_CurrentBlockDescriptorPtr->blockDesc) != NVM_REQ_OK)
        {
          /* Set the Redundant CRC Error flag to indicate that both copies were
           * successfully read but CRC comparison failed */
          NVM_SET_EXT_STATUS(NVM_ES_REDUNDANT_CRC_ERROR_MASK);
          /* Report production errors */
          NvM_ReportReadProductionErrors();
          /* Set the request result according to the value of the
           * NVM_REDUNDANT_BLOCK_RECOVERY configuration parameter
           */
          NvM_SetRedundantResult();
        }
      }
    }
    /* CRC has been calculated and was correct - clear NVM_DYN_CCU_MASK */
    NVM_CLEAR_WORKING_STATUS(NVM_DYN_CCU_MASK);
    if ( NVM_BD_CALCRAMBLOCKCRC(NvM_CurrentBlockDescriptorPtr->blockDesc) )
    {
      /*Copies CRC also to RAM_CRC buffer*/
      NvM_CopyCrc( NvM_GetStoredRamCRCAddress(),
                  (NvM_PtrToApplDataType)&NvM_CalcCrc_CalcBuffer,
                   NvM_CurrentBlockDescriptorPtr->blockDesc);
    }
#if (NVM_CRC_COMP_MECH == STD_ON)
    /* Update local CRC mirror with the one written in NV*/
    if ( NVM_BD_ENCRCCOMP(NvM_CurrentBlockDescriptorPtr->blockDesc) )
    {
      NvM_CopyCrc( NvM_GetStoredNvCRCAddress(),
                  (NvM_PtrToApplDataType)&NvM_CalcCrc_CalcBuffer,
                   NvM_CurrentBlockDescriptorPtr->blockDesc);
    }
#endif  /* NVM_CRC_COMP_MECH == STD_ON */
  }/* if ( NvM_GlobalErrorStatus == NVM_REQ_OK ) */
  else
  {
    /* Set Crc Check Unequal flag to indicate a Crc mismatch. */
    NVM_SET_WORKING_STATUS(NVM_DYN_CCU_MASK);
    /* Set CRC calculation or block copy state */
    NvM_ReadBlock_Async_State2();
    /* The error handling runs on the next level above */

    DBG_NVM_GLOBALCALLLEVEL((NvM_GlobalCallLevel),((NvM_GlobalCallLevel)+1U));
    NvM_GlobalCallLevel++;
    /* Do read retry */
    NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadBlock_Async_Retry;
  }/* NvM_GlobalErrorStatus == NVM_REQ_OK */
}/* end of function NvM_ReadBlock_Async_State5 */

FUNC(void,NVM_CODE) NvM_ReadBlock_Async_State6(void)
{
  NvM_PtrToApplDataType DataBufPtr = NvM_GetBufferAddress();
#if (NVM_SYNC_MECHANISM == STD_ON)
  Std_ReturnType ReturnVal = E_OK;
#endif
  boolean process_block = FALSE;

  /* depending on the block type it shall be assured that operations are
   *  not processed more than once for a block */
  if ((NVM_BD_BLOCKMNGTTYPE(NvM_CurrentBlockDescriptorPtr->blockDesc) == NVM_BD_BLOCKMNGTTYPE_REDUNDANT) &&
      ( (!(NVM_CHECK_WORKING_STATUS(NVM_DYN_REDUNDANCY_MASK))) &&
        (NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMDatasetBlockIndex == 1U)))
  {
    process_block = FALSE;
  }
  else
  {
    process_block = TRUE;
  }

  /* If Static Block Id is enabled, check the read
   * Block Id against the Current Block Id.
   */
#if (NVM_STATIC_BLOCKID_CHECK == STD_ON)
  if (NVM_BD_USESTATICBLOCKID(NvM_CurrentBlockDescriptorPtr->blockDesc))
  {
    /* Deviation MISRAC2012-1 */
    if (TS_MemCmp(DataBufPtr, NVM_STATIC_BLOCKID(), NVM_BLOCKID_LENGTH) != E_OK)
    {
      /* Set Wrong Block Id Flag to indicate Static Block Id check has failed.*/
      NVM_SET_WORKING_STATUS(NVM_DYN_WBID_MASK);
      /* The error handling runs on the next level above */
      /* Set CRC calculation or block copy state */
      NvM_ReadBlock_Async_State2();
      DBG_NVM_GLOBALCALLLEVEL((NvM_GlobalCallLevel),((NvM_GlobalCallLevel)+1U));
      NvM_GlobalCallLevel++;
      /* Read retry */
      NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadBlock_Async_Retry;
    }
    else
    {
      /* static blockId check was successfully - clear NVM_DYN_WBID_MASK */
      NVM_CLEAR_WORKING_STATUS(NVM_DYN_WBID_MASK);
    }
    /* Data after the Block Header shall be copied */
    DataBufPtr = &(DataBufPtr[NVM_BLOCKID_LENGTH]);
  }
#endif
  if (!NVM_CHECK_WORKING_STATUS(NVM_DYN_WBID_MASK))
  {
#if (NVM_SYNC_MECHANISM == STD_ON)
    /* For single block read job, call the callback function to
     * copy data from NvM module's mirror to RAM block
     */
    if ((NVM_CHECK_WORKING_STATUS(NVM_DYN_MIRROR_MASK)) &&
        (process_block == TRUE))
    {
      /* Set the block status to invalid and unchanged just before mirror copy */
      /* !LINKSTO SWS_NvM_00198,1 */
      NvM_SetPermanentRamBlockInvalid();
      NvM_SetPermanentRamBlockUnchanged();
      ReturnVal = NvM_MirrorCopy(NvM_CurrentBlockDescriptorPtr->readRamFromNvCallback, NULL_PTR, DataBufPtr);
      if (ReturnVal == E_OK)
      {
        NvM_GlobalErrorStatus = NVM_REQ_OK;
      }
    }
    else
#endif
    {
#if (NVM_INTERNAL_BUFFER_SIZE != 0U)
      /* If internal buffer is used,
       * Copy data from internal buffer to user RAM block.
       */
      if (NVM_BD_USEINTERNALBUFFER(NvM_CurrentBlockDescriptorPtr->blockDesc) &&
          (process_block == TRUE))
      {
        /* If user provides space for CRC and permanent RAM is used for the current request,
         * copy user data and CRC */
        if ((NVM_BD_USERPROVIDESSPACEFORCRC(NvM_CurrentBlockDescriptorPtr->blockDesc)) &&
            (NVM_CHECK_WORKING_STATUS(NVM_DYN_UPRB_MASK)))
        {
          /* Copy both data and CRC to the RAM buffer */
          TS_MemCpy(NvM_GetUserRamBlockAddress(), DataBufPtr,
            (uint16)(NvM_BlockDescriptorTable[NvM_GlobalBlockNumber].nvBlockLength +
                     NvM_GetBlockCRCLength(NvM_CurrentBlockDescriptorPtr->blockDesc)));
        }
        else
        {
          /* Copy only the data to the RAM buffer */
          TS_MemCpy(NvM_GetUserRamBlockAddress(), DataBufPtr,
            NvM_BlockDescriptorTable[NvM_GlobalBlockNumber].nvBlockLength);
        }
      }
#endif
    }

#if (NVM_SYNC_MECHANISM == STD_ON)
    if (E_OK == ReturnVal)
#endif
    {
      /* Check if the permanent RAM Block must be used by read request */
      if (NVM_CHECK_WORKING_STATUS(NVM_DYN_UPRB_MASK) &&
          (process_block == TRUE))
      {
        /* Hook function for ReadBlock */
        /* Deviation MISRAC2012-3 */
        NVM_READ_BLOCK_HOOK(NvM_GlobalBlockNumber,
          (NvM_PtrToApplDataType)NvM_BlockDescriptorTable[NvM_GlobalBlockNumber].ramBlockDataAddress,
          NvM_BlockDescriptorTable[NvM_GlobalBlockNumber].nvBlockLength
        );
      }
      /* After the data was successfully copied from NVM to RAM set permanent
       * RAM Block valid and unchanged. NVM200, ( Autosar Bugzilla #20953 ),
       * NVM251.
       * NOTE: The RAM Block was already set to unchanged at the start of
       *       NvM_ReadBlock().
       */
      NvM_SetPermanentRamBlockValid();
      /* Check if the finished Read operation is of a redundant block and also whether
       * the last read block is redundant block 0
       */
      if ( NVM_BD_BLOCKMNGTTYPE(NvM_CurrentBlockDescriptorPtr->blockDesc) ==
             NVM_BD_BLOCKMNGTTYPE_REDUNDANT
         )
      {
        if ( NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMDatasetBlockIndex == 0U )
        {
          /*  Block 0 is read; initiate read of block 1. This is needed to check
           *  Redundancy even though data has been retrieved from block 0.
           */
          NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMDatasetBlockIndex = 1U;
          /* Set return state */
          NvM_ReadBlock_Async_State2();
          /* Read operation runs on one level above */

          DBG_NVM_GLOBALCALLLEVEL((NvM_GlobalCallLevel),((NvM_GlobalCallLevel)+1U));
          NvM_GlobalCallLevel++;
          NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadBlock_Async_State10;
#if (NVM_NUMBER_OF_READ_RETRY_BLOCKS > 0U)
          /* Reset Read Retry Count for redundant block (if configured) */
          NvM_ReadRetryCount = 1U;
#endif
        }
        /* Check whether the first block of redundant block is corrupted */
        else
        {
          /* Report production errors */
          NvM_ReportReadProductionErrors();
          /* check if NVM_DYN_REDUNDANCY_MASK flag is set */
          if (NVM_CHECK_WORKING_STATUS(NVM_DYN_REDUNDANCY_MASK))
          {
            /* Set the request result according to the value of the
             * NVM_REDUNDANT_BLOCK_RECOVERY configuration parameter
             */
            NvM_SetRedundantResult();
          }
          else
          {
            /* NvM_ReadBlock is finished */
            NvM_StateMachine_TerminateCurrentLevel();
          }
        }
      }
      else
      {
        /* Report production errors */
        NvM_ReportReadProductionErrors();
        /* NvM_ReadBlock is finished */
        NvM_StateMachine_TerminateCurrentLevel();
      }
    }
  }
}/* end of function NvM_ReadBlockAsync_State6 */

FUNC(void,NVM_CODE) NvM_ReadBlock_Async_State10(void)
{

  if ( MEMIF_BUSY != MemIf_GetStatus( NVM_BD_DEVICEID(NvM_CurrentBlockDescriptorPtr->blockDesc) ) )
  {
    uint16 Length = NvM_GetBlockLength(NvM_CurrentBlockDescriptorPtr->blockDesc,
                                       NvM_CurrentBlockDescriptorPtr->nvBlockLength ) +
#if (NVM_CRYPTO_HOOKS == STD_ON)
                    NvM_CurrentBlockDescriptorPtr->CryptoExtraInfoSize +
#endif
                    (uint16)NvM_GetBlockCRCLength(NvM_CurrentBlockDescriptorPtr->blockDesc);

    NvM_MemIfRead(NvM_GetBufferAddress(), Length);
  }
  else
  {
    /* If the MemIf returns busy, return from MainFunction */

    DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_PENDING));
    NvM_GlobalErrorStatus = NVM_REQ_PENDING;
  }
}/* end of function NvM_ReadBlock_Async_State10 */

FUNC(void,NVM_CODE) NvM_ReadBlock_Async_State12(void)
{
#if(defined(NVM_VARIABLE_BLOCK_LENGTH_EN))
#if (NVM_VARIABLE_BLOCK_LENGTH_EN == STD_ON)
  uint16 BlkLength;
  uint8 CrcLength;
#endif
#endif /*#if(defined(NVM_VARIABLE_BLOCK_LENGTH_EN))*/
  const MemIf_JobResultType Result
    = MemIf_GetJobResult( NVM_BD_DEVICEID(NvM_CurrentBlockDescriptorPtr->blockDesc) );
  /* !LINKSTO NvM.ReadPRAMBlock.WriteBlockOnce.EnableProtection,1 */
  /* !LINKSTO NvM.ReadBlock.WriteBlockOnce.EnableProtection,1 */
  /* !LINKSTO NvM.VariableBlockLength.WriteProtection,1 */
#if (NVM_WRITE_BLOCK_ONCE == STD_ON)
  if ((NVM_BD_WRITEONCE(NvM_CurrentBlockDescriptorPtr->blockDesc)) &&
      (Result == MEMIF_JOB_OK))
  {
    /* set WriteProtectionFlag and WriteProtectionPermanentFlag */
    NVM_SET_WORKING_STATUS( NVM_DYN_WPF_MASK | NVM_DYN_WPPF_MASK );
  }
#endif

  switch( Result )
  {
    case MEMIF_JOB_PENDING:

      DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_PENDING));
      NvM_GlobalErrorStatus = NVM_REQ_PENDING;
      break;

    case MEMIF_JOB_CANCELED:

      DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_CANCELLED));
      NvM_GlobalErrorStatus = NVM_REQ_CANCELLED;
      /* set CallLevel to zero to terminate this service completely */

      DBG_NVM_GLOBALCALLLEVEL((NvM_GlobalCallLevel),(0U));
      NvM_GlobalCallLevel = 0U;
      break;

    case MEMIF_JOB_OK:

      DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_OK));
      NvM_GlobalErrorStatus = NVM_REQ_OK;
      /* the block was successfully read - clear invalid, inconsistent and lowlevel failure flags */
      NVM_CLEAR_WORKING_STATUS(NVM_DYN_BRVE_MASK | NVM_DYN_BRBI_MASK | NVM_DYN_LLRF_MASK);
#if (NVM_CRYPTO_HOOKS == STD_ON)
      if(NVM_BD_CRYPTOHOOKS(NvM_CurrentBlockDescriptorPtr->blockDesc))
      {
        NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_CryptoOperations_Async_State;
        NvM_PostCryptoState = NvM_ReadState;
      }
      else
#endif
      {
      NvM_StateMachine_TerminateCurrentLevel();
      }
      break;
#if(defined(NVM_VARIABLE_BLOCK_LENGTH_EN))
#if (NVM_VARIABLE_BLOCK_LENGTH_EN == STD_ON)
    case MEMIF_JOB_OK_SIZE_DECREASED:
    {
      DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_OK_BLK_DECREASED));
      /* !LINKSTO NvM.VariableBlockLength.BlockDecrease.ReadResult,1 */
      NvM_GlobalErrorStatus = NVM_REQ_OK_BLK_DECREASED;
      /* the block was successfully read - clear invalid, inconsistent and lowlevel failure flags */
      NVM_CLEAR_WORKING_STATUS(NVM_DYN_BRVE_MASK | NVM_DYN_BRBI_MASK | NVM_DYN_LLRF_MASK);
#if (NVM_CRYPTO_HOOKS == STD_ON)
      if(NVM_BD_CRYPTOHOOKS(NvM_CurrentBlockDescriptorPtr->blockDesc))
      {
        /*Crypto feature not compatible with Increasing/Decreasing blk length but we let it
        go ahead with processing, even thou we are sure it will fail*/
        NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_CryptoOperations_Async_State;
        NvM_PostCryptoState = NvM_ReadState;
      }
      else
#endif
      {
      NvM_StateMachine_TerminateCurrentLevel();
      /* !LINKSTO NvM.VariableBlockLength.BlockDecrease,1 */
      /* !LINKSTO NvM.VariableBlockLength.BlockDecrease.Crc,1 */
      /*Make sure the processing is done like CRC is not configured*/
      NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadBlock_Async_State6;
      }
      break;
    }
    case MEMIF_JOB_OK_SIZE_INCREASED:
    {
      DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_OK_BLK_INCREASED));
      /* !LINKSTO NvM.VariableBlockLength.BlockIncrease.ReadResult,1 */
      NvM_GlobalErrorStatus = NVM_REQ_OK_BLK_INCREASED;
      /* the block was successfully read - clear invalid, inconsistent and lowlevel failure flags */
      NVM_CLEAR_WORKING_STATUS(NVM_DYN_BRVE_MASK | NVM_DYN_BRBI_MASK | NVM_DYN_LLRF_MASK);
#if (NVM_CRYPTO_HOOKS == STD_ON)
        /*Crypto feature not compatible with Increasing/Decreasing blk length but we let it
        go ahead with processing, even thou we are sure it will fail*/
      if(NVM_BD_CRYPTOHOOKS(NvM_CurrentBlockDescriptorPtr->blockDesc))
      {
        NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_CryptoOperations_Async_State;
        NvM_PostCryptoState = NvM_ReadState;
      }
      else
#endif
      {
        NvM_StateMachine_TerminateCurrentLevel();
        /*Make sure the processing is done like CRC is not configured*/
        /* !LINKSTO NvM.VariableBlockLength.BlockIncrease,1 */
        /* !LINKSTO NvM.VariableBlockLength.BlockIncrease.Crc,1 */
        NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadBlock_Async_State6;
        /*Manipulate RAM in case of increasing size to overwrite the CRC bytes with padding so they don't reach application as data*/
        BlkLength = NvM_GetBlockLength(NvM_CurrentBlockDescriptorPtr->blockDesc,
                                         NvM_CurrentBlockDescriptorPtr->nvBlockLength ) +
                      (uint16)NvM_GetBlockCRCLength(NvM_CurrentBlockDescriptorPtr->blockDesc);
        CrcLength = NvM_GetBlockCRCLength(NvM_CurrentBlockDescriptorPtr->blockDesc);
        /*Manipulate RAM in case of increasing size to overwrite the CRC bytes with padding so they don't reach application as data*/
        if( CrcLength != 0 )
        {
          NvM_IsolateDataFromCrc(NvM_GetBufferAddress(), BlkLength, CrcLength );
        }
      }
      break;
    }
#endif /* NVM_VARIABLE_BLOCK_LENGTH_EN == STD_ON */
#endif /*#if(defined(NVM_VARIABLE_BLOCK_LENGTH_EN))*/
    case MEMIF_BLOCK_INCONSISTENT:
      /* MemIf delivered MEMIF_BLOCK_INCONSISTENT --> Set the
       * corresponding bit in the dynamic status word.
       * Read Retry shall be performed (if configured).
       * NOTE: the bit NVM_DYN_BRBI must be reset before a Redundant
       *       Block would be read.
       */
      NVM_SET_WORKING_STATUS(NVM_DYN_BRBI_MASK);
      NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadBlock_Async_Retry;
      break;

    case MEMIF_BLOCK_INVALID:
      /* MemIf delivered MEMIF_BLOCK_INVALID --> Set the
       * corresponding bit in the dynamic status word.
       * Read Retry shall not be performed (the block was explicitly invalidated
       * and it's data shall not be accessed, thus no retry is necessary).
       * NOTE: the bit NVM_DYN_BRVE must be reset before a Redundant
       *       Block would be read.
       */
      NVM_SET_WORKING_STATUS(NVM_DYN_BRVE_MASK);
      /* !LINKSTO NvM.ProcessingErasedBlocks.Processing.Invalid,1 */
#if (NVM_RESULT_ERASED_BLOCKS == NVM_ERASED_BLOCK_INCONSISTENT)
      /* In case the lower layer reports erased/not found blocks as inconsistent
       * no retry shall be performed by NvM because the block was explicitly
       * invalidated by user */
      NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadBlock_Async_StateError1;
#else
      /* In case the lower layer reports erased/not found blocks as invalid
       * retries shall be performed by NvM */
      NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadBlock_Async_Retry;
#endif  /* NVM_RESULT_ERASED_BLOCKS == NVM_ERASED_BLOCK_INCONSISTENT */
      break;

    default:
      /* In case MemIf delivered an other error e.g. MEMIF_JOB_FAILED,
       * Read Retry shall be performed (if configured).
       */
      NVM_SET_WORKING_STATUS(NVM_DYN_LLRF_MASK);
      NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadBlock_Async_Retry;
      break;
  }/* end of switch( Result ) */
}/* end of function NvM_ReadBlock_Async_State12 */

FUNC(void,NVM_CODE) NvM_ReadBlock_Async_Retry(void)
{
#if (NVM_NUMBER_OF_READ_RETRY_BLOCKS > 0U)
  /* check the block for read retry  */
  if ( NvM_ReadRetryCount < NvM_CurrentBlockDescriptorPtr->readRetryLimit )
  {
    /* try to read the same block again */
    NvM_ReadRetryCount++;
    NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadBlock_Async_State10;
  }
  else
#endif

  {
#if (NVM_NUMBER_OF_READ_RETRY_BLOCKS > 0U)
    /* Reset Read Retry Count for redundant block (if configured) */
    NvM_ReadRetryCount = 1U;
#endif

    /* Report the error or process the redundant block (if exists). */
    NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadBlock_Async_StateError1;
  }/* if ( NvM_ReadRetryCount < NvM_CurrentBlockDescriptorPtr->readRetryLimit ) */
}/* end of function NvM_ReadBlock_Async_Retry */

FUNC(void,NVM_CODE) NvM_ReadBlock_Async_StateError1(void)
{
  /* check if the error state entered as a result of redundant block */
  if ( NVM_BD_BLOCKMNGTTYPE(NvM_CurrentBlockDescriptorPtr->blockDesc) ==
       NVM_BD_BLOCKMNGTTYPE_REDUNDANT
     )
  {
    /* initiate read of second redundant block */
    if (NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMDatasetBlockIndex == 0U)
    {
      /* Set Loss of Redundancy flag */
      NVM_SET_WORKING_STATUS(NVM_DYN_REDUNDANCY_MASK);

      /* Read block 1 */
      NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMDatasetBlockIndex = 1U;
      NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadBlock_Async_State10;
#if (NVM_NUMBER_OF_READ_RETRY_BLOCKS > 0U)
      /* Reset Read Retry Count for redundant block (if configured) */
      NvM_ReadRetryCount = 1U;
#endif
    }
    else
    {
      /* Set the Redundant Error flag to indicate that redundant block has failed */
      NVM_SET_EXT_STATUS(NVM_ES_REDUNDANT_ERROR_MASK);
      /* Check if first copy has failed */
      if (NVM_CHECK_WORKING_STATUS(NVM_DYN_REDUNDANCY_MASK))
      {
        /* Both blocks failed */
        /* Clear the flag so that read failure is handled in the normal way */
        NVM_CLEAR_WORKING_STATUS(NVM_DYN_REDUNDANCY_MASK);
        /* Handle read failure */
        NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadBlock_Async_StateError2;
      }
      else
      {
        /* Set Loss of Redundancy flag */
        NVM_SET_WORKING_STATUS(NVM_DYN_REDUNDANCY_MASK);
        /* Report production errors */
        NvM_ReportReadProductionErrors();
        /* Only block 1 failed; Handle Loss of redundancy */
        NvM_SetRedundantResult();
        /* Read operation must be terminated completely */
        NvM_StateMachine_TerminateCurrentLevel();
      }
    }
  }
  else
  {
    /* proceed with error handling */
    NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadBlock_Async_StateError2;
  }
}/* end of function NvM_ReadBlock_Async_StateError1 */

FUNC(void,NVM_CODE) NvM_ReadBlock_Async_StateError2(void)
{
  NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadBlock_Async_StateError3;
  /* reset Rom Data Loaded Flag */
  NVM_CLEAR_WORKING_STATUS( NVM_DYN_RDL_MASK );

  DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_NOT_OK));
  NvM_GlobalErrorStatus = NVM_REQ_NOT_OK;

  /* For block 1 the Compiled Configuration Id which is set as default
   * data must not be read automatically by NvM_ReadBlock()
   */
  if ( NvM_GlobalBlockNumber > 1U )
  {
#if (NVM_RESULT_ERASED_BLOCKS == NVM_ERASED_BLOCK_INCONSISTENT)
    /* Default data shall not be restored in case the block was invalidated */
    if (!NVM_CHECK_WORKING_STATUS(NVM_DYN_BRVE_MASK))
#endif
    {
#if ( NVM_NUMBER_OF_DATASET_BLOCKS > 0U )
      /* !LINKSTO SWS_NvM_00951,1 */
      if ( ( NVM_BD_BLOCKMNGTTYPE(NvM_CurrentBlockDescriptorPtr->blockDesc) !=
             NVM_BD_BLOCKMNGTTYPE_DATASET) )
#endif
      {

        DBG_NVM_GLOBALCALLLEVEL((NvM_GlobalCallLevel),((NvM_GlobalCallLevel)+1U));
        NvM_GlobalCallLevel++;
        NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_RestoreBlockDefaults_AscSt0;
      }
    }
#if (NVM_ADVANCED_RECOVERY == STD_ON)
    if (NVM_BD_ADVANCED_RECOVERY(NvM_CurrentBlockDescriptorPtr->blockDesc))
    {
      /* mark block for re-writing */
      NVM_SET_EXT_STATUS(NVM_ES_ADVANCED_RECOVERY_MASK);
    }
#endif
  }
}/* end of NvM_ReadBlock_Async_StateError2 */

FUNC(void,NVM_CODE) NvM_ReadBlock_Async_StateError3(void)
{
  /* Check if CRC is configured and CRC does not match
   * or block was reported as inconsistent
   */
  if ( ( ( NVM_BD_CRCTYPE(NvM_CurrentBlockDescriptorPtr->blockDesc) != NVM_BD_CRCTYPE_NONE ) &&
         ( NVM_CHECK_WORKING_STATUS( NVM_DYN_CCU_MASK ) )
       ) ||
      ( NVM_CHECK_WORKING_STATUS( NVM_DYN_BRBI_MASK ) )
     )
  {
    if ( NvM_GlobalBlockNumber > 1U)
    {
#if ( NVM_NUMBER_OF_DATASET_BLOCKS > 0U )
      if ( ( NVM_BD_BLOCKMNGTTYPE(NvM_CurrentBlockDescriptorPtr->blockDesc) ==
             NVM_BD_BLOCKMNGTTYPE_DATASET) )
      {
        DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_INTEGRITY_FAILED));
        NvM_GlobalErrorStatus = NVM_REQ_INTEGRITY_FAILED;
      }
      else
#endif
      {
        /* Either CRC calculation configured and CRC check negative
         * or the Block Read Block Inconsistent flag is set (MemIf
         * job result was MEMIF_BLOCK_INCONSISTENT). Therefore job result
         * must be set to NVM_REQ_INTEGRITY_FAILED. (NVM358, NVM204, NVM360, NVM295).
         */
#if ( NVM_NUMBER_OF_INIT_BLOCK_CALLBACK > 0U )
        if(
            (NvM_CurrentBlockDescriptorPtr->romBlockDataAddress == NULL_PTR)
            &&
            (NvM_CurrentBlockDescriptorPtr->initBlockCallback == NULL_PTR)
          )
#else
        if(NvM_CurrentBlockDescriptorPtr->romBlockDataAddress == NULL_PTR)
#endif
        {
          DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_INTEGRITY_FAILED));
          NvM_GlobalErrorStatus = NVM_REQ_INTEGRITY_FAILED;
        }
        else
        {
          DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_NOT_OK));
          NvM_GlobalErrorStatus = NVM_REQ_NOT_OK;
        }
      }
    }
    else
    {
      DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_INTEGRITY_FAILED));
      NvM_GlobalErrorStatus = NVM_REQ_INTEGRITY_FAILED;
    }
  }/* if ( NVM_CHECK_CONST_BLOCK_STATUS( NVM_CALCRAMBLOCKCRC_MASK ) ) */
  else if ( NVM_CHECK_WORKING_STATUS( NVM_DYN_BRVE_MASK ) )
  {
    /* The Block Read Valid Error flag is set. This means the NVM Block is
     * invalidated. Therefore NVM_REQ_NV_INVALIDATED must set as
     * job result. NVM341, NVM342, NVM299.
    */

    DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_NV_INVALIDATED));
    NvM_GlobalErrorStatus = NVM_REQ_NV_INVALIDATED;
  }
  else if (NVM_CHECK_WORKING_STATUS(NVM_DYN_WBID_MASK))
  {
    /* The Static Block Id check has failed */

    DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_NOT_OK));
    NvM_GlobalErrorStatus = NVM_REQ_NOT_OK;
  }
  else
  {
    /* In all other cases, e.g. the MemIf job result was MEMIF_JOB_FAILED,
     * the request result is set to NVM_REQ_NOT_OK. NVM359, NVM361.
     * The job result of NvM_RestoreBlockDefaults() is completely
     * ignored.
     */

    DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_NOT_OK));
    NvM_GlobalErrorStatus = NVM_REQ_NOT_OK;
  }/* else */

   /* If ROM Block default data is loaded then the job result is also
    *  set to NVM_REQ_RESTORED_DEFAULTS.
    */
  if (NVM_CHECK_WORKING_STATUS(NVM_DYN_RDL_MASK))
  {
    DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_RESTORED_DEFAULTS));
    NvM_GlobalErrorStatus = NVM_REQ_RESTORED_DEFAULTS;
  }
  /* Report production errors */
  NvM_ReportReadProductionErrors();
  /* Terminate complete NvM_ReadBlock() function -->  */
  /* no more error handling possible                  */
  NvM_StateMachine_TerminateSubLevel();
}/* end of NvM_ReadBlock_Async_StateError3 */

/*==================[internal function definitions]==========================*/
#if(defined(NVM_VARIABLE_BLOCK_LENGTH_EN))
#if (NVM_VARIABLE_BLOCK_LENGTH_EN == STD_ON)
STATIC FUNC(void,NVM_CODE) NvM_IsolateDataFromCrc
(
  P2VAR( uint8, AUTOMATIC, NVM_APPL_DATA ) DataBufferPtr,
  uint16 BlkLength,
  uint8 CrcLength
)
{
  uint16 ParseIdx;
  uint16 Idx2;
  uint8 *pBuf = (DataBufferPtr + BlkLength - 1U);
  boolean boBreakLoop = FALSE;
  uint8 PadValue;

  for(ParseIdx = 0; (ParseIdx < BlkLength) && (boBreakLoop == FALSE); ParseIdx++)
  {
    if(*pBuf != *(pBuf-1U))
    {
      boBreakLoop = TRUE;
    }
    pBuf--;
  }
  for(Idx2 = 1; Idx2 < CrcLength; Idx2++)
  {
    pBuf--;
  }
  PadValue = ~(*pBuf);
  for(Idx2 = 0; Idx2 < (CrcLength + ParseIdx); Idx2++)
  {
    *pBuf = PadValue;
     pBuf++;
  }

}
#endif
#endif /*#if(defined(NVM_VARIABLE_BLOCK_LENGTH_EN))*/
STATIC FUNC(void,NVM_CODE) NvM_MemIfRead
(
  P2VAR( uint8, AUTOMATIC, NVM_APPL_DATA ) DataBufferPtr,
  uint16 Length
)
{
/*Set Write Block Once Read Flag*/
#if (NVM_WRITE_BLOCK_ONCE == STD_ON)
  NVM_SET_WORKING_STATUS(NVM_DYN_WBORF_MASK);
#endif
  /* evaluate Result of MemIf_Read() */
  if ( MemIf_Read(
        NVM_BD_DEVICEID(NvM_CurrentBlockDescriptorPtr->blockDesc),
        NvM_GetMergedBlockNumber(), 0U, DataBufferPtr, Length ) == E_OK )
  {
    /* Read request acceped - clear lowlevel read failure flag */
    NVM_CLEAR_WORKING_STATUS(NVM_DYN_LLRF_MASK);
    NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadBlock_Async_State12;
  }
  else
  {
    /* Read request acceped - set lowlevel read failure flag */
    NVM_SET_WORKING_STATUS(NVM_DYN_LLRF_MASK);
    /* Perform the Read Retry (if exists). */
    NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadBlock_Async_Retry;
  }
}

STATIC FUNC(void,NVM_CODE) NvM_SetRedundantResult(void)
{
#if (NVM_REDUNDANT_BLOCK_RECOVERY == NVM_RECOVERY_ON_REQUEST)
  /* Recovery visible to the user: the request shall return NVM_REQ_REDUNDANCY_FAILED */
  NvM_GlobalErrorStatus = NVM_REQ_REDUNDANCY_FAILED;
#else   /* NVM_REDUNDANT_BLOCK_RECOVERY = NVM_AUTOMATIC_RECOVERY */
  /* Recovery transparent to the user: the request shall return NVM_REQ_OK */
  NvM_GlobalErrorStatus = NVM_REQ_OK;
#endif  /* NVM_REDUNDANT_BLOCK_RECOVERY = NVM_RECOVERY_ON_REQUEST) */
  /* Read operation must be terminated here. */
  NvM_StateMachine_TerminateCurrentLevel();
}

STATIC FUNC(void, NVM_CODE) NvM_ReportReadProductionErrors(void)
{
  if (NVM_BD_BLOCKMNGTTYPE(NvM_CurrentBlockDescriptorPtr->blockDesc) ==
       NVM_BD_BLOCKMNGTTYPE_REDUNDANT)
  {
    /* check if redundancy has been lost or CRC comparison between copies failed */
    if ((NVM_CHECK_WORKING_STATUS(NVM_DYN_REDUNDANCY_MASK))||
        (NVM_CHECK_EXT_STATUS(NVM_ES_REDUNDANT_CRC_ERROR_MASK)))
    {
      /* Report Dem error as failed (SWS_NvM_00595, SWS_NvM_00655) if one copy failed. */
#if ( NVM_PROD_ERR_HANDLING_LOSS_OF_REDUNDANCY == TS_PROD_ERR_REP_TO_DEM )
#if (NVM_REDUNDANCY_LOSS_DEM_EVENT_ID != NVM_INVALID_DEM_EVENTID )
      NVM_DEM_REPORT_ERROR(NVM_REDUNDANCY_LOSS_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#endif
#elif ( NVM_PROD_ERR_HANDLING_LOSS_OF_REDUNDANCY == TS_PROD_ERR_REP_TO_DET )
      NVM_DET_REPORT_ERROR( NvM_CurrentServiceId, NVM_E_DEMTODET_LOSS_OF_REDUNDANCY );
#elif (NVM_PROD_ERR_HANDLING_LOSS_OF_REDUNDANCY == TS_PROD_ERR_REP_TO_USER)
      NVM_PRODUCTION_ERRORS_FUNCTION_CALLOUT(NvM_BlockDescriptorTable[NvM_GlobalBlockNumber].nvBlockIdentifier, NvM_CurrentServiceId, NVM_E_USER_CALLOUT_LOSS_OF_REDUNDANCY);
#endif
    }
    if ((!NVM_CHECK_WORKING_STATUS(NVM_DYN_REDUNDANCY_MASK)) &&
        (NVM_CHECK_EXT_STATUS(NVM_ES_REDUNDANT_ERROR_MASK)))
    {
      if (((NVM_BD_CRCTYPE(NvM_CurrentBlockDescriptorPtr->blockDesc) != NVM_BD_CRCTYPE_NONE) &&
            (NVM_CHECK_WORKING_STATUS(NVM_DYN_CCU_MASK))) ||
          (NVM_CHECK_WORKING_STATUS(NVM_DYN_BRBI_MASK)))
      {
        /* The error must be reported to DEM (NVM360, NVM358, NVM203) */
#if ( NVM_PROD_ERR_HANDLING_INTEGRITY_FAILED == TS_PROD_ERR_REP_TO_DEM )
#if (NVM_INTEGRITY_FAILED_DEM_EVENT_ID != NVM_INVALID_DEM_EVENTID )
        NVM_DEM_REPORT_ERROR(NVM_INTEGRITY_FAILED_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#endif
#elif ( NVM_PROD_ERR_HANDLING_INTEGRITY_FAILED == TS_PROD_ERR_REP_TO_DET )
        NVM_DET_REPORT_ERROR( NvM_CurrentServiceId, NVM_E_DEMTODET_INTEGRITY_FAILED );
#elif ( NVM_PROD_ERR_HANDLING_INTEGRITY_FAILED == TS_PROD_ERR_REP_TO_USER )
        NVM_PRODUCTION_ERRORS_FUNCTION_CALLOUT(NvM_BlockDescriptorTable[NvM_GlobalBlockNumber].nvBlockIdentifier, NvM_CurrentServiceId, NVM_E_USER_CALLOUT_INTEGRITY_FAILED);
#endif
      }
#if (NVM_RESULT_ERASED_BLOCKS == NVM_ERASED_BLOCK_INVALID)
      else if ( NVM_CHECK_WORKING_STATUS( NVM_DYN_BRVE_MASK ) )
      {
        /* The error must be reported to DEM */
#if (NVM_PROD_ERR_HANDLING_REQ_FAILED == TS_PROD_ERR_REP_TO_DEM)
#if (NVM_REQ_FAILED_DEM_EVENT_ID != NVM_INVALID_DEM_EVENTID )
        NVM_DEM_REPORT_ERROR(NVM_REQ_FAILED_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#endif
#elif (NVM_PROD_ERR_HANDLING_REQ_FAILED == TS_PROD_ERR_REP_TO_DET)
        NVM_DET_REPORT_ERROR(NvM_CurrentServiceId, NVM_E_DEMTODET_REQ_FAILED);
#elif ( NVM_PROD_ERR_HANDLING_REQ_FAILED == TS_PROD_ERR_REP_TO_USER )
        NVM_PRODUCTION_ERRORS_FUNCTION_CALLOUT(NvM_BlockDescriptorTable[NvM_GlobalBlockNumber].nvBlockIdentifier, NvM_CurrentServiceId, NVM_E_USER_CALLOUT_REQ_FAILED);
#endif
      }
#endif /* NVM_RESULT_ERASED_BLOCKS == NVM_ERASED_BLOCK_INVALID */
      else if (NVM_CHECK_WORKING_STATUS(NVM_DYN_WBID_MASK))
      {
        /* The error must be reported to DEM */
#if (NVM_PROD_ERR_HANDLING_WRONG_BLOCK_ID == TS_PROD_ERR_REP_TO_DEM)
#if (NVM_WRONG_BLOCK_DEM_EVENT_ID != NVM_INVALID_DEM_EVENTID )
        NVM_DEM_REPORT_ERROR(NVM_WRONG_BLOCK_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#endif
#elif (NVM_PROD_ERR_HANDLING_WRONG_BLOCK_ID == TS_PROD_ERR_REP_TO_DET)
        NVM_DET_REPORT_ERROR(NvM_CurrentServiceId, NVM_E_DEMTODET_REQ_FAILED);
#elif ( NVM_PROD_ERR_HANDLING_WRONG_BLOCK_ID == TS_PROD_ERR_REP_TO_USER )
        NVM_PRODUCTION_ERRORS_FUNCTION_CALLOUT(NvM_BlockDescriptorTable[NvM_GlobalBlockNumber].nvBlockIdentifier, NvM_CurrentServiceId, NVM_E_USER_CALLOUT_WRONG_BLOCK_ID);
#endif
      }
      else if (NVM_CHECK_WORKING_STATUS(NVM_DYN_LLRF_MASK))
      {
        /* The error must be reported to DEM */
#if (NVM_PROD_ERR_HANDLING_REQ_FAILED == TS_PROD_ERR_REP_TO_DEM)
#if (NVM_REQ_FAILED_DEM_EVENT_ID != NVM_INVALID_DEM_EVENTID )
        NVM_DEM_REPORT_ERROR(NVM_REQ_FAILED_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#endif
#elif (NVM_PROD_ERR_HANDLING_REQ_FAILED == TS_PROD_ERR_REP_TO_DET)
        NVM_DET_REPORT_ERROR(NvM_CurrentServiceId, NVM_E_DEMTODET_REQ_FAILED);
#elif ( NVM_PROD_ERR_HANDLING_REQ_FAILED == TS_PROD_ERR_REP_TO_USER )
        NVM_PRODUCTION_ERRORS_FUNCTION_CALLOUT(NvM_BlockDescriptorTable[NvM_GlobalBlockNumber].nvBlockIdentifier, NvM_CurrentServiceId, NVM_E_USER_CALLOUT_REQ_FAILED);
#endif
      }
      else
      {
        /* nothing to do */
      }
    }
  }
  else /* (NVM_BD_BLOCKMNGTTYPE(NvM_CurrentBlockDescriptorPtr->blockDesc) ==
                  NVM_BD_BLOCKMNGTTYPE_REDUNDANT) */
  {
    if (((NVM_BD_CRCTYPE(NvM_CurrentBlockDescriptorPtr->blockDesc) != NVM_BD_CRCTYPE_NONE) &&
          (NVM_CHECK_WORKING_STATUS(NVM_DYN_CCU_MASK))) ||
        (NVM_CHECK_WORKING_STATUS(NVM_DYN_BRBI_MASK)))
    {
      /* The error must be reported to DEM (NVM360, NVM358, NVM203) */
#if ( NVM_PROD_ERR_HANDLING_INTEGRITY_FAILED == TS_PROD_ERR_REP_TO_DEM )
#if (NVM_INTEGRITY_FAILED_DEM_EVENT_ID != NVM_INVALID_DEM_EVENTID )
      NVM_DEM_REPORT_ERROR(NVM_INTEGRITY_FAILED_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#endif
#elif ( NVM_PROD_ERR_HANDLING_INTEGRITY_FAILED == TS_PROD_ERR_REP_TO_DET )
      NVM_DET_REPORT_ERROR( NvM_CurrentServiceId, NVM_E_DEMTODET_INTEGRITY_FAILED );
#elif ( NVM_PROD_ERR_HANDLING_INTEGRITY_FAILED == TS_PROD_ERR_REP_TO_USER )
      NVM_PRODUCTION_ERRORS_FUNCTION_CALLOUT(NvM_BlockDescriptorTable[NvM_GlobalBlockNumber].nvBlockIdentifier, NvM_CurrentServiceId, NVM_E_USER_CALLOUT_INTEGRITY_FAILED);
#endif
    }
#if (NVM_RESULT_ERASED_BLOCKS == NVM_ERASED_BLOCK_INVALID)
    else if ( NVM_CHECK_WORKING_STATUS( NVM_DYN_BRVE_MASK ) )
    {
      /* The error must be reported to DEM */
#if (NVM_PROD_ERR_HANDLING_REQ_FAILED == TS_PROD_ERR_REP_TO_DEM)
#if (NVM_REQ_FAILED_DEM_EVENT_ID != NVM_INVALID_DEM_EVENTID )
      NVM_DEM_REPORT_ERROR(NVM_REQ_FAILED_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#endif
#elif (NVM_PROD_ERR_HANDLING_REQ_FAILED == TS_PROD_ERR_REP_TO_DET)
      NVM_DET_REPORT_ERROR(NvM_CurrentServiceId, NVM_E_DEMTODET_REQ_FAILED);
#elif ( NVM_PROD_ERR_HANDLING_REQ_FAILED == TS_PROD_ERR_REP_TO_USER )
      NVM_PRODUCTION_ERRORS_FUNCTION_CALLOUT(NvM_BlockDescriptorTable[NvM_GlobalBlockNumber].nvBlockIdentifier, NvM_CurrentServiceId, NVM_E_USER_CALLOUT_REQ_FAILED);
#endif
    }
#endif /* NVM_RESULT_ERASED_BLOCKS == NVM_ERASED_BLOCK_INVALID */
    else if (NVM_CHECK_WORKING_STATUS(NVM_DYN_WBID_MASK))
    {
      /* The error must be reported to DEM */
#if (NVM_PROD_ERR_HANDLING_WRONG_BLOCK_ID == TS_PROD_ERR_REP_TO_DEM)
#if (NVM_WRONG_BLOCK_DEM_EVENT_ID != NVM_INVALID_DEM_EVENTID )
      NVM_DEM_REPORT_ERROR(NVM_WRONG_BLOCK_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#endif
#elif (NVM_PROD_ERR_HANDLING_WRONG_BLOCK_ID == TS_PROD_ERR_REP_TO_DET)
      NVM_DET_REPORT_ERROR(NvM_CurrentServiceId, NVM_E_DEMTODET_REQ_FAILED);
#elif ( NVM_PROD_ERR_HANDLING_WRONG_BLOCK_ID == TS_PROD_ERR_REP_TO_USER )
      NVM_PRODUCTION_ERRORS_FUNCTION_CALLOUT(NvM_BlockDescriptorTable[NvM_GlobalBlockNumber].nvBlockIdentifier, NvM_CurrentServiceId, NVM_E_USER_CALLOUT_WRONG_BLOCK_ID);
#endif
    }
    else if (NVM_CHECK_WORKING_STATUS(NVM_DYN_LLRF_MASK))
    {
      /* The error must be reported to DEM */
#if (NVM_PROD_ERR_HANDLING_REQ_FAILED == TS_PROD_ERR_REP_TO_DEM)
#if (NVM_REQ_FAILED_DEM_EVENT_ID != NVM_INVALID_DEM_EVENTID )
      NVM_DEM_REPORT_ERROR(NVM_REQ_FAILED_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#endif
#elif (NVM_PROD_ERR_HANDLING_REQ_FAILED == TS_PROD_ERR_REP_TO_DET)
      NVM_DET_REPORT_ERROR(NvM_CurrentServiceId, NVM_E_DEMTODET_REQ_FAILED);
#elif ( NVM_PROD_ERR_HANDLING_REQ_FAILED == TS_PROD_ERR_REP_TO_USER )
      NVM_PRODUCTION_ERRORS_FUNCTION_CALLOUT(NvM_BlockDescriptorTable[NvM_GlobalBlockNumber].nvBlockIdentifier, NvM_CurrentServiceId, NVM_E_USER_CALLOUT_REQ_FAILED);
#endif
    }
    else
    {
      /* nothing to do */
    }
  }
}


#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>

/*==================[end of file]============================================*/
