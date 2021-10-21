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

#ifndef TLS_DECODE_INT_H
#define TLS_DECODE_INT_H

/*==================[inclusions]============================================*/

#include <Tls_Int.h>
#include <Tls_Cfg.h>
#if (TLS_CERTIFICATE_SUPPORT == STD_ON)
#include <KeyM.h>
#endif /* (TLS_CERTIFICATE_SUPPORT == STD_ON) */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define TLS_START_SEC_CODE
#include <Tls_MemMap.h>

#if TLS_CERTIFICATE_SUPPORT == STD_ON

/** \brief Handles a received Certificate handshake message sent by a client or server */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_HandleCertificate
(
  Tls_PtrConnection ConnectionPtr,
  Tls_Uint8PtrType DataPtr,
  uint16 Length
);

/** \brief Handles a received ServerKeyExchange handshake message using ECDH (specified in RFC 8422) sent by a server */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_HandleEcdhServerKeyExchange
(
  Tls_PtrConnection ConnectionPtr,
  Tls_PtrConstUint8 DataPtr,
  uint16 Length
);

/** \brief Handles a received ClientKeyExchange handshake message using ECDH (specified in RFC 8422) sent by a client */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_HandleEcdhClientKeyExchange
(
  Tls_PtrConnection ConnectionPtr,
  Tls_PtrConstUint8 DataPtr,
  uint16 Length
);

/** \brief Handles a received CertificateRequest handshake message sent by a server */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_HandleCertificateRequest
(
  Tls_PtrConnection ConnectionPtr,
  Tls_PtrConstUint8 DataPtr,
  uint16 Length
);

/** \brief Handles a received CertificateVerify handshake message sent by a client */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_HandleCertificateVerify
(
  Tls_PtrConnection ConnectionPtr,
  Tls_PtrConstUint8 DataPtr,
  uint16 Length
);

#endif /* TLS_CERTIFICATE_SUPPORT == STD_ON */

#define TLS_STOP_SEC_CODE
#include <Tls_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /* TLS_DECODE_INT_H */
