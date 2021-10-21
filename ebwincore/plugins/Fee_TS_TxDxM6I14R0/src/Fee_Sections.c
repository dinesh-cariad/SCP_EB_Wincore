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
/* This file contains code for section operations */
/*==================[inclusions]============================================*/

#include <Fee_Internal.h>     /* Internal function prototypes and internal defines and declarations*/
#include <TSMem.h>            /* Memory Routines */
#include <Crc.h>              /* Crc public header file  */
#include <Fee_Sections.h>     /* Not needed. Included only to prevent warning. */

/*==================[macros]=================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define FEE_START_SEC_CODE
#include <MemMap.h>

/** \brief Function to fill the buffer with all the components of a section management obj */
STATIC FUNC(void, FEE_CODE) Fee_FillSectionMngObj(void);

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

/*--------------------------[Fee_SfOnEntryMarkHeaderActive]----------------*/

FUNC(void, FEE_CODE) Fee_SfOnEntryMarkHeaderActive
(
  void
)
{
  /* Calculate target address  to write */
  const Fls_AddressType TargetAddress = FEE_SECTION_START_ADDRESS(Fee_Gv.FeeActiveSectionIndex) + FEE_HEADER_ACTIVE_MARKER_OFFSET;

  /* Clear the content of the buffer */
  TS_MemSet(Fee_Gv.FeeBuffPtr, FEE_SECTION_STATUS_ACTIVE, FEE_SECTION_STATUS_ALIGNED_SIZE);

  if(E_OK == Fls_Write(TargetAddress, Fee_Gv.FeeBuffPtr, FEE_SECTION_STATUS_ALIGNED_SIZE))
  {
    /* Reset retry counter */
    Fee_Gv.FeeRetryStateCounter = FEE_STATE_MAX_RETRIES;
    Fee_Gv.FeeFlashJobResult = MEMIF_JOB_PENDING;
  }
  else
  {
    Fee_FlsJobRefused();
  }
}

/*--------------------------[Fee_SfOnEntryMarkFooterActive]----------------*/

FUNC(void, FEE_CODE) Fee_SfOnEntryMarkFooterActive
(
  void
)
{
  /* Calculate target address  to write */
  const Fls_AddressType TargetAddress = FEE_FOOTER_ADDRESS(Fee_Gv.FeeActiveSectionIndex) + FEE_HEADER_ERASABLE_MARKER_OFFSET;

  TS_MemSet(Fee_Gv.FeeBuffPtr, FEE_SECTION_STATUS_ACTIVE, FEE_SECTION_STATUS_ALIGNED_SIZE);

  if(E_OK == Fls_Write(TargetAddress, Fee_Gv.FeeBuffPtr, FEE_SECTION_STATUS_ALIGNED_SIZE))
  {
    /* Reset retry counter */
    Fee_Gv.FeeRetryStateCounter = FEE_STATE_MAX_RETRIES;
    Fee_Gv.FeeFlashJobResult = MEMIF_JOB_PENDING;
  }
  else
  {
    Fee_FlsJobRefused();
  }
}

/*--------------------------[Fee_SfMarkHeaderFooterActive]-----------------------*/

FUNC(void, FEE_CODE) Fee_SfMarkHeaderFooterActive
(
  void
)
{
  FEE_GET_FLASH_JOB_RESULT();

  if (MEMIF_JOB_PENDING == Fee_Gv.FeeFlashJobResult)
  {
    /* Do nothing; flash job is not complete */
  }
  /* Check whether the write section active status is successful */
  else if ((MEMIF_JOB_OK == Fee_Gv.FeeFlashJobResult) && (!FEE_CHECK_FLAG(FEE_FLASH_FINAL_REFUSAL_MASK)))
  {
    Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeActiveSectionIndex].SectionStatus = FEE_SECTION_ACTIVE;

    if(FEE_SECTIONINCREMENT(Fee_Gv.FeeActiveSectionIndex) == Fee_Gv.FeeOldestSectionIndex)
    {
      /* The highest internal priority flag must have been already set by now.
       * There is an equivalence between 3 scenarios, and they should not be treated differently:
       *   1) during Startup2, a job triggers a changing section at the last available section.
       *   2) a high priority switch finishes, but the oldest section is only marked for erased,
       *   and a job triggers a changing section. The oldest section is now revalidated.
       *   3) a restart switch occurs and the section is revalidated. */
      /* !LINKSTO Fee.Dns.SS.ErasePriority,1 */
      /* If Fee mode is Startup2, transition to high priority Switch, and then "restart" the switch. */
      Fee_Gv.FeeMode = FEE_MODE_SWITCH;
      /* Check how much space is needed for high priority switch */
      Fee_Gv.FeeSwitchRequiredSpace = Fee_CalculateSSRequiredSpace();
      Fee_SectionSwitching();
    }
    else
    {
      /* If Startup2 has made a brand new active section, clear the flag. */
      FEE_RESET_FLAG(FEE_HIGHEST_INTERNAL_PRIORITY_MASK);
#if (FEE_CRITICAL_BLOCK_NUMBER != 0U)
      /* New fresh space in this new active section. */
      Fee_Gv.FeeInfoAddressCritical = 0U;
      Fee_Gv.FeeDataAddressCritical = 0U;
#endif
      switch(Fee_Gv.FeeMode)
      {
      case FEE_MODE_STARTUP2:
        /* !LINKSTO Fee.Initialization.EmptyFlash,1 */
        /* Startup2 prepared the first/a new section for job processing. */
        Fee_Gv.TriggerFunct = &Fee_StartUp2;
        FEE_TRIGGER_TRANS(FEE_TRIGGER_JOB_OR_INTERNAL);
        break;
      case FEE_MODE_SWITCH:
        /* As the delay expired, start the pending switch section */
        Fee_Gv.TriggerFunct = &Fee_SectionSwitching;
        FEE_TRIGGER_TRANS(FEE_TRIGGER_JOB_OR_INTERNAL);
        break;
      case FEE_MODE_NORMAL:
        Fee_InitiateJob();
        break;
        /* CHECK: NOPARSE */
      default:
        FEE_UNREACHABLE_CODE_ASSERT(FEE_INTERNAL_API_ID);
        break; /* Caution: this 'break' is required by MISRA-C:2004 */
        /* CHECK: PARSE */
      }
    }
  }
  /* previous flash job has failed */
  else
  {
    (void)Fee_FlsJobFailed();
    /* The job could be performed after many internal operations. Therefore clear the flag. */
    FEE_RESET_FLAG(FEE_USER_JOB_CHANGES_SECTION_MASK);

    if(Fee_State == FEE_MARK_HEADER_ACTIVE)
    {
      /* !LINKSTO Fee.Dns.FailureHandling.Behaviour.MarkingHeaderActive.Failure,1 */
      FEE_TRIGGER_TRANS(FEE_MARK_FOOTER_ACTIVE);
    }
    else
    {
      if(FEE_SECTIONINCREMENT(Fee_Gv.FeeActiveSectionIndex) == Fee_Gv.FeeOldestSectionIndex)
      {
        /* This case is similar to a restarted switch. */
        FEE_SET_FLAG(FEE_SWITCH_RESTARTED_MASK);
      }
      else
      {
        /* Do nothing. */
      }
      /* !LINKSTO Fee.Dns.FailureHandling.Behaviour.MarkingFooterActive.Failure,1 */
      /* Set the working section index the current section (for FEE_MODE_NORMAL). */
      Fee_Gv.FeeWorkingSectionIndex = Fee_Gv.FeeActiveSectionIndex;
      FEE_SET_FLAG(FEE_HIGHEST_INTERNAL_PRIORITY_MASK);
      FEE_TRIGGER_TRANS(FEE_MARK_HEADER_ERASABLE);
    }
  }
}

/*--------------------------[Fee_SfOnEntryMarkHeaderErasable]----------------*/

FUNC(void, FEE_CODE) Fee_SfOnEntryMarkHeaderErasable
(
  void
)
{
  Fls_AddressType TargetAddress;

  /* Calculate target address to write */
  TargetAddress = FEE_SECTION_START_ADDRESS(Fee_Gv.FeeWorkingSectionIndex) + FEE_HEADER_ERASABLE_MARKER_OFFSET;

  TS_MemSet(Fee_Gv.FeeBuffPtr, FEE_SECTION_STATUS_ERASABLE, FEE_SECTION_STATUS_ALIGNED_SIZE);

  if(E_OK == Fls_Write(TargetAddress, Fee_Gv.FeeBuffPtr, FEE_SECTION_STATUS_ALIGNED_SIZE))
  {
    /* Reset retry counter */
    Fee_Gv.FeeRetryStateCounter = FEE_STATE_MAX_RETRIES;
    Fee_Gv.FeeFlashJobResult = MEMIF_JOB_PENDING;
  }
  else
  {
    Fee_FlsJobRefused();
  }
}

/*--------------------------[Fee_SfMarkHeaderErasable]-----------------------*/

FUNC(void, FEE_CODE) Fee_SfMarkHeaderErasable
(
  void
)
{
  FEE_GET_FLASH_JOB_RESULT();

  if (MEMIF_JOB_PENDING == Fee_Gv.FeeFlashJobResult)
  {
    /* Do nothing; flash job is not complete */
  }
  /* Failure is not treated differently */
  else
  {
    if ((MEMIF_JOB_FAILED == Fee_Gv.FeeFlashJobResult) || (FEE_CHECK_FLAG(FEE_FLASH_FINAL_REFUSAL_MASK)))
    {
      (void)Fee_FlsJobFailed();
    }
    else
    {
      /* The operation succeeded */
    }
    /* !LINKSTO Fee.Dns.FailureHandling.Behaviour.MarkingHeaderErasable.Failure,1 */
    FEE_TRIGGER_TRANS(FEE_MARK_FOOTER_ERASABLE);
  }
}

/*--------------------------[Fee_SfOnEntryMarkFooterErasable]----------------*/

FUNC(void, FEE_CODE) Fee_SfOnEntryMarkFooterErasable
(
  void
)
{
  Fls_AddressType TargetAddress;

  /* The FeeWorkingSectionIndex is already set. */
  /* Calculate target address to write */
  TargetAddress = FEE_FOOTER_ADDRESS(Fee_Gv.FeeWorkingSectionIndex) + FEE_HEADER_ACTIVE_MARKER_OFFSET;

  TS_MemSet(Fee_Gv.FeeBuffPtr, FEE_SECTION_STATUS_ERASABLE, FEE_SECTION_STATUS_ALIGNED_SIZE);

  if(E_OK == Fls_Write(TargetAddress, Fee_Gv.FeeBuffPtr, FEE_SECTION_STATUS_ALIGNED_SIZE))
  {
    /* Reset retry counter */
    Fee_Gv.FeeRetryStateCounter = FEE_STATE_MAX_RETRIES;
    Fee_Gv.FeeFlashJobResult = MEMIF_JOB_PENDING;
  }
  else
  {
    Fee_FlsJobRefused();
  }
}

/*--------------------------[Fee_SfMarkFooterErasable]-----------------------*/

FUNC(void, FEE_CODE) Fee_SfMarkFooterErasable
(
  void
)
{
  FEE_GET_FLASH_JOB_RESULT();

  if (MEMIF_JOB_PENDING == Fee_Gv.FeeFlashJobResult)
  {
    /* Do nothing; flash job is not complete */
  }
  /* Failure is not treated differently */
  else
  {
    if ((MEMIF_JOB_FAILED == Fee_Gv.FeeFlashJobResult) || (FEE_CHECK_FLAG(FEE_FLASH_FINAL_REFUSAL_MASK)))
    {
      (void)Fee_FlsJobFailed();
    }
    else
    {
      /* The operation succeeded */
    }
    Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeWorkingSectionIndex].SectionStatus = FEE_SECTION_ERASABLE;

    /* !LINKSTO Fee.Dns.FailureHandling.Behaviour.MarkingFooterErasable.Failure,1 */
    if (!FEE_CHECK_FLAG(FEE_HIGHEST_INTERNAL_PRIORITY_MASK))
    {
      if (Fee_Gv.FeeMode == FEE_MODE_SWITCH)
      {
        Fee_Gv.FeeSwitchState = FEE_ERASE_SECTION;
        Fee_Gv.TriggerFunct = &Fee_SectionSwitching;
        FEE_TRIGGER_TRANS(FEE_TRIGGER_JOB_OR_INTERNAL);
      }
      else
      {
        Fee_Gv.TriggerFunct = &Fee_StartUp2;
        FEE_TRIGGER_TRANS(FEE_TRIGGER_JOB_OR_INTERNAL);
      }
    }
    else
    {
      FEE_TRIGGER_TRANS(FEE_ERASE_SECTION);
    }
  }
}

/*--------------------------[Fee_SfOnEntryWriteSectionHeader]------------------*/

FUNC(void, FEE_CODE) Fee_SfOnEntryWriteSectionHeader
(
  void
)
{
  Fls_AddressType TargetAddress;

  if(Fee_Gv.FeeWorkingSectionIndex != Fee_Gv.FeeActiveSectionIndex)
  {
    Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeWorkingSectionIndex].SCounter = FEE_INCREMENT_BY(Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeActiveSectionIndex].SCounter,
        FEE_SECTIONDIF(Fee_Gv.FeeWorkingSectionIndex, Fee_Gv.FeeActiveSectionIndex));
  }

  /* Calculate target address to write */
  TargetAddress = FEE_SECTION_START_ADDRESS(Fee_Gv.FeeWorkingSectionIndex) + FEE_SECTION_MANAGEMENT_OFFSET;
  /* Prepare the updated section management data in the buffer. */
  TS_MemSet(Fee_Gv.FeeBuffPtr, FEE_FLASH_ERASE_VALUE, FEE_SECTION_MANAGEMENT_OBJ_SIZE_AL);
  Fee_FillSectionMngObj();

  if(E_OK == Fls_Write(TargetAddress, Fee_Gv.FeeBuffPtr, FEE_SECTION_MANAGEMENT_OBJ_SIZE_AL))
  {
    /* Reset retry counter */
    Fee_Gv.FeeRetryStateCounter = FEE_STATE_MAX_RETRIES;
    Fee_Gv.FeeFlashJobResult = MEMIF_JOB_PENDING;
  }
  else
  {
    Fee_FlsJobRefused();
  }
}

/*--------------------------[Fee_SfWriteSectionHeader]-------------------------*/

FUNC(void, FEE_CODE) Fee_SfWriteSectionHeader
(
  void
)
{
  FEE_GET_FLASH_JOB_RESULT();

  if (MEMIF_JOB_PENDING == Fee_Gv.FeeFlashJobResult)
  {
    /* Do nothing; flash job is not complete */
  }

  else
  {
    /* Check whether the write section erased status is successful */
    if ((MEMIF_JOB_OK == Fee_Gv.FeeFlashJobResult) && (!FEE_CHECK_FLAG(FEE_FLASH_FINAL_REFUSAL_MASK)))
    {
#if (FEE_CONSISTENCY_PATTERN_ENABLED == STD_OFF)
      Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeWorkingSectionIndex].SectionStatus = FEE_SECTION_EMPTY;
      FEE_TRIGGER_TRANS(FEE_WRITE_SECTION_FOOTER);
#else
      /* !LINKSTO Fee.ConsistencyPatterns.SectionHeader.Write,1 */
      FEE_TRIGGER_TRANS(FEE_WRITE_HEADER_ERASE_COUNTER_END_PATTERN);
#endif
    }

    /* previous flash job failed */
    else
    {
      /* !LINKSTO Fee.Dns.FailureHandling.Behaviour.WritingSectionHeader.Failure,1 */
      (void)Fee_FlsJobFailed();
      FEE_TRIGGER_TRANS(FEE_WRITE_SECTION_FOOTER);
    }
  }
}

/*--------------------------[Fee_SfOnEntryWriteSectionFooter]------------------*/

FUNC(void, FEE_CODE) Fee_SfOnEntryWriteSectionFooter
(
  void
)
{
  /* Calculate target address  to write */
  const Fls_AddressType TargetAddress = FEE_FOOTER_ADDRESS(Fee_Gv.FeeWorkingSectionIndex) + FEE_SECTION_MANAGEMENT_OFFSET;

  if(Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeWorkingSectionIndex].SectionStatus == FEE_SECTION_NO_FOOTER)
  {
    TS_MemSet(Fee_Gv.FeeBuffPtr, FEE_FLASH_ERASE_VALUE, FEE_SECTION_MANAGEMENT_OBJ_SIZE_AL);
    Fee_FillSectionMngObj();
  }
  else
  {
    /* Do nothing. The buffer is already populated */
  }

  if(E_OK == Fls_Write(TargetAddress, Fee_Gv.FeeBuffPtr, FEE_SECTION_MANAGEMENT_OBJ_SIZE_AL))
  {
    /* Reset retry counter */
    Fee_Gv.FeeRetryStateCounter = FEE_STATE_MAX_RETRIES;
    Fee_Gv.FeeFlashJobResult = MEMIF_JOB_PENDING;
  }
  else
  {
    Fee_FlsJobRefused();
  }
}

/*--------------------------[Fee_SfWriteSectionFooter]-------------------------*/

FUNC(void, FEE_CODE) Fee_SfWriteSectionFooter
(
  void
)
{
  FEE_GET_FLASH_JOB_RESULT();

  if (MEMIF_JOB_PENDING == Fee_Gv.FeeFlashJobResult)
  {
    /* Do nothing; flash job is not complete */
  }
  else
  {
    /* Check whether the write section erased status is successful */
    if ((MEMIF_JOB_OK == Fee_Gv.FeeFlashJobResult) && (!FEE_CHECK_FLAG(FEE_FLASH_FINAL_REFUSAL_MASK)))
    {
#if (FEE_CONSISTENCY_PATTERN_ENABLED == STD_OFF)
      Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeWorkingSectionIndex].SectionStatus = FEE_SECTION_EMPTY;
    }
    /* previous flash job failed */
    else
    {
      /* Section status is either  FEE_SECTION_ERASED or FEE_SECTION_EMPTY (from header)*/
      (void)Fee_FlsJobFailed();
    }

    if(Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeWorkingSectionIndex].SectionStatus == FEE_SECTION_EMPTY)
    {
      /* The header is written successfully */
      if(Fee_Gv.FeeMode == FEE_MODE_SWITCH)
      {
        /* The last empty section was marked for erase and a swap triggered FEE_HIGHEST_INTERNAL_PRIORITY_MASK */
        if(FEE_CHECK_FLAG(FEE_HIGHEST_INTERNAL_PRIORITY_MASK))
        {
          if(FEE_CHECK_FLAG(FEE_SWITCH_RESTARTED_MASK))
          {
            /* If the switch is restarted clear the flag. */
            FEE_RESET_FLAG(FEE_SWITCH_RESTARTED_MASK);
          }
          else
          {
            /* !LINKSTO Fee.Dns.SS.Behaviour.ChangingActiveSection,1 */
            /* Perform the swap into this new empty section */
            Fee_Gv.FeeActiveSectionIndex = Fee_Gv.FeeWorkingSectionIndex;
          }
          Fee_MarkSectionActive(Fee_Gv.FeeActiveSectionIndex);
        }
        else
        {
          if(FEE_SECTIONDIF(Fee_Gv.FeeActiveSectionIndex,Fee_Gv.FeeOldestSectionIndex) < (FEE_NUMBER_OF_SECTIONS / 2))
          {
            /* Clear erase pending flag */
            Fee_Gv.FeeMode = FEE_MODE_NORMAL;
            Fee_InitiateJob();
          }
          else
          {
            Fee_Gv.TriggerFunct = &Fee_SectionSwitching;
            FEE_TRIGGER_TRANS(FEE_TRIGGER_JOB_OR_INTERNAL);
          }
        }
      }
      else /* FEE_MODE_STARTUP2 or FEE_MODE_NORMAL(in case of failure) */
      {
        if(FEE_CHECK_FLAG(FEE_HIGHEST_INTERNAL_PRIORITY_MASK))
        {
          /* !LINKSTO Fee.Dsn.Write.Behaviour.IDLE.NoSpace,1 */
          /* Perform the swap into this new empty section */
          Fee_MarkSectionActive(Fee_Gv.FeeWorkingSectionIndex);
          /* Marking the section as Active is the last thing that Startup2 will do to this section. */
          Fee_Gv.FeeWorkingSectionIndex = FEE_SECTIONINCREMENT(Fee_Gv.FeeWorkingSectionIndex);
        }
        else
        {
          Fee_Gv.TriggerFunct = &Fee_StartUp2;
          FEE_TRIGGER_TRANS(FEE_TRIGGER_JOB_OR_INTERNAL);
        }
      }
#else
      /* !LINKSTO Fee.ConsistencyPatterns.SectionHeader.Write,1 */
      FEE_TRIGGER_TRANS(FEE_WRITE_FOOTER_ERASE_COUNTER_END_PATTERN);
#endif
    }
    /* previous flash job failed and the header failed too */
    else
    {
      /* !LINKSTO Fee.Dns.FailureHandling.Behaviour.WritingSectionFooter.Failure,1 */
      /* Working section index is the same. */
      FEE_TRIGGER_TRANS(FEE_MARK_HEADER_ERASABLE);
    }
  }
}

/*--------------------------[Fee_SfOnEntryEraseSection]--------------------*/

FUNC(void, FEE_CODE) Fee_SfOnEntryEraseSection
(
  void
)
{

  if (E_OK == Fls_Erase((Fls_AddressType)FEE_SECTION_START_ADDRESS(Fee_Gv.FeeWorkingSectionIndex),
                        (Fls_LengthType)Fee_SectionCfg[Fee_Gv.FeeWorkingSectionIndex].FeeSectionSize
                       )
     )
  {
    /* The counter must be incremented on either erase success or failure */
    Fee_Gv.SectionEraseCounter++;
    /* Reset retry counter */
    Fee_Gv.FeeRetryStateCounter = FEE_STATE_MAX_RETRIES;
    Fee_Gv.FeeFlashJobResult = MEMIF_JOB_PENDING;
  }

  /* erase failed */
  else
  {
    Fee_FlsJobRefused();
  }
}

/*--------------------------[Fee_SfEraseSection]---------------------------*/

FUNC(void, FEE_CODE) Fee_SfEraseSection
(
  void
)
{
  FEE_GET_FLASH_JOB_RESULT();

  if (MEMIF_JOB_PENDING == Fee_Gv.FeeFlashJobResult)
  {
#if (FEE_ENABLE_ABORT_ERASE == STD_ON)
    /* !LINKSTO Fee.EnableAbortErase.HighestInternalPriority,1  */
    if (((Fee_Gv.FeeJob == FEE_WRITE_JOB) &&
        (TRUE == FEE_BLOCK_CFG(Fee_Gv.FeeIntBlockIndex)->FeeImmediateData))
        &&
        (!FEE_CHECK_FLAG(FEE_HIGHEST_INTERNAL_PRIORITY_MASK)))
    {
      if(Fee_Gv.FeeMode == FEE_MODE_SWITCH)
      {
        /* !LINKSTO Fee.Dns.SS.Behaviour.SSResume.BlockSwitching.SectionErase,1 */
        /* Save the switch job to be restarted */
        Fee_Gv.FeeSwitchState = FEE_ERASE_SECTION;
      }
      /* A job has been requested. Cancel the ongoing flash read! */
      Fls_Cancel();
      /* Fee_Gv.FeeJob will be updated when a job request is posted
       * Initiate the requested job */
      Fee_InitiateStoreRequest();
    }
    else
    {
      /* Erase shall not be interrupted. */
    }
#endif /* (FEE_ENABLE_ABORT_ERASE == STD_ON) */
  }

  /* Erase completed successfully */
  else if ((MEMIF_JOB_OK == Fee_Gv.FeeFlashJobResult) && (!FEE_CHECK_FLAG(FEE_FLASH_FINAL_REFUSAL_MASK)))
  {
    /* Reset the switch state */
    Fee_Gv.FeeSwitchState = FEE_STATE_INVALID;
    /* Update the current status of the section */
    Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeWorkingSectionIndex].SectionStatus = FEE_SECTION_ERASED;
    FEE_TRIGGER_TRANS(FEE_WRITE_SECTION_HEADER);
  }

  /* Erase failed */
  else
  {
#if (FEE_CANCEL_SECTION_ERASE == STD_ON)
    if( MEMIF_JOB_CANCELED == Fee_Gv.FeeFlashJobResult )
    {
      /* Fee_Gv.FeeSwitchState already has the value FEE_ERASE_SECTION, therefore the switch job will be restarted */
      FEE_TRIGGER_TRANS(FEE_IDLE);
    }
    else
#endif /* (FEE_CANCEL_SECTION_ERASE == STD_ON) */
    {
      (void)Fee_FlsJobFailed();
      if(FEE_CHECK_FLAG(FEE_HIGHEST_INTERNAL_PRIORITY_MASK))
      {
        /* !LINKSTO Fee.Dns.FailureHandling.Behaviour.Failure.SectionErase.HighPrio,1  */
        FEE_TRIGGER_TRANS(FEE_ERASE_SECTION);
      }
      else
      {
        /* !LINKSTO Fee.Dns.FailureHandling.Behaviour.Failure.SectionErase.LowPrio,1  */
        Fee_Gv.TriggerFunct = &Fee_StartUp2;
        FEE_TRIGGER_TRANS(FEE_TRIGGER_JOB_OR_INTERNAL);
      }
    }
  }
}

#if (FEE_CONSISTENCY_PATTERN_ENABLED == STD_ON)
/*--------------------------[Fee_SfOnEntryWriteHeaderSectionManagementEndPattern]---------*/

FUNC(void, FEE_CODE) Fee_SfOnEntryWriteHeaderSectionManagementEndPattern(void)
{
  Fls_AddressType TargetAddress = ((Fls_AddressType)(FEE_SECTION_START_ADDRESS(Fee_Gv.FeeWorkingSectionIndex) +
                  (Fls_AddressType)FEE_SECTION_MANAGEMENT_END_PATTERN_OFFSET));
  /* Write the end pattern for the Section Management Marker field */
  if(E_OK == Fls_Write(TargetAddress, &Fee_ConsistencyPattern[0], FEE_CONSISTENCY_PATTERN_SIZE))
  {
    /* Reset retry counter */
    Fee_Gv.FeeRetryStateCounter = FEE_STATE_MAX_RETRIES;
    Fee_Gv.FeeFlashJobResult = MEMIF_JOB_PENDING;
  }
  else
  {
    Fee_FlsJobRefused();
  }
}

/*--------------------------[Fee_SfWriteHeaderSectionManagementEndPattern]----------------*/

FUNC(void, FEE_CODE) Fee_SfWriteHeaderSectionManagementEndPattern(void)
{
  FEE_GET_FLASH_JOB_RESULT();

    if (MEMIF_JOB_PENDING == Fee_Gv.FeeFlashJobResult)
    {
      /* Do nothing; flash job is not complete */
    }

    /* Check whether the write section erased status is successful */
    else if ((MEMIF_JOB_OK == Fee_Gv.FeeFlashJobResult) && (!FEE_CHECK_FLAG(FEE_FLASH_FINAL_REFUSAL_MASK)))
    {
      Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeWorkingSectionIndex].SectionStatus = FEE_SECTION_EMPTY;
      FEE_TRIGGER_TRANS(FEE_WRITE_SECTION_FOOTER);
    }
    /* previous flash job failed */
    else
    {
      (void)Fee_FlsJobFailed();
      /* Try to write the footer. */
      FEE_TRIGGER_TRANS(FEE_WRITE_SECTION_FOOTER);
    }
}


/*--------------------------[Fee_SfOnEntryWriteFooterSectionManagementEndPattern]---------*/

FUNC(void, FEE_CODE) Fee_SfOnEntryWriteFooterSectionManagementEndPattern(void)
{
  Fls_AddressType TargetAddress = (Fls_AddressType)FEE_FOOTER_ADDRESS(Fee_Gv.FeeWorkingSectionIndex) + FEE_SECTION_MANAGEMENT_END_PATTERN_OFFSET;
  /* Write the end pattern for the Section Management Marker field */
  if(E_OK == Fls_Write(TargetAddress, &Fee_ConsistencyPattern[0], FEE_CONSISTENCY_PATTERN_SIZE))
  {
    /* Reset retry counter */
    Fee_Gv.FeeRetryStateCounter = FEE_STATE_MAX_RETRIES;
    Fee_Gv.FeeFlashJobResult = MEMIF_JOB_PENDING;
  }
  else
  {
    Fee_FlsJobRefused();
  }
}

/*--------------------------[Fee_SfWriteFooterSectionManagementEndPattern]----------------*/

FUNC(void, FEE_CODE) Fee_SfWriteFooterSectionManagementEndPattern(void)
{
  FEE_GET_FLASH_JOB_RESULT();

  if (MEMIF_JOB_PENDING == Fee_Gv.FeeFlashJobResult)
  {
    /* Do nothing; flash job is not complete */
  }
  else
  {
    /* Check whether the write section erased status is successful */
    if ((MEMIF_JOB_OK == Fee_Gv.FeeFlashJobResult) && (!FEE_CHECK_FLAG(FEE_FLASH_FINAL_REFUSAL_MASK)))
    {
      Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeWorkingSectionIndex].SectionStatus = FEE_SECTION_EMPTY;
    }
    /* previous flash job failed */
    else
    {
      /* Section status is either  FEE_SECTION_ERASED or FEE_SECTION_EMPTY (from header)*/
      (void)Fee_FlsJobFailed();
    }

    if(Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeWorkingSectionIndex].SectionStatus == FEE_SECTION_EMPTY)
    {
      /* The header is written successfully */
      if(Fee_Gv.FeeMode == FEE_MODE_SWITCH)
      {
        /* The last empty section was marked for erase and a swap triggered FEE_HIGHEST_INTERNAL_PRIORITY_MASK */
        if(FEE_CHECK_FLAG(FEE_HIGHEST_INTERNAL_PRIORITY_MASK))
        {
          if(FEE_CHECK_FLAG(FEE_SWITCH_RESTARTED_MASK))
          {
            /* If the switch is restarted clear the flag. */
            FEE_RESET_FLAG(FEE_SWITCH_RESTARTED_MASK);
          }
          else
          {
            /* !LINKSTO Fee.Dns.SS.Behaviour.ChangingActiveSection,1 */
            /* Perform the swap into this new empty section */
            Fee_Gv.FeeActiveSectionIndex = Fee_Gv.FeeWorkingSectionIndex;
          }
          Fee_MarkSectionActive(Fee_Gv.FeeActiveSectionIndex);
        }
        else
        {
          if(FEE_SECTIONDIF(Fee_Gv.FeeActiveSectionIndex,Fee_Gv.FeeOldestSectionIndex) < (FEE_NUMBER_OF_SECTIONS / 2))
          {
            /* Clear erase pending flag */
            Fee_Gv.FeeMode = FEE_MODE_NORMAL;
            Fee_InitiateJob();
          }
          else
          {
            Fee_Gv.TriggerFunct = &Fee_SectionSwitching;
            FEE_TRIGGER_TRANS(FEE_TRIGGER_JOB_OR_INTERNAL);
          }
        }
      }
      else /* FEE_MODE_STARTUP2 */
      {
        if(FEE_CHECK_FLAG(FEE_HIGHEST_INTERNAL_PRIORITY_MASK))
        {
          /* !LINKSTO Fee.Dns.SS.Behaviour.ChangingActiveSection,1 */
          /* Perform the swap into this new empty section */
          Fee_MarkSectionActive(Fee_Gv.FeeWorkingSectionIndex);
          /* Marking the section as Active is the last thing that Startup2 will do to this section. */
          Fee_Gv.FeeWorkingSectionIndex = FEE_SECTIONINCREMENT(Fee_Gv.FeeWorkingSectionIndex);
        }
        else
        {
          Fee_Gv.TriggerFunct = &Fee_StartUp2;
          FEE_TRIGGER_TRANS(FEE_TRIGGER_JOB_OR_INTERNAL);
        }
      }
    }
    /* previous flash job failed and the header failed too */
    else
    {
      /* Working section index is the same. */
      FEE_TRIGGER_TRANS(FEE_MARK_HEADER_ERASABLE);
    }
  }
}


#endif

/*==================[internal constants]====================================*/

/*==================[internal function definitions]=========================*/

/*--------------------------[Fee_FillSectionMngObj]------------------*/

STATIC FUNC(void, FEE_CODE) Fee_FillSectionMngObj
(
  void
)
{
  uint8 Crc;

  /* Write the next erase counter */
  FEE_WRITE_DWORD(Fee_Gv.FeeBuffPtr, FEE_INDEX_ZERO, Fee_Gv.SectionEraseCounter);
  /* Compute the erase counter's CRC */
  Crc = Crc_CalculateCRC8(&Fee_Gv.FeeBuffPtr[FEE_INDEX_ZERO], FEE_ECOUNTER_CRC_COMPUTE_SIZE, FEE_CRC_START_VALUE, TRUE);
  /* Write the erase counter's CRC */
  FEE_WRITE_BYTE(Fee_Gv.FeeBuffPtr, FEE_ECRCFIELD_OFFSET - FEE_SECTION_MANAGEMENT_OFFSET, Crc);
  /* Write the config id */
  FEE_WRITE_BYTE(Fee_Gv.FeeBuffPtr, FEE_CIDFIELD_OFFSET - FEE_SECTION_MANAGEMENT_OFFSET , FEE_CONFIG_ID);
  /* Write the section counter */
  FEE_WRITE_BYTE(Fee_Gv.FeeBuffPtr, FEE_SECTIONCOUNTERFIELD_OFFSET - FEE_SECTION_MANAGEMENT_OFFSET , Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeWorkingSectionIndex].SCounter);
  /* Compute the CRC over the section counter and the config id*/
  Crc = Crc_CalculateCRC8(&Fee_Gv.FeeBuffPtr[FEE_CIDFIELD_OFFSET - FEE_SECTION_MANAGEMENT_OFFSET], FEE_CID_CRC_COMPUTE_SIZE, FEE_CRC_START_VALUE, TRUE);
  /* Write the CRC */
  FEE_WRITE_BYTE(Fee_Gv.FeeBuffPtr, FEE_CRCFIELD_OFFSET - FEE_SECTION_MANAGEMENT_OFFSET , Crc);
}


#define FEE_STOP_SEC_CODE
#include <MemMap.h>

/*==================[end of file]===========================================*/
