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

/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true violated
 *
 * Reason: false positive, see tasking issue TCVX-41885
 *
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

#include <MemIf.h>                /* Contains extern definition of function */
                                  /* MemIf_Cancel().                        */
/* !LINKSTO SWS_NvM_00554,2 */
#include <Dem.h>                  /* General Dem header file                */
/* !LINKSTO SWS_NvM_00554,2 */
#include <NvM.h>                  /* General NvM header file                */
#include <NvM_Cbk.h>              /* Callback API                           */
#include <NvM_FunctionIntern.h>   /* NvM functions which are used           */
                                  /* only within the NvM module.            */
#include <NvM_DataIntern.h>       /* NvM data which is used only            */
                                  /* within the NvM module.                 */
#include <NvM_Queue.h>            /* extern definitions of unit queue.      */
#include <NvM_StateMachine.h>     /* Contains extern definitions for unit   */
                                  /* 'StateMachine'. It can be included in  */
                                  /* library source code files.             */
#include <NvM_CalcCrc.h> /* Contains extern definitions for unit 'CalcCrc'. */
#include <NvM_Wrapper.h>
#include <NvM_BlockCheck.h>
#if ( NVM_INCLUDE_RTE == STD_ON )
#include <Rte_NvM.h>           /* Rte type definitions.                  */
#include <NvM_BSW.h>         /* NvM's common API */
#endif
#if ( NVM_DEV_ERROR_DETECT == STD_ON )
/* !LINKSTO NVM556,1 */
#include <Det.h>               /* API of module Det.                     */
#endif

#if ((NVM_BSWM_MULTI_BLOCK_JOB_STATUS_INFO == STD_ON) || \
     (NVM_BSWM_BLOCK_STATUS_INFO == STD_ON))
#include <BswM_NvM.h>          /* API of module BswM */
#endif

/*==================[macros]=================================================*/

/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef NVM_VENDOR_ID /* configuration check */
#error NVM_VENDOR_ID must be defined
#endif

#if (NVM_VENDOR_ID != 1U) /* vendor check */
#error NVM_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef NVM_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error NVM_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (NVM_AR_RELEASE_MAJOR_VERSION != 4U)
#error NVM_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef NVM_AR_RELEASE_MINOR_VERSION /* configuration check */
#error NVM_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (NVM_AR_RELEASE_MINOR_VERSION != 0U )
#error NVM_AR_RELEASE_MINOR_VERSION wrong (!= 0U)
#endif

#ifndef NVM_AR_RELEASE_REVISION_VERSION /* configuration check */
#error NVM_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (NVM_AR_RELEASE_REVISION_VERSION != 3U )
#error NVM_AR_RELEASE_REVISION_VERSION wrong (!= 3U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef NVM_SW_MAJOR_VERSION /* configuration check */
#error NVM_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (NVM_SW_MAJOR_VERSION != 6U)
#error NVM_SW_MAJOR_VERSION wrong (!= 6U)
#endif

#ifndef NVM_SW_MINOR_VERSION /* configuration check */
#error NVM_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (NVM_SW_MINOR_VERSION < 17U)
#error NVM_SW_MINOR_VERSION wrong (< 17U)
#endif

#ifndef NVM_SW_PATCH_VERSION /* configuration check */
#error NVM_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (NVM_SW_PATCH_VERSION < 22U)
#error NVM_SW_PATCH_VERSION wrong (< 22U)
#endif

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>

#if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3)

/** \brief First state of the NvM_InvalidateNvBlock state machine.
 **        Only NvM_CurrentServiceId is initialized. */
STATIC FUNC(void,NVM_CODE) NvM_InvalidateNvBlock_Async(void);

/** \brief Starts a NVRAM invalidate request. Only immediate NVRAM Blocks are
 **        really erased, standard Blocks are only invalidated. */
STATIC FUNC(void,NVM_CODE) NvM_InvalidateNvBlock_Async_State1(void);

#if ( NVM_JOB_PRIORITIZATION == STD_ON )
/** \brief First asynchronous state of API service NvM_EraseNvBlock(). Only
 **        the global Service Id is set. */
STATIC FUNC(void,NVM_CODE) NvM_EraseNvBlock_Async(void);

/** \brief Starts a NVRAM erase request. Only immediate NVRAM Blocks are
 **        really erased. */
STATIC FUNC(void,NVM_CODE) NvM_EraseNvBlock_Async_State1(void);
#endif

#endif /* #if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3) */

#if ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U))

/** \brief Last state of CRC calculation */
STATIC FUNC(void,NVM_CODE) NvM_SetRamBlockStatus_Async_CalcCrc_End(void);

#endif

/** \brief NvM_ProcessCanceledJob removes canceled jobs from queue. */
STATIC FUNC(void, NVM_CODE) NvM_ProcessCanceledJob(void);

#if ( ( NVM_JOB_PRIORITIZATION == STD_ON ) && ( NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U ) )

/** \brief NvM_ProcessImmediateBlock interrupts the ongoing single block request
 **        of standard priority to process an immediate write request.
 **
 ** \retval TRUE: The processing of an immediate request has been started
 ** \retval FALSE: No immediate request was found in the queue
 **/
STATIC FUNC(boolean, NVM_CODE) NvM_ProcessImmediateBlock(void);

#endif

#if ( ( NVM_BSWM_MULTI_BLOCK_JOB_STATUS_INFO == STD_OFF ) || ( NVM_NUMBER_OF_SINGLE_BLOCK_CALLBACK > 0U ) )

/** \brief NvM_ProcessBlockCallback processes single block or multi block
 **        callbacks */
STATIC FUNC(void, NVM_CODE) NvM_ProcessBlockCallback(void);

#endif

#if ((NVM_BSWM_MULTI_BLOCK_JOB_STATUS_INFO == STD_ON) || \
     (NVM_BSWM_BLOCK_STATUS_INFO == STD_ON))
/** \brief NvM_ReportStatusToBswM processes single and/or multi block
 **        request status reporting to BswM */
STATIC FUNC(void, NVM_CODE) NvM_ReportStatusToBswM(void);
#endif

#if ((NVM_ENABLE_ASR32_SERVICE_API == STD_ON) || \
       (NVM_DEFAULT_ASR_SERVICE_API == NVM_SERVICE_API_ASR32) \
    )
/** \brief NvM_ASR40toASR32_StatusType to manipulate between ASR32 and ASR40
 **        result type */
STATIC FUNC(NvM_RequestResultType, NVM_CODE) NvM_ASR40toASR32_StatusType
(
  NvM_RequestResultType Status
);
#endif
#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>

/* !LINKSTO NVM193,1, NVM091,1, NVM157,1 */
FUNC(void,NVM_CODE) NvM_Init(void)
{
  uint16 i; /* loop counter to init NvM_AdminBlockTable */

  /* CurrentAdminBlockPtr : used to init the AdminBlockTable */
  P2VAR( NvM_AdminBlockType,AUTOMATIC,NVM_VAR ) CurrentAdminBlockPtr = NULL_PTR;

  DBG_NVM_INIT_ENTRY();
  /* !LINKSTO NVM399,1 */

  DBG_NVM_GLOBALWORKINGSTATUS((NvM_GlobalWorkingStatus),(0x0U));
  /* Clear NvM_GlobalWorkingStatus */
  NvM_GlobalWorkingStatus = 0x0U;

  /* Set the global service ID to NVM_NO_API_ID */
  NvM_CurrentServiceId = NVM_NO_API_ID;

  /* Reset the Standard Queue Index */
  NvM_Queue_IndexStandard = NVM_QUEUE_INDEX_RESET;
  /* Reset the number of cancelled standard requests */
  NvM_Queue_CancelledStandardJobs = 0U;

#if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)

#if ( ( NVM_JOB_PRIORITIZATION == STD_ON ) && ( NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U ) )
  /* Initialize the Immediate Queue Front and Count */
  NvM_Queue_IndexImmediate = 0U;
  NvM_Queue_CountImmediate = 0U;
  /* Reset the number of cancelled immediate requests */
  NvM_Queue_CancelledImmediateJobs = 0U;
#endif

#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1) */
#if (NVM_NUMBER_OF_SELECT_BLOCK_FOR_FIRSTINITALL_BLOCKS > 0)
NvM_InitFirstInitAll();
#endif
NvM_InitStoredCrcBuf();
#if (NVM_SYNC_MECHANISM == STD_ON)
NvM_MirrorRetryCount = 0U;
#endif
  /* !LINKSTO SWS_NvM_00400,1 */
  /* initialize NvM_AdminBlockTable */
  for( i = 0U; i < NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS; i++ )
  {
    CurrentAdminBlockPtr = &NvM_AdminBlockTable[i];
    CurrentAdminBlockPtr->NvMDatasetBlockIndex = 0U;
    CurrentAdminBlockPtr->NvMResult = 0U;
    CurrentAdminBlockPtr->NvMDynStatus = 0U;
    CurrentAdminBlockPtr->NvMExtendedStatus = 0U;
    /* set block-specific service ID to NVM_NO_API_ID */
    CurrentAdminBlockPtr->NvMCurrentServiceId = NVM_NO_API_ID;

    /* Check if the initial write protection flag is configured for block i
     */
    if( NVM_BD_WRITEBLOCKPROTECTION(NvM_BlockDescriptorTable[i].blockDesc) )
    {
      /* set dynamic write protection flag */
      NVM_SET_DYN_STATUS(i, NVM_DYN_WPF_MASK);
    }

  }/* end of for( i = 0U; i < NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS; i++ ) */
#if ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U))
  /* Deviation TASKING-1 */
  for(i=0U; i<NVM_NUMBER_OF_CRC_BLOCKS; i++)
  {
      NvM_CrcQueue[i] = 0;
  }
#endif
  /* Reset the temporary RAM block address */
  NvM_TempRamBlockAddress = NULL_PTR;

#if ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U))
  /*  Reset the RAM block CRC Calculation pending request count. */
  NvM_RamBlockCrcCalcCount = 0U;
#endif
  /* !LINKSTO NVM510,1 */
  /* indicate that NvM_Init() was called */
  NVM_SET_GLOBAL_GENERIC_STATUS( NVM_GENERIC_INIT_FLAG_MASK );
  DBG_NVM_GLOBALGENERICSTATUS((NvM_GlobalGenericStatus),
                    (NvM_GlobalGenericStatus | NVM_GENERIC_INIT_FLAG_MASK));
#if (NVM_BLOCK_CHECK_ENABLED == STD_ON)
  NvM_BcInit();
#endif
  DBG_NVM_INIT_EXIT();
}/* end of function  NvM_Init */

/* !LINKSTO NVM464,1 */
FUNC(void,NVM_CODE) NvM_MainFunction(void)
{
  uint8 JobEndIndicator = 0U;     /* Indicates whether a job is finished or  */
                                /* must be continued in the next cycle.    */

  boolean ExecuteMainFunction = FALSE;   /* Indicates whether main function must be continued. */

  DBG_NVM_MAINFUNCTION_ENTRY();

  /* protect concurrent access to NvM_GlobalGenericStatus */
  SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
  if ( ( NvM_GlobalGenericStatus & (uint16)( NVM_GENERIC_INIT_FLAG_MASK | NVM_GENERIC_MFP_MASK ) )
          == ( NVM_GENERIC_INIT_FLAG_MASK )
     )
  {
    /* NvM is initialized and the NvM_Mainfunction() is currently not being
     * processed --> set MFP ( Main Function Process ) Bit.
     */
    NVM_SET_GLOBAL_GENERIC_STATUS( NVM_GENERIC_MFP_MASK );
    DBG_NVM_GLOBALGENERICSTATUS((NvM_GlobalGenericStatus),
                    (NvM_GlobalGenericStatus | NVM_GENERIC_MFP_MASK));

    ExecuteMainFunction = TRUE;
  }

  SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

  /* Check whether NvM_MainFunction must be continued. */
  /* !LINKSTO NVM349,1 */
  if (ExecuteMainFunction == TRUE)
  {
#if (NVM_BLOCK_CHECK_ENABLED == STD_ON)
    /* Process BlockCheck Queue Handling*/
    if(NvM_BcGlobalDisableFlag == STD_OFF)
    {
      NvM_BcProcessQueue();
    }
#endif
    DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_NOT_OK));
    NvM_GlobalErrorStatus = NVM_REQ_NOT_OK;

    /* Remove the cancelled job from the queue. */
    NvM_ProcessCanceledJob();
    if ( NvM_GlobalCallLevel == 0U )
    {
      /* Currently no job is being processed */

      /* Check if there is any job to be processed or Ram block CRC calculation
       * request is pending */
#if ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U))
      if ((Nvm_Queue_CheckEmpty() == NVM_QUEUE_REQUEST_QUEUED)
           || (NvM_RamBlockCrcCalcCount > 0U)
         )
#else
      if (Nvm_Queue_CheckEmpty() == NVM_QUEUE_REQUEST_QUEUED)
#endif
      {
#if (NVM_BLOCK_CHECK_ENABLED == STD_ON)
        /*Cancel Any Ongoing BlockCheck*/
          NvM_BcResetCurrentProcess();
#endif
        /* Fetch the normal NvM job from queue. */
        NvM_StateMachine_SetNextRequest();

        NvM_PreService_Function();
        JobEndIndicator = 1U;
      }
#if (NVM_BLOCK_CHECK_ENABLED == STD_ON)
      else
      {
        if(NvM_BcGlobalDisableFlag == STD_OFF)
        {
          /* Start/Continue BlockCheck mechanism */
          NvM_BcMainFuncProcess();
        }
      }
#endif
    }
    else
    {

#if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)
#if ( ( NVM_JOB_PRIORITIZATION == STD_ON ) && ( NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U ) )

      /* Interrupt an ongoing single-block or multi-block request of standard
      * priority to process an immediate write request.
       */
      if (FALSE == NvM_ProcessImmediateBlock())
#endif /* #if ((NVM_JOB_PRIORITIZATION == STD_ON) &&
                (NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U)) */
#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1) */
      {
#if ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && \
     (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U))
        /* Interrupt the ongoing NvM_SetRamBlockStatus request to process
         * standard priority job (except write-all), if any request queued */
        if ((NvM_CurrentServiceId == NVM_SET_RAM_BLOCK_STATUS_API_ID) &&
            (NVM_STD_QUEUE_NOT_EMPTY()))
        {
          NvM_ProcessStandardPriorityJob();
        }
#endif
      }

       /* A job is currently being processed */
       JobEndIndicator = 1U;
    }/* end of else-if( NvM_GlobalCallLevel == 0U ) */

    /* process request */
    while( ( NvM_GlobalCallLevel > 0U ) && ( NvM_GlobalErrorStatus != NVM_REQ_PENDING ) )
    {
      NvM_CurrentFunctionPtr[NvM_GlobalCallLevel]();

    }/* while( ( NvM_GlobalCallLevel > 0U )&&( NvM_GlobalErrorStatus != NVM_REQ_PENDING ) ) */

    if ( ( JobEndIndicator == 1U ) && ( NvM_GlobalCallLevel == 0U ) )
    {
      /* current job finished */
      /* set block specific administrative data --> it must be set
       * only if the block really finished else it will not work for
       * multi block requests.
       */
      NvM_PostService_Function();

      /* RAM block CRC calculation starts when queue is empty, so it is not
       * required to remove any item from queue or process a callback. Also if
       * any block request is queued during RAM block CRC calculation so this
       * check avoids to remove that pending request from the queue.
       */
      if ( NVM_SET_RAM_BLOCK_STATUS_API_ID != NvM_CurrentServiceId )
      {
        /* remove current request from queue */
        NvM_Queue_RemoveFirstItem();

        /* Process the single block and multi block callbacks. */
#if ( ( NVM_BSWM_MULTI_BLOCK_JOB_STATUS_INFO == STD_OFF ) || \
    ( NVM_NUMBER_OF_SINGLE_BLOCK_CALLBACK > 0U ) )
        NvM_ProcessBlockCallback();
#endif
      }

#if ((NVM_BSWM_MULTI_BLOCK_JOB_STATUS_INFO == STD_ON) || \
    (NVM_BSWM_BLOCK_STATUS_INFO == STD_ON))

        /* Reporting single and/or multi-block request status to BswM */
        NvM_ReportStatusToBswM();
#endif

      /* Clear the current service ID (both global and block-specific) */
      NvM_CurrentServiceId = NVM_NO_API_ID;
      /* Clear the current service ID in the NvM_AdminBlockTable only if there
       there is no other job already PENDING. This could happen in case a new job
       is triggered from the callback. */
      SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
      if (NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMResult != NVM_REQ_PENDING)
      {
        NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMCurrentServiceId = NVM_NO_API_ID;
      }
      SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

      /* Clear out global block number */
      NvM_GlobalBlockNumber = NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS;

    }/* if ( ( JobEndIndicator == 1U ) && ( NvM_GlobalCallLevel == 0U ) ) */

    /* protect concurrent access to NvM_GlobalGenericStatus */
    SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

    /* reset MFP ( Main Function Process ) Bit. */
    NVM_CLEAR_GLOBAL_GENERIC_STATUS( NVM_GENERIC_MFP_MASK );
    DBG_NVM_GLOBALGENERICSTATUS((NvM_GlobalGenericStatus),
                  (NvM_GlobalGenericStatus & (uint16)((uint16)(~(NVM_GENERIC_MFP_MASK)) & 0xFFFFU)));

    SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

  }/* if (ExecuteMainFunction == TRUE) */


  DBG_NVM_MAINFUNCTION_EXIT();
}/*  end of function NvM_MainFunction  */

STATIC FUNC(void, NVM_CODE) NvM_ProcessCanceledJob(void)
{
  /* flag used to indicate if the single-block or multi-block callback function should be called */
  VAR(boolean, NVM_VAR) ExecuteBlockCallback = FALSE;
#if ( NVM_NUMBER_OF_SINGLE_BLOCK_CALLBACK > 0U )
  VAR(uint16, NVM_VAR) CurrentBlock;
#endif

  /* Protect concurrent access to queues */
  SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

  if (NVM_CHECK_GLOBAL_GENERIC_STATUS(NVM_GENERIC_CWA_MASK))
  {
    /* NvM WriteAll has not been started and removed from queue */
    if (NVM_QUEUE_IS_EMPTY == Nvm_Queue_CheckEmpty())
    {
      /* Terminate WriteAll immediately */
      /* Reset Cancel Write All flag */
      NVM_CLEAR_GLOBAL_GENERIC_STATUS(NVM_GENERIC_CWA_MASK);
      DBG_NVM_GLOBALGENERICSTATUS((NvM_GlobalGenericStatus),
                  (NvM_GlobalGenericStatus & (uint16)((uint16)(~(NVM_GENERIC_CWA_MASK)) & 0xFFFFU)));

      DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_CANCELLED));
      NvM_GlobalErrorStatus = NVM_REQ_CANCELLED;

      /* set the multi-block callback notification call flag */
      ExecuteBlockCallback = TRUE;
    }
  }
  else
  {
#if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)

#if ( ( NVM_JOB_PRIORITIZATION == STD_ON ) && ( NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U ) )
    /* Check if there are any cancelled requests in the immediate queue */
    if ((NVM_IMMEDIATE_QUEUE_NOT_EMPTY()) && (NvM_Queue_CancelledImmediateJobs > 0U))
    {
      /* Update the current number of queued immediate requests */
      NvM_Queue_CountImmediate = NvM_Queue_CountImmediate - NvM_Queue_CancelledImmediateJobs;

      /* Cancelled jobs were removed from the queue, reset the counter */
      NvM_Queue_CancelledImmediateJobs = 0U;

#if ( NVM_NUMBER_OF_SINGLE_BLOCK_CALLBACK > 0U )
      /* set the single-block callback notification call flag */
      ExecuteBlockCallback = TRUE;
#endif
    }
#endif

#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)  */
    /* Check if there are any cancelled requests in the standard queue */
    if ((NVM_STD_QUEUE_NOT_EMPTY()) && (NvM_Queue_CancelledStandardJobs > 0U))
    {
      /* Update the current number of queued standard requests */
      NvM_Queue_IndexStandard = NvM_Queue_IndexStandard - NvM_Queue_CancelledStandardJobs;

      /* Cancelled jobs were removed from the queue, reset the counter */
      NvM_Queue_CancelledStandardJobs = 0U;

#if ( NVM_NUMBER_OF_SINGLE_BLOCK_CALLBACK > 0U )
      /* set the single-block callback notification call flag */
      ExecuteBlockCallback = TRUE;
#endif
    }
  }
  SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

  if (ExecuteBlockCallback == TRUE)
  {
    /* Check if the single-block or multi-block callback should be called */
    if (NVM_REQ_CANCELED == NvM_AdminBlockTable[NVM_BLOCK_MULTI].NvMResult)
    {
#if (NVM_BSWM_MULTI_BLOCK_JOB_STATUS_INFO == STD_OFF)
      /* WriteAll job has been cancelled - call multi block callback function */
      NVM_MULTI_BLOCK_CALLBACK(NVM_WRITE_ALL_API_ID, NvM_GlobalErrorStatus);
#else
      /* WriteAll job has been cancelled, report the request status to BswM */
      BswM_NvM_CurrentJobMode(NVM_WRITE_ALL_API_ID, NVM_REQ_CANCELLED);
#endif
    }
#if ( NVM_NUMBER_OF_SINGLE_BLOCK_CALLBACK > 0U )
    else
    {
      /* Loop on all configured blocks and check for cancelled requests */
      for (CurrentBlock = 1U; CurrentBlock < NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS; CurrentBlock++)
      {
        if ((NVM_REQ_CANCELLED == NvM_AdminBlockTable[CurrentBlock].NvMResult) &&
            (NVM_NO_API_ID != NvM_AdminBlockTable[CurrentBlock].NvMCurrentServiceId))
        {
          /* !LINKSTO NVM176,1 */
          /* Call the user-defined block specific callback function */
          NvM_InvokeSingleBlockCallback(CurrentBlock,
                                        NvM_AdminBlockTable[CurrentBlock].NvMCurrentServiceId,
                                        NVM_REQ_CANCELLED);

          /* Clear the block-specific service ID */
          NvM_AdminBlockTable[CurrentBlock].NvMCurrentServiceId = NVM_NO_API_ID;
        }
      }
    }
#endif
  }
}/*  end of function NvM_ProcessCanceledJob  */

#if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)

#if ( ( NVM_JOB_PRIORITIZATION == STD_ON ) && ( NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U ) )

STATIC FUNC(boolean, NVM_CODE) NvM_ProcessImmediateBlock(void)
{
  boolean Result;
  MemIf_StatusType Status;
#if (NVM_CANCEL_INTERNAL_JOB == STD_ON)
  uint8 Idx;
#endif

  /* A job is being processed and should be interrupted
   * if there's an immediate write job to process.
   * According to NVM381 and NVM567, multi-block requests shall not be interrupted.
   *
   * Note: The only exception to the rule given in [NVM381, NVM567] is a write job with
   * immediate priority which shall preempt the running read / write job [NVM182].
   * The preempted job shall subsequently be resumed / restarted by the NvM module.
   */
  /* !LINKSTO NVM381,1, NVM567,1, NVM300,1 */
  /* Check if an immediate request is available */
  if  (( NVM_IMMEDIATE_QUEUE_NOT_EMPTY() ) &&
       ( /* Check if the current job is for a standard priority job */
         ( NvM_CurrentBlockDescriptorPtr->blockJobPriority != NVM_BLOCK_IMMEDIATE_PRIORITY ) ||
         ( /* Check if the current job is not a NvM_WriteBlock()/NvM_WritePRAMBlock()
            * request for an immediate block. */
           (NvM_CurrentServiceId != NVM_WRITE_BLOCK_API_ID) &&
           (NvM_CurrentServiceId  != NVM_WRITE_PRAM_BLOCK_API_ID))))
  {
    if ((NVM_READ_ALL_API_ID == NvM_CurrentServiceId) ||
        (NVM_WRITE_ALL_API_ID == NvM_CurrentServiceId) ||
        (NVM_FIRST_INIT_ALL_API_ID == NvM_CurrentServiceId) ||
        (NVM_VALIDATE_ALL_API_ID == NvM_CurrentServiceId))
    {
      if ((!NVM_CHECK_GLOBAL_GENERIC_STATUS(NVM_GENERIC_CANCEL_MASK)) &&
          (NVM_CHECK_GLOBAL_GENERIC_STATUS( NVM_GENERIC_MRKJP_MASK )))
      {
        /* Set the current block status to NVM_REQ_PENDING */
        NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMResult = NVM_REQ_PENDING;
      }
      else if (NVM_CHECK_GLOBAL_GENERIC_STATUS(NVM_GENERIC_CANCEL_MASK))
      {
        /* Set the current block status to NVM_REQ_CANCELLED */
        NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMResult = NVM_REQ_CANCELED;
      }
      else
      {
        /* Set the current block status to NVM_REQ_NOT_OK */
        NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMResult = NVM_REQ_NOT_OK;
      }

      /* Save the context of the interrupted multi-block request */
      NvM_Queue_UpdateMultiblockQueueItem();
    }

#if ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && \
     (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U))
    /* An immediate write job is queued and must suspend the
     * currently running standard job. For an ongoing NvM_SetRamBlockStatus job
     * MemIf_Cancel call not needed.
     */
    if (NvM_CurrentServiceId == NVM_SET_RAM_BLOCK_STATUS_API_ID)
    {
      /* Set block status to NVM_REQ_OK, since RAM block CRC calculation
       * is interrupted. So other single block request can be queued. */
      NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMResult = NVM_REQ_OK;
    }
    else
#endif
    {
      /* Call MemIf_GetStatus to get the status and the job shall be cancelled
       * in case the status is not MEMIF_IDLE  */
      /* !LINKSTO NvM.MemoryInterface.MemIfCancel_1,1 */
      Status = MemIf_GetStatus( NVM_BD_DEVICEID(NvM_CurrentBlockDescriptorPtr->blockDesc) );
      if ( Status != MEMIF_IDLE )
      {
        MemIf_Cancel(NVM_BD_DEVICEID(NvM_CurrentBlockDescriptorPtr->blockDesc));
      }
/* !LINKSTO NvM.MemoryInterface.MemIfCancel_2,1 */
#if (NVM_CANCEL_INTERNAL_JOB == STD_ON)
      for(Idx=0; Idx < NVM_NUM_USED_DEVICES; Idx++)
      {
        /* If vendor flag NVM_CANCEL_INTERNAL_JOB is enabled call MemIf_GetStatus to get the status
         * for all configured devices and the jobs shall be cancelled for each device in case the
         * status is not MEMIF_IDLE*/
        if(MemIf_GetStatus(NvM_UsedDeviceIds[Idx]) != MEMIF_IDLE)
        {
          MemIf_Cancel(NvM_UsedDeviceIds[Idx]);
        }
      }
#endif
    }

    /* Cancel shall make ready the underlying module to accept write of an immediate block.
     * So the immediate priority request can be processed straight away.
     */
    NvM_StateMachine_WriteImmediateBlock();

    Result = TRUE;
  }/* if ( ( NVM_IMMEDIATE_QUEUE_NOT_EMPTY() ) && .... ) */
  else
  {
    /* No immediate request was found in the queue */
    Result = FALSE;
  }

  return Result;

}/*  end of function NvM_ProcessImmediateBlock  */

#endif /* #if ( ( NVM_JOB_PRIORITIZATION == STD_ON ) &&
                ( NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U ) ) */

#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)  */

#if ( ( NVM_BSWM_MULTI_BLOCK_JOB_STATUS_INFO == STD_OFF ) || \
    ( NVM_NUMBER_OF_SINGLE_BLOCK_CALLBACK > 0U ) )
STATIC FUNC(void, NVM_CODE) NvM_ProcessBlockCallback(void)
{
#if (NVM_BSWM_MULTI_BLOCK_JOB_STATUS_INFO == STD_OFF)

  if ((( NVM_READ_ALL_API_ID == NvM_CurrentServiceId ) ||
       ( NVM_WRITE_ALL_API_ID == NvM_CurrentServiceId )) ||
       ( NVM_VALIDATE_ALL_API_ID == NvM_CurrentServiceId))
  {
    /* A multi-block request has finished.
     * Call the multi-block callback function if configured.
     */
    NVM_MULTI_BLOCK_CALLBACK( NvM_CurrentServiceId, NvM_GlobalErrorStatus );
  }
#if ( NVM_NUMBER_OF_SINGLE_BLOCK_CALLBACK > 0U )
  else
  {
    /* !LINKSTO NVM176,1 */
    /* A single-block request has finished.
     * Call the block specific callback function if configured.
     *
     * Note: Generally, call back functions should not be called
     * blocking preemption. But NvM requirement NVM156 permits this
     * provided that the runtime of the callbacks are reasonably short.
     */
    NvM_InvokeSingleBlockCallback(NvM_GlobalBlockNumber,
                                  NvM_CurrentServiceId,
                                  NvM_GlobalErrorStatus);
  }
#endif
#elif (NVM_NUMBER_OF_SINGLE_BLOCK_CALLBACK > 0U)
  if (((NVM_READ_ALL_API_ID != NvM_CurrentServiceId) &&
      (NVM_WRITE_ALL_API_ID != NvM_CurrentServiceId)) &&
      (NVM_VALIDATE_ALL_API_ID != NvM_CurrentServiceId))
  {
    /* !LINKSTO NVM176,1 */
    /* A single-block request has finished.
     * Call the block specific callback function if configured.
     *
     * Note: Generally, call back functions should not be called
     * blocking preemption. But NvM requirement NVM156 permits this
     * provided that the runtime of the callbacks are reasonably short.
     */
    NvM_InvokeSingleBlockCallback(NvM_GlobalBlockNumber,
                                  NvM_CurrentServiceId,
                                  NvM_GlobalErrorStatus);
  }
#endif
}/*  end of function NvM_ProcessBlockCallback  */
#endif

/* Check if Job status info or block status info reporting to BswM is enabled */
#if ((NVM_BSWM_MULTI_BLOCK_JOB_STATUS_INFO == STD_ON) || \
     (NVM_BSWM_BLOCK_STATUS_INFO == STD_ON))

STATIC FUNC(void, NVM_CODE) NvM_ReportStatusToBswM(void)
{
  /* Check if Job status info reporting to BswM is enabled and currently
   * finished job is multi-block request */
#if (NVM_BSWM_MULTI_BLOCK_JOB_STATUS_INFO == STD_ON)

  if (((NVM_READ_ALL_API_ID == NvM_CurrentServiceId) ||
      (NVM_WRITE_ALL_API_ID == NvM_CurrentServiceId)) ||
      (NVM_VALIDATE_ALL_API_ID == NvM_CurrentServiceId))
  {
    /* A multi-block request has finished.
     * Report the multi block request status to BswM.
     */
    BswM_NvM_CurrentJobMode(NvM_CurrentServiceId,
                            NvM_AdminBlockTable[NVM_BLOCK_MULTI].NvMResult);
  }
  else
#endif
  {
/* Check if block status info reporting to BswM is enabled */
#if (NVM_BSWM_BLOCK_STATUS_INFO == STD_ON)

    /* Check if block status reporting to BswM is enabled for this block */
    if (NVM_BD_BLOCKSTATUSINFO(NvM_CurrentBlockDescriptorPtr->blockDesc))
    {
      /* A single-block request has finished.
       * Report the single block request status to BswM.
       */
      BswM_NvM_CurrentBlockMode(NvM_BlockDescriptorTable[NvM_GlobalBlockNumber].nvBlockIdentifier,
                                NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMResult);
    }
#endif
  }
} /*  end of NvM_ReportStatusToBswM  */
#endif

FUNC(Std_ReturnType, NVM_CODE) NvM_ASR40_GetErrorStatus
(
  NvM_ASR40_BlockIdType BlockId,
  P2VAR( NvM_ASR40_RequestResultType,AUTOMATIC,NVM_APPL_DATA ) RequestResultPtr
)
{
  uint16 BlockIndex;
  Std_ReturnType Result = E_NOT_OK; /* Result of this function */

  DBG_NVM_ASR40_GETERRORSTATUS_ENTRY(BlockId,RequestResultPtr);

  if(BlockId != NVM_BLOCK_MULTI)
  {
    BlockIndex = NVM_GET_BLOCK_INDEX(BlockId);
  }
  else
  {
    BlockIndex = NVM_BLOCK_MULTI;
  }

#if ( NVM_DEV_ERROR_DETECT == STD_ON )
  /* Check if Block Id is within valid range */
  /* NOTE: NvM_GetErrorStatus() must work also for Block 0 which is
   *       used by Multi Request Services.
   */
  if( BlockIndex >= NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS )
  {
    NVM_DET_REPORT_ERROR( NVM_GET_ERROR_STATUS_API_ID, NVM_E_PARAM_BLOCK_ID );
  }
  /* Check if module is initialized by NvM_Init() */
  else if( NVM_CHECK_GLOBAL_GENERIC_STATUS_FALSE( NVM_GENERIC_INIT_FLAG_MASK ) )
  {
    NVM_DET_REPORT_ERROR( NVM_GET_ERROR_STATUS_API_ID, NVM_E_NOT_INITIALIZED );
  }
  /* Check if parameter RequestResultPtr is a NULL pointer */
  else if( RequestResultPtr == NULL_PTR )
  {
    NVM_DET_REPORT_ERROR( NVM_GET_ERROR_STATUS_API_ID, NVM_E_PARAM_DATA );
  }
  else
#endif /* NVM_DEV_ERROR_DETECT == STD_ON */
  {
#if ( defined(NVM_MULTICORE_ENABLED) )
    /* !LINKSTO NvM.MultiCore.BlockLocked,1 */
    if(((uint8)(NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus & (NVM_ES_MULTICORE_LOCK_MASK)) == (NVM_ES_MULTICORE_LOCK_MASK)))
    {
      *RequestResultPtr = NVM_REQ_PENDING;
    }
    else
#endif /*defined(NVM_MULTICORE_ENABLED)*/
    {
      *RequestResultPtr = NvM_AdminBlockTable[BlockIndex].NvMResult;
    }

    Result = E_OK;
  }


  DBG_NVM_ASR40_GETERRORSTATUS_EXIT(Result,BlockId,RequestResultPtr);
  return Result;
}/*  end of function NvM_GetErrorStatus  */

#if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)

FUNC(Std_ReturnType, NVM_CODE) NvM_ASR40_SetDataIndex
(
  NvM_ASR40_BlockIdType BlockId, uint8 DataIndex
)
{
#if (( NVM_NUMBER_OF_DATASET_BLOCKS > 0U ) || ( NVM_DEV_ERROR_DETECT == STD_ON ))
  uint16 BlockIndex = NVM_GET_BLOCK_INDEX(BlockId);
#endif
  Std_ReturnType Result = E_NOT_OK; /* Result of this function */
  DBG_NVM_ASR40_SETDATAINDEX_ENTRY(BlockId, DataIndex);
#if ( NVM_DEV_ERROR_DETECT == STD_ON )
  /* Check if module is initialized by NvM_Init() */
  if ( NVM_CHECK_GLOBAL_GENERIC_STATUS_FALSE( NVM_GENERIC_INIT_FLAG_MASK ) )
  {
    NVM_DET_REPORT_ERROR( NVM_SET_DATA_INDEX_API_ID, NVM_E_NOT_INITIALIZED );
  }
  /* Check if Block Id is within valid range */
  else if ( BlockIndex == (uint16)NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS )
  {
    NVM_DET_REPORT_ERROR( NVM_SET_DATA_INDEX_API_ID, NVM_E_PARAM_BLOCK_ID );
  }
  /* Check if the block management type is dataset */
  else if ( NVM_BD_BLOCKMNGTTYPE(NvM_BlockDescriptorTable[BlockIndex].blockDesc)
            != NVM_BD_BLOCKMNGTTYPE_DATASET )
  {
    NVM_DET_REPORT_ERROR( NVM_SET_DATA_INDEX_API_ID, NVM_E_PARAM_BLOCK_TYPE );
  }
  /* It is ensured during configuration time that NVM_DATASET_SELECTION_BITS
   * is lower or equal to 8U and that the number of NV blocks and ROM blocks
   * of a block does not exceed the maximum value allowed by NVM_DATASET_SELECTION_BITS.
   */
  else if (DataIndex >= (NvM_BlockDescriptorTable[BlockIndex].nvBlockNum +
                        NvM_BlockDescriptorTable[BlockIndex].romBlockNum))
  {
    NVM_DET_REPORT_ERROR( NVM_SET_DATA_INDEX_API_ID, NVM_E_PARAM_BLOCK_DATA_IDX );
  }
  /* Check if an other request for block BlockIndex is already pending. */
  else if ( NvM_AdminBlockTable[BlockIndex].NvMResult == NVM_REQ_PENDING )
  {
    NVM_DET_REPORT_ERROR( NVM_SET_DATA_INDEX_API_ID, NVM_E_BLOCK_PENDING );
  }
#if ( defined(NVM_MULTICORE_ENABLED) )
  else if (((uint8)(NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus & (NVM_ES_MULTICORE_LOCK_MASK)) == (NVM_ES_MULTICORE_LOCK_MASK)))
  {
     /* !LINKSTO NvM.MultiCore.SetDataIndex_MultiCoreLock_DET,1 */
    NVM_DET_REPORT_ERROR( NVM_SET_BLOCK_PROTECTION_API_ID, NVM_E_BLOCK_PENDING );
  }
#endif /*MULTICORE Enabled*/
  else
#else /* NVM_DEV_ERROR_DETECT == STD_OFF */
#if ( defined(NVM_MULTICORE_ENABLED) && ( NVM_NUMBER_OF_DATASET_BLOCKS > 0U ))
  if (((uint8)(NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus & (NVM_ES_MULTICORE_LOCK_MASK)) != (NVM_ES_MULTICORE_LOCK_MASK)))
#endif /*MULTICORE Enabled AND DATASET ENABLED*/
#endif /* NVM_DEV_ERROR_DETECT == STD_ON */
  {
/* !LINKSTO NVM150,1 */
#if ( NVM_NUMBER_OF_DATASET_BLOCKS > 0U )
    /* Check if block BlockIndex is a Dataset Block */
    if ( NVM_BD_BLOCKMNGTTYPE(NvM_BlockDescriptorTable[BlockIndex].blockDesc)
         == NVM_BD_BLOCKMNGTTYPE_DATASET )
    {
      /* NVM704 */
      if ( NvM_AdminBlockTable[BlockIndex].NvMResult != NVM_REQ_PENDING )
      {
        /* Set specified DataIndex in administration table
         *
         * Note: A common access to NvM_AdminBlockTable[BlockIndex].NvMResult and
         * NvM_AdminBlockTable[BlockIndex].NvMDatasetBlockIndex is _not_ protected
         * by a critical section because this would only make sense if this
         * function is interrupted by another API function which sets the
         * NvMResult to PENDING. But this concurrent access to the same block
         * can only be done by the application and this is not allowed
         * (NVM694). */
        NvM_AdminBlockTable[BlockIndex].NvMDatasetBlockIndex = DataIndex;
        Result = E_OK;
      }
    }
#endif
  }

#if ((NVM_NUMBER_OF_DATASET_BLOCKS == 0U) && (NVM_DEV_ERROR_DETECT == STD_OFF))
  TS_PARAM_UNUSED(DataIndex);
  TS_PARAM_UNUSED(BlockId);
#endif /* ((NVM_NUMBER_OF_DATASET_BLOCKS == 0U) && (NVM_DEV_ERROR_DETECT == STD_OFF)) */

  DBG_NVM_ASR40_SETDATAINDEX_EXIT(Result, BlockId, DataIndex);
  return Result;
}

FUNC(Std_ReturnType, NVM_CODE) NvM_ASR40_GetDataIndex
(
  NvM_ASR40_BlockIdType BlockId,
  P2VAR( uint8,AUTOMATIC,NVM_APPL_DATA ) DataIndex
)
{
#if (( NVM_NUMBER_OF_DATASET_BLOCKS > 0U ) || ( NVM_DEV_ERROR_DETECT == STD_ON ))
  uint16 BlockIndex = NVM_GET_BLOCK_INDEX(BlockId);
#endif
  Std_ReturnType Result = E_NOT_OK; /* Result of this function */
  DBG_NVM_ASR40_GETDATAINDEX_ENTRY(BlockId, DataIndex);
#if ( NVM_DEV_ERROR_DETECT == STD_ON )
  /* Check if Block Id is within valid range */
  if ( BlockIndex == (uint16)NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS )
  {
    NVM_DET_REPORT_ERROR( NVM_GET_DATA_INDEX_API_ID, NVM_E_PARAM_BLOCK_ID );
  }
  /* Check if module is initialized by NvM_Init() */
  else if ( NVM_CHECK_GLOBAL_GENERIC_STATUS_FALSE( NVM_GENERIC_INIT_FLAG_MASK ) )
  {
    NVM_DET_REPORT_ERROR( NVM_GET_DATA_INDEX_API_ID, NVM_E_NOT_INITIALIZED );
  }
  /* Check if parameter DataIndex is a NULL pointer */
  else if ( DataIndex == NULL_PTR )
  {
    NVM_DET_REPORT_ERROR( NVM_GET_DATA_INDEX_API_ID, NVM_E_PARAM_DATA );
  }
  else
#endif /* NVM_DEV_ERROR_DETECT == STD_ON */
  {
/* !LINKSTO NVM150,1 */
#if ( NVM_NUMBER_OF_DATASET_BLOCKS > 0U )
    /* Check if block BlockIndex is a Dataset Block */
    if ( NVM_BD_BLOCKMNGTTYPE(NvM_BlockDescriptorTable[BlockIndex].blockDesc)
         == NVM_BD_BLOCKMNGTTYPE_DATASET )
    {
      /* Return DataIndex of the requested block. */
      *DataIndex = NvM_AdminBlockTable[BlockIndex].NvMDatasetBlockIndex;
      Result     = E_OK;
    }
    else
#endif
    {
#if ( NVM_DEV_ERROR_DETECT == STD_ON )
      /* report a DET error as NvM_GetDataIndex() is never intended to be
       * called for non-dataset blocks. */
      NVM_DET_REPORT_ERROR( NVM_GET_DATA_INDEX_API_ID, NVM_E_PARAM_BLOCK_TYPE );
#endif

      /* For blocks with block management different from NVM_BLOCK_DATASET,
       * set the index to zero. This applies also to the case if DET is switch
       * off. NVM265. */
      *DataIndex = 0U;

      /* leave Result set to E_NOT_OK here */
    }

  }

#if ( ( NVM_NUMBER_OF_DATASET_BLOCKS == 0U ) && ( NVM_DEV_ERROR_DETECT == STD_OFF ) )
  TS_PARAM_UNUSED(BlockId);
#endif
  DBG_NVM_ASR40_GETDATAINDEX_EXIT(Result, BlockId, DataIndex);
  return Result;
}/* end of function NvM_GetDataIndex */

#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1) */

#if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3)
/* !LINKSTO NvM.EraseBlock.Availability,2 */
FUNC(Std_ReturnType,NVM_CODE) NvM_ASR40_EraseNvBlock( NvM_ASR40_BlockIdType BlockId )
{
#if (( NVM_JOB_PRIORITIZATION == STD_ON ) || (!defined(NVM_ERASENVBLOCK_CALLOUT)))
  uint16 BlockIndex;
#endif
  Std_ReturnType Result = E_NOT_OK; /* Result of this function */

#if ( NVM_JOB_PRIORITIZATION == STD_ON )
#if (NVM_PROD_ERR_HANDLING_WRITE_PROTECTED != TS_PROD_ERR_DISABLE)
  boolean ReportWriteProtected = FALSE; /* To check write protected DEM/DET error reporting */
#endif
#if (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW != TS_PROD_ERR_DISABLE)
  boolean ReportQueueOverflow = FALSE; /* To check queue overflow DEM/DET error reporting */
#endif
#if ( NVM_DEV_ERROR_DETECT == STD_ON )
  boolean ReportWriteOnceUnknown = FALSE; /* To check whether Det/Dem error shall be reported */
#endif
#endif /* NVM_JOB_PRIORITIZATION == STD_ON */

  DBG_NVM_ASR40_ERASENVBLOCK_ENTRY(BlockId);

#ifndef NVM_ERASENVBLOCK_CALLOUT
  BlockIndex = NvM_CheckCondEraseNvBlock(BlockId);
  if(BlockIndex != NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS)
#endif
  {
#if ( NVM_JOB_PRIORITIZATION == STD_ON )
#ifdef NVM_ERASENVBLOCK_CALLOUT
    /* If multi-core wrapper is active the parameter BlockId is actually the block index */
    BlockIndex = (uint16)BlockId;
#endif
    /* protect concurrent access to the standard queue and
     * concurrent access to write protection flag and data set index
     */
    SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

    /* Check also dynamic write protection flag which can be set by
     * NvM_SetBlockProtection(). If block is write protected report production error.
     * This check must be done always and independently of the Det checks.
     * Also the check if a Dataset ROM Block
     * is specified must be done always.  */
    if (NvM_CheckWriteProtection(BlockIndex) == TRUE)
    {
#if (NVM_PROD_ERR_HANDLING_WRITE_PROTECTED != TS_PROD_ERR_DISABLE)
      /* Write Protected DEM/DET error needs to be reported */
      ReportWriteProtected = TRUE;
#endif
    }
#if (NVM_WRITE_BLOCK_ONCE == STD_ON)
    /* !LINKSTO SWS_NvM_00952,1 */
    /*Check if block is WRITEONCE block but have not been read before*/
    else if((NVM_BD_WRITEONCE(NvM_BlockDescriptorTable[BlockIndex].blockDesc))&&(!NVM_CHECK_DYN_STATUS(BlockIndex, NVM_DYN_WBORF_MASK)))
    {
#if ( NVM_DEV_ERROR_DETECT == STD_ON )
        /* !LINKSTO SWS_NvM_00957,1 */
        ReportWriteOnceUnknown = TRUE;
#endif
    }
#endif
    else if ((NvM_CheckDataSetRomBlock(BlockIndex) == FALSE) &&
             /* Locked NV blocks shall not be invalidated */
             (!NVM_CHECK_DYN_STATUS(BlockIndex, NVM_DYN_LOCK_MASK))
            )
    {
      if (NvM_AdminBlockTable[BlockIndex].NvMResult != NVM_REQ_PENDING)
      {
        Result = NvM_Queue_InsertStandard(BlockIndex, NULL_PTR, &NvM_EraseNvBlock_Async);

#if (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW != TS_PROD_ERR_DISABLE)
        if (E_OK != Result)
        {
         /* Queue overflow DEM/DET error needs to be reported */
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
      NvM_AdminBlockTable[BlockIndex].NvMCurrentServiceId = NVM_ERASE_NV_BLOCK_API_ID;
    }

    SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

#if (NVM_PROD_ERR_HANDLING_WRITE_PROTECTED != TS_PROD_ERR_DISABLE)
    if (ReportWriteProtected == TRUE)
    {
#if (NVM_PROD_ERR_HANDLING_WRITE_PROTECTED == TS_PROD_ERR_REP_TO_DEM )
#if (NVM_WRITE_PROT_DEM_EVENT_ID != NVM_INVALID_DEM_EVENTID )
      NVM_DEM_REPORT_ERROR(NVM_WRITE_PROT_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#endif
#elif (NVM_PROD_ERR_HANDLING_WRITE_PROTECTED == TS_PROD_ERR_REP_TO_DET)
      NVM_DET_REPORT_ERROR(NVM_ERASE_NV_BLOCK_API_ID, NVM_E_DEMTODET_WRITE_PROTECTED);
#elif (NVM_PROD_ERR_HANDLING_WRITE_PROTECTED == TS_PROD_ERR_REP_TO_USER)
        NVM_PRODUCTION_ERRORS_FUNCTION_CALLOUT(BlockId, NVM_ERASE_NV_BLOCK_API_ID, NVM_E_USER_CALLOUT_WRITE_PROTECTED);
#endif
    }
#endif /* NVM_PROD_ERR_HANDLING_WRITE_PROTECTED != TS_PROD_ERR_DISABLE */

#if (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW != TS_PROD_ERR_DISABLE)
    if (ReportQueueOverflow == TRUE)
    {
#if (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW == TS_PROD_ERR_REP_TO_DEM)
#if (NVM_QUEUE_OVERFLOW_DEM_EVENT_ID != NVM_INVALID_DEM_EVENTID )
       NVM_DEM_REPORT_ERROR(NVM_QUEUE_OVERFLOW_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#endif
#elif (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW == TS_PROD_ERR_REP_TO_DET)
          NVM_DET_REPORT_ERROR(NVM_ERASE_NV_BLOCK_API_ID, NVM_E_DEMTODET_QUEUE_OVERFLOW);
#elif (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW == TS_PROD_ERR_REP_TO_USER)
        NVM_PRODUCTION_ERRORS_FUNCTION_CALLOUT(BlockId, NVM_ERASE_NV_BLOCK_API_ID, NVM_E_USER_CALLOUT_QUEUE_OVERFLOW);
#endif
    }
#endif /* NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW != TS_PROD_ERR_DISABLE */

#if ( NVM_DEV_ERROR_DETECT == STD_ON )
    if (ReportWriteOnceUnknown == TRUE)
    {
      NVM_DET_REPORT_ERROR( NVM_ERASE_NV_BLOCK_API_ID, NVM_E_WRITE_ONCE_STATUS_UNKNOWN );
    }
#endif

#if (NVM_BSWM_BLOCK_STATUS_INFO == STD_ON)
     /* Check if block status changed and block status reporting to BswM is
      * enabled for this block */
    if ((E_OK == Result) &&
        (NVM_BD_BLOCKSTATUSINFO(NvM_BlockDescriptorTable[BlockIndex].blockDesc)))
    {
      /* Block status changed due to queuing erase request, report the
       * block status to BswM */
      BswM_NvM_CurrentBlockMode(BlockId, NVM_REQ_PENDING);
    }
#endif

#if ( defined(NVM_MULTICORE_ENABLED) )
    NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus &= (uint8)(~(NVM_ES_MULTICORE_LOCK_MASK));
#endif

#else /* #if ( NVM_JOB_PRIORITIZATION == STD_ON ) */
#ifdef NVM_ERASENVBLOCK_CALLOUT
    TS_PARAM_UNUSED(BlockId);
#endif

#endif
  }
  DBG_NVM_ASR40_ERASENVBLOCK_EXIT(Result,BlockId);

  return Result;
}/* end of function NvM_EraseNvBlock */

FUNC(Std_ReturnType,NVM_CODE) NvM_ASR40_InvalidateNvBlock
(
  NvM_ASR40_BlockIdType BlockId
)
{
  uint16 BlockIndex;
  Std_ReturnType Result = E_NOT_OK; /* Result of this function */
#if (NVM_PROD_ERR_HANDLING_WRITE_PROTECTED != TS_PROD_ERR_DISABLE)
  boolean ReportWriteProtected = FALSE; /* To check write protected DEM/DET error reporting */
#endif
#if (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW != TS_PROD_ERR_DISABLE)
  boolean ReportQueueOverflow = FALSE;/* To check queue overflow DEM/DET error reporting */
#endif
#if ( NVM_DEV_ERROR_DETECT == STD_ON )
  boolean ReportWriteOnceUnknown = FALSE; /* To check whether Det/Dem error shall be reported */
#endif
  DBG_NVM_ASR40_INVALIDATENVBLOCK_ENTRY(BlockId);

#ifndef NVM_INVALIDATENVBLOCK_CALLOUT
  BlockIndex = NvM_CheckCondInvalidateNvBlock(BlockId);
  if(BlockIndex != NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS)
#else
  /* If multi-core wrapper is active the parameter BlockId is actually the block index */
  BlockIndex = (uint16)BlockId;
#endif
  {
    /* protect concurrent access to the standard queue and
    * concurrent access to write protection flag and data set index
    */
    SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

    /* Check also dynamic write protection flag which can be set by
     * NvM_SetBlockProtection(). If block is write protected report production error.
     * This check must be done always and independently of the Det checks.
     * Also the check if a Dataset ROM Block
     * is specified must be done always. */
    if (NvM_CheckWriteProtection(BlockIndex) == TRUE)
    {
#if (NVM_PROD_ERR_HANDLING_WRITE_PROTECTED != TS_PROD_ERR_DISABLE)
       /* Write Protected DEM/DET error needs to be reported */
       ReportWriteProtected = TRUE;
#endif
    }
#if (NVM_WRITE_BLOCK_ONCE == STD_ON)
    /* !LINKSTO SWS_NvM_00952,1 */
    /*Check if block is WRITEONCE block but have not been read before*/
    else if((NVM_BD_WRITEONCE(NvM_BlockDescriptorTable[BlockIndex].blockDesc))&&(!NVM_CHECK_DYN_STATUS(BlockIndex, NVM_DYN_WBORF_MASK)))
    {
#if ( NVM_DEV_ERROR_DETECT == STD_ON )
        /* !LINKSTO SWS_NvM_00958,1 */
        ReportWriteOnceUnknown = TRUE;
#endif
    }
#endif
    else if ((NvM_CheckDataSetRomBlock(BlockIndex) == FALSE) &&
             /* Locked NV blocks shall not be erased */
             (!NVM_CHECK_DYN_STATUS(BlockIndex, NVM_DYN_LOCK_MASK))
            )
    {
      if (NvM_AdminBlockTable[BlockIndex].NvMResult != NVM_REQ_PENDING)
      {
        /* The Request is accepted */
        Result = NvM_Queue_InsertStandard(BlockIndex, NULL_PTR, &NvM_InvalidateNvBlock_Async);

#if (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW != TS_PROD_ERR_DISABLE)
        if (E_OK != Result)
        {
          /* Queue overflow DEM/DET error needs to be reported */
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
      NvM_AdminBlockTable[BlockIndex].NvMCurrentServiceId = NVM_INVALIDATE_NV_BLOCK_API_ID;
    }

    SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

#if (NVM_PROD_ERR_HANDLING_WRITE_PROTECTED != TS_PROD_ERR_DISABLE)
    if (ReportWriteProtected == TRUE)
    {
#if (NVM_PROD_ERR_HANDLING_WRITE_PROTECTED == TS_PROD_ERR_REP_TO_DEM )
#if (NVM_WRITE_PROT_DEM_EVENT_ID != NVM_INVALID_DEM_EVENTID )
      NVM_DEM_REPORT_ERROR(NVM_WRITE_PROT_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#endif
#elif (NVM_PROD_ERR_HANDLING_WRITE_PROTECTED == TS_PROD_ERR_REP_TO_DET)
      NVM_DET_REPORT_ERROR(NVM_INVALIDATE_NV_BLOCK_API_ID, NVM_E_DEMTODET_WRITE_PROTECTED);
#elif (NVM_PROD_ERR_HANDLING_WRITE_PROTECTED == TS_PROD_ERR_REP_TO_USER)
        NVM_PRODUCTION_ERRORS_FUNCTION_CALLOUT(BlockId, NVM_INVALIDATE_NV_BLOCK_API_ID, NVM_E_USER_CALLOUT_WRITE_PROTECTED);
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
          NVM_DET_REPORT_ERROR(NVM_INVALIDATE_NV_BLOCK_API_ID, NVM_E_DEMTODET_QUEUE_OVERFLOW);
#elif (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW == TS_PROD_ERR_REP_TO_USER)
        NVM_PRODUCTION_ERRORS_FUNCTION_CALLOUT(BlockId, NVM_INVALIDATE_NV_BLOCK_API_ID, NVM_E_USER_CALLOUT_QUEUE_OVERFLOW);
#endif /* #if (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW == TS_PROD_ERR_REP_TO_DEM)  */
    }
#endif

#if ( NVM_DEV_ERROR_DETECT == STD_ON )
    if (ReportWriteOnceUnknown == TRUE)
    {
      NVM_DET_REPORT_ERROR( NVM_INVALIDATE_NV_BLOCK_API_ID, NVM_E_WRITE_ONCE_STATUS_UNKNOWN );
    }
#endif

#if (NVM_BSWM_BLOCK_STATUS_INFO == STD_ON)
    /* Check if block status changed and block status reporting to BswM is
     * enabled for this block */
    if ((E_OK == Result) &&
        (NVM_BD_BLOCKSTATUSINFO(NvM_BlockDescriptorTable[BlockIndex].blockDesc)))
    {
      /* Block status changed due to queuing invalidate request, report the
       * block status to BswM */
      BswM_NvM_CurrentBlockMode(BlockId, NVM_REQ_PENDING);
    }
#endif
#if ( defined(NVM_MULTICORE_ENABLED) )
    NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus &= (uint8)(~(NVM_ES_MULTICORE_LOCK_MASK));
#endif
  }

  DBG_NVM_ASR40_INVALIDATENVBLOCK_EXIT(Result,BlockId);
  return Result;
}/* end of function NvM_InvalidateNvBlock */

FUNC(Std_ReturnType, NVM_CODE) NvM_ASR40_SetBlockProtection
(
  NvM_ASR40_BlockIdType BlockId,
  boolean ProtectionEnabled
)
{
  uint16 BlockIndex = NVM_GET_BLOCK_INDEX(BlockId);
  Std_ReturnType Result = E_NOT_OK; /* Result of this function */

  DBG_NVM_ASR40_SETBLOCKPROTECTION_ENTRY(BlockId,ProtectionEnabled);

#if ( NVM_DEV_ERROR_DETECT == STD_ON )
  /* Check if Block Id is within valid range */
  if ( BlockIndex == (uint16)NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS )
  {
    NVM_DET_REPORT_ERROR( NVM_SET_BLOCK_PROTECTION_API_ID, NVM_E_PARAM_BLOCK_ID );
  }
  /* Check if module is initialized by NvM_Init() */
  else if ( NVM_CHECK_GLOBAL_GENERIC_STATUS_FALSE( NVM_GENERIC_INIT_FLAG_MASK ) )
  {
    NVM_DET_REPORT_ERROR( NVM_SET_BLOCK_PROTECTION_API_ID, NVM_E_NOT_INITIALIZED );
  }
#if (NVM_WRITE_BLOCK_ONCE == STD_ON)
  /* Check if an other request for block BlockIndex is already pending. */
  else if ( NvM_AdminBlockTable[BlockIndex].NvMResult == NVM_REQ_PENDING )
  {
    NVM_DET_REPORT_ERROR( NVM_SET_BLOCK_PROTECTION_API_ID, NVM_E_BLOCK_PENDING );
  }
  /* Check if write block once is enabled for this block */
  else if ( NVM_BD_WRITEONCE(NvM_BlockDescriptorTable[BlockIndex].blockDesc))
  {
    NVM_DET_REPORT_ERROR( NVM_SET_BLOCK_PROTECTION_API_ID, NVM_E_BLOCK_CONFIG );
  }
#endif
  /* !LINKSTO SWS_NvM_00759,1 */
  else if (NVM_CHECK_DYN_STATUS(BlockIndex, NVM_DYN_LOCK_MASK))
  {
    NVM_DET_REPORT_ERROR(NVM_SET_BLOCK_PROTECTION_API_ID, NVM_E_BLOCK_LOCKED);
  }
#if ( defined(NVM_MULTICORE_ENABLED) )
  else if (((uint8)(NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus & (NVM_ES_MULTICORE_LOCK_MASK)) == (NVM_ES_MULTICORE_LOCK_MASK)))
  {
     /* !LINKSTO NvM.MultiCore.SetBlockProtection_MultiCoreLock_DET,1 */
    NVM_DET_REPORT_ERROR( NVM_SET_BLOCK_PROTECTION_API_ID, NVM_E_BLOCK_PENDING );
  }
#endif /*MULTICORE Enabled*/
  else
#else /* NVM_DEV_ERROR_DETECT == STD_OFF */
#if ( defined(NVM_MULTICORE_ENABLED) )
  if (((uint8)(NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus & (NVM_ES_MULTICORE_LOCK_MASK)) != (NVM_ES_MULTICORE_LOCK_MASK)))
#endif /*MULTICORE Enabled*/
#endif /* NVM_DEV_ERROR_DETECT == STD_ON */
  {
    /* Administrative block modification is not allowed since already
       a block request is pending (NVM704). */
    if ( NvM_AdminBlockTable[BlockIndex].NvMResult != NVM_REQ_PENDING )
    {
#if (NVM_WRITE_BLOCK_ONCE == STD_ON)
      /* Enabling/Disabling of the write protection is not allowed when
         the NvMWriteBlockOnce is TRUE (NVM577, NVM398). */
      if (!NVM_BD_WRITEONCE(NvM_BlockDescriptorTable[BlockIndex].blockDesc))
#endif
      {
        /* protect concurrent access to NvM_AdminBlockTable[BlockIndex] */
        SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

        if ( ProtectionEnabled == TRUE )
        {
          /* set write protection flag */
          NVM_SET_DYN_STATUS(BlockIndex, NVM_DYN_WPF_MASK);
        }
        else
        {
          /* reset write protection flag */
          NVM_CLEAR_DYN_STATUS(BlockIndex, NVM_DYN_WPF_MASK);
        }
        SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

        Result = E_OK;
      }
    }
  }
  DBG_NVM_ASR40_SETBLOCKPROTECTION_EXIT(Result,BlockId,ProtectionEnabled);
  return Result;

}/* end of function NvM_SetBlockProtection */

#endif /* #if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3) */

#if (NVM_SET_RAM_BLOCK_STATUS_API == STD_ON)
FUNC(Std_ReturnType, NVM_CODE) NvM_ASR40_SetRamBlockStatus
(
  NvM_ASR40_BlockIdType BlockId,
  boolean         BlockChanged
)
{
  /* pointer to block descriptor of the requested block.  */
  P2CONST( NvM_BlockDescriptorType,AUTOMATIC,NVM_CONST )
    NvM_CurrentBlockDescriptorLocalPtr = NULL_PTR;
  uint16 BlockIndex = NVM_GET_BLOCK_INDEX(BlockId);
  Std_ReturnType Result = E_NOT_OK; /* Result of this function */

  DBG_NVM_ASR40_SETRAMBLOCKSTATUS_ENTRY(BlockId,BlockChanged);
#if ( NVM_DEV_ERROR_DETECT == STD_ON )
  /* Check if Block Id is within valid range */
  if ( BlockIndex == (uint16)NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS )
  {
    NVM_DET_REPORT_ERROR( NVM_SET_RAM_BLOCK_STATUS_API_ID, NVM_E_PARAM_BLOCK_ID );
  }
  /* Check if module is initialized by NvM_Init() */
  else if ( NVM_CHECK_GLOBAL_GENERIC_STATUS_FALSE( NVM_GENERIC_INIT_FLAG_MASK ) )
  {
    NVM_DET_REPORT_ERROR( NVM_SET_RAM_BLOCK_STATUS_API_ID, NVM_E_NOT_INITIALIZED );
  }
  /* Check if an other request for block BlockIndex is already pending. */
  else if ( NvM_AdminBlockTable[BlockIndex].NvMResult == NVM_REQ_PENDING )
  {
    NVM_DET_REPORT_ERROR( NVM_SET_RAM_BLOCK_STATUS_API_ID, NVM_E_BLOCK_PENDING );
  }
    /* !LINKSTO SWS_NvM_00758,1 */
  else if (NVM_CHECK_DYN_STATUS(BlockIndex, NVM_DYN_LOCK_MASK))
  {
    NVM_DET_REPORT_ERROR(NVM_SET_RAM_BLOCK_STATUS_API_ID, NVM_E_BLOCK_LOCKED);
  }
#if ( defined(NVM_MULTICORE_ENABLED) )
  else if (((uint8)(NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus & (NVM_ES_MULTICORE_LOCK_MASK)) == (NVM_ES_MULTICORE_LOCK_MASK)))
  {
     /* !LINKSTO NvM.MultiCore.SetRamBlockStatus_MultiCoreLock_DET,1 */
    NVM_DET_REPORT_ERROR( NVM_SET_RAM_BLOCK_STATUS_API_ID, NVM_E_BLOCK_PENDING );
  }
#endif /*MULTICORE Enabled*/
  else
#else /* NVM_DEV_ERROR_DETECT == STD_OFF */
#if ( defined(NVM_MULTICORE_ENABLED) )
  if (((uint8)(NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus & (NVM_ES_MULTICORE_LOCK_MASK)) != (NVM_ES_MULTICORE_LOCK_MASK)))
#endif /*MULTICORE Enabled*/
#endif /* NVM_DEV_ERROR_DETECT == STD_ON */
  {
    /* NVM704 */
    if ( NvM_AdminBlockTable[BlockIndex].NvMResult != NVM_REQ_PENDING )
    {
      /* NOTE: This pointer can be set only if the parameter BlockIndex was
       * checked. */
      NvM_CurrentBlockDescriptorLocalPtr = &NvM_BlockDescriptorTable[BlockIndex];

      /* SetRamBlockStatus() can be used only for permanent RAM blocks with
       * NvMBlockUseSetRamBlockStatus enabled,  or if explicit synchronization is
       * enabled: Check if block BlockIndex uses a permanent RAM block and config
       *flag enabled or explicit synchronization mechanism.
       *
       * NOTE: Normally this check could be added to the Det checks because the
       * permanent RAM Block is a static configuration parameter but it is not
       * required in the specification. */
      /* !LINKSTO NvM.BlockSetRamBlockStatus,1 */
#if (NVM_SYNC_MECHANISM == STD_ON)
      if ((NVM_BD_USERAMBLOCKSTATUS(NvM_CurrentBlockDescriptorLocalPtr->blockDesc)) &&
          ( (NvM_CurrentBlockDescriptorLocalPtr->ramBlockDataAddress != NULL_PTR) ||
             NVM_BD_USEMIRROR(NvM_CurrentBlockDescriptorLocalPtr->blockDesc)))
#else
      if ( (NvM_CurrentBlockDescriptorLocalPtr->ramBlockDataAddress != NULL_PTR) &&
           (NVM_BD_USERAMBLOCKSTATUS(NvM_CurrentBlockDescriptorLocalPtr->blockDesc)) )
#endif /* NVM_SYNC_MECHANISM = STD_ON */
      {
        /* protect concurrent access to NvM_AdminBlockTable[BlockIndex] */
        SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

        if ( BlockChanged == TRUE )
        {
          /* !LINKSTO SWS_NvM_00406,1 */
          /* Block data has changed --> validate block. NVM406. */
          NvM_AdminBlockTable[BlockIndex].NvMValid = NVM_BLOCK_VALID;

          /* set RAM block to changed and indicate that it was changed by
           * NvM_SetRamBlockStatus() */
          NVM_SET_DYN_STATUS(BlockIndex, (NVM_DYN_CHANGE_MASK | NVM_DYN_SRBSC_MASK));

#if (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U)
          /* Check if RAM CRC calculation is enabled for this block and no other
           * RAM CRC calculation is pending for the same block */
          if (NVM_BD_CALCRAMBLOCKCRC(NvM_CurrentBlockDescriptorLocalPtr->blockDesc)
              && ((NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus &
                   NVM_ES_RAM_BLOCK_CRC_CALC_MASK) != NVM_ES_RAM_BLOCK_CRC_CALC_MASK))
          {
            /* RAM CRC calculation is enabled for this block. Start CRC calculation
             * over the RAM block. */

            /* Set the current block's result to NVM_REQ_OK so that normal
             * request for the same block shall be accepted */
            NvM_AdminBlockTable[BlockIndex].NvMResult = NVM_REQ_OK;

            /* A Ram block CRC calculation request is pending */
            NvM_CalcCrc_EnqueueElement(BlockIndex);
          }
#endif /* (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U) */

        } /* if( BlockChanged == TRUE ) */

        /* else: CRC calculation is not enabled and not pending, no further operations */
        else
        {
          /* set RAM block to unchanged and invalid */
          NVM_CLEAR_DYN_STATUS(BlockIndex, NVM_DYN_CHANGE_MASK);
          NvM_AdminBlockTable[BlockIndex].NvMValid = NVM_BLOCK_INVALID;
        } /* else-if( BlockChanged == TRUE ) */

        SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

        Result = E_OK;
      }
    }
  }

  DBG_NVM_ASR40_SETRAMBLOCKSTATUS_EXIT(Result,BlockId,BlockChanged);
  return Result;
}/* end of function NvM_SetRamBlockStatus */

#endif  /* (NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) */

FUNC(void, NVM_CODE) NvM_SetBlockLockStatus
(
  NvM_ASR40_BlockIdType BlockId,
  boolean         BlockLocked
)
{
  uint16 BlockIndex = NVM_GET_BLOCK_INDEX(BlockId);
  DBG_NVM_SETBLOCKLOCKSTATUS_ENTRY(BlockId,BlockLocked);
#if (NVM_DEV_ERROR_DETECT == STD_ON)
  /* Check if module is initialized by NvM_Init() */

  if (NVM_CHECK_GLOBAL_GENERIC_STATUS_FALSE(NVM_GENERIC_INIT_FLAG_MASK))
  {
    NVM_DET_REPORT_ERROR(NVM_SET_BLOCK_LOCK_STATUS_API_ID, NVM_E_NOT_INITIALIZED);
  }
  /* Check if Block Id is within valid range */
  else if ( BlockIndex == (uint16)NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS )
  {
    NVM_DET_REPORT_ERROR(NVM_SET_BLOCK_LOCK_STATUS_API_ID, NVM_E_PARAM_BLOCK_ID);
  }
  /* Check if another request for the same block is already pending. */
  else if (NvM_AdminBlockTable[BlockIndex].NvMResult == NVM_REQ_PENDING)
  {
    NVM_DET_REPORT_ERROR(NVM_SET_BLOCK_LOCK_STATUS_API_ID, NVM_E_BLOCK_PENDING);
  }
#if (NVM_WRITE_BLOCK_ONCE == STD_ON)
  /* Check if the block is configured with NvMWriteBlockOnce = TRUE. */
  else if (NVM_BD_WRITEONCE(NvM_BlockDescriptorTable[BlockIndex].blockDesc))
  {
    NVM_DET_REPORT_ERROR(NVM_SET_BLOCK_LOCK_STATUS_API_ID, NVM_E_BLOCK_CONFIG);
  }
#endif
#if ( defined(NVM_MULTICORE_ENABLED) )
  else if (((uint8)(NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus & (NVM_ES_MULTICORE_LOCK_MASK)) == (NVM_ES_MULTICORE_LOCK_MASK)))
  {
     /* !LINKSTO NvM.MultiCore.SetBlockLockStatus_MultiCoreLock_DET,1 */
    NVM_DET_REPORT_ERROR( NVM_SET_BLOCK_LOCK_STATUS_API_ID, NVM_E_BLOCK_PENDING );
  }
#endif /*MULTICORE Enabled*/
  else
#else /* NVM_DEV_ERROR_DETECT == STD_OFF */
#if ( defined(NVM_MULTICORE_ENABLED) )
  if (((uint8)(NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus & (NVM_ES_MULTICORE_LOCK_MASK)) != (NVM_ES_MULTICORE_LOCK_MASK)))
#endif /*MULTICORE Enabled*/
#endif /* NVM_DEV_ERROR_DETECT == STD_ON */
  {
    /* NvM_SetBlockLockStatus() shall only work on NVRAM blocks with a
     * permanently configured RAM block or explicit synchronization enabled.
     */
    /* !LINKSTO SWS_NvM_00732,1 */
#if (NVM_SYNC_MECHANISM == STD_ON)
    if ( ( NvM_BlockDescriptorTable[BlockIndex].ramBlockDataAddress != NULL_PTR ) ||
         ( NVM_BD_USEMIRROR(NvM_BlockDescriptorTable[BlockIndex].blockDesc) )
       )
#else
    if ( NvM_BlockDescriptorTable[BlockIndex].ramBlockDataAddress != NULL_PTR )
#endif /* NVM_SYNC_MECHANISM == STD_ON */
    {
      /* Protect concurrent access to NvM_AdminBlockTable[BlockIndex] */
      SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

      if (BlockLocked == TRUE)
      {
        /* Set the 'Block Locked' flag
         * so that the NV block is not changed by NvM_WriteBlock,
         * NvM_InvalidateNvBlock or NvM_EraseNvBlock. (NVM751)
         */
        NVM_SET_DYN_STATUS(BlockIndex, NVM_DYN_LOCK_MASK);
      }
      else
      {
        /* Reset the 'Block Locked' flag */
        NVM_CLEAR_DYN_STATUS(BlockIndex, NVM_DYN_LOCK_MASK);
      }

      SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
    }
  }

  DBG_NVM_SETBLOCKLOCKSTATUS_EXIT(BlockId,BlockLocked);
}

#if ( NVM_VERSION_INFO_API == STD_ON )
FUNC(void, NVM_CODE) NvM_GetVersionInfo
(
  P2VAR( Std_VersionInfoType, AUTOMATIC, NVM_APPL_DATA ) versionInfoPtr
)
{
  DBG_NVM_GETVERSIONINFO_ENTRY(versionInfoPtr);
#if ( NVM_DEV_ERROR_DETECT == STD_ON )

  if (NULL_PTR == versionInfoPtr)
  {
    NVM_DET_REPORT_ERROR(NVM_GET_VERSION_INFO_API_ID, NVM_E_PARAM_POINTER);
  }
  else
#endif
  {
    versionInfoPtr->vendorID         = NVM_VENDOR_ID;
    versionInfoPtr->moduleID         = NVM_MODULE_ID;
    versionInfoPtr->sw_major_version = NVM_SW_MAJOR_VERSION;
    versionInfoPtr->sw_minor_version = NVM_SW_MINOR_VERSION;
    versionInfoPtr->sw_patch_version = NVM_SW_PATCH_VERSION;
  }

  DBG_NVM_GETVERSIONINFO_EXIT(versionInfoPtr);
}
#endif  /* NVM_VERSION_INFO_API == STD_ON */

#if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)

FUNC(Std_ReturnType, NVM_CODE) NvM_ASR40_CancelJobs(NvM_BlockIdType BlockId)
{
  uint16 BlockIndex;
  Std_ReturnType Result = E_NOT_OK; /* Result of this function */

  /* Index used to search the standard/immediate queues for cancelled requests */
  VAR(uint16, NVM_VAR) QueueIndex;
  /* Index used to shift items above/after a cancelled request from the standard/immediate queue */
  VAR(uint16, NVM_VAR) QueueCopyIndex;
  /* Variable used to temporary store the details of the cancelled request */
  VAR(NvM_QueueItemType, NVM_VAR) QueueTemp;
  /* Variable that points to the top of the standard queue (excluding cancelled requests) */
  VAR(uint16, NVM_VAR) Queue_Standard_TopIndex;
#if ((NVM_JOB_PRIORITIZATION == STD_ON) && (NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U))
  VAR(uint16, NVM_VAR) Queue_Immediate_TopIndex;
#endif
  VAR(boolean, NVM_VAR) RequestFound = FALSE;

  DBG_NVM_ASR40_CANCELJOBS_ENTRY(BlockId);

#ifndef NVM_CANCELJOBS_CALLOUT
  BlockIndex = NvM_CheckCondCancelJobs(BlockId);
  if(BlockIndex != NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS)
#else
  /* If multi-core wrapper is active the parameter BlockId is actually the block index */
  BlockIndex = (uint16)BlockId;
#endif
  {
    /* Protect concurrent access to NvM_BlockAdminTable[BlockIndex].NvMResult */
    SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

    Queue_Standard_TopIndex = NvM_Queue_IndexStandard - NvM_Queue_CancelledStandardJobs;
#if ((NVM_JOB_PRIORITIZATION == STD_ON) && (NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U))
    Queue_Immediate_TopIndex = (NvM_Queue_IndexImmediate + NvM_Queue_CountImmediate) - NvM_Queue_CancelledImmediateJobs;
#endif
    /* Check if a block request is queued and is not currently in progress */
    if ((NvM_AdminBlockTable[BlockIndex].NvMResult == NVM_REQ_PENDING) &&
         (NvM_GlobalBlockNumber != BlockIndex)
       )
    {
      /* Set state of block request to cancelled */
      NvM_AdminBlockTable[BlockIndex].NvMResult = NVM_REQ_CANCELED;

#if ((NVM_JOB_PRIORITIZATION == STD_ON) && (NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U))
      /* Check if the queued request is an immediate write request */
      if(
          (NvM_BlockDescriptorTable[BlockIndex].blockJobPriority == NVM_BLOCK_IMMEDIATE_PRIORITY) &&
          (
            (NvM_AdminBlockTable[BlockIndex].NvMCurrentServiceId == NVM_WRITE_BLOCK_API_ID) ||
            (NvM_AdminBlockTable[BlockIndex].NvMCurrentServiceId == NVM_WRITE_PRAM_BLOCK_API_ID)
          )
        )
      {
        /* Start searching the queue from the current request */
        QueueIndex = NvM_Queue_IndexImmediate;
        while ((QueueIndex < (NvM_Queue_IndexImmediate + NvM_Queue_CountImmediate)) && (RequestFound == FALSE))
        {
          if (NvM_Queue_Immediate[NVM_IMMEDIATE_QUEUE_GET_INDEX_MOD_SIZE(QueueIndex)].NvMBlockDescriptorIndex == BlockIndex)
          {
            /* Store the details of the cancelled request */
            QueueTemp = NvM_Queue_Immediate[NVM_IMMEDIATE_QUEUE_GET_INDEX_MOD_SIZE(QueueIndex)];

            /* Shift all items ahead of the cancelled request */
            for (QueueCopyIndex = QueueIndex;
                 QueueCopyIndex < (Queue_Immediate_TopIndex - 1U);
                 QueueCopyIndex++)
            {
              NvM_Queue_Immediate[NVM_IMMEDIATE_QUEUE_GET_INDEX_MOD_SIZE(QueueCopyIndex)] =
                NvM_Queue_Immediate[NVM_IMMEDIATE_QUEUE_GET_INDEX_MOD_SIZE(QueueCopyIndex + 1U)];
            }

            /* Put the cancelled request at the end of the queue (ahead of the current first element) */
            NvM_Queue_Immediate[NVM_IMMEDIATE_QUEUE_GET_INDEX_MOD_SIZE(((NvM_Queue_IndexImmediate + NVM_SIZE_IMMEDIATE_JOB_QUEUE) - NvM_Queue_CancelledImmediateJobs) - 1U)] = QueueTemp;

            /* Exit the loop, as the request was found */
            RequestFound = TRUE;
          }

          QueueIndex++;
        }

        /* Update the current number of cancelled immediate requests */
        NvM_Queue_CancelledImmediateJobs++;
      }
      else
#endif
      {
        /* Update the current number of cancelled standard requests */
        NvM_Queue_CancelledStandardJobs++;

        /* Start searching from the top of the standard queue (excluding already cancelled requests) */
        QueueIndex = Queue_Standard_TopIndex;
        while ((QueueIndex != NVM_QUEUE_INDEX_RESET) && (RequestFound == FALSE))
        {
          if (NvM_Queue_Standard[QueueIndex].NvMBlockDescriptorIndex == BlockIndex)
          {
            /* Store the details of the cancelled request */
            QueueTemp = NvM_Queue_Standard[QueueIndex];

            /* Shift all items above the cancelled request */
            for (QueueCopyIndex = QueueIndex;
                 QueueCopyIndex < Queue_Standard_TopIndex;
                 QueueCopyIndex++)
            {
              NvM_Queue_Standard[QueueCopyIndex] = NvM_Queue_Standard[QueueCopyIndex + 1U];
            }

            /* Put the cancelled request at the end of the queue */
            NvM_Queue_Standard[NVM_SIZE_STANDARD_JOB_QUEUE - NvM_Queue_CancelledStandardJobs] = QueueTemp;

            /* Exit the loop, as the request was found */
            RequestFound = TRUE;
          }

          QueueIndex--;
        }
      }

      Result = E_OK;
    }

    SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

#if (NVM_BSWM_BLOCK_STATUS_INFO == STD_ON)
    /* Check if block status is changed and block status reporting to BswM is
     * enabled for this block */
    if ((Result == E_OK) &&
        (NVM_BD_BLOCKSTATUSINFO(NvM_BlockDescriptorTable[BlockIndex].blockDesc)))
    {
      /* Block status changed since request is cancelled , report the request
       * status to BswM */
      BswM_NvM_CurrentBlockMode(BlockId, NVM_REQ_CANCELED);
    }
#endif

#if ( defined(NVM_MULTICORE_ENABLED) )
    NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus &= (uint8)(~(NVM_ES_MULTICORE_LOCK_MASK));
#endif
  }

  DBG_NVM_ASR40_CANCELJOBS_EXIT(Result, BlockId);
  return Result;

} /* end of function NvM_CancelJobs() */

#endif

#if ((NVM_ENABLE_ASR32_SERVICE_API == STD_ON) || \
       (NVM_DEFAULT_ASR_SERVICE_API == NVM_SERVICE_API_ASR32) \
    )
FUNC(void, NVM_CODE) NvM_ASR32_GetErrorStatus
(
  NvM_ASR32_BlockIdType                                       BlockId,
  P2VAR(NvM_ASR32_RequestResultType, AUTOMATIC,NVM_APPL_DATA ) RequestResultPtr
)
{
  NvM_ASR32_RequestResultType ASR40_Status;

  /* Set the value of RequestResultPtr only if NvM_ASR40_GetErrorStatus succeeded.
   * Otherwise, the content of RequestResultPtr shall not be modified.
   */
  if (E_OK == NvM_ASR40_GetErrorStatus(BlockId, &ASR40_Status))
  {
    /* !LINKSTO NvM.ASR32.RequestResult,1 */
    *RequestResultPtr = NvM_ASR40toASR32_StatusType(ASR40_Status);
  }
}

#if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)
FUNC(void, NVM_CODE) NvM_ASR32_SetDataIndex
(
  NvM_ASR32_BlockIdType BlockId, uint8 DataIndex
)
{
   /* Return type casted to void since the corresponding ASR40 API's returns  Std_ReturnType */
  (void)NvM_ASR40_SetDataIndex(BlockId, DataIndex);
}

FUNC(void, NVM_CODE) NvM_ASR32_GetDataIndex
(
  NvM_ASR32_BlockIdType                        BlockId,
  P2VAR( uint8,AUTOMATIC,NVM_APPL_DATA ) DataIndex
)
{
    /* Return type casted to void since the corresponding ASR40 API's returns  Std_ReturnType */
  (void)NvM_ASR40_GetDataIndex(BlockId, DataIndex);
}
#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1) */


#if (NVM_SET_RAM_BLOCK_STATUS_API == STD_ON)
FUNC(void, NVM_CODE) NvM_ASR32_SetRamBlockStatus
(
  NvM_ASR32_BlockIdType BlockId,
  boolean         BlockChanged
)
{
  /* Return type casted to void since the corresponding ASR40 API's returns  Std_ReturnType */

  (void)NvM_ASR40_SetRamBlockStatus(BlockId, BlockChanged);

}
#endif /* #if (NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) */

#if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)
FUNC( Std_ReturnType,NVM_CODE ) NvM_ASR32_ReadBlock
(
  NvM_ASR32_BlockIdType                       BlockId,
  P2VAR(uint8, AUTOMATIC, NVM_APPL_DATA)  NvM_DstPtr
)
{
  Std_ReturnType Result ;

  /* The BlockId argument are matching to the same base types */
#if (defined NVM_READBLOCK_CALLOUT)
  Result = NvM_Wrapper_ReadBlock(BlockId, NvM_DstPtr);
#else
  Result = NvM_ASR40_ReadBlock(BlockId, NvM_DstPtr);
#endif
  return Result;
}

FUNC( Std_ReturnType,NVM_CODE ) NvM_ASR32_WriteBlock
(
  NvM_ASR32_BlockIdType                        BlockId,
  P2CONST(uint8, AUTOMATIC, NVM_APPL_DATA) NvM_SrcPtr
)
{
  Std_ReturnType Result ;

  /* The BlockId argument are matching to the same base types */
#if (defined NVM_WRITEBLOCK_CALLOUT)
  Result = NvM_Wrapper_WriteBlock(BlockId, NvM_SrcPtr);
#else
  Result = NvM_ASR40_WriteBlock(BlockId, NvM_SrcPtr);
#endif
  return Result;
}

FUNC( Std_ReturnType,NVM_CODE ) NvM_ASR32_RestoreBlockDefaults
(
  NvM_ASR32_BlockIdType                       BlockId,
  P2VAR(uint8, AUTOMATIC, NVM_APPL_DATA) NvM_DestPtr
)
{
  Std_ReturnType Result ;

   /* The BlockId argument are matching to the same base types */
#if (defined NVM_RESTOREBLOCKDEFAULTS_CALLOUT)
  Result = NvM_Wrapper_RestoreBlockDefaults(BlockId, NvM_DestPtr);
#else
  Result = NvM_ASR40_RestoreBlockDefaults(BlockId,  NvM_DestPtr);
#endif
  return Result;
}
#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)  */

#if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3)

FUNC( Std_ReturnType,NVM_CODE ) NvM_ASR32_EraseNvBlock
(
  NvM_ASR32_BlockIdType BlockId
)
{
  Std_ReturnType Result ;


  /* The BlockId argument are matching to the same base types */
#if (defined NVM_ERASENVBLOCK_CALLOUT)
  Result = NvM_Wrapper_EraseNvBlock(BlockId);
#else
  Result = NvM_ASR40_EraseNvBlock(BlockId);
#endif

  return Result;
}

FUNC( Std_ReturnType,NVM_CODE ) NvM_ASR32_InvalidateNvBlock
(
  NvM_ASR32_BlockIdType BlockId
)
{
   Std_ReturnType Result ;


  /* The BlockId argument are matching to the same base types  */
#if (defined NVM_INVALIDATENVBLOCK_CALLOUT)
  Result = NvM_Wrapper_InvalidateNvBlock(BlockId);
#else
  Result = NvM_ASR40_InvalidateNvBlock(BlockId);
#endif

  return Result;
}

FUNC(void, NVM_CODE) NvM_ASR32_SetBlockProtection
(
  NvM_ASR32_BlockIdType BlockId,
  boolean         ProtectionEnabled
)
{
   /* Return type casted to void since the corresponding ASR40 API's returns  Std_ReturnType */

  (void)NvM_ASR40_SetBlockProtection(BlockId, ProtectionEnabled);

}
#endif /* #if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3) */
#endif /* #if (NVM_ENABLE_ASR32_SERVICE_API == STD_ON) */


#if ((NVM_ENABLE_ASR42_SERVICE_API == STD_ON) || \
       (NVM_DEFAULT_ASR_SERVICE_API == NVM_SERVICE_API_ASR42) \
    )
FUNC(Std_ReturnType, NVM_CODE) NvM_ASR42_GetErrorStatus
(
  NvM_ASR42_BlockIdType BlockId,
  P2VAR(NvM_ASR42_RequestResultType, AUTOMATIC,NVM_APPL_DATA) RequestResultPtr
)
{
  Std_ReturnType Result;

  Result = NvM_ASR40_GetErrorStatus(BlockId, RequestResultPtr);

  return Result;
}

#if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)
FUNC(Std_ReturnType, NVM_CODE) NvM_ASR42_SetDataIndex
(
  NvM_ASR42_BlockIdType BlockId,
  uint8 DataIndex
)
{
  Std_ReturnType Result;

  Result = NvM_ASR40_SetDataIndex(BlockId, DataIndex);

  return Result;
}

FUNC(Std_ReturnType, NVM_CODE) NvM_ASR42_GetDataIndex
(
  NvM_ASR42_BlockIdType BlockId,
  P2VAR(uint8, AUTOMATIC, NVM_APPL_DATA) DataIndex
)
{
  Std_ReturnType Return;

  Return = NvM_ASR40_GetDataIndex(BlockId, DataIndex);

  return Return;
}
#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1) */

#if (NVM_SET_RAM_BLOCK_STATUS_API == STD_ON)
FUNC(Std_ReturnType, NVM_CODE) NvM_ASR42_SetRamBlockStatus
(
  NvM_ASR42_BlockIdType BlockId,
  boolean BlockChanged
)
{
  Std_ReturnType Return;

  Return = NvM_ASR40_SetRamBlockStatus(BlockId, BlockChanged);

  return Return;

}
#endif /* #if (NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) */

#if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)
FUNC(Std_ReturnType, NVM_CODE) NvM_ASR42_ReadBlock
(
  NvM_ASR42_BlockIdType BlockId,
  P2VAR(void, AUTOMATIC, NVM_APPL_DATA) NvM_DstPtr
)
{
  Std_ReturnType Result;

#if (defined NVM_READBLOCK_CALLOUT)
  Result = NvM_Wrapper_ReadBlock(BlockId, NvM_DstPtr);
#else
  Result = NvM_ASR40_ReadBlock(BlockId, NvM_DstPtr);
#endif

  return Result;
}

FUNC(Std_ReturnType, NVM_CODE) NvM_ASR42_WriteBlock
(
  NvM_ASR42_BlockIdType BlockId,
  P2CONST(void, AUTOMATIC, NVM_APPL_DATA) NvM_SrcPtr
)
{
  Std_ReturnType Result;

#if (defined NVM_WRITEBLOCK_CALLOUT)
  Result = NvM_Wrapper_WriteBlock(BlockId, NvM_SrcPtr);
#else
  Result = NvM_ASR40_WriteBlock(BlockId, NvM_SrcPtr);
#endif

  return Result;
}

FUNC(Std_ReturnType, NVM_CODE) NvM_ASR42_RestoreBlockDefaults
(
  NvM_ASR42_BlockIdType BlockId,
  P2VAR(void, AUTOMATIC, NVM_APPL_DATA) NvM_DestPtr
)
{
  Std_ReturnType Result;

#if (defined NVM_RESTOREBLOCKDEFAULTS_CALLOUT)
  Result = NvM_Wrapper_RestoreBlockDefaults(BlockId, NvM_DestPtr);
#else
  Result = NvM_ASR40_RestoreBlockDefaults(BlockId,  NvM_DestPtr);
#endif

  return Result;
}
#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)  */

#if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3)
FUNC(Std_ReturnType, NVM_CODE) NvM_ASR42_EraseNvBlock
(
  NvM_ASR42_BlockIdType BlockId
)
{
  Std_ReturnType Result;

#if (defined NVM_ERASENVBLOCK_CALLOUT)
  Result = NvM_Wrapper_EraseNvBlock(BlockId);
#else
  Result = NvM_ASR40_EraseNvBlock(BlockId);
#endif

  return Result;
}

FUNC(Std_ReturnType, NVM_CODE) NvM_ASR42_InvalidateNvBlock
(
  NvM_ASR42_BlockIdType BlockId
)
{
  Std_ReturnType Result;

#if (defined NVM_INVALIDATENVBLOCK_CALLOUT)
  Result = NvM_Wrapper_InvalidateNvBlock(BlockId);
#else
  Result = NvM_ASR40_InvalidateNvBlock(BlockId);
#endif

  return Result;
}

FUNC(Std_ReturnType, NVM_CODE) NvM_ASR42_SetBlockProtection
(
  NvM_ASR42_BlockIdType BlockId,
  boolean ProtectionEnabled
)
{
  Std_ReturnType Result;

  Result = NvM_ASR40_SetBlockProtection(BlockId, ProtectionEnabled);

  return Result;
}
#endif /* #if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3) */
#endif /* #if (NVM_ENABLE_ASR42_SERVICE_API == STD_ON) */


/* ******************************* Callback Functions ******************************* */
/* !LINKSTO NVM438,1 */
FUNC(void,NVM_CODE) NvM_JobEndNotification(void)
{
  DBG_NVM_JOBENDNOTIFICATION_ENTRY();
  /* Underlaying module terminated successfully */

  DBG_NVM_JOBENDNOTIFICATION_EXIT();
}

FUNC(void,NVM_CODE) NvM_JobErrorNotification(void)
{
  DBG_NVM_JOBERRORNOTIFICATION_ENTRY();
  /* Underlaying module terminated with an error */

  DBG_NVM_JOBERRORNOTIFICATION_EXIT();
}

/*==================[internal function definitions]==========================*/

#if ((NVM_ENABLE_ASR32_SERVICE_API == STD_ON) || \
       (NVM_DEFAULT_ASR_SERVICE_API == NVM_SERVICE_API_ASR32) \
    )
STATIC FUNC(NvM_RequestResultType, NVM_CODE) NvM_ASR40toASR32_StatusType
(
  NvM_RequestResultType Status
)
{
  NvM_RequestResultType ASR32_Status;

  switch(Status)
  {
  case NVM_REQ_REDUNDANCY_FAILED:
    ASR32_Status = NVM_REQ_OK;
    break;

  case NVM_REQ_RESTORED_DEFAULTS:
    ASR32_Status = NVM_REQ_OK;
    break;

  default:
    ASR32_Status = Status;
    break;
  }

  return ASR32_Status;
}

#endif


#if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3)

STATIC FUNC(void,NVM_CODE) NvM_InvalidateNvBlock_Async(void)
{
  /* Since the processing of NvM_InvalidateNvBlock() has
   * been started, NvM_CurrentServiceId needs to be set
   */
  NvM_CurrentServiceId = NVM_INVALIDATE_NV_BLOCK_API_ID;

  /* depending on the block type the state machine stack must be
     prepared. */
  NvM_StateMachine_ProcessRedundantBlock( &NvM_InvalidateNvBlock_Async_State1 );
}

STATIC FUNC(void,NVM_CODE) NvM_InvalidateNvBlock_Async_State1(void)
{
  VAR( MemIf_StatusType, AUTOMATIC ) Status;

  /* Check if the device is not MEMIF_BUSY */
  /* In case of a redundant block this shall detect if the second copy (DATASET=1)
     is completed before starting processing first copy (DATASET=0) */
  Status = MemIf_GetStatus( NVM_BD_DEVICEID(NvM_CurrentBlockDescriptorPtr->blockDesc));

  if ( MEMIF_BUSY != Status )
  {
    if( MemIf_InvalidateBlock( NVM_BD_DEVICEID(NvM_CurrentBlockDescriptorPtr->blockDesc),
                               NvM_GetMergedBlockNumber()
                              ) == E_OK
      )
    {
      /* Poll job result. */
      NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_StateMachine_ProcessJobResult;
    }
    else
    {
      /* process error and terminate */
      NvM_StateMachine_RedundantBlockError();
    }
  }
  else
  {
    /* If the MemIf returns busy, return from MainFunction */

    DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_PENDING));
    NvM_GlobalErrorStatus = NVM_REQ_PENDING;
  }
}

#if ( NVM_JOB_PRIORITIZATION == STD_ON )

STATIC FUNC(void,NVM_CODE) NvM_EraseNvBlock_Async(void)
{
  /* Since the processing of NvM_EraseNvBlock() has
   * been started, NvM_CurrentServiceId needs to be set
   */
  NvM_CurrentServiceId = NVM_ERASE_NV_BLOCK_API_ID;

  /* depending on the block type the state machine stack must be
   * prepared. */
  NvM_StateMachine_ProcessRedundantBlock( &NvM_EraseNvBlock_Async_State1 );
}

STATIC FUNC(void,NVM_CODE) NvM_EraseNvBlock_Async_State1(void)
{
  VAR( MemIf_StatusType, AUTOMATIC ) Status;

  /* Check if the device is not MEMIF_BUSY */
  /* In case of a redundant block this shall detect if the second copy (DATASET=1)
     is completed before starting processing first copy (DATASET=0) */
  Status = MemIf_GetStatus( NVM_BD_DEVICEID(NvM_CurrentBlockDescriptorPtr->blockDesc) );

  if ( MEMIF_BUSY != Status )
  {
    if ( MemIf_EraseImmediateBlock( NVM_BD_DEVICEID(NvM_CurrentBlockDescriptorPtr->blockDesc),
                                    NvM_GetMergedBlockNumber()
                                  ) == E_OK
       )
    {
      /* Poll job result. */
      NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_StateMachine_ProcessJobResult;
    }
    else
    {
      /* process error and terminate */
      NvM_StateMachine_RedundantBlockError();
    }
  }
  else
  {
    /* If the MemIf returns busy, return from MainFunction */

    DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_PENDING));
    NvM_GlobalErrorStatus = NVM_REQ_PENDING;
  }
}

#endif /* #if ( NVM_JOB_PRIORITIZATION == STD_ON ) */

#endif /* #if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3) */

#if ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U))

FUNC(void,NVM_CODE) NvM_SetRamBlockStatus_Async_CalcCrc_Start(void)
{
#if (NVM_SYNC_MECHANISM == STD_ON)
  /* Result of the mirror copy callback */
  Std_ReturnType ReturnVal = E_OK;
#endif
  /* local variable to store pointer to user data */
  NvM_PtrToApplDataType BufferAddr;

  /* get the data address */
  BufferAddr = NvM_GetUserRamBlockAddress();
  /* Since the processing of NvM_SetRamBlockStatus() has
   * been started, NvM_CurrentServiceId needs to be set
   */

  /* check if call for function was for NvM_ValidateAll */
  if (NVM_VALIDATE_ALL_API_ID != NvM_CurrentServiceId)
  {
    /* if no ValidateAll request is ongoing, set current serviceId to NVM_SET_RAM_BLOCK_STATUS_API_ID */
    NvM_CurrentServiceId = NVM_SET_RAM_BLOCK_STATUS_API_ID;
  }
  /* Set global error status to NVM_REQ_NOT_OK */
  NvM_GlobalErrorStatus = NVM_REQ_NOT_OK;

#if (NVM_BSWM_BLOCK_STATUS_INFO == STD_ON)
  /* in case retries are performed for mirror copy operation do not call
      BswM notification again */
#if (NVM_SYNC_MECHANISM == STD_ON)
  if (NvM_MirrorRetryCount == 0U)
#endif
  {
    /* Check if block status reporting to BswM is enabled for current block */
    if (NVM_BD_BLOCKSTATUSINFO(NvM_CurrentBlockDescriptorPtr->blockDesc))
    {
      /* Block status changed since set Ram block status request has started,
       * report the request status to BswM */
      BswM_NvM_CurrentBlockMode(NvM_GlobalBlockNumber, NVM_REQ_PENDING);
    }
  }
#endif
  /* if explicit synch is enabled call Mirror callback to copy
     the data from NvM user */
#if (NVM_SYNC_MECHANISM == STD_ON)
  if (NVM_CHECK_WORKING_STATUS(NVM_DYN_MIRROR_MASK))
  {
    /* !LINKSTO NvM.SetRamBlockStatus.ExplicitSynch.CrcCalculation,1 */
    ReturnVal = NvM_MirrorCopy(NULL_PTR,
          NvM_CurrentBlockDescriptorPtr->writeRamToNvCallback, BufferAddr);
  }

  if (E_OK != ReturnVal)
  {
    if (NvM_MirrorRetryCount > 0)
    {
      /* Update NvM_CurrentFunctionPtr for next mirror operation retry */
      NvM_CurrentFunctionPtr[NvM_GlobalCallLevel]
          = &NvM_SetRamBlockStatus_Async_CalcCrc_Start;
    }
  }
  else
#endif
  {
    /* Mirror Operation OK - update NvM_CurrentFunctionPtr */
    NvM_CurrentFunctionPtr[NvM_GlobalCallLevel]
      = &NvM_SetRamBlockStatus_Async_CalcCrc_End;
    /* Call CRC calculation function */
    NvM_CalculateCrc(BufferAddr);
  }
}

STATIC FUNC(void,NVM_CODE) NvM_SetRamBlockStatus_Async_CalcCrc_End(void)
{
  /* STEP_1: Copy CRC to the global buffer */
  NvM_CopyCrc( NvM_GetStoredRamCRCAddress(),
              (NvM_PtrToApplDataType)&NvM_CalcCrc_CalcBuffer,
              NvM_CurrentBlockDescriptorPtr->blockDesc );

  /* STEP_2: set result to NVM_REQ_OK */
  NvM_GlobalErrorStatus = NVM_REQ_OK;

  /* STEP_3: terminate service */
  NvM_StateMachine_TerminateCurrentLevel();
  /* STEP_4: Reset the RAM block CRC calculation flag */
  NvM_CalcCrc_RemoveElement();
  /* STEP_5: A Ram block CRC calculation request is finished */
}

#endif  /* ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U)) */

#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>

/*==================[end of file]============================================*/
