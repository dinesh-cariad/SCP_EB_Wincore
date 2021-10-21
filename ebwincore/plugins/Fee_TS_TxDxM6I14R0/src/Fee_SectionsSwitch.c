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

#include <Fee_Internal.h>        /* Internal function prototypes and internal defines and declarations*/
#include <TSMem.h>               /* Memory Routines */
#include <Fee_SectionsSwitch.h>  /* Not needed. Included only to prevent warning. */

/*==================[macros]=================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/
#define FEE_START_SEC_CODE
#include <MemMap.h>

/*--------------------------[Fee_SfOnEntrySSCopyInfo]------------------------*/

FUNC(void, FEE_CODE) Fee_SfOnEntrySSCopyInfo
(
  void
)
{
  const Fls_AddressType TargetAddressInFlash =  FEE_CONSISTENCY_PATTERN_SIZE +
#if (FEE_CONSISTENCY_START_PATTERN_ENABLED == STD_ON)
                                               FEE_BLOCKINFO_ALIGNED_TOTAL_SIZE +
                                               FEE_WRITTEN_MARKER_SIZE +
#endif
                                               Fee_Gv.FeeWorkingInfoAddress;
  const Fls_AddressType TargetDataAddress =
#if (FEE_CONSISTENCY_START_PATTERN_ENABLED == STD_ON)
                                               FEE_CONSISTENCY_PATTERN_SIZE+
#endif
                                               Fee_Gv.FeeDataAddress;
  if (Fee_Gv.FeeSwitchJob == FEE_INVALIDATE_JOB)
  {
    Fee_FillBlockInfo(Fee_BlockCfgPtr->FeeBlockNumber,
                      FEE_INVALID_SIZE,
                      FEE_INVALID_BLOCKADDRESS(Fee_Gv.FeeActiveSectionIndex),
                      FEE_SWITCH_JOB
                     );
  }
  else
  {
    Fee_FillBlockInfo(Fee_BlockCfgPtr->FeeBlockNumber,
                      FEE_COPY_BLOCK_SIZE(),
                      TargetDataAddress,
                      FEE_SWITCH_JOB
                     );
  }

  /* write block info to flash */
  if (E_OK == Fls_Write(TargetAddressInFlash,
                        Fee_Gv.FeeBuffPtr,
                        FEE_BLOCKINFO_ALIGNED_SIZE
                       )
     )
  {
    /* Reset retry counter */
    Fee_Gv.FeeRetryStateCounter = FEE_STATE_MAX_RETRIES;
#if (FEE_CONSISTENCY_START_PATTERN_ENABLED == STD_OFF)
    /* If flash write is accepted, update the Info Address */
    Fee_Gv.FeeWorkingInfoAddress -= (FEE_CONSISTENCY_PATTERN_SIZE + FEE_BLOCKINFO_ALIGNED_SIZE + FEE_WRITTEN_MARKER_SIZE);
    /* The block info address must be updated to always be the newest empty one */
    Fee_Gv.FeeInfoAddress = Fee_Gv.FeeWorkingInfoAddress;
#endif
    Fee_Gv.FeeFlashJobResult = MEMIF_JOB_PENDING;
  }

  /* flash write was a failure */
  else
  {
    Fee_FlsJobRefused();
  }
}

/*--------------------------[Fee_SfSSCopyInfo]-------------------------------*/

FUNC(void, FEE_CODE) Fee_SfSSCopyInfo
(
  void
)
{
  FEE_GET_FLASH_JOB_RESULT();

  if (MEMIF_JOB_PENDING == Fee_Gv.FeeFlashJobResult)
  {
    /* Do nothing; flash job is not complete */
  }
  /* Copy of block info finished without error */
  else if ((MEMIF_JOB_OK == Fee_Gv.FeeFlashJobResult) && (!FEE_CHECK_FLAG(FEE_FLASH_FINAL_REFUSAL_MASK)))
  {
    Fee_Gv.Fee_NumberOfMarginBlockInfos = 0U;
#if (FEE_CONSISTENCY_PATTERN_ENABLED == STD_OFF)

    /* For an invalidated block there is no need to copy the data block */
    if(Fee_Gv.FeeSwitchJob == FEE_INVALIDATE_JOB)
    {
      if(FEE_CHECK_FLAG(FEE_HIGHEST_INTERNAL_PRIORITY_MASK))
      {
        Fee_Gv.FeeSwitchRequiredSpace -= FEE_BLOCKINFO_PLUS_WM_SIZE_AL;
        /* Save the old address in case the switch is restarted, so the rollback could be done. */
        Fee_Gv.FeeSwRestartCache[Fee_Gv.FeeCopyBlockIndex] = Fee_Gv.FeeCache[Fee_Gv.FeeCopyBlockIndex];
        Fee_Gv.FeeCache[Fee_Gv.FeeCopyBlockIndex] = FEE_INVALID_BLOCKADDRESS(Fee_Gv.FeeActiveSectionIndex);
        Fee_SectionSwitching();
      }
      else
      {
        Fee_Gv.FeeCache[Fee_Gv.FeeCopyBlockIndex] = FEE_INVALID_BLOCKADDRESS(Fee_Gv.FeeActiveSectionIndex);
        Fee_Gv.TriggerFunct = &Fee_SectionSwitching;
        FEE_TRIGGER_TRANS(FEE_TRIGGER_JOB_OR_INTERNAL);
      }
    }
    /* Block info was copied for a valid block; copy the block data */
    else
    {
      /* Block info has been successfully copied. Reserve the space for switching the data. */
      Fee_Gv.FeeDataAddress += FEE_COPY_BLOCK_SIZE_AL();

      if ((Fee_Gv.FeeJob != FEE_NO_JOB) && (!FEE_CHECK_FLAG(FEE_HIGHEST_INTERNAL_PRIORITY_MASK)))
      {
        /* !LINKSTO Fee.Dns.SS.Behaviour.SSResume.BlockSwitching.Writing,1 */
        Fee_Gv.FeeSwitchState = FEE_SS_READ_DATA;
        /* Fee_Gv.FeeJob will be updated when a job request is posted
         * Initiate the requested job */
        Fee_InitiateJob();
      }
      else
      {
        FEE_TRIGGER_TRANS(FEE_SS_READ_DATA);
      }
    }
#else
      /* !LINKSTO Fee.ConsistencyStartPattern.BlockData.Write,1 */
      /* Write the block data start pattern */
      FEE_TRIGGER_TRANS(FEE_SS_WRITE_BLOCK_INFO_END_PATTERN);
#endif
  }
  /* Copy of block info failed */
  else
  {
    if(TRUE == Fee_FlsJobFailed())
    {
      /* !LINKSTO Fee.Dns.FailureHandling.Behaviour.SectionSwitch.WriteBlockInfo,1 */
      /* If start patterns are used and Fee reached this point, it means that the start pattern was completely written. */
#if (FEE_CONSISTENCY_START_PATTERN_ENABLED == STD_OFF)
      /* Failure occurred */
      /* !LINKSTO Fee.Dns.FailureHandling.Behaviour.SectionSwitch.BlockInfoMargin,1 */
      Fee_Gv.Fee_NumberOfMarginBlockInfos++;
#if (FEE_CRITICAL_BLOCK_NUMBER != 0U)
      if(Fee_Gv.Fee_NumberOfMarginBlockInfos == (FEE_DATA_INFO_MARGIN - 1U))
      {
        Fee_JobTriggersSectionChange();
      }
#else
      if(Fee_Gv.Fee_NumberOfMarginBlockInfos == FEE_DATA_INFO_MARGIN)
      {
        /* Consider the section full. FEE_DATA_INFO_MARGIN - 1U is because info address is already updated. */
        Fee_Gv.FeeDataAddress = (Fee_Gv.FeeInfoAddress - ((FEE_DATA_INFO_MARGIN - 1U)*FEE_BLOCKINFO_PLUS_WM_SIZE_AL));
      }
#endif /* (FEE_CRITICAL_BLOCK_NUMBER != 0U) */
      else
      {
        /* There is still space for new block infos. */
      }
#endif
    }

    /* Fee will retry to switch the block that failed.
     * Make sure that the next internal operation is done on a different MF,
     * in order to let the system to respond and recover after failure. */
    Fee_Gv.TriggerFunct = &Fee_SectionSwitching;
    FEE_TRIGGER_TRANS(FEE_STATE_WAIT_A_CYCLE);
  }
}

/*--------------------------[Fee_SfOnEntrySSReadData]------------------------*/

FUNC(void, FEE_CODE) Fee_SfOnEntrySSReadData
(
  void
)
{
  if(Fee_Gv.FeeSwitchBlockRemSize >= FEE_BUFFER_SIZE)
  {
    Fee_Gv.FeeSwitchBlockSizeToCopy = FEE_BUFFER_SIZE;
  }
  else
  {
    Fee_Gv.FeeSwitchBlockSizeToCopy = Fee_Gv.FeeSwitchBlockRemSize;
  }
  /* Read the block data, block number and patterns (if enabled) in one Fls_Read call */
  if (E_OK == Fls_Read(Fee_Gv.FeeSwitchSourceAddress,
                       Fee_Gv.FeeBuffPtr,
                       (Fls_LengthType)Fee_Gv.FeeSwitchBlockSizeToCopy
                      )
     )
  {
    /* Reset retry counter */
    Fee_Gv.FeeRetryStateCounter = FEE_STATE_MAX_RETRIES;
    Fee_Gv.FeeFlashJobResult = MEMIF_JOB_PENDING;
  }

  /* flash read failed */
  else
  {
    Fee_FlsJobRefused();
  }
}

/*--------------------------[Fee_SfSSReadData]-------------------------------*/

FUNC(void, FEE_CODE) Fee_SfSSReadData
(
  void
)
{

  FEE_GET_FLASH_JOB_RESULT();

  if (MEMIF_JOB_PENDING == Fee_Gv.FeeFlashJobResult)
  {
#if (FEE_FLS_CANCEL_API == STD_ON)
    if ((Fee_Gv.FeeJob != FEE_NO_JOB) && (!FEE_CHECK_FLAG(FEE_HIGHEST_INTERNAL_PRIORITY_MASK)))
    {
      /* A job has been requested. Cancel the ongoing flash read! */
      Fls_Cancel();
      /* !LINKSTO Fee.Dns.SS.Behaviour.SSResume.BlockSwitching.Reading,1 */
      Fee_Gv.FeeSwitchState = FEE_SS_READ_DATA;
      /* Fee_Gv.FeeJob will be updated when a job request is posted
       * Initiate the requested job */
      Fee_InitiateJob();
    }
    else
    {
      /* No job requested. */
    }
#endif
    /* do nothing, flash job is not completed */
  }

  /* If block data and id read finished without error */
  else if ((MEMIF_JOB_OK == Fee_Gv.FeeFlashJobResult) && (!FEE_CHECK_FLAG(FEE_FLASH_FINAL_REFUSAL_MASK)))
  {
    if ((Fee_Gv.FeeJob != FEE_NO_JOB) && (!FEE_CHECK_FLAG(FEE_HIGHEST_INTERNAL_PRIORITY_MASK)))
    {
      /* !LINKSTO Fee.Dns.SS.Behaviour.SSResume.BlockSwitching.Reading,1 */
      Fee_Gv.FeeSwitchState = FEE_SS_READ_DATA;
      /* Fee_Gv.FeeJob will be updated when a job request is posted
       * Initiate the requested job */
      Fee_InitiateJob();
    }
    else
    {
      FEE_TRIGGER_TRANS(FEE_SS_COPY_DATA);
    }
  }

  /* flash job was a failure */
  else
  {
    Fee_Gv.FeeSwitchState = FEE_STATE_INVALID;
    if(TRUE == Fee_FlsJobFailed())
    {
      /* !LINKSTO Fee.Dns.FailureHandling.Behaviour.SectionSwitch.BlockDataRead,1 */
      if(FEE_CHECK_FLAG(FEE_SS_READ_FAIL_RETRY_MASK))
      {
        FEE_RESET_FLAG(FEE_SS_READ_FAIL_RETRY_MASK);
        /* The block could not be read, therefore it is lost. (User document must contain this) */
        Fee_Gv.FeeCache[Fee_Gv.FeeCopyBlockIndex] = FEE_INCONSISTENT_BLOCKADDRESS(Fee_Gv.FeeActiveSectionIndex);
      }
      else
      {
        FEE_SET_FLAG(FEE_SS_READ_FAIL_RETRY_MASK);
        /* Retry will be performed */
        Fee_Gv.FeeSwitchState = FEE_SS_READ_DATA;
      }
    }

    /* Fee will retry to switch the block that failed (only in case of read retry).
     * Make sure that the next internal operation is done on a different MF,
     * in order to let the system to respond and recover after failure. */
    Fee_Gv.TriggerFunct = &Fee_SectionSwitching;
    FEE_TRIGGER_TRANS(FEE_STATE_WAIT_A_CYCLE);
  }
}

/*--------------------------[Fee_SfOnEntrySSCopyData]------------------------*/

FUNC(void, FEE_CODE) Fee_SfOnEntrySSCopyData
(
  void
)
{
  const Fls_LengthType BlockSizeToCopyAlligned = FEE_ALLIGNED_TO_VIRTUAL_PAGE(Fee_Gv.FeeSwitchBlockSizeToCopy);
  TS_MemSet(&Fee_Gv.FeeBuffPtr[Fee_Gv.FeeSwitchBlockSizeToCopy], FEE_PAD_BYTE, BlockSizeToCopyAlligned - Fee_Gv.FeeSwitchBlockSizeToCopy);

  if(E_OK == Fls_Write(Fee_Gv.FeeWorkingDataAddress,
      Fee_Gv.FeeBuffPtr,
      BlockSizeToCopyAlligned)
  )
  {
    /* Reset retry counter */
    Fee_Gv.FeeRetryStateCounter = FEE_STATE_MAX_RETRIES;
    Fee_Gv.FeeSwitchBlockRemSize -= Fee_Gv.FeeSwitchBlockSizeToCopy;
    Fee_Gv.FeeWorkingDataAddress += (Fls_AddressType)BlockSizeToCopyAlligned;
    Fee_Gv.FeeFlashJobResult = MEMIF_JOB_PENDING;
  }

  /* flash write failed */
  else
  {
    Fee_FlsJobRefused();
  }
}

/*--------------------------[Fee_SfSSCopyData]-------------------------------*/

FUNC(void, FEE_CODE) Fee_SfSSCopyData
(
  void
)
{
  FEE_GET_FLASH_JOB_RESULT();

  if (MEMIF_JOB_PENDING == Fee_Gv.FeeFlashJobResult)
  {
    /* Do nothing; flash job is not complete */
  }

  /* Copy of block data was successful */
  else if ((MEMIF_JOB_OK == Fee_Gv.FeeFlashJobResult) && (!FEE_CHECK_FLAG(FEE_FLASH_FINAL_REFUSAL_MASK)))
  {
    if(Fee_Gv.FeeSwitchBlockRemSize == 0U)
    {
      Fee_Gv.FeeSwitchBlockSizeToCopy = 0U;
      /* The block is completely copied */
      FEE_TRIGGER_TRANS(FEE_SS_WRITTEN_MARKER);
    }
    else
    {
      /* !LINKSTO Fee.Dns.SS.Behaviour.SSResume.BlockSwitching.Writing,1 */
      Fee_Gv.FeeSwitchSourceAddress += (Fls_AddressType)Fee_Gv.FeeSwitchBlockSizeToCopy;

      if ((Fee_Gv.FeeJob != FEE_NO_JOB) && (!FEE_CHECK_FLAG(FEE_HIGHEST_INTERNAL_PRIORITY_MASK)))
      {
        /* !LINKSTO Fee.Dns.SS.Behaviour.SSResume.BlockSwitching.Writing,1 */
        Fee_Gv.FeeSwitchState = FEE_SS_READ_DATA;
        /* Fee_Gv.FeeJob will be updated when a job request is posted
         * Initiate the requested job */
        Fee_InitiateJob();
      }
      else
      {
        FEE_TRIGGER_TRANS(FEE_SS_READ_DATA);
      }
    }
  }
  /* Copy of data block failed */
  else
  {
    Fee_Gv.FeeSwitchState = FEE_STATE_INVALID;
    if(FALSE == Fee_FlsJobFailed())
    {
      /* !LINKSTO Fee.Dns.FailureHandling.Behaviour.SectionSwitch.BlockDataWrite,1 */
      /* Since the copy block data was not accepted there is no point in keeping the reserved space any more. */
      /* DataAddress is decreased with the current operation(usually a buffer size) plus what remained to be switched from this block */
      Fee_Gv.FeeDataAddress -= FEE_ALLIGNED_TO_VIRTUAL_PAGE(Fee_Gv.FeeSwitchBlockRemSize)
                             + FEE_ALLIGNED_TO_VIRTUAL_PAGE(Fee_Gv.FeeSwitchBlockSizeToCopy);
    }
    else
    {
      /* No job has come in between the block switching steps. */
      if(FEE_ALLIGNED_TO_VIRTUAL_PAGE((Fee_Gv.FeeWorkingDataAddress + Fee_Gv.FeeSwitchBlockRemSize)) == Fee_Gv.FeeDataAddress)
      {
        /* Save the empty space for further write operations. */
        Fee_Gv.FeeDataAddress -= FEE_ALLIGNED_TO_VIRTUAL_PAGE(Fee_Gv.FeeSwitchBlockRemSize);
      }
      else
      {
        /* FeeDataAddress is updated due to an user job. */
      }
    }

    /* Fee will retry to switch the block that failed.
     * Make sure that the next internal operation is done on a different MF,
     * in order to let the system to respond and recover after failure. */
    Fee_Gv.TriggerFunct = &Fee_SectionSwitching;
    FEE_TRIGGER_TRANS(FEE_STATE_WAIT_A_CYCLE);
  }
}

/*--------------------------[Fee_SfOnEntrySSWriteWrittenMarker]-------------------*/

FUNC(void, FEE_CODE) Fee_SfOnEntrySSWriteWrittenMarker(void)
{
  /* !LINKSTO Fee.Dsn.Initialization.Startup1.BlockInfoValidity.WrittenMarker,1 */
  Fls_AddressType TargetAddressInFlash = Fee_Gv.FeeWorkingInfoAddress +
              (FEE_BLOCKINFO_PLUS_WM_SIZE_AL) + FEE_BLOCKINFO_ALIGNED_TOTAL_SIZE;

  /* Write the data consistency pattern at the end of the block management information */
  if (E_OK == Fls_Write(TargetAddressInFlash,
                        &Fee_WrittenMarkerPattern[0],
                        FEE_WRITTEN_MARKER_SIZE
                       )
     )
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

/*--------------------------[Fee_SfSSWriteWrittenMarker]--------------------------*/

FUNC(void, FEE_CODE) Fee_SfSSWriteWrittenMarker(void)
{

  FEE_GET_FLASH_JOB_RESULT();

  if (MEMIF_JOB_PENDING == Fee_Gv.FeeFlashJobResult)
  {
    /* Do nothing; flash job is not complete */
  }

  /* Check whether the pattern write is succesful */
  else if ((MEMIF_JOB_OK == Fee_Gv.FeeFlashJobResult) && (!FEE_CHECK_FLAG(FEE_FLASH_FINAL_REFUSAL_MASK)))
  {
    const Fls_LengthType BlockSizeAlligned = FEE_COPY_BLOCK_SIZE_AL();
    /* Reset the switch state */
    Fee_Gv.FeeSwitchState = FEE_STATE_INVALID;
    if(FEE_CHECK_FLAG(FEE_HIGHEST_INTERNAL_PRIORITY_MASK))
    {
      Fee_Gv.FeeSwitchRequiredSpace -=
#if (FEE_CONSISTENCY_START_PATTERN_ENABLED == STD_ON)
                                     /* Data start pattern */
                                     FEE_CONSISTENCY_PATTERN_SIZE +
#endif
                                     ((Fls_LengthType)BlockSizeAlligned+
                                      (Fls_LengthType)FEE_BLOCKINFO_PLUS_WM_SIZE_AL);

    /* Save the old address in case the switch is restarted, so the rollback could be done. */
    Fee_Gv.FeeSwRestartCache[Fee_Gv.FeeCopyBlockIndex] = Fee_Gv.FeeCache[Fee_Gv.FeeCopyBlockIndex];
    /* Save the new address of the block */
    Fee_Gv.FeeCache[Fee_Gv.FeeCopyBlockIndex] = Fee_Gv.FeeWorkingDataAddress - BlockSizeAlligned;
    Fee_SectionSwitching();
    }
    else
    {
      /* Save the new address of the block */
      Fee_Gv.FeeCache[Fee_Gv.FeeCopyBlockIndex] = Fee_Gv.FeeWorkingDataAddress - BlockSizeAlligned;

      Fee_Gv.TriggerFunct = &Fee_SectionSwitching;
      FEE_TRIGGER_TRANS(FEE_TRIGGER_JOB_OR_INTERNAL);
    }
  }
  /* previous flash job has failed */
  else
  {
    /* !LINKSTO Fee.Dns.FailureHandling.Behaviour.SectionSwitch.BlockWrittenMarkerWrite,1 */
    /* Reset the switch state */
    Fee_Gv.FeeSwitchState = FEE_STATE_INVALID;
    (void)Fee_FlsJobFailed();

    /* Fee will retry to switch the block that failed.
     * Make sure that the next internal operation is done on a different MF,
     * in order to let the system to respond and recover after failure. */
    Fee_Gv.TriggerFunct = &Fee_SectionSwitching;
    FEE_TRIGGER_TRANS(FEE_STATE_WAIT_A_CYCLE);
  }
}
#if (FEE_CONSISTENCY_PATTERN_ENABLED == STD_ON)

#if (FEE_CONSISTENCY_START_PATTERN_ENABLED == STD_ON)
/*--------------------------[Fee_SfOnEntrySSWriteBlockInfoStartPattern]-----------------*/

FUNC(void, FEE_CODE) Fee_SfOnEntrySSWriteBlockInfoStartPattern(void)
{

  /* Write the data consistency pattern at the end of the block management information */
  if (E_OK == Fls_Write(Fee_Gv.FeeWorkingInfoAddress +
                        FEE_CONSISTENCY_PATTERN_SIZE +
                        FEE_BLOCKINFO_ALIGNED_SIZE,
                        &Fee_ConsistencyPattern[0],
                        FEE_CONSISTENCY_PATTERN_SIZE
                       )
     )
  {
    /* Reset retry counter */
    Fee_Gv.FeeRetryStateCounter = FEE_STATE_MAX_RETRIES;
    /* If the write request was accepted by Fls, update the info address */
    Fee_Gv.FeeWorkingInfoAddress -= (FEE_BLOCKINFO_ALIGNED_TOTAL_SIZE + FEE_WRITTEN_MARKER_SIZE);
    /* The block info address must be updated to always be the newest empty one */
    Fee_Gv.FeeInfoAddress = Fee_Gv.FeeWorkingInfoAddress;
    Fee_Gv.FeeFlashJobResult = MEMIF_JOB_PENDING;
  }
  else
  {
    Fee_FlsJobRefused();
  }
}

/*--------------------------[Fee_SfSSWriteBlockInfoStartPattern]------------------------*/

FUNC(void, FEE_CODE) Fee_SfSSWriteBlockInfoStartPattern(void)
{
  FEE_GET_FLASH_JOB_RESULT();

  if (MEMIF_JOB_PENDING == Fee_Gv.FeeFlashJobResult)
  {
    /* Do nothing; flash job is not complete */
  }

  /* Check whether the pattern write is succesful */
  else if ((MEMIF_JOB_OK == Fee_Gv.FeeFlashJobResult) && (!FEE_CHECK_FLAG(FEE_FLASH_FINAL_REFUSAL_MASK)))
  {

    FEE_TRIGGER_TRANS(FEE_SS_COPY_INFO);
  }
  /* previous flash job has failed */
  else
  {
    /* !LINKSTO Fee.Dns.FailureHandling.Behaviour.SectionSwitch.StartPattern,1 */
    if(TRUE == Fee_FlsJobFailed())
    {
      Fee_Gv.Fee_NumberOfMarginBlockInfos++;
      if(Fee_Gv.Fee_NumberOfMarginBlockInfos == FEE_DATA_INFO_MARGIN)
      {
        /* Consider the section full. FEE_DATA_INFO_MARGIN - 1U is because info address is already updated. */
        Fee_Gv.FeeDataAddress = (Fee_Gv.FeeInfoAddress - ((FEE_DATA_INFO_MARGIN - 1U)*FEE_BLOCKINFO_PLUS_WM_SIZE_AL));
      }
      else
      {
        /* There is still space for new block infos. */
      }
    }

    /* Fee will retry to switch the block that failed.
     * Make sure that the next internal operation is done on a different MF,
     * in order to let the system to respond and recover after failure. */
    Fee_Gv.TriggerFunct = &Fee_SectionSwitching;
    FEE_TRIGGER_TRANS(FEE_STATE_WAIT_A_CYCLE);
  }
}

/*--------------------------[Fee_SfOnEntrySSWriteBlockDataStartPattern]-----------------*/

FUNC(void, FEE_CODE) Fee_SfOnEntrySSWriteBlockDataStartPattern(void)
{
  /* Write the data consistency pattern at the end of the block management information */
  if (E_OK == Fls_Write(Fee_Gv.FeeWorkingDataAddress,
                        &Fee_ConsistencyPattern[0],
                        FEE_CONSISTENCY_PATTERN_SIZE
                       )
     )
  {
    /* Reset retry counter */
    Fee_Gv.FeeRetryStateCounter = FEE_STATE_MAX_RETRIES;
    Fee_Gv.FeeFlashJobResult = MEMIF_JOB_PENDING;
    /*Adjust FEE address*/
    Fee_Gv.FeeWorkingDataAddress += (Fls_AddressType)FEE_CONSISTENCY_PATTERN_SIZE;
  }
  else
  {
    /* Mark the section as failed */
    Fee_FlsJobRefused();
  }
}

/*--------------------------[Fee_SfSSWriteBlockDataStartPattern]------------------------*/

FUNC(void, FEE_CODE) Fee_SfSSWriteBlockDataStartPattern(void)
{
  FEE_GET_FLASH_JOB_RESULT();

  if (MEMIF_JOB_PENDING == Fee_Gv.FeeFlashJobResult)
  {
    /* Do nothing; flash job is not complete */
  }

  /* Check whether the pattern write is succesful */
  else if ((MEMIF_JOB_OK == Fee_Gv.FeeFlashJobResult) && (!FEE_CHECK_FLAG(FEE_FLASH_FINAL_REFUSAL_MASK)))
  {
    /* !LINKSTO Fee.ConsistencyStartPattern.BlockData.Flow.BlockData,1 */
    /* Copy the block data */
    FEE_TRIGGER_TRANS(FEE_SS_READ_DATA);
  }

  /* previous flash job has failed */
  else
  {
    Fee_Gv.FeeSwitchState = FEE_STATE_INVALID;
    /* !LINKSTO Fee.Dns.FailureHandling.Behaviour.SectionSwitch.StartPattern,1 */
    if(TRUE == Fee_FlsJobFailed())
    {
      /* No job has come in between the block switching steps. */
      if(FEE_ALLIGNED_TO_VIRTUAL_PAGE((Fee_Gv.FeeWorkingDataAddress + Fee_Gv.FeeSwitchBlockRemSize)) == Fee_Gv.FeeDataAddress)
      {
        /* Save the empty space for further write operations. */
        Fee_Gv.FeeDataAddress -= FEE_COPY_BLOCK_SIZE_AL();
      }
      else
      {
        /* FeeDataAddress is updated due to an user job. */
      }
    }

    /* Fee will retry to switch the block that failed.
     * Make sure that the next internal operation is done on a different MF,
     * in order to let the system to respond and recover after failure. */
    Fee_Gv.TriggerFunct = &Fee_SectionSwitching;
    FEE_TRIGGER_TRANS(FEE_STATE_WAIT_A_CYCLE);
  }
}

#endif

/*--------------------------[Fee_SfOnEntrySSWriteBlockInfoEndPattern]-------------------*/

FUNC(void, FEE_CODE) Fee_SfOnEntrySSWriteBlockInfoEndPattern(void)
{
  const Fls_AddressType TargetAddressInFlash = FEE_BLOCKINFO_ALIGNED_TOTAL_SIZE +
                                               FEE_WRITTEN_MARKER_SIZE +
                                               Fee_Gv.FeeWorkingInfoAddress;

  /* Write the data consistency pattern at the begginging of the block management information */
  if (E_OK == Fls_Write(TargetAddressInFlash,
                        &Fee_ConsistencyPattern[0],
                        FEE_CONSISTENCY_PATTERN_SIZE
                       )
     )
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

/*--------------------------[Fee_SfSSWriteBlockInfoEndPattern]---------------------*/

FUNC(void, FEE_CODE) Fee_SfSSWriteBlockInfoEndPattern(void)
{
  Fee_State_t NextSwitchState;
  FEE_GET_FLASH_JOB_RESULT();

  if (MEMIF_JOB_PENDING == Fee_Gv.FeeFlashJobResult)
  {
    /* Do nothing; flash job is not complete */
  }

  /* Check whether the pattern write is succesful */
  else if ((MEMIF_JOB_OK == Fee_Gv.FeeFlashJobResult) && (!FEE_CHECK_FLAG(FEE_FLASH_FINAL_REFUSAL_MASK)))
  {
    /* For an invalid or inconsistent block, no need to copy the data block; so check whether
     * the last copied info was for an inconsistent block */
    if(Fee_Gv.FeeSwitchJob == FEE_INVALIDATE_JOB)
    {
      if(FEE_CHECK_FLAG(FEE_HIGHEST_INTERNAL_PRIORITY_MASK))
      {
        Fee_Gv.FeeSwitchRequiredSpace -= FEE_BLOCKINFO_PLUS_WM_SIZE_AL;
        /* Save the old address in case the switch is restarted, so the rollback could be done. */
        Fee_Gv.FeeSwRestartCache[Fee_Gv.FeeCopyBlockIndex] = Fee_Gv.FeeCache[Fee_Gv.FeeCopyBlockIndex];
        Fee_Gv.FeeCache[Fee_Gv.FeeCopyBlockIndex] = FEE_INVALID_BLOCKADDRESS(Fee_Gv.FeeActiveSectionIndex);
        Fee_SectionSwitching();
      }
      else
      {
        Fee_Gv.FeeCache[Fee_Gv.FeeCopyBlockIndex] = FEE_INVALID_BLOCKADDRESS(Fee_Gv.FeeActiveSectionIndex);
        Fee_Gv.TriggerFunct = &Fee_SectionSwitching;
        FEE_TRIGGER_TRANS(FEE_TRIGGER_JOB_OR_INTERNAL);
      }
    }
    /* Block info was copied for a valid block; copy the block data */
    else
    {
      const Fls_LengthType BlockSizeAlligned = FEE_COPY_BLOCK_SIZE_AL();
      /* Block info has been successfully copied. Reserve the space for switching the data. */
#if (FEE_CONSISTENCY_START_PATTERN_ENABLED == STD_OFF)
      Fee_Gv.FeeDataAddress += BlockSizeAlligned;
#else
      Fee_Gv.FeeDataAddress += (Fls_AddressType)((uint32)BlockSizeAlligned + (uint32)FEE_CONSISTENCY_PATTERN_SIZE);
#endif
#if (FEE_CONSISTENCY_START_PATTERN_ENABLED == STD_OFF)
      NextSwitchState = FEE_SS_READ_DATA;
#else
      /* !LINKSTO Fee.ConsistencyStartPattern.BlockData.Write,1 */
      /* Write the block data start pattern */
      NextSwitchState = FEE_SS_WRITE_BLOCK_DATA_START_PATTERN;
#endif

      if ((Fee_Gv.FeeJob != FEE_NO_JOB) && (!FEE_CHECK_FLAG(FEE_HIGHEST_INTERNAL_PRIORITY_MASK)))
      {
        /* !LINKSTO Fee.Dns.SS.Behaviour.SSResume.BlockSwitching.Writing,1 */
        Fee_Gv.FeeSwitchState = NextSwitchState;
        /* Fee_Gv.FeeJob will be updated when a job request is posted
         * Initiate the requested job */
        Fee_InitiateJob();
      }
      else
      {
        /* Switch is not interrupted. No switch state to memorize. */
        Fee_Gv.FeeSwitchState = FEE_STATE_INVALID;
        FEE_TRIGGER_TRANS(NextSwitchState);
      }
    }
  }
  /* previous flash job has failed */
  else
  {
    /* !LINKSTO Fee.Dns.FailureHandling.Behaviour.SectionSwitch.EndPattern,1 */
    (void)Fee_FlsJobFailed();

    /* Fee will retry to switch the block that failed.
     * Make sure that the next internal operation is done on a different MF,
     * in order to let the system to respond and recover after failure. */
    Fee_Gv.TriggerFunct = &Fee_SectionSwitching;
    FEE_TRIGGER_TRANS(FEE_STATE_WAIT_A_CYCLE);
  }
}
#endif

#define FEE_STOP_SEC_CODE
#include <MemMap.h>

/*==================[internal constants]====================================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
