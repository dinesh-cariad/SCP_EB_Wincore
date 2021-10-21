#ifndef ETHIF_H
#define ETHIF_H

/**
 * \file
 *
 * \brief AUTOSAR EthIf
 *
 * This file contains the implementation of the AUTOSAR
 * module EthIf.
 *
 * \version 1.9.18
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

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
 * EthIf_MainFunctionRx and EthIf_MainFunctionTx can be redefined by BSW Scheduler (SchM).
 */

/*==================[inclusions]============================================*/

#include <TSAutosar.h>          /* EB specific standard types */

#if (defined ETHIF_NO_CFGCLASSMIX_REQUIRED)
#error ETHIF_NO_CFGCLASSMIX_REQUIRED is already defined
#endif
#define ETHIF_NO_CFGCLASSMIX_REQUIRED
#include <EthIf_Cfg.h>          /* Module pre-compile configuration */

/* Exclude post-build-time config include file if requested to do so. */
#if (!defined ETHIF_NO_PBCFG_REQUIRED) && (!defined ETHIF_NO_CFG_REQUIRED)
#if (ETHIF_PBCFGM_SUPPORT_ENABLED == STD_OFF)
#include <EthIf_PBcfg.h>            /* Post-build-time configuration. */
#endif /* ETHIF_PBCFGM_SUPPORT_ENABLED == STD_OFF */
#endif /* ETHIF_NO_PBCFG_REQUIRED */

/* Deviation MISRAC2012-1 */
#undef ETHIF_NO_LCFG_REQUIRED
#undef ETHIF_NO_PBCFG_REQUIRED
#undef ETHIF_NO_CFG_REQUIRED

/* !LINKSTO EthIf.ASR43.SWS_EthIf_00006, 1 */
#include <EthIf_Version.h>      /* Module Version Info */
/* !LINKSTO EthIf.ASR43.SWS_EthIf_00152, 1 */
#include <EthIf_Types.h>        /* Module public types */

#include <EthIf_Types_ExtGen.h>    /* Generated EthIf_ConfigType definition. */

#if (ETHIF_WAKEUPSUPPORT_API == STD_ON)
#include <EcuM.h>
#endif /* ETHIF_WAKEUPSUPPORT_API == STD_ON */

#if (defined TS_RELOCATABLE_CFG_ENABLE)
#error TS_RELOCATABLE_CFG_ENABLE is already defined
#endif
/** \brief Enable/disable relocateable config */
#define TS_RELOCATABLE_CFG_ENABLE    ETHIF_RELOCATABLE_CFG_ENABLE
#include <TSPBConfig_Access.h>  /* Access to postbuild config */

/*==================[macros]================================================*/

#if (defined ETHIF_INSTANCE_ID)
#error ETHIF_INSTANCE_ID is already defined
#endif
/** \brief Defines the instance number of this Ethernet Interface.
 **        Since multiple instances of Ethernet Interface are not supported
 **        the Instance Id is always zero.
 **/
#define ETHIF_INSTANCE_ID                   0U

#if (defined ETHIF_E_INV_CTRL_IDX)
#error ETHIF_E_INV_CTRL_IDX is already defined
#endif
/** \brief API requests called with invalid controller index */
#define ETHIF_E_INV_CTRL_IDX                0x01U

#if (defined ETHIF_E_INV_TRCV_IDX)
#error ETHIF_E_INV_TRCV_IDX is already defined
#endif
/** \brief API requests called with invalid transceiver index */
#define ETHIF_E_INV_TRCV_IDX                0x02U

#if (defined ETHIF_E_INV_PORT_GROUP_IDX)
#error ETHIF_E_INV_PORT_GROUP_IDX is already defined
#endif
/** \brief API requests called with invalid port group index */
#define ETHIF_E_INV_PORT_GROUP_IDX          0x03U

#if (defined ETHIF_E_NOT_INITIALIZED)
#error ETHIF_E_NOT_INITIALIZED is already defined
#endif
/** \brief API requests called before EthIf module is initialized */
#define ETHIF_E_NOT_INITIALIZED             0x04U

#if (defined ETHIF_E_PARAM_POINTER)
#error ETHIF_E_PARAM_POINTER is already defined
#endif
/** \brief API requests called with invalid pointer in parameter list */
#define ETHIF_E_PARAM_POINTER               0x05U

#if (defined ETHIF_E_INV_PARAM)
#error ETHIF_E_INV_PARAM is already defined
#endif
/** \brief API requests called with invalid parameter */
#define ETHIF_E_INV_PARAM                   0x06U

#if (defined ETHIF_E_INIT_FAILED)
#error ETHIF_E_INIT_FAILED is already defined
#endif
/** \brief Initialization of EthIf module failed */
#define ETHIF_E_INIT_FAILED                 0x07U

#if (defined ETHIF_E_INV_SWT_IDX)
#error ETHIF_E_INV_SWT_IDX is already defined
#endif
/** \brief API requests called with invalid switch index */
#define ETHIF_E_INV_SWT_IDX                 0x90U

#if (defined ETHIF_E_HW_NOT_INITIALIZED)
#error ETHIF_E_HW_NOT_INITIALIZED is already defined
#endif
/** \brief API requests called with invalid parameter */
#define ETHIF_E_HW_NOT_INITIALIZED 0x80U

#if (defined ETHIF_INIT_SVCID)
#error ETHIF_INIT_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_Init().
 **/
#define ETHIF_INIT_SVCID                    0x01U

#if (defined ETHIF_SETCONTROLLERMODE_SVCID)
#error ETHIF_SETCONTROLLERMODE_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_SetControllerMode().
 **/
#define ETHIF_SETCONTROLLERMODE_SVCID       0x03U

#if (defined ETHIF_GETCONTROLLERMODE_SVCID)
#error ETHIF_GETCONTROLLERMODE_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_GetControllerMode().
 **/
#define ETHIF_GETCONTROLLERMODE_SVCID       0x04U

#if (defined ETHIF_CLEARTRCVSIGNALQUALITY_SVCID)
#error ETHIF_CLEARTRCVSIGNALQUALITY_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_ClearTrcvSignalQuality().
 **/
#define ETHIF_CLEARTRCVSIGNALQUALITY_SVCID       0x19U

#if (defined ETHIF_GETSWITCHPORTSIGNALQUALITY_SVCID)
#error ETHIF_GETSWITCHPORTSIGNALQUALITY_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_GetSwitchPortSignalQuality().
 **/
#define ETHIF_GETSWITCHPORTSIGNALQUALITY_SVCID       0x1aU

#if (defined ETHIF_CLEARSWITCHPORTSIGNALQUALITY_SVCID)
#error ETHIF_CLEARSWITCHPORTSIGNALQUALITY_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_ClearSwitchPortSignalQuality().
 **/
#define ETHIF_CLEARSWITCHPORTSIGNALQUALITY_SVCID       0x1bU

#if (defined ETHIF_GETPHYSADDR_SVCID)
#error ETHIF_GETPHYSADDR_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_GetPhysAddr().
 **/
#define ETHIF_GETPHYSADDR_SVCID             0x08U

#if (defined ETHIF_PROVIDETXBUFFER_SVCID)
#error ETHIF_PROVIDETXBUFFER_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_ProvideTxBuffer().
 **/
#define ETHIF_PROVIDETXBUFFER_SVCID         0x09U

#if (defined ETHIF_TRANSMIT_SVCID)
#error ETHIF_TRANSMIT_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_Transmit().
 **/
#define ETHIF_TRANSMIT_SVCID                0x0AU

#if (defined ETHIF_GETVERSIONINFO_SVCID)
#error ETHIF_GETVERSIONINFO_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_GetVersionInfo().
 **/
#define ETHIF_GETVERSIONINFO_SVCID          0x0BU

#if (defined ETHIF_CBK_RXINDICATION_SVCID)
#error ETHIF_CBK_RXINDICATION_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_Cbk_RxIndication().
 **/
#define ETHIF_CBK_RXINDICATION_SVCID        0x10U

#if (defined ETHIF_CBK_TXCONFIRMATION_SVCID )
#error ETHIF_CBK_TXCONFIRMATION_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_Cbk_TxConfirmation().
 **/
#define ETHIF_CBK_TXCONFIRMATION_SVCID      0x11U

#if (defined ETHIF_MAINFUNCTIONRX_SVCID)
#error ETHIF_MAINFUNCTIONRX_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_MainFunctionRx().
 **/
#define ETHIF_MAINFUNCTIONRX_SVCID          0x20U

#if (defined ETHIF_MAINFUNCTIONTX_SVCID)
#error ETHIF_MAINFUNCTIONTX_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_MainFunctionTx().
 **/
#define ETHIF_MAINFUNCTIONTX_SVCID          0x21U

#if (defined ETHIF_MAINFUNCTIONSTATE_SVCID)
#error ETHIF_MAINFUNCTIONSTATE_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_MainFunctionState().
 **/
#define ETHIF_MAINFUNCTIONSTATE_SVCID          0x05U

#if (defined ETHIF_SETPHYSADDR_SVCID)
#error ETHIF_SETPHYSADDR_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_SetPhysAddr().
 **/
#define ETHIF_SETPHYSADDR_SVCID             0x0dU

#if (defined ETHIF_UPDATEPHYSADDRFILTER_SVCID)
#error ETHIF_UPDATEPHYSADDRFILTER_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_UpdatePhysAddrFilter().
 **/
#define ETHIF_UPDATEPHYSADDRFILTER_SVCID    0x0cU

#if(ETHIF_GLOBAL_TIME_SYNC_API_ENABLE == STD_ON)

#if (defined ETHIF_GETCURRENTTIME_SVCID)
#error ETHIF_GETCURRENTTIME_SVCID is already defined
#endif
/** \brief Defines API ID of function EthIf_GetCurrentTime(). */
#define ETHIF_GETCURRENTTIME_SVCID 0x22U

#if (defined ETHIF_ENABLEEGRESSTIMESTAMP_SVCID)
#error ETHIF_ENABLEEGRESSTIMESTAMP_SVCID is already defined
#endif
/** \brief Defines API ID of function EthIf_EnableEgressTimeStamp(). */
#define ETHIF_ENABLEEGRESSTIMESTAMP_SVCID 0x23U

#if (defined ETHIF_GETEGRESSTIMESTAMP_SVCID)
#error ETHIF_GETEGRESSTIMESTAMP_SVCID is already defined
#endif
/** \brief Defines API ID of function EthIf_GetEgressTimeStamp(). */
#define ETHIF_GETEGRESSTIMESTAMP_SVCID 0x24U

#if (defined ETHIF_GETINGRESSTIMESTAMP_SVCID)
#error ETHIF_GETINGRESSTIMESTAMP_SVCID is already defined
#endif
/** \brief Defines API ID of function EthIf_GetIngressTimeStamp(). */
#define ETHIF_GETINGRESSTIMESTAMP_SVCID 0x25U

#if (defined ETHIF_SETCORRECTIONTIME_SVCID)
#error ETHIF_SETCORRECTIONTIME_SVCID is already defined
#endif
/** \brief Defines API ID of function EthIf_SetCorrectionTime(). */
#define ETHIF_SETCORRECTIONTIME_SVCID 0x26U

#if (defined ETHIF_SETGLOBALTIME_SVCID)
#error ETHIF_SETGLOBALTIME_SVCID is already defined
#endif
/** \brief Defines API ID of function EthIf_SetGlobalTime(). */
#define ETHIF_SETGLOBALTIME_SVCID 0x27U
#endif /* #if(ETHIF_GLOBAL_TIME_SYNC_API_ENABLE == STD_ON) */

#if (defined ETHIF_SETPHYLOOPBACKMODE_SVCID)
#error ETHIF_SETPHYLOOPBACKMODE_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_SetPhyLoopbackMode().
 **/
#define ETHIF_SETPHYLOOPBACKMODE_SVCID      0x12U

#if (defined ETHIF_SETPHYTXMODE_SVCID)
#error ETHIF_SETPHYTXMODE_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_SetPhyTxMode().
 **/
#define ETHIF_SETPHYTXMODE_SVCID            0x13U

#if (defined ETHIF_GETCABLEDIAGNOSTICSRESULT_SVCID)
#error ETHIF_GETCABLEDIAGNOSTICSRESULT_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_GetCableDiagnosticsResult().
 **/
#define ETHIF_GETCABLEDIAGNOSTICSRESULT_SVCID 0x14U

#if (defined ETHIF_GETPHYIDENTIFIER_SVCID)
#error ETHIF_GETPHYIDENTIFIER_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_GetPhyIdentifier().
 **/
#define ETHIF_GETPHYIDENTIFIER_SVCID        0x15U

#if (defined ETHIF_GETTRCVSIGNALQUALITY_SVCID)
#error ETHIF_GETTRCVSIGNALQUALITY_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_GetTrcvSignalQuality().
 **/
#define ETHIF_GETTRCVSIGNALQUALITY_SVCID     0x18U

#if (defined ETHIF_SETPHYTESTMODE_SVCID)
#error ETHIF_SETPHYTESTMODE_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_SetPhyTestMode().
 **/
#define ETHIF_SETPHYTESTMODE_SVCID          0x17U

#if(ETHIF_TRCV_SUPPORT_ENABLE == STD_ON)
#if(ETHIF_MII_API_ENABLE == STD_ON)

#if (defined ETHIF_READMII_SVCID)
#error ETHIF_READMII_SVCID is already defined
#endif
/** \brief Defines API ID of function EthIf_ReadMii(). */
#define ETHIF_READMII_SVCID 0x70U

#if (defined ETHIF_WRITEMII_SVCID)
#error ETHIF_WRITEMII_SVCID is already defined
#endif
/** \brief Defines API ID of function EthIf_WriteMii(). */
#define ETHIF_WRITEMII_SVCID 0x71U

#endif /* #if(ETHIF_MII_API_ENABLE == STD_ON) */
#endif /* #if(ETHIF_TRCV_SUPPORT_ENABLE == STD_ON) */

#if(ETHIF_DEVICEAUTHENTICATION_API == STD_ON)

#if (defined ETHIF_ENABLERELATEDETHIFCTRLS_SVCID)
#error ETHIF_ENABLERELATEDETHIFCTRLS_SVCID is already defined
#endif
/** \brief Defines API ID of function EthIf_EnableRelatedEthIfCtrls(). */
#define ETHIF_ENABLERELATEDETHIFCTRLS_SVCID 0xF0U

#if (defined ETHIF_DISABLERELATEDETHIFCTRLS_SVCID)
#error ETHIF_DISABLERELATEDETHIFCTRLS_SVCID is already defined
#endif
/** \brief Defines API ID of function EthIf_DisableRelatedEthIfCtrls(). */
#define ETHIF_DISABLERELATEDETHIFCTRLS_SVCID 0xF1U

#endif /* #if(ETHIF_DEVICEAUTHENTICATION_API == STD_ON) */

#if (ETHIF_RETRANSMIT_API == STD_ON)

#if (defined ETHIF_RETRANSMIT_SVCID)
#error ETHIF_RETRANSMIT_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_Retransmit().
 **/
#define ETHIF_RETRANSMIT_SVCID               0xF2U
#endif /* ETHIF_RETRANSMIT_API == STD_ON */


/** \brief Defines API ID of function EthIf_GetPortMacAddr(). */
#define ETHIF_GETPORTMACADDR_SVCID 0x28U

/** \brief Defines API ID of function EthIf_GetArlTable(). */
#define ETHIF_GETARLTABLE_SVCID 0x29U

/** \brief Defines API ID of function EthIf_GetBufferLevel(). */
#define ETHIF_GETBUFFERLEVEL_SVCID 0x2AU

/** \brief Defines API ID of function EthIf_StoreConfiguration(). */
#define ETHIF_STORECONFIGURATION_SVCID 0x2CU

/** \brief Defines API ID of function EthIf_ResetConfiguration(). */
#define ETHIF_RESETCONFIGURATION_SVCID 0x2DU

/** \brief Defines API ID of function EthIf_SwtGetCounterValues(). */
#define ETHIF_SWTGETCOUNTERVALUES_SVCID 0x40U

#if (defined ETHIF_SETTRANSCEIVERWAKEUPMODE_SVCID)
#error ETHIF_SETTRANSCEIVERWAKEUPMODE_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_SetTransceiverWakeupMode().
 **/
#define ETHIF_SETTRANSCEIVERWAKEUPMODE_SVCID 0x2EU

#if (defined ETHIF_GETTRANSCEIVERWAKEUPMODE_SVCID)
#error ETHIF_GETTRANSCEIVERWAKEUPMODE_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_GetTransceiverWakeupMode().
 **/
#define ETHIF_GETTRANSCEIVERWAKEUPMODE_SVCID 0x2FU

#if (defined ETHIF_CHECKWAKEUP_SVCID)
#error ETHIF_CHECKWAKEUP_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_CheckWakeup().
 **/
#define ETHIF_CHECKWAKEUP_SVCID 0x30U

#if (defined ETHIF_GETCTRLIDXLIST_SVCID)
#error ETHIF_GETCTRLIDXLIST_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_GetCtrlIdxList().
 **/
#define ETHIF_GETCTRLIDXLIST_SVCID 0x44U

#if (defined ETHIF_GETVLANID_SVCID)
#error ETHIF_GETVLANID_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_GetVlanId().
 **/
#define ETHIF_GETVLANID_SVCID 0x43U

#if (defined ETHIF_GETANDRESETMEASUREMENTDATA_SVCID)
#error ETHIF_GETANDRESETMEASUREMENTDATA_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_GetAndResetMeasurementData().
 **/
#define ETHIF_GETANDRESETMEASUREMENTDATA_SVCID 0x45U

#if (defined ETHIF_SWITCHPORTGROUPREQUESTMODE_SVCID)
#error ETHIF_SWITCHPORTGROUPREQUESTMODE_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_SwitchPortGroupRequestMode().
 **/
#define ETHIF_SWITCHPORTGROUPREQUESTMODE_SVCID 0x06U

#if (defined ETHIF_STARTALLPORTS_SVCID)
#error ETHIF_STARTALLPORTS_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_StartAllPorts().
 **/
#define ETHIF_STARTALLPORTS_SVCID 0x07U

#if (defined ETHIF_VERIFYCONFIG_SVCID)
#error ETHIF_VERIFYCONFIG_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_VerifyConfig().
 **/
#define ETHIF_VERIFYCONFIG_SVCID 0x40U

#if (defined ETHIF_SETFORWARDINGMODE_SVCID)
#error ETHIF_SETFORWARDINGMODE_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_SetForwardingMode().
 **/
#define ETHIF_SETFORWARDINGMODE_SVCID 0x41U

#if (defined ETHIF_CTRLMODEINDICATION_SVCID)
#error ETHIF_CTRLMODEINDICATION_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_CtrlModeIndication().
 **/
#define ETHIF_CTRLMODEINDICATION_SVCID 0x0EU

#if (defined ETHIF_TRCVMODEINDICATION_SVCID)
#error ETHIF_TRCVMODEINDICATION_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_TrcvModeIndication().
 **/
#define ETHIF_TRCVMODEINDICATION_SVCID 0x0FU

#if (defined ETHIF_SWITCHMGMTINFOINDICATION_SVCID)
#error ETHIF_SWITCHMGMTINFOINDICATION_SVCID is already defined
#endif
/** \brief Defines API id of function EthIf_SwitchMgmtInfoIndication().
 **/
#define ETHIF_SWITCHMGMTINFOINDICATION_SVCID 0x3AU

#undef TS_RELOCATABLE_CFG_ENABLE

/*==================[macro checks]==========================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#if (ETHIF_ISVALIDCONFIG_MMAP_ENABLED == STD_ON)
#define ETHIF_START_SEC_CODE_CC_BLOCK
#else
#define ETHIF_START_SEC_CODE
#endif
#include <EthIf_MemMap.h>

/** \brief Checks compatibility of the post-build-time configuration
 *
 * This service checks the compatibility of the post-build-time configuration
 * against the source code.
 *
 * \param[in] voidConfigPtr Pointer to the configuration data of the EthIf module.
 *
 * \return Result of compatibility check
 * \retval E_OK      Provided configuration is compatible.
 * \retval E_NOT_OK  Provided configuration is not compatible.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_IsValidConfig
(
  P2CONST(void, AUTOMATIC, ETHIF_APPL_CONST) voidConfigPtr
);

#if (ETHIF_ISVALIDCONFIG_MMAP_ENABLED == STD_ON)
#define ETHIF_STOP_SEC_CODE_CC_BLOCK
#include <EthIf_MemMap.h>
#define ETHIF_START_SEC_CODE
#include <EthIf_MemMap.h>
#endif

/**
 * \brief   Initializes the EthIf module.
 *
 * This service initializes the EthIf module.
 * It shall be the first function of the module which is called.
 *
 * \param[in] CfgPtr             Address of the post-build configuration data structure.
 *
 * \ServiceID{0x01}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, ETHIF_CODE) EthIf_Init
(
  P2CONST(EthIf_ConfigType, AUTOMATIC, ETHIF_APPL_CONST) CfgPtr
);

/**
 * \brief   Sets the Ethernet controller mode.
 *
 * This service sets the Ethernet controller mode.
 *
 * \param[in] CtrlIdx             Index of the controller within the context of the EthIf.
 * \param[in] CtrlMode            Mode to set the controller to.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x03}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetControllerMode
(
  uint8 CtrlIdx,
  Eth_ModeType CtrlMode
);

/**
 * \brief   Gets the Ethernet controller mode.
 *
 * This service gets the Ethernet controller mode .
 *
 * \param[in] CtrlIdx             Index of the controller within the context of the EthIf.
 * \param[out] CtrlModePtr        Address to set the current controller mode to.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x04}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetControllerMode
(
  uint8 CtrlIdx,
  P2VAR(Eth_ModeType, AUTOMATIC, ETHIF_APPL_DATA) CtrlModePtr
);

/**
 * \brief   Gets the local Ethernet physical address.
 *
 * This service gets the local Ethernet physical device address.
 *
 * \param[in] CtrlIdx             Index of the controller the transceiver is connected to.
 *                                within the context of the EthIf.
 * \param[out] PhysAddrPtr        Address to write the physical address to.
 *
 * \ServiceID{0x08}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, ETHIF_CODE) EthIf_GetPhysAddr
(
  uint8 CtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA) PhysAddrPtr
);

/**
 * \brief   Provide an Ethernet Tx buffer.
 *
 * This service returns an Ethernet driver Tx buffer with the required length.
 *
 * \param[in] CtrlIdx             Index of the controller within the context of the EthIf.
 * \param[in] FrameType           Ethernet type of the related payload for the transmission.
 * \param[in] Priority            Priority of the related payload for transmission
 *                                (used for VLAN only).
 * \param[out] BufIdxPtr          Address an index to identify the returned Tx buffer is stored to.
 * \param[out] BufPtr             Address of a pointer where the address of the returned Tx buffer
 *                                is stored to.
 * \param[in,out] LenBytePtr      Address that holds the requested buffer length and after return
 *                                of the call it holds the actual size of the returned Tx buffer.
 *
 * \return     BUFREQ_OK: Service execution was successful.
 *             BUFREQ_E_NOT_OK: Service execution failed.
 *             BUFREQ_E_BUSY: Service currently not available.
 *
 * \ServiceID{0x09}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(BufReq_ReturnType, ETHIF_CODE) EthIf_ProvideTxBuffer
(
  uint8 CtrlIdx,
  Eth_FrameType FrameType,
  uint8 Priority,
  P2VAR(Eth_BufIdxType, AUTOMATIC, ETHIF_APPL_DATA) BufIdxPtr,
  P2VAR(EthIf_Uint8TypePtr, AUTOMATIC, ETHIF_APPL_DATA) BufPtr,
  P2VAR(uint16, AUTOMATIC, ETHIF_APPL_DATA) LenBytePtr
);

/**
 * \brief   Transmit a Tx buffer on Ethernet.
 *
 * This service unlocks a previously provided Tx buffer for transmission.
 *
 * \param[in] CtrlIdx             Index of the controller within the context of the EthIf.
 * \param[in] BufIdx              Index of the buffer to transmit.
 * \param[in] FrameType           Ethernet type of the related payload for the transmission.
 * \param[in] TxConfirmation      Activates transmission confirmation.
 * \param[in] LenByte             Length of the payload contained in the tx buffer to transmit.
 * \param[in] PhysAddrPtr         Address which holds the destination address the data shall be
                                  transmitted to.
 *
 * \ServiceID{0x0a}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_Transmit
(
  uint8 CtrlIdx,
  Eth_BufIdxType BufIdx,
  Eth_FrameType FrameType,
  boolean TxConfirmation,
  uint16 LenByte,
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA) PhysAddrPtr
);


#if (ETHIF_RETRANSMIT_API == STD_ON)
/**
 * \brief   Retransmit a received/transmitted frame.
 *
 * This function retransmits the current buffer. It can be called to retransmit a received
 * buffer (OrigCtrlIdx and DataPtr of current buffer must be provided) and to retransmit a
 * transmitted buffer (OrigCtrlIdx and BufIdxPtr of current buffer must be provided).
 * The current buffer is not released when the related EthIf_RxIndication function or
 * EthIf_TxConfirmation function returns. In both cases the function returns an buffer index
 * (parameter BufIdxPtr).
 *
 * \param[in] CtrlIdx             Index of the controller where frame shall be re-transmitted
 *                                within the context of the EthIf.
 * \param[in] OrigCtrlIdx         Index of the controller of the provided buffer within the context
 *                                of the EthIf.
 * \param[in, out] BufIdxPtr      Pointer to the Index of the buffer to transmit.
 * \param[in] DataPtr             Buffer pointer of retransmitted frame. It points before the
 *                                VLAN portion of the frame.
 * \param[in] FrameType           Ethernet type of the related payload for the transmission.
 * \param[in] LenByte             Length of the payload contained in the tx buffer to transmit.
 * \param[in] RetransmitInfoPtr   Pointer to additional retransmit info.
 *                                In case of retransmit of an already transmitted buffer, Priority
 *                                needs to be set to 0xFF.
 * \return E_OK - re-transmission of the buffer was successful
 * \return E_NOT_OK - if provided CtrlIdx-es refer to not compatible EthIf controllers
 *        (e.g. origin EthIf controller uses VLAN and EthIf controller uses no VLAN or vice versa)
 * \ServiceID{0xF2U}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_Retransmit
(
  uint8 CtrlIdx,
  uint8 OrigCtrlIdx,
  P2VAR(Eth_BufIdxType, AUTOMATIC, ETHIF_APPL_DATA) BufIdxPtr,
  P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_DATA) DataPtr,
  Eth_FrameType FrameType,
  uint16 LenByte,
  CONSTP2CONST(Eth_RetransmitInfoType, AUTOMATIC, ETHIF_APPL_DATA) RetransmitInfoPtr
);
#endif /* ETHIF_RETRANSMIT_API == STD_ON */

/**
 * \brief set physical source address of controller.
 *
 * This service sets the physical source address used by the indexed controller.
 *
 * \param[in] CtrlIdx             Index of the controller within the context of the EthIf.
 * \param[in] PhysAddrPtr         Pointer to memory containing the physical source address (MAC
                                  address) in network byte order.
 *
 * \ServiceID{0x80}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, ETHIF_CODE) EthIf_SetPhysAddr
(
  uint8 CtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHIF_APPL_DATA) PhysAddrPtr
);

/**
 * \brief Updates physical address filter
 *
 * This service updates the physical source address to/from the indexed controller
 * filter. If the Ethernet Controller is not capable to do the filtering, the
 * software has to do this.
 *
 * \param[in] CtrlIdx             Index of the controller within the context of the EthIf.
 * \param[in] PhysAddrPtr         Pointer to memory containing the physical source address (MAC
                                  address) in network byte order.
 * \param[in] Action              Add or remove the address from the Ethernet controllers filter.
 *
 * \returns Std_ReturnType
 * \retval  E_OK     Filter was successfully changed.
 * \retval  E_NOT_OK Filter could not be changed.
 *
 * \ServiceID{0x81}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_UpdatePhysAddrFilter
(
  uint8 CtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHIF_APPL_DATA) PhysAddrPtr,
  Eth_FilterActionType Action
);

#if (ETHIF_GET_VERSION_INFO == STD_ON)

/**
 * \brief Get version information of the Ethernet Interface.
 *
 * This service returns the version information of this module. The version
 * information includes:
 * - Module Id
 * - Vendor Id
 * - Vendor specific version numbers
 *
 * \param[out] VersioninfoPtr Pointer where to store the version
 *                                 information of this module.
 *
 * \ServiceID{0x0b}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, ETHIF_CODE) EthIf_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, ETHIF_APPL_DATA) VersionInfoPtr
);
#endif /* ETHIF_GET_VERSION_INFO */

/**
 * \brief Main function for rx-tasks.
 *
 * This service performs the periodic tasks for Ethernet reception (polling the Ethernet driver).
 *
 * \ServiceID{0x20}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
 /* Deviation MGCC62-1 */
 extern FUNC(void, ETHIF_CODE) EthIf_MainFunctionRx(void);

#if ((ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430) || \
     (ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430_EB))
 /**
  * \brief Main function for priority receive processing tasks.
  *
  * This service performs the periodic tasks for ethernet reception (polling the ethernet driver)
  * for the priority frames only.
  * This function should be called via the related function EthIf_MainFunctionRx_<EthIfCtr>_Prio().
  *
  * \ServiceID{0xXX}
  * \Reentrancy{Non Reentrant}
  * \Synchronicity{Synchronous}
  */
extern FUNC(void, ETHIF_CODE) EthIf_MainFunctionRxPrio
(
  uint8 PCtrlIngrFifoIdx
);
#endif /* ((ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430) ||
           (ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430_EB)) */

/**
 * \brief Main function for tx-tasks.
 *
 * This service contains currently no functionality.
 *
 * \ServiceID{0x21}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
 /* Deviation MGCC62-1 */
extern FUNC(void, ETHIF_CODE) EthIf_MainFunctionTx(void);

#if(ETHIF_MAINFUNCTIONSTATE_API_ENABLE == STD_ON)
/**
 * \brief Function to update mode state and transceiver link state of EthIf objects.
 *
 * Function is polling the link state of the used communication hardware (Ethernet transceiver,
 * Ethernet switch ports). For active objects it reads-out / calculates transceiver link state and
 * reports it to upper layers (State Manager or BswM).
 *
 * \ServiceID{0x05}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Asynchronous}
 */
extern FUNC(void, ETHIF_CODE) EthIf_MainFunctionState(void);
#endif /* #if(ETHIF_MAINFUNCTIONSTATE_API_ENABLE == STD_ON) */

#if(ETHIF_GLOBAL_TIME_SYNC_API_ENABLE == STD_ON)
/**
 * \brief Service to get current hardware time.
 *
 * \param[in] CtrlIdx - EthIf controller index.
 * \param[out] timeQualPtr - Pointer containing the quality of the hardware time stamp.
 * \param[out] timeStampPtr - Pointer containing the current time.
 *
 * \return E_OK - Service execution was successful.
 *         E_NOT_OK - Service execution failed.
 *
 * \ServiceID{0x22}
 * \Reentrancy{Non-Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetCurrentTime
(
  uint8 CtrlIdx,
  P2VAR(Eth_TimeStampQualType, AUTOMATIC, ETHIF_APPL_DATA) timeQualPtr,
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETHIF_APPL_DATA) timeStampPtr
);

/**
 * \brief Service to enable egress time stamp.
 *
 * \param[in] CtrlIdx - EthIf controller index.
 * \param[in] BufIdx - Index of the used transmit buffer.
 *
 * \note For some HW it is required to enable egress time stamping for every transmission.
 *
 * \ServiceID{0x23}
 * \Reentrancy{Non-Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, ETHIF_CODE) EthIf_EnableEgressTimeStamp
(
  uint8 CtrlIdx,
  Eth_BufIdxType BufIdx
);

/**
 * \brief Service to get egress time stamp.
 *
 * \param[in] CtrlIdx - EthIf controller index.
 * \param[in] BufIdx - Index of the used transmit buffer.
 * \param[out] timeQualPtr - Pointer containing the quality of the hardware time stamp.
 * \param[out] timeStampPtr - Pointer containing the egress time stamp.
 *
 * \returns Std_ReturnType
 * \retval  E_OK     Operation was successful
 * \retval  E_NOT_OK Operation was not successful
 * \ServiceID{0x24}
 * \Reentrancy{Non-Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetEgressTimeStamp
(
  uint8 CtrlIdx,
  Eth_BufIdxType BufIdx,
  P2VAR(Eth_TimeStampQualType, AUTOMATIC, ETHIF_APPL_DATA) timeQualPtr,
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETHIF_APPL_DATA) timeStampPtr
);

/**
 * \brief Service to get ingress time stamp.
 *
 * \param[in] CtrlIdx - EthIf controller index.
 * \param[in] DataPtr - Pointer to Rx data buffer of the requested frame.
 * \param[out] timeQualPtr - Pointer containing the quality of the hardware time stamp.
 * \param[out] timeStampPtr - Pointer containing the ingress time stamp.
 *
 * \returns Std_ReturnType
 * \retval  E_OK     Operation was successful
 * \retval  E_NOT_OK Operation was not successful
 * \ServiceID{0x25}
 * \Reentrancy{Non-Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetIngressTimeStamp
(
  uint8 CtrlIdx,
  P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_DATA) DataPtr,
  P2VAR(Eth_TimeStampQualType, AUTOMATIC, ETHIF_APPL_DATA) timeQualPtr,
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETHIF_APPL_DATA) timeStampPtr
);

/**
 * \brief Service to perform time correction time stamp.
 *
 * \param[in] CtrlIdx - EthIf controller index.
 * \param[in] timeOffsetPtr - Pointer to adjust hardware time.
 * \param[in] rateRatioPtr - Pointer to set rate ratio.
 *
 * \ServiceID{0x26}
 * \Reentrancy{Non-Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, ETHIF_CODE) EthIf_SetCorrectionTime
(
  uint8 CtrlIdx,
  P2CONST(Eth_TimeIntDiffType, AUTOMATIC, ETHIF_APPL_DATA) timeOffsetPtr,
  P2CONST(Eth_RateRatioType, AUTOMATIC, ETHIF_APPL_DATA) rateRatioPtr
);

/**
 * \brief Service to set the global time.
 *
 * \param[in] CtrlIdx - EthIf controller index.
 * \param[in] timeStampPtr - Pointer containing the new time.
 *
 * \ServiceID{0x27}
 * \Reentrancy{Non-Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetGlobalTime
(
  uint8 CtrlIdx,
  P2CONST(Eth_TimeStampType, AUTOMATIC, ETHIF_APPL_DATA) timeStampPtr
);
#endif /* #if(ETHIF_GLOBAL_TIME_SYNC_API_ENABLE == STD_ON) */

#if((ETHIF_TRCV_SUPPORT_ENABLE == STD_ON) && ((ETHIF_ETH_SUPPORT_API != ETHIF_ETH_SUPPORT_API_ASR403) && \
    (ETHIF_ETH_SUPPORT_API != ETHIF_ETH_SUPPORT_API_ASR422)))

/** \brief Activates a given loopback mode.
 **
 ** This function activates a given loopback mode of the indexed transceiver
 **
 ** \param[in]  TrcvIdx   Index of the transceiver within the context of the Ethernet Interface
 ** \param[in]  Mode      Loopback mode to be activated:
 **                       ETHTRCV_PHYLOOPBACK_NONE - Normal operation
 **                       ETHTRCV_PHYLOOPBACK_INTERNAL - Internal loopback
 **                       ETHTRCV_PHYLOOPBACK_EXTERNAL - External loopback
 **                       ETHTRCV_PHYLOOPBACK_REMOTE - Remote loopback
 **
 ** \return Standard    Return Code
 ** \retval E_OK        The request has been accepted.
 ** \retval E_NOT_OK    The request has not been accepted.
 **
 ** \ServiceID{0x12}
 ** \Reentrancy{Reentrant for different TrcvIdx. Non reentrant for the same TrcvIdx.}
 ** \Synchronicity{synchronous}
 **
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetPhyLoopbackMode
(
  uint8 TrcvIdx,
  EthTrcv_PhyLoopbackModeType Mode
);

/** \brief Activates a given transmission mode.
 **
 ** This function activates a given transmission mode of the link of the indexed transceiver.
 **
 ** \param[in] TrcvIdx  Index of the transceiver within the context of the Ethernet Interface
 ** \param[in] Mode     Transmission mode to be activated:
 **                     ETHTRCV_PHYTXMODE_NORMAL - Normal operation
 **                     ETHTRCV_PHYTXMODE_TX_OFF - Transmitter disabled
 **                     ETHTRCV_PHYTXMODE_SCRAMBLER_OFF - Scrambler disabled
 **
 ** \return Standard    Return Code
 ** \retval E_OK        The request has been accepted.
 ** \retval E_NOT_OK    The request has not been accepted.
 **
 ** \ServiceID{0x13}
 ** \Reentrancy{Reentrant for different TrcvIdx. Non reentrant for the same TrcvIdx.}
 ** \Synchronicity{synchronous}
 **
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetPhyTxMode
(
  uint8 TrcvIdx,
  EthTrcv_PhyTxModeType Mode
);

/** \brief Obtains the transceiver signal quality.
 **
 ** This function obtains the signal quality of the link of the indexed transceiver.
 **
 ** \param[in]  TrcvIdx           Index of the transceiver within the context of the
 **                               Ethernet Interface.
 ** \param[out] ResultPtr  Pointer to the memory where the signal quality shall be stored.
 **
 ** \return Std_ReturnType
 ** \retval E_OK        The request has been accepted.
 ** \retval E_NOT_OK    The request has not been accepted.
 **
 ** \ServiceID{0x18}
 ** \Reentrancy{Reentrant for different TrcvIdx. Non reentrant for the same TrcvIdx.}
 ** \Synchronicity{synchronous}
 **
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetTrcvSignalQuality
(
  uint8 TrcvIdx,
  P2VAR(EthIf_SignalQualityResultType, AUTOMATIC, ETHIF_APPL_DATA) ResultPtr
);

/** \brief Clears stored signal quality.
 **
 ** This function clears the stored signal quality of the link for the indexed transceiver.
 **
 ** \param[in]  TrcvIdx           Index of the transceiver within the context of the
 **                               Ethernet Interface.
 **
 ** \return Std_ReturnType
 ** \retval E_OK        The request has been accepted.
 ** \retval E_NOT_OK    The request has not been accepted.
 **
 ** \ServiceID{none}
 ** \Reentrancy{Reentrant for different TrcvIdx. Non reentrant for the same TrcvIdx.}
 ** \Synchronicity{synchronous}
 **
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_ClearTrcvSignalQuality
(
  uint8 TrcvIdx
);


/** \brief Retrieves the cable diagnostics result.
 **
 ** This function retrieves the cable diagnostics result of the indexed transceiver.
 **
 ** \param[in]  TrcvIdx   Index of the transceiver within the context of the Ethernet Interface
 ** \param[out] ResultPtr Pointer to the location where the cable diagnostics result shall
 **                       be stored:
 **                       ETHTRCV_CABLEDIAG_OK - Cable diagnostic OK
 **                       ETHTRCV_CABLEDIAG_ERROR - Cable diagnostic failed
 **                       ETHTRCV_CABLEDIAG_SHORT - Short circuit detected
 **                       ETHTRCV_CABLEDIAG_OPEN - Open circuit detected
 **
 ** \return Standard    Return Code
 ** \retval E_OK        The request has been accepted.
 ** \retval E_NOT_OK    The request has not been accepted.
 **
 ** \ServiceID{0x14}
 ** \Reentrancy{Reentrant for different TrcvIdx. Non reentrant for the same TrcvIdx.}
 ** \Synchronicity{synchronous}
 **
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetCableDiagnosticsResult
(
  uint8 TrcvIdx,
  P2VAR(EthTrcv_CableDiagResultType, AUTOMATIC, ETHIF_APPL_DATA) ResultPtr
);

/** \brief Obtains the PHY identifier of the Ethernet Transceiver
 **
 ** This function obtains the PHY identifier of the Ethernet Transceiver according to
 ** IEEE 802.3-2015 chapter 22.2.4.3.1 PHY Identifer.
 **
 ** \param[in]  TrcvIdx        Index of the transceiver within the context of the Ethernet Interface
 ** \param[out] OrgUniqueIdPtr Pointer to the memory where the Organizationally Unique Identifier
 **                            shall be stored.
 ** \param[out] ModelNrPtr     Pointer to the memory where the Manufacturer's Model Number
 **                            shall be stored.
 ** \param[out] RevisionNrPtr  Pointer to the memory where the Revision Number shall be stored.
 **
 ** \return Standard    Return Code
 ** \retval E_OK        The request has been accepted.
 ** \retval E_NOT_OK    The request has not been accepted.
 **
 ** \ServiceID{0x15}
 ** \Reentrancy{Reentrant for different TrcvIdx. Non reentrant for the same TrcvIdx.}
 ** \Synchronicity{synchronous}
 **
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetPhyIdentifier
(
  uint8 TrcvIdx,
  P2VAR(uint32, AUTOMATIC, ETHIF_APPL_DATA) OrgUniqueIdPtr,
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA) ModelNrPtr,
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA) RevisionNrPtr
);

/** \brief Activates a given test mode.
 **
 ** This function activates a given test mode of the link of the indexed transceiver.
 **
 ** \param[in] TrcvIdx  Index of the transceiver within the context of the Ethernet Interface
 ** \param[in] Mode     Test mode to be activated:
 **                     ETHTRCV_PHYTESTMODE_NONE - Normal operation
 **                     ETHTRCV_PHYTESTMODE_1 - Test transmitter droop
 **                     ETHTRCV_PHYTESTMODE_2 - Test master timing jitter
 **                     ETHTRCV_PHYTESTMODE_3 - Test slave timing jitter
 **                     ETHTRCV_PHYTESTMODE_4 - Test transmitter distortion
 **                     ETHTRCV_PHYTESTMODE_5 - Test power spectral density (psd) mask
 **
 ** \return Standard    Return Code
 ** \retval E_OK        The request has been accepted.
 ** \retval E_NOT_OK    The request has not been accepted.
 **
 ** \ServiceID{0x17}
 ** \Reentrancy{Reentrant for different TrcvIdx. Non reentrant for the same TrcvIdx.}
 ** \Synchronicity{synchronous}
 **
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetPhyTestMode
(
  uint8 TrcvIdx,
  EthTrcv_PhyTestModeType Mode
);

#endif /*((ETHIF_TRCV_SUPPORT_ENABLE == STD_ON) && ((ETHIF_ETH_SUPPORT_API != ETHIF_ETH_SUPPORT_API_ASR403) && \
          (ETHIF_ETH_SUPPORT_API != ETHIF_ETH_SUPPORT_API_ASR422)))*/

#if(ETHIF_MII_API_ENABLE == STD_ON)
/**
 * \brief Service to write into an Ethernet transceiver register.
 *
 * \param[in] CtrlIdx - EthIf controller index.
 * \param[in] RegIdx - Register index of the TrcvIdx assigned to the CtrlIdx.
 * \param[in] RegVal - Value which shall be written to the RegIdx .
 *
 * \ServiceID{0x71}
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_WriteMii
(
  uint8 CtrlIdx,
  uint8 RegIdx,
  uint16 RegVal
);

/**
 * \brief Service to read from an Ethernet transceiver register.
 *
 * \param[in] CtrlIdx - EthIf controller index.
 * \param[in] RegIdx - Register index of the TrcvIdx assigned to the CtrlIdx.
 * \param[out] RegValPtr - Pointer containing the value of the RegIdx .
 *
 * \ServiceID{0x70}
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_ReadMii
(
  uint8 CtrlIdx,
  uint8 RegIdx,
  P2VAR(uint16, AUTOMATIC, ETHIF_APPL_DATA) RegValPtr
);
#endif /* #if(ETHIF_MII_API_ENABLE == STD_ON) */

#if (ETHIF_GETPORTMACADDR_API == STD_ON)
/**
 * \brief Service to obtain the port over which this MAC-address can be reached
 *
 * \param[in] MacAddrPtr - MAC-address for which a switch port is searched over
 *                         which the node with this MAC-address can be reached.
 * \param[out] SwitchIdxPtr - Pointer to the switch index
 * \param[out] PortIdxPtr - Pointer to the port index
 *
 * \ServiceID{0x28}
 */
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetPortMacAddr
(
  P2CONST(uint8, AUTOMATIC, ETHIF_APPL_DATA) MacAddrPtr,
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA) SwitchIdxPtr,
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA) PortIdxPtr
);
#endif /* ETHIF_GETPORTMACADDR_API == STD_ON */

#if (ETHIF_GETARLTABLE_API == STD_ON)
/**
 * \brief Service to obtain the address resolution table of a switch.
 *
 * \param[in] SwitchIdx - Index of the switch within the context of the Ethernet Switch Driver
 * \param[inout] numberOfElements - in: Maximum number of elements which can be written into
 *                                      the arlTable.
 *                                  out: Number of elements which are currently available in the
 *                                       EthSwitch module.
 * \param[out] ArlTable - Returns the ARL table of the switch consisting of a list of structs
 *                        with MAC-address, VLAN-ID and port.
 *
 * \ServiceID{0x29}
 */
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetArlTable
(
  uint8 SwitchIdx,
  P2VAR(uint16, AUTOMATIC, ETHIF_APPL_DATA) numberOfElements,
  P2VAR(Eth_MacVlanType, AUTOMATIC, ETHIF_APPL_DATA) arlTableListPointer
);
#endif /* ETHIF_GETARLTABLE_API == STD_ON */

#if (ETHIF_GETBUFFERLEVEL_API == STD_ON)
/**
 * \brief Service to read the buffer level of the corresponding switch. Whether this buffer
 *  level is one value for the entire switch (shared memory) or one value for each port at a
 *  switch is technology dependent.
 *
 * \param[in] SwitchIdx - Index of the switch within the context of the Ethernet Switch Driver
 * \param[out] SwitchBufferLevelPtr - The interpretation of this value is switch dependent
 *
 * \ServiceID{0x2A}
 */
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetBufferLevel
(
  uint8 SwitchIdx,
  P2VAR(uint32, AUTOMATIC, ETHIF_APPL_DATA) SwitchBufferLevelPtr
);
#endif /* ETHIF_GETBUFFERLEVEL_API == STD_ON */

#if (ETHIF_SWTGETCOUNTERVALUES_API == STD_ON)
/**
 * \brief Service to read a list with drop counter values of the corresponding switch. The meaning
 *  of these values is switch dependent and can include values like 1.) dropped packets due to
 *  buffer overrun, 2.) dropped packets due to CRC errors, etc.
 *
 * \param[in] SwitchIdx - Index of the switch within the context of the Ethernet Switch Driver
 *
 * \ServiceID{0x40}
 */
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwtGetCounterValues
(
  uint8 SwitchIdx,
  uint8 SwitchPortIdx,
  P2VAR(Eth_CounterType, AUTOMATIC, ETHIF_APPL_DATA) CounterPtr
);
#endif /* ETHIF_SWTGETCOUNTERVALUES_API == STD_ON */

#if (ETHIF_STORECONFIGURATION_API == STD_ON)
/**
 * \brief Service to store the configuration of the learned MAC/Port tables of a switch in a
 *  persistent manner and will be used by e.g. CDD.
 *
 * \param[in] SwitchIdx - Index of the switch within the context of the Ethernet Switch Driver
 *
 * \ServiceID{0x2C}
 */
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_StoreConfiguration
(
  uint8 SwitchIdx
);
#endif /* ETHIF_STORECONFIGURATION_API == STD_ON */

#if (ETHIF_RESETCONFIGURATION_API == STD_ON)
/**
 * \brief Service to reset the configuration of the learned MAC/Port tables of a switch in a
 *  persistent manner and will be used by e.g. CDD. The statically configured entries shall
 *  still remain.
 *
 * \param[in] SwitchIdx - Index of the switch within the context of the Ethernet Switch Driver
 *
 * \ServiceID{0x2D}
 */
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_ResetConfiguration
(
  uint8 SwitchIdx
);
#endif /* ETHIF_RESETCONFIGURATION_API == STD_ON */

#if (ETHIF_WAKEUPSUPPORT_API == STD_ON)
/**
 * \brief   Sets the transceiver wake up mode.
 *
 * This service enables / disables the wake up mode or clear the wake-up reason
 * of the specified transceiver.
 *
 * \param[in] TrcvIdx             Index of the transceiver within the context of the
 *                                Ethernet Interface
 * \param[in] TrcvWakeupMode      Transceiver wake up mode:
 *                                ETHTRCV_WUM_DISABLE - Disable transceiver wake up
 *                                ETHTRCV_WUM_ENABLE - Enable transceiver wake up
 *                                ETHTRCV_WUM_CLEAR - Clears transceiver wake up reason
 *
 * \returns Std_ReturnType
 * \retval  E_OK     The request was successful.
 * \retval  E_NOT_OK Transceiver wake up could not be changed or wake-up reason could
 *                   not be cleared.
 * \ServiceID{0x2E}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetTransceiverWakeupMode
(
  uint8 TrcvIdx,
  EthTrcv_WakeupModeType TrcvWakeupMode
);
#endif /* ETHIF_WAKEUPSUPPORT_API == STD_ON */


#if (ETHIF_GETTRANSCEIVERWAKEUPMODE_API == STD_ON)
/**
 * \brief   Gets the transceiver wake up mode.
 *
 * This service returns the wake up mode of the indexed transceiver.
 *
 * \param[in] TrcvIdx             Index of the transceiver within the context of the
 *                                Ethernet Interface.
 * \param[out] TrcvWakeupModePtr  Pointer where transceiver wake up mode will be written:
 *                                ETHTRCV_WUM_DISABLE - Transceiver wake up is disabled
 *                                ETHTRCV_WUM_ENABLE - Transceiver wake up is enabled.
 *
 * \returns Std_ReturnType
 * \retval  E_OK     The request was successful.
 * \retval  E_NOT_OK Transceiver wake up mode could not be obtained.
 * \ServiceID{0x2F}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetTransceiverWakeupMode
(
  uint8 TrcvIdx,
  P2VAR(EthTrcv_WakeupModeType, AUTOMATIC, ETHIF_APPL_DATA) TrcvWakeupModePtr
);
#endif /* ETHIF_GETTRANSCEIVERWAKEUPMODE_API == STD_ON */

#if (ETHIF_WAKEUPSUPPORT_API == STD_ON)
/**
 * \brief   Check a wake up source.
 *
 * This service is called by integration code to check a wake up source.
 *
 * \param[in] WakeupSource        Source (transceiver) which initiated the wake up event.
 *
 * \returns Std_ReturnType
 * \retval  E_OK     The function has been successfully executed.
 * \retval  E_NOT_OK The function could not be successfully executed.
 * \ServiceID{0x30}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_CheckWakeup
(
  EcuM_WakeupSourceType WakeupSource
);
#endif /* ETHIF_WAKEUPSUPPORT_API == STD_ON */

#if (ETHIF_GETCTRLIDXLIST_API == STD_ON)
/**
 * \brief   Gets active Ethernet controllers.
 *
 * This service returns the number and index of all active Ethernet controllers.
 *
 * \param[inout] NumberOfCtrlIdx  in: Maximum number of controllers in CtrlIdxListPtr, 0 to return
 *                                    the number of controllers but without filling CtrlIdxListPtr.
 *                                out: Number of active controllers.
 * \param[out] CtrlIdxListPtr     Pointer to a list of active controller indexes.
 *
 * \returns Std_ReturnType
 * \retval  E_OK     The function has been successfully executed.
 * \retval  E_NOT_OK The function could not be successfully executed.
 * \ServiceID{0x44}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetCtrlIdxList
(
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA) NumberOfCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA) CtrlIdxListPtr
);
#endif /* ETHIF_GETCTRLIDXLIST_API == STD_ON */


#if (ETHIF_GETVLANIDSUPPORT_API == STD_ON)
/**
 * \brief   Gets Ethernet controller VLAN identifier.
 *
 * This service returns the VLAN identifier of the requested Ethernet controller.
 *
 * \param[in] CtrlIdx             Index of the Ethernet controller within the context of the
 *                                Ethernet Interface.
 * \param[out] VlanIdPtr          Pointer to store the VLAN identifier (VID) of the Ethernet
 *                                controller. 0 if the the Ethernet controller represents no
 *                                virtual network (VLAN).
 *
 * \returns Std_ReturnType
 * \retval  E_OK     The function has been successfully executed.
 * \retval  E_NOT_OK The function could not be successfully executed.
 * \ServiceID{0x43}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetVlanId
(
  uint8 CtrlIdx,
  P2VAR(uint16, AUTOMATIC, ETHIF_APPL_DATA) VlanIdPtr
);
#endif /* ETHIF_GETVLANIDSUPPORT_API == STD_ON */

#if (ETHIF_GETANDRESETMEASUREMENTDATA_API == STD_ON)
/**
 * \brief   Reads and resets measurement data.
 *
 * This service allows to read and reset detailed measurement data for diagnostic purposes.
 * Get all MeasurementIdx's at once is not supported. ETHIF_MEAS_ALL shall only be used to reset
 * all MeasurementIdx's at once. A NULL_PTR shall be provided for MeasurementDataPtr in this case.
 *
 * \param[in] MeasurementIdx         Index to select specific measurement data:
 *                                   ETHIF_MEAS_DROP_CRTLIDX (0x01) - Measurement index of dropped
 *                                   datagrams caused by invalid CrtlIdx/VLAN.
 *                                   ETHIF_MEAS_RESERVED_1 (0x02-0x7F) - Reserved by AUTOSAR.
 *                                   ETHIF_MEAS_RESERVED_2 (0x80-0xEF) - Vendor specific range.
 *                                   ETHIF_MEAS_RESERVED_3 (0xF0-0xFE) - Reserved by AUTOSAR
 *                                                                       (future use).
 *                                   ETHIF_MEAS_ALL (0xFF) - Represents all measurement indexes.
 * \param[in] MeasurementResetNeeded Flag to trigger a reset of the measurement data.
 * \param[out] MeasurementDataPtr    Pointer to data buffer, where to copy measurement data.
 *
 * \returns Std_ReturnType
 * \retval  E_OK     The function has been successfully executed.
 * \retval  E_NOT_OK The function could not be successfully executed.
 * \ServiceID{0x45}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetAndResetMeasurementData
(
  EthIf_MeasurementIdxType MeasurementIdx,
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, ETHIF_APPL_DATA) MeasurementDataPtr
);
#endif /* ETHIF_GETANDRESETMEASUREMENTDATA_API == STD_ON */

/** \brief Obtains the switch port signal quality.
 **
 ** This function obtains the signal quality of the link for the indexed switch port.
 **
 ** \param[in]  SwitchIdx         Switch index within the context of the
 **                               Ethernet Interface.
 ** \param[in]  SwitchPortIdx     Switch port index within the context of the
 **                               Ethernet Interface.
 ** \param[out] ResultPtr  Pointer to the memory where the signal quality shall be stored.
 **
 ** \return Std_ReturnType
 ** \retval E_OK        The request has been accepted.
 ** \retval E_NOT_OK    The request has not been accepted.
 **
 ** \ServiceID{none}
 ** \Reentrancy{Reentrant for different SwitchIdx and SwitchPortIdx.
                Non reentrant for the same SwitchIdx and SwitchPortIdx.}
 ** \Synchronicity{synchronous}
 **
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetSwitchPortSignalQuality
(
  uint8 SwitchIdx,
  uint8 SwitchPortIdx,
  P2VAR(EthIf_SignalQualityResultType, AUTOMATIC, ETHIF_APPL_DATA) ResultPtr
);

/** \brief Clears stored signal quality.
 **
 ** This function clears the stored signal quality of the link for the indexed switch port.
 **
 ** \param[in]  SwitchIdx         Switch index within the context of the
 **                               Ethernet Interface.
 ** \param[in]  SwitchPortIdx     Switch port index within the context of the
 **                               Ethernet Interface.
 **
 ** \return Std_ReturnType
 ** \retval E_OK        The request has been accepted.
 ** \retval E_NOT_OK    The request has not been accepted.
 **
 ** \ServiceID{none}
 ** \Reentrancy{Reentrant for different SwitchIdx and SwitchPortIdx.
                Non reentrant for the same SwitchIdx and SwitchPortIdx.}
 ** \Synchronicity{synchronous}
 **
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_ClearSwitchPortSignalQuality
(
  uint8 SwitchIdx,
  uint8 SwitchPortIdx
);

/**
 * \brief   Requests the mode of EthIfSwtPortGroup.
 *
 * This service request a mode for the EthIfSwtPortGroup. The call is forwarded to EthSwt by
 * calling EthSwt_SetSwitchPortMode for all EthSwtPorts referenced by the port group.
 *
 * \param[in] PortGroupIdx        Index of the port group within the context of the Eth. Interface.
 * \param[in] PortMode            Requested port mode:
 *                                ETHTRCV_MODE_DOWN - Disables the port group.
 *                                ETHTRCV_MODE_ACTIVE - Enables the port group.
 * \returns Std_ReturnType
 * \retval  E_OK     Success.
 * \retval  E_NOT_OK Port group mode could not be changed.
 * \ServiceID{0x06}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchPortGroupRequestMode
(
  EthIf_SwitchPortGroupIdxType PortGroupIdx,
  EthTrcv_ModeType PortMode
);

/**
 * \brief   Requests to start all configured ports.
 *
 * This service requests to start all configured ports.
 *
 * \returns Std_ReturnType
 * \retval  E_OK     Success.
 * \retval  E_NOT_OK Port mode could not be started.
 * \ServiceID{0x07}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_StartAllPorts
(
  void
);

#if (ETHIF_VERIFYCONFIG_API == STD_ON)
/**
 * \brief   Verifies the Switch Configuration.
 *
 * This service verifies the Switch Configuration depending on the  HW-Architecture,
 * HW-capability and the intended accuracy of this verification. Forwarded to EthSwt_VerifyConfig.
 *
 * \param[in] SwitchIdx           Index of the switch within the context of the Eth. Switch Driver.
 * \param[out] Result             Pointer to the result of verification:
 *                                TRUE - Configuration verified ok.
 *                                FALSE - Configuration values found corrupted.
 * \returns Std_ReturnType
 * \retval  E_OK     Configuration verification succeeded.
 * \retval  E_NOT_OK Configuration verification not succeeded.
 * \ServiceID{0x40}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_VerifyConfig
(
  uint8 SwitchIdx,
  P2VAR(boolean, AUTOMATIC, ETHIF_APPL_DATA) Result
);
#endif /* ETHIF_VERIFYCONFIG_API == STD_ON */

#if (ETHIF_SETFORWARDINGMODE_API == STD_ON)
/**
 * \brief   Sets the frame forwarding mode.
 *
 * This service sets the frame forwarding mode.
 *
 * \param[in] SwitchIdx           Index of the switch within the context of the Eth. Switch Driver.
 * \param[in] mode                Frame forwarding mode:
 *                                TRUE - Forwarding enabled.
 *                                FALSE - Forwarding disabled.
 * \returns Std_ReturnType
 * \retval  E_OK     Stopping of frame forwarding succeeded
 * \retval  E_NOT_OK stopping of frame forwarding not succeeded.
 * \ServiceID{0x41}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetForwardingMode
(
  uint8 SwitchIdx,
  boolean mode
);
#endif /* ETHIF_SETFORWARDINGMODE_API == STD_ON */

#if (ETHIF_DEVICEAUTHENTICATION_API == STD_ON)
/**
 * \brief    Within a group of EthIfControllers that reference the same PhyController,
 *           enable all but one received as a parameter.
 *
 *
 * \param[in] CtrlIdx             Index of the controller within the context of the EthIf.
 *
 *
 * \returns Std_ReturnType
 * \retval  E_OK
 * \retval  E_NOT_OK
 * \ServiceID{0xF0}
 * \Reentrancy{}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_EnableRelatedEthIfCtrls
(
  uint8 CtrlIdx
);

/**
 * \brief    Within a group of EthIfControllers that reference the same PhyController,
 *           disable all but one received as a parameter.
 *
 *
 * \param[in] CtrlIdx             Index of the controller within the context of the EthIf.
 *
 * \returns Std_ReturnType
 * \retval  E_OK
 * \retval  E_NOT_OK
 * \ServiceID{0xF1}
 * \Reentrancy{}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) EthIf_DisableRelatedEthIfCtrls
(
  uint8 CtrlIdx
);
#endif /* ETHIF_DEVICEAUTHENTICATION_API == STD_ON */

#define ETHIF_STOP_SEC_CODE
#include <EthIf_MemMap.h>
/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef ETHIF_H */
/*==================[end of file]===========================================*/
