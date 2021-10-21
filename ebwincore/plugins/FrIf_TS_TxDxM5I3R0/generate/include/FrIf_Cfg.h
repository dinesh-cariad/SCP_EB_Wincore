/**
 * \file
 *
 * \brief AUTOSAR FrIf
 *
 * This file contains the implementation of the AUTOSAR
 * module FrIf.
 *
 * \version 5.3.25
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
[!CODE!]
#ifndef FRIF_CFG_H
#define FRIF_CFG_H

/*
 * Include Section
 */
[!INCLUDE "CommonMacros.m"!][!//
#include <ComStack_Types.h> /* definitions of STD_ON and STD_OFF */
#include <TSAutosar.h>      /* definitions of TS_PROD_ERR_* */
/*
 * Global Macros
 */

[!SELECT "FrIfGeneral"!]
/* standard SWS pre-compile time configuration parameters */
[!CALL "GuardedDefine", "Comment"="'Determines wether development error detection is enabled.'",
  "Name"="'FRIF_DEV_ERROR_DETECT'"!] [!IF "FrIfDevErrorDetect = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Determine the existence of the FrIf_GetVersionInfo() API service.'",
  "Name"="'FRIF_VERSION_INFO_API'"!] [!IF "FrIfVersionInfoApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'Configuration parameter to enable/disable FrIf support to request the FlexRay hardware NMVector.'",
  "Name"="'FRIF_GETNMVECTOR_API_ENABLE'"!] [!IF "FrIfGetNmVectorSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to enable/disable FrIf support to enable/disable of polling the FlexRay Driver to getting error information about the FlexRay communications bus.'",
  "Name"="'FRIF_GETCHANNELSTATUS_API_ENABLE'"!] [!IF "FrIfGetChannelStatusSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to enable/disable FrIf support to enable/disable of polling the FlexRay Driver to getting CC clock correction values.'",
  "Name"="'FRIF_GETCLOCKCORRECTION_API_ENABLE'"!] [!IF "FrIfGetClockCorrectionSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to enable/disable FrIf support to enable/disable of polling the FlexRay Driver to getting a list of actual received sync frames.'",
  "Name"="'FRIF_GETSYNCFRAMELIST_API_ENABLE'"!] [!IF "FrIfGetSyncFrameListSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to enable/disable FrIf support to enable/disable the reconfiguration of a given LPdu according to the parameters at runtime.'",
  "Name"="'FRIF_RECONFIGLPDU_API_ENABLE'"!] [!IF "FrIfReconfigLPduSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to enable/disable FrIf support to disable the hardware resource of a LPdu for transmission/reception.'",
  "Name"="'FRIF_DISABLELPDU_API_ENABLE'"!] [!IF "FrIfDisableLPduSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to enable/disable FrIf support to get the wakeup received information from the FlexRay controller.'",
  "Name"="'FRIF_GETWAKEUPRXSTATUS_API_ENABLE'"!] [!IF "FrIfGetWakeupRxStatusSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to enable/disable FrIf support to enable/disable of switching from key-slot / single-slot mode to all slot mode.'",
  "Name"="'FRIF_ALLSLOTS_API_ENABLE'"!] [!IF "FrIfAllSlotsSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'Configuration parameter to enable/disable FrIf support to request the cancellation of the I-PDU transmission to FrDrv.'",
  "Name"="'FRIF_CANCELTRANSMIT_API_ENABLE'"!] [!IF "FrIfCancelTransmitSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to enable/disable FrIf support to enable branches of an active star.'",
  "Name"="'FRIF_ENABLETRANSCEIVERBRANCH_API_ENABLE'"!] [!IF "FrIfEnableTransceiverBranchSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to enable/disable FrIf support to disable branches of an active star.'",
  "Name"="'FRIF_DISABLETRANSCEIVERBRANCH_API_ENABLE'"!] [!IF "FrIfDisableTransceiverBranchSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to enable/disable FrIf support to enable/disable of polling the FlexRay Driver for the actual number of received startup frames on the bus.'",
  "Name"="'FRIF_GETNUMOFSTARTUPFRAMES_API_ENABLE'"!] [!IF "FrIfGetNumOfStartupFramesSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to enable/disable FrIf support to get the FlexRay Transceiver errors by calling the FlexRay Transceiver module.'",
  "Name"="'FRIF_GETTRANSCEIVERERROR_API_ENABLE'"!] [!IF "FrIfGetTransceiverErrorSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to enable/disable the optional FrIf_ReadCCConfig API.'",
  "Name"="'FRIF_READCCCONFIG_API_ENABLE'"!] [!IF "FrIfReadCCConfigApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Defines the maximum number of loops for for all communication operations with communication action RECEIVE_AND_INDICATE'",
  "Name"="'FRIF_GLOBALRXMAXLOOP'"!] [!"FrIfGlobalRxMaxLoop"!]U
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to enable/disable FrIf support to enable/disable reporting received/transmitted frames to the Bus Mirroring module.'",
  "Name"="'FRIF_BUSMIRRORINGSUPPORT_ENABLE'"!] [!IF "FrIfBusMirroringSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'Configuration parameter to enable/disable the support of setting unused bits to a defined value.'",
  "Name"="'FRIF_TX_FRAME_DEFAULT_VALUE_ENABLE'"!] [!IF "node:exists(FrIfUnusedBitValue)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to set unused bits to a defined value.'",
  "Name"="'FRIF_TX_FRAME_DEFAULT_VALUE'"!] [!IF "node:exists(FrIfUnusedBitValue)"!][!IF "FrIfUnusedBitValue = 0"!]0x00U[!ELSE!]0xFFU[!ENDIF!][!ELSE!]0x0U[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'Switch for DEM to DET reporting for production error FRIF_PROD_ERR_HANDLING_JLE_SYNC'",
  "Name"="'FRIF_PROD_ERR_HANDLING_JLE_SYNC'"!]   [!//
[!IF "ReportToDem/FrIfJoblistSyncReportToDem = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "ReportToDem/FrIfJoblistSyncReportToDem = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!//

[!IF "ReportToDem/FrIfJoblistSyncReportToDem = 'DET'"!][!//
[!CALL "GuardedDefine", "Comment"="'Det error ID, if DEM to DET reporting is enabled for FRIF_PROD_ERR_HANDLING_JLE_SYNC'",
  "Name"="'FRIF_E_DEMTODET_JLE_SYNC'"!]          [!"ReportToDem/FrIfJoblistSyncReportToDemDetErrorId"!]U
[!ELSEIF "ReportToDem/FrIfJoblistSyncReportToDem = 'DEM'"!][!//
[!CALL "GuardedDefine", "Comment"="''",
  "Name"="'FRIF_JLE_SYNC_PASSED'"!] [!IF "ReportToDem/FrIfJoblistSyncDebounceMethod = 'DEM'"!]DEM_EVENT_STATUS_PREPASSED[!ELSE!]DEM_EVENT_STATUS_PASSED[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="''",
  "Name"="'FRIF_JLE_SYNC_FAILED'"!] [!IF "ReportToDem/FrIfJoblistSyncDebounceMethod = 'DEM'"!]DEM_EVENT_STATUS_PREFAILED[!ELSE!]DEM_EVENT_STATUS_FAILED[!ENDIF!]
[!ENDIF!][!//

[!CALL "GuardedDefine", "Comment"="'Switch for DEM to DET reporting for production error FRIF_PROD_ERR_HANDLING_NIT_CH_A'",
  "Name"="'FRIF_PROD_ERR_HANDLING_NIT_CH_A'"!]   [!//
[!IF "ReportToDem/FrIfNITStatusAReportToDem = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "ReportToDem/FrIfNITStatusAReportToDem = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!//

[!IF "ReportToDem/FrIfNITStatusAReportToDem = 'DET'"!][!//
[!CALL "GuardedDefine", "Comment"="'Det error ID, if DEM to DET reporting is enabled for FRIF_PROD_ERR_HANDLING_NIT_CH_A'",
  "Name"="'FRIF_E_DEMTODET_NIT_CH_A'"!]          [!"ReportToDem/FrIfNITStatusAReportToDemDetErrorId"!]U
[!ELSEIF "ReportToDem/FrIfNITStatusAReportToDem = 'DEM'"!][!//
[!CALL "GuardedDefine", "Comment"="''",
  "Name"="'FRIF_NIT_CH_A_PASSED'"!] [!IF "ReportToDem/FrIfNITStatusADebounceMethod = 'DEM'"!]DEM_EVENT_STATUS_PREPASSED[!ELSE!]DEM_EVENT_STATUS_PASSED[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="''",
  "Name"="'FRIF_NIT_CH_A_FAILED'"!] [!IF "ReportToDem/FrIfNITStatusADebounceMethod = 'DEM'"!]DEM_EVENT_STATUS_PREFAILED[!ELSE!]DEM_EVENT_STATUS_FAILED[!ENDIF!]
[!ENDIF!][!//

[!CALL "GuardedDefine", "Comment"="'Switch for DEM to DET reporting for production error FRIF_PROD_ERR_HANDLING_NIT_CH_B'",
  "Name"="'FRIF_PROD_ERR_HANDLING_NIT_CH_B'"!]   [!//
[!IF "ReportToDem/FrIfNITStatusBReportToDem = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "ReportToDem/FrIfNITStatusBReportToDem = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!//

[!IF "ReportToDem/FrIfNITStatusBReportToDem = 'DET'"!][!//
[!CALL "GuardedDefine", "Comment"="'Det error ID, if DEM to DET reporting is enabled for FRIF_PROD_ERR_HANDLING_NIT_CH_B'",
  "Name"="'FRIF_E_DEMTODET_NIT_CH_B'"!]          [!"ReportToDem/FrIfNITStatusBReportToDemDetErrorId"!]U
[!ELSEIF "ReportToDem/FrIfNITStatusBReportToDem = 'DEM'"!][!//
[!CALL "GuardedDefine", "Comment"="''",
  "Name"="'FRIF_NIT_CH_B_PASSED'"!] [!IF "ReportToDem/FrIfNITStatusBDebounceMethod = 'DEM'"!]DEM_EVENT_STATUS_PREPASSED[!ELSE!]DEM_EVENT_STATUS_PASSED[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="''",
  "Name"="'FRIF_NIT_CH_B_FAILED'"!] [!IF "ReportToDem/FrIfNITStatusBDebounceMethod = 'DEM'"!]DEM_EVENT_STATUS_PREFAILED[!ELSE!]DEM_EVENT_STATUS_FAILED[!ENDIF!]
[!ENDIF!][!//

[!CALL "GuardedDefine", "Comment"="'Switch for DEM to DET reporting for production error FRIF_PROD_ERR_HANDLING_SW_CH_A'",
  "Name"="'FRIF_PROD_ERR_HANDLING_SW_CH_A'"!]   [!//
[!IF "ReportToDem/FrIfSymbolWindowStatusAReportToDem = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "ReportToDem/FrIfSymbolWindowStatusAReportToDem = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!//

[!IF "ReportToDem/FrIfSymbolWindowStatusAReportToDem = 'DET'"!][!//
[!CALL "GuardedDefine", "Comment"="'Det error ID, if DEM to DET reporting is enabled for FRIF_PROD_ERR_HANDLING_SW_CH_A'",
  "Name"="'FRIF_E_DEMTODET_SW_CH_A'"!]          [!"ReportToDem/FrIfSymbolWindowStatusAReportToDemDetErrorId"!]U
[!ELSEIF "ReportToDem/FrIfSymbolWindowStatusAReportToDem = 'DEM'"!][!//
[!CALL "GuardedDefine", "Comment"="''",
  "Name"="'FRIF_SW_CH_A_PASSED'"!] [!IF "ReportToDem/FrIfSymbolWindowStatusADebounceMethod = 'DEM'"!]DEM_EVENT_STATUS_PREPASSED[!ELSE!]DEM_EVENT_STATUS_PASSED[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="''",
  "Name"="'FRIF_SW_CH_A_FAILED'"!] [!IF "ReportToDem/FrIfSymbolWindowStatusADebounceMethod = 'DEM'"!]DEM_EVENT_STATUS_PREFAILED[!ELSE!]DEM_EVENT_STATUS_FAILED[!ENDIF!]
[!ENDIF!][!//

[!CALL "GuardedDefine", "Comment"="'Switch for DEM to DET reporting for production error FRIF_PROD_ERR_HANDLING_SW_CH_B'",
  "Name"="'FRIF_PROD_ERR_HANDLING_SW_CH_B'"!]   [!//
[!IF "ReportToDem/FrIfSymbolWindowStatusBReportToDem = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "ReportToDem/FrIfSymbolWindowStatusBReportToDem = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!//

[!IF "ReportToDem/FrIfSymbolWindowStatusBReportToDem = 'DET'"!][!//
[!CALL "GuardedDefine", "Comment"="'Det error ID, if DEM to DET reporting is enabled for FRIF_PROD_ERR_HANDLING_SW_CH_B'",
  "Name"="'FRIF_E_DEMTODET_SW_CH_B'"!] [!"ReportToDem/FrIfSymbolWindowStatusBReportToDemDetErrorId"!]U
[!ELSEIF "ReportToDem/FrIfSymbolWindowStatusBReportToDem = 'DEM'"!][!//
[!CALL "GuardedDefine", "Comment"="''",
  "Name"="'FRIF_SW_CH_B_PASSED'"!] [!IF "ReportToDem/FrIfSymbolWindowStatusBDebounceMethod = 'DEM'"!]DEM_EVENT_STATUS_PREPASSED[!ELSE!]DEM_EVENT_STATUS_PASSED[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="''",
  "Name"="'FRIF_SW_CH_B_FAILED'"!] [!IF "ReportToDem/FrIfSymbolWindowStatusBDebounceMethod = 'DEM'"!]DEM_EVENT_STATUS_PREFAILED[!ELSE!]DEM_EVENT_STATUS_FAILED[!ENDIF!]
[!ENDIF!][!//

[!CALL "GuardedDefine", "Comment"="'Switch for DEM to DET reporting for production error FRIF_PROD_ERR_HANDLING_ACS_CH_A'",
  "Name"="'FRIF_PROD_ERR_HANDLING_ACS_CH_A'"!]   [!//
[!IF "ReportToDem/FrIfAggregatedStatusAReportToDem = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "ReportToDem/FrIfAggregatedStatusAReportToDem = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!//

[!IF "ReportToDem/FrIfAggregatedStatusAReportToDem = 'DET'"!][!//
[!CALL "GuardedDefine", "Comment"="'Det error ID, if DEM to DET reporting is enabled for FRIF_PROD_ERR_HANDLING_ACS_CH_A'",
  "Name"="'FRIF_E_DEMTODET_ACS_CH_A'"!] [!"ReportToDem/FrIfAggregatedStatusAReportToDemDetErrorId"!]U
[!ELSEIF "ReportToDem/FrIfAggregatedStatusAReportToDem = 'DEM'"!][!//
[!CALL "GuardedDefine", "Comment"="''",
  "Name"="'FRIF_ACS_CH_A_PASSED'"!] [!IF "ReportToDem/FrIfAggregatedStatusADebounceMethod = 'DEM'"!]DEM_EVENT_STATUS_PREPASSED[!ELSE!]DEM_EVENT_STATUS_PASSED[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="''",
  "Name"="'FRIF_ACS_CH_A_FAILED'"!] [!IF "ReportToDem/FrIfAggregatedStatusADebounceMethod = 'DEM'"!]DEM_EVENT_STATUS_PREFAILED[!ELSE!]DEM_EVENT_STATUS_FAILED[!ENDIF!]
[!ENDIF!][!//

[!CALL "GuardedDefine", "Comment"="'Switch for DEM to DET reporting for production error FRIF_PROD_ERR_HANDLING_ACS_CH_B'",
  "Name"="'FRIF_PROD_ERR_HANDLING_ACS_CH_B'"!]   [!//
[!IF "ReportToDem/FrIfAggregatedStatusBReportToDem = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "ReportToDem/FrIfAggregatedStatusBReportToDem = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!//

[!IF "ReportToDem/FrIfAggregatedStatusBReportToDem = 'DET'"!][!//
[!CALL "GuardedDefine", "Comment"="'Det error ID, if DEM to DET reporting is enabled for FRIF_PROD_ERR_HANDLING_ACS_CH_B'",
  "Name"="'FRIF_E_DEMTODET_ACS_CH_B'"!]          [!"ReportToDem/FrIfAggregatedStatusBReportToDemDetErrorId"!]U
[!ELSEIF "ReportToDem/FrIfAggregatedStatusBReportToDem = 'DEM'"!][!//
[!CALL "GuardedDefine", "Comment"="''",
  "Name"="'FRIF_ACS_CH_B_PASSED'"!] [!IF "ReportToDem/FrIfAggregatedStatusBDebounceMethod = 'DEM'"!]DEM_EVENT_STATUS_PREPASSED[!ELSE!]DEM_EVENT_STATUS_PASSED[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="''",
  "Name"="'FRIF_ACS_CH_B_FAILED'"!] [!IF "ReportToDem/FrIfAggregatedStatusBDebounceMethod = 'DEM'"!]DEM_EVENT_STATUS_PREFAILED[!ELSE!]DEM_EVENT_STATUS_FAILED[!ENDIF!]
[!ENDIF!][!//
[!ENDSELECT!]

[!CALL "GuardedDefine", "Comment"="'Indicates if there is at least one UL using ASR43 APIs'",
  "Name"="'FRIF_UL_ASR43_API_ENABLE'"!] [!IF "num:i(count(FrIfUserUpperLayerConfig/*[(./FrIfUpperLayerASR43TxConfirmSupport)='true']))>0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!SELECT "FrIfGeneral/VendorSpecific"!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to enable/disable the inclusion of the API functions FrIf_SetRelativeTimer(), FrIf_CancelRelativeTimer(),
        FrIf_EnableRelativeTimerIRQ(), FrIf_DisableRelativeTimerIRQ(), FrIf_GetRelativeTimerIRQStatus()
        and FrIf_AckRelativeTimerIRQStatus().'",
  "Name"="'FRIF_RELATIVETIMER_API_ENABLE'"!] [!IF "RelativeTimerApiEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to switch the API service FrIf_SetExtSync() on or off.'",
  "Name"="'FRIF_SETEXTSYNC_API_ENABLE'"!] [!IF "SetExtSyncApiEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to switch the API services FrIf_SendMTS(), FrIf_StopMTS() and FrIf_CheckMTS() on or off.'",
  "Name"="'FRIF_MTS_API_ENABLE'"!] [!IF "MtsApiEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to switch the API services FrIf_SendWUP() and FrIf_SetWakeupChannel() on or off.'",
  "Name"="'FRIF_WAKEUP_API_ENABLE'"!] [!IF "WakeupApiEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to switch the API services FrIf_GetAbsoluteTimerIRQStatus() and FrIf_GetRelativeTimerIRQStatus() on or off.'",
  "Name"="'FRIF_GETIRQSTATUS_API_ENABLE'"!] [!IF "GetIrqStatusApiEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to switch the API services FrIf_DisableAbsoluteTimerIRQ() and FrIf_DisableRelativeTimerIRQStatus() on or off.'",
  "Name"="'FRIF_DISABLEIRQ_API_ENABLE'"!] [!IF "DisableIrqApiEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to switch the API service FrIf_AllowColdstart() on or off.'",
  "Name"="'FRIF_ALLOWCOLDSTART_API_ENABLE'"!] [!IF "AllowColdstartApiEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to switch the API service FrIf_GetControllerErrorStatus() on or off.'",
  "Name"="'FRIF_GETCONTROLLERERRORSTATUS_API_ENABLE'"!] [!IF "GetCtrlErrorStatusApiEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to switch the API services FrIf_AckIRQ(), FrIf_EnableIRQ(), FrIf_DisableIRQ() and FrIf_GetIRQStatus() on or off.'",
  "Name"="'FRIF_EXT_IRQ_SERVICES_API_ENABLE'"!] [!IF "ExtIRQServicesApiEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to switch the API service FrIf_GetSyncState() on or off.'",
  "Name"="'FRIF_GETSYNCSTATE_API_ENABLE'"!] [!IF "GetSyncStateApiEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'Configuration parameter to switch the API service FrIf_GetTransceiverMode() on or off.'",
  "Name"="'FRIF_GETTRANSCEIVERMODE_API_ENABLE'"!] [!IF "GetTransceiverModeApiEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to switch the API service FrIf_GetTransceiverWUReason() on or off.'",
  "Name"="'FRIF_GETTRANSCEIVERWUREASON_API_ENABLE'"!] [!IF "GetTransceiverWUReasonApiEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to switch the API services FrIf_ClearTransceiverWakeup() on or off.'",
  "Name"="'FRIF_WAKEUPCONTROL_API_ENABLE'"!] [!IF "WakeupControlApiEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to switch the API service FrIf_CheckWakeupByTransceiver() on or off.'",
  "Name"="'FRIF_CHECKWAKEUPBYTRANSCEIVER_API_ENABLE'"!] [!IF "CheckWakeupByTransceiverApiEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'Configuration parameter to switch the Development Error Notification on or off.'",
  "Name"="'FRIF_REPORT_TO_DET_ENABLE'"!] [!IF "ReportToDetEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to enable/disable the optimization for a configuration consisting of a single FlexRay cluster.'",
  "Name"="'FRIF_SINGLE_CLST_OPT_ENABLE'"!] [!IF "SingleClstOptEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to enable/disable the optimization for a configuration consisting of a single FlexRay controller.'",
  "Name"="'FRIF_SINGLE_CTRL_OPT_ENABLE'"!] [!IF "SingleCtrlOptEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="''",
  "Name"="'FRIF_FR_ASR_40_SUPPORT'"!] 0x00U
[!CALL "GuardedDefine", "Comment"="''",
  "Name"="'FRIF_FR_ASR_44_SUPPORT'"!] 0x01U
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to state used Fr driver.'",
  "Name"="'FRIF_FR_DRIVER_AUTOSAR_VERSION'"!] [!IF "FrDriverAutosarVersion = 'ASR_44'"!]FRIF_FR_ASR_44_SUPPORT[!ELSE!]FRIF_FR_ASR_40_SUPPORT[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'Configuration parameter to enable/disableFr integration of FrIf'",
  "Name"="'FRIF_FR_INTEGRATION_ENABLE'"!] [!IF "FrIntegrationEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to enable/disable the optimizations for a configuration consisting of a single FlexRay driver.'",
  "Name"="'FRIF_SINGLE_FR_OPT_ENABLE'"!] [!IF "SingleFrOptEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!IF "SingleFrOptEnable = 'true'"!]
[!IF "not(node:refvalid(../../FrIfConfig/*[1]/FrIfCluster/*/FrIfController/*[1]/FrIfFrCtrlRef))"!]
[!ERROR!]Fr controller referenced by FrIfFrCtrlRef of FrIfController [!"node:path(../FrIfConfig/*[1]/FrIfCluster/*/FrIfController/*[1])"!] is not valid[!ENDERROR!]
[!ENDIF!]
[!SELECT "as:ref(../../FrIfConfig/*[1]/FrIfCluster/*/FrIfController/*[1]/FrIfFrCtrlRef)/../../../../CommonPublishedInformation"!]
[!VAR "FrApiPostfix"="''"!]
[!IF "node:exists(VendorApiInfix) and (VendorApiInfix != '')"!]
[!VAR "FrApiPostfix"!]_[!"VendorId"!]_[!"VendorApiInfix"!][!ENDVAR!]
[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="''",
  "Name"="'FRIF_SINGLE_FR_PREFIX'"!] Fr[!"$FrApiPostfix"!]
#include <Fr[!"$FrApiPostfix"!].h>
[!ENDSELECT!]
[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'Configuration parameter to enable/disable the optimization for a configuration consisting of a single FlexRay transceiver driver.'",
  "Name"="'FRIF_SINGLE_FRTRCV_OPT_ENABLE'"!] [!IF "SingleFrTrcvOptEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!IF "SingleFrTrcvOptEnable = 'true'"!]
[!VAR "FrTrcvApiPostfix"="''"!]
[!IF "node:exists(../../FrIfConfig/*[1]/FrIfCluster/*/FrIfController/*/FrIfTransceiver/*[1])"!]
[!IF "not(node:refvalid(../../FrIfConfig/*[1]/FrIfCluster/*/FrIfController/*/FrIfTransceiver/*[1]/FrIfFrTrcvChannelRef))"!]
[!ERROR!]FrTrcv transceiver referenced by FrIfFrTrcvChannelRef of FrIfTransceiver [!"node:path(../FrIfConfig/*[1]/FrIfCluster/*/FrIfController/*/FrIfTransceiver/*[1])"!] is not valid[!ENDERROR!]
[!ENDIF!]
[!SELECT "as:ref(../../FrIfConfig/*[1]/FrIfCluster/*/FrIfController/*/FrIfTransceiver/*[1]/FrIfFrTrcvChannelRef)/../../CommonPublishedInformation"!]
[!IF "node:exists(VendorApiInfix) and (VendorApiInfix != '')"!]
[!VAR "FrTrcvApiPostfix"!]_[!"VendorId"!]_[!"VendorApiInfix"!][!ENDVAR!]
[!ENDIF!]
[!ENDSELECT!]
[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="''",
  "Name"="'FRIF_SINGLE_FRTRCV_PREFIX'"!] FrTrcv[!"$FrTrcvApiPostfix"!]
#include <FrTrcv[!"$FrTrcvApiPostfix"!].h>
#include <FrTrcv[!"$FrTrcvApiPostfix"!]_Cbk.h>
[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'Configuration parameter to switch demultiplexing of absolute timer interrupt for joblist execution on or off.'",
  "Name"="'FRIF_JOBLIST_IRQMUX_ENABLE'"!] [!IF "JoblistIRQMuxEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to switch invocation delay check for joblist execution on or off.'",
  "Name"="'FRIF_JOBLIST_IRQDELAYCHECK_ENABLE'"!] [!IF "JoblistIRQDelayCheckEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to switch support for the communication operation PREPARE_LPDU on and off.'",
  "Name"="'FRIF_JOBLIST_PREPARE_LPDU_ENABLE'"!] [!IF "JoblistPrepareLPduEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'Configuration parameter to switch support for dynamic payload length on or off.'",
  "Name"="'FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE'"!] [!IF "DynamicPayloadLengthEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to switch support for immediate transmission on or off.'",
  "Name"="'FRIF_IMMEDIATE_TX_ENABLE'"!] [!IF "ImmediateTxEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to switch support for transmit request queueing on or off.'",
  "Name"="'FRIF_TRANSMIT_QUEUE_ENABLE'"!] [!IF "TransmitQueueEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Configuration parameter to switch support for decoupled reception on or off.
'",
  "Name"="'FRIF_DECOUPLED_RX_ENABLE'"!] [!IF "DecoupledRxEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'Configuration parameter to enable/disable support for demultiplexing received LPdus from the extended RxFIFO (EB FlexRay driver feature).'",
  "Name"="'FRIF_EXTENDED_RXFIFO_ENABLE'"!] [!IF "ExtendedRxFIFOEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'Configuration parameter to enable/disable support fort PbcfgM'",
  "Name"="'FRIF_PBCFGM_SUPPORT_ENABLED'"!] [!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('FrIf')) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'Configuration parameter to enable/disable a a relocatable configuration.'",
  "Name"="'FRIF_RELOCATABLE_CFG_ENABLE'"!] [!//
[!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('FrIf')) = 'true'"!][!//
[!IF "as:modconf('PbcfgM')/PbcfgMGeneral/PbcfgMRelocatableCfgEnable = 'true'"!][!//
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!][!//
[!ELSE!][!//
[!IF "FrIfRelocatablePbcfgEnable = 'true'"!][!//
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!][!//
[!ENDIF!][!//

[!CALL "GuardedDefine", "Comment"="'Link time verification value'",
  "Name"="'FRIF_PUBLIC_INFO_SIGNATURE'"!] [!"asc:getConfigSignature(node:difference(as:modconf('FrIf')[1]/CommonPublishedInformation//*[not(child::*) and (node:configclass() = 'PublishedInformation')], as:modconf('FrIf')[1]/CommonPublishedInformation/Release))"!]U

[!CALL "GuardedDefine", "Comment"="'Compile time verification value'",
  "Name"="'FRIF_CFG_SIGNATURE'"!] [!"asc:getConfigSignature(as:modconf('FrIf')[1]//*[not(child::*) and (node:configclass() = 'PreCompile') ])"!]U

[!CALL "GuardedDefine", "Comment"="'Link time verification value'",
  "Name"="'FRIF_LCFG_SIGNATURE'"!] [!"asc:getConfigSignature(as:modconf('FrIf')[1]//*[not(child::*) and (node:configclass() = 'Link') ])"!]U
[!ENDSELECT!]




/*------------------[Defensive programming]---------------------------------*/
[!SELECT "FrIfDefensiveProgramming"!][!//

#if (defined FRIF_DEFENSIVE_PROGRAMMING_ENABLED)
#error FRIF_DEFENSIVE_PROGRAMMING_ENABLED is already defined
#endif
/** \brief Defensive programming usage
 **
 ** En- or disables the usage of the defensive programming */
#define FRIF_DEFENSIVE_PROGRAMMING_ENABLED   [!//
[!IF "(../FrIfGeneral/FrIfDevErrorDetect  = 'true') and (FrIfDefProgEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRIF_PRECONDITION_ASSERT_ENABLED)
#error FRIF_PRECONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Precondition assertion usage
 **
 ** En- or disables the usage of precondition assertion checks */
#define FRIF_PRECONDITION_ASSERT_ENABLED     [!//
[!IF "(../FrIfGeneral/FrIfDevErrorDetect  = 'true') and (FrIfDefProgEnabled = 'true') and (FrIfPrecondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRIF_POSTCONDITION_ASSERT_ENABLED)
#error FRIF_POSTCONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Postcondition assertion usage
 **
 ** En- or disables the usage of postcondition assertion checks */
#define FRIF_POSTCONDITION_ASSERT_ENABLED    [!//
[!IF "(../FrIfGeneral/FrIfDevErrorDetect  = 'true') and (FrIfDefProgEnabled = 'true') and (FrIfPostcondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRIF_UNREACHABLE_CODE_ASSERT_ENABLED)
#error FRIF_UNREACHABLE_CODE_ASSERT_ENABLED is already defined
#endif
/** \brief Unreachable code assertion usage
 **
 ** En- or disables the usage of unreachable code assertion checks */
#define FRIF_UNREACHABLE_CODE_ASSERT_ENABLED [!//
[!IF "(../FrIfGeneral/FrIfDevErrorDetect  = 'true') and (FrIfDefProgEnabled = 'true') and (FrIfUnreachAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRIF_INVARIANT_ASSERT_ENABLED)
#error FRIF_INVARIANT_ASSERT_ENABLED is already defined
#endif
/** \brief Invariant assertion usage
 **
 ** En- or disables the usage of invariant assertion checks */
#define FRIF_INVARIANT_ASSERT_ENABLED        [!//
[!IF "(../FrIfGeneral/FrIfDevErrorDetect  = 'true') and (FrIfDefProgEnabled = 'true') and (FrIfInvariantAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRIF_STATIC_ASSERT_ENABLED)
#error FRIF_STATIC_ASSERT_ENABLED is already defined
#endif
/** \brief Static assertion usage
 **
 ** En- or disables the usage of static assertion checks */
#define FRIF_STATIC_ASSERT_ENABLED           [!//
[!IF "(../FrIfGeneral/FrIfDevErrorDetect  = 'true') and (FrIfDefProgEnabled = 'true') and (FrIfStaticAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!][!//






/*------------------------[Defensive programming]----------------------------*/

#if (defined FRIF_PRECONDITION_ASSERT)
#error FRIF_PRECONDITION_ASSERT is already defined
#endif

#if (defined FRIF_PRECONDITION_ASSERT_NO_EVAL)
#error FRIF_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (FRIF_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FRIF_PRECONDITION_ASSERT(Condition, ApiId) \
  DET_PRECONDITION_ASSERT((Condition), FRIF_MODULE_ID, FRIF_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FRIF_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL((Condition), FRIF_MODULE_ID, FRIF_INSTANCE_ID, (ApiId))
#else
#define FRIF_PRECONDITION_ASSERT(Condition, ApiId)
#define FRIF_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined FRIF_POSTCONDITION_ASSERT)
#error FRIF_POSTCONDITION_ASSERT is already defined
#endif

#if (defined FRIF_POSTCONDITION_ASSERT_NO_EVAL)
#error FRIF_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (FRIF_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FRIF_POSTCONDITION_ASSERT(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT((Condition), FRIF_MODULE_ID, FRIF_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FRIF_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL((Condition), FRIF_MODULE_ID, FRIF_INSTANCE_ID, (ApiId))
#else
#define FRIF_POSTCONDITION_ASSERT(Condition, ApiId)
#define FRIF_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined FRIF_INVARIANT_ASSERT)
#error FRIF_INVARIANT_ASSERT is already defined
#endif

#if (defined FRIF_INVARIANT_ASSERT_NO_EVAL)
#error FRIF_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (FRIF_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FRIF_INVARIANT_ASSERT(Condition, ApiId) \
  DET_INVARIANT_ASSERT((Condition), FRIF_MODULE_ID, FRIF_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FRIF_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL((Condition), FRIF_MODULE_ID, FRIF_INSTANCE_ID, (ApiId))
#else
#define FRIF_INVARIANT_ASSERT(Condition, ApiId)
#define FRIF_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined FRIF_STATIC_ASSERT)
# error FRIF_STATIC_ASSERT is already defined
#endif
#if (FRIF_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated */
# define FRIF_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define FRIF_STATIC_ASSERT(expr)
#endif

#if (defined FRIF_UNREACHABLE_CODE_ASSERT)
#error FRIF_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (FRIF_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 **

 ** \param[in] ApiId Service ID of the API function */
#define FRIF_UNREACHABLE_CODE_ASSERT(ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(FRIF_MODULE_ID, FRIF_INSTANCE_ID, (ApiId))
#else
#define FRIF_UNREACHABLE_CODE_ASSERT(ApiId)
#endif

#if (defined FRIF_INTERNAL_API_ID)
#error FRIF_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define FRIF_INTERNAL_API_ID DET_INTERNAL_API_ID



#endif /* FRIF_CFG_H */

/*==================[end of file]============================================*/

[!ENDCODE!][!//

