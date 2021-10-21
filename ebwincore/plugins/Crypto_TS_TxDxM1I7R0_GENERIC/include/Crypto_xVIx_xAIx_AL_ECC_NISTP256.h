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

#ifndef CRYPTO_XVIX_XAIX_AL_ECC_NISTP256_H
#define CRYPTO_XVIX_XAIX_AL_ECC_NISTP256_H


/*==[Includes]====================================================================================*/

#include <Std_Types.h>
#include <Crypto_xVIx_xAIx_AL_Cfg.h>
#include <Csm_Types.h>
#include <Crypto_xVIx_xAIx_Cfg.h>
#include <Crypto_xVIx_xAIx_AL_Common.h>

/*==[Macros]======================================================================================*/

/*==[Types]=======================================================================================*/

/*==[Constants with external linkage]=============================================================*/

/*==[Variables with external linkage]=============================================================*/

/*==[Declaration of functions with external linkage]==============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*--ECDSAP256Gen----------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)

/** \brief AL Start Function for ECDSA generate
 *
 *         This Function provides the ECDSA Start Function to call from the upper Layer
 *
 * \param[in]     ObjId   The Driver object ID
 * \param[in,out] Job     A pointer to the processed Job
 *
 * \returns Error code
 * \retval E_OK           If start was successfully requested.
 * \retval E_NOT_OK       If the Key is invalid.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ECCNIST_NOT_SET_Start
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Update Function for ECDSA generate
 *
 *         This Function provides the ECDSA Update Function to call from the upper layer
 *
 * \param[in]     ObjectId  The Driver object ID of the process
 * \param[in,out] Job       The processed Job
 *
 * \returns Error code
 * \retval  E_OK           If the update was successfully requested.
 * \retval  CRYPTO_E_BUSY  If the main function is processing a requested service at the moment
 * \retval  E_NOT_OK       If no ECDSA computation has been started via Start-function.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ECCNIST_NOT_SET_Update
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Finish Function for ECDSA generate
 *
 *         This Function provides the ECDSA Finish Function to call from the upper layer
 *
 * \param[in]     ObjectId  The Driver object ID of the process
 * \param[in,out] Job       The processed Job
 *
 * \returns Error code
 * \retval  E_OK           If the finish was successfully requested.
 * \retval  CRYPTO_E_BUSY  If the main function is processing a requested service at the moment
 * \retval  E_NOT_OK       If no ECDSA computation has been started via Start-function.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ECCNIST_NOT_SET_Finish
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Cancel Function for ECDSA generate
 *
 *         This Function provides the ECDSA Cancel Function to call from the upper layer
 *
 * \param[in]     ObjectId  The Driver object ID of the process
 * \param[in,out] Job       The processed Job
 *
 * \returns Result of the cancellation attempt
 * \retval E_OK                   Job was successfully cancelled.
 * \retval E_NOT_OK               Job could not be cancelled.
 * \retval CRYPTO_E_JOB_CANCELED  Job could not be canceled immediately
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ECCNIST_NOT_SET_Cancel
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Main Function for ECDSA generate
 *
 *         This Function provides the ECDSA Main Function to call from the upper layer
 *
 * \param[out] Result  Pointer where to store the Result of the processing
 * \param[out] Busy    Pointer to where to store if Main Function is busy
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ECCNIST_NOT_SET_MainFunction
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean,       AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Busy
);

/** \brief Helper for Signature generation Sync SingleCall.
 *
 *  This function performs ECDSA signature generation with processing type Synchronous and
 *  mode SingleCall.
 *
 * \param[in]     InputPtr         A pointer to the start of an array which contains data.
 * \param[in]     InputLength      Length of the data in bytes.
 * \param[in]     Key              Key to be used for signature generation.
 * \param[in,out] signaturePtr     A pointer to the start of a buffer which holds the Signature.
 * \param[in,out] signatureLength  Length of the Signature in bytes.
 *
 * \returns Result of the job processing
 * \retval  E_OK           Processing succeeded
 * \retval  E_NOT_OK       Processing failed.
 * \retval  CRYPTO_E_BUSY  If the main function is processing a requested service at the moment.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE)
  Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ECCNIST_NOT_SET_Helper_SyncSingleCall
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                            InputPtr,
  uint32                                                                           InputLength,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                              SignaturePtr,
  uint32                                                                           SignatureLength,
  P2VAR(Crypto_xVIx_xAIx_AsymPublicKeyType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Key
);

#endif /* #if (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON) */

/*--ECDSAP256Vrfy---------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON)

/** \brief AL Start Function for ECDSA verify
 *
 *         This Function provides the ECDSA Start Function to call from the upper Layer
 *
 * \param[in]     ObjId   The Driver object ID
 * \param[in,out] Job     A pointer to the processed Job
 *
 * \returns Error code
 * \retval E_OK           If start was successfully requested.
 * \retval E_NOT_OK       If the Key is invalid.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ECCNIST_NOT_SET_Start
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Update Function for ECDSA verify
 *
 *         This Function provides the ECDSA Update Function to call from the upper layer
 *
 * \param[in]     ObjectId  The Driver object ID of the process
 * \param[in,out] Job       The processed Job
 *
 * \returns Error code
 * \retval E_OK           If the update was successfully requested.
 * \retval CRYPTO_E_BUSY  If the main function is processing a requested service at the moment
 * \retval E_NOT_OK       If no ECDSA computation has been started via Start-function yet
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ECCNIST_NOT_SET_Update
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Finish Function for ECDSA verify
 *
 *         This Function provides the ECDSA Finish Function to call from the upper layer
 *
 * \param[in]     ObjectId  The Driver object ID of the process
 * \param[in,out] Job       The processed Job
 *
 * \returns Error code
 * \retval E_OK           If the finish was successfully requested.
 * \retval CRYPTO_E_BUSY  If the main function is processing a requested service at the moment
 * \retval _E_NOT_OK      If no ECDSA computation has been started via Start-function yet.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ECCNIST_NOT_SET_Finish
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Cancel Function for ECDSA verify
 *
 *         This Function provides the ECDSA Cancel Function to call from the upper layer
 *
 * \param[in]     ObjectId  The Driver object ID of the process
 * \param[in,out] Job       The processed Job
 *
 * \returns Result of the cancellation attempt
 * \retval E_OK                   Job was successfully cancelled.
 * \retval E_NOT_OK               Job could not be cancelled.
 * \retval CRYPTO_E_JOB_CANCELED  Job could not be canceled immediately
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ECCNIST_NOT_SET_Cancel
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Main Function for ECDSA verify
 *
 *         This Function provides the ECDSA Main Function to call from the upper layer
 *
 * \param[out] Result  Pointer where to store the Result of the processing
 * \param[out] Busy    Pointer to where to store if Main Function is busy
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ECCNIST_NOT_SET_MainFunction
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean,       AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Busy
);

/** \brief Helper Signature verification for Sync SingleCall.
 *
 *  This function performs ECDSA signature generation with processing type Synchronous and
 *  mode SingleCall.
 *
 * \param[in]     InputPtr         A pointer to the start of an array which contains data.
 * \param[in]     InputLength      Length of the data in bytes.
 * \param[in]     Key              Key to be used for signature verification.
 * \param[in]     signaturePtr     A pointer to the start of a buffer which holds the Signature.
 * \param[in]     signatureLength  Length of the Signature in bytes.
 * \param[in,out] VerifyPtr        Pointer containing the verification result.
 *
 * \returns Result of the job processing
 * \retval  E_OK           Processing succeeded
 * \retval  E_NOT_OK       Processing failed.
 * \retval  CRYPTO_E_BUSY  If the main function is processing a requested service at the moment.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE)
  Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ECCNIST_NOT_SET_Helper_SyncSingleCall
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                            InputPtr,
  uint32                                                                           InputLength,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                            SignaturePtr,
  uint32                                                                           SignatureLength,
  P2VAR(Crypto_xVIx_xAIx_AsymPublicKeyType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Key,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)            VerifyPtr
);

#endif /* #if (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>


#endif /* CRYPTO_XVIX_XAIX_AL_ECC_NISTP256_H */

/*==[end of file]=================================================================================*/

