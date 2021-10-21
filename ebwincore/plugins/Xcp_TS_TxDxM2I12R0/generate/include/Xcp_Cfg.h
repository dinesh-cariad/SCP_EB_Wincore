/**
 * \file
 *
 * \brief AUTOSAR Xcp
 *
 * This file contains the implementation of the AUTOSAR
 * module Xcp.
 *
 * \version 2.12.0
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
/* This file contains the generated Xcp module configuration. */

[!INCLUDE "../../generate_macros/Xcp_Vars.m"!][!//
#ifndef XCP_CFG_H
#define XCP_CFG_H
[!//
[!AUTOSPACING!]
[!//
[!VAR "Spaces31"  = "'                               '"!]
[!//
[!VAR "DynamicAreaSize" = "0"!]
[!IF "(text:tolower(XcpGeneral/XcpDaqSupported) = 'true') and ($XcpDaqConfigType = 'DAQ_DYNAMIC')"!]
  [!IF "num:integer(XcpGeneral/XcpDynamicAreaSize mod 4) = 0"!]
[!VAR "DynamicAreaSize" = "num:integer(XcpGeneral/XcpDynamicAreaSize)"!]
  [!ELSE!]
[!VAR "DynamicAreaSize" = "num:integer(XcpGeneral/XcpDynamicAreaSize + (4 - (XcpGeneral/XcpDynamicAreaSize mod 4)))"!]
  [!ENDIF!]
[!ENDIF!]
[!//
[!VAR "DynamicMirrorAreaSize" = "0"!]
[!IF "(text:tolower(XcpGeneral/XcpDaqSupported) = 'true') and ($XcpDaqConfigType = 'DAQ_DYNAMIC')"!]
  [!VAR "DynamicMirrorAreaSize" = "num:integer(XcpGeneral/XcpDynamicMirrorAreaSize)"!]
[!ENDIF!]
[!/* Variable to calculate the number of bits for the MirrorSize type */!][!//
[!VAR "MaxSize" = "$NonDynamicMirrorSize"!]
[!IF "$NonDynamicMirrorSize < $DynamicMirrorAreaSize"!]
  [!VAR "MaxSize" = "$DynamicMirrorAreaSize"!]
[!ENDIF!][!//
[!IF "num:i(round($MaxSize)) < bit:shl(1,8)"!]
  [!VAR "MirrorTypeSize" = "num:i(8)"!]
[!ELSEIF "num:i(round($MaxSize)) < bit:shl(1,16)"!]
  [!VAR "MirrorTypeSize" = "num:i(16)"!]
[!ELSE!]
  [!VAR "MirrorTypeSize" = "num:i(32)"!]
[!ENDIF!]
[!/* Variable to calculate the number of bits for the DataSize type */!][!//
[!IF "num:i(round($MaxSizeOdt)) < bit:shl(1,8)"!]
  [!VAR "OdtTypeSize" = "num:i(8)"!]
[!ELSEIF "num:i(round($MaxSizeOdt)) < bit:shl(1,16)"!]
  [!VAR "OdtTypeSize" = "num:i(16)"!]
[!ELSE!]
  [!VAR "OdtTypeSize" = "num:i(32)"!]
[!ENDIF!]
[!//
/*==================[includes]===============================================*/

#include <Xcp_Types.h>    /* Xcp types to be published */
#include <TSAutosar.h>    /* EB specific standard types */
#include <ComStack_Types.h> /* Comstack types */
[!IF "text:tolower(XcpGeneral/XcpOnEthernetEnabled) = 'true'"!]
#include <SoAd.h>     /* SoAd types */
[!ENDIF!]
[!IF "(text:tolower(XcpGeneral/XcpOnCanEnabled) = 'true') or (text:tolower(XcpGeneral/XcpOnCanFDEnabled) = 'true')"!]
#include <CanIf.h>             /* CAN interface APIs  */
[!ENDIF!]
[!IF "text:tolower(XcpGeneral/XcpOnFlexRayEnabled) = 'true'"!]
# include <FrIf.h>             /* FlexRay interface APIs  */
[!ENDIF!]

[!IF "$XcpBswDistributionEnabled"!]
#include <Os.h>
[!ENDIF!]

/*==================[macros]=================================================*/

#if (defined XCP_OVERLOAD_MSB_SUPPORT)
#error XCP_OVERLOAD_MSB_SUPPORT already defined
#endif
/** \brief Switch, indicating whether support for overload MSB 
 ** is enabled or disabled */
#define XCP_OVERLOAD_MSB_SUPPORT         [!//
[!IF "text:tolower(XcpGeneral/XcpSupportForOverloadMSB) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCP_WRITE_READ_MEMORY_RAM)
#error XCP_WRITE_READ_MEMORY_RAM already defined
#endif
/** \brief Switch, indicating whether user RAM memory access
 ** callouts are enabled or disabled */
#define XCP_WRITE_READ_MEMORY_RAM         [!//
[!IF "text:tolower(XcpGeneral/XcpBlockWriteReadMemoryRAMMechanism) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCP_ASYNCH_CRC_SUPPORT)
#error XCP_ASYNCH_CRC_SUPPORT already defined
#endif
/** \brief Enable/disable support for asynchronous calculation 
 ** of the Build Checksum on Xcp_CrcMainFunction context. */
#define XCP_ASYNCH_CRC_SUPPORT   [!//
[!IF "text:tolower(XcpGeneral/XcpBuildChecksumMainFunctionSupport) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCP_VERSION_INFO_API)
#error XCP_VERSION_INFO_API already defined
#endif
/** \brief Switch, indicating whether API version information is
 ** activated or deactivated  for XCP */
#define XCP_VERSION_INFO_API         [!//
[!IF "text:tolower(XcpGeneral/XcpVersionInfoApi) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#if (defined XCP_ON_CAN_ENABLED)
#error XCP_ON_CAN_ENABLED already defined
#endif
/** \brief Switch, indicating whether XCPonCAN functionality is available */
#define XCP_ON_CAN_ENABLED           [!//
[!IF "text:tolower(XcpGeneral/XcpOnCanEnabled) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#if (defined XCP_ON_CANFD_ENABLED)
#error XCP_ON_CANFD_ENABLED already defined
#endif
/** \brief Switch, indicating whether XCPonCANFD functionality is available */
#define XCP_ON_CANFD_ENABLED           [!//
[!IF "text:tolower(XcpGeneral/XcpOnCanFDEnabled) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#if (defined XCP_ON_FLEXRAY_ENABLED)
#error XCP_ON_FLEXRAY_ENABLED already defined
#endif
/** \brief Switch, indicating whether XCPonFlexRay functionality is available */
#define XCP_ON_FLEXRAY_ENABLED       [!//
[!IF "text:tolower(XcpGeneral/XcpOnFlexRayEnabled) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#if (defined XCP_ON_ETHERNET_ENABLED)
#error XCP_ON_ETHERNET_ENABLED already defined
#endif
/** \brief Switch, indicating whether XCPonEthernet functionality is available */
#define XCP_ON_ETHERNET_ENABLED      [!//
[!IF "text:tolower(XcpGeneral/XcpOnEthernetEnabled) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#if (defined XCP_ON_CDD_ENABLED)
#error XCP_ON_CDD_ENABLED already defined
#endif
/** \brief Switch, indicating whether XCPonCDD functionality is available */
#define XCP_ON_CDD_ENABLED           [!//
[!IF "text:tolower(XcpGeneral/XcpOnCddEnabled) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#if (defined XCP_SOAD_PROTOCOL_UDP_SUPPORT)
#error XCP_SOAD_PROTOCOL_UDP_SUPPORT already defined
#endif
/** \brief Switch, indicating whether the support for UDP protocol is enabled.
 ** If UDP is used the socket must be closed if it is not used anymore.
 ** Otherwise the socket would be locked to the initial source port,
 ** preventing further requests from other ports.
 **/
#define XCP_SOAD_PROTOCOL_UDP_SUPPORT          [!//
[!IF "$SuportForSoAdSocketProtocolUdp"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCP_SOAD_PROTOCOL_TCP_SUPPORT)
#error XCP_SOAD_PROTOCOL_TCP_SUPPORT already defined
#endif
/** \brief Switch, indicating whether the support for TCP protocol is enabled.
 ** If TCP is used the connection has to be terminated in case the
 ** connection is closed at the client side.
 **/
#define XCP_SOAD_PROTOCOL_TCP_SUPPORT          [!//
[!IF "$SuportForSoAdSocketProtocolTcp"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#if (defined XCP_EVENT_TRIGGERED_MAINFUNC )
#error XCP_EVENT_TRIGGERED_MAINFUNC  already defined
#endif
/** \brief Switch, indicating whether the processing of cyclicaly
 **        events are processed by Event MainFunctions */
#define XCP_EVENT_TRIGGERED_MAINFUNC         [!//
[!IF "text:tolower(XcpGeneral/XcpEventTriggerMainFunc) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#if (defined XCP_PROCESS_MULTI_MF_EVENTS )
#error XCP_PROCESS_MULTI_MF_EVENTS  already defined
#endif
/** \brief Switch, indicating if the Xcp_MainFunction is processing multiple events */
#define XCP_PROCESS_MULTI_MF_EVENTS         [!//
[!IF "text:tolower(XcpGeneral/XcpProcessMultiEventMainFunc) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


[!IF "ReportToDem/XcpRetryFailedReportToDem = 'DET'"!][!//
#if (defined XCP_E_DEMTODET_RETRY_FAILED)
#error XCP_E_DEMTODET_RETRY_FAILED already defined
#endif
/* DET error Id for Xcp Retry Failed */
#define XCP_E_DEMTODET_RETRY_FAILED  [!//
[!"ReportToDem/XcpRetryFailedDemDetErrorId"!]U
[!ELSEIF "ReportToDem/XcpRetryFailedReportToDem = 'DEM'"!]
#if (defined XCP_E_RETRY_FAILED_DEM_EVENT_ID)
#error XCP_E_RETRY_FAILED_DEM_EVENT_ID already defined
#endif
/** \brief DEM Event ID for Xcp Retry Failed */
#define XCP_E_RETRY_FAILED_DEM_EVENT_ID [!//
[!"node:value(as:ref(XcpConfig/*[1]/XcpDemEventParameterRefs/XCP_E_RETRY_FAILED)/DemEventId)"!]U
[!ENDIF!][!//


[!IF "num:i(count(XcpConfig/*/XcpMemoryAccessArea/*)) > 0"!]
[!IF "ReportToDem/XcpIllegalMemoryAccessReportToDem = 'DET'"!][!//
#if (defined XCP_E_DEMTODET_ILLEGAL_MEMORY_ACCESS)
#error XCP_E_DEMTODET_ILLEGAL_MEMORY_ACCESS already defined
#endif
/* DET error Id for Xcp Illegal Memory Access */
#define XCP_E_DEMTODET_ILLEGAL_MEMORY_ACCESS  [!//
[!"ReportToDem/XcpIllegalMemoryAccessDemDetErrorId"!]U
[!ELSEIF "ReportToDem/XcpIllegalMemoryAccessReportToDem = 'DEM'"!]
#if (defined XCP_E_ILLEGAL_MEMORY_ACCESS_DEM_EVENT_ID)
#error XCP_E_ILLEGAL_MEMORY_ACCESS_DEM_EVENT_ID already defined
#endif
/** \brief DEM Event ID for Xcp Illegal Memory Access */
#define XCP_E_ILLEGAL_MEMORY_ACCESS_DEM_EVENT_ID [!//
[!"node:value(as:ref(XcpConfig/*[1]/XcpDemEventParameterRefs/XCP_E_ILLEGAL_MEMORY_ACCESS)/DemEventId)"!]U
[!ENDIF!][!//
[!ENDIF!][!//



[!IF "ReportToDem/XcpPduBufferLockedReportToDem = 'DET'"!][!//
#if (defined XCP_E_DEMTODET_PDU_BUFFER_LOCKED)
#error XCP_E_DEMTODET_PDU_BUFFER_LOCKED already defined
#endif
/* DET error Id for Pdu Buffer Locked*/
#define XCP_E_DEMTODET_PDU_BUFFER_LOCKED  [!//
[!"ReportToDem/XcpPduBufferLockedDemDetErrorId"!]U
[!ELSEIF "ReportToDem/XcpPduBufferLockedReportToDem = 'DEM'"!]
#if (defined XCP_E_PDU_BUFFER_LOCKED_DEM_EVENT_ID)
#error XCP_E_PDU_BUFFER_LOCKED_DEM_EVENT_ID already defined
#endif
/** \brief DEM Event ID for Xcp Pdu Buffer Locked */
#define XCP_E_PDU_BUFFER_LOCKED_DEM_EVENT_ID [!//
[!"node:value(as:ref(XcpConfig/*[1]/XcpDemEventParameterRefs/XCP_E_PDU_BUFFER_LOCKED)/DemEventId)"!]U
[!ENDIF!][!//

[!IF "ReportToDem/XcpRespCTOQueueFullReportToDem = 'DET'"!][!//
#if (defined XCP_E_DEMTODET_RESP_CTO_QUEUE_FULL)
#error XCP_E_DEMTODET_RESP_CTO_QUEUE_FULL already defined
#endif
/* DET error Id for Xcp Response CTO queue full */
#define XCP_E_DEMTODET_RESP_CTO_QUEUE_FULL  [!//
[!"ReportToDem/XcpRespCTOQueueFullDemDetErrorId"!]U
[!ELSEIF "ReportToDem/XcpRespCTOQueueFullReportToDem = 'DEM'"!]
#if (defined XCP_E_RESP_CTO_QUEUE_FULL_DEM_EVENT_ID)
#error XCP_E_RESP_CTO_QUEUE_FULL_DEM_EVENT_ID already defined
#endif
/** \brief DEM Event ID for Xcp Response CTO queue full  */
#define XCP_E_RESP_CTO_QUEUE_FULL_DEM_EVENT_ID [!//
[!"node:value(as:ref(XcpConfig/*[1]/XcpDemEventParameterRefs/XCP_E_RESP_CTO_QUEUE_FULL)/DemEventId)"!]U
[!ENDIF!][!//

[!IF "ReportToDem/XcpPDUBufferFullReportToDem = 'DET'"!][!//
#if (defined XCP_E_DEMTODET_PDU_BUFFER_FULL)
#error XCP_E_DEMTODET_PDU_BUFFER_FULL already defined
#endif
/* DET error Id for Xcp Pdu Buffer Full */
#define XCP_E_DEMTODET_PDU_BUFFER_FULL  [!//
[!"ReportToDem/XcpPDUBufferFullDemDetErrorId"!]U
[!ELSEIF "ReportToDem/XcpPDUBufferFullReportToDem = 'DEM'"!]
#if (defined XCP_E_PDU_BUFFER_FULL_DEM_EVENT_ID)
#error XCP_E_PDU_BUFFER_FULL_DEM_EVENT_ID already defined
#endif
/** \brief DEM Event ID for Xcp Pdu Buffer full  */
#define XCP_E_PDU_BUFFER_FULL_DEM_EVENT_ID [!//
[!"node:value(as:ref(XcpConfig/*[1]/XcpDemEventParameterRefs/XCP_E_PDU_BUFFER_FULL)/DemEventId)"!]U
[!ENDIF!][!//

[!IF "ReportToDem/XcpPDULostReportToDem = 'DET'"!][!//
#if (defined XCP_E_DEMTODET_PDU_LOST)
#error XCP_E_DEMTODET_PDU_LOST already defined
#endif
/* DET error Id for Xcp PDU Lost */
#define XCP_E_DEMTODET_PDU_LOST  [!//
[!"ReportToDem/XcpPDULostDemDetErrorId"!]U
[!ELSEIF "ReportToDem/XcpPDULostReportToDem = 'DEM'"!]
#if (defined XCP_E_PDU_LOST_DEM_EVENT_ID)
#error XCP_E_PDU_LOST_DEM_EVENT_ID already defined
#endif
/** \brief DEM Event ID for Xcp Pdu lost  */
#define XCP_E_PDU_LOST_DEM_EVENT_ID [!//
[!"node:value(as:ref(XcpConfig/*[1]/XcpDemEventParameterRefs/XCP_E_PDU_LOST)/DemEventId)"!]U
[!ENDIF!][!//

[!IF "ReportToDem/XcpStimulationDataLostReportToDem = 'DET'"!][!//
#if (defined XCP_E_DEMTODET_STIMULATION_DATA_LOST)
#error XCP_E_DEMTODET_STIMULATION_DATA_LOST already defined
#endif
/* DET error Id for Xcp Stimulation Data Lost */
#define XCP_E_DEMTODET_STIMULATION_DATA_LOST [!//
[!"ReportToDem/XcpStimulationDataLostDemDetErrorId"!]U
[!ELSEIF "ReportToDem/XcpStimulationDataLostReportToDem = 'DEM'"!]
#if (defined XCP_E_STIMULATION_DATA_LOST_DEM_EVENT_ID)
#error XCP_E_STIMULATION_DATA_LOST_DEM_EVENT_ID already defined
#endif
/** \brief DEM Event ID for Xcp Stimulation Data Lost  */
#define XCP_E_STIMULATION_DATA_LOST_DEM_EVENT_ID [!//
[!"node:value(as:ref(XcpConfig/*[1]/XcpDemEventParameterRefs/XCP_E_STIMULATION_DATA_LOST)/DemEventId)"!]U
[!ENDIF!][!//

[!IF "ReportToDem/XcpAcquisitionDataLostReportToDem = 'DET'"!][!//
#if (defined XCP_E_DEMTODET_ACQUISITION_DATA_LOST)
#error XCP_E_DEMTODET_ACQUISITION_DATA_LOST already defined
#endif
/* DET error Id for Xcp Acquisition Data Lost */
#define XCP_E_DEMTODET_ACQUISITION_DATA_LOST [!//
[!"ReportToDem/XcpAcquisitionDataLostDemDetErrorId"!]U
[!ELSEIF "ReportToDem/XcpAcquisitionDataLostReportToDem = 'DEM'"!]
#if (defined XCP_E_ACQUISITION_DATA_LOST_DEM_EVENT_ID)
#error XCP_E_ACQUISITION_DATA_LOST_DEM_EVENT_ID already defined
#endif
/** \brief DEM Event ID for Xcp Acquisition Data Lost  */
#define XCP_E_ACQUISITION_DATA_LOST_DEM_EVENT_ID [!//
[!"node:value(as:ref(XcpConfig/*[1]/XcpDemEventParameterRefs/XCP_E_ACQUISITION_DATA_LOST)/DemEventId)"!]U
[!ENDIF!][!//

#if (defined XCP_NON_DYNAMIC_MIRROR_SIZE)
#error XCP_NON_DYNAMIC_MIRROR_SIZE already defined
#endif
/** \brief Value indicating the ODT Mirror Size.
 **/
#define XCP_NON_DYNAMIC_MIRROR_SIZE           [!"num:integer($NonDynamicMirrorSize)"!]U

#if (defined XCP_MAX_ODTENTRY_SIZE)
#error XCP_MAX_ODTENTRY_SIZE already defined
#endif
/** \brief Value indicating the Maximum ODT Entry Size.
 **/
#define XCP_MAX_ODTENTRY_SIZE                [!"num:integer($MaxSizeOdtEntry)"!]U

#if (defined XCP_ENABLE_XCP_CONTROL_API)
#error XCP_ENABLE_XCP_CONTROL_API already defined
#endif
/** \brief Switch, indicating whether XCP control API functionality is available.
 **/
#define XCP_ENABLE_XCP_CONTROL_API          [!//
[!IF "text:tolower(XcpGeneral/XcpEnableXcpControlApi) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#if (defined XCP_DEFAULT_XCP_MODULE_STATE)
#error XCP_DEFAULT_XCP_MODULE_STATE already defined
#endif
/** \brief Switch, indicating whether XCP default module state is available.
 **/
#define XCP_DEFAULT_XCP_MODULE_STATE                              [!//
[!IF "text:tolower(XcpGeneral/XcpDefaultXcpModuleState) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCP_NUMBER_OF_USERCOMMANDS)
#error XCP_NUMBER_OF_USERCOMMANDS already defined
#endif
/** \brief Number of user defined commands.
 **/
#define XCP_NUMBER_OF_USERCOMMANDS                                [!"num:i(count(XcpUserCommand/*))"!]U

#if (defined XCP_DYNAMIC_PDU_SUPPORTED)
#error XCP_DYNAMIC_PDU_SUPPORTED already defined
#endif
/** \brief Switch, indicating whether XCP PDU can be enabled/disabled at runtime.
 **/
#define XCP_DYNAMIC_PDU_SUPPORTED         [!//
STD_[!IF "XcpGeneral/XcpSupportDynamicComChannels = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined XCP_COM_MAINFUNCTION_SUPPORT)
  
#error XCP_COM_MAINFUNCTION_SUPPORT already defined
#endif
/** \brief Switch, indicating whether Tx and Rx MainFunctions are available.
 **/
#define XCP_COM_MAINFUNCTION_SUPPORT      [!//
STD_[!IF "node:exists(XcpGeneral/XcpSupportComMainFunctions) = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined XCP_PDU_ROUTING_TABLE)
  
#error XCP_PDU_ROUTING_TABLE already defined
#endif
/** \brief Switch, indicating whether the PduId from lower layer are the Xcp PduIds.
 **/
#define XCP_PDU_ROUTING_TABLE             [!//
STD_[!IF "XcpGeneral/XcpSupportForPDURoutingTable = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]


#if (defined XCP_FLEXRAY_TP_COMMANDS_SUPPORTED)
#error XCP_FLEXRAY_TP_COMMANDS_SUPPORTED already defined
#endif
/** \brief Number of user defined commands.
 **/
#define XCP_FLEXRAY_TP_COMMANDS_SUPPORTED            [!//
STD_[!IF "(node:exists(XcpGeneral/XcpSupportForFlxTPCommands) = 'true') and (text:tolower(XcpGeneral/XcpSupportForFlxTPCommands) = 'true')"!]ON[!ELSE!]OFF[!ENDIF!]


#if (defined XCP_NO_OF_FLX_BUF_CONFIGURED)
#error XCP_NO_OF_FLX_BUF_CONFIGURED already defined
#endif
/** \brief Number of Flexray hardware buffers configured.
 **/
#define XCP_NO_OF_FLX_BUF_CONFIGURED                                [!"num:i(count(XcpConfig/*[1]/XcpConnectionCfg/*/XcpConnectionInterfaceType/XcpFlexrayBufferCfg/*))"!]U

#if (defined XCP_SUPPRESS_TX_SUPPORT)
#error XCP_SUPPRESS_TX_SUPPORT already defined
#endif
/** \brief Switch, indicating whether Xcp_SetTransmissionMode() is supported or not. */
#define XCP_SUPPRESS_TX_SUPPORT      [!//
STD_[!IF "text:tolower(XcpGeneral/XcpSuppressTxSupport) = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

/*------------------[symbolic name values]----------------------------------*/

[!LOOP "node:order(XcpConfig/*[1]/XcpPdu/*[@name='XcpTxPdu'],'XcpTxPduId')"!]
  [!CALL "GetSymbolName","ShortNameRef"="'.'","OldName"="name(.)"!]
#if (defined XcpConf_XcpPdu_[!"$SymbolName"!])
#error XcpConf_XcpPdu_[!"$SymbolName"!] already defined
#endif
/** \brief Symbolic name value of the Handle ID for receiving a TxConfirmation or a TriggerTransmit
 **
 ** This Handle ID must be used by the underlying communication layer after the Xcp requested a
 ** transmission.
 **/
#define XcpConf_XcpPdu_[!"$SymbolName"!]        [!"XcpTxPduId"!]U

#ifndef XCP_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#if (defined [!"$SymbolName"!])
#error [!"$SymbolName"!] is already defined
#endif
/** \brief Export symbolic name value without prefix (AUTOSAR version <= 3.1 rev4) */
#define [!"$SymbolName"!]        [!"XcpTxPduId"!]U

#if (defined Xcp_[!"$SymbolName"!])
#error Xcp_[!"$SymbolName"!] is already defined
#endif
/** \brief Export symbolic name value with module abbreviation as prefix only (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2) */
#define Xcp_[!"$SymbolName"!]        [!"XcpTxPduId"!]U
#endif /* XCP_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDLOOP!]

[!LOOP "node:order(XcpConfig/*[1]/XcpPdu/*[@name='XcpRxPdu'],'XcpRxPduId')"!]
  [!CALL "GetSymbolName","ShortNameRef"="'.'","OldName"="name(.)"!]
#if (defined XcpConf_XcpPdu_[!"$SymbolName"!])
#error XcpConf_XcpPdu_[!"$SymbolName"!] already defined
#endif
/** \brief Symbolic name value for the Handle IDs used to receive packets from the master. */
#define XcpConf_XcpPdu_[!"$SymbolName"!]        [!"XcpRxPduId"!]U

#ifndef XCP_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#if (defined [!"$SymbolName"!])
#error [!"$SymbolName"!] is already defined
#endif
/** \brief Export symbolic name value without prefix (AUTOSAR version <= 3.1 rev4) */
#define [!"$SymbolName"!]        [!"XcpRxPduId"!]U

#if (defined Xcp_[!"$SymbolName"!])
#error Xcp_[!"$SymbolName"!] is already defined
#endif
/** \brief Export symbolic name value with module abbreviation as prefix only (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2) */
#define Xcp_[!"$SymbolName"!]        [!"XcpRxPduId"!]U
#endif /* XCP_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDLOOP!]

[!LOOP "node:order(XcpConfig/*/XcpConnectionCfg/*,'XcpConnectionId')"!]

#if (defined XcpConf_XcpConnectionCfg_[!"name(.)"!])
#error XcpConf_XcpConnectionCfg_[!"name(.)"!] already defined
#endif
/** \brief The Index number of the connection configuration*/
#define XcpConf_XcpConnectionCfg_[!"name(.)"!]         [!"XcpConnectionId"!]U

#ifndef XCP_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#if (defined [!"name(.)"!])
#error [!"name(.)"!] is already defined
#endif
/** \brief Export symbolic name value without prefix (AUTOSAR version <= 3.1 rev4) */
#define [!"name(.)"!]        [!"XcpConnectionId"!]U

#if (defined Xcp_[!"name(.)"!])
#error Xcp_[!"name(.)"!] is already defined
#endif
/** \brief Export symbolic name value with module abbreviation as prefix only (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2) */
#define Xcp_[!"name(.)"!]        [!"XcpConnectionId"!]U
#endif /* XCP_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDLOOP!]

[!LOOP "node:order(XcpConfig/*[1]/XcpEventChannel/*,'XcpEventChannelNumber')"!]

#if (defined XcpConf_XcpEventChannel_[!"name(.)"!])
#error XcpConf_XcpEventChannel_[!"name(.)"!] already defined
#endif
/** \brief The Index number of the event channel*/
#define XcpConf_XcpEventChannel_[!"name(.)"!]         [!"XcpEventChannelNumber"!]U

#ifndef XCP_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#if (defined [!"name(.)"!])
#error [!"name(.)"!] is already defined
#endif
/** \brief Export symbolic name value without prefix (AUTOSAR version <= 3.1 rev4) */
#define [!"name(.)"!]        [!"XcpEventChannelNumber"!]U

#if (defined Xcp_[!"name(.)"!])
#error Xcp_[!"name(.)"!] is already defined
#endif
/** \brief Export symbolic name value with module abbreviation as prefix only (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2) */
#define Xcp_[!"name(.)"!]        [!"XcpEventChannelNumber"!]U
#endif /* XCP_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDLOOP!]

[!LOOP "node:order(XcpConfig/*/XcpDaqList/*,'XcpDaqListNumber')"!]

#if (defined XcpConf_XcpDaqList_[!"name(.)"!])
#error XcpConf_XcpDaqList_[!"name(.)"!] already defined
#endif
/** \brief The Index number of the static/predefined DAQ List*/
#define XcpConf_XcpDaqList_[!"name(.)"!]         [!"XcpDaqListNumber"!]U

#ifndef XCP_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#if (defined [!"name(.)"!])
#error [!"name(.)"!] is already defined
#endif
/** \brief Export symbolic name value without prefix (AUTOSAR version <= 3.1 rev4) */
#define [!"name(.)"!]        [!"XcpDaqListNumber"!]U

#if (defined Xcp_[!"name(.)"!])
#error Xcp_[!"name(.)"!] is already defined
#endif
/** \brief Export symbolic name value with module abbreviation as prefix only (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2) */
#define Xcp_[!"name(.)"!]        [!"XcpDaqListNumber"!]U
#endif /* XCP_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDLOOP!]

[!LOOP "node:order(XcpConfig/*/XcpDaqList/*[XcpDaqListNumber < node:current()/XcpGeneral/XcpMinDaq],'XcpDaqListNumber')"!]
  [!LOOP "node:order(XcpOdt/*,'XcpOdtNumber')"!]

#if (defined XcpConf_[!"../../@name"!]_[!"name(.)"!])
#error XcpConf_[!"../../@name"!]_[!"name(.)"!] already defined
#endif
/** \brief Index number of ODT [!"XcpOdtNumber"!] within the predefined DAQ list [!"../../XcpDaqListNumber"!] */
#define XcpConf_[!"../../@name"!]_[!"name(.)"!]         [!"XcpOdtNumber"!]U

#ifndef XCP_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#if (defined [!"../../@name"!]_[!"name(.)"!])
#error [!"../../@name"!]_[!"name(.)"!] is already defined
#endif
/** \brief Export symbolic name value without prefix (AUTOSAR version <= 3.1 rev4) */
#define [!"../../@name"!]_[!"name(.)"!]        [!"XcpOdtNumber"!]U

#if (defined Xcp_[!"../../@name"!]_[!"name(.)"!])
#error Xcp_[!"../../@name"!]_[!"name(.)"!] is already defined
#endif
/** \brief Export symbolic name value with module abbreviation as prefix only (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2) */
#define Xcp_[!"../../@name"!]_[!"name(.)"!]        [!"XcpOdtNumber"!]U
#endif /* XCP_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES */
  [!ENDLOOP!]
[!ENDLOOP!]

#if (defined XCP_MAX_DAQ)
#error XCP_MAX_DAQ already defined
#endif
/** \brief The maximum number of DAQ lists on the XCP slave. */
#define XCP_MAX_DAQ                  [!//
[!"num:integer($XcpMaxDaq)"!]U

#if (defined XCP_NO_OF_NON_DYNAMIC_DAQ_LISTS)
#error XCP_NO_OF_NON_DYNAMIC_DAQ_LISTS already defined
#endif
/** \brief The number of predefined/static DAQ lists */
#define XCP_NO_OF_NON_DYNAMIC_DAQ_LISTS             [!//
[!"num:integer($XcpNoOfNonDynamicDaqLists)"!]U

#if (defined XCP_NO_OF_NON_DYNAMIC_ODTS)
#error XCP_NO_OF_NON_DYNAMIC_ODTS already defined
#endif
/** \brief The total number of predefined/static ODTs */
#define XCP_NO_OF_NON_DYNAMIC_ODTS                   [!//
[!"num:integer($XcpNoOfNonDynamicODTs)"!]U

#if (defined XCP_NO_OF_NON_DYNAMIC_ODTENTRIES)
#error XCP_NO_OF_NON_DYNAMIC_ODTENTRIES already defined
#endif
/** \brief The total number of predefined/static ODT entries */
#define XCP_NO_OF_NON_DYNAMIC_ODTENTRIES             [!//
[!"num:integer($XcpNoOfNonDynamicODTEntries)"!]U

#if (defined XCP_MAX_ODT)
#error XCP_MAX_ODT already defined
#endif
/** \brief Total number of ODTs possible in the slave.
 **
 **/
#define XCP_MAX_ODT                                        [!"num:integer($XcpTotalNumberOfOdts)"!]U

#if (defined XCP_DYNAMIC_AREA_SIZE)
#error XCP_DYNAMIC_AREA_SIZE already defined
#endif
/** \brief The size in bytes of the memory area used for dynamic DAQ list configuration.
 **
 **/
#define XCP_DYNAMIC_AREA_SIZE                             [!"num:integer($DynamicAreaSize)"!]U

#if (defined XCP_DYNAMIC_MIRROR_SIZE)
#error XCP_DYNAMIC_MIRROR_SIZE already defined
#endif
/** \brief The size in bytes of the memory area used for Mirror of ODT data of dynamic DAQ list.
 **
 **/
#define XCP_DYNAMIC_MIRROR_SIZE                            [!"num:integer($DynamicMirrorAreaSize)"!]U

#if (defined XCP_NUMBER_OF_BITS_IN_BYTE)
#error XCP_NUMBER_OF_BITS_IN_BYTE already defined
#endif
/** \brief The number of bits in a byte */
#define XCP_NUMBER_OF_BITS_IN_BYTE        8U

[!IF "text:tolower($StoreDaq) = 'true'"!]

#if (defined XCP_NUMBER_OF_BYTES_SELECTED_DAQ)
#error XCP_NUMBER_OF_BYTES_SELECTED_DAQ already defined
#endif
/** \brief The number of bytes needed to hold the information of which DAQ lists is selected to
 * start immediately after a DAQ store in NV was completed successfully. */
#define XCP_NUMBER_OF_BYTES_SELECTED_DAQ  (((XCP_MAX_DAQ-1U)/XCP_NUMBER_OF_BITS_IN_BYTE) + 1U)

#if (defined XCP_NVM_DAQ_LISTS_RAM_ADDRESS)
#error XCP_NVM_DAQ_LISTS_RAM_ADDRESS already defined
#endif
/** \brief Symbolic name for the DAQ lists runtime structure used to configure the NvM block */
#define XCP_NVM_DAQ_LISTS_RAM_ADDRESS     &Xcp_DaqLists

[!ENDIF!] [!/* $StoreDaq = 'true' */!]


#if (defined XCP_STORE_DAQ_SUPPORTED)
#error XCP_STORE_DAQ_SUPPORTED already defined
#endif
/** \brief Switch, indicating whether all DAQ lists can be stored in the NV memory. **/
#define XCP_STORE_DAQ_SUPPORTED         [!//
[!IF "text:tolower($StoreDaq) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#if (defined XCP_CAL_PAG_STORE_SUPPORTED)
#error XCP_CAL_PAG_STORE_SUPPORTED already defined
#endif
/** \brief Switch, indicating whether calibration data storing to NV memory functionality is available.
 **/
#define XCP_CAL_PAG_STORE_SUPPORTED          [!//
STD_[!IF "(node:exists(XcpGeneral/XcpCalPagStoreSupported) = 'true') and (text:tolower(XcpGeneral/XcpCalPagStoreSupported) = 'true')"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined XCP_GET_DAQ_ID_SUPPORTED)
#error XCP_GET_DAQ_ID_SUPPORTED already defined
#endif
/** \brief Switch, indicating whether GET_DAQ_ID command is supported.
 **/
#define XCP_GET_DAQ_ID_SUPPORTED          [!//
STD_[!IF "(node:exists(XcpGeneral/XcpSupportForGetDaqId) = 'true') and (text:tolower(XcpGeneral/XcpSupportForGetDaqId) = 'true')"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined XCP_TRIGGER_TX_API_ENABLED)
#error XCP_TRIGGER_TX_API_ENABLED already defined
#endif
/** \brief Switch, indicating whether Xcp_TriggerTransmit API is supported.
 **/
#define XCP_TRIGGER_TX_API_ENABLED        [!//
STD_[!IF "$TriggerTransmitIsEnabled = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined XCP_CDD_TRIGGER_TX_API_ENABLED)
#error XCP_CDD_TRIGGER_TX_API_ENABLED already defined
#endif
/** \brief Switch, indicating whether Xcp_CddTriggerTransmit API is supported.
 **/
#define XCP_CDD_TRIGGER_TX_API_ENABLED        [!//
STD_[!IF "$CddTriggerTransmitIsEnabled = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]


#if (defined XCP_NUMBER_OF_MEMORY_AREAS)
#error XCP_NUMBER_OF_MEMORY_AREAS already defined
#endif
/** \brief Number of memory access areas
 **/
#define XCP_NUMBER_OF_MEMORY_AREAS          [!"num:i(count(XcpConfig/*/XcpMemoryAccessArea/XcpMemoryAccessArea/*))"!]U


#if (defined XCP_MEMORY_AREAS_ACCESS_CALLOUT)
#error XCP_MEMORY_AREAS_ACCESS_CALLOUT already defined
#endif
/** \brief Switch, indicating whether XcpMemoryAccessAreasCallout is enabled.
 **/
#define XCP_MEMORY_AREAS_ACCESS_CALLOUT          [!//
STD_[!IF "XcpConfig/*[1]/XcpMemoryAccessArea/XcpMemoryAccessAreasCallout = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined XCP_BSW_DISTRIBUTION_ENABLED)
#error XCP_BSW_DISTRIBUTION_ENABLED already defined
#endif
[!IF "$XcpBswDistributionEnabled"!]
#define XCP_BSW_DISTRIBUTION_ENABLED STD_ON
[!ELSE!]
#define XCP_BSW_DISTRIBUTION_ENABLED STD_OFF
[!ENDIF!]

#define XCP_MASTER_APPLICATION [!"$XcpMasterApplication"!]
#if (defined XCP_REMOTE_DESTINATIONS)
#error XCP_REMOTE_DESTINATIONS already defined
#endif
/** \brief Switch, indicating whether Remote destinations are enabled.
 **/
[!IF "count(text:split($XcpRemoteDestinations,' '))>0"!]
#define XCP_REMOTE_DESTINATIONS STD_ON
[!ELSE!]
#define XCP_REMOTE_DESTINATIONS STD_OFF
[!ENDIF!]

#if (defined XCP_MEMORY_ACCESS_AREAS)
#error XCP_MEMORY_ACCESS_AREAS already defined
#endif
/** \brief Switch, indicating whether memory access areas are configured.
 **/
[!IF "$XcpMemoryAccessAreas"!]
#define XCP_MEMORY_ACCESS_AREAS STD_ON
[!ELSE!]
#define XCP_MEMORY_ACCESS_AREAS STD_OFF
[!ENDIF!]

/* !LINKSTO Xcp.BswDistribution.Calibration.MemoryProxy.Usage,1 */
#if (defined XCP_CALIBRATION_MEMORY_ACCESS_AREAS)
#error XCP_CALIBRATION_MEMORY_ACCESS_AREAS already defined
#endif
/** \brief Switch, indicating whether calibration memory access areas are configured.
 **/
[!IF "$XcpCalibrationMemoryAccessAreas"!]
#define XCP_CALIBRATION_MEMORY_ACCESS_AREAS STD_ON
[!ELSE!]
#define XCP_CALIBRATION_MEMORY_ACCESS_AREAS STD_OFF
[!ENDIF!]

#if (defined XCP_DAQSTIM_MEMORY_ACCESS_AREAS)
#error XCP_DAQSTIM_MEMORY_ACCESS_AREAS already defined
#endif
/** \brief Switch, indicating whether daq stim memory access areas are configured.
 **/
[!IF "$XcpDaqStimMemoryAccessAreas = 'true'"!]
#define XCP_DAQSTIM_MEMORY_ACCESS_AREAS STD_ON
[!ELSE!]
#define XCP_DAQSTIM_MEMORY_ACCESS_AREAS STD_OFF
[!ENDIF!]

#if (defined XCP_RAM_CALLOUT_PROXY_ENABLED)
#error XCP_RAM_CALLOUT_PROXY_ENABLED already defined
#endif
/** \brief Switch, indicating whether User Callouts for Memory Proxy are configured.
 **/
[!IF "$RAMCalloutProxyEnabled = 'true'"!]
#define XCP_RAM_CALLOUT_PROXY_ENABLED STD_ON
[!ELSE!]
#define XCP_RAM_CALLOUT_PROXY_ENABLED STD_OFF
[!ENDIF!]

#ifdef XCP_ENABLE_DAQ_OPTIMIZATION
#error XCP_ENABLE_DAQ_OPTIMIZATION already defined
#endif
/** \brief Switch indicating whether RAM optimizations are enabled or not
 **/
#define XCP_ENABLE_DAQ_OPTIMIZATION [!"node:when(RamOptimizations/XcpEnableDaqOptimization='true','STD_ON','STD_OFF')"!]

#ifdef XCP_ENABLE_BIT_OFFSET
#error XCP_ENABLE_BIT_OFFSET already defined
#endif
/** \brief Switch indicating whether bit offsets are enabled in the context of RAM optimizations
 **/
#define XCP_ENABLE_BIT_OFFSET [!"node:when(node:exists(RamOptimizations/XcpEnableBitOffset) and RamOptimizations/XcpEnableBitOffset='true','STD_ON','STD_OFF')"!]

/*==================[type definitions]=======================================*/

[!IF "(XcpGeneral/XcpBlockWriteReadMemoryRAMMechanism = 'true')"!]
/** \brief Callbacks for access memory type definitions. */
typedef P2FUNC(uint8, XCP_APPL_CODE, Xcp_WriteToRAMCallbackType)
            (
              P2VAR( void, AUTOMATIC, XCP_APPL_DATA ) AddressPtr,
              P2CONST( uint8, AUTOMATIC, XCP_APPL_DATA ) DataPtr,
              uint8 DataLength
            );
typedef P2FUNC(uint8, XCP_APPL_CODE, Xcp_ReadFromRAMCallbackType)
            (
              P2VAR( void, AUTOMATIC, XCP_APPL_DATA ) AddressPtr,
              P2CONST( uint8, AUTOMATIC, XCP_APPL_DATA ) DataPtr,
              uint8 DataLength
            );

/** \brief Callbacks for access memory administration structure Type. */
typedef struct
{
    Xcp_WriteToRAMCallbackType writeToRAMCallback;
    Xcp_ReadFromRAMCallbackType readFromRAMCallback;
}Xcp_DescriptorCallbackType;

[!ENDIF!]

[!IF "$XcpTimestampIsEnabled = 'true'"!]
/** \brief Timestamp Type definition
 ** */
[!IF "XcpGeneral/XcpTimestampType = 'ONE_BYTE'"!]
typedef uint8  Xcp_TimestampType;
[!ENDIF!]
[!IF "XcpGeneral/XcpTimestampType = 'TWO_BYTE'"!]
typedef uint16 Xcp_TimestampType;
[!ENDIF!]
[!IF "XcpGeneral/XcpTimestampType = 'FOUR_BYTE'"!]
typedef uint32 Xcp_TimestampType;
[!ENDIF!]

[!ENDIF!] [!/* XcpGeneral/XcpTimestampType != 'NO_TIME_STAMP and XcpGeneral/XcpDaqSupported = 'true' */!]

[!IF "$XcpMaxDaq > 0"!]

/** \brief DAQ Id type */
[!IF "XcpGeneral/XcpIdentificationFieldType = 'RELATIVE_BYTE'"!]
typedef uint8 Xcp_DaqIdType;
[!ELSE!]
typedef uint16 Xcp_DaqIdType;
[!ENDIF!]

/** \brief Odt Type */
typedef uint[!"$OdtTypeSize"!] Xcp_OdtSizeType;
 
/** \brief Mirror Type */
typedef uint[!"$MirrorTypeSize"!] Xcp_MirrorSizeType;

[!IF "as:modconf('Xcp')[1]/RamOptimizations/XcpEnableDaqOptimization='false'"!][!//
/** \brief ODT Entry Type. */
typedef struct
{
  /* The address location of the ODT entry */
  P2VAR(uint8, TYPEDEF, XCP_APPL_DATA) Address;
  uint8 Length;     /* Size of the element in AG */
  uint8 BitOffset;  /* Position of the Bit to be considered */
  uint8 AddrExtn;   /* Address extension of the ODT */
} Xcp_OdtEntryType;
[!ENDIF!][!//

/** \brief ODT Table Type. */
typedef struct
{
[!IF "as:modconf('Xcp')[1]/RamOptimizations/XcpEnableDaqOptimization='false'"!][!//
  P2VAR(Xcp_OdtEntryType, TYPEDEF, XCP_VAR) OdtEntry; /* Pointer to either Xcp_DaqLists.Xcp_NonDynamicDaqLists.Xcp_OdtEntry or Xcp_DaqLists.Xcp_DynamicArea */
[!ELSE!][!//
  P2VAR(usize, TYPEDEF, XCP_VAR) OdtEntry; /* Pointer to either Xcp_OdtAddressTableRAM or Xcp_DaqLists.Xcp_DynamicArea */
[!ENDIF!][!//
  Xcp_OdtSizeType DTODataSize;                  /* Size of data segment in DTO package. */
  Xcp_MirrorSizeType MirrorOffsetPos;           /* Position index of data in OdtMirrorDataArray. */

  uint8 NrOfOdtEntries;                               /* Number of elements in the ODT */
} Xcp_OdtType;

/** \brief DAQ List properties. */
typedef struct
{
  P2VAR(Xcp_OdtType, TYPEDEF, XCP_VAR) OdtList; /* Pointer to the ODT Array or DAQ List */
  uint16 EventId;      /* Event channel number for this DAQ */
  uint8 Mode;          /* Current  mode information of the DAQ List.
                              Bit7: Resume status,
                              Bit6: Running status,
                              Bit5: PID_OFF,
                              Bit4: Time Stamp,
                              Bit2: Selected,
                              Bit1: Direction,
                              Bit0: Alternating */
  uint8 Priority;      /* Priority of this DAQ */
[!IF "XcpGeneral/XcpIdentificationFieldType = 'ABSOLUTE'"!]
  uint8 FirstPID;      /* The first PID only, when using absolute ODT number */
[!ENDIF!]
  uint8 MaxOdt;        /* Number of ODTs in this DAQ list */
  uint8 MaxOdtEntries; /* Maximum number of ODT entries into an Object Descriptor Table
                          of this DAQ list. */
  uint8 Prescaler;     /* Preset value of the prescaler */
  uint8 PrescalerCnt;  /* Current value of the prescaler */
  uint8 Properties;   /*  Configuration properties of the DAQ List:
                              Bit7: RESERVED,
                              Bit6: RESERVED
                              Bit5: RESERVED,
                              Bit4: RESERVED,
                              Bit3: STIM direction allowed,
                              Bit2: DAQ direction allowed,
                              Bit1: EventFixed,
                              Bit0: Predefined */
  uint8 Flags;         /* Additional configuration of the DAQ List:
                              Bit7: RESERVED,
                              Bit6: RESERVED
                              Bit5: RESERVED,
                              Bit4: RESERVED,
                              Bit3: OverloadCheckIsSkipped,
                              Bit2: SkipOverloadCheck,
                              Bit1: Configured,
                              Bit0: Ready to sample */
} Xcp_DaqType;

[!IF "text:tolower($StoreDaq) = 'true'"!]
/** \brief The administration structure saved in the NV upon a store request */
typedef struct
{
  uint16 ConfigurationSessionID;                        /* Session Configuration ID */
[!IF "$XcpDaqConfigType = 'DAQ_DYNAMIC'"!]
  Xcp_DaqIdType NoOfConfiguredDynamicDaqLists;          /* How many Dynamic DAQ lists were allocated
                                                           during  a runtime configuration */
[!IF "as:modconf('Xcp')[1]/RamOptimizations/XcpEnableDaqOptimization='true'"!][!//
  uint32 NoOfConfiguredOdts;
  uint32 NoOfConfiguredOdtEntries;
[!ENDIF!][!//
[!ENDIF!]
[!IF "$NoOfConnections > 1"!]
  uint8 ConnectionId;                                   /* the ID of the connection used 
                                                           SET_REQUEST(STORE_DAQ_REQ_RESUME) 
                                                           was sent. */
[!ENDIF!]
  uint8 Mode;                                           /* The requested mode:
                                                           Bit1: STORE_DAQ_REQ_NORESUME
                                                           Bit2: STORE_DAQ_REQ_RESUME
                                                           Bit3: CLEAR_DAQ_REQ
                                                         */
  uint8 SelectedDAQ[XCP_NUMBER_OF_BYTES_SELECTED_DAQ];   /* Each bit in this array corresponds to a
                                                         DAQ list. If the bit is set it means that
                                                         the DAQ list was selected to be stored */
} Xcp_StoreDAQType;
[!ENDIF!]  [!/* $StoreDaq  = 'true' */!]

[!IF "$XcpMaxDaq > $XcpDaqCount"!] [!/* We have static or predefined DAQ lists */!]
/** \brief Static/predefined DAQ list related information grouped into a contiguous area. */
typedef struct
{
  /* All DAQ lists wich are not dynamically allocated */
  Xcp_DaqType Xcp_Daq[XCP_NO_OF_NON_DYNAMIC_DAQ_LISTS];
  /* All ODTs wich are not dynamically allocated */
  Xcp_OdtType Xcp_Odt[XCP_NO_OF_NON_DYNAMIC_ODTS];
[!IF "as:modconf('Xcp')[1]/RamOptimizations/XcpEnableDaqOptimization='false'"!][!//
  /* !LINKSTO Xcp.RamOptimizations.XcpEnableDaqOptimization.False.DaqData,1 */
  /* All ODT entries wich are not dynamically allocated */
  Xcp_OdtEntryType Xcp_OdtEntry[XCP_NO_OF_NON_DYNAMIC_ODTENTRIES];
[!ENDIF!][!//
} Xcp_NonDynamicDaqListsType;
[!ENDIF!]

/** \brief All DAQ list related information grouped into a contiguous area
 ** Grouping them enables the DAQ lists storage in NV memory using a single NvM block. */
typedef struct
{
[!IF "$XcpDaqConfigType = 'DAQ_DYNAMIC'"!]
  /* Xcp_DynamicArea array must be the first variable of this structure due to alignment
   restrictions - the start address will be converted into a Xcp_DaqType structure. */
  uint8 Xcp_DynamicArea[XCP_DYNAMIC_AREA_SIZE];
[!ENDIF!]
  /* This dummy variable is added so that the entire structure is aligned to 4 bytes in order to
  prevent inconsistencies between what sizeof(Xcp_DaqLists) is providing and the actual size of
  Xcp_DaqLists in the map file after the linkage. Without this, the members of this structure are
  aligned to 2 bytes by the compiler, but the linker might align the entire structure to 4 bytes -
  thus adding extra bytes that the sizeof() will be unaware of.*/
  uint32 DummyVar;
[!IF "text:tolower($StoreDaq) = 'true'"!]
  Xcp_StoreDAQType Xcp_StoreDaq;
[!ENDIF!]
[!IF "$XcpMaxDaq > $XcpDaqCount"!]
  Xcp_NonDynamicDaqListsType Xcp_NonDynamicDaqLists;
[!ENDIF!]
} Xcp_AllDaqListsType;

/** \brief All Dto Data Mirror Type grouped into a contiguous area */
typedef struct
{
[!IF "$XcpDaqConfigType = 'DAQ_DYNAMIC'"!]
  /* Number of reserved bytes in Dynamic ODT Mirror */
  Xcp_MirrorSizeType DynamicMirrorReservedBytes;
  /* Array Mirror for ODTs from Dynamic DAQ */
  uint8  DynamicOdtMirrorDataArray[XCP_DYNAMIC_MIRROR_SIZE];
[!ENDIF!]
[!IF "$XcpMaxDaq > $XcpDaqCount"!]
  /* Array Mirror for ODTs from Non Dynamic DAQ */
  uint8  NonDynamicOdtMirrorDataArray[XCP_NON_DYNAMIC_MIRROR_SIZE];
[!ENDIF!]
} Xcp_AllDtoDataMirrorType;

[!ENDIF!][!/* MAX_DAQ > 0 */!]

/** \brief Definition for Xcp LPdu lenght type. */
[!IF "$MaxLPduBufferSize < bit:shl(1,8)"!]
typedef uint8 Xcp_LPduLengthType;
[!ELSEIF "$MaxLPduBufferSize < bit:shl(1,16)"!]
typedef uint16 Xcp_LPduLengthType;
[!ELSEIF "$MaxLPduBufferSize < bit:shl(1,32)"!]
typedef uint32 Xcp_LPduLengthType;
[!ELSE!]
  [!ERROR!]The Xcp_LPduLengthType cannot be calculated because the PDU buffer size exceed 2^32-1.[!ENDERROR!]
[!ENDIF!]

/** \brief Data structure storing property information regarding transmit PDU. */
typedef struct
{
[!IF "(XcpGeneral/XcpOnCanEnabled = 'true') or (XcpGeneral/XcpOnCanFDEnabled = 'true')"!]
  uint32 PduCanId;                  /* CAN/CANFD Id for the selected PDU */
[!ENDIF!]
  Xcp_LPduLengthType LPduStartPos;  /* the start position of the PDU data inside LPDU buffer 
                                       when FLX_ASSIGN is not used on that connection */
  PduIdType DestPduId;              /* store the transport layer ID for the referenced PDU. */
  PduIdType IntPduBufId;            /* map PDU to internal buffer Id */
  uint8 IntConnectionId;            /* maps PDU internal id towards a Xcp connection Id. */
  uint8 Properties;                 /* Additional configuration properties:
                                       Bit7: Support for trigger transmit API,
                                       Bit6: Support for Tx from confirmation,
                                       Bit5: Dynamic TxPdu,
                                       Bit4: Default state of dynamic TxPdu,
                                       Bit3: RESERVED,
                                       Bit2: Support for DAQ DTO packages,
                                       Bit1: Support for EV/SERV CTO packages,
                                       Bit0: Support for RES/ERR CTO packages */
}Xcp_TxLPduIdConfType;

/** \brief Data structure storing property information regarding receive PDU. */
typedef struct
{
[!IF "(XcpGeneral/XcpOnCanEnabled = 'true') or (XcpGeneral/XcpOnCanFDEnabled = 'true')"!]
  uint32 PduCanId;                 /* CAN Id for the selected PDU */
[!ENDIF!]
  Xcp_LPduLengthType LPduStartPos; /* the start position of the PDU data inside LPDU buffer
                                      when FLX_ASSIGN is not used on that connection */
  PduIdType IntPduBufId;           /* map PDU to internal buffer Id */
  uint8 IntConnectionId;           /* maps PDU internal id towards a Xcp connection Id. */
  uint8 Properties;                /* Additional configuration properties:
                                      Bit7: RESERVED,
                                      Bit6: Support for Rx from indication,
                                      Bit5: Dynamic RxPdu,
                                      Bit4: Default state of dynamic RxPdu,
                                      Bit3: RESERVED,
                                      Bit2: RESERVED,
                                      Bit1: Support for STIM DTO packages,
                                      Bit0: Support for CMD CTO packages */
}Xcp_RxLPduIdConfType;

[!IF "$SuportForSoAdSocketProtocolTcp = 'true'"!]
/** \brief Data structure mapping TCP/IP socket connection to Xcp internal connections. */
typedef struct
{
  SoAd_SoConIdType SoAdConId;      /* Socket connection Id. */
  uint8 IntConnectionId;           /* Xcp connection Id. */
}Xcp_TcpIpSocketConMapType;
[!ENDIF!]

/** \brief Transmit function pointer type */
typedef P2FUNC(Std_ReturnType, XCP_CONST, Xcp_TransmitFctPtrType)
            (
              PduIdType XcpTxPduId,
              P2CONST(PduInfoType, AUTOMATIC, XCP_APPL_DATA) PduInfoPtr
            );

/** \brief Configuration data structure for Xcp connection */
typedef struct
{
  P2CONST(PduIdType, TYPEDEF, XCP_APPL_CONST) TxPduIdList;     /* list of the Tx Pdus
                                                                 * mapped to connection*/
  Xcp_TransmitFctPtrType TransmitFunctionPtr;  /* transmit function pointer */
  PduLengthType PduLengthMax;    /* maximum size for a PDU */
  PduLengthType PduHeaderLength; /* Pdu header size */
  PduIdType TxPduMax;            /* number of mapped Tx PDUs to connection */
  PduIdType RxPduMax;            /* number of mapped Rx PDUs to connection */
[!IF "((XcpGeneral/XcpOnCanEnabled = 'true') or (XcpGeneral/XcpOnCanFDEnabled = 'true') or (XcpGeneral/XcpOnCddEnabled = 'true'))"!]
  PduIdType CtoSlavePduId;       /* PDU channel, used to receive CMD data GET_SLAVE_ID. */
  PduIdType BroadcastPduId;      /* PDU channel, used to receive bradcasted GET_SLAVE_ID. */
  [!IF "(XcpGeneral/XcpStimSupported = 'true') and (XcpGeneral/XcpSupportForGetDaqId = 'true')"!]
  PduIdType StimDtoPduId;        /* PDU channel, used to receive STIM packages(GET_DAQ_ID). */
  [!ENDIF!]
  [!IF "(XcpGeneral/XcpDaqSupported = 'true') and (XcpGeneral/XcpSupportForGetDaqId = 'true')"!]
  PduIdType DaqDtoPduId;         /* PDU channel, used to transmit DAQ packages(GET_DAQ_ID). */
  [!ENDIF!]
[!ENDIF!]
[!IF "XcpGeneral/XcpOnEthernetEnabled = 'true'"!]
  SoAd_SoConIdType SoAdTxConId;  /* Id of transmit socket connection */
  [!IF "$SuportForSoAdSocketProtocolUdp = 'true'"!]
  SoAd_SoConIdType SoAdRxConId;  /* Id of receive socket connection */
  [!ENDIF!]
[!ENDIF!]
  uint8 IntCounterId;            /* Id of internal package counter */
[!IF "(count(XcpConfig/*/XcpConnectionCfg/*[XcpConnectionInterfaceType = 'XcpConnectionOverCANFD']) > 0)"!]
  PduLengthType MaxDlcValue;     /* MAX_DLC value */
  uint8 FillValue;               /* Fill byte value */
[!ENDIF!]
[!IF "XcpGeneral/XcpOnFlexRayEnabled = 'true'"!]
  uint8 FlexRayNax;              /* FlexRay network address */
  PduLengthType FlexRayPackageAlignment; /* FlexRay package alignment */     
[!ENDIF!]
  uint8 Properties;              /* Additional connection configuration properties:
                                    Bit7: RESERVED,
                                    Bit6: RESERVED
                                    Bit5: RESERVED,
                                    Bit4: RESERVED,
                                    Bit3: MaxDlcRequired is enabled,
                                    Bit2: FLX_ASSIGN is supported,
                                    Bit1: Automatically open the socket connections,
                                    Bit0: Support for multiple Pdus in one frame */
  uint8 ConnectionType;          /* Xcp connection type */
  uint8 Xcp_Max_Cto;             /* connection specific MAX_CTO */
  uint16 Xcp_Max_Dto;             /* connection specific MAX_DTO */
[!IF "$XcpTimestampIsEnabled = 'true'"!]
  boolean Xcp_Timestamp_Enabled; /* flag indicating if DTO timestamps are enabled for this connection */
[!ENDIF!]
[!IF "XcpGeneral/XcpMasterBlockModeSupported = 'true'"!]
  uint8 Xcp_Max_BS;              /* connection specific maximum block size */
[!ENDIF!]
[!IF "XcpGeneral/XcpPgmSupported = 'true'"!]
  uint8 Xcp_Max_Cto_Pgm;         /* connection specific MAX_CTO for flash programming */
[!IF "XcpGeneral/XcpMasterBlockModePgmSupported"!]
  uint8 Xcp_Max_BS_Pgm;          /* connection specific maximum block size for flash programming */
[!ENDIF!]
[!ENDIF!] /* XcpPgmSupported = 'true' */
[!IF "XcpGeneral/XcpDaqSupported = 'true'"!]
  uint8 Xcp_Max_OdtEntry_Size_Daq; /* connection specific maximum odt entry size for direction DAQ */
[!IF "XcpGeneral/XcpDaqConfigType = 'DAQ_DYNAMIC'"!]
  uint8 Xcp_Max_Entries_Per_Odt_Dyn; /* connection specific maximum odt entries for dynamic DAQ */
[!IF "XcpGeneral/XcpTimestampType != 'NO_TIME_STAMP'"!]
  uint8 Xcp_Max_Entries_Per_Odt_Dyn_TS; /* connection specific maximum odt entries for dynamic DAQ with timestamps */
[!ENDIF!] /* XcpTimestampType != 'NO_TIME_STAMP' */
[!ENDIF!] /* XcpDaqConfigType = 'DAQ_DYNAMIC' */
[!IF "XcpGeneral/XcpStimSupported = 'true'"!]
  uint8 Xcp_Max_OdtEntry_Size_Stim; /* connection specific maximum odt entry size for direction STIM */
[!ENDIF!]
[!ENDIF!] /* XcpDaqSupported = 'true' */
}Xcp_ConnectionCfgType;

/** \brief Status information used to route lower layer Pdu Id to Xcp Internal Pdu Id. */
typedef struct
{
  PduIdType LowerLayerPduId;  /** the lower layer Pdu Id used by the lower layer to notify Xcp */
  PduIdType XcpInternalPduId; /** the internal Xcp Pdu Id mapped to the lower layer Pdu Id */
}Xcp_LPduIdRouteConfType;

[!IF "count(XcpUserCommand/*) > 0"!]
/** \brief Pointer to sub-command call-out type definitions. */
typedef P2FUNC(uint8, XCP_APPL_CODE, Xcp_SuCommandCalloutType)
            (
              P2CONST( uint8, AUTOMATIC, XCP_APPL_DATA ) ParamPtr,
              uint8 Length,
              P2VAR( uint8, AUTOMATIC, XCP_APPL_DATA ) SubCmdResponse,
              uint8 RespLength
            );

/** \brief Configuration type for user commands. */
typedef struct
{
  Xcp_SuCommandCalloutType SubCmdCallout; /** pointer to user defined call-out function */
  uint8 SubCmdCode;                       /** the Code configured for the SubCommand */
  uint8 SubCmdLength;                     /** the Length configured for the SubCommand */
  uint8 SubCmdSeedKey;                    /** boolean parameter used to decide if Seed and key support is enabled */
  uint8 SubCmdRespLength;                 /** the Length configured for the SubCommand */
}Xcp_UserCommandConfType;
[!ENDIF!]

/*==================[external function declarations]=========================*/

[!IF "XcpGeneral/XcpEventTriggerMainFunc = 'true' or 
      XcpGeneral/XcpBuildChecksumMainFunctionSupport = 'true' or
      node:exists(XcpGeneral/XcpSupportComMainFunctions) = 'true'"!]
#define XCP_START_SEC_CODE
#include <Xcp_MemMap.h>
[!ENDIF!]

[!IF "XcpGeneral/XcpEventTriggerMainFunc = 'true'"!]
  [!IF "node:exists(XcpConfig/*[1]/XcpEventChannel)"!]
    [!IF "num:i(count(XcpConfig/*[1]/XcpEventChannel/*)) > 0"!]
      [!LOOP "XcpConfig/*[1]/XcpEventChannel/*"!]
        [!IF "XcpEventChannelTimeCycle > 0"!]   
/** \brief Scheduled functions of the XCP module for event [!"@name"!].
 ** This functions handle the events with [!"XcpEventChannelNumber"!] Id.
 */
extern FUNC(void, XCP_CODE) Xcp_MainFunction_[!"@name"!](void);
        [!ENDIF!]
      [!ENDLOOP!]
    [!ENDIF!] 
  [!ENDIF!]
[!ENDIF!]

[!IF "XcpGeneral/XcpBuildChecksumMainFunctionSupport = 'true'"!]
/** \brief Scheduled function of the XCP module for asynchronous calculation 
 ** of the build checksum job operation.
 */
extern FUNC(void, XCP_CODE) Xcp_CrcMainFunction(void);
[!ENDIF!]

[!IF "node:exists(XcpGeneral/XcpSupportComMainFunctions) = 'true'"!]
/** \brief Scheduled function of the XCP module for processing the reception.
 */
extern FUNC(void, XCP_CODE) Xcp_RxMainFunction(void);

/** \brief Scheduled function of the XCP module for processing the transmission.
 */
extern FUNC(void, XCP_CODE) Xcp_TxMainFunction(void);
[!ENDIF!]

[!IF "XcpGeneral/XcpEventTriggerMainFunc = 'true' or 
      XcpGeneral/XcpBuildChecksumMainFunctionSupport = 'true' or
      node:exists(XcpGeneral/XcpSupportComMainFunctions) = 'true'"!]
#define XCP_STOP_SEC_CODE
#include <Xcp_MemMap.h>
[!ENDIF!]

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

#define XCP_START_SEC_CONST_UNSPECIFIED
#include <Xcp_MemMap.h>

/** \brief Configuration placeholder for Xcp_Init() call. */
extern CONST( Xcp_ConfigType, XCP_CONST ) [!"node:name(XcpConfig/*[1])"!];

#define XCP_STOP_SEC_CONST_UNSPECIFIED
#include <Xcp_MemMap.h>

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

[!IF "$XcpMaxDaq > 0"!][!//

#define XCP_START_SEC_VAR_SAVED_ZONE_UNSPECIFIED
#include <Xcp_MemMap.h>

/** \brief All DAQ list related information grouped together. */
extern VAR(Xcp_AllDaqListsType, XCP_VAR_POWER_ON_CLEARED) Xcp_DaqLists;

[!IF "$XcpNoOfNonDynamicODTEntries > 0 and RamOptimizations/XcpEnableDaqOptimization='true'"!][!//
/* !LINKSTO Xcp.RamOptimizations.XcpEnableDaqOptimization.True.DaqData,1 */
/** \brief The addresses of all ODT entries */
extern P2VAR(uint8, XCP_APPL_DATA, XCP_VAR_POWER_ON_CLEARED) Xcp_OdtAddressTableRAM[XCP_NO_OF_NON_DYNAMIC_ODTENTRIES];
[!ENDIF!][!//

#define XCP_STOP_SEC_VAR_SAVED_ZONE_UNSPECIFIED
#include <Xcp_MemMap.h>

[!IF "$XcpNoOfNonDynamicODTEntries > 0 and RamOptimizations/XcpEnableDaqOptimization='true'"!][!//
#define XCP_START_SEC_VAR_SAVED_ZONE_UNSPECIFIED_8
#include <Xcp_MemMap.h>

/* !LINKSTO Xcp.RamOptimizations.XcpEnableDaqOptimization.True.DaqData,1 */
/** \brief The lengths of all ODT entries */
extern VAR(uint8,XCP_VAR_POWER_ON_CLEARED) Xcp_OdtLengthTableRAM[XCP_NO_OF_NON_DYNAMIC_ODTENTRIES];

[!IF "RamOptimizations/XcpEnableBitOffset='true'"!][!//
/* !LINKSTO Xcp.RamOptimizations.XcpEnableDaqOptimization.True.DaqData,1 */
/* !LINKSTO Xcp.RamOptimizations.XcpEnableBitOffset.True.DaqData,1 */
/* !LINKSTO Xcp.RamOptimizations.XcpEnableBitOffset.False.DaqData,1 */
/** \brief The bit offsets of all ODT entries */
extern VAR(uint8,XCP_VAR_POWER_ON_CLEARED) Xcp_OdtBitOffsetTableRAM[XCP_NO_OF_NON_DYNAMIC_ODTENTRIES];
[!ENDIF!][!//

#define XCP_STOP_SEC_VAR_SAVED_ZONE_UNSPECIFIED_8
#include <Xcp_MemMap.h>
[!ENDIF!][!//

[!ENDIF!][!// $XcpMaxDaq > 0

[!IF "(XcpGeneral/XcpBlockWriteReadMemoryRAMMechanism = 'true')"!]

#define XCP_START_SEC_VAR_UNSPECIFIED
#include <Xcp_MemMap.h>

/** \brief Variable to hold the current information for accessing memory with a callout */
extern VAR(Xcp_DescriptorCallbackType, XCP_VAR) Xcp_DescriptorCallbackTypePtr;

#define XCP_STOP_SEC_VAR_UNSPECIFIED
#include <Xcp_MemMap.h>

[!ENDIF!]

#define XCP_START_SEC_VAR_8
#include <Xcp_MemMap.h>

#if ( (XCP_DEFAULT_XCP_MODULE_STATE == STD_ON) || (XCP_ENABLE_XCP_CONTROL_API == STD_ON) )
/** \brief Variable to hold the current state of Xcp module */
extern VAR(uint8, XCP_VAR) Xcp_ControlStateOfXcpModule;
#endif 

#define XCP_STOP_SEC_VAR_8
#include <Xcp_MemMap.h>
/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef XCP_CFG_H */
/*==================[end of file]============================================*/
