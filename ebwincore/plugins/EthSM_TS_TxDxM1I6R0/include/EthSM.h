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
#ifndef ETHSM_H
#define ETHSM_H

/*
 * MISRA-C:2012 Deviation List:
 *
 * MISRAC2012-1) Deviated Rule: 20.5 (advisory)
 * '#undef' shall not be used.
 *
 * Reason:
 * In order to modify the behavior of the included file, enclosing the file inclusion by a special
 * macro definition is required.
 *
 */

/* mgcc62 Deviation List
 *
 * MGCC62-1) Deviated Rule: redundant-decls
 * warning: redundant redeclaration [-Wredundant-decls]
 *
 * Reason:
 * EthSM_MainFunction can be redefined by BSW Scheduler (SchM).
 */

/*==================[inclusions]============================================*/

#include <TSAutosar.h>          /* EB specific standard types */
#include <ComStack_Types.h>
#include <ComM.h>
#include <TcpIp.h>              /* TcpIp interface header */
#include <EthSM_Version.h>      /* Module Version Info */
#include <EthSM_Types.h>        /* Module public types */
#include <EthSM_Lcfg.h>         /* Link-time config declarations */
#include <EthSM_Types_ExtGen.h>

#ifndef ETHSM_NO_CFGCLASSMIX_REQUIRED
#define ETHSM_NO_CFGCLASSMIX_REQUIRED
#endif
#include <EthSM_Cfg.h>          /* Module configuration */

/* Exclude post-build-time config include file if requested to do so. */
#if (!defined ETHSM_NO_PBCFG_REQUIRED) && (!defined ETHSM_NO_CFG_REQUIRED)
#if (ETHSM_PBCFGM_SUPPORT_ENABLED == STD_OFF)
#include <EthSM_PBcfg.h>            /* Post-build-time configuration. */
#endif /* ETHSM_PBCFGM_SUPPORT_ENABLED == STD_OFF */
#endif /* ETHSM_NO_PBCFG_REQUIRED */

/* Deviation MISRAC2012-1 */
#undef ETHSM_NO_LCFG_REQUIRED
#undef ETHSM_NO_PBCFG_REQUIRED
#undef ETHSM_NO_CFG_REQUIRED

/*==================[macros]================================================*/

#if (defined ETHSM_INSTANCE_ID)
#error ETHSM_INSTANCE_ID is already defined
#endif
/** \brief Defines the instance number of this Ethernet State Manager.
 **        Since multiple instances of Ethernet State Manager are not supported
 **        the Instance Id is always zero.
 **/
#define ETHSM_INSTANCE_ID 0U

#if (defined ETHSM_E_INVALID_NETWORK_MODE)
#error ETHSM_E_INVALID_NETWORK_MODE is already defined
#endif
/** \brief API requests called with invalid communication mode */
#define ETHSM_E_INVALID_NETWORK_MODE 0x01U

#if (defined ETHSM_E_UNINIT)
#error  ETHSM_E_UNINIT is already defined
#endif
/** \brief API requests called before EthSM module is initialized */
#define ETHSM_E_UNINIT 0x02U

#if (defined ETHSM_E_PARAM_POINTER)
#error ETHSM_E_PARAM_POINTER is already defined
#endif
/** \brief API requests called with invalid pointer in parameter list */
#define ETHSM_E_PARAM_POINTER 0x03U

#if (defined ETHSM_E_INVALID_NETWORK_HANDLE)
#error  ETHSM_E_INVALID_NETWORK_HANDLE is already defined
#endif
/** \brief API requests called with invalid parameter in parameter list */
#define ETHSM_E_INVALID_NETWORK_HANDLE 0x04U

#if (defined ETHSM_E_INVALID_TcpIpMode)
#error  ETHSM_E_INVALID_TcpIpMode is already defined
#endif
/** \brief API requests called with invalid parameter in parameter list */
#define ETHSM_E_INVALID_TcpIpMode 0x05U

#if (defined ETHSM_E_INVALID_TRCV_LINK_STATE)
#error  ETHSM_E_INVALID_TRCV_LINK_STATE is already defined
#endif
/** \brief API requests called with invalid parameter in parameter list */
#define ETHSM_E_INVALID_TRCV_LINK_STATE 0x06U

#if (defined ETHSM_E_PARAM_CONTROLLER)
#error  ETHSM_E_PARAM_CONTROLLER is already defined
#endif
/** \brief API requests called with invalid parameter in parameter list */
#define ETHSM_E_PARAM_CONTROLLER 0x07U

#if (defined ETHSM_E_PARAM_TRANSCEIVER)
#error  ETHSM_E_PARAM_TRANSCEIVER is already defined
#endif
/** \brief API requests called with invalid parameter in parameter list */
#define ETHSM_E_PARAM_TRANSCEIVER 0x08U

#if (defined ETHSM_E_BUSSMMODEIND)
#error  ETHSM_E_BUSSMMODEIND is already defined
#endif
/** \brief API request returns an error */
#define ETHSM_E_BUSSMMODEIND 0x09U

/* === Service IDs === */

#if (defined ETHSM_MAINFUNCTION_SVCID)
#error  ETHSM_MAINFUNCTION_SVCID is already defined
#endif
/** \brief Defines API service ID of function EthSM_MainFunction(). */
#define ETHSM_MAINFUNCTION_SVCID 0x01U

#if (defined ETHSM_GETVERSIONINFO_SVCID)
#error  ETHSM_GETVERSIONINFO_SVCID is already defined
#endif
/** \brief Defines API service ID of function EthSM_GetVersionInfo(). */
#define ETHSM_GETVERSIONINFO_SVCID 0x02U

#if (defined ETHSM_GETCURRENTINTERNALMODE_SVCID)
#error  ETHSM_GETCURRENTINTERNALMODE_SVCID is already defined
#endif
/** \brief Defines API service ID of function EthSM_GetCurrentInternalMode(). */
#define ETHSM_GETCURRENTINTERNALMODE_SVCID 0x03U

#if (defined ETHSM_GETCURRENTCOMMODE_SVCID)
#error  ETHSM_GETCURRENTCOMMODE_SVCID is already defined
#endif
/** \brief Defines API service ID of function EthSM_GetCurrentComMode(). */
#define ETHSM_GETCURRENTCOMMODE_SVCID 0x04U

#if (defined ETHSM_REQUESTCOMMODE_SVCID)
#error  ETHSM_REQUESTCOMMODE_SVCID is already defined
#endif
/** \brief Defines API service ID of function EthSM_RequestComMode(). */
#define ETHSM_REQUESTCOMMODE_SVCID 0x05U

#if (defined ETHSM_TRCVLINKSTATECHG_SVCID)
#error  ETHSM_TRCVLINKSTATECHG_SVCID is already defined
#endif
/** \brief Defines API service ID of function EthSM_TrcvLinkStateChg(). */
#define ETHSM_TRCVLINKSTATECHG_SVCID 0x06U

#if (defined ETHSM_INIT_SVCID)
#error  ETHSM_INIT_SVCID is already defined
#endif
/** \brief Defines API service ID of function EthSM_Init(). */
#define ETHSM_INIT_SVCID 0x07U

#if (defined ETHSM_TCPIPMODEINDICATION_SVCID)
#error  ETHSM_TCPIPMODEINDICATION_SVCID is already defined
#endif
/** \brief Defines API service ID of function EthSM_TcpIpModeIndication(). */
#define ETHSM_TCPIPMODEINDICATION_SVCID 0x08U

#if (defined ETHSM_CTRLMODEINDICATION_SVCID)
#error  ETHSM_CTRLMODEINDICATION_SVCID is already defined
#endif
/** \brief Defines API service ID of function EthSM_CtrlModeIndication(). */
#define ETHSM_CTRLMODEINDICATION_SVCID 0x09U

#if (defined ETHSM_TRCVMODEINDICATION_SVCID)
#error  ETHSM_TRCVMODEINDICATION_SVCID is already defined
#endif
/** \brief Defines API service ID of function EthSM_TrcvModeIndication(). */
#define ETHSM_TRCVMODEINDICATION_SVCID 0x10U

#if (ETHSM_DEVAUTH_SUPPORT_ENABLE == STD_ON)
#if (defined ETHSM_DEVAUTHMODEINDICATION_SVCID)
#error  ETHSM_DEVAUTHMODEINDICATION_SVCID is already defined
#endif
/** \brief Defines API service ID of function EthSM_DevAuthNoComModeIndication(). */
#define ETHSM_DEVAUTHMODEINDICATION_SVCID 0x11U
#endif /* ETHSM_DEVAUTH_SUPPORT_ENABLE == STD_ON */

/*==================[macro checks]==========================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#if ETHSM_ISVALIDCONFIG_MMAP_ENABLED == STD_ON
#define ETHSM_START_SEC_CODE_CC_BLOCK
#else
#define ETHSM_START_SEC_CODE
#endif
#include <EthSM_MemMap.h>

/** \brief Checks compatibility of the post-build-time configuration
 *
 * This service checks the compatibility of the post-build-time configuration
 * against the source code.
 *
 * \param[in] voidConfigPtr Pointer to the configuration data of the EthSM module.
 *
 * \return Result of compatibility check
 * \retval E_OK      Provided configuration is compatible.
 * \retval E_NOT_OK  Provided configuration is not compatible.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, ETHSM_CODE) EthSM_IsValidConfig
(
  P2CONST(void, AUTOMATIC, ETHSM_APPL_CONST) voidConfigPtr
);

#if ETHSM_ISVALIDCONFIG_MMAP_ENABLED == STD_ON
#define ETHSM_STOP_SEC_CODE_CC_BLOCK
#include <EthSM_MemMap.h>
#define ETHSM_START_SEC_CODE
#include <EthSM_MemMap.h>
#endif

/**
 * \brief EthSM_Init() initializes the EthSM module.
 *
 * This service initializes the EthSM module. It shall be the first function of the module which
 * to be called.
 *
 * \param[in] ConfigPtr - Address of the post-build configuration data structure.
 *
 * \return E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x07}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, ETHSM_CODE) EthSM_Init
(
  P2CONST(EthSM_ConfigType, AUTOMATIC, ETHSM_APPL_CONST) ConfigPtr
);

#if (ETHSM_VERSION_INFO_API == STD_ON)
/** \brief Get version information of the Ethernet State Manager.
 *
 * This service returns the version information of this module. The version
 * information includes:
 * - Module Id
 * - Vendor Id
 * - Vendor specific version numbers
 *
 * \param[out] versioninfo - Pointer where to store the version information of this module.
 *
 * \ServiceID{0x02}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, ETHSM_CODE) EthSM_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, ETHSM_APPL_DATA) versioninfo
);
#endif /* ETHSM_VERSION_INFO_API */

#if (ETHSM_MULTI_CORE == STD_OFF)
/**
 * \brief Service to set the requested Com mode.
 *
 * This service sets the Com mode requested by the upper layer.
 *
 * \param[in] NetworkHandle - Handle of the affected network.
 * \param[in] ComM_Mode - Com mode requested.
 *
 * \return E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x05}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, ETHSM_CODE) EthSM_RequestComMode
(
  NetworkHandleType NetworkHandle,
  ComM_ModeType ComM_Mode
);

/**
 * \brief Gets the current communication mode.
 *
 * This service gets the current communication mode of the network.
 *
 * \param[in] NetworkHandle - Handle of the affected network
 * \param[out] ComM_ModePtr - Address to write the current Com mode to.
 *
 * \return E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x04}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, ETHSM_CODE) EthSM_GetCurrentComMode
(
  NetworkHandleType NetworkHandle,
  P2VAR(ComM_ModeType, AUTOMATIC, ETHSM_APPL_DATA) ComM_ModePtr
);
#endif /* ETHSM_MULTI_CORE == STD_OFF */

/**
 * \brief EthSM main function.
 *
 * This service  operate the effects of the EthSM state machine.
 *
 * \ServiceID{0x01}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
/* Deviation MGCC62-1 */
extern FUNC(void, ETHSM_CODE) EthSM_MainFunction
(
  void
);

/**
 * \brief TcpIp mode indication.
 *
 * This service is called by the TcpIp to report the actual TcpIp state.
 *
 * \param[in] CtrlIdx - Controller index which changed the TcpIp state.
 * \param[in] TcpIpState - New state the TcpIp has changed to.
 *
 * \ServiceID{0x08}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, ETHSM_CODE) EthSM_TcpIpModeIndication
(
  uint8 CtrlIdx,
  TcpIp_StateType TcpIpState
);

#define ETHSM_STOP_SEC_CODE
#include <EthSM_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef ETHSM_H */
/*==================[end of file]===========================================*/
