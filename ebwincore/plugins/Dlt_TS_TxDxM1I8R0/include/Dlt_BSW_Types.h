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
#ifndef DLT_BSW_TYPES_H
#define DLT_BSW_TYPES_H

/* This file contains definitions of the types used by the interfaces published for BSW usage. */
/* !LINKSTO Dlt.BSW.Usage.Type_Definition, 1 */

/*==================[inclusions]============================================*/

#include <Dlt_Cfg.h>

/* !LINKSTO SWS_Dlt_00482,1 */
#if (DLT_INCLUDE_RTE == STD_ON)
#include <Rte_Type.h>
#endif

/*==================[macros]================================================*/

/* Values of Dlt_ReturnType */
/* !LINKSTO Dlt.ReturnValues.AUTOSAR421,1 */
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_421)

#define DLT_E_OK                  0x00U
#define DLT_E_MSG_TOO_LARGE       0x01U
#define DLT_E_CONTEXT_ALREADY_REG 0x02U
#define DLT_E_UNKNOWN_SESSION_ID  0x03U
#define DLT_E_IF_NOT_AVAILABLE    0x04U
#define DLT_E_IF_BUSY             0x05U
#define DLT_E_ERROR_UNKNOWN       0x06U
#define DLT_E_PENDING             0x07U
#define DLT_E_NOT_IN_VERBOSE_MODE 0x08U

/* !LINKSTO Dlt.ReturnValues.AUTOSAR422,1 */
#elif  (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_422)

#define DLT_E_OK                  0x00U
#define DLT_E_MSG_TOO_LARGE       0x02U
#define DLT_E_CONTEXT_ALREADY_REG 0x03U
#define DLT_E_UNKNOWN_SESSION_ID  0x04U
#define DLT_E_IF_NOT_AVAILABLE    0x05U
#define DLT_E_IF_BUSY             0x06U
#define DLT_E_ERROR_UNKNOWN       0x07U
#define DLT_E_PENDING             0x08U
#define DLT_E_NOT_IN_VERBOSE_MODE 0x09U
/* !LINKSTO Dlt.ReturnValues.AUTOSAR431,1 */
#else
#define DLT_E_OK                  0x00U
#define DLT_E_MSG_TOO_LARGE       0x02U
#define DLT_E_CONTEXT_ALREADY_REG 0x03U
#define DLT_E_UNKNOWN_SESSION_ID  0x04U
#define DLT_E_NO_BUFFER           0x05U
#define DLT_E_CONTEXT_NOT_YET_REG 0x06U
#define DLT_E_ERROR               0x09U


#define DLT_E_NOT_IN_VERBOSE_MODE 0x08U


#endif /* DLT_DEFAULT_ASR_RETVAL == AUTOSAR_421 */
/* !LINKSTO Dlt.ASR431.SWS_Dlt_00730,1 */
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
/** \brief This marco is used to add a channel assignment to a registered context */
#define DLT_ASSIGN_ADD  0x01U
/** \brief This marco is used to remove a channel assignment to a registered context */
#define DLT_ASSIGN_REMOVE  0x02U
/** \brief This marco is used to twhron a not supported error in case the Service is not supported */
#define DLT_E_NOT_SUPPORTED  0x07U
#endif
/* Values of Dlt_MessageLogLevelType */
/* !LINKSTO SWS_Dlt_00230,1, PRS_Dlt_00619,1*/
#ifndef DLT_LOG_OFF
/** \brief Message of LogLevel type Information */
#define DLT_LOG_OFF               0x00U
#endif
#ifndef DLT_LOG_FATAL
/** \brief Fatal system error */
#define DLT_LOG_FATAL             0x01U
#endif
#ifndef DLT_LOG_ERROR
/** \brief Application error */
#define DLT_LOG_ERROR             0x02U
#endif
#ifndef DLT_LOG_WARN
/** \brief Correct behavior cannot be ensured */
#define DLT_LOG_WARN              0x03U
#endif
#ifndef DLT_LOG_INFO
/** \brief Message of LogLevel type Information */
#define DLT_LOG_INFO              0x04U
#endif
#ifndef DLT_LOG_DEBUG
/** \brief Message of LogLevel type Debug */
#define DLT_LOG_DEBUG             0x05U
#endif
#ifndef DLT_LOG_VERBOSE
/* !LINKSTO Dlt.ASR431.SWS_Dlt_00656,1*/
/** \brief Message of LogLevel type Verbose */
#define DLT_LOG_VERBOSE           0x06U
#endif
#ifndef DLT_LOG_DEFAULT
/** \brief Message of LogLevel type Default */
#define DLT_LOG_DEFAULT           0xffU
#endif

/* Values of Dlt_MessageTraceType */
/* !LINKSTO SWS_Dlt_00231,1, PRS_Dlt_00620,1*/
#ifndef DLT_TRACE_VARIABLE
/** \brief Value of variable */
#define DLT_TRACE_VARIABLE        0x01U
#endif
#ifndef DLT_TRACE_FUNCTION_IN
/** \brief Call of a function */
#define DLT_TRACE_FUNCTION_IN     0x02U
#endif
#ifndef DLT_TRACE_FUNCTION_OUT
/** \brief Return of a function */
#define DLT_TRACE_FUNCTION_OUT    0x03U
#endif
#ifndef DLT_TRACE_STATE
/** \brief State of a State Machine*/
#define DLT_TRACE_STATE           0x04U
#endif
#ifndef DLT_TRACE_VFB
/** \brief RTE events*/
#define DLT_TRACE_VFB             0x05U
#endif

/* Values of Dlt_GetLogInfo's options parameter */
#ifndef DLT_GETLOGINFO_OPTIONS_NO_DESC
#define DLT_GETLOGINFO_OPTIONS_NO_DESC   0x06U
#endif

#ifndef DLT_GETLOGINFO_OPTIONS_WITH_DESC
#define DLT_GETLOGINFO_OPTIONS_WITH_DESC 0x07U
#endif

/*==================[type definitions]======================================*/

/* !LINKSTO Dlt.ReturnValues.AUTOSAR421,1 */
/* !LINKSTO Dlt.ReturnValues.AUTOSAR422,1 */
typedef uint8 Dlt_ReturnType;

#ifndef RTE_TYPE_Dlt_SessionIDType
/* !LINKSTO Dlt.ASR431.SWS_Dlt_00225,1 */
/* !LINKSTO Dlt.Ref_SWS_Dlt_00225,1 */
/* !LINKSTO SWS_Dlt_00322,1 */
/** \brief This type describes the Session ID */
typedef uint32 Dlt_SessionIDType;
#endif

/** \brief Internal representation of Dlt_ApplicationIDType */
typedef uint32 Dlt_Internal_ApplicationIDType;

/** \brief Internal representation of Dlt_ContextIDType */
typedef uint32 Dlt_Internal_ContextIDType;

/* Dlt_ApplicationIDType */

#ifndef RTE_TYPE_Dlt_ApplicationIDType

/** \brief This type describes the Application ID */
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)

/* !LINKSTO Dlt.ASR431.SWS_Dlt_00226, 1 */
typedef uint8 Dlt_ApplicationIDType[4U];

#else /* #if (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431) */

/* !LINKSTO Dlt.Ref_SWS_Dlt_00226,1 */
typedef uint32 Dlt_ApplicationIDType;

#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */

#endif /* #ifndef RTE_TYPE_Dlt_ApplicationIDType */

/* Dlt_ContextIDType */

#ifndef RTE_TYPE_Dlt_ContextIDType

/** \brief This type describes the Context ID */
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)

/* !LINKSTO Dlt.ASR431.SWS_Dlt_00227, 1 */
typedef uint8 Dlt_ContextIDType[4U];

#else /* #if (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431) */

/* !LINKSTO Dlt.Ref_SWS_Dlt_00227,1 */
typedef uint32 Dlt_ContextIDType;

#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */

#endif /* #ifndef RTE_TYPE_Dlt_ContextIDType */

#ifndef RTE_TYPE_Dlt_MessageArgumentCountType
/* !LINKSTO SWS_Dlt_00235,1 */
typedef uint16 Dlt_MessageArgumentCountType;
#endif

#ifndef RTE_TYPE_Dlt_MessageLogLevelType
/* !LINKSTO SWS_Dlt_00230,1 */
typedef uint8 Dlt_MessageLogLevelType;
#endif
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
/* !LINKSTO Dlt.ASR431.SWS_Dlt_00730,1 */
#ifndef RTE_TYPE_Dlt_AssignmentOperationType
/** \brief Assignment operation used for setting log channel assignments */
typedef uint8 Dlt_AssignmentOperationType;
#endif
#endif
#ifndef RTE_TYPE_Dlt_MessageTraceType
/* !LINKSTO SWS_Dlt_00231,1 */
typedef uint8 Dlt_MessageTraceType;
#endif

#ifndef RTE_TYPE_Dlt_MessageLogTraceType
typedef uint8 Dlt_MessageLogTraceType;
#endif

#ifndef RTE_TYPE_Dlt_MessageOptionsType
/* !LINKSTO SWS_Dlt_00229, 1, Dlt.ASR431.SWS_Dlt_00229, 1 */
/** \brief Bitfield */
typedef uint8 Dlt_MessageOptionsType;
#endif

#ifndef RTE_TYPE_Dlt_MessageTraceStatusType
typedef uint8 Dlt_MessageTraceStatusType;
#endif

#ifndef RTE_TYPE_Dlt_MessageLogInfoType
/* !LINKSTO SWS_Dlt_00236,1 */
/* Violation of [BSWM_C_005]
 * Description:  Members of structs SHALL be sorted in descending alignment requirement order.
 * Rationale:    This is an exported data type, and the order is defined in SWS.
 */
typedef struct
{
  Dlt_MessageArgumentCountType arg_count;
  Dlt_MessageLogLevelType      log_level;
  Dlt_MessageOptionsType       options;
  Dlt_ContextIDType            context_id;
  Dlt_ApplicationIDType        app_id;
} Dlt_MessageLogInfoType;
#endif

#ifndef RTE_TYPE_Dlt_MessageTraceInfoType
/* !LINKSTO SWS_Dlt_00237,1 */
/* Violation of [BSWM_C_005]
 * Description:  Members of structs SHALL be sorted in descending alignment requirement order.
 * Rationale:    This is an exported data type, and the order is defined in SWS.
 */
typedef struct
{
  Dlt_MessageTraceType      trace_info;
  Dlt_MessageOptionsType    options;
  Dlt_ContextIDType         context_id;
  Dlt_ApplicationIDType     app_id;
} Dlt_MessageTraceInfoType;
#endif

#ifndef RTE_TYPE_Dlt_MessageCommonInfoType
/* Violation of [BSWM_C_005]
 * Description:  Members of structs SHALL be sorted in descending alignment requirement order.
 * Rationale:    This is an exported data type, and the order is defined in SWS.
 */
typedef struct
{
  Dlt_MessageArgumentCountType arg_count;
  Dlt_MessageLogTraceType      log_level_trace_info;
  Dlt_MessageOptionsType       options;
  Dlt_ContextIDType            context_id;
  Dlt_ApplicationIDType        app_id;
} Dlt_MessageCommonInfoType;
#endif

/* !LINKSTO Dlt.ASR431.SWS_Dlt_00730,1 */
#ifndef RTE_TYPE_Dlt_AssignmentOperation

/** \brief Assignment operation used for setting log channel assignments */
typedef uint8 Dlt_AssignmentOperation;

#endif

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef DLT_BSW_TYPES_H */
/*==================[end of file]===========================================*/
