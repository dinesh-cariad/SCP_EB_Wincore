/**
 * \file
 *
 * \brief AUTOSAR Fee
 *
 * This file contains the implementation of the AUTOSAR
 * module Fee.
 *
 * \version 6.14.13
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

/*==================[inclusions]=============================================*/

#include <Fee_Trace.h>
/* !LINKSTO FEE002,1 */
#include <Fee.h>                 /* External API declarations  */
#include <Fee_Jobs.h>            /* Internal function prototypes for Fee jobs */
#include <Fee_Internal.h>        /* Internal function prototypes and internal defines and declarations*/
#include <Fee_Initialization.h>  /* Internal function prototypes for Fee initialization*/
#include <Fee_Sections.h>        /* Internal function prototypes for Fee section operations*/
#include <Fee_SectionsSwitch.h>  /* Internal function prototypes for Fee section operations*/
#if FEE_POLLING_MODE == STD_OFF
#include <Fee_Cbk.h>             /* Not needed. Included only to prevent warning. */
#endif /* FEE_POLING_MODE == STD_OFF */

/*==================[macros]=================================================*/

/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef FEE_VENDOR_ID /* configuration check */
#error FEE_VENDOR_ID must be defined
#endif

#if (FEE_VENDOR_ID != 1U) /* vendor check */
#error FEE_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef FEE_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error FEE_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (FEE_AR_RELEASE_MAJOR_VERSION != 4U)
#error FEE_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef FEE_AR_RELEASE_MINOR_VERSION /* configuration check */
#error FEE_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (FEE_AR_RELEASE_MINOR_VERSION != 0U )
#error FEE_AR_RELEASE_MINOR_VERSION wrong (!= 0U)
#endif

#ifndef FEE_AR_RELEASE_REVISION_VERSION /* configuration check */
#error FEE_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (FEE_AR_RELEASE_REVISION_VERSION != 3U )
#error FEE_AR_RELEASE_REVISION_VERSION wrong (!= 3U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef FEE_SW_MAJOR_VERSION /* configuration check */
#error FEE_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (FEE_SW_MAJOR_VERSION != 6U)
#error FEE_SW_MAJOR_VERSION wrong (!= 6U)
#endif

#ifndef FEE_SW_MINOR_VERSION /* configuration check */
#error FEE_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (FEE_SW_MINOR_VERSION < 14U)
#error FEE_SW_MINOR_VERSION wrong (< 14U)
#endif

#ifndef FEE_SW_PATCH_VERSION /* configuration check */
#error FEE_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (FEE_SW_PATCH_VERSION < 13U)
#error FEE_SW_PATCH_VERSION wrong (< 13U)
#endif


/*==================[type definitions]=======================================*/

/** \brief Structure of state functions pointers */
typedef struct
{
  void (*onEntry)(void); /**< \brief Function to be executed on state entry */
  void (*fn)(void); /**< \brief Function to be executed during state activity */

} Fee_StateDescType;

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]=========================================*/

#define FEE_START_SEC_VAR_INIT_UNSPECIFIED
#include <MemMap.h>
/* !LINKSTO Fee.MainFunction.InitCheck,1 */
VAR(Fee_State_t, FEE_VAR) Fee_State = FEE_UNINIT;
VAR(Fee_State_t, FEE_VAR) Fee_NextState = FEE_STATE_INVALID;
/* Pointer used to address the blocks configuration. */
P2CONST(Fee_BlockConfiguration_t, AUTOMATIC, FEE_VAR) Fee_BlockCfgPtr = NULL_PTR;

#if (FEE_NUMBER_NOT_CONFIGURED_BLOCKS > 0U)
/* The configuration of not configured blocks found at runtime */
VAR(Fee_BlockConfiguration_t, FEE_VAR) Fee_NotCfgBlocks[FEE_NUMBER_NOT_CONFIGURED_BLOCKS] = {{0U,0U,0U,FALSE}};
#endif

#define FEE_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <MemMap.h>

#define FEE_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <MemMap.h>
/* !LINKSTO Fee.GlobalVariables,1 */
VAR(Fee_Gv_t, FEE_VAR) Fee_Gv;

#define FEE_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <MemMap.h>

/*==================[internal data]==========================================*/

#define FEE_START_SEC_VAR_CLEARED_32
#include <MemMap.h>

uint32 FeeBuffer[FEE_BUFFER_SIZE_32];

#define FEE_STOP_SEC_VAR_CLEARED_32
#include <MemMap.h>

#define FEE_START_SEC_CONST_UNSPECIFIED
#include <MemMap.h>

/* \brief Array of function pointers of main state machine */
STATIC CONST(Fee_StateDescType, FEE_CONST) Fee_States[FEE_NUM_STATES] =
{
  {NULL_PTR, NULL_PTR},
  {NULL_PTR, &Fee_SfIdleState},
  {&Fee_SfOnEntryInitReadHeader, &Fee_SfInitReadHeader},
  {&Fee_SfOnEntryInitReadFooter, &Fee_SfInitReadFooter},
  {&Fee_SfOnEntryInitFillCache, &Fee_SfInitFillCache},
  {&Fee_SfOnEntryInitBlockByBlock, &Fee_SfInitBlockByBlock},
  {&Fee_SfOnEntryReadBlockData, &Fee_SfReadBlockData},
  {&Fee_SfOnEntryWriteBlockInfo, &Fee_SfWriteBlockInfo},
  {&Fee_SfOnEntryWriteBlockData, &Fee_SfWriteBlockData},
  {&Fee_SfOnEntryWriteWrittenMarker, &Fee_SfWriteWrittenMarker},
  {&Fee_SfOnEntrySSWriteWrittenMarker, &Fee_SfSSWriteWrittenMarker},
  {&Fee_SfOnEntryMarkHeaderActive, &Fee_SfMarkHeaderFooterActive},
  {&Fee_SfOnEntryMarkFooterActive, &Fee_SfMarkHeaderFooterActive},
  {&Fee_SfOnEntryMarkHeaderErasable, &Fee_SfMarkHeaderErasable},
  {&Fee_SfOnEntryMarkFooterErasable, &Fee_SfMarkFooterErasable},
  {&Fee_SfOnEntryWriteSectionHeader, &Fee_SfWriteSectionHeader},
  {&Fee_SfOnEntryWriteSectionFooter, &Fee_SfWriteSectionFooter},
  {&Fee_SfOnEntryInvalidate, &Fee_SfWriteBlockInfo},
  {&Fee_SfOnEntryEraseImmediateBlock ,&Fee_SfWriteBlockInfo},
  {&Fee_SfOnEntrySSCopyInfo, &Fee_SfSSCopyInfo},
  {&Fee_SfOnEntrySSReadData, &Fee_SfSSReadData},
  {&Fee_SfOnEntrySSCopyData, &Fee_SfSSCopyData},
  {&Fee_SfOnEntryEraseSection, &Fee_SfEraseSection},
  {NULL_PTR, &Fee_SfWaitACycle},
  {&Fee_SfTriggerJobOrInternal, NULL_PTR},
#if (FEE_CRITICAL_BLOCK_NUMBER != 0U)
  {&Fee_SfFrozenCritical, &Fee_SfFrozenCritical},
#endif /* (FEE_CRITICAL_BLOCK_NUMBER != 0U) */
#if (FEE_CONSISTENCY_PATTERN_ENABLED == STD_ON)
#if (FEE_CONSISTENCY_START_PATTERN_ENABLED == STD_ON)
  {&Fee_SfOnEntrySSWriteBlockInfoStartPattern, &Fee_SfSSWriteBlockInfoStartPattern},
  {&Fee_SfOnEntryWriteBlockInfoStartPattern, &Fee_SfWriteBlockInfoStartPattern},
  {&Fee_SfOnEntrySSWriteBlockDataStartPattern, &Fee_SfSSWriteBlockDataStartPattern},
  {&Fee_SfOnEntryWriteBlockDataStartPattern, &Fee_SfWriteBlockDataStartPattern},
  {&Fee_SfOnEntryReadBlockDataStartPattern,  &Fee_SfReadBlockDataStartPattern},
#endif
  {&Fee_SfOnEntryWriteBlockInfoEndPattern, &Fee_SfWriteBlockInfoEndPattern},
  {&Fee_SfOnEntrySSWriteBlockInfoEndPattern, &Fee_SfSSWriteBlockInfoEndPattern},
  {&Fee_SfOnEntryWriteHeaderSectionManagementEndPattern, &Fee_SfWriteHeaderSectionManagementEndPattern},
  {&Fee_SfOnEntryWriteFooterSectionManagementEndPattern, &Fee_SfWriteFooterSectionManagementEndPattern},
#endif
};

#define FEE_STOP_SEC_CONST_UNSPECIFIED
#include <MemMap.h>

/*==================[external function definitions]==========================*/

#define FEE_START_SEC_CODE
#include <MemMap.h>

/*--------------------------[Fee_Init]---------------------------------------*/

FUNC(void, FEE_CODE) Fee_Init
(
  void
)
{
  uint16 i;

  DBG_FEE_INIT_ENTRY();
  /* Initialize Fee internal structure */
  /* First Fee mode should be FEE_MODE_STARTUP1 */
  Fee_Gv.FeeMode = FEE_MODE_STARTUP1;
  /* Fee internal flag - set first step of the initialization to pending */
  Fee_Gv.FeeInternalFlags = 0U;
  /* The block number of the current job */
  Fee_Gv.FeeIntBlockIndex = 0U;
  /* index of the last copied block - no block pending*/
  Fee_Gv.FeeCopyBlockIndex = 0U;
  /* !LINKSTO Fee.Dsn.Initialization.Idle,1 */
  /* Fee delay counter - after StartUp1 Fee shall do nothing until a job comes */
  Fee_Gv.FeeDelayTimer = FEE_INFINITE_DELAY;
  /* retry counter for section status - no retry ongoing */
  Fee_Gv.FeeRetryStateCounter = FEE_STATE_MAX_RETRIES;
  /* Fee state for pending retry - no retry pending */
  Fee_Gv.FeeRetryState = FEE_STATE_INVALID;
  /* No current switch state */
  Fee_Gv.FeeSwitchState = FEE_STATE_INVALID;
#if (FEE_CRITICAL_BLOCK_NUMBER != 0U)
  /* No current critical state */
  Fee_Gv.FeeCriticalState = FEE_STATE_INVALID;
  /* address of block info - not available */
  Fee_Gv.FeeInfoAddressCritical = 0U;
  /* address of block data - not available */
  Fee_Gv.FeeDataAddressCritical = 0U;
#endif
  /* User pending job - no job pending */
  Fee_Gv.FeeJob = FEE_NO_JOB;
  /* Internal pending job - no job pending */
  Fee_Gv.FeeSwitchJob = FEE_NO_JOB;
  /* Fee job result - no job */
  Fee_Gv.FeeJobResult = MEMIF_JOB_OK;
  /* flash job result - no job */
  Fee_Gv.FeeFlashJobResult = MEMIF_JOB_OK;
  /* address of block info - not available */
  Fee_Gv.FeeWorkingInfoAddress = 0U;
  /* address of block data - not available */
  Fee_Gv.FeeWorkingDataAddress = 0U;
  /* address of block data - not available */
  Fee_Gv.FeeSwitchSourceAddress = 0U;
  /* nothing to switch yet */
  Fee_Gv.FeeSwitchBlockRemSize = 0U;
  /* nothing to switch yet */
  Fee_Gv.FeeSwitchBlockSizeToCopy = 0U;
  /* nothing to switch yet */
  Fee_Gv.FeeSwitchRequiredSpace = 0U;
  /* index in Fee buffer - not available */
  Fee_Gv.Fee_BufferIndex = 0U;
  /* The size considered written - not available */
  Fee_Gv.FeeLostSectionSize = 0;
  /* address of block info - not available */
  Fee_Gv.FeeInfoAddress = 0U;
  /* address of block data - not available */
  Fee_Gv.FeeDataAddress = 0U;
  /* index of the newest active section */
  Fee_Gv.FeeActiveSectionIndex = 255U;
  /* index of the oldest active section */
  Fee_Gv.FeeOldestSectionIndex = 255U;
  /* flash erase cycle counters to the maximum value (invalid value) */
  Fee_Gv.SectionEraseCounter = 1U;
  /* initialize with NULL ptr */
  Fee_Gv.TriggerFunct = NULL_PTR;
  /* At first no block is found */
  Fee_Gv.FeeNbOfBlocksFound = 0U;
#if (FEE_FREEZE_ACTIVITIES_API == STD_ON)
  Fee_Gv.FreezeActivities = 0;
#endif
  /* !LINKSTO Fee.Dsn.Initialization.CacheInit,1 */
  /* Update cache entry for every block as inconsistent, as the
   * blocks are not yet identified and thus not available for read */
  /* !LINKSTO Fee.Dsn.Initialization.Startup1.BlockInconsistent.01,1 */
  for (i = 0U; i < FEE_TOTAL_NUMBER_OF_BLOCKS; ++i)
  {
    Fee_Gv.FeeCache[i] = FEE_ZERO_BLOCKADDRESS;
    Fee_Gv.FeeSwRestartCache[i] = FEE_ZERO_BLOCKADDRESS;
#if (FEE_DYNAMIC_BLOCK_LENGTH == STD_ON)
    if (i < FEE_NO_OF_CONFIGURED_BLOCKS)
    {
      Fee_Gv.FeeOldSizes[i] = Fee_BlockCfg[i].FeeBlockSize;
    }
    else
    {
      /* do nothing */
    }
#endif
  }

#if (FEE_NUMBER_NOT_CONFIGURED_BLOCKS > 0U)
  /* initialize the number of not configured blocks found */
  Fee_Gv.FeeNbOfNotCfgBlocksFound = 0U;
  /* initialize not configured blocks */
  /* Deviation TASKING-1 */
  for (i = 0U; i < FEE_NUMBER_NOT_CONFIGURED_BLOCKS; i++)
  {
    /* set the block length to invalid */
    Fee_NotCfgBlocks[i].FeeBlockSize = FEE_INVALID_SIZE;
    Fee_NotCfgBlocks[i].FeeBlockNumber = 0U;
    Fee_NotCfgBlocks[i].FeeBlockAlignedSize = FEE_INVALID_SIZE;
  }
#endif

  /* Initialize buffer pointer */
  Fee_Gv.FeeBuffPtr = (uint8*)&FeeBuffer[0];

  /* Initialize sections headers statuses */
  for (i = 0U; i < FEE_NUMBER_OF_SECTIONS; ++i)
  {
    Fee_Gv.Fee_SectionManagement[i].SectionStatus = FEE_SECTION_UNKNOWN_STATUS;
    Fee_Gv.Fee_SectionManagement[i].SCounter = 0U;
  }
  /*current section index initialization*/
  Fee_Gv.FeeWorkingSectionIndex = 0U;

  /* Number of BlockInfos found empty at FillCache step */
  Fee_Gv.Fee_NumberOfMarginBlockInfos = 0;

  /* Initiate Startup1 */
  /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.02,1 */
  FEE_TRIGGER_TRANS(FEE_INIT_READ_HEADER);

  /* Fee module state */
  DBG_FEE_STATE((Fee_State),(FEE_IDLE));
  Fee_State = FEE_STATE_INVALID;

  DBG_FEE_INIT_EXIT();
}

/*---------------------------------[Fee_SetMode]---------------------------------------*/

#if (FEE_SET_MODE_SUPPORTED == STD_ON)

FUNC(void, FEE_CODE) Fee_SetMode(MemIf_ModeType Mode)
{
  DBG_FEE_SETMODE_ENTRY(Mode);
#if (FEE_DEV_ERROR_DETECT == STD_ON)

  /* is the module initialized? */

  if(FEE_UNINIT == Fee_State)
  {
    FEE_DET_REPORT_ERROR(FEE_SETMODE_API_ID, FEE_E_UNINIT);
  }

  /* is the module busy? */
  else if(FEE_NO_JOB != Fee_Gv.FeeJob)
  {
    FEE_DET_REPORT_ERROR(FEE_SETMODE_API_ID, FEE_E_BUSY);
  }

  /* is the given mode valid? */
  else if((MEMIF_MODE_FAST != Mode) && (MEMIF_MODE_SLOW != Mode))
  {
    /* !LINKSTO Fee.Mode.DET,1 */
    FEE_DET_REPORT_ERROR(FEE_SETMODE_API_ID, FEE_E_INVALID_MODE);
  }
  else
#else
    /* !LINKSTO SWS_Fee_00190,1 */
    /* Fls_Set mode shall only be requested in case Fee status is MEMIF_IDLE */
  if((FEE_NO_JOB == Fee_Gv.FeeJob) && (FEE_UNINIT != Fee_State))
#endif
  {
    /* Set the operation mode of the flash driver */
    FEE_FLS_SETMODE(Mode);
  }

  DBG_FEE_SETMODE_EXIT(Mode);
}

#endif

/*------------------------------------[Fee_Read]---------------------------------------*/

FUNC(Std_ReturnType, FEE_CODE) Fee_Read
(
  uint16                                 BlockNumber,
  uint16                                 BlockOffset,
  P2VAR(uint8, AUTOMATIC, FEE_APPL_DATA) DataBufferPtr,
  uint16                                 Length
)
{
  uint16 IntBlockNumber;
  Std_ReturnType RetValue = E_NOT_OK;
#if  ((FEE_WRITE_CUSTOM_API == STD_ON) || (FEE_READ_CUSTOM_API == STD_ON))
  boolean PreemptionProtection;
#endif /* ((FEE_WRITE_CUSTOM_API == STD_ON) || (FEE_READ_CUSTOM_API == STD_ON)) */

  DBG_FEE_READ_ENTRY(BlockNumber,BlockOffset,DataBufferPtr,Length);

#if  ((FEE_WRITE_CUSTOM_API == STD_ON) || (FEE_READ_CUSTOM_API == STD_ON))
  PreemptionProtection = Fee_CheckAndSetPreemptionProtection();
  if (PreemptionProtection == TRUE)
  {
    /* Report the preemption Det error */
    /* !LINKSTO Fee.PreemptionProtection.DET,1 */
    FEE_DET_REPORT_ERROR(FEE_READ_API_ID, FEE_E_PREEMPTION_OCCURED);
  }
  else
#endif /* ((FEE_WRITE_CUSTOM_API == STD_ON) || (FEE_READ_CUSTOM_API == STD_ON)) */
  {
    /* !LINKSTO Fee.FeeCriticalBlock.StopBlockJobs,1 */
#if (FEE_CRITICAL_BLOCK_NUMBER != 0U)
    if ((FEE_CRITICAL_FROZEN == Fee_State) && (FEE_CRITICAL_BLOCK_NUMBER != BlockNumber))
    {
      /* Do not accept the job. */
    }
    else
#endif /* (FEE_CRITICAL_BLOCK_NUMBER != 0U) */
    {
      /* Is the module initialized? */
      /* Fee_Init API should have been called by now, and no job should be already memorised */
      /* !LINKSTO Fee.Initializaion.UserJobRequests,1 */
      /* !LINKSTO SWS_Fee_00022,1 */
      /* !LINKSTO SWS_Fee_00172,1 */
      if (FEE_UNINIT == Fee_State)
      {
#if (FEE_DEV_ERROR_DETECT == STD_ON)
        FEE_DET_REPORT_ERROR(FEE_READ_API_ID, FEE_E_UNINIT);
#endif
      }

      /* is the module busy? */
      /* !LINKSTO SWS_Fee_00172,1 */
      else if(FEE_NO_JOB != Fee_Gv.FeeJob)
      {
#if (FEE_DEV_ERROR_DETECT == STD_ON)
        FEE_DET_REPORT_ERROR(FEE_READ_API_ID, FEE_E_BUSY);
#endif
      }
#if (FEE_DEV_ERROR_DETECT == STD_ON)
      /* Is the data buffer valid? */
      else if (NULL_PTR == DataBufferPtr)
      {
        FEE_DET_REPORT_ERROR(FEE_READ_API_ID, FEE_E_INVALID_DATA_PTR);
      }
#endif
      else
      {
        /* Get the block index of requested block */
        IntBlockNumber = Fee_SearchConfigTable(BlockNumber);

#if (FEE_DEV_ERROR_DETECT == STD_ON)
        /* has the block number been configured in the BlockCfg Table? */
        if (IntBlockNumber >= FEE_NO_OF_CONFIGURED_BLOCKS)
        {
          FEE_DET_REPORT_ERROR(FEE_READ_API_ID, FEE_E_INVALID_BLOCK_NO);
        }

        /* is the block offset valid? */
        else if ( BlockOffset >= Fee_BlockCfg[IntBlockNumber].FeeBlockSize)
        {
          FEE_DET_REPORT_ERROR(FEE_READ_API_ID, FEE_E_INVALID_BLOCK_OFS);
        }

        /* is the block length valid? */
        else if ((Length + BlockOffset) > Fee_BlockCfg[IntBlockNumber].FeeBlockSize)
        {
          FEE_DET_REPORT_ERROR(FEE_READ_API_ID, FEE_E_INVALID_BLOCK_LEN);
        }
        else
#endif
        {
          Fee_Gv.FeeIntBlockIndex = IntBlockNumber;
          Fee_Gv.FeeBlockDataReadBuffPtr = DataBufferPtr;
          Fee_Gv.FeeLength = Length;
          Fee_Gv.FeeBlockReadOffset = BlockOffset;
          Fee_Gv.FeeJob = FEE_READ_JOB;
          Fee_Gv.FeeJobResult = MEMIF_JOB_PENDING;
          RetValue = E_OK;
        }
      }
    }
    /* Reset the preemption flag */
    FEE_RESET_PREEMPTION_PROTECTION();
  }

  DBG_FEE_READ_EXIT(RetValue,BlockNumber,BlockOffset,DataBufferPtr,Length);
  return RetValue;
}

/*----------------------------------[Fee_Write]---------------------------------------*/

FUNC(Std_ReturnType, FEE_CODE) Fee_Write
(
  uint16                                 BlockNumber,
  P2CONST(uint8, AUTOMATIC, FEE_APPL_DATA) DataBufferPtr
)
{
  uint16 IntBlockNumber;
  Std_ReturnType ReturnValue = E_NOT_OK;
#if  ((FEE_WRITE_CUSTOM_API == STD_ON) || (FEE_READ_CUSTOM_API == STD_ON))
  boolean PreemptionProtection;
#endif /* ((FEE_WRITE_CUSTOM_API == STD_ON) || (FEE_READ_CUSTOM_API == STD_ON)) */

  DBG_FEE_WRITE_ENTRY(BlockNumber,DataBufferPtr);

#if  ((FEE_WRITE_CUSTOM_API == STD_ON) || (FEE_READ_CUSTOM_API == STD_ON))
  PreemptionProtection = Fee_CheckAndSetPreemptionProtection();
  if (PreemptionProtection == TRUE)
  {
    /* Report the preemption Det error */
    /* !LINKSTO Fee.PreemptionProtection.DET,1 */
    FEE_DET_REPORT_ERROR(FEE_WRITE_API_ID, FEE_E_PREEMPTION_OCCURED);
  }
  else
#endif /* ((FEE_WRITE_CUSTOM_API == STD_ON) || (FEE_READ_CUSTOM_API == STD_ON)) */
  {
    /* !LINKSTO Fee.FeeCriticalBlock.StopBlockJobs,1 */
#if (FEE_CRITICAL_BLOCK_NUMBER != 0U)
    if ((FEE_CRITICAL_FROZEN == Fee_State)
        ||
        /* !LINKSTO Fee.Dsn.Critical.InterruptItself,1 */
        /* A critical can not interrupt a critical write or a critical read. */
        ((Fee_BlockCfg[Fee_Gv.FeeIntBlockIndex].FeeBlockNumber == FEE_CRITICAL_BLOCK_NUMBER)
            && ((FEE_WRITE_JOB == Fee_Gv.FeeJob) || (FEE_READ_BLOCKDATA == Fee_State))))
    {
      /* Do not accept the job. */
    }
    else
#endif /* (FEE_CRITICAL_BLOCK_NUMBER != 0U) */
    {
#if (FEE_CRITICAL_BLOCK_NUMBER != 0U)
      if (FEE_CRITICAL_BLOCK_NUMBER == BlockNumber)
      {
        Fee_CriticalCancel();
      }
      else
      {
        /* Do nothing. It's a regular block. */
      }
#endif
      /* is the module initialized? */
      if (FEE_UNINIT == Fee_State)
      {
        /* !LINKSTO SWS_Fee_00174,1 */
        /* Job is rejected. */
#if (FEE_DEV_ERROR_DETECT == STD_ON)
        FEE_DET_REPORT_ERROR(FEE_WRITE_API_ID, FEE_E_UNINIT);
#endif
      }
      /* is the module busy? */
      else if(FEE_NO_JOB != Fee_Gv.FeeJob)
      {

#if (FEE_DEV_ERROR_DETECT == STD_ON)
        FEE_DET_REPORT_ERROR(FEE_WRITE_API_ID, FEE_E_BUSY);
#endif
      }
      else
      {
#if (FEE_DEV_ERROR_DETECT == STD_ON)
        /* is the data buffer valid? */
        if (NULL_PTR == DataBufferPtr)
        {
          FEE_DET_REPORT_ERROR(FEE_WRITE_API_ID, FEE_E_INVALID_DATA_PTR);
        }
        else
#endif
        {
          /* Get the block index of requested block */
          IntBlockNumber = Fee_SearchConfigTable(BlockNumber);
#if (FEE_DEV_ERROR_DETECT == STD_ON)
          /* has the block been configured in the BlockCfg Table? */
          if (IntBlockNumber >= FEE_NO_OF_CONFIGURED_BLOCKS)
          {
            FEE_DET_REPORT_ERROR(FEE_WRITE_API_ID, FEE_E_INVALID_BLOCK_NO);
          }
          else
#endif
          {
            /* !LINKSTO SWS_Fee_00025,1 */
            /* Fee_Init API should have been called by now, and no job should be already memorised */
            Fee_Gv.FeeJob = FEE_WRITE_JOB;
            Fee_Gv.FeeIntBlockIndex = IntBlockNumber;
            Fee_Gv.FeeBlockDataWriteBuffPtr = DataBufferPtr;
            Fee_Gv.FeeJobResult = MEMIF_JOB_PENDING;
            ReturnValue = E_OK;
          }
        }
      }
    }
    /* Reset the preemption flag */
    FEE_RESET_PREEMPTION_PROTECTION();
  }

  DBG_FEE_WRITE_EXIT(ReturnValue,BlockNumber,DataBufferPtr);
  return ReturnValue;
}


/*------------------------------------[Fee_Cancel]---------------------------------------*/

FUNC(void, FEE_CODE) Fee_Cancel
(
  void
)
{
  DBG_FEE_CANCEL_ENTRY();

#if (FEE_CRITICAL_BLOCK_NUMBER != 0U)
  /* !LINKSTO Fee.FeeCriticalBlock.Cancel,1 */
  if (FEE_BLOCK_CFG(Fee_Gv.FeeIntBlockIndex)->FeeBlockNumber == FEE_CRITICAL_BLOCK_NUMBER)
  {
    /* Do not accept the job. A write or an erase immediate job was requested for the critical block. */
  }
  else
#endif /* (FEE_CRITICAL_BLOCK_NUMBER != 0U) */
  {
#if (FEE_DEV_ERROR_DETECT == STD_ON)
    /* is the module initialized? */

    if (FEE_UNINIT == Fee_State)
    {
      FEE_DET_REPORT_ERROR(FEE_CANCEL_API_ID, FEE_E_UNINIT);
    }
    else
#endif
    {
      /* Check if the current module status is not MEMIF_BUSY */
      if(FEE_NO_JOB == Fee_Gv.FeeJob)
      {
        /* !LINKSTO Fee.Dsn.Startup1.Cancel.NoRequest,1 */
        /* !LINKSTO Fee.Dsn.Startup2.Cancel.NoRequest,1 */
        /* !LINKSTO Fee.Dsn.Cancel.Behaviour.NoJobPending,1 */
        /* !LINKSTO Fee.Dsn.Cancel.Behaviour.InternalOperations,1 */
        FEE_DET_REPORT_ERROR(FEE_CANCEL_API_ID, FEE_E_INVALID_CANCEL);
      }
      else
      {
        Fee_CancelJob();
        Fee_Gv.FeeJobResult = MEMIF_JOB_CANCELED;
        /* !LINKSTO Fee.Dsn.Cancel.Behaviour.ReportNvm,1 */
        /* Call NVM's error notification function */
        FEE_NVM_JOB_ERROR_NOTIFICATION();
      }
    }
  }

  DBG_FEE_CANCEL_EXIT();
}

/*----------------------------------[Fee_GetStatus]---------------------------------------*/

FUNC(MemIf_StatusType, FEE_CODE) Fee_GetStatus
(
  void
)
{
  MemIf_StatusType RetVal;

  DBG_FEE_GETSTATUS_ENTRY();

  /* is the module initialized? */
  if (FEE_UNINIT == Fee_State)
  {
    /* !LINKSTO Fee.GetStatus.InitCheck,1 */
    FEE_DET_REPORT_ERROR(FEE_GETSTATUS_API_ID, FEE_E_UNINIT);
    RetVal = MEMIF_UNINIT;
  }
  else
  {
    if(Fee_Gv.FeeJob == FEE_NO_JOB)
    {
      if ((Fee_Gv.FeeMode == FEE_MODE_NORMAL)
          ||
          ((Fee_Gv.FeeMode != FEE_MODE_STARTUP1) && (Fee_Gv.FeeDelayTimer != 0U)))
      {
        RetVal = MEMIF_IDLE;
      }
      else
      {
        /* !LINKSTO FEE074,1 */
        /* No job from upper layer, but Fee passed a job to the driver */
        RetVal = MEMIF_BUSY_INTERNAL;
      }
    }
    else
    {
      RetVal = MEMIF_BUSY;
    }
  }


  DBG_FEE_GETSTATUS_EXIT(RetVal);
  return RetVal;
}

/*----------------------------------[Fee_GetJobResult]---------------------------------------*/

FUNC(MemIf_JobResultType, FEE_CODE) Fee_GetJobResult
(
  void
)
{
  MemIf_JobResultType RetVal;

  DBG_FEE_GETJOBRESULT_ENTRY();

#if (FEE_DEV_ERROR_DETECT == STD_ON)
  /* is the module initialized? */
  if (FEE_UNINIT == Fee_State)
  {
    FEE_DET_REPORT_ERROR(FEE_GETJOBRESULT_API_ID, FEE_E_UNINIT);
    RetVal = MEMIF_JOB_FAILED;
  }
  else
#endif
  {
#if (FEE_CRITICAL_BLOCK_NUMBER != 0U)
    const boolean IsCriticalBlockErased = Fee_IsCriticalBlockErased(Fee_Gv.FeeIntBlockIndex);
    /* !LINKSTO Fee.FeeCriticalBlock.GetJobResult,1 */
    /* If the critical block has been erased successfully NvM will see that it's job has failed */
    if(((FEE_BLOCK_CFG(Fee_Gv.FeeIntBlockIndex)->FeeBlockNumber == FEE_CRITICAL_BLOCK_NUMBER) &&
        (FEE_NO_JOB == Fee_Gv.FeeJob)) &&
        (TRUE == IsCriticalBlockErased))
    {
      Fee_Gv.FeeIntBlockIndex = 0U;
      RetVal = Fee_Gv.FeeJobResult;
      Fee_Gv.FeeJobResult = MEMIF_JOB_FAILED;
    }
    else
#endif
    {
      /* Flash get job result is not called, Fee job result is returned;
       Reference: EB Refinement EB_FEE091 */
      RetVal = Fee_Gv.FeeJobResult;
    }
  }


  DBG_FEE_GETJOBRESULT_EXIT(RetVal);
  return RetVal;
}

/*----------------------------------[Fee_InvalidateBlock]---------------------------------------*/

FUNC(Std_ReturnType, FEE_CODE) Fee_InvalidateBlock
(
  uint16 BlockNumber
)
{
  uint16 IntBlockNumber;
  Std_ReturnType ReturnValue = E_NOT_OK;
#if  ((FEE_WRITE_CUSTOM_API == STD_ON) || (FEE_READ_CUSTOM_API == STD_ON))
  boolean PreemptionProtection;
#endif /* ((FEE_WRITE_CUSTOM_API == STD_ON) || (FEE_READ_CUSTOM_API == STD_ON)) */

  DBG_FEE_INVALIDATEBLOCK_ENTRY(BlockNumber);

#if  ((FEE_WRITE_CUSTOM_API == STD_ON) || (FEE_READ_CUSTOM_API == STD_ON))
  PreemptionProtection = Fee_CheckAndSetPreemptionProtection();
  if (PreemptionProtection == TRUE)
  {
    /* Report the preemption Det error */
    /* !LINKSTO Fee.PreemptionProtection.DET,1 */
    FEE_DET_REPORT_ERROR(FEE_INVALIDATEBLOCK_API_ID, FEE_E_PREEMPTION_OCCURED);
  }
  else
#endif /* ((FEE_WRITE_CUSTOM_API == STD_ON) || (FEE_READ_CUSTOM_API == STD_ON)) */
  {
    /* !LINKSTO Fee.FeeCriticalBlock.StopBlockJobs,1 */
#if (FEE_CRITICAL_BLOCK_NUMBER != 0U)
    if (FEE_CRITICAL_FROZEN == Fee_State)
    {
      /* Do not accept the job. */
    }
    else
#endif /* (FEE_CRITICAL_BLOCK_NUMBER != 0U) */
    {
      /* is the module initialized? */
      if (FEE_UNINIT == Fee_State)
      {
        /* !LINKSTO FEE126,1 */
#if (FEE_DEV_ERROR_DETECT == STD_ON)
        FEE_DET_REPORT_ERROR(FEE_INVALIDATEBLOCK_API_ID, FEE_E_UNINIT);
#endif
      }

      /* is the module busy? */
      else if(Fee_Gv.FeeJob != FEE_NO_JOB)
      {
        /* !LINKSTO FEE145,1 */
#if (FEE_DEV_ERROR_DETECT == STD_ON)
        FEE_DET_REPORT_ERROR(FEE_INVALIDATEBLOCK_API_ID, FEE_E_BUSY);
#endif
      }

      else
      {
        /* Get the block index of requested block */
        IntBlockNumber = Fee_SearchConfigTable(BlockNumber);

#if (FEE_DEV_ERROR_DETECT == STD_ON)
        /* Check whether the block has been configured in the BlockCfg Table */
        if (IntBlockNumber >= FEE_NO_OF_CONFIGURED_BLOCKS)
        {
          FEE_DET_REPORT_ERROR(FEE_INVALIDATEBLOCK_API_ID, FEE_E_INVALID_BLOCK_NO);
        }

        else
#endif
        {
          /* !LINKSTO SWS_Fee_00192,1 */
          Fee_Gv.FeeIntBlockIndex = IntBlockNumber;
          Fee_Gv.FeeJob = FEE_INVALIDATE_JOB;
          Fee_Gv.FeeJobResult = MEMIF_JOB_PENDING;
          ReturnValue = E_OK;
        }
      }
    }
    /* Reset the preemption flag */
    FEE_RESET_PREEMPTION_PROTECTION();
  }

  DBG_FEE_INVALIDATEBLOCK_EXIT(ReturnValue,BlockNumber);
  return ReturnValue;
}

/*----------------------------------[Fee_GetVersionInfo]---------------------------------------*/

#if (FEE_VERSION_INFO_API == STD_ON)

FUNC(void, FEE_CODE)Fee_GetVersionInfo
(P2VAR(Std_VersionInfoType, AUTOMATIC, FEE_APPL_DATA)VersionInfoPtr)
{
  DBG_FEE_GETVERSIONINFO_ENTRY(VersionInfoPtr);
#if (FEE_DEV_ERROR_DETECT == STD_ON)
  /* is the version pointer valid? */

  if(NULL_PTR == VersionInfoPtr)
  {
    FEE_DET_REPORT_ERROR(FEE_GET_VERSION_INFO_API_ID, FEE_E_INVALID_DATA_PTR);
  }

  else
#endif
  {
    VersionInfoPtr->vendorID = FEE_VENDOR_ID;
    VersionInfoPtr->moduleID = FEE_MODULE_ID;
    VersionInfoPtr->sw_major_version = FEE_SW_MAJOR_VERSION;
    VersionInfoPtr->sw_minor_version = FEE_SW_MINOR_VERSION;
    VersionInfoPtr->sw_patch_version = FEE_SW_PATCH_VERSION;
  }

  DBG_FEE_GETVERSIONINFO_EXIT(VersionInfoPtr);
}

#endif

/*----------------------------------[Fee_EraseImmediateBlock]----------------------------------*/

FUNC(Std_ReturnType, FEE_CODE) Fee_EraseImmediateBlock
(
  uint16 BlockNumber
)
{
  uint16 IntBlockNumber;
  Std_ReturnType ReturnValue = E_NOT_OK;
#if  ((FEE_WRITE_CUSTOM_API == STD_ON) || (FEE_READ_CUSTOM_API == STD_ON))
  boolean PreemptionProtection;
#endif /* ((FEE_WRITE_CUSTOM_API == STD_ON) || (FEE_READ_CUSTOM_API == STD_ON)) */

  DBG_FEE_ERASEIMMEDIATEBLOCK_ENTRY(BlockNumber);

#if  ((FEE_WRITE_CUSTOM_API == STD_ON) || (FEE_READ_CUSTOM_API == STD_ON))
  PreemptionProtection = Fee_CheckAndSetPreemptionProtection();
  if (PreemptionProtection == TRUE)
  {
    /* Report the preemption Det error */
    /* !LINKSTO Fee.PreemptionProtection.DET,1 */
    FEE_DET_REPORT_ERROR(FEE_ERASEIMMEDIATEBLOCK_API_ID, FEE_E_PREEMPTION_OCCURED);
  }
  else
#endif /* ((FEE_WRITE_CUSTOM_API == STD_ON) || (FEE_READ_CUSTOM_API == STD_ON)) */
  {
    /* !LINKSTO Fee.FeeCriticalBlock.StopBlockJobs,1 */
    /* !LINKSTO Fee.FeeCriticalBlock.WriteFreeze,1 */
#if (FEE_CRITICAL_BLOCK_NUMBER != 0U)
    if ((FEE_CRITICAL_FROZEN == Fee_State) && (FEE_CRITICAL_BLOCK_NUMBER != BlockNumber))
    {
      /* Do not accept the job. */
    }
    else
#endif /* (FEE_CRITICAL_BLOCK_NUMBER != 0U) */
    {
      /* is the module initialized? */
      if (FEE_UNINIT == Fee_State)
      {
        /* !LINKSTO FEE127,1 */
#if (FEE_DEV_ERROR_DETECT == STD_ON)
        FEE_DET_REPORT_ERROR(FEE_ERASEIMMEDIATEBLOCK_API_ID, FEE_E_UNINIT);
#endif
      }

      /* is the module busy? */
      else if(Fee_Gv.FeeJob != FEE_NO_JOB)
      {
        /* !LINKSTO FEE146,1 */
#if (FEE_DEV_ERROR_DETECT == STD_ON)
        FEE_DET_REPORT_ERROR(FEE_ERASEIMMEDIATEBLOCK_API_ID, FEE_E_BUSY);
#endif
      }

      else
      {
        /* Get the block index of requested block */
        IntBlockNumber = Fee_SearchConfigTable(BlockNumber);

#if (FEE_DEV_ERROR_DETECT == STD_ON)
        /* Check whether the block has been configured in the BlockCfg Table and
         * that the block is an immediate block */
        if ((IntBlockNumber >= FEE_NO_OF_CONFIGURED_BLOCKS) ||
            (FALSE == Fee_BlockCfg[IntBlockNumber].FeeImmediateData))
        {
          FEE_DET_REPORT_ERROR(FEE_ERASEIMMEDIATEBLOCK_API_ID, FEE_E_INVALID_BLOCK_NO);
        }
        else
#endif
        {
          ReturnValue = E_OK;
          Fee_Gv.FeeIntBlockIndex = IntBlockNumber;
          Fee_Gv.FeeJob = FEE_ERASE_IMMEDIATE_JOB;
          Fee_Gv.FeeJobResult = MEMIF_JOB_PENDING;
        }
      }
    }
    /* Reset the preemption flag */
    FEE_RESET_PREEMPTION_PROTECTION();
  }

  DBG_FEE_ERASEIMMEDIATEBLOCK_EXIT(ReturnValue,BlockNumber);
  return ReturnValue;
}


#if FEE_POLLING_MODE == STD_OFF

/*----------------------------------[Fee_JobEndNotification]-----------------------------------*/

FUNC(void,FEE_CODE)Fee_JobEndNotification(void)
{

  DBG_FEE_JOBENDNOTIFICATION_ENTRY();
  /* !LINKSTO Fee.JobNotification.InitCheck,1 */
  if (Fee_State != FEE_UNINIT)
  {
    /* The last flash job was success */
    if(MEMIF_JOB_PENDING == Fee_Gv.FeeFlashJobResult)
    {
      Fee_Gv.FeeFlashJobResult = MEMIF_JOB_OK;
    }
  }

  DBG_FEE_JOBENDNOTIFICATION_EXIT();
}
/*----------------------------------[Fee_JobErrorNotification]--------------------------------*/

FUNC(void,FEE_CODE)Fee_JobErrorNotification(void)
{

  DBG_FEE_JOBERRORNOTIFICATION_ENTRY();
  /* !LINKSTO Fee.JobNotification.InitCheck,1 */
  if (Fee_State != FEE_UNINIT)
  {
    /* The last flash job was failure */
    if( MEMIF_JOB_PENDING == Fee_Gv.FeeFlashJobResult)
    {
      Fee_Gv.FeeFlashJobResult = MEMIF_JOB_FAILED;
    }
  }

  DBG_FEE_JOBERRORNOTIFICATION_EXIT();
}

#endif

/*----------------------------------[Fee_MainFunction]---------------------------------------*/
/* !LINKSTO FEE097,1 */
FUNC(void, FEE_CODE) Fee_MainFunction(void)
{
/* !LINKSTO Fee.MainFunction.InitCheck,1 */
  DBG_FEE_MAINFUNCTION_ENTRY();
#if (FEE_CANCEL_SECTION_ERASE == STD_ON)
  FEE_SET_FLAG(FEE_MAIN_FUNCTION_CALL_MASK);
#endif
#if (FEE_FREEZE_ACTIVITIES_API == STD_ON)
  if(Fee_Gv.FreezeActivities == 0)
#endif
  {
#if (FEE_CRITICAL_BLOCK_NUMBER != 0U)
    Fee_CriticalBlockHandle();
#endif
    /* !LINKSTO FEE057,1 */
    /* call the state function if available */
    if ((Fee_State != FEE_STATE_INVALID)
        && (Fee_States[Fee_State].fn != NULL_PTR))
    {
      Fee_States[Fee_State].fn();
    }
    /* enter the next requested state */
    while (Fee_NextState != FEE_STATE_INVALID)
    {
      DBG_FEE_STATE((Fee_State),(Fee_NextState));
      Fee_State = Fee_NextState;

      DBG_FEE_NEXTSTATE((Fee_NextState),(FEE_STATE_INVALID));
      Fee_NextState = FEE_STATE_INVALID;
      if (NULL_PTR != Fee_States[Fee_State].onEntry)
      {
        Fee_States[Fee_State].onEntry();
      }
    }
  }
#if (FEE_CANCEL_SECTION_ERASE == STD_ON)
  FEE_RESET_FLAG(FEE_MAIN_FUNCTION_CALL_MASK);
#endif
  DBG_FEE_MAINFUNCTION_EXIT();
}

#define FEE_STOP_SEC_CODE
#include <MemMap.h>
/*==================[internal function definitions]=========================*/

/*==================[end of file]=================================*/

