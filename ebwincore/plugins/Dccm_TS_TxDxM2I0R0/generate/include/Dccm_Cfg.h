#ifndef DCCM_CFG_H
#define DCCM_CFG_H

 /***************************************************************************
 *
 * \file Dccm_Cfg.h
 *
 * \brief Dccm Diagnostic implementation of Dccm configuration
 *
 * Copyright 2018 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* MISRA-C:2012 Deviation List
 *
 */

/*==================[inclusions]=============================================*/

/*==================[macros]=================================================*/

/**
 *
 * \brief The execute interval of ::Dccm_MainFunction
 *
 */
#if (defined DCCM_EXE_INTERVAL)
#error DCCM_EXE_INTERVAL is already defined
#endif
[!VAR "DccmMainFuncTime" = "node:value(DccmGeneral/DccmMainfunctioCycle)"!][!//
[!VAR "UdsExeInterval" = "(num:f($DccmMainFuncTime) * 1000)"!][!//
#define DCCM_EXE_INTERVAL             [!"num:i($UdsExeInterval)"!]U    /* in ms */

/** \brief Switch for DET usage */
#if (defined DCCM_DEV_ERROR_DETECT)
#error DCCM_DEV_ERROR_DETECT is already defined
#endif
[!IF "DccmGeneral/Dccm_Dev_Error_Detect = 'true'"!][!//
#define DCCM_DEV_ERROR_DETECT         STD_ON
[!ELSE!][!//
#define DCCM_DEV_ERROR_DETECT         STD_OFF
[!ENDIF!][!//

/** \brief Switch for Buffer Streaming functionality */
#if (defined DCCM_BUFFER_STREAMING)
#error DCCM_BUFFER_STREAMING is already defined
#endif
[!IF "DccmGeneral/Dccm_Buffer_Streaming = 'true'"!][!//
#define DCCM_BUFFER_STREAMING         STD_ON
[!ELSE!][!//
#define DCCM_BUFFER_STREAMING         STD_OFF
[!ENDIF!][!//

/** \brief Switch for P2Client usage */
#if (defined DCCM_P2CLIENT_ENABLED)
#error DCCM_P2CLIENT_ENABLED is already defined
#endif
[!IF "node:exists(as:modconf('Dccm')[1]/DccmTimeouts/DccmP2Client)"!][!//
#define DCCM_P2CLIENT_ENABLED         STD_ON
[!ELSE!][!//
#define DCCM_P2CLIENT_ENABLED         STD_OFF
[!ENDIF!][!//

/** \brief Switch for P6Client usage */
#if (defined DCCM_P6CLIENT_ENABLED)
#error DCCM_P6CLIENT_ENABLED is already defined
#endif
[!IF "node:exists(as:modconf('Dccm')[1]/DccmTimeouts/DccmP6Client)"!][!//
#define DCCM_P6CLIENT_ENABLED         STD_ON
[!ELSE!][!//
#define DCCM_P6CLIENT_ENABLED         STD_OFF
[!ENDIF!][!//

/** \brief For functional communication, Dccm will not expect any answer from the server. */
#if (defined DCCM_FUNCTIONAL_COMM_NO_RESPONSE_EXPECTED)
#error DCCM_FUNCTIONAL_COMM_NO_RESPONSE_EXPECTED is already defined
#endif
[!IF "DccmGeneral/Dccm_Functional_Communication_With_No_Response_From_Server = 'true'"!][!//
#define DCCM_FUNCTIONAL_COMM_NO_RESPONSE_EXPECTED         STD_ON
[!ELSE!][!//
#define DCCM_FUNCTIONAL_COMM_NO_RESPONSE_EXPECTED         STD_OFF
[!ENDIF!][!//

/** \brief Max number of parallel diagnostic protocols */
#if (defined DCCM_MAX_DIAGNOSTIC_PROTOCOLS)
#error DCCM_MAX_DIAGNOSTIC_PROTOCOLS is already defined
#endif
#define DCCM_MAX_DIAGNOSTIC_PROTOCOLS   [!"num:integer(DccmGeneral/Dccm_Num_Of_Parallel_Diagnostic_Protocols)"!]U

/** \brief Number of diagnostic protocols used for physical communication*/
#if (defined DCCM_MAX_PHYSICAL_DIAGNOSTIC_PROTOCOLS)
#error DCCM_MAX_PHYSICAL_DIAGNOSTIC_PROTOCOLS is already defined
#endif
#define DCCM_MAX_PHYSICAL_DIAGNOSTIC_PROTOCOLS  [!"num:integer(DccmGeneral/Dccm_Num_Of_Parallel_Diagnostic_Protocols - DccmGeneral/Dccm_Num_Of_Functional_Diagnostic_Protocols)"!]U

/** \brief Dccm Number Of Physical Addresses */
#if (defined DCCM_MAX_SERVERS_PHYSICAL_ADDRESSING)
#error DCCM_MAX_SERVERS_PHYSICAL_ADDRESSING is already defined
#endif
[!VAR "Udsservers" = "0"!][!//
[!LOOP "DccmPhysicalPduIds"!][!VAR "Udsservers" = "num:i(count(*))"!] [!ENDLOOP!][!//
#define DCCM_MAX_SERVERS_PHYSICAL_ADDRESSING            [!WS!][!"$Udsservers"!]U

/** \brief Dccm Number Of Functional Addresses */
#if (defined DCCM_MAX_SERVERS_FUNCTIONAL_ADDRESSING)
#error DCCM_MAX_SERVERS_FUNCTIONAL_ADDRESSING is already defined
#endif
[!VAR "Udsserversf" = "0"!][!//
[!LOOP "DccmFunctionalPduIds"!][!VAR "Udsserversf" = "num:i(count(*))"!] [!ENDLOOP!][!//
#define DCCM_MAX_SERVERS_FUNCTIONAL_ADDRESSING            [!WS!][!"$Udsserversf"!]U

/** \brief Timeout value for internal timeout handling. */
#if (defined DCCM_TIMEOUT_INTERNAL)
#error DCCM_TIMEOUT_INTERNAL is already defined
#endif
[!VAR "DccmTimeoutInternal" = "node:value(DccmTimeouts/DccmTimeoutInternal)"!][!//
[!IF "DccmTimeouts/DccmTimeoutInternal = 0"!][!//
#define DCCM_TIMEOUT_INTERNAL           0xFFFFFFFFUL                          /* timeout disabled */
[!ELSE!][!//
#define DCCM_TIMEOUT_INTERNAL           [!"num:i($DccmTimeoutInternal)"!]UL   /* in ms */
[!ENDIF!][!//

/** \brief Timeout value for P2Client timeout handling. */
#if (defined DCCM_TIMEOUT_P2CLIENT)
#error DCCM_TIMEOUT_P2CLIENT is already defined
#endif
[!IF "node:exists(as:modconf('Dccm')[1]/DccmTimeouts/DccmP2Client)"!][!//
[!VAR "DccmTimeoutP2Client" = "node:value(DccmTimeouts/DccmP2Client/DccmTimeoutP2Client)"!][!//
#define DCCM_TIMEOUT_P2CLIENT            [!"num:i($DccmTimeoutP2Client)"!]UL   /* in ms */
[!ELSE!][!//
#define DCCM_TIMEOUT_P2CLIENT           0xFFFFFFFFUL                          /* timeout disabled */
[!ENDIF!][!//

/** \brief Timeout value for P2*Client timeout handling. */
#if (defined DCCM_TIMEOUT_P2STARCLIENT)
#error DCCM_TIMEOUT_P2STARCLIENT is already defined
#endif
[!IF "node:exists(as:modconf('Dccm')[1]/DccmTimeouts/DccmP2Client)"!][!//
[!VAR "DccmTimeoutP2StarClient" = "node:value(DccmTimeouts/DccmP2Client/DccmTimeoutP2StarClient)"!][!//
#define DCCM_TIMEOUT_P2STARCLIENT           [!"num:i($DccmTimeoutP2StarClient)"!]UL   /* in ms */
[!ELSE!][!//
#define DCCM_TIMEOUT_P2STARCLIENT           0xFFFFFFFFUL                          /* timeout disabled */
[!ENDIF!][!//

/** \brief Timeout value for P6Client timeout handling. */
#if (defined DCCM_TIMEOUT_P6CLIENT)
#error DCCM_TIMEOUT_P6CLIENT is already defined
#endif
[!IF "node:exists(as:modconf('Dccm')[1]/DccmTimeouts/DccmP6Client)"!][!//
[!VAR "DccmTimeoutP6Client" = "node:value(DccmTimeouts/DccmP6Client/DccmTimeoutP6Client)"!][!//
#define DCCM_TIMEOUT_P6CLIENT            [!"num:i($DccmTimeoutP6Client)"!]UL   /* in ms */
[!ELSE!][!//
#define DCCM_TIMEOUT_P6CLIENT           0xFFFFFFFFUL                          /* timeout disabled */
[!ENDIF!][!//

/** \brief Timeout value for P6*Client timeout handling. */
#if (defined DCCM_TIMEOUT_P6STARCLIENT)
#error DCCM_TIMEOUT_P6STARCLIENT is already defined
#endif
[!IF "node:exists(as:modconf('Dccm')[1]/DccmTimeouts/DccmP6Client)"!][!//
[!VAR "DccmTimeoutP6StarClient" = "node:value(DccmTimeouts/DccmP6Client/DccmTimeoutP6StarClient)"!][!//
#define DCCM_TIMEOUT_P6STARCLIENT           [!"num:i($DccmTimeoutP6StarClient)"!]UL   /* in ms */
[!ELSE!][!//
#define DCCM_TIMEOUT_P6STARCLIENT           0xFFFFFFFFUL                          /* timeout disabled */
[!ENDIF!][!//

/*==================[type definitions]=======================================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

/** @} doxygen end group definition */
#endif /* #ifndef DCCM_CFG_H */
/*==================[end of file]============================================*/
