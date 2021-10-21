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
/* This file contains code for jobs operations */
/*==================[inclusions]============================================*/

#include <Fee_Internal.h>     /* Internal function prototypes and internal defines and declarations*/
#include <TSMem.h>            /* Memory Routines */
#include <Fee_Jobs.h>         /* Not needed. Included only to prevent warning. */

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

/*--------------------------[Fee_SfOnEntryInvalidate]------------------------*/

FUNC(void, FEE_CODE) Fee_SfOnEntryInvalidate
(
  void
)
{
  const Fls_AddressType TargetAddressInFlash = FEE_CONSISTENCY_PATTERN_SIZE +
#if (FEE_CONSISTENCY_START_PATTERN_ENABLED == STD_ON)
                                               FEE_BLOCKINFO_ALIGNED_TOTAL_SIZE +
                                               FEE_WRITTEN_MARKER_SIZE +
#endif
                                               Fee_Gv.FeeInfoAddress;

  Fee_FillBlockInfo(Fee_BlockCfg[Fee_Gv.FeeIntBlockIndex].FeeBlockNumber,
                    FEE_INVALID_SIZE,
                    FEE_INVALID_BLOCKADDRESS(Fee_Gv.FeeActiveSectionIndex),
                    FEE_USER_JOB
                   );
  /* Call flash write to initiate the write of block info*/
  if (E_OK == Fls_Write(TargetAddressInFlash,
                        Fee_Gv.FeeBuffPtr,
                        FEE_BLOCKINFO_ALIGNED_SIZE
                       )
     )
  {
    /* Reset retry counter */
    Fee_Gv.FeeRetryStateCounter = FEE_STATE_MAX_RETRIES;
#if (FEE_CONSISTENCY_START_PATTERN_ENABLED == STD_OFF)
    /* Update the Block Info Address */
    Fee_Gv.FeeInfoAddress -= (FEE_BLOCKINFO_ALIGNED_TOTAL_SIZE + FEE_WRITTEN_MARKER_SIZE);
#endif

    Fee_Gv.FeeFlashJobResult = MEMIF_JOB_PENDING;
  }

  /* flash write failed */
  else
  {
    Fee_FlsJobRefused();
  }
}

/*--------------------------[Fee_SfOnEntryEraseImmediateBlock]------------------------*/

FUNC(void, FEE_CODE) Fee_SfOnEntryEraseImmediateBlock
(
  void
)
{
  const Fls_AddressType TargetAddressInFlash = FEE_CONSISTENCY_PATTERN_SIZE +
#if (FEE_CONSISTENCY_START_PATTERN_ENABLED == STD_ON)
                                               FEE_BLOCKINFO_ALIGNED_TOTAL_SIZE +
                                               FEE_WRITTEN_MARKER_SIZE +
#endif
                                               Fee_Gv.FeeInfoAddress;
  Fee_FillBlockInfo(Fee_BlockCfg[Fee_Gv.FeeIntBlockIndex].FeeBlockNumber,
                    FEE_INVALID_SIZE,
                    FEE_INCONSISTENT_BLOCKADDRESS(Fee_Gv.FeeActiveSectionIndex),
                    FEE_USER_JOB
                   );

  /* Call flash write to initiate the write of block info */
  if (E_OK == Fls_Write(TargetAddressInFlash,
                        Fee_Gv.FeeBuffPtr,
                        FEE_BLOCKINFO_ALIGNED_SIZE
                       )
     )
  {
    /* Reset retry counter */
    Fee_Gv.FeeRetryStateCounter = FEE_STATE_MAX_RETRIES;
#if (FEE_CONSISTENCY_START_PATTERN_ENABLED == STD_OFF)
    /* Update the Block Info Address */
    Fee_Gv.FeeInfoAddress -= (FEE_BLOCKINFO_ALIGNED_TOTAL_SIZE + FEE_WRITTEN_MARKER_SIZE);
#endif

    Fee_Gv.FeeFlashJobResult = MEMIF_JOB_PENDING;
  }

  /* flash write failed */
  else
  {
    Fee_FlsJobRefused();
  }
}

/*--------------------------[Fee_SfOnEntryWriteBlockInfo]--------------------*/

FUNC(void, FEE_CODE) Fee_SfOnEntryWriteBlockInfo
(
  void
)
{
  const Fls_AddressType TargetAddressInFlash = FEE_CONSISTENCY_PATTERN_SIZE +
#if (FEE_CONSISTENCY_START_PATTERN_ENABLED == STD_ON)
                                               FEE_BLOCKINFO_ALIGNED_TOTAL_SIZE +
                                               FEE_WRITTEN_MARKER_SIZE +
#endif
                                               Fee_Gv.FeeInfoAddress;
  const Fls_AddressType TargetAddressInCache =
#if (FEE_CONSISTENCY_START_PATTERN_ENABLED == STD_ON)
                                               FEE_CONSISTENCY_PATTERN_SIZE +
#endif
                                              Fee_Gv.FeeDataAddress;
  Fee_FillBlockInfo(FEE_BLOCK_CFG(Fee_Gv.FeeIntBlockIndex)->FeeBlockNumber,
                    FEE_BLOCK_CFG(Fee_Gv.FeeIntBlockIndex)->FeeBlockSize,
                    TargetAddressInCache,
                    FEE_USER_JOB
                   );

  if (E_OK == Fls_Write(TargetAddressInFlash,
                        Fee_Gv.FeeBuffPtr,
                        FEE_BLOCKINFO_ALIGNED_SIZE
                       )
     )
  {
    /* Reset retry counter */
    Fee_Gv.FeeRetryStateCounter = FEE_STATE_MAX_RETRIES;
#if (FEE_CONSISTENCY_START_PATTERN_ENABLED == STD_OFF)
    /* !LINKSTO Fee.Dns.FailureHandling.Behaviour.StoreReqFailure,1 */
    /* Update the Block Info Address */
    Fee_Gv.FeeInfoAddress -= (FEE_BLOCKINFO_ALIGNED_TOTAL_SIZE + FEE_WRITTEN_MARKER_SIZE);
#endif

    Fee_Gv.FeeFlashJobResult = MEMIF_JOB_PENDING;
  }

  /* flash write failed */
  else
  {
    Fee_FlsJobRefused();
  }
}

/*--------------------------[Fee_SfWriteBlockInfo]---------------------------*/

FUNC(void, FEE_CODE) Fee_SfWriteBlockInfo
(
  void
)
{
  FEE_GET_FLASH_JOB_RESULT();

  if (MEMIF_JOB_PENDING == Fee_Gv.FeeFlashJobResult)
  {
    /* Do nothing; flash job is not complete */
  }

  /* If last block info write was successful */
  else if (MEMIF_JOB_OK == Fee_Gv.FeeFlashJobResult)
  {
#if (FEE_CONSISTENCY_PATTERN_ENABLED == STD_OFF)
    /* Reset the number of failed block infos */
    Fee_Gv.Fee_NumberOfMarginBlockInfos = 0;

    switch(Fee_Gv.FeeJob)
    {

      case FEE_WRITE_JOB:
        /* !LINKSTO FEE153,1 */
        Fee_Gv.FeeCache[Fee_Gv.FeeIntBlockIndex] = (Fls_AddressType)FEE_INCONSISTENT_BLOCKADDRESS(Fee_Gv.FeeActiveSectionIndex);
        /* Write the block data */
        FEE_TRIGGER_TRANS(FEE_WRITE_BLOCK_DATA);
        break;

      /* If the job pending is Invalidate */
      case FEE_INVALIDATE_JOB:
        Fee_Gv.FeeCache[Fee_Gv.FeeIntBlockIndex] = (Fls_AddressType)FEE_INVALID_BLOCKADDRESS(Fee_Gv.FeeActiveSectionIndex);
        Fee_JobEnd();
        break;

      /* If the job pending is Erase */
      case FEE_ERASE_IMMEDIATE_JOB:
        Fee_Gv.FeeCache[Fee_Gv.FeeIntBlockIndex] = (Fls_AddressType)FEE_INCONSISTENT_BLOCKADDRESS(Fee_Gv.FeeActiveSectionIndex);
        Fee_JobEnd();
        break;

        /* CHECK: NOPARSE */
      default:
        FEE_UNREACHABLE_CODE_ASSERT(FEE_INTERNAL_API_ID);
        break; /* Caution: this 'break' is required by MISRA-C:2004 */
        /* CHECK: PARSE */
    }
#else
    /* !LINKSTO Fee.ConsistencyPattern.BlockManagementInformation.Write,1,
                Fee.ConsistencyPattern.BlockManagementInformation.Flow.EndPattern,1 */
    /* Write the block info end pattern */
    FEE_TRIGGER_TRANS(FEE_WRITE_BLOCK_INFO_END_PATTERN);
#endif
  }
  /* The write of block info failed */
  else
  {
#if (FEE_CRITICAL_BLOCK_NUMBER != 0U)
    if(Fee_Gv.FeeInfoAddressCritical != 0U)
    {
      /* Do nothing. Critical block failed in critical section. */
    }
#if (FEE_NUMBER_OF_SECTIONS > 2U)
    else if ((FEE_DATA_ADDRESS(Fee_Gv.FeeActiveSectionIndex) == Fee_Gv.FeeDataAddress) &&
        (FEE_BLOCK_CFG(Fee_Gv.FeeIntBlockIndex)->FeeBlockNumber == FEE_CRITICAL_BLOCK_NUMBER))
    {
      /* Critical block changed section. No Active marker. Consider the section full.*/
      /* !LINKSTO Fee.Dsn.Critical.SectionFull,1 */
      Fee_JobTriggersSectionChange();
    }
#endif /* (FEE_NUMBER_OF_SECTIONS > 2U) */
    else
#endif /* (FEE_CRITICAL_BLOCK_NUMBER != 0U) */
    {
      /* !LINKSTO Fee.Dns.FailureHandling.Behaviour.BlockInfoMargin,1 */
      Fee_Gv.Fee_NumberOfMarginBlockInfos++;
#if (FEE_CRITICAL_BLOCK_NUMBER != 0U)
      if(Fee_Gv.Fee_NumberOfMarginBlockInfos == (FEE_DATA_INFO_MARGIN - 1U))
      {
        /* !LINKSTO Fee.Dsn.Critical.SectionFull,1 */
        Fee_JobTriggersSectionChange();
      }
#else
      if(Fee_Gv.Fee_NumberOfMarginBlockInfos == FEE_DATA_INFO_MARGIN)
      {
        /* !LINKSTO Fee.Dns.FailureHandling.Behaviour.StoreReqFailure,1 */
        /* Consider the section full. FEE_DATA_INFO_MARGIN - 1U is because info address is already updated. */
        Fee_Gv.FeeDataAddress = (Fee_Gv.FeeInfoAddress - ((FEE_DATA_INFO_MARGIN - 1U)*FEE_BLOCKINFO_PLUS_WM_SIZE_AL));
      }
#endif /* (FEE_CRITICAL_BLOCK_NUMBER != 0U) */
      else
      {
        /* There is still space for new block infos. */
      }
    }
    /* !LINKSTO Fee.Dns.FailureHandling.Job,1 */
    Fee_JobError();
  }
}

/*--------------------------[Fee_SfOnEntryWriteBlockData]--------------------*/

FUNC(void, FEE_CODE) Fee_SfOnEntryWriteBlockData
(
  void
)
{
  const uint16 BlockSize = FEE_BLOCK_CFG(Fee_Gv.FeeIntBlockIndex)->FeeBlockSize;

#if FEE_FLS_ALIGNED_32 == STD_ON
  const uint16 BlockSizeAl = FEE_BLOCK_CFG(Fee_Gv.FeeIntBlockIndex)->FeeBlockAlignedSize;

  TS_MemCpy(Fee_Gv.FeeBuffPtr, Fee_Gv.FeeBlockDataWriteBuffPtr, BlockSize);
  if (E_OK == Fls_Write(Fee_Gv.FeeDataAddress,
        Fee_Gv.FeeBuffPtr,
        (Fls_LengthType)BlockSizeAl))
  {
    /* !LINKSTO Fee.Dns.FailureHandling.Behaviour.StoreReqFailure,1 */
    /* Reset retry counter */
    Fee_Gv.FeeRetryStateCounter = FEE_STATE_MAX_RETRIES;
    Fee_Gv.FeeFlashJobResult = MEMIF_JOB_PENDING;
    Fee_Gv.Fee_BufferIndex = BlockSizeAl;
    /* Update the block data address since the block info write is successful */
    Fee_Gv.FeeDataAddress += Fee_Gv.Fee_BufferIndex;
  }
  /* flash write failed */
  else
  {
    Fee_FlsJobRefused();
  }
#else /* FEE_FLS_ALIGNED_32 == STD_ON */

  const uint16 UnalignedRemaining = BlockSize % FEE_VIRTUAL_PAGE_SIZE;
  /* !LINKSTO Fee.WriteMultipleOfVP,1 */
  if((Fee_Gv.Fee_BufferIndex == 0U) && (BlockSize >= FEE_VIRTUAL_PAGE_SIZE))
  {
    if (E_OK == Fls_Write(Fee_Gv.FeeDataAddress,
        Fee_Gv.FeeBlockDataWriteBuffPtr,
        (Fls_LengthType)BlockSize - UnalignedRemaining))
    {
      /* !LINKSTO Fee.Dns.FailureHandling.Behaviour.StoreReqFailure,1 */
      /* Reset retry counter */
      Fee_Gv.FeeRetryStateCounter = FEE_STATE_MAX_RETRIES;
      /* Update buffer index for the last part of data write */
      Fee_Gv.Fee_BufferIndex = BlockSize - UnalignedRemaining;
      Fee_Gv.FeeFlashJobResult = MEMIF_JOB_PENDING;
      /* Update the block data address since the block info write is successful */
      Fee_Gv.FeeDataAddress += Fee_Gv.Fee_BufferIndex;
    }
    /* flash write failed */
    else
    {
      Fee_FlsJobRefused();
    }
  }
  else
  {
    /* Copy application buffer data to FEE buffer */
    TS_MemCpy(Fee_Gv.FeeBuffPtr,&Fee_Gv.FeeBlockDataWriteBuffPtr[Fee_Gv.Fee_BufferIndex], UnalignedRemaining);
    TS_MemSet(&Fee_Gv.FeeBuffPtr[UnalignedRemaining], FEE_PAD_BYTE, (uint32)((uint32)FEE_VIRTUAL_PAGE_SIZE - (uint32)UnalignedRemaining));
    if (E_OK == Fls_Write(Fee_Gv.FeeDataAddress,
        Fee_Gv.FeeBuffPtr,
        (Fls_LengthType)FEE_VIRTUAL_PAGE_SIZE))
    {
      /* !LINKSTO Fee.Dns.FailureHandling.Behaviour.StoreReqFailure,1 */
      /* Reset retry counter */
      Fee_Gv.FeeRetryStateCounter = FEE_STATE_MAX_RETRIES;
      Fee_Gv.FeeFlashJobResult = MEMIF_JOB_PENDING;
      /* Update the block data address since the block info write is successful */
      Fee_Gv.FeeDataAddress += FEE_VIRTUAL_PAGE_SIZE;
      /* Reset the buffer index as the data write was accepted */
      Fee_Gv.Fee_BufferIndex += FEE_VIRTUAL_PAGE_SIZE;
    }
    /* flash write failed */
    else
    {
      Fee_FlsJobRefused();
    }
  }
#endif /* FEE_FLS_ALIGNED_32 == STD_ON */
}

/*--------------------------[Fee_SfWriteBlockData]---------------------------*/

FUNC(void, FEE_CODE) Fee_SfWriteBlockData
(
  void
)
{
  const uint16 BlockAlignedSize = FEE_BLOCK_CFG(Fee_Gv.FeeIntBlockIndex)->FeeBlockAlignedSize;
  FEE_GET_FLASH_JOB_RESULT();

  if (MEMIF_JOB_PENDING == Fee_Gv.FeeFlashJobResult)
  {
    /* Do nothing; flash job is not complete */
  }

  /* If block data write was successful */
  else if (MEMIF_JOB_OK == Fee_Gv.FeeFlashJobResult)
  {
    if(Fee_Gv.Fee_BufferIndex == BlockAlignedSize)
    {
      /* !LINKSTO Fee.ConsistencyPattern.BlockData.Write,1,
                  Fee.ConsistencyPattern.BlockData.Flow.EndPattern,1 */
      /* !LINKSTO Fee.Dsn.WrittenMarker,1 */
      /* Write Written Marker (block data end pattern) */
      FEE_TRIGGER_TRANS(FEE_WRITTEN_MARKER);
    }
    else
    {
      FEE_TRIGGER_TRANS(FEE_WRITE_BLOCK_DATA);
    }
  }

  /* data write failed */
  else
  {
    /* !LINKSTO Fee.Dns.FailureHandling.Job,1 */
    Fee_JobError();
  }
}

/*--------------------------[Fee_SfOnEntryReadBlockData]---------------------*/

FUNC(void, FEE_CODE) Fee_SfOnEntryReadBlockData
(
  void
)
{
  Fls_LengthType DataLength;
#if (FEE_DYNAMIC_BLOCK_LENGTH == STD_OFF)
  DataLength = (Fls_LengthType)Fee_Gv.FeeLength;
#else
#if (FEE_NUMBER_NOT_CONFIGURED_BLOCKS > 0U)
  if (Fee_Gv.FeeIntBlockIndex >= FEE_NO_OF_CONFIGURED_BLOCKS)
  {
    DataLength = (Fls_LengthType)Fee_Gv.FeeLength;
  }
  else
#endif /* #if (FEE_NUMBER_NOT_CONFIGURED_BLOCKS > 0U) */
  {
    if((Fee_Gv.FeeBlockReadOffset + Fee_Gv.FeeLength) > Fee_Gv.FeeOldSizes[Fee_Gv.FeeIntBlockIndex])
    {
      DataLength = (Fls_LengthType)Fee_Gv.FeeOldSizes[Fee_Gv.FeeIntBlockIndex] - (Fls_LengthType)Fee_Gv.FeeBlockReadOffset;
    }
    else
    {
      DataLength = (Fls_LengthType)Fee_Gv.FeeLength;
    }
  }
#endif /* #if (FEE_DYNAMIC_BLOCK_LENGTH == STD_OFF) */
  /* !LINKSTO FEE007,1 */
#if FEE_FLS_ALIGNED_32 == STD_OFF
  if(E_OK == Fls_Read(Fee_Gv.FeeCache[Fee_Gv.FeeIntBlockIndex] + (Fls_AddressType)Fee_Gv.FeeBlockReadOffset,
                      Fee_Gv.FeeBlockDataReadBuffPtr,
                      DataLength
                     )
    )
#else
  if(E_OK == Fls_Read(Fee_Gv.FeeCache[Fee_Gv.FeeIntBlockIndex] + (Fls_AddressType)Fee_Gv.FeeBlockReadOffset,
                      Fee_Gv.FeeBuffPtr,
                      DataLength
                     )
    )
#endif
  {
    /* Reset retry counter */
    Fee_Gv.FeeRetryStateCounter = FEE_STATE_MAX_RETRIES;
    Fee_Gv.FeeFlashJobResult = MEMIF_JOB_PENDING;
  }
  /* flash job failed */
  else
  {
    Fee_FlsJobRefused();
  }
}

/*--------------------------[Fee_SfReadBlockData]----------------------------*/

FUNC(void, FEE_CODE) Fee_SfReadBlockData(void)
{
  FEE_GET_FLASH_JOB_RESULT();

  if (MEMIF_JOB_PENDING == Fee_Gv.FeeFlashJobResult)
  {
    /* Do nothing; flash job is not complete */
  }

  /* Block data read was successful */
  else if (MEMIF_JOB_OK == Fee_Gv.FeeFlashJobResult)
  {
#if (FEE_DYNAMIC_BLOCK_LENGTH == STD_ON)
#if (FEE_NUMBER_NOT_CONFIGURED_BLOCKS > 0U)
    if (Fee_Gv.FeeIntBlockIndex >= FEE_NO_OF_CONFIGURED_BLOCKS)
    {
      /* Not config block was successfully read */
#if FEE_FLS_ALIGNED_32 == STD_ON
      TS_MemCpy(Fee_Gv.FeeBlockDataReadBuffPtr,Fee_Gv.FeeBuffPtr,Fee_Gv.FeeLength);
#endif
      Fee_JobEnd();
    }
    else
#endif /* #if (FEE_NUMBER_NOT_CONFIGURED_BLOCKS > 0U) */
    {
      if (Fee_Gv.FeeOldSizes[Fee_Gv.FeeIntBlockIndex] > Fee_BlockCfg[Fee_Gv.FeeIntBlockIndex].FeeBlockSize)
      {
#if FEE_FLS_ALIGNED_32 == STD_ON
        TS_MemCpy(Fee_Gv.FeeBlockDataReadBuffPtr,Fee_Gv.FeeBuffPtr,Fee_Gv.FeeLength);
#endif
        /* Handle activities after a successful ending job */
        /* !LINKSTO Fee.FeeDynamicBlockLength.DecreasedSizeResult,1 */
        Fee_Gv.FeeJobResult = MEMIF_JOB_OK_SIZE_DECREASED;
        /* After a job is completed Fee should always transition to Idle */
        /* Call NVM's notification function */
        FEE_NVM_JOB_END_NOTIFICATION();
        /* !LINKSTO Fee.Dsn.Idle,1 */
        FEE_TRIGGER_TRANS(FEE_IDLE);
        /* Unqueue the current job */
        Fee_Gv.FeeJob = FEE_NO_JOB;
      }
      else if (Fee_Gv.FeeOldSizes[Fee_Gv.FeeIntBlockIndex] < Fee_BlockCfg[Fee_Gv.FeeIntBlockIndex].FeeBlockSize)
      {
        if((Fee_Gv.FeeBlockReadOffset + Fee_Gv.FeeLength) > Fee_Gv.FeeOldSizes[Fee_Gv.FeeIntBlockIndex])
        {
          uint8 LastDataByteNegated;
#if FEE_FLS_ALIGNED_32 == STD_ON
          TS_MemCpy(Fee_Gv.FeeBlockDataReadBuffPtr,Fee_Gv.FeeBuffPtr,(Fee_Gv.FeeOldSizes[Fee_Gv.FeeIntBlockIndex] - Fee_Gv.FeeBlockReadOffset));
#endif
          LastDataByteNegated = (uint8)~Fee_Gv.FeeBlockDataReadBuffPtr[(Fee_Gv.FeeOldSizes[Fee_Gv.FeeIntBlockIndex] - Fee_Gv.FeeBlockReadOffset) - 1U];
          /* !LINKSTO Fee.FeeDynamicBlockLength.IncreasedSizePedding,1 */
          TS_MemSet(&Fee_Gv.FeeBlockDataReadBuffPtr[Fee_Gv.FeeOldSizes[Fee_Gv.FeeIntBlockIndex] - Fee_Gv.FeeBlockReadOffset],
                    LastDataByteNegated,
                    ((Fls_LengthType)Fee_Gv.FeeLength + (Fls_LengthType)Fee_Gv.FeeBlockReadOffset) - (Fls_LengthType)Fee_Gv.FeeOldSizes[Fee_Gv.FeeIntBlockIndex]);
        }
        else
        {
#if FEE_FLS_ALIGNED_32 == STD_ON
          TS_MemCpy(Fee_Gv.FeeBlockDataReadBuffPtr,Fee_Gv.FeeBuffPtr,Fee_Gv.FeeLength);
#endif
        }
        /* Handle activities after a successful ending job */
        /* !LINKSTO Fee.FeeDynamicBlockLength.IncreasedSizeResult,1 */
        Fee_Gv.FeeJobResult = MEMIF_JOB_OK_SIZE_INCREASED;
        /* After a job is completed Fee should always transition to Idle */
        /* Call NVM's notification function */
        FEE_NVM_JOB_END_NOTIFICATION();
        /* !LINKSTO Fee.Dsn.Idle,1 */
        FEE_TRIGGER_TRANS(FEE_IDLE);
        /* Unqueue the current job */
        Fee_Gv.FeeJob = FEE_NO_JOB;
      }
      else
#endif /* #if (FEE_DYNAMIC_BLOCK_LENGTH == STD_ON) */
      {
        /* Block was successfully read */
#if FEE_FLS_ALIGNED_32 == STD_ON
        TS_MemCpy(Fee_Gv.FeeBlockDataReadBuffPtr,Fee_Gv.FeeBuffPtr,Fee_Gv.FeeLength);
#endif
        Fee_JobEnd();
      }
#if (FEE_DYNAMIC_BLOCK_LENGTH == STD_ON)
    }
#endif /* #if (FEE_DYNAMIC_BLOCK_LENGTH == STD_ON) */
  }

  /* Block data read from flash failed */
  else
  {
    /* !LINKSTO Fee.Dns.FailureHandling.Job,1 */
    Fee_JobError();
  }
}

#if (FEE_CONSISTENCY_PATTERN_ENABLED == STD_ON)

#if (FEE_CONSISTENCY_START_PATTERN_ENABLED == STD_ON)
/*--------------------------[Fee_SfOnEntryWriteBlockInfoStartPattern]-----------------*/

FUNC(void, FEE_CODE) Fee_SfOnEntryWriteBlockInfoStartPattern(void)
{

  /* Write the data consistency pattern at the end of the block management information */
  if (E_OK == Fls_Write(Fee_Gv.FeeInfoAddress +
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
    Fee_Gv.FeeInfoAddress -= (FEE_BLOCKINFO_ALIGNED_TOTAL_SIZE + FEE_WRITTEN_MARKER_SIZE);
    Fee_Gv.FeeFlashJobResult = MEMIF_JOB_PENDING;
  }
  else
  {
    /* Mark the section as failed */
    Fee_FlsJobRefused();
  }
}

/*--------------------------[Fee_SfWriteBlockInfoStartPattern]------------------------*/

FUNC(void, FEE_CODE) Fee_SfWriteBlockInfoStartPattern(void)
{
  FEE_GET_FLASH_JOB_RESULT();

  if (MEMIF_JOB_PENDING == Fee_Gv.FeeFlashJobResult)
  {
    /* Do nothing; flash job is not complete */
  }

  /* Check whether the pattern write is succesful */
  else if (MEMIF_JOB_OK == Fee_Gv.FeeFlashJobResult)
  {
    /* Check if the start pattern was written for a write request */
    if (FEE_WRITE_JOB == Fee_Gv.FeeJob)
    {
      /* !LINKSTO Fee.ConsistencyStartPattern.BlockManagementInformation.Flow.BlockInfo,1 */
      FEE_TRIGGER_TRANS(FEE_WRITE_BLOCK_INFO);
    }

    /* Check if the start pattern was written for an invalidate request */
    if (FEE_INVALIDATE_JOB == Fee_Gv.FeeJob)
    {
      /* !LINKSTO Fee.ConsistencyStartPattern.BlockManagementInformation.Flow.BlockInfo,1 */
      FEE_TRIGGER_TRANS(FEE_INVALIDATE);
    }

    /* Check if the start pattern was written for an erase request */
    if (FEE_ERASE_IMMEDIATE_JOB == Fee_Gv.FeeJob)
    {
      /* !LINKSTO Fee.ConsistencyStartPattern.BlockManagementInformation.Flow.BlockInfo,1 */
      FEE_TRIGGER_TRANS(FEE_ERASE_IMMEDIATE);
    }
  }
  /* previous flash job has failed */
  else
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
    /* !LINKSTO Fee.Dns.FailureHandling.Job,1 */
    Fee_JobError();
  }
}

/*--------------------------[Fee_SfOnEntryWriteBlockDataStartPattern]-----------------*/

FUNC(void, FEE_CODE) Fee_SfOnEntryWriteBlockDataStartPattern(void)
{
  /* Write the data consistency pattern at the end of the block management information */
  if (E_OK == Fls_Write(Fee_Gv.FeeDataAddress,
                        &Fee_ConsistencyPattern[0],
                        FEE_CONSISTENCY_PATTERN_SIZE
                       )
     )
  {
    /* Reset retry counter */
    Fee_Gv.FeeRetryStateCounter = FEE_STATE_MAX_RETRIES;
    Fee_Gv.FeeFlashJobResult = MEMIF_JOB_PENDING;
    /* Update the data address with the total data length to be written. At this point the block info
     * is written and contains a valid address (FeeBlockDataAddress).
     *
     * If the write request fails because of an underlying layer problem or the request is cancelled,
     * the flash memory needs to remain unwritten in order to have the block detected as inconsistent.
     */
    /*Adjust FEE address*/
    Fee_Gv.FeeDataAddress += (Fls_AddressType)FEE_CONSISTENCY_PATTERN_SIZE;
  }
  else
  {
    /* Mark the section as failed */
    Fee_FlsJobRefused();
  }
}

/*--------------------------[Fee_SfWriteBlockDataStartPattern]------------------------*/

FUNC(void, FEE_CODE) Fee_SfWriteBlockDataStartPattern(void)
{
  FEE_GET_FLASH_JOB_RESULT();

  if (MEMIF_JOB_PENDING == Fee_Gv.FeeFlashJobResult)
  {
    /* Do nothing; flash job is not complete */
  }

  /* Check whether the pattern write is succesful */
  else if (MEMIF_JOB_OK == Fee_Gv.FeeFlashJobResult)
  {
    /* !LINKSTO Fee.ConsistencyStartPattern.BlockData.Flow.BlockData,1 */
    /* Write the block data */
    FEE_TRIGGER_TRANS(FEE_WRITE_BLOCK_DATA);
  }

  /* previous flash job has failed */
  else
  {
    /* !LINKSTO Fee.Dns.FailureHandling.Job,1 */
    Fee_JobError();
  }
}


/*--------------------------[Fee_SfOnEntryReadBlockDataStartPattern]-----------------*/

FUNC(void, FEE_CODE) Fee_SfOnEntryReadBlockDataStartPattern(void)
{
  Fls_AddressType TargetAddressInFlash = Fee_Gv.FeeCache[Fee_Gv.FeeIntBlockIndex] - FEE_CONSISTENCY_PATTERN_SIZE;
  if(E_OK == Fls_Read(TargetAddressInFlash,
                      Fee_Gv.FeeBuffPtr,
                      ((Fls_LengthType)FEE_CONSISTENCY_PATTERN_SIZE)
                     )
    )
  {
    /* Reset retry counter */
    Fee_Gv.FeeRetryStateCounter = FEE_STATE_MAX_RETRIES;
    Fee_Gv.FeeFlashJobResult = MEMIF_JOB_PENDING;
  }
  /* flash job failed */
  else
  {
    Fee_FlsJobRefused();
  }
}

/*--------------------------[Fee_SfReadBlockDataStartPattern]------------------------*/

FUNC(void, FEE_CODE) Fee_SfReadBlockDataStartPattern(void)
{
  FEE_GET_FLASH_JOB_RESULT();

  if (MEMIF_JOB_PENDING == Fee_Gv.FeeFlashJobResult)
  {
    /* Do nothing; flash job is not complete */
  }

  /* Check whether the pattern was written succesful */
  else if (MEMIF_JOB_OK == Fee_Gv.FeeFlashJobResult)
  {
    /* !LINKSTO Fee.ConsistencyStartPattern.BlockData.Read,1 */
    if (FEE_READ_BYTE(Fee_Gv.FeeBuffPtr, FEE_INDEX_ZERO) == Fee_ConsistencyPattern[0])
    {
      FEE_TRIGGER_TRANS(FEE_READ_BLOCKDATA);
    }
    else
    {
      /* Start pattern was not written */
      Fee_Gv.FeeCache[Fee_Gv.FeeIntBlockIndex] = FEE_INCONSISTENT_BLOCKADDRESS(Fee_Gv.FeeWorkingSectionIndex);
      FEE_TRIGGER_TRANS(FEE_IDLE);
      Fee_Gv.FeeJob = FEE_NO_JOB;
      Fee_Gv.FeeJobResult = MEMIF_BLOCK_INCONSISTENT;
      FEE_NVM_JOB_ERROR_NOTIFICATION();
    }
  }

  /* previous flash job has failed */
  else
  {
    /* !LINKSTO Fee.Dns.FailureHandling.Job,1 */
    /* !LINKSTO Fee.Dns.FailureHandling.Write.StartPattern,1 */
    Fee_JobError();
  }
}
#endif

/*--------------------------[Fee_SfOnEntryWriteBlockInfoEndPattern]-------------------*/

FUNC(void, FEE_CODE) Fee_SfOnEntryWriteBlockInfoEndPattern(void)
{
  const Fls_AddressType TargetAddressInFlash = FEE_BLOCKINFO_ALIGNED_TOTAL_SIZE +
                                               FEE_WRITTEN_MARKER_SIZE +
                                               Fee_Gv.FeeInfoAddress;

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
    /* Mark the section as failed */
    Fee_FlsJobRefused();
  }
}

/*--------------------------[Fee_SfWriteBlockInfoEndPattern]---------------------*/

FUNC(void, FEE_CODE) Fee_SfWriteBlockInfoEndPattern(void)
{
  FEE_GET_FLASH_JOB_RESULT();

  if (MEMIF_JOB_PENDING == Fee_Gv.FeeFlashJobResult)
  {
    /* Do nothing; flash job is not complete */
  }

  /* Check whether the pattern write is succesful */
  else if (MEMIF_JOB_OK == Fee_Gv.FeeFlashJobResult)
  {
    /* Reset the number of failed block infos */
    Fee_Gv.Fee_NumberOfMarginBlockInfos = 0;
    /* Check if the block info end pattern was written for a write job */
    if (FEE_WRITE_JOB == Fee_Gv.FeeJob)
    {
      Fee_Gv.FeeCache[Fee_Gv.FeeIntBlockIndex] = (Fls_AddressType)FEE_INCONSISTENT_BLOCKADDRESS(Fee_Gv.FeeActiveSectionIndex);
#if (FEE_CONSISTENCY_START_PATTERN_ENABLED == STD_OFF)
      /* Write the block data */
      FEE_TRIGGER_TRANS(FEE_WRITE_BLOCK_DATA);
#else
      /* !LINKSTO Fee.ConsistencyStartPattern.BlockData.Write,1 */
      /* Write the start pattern for the block data */
      FEE_TRIGGER_TRANS(FEE_WRITE_BLOCK_DATA_START_PATTERN);
#endif
    }
    else if(FEE_INVALIDATE_JOB == Fee_Gv.FeeJob)
    {
      Fee_Gv.FeeCache[Fee_Gv.FeeIntBlockIndex] = (Fls_AddressType)FEE_INVALID_BLOCKADDRESS(Fee_Gv.FeeActiveSectionIndex);
      Fee_JobEnd();
    }
    else /*(FEE_ERASE_IMMEDIATE_JOB == Fee_Gv.FeeJob)*/
    {
      Fee_Gv.FeeCache[Fee_Gv.FeeIntBlockIndex] = (Fls_AddressType)FEE_INCONSISTENT_BLOCKADDRESS(Fee_Gv.FeeActiveSectionIndex);
      Fee_JobEnd();
    }
  }

  /* previous flash job has failed */
  else
  {
    /* !LINKSTO Fee.Dns.FailureHandling.Job,1 */
    /* !LINKSTO Fee.Dns.FailureHandling.Write.EndPattern,1 */
    Fee_JobError();
  }
}
#endif
/*--------------------------[Fee_SfOnEntryWriteWrittenMarker]-------------------*/

FUNC(void, FEE_CODE) Fee_SfOnEntryWriteWrittenMarker(void)
{
  /* !LINKSTO Fee.Dsn.Initialization.Startup1.BlockInfoValidity.WrittenMarker,1 */
  Fls_AddressType TargetAddressInFlash =  Fee_Gv.FeeInfoAddress +
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

/*--------------------------[Fee_SfWriteWrittenMarker]--------------------------*/

FUNC(void, FEE_CODE) Fee_SfWriteWrittenMarker(void)
{

  FEE_GET_FLASH_JOB_RESULT();

  if (MEMIF_JOB_PENDING == Fee_Gv.FeeFlashJobResult)
  {
    /* Do nothing; flash job is not complete */
  }

  /* Check whether the pattern write is succesful */
  else if (MEMIF_JOB_OK == Fee_Gv.FeeFlashJobResult)
  {
    /* !LINKSTO FEE154,1 */
    /* User write job finished */
    Fee_Gv.FeeCache[Fee_Gv.FeeIntBlockIndex] = Fee_Gv.FeeDataAddress -
                                                (Fls_AddressType)FEE_BLOCK_CFG(Fee_Gv.FeeIntBlockIndex)->FeeBlockAlignedSize;
    if((Fee_Gv.FeeIntBlockIndex == Fee_Gv.FeeCopyBlockIndex) && (Fee_Gv.FeeSwitchState != FEE_ERASE_SECTION))
    {
      /* !LINKSTO Fee.Dns.SS.SwitchPriority.BlockToSwitch,1 */
      /* The block that is being switched has just been written by user.
       * Fee shall not continue switching the old instance. */
      Fee_Gv.FeeSwitchState = FEE_STATE_INVALID;
    }
    else
    {
      /* Nothing to do. The user block number is different than the one that is being switched. */
    }
#if (FEE_DYNAMIC_BLOCK_LENGTH == STD_ON)
#if (FEE_NUMBER_NOT_CONFIGURED_BLOCKS > 0U)
    if (Fee_Gv.FeeIntBlockIndex >= FEE_NO_OF_CONFIGURED_BLOCKS)
    {
      /* Do nothing */
    }
    else
#endif /* #if (FEE_NUMBER_NOT_CONFIGURED_BLOCKS > 0U) */
    {
      /* The block was written with the new configuration. The old size is not important anymore. */
      /* !LINKSTO Fee.FeeDynamicBlockLength.Write,1 */
      Fee_Gv.FeeOldSizes[Fee_Gv.FeeIntBlockIndex] = Fee_BlockCfg[Fee_Gv.FeeIntBlockIndex].FeeBlockSize;
    }
#endif /* #if (FEE_DYNAMIC_BLOCK_LENGTH == STD_ON) */
    /* Set the job result */
    Fee_JobEnd();
  /* previous flash job has failed */
  }
  else
  {
    /* !LINKSTO Fee.Dns.FailureHandling.Job,1 */
    Fee_JobError();
  }
}

#define FEE_STOP_SEC_CODE
#include <MemMap.h>

/*==================[internal constants]====================================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
