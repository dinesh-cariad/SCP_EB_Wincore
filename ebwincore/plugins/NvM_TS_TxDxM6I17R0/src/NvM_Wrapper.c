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
/*==================[inclusions]============================================*/

#include <Std_Types.h>                                               /* AUTOSAR standard types */
#include <TSAutosar.h>                                               /* EB specific standard types */

#include <NvM.h>                                                     /* configuration header*/
#include <NvM_Wrapper.h>                                             /* own header */
#include <NvM_FunctionIntern.h>                                      /* own header */
#include <NvM_DataIntern.h>                                          /* User Header Needed for Callout declaration*/
#if (defined NVM_INTERNAL_USE)
#error NVM_INTERNAL_USE is already defined
#endif
#define NVM_INTERNAL_USE

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

#define NVM_START_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>

#if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)

#if (defined (NVM_READBLOCK_CALLOUT))
/*======================NvM_Wrapper_ReadBlock==========================*/
FUNC(Std_ReturnType, NVM_CODE) NvM_Wrapper_ReadBlock
(
  NvM_BlockIdType BlockId,
  P2VAR(void, AUTOMATIC, NVM_APPL_DATA) NvM_DstPtr
)
{
  /* !LINKSTO NvM.MultiCore.ReadCallout_PreVerification,1 */
  uint16 BlockIndex = NvM_CheckCondReadBlock(BlockId, NvM_DstPtr);
  Std_ReturnType ret = E_NOT_OK;

  /* Check block is not already pending for the case when DET is disabled */
  if(BlockIndex != NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS)
  {
    if( NvM_AdminBlockTable[BlockIndex].NvMResult != NVM_REQ_PENDING )
    {
      ret = NVM_READBLOCK_CALLOUT(BlockIndex, NvM_DstPtr);
      if (ret == E_NOT_OK)
      {
        NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus &= (uint8)(~(NVM_ES_MULTICORE_LOCK_MASK));
      }
    }
    else
    {
      NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus &= (uint8)(~(NVM_ES_MULTICORE_LOCK_MASK));
    }
  }
  return ret;
}
#endif /* defined NVM_READBLOCK_CALLOUT */

#if (defined(NVM_WRITEBLOCK_CALLOUT))
/*======================NvM_Wrapper_WriteBlock==========================*/
FUNC(Std_ReturnType, NVM_CODE) NvM_Wrapper_WriteBlock
(
  NvM_BlockIdType BlockId,
  P2CONST(void, AUTOMATIC, NVM_APPL_DATA) NvM_SrcPtr
)
{
  /* !LINKSTO NvM.MultiCore.WriteCallout_PreVerification,1 */
  uint16 BlockIndex = NvM_CheckCondWriteBlock(BlockId, NvM_SrcPtr);
  Std_ReturnType ret = E_NOT_OK;
  if(BlockIndex != NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS)
  {
    if( NvM_AdminBlockTable[BlockIndex].NvMResult != NVM_REQ_PENDING )
    {
      ret = NVM_WRITEBLOCK_CALLOUT(BlockIndex, NvM_SrcPtr);
      if (ret == E_NOT_OK)
      {
        NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus &= (uint8)(~(NVM_ES_MULTICORE_LOCK_MASK));
      }
    }
    else
    {
      NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus &= (uint8)(~(NVM_ES_MULTICORE_LOCK_MASK));
    }
  }
  return ret;
}
#endif /* defined NVM_WRITEBLOCK_CALLOUT */

#if (defined(NVM_RESTOREBLOCKDEFAULTS_CALLOUT))
/*======================NvM_Wrapper_RestoreBlockDefaults==========================*/
FUNC(Std_ReturnType,NVM_CODE) NvM_Wrapper_RestoreBlockDefaults
(
  NvM_BlockIdType BlockId,
  P2VAR(void, AUTOMATIC, NVM_APPL_DATA) NvM_DstPtr
)
{
  /* !LINKSTO NvM.MultiCore.RestoreBlockDefaultsCallout_PreVerification,1 */
  uint16 BlockIndex = NvM_CheckCondRestoreBlockDefaults(BlockId, NvM_DstPtr);
  Std_ReturnType ret = E_NOT_OK;
  if(BlockIndex != NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS)
  {
    if( NvM_AdminBlockTable[BlockIndex].NvMResult != NVM_REQ_PENDING )
    {
      ret = NVM_RESTOREBLOCKDEFAULTS_CALLOUT(BlockIndex, NvM_DstPtr);
      if (ret == E_NOT_OK)
      {
        NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus &= (uint8)(~(NVM_ES_MULTICORE_LOCK_MASK));
      }
    }
    else
    {
      NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus &= (uint8)(~(NVM_ES_MULTICORE_LOCK_MASK));
    }
  }
  return ret;
}
#endif /* defined NVM_RESTOREBLOCKDEFAULTS_CALLOUT */

#if (defined(NVM_READPRAMBLOCK_CALLOUT))
/*======================NvM_Wrapper_ReadPRAMBlock==========================*/
FUNC(Std_ReturnType, NVM_CODE) NvM_Wrapper_ReadPRAMBlock
(
  NvM_BlockIdType BlockId
)
{
  /* !LINKSTO NvM.MultiCore.ReadPRAMBlockCallout_PreVerification,1 */
  uint16 BlockIndex = NvM_CheckCondReadPRAMBlock(BlockId);
  Std_ReturnType ret = E_NOT_OK;

  if(BlockIndex != NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS)
  {
    if( NvM_AdminBlockTable[BlockIndex].NvMResult != NVM_REQ_PENDING )
    {
      ret = NVM_READPRAMBLOCK_CALLOUT(BlockIndex);
      if (ret == E_NOT_OK)
      {
        NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus &= (uint8)(~(NVM_ES_MULTICORE_LOCK_MASK));
      }
    }
    else
    {
      NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus &= (uint8)(~(NVM_ES_MULTICORE_LOCK_MASK));
    }
  }
  return ret;
}
#endif /* defined NVM_READPRAMBLOCK_CALLOUT */

#if (defined(NVM_WRITEPRAMBLOCK_CALLOUT))
/*======================NvM_Wrapper_WritePRAMBlock==========================*/
FUNC(Std_ReturnType, NVM_CODE) NvM_Wrapper_WritePRAMBlock
(
  NvM_BlockIdType BlockId
)
{
  /* !LINKSTO NvM.MultiCore.WritePRAMBlockCallout_PreVerification,1 */
  uint16 BlockIndex = NvM_CheckCondWritePRAMBlock(BlockId);
  Std_ReturnType ret = E_NOT_OK;

  if(BlockIndex != NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS)
  {
    if( NvM_AdminBlockTable[BlockIndex].NvMResult != NVM_REQ_PENDING )
    {
      ret = NVM_WRITEPRAMBLOCK_CALLOUT(BlockIndex);
      if (ret == E_NOT_OK)
      {
        NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus &= (uint8)(~(NVM_ES_MULTICORE_LOCK_MASK));
      }
    }
    else
    {
      NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus &= (uint8)(~(NVM_ES_MULTICORE_LOCK_MASK));
    }
  }
  return ret;
}
#endif /* defined NVM_WRITEPRAMBLOCK_CALLOUT */

#if (defined(NVM_RESTOREPRAMBLOCKDEFAULTS_CALLOUT))
/*======================NvM_Wrapper_RestorePRAMBlockDefaults==========================*/
FUNC(Std_ReturnType,NVM_CODE) NvM_Wrapper_RestorePRAMBlockDefaults
(
  NvM_BlockIdType BlockId
)
{
  /* !LINKSTO NvM.MultiCore.RestorePRAMBlockDefaultsCallout_PreVerification,1 */
  uint16 BlockIndex = NvM_CheckCondRestorePRAMBlockDefaults(BlockId);
  Std_ReturnType ret = E_NOT_OK;

  if(BlockIndex != NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS)
  {
    if( NvM_AdminBlockTable[BlockIndex].NvMResult != NVM_REQ_PENDING )
    {
      ret = NVM_RESTOREPRAMBLOCKDEFAULTS_CALLOUT(BlockIndex);
      if (ret == E_NOT_OK)
      {
        NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus &= (uint8)(~(NVM_ES_MULTICORE_LOCK_MASK));
      }
    }
    else
    {
      NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus &= (uint8)(~(NVM_ES_MULTICORE_LOCK_MASK));
    }
  }
  return ret;
}
#endif /* defined NVM_RESTOREPRAMBLOCKDEFAULTS_CALLOUT */

#if (defined(NVM_CANCELJOBS_CALLOUT))
/*======================NvM_Wrapper_CancelJobs==========================*/
FUNC(Std_ReturnType, NVM_CODE) NvM_Wrapper_CancelJobs
(
  NvM_BlockIdType BlockId
)
{
  /* !LINKSTO NvM.MultiCore.CancelJobsCallout_PreVerification,1 */
  uint16 BlockIndex = NvM_CheckCondCancelJobs(BlockId);
  Std_ReturnType ret = E_NOT_OK;

  if(BlockIndex != NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS)
  {
    if( NvM_AdminBlockTable[BlockIndex].NvMResult == NVM_REQ_PENDING )
    {
      ret = NVM_CANCELJOBS_CALLOUT(BlockIndex);
      if (ret == E_NOT_OK)
      {
        NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus &= (uint8)(~(NVM_ES_MULTICORE_LOCK_MASK));
      }
    }
    else
    {
      NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus &= (uint8)(~(NVM_ES_MULTICORE_LOCK_MASK));
    }
  }
  return ret;
}
#endif /* defined NVM_CANCELJOBS_CALLOUT */

#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1) */

#if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3)

#if (defined(NVM_ERASENVBLOCK_CALLOUT))
/*======================NvM_Wrapper_EraseNvBlock==========================*/
FUNC(Std_ReturnType, NVM_CODE) NvM_Wrapper_EraseNvBlock
(
  NvM_BlockIdType BlockId
)
{
  /* !LINKSTO NvM.MultiCore.EraseNvBlockCallout_PreVerification,1 */
  uint16 BlockIndex = NvM_CheckCondEraseNvBlock(BlockId);
  Std_ReturnType ret = E_NOT_OK;

  if(BlockIndex != NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS)
  {
    if( NvM_AdminBlockTable[BlockIndex].NvMResult != NVM_REQ_PENDING )
    {
      ret = NVM_ERASENVBLOCK_CALLOUT(BlockIndex);
      if (ret == E_NOT_OK)
      {
        NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus &= (uint8)(~(NVM_ES_MULTICORE_LOCK_MASK));
      }
    }
    else
    {
      NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus &= (uint8)(~(NVM_ES_MULTICORE_LOCK_MASK));
    }
  }

  return ret;
}
#endif /* defined NVM_ERASENVBLOCK_CALLOUT */

#if (defined(NVM_INVALIDATENVBLOCK_CALLOUT))
/*======================NvM_Wrapper_InvalidateNvBlock==========================*/
FUNC( Std_ReturnType,NVM_CODE ) NvM_Wrapper_InvalidateNvBlock
(
  NvM_BlockIdType BlockId
)
{
  /* !LINKSTO NvM.MultiCore.InvalidateNvBlockCallout_PreVerification,1 */
  uint16 BlockIndex = NvM_CheckCondInvalidateNvBlock(BlockId);
  Std_ReturnType ret = E_NOT_OK;

  if(BlockIndex != NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS)
  {
    if( NvM_AdminBlockTable[BlockIndex].NvMResult != NVM_REQ_PENDING )
    {
      ret = NVM_INVALIDATENVBLOCK_CALLOUT(BlockIndex);
      if (ret == E_NOT_OK)
      {
        NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus &= (uint8)(~(NVM_ES_MULTICORE_LOCK_MASK));
      }
    }
    else
    {
      NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus &= (uint8)(~(NVM_ES_MULTICORE_LOCK_MASK));
    }
  }

  return ret;
}
#endif /* defined NVM_INVALIDATENVBLOCK_CALLOUT */

#endif /* #if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3) */

#define NVM_STOP_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/
