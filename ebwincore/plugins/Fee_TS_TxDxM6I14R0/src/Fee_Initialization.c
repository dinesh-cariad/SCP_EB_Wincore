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
/* This file contains code for Fee initialization */
/*==================[inclusions]============================================*/
#include <Fee.h>                /* External API declarations  */
#include <Fee_Internal.h>       /* Internal function prototypes and internal defines and declarations*/
#include <TSMem.h>              /* Memory Routines */
#if (FEE_PROD_ERROR_DETECT == STD_ON)
/* !LINKSTO FEE060,1 */
#include <Dem.h>                /* General Dem header file */
#endif
#include <Crc.h>                /* Crc public header file  */
#include <Fee_Initialization.h> /* Not needed. Included only to prevent warning. */

/*==================[macros]=================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define FEE_START_SEC_CODE
#include <MemMap.h>

/** \brief Function to decide the next step after Footer reading */
INLINE STATIC FUNC(void, FEE_CODE) Fee_FooterReadNextStep(void);

/** \brief Function to read the next block info or the next buffer of block infos */
INLINE STATIC FUNC(void, FEE_CODE) Fee_ReadNextBlockInfo(void);

/** \brief Function to read the next buffer of block infos */
STATIC FUNC(void, FEE_CODE) Fee_ReadNextBufferOfBlockInfos(void);

/** \brief Function to go cache the next section or finish StartUp1 */
STATIC FUNC(void, FEE_CODE) Fee_SectionCachedGoToNextStep(void);

/** \brief Function to fill the cache after each Fls_Read of block infos */
STATIC FUNC(boolean, FEE_CODE) Fee_FillCache(void);

/** \brief Function to cache one block info if valid */
INLINE STATIC FUNC(void, FEE_CODE) Fee_ProcessBlockInfo
(
    uint16 BufferIndex
);

/** \brief Function to verify if the block needs to be chached, and where(FeeCache or FeeSwRestartCache).
           Calls the Fee_CacheBlockAddress function if all the preconditions for caching are met. */
STATIC FUNC(void, FEE_CODE) Fee_CacheIfPreconditionsMet
(
  Fls_AddressType BlockAddress,
  uint16 BlockLength,
  uint16 BlockIndex,
  uint16 WrittenMarkerIndex
);

/** \brief Function to save the data address of the block in cache.
 *         It returns TRUE if the block was cached. */
STATIC FUNC(boolean, FEE_CODE) Fee_CacheBlockAddress
(
  Fls_AddressType * CacheObject,
  Fls_AddressType BlockAddress,
  uint16 BlockLength,
  uint16 WrittenMarkerIndex,
  uint16 BlockIndex
);

/** \brief Function to check the validity of the currently read section management (header/footer).
 *         Also memorizes the erase counter if it is the greatest and establish the newest section.
 *
 * \revtal - the validity of the section management object.
 */
STATIC FUNC(Fee_SectionStatus_t, FEE_CODE) Fee_CheckSectionManagementValidity(void);

/** \brief Function to check and set the status of the currently read section management (header/footer)
 *         based on the validity of the section management object.
 *
 * \param[in] HeaderOrFooter - type of section management object
 * \param[in] Validity - validity of the section management object
 *
 * \revtal - the status of the section management object.
 */
STATIC FUNC(Fee_SectionStatus_t, FEE_CODE) Fee_GetSectionManagementStatus
(
    void
);

/** \brief Function to set the section status of the currently processed section,
 *         based on both section management objects statuses.
 *
 * \param[in] HeaderStatus - the status of the section's header
 * \param[in] FooterStatus - the status of the section's footer
 */
STATIC FUNC(void, FEE_CODE) Fee_DecideSectionStatus
(
    Fee_SectionStatus_t HeaderStatus,
    Fee_SectionStatus_t FooterStatus
);

/** \brief Function to find the newest Active section, and to figure out whether a switch
 *         was interrupted. */
STATIC FUNC(void, FEE_CODE) Fee_FindNewestActiveSection(void);

/** \brief Function that returns TRUE if the currently read header is marked as To Be Erased,
 *         otherwise returns FALSE. */
STATIC FUNC(boolean, FEE_CODE) Fee_IsHeaderMarkedAsToBeErased(void);

/** \brief Function that returns TRUE if the currently read header is marked as Active,
 *         otherwise returns FALSE. */
STATIC FUNC(boolean, FEE_CODE) Fee_IsHeaderMarkedAsActive(void);

/** \brief Function that returns TRUE if the currently read footer is marked as To Be Erased,
 *         otherwise returns FALSE. */
STATIC FUNC(boolean, FEE_CODE) Fee_IsFooterMarkedAsToBeErased(void);

/** \brief Function that returns TRUE if the currently read footer is marked as Active,
 *         otherwise returns FALSE. */
STATIC FUNC(boolean, FEE_CODE) Fee_IsFooterMarkedAsActive(void);

/** \brief Function that handles the finalization of Startup1 and decide the next steps. */
STATIC FUNC(void, FEE_CODE) Fee_StartUp1Finished(void);

#if (FEE_CRITICAL_BLOCK_NUMBER != 0U)
/** \brief Function that handles activity when critical block was found in flash */
STATIC FUNC(boolean, FEE_CODE) Fee_CriticalBlockInit(void);
#endif /* (FEE_CRITICAL_BLOCK_NUMBER != 0U) */

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/


/*--------------------------[Fee_SfOnEntryInitReadHeader]-------------------*/

FUNC(void, FEE_CODE) Fee_SfOnEntryInitReadHeader
(
  void
)
{
  /* Clear the content of the buffer */
  TS_MemSet(Fee_Gv.FeeBuffPtr, ((uint8)FEE_FLASH_ERASE_VALUE), FEE_HEADER_SIZE + FEE_CONSISTENCY_PATTERN_SIZE);

  /* !LINKSTO Fee.ConsistencyPatterns.SectionHeader.Read.Startup,1 */
  /* Call flash read to read the header 0 fields and their corresponding patterns */
  if (E_OK == Fls_Read(FEE_SECTION_START_ADDRESS(Fee_Gv.FeeWorkingSectionIndex), Fee_Gv.FeeBuffPtr, FEE_HEADER_SIZE + FEE_CONSISTENCY_PATTERN_SIZE))
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

/*--------------------------[Fee_SfInitReadHeader]--------------------------*/

FUNC(void, FEE_CODE) Fee_SfInitReadHeader
(
  void
)
{
  FEE_GET_FLASH_JOB_RESULT();

  if (MEMIF_JOB_PENDING == Fee_Gv.FeeFlashJobResult)
  {
    /* Do nothing; flash job is not complete */
  }
  /* Read of section 0 headers completed successfully */
  else if ((MEMIF_JOB_OK == Fee_Gv.FeeFlashJobResult) && (!FEE_CHECK_FLAG(FEE_FLASH_FINAL_REFUSAL_MASK)))
  {
    /* If the current header has an invalid ConfigId mark the section as Inconsistent */
    Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeWorkingSectionIndex].SectionStatus = Fee_GetSectionManagementStatus();
    /* Check the Footer */
    FEE_TRIGGER_TRANS(FEE_INIT_READ_FOOTER);
  }
  /* previous flash job has failed */
  else
  {
    (void)Fee_FlsJobFailed();
    /* !LINKSTO Fee.Dsn.FailureHandling.Behaviour.Startup1.HeaderFailed,1 */
    Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeWorkingSectionIndex].SectionStatus = FEE_SECTION_INCONSISTENT;
    /* Check the Footer */
    FEE_TRIGGER_TRANS(FEE_INIT_READ_FOOTER);
  }
}

/*--------------------------[Fee_SfOnEntryInitReadFooter]-------------------*/

FUNC(void, FEE_CODE) Fee_SfOnEntryInitReadFooter
(
  void
)
{
  /* Clear the content of the buffer */
  TS_MemSet(Fee_Gv.FeeBuffPtr, ((uint8)FEE_FLASH_ERASE_VALUE), ((uint32)FEE_HEADER_SIZE + (uint32)FEE_FOOTER_BUFFER_IDX_FOR_CRASH) + (uint32)FEE_CONSISTENCY_PATTERN_SIZE);

  /* !LINKSTO Fee.ConsistencyPatterns.SectionHeader.Read.Startup,1 */
  /* Call flash read to read the header fields of section 1 */
  if (E_OK == Fls_Read(FEE_FOOTER_ADDRESS(Fee_Gv.FeeWorkingSectionIndex) - FEE_FOOTER_BUFFER_IDX_FOR_CRASH, Fee_Gv.FeeBuffPtr, FEE_HEADER_SIZE + FEE_FOOTER_BUFFER_IDX_FOR_CRASH + FEE_CONSISTENCY_PATTERN_SIZE))
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

/*--------------------------[Fee_SfInitReadFooter]--------------------------*/

FUNC(void, FEE_CODE) Fee_SfInitReadFooter
(
  void
)
{
  Fee_SectionStatus_t FooterStatus;
  FEE_GET_FLASH_JOB_RESULT();

  if (MEMIF_JOB_PENDING == Fee_Gv.FeeFlashJobResult)
  {
    /* Do nothing; flash job is not complete */
  }

  /* Read of footer finished */
  else
  {
    /* Read of footer completed successfully */
    if((MEMIF_JOB_OK == Fee_Gv.FeeFlashJobResult) && (!FEE_CHECK_FLAG(FEE_FLASH_FINAL_REFUSAL_MASK)))
    {
      /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.06,1 */
      FooterStatus = Fee_GetSectionManagementStatus();
    }
    /* previous flash job failed */
    else
    {
      (void)Fee_FlsJobFailed();
      /* !LINKSTO Fee.Dns.FailureHandling.Behaviour.Startup1.FooterFailed,1 */
      FooterStatus = FEE_SECTION_INCONSISTENT;
    }

    /* Now we know the status of the header and the status of the footer. Decide the section's overall status */
    Fee_DecideSectionStatus(Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeWorkingSectionIndex].SectionStatus, FooterStatus);

    if(((Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeWorkingSectionIndex].SectionStatus == FEE_SECTION_ACTIVE) ||
        (Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeWorkingSectionIndex].SectionStatus == FEE_SECTION_EMPTY)) ||
        (Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeWorkingSectionIndex].SectionStatus == FEE_SECTION_NO_FOOTER))
    {
      /* Find the newest empty section */
      /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.05,1 */
      if(Fee_Gv.FeeActiveSectionIndex != 255U)
      {
        if(Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeWorkingSectionIndex].SCounter != FEE_BYTESUM(Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeActiveSectionIndex].SCounter,
            FEE_SECTIONDIF(Fee_Gv.FeeWorkingSectionIndex, Fee_Gv.FeeActiveSectionIndex)))
        {
          /* Do nothing. The section is older than the previous one */
        }
        else
        {
          /* The section is newer than the previous one. Save its index. */
          Fee_Gv.FeeActiveSectionIndex = Fee_Gv.FeeWorkingSectionIndex;
        }
      }
      else
      {
        /* First consistent section found. Save its index. It could be the newest section. */
        Fee_Gv.FeeActiveSectionIndex = Fee_Gv.FeeWorkingSectionIndex;
      }
    }
    else
    {
      /* Section is not consistent. */
    }
    Fee_FooterReadNextStep();
  }
}

/*--------------------------[Fee_SfOnEntryInitFillCache]---------------------*/

FUNC(void, FEE_CODE) Fee_SfOnEntryInitFillCache
(
  void
)
{
  /* Go to the Active section if exist, otherwise to the newest full section */
  Fee_Gv.FeeWorkingSectionIndex = Fee_Gv.FeeActiveSectionIndex;
  /* We have to verify if the "to be Active" section is really Active and it is ready to be used */
  if(Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeWorkingSectionIndex].SectionStatus == FEE_SECTION_ACTIVE)
  {
    /* First read of block infos will always be of a FEE_INIT_BUFFER_SIZE */
    Fee_Gv.Fee_BufferIndex = FEE_INIT_BUFFER_SIZE;

    Fee_Gv.FeeWorkingDataAddress = FEE_DATA_ADDRESS(Fee_Gv.FeeWorkingSectionIndex);
    Fee_Gv.FeeWorkingInfoAddress = FEE_FIRST_BLOCK_INFO_ADDRESS(Fee_Gv.FeeWorkingSectionIndex);

    /* Call flash read to read the block infos to buffer from the section to be
     * filled into cache */
    /* !LINKSTO Fee.Dsn.Initialization.Startup1.FillCacheWithBlockInfos,1 */
    if (E_OK == Fls_Read(FEE_FOOTER_ADDRESS(Fee_Gv.FeeWorkingSectionIndex) - FEE_INIT_BUFFER_SIZE, Fee_Gv.FeeBuffPtr, FEE_INIT_BUFFER_SIZE))
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
  else
  {
    Fee_SectionCachedGoToNextStep();
  }
}

/*--------------------------[Fee_SfInitFillCache]----------------------------*/

FUNC(void, FEE_CODE) Fee_SfInitFillCache
(
  void
)
{
  boolean Completed = FALSE; /* if cache filling is completed */
  FEE_GET_FLASH_JOB_RESULT();

  if (MEMIF_JOB_PENDING == Fee_Gv.FeeFlashJobResult)
  {
    /* Do nothing; flash job is not complete */
  }

  else if ((MEMIF_JOB_OK == Fee_Gv.FeeFlashJobResult) && (!FEE_CHECK_FLAG(FEE_FLASH_FINAL_REFUSAL_MASK)))
  {
    /* Fill the Cache using the block infos read from flash to buffer */
    Completed = Fee_FillCache();

    /* If block infos are yet to be read from the current section */
    if (FALSE == Completed)
    {
      Fee_ReadNextBufferOfBlockInfos();
    }
    else
    {
      Fee_SectionCachedGoToNextStep();
    }
  }

  /* previous flash job failed */
  else
  {
    /* !LINKSTO Fee.Dsn.FlsRead.BlockInfos.Fails,1 */
    (void)Fee_FlsJobFailed();
    FEE_TRIGGER_TRANS(FEE_INIT_BLOCK_BY_BLOCK);
  }
}

/*--------------------------[Fee_SfOnEntryInitBlockByBlock]---------------------*/

FUNC(void, FEE_CODE) Fee_SfOnEntryInitBlockByBlock
(
  void
)
{
  /* The last block info found in the previous buffer valid or not led to and FeeInitializationInfoAddress update.
   * Otherwise, if the last block infos corresponding area has been found empty than we must take
   * Fee_NumberOfMarginBlockInfos into consideration */
  if (E_OK == Fls_Read(Fee_Gv.FeeWorkingInfoAddress - (Fls_AddressType)((uint32)Fee_Gv.Fee_NumberOfMarginBlockInfos * (uint32)FEE_BLOCKINFO_PLUS_WM_SIZE_AL),
                       &Fee_Gv.FeeBuffPtr[Fee_Gv.Fee_BufferIndex - (uint16)FEE_BLOCKINFO_PLUS_WM_SIZE_AL],
                       FEE_BLOCKINFO_PLUS_WM_SIZE_AL))
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

/*--------------------------[Fee_SfInitBlockByBlock]----------------------------*/

FUNC(void, FEE_CODE) Fee_SfInitBlockByBlock
(
  void
)
{
  boolean Completed = FALSE; /* if cache filling is completed */

    FEE_GET_FLASH_JOB_RESULT();

    if (MEMIF_JOB_PENDING == Fee_Gv.FeeFlashJobResult)
    {
      /* Do nothing; flash job is not complete */
    }

    else if ((MEMIF_JOB_OK == Fee_Gv.FeeFlashJobResult) && (!FEE_CHECK_FLAG(FEE_FLASH_FINAL_REFUSAL_MASK)))
    {
      /* Fill the Cache using the block infos read from flash to buffer */
      Completed = Fee_FillCache();

      /* If block infos are yet to be read from the current section */
      if (FALSE == Completed)
      {
        Fee_ReadNextBlockInfo();
      }
      else
      {
        Fee_SectionCachedGoToNextStep();
        /* Fee buffer is completely checked */
        Fee_State = FEE_INIT_FILL_CACHE;
      }
    }
    else
    {
      (void)Fee_FlsJobFailed();

      /* Update also the Fee_BufferIndex by going to the next block info */
      Fee_Gv.Fee_BufferIndex -= FEE_BLOCKINFO_PLUS_WM_SIZE_AL;
      /* !LINKSTO Fee.Dsn.FlsRead.BlockInfoByBlockInfo.Fails,1 */
      /* Increment LostSectionSize with BBS */
      /* !LINKSTO Fee.Dsn.Initialization.Startup1.InvalidLastBlockInfos,1 */
      Fee_Gv.FeeLostSectionSize += FEE_BIGGEST_BLOCK_SIZE;
      /* FeeInitializationInfoAddress must be updated in order to jump over the messed block info("+ 1") */
      /* and eventually empty area */
      Fee_Gv.FeeWorkingInfoAddress -= (Fls_AddressType)((uint32)((uint32)Fee_Gv.Fee_NumberOfMarginBlockInfos + (uint32)1U) * (uint32)FEE_BLOCKINFO_PLUS_WM_SIZE_AL);
      /* Reset Fee_NumberOfMarginBlockInfos as the failed block info is considered not empty
       * in order to be on the safe side */
      Fee_Gv.Fee_NumberOfMarginBlockInfos = 0U;
      if ((Fee_Gv.FeeWorkingDataAddress + (FEE_DATA_INFO_MARGIN * FEE_BLOCKINFO_PLUS_WM_SIZE_AL)) >
      Fee_Gv.FeeWorkingInfoAddress)
      {
        /* Section completed. Reset Fee_Gv.FeeLostSectionSize */
        Fee_Gv.FeeLostSectionSize = 0U;
        /* Reset the empty block infos counter */
        Fee_Gv.Fee_NumberOfMarginBlockInfos = 0U;
        /* The section is cached */
        Fee_SectionCachedGoToNextStep();
        /* Fee buffer is completely checked */
        Fee_State = FEE_INIT_FILL_CACHE;
      }
      else
      {
        /* Go on reading the next block info if there is anything left to read in the problematic buffer size */
        Fee_ReadNextBlockInfo();
      }
    }
}

/*==================[internal constants]====================================*/

/*==================[internal function definitions]=========================*/

/*--------------------------[Fee_FooterReadNextStep]-------------------------*/

INLINE STATIC FUNC(void, FEE_CODE) Fee_FooterReadNextStep
(
  void
)
{
  if(Fee_Gv.FeeWorkingSectionIndex != (FEE_NUMBER_OF_SECTIONS - 1U))
  {
    /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.02,1 */
    Fee_Gv.FeeWorkingSectionIndex = Fee_Gv.FeeWorkingSectionIndex + (uint8)1;
    /* Proceed with reading Section 1 header */
    FEE_TRIGGER_TRANS(FEE_INIT_READ_HEADER);
  }
  else
  {
    /* Find the active section and start searching for blocks */
    Fee_FindNewestActiveSection();
    /* !LINKSTO Fee.Dsn.Initialization.Startup1.BlockInfos.01,1 */
    FEE_TRIGGER_TRANS(FEE_INIT_FILL_CACHE);
  }
}


/*--------------------------[Fee_FillCache]----------------------------------*/

STATIC FUNC(boolean, FEE_CODE) Fee_FillCache
(
  void
)
{
  boolean Completed = FALSE;
#if (FEE_CONSISTENCY_PATTERN_ENABLED == STD_ON)
  boolean Condition = TRUE;
#endif
  /* !LINKSTO Fee.Dsn.Initialization.Startup1.BlockInfosSearch,1 */
  do{
    /* Go to the index which contains the next block info written to the section */
    Fee_Gv.Fee_BufferIndex -= FEE_BLOCKINFO_PLUS_WM_SIZE_AL;

    /* Check whether all block infos are read */
    if (TRUE == Fee_CheckBlockInfoErased(Fee_Gv.Fee_BufferIndex))
    {
      /* Set the EmptyBlockInfoDetected flag because an empty block info was detected */
      Fee_Gv.Fee_NumberOfMarginBlockInfos++;
      /* First empty block management information found, try to read the second block info */
      if (Fee_Gv.Fee_NumberOfMarginBlockInfos < FEE_DATA_INFO_MARGIN)
      {
        /* do nothing, go to the next BI */
      }
      /* The configured number of empty block management info found, set the completed flag */
      else
      {
        /* If the last block info was inconsistent than possibly written biggest blocks size
         * must be considered. Otherwise Fee_Gv.FeeLostSectionSize should be 0 */
        Fee_Gv.FeeWorkingDataAddress += Fee_Gv.FeeLostSectionSize;
        /* Section completed. Reset Fee_Gv.FeeLostSectionSize */
        Fee_Gv.FeeLostSectionSize = 0U;
        /* Reset the empty block infos counter */
        Fee_Gv.Fee_NumberOfMarginBlockInfos = 0U;
        /* Set the flag to indicate that all block infos in the current section are read */
        Completed = TRUE;
      }
    }
    else
    {
#if (FEE_CONSISTENCY_PATTERN_ENABLED == STD_ON)
      /* !LINKSTO Fee.ConsistencyPattern.BlockManagementInformation.Read,1 */
      /* !LINKSTO Fee.ConsistencyStartPattern.BlockManagementInformation.Read,1 */
      /* Check if the data consistency patterns match the expected value */
#if (FEE_CONSISTENCY_START_PATTERN_ENABLED == STD_ON)
       Condition = (FEE_READ_BYTE(Fee_Gv.FeeBuffPtr,
              Fee_Gv.Fee_BufferIndex +
              FEE_CONSISTENCY_PATTERN_SIZE +
              FEE_BLOCKINFO_ALIGNED_SIZE) == Fee_ConsistencyPattern[0]);
#endif
      if (Condition && (FEE_READ_BYTE(Fee_Gv.FeeBuffPtr, Fee_Gv.Fee_BufferIndex) == Fee_ConsistencyPattern[0])
      )
#endif
      {
        Fee_ProcessBlockInfo(Fee_Gv.Fee_BufferIndex);
        /* Update the block info address */
        Fee_Gv.FeeWorkingInfoAddress -= FEE_BLOCKINFO_PLUS_WM_SIZE_AL;
        /* If empty block infos had been found before we should take them into acount */
        Fee_Gv.FeeWorkingInfoAddress -= (Fls_AddressType)((uint32)Fee_Gv.Fee_NumberOfMarginBlockInfos * (uint32)FEE_BLOCKINFO_PLUS_WM_SIZE_AL);
        /* Something different than empty was detected, reset the counter */
        Fee_Gv.Fee_NumberOfMarginBlockInfos = 0;
      }
#if (FEE_CONSISTENCY_PATTERN_ENABLED == STD_ON)
      else
      {
        /* In case the block info was deemed invalid, it's data fields content can not
         * be used to update the Fee's data address (Fee_Gv.FeeInitializationDataAddress). */

        /* Update the block info address */
        Fee_Gv.FeeWorkingInfoAddress -= FEE_BLOCKINFO_PLUS_WM_SIZE_AL;
        /* The block info is not valid. Consider the worst case scenario */
        Fee_Gv.FeeLostSectionSize += FEE_BIGGEST_BLOCK_SIZE;
      }
#endif
    }
  }/* Check each element in the buffer till all block infos are read */
  while (((FALSE == Completed) && (Fee_Gv.Fee_BufferIndex != 0U)) && (Fee_State == FEE_INIT_FILL_CACHE));

  /* This is the ultimate exit condition:
   *  */
  if((FALSE == Completed) && (Fee_Gv.Fee_BufferIndex == 0U))
  {
    if (Fee_Gv.FeeWorkingInfoAddress <
        (Fee_Gv.FeeWorkingDataAddress + (Fls_AddressType)((uint32)((uint32)Fee_Gv.Fee_NumberOfMarginBlockInfos + (uint32)FEE_DATA_INFO_MARGIN) * (uint32)FEE_BLOCKINFO_PLUS_WM_SIZE_AL)))
    {
      /* Section completed. Reset Fee_Gv.FeeLostSectionSize */
      Fee_Gv.FeeLostSectionSize = 0U;
      /* Reset the empty block infos counter */
      Fee_Gv.Fee_NumberOfMarginBlockInfos = 0U;
      /* Set the flag to indicate that all block infos in the current section are read */
      Completed = TRUE;
    }
    else
    {
      /* There is still more data to be cached in this section. */
    }
  }
  else
  {
    /* Section cached. */
  }

  return Completed;
}

/*--------------------------[Fee_ProcessBlockInfo]----------------------------------*/
INLINE STATIC FUNC(void, FEE_CODE) Fee_ProcessBlockInfo
(
    uint16 BufferIndex
)
{
  /* Get data from the block info */
  const uint16 BlockLength =
      FEE_READ_WORD(Fee_Gv.FeeBuffPtr, BufferIndex + FEE_BLOCKLENGTH_INDEX + FEE_CONSISTENCY_PATTERN_SIZE);
  const uint16 BlockNumber =
      FEE_READ_WORD(Fee_Gv.FeeBuffPtr, BufferIndex + FEE_BLOCKNUMBER_INDEX + FEE_CONSISTENCY_PATTERN_SIZE);
  const Fls_AddressType BlockDataAddress =
      FEE_READ_DWORD(Fee_Gv.FeeBuffPtr, BufferIndex + FEE_BLOCKDATAADDRESS_INDEX + FEE_CONSISTENCY_PATTERN_SIZE);
  const uint16 WrittenMarkerIndex = BufferIndex + FEE_BLOCKINFO_ALIGNED_TOTAL_SIZE;
  const uint8 Checksum = Fee_Gv.FeeBuffPtr[BufferIndex + FEE_CHECKSUM_INDEX + FEE_CONSISTENCY_PATTERN_SIZE];
  const Std_ReturnType ComputedChecksum =
      Fee_CalculateChecksum(&(Fee_Gv.FeeBuffPtr[BufferIndex + FEE_CONSISTENCY_PATTERN_SIZE]), FEE_CHECKSUM_NUMBER_OF_BYTES);
  uint16 IntBlockIndex;

  /* !LINKSTO Fee.Dsn.Initialization.Startup1.BlockInfoValidity.SwitchNotConfiguredBlocksOFF,1 */
  /* !LINKSTO Fee.Dsn.Initialization.Startup1.BlockInfoValidity.SwitchNotConfiguredBlocksON,1 */
  /* !LINKSTO Fee.Dsn.Initialization.Startup1.BlockInconsistent.02,1 */
  /* Validate the checksum field of the block info */
  if (ComputedChecksum == Checksum)
  {
    IntBlockIndex = Fee_SearchConfigTable(BlockNumber);
    /* Check whether the block number has been configured in the BlockCfg Table */
    if (IntBlockIndex < FEE_NO_OF_CONFIGURED_BLOCKS)
    {
      Fee_BlockCfgPtr = &Fee_BlockCfg[IntBlockIndex];
      /* Check whether the found block needs to be chached */
      Fee_CacheIfPreconditionsMet(BlockDataAddress, BlockLength, IntBlockIndex, WrittenMarkerIndex);
    }
    /* The block is not present in the new configuration */
    else
    {
#if (FEE_NUMBER_NOT_CONFIGURED_BLOCKS > 0U)
      uint32 NotConfigFoundBlocksUsedSize = Fee_GetNotConfigFoundBlocksUsedSize();
      /* !LINKSTO Fee.Dsn.Initialization.Startup1.SwitchNotConfiguredBlocks,1 */
      /* !LINKSTO Fee.Dsn.Initialization.Startup1.BlockInfos.02,1 */
      /* !LINKSTO Fee.Dsn.Initialization.Startup1.BlocksSearchStop.SwNotConfigBlocksON,1 */
      if(IntBlockIndex == 0xFFFFU)
      {
        if(
           ((Fee_Gv.FeeNbOfNotCfgBlocksFound + FEE_NO_OF_CONFIGURED_BLOCKS) < FEE_TOTAL_NUMBER_OF_BLOCKS)
           &&
           (BlockLength <= FEE_NOT_CONFIGURED_BLOCKS_DATA_SIZE)
           &&
           (( FEE_ALLIGNED_TO_VIRTUAL_PAGE(BlockLength)
                + FEE_BLOCK_MANAGEMENT_ALIGNED_TOTAL_SIZE
                + NotConfigFoundBlocksUsedSize
              )
              <=
              FEE_NOT_CONFIGURED_BLOCKS_TOTAL_SIZE
             )
          )
        {
            IntBlockIndex = Fee_Gv.FeeNbOfNotCfgBlocksFound + FEE_NO_OF_CONFIGURED_BLOCKS;
            /* memorize its configuration */
            Fee_NotCfgBlocks[Fee_Gv.FeeNbOfNotCfgBlocksFound].FeeBlockSize = BlockLength;
            Fee_NotCfgBlocks[Fee_Gv.FeeNbOfNotCfgBlocksFound].FeeBlockNumber = BlockNumber;
            Fee_NotCfgBlocks[Fee_Gv.FeeNbOfNotCfgBlocksFound].FeeBlockAlignedSize = (uint16)FEE_ALLIGNED_TO_VIRTUAL_PAGE(BlockLength);
            Fee_BlockCfgPtr = &Fee_NotCfgBlocks[Fee_Gv.FeeNbOfNotCfgBlocksFound];
            Fee_Gv.FeeNbOfNotCfgBlocksFound++;
            /* This block is now the first time found and cached */
          (void)Fee_CacheBlockAddress(&Fee_Gv.FeeCache[IntBlockIndex], BlockDataAddress, BlockLength, WrittenMarkerIndex, IntBlockIndex);
        }
        else
        {
          /* !LINKSTO Fee.FeeNumberOfNotConfigBlocks.BlocksLost,1 */
          /* The current block can not be switched and it will be lost. */
          FEE_DET_REPORT_ERROR(FEE_MAINFUNCTION_API_ID, FEE_E_NOT_CFG_BLK_LOST);
          FEE_SET_FLAG(FEE_NOT_CFG_BLK_LOST_MASK);
          /* Update the data address with the offset given by the block's data address, it's length
           * and the length of the block number), so that it points to the start of the next block.
           * Cache cannot be updated as this block is not configured now */
          Fee_Gv.FeeWorkingDataAddress = BlockDataAddress + FEE_ALLIGNED_TO_VIRTUAL_PAGE(BlockLength);
        }
      }
      else
      {
        uint32 OldAlignedSize = Fee_NotCfgBlocks[(IntBlockIndex - FEE_NO_OF_CONFIGURED_BLOCKS)].FeeBlockAlignedSize;
        if(
             (BlockLength <= FEE_NOT_CONFIGURED_BLOCKS_DATA_SIZE)
             &&
             (
               ( FEE_ALLIGNED_TO_VIRTUAL_PAGE(BlockLength)
                + FEE_BLOCK_MANAGEMENT_ALIGNED_TOTAL_SIZE
                + NotConfigFoundBlocksUsedSize
                - OldAlignedSize
               )
               <=
               FEE_NOT_CONFIGURED_BLOCKS_TOTAL_SIZE
             )
          )
        {
          Fee_BlockCfgPtr = &Fee_NotCfgBlocks[(IntBlockIndex - FEE_NO_OF_CONFIGURED_BLOCKS)];
          Fee_CacheIfPreconditionsMet(BlockDataAddress, BlockLength, IntBlockIndex, WrittenMarkerIndex);
        }
        else
        {
          /* !LINKSTO Fee.FeeNumberOfNotConfigBlocks.BlocksLost,1 */
          /* The current block can not be switched and it will be lost. */
          FEE_DET_REPORT_ERROR(FEE_MAINFUNCTION_API_ID, FEE_E_NOT_CFG_BLK_LOST);
          FEE_SET_FLAG(FEE_NOT_CFG_BLK_LOST_MASK);
          /* Update the data address with the offset given by the block's data address, it's length
           * and the length of the block number), so that it points to the start of the next block.
           * Cache cannot be updated as this block is not configured now */
          Fee_Gv.FeeWorkingDataAddress = BlockDataAddress + FEE_ALLIGNED_TO_VIRTUAL_PAGE(BlockLength);
        }
      }
#else
      /* Update the data address with the offset given by the block's data address, it's length
       * and the length of the block number), so that it points to the start of the next block.
       * Cache cannot be updated as this block is not configured now */
      Fee_Gv.FeeWorkingDataAddress = BlockDataAddress + FEE_ALLIGNED_TO_VIRTUAL_PAGE(BlockLength);
#endif
    }
  }
  else
  {
    /* The block info is not valid. Consider the worst case scenario */
    /* !LINKSTO Fee.Dsn.Initialization.Startup1.InvalidLastBlockInfos,1 */
    Fee_Gv.FeeLostSectionSize += FEE_BIGGEST_BLOCK_SIZE
#if (FEE_CONSISTENCY_START_PATTERN_ENABLED == STD_ON)
                                 + FEE_CONSISTENCY_PATTERN_SIZE
#endif
        ;
  }
}

/*------------------------------[Fee_CacheIfPreconditionsMet]--------------------------*/
STATIC FUNC(void, FEE_CODE) Fee_CacheIfPreconditionsMet
(
  Fls_AddressType BlockAddress,
  uint16 BlockLength,
  uint16 BlockIndex,
  uint16 WrittenMarkerIndex
)
{
  if (Fee_Gv.FeeCache[BlockIndex] == FEE_ZERO_BLOCKADDRESS)
  {
#if (FEE_NUMBER_NOT_CONFIGURED_BLOCKS > 0U)
    if (BlockIndex >= FEE_NO_OF_CONFIGURED_BLOCKS)
    {
      (void)Fee_CacheBlockAddress(&Fee_Gv.FeeCache[BlockIndex], BlockAddress, BlockLength, WrittenMarkerIndex, BlockIndex);
    }
    else
#endif
    {
      if(TRUE == Fee_CacheBlockAddress(&Fee_Gv.FeeCache[BlockIndex], BlockAddress, BlockLength, WrittenMarkerIndex, BlockIndex))
      {
        /* !LINKSTO Fee.Dsn.Initialization.Startup1.BlocksSearchStop.SwNotConfigBlocksOFF,1 */
        Fee_Gv.FeeNbOfBlocksFound++;
      }
    }
  }
  else
  {
     /* !LINKSTO Fee.Dsn.Initialization.Startup1.BlockInfoFound,1 */
    /* If the highest priority switch has been detected memorize the addresses of all the blocks that were switched or
     * needs to be switched, just in case the switch will be restarted. */
    if(
        (FEE_CHECK_FLAG(FEE_HIGHEST_INTERNAL_PRIORITY_MASK) && (Fee_Gv.FeeWorkingSectionIndex == Fee_Gv.FeeOldestSectionIndex))
        &&
        (Fee_Gv.FeeCache[BlockIndex] >= FEE_DATA_ADDRESS(Fee_Gv.FeeActiveSectionIndex))
        &&
        (Fee_Gv.FeeCache[BlockIndex] < FEE_SECTION_END_ADDRESS(Fee_Gv.FeeActiveSectionIndex))
       )
    {
      (void)Fee_CacheBlockAddress(&Fee_Gv.FeeSwRestartCache[BlockIndex], BlockAddress, BlockLength, WrittenMarkerIndex, BlockIndex);
    }
    else
    {
      /* !LINKSTO Fee.Dsn.Initialization.Startup1.BlockInfoCacheIfNewer,1 */
      /* Cache the block address only if this instance is newer than previous one cached */
      if (
           (Fee_Gv.FeeCache[BlockIndex] > FEE_SECTION_START_ADDRESS(Fee_Gv.FeeWorkingSectionIndex))
           &&
           (Fee_Gv.FeeCache[BlockIndex] < FEE_SECTION_END_ADDRESS(Fee_Gv.FeeWorkingSectionIndex))
         )
      {
#if (FEE_NUMBER_NOT_CONFIGURED_BLOCKS > 0U)
        /* !LINKSTO Fee.FeeNotConfigBlocks.SizeReconfigured,1 */
        /* Update the not configured block size */
        if (BlockIndex >= FEE_NO_OF_CONFIGURED_BLOCKS)
        {
          Fee_NotCfgBlocks[BlockIndex - FEE_NO_OF_CONFIGURED_BLOCKS].FeeBlockSize = BlockLength;
          Fee_NotCfgBlocks[BlockIndex - FEE_NO_OF_CONFIGURED_BLOCKS].FeeBlockAlignedSize = (uint16)FEE_ALLIGNED_TO_VIRTUAL_PAGE(BlockLength);
        }
        else
        {
          /* The block is configured. Nothing to be updated. */
        }
#endif
        (void)Fee_CacheBlockAddress(&Fee_Gv.FeeCache[BlockIndex], BlockAddress, BlockLength, WrittenMarkerIndex, BlockIndex);
      }
      else
      {
        Fee_Gv.FeeWorkingDataAddress = BlockAddress + FEE_ALLIGNED_TO_VIRTUAL_PAGE(BlockLength);
      }
    }
  }
}

/*------------------------------[Fee_CacheBlockAddress]--------------------------*/
STATIC FUNC(boolean, FEE_CODE) Fee_CacheBlockAddress
(
  Fls_AddressType * CacheObject,
  Fls_AddressType BlockAddress,
  uint16 BlockLength,
  uint16 WrittenMarkerIndex,
  uint16 BlockIndex
)
{
  boolean AddressCached = FALSE;

  if(FEE_INVALID_SIZE == BlockLength)
  {
    AddressCached = TRUE;
    if(FEE_INVALID_BLOCKADDRESS(Fee_Gv.FeeWorkingSectionIndex) == BlockAddress)
    {
      /* Store the address of the block data in the cache table */
      *CacheObject = BlockAddress;
    }
    else
    {
      /* The block must be erased, therefore save its inconsistent address. */
      *CacheObject = FEE_INCONSISTENT_BLOCKADDRESS(Fee_Gv.FeeWorkingSectionIndex);
    }
  }
  else
  {
    /* !LINKSTO Fee.Dsn.Initialization.Startup1.BlockInfoValidity.WrittenMarker,1 */
    /* !LINKSTO Fee.Dsn.Initialization.Startup1.BlockInfoValidity.WrittenMarkerValidity,1 */
    /* !LINKSTO Fee.Dsn.Initialization.Startup1.BlockInfoValidity.SwitchNotConfiguredBlocksOFF,1 */
    /* !LINKSTO Fee.Dsn.Initialization.Startup1.BlockInfoValidity.SwitchNotConfiguredBlocksON,1 */
    /* !LINKSTO Fee.ConsistencyPattern.BlockData.Read,1 */
    /* Check if the WrittenMarker is written even partially */
    if(FEE_HAS_ATLEAST_ONEBYTE_OF(&Fee_Gv.FeeBuffPtr[WrittenMarkerIndex], FEE_WRITTEN_MARKER_PATTERN))
    {

      /* Valid block found, reset the section lost estimated size */
      Fee_Gv.FeeLostSectionSize = 0U;
      /* Check if the data address read from the flash memory is valid
       * (the data address is in the range of addresses of the current section) */
      /* !LINKSTO Fee.Dsn.Initialization.Startup1.BlockInfoValidity.SwitchNotConfiguredBlocksON,1 */
      /* !LINKSTO Fee.Dsn.Initialization.Startup1.BlockInfoValidity.SwitchNotConfiguredBlocksOFF,1 */
      if ((BlockAddress + FEE_ALLIGNED_TO_VIRTUAL_PAGE(BlockLength)) < (Fee_Gv.FeeWorkingInfoAddress - (FEE_BLOCKINFO_PLUS_WM_SIZE_AL * FEE_DATA_INFO_MARGIN)))
      {
        if ((Fee_BlockCfgPtr->FeeBlockSize == BlockLength) || (BlockIndex >= FEE_NO_OF_CONFIGURED_BLOCKS))
        {
          AddressCached = TRUE;
          /* Store the address of the block data in the cache table */
          *CacheObject = BlockAddress;
        }
        /* Block size is reconfigured */
        else
        {
#if (FEE_DYNAMIC_BLOCK_LENGTH == STD_ON)
          /* Store the old length of the block */
          /* !LINKSTO Fee.Dsn.DynamicBlockLength.Init,1 */
          Fee_Gv.FeeOldSizes[BlockIndex] = BlockLength;
          AddressCached = TRUE;
          /* Store the address of the block data in the cache table */
          *CacheObject = BlockAddress;
#endif
        }
        /* Update the data address with the offset given by the block's data address, it's length
         * and the length of the block number), so that it points to the start of the next block. */
        Fee_Gv.FeeWorkingDataAddress = BlockAddress + FEE_ALLIGNED_TO_VIRTUAL_PAGE(BlockLength);
      }
      /* In case the data address read from the flash memory is out of bounds,
       * Fee's data address (Fee_Gv.FeeInitializationDataAddress) can not be updated. */
      else
      {
        /* the block was found at an invalid address, therefore this block is not considered */
        /* DataAddress is already updated */
      }
    }
    else
    {
      /* We have found a valid block but without written marker */
      /* We shall consider the size of this block */
      Fee_Gv.FeeWorkingDataAddress = BlockAddress + FEE_ALLIGNED_TO_VIRTUAL_PAGE(BlockLength);
    }
  }

#if (FEE_DYNAMIC_BLOCK_LENGTH == STD_OFF)
  TS_PARAM_UNUSED(BlockIndex);
#endif

return AddressCached;
}

/*--------------------------[Fee_ReadNextBufferOfBlockInfos]---------------------*/

STATIC FUNC(void, FEE_CODE) Fee_ReadNextBufferOfBlockInfos
(
  void
)
{
  Fls_AddressType RemainingInfoSize = ((Fee_Gv.FeeWorkingInfoAddress + (Fls_AddressType)FEE_BLOCKINFO_PLUS_WM_SIZE_AL)
                                      - Fee_Gv.FeeWorkingDataAddress)
                                      - (Fls_AddressType)((uint32)((uint32)Fee_Gv.Fee_NumberOfMarginBlockInfos + (uint32)FEE_DATA_INFO_MARGIN) * (uint32)FEE_BLOCKINFO_PLUS_WM_SIZE_AL);

  Fls_AddressType ReadAddress;
  /* !LINKSTO Fee.Dsn.Initialization.Startup1.FillCacheWithBlockInfos,1 */
  /* Check the remaining size to be read for caching */
  if(RemainingInfoSize < FEE_INIT_BUFFER_SIZE)
  {
    /* Read the remaining chunk of block infos in section */
    Fee_Gv.Fee_BufferIndex = (uint16)((RemainingInfoSize / FEE_BLOCKINFO_PLUS_WM_SIZE_AL) * FEE_BLOCKINFO_PLUS_WM_SIZE_AL);
  }
  else
  {
    /* Read a FEE_INIT_BUFFER_SIZE of block infos */
    Fee_Gv.Fee_BufferIndex = FEE_INIT_BUFFER_SIZE;
  }
  ReadAddress = (((Fee_Gv.FeeWorkingInfoAddress + (Fls_AddressType)FEE_BLOCKINFO_PLUS_WM_SIZE_AL)
      - (Fls_AddressType)((uint32)Fee_Gv.Fee_NumberOfMarginBlockInfos * (uint32)FEE_BLOCKINFO_PLUS_WM_SIZE_AL))
      - (Fls_AddressType)Fee_Gv.Fee_BufferIndex);
  if (E_OK == Fls_Read(ReadAddress, Fee_Gv.FeeBuffPtr, Fee_Gv.Fee_BufferIndex))
  {
    Fee_Gv.FeeFlashJobResult = MEMIF_JOB_PENDING;
  }
  /* flash job was refused */
  else
  {
    Fee_FlsJobRefused();
  }
}

/*--------------------------[Fee_ReadNextBlockInfo]---------------------*/

INLINE STATIC FUNC(void, FEE_CODE) Fee_ReadNextBlockInfo
(
  void
)
{
  Fls_AddressType ReadAddress;
  /* Go on reading the next block info if there is anything left to read in the problematic buffer size */
  if(Fee_Gv.Fee_BufferIndex >= FEE_BLOCKINFO_PLUS_WM_SIZE_AL)
  {
    ReadAddress = (Fee_Gv.FeeWorkingInfoAddress - (Fls_AddressType)((uint32)Fee_Gv.Fee_NumberOfMarginBlockInfos * (uint32)FEE_BLOCKINFO_PLUS_WM_SIZE_AL));
    /* If something was found with the last read the FeeInitializationInfoAddress
     * has been already updated to the next one (and Fee_NumberOfMarginBlockInfos is 0).
     * Else, if the area was found empty we must skip the empty area and read
     * the next possible block info. */
    /* Read the next block info */
    if (E_OK == Fls_Read(ReadAddress,
        &Fee_Gv.FeeBuffPtr[Fee_Gv.Fee_BufferIndex - (uint16)FEE_BLOCKINFO_PLUS_WM_SIZE_AL],
        FEE_BLOCKINFO_PLUS_WM_SIZE_AL))
    {
      Fee_Gv.FeeFlashJobResult = MEMIF_JOB_PENDING;
    }
    /* flash job was refused */
    else
    {
      Fee_FlsJobRefused();
    }
  }
  else
  {
    /* The problematic Fee buffer is completely checked */
    Fee_State = FEE_INIT_FILL_CACHE;
    /* Read the next buffer all at once */
    Fee_ReadNextBufferOfBlockInfos();
  }
}

/*--------------------------[Fee_SectionCachedGoToNextStep]---------------------*/

STATIC FUNC(void, FEE_CODE) Fee_SectionCachedGoToNextStep
(
  void
)
{
  boolean ActiveReachedSecondTime = FALSE;
  boolean Condition = TRUE;
  /* before going to the next section update the working addresses for the Active section */
  if(Fee_Gv.FeeInfoAddress == 0U)
  {
    Fee_Gv.FeeInfoAddress = Fee_Gv.FeeWorkingInfoAddress;
    Fee_Gv.FeeDataAddress = Fee_Gv.FeeWorkingDataAddress;
  }
  else
  {
    /* Nothing to do, Active section has already been checked */
  }
  Fee_Gv.Fee_NumberOfMarginBlockInfos = 0U;
  Fee_Gv.FeeWorkingSectionIndex = FEE_SECTIONDECREMENT(Fee_Gv.FeeWorkingSectionIndex);
  /* If the current working section has no data it must not be cached.
   * Therefore search for a section with valid data */
  /* !LINKSTO Fee.Dsn.Initialization.Startup1.BlockInfos.03,1 */
  while((Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeWorkingSectionIndex].SectionStatus != FEE_SECTION_ACTIVE) &&
      (ActiveReachedSecondTime == FALSE))
  {
    /* Exit condition, in order to avoid deadlock if no section is valid or no section with markers */
    if(Fee_Gv.FeeWorkingSectionIndex == Fee_Gv.FeeActiveSectionIndex)
    {
      ActiveReachedSecondTime = TRUE;
    }
    else
    {
      Fee_Gv.FeeWorkingSectionIndex = FEE_SECTIONDECREMENT(Fee_Gv.FeeWorkingSectionIndex);
    }
  }
  /* Exit condition. We arrived where we started from */
  if(Fee_Gv.FeeWorkingSectionIndex == Fee_Gv.FeeActiveSectionIndex)
  {
    ActiveReachedSecondTime = TRUE;
  }
#if (FEE_NUMBER_NOT_CONFIGURED_BLOCKS != 0U)
  Condition = FEE_CHECK_FLAG(FEE_NOT_CFG_BLK_LOST_MASK);
#endif
  if((ActiveReachedSecondTime == TRUE)
      /* Even if all the blocks are found, Fee shall still discover the old instances,
       * in case of possible restart switch situation. */
       /* !LINKSTO Fee.Dsn.Initialization.Startup1.BlocksSearchStop.SwNotConfigBlocksOFF,1 */
      || (((Fee_Gv.FeeNbOfBlocksFound == FEE_NO_OF_CONFIGURED_BLOCKS) &&
          (!FEE_CHECK_FLAG(FEE_HIGHEST_INTERNAL_PRIORITY_MASK)))
    /* !LINKSTO Fee.Dsn.Initialization.Startup1.BlocksSearchStop.SwNotConfigBlocksON,1 */
          && Condition)
    )
  {
    /* First part of initialization is complete. */
    Fee_StartUp1Finished();
  }
  else
  {
    /* First read of block infos will always be of a FEE_INIT_BUFFER_SIZE */
    Fee_Gv.Fee_BufferIndex = FEE_INIT_BUFFER_SIZE;
    /* Initialize init working addresses */
    Fee_Gv.FeeWorkingDataAddress = FEE_DATA_ADDRESS(Fee_Gv.FeeWorkingSectionIndex);
    Fee_Gv.FeeWorkingInfoAddress = FEE_FIRST_BLOCK_INFO_ADDRESS(Fee_Gv.FeeWorkingSectionIndex);

    /* Call flash read to read the block infos to buffer from the section to be
     * filled into cache */
    /* !LINKSTO Fee.Dsn.Initialization.Startup1.FillCacheWithBlockInfos,1 */
    if (E_OK == Fls_Read(FEE_FOOTER_ADDRESS(Fee_Gv.FeeWorkingSectionIndex) - FEE_INIT_BUFFER_SIZE,
                           Fee_Gv.FeeBuffPtr,
                           FEE_INIT_BUFFER_SIZE
                          )
         )
    {
      Fee_Gv.FeeFlashJobResult = MEMIF_JOB_PENDING;
    }

    /* flash read failed */
    else
    {
      Fee_FlsJobRefused();
    }
  }
}

/*--------------------------[Fee_CheckSectionManagementValidity]----------------------------------*/

STATIC FUNC(Fee_SectionStatus_t, FEE_CODE) Fee_CheckSectionManagementValidity
(
    void
)
{
  const uint8 ReadConfigId = FEE_READ_BYTE(&Fee_Gv.FeeBuffPtr[FEE_FOOTER_BUFFER_IDX_FOR_CRASH], FEE_CIDFIELD_OFFSET);
  const uint8 ReadECounterCrc = FEE_READ_BYTE(&Fee_Gv.FeeBuffPtr[FEE_FOOTER_BUFFER_IDX_FOR_CRASH], FEE_ECRCFIELD_OFFSET);
  const uint8 ReadCidCrc = FEE_READ_BYTE(&Fee_Gv.FeeBuffPtr[FEE_FOOTER_BUFFER_IDX_FOR_CRASH], FEE_CRCFIELD_OFFSET);
  const uint8 SectionCounter = FEE_READ_BYTE(&Fee_Gv.FeeBuffPtr[FEE_FOOTER_BUFFER_IDX_FOR_CRASH], FEE_SECTIONCOUNTERFIELD_OFFSET);
  const uint32 EraseCounter = FEE_READ_DWORD(&Fee_Gv.FeeBuffPtr[FEE_FOOTER_BUFFER_IDX_FOR_CRASH], FEE_SECTION_MANAGEMENT_OFFSET);
  uint8 ComputedCrc;
  Fee_SectionStatus_t  RetValue;

  /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.01,1 */
  if(((FEE_FLASH_ERASE_VALUE == ReadConfigId)
      && (FEE_FLASH_ERASE_VALUE == ReadECounterCrc))
      && (FEE_FLASH_ERASE_VALUE == SectionCounter)
      && (FEE_SECTION_STATUS_ERASED_COUNTER == EraseCounter))
  {
#if (FEE_CONSISTENCY_PATTERN_ENABLED == STD_ON)
    if (FEE_SECTION_STATUS_ERASED ==
        FEE_READ_WORD(Fee_Gv.FeeBuffPtr, FEE_SECTION_MANAGEMENT_END_PATTERN_OFFSET))
#endif
    {
      RetValue = FEE_SECTION_ERASED;
    }
#if (FEE_CONSISTENCY_PATTERN_ENABLED == STD_ON)
    else
    {
      RetValue = FEE_SECTION_INCONSISTENT;
    }
#endif
  }
  else
  {
#if (FEE_CONSISTENCY_PATTERN_ENABLED == STD_ON)
    if (FEE_SECTION_STATUS_ERASED !=
        FEE_READ_WORD(Fee_Gv.FeeBuffPtr, FEE_SECTION_MANAGEMENT_END_PATTERN_OFFSET))
    {
#endif
      ComputedCrc = Crc_CalculateCRC8(&Fee_Gv.FeeBuffPtr[FEE_FOOTER_BUFFER_IDX_FOR_CRASH + FEE_CIDFIELD_OFFSET], FEE_CID_CRC_COMPUTE_SIZE, FEE_CRC_START_VALUE, TRUE);
      /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.03,2 */
      if((ReadCidCrc == ComputedCrc) && ((ReadConfigId == FEE_CONFIG_ID) || (ReadConfigId == FEE_LEGACY_CONFIG_ID)))
      {
        RetValue = FEE_SECTION_EMPTY;
        /* Header valid / section consistent. Memorize its counter. */
        Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeWorkingSectionIndex].SCounter = SectionCounter;

        /* If valid check the erase counter and save its value if it is the highest found until now */
        /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.04,1 */
        /* !LINKSTO Fee.Sections.HeaderSecured,1 */
        ComputedCrc = Crc_CalculateCRC8(&Fee_Gv.FeeBuffPtr[FEE_FOOTER_BUFFER_IDX_FOR_CRASH + FEE_SECTION_MANAGEMENT_OFFSET], FEE_ECOUNTER_CRC_COMPUTE_SIZE, FEE_CRC_START_VALUE, TRUE);
        if(ReadECounterCrc == ComputedCrc)
        {
        /* !LINKSTO Fee.SectionEraseCounter,1 */
        /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.12,1 */
          if(EraseCounter > Fee_Gv.SectionEraseCounter)
          {
            Fee_Gv.SectionEraseCounter = EraseCounter;
          }
          else
          {
            /* This is not the section with the highest counter */
          }
        }
      }
      else
      {
        RetValue = FEE_SECTION_INCONSISTENT;
      }
#if (FEE_CONSISTENCY_PATTERN_ENABLED == STD_ON)
    }
    else
    {
      RetValue = FEE_SECTION_INCONSISTENT;
    }
#endif
  }
  return RetValue;
}

/*--------------------------[Fee_GetSectionManagementStatus]----------------------------------*/

STATIC FUNC(Fee_SectionStatus_t, FEE_CODE) Fee_GetSectionManagementStatus
(
    void
)
{
  boolean (*IsMarkedAsToBeErased)(void);
  boolean (*IsMarkedAsActive)(void);
  Fee_SectionStatus_t  SectionManagementStatus;
  boolean FirstBlockInfoEmpty = TRUE;

  /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.06,1 */
  SectionManagementStatus = Fee_CheckSectionManagementValidity();

  if(Fee_State == FEE_INIT_READ_HEADER)
  {
    IsMarkedAsActive = &Fee_IsHeaderMarkedAsActive;
    IsMarkedAsToBeErased = &Fee_IsHeaderMarkedAsToBeErased;
  }
  else
  {
    IsMarkedAsActive = &Fee_IsFooterMarkedAsActive;
    IsMarkedAsToBeErased = &Fee_IsFooterMarkedAsToBeErased;
#if (FEE_CRITICAL_BLOCK_NUMBER != 0U)
    FirstBlockInfoEmpty = Fee_CheckBlockInfoErased(0U);
#endif
  }

  if(IsMarkedAsToBeErased())
  {
    if(SectionManagementStatus == FEE_SECTION_EMPTY)
    {
      /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.08,1 */
      SectionManagementStatus = FEE_SECTION_ERASABLE;
    }
    else
    {
      /* FEE_SECTION_ERASED  or  FEE_SECTION_INCONSISTENT */
      SectionManagementStatus = FEE_SECTION_ERASABLE_INCONSISTENT;
    }
  }
  else
  {
    if(IsMarkedAsActive() || (FirstBlockInfoEmpty == FALSE))
    {
      if(SectionManagementStatus == FEE_SECTION_EMPTY)
      {
        /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.07,1 */
        SectionManagementStatus = FEE_SECTION_ACTIVE;
      }
      else
      {
        /* FEE_SECTION_ERASED  or  FEE_SECTION_INCONSISTENT */
        SectionManagementStatus = FEE_SECTION_ACTIVE_INCONSISTENT;
      }
    }
    else
    {
      /* Remains FEE_SECTION_ERASED  or  FEE_SECTION_INCONSISTENT or FEE_SECTION_EMPTY */
    }
  }

  return SectionManagementStatus;
}

/*--------------------------[Fee_IsHeaderMarkedAsToBeErased]---------------------------*/

STATIC FUNC(boolean, FEE_CODE) Fee_IsHeaderMarkedAsToBeErased
(
    void
)
{
  boolean RetValue;

  /* !LINKSTO Fee.Dsn.Initialization.Startup1.MarkerValidity,1 */
  if(FEE_HAS_ATLEAST_ONEBYTE_OF(&Fee_Gv.FeeBuffPtr[FEE_HEADER_ERASABLE_MARKER_OFFSET], FEE_SECTION_STATUS_ERASABLE))
  {
    RetValue = TRUE;
  }
  else
  {
    RetValue = FALSE;
  }
  return RetValue;
}

/*--------------------------[Fee_IsHeaderMarkedAsActive]---------------------------*/

STATIC FUNC(boolean, FEE_CODE) Fee_IsHeaderMarkedAsActive
(
    void
)
{
  boolean RetValue;

  /* !LINKSTO Fee.Dsn.Initialization.Startup1.MarkerValidity,1 */
  if(FEE_HAS_ATLEAST_ONEBYTE_OF(&Fee_Gv.FeeBuffPtr[FEE_HEADER_ACTIVE_MARKER_OFFSET], FEE_SECTION_STATUS_ACTIVE))
  {
    RetValue = TRUE;
  }
  else
  {
    RetValue = FALSE;
  }
  return RetValue;
}

/*--------------------------[Fee_IsFooterMarkedAsToBeErased]---------------------------*/

STATIC FUNC(boolean, FEE_CODE) Fee_IsFooterMarkedAsToBeErased
(
    void
)
{
  boolean RetValue;

  /* !LINKSTO Fee.Dsn.Initialization.Startup1.MarkerValidity,1 */
  if(FEE_HAS_ATLEAST_ONEBYTE_OF(&(Fee_Gv.FeeBuffPtr[(FEE_HEADER_ACTIVE_MARKER_OFFSET + FEE_FOOTER_BUFFER_IDX_FOR_CRASH)]), FEE_SECTION_STATUS_ERASABLE))
  {
    RetValue = TRUE;
  }
  else
  {
    RetValue = FALSE;
  }
  return RetValue;
}

/*--------------------------[Fee_IsFooterMarkedAsActive]---------------------------*/

STATIC FUNC(boolean, FEE_CODE) Fee_IsFooterMarkedAsActive
(
    void
)
{
  boolean RetValue;

  /* !LINKSTO Fee.Dsn.Initialization.Startup1.MarkerValidity,1 */
  if(FEE_HAS_ATLEAST_ONEBYTE_OF(&(Fee_Gv.FeeBuffPtr[(FEE_HEADER_ERASABLE_MARKER_OFFSET + FEE_FOOTER_BUFFER_IDX_FOR_CRASH)]), FEE_SECTION_STATUS_ACTIVE))
  {
    RetValue = TRUE;
  }
  else
  {
    RetValue = FALSE;
  }
  return RetValue;
}

/*--------------------------[Fee_DecideSectionStatus]----------------------------------*/

STATIC FUNC(void, FEE_CODE) Fee_DecideSectionStatus
(
    Fee_SectionStatus_t HeaderStatus,
    Fee_SectionStatus_t FooterStatus
)
{
  Fee_SectionStatus_t * WorkingSectionStatus = &Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeWorkingSectionIndex].SectionStatus;
  /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.03,2 */
  /* !LINKSTO Fee.Dns.FailureHandling.Behaviour.Startup1.BothSectionManagementObjectsFailed,1 */
  switch(HeaderStatus)
  {
  case FEE_SECTION_ERASED:
    if((FooterStatus == FEE_SECTION_ACTIVE_INCONSISTENT) ||
        (FooterStatus == FEE_SECTION_ERASABLE_INCONSISTENT) ||
        (FooterStatus == FEE_SECTION_ERASED))
    {
      *WorkingSectionStatus = FEE_SECTION_INCONSISTENT;
    }
    else
    {
      /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.07,1 */
      /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.08,1 */
      *WorkingSectionStatus = FooterStatus;
    }
    break;

  case FEE_SECTION_INCONSISTENT:
    if((FooterStatus == FEE_SECTION_ACTIVE) ||
        (FooterStatus == FEE_SECTION_ERASABLE) ||
        (FooterStatus == FEE_SECTION_EMPTY))
    {
      /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.07,1 */
      /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.08,1 */
      *WorkingSectionStatus = FooterStatus;
    }
    else
    {
      *WorkingSectionStatus = FEE_SECTION_INCONSISTENT;
    }
    break;

  case FEE_SECTION_EMPTY:
    if(FooterStatus == FEE_SECTION_ERASED)
    {
      *WorkingSectionStatus = FEE_SECTION_NO_FOOTER;
    }
    else if((FooterStatus == FEE_SECTION_INCONSISTENT) ||
        (FooterStatus == FEE_SECTION_EMPTY))
    {
      *WorkingSectionStatus = FEE_SECTION_EMPTY;
    }
    else if((FooterStatus == FEE_SECTION_ACTIVE) ||
        (FooterStatus == FEE_SECTION_ACTIVE_INCONSISTENT))
    {
      /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.07,1 */
      *WorkingSectionStatus = FEE_SECTION_ACTIVE;
    }
    else /* FEE_SECTION_ERASABLE_INCONSISTENT or FEE_SECTION_ERASABLE */
    {
      /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.08,1 */
      *WorkingSectionStatus = FEE_SECTION_ERASABLE;
    }
    break;

  case FEE_SECTION_ACTIVE:
    if((FooterStatus == FEE_SECTION_ERASABLE) ||
        (FooterStatus == FEE_SECTION_ERASABLE_INCONSISTENT))
    {
      /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.08,1 */
      *WorkingSectionStatus = FEE_SECTION_ERASABLE;
    }
    else
    {
      /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.07,1 */
      *WorkingSectionStatus = FEE_SECTION_ACTIVE;
    }
    break;
  case FEE_SECTION_ERASABLE:
    if((FooterStatus == FEE_SECTION_EMPTY) ||
        (FooterStatus == FEE_SECTION_ACTIVE))
    {
      /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.08,1 */
      *WorkingSectionStatus = FEE_SECTION_ERASABLE_HEADER;
    }
    else
    {
      /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.08,1 */
      *WorkingSectionStatus = FEE_SECTION_ERASABLE;
    }
    break;
  case FEE_SECTION_ACTIVE_INCONSISTENT:
    if((FooterStatus == FEE_SECTION_EMPTY) ||
        (FooterStatus == FEE_SECTION_ACTIVE))
    {
      /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.07,1 */
      *WorkingSectionStatus = FEE_SECTION_ACTIVE;
    }
    else if(FooterStatus == FEE_SECTION_ERASABLE)
    {
      /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.08,1 */
      *WorkingSectionStatus = FEE_SECTION_ERASABLE;
    }
    else
    {
      *WorkingSectionStatus = FEE_SECTION_INCONSISTENT;
    }
    break;
  case FEE_SECTION_ERASABLE_INCONSISTENT:
    if((FooterStatus == FEE_SECTION_EMPTY) ||
        (FooterStatus == FEE_SECTION_ACTIVE))
    {
      /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.08,1 */
      *WorkingSectionStatus = FEE_SECTION_ERASABLE_HEADER;
    }
    else if(FooterStatus == FEE_SECTION_ERASABLE)
    {
      /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.08,1 */
      *WorkingSectionStatus = FEE_SECTION_ERASABLE;
    }
    else
    {
      *WorkingSectionStatus = FEE_SECTION_INCONSISTENT;
    }
    break;

    /* CHECK: NOPARSE */
    default:
      FEE_UNREACHABLE_CODE_ASSERT(FEE_INTERNAL_API_ID);
      break; /* Caution: this 'break' is required by MISRA-C:2004 */
      /* CHECK: PARSE */
  }
}

/*--------------------------[Fee_FindNewestActiveSection]----------------------------------*/

STATIC FUNC(void, FEE_CODE) Fee_FindNewestActiveSection
(
    void
)
{
  uint8 CountSections = 0U;
  boolean ActiveSectionFound = FALSE;

  if(Fee_Gv.FeeActiveSectionIndex != 255)
  {
    /* Go to the newest consistent section */
    Fee_Gv.FeeWorkingSectionIndex = Fee_Gv.FeeActiveSectionIndex;
    /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.10,1 */
    Fee_Gv.FeeOldestSectionIndex = Fee_Gv.FeeActiveSectionIndex;

    /* Newest section found. Go through sections in order to find the Active section*/
    do{
      if(((Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeWorkingSectionIndex].SectionStatus == FEE_SECTION_EMPTY) ||
          (Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeWorkingSectionIndex].SectionStatus == FEE_SECTION_NO_FOOTER)) ||
          (Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeWorkingSectionIndex].SectionStatus == FEE_SECTION_INCONSISTENT))
      {
        if(ActiveSectionFound == FALSE)
        {
          if(Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeWorkingSectionIndex].SectionStatus != FEE_SECTION_INCONSISTENT)
          {
            /* It is possible that all the sections to be empty.
             * Fee shall write the first block in the oldest section. */
            /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.10,1 */
            Fee_Gv.FeeOldestSectionIndex = Fee_Gv.FeeWorkingSectionIndex;
          }
        }
        else
        {
          /* The section is inconsistent */
        }
      }
      else if(Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeWorkingSectionIndex].SectionStatus == FEE_SECTION_ACTIVE)
      {
        if(ActiveSectionFound == FALSE)
        {
          /* This can be considered the Active section */
          ActiveSectionFound = TRUE;
          /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.09,1 */
          Fee_Gv.FeeActiveSectionIndex = Fee_Gv.FeeWorkingSectionIndex;
        }
        else
        {
          /* Inconsistent sections found among the active sections are considered full. They will be healed by switch */
        }
        /* This is possibly the oldest section with data */
        /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.10,1 */
        Fee_Gv.FeeOldestSectionIndex = Fee_Gv.FeeWorkingSectionIndex;
      }
      else
      {
        /* Section is either FEE_SECTION_ERASABLE or FEE_SECTION_ERASABLE_HEADER/FOOTER */
      }

      Fee_Gv.FeeWorkingSectionIndex = FEE_SECTIONDECREMENT(Fee_Gv.FeeWorkingSectionIndex);
      CountSections++;
    }while(CountSections != FEE_NUMBER_OF_SECTIONS);
  }
  else
  {
    /* There are no consistent sections. Take 0 as an arbitrary value to consider for the oldest section. */
    Fee_Gv.FeeOldestSectionIndex = 0U;
  }

  if(ActiveSectionFound == FALSE)
  {
    /* No data can be retrieved at init. */
#ifdef FEE_E_DATA_RECOVERED_DEM_EVENT_ID
     /* If neither of the section headers could be read successfully, a production
     * error shall be raised. Additionally, the entire flash shall be erased as part
     * of start-up in order to restore Fee's functionality.
     */
      /* !LINKSTO Fee.FailureHandling.Behaviour.Startup1.ProductionError,1 */
      /* Notify user by reporting a production error */
      Dem_ReportErrorStatus(FEE_E_DATA_RECOVERED_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#endif
    /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.09,1 */
    Fee_Gv.FeeActiveSectionIndex = Fee_Gv.FeeOldestSectionIndex;
  }
  else
  {
    if(FEE_SECTIONINCREMENT(Fee_Gv.FeeActiveSectionIndex) == Fee_Gv.FeeOldestSectionIndex)
    {
      /* !LINKSTO Fee.Dsn.Initialization.Startup1.Sections.11,1 */
      /* There is a switch-no-suspend situation. Set the flag. */
      FEE_SET_FLAG(FEE_HIGHEST_INTERNAL_PRIORITY_MASK);
    }
    else
    {
      /* Nothing to do. */
    }
  }
}


/*--------------------------[Fee_StartUp1Finished]----------------------------------*/

STATIC FUNC(void, FEE_CODE) Fee_StartUp1Finished
(
    void
)
{
  if(Fee_Gv.Fee_SectionManagement[Fee_Gv.FeeActiveSectionIndex].SectionStatus == FEE_SECTION_ACTIVE)
  {
    if(FEE_CHECK_FLAG(FEE_HIGHEST_INTERNAL_PRIORITY_MASK))
    {
      Fee_Gv.FeeMode = FEE_MODE_SWITCH;
      /* Check how much space is needed for high priority switch */
      Fee_Gv.FeeSwitchRequiredSpace = Fee_CalculateSSRequiredSpace();
      /* !LINKSTO Fee.Dsn.Initialization.Idle,1 */
      /* This must be the only time the Idle state is triggered in highest priority switch.
       * The reason is for accepting read requests for ReadAll */
      /* !LINKSTO Fee.Dns.SS.RestartSwitch.TriggerAfterInit,1 */
    }
    else
    {
      Fee_Gv.FeeMode = FEE_MODE_STARTUP2;
      /* The Active section is ACTIVE. StartUp2 must start with the next section. */
      Fee_Gv.FeeWorkingSectionIndex = FEE_SECTIONINCREMENT(Fee_Gv.FeeActiveSectionIndex);
      /* !LINKSTO Fee.Dsn.Initialization.Idle,1 */
      /* !LINKSTO FEE169,1 */
    }
#if (FEE_CRITICAL_BLOCK_NUMBER != 0U)
    if (TRUE == Fee_CriticalBlockInit())
    {
      /* Do nothing. The critical block related activity has been handled already. */
    }
    else
#endif /* (FEE_CRITICAL_BLOCK_NUMBER != 0U) */
    {
      /* If there is no job to be processed Fee will go Idle. */
      Fee_InitiateJob();
    }
  }
  else
  {
    Fee_Gv.FeeMode = FEE_MODE_STARTUP2;
    /* The active section is either empty or inconsistent. StartUp2 must start with it. */
    Fee_Gv.FeeWorkingSectionIndex = Fee_Gv.FeeActiveSectionIndex;
    FEE_SET_FLAG(FEE_HIGHEST_INTERNAL_PRIORITY_MASK);
    Fee_StartUp2();
  }
}

#if (FEE_CRITICAL_BLOCK_NUMBER != 0U)
/*--------------------------[Fee_CriticalBlockInit]----------------------------------*/

STATIC FUNC(boolean, FEE_CODE) Fee_CriticalBlockInit
(
    void
)
{
  boolean CriticalBlockFound = TRUE;
  const uint16 CriticalBlockIndex = Fee_SearchConfigTable(FEE_CRITICAL_BLOCK_NUMBER);
  /* Check whether the critical block was found. */
  if (FALSE == Fee_IsCriticalBlockErased(CriticalBlockIndex))
  {
    /* For symmetry. Just as the critical block has just been written.
     * This time FeeCriticalState should always be Idle because Fee is still in initialization phase
     * and ReadAll has not been performed yet. */
    Fee_Gv.FeeCriticalState = FEE_IDLE;
    if (FEE_NO_JOB != Fee_Gv.FeeJob)
    {
      /* Any job for a non-critical block must be canceled in order to make room for critical jobs. */
      if (CriticalBlockIndex != Fee_Gv.FeeIntBlockIndex)
      {
        /* !LINKSTO Fee.FeeCriticalBlock.CancelNvMJob,1 */
        Fee_CancelJob();
        /* It's not NvM that canceled this job. NvM is the only one that should. Consider job as failed. */
        Fee_Gv.FeeJobResult = MEMIF_JOB_FAILED;
        /* Call NVM's error notification function */
        FEE_NVM_JOB_ERROR_NOTIFICATION();
        /* Freeze Fee */
        /* !LINKSTO Fee.FeeCriticalBlock.InitFreeze,1 */
        FEE_TRIGGER_TRANS(FEE_CRITICAL_FROZEN);
      }
      else
      {
        /* Read or EraseImmediate job for critical block will be done. */
        /* !LINKSTO Fee.FeeCriticalBlock.StopBlockJobs,1 */
        Fee_InitiateJob();
      }
    }
    else
    {
      /* Freeze Fee */
      /* !LINKSTO Fee.FeeCriticalBlock.InitFreeze,1 */
      FEE_TRIGGER_TRANS(FEE_CRITICAL_FROZEN);
    }
  }
  else
  {
    CriticalBlockFound = FALSE;
  }
  return CriticalBlockFound;
}

#endif /* (FEE_CRITICAL_BLOCK_NUMBER != 0U) */

#define FEE_STOP_SEC_CODE
#include <MemMap.h>

/*==================[end of file]===========================================*/
