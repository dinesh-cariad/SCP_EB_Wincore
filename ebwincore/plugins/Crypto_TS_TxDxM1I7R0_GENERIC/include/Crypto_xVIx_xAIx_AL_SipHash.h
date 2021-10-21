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

#ifndef CRYPTO_XVIX_XAIX_AL_SIPHASH_H
#define CRYPTO_XVIX_XAIX_AL_SIPHASH_H

/*==[Includes]====================================================================================*/

#include <Std_Types.h>
#include <Crypto_xVIx_xAIx_Cfg.h>
#include <Crypto_xVIx_xAIx_AL_Cfg.h>

#if ((CRYPTO_XVIX_XAIX_SIPHASH_GEN_ENABLED == STD_ON) || (CRYPTO_XVIX_XAIX_SIPHASH_VRFY_ENABLED == STD_ON))

/*==[Macros]======================================================================================*/

/*==[Types]=======================================================================================*/

/*==[Constants with external linkage]=============================================================*/

/*==[Variables with external linkage]=============================================================*/

/*==[Declaration of functions with external linkage]==============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*--SIPHASH-GENERATE------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_SIPHASH_GEN_ENABLED == STD_ON)

/** \brief Start function for SipHash Generation
 *
 *  Starts the processing of an SipHash Generation job.
 *
 *  \param[in] ObjectId       Identifier of the crypto driver object that shall
 *                            process the request.
 *  \param[in] Job            Reference to the job that shall be processed.
 *
 *  \returns  Result of the job processing
 *  \retval   E_OK                         Job has been successfully processed
 *                                         (synchronous request) or has been
 *                                         forwarded to the crypto engine
 *                                         (asynchronous request).
 *  \retval   E_NOT_OK                     Request failed.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACGENERATE_SIPHASH_SIPHASH_2_4_Start
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Update function for SipHash Generation
 *
 *  Update function for the processing of an SipHash Generation job.
 *
 *  \param[in] ObjectId       Identifier of the crypto driver object that shall
 *                            process the request.
 *  \param[in] Job            Reference to the job that shall be processed.
 *
 *  \returns  Result of the job processing
 *  \retval   E_OK                         Job has been successfully processed
 *                                         (synchronous request) or has been
 *                                         forwarded to the crypto engine
 *                                         (asynchronous request).
 *  \retval   E_NOT_OK                     Request failed.
 *  \retval   CRYPTO_E_BUSY                Request failed, a previous request was not yet finished.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACGENERATE_SIPHASH_SIPHASH_2_4_Update
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Finish function for SipHash Generation
 *
 *  Finishes the processing of an SipHash Generation job.
 *
 *  \param[in] ObjectId       Identifier of the crypto driver object that shall
 *                            process the request.
 *  \param[in] Job            Reference to the job that shall be processed.
 *
 *  \returns  Result of the job processing
 *  \retval   E_OK                         Job has been successfully processed
 *                                         (synchronous request) or has been
 *                                         forwarded to the crypto engine
 *                                         (asynchronous request).
 *  \retval   E_NOT_OK                     Request failed.
 *  \retval   CRYPTO_E_BUSY                Request failed, a previous request was not yet finished.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACGENERATE_SIPHASH_SIPHASH_2_4_Finish
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Synchronous single call function for SipHash Generation
 *
 *  Performs the SipHash Generation synchronously in a single call.
 *
 *  \param[in]  ObjectId    Identifier of the crypto driver object that shall process the request.
 *  \param[in]  Job         Reference to the job that shall be processed.
 *
 *  \returns  Result of the job processing
 *  \retval   E_OK                         Job has been successfully processed
 *                                         (synchronous request) or has been
 *                                         forwarded to the crypto engine
 *                                         (asynchronous request).
 *  \retval   E_NOT_OK                     Request failed.
 *
 *  \Reentrancy{Reentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACGENERATE_SIPHASH_SIPHASH_2_4_SyncSingleCall
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Cancel function for SipHash Generation
 *
 *  This function attempts to cancel the SipHash Generation.
 *
 *  \param[in]      ObjectId    Identifier of the crypto driver object that processes the request.
 *  \param[in,out]  Job         Reference to the Job that shall be cancelled.
 *
 * \returns Result of the cancellation attempt
 * \retval E_OK                   Job was successfully cancelled.
 * \retval E_NOT_OK               Job could not be cancelled.
 * \retval CRYPTO_E_JOB_CANCELED  Job could not be canceled immediately
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACGENERATE_SIPHASH_SIPHASH_2_4_Cancel
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Main function for SipHash Generation
 *
 *  This function provides the SipHash Generation main function to the upper layer.
 *
 *  \param[out] Result  Pointer to where the result of the processing should be stored.
 *  \param[out] Busy    After the function returns, this location indicates whether the computation
 *                      has finished or whether the primitive is still busy.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACGENERATE_SIPHASH_SIPHASH_2_4_MainFunction
(
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
);

#endif /* #if (CRYPTO_XVIX_XAIX_SIPHASH_GEN_ENABLED == STD_ON) */

/*--CMAC-VERIFY-----------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_SIPHASH_VRFY_ENABLED == STD_ON)

/** \brief Start function for SipHash Verification
 *
 *  Starts the processing of an SipHash Verification job.
 *
 *  \param[in] ObjectId       Identifier of the crypto driver object that shall
 *                            process the request.
 *  \param[in] Job            Reference to the job that shall be processed.
 *
 *  \returns  Result of the job processing
 *  \retval   E_OK                         Job has been successfully processed
 *                                         (synchronous request) or has been
 *                                         forwarded to the crypto engine
 *                                         (asynchronous request).
 *  \retval   E_NOT_OK                     Request failed.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACVERIFY_SIPHASH_SIPHASH_2_4_Start
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Update function for SipHash Verification
 *
 *  Update function for the processing of an SipHash Verification job.
 *
 *  \param[in] ObjectId       Identifier of the crypto driver object that shall
 *                            process the request.
 *  \param[in] Job            Reference to the job that shall be processed.
 *
 *  \returns  Result of the job processing
 *  \retval   E_OK                         Job has been successfully processed
 *                                         (synchronous request) or has been
 *                                         forwarded to the crypto engine
 *                                         (asynchronous request).
 *  \retval   E_NOT_OK                     Request failed.
 *  \retval   CRYPTO_E_BUSY                Request failed, a previous request was not yet finished.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACVERIFY_SIPHASH_SIPHASH_2_4_Update
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Finish function for SipHash Verification
 *
 *  Finishes the processing of an SipHash Verification job.
 *
 *  \param[in] ObjectId       Identifier of the crypto driver object that shall
 *                            process the request.
 *  \param[in] Job            Reference to the job that shall be processed.
 *
 *  \returns  Result of the job processing
 *  \retval   E_OK                         Job has been successfully processed
 *                                         (synchronous request) or has been
 *                                         forwarded to the crypto engine
 *                                         (asynchronous request).
 *  \retval   E_NOT_OK                     Request failed.
 *  \retval   CRYPTO_E_BUSY                Request failed, a previous request was not yet finished.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACVERIFY_SIPHASH_SIPHASH_2_4_Finish
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Synchronous single call function for SipHash Verification
 *
 *  Performs the SipHash Verification synchronously in a single call.
 *
 *  \param[in]  ObjectId    Identifier of the crypto driver object that shall process the request.
 *  \param[in]  Job         Reference to the job that shall be processed.
 *
 *  \returns  Result of the job processing
 *  \retval   E_OK                         Job has been successfully processed
 *                                         (synchronous request) or has been
 *                                         forwarded to the crypto engine
 *                                         (asynchronous request).
 *  \retval   E_NOT_OK                     Request failed.
 *
 *  \Reentrancy{Reentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACVERIFY_SIPHASH_SIPHASH_2_4_SyncSingleCall
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Cancel function for SipHash Verification
 *
 *  This function attempts to cancel the SipHash Verification.
 *
 *  \param[in]      ObjectId    Identifier of the crypto driver object that processes the request.
 *  \param[in,out]  Job         Reference to the Job that shall be cancelled.
 *
 * \returns Result of the cancellation attempt
 * \retval E_OK                   Job was successfully cancelled.
 * \retval E_NOT_OK               Job could not be cancelled.
 * \retval CRYPTO_E_JOB_CANCELED  Job could not be canceled immediately
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACVERIFY_SIPHASH_SIPHASH_2_4_Cancel
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief Main function for SipHash Verification
 *
 *  This function provides the SipHash Verification main function to the upper layer.
 *
 *  \param[out] Result  Pointer to where the result of the processing should be stored.
 *  \param[out] Busy    After the function returns, this location indicates whether the computation
 *                      has finished or whether the primitive is still busy.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_MACVERIFY_SIPHASH_SIPHASH_2_4_MainFunction
(
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
);

#endif /* #if (CRYPTO_XVIX_XAIX_SIPHASH_VRFY_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if ((CRYPTO_XVIX_XAIX_SIPHASH_GEN_ENABLED == STD_ON) || (CRYPTO_XVIX_XAIX_SIPHASH_VRFY_ENABLED == STD_ON)) */

#endif /* CRYPTO_XVIX_XAIX_AL_SIPHASH_H */

/*==[end of file]=================================================================================*/

