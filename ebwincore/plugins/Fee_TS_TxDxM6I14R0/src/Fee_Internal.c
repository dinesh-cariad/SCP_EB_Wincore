/**
 * \file
 *
 * \brief AUTOSAR Fee
 *
 * This file contains the implementation of the AUTOSAR
 * module Fee.
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2017 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
/* This file contains code for internal functions used by Fee APIs from AUTOSAR_SWS_FlashEEPROMEmulation.pdf */

/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true violated
 *
 * Reason: false positive, see tasking issue TCVX-41885
 *
 */

/*==================[inclusions]============================================*/

#include <Fee_Internal.h>     /* Internal function prototypes and internal defines and declarations*/
#include <Fee.h>              /* External API declarations  */
#include <TSMem.h>            /* Memory Routines */
#if (FEE_PROD_ERROR_DETECT == STD_ON)
/* !LINKSTO FEE060,1 */
#include <Dem.h>              /* General Dem header file */
#endif
#if ((FEE_WRITE_CUSTOM_API == STD_ON) || (FEE_READ_CUSTOM_API == STD_ON))
#include <SchM_Fee.h>             /* module specific header for             */
#endif

/*==================[macros]=================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define FEE_START_SEC_CODE
#include <MemMap.h>

/** \brief Function to return whether the block is invalidated or not */
STATIC FUNC(boolean, FEE_CODE) Fee_IsInvalidBlockAddress
(
    uint16 BlockIndex
);

/** \brief Function to return whether the block is erased or not */
STATIC FUNC(boolean, FEE_CODE) Fee_IsInconsistentBlockAddress
(
    uint16 BlockIndex
);

/** \brief Function to start the processing of invalidate/write job */
INLINE STATIC FUNC(void, FEE_CODE) Fee_ProcessWriteInvalidateOrEraseJob(void);

/** \brief Function to initiate Fee read  */
STATIC FUNC(void, FEE_CODE) Fee_InitiateRead(void);

/** \brief Function to check if sufficient space available to write in the
 *         current active section
 *
 * \revtal TRUE free space is available to write.
 * \retval FALSE free space is not available to write.
 */
STATIC FUNC(boolean, FEE_CODE) Fee_ChkFreeSpace(void);

/** \brief Function to check space availability to copy next block during switch
 *
 * \revtal TRUE free space is available to copy.
 * \retval FALSE free space is not available to copy.
 */
STATIC FUNC(boolean, FEE_CODE) Fee_ChkFreeSpaceForCopy(void);

/** \brief Function to trigger the switch restart.
 */
STATIC FUNC(void, FEE_CODE) Fee_RestartSwitch(void);

#if (FEE_CRITICAL_BLOCK_NUMBER != 0U)
/** \brief Function to initiate Fee critical block  */
STATIC FUNC(void, FEE_CODE) Fee_InitiateCriticalWrite(void);
/** \brief Function finish a job for critical block  */
STATIC FUNC(void, FEE_CODE) Fee_CriticalJobFinish(MemIf_JobResultType CriticalJobResult);
#endif /* (FEE_CRITICAL_BLOCK_NUMBER != 0U) */

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

/*--------------------------[Fee_SfTriggerJobOrInternal]--------------------------------*/

FUNC(void, FEE_CODE) Fee_SfTriggerJobOrInternal
(
  void
)
{
  if (Fee_Gv.FeeJob != FEE_NO_JOB)
  {
    /* Fee_Gv.FeeJob will be updated when a job request is posted
     * Initiate the requested job */
     /* !LINKSTO Fee.Dsn.Read.Behaviour.SSOngoing,1 */
    Fee_InitiateJob();
  }
  else
  {
    Fee_Gv.TriggerFunct();
  }
}

/*--------------------------[Fee_SfIdleState]--------------------------------*/

FUNC(void, FEE_CODE) Fee_SfIdleState
(
  void
)
{
  /* Get the flash job result */
  FEE_GET_FLASH_JOB_RESULT();

  /* Flash failure need not be handled in idle state;
   * The flash job completed in idle state will always be a cancel job
   * in this case the job result will be already updated as job cancelled */

  /* !LINKSTO Fee.Initializaion.UserJobRequests,1 */
  /* !LINKSTO Fee.SS.UserJobRequests,1 */
  if (Fee_Gv.FeeJob != FEE_NO_JOB)
  {
    /* Fee_Gv.FeeJob will be updated when a job request is posted
     * Initiate the requested job */
    Fee_InitiateJob();
  }
  /* Fee still has to do something internally */
  else if (Fee_Gv.FeeMode != FEE_MODE_NORMAL)
  {
    if (Fee_Gv.FeeDelayTimer == FEE_INFINITE_DELAY)
    {
      /* !LINKSTO Fee.Dsn.Initialization.Idle,1 */
      /* Do nothing. */
    }
    /* Check whether delay is not completed */
    /* !LINKSTO Fee.Dsn.Timer.UserJobRequests,1 */
    /* !LINKSTO Fee.Dsn.Timer.InternalOperation,1 */
    else if (Fee_Gv.FeeDelayTimer != 0U)
    {
      Fee_Gv.FeeDelayTimer--;
    }
    else
    {
      switch(Fee_Gv.FeeMode)
      {
      case FEE_MODE_STARTUP2:
        /* Do the part 2 of initialization */
        Fee_StartUp2();
        break;

      case FEE_MODE_SWITCH:
        /* As the delay expired, start the pending switch section */
        Fee_SectionSwitching();
        break;

        /* CHECK: NOPARSE */
      default:
        FEE_UNREACHABLE_CODE_ASSERT(FEE_INTERNAL_API_ID);
        break; /* Caution: this 'break' is required by MISRA-C:2004 */
        /* CHECK: PARSE */
      }
    }
  }
  else
  {
    /* Nothing to do. Wait for a job. */
  }
}

/*--------------------------[Fee_SfWaitACycle]--------------------------------*/

FUNC(void, FEE_CODE) Fee_SfWaitACycle
(
  void
)
{
  /* Check if a retry is needed */
  if(Fee_Gv.FeeRetryState != FEE_STATE_INVALID)
  {
    FEE_TRIGGER_TRANS(Fee_Gv.FeeRetryState);
    /* Reset the retry state */
    Fee_Gv.FeeRetryState = FEE_STATE_INVALID;
  }
  else
  {
    /* Trigger the needed function */
    Fee_Gv.TriggerFunct();
    /* Reset the pointer */
    Fee_Gv.TriggerFunct = NULL_PTR;
  }
}

#if (FEE_CRITICAL_BLOCK_NUMBER != 0U)
/*--------------------------[Fee_SfFrozenCritical]--------------------------------*/

FUNC(void, FEE_CODE) Fee_SfFrozenCritical
(
  void
)
{
  /* !LINKSTO Fee.FeeCriticalBlock.WriteFreeze,1 */
  if (Fee_Gv.FeeJob == FEE_ERASE_IMMEDIATE_JOB)
  {
    /* !LINKSTO Fee.FeeCriticalBlock.HealInternaly,1 */
    /* Fee_Gv.FeeJob == FEE_ERASE_IMMEDIATE_JOB. Unfreeze! */
    if (FEE_STATE_INVALID != Fee_Gv.FeeCriticalState)
    {
      Fee_State = Fee_Gv.FeeCriticalState;
#if (FEE_POLLING_MODE != STD_ON)
      /* !LINKSTO Fee.Dsn.Critical.ResumeFailed,1 */
      Fee_Gv.FeeCriticalState = FEE_STATE_INVALID;
      Fee_Gv.FeeFlashJobResult = MEMIF_JOB_FAILED;
#endif /* (FEE_POLING_MODE != STD_ON) */
    }
    else
    {
      /* The block was found at initialization. No state was interrupted at current runtime.
       * Initiate the erase immediate! */
      Fee_InitiateJob();
    }
  }
  else if(Fee_Gv.FeeJob == FEE_READ_JOB)
  {
    FEE_TRIGGER_TRANS(FEE_READ_BLOCKDATA);
  }
  else
  {
    /* Do nothing. */
  }
}
#endif /* (FEE_CRITICAL_BLOCK_NUMBER != 0U) */

/*--------------------------[Fee_MarkSectionActive]-------------------------*/

FUNC(void, FEE_CODE) Fee_MarkSectionActive
(
  uint8 ActiveSectionIndex
)
{
  Fee_Gv.FeeActiveSectionIndex = ActiveSectionIndex;
  /* Load next data write address */
  Fee_Gv.FeeDataAddress = FEE_DATA_ADDRESS(Fee_Gv.FeeActiveSectionIndex);
  /* Load next info write address */
  Fee_Gv.FeeInfoAddress = FEE_FIRST_BLOCK_INFO_ADDRESS(Fee_Gv.FeeActiveSectionIndex);
  FEE_TRIGGER_TRANS(FEE_MARK_HEADER_ACTIVE);
}

/*--------------------------[Fee_StartUp2]-------------------------*/

FUNC(void, FEE_CODE) Fee_StartUp2
(
  void
)
{
  boolean ExitFlag;
  /*Delay timer set to 0*/
  Fee_Gv.FeeDelayTimer = 0U;
  /* The working section should be the first section searched for blocks.
   * (usually the Active section). Meanwhile the active section can change. */

  do{
    ExitFlag = TRUE;

    switch(Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeWorkingSectionIndex].SectionStatus)
    {
      /* !LINKSTO Fee.Dsn.Initialization.Startup2.01,2 */
      /* !LINKSTO Fee.Dsn.Initialization.Startup2.02,1 */
      /* !LINKSTO Fee.Initialization.EmptyFlash,1 */
      case FEE_SECTION_INCONSISTENT:
      case FEE_SECTION_ERASABLE:
      case FEE_SECTION_ERASED:
      /* !LINKSTO Fee.Dsn.Initialization.Startup2.06,1 */
        FEE_TRIGGER_TRANS(FEE_ERASE_SECTION);
        break;

      case FEE_SECTION_EMPTY:
        if(Fee_Gv.FeeWorkingSectionIndex == Fee_Gv.FeeActiveSectionIndex)
        {
          /* !LINKSTO Fee.Dsn.Initialization.Startup2.03,1 */
          Fee_MarkSectionActive(Fee_Gv.FeeActiveSectionIndex);
          /* Marking the section as Active is the last thing that Startup2 will do to this section. */
          Fee_Gv.FeeWorkingSectionIndex = FEE_SECTIONINCREMENT(Fee_Gv.FeeWorkingSectionIndex);
        }
        else
        {
          Fee_Gv.FeeWorkingSectionIndex = FEE_SECTIONINCREMENT(Fee_Gv.FeeWorkingSectionIndex);
          ExitFlag = FALSE;
        }
        break;

      case FEE_SECTION_NO_FOOTER:
        /* !LINKSTO Fee.Dsn.Initialization.Startup2.04,1 */
        FEE_TRIGGER_TRANS(FEE_WRITE_SECTION_FOOTER);
        break;

      case FEE_SECTION_ERASABLE_HEADER:
        /* !LINKSTO Fee.Dsn.Initialization.Startup2.05,1 */
        FEE_TRIGGER_TRANS(FEE_MARK_FOOTER_ERASABLE);
        break;

      case FEE_SECTION_ACTIVE:
        if(Fee_Gv.FeeWorkingSectionIndex != Fee_Gv.FeeOldestSectionIndex)
        {
          Fee_Gv.FeeWorkingSectionIndex = FEE_SECTIONINCREMENT(Fee_Gv.FeeWorkingSectionIndex);
          ExitFlag = FALSE;
        }
        else
        {
          /* Fee reached the oldest active section. StartUp2 is over. */
          /* !LINKSTO Fee.Dsn.Initialization.Startup2.07,1 */
          if((FEE_SECTIONDIF(Fee_Gv.FeeActiveSectionIndex, Fee_Gv.FeeOldestSectionIndex)) >= ((uint8)FEE_NUMBER_OF_SECTIONS / (uint8)2U))
          {
            /* !LINKSTO Fee.Dns.SS.TriggeringTheSwitch.AfterInit,1 */
            Fee_Gv.FeeMode = FEE_MODE_SWITCH;
            Fee_Gv.FeeDelayTimer = 0U;
            Fee_Gv.TriggerFunct = &Fee_SectionSwitching;
            FEE_TRIGGER_TRANS(FEE_TRIGGER_JOB_OR_INTERNAL);
          }
          else
          {
            /* !LINKSTO Fee.Dsn.Initialization.Startup2.09,1 */
            Fee_Gv.FeeMode = FEE_MODE_NORMAL;
            FEE_TRIGGER_TRANS(FEE_IDLE);
          }
        }
        break;

      /* CHECK: NOPARSE */
      default:
        FEE_UNREACHABLE_CODE_ASSERT(FEE_INTERNAL_API_ID);
        break; /* Caution: this 'break' is required by MISRA-C:2004 */
        /* CHECK: PARSE */
    }
  }while(ExitFlag != TRUE);
}

/*--------------------------[Fee_SectionSwitching]-------------------------*/

FUNC(void, FEE_CODE) Fee_SectionSwitching
(
  void
)
{
#if (FEE_NUMBER_NOT_CONFIGURED_BLOCKS > 0U)
  uint16 NbOfNotCfgBlocksFound = Fee_Gv.FeeNbOfNotCfgBlocksFound;
#else
  uint16 NbOfNotCfgBlocksFound = 0U;
#endif
  if(Fee_Gv.FeeSwitchState != FEE_STATE_INVALID)
  {
    /* !LINKSTO Fee.Dns.SS.Behaviour.SSResume.BlockSwitching.Writing,1 */
    /* !LINKSTO Fee.Dns.SS.Behaviour.SSResume.BlockSwitching.Reading,1 */
    /* !LINKSTO Fee.Dns.SS.Behaviour.SSResume.BlockSwitching.SectionErase,1 */
    FEE_TRIGGER_TRANS(Fee_Gv.FeeSwitchState);
  }
  else
  {
    /* Get the next block that can be copied.
     * The next block to be copied must be in inactive section, consistent or invalid
     * and there must be enough space to copy the block. */
    while (Fee_Gv.FeeCopyBlockIndex < (FEE_NO_OF_CONFIGURED_BLOCKS + NbOfNotCfgBlocksFound))
    {
      /* !LINKSTO Fee.Dns.SS.CopyBlocks,1 */
      /* Check if the block is inconsistent or the block is not present in the oldest section */
      if ((Fee_IsInconsistentBlockAddress(Fee_Gv.FeeCopyBlockIndex)) ||
          (!((Fee_Gv.FeeCache[Fee_Gv.FeeCopyBlockIndex] >=
                FEE_DATA_ADDRESS(Fee_Gv.FeeOldestSectionIndex)
              )
              &&
              (Fee_Gv.FeeCache[Fee_Gv.FeeCopyBlockIndex] <
                 FEE_SECTION_END_ADDRESS(Fee_Gv.FeeOldestSectionIndex)
              )
            )
          )
         )
      {
        /* No need to copy current block. Look for next block which needs to be copied */
        Fee_Gv.FeeCopyBlockIndex++;
      }
      /* If block is consistent or invalid, and in the oldest section section */
      else
      {
        /* Block to be copied found. */
        break;
      }
    }

    /* No more blocks remaining to be copied */
    if (Fee_Gv.FeeCopyBlockIndex >= (FEE_NO_OF_CONFIGURED_BLOCKS + NbOfNotCfgBlocksFound))
    {
      /* !LINKSTO Fee.Dns.SS.RevalidateOldSection,1 */
      /* Blocks copied. Reinitialize the copy block index for the next switch */
      Fee_Gv.FeeCopyBlockIndex = 0U;
      /* !LINKSTO Fee.Dns.SS.RestartSwitch.Priority,1 */
      FEE_RESET_FLAG(FEE_HIGHEST_INTERNAL_PRIORITY_MASK);
      Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeOldestSectionIndex].SectionStatus = FEE_SECTION_ERASABLE;
      /* The oldest section has to be erased now. */
      Fee_Gv.FeeWorkingSectionIndex = Fee_Gv.FeeOldestSectionIndex;
      FEE_TRIGGER_TRANS(FEE_MARK_HEADER_ERASABLE);
      /* A switch has finished. Update the oldest index. */
      Fee_Gv.FeeOldestSectionIndex = FEE_SECTIONINCREMENT(Fee_Gv.FeeOldestSectionIndex);
    }
    /* More blocks are yet to be copied */
    else
    {
      Fee_BlockCfgPtr = FEE_BLOCK_CFG(Fee_Gv.FeeCopyBlockIndex);

      if(Fee_IsInvalidBlockAddress(Fee_Gv.FeeCopyBlockIndex))
      {
        Fee_Gv.FeeSwitchJob = FEE_INVALIDATE_JOB;
      }
      else
      {
        Fee_Gv.FeeSwitchJob = FEE_WRITE_JOB;
      }

      /* Check whether space is available to copy current block */
      if (TRUE == Fee_ChkFreeSpaceForCopy())
      {
        /* Initialize the address to copy from */
        Fee_Gv.FeeSwitchSourceAddress = Fee_Gv.FeeCache[Fee_Gv.FeeCopyBlockIndex];
        Fee_Gv.FeeSwitchBlockRemSize = FEE_COPY_BLOCK_SIZE();
        Fee_Gv.FeeWorkingInfoAddress = Fee_Gv.FeeInfoAddress;
        Fee_Gv.FeeWorkingDataAddress = Fee_Gv.FeeDataAddress;

#if (FEE_CONSISTENCY_START_PATTERN_ENABLED == STD_OFF)
          FEE_TRIGGER_TRANS(FEE_SS_COPY_INFO);
#else
          /* !LINKSTO Fee.ConsistencyStartPattern.BlockManagementInformation.Write,1 */
          /* Write the block info start pattern */
          FEE_TRIGGER_TRANS(FEE_SS_WRITE_BLOCK_INFO_START_PATTERN);
#endif
        if(FEE_CHECK_FLAG(FEE_HIGHEST_INTERNAL_PRIORITY_MASK))
        {
          /* Used for restart switch */
          Fee_Gv.FeeSwRestartCache[Fee_Gv.FeeCopyBlockIndex] = Fee_Gv.FeeCache[Fee_Gv.FeeCopyBlockIndex];
        }
        else
        {
          /* Do nothing regarding restart switch, because Fee is in normal switch. */
        }
      }
      else
      {
#if (FEE_NUMBER_OF_SECTIONS > 2U)
        if(FEE_CHECK_FLAG(FEE_HIGHEST_INTERNAL_PRIORITY_MASK))
        {
#endif
          /* !LINKSTO Fee.Dns.SS.RestartSwitch.ConditionForRestart,1 */
          /* Restart switch */
          Fee_RestartSwitch();
#if (FEE_NUMBER_OF_SECTIONS > 2U)
        }
        else
        {
          if(FEE_SECTIONSUM(Fee_Gv.FeeActiveSectionIndex,(uint8)2) == Fee_Gv.FeeOldestSectionIndex)
          {
            FEE_SET_FLAG(FEE_HIGHEST_INTERNAL_PRIORITY_MASK);
          }
          else
          {
            /* Do nothing */
          }
          /* !LINKSTO Fee.Dns.SS.Behaviour.ChangingActiveSection,1 */
          /* Perform the swap */
          Fee_MarkSectionActive(FEE_SECTIONINCREMENT(Fee_Gv.FeeActiveSectionIndex));
        }
#endif
      }
    }
  }
}


/*--------------------------[Fee_CalculateSSRequiredSpace]-------------------------*/

FUNC(Fls_LengthType, FEE_CODE) Fee_CalculateSSRequiredSpace
(
  void
)
{
  Fls_LengthType SpaceRequired = 0U;
  uint16 IntBlockIdx;
  const uint16 TotalNumberOfBlocks = FEE_NO_OF_CONFIGURED_BLOCKS
#if (FEE_NUMBER_NOT_CONFIGURED_BLOCKS > 0U)
                                     + Fee_Gv.FeeNbOfNotCfgBlocksFound
#endif
                                     ;

  for (IntBlockIdx = 0; IntBlockIdx < TotalNumberOfBlocks; IntBlockIdx++)
  {
    /* If the block is in the oldest section then consider the block */
    if (((Fee_Gv.FeeCache[IntBlockIdx] >= FEE_DATA_ADDRESS(Fee_Gv.FeeOldestSectionIndex)) &&
        (Fee_Gv.FeeCache[IntBlockIdx] < FEE_SECTION_END_ADDRESS(Fee_Gv.FeeOldestSectionIndex))))
    {
      if(Fee_Gv.FeeCache[IntBlockIdx] == FEE_FOOTER_ADDRESS(Fee_Gv.FeeOldestSectionIndex))
      {
        SpaceRequired += (Fls_LengthType)FEE_BLOCKINFO_PLUS_WM_SIZE_AL;
      }
      else
      {
        SpaceRequired +=
#if (FEE_DYNAMIC_BLOCK_LENGTH == STD_ON)
#if (FEE_NUMBER_NOT_CONFIGURED_BLOCKS > 0U)
                        (((IntBlockIdx) >= FEE_NO_OF_CONFIGURED_BLOCKS) ?
                        (Fls_LengthType)(FEE_BLOCK_CFG(IntBlockIdx)->FeeBlockAlignedSize) :
                        (Fls_LengthType)FEE_ALLIGNED_TO_VIRTUAL_PAGE(Fee_Gv.FeeOldSizes[IntBlockIdx])
#else
                        ((Fls_LengthType)(FEE_ALLIGNED_TO_VIRTUAL_PAGE(Fee_Gv.FeeOldSizes[IntBlockIdx]))
#endif /* #if (FEE_NUMBER_NOT_CONFIGURED_BLOCKS > 0U) */
#else
                        ((Fls_LengthType)FEE_BLOCK_CFG(IntBlockIdx)->FeeBlockAlignedSize
#endif /* #if (FEE_DYNAMIC_BLOCK_LENGTH != STD_OFF) */
                         + (Fls_LengthType)FEE_BLOCKINFO_PLUS_WM_SIZE_AL)
#if (FEE_CONSISTENCY_START_PATTERN_ENABLED == STD_ON)
                         /* Data start pattern */
                         + (Fls_LengthType)FEE_CONSISTENCY_PATTERN_SIZE
#endif /* #if (FEE_CONSISTENCY_START_PATTERN_ENABLED == STD_ON) */
                         ;
      }
    }
    else
    {
#if ((FEE_CRITICAL_BLOCK_NUMBER != 0U) && (FEE_NUMBER_OF_SECTIONS == 2U))
    /* !LINKSTO Fee.Dsn.Critical.SwitchRequiredSize,1 */
      if (FEE_BLOCK_CFG(IntBlockIdx)->FeeBlockNumber == FEE_CRITICAL_BLOCK_NUMBER)
      {
        /* In case of highest priority switch, there must also be taken into consideration a possible critical write request
         * in critical section if this section coincide with the oldest section where Fee copies from */
        SpaceRequired += (Fls_LengthType)FEE_CRITICAL_BLOCK_SIZE + (Fls_LengthType)FEE_BLOCKINFO_PLUS_WM_SIZE_AL + (Fls_LengthType)FEE_CONSISTENCY_PATTERN_SIZE;
      }
      else
      {
        /* The block is not the critical one. */
      }
#endif /* (FEE_CRITICAL_BLOCK_NUMBER != 0U) */
    }
  }

  return SpaceRequired;
}

/*--------------------------[Fee_CheckBlockInfoErased]-----------------------*/

FUNC (boolean, FEE_CODE) Fee_CheckBlockInfoErased
(
  const uint16 BufferIndex
)
{
  uint8 Idx;
  boolean Flag = TRUE; /* if all bytes are erased */

  for (Idx = 0U; Idx < FEE_BLOCKINFO_ALIGNED_TOTAL_SIZE; Idx++)
  {
    /* Check whether the byte is in erased state */
    if (FEE_FLASH_ERASE_VALUE != Fee_Gv.FeeBuffPtr[BufferIndex + Idx])
    {
      Flag = FALSE;
      break;
    }
  }

  return Flag;
}


/*--------------------------[Fee_FlsJobRefused]--------------------------------*/

FUNC(void, FEE_CODE) Fee_FlsJobRefused
(
  void
)
{
  MemIf_StatusType FlsStatus = Fls_GetStatus();
  /* For reasons other then Fls being busy consider the job as failed and perform not retries */
  if ((MEMIF_BUSY == FlsStatus) && (Fee_Gv.FeeRetryStateCounter != 0U))
  {
    /* !LINKSTO Fee.Dsn.FlsRefusal,1 */
    Fee_Gv.FeeRetryState = Fee_State;
    FEE_TRIGGER_TRANS(FEE_STATE_WAIT_A_CYCLE);
    Fee_Gv.FeeRetryStateCounter--;
  }
  else
  {
  /* !LINKSTO Fee.Dsn.FlsRefusal.Failed.Det,1 */
  /* Header read request was rejected by Fls, report a development error */
  FEE_DET_REPORT_ERROR(FEE_MAINFUNCTION_API_ID, FEE_E_FLS_REQUEST_REFUSED);

    /* !LINKSTO Fee.Dsn.FlsRefusal.Failed,1 */
    switch (Fee_State)
      {
        case FEE_WRITE_BLOCK_DATA:
        case FEE_READ_BLOCKDATA:
        case FEE_WRITE_BLOCK_INFO:
        case FEE_INVALIDATE:
        case FEE_ERASE_IMMEDIATE:
        case FEE_WRITTEN_MARKER:
#if (FEE_CONSISTENCY_PATTERN_ENABLED == STD_ON)
#if (FEE_CONSISTENCY_START_PATTERN_ENABLED == STD_ON)
        case FEE_WRITE_BLOCK_INFO_START_PATTERN:
        case FEE_WRITE_BLOCK_DATA_START_PATTERN:
        case FEE_READ_BLOCK_DATA_START_PATTERN:
#endif
        case FEE_WRITE_BLOCK_INFO_END_PATTERN:
#endif
        {
          Fee_JobError();
          break;
        }
        /* Else the refusal happened during an internal operation. */
        default:
        {
          FEE_SET_FLAG(FEE_FLASH_FINAL_REFUSAL_MASK);
          break;
        }
      }
    /* Reset retry counter */
    Fee_Gv.FeeRetryStateCounter = FEE_STATE_MAX_RETRIES;
  }
}

/*--------------------------[Fee_FlsJobFailed]--------------------------------*/

FUNC(boolean, FEE_CODE) Fee_FlsJobFailed
(
  void
)
{
  boolean DidTheFailureOccured;
  if(FEE_CHECK_FLAG(FEE_FLASH_FINAL_REFUSAL_MASK))
  {
    FEE_RESET_FLAG(FEE_FLASH_FINAL_REFUSAL_MASK);
    DidTheFailureOccured = FALSE;
  }
  else
  {
    DidTheFailureOccured = TRUE;
#ifdef FEE_E_FLASH_ACCESSIBLE_DEM_EVENT_ID
    /* !LINKSTO Fee.FailureHandling.Behaviour.ProductionError.FEE_E_FLASH_ACCESSIBLE,1 */
    /* Notify user by reporting a production error */
    Dem_ReportErrorStatus(FEE_E_FLASH_ACCESSIBLE_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#endif
  }

  return DidTheFailureOccured;
}

/*--------------------------[Fee_CancelJob]--------------------------------*/

FUNC(void, FEE_CODE) Fee_CancelJob
(
  void
)
{
  switch (Fee_State)
  {
    /* !LINKSTO Fee.Dsn.Startup2.Cancel.HasRequest.Ongoing,1 */
    /* !LINKSTO Fee.Dsn.Normal.Cancel.HasRequest.Ongoing,1 */
    /* !LINKSTO Fee.Dsn.SectonSwitch.Cancel.HasRequest.Ongoing,1 */
    /* !LINKSTO Fee.Dsn.Cancel.Behaviour.ReadJob,1 */
    /* !LINKSTO Fee.Dsn.Cancel.Behaviour.WriteJob.WriteBlockInfo,1 */
    /* !LINKSTO Fee.Dsn.Cancel.Behaviour.WriteJob.WriteWrittenMarker,1 */
    /* !LINKSTO Fee.Dsn.Cancel.Behaviour.InvalidateJob.WriteBlockInfo,1 */
    /* !LINKSTO Fee.Dsn.Cancel.Behaviour.WriteBlockData,1 */
    case FEE_WRITE_BLOCK_DATA:
    case FEE_READ_BLOCKDATA:
    case FEE_WRITE_BLOCK_INFO:
    case FEE_INVALIDATE:
    case FEE_ERASE_IMMEDIATE:
    case FEE_WRITTEN_MARKER:
#if (FEE_CONSISTENCY_PATTERN_ENABLED == STD_ON)
#if (FEE_CONSISTENCY_START_PATTERN_ENABLED == STD_ON)
    /* Writing of the block info start pattern can not be cancelled because the FeeInfoAddress
     * has already been updated. Destructively cancelling this write request might leave
     * the flash memory not written (equal to flash erase value), depending on the availability
     * of the Fls_Cancel() API and the moment Fee_Cancel() is called.
     * During start-up (Fill Cache), if the block info does not contain any data,
     * it will be detected as empty instead of inconsistent.
     */
    /* !LINKSTO Fee.ConsistencyPattern.FeeWrite.BlockManagementInformation.Cancel,1 */
    /* !LINKSTO Fee.ConsistencyPattern.FeeWrite.BlockData.Cancel,1 */
    /* !LINKSTO Fee.ConsistencyPattern.FeeInvalidateBlock.BlockManagementInformation.Cancel,1 */
    /* !LINKSTO Fee.ConsistencyPattern.FeeEraseImmediateblock.BlockManagementInformation.Cancel,1 */
    /* !LINKSTO Fee.ConsistencyStartPattern.FeeWrite.BlockData.Cancel,1 */
    /* !LINKSTO Fee.ConsistencyStartPattern.FeeWrite.BlockManagementInformation.Cancel,1 */
    /* !LINKSTO Fee.ConsistencyStartPattern.FeeInvalidateBlock.BlockManagementInformation.Cancel,1 */
    /* !LINKSTO Fee.ConsistencyStartPattern.FeeEraseImmediateBlock.BlockManagementInformation.Cancel,1 */
    case FEE_WRITE_BLOCK_INFO_START_PATTERN:
    case FEE_WRITE_BLOCK_DATA_START_PATTERN:
    case FEE_READ_BLOCK_DATA_START_PATTERN:
#endif
    case FEE_WRITE_BLOCK_INFO_END_PATTERN:
#endif
    {
      FEE_CANCEL_FLASHJOB();
      Fee_State = FEE_IDLE;
      break;
    }
    /* !LINKSTO Fee.ConsistencyPattern.SSInterrupt,1 */
    /* Else the job is in queue, due to some internal management operations or on written marker state */
    default:
    {
      /* FEE_IDLE */
      break;
    }
  }
  /* !LINKSTO Fee.Dsn.Startup2.Cancel.HasRequest.Pending,1 */
  /* !LINKSTO Fee.Dsn.Normal.Cancel.HasRequest.Pending,1 */
  /* !LINKSTO Fee.Dsn.SectonSwitch.Cancel.HasRequest.Pending,1 */
  /* !LINKSTO Fee.Dsn.Startup1.Cancel.HasRequest,1 */
  /* Unqueue the current job */
  Fee_Gv.FeeJob = FEE_NO_JOB;

  /* If the canceled job changes section clear the flag, since the job is not performed any more. */
  FEE_RESET_FLAG(FEE_USER_JOB_CHANGES_SECTION_MASK);
  /* Reset the retry counter. */
  Fee_Gv.FeeRetryStateCounter = FEE_STATE_MAX_RETRIES;
}

/*--------------------------[Fee_InitiateJob]--------------------------------*/

FUNC(void, FEE_CODE) Fee_InitiateJob
(
  void
)
{
  /* Fee_Gv.FeeJob is updated with the corresponding job
   * from the API, when the job is accepted */
  switch(Fee_Gv.FeeJob)
  {
    case FEE_WRITE_JOB:
        Fee_InitiateStoreRequest();
      break;

    case FEE_READ_JOB:
      Fee_InitiateRead();
      break;

    case FEE_INVALIDATE_JOB:
      if (Fee_IsInvalidBlockAddress(Fee_Gv.FeeIntBlockIndex))
      {
        /* Stop the job processing as the block is found to be invalidated in cache.
         * The state machine shall return to IDLE state.
         */
        Fee_JobEnd();
      }
      else
      {
        Fee_InitiateStoreRequest();
      }
      break;

    case FEE_ERASE_IMMEDIATE_JOB:
      if (Fee_IsInconsistentBlockAddress(Fee_Gv.FeeIntBlockIndex))
      {
        /* Stop the job processing as the block is found to be inconsistent in cache.
         * The state machine shall return to IDLE state.
         */
        Fee_JobEnd();
      }
      else
      {
        Fee_InitiateStoreRequest();
      }
      break;

    case FEE_NO_JOB:
      FEE_TRIGGER_TRANS(FEE_IDLE);
      break;

      /* CHECK: NOPARSE */
    default:
      /* FEE_NO_JOB */
      FEE_UNREACHABLE_CODE_ASSERT(FEE_INTERNAL_API_ID);
      break; /* Caution: this 'break' is required by MISRA-C:2004 */
      /* CHECK: PARSE */
  }
}

/*--------------------------[Fee_InitiateStoreRequest]----------------*/

FUNC(void, FEE_CODE) Fee_InitiateStoreRequest(void)
{
  boolean IsSpaceInSection;

  if(FEE_CHECK_FLAG(FEE_HIGHEST_INTERNAL_PRIORITY_MASK))
  {
    /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.11,1 */
    /* !LINKSTO Fee.Dns.SS.RestartSwitch.TriggerAfterInit,1 */
    Fee_Gv.FeeDelayTimer = 0U;
    Fee_SectionSwitching();
  }
  else
  {
    /* FEE_USER_JOB_CHANGES_SECTION_MASK is set in the same function if the write will be done after the swap */
    if(FEE_CHECK_FLAG(FEE_USER_JOB_CHANGES_SECTION_MASK))
    {
      FEE_RESET_FLAG(FEE_USER_JOB_CHANGES_SECTION_MASK);
      IsSpaceInSection = TRUE;
    }
    else
    {
      if(TRUE == Fee_ChkFreeSpace())
      {
        IsSpaceInSection = TRUE;
      }
      else
      {
        IsSpaceInSection = FALSE;
      }
    }
    if(TRUE == IsSpaceInSection)
    {
      /* !LINKSTO Fee.Dsn.Invalidate.Behaviour.IDLE.IsSpace,1 */
      /* !LINKSTO Fee.Dsn.EraseImmediate.Behaviour.IDLE.IsSpace,1 */
      /* !LINKSTO Fee.Dsn.Write.Behaviour.IDLE.IsSpace,1 */
      /* !LINKSTO Fee.Dsn.Write.Behaviour.SSOngoing.IsSpace,1 */
      /* !LINKSTO Fee.Dsn.EraseImmediate.Behaviour.SSOngoing.IsSpace,1 */
      /* !LINKSTO Fee.Dsn.Invalidate.Behaviour.SSOngoing.IsSpace,1 */
      Fee_ProcessWriteInvalidateOrEraseJob();
    }
    else
    {
      Fee_JobTriggersSectionChange();
    }
  }
}

/*--------------------------[Fee_JobTriggersSectionChange]----------------*/

FUNC(void, FEE_CODE) Fee_JobTriggersSectionChange(void)
{
  if(Fee_Gv.Fee_SectionManagement[FEE_SECTIONINCREMENT(Fee_Gv.FeeActiveSectionIndex)].SectionStatus != FEE_SECTION_EMPTY)
  {
    /* !LINKSTO Fee.Dns.SS.ErasePriority,1 */
    /* !LINKSTO Fee.Dsn.Invalidate.Behaviour.SSOngoing.NoSpace,1 */
    /* !LINKSTO Fee.Dsn.EraseImmediate.Behaviour.SSOngoing.NoSpace,1 */
    /* !LINKSTO Fee.Dns.SS.SwitchPriority,1 */
    /* !LINKSTO Fee.Dsn.Initialization.Startup2.08,1 */
    FEE_SET_FLAG(FEE_HIGHEST_INTERNAL_PRIORITY_MASK);
    if(Fee_Gv.FeeMode == FEE_MODE_STARTUP2)
    {
      Fee_StartUp2();
    }
    else /* FEE_MODE_SWITCH */
    {
      /* The next section is marked as ERASABLE and has to be erased first */
      Fee_SectionSwitching();
    }
  }
  else
  {
#if (FEE_NUMBER_OF_SECTIONS > 2U)
    if(FEE_SECTIONDIF(Fee_Gv.FeeOldestSectionIndex,FEE_SECTIONINCREMENT(Fee_Gv.FeeActiveSectionIndex)) == (uint8)1U)
    {
#endif
#if (FEE_CRITICAL_BLOCK_NUMBER != 0U)
      /* !LINKSTO Fee.Dsn.Critical.CriticalSection,1 */
      /* Save the addresses for critical block */
      Fee_Gv.FeeInfoAddressCritical = Fee_Gv.FeeInfoAddress;
      Fee_Gv.FeeDataAddressCritical = Fee_Gv.FeeDataAddress;
#endif /* (FEE_CRITICAL_BLOCK_NUMBER != 0U) */
      /* !LINKSTO Fee.Dsn.Invalidate.Behaviour.SSOngoing.NoSpace,1 */
      /* !LINKSTO Fee.Dsn.EraseImmediate.Behaviour.SSOngoing.NoSpace,1 */
      /* !LINKSTO Fee.Dns.SS.SwitchPriority,1 */
      /* !LINKSTO Fee.Dns.SS.SwitchPriority.JobLosesPriority,1 */
      FEE_SET_FLAG(FEE_HIGHEST_INTERNAL_PRIORITY_MASK);
      /* For when it gets into switch no suspend directly from StartUp2 */
      Fee_Gv.FeeMode = FEE_MODE_SWITCH;
      Fee_Gv.FeeDelayTimer = 0U;
#if (FEE_NUMBER_OF_SECTIONS > 2U)
    }
    else
    {
      /* !LINKSTO Fee.Dsn.Write.Behaviour.SSOngoing.SwapNeeded,1 */
      /* !LINKSTO Fee.Dsn.Invalidate.Behaviour.SSOngoing.SwapNeeded,1 */
      /* !LINKSTO Fee.Dsn.EraseImmediate.Behaviour.SSOngoing.SwapNeeded,1 */
      if(Fee_Gv.FeeMode == FEE_MODE_NORMAL)
      {
        if(FEE_SECTIONDIF(FEE_SECTIONINCREMENT(Fee_Gv.FeeActiveSectionIndex),
            Fee_Gv.FeeOldestSectionIndex)
            == (FEE_NUMBER_OF_SECTIONS /2U))
        {
          /* !LINKSTO Fee.Dns.SS.TriggeringTheSwitch.WriteRequest,1 */
          /* !LINKSTO Fee.Dns.SS.TriggeringTheSwitch.InvalidateRequest,1 */
          /* !LINKSTO Fee.Dns.SS.TriggeringTheSwitch.EraseRequest,1 */
          Fee_Gv.FeeMode = FEE_MODE_SWITCH;
        }
        else
        {
          /* Do nothing. Just change the section. */
        }
      }
      else /* FEE_MODE_SWITCH  or  FEE_MODE_STARTUP2 */
      {
        /* Do nothing */
      }
      /* The write is prioritized and done after the swap */
      FEE_SET_FLAG(FEE_USER_JOB_CHANGES_SECTION_MASK);
    }
#endif
    /* !LINKSTO Fee.Dsn.Invalidate.Behaviour.IDLE.NoSpace,1 */
    /* !LINKSTO Fee.Dsn.EraseImmediate.Behaviour.IDLE.NoSpace,1 */
    /* !LINKSTO Fee.Dsn.Write.Behaviour.IDLE.NoSpace,1 */
    /* Perform the swap */
    Fee_MarkSectionActive(FEE_SECTIONINCREMENT(Fee_Gv.FeeActiveSectionIndex));
  }
}

/*--------------------------[Fee_SearchConfigTable]--------------------------*/

FUNC(uint16, FEE_CODE) Fee_SearchConfigTable
(
  const uint16 BlockId
)
{
    uint16 localReturnValue = 0xFFFF;
    uint16 localStartIndex;
    uint16 localMiddleIndex;
    uint16 localEndIndex;
    boolean localExitCondition = (boolean)FALSE;

    /* set index values for start of search (Divide et Impera algorithm) */
    localStartIndex = (uint16)0;
    localEndIndex = (uint16)(FEE_NO_OF_CONFIGURED_BLOCKS - (uint16)1);
    localMiddleIndex = (uint16)(localEndIndex / (uint16)2);
    /* iterate through the configured block IDs to search for the received one */
    /* Deviation TASKING-1 */
    while(localExitCondition == (boolean)FALSE)
    {
      /* if the currently checked block ID is the same as the received parameter */
      if(Fee_BlockCfg[localMiddleIndex].FeeBlockNumber == BlockId)
      {
        /* set the pointer that is the returned value */
        localReturnValue = localMiddleIndex;
        /* indicate that the while loop can exit */
        localExitCondition = (boolean)TRUE;
      }
      else
      {
        /* if the two margin indexes are no more than 1 appart */
        if(((uint16)(localEndIndex - localStartIndex)) <= (uint16)1)
        {
          /* set the exit condition to TRUE - the 2 remaining checks are done below */
          localExitCondition = (boolean)TRUE;
          /* check the ID for the End index */
          if(Fee_BlockCfg[localEndIndex].FeeBlockNumber == BlockId)
          {
            /* set the pointer that is the returned value */
            localReturnValue = localEndIndex;
          }
          else
          {
            /* nothing to do here - block ID is not among the configured ones */
          }
        }
        else
        {
          /* if the currently checked block ID is smaller than the received parameter */
          if(Fee_BlockCfg[localMiddleIndex].FeeBlockNumber < BlockId)
          {
            /* update start index so that the search is conducted in the upper half */
            localStartIndex = localMiddleIndex + (uint16)1;
          }
          else
          {
            /* update end index so that the search is conducted in the lower half */
            localEndIndex = (uint16)(localMiddleIndex - (uint16)1);
          }
          /* determine next middle index value for block ID comparison - searching must continue */
          localMiddleIndex = (uint16)(localEndIndex + localStartIndex);
          localMiddleIndex /= (uint16)2;
        }
      }
    } /* end of "while" */

#if (FEE_NUMBER_NOT_CONFIGURED_BLOCKS > 0U)
    /* Block might be among the not configured blocks */
    if(localReturnValue == 0xFFFF)
    {
      for(localMiddleIndex = FEE_NO_OF_CONFIGURED_BLOCKS; localMiddleIndex < (FEE_NO_OF_CONFIGURED_BLOCKS + Fee_Gv.FeeNbOfNotCfgBlocksFound); localMiddleIndex++)
      {
        if(Fee_NotCfgBlocks[(localMiddleIndex - FEE_NO_OF_CONFIGURED_BLOCKS)].FeeBlockNumber == BlockId)
        {
          localReturnValue = localMiddleIndex;
          break;
        }
      }
    }
#endif

    return localReturnValue;
}

/*--------------------------[Fee_CalculateChecksum]--------------------------*/

FUNC(uint8, FEE_CODE) Fee_CalculateChecksum
(
  P2CONST(uint8, FEE_CONST, FEE_APPL_DATA) Buffer,
  Fls_LengthType Length
)
{
  uint8 ChkSum = 0U;
  Fls_LengthType ByteIndex;


  for (ByteIndex = 0U; ByteIndex < Length; ByteIndex++)
  {
    ChkSum += Buffer[ByteIndex];
  }

  /* Calculate final checksum by using XOR */
  ChkSum ^= (uint8)0xAA;

  return ChkSum;
}

/*--------------------------[Fee_FillBlockInfo]------------------------------*/

FUNC(void, FEE_CODE) Fee_FillBlockInfo
(
  const uint16 BlockNumber,
  const uint16 BlockSize,
  const Fls_AddressType BlockAddress,
  const uint8  CopyWriteStatus
)
{
  /* Update block number */
  FEE_WRITE_WORD(Fee_Gv.FeeBuffPtr, FEE_BLOCKNUMBER_INDEX, BlockNumber);

  /* Update block length */
  FEE_WRITE_WORD(Fee_Gv.FeeBuffPtr, FEE_BLOCKLENGTH_INDEX, BlockSize);

  /* Update block data address */
  FEE_WRITE_DWORD(Fee_Gv.FeeBuffPtr, FEE_BLOCKDATAADDRESS_INDEX, BlockAddress);

  /* Update copy-write info */
  Fee_Gv.FeeBuffPtr[FEE_WRITE_STATUS_INDEX] = CopyWriteStatus;

  /* Calculate and update checksum */
  Fee_Gv.FeeBuffPtr[FEE_CHECKSUM_INDEX] = Fee_CalculateChecksum(Fee_Gv.FeeBuffPtr, FEE_CHECKSUM_NUMBER_OF_BYTES);

  /* Pad the unused space in block info */
  TS_MemSet(&(Fee_Gv.FeeBuffPtr[FEE_BLOCKINFO_SIZE]), FEE_PAD_BYTE,
             FEE_BLOCKINFO_ALIGNED_SIZE - FEE_BLOCKINFO_SIZE);
}

/*--------------------------[Fee_JobEnd]------------------------------------*/

FUNC(void, FEE_CODE) Fee_JobEnd
(
  void
)
{
  /* Handle activities after a successful ending job */
  Fee_Gv.FeeJobResult = MEMIF_JOB_OK;
  /* After a job is completed Fee should always transition to Idle */
#if (FEE_CRITICAL_BLOCK_NUMBER != 0U)
  if(FEE_BLOCK_CFG(Fee_Gv.FeeIntBlockIndex)->FeeBlockNumber == FEE_CRITICAL_BLOCK_NUMBER)
  {
    Fee_CriticalJobFinish(MEMIF_JOB_OK);
  }
  else
#endif
  {
    /* Call NVM's notification function */
    FEE_NVM_JOB_END_NOTIFICATION();
    /* !LINKSTO Fee.Dsn.Idle,1 */
    FEE_TRIGGER_TRANS(FEE_IDLE);
  }
  /* Unqueue the current job */
  Fee_Gv.FeeJob = FEE_NO_JOB;
}

/*--------------------------[Fee_JobError]------------------------------------*/

FUNC(void, FEE_CODE) Fee_JobError
(
  void
)
{
  /* Set job result to MEMIF_JOB_FAILED and FeeJob to FEE_NO_JOB since the job failed */
  Fee_Gv.FeeJobResult = MEMIF_JOB_FAILED;
  /* set Fee status to idle and store failure */
#if (FEE_CRITICAL_BLOCK_NUMBER != 0U)
  if(FEE_BLOCK_CFG(Fee_Gv.FeeIntBlockIndex)->FeeBlockNumber == FEE_CRITICAL_BLOCK_NUMBER)
  {
    Fee_CriticalJobFinish(MEMIF_JOB_FAILED);
  }
  else
#endif
  {
    /* Call NvM's error notification function */
    FEE_NVM_JOB_ERROR_NOTIFICATION();
    /* !LINKSTO Fee.Dsn.Idle,1 */
    FEE_TRIGGER_TRANS(FEE_IDLE);
  }
  /* Unqueue the current job */
  Fee_Gv.FeeJob = FEE_NO_JOB;
}

/*--------------------------[Fee_CriticalBlockHandle]------------------------------------*/
#if (FEE_CRITICAL_BLOCK_NUMBER != 0U)
FUNC(void, FEE_CODE) Fee_CriticalBlockHandle
(
  void
)
{
  if(FEE_CRITICAL_BLOCK_NUMBER == FEE_BLOCK_CFG(Fee_Gv.FeeIntBlockIndex)->FeeBlockNumber)
  {
    /* Check that the critical write has not started yet */
    if((Fee_Gv.FeeCriticalState == FEE_STATE_INVALID) && (Fee_Gv.FeeJob == FEE_WRITE_JOB))
    {
      /* Fee_Gv.FeeJob == FEE_WRITE_JOB */
      if(Fee_Gv.FeeMode != FEE_MODE_STARTUP1)
      {
        Fee_InitiateCriticalWrite();
      }
      else
      {
        /* This critical write is requested too early at initialization */
        Fee_JobError();
      }
    }
    else
    {
      /* Do nothing */
    }
  }
  else
  {
    /* Do nothing */
  }
}

/*--------------------------[Fee_Get_Critical_Result]-------------------------------*/

FUNC(void, FEE_CODE) Fee_Get_Critical_Result
(
  void
)
{
  if ((Fee_Gv.FeeCriticalState != FEE_STATE_INVALID) && (FEE_ERASE_IMMEDIATE_JOB == Fee_Gv.FeeJob))
  {
    /* !LINKSTO Fee.Dsn.Critical.ResumeFailed,1 */
    Fee_Gv.FeeCriticalState = FEE_STATE_INVALID;
    Fee_Gv.FeeFlashJobResult = MEMIF_JOB_FAILED;
  }
  else
  {
    Fee_Gv.FeeFlashJobResult = Fls_GetJobResult();
  }
}

/*--------------------------[Fee_IsCriticalBlockErased]-------------------------------*/

FUNC(boolean, FEE_CODE) Fee_IsCriticalBlockErased
(
  uint16 CriticalBlockIndex
)
{
  return Fee_IsInconsistentBlockAddress(CriticalBlockIndex);
}

/*--------------------------[Fee_CriticalCancel]-------------------------------*/

FUNC(void, FEE_CODE) Fee_CriticalCancel
(
  void
)
{
  if (FEE_NO_JOB != Fee_Gv.FeeJob)
  {
    /* !LINKSTO Fee.FeeCriticalBlock.CancelNvMJob,1 */
    Fee_CancelJob();
    /* It's not NvM that canceled this job. NvM is the only one that should. Consider job as failed. */
    Fee_Gv.FeeJobResult = MEMIF_JOB_FAILED;
    /* A critical write can interrupt a critical erase */
    if (Fee_BlockCfg[Fee_Gv.FeeIntBlockIndex].FeeBlockNumber == FEE_CRITICAL_BLOCK_NUMBER)
    {
      /* Do not report to NvM. */
      /* FeeCriticalState is not needed any more because the first MF after erase request got the FeeCriticalState in Fee_State. */
      Fee_Gv.FeeCriticalState = FEE_STATE_INVALID;
    }
    else
    {
      /* Call NVM's error notification function */
      FEE_NVM_JOB_ERROR_NOTIFICATION();
    }
  }
  else
  {
    /* Do nothing. There is no regular job to be canceled. */
  }
}
#endif /* (FEE_CRITICAL_BLOCK_NUMBER != 0U) */

#if (FEE_NUMBER_NOT_CONFIGURED_BLOCKS > 0U)
/*--------------------------[Fee_GetNotConfigFoundBlocksUsedSize]-------------------------------*/

FUNC(uint32, FEE_CODE) Fee_GetNotConfigFoundBlocksUsedSize(void)
{
  uint32 NotCfgBlocksUsedSize = 0;
  uint8  NotConfigBlockIndex;
  for (NotConfigBlockIndex = 0; NotConfigBlockIndex < Fee_Gv.FeeNbOfNotCfgBlocksFound; ++NotConfigBlockIndex)
  {
    NotCfgBlocksUsedSize += (uint32)(
                                     ((uint32)(Fee_NotCfgBlocks[NotConfigBlockIndex].FeeBlockAlignedSize))
                                     + FEE_BLOCK_MANAGEMENT_ALIGNED_TOTAL_SIZE
                                    );
  }
  return NotCfgBlocksUsedSize;
}
#endif
/*==================[internal function definitions]=========================*/

/*--------------------------[Fee_IsInvalidBlockAddress]--------------------*/

STATIC FUNC(boolean, FEE_CODE) Fee_IsInvalidBlockAddress
(
  uint16 BlockIndex
)
{
  boolean Result = FALSE;
  uint8 SectionIndex;
  for(SectionIndex = 0;SectionIndex < FEE_NUMBER_OF_SECTIONS; SectionIndex++)
  {
    if (FEE_INVALID_BLOCKADDRESS(SectionIndex) == Fee_Gv.FeeCache[BlockIndex])
    {
      Result = TRUE;
    }
  }
  return Result;
}

/*--------------------------[Fee_IsInconsistentBlockAddress]--------------------*/

STATIC FUNC(boolean, FEE_CODE) Fee_IsInconsistentBlockAddress
(
  uint16 BlockIndex
)
{
  boolean Result = FALSE;
  uint8 SectionIndex;
  for(SectionIndex = 0;SectionIndex < FEE_NUMBER_OF_SECTIONS; SectionIndex++)
  {
    if (FEE_INCONSISTENT_BLOCKADDRESS(SectionIndex) == Fee_Gv.FeeCache[BlockIndex])
    {
      Result = TRUE;
    }
  }
  if(Fee_Gv.FeeCache[BlockIndex] == FEE_ZERO_BLOCKADDRESS)
  {
    Result = TRUE;
  }
  else
  {
    /* Do nothing. Result is FALSE */
  }
  return Result;
}

#if (FEE_CRITICAL_BLOCK_NUMBER != 0U)
/*--------------------------[Fee_InitiateCriticalWrite]----------------*/

STATIC FUNC(void, FEE_CODE) Fee_InitiateCriticalWrite(void)
{
  Fls_AddressType AuxAddress;
  boolean CriticalBlockCanBeWritten = TRUE;

  /* Interchange addresses if it was reserved space in critical section */
  if(Fee_Gv.FeeInfoAddressCritical != 0U)
  {
    /* !LINKSTO Fee.Dsn.Critical.CriticalSection,1 */
    AuxAddress = Fee_Gv.FeeInfoAddress;
    Fee_Gv.FeeInfoAddress = Fee_Gv.FeeInfoAddressCritical;
    Fee_Gv.FeeInfoAddressCritical = AuxAddress;

    AuxAddress = Fee_Gv.FeeDataAddress;
    Fee_Gv.FeeDataAddress = Fee_Gv.FeeDataAddressCritical;
    Fee_Gv.FeeDataAddressCritical = AuxAddress;
  }
  else
  {
    /* If available space is not enough to hold required space */
    if (((Fee_Gv.FeeDataAddress + (Fls_LengthType)Fee_BlockCfg[Fee_Gv.FeeIntBlockIndex].FeeBlockAlignedSize)
        >
      (Fee_Gv.FeeInfoAddress - (FEE_DATA_INFO_MARGIN*FEE_BLOCKINFO_PLUS_WM_SIZE_AL)))
        || (FEE_SECTIONINCREMENT(Fee_Gv.FeeActiveSectionIndex) == Fee_Gv.FeeOldestSectionIndex))
    {
#if (FEE_NUMBER_OF_SECTIONS > 2U)
      /* If this is not the critical section */
      if((Fee_Gv.Fee_SectionManagement[FEE_SECTIONINCREMENT(Fee_Gv.FeeActiveSectionIndex)].SectionStatus == FEE_SECTION_EMPTY) &&
          (FEE_SECTIONDIF(Fee_Gv.FeeOldestSectionIndex,FEE_SECTIONINCREMENT(Fee_Gv.FeeActiveSectionIndex)) != (uint8)1U))
      {
        /* !LINKSTO Fee.Dsn.Write.Behaviour.IDLE.NoSpace,1 */
        /* !LINKSTO Fee.Dsn.Critical.SectionChange,1 */
        /* Change the section */
        Fee_Gv.FeeActiveSectionIndex = FEE_SECTIONINCREMENT(Fee_Gv.FeeActiveSectionIndex);
        /* Load next data write address */
        Fee_Gv.FeeDataAddress = FEE_DATA_ADDRESS(Fee_Gv.FeeActiveSectionIndex);
        /* Load next info write address */
        Fee_Gv.FeeInfoAddress = FEE_FIRST_BLOCK_INFO_ADDRESS(Fee_Gv.FeeActiveSectionIndex);
        Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeActiveSectionIndex].SectionStatus = FEE_SECTION_ACTIVE;
      }
      else
#endif /* (FEE_NUMBER_OF_SECTIONS > 2U) */
      {
        /* !LINKSTO Fee.FeeCriticalBlock.Fail,1 */
        /* This is the second write of the critical block in the same section. Report failure. */
        Fee_JobError();
        CriticalBlockCanBeWritten = FALSE;
      }
    }
    else
    {
      /* The block fits in the section. There is no need to change the section. */
      /* In critical section there is always free space for the first write of the critical block */
    }
  }

  if(CriticalBlockCanBeWritten == TRUE)
  {
    /* !LINKSTO Fee.FeeCriticalBlock.Priority,1 */
    Fee_Gv.FeeCriticalState = Fee_State;
    Fls_Cancel();
    Fee_State = FEE_STATE_INVALID;
    Fee_Gv.Fee_BufferIndex = 0U;
    /* Start block write as space is available in active section */
    /* !LINKSTO Fee.Dsn.WriteBlockInfo,1 */
    FEE_TRIGGER_TRANS(FEE_WRITE_BLOCK_INFO);
  }
  else
  {
    /* Block can not be written */
  }
}

/*--------------------------[Fee_CriticalJobFinish]----------------*/

STATIC FUNC(void, FEE_CODE) Fee_CriticalJobFinish(MemIf_JobResultType CriticalJobResult)
{
  if(Fee_Gv.FeeJob == FEE_WRITE_JOB)
  {
    if(Fee_Gv.FeeInfoAddressCritical != 0)
    {
      /* Take the saved values back in case the write was performed in critical section's reserved space*/
      /* !LINKSTO Fee.Dsn.Critical.CriticalSection,1 */
      Fee_Gv.FeeInfoAddress = Fee_Gv.FeeInfoAddressCritical;
      Fee_Gv.FeeDataAddress = Fee_Gv.FeeDataAddressCritical;

      Fee_Gv.FeeInfoAddressCritical = 0U;
      Fee_Gv.FeeDataAddressCritical = 0U;
    }
    else
    {
      /* Do nothing. The block was written in normal conditions. */
    }
    if(MEMIF_JOB_OK == CriticalJobResult)
    {
      /* Freeze Fee */
      FEE_TRIGGER_TRANS(FEE_CRITICAL_FROZEN);
    }
    else
    {
      if(FEE_STATE_INVALID != Fee_Gv.FeeCriticalState)
      {
        /* The job is considered failed. Resume Fee operations */
        Fee_State = Fee_Gv.FeeCriticalState;
        /* Reset critical state */
        Fee_Gv.FeeCriticalState = FEE_STATE_INVALID;
      }
      else
      {
        if (FALSE == Fee_IsCriticalBlockErased(Fee_Gv.FeeIntBlockIndex))
        {
          /* The critical block is still written. Freeze Fee */
          Fee_State = FEE_CRITICAL_FROZEN;
        }
        else
        {
          /* Do nothing the critical write could not be performed due to lack of space.
           * In this case Fee only canceled the NvM job, if any. Fee naturally recovers. */
        }
      }
    }
  }
  else
  {
    /* After the read job or after a failed erase immediate go back to frozen. */
    if((Fee_Gv.FeeJob == FEE_READ_JOB) || (MEMIF_JOB_FAILED == CriticalJobResult))
    {
      /* !LINKSTO Fee.FeeCriticalBlock.ReadBlock,1 */
      FEE_TRIGGER_TRANS(FEE_CRITICAL_FROZEN);
    }
    else
    {
      /* There is an erase immediate job for critical block (a standard job) and it succeeded. */
      FEE_TRIGGER_TRANS(FEE_IDLE);
    }
  }
}
#endif /* (FEE_CRITICAL_BLOCK_NUMBER != 0U) */

/*--------------------------[Fee_ChkFreeSpace]-------------------------------*/

STATIC FUNC(boolean, FEE_CODE)Fee_ChkFreeSpace
(
  void
)
{
  Fls_LengthType SpaceAvailable;
  Fls_LengthType SpaceRequired = 0U;
  boolean IsEnoughSpace = FALSE;

#if (FEE_CRITICAL_BLOCK_NUMBER != 0U)
#if (FEE_NUMBER_OF_SECTIONS > 2U)
  if((Fee_Gv.Fee_SectionManagement[FEE_SECTIONINCREMENT(Fee_Gv.FeeActiveSectionIndex)].SectionStatus == FEE_SECTION_EMPTY) &&
      (FEE_SECTIONDIF(Fee_Gv.FeeOldestSectionIndex,FEE_SECTIONINCREMENT(Fee_Gv.FeeActiveSectionIndex)) != (uint8)1U))
  {
    /* Do nothing. No need to save space for critical block. */
  }
  else
#endif /* (FEE_NUMBER_OF_SECTIONS > 2U) */
  {
    /* !LINKSTO Fee.Dsn.Critical.CriticalSection,1 */
    /* Critical block space must be considered in critical active section */
    SpaceRequired = FEE_CRITICAL_BLOCK_SIZE + FEE_BLOCKINFO_PLUS_WM_SIZE_AL;
  }
#endif /* (FEE_CRITICAL_BLOCK_NUMBER != 0U) */

  /* Calculate available space ("- 1U" is because info address is already updated) */
  if(Fee_Gv.FeeDataAddress < (Fee_Gv.FeeInfoAddress - ((FEE_DATA_INFO_MARGIN - 1U)*FEE_BLOCKINFO_PLUS_WM_SIZE_AL)))
  {
    SpaceAvailable = ((Fee_Gv.FeeInfoAddress - ((FEE_DATA_INFO_MARGIN - 1U)*FEE_BLOCKINFO_PLUS_WM_SIZE_AL)) - Fee_Gv.FeeDataAddress);
  }
  else
  {
    /* There is already no space. */
    SpaceAvailable = 0U;
  }

  if(FEE_WRITE_JOB == Fee_Gv.FeeJob)
  {
    /* Set space required as block size aligned to virtual page size and block info
           size aligned to virtual page size */
    SpaceRequired +=
        (Fls_LengthType)FEE_BLOCK_CFG(Fee_Gv.FeeIntBlockIndex)->FeeBlockAlignedSize +
#if (FEE_CONSISTENCY_START_PATTERN_ENABLED == STD_ON)
        FEE_CONSISTENCY_PATTERN_SIZE +
#endif
        (Fls_LengthType)FEE_BLOCKINFO_PLUS_WM_SIZE_AL;
  }
  else /* (FEE_INVALIDATE_JOB == Fee_Gv.FeeJob) || (FEE_ERASE_IMMEDIATE_JOB == Fee_Gv.FeeJob) */
  {
    /* Set space required as the block info size aligned to virtual page size */
    SpaceRequired += (Fls_LengthType)FEE_BLOCKINFO_PLUS_WM_SIZE_AL;
  }
  /* If available space is enough to hold required space */
  if (SpaceAvailable >= SpaceRequired)
  {
    IsEnoughSpace = TRUE;
  }

  return IsEnoughSpace;
}

/*--------------------------[Fee_ChkFreeSpaceForCopy]------------------------*/

STATIC FUNC(boolean, FEE_CODE) Fee_ChkFreeSpaceForCopy
(
  void
)
{
  Fls_AddressType SpaceRequired;
  boolean RetVal = FALSE;

  const Fls_AddressType SpaceAvailable =
                            (((Fee_Gv.FeeInfoAddress + (Fls_AddressType)FEE_BLOCKINFO_PLUS_WM_SIZE_AL) -
                            Fee_Gv.FeeDataAddress) -
                            (FEE_DATA_INFO_MARGIN*FEE_BLOCKINFO_PLUS_WM_SIZE_AL));
  if(FEE_CHECK_FLAG(FEE_HIGHEST_INTERNAL_PRIORITY_MASK))
  {
    SpaceRequired = Fee_Gv.FeeSwitchRequiredSpace;
  }
  else
  {
    if(Fee_Gv.FeeSwitchJob == FEE_INVALIDATE_JOB)
    {
      SpaceRequired = (Fls_AddressType)FEE_BLOCKINFO_PLUS_WM_SIZE_AL;
    }
    else
    {
      /* Set space required as block size aligned to virtual page size and block info
       * size aligned to virtual page size */
      SpaceRequired =
#if (FEE_CONSISTENCY_START_PATTERN_ENABLED == STD_ON)
                      FEE_CONSISTENCY_PATTERN_SIZE +
#endif
                      ((Fls_AddressType)FEE_COPY_BLOCK_SIZE_AL() +
                       (Fls_AddressType)FEE_BLOCKINFO_PLUS_WM_SIZE_AL);
    }
#if (FEE_CRITICAL_BLOCK_NUMBER != 0U)
#if (FEE_NUMBER_OF_SECTIONS > 2U)
    if(FEE_SECTIONDIF(Fee_Gv.FeeOldestSectionIndex,FEE_SECTIONINCREMENT(Fee_Gv.FeeActiveSectionIndex)) != (uint8)1U)
    {
      /* Do nothing. No need to save space for critical block. */
    }
    else
#endif /* (FEE_NUMBER_OF_SECTIONS > 2U) */
    {
      /* !LINKSTO Fee.Dsn.Critical.CriticalSection,1 */
      /* Critical block space must be considered in critical active section */
      SpaceRequired += FEE_CRITICAL_BLOCK_SIZE + FEE_BLOCKINFO_PLUS_WM_SIZE_AL;
    }
#endif /* (FEE_CRITICAL_BLOCK_NUMBER != 0U) */
  }

  /* If available space is enough to hold required space */
  if (SpaceAvailable >= SpaceRequired)
  {
    RetVal = TRUE;
  }

  return RetVal;
}

/*--------------------------[Fee_ProcessWriteInvalidateOrEraseJob]----------------*/

INLINE STATIC FUNC(void, FEE_CODE) Fee_ProcessWriteInvalidateOrEraseJob(void)
{
  /* Restart the delay timer, if startup or switch internal
         operation time delay is active */
  /* !LINKSTO Fee.Dsn.Timer.UserJobRequests,1 */
  Fee_Gv.FeeDelayTimer = FEE_DELAY;
  if(FEE_WRITE_JOB == Fee_Gv.FeeJob)
  {
    /* !LINKSTO Fee.Dsn.Invalidate.Behaviour.IDLE.IsSpace,1 */
    /* !LINKSTO Fee.Dsn.Invalidate.Behaviour.SSOngoing.IsSpace,1 */
    Fee_Gv.Fee_BufferIndex = 0U;
    /* Start block write as space is available in active section */
#if ((FEE_CONSISTENCY_START_PATTERN_ENABLED == STD_OFF))
    /* !LINKSTO Fee.Dsn.WriteBlockInfo,1 */
    FEE_TRIGGER_TRANS(FEE_WRITE_BLOCK_INFO);
#else
    /* !LINKSTO Fee.ConsistencyStartPattern.BlockManagementInformation.Write,1 */
    /* Write the block info start pattern */
    FEE_TRIGGER_TRANS(FEE_WRITE_BLOCK_INFO_START_PATTERN);
#endif
  }
  /* This is an invalidate job */
  else if (FEE_INVALIDATE_JOB == Fee_Gv.FeeJob)
  {
    /* !LINKSTO Fee.Dsn.Invalidate.Behaviour.IDLE.IsSpace,1 */
    /* !LINKSTO Fee.Dsn.Invalidate.Behaviour.SSOngoing.IsSpace,1 */
    /* Initiate the Block Info write as space is available
     * in the active section */
#if ((FEE_CONSISTENCY_START_PATTERN_ENABLED == STD_OFF))
    FEE_TRIGGER_TRANS(FEE_INVALIDATE);
#else
    /* !LINKSTO Fee.ConsistencyStartPattern.BlockManagementInformation.Write,1 */
    /* Write the block info start pattern */
    FEE_TRIGGER_TRANS(FEE_WRITE_BLOCK_INFO_START_PATTERN);
#endif
  }
  else
  {
    /* !LINKSTO Fee.Dsn.EraseImmediate.Behaviour.IDLE.IsSpace,1 */
    /* !LINKSTO Fee.Dsn.EraseImmediate.Behaviour.SSOngoing.IsSpace,1 */
    /* Initiate the Block Info write as space is available
     * in the active section */
#if ((FEE_CONSISTENCY_START_PATTERN_ENABLED == STD_OFF))
    FEE_TRIGGER_TRANS(FEE_ERASE_IMMEDIATE);
#else
    /* !LINKSTO Fee.ConsistencyStartPattern.BlockManagementInformation.Write,1 */
    /* Write the block info start pattern */
    FEE_TRIGGER_TRANS(FEE_WRITE_BLOCK_INFO_START_PATTERN);
#endif
  }
}

/*--------------------------[Fee_InitiateRead]-------------------------------*/

STATIC FUNC(void, FEE_CODE) Fee_InitiateRead
(
  void
)
{
  /* Restart the delay timer, if startup or switch internal
               operation time delay is active */
  /* !LINKSTO Fee.Dsn.Timer.UserJobRequests,1 */
  Fee_Gv.FeeDelayTimer = FEE_DELAY;

  if (Fee_IsInconsistentBlockAddress(Fee_Gv.FeeIntBlockIndex))
  {
    /* !LINKSTO Fee.Dsn.Read.Behaviour.BlockInconsistent,1 */
    Fee_Gv.FeeJobResult = MEMIF_BLOCK_INCONSISTENT;
    Fee_Gv.FeeJob = FEE_NO_JOB;

    /* Stop the job processing as the block is found to be inconsistent in cache.
     * The state machine shall return to IDLE state.
     */
    FEE_TRIGGER_TRANS(FEE_IDLE);
#if (FEE_CRITICAL_BLOCK_NUMBER != 0U)
    if(FEE_BLOCK_CFG(Fee_Gv.FeeIntBlockIndex)->FeeBlockNumber == FEE_CRITICAL_BLOCK_NUMBER)
    {
      /* Do not report to NvM. */
    }
    else
#endif
    {
      FEE_NVM_JOB_ERROR_NOTIFICATION();
    }
  }

  else if (Fee_IsInvalidBlockAddress(Fee_Gv.FeeIntBlockIndex))
  {
    /* !LINKSTO Fee.Dsn.Read.Behaviour.BlockInvalid,1 */
    Fee_Gv.FeeJobResult = MEMIF_BLOCK_INVALID;
    Fee_Gv.FeeJob = FEE_NO_JOB;

    /* Stop the job processing as the block is found to be invalidated in cache.
     * The state machine shall return to IDLE state.
     */
    FEE_TRIGGER_TRANS(FEE_IDLE);

    FEE_NVM_JOB_ERROR_NOTIFICATION();
  }

  else
  {
    /* !LINKSTO Fee.Dsn.Read.Behaviour.IDLE,1 */
    /* Read the block data and block number */
#if (FEE_CONSISTENCY_START_PATTERN_ENABLED == STD_ON)
    FEE_TRIGGER_TRANS(FEE_READ_BLOCK_DATA_START_PATTERN);
#else
    FEE_TRIGGER_TRANS(FEE_READ_BLOCKDATA);
#endif
  }
}

/*--------------------------[Fee_RestartSwitch]-------------------------*/

STATIC FUNC(void, FEE_CODE) Fee_RestartSwitch
(
  void
)
{
  uint16 BlockIndex = (uint16)0;
#if (FEE_NUMBER_NOT_CONFIGURED_BLOCKS > 0U)
  uint16 NbOfNotCfgBlocksFound = Fee_Gv.FeeNbOfNotCfgBlocksFound;
#else
  uint16 NbOfNotCfgBlocksFound = 0U;
#endif

  FEE_SET_FLAG(FEE_SWITCH_RESTARTED_MASK);
  while (BlockIndex < (FEE_NO_OF_CONFIGURED_BLOCKS + NbOfNotCfgBlocksFound))
  {
    /* !LINKSTO Fee.Dns.SS.RestartSwitch,1 */
    /* Check if the current block has already been switched */
    if ((Fee_Gv.FeeCache[BlockIndex] >=
           FEE_DATA_ADDRESS(Fee_Gv.FeeActiveSectionIndex)
       )
        &&
       (Fee_Gv.FeeCache[BlockIndex] <
           FEE_SECTION_END_ADDRESS(Fee_Gv.FeeActiveSectionIndex)
       )
    )
    {
      /* Rollback the address in cache. */
      Fee_Gv.FeeCache[BlockIndex] = Fee_Gv.FeeSwRestartCache[BlockIndex];
    }
    /* If block is consistent or invalid, and in the oldest section section */
    else
    {
      /* No need to rollback the address */
      BlockIndex++;
    }
  }

  /* Initialize the copy block index to start the switch operation */
  Fee_Gv.FeeCopyBlockIndex = 0U;
  /* The Active section needs to be erased and revalidated */
  Fee_Gv.FeeWorkingSectionIndex = Fee_Gv.FeeActiveSectionIndex;
  FEE_TRIGGER_TRANS(FEE_MARK_HEADER_ERASABLE);
}

#if  ((FEE_WRITE_CUSTOM_API == STD_ON) || (FEE_READ_CUSTOM_API == STD_ON))
/*--------------------------[Fee_CheckAndSetPreemption]-------------------------*/

FUNC(boolean, FEE_CODE) Fee_CheckAndSetPreemptionProtection(void)
{
  boolean Preempted = FALSE;
  /* !LINKSTO Fee.PreemptionProtection,1 */
  /* Protection against interruptions by other asynchronous job APIs */
  SchM_Enter_Fee_SCHM_FEE_EXCLUSIVE_AREA_0();
  if (!FEE_CHECK_FLAG(FEE_PREEMPTION_MASK))
  {
    /* Set the preemption flag */
    FEE_SET_FLAG(FEE_PREEMPTION_MASK);
    /* End of interruption protection */
    SchM_Exit_Fee_SCHM_FEE_EXCLUSIVE_AREA_0();
  }
  else
  {
    /* End of interruption protection */
    SchM_Exit_Fee_SCHM_FEE_EXCLUSIVE_AREA_0();
    Preempted = TRUE;
  }

  return Preempted;
}
#endif /* ((FEE_WRITE_CUSTOM_API == STD_ON) || (FEE_READ_CUSTOM_API == STD_ON)) */

#define FEE_STOP_SEC_CODE
#include <MemMap.h>

/*==================[end of file]===========================================*/
