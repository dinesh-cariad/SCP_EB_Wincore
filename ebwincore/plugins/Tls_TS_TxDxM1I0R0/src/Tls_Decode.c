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

#include <Tls_Decode_Int.h>

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#if TLS_CERTIFICATE_SUPPORT == STD_ON
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_KeyExchangeCalcSecret(Tls_PtrConnection ConnectionPtr, Tls_ConstUint8PtrType publicValuePtr, uint32 publicValueLength);
#endif /* TLS_CERTIFICATE_SUPPORT == STD_ON */

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#define TLS_START_SEC_CODE
#include <Tls_MemMap.h>

#if TLS_CERTIFICATE_SUPPORT == STD_ON
TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_HandleCertificate(Tls_PtrConnection ConnectionPtr, Tls_Uint8PtrType DataPtr, uint16 Length)
{
  Std_ReturnType retVal = E_OK;
  Tls_PtrUint8 payloadPtr;
  uint32 handshakeLength;
  uint8 handshakeHeaderSize = Tls_Handshake_GetHeaderSize(ConnectionPtr);
  uint32 payloadIdx = 0;
  uint32 certsLength;
  uint32 certLength;
  KeyM_CertDataType cert;
  uint8 certIdx;
  uint8 certChainLength = ConnectionPtr->config->remoteCertChainLength;
  KeyM_CertificateIdType const * certIdPtr = ConnectionPtr->config->remoteCertChainIdPtr;

  /* TODO: check Length >= minimum header size and >= length indicated in handshake header */
  TS_PARAM_UNUSED(Length);

  /* Get the length of payload from the header */
  handshakeLength = Tls_Handshake_GetLength(DataPtr);
  payloadPtr = &DataPtr[handshakeHeaderSize];

  /* Read all certificates length */
  certsLength = TLS_GET24(&payloadPtr[payloadIdx], 0U);
  payloadIdx += TLS_CERTIFICATE_LENGTH_SIZE;

  if(((certsLength + payloadIdx) > handshakeLength)  || (certsLength == 0))
  {
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_DECODE_ERROR);
    retVal = E_NOT_OK;
  }

  if(retVal == E_OK)
  {
    /* Iterate all certs up to a max of the chain length - 1 (to not overwrite root CA) */
    for(certIdx = 0U; (retVal == E_OK) && (certIdx < (certChainLength - 1U)); certIdx++)
    {
      /* TODO: length check before accessing cert length */

      /* Read certificate length */
      certLength = TLS_GET24(&payloadPtr[payloadIdx], 0U);
      payloadIdx += TLS_CERTIFICATE_LENGTH_SIZE;

      if(((certLength + payloadIdx) > handshakeLength) || (certLength == 0))
      {
        Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_DECODE_ERROR);
        retVal = E_NOT_OK;
      }
      else
      {
        cert.certDataLength = certLength;
        /* TODO: assignment discards constness */
        cert.certData =  &payloadPtr[payloadIdx];

        payloadIdx += certLength;

        /* Set the cert in KeyM */
        retVal = KeyM_SetCertificate(certIdPtr[certIdx], &cert);

        if(retVal != E_OK)
        {
          Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
          retVal = E_NOT_OK;
        }
      }
    }
  }

  if(retVal == E_OK)
  {
    /* Validate the whole certificate chain (always async) */
    retVal = KeyM_VerifyCertificate(certIdPtr[0U]);

    if (retVal == E_OK)
    {
      ConnectionPtr->handshakeData.verifyCertState = TLS_VERIFY_CERT_STATE_BUSY;
    }
    else
    {
      ConnectionPtr->handshakeData.verifyCertState = TLS_VERIFY_CERT_STATE_ERROR;
    }
  }

  return retVal;
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_HandleEcdhServerKeyExchange
(
  Tls_PtrConnection ConnectionPtr,
  Tls_PtrConstUint8 DataPtr,
  uint16 Length
)
{
  Std_ReturnType retVal = E_NOT_OK;
  Tls_PtrConstUint8 payloadPtr;
  uint32 handshakeLength;
  uint8 handshakeHeaderSize = Tls_Handshake_GetHeaderSize(ConnectionPtr);
  uint16 payloadIdx = 0U;
  uint16 paramsLength;

  if(Length > ((uint16) handshakeHeaderSize))
  {
    /* Get the length of payload from the header */
    handshakeLength = Tls_Handshake_GetLength(DataPtr);
    payloadPtr = &DataPtr[handshakeHeaderSize];

    if
    (
      ((handshakeHeaderSize + handshakeLength) == ((uint32) Length)) &&
      (handshakeLength > (TLS_SERVER_KEY_EXCHANGE_EC_CURVE_TYPE_SIZE + TLS_SERVER_KEY_EXCHANGE_NAMED_CURVE_SIZE + TLS_SERVER_KEY_EXCHANGE_EC_POINT_LENGTH_SIZE))
    )
    {
      /* Read params.curve_params.curve_type and params.curve_params.namedcurve */
      Tls_PtrConstUint8 paramsPtr = &payloadPtr[payloadIdx];
      uint8 curveType = payloadPtr[payloadIdx];
      uint16 namedCurve;
      payloadIdx += TLS_SERVER_KEY_EXCHANGE_EC_CURVE_TYPE_SIZE;
      namedCurve = TLS_GET16(&payloadPtr[payloadIdx]);
      payloadIdx += TLS_SERVER_KEY_EXCHANGE_NAMED_CURVE_SIZE;

      if((curveType == TLS_EC_CURVE_TYPE_NAMED_CURVE) && (namedCurve == TLS_NAMED_CURVE_X25519))
      {
        /* Read length of params.public */
        uint8 dhPublicLength = payloadPtr[payloadIdx];
        payloadIdx += TLS_SERVER_KEY_EXCHANGE_EC_POINT_LENGTH_SIZE;

        if(handshakeLength > payloadIdx + dhPublicLength)
        {
          if(dhPublicLength == TLS_KEY_ED25519_SIZE)
          {
            retVal = Csm_KeyExchangeCalcSecret(ConnectionPtr->config->keyExchangeEphemeralExchangeKeyId, &payloadPtr[payloadIdx], (uint32) dhPublicLength);
            payloadIdx += dhPublicLength;
            paramsLength = payloadIdx;
          }
        }
      }

      if(retVal == E_OK)
      {
        if(payloadIdx + TLS_SIGNATURE_AND_HASH_ALGORITHM_LENGTH < Length)
        {
          /* Parse algorithm */
          uint16 algorithm = TLS_GET16(&payloadPtr[payloadIdx]);
          payloadIdx += TLS_SIGNATURE_AND_HASH_ALGORITHM_LENGTH;

          if(algorithm == TLS_SIGNATURE_AND_HASH_ALGORITHM_ED25519)
          {
            if(payloadIdx + TLS_SIGNATURE_LENGTH_SIZE <= Length)
            {
              /* Parse length of signature */
              uint16 sigLength = TLS_GET16(&payloadPtr[payloadIdx]);
              payloadIdx += TLS_SIGNATURE_LENGTH_SIZE;

              if((sigLength == TLS_SIGNATURE_ED25519_SIZE) && (handshakeHeaderSize + payloadIdx + sigLength == Length))
              {
                /* Verify signature */
                /* TODO: consider async crypto */
                Crypto_VerifyResultType verifyResult;
                uint32 jobId = ConnectionPtr->config->remoteCertSigVerifyJobId;
                uint8 *messageInPtr = ConnectionPtr->handshakeData.clientRandom;
                uint16 messageLength = TLS_RANDOM_SIZE;
                uint8 const *currentPayloadPtr = &payloadPtr[payloadIdx];
                retVal = Csm_SignatureVerify(jobId, CRYPTO_OPERATIONMODE_STREAMSTART, messageInPtr, messageLength, currentPayloadPtr, sigLength, &verifyResult);

                if(retVal == E_OK)
                {
                  messageInPtr = ConnectionPtr->handshakeData.serverRandom;
                  messageLength = TLS_RANDOM_SIZE;
                  retVal = Csm_SignatureVerify(jobId, CRYPTO_OPERATIONMODE_UPDATE, messageInPtr, messageLength, currentPayloadPtr, sigLength, &verifyResult);
                  if(retVal == E_OK)
                  {
                    retVal = Csm_SignatureVerify(jobId, CRYPTO_OPERATIONMODE_UPDATE, paramsPtr, paramsLength, currentPayloadPtr, sigLength, &verifyResult);

                    if(retVal == E_OK)
                    {
                      retVal = Csm_SignatureVerify(jobId, CRYPTO_OPERATIONMODE_FINISH, paramsPtr, 0U, currentPayloadPtr, sigLength, &verifyResult);

                      if(verifyResult != CRYPTO_E_VER_OK)
                      {
                        retVal = E_NOT_OK;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  if(retVal != E_OK)
  {
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
    retVal = E_NOT_OK;
  }

  return retVal;
}

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_KeyExchangeCalcSecret(Tls_PtrConnection ConnectionPtr, Tls_ConstUint8PtrType publicValuePtr, uint32 publicValueLength)
{
  Std_ReturnType retVal;
#if TLS_CFG_CSM_JOB_BASED_KEYEXCHANGE == STD_ON
  /* TODO: get job id from config */
  retVal = Csm_JobKeyExchangeCalcSecret(ConnectionPtr->config->keyExchangeEphemeralExchangeKeyId, ConnectionPtr->config->keyExchangeEphemeralExchangeKeyId, publicValuePtr, publicValueLength);
#else
  retVal = Csm_KeyExchangeCalcSecret(ConnectionPtr->config->keyExchangeEphemeralExchangeKeyId, publicValuePtr, publicValueLength);
#endif
  return retVal;
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_HandleEcdhClientKeyExchange
(
  Tls_PtrConnection ConnectionPtr,
  Tls_PtrConstUint8 DataPtr,
  uint16 Length
)
{
  Std_ReturnType retVal = E_NOT_OK;
  Tls_PtrConstUint8 payloadPtr;
  uint32 handshakeLength;
  uint8 handshakeHeaderSize = Tls_Handshake_GetHeaderSize(ConnectionPtr);
  uint32 payloadIdx = 0U;
  uint8 dhPublicLength;

  if(Length > handshakeHeaderSize)
  {
    /* Get the length of payload from the header */
    handshakeLength = Tls_Handshake_GetLength(DataPtr);
    payloadPtr = &DataPtr[handshakeHeaderSize];

    {
      boolean hasHandshakeCorrectLength = handshakeHeaderSize + handshakeLength == Length;
      boolean isPubKeyLengthWithinBounds = handshakeHeaderSize > TLS_CLIENT_KEY_EXCHANGE_DH_PUBLIC_LENGTH_SIZE;
      boolean isPublicKeyLengthReadable = hasHandshakeCorrectLength && isPubKeyLengthWithinBounds;
      if(isPublicKeyLengthReadable)
      {
        /* Read length of dh_public */
        dhPublicLength = payloadPtr[payloadIdx];
        payloadIdx += TLS_CLIENT_KEY_EXCHANGE_DH_PUBLIC_LENGTH_SIZE;
        {
          boolean isPubKeyEqualToBounds = (handshakeHeaderSize + dhPublicLength + TLS_CLIENT_KEY_EXCHANGE_DH_PUBLIC_LENGTH_SIZE) == Length;
          boolean hasPublicKeyCorrectLength = dhPublicLength == TLS_KEY_ED25519_SIZE;
          boolean isPublicKeyReadable = isPubKeyEqualToBounds && hasPublicKeyCorrectLength;
          if(isPublicKeyReadable)
          {
            /* Calculate the shared secret */
            retVal = Tls_Handshake_KeyExchangeCalcSecret(ConnectionPtr, &payloadPtr[payloadIdx], dhPublicLength);
          }
        }
      }
    }
  }

  if(retVal != E_OK)
  {
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
    retVal = E_NOT_OK;
  }

  return retVal;
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_HandleCertificateRequest
(
  Tls_PtrConnection ConnectionPtr,
  Tls_PtrConstUint8 DataPtr,
  uint16 Length
)
{
  Std_ReturnType retVal = E_NOT_OK;
  uint32 handshakeLength;
  uint8 handshakeHeaderSize = Tls_Handshake_GetHeaderSize(ConnectionPtr);
  uint16 payloadIdx = 0U;

  if(Length > handshakeHeaderSize)
  {
    /* Get the length of payload from the header */
    handshakeLength = Tls_Handshake_GetLength(DataPtr);
    payloadIdx += handshakeHeaderSize;

    if((payloadIdx + handshakeLength) == Length)
    {
      uint8 certTypesLen = DataPtr[payloadIdx];
      uint8 certTypeIdx;
      payloadIdx += TLS_CERTIFICATE_REQUEST_CERT_TYPES_LENGTH_SIZE;

      if((payloadIdx + certTypesLen) <= Length)
      {
        /* Iterate all entries of certificate_types */
        for(certTypeIdx = 0U; certTypeIdx < certTypesLen; certTypeIdx++)
        {
          uint8 certType = DataPtr[payloadIdx + certTypeIdx];
          if(certType == TLS_CLIENT_CERTIFICATE_TYPE_ECDSA_SIGN)
          {
            break;
          }
        }

        /* If a match was found */
        if(certTypeIdx < certTypesLen)
        {
          payloadIdx += certTypesLen;

          if((payloadIdx + TLS_CERTIFICATE_REQUEST_SUPP_SIG_ALGOS_LENGTH_SIZE) < Length)
          {
            uint16 sigAlgosLen = TLS_GET16(&DataPtr[payloadIdx]);
            uint16 sigAlgoIdx;
            payloadIdx += TLS_CERTIFICATE_REQUEST_SUPP_SIG_ALGOS_LENGTH_SIZE;

            if((payloadIdx + sigAlgosLen) <= Length)
            {
              /* Iterate all entries of supported_signature_algorithms */
              for(sigAlgoIdx = 0U; sigAlgoIdx < sigAlgosLen; sigAlgoIdx += TLS_CERTIFICATE_REQUEST_SUPP_SIG_ALGOS_LENGTH_SIZE)
              {
                uint16 sigAlgo = TLS_GET16(&DataPtr[payloadIdx + sigAlgoIdx]);
                if(sigAlgo == TLS_SIGNATURE_AND_HASH_ALGORITHM_ED25519)
                {
                  break;
                }
              }

              /* If a match was found */
              if(sigAlgoIdx < sigAlgosLen)
              {
                /* TODO: parse certificate_authorities */
                retVal = E_OK;
              }
            }
          }
        }
      }
    }
  }

  if(retVal != E_OK)
  {
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
    retVal = E_NOT_OK;
  }

  return retVal;
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_HandleCertificateVerify
(
  Tls_PtrConnection ConnectionPtr,
  Tls_PtrConstUint8 DataPtr,
  uint16 Length
)
{
  Std_ReturnType retVal = E_NOT_OK;
  uint32 handshakeLength;
  uint8 handshakeHeaderSize = Tls_Handshake_GetHeaderSize(ConnectionPtr);
  uint16 payloadIdx = 0U;

  if(Length > handshakeHeaderSize)
  {
    /* Get the length of payload from the header */
    handshakeLength = Tls_Handshake_GetLength(DataPtr);
    payloadIdx += handshakeHeaderSize;

    if((handshakeHeaderSize + handshakeLength) == Length)
    {
      if((payloadIdx + TLS_SIGNATURE_AND_HASH_ALGORITHM_LENGTH) < Length)
      {
        /* Parse algorithm */
        uint16 algorithm = TLS_GET16(&DataPtr[payloadIdx]);
        payloadIdx += TLS_SIGNATURE_AND_HASH_ALGORITHM_LENGTH;

        if(algorithm == TLS_SIGNATURE_AND_HASH_ALGORITHM_ED25519)
        {
          if((payloadIdx + TLS_SIGNATURE_LENGTH_SIZE) <= Length)
          {
            /* Parse length of signature */
            uint16 sigLength = TLS_GET16(&DataPtr[payloadIdx]);
            payloadIdx += TLS_SIGNATURE_LENGTH_SIZE;

            if((sigLength == TLS_SIGNATURE_ED25519_SIZE) && ((payloadIdx + sigLength) == Length))
            {
              /* Verify signature */
              uint32 jobId = ConnectionPtr->config->remoteCertSigVerifyJobId;
              uint8 *messagesPtr = ConnectionPtr->handshakeData.handshakeMessages;
              uint16 messagesLength = ConnectionPtr->handshakeData.handshakeMessagesLength;
              retVal = Csm_SignatureVerify(jobId, CRYPTO_OPERATIONMODE_SINGLECALL, messagesPtr, messagesLength, &DataPtr[payloadIdx], sigLength, &(ConnectionPtr->handshakeData.sigVerifyResult));
            }
          }
        }
      }
    }
  }

  if(retVal != E_OK)
  {
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
    retVal = E_NOT_OK;
  }

  return retVal;
}

#endif /* TLS_CERTIFICATE_SUPPORT == STD_ON */

#define TLS_STOP_SEC_CODE
#include <Tls_MemMap.h>

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
