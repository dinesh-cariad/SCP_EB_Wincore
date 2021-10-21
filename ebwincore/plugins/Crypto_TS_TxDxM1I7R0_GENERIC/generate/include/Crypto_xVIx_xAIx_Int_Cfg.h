/**
 * \file
 *
 * \brief AUTOSAR Crypto
 *
 * This file contains the implementation of the AUTOSAR
 * module Crypto.
 *
 * \version 1.7.42
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

[!AUTOSPACING!]

#ifndef CRYPTO_XVIX_XAIX_INT_CFG_H
#define CRYPTO_XVIX_XAIX_INT_CFG_H

/*==================[misra deviations]============================================================*/

/*==================[inclusions]==================================================================*/

#include <Crypto_xVIx_xAIx.h>

/*==================[defensive programming]=======================================================*/

#if (defined CRYPTO_XVIX_XAIX_DEFENSIVE_PROGRAMMING_ENABLED)
#error CRYPTO_XVIX_XAIX_DEFENSIVE_PROGRAMMING_ENABLED is already defined
#endif
/** \brief Defensive programming usage
 *         En- or disables the usage of the defensive programming
 */
#define CRYPTO_XVIX_XAIX_DEFENSIVE_PROGRAMMING_ENABLED [!//
[!IF "(
        (CryptoGeneral/CryptoDevErrorDetect  = 'true') and
        (CryptoDefensiveProgramming/CryptoDefProgEnabled = 'true')
      )"!][!//
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined CRYPTO_XVIX_XAIX_PRECONDITION_ASSERT_ENABLED)
#error CRYPTO_XVIX_XAIX_PRECONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Precondition assertion usage
 *         En- or disables the usage of precondition assertion checks
 */
#define CRYPTO_XVIX_XAIX_PRECONDITION_ASSERT_ENABLED [!//
[!IF "(
        (CryptoGeneral/CryptoDevErrorDetect  = 'true') and
        (CryptoDefensiveProgramming/CryptoDefProgEnabled = 'true') and
        (CryptoDefensiveProgramming/CryptoPrecondAssertEnabled = 'true')
      )"!][!//
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined CRYPTO_XVIX_XAIX_POSTCONDITION_ASSERT_ENABLED)
#error CRYPTO_XVIX_XAIX_POSTCONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Postcondition assertion usage
 *         En- or disables the usage of postcondition assertion checks
 */
#define CRYPTO_XVIX_XAIX_POSTCONDITION_ASSERT_ENABLED [!//
[!IF "(
        (CryptoGeneral/CryptoDevErrorDetect  = 'true') and
        (CryptoDefensiveProgramming/CryptoDefProgEnabled = 'true') and
        (CryptoDefensiveProgramming/CryptoPostcondAssertEnabled = 'true')
      )"!][!//
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined CRYPTO_XVIX_XAIX_UNREACHABLE_CODE_ASSERT_ENABLED)
#error CRYPTO_XVIX_XAIX_UNREACHABLE_CODE_ASSERT_ENABLED is already defined
#endif
/** \brief Unreachable code assertion usage
 *         En- or disables the usage of unreachable code assertion checks
 */
#define CRYPTO_XVIX_XAIX_UNREACHABLE_CODE_ASSERT_ENABLED [!//
[!IF "(
        (CryptoGeneral/CryptoDevErrorDetect  = 'true') and
        (CryptoDefensiveProgramming/CryptoDefProgEnabled = 'true') and
        (CryptoDefensiveProgramming/CryptoUnreachAssertEnabled = 'true')
      )"!][!//
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined CRYPTO_XVIX_XAIX_INVARIANT_ASSERT_ENABLED)
#error CRYPTO_XVIX_XAIX_INVARIANT_ASSERT_ENABLED is already defined
#endif
/** \brief Invariant assertion usage
 *         En- or disables the usage of invariant assertion checks
 */
#define CRYPTO_XVIX_XAIX_INVARIANT_ASSERT_ENABLED [!//
[!IF "(
        (CryptoGeneral/CryptoDevErrorDetect  = 'true') and
        (CryptoDefensiveProgramming/CryptoDefProgEnabled = 'true') and
        (CryptoDefensiveProgramming/CryptoInvariantAssertEnabled = 'true')
      )"!][!//
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined CRYPTO_XVIX_XAIX_STATIC_ASSERT_ENABLED)
#error CRYPTO_XVIX_XAIX_STATIC_ASSERT_ENABLED is already defined
#endif
/** \brief Static assertion usage
 *         En- or disables the usage of static assertion checks
 */
#define CRYPTO_XVIX_XAIX_STATIC_ASSERT_ENABLED [!//
[!IF "(
        (CryptoGeneral/CryptoDevErrorDetect  = 'true') and
        (CryptoDefensiveProgramming/CryptoDefProgEnabled = 'true') and
        (CryptoDefensiveProgramming/CryptoStaticAssertEnabled = 'true')
      )"!][!//
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

/*==================[macros]======================================================================*/

/*==================[declaration of types]========================================================*/

/*==================[declaration of external constants]===========================================*/

/*==================[declaration of external variables]===========================================*/

/*==================[declaration of external functions]===========================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>
[!VAR "rsaPssVrfyFound" = "0"!]
[!VAR "rsaPkcsVrfyFound" = "0"!]
[!VAR "rsaPkcsGenFound" = "0"!]
[!VAR "rsaesOaepEncFound" = "0"!]
[!VAR "rsaesOaepDecFound" = "0"!]
[!LOOP "CryptoDriverObjects/CryptoDriverObject/*/CryptoPrimitiveRef/*"!]
  [!IF "as:ref(.)/CryptoPrimitiveService = 'DECRYPT'"!]
    [!VAR "service" = "'DECRYPT'"!]
    [!VAR "Output" = "'UF'"!]
  [!ELSEIF "as:ref(.)/CryptoPrimitiveService = 'ENCRYPT'"!]
    [!VAR "service" = "'ENCRYPT'"!]
    [!VAR "Output" = "'UF'"!]
  [!ELSEIF "as:ref(.)/CryptoPrimitiveService = 'MAC_GENERATE'"!]
    [!VAR "service" = "'MACGENERATE'"!]
    [!VAR "Output" = "'F'"!]
  [!ELSEIF "as:ref(.)/CryptoPrimitiveService = 'MAC_VERIFY'"!]
    [!VAR "service" = "'MACVERIFY'"!]
    [!VAR "Output" = "'None'"!]

  [!ELSEIF "as:ref(.)/CryptoPrimitiveService = 'AEAD_DECRYPT'"!]
    [!VAR "service" = "'AEADDECRYPT'"!]
    [!VAR "Output" = "'UF'"!]

  [!ELSEIF "as:ref(.)/CryptoPrimitiveService = 'AEAD_ENCRYPT'"!]
    [!VAR "service" = "'AEADENCRYPT'"!]
    [!VAR "Output" = "'UF'"!]


  [!ELSEIF "as:ref(.)/CryptoPrimitiveService = 'HASH'"!]
    [!VAR "service" = "'HASH'"!]
    [!VAR "Output" = "'F'"!]

  [!ELSEIF "as:ref(.)/CryptoPrimitiveService = 'SIGNATURE_GENERATE'"!]
    [!VAR "service" = "'SIGNATUREGENERATE'"!]
    [!VAR "Output" = "'F'"!]

  [!ELSEIF "as:ref(.)/CryptoPrimitiveService = 'SIGNATURE_VERIFY'"!]
    [!VAR "service" = "'SIGNATUREVERIFY'"!]
    [!VAR "Output" = "'None'"!]
  [!ELSEIF "as:ref(.)/CryptoPrimitiveService = 'RANDOM'"!]
    [!VAR "service" = "'RANDOMGENERATE'"!]
    [!VAR "Output" = "'F'"!]
  [!ENDIF!]
  [!LOOP "as:ref(.)/CryptoPrimitiveAlgorithmFamiliy/*"!]
    [!VAR "family" = "node:value(.)"!]
    [!LOOP "../../CryptoPrimitiveAlgorithmMode/*"!]
      [!VAR "mode" = "node:value(.)"!]

      [!IF "($service = 'SIGNATUREVERIFY' and $mode = 'CRYPTO_ALGOMODE_RSASSA_PSS' and $rsaPssVrfyFound = 1) or
            ($service = 'SIGNATUREVERIFY' and $mode = 'CRYPTO_ALGOMODE_RSASSA_PKCS1_v1_5' and $rsaPkcsVrfyFound = 1) or
            ($service = 'SIGNATUREGENERATE' and $mode = 'CRYPTO_ALGOMODE_RSASSA_PKCS1_v1_5' and $rsaPkcsGenFound = 1) or
            ($service = 'ENCRYPT' and $mode = 'CRYPTO_ALGOMODE_RSAES_OAEP' and $rsaesOaepEncFound = 1) or
            ($service = 'DECRYPT' and $mode = 'CRYPTO_ALGOMODE_RSAES_OAEP' and $rsaesOaepDecFound = 1)"!][!//
        [!BREAK!]
      [!ENDIF!]
      [!IF "$service = 'SIGNATUREVERIFY' and $mode = 'CRYPTO_ALGOMODE_RSASSA_PSS' and $rsaPssVrfyFound = 0"!]
        [!VAR "rsaPssVrfyFound" = "1"!]
      [!ELSEIF "$service = 'SIGNATUREVERIFY' and $mode = 'CRYPTO_ALGOMODE_RSASSA_PKCS1_v1_5' and $rsaPkcsVrfyFound = 0"!]
        [!VAR "rsaPkcsVrfyFound" = "1"!]
      [!ELSEIF "$service = 'SIGNATUREGENERATE' and $mode = 'CRYPTO_ALGOMODE_RSASSA_PKCS1_v1_5' and $rsaPkcsGenFound = 0"!]
        [!VAR "rsaPkcsGenFound" = "1"!]
      [!ELSEIF "$service = 'ENCRYPT' and $mode = 'CRYPTO_ALGOMODE_RSAES_OAEP' and $rsaesOaepEncFound = 0"!]
        [!VAR "rsaesOaepEncFound" = "1"!]
      [!ELSEIF "$service = 'DECRYPT' and $mode = 'CRYPTO_ALGOMODE_RSAES_OAEP' and $rsaesOaepDecFound = 0"!]
        [!VAR "rsaesOaepDecFound" = "1"!]
      [!ENDIF!]
/** \brief Process function for [!"$service"!]_[!"text:replace($family,'CRYPTO_ALGOFAM_','')"!]_[!"text:replace($mode,'CRYPTO_ALGOMODE_','')"!]
 *
 *  This function processes the request for service [!"$service"!] with algorithm family
 *  [!"text:replace($family,'CRYPTO_ALGOFAM_','')"!] and algorithm mode
 *  [!"text:replace($mode,'CRYPTO_ALGOMODE_','')"!]
 *
 *  \param[in] ObjectId       Identifier of the crypto driver object that shall process the request
 *  \param[in] Job            Reference to the job that shall be processed
 *
 *  \returns  Result of the job processing
 *  \retval   E_OK                         Job has been successfully processed (synchronous request)
 *                                         or has been forwarded to the crypto engine
 *                                         (asynchronous request)
 *  \retval   E_NOT_OK                     Request failed
 *  \retval   CRYPTO_E_KEY_NOT_VALID       Request failed, the key to be used is not valid
 *  \retval   CRYPTO_E_KEY_SIZE_MISMATCH   Request failed, a key element has the wrong size
 *  \retval   CRYPTO_E_ENTROPY_EXHAUSTION  Request failed, the entropy is exhausted
 *  \retval   CRYPTO_E_SMALL_BUFFER        Request failed, the provided buffer is too small to
 *                                         store the result
 *  \retval   CRYPTO_E_JOB_CANCELED        Request failed, the synchronous job was canceled
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_[!"$service"!]_[!"text:replace($family,'CRYPTO_ALGOFAM_','')"!]_[!"text:replace($mode,'CRYPTO_ALGOMODE_','')"!]_Process
(
  uint32 ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Cancel function for [!"$service"!]_[!"text:replace($family,'CRYPTO_ALGOFAM_','')"!]_[!"text:replace($mode,'CRYPTO_ALGOMODE_','')"!]
 *
 *  This function attempts to cancel the job for service [!"$service"!] with algorithm family
 *  [!"text:replace($family,'CRYPTO_ALGOFAM_','')"!] and algorithm mode
 *  [!"text:replace($mode,'CRYPTO_ALGOMODE_','')"!]
 *
 *  \param[in] ObjectId       Identifier of the crypto driver object that processes the job
 *  \param[in] Job            Reference to the job that shall be cancelled
 *
 *  \returns  Result of the job cancellation attempt
 *  \retval   E_OK                         Job has been successfully cancelled
 *  \retval   E_NOT_OK                     Job could not be cancelled
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_[!"$service"!]_[!"text:replace($family,'CRYPTO_ALGOFAM_','')"!]_[!"text:replace($mode,'CRYPTO_ALGOMODE_','')"!]_Cancel
(
  uint32 ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Process function for [!"$service"!]_[!"text:replace($family,'CRYPTO_ALGOFAM_','')"!]_[!"text:replace($mode,'CRYPTO_ALGOMODE_','')"!]_Process_SyncUpdate
 *
 *  This function processes the sync update request for service [!"$service"!] with algorithm family
 *  [!"text:replace($family,'CRYPTO_ALGOFAM_','')"!] and algorithm mode
 *  [!"text:replace($mode,'CRYPTO_ALGOMODE_','')"!]
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_[!"$service"!]_[!"text:replace($family,'CRYPTO_ALGOFAM_','')"!]_[!"text:replace($mode,'CRYPTO_ALGOMODE_','')"!]_Process_SyncUpdate
(
  [!IF "contains($Output, 'U')"!]uint32 StartOutputLength,[!ENDIF!]
  uint32 ObjectId,
  boolean Busy,
  Std_ReturnType RetVal,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

    [!ENDLOOP!]
  [!ENDLOOP!]
[!ENDLOOP!]

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*================================================================================================*/

#endif /* #ifndef CRYPTO_XVIX_XAIX_INT_CFG_H */

/*==================[end of file]=================================================================*/

