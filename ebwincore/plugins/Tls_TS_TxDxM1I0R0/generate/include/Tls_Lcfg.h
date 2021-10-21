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
#ifndef TLS_LCFG_H
#define TLS_LCFG_H

[!AUTOSPACING!]
/*==================[inclusions]============================================*/
#include <TcpIp_Socket_Api.h>       /* Type definitions of Socket SW-unit */
#include <TcpIp_IpAddrM_Api.h>      /* Type definitions of IpAddrM SW-unit*/
#include <TcpIp_Api.h>              /* Type definitions of Tcp SW-unit */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

[!SELECT "as:modconf('Tls')[1]"!][!//
[!IF "node:exists(TlsExtensionsDetectionConfig)"!][!//
[!//
/** \brief Function pointer type for <Up>_TlsExtensionsDetection */
typedef P2FUNC(void, TLS_APPL_CODE, Tls_ExtensionsDetectionFpType)
(
  TcpIp_ConstSockAddrPtrType const remoteAddrPtr,
  uint16 extensionType
);
[!//
[!ENDIF!][!//
[!ENDSELECT!][!//

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/* start data section declaration */
#define TLS_START_SEC_CONST_32
#include <Tls_MemMap.h>

/* value used to validate post build configuration against link time configuration */
extern CONST(uint32, TLS_CONST) Tls_LcfgSignature;

/* stop data section declaration */
#define TLS_STOP_SEC_CONST_32
#include <Tls_MemMap.h>

#define TLS_START_SEC_CONST_UNSPECIFIED
#include <Tls_MemMap.h>


[!SELECT "as:modconf('Tls')[1]"!][!//
[!IF "node:exists(TlsExtensionsDetectionConfig)"!][!//
[!//
/** \brief Function pointer for <Up>_TlsExtensionsDetection */
extern CONST(Tls_ExtensionsDetectionFpType, TLS_APPL_CONST) Tls_ExtensionsDetectionAPI;
[!//
[!ENDIF!][!//
[!ENDSELECT!][!//

#define TLS_STOP_SEC_CONST_UNSPECIFIED
#include <Tls_MemMap.h>

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef TLS_LCFG_H */
/*==================[end of file]===========================================*/
