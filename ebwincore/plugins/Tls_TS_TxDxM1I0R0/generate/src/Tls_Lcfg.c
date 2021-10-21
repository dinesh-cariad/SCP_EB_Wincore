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

/*==================[inclusions]============================================*/

#include <TSAutosar.h>          /* EB specific standard types */
#include <Tls_Lcfg.h>         /* link time configuration */

[!SELECT "as:modconf('Tls')[1]"!][!//
[!IF "node:exists(TlsExtensionsDetectionConfig)"!][!//
#include <[!"TlsExtensionsDetectionConfig/TlsExtensionsDetectionHeaderFileName"!]>
[!ENDIF!][!//
[!ENDSELECT!][!//

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/* start data section declaration */
#define TLS_START_SEC_CONST_32
#include <Tls_MemMap.h>

/* value used to validate post build configuration against link time configuration */
CONST(uint32, TLS_CONST) Tls_LcfgSignature = [!"asc:getConfigSignature(as:modconf('Tls')[1]//*[not(child::*) and (node:configclass() = 'Link')])"!]U;

/* stop data section declaration */
#define TLS_STOP_SEC_CONST_32
#include <Tls_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

#define TLS_START_SEC_CONST_UNSPECIFIED
#include <Tls_MemMap.h>


[!SELECT "as:modconf('Tls')[1]"!][!//
[!IF "node:exists(TlsExtensionsDetectionConfig)"!][!//
[!//
/** \brief Function pointer for <Up>_TlsExtensionsDetection */
CONST(Tls_ExtensionsDetectionFpType, TLS_APPL_CONST) Tls_ExtensionsDetectionAPI =
  &[!"TlsExtensionsDetectionConfig/TlsExtensionsDetectionCalloutName"!];
[!//
[!ENDIF!][!//
[!ENDSELECT!][!//

#define TLS_STOP_SEC_CONST_UNSPECIFIED
#include <Tls_MemMap.h>

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
