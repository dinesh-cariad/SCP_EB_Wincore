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

#ifndef DLT_CONTROL_H
#define DLT_CONTROL_H

/* This file contains or includes the prototypes for the Dlt control C API */

/*==================[inclusions]============================================*/

#include <Dlt_Types.h>

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define DLT_START_SEC_CODE
#include <Dlt_MemMap.h>

#if (DLT_IMPLEMENT_FILTER_MESSAGES == STD_ON)

/** \brief Set log level
 **
 ** This service sets the maximum level a log message may have to be accepted
 ** by the Dlt module.
 **
 ** \param[in] AppId The application Id for which the log level is set
 ** \param[in] ContextId The context Id for which the log level is set
 ** \param[in] NewLevel The new log level for the given application and
 **                     context Ids
 **
 ** \return Result of changing the log level
 ** \retval DLT_CTRL_OK Changing the log level succeeded
 ** \retval DLT_CTRL_ERROR Changing the log level failed
 **
 ** \ServiceID{::DLT_SID_SetLogLevel}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_ASR42_SetLogLevel(
    Dlt_Internal_ApplicationIDType AppId,
    Dlt_Internal_ContextIDType ContextId,
    Dlt_MessageLogLevelType NewLevel);

/** \brief Set trace status
 **
 ** This service enables or disables the processing of trace messages by the
 ** Dlt module.
 **
 ** \param[in] AppId The application Id for which the trace status is set
 ** \param[in] ContextId The context Id for which the trace status is set
 ** \param[in] NewStatus The new trace status for the given application and
 **                      context Ids
 **
 ** \return Result of changing the trace status
 ** \retval DLT_CTRL_OK Changing the trace status succeeded
 ** \retval DLT_CTRL_ERROR Changing the trace status failed
 **
 ** \ServiceID{::DLT_SID_SetTraceStatus}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_ASR42_SetTraceStatus(
    Dlt_Internal_ApplicationIDType AppId,
    Dlt_Internal_ContextIDType ContextId,
    Dlt_MessageTraceStatusType NewStatus);

/** \brief Get trace status
 **
 ** This service returns the trace status setting for a given context
 **
 ** \param[in] AppId The application Id for which the trace status shall
 **                  be retrieved
 ** \param[in] ContextId The context Id for which the trace status shall
 **                      be retrieved
 ** \param[out] TraceStatus Trace status of the given application and
 **                         context Ids
 **
 ** \return Result of retrieving the trace status
 ** \retval DLT_CTRL_OK Retrieving the trace status succeeded
 ** \retval DLT_CTRL_ERROR Retrieving the trace status failed
 **
 ** \ServiceID{::DLT_SID_GetTraceStatus}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_ASR42_GetTraceStatus(
    Dlt_Internal_ApplicationIDType AppId,
    Dlt_Internal_ContextIDType ContextId,
    P2VAR(Dlt_MessageTraceStatusType, AUTOMATIC, DLT_APPL_DATA) TraceStatus);

/** \brief Get log level
 **
 ** This service returns the maximum level a log message may have to be
 ** accepted by the Dlt module.
 **
 ** \param[in] AppId The application Id for which the log level shall be retrieved
 ** \param[in] ContextId The context Id for which the log level shall be retrieved
 ** \param[out] LogLevel Log level setting for the given application and
 **                     context Ids
 **
 ** \return Result of retrieving the log level
 ** \retval DLT_CTRL_OK Retrieving the log level succeeded
 ** \retval DLT_CTRL_ERROR Retrieving the log level failed
 **
 ** \ServiceID{::DLT_SID_GetLogLevel}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
extern FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetLogLevel(
    P2CONST(Dlt_ApplicationIDType, AUTOMATIC, DLT_APPL_CONST) AppId,
    P2CONST(Dlt_ContextIDType, AUTOMATIC, DLT_APPL_CONST) ContextId,
    P2VAR(Dlt_MessageLogLevelType, AUTOMATIC, DLT_APPL_DATA) LogLevel);
#else /* #if (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431) */
extern FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetLogLevel(
    Dlt_ApplicationIDType AppId,
    Dlt_ContextIDType ContextId,
    P2VAR(Dlt_MessageLogLevelType, AUTOMATIC, DLT_APPL_DATA) LogLevel);
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */

#endif /* DLT_IMPLEMENT_FILTER_MESSAGES == STD_ON */

#if (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431)
/* !LINKSTO Dlt.ASR431.Ref_SWS_Dlt_00732, 1 */
/** \brief Get log info
 *
 * Called to request information about registered ApplicationIds,
 * their ContextIds and the corresponding log level.
 *
 * \param[in]  options   Used to filter the response in respect
 *                       to the ApplicationId, ContextId and Trace Status information
 * \param[in]  appId     Representation of the ApplicationId
 * \param[in]  contextId Representation of the ContextId
 *
 * \param[out] status
 * \param[out] logInfo   Details about the returned Application IDs
 *
 * \return Result of retrieving the log info
 *
 * \retval E_OK     No error occurred
 * \retval E_NOT_OK LogInfo could not be returned
 *
 * \ServiceID{::DLT_SID_GetLogInfo}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DLT_CODE) Dlt_ASR43_GetLogInfo(
    uint8 options,
    Dlt_ApplicationIDType appId,
    Dlt_ContextIDType contextId,
    P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) status,
    P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) logInfo);
#endif /* DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431 */

/** \brief Get log info internal
 *
 * Called to request information about registered ApplicationIds,
 * providing support to Dlt_GetLogInfo by extending the API
 * with a session ID parameter where available.
 *
 * \param[in]  SessionID   Used to distinguish identical tuples
 * \param[in]  options   Used to filter the response in respect
 *                       to the ApplicationId, ContextId and Trace Status information
 * \param[in]  AppID     Representation of the ApplicationId
 * \param[in]  ContextID Representation of the ContextId
 *
 * \param[out] status
 * \param[out] logInfo   Details about the returned Application IDs
 *
 * \return Result of retrieving the log info
 *
 * \retval E_OK     No error occurred
 * \retval E_NOT_OK LogInfo could not be returned
 *
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */

FUNC(Std_ReturnType, DLT_CODE) Dlt_GetLogInfoInternal(
    Dlt_SessionIDType SessionID,
    uint8 options,
    Dlt_Internal_ApplicationIDType AppID,
    Dlt_Internal_ContextIDType ContextID,
    P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) status,
    P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) logInfo);

/** \brief Set default log level
 **
 ** This service sets the default log level applied to each context for which
 ** the log level has not been set explicitly.
 **
 ** \param[in] NewLevel The new default log level
 **
 ** \return Result of changing the log level
 ** \retval DLT_CTRL_OK Changing the log level succeeded
 ** \retval DLT_CTRL_ERROR Changing the log level failed
 **
 ** \ServiceID{::DLT_SID_SetDefaultLogLevel}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_ASR42_SetDefaultLogLevel(Dlt_MessageLogLevelType NewLevel);

/** \brief Get default log level
 **
 ** This service gets the default log level applied to each context for which
 ** the log level has not been set explicitly.
 **
 ** \return Default log level setting
 **
 ** \ServiceID{::DLT_SID_GetDefaultLogLevel}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Dlt_MessageLogLevelType, DLT_CODE) Dlt_ASR42_GetDefaultLogLevel(void);

/** \brief Set default trace status
 **
 ** This service sets the default trace status applied to each context for
 ** which the trace status has not been set explicitly.
 **
 ** \param[in] NewStatus The new default trace status
 **
 ** \return Result of changing the trace status
 ** \retval DLT_CTRL_OK Changing the trace status succeeded
 ** \retval DLT_CTRL_ERROR Changing the trace status failed
 **
 ** \ServiceID{::DLT_SID_SetDefaultTraceStatus}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_ASR42_SetDefaultTraceStatus(Dlt_MessageTraceStatusType NewStatus);

/** \brief Get default trace status
 **
 ** This service gets the default trace status applied to each context
 ** for which the trace status has not been set explicitly.
 **
 ** \return Default trace status setting
 **
 ** \ServiceID{::DLT_SID_GetDefaultTraceStatus}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Dlt_MessageTraceStatusType, DLT_CODE) Dlt_ASR42_GetDefaultTraceStatus(void);

/** \brief Enable / disable message filtering
 **
 ** This function enables and disables message filtering. If message
 ** filtering is disabled, all messages are processed by the Dlt,
 ** independently from the log level or trace status settings.
 **
 ** \param[in] NewStatus New status of message filtering
 **                      DLT_FILTER_MESSAGES_ON: enable message filtering
 **                      DLT_FILTER_MESSAGES_OFF: disable message filtering
 **
 ** \return Result of changing the message filtering status
 ** \retval DLT_CTRL_OK Changing the message filtering status succeeded
 ** \retval DLT_CTRL_ERROR Changing the message filtering status failed
 **
 ** \ServiceID{::DLT_SID_SetMessageFiltering}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_ASR42_SetMessageFiltering(Dlt_FilterMessagesType NewStatus);

/** \brief Get message filtering status
 **
 ** This function retrieves the current message filtering status.
 **
 ** \return Current message filtering status
 ** \retval DLT_FILTER_MESSAGES_ON Message filtering is enabled
 ** \retval DLT_FILTER_MESSAGES_OFF Message filtering is disabled
 **
 ** \ServiceID{::DLT_SID_GetMessageFilteringStatus}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Dlt_FilterMessagesType, DLT_CODE) Dlt_GetMessageFilteringStatus(void);

/** \brief Enable / disable the transmission of the EcuID in message header
 **
 ** This function enables and disables the inclusion of the EcuID
 ** in the Dlt message header (the ECUID is attached in the Standard Header).
 **
 ** \param[in] NewStatus New status of including the EcuID
 **                      (1: include the EcuID (ON),
 **                       0: do not include the EcuID (OFF))
 **
 ** \return Result of changing the status
 ** \retval DLT_CTRL_OK Changing the status succeeded.
 ** \retval DLT_CTRL_ERROR Changing the status failed.
 **
 ** \ServiceID{::DLT_SID_SetUseECUID}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetUseECUID(uint8 NewStatus);

/** \brief Get EcuID header field status
 **
 ** This function returns whether the EcuID is included in the Dlt
 ** message header (Used to get status of DltHeaderUseEcuId).
 **
 ** \return Current EcuID header field status
 ** \retval 1 EcuID is included in the Dlt message header
 ** \retval 0 EcuID is not included in the Dlt message header
 ** \retval 2 Call to the API to retrieve EcuID usage failed
 **
 ** \ServiceID{::DLT_SID_GetUseECUID}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetUseECUID(void);

/** \brief Switch to Verbose Mode
 **
 ** This function switches verbose mode
 **
 ** \param[in] AppId The application Id for which the verbose mode is set
 ** \param[in] ContextId The context Id for which the verbose mode is set
 ** \param[in] NewStatus New status of verbose mode
 **                      (1: enable verbose mode (ON),
 **                       0: disable verbose mode (OFF))
 **
 ** \return Result of changing the status
 ** \retval DLT_CTRL_OK        Changing the status succeeded
 ** \retval DLT_NOT_SUPPORTED  Verbose mode is not implemented
 ** \retval DLT_CTRL_ERROR     Changing the status failed
 **
 ** \ServiceID{::DLT_SID_SetVerboseMode}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
extern FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetVerboseMode(
    P2CONST(Dlt_ApplicationIDType, AUTOMATIC, DLT_APPL_CONST) AppId,
    P2CONST(Dlt_ContextIDType, AUTOMATIC, DLT_APPL_CONST) ContextId,
    Dlt_MessageLogLevelType NewStatus);
#else /* #if (DLT_DEFAULT_ASR_RETVAL 1= AUTOSAR_431) */
extern FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetVerboseMode(
    Dlt_ApplicationIDType AppId,
    Dlt_ContextIDType ContextId,
    Dlt_MessageLogLevelType NewStatus);
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */
/** \brief Get verbose mode status
 **
 ** This function returns whether the verbose mode is ON or OFF for a given
 ** application and context Id pair
 **
 ** \param[in] AppId The application Id for which the verbose mode is set
 ** \param[in] ContextId The context Id for which the verbose mode is set
 ** \param[out] ModeStatus Mode setting for the given application and
 **                        context Ids
 **
 ** \return Result of retrieving the mode
 ** \retval DLT_CTRL_OK         Retrieving the mode succeeded
 ** \retval DLT_CTRL_ERROR      Retrieving the mode failed
 **
 ** \ServiceID{::DLT_SID_GetVerboseModeStatus}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
extern FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetVerboseModeStatus(
    P2CONST(Dlt_ApplicationIDType, AUTOMATIC, DLT_APPL_CONST) AppId,
    P2CONST(Dlt_ContextIDType, AUTOMATIC, DLT_APPL_CONST) ContextId,
    P2VAR(Dlt_MessageLogLevelType, AUTOMATIC, DLT_APPL_DATA) ModeStatus);
#else /* #if (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431) */
extern FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetVerboseModeStatus(
    Dlt_ApplicationIDType AppId,
    Dlt_ContextIDType ContextId,
    P2VAR(Dlt_MessageLogLevelType, AUTOMATIC, DLT_APPL_DATA) ModeStatus);
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */
/** \brief Enable / disable SessionID in message header
 **
 ** This function enables and disables the inclusion of the SessionID
 ** in the Dlt message header.
 **
 ** \param[in] NewStatus New status of including the SessionID
 **                      (1: include the SessionID (ON),
 **                       0: do not include the SessionID (OFF))
 **
 ** \return Result of changing the status
 ** \retval DLT_CTRL_OK    Changing the status succeeded
 ** \retval DLT_CTRL_ERROR Changing the status failed
 **
 ** \ServiceID{::DLT_SID_SetUseSessionID}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetUseSessionID(uint8 NewStatus);

/** \brief Get SessionID header field status
 **
 ** This function returns whether the SessionID is included in the Dlt
 ** message header.
 **
 ** \return Current SessionID header field status
 ** \retval 1 SessionID is included in the Dlt message header
 ** \retval 0 SessionID is not included in the Dlt message header
 ** \retval 2 Call to the API to retrieve SessionID usage failed
 **
 ** \ServiceID{::DLT_SID_GetUseSessionID}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetUseSessionID(void);

#if (DLT_IMPLEMENT_TIMESTAMP != STD_OFF)
/** \brief Enable / disable timestamps in message header
 **
 ** This function enables and disables the inclusion of timestamps
 ** in the Dlt message header.
 **
 ** \param[in] NewStatus New status of including timestamps
 **                      (1: include timestamps (ON),
 **                       0: do not include timestamps (OFF))
 **
 ** \return Result of changing the status
 ** \retval DLT_CTRL_OK    Changing the status succeeded
 ** \retval DLT_CTRL_ERROR Changing the status failed
 **
 ** \ServiceID{::DLT_SID_SetUseTimestamp}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetUseTimestamp(uint8 NewStatus);

/** \brief Get timestamp header field status
 **
 ** This function returns whether timestamps are included in the Dlt
 ** message header.
 **
 ** \return Current timestamp header field status
 ** \retval 1 timestamps are included in the Dlt message header
 ** \retval 0 timestamps are not included in the Dlt message header
 ** \retval 2 Call failed to retrieve timestamp usage
 **
 ** \ServiceID{::DLT_SID_GetUseTimestamp}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetUseTimestamp(void);
#endif /* DLT_IMPLEMENT_TIMESTAMP != STD_OFF */

#if (DLT_IMPLEMENT_EXTENDED_HEADER == STD_ON)

/** \brief Enable / disable use of an extended message header
 **
 ** This function enables and disables the use of an extended Dlt
 ** message header.
 **
 ** \param[in] NewStatus New status of using extended header
 **                      (1: use extended header (ON),
 **                       0: use standard header (OFF))
  **
 ** \return Result of changing the status
 ** \retval DLT_CTRL_OK Changing the status succeeded
 ** \retval DLT_CTRL_ERROR Changing the status failed
 **
 ** \ServiceID{::DLT_SID_SetUseExtendedHeader}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetUseExtendedHeader(uint8 NewStatus);

/** \brief Get extended header use status
 **
 ** This function returns whether extended Dlt message headers are
 ** used.
 **
 ** \return Current Dlt header format status
 ** \retval 1 Extended Dlt message headers are used
 ** \retval 0 Standard Dlt message headers are used
 ** \retval 2 Call to API failed to retrieve status of extended header usage.
 **
 ** \ServiceID{::DLT_SID_GetUseExtendedHeader}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetUseExtendedHeader(void);
#endif /* DLT_IMPLEMENT_EXTENDED_HEADER == STD_ON */

/** \brief Globally enable / disable message logging
 **
 ** This function enables and disables message logging. If message
 ** logging is disabled all messages are discarded by the Dlt,
 ** independently from the log level or trace status settings.
 **
 ** \param[in] logStatus New status of message logging
 **                      (DLT_LOGGING_ENABLED: enable message logging,
 **                       DLT_LOGGING_DISABLED: disable message logging)
 **
 ** \return Result of changing the message logging status
 ** \retval DLT_CTRL_OK    Changing the message logging status succeeded
 ** \retval DLT_CTRL_ERROR Changing the message logging status failed
 **
 ** \ServiceID{::DLT_SID_SetGlobalLogging}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetGlobalLogging(Dlt_GlobalLogStatusType logStatus);

/** \brief Get global message logging status
 **
 ** This function retrieves the current message logging status.
 **
 ** \return Current message logging status
 ** \retval DLT_LOGGING_ENABLED  Message logging is enabled
 ** \retval DLT_LOGGING_DISABLED Message logging is disabled
 **
 ** \ServiceID{::DLT_SID_GetGlobalLogging}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Dlt_GlobalLogStatusType, DLT_CODE) Dlt_GetGlobalLogging(void);

#if (DLT_TRAFFIC_SHAPER_ENABLED == STD_ON)
/** \brief Set the maximum bandwidth for the communication interface
 **
 ** This function sets the maximum allowed bandwidth available for the
 ** Dlt on the communication interface. If this bandwidth is exceeded,
 ** the Dlt will stop transmitting messages until the bandwidth is
 ** balanced again.
 **
 ** \param[in] max_bandwidth The maximum bandwidth in bits / second
 **                          This must be a multiple of 8.
 **
 ** \return Result of changing the maximum bandwidth
 ** \retval DLT_CTRL_OK    Changing the maximum bandwidth succeeded
 ** \retval DLT_CTRL_ERROR Changing the maximum bandwidth failed
 **
 ** \ServiceID{::DLT_SID_SetComInterfaceMaxBandwidth}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetComInterfaceMaxBandwidth(
  uint32 max_bandwidth);

/** \brief Get maximum bandwidth for the communication interface
 **
 ** This function returns the maximum bandwidth available for the Dlt
 ** on the communication interface. If the Dlt module is not initialized,
 ** no messages will be sent (only stored in the internal buffer), thus
 ** this function will return 0 in this case.
 **
 ** \return The current maximum bandwidth in bits / second
 **
 ** \ServiceID{::DLT_SID_GetComInterfaceMaxBandwidth}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(uint32, DLT_CODE) Dlt_GetComInterfaceMaxBandwidth(void);
#endif /* (DLT_TRAFFIC_SHAPER_ENABLED == STD_ON) */

#define DLT_STOP_SEC_CODE
#include <Dlt_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef DLT_CONTROL_H */
/*==================[end of file]===========================================*/
