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

#ifndef CRYPTO_XVIX_XAIX_AL_SHA1_H
#define CRYPTO_XVIX_XAIX_AL_SHA1_H


/*==[Includes]====================================================================================*/

#include <Std_Types.h>
#include <Crypto_xVIx_xAIx_Cfg.h>
#include <Crypto_xVIx_xAIx_AL_Cfg.h>

#if (CRYPTO_XVIX_XAIX_SHA1_ENABLED  == STD_ON)

/*==[Macros]======================================================================================*/

/* !LINKSTO EB_Crypto_CryptAlgos_0559,1 */
#if (defined CRYPTO_XVIX_XAIX_HASH_SHA1_ALGORITHM)
#error CRYPTO_XVIX_XAIX_HASH_SHA1_ALGORITHM already defined
#endif
/** \brief The algorithm id of the SHA1 primitive which can be set in the
 *         CRYPTO_KE_KEYDERIVATION_ALGORITHM key element.
 */
#define CRYPTO_XVIX_XAIX_HASH_SHA1_ALGORITHM 0x07U
/*==[Types]=======================================================================================*/

/*==[Constants with external linkage]=============================================================*/

/*==[Variables with external linkage]=============================================================*/

/*==[Declaration of functions with external linkage]==============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*--SHA1 Helper-----------------------------------------------------------------------------------*/

/** \brief Helper Start Function for SHA1
 *
 *         This Function provides the SHA1 Start Function to call from another primitive
 *
 * \param[in] ObjId                    The Driver object ID
 * \param[in] Crypto_Generic_Callback  A pointer to the Callback
 * \param[in,out] Job                  A pointer to the processed Job
 * \param[in] ProcessingType           The processing type for the helper
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1HelperStart
(
  CONST(uint32,CRYPTO_xVIx_xAIx_CONST)                         ObjId,
  P2FUNC(void, CRYPTO_xVIx_xAIx_CODE, Crypto_Generic_Callback)
  (
    P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
    uint8                                                        Error
  ),
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  boolean                                                      ProcessingType
);

/** \brief Helper Update Function for SHA1
 *
 *         This Function provides the SHA1 Update Function to call from another primitive
 *
 * \param[in] DataPtr         A pointer to the data
 * \param[in] DataLength      The length of the data
 * \param[in] ProcessingType  The processing type for the helper
 *
 * \returns Error code
 * \retval E_OK           If the update was successfully requested.
 * \retval CRYPTO_E_BUSY  If the main function is processing a requested service at the moment
 * \retval E_NOT_OK       If no SHA computation has been started via Crypto_xVIx_xAIx_SHA1Start(),
 *                        yet
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1HelperUpdate
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) DataPtr,
  uint32                                                DataLength,
  boolean                                               ProcessingType
);


/** \brief Helper Finish Function for SHA1
 *
 *         This Function provides the SHA1 Finsih Function to call from another primitive
 *
 * \param[out] ResultPtr         A pointer to where the result is to be stored
 * \param[in] ResultLengthPtr    A pointer where the length of the result is to be stored
 * \param[in] TruncationAllowed  Defines if truncation is allowed
 * \param[in] ProcessingType     The processing type for the helper
 *
 * \returns Error code
 * \retval E_OK           If the finish was successfully requested.
 * \retval CRYPTO_E_BUSY  If the main function is processing a requested service at the moment
 * \retval E_NOT_OK       If no SHA computation has been started via Crypto_xVIx_xAIx_SHA1Start(),
 *                        yet.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1HelperFinish
(
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  ResultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultLengthPtr,
  boolean                                              ProcessingType
);

/** \brief Helper Main Function for SHA1
 *
 *         This Function provides the SHA1 Main Function to call from another primitive
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1HelperMainFunction
(
  void
);

/** \brief AL Start Function for SHA1
 *
 *         This Function provides the SHA1 Start Function to call from the upper layer
 *
 * \param[in]     ObjectId  The Driver object ID of the process
 * \param[in,out] Job       The processed Job
 *
 * \returns Error code
 * \retval E_OK           If start was successfully requested.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA1_NOT_SET_Start
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Update Function for SHA1
 *
 *         This Function provides the SHA1 Update Function to call from the upper layer
 *
 * \param[in]     ObjectId  The Driver object ID of the process
 * \param[in,out] Job       The processed Job
 *
 * \returns Error code
 * \retval E_OK           If the update was successfully requested.
 * \retval CRYPTO_E_BUSY  If the main function is processing a requested service at the moment
 * \retval E_NOT_OK       If no SHA computation has been started via Start-function yet
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA1_NOT_SET_Update
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Finish Function for SHA1
 *
 *         This Function provides the SHA1 Finish Function to call from the upper layer
 *
 * \param[in]     ObjectId  The Driver object ID of the process
 * \param[in,out] Job       The processed Job
 *
 * \returns Error code
 * \retval E_OK           If the finish was successfully requested.
 * \retval CRYPTO_E_BUSY  If the main function is processing a requested service at the moment
 * \retval _E_NOT_OK      If no SHA computation has been started via Start-function yet.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA1_NOT_SET_Finish
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Cancel Function for SHA1
 *
 *         This Function provides the SHA1 Cancel Function to call from the upper layer
 *
 * \param[in]     ObjectId  The Driver object ID of the process
 * \param[in,out] Job       The processed Job
 *
 * \returns Result of the cancellation attempt
 * \retval E_OK                   Job was successfully cancelled.
 * \retval E_NOT_OK               Job could not be cancelled.
 * \retval CRYPTO_E_JOB_CANCELED  Job could not be canceled immediately
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA1_NOT_SET_Cancel
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);


/** \brief AL Main Function for SHA1
 *
 *         This Function provides the SHA1 Main Function to call from the upper layer
 *
 * \param[out] Result  Pointer where to store the Result of the processing
 * \param[out] Busy    Pointer to where to store if Main Function is busy
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA1_NOT_SET_MainFunction
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
);

/**  \brief  Crypto SHA1 Reset
 *
 *           This Function resets the Context of SHA1.
 *
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA1_NOT_SET_Reset
(
  void
);
#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if (CRYPTO_XVIX_XAIX_SHA1_ENABLED == STD_ON) */

#endif /* CRYPTO_XVIX_XAIX_AL_SHA1_H */

/*==[end of file]=================================================================================*/

