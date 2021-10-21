/**
 * \file
 *
 * \brief AUTOSAR FrArTp
 *
 * This file contains the implementation of the AUTOSAR
 * module FrArTp.
 *
 * \version 1.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* ****************************************************************************
 **                      Include Section                                     **
 *****************************************************************************/

#include <FrArTp_CfgAccess_Int.h> /* get module internal generic interfaces */

#if (FRARTP_PBCFGM_SUPPORT_ENABLED == STD_ON)
#define PBCFGM_NO_PBCFG_REQUIRED
#include <PbcfgM.h>           /* Post build configuration manager API */
#endif /* FRARTP_PBCFGM_SUPPORT_ENABLED */


/* ****************************************************************************
 **                      Local Macros                                        **
 *****************************************************************************/

#ifdef TS_RELOCATABLE_CFG_ENABLE
#error TS_RELOCATABLE_CFG_ENABLE is already defined
#endif
#define TS_RELOCATABLE_CFG_ENABLE FRARTP_RELOCATABLE_CFG_ENABLE
#include <TSPBConfig.h>           /* post-build config access macros */

/* ****************************************************************************
**                      Variable Definitions                                 **
******************************************************************************/

#define FRARTP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <FrArTp_MemMap.h>

/** \brief Post-build configuration pointer */
STATIC P2CONST(FrArTp_ConfigType, FRARTP_VAR_CLEARED, FRARTP_APPL_CONST) FrArTp_PBcfgPtr;

#define FRARTP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <FrArTp_MemMap.h>


/*
 * Start code section declaration
 */
#define FRARTP_START_SEC_CODE
#include <FrArTp_MemMap.h>

/* ****************************************************************************
**                      Local Function Prototypes                            **
******************************************************************************/

/* ****************************************************************************
**                      Local Function Definitions                           **
******************************************************************************/

/* ****************************************************************************
**                      Function Definitions                                 **
******************************************************************************/

FUNC( Std_ReturnType, FRARTP_CODE) FrArTp_IsValidConfig
(
  P2CONST(void,AUTOMATIC,FRARTP_APPL_CONST) voidConfigPtr
)
{
  /* Assume an invalid configuration */
  Std_ReturnType RetVal = E_NOT_OK;
  P2CONST(FrArTp_ConfigType,AUTOMATIC,FRARTP_APPL_CONST) ConfigPtr = voidConfigPtr;

  DBG_FRARTP_ISVALIDCONFIG_ENTRY(voidConfigPtr);

  if(ConfigPtr != NULL_PTR)
  {
    /* Check if the configuration fits to the platform */
    if(TS_PlatformSigIsValid(ConfigPtr->PlatformSignature))
    {
      /* Validate the post build configuration against the compile time configuration */
      if(FRARTP_CFG_SIGNATURE == ConfigPtr->CfgSignature)
      {
        /* Validate the Published information in post build configuration against the
            Published information in compile time configuration*/
        if(FRARTP_PUBLIC_INFO_SIGNATURE == ConfigPtr->PublicInfoSignature)
        {
          /* Indicate that the configuration is valid */
          RetVal = E_OK;
        }
      }
    }
  }

  DBG_FRARTP_ISVALIDCONFIG_EXIT(RetVal,voidConfigPtr);

  return RetVal;
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, FRARTP_CODE) FrArTp_CfgAccess_init
(
    P2CONST(FrArTp_ConfigType, FRARTP_VAR_CLEARED, FRARTP_APPL_CONST) cfgPtr
)
{
  Std_ReturnType retCode = E_NOT_OK;

#if (FRARTP_PBCFGM_SUPPORT_ENABLED == STD_ON)

  /* If the initialization function is called with a null pointer get the configuration from the
   * post build configuration manager */
  if(cfgPtr == NULL_PTR)
  {
    PbcfgM_ModuleConfigPtrType ModuleConfig = NULL_PTR;
    if(E_OK == PbcfgM_GetConfig(FRARTP_MODULE_ID, FRARTP_INSTANCE_ID, &ModuleConfig))
    {
      cfgPtr = (P2CONST(FrArTp_ConfigType, AUTOMATIC, FRARTP_APPL_CONST)) ModuleConfig;
    }
  }
#endif /* FRARTP_PBCFGM_SUPPORT_ENABLED == STD_OFF */

/* check if development error detection is enabled */
#if (FRARTP_DEV_ERROR_DETECT == STD_ON)
  /* check whether parameter FrArTp_RootPtr is a NULL_PTR */
  /* !LINKSTO FrArTp.EB.ASR42.SWS_FrArTp_00147.FRARTP_E_PARAM_POINTER,1 */
  if(cfgPtr == NULL_PTR)
  {
    /* found a NULL_PTR, report to DET */
    FRARTP_REPORT_ERROR(FRARTP_INIT_SERVICE_ID, FRARTP_E_PARAM_POINTER);
  }
  /* check that configuration pointer is valid */
  /* !LINKSTO FrArTp.EB.ASR42.SWS_FrArTp_00147.FRARTP_E_INVALID_PARAMETER,1 */
  else if(E_OK != FrArTp_IsValidConfig(cfgPtr))
  {
    FRARTP_REPORT_ERROR(FRARTP_INIT_SERVICE_ID, FRARTP_E_INVALID_PARAMETER);
  }
  else
#endif /* FRARTP_DEV_ERROR_DETECT */
  {
    FrArTp_PBcfgPtr = cfgPtr;
    retCode = E_OK;
  }

  return retCode;
}

#if (FRARTP_DEV_ERROR_DETECT == STD_ON)
TS_MOD_PRIV_DEFN FUNC(boolean, FRARTP_CODE) FrArTp_CfgAccess_isValid_RxPduIdx
(
  PduIdType rxPduIdx
)
{
 return (rxPduIdx >= FrArTp_PBcfgPtr->numRxPdus) ? FALSE : TRUE;
}

TS_MOD_PRIV_DEFN FUNC(boolean, FRARTP_CODE) FrArTp_CfgAccess_isValid_TxPduIdx
(
    PduIdType txPduIdx
)
{
  return (txPduIdx >= FrArTp_PBcfgPtr->numTxPdus) ? FALSE : TRUE;
}

#endif /* (FRARTP_DEV_ERROR_DETECT == STD_ON) */

TS_MOD_PRIV_DEFN FUNC_P2CONST(FrArTp_ChannelCfgType, AUTOMATIC, FRARTP_CODE) FrArTp_CfgAccess_getChannelCfgPtr_byRxPduIdx
(
  FrArTp_RxPduIdxType rxPduIdx
)
{
  CONSTP2CONST(FrArTp_RxPduCfgType,AUTOMATIC,FRARTP_APPL_CONST) rxPduCfgPtr =
      &FRARTP_GET_CONFIG_ADDR(FrArTp_RxPduCfgType,RxPdus)[rxPduIdx];
  CONSTP2CONST(FrArTp_ChannelCfgType,AUTOMATIC,FRARTP_APPL_CONST) channelCfgPtr =
      &FRARTP_GET_CONFIG_ADDR(FrArTp_ChannelCfgType,Channels)[rxPduCfgPtr->channelIdx];
  return channelCfgPtr;
}


TS_MOD_PRIV_DEFN FUNC_P2CONST(FrArTp_ChannelCfgType, AUTOMATIC, FRARTP_CODE) FrArTp_CfgAccess_getChannelCfgPtr_byConnIdx
(
  FrArTp_ConnIdxType connIdx
)
{
  CONSTP2CONST(FrArTp_ConnectionCfgType,AUTOMATIC,FRARTP_APPL_CONST) connectionPtr =
      &FRARTP_GET_CONFIG_ADDR(FrArTp_ConnectionCfgType,Connections)[connIdx];
  CONSTP2CONST(FrArTp_ChannelCfgType,AUTOMATIC,FRARTP_APPL_CONST) channelCfgPtr =
      &FRARTP_GET_CONFIG_ADDR(FrArTp_ChannelCfgType,Channels)[connectionPtr->channelIdx];
  return channelCfgPtr;
}


TS_MOD_PRIV_DEFN FUNC(FrArTp_ConnIdxType, FRARTP_CODE) FrArTp_CfgAccess_getConnectionIdx_byAddress
(
  FrArTp_RxPduIdxType rxPduIdx,
  uint16 localAddress,
  uint16 remoteAddress
)
{
  CONSTP2CONST(FrArTp_RxPduCfgType,AUTOMATIC,FRARTP_APPL_CONST) rxPduCfgPtr =
      &FRARTP_GET_CONFIG_ADDR(FrArTp_RxPduCfgType,RxPdus)[rxPduIdx];
  FrArTp_ConnIdxType connIdx;
  const FrArTp_ConnIdxType endConnIdx = rxPduCfgPtr->startConnIdx + rxPduCfgPtr->numConn;

  for(connIdx = rxPduCfgPtr->startConnIdx; connIdx < endConnIdx; connIdx++)
  {
    CONSTP2CONST(FrArTp_ConnectionCfgType,AUTOMATIC,FRARTP_APPL_CONST) connectionPtr =
        &FRARTP_GET_CONFIG_ADDR(FrArTp_ConnectionCfgType,Connections)[connIdx];

    if( (localAddress == connectionPtr->localAddress) &&
        (remoteAddress == connectionPtr->remoteAddress) )
    {
      break;
    }
  }

  /* If no match has been found */
  if(connIdx == endConnIdx)
  {
    connIdx = FRARTP_CONN_INVALID;
  }

  return connIdx;
}

TS_MOD_PRIV_DEFN FUNC(FrArTp_ConnIdxType, FRARTP_CODE) FrArTp_CfgAccess_getConnectionIdx_byTxSduId
(
  PduIdType txSduId
)
{
  return FRARTP_GET_CONFIG_ADDR(FrArTp_ConnIdxType,TxSdus)[txSduId];
}

TS_MOD_PRIV_DEFN FUNC(FrArTp_ConnIdxType, FRARTP_CODE) FrArTp_CfgAccess_getConnectionIdx_byRxSduId
(
  PduIdType rxSduId
)
{
  return FRARTP_GET_CONFIG_ADDR(FrArTp_ConnIdxType,RxSdus)[rxSduId];
}

TS_MOD_PRIV_DEFN FUNC_P2CONST(FrArTp_TxPoolCfgType, AUTOMATIC, FRARTP_CODE) FrArTp_CfgAccess_getTxPoolCfgPtr
(
    FrArTp_TxPoolIdxType txPoolIdx
)
{
  return &FRARTP_GET_CONFIG_ADDR(FrArTp_TxPoolCfgType,TxPools)[txPoolIdx];
}

TS_MOD_PRIV_DEFN FUNC_P2CONST(FrArTp_TxPduCfgType, AUTOMATIC, FRARTP_CODE) FrArTp_CfgAccess_getTxPduCfgPtr
(
  FrArTp_TxPduIdxType txPduIdx
)
{
  return &FRARTP_GET_CONFIG_ADDR(FrArTp_TxPduCfgType,TxPdus)[txPduIdx];
}

TS_MOD_PRIV_DEFN FUNC_P2CONST(FrArTp_ConnectionCfgType, AUTOMATIC, FRARTP_CODE) FrArTp_CfgAccess_getConnectionCfgPtr
(
  FrArTp_ConnIdxType connIdx
)
{
  return &FRARTP_GET_CONFIG_ADDR(FrArTp_ConnectionCfgType,Connections)[connIdx];
}

TS_MOD_PRIV_DEFN FUNC(FrArTp_TxPoolIdxType, FRARTP_CODE) FrArTp_CfgAccess_getNumTxPools
(
  void
)
{
  return FrArTp_PBcfgPtr->numTxPools;
}



TS_MOD_PRIV_DEFN FUNC(FrArTp_ConnIdxType, FRARTP_CODE) FrArTp_CfgAccess_getNumConn
(
  void
)
{
  return FrArTp_PBcfgPtr->numConn;
}



TS_MOD_PRIV_DEFN FUNC(uint16, FRARTP_CODE) FrArTp_CfgAccess_getMainFunctionPeriodMS
(
  void
)
{
  return FrArTp_PBcfgPtr->mainFunctionPeriod;
}


#if (FRARTP_DEV_ERROR_DETECT == STD_ON)

TS_MOD_PRIV_DEFN FUNC(FrArTp_ConnIdxType, FRARTP_CODE) FrArTp_CfgAccess_getNumTxSdus
(
  void
)
{
  return FrArTp_PBcfgPtr->numTxSdus;
}


TS_MOD_PRIV_DEFN FUNC(FrArTp_ConnIdxType, FRARTP_CODE) FrArTp_CfgAccess_getNumRxSdus
(
  void
)
{
  return FrArTp_PBcfgPtr->numRxSdus;
}

#endif /* (FRARTP_DEV_ERROR_DETECT == STD_ON) */

/*
 * Stop code section declaration
 */
#define FRARTP_STOP_SEC_CODE
#include <FrArTp_MemMap.h>
