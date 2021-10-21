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

#ifndef TLS_CFG_H
#define TLS_CFG_H

/*==================[inclusions]==================================================================*/

#include <Tls_DefProgCfg.h>
[!IF "(TlsGeneral/TlsEnableCertificates = 'true')"!]
#include <KeyM.h>
[!ENDIF!][!//

/*==================[macros]======================================================================*/

/** \brief Configuration parameter TlsDevErrorDetection
 */
[!IF "(TlsGeneral/TlsDevErrorDetect = 'true')"!][!//
#define TLS_DEV_ERROR_DETECT  STD_ON
[!ELSE!][!//
#define TLS_DEV_ERROR_DETECT  STD_OFF
[!ENDIF!][!//

#define TLS_CERTIFICATE_SUPPORT \
[!IF "(TlsGeneral/TlsEnableCertificates = 'true')"!][!//
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!][!//

#define TLS_CFG_CSM_JOB_BASED_KEYEXCHANGE \
[!IF "(TlsGeneral/TlsEnableJobBasedKeyExchange = 'true')"!][!//
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!][!//

#define TLS_CFG_CONNECTION_COUNT ([!"num:i(count(TlsConnection/*))"!]u)
#define TLS_CFG_CSM_PROCESSING [!"TlsGeneral/TlsCsmProcessing"!]
#define TLS_CFG_MAINFUNCTION_PERIOD ([!"num:i(TlsGeneral/TlsMainFunctionPeriod*1000)"!])

/** \brief Detection of Extensions Callout
 */
#define TLS_EXTENSIONS_DETECTION_FCT_ENABLE \
[!IF "node:exists(TlsExtensionsDetectionConfig)"!][!//
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!][!//

#define TLS_CSM_KEYDERIVE_SUPPORT \
[!IF "(TlsGeneral/TlsCsmKeyDeriveSupport = 'true')"!][!//
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!][!//

/*==================[declaration of types]========================================================*/

/** \brief TLS client or server */
typedef enum
{
  /** \brief TLS client */
    TLS_CLIENT,
    /** \brief TLS server */
    TLS_SERVER
} Tls_ConnectionEndType;

/** \brief TLS connection configuration */
typedef struct
{
  /* !LINKSTO Tls.Config.Connection.CipherSuite,1 */
  /** \brief The cipher ID to negotiate during handshake */
  uint16 cipher;

  /** \brief TRUE if DTLS is enabled */
  boolean dtls;
  /** \brief Endpoint of the TLS connection */
  Tls_ConnectionEndType entity;
  /** \brief Pointer to identity data (server identify hint or client identity)*/
  P2CONST(uint8, AUTOMATIC, TLS_CONST) identity;
  /** \brief Identity length */
  uint16 identityLength;

  /** \brief Csm job for MacGenerate with HMAC and PreMaster key */
  uint32 PRFPreMasterSecretJobId;
  /** \brief Csm job for MacGenerate with HMAC and Master key */
  uint32 PRFMasterSecretJobId;
  /* !LINKSTO Tls.RFC.Handshake.PreMasterSecret,1 */
  /** \brief Csm key with the PreMasterSecret. This key is not read or written by Tls.
   * The user has to make sure it contains the premaster secret
   *  */
  uint32 PreMasterSecretKeyId;
  /** \brief Csm key with the MasterSecret */
  uint32 MasterSecretKeyId;

  /* !LINKSTO Tls.Config.Connection.Jobs,1 */
  /* !LINKSTO Tls.Config.Connection.Keys,1 */
  /** \brief Csm job for record MAC generation */
  uint32 MACGenerateJobId;
  /** \brief Csm job for record MAC verification */
  uint32 MACVerifyJobId;
  /** \brief Csm key for record MAC generation */
  uint32 MACGenerateKeyId;
  /** \brief Csm key for record MAC verification */
  uint32 MACVerifyKeyId;

  /** \brief Csm job SHA256 hash */
  uint32 hashJobId;

  /** \brief Csm job for random number generation. Used for generation of client_random, server_random and DTLS cookie */
  uint32 randomJobId;

  /** \brief Csm job for record encryption */
  uint32 encryptJobId;
  /** \brief Csm job for record decryption */
  uint32 decryptJobId;
  /** \brief Csm key for record encryption */
  uint32 encryptKeyId;
  /** \brief Csm key for record decryption */
  uint32 decryptKeyId;

  /* Tls ConnectionId */
  uint8 connectionId;

  /** \brief Pointer to the configurable transmit buffer */
  Tls_PtrUint8 transmitBufferPtr;
  /** \brief Pointer to the configurable receive buffer */
  Tls_PtrUint8 receiveBufferPtr;
  /** \brief Pointer to the configurable decryption buffer */
  Tls_PtrUint8 decryptBufferPtr;
  /** \brief Pointer to the configurable encryption buffer for dtls */
  Tls_PtrUint8 dtlsEncryptBufferPtr;
  /** \brief Size of the configurable transmit buffer */
  uint16 transmitBufferSize;
  /** \brief Size of the configurable receive buffer */
  uint16 receiveBufferSize;
  /** \brief Size of the configurable decrypt buffer */
  uint16 decryptBufferSize;
  /** \brief Size of the configurable encrypt buffer for dtls */
  uint16 dtlsEncryptBufferSize;

  /** \brief Maximum record size for transmission and reception.
   *   When RecordSizeLimitExtensionEnabled is TRUE:
   *   - this value is used to notify the local recordSizeLimit (maximum size the local node can receive).
   *   - the remote recordsizeLimit (maximum size to be transmitted) should be less or equal to this value. */
  uint16 maxRecordSize;

#if TLS_CERTIFICATE_SUPPORT == STD_ON
  /* TODO: struct packing */
  /** \brief Pointer the local certificate chain. */
  P2CONST(KeyM_CertificateIdType, AUTOMATIC, TLS_CONST) localCertChainIdPtr;
  /** \brief Number of certificates present in the local certificate chain. */
  /* TODO: document/check max */
  uint8 localCertChainLength;
  /** \brief Csm job ID to generate signatures using the private key of the local end-entity certificate */
  uint32 localCertSigGenerateJobId;
  /** \brief Pointer to the local CA common name */
  P2CONST(uint8, AUTOMATIC, TLS_CONST) localCaCommonName;
  /** \brief local CA common name length */
  uint8 localCaCommonNameLength;
  /** \brief Pointer the remote certificate chain. */
  P2CONST(KeyM_CertificateIdType, AUTOMATIC, TLS_CONST) remoteCertChainIdPtr;
  /** \brief Number of certificates present in the remote certificate chain. */
  /* TODO: document/check max */
  uint8 remoteCertChainLength;
  /** \brief Csm job ID to verify signatures using the public key of the remote end-entity certificate */
  uint32 remoteCertSigVerifyJobId;
#endif /* TLS_CERTIFICATE_SUPPORT == STD_ON */

  /** \brief Csm key ID of the key used for ephemeral DH key exchange for private key generation */
  uint32  keyExchangeEphemeralGenerateKeyId;
  /** \brief Csm key ID of the key used for ephemeral DH key exchange for key exchange */
  uint32  keyExchangeEphemeralExchangeKeyId;

  /** \brief TRUE if Record_size_limit extension is enabled */
  boolean RecordSizeLimitExtensionEnabled;

} Tls_ConnectionConfigType;

typedef P2CONST(Tls_ConnectionConfigType, AUTOMATIC, TLS_CONST) Tls_PtrConstConnectionConfig;

typedef struct
{
  Tls_ConnectionConfigType ConnectionConfigs[TLS_CFG_CONNECTION_COUNT];
} Tls_GlobalConfigType;

/*==================[declaration of external constants]===========================================*/

#define TLS_START_SEC_CONST_UNSPECIFIED
#include <Tls_MemMap.h>

extern CONST(Tls_GlobalConfigType, TLS_CONST) Tls_Config;

#define TLS_STOP_SEC_CONST_UNSPECIFIED
#include <Tls_MemMap.h>

/*==================[declaration of external variables]===========================================*/

/*==================[declaration of external functions]===========================================*/

/*================================================================================================*/

#endif /* #ifndef TLS_CFG_H */

/*==================[end of file]=================================================================*/
