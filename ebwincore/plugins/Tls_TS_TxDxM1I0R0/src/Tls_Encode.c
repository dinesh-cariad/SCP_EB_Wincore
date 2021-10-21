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

#include <Tls_Encode_Int.h>

/*==================[macros]================================================*/

/* TODO: move */
#define TLS_DER_SEQUENCE_TAG 0x30U
#define TLS_DER_SET_TAG 0x31U
#define TLS_DER_OBJECT_IDENTIFIER_TAG 0x06U
#define TLS_DER_OBJECT_IDENTIFIER_COMMON_NAME_LENGTH 3U
#define TLS_DER_OBJECT_IDENTIFIER_COMMON_NAME_VALUE 0x550403U
#define TLS_DER_UTF8STRING_TAG 0x0CU
#define TLS_DER_LENGTH_TAG_LENGTH 2U

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#define TLS_START_SEC_CODE
#include <Tls_MemMap.h>

#if TLS_CERTIFICATE_SUPPORT == STD_ON
TS_MOD_PRIV_DEFN FUNC(uint32, TLS_CODE) Tls_Handshake_WriteCertificate(Tls_PtrConnection ConnectionPtr)
{
  uint32 writtenSize;
  Std_ReturnType status = E_OK;
  uint8 const handshakeHeaderLength = Tls_Handshake_GetHeaderSize(ConnectionPtr);
  uint8 const recordHeaderLength = Tls_GetRecordHeaderSize(ConnectionPtr);
  KeyM_CertDataType cert;
  uint8 certChainLength = ConnectionPtr->config->localCertChainLength;
  KeyM_CertificateIdType const * certIdPtr = ConnectionPtr->config->localCertChainIdPtr;
  uint8 certIdx;

  uint16 certOffset = ConnectionPtr->transmitBuffer.count + recordHeaderLength + handshakeHeaderLength + TLS_CERTIFICATE_LENGTH_SIZE;
  uint16 totalCertSize = 0U;

 /* Construct certificate chain, but skip the last one i.e. the root */
  for(certIdx = 0U; (certIdx < (certChainLength - 1U)) && (status == E_OK); certIdx++)
  {
    KeyM_CertificateIdType certId = certIdPtr[certIdx];
    cert.certDataLength = ConnectionPtr->transmitBuffer.size - certOffset - totalCertSize - TLS_CERTIFICATE_LENGTH_SIZE;
    cert.certData = &ConnectionPtr->transmitBuffer.buffer[certOffset + totalCertSize + TLS_CERTIFICATE_LENGTH_SIZE];

    /* Copy certificate from KeyM */
    status = KeyM_GetCertificate(certId, &cert);

    if(status == E_OK)
    {
      TLS_SET24(&(ConnectionPtr->transmitBuffer.buffer[certOffset + totalCertSize]), 0U, cert.certDataLength);
      totalCertSize += cert.certDataLength + TLS_CERTIFICATE_LENGTH_SIZE;
    }
  }

  if(status == E_OK)
  {
    uint32 handshakeLength = totalCertSize + TLS_CERTIFICATE_LENGTH_SIZE;

    /* TODO: why only here? */
    TLS_ENTER_EXCLUSIVE_AREA_TX();

    /* Write record header */
    Tls_WriteRecordHeaderToTransmitBuffer(ConnectionPtr, ConnectionPtr->transmitBuffer.count, handshakeLength + handshakeHeaderLength, TLS_RECORD_HANDSHAKE);
    ConnectionPtr->transmitBuffer.count += recordHeaderLength;

    /* Write handshake header */
    Tls_Handshake_WriteHeader(ConnectionPtr, &ConnectionPtr->transmitBuffer.buffer[ConnectionPtr->transmitBuffer.count], TLS_HANDSHAKE_CERTIFICATE, handshakeLength);
    ConnectionPtr->transmitBuffer.count += handshakeHeaderLength;

    /* Write size of all certificates */
    TLS_SET24(&ConnectionPtr->transmitBuffer.buffer[ConnectionPtr->transmitBuffer.count], 0U, totalCertSize);
    ConnectionPtr->transmitBuffer.count += handshakeLength;

    TLS_EXIT_EXCLUSIVE_AREA_TX();

    /* TODO: should not include record header length */
    writtenSize = handshakeLength + handshakeHeaderLength;
  }
  else
  {
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
    writtenSize = 0U;
  }

  return writtenSize;
}

TS_MOD_PRIV_DEFN FUNC(uint32, TLS_CODE) Tls_Handshake_WriteServerCertificateRequest(Tls_PtrConnection ConnectionPtr)
{
  uint32 writtenSize;
  uint8 const handshakeHeaderLength = Tls_Handshake_GetHeaderSize(ConnectionPtr);
  uint8 const recordHeaderLength = Tls_GetRecordHeaderSize(ConnectionPtr);
  uint16 certReqOffset = ConnectionPtr->transmitBuffer.count + recordHeaderLength + handshakeHeaderLength;
  uint32 handshakeLength;

  /* Set length of CertificateRequest.certificate_types */
  ConnectionPtr->transmitBuffer.buffer[certReqOffset] = TLS_CERTIFICATE_REQUEST_CERT_TYPES_LENGTH_SIZE;
  certReqOffset += TLS_CERTIFICATE_REQUEST_CERT_TYPES_LENGTH_SIZE;

  /* Set CertificateRequest.certificate_types[0] to ecdsa_sign */
  ConnectionPtr->transmitBuffer.buffer[certReqOffset] = TLS_CLIENT_CERTIFICATE_TYPE_ECDSA_SIGN;
  certReqOffset += TLS_CERTIFICATE_REQUEST_CERT_TYPES_LENGTH_SIZE;

  /* Set length of CertificateRequest.signature_and_hash_algorithms */
  TLS_SET16(&(ConnectionPtr->transmitBuffer.buffer[certReqOffset]), 0U, TLS_CERTIFICATE_REQUEST_SUPP_SIG_ALGOS_LENGTH_SIZE);
  certReqOffset += TLS_CERTIFICATE_REQUEST_SUPP_SIG_ALGOS_LENGTH_SIZE;

  /* Set length of CertificateRequest.signature_and_hash_algorithms[0] to Ed25519 */
  TLS_SET16(&(ConnectionPtr->transmitBuffer.buffer[certReqOffset]), 0U, TLS_SIGNATURE_AND_HASH_ALGORITHM_ED25519);
  certReqOffset += TLS_CERTIFICATE_REQUEST_SUPP_SIG_ALGOS_LENGTH_SIZE;

  /* If a CA name is configured */
  if(ConnectionPtr->config->localCaCommonNameLength > 0U)
  {
    /* TODO: no hardcoded values */
    uint8 caDerLen = ConnectionPtr->config->localCaCommonNameLength + (6U * TLS_CERTIFICATE_REQUEST_CAS_LENGTH_SIZE) + 1U;
    uint16 distinguishedNameLen = caDerLen;
    uint16 distinguishedNamesLen = distinguishedNameLen + TLS_CERTIFICATE_REQUEST_CAS_LENGTH_SIZE;

    /* Set length of CertificateRequest.certificate_authorities */
    TLS_SET16(&(ConnectionPtr->transmitBuffer.buffer[certReqOffset]), 0U, distinguishedNamesLen);
    certReqOffset += TLS_CERTIFICATE_REQUEST_CAS_LENGTH_SIZE;

    /* Set length of CertificateRequest.certificate_authorities[0] */
    TLS_SET16(&(ConnectionPtr->transmitBuffer.buffer[certReqOffset]), 0U, caDerLen);
    certReqOffset += TLS_CERTIFICATE_REQUEST_CAS_LENGTH_SIZE;
    caDerLen -= TLS_CERTIFICATE_REQUEST_CAS_LENGTH_SIZE;

    /* Set sequence tag and length */
    ConnectionPtr->transmitBuffer.buffer[certReqOffset] = TLS_DER_SEQUENCE_TAG;
    ConnectionPtr->transmitBuffer.buffer[certReqOffset + 1U] = caDerLen;
    certReqOffset += TLS_DER_LENGTH_TAG_LENGTH;
    caDerLen -= TLS_DER_LENGTH_TAG_LENGTH;

    /* Set set tag and length */
    ConnectionPtr->transmitBuffer.buffer[certReqOffset] = TLS_DER_SET_TAG;
    ConnectionPtr->transmitBuffer.buffer[certReqOffset + 1U] = caDerLen;
    certReqOffset += TLS_DER_LENGTH_TAG_LENGTH;
    caDerLen -= TLS_DER_LENGTH_TAG_LENGTH;

    /* Set sequence tag and length */
    ConnectionPtr->transmitBuffer.buffer[certReqOffset] = TLS_DER_SEQUENCE_TAG;
    ConnectionPtr->transmitBuffer.buffer[certReqOffset + 1U] = caDerLen;
    certReqOffset += TLS_DER_LENGTH_TAG_LENGTH;
    caDerLen -= TLS_DER_LENGTH_TAG_LENGTH;

    /* Set object identifier tag, length and value */
    ConnectionPtr->transmitBuffer.buffer[certReqOffset] = TLS_DER_OBJECT_IDENTIFIER_TAG;
    ConnectionPtr->transmitBuffer.buffer[certReqOffset + 1U] = TLS_DER_OBJECT_IDENTIFIER_COMMON_NAME_LENGTH;
    certReqOffset += TLS_DER_LENGTH_TAG_LENGTH;
    TLS_SET24(&ConnectionPtr->transmitBuffer.buffer[certReqOffset], 0U, TLS_DER_OBJECT_IDENTIFIER_COMMON_NAME_VALUE);
    certReqOffset += TLS_DER_OBJECT_IDENTIFIER_COMMON_NAME_LENGTH;

    /* Set UTF8String tag, length and value */
    ConnectionPtr->transmitBuffer.buffer[certReqOffset] = TLS_DER_UTF8STRING_TAG;
    ConnectionPtr->transmitBuffer.buffer[certReqOffset + 1U] = ConnectionPtr->config->localCaCommonNameLength;
    certReqOffset += TLS_DER_LENGTH_TAG_LENGTH;
    TS_MemCpy(&ConnectionPtr->transmitBuffer.buffer[certReqOffset], ConnectionPtr->config->localCaCommonName, ConnectionPtr->config->localCaCommonNameLength);

    handshakeLength =
        2U * TLS_CERTIFICATE_REQUEST_CERT_TYPES_LENGTH_SIZE +
        2U * TLS_CERTIFICATE_REQUEST_SUPP_SIG_ALGOS_LENGTH_SIZE +
        TLS_CERTIFICATE_REQUEST_CAS_LENGTH_SIZE +
        distinguishedNamesLen;
  }
  /* If no CA name is configured */
  else
  {
    /* Set length of CertificateRequest.certificate_authorities */
    TLS_SET16(&(ConnectionPtr->transmitBuffer.buffer[certReqOffset]), 0U, 0U * TLS_CERTIFICATE_REQUEST_CAS_LENGTH_SIZE);
    certReqOffset += TLS_CERTIFICATE_REQUEST_CAS_LENGTH_SIZE;

    handshakeLength =
        2U * TLS_CERTIFICATE_REQUEST_CERT_TYPES_LENGTH_SIZE +
        2U * TLS_CERTIFICATE_REQUEST_SUPP_SIG_ALGOS_LENGTH_SIZE +
        TLS_CERTIFICATE_REQUEST_CAS_LENGTH_SIZE;
  }

  /* TODO: why only here? */
  TLS_ENTER_EXCLUSIVE_AREA_TX();

  /* Write record header */
  Tls_WriteRecordHeaderToTransmitBuffer(ConnectionPtr, ConnectionPtr->transmitBuffer.count, handshakeLength + handshakeHeaderLength, TLS_RECORD_HANDSHAKE);
  ConnectionPtr->transmitBuffer.count += recordHeaderLength;

  /* Write handshake header */
  Tls_Handshake_WriteHeader(ConnectionPtr, &ConnectionPtr->transmitBuffer.buffer[ConnectionPtr->transmitBuffer.count], TLS_HANDSHAKE_CERTIFICATEREQUEST, handshakeLength);
  ConnectionPtr->transmitBuffer.count += handshakeHeaderLength;

  TLS_EXIT_EXCLUSIVE_AREA_TX();

  /* TODO: should not include record header length */
  writtenSize = handshakeLength + handshakeHeaderLength;

  /* Update buffer count */
  ConnectionPtr->transmitBuffer.count += handshakeLength;

  return writtenSize;
}

TS_MOD_PRIV_DEFN FUNC(uint32, TLS_CODE) Tls_Handshake_WriteClientCertificateVerify(Tls_PtrConnection ConnectionPtr)
{
  uint32 writtenSize;
  uint8 const handshakeHeaderLength = Tls_Handshake_GetHeaderSize(ConnectionPtr);
  uint8 const recordHeaderLength = Tls_GetRecordHeaderSize(ConnectionPtr);
  uint16 certReqOffset = ConnectionPtr->transmitBuffer.count + recordHeaderLength + handshakeHeaderLength;
  uint32 handshakeLength;
  Std_ReturnType status;

  /* Set CertificateVerify.algorithm to Ed25519 */
  TLS_SET16(&(ConnectionPtr->transmitBuffer.buffer[certReqOffset]), 0U, TLS_SIGNATURE_AND_HASH_ALGORITHM_ED25519);
  certReqOffset += TLS_SIGNATURE_AND_HASH_ALGORITHM_LENGTH;

  /* Set length of CertificateVerify.signature */
  TLS_SET16(&(ConnectionPtr->transmitBuffer.buffer[certReqOffset]), 0U, TLS_SIGNATURE_ED25519_SIZE);
  certReqOffset += TLS_SIGNATURE_LENGTH_SIZE;

  /* Generate the signature */
  {
    uint32 sigJobId = ConnectionPtr->config->localCertSigGenerateJobId;
    uint32 sigOutLen = TLS_SIGNATURE_ED25519_SIZE;
    uint8 *sigInPtr = ConnectionPtr->handshakeData.handshakeMessages;
    uint32 sigInLen = ConnectionPtr->handshakeData.handshakeMessagesLength;

    /* TODO: support async crypto */
    status = Csm_SignatureGenerate(sigJobId, CRYPTO_OPERATIONMODE_SINGLECALL, sigInPtr, sigInLen, &(ConnectionPtr->transmitBuffer.buffer[certReqOffset]), &sigOutLen);
    /* TODO: postcondition assert for resultLength */
  }

  if(status == E_OK)
  {
    handshakeLength = TLS_SIGNATURE_AND_HASH_ALGORITHM_LENGTH + TLS_SIGNATURE_LENGTH_SIZE + TLS_SIGNATURE_ED25519_SIZE;

    /* TODO: why only here? */
    TLS_ENTER_EXCLUSIVE_AREA_TX();

    /* Write record header */
    Tls_WriteRecordHeaderToTransmitBuffer(ConnectionPtr, ConnectionPtr->transmitBuffer.count, handshakeLength + handshakeHeaderLength, TLS_RECORD_HANDSHAKE);
    ConnectionPtr->transmitBuffer.count += recordHeaderLength;

    /* Write handshake header */
    Tls_Handshake_WriteHeader(ConnectionPtr, &ConnectionPtr->transmitBuffer.buffer[ConnectionPtr->transmitBuffer.count], TLS_HANDSHAKE_CERTIFICATEVERIFY, handshakeLength);
    ConnectionPtr->transmitBuffer.count += handshakeHeaderLength;

    TLS_EXIT_EXCLUSIVE_AREA_TX();

    /* TODO: should not include record header length */
    writtenSize = handshakeLength + handshakeHeaderLength;
  }
  else
  {
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
    writtenSize = 0U;
  }

  return writtenSize;
}

TS_MOD_PRIV_DEFN FUNC(uint32, TLS_CODE) Tls_Handshake_GetSizeEcdhServerKeyExchange(void)
{
  uint32 handshakeLength =
      TLS_SERVER_KEY_EXCHANGE_EC_CURVE_TYPE_SIZE +
      TLS_SERVER_KEY_EXCHANGE_NAMED_CURVE_SIZE +
      TLS_SERVER_KEY_EXCHANGE_EC_POINT_LENGTH_SIZE +
      TLS_KEY_ED25519_SIZE +
      TLS_SIGNATURE_AND_HASH_ALGORITHM_LENGTH +
      TLS_SIGNATURE_LENGTH_SIZE +
      TLS_SIGNATURE_ED25519_SIZE;
  return handshakeLength;
}

TS_MOD_PRIV_DEFN FUNC(uint32, TLS_CODE) Tls_Handshake_WriteEcdhServerKeyExchange(Tls_PtrConnection ConnectionPtr)
{
  uint32 writtenSize;
  uint8 const handshakeHeaderLength = Tls_Handshake_GetHeaderSize(ConnectionPtr);
  uint8 const recordHeaderLength = Tls_GetRecordHeaderSize(ConnectionPtr);
  uint16 sKexOffset = ConnectionPtr->transmitBuffer.count + recordHeaderLength + handshakeHeaderLength;
  uint16 sKexIdxStart = sKexOffset;
  uint32 handshakeLength;
  Std_ReturnType status;

  ConnectionPtr->transmitBuffer.buffer[sKexOffset] = TLS_EC_CURVE_TYPE_NAMED_CURVE;
  sKexOffset += TLS_SERVER_KEY_EXCHANGE_EC_CURVE_TYPE_SIZE;

  TLS_SET16(&(ConnectionPtr->transmitBuffer.buffer[sKexOffset]), 0U, TLS_NAMED_CURVE_X25519);
  sKexOffset += TLS_SERVER_KEY_EXCHANGE_NAMED_CURVE_SIZE;

  ConnectionPtr->transmitBuffer.buffer[sKexOffset] = TLS_KEY_ED25519_SIZE;
  sKexOffset += TLS_SERVER_KEY_EXCHANGE_EC_POINT_LENGTH_SIZE;

  /* Generate the public key */
  {
    uint32 pubKeyLen = TLS_KEY_ED25519_SIZE;
    uint8 *pubKeyPtr = &(ConnectionPtr->transmitBuffer.buffer[sKexOffset]);

    /* Generate a new ephemeral private key. */
    status = Csm_KeyGenerate(ConnectionPtr->config->keyExchangeEphemeralGenerateKeyId);

    if(status == E_OK)
    {
      /* Copy the key element */
      status = Csm_KeyElementCopy
      (
        ConnectionPtr->config->keyExchangeEphemeralGenerateKeyId,
        CRYPTO_KE_KEYGENERATE_KEY,
        ConnectionPtr->config->keyExchangeEphemeralExchangeKeyId,
        CRYPTO_KE_KEYEXCHANGE_PRIVKEY
      );

      if(status == E_OK)
      {
        /* Set the copied key valid */
        status = Csm_KeySetValid(ConnectionPtr->config->keyExchangeEphemeralExchangeKeyId);

        if(status == E_OK)
        {
          ConnectionPtr->handshakeData.keCalcPubValLength = pubKeyLen;
          /* Retrieve the public key */
#if TLS_CFG_CSM_JOB_BASED_KEYEXCHANGE == STD_OFF
          status = Csm_KeyExchangeCalcPubVal(ConnectionPtr->config->keyExchangeEphemeralExchangeKeyId, pubKeyPtr, &ConnectionPtr->handshakeData.keCalcPubValLength);
#else
          /* TODO: Job Id as config param! */
          status = Csm_JobKeyExchangeCalcPubVal(ConnectionPtr->config->keyExchangeEphemeralExchangeKeyId, ConnectionPtr->config->keyExchangeEphemeralExchangeKeyId, pubKeyPtr, &pubKeyLen);
#endif
          sKexOffset += pubKeyLen;
          /* TODO: assert for pubKeyLen */
        }
      }
    }
  }

  /* Start the signature generation */
  if(status == E_OK)
  {
    /* Set ServerKeyExchange.signed_params.algorithm to Ed25519 */
    TLS_SET16(&(ConnectionPtr->transmitBuffer.buffer[sKexOffset]), 0U, TLS_SIGNATURE_AND_HASH_ALGORITHM_ED25519);
    sKexOffset += TLS_SIGNATURE_AND_HASH_ALGORITHM_LENGTH;

    /* Set length of ServerKeyExchange.signed_params.signature */
    TLS_SET16(&(ConnectionPtr->transmitBuffer.buffer[sKexOffset]), 0U, TLS_SIGNATURE_ED25519_SIZE);
    sKexOffset += TLS_SIGNATURE_LENGTH_SIZE;

    /* ClientHello.Random */
    {
      uint32 sigJobId = ConnectionPtr->config->localCertSigGenerateJobId;
      uint8 *sigOutPtr = &(ConnectionPtr->transmitBuffer.buffer[sKexOffset]);
      uint32 *sigOutLenPtr = &(ConnectionPtr->handshakeData.sigGenOutputLength);
      uint8 *sigInPtr = ConnectionPtr->handshakeData.clientRandom;
      uint32 sigInLen = TLS_RANDOM_SIZE;

      ConnectionPtr->handshakeData.sigGenInputDataPtr = &(ConnectionPtr->transmitBuffer.buffer[sKexIdxStart]);
      ConnectionPtr->handshakeData.sigGenInputLength = sKexOffset - sKexIdxStart - TLS_SIGNATURE_AND_HASH_ALGORITHM_LENGTH - TLS_SIGNATURE_LENGTH_SIZE;
      ConnectionPtr->handshakeData.sigGenOutputDataPtr = sigOutPtr;

      status = Csm_SignatureGenerate(sigJobId, CRYPTO_OPERATIONMODE_STREAMSTART, sigInPtr, sigInLen, sigOutPtr, sigOutLenPtr);
    }
  }

  if(status == E_OK)
  {
    handshakeLength = Tls_Handshake_GetSizeEcdhServerKeyExchange();

    /* TODO: why only here? */
    TLS_ENTER_EXCLUSIVE_AREA_TX();

    /* Write record header */
    Tls_WriteRecordHeaderToTransmitBuffer(ConnectionPtr, ConnectionPtr->transmitBuffer.count, handshakeLength + handshakeHeaderLength, TLS_RECORD_HANDSHAKE);
    ConnectionPtr->transmitBuffer.count += recordHeaderLength;

    /* Write handshake header */
    Tls_Handshake_WriteHeader(ConnectionPtr, &ConnectionPtr->transmitBuffer.buffer[ConnectionPtr->transmitBuffer.count], TLS_HANDSHAKE_SERVERKEYEXCHANGE, handshakeLength);
    ConnectionPtr->transmitBuffer.count += handshakeHeaderLength;

    TLS_EXIT_EXCLUSIVE_AREA_TX();

    writtenSize = handshakeLength + handshakeHeaderLength;

    /* TODO: check if also added to all other new functions */
    /* Update buffer count */
    ConnectionPtr->transmitBuffer.count += handshakeLength;
  }
  else
  {
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
    writtenSize = 0U;
  }

  return writtenSize;
}

TS_MOD_PRIV_DEFN FUNC(uint32, TLS_CODE) Tls_Handshake_WriteEcdhClientKeyExchange(Tls_PtrConnection ConnectionPtr)
{
  uint32 writtenSize;
  uint8 const handshakeHeaderLength = Tls_Handshake_GetHeaderSize(ConnectionPtr);
  uint8 const recordHeaderLength = Tls_GetRecordHeaderSize(ConnectionPtr);
  uint16 sKexOffset = ConnectionPtr->transmitBuffer.count + recordHeaderLength + handshakeHeaderLength;
  Std_ReturnType status;

  ConnectionPtr->transmitBuffer.buffer[sKexOffset] = TLS_KEY_ED25519_SIZE;
  sKexOffset += TLS_CLIENT_KEY_EXCHANGE_DH_PUBLIC_LENGTH_SIZE;

  /* Generate the public key */
  {
    uint32 pubKeyLen = TLS_KEY_ED25519_SIZE;
    uint8 *pubKeyPtr = &(ConnectionPtr->transmitBuffer.buffer[sKexOffset]);

    /* Generate a new ephemeral private key. */
    status = Csm_KeyGenerate(ConnectionPtr->config->keyExchangeEphemeralGenerateKeyId);

    if(status == E_OK)
    {
      /* Copy the key element */
      status = Csm_KeyElementCopy
      (
        ConnectionPtr->config->keyExchangeEphemeralGenerateKeyId,
        CRYPTO_KE_KEYGENERATE_KEY,
        ConnectionPtr->config->keyExchangeEphemeralExchangeKeyId,
        CRYPTO_KE_KEYEXCHANGE_PRIVKEY
      );

      if(status == E_OK)
      {
        /* Retrieve the public key */
        status = Csm_KeyExchangeCalcPubVal(ConnectionPtr->config->keyExchangeEphemeralExchangeKeyId, pubKeyPtr, &pubKeyLen);
        sKexOffset += pubKeyLen;
        /* TODO: assert for pubKeyLen */
      }
    }
  }

  if(status == E_OK)
  {
    uint32 handshakeLength =
        TLS_CLIENT_KEY_EXCHANGE_DH_PUBLIC_LENGTH_SIZE +
        TLS_KEY_ED25519_SIZE;

    /* TODO: why only here? */
    TLS_ENTER_EXCLUSIVE_AREA_TX();

    /* Write record header */
    Tls_WriteRecordHeaderToTransmitBuffer(ConnectionPtr, ConnectionPtr->transmitBuffer.count, handshakeLength + handshakeHeaderLength, TLS_RECORD_HANDSHAKE);
    ConnectionPtr->transmitBuffer.count += recordHeaderLength;

    /* Write handshake header */
    Tls_Handshake_WriteHeader(ConnectionPtr, &ConnectionPtr->transmitBuffer.buffer[ConnectionPtr->transmitBuffer.count], TLS_HANDSHAKE_CLIENTKEYEXCHANGE, handshakeLength);
    ConnectionPtr->transmitBuffer.count += handshakeHeaderLength;

    TLS_EXIT_EXCLUSIVE_AREA_TX();

    /* TODO: should not include record header length */
    writtenSize = handshakeLength + handshakeHeaderLength;
  }
  else
  {
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
    writtenSize = 0U;
  }

  return writtenSize;
}

#endif /* TLS_CERTIFICATE_SUPPORT == STD_ON */

#define TLS_STOP_SEC_CODE
#include <Tls_MemMap.h>

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
