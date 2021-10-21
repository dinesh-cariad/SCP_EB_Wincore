/**
 * \file
 *
 * \brief AUTOSAR SoAd
 *
 * This file contains the implementation of the AUTOSAR
 * module SoAd.
 *
 * \version 1.8.16
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*
 * Misra-C:2012 Deviations:
 *
 * MISRAC2012-1) Deviated Rule: 8.13
 * A pointer should point to a const-qualified type whenever possible.
 *
 * Reason:
 * This violation usually occurs if the AUTOSAR API of a module has
 * a P2VAR as parameter and the data is read only.
 * This cannot be fixed if the API should be conform to AUTOSAR.
 *
 *
 * MISRAC2012-2) Deviated Rule: 12.1
 * The precedence of operators within expressions should be made explicit.
 *
 * Reason:
 * It may be required to use more complex expressions, if Det checks shall be executed at the
 * beginning of the function.
 *
 *
 * MISRAC2012-3) Deviated Rule: 14.2
 * A for loop shall be well-formed.
 *
 * Reason:
 * 1)The comparison is done with the incremented loop counter to protect the last for loop entry.
 * 2)Macro COMSTACK_BF_ITERATE is an optimized pattern to iterate over bit fields.
 *   It allows the compiler simplifying control code (fewer conditional jumps) and reducing the
 *   number of cache misses.
 *
 *
 * MISRAC2012-6) Deviated Rule: 11.5 (advisory)
 * A conversion should not be performed from pointer to void into pointer to
 * object.
 *
 * Reason:
 * The memory routines are optimized for dealing with aligned memory sections.
 *
 */

/*==================[inclusions]============================================*/
#include <TSAutosar.h> /* EB specific standard types */
/* !LINKSTO SoAd.ASR42.SWS_SoAd_00073, 1 */
#include <ComStack_Types.h>
/* !LINKSTO SoAd.ASR42.SWS_SoAd_00072, 1 */
#include <SoAd.h> /* Module public API */
#if(SOAD_META_DATA_HANDLING == STD_ON)
#include <EcuC.h> /* Module public API */
#endif
#include <SoAd_Int.h> /* Module internal header file */
/* !LINKSTO SoAd.ASR42.SWS_SoAd_00073, 1 */
#include <SoAd_Cbk.h> /* Module callbacks */
#include <SoAd_Version.h> /* Module Version Info */
#include <SchM_SoAd.h>
#if(SOAD_DEV_ERROR_DETECT == STD_ON)
#include <Det.h> /* Det API */
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */

#include <SoAd_01_UdpSM_Int.h> /* UDP state machine unit. */
#include <SoAd_04_TcpSM_Int.h> /* UDP state machine unit. */

#include <SoAd_02_SocketCon_Int.h> /* Common socket handling relevant APIs. */
#include <ComStack_Helpers.h>


#if (SOAD_PBCFGM_SUPPORT_ENABLED == STD_ON)
#define PBCFGM_NO_PBCFG_REQUIRED
#include <PbcfgM.h>           /* Post build configuration manager API */
#endif /* SOAD_PBCFGM_SUPPORT_ENABLED */

/*==================[macros]================================================*/


#if (defined SOAD_MAX_SM_REPETITION)
#error SOAD_MAX_SM_REPETITION is already defined
#endif
#define SOAD_MAX_SM_REPETITION 5U

/*==================[type definitions]======================================*/

typedef struct
{
  boolean Connecting;
  boolean Error;
  boolean UpClosing;
  boolean Closed;
  boolean Connected;
  boolean Release;
} SoAd_Int_08_UdpSMTriggerType;


/*==================[external data]=========================================*/
#define SOAD_START_SEC_VAR_INIT_8
#include <SoAd_MemMap.h>

VAR(uint8, SOAD_VAR) SoAd_Initialized = SOAD_UNINITED;

#define SOAD_STOP_SEC_VAR_INIT_8
#include <SoAd_MemMap.h>


#define SOAD_START_SEC_VAR_INIT_UNSPECIFIED
#include <SoAd_MemMap.h>

P2CONST(SoAd_ConfigType, SOAD_VAR, SOAD_APPL_CONST) SoAd_PBcfg = NULL_PTR;

#define SOAD_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <SoAd_MemMap.h>

#define SOAD_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <SoAd_MemMap.h>

VAR(SoAd_TimerType, SOAD_VAR) SoAd_CurrentTimerValue;
VAR(SoAd_TimerType, SOAD_VAR) SoAd_NextUdpAliveTimeout;

#define SOAD_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <SoAd_MemMap.h>

#define SOAD_START_SEC_VAR_CLEARED_8
#include <SoAd_MemMap.h>

VAR(boolean, SOAD_VAR) SoAd_RoutingGroupTriggered;

VAR(boolean, SOAD_VAR) SoAd_StateTransCondChange;

VAR(boolean, SOAD_VAR) SoAd_RxCondChange;

VAR(boolean, SOAD_VAR) SoAd_TxTpCondChange;

VAR(boolean, SOAD_VAR) SoAd_TxIfCondChange;

#define SOAD_STOP_SEC_VAR_CLEARED_8
#include <SoAd_MemMap.h>

#define SOAD_START_SEC_VAR_INIT_32
#include <SoAd_MemMap.h>

#if (SOAD_GETANDRESETMEASUREMENTDATA_API == STD_ON)
VAR(uint32, SOAD_VAR) SoAd_MeasurementDropData_Tcp = 0U;
VAR(uint32, SOAD_VAR) SoAd_MeasurementDropData_Udp = 0U;
#endif /* SOAD_GETANDRESETMEASUREMENTDATA_API == STD_ON */

#define SOAD_STOP_SEC_VAR_INIT_32
#include <SoAd_MemMap.h>

/*==================[internal data]=========================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/
#define SOAD_START_SEC_CODE
#include <SoAd_MemMap.h>


/** \brief Helper function initialize RAM data structures. */
STATIC FUNC(void, SOAD_CODE) SoAd_InitDataStructures
(
  void
);

/** \brief Helper function initialize SoCon specific RAM data structures. */
STATIC FUNC(void, SOAD_CODE) SoAd_InitSoConDataStructures
(
  void
);

STATIC FUNC(void, SOAD_CODE) SoAd_MainFunctionSoCon(void);

STATIC FUNC(void, SOAD_CODE) SoAd_VerifyRxTxInProgress(void);


/** \brief Sub function of SoAd_MainFunction() to handle UDP state changes.
 **
 ** \param[in] SoConPtr - Pointer to the configuration of the socket connection.
 ** \param[in] SoConStatePtr - Pointer to the state information of the socket connection
 **
 ** \returns Repetition of SM processing.
 */
STATIC FUNC(boolean, SOAD_CODE) SoAd_MainFunctionSoConUdp
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

/** \brief Helper function to evaluate UDP state machine triggers.
 **
 ** \param[in] SoConPtr - Pointer to the configuration of the socket connection.
 ** \param[in] SoConStatePtr - Pointer to the state information of the socket connection
 ** \param[out] UdpSMTriggerPtr - Pointer to return the Udp SM trigger results.
 */
STATIC FUNC(void, SOAD_CODE) SoAd_Int_08_UdpSMTriggerEval
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(SoAd_Int_08_UdpSMTriggerType, AUTOMATIC, SOAD_APPL_DATA) UdpSMTriggerPtr
);

STATIC FUNC(boolean, SOAD_CODE) SoAd_MainFunctionSoConTcp
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);



STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_SetRemoteAddress
(
  SoAd_SoConIdType SoConId,
  CONSTP2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) IpAddrPtr,
  CONSTP2VAR(boolean, AUTOMATIC, SOAD_APPL_DATA) IsDisconnect,
  CONSTP2VAR(boolean, AUTOMATIC, SOAD_APPL_DATA) IsConnected
);


STATIC FUNC(SoAd_SoConIdType, SOAD_CODE) SoAd_SocketToMapId
(
  TcpIp_SocketIdType SocketId
);

STATIC FUNC(SoAd_SoConIdType, SOAD_CODE) SoAd_ListenSocketToMapId
(
  TcpIp_SocketIdType ListenSocketId
);

STATIC FUNC(SoAd_RoutingGroupIdType, SOAD_CODE) SoAd_RoutingGroupExtToInt
(
  SoAd_RoutingGroupIdType Id,
  SoAd_SoConIdType SoConId
);

STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_RoutingGroupAction
(
  SoAd_RoutingGroupIdType ExternalRoutingGroupId,
  SoAd_SoConIdType SoConId,
  SoAd_RoutingGroupActionType Action,
  uint8 ServiceId
);

/** \brief Helper function to enable Tx Data Structures especially PduRouteVirtDestinations in RAM
 **
 ** \param[in] InternalRoutingGroupId - Id that shall be enabled
 */
STATIC FUNC(void, SOAD_CODE) SoAd_EnableRoutingGroup_TxDataStructures
(
  SoAd_RoutingGroupIdType InternalRoutingGroupId
);

/** \brief Helper function to enable Rx Data Structures especially SocketRouteDestinations in RAM
 **
 ** \param[in] SoConId - SoConId of specific RG to be enabled.
 ** \param[in] ExternalRoutingGroupId - Id of RG to be enabled.
 ** \param[in] ServiceId - Id of Calling Service. Used for DET.
 */
STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_EnableRoutingGroup_RxDataStructures
(
  SoAd_SoConIdType SoConId,
  SoAd_RoutingGroupIdType ExternalRoutingGroupId,
  uint8 ServiceId
);

/** \brief Helper function to disable Tx Data Structures especially PduRouteVirtDestinations in RAM
 **
 ** \param[in] InternalRoutingGroupId - Id that shall be disabled
 */
STATIC FUNC(void, SOAD_CODE) SoAd_DisableRoutingGroup_TxDataStructures
(
  SoAd_RoutingGroupIdType InternalRoutingGroupId
);

/** \brief Helper function to disable Rx Data Structures especially SocketRouteDestinations in RAM
 **
 ** \param[in] SoConId - SoConId of specific RG to be enabled.
 ** \param[in] ExternalRoutingGroupId - Id of RG to be enabled.
 */
STATIC FUNC(void, SOAD_CODE) SoAd_DisableRoutingGroup_RxDataStructures
(
  SoAd_SoConIdType SoConId,
  SoAd_RoutingGroupIdType ExternalRoutingGroupId
);

/** \brief Helper function to check if any other associated RG is still enabled.
 **
 ** \param[in] SocketRouteDestPtr - SocketRouteDestination that shall be checked.
 ** \param[in] ExternalRoutingGroupId - Id of RG to be checked.
 */
STATIC FUNC(boolean, SOAD_CODE) SoAd_OneOtherInternalRGActiveForSocketRouteDest
(
  P2CONST(SoAd_SocketRouteDestType, AUTOMATIC, SOAD_APPL_DATA) SocketRouteDestPtr,
  SoAd_RoutingGroupIdType ExternalRoutingGroupId
);

/** \brief Helper function to check if any other associated RG is still enabled.
 **
 ** \param[in] PduRouteDestVirtualPtr - PduRouteVirtDestination that shall be checked.
 ** \param[in] InternalRoutingGroupId - Id of RG to be checked.
 */
STATIC FUNC(boolean, SOAD_CODE) SoAd_OneOtherInternalRGActiveForPduRouteDest
(
  P2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr,
  SoAd_RoutingGroupIdType InternalRoutingGroupId
);

/** \brief Helper function to report local IP address assignment changes to the upper layer.
 **
 ** \param[in] SoConPtr - Pointer to socket connection configuration.
 ** \param[in] State - State of the local IP address.
 **/
STATIC FUNC(void, SOAD_CODE) SoAd_ULLocalIpAddrAssignmentChg
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  TcpIp_IpAddrStateType State
);



/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#if(SOAD_VERSION_INFO_API == STD_ON)
FUNC(void, SOAD_CODE) SoAd_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, SOAD_APPL_DATA) Versioninfo
)
{
  DBG_SOAD_GETVERSIONINFO_ENTRY(Versioninfo);

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  if(Versioninfo == NULL_PTR)
  {
    SOAD_DET_REPORT_ERROR(SOAD_GETVERSIONINFO_SVCID, SOAD_E_PARAM_POINTER);
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* assign logistic values to structure members */
    Versioninfo->vendorID = (uint16) SOAD_VENDOR_ID;
    Versioninfo->moduleID = (uint16) SOAD_MODULE_ID;
    Versioninfo->sw_major_version = (uint8) SOAD_SW_MAJOR_VERSION;
    Versioninfo->sw_minor_version = (uint8) SOAD_SW_MINOR_VERSION;
    Versioninfo->sw_patch_version = (uint8) SOAD_SW_PATCH_VERSION;
  }

  DBG_SOAD_GETVERSIONINFO_EXIT(Versioninfo);
}
#endif /* SOAD_VERSION_INFO_API == STD_ON */


/* !LINKSTO SoAd.ASR42.SWS_SoAd_00211, 1 */
/* !LINKSTO SoAd.ASR42.SWS_SoAd_00723, 1 */
FUNC(void, SOAD_CODE) SoAd_Init
(
  P2CONST(SoAd_ConfigType, AUTOMATIC, SOAD_APPL_CONST) SoAdConfigPtr
)
{
  P2CONST(SoAd_ConfigType,AUTOMATIC,SOAD_APPL_CONST) LocalConfigPtr = SoAdConfigPtr;
  DBG_SOAD_INIT_ENTRY(SoAdConfigPtr);

  /* Save the configuration pointer */
#if (SOAD_PBCFGM_SUPPORT_ENABLED == STD_ON)
  /* If the initialization function is called with a null pointer get the configuration from the
   * post build configuration manager */
  if(LocalConfigPtr == NULL_PTR)
  {
      PbcfgM_ModuleConfigPtrType ModuleConfig = NULL_PTR;
      if(E_OK == PbcfgM_GetConfig(
            SOAD_MODULE_ID,
            SOAD_INSTANCE_ID,
            &ModuleConfig))
      {
          LocalConfigPtr = (P2CONST(SoAd_ConfigType, AUTOMATIC, SOAD_APPL_CONST)) ModuleConfig;
      }
  }
#endif /* SOAD_PBCFGM_SUPPORT_ENABLED == STD_OFF */

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  /* SoAd shall check the validity of the PostBuild configuration and report SOAD_E_INV_ARG
   * if the check fails. */
  if(LocalConfigPtr == NULL_PTR)
  {
    SOAD_DET_REPORT_ERROR(SOAD_INIT_SVCID, SOAD_E_PARAM_POINTER);
  }
  /* check that configuration pointer is valid */
  else if( E_OK != SoAd_IsValidConfig(LocalConfigPtr))
  {
    SOAD_DET_REPORT_ERROR(SOAD_INIT_SVCID,SOAD_E_INV_ARG);
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* Required for protection in case of reinitialization. */
    TS_AtomicAssign8(SoAd_Initialized, SOAD_UNINITED);

    SoAd_PBcfg = LocalConfigPtr;

    /* Initialization shall not be protected with exclusive area since it takes very long and
       depending on project settings it could be too long which could trigger a watch dog timeout.
     */
    SoAd_InitDataStructures();

    /* Atomic assign shall protect against instruction reordering. */
    TS_AtomicAssign8(SoAd_Initialized, SOAD_INITED);

  }

  DBG_SOAD_INIT_EXIT(SoAdConfigPtr);
}


FUNC(Std_ReturnType, SOAD_CODE) SoAd_IsValidConfig
(
  P2CONST(void,AUTOMATIC,SOAD_APPL_CONST) SoAdConfigPtr
)
{
  /* Assume an invalid configuration */
  Std_ReturnType RetVal = E_NOT_OK;
  P2CONST(SoAd_ConfigType,AUTOMATIC,SOAD_APPL_CONST) ConfigPtr = SoAdConfigPtr;

  DBG_SOAD_ISVALIDCONFIG_ENTRY(SoAdConfigPtr);

  if(ConfigPtr != NULL_PTR)
  {
    /* Check if the configuration fits to the platform */
    if(TS_PlatformSigIsValid(ConfigPtr->PlatformSignature))
    {
      /* Validate the post build configuration against the compile time configuration */
      if(SOAD_CFG_SIGNATURE == ConfigPtr->CfgSignature)
      {
        /* Validate the post build configuration against the link time configuration */
        if(SoAd_LcfgSignature == ConfigPtr->LcfgSignature)
        {
          /* Validate the Published information in post build configuration against the
             Published information in compile time configuration*/
          if(SOAD_PUBLIC_INFO_SIGNATURE == ConfigPtr->PublicInfoSignature)
          {
              /* Indicate that the configuration is valid */
              RetVal = E_OK;
          }
        }
      }
    }
  }

  DBG_SOAD_ISVALIDCONFIG_EXIT(RetVal,SoAdConfigPtr);

  return RetVal;
}


FUNC(Std_ReturnType, SOAD_CODE) SoAd_IfRoutingGroupTransmit
(
  SoAd_RoutingGroupIdType Id
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_SOAD_IFROUTINGGROUPTRANSMIT_ENTRY(Id);

  /* development error checks are performed in SoAd_RoutingGroupAction() */

  RetVal = SoAd_RoutingGroupAction
  (
    Id,
    SOAD_INVALID_SOCON_ID,
    SOAD_ROUTINGGROUPACTION_TRIGGER,
    SOAD_IFROUTINGGROUPTRANSMIT_SVCID
  );

  DBG_SOAD_IFROUTINGGROUPTRANSMIT_EXIT(RetVal, Id);
  return RetVal;
}

FUNC(Std_ReturnType, SOAD_CODE) SoAd_IfSpecificRoutingGroupTransmit
(
  SoAd_RoutingGroupIdType Id,
  SoAd_SoConIdType SoConId
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_SOAD_IFROUTINGGROUPTRANSMIT_ENTRY(Id);

  /* development error checks are performed in SoAd_RoutingGroupAction() */

  RetVal = SoAd_RoutingGroupAction
  (
    Id,
    SoConId,
    SOAD_ROUTINGGROUPACTION_TRIGGER,
    SOAD_IFSPECIFICROUTINGGROUPTRANSMIT_SVCID
  );

  DBG_SOAD_IFSPECIFICROUTINGGROUPTRANSMIT_EXIT(RetVal, Id, SoConId);
  return RetVal;
}

FUNC(TcpIp_ReturnType, SOAD_CODE) SoAd_IsConnectionReady
(
    SoAd_SoConIdType SoConId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) RemoteAddrPtr
)
{
  TcpIp_ReturnType RetVal = TCPIP_E_NOT_OK;

  DBG_SOAD_ISCONNECTIONREADY_ENTRY(SoConId, RemoteAddrPtr);

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  if(SoAd_Initialized == SOAD_UNINITED)
  {
    SOAD_DET_REPORT_ERROR(SOAD_ISCONNECTIONREADY_SVCID, SOAD_E_NOTINIT);
  }
  else if(SoConId >= SoAd_PBcfg->SoConCnt)
  {
    SOAD_DET_REPORT_ERROR(SOAD_ISCONNECTIONREADY_SVCID, SOAD_E_INV_ARG);
  }
  /* NULL_PTR check of RemoteAddrPtr not necessary as TcpIp shall get a NULL_PTR
   * for Tcp connections as the remote address is already known */
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr =
      &SOAD_PBRAM(SoConState, SoConId);

    if(SoConStatePtr->SocketId != SOAD_SOCKETID_INVALID)
    {
      RetVal = TcpIp_IsConnectionReady(SoConStatePtr->SocketId, RemoteAddrPtr);
    }

  }
  DBG_SOAD_ISCONNECTIONREADY_EXIT(RetVal, SoConId, RemoteAddrPtr);
  return RetVal;
}

FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetSoConId
(
  PduIdType TxPduId,
  P2VAR(SoAd_SoConIdType, AUTOMATIC, SOAD_APPL_DATA) SoConIdPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_SOAD_GETSOCONID_ENTRY(TxPduId, SoConIdPtr);

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  if(SoAd_Initialized == SOAD_UNINITED)
  {
    SOAD_DET_REPORT_ERROR(SOAD_GETSOCONID_SVCID, SOAD_E_NOTINIT);
  }
  else if(SOAD_TX_PDUID_TO_PDUROUTE_ID(TxPduId) >= SoAd_PBcfg->PduRouteCnt)
  {
    SOAD_DET_REPORT_ERROR(SOAD_GETSOCONID_SVCID, SOAD_E_INV_PDUID);
  }
  else if(SoConIdPtr == NULL_PTR)
  {
    SOAD_DET_REPORT_ERROR(SOAD_GETSOCONID_SVCID, SOAD_E_PARAM_POINTER);
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    const SoAd_RouteIdType PduRouteId = SOAD_TX_PDUID_TO_PDUROUTE_ID(TxPduId);

    /* make sure this TxPduId is not associated with more than one socket connection (fan-out) */
    if(SOAD_CFG(PduRoute, PduRouteId).PduRouteDestVirtualCnt == 1U)
    {
      /* store the socket connection index */
      *SoConIdPtr =
          SOAD_CFG(PduRouteDestVirtual, SOAD_CFG(PduRoute, PduRouteId).PduRouteDestVirtualFirstId).SoConId;

      /* the request was successful */
      RetVal = E_OK;
    }
  }

  DBG_SOAD_GETSOCONID_EXIT(RetVal, TxPduId, SoConIdPtr);
  return RetVal;
}

FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetSoConMode
(
  SoAd_SoConIdType SoConId,
  P2VAR(SoAd_SoConModeType, AUTOMATIC, SOAD_APPL_DATA) ModePtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_SOAD_GETSOCONMODE_ENTRY(SoConId, ModePtr);

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  if(SoAd_Initialized == SOAD_UNINITED)
  {
    SOAD_DET_REPORT_ERROR(SOAD_GETSOCONMODE_SVCID, SOAD_E_NOTINIT);
  }
  else if(SoConId >= SoAd_PBcfg->SoConCnt)
  {
    SOAD_DET_REPORT_ERROR(SOAD_GETSOCONMODE_SVCID, SOAD_E_INV_ARG);
  }
  else if(ModePtr == NULL_PTR)
  {
    SOAD_DET_REPORT_ERROR(SOAD_GETSOCONMODE_SVCID, SOAD_E_PARAM_POINTER);
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr =
      &SOAD_PBRAM(SoConState, SoConId);

    RetVal = E_OK;

    SOAD_ENTER_CRITSEC();

    switch(SoConStatePtr->SocketState)
    {
      case SOAD_SOCKETSTATE_CLOSED: /* fall through */
      case SOAD_SOCKETSTATE_CLOSING:
      {
        *ModePtr = SOAD_SOCON_OFFLINE;
        break;
      }
      case SOAD_SOCKETSTATE_ONLINE:
      {
        *ModePtr = SOAD_SOCON_ONLINE;
        break;
      }
      case SOAD_SOCKETSTATE_CONNECTING: /* fall through */
      case SOAD_SOCKETSTATE_RESTART: /* fall through */
      case SOAD_SOCKETSTATE_SHUTDOWN:
      {
        *ModePtr = SOAD_SOCON_RECONNECT;
        break;
      }
      /* CHECK: NOPARSE */
      default:
      {
        SOAD_UNREACHABLE_CODE_ASSERT(SOAD_GETSOCONMODE_SVCID);
        RetVal = E_NOT_OK;
        break; /* Caution: this 'break' is required by MISRA-C:2012 */
      }
      /* CHECK: PARSE */
    }
    SOAD_EXIT_CRITSEC();
  }

  DBG_SOAD_GETSOCONMODE_EXIT(RetVal, SoConId, ModePtr);
  return RetVal;
}

FUNC(Std_ReturnType, SOAD_CODE) SoAd_OpenSoCon
(
  SoAd_SoConIdType SoConId
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_SOAD_OPENSOCON_ENTRY(SoConId);

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  if(SoAd_Initialized == SOAD_UNINITED)
  {
    SOAD_DET_REPORT_ERROR(SOAD_OPENSOCON_SVCID, SOAD_E_NOTINIT);
  }
  else if(SoConId >= SoAd_PBcfg->SoConCnt)
  {
    SOAD_DET_REPORT_ERROR(SOAD_OPENSOCON_SVCID, SOAD_E_INV_ARG);
  }
  else if(SOAD_FLAG(&SOAD_CFG(SoCon, SoConId), SOAD_SOCON_FLAG_AUTO_SETUP))
  {
    SOAD_DET_REPORT_ERROR(SOAD_OPENSOCON_SVCID, SOAD_E_INV_ARG);
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr =
      &SOAD_PBRAM(SoConState, SoConId);
    CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr =
          &SOAD_CFG(SoCon, SoConId);

    SOAD_ENTER_CRITSEC();
    SoConStatePtr->OpenRefCnt++;

    SOAD_EXIT_CRITSEC();

    /* Set global flag to know that for a certain SoCon a transition condition was met. */
    SoAd_SetBitInTransCondChange(SoConPtr);

    RetVal = E_OK;
  }

  DBG_SOAD_OPENSOCON_EXIT(RetVal, SoConId);
  return RetVal;
}

FUNC(Std_ReturnType, SOAD_CODE) SoAd_CloseSoCon
(
  SoAd_SoConIdType SoConId,
  boolean Abort
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_SOAD_CLOSESOCON_ENTRY(SoConId, Abort);

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  if(SoAd_Initialized == SOAD_UNINITED)
  {
    SOAD_DET_REPORT_ERROR(SOAD_CLOSESOCON_SVCID, SOAD_E_NOTINIT);
  }
  else if(SoConId >= SoAd_PBcfg->SoConCnt)
  {
    SOAD_DET_REPORT_ERROR(SOAD_CLOSESOCON_SVCID, SOAD_E_INV_ARG);
  }
  else if(SOAD_FLAG(&SOAD_CFG(SoCon, SoConId), SOAD_SOCON_FLAG_AUTO_SETUP))
  {
    SOAD_DET_REPORT_ERROR(SOAD_CLOSESOCON_SVCID, SOAD_E_INV_ARG);
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr =
      &SOAD_PBRAM(SoConState, SoConId);

    SOAD_ENTER_CRITSEC();
    /* prevent counter underflow if SoAd_CloseSon is called to often */
    if(SoConStatePtr->OpenRefCnt > 0U)
    {
      CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr =
        &SOAD_CFG(SoCon, SoConId);

      SoConStatePtr->OpenRefCnt--;

      if(Abort == TRUE)
      {
        /* the socket connection shall be closed immediately in the context of the next MainFunction
         * meaning that current transmissions and receptions will be cancelled */
        /* Set the flag to indicate that socket shall be closed */
        SOAD_FLAG_SET_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_CLOSE_SOCON);

        /* Set the flag to indicate that an abort occurred. */
        SOAD_FLAG_SET_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_CLOSE_SOCON_ABORT);

        /* Clear TXING_OK_State */
        SOAD_FLAG_CLR_UP(SoConStatePtr,SOAD_SOCONSTATE_TXING_OK);

        /* Set global flag to know that for a certain SoCon a transition condition was met. */
        SoAd_SetBitInTransCondChange(SoConPtr);
      }
      else
      {
        /* if no more upper layers reference this open socket connection, it shall be closed */
        if(SoConStatePtr->OpenRefCnt == 0U)
        {
          SOAD_FLAG_SET_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_CLOSE_SOCON);

          /* Clear TXING_OK_State */
          SOAD_FLAG_CLR_UP(SoConStatePtr,SOAD_SOCONSTATE_TXING_OK);

          /* Set global flag to know that for a certain SoCon a transition condition was met. */
          SoAd_SetBitInTransCondChange(SoConPtr);
        }
      }
    }

    SOAD_EXIT_CRITSEC();

    /* close request was successful */
    RetVal = E_OK;
  }

  DBG_SOAD_CLOSESOCON_EXIT(RetVal, SoConId, Abort);
  return RetVal;
}

FUNC(Std_ReturnType, SOAD_CODE) SoAd_RequestIpAddrAssignment
(
  SoAd_SoConIdType SoConId,
  TcpIp_IpAddrAssignmentType Type,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) LocalIpAddrPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_SOAD_REQUESTIPADDRASSIGNMENT_ENTRY(SoConId, Type, LocalIpAddrPtr);

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  if(SoAd_Initialized == SOAD_UNINITED)
  {
    SOAD_DET_REPORT_ERROR(SOAD_REQUESTIPADDRASSIGNMENT_SVCID, SOAD_E_NOTINIT);
  }
  else if(SoConId >= SoAd_PBcfg->SoConCnt)
  {
    SOAD_DET_REPORT_ERROR(SOAD_REQUESTIPADDRASSIGNMENT_SVCID, SOAD_E_INV_ARG);
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr =
          &SOAD_CFG(SoCon, SoConId);
    CONSTP2CONST(SoAd_SoConGroupType, AUTOMATIC, SOAD_APPL_DATA) SoConGroupPtr =
          &SOAD_CFG(SoConGroup, SoConPtr->SoConGroupId);

    RetVal = TcpIp_RequestIpAddrAssignment
              (
                SoConGroupPtr->LocalIpAddrId,
                Type,
                LocalIpAddrPtr
              );
  }

  DBG_SOAD_REQUESTIPADDRASSIGNMENT_EXIT(RetVal, SoConId, Type, LocalIpAddrPtr);
  return RetVal;
}

FUNC(Std_ReturnType, SOAD_CODE) SoAd_ReleaseIpAddrAssignment
(
  SoAd_SoConIdType SoConId
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_SOAD_RELEASEIPADDRASSIGNMENT_ENTRY(SoConId);

#if (SOAD_DEV_ERROR_DETECT == STD_ON)
  if(SoAd_Initialized == SOAD_UNINITED)
  {
    SOAD_DET_REPORT_ERROR(SOAD_RELEASEIPADDRASSIGNMENT_SVCID, SOAD_E_NOTINIT);
  }
  else if(SoConId >= SoAd_PBcfg->SoConCnt)
  {
    SOAD_DET_REPORT_ERROR(SOAD_RELEASEIPADDRASSIGNMENT_SVCID, SOAD_E_INV_ARG);
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr =
          &SOAD_CFG(SoCon, SoConId);
    CONSTP2CONST(SoAd_SoConGroupType, AUTOMATIC, SOAD_APPL_DATA) SoConGroupPtr =
          &SOAD_CFG(SoConGroup, SoConPtr->SoConGroupId);

    RetVal = TcpIp_ReleaseIpAddrAssignment(SoConGroupPtr->LocalIpAddrId);
  }

  DBG_SOAD_RELEASEIPADDRASSIGNMENT_EXIT(RetVal, SoConId);
  return RetVal;
}

FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetLocalAddr
(
  SoAd_SoConIdType SoConId,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) LocalAddrPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) NetmaskPtr,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) DefaultRouterPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_SOAD_GETLOCALADDR_ENTRY(SoConId, LocalAddrPtr, NetmaskPtr, DefaultRouterPtr);

#if (SOAD_DEV_ERROR_DETECT == STD_ON)
  if(SoAd_Initialized == SOAD_UNINITED)
  {
    SOAD_DET_REPORT_ERROR(SOAD_GETLOCALADDR_SVCID, SOAD_E_NOTINIT);
  }
  else if(SoConId >= SoAd_PBcfg->SoConCnt)
  {
    SOAD_DET_REPORT_ERROR(SOAD_GETLOCALADDR_SVCID, SOAD_E_INV_ARG);
  }
  else if(LocalAddrPtr == NULL_PTR)
  {
    SOAD_DET_REPORT_ERROR(SOAD_GETLOCALADDR_SVCID, SOAD_E_PARAM_POINTER);
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr =
          &SOAD_CFG(SoCon, SoConId);

    if(LocalAddrPtr->domain == TCPIP_AF_UNSPEC)
    {
      if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_IPV6))
      {
        LocalAddrPtr->domain = TCPIP_AF_INET6;
      }
      else
      {
        LocalAddrPtr->domain = TCPIP_AF_INET;
      }
      RetVal = E_OK;
    }
    else
    {
      CONSTP2CONST(SoAd_SoConGroupType, AUTOMATIC, SOAD_APPL_DATA) SoConGroupPtr =
            &SOAD_CFG(SoConGroup, SoConPtr->SoConGroupId);

      RetVal = TcpIp_GetIpAddr(
                                SoConGroupPtr->LocalIpAddrId,
                                LocalAddrPtr,
                                NetmaskPtr,
                                DefaultRouterPtr
                              );

      if(RetVal == E_OK)
      {
        CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr =
            &SOAD_PBRAM(SoConState, SoConId);

  #if(SOAD_IPV6_SUPPORT == STD_ON)
        /* check the domain type of the local address */
        if(LocalAddrPtr->domain == TCPIP_AF_INET6)
        {
          /* Deviation MISRAC2012-6  <+4> */
          P2VAR(TcpIp_SockAddrInet6Type, AUTOMATIC, SOAD_APPL_DATA) LocalInet6AddrPtr =
              (P2VAR(TcpIp_SockAddrInet6Type, TYPEDEF, SOAD_APPL_DATA))
              (P2VAR(void, TYPEDEF, SOAD_APPL_DATA))
              LocalAddrPtr;

          /* add local port information */
          TS_AtomicAssign16(LocalInet6AddrPtr->port, SoConStatePtr->AssignedLocalPort);
        }
        else
  #endif /* (SOAD_IPV6_SUPPORT == STD_ON) */
        {
          /* Deviation MISRAC2012-6  <+4> */
          P2VAR(TcpIp_SockAddrInetType, AUTOMATIC, SOAD_APPL_DATA) LocalInetAddrPtr =
              (P2VAR(TcpIp_SockAddrInetType, TYPEDEF, SOAD_APPL_DATA))
              (P2VAR(void, TYPEDEF, SOAD_APPL_DATA))
              LocalAddrPtr;

          /* add local port information */
          TS_AtomicAssign16(LocalInetAddrPtr->port, SoConStatePtr->AssignedLocalPort);
        }
      }
    }
  }

  DBG_SOAD_GETLOCALADDR_EXIT(RetVal, SoConId, LocalAddrPtr, NetmaskPtr, DefaultRouterPtr);
  return RetVal;
}

FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetPhysAddr
(
  SoAd_SoConIdType SoConId,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) PhysAddrPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_SOAD_GETPHYSADDR_ENTRY(SoConId, PhysAddrPtr);

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  if(SoAd_Initialized == SOAD_UNINITED)
  {
    SOAD_DET_REPORT_ERROR(SOAD_GETPHYSADDR_SVCID, SOAD_E_NOTINIT);
  }
  else if(SoConId >= SoAd_PBcfg->SoConCnt)
  {
    SOAD_DET_REPORT_ERROR(SOAD_GETPHYSADDR_SVCID, SOAD_E_INV_ARG);
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr =
          &SOAD_CFG(SoCon, SoConId);
    CONSTP2CONST(SoAd_SoConGroupType, AUTOMATIC, SOAD_APPL_DATA) SoConGroupPtr =
          &SOAD_CFG(SoConGroup, SoConPtr->SoConGroupId);

    RetVal = TcpIp_GetPhysAddr(
                                SoConGroupPtr->LocalIpAddrId,
                                PhysAddrPtr
                              );
  }

  DBG_SOAD_GETPHYSADDR_EXIT(RetVal, SoConId, PhysAddrPtr);
  return RetVal;
}

FUNC(Std_ReturnType, SOAD_CODE) SoAd_EnableRouting
(
  SoAd_RoutingGroupIdType Id
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_SOAD_ENABLEROUTING_ENTRY(Id);

  /* development error checks are performed in SoAd_RoutingGroupAction() */

  RetVal = SoAd_RoutingGroupAction(
                                    Id,
                                    SOAD_INVALID_SOCON_ID,
                                    SOAD_ROUTINGGROUPACTION_ENABLE,
                                    SOAD_ENABLEROUTING_SVCID
                                  );

  DBG_SOAD_ENABLEROUTING_EXIT(RetVal, Id);
  return RetVal;
}

FUNC(Std_ReturnType, SOAD_CODE) SoAd_EnableSpecificRouting
(
  SoAd_RoutingGroupIdType Id,
  SoAd_SoConIdType SoConId
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_SOAD_ENABLESPECIFICROUTING_ENTRY(Id, SoConId);

  /* development error checks are performed in SoAd_RoutingGroupAction() */

  RetVal = SoAd_RoutingGroupAction(
                                    Id,
                                    SoConId,
                                    SOAD_ROUTINGGROUPACTION_ENABLE,
                                    SOAD_ENABLESPECIFICROUTING_SVCID
                                  );

  DBG_SOAD_ENABLESPECIFICROUTING_EXIT(RetVal, Id, SoConId);
  return RetVal;
}

FUNC(Std_ReturnType, SOAD_CODE) SoAd_DisableRouting
(
  SoAd_RoutingGroupIdType Id
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_SOAD_DISABLEROUTING_ENTRY(Id);

  /* development error checks are performed in SoAd_RoutingGroupAction() */

  RetVal = SoAd_RoutingGroupAction(
                                    Id,
                                    SOAD_INVALID_SOCON_ID,
                                    SOAD_ROUTINGGROUPACTION_DISABLE,
                                    SOAD_DISABLEROUTING_SVCID
                                  );

  DBG_SOAD_DISABLEROUTING_EXIT(RetVal, Id);
  return RetVal;
}

FUNC(Std_ReturnType, SOAD_CODE) SoAd_DisableSpecificRouting
(
  SoAd_RoutingGroupIdType Id,
  SoAd_SoConIdType SoConId
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_SOAD_DISABLESPECIFICROUTING_ENTRY(Id, SoConId);

  /* development error checks are performed in SoAd_RoutingGroupAction() */

  RetVal = SoAd_RoutingGroupAction(
                                    Id,
                                    SoConId,
                                    SOAD_ROUTINGGROUPACTION_DISABLE,
                                    SOAD_DISABLESPECIFICROUTING_SVCID
                                  );

  DBG_SOAD_DISABLESPECIFICROUTING_EXIT(RetVal, Id, SoConId);
  return RetVal;
}

FUNC(Std_ReturnType, SOAD_CODE) SoAd_SetRemoteAddr
(
  SoAd_SoConIdType SoConId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) IpAddrPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  boolean SetRemoteAddrLocked = FALSE;

  DBG_SOAD_SETREMOTEADDR_ENTRY(SoConId, IpAddrPtr);

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  if(SoAd_Initialized == SOAD_UNINITED)
  {
    SOAD_DET_REPORT_ERROR(SOAD_SETREMOTEADDR_SVCID, SOAD_E_NOTINIT);
  }
  else if(SoConId >= SoAd_PBcfg->SoConCnt)
  {
    SOAD_DET_REPORT_ERROR(SOAD_SETREMOTEADDR_SVCID, SOAD_E_INV_ARG);
  }
  else if(SOAD_FLAG(&SOAD_CFG(SoCon, SoConId), SOAD_SOCON_FLAG_AUTO_SETUP))
  {
    SOAD_DET_REPORT_ERROR(SOAD_SETREMOTEADDR_SVCID, SOAD_E_INV_ARG);
  }
  else if(IpAddrPtr == NULL_PTR)
  {
    SOAD_DET_REPORT_ERROR(SOAD_SETREMOTEADDR_SVCID, SOAD_E_PARAM_POINTER);
  }
  else if(
           /* Deviation MISRAC2012-2 */
           (!SOAD_FLAG(&SOAD_CFG(SoCon, SoConId), SOAD_SOCON_FLAG_IS_IPV6))
           && (IpAddrPtr->domain != TCPIP_AF_INET)
         )
  {
    SOAD_DET_REPORT_ERROR(SOAD_SETREMOTEADDR_SVCID, SOAD_E_INV_ARG);
  }
  /* Check that the provided IpAddr struct is sufficiently large for the remote address */
  else if(
           SOAD_FLAG(&SOAD_CFG(SoCon, SoConId), SOAD_SOCON_FLAG_IS_IPV6)
           && (IpAddrPtr->domain != TCPIP_AF_INET6)
         )
  {
    SOAD_DET_REPORT_ERROR(SOAD_SETREMOTEADDR_SVCID, SOAD_E_INV_ARG);
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr =
      &SOAD_CFG(SoCon, SoConId);
    CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr =
      &SOAD_PBRAM(SoConState, SoConId);

    boolean Disconnect = FALSE;
    boolean Connected = FALSE;

    SOAD_ENTER_CRITSEC();
    /* Active TP session is checked in SoAd_SetRemoteAddress() */
    /* !LINKSTO SoAd.dsn.RemoteAddressHandling.SetRemoteAddr.Udp.LockState,1 */
    if((SoConStatePtr->SetUniqueRemoteAddrCnt != 0U) ||
       (SoConStatePtr->UdpAliveTimeout != 0U) ||
       (SoConStatePtr->BusyCnt != 0) ||
       SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_LOCK)
      )
    {
      SetRemoteAddrLocked = TRUE;
    }
    SOAD_EXIT_CRITSEC();

    if(SetRemoteAddrLocked == FALSE)
    {
      SOAD_ENTER_CRITSEC();

      RetVal = SoAd_SetRemoteAddress(SoConId, IpAddrPtr, &Disconnect, &Connected);

      SOAD_EXIT_CRITSEC();

      if(Disconnect)
      {
        /* Return value is ignored. If transition is not executed it is already in the state or it
         *  will be repeated in the next main function. */
        (void)SoAd_Int_01_UdpSM_Disconnected(SoConPtr, SoConStatePtr);

        /* SOAD_SOCONSTATE_FLAG_USED_SETREMOTEADDR must not be cleared here.
         * Otherwise it can happen that a SoCon remote address is released with
         * ReleaseRemoteAddr although it's configured remote address is disabled.
         * the flag will be reset when SoCon goes offline or is reset */
      }

      if(Connected)
      {
        /* Return value is ignored. If transition is not executed it is already in the state or it
         *  will be repeated in the next main function. */
        (void)SoAd_Int_01_UdpSM_Connected(SoConPtr, SoConStatePtr);
      }
    }
  }

  DBG_SOAD_SETREMOTEADDR_EXIT(RetVal, SoConId, IpAddrPtr);
  return RetVal;
}

FUNC(Std_ReturnType, SOAD_CODE) SoAd_SetUniqueRemoteAddr
(
  SoAd_SoConIdType SoConId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) RemoteAddrPtr,
  P2VAR(SoAd_SoConIdType, AUTOMATIC, SOAD_APPL_DATA) AssignedSoConIdPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_SOAD_SETUNIQUEREMOTEADDR_ENTRY(SoConId, RemoteAddrPtr, AssignedSoConIdPtr);
#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  if(SoAd_Initialized == SOAD_UNINITED)
  {
    SOAD_DET_REPORT_ERROR(SOAD_SETUNIQUEREMOTEADDR_SVCID, SOAD_E_NOTINIT);
  }
  else if(SoConId >= SoAd_PBcfg->SoConCnt)
  {
    SOAD_DET_REPORT_ERROR(SOAD_SETUNIQUEREMOTEADDR_SVCID, SOAD_E_INV_ARG);
  }
  else if(SOAD_FLAG(&SOAD_CFG(SoCon, SoConId), SOAD_SOCON_FLAG_AUTO_SETUP))
  {
    SOAD_DET_REPORT_ERROR(SOAD_SETUNIQUEREMOTEADDR_SVCID, SOAD_E_INV_ARG);
  }
  else if(RemoteAddrPtr == NULL_PTR)
  {
    SOAD_DET_REPORT_ERROR(SOAD_SETUNIQUEREMOTEADDR_SVCID, SOAD_E_PARAM_POINTER);
  }
  else if(AssignedSoConIdPtr == NULL_PTR)
  {
    SOAD_DET_REPORT_ERROR(SOAD_SETUNIQUEREMOTEADDR_SVCID, SOAD_E_PARAM_POINTER);
  }
  else if(
           /* Deviation MISRAC2012-2 */
           (!SOAD_FLAG(&SOAD_CFG(SoCon, SoConId), SOAD_SOCON_FLAG_IS_IPV6))
           && (RemoteAddrPtr->domain != TCPIP_AF_INET)
         )
  {
    SOAD_DET_REPORT_ERROR(SOAD_SETUNIQUEREMOTEADDR_SVCID, SOAD_E_INV_ARG);
  }
  /* Check that the provided IpAddr struct is sufficiently large for the remote address */
  else if(
           SOAD_FLAG(&SOAD_CFG(SoCon, SoConId), SOAD_SOCON_FLAG_IS_IPV6)
           && (RemoteAddrPtr->domain != TCPIP_AF_INET6)
         )
  {
    SOAD_DET_REPORT_ERROR(SOAD_SETUNIQUEREMOTEADDR_SVCID, SOAD_E_INV_ARG);
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    SoAd_SoConIdType MatchingSoConId;
    SoAd_AddressMatchType AddressMatch = SOAD_ADDRESSMATCH_NONE;
    P2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr =
    &SOAD_CFG(SoCon, SoConId);
    P2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr;
    boolean Connect = FALSE;
    const boolean IsTcp = SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_TCP);

    SOAD_ENTER_CRITSEC();
    SoAd_BestMatchAlgorithm(SoConPtr, RemoteAddrPtr, &MatchingSoConId, &AddressMatch, FALSE, TRUE);

    SoConStatePtr = &SOAD_PBRAM(SoConState, MatchingSoConId);

    switch(AddressMatch)
    {
      case SOAD_ADDRESSMATCH_WILDCARD: /* fall through */
      case SOAD_ADDRESSMATCH_PORT: /* fall through */
      case SOAD_ADDRESSMATCH_IPADDR:
      {
        *AssignedSoConIdPtr = MatchingSoConId;
        SoConStatePtr = &SOAD_PBRAM(SoConState, MatchingSoConId);
        SoConPtr = &SOAD_CFG(SoCon, MatchingSoConId);

        SoAd_AssignRemoteAddressSockAddrType2SoCon(SoConPtr, SoConStatePtr, RemoteAddrPtr);

        if(!IsTcp)
        {
          SoConStatePtr->SetUniqueRemoteAddrCnt++;

          /* In case that all preconditions are fulfilled, go to ONLINE */
          if(SoConStatePtr->SocketState == SOAD_SOCKETSTATE_CONNECTING)
          {
            Connect = TRUE;
          }
        }

        /* Set global flag to know that for a certain SoCon a transition condition was met. */
        SoAd_SetBitInTransCondChange(SoConPtr);

        RetVal = E_OK;

        break;
      }
      case SOAD_ADDRESSMATCH_EXACT:
      {
        if(!IsTcp)
        {
          SoConStatePtr = &SOAD_PBRAM(SoConState, MatchingSoConId);

          if(SoConStatePtr->SetUniqueRemoteAddrCnt < SOAD_SETUNIQUEREMOTEADDRCNT_MAX)
          {
            SoConStatePtr->SetUniqueRemoteAddrCnt++;
            *AssignedSoConIdPtr = MatchingSoConId;
            RetVal = E_OK;
          }
#if(SOAD_DEV_ERROR_DETECT == STD_ON)
          else
          {
            /* an overflow of SetUniqueRemoteAddrCnt occurred */
            SOAD_DET_REPORT_ERROR(SOAD_SETUNIQUEREMOTEADDR_SVCID, SOAD_E_INV_ARG);
          }
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
        }
        else
        {
          *AssignedSoConIdPtr = MatchingSoConId;
          RetVal = E_OK;
        }

        break;
      }
      case SOAD_ADDRESSMATCH_NONE:
      {
        break;
      }
      /* CHECK: NOPARSE */
      default:
        SOAD_UNREACHABLE_CODE_ASSERT(SOAD_MAINFUNCTION_SVCID);
        break; /* Caution: this 'break' is required by MISRA-C:2012 */
      /* CHECK: PARSE */
    }
    SOAD_EXIT_CRITSEC();

    if(Connect)
    {
      /* Return value is ignored. If transition is not executed it is already in the state or it
       *  will be repeated in the next main function. */
      (void)SoAd_Int_01_UdpSM_Connected(SoConPtr, SoConStatePtr);
    }
  }

  DBG_SOAD_SETUNIQUEREMOTEADDR_EXIT(RetVal, SoConId, RemoteAddrPtr, AssignedSoConIdPtr);
  return RetVal;
}

FUNC(Std_ReturnType, SOAD_CODE) SoAd_TpChangeParameter
(
  PduIdType Id,
  TPParameterType Parameter,
  uint16 Value
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_SOAD_TPCHANGEPARAMETER_ENTRY(Id, Parameter, Value);

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  if(SoAd_Initialized == SOAD_UNINITED)
  {
    SOAD_DET_REPORT_ERROR(SOAD_TPCHANGEPARAMETER_SVCID, SOAD_E_NOTINIT);
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    TS_PARAM_UNUSED(Id);
    TS_PARAM_UNUSED(Parameter);
    TS_PARAM_UNUSED(Value);
  }

  DBG_SOAD_TPCHANGEPARAMETER_EXIT(RetVal, Id, Parameter, Value);

  return RetVal;
}

FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetRemoteAddr
(
  SoAd_SoConIdType SoConId,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) IpAddrPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_SOAD_GETREMOTEADDR_ENTRY(SoConId, IpAddrPtr);

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  if(SoAd_Initialized == SOAD_UNINITED)
  {
    SOAD_DET_REPORT_ERROR(SOAD_GETREMOTEADDR_SVCID, SOAD_E_NOTINIT);
  }
  else if(SoConId >= SoAd_PBcfg->SoConCnt)
  {
    SOAD_DET_REPORT_ERROR(SOAD_GETREMOTEADDR_SVCID, SOAD_E_INV_ARG);
  }
  else if(IpAddrPtr == NULL_PTR)
  {
    SOAD_DET_REPORT_ERROR(SOAD_GETREMOTEADDR_SVCID, SOAD_E_PARAM_POINTER);
  }
  else if(
           /* Deviation MISRAC2012-2 */
           (!SOAD_FLAG(&SOAD_CFG(SoCon, SoConId), SOAD_SOCON_FLAG_IS_IPV6))
           && (IpAddrPtr->domain != TCPIP_AF_INET)
         )
  {
    SOAD_DET_REPORT_ERROR(SOAD_GETREMOTEADDR_SVCID, SOAD_E_INV_ARG);
  }
  /* Check that the provided IpAddr struct is sufficiently large for the remote address */
  else if(
           SOAD_FLAG(&SOAD_CFG(SoCon, SoConId), SOAD_SOCON_FLAG_IS_IPV6)
           && (IpAddrPtr->domain != TCPIP_AF_INET6)
         )
  {
    SOAD_DET_REPORT_ERROR(SOAD_GETREMOTEADDR_SVCID, SOAD_E_INV_ARG);
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr =
        &SOAD_CFG(SoCon, SoConId);
    CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr =
        &SOAD_PBRAM(SoConState, SoConId);

    SOAD_ENTER_CRITSEC();
    /* It is only possible to return a remote address if it is either configured or set by UL */
    if(SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_REMOTE_SET))
    {
      SoAd_AssignRemoteAddressSoCon2SockAddrType(SoConPtr, SoConStatePtr, IpAddrPtr);
      RetVal = E_OK;
    }
    SOAD_EXIT_CRITSEC();

  }

  DBG_SOAD_GETREMOTEADDR_EXIT(RetVal, SoConId, IpAddrPtr);
  return RetVal;
}

FUNC(Std_ReturnType, SOAD_CODE) SoAd_ChangeParameter
(
  SoAd_SoConIdType SoConId,
  uint8 ParameterId,
  /* Deviation MISRAC2012-1 */
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ParameterValuePtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_SOAD_CHANGEPARAMETER_ENTRY(SoConId, ParameterId, ParameterValuePtr);

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  if(SoAd_Initialized == SOAD_UNINITED)
  {
    SOAD_DET_REPORT_ERROR(SOAD_CHANGEPARAMETER_SVCID, SOAD_E_NOTINIT);
  }
  else if(SoConId >= SoAd_PBcfg->SoConCnt)
  {
    SOAD_DET_REPORT_ERROR(SOAD_CHANGEPARAMETER_SVCID, SOAD_E_INV_ARG);
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    P2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConState =
      &SOAD_PBRAM(SoConState, SoConId);

    if(SOAD_SOCKETID_INVALID != SoConState->SocketId)
    {
      RetVal = TcpIp_ChangeParameter(
                                      SoConState->SocketId,
                                      ParameterId,
                                      ParameterValuePtr
                                    );
    }
  }

  DBG_SOAD_CHANGEPARAMETER_EXIT(RetVal, SoConId, ParameterId, ParameterValuePtr);
  return RetVal;
}

FUNC(Std_ReturnType, SOAD_CODE) SoAd_ReadDhcpHostNameOption
(
  SoAd_SoConIdType SoConId,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) Length,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) Data
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_SOAD_READDHCPHOSTNAMEOPTION_ENTRY(SoConId, Length, Data);

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  if(SoAd_Initialized == SOAD_UNINITED)
  {
    SOAD_DET_REPORT_ERROR(SOAD_READDHCPHOSTNAMEOPTION_SVCID, SOAD_E_NOTINIT);
  }
  else if(SoConId >= SoAd_PBcfg->SoConCnt)
  {
    SOAD_DET_REPORT_ERROR(SOAD_READDHCPHOSTNAMEOPTION_SVCID, SOAD_E_INV_ARG);
  }
  else if(Length == NULL_PTR)
  {
    SOAD_DET_REPORT_ERROR(SOAD_READDHCPHOSTNAMEOPTION_SVCID, SOAD_E_PARAM_POINTER);
  }
  else if(Data == NULL_PTR)
  {
    SOAD_DET_REPORT_ERROR(SOAD_READDHCPHOSTNAMEOPTION_SVCID, SOAD_E_PARAM_POINTER);
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr =
          &SOAD_CFG(SoCon, SoConId);
    CONSTP2CONST(SoAd_SoConGroupType, AUTOMATIC, SOAD_APPL_DATA) SoConGroupPtr =
          &SOAD_CFG(SoConGroup, SoConPtr->SoConGroupId);

#if(SOAD_IPV6_SUPPORT == STD_ON)
    if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_IPV6))
    {
      uint16 Len16 = *Length;

      RetVal = TcpIp_DhcpV6ReadOption(
                                       SoConGroupPtr->LocalIpAddrId,
                                       39U,
                                       &Len16,
                                       Data
                                     );

      SOAD_POSTCONDITION_ASSERT(Len16 < 256U, SOAD_INTERNAL_SVCID);

      *Length = (uint8) Len16;
    }
    else
#endif /* (SOAD_IPV6_SUPPORT == STD_ON) */
    {
      /* Note: According to RFC4702 Chapter 2. Client FQDN option has Code 81 */
      RetVal = TcpIp_DhcpReadOption(
                                     SoConGroupPtr->LocalIpAddrId,
                                     81U,
                                     Length,
                                     Data
                                   );
    }


  }

  DBG_SOAD_READDHCPHOSTNAMEOPTION_EXIT(RetVal, SoConId, Length, Data);
  return RetVal;
}

FUNC(Std_ReturnType, SOAD_CODE) SoAd_WriteDhcpHostNameOption
(
  SoAd_SoConIdType SoConId,
  uint8 Length,
  P2CONST(uint8, AUTOMATIC, SOAD_APPL_CONST) Data
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_SOAD_WRITEDHCPHOSTNAMEOPTION_ENTRY(SoConId, Length, Data);

#if (SOAD_DEV_ERROR_DETECT == STD_ON)
  if(SoAd_Initialized == SOAD_UNINITED)
  {
    SOAD_DET_REPORT_ERROR(SOAD_WRITEDHCPHOSTNAMEOPTION_SVCID, SOAD_E_NOTINIT);
  }
  else if(SoConId >= SoAd_PBcfg->SoConCnt)
  {
    SOAD_DET_REPORT_ERROR(SOAD_WRITEDHCPHOSTNAMEOPTION_SVCID, SOAD_E_INV_ARG);
  }
  else if(Data == NULL_PTR)
  {
    SOAD_DET_REPORT_ERROR(SOAD_WRITEDHCPHOSTNAMEOPTION_SVCID, SOAD_E_PARAM_POINTER);
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr =
          &SOAD_CFG(SoCon, SoConId);
    CONSTP2CONST(SoAd_SoConGroupType, AUTOMATIC, SOAD_APPL_DATA) SoConGroupPtr =
          &SOAD_CFG(SoConGroup, SoConPtr->SoConGroupId);

#if(SOAD_IPV6_SUPPORT == STD_ON)
    if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_IPV6))
    {
      RetVal = TcpIp_DhcpV6WriteOption(
                                        SoConGroupPtr->LocalIpAddrId,
                                        39U,
                                        Length,
                                        Data
                                      );
    }
    else
#endif /* (SOAD_IPV6_SUPPORT == STD_ON) */
    {
      /* Note: According to RFC4702 Chapter 2. Client FQDN option has Code 81 */
      RetVal = TcpIp_DhcpWriteOption(
                                      SoConGroupPtr->LocalIpAddrId,
                                      81U,
                                      Length,
                                      Data
                                    );
    }
  }

  DBG_SOAD_WRITEDHCPHOSTNAMEOPTION_EXIT(RetVal, SoConId, Length, Data);
  return RetVal;
}


FUNC(void, SOAD_CODE) SoAd_TcpIpEvent
(
  TcpIp_SocketIdType SocketId,
  TcpIp_EventType Event
)
{
  DBG_SOAD_TCPIPEVENT_ENTRY(SocketId, Event);

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  if(SoAd_Initialized == SOAD_UNINITED)
  {
    SOAD_DET_REPORT_ERROR(SOAD_TCPIPEVENT_SVCID, SOAD_E_NOTINIT);
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    const SoAd_SoConIdType SoConMapId = SoAd_SocketToMapId(SocketId);
    const SoAd_SoConIdType ListenSocketMapId = SoAd_ListenSocketToMapId(SocketId);

    /* If it is a normal socket */
    if(SOAD_INVALID_SOCON_ID != SoConMapId)
    {
      SoAd_SoConIdType SoConId = SOAD_PBRAM(SocketSoConMap, SoConMapId).SoConId;
      P2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr =
        &SOAD_CFG(SoCon, SoConId);
      P2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr =
        &SOAD_PBRAM(SoConState, SoConId);
      switch(Event)
      {
        case TCPIP_TCP_FIN_RECEIVED:
        {
          /* Note: TCPIP_TCP_FIN_RECEIVED is reported when the remote node wants to close the
           * socket connection. The TCP socket remains valid until the disconnection procedure is
           * complete, but SoAd needs confirm that the connection can be closed, by closing the
           * related socket. This is done in the context of the next Mainfunction to avoid side
           * effects if an SoAd_IfTransmit was interrupted by SoAd_TcpIpEvent.
           */
          SOAD_FLAG_SET_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TCP_FIN_RECEIVED);
          SoAd_SetBitInTransCondChange(SoConPtr);
          break;
        }
        case TCPIP_TCP_RESET:
        {
          /* Note: TCPIP_TCP_RESET is only reported in case the remote node immediately closes the
           * TCP connection with a reset flag (RST). It can not be caused by any action of the
           * upper layer or the SoAd. Therefore the socket connection is usually active when a
           * TCPIP_TCP_RESET is reported thus only a flag is set to close the socket connection
           * in the context of the next MainFunction. */
          SOAD_FLAG_SET_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TCPIP_SOCKET_CLOSED);

          /* Set global flag to know that for a certain SoCon a transition condition was met. */
          SoAd_SetBitInTransCondChange(SoConPtr);

          /* the socket is no longer valid, remove the entry from the SocketSoConMap */
          SOAD_PBRAM(SocketSoConMap, SoConMapId).SoConId = SOAD_INVALID_SOCON_ID;
          break;
        }
        case TCPIP_TCP_CLOSED: /* fall through */
                               /* Caution: the previous comment is required by MISRA-C:2012 */
        case TCPIP_UDP_CLOSED:
        {
          SoAd_SoConIdType soConLastId = SoConId;

          /* Note: TCPIP_TCP_CLOSED/TCPIP_UDP_CLOSED can be reported because of several reasons:
           * - SoAd closed the socket intentionally as it is
           *   - no longer needed (upper layer closed the socket connection) or
           *   - must be reconnected (e.g. due to an error during communication)
           * - the remote node closed the TCP connection (FIN)
           * - TcpIp module is shutting down and closed the socket */

          /* In case of UDP this affects the whole socket connection group. */
          if(!SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_TCP))
          {
            CONSTP2CONST(SoAd_SoConGroupType, AUTOMATIC, SOAD_APPL_DATA) SoConGroupPtr =
                  &SOAD_CFG(SoConGroup, SoConPtr->SoConGroupId);
            SoConId = SoConGroupPtr->SoConFirstId;
            soConLastId = (SoConGroupPtr->SoConFirstId + SoConGroupPtr->SoConCnt) - (SoAd_SoConIdType)1U;
          }

          /* go through all affected socket connections */
          for(; SoConId <= soConLastId; SoConId++)
          {
            SoConStatePtr = &SOAD_PBRAM(SoConState, SoConId);
            SoConPtr = &SOAD_CFG(SoCon, SoConId);

            /* SocketId will be set to invalid in the context of the next mainfunction */
            /* Set flag to close in the context of the next MainFunction. */
            SOAD_FLAG_SET_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TCPIP_SOCKET_CLOSED);

            /* Update TXING_OK_State */
            SOAD_FLAG_CLR_UP(SoConStatePtr,SOAD_SOCONSTATE_TXING_OK);

            /* Set global flag to know that for a certain SoCon a transition condition was met. */
            SoAd_SetBitInTransCondChange(SoConPtr);
          }

          /* the socket is no longer valid, remove the entry from the SocketSoConMap */
          SOAD_PBRAM(SocketSoConMap, SoConMapId).SoConId = SOAD_INVALID_SOCON_ID;
          break;
        }
        default:
        {
#if(SOAD_DEV_ERROR_DETECT == STD_ON)
          SOAD_DET_REPORT_ERROR(SOAD_TCPIPEVENT_SVCID, SOAD_E_INV_ARG);
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
          break; /* Caution: this 'break' is required by MISRA-C:2012 */
        }
      }
    }
    /* If it is a listen socket. */
    else if(SOAD_INVALID_SOCON_ID != ListenSocketMapId)
    {
      if(TCPIP_TCP_CLOSED == Event)
      {
        SoAd_SoConIdType Idx;
        CONSTP2VAR(SoAd_ListenSocketSoConMapType, AUTOMATIC, SOAD_APPL_DATA) ListenMapEntryPtr =
          &SOAD_PBRAM(ListenSocketSoConMap, ListenSocketMapId);
        CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) FirstSoConPtr =
          &SOAD_CFG(SoCon, ListenMapEntryPtr->SoConListFirstId);
        CONSTP2CONST(SoAd_SoConGroupType, AUTOMATIC, SOAD_APPL_DATA) SoConGroupPtr =
          &SOAD_CFG(SoConGroup, FirstSoConPtr->SoConGroupId);

        const SoAd_SoConIdType SoConCnt = SoConGroupPtr->SoConCnt;

        /* NOTE: If acceptSockets are closed before the listenSocket their state is changed to
         *       SOAD_SOCKETSTATE_CONNECTING, therefore it's necessary to really close all these
         *       related sockets. */

        SOAD_ENTER_CRITSEC();
        for(Idx = 0U; Idx < SoConCnt; Idx++)
        {
          CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr =
            &SOAD_CFG(SoCon, SoConGroupPtr->SoConFirstId + Idx);
          CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr =
            &SOAD_PBRAM(SoConState, SoConGroupPtr->SoConFirstId + Idx);

          /* if the socket connection is not still connected */
          if(SOAD_SOCKETSTATE_ONLINE != SoConStatePtr->SocketState)
          {
            (void)SoAd_Int_04_TcpSM_Closed(SoConPtr, SoConStatePtr);
          }
        }

        /* remove the entry from the ListenSocketSoConMap, it is no longer available */
        ListenMapEntryPtr->ListenSocketId = SOAD_SOCKETID_INVALID;
        ListenMapEntryPtr->SoConListFirstId = SOAD_INVALID_SOCON_ID;
        SOAD_EXIT_CRITSEC();
      }
    }
    else
    {
#if(SOAD_DEV_ERROR_DETECT == STD_ON)
      /* It's not a normal socket and not a listenSocket -> SoAd doesn't know this socketId */
      SOAD_DET_REPORT_ERROR(SOAD_TCPIPEVENT_SVCID, SOAD_E_INV_SOCKETID);
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
    }
  }

  DBG_SOAD_TCPIPEVENT_EXIT(SocketId, Event);
}


STATIC FUNC(SoAd_SoConIdType, SOAD_CODE) SoAd_SocketToMapId
(
  TcpIp_SocketIdType SocketId
)
{
  SoAd_SoConIdType RetVal = SOAD_INVALID_SOCON_ID;
  SoAd_SoConIdType Idx;
  const SoAd_SoConIdType SoConCnt = SoAd_PBcfg->SoConCnt;
  P2VAR(SoAd_SocketSoConMapType, AUTOMATIC, SOAD_APPL_DATA) SocketSoConMapEntryPtr;

  for(Idx = 0U; Idx < SoConCnt; Idx++)
  {
    SocketSoConMapEntryPtr = &SOAD_PBRAM(SocketSoConMap, Idx);
    if(
        (SocketSoConMapEntryPtr->SoConId != SOAD_INVALID_SOCON_ID)
        && (SocketSoConMapEntryPtr->SocketId == SocketId)
      )
    {
      RetVal = Idx;
      break;
    }
  }

  /* Required for the dynamic Polyspace analysis
    Accessing the PBRAM returns full range values due to the type casts; the
    assertion assures that we continue with a valid range */
  SOAD_POSTCONDITION_ASSERT(   (RetVal < SoAd_PBcfg->SoConCnt)
                            || (RetVal == SOAD_INVALID_SOCON_ID),
                            SOAD_INTERNAL_SVCID
                           );

  return RetVal;
}


FUNC(void, SOAD_CODE) SoAd_LocalIpAddrAssignmentChg
(
  TcpIp_LocalAddrIdType IpAddrId,
  TcpIp_IpAddrStateType State
)
{
  DBG_SOAD_LOCALIPADDRASSIGNMENTCHG_ENTRY(IpAddrId, State);

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  if(SoAd_Initialized == SOAD_UNINITED)
  {
    SOAD_DET_REPORT_ERROR(SOAD_LOCALIPADDRASSIGNMENTCHG_SVCID, SOAD_E_NOTINIT);
  }
  else if(
           ((State != TCPIP_IPADDR_STATE_ASSIGNED) && (State != TCPIP_IPADDR_STATE_ONHOLD))
           && (State != TCPIP_IPADDR_STATE_UNASSIGNED)
         )
  {
    SOAD_DET_REPORT_ERROR(SOAD_LOCALIPADDRASSIGNMENTCHG_SVCID, SOAD_E_INV_ARG);
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    if(IpAddrId < SoAd_PBcfg->LocalIpAddrCnt)
    {
      SoAd_SoConIdType SoConGroupId;
      const SoAd_SoConIdType SoConGroupCnt = SoAd_PBcfg->SoConGroupCnt;

      for(SoConGroupId = 0U; SoConGroupId < SoConGroupCnt; SoConGroupId++)
      {
        P2CONST(SoAd_SoConGroupType, AUTOMATIC, SOAD_APPL_DATA) SoConGroupPtr
          = &SOAD_CFG(SoConGroup, SoConGroupId);

        if(SoConGroupPtr->LocalIpAddrId == IpAddrId)
        {
          SoAd_SoConIdType SoConId;
          const SoAd_SoConIdType SoConNextGroup
            = SoConGroupPtr->SoConFirstId + SoConGroupPtr->SoConCnt;

          for(SoConId = SoConGroupPtr->SoConFirstId; SoConId < SoConNextGroup; SoConId++)
          {
            P2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr
              = &SOAD_CFG(SoCon, SoConId);

            if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_NOTIFY_LOCALIPADDRCHG))
            {
              /* notify upper layer(s) about assignment */
              SoAd_ULLocalIpAddrAssignmentChg(SoConPtr, State);
            }

            SoAd_SetBitInTransCondChange(SoConPtr);
          }
        }
      }

      /* if the local IP address is ok */
      if((State == TCPIP_IPADDR_STATE_ASSIGNED) || (State == TCPIP_IPADDR_STATE_ONHOLD))
      {
        /* set bit in LocalIpAddrState table */
        /* Deviation MISRAC2012-6  <+4> */
        SOAD_SET_BIT_IN_BITARRAY
        (
          &SOAD_PBRAM_REF_ID(uint8, SoAd_PBcfg->LocalIpAddrStateBitMapBase, 0U), IpAddrId
        );
      }
      else
      {
        /* otherwise clear the bit in LocalIpAddrState table */
        /* Deviation MISRAC2012-6  <+4> */
        SOAD_CLEAR_BIT_IN_BITARRAY
        (
          &SOAD_PBRAM_REF_ID(uint8, SoAd_PBcfg->LocalIpAddrStateBitMapBase, 0U), IpAddrId
        );
      }
    }
  }

  DBG_SOAD_LOCALIPADDRASSIGNMENTCHG_EXIT(IpAddrId, State);
}

FUNC(void, SOAD_CODE) SoAd_ReleaseRemoteAddr
(
  SoAd_SoConIdType SoConId
)
{
  DBG_SOAD_RELEASEREMOTEADDR_ENTRY(SoConId);

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  if(SoAd_Initialized == SOAD_UNINITED)
  {
    SOAD_DET_REPORT_ERROR(SOAD_RELEASEREMOTEADDR_SVCID, SOAD_E_NOTINIT);
  }
  else if(SoConId >= SoAd_PBcfg->SoConCnt)
  {
    SOAD_DET_REPORT_ERROR(SOAD_RELEASEREMOTEADDR_SVCID, SOAD_E_INV_ARG);
  }
  else if(SOAD_FLAG(&SOAD_CFG(SoCon, SoConId), SOAD_SOCON_FLAG_AUTO_SETUP))
  {
    SOAD_DET_REPORT_ERROR(SOAD_RELEASEREMOTEADDR_SVCID, SOAD_E_INV_ARG);
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr =
      &SOAD_CFG(SoCon, SoConId);
    CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr =
      &SOAD_PBRAM(SoConState, SoConId);
    boolean Release = FALSE;

    if(!SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_TCP))
    {
      SOAD_ENTER_CRITSEC();
      if(SoConStatePtr->SetUniqueRemoteAddrCnt > 0U)
      {
        SoConStatePtr->SetUniqueRemoteAddrCnt--;
      }

      /* !LINKSTO SoAd.dsn.RemoteAddressHandling.ReleaseRemoteAddr.Udp.UsedSetRemoteAddr,1 */
      if((!SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_USED_SETREMOTEADDR)) &&
          (SoConStatePtr->SetUniqueRemoteAddrCnt == 0U) &&
          (SoConStatePtr->UdpAliveTimeout == 0U)
      )
      {
        if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_REMOTE_SET))
        {
          /* Set global flag to know that for a certain SoCon a transition condition was met. */
          SoAd_SetBitInTransCondChange(SoConPtr);

          if(
              SOAD_FLAG_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RX_DATA_BUFFERED) ||
              SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_TP_PENDING) ||
              SoConStatePtr->BusyCnt != 0
            )
          {
            SOAD_FLAG_SET_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_LOCK);
          }
          else
          {
            Release = TRUE;
          }
        }
#if(SOAD_DEV_ERROR_DETECT == STD_ON)
        else
        {
          /* SoAd_ReleaseRemoteAddr cannot be used to reset a SoConRemoteAddr to unset */
          SOAD_DET_REPORT_ERROR(SOAD_RELEASEREMOTEADDR_SVCID, SOAD_E_INV_ARG);
        }
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
      }
      SOAD_EXIT_CRITSEC();

      if(Release == TRUE)
      {
        /* reset the remote address to the configured one. Configured remote
         * address cannot be unset because SetRemoteAddr was not used before. */
        if(SoAd_ResetRemoteAddress(SoConPtr, SoConStatePtr) == TRUE)
        {
          /* Remote Address contains Wildcard --> change socket state to RECONNECT */
          /* !LINKSTO SoAd.dsn.RemoteAddressHandling.ReleaseRemoteAddr.Udp.RemoteAddrAndPort_WildCards,1 */
          (void)SoAd_Int_01_UdpSM_Disconnected(SoConPtr, SoConStatePtr);
        }
      }
    }
    else
    {
      SOAD_ENTER_CRITSEC();
      /* !LINKSTO SoAd.dsn.RemoteAddressHandling.ReleaseRemoteAddr.Tcp.UsedSetRemoteAddr,1 */
      if((!SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_USED_SETREMOTEADDR)) &&
         (SoConStatePtr->SocketState != SOAD_SOCKETSTATE_ONLINE)
      )
      {
        /* Set global flag to know that for a certain SoCon a transition condition was met. */
        SoAd_SetBitInTransCondChange(SoConPtr);

        Release = TRUE;
      }
      SOAD_EXIT_CRITSEC();

      if(Release == TRUE)
      {
        /* reset the remote address to the configured one. Configured remote
         * address cannot be unset because SetRemoteAddr was not used before. */
        (void)SoAd_ResetRemoteAddress(SoConPtr, SoConStatePtr);
      }
    }
  }
  DBG_SOAD_RELEASEREMOTEADDR_EXIT(SoConId);
}

#if (SOAD_GETANDRESETMEASUREMENTDATA_API == STD_ON)
FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetAndResetMeasurementData
(
  SoAd_MeasurementIdxType MeasurementIdx,
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) MeasurementDataPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_SOAD_GETANDRESETMEASUREMENTDATA_ENTRY(
                                              MeasurementIdx,
                                              MeasurementResetNeeded,
                                              MeasurementDataPtr
                                            );

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  if(SoAd_Initialized != SOAD_INITED)
  {
    SOAD_DET_REPORT_ERROR(SOAD_GETANDRESETMEASUREMENTDATA_SVCID, SOAD_E_NOTINIT);
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT */
  {
    SchM_Enter_SoAd_SCHM_SOAD_EXCLUSIVE_AREA_0();

    switch(MeasurementIdx)
    {
      case SOAD_MEAS_DROP_TCP:
      {
        if(MeasurementDataPtr != NULL_PTR)
        {
          TS_AtomicAssign32(*MeasurementDataPtr, SoAd_MeasurementDropData_Tcp);
        }
        if(MeasurementResetNeeded)
        {
          TS_AtomicAssign32(SoAd_MeasurementDropData_Tcp, 0U);
        }
        RetVal = E_OK;
        break;
      }
      case SOAD_MEAS_DROP_UDP:
      {
        if(MeasurementDataPtr != NULL_PTR)
        {
          TS_AtomicAssign32(*MeasurementDataPtr, SoAd_MeasurementDropData_Udp);
        }
        if(MeasurementResetNeeded)
        {
          TS_AtomicAssign32(SoAd_MeasurementDropData_Udp, 0U);
        }
        RetVal = E_OK;
        break;
      }
      case SOAD_MEAS_ALL:
      {
        if(MeasurementResetNeeded)
        {
          TS_AtomicAssign32(SoAd_MeasurementDropData_Tcp, 0U);
          TS_AtomicAssign32(SoAd_MeasurementDropData_Udp, 0U);
        }
        RetVal = E_OK;
        break;
      }
      default:
      {
        break;
      }
    }
    SchM_Exit_SoAd_SCHM_SOAD_EXCLUSIVE_AREA_0();
  }

  DBG_SOAD_GETANDRESETMEASUREMENTDATA_EXIT(
                                             RetVal,
                                             MeasurementIdx,
                                             MeasurementResetNeeded,
                                             MeasurementDataPtr
                                           );

  return RetVal;
}
#endif /* #if (SOAD_GETANDRESETMEASUREMENTDATA_API == STD_ON) */



FUNC(void, SOAD_CODE) SoAd_MainFunction
(
  void
)
{
  DBG_SOAD_MAINFUNCTION_ENTRY();

  if(SoAd_Initialized == SOAD_INITED)
  {
    boolean GlobalCondChange;

    SoAd_VerifyRxTxInProgress();

    TS_AtomicAssign8(GlobalCondChange, SoAd_StateTransCondChange);
    if(TRUE == GlobalCondChange)
    {
      TS_AtomicAssign8(SoAd_StateTransCondChange, FALSE);
      SoAd_MainFunctionSoCon();
    }

    TS_AtomicAssign8(GlobalCondChange, SoAd_RxCondChange);
    if(TRUE == GlobalCondChange)
    {
      TS_AtomicAssign8(SoAd_RxCondChange, FALSE);
      SoAd_MainFunctionRx();
    }

    SoAd_MainFunctionTx();

    TS_AtomicAssign8(GlobalCondChange, SoAd_TxIfCondChange);
    if(TRUE == GlobalCondChange)
    {
      TS_AtomicAssign8(SoAd_TxIfCondChange, FALSE);
      SoAd_MainFunctionTxIf();
    }

    /* if triggerable routing groups exist */
    TS_AtomicAssign8(GlobalCondChange, SoAd_RoutingGroupTriggered);
    if(TRUE == GlobalCondChange)
    {
      TS_AtomicAssign8(SoAd_RoutingGroupTriggered, FALSE);
      /* check if routing groups were triggered and handle the related transmission(s) */
      SoAd_HandleTriggeredRoutingGroups();
    }

    SoAd_CurrentTimerValue++;

    if(SoAd_CurrentTimerValue == 0U)
    {
      SoAd_CurrentTimerValue = 1U; /* Timer is not set to 0 because 0 is used when UdpAlive is disabled */
    }

    if(SoAd_CurrentTimerValue == SoAd_NextUdpAliveTimeout)
    {
      SoAd_HandleUdpAliveTimeout();
    }
  }

  DBG_SOAD_MAINFUNCTION_EXIT();
}


/*==================[internal function definitions]=========================*/
STATIC FUNC(void, SOAD_CODE) SoAd_InitDataStructures
(
  void
)
{


  DBG_SOAD_INITDATASTRUCTURES_ENTRY();
  /* Check if routing groups are initially triggered by configuration after initialization. */
  SoAd_RoutingGroupTriggered = TRUE;

  /* Set SoAd_CurrentTimerValue to 1 */
  SoAd_CurrentTimerValue = 1U;

  SoAd_NextUdpAliveTimeout = 0U;

  SoAd_InitSoConDataStructures();

  /* set the whole LocalIpAddrState uint8 array to zero (i.e. TCPIP_IPADDR_STATE_UNASSIGNED)
   * LocalIpAddrCnt must be divided by 8, each LocalIpAddrState is represented by only one bit */
  /* Deviation MISRAC2012-6  <+4> */
  TS_MemBZero(
               &SOAD_PBRAM_REF_ID(uint8, SoAd_PBcfg->LocalIpAddrStateBitMapBase, 0U),
               (uint32)(((uint32)SoAd_PBcfg->LocalIpAddrCnt / 8U) + 1U)
             );
  TS_MemCpy(
              &SOAD_PBRAM_REF_ID(uint32, SoAd_PBcfg->PduRouteDestVirtualTxEnabledBitMapBase, 0U),
              &SOAD_CFG_TYPE(uint32, PduRouteDestVirtualTxEnableBitMapInit, 0U),
              (uint32)(4*(((uint32)SoAd_PBcfg->PduRouteDestVirtualCnt / 32U) + 1U))
            );
  TS_MemCpy(
              &SOAD_PBRAM_REF_ID(uint32, SoAd_PBcfg->SocketRouteDestEnabledBitMapBase, 0U),
              &SOAD_CFG_TYPE(uint32, SocketRouteDestRxEnableBitMapInit, 0U),
              (uint32)(4*(((uint32)SoAd_PBcfg->SocketRouteDestCnt / 32U) + 1U))
            );

  /* if routing groups exist */
  if(SoAd_PBcfg->RoutingGroupCnt > 0U)
  {
    /* copy the configured values into the RoutingGroupState variable. */
    /* Deviation MISRAC2012-6  <+4> */
    TS_MemCpy(
               &SOAD_PBRAM_REF_ID(uint8, SoAd_PBcfg->RoutingGroupStateBitMapBase, 0U),
               &SOAD_CFG_TYPE(uint8, RoutingGroupStateBitMapInit, 0U),
               (uint32)(((uint32)SoAd_PBcfg->RoutingGroupCnt / 8U) + 1U)
             );

  }

  /* if triggerable routing groups exist */
  if(SoAd_PBcfg->RoutingGroupTriggerableCnt > 0U)
  {
    /* set the whole RoutingGroupTriggered uint8 array to zero (i.e. not triggered)
     * RoutingGroupTriggerableCnt must be divided by 8,
     * each RoutingGroupTriggerable is represented by only one bit. This is represented by
     * internal routing group count */
    /* Deviation MISRAC2012-6  <+4> */
    TS_MemBZero(
                 &SOAD_PBRAM_REF_ID(uint32, SoAd_PBcfg->RoutingGroupTriggeredBitMapBase, 0U),
                 (uint32)(((uint32)SoAd_PBcfg->RoutingGroupTriggerableCnt / 8U) + 1U)
               );
  }

  DBG_SOAD_INITDATASTRUCTURES_EXIT();
}

STATIC FUNC(void, SOAD_CODE) SoAd_InitSoConDataStructures
(
  void
)
{
  SoAd_SoConIdType SoConId;
  const SoAd_SoConIdType SoConCnt = SoAd_PBcfg->SoConCnt;
  const SoAd_SoConIdType ListenSocketCapacity = SoAd_PBcfg->ListenSocketCapacity;
  CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) TransCondChangeBitMapPtr =
      &SOAD_PBRAM_REF_ID(uint32, SoAd_PBcfg->SoConTransCondChangeBitMapBase, 0U);
  CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) DestVirtualNotIdleBitMapPtr =
    &SOAD_PBRAM_REF_ID(uint32, SoAd_PBcfg->DestVirtualNotIdleBitMapBase, 0U);
  CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) PduTxDataNotIdleBitMapPtr =
    &SOAD_PBRAM_REF_ID(uint32, SoAd_PBcfg->PduTxDataNotIdleBitMapBase, 0U);
  const SoAd_PduRouteDestVirtualIdType DestVirtualCnt = SoAd_PBcfg->PduRouteDestVirtualCnt;
  CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) SoConTxIFNotIdleBitMapPtr =
   &SOAD_PBRAM_REF_ID(uint32, SoAd_PBcfg->SoConTxIFNotIdleBitMapBase, 0U);
  CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) SoConRxNotIdleBitMapBitMapPtr =
    &SOAD_PBRAM_REF_ID(uint32, SoAd_PBcfg->SoConRxNotIdleBitMapBase, 0U);
  CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) SoConTxTPNotIdleBitMapPtr =
    &SOAD_PBRAM_REF_ID(uint32, SoAd_PBcfg->SoConTxTPNotIdleBitMapBase, 0U);

  /* Initialize bit fields and corresponding global variable */
  SoAd_StateTransCondChange = FALSE;
  SOAD_COMSTACK_BF_INIT(TransCondChangeBitMapPtr, SoConCnt);

  /* These bit fields do not have a global flag. They are used to find all PDUs which
     are not idle if transmission is done. */
  SOAD_COMSTACK_BF_INIT(DestVirtualNotIdleBitMapPtr, DestVirtualCnt);
  SOAD_COMSTACK_BF_INIT(PduTxDataNotIdleBitMapPtr, DestVirtualCnt);

  SoAd_TxIfCondChange = FALSE;
  SOAD_COMSTACK_BF_INIT(SoConTxIFNotIdleBitMapPtr, SoConCnt);

  SoAd_RxCondChange = FALSE;
  SOAD_COMSTACK_BF_INIT(SoConRxNotIdleBitMapBitMapPtr, SoConCnt);

  SoAd_TxTpCondChange = FALSE;
  SOAD_COMSTACK_BF_INIT(SoConTxTPNotIdleBitMapPtr, SoConCnt);

  /* initialize all socket connections */
  for(SoConId = 0U; SoConId < SoConCnt; SoConId++)
  {
    CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr =
      &SOAD_CFG(SoCon, SoConId);
    CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr =
      &SOAD_PBRAM(SoConState, SoConId);

    SoConStatePtr->SocketState = SOAD_SOCKETSTATE_CLOSED;
    SoConStatePtr->SocketId = SOAD_SOCKETID_INVALID;
    SoConStatePtr->BusyCnt = 0U;
    SoConStatePtr->UpperFlags = 0U;
    SoConStatePtr->LowerFlags = 0U;
    SoConStatePtr->UdpAliveTimeout = 0U;
    SoConStatePtr->SetUniqueRemoteAddrCnt = 0U;

    /* Reset all Rx/Tx data structures. */
    SoAd_InitSoConTxDataStructures(SoConPtr);
    SoAd_InitSoConRxDataStructures(SoConPtr);

    /* is socket connection configured for automatic setup (SoAdSocketAutomaticSoConSetup = TRUE) */
    if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_AUTO_SETUP))
    {
      /* set the open reference counter to 1
       * SoAd_OpenSoCon and SoAd_CloseSoCon wont change that value
       * MainFunction will keep this connection alive
       */
      SoConStatePtr->OpenRefCnt = 1U;
    }
    else
    {
      SoConStatePtr->OpenRefCnt = 0U;
    }

    /* Set IP addresses and ports to configured values. */
    SoConStatePtr->AssignedLocalPort = SOAD_CFG(SoConGroup, SoConPtr->SoConGroupId).LocalPort;

    /* use the configured remote address for the socket connection */
    if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_REMOTE_SET))
    {
      SOAD_FLAG_SET_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_REMOTE_SET);
      /* Return value can be ignored */
      (void)SoAd_ResetRemoteAddress(SoConPtr, SoConStatePtr);
    }

    /* Initialize ListenSocketSoConMap up to ListenSocketCapacity */
    if(SoConId < ListenSocketCapacity)
    {
      CONSTP2VAR(SoAd_ListenSocketSoConMapType, AUTOMATIC, SOAD_APPL_DATA) ListenMapEntryPtr
        = &SOAD_PBRAM(ListenSocketSoConMap, SoConId);
      ListenMapEntryPtr->SoConListFirstId = SOAD_INVALID_SOCON_ID;
      ListenMapEntryPtr->ListenSocketId = SOAD_SOCKETID_INVALID;
    }

    /* Initialize SocketSoConMap */
    {
      CONSTP2VAR(SoAd_SocketSoConMapType, AUTOMATIC, SOAD_APPL_DATA) SocketSoConMapEntryPtr
        = &SOAD_PBRAM(SocketSoConMap, SoConId);
      SocketSoConMapEntryPtr->SoConId = SOAD_INVALID_SOCON_ID;
      SocketSoConMapEntryPtr->SocketId = SOAD_SOCKETID_INVALID;
    }

  }
  if(SoAd_PBcfg->SoConToSocketRouteDestMapCnt > 0U)
  {
    SoAd_SocketRouteDestIdType MapId;
    const SoAd_SocketRouteDestIdType MapCnt = SoAd_PBcfg->SoConToSocketRouteDestMapCnt;
    for(MapId = 0U;
        MapId < MapCnt;
        MapId ++)
    {
      CONSTP2VAR(SoAd_SocketRouteDestIdType, AUTOMATIC, SOAD_APPL_DATA) SocketRouteDestIdPtr =
        &SOAD_PBRAM_REF_ID
        (
          SoAd_SocketRouteDestIdType,
          SoAd_PBcfg->SoConToSocketRouteDestMapBase,
          MapId
        );
      *SocketRouteDestIdPtr = SOAD_INVALID_SOCKET_ROUTE_DEST_ID;
    }
  }

}


STATIC FUNC(void, SOAD_CODE) SoAd_VerifyRxTxInProgress(void)
{

  /* if triggerable routing groups exist */
  if(SoAd_RoutingGroupTriggered == TRUE)
  {
    uint32 NextBit;
    const SoAd_RoutingGroupIdType RGTriggerableCnt =
                                  SoAd_PBcfg->RoutingGroupTriggerableCnt;
    CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) RGTriggeredMapPtr =
                      &SOAD_PBRAM_REF_ID(uint32, SoAd_PBcfg->RoutingGroupTriggeredBitMapBase, 0U);
    uint32 StartBit = COMSTACK_BF_START;

    /* Deviation MISRAC2012-3 */
    for(SOAD_COMSTACK_BF_ITERATE(NextBit, &RGTriggeredMapPtr[0U], StartBit, RGTriggerableCnt))
    {
      SoAd_PduRouteDestVirtualIdType MapId;
      SoAd_RoutingGroupIdType InternalRoutingGroupId = (SoAd_RoutingGroupIdType)NextBit;
      P2CONST(SoAd_RoutingGroupPduRouteDestMapType, AUTOMATIC, SOAD_APPL_DATA)
        RoutingGroupTriggerPtr = &SOAD_CFG_TYPE
        (
          SoAd_RoutingGroupPduRouteDestMapType,
          RoutingGroupPduRouteDestMap,
          InternalRoutingGroupId
        );
      const SoAd_PduRouteDestVirtualIdType MaxPduRouteDestMapId =
        RoutingGroupTriggerPtr->PduRouteDestVirtualMapFirstId
        + RoutingGroupTriggerPtr->PduRouteDestVirtualMapCnt;

      /* loop over all PduRouteDests related to this routing group */
      for(MapId = RoutingGroupTriggerPtr->PduRouteDestVirtualMapFirstId;
          MapId < MaxPduRouteDestMapId;
          MapId++
         )
      {
        SoAd_PduRouteDestVirtualIdType PduRouteDestVirtualId =
          SOAD_CFG_TYPE(SoAd_PduRouteDestVirtualIdType,RoutingGroupPduRouteDestVirtualMap, MapId);
        CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr =
          &SOAD_CFG(PduRouteDestVirtual, PduRouteDestVirtualId);
        SoAd_RouteIdType PduRouteId = SOAD_CFG(PduRouteDestVirtual, PduRouteDestVirtualId).PduRouteId;
        P2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr =
          &SOAD_PBRAM(SoConState, PduRouteDestVirtualPtr->SoConId);

        CONSTP2CONST(SoAd_PduRouteType, AUTOMATIC, SOAD_APPL_DATA) PduRoutePtr = &SOAD_CFG(PduRoute,PduRouteId);
        if(!SOAD_FLAG(PduRoutePtr, SOAD_PDUROUTE_FLAG_IS_UL_TP))
        {
          SOAD_FLAG_SET_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RXTX_PROGRESS);
        }
      }
    }
  }
}

STATIC FUNC(void, SOAD_CODE) SoAd_MainFunctionSoCon(void)
{
  uint32 NextSoConId;
  const SoAd_SoConIdType SoConMax = SoAd_PBcfg->SoConCnt;
  P2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr;
  P2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr;

  CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) TransCondChange =
    &SOAD_PBRAM_REF_ID(uint32, SoAd_PBcfg->SoConTransCondChangeBitMapBase, 0U);

  /* Deviation MISRAC2012-3 */
  for(SOAD_COMSTACK_BF_ITERATE(NextSoConId, &TransCondChange[0U], COMSTACK_BF_START, SoConMax))
  {
    uint8 BusyCnt;
    boolean RepeatInNextMainFunction = FALSE;
    SoConPtr = &SOAD_CFG(SoCon, NextSoConId);
    SoConStatePtr = &SOAD_PBRAM(SoConState, NextSoConId);

    TS_AtomicAssign8(BusyCnt, SoConStatePtr->BusyCnt);

    if(BusyCnt == 0U)
    {
      boolean RepeatExecution = FALSE;
      uint8 RxTxFlag;
      uint8 MaxRepIdx;
      if(SoConPtr->RxDataId != SOAD_INVALID_SOCON_ID)
      {
        P2VAR(SoAd_SoConRxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConRxDataPtr = NULL_PTR;
        SoAd_GetSoConRxData(SoConPtr, SoConRxDataPtr);

        /* set the SOAD_SOCONSTATE_FLAG_RXTX_PROGRESS flag if there is a reception in progress */
        if(SoConRxDataPtr->CurrSocketRouteId != SOAD_INVALID_ROUTE_ID)
        {
          SOAD_FLAG_SET_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RXTX_PROGRESS);
        }
      }
      if(SoConPtr->PduTxDataCnt > 0U)
      {
        if(!SOAD_FLAG_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RXTX_PROGRESS))
        {
          const SoAd_PduTxDataIdType MaxBit
            = SoConPtr->PduTxDataFirstId + SoConPtr->PduTxDataCnt;
          /* Initialize with previous. Underflow is covered by ComStack_FindNextOne() */
          const uint32 StartBit = SoConPtr->PduTxDataFirstId - (uint32)1U;
          uint32 NextBit;
          CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) PduTxDataNotIdleBitMap =
              &SOAD_PBRAM_REF_ID(uint32, SoAd_PBcfg->PduTxDataNotIdleBitMapBase, 0U);
          /* loop over all PduRoutes of this socket connections, to find the recently transmitted
             PDUs */
          /* !LINKSTO SoAd.dsn.IndicationBitMaps, 1 */
          /* Deviation MISRAC2012-3 */
          for(SOAD_COMSTACK_BF_ITERATE(NextBit, &PduTxDataNotIdleBitMap[0U], StartBit, MaxBit))
          {
            /* Find the first one. */
            SOAD_FLAG_SET_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RXTX_PROGRESS);
            break;
          }
        }
      }

      TS_AtomicAssign8(RxTxFlag, SoConStatePtr->LowerFlags);
      RxTxFlag &= SOAD_SOCONSTATE_FLAG_RXTX_PROGRESS_MASK;
      if(RxTxFlag != 0U)
      {
        /* Since either Rx or Tx are in progress transition could be suspended to next main function.
           This needs to be considered. */
        RepeatInNextMainFunction = TRUE;
      }

      for(MaxRepIdx = 0U; MaxRepIdx < SOAD_MAX_SM_REPETITION; MaxRepIdx++)
      {
        if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_TCP))
        {
          RepeatExecution = SoAd_MainFunctionSoConTcp(SoConPtr, SoConStatePtr);
        }
        else/* UDP */
        {
          RepeatExecution = SoAd_MainFunctionSoConUdp(SoConPtr, SoConStatePtr);
        }

        if(RepeatExecution == FALSE)
        {
          break;
        }
      }

      /* In case that after repetition loop finished there are still transitions to be done, set
         global flag to enter state machine again in next main function. */
      if(RepeatExecution == TRUE)
      {
        RepeatInNextMainFunction = RepeatExecution;
      }
    }
    else
    {
      RepeatInNextMainFunction = TRUE;
    }

    if(RepeatInNextMainFunction)
    {
      /* Set global flag again. TransCondChange[] bit is still set. */
      TS_AtomicAssign8(SoAd_StateTransCondChange, TRUE);
    }
    else
    {
      const uint16 BitMapIdx = SoConPtr->SoConId / (uint16)32U;
      const uint8 SoConOffset = SoConPtr->SoConId % 32U;
      TS_AtomicClearBit_32(&TransCondChange[BitMapIdx], SoConOffset);
    }

    /* Clear RX/TX flag to unlock state machine transitions to offline. */
    SOAD_FLAG_CLR_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RXTX_PROGRESS);
  }
}

STATIC FUNC(boolean, SOAD_CODE) SoAd_MainFunctionSoConUdp
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  boolean RepeatExecution = FALSE;
  SoAd_Int_08_UdpSMTriggerType UdpSMTrigger;
  SoAd_Int_08_UdpSMTriggerEval(SoConPtr, SoConStatePtr, &UdpSMTrigger);

  if(UdpSMTrigger.UpClosing)
  {
    RepeatExecution = SoAd_Int_01_UdpSM_UpClosing(SoConPtr, SoConStatePtr);
  }
  if(UdpSMTrigger.Error && (!RepeatExecution))
  {
    RepeatExecution = SoAd_Int_01_UdpSM_Error(SoConPtr, SoConStatePtr);
  }
  if(UdpSMTrigger.Closed && (!RepeatExecution))
  {
    RepeatExecution = SoAd_Int_01_UdpSM_Closed(SoConPtr, SoConStatePtr);
  }
  if(UdpSMTrigger.Connecting && (!RepeatExecution))
  {
    RepeatExecution = SoAd_Int_01_UdpSM_Connecting(SoConPtr, SoConStatePtr);
  }
  if(UdpSMTrigger.Connected && (!RepeatExecution))
  {
    RepeatExecution = SoAd_Int_01_UdpSM_Connected(SoConPtr, SoConStatePtr);
  }
  if(UdpSMTrigger.Release && (!RepeatExecution))
  {
    /* reset the remote address to the configured one */
    if(SoAd_ResetRemoteAddress(SoConPtr, SoConStatePtr) == TRUE)
    {
      /* Remote Address contains Wildcard --> change socket state to RECONNECT */
      RepeatExecution = SoAd_Int_01_UdpSM_Disconnected(SoConPtr, SoConStatePtr);
      SOAD_FLAG_CLR_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_LOCK);
    }
  }

  return RepeatExecution;
}

STATIC FUNC(void, SOAD_CODE) SoAd_Int_08_UdpSMTriggerEval
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(SoAd_Int_08_UdpSMTriggerType, AUTOMATIC, SOAD_APPL_DATA) UdpSMTriggerPtr
)
{
  UdpSMTriggerPtr->Connecting = FALSE;
  UdpSMTriggerPtr->Error = FALSE;
  UdpSMTriggerPtr->UpClosing = FALSE;
  UdpSMTriggerPtr->Closed = FALSE;
  UdpSMTriggerPtr->Connected = FALSE;
  UdpSMTriggerPtr->Release = FALSE;

  SOAD_ENTER_CRITSEC();

  /* the socket connection shall be closed immediately in one of the following situation:
   * - SoAd_CloseSoCon(TRUE) was called
   * - SoAd_CloseSoCon(FALSE) was called and no Rx/Tx was in progress. */
  if(
      (
        (!SOAD_FLAG_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RXTX_PROGRESS)) ||
        SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_CLOSE_SOCON_ABORT)
      )
      &&
      SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_CLOSE_SOCON)
    )
  {
    UdpSMTriggerPtr->UpClosing = TRUE;
  }

  /* the socket connection shall be closed immediately in one of the following situations:
   * - SoAd_TpCancelTransmit() was called (concurrent receptions are cancelled too)
   * - SoAd_TpCancelReceive() was called (concurrent transmissions are cancelled too) */
  if(
      SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_TP_CANCELED) ||
      SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RX_TP_CANCELED) ||
      SOAD_FLAG_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RECONNECT_SOCKET)
    )
  {
    UdpSMTriggerPtr->Error = TRUE;
  }
  /* the socket connection shall be closed immediately in one of the following situation:
   * - the socket was closed by the TcpIp module */
  UdpSMTriggerPtr->Closed = SOAD_FLAG_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TCPIP_SOCKET_CLOSED);

  /* if at least one UL requested the socket (OpenRefCnt),
   * the remote IP address is set */
  {
    /* Note: The state OFF shall only be left if local IP address is assigned or on hold.
     *       No need to handle a change back to local IP address unassigned since TcpIp will
     *       always close the sockets before local IP address notification. */
    /* Deviation MISRAC2012-6  <+6> */
    const uint8 LocalIpAddrValid =
      SOAD_GET_BIT_FROM_BITARRAY
        (
          &SOAD_PBRAM_REF_ID(uint8, SoAd_PBcfg->LocalIpAddrStateBitMapBase, 0U),
          SOAD_CFG(SoConGroup, SoConPtr->SoConGroupId).LocalIpAddrId
        );

    const boolean RemoteIsWildcard
      = SOAD_IS_REMOTE_WILDCARD(SoConStatePtr->RemoteAddress, SoConStatePtr->RemotePort);

    /* SoConState(SOAD_SOCONSTATE_FLAG_REMOTE_SET) indicated if a remote address is available
     * The remote address can be either from config or set by upper layer. A remote address is
     * required independent if it is valid or not to get connected.
     * This flag is required since the remote address can be disabled in configuration.
     */
    if(
        (SoConStatePtr->OpenRefCnt > 0U)
        && (LocalIpAddrValid != 0U)
        && SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_REMOTE_SET)
      )
    {
      UdpSMTriggerPtr->Connecting = TRUE;
    }

    if(
        SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_UDP_LISTEN_ONLY)
        || SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_NO_MSG_FILTER)
        || (!RemoteIsWildcard)
      )
    {
      UdpSMTriggerPtr->Connected = TRUE;
    }
  }

  if(
      SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_LOCK) &&
      (!SOAD_FLAG_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RXTX_PROGRESS))
    )
  {
    UdpSMTriggerPtr->Release = TRUE;
  }

  SOAD_EXIT_CRITSEC();
}

STATIC FUNC(boolean, SOAD_CODE) SoAd_MainFunctionSoConTcp
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  boolean RepeatExecution = FALSE;

  boolean ConnectingTCP = FALSE;
  boolean LoClosingTCP = FALSE;
  boolean UpClosingTCP = FALSE;
  boolean ClosedTCP = FALSE;

  SOAD_ENTER_CRITSEC();
  /* the socket connection shall be closed immediately in one of the following situation:
   * - SoAd_CloseSonCon() was called. */
  if(
      SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_CLOSE_SOCON)
    )
  {
    UpClosingTCP = TRUE;
  }

  /* the socket connection shall be closed immediately in one of the following situations:
     * - SoAd_TpCancelTransmit() was called (concurrent receptions are cancelled too)
     * - SoAd_TpCancelReceive() was called (concurrent transmissions are cancelled too)
     * - SoAd_TcpIpEvent() was called with event TCPIP_TCP_FIN_RECEIVED or TCPIP_TCP_RESET */
  if(
      SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_TP_CANCELED) ||
      SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RX_TP_CANCELED) ||
      SOAD_FLAG_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RECONNECT_SOCKET) ||
      SOAD_FLAG_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TCP_FIN_RECEIVED)
    )
  {
    LoClosingTCP = TRUE;
  }

  /* the socket connection shall be closed immediately in one of the following situation:
   * - the socket was closed by the TcpIp module */
  ClosedTCP = SOAD_FLAG_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TCPIP_SOCKET_CLOSED);

  /* if at least one UL requested the socket (OpenRefCnt),
   * the remote IP address is set */
  {
    /* Note: The state OFF shall only be left if local IP address is assigned or on hold.
     *       No need to handle a change back to local IP address unassigned since TcpIp will
     *       always close the sockets before local IP address notification. */
    /* Deviation MISRAC2012-6  <+6> */
    const uint8 LocalIpAddrValid =
      SOAD_GET_BIT_FROM_BITARRAY
        (
          &SOAD_PBRAM_REF_ID(uint8, SoAd_PBcfg->LocalIpAddrStateBitMapBase, 0U),
          SOAD_CFG(SoConGroup, SoConPtr->SoConGroupId).LocalIpAddrId
        );

    /* !LINKSTO SoAd.EB.TcpSM_TriggerConnecting_TcpClient, 1 */
    /* !LINKSTO SoAd.EB.TcpSM_TriggerConnecting_TcpServer, 1 */
    /* SoConState(SOAD_SOCONSTATE_FLAG_REMOTE_SET) indicated if a remote address is available
     * The remote address can be either from config or set by upper layer. A remote address is
     * required independent if it is valid or not to get connected.
     * This flag is required since the remote address can be disabled in configuration.
     */
    if(
        (SoConStatePtr->OpenRefCnt > 0U) && (LocalIpAddrValid != 0U) &&
        SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_REMOTE_SET)
      )
    {
      if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_TCP_INITIATE))
      {
        /* TCP Client connections shall only try to connect if valid remote address is available. */
        if(!SOAD_IS_REMOTE_WILDCARD(SoConStatePtr->RemoteAddress, SoConStatePtr->RemotePort))
        {
          ConnectingTCP = TRUE;
        }
      }
      else
      {
        /* TCP Server connections. */
        ConnectingTCP = TRUE;
      }
    }
  }

  SOAD_EXIT_CRITSEC();

  if(UpClosingTCP)
  {
    RepeatExecution = SoAd_Int_04_TcpSM_UpClosing(SoConPtr, SoConStatePtr);
  }
  if(LoClosingTCP && (!RepeatExecution))
  {
    RepeatExecution = SoAd_Int_04_TcpSM_LoClosing(SoConPtr, SoConStatePtr);
  }
  if(ClosedTCP && (!RepeatExecution))
  {
    RepeatExecution = SoAd_Int_04_TcpSM_Closed(SoConPtr, SoConStatePtr);
  }
  if(ConnectingTCP && (!RepeatExecution))
  {
    RepeatExecution = SoAd_Int_04_TcpSM_Connecting(SoConPtr, SoConStatePtr);
  }

  return RepeatExecution;
}




/** \brief Translates an external routing group id to an internal one.
 **        This translation is required because external routing groups are converted to multiple
*          internal routing groups to represent separate states for multiple socket connections.
 ** \param[in] id       Index of the external routing group.
 ** \param[in] soConId  Index of the socket connection
                        (optional - set to SOAD_INVALID_SOCON_ID otherwise).
 ** \returns   SoAd_RoutingGroupIdType
 ** \retval    Index of the internal routing group.
 **/
STATIC FUNC(SoAd_RoutingGroupIdType, SOAD_CODE) SoAd_RoutingGroupExtToInt
(
  SoAd_RoutingGroupIdType Id,
  SoAd_SoConIdType SoConId
)
{
  CONSTP2CONST(SoAd_RoutingGroupExtToIntIdMapType, AUTOMATIC, SOAD_APPL_DATA)
    RoutingGroupExtToIntIdMapEntryPtr = &SOAD_CFG(RoutingGroupExtToIntIdMap, Id);
  SoAd_RoutingGroupIdType InternalRoutingGroupId = SOAD_INVALID_ROUTINGGROUP_ID;

  /* Note: SoAd differentiates between "global" and "specific" SoAdRoutingGroups.
   * Global SoAdRoutingGroups have only one state (enabled/disabled) whereas specific
   * SoAdRoutingGroups can be en-/disabled per SoAdSocketConnection individually. */

  /* For global SoAdRoutingGroups the functions SoAd_EnableRouting, SoAd_DisableRouting as well as
   * SoAd_IfRoutingGroupTransmit() shall be used exclusively, hence
   * => soConId must NOT be set
   * => related RoutingGroup does NOT refer to a SoConGroup, i.e. SoConGroupId is invalid */
  if(
      (SOAD_INVALID_SOCON_ID == SoConId)
      && (SOAD_INVALID_SOCON_ID == RoutingGroupExtToIntIdMapEntryPtr->SoConGroupId)
    )
  {
      InternalRoutingGroupId = RoutingGroupExtToIntIdMapEntryPtr->RoutingGroupId;
  }
  /* For specific SoAdRoutingGroups the functions SoAd_EnableSpecificRouting(),
   * SoAd_DisableSpecificRouting() as well as SoAd_IfSpecificRoutingGroupTransmit() shall be used
   * exclusively, hence
   * => soConId must be set
   * => related RoutingGroup must refer to a SoConGroup, i.e. SoConGroupId is invalid */
  else if(
           (SOAD_INVALID_SOCON_ID != SoConId)
           && (SOAD_INVALID_SOCON_ID != RoutingGroupExtToIntIdMapEntryPtr->SoConGroupId)
         )
  {
    CONSTP2CONST(SoAd_SoConGroupType, AUTOMATIC, SOAD_APPL_DATA) SoConGroupPtr =
      &SOAD_CFG(
                 SoConGroup,
                 RoutingGroupExtToIntIdMapEntryPtr->SoConGroupId
               );

    /* check the selected SoCon is part of the SoConGroup referred by the RoutingGroup */
    if(
        (SoConId >= SoConGroupPtr->SoConFirstId)
        && (SoConId < (SoConGroupPtr->SoConFirstId + SoConGroupPtr->SoConCnt))
      )
    {
      InternalRoutingGroupId =
        RoutingGroupExtToIntIdMapEntryPtr->RoutingGroupId + (SoConId - SoConGroupPtr->SoConFirstId);
    }
  }
  else
  {
    /* Wrong API was used -> return SOAD_INVALID_ROUTINGGROUP_ID */
  }

  return InternalRoutingGroupId;
}

/** \brief Performs a routing group related action (enable, disable or trigger),
 **        either on a global routing group or on a specific socket connection.
 ** \param[in] id       Index of the routing group.
 ** \param[in] soConId  Index of the socket connection
                        (optional - set to SOAD_INVALID_SOCON_ID otherwise).
 ** \param[in] action   Type of action to perform for this routing group.
 ** \returns   Std_ReturnType
 ** \retval    E_OK ... Action performed, E_NOT_OK ... development error detected.
 **/
STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_RoutingGroupAction
(
  SoAd_RoutingGroupIdType ExternalRoutingGroupId,
  SoAd_SoConIdType SoConId,
  SoAd_RoutingGroupActionType Action,
  uint8 ServiceId
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_SOAD_ROUTINGGROUPACTION_ENTRY(ExternalRoutingGroupId, SoConId, Action, ServiceId);

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  if(SoAd_Initialized == SOAD_UNINITED)
  {
    SOAD_DET_REPORT_ERROR(ServiceId, SOAD_E_NOTINIT);
  }
  else if(ExternalRoutingGroupId >= SoAd_PBcfg->RoutingGroupCnt)
  {
    SOAD_DET_REPORT_ERROR(ServiceId, SOAD_E_INV_ARG);
  }
  else
#else
  TS_PARAM_UNUSED(ServiceId);
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /*
     * Global Routing Groups as well as Specific Routing Groups yield to one single InternalId.
     * Global because they are represented by only one id and specific because they have the
     * SoConId to define the offset
     */
    SoAd_RoutingGroupIdType InternalRoutingGroupId =
      SoAd_RoutingGroupExtToInt(ExternalRoutingGroupId, SoConId);

    if(SOAD_INVALID_ROUTINGGROUP_ID != InternalRoutingGroupId)
    {
      SOAD_ENTER_CRITSEC();
      switch(Action)
      {
        case SOAD_ROUTINGGROUPACTION_ENABLE:
        {
          RetVal = SoAd_EnableRoutingGroup_RxDataStructures(SoConId, ExternalRoutingGroupId, ServiceId);
          if(RetVal == E_OK)
          {
            SoAd_EnableRoutingGroup_TxDataStructures(InternalRoutingGroupId);
            SOAD_ENABLE_ROUTINGGROUP(InternalRoutingGroupId);
          }
          break;
        }
        case SOAD_ROUTINGGROUPACTION_DISABLE:
        {
          SoAd_DisableRoutingGroup_RxDataStructures(SoConId, ExternalRoutingGroupId);
          SoAd_DisableRoutingGroup_TxDataStructures(InternalRoutingGroupId);
          SOAD_DISABLE_ROUTINGGROUP(InternalRoutingGroupId);
          RetVal = E_OK;
          break;
        }
        case SOAD_ROUTINGGROUPACTION_TRIGGER:
        {
          if(SoAd_PBcfg->RoutingGroupTriggerableCnt > InternalRoutingGroupId)
          {
            SOAD_SET_ROUTINGGROUP_TRIGGER(InternalRoutingGroupId);
            SoAd_RoutingGroupTriggered = TRUE;
            RetVal = E_OK;
          }
#if(SOAD_DEV_ERROR_DETECT == STD_ON)
          else
          {
            SOAD_DET_REPORT_ERROR(ServiceId, SOAD_E_INV_ARG);
          }
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
          break;
        }
        /* CHECK: NOPARSE */
        default:
        {
          SOAD_UNREACHABLE_CODE_ASSERT(ServiceId);
          break; /* Caution: this 'break' is required by MISRA-C:2012 */
        /* CHECK: PARSE */
        }
      }
      SOAD_EXIT_CRITSEC();
    }
#if(SOAD_DEV_ERROR_DETECT == STD_ON)
    else
    {
      SOAD_DET_REPORT_ERROR(ServiceId, SOAD_E_INV_ARG);
    }
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  }

  DBG_SOAD_ROUTINGGROUPACTION_EXIT(RetVal, ExternalRoutingGroupId, SoConId, Action, ServiceId);
  return RetVal;
}

STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_EnableRoutingGroup_RxDataStructures
(
  SoAd_SoConIdType SoConId,
  SoAd_RoutingGroupIdType ExternalRoutingGroupId,
  uint8 ServiceId
)
{
  Std_ReturnType RetVal = E_NOT_OK;
#if(!(SOAD_DEV_ERROR_DETECT == STD_ON))
  TS_PARAM_UNUSED(ServiceId);
#endif
  if(SoConId != SOAD_INVALID_SOCON_ID)
  {
    CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr =
      &SOAD_CFG(SoCon, SoConId);
    CONSTP2CONST(SoAd_SoConGroupType, AUTOMATIC, SOAD_APPL_DATA) SoConGroupPtr =
      &SOAD_CFG(SoConGroup, SoConPtr->SoConGroupId);
    CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConFirstIdPtr =
      &SOAD_CFG(SoCon, SoConGroupPtr->SoConFirstId);
    SoAd_RouteIdType SocketRouteId;
    const SoAd_RouteIdType MaxRouteId = SoConFirstIdPtr->SocketRouteFirstId
                                        + SoConFirstIdPtr->SocketRouteCnt;
    if(SoConFirstIdPtr->SocketRouteFirstId != SOAD_INVALID_ROUTE_ID)
    {
#if(SOAD_DEV_ERROR_DETECT == STD_ON)
      boolean ErrorTwoSoConSamePduOccurred = FALSE,ErrorTwoPdusSameSoConOccurred = FALSE;
#endif
      for(
           SocketRouteId = SoConFirstIdPtr->SocketRouteFirstId;
           SocketRouteId < MaxRouteId;
           SocketRouteId++
         )
      {
        CONSTP2CONST(SoAd_SocketRouteType, AUTOMATIC, SOAD_APPL_DATA) SocketRoutePtr =
          &SOAD_CFG(SocketRoute, SocketRouteId);
        /* If there is only 1 destination, this is not multiple psi and we can
         * directly go to the destination */
        if(SocketRoutePtr->SocketRouteDestCnt == 1U)
        {
          boolean DestBelongsToRoutingGroup = FALSE;
          SoAd_SocketRouteDestType const *SocketRouteDestPtr =
            &SOAD_CFG(SocketRouteDest, SocketRoutePtr->SocketRouteDestFirstId);
          const SoAd_RoutingGroupMapIdType RGMaxId = SocketRouteDestPtr->RoutingGroupMapFirstId
                                                     + SocketRouteDestPtr->RoutingGroupMapCnt;
          SoAd_RoutingGroupMapIdType RGMapId;
          for(RGMapId = SocketRouteDestPtr->RoutingGroupMapCnt;
              RGMapId < RGMaxId;
              RGMapId++
             )
          {
            SoAd_RoutingGroupIdType RoutingGroupExtFromRGMapId =
              SOAD_CFG_TYPE(SoAd_RoutingGroupIdType, SocketRouteRoutingGroupMap, RGMapId);
            /* The check if the InternalRoutinggroup matches to the ExternalRG was already done.
             * Therefore the check of the external RoutingGroup is sufficient.*/
            if(RoutingGroupExtFromRGMapId == ExternalRoutingGroupId)
            {
              DestBelongsToRoutingGroup = TRUE;
              break;
            }
          }
          if(DestBelongsToRoutingGroup == TRUE)
          {
            SoAd_SetBitIn32BitMap(
              SocketRoutePtr->SocketRouteDestFirstId,
              &SoAd_PBcfg->SocketRouteDestEnabledBitMapBase);
          }
          RetVal = E_OK;
        }
        else
        {
          /* Multiple SocketRouteDestinations */
          SoAd_SocketRouteDestIdType DestMapId;
          SoAd_SocketRouteDestIdType DestWithCorrectRoutingGroupId =
            SOAD_INVALID_SOCKET_ROUTE_DEST_ID;
          /* Range check of (SoConId - SoConGroupPtr->SoConFirstId) is done
           * in SoAd_RoutingGroupExtToInt() */
          const SoAd_SocketRouteDestIdType SoConOffSet =
            ((SoAd_SocketRouteDestIdType)SoConId) -
            ((SoAd_SocketRouteDestIdType)SoConGroupPtr->SoConFirstId);
          const SoAd_SocketRouteDestIdType SoConToSocketRouteDestMapId =
            SocketRoutePtr->SoConToSocketRouteDestMapFirstId + SoConOffSet;
          P2CONST(SoAd_RoutingGroupSocketRouteDestMapMapType,
            AUTOMATIC,
            SOAD_APPL_DATA) RoutingGroupSocketRouteDestMapMapPtr =
            &SOAD_CFG(RoutingGroupSocketRouteDestMapMap, ExternalRoutingGroupId);
          const SoAd_SocketRouteDestIdType MaxDestMapId =
              RoutingGroupSocketRouteDestMapMapPtr->SocketRouteDestMapFirstId
                + RoutingGroupSocketRouteDestMapMapPtr->SocketRouteDestMapCnt;
          const SoAd_SocketRouteDestIdType MaxDestId =
              SocketRoutePtr->SocketRouteDestFirstId
                + SocketRoutePtr->SocketRouteDestCnt;
          /* Find SocketRouteDestination that is referenced from the external RG parameter.
           * Within one SocketRoute one RG can only be referenced once so the first one is correct
           * This loop will never terminate because of
           * SoAd.dsn.Func0130.XdmCheck.MultiplePSI.NoRoutingGroupRef */
          /* CHECK: NOPARSE */
          for(DestMapId = RoutingGroupSocketRouteDestMapMapPtr->SocketRouteDestMapFirstId;
              DestMapId < MaxDestMapId;
              DestMapId++)
          /* CHECK: PARSE */
          {
            SoAd_SocketRouteDestIdType SocketRouteDestId =
              SOAD_CFG_TYPE(
                SoAd_SocketRouteDestIdType,
                RoutingGroupSocketRouteDestMap,
                DestMapId);
            if(
                SocketRouteDestId >= SocketRoutePtr->SocketRouteDestFirstId
                && SocketRouteDestId < MaxDestId
               )
            {
              /* An XDM Check prevents us from having multiple destinations per socket
               * Route per external routing group */
              DestWithCorrectRoutingGroupId = SocketRouteDestId;
              break;
            }
          }
          if(DestWithCorrectRoutingGroupId != SOAD_INVALID_SOCKET_ROUTE_DEST_ID)
          {
            /* If there is a destination for the external RG we know where in the
             * SoConToSocketRouteDestMap we should save the value. */
            CONSTP2VAR(SoAd_SocketRouteDestIdType, AUTOMATIC, SOAD_APPL_DATA)
              ExpectedSocketRouteDestIdPtr =
              &SOAD_PBRAM_REF_ID(SoAd_SocketRouteDestIdType,
                                 SoAd_PBcfg->SoConToSocketRouteDestMapBase,
                                 SoConToSocketRouteDestMapId
                                );
            if((*ExpectedSocketRouteDestIdPtr) == SOAD_INVALID_SOCKET_ROUTE_DEST_ID)
            {
              /* The MapEntry within the SoConToSocketRouteDestMap is unused.
               * Now we have to additionally check if the same desination was designated in
               * any other MapEntry */
              SoAd_SocketRouteDestIdType CurrentDestId;
              const SoAd_SocketRouteDestIdType MaxRouteDestId =
                SocketRoutePtr->SoConToSocketRouteDestMapFirstId
                + SocketRoutePtr->SocketRouteDestCnt;
              boolean DestIdUniqueInSocketRoute = TRUE;
              for(CurrentDestId = SocketRoutePtr->SoConToSocketRouteDestMapFirstId;
                  CurrentDestId < MaxRouteDestId;
                  CurrentDestId++
                 )
              {
                CONSTP2VAR(SoAd_SocketRouteDestIdType, AUTOMATIC, SOAD_APPL_DATA)
                  CurrentSocketRouteDestIdPtr =
                  &SOAD_PBRAM_REF_ID(SoAd_SocketRouteDestIdType,
                                     SoAd_PBcfg->SoConToSocketRouteDestMapBase,
                                     CurrentDestId
                                    );
                if((*CurrentSocketRouteDestIdPtr) == DestWithCorrectRoutingGroupId)
                {
#if(SOAD_DEV_ERROR_DETECT == STD_ON)
                  /* Same Pdu cannnot be configurated to two different SocketConnections */
                  ErrorTwoSoConSamePduOccurred = TRUE;
#endif
                  DestIdUniqueInSocketRoute = FALSE;
                  break;
                }
              }
              if(DestIdUniqueInSocketRoute)
              {
                /* !LINKSTO SoAd.dsn.Func0130.Rx.Udp.If.EnableServiceInstance, 1 */
                *ExpectedSocketRouteDestIdPtr = DestWithCorrectRoutingGroupId;
                RetVal = E_OK;
              }
            }
            /* same destination allowed */
            else if((*ExpectedSocketRouteDestIdPtr) == DestWithCorrectRoutingGroupId)
            {
              /* !LINKSTO SoAd.dsn.Func0130.Rx.Udp.If.EnableSameServiceInstanceMultipleTimes, 1 */
              /*Setting the same destination is allowed*/
              RetVal = E_OK;
            }
            else
            {
#if(SOAD_DEV_ERROR_DETECT == STD_ON)
              /* Two Pdus cant be used with the same SocketConnection */
              ErrorTwoPdusSameSoConOccurred = TRUE;
#endif
            }
          }
        }
      }
/* !LINKSTO SoAd.dsn.Func0130.Rx.Udp.If.TwoPdusForSameSoCon, 1 */
#if(SOAD_DEV_ERROR_DETECT == STD_ON)
      if(ErrorTwoPdusSameSoConOccurred == TRUE)
      {
        SOAD_DET_REPORT_ERROR(ServiceId, SOAD_E_TWO_PDUS_SAME_SOCON);
      }
#endif

/* !LINKSTO SoAd.dsn.Func0130.Rx.Udp.If.TwoSoConSamePdu, 1 */
#if(SOAD_DEV_ERROR_DETECT == STD_ON)
      if(ErrorTwoSoConSamePduOccurred == TRUE)
      {
        SOAD_DET_REPORT_ERROR(ServiceId, SOAD_E_TWO_SOCON_SAME_PDU);
      }
#endif
    }
    else /* SoCons can exist without SocketRoutes */
    {
      RetVal = E_OK;
    }
  }
  else /* global Routing Group */
  {
    SoAd_SocketRouteDestIdType DestMapId;
    P2CONST(SoAd_RoutingGroupSocketRouteDestMapMapType,
      AUTOMATIC, SOAD_APPL_DATA) RoutingGroupSocketRouteDestMapMapPtr =
      &SOAD_CFG(RoutingGroupSocketRouteDestMapMap, ExternalRoutingGroupId);
    const SoAd_SocketRouteDestIdType MaxDestMapId =
      RoutingGroupSocketRouteDestMapMapPtr->SocketRouteDestMapFirstId
      + RoutingGroupSocketRouteDestMapMapPtr->SocketRouteDestMapCnt;
    for(DestMapId = RoutingGroupSocketRouteDestMapMapPtr->SocketRouteDestMapFirstId;
        DestMapId < MaxDestMapId;
        DestMapId++
       )
    {
      SoAd_SocketRouteDestIdType SocketRouteDestId =
        SOAD_CFG_TYPE(SoAd_SocketRouteDestIdType,RoutingGroupSocketRouteDestMap, DestMapId);
      SoAd_SetBitIn32BitMap(SocketRouteDestId, &SoAd_PBcfg->SocketRouteDestEnabledBitMapBase);
    }
    RetVal = E_OK;
  }
  return RetVal;
}

STATIC FUNC(void, SOAD_CODE) SoAd_EnableRoutingGroup_TxDataStructures
(
  SoAd_RoutingGroupIdType InternalRoutingGroupId
)
{
  /*
   * At least one active routing group sets the destination therefore this is sufficient
   */
  CONSTP2CONST(SoAd_RoutingGroupPduRouteDestMapType, AUTOMATIC, SOAD_APPL_DATA)
    RoutingGroupPduRouteDestMapPtr =
    &SOAD_CFG_TYPE
    (
      SoAd_RoutingGroupPduRouteDestMapType,
      RoutingGroupPduRouteDestMap,
      InternalRoutingGroupId
    );
  SoAd_PduRouteDestVirtualIdType MapId;
  const SoAd_PduRouteDestVirtualIdType MaxPduRouteDestMapId =
    RoutingGroupPduRouteDestMapPtr->PduRouteDestVirtualMapFirstId
    + RoutingGroupPduRouteDestMapPtr->PduRouteDestVirtualMapCnt;
  /* loop over all PduRouteDests related to this routing group */
  for(MapId = RoutingGroupPduRouteDestMapPtr->PduRouteDestVirtualMapFirstId;
      MapId < MaxPduRouteDestMapId;
      MapId++
     )
  {
    SoAd_PduRouteDestVirtualIdType PduRouteDestVirtualId =
      SOAD_CFG_TYPE(SoAd_PduRouteDestVirtualIdType,RoutingGroupPduRouteDestVirtualMap, MapId);
    SoAd_SetBitIn32BitMap(PduRouteDestVirtualId,
                          &SoAd_PBcfg->PduRouteDestVirtualTxEnabledBitMapBase
                         );
  }
}

STATIC FUNC(void, SOAD_CODE) SoAd_DisableRoutingGroup_TxDataStructures
(
  SoAd_RoutingGroupIdType InternalRoutingGroupId
)
{
  /*
   * All routing groups for the destination have to be disabled to disable the destination.
   * Therefore all have to be checked.
   */
  CONSTP2CONST(SoAd_RoutingGroupPduRouteDestMapType, AUTOMATIC, SOAD_APPL_DATA)
    RoutingGroupPduRouteDestMapPtr =
    &SOAD_CFG_TYPE
    (
      SoAd_RoutingGroupPduRouteDestMapType,
      RoutingGroupPduRouteDestMap,
      InternalRoutingGroupId
    );
  SoAd_PduRouteDestVirtualIdType MapId;
  const SoAd_PduRouteDestVirtualIdType MaxPduRouteDestMapId =
    RoutingGroupPduRouteDestMapPtr->PduRouteDestVirtualMapFirstId
    + RoutingGroupPduRouteDestMapPtr->PduRouteDestVirtualMapCnt;
  /* loop over all PduRouteDests related to this routing group */
  for(MapId = RoutingGroupPduRouteDestMapPtr->PduRouteDestVirtualMapFirstId;
      MapId < MaxPduRouteDestMapId;
      MapId++
     )
  {
    SoAd_PduRouteDestVirtualIdType PduRouteDestVirtualId =
      SOAD_CFG_TYPE(SoAd_PduRouteDestVirtualIdType,
                    RoutingGroupPduRouteDestVirtualMap,
                    MapId
                   );
    CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr =
      &SOAD_CFG(PduRouteDestVirtual, PduRouteDestVirtualId);
    boolean IsEnabled = SoAd_OneOtherInternalRGActiveForPduRouteDest(
                                                                     PduRouteDestVirtualPtr,
                                                                     InternalRoutingGroupId
                                                                    );
    if(!IsEnabled)
    {
      SoAd_ClearBitIn32BitMap(PduRouteDestVirtualId,
                              &SoAd_PBcfg->PduRouteDestVirtualTxEnabledBitMapBase
                             );
    }
  }
}

STATIC FUNC(boolean, SOAD_CODE) SoAd_OneOtherInternalRGActiveForPduRouteDest
(
  P2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr,
  SoAd_RoutingGroupIdType InternalRoutingGroupId
)
{
  boolean IsEnabled = FALSE;
  SoAd_RoutingGroupMapIdType RgId;
  CONSTP2CONST(SoAd_PduRouteDestCommonType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestCommonPtr =
    &SOAD_CFG(PduRouteDestCommon, PduRouteDestVirtualPtr->PduRouteDestCommonId);
  const SoAd_RoutingGroupMapIdType RoutingGroupMapFirstId =
    PduRouteDestCommonPtr->RoutingGroupMapFirstId;
  const SoAd_RoutingGroupMapIdType MaxRoutingGroupMap =
    RoutingGroupMapFirstId + PduRouteDestCommonPtr->RoutingGroupMapCnt;
  for(RgId = RoutingGroupMapFirstId; RgId < MaxRoutingGroupMap; RgId++)
  {
    SoAd_RoutingGroupIdType RoutingGroupExtId = SOAD_CFG_TYPE(SoAd_RoutingGroupIdType,
                                                              PduRouteDestRoutingGroupMap,
                                                              RgId
                                                             );
    CONSTP2CONST(SoAd_RoutingGroupExtToIntIdMapType, AUTOMATIC, SOAD_APPL_DATA)
      RoutingGroupExtToIntIdMapEntryPtr
      = &SOAD_CFG(RoutingGroupExtToIntIdMap, RoutingGroupExtId);
    SoAd_RoutingGroupIdType RoutingGroupIntId = RoutingGroupExtToIntIdMapEntryPtr->RoutingGroupId;
    CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr =
    &SOAD_CFG(SoCon, PduRouteDestVirtualPtr->SoConId);
    if(SOAD_INVALID_SOCON_ID != RoutingGroupExtToIntIdMapEntryPtr->SoConGroupId)
    {
      CONSTP2CONST(SoAd_SoConGroupType, AUTOMATIC, SOAD_APPL_DATA) SoConGroupPtr =
      &SOAD_CFG(SoConGroup, RoutingGroupExtToIntIdMapEntryPtr->SoConGroupId);
      SOAD_PRECONDITION_ASSERT
      (
      SoConPtr->SoConGroupId == RoutingGroupExtToIntIdMapEntryPtr->SoConGroupId,
      SOAD_IFTRANSMIT_SVCID
      );
      RoutingGroupIntId += SoConPtr->SoConId - SoConGroupPtr->SoConFirstId;
    }
    /* Check if any other than the currently to turn off routing group is active. */
    if((RoutingGroupIntId != InternalRoutingGroupId)
        && (SOAD_IS_ROUTINGGROUP_ENABLED(RoutingGroupIntId) == TRUE)
      )
    {
      IsEnabled = TRUE;
      break;
    }
  }
  return IsEnabled;
}

STATIC FUNC(void, SOAD_CODE) SoAd_DisableRoutingGroup_RxDataStructures
(
  SoAd_SoConIdType SoConId,
  SoAd_RoutingGroupIdType ExternalRoutingGroupId
)
{
  if(SoConId != SOAD_INVALID_SOCON_ID)
  {
    CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr =
    &SOAD_CFG(SoCon, SoConId);
    CONSTP2CONST(SoAd_SoConGroupType, AUTOMATIC, SOAD_APPL_DATA) SoConGroupPtr =
    &SOAD_CFG(SoConGroup, SoConPtr->SoConGroupId);
    CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConFirstIdPtr =
    &SOAD_CFG(SoCon, SoConGroupPtr->SoConFirstId);
    SoAd_RouteIdType SocketRouteId;
    const SoAd_RouteIdType MaxRouteId =
      SoConFirstIdPtr->SocketRouteFirstId + SoConFirstIdPtr->SocketRouteCnt;
    for(SocketRouteId = SoConFirstIdPtr->SocketRouteFirstId;
        SocketRouteId < MaxRouteId;
        SocketRouteId++
       )
    {
      CONSTP2CONST(SoAd_SocketRouteType, AUTOMATIC, SOAD_APPL_DATA) SocketRoutePtr =
        &SOAD_CFG(SocketRoute, SocketRouteId);
      if(SocketRoutePtr->SocketRouteDestCnt == 1)
      {
        boolean OtherRoutingGroupIsStillEnabled = SoAd_OneOtherInternalRGActiveForSocketRouteDest
        (
          &SOAD_CFG(SocketRouteDest, SocketRoutePtr->SocketRouteDestFirstId),
          ExternalRoutingGroupId
        );
        if(!OtherRoutingGroupIsStillEnabled)
        {
          SoAd_ClearBitIn32BitMap(SocketRoutePtr->SocketRouteDestFirstId,
                                  &SoAd_PBcfg->SocketRouteDestEnabledBitMapBase
                                 );
        }
      }
      else
      {/* Multiple SocketRouteDest */
        const SoAd_SocketRouteDestIdType SoConOffSet =
            ((SoAd_SocketRouteDestIdType)SoConId) -
            ((SoAd_SocketRouteDestIdType)SoConGroupPtr->SoConFirstId);
        const SoAd_SocketRouteDestIdType SoConToSocketRouteDestMapId =
          SocketRoutePtr->SoConToSocketRouteDestMapFirstId + SoConOffSet;
        CONSTP2VAR(SoAd_SocketRouteDestIdType, AUTOMATIC, SOAD_APPL_DATA) SocketRouteDestIdPtr =
          &SOAD_PBRAM_REF_ID(SoAd_SocketRouteDestIdType,
                             SoAd_PBcfg->SoConToSocketRouteDestMapBase,
                             SoConToSocketRouteDestMapId
                            );
        SoAd_SocketRouteDestIdType CurrentDestId = (*SocketRouteDestIdPtr);
        if(CurrentDestId != SOAD_INVALID_SOCKET_ROUTE_DEST_ID)
        {
          boolean OtherRoutingGroupIsStillEnabled = SoAd_OneOtherInternalRGActiveForSocketRouteDest
                    (
                        &SOAD_CFG(SocketRouteDest,CurrentDestId),
                        ExternalRoutingGroupId
                    );
          if(!OtherRoutingGroupIsStillEnabled)
          {
            /* !LINKSTO SoAd.dsn.Func0130.Rx.Udp.If.DisableServiceInstance, 1 */
            (*SocketRouteDestIdPtr) = SOAD_INVALID_SOCKET_ROUTE_DEST_ID;
          }
        }
      }
    }
  }
  else/* global Routing Group */
  {
    SoAd_SocketRouteDestIdType DestMapId;
    P2CONST(SoAd_RoutingGroupSocketRouteDestMapMapType, AUTOMATIC, SOAD_APPL_DATA)
      RoutingGroupSocketRouteDestMapMapPtr =
      &SOAD_CFG(RoutingGroupSocketRouteDestMapMap, ExternalRoutingGroupId);
    const SoAd_SocketRouteDestIdType MaxDestMapId =
      RoutingGroupSocketRouteDestMapMapPtr->SocketRouteDestMapFirstId
      + RoutingGroupSocketRouteDestMapMapPtr->SocketRouteDestMapCnt;
    for(DestMapId = RoutingGroupSocketRouteDestMapMapPtr->SocketRouteDestMapFirstId;
        DestMapId < MaxDestMapId;
        DestMapId++
       )
    {
      SoAd_SocketRouteDestIdType DestId =
          SOAD_CFG_TYPE(SoAd_SocketRouteDestIdType,RoutingGroupSocketRouteDestMap, DestMapId);
      boolean OtherRoutingGroupIsStillEnabled = SoAd_OneOtherInternalRGActiveForSocketRouteDest
          (
            &SOAD_CFG(SocketRouteDest, DestId),
            ExternalRoutingGroupId
          );
      if(!OtherRoutingGroupIsStillEnabled)
      {
        SoAd_ClearBitIn32BitMap(DestId, &SoAd_PBcfg->SocketRouteDestEnabledBitMapBase);
      }
    }
  }
}

STATIC FUNC(boolean, SOAD_CODE) SoAd_OneOtherInternalRGActiveForSocketRouteDest
(
    P2CONST(SoAd_SocketRouteDestType, AUTOMATIC, SOAD_APPL_DATA) SocketRouteDestPtr,
    SoAd_RoutingGroupIdType ExternalRoutingGroupId
)
{
  /* Check that no other Referenced RoutingGroup is still active */
  boolean OtherRoutingGroupIsStillEnabled = FALSE;
  SoAd_RoutingGroupMapIdType CurrentRGMapId;
  const SoAd_RoutingGroupMapIdType MaxRGMapId = SocketRouteDestPtr->RoutingGroupMapFirstId
                                                + SocketRouteDestPtr->RoutingGroupMapCnt;
  for(CurrentRGMapId = SocketRouteDestPtr->RoutingGroupMapFirstId;
      CurrentRGMapId < MaxRGMapId;
      CurrentRGMapId++
     )
  {
    SoAd_RoutingGroupIdType RoutingGroupExtFromRGMapId =
      SOAD_CFG_TYPE(SoAd_RoutingGroupIdType, SocketRouteRoutingGroupMap, CurrentRGMapId);
    CONSTP2CONST(SoAd_RoutingGroupExtToIntIdMapType, AUTOMATIC, SOAD_APPL_DATA)
      RoutingGroupExtToIntIdMapEntryPtr = &SOAD_CFG(RoutingGroupExtToIntIdMap,
                                                    RoutingGroupExtFromRGMapId
                                                   );
    SoAd_RoutingGroupIdType RoutingGroupIntId = RoutingGroupExtToIntIdMapEntryPtr->RoutingGroupId;
    if(RoutingGroupExtFromRGMapId != ExternalRoutingGroupId
       && SOAD_IS_ROUTINGGROUP_ENABLED(RoutingGroupIntId) == TRUE
      )
    {
      OtherRoutingGroupIsStillEnabled = TRUE;
      break;
    }
  }
  return OtherRoutingGroupIsStillEnabled;
}

STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_SetRemoteAddress
(
  SoAd_SoConIdType SoConId,
  CONSTP2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) IpAddrPtr,
  CONSTP2VAR(boolean, AUTOMATIC, SOAD_APPL_DATA) IsDisconnect,
  CONSTP2VAR(boolean, AUTOMATIC, SOAD_APPL_DATA) IsConnected
)
{
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr =
    &SOAD_CFG(SoCon, SoConId);
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr =
    &SOAD_PBRAM(SoConState, SoConId);

  Std_ReturnType RetVal = E_NOT_OK;
  boolean ActiveTPSession;
  /* there is an active TP session if
   * - either received TP data is buffered or
   * - a TP transmission is pending
   */
  ActiveTPSession = SOAD_FLAG_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RX_DATA_BUFFERED)
                    || SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_TP_PENDING);

  /* if socket connection is open, the PduHeaderMode is disabled
   * and there's a socket route configured */
  if( (SoConStatePtr->SocketState == SOAD_SOCKETSTATE_ONLINE) &&
      (
        (!SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_PDU_HEADER_MODE))
        && (SoConPtr->SocketRouteFirstId != SOAD_INVALID_ROUTE_ID)
      )
    )
  {
    CONSTP2CONST(SoAd_SocketRouteType, AUTOMATIC, SOAD_APPL_DATA) SocketRoutePtr =
        &SOAD_CFG(SocketRoute, SoConPtr->SocketRouteFirstId);

    /* there is also an active TP session if the upper layer uses the TP-API */
    ActiveTPSession = ActiveTPSession || SOAD_FLAG(SocketRoutePtr, SOAD_SOCKETROUTE_FLAG_IS_UL_TP);
  }

  /* remote address can be changed if the socket connection is closed
   * or it is a UDP socket connection without an active TP session */
  if( (SoConStatePtr->SocketState == SOAD_SOCKETSTATE_CLOSED)
      || ( (!SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_TCP))
           && (!ActiveTPSession)
         )
    )
  {
    boolean RemoteIsWildcard;
    /* store remote address and port for the socket connection */
    SoAd_AssignRemoteAddressSockAddrType2SoCon(SoConPtr, SoConStatePtr, IpAddrPtr);

    /* set the remote address set flag */
    SOAD_FLAG_SET_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_REMOTE_SET);

    /* Set global flag to know that for a certain SoCon a transition condition was met. */
    SoAd_SetBitInTransCondChange(SoConPtr);

    RemoteIsWildcard
      = SOAD_IS_REMOTE_WILDCARD(SoConStatePtr->RemoteAddress, SoConStatePtr->RemotePort);

    if(!RemoteIsWildcard)
    {
      SOAD_FLAG_SET_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_USED_SETREMOTEADDR);
    }

    /* Verify the preconditions for UDP to set the state machine to connected or disconnected. */
    if(!SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_TCP))
    {
      if(
          SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_UDP_LISTEN_ONLY)
          || ((!RemoteIsWildcard))
        )
      {
        *IsConnected = TRUE;
      }
      else
      {
        *IsDisconnect = TRUE;
      }
    }

   /* the request was successful */
   RetVal = E_OK;
  }

  return RetVal;
}


STATIC FUNC(void, SOAD_CODE) SoAd_ULLocalIpAddrAssignmentChg
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  TcpIp_IpAddrStateType State
)
{
  /* each bit in ULNotifBitmap represents an upper layer that shall be notified if the bit is set */
  uint32 ULNotifBits = SoConPtr->ULNotifBitmap;
  uint16_least Idx;

  DBG_SOAD_ULLOCALIPADDRASSIGNMENTCHG_ENTRY(SoConPtr, State);

  /* loop over all available upper layer modules as long as notifications are outstanding */
  for(Idx = 0U; (Idx < SoAd_NotificationAPISize) && (ULNotifBits!=0U); Idx++)
  {
    /* if upper layer shall be notified and
     * a valid function pointer exists ... */
    if(
        ((ULNotifBits & 1U) == 1U)
        && (SoAd_NotificationAPI[Idx].LocalIpAddrAssignmentChg != NULL_PTR)
      )
    {
      /* ... call upper layer function */
      SoAd_NotificationAPI[Idx].LocalIpAddrAssignmentChg(SoConPtr->SoConId, State);
    }

    /* shift ULNotifBits */
    ULNotifBits >>= 1U;
  }

  DBG_SOAD_ULLOCALIPADDRASSIGNMENTCHG_EXIT(SoConPtr, State);
}

STATIC FUNC(SoAd_SoConIdType, SOAD_CODE) SoAd_ListenSocketToMapId
(
  TcpIp_SocketIdType ListenSocketId
)
{
  SoAd_SoConIdType RetVal = SOAD_INVALID_SOCON_ID;
  SoAd_SoConIdType Idx;
  const SoAd_SoConIdType ListenSocketCapacity = SoAd_PBcfg->ListenSocketCapacity;
  P2VAR(SoAd_ListenSocketSoConMapType, AUTOMATIC, SOAD_APPL_DATA) ListenMapEntryPtr;

  DBG_SOAD_LISTENSOCKETTOMAPID_ENTRY(ListenSocketId);

  for(Idx = 0U; Idx < ListenSocketCapacity; Idx++)
  {
    ListenMapEntryPtr = &SOAD_PBRAM(ListenSocketSoConMap, Idx);
    if(
        (ListenMapEntryPtr->SoConListFirstId != SOAD_INVALID_SOCON_ID)
        && (ListenMapEntryPtr->ListenSocketId == ListenSocketId)
      )
    {
      RetVal = Idx;
      break;
    }
  }

  DBG_SOAD_LISTENSOCKETTOMAPID_EXIT(RetVal, ListenSocketId);
  return RetVal;
}

TS_MOD_PRIV_DEFN FUNC(SoAd_SoConIdType, SOAD_CODE) SoAd_ListenSocketToSoConId
(
  TcpIp_SocketIdType ListenSocketId
)
{
  SoAd_SoConIdType RetVal = SOAD_INVALID_SOCON_ID;
  SoAd_SoConIdType Idx;
  const SoAd_SoConIdType ListenSocketCapacity = SoAd_PBcfg->ListenSocketCapacity;
  P2VAR(SoAd_ListenSocketSoConMapType, AUTOMATIC, SOAD_APPL_DATA) ListenMapEntryPtr;

  DBG_SOAD_LISTENSOCKETTOSOCONID_ENTRY(ListenSocketId);

  for(Idx = 0U; Idx < ListenSocketCapacity; Idx++)
  {
    ListenMapEntryPtr = &SOAD_PBRAM(ListenSocketSoConMap, Idx);
    if(
        (ListenMapEntryPtr->SoConListFirstId != SOAD_INVALID_SOCON_ID)
        && (ListenMapEntryPtr->ListenSocketId == ListenSocketId)
      )
    {
      RetVal = ListenMapEntryPtr->SoConListFirstId;
      break;
    }
  }

  DBG_SOAD_LISTENSOCKETTOSOCONID_EXIT(RetVal, ListenSocketId);
  return RetVal;
}

#define SOAD_STOP_SEC_CODE
#include <SoAd_MemMap.h>

/*==================[version check]=========================================*/

/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef SOAD_VENDOR_ID /* configuration check */
#error SOAD_VENDOR_ID must be defined
#endif

#if (SOAD_VENDOR_ID != 1U) /* vendor check */
#error SOAD_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef SOAD_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error SOAD_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (SOAD_AR_RELEASE_MAJOR_VERSION != 4U)
#error SOAD_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef SOAD_AR_RELEASE_MINOR_VERSION /* configuration check */
#error SOAD_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (SOAD_AR_RELEASE_MINOR_VERSION != 2U )
#error SOAD_AR_RELEASE_MINOR_VERSION wrong (!= 2U)
#endif

#ifndef SOAD_AR_RELEASE_REVISION_VERSION /* configuration check */
#error SOAD_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (SOAD_AR_RELEASE_REVISION_VERSION != 2U )
#error SOAD_AR_RELEASE_REVISION_VERSION wrong (!= 2U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef SOAD_SW_MAJOR_VERSION /* configuration check */
#error SOAD_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (SOAD_SW_MAJOR_VERSION != 1U)
#error SOAD_SW_MAJOR_VERSION wrong (!= 1U)
#endif

#ifndef SOAD_SW_MINOR_VERSION /* configuration check */
#error SOAD_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (SOAD_SW_MINOR_VERSION < 8U)
#error SOAD_SW_MINOR_VERSION wrong (< 8U)
#endif

#ifndef SOAD_SW_PATCH_VERSION /* configuration check */
#error SOAD_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (SOAD_SW_PATCH_VERSION < 16U)
#error SOAD_SW_PATCH_VERSION wrong (< 16U)
#endif
/*==================[end of file]===========================================*/
