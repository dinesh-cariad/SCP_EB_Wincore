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

/*==[Includes]====================================================================================*/

#include <Std_Types.h>
#include <TSAutosar.h>
#include <Csm_Types.h>
#include <CryIf_Cbk.h>
#include <SchM_Crypto_xVIx_xAIx.h>
#include <TSMem.h>

#include <Crypto_xVIx_xAIx.h>
#include <Crypto_xVIx_xAIx_AL_SHA1.h>
#include <Crypto_xVIx_xAIx_AL_Common.h>
#include <Crypto_xVIx_xAIx_AL_Cfg.h>

#if(CRYPTO_XVIX_XAIX_SHA1_ENABLED == STD_ON)

/*==[Macros]======================================================================================*/

#if (defined CRYPTO_XVIX_XAIX_SHA1_DEFAULT_CURRENT_MODE)
#error CRYPTO_XVIX_XAIX_SHA1_DEFAULT_CURRENT_MODE is already defined
#endif
/** \brief To set the default current mode */
#define CRYPTO_XVIX_XAIX_SHA1_DEFAULT_CURRENT_MODE 0x00U

#if (defined CRYPTO_XVIX_XAIX_SHA1_OUTPUT_LEN_BYTES)
#error CRYPTO_XVIX_XAIX_SHA1_OUTPUT_LEN_BYTES is already defined
#endif
/** \brief The length of a message digest computed with the SHA-1 algorithm in bytes.*/
#define CRYPTO_XVIX_XAIX_SHA1_OUTPUT_LEN_BYTES 20U

#if (defined CRYPTO_XVIX_XAIX_SHA1_STATE_WORDS)
#error CRYPTO_XVIX_XAIX_SHA1_STATE_WORDS is already defined
#endif
/** \brief Size of the internal state
 *         The hash algorithm uses a state which is modified by processing the input.
 *         The state after processing the whole input is the hash digest. This macro
 *         gives the length of the SHA-1 state in 32-bit words.
 */
#define CRYPTO_XVIX_XAIX_SHA1_STATE_WORDS (CRYPTO_XVIX_XAIX_SHA1_OUTPUT_LEN_BYTES >> 2U)

#if (defined CRYPTO_XVIX_XAIX_SHA1_ROUNDS)
#error CRYPTO_XVIX_XAIX_SHA1_ROUNDS is already defined
#endif
/** \brief The number of rounds performed by the SHA-1 algorithm */
#define CRYPTO_XVIX_XAIX_SHA1_ROUNDS 80U

#if (defined CRYPTO_XVIX_XAIX_SHA1_BLOCK_LEN_WORDS)
#error CRYPTO_XVIX_XAIX_SHA1_BLOCK_LEN_WORDS is already defined
#endif
/** \brief A message which should be hashed is divided into blocks which are processed
 *         individually. This macro gives the length of a block in words.
 */
#define CRYPTO_XVIX_XAIX_SHA1_BLOCK_LEN_WORDS 16U

#if (defined CRYPTO_XVIX_XAIX_SHA1_BLOCK_LEN_BYTES)
#error CRYPTO_XVIX_XAIX_SHA1_BLOCK_LEN_BYTES is already defined
#endif
/** \brief A message which should be hashed is divided into blocks which are processed
 *         individually. This macro gives the length of a block in bytes.
 */
#define CRYPTO_XVIX_XAIX_SHA1_BLOCK_LEN_BYTES (CRYPTO_XVIX_XAIX_SHA1_BLOCK_LEN_WORDS << 2U)

/*--SHA1StateType---------------------------------------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_SHA1_IDLE)
#error CRYPTO_XVIX_XAIX_SHA1_IDLE is already defined
#endif
/** \brief The Idle state of the SHA computation*/
#define CRYPTO_XVIX_XAIX_SHA1_IDLE 0U

#if (defined CRYPTO_XVIX_XAIX_SHA1_INIT)
#error CRYPTO_XVIX_XAIX_SHA1_INIT is already defined
#endif
/** \brief The Init state of the SHA computation*/
#define CRYPTO_XVIX_XAIX_SHA1_INIT 1U

#if (defined CRYPTO_XVIX_XAIX_SHA1_INITIALIZED)
#error CRYPTO_XVIX_XAIX_SHA1_INITIALIZED is already defined
#endif
/** \brief The Initialized state of the SHA computation*/
#define CRYPTO_XVIX_XAIX_SHA1_INITIALIZED 2U

#if (defined CRYPTO_XVIX_XAIX_SHA1_UPDATE)
#error CRYPTO_XVIX_XAIX_SHA1_UPDATE is already defined
#endif
/** \brief The Update state of the SHA computation*/
#define CRYPTO_XVIX_XAIX_SHA1_UPDATE 3U

#if (defined CRYPTO_XVIX_XAIX_SHA1_FINISH)
#error CRYPTO_XVIX_XAIX_SHA1_FINISH is already defined
#endif
/** \brief The Finish state of the SHA computation*/
#define CRYPTO_XVIX_XAIX_SHA1_FINISH 4U

#if (defined CRYPTO_XVIX_XAIX_SHA1_FINISH_UPDATE1)
#error CRYPTO_XVIX_XAIX_SHA1_FINISH_UPDATE1 is already defined
#endif
/** \brief The Finish Update1 state of the SHA computation*/
#define CRYPTO_XVIX_XAIX_SHA1_FINISH_UPDATE1 5U

#if (defined CRYPTO_XVIX_XAIX_SHA1_FINISH_UPDATE2)
#error CRYPTO_XVIX_XAIX_SHA1_FINISH_UPDATE2 is already defined
#endif
/** \brief The Finish Update2 state of the SHA computation*/
#define CRYPTO_XVIX_XAIX_SHA1_FINISH_UPDATE2 6U

#if (defined CRYPTO_XVIX_XAIX_SHA1_FINISH_EXPORT)
#error CRYPTO_XVIX_XAIX_SHA1_FINISH_EXPORT is already defined
#endif
/** \brief The Finish Export state of the SHA computation*/
#define CRYPTO_XVIX_XAIX_SHA1_FINISH_EXPORT 7U

/*--ProcessInputState-----------------------------------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_SHA1_PROCESS_IDLE)
#error CRYPTO_XVIX_XAIX_SHA1_PROCESS_IDLE is already defined
#endif
/** \brief The Idle state of Crypto_xVIx_xAIx_SHA1ProcessInput*/
#define CRYPTO_XVIX_XAIX_SHA1_PROCESS_IDLE 0U

#if (defined CRYPTO_XVIX_XAIX_SHA1_PROCESS)
#error CRYPTO_XVIX_XAIX_SHA1_PROCESS is already defined
#endif
/** \brief The Process state of Crypto_xVIx_xAIx_SHA1ProcessInput*/
#define CRYPTO_XVIX_XAIX_SHA1_PROCESS 1U

#if (defined CRYPTO_XVIX_XAIX_SHA1_PROCESS_COPY1)
#error CRYPTO_XVIX_XAIX_SHA1_PROCESS_COPY1 is already defined
#endif
/** \brief The first Copy state of Crypto_xVIx_xAIx_SHA1ProcessInput*/
#define CRYPTO_XVIX_XAIX_SHA1_PROCESS_COPY1 2U

#if (defined CRYPTO_XVIX_XAIX_SHA1_PROCESS_COMPRESS1)
#error CRYPTO_XVIX_XAIX_SHA1_PROCESS_COMPRESS1 is already defined
#endif
/** \brief The fist Compress state of Crypto_xVIx_xAIx_SHA1ProcessInput*/
#define CRYPTO_XVIX_XAIX_SHA1_PROCESS_COMPRESS1 3U

#if (defined CRYPTO_XVIX_XAIX_SHA1_PROCESS_COMPRESS2)
#error CRYPTO_XVIX_XAIX_SHA1_PROCESS_COMPRESS2 is already defined
#endif
/** \brief The second Compress state of Crypto_xVIx_xAIx_SHA1ProcessInput*/
#define CRYPTO_XVIX_XAIX_SHA1_PROCESS_COMPRESS2 4U

#if (defined CRYPTO_XVIX_XAIX_SHA1_PROCESS_COMPRESS3)
#error CRYPTO_XVIX_XAIX_SHA1_PROCESS_COMPRESS3 is already defined
#endif
/** \brief The third Compress state of Crypto_xVIx_xAIx_SHA1ProcessInput*/
#define CRYPTO_XVIX_XAIX_SHA1_PROCESS_COMPRESS3 5U

#if (defined CRYPTO_XVIX_XAIX_SHA1_PROCESS_COPY2)
#error CRYPTO_XVIX_XAIX_SHA1_PROCESS_COPY2 is already defined
#endif
/** \brief The second Copy state of Crypto_xVIx_xAIx_SHA1ProcessInput*/
#define CRYPTO_XVIX_XAIX_SHA1_PROCESS_COPY2 6U

/*--CompressState-----------------------------------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_SHA1_COMPRESS_IDLE)
#error CRYPTO_XVIX_XAIX_SHA1_COMPRESS_IDLE is already defined
#endif
/** \brief The Idle state of SHA1Compress */
#define CRYPTO_XVIX_XAIX_SHA1_COMPRESS_IDLE 0U

#if (defined CRYPTO_XVIX_XAIX_SHA1_COMPRESS)
#error CRYPTO_XVIX_XAIX_SHA1_COMPRESS is already defined
#endif
/** \brief The Compress state of SHA1Compress */
#define CRYPTO_XVIX_XAIX_SHA1_COMPRESS 1U

#if (defined CRYPTO_XVIX_XAIX_SHA1_COMPRESS_PRECSMC)
#error CRYPTO_XVIX_XAIX_SHA1_COMPRESS_PRECSMC is already defined
#endif
/** \brief The Precsmc state of SHA1Compress */
#define CRYPTO_XVIX_XAIX_SHA1_COMPRESS_PRECSMC 2U

#if (defined CRYPTO_XVIX_XAIX_SHA1_COMPRESS_TRANSFORM1)
#error CRYPTO_XVIX_XAIX_SHA1_COMPRESS_TRANSFORM1 is already defined
#endif
/** \brief The Transform1 state of SHA1Compress */
#define CRYPTO_XVIX_XAIX_SHA1_COMPRESS_TRANSFORM1 3U

#if (defined CRYPTO_XVIX_XAIX_SHA1_COMPRESS_EXPANSION)
#error CRYPTO_XVIX_XAIX_SHA1_COMPRESS_EXPANSION is already defined
#endif
/** \brief The Expansion state of SHA1Compress */
#define CRYPTO_XVIX_XAIX_SHA1_COMPRESS_EXPANSION 4U

#if (defined CRYPTO_XVIX_XAIX_SHA1_COMPRESS_TRANSFORM2)
#error CRYPTO_XVIX_XAIX_SHA1_COMPRESS_TRANSFORM2 is already defined
#endif
/** \brief The Transform2 state of SHA1Compress */
#define CRYPTO_XVIX_XAIX_SHA1_COMPRESS_TRANSFORM2 5U

#if (defined CRYPTO_XVIX_XAIX_SHA1_COMPRESS_TRANSFORM3)
#error CRYPTO_XVIX_XAIX_SHA1_COMPRESS_TRANSFORM3 is already defined
#endif
/** \brief The Transform3 state of SHA1Compress */
#define CRYPTO_XVIX_XAIX_SHA1_COMPRESS_TRANSFORM3 6U

#if (defined CRYPTO_XVIX_XAIX_SHA1_COMPRESS_EXPORT)
#error CRYPTO_XVIX_XAIX_SHA1_COMPRESS_EXPORT is already defined
#endif
/** \brief The Export state of SHA1Compress */
#define CRYPTO_XVIX_XAIX_SHA1_COMPRESS_EXPORT 7U

/*==[Types]=======================================================================================*/

/** \brief Configuration type for SHA
 *
 * \var Crypto_xVIx_xAIx_SHA1ConfigType::AlgorithmId
 * \brief The algorithm identifier for the configured algorithm
 *
 * \var Crypto_xVIx_xAIx_SHA1ConfigType::Job
 * \brief The processed job
 *
 * \var Crypto_xVIx_xAIx_SHA1ConfigType::ObjId
 * \brief The driver object ID
 *
 * \var Crypto_xVIx_xAIx_SHA1ConfigType::AlgorithmIdLength
 * \brief The length of the algorithm identifier for the configured algorithm
 *
 * \var Crypto_xVIx_xAIx_SHA1ConfigType::HashLength
 *  \brief The length of the Hash algorithm
*/
typedef struct
{
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_CONST)            AlgorithmId;
  P2VAR(Crypto_JobType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA)   Job;
  uint32                                                       ObjId;
  uint32                                                       AlgorithmIdLength;
  uint32                                                       HashLength;

  P2FUNC(void, CRYPTO_xVIx_xAIx_CODE, Crypto_Generic_Callback)
  (
    P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
    uint8                                                        Error
  );
}
Crypto_xVIx_xAIx_SHA1ConfigType;

/** \brief The internal states of Crypto_xVIx_xAIx_SHA1ProcessInputStateType()*/
typedef uint8 Crypto_xVIx_xAIx_SHA1ProcessInputStateType;

/** \brief The internal states of Crypto_xVIx_xAIx_SHA1CompressStateType()*/
typedef uint8 Crypto_xVIx_xAIx_SHA1CompressStateType;

/** \brief The internal states of the SHA1 computation*/
typedef uint8 Crypto_xVIx_xAIx_SHA1StateType;

/**
 * \brief The context of the SHA1 hash
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::ProcessInputState
 * \brief Internal state of the function Crypto_xVIx_xAIx_SHA1ProcessInput()
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::CompressState
 * \brief Internal state of the function Crypto_xVIx_xAIx_SHA1Compress()
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::State
 * \brief The current state of the hash computation
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::Result
 * \brief Pointer to the memory location in which to store the result
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::ResultLengthPtr
 * \brief Pointer to a variable which contains the maximal Result length and
 *        where the length of the result will be stored
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::Data
 * \brief Pointer to the input data
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::DataLength
 * \brief Length of the input data in bytes
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::P
 * \brief Pointer into data
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::Internalstate
 * \brief The internal state of the hash computation, i.e. the hash value of the data stored so far.
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::Count
 * \brief Number of bytes already hashed
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::A
 * \brief Variable to hold the internal state
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::B
 * \brief Variable to hold the internal state
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::C
 * \brief Variable to hold the internal state
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::D
 * \brief Variable to hold the internal state
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::E
 * \brief Variable to hold the internal state
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::T1
 * \brief Temporary variable
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::W
 * \brief Internal copy of the user data
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::Left
 * \brief Number of bytes left
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::Copy
 * \brief Number of bytes to copy
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::Offset
 * \brief Offset into temporal buffer in Ctx
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::I
 * \brief Loop variable
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::K
 * \brief Loop variable
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::L
 * \brief Variable for internal loop calculations
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::Partial
 * \brief Input data to be hashed
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::Length
 * \brief 64-bit representation of length of data
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::ObjId
 * \brief The Driver object ID
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::Job
 * \brief The processed job
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::ProcessingType
 * \brief The processing Type
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::ResultError
 * \brief Error value of MainFunction
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::IsBusy
 * \brief MainFunction is busy
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::CurrentMode
 * \brief The current processing mode
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::CancelFlag
 * \brief This Flag indicates if processing shall be canceled.
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::CallbackFlag
 * \brief This Flag indicates if a callback shall be called.
 *
 * \brief Crypto_xVIx_xAIx_SHA1CtxType::CallbackResult
 * \var The value in this variable will be forwarded to the callback.
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::ErrorCode
 * \brief The error code which should be returned when the current task finishes.
 *
 * \var Crypto_xVIx_xAIx_SHA1CtxType::CfgPtr
 * \brief Pointer to the current SHA1 primitve configuration.
 */
typedef struct
{
  Crypto_xVIx_xAIx_SHA1ProcessInputStateType                          ProcessInputState;
  Crypto_xVIx_xAIx_SHA1CompressStateType                              CompressState;
  Crypto_xVIx_xAIx_SHA1StateType                                      State;
  P2VAR(uint8, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA)                   Result;
  P2VAR(uint32, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA)                  ResultLengthPtr;
  P2CONST(uint8, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA)                 Data;
  uint32                                                              DataLength;
  P2CONST(uint8, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA)                 P;
  uint32                                                              Internalstate[CRYPTO_XVIX_XAIX_SHA1_STATE_WORDS];
  uint32                                                              Count;
  uint32                                                              A;
  uint32                                                              B;
  uint32                                                              C;
  uint32                                                              D;
  uint32                                                              E;
  uint32                                                              T1;
  uint32                                                              W[CRYPTO_XVIX_XAIX_SHA1_BLOCK_LEN_WORDS];
  uint32                                                              Left;
  uint8                                                               Copy;
  uint8                                                               Offset;
  uint8                                                               I;
  uint8                                                               K;
  uint8                                                               L;
  uint8                                                               Partial[CRYPTO_XVIX_XAIX_SHA1_BLOCK_LEN_BYTES];
  uint8                                                               Length[8U];
  uint32                                                              ObjId;
  P2VAR(Crypto_JobType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA)          Job;
  boolean                                                             ProcessingType;
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        ResultError;
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)               IsBusy;
  Crypto_OperationModeType                                            CurrentMode;
  boolean                                                             CancelFlag;
  boolean                                                             CallbackFlag;
  Std_ReturnType                                                      CallbackResult;
  Std_ReturnType                                                      ErrorCode;
  P2CONST(Crypto_xVIx_xAIx_SHA1ConfigType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA) CfgPtr;
}
Crypto_xVIx_xAIx_SHA1CtxType;

/*==[Declaration of functions with internal linkage]==============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief Call HashMain Synchron
 *
 *         This function calls the HashMainFunction inside the StartCall. The calling Task is
 *         blocked until the Hash is produced.
 *
 * \returns If Main is finished or not
 * \retval TRUE   finished
 * \retval FALSE  Not finished
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous}
 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1HelperSynchronousMainCall
(
  void
);

/** \brief AL Callback for other primitives
 *
 *         This function is a callback function for other primitives
 *
 * \param[in,out] Job      A pointer to the current Job
 * \param[in]     Result   The Result of the process
 *
 * \Reentrancy Nonreentrant
 * \Synchronicity Synchronous/Asynchronous
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA1_NOT_SET_CallBack
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint8                                                        Result
);

/** \brief Function to perform the callback and job cancellation.
 *
 *         This function is implemented in order to perform the job cancellation
 *         and callback functionality is a single call, instead of having it in the
 *         Crypto_xVIx_xAIx_AL_HASH_SHA1_NOT_SET_MainFunction.
 *
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1_Callback_Cancel
(
  void
);

/** \brief Function to perform CRYPTO_XVIX_XAIX_SHA1_INIT state processing in a single function
 *
 *     This function is implemented in order to perform the whole processing of
 *     CRYPTO_XVIX_XAIX_SHA1_INIT state in a single function. It enables the state
 *     to have a single function call which performes its whole processing.
 *
 * \param[in,out] CtxPtr A pointer to the context of the algorithm
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1Init
(
  P2VAR(Crypto_xVIx_xAIx_SHA1CtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Function to perform CRYPTO_XVIX_XAIX_SHA1_UPDATE state processing in a single function
 *
 *     This function is implemented in order to perform the whole processing of
 *     CRYPTO_XVIX_XAIX_SHA1_UPDATE state in a single function. It enables the state
 *     to have a single function call which performes its whole processing.
 *
 * \param[in,out] CtxPtr A pointer to the context of the algorithm
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1Update
(
  P2VAR(Crypto_xVIx_xAIx_SHA1CtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Function to perform CRYPTO_XVIX_XAIX_SHA1_FINISH state processing in a single function
 *
 *     This function is implemented in order to perform the whole processing of
 *     CRYPTO_XVIX_XAIX_SHA1_FINISH state in a single function. It enables the state
 *     to have a single function call which performes its whole processing.
 *
 * \param[in,out] CtxPtr  A pointer to the context of the algorithm
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1Finish
(
  P2VAR(Crypto_xVIx_xAIx_SHA1CtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Copies the digest to the output area
 *
 *         The internal state of the SHA algorithm after the input has been
 *         processed completely is the digest. This function copies the digest from
 *         the internal state to the output array.
 *
 * \param[in,out] CtxPtr  A pointer to the context of the algorithm
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1Export
(
   P2VAR(Crypto_xVIx_xAIx_SHA1CtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/*---------------------Process Input--------------------------------------------------------------*/
/** \brief Processes the given input data
 *
 *         This function processes the input data stored in the context of the algorithm
 *
 * \param[in,out] CtxPtr   A pointer to the context of the algorithm
 *
 * \returns If process input has been finished or not
 * \retval TRUE   finished
 * \retval FALSE  Not finished
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1ProcessInput
(
  P2VAR(Crypto_xVIx_xAIx_SHA1CtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Process Input state helper function to perform the state processing for the
 *         CRYPTO_XVIX_XAIX_SHA1_PROCESS state. This helper function is implemented to
 *         reduce the complexity of the Crypto_xVIx_xAIx_SHA1ProcessInput function.
 *
 *
 * \param[in,out] CtxPtr   A pointer to the context of the algorithm
 *
 * \returns If process input has been finished or not
 * \retval TRUE   finished
 * \retval FALSE  Not finished
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1ProcessInputProcess
(
  P2VAR(Crypto_xVIx_xAIx_SHA1CtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Process Input state helper function to perform the state processing for the
 *         CRYPTO_XVIX_XAIX_SHA1_PROCESS_COPY1 state. This helper function is implemented to
 *         reduce the complexity of the Crypto_xVIx_xAIx_SHA1ProcessInput function.
 *
 * \param[in,out] CtxPtr   A pointer to the context of the algorithm
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1ProcessInputCopy1
(
  P2VAR(Crypto_xVIx_xAIx_SHA1CtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Process Input state helper function to perform the state processing for the
 *         CRYPTO_XVIX_XAIX_SHA1_PROCESS_COMPRESS1 state. This helper function is implemented to
 *         reduce the complexity of the Crypto_xVIx_xAIx_SHA1ProcessInput function.
 *
 * \param[in,out] CtxPtr   A pointer to the context of the algorithm
 *
 * \returns If process input has been finished or not
 * \retval TRUE   finished
 * \retval FALSE  Not finished
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1ProcessInputCompress1
(
  P2VAR(Crypto_xVIx_xAIx_SHA1CtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Process Input state helper function to perform the state processing for the
 *         CRYPTO_XVIX_XAIX_SHA1_PROCESS_COMPRESS2 state. This helper function is implemented to
 *         reduce the complexity of the Crypto_xVIx_xAIx_SHA1ProcessInput function.
 *
 * \param[in,out] CtxPtr   A pointer to the context of the algorithm
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1ProcessInputCompress2
(
  P2VAR(Crypto_xVIx_xAIx_SHA1CtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Process Input state helper function to perform the state processing for the
 *         CRYPTO_XVIX_XAIX_SHA1_PROCESS_COMPRESS3 state. This helper function is implemented to
 *         reduce the complexity of the Crypto_xVIx_xAIx_SHA1ProcessInput function.
 *
 * \param[in,out] CtxPtr   A pointer to the context of the algorithm
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1ProcessInputCompress3
(
  P2VAR(Crypto_xVIx_xAIx_SHA1CtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Process Input state helper function to perform the state processing for the
 *         CRYPTO_XVIX_XAIX_SHA1_PROCESS_COPY2 state. This helper function is implemented to
 *         reduce the complexity of the Crypto_xVIx_xAIx_SHA1ProcessInput function.
 *
 * \param[in,out] CtxPtr   A pointer to the context of the algorithm
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1ProcessInputCopy2
(
  P2VAR(Crypto_xVIx_xAIx_SHA1CtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/*-------------------------Compress---------------------------------------------------------------*/
/** \brief Initialize compress functions
 *
 *         This function initializes the compression states of the context variable so
 *         that the compress functions can be used
 *
 * \param[in,out] CtxPtr  A pointer to the context of the algorithm
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1CompressStart
(
  P2VAR(Crypto_xVIx_xAIx_SHA1CtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Processes one block of data to hash
 *
 *         This function compresses one block of data to hash according to the rules
 *         of SHA1 and updates the internal state accordingly. The function
 *         Crypto_xVIx_xAIx_SHA1CompressStart() has to be called before the first call to this
 *         function.
 *
 * \param[in] Data A pointer to a block of data of size CRYPTO_XVIX_XAIX_SHA1_BLOCK_LEN_BYTES
 *                 bytes which has to be processed
 *
 * \returns Compression operation state
 * \retval FALSE  The compression has not yet finished. It has to be called again with
 *                the same parameters
 * \retval TRUE   The compression has finished. Now, the resultError is stored in the
 *                contect variable
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1Compress
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Data
);

/** \brief Helper function for Crypto_xVIx_xAIx_SHA1StateCompress
 *
 *         This function perform the computations in the state CRYPTO_XVIX_XAIX_SHA1_COMPRESS.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1StateCompress
(
  void
);

/** \brief Helper function for Crypto_xVIx_xAIx_SHA1StateCompressPrecsmc
 *
 *         This function perform the computations in the state CRYPTO_XVIX_XAIX_SHA1_COMPRESS_PRECSMC.
 *
 * \param[in] Data  A pointer to a block of data of size CRYPTO_XVIX_XAIX_SHA1_BLOCK_LEN_BYTES
 *                  bytes which has to be processed
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1StateCompressPrecsmc
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Data
);

/** \brief Helper function for Crypto_xVIx_xAIx_SHA1StateCompressTransform1
 *
 *         This function perform the computations in the state CRYPTO_XVIX_XAIX_SHA1_COMPRESS_TRANSFORM1.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1StateCompressTransform1
(
  void
);

/** \brief Helper function for Crypto_xVIx_xAIx_SHA1StateCompressExpansion
 *
 *         This function perform the computations in the state CRYPTO_XVIX_XAIX_SHA1_COMPRESS_EXPANSION.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1StateCompressExpansion
(
  void
);

/** \brief Helper function for Crypto_xVIx_xAIx_SHA1StateCompressTransform2
 *
 *         This function perform the computations in the state CRYPTO_XVIX_XAIX_SHA1_COMPRESS_TRANSFORM2.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1StateCompressTransform2
(
  void
);

/** \brief Helper function for Crypto_xVIx_xAIx_SHA1StateCompressTransform3
 *
 *         This function perform the computations in the state CRYPTO_XVIX_XAIX_SHA1_COMPRESS_TRANSFORM3.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1StateCompressTransform3
(
  void
);

/** \brief Helper function for Crypto_xVIx_xAIx_SHA1StateCompressExport
 *
 *         This function perform the computations in the state CRYPTO_XVIX_XAIX_SHA1_COMPRESS_EXPORT.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1StateCompressExport
(
  void
);

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Constants with internal linkage]=============================================================*/


#define CRYPTO_xVIx_xAIx_START_SEC_CONST_8
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief constant padding block.
 *
 *  This is the constant padding block used for the SHA1 last block. The max length is reduced by
 *  8 bytes because the input length has to be appended at the end.
 */
/* !LINKSTO EB_Crypto_CryptAlgos_1703,1 */
static CONST(uint8, CRYPTO_xVIx_xAIx_CONST) Crypto_xVIx_xAIx_SHA1PaddBlock[CRYPTO_XVIX_XAIX_SHA1_BLOCK_LEN_BYTES] =
{
  0x80U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
  0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
  0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
  0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
  0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
  0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
  0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
  0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U
};

#define CRYPTO_xVIx_xAIx_STOP_SEC_CONST_8
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Variables with internal linkage]=============================================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_INIT_BOOLEAN
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief States if Helper is busy */
static VAR(boolean, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_HelperBusy = FALSE;

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_INIT_BOOLEAN
#include <Crypto_xVIx_xAIx_MemMap.h>

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_CLEARED_8
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief Result of the Helper */
static VAR(Std_ReturnType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_HelperResult;

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_CLEARED_8
#include <Crypto_xVIx_xAIx_MemMap.h>

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief SHA1 Ctx
 *
 * The context variable which is used to store the internal state of the
 * SHA1 computation.
 */
static VAR(Crypto_xVIx_xAIx_SHA1CtxType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_SHA1Ctx;

/** \brief Configuration for SHA1 */
static VAR(Crypto_xVIx_xAIx_SHA1ConfigType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_SHA1_Config;

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Constants with external linkage]=============================================================*/

/*==[Variables with external linkage]=============================================================*/

/*==[Definition of functions with external linkage]===============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*--SHA1 Helper-----------------------------------------------------------------------------------*/

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1HelperStart
(
  const  uint32                                                   ObjId,
  P2FUNC(void, CRYPTO_xVIx_xAIx_CODE, Crypto_Generic_Callback)
  (
    P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
          uint8                                                  Error
  ),
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Job,
        boolean                                                  ProcessingType
)
{
  Crypto_xVIx_xAIx_AL_HASH_SHA1_NOT_SET_Reset();

  /* Set the Length of the Hashlength */
  Crypto_xVIx_xAIx_SHA1_Config.HashLength              = CRYPTO_XVIX_XAIX_SHA1_OUTPUT_LEN_BYTES;
  Crypto_xVIx_xAIx_SHA1_Config.Crypto_Generic_Callback = Crypto_Generic_Callback;

  Crypto_xVIx_xAIx_SHA1Ctx.CfgPtr         = &Crypto_xVIx_xAIx_SHA1_Config;
  Crypto_xVIx_xAIx_SHA1Ctx.Job            = Job;
  Crypto_xVIx_xAIx_SHA1Ctx.ObjId          = ObjId;
  Crypto_xVIx_xAIx_SHA1Ctx.CurrentMode    = CRYPTO_OPERATIONMODE_START;
  Crypto_xVIx_xAIx_SHA1Ctx.State          = CRYPTO_XVIX_XAIX_SHA1_INIT;
  Crypto_xVIx_xAIx_SHA1Ctx.CallbackFlag   = TRUE;
  Crypto_xVIx_xAIx_SHA1Ctx.CallbackResult = E_OK;
  Crypto_xVIx_xAIx_SHA1Ctx.ErrorCode      = E_OK;

  if(CRYPTO_PROCESSING_SYNC == ProcessingType)
  {
    /* This function can not fail for the first state */
    (void)Crypto_xVIx_xAIx_SHA1HelperSynchronousMainCall();
  }
}

FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1HelperUpdate
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) DataPtr,
  uint32                                                DataLength,
  boolean                                               ProcessingType
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  /* Store data into context */
  Crypto_xVIx_xAIx_SHA1Ctx.Data       = DataPtr;
  Crypto_xVIx_xAIx_SHA1Ctx.DataLength = DataLength;

  /* Set state of the state machine to update */
  Crypto_xVIx_xAIx_SHA1Ctx.State             = CRYPTO_XVIX_XAIX_SHA1_UPDATE;
  Crypto_xVIx_xAIx_SHA1Ctx.ProcessInputState = CRYPTO_XVIX_XAIX_SHA1_PROCESS;
  Crypto_xVIx_xAIx_SHA1Ctx.ErrorCode         = E_OK;

  if(CRYPTO_PROCESSING_SYNC == ProcessingType)
  {
    RetVal = Crypto_xVIx_xAIx_SHA1HelperSynchronousMainCall();
  }
  else
  {
    RetVal = E_OK;
  }
  return RetVal;
}

FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1HelperFinish
(
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  ResultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultLengthPtr,
  boolean                                              ProcessingType
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  /* Store data into context */
  Crypto_xVIx_xAIx_SHA1Ctx.Result          = ResultPtr;
  Crypto_xVIx_xAIx_SHA1Ctx.ResultLengthPtr = ResultLengthPtr;

  /* Set state of the state machine to finish */
  Crypto_xVIx_xAIx_SHA1Ctx.State             = CRYPTO_XVIX_XAIX_SHA1_FINISH;
  Crypto_xVIx_xAIx_SHA1Ctx.ProcessInputState = CRYPTO_XVIX_XAIX_SHA1_PROCESS;
  Crypto_xVIx_xAIx_SHA1Ctx.ErrorCode         = E_OK;

  if(CRYPTO_PROCESSING_SYNC == ProcessingType)
  {
    RetVal = Crypto_xVIx_xAIx_SHA1HelperSynchronousMainCall();
  }
  else
  {
    RetVal = E_OK;
  }
  return RetVal;
}

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1HelperMainFunction
(
  void
)
{
  Crypto_xVIx_xAIx_AL_HASH_SHA1_NOT_SET_MainFunction(&Crypto_xVIx_xAIx_HelperResult, &Crypto_xVIx_xAIx_HelperBusy);
}

/* !LINKSTO EB_Crypto_CryptAlgos_1265,1, EB_Crypto_CryptAlgos_0975,1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA1_NOT_SET_Start
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  /* !LINKSTO SWS_Crypto_00017,1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Start,1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Start2Start,1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Update2Start,1 */
  Crypto_xVIx_xAIx_AL_HASH_SHA1_NOT_SET_Reset();
  /* Set the Length of the Hashlength */
  Crypto_xVIx_xAIx_SHA1_Config.HashLength = CRYPTO_XVIX_XAIX_SHA1_OUTPUT_LEN_BYTES;
  Crypto_xVIx_xAIx_SHA1_Config.Crypto_Generic_Callback =
    Crypto_xVIx_xAIx_AL_HASH_SHA1_NOT_SET_CallBack;
  Crypto_xVIx_xAIx_SHA1Ctx.CfgPtr = &Crypto_xVIx_xAIx_SHA1_Config;
  Crypto_xVIx_xAIx_SHA1Ctx.Job    = Job;
  Crypto_xVIx_xAIx_SHA1Ctx.ObjId  = ObjectId;

  Crypto_xVIx_xAIx_SHA1Ctx.CurrentMode    = CRYPTO_OPERATIONMODE_START;
  Crypto_xVIx_xAIx_SHA1Ctx.State          = CRYPTO_XVIX_XAIX_SHA1_INIT;
  Crypto_xVIx_xAIx_SHA1Ctx.CallbackFlag   = TRUE;
  Crypto_xVIx_xAIx_SHA1Ctx.CallbackResult = E_OK;

  Crypto_xVIx_xAIx_SHA1Ctx.ErrorCode = E_OK;

  return E_OK;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1266,1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA1_NOT_SET_Update
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  TS_PARAM_UNUSED(ObjectId);

  if(CRYPTO_XVIX_XAIX_SHA1_INITIALIZED == Crypto_xVIx_xAIx_SHA1Ctx.State)
  {
    if
    (
      (Crypto_xVIx_xAIx_SHA1Ctx.Count + Job->jobPrimitiveInputOutput.inputLength) <
      (Job->jobPrimitiveInputOutput.inputLength)
    )
    {
      RetVal = E_NOT_OK;
    }
    else
    {
      /* Store data into context */
      Crypto_xVIx_xAIx_SHA1Ctx.Data       = Job->jobPrimitiveInputOutput.inputPtr;
      Crypto_xVIx_xAIx_SHA1Ctx.DataLength = Job->jobPrimitiveInputOutput.inputLength;

      /* Set state of the state machine to update */
      Crypto_xVIx_xAIx_SHA1Ctx.State             = CRYPTO_XVIX_XAIX_SHA1_UPDATE;
      Crypto_xVIx_xAIx_SHA1Ctx.ProcessInputState = CRYPTO_XVIX_XAIX_SHA1_PROCESS;
      Crypto_xVIx_xAIx_SHA1Ctx.ErrorCode         = E_OK;

      RetVal = E_OK;
    }

  }
  else
  {
    RetVal = CRYPTO_E_BUSY;
  }

  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1267,1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA1_NOT_SET_Finish
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  TS_PARAM_UNUSED(ObjectId);

  if(CRYPTO_XVIX_XAIX_SHA1_INITIALIZED == Crypto_xVIx_xAIx_SHA1Ctx.State)
  {
    /* Store data into context */
    Crypto_xVIx_xAIx_SHA1Ctx.Result            = Job->jobPrimitiveInputOutput.outputPtr;
    Crypto_xVIx_xAIx_SHA1Ctx.ResultLengthPtr   = Job->jobPrimitiveInputOutput.outputLengthPtr;

    /* Set state of the state machine to finish */
    Crypto_xVIx_xAIx_SHA1Ctx.State             = CRYPTO_XVIX_XAIX_SHA1_FINISH;
    Crypto_xVIx_xAIx_SHA1Ctx.ProcessInputState = CRYPTO_XVIX_XAIX_SHA1_PROCESS;
    Crypto_xVIx_xAIx_SHA1Ctx.ErrorCode         = E_OK;
    RetVal = E_OK;
  }
  else
  {
    RetVal = CRYPTO_E_BUSY;
  }

  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1268,1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA1_NOT_SET_Cancel
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  /* This state indicates, that the Primitive is currently waiting for an Update/Finish call */
  if(CRYPTO_XVIX_XAIX_SHA1_INITIALIZED == Crypto_xVIx_xAIx_SHA1Ctx.State)
  {
    /* !LINKSTO SWS_Crypto_00122_CORRECTION,1 */
    Crypto_xVIx_xAIx_AL_HASH_SHA1_NOT_SET_Reset();
    /* !LINKSTO EB_Crypto_00063,1 */
    RetVal = E_OK;
  }
  else
  {
    /* Job can not be canceled imideatly */
    Crypto_xVIx_xAIx_SHA1Ctx.CancelFlag = TRUE;
    /* !LINKSTO SWS_Crypto_00143_CORRECTION_SWS_Crypto_00183,1 */
    RetVal = CRYPTO_E_JOB_CANCELED;
  }

  TS_PARAM_UNUSED(Job);
  TS_PARAM_UNUSED(ObjectId);
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1269,1 */
FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA1_NOT_SET_MainFunction
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
)
{
  if(
      !(Crypto_xVIx_xAIx_CheckSkipMain(Result, Busy, Crypto_xVIx_xAIx_SHA1Ctx.ObjId)) &&
      (NULL_PTR != Crypto_xVIx_xAIx_SHA1Ctx.Job)
    )
  {
    Crypto_xVIx_xAIx_SHA1Ctx.IsBusy      = Busy;
    Crypto_xVIx_xAIx_SHA1Ctx.ResultError = Result;
    if((NULL_PTR != Result) && (NULL_PTR != Busy))
    {
      *Crypto_xVIx_xAIx_SHA1Ctx.IsBusy      = TRUE;
      *Crypto_xVIx_xAIx_SHA1Ctx.ResultError = E_OK;
    }
    switch(Crypto_xVIx_xAIx_SHA1Ctx.State)
    {
      case CRYPTO_XVIX_XAIX_SHA1_INIT:
      {
        Crypto_xVIx_xAIx_SHA1Init(&Crypto_xVIx_xAIx_SHA1Ctx);
        break;
      }
      case CRYPTO_XVIX_XAIX_SHA1_UPDATE:
      {
        Crypto_xVIx_xAIx_SHA1Update(&Crypto_xVIx_xAIx_SHA1Ctx);
        break;
      }
      case CRYPTO_XVIX_XAIX_SHA1_FINISH:
      {
        Crypto_xVIx_xAIx_SHA1Finish(&Crypto_xVIx_xAIx_SHA1Ctx);
        break;
      }

      case CRYPTO_XVIX_XAIX_SHA1_FINISH_UPDATE1:
      {
        if(TRUE == Crypto_xVIx_xAIx_SHA1ProcessInput(&Crypto_xVIx_xAIx_SHA1Ctx))
        {
         Crypto_xVIx_xAIx_SHA1Ctx.ProcessInputState = CRYPTO_XVIX_XAIX_SHA1_PROCESS;

          /* 3. Append length information and complete last block */
          Crypto_xVIx_xAIx_SHA1Ctx.Data       = Crypto_xVIx_xAIx_SHA1Ctx.Length;
          Crypto_xVIx_xAIx_SHA1Ctx.DataLength = 8U;
          Crypto_xVIx_xAIx_SHA1Ctx.State      = CRYPTO_XVIX_XAIX_SHA1_FINISH_UPDATE2;
        }
        break;
      }

      case CRYPTO_XVIX_XAIX_SHA1_FINISH_UPDATE2:
      {
        if(TRUE == Crypto_xVIx_xAIx_SHA1ProcessInput(&Crypto_xVIx_xAIx_SHA1Ctx))
        {
          Crypto_xVIx_xAIx_SHA1Ctx.State = CRYPTO_XVIX_XAIX_SHA1_FINISH_EXPORT;
        }
        break;
      }

      case CRYPTO_XVIX_XAIX_SHA1_FINISH_EXPORT:
      {
        /* 4. Export CtxPtr->Common.resultError to user */
        Crypto_xVIx_xAIx_SHA1Export(&Crypto_xVIx_xAIx_SHA1Ctx);

        Crypto_xVIx_xAIx_SHA1Ctx.State = CRYPTO_XVIX_XAIX_SHA1_IDLE;
        Crypto_xVIx_xAIx_SHA1Ctx.CallbackFlag = TRUE;
        break;
      }

      case CRYPTO_XVIX_XAIX_SHA1_INITIALIZED:
      default:
      {
        /* Defensive Programming - This default case is required by MISRA-C:2012 */
      }
      break;
    }
    Crypto_xVIx_xAIx_SHA1_Callback_Cancel();
  }
}
#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Definition of functions with internal linkage]===============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1HelperSynchronousMainCall
(
  void
)
{
  Crypto_xVIx_xAIx_HelperBusy = TRUE;
  do
  {
    Crypto_xVIx_xAIx_AL_HASH_SHA1_NOT_SET_MainFunction(&Crypto_xVIx_xAIx_HelperResult, &Crypto_xVIx_xAIx_HelperBusy);
  }
  while ( TRUE == Crypto_xVIx_xAIx_HelperBusy );
  return Crypto_xVIx_xAIx_HelperResult;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA1_NOT_SET_CallBack
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint8                                                        Result
)
{
  if(
      (NULL_PTR != Crypto_xVIx_xAIx_SHA1Ctx.IsBusy) &&
      (NULL_PTR != Crypto_xVIx_xAIx_SHA1Ctx.ResultError)
    )
  {
    *Crypto_xVIx_xAIx_SHA1Ctx.IsBusy = FALSE;
    *Crypto_xVIx_xAIx_SHA1Ctx.ResultError = Result;
  }
  if(CRYPTO_PROCESSING_ASYNC == Job->jobPrimitiveInfo->processingType)
  {
    if(
        (
          CRYPTO_OPERATIONMODE_FINISH ==
          (CRYPTO_OPERATIONMODE_FINISH & Crypto_xVIx_xAIx_SHA1Ctx.CurrentMode)
        ) || (E_OK != Result)
      )
    {
      /* !LINKSTO SWS_Crypto_00025,1 */
      /* !LINKSTO SWS_Crypto_00119,1 */
      /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Idle,1 */
      /* !LINKSTO Crypto.Dsn.JobTransition.ActiveError2Idle,1 */
      Job->jobState = CRYPTO_JOBSTATE_IDLE;
      Crypto_xVIx_xAIx_SHA1Ctx.CurrentMode = CRYPTO_XVIX_XAIX_SHA1_DEFAULT_CURRENT_MODE;
      /* !LINKSTO EB_Crypto_01050,1 */
      SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_SHA1Ctx.ObjId].DriverObjectState =
      CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_SHA1Ctx.ObjId].CurrentJob = NULL_PTR;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_SHA1Ctx.ObjId].SkipPeriodicMainFunction = TRUE;
      SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      CryIf_CallbackNotification(Job, Result);
      Crypto_xVIx_xAIx_AL_HASH_SHA1_NOT_SET_Reset();
    }
    if(
        (
          CRYPTO_OPERATIONMODE_UPDATE ==
          (CRYPTO_OPERATIONMODE_UPDATE & Crypto_xVIx_xAIx_SHA1Ctx.CurrentMode)
        ) && (
               ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL) !=
               (
                 ((uint8)Crypto_xVIx_xAIx_SHA1Ctx.Job->jobPrimitiveInputOutput.mode) &
                 ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
               )
             )
      )
    {
      CryIf_CallbackNotification(Job, Result);
    }
  }
}

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA1_NOT_SET_Reset
(
  void
)
{
  SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
  TS_MemBZero(&Crypto_xVIx_xAIx_SHA1Ctx, sizeof(Crypto_xVIx_xAIx_SHA1Ctx));
  SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1_Callback_Cancel
(
  void
)
{
  /* !LINKSTO SWS_Crypto_00143_CORRECTION_SWS_Crypto_00181,1, SWS_Crypto_00144_CORRECTION,1 */
  if(TRUE == Crypto_xVIx_xAIx_SHA1Ctx.CancelFlag)
  {
    if (
         (NULL_PTR != Crypto_xVIx_xAIx_SHA1Ctx.ResultError)
         && (NULL_PTR != Crypto_xVIx_xAIx_SHA1Ctx.IsBusy)
       )
    {
      *Crypto_xVIx_xAIx_SHA1Ctx.IsBusy = FALSE;
      *Crypto_xVIx_xAIx_SHA1Ctx.ResultError = CRYPTO_E_JOB_CANCELED;
      /* ProcessingType is synchronous */
      /* !LINKSTO Crypto.Dsn.JobTransition.ActiveCancel2Idle,1 */
      Crypto_xVIx_xAIx_SHA1Ctx.Job->jobState = CRYPTO_JOBSTATE_IDLE;
      SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_SHA1Ctx.ObjId].DriverObjectState =
        CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_SHA1Ctx.ObjId].CurrentJob =
        NULL_PTR;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_SHA1Ctx.ObjId].SkipPeriodicMainFunction = TRUE;
      SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
    }
    Crypto_xVIx_xAIx_SHA1Ctx.CfgPtr->Crypto_Generic_Callback
    (
      Crypto_xVIx_xAIx_SHA1Ctx.Job,
      CRYPTO_E_JOB_CANCELED
    );
    Crypto_xVIx_xAIx_AL_HASH_SHA1_NOT_SET_Reset();
  }
  else
  if(TRUE == Crypto_xVIx_xAIx_SHA1Ctx.CallbackFlag)
  {
    if (
         (NULL_PTR != Crypto_xVIx_xAIx_SHA1Ctx.ResultError) &&
         (NULL_PTR != Crypto_xVIx_xAIx_SHA1Ctx.IsBusy)
       )
    {
      *Crypto_xVIx_xAIx_SHA1Ctx.IsBusy      = FALSE;
      *Crypto_xVIx_xAIx_SHA1Ctx.ResultError = Crypto_xVIx_xAIx_SHA1Ctx.CallbackResult;
    }
    if(
        (
          CRYPTO_PROCESSING_ASYNC ==
          Crypto_xVIx_xAIx_SHA1Ctx.Job->jobPrimitiveInfo->processingType
        ) &&
        (
          CRYPTO_HASH ==
          Crypto_xVIx_xAIx_SHA1Ctx.Job->jobPrimitiveInfo->primitiveInfo->service
        ) &&
        (
          CRYPTO_ALGOFAM_NOT_SET ==
          Crypto_xVIx_xAIx_SHA1Ctx.Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily
        ) &&
        (
          (
            (
              ((uint8)Crypto_xVIx_xAIx_SHA1Ctx.Job->jobPrimitiveInputOutput.mode) &
                ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
            ) == ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
          ) ||
          (
            (
              ((uint8)Crypto_xVIx_xAIx_SHA1Ctx.Job->jobPrimitiveInputOutput.mode) &
              ((uint8) CRYPTO_OPERATIONMODE_STREAMSTART)
            ) == ( (uint8) CRYPTO_OPERATIONMODE_STREAMSTART)
          )
        ) && (CRYPTO_OPERATIONMODE_START == Crypto_xVIx_xAIx_SHA1Ctx.CurrentMode)
      )
    {
      (void)Crypto_xVIx_xAIx_AL_HASH_SHA1_NOT_SET_Update
            (
              Crypto_xVIx_xAIx_SHA1Ctx.ObjId,
              Crypto_xVIx_xAIx_SHA1Ctx.Job
            );
    }
    else
    if
    (
      (
        CRYPTO_PROCESSING_ASYNC ==
        Crypto_xVIx_xAIx_SHA1Ctx.Job->jobPrimitiveInfo->processingType
      ) &&
      (
        CRYPTO_HASH ==
        Crypto_xVIx_xAIx_SHA1Ctx.Job->jobPrimitiveInfo->primitiveInfo->service
      ) &&
      (
        CRYPTO_ALGOFAM_NOT_SET ==
        Crypto_xVIx_xAIx_SHA1Ctx.Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily
      ) &&
      (
        (
          (
            ((uint8)Crypto_xVIx_xAIx_SHA1Ctx.Job->jobPrimitiveInputOutput.mode) &
            ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
          ) == ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
        )
      ) && (CRYPTO_OPERATIONMODE_UPDATE == Crypto_xVIx_xAIx_SHA1Ctx.CurrentMode)
    )
    {
      (void)Crypto_xVIx_xAIx_AL_HASH_SHA1_NOT_SET_Finish
            (
              Crypto_xVIx_xAIx_SHA1Ctx.ObjId,
              Crypto_xVIx_xAIx_SHA1Ctx.Job
            );
    }
    else
    {
      /* Nothing to do. */
    }

    Crypto_xVIx_xAIx_SHA1Ctx.CfgPtr->Crypto_Generic_Callback
    (
      Crypto_xVIx_xAIx_SHA1Ctx.Job,
      Crypto_xVIx_xAIx_SHA1Ctx.CallbackResult
    );
    Crypto_xVIx_xAIx_SHA1Ctx.CallbackFlag = FALSE;
  }

  else
  {
    /* Nothing to do. */
  }
}

/* MAIN */

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1Init
(
  P2VAR(Crypto_xVIx_xAIx_SHA1CtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  /* 1. Init the number of processed bytes */
  CtxPtr->Count = (uint32) 0U;

  /* !LINKSTO EB_Crypto_CryptAlgos_1707, 1 */
  /* 2. Initialize the internal state of the hash function */
  CtxPtr->Internalstate[0U] = (uint32) 0x67452301UL;
  CtxPtr->Internalstate[1U] = (uint32) 0xEFCDAB89UL;
  CtxPtr->Internalstate[2U] = (uint32) 0x98BADCFEUL;
  CtxPtr->Internalstate[3U] = (uint32) 0x10325476UL;
  CtxPtr->Internalstate[4U] = (uint32) 0xC3D2E1F0UL;

  CtxPtr->State = CRYPTO_XVIX_XAIX_SHA1_INITIALIZED;
  CtxPtr->CallbackFlag = TRUE;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1Update
(
  P2VAR(Crypto_xVIx_xAIx_SHA1CtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  if((NULL_PTR != CtxPtr->IsBusy))
  {
    *CtxPtr->IsBusy = TRUE;
  }
  Crypto_xVIx_xAIx_SHA1Ctx.CurrentMode = CRYPTO_OPERATIONMODE_UPDATE;
  if(Crypto_xVIx_xAIx_SHA1ProcessInput(CtxPtr) == TRUE)
  {
    CtxPtr->State = CRYPTO_XVIX_XAIX_SHA1_INITIALIZED;
    CtxPtr->CallbackFlag = TRUE;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1Finish
(
  P2VAR(Crypto_xVIx_xAIx_SHA1CtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  /* !LINKSTO EB_Crypto_CryptAlgos_1702,1 */
  if((NULL_PTR != CtxPtr->IsBusy))
  {
    *CtxPtr->IsBusy = TRUE;
  }
  /* Pad the buffer */

  /* Set current mode to call Callback correctly */
  CtxPtr->CurrentMode = CRYPTO_OPERATIONMODE_FINISH;

  /* !LINKSTO EB_Crypto_CryptAlgos_1705,1 */
  /* 1. Convert into bits */
  /* Copy the result. We only have the lower 32 bits, so the upper 32 bits are zero. */
  CtxPtr->Length[0U] = (uint8) 0U;
  CtxPtr->Length[1U] = (uint8) 0U;
  CtxPtr->Length[2U] = (uint8) 0U;
  CtxPtr->Length[3U] = (uint8) (CtxPtr->Count >> 29U);
  CtxPtr->Length[4U] = (uint8) (CtxPtr->Count >> 21U);
  CtxPtr->Length[5U] = (uint8) (CtxPtr->Count >> 13U);
  CtxPtr->Length[6U] = (uint8) (CtxPtr->Count >> 5U);
  CtxPtr->Length[7U] = (uint8) (CtxPtr->Count << 3U);

  /* !LINKSTO EB_Crypto_CryptAlgos_1704,1 */
  /* 2. Append padding to CtxPtr->Common.result_context */
  CtxPtr->Data = Crypto_xVIx_xAIx_SHA1PaddBlock;
  CtxPtr->DataLength =
    (
      ((CRYPTO_XVIX_XAIX_SHA1_BLOCK_LEN_BYTES - 9U) - (CtxPtr->Count)) &
      ((uint32)0x3fUL)
    ) +
    (uint32)1U;

  CtxPtr->State = CRYPTO_XVIX_XAIX_SHA1_FINISH_UPDATE1;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1Export
(
   P2VAR(Crypto_xVIx_xAIx_SHA1CtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  uint8_least Index; /* loop variable */
  uint32 HashLength;

  HashLength = CtxPtr->CfgPtr->HashLength;

  if(HashLength <= *(CtxPtr->ResultLengthPtr))
  {
    *(CtxPtr->ResultLengthPtr) = HashLength;
  }

  /* !LINKSTO EB_Crypto_CryptAlgos_1714,1 */
  Index = (uint8_least)0U;
  /* !LINKSTO EB_Crypto_CryptAlgos_1716,1 */
  do
  {
    uint8_least TmpIndex; /* Tmp space to save loop resultErrors */

    TmpIndex = Index;
    TmpIndex <<= 2U;
    if(TmpIndex < *(CtxPtr->ResultLengthPtr))
    {
     CtxPtr->Result[TmpIndex] = (uint8)(CtxPtr->Internalstate[Index] >> 24U);
     TmpIndex++;
    }

    if(TmpIndex < *(CtxPtr->ResultLengthPtr))
    {
     CtxPtr->Result[TmpIndex] = (uint8)(CtxPtr->Internalstate[Index] >> 16U);
     TmpIndex++;
    }

    if(TmpIndex < *(CtxPtr->ResultLengthPtr))
    {
     CtxPtr->Result[TmpIndex] = (uint8)(CtxPtr->Internalstate[Index] >>  8U);
     TmpIndex++;
    }

    if(TmpIndex < *(CtxPtr->ResultLengthPtr))
    {
     CtxPtr->Result[TmpIndex] = (uint8)(CtxPtr->Internalstate[Index]);
    }

    Index++;
  } while (Index <= (uint8_least)(*(CtxPtr->ResultLengthPtr) >> 2U));
}

/*---------------------Process Input--------------------------------------------------------------*/
/** \brief Intern function for sha computation*/
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1ProcessInput
(
  P2VAR(Crypto_xVIx_xAIx_SHA1CtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  boolean Finished = FALSE;

  switch(CtxPtr->ProcessInputState)
  {
    case CRYPTO_XVIX_XAIX_SHA1_PROCESS:
    {
      Finished = Crypto_xVIx_xAIx_SHA1ProcessInputProcess(CtxPtr);
      break;
    }
    case CRYPTO_XVIX_XAIX_SHA1_PROCESS_COPY1:
    {
      Crypto_xVIx_xAIx_SHA1ProcessInputCopy1(CtxPtr);
      break;
    }

    case CRYPTO_XVIX_XAIX_SHA1_PROCESS_COMPRESS1:
    {
      Finished = Crypto_xVIx_xAIx_SHA1ProcessInputCompress1(CtxPtr);
      break;
    }

    case CRYPTO_XVIX_XAIX_SHA1_PROCESS_COMPRESS2:
    {
      Crypto_xVIx_xAIx_SHA1ProcessInputCompress2(CtxPtr);
      break;
    }

    case CRYPTO_XVIX_XAIX_SHA1_PROCESS_COMPRESS3:
    {
      Crypto_xVIx_xAIx_SHA1ProcessInputCompress3(CtxPtr);
      break;
    }

    case CRYPTO_XVIX_XAIX_SHA1_PROCESS_COPY2:
    {
      Crypto_xVIx_xAIx_SHA1ProcessInputCopy2(CtxPtr);
      Finished = TRUE;
      break;
    }
    /* CHECK: NOPARSE */
    /* All the valid states are addressed in the above cases. This is internal state machine and the
       states are changes internally. It can not be called from outside mainfunction, to test the
       default case using CRYPTO_XVIX_XAIX_SHA1_PROCESS_IDLE.*/
    /* Defensive Programming - unreachable code. This default case is required by MISRA-C:2012 */
    default:
    {
    }
    break;
    /* CHECK: PARSE */
  }

  return Finished;
}

static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1ProcessInputProcess
(
  P2VAR(Crypto_xVIx_xAIx_SHA1CtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  boolean Finished = FALSE;

  if (CtxPtr->DataLength != (uint32) 0U)
  {
    /* Initialize temporal data */
    CtxPtr->P    = CtxPtr->Data;
    CtxPtr->Left = CtxPtr->DataLength;
    CtxPtr->Copy = (uint8) 0U;

    /* 1. Get unprocessed bytes from last block and update the counter */
    CtxPtr->Offset = (uint8) ((CtxPtr->Count) & (uint32) 0x3fU);
    /* Ignore a possible overflow here */
    CtxPtr->Count += CtxPtr->DataLength;

    CtxPtr->ProcessInputState = CRYPTO_XVIX_XAIX_SHA1_PROCESS_COPY1;
  }
  else
  {
    /* If there is no data, there is no work to do */
    Finished = TRUE;
  }

  return Finished;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1ProcessInputCopy1
(
  P2VAR(Crypto_xVIx_xAIx_SHA1CtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  /* 2. If existent, process incomplete blocks */

  if (CtxPtr->Offset > (uint8) 0U)
  {
    /* Find out how much data we need to copy */
    if
    (
      (uint32)((uint32)CtxPtr->Offset + CtxPtr->DataLength) >
      (uint32)CRYPTO_XVIX_XAIX_SHA1_BLOCK_LEN_BYTES
    )
    {
      CtxPtr->Copy = (uint8)CRYPTO_XVIX_XAIX_SHA1_BLOCK_LEN_BYTES - CtxPtr->Offset;
    }
    else
    {
      /* Cast is ok here: we tested dataBytes "< 64" above */
      CtxPtr->Copy = (uint8) CtxPtr->DataLength;
    }

    /* Fill up the temporary buffer with as much data as possible */
    TS_MemCpy
    (
      &CtxPtr->Partial[CtxPtr->Offset],
      CtxPtr->P,
      (uint16)CtxPtr->Copy
    );

    CtxPtr->ProcessInputState = CRYPTO_XVIX_XAIX_SHA1_PROCESS_COMPRESS1;
  }
  else
  {
    Crypto_xVIx_xAIx_SHA1CompressStart(CtxPtr);

    CtxPtr->ProcessInputState = CRYPTO_XVIX_XAIX_SHA1_PROCESS_COMPRESS3;
  }
}

static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1ProcessInputCompress1
(
  P2VAR(Crypto_xVIx_xAIx_SHA1CtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  boolean Finished = FALSE;
  /* Look if we have one block to compress */
  if
  (
    (uint8)(CtxPtr->Offset + CtxPtr->Copy) < (uint8)CRYPTO_XVIX_XAIX_SHA1_BLOCK_LEN_BYTES
  )
  {
    Finished = TRUE;
  }
  else
  {
    /* We have one complete block, so compress it */
    Crypto_xVIx_xAIx_SHA1CompressStart(CtxPtr);

    CtxPtr->P = &CtxPtr->P[CtxPtr->Copy];
    CtxPtr->Left -= (uint32)CtxPtr->Copy;

    CtxPtr->ProcessInputState = CRYPTO_XVIX_XAIX_SHA1_PROCESS_COMPRESS2;
  }

  return Finished;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1ProcessInputCompress2
(
  P2VAR(Crypto_xVIx_xAIx_SHA1CtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  if(TRUE == Crypto_xVIx_xAIx_SHA1Compress(CtxPtr->Partial))
  {
    Crypto_xVIx_xAIx_SHA1CompressStart(CtxPtr);
    CtxPtr->ProcessInputState = CRYPTO_XVIX_XAIX_SHA1_PROCESS_COMPRESS3;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1ProcessInputCompress3
(
  P2VAR(Crypto_xVIx_xAIx_SHA1CtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
  )
{
  if ((CtxPtr->Left & (uint32)0xffffffc0UL) != (uint32) 0U)
  {
    if(Crypto_xVIx_xAIx_SHA1Compress(CtxPtr->P) == TRUE)
    {
      CtxPtr->P = &CtxPtr->P[CRYPTO_XVIX_XAIX_SHA1_BLOCK_LEN_BYTES];
      CtxPtr->Left -= (uint32)CRYPTO_XVIX_XAIX_SHA1_BLOCK_LEN_BYTES;

      if ((CtxPtr->Left & (uint32)0xffffffc0UL) != (uint32) 0U)
      {
        Crypto_xVIx_xAIx_SHA1CompressStart(CtxPtr);
      }
    }
  }
  else
  {
    CtxPtr->ProcessInputState = CRYPTO_XVIX_XAIX_SHA1_PROCESS_COPY2;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1ProcessInputCopy2
(
  P2VAR(Crypto_xVIx_xAIx_SHA1CtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  /* 4. Check if there is still data */
  if (CtxPtr->Left > (uint32) 0U)
  {
    /* Copy data into partial buffer */
    TS_MemCpy (CtxPtr->Partial, CtxPtr->P, (uint16) CtxPtr->Left);
  }
}

/*-------------------------Compress---------------------------------------------------------------*/
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1CompressStart
(
  P2VAR(Crypto_xVIx_xAIx_SHA1CtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  CtxPtr->CompressState = CRYPTO_XVIX_XAIX_SHA1_COMPRESS;
}

static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1Compress
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Data
)
{
  boolean Finished = FALSE;

  switch(Crypto_xVIx_xAIx_SHA1Ctx.CompressState)
  {
    case CRYPTO_XVIX_XAIX_SHA1_COMPRESS:
    {
      Crypto_xVIx_xAIx_SHA1StateCompress();
      break;
    }

    case CRYPTO_XVIX_XAIX_SHA1_COMPRESS_PRECSMC:
    {
      Crypto_xVIx_xAIx_SHA1StateCompressPrecsmc(Data);
      break;
    }

    case CRYPTO_XVIX_XAIX_SHA1_COMPRESS_TRANSFORM1:
    {
      Crypto_xVIx_xAIx_SHA1StateCompressTransform1();
      break;
    }

    case CRYPTO_XVIX_XAIX_SHA1_COMPRESS_EXPANSION:
    {
      Crypto_xVIx_xAIx_SHA1StateCompressExpansion();
      break;
    }

    case CRYPTO_XVIX_XAIX_SHA1_COMPRESS_TRANSFORM2:
    {
      Crypto_xVIx_xAIx_SHA1StateCompressTransform2();
      break;
    }

    case CRYPTO_XVIX_XAIX_SHA1_COMPRESS_TRANSFORM3:
    {
      Crypto_xVIx_xAIx_SHA1StateCompressTransform3();
      break;
    }

    case CRYPTO_XVIX_XAIX_SHA1_COMPRESS_EXPORT:
    {
      Crypto_xVIx_xAIx_SHA1StateCompressExport();
      Finished = TRUE;
      break;
    }
    /* CHECK: NOPARSE */
    /* All the valid states are addressed in the above cases. This is internal state machine and the
       states are changed internally. It can not be called from outside mainfunction, to test the
       default case using CRYPTO_XVIX_XAIX_SHA1_COMPRESS_IDLE.*/
    default:
    {
      /* Defensive Programming - unreachable code. This default case is required by MISRA-C:2012 */
    }
    break;
    /* CHECK: PARSE */
  }
  return Finished;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1StateCompress
(
  void
)
{
  /* Let A = H0, B = H1, C = H2, D = H3, E = H4 */
  Crypto_xVIx_xAIx_SHA1Ctx.A = Crypto_xVIx_xAIx_SHA1Ctx.Internalstate[0U];
  Crypto_xVIx_xAIx_SHA1Ctx.B = Crypto_xVIx_xAIx_SHA1Ctx.Internalstate[1U];
  Crypto_xVIx_xAIx_SHA1Ctx.C = Crypto_xVIx_xAIx_SHA1Ctx.Internalstate[2U];
  Crypto_xVIx_xAIx_SHA1Ctx.D = Crypto_xVIx_xAIx_SHA1Ctx.Internalstate[3U];
  Crypto_xVIx_xAIx_SHA1Ctx.E = Crypto_xVIx_xAIx_SHA1Ctx.Internalstate[4U];

  /* Copy data to local buffer */
  Crypto_xVIx_xAIx_SHA1Ctx.I = (uint8) 0U;

  Crypto_xVIx_xAIx_SHA1Ctx.CompressState = CRYPTO_XVIX_XAIX_SHA1_COMPRESS_PRECSMC;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1StateCompressPrecsmc
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Data
)
{
  /* Precalculate 4 * i once during the loop */
  Crypto_xVIx_xAIx_SHA1Ctx.K = Crypto_xVIx_xAIx_SHA1Ctx.I;
  Crypto_xVIx_xAIx_SHA1Ctx.K <<= 2U;

  Crypto_xVIx_xAIx_SHA1Ctx.W[Crypto_xVIx_xAIx_SHA1Ctx.I] =
  (uint32) ((uint32) Data[Crypto_xVIx_xAIx_SHA1Ctx.K ] << 24U)             |
  (uint32) ((uint32) Data[Crypto_xVIx_xAIx_SHA1Ctx.K + (uint8) 1U] << 16U) |
  (uint32) ((uint32) Data[Crypto_xVIx_xAIx_SHA1Ctx.K + (uint8) 2U] << 8U)  |
  (uint32) Data[Crypto_xVIx_xAIx_SHA1Ctx.K + (uint8) 3U];

  ++Crypto_xVIx_xAIx_SHA1Ctx.I;

  if (Crypto_xVIx_xAIx_SHA1Ctx.I >= (uint8) (CRYPTO_XVIX_XAIX_SHA1_BLOCK_LEN_BYTES >> 2U))
  {
    Crypto_xVIx_xAIx_SHA1Ctx.CompressState = CRYPTO_XVIX_XAIX_SHA1_COMPRESS_TRANSFORM1;

    /* 2. The message Crypto_xVIx_xAIx_SHA1Ctx.resultError transformation loop */
    Crypto_xVIx_xAIx_SHA1Ctx.I = (uint8) 0U;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1StateCompressTransform1
(
  void
)
{
  /* 2.1 Precalculations */
  Crypto_xVIx_xAIx_SHA1Ctx.K = (uint8) (Crypto_xVIx_xAIx_SHA1Ctx.I & (uint8) 0x0fU);
  Crypto_xVIx_xAIx_SHA1Ctx.L = (uint8) (Crypto_xVIx_xAIx_SHA1Ctx.I / (uint8) 20U);

  Crypto_xVIx_xAIx_SHA1Ctx.CompressState = CRYPTO_XVIX_XAIX_SHA1_COMPRESS_EXPANSION;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1StateCompressExpansion
(
  void
)
{
  /* 2.2 User data expansion:
   * during the first 16 rounds, no expansion is needed. sha1 uses
   * the user input data directly from rounds 17 to 79, calculate the necessary word
   */
  if ((uint8)(Crypto_xVIx_xAIx_SHA1Ctx.I & (uint8) 0xf0) != (uint8) 0U)
  {
    /* The word is constructed recursively from 4 previous words */
    Crypto_xVIx_xAIx_SHA1Ctx.T1 =
      Crypto_xVIx_xAIx_SHA1Ctx.W[(uint8)(Crypto_xVIx_xAIx_SHA1Ctx.I + (uint8) 13U) & (uint8)0x0fU] ^
      Crypto_xVIx_xAIx_SHA1Ctx.W[(uint8)(Crypto_xVIx_xAIx_SHA1Ctx.I + (uint8) 8U) &  (uint8)0x0fU] ^
      Crypto_xVIx_xAIx_SHA1Ctx.W[(uint8)(Crypto_xVIx_xAIx_SHA1Ctx.I + (uint8) 2U) &  (uint8)0x0fU] ^
      Crypto_xVIx_xAIx_SHA1Ctx.W[Crypto_xVIx_xAIx_SHA1Ctx.K];

    Crypto_xVIx_xAIx_SHA1Ctx.W[Crypto_xVIx_xAIx_SHA1Ctx.K] =
      (uint32)
      (
        (uint32)(Crypto_xVIx_xAIx_SHA1Ctx.T1 << 1U ) |
        (uint32)(Crypto_xVIx_xAIx_SHA1Ctx.T1 >> 31U)
      );
  }

  Crypto_xVIx_xAIx_SHA1Ctx.CompressState = CRYPTO_XVIX_XAIX_SHA1_COMPRESS_TRANSFORM2;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1StateCompressTransform2
(
  void
)
{
  /* 2.3 Round transformation */

  /* !LINKSTO EB_Crypto_CryptAlgos_1708,1 */
  /* !LINKSTO EB_Crypto_CryptAlgos_1713,1 */
  /* round dependent part first */
  switch (Crypto_xVIx_xAIx_SHA1Ctx.L)
  {
    /* !LINKSTO EB_Crypto_CryptAlgos_1709,1 */
    case 0:
    {
     /* Rounds 0 .. 19 */
      Crypto_xVIx_xAIx_SHA1Ctx.T1 =
        (uint32)
        (
           (uint32)
           (
              (uint32)((uint32)Crypto_xVIx_xAIx_SHA1Ctx.C ^ (uint32)Crypto_xVIx_xAIx_SHA1Ctx.D) &
              (uint32) Crypto_xVIx_xAIx_SHA1Ctx.B
           ) ^
           (uint32) Crypto_xVIx_xAIx_SHA1Ctx.D
        ) +
        (uint32) 0x5A827999UL;
      break;
     }

    case 1:
    {
      /* !LINKSTO EB_Crypto_CryptAlgos_1710,1 */
      /* Rounds 20 .. 39 */
      Crypto_xVIx_xAIx_SHA1Ctx.T1 =
        (uint32)
        (
          (uint32)((uint32)Crypto_xVIx_xAIx_SHA1Ctx.B ^ (uint32)Crypto_xVIx_xAIx_SHA1Ctx.C) ^
          (uint32) Crypto_xVIx_xAIx_SHA1Ctx.D
        ) +
        (uint32) 0x6ED9EBA1UL;
      break;
    }

    /* !LINKSTO EB_Crypto_CryptAlgos_1711,1 */
    case 2:
    {
      /* Rounds 40 .. 59 */
      Crypto_xVIx_xAIx_SHA1Ctx.T1 =
         (uint32)
         (
            (uint32)
            (
               (uint32)
               (
                  (uint32) Crypto_xVIx_xAIx_SHA1Ctx.B & (uint32) Crypto_xVIx_xAIx_SHA1Ctx.C
               ) |
               (uint32)
               (
                  (uint32) Crypto_xVIx_xAIx_SHA1Ctx.B & (uint32) Crypto_xVIx_xAIx_SHA1Ctx.D
               )
            ) |
            (uint32)
            (
               (uint32)
               (
                  (uint32)Crypto_xVIx_xAIx_SHA1Ctx.C & (uint32)Crypto_xVIx_xAIx_SHA1Ctx.D
               )
            )
         ) +
         (uint32) 0x8F1BBCDCUL;
      break;
    }

    /* !LINKSTO EB_Crypto_CryptAlgos_1712,1 */
    default:
    {
      /* Rounds 60 .. 79 */
      Crypto_xVIx_xAIx_SHA1Ctx.T1 =
        (uint32)
        (
          (uint32)((uint32)Crypto_xVIx_xAIx_SHA1Ctx.B ^ (uint32)Crypto_xVIx_xAIx_SHA1Ctx.C) ^
          (uint32)Crypto_xVIx_xAIx_SHA1Ctx.D
        ) +
        (uint32) 0xCA62C1D6UL;
      break;
    }
  }
  /* Round independent part */
  Crypto_xVIx_xAIx_SHA1Ctx.T1 += (uint32) ((uint32) ((uint32) Crypto_xVIx_xAIx_SHA1Ctx.A << 5U) |
                                 (uint32) ((uint32) Crypto_xVIx_xAIx_SHA1Ctx.A >> 27U))         +
                                 (Crypto_xVIx_xAIx_SHA1Ctx.E)                                   +
                                 (Crypto_xVIx_xAIx_SHA1Ctx.W[Crypto_xVIx_xAIx_SHA1Ctx.K]);

  Crypto_xVIx_xAIx_SHA1Ctx.CompressState = CRYPTO_XVIX_XAIX_SHA1_COMPRESS_TRANSFORM3;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1StateCompressTransform3
(
  void
)
{
  /* 2.4 Permute the words for next round */
  Crypto_xVIx_xAIx_SHA1Ctx.E = Crypto_xVIx_xAIx_SHA1Ctx.D;
  Crypto_xVIx_xAIx_SHA1Ctx.D = Crypto_xVIx_xAIx_SHA1Ctx.C;

  /* Optimization: ROTL by 30 is ROTR by 2 */
  Crypto_xVIx_xAIx_SHA1Ctx.C = (uint32) ((uint32) ((uint32) Crypto_xVIx_xAIx_SHA1Ctx.B >> 2U) |
                               (uint32) ((uint32) Crypto_xVIx_xAIx_SHA1Ctx.B << 30U));
  Crypto_xVIx_xAIx_SHA1Ctx.B = Crypto_xVIx_xAIx_SHA1Ctx.A;
  Crypto_xVIx_xAIx_SHA1Ctx.A = Crypto_xVIx_xAIx_SHA1Ctx.T1;

  Crypto_xVIx_xAIx_SHA1Ctx.I++;

  if (Crypto_xVIx_xAIx_SHA1Ctx.I != (uint8) CRYPTO_XVIX_XAIX_SHA1_ROUNDS)
  {
     Crypto_xVIx_xAIx_SHA1Ctx.CompressState = CRYPTO_XVIX_XAIX_SHA1_COMPRESS_TRANSFORM1;
  }
  else
  {
     Crypto_xVIx_xAIx_SHA1Ctx.CompressState = CRYPTO_XVIX_XAIX_SHA1_COMPRESS_EXPORT;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA1StateCompressExport
(
  void
)
{
  /* 3. Copy internal state back to Ctx */
  Crypto_xVIx_xAIx_SHA1Ctx.Internalstate[0U] += Crypto_xVIx_xAIx_SHA1Ctx.A;
  Crypto_xVIx_xAIx_SHA1Ctx.Internalstate[1U] += Crypto_xVIx_xAIx_SHA1Ctx.B;
  Crypto_xVIx_xAIx_SHA1Ctx.Internalstate[2U] += Crypto_xVIx_xAIx_SHA1Ctx.C;
  Crypto_xVIx_xAIx_SHA1Ctx.Internalstate[3U] += Crypto_xVIx_xAIx_SHA1Ctx.D;
  Crypto_xVIx_xAIx_SHA1Ctx.Internalstate[4U] += Crypto_xVIx_xAIx_SHA1Ctx.E;
}

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif  /* #if (CRYPTO_XVIX_XAIX_SHA1_ENABLED == STD_ON) */

/*==[end of file]=================================================================================*/


