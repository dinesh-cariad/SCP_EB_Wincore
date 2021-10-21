/**
 * \file
 *
 * \brief AUTOSAR Tls
 *
 * This file contains the implementation of the AUTOSAR
 * module Tls.
 *
 * \version 1.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef SRC_AUTOSAR_INCLUDE_TLS_DEBUG_H_
#define SRC_AUTOSAR_INCLUDE_TLS_DEBUG_H_

/*==================[inclusions]==================================================================*/
/* Define this to enable debug output */
/* #define TLS_ENABLE_DEBUG */


#include <Tls_Int.h>
#ifdef TLS_ENABLE_DEBUG
#include <stdio.h>
#endif

/*==================[macros]======================================================================*/
#ifdef TLS_ENABLE_DEBUG
#define TLS_PRINTF(param) (void)printf param
#define TLS_DEBUG_PRF_PRINTSTATE Tls_Debug_PRF_PrintState
#define TLS_DEBUG_PRINTSTATE Tls_Debug_PrintState
#define TLS_DEBUG_PRINTBYTES Tls_Debug_PrintBytes
#define TLS_DEBUG_PRINTRECORDTYPE Tls_Debug_PrintRecordType
#define TLS_DEBUG_PRINTHANDSHAKETYPE Tls_Debug_PrintHandshakeType
#else
#define TLS_PRINTF(param)
#define TLS_DEBUG_PRF_PRINTSTATE(C)
#define TLS_DEBUG_PRINTSTATE(C)
#define TLS_DEBUG_PRINTBYTES(T,B,C)
#define TLS_DEBUG_PRINTRECORDTYPE(T)
#define TLS_DEBUG_PRINTHANDSHAKETYPE(T)
#endif

/*==================[declaration of types]========================================================*/

/*==================[declaration of external constants]===========================================*/

/*==================[declaration of external variables]===========================================*/

/*==================[declaration of external functions]===========================================*/
#ifdef TLS_ENABLE_DEBUG

#define TLS_START_SEC_CODE
#include <Tls_MemMap.h>

extern FUNC(void, TLS_CODE) Tls_Debug_PRF_PrintState(P2VAR(Tls_PRF_ContextType, AUTOMATIC, TLS_VAR) context);
extern FUNC(void, TLS_CODE) Tls_Debug_PrintBytes(P2CONST(char, AUTOMATIC, TLS_CONST) title, P2CONST(uint8, AUTOMATIC, TLS_VAR) bytes, uint32 count);
extern FUNC(void, TLS_CODE) Tls_Debug_PrintState(P2VAR(Tls_ConnectionType, AUTOMATIC, TLS_VAR) context);
extern FUNC(void, TLS_CODE) Tls_Debug_PrintRecordType(uint8 type);
extern FUNC(void, TLS_CODE) Tls_Debug_PrintHandshakeType(uint8 type);

#define TLS_STOP_SEC_CODE
#include <Tls_MemMap.h>

#endif

/*================================================================================================*/
#endif /* SRC_AUTOSAR_INCLUDE_TLS_DEBUG_H_ */

/*==================[end of file]=================================================================*/
