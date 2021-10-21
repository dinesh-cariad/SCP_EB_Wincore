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


#if (NVM_ENABLE_ASR32_SERVICE_API == STD_ON)

#if (NVM_SERVICE_PORT_USED == STD_ON)
FUNC(void, NVM_CODE) NvM_ASR32_Rte_GetErrorStatus
(
    NvM_ASR32_BlockIdType BlockId,
    P2VAR( NvM_ASR32_RequestResultType, AUTOMATIC, NVM_APPL_DATA ) RequestResultPtr
)
{

  NvM_ASR32_GetErrorStatus( BlockId, RequestResultPtr );

}

#if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)
FUNC(void, NVM_CODE) NvM_ASR32_Rte_SetDataIndex
(
  NvM_ASR32_BlockIdType BlockId, uint8 DataIndex
)
{
  NvM_ASR32_SetDataIndex(BlockId, DataIndex);
}

FUNC(void, NVM_CODE) NvM_ASR32_Rte_GetDataIndex
(
  NvM_ASR32_BlockIdType                        BlockId,
  P2VAR( uint8, AUTOMATIC, NVM_APPL_DATA ) DataIndex
)
{
  NvM_ASR32_GetDataIndex(BlockId, DataIndex);
}
#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1) */

#if (NVM_SET_RAM_BLOCK_STATUS_API == STD_ON)
FUNC(void, NVM_CODE) NvM_ASR32_Rte_SetRamBlockStatus
(
  NvM_ASR32_BlockIdType BlockId,
  boolean         BlockChanged
)
{

  NvM_ASR32_SetRamBlockStatus(BlockId, BlockChanged);

}
#endif  /* (NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) */

#if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)
FUNC( Std_ReturnType,NVM_CODE ) NvM_ASR32_Rte_ReadBlock
(
  NvM_ASR32_BlockIdType                        BlockId,
  P2VAR(uint8, AUTOMATIC, NVM_APPL_DATA)  NvM_DstPtr
)
{
  Std_ReturnType retValue = E_NOT_OK;
  retValue = NvM_ASR32_ReadBlock(BlockId, NvM_DstPtr);
  return retValue;
}

FUNC( Std_ReturnType,NVM_CODE ) NvM_ASR32_Rte_WriteBlock
(
  NvM_ASR32_BlockIdType                         BlockId,
  P2CONST(uint8, AUTOMATIC, NVM_APPL_DATA) NvM_SrcPtr
)
{
  Std_ReturnType retValue = E_NOT_OK;
  retValue = NvM_ASR32_WriteBlock(BlockId, NvM_SrcPtr);
  return retValue;
}

FUNC( Std_ReturnType,NVM_CODE ) NvM_ASR32_Rte_RestoreBlockDefaults
(
  NvM_ASR32_BlockIdType                       BlockId,
  P2VAR(uint8, AUTOMATIC, NVM_APPL_DATA) NvM_DestPtr
)
{
  Std_ReturnType retValue = E_NOT_OK;
  retValue = NvM_ASR32_RestoreBlockDefaults(BlockId,  NvM_DestPtr);
  return retValue;
}

#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1) */
#endif /* #if (NVM_SERVICE_PORT_USED == STD_ON) */

#if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3)
#if (NVM_SERVICE_PORT_USED == STD_ON)

FUNC( Std_ReturnType,NVM_CODE ) NvM_ASR32_Rte_EraseNvBlock
(
  NvM_ASR32_BlockIdType BlockId
)
{
  Std_ReturnType retValue = E_NOT_OK;
  retValue = NvM_ASR32_EraseNvBlock(BlockId);
  return retValue;
}

FUNC( Std_ReturnType,NVM_CODE ) NvM_ASR32_Rte_InvalidateNvBlock
(
  NvM_ASR32_BlockIdType BlockId
)
{
  Std_ReturnType retValue = E_NOT_OK;
  retValue = NvM_ASR32_InvalidateNvBlock(BlockId);
  return retValue;
}

#endif /* #if (NVM_SERVICE_PORT_USED == STD_ON) */

#if (NVM_ADMIN_PORT_USED == STD_ON)
FUNC(void, NVM_CODE) NvM_ASR32_Rte_SetBlockProtection
(
  NvM_ASR32_BlockIdType BlockId,
  boolean         ProtectionEnabled
)
{

  NvM_ASR32_SetBlockProtection(BlockId, ProtectionEnabled);

}
#endif /*#if (NVM_ADMIN_PORT_USED == STD_ON)*/

#endif /* #if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3) */
#endif /* #if (NVM_ENABLE_ASR32_SERVICE_API == STD_ON) */


#define NvM_STOP_SEC_CODE
#include <NvM_MemMap.h>
#endif /* #if (NVM_INCLUDE_RTE == STD_ON) */

#if ((NVM_INCLUDE_RTE == STD_OFF) &&\
     (NVM_ENABLE_ASR32_SERVICE_API == STD_OFF))

#include <TSCompiler.h>           /* usage of macro in TSCompiler_Default.h */

/* Avoid empty translation unit according to ISO C90 */
TS_PREVENTEMPTYTRANSLATIONUNIT

#endif /* ((NVM_INCLUDE_RTE == STD_OFF) && (NVM_ENABLE_ASR32_SERVICE_API == STD_OFF)) */

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/
