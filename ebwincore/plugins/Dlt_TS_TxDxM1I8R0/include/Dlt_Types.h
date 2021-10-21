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
#ifndef DLT_TYPES_H
#define DLT_TYPES_H

/* This file provides the type definitions of the AUTOSAR module Dlt.
 * Note: Some types are fixed and others depending on the configuration.
 */

/*==================[inclusions]============================================*/

#include <Dlt_Cfg.h>

/*==================[macros]================================================*/

/* Values of Dlt_MessageTypeType */
/* !LINKSTO SWS_Dlt_00123,1, SWS_Dlt_00120,1, SWS_Dlt_00224,1, PRS_Dlt_00120,1 */
/** \brief A log message */
#define DLT_TYPE_LOG              0x00U
/** \brief A trace message */
#define DLT_TYPE_APP_TRACE        0x01U
/** \brief A message forwarded from a communication bus (like CAN, FlexRay) */
#define DLT_TYPE_NW_TRACE         0x02U
/** \brief A message for internal use/control send between Dlt module and external client. */
#define DLT_TYPE_CONTROL          0x03U

/* Values of Dlt_MessageControlInfoType */
/* !LINKSTO SWS_Dlt_00232,1, PRS_Dlt_00621,1*/
/** \brief Request Control Message */
#define DLT_CONTROL_REQUEST       0x01U
/** \brief Respond Control Message */
#define DLT_CONTROL_RESPONSE      0x02U
#if (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431)
#define DLT_CONTROL_TIME          0x03U
#endif
/* Values of Dlt_MessageNetworkTraceInfoType */
/* !LINKSTO SWS_Dlt_00125,1, PRS_Dlt_00621,1 */
/** \brief Inter-Process-Communication */
#define DLT_NW_TRACE_IPC          0x01U
/** \brief CAN Communications bus */
#define DLT_NW_TRACE_CAN          0x02U
/** \brief FlexRay Communications bus */
#define DLT_NW_TRACE_FLEXRAY      0x03U
/** \brief Most Communications bus */
#define DLT_NW_TRACE_MOST         0x04U
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
  /** \brief Ethernet Communications bus */
#define Dlt_NW_TRACE_ETHERNET     0x05U
/** \brief Inter-SOME/IP Communication */
#define Dlt_NW_TRACE_SOMEIP       0x06U
#endif
/* Values of Dlt_CtrlReturnType */
#define DLT_CTRL_OK               0x00U
#define DLT_CTRL_NOT_SUPPORTED    0x01U
#define DLT_CTRL_ERROR            0x02U

/* Values of Dlt_GlobalLogStatusType */
#define DLT_LOGGING_DISABLED      0x00U
#define DLT_LOGGING_ENABLED       0x01U

/* Values of Dlt_MessageTraceStatusType */
#define DLT_TRACE_STATUS_OFF      0x00U
#define DLT_TRACE_STATUS_ON       0x01U
#define DLT_TRACE_STATUS_DEFAULT  0xffU

/* Values of Dlt_FilterMessagesType */
#define DLT_FILTER_MESSAGES_OFF   0x00U
#define DLT_FILTER_MESSAGES_ON    0x01U

/*==================[type definitions]======================================*/

#if (DLT_INCLUDE_RTE == STD_OFF)

#ifndef RTE_TYPE_Dlt_FilterMessagesType
typedef uint8 Dlt_FilterMessagesType;
#endif

#endif

#ifndef RTE_TYPE_Dlt_MessageTypeType
/* !LINKSTO SWS_Dlt_00123,1 */
/** \brief This type describes the type of the message */
typedef uint8 Dlt_MessageTypeType;
#endif

#ifndef RTE_TYPE_Dlt_MessageIDType
/* !LINKSTO Dlt.Ref_SWS_Dlt_00228,1 */
/** \brief Contains the unique Message ID for a message */
typedef uint8 Dlt_MessageIDType;
#endif

#ifndef RTE_TYPE_Dlt_MessageControlInfoType
/* !LINKSTO SWS_Dlt_00232,1 */
typedef uint8 Dlt_MessageControlInfoType;
#endif

#ifndef RTE_TYPE_Dlt_MessageNetworkTraceInfoType
/* !LINKSTO SWS_Dlt_00125,1 */
typedef uint8 Dlt_MessageNetworkTraceInfoType;
#endif

#ifndef RTE_TYPE_Dlt_CtrlReturnType
typedef uint8 Dlt_CtrlReturnType;
#endif

#ifndef RTE_TYPE_Dlt_GlobalLogStatusType
typedef uint8 Dlt_GlobalLogStatusType;
#endif

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef DLT_TYPES_H */
/*==================[end of file]===========================================*/
