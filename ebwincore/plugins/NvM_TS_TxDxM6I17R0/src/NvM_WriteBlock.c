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
 * MISRAC2012-2) Deviated Rule: 11.8 (required)
 * A cast shall not remove any const or volatile qualification from the type pointed to by a pointer.
 *
 * Reason: For memory optimization the portion of code is necessary and it has no side effects.
 *
 * MISRAC2012-3) Deviated Rule: 11.5 (advisory)
 * A conversion should not be performed from pointer to void into pointer to object.
 *
 * Reason: The portion of code is necessary and it has no alignment problems.
 */

/* mgcc62 Deviation List
 *
 * MGCC62-1) Deviated Rule: cast-qual
 * warning: cast discards 'const' qualifier from pointer target type [-Wcast-qual]
 *
 * Reason: Because of memory optimization, the same pointer type uint8* is used for storing NvM_SrcPtr
 * (of type const void *) and NvM_DstPtr (of type void *). As a consequence, a casting of NvM_SrcPtr is
 * needed.
 */

/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true violated
 *
 * Reason: conditions depends on configuration, "false" case is covered by other tests
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

#include <TSMem.h>                /* Autosar Module  Base */

/* !LINKSTO SWS_NvM_00554,2 */
#include <MemIf.h>                /* for MemIf_GetStatus()                  */

/* !LINKSTO SWS_NvM_00554,2 */
#include <NvM.h>                  /* Extern NVRAM Manager API               */
                                  /* definitions                            */
#include <NvM_FunctionIntern.h>   /* NvM functions which are used           */
                                  /* only within the NvM module.            */
#include <NvM_DataIntern.h>       /* NvM data which is used only            */
                                  /* within the NvM module.                 */
#include <NvM_Queue.h>            /* Extern definitions of the unit         */
                                  /* 'Queue'.                               */
#include <NvM_CalcCrc.h>
#include <NvM_StateMachine.h>     /* Contains extern definitions for unit   */
                                  /* 'StateMachine'. It can be included in  */
                                  /* library source code files.             */
#if ( NVM_INCLUDE_RTE == STD_ON )
#include <Rte_NvM.h>           /* Rte type definitions. */
#endif

/* if any DEM event is switched on, include Dem.h */
#if ((NVM_PROD_ERR_HANDLING_REQ_FAILED == TS_PROD_ERR_REP_TO_DEM) || \
    (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW == TS_PROD_ERR_REP_TO_DEM) || \
    (NVM_PROD_ERR_HANDLING_VERIFY_FAILED == TS_PROD_ERR_REP_TO_DEM) || \
    (NVM_PROD_ERR_HANDLING_WRITE_PROTECTED == TS_PROD_ERR_REP_TO_DEM))
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

/** \brief Check result of the read operation and prepare state machine
 ** for writing the corrupted copy.
 **/
STATIC FUNC(void,NVM_CODE) NvM_WriteBlock_ProcessRedundantBlock_State1(void);

/** \brief Performs error handling by reporting the error to Dem.
 **        It is used by NvM_WriteBlock().
 **/
STATIC FUNC(void,NVM_CODE) NvM_WriteBlock_RedundantBlockError(void);

/** \brief Reads data in case of write verification or recovery of redundant blocks
 **        This function checks the status of MemIf and
 **        calls MemIf_Read to read data from NV memory.
 **
 **/
STATIC FUNC(void, NVM_CODE) NvM_WriteBlock_MemIfReadBlock(void);

#if (NVM_WRITE_VERIFICATION == STD_ON)

/** \brief Returns the length of the data to be write verified.
 **
 ** \revtal length of data of current block. range 0..2^16-1
 **
 **/
STATIC FUNC(uint16,NVM_CODE) NvM_GetVerifyDataSize(void);

#endif

#if (NVM_PRE_WRITE_DATA_COMP == STD_ON)

/** \brief Returns the length of the data to be pre write compared.
 **
 ** \revtal length of data of current block. range 0..2^16-1
 **
 **/
STATIC FUNC(uint16,NVM_CODE) NvM_GetPreWriteDataComparisonDataSize(void);

#endif /* NVM_PRE_WRITE_DATA_COMP = STD_ON */

/** \brief Copies the data and Block Id (if required) to the Internal Buffer.
 **        If Internal Buffer is enabled, this function copies
 **        block data from the user Ram block to the Internal Buffer.
 **        If Static Block Id is enabled, the Block Id also is copied.
 **
 **/
STATIC FUNC(void, NVM_CODE) NvM_FillInternalBuffer(void);

#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/
#if ((NVM_WRITE_VERIFICATION == STD_ON) ||(NVM_PRE_WRITE_DATA_COMP == STD_ON))
#define NVM_START_SEC_VAR_INIT_16
#include <NvM_MemMap.h>
/**  \brief The number of bytes for write verification.
 **/
STATIC VAR( uint16, NVM_VAR ) NvM_VerifyOffset = 0U;
#define NVM_STOP_SEC_VAR_INIT_16
#include <NvM_MemMap.h>
#endif /* NVM_WRITE_VERIFICATION = STD_ON || NVM_PRE_WRITE_DATA_COMP = STD_ON */

/*==================[external function definitions]==========================*/
#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>

#if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)

FUNC(Std_ReturnType, NVM_CODE) NvM_ASR40_WriteBlock
(
  NvM_ASR40_BlockIdType                   BlockId,
  P2CONST(void, AUTOMATIC, NVM_APPL_DATA) NvM_SrcPtr
)
{
  uint16 BlockIndex;
  Std_ReturnType Result = E_NOT_OK; /* Result of this function */
#if (NVM_PROD_ERR_HANDLING_WRITE_PROTECTED != TS_PROD_ERR_DISABLE)
  boolean ReportWriteProtected = FALSE; /* To check write protected DEM/DET error reporting */
#endif
#if (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW != TS_PROD_ERR_DISABLE)
  boolean ReportQueueOverflow = FALSE; /* To check whether Det/Dem error shall be reported */
#endif
#if ( NVM_DEV_ERROR_DETECT == STD_ON )
  boolean ReportWriteOnceUnknown = FALSE; /* To check whether Det/Dem error shall be reported */
#endif
  DBG_NVM_ASR40_WRITEBLOCK_ENTRY(BlockId, NvM_SrcPtr);
#ifndef NVM_WRITEBLOCK_CALLOUT
  BlockIndex = NvM_CheckCondWriteBlock(BlockId, NvM_SrcPtr);
  if(BlockIndex != NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS)
#else
  /* If multi-core wrapper is active the parameter BlockId is actually the block index */
  BlockIndex = (uint16)BlockId;
#endif
  {
    /* protect concurrent access to the standard, immediate queues and
     * concurrent access to write protection flag and data set index
     */
    SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

    /* Check also dynamic write protection flag which can be set by
     * NvM_SetBlockProtection(). If block is write protected report production error.
     * This check must be done always and independently of the Det checks.
     * Also the check if a Dataset ROM Block is specified must be done
     * always.
     */
    if (NvM_CheckWriteProtection(BlockIndex) == TRUE)
    {
#if (NVM_PROD_ERR_HANDLING_WRITE_PROTECTED != TS_PROD_ERR_DISABLE)
     /* Write protected DEM/DET error needs to be reported */
      ReportWriteProtected = TRUE;
#endif
    }
#if (NVM_WRITE_BLOCK_ONCE == STD_ON)
    /* !LINKSTO SWS_NvM_00952,1 */
    /*Check if block is WRITEONCE block but have not been read before*/
    else if((NVM_BD_WRITEONCE(NvM_BlockDescriptorTable[BlockIndex].blockDesc))&&(!NVM_CHECK_DYN_STATUS(BlockIndex, NVM_DYN_WBORF_MASK)))
    {
#if ( NVM_DEV_ERROR_DETECT == STD_ON )
        /* !LINKSTO SWS_NvM_00954,1 */
        ReportWriteOnceUnknown = TRUE;
#endif
    }
#endif
    else if ((NvM_CheckDataSetRomBlock(BlockIndex) == FALSE) &&
             /* Locked NV blocks shall not be written */
             (!NVM_CHECK_DYN_STATUS(BlockIndex, NVM_DYN_LOCK_MASK))
            )
    {
      /* The Request is accepted */
      if (NvM_AdminBlockTable[BlockIndex].NvMResult != NVM_REQ_PENDING)
      {
#if ( ( NVM_JOB_PRIORITIZATION == STD_ON ) && ( NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U ) )
        /* Compiler warning explanation: Because of memory optimization,
           the same pointer type uint8* is used for storing NvM_SrcPtr (of type const void *)
           and NvM_DstPtr (of type void *).
           As a consequence, a casting of NvM_SrcPtr is needed. */
        /* Deviation MISRAC2012-2, MGCC62-1 */
        Result = NvM_Queue_InsertImmediate(BlockIndex, (P2VAR(uint8, AUTOMATIC,
                             NVM_APPL_DATA))NvM_SrcPtr, &NvM_WriteBlock_Async);
#else
        /* Compiler warning explanation: Because of memory optimization,
           the same pointer type uint8* is used for storing NvM_SrcPtr (of type const void *)
           and NvM_DstPtr (of type void *).
           As a consequence, a casting of NvM_SrcPtr is needed. */
        /* Deviation MISRAC2012-2, MGCC62-1 */
        Result = NvM_Queue_InsertStandard(BlockIndex, (P2VAR(uint8, AUTOMATIC,
                             NVM_APPL_DATA))NvM_SrcPtr, &NvM_WriteBlock_Async);
#endif

#if (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW != TS_PROD_ERR_DISABLE)
        if (E_OK != Result)
        {
          ReportQueueOverflow = TRUE;
        }
#endif
      }
    }
    else
    {
    }

    if (E_OK == Result)
    {
      /* Insert the API ID into NvM_AdminBlockTable.
       * The information shall be available in case the request has only been queued,
       * but the processing has not been started and a cancel request was issued.
       */
      NvM_AdminBlockTable[BlockIndex].NvMCurrentServiceId = NVM_WRITE_BLOCK_API_ID;
    }
    SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

#if (NVM_PROD_ERR_HANDLING_WRITE_PROTECTED != TS_PROD_ERR_DISABLE)
    if (ReportWriteProtected == TRUE)
    {
#if (NVM_PROD_ERR_HANDLING_WRITE_PROTECTED == TS_PROD_ERR_REP_TO_DEM)
#if (NVM_WRITE_PROT_DEM_EVENT_ID != NVM_INVALID_DEM_EVENTID )
      NVM_DEM_REPORT_ERROR(NVM_WRITE_PROT_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#endif
#elif (NVM_PROD_ERR_HANDLING_WRITE_PROTECTED == TS_PROD_ERR_REP_TO_DET)
      NVM_DET_REPORT_ERROR(NVM_WRITE_BLOCK_API_ID, NVM_E_DEMTODET_WRITE_PROTECTED);
#elif ( NVM_PROD_ERR_HANDLING_WRITE_PROTECTED == TS_PROD_ERR_REP_TO_USER )
      NVM_PRODUCTION_ERRORS_FUNCTION_CALLOUT(BlockId, NVM_WRITE_BLOCK_API_ID, NVM_E_USER_CALLOUT_WRITE_PROTECTED);
#endif
    }
#endif

#if (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW != TS_PROD_ERR_DISABLE)
    if (ReportQueueOverflow == TRUE)
    {
#if (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW == TS_PROD_ERR_REP_TO_DEM)
#if (NVM_QUEUE_OVERFLOW_DEM_EVENT_ID != NVM_INVALID_DEM_EVENTID )
       NVM_DEM_REPORT_ERROR(NVM_QUEUE_OVERFLOW_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#endif
#elif (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW == TS_PROD_ERR_REP_TO_DET)
       NVM_DET_REPORT_ERROR(NVM_WRITE_BLOCK_API_ID, NVM_E_DEMTODET_QUEUE_OVERFLOW);
#elif ( NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW == TS_PROD_ERR_REP_TO_USER )
       NVM_PRODUCTION_ERRORS_FUNCTION_CALLOUT(BlockId, NVM_WRITE_BLOCK_API_ID, NVM_E_USER_CALLOUT_QUEUE_OVERFLOW);
#endif
    }
#endif

#if ( NVM_DEV_ERROR_DETECT == STD_ON )
    if (ReportWriteOnceUnknown == TRUE)
    {
      NVM_DET_REPORT_ERROR( NVM_WRITE_BLOCK_API_ID, NVM_E_WRITE_ONCE_STATUS_UNKNOWN );
    }
#endif


#if (NVM_BSWM_BLOCK_STATUS_INFO == STD_ON)
    /* Check if block status changed and block status reporting to BswM is
     * enabled for this block */
    if ((E_OK == Result) &&
        (NVM_BD_BLOCKSTATUSINFO(NvM_BlockDescriptorTable[BlockIndex].blockDesc)))
    {
      /* Block status changed due to queuing write request, report the
       * block status to BswM */
      BswM_NvM_CurrentBlockMode(BlockId, NVM_REQ_PENDING);
    }
#endif
#if ( defined(NVM_MULTICORE_ENABLED) )
    NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus &= (uint8)(~(NVM_ES_MULTICORE_LOCK_MASK));
#endif
  }

  DBG_NVM_ASR40_WRITEBLOCK_EXIT(Result, BlockId, NvM_SrcPtr);
  return Result;

}/* end of function NvM_WriteBlock */

/* !LINKSTO SWS_NvM_00793,1 */
FUNC(Std_ReturnType, NVM_CODE) NvM_ASR42_WritePRAMBlock
(
  NvM_ASR42_BlockIdType BlockId
)
{
  uint16 BlockIndex;
  Std_ReturnType Result = E_NOT_OK; /* Result of this function */
#if (NVM_PROD_ERR_HANDLING_WRITE_PROTECTED != TS_PROD_ERR_DISABLE)
  boolean ReportWriteProtected = FALSE; /* To check write protected DEM/DET error reporting */
#endif
#if (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW != TS_PROD_ERR_DISABLE)
  boolean ReportQueueOverflow = FALSE; /* To check whether Det/Dem error shall be reported */
#endif
#if ( NVM_DEV_ERROR_DETECT == STD_ON )
  boolean ReportWriteOnceUnknown = FALSE; /* To check whether Det/Dem error shall be reported */
#endif
  DBG_NVM_ASR42_WRITEPRAMBLOCK_ENTRY(BlockId);

#ifndef NVM_WRITEPRAMBLOCK_CALLOUT
  BlockIndex = NvM_CheckCondWritePRAMBlock(BlockId);
  if(BlockIndex != NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS)
#else
  /* If multi-core wrapper is active the parameter BlockId is actually the block index */
  BlockIndex = (uint16)BlockId;
#endif
  {
    /* protect concurrent access to the standard, immediate queues and
     * concurrent access to write protection flag and data set index
     */
    SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

    /* Check also dynamic write protection flag which can be set by
     * NvM_SetBlockProtection(). If block is write protected report production error.
     * This check must be done always and independently of the Det checks.
     * Also the check if a Dataset ROM Block is specified must be done
     * always.
     */
    if (NvM_CheckWriteProtection(BlockIndex) == TRUE)
    {
#if (NVM_PROD_ERR_HANDLING_WRITE_PROTECTED != TS_PROD_ERR_DISABLE)
     /* Write protected DEM/DET error needs to be reported */
      /* !LINKSTO SWS_NvM_00796,1 */
      ReportWriteProtected = TRUE;
#endif
    }
#if (NVM_WRITE_BLOCK_ONCE == STD_ON)
    /* !LINKSTO SWS_NvM_00952,1 */
    /*Check if block is WRITEONCE block but have not been read before*/
    else if((NVM_BD_WRITEONCE(NvM_BlockDescriptorTable[BlockIndex].blockDesc))&&(!NVM_CHECK_DYN_STATUS(BlockIndex, NVM_DYN_WBORF_MASK)))
    {
#if ( NVM_DEV_ERROR_DETECT == STD_ON )
        /* !LINKSTO SWS_NvM_00955,1 */
        ReportWriteOnceUnknown = TRUE;
#endif
    }
#endif
    else if ( (NvM_CheckDataSetRomBlock(BlockIndex) == FALSE) &&
              /* Locked NV blocks shall not be written */
              /* !LINKSTO SWS_NvM_00797,1 */
              (!NVM_CHECK_DYN_STATUS(BlockIndex, NVM_DYN_LOCK_MASK))
            )
    {
      /* !LINKSTO SWS_NvM_00798,1 */
      /* The Request is accepted */
      if (NvM_AdminBlockTable[BlockIndex].NvMResult != NVM_REQ_PENDING)
      {
#if ( ( NVM_JOB_PRIORITIZATION == STD_ON ) && ( NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U ) )
        /* Compiler warning explanation: Because of memory optimization,
           the same pointer type uint8* is used for storing NULL_PTR (of type const void *)
           and NvM_DstPtr (of type void *).
           As a consequence, a casting of NULL_PTR is needed. */
        /* Deviation MISRAC2012-2 */
        Result = NvM_Queue_InsertImmediate(BlockIndex, (P2VAR(uint8, AUTOMATIC,
                             NVM_APPL_DATA))NULL_PTR, &NvM_WritePRAMBlock_Async);
#else
        /* Compiler warning explanation: Because of memory optimization,
           the same pointer type uint8* is used for storing NULL_PTR (of type const void *)
           and NvM_DstPtr (of type void *).
           As a consequence, a casting of NULL_PTR is needed. */
        /* Deviation MISRAC2012-2 */
        Result = NvM_Queue_InsertStandard(BlockIndex, (P2VAR(uint8, AUTOMATIC,
                             NVM_APPL_DATA))NULL_PTR, &NvM_WritePRAMBlock_Async);
#endif

#if (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW != TS_PROD_ERR_DISABLE)
        if (E_OK != Result)
        {
          ReportQueueOverflow = TRUE;
        }
#endif
      }
    }
    else
    {
    }

    if (E_OK == Result)
    {
      /* Insert the API ID into NvM_AdminBlockTable.
       * The information shall be available in case the request has only been queued,
       * but the processing has not been started and a cancel request was issued.
       */
      NvM_AdminBlockTable[BlockIndex].NvMCurrentServiceId = NVM_WRITE_PRAM_BLOCK_API_ID;
    }
    SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

#if (NVM_PROD_ERR_HANDLING_WRITE_PROTECTED != TS_PROD_ERR_DISABLE)
    /* !LINKSTO SWS_NvM_00795,1 */
    if (ReportWriteProtected == TRUE)
    {
#if (NVM_PROD_ERR_HANDLING_WRITE_PROTECTED == TS_PROD_ERR_REP_TO_DEM)
#if (NVM_WRITE_PROT_DEM_EVENT_ID != NVM_INVALID_DEM_EVENTID )
      NVM_DEM_REPORT_ERROR(NVM_WRITE_PROT_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#endif
#elif (NVM_PROD_ERR_HANDLING_WRITE_PROTECTED == TS_PROD_ERR_REP_TO_DET)
      NVM_DET_REPORT_ERROR(NVM_WRITE_PRAM_BLOCK_API_ID, NVM_E_DEMTODET_WRITE_PROTECTED);
#elif ( NVM_PROD_ERR_HANDLING_WRITE_PROTECTED == TS_PROD_ERR_REP_TO_USER )
      NVM_PRODUCTION_ERRORS_FUNCTION_CALLOUT(BlockId, NVM_WRITE_PRAM_BLOCK_API_ID, NVM_E_USER_CALLOUT_WRITE_PROTECTED);
#endif
    }
#endif

#if (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW != TS_PROD_ERR_DISABLE)
    if (ReportQueueOverflow == TRUE)
    {
#if (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW == TS_PROD_ERR_REP_TO_DEM)
#if (NVM_QUEUE_OVERFLOW_DEM_EVENT_ID != NVM_INVALID_DEM_EVENTID )
      NVM_DEM_REPORT_ERROR(NVM_QUEUE_OVERFLOW_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#endif
#elif (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW == TS_PROD_ERR_REP_TO_DET)
      NVM_DET_REPORT_ERROR(NVM_WRITE_PRAM_BLOCK_API_ID, NVM_E_DEMTODET_QUEUE_OVERFLOW);
#elif ( NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW == TS_PROD_ERR_REP_TO_USER )
      NVM_PRODUCTION_ERRORS_FUNCTION_CALLOUT(BlockId, NVM_WRITE_PRAM_BLOCK_API_ID, NVM_E_USER_CALLOUT_QUEUE_OVERFLOW);
#endif
    }
#endif

#if ( NVM_DEV_ERROR_DETECT == STD_ON )
    if (ReportWriteOnceUnknown == TRUE)
    {
      NVM_DET_REPORT_ERROR( NVM_WRITE_PRAM_BLOCK_API_ID, NVM_E_WRITE_ONCE_STATUS_UNKNOWN );
    }
#endif

#if (NVM_BSWM_BLOCK_STATUS_INFO == STD_ON)
    /* Check if block status changed and block status reporting to BswM is
     * enabled for this block */
    if ((E_OK == Result) &&
        (NVM_BD_BLOCKSTATUSINFO(NvM_BlockDescriptorTable[BlockIndex].blockDesc)))
    {
      /* Block status changed due to queuing write request, report the
       * block status to BswM */
      BswM_NvM_CurrentBlockMode(BlockId, NVM_REQ_PENDING);
    }
#endif
#if ( defined(NVM_MULTICORE_ENABLED) )
    NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus &= (uint8)(~(NVM_ES_MULTICORE_LOCK_MASK));
#endif
  }
  DBG_NVM_ASR42_WRITEPRAMBLOCK_EXIT(Result, BlockId);
  return Result;

}/* end of function NvM_ASR42_WritePRAMBlock */

FUNC(void,NVM_CODE) NvM_WriteBlock_Async(void)
{
  /* Since the processing of NvM_WriteBlock() has
   * been started, NvM_CurrentServiceId needs to be set
   */
  NvM_CurrentServiceId = NVM_WRITE_BLOCK_API_ID;

#if ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U))

  if ((NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMExtendedStatus &
       NVM_ES_RAM_BLOCK_CRC_CALC_MASK) == NVM_ES_RAM_BLOCK_CRC_CALC_MASK)
  {
     NvM_CalcCrc_RemoveElement();
  }
#endif
#if (NVM_SYNC_MECHANISM == STD_ON)
  if (!(NVM_CHECK_WORKING_STATUS(NVM_DYN_MIRROR_MASK)))
#endif
  {
    /* !LINKSTO SWS_NvM_00303,1 */
    /* if permanent RAM is used  */
    NvM_SetPermanentRamBlockValid();
  }
  /* Proceed with state 1 */
  NvM_WriteBlock_Async_State1();
}/* end of function NvM_WriteBlock_Async */

FUNC(void,NVM_CODE) NvM_WritePRAMBlock_Async(void)
{
  /* Since the processing of NvM_WritePRAMBlock() has
   * been started, NvM_CurrentServiceId needs to be set
   */
  NvM_CurrentServiceId = NVM_WRITE_PRAM_BLOCK_API_ID;

#if ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U))

  if ((NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMExtendedStatus &
       NVM_ES_RAM_BLOCK_CRC_CALC_MASK) == NVM_ES_RAM_BLOCK_CRC_CALC_MASK)
  {

     NvM_CalcCrc_RemoveElement();

  }
#endif
#if (NVM_SYNC_MECHANISM == STD_ON)
  if (!(NVM_CHECK_WORKING_STATUS(NVM_DYN_MIRROR_MASK)))
#endif
  {
    /* !LINKSTO SWS_NvM_00803,1 */
    /* if permanent RAM is used  */
    NvM_SetPermanentRamBlockValid();
  }
  /* Proceed with state 1 */
  NvM_WriteBlock_Async_State1();
}/* end of function NvM_WritePRAMBlock_Async */

#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1) */

#if (NVM_CRC_COMP_MECH == STD_ON)

FUNC(void,NVM_CODE) NvM_WriteBlock_Async_CheckAndCopyCrc(void)
{
  NvM_RequestResultType CrcNotChanged = NVM_REQ_NOT_OK;
  /* !LINKSTO SWS_NvM_00852,1, SWS_NvM_00853,1, SWS_NvM_00854, 1 */
  /*Check first if a loss of redundancy occurred */
  if( NVM_CHECK_WORKING_STATUS(NVM_DYN_REDUNDANCY_MASK) == FALSE)
  {
    /*Check If CRC is the same*/
    CrcNotChanged = NvM_CheckCrc( NvM_GetStoredNvCRCAddress(),
                                 (NvM_PtrToApplDataType)(&NvM_CalcCrc_CalcBuffer),
                                  NvM_CurrentBlockDescriptorPtr->blockDesc);
  }
  else
  {
    /*In case of redundancy loss CRC compare will be skipped */
    CrcNotChanged = NVM_REQ_NOT_OK;
  }

  /* !LINKSTO SWS_NvM_00852,1, SWS_NvM_00853,1, SWS_NvM_00854, 1 */
  /* If the CRC is different then the one saved, then copy the calculated CRC
   * to the currently used write buffer (internal buffer or user RAM block) so
   * that user data and CRC can be written in a single call of the underlying MemIf module.
   */
  if(CrcNotChanged == NVM_REQ_NOT_OK)
  {
    NvM_CopyCrc( NvM_CalcCrc_GetBlockCrcAddress(),
                (NvM_PtrToApplDataType)&NvM_CalcCrc_CalcBuffer,
                 NvM_CurrentBlockDescriptorPtr->blockDesc);
    /* Prepare state machine for next state */
    NvM_WriteBlock_ProcessRedundantBlock(&NvM_WriteBlock_Async_State3);
  }
  else
  {
#if (NVM_PRE_WRITE_DATA_COMP == STD_ON)
/* !LINKSTO NvM.CompPreWrite.CrcComp,1  */
    if(NVM_BD_PRE_WRITE_DATA_COMP(NvM_CurrentBlockDescriptorPtr->blockDesc))
    {
#if (NVM_STATIC_BLOCKID_CHECK == STD_ON)
      if (NVM_BD_USESTATICBLOCKID(NvM_CurrentBlockDescriptorPtr->blockDesc))
      {
        /* Reset Pre Write Data Comp offset to start of data */
        NvM_VerifyOffset = NVM_BLOCKID_LENGTH;
      }
      else
#endif /* NVM_STATIC_BLOCKID_CHECK = STD_ON */
      {
        /* Reset Pre Write Data Comp offset */
        NvM_VerifyOffset = 0U;
      }
      /* set verification flag in the extended status */
      NVM_SET_EXT_STATUS(NVM_ES_PRE_WRITE_DATA_COMP_MASK);
      /* Pre Write Data Comparison */
      NvM_WriteBlock_MemIfReadBlock();
    }
    else
#endif /* NVM_PRE_WRITE_DATA_COMP == STD_ON */
    {
      /*CRC did not change. Finish the Write as it would have been successful*/
      NvM_WriteBlock_ProcessJobRes();
    }
  }

}/* end of function NvM_WriteBlock_Async_CheckAndCopyCrc */

#endif /* NVM_CRC_COMP_MECH == STD_ON */
FUNC(void,NVM_CODE) NvM_WriteBlock_Async_CopyCrc(void)
{
  /* Copy the calculated CRC copied to the currently used write buffer
   * (internal buffer or user RAM block) so that user data and CRC can be written
   * in a single call of the underlying MemIf module.
   */
  NvM_CopyCrc( NvM_CalcCrc_GetBlockCrcAddress(),
              (NvM_PtrToApplDataType)&NvM_CalcCrc_CalcBuffer,
               NvM_CurrentBlockDescriptorPtr->blockDesc);
#if (NVM_PRE_WRITE_DATA_COMP == STD_ON)
  if(NVM_BD_PRE_WRITE_DATA_COMP(NvM_CurrentBlockDescriptorPtr->blockDesc))
  {
    if(NVM_CHECK_WORKING_STATUS(NVM_DYN_REDUNDANCY_MASK) == FALSE)
    {
#if (NVM_STATIC_BLOCKID_CHECK == STD_ON)
      if (NVM_BD_USESTATICBLOCKID(NvM_CurrentBlockDescriptorPtr->blockDesc))
      {
        /* Reset Pre Write Data Comp offset to start of data */
        NvM_VerifyOffset = NVM_BLOCKID_LENGTH;
      }
      else
#endif /* NVM_STATIC_BLOCKID_CHECK = STD_ON */
      {
        /* Reset Pre Write Data Comparison offset */
        NvM_VerifyOffset = 0U;
      }
      /* set data comparison flag in the extended status */
      NVM_SET_EXT_STATUS(NVM_ES_PRE_WRITE_DATA_COMP_MASK);
      /* Pre Write Data Comp */
      NvM_WriteBlock_MemIfReadBlock();
    }
    else
    {
      /* Prepare state machine for next state */
      NvM_WriteBlock_ProcessRedundantBlock(&NvM_WriteBlock_Async_State3);
    }
  }
  else
#endif /* NVM_PRE_WRITE_DATA_COMP = STD_ON */
  {
#if (NVM_CRYPTO_HOOKS == STD_ON)
    if(NVM_BD_CRYPTOHOOKS(NvM_CurrentBlockDescriptorPtr->blockDesc))
    {
      NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_CryptoOperations_Async_State;
      NvM_PostCryptoState = NvM_WriteState;
    }
    else
#endif
    {
    /* Prepare state machine for next state */
    NvM_WriteBlock_ProcessRedundantBlock(&NvM_WriteBlock_Async_State3);
    }
  }

}/* end of function NvM_WriteBlock_Async_CopyCrc */

FUNC(void,NVM_CODE) NvM_WriteBlock_Async_Error(void)
{
#if (NVM_NUMBER_OF_WRITE_RETRY_BLOCKS > 0U)
  if ( NvM_WriteRetryCount < NvM_CurrentBlockDescriptorPtr->writeRetryLimit )
  {
    /* try to write same block again */
    NvM_WriteRetryCount++;
    NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_WriteBlock_Async_State3;
  }
  else
#endif
  {
    /* if a redundant block is configured then try to write the second
     * block else terminate service.
     * The entrance point of the next state is stored on the state stack
     * in both cases.
     */
#if (NVM_NUMBER_OF_WRITE_RETRY_BLOCKS > 0U)
    NvM_WriteRetryCount = 1U;
#endif

    NvM_WriteBlock_RedundantBlockError();

  }/* if ( NvM_WriteRetryCount < NvM_CurrentBlockDescriptorPtr->writeRetryLimit ) */
}/* end of function NvM_WriteBlock_Async_Error */

FUNC(void,NVM_CODE) NvM_WriteBlock_Async_State1(void)
{
  NvM_PtrToApplDataType TempUserRamAddress;

#if (NVM_SYNC_MECHANISM == STD_ON)
  NvM_MirrorRetryCount = 0U;
#endif
  TempUserRamAddress = NvM_GetUserRamBlockAddress();
  if (TempUserRamAddress != NULL_PTR)
  {
    if (NVM_CHECK_WORKING_STATUS(NVM_DYN_UPRB_MASK))
    {
      /* Hook function for WriteBlock */
      /* Deviation MISRAC2012-3 */
      NVM_WRITE_BLOCK_HOOK(NvM_GlobalBlockNumber,
        (NvM_PtrToApplDataType)NvM_BlockDescriptorTable[NvM_GlobalBlockNumber].ramBlockDataAddress,
        NvM_BlockDescriptorTable[NvM_GlobalBlockNumber].nvBlockLength
      );
    }

#if (NVM_NUMBER_OF_WRITE_RETRY_BLOCKS > 0U)
    /* Reset Write Retry Counter */
    NvM_WriteRetryCount = 1U;
#endif

    /* Set the state function for explicit synchronization */
    NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_WriteBlock_Async_State2;

    /* Check if write request is for block 1.
     * Restore configured Compiled Configuration Id to RAM block.
     */
    /* !LINKSTO SWS_NvM_00812,1 */
    /* !LINKSTO NvM.NvMSoftwareChangeCallout.SWS_NvM_00812,1 */
    /* !LINKSTO NVM547,1 */
    /* !LINKSTO NvM.NvMSoftwareChangeCallout.SWS_NvM_00547,1 */
    if (NvM_GlobalBlockNumber == NVM_BLOCK_CONFIG_ID)
    {
      DBG_NVM_GLOBALCALLLEVEL((NvM_GlobalCallLevel),((NvM_GlobalCallLevel)+1U));
      NvM_GlobalCallLevel++;
      NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_RestoreBlockDefaults_AscSt0;
    }
  } /* TempUserRamAddress != NULL_PTR */
}/* end of function NvM_WriteBlock_Async_State1 */

FUNC(void,NVM_CODE) NvM_WriteBlock_Async_State2(void)
{
  NvM_PtrToApplDataType BufferAddr = NvM_GetDataAddress();

#if (NVM_SYNC_MECHANISM == STD_ON)
  Std_ReturnType ReturnVal = E_OK;
#endif

#if (NVM_SYNC_MECHANISM == STD_ON)
  /* For single block write job, call the callback function to
   * copy data from the RAM block to NvM module's mirror
   */
  if (NVM_CHECK_WORKING_STATUS(NVM_DYN_MIRROR_MASK))
  {
    ReturnVal = NvM_MirrorCopy(NULL_PTR, NvM_CurrentBlockDescriptorPtr->writeRamToNvCallback, BufferAddr);
  }

  if (E_OK == ReturnVal)
#endif
  {
    /* !LINKSTO SWS_NvM_00303,1 */
    /* Mark the block as valid after mirror copy Block valid if it is currently used */
    NvM_SetPermanentRamBlockValid();


    /* If internal buffer is used, Copy data from user RAM block
     * and Block Id (if required) to internal buffer.
     */
    if ((NVM_BD_USEINTERNALBUFFER(NvM_CurrentBlockDescriptorPtr->blockDesc)) ||
        (NVM_CHECK_WORKING_STATUS(NVM_DYN_MIRROR_MASK))
       )
    {
      NvM_FillInternalBuffer();
    }

    /* Calculate CRC if it is configured for this NVRAM Block */
    /* !LINKSTO NVM127,1 */
    if (NVM_BD_CRCTYPE(NvM_CurrentBlockDescriptorPtr->blockDesc) != NVM_BD_CRCTYPE_NONE)
    {
#if (NVM_CRC_COMP_MECH == STD_ON)
      if( NVM_BD_ENCRCCOMP(NvM_CurrentBlockDescriptorPtr->blockDesc) )
      {
        NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_WriteBlock_Async_CheckAndCopyCrc;
      }
      else
#endif /* NVM_CRC_COMP_MECH == STD_ON */
      {
        NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_WriteBlock_Async_CopyCrc;
      }
      NvM_CalculateCrc(BufferAddr);
    }
    else
    {
#if (NVM_PRE_WRITE_DATA_COMP == STD_ON)
      if(NVM_BD_PRE_WRITE_DATA_COMP(NvM_CurrentBlockDescriptorPtr->blockDesc))
      {
        if(NVM_CHECK_WORKING_STATUS(NVM_DYN_REDUNDANCY_MASK) == FALSE)
        {
#if (NVM_STATIC_BLOCKID_CHECK == STD_ON)
          if (NVM_BD_USESTATICBLOCKID(NvM_CurrentBlockDescriptorPtr->blockDesc))
          {
            /* Reset Pre Write Data Comparison offset to start of data */
            NvM_VerifyOffset = NVM_BLOCKID_LENGTH;
          }
          else
#endif /* NVM_STATIC_BLOCKID_CHECK = STD_ON */
          {
            /* Reset Pre Write Data Comparison offset */
            NvM_VerifyOffset = 0U;
          }
          /* set Data Comparison flag in the extended status */
          NVM_SET_EXT_STATUS(NVM_ES_PRE_WRITE_DATA_COMP_MASK);
          /* Pre Write Data Comparison */
          NvM_WriteBlock_MemIfReadBlock();
        }
        else
        {
          /* Prepare state machine for next state */
          NvM_WriteBlock_ProcessRedundantBlock(&NvM_WriteBlock_Async_State3);
        }
      }
      else
#endif /* NVM_PRE_WRITE_DATA_COMP = STD_ON */
      {
#if (NVM_CRYPTO_HOOKS == STD_ON)
        if(NVM_BD_CRYPTOHOOKS(NvM_CurrentBlockDescriptorPtr->blockDesc))
        {
          NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_CryptoOperations_Async_State;
          NvM_PostCryptoState = NvM_WriteState;
        }
        else
#endif
        {
          /* Prepare state machine for next state */
          NvM_WriteBlock_ProcessRedundantBlock(&NvM_WriteBlock_Async_State3);
        }
      }
    }
  }
}

FUNC(void,NVM_CODE) NvM_WriteBlock_Async_State3(void)
{


  /* In case writeAll is active and this is part of writeAll and it was canceled do not write the block */
  /* If this is second block of redundant allow it to write */
  if((( NvM_CurrentServiceId == NVM_WRITE_ALL_API_ID ) &&
      (NVM_CHECK_GLOBAL_GENERIC_STATUS(NVM_GENERIC_CWA_MASK))) &&
      !(( NVM_BD_BLOCKMNGTTYPE(NvM_CurrentBlockDescriptorPtr->blockDesc) ==
         NVM_BD_BLOCKMNGTTYPE_REDUNDANT
      )&&(NVM_CHECK_GLOBAL_GENERIC_STATUS_FALSE(NVM_WRITE_REDUNDANT_MASK))))
  {
    /* Exit without marking block, flag will be processed on next main function */
    NvM_StateMachine_TerminateCurrentLevel();
    /* Flag to restore the block status so it may be written later */
    NVM_SET_GLOBAL_GENERIC_STATUS(NVM_WRITE_ALL_PREVENTED_MASK);
  }
  else
  {
   /* For blocks with standard or immediate priority,
   * job cannot be started if MemIf is busy.
   */
    if (MEMIF_BUSY != MemIf_GetStatus( NVM_BD_DEVICEID(NvM_CurrentBlockDescriptorPtr->blockDesc)))
    {
      /* call MemIf_Write() and evaluate result. */
      if ( MemIf_Write(
          NVM_BD_DEVICEID(NvM_CurrentBlockDescriptorPtr->blockDesc),
          NvM_GetMergedBlockNumber(),
          NvM_GetBufferAddress()) == E_OK
      )
      {
#if (NVM_WRITE_VERIFICATION == STD_ON)
#if (NVM_STATIC_BLOCKID_CHECK == STD_ON)
        if (NVM_BD_USESTATICBLOCKID(NvM_CurrentBlockDescriptorPtr->blockDesc))
        {
          /* Reset Write verification data offset to start of data */
          NvM_VerifyOffset = NVM_BLOCKID_LENGTH;
        }
        else
#endif /* NVM_STATIC_BLOCKID_CHECK = STD_ON */
        {
          /* Reset Write verification data offset */
          NvM_VerifyOffset = 0U;
        }
#endif  /* NVM_WRITE_VERIFICATION = STD_ON */

        NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_WriteBlock_ProcessJobRes_Async_State;
      }
      else
      {
        NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_WriteBlock_Async_Error;
      }
    }
    else
    {
      /* If the MemIf returns busy, return from MainFunction */

      DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_PENDING));
      NvM_GlobalErrorStatus = NVM_REQ_PENDING;
    }
  }

}/* end of function NvM_WriteBlock_Async_State3 */

FUNC(void,NVM_CODE) NvM_WriteBlock_ProcessJobRes_Async_State(void)
{
  const MemIf_JobResultType Result =
    MemIf_GetJobResult(NVM_BD_DEVICEID(NvM_CurrentBlockDescriptorPtr->blockDesc));

  /* evaluate Result */
  if (Result == MEMIF_JOB_OK)
  {
#if (NVM_WRITE_VERIFICATION == STD_ON)
    /* Check write verification required */
    if (NVM_BD_WRITE_VERIFICATION(NvM_CurrentBlockDescriptorPtr->blockDesc))
    {
      /* set verification flag in the extended status */
      NVM_SET_EXT_STATUS(NVM_ES_WRITE_VERIFICATION_MASK);
      /* Write Verification */
      NvM_WriteBlock_MemIfReadBlock();
    }
    else
#endif /* NVM_WRITE_VERIFICATION = STD_ON */
    {
      /* Skip Write Verification and process Job Result */
      NvM_WriteBlock_ProcessJobRes();
    }
  }
  else if ( Result == MEMIF_JOB_CANCELED )
  {
    /* Clear first of redundant blocks flag when it's done with the abstraction*/
    NVM_CLEAR_GLOBAL_GENERIC_STATUS(NVM_WRITE_REDUNDANT_MASK);
    DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_CANCELLED));
    NvM_GlobalErrorStatus = NVM_REQ_CANCELLED;
    /* set CallLevel to zero to terminate this service completely */

    DBG_NVM_GLOBALCALLLEVEL((NvM_GlobalCallLevel),(0U));
    NvM_GlobalCallLevel = 0U;

    NvM_ResetRedundantBlockDataIndex();
  }
  else if (Result == MEMIF_JOB_PENDING)
  {

    DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_PENDING));
    NvM_GlobalErrorStatus = NVM_REQ_PENDING;
  }
  else
  {
    NvM_WriteBlock_Async_Error();
  }/* evaluate Result */
}/* end of function NvM_WriteBlock_ProcessJobRes_Async_State */

FUNC(void,NVM_CODE) NvM_WriteBlock_ProcessJobRes(void)
{
  /* Write was successful */
  NvM_GlobalErrorStatus = NVM_REQ_OK;

  if ( NVM_BD_BLOCKMNGTTYPE(NvM_CurrentBlockDescriptorPtr->blockDesc) ==
         NVM_BD_BLOCKMNGTTYPE_REDUNDANT
     )
  {
    NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMDatasetBlockIndex =
      NVM_INVERT_DATAINDEX(NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMDatasetBlockIndex);
    /* Clear CRC mismatch flag */
    NVM_CLEAR_EXT_STATUS(NVM_ES_REDUNDANT_CRC_ERROR_MASK);
    /* Clear first of redundant blocks flag when it's done with the abstraction*/
    NVM_CLEAR_GLOBAL_GENERIC_STATUS(NVM_WRITE_REDUNDANT_MASK);
#if (NVM_NUMBER_OF_WRITE_RETRY_BLOCKS > 0U)
    /* Reset Write Retry Counter */
    NvM_WriteRetryCount = 1U;
#endif
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
  NvM_SetPermanentRamBlockUnchanged();

#if (NVM_WRITE_BLOCK_ONCE == STD_ON)
  if ( NVM_BD_WRITEONCE(NvM_CurrentBlockDescriptorPtr->blockDesc))
  {
    /* Set WriteProtectionFlag and WriteProtectionPermanentFlag */
    NVM_SET_WORKING_STATUS( NVM_DYN_WPF_MASK|NVM_DYN_WPPF_MASK );
    /* The Extended Runtime Block Flag must be reset. It indicates
     * that the block address changed in case NvM Dynamic
     * Configuration is enabled. Now the data is written to the new
     * address and therefore the flag can be reset.
     */
    NVM_CLEAR_WORKING_STATUS( NVM_ERBF_MASK );
  }
#endif

  NvM_StateMachine_TerminateCurrentLevel();
}/* end of function NvM_WriteBlock_ProcessJobRes */

FUNC(void,NVM_CODE) NvM_WriteBlock_ProcessRedundantBlock
(
  NvM_CurrentFunctionPtrType NextState
)
{
  boolean writeBoth = FALSE;

  /* Set the next state function for processing a block */
  NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = NextState;

  if ( NVM_BD_BLOCKMNGTTYPE(NvM_CurrentBlockDescriptorPtr->blockDesc) ==
         NVM_BD_BLOCKMNGTTYPE_REDUNDANT
     )
  {
    /* reset error counter */
    NVM_REDUNDANT_ERROR_COUNTER = 0U;

    /* Check which of the two copies has not been read successfully.
     * If first copy of a redundant block has not been read successfully
     * the second copy shall be written first. If reading second copy
     * failed the first copy shall be written first.
     */
    if ((NVM_CHECK_WORKING_STATUS(NVM_DYN_REDUNDANCY_MASK)) &&
        (NVM_CHECK_EXT_STATUS(NVM_ES_REDUNDANT_ERROR_MASK))
       )
    {
      NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMDatasetBlockIndex = 1U;
    }
    else
    {
      NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMDatasetBlockIndex = 0U;
    }

    /* !LINKSTO NvM.Redundant.WriteAll.Order,1, NvM.Redundant.WriteAll.NvMSetRamBlockStatusApi.Disabled,1 */
    /* Check if both copies have to be written with data from RAM */
#if (NVM_SET_RAM_BLOCK_STATUS_API == STD_ON)
    if (
          (NVM_CHECK_WORKING_STATUS( NVM_DYN_CHANGE_MASK)) ||
          (NVM_WRITE_BLOCK_API_ID == NvM_CurrentServiceId) ||
          (NVM_WRITE_PRAM_BLOCK_API_ID == NvM_CurrentServiceId) ||
          (NVM_BD_USERAMBLOCKSTATUS(NvM_CurrentBlockDescriptorPtr->blockDesc) == FALSE)
        )
#endif
    {
      /* If the block is marked as changed or if the user requested
       * NvM_WriteBlock(), both copies shall be written
       */
      DBG_NVM_GLOBALCALLLEVEL((NvM_GlobalCallLevel),((NvM_GlobalCallLevel)+1U));
      NvM_GlobalCallLevel++;
      NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = NextState;
      writeBoth = TRUE;
      /* Do not write block on write all cancel if flag is set */
      NVM_SET_GLOBAL_GENERIC_STATUS(NVM_WRITE_REDUNDANT_MASK);
    }
    /* Check if only one copy of the redundant block failed  */
    if ( (writeBoth == FALSE) &&
         ((NVM_CHECK_WORKING_STATUS(NVM_DYN_REDUNDANCY_MASK)) ||
          (NVM_CHECK_EXT_STATUS(NVM_ES_REDUNDANT_CRC_ERROR_MASK))))
    {
      /* Do not write block on write all cancel if flag is set */
      NVM_SET_GLOBAL_GENERIC_STATUS(NVM_WRITE_REDUNDANT_MASK);
      /* Set dataset to the correct copy */
      NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMDatasetBlockIndex =
        NVM_INVERT_DATAINDEX(NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMDatasetBlockIndex);

      /* If both copies have been successfully read but CRCs are different
       * the correct dataset can not be determined.
       */
      if (NVM_CHECK_EXT_STATUS(NVM_ES_REDUNDANT_CRC_ERROR_MASK))
      {
        /* Set the active dataset to second copy */
        NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMDatasetBlockIndex = 1U;
      }

      /* !LINKSTO NvM.Redundant.WriteAll.Failed,1 */
      /* Set read redundant block flag in the extended status */
      NVM_SET_EXT_STATUS(NVM_ES_READ_REDUNDANT_MASK);
      /* Initiate read from NV memory for the correct copy */
      NvM_WriteBlock_MemIfReadBlock();
    }
  }

}/* end of function NvM_WriteBlock_ProcessRedundantBlock */

STATIC FUNC(void,NVM_CODE) NvM_WriteBlock_ProcessRedundantBlock_State1(void)
{
  MemIf_JobResultType Result = MEMIF_JOB_FAILED;

  /* Get result of the read operation from underlying layer */
  Result = MemIf_GetJobResult(NVM_BD_DEVICEID(NvM_CurrentBlockDescriptorPtr->blockDesc));

  /* Check the job result */
  if (MEMIF_JOB_PENDING == Result)
  {
    DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_PENDING));
    NvM_GlobalErrorStatus = NVM_REQ_PENDING;
  }
  else if(MEMIF_JOB_OK == Result)
  {
    /* set the data set to be written */
    NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMDatasetBlockIndex =
        NVM_INVERT_DATAINDEX(NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMDatasetBlockIndex);
    NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_WriteBlock_Async_State3;
  }
  else
  {
    /* No retry in this case */
    NvM_GlobalErrorStatus = NVM_REQ_OK;
    NvM_StateMachine_TerminateCurrentLevel();
  }
  /* clear read redundant block flag in the extended status */
  NVM_CLEAR_EXT_STATUS(NVM_ES_READ_REDUNDANT_MASK);
}/* end of function NvM_WriteBlock_ProcessRedundantBlock */

STATIC FUNC(void,NVM_CODE) NvM_WriteBlock_RedundantBlockError(void)
{
  /* In case of redundant blocks, if the error counter is 0,
   * retry the write operation with the other block.
   * Otherwise report the Dem error.
   * In case of other block types, report the error straightaway.
   */

  if ( ( NVM_BD_BLOCKMNGTTYPE(NvM_CurrentBlockDescriptorPtr->blockDesc) ==
           NVM_BD_BLOCKMNGTTYPE_REDUNDANT
       ) &&
       ( NVM_REDUNDANT_ERROR_COUNTER == 0U )
     )
  {
      /* Clear first of redundant blocks flag when it's done with the abstraction*/
      NVM_CLEAR_GLOBAL_GENERIC_STATUS(NVM_WRITE_REDUNDANT_MASK);
      /* Write the other block also */
      NVM_REDUNDANT_ERROR_COUNTER++;

      NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMDatasetBlockIndex =
        NVM_INVERT_DATAINDEX(NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMDatasetBlockIndex);
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

}/* end of function NvM_WriteBlock_RedundantBlockError */

#if (NVM_WRITE_VERIFICATION == STD_ON)

FUNC(void,NVM_CODE) NvM_WriteBlock_Verify_Async_State(void)
{
  boolean ExecuteBlockVerifyDefault = TRUE;  /* Flag used to identify the
                                                correct verification address*/

  const MemIf_JobResultType Result =
    MemIf_GetJobResult(NVM_BD_DEVICEID(NvM_CurrentBlockDescriptorPtr->blockDesc));

  /* Evaluate the read result */
  if (Result == MEMIF_JOB_OK)
  {
    NvM_PtrToApplDataType VerifyAddress = NvM_GetUserRamBlockAddress();

#if (NVM_STATIC_BLOCKID_CHECK == STD_ON)
    if (NVM_BD_USESTATICBLOCKID(NvM_CurrentBlockDescriptorPtr->blockDesc))
    {
#if (NVM_SYNC_MECHANISM == STD_ON)
      if (!(NVM_CHECK_WORKING_STATUS(NVM_DYN_MIRROR_MASK)))
#endif
      {
        VerifyAddress = &(VerifyAddress[NvM_VerifyOffset - NVM_BLOCKID_LENGTH]);
        ExecuteBlockVerifyDefault = FALSE;
      }
    }
#endif
    /* Deviation TASKING-1 */
    if (ExecuteBlockVerifyDefault == TRUE)
    {
      VerifyAddress = &(VerifyAddress[NvM_VerifyOffset]);
    }
    /* Compare RAM data and read back data */
    if (TS_MemCmp(VerifyAddress, NvM_IntermediateReadBuffer, NvM_GetVerifyDataSize()) != E_OK)
    {
#if ( NVM_PROD_ERR_HANDLING_VERIFY_FAILED == TS_PROD_ERR_REP_TO_DEM )
#if (NVM_VERIFY_FAILED_DEM_EVENT_ID != NVM_INVALID_DEM_EVENTID )
      NVM_DEM_REPORT_ERROR(NVM_VERIFY_FAILED_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#endif
#elif ( NVM_PROD_ERR_HANDLING_VERIFY_FAILED == TS_PROD_ERR_REP_TO_DET )
      NVM_DET_REPORT_ERROR( NvM_CurrentServiceId, NVM_E_DEMTODET_VERIFY_FAILED );
#elif ( NVM_PROD_ERR_HANDLING_VERIFY_FAILED == TS_PROD_ERR_REP_TO_USER )
      NVM_PRODUCTION_ERRORS_FUNCTION_CALLOUT(NvM_BlockDescriptorTable[NvM_GlobalBlockNumber].nvBlockIdentifier, NvM_CurrentServiceId, NVM_E_USER_CALLOUT_VERIFY_FAILED);
#endif

      /* Write verification failed. Do write retry */
      NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_WriteBlock_Async_Error;
    }
    else
    {
      /* Next data size to verify */
      NvM_VerifyOffset += NvM_CurrentBlockDescriptorPtr->verifyDataSize;

      if (NvM_VerifyOffset < NvM_GetBlockLength(NvM_CurrentBlockDescriptorPtr->blockDesc,
                             NvM_CurrentBlockDescriptorPtr->nvBlockLength))
      {
        /* set verification flag in the extended status */
        NVM_SET_EXT_STATUS(NVM_ES_WRITE_VERIFICATION_MASK);
        /* Do Write Verification for the remaining data */
        NvM_WriteBlock_MemIfReadBlock();
      }
      else
      {
        /* write verification successfully finished */
        NvM_WriteBlock_ProcessJobRes();
      }
    }
  }
  else if ( Result == MEMIF_JOB_PENDING)
  {

    DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_PENDING));
    NvM_GlobalErrorStatus = NVM_REQ_PENDING;
  }
  else
  {
    /* !LINKSTO NvM.VariableBlockLength.PostWrite,1 */
    /* write verification skipped due to read failure */
    NvM_WriteBlock_ProcessJobRes();
  }

}/* end of function NvM_WriteBlock_Verify_Async_State */

STATIC FUNC(uint16, NVM_CODE) NvM_GetVerifyDataSize(void)
{
  uint16 TotalDataSize = NvM_GetBlockLength( NvM_CurrentBlockDescriptorPtr->blockDesc,
                                             NvM_CurrentBlockDescriptorPtr->nvBlockLength);
  uint16 VerifyDataSize = NvM_VerifyOffset + NvM_CurrentBlockDescriptorPtr->verifyDataSize;

  if (TotalDataSize >= VerifyDataSize)
  {
    VerifyDataSize = NvM_CurrentBlockDescriptorPtr->verifyDataSize;
  }
  else
  {
    VerifyDataSize = TotalDataSize - NvM_VerifyOffset;
  }

  return VerifyDataSize;
}/* end of function NvM_GetVerifyDataSize */

#endif /* NVM_WRITE_VERIFICATION = STD_ON */

#if (NVM_PRE_WRITE_DATA_COMP == STD_ON)
  /* !LINKSTO NvM.CompPreWrite.WritePRAM,1 , NvM.CompPreWrite.Write,1 ,NvM.CompPreWrite.WriteAll,1 */
FUNC(void,NVM_CODE) NvM_WriteBlock_PreWriteDataComp_Async_State(void)
{
  boolean ExecuteBlockVerifyDefault = TRUE;  /* Flag used to identify the
                                                correct verification address*/

  const MemIf_JobResultType Result =
    MemIf_GetJobResult(NVM_BD_DEVICEID(NvM_CurrentBlockDescriptorPtr->blockDesc));

  NvM_PtrToApplDataType VerifyAddress = NvM_GetUserRamBlockAddress();
  /* Evaluate the read result */
  if (Result == MEMIF_JOB_OK)
  {

#if (NVM_STATIC_BLOCKID_CHECK == STD_ON)
    if (NVM_BD_USESTATICBLOCKID(NvM_CurrentBlockDescriptorPtr->blockDesc))
    {
#if (NVM_SYNC_MECHANISM == STD_ON)
      if (!(NVM_CHECK_WORKING_STATUS(NVM_DYN_MIRROR_MASK)))
#endif
      {
        VerifyAddress = &(VerifyAddress[NvM_VerifyOffset - NVM_BLOCKID_LENGTH]);
        ExecuteBlockVerifyDefault = FALSE;
      }
    }
#endif
    if (ExecuteBlockVerifyDefault == TRUE)
    {
      VerifyAddress = &(VerifyAddress[NvM_VerifyOffset]);
    }
    /* Compare RAM data and read back data */
    if (TS_MemCmp(VerifyAddress, NvM_IntermediateReadBuffer, NvM_GetPreWriteDataComparisonDataSize()) != E_OK)
    {
      /* Data Comparison failed, Proceed with normal Write */
      /* Prepare state machine for next state */
      NvM_WriteBlock_ProcessRedundantBlock(&NvM_WriteBlock_Async_State3);
    }
    else
    {
      /* Next data size to verify */
      NvM_VerifyOffset += NvM_CurrentBlockDescriptorPtr->preWriteDataComparisonDataSize;

      if (NvM_VerifyOffset < NvM_GetBlockLength(NvM_CurrentBlockDescriptorPtr->blockDesc,
                                                NvM_CurrentBlockDescriptorPtr->nvBlockLength))
      {
        /* set verification flag in the extended status */
        NVM_SET_EXT_STATUS(NVM_ES_PRE_WRITE_DATA_COMP_MASK);
        /* Do Pre Write Data Comparison for the remaining data */
        NvM_WriteBlock_MemIfReadBlock();
      }
      else
      {
        if ( ( NVM_BD_BLOCKMNGTTYPE(NvM_CurrentBlockDescriptorPtr->blockDesc) ==
               NVM_BD_BLOCKMNGTTYPE_REDUNDANT
              )  &&
             ( NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMDatasetBlockIndex == 0U )
            )
        {
          /* set verification flag in the extended status */
          NVM_SET_EXT_STATUS(NVM_ES_PRE_WRITE_DATA_COMP_MASK);
          NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMDatasetBlockIndex =
            NVM_INVERT_DATAINDEX(NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMDatasetBlockIndex);
#if (NVM_STATIC_BLOCKID_CHECK == STD_ON)
          if (NVM_BD_USESTATICBLOCKID(NvM_CurrentBlockDescriptorPtr->blockDesc))
          {
            /* Reset Pre Write Data Comparison offset to start of data */
            NvM_VerifyOffset = NVM_BLOCKID_LENGTH;
          }
          else
#endif /* NVM_STATIC_BLOCKID_CHECK = STD_ON */
          {
            /* Reset Pre Write Data Comparison offset */
            NvM_VerifyOffset = 0U;
           }
           /* Pre Write Data Comparison */
           NvM_WriteBlock_MemIfReadBlock();
        }
        else
        {
          /* write verification successfully finished */
          NvM_WriteBlock_ProcessJobRes();
        }
      }
    }
  }
  else if ( Result == MEMIF_JOB_PENDING)
  {

    DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_PENDING));
    NvM_GlobalErrorStatus = NVM_REQ_PENDING;
  }
  else
  {
    /* !LINKSTO NvM.VariableBlockLength.PreWrite,1 */
    /* write verification skipped due to read failure */
    /* Proceed with normal Write */
    /* Prepare state machine for next state */
    NvM_WriteBlock_ProcessRedundantBlock(&NvM_WriteBlock_Async_State3);
  }

}/* end of function NvM_WriteBlock_PreWriteDataComp_Async_State */

FUNC(void,NVM_CODE) NvM_WriteBlock_PreWriteDataComp_ReadError_State(void)
{
  NvM_WriteBlock_ProcessRedundantBlock(&NvM_WriteBlock_Async_State3);
}/* end of function NvM_WriteBlock_PreWriteDataComp_ReadError_State */

STATIC FUNC(uint16, NVM_CODE) NvM_GetPreWriteDataComparisonDataSize(void)
{
  uint16 TotalDataSize = NvM_GetBlockLength( NvM_CurrentBlockDescriptorPtr->blockDesc,
                                             NvM_CurrentBlockDescriptorPtr->nvBlockLength);
  uint16 VerifyDataSize = NvM_VerifyOffset + NvM_CurrentBlockDescriptorPtr->preWriteDataComparisonDataSize;

  if (TotalDataSize >= VerifyDataSize)
  {
    VerifyDataSize = NvM_CurrentBlockDescriptorPtr->preWriteDataComparisonDataSize;
  }
  else
  {
    VerifyDataSize = TotalDataSize - NvM_VerifyOffset;
  }

  return VerifyDataSize;
}/* end of function NvM_GetPreWriteDataComparisonDataSize */

#endif

STATIC FUNC(void, NVM_CODE) NvM_WriteBlock_MemIfReadBlock(void)
{
  uint16 Length = NvM_GetBlockLength( NvM_CurrentBlockDescriptorPtr->blockDesc,
                                      NvM_CurrentBlockDescriptorPtr->nvBlockLength) +
#if (NVM_CRYPTO_HOOKS == STD_ON)
                  NvM_CurrentBlockDescriptorPtr->CryptoExtraInfoSize +
#endif
                  (uint16)NvM_GetBlockCRCLength(NvM_CurrentBlockDescriptorPtr->blockDesc);

  CONST (MemIf_StatusType, AUTOMATIC) Status =
    MemIf_GetStatus(NVM_BD_DEVICEID(NvM_CurrentBlockDescriptorPtr->blockDesc));

  if (MEMIF_IDLE == Status)
  {
#if (NVM_WRITE_VERIFICATION == STD_ON)
    /* Read data for write verification */
    if (NVM_CHECK_EXT_STATUS(NVM_ES_WRITE_VERIFICATION_MASK))
    {
      NVM_CLEAR_EXT_STATUS(NVM_ES_WRITE_VERIFICATION_MASK);
      if ( MemIf_Read(
            NVM_BD_DEVICEID(NvM_CurrentBlockDescriptorPtr->blockDesc),
            NvM_GetMergedBlockNumber(),
            NvM_VerifyOffset,
            NvM_IntermediateReadBuffer,
            NvM_GetVerifyDataSize()) == E_OK
          )
      {
        NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_WriteBlock_Verify_Async_State;
      }
      else
      {
        /* Write verification skipped due to read failure */
        NvM_WriteBlock_ProcessJobRes();
      }
    }
#endif /* NVM_WRITE_VERIFICATION = STD_ON */
#if (NVM_PRE_WRITE_DATA_COMP == STD_ON)
    /* Read data for pre write Data Comparison */
    if (NVM_CHECK_EXT_STATUS(NVM_ES_PRE_WRITE_DATA_COMP_MASK))
    {
      NVM_CLEAR_EXT_STATUS(NVM_ES_PRE_WRITE_DATA_COMP_MASK);
      if ( MemIf_Read(
            NVM_BD_DEVICEID(NvM_CurrentBlockDescriptorPtr->blockDesc),
            NvM_GetMergedBlockNumber(),
            NvM_VerifyOffset,
            NvM_IntermediateReadBuffer,
            NvM_GetPreWriteDataComparisonDataSize()) == E_OK
          )
      {
        NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_WriteBlock_PreWriteDataComp_Async_State;
      }
      else
      {
        /* !LINKSTO NvM.CompPreWrite.WritePRAM,1 , NvM.CompPreWrite.Write,1 ,NvM.CompPreWrite.WriteAll,1 */
        /* Data comparison skipped due to read failure */
        /* Prepare state machine for next state */
        NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_WriteBlock_PreWriteDataComp_ReadError_State;
      }
    }
#endif /* NVM_PRE_WRITE_DATA_COMP = STD_ON */
    /* Redundant block is read for recovery */
    if (NVM_CHECK_EXT_STATUS(NVM_ES_READ_REDUNDANT_MASK))
    {
      if ( MemIf_Read(
            NVM_BD_DEVICEID(NvM_CurrentBlockDescriptorPtr->blockDesc),
            NvM_GetMergedBlockNumber(),
            0U, NvM_GetBufferAddress(),
            Length) == E_OK
         )
      {
        /* Set next state for processing the block */
        NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_WriteBlock_ProcessRedundantBlock_State1;
      }
      else
      {
        /* No retry in this case */
        NvM_GlobalErrorStatus = NVM_REQ_OK;
        NvM_StateMachine_TerminateCurrentLevel();
      }
    }
  }
  else
  {
    /* If the MemIf returns busy, set current function pointer and return from MainFunction */
    NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_WriteBlock_MemIfReadBlock;

    DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_PENDING));
    NvM_GlobalErrorStatus = NVM_REQ_PENDING;
  }
}/* end of function NvM_WriteBlock_MemIfReadBlock */

STATIC FUNC(void, NVM_CODE) NvM_FillInternalBuffer(void)
{
  NvM_PtrToApplDataType DataAddress = NvM_GetDataAddress();
#if (NVM_STATIC_BLOCKID_CHECK == STD_ON)
  NvM_PtrToApplDataType BufAddress = NvM_GetBufferAddress();
#endif

#if (NVM_INTERNAL_BUFFER_SIZE != 0U)
  /* Copy data from user RAM block to internal buffer */
  /* In case mirror is used data has already been copied to internal buffer */
  if ((NVM_BD_USEINTERNALBUFFER(NvM_CurrentBlockDescriptorPtr->blockDesc)) &&
     (!(NVM_CHECK_WORKING_STATUS(NVM_DYN_MIRROR_MASK))))
  {
    TS_MemCpy(DataAddress, NvM_GetUserRamBlockAddress(),
             NvM_CurrentBlockDescriptorPtr->nvBlockLength);
  }
#endif

#if (NVM_STATIC_BLOCKID_CHECK == STD_ON)
  /* If Static Block Id is enabled,
   * Copy the Block Identifier to internal buffer (NVM522, NVM523).
   */
  if (NVM_BD_USESTATICBLOCKID(NvM_CurrentBlockDescriptorPtr->blockDesc))
  {
    /* Deviation MISRAC2012-1 */
    TS_MemCpy(BufAddress, NVM_STATIC_BLOCKID(), NVM_BLOCKID_LENGTH);
  }
#endif
}

#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>

/*==================[end of file NvM_WriteBlock.c]==========================*/
