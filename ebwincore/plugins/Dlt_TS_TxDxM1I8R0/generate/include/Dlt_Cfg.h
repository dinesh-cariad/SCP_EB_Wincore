/**
 * \file
 *
 * \brief AUTOSAR Dlt
 *
 * This file contains the implementation of the AUTOSAR
 * module Dlt.
 *
 * \version 1.8.6
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef DLT_CFG_H
#define DLT_CFG_H

/* This file contains all target independent public configuration declarations
 * for the AUTOSAR module Dlt. */
/* !LINKSTO Dlt.swdd.DltConfig.Template.Dlt_Cfg_h,1 */

[!INCLUDE "Dlt_Vars.m"!][!//
/*==================[includes]==============================================*/

#include <Std_Types.h>                            /* AUTOSAR standard types */

#include <TSAutosar.h>                        /* EB specific standard types */

#include <ComStack_Cfg.h>

[!IF "DltGeneral/DltRteUsage = 'true'"!]
#include <Rte_Dlt_Type.h>
[!ENDIF!]
/*==================[macros]================================================*/

[!IF "node:exists(DltMultipleConfigurationContainer/DltPduConfig/DltRxPduId)"!][!//
  [!VAR "DltOnEthernet" = "'false'"!][!//
  [!VAR "DltSocketId" = "0"!][!//
  [!IF "node:exists(as:modconf('PduR')) and node:exists(as:modconf('SoAd'))  and node:exists(DltMultipleConfigurationContainer/DltPduConfig/DltRxPduRef) and node:exists(as:ref(as:modconf('PduR')/PduRRoutingTables/*/PduRRoutingTable/*/PduRRoutingPath/*/PduRDestPdu/*/PduRDestPduRef)) "!][!//
    [!VAR "DltRxPduName" = "((as:ref(DltMultipleConfigurationContainer/DltPduConfig/DltRxPduRef))/@name)"!][!//
    [!IF "node:exists(as:modconf('PduR')/PduRRoutingTables/*/PduRRoutingTable/*/PduRRoutingPath/*/PduRDestPdu/*/PduRDestPduRef[as:ref(.)/@name=$DltRxPduName])"!][!//
      [!VAR "PduRUpperRefName" = "as:ref(as:modconf('PduR')/PduRRoutingTables/*/PduRRoutingTable/*/PduRRoutingPath/*/PduRDestPdu/*/PduRDestPduRef[as:ref(.)/@name=$DltRxPduName]/../../../PduRSrcPdu/PduRSrcPduRef)/@name"!][!//
      [!IF "node:exists(as:modconf('SoAd')/SoAdConfig/*/SoAdSocketRoute/*/SoAdSocketRouteDest/*/SoAdRxPduRef[node:ref(.)/@name=$PduRUpperRefName])"!][!//
        [!VAR "DltOnEthernet" = "'true'"!][!//
        [!VAR "DltSocketId" = "as:ref(as:modconf('SoAd')/SoAdConfig/*/SoAdSocketRoute/*/SoAdSocketRouteDest/*/SoAdRxPduRef[as:ref(.)/@name=$PduRUpperRefName]/../../../SoAdRxSocketConnOrSocketConnBundleRef)/SoAdSocketId"!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
/** \brief Switch indicating whether the underlying communication layer is Ethernet or not  */
#define DLT_ON_ETHERNET          [!IF "$DltOnEthernet"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
/** \brief The Socket ID on which the reception happens if Dlt is on Ethernet  */
#define DLT_RX_SOCKET_ID         [!IF "$DltOnEthernet"!][!"$DltSocketId"!][!ELSE!]0xFFU[!ENDIF!]
[!ENDIF!][!//

#define STBM_TIMESTAMP                        1U
#define OS_TIMESTAMP                          2U

#define AUTOSAR_421                           0U
#define AUTOSAR_422                           1U
#define AUTOSAR_431                           2U

#if (defined DLT_LITTLE_ENDIAN)
#error DLT_LITTLE_ENDIAN already defined
#endif
/** \brief Symbolic name for little endianness */
#define DLT_LITTLE_ENDIAN                     0U

#if (defined DLT_BIG_ENDIAN)
#error DLT_BIG_ENDIAN already defined
#endif
/** \brief Symbolic name for big endianness */
#define DLT_BIG_ENDIAN                        1U

/** \brief Length of the "Message info" header field */
#define DLT_MSIN_HEADER_LEN             1U
/** \brief Length of the "Number of arguments" header field */
#define DLT_NOAR_HEADER_LEN             1U
/** \brief Length of the "Application ID" header field */
#define DLT_APID_HEADER_LEN             4U
/** \brief Length of the "Context ID" header field */
#define DLT_CTID_HEADER_LEN             4U

/* !LINKSTO SWS_Dlt_00458,1 */
/** \brief Minimum header length (mandatory HTYP, MCNT and LEN fields */
#define DLT_MIN_HEADER_LENGTH           4U
/** \brief Length of the "ECU ID" header field */
#define DLT_ECU_HEADER_LENGTH           4U
/** \brief Length of the "Session ID" header field */
#define DLT_SEID_HEADER_LENGTH          4U
/** \brief Length of the "Timestamp" header field */
#define DLT_TMSP_HEADER_LENGTH          4U

/** \brief Length of extended header */
#define DLT_EXT_HEADER_LENGTH           (DLT_MSIN_HEADER_LEN + DLT_NOAR_HEADER_LEN + \
                                         DLT_APID_HEADER_LEN + DLT_CTID_HEADER_LEN)
/* AUTOSAR in use APIs */
#define DLT_DEFAULT_ASR_RETVAL          [!IF "DltMultipleConfigurationContainer/DltServiceAPI/DltServiceAPI = 'AUTOSAR_421'"!]AUTOSAR_421[!ELSEIF "DltMultipleConfigurationContainer/DltServiceAPI/DltServiceAPI = 'AUTOSAR_422'"!]AUTOSAR_422[!ELSE!]AUTOSAR_431[!ENDIF!]

#if (defined DLT_MESSAGE_OPTIONS_BITFIELD)
#error DLT_MESSAGE_OPTIONS_BITFIELD is already defined.
#endif
/* !LINKSTO EB.Req.DltMessageOptionsBitField, 1 */
/** \brief Defines which AUTOSAR release is used for the "options" member bit fields */
#define DLT_MESSAGE_OPTIONS_BITFIELD [!IF "DltGeneral/DltMessageOptionsBitField = 'AUTOSAR_421'"!]AUTOSAR_421[!ELSE!]AUTOSAR_431[!ENDIF!]

#if (defined DLT_VERSION_INFO_API)
#error DLT_VERSION_INFO_API is already defined.
#endif
/** \brief Switch, indicating if Version Info is activated for Dlt */
#define DLT_VERSION_INFO_API             [!IF "(node:exists(DltGeneral/DltVersionInfoApi)) and (DltGeneral/DltVersionInfoApi = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#define DLT_DEV_ERROR_DETECT             [!IF "DltGeneral/DltDevErrorDetect = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DLT_INCLUDE_RTE)
#error DLT_INCLUDE_RTE is already defined.
#endif
/** \brief Defines whether a Rte module configuration is available.
 **        If it is available then it is automatically used by the Dlt module.
 **        STD_ON:  Rte configuration is available.
 **        STD_OFF: Rte configuration is not available.
 **/
#define DLT_INCLUDE_RTE                    [!IF "DltGeneral/DltRteUsage = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Macro used for the visibility of symbols generated by the Rte for the DltControlService interface when DltServiceAPI == AUTOSAR_431 */
#define DLT_CONTROL_SERVICE_PORT           [!IF "count(DltSwc/*[(DltProvideControlServicePort = 'true')]) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Macro used for the visibility of symbols generated by the Rte for the DltSwcMessageService interface when DltServiceAPI == AUTOSAR_431 */
#define DLT_MESSAGE_SERVICE_PORT           [!IF "count(DltSwc/*[(DltProvideSwcMessageServicePort = 'true')]) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#define DLT_IMPLEMENT_EXTENDED_HEADER      [!IF "DltGeneral/DltImplementExtendedHeader = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#define DLT_IMPLEMENT_FILTER_MESSAGES      [!IF "DltGeneral/DltImplementFilterMessages = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!IF "node:exists(DltGeneral/DltGeneralStbMTimeBaseRef) and node:refvalid(DltGeneral/DltGeneralStbMTimeBaseRef)"!][!//
#define DLT_IMPLEMENT_TIMESTAMP            STBM_TIMESTAMP
#define DLT_STBM_TIMEBASE_REF_ID               [!"node:value(as:ref(./DltGeneral/DltGeneralStbMTimeBaseRef)/StbMSynchronizedTimeBaseIdentifier)"!]U
[!ELSEIF "DltGeneral/DltImplementTimestamp = 'true'"!][!//
/* !LINKSTO Dlt.Config.StbMTimeBaseRef.NotConfigured,2 */
#define DLT_IMPLEMENT_TIMESTAMP            OS_TIMESTAMP
[!ELSE!][!//
#define DLT_IMPLEMENT_TIMESTAMP            STD_OFF
[!ENDIF!][!//

/* !LINKSTO SWS_Dlt_00014,1 */
#define DLT_IMPLEMENT_VERBOSE              [!IF "DltGeneral/DltImplementVerboseMode  = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#define DLT_IMPLEMENT_NV_RAM_STORAGE       [!IF "DltGeneral/DltImplementNVRamStorage = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#define DLT_IMPLEMENT_VFB_TRACE            [!IF "DltGeneral/DltImplementVfbTrace = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!IF "DltGeneral/DltImplementVfbTrace = 'true'"!][!//
[!IF "DltGeneral/DltVfbSendHookFunctionParameters = 'false'"!][!//
#if (defined DLT_VFB_TRACE_PAYLOAD_MAX_SIZE)
#error DLT_VFB_TRACE_PAYLOAD_MAX_SIZE is already defined
#endif
/** \brief Maximum size of the payload for a configured VFB hook function:
 *         - 4 bytes represent the MessageId;
 *         - 6 bytes represent the ContextId and the payload length when PduLengthType is uint16
 *         - 8 bytes represent the ContextId and the payload length when PduLengthType is uint32 */
#if (TS_SIZE_PduLengthType == 2U)
#define DLT_VFB_TRACE_PAYLOAD_MAX_SIZE  10U
#else /* #if (TS_SIZE_PduLengthType == 4U) */
#define DLT_VFB_TRACE_PAYLOAD_MAX_SIZE  12U
#endif /* #if (TS_SIZE_PduLengthType == 2U) */
[!ELSE!][!//
/** \brief Maximum size of the payload for a configured VFB hook function:
 *         - configurable amount of bytes for payload
 *         - 6 bytes represent the ContextId and the payload length when PduLengthType is uint16
 *         - 8 bytes represent the ContextId and the payload length when PduLengthType is uint32 */
#if (TS_SIZE_PduLengthType == 2U)
#define DLT_VFB_TRACE_PAYLOAD_MAX_SIZE  ([!"DltGeneral/DltVfbTracePayloadMaxSize"!]U + 6U)
#else /* #if (TS_SIZE_PduLengthType == 4U) */
#define DLT_VFB_TRACE_PAYLOAD_MAX_SIZE  ([!"DltGeneral/DltVfbTracePayloadMaxSize"!]U + 8U)
#endif /* #if (TS_SIZE_PduLengthType == 2U) */
[!ENDIF!][!//

[!IF "node:exists(DltGeneral/DltVfbMainFunctionPeriod)"!][!//
#if (defined DLT_VFB_MAINFUNCTION_PERIOD)
#error DLT_VFB_MAINFUNCTION_PERIOD is already defined
#endif
/** \brief Period (in milliseconds) of Dlt_VfbMainFunction() */
#define DLT_VFB_MAINFUNCTION_PERIOD        [!"num:i(DltGeneral/DltVfbMainFunctionPeriod * 1000)"!]U
[!ENDIF!][!//

#if (defined DLT_VFB_TRACE_NO_OF_INTERRUPTS)
#error DLT_VFB_TRACE_NO_OF_INTERRUPTS is already defined
#endif
/** \brief The maximum number of interrupts possible within the system */
#define DLT_VFB_TRACE_NO_OF_INTERRUPTS  [!"DltGeneral/DltVfbTraceNoOfInterrupts"!]U

#if (defined DLT_RTE_MODULE_ID)
#error DLT_RTE_MODULE_ID is already defined
#endif
/* !LINKSTO Dlt.ASR43.VfbTracing.RteModuleIdAsTraceFunctionSessionId, 1 */
/** \brief ModuleId of the Rte module to be used as SessionId for the VFB hook functions */
#define DLT_RTE_MODULE_ID               [!"num:i(as:modconf('Rte')/CommonPublishedInformation/ModuleId)"!]U
[!ENDIF!][!//

[!IF "ReportToDem/DltVfbBufferFullReportToDem = 'DET'"!][!//
#if (defined DLT_E_DEMTODET_VFB_BUFFER_FULL)
#error DLT_E_DEMTODET_VFB_BUFFER_FULL already defined
#endif
/* DET error Id for Dlt full VFB buffer */
#define DLT_E_DEMTODET_VFB_BUFFER_FULL  [!//
[!"ReportToDem/DltVfbBufferFullDemDetErrorId"!]U
[!ELSEIF "ReportToDem/DltVfbBufferFullReportToDem = 'DEM'"!]
#if (defined DLT_E_VFB_BUFFER_FULL_DEM_EVENT_ID)
#error DLT_E_VFB_BUFFER_FULL_DEM_EVENT_ID already defined
#endif
/** \brief DEM Event ID for Dlt bull VFB buffer */
#define DLT_E_VFB_BUFFER_FULL_DEM_EVENT_ID [!//
[!"node:value(as:ref(DltGeneral/DltDemEventParameterRefs/DLT_E_VFB_BUFFER_FULL)/DemEventId)"!]U
[!ENDIF!][!//

/* !LINKSTO Dlt.ASR43.VfbTracing.ContextInternalHandling, 1 */
/** \brief No of ContextIds (VFB trace functions in this case) registered by the Rte */
#define DLT_VFB_CONTEXT_COUNT              [!"num:i($VfbTraceFunctionCount)"!]U

/** \brief  maximum response size for services */
#define DLT_MAX_CONTROL_MESSAGE_RESPONSE_SIZE [!IF "node:exists(DltGeneral/DltControlMessageMaxResponseSize)"!][!"DltGeneral/DltControlMessageMaxResponseSize"!]U[!ELSE!]4U[!ENDIF!]


[!IF "node:value(DltMultipleConfigurationContainer/DltServiceAPI/DltServiceAPI) != 'AUTOSAR_431'"!][!//
/** \brief No of unique application ids that can be registered by the user. */
#define DLT_MAX_COUNT_APP_IDS              [!"DltMemory/DltMaxCountAppIds"!]U
/** \brief No of unique context ids that can be registered by the user. */
#define DLT_MAX_COUNT_CONTEXT_IDS          [!"DltMemory/DltMaxCountContextIds"!]U
[!ELSE!][!//

/** \brief No of unique context ids registered in the configuration. */
#define DLT_MAX_COUNT_CONTEXT_IDS          ([!"num:i(count(DltSwc/*/DltSwcContext/*))"!]U + DLT_VFB_CONTEXT_COUNT)

/** \brief Maximum number of contexts allowed to be registered per application. */
#define DLT_MAX_COUNT_CONTEXT_IDS_PER_APP_ID  [!"num:i($MaxCountCtxtPerAPP)"!]U

/** \brief No of unique application ids registered in the configuration. */
#define DLT_MAX_COUNT_APP_IDS [!IF "(num:i($VfbTraceFunctionCount) > 0)"!][!"num:i($TotalAppIDNo + 1)"!]U[!ELSE!][!"num:i($TotalAppIDNo)"!]U[!ENDIF!]

#if (defined APPCTXDEXLEN)
#error APPCTXDEXLEN already defined
#endif
/** \brief Maximum length of application/context description information */
#define APPCTXDEXLEN       0xFFU
/** \brief 4 bytes of application id and 2 bytes for no of application registered (index or total no)
 * depending either the requested application is wildcard or not */
#define DLT_APP_BUFFER_SIZE_WITHOUT_DESC          6U
/** \brief 4 bytes of context id + 1 byte for log level + 1 byte for trace status  */
#define DLT_CTXT_BUFFER_SIZE_WITHOUT_DESC         6U
/** \brief 4 bytes of application id and 2 bytes for no of application registered (index or total no)
* depending either the requested application is wildcard or not */
#define DLT_APP_BUFFER_SIZE_WITH_DESC          7U
/** \brief 4 bytes of context id + 1 byte for log level + 1 byte for trace status  */
#define DLT_CTXT_BUFFER_SIZE_WITH_DESC         7U
/** \brief 2 bytes for describing the no of registered application or context */
#define DLT_REGISTERED_APP_OR_CTXT_NO_SIZE        2U

[!ENDIF!][!//

[!IF "DltMultipleConfigurationContainer/DltServiceAPI/DltServiceAPI = 'AUTOSAR_431'"!][!//
/** \brief Macro for Unregister/Register Notification for each ContextId/ApplicationId tuple*/
#define DLT_REGISTER_CONTEXT_NOTIFICATION    [!IF "DltGeneral/DltGeneralRegisterContextNotification = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
/** \brief Default max Log Level Threshold for tuple ApplicationId/ContextId */
#define DLT_DEFAULT_MAX_LOG_LEVEL             [!"DltConfigSet/*/DltLogLevelSetting/*/DltDefaultLogLevel"!]
/** \brief Default Trace Status for tuple ApplicationId/ContextId */
#define DLT_DEFAULT_TRACE_STATUS              [!IF "DltConfigSet/*/DltTraceStatusSetting/*/DltDefaultTraceStatus = 'true'"!]DLT_TRACE_STATUS_ON[!ELSE!]DLT_TRACE_STATUS_OFF[!ENDIF!]
[!ELSE!]
/** \brief Macro for Unregister/Register Notification for each ContextId/ApplicationId tuple*/
#define DLT_REGISTER_CONTEXT_NOTIFICATION     STD_OFF

#define DLT_DEFAULT_MAX_LOG_LEVEL             [!"DltMultipleConfigurationContainer/DltMessageFiltering/DltDefaultMaxLogLevel"!]U

#define DLT_DEFAULT_TRACE_STATUS              [!IF "DltMultipleConfigurationContainer/DltMessageFiltering/DltDefaultTraceStatus = 'true'"!]DLT_TRACE_STATUS_ON[!ELSE!]DLT_TRACE_STATUS_OFF[!ENDIF!]
/** \brief This is the maximum count for Context IDs per Application ID.*/
#define DLT_MAX_COUNT_CONTEXT_IDS_PER_APP_ID  [!"DltMemory/DltMaxCountContextIdsPerAppId"!]U
/** \brief Buffer size for storing Dlt messages for waiting to transmit over the Network (send buffer).*/
#define DLT_MESSAGE_BUFFER_SIZE               [!"DltMemory/DltMessageBufferSize"!]U
 /** \brief The maximum log level a received message (from SW-C to Dlt) can have.*/
#define DLT_FACTORY_DEFAULT_MAX_LOG_LEVEL     [!"DltMultipleConfigurationContainer/DltMessageFiltering/DltFactoryDefaultMaxLogLevel"!]U
/** \brief The maximum length of a Dlt log or trace message.*/
#define DLT_MAX_MESSAGE_LENGTH                [!"DltMultipleConfigurationContainer/DltProtocol/DltMaxMessageLength"!]U
[!ENDIF!]
/** \brief Number of software components that are configured to be notified in case of log level and trace status changes. */
#define DLT_NUM_SWCS_TO_NOTIFY                [!"num:i($DltNumSwcsToNotify)"!]U

#define DLT_FILTER_MESSAGES                   [!IF "DltMultipleConfigurationContainer/DltMessageFiltering/DltFilterMessages = 'true'"!]DLT_FILTER_MESSAGES_ON[!ELSE!]DLT_FILTER_MESSAGES_OFF[!ENDIF!]

#define DLT_HEADER_PAYLOAD_ENDIANES           [!IF "DltMultipleConfigurationContainer/DltProtocol/DltHeaderPayloadEndianes = 'LittleEndian'"!]DLT_LITTLE_ENDIAN[!ELSE!]DLT_BIG_ENDIAN[!ENDIF!]

#define DLT_HEADER_USE_ECU_ID                 [!IF "DltMultipleConfigurationContainer/DltProtocol/DltHeaderUseEcuId = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#define DLT_HEADER_USE_EXTENDED_HEADER        [!IF "DltMultipleConfigurationContainer/DltProtocol/DltHeaderUseExtendedHeader = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#define DLT_HEADER_USE_SESSION_ID             [!IF "DltMultipleConfigurationContainer/DltProtocol/DltHeaderUseSessionID = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!IF "DltGeneral/DltImplementTimestamp = 'true'"!][!//
#define DLT_HEADER_USE_TIMESTAMP              [!IF "DltMultipleConfigurationContainer/DltProtocol/DltHeaderUseTimestamp = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!ENDIF!][!//
#define DLT_INIT_HEADER_LENGTH                ((DLT_MIN_HEADER_LENGTH)[!IF "DltMultipleConfigurationContainer/DltProtocol/DltHeaderUseEcuId = 'true'"!] + (DLT_ECU_HEADER_LENGTH)[!ENDIF!][!IF "DltMultipleConfigurationContainer/DltProtocol/DltHeaderUseSessionID = 'true'"!] + (DLT_SEID_HEADER_LENGTH)[!ENDIF!][!IF "DltMultipleConfigurationContainer/DltProtocol/DltHeaderUseTimestamp = 'true'"!] + (DLT_TMSP_HEADER_LENGTH)[!ENDIF!][!IF "DltMultipleConfigurationContainer/DltProtocol/DltHeaderUseExtendedHeader = 'true'"!] + (DLT_EXT_HEADER_LENGTH)[!ENDIF!])

#define DLT_USE_VERBOSE_MODE                  [!IF "DltMultipleConfigurationContainer/DltProtocol/DltUseVerboseMode = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!IF "DltMultipleConfigurationContainer/DltProtocol/DltEcuIdChoice = 'Value'"!][!//
#define DLT_ECU_ID                            "[!"DltMultipleConfigurationContainer/DltProtocol/DltEcuId"!]"
[!ENDIF!][!//
#define DLT_HEADER_PAYLOAD_ENDIANES_BIT       [!IF "DltMultipleConfigurationContainer/DltProtocol/DltHeaderPayloadEndianes = 'BigEndian'"!]0x02[!ELSE!]0x00[!ENDIF!]U

#define DLT_HEADER_CONFIG_ECU_ID_BIT          [!IF "DltMultipleConfigurationContainer/DltProtocol/DltHeaderUseEcuId = 'true'"!]0x04[!ELSE!]0x00[!ENDIF!]U

#define DLT_HEADER_CONFIG_EXTENDED_HEADER_BIT [!IF "DltMultipleConfigurationContainer/DltProtocol/DltHeaderUseExtendedHeader = 'true'"!]0x01[!ELSE!]0x00[!ENDIF!]U

#define DLT_HEADER_CONFIG_SESSION_ID_BIT      [!IF "DltMultipleConfigurationContainer/DltProtocol/DltHeaderUseSessionID = 'true'"!]0x08[!ELSE!]0x00[!ENDIF!]U

#if (DLT_IMPLEMENT_TIMESTAMP != STD_OFF)
#define DLT_HEADER_CONFIG_TIMESTAMP_BIT       [!IF "DltMultipleConfigurationContainer/DltProtocol/DltHeaderUseTimestamp = 'true'"!]0x10[!ELSE!]0x00[!ENDIF!]U
#endif /* DLT_IMPLEMENT_TIMESTAMP != STD_OFF */
/* !LINKSTO Dlt.RxPath.MessageReception.RxPDU, 1 */
[!IF "node:exists(DltMultipleConfigurationContainer/DltPduConfig/DltRxPduId)"!]
#define DLT_RX_PDU_ID                              [!"DltMultipleConfigurationContainer/DltPduConfig/DltRxPduId"!]U
#define DLT_RX_MESSAGE_BUFFER_SIZE                 [!"DltMemory/DltReceptionBufferSize"!]U
#define DLT_RECEPTION_CONTROL_MESSAGES_ENABLED     STD_ON
[!ELSE!]
#define DLT_RECEPTION_CONTROL_MESSAGES_ENABLED     STD_OFF
[!ENDIF!]
[!IF "DltMultipleConfigurationContainer/DltServiceAPI/DltServiceAPI != 'AUTOSAR_431'"!]
#define DLT_TX_PDU_ID                         [!"DltMultipleConfigurationContainer/DltPduConfig/DltTxPduId"!]U
[!ENDIF!]

#define DLT_ENABLE_MULTIPLE_FRAMES            [!IF "DltMultipleConfigurationContainer/DltPduConfig/DltEnableMultipleFrames = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Defines if DLT messages are discarded or not when transmit
 ** request to PduR return an error. */
/* !LINKSTO Dlt.Transmit.DltDiscardMsgTxFail.Config,1 */
#define DLT_DISCARD_MSG_TX_FAIL            [!IF "DltMultipleConfigurationContainer/DltPduConfig/DltDiscardMsgTxFail = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* !LINKSTO Dlt.TrafficShaper.ParamConfig,1 */
#ifndef DLT_TRAFFIC_SHAPER_ENABLED
#define DLT_TRAFFIC_SHAPER_ENABLED            [!IF "DltGeneral/DltEnableTrafficShaper = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
#endif

/* Dlt_MainFunction() period in milliseconds */
#define DLT_MAINFUNCTION_PERIOD               [!"num:i(DltMultipleConfigurationContainer/DltMainFunctionPeriod * 1000)"!]U


#if (defined DLT_TUPLE_PLATFORM_ENDIANNESS_DISABLE)
#error DLT_TUPLE_PLATFORM_ENDIANNESS_DISABLE is already defined
#endif
[!IF "DltMultipleConfigurationContainer/DltServiceAPI/DltServiceAPI = 'AUTOSAR_431'"!]
/** \brief Macro which allows interpretation of the SwC tuples MSB only or based on platform byte order */
#define DLT_TUPLE_PLATFORM_ENDIANNESS_DISABLE        [!IF "DltGeneral/DltTupleEndianness = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!ELSE!]
#define DLT_TUPLE_PLATFORM_ENDIANNESS_DISABLE STD_OFF
[!ENDIF!]

[!IF "DltGeneral/DltEnableTrafficShaper = 'true'"!]
/* !LINKSTO Dlt.TrafficShaping.ComModule,1 */
/* Convert kbits/s into bytes/s */
#define DLT_BANDWIDTH_FOR_COM_MODULE          [!"num:i((DltMultipleConfigurationContainer/DltBandwidth/DltBandwidthForComModule * 1000) div 8)"!]U

/* Traffic shaping integration window in milliseconds */
#define DLT_TIME_PERIOD_TRAFFIC_SHAPING       [!"num:i(DltMultipleConfigurationContainer/DltBandwidth/DltTimePeriodTrafficShaping * 1000)"!]U

/* This is the maximum number of bytes per milliseconds */
#define DLT_BYTES_PER_MILLISECOND             (Dlt_BandwidthForComModule / 1000U)

/* How many Dlt_MainFunction() calls correspond to the integration window */
/* !LINKSTO Dlt.swdd.Dlt_MainFunction.IntegrationWindow.Calculate,1, SWS_Dlt_00056,1 */
#define DLT_INTEGRATION_WINDOW                (DLT_TIME_PERIOD_TRAFFIC_SHAPING / DLT_MAINFUNCTION_PERIOD)

/* The maximum number of bytes allowed in the integration window */
/* !LINKSTO Dlt.swdd.MaxNoOfBytesTransmittedWithinWindow,1 */
#define DLT_MAXIMUM_NUMBER_OF_BYTES           ((Dlt_BandwidthForComModule * DLT_TIME_PERIOD_TRAFFIC_SHAPING) / 1000U)

/* How many bytes are transmitted in one Dlt_MainFunction() call at maximum bandwidth */
/* !LINKSTO Dlt.swdd.BytesTransmittedPerMainFunction,1 */
#define DLT_BYTES_PER_MAINFUNCTION            ((Dlt_BandwidthForComModule * DLT_MAINFUNCTION_PERIOD) / 1000)

[!ELSE!]
#define DLT_BANDWIDTH_FOR_COM_MODULE           0U
[!ENDIF!]


[!IF "node:exists(DltGeneral/DltGeneralStartUpDelayTimer)"!]
/* Delay for starting the transmission of messages after the Dlt module has been initialized. In milliseconds */
#define DLT_STARTUP_DELAY_TIMER [!"num:i(DltGeneral/DltGeneralStartUpDelayTimer * 1000)"!]U
[!ELSE!]
#define DLT_STARTUP_DELAY_TIMER 0U
[!ENDIF!]

/* Switch indicating whether support for basic software distribuiton (multi-core support) is enabled or disabled */
#define DLT_ENABLE_BSW_DISTRIBUTION           [!IF "(node:exists(DltMultipleConfigurationContainer/DltBswDistribution/DltEnableBswDistribution)) and DltMultipleConfigurationContainer/DltBswDistribution/DltEnableBswDistribution = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* Value indicating the number of configured satellite cores (slave cores).
   If multicore support is disabled the number of slave cores shall be 0U.
   If basic software distribution is enabled, the number of cores shall be configurable. */
#define DLT_NUMBER_OF_SATELLITES              [!IF "(node:exists(DltMultipleConfigurationContainer/DltBswDistribution/DltEnableBswDistribution)) and DltMultipleConfigurationContainer/DltBswDistribution/DltEnableBswDistribution = 'true'"!][!"DltMultipleConfigurationContainer/DltBswDistribution/DltNumberOfSatellites"!]U[!ELSE!]0U[!ENDIF!]

/* Value indicating the CoreId of the selected master core. */
#define DLT_MASTER_CORE                       [!IF "(node:exists(DltMultipleConfigurationContainer/DltBswDistribution/DltEnableBswDistribution)) and DltMultipleConfigurationContainer/DltBswDistribution/DltEnableBswDistribution = 'true'"!][!"node:ref(DltMultipleConfigurationContainer/DltBswDistribution/DltMasterCore)/OsCoreId"!]U[!ELSE!]0U[!ENDIF!]
[!IF "DltGeneral/DltImplementNVRamStorage = 'true'"!]
  [!VAR "GeneralBlockIdRef" = "as:ref(./DltMultipleConfigurationContainer/DltNvRamBlockRef)/NvMNvramBlockIdentifier"!]
  [!VAR "GeneralBlockIdLength" = "as:ref(./DltMultipleConfigurationContainer/DltNvRamBlockRef)/NvMNvBlockLength "!]
#if (defined DLT_STORE_NATIVE_NVRAM_BLOCK_ID)
#error DLT_STORE_NATIVE_NVRAM_BLOCK_ID is already defined.
#endif
/** \brief The NvM block ID used to store runtime variables for Dlt module */
#define DLT_STORE_NATIVE_NVRAM_BLOCK_ID   [!"$GeneralBlockIdRef"!]U

#if (defined DLT_STORE_NATIVE_NVRAM_BLOCK_LENGTH)
#error DLT_STORE_NATIVE_NVRAM_BLOCK_LENGTH already defined
#endif
/** \brief The length of the NvM block used to store the runtime variables for Dlt module */
#define DLT_STORE_NATIVE_NVRAM_BLOCK_LENGTH   [!"$GeneralBlockIdLength"!]U
[!ENDIF!]
[!IF "DltGeneral/DltImplementNVRamStorage = 'true'"!]
  [!VAR "DataSetBlockIdRef" = "as:ref(./DltMultipleConfigurationContainer/DltNvRamDataSetBlockRef)/NvMNvramBlockIdentifier"!]
  [!VAR "DataSetBlockIdLength" = "as:ref(./DltMultipleConfigurationContainer/DltNvRamDataSetBlockRef)/NvMNvBlockLength "!]
#if (defined DLT_STORE_DATASET_NVRAM_BLOCK_ID)
#error DLT_STORE_DATASET_NVRAM_BLOCK_ID is already defined.
#endif
/** \brief The NvM block ID used to store information related to a registered pair of
 **        ApplicationId and ContextId for Dlt module
 **/
#define DLT_STORE_DATASET_NVRAM_BLOCK_ID   [!"$DataSetBlockIdRef"!]U

#if (defined DLT_STORE_DATASET_NVRAM_BLOCK_LENGTH)
#error DLT_STORE_DATASET_NVRAM_BLOCK_LENGTH is already defined.
#endif
/** \brief The length of the NvM block used to store information related to
 **        a pair of ApplicationID and ContextID for Dlt module.
 **/
#define DLT_STORE_DATASET_NVRAM_BLOCK_LENGTH   [!"$DataSetBlockIdLength"!]U
[!ENDIF!]
[!IF "DltGeneral/DltImplementNVRamStorage = 'true'"!]
  [!IF "node:value(DltMultipleConfigurationContainer/DltServiceAPI/DltServiceAPI) = 'AUTOSAR_431'"!]
    [!VAR "DataSetBlockNumber" = "num:i(count(DltSwc/*/DltSwcContext/*))"!]
  [!ELSE!]
    [!VAR "DataSetBlockNumber" = "num:integer(number(DltMemory/DltMaxCountContextIds))"!]
  [!ENDIF!]
#if (defined DLT_STORE_DATASET_NVRAM_BLOCK_NUMBER)
#error DLT_STORE_DATASET_NVRAM_BLOCK_NUMBER is already defined.
#endif
/** \brief The NvM total number of data set block used to store information related to
 **        a pair of ApplicationID and ContextID for Dlt module.
 ** Note: the maximum number of dataset has to fulfill two requirements:
 **       1. It cannot be smaller than the total number of ApplicationID and ContextId
 **          which can be registered. Therefore in the current implementation the number
 **          of dataset is equal to the maximum number of pairs which can be registered.
 **       2. The range of data set NvM block index:
 **          0x00...(2^NvMDatasetSelectionBits-1)
 ** In the current implementation there are no checks performed for
 **/
#define DLT_STORE_DATASET_NVRAM_BLOCK_NUMBER   ([!"$DataSetBlockNumber"!]U + DLT_VFB_CONTEXT_COUNT)
[!ENDIF!]
[!IF "DltMultipleConfigurationContainer/DltServiceAPI/DltServiceAPI = 'AUTOSAR_431'"!]
/** \brief Current no. of registered SW-Cs */
#define DLT_SWC_REGISTERED_NO                  [!IF "(num:i($VfbTraceFunctionCount) > 0)"!][!"num:i(count(DltSwc/*) + 1)"!]U[!ELSE!][!"num:i(count(DltSwc/*))"!]U[!ENDIF!]
  [!VAR "TotalNoOfRegCtxt" = "0"!]
  [!LOOP "DltSwc/*"!]
/** \brief Current no of registered context on [!"node:name(.)"!] */
#define DLT_NO_OF_REG_CTX_[!"node:name(.)"!]  [!"num:i(count(DltSwcContext/*))"!]U
    [!VAR "TotalNoOfRegCtxt" = "$TotalNoOfRegCtxt + num:i(count(DltSwcContext/*))"!]
  [!ENDLOOP!]

  [!IF "node:refvalid(./DltConfigSet/*[1]/DltLogOutput/*[1]/DltDefaultLogChannelRef) and node:exists(as:ref(./DltConfigSet/*[1]/DltLogOutput/*[1]/DltDefaultLogChannelRef)/DltLogChannelId)"!]
    [!VAR "defaultlogChn" = "num:i(node:ref(./DltConfigSet/*[1]/DltLogOutput/*[1]/DltDefaultLogChannelRef)/@index)"!]
 /** \brief Current default log channel configured for the transmission in case of a send log / send trace message */
#define DEFAULT_LOG_CHN_IDX [!"$defaultlogChn"!]U
  [!ENDIF!]
 /** \brief Current no of configured log channels */
#define DLT_TXPDU_NO [!"num:i(count(DltConfigSet/*/DltLogOutput/*/DltLogChannel/*))"!]U
[!ELSE!]
 /** \brief Current no of configured log channels */
#define DLT_TXPDU_NO  1U
[!ENDIF!]
 /** \brief Macro for enabling/disabling the GetSoftwareVersion control message */
[!IF "DltMultipleConfigurationContainer/DltProtocol/DltGetSoftwareVersion = 'true'"!]
#define DLT_GETSOFTWAREVERSION_CONTROL_MESSAGE     STD_ON
 /** \brief The length of the ECU software version string in bytes */
#define DLT_GETSOFTWAREVERSION_LENGTH   [!"DltMultipleConfigurationContainer/DltProtocol/DltGetSoftwareVersionLength"!]U
[!ELSE!]
#define DLT_GETSOFTWAREVERSION_CONTROL_MESSAGE     STD_OFF
[!ENDIF!]
 /** \brief Macro to enable/disable the retrieval of the EcuId at run-time */
[!IF "DltMultipleConfigurationContainer/DltProtocol/DltEcuIdChoice = 'Callout'"!][!//
#define DLT_GETECUID_AT_RUNTIME               STD_ON
[!ELSE!]
#define DLT_GETECUID_AT_RUNTIME               STD_OFF
[!ENDIF!]
/*==================[type definitions]======================================*/

/*------------------[Dlt_ConfigType]----------------------------------------*/
/* !LINKSTO SWS_Dlt_00437,1 */
/** \brief This type of the external data structure shall contain the post
 **  build initialization data for the Dlt.
 **
 ** \note Type is unused, as only pre-compile time support is implemented. */
typedef uint8 Dlt_ConfigType;

/** \brief Type that represents the length of a Dlt message */
#ifndef RTE_TYPE_Dlt_MsgLengthType
#define RTE_TYPE_Dlt_MsgLengthType
/* !LINKSTO SWS_Dlt_00320, 1 */
typedef PduLengthType Dlt_MsgLengthType;
#endif

[!IF "DltMultipleConfigurationContainer/DltServiceAPI/DltServiceAPI = 'AUTOSAR_431'"!]
/* !LINKSTO Dlt.ASR431.SWS_Dlt_00232,1 */
/** \brief This type describes the LogChannel name. */
#ifndef RTE_TYPE_Dlt_LogChannelNameType
#define RTE_TYPE_Dlt_LogChannelNameType
typedef uint8 Dlt_LogChannelNameType[4U];
#endif

/* !LINKSTO Dlt.LogChannelNameArrayType, 1 */
/** \brief This type describes the list of log channel names */
#ifndef RTE_TYPE_Dlt_LogChannelNameArrayType
#define RTE_TYPE_Dlt_LogChannelNameArrayType
typedef Dlt_LogChannelNameType Dlt_LogChannelNameArrayType[DLT_TXPDU_NO];
#endif

/** \brief This type for ApplicationId/ContextId tuple. */
typedef struct{
  uint32 DltSwCApplicationId;
  uint32 DltSwCContextId;
}Dlt_AppCtxTupleType;

#ifndef RTE_TYPE_AppCtxDesc
#define RTE_TYPE_AppCtxDesc
 /** \brief Typedef for application/context description accepted size */
typedef uint8 AppCtxDesc[0xFFU];
#endif

 /** \brief This type for Application description length.
  ** AppDesc: Application description
  ** AppDescLen: Application description length */
#ifndef RTE_TYPE_Dlt_AppDescType
#define RTE_TYPE_Dlt_AppDescType
typedef struct{
  AppCtxDesc AppDesc;
  uint8 AppDescLen;
} Dlt_AppDescType;
#endif

/** \brief This type describes the LogChannel information.
 ** DltLogChannelBufferOverflowTimer: Cycle time in seconds for resetting
 **                                   the buffer overflow flag in case a
 **                                   buffer overflow occurred.
 ** DltLogChannelTransmitCycle: Specifies the cycle time in seconds of the
 **                             transmit functionality of this log channel.
 ** DltLogChannelTrafficShapingBandwidth: Set the maximum possible bandwidth in bit/s.
 ** DltLogChannelBufferSize : Buffer size in bytes for the LogChannel specific message buffer.
 ** DltMessageTxBuffer: Address for the transmission buffer
 ** DltLogChannelMaxMessageLength: The maximum length of a Dlt log or trace message
 ** DltITxPduHandleId: Pdu Tx Handle Id for a specific log channel
 ** DltLogChannelId: This is the 4 ASCII character long name of the log channel
 ** DltLogChannelMaxNumOfRetries: The maximum length of a Dlt log or trace message. */
typedef struct{
  uint32  DltLogChannelBufferOverflowTimer;
  uint32  DltLogChannelTransmitCycle;
  uint32  DltLogChannelTrafficShapingBandwidth;
  uint32  DltLogChannelBufferSize;
  uint8 * DltMessageTxBuffer;
  Dlt_MsgLengthType DltLogChannelMaxMessageLength;
  PduIdType DltITxPduHandleId;
  Dlt_LogChannelNameType  DltLogChannelId;
  uint8   DltLogChannelMaxNumOfRetries;
}Dlt_LogChannelType;

/** \brief This type describes the Threshold information for log channels.
 ** DltLogChannelThreshold: LogLevel Threshold for a specific Channel
 ** DltLogTraceStatusFlag: Parameter to turn on/off trace for this channel completely */
typedef struct{
  uint8  DltLogChannelThreshold;
  boolean DltLogTraceStatusFlag;
}Dlt_LogChannelThresholdInfoType;

/** \brief This type describes SW context info for SW-C.
 ** Dlt_AppCtxTuple: Information regarding the ApplcationId/ContextId tuple
 ** DltLoglevel: Specific log level for the tuple
 ** DltLogChannelIdx: boolean status for each channel that
 **                   indicates whether the channel is assigned
 **                   or not to the context
 ** DltTraceStatus: Specific trace status for the tuple */
typedef struct{
  Dlt_AppCtxTupleType Dlt_AppCtxTuple;
  /* Log level for log messages */
  uint8 DltLoglevel;
  boolean  DltLogChannelIdx[DLT_TXPDU_NO];
  /* Trace status for trace messages */
  boolean DltTraceStatus;
}Dlt_SwCContextType;

/** \brief This type describes Log info for SW-C.
** DltSwCSessionId:          An ECU wide unique ID to identify the port a SWC (instance) uses
** MaxSwCLogMessageLength:   Defines the maximum allowed length (Dlt_MsgLengthType) for LogMessages.
** MaxSwCTraceMessageLength: Defines the maximum allowed length (Dlt_MsgLengthType) for TraceMessages.
** SwCCtxRef:                address of the SwContext info variable
** DltSwCSupportLogLevelChangeNotification: Flag indicating, whether Dlt has to
**                                          provide a R-Port for the notification
**                                          of the SWC about LogLevel changes */
typedef struct{
  uint32 DltSwCSessionId;
  Dlt_MsgLengthType MaxSwCLogMessageLength;
  Dlt_MsgLengthType MaxSwCTraceMessageLength;
  boolean DltSwCSupportLogLevelChangeNotification;
}Dlt_SwCLogType;

/** \brief This type describes how many contexts ids are registered per application id.
** DltSwCApplicationId : Application id
** CtxtNo : No of contexts registered per application*/
typedef struct{
  uint32  DltSwCApplicationId;
  uint32  CtxtNo;
}Dlt_CtxtPerApplType;
[!ENDIF!]
/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

#define DLT_START_SEC_CONST_8
#include <Dlt_MemMap.h>

extern CONST(Dlt_ConfigType, DLT_CONST) Dlt_ConfigSet_0;

#define DLT_STOP_SEC_CONST_8
#include <Dlt_MemMap.h>

[!IF "DltMultipleConfigurationContainer/DltServiceAPI/DltServiceAPI = 'AUTOSAR_431'"!][!//
#define DLT_START_SEC_VAR_INIT_UNSPECIFIED
#include <Dlt_MemMap.h>

[!IF "(node:exists(DltSwc/*)) and (num:i(count(DltSwc/*)) > 0)"!][!//
extern P2CONST(Dlt_SwCContextType, AUTOMATIC, DLT_APPL_DATA) DltSwCContextInfoPtr[DLT_SWC_REGISTERED_NO];
[!ENDIF!][!//
extern VAR(Dlt_CtxtPerApplType, DLT_VAR) Dlt_MaxCtxtPerApplication[DLT_MAX_COUNT_APP_IDS];

#define DLT_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <Dlt_MemMap.h>

#define DLT_START_SEC_CONST_UNSPECIFIED
#include <Dlt_MemMap.h>

extern CONST(Dlt_LogChannelType, DLT_CONST) Dlt_CfgLogChannel[DLT_TXPDU_NO];
extern CONST(Dlt_LogChannelThresholdInfoType, DLT_CONST) Dlt_CfgLogChannelThresholdInfo[DLT_TXPDU_NO];
[!IF "num:i(count(DltSwc/*)) > 0"!][!//
extern CONST(Dlt_SwCLogType, DLT_CONST) Dlt_SwCLogInfo[DLT_SWC_REGISTERED_NO];
extern CONST(uint8, DLT_CONST) Dlt_MaxCtxAppIdPerSwC[DLT_SWC_REGISTERED_NO];
[!ENDIF!][!//

#define DLT_STOP_SEC_CONST_UNSPECIFIED
#include <Dlt_MemMap.h>

[!ENDIF!][!//
/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef DLT_CFG_H */
/*==================[end of file]===========================================*/
