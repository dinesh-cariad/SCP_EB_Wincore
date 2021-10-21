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

#ifndef TLS_INT_H
#define TLS_INT_H

/* TODO: file should be in src/ not include/ */

/*===============================[includes]======================================================*/

#include <Csm.h>
#include <SchM_Tls.h>
#include <TcpIp_Ip_Int.h>
#include <Tls.h>
#include <Tls_Cfg.h>
#include <Tls_DefProg.h>
#include <ComStack_Helpers.h>

/*===========================[macro definitions]=================================================*/

/** \brief Instance Id for DET 2.0 */
#define TLS_INSTANCE_ID 0U

#define TLS_CIPHER_USES_ENCRYPTION(TlsConnectionPtr) \
  ((ConnectionPtr->cipherParams->cipherType) != TLS_NULL_CIPHER)

#define TLS_CIPHER_USES_AUTHENTICATION(TlsConnectionPtr) \
  ((ConnectionPtr->cipherParams->MACKeyLength) > 0u)

/** \brief TLS version 1.2 */
#define TLS_VERSION                     (0x0303U)

/** \brief Size of SHA256 block */
#define TLS_SHA256_SIZE                     (32U)

/** \brief Size of the TLS header */
#define TLS_RECORD_HEADER_SIZE               (5U)

/** \brief Maximum size of the initialization vector */
#define TLS_MAX_IV_LENGTH                   (16U)

/** \brief Maximum size of the record MAC for AEAD ciphers */
#define TLS_AEAD_MAX_MAC_LENGTH             (16U)

/** \brief Size of a PRF block(HMAC-SHA256) */
#define TLS_PRF_BLOCK_SIZE                  (32U)

/** \brief Size of the length field of a signature */
#define TLS_SIGNATURE_LENGTH_SIZE            (2U)

/** \brief Size of a signature using (Pure/Hash)EdDSA Ed25519 */
#define TLS_SIGNATURE_ED25519_SIZE          (64U)

/** \brief Size of a private/public key for (Pure/Hash)EdDSA Ed25519 */
#define TLS_KEY_ED25519_SIZE                (32U)

/** \brief Length of "client finished" string */
#define TLS_CLIENTFINISHED_STRING_LENGTH    (15U)
/** \brief Length of "server finished" string */
#define TLS_SERVERFINISHED_STRING_LENGTH    (15U)
#define TLS_FINISHEDLABEL_MAX_LENGTH ((TLS_CLIENTFINISHED_STRING_LENGTH > TLS_SERVERFINISHED_STRING_LENGTH) ? TLS_CLIENTFINISHED_STRING_LENGTH : TLS_SERVERFINISHED_STRING_LENGTH)

/** \brief Maximum of all TLS string lengths for PRF */
#define TLS_LONGEST_STRING                  (15U)
/** \brief Maximum length of the seed for the PRF function */
#define TLS_PRF_SEED_MAX_LENGTH ((2U * TLS_RANDOM_SIZE) + TLS_LONGEST_STRING)
/** \brief Maximum length of the PRF result */
#define TLS_PRF_RESULT_MAX_LENGTH          (128U)

/** \brief Handshake record type */
#define TLS_RECORD_HANDSHAKE                (22U)
/** \brief ApplicationData record type */
#define TLS_RECORD_APPLICATIONDATA          (23U)
/** \brief ChangeCipherSpec record type */
#define TLS_RECORD_CHANGECIPHERSPEC         (20U)
/** \brief Alert record type */
#define TLS_RECORD_ALERT                    (21U)

/** \brief ClientHello handshake type */
#define TLS_HANDSHAKE_CLIENTHELLO            (1U)
/** \brief ServerHello handshake type */
#define TLS_HANDSHAKE_SERVERHELLO            (2U)
/** \brief Certificate handshake type */
#define TLS_HANDSHAKE_CERTIFICATE           (11U)
/** \brief ServerKeyExchange handshake type */
#define TLS_HANDSHAKE_SERVERKEYEXCHANGE     (12U)
/** \brief CertificateRequest handshake type */
#define TLS_HANDSHAKE_CERTIFICATEREQUEST    (13U)
/** \brief ServerHelloDone handshake type */
#define TLS_HANDSHAKE_SERVERHELLODONE       (14U)
/** \brief CertificateVerify handshake type */
#define TLS_HANDSHAKE_CERTIFICATEVERIFY     (15U)
/** \brief ClientKeyExchange handshake type */
#define TLS_HANDSHAKE_CLIENTKEYEXCHANGE     (16U)
/** \brief Finished handshake type */
#define TLS_HANDSHAKE_FINISHED              (20U)
/** \brief HelloVerifyRequest handshake type for DTLS */
#define TLS_HANDSHAKE_HELLOVERIFYREQUEST     (3U)
/** \brief HelloRequest handshake type */
#define TLS_HANDSHAKE_HELLOREQUEST           (0U)

/** \brief Size of the TLS handshake header */
#define TLS_HANDSHAKE_HEADER_SIZE              (4U)
/** \brief Size of the handshake fragment buffer for reassembly of received handshake messages */
#define TLS_HANDSHAKE_FRAGMENT_BUFFER_SIZE   (2048U)
/* \brief Size of the buffer that holds all handshake messages for verify data computation */
#define TLS_HANDSHAKE_VERIFYDATA_BUFFER_SIZE (2048U)

/** \brief Chipher suite ID for TLS_PSK_WITH_AES_128_CCM */
#define TLS_PSK_WITH_AES_128_CCM            (0xC0A4)
/** \brief Chipher suite ID for TLS_PSK_WITH_AES_256_CCM */
#define TLS_PSK_WITH_AES_256_CCM            (0xC0A5)
/** \brief Chipher suite ID for TLS_PSK_WITH_AES_128_GCM_SHA256 */
#define TLS_PSK_WITH_AES_128_GCM_SHA256     (0x00A8)
/** \brief Chipher suite ID for TLS_PSK_WITH_AES_256_GCM_SHA384 */
#define TLS_PSK_WITH_AES_256_GCM_SHA384     (0x00A9)
/** \brief Chipher suite ID for TLS_PSK_WITH_NULL_SHA256 */
#define TLS_PSK_WITH_NULL_SHA256            (0x00B0)

/** \brief Chipher suite ID for TLS_ECDHE_RSA_WITH_NULL_SHA. From RFC 4492. */
#define TLS_ECDHE_RSA_WITH_NULL_SHA               (0xC010)
/** \brief Chipher suite ID for TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA. From RFC 4492. */
#define TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA        (0xC013)
/** \brief Chipher suite ID for TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA. From RFC 4492. */
#define TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA        (0xC014)
/** \brief Chipher suite ID for TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256. From RFC 5289. */
#define TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256     (0xC027)
/** \brief Chipher suite ID for TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384. From RFC 5289. */
#define TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384     (0xC028)
/** \brief Chipher suite ID for TLS_ECDHE_ECDSA_WITH_NULL_SHA. From RFC 4492. */
#define TLS_ECDHE_ECDSA_WITH_NULL_SHA             (0xC006)
/** \brief Chipher suite ID for TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256. From RFC 5289. */
#define TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256   (0xC02B)

/** \brief Output size for Csm_RandomGenerate */
#define TLS_RANDOM_SIZE               (32U)
/* !LINKSTO Tls.RFC.Handshake.MasterSecret.Size,1 */
/** \brief Size of the TLS master secret */
#define TLS_MASTER_SECRET_SIZE        (48U)
/** \brief Size of verify_data */
#define TLS_VERIFY_DATA_LENGTH        (12U)

/** \brief Warning alert level */
#define TLS_ALERT_LEVEL_WARNING       (1U)
/** \brief Fatal alert level */
#define TLS_ALERT_LEVEL_FATAL         (2U)

/** \brief close_notify alert description */
#define TLS_ALERT_DESC_CLOSE_NOTIFY              (0U)
/** \brief unexpected_message alert description */
#define TLS_ALERT_DESC_UNEXPECTED_MESSAGE       (10U)
/** \brief bad_record_mac alert description */
#define TLS_ALERT_DESC_BAD_RECORD_MAC           (20U)
/** \brief decryption_failed alert description */
#define TLS_ALERT_DESC_DECRYPTION_FAILED        (21U)
/** \brief record_overflow alert description */
#define TLS_ALERT_DESC_RECORD_OVERFLOW          (22U)
/** \brief decompression_failure alert description */
#define TLS_ALERT_DESC_DECOMPRESSION_FAILURE    (30U)
/** \brief handshake_failure alert description */
#define TLS_ALERT_DESC_HANDSHAKE_FAILURE        (40U)
/** \brief no_certificate alert description */
#define TLS_ALERT_DESC_NO_CERTIFICATE           (41U)
/** \brief bad_certificate alert description */
#define TLS_ALERT_DESC_BAD_CERTIFICATE          (42U)
/** \brief unsupported_certificate alert description */
#define TLS_ALERT_DESC_UNSUPPORTED_CERTIFICATE  (43U)
/** \brief certificate_revoked alert description */
#define TLS_ALERT_DESC_CERTIFICATE_REVOKED      (44U)
/** \brief certificate_expired alert description */
#define TLS_ALERT_DESC_CERTIFICATE_EXPIRED      (45U)
/** \brief certificate_unknown alert description */
#define TLS_ALERT_DESC_CERTIFICATE_UNKNOWN      (46U)
/** \brief illegal_parameter alert description */
#define TLS_ALERT_DESC_ILLEGAL_PARAMETER        (47U)
/** \brief unknown_ca alert description */
#define TLS_ALERT_DESC_UNKNOWN_CA               (48U)
/** \brief access_denied alert description */
#define TLS_ALERT_DESC_ACCESS_DENIED            (49U)
/** \brief decode_error alert description */
#define TLS_ALERT_DESC_DECODE_ERROR             (50U)
/** \brief decrypt_error alert description */
#define TLS_ALERT_DESC_DECRYPT_ERROR            (51U)
/** \brief export_restriction alert description */
#define TLS_ALERT_DESC_EXPORT_RESTRICTION       (60U)
/** \brief protocol_version alert description */
#define TLS_ALERT_DESC_PROTOCOL_VERSION         (70U)
/** \brief insufficient_security alert description */
#define TLS_ALERT_DESC_INSUFFICIENT_SECURITY    (71U)
/** \brief internal_error alert description */
#define TLS_ALERT_DESC_INTERNAL_ERROR           (80U)
/** \brief user_canceled alert description */
#define TLS_ALERT_DESC_USER_CANCELED            (90U)
/** \brief no_renegotiation alert description */
#define TLS_ALERT_DESC_NO_RENEGOTIATION        (100U)
/** \brief unsupported_extension alert description */
#define TLS_ALERT_DESC_UNSUPPORTED_EXTENSION   (110U)

/** \brief ClientCertificateType: ecdsa_sign */
#define TLS_CLIENT_CERTIFICATE_TYPE_ECDSA_SIGN  (64U)
/** \brief Lenght of a SignatureAndHashAlgorithm */
#define TLS_SIGNATURE_AND_HASH_ALGORITHM_LENGTH (2U)
/** \brief SignatureAndHashAlgorithm: Ed25519 */
#define TLS_SIGNATURE_AND_HASH_ALGORITHM_ED25519 (0x0807U)
/** \brief ECCurveType: named_curve */
#define TLS_EC_CURVE_TYPE_NAMED_CURVE           (3U)
/** \brief NamedCurve: X25519 */
#define TLS_NAMED_CURVE_X25519                 (29U)

/** \brief Size of AEAD associated data */
#define TLS_AEAD_ASSOCIATED_DATA_LENGTH (8U+1U+2U+2U)

/** \brief DTLS 1.2 version */
#define TLS_DTLS_VERSION                  (0xFEFDU)
/** \brief Size of DTLS receive window. 64 is recommended but we use 32 so that it fits in uint32. */
#define TLS_DTLS_RECEIVE_WINDOW_SIZE          (32U)
/* !LINKSTO Dtls.RFC.Handshake.CookieSize,1 */
/** \brief Maximum length of a DTLS cookie */
#define TLS_DTLS_MAX_COOKIE_LENGTH           (255U)
/** \brief Size of the DTLS record header */
#define TLS_RECORD_DTLS_HEADER_SIZE           (13U)
/** \brief Size of the DTLS handshake header */
#define TLS_HANDSHAKE_DTLS_HEADER_SIZE        (12U)
/** \brief Size of the DTLS retransmit buffer. Must have enough space for one flight of messages. */
#define TLS_DTLS_RETRANSMIT_BUFFER_SIZE      (256U)
/** \brief Maximum number of retransmits. !LINKSO Dtls.RFC.Handshake.RetransmitLimit,1 */
#define TLS_DTLS_RETRANSMIT_LIMIT              (6U)
/** \brief Initial retransmit timeout. !LINKSTO Dtls.RFC.Handshake.RetransmitTimeout,1 */
#define TLS_DTLS_HANDSHAKE_INITIAL_TIMEOUT  (1000U/TLS_CFG_MAINFUNCTION_PERIOD)

/** \brief Size of the header that is prepended to the record before MAC generation and verification */
#define TLS_MAC_HEADER_SIZE                   (13U)

/** \brief Number of records for which TxConfirmations can be buffered. */
#define TLS_TXCONFIRMATION_SIZE               (10U)

/* !LINKSTO Tls.Async,1 */
/** \brief Asynchronous Csm interface */
#define TLS_CSM_ASYNCHRONOUS                   (0U)
/** \brief Synchronous Csm interface */
#define TLS_CSM_SYNCHRONOUS                    (1U)

/** \brief The Tls module ID */
#define TLS_MODULE_ID                        (789U)

#if (defined ComStack_Uint8PtrType)
#if 0 /* TODO: Needs to be activated as soon as TcpIp_Ip_Int.h is not included anymore */
#error ComStack_Uint8PtrType is already defined
#endif
#else
#define ComStack_Uint8PtrType Tls_Uint8PtrType
#endif

#if (defined ComStack_ConstUint8PtrType)
#if 0 /* TODO: Needs to be activated as soon as TcpIp_Ip_Int.h is not included anymore */
#error ComStack_ConstUint8PtrType is already defined
#endif
#else
#define ComStack_ConstUint8PtrType Tls_ConstUint8PtrType
#endif

#define TLS_GET16(headerPtr) COMSTACK_GET16(headerPtr, 0U)
#define TLS_GET24(headerPtr, constByteOffset) \
  (((uint32)(((uint32)(((Tls_ConstUint8PtrType)(headerPtr))[(constByteOffset)+0U]))<<16u)) | \
   ((uint32)(((uint32)(((Tls_ConstUint8PtrType)(headerPtr))[(constByteOffset)+1U]))<< 8u)) | \
   ((uint32)(((uint32)(((Tls_ConstUint8PtrType)(headerPtr))[(constByteOffset)+2U]))     )) )
#define TLS_SET16(headerPtr, constByteOffset, value16) COMSTACK_SET16(headerPtr, constByteOffset, value16)

#define TLS_SET24(headerPtr, constByteOffset, value32) \
  do { \
    enum {ofs = (constByteOffset) }; \
    Tls_Uint8PtrType const hdrPtr = (headerPtr); \
    uint32 const val32 = (value32); \
    hdrPtr[ofs+0U] = TS_CAST(uint, 8, val32 >> 16U); \
    hdrPtr[ofs+1U] = TS_CAST(uint, 8, val32 >>  8U); \
    hdrPtr[ofs+2U] = TS_CAST(uint, 8, val32       ); \
  } while(0)

#define TLS_CIPHER_USES_CERTS(CipherSuite) \
  (((CipherSuite) == TLS_ECDHE_ECDSA_WITH_NULL_SHA) || \
  ((CipherSuite) == TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256))

/** \brief Size of Handshake Protocol fields */
/** \brief size of TLS/DTLS version */
#define TLS_IS_POINTER_VALID(payloadPtr) \
  (payloadPtr != NULL_PTR)

/** \brief Size of Handshake Protocol fields */
/** \brief size of TLS/DTLS version */
#define TLS_VERSION_SIZE                                    (2u)
/** \brief Size of cipher suite Length field */
#define TLS_CIPHERSUITE_LENGTH_SIZE                         (2u)
/** \brief Size of Compression Method length fields */
#define TLS_COMPRESSIONMETHOD_LENGTH_SIZE                   (1u)
/** \brief Size of Extension Type fields */
#define TLS_EXTENSION_TYPE_SIZE                             (2u)
/** \brief Size of Extension Length fields */
#define TLS_EXTENSION_LENGTH_SIZE                           (2u)
/** \brief Size of the certificate(s) length fields of the certificate messages */
#define TLS_CERTIFICATE_LENGTH_SIZE                         (3U)
/** \brief Size of the length field of CertificateRequest.certificate_types */
#define TLS_CERTIFICATE_REQUEST_CERT_TYPES_LENGTH_SIZE      (1U)
/** \brief Size of the length field of CertificateRequest.supported_signature_algorithms */
#define TLS_CERTIFICATE_REQUEST_SUPP_SIG_ALGOS_LENGTH_SIZE  (2U)
/** \brief Size of the length field of CertificateRequest.certificate_authorities */
#define TLS_CERTIFICATE_REQUEST_CAS_LENGTH_SIZE             (2U)
/** \brief Size of the ServerKeyExchange.params.curve_type field */
#define TLS_SERVER_KEY_EXCHANGE_EC_CURVE_TYPE_SIZE          (1U)
/** \brief Size of the ServerKeyExchange.params.named_curve field*/
#define TLS_SERVER_KEY_EXCHANGE_NAMED_CURVE_SIZE            (2U)
/** \brief Size of the length field of ServerKeyExchange.params.public */
#define TLS_SERVER_KEY_EXCHANGE_EC_POINT_LENGTH_SIZE        (1U)
/** \brief Size of the length field of ClientKeyExchange.dh_public */
#define TLS_CLIENT_KEY_EXCHANGE_DH_PUBLIC_LENGTH_SIZE       (1U)

/** \brief Protects the TX buffer. */
#define TLS_ENTER_EXCLUSIVE_AREA_TX() SchM_Enter_Tls_SCHM_TLS_EXCLUSIVE_AREA_TX()
/** \brief Protects the TX buffer. */
#define TLS_EXIT_EXCLUSIVE_AREA_TX() SchM_Exit_Tls_SCHM_TLS_EXCLUSIVE_AREA_TX()
/** \brief Protects the RX buffer. Used when data is received via RxIndication or consumed from the rx buffer. */
#define TLS_ENTER_EXCLUSIVE_AREA_RX() SchM_Enter_Tls_SCHM_TLS_EXCLUSIVE_AREA_RX()
/** \brief Protects the RX buffer. Used when data is received via RxIndication or consumed from the rx buffer. */
#define TLS_EXIT_EXCLUSIVE_AREA_RX() SchM_Exit_Tls_SCHM_TLS_EXCLUSIVE_AREA_RX()


/*==================[declaration of types]=======================================================*/

/** \brief Pointer to a constant unsigned integer 8 bit */
typedef P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) Tls_ConstUint8PtrType;
/** \brief Pointer to a variable unsigned integer 8 bit */
typedef P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) Tls_Uint8PtrType;

/** \brief Type for unsigned 64bit integer. There is no uint64! */
typedef unsigned long long Tls_Uint64Type;

/* TODO: Add states for new message types */
/** \brief State of the TLS connection */
typedef enum
{
  /** \brief Default state after initialization. */
  TLS_STATE_DEFAULT = 0,
  /** \brief Handshake shall be started by this endpoint. Will be set when socket is connected. */
  TLS_STATE_START,
  /** \brief ClientHello was sent/received. */
  TLS_STATE_CLIENTHELLO_DONE,
  /** \brief ServerHello was sent/received. */
  TLS_STATE_SERVERHELLO_DONE,
  /** \brief Certificate from server was sent/received */
  TLS_STATE_SERVERCERTIFICATE_DONE,
  /** \brief ServerKeyExchange was sent/received. */
  TLS_STATE_SERVERKEYEXCHANGE_DONE,
  /** \brief CertificateRequest was sent/received */
  TLS_STATE_CERTIFICATE_REQUEST_DONE,
  /** \brief ServerHelloDone was sent/received. */
  TLS_STATE_SERVERHELLODONE_DONE,
  /** \brief Certificate from the client was sent/received */
  TLS_STATE_CLIENTCERTIFICATE_DONE,
  /** \brief ClientKeyExchange was sent/received. */
  TLS_STATE_CLIENTKEYEXCHANGE_DONE,
  /** \brief CertificateVerify was sent/received. */
  TLS_STATE_CERTIFICATE_VERIFY_DONE,
  /** \brief ChangeCipherSpec was sent. */
  TLS_STATE_CHANGECIPHERSPEC_SENT,
  /** \brief Finished was sent. */
  TLS_STATE_FINISHED_SENT,
  /** \brief ChangeCipherSpec was received. */
  TLS_STATE_CHANGECIPHERSPEC_RECEIVED,
  /** \brief Finished was received. */
  TLS_STATE_FINISHED_RECEIVED,
  /** \brief Connection is established. ApplicationData may be sent/received. */
  TLS_STATE_OK,
  /** \brief Connection is being closed. */
  TLS_STATE_CLOSING,
  /** \brief Connection is closed. */
  TLS_STATE_CLOSED,
  /** \brief The connection failed. */
  TLS_STATE_ERROR
} Tls_StateType;

/** \brief State of asynchronous key derivation */
typedef enum
{
  /** \brief Key derivation is idle. */
  TLS_KEYDERIVE_STATE_IDLE = 0,
  /** \brief Key derivation shall be started. */
  TLS_KEYDERIVE_STATE_START,
  /** \brief Master secret generation is busy. */
  TLS_KEYDERIVE_STATE_BUSY,
  /** \brief Key generation is busy. */
  TLS_KEYDERIVE_STATE_BUSY_KEYS,
  /** \brief Key derivation is ready. */
  TLS_KEYDERIVE_STATE_READY,
  /** \brief Key derivation failed. */
  TLS_KEYDERIVE_STATE_ERROR
} Tls_KeyDeriveStateType;

/** \brief State of asynchronous decryption */
typedef enum
{
  /** \brief Decryption job is idle. */
  TLS_DECRYPT_STATE_IDLE = 0,
  /** \brief Decryption job shall be started. */
  TLS_DECRYPT_STATE_START,
  /** \brief Decryption job is busy. */
  TLS_DECRYPT_STATE_BUSY,
  /** \brief Decryption job is ready. */
  TLS_DECRYPT_STATE_READY,
  /** \brief Decryption failed. */
  TLS_DECRYPT_STATE_ERROR
} Tls_DecryptStateType;

/** \brief State of asynchronous MAC generation */
typedef enum
{
  /** \brief MAC job is idle. */
  TLS_MAC_STATE_IDLE = 0,
  /** \brief MAC job shall be started. */
  TLS_MAC_STATE_START,
  /** \brief MAC job is busy processing the header. */
  TLS_MAC_STATE_HEADER_BUSY,
  /** \brief MAC job is ready processing the header. */
  TLS_MAC_STATE_HEADER_READY,
  /** \brief MAC job is busy processing the payload. */
  TLS_MAC_STATE_PAYLOAD_BUSY,
  /** \brief MAC job is ready processing the payload. */
  TLS_MAC_STATE_PAYLOAD_READY,
  /** \brief MAC job finish operation is busy. */
  TLS_MAC_STATE_FINISH_BUSY,
  /** \brief MAC job is ready. */
  TLS_MAC_STATE_READY,
  /** \brief MAC failed. */
  TLS_MAC_STATE_ERROR
} Tls_MacStateType;

/** \brief State of asynchronous certificate verification */
typedef enum
{
  /** \brief VerifyCertificate is idle. */
  TLS_VERIFY_CERT_STATE_IDLE = 0,
  /** \brief VerifyCertificate is busy processing the certificate chain. */
  TLS_VERIFY_CERT_STATE_BUSY,
  /** \brief VerifyCertificate is ready. */
  TLS_VERIFY_CERT_STATE_READY,
  /** \brief VerifyCertificate failed. */
  TLS_VERIFY_CERT_STATE_ERROR
} Tls_VerifyCertStateType;

/** \brief State of asynchronous signature generation */
typedef enum
{
  /** \brief SignatureGenerate is idle. */
  TLS_SIG_GEN_STATE_IDLE = 0,
  /** \brief SignatureGenerate is busy processing ClientHello.Random. */
  TLS_SIG_GEN_STATE_CLIENT_RANDOM_BUSY,
  /** \brief SignatureGenerate is finished processing ClientHello.Random. */
  TLS_SIG_GEN_STATE_CLIENT_RANDOM_READY,
  /** \brief SignatureGenerate is busy processing ServerHello.Random. */
  TLS_SIG_GEN_STATE_SERVER_RANDOM_BUSY,
  /** \brief SignatureGenerate is finished processing ServerHello.Random. */
  TLS_SIG_GEN_STATE_SERVER_RANDOM_READY,
  /** \brief SignatureGenerate is busy processing ServerKeyExchange.params. */
  TLS_SIG_GEN_STATE_SKEX_PARAMS_BUSY,
  /** \brief SignatureGenerate is finished processing ServerKeyExchange.params. */
  TLS_SIG_GEN_STATE_SKEX_PARAMS_READY,
  /** \brief SignatureGenerate is busy processing the finish call */
  TLS_SIG_GEN_STATE_FINISH_BUSY,
  /** \brief SignatureGenerate is finished calculating the signature. */
  TLS_SIG_GEN_STATE_READY,
  /** \brief SignatureGenerate failed. */
  TLS_SIG_GEN_STATE_ERROR
} Tls_SigGenStateType;

/** \brief State of asynchronous signature verification */
typedef enum
{
  /** \brief SignatureVerify is idle. */
  TLS_SIG_VER_STATE_IDLE = 0,
  /** \brief SignatureVerify is busy verifying the signature. */
  TLS_SIG_VER_STATE_BUSY,
  /** \brief SignatureVerify is ready. */
  TLS_SIG_VER_STATE_READY,
  /** \brief SignatureVerify failed. */
  TLS_SIG_VER_STATE_ERROR
} Tls_SigVerStateType;

/** \brief State of asynchronous job-based key exchange calculation of public key */
typedef enum
{
  /** \brief KeyExchangeCalcPubVal is idle. */
  TLS_KE_CALC_PUB_VAL_STATE_IDLE = 0,
  /** \brief KeyExchangeCalcPubVal is busy verifying the signature. */
  TLS_KE_CALC_PUB_VAL_STATE_BUSY,
  /** \brief KeyExchangeCalcPubVal is ready. */
  TLS_KE_CALC_PUB_VAL_STATE_READY,
  /** \brief KeyExchangeCalcPubVal failed. */
  TLS_KE_CALC_PUB_VAL_STATE_ERROR
} Tls_KeCalcPubValStateType;

/** \brief State of asynchronous job-based key exchange calculation of shared secret */
typedef enum
{
  /** \brief KeyExchangeCalcSecret is idle. */
  TLS_KE_CALC_SECRET_STATE_IDLE = 0,
  /** \brief KeyExchangeCalcSecret is busy verifying the signature. */
  TLS_KE_CALC_SECRET_STATE_BUSY,
  /** \brief KeyExchangeCalcSecret is ready. */
  TLS_KE_CALC_SECRET_STATE_READY,
  /** \brief KeyExchangeCalcSecret failed. */
  TLS_KE_CALC_SECRET_STATE_ERROR
} Tls_KeCalcSecretStateType;

/** \brief State of asynchronous encryption */
typedef enum
{
  /** \brief Encryption job is idle. */
  TLS_ENCRYPT_STATE_IDLE = 0,
  /** \brief Encryption job shall be started. */
  TLS_ENCRYPT_STATE_START,
  /** \brief Encryption job is busy. */
  TLS_ENCRYPT_STATE_BUSY,
  /** \brief Encryption job is ready. */
  TLS_ENCRYPT_STATE_READY,
  /** \brief Encryption failed. */
  TLS_ENCRYPT_STATE_ERROR
} Tls_EncryptStateType;

/** \brief State of asynchronous random number generation */
typedef enum
{
  /** \brief Random job is idle. */
  TLS_RANDOM_STATE_IDLE = 0,
  /** \brief Random job shall be started. */
  TLS_RANDOM_STATE_START,
  /** \brief Random job is busy. */
  TLS_RANDOM_STATE_BUSY,
  /** \brief Random job is ready. */
  TLS_RANDOM_STATE_READY,
  /** \brief Random job failed. */
  TLS_RANDOM_STATE_ERROR
} Tls_RandomStateType;

/** \brief State of the verify data generation which consists of a hash and PRF operation. */
typedef enum
{
  /** \brief VerifyData function is idle. */
  TLS_VERIFYDATA_STATE_IDLE = 0,
  /** \brief VerifyData function is started. */
  TLS_VERIFYDATA_STATE_START,
  /** \brief The hash job is running. */
  TLS_VERIFYDATA_STATE_HASH_BUSY,
  /** \brief The hash job is ready. */
  TLS_VERIFYDATA_STATE_HASH_READY,
  /** \brief The PRF function is running. */
  TLS_VERIFYDATA_STATE_PRF_BUSY,
  /** \brief The PRF function is ready. */
  TLS_VERIFYDATA_STATE_READY,
  /** \brief VerifyData generation failed. */
  TLS_VERIFYDATA_STATE_ERROR
} Tls_VerifyDataStateType;

/** \brief States for asynchronous PRF */
typedef enum
{
  /** \brief PRF is idle. */
  TLS_PRF_STATE_IDLE = 0,
  /** \brief PRF shall be started. */
  TLS_PRF_STATE_START,
  /** \brief PRF is busy. */
  TLS_PRF_STATE_BUSY_1,
  /** \brief PRF is busy. */
  TLS_PRF_STATE_BUSY_2,
  /** \brief PRF is busy. */
  TLS_PRF_STATE_BUSY_2A,
  /** \brief PRF is busy. */
  TLS_PRF_STATE_BUSY_2B,
  /** \brief PRF is busy. */
  TLS_PRF_STATE_BUSY_2C,
  /** \brief PRF is busy. */
  TLS_PRF_STATE_BUSY_2D,
  /** \brief PRF is busy. */
  TLS_PRF_STATE_BUSY_2E,
  /** \brief PRF is ready. */
  TLS_PRF_STATE_READY,
  /** \brief PRF failed. */
  TLS_PRF_STATE_ERROR
} Tls_PRF_StateType;

/** \brief TLS Cipher Type */
typedef enum
{
  /** \brief No cipher */
  TLS_NULL_CIPHER,
  /** \brief Stream cipher (deprecated, use AEAD instead */
  TLS_STREAM_CIPHER,
  /** \brief Block cipher (deprecated, use AEAD instead */
  TLS_BLOCK_CIPHER,
  /** \brief AEAD cipher */
  TLS_AEAD_CIPHER
} Tls_CypherType;

/** \brief Cipher suite information */
typedef struct
{
  /** \brief TLS cipher suite ID */
  uint16 cipher;
  /** \brief Cipher Type */
  Tls_CypherType cipherType;
  /** \brief Length of the MAC key */
  uint8 MACKeyLength;
  /** \brief Length of the MAC that is appended to the record */
  uint8 MACLength;
  /** \brief Length of the initialization vector */
  uint8 IVLength;
  /** \brief Length of the encryption/decryption key */
  uint8 encKeyLength;

} Tls_CipherSuiteParametersType;
typedef P2CONST(Tls_CipherSuiteParametersType, AUTOMATIC, TLS_CONST) Tls_PtrConstCipherSuiteParameters;

/** \brief State and input/output data for PRF */
typedef struct
{
  /** \brief Input seed for the PRF */
  uint8 seed[TLS_PRF_SEED_MAX_LENGTH];
  /** \brief Length of seed for the PRF */
  uint32 seedLength;
  /** \brief PRF result */
  uint8 result[TLS_PRF_RESULT_MAX_LENGTH];
  /** \brief PRF result length */
  uint32 resultLength;

  /** \brief temporary PRF data */
  uint8 tmp[TLS_PRF_BLOCK_SIZE + TLS_PRF_SEED_MAX_LENGTH];
  /** \brief temporary PRF data */
  uint8 tmp2[TLS_PRF_BLOCK_SIZE];
  /** \brief temporary PRF data */
  uint8 r[TLS_PRF_BLOCK_SIZE];
  /** \brief temporary PRF data */
  uint32 pos;
  /** \brief temporary PRF data */
  uint32 len;

  /** \brief Csm job to use for PRF (MacGenerate) */
  uint32 jobId;
  /** \brief State of the PRF execution */
  Tls_PRF_StateType state;
} Tls_PRF_ContextType;
typedef P2VAR(Tls_PRF_ContextType, AUTOMATIC, TLS_VAR) Tls_PtrPRF_Context;

/** \brief Data that is only used during the handshake. This could be shared between connections. */
typedef struct
{
  /** \brief Buffer for handling fragmented handshake messages */
  uint8 handshakeFragmentBuffer[TLS_HANDSHAKE_FRAGMENT_BUFFER_SIZE];
  /** \brief Number of elements in the handshake fragment buffer */
  uint16 handshakeFragmentBufferLength;

  /** \brief Buffer for handshake data to calculate verify data. Used during handshake. */
  uint8 handshakeMessages[TLS_HANDSHAKE_VERIFYDATA_BUFFER_SIZE];
  /** \brief Number of bytes in the handshake message buffer */
  uint16 handshakeMessagesLength;
  /** \brief Overflow flag for the handshake buffer */
  boolean handshakeMessagesOverflow;

  /** \brief Client verification data. Used during handshake. */
  uint8 clientVerifyData[TLS_VERIFY_DATA_LENGTH];
  /** \brief Flag if the clientVerifyData was generated. */
  Tls_VerifyDataStateType clientVerifyDataState;

  /** \brief Server verification data. Used during handshake. */
  uint8 serverVerifyData[TLS_VERIFY_DATA_LENGTH];
  /** \brief Flag if the serverVerifyData was generated. */
  Tls_VerifyDataStateType serverVerifyDataState;

  /** \brief VerifyData that was received in the finished message. */
  uint8 receivedVerifyData[TLS_VERIFY_DATA_LENGTH];

  /** \brief Temporary storage for the handshake PRF seed. */
  uint8 verifyDataSeed[TLS_SHA256_SIZE + TLS_FINISHEDLABEL_MAX_LENGTH];
  /* Length value for Csm_Hash. */
  uint32 hashLength;

  /** \brief The server random number. Used during handshake. */
  uint8 serverRandom[TLS_RANDOM_SIZE];
  /** \brief The client random number. Used during handshake. */
  uint8 clientRandom[TLS_RANDOM_SIZE];

  /** \brief State of the Csm_RandomGenerate operation. */
  Tls_RandomStateType randomState;
  /** \brief Size of the random data. Used for Csm_RandomGenerate. */
  uint32 randomSize;

  /** \brief State of the asynchronous key derivation operation */
  Tls_KeyDeriveStateType keyDeriveState;

  /** \brief State of the asynchronous TLS pseudo random function */
  Tls_PRF_ContextType prf;

  /** \brief State of certificate verification. Only needed if the cipher suite uses certificates. */
  Tls_VerifyCertStateType verifyCertState;

  /** \brief State of signature generation. Only needed if cipher suite uses certificates and async crypto is configured. */
  Tls_SigGenStateType sigGenState;

  /** \brief Length of the generated signature. */
  uint32 sigGenOutputLength;

  /** \brief Pointer to the memory where the signature is written */
  Tls_Uint8PtrType sigGenOutputDataPtr;

  /** \brief Length of the data that is signed */
  uint32 sigGenInputLength;

  /** \brief Pointer to the memory where the signed data is located */
  Tls_Uint8PtrType sigGenInputDataPtr;

  /** \brief State of signature verification. Only needed if cipher suite uses certificates and async crypto is configured. */
  Tls_SigVerStateType sigVerState;

  /** \brief Result of the signature verification. Only needed if cipher suite uses certificates */
  Crypto_VerifyResultType sigVerifyResult;

  /** \brief State of key exchange public key calculation */
  Tls_KeCalcPubValStateType keCalcPubValState;

  /** \brief Length of the key exchange public key */
  uint32 keCalcPubValLength;

  /** \brief State of key exchange shared secret calculation */
  Tls_KeCalcSecretStateType keCalcSecretState;

  /** \brief DTLS retransmit timer. Decremented once per MainFunction. */
  uint32 dtls_retransmitTimer;

  /** \brief Buffer for one DTLS flight of messages. This is resent if a timeout occurs. */
  uint8 dtls_retransmitBuffer[TLS_DTLS_RETRANSMIT_BUFFER_SIZE];

  /** \brief Number of bytes in the DTLS retransmit buffer. */
  uint16 dtls_retransmitBufferCount;

  /** \brief Current number of retransmits */
  uint8 dtls_retransmitCount;

  /** \brief The cookie for DTLS */
  uint8 dtls_cookie[TLS_DTLS_MAX_COOKIE_LENGTH];
  /** \brief Length of the cookie */
  uint8 dtls_cookieLength;
  /** \brief State of the cookie generation for DTLS server */
  Tls_RandomStateType dtls_cookieState;

  uint8 dtls_helloCount;

  /** \brief Next DTLS handshake message sequence number to send */
  uint16 dtls_handshakeSeq;

  /** \brief Next DTLS handshake message sequence number to receive */
  uint8 dtls_nextReceiveSeq;
} Tls_HandshakeDataType;
typedef P2VAR(Tls_HandshakeDataType, AUTOMATIC, TLS_VAR) Tls_PtrHandshakeData;

/** \brief A buffer with configurable size. */
typedef struct
{
  /** \brief Current number of bytes in the buffer */
  uint16 count;

  /** \brief Size of the buffer */
  uint16 size;

  /** \brief Pointer to the buffer data*/
  Tls_PtrUint8 buffer;
} Tls_BufferType;

typedef P2VAR(Tls_BufferType, AUTOMATIC, TLS_VAR) Tls_PtrBuffer;

/** \brief Holds connection data that is specific to DTLS. */
typedef struct
{
  /** \brief DTLS write epoch. Can only be 0 or 1 as renegotiation is not supported */
  uint16 writeEpoch;
  /** \brief DTLS read epoch. Can only be 0 or 1 as renegotiation is not supported */
  uint16 readEpoch;
  /** \brief DTLS receive window bit pattern. Each bit is one received record in the window. */
  uint32 receiveWindow;
  /** \brief Needed for TcpIp_Udp_provideTxBuffer and TcpIp_Udp_transmit */
  TcpIp_Ip_EthTxHandleType ethTxHandle;
} Tls_DtlsDataType;

/** \brief Queue for TxConfirmation record size calculation */
typedef struct
{
  /** \brief Number of confirmed bytes that is lower than the size of the first record size. If this is != 0 it means a record was partly confirmed. */
  uint32 confirmed;
  /** \brief Storage for record sizes */
  uint16 buffer[TLS_TXCONFIRMATION_SIZE];
  /** \brief Storage for record types */
  uint8 types[TLS_TXCONFIRMATION_SIZE];
  /** \brief Number of record sizes in the buffer */
  uint8 count;
  /** \brief Index of the first record size in the buffer */
  uint8 start;
} Tls_TxConfirmationBufferType;

/** \brief Pointer to a TxConfirmation buffer */
typedef P2VAR(Tls_TxConfirmationBufferType, AUTOMATIC, TLS_VAR) Tls_PtrTxConfirmationBuffer;

typedef struct
{
  /** \brief Use flag. Set to TRUE if this connection is in use. */
  boolean active;

  /** \brief The TcpIp socket number this connection belongs to. */
  TcpIp_SocketIdType socket;

  /** \brief State of the TLS connection */
  Tls_StateType state;

  /** \brief TRUE if an error occurred before entering CLOSING state. In this case the next state is ERROR instead of CLOSED. */
  boolean errorFlag;

  /** \brief State of MAC generation. This is only needed for cipher suites that do not use the AEAD interface. */
  Tls_MacStateType macGenerateState;
  /** \brief Header for MAC generation. This is only needed for cipher suites that do not use the AEAD interface. */
  uint8 macGenerateHeader[TLS_MAC_HEADER_SIZE];
  /** \brief MAC length returned bei Csm_MacGenerate */
  uint32 macGenerateLength;
  /** \brief State of MAC verification. This is only needed for cipher suites that do not use the AEAD interface. */
  Tls_MacStateType macVerifyState;
  /** \brief Header for MAC verification. This is only needed for cipher suites that do not use the AEAD interface. */
  uint8 macVerifyHeader[TLS_MAC_HEADER_SIZE];

  /** \brief State of the asynchronous decryption operation */
  Tls_DecryptStateType decryptState;
  /** \brief Associated data for AEAD decryption */
  uint8 decryptAssociatedData[TLS_AEAD_ASSOCIATED_DATA_LENGTH];
  Crypto_VerifyResultType decryptVerify;
  uint32 decryptLength;

  /** \brief Receive buffer */
  Tls_BufferType receiveBuffer;

  /** \brief Size of the (potentially still encrypted) received record. */
  uint32 receivedRecordSize;

  /** \brief State of the asynchronous encryption operation */
  Tls_EncryptStateType encryptState;
  /** \brief Associated data for AEAD encryption */
  uint8 encryptAssociatedData[TLS_AEAD_ASSOCIATED_DATA_LENGTH];
  /** \brief Buffer for Csm_AEADEncrypt MAC output */
  uint8 encryptMAC[TLS_AEAD_MAX_MAC_LENGTH];
  /** \brief Length parameter for Csm_AEADEncrypt MAC */
  uint32 encryptMACLength;
  /** \brief Length parameter for Csm_AEADEncrypt cipher text */
  uint32 encryptLength;

  /** \brief The transmit buffer. Contains plaintext records. */
  Tls_BufferType transmitBuffer;
  /** \brief The maximum transmit record size. Provided by the remote host when the recordSizeLimit is enabled,
             or set to the maximum buffer capacity otherwise. */
  uint16 transmitRecordSizeLimit;

  /** \brief Buffer for encrypted data. It contains only one TLS record. Provided by TcpIp_Tcp_provideTxBuffer. */
  P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) encryptBuffer;
  /** \brief Number of valid bytes in the encrypt buffer. */
  uint16 encryptBufferLength;
  /** \brief Capacity of the encrypt buffer. */
  uint16 encryptBufferCapacity;

  /** \brief Buffer for decrypted data. It contains only one TLS record. */
  Tls_BufferType decryptBuffer;

  /** \brief The initialization vector for encryption */
  uint8 writeIV[TLS_MAX_IV_LENGTH];
  /** \brief The initialization vector for decryption */
  uint8 readIV[TLS_MAX_IV_LENGTH];
  /** \brief The current cipher suite */
  /* TODO: redundant with cipherParams->cipher? */
  uint16 cipher;

  /** \brief The sequence number used for reading */
  Tls_Uint64Type readSeqNum;
  /** \brief The sequence number used for writing */
  Tls_Uint64Type writeSeqNum;

  /** \brief DTLS specific data */
  Tls_DtlsDataType dtls;

  /** \brief Data that is only used during the handshake. */
  Tls_HandshakeDataType handshakeData;

  /** \brief Storage for length of transmitted records */
  Tls_TxConfirmationBufferType txConfirmations;

  /** \brief Pointer to the configuration for this connection. */
  Tls_PtrConstConnectionConfig config;
  /** \brief Pointer to the cipher parameters for this connection. */
  Tls_PtrConstCipherSuiteParameters cipherParams;

  /** \brief Listen socket for TcpAccepted. Only used for TLS_SERVER. */
  TcpIp_SocketIdType listenSocket;
  /** \brief Address of the remote endpoint. May be an IPV6 or IPV4 address */
  TcpIp_SockAddrInet6Type remoteAddr;

  /** \brief Close flag: Set to true do close the connection */
  boolean closeRequest;

  /** \brief Transmission error flag: TcpIp failed to send data */
  boolean transmissionError;
} Tls_ConnectionType;

/** \brief Pointer to a TLS connection */
typedef P2VAR(Tls_ConnectionType, AUTOMATIC, TLS_VAR) Tls_PtrConnection;
/** \brief Pointer to a const TLS connection */
typedef P2CONST(Tls_ConnectionType, AUTOMATIC, TLS_VAR) Tls_PtrConstConnection;


/*=====================[external constants declarations]=========================================*/

/*=====================[internal constants declarations]=========================================*/

/*==================[internal function declarations]=============================================*/

/* TODO: documentation */
TS_MOD_PRIV_DECL FUNC(uint8, TLS_CODE) Tls_Handshake_GetHeaderSize(Tls_PtrConnection ConnectionPtr);

/** \brief Get the size of a TLS/DTLS record for a connection
 *
 * Returns 5 for TLS connections and 13 for DTLS connections.
 *
 * \param[in] ConnectionPtr     Pointer to the TLS connection context
 * \param[in] BufPtr            Pointer to the buffer to fill with data
 * \param[in] BufLength         Size of the buffer
 *
 * \return Size in bytes
 */
TS_MOD_PRIV_DECL FUNC(uint8, TLS_CODE) Tls_GetRecordHeaderSize(Tls_PtrConstConnection ConnectionPtr);

/* TODO: documentation */
TS_MOD_PRIV_DECL FUNC(void, TLS_CODE) Tls_Handshake_WriteHeader(Tls_PtrConnection ConnectionPtr, Tls_PtrUint8 dest, uint8 handshake_type, uint32 handshake_len);

/** \brief Writes a TLS or DTLS record header to the transmit buffer
 *
 *
 * \param[in] ConnectionPtr    Pointer to the TLS connection context
 * \param[in] pos              Position in the transmit buffer where the record header begins
 * \param[in] length           Length field of the record header
 * \param[in] type             Type field of the record header
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TLS_CODE) Tls_WriteRecordHeaderToTransmitBuffer(Tls_PtrConnection ConnectionPtr, uint32 pos, uint16 Length, uint8 type);

/** \brief Appends a single byte to a buffer
 *
 *
 * \param[in,out] BufferPtr    Pointer to the Tls_BufferType
 * \param[in] Byte             The byte to append
 *
 * \return E_OK if successfull, E_NOT_OK otherwise
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TLS_CODE) Tls_Buffer_AppendByte(Tls_PtrBuffer BufferPtr, uint8 Byte);

/** \brief Sends a TLS alert message and closes the connection
 *
 *  The alert record is queued for transmission and will be sent asynchronously.
 *  After transmission the connection will be closed.
 *
 * \param[in] ConnectionPtr     Pointer to the TLS connection context
 * \param[in] Level             TLS alert level
 * \param[in] Desc              TLS alert description
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TLS_CODE) Tls_Alert_Transmit(Tls_PtrConnection ConnectionPtr, uint8 Level, uint8 Desc);

/* TODO: documentation */
TS_MOD_PRIV_DECL FUNC(uint32, TLS_CODE) Tls_Handshake_GetLength(Tls_PtrConstUint8 handshake);

/*==================[external function definitions]==============================================*/
/* TODO: Remove when Csm supports job based key exchange */
extern FUNC(Std_ReturnType, TLS_CODE) Csm_JobKeyExchangeCalcPubVal
(
  uint32 jobId,
  uint32 keyId,
  P2VAR(uint8, AUTOMATIC, EBSTUBS_APPL_DATA) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, EBSTUBS_APPL_DATA) publicValueLengthPtr
);

/* TODO: Remove when Csm supports job based key exchange */
extern FUNC(Std_ReturnType, TLS_CODE) Csm_JobKeyExchangeCalcSecret
(
  uint32 jobId,
  uint32 keyId,
  P2CONST(uint8, AUTOMATIC, EBSTUBS_APPL_CONST) partnerPublicValuePtr,
  uint32 partnerPublicValueLength
);

TS_MOD_PRIV_DECL FUNC(uint16, TLS_CODE) Tls_Handshake_WriteExtension(Tls_PtrUint8 dest, uint16 extension_type, uint32 extension_len, uint8 * extension_payload);

TS_MOD_PRIV_DECL FUNC(uint32, TLS_CODE) Tls_GetExtensionsLength(Tls_PtrConnection ConnectionPtr);

TS_MOD_PRIV_DEFN FUNC(uint16, TLS_CODE) Tls_Handshake_WriteExtension_RecordSizeLimit(Tls_PtrUint8 dataPtr, uint16 size);

TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_HandleExtensions(Tls_PtrConnection ConnectionPtr, Tls_PtrConstUint8 DataPtr, uint16 Length);

TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_HandleExtension_RecordSizeLimit(Tls_PtrConnection ConnectionPtr, const uint8 * extensionPtr, uint16 extensionLength);

TS_MOD_PRIV_DECL FUNC(Tls_PtrConnection, TLS_CODE) Tls_GetConnectionPtr(Tls_ConnectionIdType ConnectionId);
/*=======================[external data declarations]============================================*/

/*=======================[internal data declarations]============================================*/

#endif
