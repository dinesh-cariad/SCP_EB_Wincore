/**
 * \file
 *
 * \brief AUTOSAR Csm
 *
 * This file contains the implementation of the AUTOSAR
 * module Csm.
 *
 * \version 3.1.13
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef CSM_H
#define CSM_H

/* !LINKSTO CSM.Dsn.SWS_Csm_00005,1 */

/*==================[misra deviations]============================================================*/

/*==================[includes]====================================================================*/

#include <Csm_Cfg.h>
#include <Csm_Cbk.h>

/*==================[macros]======================================================================*/

/* ServiceIds */
/** \brief  The 'Csm_AEADDecrypt' API service identifier
 **/
#if (defined CSM_SID_AEADDECRYPT)
  #error CSM_SID_AEADDECRYPT is already defined
#endif
#define  CSM_SID_AEADDECRYPT            0x0063U

/** \brief  The 'Csm_AEADEncrypt' API service identifier
 **/
#if (defined CSM_SID_AEADENCRYPT)
  #error CSM_SID_AEADENCRYPT is already defined
#endif
#define  CSM_SID_AEADENCRYPT            0x0062U

/** \brief  The 'Csm_CallbackNotification' API service identifier
 **/
#if (defined CSM_SID_CALLBACKNOTIFICATION)
  #error CSM_SID_CALLBACKNOTIFICATION is already defined
#endif
#define  CSM_SID_CALLBACKNOTIFICATION   0x0070U

/** \brief  The 'Csm_CancelJob' API service identifier
 **/
#if (defined CSM_SID_CANCELJOB)
  #error CSM_SID_CANCELJOB is already defined
#endif
#define  CSM_SID_CANCELJOB              0x006FU

/** \brief  The 'Csm_CertificateParse' API service identifier
 **/
#if (defined CSM_SID_CERTIFICATEPARSE)
  #error CSM_SID_CERTIFICATEPARSE is already defined
#endif
#define  CSM_SID_CERTIFICATEPARSE       0x006EU

/** \brief  The 'Csm_CertificateVerify' API service identifier
 **/
#if (defined CSM_SID_CERTIFICATEVERIFY)
  #error CSM_SID_CERTIFICATEVERIFY is already defined
#endif
#define  CSM_SID_CERTIFICATEVERIFY      0x0074U

/** \brief  The 'Csm_Decrypt' API service identifier
 **/
#if (defined CSM_SID_DECRYPT)
  #error CSM_SID_DECRYPT is already defined
#endif
#define  CSM_SID_DECRYPT                0x005FU

/** \brief  The 'Csm_Encrypt' API service identifier
 **/
#if (defined CSM_SID_ENCRYPT)
  #error CSM_SID_ENCRYPT is already defined
#endif
#define  CSM_SID_ENCRYPT                0x005EU

/** \brief  The 'Csm_GetVersionInfo' API service identifier
 **/
#if (defined CSM_SID_GETVERSIONINFO)
  #error CSM_SID_GETVERSIONINFO is already defined
#endif
#define  CSM_SID_GETVERSIONINFO         0x003BU

/** \brief  The 'Csm_Hash' API service identifier
 **/
#if (defined CSM_SID_HASH)
  #error CSM_SID_HASH is already defined
#endif
#define  CSM_SID_HASH                   0x005DU

/** \brief  The 'Csm_Init' API service identifier
 **/
#if (defined CSM_SID_INIT)
  #error CSM_SID_INIT is already defined
#endif
#define  CSM_SID_INIT                   0x0000U

/** \brief  The 'Csm_KeyCopy' API service identifier
 **/
#if (defined CSM_SID_KEYCOPY)
  #error CSM_SID_KEYCOPY is already defined
#endif
#define  CSM_SID_KEYCOPY                0x0073U

/** \brief  The 'Csm_KeyDerive' API service identifier
 **/
#if (defined CSM_SID_KEYDERIVE)
  #error CSM_SID_KEYDERIVE is already defined
#endif
#define  CSM_SID_KEYDERIVE              0x006BU

/** \brief  The 'Csm_KeyElementCopy' API service identifier
 **/
#if (defined CSM_SID_KEYELEMENTCOPY)
  #error CSM_SID_KEYELEMENTCOPY is already defined
#endif
#define  CSM_SID_KEYELEMENTCOPY         0x0071U

/** \brief  The 'Csm_KeyElementGet' API service identifier
 **/
#if (defined CSM_SID_KEYELEMENTGET)
  #error CSM_SID_KEYELEMENTGET is already defined
#endif
#define  CSM_SID_KEYELEMENTGET          0x0068U

/** \brief  The 'Csm_KeyElementSet' API service identifier
 **/
#if (defined CSM_SID_KEYELEMENTSET)
  #error CSM_SID_KEYELEMENTSET is already defined
#endif
#define  CSM_SID_KEYELEMENTSET          0x0078U

/** \brief  The 'Csm_KeyExchangeCalcPubVal' API service identifier
 **/
#if (defined CSM_SID_KEYEXCHANGECALCPUBVAL)
  #error CSM_SID_KEYEXCHANGECALCPUBVAL is already defined
#endif
#define  CSM_SID_KEYEXCHANGECALCPUBVAL  0x006CU

/** \brief  The 'Csm_KeyExchangeCalcSecret' API service identifier
 **/
#if (defined CSM_SID_KEYEXCHANGECALCSECRET)
  #error CSM_SID_KEYEXCHANGECALCSECRET is already defined
#endif
#define  CSM_SID_KEYEXCHANGECALCSECRET  0x006DU

/** \brief  The 'Csm_KeyGenerate' API service identifier
 **/
#if (defined CSM_SID_KEYGENERATE)
  #error CSM_SID_KEYGENERATE is already defined
#endif
#define  CSM_SID_KEYGENERATE            0x006AU

/** \brief  The 'Csm_KeySetValid' API service identifier
 **/
#if (defined CSM_SID_KEYSETVALID)
  #error CSM_SID_KEYSETVALID is already defined
#endif
#define  CSM_SID_KEYSETVALID            0x0067U

/** \brief  The 'Csm_MacGenerate' API service identifier
 **/
#if (defined CSM_SID_MACGENERATE)
  #error CSM_SID_MACGENERATE is already defined
#endif
#define  CSM_SID_MACGENERATE            0x0060U

/** \brief  The 'Csm_MacVerify' API service identifier
 **/
#if (defined CSM_SID_MACVERIFY)
  #error CSM_SID_MACVERIFY is already defined
#endif
#define  CSM_SID_MACVERIFY              0x0061U

/** \brief  The 'Csm_MainFunction' API service identifier
 **/
#if (defined CSM_SID_MAINFUNCTION)
  #error CSM_SID_MAINFUNCTION is already defined
#endif
#define  CSM_SID_MAINFUNCTION           0x0001U

/** \brief  The 'Csm_RandomGenerate' API service identifier
 **/
#if (defined CSM_SID_RANDOMGENERATE)
  #error CSM_SID_RANDOMGENERATE is already defined
#endif
#define  CSM_SID_RANDOMGENERATE         0x0072U

/** \brief  The 'Csm_RandomSeed' API service identifier
 **/
#if (defined CSM_SID_RANDOMSEED)
  #error CSM_SID_RANDOMSEED is already defined
#endif
#define  CSM_SID_RANDOMSEED             0x0069U

/** \brief  The 'Csm_SignatureGenerate' API service identifier
 **/
#if (defined CSM_SID_SIGNATUREGENERATE)
  #error CSM_SID_SIGNATUREGENERATE is already defined
#endif
#define  CSM_SID_SIGNATUREGENERATE      0x0076U

/** \brief  The 'Csm_SignatureVerify' API service identifier
 **/
#if (defined CSM_SID_SIGNATUREVERIFY)
  #error CSM_SID_SIGNATUREVERIFY is already defined
#endif
#define  CSM_SID_SIGNATUREVERIFY        0x0064U

/* --- DevelopmentErrors --- */
/* !LINKSTO SWS_Csm_91004,1 */

/** \brief  Development Error to be raised if API request called with invalid parameter
 **         (Nullpointer)
 **/
#if (defined CSM_E_PARAM_POINTER)
  #error CSM_E_PARAM_POINTER is already defined
#endif
#define  CSM_E_PARAM_POINTER          0x00001U

/** \brief  Development Error to be raised if API request called before initialization of
 **         Csm module
 **/
#if (defined CSM_E_UNINIT)
  #error CSM_E_UNINIT is already defined
#endif
#define  CSM_E_UNINIT                 0x00005U

/** \brief  Development Error to be raised if initialization of Csm module failed
 **/
#if (defined CSM_E_INIT_FAILED)
  #error CSM_E_INIT_FAILED is already defined
#endif
#define  CSM_E_INIT_FAILED            0x00007U

/** \brief  Development Error to be raised if requested service is not initialized
 **/
#if (defined CSM_E_SERVICE_NOT_STARTED)
  #error CSM_E_SERVICE_NOT_STARTED is already defined
#endif
#define  CSM_E_SERVICE_NOT_STARTED    0x00009U

/** \brief  Development Error to be raised if keyId or jobId of requested service is out of range
 **/
#if (defined CSM_E_PARAM_HANDLE)
  #error CSM_E_PARAM_HANDLE is already defined
#endif
#define  CSM_E_PARAM_HANDLE           0x00004U

/** \brief  Development Error to be raised if service of the job referenced by jobId did not match
 **         the service designated by the API function
 **/
/* !LINKSTO CSM.Req.Gen/Det/00003,1 */
#if (defined CSM_E_SERVICE_NOT_IDENTICAL)
  #error CSM_E_SERVICE_NOT_IDENTICAL is already defined
#endif
#define  CSM_E_SERVICE_NOT_IDENTICAL  0x000E1U

/*==================[type definitions]============================================================*/

/*==================[external constants]==========================================================*/

/*==================[external data]===============================================================*/

/*==================[external function declarations]==============================================*/

#define CSM_START_SEC_CODE
#include <Csm_MemMap.h>

#if ((CSM_API_ENABLED_SERVICE_AEADDECRYPT == STD_ON) && ((CSM_RTE_ENABLED_SERVICE_AEADDECRYPT == STD_OFF) || (!defined(CSM_SRC))))
#ifndef RTE_SCHM_SWCBSW_CSM_AEADDECRYPT
#define RTE_SCHM_SWCBSW_CSM_AEADDECRYPT

/** \brief  Uses the given data to perform an AEAD encryption and stores the ciphertext and the
 **         MAC in the memory locations pointed by the ciphertext pointer and Tag pointer.
 **
 ** \param[in]      jobId                 Holds the identifier of the job using the CSM
 **                                       service.
 ** \param[in]      mode                  Indicates which operation mode(s) to perfom.
 ** \param[in]      ciphertextPtr         Contains the pointer to the data to be decrypted.
 ** \param[in]      ciphertextLength      Contains the number of bytes to decrypt.
 ** \param[in]      associatedDataPtr     Contains the pointer to the associated data.
 ** \param[in]      associatedDataLength  Contains the length in bytes of the associated data.
 ** \param[in]      tagPtr                Contains the pointer to the Tag to be verified.
 ** \param[in]      tagLength             Contains the length in bytes of the Tag to be
 **                                       verified.
 ** \param[out]     plaintextPtr          Contains the pointer to the data where the decrypted
 **                                       data shall be stored.
 ** \param[in,out]  plaintextLengthPtr    Holds a pointer to the memory location in which the
 **                                       output length in bytes of the paintext is stored. On
 **                                       calling this function, this parameter shall contain
 **                                       the size of the buffer provided by plaintextPtr. When
 **                                       the request has finished, the actual length of the
 **                                       returned value shall be stored.
 ** \param[out]     verifyPtr             Contains the pointer to the result of the
 **                                       verification.
 **
 ** \returns  Error value.
 **
 ** \retval   E_OK                    request successful
 ** \retval   E_NOT_OK                request failed
 ** \retval   CRYPTO_E_BUSY           request failed, service is still busy
 ** \retval   CRYPTO_E_QUEUE_FULL     request failed, the queue is full
 ** \retval   CRYPTO_E_KEY_NOT_VALID  request failed, the key's state is 'invalid'
 **
 ** \ServiceID{::CSM_SID_AEADDECRYPT}
 ** \Synchronicity{Sync or Async, dependend on the job configuration}
 ** \Reentrancy{Reentrant}
 **/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_AEADDecrypt
(
          uint32                                              jobId,
          Crypto_OperationModeType                            mode,
  P2CONST(uint8,                    AUTOMATIC, CSM_APPL_DATA) ciphertextPtr,
          uint32                                              ciphertextLength,
  P2CONST(uint8,                    AUTOMATIC, CSM_APPL_DATA) associatedDataPtr,
          uint32                                              associatedDataLength,
  P2CONST(uint8,                    AUTOMATIC, CSM_APPL_DATA) tagPtr,
          uint32                                              tagLength,
    P2VAR(uint8,                    AUTOMATIC, CSM_APPL_DATA) plaintextPtr,
    P2VAR(uint32,                   AUTOMATIC, CSM_APPL_DATA) plaintextLengthPtr,
    P2VAR(Crypto_VerifyResultType,  AUTOMATIC, CSM_APPL_DATA) verifyPtr
);

#endif /* #ifndef RTE_SCHM_SWCBSW_CSM_AEADDECRYPT */
#endif /* #if ((CSM_API_ENABLED_SERVICE_AEADDECRYPT == STD_ON) && ((CSM_RTE_ENABLED_SERVICE_AEADDECRYPT == STD_OFF) || (!defined(CSM_SRC)))) */

#if ((CSM_API_ENABLED_SERVICE_AEADENCRYPT == STD_ON) && ((CSM_RTE_ENABLED_SERVICE_AEADENCRYPT == STD_OFF) || (!defined(CSM_SRC))))
#ifndef RTE_SCHM_SWCBSW_CSM_AEADENCRYPT
#define RTE_SCHM_SWCBSW_CSM_AEADENCRYPT

/** \brief  Uses the given input data to perform a AEAD encryption and stores the ciphertext
 **         and the MAC in the memory locations pointed by the ciphertext pointer and Tag
 **         pointer.
 **
 ** \param[in]      jobId                 Holds the identifier of the job using the CSM
 **                                       service.
 ** \param[in]      mode                  Indicates which operation mode(s) to perfom.
 ** \param[in]      plaintextPtr          Contains the pointer to the data to be encrypted.
 ** \param[in]      plaintextLength       Contains the number of bytes to encrypt.
 ** \param[in]      associatedDataPtr     Contains the pointer to the associated data.
 ** \param[in]      associatedDataLength  Contains the number of bytes of the associated data.
 ** \param[out]     ciphertextPtr         Contains the pointer to the data where the encrypted
 **                                       data shall be stored.
 ** \param[in,out]  ciphertextLengthPtr   Holds a pointer to the memory location in which the
 **                                       output length in bytes of the ciphertext is stored.
 **                                       On calling this function, this parameter shall
 **                                       contain the size of the buffer in bytes provided by
 **                                       resultPtr. When the request has finished, the actual
 **                                       length of the returned value shall be stored.
 ** \param[out]     tagPtr                Contains the pointer to the data where the Tag shall
 **                                       be stored.
 ** \param[in,out]  tagLengthPtr          Holds a pointer to the memory location in which the
 **                                       output length in bytes of the Tag is stored. On
 **                                       calling this function, this parameter shall contain
 **                                       the size of the buffer in bytes provided by
 **                                       resultPtr. When the request has finished, the actual
 **                                       length of the returned value shall be stored.
 **
 ** \returns  Error value.
 **
 ** \retval   E_OK                    request successful
 ** \retval   E_NOT_OK                request failed
 ** \retval   CRYPTO_E_BUSY           request failed, service is still busy
 ** \retval   CRYPTO_E_QUEUE_FULL     request failed, the queue is full
 ** \retval   CRYPTO_E_KEY_NOT_VALID  request failed, the key's state is 'invalid'
 **
 ** \ServiceID{::CSM_SID_AEADENCRYPT}
 ** \Synchronicity{Sync or Async, dependend on the job configuration}
 ** \Reentrancy{Reentrant}
 **/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_AEADEncrypt
(
          uint32                                              jobId,
          Crypto_OperationModeType                            mode,
  P2CONST(uint8,                    AUTOMATIC, CSM_APPL_DATA) plaintextPtr,
          uint32                                              plaintextLength,
  P2CONST(uint8,                    AUTOMATIC, CSM_APPL_DATA) associatedDataPtr,
          uint32                                              associatedDataLength,
    P2VAR(uint8,                    AUTOMATIC, CSM_APPL_DATA) ciphertextPtr,
    P2VAR(uint32,                   AUTOMATIC, CSM_APPL_DATA) ciphertextLengthPtr,
    P2VAR(uint8,                    AUTOMATIC, CSM_APPL_DATA) tagPtr,
    P2VAR(uint32,                   AUTOMATIC, CSM_APPL_DATA) tagLengthPtr
);

#endif /* #ifndef RTE_SCHM_SWCBSW_CSM_AEADENCRYPT */
#endif /* #if ((CSM_API_ENABLED_SERVICE_AEADENCRYPT == STD_ON) && ((CSM_RTE_ENABLED_SERVICE_AEADENCRYPT == STD_OFF) || (!defined(CSM_SRC)))) */

#if ((CSM_API_ENABLED_SERVICE_GENERAL == STD_ON)&& ((CSM_RTE_ENABLED_SERVICE_GENERAL == STD_OFF) || (!defined(CSM_SRC))))
#ifndef RTE_SCHM_SWCBSW_CSM_CANCELJOB
#define RTE_SCHM_SWCBSW_CSM_CANCELJOB

/** \brief  Removes the job in the Csm Queue and calls the job's callback with the result
 **         CRYPTO_E_JOB_CANCELED. It also passes the cancellation command to the CryIf to try
 **         to cancel the job in the Crypto Driver.
 **
 ** \param[in]  job   Holds the identifier of the job to be canceled.
 ** \param[in]  mode  Not used, just for interface compatibility provided.
 **
 ** \returns  Error value.
 **
 ** \retval   E_OK      request successful
 ** \retval   E_NOT_OK  request failed
 **
 ** \ServiceID{::CSM_SID_CANCELJOB}
 ** \Synchronicity{Synchronous}
 ** \Reentrancy{Non Reentrant}
 **/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_CancelJob
(
  uint32                   job,
  Crypto_OperationModeType mode
);

#endif /* #ifndef RTE_SCHM_SWCBSW_CSM_CANCELJOB */
#endif /* #if ((CSM_API_ENABLED_SERVICE_GENERAL == STD_ON)&& ((CSM_RTE_ENABLED_SERVICE_GENERAL == STD_OFF) || (!defined(CSM_SRC)))) */

#if ((CSM_API_ENABLED_KEYMNGMNT == STD_ON) && ((CSM_RTE_ENABLED_KEYMNGMNT == STD_OFF) || (!defined(CSM_SRC))))
#ifndef RTE_SCHM_SWCBSW_CSM_CERTIFICATEPARSE
#define RTE_SCHM_SWCBSW_CSM_CERTIFICATEPARSE

/** \brief  This function shall dispatch the certificate parse function to the CRYIF.
 **
 ** \param[in]  keyId  Holds the identifier of the key to be used for the certificate parsing.
 **
 ** \returns  Error value.
 **
 ** \retval   E_OK      Request successful
 ** \retval   E_NOT_OK  Request Failed
 **
 ** \ServiceID{::CSM_SID_CERTIFICATEPARSE}
 ** \Synchronicity{Synchronous}
 ** \Reentrancy{Reentrant, but not for same keyId}
 **/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_CertificateParse
(
  uint32 keyId
);

#endif /* #ifndef RTE_SCHM_SWCBSW_CSM_CERTIFICATEPARSE */
#endif /* #if ((CSM_API_ENABLED_KEYMNGMNT == STD_ON) && ((CSM_RTE_ENABLED_KEYMNGMNT == STD_OFF) || (!defined(CSM_SRC)))) */

#if ((CSM_API_ENABLED_KEYMNGMNT == STD_ON) && ((CSM_RTE_ENABLED_KEYMNGMNT == STD_OFF) || (!defined(CSM_SRC))))
#ifndef RTE_SCHM_SWCBSW_CSM_CERTIFICATEVERIFY
#define RTE_SCHM_SWCBSW_CSM_CERTIFICATEVERIFY

/** \brief  Verifies the certificate stored in the key referenced by verifyKeyId with the
 **         certificate stored in the key referenced by keyId. Note: Only certificates stored
 **         in the same Crypto Driver can be verified against each other. If the key element
 **         CRYPTO_KE_CERTIFICATE_CURRENT_TIME is used for the verification of the validity
 **         period of the certificate indentified by verifyKeyId, it shall have the same format
 **         as the timestamp in the certificate.
 **
 ** \param[in]   keyId             Holds the identifier of the key which shall be used to
 **                                validate the certificate.
 ** \param[in]   verifyCryIfKeyId  Holds the identifier of the key containing the certificate
 **                                to be verified.
 ** \param[out]  verifyPtr         Holds a pointer to the memory location which will contain
 **                                the result of the certificate verification.
 **
 ** \returns  Error value.
 **
 ** \retval   E_OK      Request successful
 ** \retval   E_NOT_OK  Request Failed
 **
 ** \ServiceID{::CSM_SID_CERTIFICATEVERIFY}
 ** \Synchronicity{Synchronous}
 ** \Reentrancy{Reentrant but not for the same cryptoKeyId}
 **/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_CertificateVerify
(
        uint32                                             keyId,
        uint32                                             verifyCryIfKeyId,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_DATA) verifyPtr
);

#endif /* #ifndef RTE_SCHM_SWCBSW_CSM_CERTIFICATEVERIFY */
#endif /* #if ((CSM_API_ENABLED_KEYMNGMNT == STD_ON) && ((CSM_RTE_ENABLED_KEYMNGMNT == STD_OFF) || (!defined(CSM_SRC)))) */

#if ((CSM_API_ENABLED_SERVICE_DECRYPT == STD_ON) && ((CSM_RTE_ENABLED_SERVICE_DECRYPT == STD_OFF) || (!defined(CSM_SRC))))
#ifndef RTE_SCHM_SWCBSW_CSM_DECRYPT
#define RTE_SCHM_SWCBSW_CSM_DECRYPT

/** \brief  Decrypts the given encrypted data and store the decrypted plaintext in the memory
 **         location pointed by the result pointer.
 **
 ** \param[in]      jobId            Holds the identifier of the job using the CSM service.
 ** \param[in]      mode             Indicates which operation mode(s) to perfom.
 ** \param[in]      dataPtr          Contains the pointer to the data to be decrypted.
 ** \param[in]      dataLength       Contains the number of bytes to decrypt.
 ** \param[out]     resultPtr        Contains the pointer to the memory location where the
 **                                  decrypted data shall be stored.
 ** \param[in,out]  resultLengthPtr  Holds a pointer to the memory location in which the output
 **                                  length information is stored in bytes. On calling this
 **                                  function, this parameter shall contain the size of the
 **                                  buffer provided by resultPtr. When the request has
 **                                  finished, the actual length of the returned value shall be
 **                                  stored.
 **
 ** \returns  Error value.
 **
 ** \retval   E_OK                    request successful
 ** \retval   E_NOT_OK                request failed
 ** \retval   CRYPTO_E_BUSY           request failed, service is still busy
 ** \retval   CRYPTO_E_QUEUE_FULL     request failed, the queue is full
 ** \retval   CRYPTO_E_KEY_NOT_VALID  request failed, the key's state is 'invalid'
 ** \retval   CRYPTO_E_SMALL_BUFFER   the provided buffer is too small to store the result
 **
 ** \ServiceID{::CSM_SID_DECRYPT}
 ** \Synchronicity{Sync or Async, dependend on the job configuration}
 ** \Reentrancy{Reentrant}
 **/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_Decrypt
(
          uint32                                              jobId,
          Crypto_OperationModeType                            mode,
  P2CONST(uint8,                    AUTOMATIC, CSM_APPL_DATA) dataPtr,
          uint32                                              dataLength,
    P2VAR(uint8,                    AUTOMATIC, CSM_APPL_DATA) resultPtr,
    P2VAR(uint32,                   AUTOMATIC, CSM_APPL_DATA) resultLengthPtr
);

#endif /* #ifndef RTE_SCHM_SWCBSW_CSM_DECRYPT */
#endif /* #if ((CSM_API_ENABLED_SERVICE_DECRYPT == STD_ON) && ((CSM_RTE_ENABLED_SERVICE_DECRYPT == STD_OFF) || (!defined(CSM_SRC)))) */

#if ((CSM_API_ENABLED_SERVICE_ENCRYPT == STD_ON) && ((CSM_RTE_ENABLED_SERVICE_ENCRYPT == STD_OFF) || (!defined(CSM_SRC))))
#ifndef RTE_SCHM_SWCBSW_CSM_ENCRYPT
#define RTE_SCHM_SWCBSW_CSM_ENCRYPT

/** \brief  Encrypts the given data and store the ciphertext in the memory location pointed by
 **         the result pointer.
 **
 ** \param[in]      jobId            Holds the identifier of the job using the CSM service.
 ** \param[in]      mode             Indicates which operation mode(s) to perfom.
 ** \param[in]      dataPtr          Contains the pointer to the data to be encrypted.
 ** \param[in]      dataLength       Contains the number of bytes to encrypt.
 ** \param[out]     resultPtr        Contains the pointer to the data where the encrypted data
 **                                  shall be stored.
 ** \param[in,out]  resultLengthPtr  Holds a pointer to the memory location in which the output
 **                                  length information is stored in bytes. On calling this
 **                                  function, this parameter shall contain the size of the
 **                                  buffer provided by resultPtr. When the request has
 **                                  finished, the actual length of the returned value shall be
 **                                  stored.
 **
 ** \returns  Error value.
 **
 ** \retval   E_OK                    request successful
 ** \retval   E_NOT_OK                request failed
 ** \retval   CRYPTO_E_BUSY           request failed, service is still busy
 ** \retval   CRYPTO_E_QUEUE_FULL     request failed, the queue is full
 ** \retval   CRYPTO_E_KEY_NOT_VALID  request failed, the key's state is 'invalid'
 ** \retval   CRYPTO_E_SMALL_BUFFER   the provided buffer is too small to store the result
 **
 ** \ServiceID{::CSM_SID_ENCRYPT}
 ** \Synchronicity{Sync or Async, dependend on the job configuration}
 ** \Reentrancy{Reentrant}
 **/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_Encrypt
(
          uint32                                              jobId,
          Crypto_OperationModeType                            mode,
  P2CONST(uint8,                    AUTOMATIC, CSM_APPL_DATA) dataPtr,
          uint32                                              dataLength,
    P2VAR(uint8,                    AUTOMATIC, CSM_APPL_DATA) resultPtr,
    P2VAR(uint32,                   AUTOMATIC, CSM_APPL_DATA) resultLengthPtr
);

#endif /* #ifndef RTE_SCHM_SWCBSW_CSM_ENCRYPT */
#endif /* #if ((CSM_API_ENABLED_SERVICE_ENCRYPT == STD_ON) && ((CSM_RTE_ENABLED_SERVICE_ENCRYPT == STD_OFF) || (!defined(CSM_SRC)))) */

#if (CSM_API_ENABLED_VERSIONINFO == STD_ON)
#ifndef RTE_SCHM_SWCBSW_CSM_GETVERSIONINFO
#define RTE_SCHM_SWCBSW_CSM_GETVERSIONINFO

/** \brief  Returns the version information of this module.
 **
 ** \param[out]  versioninfo  Pointer to where to store the version information of this module.
 **
 ** \ServiceID{::CSM_SID_GETVERSIONINFO}
 ** \Synchronicity{Synchronous}
 ** \Reentrancy{Reentrant}
 **/
extern FUNC(void, CSM_CODE) Csm_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, CSM_APPL_DATA) versioninfo
);

#endif /* #ifndef RTE_SCHM_SWCBSW_CSM_GETVERSIONINFO */
#endif /* #if (CSM_API_ENABLED_VERSIONINFO == STD_ON) */

#if ((CSM_API_ENABLED_SERVICE_HASH == STD_ON) && ((CSM_RTE_ENABLED_SERVICE_HASH == STD_OFF) || (!defined(CSM_SRC))))
#ifndef RTE_SCHM_SWCBSW_CSM_HASH
#define RTE_SCHM_SWCBSW_CSM_HASH

/** \brief  Uses the given data to perform the hash calculation and stores the hash.
 **
 ** \param[in]      jobId            Holds the identifier of the job using the CSM service.
 ** \param[in]      mode             Indicates which operation mode(s) to perfom.
 ** \param[in]      dataPtr          Contains the pointer to the data for which the hash shall
 **                                  be computed.
 ** \param[in]      dataLength       Contains the number of bytes to be hashed.
 ** \param[out]     resultPtr        Contains the pointer to the data where the hash value
 **                                  shall be stored.
 ** \param[in,out]  resultLengthPtr  Holds a pointer to the memory location in which the output
 **                                  length in bytes is stored. On calling this function, this
 **                                  parameter shall contain the size of the buffer provided by
 **                                  resultPtr. When the request has finished, the actual
 **                                  length of the returned value shall be stored.
 **
 ** \returns  Error value.
 **
 ** \retval   E_OK                   request successful
 ** \retval   E_NOT_OK               request failed
 ** \retval   CRYPTO_E_BUSY          request failed, service is still busy
 ** \retval   CRYPTO_E_QUEUE_FULL    request failed, the queue is full
 ** \retval   CRYPTO_E_SMALL_BUFFER  the provided buffer is too small to store the result
 **
 ** \ServiceID{::CSM_SID_HASH}
 ** \Synchronicity{Sync or Async, dependend on the job configuration}
 ** \Reentrancy{Reentrant}
 **/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_Hash
(
          uint32                                              jobId,
          Crypto_OperationModeType                            mode,
  P2CONST(uint8,                    AUTOMATIC, CSM_APPL_DATA) dataPtr,
          uint32                                              dataLength,
    P2VAR(uint8,                    AUTOMATIC, CSM_APPL_DATA) resultPtr,
    P2VAR(uint32,                   AUTOMATIC, CSM_APPL_DATA) resultLengthPtr
);

#endif /* #ifndef RTE_SCHM_SWCBSW_CSM_HASH */
#endif /* #if ((CSM_API_ENABLED_SERVICE_HASH == STD_ON) && ((CSM_RTE_ENABLED_SERVICE_HASH == STD_OFF) || (!defined(CSM_SRC)))) */

#ifndef RTE_SCHM_SWCBSW_CSM_INIT
#define RTE_SCHM_SWCBSW_CSM_INIT

/** \brief  Initializes the CSM module.
 **
 ** \ServiceID{::CSM_SID_INIT}
 ** \Synchronicity{Synchronous}
 ** \Reentrancy{Reentrant}
 **/
extern FUNC(void, CSM_CODE) Csm_Init
(
  void
);

#endif /* #ifndef RTE_SCHM_SWCBSW_CSM_INIT */

#if ((CSM_API_ENABLED_KEYMNGMNT == STD_ON) && ((CSM_RTE_ENABLED_KEYMNGMNT == STD_OFF) || (!defined(CSM_SRC))))
#ifndef RTE_SCHM_SWCBSW_CSM_KEYCOPY
#define RTE_SCHM_SWCBSW_CSM_KEYCOPY

/** \brief  This function shall copy all key elements from the source key to a target key.
 **
 ** \param[in]  keyId        Holds the identifier of the key whose key element shall be the
 **                          source element.
 ** \param[in]  targetKeyId  Holds the identifier of the key whose key element shall be the
 **                          destination element.
 **
 ** \returns  Error value.
 **
 ** \retval   E_OK                        Request successful
 ** \retval   E_NOT_OK                    Request Failed
 ** \retval   CRYPTO_E_BUSY               Request Failed, Crypto Driver Object is Busy
 ** \retval   CRYPTO_E_KEY_NOT_AVAILABLE  Request failed, the requested key element is not available
 ** \retval   CRYPTO_E_KEY_READ_FAIL      Request failed, not allowed to extract key element
 ** \retval   CRYPTO_E_KEY_WRITE_FAIL     Request failed, not allowed to write key element
 ** \retval   CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, key element sizes are not compatible
 **
 ** \ServiceID{::CSM_SID_KEYCOPY}
 ** \Synchronicity{Synchronous}
 ** \Reentrancy{Reentrant, but not for same keyId}
 **/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeyCopy
(
  uint32 keyId,
  uint32 targetKeyId
);

#endif /* #ifndef RTE_SCHM_SWCBSW_CSM_KEYCOPY */
#endif /* #if ((CSM_API_ENABLED_KEYMNGMNT == STD_ON) && ((CSM_RTE_ENABLED_KEYMNGMNT == STD_OFF) || (!defined(CSM_SRC)))) */

#if ((CSM_API_ENABLED_KEYMNGMNT == STD_ON) && ((CSM_RTE_ENABLED_KEYMNGMNT == STD_OFF) || (!defined(CSM_SRC))))
#ifndef RTE_SCHM_SWCBSW_CSM_KEYDERIVE
#define RTE_SCHM_SWCBSW_CSM_KEYDERIVE

/** \brief  Derives a new key by using the key elements in the given key identified by the
 **         keyId. The given key contains the key elements for the password and salt. The
 **         derived key is stored in the key element with the id 1 of the key identified by
 **         targetCryptoKeyId.
 **
 ** \param[in]  keyId        Holds the identifier of the key which is used for key derivation.
 ** \param[in]  targetKeyId  Holds the identifier of the key which is used to store the derived
 **                          key.
 **
 ** \returns  Error value.
 **
 ** \retval   E_OK           Request successful
 ** \retval   E_NOT_OK       Request Failed
 ** \retval   CRYPTO_E_BUSY  Request Failed, Crypto Driver Object is Busy
 **
 ** \ServiceID{::CSM_SID_KEYDERIVE}
 ** \Synchronicity{Synchronous}
 ** \Reentrancy{Reentrant, but not for same keyId}
 **/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeyDerive
(
  uint32 keyId,
  uint32 targetKeyId
);

#endif /* #ifndef RTE_SCHM_SWCBSW_CSM_KEYDERIVE */
#endif /* #if ((CSM_API_ENABLED_KEYMNGMNT == STD_ON) && ((CSM_RTE_ENABLED_KEYMNGMNT == STD_OFF) || (!defined(CSM_SRC)))) */

#if ((CSM_API_ENABLED_KEYMNGMNT == STD_ON) && ((CSM_RTE_ENABLED_KEYMNGMNT == STD_OFF) || (!defined(CSM_SRC))))
#ifndef RTE_SCHM_SWCBSW_CSM_KEYELEMENTCOPY
#define RTE_SCHM_SWCBSW_CSM_KEYELEMENTCOPY

/** \brief  This function shall copy a key elements from one key to a target key.
 **
 ** \param[in]  keyId               Holds the identifier of the key whose key element shall be
 **                                 the source element.
 ** \param[in]  keyElementId        Holds the identifier of the key element which shall be the
 **                                 source for the copy operation.
 ** \param[in]  targetKeyId         Holds the identifier of the key whose key element shall be
 **                                 the destination element.
 ** \param[in]  targetKeyElementId  Holds the identifier of the key element which shall be the
 **                                 destination for the copy operation.
 **
 ** \returns  Error value.
 **
 ** \retval   E_OK                        Request successful
 ** \retval   E_NOT_OK                    Request Failed
 ** \retval   CRYPTO_E_BUSY               Request Failed, Crypto Driver Object is Busy
 ** \retval   CRYPTO_E_KEY_NOT_AVAILABLE  Request failed, the requested key element is not available
 ** \retval   CRYPTO_E_KEY_READ_FAIL      Request failed, not allowed to extract key element
 ** \retval   CRYPTO_E_KEY_WRITE_FAIL     Request failed, not allowed to write key element
 ** \retval   CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, key element sizes are not compatible
 **
 ** \ServiceID{::CSM_SID_KEYELEMENTCOPY}
 ** \Synchronicity{Synchronous}
 ** \Reentrancy{Reentrant, but not for the same keyId}
 **/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementCopy
(
  uint32 keyId,
  uint32 keyElementId,
  uint32 targetKeyId,
  uint32 targetKeyElementId
);

#endif /* #ifndef RTE_SCHM_SWCBSW_CSM_KEYELEMENTCOPY */
#endif /* #if ((CSM_API_ENABLED_KEYMNGMNT == STD_ON) && ((CSM_RTE_ENABLED_KEYMNGMNT == STD_OFF) || (!defined(CSM_SRC)))) */

#if ((CSM_API_ENABLED_KEYMNGMNT == STD_ON) && ((CSM_RTE_ENABLED_KEYMNGMNT == STD_OFF) || (!defined(CSM_SRC))))
#ifndef RTE_SCHM_SWCBSW_CSM_KEYELEMENTGET
#define RTE_SCHM_SWCBSW_CSM_KEYELEMENTGET

/** \brief  Retrieves the key element bytes from a specific key element of the key identified
 **         by the keyId and stores the key element in the memory location pointed by the key
 **         pointer.
 **
 ** \param[in]      keyId         Holds the identifier of the key from which a key element
 **                               shall be extracted.
 ** \param[in]      keyElementId  Holds the identifier of the key element to be extracted.
 ** \param[out]     keyPtr        Holds the pointer to the memory location where the key shall
 **                               be copied to.
 ** \param[in,out]  keyLengthPtr  Holds a pointer to the memory location in which the output
 **                               buffer length in bytes is stored. On calling this function,
 **                               this parameter shall contain the buffer length in bytes of
 **                               the keyPtr. When the request has finished, the actual size of
 **                               the written input bytes shall be stored.
 **
 ** \returns  Error value.
 **
 ** \retval   E_OK                        request successful
 ** \retval   E_NOT_OK                    request failed
 ** \retval   CRYPTO_E_BUSY               Request Failed, Crypto Driver Object is Busy
 ** \retval   CRYPTO_E_KEY_NOT_AVAILABLE  request failed, the requested key element is not available
 ** \retval   CRYPTO_E_KEY_READ_FAIL      Request failed because read access was denied
 ** \retval   CRYPTO_E_SMALL_BUFFER       the provided buffer is too small to store the result
 **
 ** \ServiceID{::CSM_SID_KEYELEMENTGET}
 ** \Synchronicity{Synchronous}
 ** \Reentrancy{Reentrant}
 **/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementGet
(
        uint32                            keyId,
        uint32                            keyElementId,
  P2VAR(uint8,  AUTOMATIC, CSM_APPL_DATA) keyPtr,
  P2VAR(uint32, AUTOMATIC, CSM_APPL_DATA) keyLengthPtr
);

#endif /* #ifndef RTE_SCHM_SWCBSW_CSM_KEYELEMENTGET */
#endif /* #if ((CSM_API_ENABLED_KEYMNGMNT == STD_ON) && ((CSM_RTE_ENABLED_KEYMNGMNT == STD_OFF) || (!defined(CSM_SRC)))) */

#if ((CSM_API_ENABLED_KEYMNGMNT == STD_ON) && ((CSM_RTE_ENABLED_KEYMNGMNT == STD_OFF) || (!defined(CSM_SRC))))
#ifndef RTE_SCHM_SWCBSW_CSM_KEYELEMENTSET
#define RTE_SCHM_SWCBSW_CSM_KEYELEMENTSET

/** \brief  Sets the given key element bytes to the key identified by keyId.
 **
 ** \param[in]  keyId         Holds the identifier of the key for which a new material shall be
 **                           set.
 ** \param[in]  keyElementId  Holds the identifier of the key element to be written.
 ** \param[in]  keyPtr        Holds the pointer to the key element bytes to be processed.
 ** \param[in]  keyLength     Contains the number of key element bytes.
 **
 ** \returns  Error value.
 **
 ** \retval   E_OK                        request successful
 ** \retval   E_NOT_OK                    request failed
 ** \retval   CRYPTO_E_BUSY               Request Failed, Crypto Driver Object is Busy
 ** \retval   CRYPTO_E_KEY_WRITE_FAIL     Request failed because write access was denied
 ** \retval   CRYPTO_E_KEY_NOT_AVAILABLE  Request failed because the key is not available
 ** \retval   CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, key element size does not match size of provided data
 **
 ** \ServiceID{::CSM_SID_KEYELEMENTSET}
 ** \Synchronicity{Synchronous}
 ** \Reentrancy{Non Reentrant}
 **/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementSet
(
          uint32                            keyId,
          uint32                            keyElementId,
  P2CONST(uint8,  AUTOMATIC, CSM_APPL_DATA) keyPtr,
          uint32                            keyLength
);

#endif /* #ifndef RTE_SCHM_SWCBSW_CSM_KEYELEMENTSET */
#endif /* #if ((CSM_API_ENABLED_KEYMNGMNT == STD_ON) && ((CSM_RTE_ENABLED_KEYMNGMNT == STD_OFF) || (!defined(CSM_SRC)))) */

#if ((CSM_API_ENABLED_KEYMNGMNT == STD_ON) && ((CSM_RTE_ENABLED_KEYMNGMNT == STD_OFF) || (!defined(CSM_SRC))))
#ifndef RTE_SCHM_SWCBSW_CSM_KEYEXCHANGECALCPUBVAL
#define RTE_SCHM_SWCBSW_CSM_KEYEXCHANGECALCPUBVAL

/** \brief  Calculates the public value of the current user for the key exchange and stores the
 **         public key in the memory location pointed by the public value pointer.
 **
 ** \param[in]      keyId                 Holds the identifier of the key which shall be used
 **                                       for the key exchange protocol.
 ** \param[out]     publicValuePtr        Contains the pointer to the data where the public
 **                                       value shall be stored.
 ** \param[in,out]  publicValueLengthPtr  Holds a pointer to the memory location in which the
 **                                       public value length information is stored. On calling
 **                                       this function, this parameter shall contain the size
 **                                       of the buffer provided by publicValuePtr. When the
 **                                       request has finished, the actual length of the
 **                                       returned value shall be stored.
 **
 ** \returns  Error value.
 **
 ** \retval   E_OK                    request successful
 ** \retval   E_NOT_OK                request failed
 ** \retval   CRYPTO_E_KEY_NOT_VALID  request failed, the key's state is 'invalid'
 ** \retval   CRYPTO_E_SMALL_BUFFER   the provided buffer is too small to store the result
 **
 ** \ServiceID{::CSM_SID_KEYEXCHANGECALCPUBVAL}
 ** \Synchronicity{Synchronous}
 ** \Reentrancy{Reentrant, but not for same keyId}
 **/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeyExchangeCalcPubVal
(
        uint32                            keyId,
  P2VAR(uint8,  AUTOMATIC, CSM_APPL_DATA) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CSM_APPL_DATA) publicValueLengthPtr
);

#endif /* #ifndef RTE_SCHM_SWCBSW_CSM_KEYEXCHANGECALCPUBVAL */
#endif /* #if ((CSM_API_ENABLED_KEYMNGMNT == STD_ON) && ((CSM_RTE_ENABLED_KEYMNGMNT == STD_OFF) || (!defined(CSM_SRC)))) */

#if ((CSM_API_ENABLED_KEYMNGMNT == STD_ON) && ((CSM_RTE_ENABLED_KEYMNGMNT == STD_OFF) || (!defined(CSM_SRC))))
#ifndef RTE_SCHM_SWCBSW_CSM_KEYEXCHANGECALCSECRET
#define RTE_SCHM_SWCBSW_CSM_KEYEXCHANGECALCSECRET

/** \brief  Calculates the shared secret key for the key exchange with the key material of the
 **         key identified by the keyId and the partner public key. The shared secret key is
 **         stored as a key element in the same key.
 **
 ** \param[in]  keyId                     Holds the identifier of the key which shall be used
 **                                       for the key exchange protocol.
 ** \param[in]  partnerPublicValuePtr     Holds the pointer to the memory location which
 **                                       contains the partner's public value.
 ** \param[in]  partnerPublicValueLength  Contains the length of the partner's public value in
 **                                       bytes.
 **
 ** \returns  Error value.
 **
 ** \retval   E_OK                   Request successful
 ** \retval   E_NOT_OK               Request Failed
 ** \retval   CRYPTO_E_BUSY          Request Failed, Crypto Driver Object is Busy
 ** \retval   CRYPTO_E_SMALL_BUFFER  The provided buffer is too small to store the result
 **
 ** \ServiceID{::CSM_SID_KEYEXCHANGECALCSECRET}
 ** \Synchronicity{Synchronous}
 ** \Reentrancy{Reentrant but not for same keyId}
 **/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeyExchangeCalcSecret
(
          uint32                            keyId,
  P2CONST(uint8,  AUTOMATIC, CSM_APPL_DATA) partnerPublicValuePtr,
          uint32                            partnerPublicValueLength
);

#endif /* #ifndef RTE_SCHM_SWCBSW_CSM_KEYEXCHANGECALCSECRET */
#endif /* #if ((CSM_API_ENABLED_KEYMNGMNT == STD_ON) && ((CSM_RTE_ENABLED_KEYMNGMNT == STD_OFF) || (!defined(CSM_SRC)))) */

#if ((CSM_API_ENABLED_KEYMNGMNT == STD_ON) && ((CSM_RTE_ENABLED_KEYMNGMNT == STD_OFF) || (!defined(CSM_SRC))))
#ifndef RTE_SCHM_SWCBSW_CSM_KEYGENERATE
#define RTE_SCHM_SWCBSW_CSM_KEYGENERATE

/** \brief  Generates new key material and store it in the key identified by keyId.
 **
 ** \param[in]  keyId  Holds the identifier of the key for which a new material shall be
 **                    generated.
 **
 ** \returns  Error value.
 **
 ** \retval   E_OK      Request successful
 ** \retval   E_NOT_OK  Request Failed
 **
 ** \ServiceID{::CSM_SID_KEYGENERATE}
 ** \Synchronicity{Synchronous}
 ** \Reentrancy{Reentrant but not for same keyId}
 **/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeyGenerate
(
  uint32 keyId
);

#endif /* #ifndef RTE_SCHM_SWCBSW_CSM_KEYGENERATE */
#endif /* #if ((CSM_API_ENABLED_KEYMNGMNT == STD_ON) && ((CSM_RTE_ENABLED_KEYMNGMNT == STD_OFF) || (!defined(CSM_SRC)))) */

#if ((CSM_API_ENABLED_KEYMNGMNT == STD_ON) && ((CSM_RTE_ENABLED_KEYMNGMNT == STD_OFF) || (!defined(CSM_SRC))))
#ifndef RTE_SCHM_SWCBSW_CSM_KEYSETVALID
#define RTE_SCHM_SWCBSW_CSM_KEYSETVALID

/** \brief  Sets the key state of the key identified by keyId to valid.
 **
 ** \param[in]  keyId  Holds the identifier of the key for which a new material shall be
 **                    validated.
 **
 ** \returns  Error value.
 **
 ** \retval   E_OK           request successful
 ** \retval   E_NOT_OK       request failed
 ** \retval   CRYPTO_E_BUSY  Request Failed, Crypro Driver Object is Busy
 **
 ** \ServiceID{::CSM_SID_KEYSETVALID}
 ** \Synchronicity{Synchronous}
 ** \Reentrancy{Non Reentrant}
 **/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeySetValid
(
  uint32 keyId
);

#endif /* #ifndef RTE_SCHM_SWCBSW_CSM_KEYSETVALID */
#endif /* #if ((CSM_API_ENABLED_KEYMNGMNT == STD_ON) && ((CSM_RTE_ENABLED_KEYMNGMNT == STD_OFF) || (!defined(CSM_SRC)))) */

#if ((CSM_API_ENABLED_SERVICE_MACGENERATE == STD_ON) && ((CSM_RTE_ENABLED_SERVICE_MACGENERATE == STD_OFF) || (!defined(CSM_SRC))))
#ifndef RTE_SCHM_SWCBSW_CSM_MACGENERATE
#define RTE_SCHM_SWCBSW_CSM_MACGENERATE

/** \brief  Uses the given data to perform a MAC generation and stores the MAC in the memory
 **         location pointed to by the MAC pointer.
 **
 ** \param[in]      jobId         Holds the identifier of the job using the CSM service.
 ** \param[in]      mode          Indicates which operation mode(s) to perfom.
 ** \param[in]      dataPtr       Contains the pointer to the data for which the MAC shall be
 **                               computed.
 ** \param[in]      dataLength    Contains the number of bytes to be hashed.
 ** \param[out]     macPtr        Contains the pointer to the data where the MAC shall be
 **                               stored.
 ** \param[in,out]  macLengthPtr  Holds a pointer to the memory location in which the output
 **                               length in bytes is stored. On calling this function, this
 **                               parameter shall contain the size of the buffer provided by
 **                               macPtr. When the request has finished, the actual length of
 **                               the returned MAC shall be stored.
 **
 ** \returns  Error value.
 **
 ** \retval   E_OK                    request successful
 ** \retval   E_NOT_OK                request failed
 ** \retval   CRYPTO_E_BUSY           request failed, service is still busy
 ** \retval   CRYPTO_E_QUEUE_FULL     request failed, the queue is full
 ** \retval   CRYPTO_E_KEY_NOT_VALID  request failed, the key's state is 'invalid'
 ** \retval   CRYPTO_E_SMALL_BUFFER   the provided buffer is too small to store the result
 **
 ** \ServiceID{::CSM_SID_MACGENERATE}
 ** \Synchronicity{Asynchronous or Async, dependend on the job configuration}
 ** \Reentrancy{Reentrant}
 **/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_MacGenerate
(
          uint32                                              jobId,
          Crypto_OperationModeType                            mode,
  P2CONST(uint8,                    AUTOMATIC, CSM_APPL_DATA) dataPtr,
          uint32                                              dataLength,
    P2VAR(uint8,                    AUTOMATIC, CSM_APPL_DATA) macPtr,
    P2VAR(uint32,                   AUTOMATIC, CSM_APPL_DATA) macLengthPtr
);

#endif /* #ifndef RTE_SCHM_SWCBSW_CSM_MACGENERATE */
#endif /* #if ((CSM_API_ENABLED_SERVICE_MACGENERATE == STD_ON) && ((CSM_RTE_ENABLED_SERVICE_MACGENERATE == STD_OFF) || (!defined(CSM_SRC)))) */

#if ((CSM_API_ENABLED_SERVICE_MACVERIFY == STD_ON) && ((CSM_RTE_ENABLED_SERVICE_MACVERIFY == STD_OFF) || (!defined(CSM_SRC))))
#ifndef RTE_SCHM_SWCBSW_CSM_MACVERIFY
#define RTE_SCHM_SWCBSW_CSM_MACVERIFY

/** \brief  Verifies the given MAC by comparing if the MAC is generated with the given data.
 **
 ** \param[in]   jobId       Indicates which operation mode(s) to perfom.
 ** \param[in]   mode        Indicates which operation mode(s) to perfom.
 ** \param[in]   dataPtr     Holds a pointer to the data for which the MAC shall be verified.
 ** \param[in]   dataLength  Contains the number of data bytes for which the MAC shall be
 **                          verified.
 ** \param[in]   macPtr      Holds a pointer to the MAC to be verified.
 ** \param[in]   macLength   Contains the MAC length in BITS to be verified.
 ** \param[out]  verifyPtr   Holds a pointer to the memory location, which will hold the result
 **                          of the MAC verification.
 **
 ** \returns  Error value.
 **
 ** \retval   E_OK                    request successful
 ** \retval   E_NOT_OK                request failed
 ** \retval   CRYPTO_E_BUSY           request failed, service is still busy
 ** \retval   CRYPTO_E_QUEUE_FULL     request failed, the queue is full
 ** \retval   CRYPTO_E_KEY_NOT_VALID  request failed, the key's state is 'invalid'
 **
 ** \ServiceID{::CSM_SID_MACVERIFY}
 ** \Synchronicity{Sync or Async, dependend on the job configuration}
 ** \Reentrancy{Reentrant}
 **/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_MacVerify
(
          uint32                                              jobId,
          Crypto_OperationModeType                            mode,
  P2CONST(uint8,                    AUTOMATIC, CSM_APPL_DATA) dataPtr,
          uint32                                              dataLength,
  P2CONST(uint8,                    AUTOMATIC, CSM_APPL_DATA) macPtr,
          uint32                                              macLength,
    P2VAR(Crypto_VerifyResultType,  AUTOMATIC, CSM_APPL_DATA) verifyPtr
);

#endif /* #ifndef RTE_SCHM_SWCBSW_CSM_MACVERIFY */
#endif /* #if ((CSM_API_ENABLED_SERVICE_MACVERIFY == STD_ON) && ((CSM_RTE_ENABLED_SERVICE_MACVERIFY == STD_OFF) || (!defined(CSM_SRC)))) */

#if ((CSM_API_ENABLED_SERVICE_ASYNCHRONOUS == STD_ON) && ((CSM_RTE_ENABLED == STD_OFF) || (!defined(CSM_SRC))))
#ifndef RTE_SCHM_SWCBSW_CSM_MAINFUNCTION
#define RTE_SCHM_SWCBSW_CSM_MAINFUNCTION

/** \brief  API to be called cyclically to process the requested jobs. The Csm_MainFunction
 **         shall check the queues for jobs to pass to the underlying CRYIF.
 **
 ** \ServiceID{::CSM_SID_MAINFUNCTION}
 ** \Synchronicity{Synchronous}
 ** \Reentrancy{Non Reentrant}
 **/
extern FUNC(void, CSM_CODE) Csm_MainFunction
(
  void
);

#endif /* #ifndef RTE_SCHM_SWCBSW_CSM_MAINFUNCTION */
#endif /* #if ((CSM_API_ENABLED_SERVICE_ASYNCHRONOUS == STD_ON) && ((CSM_RTE_ENABLED == STD_OFF) || (!defined(CSM_SRC)))) */

#if ((CSM_API_ENABLED_SERVICE_RANDOMGENERATE == STD_ON) && ((CSM_RTE_ENABLED_SERVICE_RANDOMGENERATE == STD_OFF) || (!defined(CSM_SRC))))
#ifndef RTE_SCHM_SWCBSW_CSM_RANDOMGENERATE
#define RTE_SCHM_SWCBSW_CSM_RANDOMGENERATE

/** \brief  Generate a random number and stores it in the memory location pointed by the result
 **         pointer.
 **
 ** \param[in]      jobId            Holds the identifier of the job using the CSM service.
 ** \param[out]     resultPtr        Holds a pointer to the memory location which will hold the
 **                                  result of the random number generation.
 ** \param[in,out]  resultLengthPtr  Holds a pointer to the memory location in which the result
 **                                  length in bytes is stored. On calling this function, this
 **                                  parameter shall contain the number of random bytes, which
 **                                  shall be stored to the buffer provided by resultPtr. When
 **                                  the request has finished, the actual length of the
 **                                  returned value shall be stored.
 **
 ** \returns  Error value.
 **
 ** \retval   E_OK                         request successful
 ** \retval   E_NOT_OK                     request failed
 ** \retval   CRYPTO_E_BUSY                request failed, service is still busy
 ** \retval   CRYPTO_E_QUEUE_FULL          request failed, the queue is full
 ** \retval   CRYPTO_E_ENTROPY_EXHAUSTION  request failed, entropy of random number generator is exhausted
 **
 ** \ServiceID{::CSM_SID_RANDOMGENERATE}
 ** \Synchronicity{Sync or Async, dependend on the job configuration}
 ** \Reentrancy{Reentrant}
 **/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_RandomGenerate
(
        uint32                            jobId,
  P2VAR(uint8,  AUTOMATIC, CSM_APPL_DATA) resultPtr,
  P2VAR(uint32, AUTOMATIC, CSM_APPL_DATA) resultLengthPtr
);

#endif /* #ifndef RTE_SCHM_SWCBSW_CSM_RANDOMGENERATE */
#endif /* #if ((CSM_API_ENABLED_SERVICE_RANDOMGENERATE == STD_ON) && ((CSM_RTE_ENABLED_SERVICE_RANDOMGENERATE == STD_OFF) || (!defined(CSM_SRC)))) */

#if ((CSM_API_ENABLED_KEYMNGMNT == STD_ON) && ((CSM_RTE_ENABLED_KEYMNGMNT == STD_OFF) || (!defined(CSM_SRC))))
#ifndef RTE_SCHM_SWCBSW_CSM_RANDOMSEED
#define RTE_SCHM_SWCBSW_CSM_RANDOMSEED

/** \brief  This function shall dispatch the random seed function to the configured crypto
 **         driver object.
 **
 ** \param[in]  keyId       Holds the identifier of the key for which a new seed shall be
 **                         generated.
 ** \param[in]  seedPtr     Holds a pointer to the memory location which contains the data to
 **                         feed the seed.
 ** \param[in]  seedLength  Contains the length of the seed in bytes.
 **
 ** \returns  Error value.
 **
 ** \retval   E_OK      Request successful
 ** \retval   E_NOT_OK  Request Failed
 **
 ** \ServiceID{::CSM_SID_RANDOMSEED}
 ** \Synchronicity{Synchronous}
 ** \Reentrancy{Reentrant, but not for same keyId}
 **/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_RandomSeed
(
          uint32                            keyId,
  P2CONST(uint8,  AUTOMATIC, CSM_APPL_DATA) seedPtr,
          uint32                            seedLength
);

#endif /* #ifndef RTE_SCHM_SWCBSW_CSM_RANDOMSEED */
#endif /* #if ((CSM_API_ENABLED_KEYMNGMNT == STD_ON) && ((CSM_RTE_ENABLED_KEYMNGMNT == STD_OFF) || (!defined(CSM_SRC)))) */

#if ((CSM_API_ENABLED_SERVICE_SIGNATUREGENERATE == STD_ON) && ((CSM_RTE_ENABLED_SERVICE_SIGNATUREGENERATE == STD_OFF) || (!defined(CSM_SRC))))
#ifndef RTE_SCHM_SWCBSW_CSM_SIGNATUREGENERATE
#define RTE_SCHM_SWCBSW_CSM_SIGNATUREGENERATE

/** \brief  Uses the given data to perform the signature calculation and stores the signature
 **         in the memory location pointed by the result pointer.
 **
 ** \param[in]      jobId            Holds the identifier of the job using the CSM service.
 ** \param[in]      mode             Indicates which operation mode(s) to perform.
 ** \param[in]      dataPtr          Contains the pointer to the data to be signed.
 ** \param[in]      dataLength       Contains the number of bytes to sign.
 ** \param[out]     resultPtr        Contains the pointer to the data where the signature shall
 **                                  be stored.
 ** \param[in,out]  resultLengthPtr  Holds a pointer to the memory location in which the output
 **                                  length in bytes of the signature is stored. On calling
 **                                  this function, this parameter shall contain the size of
 **                                  the buffer provided by resultPtr. When the request has
 **                                  finished, the actual length of the returned value shall be
 **                                  stored.
 **
 ** \returns  Error value.
 **
 ** \retval   E_OK                    request successful
 ** \retval   E_NOT_OK                request failed
 ** \retval   CRYPTO_E_BUSY           request failed, service is still busy
 ** \retval   CRYPTO_E_QUEUE_FULL     request failed, the queue is full
 ** \retval   CRYPTO_E_KEY_NOT_VALID  request failed, the key's state is 'invalid'
 ** \retval   CRYPTO_E_SMALL_BUFFER   the provided buffer is too small to store the result
 **
 ** \ServiceID{::CSM_SID_SIGNATUREGENERATE}
 ** \Synchronicity{Sync or Async, dependend on the job configuration}
 ** \Reentrancy{Reentrant}
 **/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureGenerate
(
          uint32                                              jobId,
          Crypto_OperationModeType                            mode,
  P2CONST(uint8,                    AUTOMATIC, CSM_APPL_DATA) dataPtr,
          uint32                                              dataLength,
    P2VAR(uint8,                    AUTOMATIC, CSM_APPL_DATA) resultPtr,
    P2VAR(uint32,                   AUTOMATIC, CSM_APPL_DATA) resultLengthPtr
);

#endif /* #ifndef RTE_SCHM_SWCBSW_CSM_SIGNATUREGENERATE */
#endif /* #if ((CSM_API_ENABLED_SERVICE_SIGNATUREGENERATE == STD_ON) && ((CSM_RTE_ENABLED_SERVICE_SIGNATUREGENERATE == STD_OFF) || (!defined(CSM_SRC)))) */

#if ((CSM_API_ENABLED_SERVICE_SIGNATUREVERIFY == STD_ON) && ((CSM_RTE_ENABLED_SERVICE_SIGNATUREVERIFY == STD_OFF) || (!defined(CSM_SRC))))
#ifndef RTE_SCHM_SWCBSW_CSM_SIGNATUREVERIFY
#define RTE_SCHM_SWCBSW_CSM_SIGNATUREVERIFY

/** \brief  Verifies the given MAC by comparing if the signature is generated with the given
 **         data.
 **
 ** \param[in]   jobId            Holds the identifier of the job using the CSM service.
 ** \param[in]   mode             The Crypto_JobInfoType job with the corresponding jobId shall
 **                               be modified in the following way:.
 ** \param[in]   dataPtr          Contains the pointer to the data to be verified.
 ** \param[in]   dataLength       Contains the number of data bytes.
 ** \param[in]   signaturePtr     Holds a pointer to the signature to be verified.
 ** \param[in]   signatureLength  Contains the signature length in bytes.
 ** \param[out]  verifyPtr        Holds a pointer to the memory location, which will hold the
 **                               result of the signature verification.
 **
 ** \returns  Error value.
 **
 ** \retval   E_OK                    request successful
 ** \retval   E_NOT_OK                request failed
 ** \retval   CRYPTO_E_BUSY           request failed, service is still busy
 ** \retval   CRYPTO_E_QUEUE_FULL     request failed, the queue is full
 ** \retval   CRYPTO_E_KEY_NOT_VALID  request failed, the key's state is 'invalid'
 ** \retval   CRYPTO_E_SMALL_BUFFER   the provided buffer is too small to store the result
 **
 ** \ServiceID{::CSM_SID_SIGNATUREVERIFY}
 ** \Synchronicity{Sync or Async, dependend on the job configuration}
 ** \Reentrancy{Reentrant}
 **/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureVerify
(
          uint32                                              jobId,
          Crypto_OperationModeType                            mode,
  P2CONST(uint8,                    AUTOMATIC, CSM_APPL_DATA) dataPtr,
          uint32                                              dataLength,
  P2CONST(uint8,                    AUTOMATIC, CSM_APPL_DATA) signaturePtr,
          uint32                                              signatureLength,
    P2VAR(Crypto_VerifyResultType,  AUTOMATIC, CSM_APPL_DATA) verifyPtr
);

#endif /* #ifndef RTE_SCHM_SWCBSW_CSM_SIGNATUREVERIFY */
#endif /* #if ((CSM_API_ENABLED_SERVICE_SIGNATUREVERIFY == STD_ON) && ((CSM_RTE_ENABLED_SERVICE_SIGNATUREVERIFY == STD_OFF) || (!defined(CSM_SRC)))) */

#define CSM_STOP_SEC_CODE
#include <Csm_MemMap.h>

/*================================================================================================*/

#endif /* #ifndef CSM_H */

/*==================[end of file]=================================================================*/

