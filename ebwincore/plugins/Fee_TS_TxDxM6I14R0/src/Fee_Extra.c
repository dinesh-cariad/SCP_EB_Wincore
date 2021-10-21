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
/* This file contains code for vendor specific features. */
/*==================[inclusions]============================================*/
#include <Fee_Internal.h>     /* Internal function prototypes and internal defines and declarations*/
#include <Fee_Extra.h>        /* Not needed. Included only to prevent warning. */
#include <Fee_Trace.h>        /* Trace */

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#define FEE_START_SEC_CODE
#include <MemMap.h>

#if (FEE_ERASE_COUNTER_API == STD_ON)
/*--------------------------[Fee_GetEraseCounterValue]-------------------------------*/

FUNC(uint32, FEE_CODE) Fee_GetEraseCounterValue(void)
{
  DBG_FEE_GETERASECOUNTERVALUE_ENTRY();
  DBG_FEE_GETERASECOUNTERVALUE_EXIT();
  return Fee_Gv.SectionEraseCounter;
}
#endif


#if (FEE_FREEZE_ACTIVITIES_API == STD_ON)
/*----------------------------------[Fee_FreezeActivities]---------------------------------------*/
/* !LINKSTO Fee.FreezeActivities.API,1 */
FUNC(Std_ReturnType, FEE_CODE) Fee_FreezeActivities
(
uint8  Freeze
)
{
  Std_ReturnType ReturnValue = E_NOT_OK;
  DBG_FEE_FREEZEACTIVITIES_ENTRY();
  if (Fee_State != FEE_UNINIT)
  {
   Fee_Gv.FreezeActivities = Freeze;
   ReturnValue = E_OK;
  }
  DBG_FEE_FREEZEACTIVITIES_EXIT();
  return ReturnValue;
}
#endif


#if (FEE_WRITE_CUSTOM_API == STD_ON)
/*----------------------------------[Fee_WriteCustom]---------------------------------------*/
/* !LINKSTO Fee.FeeWriteCustom,1 */
FUNC(Std_ReturnType, FEE_CODE) Fee_WriteCustom
(
  uint16                                   BlockNumber,
  P2CONST(uint8, AUTOMATIC, FEE_APPL_DATA) DataBufferPtr,
  uint16                                   BlockSize
)
{
  uint16 IntBlockIndex;
  Std_ReturnType ReturnValue = E_NOT_OK;
#if  ((FEE_WRITE_CUSTOM_API == STD_ON) || (FEE_READ_CUSTOM_API == STD_ON))
  boolean PreemptionProtection;
#endif /* ((FEE_WRITE_CUSTOM_API == STD_ON) || (FEE_READ_CUSTOM_API == STD_ON)) */

  DBG_FEE_WRITE_CUSTOM_ENTRY(BlockNumber,DataBufferPtr,BlockSize);

#if  ((FEE_WRITE_CUSTOM_API == STD_ON) || (FEE_READ_CUSTOM_API == STD_ON))
  PreemptionProtection = Fee_CheckAndSetPreemptionProtection();
  if (PreemptionProtection == TRUE)
  {
    /* Report the preemption Det error */
    /* !LINKSTO Fee.PreemptionProtection.DET,1 */
    FEE_DET_REPORT_ERROR(FEE_WRITE_CUSTOM_API_ID, FEE_E_PREEMPTION_OCCURED);
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
        /* !LINKSTO Fee.FeeWriteCustom.UninitOrBusy,1 */
        /* Job is rejected. */
#if (FEE_DEV_ERROR_DETECT == STD_ON)
        /* !LINKSTO Fee.FeeWriteCustom.Det.Uninit,1 */
        FEE_DET_REPORT_ERROR(FEE_WRITE_CUSTOM_API_ID, FEE_E_UNINIT);
#endif
      }

      /* is the module busy? */
      else if(FEE_NO_JOB != Fee_Gv.FeeJob)
      {
        /* !LINKSTO Fee.FeeWriteCustom.UninitOrBusy,1 */
        /* Job is rejected. */
#if (FEE_DEV_ERROR_DETECT == STD_ON)
        /* !LINKSTO Fee.FeeWriteCustom.Det.Busy,1 */
        FEE_DET_REPORT_ERROR(FEE_WRITE_CUSTOM_API_ID, FEE_E_BUSY);
#endif
      }
      else
      {
#if (FEE_DEV_ERROR_DETECT == STD_ON)
        /* is the block number valid? */
        if (BlockNumber == 0U)
        {
          /* !LINKSTO Fee.FeeWriteCustom.Det.BlockNumber.Invalid,1 */
          FEE_DET_REPORT_ERROR(FEE_WRITE_CUSTOM_API_ID, FEE_E_INVALID_BLOCK_NO);
        }
        /* is the data buffer valid? */
        else if (NULL_PTR == DataBufferPtr)
        {
          /* !LINKSTO Fee.FeeWriteCustom.Det.DataPointer,1 */
          FEE_DET_REPORT_ERROR(FEE_WRITE_CUSTOM_API_ID, FEE_E_INVALID_DATA_PTR);
        }
        else
#endif
        {
          /* Get the block index of requested block */
          IntBlockIndex = Fee_SearchConfigTable(BlockNumber);
          if (IntBlockIndex < FEE_NO_OF_CONFIGURED_BLOCKS)
          {
              /* !LINKSTO Fee.FeeWriteCustom.ConfiguredBlock,2 */
              /* Job is rejected. */
#if (FEE_DEV_ERROR_DETECT == STD_ON)
              /* !LINKSTO Fee.FeeWriteCustom.Det.BlockNumber.ConfiguredBlock,1 */
              FEE_DET_REPORT_ERROR(FEE_WRITE_CUSTOM_API_ID, FEE_E_INVALID_BLOCK_NO);
#endif
          }
          /* The block is not present in the new configuration */
          else
          {
            /* !LINKSTO Fee.Dsn.Initialization.Startup1.SwitchNotConfiguredBlocks,1 */
            /* !LINKSTO Fee.Dsn.Initialization.Startup1.BlockInfos.02,1 */
            /* !LINKSTO Fee.Dsn.Initialization.Startup1.BlocksSearchStop.SwNotConfigBlocksON,1 */
            if(IntBlockIndex == 0xFFFFU)
            {
              if((Fee_Gv.FeeNbOfNotCfgBlocksFound + FEE_NO_OF_CONFIGURED_BLOCKS) < FEE_TOTAL_NUMBER_OF_BLOCKS)
              {
                if((BlockSize > 0) && (BlockSize <= FEE_NOT_CONFIGURED_BLOCKS_DATA_SIZE))
                {
                  /* is enough space as reserved for not config blocks? */
                  if (
                      ( FEE_ALLIGNED_TO_VIRTUAL_PAGE(BlockSize)
                          + FEE_BLOCK_MANAGEMENT_ALIGNED_TOTAL_SIZE
                          + Fee_GetNotConfigFoundBlocksUsedSize()
                      )
                      <=
                      FEE_NOT_CONFIGURED_BLOCKS_TOTAL_SIZE
                  )
                  {
                    IntBlockIndex = Fee_Gv.FeeNbOfNotCfgBlocksFound + FEE_NO_OF_CONFIGURED_BLOCKS;
                    /* memorize config of new block */
                    Fee_NotCfgBlocks[Fee_Gv.FeeNbOfNotCfgBlocksFound].FeeBlockSize = BlockSize;
                    Fee_NotCfgBlocks[Fee_Gv.FeeNbOfNotCfgBlocksFound].FeeBlockNumber = BlockNumber;
                    Fee_NotCfgBlocks[Fee_Gv.FeeNbOfNotCfgBlocksFound].FeeBlockAlignedSize = (uint16)FEE_ALLIGNED_TO_VIRTUAL_PAGE(BlockSize);
                    Fee_Gv.FeeNbOfNotCfgBlocksFound++;
                    /* Job accepted */
                    ReturnValue = E_OK;
                  }
                  else
                  {
                    /* Job is rejected. */
#if (FEE_DEV_ERROR_DETECT == STD_ON)
                    /* !LINKSTO Fee.FeeWriteCustom.Det.BlockSize,1 */
                    FEE_DET_REPORT_ERROR(FEE_WRITE_CUSTOM_API_ID, FEE_E_INVALID_BLOCK_LEN);
#endif
                  }
                }
                else
                {
                  /* Job is rejected. */
#if (FEE_DEV_ERROR_DETECT == STD_ON)
                  /* !LINKSTO Fee.FeeWriteCustom.Det.BlockSize,1 */
                  FEE_DET_REPORT_ERROR(FEE_WRITE_CUSTOM_API_ID, FEE_E_INVALID_BLOCK_LEN);
#endif
                }
              }
              else
              {
                /* Job is rejected. */
#if (FEE_DEV_ERROR_DETECT == STD_ON)
                /* !LINKSTO Fee.Dsn.WriteCustom.TooManyBlocks,1 */
                /* The current block can not be switched and it will be lost. */
                FEE_DET_REPORT_ERROR(FEE_WRITE_CUSTOM_API_ID, FEE_E_NOT_CFG_BLK_LOST);
#endif
              }
            }
            else
            {
              if(Fee_NotCfgBlocks[IntBlockIndex - FEE_NO_OF_CONFIGURED_BLOCKS].FeeBlockSize != BlockSize)
              {
                if(BlockSize > Fee_NotCfgBlocks[IntBlockIndex - FEE_NO_OF_CONFIGURED_BLOCKS].FeeBlockSize)
                {
                  if (
                      ( (FEE_ALLIGNED_TO_VIRTUAL_PAGE(BlockSize)
                          + Fee_GetNotConfigFoundBlocksUsedSize())
                          - Fee_NotCfgBlocks[IntBlockIndex - FEE_NO_OF_CONFIGURED_BLOCKS].FeeBlockAlignedSize
                      )
                      <=
                      FEE_NOT_CONFIGURED_BLOCKS_TOTAL_SIZE
                  )
                  {
                    Fee_NotCfgBlocks[IntBlockIndex - FEE_NO_OF_CONFIGURED_BLOCKS].FeeBlockSize = BlockSize;
                    Fee_NotCfgBlocks[IntBlockIndex - FEE_NO_OF_CONFIGURED_BLOCKS].FeeBlockAlignedSize = (uint16)FEE_ALLIGNED_TO_VIRTUAL_PAGE(BlockSize);
                    /* Job accepted */
                    ReturnValue = E_OK;
                  }
                  else
                  {
                    /* Job is rejected. */
#if (FEE_DEV_ERROR_DETECT == STD_ON)
                    /* !LINKSTO Fee.FeeWriteCustom.Det.BlockSize,1 */
                    FEE_DET_REPORT_ERROR(FEE_WRITE_CUSTOM_API_ID, FEE_E_INVALID_BLOCK_LEN);
#endif
                  }
                }
                else
                {
                  if(BlockSize > 0)
                  {
                    Fee_NotCfgBlocks[IntBlockIndex - FEE_NO_OF_CONFIGURED_BLOCKS].FeeBlockSize = BlockSize;
                    Fee_NotCfgBlocks[IntBlockIndex - FEE_NO_OF_CONFIGURED_BLOCKS].FeeBlockAlignedSize = (uint16)FEE_ALLIGNED_TO_VIRTUAL_PAGE(BlockSize);
                    /* Job accepted */
                    ReturnValue = E_OK;
                  }
                  else
                  {
                    /* Job is rejected. */
#if (FEE_DEV_ERROR_DETECT == STD_ON)
                    /* !LINKSTO Fee.FeeWriteCustom.Det.BlockSize,1 */
                    FEE_DET_REPORT_ERROR(FEE_WRITE_CUSTOM_API_ID, FEE_E_INVALID_BLOCK_LEN);
#endif
                  }
                }
              }
              else
              {
                /* Job accepted */
                ReturnValue = E_OK;
              }
            }
          }

          /* !LINKSTO SWS_Fee_00025,1 */
          /* Fee_Init API should have been called by now, and no job should be already memorized */
          if(ReturnValue == E_OK)
          {
            /* Set common job parameters */
            Fee_Gv.FeeIntBlockIndex = IntBlockIndex;
            Fee_Gv.FeeJob = FEE_WRITE_JOB;
            Fee_Gv.FeeBlockDataWriteBuffPtr = DataBufferPtr;
            Fee_Gv.FeeJobResult = MEMIF_JOB_PENDING;
          }
        }
      }
    }
    /* Reset the preemption flag */
    FEE_RESET_PREEMPTION_PROTECTION();
  }

  DBG_FEE_WRITE_CUSTOM_EXIT(ReturnValue,BlockNumber,DataBufferPtr,BlockSize);
  return ReturnValue;
}
#endif


#if (FEE_READ_CUSTOM_API == STD_ON)
/*----------------------------------[Fee_ReadCustom]---------------------------------------*/
/* !LINKSTO Fee.FeeReadCustom,1 */
FUNC(Std_ReturnType, FEE_CODE) Fee_ReadCustom
(
  uint16                                 BlockNumber,
  uint16                                 BlockOffset,
  P2VAR(uint8, AUTOMATIC, FEE_APPL_DATA) DataBufferPtr,
  uint16                                 Length
)
{
  uint16 IntBlockIndex;
  /* !LINKSTO SWS_Fee_00172,1 */
  Std_ReturnType RetValue = E_NOT_OK;
#if  ((FEE_WRITE_CUSTOM_API == STD_ON) || (FEE_READ_CUSTOM_API == STD_ON))
  boolean PreemptionProtection;
#endif /* ((FEE_WRITE_CUSTOM_API == STD_ON) || (FEE_READ_CUSTOM_API == STD_ON)) */

  DBG_FEE_READ_CUSTOM_ENTRY(BlockNumber,BlockOffset,DataBufferPtr,Length);

#if  ((FEE_WRITE_CUSTOM_API == STD_ON) || (FEE_READ_CUSTOM_API == STD_ON))
  PreemptionProtection = Fee_CheckAndSetPreemptionProtection();
  if (PreemptionProtection == TRUE)
  {
    /* Report the preemption Det error */
    /* !LINKSTO Fee.PreemptionProtection.DET,1 */
    FEE_DET_REPORT_ERROR(FEE_READ_CUSTOM_API_ID, FEE_E_PREEMPTION_OCCURED);
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
      /* !LINKSTO Fee.FeeReadCustom.UninitOrBusy,1 */
      /* Is the module initialized? */
      if (FEE_UNINIT == Fee_State)
      {
#if (FEE_DEV_ERROR_DETECT == STD_ON)
        /* !LINKSTO Fee.FeeReadCustom.Det.Uninit,1 */
        FEE_DET_REPORT_ERROR(FEE_READ_CUSTOM_API_ID, FEE_E_UNINIT);
#endif
      }

      /* is the module busy? */
      else if(FEE_NO_JOB != Fee_Gv.FeeJob)
      {
#if (FEE_DEV_ERROR_DETECT == STD_ON)
        /* !LINKSTO Fee.FeeReadCustom.Det.Busy,1 */
        FEE_DET_REPORT_ERROR(FEE_READ_CUSTOM_API_ID, FEE_E_BUSY);
#endif
      }

      else
      {
#if (FEE_DEV_ERROR_DETECT == STD_ON)
        /* Is the data buffer valid? */
        if (NULL_PTR == DataBufferPtr)
        {
          /* !LINKSTO Fee.FeeReadCustom.Det.DataPointer,1 */
          FEE_DET_REPORT_ERROR(FEE_READ_CUSTOM_API_ID, FEE_E_INVALID_DATA_PTR);
        }
        else
#endif
        {
          /* Get the block index of requested block */
          IntBlockIndex = Fee_SearchConfigTable(BlockNumber);
          /* has the block number been configured in the BlockCfg Table? */
          /* !LINKSTO Fee.FeeReadCustom.BlockNumber,2 */
          if (
               (IntBlockIndex < FEE_NO_OF_CONFIGURED_BLOCKS)
               ||
               (IntBlockIndex >= (FEE_NO_OF_CONFIGURED_BLOCKS + Fee_Gv.FeeNbOfNotCfgBlocksFound))
             )
          {
#if (FEE_DEV_ERROR_DETECT == STD_ON)
            /* !LINKSTO Fee.FeeReadCustom.Det.BlockNumber,2 */
            FEE_DET_REPORT_ERROR(FEE_READ_CUSTOM_API_ID, FEE_E_INVALID_BLOCK_NO);
#endif
          }

          /* is the block offset valid? */
          /* If Fee has found the block with size 0 at initialization, it means that the block was invalidated.
           * Fee does not know the real size of the block. Therefore the offset check can not apply */
          else
          {
#if (FEE_DEV_ERROR_DETECT == STD_ON)
            const Fee_BlockConfiguration_t* BlockConfPtr = FEE_BLOCK_CFG(IntBlockIndex);

            if (( BlockOffset >= BlockConfPtr->FeeBlockSize) && (BlockConfPtr->FeeBlockSize != 0U))
            {
              /* !LINKSTO Fee.FeeReadCustom.Det.WrongOffset,1 */
              FEE_DET_REPORT_ERROR(FEE_READ_CUSTOM_API_ID, FEE_E_INVALID_BLOCK_OFS);
            }

            /* is the block length valid? */
            /* If Fee has found the block with size 0 at initialization, it means that the block was invalidated.
             * Fee does not know the real size of the block. Therefore the length check can not apply */
            else if (((Length + BlockOffset) > BlockConfPtr->FeeBlockSize) && (BlockConfPtr->FeeBlockSize != 0U))
            {
              /* !LINKSTO Fee.FeeReadCustom.Det.WrongLength,1 */
              FEE_DET_REPORT_ERROR(FEE_READ_CUSTOM_API_ID, FEE_E_INVALID_BLOCK_LEN);
            }
            else
#endif
            {
              Fee_Gv.FeeIntBlockIndex = IntBlockIndex;
              Fee_Gv.FeeBlockDataReadBuffPtr = DataBufferPtr;
              Fee_Gv.FeeLength = Length;
              Fee_Gv.FeeBlockReadOffset = BlockOffset;
              Fee_Gv.FeeJob = FEE_READ_JOB;
              Fee_Gv.FeeJobResult = MEMIF_JOB_PENDING;
              RetValue = E_OK;
            }
          }
        }
      }
    }
    /* Reset the preemption flag */
    FEE_RESET_PREEMPTION_PROTECTION();
  }

  DBG_FEE_READ_CUSTOM_EXIT(RetValue,BlockNumber,BlockOffset,DataBufferPtr,Length);
  return RetValue;
}
#endif


#if (FEE_CANCEL_SECTION_ERASE == STD_ON)
/*--------------------------[Fee_CancelSectionErase]--------------------*/
/* !LINKSTO Fee.Dsn.CancelSectionErase,1 */
FUNC(Std_ReturnType, FEE_CODE) Fee_CancelSectionErase
(
  void
)
{
  Std_ReturnType RetValue = E_NOT_OK;
  DBG_FEE_CANCELSECTIONERASE_ENTRY();
#if (FEE_FLS_CANCEL_API == STD_ON)
  /* Cancel Erase only if erase is ongoing and the main function has not been interrupted by current function call */
  if( ! (FEE_CHECK_FLAG(FEE_MAIN_FUNCTION_CALL_MASK) ||
     FEE_CHECK_FLAG(FEE_HIGHEST_INTERNAL_PRIORITY_MASK)))
  {
    if(Fee_State == FEE_ERASE_SECTION)
    {
      /* Cancel the Fls Erase */
      Fls_Cancel();
      /* Reset the delay timer */
      Fee_Gv.FeeDelayTimer = FEE_DELAY;
      RetValue = E_OK;
    }
  }
#endif
  DBG_FEE_CANCELSECTIONERASE_EXIT(RetValue);
  return RetValue;
}
#endif

#define FEE_STOP_SEC_CODE
#include <MemMap.h>

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
