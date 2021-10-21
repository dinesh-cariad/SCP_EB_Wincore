[!/**
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
 */!][!//
[!// multiple inclusion protection
[!IF "not(var:defined('ETHIF_CFG_M'))"!][!//
[!VAR "ETHIF_CFG_M"="'true'"!][!/*

=== Cfg macros ===

*/!][!//
[!/* Pre-compile switch for enabling Trcv support
*/!][!VAR "EthIf_TrcvSupportEnable" = "as:modconf('EthIf')[1]/EthIfGeneral/EthIfTrcvSupportEnable"!][!//
[!/* Pre-compile switch for enabling VLAN support
*/!][!VAR "EthIf_VlanSupportEnable" = "as:modconf('EthIf')[1]/EthIfGeneral/EthIfVLANSupportEnable"!][!//
[!/* Pre-compile switch for EthIf_ReadMii and EthIf_WriteMii API
*/!][!VAR "EthIf_MiiApiEnable" = "as:modconf('EthIf')[1]/EthIfGeneral/EthIfMiiApiEnable"!][!//
[!/* Pre-compile switch for Eth_SetPhysAddr API
*/!][!VAR "EthIf_SetPhysAddrApiEnable" = "as:modconf('EthIf')[1]/EthIfGeneral/EthIfSetPhysAddrSupportEnable"!][!//
[!/* Pre-compile switch for Eth_UpdatePhysAddrFilter API
*/!][!VAR "EthIf_UpdatePhysAddrFilterApiEnable" = "as:modconf('EthIf')[1]/EthIfGeneral/EthIfUpdatePhysAddrFilterSupportEnable"!][!//
[!/* Pre-compile switch for all global time synch APIs
*/!][!VAR "EthIf_GlobalTimeApiEnable" = "as:modconf('EthIf')[1]/EthIfGeneral/EthIfGlobalTimeSupport"!][!//
[!/* Pre-compile switch for EthIf_EnableRelatedEthIfCtrls and EthIf_DisableRelatedEthIfCtrls API
*/!][!VAR "EthIf_DeviceAuthenticationApiEnable" = "as:modconf('EthIf')[1]/EthIfGeneral/EthIfDeviceAuthenticationApiEnable"!][!//
[!/* Pre-compile switch for EthIf_Retransmit API
*/!][!VAR "EthIf_EthIfRetransmitApiEnable" = "as:modconf('EthIf')[1]/EthIfGeneral/EthIfRetransmitApiEnable"!][!//
[!/* Pre-compile parameter to select version of Eth API
*/!][!VAR "EthIf_EthSupportApi" = "as:modconf('EthIf')[1]/EthIfGeneral/EthIfSupportEthAPI"!][!//
[!/* Holds information if port state management is enabled
*/!][!VAR "EthIf_PortStateMgmtEnabled" = "not((as:modconf('EthIf')[1]/EthIfGeneral/EthIfMaxEthSwitches = 0) or (as:modconf('EthIf')[1]/EthIfGeneral/EthIfMaxSwtPorts = 0))"!][!//
[!/* Pre-processor switch for EthIf_SetTransceiverWakeupMode and EthIf_CheckWakeup API
*/!][!VAR "EthIf_WakeUpSupportApi" = "as:modconf('EthIf')[1]/EthIfGeneral/EthIfWakeUpSupport"!][!//
[!/* Pre-processor switch for enabling APIs to support Switching of port groups
*/!][!VAR "EthIf_PortGroupSupport" = "as:modconf('EthIf')[1]/EthIfGeneral/EthIfSwitchingPortGroupSupport"!][!//
[!/* Pre-processor switch for EthIf_GetTransceiverWakeupMode API
*/!][!VAR "EthIf_GetTransceiverWakeupModeApi" = "node:exists(as:modconf('EthIf')[1]/EthIfGeneral/EthIfGetTransceiverWakeupModeApi) and (as:modconf('EthIf')[1]/EthIfGeneral/EthIfGetTransceiverWakeupModeApi = 'true')"!][!//
[!/* Pre-processor switch for enabling Switch support
*/!][!VAR "EthIf_SwtSupportEnable" = "num:i(count(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfSwitch/*)) > 0"!][!//
[!/* Pre-processor switch for enabling EthSwt_EthIfTxAdaptFrameTypeBufferLength API
*/!][!VAR "EthIf_AdpatTxFrame" = "as:modconf('EthIf')[1]/EthIfGeneral/EthIfSwtAdpatTxFrame"!][!//
[!/* Pre-processor switch for enabling EthSwt_EthIfPreProcessRxFrame API
*/!][!VAR "EthIf_PreProcessRxFrame" = "as:modconf('EthIf')[1]/EthIfGeneral/EthIfSwtPreProcessRxFrame"!][!//
[!/* Pre-processor switch for enabling EthSwt_EthIfPreProcessTxFrame API
*/!][!VAR "EthIf_PreProcessTxFrame" = "as:modconf('EthIf')[1]/EthIfGeneral/EthIfSwtAdpatTxFrame"!][!//
[!/* Pre-processor switch for enabling EthIf_GetArlTable API
*/!][!VAR "EthIf_GetArlTable" = "as:modconf('EthIf')[1]/EthIfGeneral/EthIfGetArlTableApi"!][!//
[!/* Pre-processor switch for enabling EthIf_GetBufferLevel API
*/!][!VAR "EthIf_GetBufferLevelApi" = "as:modconf('EthIf')[1]/EthIfGeneral/EthIfGetBufferLevelApi"!][!//
[!/* Pre-processor switch for enabling EthIf_SwtGetCounterValues API
*/!][!VAR "EthIf_GetCounterValuesApi" = "as:modconf('EthIf')[1]/EthIfGeneral/EthIfSwtGetCounterValuesApi"!][!//
[!/* Pre-processor switch for enabling EthIf_StoreConfiguration API
*/!][!VAR "EthIf_StoreConfigurationApi" = "as:modconf('EthIf')[1]/EthIfGeneral/EthIfStoreConfigurationApi"!][!//
[!/* Pre-processor switch for enabling EthIf_ResetConfiguration API
*/!][!VAR "EthIf_ResetConfigurationApi" = "as:modconf('EthIf')[1]/EthIfGeneral/EthIfResetConfigurationApi"!][!//

[!VAR "EthIf_CtrlBswRefNr" = "num:i(count(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfEthControllerType/*/EthIfEthControllerBswmdImplementationRefs))"!][!//
[!VAR "EthIf_TrcvBswRefNr" = "num:i(count(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfEthTrcvType/*/EthIfEthTrcvBswmdImplementationRefs))"!][!//
[!VAR "EthIf_SwtBswRefNr" = "num:i(count(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfEthSwtType/*/EthIfEthSwtBswmdImplementationRefs))"!][!//
[!VAR "swtExtension" = "''"!][!//
[!IF "$EthIf_SwtBswRefNr > 0"!][!//
  [!/*
     === Vendor ID ===
  */!][!//
  [!VAR "swtVendorId" = "asc:getVendorId(as:path(node:ref(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfEthSwtType/*[1]/EthIfEthSwtBswmdImplementationRefs)))"!][!//
  [!/*
     *** Api Infix ***
  */!][!//
  [!VAR "swtApiInfix" = "asc:getVendorApiInfix(as:path(node:ref(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfEthSwtType/*[1]/EthIfEthSwtBswmdImplementationRefs)))"!][!//
  [!/*
     *** Put together the VendorId and ApiInfix ***
  */!][!//
  [!VAR "swtExtension" = "concat($swtVendorId,'_', $swtApiInfix)"!][!//
[!ENDIF!][!// EthIf_SwtBswRefNr
[!ENDIF!][!// ETHIF_CFG_M