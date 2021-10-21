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
/*  MISRA-C:2012 Deviation List
 *
 *  MISRAC2012-1) Deviated Rule: 11.5 (advisory)
 *    "A conversion should not be performed from pointer to void into pointer to object."
 *
 *    Reason: The portion of code is necessary and it has no alignment problems.
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
#include <NvM.h>                  /* Extern NVRAM Manager API definitions   */

#include <NvM_FunctionIntern.h>   /* NvM functions which are used           */
                                  /* only within the NvM module.            */
#include <NvM_DataIntern.h>       /* NvM data which is used only            */
                                  /* within the NvM module.                 */
#include <NvM_Queue.h>            /* queue external definitions.            */
#include <NvM_CalcCrc.h> /* Contains extern definitions for unit 'CalcCrc'. */
#include <NvM_StateMachine.h>     /* Contains extern definitions for unit   */
                                  /* 'StateMachine'. It can be included in  */
                                  /* library source code files.             */

#if ( NVM_INCLUDE_RTE == STD_ON )
#include <Rte_NvM.h>           /* Rte type definitions. */
#endif

/*==================[macros]================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]========================*/
#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>

/** \brief Copies default data from default ROM block to user RAM block.
 **
 **        Shall be called only for blocks with default ROM
 ** \param[in] CurrentRamAddress: Pointer to the data buffer in RAM
 **/
STATIC FUNC(void,NVM_CODE) NvM_CopyBlockRom2Ram
(
  NvM_PtrToApplDataType CurrentRamAddress
);

#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>
/*==================[external function definitions]=========================*/

#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>

#if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)

FUNC(Std_ReturnType,NVM_CODE) NvM_ASR40_RestoreBlockDefaults
(
  NvM_ASR40_BlockIdType                 BlockId,
  P2VAR(void, AUTOMATIC, NVM_APPL_DATA) NvM_DestPtr
)
{
  uint16 BlockIndex;
  Std_ReturnType Result = E_NOT_OK;

  DBG_NVM_ASR40_RESTOREBLOCKDEFAULTS_ENTRY(BlockId, NvM_DestPtr);

#ifndef NVM_RESTOREBLOCKDEFAULTS_CALLOUT
  BlockIndex = NvM_CheckCondRestoreBlockDefaults(BlockId, NvM_DestPtr);
  if(BlockIndex != NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS)
#else
  /* If multi-core wrapper is active the parameter BlockId is actually the block index */
  BlockIndex = (uint16)BlockId;
#endif
  {
    Result = NvM_InsertRestoreBlockJob(FALSE, BlockId, BlockIndex, NvM_DestPtr);
  }

  DBG_NVM_ASR40_RESTOREBLOCKDEFAULTS_EXIT(Result, BlockId, NvM_DestPtr);

  return Result;
}/* end of function NvM_RestoreBlockDefaults */


/* !LINKSTO SWS_NvM_00813,1 */
FUNC(Std_ReturnType,NVM_CODE) NvM_ASR42_RestorePRAMBlockDefaults
(
  NvM_ASR42_BlockIdType BlockId
)
{
  uint16 BlockIndex;
  Std_ReturnType Result = E_NOT_OK;   /* Result of this function        */

  DBG_NVM_ASR42_RESTOREPRAMBLOCKDEFAULTS_ENTRY(BlockId);

#ifndef NVM_RESTOREPRAMBLOCKDEFAULTS_CALLOUT
  BlockIndex = NvM_CheckCondRestorePRAMBlockDefaults(BlockId);
  if(BlockIndex != NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS)
#else
  /* If multi-core wrapper is active the parameter BlockId is actually the block index */
  BlockIndex = (uint16)BlockId;
#endif
  {
    Result = NvM_InsertRestoreBlockJob(TRUE, BlockId, BlockIndex, NULL_PTR);
  }

  DBG_NVM_ASR42_RESTOREPRAMBLOCKDEFAULTS_EXIT(Result, BlockId);

  return Result;
}/* end of function NvM_ASR42_RestorePRAMBlockDefaults */

FUNC(void,NVM_CODE) NvM_RestoreBlockDefaults_Asc(void)
{
  /* Since the processing of NvM_RestoreBlockDefaults() has
   * been started, NvM_CurrentServiceId needs to be set
   */
  NvM_CurrentServiceId = NVM_RESTORE_BLOCK_DEFAULTS_API_ID;

  /* if a permanent RAM block is used then it must be set invalid
   * and unchanged.
   */
#if (NVM_SYNC_MECHANISM == STD_ON)
  if (!(NVM_CHECK_WORKING_STATUS(NVM_DYN_MIRROR_MASK)))
#endif
  {
    /* !LINKSTO SWS_NvM_00227,1 */
    NvM_SetPermanentRamBlockInvalid();
    NvM_SetPermanentRamBlockUnchanged();
  }
  NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_RestoreBlockDefaults_AscSt0;

#if ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U))
   if ((NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMExtendedStatus &
        NVM_ES_RAM_BLOCK_CRC_CALC_MASK
        ) == NVM_ES_RAM_BLOCK_CRC_CALC_MASK
      )
  {
     /* Reset the RAM block CRC calculation flag */
     NvM_CalcCrc_RemoveElement();
     /* Ram block CRC will be recalculated again */
  }
#endif

}/* end of function NvM_RestoreBlockDefaults_Asc  */

FUNC(void,NVM_CODE) NvM_RestorePRAMBlockDefaults_Asc(void)
{
  /* Since the processing of NvM_RestorePRAMBlockDefaults() has
   * been started, NvM_CurrentServiceId needs to be set
   */
  NvM_CurrentServiceId = NVM_RESTORE_PRAM_BLOCK_DEFAULTS_API_ID;

  /* if a permanent RAM block is used then it must be set invalid
   * and unchanged.
   */
#if (NVM_SYNC_MECHANISM == STD_ON)
  if (!(NVM_CHECK_WORKING_STATUS(NVM_DYN_MIRROR_MASK)))
#endif
  {
    /* !LINKSTO SWS_NvM_00819,1 */
    NvM_SetPermanentRamBlockInvalid();
    NvM_SetPermanentRamBlockUnchanged();
  }
  NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_RestoreBlockDefaults_AscSt0;

#if ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U))
   if ((NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMExtendedStatus &
        NVM_ES_RAM_BLOCK_CRC_CALC_MASK
        ) == NVM_ES_RAM_BLOCK_CRC_CALC_MASK
      )
  {
    /* Reset the RAM block CRC calculation flag */
    /* !LINKSTO SWS_NvM_00821,1 */
    NvM_CalcCrc_RemoveElement();
    /* Ram block CRC will be recalculated again */
  }
#endif

}/* end of function NvM_RestorePRAMBlockDefaults_Asc  */

#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1) */

FUNC(void,NVM_CODE) NvM_RestoreBlockDefaults_AscSt0(void)
{
#if (NVM_SYNC_MECHANISM == STD_ON)
  /* reset mirror retry counter */
  NvM_MirrorRetryCount = 0U;
#endif
  /* reset ROM Data Loaded Flag */
  NVM_CLEAR_WORKING_STATUS(NVM_DYN_RDL_MASK);
  NvM_RestoreBlockDefaults_AscSt1();
}/* NvM_RestoreBlockDefaults_AscSt0 */

FUNC(void,NVM_CODE) NvM_RestoreBlockDefaults_AscSt1(void)
{
  boolean DefaultsConfigured = TRUE;
#if ( NVM_NUMBER_OF_INIT_BLOCK_CALLBACK > 0U )
  /* Result of the Init block callback function. */
  Std_ReturnType InitCallbackResult = E_NOT_OK;
#if (NVM_SYNC_MECHANISM == STD_ON)
  /* Result of the Mirror block callback function. */
  Std_ReturnType MirrorCallbackResult = E_OK;
#endif
#endif
  /* CurrentRamAddress: address of the target user RAM data buffer */
  NvM_PtrToApplDataType CurrentRamAddress;

  CurrentRamAddress = NvM_GetUserRamBlockAddress();
  if (CurrentRamAddress != NULL_PTR)
  {
    /* Set NvM_GlobalErrorStatus to NVM_REQ_NOT_OK. In case restoring data  succeeds,
     *  the status will be set to NVM_REQ_OK.
     */
    DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_NOT_OK));
    NvM_GlobalErrorStatus = NVM_REQ_NOT_OK;

    if(NvM_CurrentBlockDescriptorPtr->romBlockDataAddress != NULL_PTR)
    {
      NvM_CopyBlockRom2Ram(CurrentRamAddress);
    }
    else
    {
  #if ( NVM_NUMBER_OF_INIT_BLOCK_CALLBACK > 0U )
      if (NvM_CurrentBlockDescriptorPtr->initBlockCallback != NULL_PTR)
      {
        InitCallbackResult = NvM_CurrentBlockDescriptorPtr->initBlockCallback();
    #if (NVM_SYNC_MECHANISM == STD_ON)
        /* get the data from the application in order to calculate CRC */
        if ((InitCallbackResult == E_OK)&&(NVM_CHECK_WORKING_STATUS(NVM_DYN_MIRROR_MASK)))
        {
          /* in case of mirror retries set the current level function pointer */
          NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_RestoreBlockDefaults_AscSt1;
          /* !LINKSTO NvM.RestoreBlockDefaults.ExplicitSynch.InitCallback,1 */
          MirrorCallbackResult = NvM_MirrorCopy(NULL_PTR,
                NvM_CurrentBlockDescriptorPtr->writeRamToNvCallback, CurrentRamAddress);
        }
    #endif
      }
      else
  #endif /* NVM_NUMBER_OF_INIT_BLOCK_CALLBACK > 0U */
      {
        DefaultsConfigured = FALSE;
      }
    }
    if(DefaultsConfigured)
    {
  #if ( ( NVM_NUMBER_OF_INIT_BLOCK_CALLBACK > 0U ) && ( NVM_SYNC_MECHANISM == STD_ON ) )
      if ( ( NvM_GlobalErrorStatus == NVM_REQ_OK ) ||
           ( ( InitCallbackResult == E_OK ) &&
             ( MirrorCallbackResult == E_OK ) )
         )
  #elif ( ( NVM_NUMBER_OF_INIT_BLOCK_CALLBACK > 0U ) && ( NVM_SYNC_MECHANISM == STD_OFF ) )
      if ( ( NvM_GlobalErrorStatus == NVM_REQ_OK ) ||
           ( InitCallbackResult == E_OK )
         )
  #else
        /* if no init callback, defaults were copied from the ROM */
  #endif
      {
        /* Default data was copied successfully. */
        if ( NVM_BD_CALCRAMBLOCKCRC(NvM_CurrentBlockDescriptorPtr->blockDesc) )
        {
          /* CRC must be calculated */
          NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_RestoreBlockDefaults_AscSt2;
        }
        else
        {
          /* CRC must not be calculated */
          NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_RestoreBlockDefaults_AscSt4;
        }
      } /* if ( NvM_GlobalErrorStatus == NVM_REQ_OK ) */
#if ( NVM_NUMBER_OF_INIT_BLOCK_CALLBACK > 0U )
      else
      {
        /* if no init from ROM block nor via callback was performed */
        if ( InitCallbackResult != E_OK )
        {
          NvM_StateMachine_TerminateCurrentLevel();
        }
      }/* end of else-if ( NvM_GlobalErrorStatus == NVM_REQ_OK ) */
#endif
    }
    else
    {
      NvM_GlobalErrorStatus = NVM_REQ_INTEGRITY_FAILED;
      NvM_StateMachine_TerminateCurrentLevel();
    }
  }
  else
  {/*Handled by NvM_GetUserRamBlockAddress()*/
  }
}/* end of function NvM_RestoreBlockDefaults_AscSt1  */

FUNC(void,NVM_CODE) NvM_RestoreBlockDefaults_AscSt2(void)
{
  NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_RestoreBlockDefaults_AscSt3;
  /* NOTE: NvM_GlobalCallLevel must not be incremented here, it's done in
   *       NvM_CalculateCrc()
   */
  NvM_CalculateCrc( NvM_GetUserRamBlockAddress() );
}/* end of NvM_RestoreBlockDefaults_AscSt2 */

FUNC(void,NVM_CODE) NvM_RestoreBlockDefaults_AscSt3(void)
{
  NvM_PtrToApplDataType DataAddress;

  NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_RestoreBlockDefaults_AscSt4;

  /* Copy calculated CRC to the RAM CRC Buffer */
  NvM_CopyCrc( NvM_GetStoredRamCRCAddress(),
              (NvM_PtrToApplDataType)&NvM_CalcCrc_CalcBuffer,
               NvM_CurrentBlockDescriptorPtr->blockDesc);

  /* If user provides space for CRC and permanent RAM is used for the current request,
   * copy user data and CRC */
  if ((NVM_BD_USERPROVIDESSPACEFORCRC(NvM_CurrentBlockDescriptorPtr->blockDesc)) &&
      (NVM_CHECK_WORKING_STATUS(NVM_DYN_UPRB_MASK)))
  {
    /* Copy calculated CRC to the user buffer */
    DataAddress = NvM_GetUserRamBlockAddress();
    DataAddress = &(DataAddress[NvM_CurrentBlockDescriptorPtr->nvBlockLength]);
    /* Copy CRC from the CRC Buffer */
    NvM_CopyCrc( DataAddress,
                (NvM_PtrToApplDataType)&NvM_CalcCrc_CalcBuffer,
                 NvM_CurrentBlockDescriptorPtr->blockDesc);
  }
}/* end of function NvM_RestoreBlockDefaults_AscSt3 */

FUNC(void,NVM_CODE) NvM_RestoreBlockDefaults_AscSt4(void)
{
#if (NVM_SYNC_MECHANISM == STD_ON)
  Std_ReturnType ReturnVal = E_OK;
#endif

  #if (NVM_SYNC_MECHANISM == STD_ON)
  /* Call the callback function to copy data from NvM module's mirror to RAM block */
  if (NVM_CHECK_WORKING_STATUS(NVM_DYN_MIRROR_MASK))
  {
    /* Set the block status to invalid and unchanged */
    /* !LINKSTO SWS_NvM_00227,1 */
    NvM_SetPermanentRamBlockInvalid();
    NvM_SetPermanentRamBlockUnchanged();
    ReturnVal = NvM_MirrorCopy(NvM_CurrentBlockDescriptorPtr->readRamFromNvCallback, NULL_PTR,
      NVM_GET_MIRROR_ADDR());
  }

  if (E_OK == ReturnVal)
#endif
  {
    /* set ROM data loaded flag */
    NVM_SET_WORKING_STATUS( NVM_DYN_RDL_MASK );

    /* Default data was copied successfully from ROM to RAM. Set permanent
     * RAM Block valid and changed. NVM366,SWS_NvM_00228,NVM367.
     */
    /* !LINKSTO SWS_NvM_00228,1 */
    NvM_SetPermanentRamBlockValid();
    NvM_SetPermanentRamBlockChanged();

    /* NvM_GlobalErrorStatus must be set to NVM_REQ_RESTORED_DEFAULTS because
     * either InitBlockCallback provided default values, either data was restored from ROM.
     */
    DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_RESTORED_DEFAULTS));
    NvM_GlobalErrorStatus = NVM_REQ_RESTORED_DEFAULTS;

    NvM_StateMachine_TerminateCurrentLevel();
  }
}/* NvM_RestoreBlockDefaults_AscSt4 */

/*==================[internal function definitions]=========================*/

STATIC FUNC(void,NVM_CODE) NvM_CopyBlockRom2Ram
(
  NvM_PtrToApplDataType CurrentRamAddress
)
{
  uint8  NvMDatasetBlockIndex = 0U;

  P2CONST(uint8,AUTOMATIC,NVM_APPL_CONST) CurrentRomAddress
    = NvM_CurrentBlockDescriptorPtr->romBlockDataAddress;

/* !LINKSTO NVM150,1 */
#if ( NVM_NUMBER_OF_DATASET_BLOCKS > 0U )
  if ( NVM_BD_BLOCKMNGTTYPE(NvM_CurrentBlockDescriptorPtr->blockDesc) ==
       NVM_BD_BLOCKMNGTTYPE_DATASET
     )
  {
    /* A Dataset Block uses the ROM Block specified by the current
     * value of the Data Index;
     * Validation of index is done when Restore Block Defaults or Read Block
     * services are requested.
     * Defaults restore is not called internally by Read All
     */
    NvMDatasetBlockIndex = NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMDatasetBlockIndex;
  }
  else
#endif /* NVM_NUMBER_OF_DATASET_BLOCKS > 0U */
  {
    /* Native and Redundant blocks only have a single ROM block */
    NvMDatasetBlockIndex = NvM_CurrentBlockDescriptorPtr->nvBlockNum;
  }

  /* CurrentRomAddress: address of the source ROM data buffer which
   * contains the default data.
   * It must be calculated from the basic Rom block data address plus the
   * offset of the current dataset indicated by parameter
   * NvMDatasetBlockIndex.
   */
  CurrentRomAddress =
    &( CurrentRomAddress[
                          NvM_CurrentBlockDescriptorPtr->nvBlockLength
                          *
                          ((uint16)NvMDatasetBlockIndex - NvM_CurrentBlockDescriptorPtr->nvBlockNum)
                         ]
     );

  TS_MemCpy( CurrentRamAddress,
             CurrentRomAddress,
             NvM_CurrentBlockDescriptorPtr->nvBlockLength
           );

  /* If the default data is copied successfully the job result shall
   * be set to NVM_REQ_OK. NVM354.
   */

  DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_OK));
  NvM_GlobalErrorStatus = NVM_REQ_OK;
}/* end of function NvM_CopyBlockRom2Ram  */

#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>

/*==================[end of file]===========================================*/
