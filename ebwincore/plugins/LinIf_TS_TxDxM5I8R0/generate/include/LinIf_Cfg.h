/**
 * \file
 *
 * \brief AUTOSAR LinIf
 *
 * This file contains the implementation of the AUTOSAR
 * module LinIf.
 *
 * \version 5.8.27
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef LINIF_CFG_H
#define LINIF_CFG_H

[!NOCODE!][!INCLUDE "LinIf_Macros.m"!][!ENDNOCODE!]

/*==================[includes]===============================================*/
#include <Std_Types.h>      /* AUTOSAR standard types */

[!/* *** include drivers *** */!][!//
[!IF "$useDriverInfix = 'true'"!][!//
[!FOR "i"="0" TO "$numberOfDrivers - 1"!][!//
[!VAR "driverInfix"!][!CALL "getDriverInfo","driver" = "$currentDriver", "onlyInfix" = "true()", "index" = "$i"!][!ENDVAR!][!//
[!IF "$driverInfix = '_'"!][!//
#include <Lin.h>            /* AUTOSAR Lin definitions */
[!ELSE!][!//
#include <Lin_[!"$driverInfix"!].h>   /* AUTOSAR Lin definitions */
[!ENDIF!][!//
[!ENDFOR!][!//
[!ELSE!][!//
#include <Lin.h>            /* AUTOSAR Lin definitions */
[!ENDIF!][!//

[!IF "LinIfGeneral/LinIfBusMirroringSupported = 'true'"!]
[!IF "LinIfGeneral/LinIfMirrorToCDDReportingEnable = 'true'"!]
/* !LINKSTO LinIf.EB.SWS_LinIf_00872.Cdd,1 */
#include <[!"LinIfGeneral/LinIfMirrorToCDDReportingHeader"!]>
[!ENDIF!]
[!ENDIF!]

[!IF "$numTrcvs > 0"!][!//
#include <LinIf_TrcvTypes.h>

/*==================[external function declarations]=========================*/
#define LINIF_START_SEC_CONST_UNSPECIFIED
#include <LinIf_MemMap.h>

[!CALL "EnumerateTrcvs"!][!//
extern CONST(LinIf_LinTrcvFuncPtrType, LINIF_APPL_CONST) LinIf_TrcvFuncPtrs[[!"$numTrcvs"!]U];

#define LINIF_STOP_SEC_CONST_UNSPECIFIED
#include <LinIf_MemMap.h>
[!ENDIF!][!//

/*==================[macros]=================================================*/
/** \brief Maximum number of channels */
#if (defined LINIF_MAX_CHANNELS)
#error LINIF_MAX_CHANNELS already defined!
#endif /* #if( defined LINIF_MAX_CHANNELS)*/
#define LINIF_MAX_CHANNELS [!IF "LinIfGeneral/LinIfMaxChannels = 0"!]1U[!ELSE!][!"num:i(LinIfGeneral/LinIfMaxChannels)"!]U[!ENDIF!]

/** \brief Maximum number of Pdus */
[!VAR "NumOfPdus"="LinIfGeneral/LinIfMaxTxPdus"!]
#if (defined LINIF_MAX_PDUS_FLAGS)
#error LINIF_MAX_PDUS_FLAGS already defined!
#endif /* #if( defined LINIF_MAX_PDUS_FLAGS)*/
#define LINIF_MAX_PDUS_FLAGS [!IF "$NumOfPdus = 0"!]1U[!ELSE!][!IF "num:i($NumOfPdus div 8) = 0"!]1U[!ELSE!][!"num:i(($NumOfPdus div 8) + 1)"!]U[!ENDIF!][!ENDIF!] /*total number of flags is [!"num:i($NumOfPdus)"!] */

/** \brief Maximum number of Event triggered frames */
#if (defined LINIF_MAX_EV_TRGD)
#error LINIF_MAX_EV_TRGD already defined!
#endif /* #if( defined LINIF_MAX_EV_TRGD)*/
#define LINIF_MAX_EV_TRGD [!IF "LinIfGeneral/LinIfMaxEventTriggeredFrames = 0"!]1U[!ELSE!][!"LinIfGeneral/LinIfMaxEventTriggeredFrames"!]U[!ENDIF!]

/** \brief Master support availability */
#if (defined LINIF_MASTER_SUPPORTED)
#error LINIF_MASTER_SUPPORTED already defined!
#endif /* #if( defined LINIF_MASTER_SUPPORTED)*/
#define LINIF_MASTER_SUPPORTED STD_[!"node:when($numChannelsMaster = 0, 'OFF', 'ON')"!]

/** \brief Slave support availability */
#if (defined LINIF_SLAVE_SUPPORTED)
#error LINIF_SLAVE_SUPPORTED already defined!
#endif /* #if( defined LINIF_SLAVE_SUPPORTED)*/
#define LINIF_SLAVE_SUPPORTED STD_[!"node:when($numSlaveChannels = 0, 'OFF', 'ON')"!]

/** \brief Save Configuration Request availability */
#if (defined LINIF_NC_SAVECONF_SUPPORTED)
#error LINIF_NC_SAVECONF_SUPPORTED already defined!
#endif /* #if( defined LINIF_NC_SAVECONF_SUPPORTED)*/
#define LINIF_NC_SAVECONF_SUPPORTED STD_[!"node:when(node:exists(LinIfGeneral/LinIfSaveConfigurationCallout), 'ON', 'OFF')"!]

/** \brief Response error signal change callout availability */
#if (defined LINIF_ERR_RES_CHG_CO_SUPPORTED)
#error LINIF_ERR_RES_CHG_CO_SUPPORTED already defined!
#endif /* #if( defined LINIF_ERR_RES_CHG_CO_SUPPORTED)*/
#define LINIF_ERR_RES_CHG_CO_SUPPORTED STD_[!"node:when(node:exists(LinIfGeneral/LinIfResponseErrorSignalChangedCallout), 'ON', 'OFF')"!]

[!IF "$numSlaveChannels != 0"!][!//
/** \brief Maximum number of Slave frames */
#if (defined LINIF_MAX_SLAVE_FRAMES)
#error LINIF_MAX_SLAVE_FRAMES
#endif /* #if( defined LINIF_MAX_SLAVE_FRAMES) */
#define LINIF_MAX_SLAVE_FRAMES [!"$numFramesMax"!]U
[!ENDIF!][!//

/** \brief Version info API availability
 **
 ** En- or disable the availability of LinIf_GetVersionInfo() */
#if (defined LINIF_VERSION_INFO_API)
#error LINIF_VERSION_INFO_API already defined!
#endif /* #if( defined LINIF_VERSION_INFO_API)*/
#define LINIF_VERSION_INFO_API  STD_[!IF "LinIfGeneral/LinIfVersionInfoApi = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

/** \brief Macro for DET usage */
#if (defined LINIF_DEV_ERROR_DETECT)
#error LINIF_DEV_ERROR_DETECT already defined!
#endif /* #if( defined LINIF_DEV_ERROR_DETECT)*/
#define LINIF_DEV_ERROR_DETECT  STD_[!IF "LinIfGeneral/LinIfDevErrorDetect = 'true'"!]ON[!ELSE!]OFF[!ENDIF!][!//

/** \brief Macro for runtime DET usage */
#if (defined LINIF_RUNTIME_ERROR_REPORTING)
#error LINIF_RUNTIME_ERROR_REPORTING already defined!
#endif /* #if( defined LINIF_RUNTIME_ERROR_REPORTING)*/
#define LINIF_RUNTIME_ERROR_REPORTING  STD_[!IF "LinIfGeneral/LinIfRuntimeErrorReporting = 'true'"!]ON[!ELSE!]OFF[!ENDIF!][!//

/** \brief Macro for TP support */
#if (defined LINIF_TP_SUPPORTED)
#error LINIF_TP_SUPPORTED already defined!
#endif /* #if( defined LINIF_TP_SUPPORTED)*/
#define LINIF_TP_SUPPORTED  STD_[!IF "LinIfGeneral/LinIfTpSupported = 'true'"!]ON[!ELSE!]OFF[!ENDIF!][!//

#if (defined LINIF_MULTIPLE_DRIVER_SUPPORT)
#error LINIF_MULTIPLE_DRIVER_SUPPORT already defined!
#endif /* #if( defined LINIF_MULTIPLE_DRIVER_SUPPORT)*/
#define LINIF_MULTIPLE_DRIVER_SUPPORT    STD_[!IF "LinIfGeneral/LinIfMultipleDriversSupported = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

/** \brief Macro for Lin wake up support */
#if (defined LINIF_CHECKWAKEUP_SUPPORTED)
#error LINIF_CHECKWAKEUP_SUPPORTED already defined!
#endif /* #if( defined LINIF_CHECKWAKEUP_SUPPORTED)*/
#define LINIF_CHECKWAKEUP_SUPPORTED STD_[!IF "LinIfGeneral/LinIfCheckWakeupSupported = 'true'"!]ON[!ELSE!]OFF[!ENDIF!][!//

/** \brief Macro for LinTrcv wake up support */
#if (defined LINIF_TRCVCHECKWAKEUP_SUPPORTED)
#error LINIF_TRCVCHECKWAKEUP_SUPPORTED already defined!
#endif /* #if( defined LINIF_TRCVCHECKWAKEUP_SUPPORTED)*/
#define LINIF_TRCVCHECKWAKEUP_SUPPORTED STD_[!IF "$cwuNeeded = 'true'"!]ON[!ELSE!]OFF[!ENDIF!][!//

#if (defined LINIF_TRCV_SUPPORTED)
#error LINIF_TRCV_SUPPORTED already defined!
#endif /* #if( defined LINIF_TRCV_SUPPORTED)*/
#define LINIF_TRCV_SUPPORTED STD_[!IF "$numTrcvs > 0"!]ON[!ELSE!]OFF[!ENDIF!][!//

#if (defined LINIF_TRCV_MULTIPLE_SUPPORTED)
#error LINIF_TRCV_MULTIPLE_SUPPORTED already defined!
#endif /* #if( defined LINIF_TRCV_MULTIPLE_SUPPORTED)*/
#define LINIF_TRCV_MULTIPLE_SUPPORTED STD_[!IF "$numTrcvs > 1"!]ON[!ELSE!]OFF[!ENDIF!][!//

#if (defined LINIF_PROD_ERR_HANDLING_TX_BIT)
#error LINIF_PROD_ERR_HANDLING_TX_BIT already defined!
#endif /* #if( defined LINIF_PROD_ERR_HANDLING_TX_BIT)*/

#if (defined LINIF_E_DEMTODET_TX_BIT)
#error LINIF_E_DEMTODET_TX_BIT already defined!
#endif /* #if( defined LINIF_E_DEMTODET_TX_BIT)*/

#if (defined LINIF_TX_BIT_PASSED)
#error LINIF_TX_BIT_PASSED already defined!
#endif /* #if( defined LINIF_TX_BIT_PASSED)*/

#if (defined LINIF_TX_BIT_FAILED)
#error LINIF_TX_BIT_FAILED already defined!
#endif /* #if( defined LINIF_TX_BIT_FAILED)*/

/** \brief Switch for DEM to DET reporting for production error LINIF_PROD_ERR_HANDLING_TX_BIT */
#define LINIF_PROD_ERR_HANDLING_TX_BIT [!//
[!IF "LinIfGeneral/ReportToDem/LinIfTxBitErrorReportToDem = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "LinIfGeneral/ReportToDem/LinIfTxBitErrorReportToDem = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!//

[!IF "LinIfGeneral/ReportToDem/LinIfTxBitErrorReportToDem = 'DET'"!][!//
/** \brief Det error ID, if DEM to DET reporting is enabled for LINIF_PROD_ERR_HANDLING_TX_BIT */
#define LINIF_E_DEMTODET_TX_BIT          [!"LinIfGeneral/ReportToDem/LinIfTxBitErrorDemDetErrorId"!]U
[!ELSEIF "LinIfGeneral/ReportToDem/LinIfTxBitErrorReportToDem = 'DEM'"!][!//
[!IF "LinIfGeneral/ReportToDem/LinIfTxBitErrorDebounceMethod = 'DEM'"!][!//
#define LINIF_TX_BIT_PASSED   DEM_EVENT_STATUS_PREPASSED
#define LINIF_TX_BIT_FAILED   DEM_EVENT_STATUS_PREFAILED
[!ELSE!][!//
#define LINIF_TX_BIT_PASSED   DEM_EVENT_STATUS_PASSED
#define LINIF_TX_BIT_FAILED   DEM_EVENT_STATUS_FAILED
[!ENDIF!][!//
[!ENDIF!][!//

#if (defined LINIF_PROD_ERR_HANDLING_RX_CHECKSUM)
#error LINIF_PROD_ERR_HANDLING_RX_CHECKSUM already defined!
#endif /* #if( defined LINIF_PROD_ERR_HANDLING_RX_CHECKSUM)*/

#if (defined LINIF_E_DEMTODET_RX_CHECKSUM)
#error LINIF_E_DEMTODET_RX_CHECKSUM already defined!
#endif /* #if( defined LINIF_E_DEMTODET_RX_CHECKSUM)*/

#if (defined LINIF_RX_CHECKSUM_PASSED)
#error LINIF_RX_CHECKSUM_PASSED already defined!
#endif /* #if( defined LINIF_RX_CHECKSUM_PASSED)*/

#if (defined LINIF_RX_CHECKSUM_FAILED)
#error LINIF_RX_CHECKSUM_FAILED already defined!
#endif /* #if( defined LINIF_RX_CHECKSUM_FAILED)*/

/** \brief Switch for DEM to DET reporting for production error LINIF_PROD_ERR_HANDLING_RX_CHECKSUM */
#define LINIF_PROD_ERR_HANDLING_RX_CHECKSUM [!//
[!IF "LinIfGeneral/ReportToDem/LinIfRxChecksumErrorReportToDem = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "LinIfGeneral/ReportToDem/LinIfRxChecksumErrorReportToDem = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!//

[!IF "LinIfGeneral/ReportToDem/LinIfRxChecksumErrorReportToDem = 'DET'"!][!//
/** \brief Det error ID, if DEM to DET reporting is enabled for LINIF_PROD_ERR_HANDLING_RX_CHECKSUM */
#define LINIF_E_DEMTODET_RX_CHECKSUM    [!"LinIfGeneral/ReportToDem/LinIfRxChecksumErrDemDetErrorId"!]U
[!ELSEIF "LinIfGeneral/ReportToDem/LinIfRxChecksumErrorReportToDem = 'DEM'"!][!//
[!IF "LinIfGeneral/ReportToDem/LinIfRxChecksumDebounceMethod = 'DEM'"!][!//
#define LINIF_RX_CHECKSUM_PASSED   DEM_EVENT_STATUS_PREPASSED
#define LINIF_RX_CHECKSUM_FAILED   DEM_EVENT_STATUS_PREFAILED
[!ELSE!][!//
#define LINIF_RX_CHECKSUM_PASSED   DEM_EVENT_STATUS_PASSED
#define LINIF_RX_CHECKSUM_FAILED   DEM_EVENT_STATUS_FAILED
[!ENDIF!][!//
[!ENDIF!][!//

#if (defined LINIF_PROD_ERR_HANDLING_RX_NO_RESPONSE)
#error LINIF_PROD_ERR_HANDLING_RX_NO_RESPONSE already defined!
#endif /* #if( defined LINIF_PROD_ERR_HANDLING_RX_NO_RESPONSE)*/

#if (defined LINIF_E_DEMTODET_RX_NO_RESPONSE)
#error LINIF_E_DEMTODET_RX_NO_RESPONSE already defined!
#endif /* #if( defined LINIF_E_DEMTODET_RX_NO_RESPONSE)*/

#if (defined LINIF_RX_NO_RESPONSE_PASSED)
#error LINIF_RX_NO_RESPONSE_PASSED already defined!
#endif /* #if( defined LINIF_RX_NO_RESPONSE_PASSED)*/

#if (defined LINIF_RX_NO_RESPONSE_FAILED)
#error LINIF_RX_NO_RESPONSE_FAILED already defined!
#endif /* #if( defined LINIF_RX_NO_RESPONSE_FAILED)*/

/** \brief Switch for DEM to DET reporting for production error LINIF_PROD_ERR_HANDLING_RX_NO_RESPONSE */
#define LINIF_PROD_ERR_HANDLING_RX_NO_RESPONSE [!//
[!IF "LinIfGeneral/ReportToDem/LinIfRxNoRespErrorReportToDem = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "LinIfGeneral/ReportToDem/LinIfRxNoRespErrorReportToDem = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!//

[!IF "LinIfGeneral/ReportToDem/LinIfRxNoRespErrorReportToDem = 'DET'"!][!//
/** \brief Det error ID, if DEM to DET reporting is enabled for LINIF_PROD_ERR_HANDLING_RX_NO_RESPONSE */
#define LINIF_E_DEMTODET_RX_NO_RESPONSE    [!"LinIfGeneral/ReportToDem/LinIfRxNoRespErrDemDetErrorId"!]U
[!ELSEIF "LinIfGeneral/ReportToDem/LinIfRxNoRespErrorReportToDem = 'DEM'"!][!//
[!IF "LinIfGeneral/ReportToDem/LinIfRxNoRespDebounceMethod = 'DEM'"!][!//
#define LINIF_RX_NO_RESPONSE_PASSED   DEM_EVENT_STATUS_PREPASSED
#define LINIF_RX_NO_RESPONSE_FAILED   DEM_EVENT_STATUS_PREFAILED
[!ELSE!][!//
#define LINIF_RX_NO_RESPONSE_PASSED   DEM_EVENT_STATUS_PASSED
#define LINIF_RX_NO_RESPONSE_FAILED   DEM_EVENT_STATUS_FAILED
[!ENDIF!][!//
[!ENDIF!][!//

#if (defined LINIF_USE_LINAPI_REV2)
#error LINIF_USE_LINAPI_REV2 already defined!
#endif /* #if( defined LINIF_USE_LINAPI_REV2)*/

#if (defined LINIF_USE_LINAPI_REV3)
#error LINIF_USE_LINAPI_REV3 already defined!
#endif /* #if( defined LINIF_USE_LINAPI_REV3)*/

#if (defined LINIF_USE_LINAPI_42)
#error LINIF_USE_LINAPI_42 already defined!
#endif /* #if( defined LINIF_USE_LINAPI_42)*/

#if (defined LINIF_WAKEUP_REV2_NOT_USED)
#error LINIF_WAKEUP_REV2_NOT_USED already defined!
#endif /* #if (defined LINIF_WAKEUP_REV2_NOT_USED)*/

#if (defined LINIF_WAKEUP_LINAPI_42)
#error LINIF_WAKEUP_LINAPI_42 already defined!
#endif /* #if (defined LINIF_WAKEUP_LINAPI_42)*/

#if (defined LINIF_LIN_RESP_MR_TX)
#error LINIF_LIN_RESP_MR_TX already defined!
#endif /* #if (defined LINIF_LIN_RESP_MR_TX)*/

#if (defined LINIF_LIN_RESP_SL_RX)
#error LINIF_LIN_RESP_SL_RX already defined!
#endif /* #if (defined LINIF_LIN_RESP_SL_RX)*/

#if (defined LINIF_LIN_RESP_S2S_IGN)
#error LINIF_LIN_RESP_S2S_IGN already defined!
#endif /* #if (defined LINIF_LIN_RESP_S2S_IGN)*/

/** \brief Macro for Lin driver API switch */
[!IF "LinIfGeneral/LinIfLinDriverAPI = 'REV2'"!][!//
#define LINIF_USE_LINAPI_REV2
[!ELSEIF "LinIfGeneral/LinIfLinDriverAPI = 'REV3'"!][!//
#define LINIF_USE_LINAPI_REV3
[!IF "LinIfGeneral/LinIfCheckWakeupSupported = 'true'"!][!//
/** \brief Available for REV3 and REV42 */
#define LINIF_WAKEUP_REV2_NOT_USED
[!ENDIF!][!//
[!ELSE!][!//
#define LINIF_USE_LINAPI_42
[!IF "LinIfGeneral/LinIfCheckWakeupSupported = 'true'"!][!//
/** \brief Available for REV3 and 42 driver*/
#define LINIF_WAKEUP_REV2_NOT_USED
/** \brief Wakeup is available for 42 driver*/
#define LINIF_WAKEUP_LINAPI_42
[!ENDIF!][!// LinIfCheckWakeupSupported
[!ENDIF!][!// LinIfLinDriverAPI
[!IF "LinIfGeneral/LinIfLinDriverAPI = 'REV431' or LinIfGeneral/LinIfLinDriverAPI = 'REV44'"!][!//
#define LINIF_USE_LINAPI_REV431
[!ENDIF!][!//
/* !LINKSTO LinIf.EB.Lin_FrameResponseType,1 */
[!IF "LinIfGeneral/LinIfLinDriverAPI = 'REV44'"!][!//
#define LINIF_USE_LINAPI_REV44
#define LINIF_LIN_RESP_MR_TX   LIN_FRAMERESPONSE_TX
#define LINIF_LIN_RESP_SL_RX   LIN_FRAMERESPONSE_RX
#define LINIF_LIN_RESP_S2S_IGN LIN_FRAMERESPONSE_IGNORE
[!ELSE!][!//
#define LINIF_LIN_RESP_MR_TX   LIN_MASTER_RESPONSE
#define LINIF_LIN_RESP_SL_RX   LIN_SLAVE_RESPONSE
#define LINIF_LIN_RESP_S2S_IGN LIN_SLAVE_TO_SLAVE
[!ENDIF!][!//

[!IF "node:exists(as:modconf('LinIf')[1]/LinIfGeneral/LinIfLinErrorCalloutName)"!]
/** \brief Macro for Lin bus user error callout support */
#if (defined LINIF_LIN_ERROR_CALLOUT)
#error LINIF_LIN_ERROR_CALLOUT already defined!
#endif /* #if( defined LINIF_LIN_ERROR_CALLOUT)*/
#define LINIF_LIN_ERROR_CALLOUT STD_ON

/** \brief Macro for Lin bus user error callout */
#if (defined LINIF_LIN_ERROR_CALLOUT_FCT)
#error LINIF_LIN_ERROR_CALLOUT_FCT already defined!
#endif /* #if( defined LINIF_LIN_ERROR_CALLOUT_FCT)*/
#define LINIF_LIN_ERROR_CALLOUT_FCT [!"as:modconf('LinIf')[1]/LinIfGeneral/LinIfLinErrorCalloutName"!]

[!IF "'true' = as:modconf('LinIf')[1]/LinIfGeneral/LinIfLinErrorCalloutStatusForward"!]
/** \brief Macro for Lin bus user error status forwarding */
#if (defined LINIF_LIN_ERROR_CALLOUT_STATUS_FWD)
#error LINIF_LIN_ERROR_CALLOUT_STATUS_FWD already defined!
#endif /* #if( defined LINIF_LIN_ERROR_CALLOUT_STATUS_FWD)*/
#define LINIF_LIN_ERROR_CALLOUT_STATUS_FWD STD_ON

/** \brief Macro used to call the user callout for failed reception / transmission */
#if (defined LINIF_LIN_ERROR_NOTIFY)
#error LINIF_LIN_ERROR_NOTIFY already defined!
#endif /* (defined LINIF_LIN_ERROR_NOTIFY) */
#define LINIF_LIN_ERROR_NOTIFY(id, status) [!"as:modconf('LinIf')[1]/LinIfGeneral/LinIfLinErrorCalloutName"!](LINIF_GET_COMM_CHANNEL((id)), (status))
[!ELSE!]
/** \brief Macro for Lin bus user error status forwarding */
#if (defined LINIF_LIN_ERROR_CALLOUT_STATUS_FWD)
#error LINIF_LIN_ERROR_CALLOUT_STATUS_FWD already defined!
#endif /* #if( defined LINIF_LIN_ERROR_CALLOUT_STATUS_FWD)*/
#define LINIF_LIN_ERROR_CALLOUT_STATUS_FWD STD_OFF

/** \brief Macro used to call the user callout for failed reception / transmission */
#if (defined LINIF_LIN_ERROR_NOTIFY)
#error LINIF_LIN_ERROR_NOTIFY already defined!
#endif /* (defined LINIF_LIN_ERROR_NOTIFY) */
#define LINIF_LIN_ERROR_NOTIFY(id, status) [!"as:modconf('LinIf')[1]/LinIfGeneral/LinIfLinErrorCalloutName"!](LINIF_GET_COMM_CHANNEL((id)))
[!ENDIF!]

/** \brief Macro for Lin bus user error callout header file */
#if (defined LINIF_LIN_ERROR_CALLOUT_INCLUDE)
#error LINIF_LIN_ERROR_CALLOUT_INCLUDE already defined!
#endif /* #if( defined LINIF_LIN_ERROR_CALLOUT_INCLUDE)*/
#define LINIF_LIN_ERROR_CALLOUT_INCLUDE "[!"as:modconf('LinIf')[1]/LinIfGeneral/LinIfLinErrorCalloutHeaderFile"!]"
[!ELSE!]
/** \brief Macro for Lin bus user error callout support */
#if (defined LINIF_LIN_ERROR_CALLOUT)
#error LINIF_LIN_ERROR_CALLOUT already defined!
#endif /* #if( defined LINIF_LIN_ERROR_CALLOUT)*/
#define LINIF_LIN_ERROR_CALLOUT STD_OFF

/** \brief Macro for Lin bus user error status forwarding */
#if (defined LINIF_LIN_ERROR_CALLOUT_STATUS_FWD)
#error LINIF_LIN_ERROR_CALLOUT_STATUS_FWD already defined!
#endif /* #if( defined LINIF_LIN_ERROR_CALLOUT_STATUS_FWD)*/
#define LINIF_LIN_ERROR_CALLOUT_STATUS_FWD STD_OFF

/** \brief Macro used to call the user callout for failed reception / transmission */
#if (defined LINIF_LIN_ERROR_NOTIFY)
#error LINIF_LIN_ERROR_NOTIFY already defined!
#endif /* (defined LINIF_LIN_ERROR_NOTIFY) */
#define LINIF_LIN_ERROR_NOTIFY(id, status) ((void)0U)
[!ENDIF!]

[!IF "node:exists(as:modconf('LinIf')[1]/LinIfGeneral/LinIfLinSuccessCalloutName)"!]
/** \brief Macro for Lin successful Rx/Tx notification callout support */
#if (defined LINIF_LIN_SUCCESS_CALLOUT)
#error LINIF_LIN_SUCCESS_CALLOUT already defined!
#endif /* #if( defined LINIF_LIN_SUCCESS_CALLOUT)*/
#define LINIF_LIN_SUCCESS_CALLOUT STD_ON

/** \brief Macro for Lin successful Rx/Tx notification callout header file */
#if (defined LINIF_LIN_SUCCESS_CALLOUT_INCLUDE)
#error LINIF_LIN_SUCCESS_CALLOUT_INCLUDE already defined!
#endif /* #if( defined LINIF_LIN_SUCCESS_CALLOUT_INCLUDE)*/
#define LINIF_LIN_SUCCESS_CALLOUT_INCLUDE "[!"as:modconf('LinIf')[1]/LinIfGeneral/LinIfLinSuccessCalloutHeaderFile"!]"

/** \brief Macro used to call the user callout for successful reception / transmission */
#if (defined LINIF_LIN_SUCCESS_NOTIFY)
#error LINIF_LIN_SUCCESS_NOTIFY already defined!
#endif /* (defined LINIF_LIN_SUCCESS_NOTIFY) */
[!IF "'true' = as:modconf('LinIf')[1]/LinIfGeneral/LinIfLinSuccessCalloutStatusForward"!][!//
#define LINIF_LIN_SUCCESS_NOTIFY(id, status) [!"as:modconf('LinIf')[1]/LinIfGeneral/LinIfLinSuccessCalloutName"!](LINIF_GET_COMM_CHANNEL((id)), (status))
[!ELSE!][!//
#define LINIF_LIN_SUCCESS_NOTIFY(id, status) [!"as:modconf('LinIf')[1]/LinIfGeneral/LinIfLinSuccessCalloutName"!](LINIF_GET_COMM_CHANNEL((id)))
[!ENDIF!][!//

[!ELSE!]
/** \brief Macro for Lin successful Rx/Tx notification callout support */
#if (defined LINIF_LIN_SUCCESS_CALLOUT)
#error LINIF_LIN_SUCCESS_CALLOUT already defined!
#endif /* #if( defined LINIF_LIN_SUCCESS_CALLOUT)*/
#define LINIF_LIN_SUCCESS_CALLOUT STD_OFF

/** \brief Macro used to call the user callout for successful reception / transmission */
#if (defined LINIF_LIN_SUCCESS_NOTIFY)
#error LINIF_LIN_SUCCESS_NOTIFY already defined!
#endif /* (defined LINIF_LIN_SUCCESS_NOTIFY) */
#define LINIF_LIN_SUCCESS_NOTIFY(id, status) ((void)0U)
[!ENDIF!]

/** \brief Macro for LinIf end-of-schedule notification support */
#if (defined LINIF_EOS_NOTIF_CALLOUT_SUPPORTED)
#error LINIF_EOS_NOTIF_CALLOUT_SUPPORTED already defined!
#endif /* #if( defined LINIF_EOS_NOTIF_CALLOUT_SUPPORTED)*/
#define LINIF_EOS_NOTIF_CALLOUT_SUPPORTED STD_[!IF "$endOfSchedNotifSupported = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

/** \brief Upper layer Cdd support */
#if (defined LINIF_UL_CDD_SUPPORTED)
#error LINIF_UL_CDD_SUPPORTED already defined!
#endif /* #if( defined LINIF_UL_CDD_SUPPORTED)*/
#define LINIF_UL_CDD_SUPPORTED STD_[!IF "$ulCddSupported = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined LINIF_MAP_CHANNEL_ID_DIRECT)
#error LINIF_MAP_CHANNEL_ID_DIRECT already defined!
#endif /* #if( defined LINIF_MAP_CHANNEL_ID_DIRECT)*/
#define LINIF_MAP_CHANNEL_ID_DIRECT    STD_[!IF "LinIfGeneral/LinIfMapChannelIdDirect = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined LINIF_MAP_COMM_CHANNEL_ID_DIRECT)
#error LINIF_MAP_COMM_CHANNEL_ID_DIRECT already defined!
#endif /* #if( defined LINIF_MAP_COMM_CHANNEL_ID_DIRECT)*/
#define LINIF_MAP_COMM_CHANNEL_ID_DIRECT    STD_[!IF "LinIfGeneral/LinIfMapComMChannelIdDirect = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

/** \brief Macro for PBCFGM usage */
#if (defined LINIF_PBCFGM_SUPPORT_ENABLED)
#error LINIF_PBCFGM_SUPPORT_ENABLED already defined!
#endif /* #if( defined LINIF_PBCFGM_SUPPORT_ENABLED)*/
#define LINIF_PBCFGM_SUPPORT_ENABLED [!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('LinIf')) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Compile time verification value */
#if (defined LINIF_CFG_SIGNATURE)
#error LINIF_CFG_SIGNATURE already defined!
#endif /* #if( defined LINIF_CFG_SIGNATURE)*/
#define LINIF_CFG_SIGNATURE [!"asc:getConfigSignature(as:modconf('LinIf')[1]//*[not(child::*) and (node:configclass() = 'PreCompile') ])"!]U

#if (defined LINIF_PUBLIC_INFO_SIGNATURE)
#error LINIF_PUBLIC_INFO_SIGNATURE already defined!
#endif /* #if( defined LINIF_PUBLIC_INFO_SIGNATURE)*/
#define LINIF_PUBLIC_INFO_SIGNATURE [!"asc:getConfigSignature(node:difference(as:modconf('LinIf')[1]/CommonPublishedInformation//*[not(child::*) and (node:configclass() = 'PublishedInformation') ], as:modconf('LinIf')[1]/CommonPublishedInformation/Release))"!]U /* PublicInfoSignature */

/* ----- Pre-processor switch to enable/diable relocateable postbuild config ----- */
[!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('LinIf')) = 'true'"!]
#if (defined LINIF_RELOCATABLE_CFG_ENABLE)
#error LINIF_RELOCATABLE_CFG_ENABLE already defined!
#endif /* #if( defined LINIF_RELOCATABLE_CFG_ENABLE)*/
#define LINIF_RELOCATABLE_CFG_ENABLE [!IF "as:modconf('PbcfgM')/PbcfgMGeneral/PbcfgMRelocatableCfgEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!ELSE!]
#if (defined LINIF_RELOCATABLE_CFG_ENABLE)
#error LINIF_RELOCATABLE_CFG_ENABLE already defined!
#endif /* #if( defined LINIF_RELOCATABLE_CFG_ENABLE)*/
#define LINIF_RELOCATABLE_CFG_ENABLE [!IF "LinIfGeneral/LinIfRelocatablePbcfgEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!ENDIF!]

#if (defined LINIF_BUS_MIRRORING_SUPPORT_ENABLE)
#error LINIF_BUS_MIRRORING_SUPPORT_ENABLE already defined!
#endif /* #if( defined LINIF_BUS_MIRRORING_SUPPORT_ENABLE)*/
#define LINIF_BUS_MIRRORING_SUPPORT_ENABLE [!IF "LinIfGeneral/LinIfBusMirroringSupported = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!IF "LinIfGeneral/LinIfBusMirroringSupported = 'true'"!]
#if (defined LINIF_CDD_REPORTING_ENABLE)
#error LINIF_CDD_REPORTING_ENABLE already defined!
#endif /* #if( defined LINIF_CDD_REPORTING_ENABLE)*/
#define LINIF_CDD_REPORTING_ENABLE [!IF "LinIfGeneral/LinIfMirrorToCDDReportingEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined LINIF_UL_REPORTING)
#error LINIF_UL_REPORTING already defined!
#endif /* #if( defined LINIF_UL_REPORTING)*/
[!IF "LinIfGeneral/LinIfMirrorToCDDReportingEnable = 'true'"!]
#define LINIF_UL_REPORTING [!"LinIfGeneral/LinIfMirrorToCDDReportingFunctionName"!]
[!ELSE!]
#define LINIF_UL_REPORTING Mirror_ReportLinFrame
[!ENDIF!] /* LinIfGeneral/LinIfMirrorToCDDReportingEnable = 'true' */

[!ENDIF!] /* LinIfGeneral/LinIfBusMirroringSupported = 'true' */

#if (defined LINIF_MIRROR_ON_MULTICORE_SUPPORT_ENABLE)
#error LINIF_MIRROR_ON_MULTICORE_SUPPORT_ENABLE already defined!
#endif /* #if( defined LINIF_MIRROR_ON_MULTICORE_SUPPORT_ENABLE)*/
#define LINIF_MIRROR_ON_MULTICORE_SUPPORT_ENABLE [!IF "LinIfGeneral/LinIfMirroringOnMultiCoreSupported = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!IF "LinIfGeneral/LinIfTpSupported = 'true'"!]

/** \brief Version info API availability
 **
 ** En- or disable the availability of LinTp_GetVersionInfo() */
#if (defined LINTP_VERSION_INFO_API)
#error LINTP_VERSION_INFO_API already defined!
#endif /* #if( defined LINTP_VERSION_INFO_API)*/
#define LINTP_VERSION_INFO_API  STD_[!IF "as:modconf('LinTp')[1]/LinTpGeneral/LinTpVersionInfoApi = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

/** \brief Enables or disables the call of BswM_LinTp_RequestMode() */
#if (defined LINTP_SCHEDULE_CHANGE_DIAG_API)
#error LINTP_SCHEDULE_CHANGE_DIAG_API already defined!
#endif /* #if( defined LINTP_SCHEDULE_CHANGE_DIAG_API)*/
#define LINTP_SCHEDULE_CHANGE_DIAG_API  STD_[!IF "node:fallback("->node:value(as:modconf('LinTp')[1]/LinTpGeneral/LinTpScheduleChangeDiagApiEnable)", false()) = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

/** \brief Macro for PBCFGM usage */
#if (defined LINTP_PBCFGM_SUPPORT_ENABLED)
#error LINTP_PBCFGM_SUPPORT_ENABLED already defined!
#endif /* #if( defined LINTP_PBCFGM_SUPPORT_ENABLED)*/
#define LINTP_PBCFGM_SUPPORT_ENABLED [!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('LinTp')) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Compile time verification value */
#if (defined LINTP_CFG_SIGNATURE)
#error LINTP_CFG_SIGNATURE already defined!
#endif /* #if( defined LINTP_CFG_SIGNATURE)*/
#define LINTP_CFG_SIGNATURE [!"asc:getConfigSignature(as:modconf('LinTp')[1]//*[not(child::*) and (node:configclass() = 'PreCompile') ])"!]U

/** \brief Macro for PBCFGM usage */
#if (defined LINTP_PUBLIC_INFO_SIGNATURE)
#error LINTP_PUBLIC_INFO_SIGNATURE already defined!
#endif /* #if( defined LINTP_PUBLIC_INFO_SIGNATURE)*/
#define LINTP_PUBLIC_INFO_SIGNATURE [!"asc:getConfigSignature(node:difference(as:modconf('LinTp')[1]/CommonPublishedInformation//*[not(child::*) and (node:configclass() = 'PublishedInformation') ], as:modconf('LinTp')[1]/CommonPublishedInformation/Release))"!]U /* PublicInfoSignature */

/* ----- Pre-processor switch to enable/diable relocateable postbuild config ----- */
[!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('LinTp')) = 'true'"!]
#if (defined LINTP_RELOCATABLE_CFG_ENABLE)
#error LINTP_RELOCATABLE_CFG_ENABLE already defined!
#endif /* #if( defined LINTP_RELOCATABLE_CFG_ENABLE)*/
#define LINTP_RELOCATABLE_CFG_ENABLE [!IF "as:modconf('PbcfgM')/PbcfgMGeneral/PbcfgMRelocatableCfgEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!ELSE!]
#if (defined LINTP_RELOCATABLE_CFG_ENABLE)
#error LINTP_RELOCATABLE_CFG_ENABLE already defined!
#endif /* #if( defined LINTP_RELOCATABLE_CFG_ENABLE)*/
#define LINTP_RELOCATABLE_CFG_ENABLE [!IF "as:modconf('LinTp')[1]/LinTpGeneral/LinTpRelocatablePbcfgEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!ENDIF!]

[!ENDIF!]

#if (LINIF_DEV_ERROR_DETECT == STD_ON)



/*------------------[Defensive programming]---------------------------------*/
[!SELECT "LinIfDefensiveProgramming"!][!//

#if (defined LINIF_DEFENSIVE_PROGRAMMING_ENABLED)
#error LINIF_DEFENSIVE_PROGRAMMING_ENABLED is already defined
#endif
/** \brief Defensive programming usage
 **
 ** En- or disables the usage of the defensive programming */
#define LINIF_DEFENSIVE_PROGRAMMING_ENABLED   [!//
[!IF "(../LinIfGeneral/LinIfDevErrorDetect  = 'true') and (LinIfDefProgEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined LINIF_PRECONDITION_ASSERT_ENABLED)
#error LINIF_PRECONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Precondition assertion usage
 **
 ** En- or disables the usage of precondition assertion checks */
#define LINIF_PRECONDITION_ASSERT_ENABLED     [!//
[!IF "(../LinIfGeneral/LinIfDevErrorDetect  = 'true') and (LinIfDefProgEnabled = 'true') and (LinIfPrecondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined LINIF_POSTCONDITION_ASSERT_ENABLED)
#error LINIF_POSTCONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Postcondition assertion usage
 **
 ** En- or disables the usage of postcondition assertion checks */
#define LINIF_POSTCONDITION_ASSERT_ENABLED    [!//
[!IF "(../LinIfGeneral/LinIfDevErrorDetect  = 'true') and (LinIfDefProgEnabled = 'true') and (LinIfPostcondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined LINIF_UNREACHABLE_CODE_ASSERT_ENABLED)
#error LINIF_UNREACHABLE_CODE_ASSERT_ENABLED is already defined
#endif
/** \brief Unreachable code assertion usage
 **
 ** En- or disables the usage of unreachable code assertion checks */
#define LINIF_UNREACHABLE_CODE_ASSERT_ENABLED [!//
[!IF "(../LinIfGeneral/LinIfDevErrorDetect  = 'true') and (LinIfDefProgEnabled = 'true') and (LinIfUnreachAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined LINIF_INVARIANT_ASSERT_ENABLED)
#error LINIF_INVARIANT_ASSERT_ENABLED is already defined
#endif
/** \brief Invariant assertion usage
 **
 ** En- or disables the usage of invariant assertion checks */
#define LINIF_INVARIANT_ASSERT_ENABLED        [!//
[!IF "(../LinIfGeneral/LinIfDevErrorDetect  = 'true') and (LinIfDefProgEnabled = 'true') and (LinIfInvariantAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined LINIF_STATIC_ASSERT_ENABLED)
#error LINIF_STATIC_ASSERT_ENABLED is already defined
#endif
/** \brief Static assertion usage
 **
 ** En- or disables the usage of static assertion checks */
#define LINIF_STATIC_ASSERT_ENABLED           [!//
[!IF "(../LinIfGeneral/LinIfDevErrorDetect  = 'true') and (LinIfDefProgEnabled = 'true') and (LinIfStaticAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!][!//

#endif /* LINIF_DEV_ERROR_DETECT == STD_ON */

#endif /* ifndef LINIF_CFG_H */

/*==================[end of file]============================================*/
