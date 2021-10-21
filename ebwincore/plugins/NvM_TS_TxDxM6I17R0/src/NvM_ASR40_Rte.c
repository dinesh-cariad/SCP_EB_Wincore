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

#include <Std_Types.h>                                                 /* AUTOSAR standard types */
#include <TSAutosar.h>                                             /* EB specific standard types */

#if (defined NVM_INTERNAL_USE)
#error NVM_INTERNAL_USE is already defined
#endif
#define NVM_INTERNAL_USE
#include <NvM_BSW.h>                                              /* Declaration of Nvm BSW APIs */

#if (NVM_INCLUDE_RTE == STD_ON)
#include <Rte_NvM.h>         /* Declaration of Nvm APIs visible to RTE - consistency enforcement */
#endif
#include <NvM_Wrapper.h>
#include <NvM_Cfg.h>                                                /*configuration header*/

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/
#if (NVM_INCLUDE_RTE == STD_ON)

#define NvM_START_SEC_CODE
#include <NvM_MemMap.h>


#if (NVM_ENABLE_ASR40_SERVICE_API == STD_ON)

#if (NVM_SERVICE_PORT_USED == STD_ON)
FUNC(Std_ReturnType, NVM_CODE) NvM_ASR40_Rte_GetErrorStatus
(
  NvM_ASR40_BlockIdType BlockId,
  P2VAR( NvM_ASR40_RequestResultType,AUTOMATIC,NVM_APPL_DATA ) RequestResultPtr
)
{
  Std_ReturnType ret = E_NOT_OK;
  ret = NvM_ASR40_GetErrorStatus( BlockId, RequestResultPtr );
  return ret;
}

#if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)
FUNC(Std_ReturnType, NVM_CODE) NvM_ASR40_Rte_SetDataIndex
(
  NvM_ASR40_BlockIdType BlockId, uint8 DataIndex
)
{
  Std_ReturnType ret = E_NOT_OK;
  ret = NvM_ASR40_SetDataIndex(BlockId, DataIndex);
  return ret;
}

FUNC(Std_ReturnType, NVM_CODE) NvM_ASR40_Rte_GetDataIndex
(
  NvM_ASR40_BlockIdType                        BlockId,
  P2VAR( uint8,AUTOMATIC,NVM_APPL_DATA ) DataIndex
)
{
  Std_ReturnType ret = E_NOT_OK;
  ret = NvM_ASR40_GetDataIndex(BlockId, DataIndex);
  return ret;
}
#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1) */

#if (NVM_SET_RAM_BLOCK_STATUS_API == STD_ON)
FUNC(Std_ReturnType, NVM_CODE) NvM_ASR40_Rte_SetRamBlockStatus
(
  NvM_ASR40_BlockIdType BlockId,
  boolean         BlockChanged
)
{
  Std_ReturnType ret = E_NOT_OK;
  ret = NvM_ASR40_SetRamBlockStatus(BlockId, BlockChanged);
  return ret;
}
#endif  /* (NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) */

#if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)
FUNC( Std_ReturnType,NVM_CODE ) NvM_ASR40_Rte_ReadBlock
(
  NvM_ASR40_BlockIdType                        BlockId,
  P2VAR(void, AUTOMATIC, NVM_APPL_DATA)  NvM_DstPtr
)
{
  Std_ReturnType ret = E_NOT_OK;
#if (defined NVM_READBLOCK_CALLOUT)
  ret = NvM_Wrapper_ReadBlock(BlockId, NvM_DstPtr);
#else
  ret = NvM_ASR40_ReadBlock(BlockId, NvM_DstPtr);
#endif
  return ret;
}

FUNC( Std_ReturnType,NVM_CODE ) NvM_ASR40_Rte_WriteBlock
(
  NvM_ASR40_BlockIdType                         BlockId,
  P2CONST(void, AUTOMATIC, NVM_APPL_DATA) NvM_SrcPtr
)
{
  Std_ReturnType ret = E_NOT_OK;
#if (defined NVM_WRITEBLOCK_CALLOUT)
  ret = NvM_Wrapper_WriteBlock(BlockId, NvM_SrcPtr);
#else
  ret = NvM_ASR40_WriteBlock(BlockId, NvM_SrcPtr);
#endif
  return ret;
}

FUNC( Std_ReturnType,NVM_CODE ) NvM_ASR40_Rte_RestoreBlockDefaults
(
  NvM_ASR40_BlockIdType                       BlockId,
  P2VAR(void, AUTOMATIC, NVM_APPL_DATA) NvM_DestPtr
)
{
  Std_ReturnType ret = E_NOT_OK;
#if (defined NVM_RESTOREBLOCKDEFAULTS_CALLOUT)
  ret = NvM_Wrapper_RestoreBlockDefaults(BlockId, NvM_DestPtr);
#else
  ret = NvM_ASR40_RestoreBlockDefaults(BlockId,  NvM_DestPtr);
#endif

  return ret;
}
#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1) */
#endif /* #if (NVM_SERVICE_PORT_USED == STD_ON) */

#if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3)
#if (NVM_SERVICE_PORT_USED == STD_ON)

FUNC( Std_ReturnType,NVM_CODE ) NvM_ASR40_Rte_EraseNvBlock
(
  NvM_ASR40_BlockIdType BlockId
)
{
  Std_ReturnType ret = E_NOT_OK;
#if (defined NVM_ERASENVBLOCK_CALLOUT)
  ret = NvM_Wrapper_EraseNvBlock(BlockId);
#else
  ret = NvM_ASR40_EraseNvBlock(BlockId);
#endif
  return ret;
}

FUNC( Std_ReturnType,NVM_CODE ) NvM_ASR40_Rte_InvalidateNvBlock
(
  NvM_ASR40_BlockIdType BlockId
)
{
  Std_ReturnType ret = E_NOT_OK;
#if (defined NVM_INVALIDATENVBLOCK_CALLOUT)
  ret = NvM_Wrapper_InvalidateNvBlock(BlockId);
#else
  ret = NvM_ASR40_InvalidateNvBlock(BlockId);
#endif
  return ret;
}
#endif /* #if (NVM_SERVICE_PORT_USED == STD_ON) */

#if (NVM_ADMIN_PORT_USED == STD_ON)
FUNC(Std_ReturnType, NVM_CODE) NvM_ASR40_Rte_SetBlockProtection
(
  NvM_ASR40_BlockIdType BlockId,
  boolean         ProtectionEnabled
)
{
  Std_ReturnType ret = E_NOT_OK;
  ret = NvM_ASR40_SetBlockProtection(BlockId, ProtectionEnabled);
  return ret;
}
#endif /* #if (NVM_ADMIN_PORT_USED == STD_ON) */

#endif /* #if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3) */
#endif /* #if (NVM_ENABLE_ASR40_SERVICE_API == STD_ON) */


#define NvM_STOP_SEC_CODE
#include <NvM_MemMap.h>

#endif /* (NVM_INCLUDE_RTE == STD_ON) */

#if ((NVM_INCLUDE_RTE == STD_OFF) &&\
     (NVM_ENABLE_ASR40_SERVICE_API == STD_OFF))

#include <TSCompiler.h>           /* usage of macro in TSCompiler_Default.h */

/* Avoid empty translation unit according to ISO C90 */
TS_PREVENTEMPTYTRANSLATIONUNIT

#endif /* ((NVM_INCLUDE_RTE == STD_OFF) && (NVM_ENABLE_ASR40_SERVICE_API == STD_OFF)) */

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/
