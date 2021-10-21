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

#ifndef TLS_ENCODE_INT_H
#define TLS_ENCODE_INT_H

/*==================[inclusions]============================================*/

#include <TSMem.h>
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

/** \brief Write the Certificate (client or server) handshake message to the transmit buffer. */
TS_MOD_PRIV_DECL FUNC(uint32, TLS_CODE) Tls_Handshake_WriteCertificate
(
  Tls_PtrConnection ConnectionPtr
);


/** \brief  Write the CertificateRequest handshake message from a server to the transmit buffer. */
TS_MOD_PRIV_DECL FUNC(uint32, TLS_CODE) Tls_Handshake_WriteServerCertificateRequest
(
  Tls_PtrConnection ConnectionPtr
);

/** \brief Write the CertificateVerify handshake message from a client to the transmit buffer. */
TS_MOD_PRIV_DECL FUNC(uint32, TLS_CODE) Tls_Handshake_WriteClientCertificateVerify
(
  Tls_PtrConnection ConnectionPtr
);

/** \brief Return the size of a ServerKeyExchange message that uses ECDH from a server (specified in RFC 8422). */
TS_MOD_PRIV_DECL FUNC(uint32, TLS_CODE) Tls_Handshake_GetSizeEcdhServerKeyExchange
(
  void
);

/** \brief Write the ServerKeyExchange message that uses ECDH from a server to the transmit buffer (specified in RFC 8422). */
TS_MOD_PRIV_DECL FUNC(uint32, TLS_CODE) Tls_Handshake_WriteEcdhServerKeyExchange
(
  Tls_PtrConnection ConnectionPtr
);

/** \brief Write the ClientKeyExchange message that uses ECDH from a client to the transmit buffer (specified in RFC 8422). */
TS_MOD_PRIV_DECL FUNC(uint32, TLS_CODE) Tls_Handshake_WriteEcdhClientKeyExchange
(
  Tls_PtrConnection ConnectionPtr
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


#endif /* TLS_ENCODE_INT_H */
