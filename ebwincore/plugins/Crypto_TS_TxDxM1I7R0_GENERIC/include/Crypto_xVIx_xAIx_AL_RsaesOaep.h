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

#ifndef CRYPTO_XVIX_XAIX_RSAESOAEP_H
#define CRYPTO_XVIX_XAIX_RSAESOAEP_H

/*==[Includes]====================================================================================*/

#include <Std_Types.h>
#include <Csm_Types.h>

#if (                                                                                              \
      (STD_ON == CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_ENABLED) ||                                        \
      (STD_ON == CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_ENABLED)                                           \
    )

/*==[Macros]======================================================================================*/

/*==[Types]=======================================================================================*/

/*==[Constants with external linkage]=============================================================*/

/*==[Variables with external linkage]=============================================================*/

/*==[Declaration of functions with external linkage]==============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#if(STD_ON == CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_ENABLED)
/** \brief AL Start function for RSAES-OAEP encrypt
 *
 * This function provides the RSAES-OAEP encrypt Start function to call from the upper layer.
 *
 * \param[in] ObjId    The Driver object ID.
 * \param[in,out] Job  A pointer to the processed Job.
 *
 * \returns Error code.
 * \retval E_OK      If start was successfully requested.
 * \retval E_NOT_OK  If the key is invalid.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_RSA_RSAES_OAEP_Start
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Update function for RSAES-OAEP encrypt
 *
 * This function provides the RSAES-OAEP encrypt Update function to call from the upper layer.
 *
 * \param[in] ObjectId  The Driver object ID of the process.
 * \param[in,out] Job   The processed Job.
 *
 * \returns Error code.
 * \retval E_OK           If the update was successfully requested.
 * \retval CRYPTO_E_BUSY  Request failed, a service is already beeing processed.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_RSA_RSAES_OAEP_Update
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Finish function for RSAES-OAEP encrypt
 *
 * This function provides the RSAES-OAEP encrypt Finish function to call from the upper layer.
 *
 * \param[in] ObjectId  The Driver object ID of the process.
 * \param[in,out] Job   The processed Job.
 *
 * \returns Error code.
 * \retval E_OK           If the finish was successfully requested.
 * \retval CRYPTO_E_BUSY  Request failed, a service is already beeing processed.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_RSA_RSAES_OAEP_Finish
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Cancel function for RSAES-OAEP encrypt.
 *
 * This function provides the RSAES-OAEP encrypt Cancel function to call from the upper layer.
 *
 * \param[in] ObjectId  The Driver object ID of the process.
 * \param[in,out] Job   The processed Job.
 *
 * \returns Result of the cancellation attempt.
 * \retval E_OK                   Job was successfully cancelled.
 * \retval E_NOT_OK               Job could not be cancelled.
 * \retval CRYPTO_E_JOB_CANCELED  Job could not be canceled immediately.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_RSA_RSAES_OAEP_Cancel
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Perform the RSAES-OAEP encryption tasks.
 *
 * This function performs the actual RSAES-OAEP encryption
 * tasks which has been requested by the service functions.
 *
 * \param[out] Result  Pointer where to store the Result of the processing.
 * \param[out] Busy    Pointer to where to store if Main function is busy.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Asynchronous}
 *
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_RSA_RSAES_OAEP_MainFunction
(
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
);

#endif /* #if(STD_ON == CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_ENABLED) */

#if (STD_ON == CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_ENABLED)
/** \brief AL Start function for RSAES-OAEP decrypt
 *
 * This function provides the RSAES-OAEP decrypt Start function to call from the upper layer.
 *
 * \param[in] ObjId    The Driver object ID.
 * \param[in,out] Job  A pointer to the processed Job.
 *
 * \returns Error code.
 * \retval E_OK      If start was successfully requested.
 * \retval E_NOT_OK  If the key is invalid.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_RSA_RSAES_OAEP_Start
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Update function for RSAES-OAEP decrypt
 *
 * This function provides the RSAES-OAEP decrypt Update function to call from the upper layer.
 *
 * \param[in] ObjectId  The Driver object ID of the process.
 * \param[in,out] Job   The processed Job.
 *
 * \returns Error code.
 * \retval E_OK           If the update was successfully requested.
 * \retval CRYPTO_E_BUSY  If the main function is processing a requested service at the moment.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_RSA_RSAES_OAEP_Update
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Finish function for RSAES-OAEP decrypt
 *
 * This function provides the RSAES-OAEP decrypt Finish function to call from the upper layer.
 *
 * \param[in] ObjectId  The Driver object ID of the process.
 * \param[in,out] Job   The processed Job.
 *
 * \returns Error code.
 * \retval E_OK           If the finish was successfully requested.
 * \retval CRYPTO_E_BUSY  If the main function is processing a requested service at the moment.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_RSA_RSAES_OAEP_Finish
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Cancel function for RSAES-OAEP decrypt.
 *
 * This function provides the RSAES-OAEP decrypt Cancel function to call from the upper layer.
 *
 * \param[in] ObjectId  The Driver object ID of the process.
 * \param[in,out] Job   The processed Job.
 *
 * \returns Result of the cancellation attempt.
 * \retval E_OK                   Job was successfully cancelled.
 * \retval E_NOT_OK               Job could not be cancelled.
 * \retval CRYPTO_E_JOB_CANCELED  Job could not be canceled immediately.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_RSA_RSAES_OAEP_Cancel
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Perform the RSAES-OAEP decryption tasks.
 *
 * This function performs the actual RSAES-OAEP decryption
 * tasks which has been requested by the service functions.
 *
 * \param[out] Result  Pointer where to store the Result of the processing.
 * \param[out] Busy    Pointer to where to store if Main function is busy.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Asynchronous}
 *
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_RSA_RSAES_OAEP_MainFunction
(
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
);

#endif /* #if (STD_ON == CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_ENABLED) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif

#endif /* CRYPTO_XVIX_XAIX_RSAESOAEP_H */

/*==[end of file]=================================================================================*/

