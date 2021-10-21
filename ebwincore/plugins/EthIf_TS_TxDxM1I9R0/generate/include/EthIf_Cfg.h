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

[!CODE!]
[!INCLUDE "../../generate_macros/EthIf_CalcToTicks.m"!][!//
[!INCLUDE "../../generate_macros/EthIf_Cfg.m"!][!//
#ifndef ETHIF_CFG_H
#define ETHIF_CFG_H

/*==================[includes]==============================================*/

#include <TSAutosar.h>              /* Global configuration */
#include <EthIf_DefProg_Cfg.h>

/*==================[macros]================================================*/

#define ETHIF_ASR403    0U
#define ETHIF_ASR422    1U
#define ETHIF_ASR430    2U

#if (defined ETHIF_DEV_ERROR_DETECT)
#error ETHIF_DEV_ERROR_DETECT is already defined
#endif
/** \brief Pre-processor switch for enabling development error detection.
 */
#define ETHIF_DEV_ERROR_DETECT        [!IF "as:modconf('EthIf')[1]/EthIfGeneral/EthIfDevErrorDetect = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Switch for DEM to DET reporting for production errors  */
#define ETHIF_ERR_HANDLING_TEST   [!//
[!IF "as:modconf('EthIf')[1]/EthIfGeneral/ReportToDem/EthIfDemCtrlTestResultReportToDem = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "as:modconf('EthIf')[1]/EthIfGeneral/ReportToDem/EthIfDemCtrlTestResultReportToDem = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!//

[!IF "as:modconf('EthIf')[1]/EthIfGeneral/ReportToDem/EthIfDemCtrlTestResultReportToDem = 'DET'"!][!//
/** \brief Det error ID, if DEM to DET reporting is enabled */
#define ETHIF_E_DEMTODET_CTRL_TEST          [!"as:modconf('EthIf')[1]/EthIfGeneral/ReportToDem/EthIfDemCtrlTestResultReportToDemDetErrorId"!]U
[!ENDIF!][!//

#if (defined ETHIF_GET_VERSION_INFO)
#error ETHIF_GET_VERSION_INFO is already defined
#endif
/** \brief Pre-processor switch for enabling version info API support.
 */
#define ETHIF_GET_VERSION_INFO        [!IF "as:modconf('EthIf')[1]/EthIfGeneral/EthIfVersionInfoApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHIF_VLAN_SUPPORT_ENABLE)
#error ETHIF_VLAN_SUPPORT_ENABLE is already defined
#endif
/** \brief Pre-processor switch for enabling VLAN support.
 */
#define ETHIF_VLAN_SUPPORT_ENABLE     [!IF "$EthIf_VlanSupportEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHIF_TRCV_SUPPORT_ENABLE)
#error ETHIF_TRCV_SUPPORT_ENABLE is already defined
#endif
/** \brief Pre-processor switch for enabling Trcv support.
 */
#define ETHIF_TRCV_SUPPORT_ENABLE     [!IF "$EthIf_TrcvSupportEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHIF_TRCV_ASYNC_ENABLE)
#error ETHIF_TRCV_ASYNC_ENABLE is already defined
#endif
/** \brief Pre-processor switch for enabling async Trcv mode.
 */
#define ETHIF_TRCV_ASYNC_ENABLE     [!IF "as:modconf('EthIf')[1]/EthIfGeneral/EthIfAsyncEthTrcvModeSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHIF_SWT_SUPPORT_ENABLE)
#error ETHIF_SWT_SUPPORT_ENABLE is already defined
#endif
/** \brief Pre-processor switch for enabling Swt support.
 */
#define ETHIF_SWT_SUPPORT_ENABLE      [!IF "num:i(count(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfSwitch/*)) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHIF_VIRTUALCTRL_SUPPORT_ENABLE)
#error ETHIF_VIRTUALCTRL_SUPPORT_ENABLE is already defined
#endif
/** \brief Pre-processor switch for enabling virtual EthIfCtrl support.
 */
#define ETHIF_VIRTUALCTRL_SUPPORT_ENABLE [!IF "as:modconf('EthIf')[1]/EthIfGeneral/EthIfVirtualCtrlSupportEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Pre-processor switch for Eth API version.
 */
#if (defined ETHIF_ETH_SUPPORT_API_ASR403)
#error ETHIF_ETH_SUPPORT_API_ASR403 is already defined
#endif
#define ETHIF_ETH_SUPPORT_API_ASR403        0U

#if (defined ETHIF_ETH_SUPPORT_API_ASR422)
#error ETHIF_ETH_SUPPORT_API_ASR422 is already defined
#endif
#define ETHIF_ETH_SUPPORT_API_ASR422        1U

#if (defined ETHIF_ETH_SUPPORT_API_ASR430)
#error ETHIF_ETH_SUPPORT_API_ASR430 is already defined
#endif
#define ETHIF_ETH_SUPPORT_API_ASR430        2U

#if (defined ETHIF_ETH_SUPPORT_API_ASR430_EB)
#error ETHIF_ETH_SUPPORT_API_ASR430_EB is already defined
#endif
#define ETHIF_ETH_SUPPORT_API_ASR430_EB     3U

#if (defined ETHIF_ETH_SUPPORT_API)
#error ETHIF_ETH_SUPPORT_API is already defined
#endif
#define ETHIF_ETH_SUPPORT_API           ETHIF_ETH_SUPPORT_API_[!"$EthIf_EthSupportApi"!]

#if (defined ETHIF_ETH_UPDATEPHYSADDRFILTER_API_ENABLE)
#error ETHIF_ETH_UPDATEPHYSADDRFILTER_API_ENABLE is already defined
#endif
/** \brief Pre-processor switch for Eth_UpdatePhysAddrFilter API.
 */
#define ETHIF_ETH_UPDATEPHYSADDRFILTER_API_ENABLE [!IF "$EthIf_UpdatePhysAddrFilterApiEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHIF_ETH_SETPHYSADDR_API_ENABLE)
#error ETHIF_ETH_SETPHYSADDR_API_ENABLE is already defined
#endif
/** \brief Pre-processor switch for Eth_UpdatePhysAddrFilter API. */
#define ETHIF_ETH_SETPHYSADDR_API_ENABLE [!IF "$EthIf_SetPhysAddrApiEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHIF_MII_API_ENABLE)
#error ETHIF_MII_API_ENABLE is already defined
#endif
/** \brief Pre-processor switch for EthIf_ReadMii and EthIf_WriteMii API. */
#define ETHIF_MII_API_ENABLE [!IF "$EthIf_MiiApiEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHIF_ETH_ASYNC_ENABLE)
#error ETHIF_ETH_ASYNC_ENABLE is already defined
#endif
/** \brief Pre-processor switch for enabling async Eth physical controller mode.
 */
#define ETHIF_ETH_ASYNC_ENABLE     [!IF "as:modconf('EthIf')[1]/EthIfGeneral/EthIfAsyncEthCtrlModeSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHIF_GLOBAL_TIME_SYNC_API_ENABLE)
#error ETHIF_GLOBAL_TIME_SYNC_API_ENABLE is already defined
#endif
/** \brief Pre-processor switch for all global time synch API. */
#define ETHIF_GLOBAL_TIME_SYNC_API_ENABLE [!IF "$EthIf_GlobalTimeApiEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHIF_GETPORTMACADDR_API)
#error ETHIF_GETPORTMACADDR_API is already defined
#endif
/** \brief Pre-processor switch for EthIf_GetPortMacAddr API. */
#define ETHIF_GETPORTMACADDR_API [!IF "as:modconf('EthIf')[1]/EthIfGeneral/EthIfGetPortMacAddrApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHIF_GETARLTABLE_API)
#error ETHIF_GETARLTABLE_API is already defined
#endif
/** \brief Pre-processor switch for EthIf_GetArlTable API. */
#define ETHIF_GETARLTABLE_API [!IF "as:modconf('EthIf')[1]/EthIfGeneral/EthIfGetArlTableApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHIF_GETBUFFERLEVEL_API)
#error ETHIF_GETBUFFERLEVEL_API is already defined
#endif
/** \brief Pre-processor switch for EthIf_GetBufferLevel API. */
#define ETHIF_GETBUFFERLEVEL_API [!IF "as:modconf('EthIf')[1]/EthIfGeneral/EthIfGetBufferLevelApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHIF_STORECONFIGURATION_API)
#error ETHIF_STORECONFIGURATION_API is already defined
#endif
/** \brief Pre-processor switch for EthIf_StoreConfiguration API. */
#define ETHIF_STORECONFIGURATION_API [!IF "as:modconf('EthIf')[1]/EthIfGeneral/EthIfStoreConfigurationApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHIF_RESETCONFIGURATION_API)
#error ETHIF_RESETCONFIGURATION_API is already defined
#endif
/** \brief Pre-processor switch for EthIf_ResetConfiguration API. */
#define ETHIF_RESETCONFIGURATION_API [!IF "as:modconf('EthIf')[1]/EthIfGeneral/EthIfResetConfigurationApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHIF_SWTGETCOUNTERVALUES_API)
#error ETHIF_SWTGETCOUNTERVALUES_API is already defined
#endif
/** \brief Pre-processor switch for EthIf_SwtGetCounterValues API. */
#define ETHIF_SWTGETCOUNTERVALUES_API [!IF "as:modconf('EthIf')[1]/EthIfGeneral/EthIfSwtGetCounterValuesApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHIF_WAKEUPSUPPORT_API)
#error ETHIF_WAKEUPSUPPORT_API is already defined
#endif
/** \brief Pre-processor switch for EthIf_SetTransceiverWakeupMode and EthIf_CheckWakeup API. */
#define ETHIF_WAKEUPSUPPORT_API [!IF "$EthIf_WakeUpSupportApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHIF_GETCTRLIDXLIST_API)
#error ETHIF_GETCTRLIDXLIST_API is already defined
#endif
/** \brief Pre-processor switch for EthIf_GetCtrlIdxList API. */
#define ETHIF_GETCTRLIDXLIST_API [!IF "as:modconf('EthIf')[1]/EthIfGeneral/EthIfGetCtrlIdxList = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHIF_GETVLANIDSUPPORT_API)
#error ETHIF_GETVLANIDSUPPORT_API is already defined
#endif
/** \brief Pre-processor switch for EthIf_GetVlanId API. */
#define ETHIF_GETVLANIDSUPPORT_API [!IF "as:modconf('EthIf')[1]/EthIfGeneral/EthIfGetVlanIdSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHIF_GETANDRESETMEASUREMENTDATA_API)
#error ETHIF_GETANDRESETMEASUREMENTDATA_API is already defined
#endif
/** \brief Pre-processor switch for EthIf_GetAndResetMeasurementData API. */
#define ETHIF_GETANDRESETMEASUREMENTDATA_API [!IF "as:modconf('EthIf')[1]/EthIfGeneral/EthIfGetAndResetMeasurementDataApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHIF_VERIFYCONFIG_API)
#error ETHIF_VERIFYCONFIG_API is already defined
#endif
/** \brief Pre-processor switch for EthIf_VerifyConfig API. */
#define ETHIF_VERIFYCONFIG_API [!IF "as:modconf('EthIf')[1]/EthIfGeneral/EthIfVerifyConfigApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHIF_SETFORWARDINGMODE_API)
#error ETHIF_SETFORWARDINGMODE_API is already defined
#endif
/** \brief Pre-processor switch for EthIf_SetForwardingMode API. */
#define ETHIF_SETFORWARDINGMODE_API [!IF "as:modconf('EthIf')[1]/EthIfGeneral/EthIfSetForwardingModeApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHIF_GETTRANSCEIVERWAKEUPMODE_API)
#error ETHIF_GETTRANSCEIVERWAKEUPMODE_API is already defined
#endif
/** \brief Pre-processor switch for EthIf_GetTransceiverWakeupMode API. */
#define ETHIF_GETTRANSCEIVERWAKEUPMODE_API [!IF "$EthIf_GetTransceiverWakeupModeApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
#if (defined ETHIF_DEVICEAUTHENTICATION_API)
#error ETHIF_DEVICEAUTHENTICATION_API is already defined
#endif
/** \brief Pre-processor switch for EthIf_EnableRelatedEthIfCtrls and EthIf_DisableRelatedEthIfCtrls API. */
#define ETHIF_DEVICEAUTHENTICATION_API [!IF "$EthIf_DeviceAuthenticationApiEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHIF_RETRANSMIT_API)
#error ETHIF_RETRANSMIT_API is already defined
#endif
/** \brief Pre-processor switch for EthIf_Retransmit API. */
#define ETHIF_RETRANSMIT_API [!IF "$EthIf_EthIfRetransmitApiEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHIF_SWITCHMANAGEMENTSUPPORT)
#error ETHIF_SWITCHMANAGEMENTSUPPORT is already defined
#endif
/** \brief Pre-processor switch for switch management APIs to support a Switch-port specific
           communication attribute access. */
#define ETHIF_SWITCHMANAGEMENTSUPPORT [!IF "as:modconf('EthIf')[1]/EthIfGeneral/EthIfSwitchManagementSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHIF_MAINFUNCTIONSTATE_API_ENABLE)
#error ETHIF_MAINFUNCTIONSTATE_API_ENABLE is already defined
#endif
/** \brief Pre-processor switch for EthIf_MainFunctionState API.
 */
#define ETHIF_MAINFUNCTIONSTATE_API_ENABLE    [!//
[!IF "node:exists(EthIfGeneral/EthIfMainFunctionStatePeriod)"!][!//
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!][!//

#if (defined ETHIF_MAINFUNCTIONSTATE_IS_STATIC)
#error ETHIF_MAINFUNCTIONSTATE_IS_STATIC is already defined
#endif
/** \brief macro to determine if  EthIf_MainFunctionState() is external API or not */
#define ETHIF_MAINFUNCTIONSTATE_IS_STATIC    [!//
[!IF "node:exists(EthIfGeneral/EthIfMainFunctionStatePeriod)"!][!//

[!ELSE!][!//
STATIC
[!ENDIF!][!//

#if (defined ETHIF_RX_INDICATION_ITERATIONS)
#error ETHIF_RX_INDICATION_ITERATIONS is already defined
#endif
/** \brief Maximum number of RxINdication per controller per EthIf_MainFunctionRx iteration.
 */
#define ETHIF_RX_INDICATION_ITERATIONS [!"num:i(as:modconf('EthIf')[1]/EthIfGeneral/EthIfRxIndicationIterations)"!]U

#if (defined ETHIF_SET_MODE_TIMEOUT)
#error ETHIF_SET_MODE_TIMEOUT is already defined
#endif
/** \brief Time duration after which set mode operation will be repeated
 */
[!IF "node:exists(EthIfGeneral/EthIfSetModeTimeout)"!]
#define ETHIF_SET_MODE_TIMEOUT           [!CALL "CalcToTicks", "time"="EthIfGeneral/EthIfSetModeTimeout", "maxTicks"="65535"!]U
[!ELSE!][!//
#define ETHIF_SET_MODE_TIMEOUT           0U
[!ENDIF!][!//

#if (defined ETHIF_SWITCH_OFF_PORT_TIMEDELAY)
#error ETHIF_SWITCH_OFF_PORT_TIMEDELAY is already defined
#endif
/** \brief Time delay after the mode "ETHTRCV_MODE_DOWN" of a EthIfSwitchPortGroup will be executed.
 */
[!IF "node:exists(EthIfGeneral/EthIfSwitchOffPortTimeDelay)"!]
#define ETHIF_SWITCH_OFF_PORT_TIMEDELAY           [!CALL "CalcToTicks", "time"="EthIfGeneral/EthIfSwitchOffPortTimeDelay", "maxTicks"="65535"!]U
[!ELSE!][!//
#define ETHIF_SWITCH_OFF_PORT_TIMEDELAY           0U
[!ENDIF!][!//

#if (defined ETHIF_PORT_STARTUP_ACTIVE_TIME)
#error ETHIF_PORT_STARTUP_ACTIVE_TIME is already defined
#endif
/** \brief Time delay to switch off ports started with EthIf_StartAllPorts()
 */
[!IF "node:exists(EthIfGeneral/EthIfPortStartupActiveTime)"!]
#define ETHIF_PORT_STARTUP_ACTIVE_TIME           [!CALL "CalcToTicks", "time"="EthIfGeneral/EthIfPortStartupActiveTime", "maxTicks"="65535"!]U
[!ELSE!][!//
#define ETHIF_PORT_STARTUP_ACTIVE_TIME           0U
[!ENDIF!][!//

#if (defined ETHIF_ETH_BSWIMPL)
#error ETHIF_ETH_BSWIMPL is already defined
#endif
/** \brief Number of configured EthIfEthControllerBswmdImplementationRefs.
 */
#define ETHIF_ETH_BSWIMPL [!"num:i(count(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfEthControllerType/*/EthIfEthControllerBswmdImplementationRefs))"!]U
#if (defined ETHIF_ETHTRCV_BSWIMPL)
#error ETHIF_ETHTRCV_BSWIMPL is already defined
#endif
/** \brief Number of configured EthIfEthTrcvBswmdImplementationRefs.
 */
#define ETHIF_ETHTRCV_BSWIMPL [!"num:i(count(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfEthTrcvType/*/EthIfEthTrcvBswmdImplementationRefs))"!]U
#if (defined ETHIF_ETHSWT_BSWIMPL)
#error ETHIF_ETHSWT_BSWIMPL is already defined
#endif
/** \brief Number of configured EthIfEthSwtBswmdImplementationRefs.
 */
#define ETHIF_ETHSWT_BSWIMPL [!"num:i(count(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfEthSwtType/*/EthIfEthSwtBswmdImplementationRefs))"!]U

#if (defined ETHIF_ETHIFCTRL_ARRAYSIZE)
#error ETHIF_ETHIFCTRL_ARRAYSIZE is already defined
#endif
/** \brief Reserved array space for EthIf controllers.
 */
#define ETHIF_ETHIFCTRL_ARRAYSIZE [!"num:i(as:modconf('EthIf')[1]/EthIfGeneral/EthIfMaxCtrl)"!]U

#if (defined ETHIF_PHYCTRL_ARRAYSIZE)
#error ETHIF_PHYCTRL_ARRAYSIZE is already defined
#endif
/** \brief Reserved array space for physical controllers.
 */
#define ETHIF_PHYCTRL_ARRAYSIZE [!"num:i(as:modconf('EthIf')[1]/EthIfGeneral/EthIfMaxPhyCtrl)"!]U

#if (defined ETHIF_TRCV_ARRAYSIZE)
#error ETHIF_TRCV_ARRAYSIZE is already defined
#endif
/** \brief Reserved array space for transceivers.
 */
#define ETHIF_TRCV_ARRAYSIZE [!"num:i(as:modconf('EthIf')[1]/EthIfGeneral/EthIfMaxTrcvsTotal)"!]U

#if (defined ETHIF_PORTGROUPSUPPORT)
#error ETHIF_PORTGROUPSUPPORT is already defined
#endif
/** \brief Pre-processor switch for enabling APIs to support a Switching of port groups
 */
#define ETHIF_PORTGROUPSUPPORT [!IF "$EthIf_PortGroupSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHIF_ETHSWT_ARRAYSIZE)
#error ETHIF_ETHSWT_ARRAYSIZE is already defined
#endif
/** \brief Reserved array space for Ethernet switches
 */
#define ETHIF_ETHSWT_ARRAYSIZE [!"num:i(as:modconf('EthIf')[1]/EthIfGeneral/EthIfMaxEthSwitches)"!]U

#if (defined ETHIF_PORTGROUPS_ARRAYSIZE)
#error ETHIF_PORTGROUPS_ARRAYSIZE is already defined
#endif
/** \brief Reserved array space for switch port groups supported switch.
 *  When EthIfController references Ethernet Switch (not Switch Port Group), it is considered
 *  as a logical Switch Port Group, thus needs to be counted in this parameter.
 */
#define ETHIF_PORTGROUPS_ARRAYSIZE [!"num:i(as:modconf('EthIf')[1]/EthIfGeneral/EthIfMaxSwtPortGroups + as:modconf('EthIf')[1]/EthIfGeneral/EthIfMaxEthSwitches)"!]U

#if (defined ETHIF_PORTS_ARRAYSIZE)
#error ETHIF_PORTS_ARRAYSIZE is already defined
#endif
/** \brief Reserved array space for Ethernet switch ports
 */
#define ETHIF_PORTS_ARRAYSIZE [!"num:i(as:modconf('EthIf')[1]/EthIfGeneral/EthIfMaxSwtPorts)"!]U

#if (defined ETHIF_INIT_CTRL_TRCV)
#error ETHIF_INIT_CTRL_TRCV is already defined
#endif
/** \brief Pre-processor switch to enable / disable EthIf to initialize configured physical controllers and transceivers.
 */
#define ETHIF_INIT_CTRL_TRCV [!IF "as:modconf('EthIf')[1]/EthIfGeneral/EthIfInitControllersTransceivers = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#if (defined ETHIF_ETHSWT_RX_PREPROCESS)
#error ETHIF_ETHSWT_RX_PREPROCESS is already defined
#endif

/** \brief Pre-processor switch to enable / disable the call to EthSwt_EthIfPreProcessRxFrame().
 */
#if(ETHIF_SWT_SUPPORT_ENABLE == STD_ON)
#define ETHIF_ETHSWT_RX_PREPROCESS [!IF "as:modconf('EthIf')[1]/EthIfGeneral/EthIfSwtPreProcessRxFrame = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
#else
#define ETHIF_ETHSWT_RX_PREPROCESS STD_OFF
#endif

#if (defined ETHIF_ETHSWT_TX_ADAPTFRAME)
#error ETHIF_ETHSWT_TX_ADAPTFRAME is already defined
#endif

/** \brief Pre-processor switch to enable / disable the API calls to .
 */
#if(ETHIF_SWT_SUPPORT_ENABLE == STD_ON)
#define ETHIF_ETHSWT_TX_ADAPTFRAME [!IF "as:modconf('EthIf')[1]/EthIfGeneral/EthIfSwtAdpatTxFrame = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
#else
#define ETHIF_ETHSWT_TX_ADAPTFRAME STD_OFF
#endif

#if (defined ETHIF_PORT_STATE_MGMT_ENABLED)
#error ETHIF_PORT_STATE_MGMT_ENABLED is already defined
#endif
/** \brief Pre-processor switch for enabling Ethernet switch port made and link state management
 */
[!IF "$EthIf_PortStateMgmtEnabled = 'true'"!] [!//
#define ETHIF_PORT_STATE_MGMT_ENABLED STD_ON
[!ELSE!] [!//
#define ETHIF_PORT_STATE_MGMT_ENABLED STD_OFF
[!ENDIF!]
/** \brief Compile time verification value */
#if (defined ETHIF_CFG_SIGNATURE)
#error ETHIF_CFG_SIGNATURE already defined!
#endif
#define ETHIF_CFG_SIGNATURE  [!"asc:getConfigSignature(as:modconf('EthIf')[1]//*[not(child::*) and (node:configclass() = 'PreCompile') ])"!]U

/** \brief Compile time Published information verification value */
#if (defined ETHIF_PUBLIC_INFO_SIGNATURE)
#error ETHIF_PUBLIC_INFO_SIGNATURE already defined!
#endif
#define ETHIF_PUBLIC_INFO_SIGNATURE  [!"asc:getConfigSignature(node:difference(as:modconf('EthIf')[1]/CommonPublishedInformation//*[not(child::*) and (node:configclass() = 'PublishedInformation') ], as:modconf('EthIf')[1]/CommonPublishedInformation/Release))"!]U

/** \brief Pre-processor switch indicates if PbcfgM uses EthIf */
#if (defined ETHIF_PBCFGM_SUPPORT_ENABLED)
#error ETHIF_PBCFGM_SUPPORT_ENABLED already defined!
#endif
#define ETHIF_PBCFGM_SUPPORT_ENABLED  [!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('EthIf')) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Pre-processor switch to enable/disable relocatable postbuild config
 */
#if (defined ETHIF_RELOCATABLE_CFG_ENABLE)
#error ETHIF_RELOCATABLE_CFG_ENABLE is already defined
#endif
#define ETHIF_RELOCATABLE_CFG_ENABLE [!//
[!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('EthIf')) = 'true'"!][!//
[!IF "as:modconf('PbcfgM')/PbcfgMGeneral/PbcfgMRelocatableCfgEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!//
[!ELSE!][!IF "EthIfGeneral/EthIfRelocatablePbcfgEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!//
[!ENDIF!]

/** \brief Pre-processor switch indicating that EthIf_IsValidConfig function shall be mapped to memory section ETHIF_CODE_CC_BLOCK
*/
#if (defined ETHIF_ISVALIDCONFIG_MMAP_ENABLED)
#error ETHIF_ISVALIDCONFIG_MMAP_ENABLED already defined!
#endif
#define ETHIF_ISVALIDCONFIG_MMAP_ENABLED [!//
[!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('EthIf')) = 'true'"!][!//
[!IF "node:exists(as:modconf('PbcfgM')[1]/PbcfgMGeneral/PbcfgMMapIsValidFunctionToMemSection) and as:modconf('PbcfgM')[1]/PbcfgMGeneral/PbcfgMMapIsValidFunctionToMemSection = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!//
[!ELSE!]STD_OFF[!//
[!ENDIF!]


/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[includes]==============================================*/

#endif /* ifndef ETHIF_CFG_H */

/*==================[end of file]===========================================*/
[!ENDCODE!][!//
