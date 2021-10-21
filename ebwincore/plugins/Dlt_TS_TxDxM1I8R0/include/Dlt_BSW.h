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
#ifndef DLT_BSW_H
#define DLT_BSW_H

/* !LINKSTO Dlt.BSW.Usage.API_Declaration, 1 */

/*==[Includes]================================================================*/

#include <Dlt_BSW_Types.h>

/*==[Declaration of functions with external linkage]==========================*/

#define DLT_START_SEC_CODE
#include <Dlt_MemMap.h>

/** \brief Send a log message to the DLT module
 **
 ** The service represents the interface to be used by basic software
 ** modules or by software component to send log messages.
 **
 ** \param[in] session_id For SW-C this is not visible (Port defined
 **     argument value), for BSW-modules it is the module number
 ** \param[in] log_info Structure containing the relevant information
 **     for filtering the message
 ** \param[in] log_data Buffer containing the parameters to be logged.
 **     The contents of this pointer represents the payload of the
 **     send log message
 ** \param[in] log_data_length Length of the data buffer log_data
 **
 ** \return Result of sending the log message
 ** \retval DLT_E_OK The required operation succeeded
 ** \retval DLT_E_MSG_TOO_LARGE The message is too large for
 **     sending over the network
 ** \retval DLT_E_IF_NOT_AVAILABLE The interface is not
 **     available
 ** \retval DLT_E_UNKNOWN_SESSION_ID The provided session id
 **     is unknown
 ** \retval DLT_E_NOT_IN_VERBOSE_MODE Unable to send the message
 **     in verbose mode
 **
 ** ServiceID{::DLT_SID_SendLogMessage}
 ** Reentrancy{Reentrant}
 ** Synchronicity{Synchronous}
 ** Possible development errors:
 **  - DLT_E_PARAM_POINTER - in case a null pointer was used
 **                          for log_info or log_data input
 **  - DLT_E_WRONG_PARAMETERS - a wrong parameter is used in any case
 **  - DLT_E_UNKNOWN_SESSION_ID - session_id is used with a wrong/not registered value
 **  - DLT_E_NO_BUFFER - log_data_length value is greater than configured maximum length
 **                       for log messages
 */
extern FUNC(Dlt_ReturnType, DLT_CODE) Dlt_ASR43_SendLogMessage(
    Dlt_SessionIDType session_id,
    P2CONST(Dlt_MessageLogInfoType, AUTOMATIC, DLT_APPL_CONST) log_info,
    P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) log_data,
    uint16 log_data_length);

/** \brief Send a trace message to the DLT module
 **
 ** The service represents the interface to be used by basic software
 ** modules or by software component to trace parameters.
 **
 ** \param[in] session_id number of the module (Module ID within BSW,
 **     port defined argument value within SW-C)
 ** \param[in] trace_info Structure containing the relevant information
 **     for filtering the message
 ** \param[in] trace_data Buffer containing the parameters to be traced.
 **     The contents of this pointer represents the payload of the
 **     send log message
 ** \param[in] trace_data_length Length of the data buffer trace_data
 **
 ** \return Result of sending the trace message
 ** \retval DLT_E_OK The required operation succeeded
 ** \retval DLT_E_MSG_TOO_LARGE The message is too large for
 **     sending over the network
 ** \retval DLT_E_IF_NOT_AVAILABLE The interface is not
 **     available
 ** \retval DLT_E_UNKNOWN_SESSION_ID The provided session id
 **     is unknown
 ** \retval DLT_E_NOT_IN_VERBOSE_MODE Unable to send the message
 **     in verbose mode
 **
 ** ServiceID{::DLT_SID_SendTraceMessage}
 ** Reentrancy{Reentrant}
 ** Synchronicity{Synchronous}
 ** Possible development errors:
 **  - DLT_E_PARAM_POINTER - in case a null pointer was used
 **                          for log_info or log_data input
 **  - DLT_E_WRONG_PARAMETERS - a wrong parameter is used in any case
 **  - DLT_E_UNKNOWN_SESSION_ID - session_id is used with a wrong/not registered value
 **  - DLT_E_NO_BUFFER - log_data_length value is greater than configured maximum length
 **                       for log messages
 */
extern FUNC(Dlt_ReturnType, DLT_CODE) Dlt_ASR43_SendTraceMessage(
    Dlt_SessionIDType session_id,
    P2CONST(Dlt_MessageTraceInfoType, AUTOMATIC, DLT_APPL_CONST) trace_info,
    P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) trace_data,
    uint16 trace_data_length);

/** \brief Register new logging or tracing context
 **
 ** The service has to be called when a software module wants to use
 ** services offered by DLT software component for a specific context.
 ** If a context id is being registered for an already registered
 ** application id then app_description can be NULL and
 ** len_app_description zero.
 **
 ** \param[in] session_id number of the module (Module ID within BSW,
 **     port defined argument value within SW-C)
 ** \param[in] app_id the Application ID
 ** \param[in] context_id the Context ID
 ** \param[in] app_description Points to description string for the
 **     provided application id. At maximum 255 characters are
 **     interpreted.
 ** \param[in] len_app_description The length of the description for
 **     the application id string (number of characters of description
 **     string).
 ** \param[in] context_description Points to description string for the
 **     provided context id. At maximum 255 characters are
 **     interpreted.
 ** \param[in] len_context_description The length of the description
 **     string (number of characters of description string).
 **
 ** \return Result of registering the context
 ** \retval Autosar_4.2 DLT_E_IF_NOT_AVAILABLE The module has not been initialized,
 **         thus the interface is not available
 ** \retval Autosar_4.2 DLT_E_ERROR_UNKNOWN Too many contexts have been registered
 ** \retval Autosar_4.3 or below version DLT_E_CONTEXT_ALREADY_REG The software module
 **         context has already been registered
 ** \retval Autosar_4.3 or below version DLT_E_UNKNOWN_SESSION_ID The session Id is unknown, i.e. it
 **         is not specified by any SW-C
 **         In Autosar_4.3 version, each time a registration of a context
 **         was not succeeded because of Dlt not being initialized or
 **         because of a overrun number of registered contexts,
 **         or of a wrong parameter used, the DLT_E_UNKNOWN_SESSION_ID shall be
 **         returned.
 ** \retval DLT_E_OK The required operation succeeded
 **
 ** ServiceID{::DLT_SID_RegisterContext}
 ** Reentrancy{Reentrant}
 ** Synchronicity{Synchronous}
 ** Possible development errors
 ** DLT_E_ERROR_TO_MANY_CONTEXT - the number of contexts to be registered
 **                                has reached the maximum value
 ** DLT_E_NOT_INITIALIZED - Api was called before initializing Dlt module
 ** DLT_E_PARAM_POINTER  - null pointer was used for context_description
 ** DLT_E_WRONG_PARAMETERS -wrong len_context_description was used
 ** DLT_E_UNKNOWN_SESSION_ID - wrong session id was used
 */
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
extern FUNC(Dlt_ReturnType, DLT_CODE) Dlt_ASR43_RegisterContext(
    Dlt_SessionIDType session_id,
    P2CONST(Dlt_ApplicationIDType, AUTOMATIC, DLT_APPL_CONST) app_id,
    P2CONST(Dlt_ContextIDType, AUTOMATIC, DLT_APPL_CONST) context_id,
    P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) app_description,
    uint8 len_app_description,
    P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) context_description,
    uint8 len_context_description);
#else /* #if (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431) */
extern FUNC(Dlt_ReturnType, DLT_CODE) Dlt_ASR43_RegisterContext(
    Dlt_SessionIDType session_id,
    Dlt_ApplicationIDType app_id,
    Dlt_ContextIDType context_id,
    P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) app_description,
    uint8 len_app_description,
    P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) context_description,
    uint8 len_context_description);
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)

/** \brief Unregister already logged application/context ids
 **
 ** The service has to be called when a software module wants to use
 ** services offered by DLT software component for a specific context.
 **
 ** \param[in] sessionId number of the module (Module ID within BSW,
 **     port defined argument value within SW-C)
 ** \param[in] appId the Application ID
 ** \param[in] contextId the Context ID
 ** \return Result of unregistering the context
 ** \retval DLT_E_CONTEXT_NOT_YET_REG: The software module context has not registered before.
 ** \retval DLT_E_UNKNOWN_SESSION_ID: The provided session id is unknown.
 ** \retval DLT_E_OK The required operation succeeded
 **
 ** ServiceID{::DLT_SID_UnregisterContext}
 ** Reentrancy{Reentrant}
 ** Synchronicity{Synchronous}
 **
 ** Possible development errors
 ** DLT_E_NOT_INITIALIZED - Api was called before initializing Dlt module
 ** DLT_E_CONTEXT_NOT_YET_REG - context was not registered yet
 ** DLT_E_UNKNOWN_SESSION_ID - wrong session id was used
 */
extern FUNC(Dlt_ReturnType, DLT_CODE) Dlt_ASR43_UnregisterContext(
    Dlt_SessionIDType sessionId,
    P2CONST(Dlt_ApplicationIDType, AUTOMATIC, DLT_APPL_CONST) appId,
    P2CONST(Dlt_ContextIDType, AUTOMATIC, DLT_APPL_CONST) contextId);

/** \brief Get Log Channel Threshold
 **
 ** Returns the filter threshold for the given LogChannel.
 **
 ** \param[in]  logChannelName Addressed LogChannel name
 ** \param[out] logChannelThreshold current LogChannelTreshold
 ** \param[out] traceStatus Current TraceStatus. TRUE: TraceMessages enabled. FALSE: TraceMessages disabled.
 **
 ** \return Result of changing the log level
 ** \retval DLT_CTRL_OK Changing the log level succeeded
 ** \retval DLT_CTRL_ERROR Changing the log level failed
 **
 ** ServiceID{::DLT_SID_GetLogChannelThreshold}
 ** Reentrancy{Reentrant for different LogChannelNames}
 ** Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DLT_CODE) Dlt_ASR43_GetLogChannelThreshold(
       P2CONST(Dlt_LogChannelNameType, AUTOMATIC, DLT_APPL_CONST) logChannelName,
       P2VAR(Dlt_MessageLogLevelType, AUTOMATIC, DLT_APPL_DATA) logChannelThreshold,
       P2VAR(boolean, AUTOMATIC, DLT_APPL_DATA) traceStatus);
/** \brief Set log Channel Threshold
 **
 ** Sets the filter threshold for the given LogChannel.
 **
 ** \param[in] logChannelName Name of the addressed LogChannel
 ** \param[in] newThreshold Threshold for LogMessages
 ** \param[in] newTraceStatus TRUE: enable TraceMessages FALSE: disable TraceMessages
 **
 ** \return Result of changing the log level
 ** \retval DLT_CTRL_OK Changing the log level succeeded
 ** \retval DLT_CTRL_ERROR Changing the log level failed
 **
 ** ServiceID{::DLT_SID_SetLogChannelThreshold}
 ** Reentrancy{Reentrant for different LogChannelNames}
 ** Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DLT_CODE) Dlt_ASR43_SetLogChannelThreshold(
        P2CONST(Dlt_LogChannelNameType, AUTOMATIC, DLT_APPL_CONST) logChannelName,
        Dlt_MessageLogLevelType newThreshold,
        boolean newTraceStatus);

/** \brief Get Log Channel Names
 **
 ** Returns the filter threshold for the given LogChannel.
 **
 ** \param[out] numberOfLogChannels Returns the number of configured LogChannels
 ** \param[out] logChannelNames Returns a list of configured LogChannel names
 **
 ** \return Result of changing the log level
 ** \retval DLT_CTRL_OK Changing the log level succeeded
 ** \retval DLT_CTRL_ERROR Changing the log level failed
 **
 ** ServiceID{::DLT_SID_GetLogChannelNames}
 ** Reentrancy{Non Reentrant}
 ** Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DLT_CODE) Dlt_ASR43_GetLogChannelNames(
       P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) numberOfLogChannels,
       P2VAR(Dlt_LogChannelNameArrayType, AUTOMATIC, DLT_APPL_DATA) logChannelNames);

/** \brief Set log channel assignment
 **
 ** Adds/removes the addressed tuple ApplicationId/ContextId to/from the addressed LogChannel.
 ** \param[in] appId ID of the addressed application / SW-C
 ** \param[in] contextId The context Id for which the log level is set
 ** \param[in] logChannelName Name of the addressed LogChannel
 ** \param[in] addRemoveOp Operation to add/remove the addressed
 **            tuple ApplicationId/ContextId to/from the addressed LogChannel
 **
 ** \return Result of changing the log level
 ** \retval DLT_CTRL_OK Changing the log level succeeded
 ** \retval DLT_CTRL_ERROR Changing the log level failed
 **
 ** ServiceID{::DLT_SID_SetLogChannelAssignment}
 ** Reentrancy{Non Reentrant}
 ** Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DLT_CODE) Dlt_ASR43_SetLogChannelAssignment(
            P2CONST(Dlt_ApplicationIDType, AUTOMATIC, DLT_APPL_CONST) appId,
            P2CONST(Dlt_ContextIDType, AUTOMATIC, DLT_APPL_CONST) contextId,
            P2CONST(Dlt_LogChannelNameType, AUTOMATIC, DLT_APPL_CONST) logChannelName,
            Dlt_AssignmentOperationType addRemoveOp);

/* !LINKSTO Dlt.ASR431.SWS_Dlt_00252, 1 */
/** \brief Set log level
 **
 ** This service is used to change the LogLevel for the given tuple of ApplicationID/ContextID.
 **
 ** \param[in] appId       ID of the SW-C
 ** \param[in] contextId   ID of the context
 ** \param[in] newLogLevel new log level to set
 **
 ** \return Result of changing the log level
 **
 ** \retval E_OK: No error occurred
 ** \retval E_NOT_OK: LogLevel could not be changed
 **
 ** ServiceID{::DLT_SID_SetLogLevel}
 ** Reentrancy{Reentrant}
 ** Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DLT_CODE) Dlt_ASR43_SetLogLevel(
    P2CONST(Dlt_ApplicationIDType, AUTOMATIC, DLT_APPL_CONST) appId,
    P2CONST(Dlt_ContextIDType, AUTOMATIC, DLT_APPL_CONST) contextId,
    Dlt_MessageLogLevelType newLogLevel);

/* !LINKSTO Dlt.ASR431.SWS_Dlt_00254, 1 */
/** \brief Set trace status
 **
 ** The service Dlt_SetTraceStatus sets the trace status
 ** for a specific tuple of ApplicationID and ContextID.
 **
 ** \param[in] appId ID of the SW-C
 ** \param[in] contextId ID of the context
 ** \param[in] newTraceStatus New trace status
 **
 ** \return Result of changing the trace status
 **
 ** \retval E_OK: No error occurred
 ** \retval E_NOT_OK: Trace status could not be changed
 **
 ** ServiceID{::DLT_SID_SetTraceStatus}
 ** Reentrancy{Non Reentrant}
 ** Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DLT_CODE) Dlt_ASR43_SetTraceStatus(
    P2CONST(Dlt_ApplicationIDType, AUTOMATIC, DLT_APPL_CONST) appId,
    P2CONST(Dlt_ContextIDType, AUTOMATIC, DLT_APPL_CONST) contextId,
    boolean newTraceStatus);

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
 * ServiceID{::DLT_SID_GetLogInfo}
 * Reentrancy{Non Reentrant}
 * Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DLT_CODE) Dlt_ASR43_GetLogInfo(
    uint8 options,
    P2CONST(Dlt_ApplicationIDType, AUTOMATIC, DLT_APPL_CONST) appId,
    P2CONST(Dlt_ContextIDType, AUTOMATIC, DLT_APPL_CONST) contextId,
    P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) status,
    P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) logInfo);

/* !LINKSTO Dlt.ASR431.SWS_Dlt_00736, 1 */
/** \brief Store the general configuration, log level
 ** and trace status set for a pair of Application ID
 ** and Context ID in NvM blocks.
 **
 ** Copies the current Dlt configuration to NvRAM by calling NvM_WriteBlock().
 **
 ** \return Result of configuration storage
 **
 ** \retval E_OK: No error occurred
 ** \retval E_NOT_OK: The configuration could not be stored
 ** \retval DLT_E_NOT_SUPPORTED: Service is not supported
 **
 ** ServiceID{::DLT_SID_StoreConfiguration}
 ** Reentrancy{Non Reentrant}
 ** Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DLT_CODE) Dlt_ASR43_StoreConfiguration(void);

/* !LINKSTO Dlt.ASR431.SWS_Dlt_00750, 1 */
/** \brief Get trace status
 **
 ** Returns the current Trace Status for a given tuple ApplicationId/ContextId.
 **
 ** \param[in]  appId       ApplicationId
 ** \param[in]  contextId   ContextId
 ** \param[out] traceStatus current Trace Status of the tuple ApplicationId/ContextId
 **
 ** \return Result of retrieving the trace status
 **
 ** \retval E_OK: No error occurred
 ** \retval E_NOT_OK: TraceStatus could not be returned
 **
 ** ServiceID{::DLT_SID_GetTraceStatus}
 ** Reentrancy{Non Reentrant}
 ** Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DLT_CODE) Dlt_ASR43_GetTraceStatus(
    P2CONST(Dlt_ApplicationIDType, AUTOMATIC, DLT_APPL_CONST) appId,
    P2CONST(Dlt_ContextIDType, AUTOMATIC, DLT_APPL_CONST) contextId,
    P2VAR(boolean, AUTOMATIC, DLT_APPL_DATA) traceStatus);

/* !LINKSTO Dlt.ASR431.SWS_Dlt_00740, 1 */
/** \brief Set default log level
 **
 ** Called to modify the pass through range for Log Messages for all not explicit set ContextIds.
 **
 ** \param[in] newLogLevel sets the new filter value
 **
 ** \return Result of changing the log level
 **
 ** \retval E_OK: No error occurred
 ** \retval E_NOT_OK: Default LogLevel could not be set
 **
 ** ServiceID{::DLT_SID_SetDefaultLogLevel}
 ** Reentrancy{Reentrant}
 ** Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DLT_CODE) Dlt_ASR43_SetDefaultLogLevel(Dlt_MessageLogLevelType newLogLevel);

/* !LINKSTO Dlt.ASR431.SWS_Dlt_00733, 1 */
/** \brief Get default log level
 **
 ** Returns the Default Log Level currently used by the Dlt module.
 ** The returned Log Level might differ from the one which is stored non volatile.
 **
 ** \param[out] defaultLogLevel Returns the stored LogLevel setting
 **
 ** \return Default log level setting
 **
 ** \retval E_OK: No error occurred
 ** \retval E_NOT_OK: The default LogLevel could not be returned
 **
 ** ServiceID{::DLT_SID_GetDefaultLogLevel}
 ** Reentrancy{Non Reentrant}
 ** Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DLT_CODE) Dlt_ASR43_GetDefaultLogLevel(
    P2VAR(Dlt_MessageLogLevelType, AUTOMATIC, DLT_APPL_DATA) defaultLogLevel);

/* !LINKSTO Dlt.ASR431.Ref_SWS_Dlt_00743, 1 */
/** \brief Set default trace status
 **
 ** Called to enable or disable trace messages for all not explicitly set ContextIds.
 **
 ** \param[in] newTraceStatus enabling/disabling of Trace messages
 **
 ** \return Result of changing the trace status
 **
 ** \retval E_OK: No error occurred
 ** \retval E_NOT_OK: Default Trace Status could not be set
 **
 ** ServiceID{::DLT_SID_SetDefaultTraceStatus}
 ** Reentrancy{Reentrant}
 ** Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DLT_CODE) Dlt_ASR43_SetDefaultTraceStatus(boolean newTraceStatus);

/* !LINKSTO Dlt.ASR431.Ref_SWS_Dlt_00746, 1 */
/** \brief Get default trace status
 **
 ** Returns the current global default trace status.
 **
 ** \param[out] traceStatus current global default trace status (enabled/disabled)
 **
 ** \return Default trace status setting
 **
 ** \retval E_OK: No error occurred
 ** \retval E_NOT_OK: Default Trace Status could not be returned
 **
 ** ServiceID{::DLT_SID_GetDefaultTraceStatus}
 ** Reentrancy{Reentrant}
 ** Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DLT_CODE) Dlt_ASR43_GetDefaultTraceStatus(
    P2VAR(boolean, AUTOMATIC, DLT_APPL_DATA) traceStatus);

/* !LINKSTO Dlt.ASR431.SWS_Dlt_00770, 1 */
/** \brief Enable / disable message filtering
 **
 ** Switches on/off the message filtering functionality of the Dlt module.
 **
 ** \param[in] status TRUE: enable message filtering
 **                   FALSE: disable message filtering
 **
 ** \return Result of changing the message filtering status
 **
 ** \retval E_OK: No error occurred
 ** \retval E_NOT_OK: Setting of message filtering failed
 **
 ** ServiceID{::DLT_SID_SetMessageFiltering}
 ** Reentrancy{Non Reentrant}
 ** Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DLT_CODE) Dlt_ASR43_SetMessageFiltering(boolean status);

/* !LINKSTO Dlt.ASR431.SWS_Dlt_00739, 1 */
/** \brief Reset the configuration to factory defaults
 **
 ** The service Dlt_ResetToFactoryDefault sets the LogLevel and TraceStatus back
 ** to the persistently stored default values. If the feature NvMRAM support is enabled,
 ** all stored Dlt values in the NvM are deleted.
 **
 ** \return Result of resetting the configuration to factory defaults
 **
 ** \retval E_OK: Configuration has been reset successfully
 ** \retval E_NOT_OK: Configuration has not been reset
 **
 ** ServiceID{::DLT_SID_ResetToFactoryDefault}
 ** Reentrancy{Non Reentrant}
 ** Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DLT_CODE) Dlt_ASR43_ResetToFactoryDefault(void);

#else /* DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431 */

#if (DLT_IMPLEMENT_NV_RAM_STORAGE == STD_ON)

/** \brief Store persistent general configuration and log level
 ** and trace status explicitly set for a pair of Application ID
 ** and Context ID
 **
 ** The service has to be called when a software module wants to use
 ** persistent storage functionality offered by DLT software component
 ** for a specific set of data.
 **
 ** \param[in] session_id number of the module (Module ID within BSW,
 **     port defined argument value within SW-C)
 **
 ** \return Result of persistent storage
 ** \retval DLT_E_ERROR_UNKNOWN The required operation failed.
 ** \retval DLT_E_OK The required operation succeeded.
 **
 ** ServiceID{::DLT_SID_StorePersistent}
 ** Reentrancy{Non Reentrant}
 ** Synchronicity{Synchronous}
 */
extern FUNC(Dlt_ReturnType, DLT_CODE) Dlt_StorePersistent(Dlt_SessionIDType session_id);

#endif /* DLT_IMPLEMENT_NV_RAM_STORAGE == STD_ON */

#endif /* DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431 */

#define DLT_STOP_SEC_CODE
#include <Dlt_MemMap.h>

#endif /* ifndef DLT_BSW_H */
