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

#ifndef CRYPTO_XVIX_XAIX_AL_HASH_H
#define CRYPTO_XVIX_XAIX_AL_HASH_H

/*==[Includes]====================================================================================*/

#include <Std_Types.h>
#include <Crypto_xVIx_xAIx_AL_Cfg.h>
#include <Crypto_xVIx_xAIx_Cfg.h>

/*==[Macros]======================================================================================*/

/* !LINKSTO EB_Crypto_CryptAlgos_0555,1 */
#if (defined CRYPTO_XVIX_XAIX_HASH_SHA2_224_ALGORITHM)
#error CRYPTO_XVIX_XAIX_HASH_SHA2_224_ALGORITHM already defined
#endif
/** \brief The algorithm id of the SHA2 224 primitive which can be set in the
 *         CRYPTO_KE_KEYDERIVATION_ALGORITHM key element.
 */
#define CRYPTO_XVIX_XAIX_HASH_SHA2_224_ALGORITHM 0x03U

/* !LINKSTO EB_Crypto_CryptAlgos_0556,1 */
#if (defined CRYPTO_XVIX_XAIX_HASH_SHA2_256_ALGORITHM)
#error CRYPTO_XVIX_XAIX_HASH_SHA2_256_ALGORITHM already defined
#endif
/** \brief The algorithm id of the SHA2 256 primitive which can be set in the
 *         CRYPTO_KE_KEYDERIVATION_ALGORITHM key element.
 */
#define CRYPTO_XVIX_XAIX_HASH_SHA2_256_ALGORITHM 0x04U

/* !LINKSTO EB_Crypto_CryptAlgos_0557,1 */
#if (defined CRYPTO_XVIX_XAIX_HASH_SHA2_384_ALGORITHM)
#error CRYPTO_XVIX_XAIX_HASH_SHA2_384_ALGORITHM already defined
#endif
/** \brief The algorithm id of the SHA2 384 primitive which can be set in the
 *         CRYPTO_KE_KEYDERIVATION_ALGORITHM key element.
 */
#define CRYPTO_XVIX_XAIX_HASH_SHA2_384_ALGORITHM 0x05U

/* !LINKSTO EB_Crypto_CryptAlgos_0558,1 */
#if (defined CRYPTO_XVIX_XAIX_HASH_SHA2_512_ALGORITHM)
#error CRYPTO_XVIX_XAIX_HASH_SHA2_512_ALGORITHM already defined
#endif
/** \brief The algorithm id of the SHA2 512 primitive which can be set in the
 *         CRYPTO_KE_KEYDERIVATION_ALGORITHM key element.
 */
#define CRYPTO_XVIX_XAIX_HASH_SHA2_512_ALGORITHM 0x06U

/*==[Types]=======================================================================================*/

/*==[Constants with external linkage]=============================================================*/

/*==[Variables with external linkage]=============================================================*/

/*==[Declaration of functions with external linkage]==============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*--SHA-Common------------------------------------------------------------------------------------*/

/**  \brief  Crypto SHA Reset
 *
 *           This Function resets the Context of SHA.
 *
 * \returns  void
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_NOT_SET_Reset
(
  void
);

/*--SHA2 Helper-----------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_SHA2_224_ENABLED == STD_ON)

/** \brief Helper Start Function for SHA2224
 *
 *         This Function provides the SHA2224 Start Function to call from another primitive
 *
 * \param[in] ObjId                    The Driver object ID
 * \param[in] Crypto_Generic_Callback  A pointer to the Callback
 * \param[in,out] Job                  A pointer to the processed Job
 * \param[in] ProcessingType           The processing type for the helper
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2224HelperStart
(
  CONST(uint32,CRYPTO_xVIx_xAIx_CONST)                         ObjId,
  P2FUNC(void, CRYPTO_xVIx_xAIx_CODE, Crypto_Generic_Callback)
  (
    P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
    uint8                                                        Error
  ),
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Job,
  boolean                                                        ProcessingType
);

/** \brief Helper Update Function for SHA2224
 *
 *         This Function provides the SHA2224 Update Function to call from another primitive
 *
 * \param[in] DataPtr         A pointer to the data
 * \param[in] DataLength      The length of the data
 * \param[in] ProcessingType  The processing type for the helper
 *
 * \returns Error code
 * \retval E_OK           If the update was successfully requested.
 * \retval CRYPTO_E_BUSY  If the main function is processing a requested service at the moment
 * \retval E_NOT_OK       If no SHA computation has been started via Crypto_xVIx_xAIx_SHA2Start(), yet
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2224HelperUpdate
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) DataPtr,
  uint32                                                DataLength,
  boolean                                               ProcessingType
);

/** \brief Helper Finish Function for SHA2224
 *
 *         This Function provides the SHA2224 Finsih Function to call from another primitive
 *
 * \param[out] ResultPtr         A pointer to where the result is to be stored
 * \param[in] ResultLengthPtr    A pointer where the length of the result is to be stored
 * \param[in] TruncationAllowed  Defines if truncation is allowed
 * \param[in] ProcessingType     The processing type for the helper
 *
 * \returns Error code
 * \retval E_OK           If the finish was successfully requested.
 * \retval CRYPTO_E_BUSY  If the main function is processing a requested service at the moment
 * \retval E_NOT_OK       If no SHA computation has been started via Crypto_xVIx_xAIx_SHA2Start(), yet.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2224HelperFinish
(
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  ResultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultLengthPtr,
  boolean                                              TruncationAllowed,
  boolean                                              ProcessingType
);

/** \brief Helper Main Function for SHA2224
 *
 *         This Function provides the SHA2224 Main Function to call from another primitive
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2224HelperMainFunction
(
  void
);

#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_224_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON)

/** \brief Helper Start Function for SHA2256
 *
 *         This Function provides the SHA2256 Start Function to call from another primitive
 *
 * \param[in] ObjId                    The Driver object ID
 * \param[in] Crypto_Generic_Callback  A pointer to the Callback
 * \param[in,out] Job                  A pointer to the processed Job
 * \param[in] ProcessingType           The processing type for the helper
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2256HelperStart
(
  CONST(uint32,CRYPTO_xVIx_xAIx_CONST)                         ObjId,
  P2FUNC(void, CRYPTO_xVIx_xAIx_CODE, Crypto_Generic_Callback)
  (
    P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
    uint8                                                        Error
  ),
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Job,
  boolean                                                        ProcessingType
);

/** \brief Helper Update Function for SHA2256
 *
 *         This Function provides the SHA2256 Update Function to call from another primitive
 *
 * \param[in] DataPtr         A pointer to the data
 * \param[in] DataLength      The length of the data
 * \param[in] ProcessingType  The processing type for the helper
 *
 * \returns Error code
 * \retval E_OK           If the update was successfully requested.
 * \retval CRYPTO_E_BUSY  If the main function is processing a requested service at the moment
 * \retval E_NOT_OK       If no SHA computation has been started via Crypto_xVIx_xAIx_SHA2Start(), yet
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2256HelperUpdate
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) DataPtr,
  uint32                                                DataLength,
  boolean                                               ProcessingType
);

/** \brief Helper Finish Function for SHA2256
 *
 *         This Function provides the SHA2256 Finsih Function to call from another primitive
 *
 * \param[out] ResultPtr         A pointer to where the result is to be stored
 * \param[in] ResultLengthPtr    A pointer where the length of the result is to be stored
 * \param[in] TruncationAllowed  Defines if truncation is allowed
 * \param[in] ProcessingType     The processing type for the helper
 *
 * \returns Error code
 * \retval E_OK           If the finish was successfully requested.
 * \retval CRYPTO_E_BUSY  If the main function is processing a requested service at the moment
 * \retval E_NOT_OK       If no SHA computation has been started via Crypto_xVIx_xAIx_SHA2Start(), yet.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2256HelperFinish
(
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  ResultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultLengthPtr,
  boolean                                              TruncationAllowed,
  boolean                                              ProcessingType
);

/** \brief Helper for calling Sha2 256 in mode synchronous singlecall.
 *
 * \param[in] InputPtr          Pointer to input data for Sha2 256
 * \param[in] InputLength       Length of the input data
 * \param[out] OutputPtr        Pointer to the output data of Sha2 256
 * \param[out] OutputLengthPtr  Length of the output data

 * \return Result of the synchronous call of Sha2 256
 * \retval E_OK     Sha2 256 has been successfully processed
 * \retval E_NOT_OK Request failed
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2256HelperSyncSingleCall
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) InputPtr,
  uint32                                                InputLength,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   OutputPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  OutputLengthPtr
);

/** \brief Helper Main Function for SHA2256
 *
 *         This Function provides the SHA2256 Main Function to call from another primitive
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2256HelperMainFunction
(
  void
);

#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_SHA2_384_ENABLED == STD_ON)

/** \brief Helper Start Function for SHA2384
 *
 *         This Function provides the SHA2384 Start Function to call from another primitive
 *
 * \param[in] ObjId                    The Driver object ID
 * \param[in] Crypto_Generic_Callback  A pointer to the Callback
 * \param[in,out] Job                  A pointer to the processed Job
 * \param[in] ProcessingType           The processing type for the helper
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2384HelperStart
(
  CONST(uint32,CRYPTO_xVIx_xAIx_CONST)                         ObjId,
  P2FUNC(void, CRYPTO_xVIx_xAIx_CODE, Crypto_Generic_Callback)
  (
    P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
    uint8                                                        Error
  ),
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Job,
  boolean                                                        ProcessingType
);

/** \brief Helper Update Function for SHA2384
 *
 *         This Function provides the SHA2384 Update Function to call from another primitive
 *
 * \param[in] DataPtr         A pointer to the data
 * \param[in] DataLength      The length of the data
 * \param[in] ProcessingType  The processing type for the helper
 *
 * \returns Error code
 * \retval E_OK           If the update was successfully requested.
 * \retval CRYPTO_E_BUSY  If the main function is processing a requested service at the moment
 * \retval E_NOT_OK       If no SHA computation has been started via Crypto_xVIx_xAIx_SHA2Start(), yet
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2384HelperUpdate
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) DataPtr,
  uint32                                                DataLength,
  boolean                                               ProcessingType
);

/** \brief Helper Finish Function for SHA2384
 *
 *         This Function provides the SHA2384 Finsih Function to call from another primitive
 *
 * \param[out] ResultPtr         A pointer to where the result is to be stored
 * \param[in] ResultLengthPtr    A pointer where the length of the result is to be stored
 * \param[in] TruncationAllowed  Defines if truncation is allowed
 * \param[in] ProcessingType     The processing type for the helper
 *
 * \returns Error code
 * \retval E_OK           If the finish was successfully requested.
 * \retval CRYPTO_E_BUSY  If the main function is processing a requested service at the moment
 * \retval E_NOT_OK       If no SHA computation has been started via Crypto_xVIx_xAIx_SHA2Start(), yet.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2384HelperFinish
(
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  ResultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultLengthPtr,
  boolean                                              TruncationAllowed,
  boolean                                              ProcessingType
);

/** \brief Helper Main Function for SHA2384
 *
 *         This Function provides the SHA2384 Main Function to call from another primitive
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2384HelperMainFunction
(
  void
);

#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_384_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_SHA2_512_ENABLED == STD_ON)

/** \brief Helper Start Function for SHA2512
 *
 *         This Function provides the SHA2512 Start Function to call from another primitive
 *
 * \param[in] ObjId                    The Driver object ID
 * \param[in] Crypto_Generic_Callback  A pointer to the Callback
 * \param[in,out] Job                  A pointer to the processed Job
 * \param[in] ProcessingType           The processing type for the helper
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2512HelperStart
(
  const uint32                                       ObjId,
  P2FUNC(void, CRYPTO_xVIx_xAIx_CODE, Crypto_Generic_Callback)
  (
    P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
    uint8                                                        Error
  ),
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Job,
  boolean                                                        ProcessingType
);

/** \brief Helper Update Function for SHA2512
 *
 *         This Function provides the SHA2512 Update Function to call from another primitive
 *
 * \param[in] DataPtr         A pointer to the data
 * \param[in] DataLength      The length of the data
 * \param[in] ProcessingType  The processing type for the helper
 *
 * \returns Error code
 * \retval E_OK           If the update was successfully requested.
 * \retval CRYPTO_E_BUSY  If the main function is processing a requested service at the moment
 * \retval E_NOT_OK       If no SHA computation has been started via Crypto_xVIx_xAIx_SHA2Start(), yet
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2512HelperUpdate
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) DataPtr,
  uint32                                                DataLength,
  boolean                                               ProcessingType
);

/** \brief Helper Finish Function for SHA2512
 *
 *         This Function provides the SHA2512 Finsih Function to call from another primitive
 *
 * \param[out] ResultPtr         A pointer to where the result is to be stored
 * \param[in] ResultLengthPtr    A pointer where the length of the result is to be stored
 * \param[in] TruncationAllowed  Defines if truncation is allowed
 * \param[in] ProcessingType     The processing type for the helper
 *
 * \returns Error code
 * \retval E_OK           If the finish was successfully requested.
 * \retval CRYPTO_E_BUSY  If the main function is processing a requested service at the moment
 * \retval E_NOT_OK       If no SHA computation has been started via Crypto_xVIx_xAIx_SHA2Start(), yet.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2512HelperFinish
(
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  ResultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultLengthPtr,
  boolean                                              TruncationAllowed,
  boolean                                              ProcessingType
);

/** \brief Helper Main Function for SHA2512
 *
 *         This Function provides the SHA2512 Main Function to call from another primitive
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2512HelperMainFunction
(
  void
);

#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_512_ENABLED == STD_ON) */

/*--SHA2 224 AL FUNCTIONS-------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_SHA2_224_ENABLED == STD_ON)

/** \brief AL Start Function for SHA2224
 *
 *         This Function provides the SHA2224 Start Function to call from the upper layer
 *
 * \param[in] ObjectId  The Driver object ID of the process
 * \param[in,out] Job   The processed Job
 *
 * \returns Error code
 * \retval E_OK           If start was successfully requested.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_224_NOT_SET_Start
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Update Function for SHA2224
 *
 *         This Function provides the SHA2224 Update Function to call from the upper layer
 *
 * \param[in] ObjectId  The Driver object ID of the process
 * \param[in,out] Job   The processed Job
 *
 * \returns Error code
 * \retval E_OK           If the update was successfully requested.
 * \retval CRYPTO_E_BUSY  If the main function is processing a requested service at the moment
 * \retval E_NOT_OK       If no SHA computation has been started via Crypto_xVIx_xAIx_SHA2Start(), yet
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_224_NOT_SET_Update
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Finish Function for SHA2224
 *
 *         This Function provides the SHA2512 Finish Function to call from the upper layer
 *
 * \param[in] ObjectId  The Driver object ID of the process
 * \param[in,out] Job   The processed Job
 *
 * \returns Error code
 * \retval E_OK           If the finish was successfully requested.
 * \retval CRYPTO_E_BUSY  If the main function is processing a requested service at the moment
 * \retval _E_NOT_OK      If no SHA computation has been started via Crypto_xVIx_xAIx_SHA2Start(), yet.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_224_NOT_SET_Finish
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Cancel Function for SHA2224
 *
 *         This Function provides the SHA2512 Cancel Function to call from the upper layer
 *
 * \param[in] ObjectId  The Driver object ID of the process
 * \param[in,out] Job   The processed Job
 *
 * \returns Result of the cancellation attempt
 * \retval E_OK                   Job was successfully cancelled.
 * \retval E_NOT_OK               Job could not be cancelled.
 * \retval CRYPTO_E_JOB_CANCELED  Job could not be canceled immediately
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_224_NOT_SET_Cancel
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Main Function for SHA2224
 *
 *         This Function provides the SHA2224 Main Function to call from the upper layer
 *
 * \param[out] Result  Pointer where to store the Result of the processing
 * \param[out] Busy    Pointer to where to store if Main Function is busy
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_224_NOT_SET_MainFunction
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
);

#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_224_ENABLED == STD_ON) */

/*--SHA2 256 AL FUNCTIONS-------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON)

/** \brief AL Start Function for SHA2256
 *
 *         This Function provides the SHA2256 Start Function to call from the upper layer
 *
 * \param[in] ObjectId  The Driver object ID of the process
 * \param[in,out] Job   The processed Job
 *
 * \returns Error code
 * \retval E_OK           If start was successfully requested.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_Start
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Update Function for SHA2256
 *
 *         This Function provides the SHA2256 Update Function to call from the upper layer
 *
 * \param[in] ObjectId  The Driver object ID of the process
 * \param[in,out] Job   The processed Job
 *
 * \returns Error code
 * \retval E_OK           If the update was successfully requested.
 * \retval CRYPTO_E_BUSY  If the main function is processing a requested service at the moment
 * \retval E_NOT_OK       If no SHA computation has been started via Crypto_xVIx_xAIx_SHA2Start(), yet
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_Update
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Finish Function for SHA2256
 *
 *         This Function provides the SHA2512 Finish Function to call from the upper layer
 *
 * \param[in] ObjectId  The Driver object ID of the process
 * \param[in,out] Job   The processed Job
 *
 * \returns Error code
 * \retval E_OK           If the finish was successfully requested.
 * \retval CRYPTO_E_BUSY  If the main function is processing a requested service at the moment
 * \retval _E_NOT_OK      If no SHA computation has been started via Crypto_xVIx_xAIx_SHA2Start(), yet.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_Finish
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Cancel Function for SHA2256
 *
 *         This Function provides the SHA2512 Cancel Function to call from the upper layer
 *
 * \param[in] ObjectId  The Driver object ID of the process
 * \param[in,out] Job   The processed Job
 *
 * \returns Result of the cancellation attempt
 * \retval E_OK                   Job was successfully cancelled.
 * \retval E_NOT_OK               Job could not be cancelled.
 * \retval CRYPTO_E_JOB_CANCELED  Job could not be canceled immediately
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_Cancel
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Main Function for SHA2256
 *
 *         This Function provides the SHA2256 Main Function to call from the upper layer
 *
 * \param[out] Result  Pointer where to store the Result of the processing
 * \param[out] Busy    Pointer to where to store if Main Function is busy
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_MainFunction
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
);

#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON) */

/*--SHA2 384 AL FUNCTIONS-------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_SHA2_384_ENABLED == STD_ON)

/** \brief AL Start Function for SHA2384
 *
 *         This Function provides the SHA2384 Start Function to call from the upper layer
 *
 * \param[in] ObjectId  The Driver object ID of the process
 * \param[in,out] Job   The processed Job
 *
 * \returns Error code
 * \retval E_OK           If start was successfully requested.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_384_NOT_SET_Start
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Update Function for SHA2384
 *
 *         This Function provides the SHA2384 Update Function to call from the upper layer
 *
 * \param[in] ObjectId  The Driver object ID of the process
 * \param[in,out] Job   The processed Job
 *
 * \returns Error code
 * \retval E_OK           If the update was successfully requested.
 * \retval CRYPTO_E_BUSY  If the main function is processing a requested service at the moment
 * \retval E_NOT_OK       If no SHA computation has been started via Crypto_xVIx_xAIx_SHA2Start(), yet
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_384_NOT_SET_Update
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Finish Function for SHA2384
 *
 *         This Function provides the SHA2512 Finish Function to call from the upper layer
 *
 * \param[in] ObjectId  The Driver object ID of the process
 * \param[in,out] Job   The processed Job
 *
 * \returns Error code
 * \retval E_OK           If the finish was successfully requested.
 * \retval CRYPTO_E_BUSY  If the main function is processing a requested service at the moment
 * \retval _E_NOT_OK      If no SHA computation has been started via Crypto_xVIx_xAIx_SHA2Start(), yet.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_384_NOT_SET_Finish
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Cancel Function for SHA2384
 *
 *         This Function provides the SHA2512 Cancel Function to call from the upper layer
 *
 * \param[in] ObjectId  The Driver object ID of the process
 * \param[in,out] Job   The processed Job
 *
 * \returns Result of the cancellation attempt
 * \retval E_OK                   Job was successfully cancelled.
 * \retval E_NOT_OK               Job could not be cancelled.
 * \retval CRYPTO_E_JOB_CANCELED  Job could not be canceled immediately
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_384_NOT_SET_Cancel
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Main Function for SHA2384
 *
 *         This Function provides the SHA2384 Main Function to call from the upper layer
 *
 * \param[out] Result  Pointer where to store the Result of the processing
 * \param[out] Busy    Pointer to where to store if Main Function is busy
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_384_NOT_SET_MainFunction
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
);

#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_384_ENABLED == STD_ON) */

/*--SHA2 512 AL FUNCTIONS-------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_SHA2_512_ENABLED == STD_ON)

/** \brief AL Start Function for SHA2512
 *
 *         This Function provides the SHA2512 Start Function to call from the upper layer
 *
 * \param[in] ObjectId  The Driver object ID of the process
 * \param[in,out] Job   The processed Job
 *
 * \returns Error code
 * \retval E_OK           If start was successfully requested.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_512_NOT_SET_Start
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Update Function for SHA2512
 *
 *         This Function provides the SHA2512 Update Function to call from the upper layer
 *
 * \param[in] ObjectId  The Driver object ID of the process
 * \param[in,out] Job   The processed Job
 *
 * \returns Error code
 * \retval E_OK           If the update was successfully requested.
 * \retval CRYPTO_E_BUSY  If the main function is processing a requested service at the moment
 * \retval E_NOT_OK       If no SHA computation has been started via Crypto_xVIx_xAIx_SHA2Start(), yet
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_512_NOT_SET_Update
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Finish Function for SHA2512
 *
 *         This Function provides the SHA2512 Finish Function to call from the upper layer
 *
 * \param[in] ObjectId  The Driver object ID of the process
 * \param[in,out] Job   The processed Job
 *
 * \returns Error code
 * \retval E_OK           If the finish was successfully requested.
 * \retval CRYPTO_E_BUSY  If the main function is processing a requested service at the moment
 * \retval _E_NOT_OK      If no SHA computation has been started via Crypto_xVIx_xAIx_SHA2Start(), yet.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_512_NOT_SET_Finish
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Cancel Function for SHA2512
 *
 *         This Function provides the SHA2512 Cancel Function to call from the upper layer
 *
 * \param[in] ObjectId  The Driver object ID of the process
 * \param[in,out] Job   The processed Job
 *
 * \returns Result of the cancellation attempt
 * \retval E_OK                   Job was successfully cancelled.
 * \retval E_NOT_OK               Job could not be cancelled.
 * \retval CRYPTO_E_JOB_CANCELED  Job could not be canceled immediately
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_512_NOT_SET_Cancel
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
);

/** \brief AL Main Function for SHA2512
 *
 *         This Function provides the SHA2512 Main Function to call from the upper layer
 *
 * \param[out] Result  Pointer where to store the Result of the processing
 * \param[out] Busy    Pointer to where to store if Main Function is busy
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_512_NOT_SET_MainFunction
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
);

#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_512_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #ifndef CRYPTO_XVIX_XAIX_SHA2_H */

/*==[end of file]=================================================================================*/

