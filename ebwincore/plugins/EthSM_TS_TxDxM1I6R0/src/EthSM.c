/**
 * \file
 *
 * \brief AUTOSAR EthSM
 *
 * This file contains the implementation of the AUTOSAR
 * module EthSM.
 *
 * \version 1.6.14
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */


/*
 *  Misra-C:2012 Deviations:
 *
 *  MISRAC2012-1) Deviated Rule: 8.3 (required)
 *  All declarations of an object or function shall use the same names
 *  and type qualifiers.
 *
 *  Reason:
 *   - Function prototype of generated file .arxml does not use the same
 *     typedef for the datatypes.
 *
 *
 *  MISRAC2012-2) Deviated Rule 20.10 (advisory)
 *  The # and ## preprocessor operators should not be used.
 *
 *  Reason:
 *   - Usage of concatenation operator improves readability of code.
 *
 */

/*==================[inclusions]============================================*/
#ifndef ETHSM_NO_CFGCLASSMIX_REQUIRED
#define ETHSM_NO_CFGCLASSMIX_REQUIRED
#endif

#include <EthSM_Trace.h>
#include <SchM_EthSM.h>
#include <TSAutosar.h>            /* EB specific standard types */
#include <TSAtomic_Assign.h>      /* Atomic assignment macros */
#include <EthSM.h>                /* Module public API */
#include <EthSM_Cbk.h>            /* Module public API */
#include <TSPBConfig_Signature.h> /* relocatable post-build macros */

#if (ETHSM_PBCFGM_SUPPORT_ENABLED == STD_ON)
#define PBCFGM_NO_PBCFG_REQUIRED
#include <PbcfgM.h>                /* Post build configuration manager API */
#endif /* ETHSM_PBCFGM_SUPPORT_ENABLED */

#if (ETHSM_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>                   /* Det API */
#endif

#if (ETHSM_PROD_ERR_HANDLING_CTRL_TEST == TS_PROD_ERR_REP_TO_DEM)
#include <Dem.h>                   /* Dem API */
#endif

#include <EthIf.h>                 /* EthIf interface header */
#include <ComM_BusSm.h>            /* ComM interface header */
#include <BswM_EthSM.h>            /* BswM interface header */
#include <SchM_EthSM.h>            /* SchM interface header */
#include <TcpIp.h>                 /* TcpIp interface header */

#if (ETHSM_DEVAUTH_SUPPORT_ENABLE == STD_ON)
#include <DevAuth_Cbk.h>           /* DevAuth callback declarations header file */
#endif

#define TS_RELOCATABLE_CFG_ENABLE    ETHSM_RELOCATABLE_CFG_ENABLE
#include <TSPBConfig_Access.h>     /* Access to postbuild config */

/*==================[version check]=========================================*/

/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef ETHSM_VENDOR_ID /* configuration check */
#error ETHSM_VENDOR_ID must be defined
#endif

#if (ETHSM_VENDOR_ID != 1U) /* vendor check */
#error ETHSM_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef ETHSM_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error ETHSM_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (ETHSM_AR_RELEASE_MAJOR_VERSION != 4U)
#error ETHSM_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef ETHSM_AR_RELEASE_MINOR_VERSION /* configuration check */
#error ETHSM_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (ETHSM_AR_RELEASE_MINOR_VERSION != 3U )
#error ETHSM_AR_RELEASE_MINOR_VERSION wrong (!= 3U)
#endif

#ifndef ETHSM_AR_RELEASE_REVISION_VERSION /* configuration check */
#error ETHSM_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (ETHSM_AR_RELEASE_REVISION_VERSION != 0U )
#error ETHSM_AR_RELEASE_REVISION_VERSION wrong (!= 0U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef ETHSM_SW_MAJOR_VERSION /* configuration check */
#error ETHSM_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (ETHSM_SW_MAJOR_VERSION != 1U)
#error ETHSM_SW_MAJOR_VERSION wrong (!= 1U)
#endif

#ifndef ETHSM_SW_MINOR_VERSION /* configuration check */
#error ETHSM_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (ETHSM_SW_MINOR_VERSION < 6U)
#error ETHSM_SW_MINOR_VERSION wrong (< 6U)
#endif

#ifndef ETHSM_SW_PATCH_VERSION /* configuration check */
#error ETHSM_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (ETHSM_SW_PATCH_VERSION < 14U)
#error ETHSM_SW_PATCH_VERSION wrong (< 14U)
#endif

/*==================[macros]================================================*/

#if (defined ETHSM_DET_REPORT_ERROR)
#error  ETHSM_DET_REPORT_ERROR is already defined
#endif
/** \brief Macro to report a Det error provided only with the ApiId and ErrorId */
#define ETHSM_DET_REPORT_ERROR(ApiId, ErrorId)    \
    ((void)Det_ReportError(ETHSM_MODULE_ID, ETHSM_INSTANCE_ID, (ApiId), (ErrorId)))


#if ETHSM_SINGLE_NETWORK_OPT_ENABLE == STD_OFF

/* map network handle to internal network index */
#if (defined ETHSM_LOOKUPNETWORK_IDX_FROM_NETWORK_HANDLE)
#error  ETHSM_LOOKUPNETWORK_IDX_FROM_NETWORK_HANDLE is already defined
#endif
#define ETHSM_LOOKUPNETWORK_IDX_FROM_NETWORK_HANDLE(handle) \
    EthSM_LookUpNetworkIdxFromNetworkHandle(handle)

#if (defined ETHSM_LOOKUPNETWORK_IDX_FROM_CTRL_IDX)
#error  ETHSM_LOOKUPNETWORK_IDX_FROM_CTRL_IDX is already defined
#endif
#define ETHSM_LOOKUPNETWORK_IDX_FROM_CTRL_IDX(handle) EthSM_LookUpNetworkIdxFromCtrlIdx(handle)

#else /* ETHSM_SINGLE_NETWORK_OPT_ENABLE */

#if (defined ETHSM_LOOKUPNETWORK_IDX_FROM_NETWORK_HANDLE)
#error  ETHSM_LOOKUPNETWORK_IDX_FROM_NETWORK_HANDLE is already defined
#endif
#define ETHSM_LOOKUPNETWORK_IDX_FROM_NETWORK_HANDLE(handle) 0U

#if (defined ETHSM_LOOKUPNETWORK_IDX_FROM_CTRL_IDX)
#error  ETHSM_LOOKUPNETWORK_IDX_FROM_CTRL_IDX is already defined
#endif
#define ETHSM_LOOKUPNETWORK_IDX_FROM_CTRL_IDX(handle) 0U

#endif /* ETHSM_SINGLE_NETWORK_OPT_ENABLE */


/** \brief Access Post-Build ROM config
 **
 ** This macro accesses an element (struct) in the Post-build config
 ** by using the given references.
 **
 ** \param[in] type     Type of the element (member) to access
 ** \param[in] name     Name of the element (member) to access
 ** \param[in] element  Element (member) in the config to access
 */
#if (defined ETHSM_PBCFG_ACCESS)
#error ETHSM_PBCFG_ACCESS is already defined
#endif
/* Deviation MISRAC2012-2 */
#define ETHSM_PBCFG_ACCESS(type, name, element) \
    (TS_UNCHECKEDGETCFG(EthSM_RootPtr, type, ETHSM, (EthSM_RootPtr->name ## Ref))[(element)])

/** \brief Returns CtrlIdx of a specific Network in Post-Build ROM config
 */
#if (defined ETHSM_GET_NETWORK_CTRLIDX)
#error ETHSM_GET_NETWORK_CTRLIDX is already defined
#endif
#define ETHSM_GET_NETWORK_CTRLIDX(NetworkIdx)  \
  ETHSM_PBCFG_ACCESS(EthSM_NetworkType, Network, NetworkIdx).CtrlIdx

/** \brief Returns NetworkHandleIdx of a specific Network in Post-Build ROM config
 */
#if (defined ETHSM_GET_NETWORK_HANDLEIDX)
#error ETHSM_GET_NETWORK_HANDLEIDX is already defined
#endif
#define ETHSM_GET_NETWORK_HANDLEIDX(NetworkIdx)  \
  ETHSM_PBCFG_ACCESS(EthSM_NetworkType, Network, NetworkIdx).NetworkHandleIdx

/** \brief Returns DevAuthNotifEnable of a specific Network in Post-Build ROM config
 */
#if (defined ETHSM_GET_NETWORK_DEVAUTHNOTIFENABLE)
#error ETHSM_GET_NETWORK_DEVAUTHNOTIFENABLE is already defined
#endif
#define ETHSM_GET_NETWORK_DEVAUTHNOTIFENABLE(NetworkIdx)  \
  ETHSM_PBCFG_ACCESS(EthSM_NetworkType, Network, NetworkIdx).DevAuthNotifEnable

#if (ETHSM_PROD_ERR_HANDLING_CTRL_TEST == TS_PROD_ERR_REP_TO_DEM)
/** \brief Returns DemEventIdx of a specific Network in Post-Build ROM config
 */
#if (defined ETHSM_GET_NETWORK_DEMEVENTIDX)
#error ETHSM_GET_NETWORK_DEMEVENTIDX is already defined
#endif
#define ETHSM_GET_NETWORK_DEMEVENTIDX(NetworkIdx)  \
  ETHSM_PBCFG_ACCESS(EthSM_NetworkType, Network, NetworkIdx).DemEventIdx
#endif

/*==================[type definitions]======================================*/

/** \brief Type of EthSM network */
typedef struct
{
  VAR(EthSM_NetworkModeStateType, TYPEDEF) EthSMState; /* State machine state of the network. */
  VAR(ComM_ModeType, TYPEDEF) ComMMode; /* Actual ComM mode. */
  VAR(TcpIp_StateType, TYPEDEF) TcpIpState; /* Actual TcpIp state. */
  VAR(EthTrcv_LinkStateType, TYPEDEF) TrcvLinkState; /* Actual Trcv link state. */
  VAR(Eth_ModeType, TYPEDEF) CtrlMode; /* Actual Controler mode. */
  VAR(boolean, TYPEDEF) TcpIpModeIndFlag; /* Indication flag if corresponding API was called. */
  VAR(boolean, TYPEDEF) RequestComModeFlag; /* Indication flag if corresponding API was called. */
  VAR(boolean, TYPEDEF) TrcvLinkStateChgFlag; /* Indication flag if corresponding API was called. */
  VAR(boolean, TYPEDEF) CtrlModeChgFlag; /* Indication flag if corresponding API was called. */
#if (ETHSM_DEVAUTH_SUPPORT_ENABLE == STD_ON)
  VAR(boolean, TYPEDEF) DevAuthNoComFlag; /* Indication flag if corresponding API was called. */
#endif
} EthSM_IntNetworkType;
/*==================[external function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/
#define ETHSM_START_SEC_VAR_INIT_8
#include <EthSM_MemMap.h>

STATIC VAR(boolean, ETHSM_VAR) EthSM_Initialized = FALSE;

#define ETHSM_STOP_SEC_VAR_INIT_8
#include <EthSM_MemMap.h>

#define ETHSM_START_SEC_VAR_CLEARED_8
#include <EthSM_MemMap.h>

/* typedef uint8 ComM_ModeType */
STATIC VAR(ComM_ModeType, ETHSM_VAR) EthSM_NoCommunication;
STATIC VAR(ComM_ModeType, ETHSM_VAR) EthSM_FullCommunication;

#define ETHSM_STOP_SEC_VAR_CLEARED_8
#include <EthSM_MemMap.h>

#define ETHSM_START_SEC_VAR_INIT_UNSPECIFIED
#include <EthSM_MemMap.h>

STATIC P2CONST(EthSM_ConfigType, ETHSM_VAR, ETHSM_APPL_CONST) EthSM_RootPtr = NULL_PTR;

#define ETHSM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <EthSM_MemMap.h>

#define ETHSM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <EthSM_MemMap.h>

STATIC VAR(EthSM_IntNetworkType, ETHSM_VAR) EthSM_Network[ETHSM_NETWORKS_ARRAYSIZE];

#define ETHSM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <EthSM_MemMap.h>
/*==================[internal function declarations]========================*/

#define ETHSM_START_SEC_CODE
#include <EthSM_MemMap.h>

/** \brief Internal functions to perform state machine transitions.
 *
 * \param[in] NetworkIdx - Network index within the EthSM configuration.
 **/
STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_Offline_To_WaitCtrlModeInd
(
  uint8 NetworkIdx
);

STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_WaitCtrlModeInd_To_WaitTrcvLink
(
  uint8 NetworkIdx
);

STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_WaitTrcvLink_To_WaitCtrlMIndDown
(
  uint8 NetworkIdx
);

STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_WaitCtrlMIndDown_To_Offline
(
  uint8 NetworkIdx
);

STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_WaitTrcvLink_To_WaitOnline
(
  uint8 NetworkIdx
);
STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_WaitOnline_To_WaitTrcvLink
(
  uint8 NetworkIdx
);
STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_WaitOnline_To_Online
(
  uint8 NetworkIdx
);
STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_WaitOnline_To_WaitCtrlMindDown
(
  uint8 NetworkIdx
);
STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_Online_To_WaitOffline
(
  uint8 NetworkIdx
);
STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_Online_To_OnHold
(
  uint8 NetworkIdx
);
STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_Online_To_WaitOnline
(
  uint8 NetworkIdx
);
STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_OnHold_To_Online
(
  uint8 NetworkIdx
);
STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_OnHold_To_WaitTrcvLink
(
  uint8 NetworkIdx
);
STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_WaitLinkDown_To_Offline
(
  uint8 NetworkIdx
);

#if (ETHSM_DEVAUTH_SUPPORT_ENABLE == STD_ON)
STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_WaitOffline_To_WaitDevAuthOffline
(
  uint8 NetworkIdx
);

STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_OnHold_To_WaitDevAuthOffline
(
  uint8 NetworkIdx
);

STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_WaitDevAuthOffline_To_WaitLinkDown
(
  uint8 NetworkIdx
);
#else
STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_OnHold_To_WaitLinkDown
(
  uint8 NetworkIdx
);
STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_WaitOffline_To_WaitLinkDown
(
  uint8 NetworkIdx
);
#endif /* ETHSM_DEVAUTH_SUPPORT_ENABLE == STD_ON */

/** \brief Internal function to process TcpIp mode indication in main function.
 *
 * \param[in] NetworkIdx - Network index within the EthSM configuration.
 * \param[in] EthSMState - Actual state of the state machine.
 * \param[in] TcpIpState - TcpIp state provided from the mode indication.
 **/
STATIC FUNC(void, ETHSM_CODE) EthSM_HandleTcpIpModeIndication
(
  uint8 NetworkIdx,
  EthSM_NetworkModeStateType EthSMState,
  TcpIp_StateType TcpIpState
);

/** \brief Internal function to process ComM mode requests in main function.
 *
 * \param[in] NetworkIdx - Network index within the EthSM configuration.
 * \param[in] EthSMState - Actual state of the state machine.
 * \param[in] ComMMode - Requested communication mode.
 *
 * \return TRUE: Restore flag (No state transition performed).
 *         FALSE: Do not restore flag.
 **/
STATIC FUNC(boolean, ETHSM_CODE) EthSM_HandleComMModeRequest
(
  uint8 NetworkIdx,
  EthSM_NetworkModeStateType EthSMState,
  ComM_ModeType ComMMode
);

/** \brief Internal function to process transceiver link state changes in main function.
 *
 * \param[in] NetworkIdx - Network index within the EthSM configuration.
 * \param[in] EthSMState - Actual state of the state machine.
 * \param[in] TrcvLinkState - Reported transceiver link state.
 *
 **/
STATIC FUNC(void, ETHSM_CODE) EthSM_HandleTrcvLinkStateChg
(
  uint8 NetworkIdx,
  EthSM_NetworkModeStateType EthSMState,
  EthTrcv_LinkStateType TrcvLinkState
);

/** \brief Internal function to process controler mode state changes in main function.
 *
 * \param[in] NetworkIdx - Network index within the EthSM configuration.
 * \param[in] EthSMState - Actual state of the state machine.
 * \param[in] CtrlMode - Reported controler mode.
 *
 **/
STATIC FUNC(void, ETHSM_CODE) EthSM_HandleCtrlModeStateChg
(
  uint8 NetworkIdx,
  EthSM_NetworkModeStateType EthSMState,
  Eth_ModeType CtrlMode
);

#if (ETHSM_DEVAUTH_SUPPORT_ENABLE == STD_ON)
/** \brief Internal function to process DevAuth state changes in main function.
 *
 * \param[in] NetworkIdx - Network index within the EthSM configuration.
 * \param[in] EthSMState - Actual state of the state machine.
 *
 **/
STATIC FUNC(void, ETHSM_CODE) EthSM_HandleDevAuthNoComInd
(
  uint8 NetworkIdx,
  EthSM_NetworkModeStateType EthSMState
);

#endif /* ETHSM_DEVAUTH_SUPPORT_ENABLE == STD_ON */

#if ETHSM_SINGLE_NETWORK_OPT_ENABLE == STD_OFF

/** \brief Internal function retrieve NetworkIdx from NetworkHandle.
 *
 * \param[in] NetworkHandle - Upper layer network handle ID.
 *
 * \return Network index within EthSM configuration.
 **/
STATIC FUNC(uint8, ETHSM_CODE) EthSM_LookUpNetworkIdxFromNetworkHandle(uint8 NetworkHandle);

/** \brief Internal function retrieve NetworkIdx from CtrlIdx.
 *
 * \param[in] CtrlIdx - Lower layer controller index.
 *
 * \return Network index within EthSM configuration.
 **/
STATIC FUNC(uint8, ETHSM_CODE) EthSM_LookUpNetworkIdxFromCtrlIdx(uint8 CtrlIdx);

#endif /* ETHSM_SINGLE_NETWORK_OPT_ENABLE */

#define ETHSM_STOP_SEC_CODE
#include <EthSM_MemMap.h>

/*==================[external function definitions]=========================*/

#if ETHSM_ISVALIDCONFIG_MMAP_ENABLED == STD_ON
#define ETHSM_START_SEC_CODE_CC_BLOCK
#else
#define ETHSM_START_SEC_CODE
#endif
#include <EthSM_MemMap.h>

FUNC(Std_ReturnType, ETHSM_CODE) EthSM_IsValidConfig
(
  P2CONST(void, AUTOMATIC, ETHSM_APPL_CONST) voidConfigPtr
)
{
  /* Assume an invalid configuration. */
  Std_ReturnType RetVal = E_NOT_OK;
  P2CONST(EthSM_ConfigType, AUTOMATIC, ETHSM_APPL_CONST) ConfigPtr = voidConfigPtr;

  DBG_ETHSM_ISVALIDCONFIG_ENTRY(voidConfigPtr);

  if(ConfigPtr != NULL_PTR)
  {
    /* Check if the configuration fits to the platform. */
    if(TS_PlatformSigIsValid(ConfigPtr->PlatformSignature) == TRUE)
    {
      /* Validate the post build configuration against the compile time configuration. */
      if(ETHSM_CFG_SIGNATURE == ConfigPtr->CfgSignature)
      {
        /* Validate the post build configuration against the link time configuration. */
        if(EthSM_LcfgSignature == ConfigPtr->LcfgSignature)
        {
          /* Validate the post build configuration against the compile time published information
             configuration. */
          if(ETHSM_PUBLIC_INFO_SIGNATURE == ConfigPtr->PublicInfoSignature)
          {
            /* Indicate that the configuration is valid. */
            RetVal = E_OK;
          }
        }
      }
    }
  }

  DBG_ETHSM_ISVALIDCONFIG_EXIT(RetVal, voidConfigPtr);
  return RetVal;
}

#if ETHSM_ISVALIDCONFIG_MMAP_ENABLED == STD_ON
#define ETHSM_STOP_SEC_CODE_CC_BLOCK
#include <EthSM_MemMap.h>

#define ETHSM_START_SEC_CODE
#include <EthSM_MemMap.h>
#endif

FUNC(Std_ReturnType, ETHSM_CODE) EthSM_Init
(
  P2CONST(EthSM_ConfigType, AUTOMATIC, ETHSM_APPL_CONST) ConfigPtr
)
{
  P2CONST(EthSM_ConfigType, AUTOMATIC, ETHSM_APPL_CONST) LocalConfigPtr = ConfigPtr;
  Std_ReturnType Result = E_NOT_OK;

  DBG_ETHSM_INIT_ENTRY(ConfigPtr);

  /* Save the configuration pointer */
#if (ETHSM_PBCFGM_SUPPORT_ENABLED == STD_ON)
  /* If the initialization function is called with a null pointer get the configuration from the
   * post build configuration manager. */
  if(LocalConfigPtr == NULL_PTR)
  {
      PbcfgM_ModuleConfigPtrType ModuleConfig = NULL_PTR;
      if(E_OK == PbcfgM_GetConfig(
                                   ETHSM_MODULE_ID,
                                   ETHSM_INSTANCE_ID,
                                   &ModuleConfig)
                                 )
      {
        LocalConfigPtr = (P2CONST(EthSM_ConfigType, AUTOMATIC, ETHSM_APPL_CONST)) ModuleConfig;
      }
  }
#endif /* ETHSM_PBCFGM_SUPPORT_ENABLED == STD_OFF */

#if (ETHSM_DEV_ERROR_DETECT == STD_ON)
  if (LocalConfigPtr == NULL_PTR)
  {
    ETHSM_DET_REPORT_ERROR(ETHSM_INIT_SVCID, ETHSM_E_PARAM_POINTER);
  }
  /* check that configuration pointer is valid */
  else if(E_OK != EthSM_IsValidConfig(LocalConfigPtr))
  {
    ETHSM_DET_REPORT_ERROR(ETHSM_INIT_SVCID, ETHSM_E_PARAM_POINTER);
  }
  else
#endif
  {
    /* iterator variable */
    uint8_least i;

    SchM_Enter_EthSM_SCHM_ETHSM_EXCLUSIVE_AREA_0();

    /* Reset init flag in case that reinitialization is required and locking mechanism is
       disabled. */

    for(i = 0U; i < ETHSM_NETWORKS_ARRAYSIZE; i++)
    {
      /* initialize internal state variable */
      EthSM_Network[i].EthSMState = ETHSM_STATE_OFFLINE;
      /* initialize ComM mode */
      EthSM_Network[i].TcpIpState = TCPIP_STATE_OFFLINE;
      /* initialize ComM mode */
      EthSM_Network[i].ComMMode = COMM_NO_COMMUNICATION;
      /* initialize transceiver link state */
      EthSM_Network[i].TrcvLinkState = ETHTRCV_LINK_STATE_DOWN;
      /* initialize controler mode state */
      EthSM_Network[i].CtrlMode = ETH_MODE_DOWN;
      /* initialize mode indication flag */
      EthSM_Network[i].TcpIpModeIndFlag = FALSE;
      /* initialize ComM mode request flag */
      EthSM_Network[i].RequestComModeFlag = FALSE;
      /* initialize Trcv link state change flag */
      EthSM_Network[i].TrcvLinkStateChgFlag = FALSE;
      /* initialize Ctrl mode state change flag */
      EthSM_Network[i].CtrlModeChgFlag = FALSE;
#if (ETHSM_DEVAUTH_SUPPORT_ENABLE == STD_ON)
      /* initialize DevAuth NoCom flag */
      EthSM_Network[i].DevAuthNoComFlag = FALSE;
#endif
    }
    EthSM_NoCommunication = COMM_NO_COMMUNICATION;
    EthSM_FullCommunication = COMM_FULL_COMMUNICATION;
    /* store the post-build pointer */
    EthSM_RootPtr = LocalConfigPtr;
    SchM_Exit_EthSM_SCHM_ETHSM_EXCLUSIVE_AREA_0();

    /* module is finally initialized */
    TS_AtomicAssign8(EthSM_Initialized, TRUE);

    /* function executed successfully */
    Result = E_OK;
  }

  DBG_ETHSM_INIT_EXIT(Result,ConfigPtr);
  return Result;
}

#if (ETHSM_VERSION_INFO_API == STD_ON)
FUNC(void, ETHSM_CODE) EthSM_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, ETHSM_APPL_DATA) versioninfo
)
{
  DBG_ETHSM_GETVERSIONINFO_ENTRY(versioninfo);

#if (ETHSM_DEV_ERROR_DETECT == STD_ON)
  /* argument null pointer check */
  if (versioninfo == NULL_PTR)
  {
    ETHSM_DET_REPORT_ERROR(ETHSM_GETVERSIONINFO_SVCID, ETHSM_E_PARAM_POINTER);
  }
  else
#endif
  {
    /* write version information */
    versioninfo->vendorID         = ETHSM_VENDOR_ID;
    versioninfo->moduleID         = ETHSM_MODULE_ID;
    versioninfo->sw_major_version = ETHSM_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = ETHSM_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = ETHSM_SW_PATCH_VERSION;
  }

  DBG_ETHSM_GETVERSIONINFO_EXIT(versioninfo);
}
#endif

/* Deviation MISRAC2012-1 */
FUNC(Std_ReturnType, ETHSM_CODE) EthSM_RequestComMode
(
  NetworkHandleType NetworkHandle,
  ComM_ModeType ComM_Mode
)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_ETHSM_REQUESTCOMMODE_ENTRY(NetworkHandle, ComM_Mode);

#if (ETHSM_DEV_ERROR_DETECT == STD_ON)
  /* initialization check */
  if (EthSM_Initialized != TRUE)
  {
    ETHSM_DET_REPORT_ERROR(ETHSM_REQUESTCOMMODE_SVCID, ETHSM_E_UNINIT);
  }
  else
#endif /* ETHSM_DEV_ERROR_DETECT */
  {
    /* lookup internal index from NetworkHandle */
    const uint8 NetworkIdx = ETHSM_LOOKUPNETWORK_IDX_FROM_NETWORK_HANDLE(NetworkHandle);

#if ETHSM_SINGLE_NETWORK_OPT_ENABLE == STD_ON
    TS_PARAM_UNUSED(NetworkHandle);
#endif /* ETHSM_SINGLE_NETWORK_OPT_ENABLE */

#if (ETHSM_DEV_ERROR_DETECT == STD_ON)
    /* check if lookup was successful - handle is valid */
    if(NetworkIdx >= EthSM_RootPtr->NumNetworks)
    {
      ETHSM_DET_REPORT_ERROR(ETHSM_REQUESTCOMMODE_SVCID, ETHSM_E_INVALID_NETWORK_HANDLE);
    }
    else
#endif /* ETHSM_DEV_ERROR_DETECT */
    {
      switch(ComM_Mode)
      {
        case COMM_FULL_COMMUNICATION: /* fall through */
        case COMM_NO_COMMUNICATION:
        {
          /* Updating the requested ComM mode */
          SchM_Enter_EthSM_SCHM_ETHSM_EXCLUSIVE_AREA_0();
          EthSM_Network[NetworkIdx].ComMMode = ComM_Mode;
          TS_AtomicAssign8(EthSM_Network[NetworkIdx].RequestComModeFlag, TRUE);
          SchM_Exit_EthSM_SCHM_ETHSM_EXCLUSIVE_AREA_0();

          Result = E_OK;
          break;
        }
        case COMM_SILENT_COMMUNICATION:
        {
          /* Accept request, but no action shall be performed */
          Result = E_OK;
          break;
        }
        default:
        {
#if (ETHSM_DEV_ERROR_DETECT == STD_ON)
          ETHSM_DET_REPORT_ERROR(ETHSM_REQUESTCOMMODE_SVCID, ETHSM_E_INVALID_NETWORK_MODE);
#endif /* ETHSM_DEV_ERROR_DETECT */
          break;
        }
      }
    }
  }

  DBG_ETHSM_REQUESTCOMMODE_EXIT(Result, NetworkHandle, ComM_Mode);
  return Result;
}

/* Deviation MISRAC2012-1 */
FUNC(Std_ReturnType, ETHSM_CODE) EthSM_GetCurrentComMode
(
  NetworkHandleType NetworkHandle,
  P2VAR(ComM_ModeType, AUTOMATIC, ETHSM_APPL_DATA) ComM_ModePtr
)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_ETHSM_GETCURRENTCOMMODE_ENTRY(NetworkHandle,ComM_ModePtr);

#if (ETHSM_DEV_ERROR_DETECT == STD_ON)
  /* initialization check */
  if (EthSM_Initialized != TRUE)
  {
    ETHSM_DET_REPORT_ERROR(ETHSM_GETCURRENTCOMMODE_SVCID, ETHSM_E_UNINIT);
  }
  /* argument null pointer check */
  else if (ComM_ModePtr == NULL_PTR)
  {
    ETHSM_DET_REPORT_ERROR(ETHSM_GETCURRENTCOMMODE_SVCID, ETHSM_E_PARAM_POINTER);
  }
  else
#endif /* ETHSM_DEV_ERROR_DETECT */
  {
    /* lookup internal index from NetworkHandle */
    const uint8 NetworkIdx = ETHSM_LOOKUPNETWORK_IDX_FROM_NETWORK_HANDLE(NetworkHandle);

#if ETHSM_SINGLE_NETWORK_OPT_ENABLE == STD_ON
    TS_PARAM_UNUSED(NetworkHandle);
#endif /* ETHSM_SINGLE_NETWORK_OPT_ENABLE */

#if (ETHSM_DEV_ERROR_DETECT == STD_ON)
    /* check if lookup was successful - handle is valid */
    if(NetworkIdx >= EthSM_RootPtr->NumNetworks)
    {
      ETHSM_DET_REPORT_ERROR(ETHSM_GETCURRENTCOMMODE_SVCID, ETHSM_E_INVALID_NETWORK_HANDLE);
    }
    else
#endif /* ETHSM_DEV_ERROR_DETECT */
    {
      EthSM_NetworkModeStateType EthSMState;
      TS_AtomicAssign8(EthSMState, EthSM_Network[NetworkIdx].EthSMState);

      /* return current state */
      switch(EthSMState)
      {
        case ETHSM_STATE_ONLINE:          /* fall through */
        case ETHSM_STATE_ONHOLD:          /* fall through */
        case ETHSM_STATE_WAIT_LINK_DOWN:  /* fall through */
        case ETHSM_STATE_WAIT_OFFLINE:
        {
          *ComM_ModePtr = COMM_FULL_COMMUNICATION;
          Result = E_OK;
          break;
        }
        case ETHSM_STATE_WAIT_CTRLMODEIND: /* fall through */
        case ETHSM_STATE_WAIT_TRCVLINK:    /* fall through */
        case ETHSM_STATE_WAIT_ONLINE:      /* fall through */
        case ETHSM_STATE_WAIT_CTRLMIND_DOWN: /* fall through */
        case ETHSM_STATE_OFFLINE:
        {
          *ComM_ModePtr = COMM_NO_COMMUNICATION;
          Result = E_OK;
          break;
        }
        /* CHECK: NOPARSE */
        default: /* Can not be reached. */
        {
          ETHSM_UNREACHABLE_CODE_ASSERT(ETHSM_MAINFUNCTION_SVCID);
          break;
        }
        /* CHECK: PARSE */
      }
    }
  }

  DBG_ETHSM_GETCURRENTCOMMODE_EXIT(Result, NetworkHandle, ComM_ModePtr);
  return Result;
}

FUNC(void, ETHSM_CODE) EthSM_TcpIpModeIndication(uint8 CtrlIdx, TcpIp_StateType TcpIpState)
{
  DBG_ETHSM_TCPIPMODEINDICATION_ENTRY(CtrlIdx, TcpIpState);

#if (ETHSM_DEV_ERROR_DETECT == STD_ON)
  /* initialization check */
  if (EthSM_Initialized != TRUE)
  {
    ETHSM_DET_REPORT_ERROR(ETHSM_TCPIPMODEINDICATION_SVCID, ETHSM_E_UNINIT);
  }
  else
#endif /* ETHSM_DEV_ERROR_DETECT */
  {
    /* lookup internal index from NetworkHandle */
    const uint8 NetworkIdx = ETHSM_LOOKUPNETWORK_IDX_FROM_CTRL_IDX(CtrlIdx);

#if (ETHSM_SINGLE_NETWORK_OPT_ENABLE == STD_ON)
    TS_PARAM_UNUSED(CtrlIdx);
#endif /* ETHSM_SINGLE_NETWORK_OPT_ENABLE */

#if (ETHSM_DEV_ERROR_DETECT == STD_ON)
    /* check if lookup was successful */
    if(NetworkIdx >= EthSM_RootPtr->NumNetworks)
    {
      ETHSM_DET_REPORT_ERROR(ETHSM_TCPIPMODEINDICATION_SVCID, ETHSM_E_PARAM_CONTROLLER);
    }
    else
#endif /* ETHSM_DEV_ERROR_DETECT */
    {
      switch(TcpIpState)
      {
        case TCPIP_STATE_ONLINE: /* fall through */
        case TCPIP_STATE_OFFLINE:
        {
          /* Setting the Indication flag and the indicated state of each network */
          SchM_Enter_EthSM_SCHM_ETHSM_EXCLUSIVE_AREA_0();
          EthSM_Network[NetworkIdx].TcpIpState =  TcpIpState;
          TS_AtomicAssign8(EthSM_Network[NetworkIdx].TcpIpModeIndFlag, TRUE);
          SchM_Exit_EthSM_SCHM_ETHSM_EXCLUSIVE_AREA_0();
          break;
        }
        case TCPIP_STATE_ONHOLD: /* fall through */
        case TCPIP_STATE_STARTUP: /* fall through */
        case TCPIP_STATE_SHUTDOWN: /* fall through */
        {
          /* Nothing to do for these TCP IP state changes. */
          break;
        }
        default:
        {
#if (ETHSM_DEV_ERROR_DETECT == STD_ON)
          ETHSM_DET_REPORT_ERROR(ETHSM_TCPIPMODEINDICATION_SVCID, ETHSM_E_INVALID_TcpIpMode);
#endif /* ETHSM_DEV_ERROR_DETECT */
          break;
        }
      }
    }
  }

  DBG_ETHSM_TCPIPMODEINDICATION_EXIT(CtrlIdx, TcpIpState);
}

FUNC(void, ETHSM_CODE) EthSM_TrcvLinkStateChg
(
  uint8 CtrlIdx,
  EthTrcv_LinkStateType LinkState
)
{
  DBG_ETHSM_TRCVLINKSTATECHG_ENTRY(CtrlIdx,LinkState);

#if(ETHSM_DEV_ERROR_DETECT == STD_ON)
  /* initialization check */
  if(EthSM_Initialized != TRUE)
  {
    ETHSM_DET_REPORT_ERROR(ETHSM_TRCVLINKSTATECHG_SVCID, ETHSM_E_UNINIT);
  }
  else
#endif /* ETHSM_DEV_ERROR_DETECT */
  {
    /* lookup internal index from NetworkHandle */
    const uint8 NetworkIdx = ETHSM_LOOKUPNETWORK_IDX_FROM_CTRL_IDX(CtrlIdx);

#if(ETHSM_SINGLE_NETWORK_OPT_ENABLE == STD_ON)
    TS_PARAM_UNUSED(CtrlIdx);
#endif /* ETHSM_SINGLE_NETWORK_OPT_ENABLE */

#if(ETHSM_DEV_ERROR_DETECT == STD_ON)
    /* check if lookup was successful */
    if(NetworkIdx >= EthSM_RootPtr->NumNetworks)
    {
      ETHSM_DET_REPORT_ERROR(ETHSM_TRCVLINKSTATECHG_SVCID, ETHSM_E_PARAM_CONTROLLER);
    }
    else
#endif /* ETHSM_DEV_ERROR_DETECT */
    {
      switch(LinkState)
      {
        case ETHTRCV_LINK_STATE_DOWN: /* fall through */
        case ETHTRCV_LINK_STATE_ACTIVE:
        {
          /* Setting the state change */
          SchM_Enter_EthSM_SCHM_ETHSM_EXCLUSIVE_AREA_0();
          EthSM_Network[NetworkIdx].TrcvLinkState = LinkState;
          TS_AtomicAssign8(EthSM_Network[NetworkIdx].TrcvLinkStateChgFlag, TRUE);
          SchM_Exit_EthSM_SCHM_ETHSM_EXCLUSIVE_AREA_0();
          break;
        }
        default:
        {
#if(ETHSM_DEV_ERROR_DETECT == STD_ON)
          ETHSM_DET_REPORT_ERROR(ETHSM_TRCVLINKSTATECHG_SVCID, ETHSM_E_INVALID_TRCV_LINK_STATE);
#endif /* ETHSM_DEV_ERROR_DETECT */
          break;
        }
      }
    }
  }

  DBG_ETHSM_TRCVLINKSTATECHG_EXIT(CtrlIdx,LinkState);
}

FUNC(void, ETHSM_CODE) EthSM_MainFunction
(
  void
)
{
  DBG_ETHSM_MAINFUNCTION_ENTRY();

  /* Proceed only if initialized */
  if(EthSM_Initialized == TRUE)
  {
    uint8 NetworkIdx;
    const uint8 EthSMNetworksMax = EthSM_RootPtr->NumNetworks;

    /* Loop over all configured networks. */
    for(NetworkIdx = 0U; NetworkIdx < EthSMNetworksMax; NetworkIdx++)
    {
      EthTrcv_LinkStateType TrcvLinkState;
      ComM_ModeType ComMMode;
      boolean TcpIpModeIndFlag;
      boolean RequestComModeFlag;
      boolean CtrlModeChgFlag;
      boolean TrcvLinkStateChgFlag;
#if (ETHSM_DEVAUTH_SUPPORT_ENABLE == STD_ON)
      boolean DevAuthNoComFlag;
#endif
      TcpIp_StateType TcpIpState;

      /* Check if ComM mode request occurred. */
      SchM_Enter_EthSM_SCHM_ETHSM_EXCLUSIVE_AREA_0();
      TS_AtomicAssign8(RequestComModeFlag, EthSM_Network[NetworkIdx].RequestComModeFlag);
      TS_AtomicAssign8(EthSM_Network[NetworkIdx].RequestComModeFlag, FALSE); /* Reset flag */
      ComMMode = EthSM_Network[NetworkIdx].ComMMode;
      SchM_Exit_EthSM_SCHM_ETHSM_EXCLUSIVE_AREA_0();

      if(RequestComModeFlag == TRUE)
      {
        /* Process the state transitions which depends on mode requests. */
        const boolean RestoreFlag
          = EthSM_HandleComMModeRequest(NetworkIdx, EthSM_Network[NetworkIdx].EthSMState, ComMMode);

        if(RestoreFlag)
        {
          /* No need for critical section if flag is set again. An interrupt at this point
             will only set the flag which leads to the same result. */
          TS_AtomicAssign8(EthSM_Network[NetworkIdx].RequestComModeFlag, TRUE);
        }
      }

      /* Verify if controler mode indication occurred.
         Note: Shall be processed after RequestComModeFlag in case that controler mode
               indication comes immediately. */
      SchM_Enter_EthSM_SCHM_ETHSM_EXCLUSIVE_AREA_0();
      TS_AtomicAssign8(CtrlModeChgFlag, EthSM_Network[NetworkIdx].CtrlModeChgFlag);
      TS_AtomicAssign8(EthSM_Network[NetworkIdx].CtrlModeChgFlag, FALSE); /* Reset flag */
      TrcvLinkState = EthSM_Network[NetworkIdx].CtrlMode;
      SchM_Exit_EthSM_SCHM_ETHSM_EXCLUSIVE_AREA_0();

      if(CtrlModeChgFlag == TRUE)
      {

        /* Process the state transitions which depend on Ctrl mode state changes. */
        EthSM_HandleCtrlModeStateChg(
                                      NetworkIdx,
                                      EthSM_Network[NetworkIdx].EthSMState,
                                      TrcvLinkState
                                    );
      }

      /* Verify if transceiver link state change occurred. */
      SchM_Enter_EthSM_SCHM_ETHSM_EXCLUSIVE_AREA_0();
      TS_AtomicAssign8(TrcvLinkStateChgFlag, EthSM_Network[NetworkIdx].TrcvLinkStateChgFlag);
      TS_AtomicAssign8(EthSM_Network[NetworkIdx].TrcvLinkStateChgFlag, FALSE); /* Reset flag */
      TrcvLinkState = EthSM_Network[NetworkIdx].TrcvLinkState;
      SchM_Exit_EthSM_SCHM_ETHSM_EXCLUSIVE_AREA_0();

      if(TrcvLinkStateChgFlag == TRUE)
      {

        /* Process the state transitions which depends on Trcv state changes. */
        EthSM_HandleTrcvLinkStateChg(
                                      NetworkIdx,
                                      EthSM_Network[NetworkIdx].EthSMState,
                                      TrcvLinkState
                                    );
      }

      /* Verify if TcpIp mode indication occurred.
         Note: Shall be processed after TrcvLinkStateChgFlag in case the indication comes
               immediately. */
      SchM_Enter_EthSM_SCHM_ETHSM_EXCLUSIVE_AREA_0();
      TS_AtomicAssign8(TcpIpModeIndFlag, EthSM_Network[NetworkIdx].TcpIpModeIndFlag);
      TS_AtomicAssign8(EthSM_Network[NetworkIdx].TcpIpModeIndFlag, FALSE); /* Reset flag */
      TS_AtomicAssign8(TcpIpState, EthSM_Network[NetworkIdx].TcpIpState);
      SchM_Exit_EthSM_SCHM_ETHSM_EXCLUSIVE_AREA_0();

      if(TcpIpModeIndFlag == TRUE)
      {
        /* See protection mechanism above. */

        /* Process the state transitions which depends on TcpIp mode indications. */
        EthSM_HandleTcpIpModeIndication(
                                         NetworkIdx,
                                         EthSM_Network[NetworkIdx].EthSMState,
                                         TcpIpState
                                       );
      }

#if (ETHSM_DEVAUTH_SUPPORT_ENABLE == STD_ON)
      /* Verify if DevAuthNoComModeIndication occurred. */
      SchM_Enter_EthSM_SCHM_ETHSM_EXCLUSIVE_AREA_0();
      TS_AtomicAssign8(DevAuthNoComFlag, EthSM_Network[NetworkIdx].DevAuthNoComFlag);
      TS_AtomicAssign8(EthSM_Network[NetworkIdx].DevAuthNoComFlag, FALSE); /* Reset flag */
      SchM_Exit_EthSM_SCHM_ETHSM_EXCLUSIVE_AREA_0();

      if(DevAuthNoComFlag == TRUE)
      {

        /* Process the state transitions which depend on DevAuthNoComModeIndication. */
        EthSM_HandleDevAuthNoComInd(
                                      NetworkIdx,
                                      EthSM_Network[NetworkIdx].EthSMState
                                    );
      }
#endif /* ETHSM_DEVAUTH_SUPPORT_ENABLE == STD_ON */
    }
  }

  DBG_ETHSM_MAINFUNCTION_EXIT();
}

FUNC(void, ETHSM_CODE) EthSM_CtrlModeIndication
(
  uint8 CtrlIdx,
  Eth_ModeType CtrlMode
)
{
  DBG_ETHSM_CTRLMODEINDICATION_ENTRY(CtrlIdx, CtrlMode);

#if(ETHSM_DEV_ERROR_DETECT == STD_ON)
  /* initialization check */
  if(EthSM_Initialized != TRUE)
  {
    ETHSM_DET_REPORT_ERROR(ETHSM_CTRLMODEINDICATION_SVCID, ETHSM_E_UNINIT);
  }
  else
#endif /* ETHSM_DEV_ERROR_DETECT */
  {
    /* lookup internal index from NetworkHandle */
    const uint8 NetworkIdx = ETHSM_LOOKUPNETWORK_IDX_FROM_CTRL_IDX(CtrlIdx);

#if(ETHSM_SINGLE_NETWORK_OPT_ENABLE == STD_ON)
    TS_PARAM_UNUSED(CtrlIdx);
#endif /* ETHSM_SINGLE_NETWORK_OPT_ENABLE */

#if(ETHSM_DEV_ERROR_DETECT == STD_ON)
    /* check if lookup was successful */
    if(NetworkIdx >= EthSM_RootPtr->NumNetworks)
    {
      ETHSM_DET_REPORT_ERROR(ETHSM_CTRLMODEINDICATION_SVCID, ETHSM_E_PARAM_CONTROLLER);
    }
    else
#endif /* ETHSM_DEV_ERROR_DETECT */
    {
      /* Setting the state change */
      SchM_Enter_EthSM_SCHM_ETHSM_EXCLUSIVE_AREA_0();
      EthSM_Network[NetworkIdx].CtrlMode = CtrlMode;
      TS_AtomicAssign8(EthSM_Network[NetworkIdx].CtrlModeChgFlag, TRUE);
      SchM_Exit_EthSM_SCHM_ETHSM_EXCLUSIVE_AREA_0();
    }
  }

  DBG_ETHSM_CTRLMODEINDICATION_EXIT(CtrlIdx, CtrlMode);
}

#if (ETHSM_DEVAUTH_SUPPORT_ENABLE == STD_ON)
FUNC(void, ETHSM_CODE) EthSM_DevAuthNoComModeIndication
(
  uint8 CtrlIdx
)
{
  DBG_ETHSM_DEVAUTHMODEINDICATION_ENTRY(CtrlIdx);

#if(ETHSM_DEV_ERROR_DETECT == STD_ON)
  /* initialization check */
  if(EthSM_Initialized != TRUE)
  {
    ETHSM_DET_REPORT_ERROR(ETHSM_DEVAUTHMODEINDICATION_SVCID, ETHSM_E_UNINIT);
  }
  else
#endif /* ETHSM_DEV_ERROR_DETECT */
  {
    /* lookup internal index from NetworkHandle */
    const uint8 NetworkIdx = ETHSM_LOOKUPNETWORK_IDX_FROM_CTRL_IDX(CtrlIdx);

#if(ETHSM_DEV_ERROR_DETECT == STD_ON)
    /* check if lookup was successful */
    if(NetworkIdx >= EthSM_RootPtr->NumNetworks)
    {
      ETHSM_DET_REPORT_ERROR(ETHSM_DEVAUTHMODEINDICATION_SVCID, ETHSM_E_PARAM_CONTROLLER);
    }
    else
#endif /* ETHSM_DEV_ERROR_DETECT */
    {
      /* Setting the state change */
      SchM_Enter_EthSM_SCHM_ETHSM_EXCLUSIVE_AREA_0();
      TS_AtomicAssign8(EthSM_Network[NetworkIdx].DevAuthNoComFlag, TRUE);
      SchM_Exit_EthSM_SCHM_ETHSM_EXCLUSIVE_AREA_0();
    }
  }

  DBG_ETHSM_DEVAUTHMODEINDICATION_EXIT(CtrlIdx);
}
#endif /* ETHSM_DEVAUTH_SUPPORT_ENABLE == STD_ON */

#define ETHSM_STOP_SEC_CODE
#include <EthSM_MemMap.h>

/*==================[internal function definitions]=========================*/

#define ETHSM_START_SEC_CODE
#include <EthSM_MemMap.h>

/* -------------------------------------------------------------------------
 *   from: ETHSM_STATE_OFFLINE --> TO: ETHSM_STATE_WAIT_CTRLMODEIND
 * ------------------------------------------------------------------------- */
STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_Offline_To_WaitCtrlModeInd
(
  uint8 NetworkIdx
)
{
  const uint8 CtrlIdx = ETHSM_GET_NETWORK_CTRLIDX(NetworkIdx);

  DBG_ETHSM_TRANSITION_OFFLINE_TO_WAITCTRLMODEIND_ENTRY(NetworkIdx);

  /* Reset controler mode state change flag. Expect a new notification after this transition. */
  TS_AtomicAssign8(EthSM_Network[NetworkIdx].CtrlModeChgFlag, FALSE); /* Reset flag */

  /* set Ethernet controller active */
  /* Error handling is not specified. */
  (void) EthIf_SetControllerMode(CtrlIdx, ETH_MODE_ACTIVE);

  /* switch internal state */
  TS_AtomicAssign8(EthSM_Network[NetworkIdx].EthSMState, ETHSM_STATE_WAIT_CTRLMODEIND);

  DBG_ETHSM_TRANSITION_OFFLINE_TO_WAITCTRLMODEIND_EXIT(NetworkIdx);
}

/* -------------------------------------------------------------------------
 *   from: ETHSM_STATE_WAIT_CTRLMODEIND --> TO: ETHSM_STATE_WAIT_TRCVLINK
 * ------------------------------------------------------------------------- */
STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_WaitCtrlModeInd_To_WaitTrcvLink
(
  uint8 NetworkIdx
)
{
  const NetworkHandleType NetworkHandle =ETHSM_GET_NETWORK_HANDLEIDX(NetworkIdx);

  DBG_ETHSM_TRANSITION_WAITCTRLMODEIND_TO_WAITTRCVLINK_ENTRY(NetworkIdx);

  /* indicate state change to BswM */
  BswM_EthSM_CurrentState(NetworkHandle, ETHSM_STATE_WAIT_TRCVLINK);

  /* switch internal state */
  TS_AtomicAssign8(EthSM_Network[NetworkIdx].EthSMState, ETHSM_STATE_WAIT_TRCVLINK);

  DBG_ETHSM_TRANSITION_WAITCTRLMODEIND_TO_WAITTRCVLINK_EXIT(NetworkIdx);
}

/* --------------------------------------------------------------------------
 *   from: ETHSM_STATE_WAIT_TRCVLINK --> TO: ETHSM_STATE_WAIT_CTRLMIND_DOWN
 * ------------------------------------------------------------------------- */
STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_WaitTrcvLink_To_WaitCtrlMIndDown
(
  uint8 NetworkIdx
)
{
  const uint8 CtrlIdx = ETHSM_GET_NETWORK_CTRLIDX(NetworkIdx);

  DBG_ETHSM_TRANSITION_WAITTRCVLINK_TO_WAIT_CTRLMIND_DOWN_ENTRY(NetworkIdx);

  /* set Ethernet controller down */
  /* Error handling is not specified. */
  (void) EthIf_SetControllerMode(CtrlIdx, ETH_MODE_DOWN);

  /* switch internal state */
  TS_AtomicAssign8(EthSM_Network[NetworkIdx].EthSMState, ETHSM_STATE_WAIT_CTRLMIND_DOWN);

  DBG_ETHSM_TRANSITION_WAITTRCVLINK_TO_WAIT_CTRLMIND_DOWN_EXIT(NetworkIdx);
}

/* --------------------------------------------------------------------------
 *   from: ETHSM_STATE_WAIT_CTRLMIND_DOWN --> TO: ETHSM_STATE_OFFLINE
 * ------------------------------------------------------------------------- */
STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_WaitCtrlMIndDown_To_Offline
(
  uint8 NetworkIdx
)
{
  const NetworkHandleType NetworkHandle = ETHSM_GET_NETWORK_HANDLEIDX(NetworkIdx);

  DBG_ETHSM_TRANSITION_WAIT_CTRLMIND_DOWN_TO_OFFLINE_ENTRY(NetworkIdx);
  /* switch internal state */
  TS_AtomicAssign8(EthSM_Network[NetworkIdx].EthSMState, ETHSM_STATE_OFFLINE);

  /* indicate state change to BswM */
  BswM_EthSM_CurrentState(NetworkHandle, ETHSM_STATE_OFFLINE);

  DBG_ETHSM_TRANSITION_WAIT_CTRLMIND_DOWN_TO_OFFLINE_EXIT(NetworkIdx);
}

/* -------------------------------------------------------------------------
 *   from: ETHSM_STATE_WAIT_TRCVLINK --> TO: ETHSM_STATE_WAIT_ONLINE
 * ------------------------------------------------------------------------- */
STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_WaitTrcvLink_To_WaitOnline
(
  uint8 NetworkIdx
)
{
  const NetworkHandleType NetworkHandle = ETHSM_GET_NETWORK_HANDLEIDX(NetworkIdx);
  const uint8 CtrlIdx = ETHSM_GET_NETWORK_CTRLIDX(NetworkIdx);

  DBG_ETHSM_TRANSITION_WAITTRCVLINK_TO_WAITONLINE_ENTRY(NetworkIdx);

  /* Reset TcpIp mode indication flag. Expect a new notification after this transition. */
  TS_AtomicAssign8(EthSM_Network[NetworkIdx].TcpIpModeIndFlag, FALSE); /* Reset flag */

  /* set TCP/IP online */
  /* Error handling is not specified. */
  (void) TcpIp_RequestComMode(CtrlIdx, TCPIP_STATE_ONLINE);

  /* switch internal state */
  TS_AtomicAssign8(EthSM_Network[NetworkIdx].EthSMState, ETHSM_STATE_WAIT_ONLINE);

  /* indicate state change to BswM */
  BswM_EthSM_CurrentState(NetworkHandle, ETHSM_STATE_WAIT_ONLINE);

#if(ETHSM_PROD_ERR_HANDLING_CTRL_TEST == TS_PROD_ERR_REP_TO_DEM)
  if(ETHSM_GET_NETWORK_DEMEVENTIDX(NetworkIdx) != ETHSM_INVALID_DEM_EVENTID)
  {
    Dem_ReportErrorStatus(
                           ETHSM_GET_NETWORK_DEMEVENTIDX(NetworkIdx),
                           DEM_EVENT_STATUS_PREPASSED
                         );
  }
#endif  /* (ETHSM_PROD_ERR_HANDLING_CTRL_TEST == TS_PROD_ERR_REP_TO_DEM) */

  DBG_ETHSM_TRANSITION_WAITTRCVLINK_TO_WAITONLINE_EXIT(NetworkIdx);
}

/* -------------------------------------------------------------------------
 *   from: ETHSM_STATE_WAIT_ONLINE --> TO: ETHSM_STATE_WAIT_TRCVLINK
 * ------------------------------------------------------------------------- */
STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_WaitOnline_To_WaitTrcvLink
(
  uint8 NetworkIdx
)
{
  const NetworkHandleType NetworkHandle = ETHSM_GET_NETWORK_HANDLEIDX(NetworkIdx);
  const uint8 CtrlIdx = ETHSM_GET_NETWORK_CTRLIDX(NetworkIdx);

  DBG_ETHSM_TRANSITION_WAITONLINE_TO_WAITTRCVLINK_ENTRY(NetworkIdx);

  /* set TCP/IP offline */
  /* Error handling is not specified. */
  (void) TcpIp_RequestComMode(CtrlIdx, TCPIP_STATE_OFFLINE);

  /* switch internal state */
  TS_AtomicAssign8(EthSM_Network[NetworkIdx].EthSMState, ETHSM_STATE_WAIT_TRCVLINK);

  /* indicate state change to BswM */
  BswM_EthSM_CurrentState(NetworkHandle, ETHSM_STATE_WAIT_TRCVLINK);

  DBG_ETHSM_TRANSITION_WAITONLINE_TO_WAITTRCVLINK_EXIT(NetworkIdx);
}

/* -------------------------------------------------------------------------
 *   from: ETHSM_STATE_WAIT_ONLINE --> TO: ETHSM_STATE_ONLINE
 * ------------------------------------------------------------------------- */
STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_WaitOnline_To_Online
(
  uint8 NetworkIdx
)
{
  const NetworkHandleType NetworkHandle = ETHSM_GET_NETWORK_HANDLEIDX(NetworkIdx);
  /* Set to COMM_FULL_COMMUNICATION again */
  EthSM_FullCommunication = COMM_FULL_COMMUNICATION;

  DBG_ETHSM_TRANSITION_WAITONLINE_TO_ONLINE_ENTRY(NetworkIdx);

  /* switch internal state */
  TS_AtomicAssign8(EthSM_Network[NetworkIdx].EthSMState, ETHSM_STATE_ONLINE);

  /* indicate state change to BswM */
  BswM_EthSM_CurrentState(NetworkHandle, ETHSM_STATE_ONLINE);

  /* indicate state change to ComM */
#if (ETHSM_MULTI_CORE == STD_OFF)
  ETHSM_CALL_COMM_BUSSM_MODEINDICATION(NetworkIdx, NetworkHandle, &EthSM_FullCommunication);
#else
  /* !LINKSTO EthSM.EB.MultiCoreSupport.SchM_Call_ComM_BusSM_ModeIndication, 1 */
  if (ETHSM_CALL_COMM_BUSSM_MODEINDICATION(NetworkIdx, NetworkHandle, COMM_FULL_COMMUNICATION) != SCHM_E_OK)
  {
#if (ETHSM_DEV_ERROR_DETECT == STD_ON)
      /* !LINKSTO EthSM.EB.MultiCoreSupport.SchM_Call_DET, 1 */
      ETHSM_DET_REPORT_ERROR(ETHSM_MAINFUNCTION_SVCID, ETHSM_E_BUSSMMODEIND);
#endif /* ETHSM_DEV_ERROR_DETECT */
  }
#endif /* ETHSM_MULTI_CORE */
  DBG_ETHSM_TRANSITION_WAITONLINE_TO_ONLINE_EXIT(NetworkIdx);
}

/* -------------------------------------------------------------------------
 *   from: ETHSM_STATE_WAIT_ONLINE --> TO: ETHSM_STATE_WAIT_CTRLMIND_DOWN
 * ------------------------------------------------------------------------- */
STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_WaitOnline_To_WaitCtrlMindDown
(
  uint8 NetworkIdx
)
{
  const uint8 CtrlIdx = ETHSM_GET_NETWORK_CTRLIDX(NetworkIdx);

  DBG_ETHSM_TRANSITION_WAITONLINE_TO_OFFLINE_ENTRY(NetworkIdx);

  /* set TCP/IP offline */
  /* Error handling is not specified. */
  (void) TcpIp_RequestComMode(CtrlIdx, TCPIP_STATE_OFFLINE);

  /* set Ethernet controller down */
  /* Error handling is not specified. */
  (void) EthIf_SetControllerMode(CtrlIdx, ETH_MODE_DOWN);

  /* switch internal state */
  TS_AtomicAssign8(EthSM_Network[NetworkIdx].EthSMState, ETHSM_STATE_WAIT_CTRLMIND_DOWN);

  DBG_ETHSM_TRANSITION_WAITONLINE_TO_OFFLINE_EXIT(NetworkIdx);
}

/* -------------------------------------------------------------------------
 *   from: ETHSM_STATE_ONLINE --> TO: ETHSM_STATE_WAIT_OFFLINE
 * ------------------------------------------------------------------------- */
STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_Online_To_WaitOffline
(
  uint8 NetworkIdx
)
{
  const NetworkHandleType NetworkHandle = ETHSM_GET_NETWORK_HANDLEIDX(NetworkIdx);
  const uint8 CtrlIdx = ETHSM_GET_NETWORK_CTRLIDX(NetworkIdx);

  DBG_ETHSM_TRANSITION_ONLINE_TO_WAITOFFLINE_ENTRY(NetworkIdx);

  /* set TCP/IP offline */
  /* Error handling is not specified. */
  (void) TcpIp_RequestComMode(CtrlIdx, TCPIP_STATE_OFFLINE);

  /* switch internal state */
  TS_AtomicAssign8(EthSM_Network[NetworkIdx].EthSMState, ETHSM_STATE_WAIT_OFFLINE);

  /* indicate state change to BswM */
  BswM_EthSM_CurrentState(NetworkHandle, ETHSM_STATE_WAIT_OFFLINE);

  DBG_ETHSM_TRANSITION_ONLINE_TO_WAITOFFLINE_EXIT(NetworkIdx);
}

/* -------------------------------------------------------------------------
 *   from: ETHSM_STATE_ONLINE --> TO: ETHSM_STATE_ONHOLD
 * ------------------------------------------------------------------------- */
STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_Online_To_OnHold
(
  uint8 NetworkIdx
)
{
  const NetworkHandleType NetworkHandle = ETHSM_GET_NETWORK_HANDLEIDX(NetworkIdx);
  const uint8 CtrlIdx = ETHSM_GET_NETWORK_CTRLIDX(NetworkIdx);

  DBG_ETHSM_TRANSITION_ONLINE_TO_ONHOLD_ENTRY(NetworkIdx);

  /* set TCP/IP on hold */
  /* Error handling is not specified. */
  (void) TcpIp_RequestComMode(CtrlIdx, TCPIP_STATE_ONHOLD);

  /* switch internal state */
  TS_AtomicAssign8(EthSM_Network[NetworkIdx].EthSMState, ETHSM_STATE_ONHOLD);

  /* indicate state change to BswM */
  BswM_EthSM_CurrentState(NetworkHandle, ETHSM_STATE_ONHOLD);

#if (ETHSM_PROD_ERR_HANDLING_CTRL_TEST == TS_PROD_ERR_REP_TO_DEM)

  if(ETHSM_GET_NETWORK_DEMEVENTIDX(NetworkIdx) != ETHSM_INVALID_DEM_EVENTID)
  {
    Dem_ReportErrorStatus(
                           ETHSM_GET_NETWORK_DEMEVENTIDX(NetworkIdx),
                           DEM_EVENT_STATUS_PREFAILED
                         );
  }

#elif( \
       (ETHSM_PROD_ERR_HANDLING_CTRL_TEST == TS_PROD_ERR_REP_TO_DET) \
       && (ETHSM_DEV_ERROR_DETECT == STD_ON) \
     )

  ETHSM_DET_REPORT_ERROR(ETHSM_TRCVLINKSTATECHG_SVCID, ETHSM_E_DEMTODET_CTRL_TEST);

#else /* (ETHSM_PROD_ERR_HANDLING_CTRL_TEST == TS_PROD_ERR_DISABLE) */

#endif  /* (ETHSM_PROD_ERR_HANDLING_CTRL_TEST == TS_PROD_ERR_REP_TO_DEM) */

  DBG_ETHSM_TRANSITION_ONLINE_TO_ONHOLD_EXIT(NetworkIdx);
}

/* -------------------------------------------------------------------------
 *   from: ETHSM_STATE_ONLINE --> TO: ETHSM_STATE_WAIT_ONLINE
 * ------------------------------------------------------------------------- */
STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_Online_To_WaitOnline
(
  uint8 NetworkIdx
)
{
  const NetworkHandleType NetworkHandle = ETHSM_GET_NETWORK_HANDLEIDX(NetworkIdx);

  DBG_ETHSM_TRANSITION_ONLINE_TO_WAITONLINE_ENTRY(NetworkIdx);

  /* switch internal state */
  TS_AtomicAssign8(EthSM_Network[NetworkIdx].EthSMState, ETHSM_STATE_WAIT_ONLINE);

  /* indicate state change to BswM */
  BswM_EthSM_CurrentState(NetworkHandle, ETHSM_STATE_WAIT_ONLINE);

  /* indicate state change to ComM */
  EthSM_NoCommunication = COMM_NO_COMMUNICATION;
#if (ETHSM_MULTI_CORE == STD_OFF)
  ETHSM_CALL_COMM_BUSSM_MODEINDICATION(NetworkIdx, NetworkHandle, &EthSM_NoCommunication);
#else
  /* !LINKSTO EthSM.EB.MultiCoreSupport.SchM_Call_ComM_BusSM_ModeIndication, 1 */
  if (ETHSM_CALL_COMM_BUSSM_MODEINDICATION(NetworkIdx, NetworkHandle, COMM_NO_COMMUNICATION) != SCHM_E_OK)
  {
#if (ETHSM_DEV_ERROR_DETECT == STD_ON)
      /* !LINKSTO EthSM.EB.MultiCoreSupport.SchM_Call_DET, 1 */
      ETHSM_DET_REPORT_ERROR(ETHSM_MAINFUNCTION_SVCID, ETHSM_E_BUSSMMODEIND);
#endif /* ETHSM_DEV_ERROR_DETECT */
  }
#endif /* ETHSM_MULTI_CORE */

  DBG_ETHSM_TRANSITION_ONLINE_TO_WAITONLINE_EXIT(NetworkIdx);
}

/* -------------------------------------------------------------------------
 *   from: ETHSM_STATE_ONHOLD --> TO: ETHSM_STATE_ONLINE
 * ------------------------------------------------------------------------- */
STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_OnHold_To_Online
(
  uint8 NetworkIdx
)
{
  const NetworkHandleType NetworkHandle = ETHSM_GET_NETWORK_HANDLEIDX(NetworkIdx);
  const uint8 CtrlIdx = ETHSM_GET_NETWORK_CTRLIDX(NetworkIdx);

  DBG_ETHSM_TRANSITION_ONHOLD_TO_ONLINE_ENTRY(NetworkIdx);

  /* set TCP/IP online */
  /* Error handling is not specified. */
  (void) TcpIp_RequestComMode(CtrlIdx, TCPIP_STATE_ONLINE);

  /* switch internal state */
  TS_AtomicAssign8(EthSM_Network[NetworkIdx].EthSMState, ETHSM_STATE_ONLINE);

  /* indicate state change to BswM */
  BswM_EthSM_CurrentState(NetworkHandle, ETHSM_STATE_ONLINE);

#if(ETHSM_PROD_ERR_HANDLING_CTRL_TEST == TS_PROD_ERR_REP_TO_DEM)

  if(ETHSM_GET_NETWORK_DEMEVENTIDX(NetworkIdx) != ETHSM_INVALID_DEM_EVENTID)
  {
    Dem_ReportErrorStatus(
                           ETHSM_GET_NETWORK_DEMEVENTIDX(NetworkIdx),
                           DEM_EVENT_STATUS_PREPASSED
                         );
  }

#elif( \
       (ETHSM_PROD_ERR_HANDLING_CTRL_TEST == TS_PROD_ERR_REP_TO_DET) \
       && (ETHSM_DEV_ERROR_DETECT == STD_ON) \
     )
  ETHSM_DET_REPORT_ERROR(ETHSM_TRCVLINKSTATECHG_SVCID, ETHSM_E_DEMTODET_CTRL_TEST);

#else /* (ETHSM_PROD_ERR_HANDLING_CTRL_TEST == TS_PROD_ERR_DISABLE) */

#endif  /* (ETHSM_PROD_ERR_HANDLING_CTRL_TEST == TS_PROD_ERR_REP_TO_DEM) */

  DBG_ETHSM_TRANSITION_ONHOLD_TO_ONLINE_EXIT(NetworkIdx);
}

/* -------------------------------------------------------------------------
 *   from: ETHSM_STATE_WAIT_LINK_DOWN --> TO: ETHSM_STATE_OFFLINE
 * ------------------------------------------------------------------------- */
STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_WaitLinkDown_To_Offline
(
  uint8 NetworkIdx
)
{
  const NetworkHandleType NetworkHandle = ETHSM_GET_NETWORK_HANDLEIDX(NetworkIdx);
  const uint8 CtrlIdx = ETHSM_GET_NETWORK_CTRLIDX(NetworkIdx);

  DBG_ETHSM_TRANSITION_WAITLINKDOWN_TO_OFFLINE_ENTRY(NetworkIdx);
  /* switch internal state */
  TS_AtomicAssign8(EthSM_Network[NetworkIdx].EthSMState, ETHSM_STATE_OFFLINE);

  /* set TCP/IP offline */
  /* Error handling is not specified. */
  (void) TcpIp_RequestComMode(CtrlIdx, TCPIP_STATE_OFFLINE);

  /* indicate state change to BswM */
  BswM_EthSM_CurrentState(NetworkHandle, ETHSM_STATE_OFFLINE);

  /* indicate state change to ComM */
  EthSM_NoCommunication = COMM_NO_COMMUNICATION;
#if (ETHSM_MULTI_CORE == STD_OFF)
  ETHSM_CALL_COMM_BUSSM_MODEINDICATION(NetworkIdx, NetworkHandle, &EthSM_NoCommunication);
#else
  /* !LINKSTO EthSM.EB.MultiCoreSupport.SchM_Call_ComM_BusSM_ModeIndication, 1 */
  if (ETHSM_CALL_COMM_BUSSM_MODEINDICATION(NetworkIdx, NetworkHandle, COMM_NO_COMMUNICATION) != SCHM_E_OK)
  {
#if (ETHSM_DEV_ERROR_DETECT == STD_ON)
      /* !LINKSTO EthSM.EB.MultiCoreSupport.SchM_Call_DET, 1 */
      ETHSM_DET_REPORT_ERROR(ETHSM_MAINFUNCTION_SVCID, ETHSM_E_BUSSMMODEIND);
#endif /* ETHSM_DEV_ERROR_DETECT */
  }
#endif /* ETHSM_MULTI_CORE */

  DBG_ETHSM_TRANSITION_WAITLINKDOWN_TO_OFFLINE_EXIT(NetworkIdx);
}

/* -------------------------------------------------------------------------
 *   from: ETHSM_STATE_ONHOLD --> TO: ETHSM_STATE_WAIT_TRCVLINK
 * ------------------------------------------------------------------------- */
STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_OnHold_To_WaitTrcvLink
(
  uint8 NetworkIdx
)
{
  const NetworkHandleType NetworkHandle = ETHSM_GET_NETWORK_HANDLEIDX(NetworkIdx);

  DBG_ETHSM_TRANSITION_ONHOLD_TO_WAITTRCVLINK_ENTRY(NetworkIdx);

  /* switch internal state */
  TS_AtomicAssign8(EthSM_Network[NetworkIdx].EthSMState, ETHSM_STATE_WAIT_TRCVLINK);

  /* indicate state change to BswM */
  BswM_EthSM_CurrentState(NetworkHandle, ETHSM_STATE_WAIT_TRCVLINK);

  /* indicate state change to ComM */
  EthSM_NoCommunication = COMM_NO_COMMUNICATION;
#if (ETHSM_MULTI_CORE == STD_OFF)
  ETHSM_CALL_COMM_BUSSM_MODEINDICATION(NetworkIdx, NetworkHandle, &EthSM_NoCommunication);
#else
  /* !LINKSTO EthSM.EB.MultiCoreSupport.SchM_Call_ComM_BusSM_ModeIndication, 1 */
  if (ETHSM_CALL_COMM_BUSSM_MODEINDICATION(NetworkIdx, NetworkHandle, COMM_NO_COMMUNICATION) != SCHM_E_OK)
  {
#if (ETHSM_DEV_ERROR_DETECT == STD_ON)
      /* !LINKSTO EthSM.EB.MultiCoreSupport.SchM_Call_DET, 1 */
      ETHSM_DET_REPORT_ERROR(ETHSM_MAINFUNCTION_SVCID, ETHSM_E_BUSSMMODEIND);
#endif /* ETHSM_DEV_ERROR_DETECT */
  }
#endif /* ETHSM_MULTI_CORE */

  DBG_ETHSM_TRANSITION_ONHOLD_TO_WAITTRCVLINK_EXIT(NetworkIdx);
}

#if (ETHSM_DEVAUTH_SUPPORT_ENABLE == STD_ON)
/* -------------------------------------------------------------------------
 *   from: ETHSM_STATE_WAIT_OFFLINE --> TO: ETHSM_STATE_WAIT_DEVAUTH_OFFLINE
 * ------------------------------------------------------------------------- */
STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_WaitOffline_To_WaitDevAuthOffline
(
  uint8 NetworkIdx
)
{
  const uint8 CtrlIdx = ETHSM_GET_NETWORK_CTRLIDX(NetworkIdx);

  DBG_ETHSM_TRANSITION_WAITOFFLINE_TO_WAITDEVAUTHOFFLINE_ENTRY(NetworkIdx);

  if(ETHSM_GET_NETWORK_DEVAUTHNOTIFENABLE(NetworkIdx) == TRUE)
  {
    /* call DevAuth_RequestComModeNoCom */
    /* Error handling is not specified. */
    (void) DevAuth_RequestComModeNoCom(CtrlIdx);
  }
  else
  {
    /* if EthSMDevAuthNoComNotificationEnable is not set for this Network */
    /* call EthSM_DevAuthNoComModeIndication() to exit this state */
    EthSM_DevAuthNoComModeIndication(CtrlIdx);
  }

  /* switch internal state */
  TS_AtomicAssign8(EthSM_Network[NetworkIdx].EthSMState, ETHSM_STATE_WAIT_DEVAUTH_OFFLINE);

  DBG_ETHSM_TRANSITION_WAITOFFLINE_TO_WAITDEVAUTHOFFLINE_EXIT(NetworkIdx);
}

/* -------------------------------------------------------------------------
 *   from: ETHSM_STATE_ONHOLD --> TO: ETHSM_STATE_WAIT_DEVAUTH_OFFLINE
 * ------------------------------------------------------------------------- */
STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_OnHold_To_WaitDevAuthOffline
(
  uint8 NetworkIdx
)
{
  const uint8 CtrlIdx = ETHSM_GET_NETWORK_CTRLIDX(NetworkIdx);

  DBG_ETHSM_TRANSITION_ONHOLD_TO_WAITDEVAUTHOFFLINE_ENTRY(NetworkIdx);

  if(ETHSM_GET_NETWORK_DEVAUTHNOTIFENABLE(NetworkIdx) == TRUE)
  {
    /* call DevAuth_RequestComModeNoCom */
    /* Error handling is not specified. */
    (void) DevAuth_RequestComModeNoCom(CtrlIdx);
  }
  else
  {
    /* if EthSMDevAuthNoComNotificationEnable is not set for this Network */
    /* call EthSM_DevAuthNoComModeIndication() to exit this state */
    EthSM_DevAuthNoComModeIndication(CtrlIdx);
  }

  /* switch internal state */
  TS_AtomicAssign8(EthSM_Network[NetworkIdx].EthSMState, ETHSM_STATE_WAIT_DEVAUTH_OFFLINE);

  DBG_ETHSM_TRANSITION_ONHOLD_TO_WAITDEVAUTHOFFLINE_EXIT(NetworkIdx);
}


/* ---------------------------------------------------------------------------
 *   from: ETHSM_STATE_WAIT_DEVAUTH_OFFLINE --> TO: ETHSM_STATE_WAIT_LINK_DOWN
 * --------------------------------------------------------------------------- */
STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_WaitDevAuthOffline_To_WaitLinkDown
(
  uint8 NetworkIdx
)
{
  const uint8 CtrlIdx = ETHSM_GET_NETWORK_CTRLIDX(NetworkIdx);

  DBG_ETHSM_TRANSITION_WAITDEVAUTHOFFLINE_TO_WAITLINKDOWN_ENTRY(NetworkIdx);

  /* set Ethernet controller down */
  /* Error handling is not specified. */
  (void) EthIf_SetControllerMode(CtrlIdx, ETH_MODE_DOWN);

  /* switch internal state */
  TS_AtomicAssign8(EthSM_Network[NetworkIdx].EthSMState, ETHSM_STATE_WAIT_LINK_DOWN);

  DBG_ETHSM_TRANSITION_WAITDEVAUTHOFFLINE_TO_WAITLINKDOWN_EXIT(NetworkIdx);
}

#else

/* -------------------------------------------------------------------------
 *   from: ETHSM_STATE_WAIT_OFFLINE --> TO: ETHSM_STATE_WAIT_LINK_DOWN
 * ------------------------------------------------------------------------- */
STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_WaitOffline_To_WaitLinkDown
(
  uint8 NetworkIdx
)
{
  const uint8 CtrlIdx = ETHSM_GET_NETWORK_CTRLIDX(NetworkIdx);

  DBG_ETHSM_TRANSITION_WAITOFFLINE_TO_WAITLINKDOWN_ENTRY(NetworkIdx);

  /* set Ethernet controller down */
  /* Error handling is not specified. */
  (void) EthIf_SetControllerMode(CtrlIdx, ETH_MODE_DOWN);

  /* switch internal state */
  TS_AtomicAssign8(EthSM_Network[NetworkIdx].EthSMState, ETHSM_STATE_WAIT_LINK_DOWN);

  DBG_ETHSM_TRANSITION_WAITOFFLINE_TO_WAITLINKDOWN_EXIT(NetworkIdx);
}

/* -------------------------------------------------------------------------
 *   from: ETHSM_STATE_ONHOLD --> TO: ETHSM_STATE_WAIT_LINK_DOWN
 * ------------------------------------------------------------------------- */
STATIC FUNC(void, ETHSM_CODE) EthSM_Transition_OnHold_To_WaitLinkDown
(
  uint8 NetworkIdx
)
{
  const uint8 CtrlIdx = ETHSM_GET_NETWORK_CTRLIDX(NetworkIdx);

  DBG_ETHSM_TRANSITION_ONHOLD_TO_WAITLINKDOWN_ENTRY(NetworkIdx);

  /* set Ethernet controller down */
  /* Error handling is not specified. */
  (void) EthIf_SetControllerMode(CtrlIdx, ETH_MODE_DOWN);

   /* switch internal state */
   TS_AtomicAssign8(EthSM_Network[NetworkIdx].EthSMState, ETHSM_STATE_WAIT_LINK_DOWN);

  DBG_ETHSM_TRANSITION_ONHOLD_TO_WAITLINKDOWN_EXIT(NetworkIdx);
}

#endif /* ETHSM_DEVAUTH_SUPPORT_ENABLE == STD_ON */

#if (ETHSM_SINGLE_NETWORK_OPT_ENABLE == STD_OFF)
STATIC FUNC(uint8, ETHSM_CODE) EthSM_LookUpNetworkIdxFromNetworkHandle(uint8 NetworkHandle)
{
  uint8_least i;
  const uint8 EthSMNetworksMax = EthSM_RootPtr->NumNetworks;

  DBG_ETHSM_LOOKUPNETWORKIDXFROMNETWORKHANDLE_ENTRY(NetworkHandle);

  for(i = 0U; i < EthSMNetworksMax; i++)
  {
    if(ETHSM_GET_NETWORK_HANDLEIDX(i) == NetworkHandle)
    {
      /* Network index found. */
      break;
    }
  }

  DBG_ETHSM_LOOKUPNETWORKIDXFROMNETWORKHANDLE_EXIT((uint8)i, NetworkHandle);
  return (uint8)i;
}

STATIC FUNC(uint8, ETHSM_CODE) EthSM_LookUpNetworkIdxFromCtrlIdx(uint8 CtrlIdx)
{
  uint8_least i;
  const uint8 EthSMNetworksMax = EthSM_RootPtr->NumNetworks;

  DBG_ETHSM_LOOKUPNETWORKIDXFROMCTRLIDX_ENTRY(CtrlIdx);

  for(i = 0U; i < EthSMNetworksMax; i++)
  {
    if(ETHSM_GET_NETWORK_CTRLIDX(i) == CtrlIdx)
    {
      /* Network index found. */
      break;
    }
  }

  DBG_ETHSM_LOOKUPNETWORKIDXFROMCTRLIDX_EXIT((uint8)i, CtrlIdx);
  return (uint8)i;
}
#endif /* ETHSM_SINGLE_NETWORK_OPT_ENABLE */

STATIC FUNC(void, ETHSM_CODE) EthSM_HandleTcpIpModeIndication
(
  uint8 NetworkIdx,
  EthSM_NetworkModeStateType EthSMState,
  TcpIp_StateType TcpIpState
)
{
  DBG_ETHSM_HANDLETCPIPMODEINDICATION_ENTRY(NetworkIdx, EthSMState, TcpIpState);

  /* Check whether indicated state is OFFLINE */
  if(TcpIpState == TCPIP_STATE_OFFLINE)
  {
    switch(EthSMState)
    {
      case ETHSM_STATE_WAIT_OFFLINE:
      {
#if (ETHSM_DEVAUTH_SUPPORT_ENABLE == STD_ON)
        /* --- from: ETHSM_STATE_WAIT_OFFLINE --> TO: ETHSM_STATE_WAIT_DEVAUTH_OFFLINE --- */
        EthSM_Transition_WaitOffline_To_WaitDevAuthOffline(NetworkIdx);
#else
        /* --- from: ETHSM_STATE_WAIT_OFFLINE --> TO: ETHSM_STATE_WAIT_LINK_DOWN --- */
        EthSM_Transition_WaitOffline_To_WaitLinkDown(NetworkIdx);
#endif
        break;
      }
      case ETHSM_STATE_ONHOLD:
      { /* --- from: ETHSM_STATE_ONHOLD --> TO: ETHSM_STATE_WAIT_TRCVLINK --- */
        EthSM_Transition_OnHold_To_WaitTrcvLink(NetworkIdx);
        break;
      }
      case ETHSM_STATE_ONLINE:
      { /* --- from: ETHSM_STATE_ONLINE --> TO: ETHSM_STATE_WAIT_ONLINE --- */
        EthSM_Transition_Online_To_WaitOnline(NetworkIdx);
        break;
      }
      default: /* Ignore mode indication for all other states. */
      {
        break;
      }
    }
  }
  else
  {
    ETHSM_PRECONDITION_ASSERT(TcpIpState == TCPIP_STATE_ONLINE, ETHSM_MAINFUNCTION_SVCID);

    if(EthSMState == ETHSM_STATE_WAIT_ONLINE)
    { /* --- ETHSM_STATE_WAIT_ONLINE --> TO: ETHSM_STATE_ONLINE --- */
      EthSM_Transition_WaitOnline_To_Online(NetworkIdx);
    } /* Ignore mode indication for all other states. */
  }

  DBG_ETHSM_HANDLETCPIPMODEINDICATION_EXIT(NetworkIdx, EthSMState, TcpIpState);
}

STATIC FUNC(boolean, ETHSM_CODE) EthSM_HandleComMModeRequest
(
  uint8 NetworkIdx,
  EthSM_NetworkModeStateType EthSMState,
  ComM_ModeType ComMMode
)
{
  boolean RestoreFlag = FALSE; /* by default do not restore the ComMMode flag */
  DBG_ETHSM_HANDLECOMMMODEREQUEST_ENTRY(NetworkIdx, EthSMState, ComMMode);

  if(ComMMode == COMM_FULL_COMMUNICATION)
  {
    if(EthSMState == ETHSM_STATE_OFFLINE)
    {
      /* --- from: ETHSM_STATE_OFFLINE --> TO: ETHSM_STATE_WAIT_CTRLMODEIND --- */
      EthSM_Transition_Offline_To_WaitCtrlModeInd(NetworkIdx);
    }
    else
    {
      RestoreFlag = TRUE; /* For all other states, remember notification. */
    }
  }
  else
  {
    ETHSM_PRECONDITION_ASSERT(ComMMode == COMM_NO_COMMUNICATION, ETHSM_MAINFUNCTION_SVCID);

    switch(EthSMState)
    {
    case ETHSM_STATE_WAIT_TRCVLINK:
    { /* --- from: ETHSM_STATE_WAIT_TRCVLINK --> TO: ETHSM_STATE_WAIT_CTRLMIND_DOWN --- */
      EthSM_Transition_WaitTrcvLink_To_WaitCtrlMIndDown(NetworkIdx);
      break;
    }
      case ETHSM_STATE_WAIT_ONLINE:
      { /* --- from: ETHSM_STATE_WAIT_ONLINE --> TO: ETHSM_STATE_WAIT_CTRLMIND_DOWN --- */
        EthSM_Transition_WaitOnline_To_WaitCtrlMindDown(NetworkIdx);
        break;
      }
      case ETHSM_STATE_ONHOLD:
      {
#if (ETHSM_DEVAUTH_SUPPORT_ENABLE == STD_ON)
        /* --- from: ETHSM_STATE_ONHOLD --> TO: ETHSM_STATE_WAIT_DEVAUTH_OFFLINE --- */
        EthSM_Transition_OnHold_To_WaitDevAuthOffline(NetworkIdx);
#else
        /* --- from: ETHSM_STATE_ONHOLD --> TO: ETHSM_STATE_WAIT_LINK_DOWN --- */
        EthSM_Transition_OnHold_To_WaitLinkDown(NetworkIdx);
#endif /* ETHSM_DEVAUTH_SUPPORT_ENABLE == STD_ON */
        break;
      }
      case ETHSM_STATE_ONLINE:
      { /* --- from: ETHSM_STATE_ONLINE --> TO: ETHSM_STATE_WAIT_OFFLINE --- */
        EthSM_Transition_Online_To_WaitOffline(NetworkIdx);
        break;
      }

      default:
      {
        break; /* Ignore mode request for all other states. */
      }
    }
  }

  DBG_ETHSM_HANDLECOMMMODEREQUEST_EXIT(RestoreFlag, NetworkIdx, EthSMState, ComMMode);
  return RestoreFlag;
}

STATIC FUNC(void, ETHSM_CODE) EthSM_HandleTrcvLinkStateChg
(
  uint8 NetworkIdx,
  EthSM_NetworkModeStateType EthSMState,
  EthTrcv_LinkStateType TrcvLinkState
)
{
  DBG_ETHSM_HANDLETRCVLINKSTATECHG_ENTRY(NetworkIdx, EthSMState, TrcvLinkState);

  if(TrcvLinkState == ETHTRCV_LINK_STATE_ACTIVE)
  {
    switch(EthSMState)
    {
      case ETHSM_STATE_WAIT_TRCVLINK:
      { /* --- ETHSM_STATE_WAIT_TRCVLINK --> TO: ETHSM_STATE_WAIT_ONLINE --- */
        EthSM_Transition_WaitTrcvLink_To_WaitOnline(NetworkIdx);
        break;
      }
      case ETHSM_STATE_ONHOLD:
      { /* --- from: ETHSM_STATE_ONHOLD --> TO: ETHSM_STATE_ONLINE --- */
        EthSM_Transition_OnHold_To_Online(NetworkIdx);
        break;
      }
      default:
      {
        break; /* Ignore transceiver link state changes for all other states. */
      }
    }
  }
  else
  {
    ETHSM_PRECONDITION_ASSERT(TrcvLinkState == ETHTRCV_LINK_STATE_DOWN, ETHSM_MAINFUNCTION_SVCID);

    switch(EthSMState)
    {
      case ETHSM_STATE_WAIT_ONLINE:
      { /* --- from: ETHSM_STATE_WAIT_ONLINE --> TO: ETHSM_STATE_WAIT_TRCVLINK --- */
        EthSM_Transition_WaitOnline_To_WaitTrcvLink(NetworkIdx);
        break;
      }
      case ETHSM_STATE_ONLINE:
      { /* --- from: ETHSM_STATE_ONLINE --> TO: ETHSM_STATE_ONHOLD --- */
        EthSM_Transition_Online_To_OnHold(NetworkIdx);
        break;
      }
      default:
      {
        break; /* Ignore transceiver link state changes for all other states. */
      }
    }
  }

  DBG_ETHSM_HANDLETRCVLINKSTATECHG_EXIT(NetworkIdx, EthSMState, TrcvLinkState);
}

STATIC FUNC(void, ETHSM_CODE) EthSM_HandleCtrlModeStateChg
(
  uint8 NetworkIdx,
  EthSM_NetworkModeStateType EthSMState,
  Eth_ModeType CtrlMode
)
{
  DBG_ETHSM_HANDLECTRLMODEINDICATION_ENTRY(NetworkIdx, EthSMState, CtrlMode);

  if(CtrlMode == ETH_MODE_ACTIVE)
  {
    switch(EthSMState)
    {
      case ETHSM_STATE_WAIT_CTRLMODEIND:
      { /* --- ETHSM_STATE_WAIT_CTRLMODEIND --> TO: ETHSM_STATE_WAIT_TRCVLINK --- */
        EthSM_Transition_WaitCtrlModeInd_To_WaitTrcvLink(NetworkIdx);
        break;
      }
      default:
      {
        break; /* Ignore controler mode state changes for all other states. */
      }
    }
  }
  else
  {
    ETHSM_PRECONDITION_ASSERT(CtrlMode == ETH_MODE_DOWN, ETHSM_MAINFUNCTION_SVCID);

    switch(EthSMState)
    {
      case ETHSM_STATE_WAIT_LINK_DOWN:
      { /* --- from: ETHSM_STATE_WAIT_LINK_DOWN --> TO: ETHSM_STATE_OFFLINE --- */
        EthSM_Transition_WaitLinkDown_To_Offline(NetworkIdx);
        break;
      }
      case ETHSM_STATE_WAIT_CTRLMIND_DOWN:
      { /* --- from: ETHSM_STATE_WAIT_CTRLMIND_DOWN --> TO: ETHSM_STATE_OFFLINE --- */
        EthSM_Transition_WaitCtrlMIndDown_To_Offline(NetworkIdx);
        break;
      }
      default:
      {
        break; /* Ignore controler mode state changes for all other states. */
      }
    }
  }

  DBG_ETHSM_HANDLECTRLMODEINDICATION_EXIT(NetworkIdx, EthSMState, CtrlMode)
}

#if (ETHSM_DEVAUTH_SUPPORT_ENABLE == STD_ON)
STATIC FUNC(void, ETHSM_CODE) EthSM_HandleDevAuthNoComInd
(
  uint8 NetworkIdx,
  EthSM_NetworkModeStateType EthSMState
)
{
  DBG_ETHSM_HANDLEDEVAUTHNOCOMIND_ENTRY(NetworkIdx, EthSMState);

  switch(EthSMState)
  {
    case ETHSM_STATE_WAIT_DEVAUTH_OFFLINE:
    { /* --- ETHSM_STATE_WAIT_DEVAUTH_OFFLINE --> TO: ETHSM_STATE_WAIT_LINK_DOWN --- */
      EthSM_Transition_WaitDevAuthOffline_To_WaitLinkDown(NetworkIdx);
      break;
    }
    default:
    {
      break; /* Ignore DevAuthNoComModeIndication for all other states. */
    }
  }

  DBG_ETHSM_HANDLEDEVAUTHNOCOMIND_EXIT(NetworkIdx, EthSMState);
}
#endif /* ETHSM_DEVAUTH_SUPPORT_ENABLE == STD_ON */

#define ETHSM_STOP_SEC_CODE
#include <EthSM_MemMap.h>


/*==================[end of file]===========================================*/
