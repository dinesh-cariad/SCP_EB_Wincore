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
#ifndef DLT_H
#define DLT_H

/* This file contains or includes all external identifiers of
 * the AUTOSAR module Dlt. */

/*==[Includes]================================================================*/

#include <Dlt_BSW.h>
#include <Dlt_Control.h>

#include <Dlt_Version.h>
#include <Dlt_Cbk.h>

/*==[Macros]==================================================================*/

#define Dlt_RegisterContext         Dlt_ASR43_RegisterContext
#define Dlt_UnregisterContext       Dlt_ASR43_UnregisterContext
#define Dlt_SendLogMessage          Dlt_ASR43_SendLogMessage
#define Dlt_SendTraceMessage        Dlt_ASR43_SendTraceMessage
#define Dlt_GetLogInfo              Dlt_ASR43_GetLogInfo

#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)

#define Dlt_SetLogChannelAssignment Dlt_ASR43_SetLogChannelAssignment
#define Dlt_GetLogChannelNames      Dlt_ASR43_GetLogChannelNames
#define Dlt_SetLogChannelThreshold  Dlt_ASR43_SetLogChannelThreshold
#define Dlt_GetLogChannelThreshold  Dlt_ASR43_GetLogChannelThreshold
#define Dlt_ResetToFactoryDefault   Dlt_ASR43_ResetToFactoryDefault
#define Dlt_StoreConfiguration      Dlt_ASR43_StoreConfiguration
#define Dlt_SetLogLevel             Dlt_ASR43_SetLogLevel
#define Dlt_SetTraceStatus          Dlt_ASR43_SetTraceStatus
#define Dlt_GetTraceStatus          Dlt_ASR43_GetTraceStatus
#define Dlt_GetDefaultLogLevel      Dlt_ASR43_GetDefaultLogLevel
#define Dlt_SetDefaultLogLevel      Dlt_ASR43_SetDefaultLogLevel
#define Dlt_GetDefaultTraceStatus   Dlt_ASR43_GetDefaultTraceStatus
#define Dlt_SetDefaultTraceStatus   Dlt_ASR43_SetDefaultTraceStatus
#define Dlt_SetMessageFiltering     Dlt_ASR43_SetMessageFiltering

#else /* (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431) */

#if (DLT_IMPLEMENT_FILTER_MESSAGES == STD_ON)

#define Dlt_SetLogLevel             Dlt_ASR42_SetLogLevel
#define Dlt_SetTraceStatus          Dlt_ASR42_SetTraceStatus
#define Dlt_GetTraceStatus          Dlt_ASR42_GetTraceStatus

#endif /* DLT_IMPLEMENT_FILTER_MESSAGES == STD_ON */

#define Dlt_SetDefaultLogLevel      Dlt_ASR42_SetDefaultLogLevel
#define Dlt_GetDefaultLogLevel      Dlt_ASR42_GetDefaultLogLevel
#define Dlt_SetDefaultTraceStatus   Dlt_ASR42_SetDefaultTraceStatus
#define Dlt_GetDefaultTraceStatus   Dlt_ASR42_GetDefaultTraceStatus
#define Dlt_SetMessageFiltering     Dlt_ASR42_SetMessageFiltering

#endif /* DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431 */

/*--[API Service IDs]---------------------------------------------------------*/

#if (defined DLT_SID_Init)
#error DLT_SID_Init already defined
#endif
/** \brief Service Id for Dlt_Init() */
#define DLT_SID_Init                                              0x01U

#if (defined DLT_SID_GetVersionInfo)
#error DLT_SID_GetVersionInfo already defined
#endif
/** \brief Service Id for Dlt_GetVersionInfo() */
#define DLT_SID_GetVersionInfo                                    0x02U

#if (defined DLT_SID_SendLogMessage)
#error DLT_SID_SendLogMessage already defined
#endif
/** \brief Service Id for Dlt_SendLogMessage() */
#define DLT_SID_SendLogMessage                                    0x03U

#if (defined DLT_SID_SendTraceMessage)
#error DLT_SID_SendTraceMessage already defined
#endif
/** \brief Service Id for Dlt_SendTraceMessage() */
#define DLT_SID_SendTraceMessage                                  0x04U

#if (defined DLT_SID_RegisterContext)
#error DLT_SID_RegisterContext already defined
#endif
/** \brief Service Id for Dlt_RegisterContext() */
#define DLT_SID_RegisterContext                                   0x05U

#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
#if (defined DLT_SID_UnregisterContext)
#error DLT_SID_UnregisterContext already defined
#endif
/** \brief Service Id for Dlt_UnregisterContext() */
#define DLT_SID_UnregisterContext                                 0x16U
#endif

#if (defined DLT_SID_InternalRegisterContext)
#error DLT_SID_InternalRegisterContext already defined
#endif
/** \brief Service Id for Dlt_InternalRegisterContext() */
#define DLT_SID_InternalRegisterContext                           0x86U

#if (defined DLT_SID_NvMSingleBlockCallbackNative)
#error DLT_SID_NvMSingleBlockCallbackNative already defined
#endif
/** \brief Defines API id of function
 ** Dlt_NvMSingleBlockCallbackNative() */
#define DLT_SID_NvMSingleBlockCallbackNative                      0x88U

#if (defined DLT_SID_InternalWriteToNativeApiId)
#error DLT_SID_InternalWriteToNativeApiId already defined
#endif
/** \brief Internal API Id for Dlt_NvMWriteRamBlockToNvMNativeCbk() */
#define DLT_SID_InternalWriteToNativeApiId                        0x81U

#if (defined DLT_SID_InternalWriteToDataSetApiId)
#error DLT_SID_InternalWriteToDataSetApiId already defined
#endif
/** \brief Internal API Id for Dlt_NvMWriteRamBlockToNvMDataSetCbk() */
#define DLT_SID_InternalWriteToDataSetApiId                       0x82U

#if (defined DLT_SID_InternalReadFromNativeApiId)
#error DLT_SID_InternalReadFromNativeApiId already defined
#endif
/** \brief Internal API Id for Dlt_NvMReadRamBlockFromNvMNativeCbk() */
#define DLT_SID_InternalReadFromNativeApiId                       0x83U

#if (defined DLT_SID_InternalReadFromDataSetApiId)
#error DLT_SID_InternalReadFromDataSetApiId already defined
#endif
/** \brief Internal API Id for Dlt_NvMReadRamBlockFromNvMDataSetCbk() */
#define DLT_SID_InternalReadFromDataSetApiId                      0x84U

#if (defined DLT_SID_SingleBlockCallbackDataSet)
#error DLT_SID_SingleBlockCallbackDataSet already defined
#endif
/** \brief Service Id for Dlt_NvMSingleBlockCallbackDataSet() */
#define DLT_SID_SingleBlockCallbackDataSet                        0x85U

#if (defined DLT_SID_ComCopyTxData)
#error DLT_SID_ComCopyTxData already defined
#endif
/** \brief Service Id for Dlt_ComCopyTxData() */
#define DLT_SID_ComCopyTxData                                     0x43U

#if (defined DLT_SID_TriggerTransmit)
#error DLT_SID_TriggerTransmit already defined
#endif
/** \brief Service Id for Dlt_TriggerTransmit() */
#define DLT_SID_TriggerTransmit                                   0x41U


#if (defined DLT_SID_ComRxIndication)
#error DLT_SID_ComRxIndication already defined
#endif
/** \brief Service Id for Dlt_ComRxIndication() */
#define DLT_SID_ComRxIndication                                   0x42U

#if (defined DLT_SID_ComCopyRxData)
#error DLT_SID_ComCopyRxData already defined
#endif
/** \brief Service Id for Dlt_ComCopyRxData() */
#define DLT_SID_ComCopyRxData                                     0x44U

#if (defined DLT_SID_ComStartOfReception)
#error DLT_SID_ComStartOfReception already defined
#endif
/** \brief Service Id for Dlt_ComStartOfReception() */
#define DLT_SID_ComStartOfReception                               0x46U

#if (DLT_IMPLEMENT_FILTER_MESSAGES == STD_ON)

#if (defined DLT_SID_SetLogLevel)
#error DLT_SID_SetLogLevel already defined
#endif
/** \brief Service Id for Dlt_SetLogLevel() */
#define DLT_SID_SetLogLevel                                       0x08U

#if (defined DLT_SID_GetLogLevel)
#error DLT_SID_GetLogLevel already defined
#endif
/** \brief Service Id for Dlt_GetLogLevel() */
#define DLT_SID_GetLogLevel                                       0xAAU

#if (defined DLT_SID_GetLogInfo)
#error DLT_SID_GetLogInfo already defined
#endif
/** \brief Service Id for Dlt_GetLogInfo() */
#define DLT_SID_GetLogInfo                                        0x0AU

#endif /* DLT_IMPLEMENT_FILTER_MESSAGES == STD_ON */

#if (defined DLT_SID_SetDefaultLogLevel)
#error DLT_SID_SetDefaultLogLevel already defined
#endif
/** \brief Service Id for Dlt_SetDefaultLogLevel() */
#define DLT_SID_SetDefaultLogLevel                                0x11U

#if (defined DLT_SID_SetLogChannelAssignment)
#error DLT_SID_SetLogChannelAssignment already defined
#endif
/** \brief Service Id for Dlt_SetLogChannelAssignment () */
#define DLT_SID_SetLogChannelAssignment                           0x20U

#if (defined DLT_SID_SetLogChannelThreshold)
#error DLT_SID_SetLogChannelThreshold already defined
#endif
/** \brief Service Id for Dlt_SetLogChannelThreshold () */
#define DLT_SID_SetLogChannelThreshold                            0x21U

#if (defined DLT_SID_GetLogChannelThreshold)
#error DLT_SID_GetLogChannelThreshold already defined
#endif
/** \brief Service Id for Dlt_GetLogChannelThreshold () */
#define DLT_SID_GetLogChannelThreshold                            0x22U

#if (defined DLT_SID_GetLogChannelNames)
#error DLT_SID_GetLogChannelNames already defined
#endif
/** \brief Service Id for Dlt_GetLogChannelNames () */
#define DLT_SID_GetLogChannelNames                                0x17U

#if (defined DLT_SID_GetDefaultLogLevel)
#error DLT_SID_GetDefaultLogLevel already defined
#endif
/** \brief Service Id for Dlt_ASR42/43_GetDefaultLogLevel() */
#define DLT_SID_GetDefaultLogLevel                                0x18U

#if (DLT_IMPLEMENT_FILTER_MESSAGES == STD_ON)
#if (defined DLT_SID_GetTraceStatus)
#error DLT_SID_GetTraceStatus already defined
#endif
/** \brief Service Id for Dlt_ASR42/43_GetTraceStatus() */
#define DLT_SID_GetTraceStatus                                    0x1FU

#if (defined DLT_SID_SetTraceStatus)
#error DLT_SID_SetTraceStatus already defined
#endif
/** \brief Service Id for Dlt_SetTraceStatus() */
#define DLT_SID_SetTraceStatus                                    0x09U
#endif /* #if (DLT_IMPLEMENT_FILTER_MESSAGES == STD_ON) */

#if (defined DLT_SID_SetDefaultTraceStatus)
#error DLT_SID_SetDefaultTraceStatus already defined
#endif
/** \brief Service Id for Dlt_ASR42/43_SetDefaultTraceStatus() */
#define DLT_SID_SetDefaultTraceStatus                             0x12U

#if (defined DLT_SID_GetDefaultTraceStatus)
#error DLT_SID_GetDefaultTraceStatus already defined
#endif
/** \brief Service Id for Dlt_ASR42/43_GetDefaultTraceStatus() */
#define DLT_SID_GetDefaultTraceStatus                             0x19U

#if (defined DLT_SID_SetMessageFiltering)
#error DLT_SID_SetMessageFiltering already defined
#endif
/** \brief Service Id for Dlt_ASR42/43_SetMessageFiltering() */
#define DLT_SID_SetMessageFiltering                                0x1BU

#if (defined DLT_SID_GetMessageFilteringStatus)
#error DLT_SID_GetMessageFilteringStatus already defined
#endif
/** \brief Service Id for Dlt_GetMessageFilteringStatus() */
#define DLT_SID_GetMessageFilteringStatus                          0xE6U

#if (defined DLT_SID_ResetToFactoryDefault)
#error DLT_SID_ResetToFactoryDefault already defined
#endif
/** \brief Service Id for Dlt_ResetToFactoryDefault() */
#define DLT_SID_ResetToFactoryDefault                              0x06U

#if (defined DLT_SID_SetUseECUID)
#error DLT_SID_SetUseECUID already defined
#endif
/** \brief Service Id for Dlt_SetUseECUID() */
#define DLT_SID_SetUseECUID                                        0xF3U

#if (defined DLT_SID_GetUseECUID)
#error DLT_SID_GetUseECUID already defined
#endif
/** \brief Service Id for Dlt_GetUseECUID() */
#define DLT_SID_GetUseECUID                                        0xE5U

#if (defined DLT_SID_SetUseSessionID)
#error DLT_SID_SetUseSessionID already defined
#endif
/** \brief Service Id for Dlt_SetUseSessionID() */
#define DLT_SID_SetUseSessionID                                    0xF2U

#if (defined DLT_SID_GetUseSessionID)
#error DLT_SID_GetUseSessionID already defined
#endif
/** \brief Service Id for Dlt_GetUseSessionID() */
#define DLT_SID_GetUseSessionID                                    0xE4U

#if (DLT_IMPLEMENT_TIMESTAMP != STD_OFF)
#if (defined DLT_SID_SetUseTimestamp)
#error DLT_SID_SetUseTimestamp already defined
#endif
/** \brief Service Id for Dlt_SetUseTimestamp() */
#define DLT_SID_SetUseTimestamp                                    0xF1U

#if (defined DLT_SID_GetUseTimestamp)
#error DLT_SID_GetUseTimestamp already defined
#endif
/** \brief Service Id for Dlt_GetUseTimestamp() */
#define DLT_SID_GetUseTimestamp                                    0xE3U
#endif /* DLT_IMPLEMENT_TIMESTAMP != STD_OFF */

#if (defined DLT_SID_SetUseExtendedHeader)
#error DLT_SID_SetUseExtendedHeader already defined
#endif
/** \brief Service Id for Dlt_SetUseExtendedHeader() */
#define DLT_SID_SetUseExtendedHeader                               0xF0U

#if (defined DLT_SID_GetUseExtendedHeader)
#error DLT_SID_GetUseExtendedHeader already defined
#endif
/** \brief Service Id for Dlt_GetUseExtendedHeader() */
#define DLT_SID_GetUseExtendedHeader                               0xE2U

#if (defined DLT_SID_SetGlobalLogging)
#error DLT_SID_SetGlobalLogging already defined
#endif
/** \brief Service Id for Dlt_SetGlobalLogging() */
#define DLT_SID_SetGlobalLogging                                   0xACU

#if (defined DLT_SID_GetGlobalLogging)
#error DLT_SID_GetGlobalLogging already defined
#endif
/** \brief Service Id for Dlt_GetGlobalLogging() */
#define DLT_SID_GetGlobalLogging                                   0xADU

#if (defined DLT_SID_SetVerboseMode)
#error DLT_SID_SetVerboseMode already defined
#endif
/** \brief Service Id for Dlt_SetVerboseMode() */
#define DLT_SID_SetVerboseMode                                     0x93U

#if (defined DLT_SID_GetVerboseModeStatus)
#error DLT_SID_GetVerboseModeStatus already defined
#endif
/** \brief Service Id for Dlt_GetVerboseModeStatus() */
#define DLT_SID_GetVerboseModeStatus                               0xE7U

#if (DLT_TRAFFIC_SHAPER_ENABLED == STD_ON)
#if (defined DLT_SID_SetComInterfaceMaxBandwidth)
#error DLT_SID_SetComInterfaceMaxBandwidth already defined
#endif
/** \brief Service Id for Dlt_SetComInterfaceMaxBandwidth() */
#define DLT_SID_SetComInterfaceMaxBandwidth                        0xF8U

#if (defined DLT_SID_GetComInterfaceMaxBandwidth)
#error DLT_SID_GetComInterfaceMaxBandwidth already defined
#endif
/** \brief Service Id for Dlt_GetComInterfaceMaxBandwidth() */
#define DLT_SID_GetComInterfaceMaxBandwidth                        0xE8U
#endif /* (DLT_TRAFFIC_SHAPER_ENABLED == STD_ON) */

#if (defined DLT_SID_StorePersistent)
#error DLT_SID_StorePersistent already defined
#endif
/** \brief Service Id for Dlt_StorePersistent() */
#define DLT_SID_StorePersistent                                    0x87U

#if (defined DLT_SID_StoreConfiguration)
#error DLT_SID_StoreConfiguration already defined
#endif
/** \brief Service Id for Dlt_StoreConfiguration() */
#define DLT_SID_StoreConfiguration                                 0x1AU

#if (defined DLT_SID_GetEcuIdAddress)
#error DLT_SID_GetEcuIdAddress already defined
#endif
/** \brief Service Id for Dlt_AppGetEcuIdAddress() */
#define DLT_SID_GetEcuIdAddress                                    0x89U

/*--[Error codes]-------------------------------------------------------------*/

/* !LINKSTO EB_Ref_SWS_Dlt_00447, 1 */
#if (defined DLT_E_WRONG_PARAMETERS)
#error DLT_E_WRONG_PARAMETERS already defined
#endif
/** \brief Development error for wrong parameters */
#define DLT_E_WRONG_PARAMETERS                                    0x01U

#if (defined DLT_E_ERROR_IN_PROV_SERVICE)
#error DLT_E_ERROR_IN_PROV_SERVICE already defined
#endif
/** \brief Development error for error in provided service */
#define DLT_E_ERROR_IN_PROV_SERVICE                               0x02U

#if (defined DLT_E_COM_FAILURE)
#error DLT_E_COM_FAILURE already defined
#endif
/** \brief Development error for error in communication module */
#define DLT_E_COM_FAILURE                                         0x03U

#if (defined DLT_E_ERROR_TO_MANY_CONTEXT)
#error DLT_E_ERROR_TO_MANY_CONTEXT already defined
#endif
/** \brief Development error for too many registered contexts */
#define DLT_E_ERROR_TO_MANY_CONTEXT                               0x04U

#if (defined DLT_E_MSG_LOOSE)
#error DLT_E_MSG_LOOSE already defined
#endif
/** \brief Development error for message buffer overflow */
#define DLT_E_MSG_LOOSE                                           0x05U

/* !LINKSTO Dlt.ASR431.Ref_SWS_Dlt_00727, 1 */
#if (defined DLT_E_PARAM_POINTER)
#error DLT_E_PARAM_POINTER already defined
#endif
/** \brief Development error for NULL pointer passed to an API */
#define DLT_E_PARAM_POINTER                                       0x06U

#if (defined DLT_E_NOT_INITIALIZED)
#error DLT_E_NOT_INITIALIZED already defined
#endif
/** \brief Development error for wrong initialization state */
#define DLT_E_NOT_INITIALIZED                                     0x08U

#if (defined DLT_E_NOT_INITIALIZED_PERSISTENT)
#error DLT_E_NOT_INITIALIZED_PERSISTENT already defined
#endif
/** \brief Development error for wrong initialization state */
#define DLT_E_NOT_INITIALIZED_PERSISTENT                          0x09U

#if (defined DLT_E_REQUEST_NOT_ACCEPTED)
#error DLT_E_REQUEST_NOT_ACCEPTED already defined
#endif
/** \brief Development error for read/write request unsuccessful */
#define DLT_E_REQUEST_NOT_ACCEPTED                                0x0AU

#if (defined DLT_E_CORE_SYNC_FAILED)
#error DLT_E_CORE_SYNC_FAILED already defined
#endif
/** \brief Development error for core synchronization */
#define DLT_E_CORE_SYNC_FAILED                                    0x0BU

/*==[Declaration of functions with external linkage]==========================*/

#define DLT_START_SEC_CODE
#include <Dlt_MemMap.h>

/* !LINKSTO SWS_Dlt_00239,1 */
/** \brief This service initializes the Dlt module
 **
 ** Dlt is using the NVRamManager and is to be initialized very late in the
 ** ECU startup phase. The Dlt_init() function should be called after the
 ** NVRamManager is initialized.
 **
 ** \param[in] config Pointer to a DLT configuration structure
 **
 ** \ServiceID{::DLT_SID_Init}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, DLT_CODE) Dlt_Init(
    P2CONST(Dlt_ConfigType, AUTOMATIC, DLT_APPL_CONST) config);

#if (DLT_VERSION_INFO_API == STD_ON)
/* !LINKSTO SWS_Dlt_00271,1 */
/** \brief This service returns the version information of this module.
 **
 ** The version information includes:
 ** - Module Id
 ** - Vendor Id
 ** - Vendor specific version numbers (BSW00407).
 **
 ** This function shall be pre-compile time configurable On/Off by the
 ** configuration parameter: DLT_VERSION_INFO_API
 **
 ** \param[out] versioninfo  Pointer to where to store the version information
 **                          of this module.
 **
 ** \ServiceID{::DLT_SID_GetVersionInfo}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, DLT_CODE) Dlt_GetVersionInfo(
    P2VAR(Std_VersionInfoType, AUTOMATIC, DLT_APPL_DATA) versioninfo);
#endif /* (DLT_VERSION_INFO_API == STD_ON) */

/** \brief Periodically trigger message transmission
 **
 ** This function periodically triggers the transmission of Dlt
 ** frames to the PduR. The function also performs bandwidth
 ** management tasks.
 **
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, DLT_CODE) Dlt_MainFunction(void);

#ifdef DLT_VFB_MAINFUNCTION_PERIOD
/** \brief Periodically trigger VFB message transmission
 **
 ** This function periodically triggers the transmission of VFB
 ** trace messages to the PduR.
 **
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, DLT_CODE) Dlt_VfbMainFunction(void);
#endif /* #ifdef DLT_VFB_MAINFUNCTION_PERIOD */

#define DLT_STOP_SEC_CODE
#include <Dlt_MemMap.h>

#endif /* ifndef DLT_H */
