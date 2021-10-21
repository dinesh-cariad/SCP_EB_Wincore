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

/*==[include directives]==========================================================================*/

#include <Std_Types.h>
#include <TSAutosar.h>
#include <Crypto_xVIx_xAIx.h>
#include <Crypto_xVIx_xAIx_AL_AES_CFB.h>
#include <Crypto_xVIx_xAIx_AL_Cipher.h>
#include <Crypto_xVIx_xAIx_AL_Common.h>
#include <Crypto_xVIx_xAIx_AL_KeyManagement.h>
#include <Crypto_xVIx_xAIx_Int.h>
#include <Csm_Types.h>
#include <CryIf_Cbk.h>
#include <TSMem.h>
#include <SchM_Crypto_xVIx_xAIx.h>

#if ( (CRYPTO_XVIX_XAIX_AES_CFB_ENC_ENABLED == STD_ON) || \
      (CRYPTO_XVIX_XAIX_AES_CFB_DEC_ENABLED == STD_ON)    \
    )

/*==[Macro definitions]===========================================================================*/

#if (defined CRYPTO_XVIX_XAIX_SEGMENT_SIZE)
#error CRYPTO_XVIX_XAIX_SEGMENT_SIZE already defined
#endif
/** \brief The size of one AES_CFB segment in byte. */
#define CRYPTO_XVIX_XAIX_SEGMENT_SIZE 16U

#if (defined CRYPTO_XVIX_XAIX_BLOCK_SIZE)
#error CRYPTO_XVIX_XAIX_BLOCK_SIZE already defined
#endif
/** \brief The size of one AES block in byte. */
#define CRYPTO_XVIX_XAIX_BLOCK_SIZE 16U

#if (defined CRYPTO_XVIX_XAIX_CFB_TRANSITION_COUNT)
#error CRYPTO_XVIX_XAIX_CFB_TRANSITION_COUNT is already defined
#endif
/** \brief Number of transitions in CFB. */
#define CRYPTO_XVIX_XAIX_CFB_TRANSITION_COUNT 11U

#if (defined CRYPTO_XVIX_XAIX_CFB_ACTION_COUNT)
#error CRYPTO_XVIX_XAIX_CFB_ACTION_COUNT is already defined
#endif
/** \brief Number of action functions in CFB. */
#define CRYPTO_XVIX_XAIX_CFB_ACTION_COUNT 7U

/*--AES_CFB states--------------------------------------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_AES_CFB_CALC_ROUND_STATE)
#error CRYPTO_XVIX_XAIX_AES_CFB_CALC_ROUND_STATE already defined
#endif
/** \brief The state to encrypt a CFB block. */
#define CRYPTO_XVIX_XAIX_AES_CFB_CALC_ROUND_STATE 5U

#if (defined CRYPTO_XVIX_XAIX_AES_CFB_FINISH_STATE)
#error CRYPTO_XVIX_XAIX_AES_CFB_FINISH_STATE already defined
#endif
/** \brief The state to encrypt a CFB block. */
#define CRYPTO_XVIX_XAIX_AES_CFB_FINISH_STATE 6U

/*==[Type declarations]===========================================================================*/

/** \brief Needed values to calculate one block.
 *
 * \var InputBlock
 *  \brief The block which shall be encrypted using ECB.
 *
 * \var XORBlock
 *  \brief The block which shall be used to XOR the output.
 *
 * \var OutputBlock
 *  \brief The result of the round calculation.
 *
 * \var SegmentLength
 *  \brief The length of the segment.
 *
 * \var Key
 *  \brief The key which shall be used for encryption.
 *
 */
typedef struct
{
  uint8 InputBlock[CRYPTO_XVIX_XAIX_BLOCK_SIZE];
  uint8 XORBlock[CRYPTO_XVIX_XAIX_BLOCK_SIZE];
  uint8 OutputBlock[CRYPTO_XVIX_XAIX_BLOCK_SIZE];
  uint32 SegmentLength;
  Crypto_xVIx_xAIx_SymKeyType Key;
}Crypto_xVIx_xAIx_AesCfbBlockValuesType;

#if (CRYPTO_XVIX_XAIX_AES_CFB_ENC_ENABLED == STD_ON)

/** \brief The context type for AES_CFB encryption.
 *
 * \var BlockValues
 *  \brief The values need to calculate one block.
 *
 * \var PlainTextPtr
 *  \brief A pointer to the plaintext.
 *
 * \var PlainTextLength
 *  \brief The length of the plaintext.
 *
 * \var CipherTextPtr
 *  \brief A pointer to where the ciphertext shall be stored.
 *
 * \var CipherTextLengthPtr
 *  \brief A pointer to how long the ciphertext buffer is.
 *
 * \var State
 *  \brief The current State of the processing.
 *
 * \var RemainingBytesLength
 *  \brief The length of the remaining bytes from the last call.
 *
 * \var WrittenBytes
 *  \brief The amount of Bytes that where written within the current call.
 *
 * \var ReadBytes
 *  \brief The amount of Bytes that where read within the current call.
 *
 * \var Result
 *  \brief A Pointer to where the result of the synchronous processing shall be stored.

 * \var Busy
 *  \brief A Pointer to the variable indicating if the synchronous processing is done.
 *
 * \var CallbackResult
 *  \brief The result of the next Callback call.
 *
 * \var CallbackFlag
 *  \brief A flag to indicate if a Callback shall be called.
 *
 * \var CancelFlag
 *  \brief A flag to indicate if Cancel shall be called.
 *
 * \var ProcessDone
 *  \brief A flag to indicate if processing is done after the current call.
 *
 * \var Job
 *  \brief A pointer to the Job which shall be processed.
 *
 * \var ObjId
 *  \brief The Object id of the crypto driver.
 *
 * \var StartOutputLength
 *  \brief The output length at the beginning.
 *
 * \var UpdateOutputLength
 *  \brief The output length after the update call.
 *
*/
typedef struct
{
  Crypto_xVIx_xAIx_AesCfbBlockValuesType BlockValues;
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PlainTextPtr;
  uint32 PlainTextLength;
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CipherTextPtr;
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CipherTextLengthPtr;
  Crypto_xVIx_xAIx_StateType State;
  uint32 RemainingBytesLength;
  uint32 WrittenBytes;
  uint32 ReadBytes;
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result;
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Busy;
  Std_ReturnType CallbackResult;
  boolean CallbackFlag;
  boolean CancelFlag;
  boolean ProcessDone;
  P2VAR(Crypto_JobType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA) Job;
  uint32 ObjId;
  uint32 StartOutputLength;
  uint32 UpdateOutputLength;
}Crypto_xVIx_xAIx_AesCfbEncryptContextType;

#endif /* #if (CRYPTO_XVIX_XAIX_AES_CFB_ENC_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_AES_CFB_DEC_ENABLED == STD_ON)

/** \brief The context type for AES_CFB encryption.
 *
 * \var BlockValues
 *  \brief The values need to calculate one block.
 *
 * \var CipherTextPtr
 *  \brief A pointer to the ciphertext.
 *
 * \var CipherTextLength
 *  \brief The length of the ciphertext.
 *
 * \var PlainTextPtr
 *  \brief A pointer to where the plaintext shall be stored.
 *
 * \var PlainTextLengthPtr
 *  \brief A pointer to how long the plaintext buffer is.
 *
 * \var State
 *  \brief The current State of the processing.
 *
 * \var RemainingBytesLength
 *  \brief The length of the remaining bytes from the last call.
 *
 * \var WrittenBytes
 *  \brief The amount of Bytes that where written within the current call.
 *
 * \var ReadBytes
 *  \brief The amount of Bytes that where read within the current call.
 *
 * \var Result
 *  \brief A Pointer to where the result of the synchronous processing shall be stored.
 *
 * \var Busy
 *  \brief A Pointer to the variable indicating if the synchronous processing is done.
 *
 * \var CallbackResult
 *  \brief The result of the next Callback call.
 *
 * \var CallbackFlag
 *  \brief A flag to indicate if a Callback shall be called.
 *
 * \var CancelFlag
 *  \brief A flag to indicate if Cancel shall be called.
 *
 * \var ProcessDone
 *  \brief A flag to indicate if processing is done after the current call.
 *
 * \var Job
 *  \brief A pointer to the Job which shall be processed.
 *
 * \var ObjId
 *  \brief The Object id of the crypto driver.
 *
 * \var StartOutputLength
 *  \brief The output length at the beginning.
 *
 * \var UpdateOutputLength
 *  \brief The output length after the update call.
 *
*/
typedef struct
{
  Crypto_xVIx_xAIx_AesCfbBlockValuesType BlockValues;
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CipherTextPtr;
  uint32 CipherTextLength;
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PlainTextPtr;
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PlainTextLengthPtr;
  Crypto_xVIx_xAIx_StateType State;
  uint32 RemainingBytesLength;
  uint32 WrittenBytes;
  uint32 ReadBytes;
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result;
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Busy;
  Std_ReturnType CallbackResult;
  boolean CallbackFlag;
  boolean CancelFlag;
  boolean ProcessDone;
  P2VAR(Crypto_JobType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA) Job;
  uint32 ObjId;
  uint32 StartOutputLength;
  uint32 UpdateOutputLength;
}Crypto_xVIx_xAIx_AesCfbDecryptContextType;

#endif /* #if (CRYPTO_XVIX_XAIX_AES_CFB_DEC_ENABLED == STD_ON) */

/*==[Declarations of functions with internal linkage]=============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief Shifts a byte array.
 *
 * This function shifts the byte array according to length to the left.
 *
 * \param[in,out] Array The array to be shifted.
 * \param[in] Length    The length how far shall be shifted.
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AES_CFB_ShiftBytes
(
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Array,
        uint32                                        ArrayLength,
        uint32                                        ShiftLength
);

/** \brief Calculate one single round for AES_CFB.
 *
 * This function calculates the output of one single AES_CFB round.
 *
 * \param[in,out] BlockValues A struct containing all needed inputs and outputs.
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AES_CFB_CalcSingleRound
(
  P2VAR(Crypto_xVIx_xAIx_AesCfbBlockValuesType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) BlockValues
);

#if (CRYPTO_XVIX_XAIX_AES_CFB_ENC_ENABLED == STD_ON)

/** \brief Crypto CFB Encrypt Reset context
 *
 * This Function resets the context of AES_CFB encrypt.
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CFB_ResetCtx
(
  void
);

/** \brief Crypto CFB Encrypt Reset context and driver object
 *
 * This Function resets the context and the driver object of AES_CFB encrypt.
 *
 * \param[in] Job   A Pointer to the currently processed job.
 * \param[in] ObjId The object id of the driver object.

 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CFB_ResetCtxAndCDO
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint32 ObjId
);

/** \brief State function for CRYPTO_XVIX_XAIX_ERROR_STATE.
 *
 * \returns Result of the processing.
 * \retval E_OK Processing successfully completed.
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ENCRYPT_AES_CFB_Error
(
  void
);

/** \brief State function for CRYPTO_XVIX_XAIX_CANCEL_STATE.
 *
 * \returns Result of the processing.
 * \retval E_OK Processing successfully completed.
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ENCRYPT_AES_CFB_Cancel
(
  void
);

/** \brief State function for CRYPTO_XVIX_XAIX_AES_CFB_CALC_ROUND_STATE.
 *
 * \returns Result of the processing.
 * \retval E_OK          Processing successfully completed.
 * \retval CRYPTO_E_BUSY Processing successful but not completle done.
 * \retval E_NOT_OK      An Error occured.
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ENCRYPT_AES_CFB_CalcRound
(
  void
);

/** \brief State function for CRYPTO_XVIX_XAIX_AES_CFB_FINISH_STATE.
 *
 * \returns Result of the processing.
 * \retval E_OK Nothing has to be done here.
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ENCRYPT_AES_CFB_Finish
(
  void
);

/** \brief Crypto AL level callback for the AES_CFB encrypt.
 *
 * This callback is used for all modes (start, update, finish) and for both processing
 * types (sync, async). It handles the setting of the status variables and calls the CryIf callback.
 *
 * \param[in] Job    Pointer to the job which the primive computation belongs to.
 * \param[in] Result Result of the intern step/state for which the callback is called.
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CFB_CallBack
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint8                                                        Result
);

#endif /* #if (CRYPTO_XVIX_XAIX_AES_CFB_ENC_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_AES_CFB_DEC_ENABLED == STD_ON)

/** \brief Crypto CFB Decrypt Reset
 *
 * This Function resets the Context of AES_CFB decrypt.
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_CFB_ResetCtx
(
  void
);

/** \brief Crypto CFB Decrypt Reset context and driver object
 *
 * This Function resets the context and the driver object of AES_CFB decrypt.
 *
 * \param[in] Job   A Pointer to the currently processed job.
 * \param[in] ObjId The object id of the driver object.

 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_CFB_ResetCtxAndCDO
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint32 ObjId
);

/** \brief State function for CRYPTO_XVIX_XAIX_ERROR_STATE.
 *
 * \returns Result of the processing.
 * \retval E_OK Processing successfully completed.
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_DECRYPT_AES_CFB_Error
(
  void
);

/** \brief State function for CRYPTO_XVIX_XAIX_CANCEL_STATE.
 *
 * \returns Result of the processing.
 * \retval E_OK Processing successfully completed.
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_DECRYPT_AES_CFB_Cancel
(
  void
);

/** \brief State function for CRYPTO_XVIX_XAIX_AES_CFB_CALC_ROUND_STATE.
 *
 * \returns Result of the processing.
 * \retval E_OK          Processing successfully completed.
 * \retval CRYPTO_E_BUSY Processing successful but not completle done.
 * \retval E_NOT_OK      An Error occured.
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_DECRYPT_AES_CFB_CalcRound
(
  void
);

/** \brief State function for CRYPTO_XVIX_XAIX_AES_CFB_FINISH_STATE.
 *
 * \returns Result of the processing.
 * \retval E_OK Nothing has to be done here.
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_DECRYPT_AES_CFB_Finish
(
  void
);

/** \brief Crypto AL level callback for the AES_CFB decrypt.
 *
 * This callback is used for all modes (start, update, finish) and for both processing
 * types (sync, async). It handles the setting of the status variables and calls the CryIf callback.
 *
 * \param[in] Job    Pointer to the job which the primive computation belongs to.
 * \param[in] Result Result of the intern step/state for which the callback is called.
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_CFB_CallBack
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint8                                                        Result
);

#endif /* #if (CRYPTO_XVIX_XAIX_AES_CFB_DEC_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Constants with internal linkage]=============================================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CONST_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

#if (CRYPTO_XVIX_XAIX_AES_CFB_ENC_ENABLED == STD_ON)

/** \brief State Transitions for AES_CFB encryption.
 *
 * This constant array is needed for specifying the state transitions of AES_CFB.
 * First entry denotes the source state, 2nd entry the return value of the processing
 * of source state, and 3rd entry names the target state.
 *
 */
static CONST(Crypto_xVIx_xAIx_StateTransitionType, CRYPTO_xVIx_xAIx_CONST)
Crypto_xVIx_xAIx_StateTrans_CfbEncrypt[CRYPTO_XVIX_XAIX_CFB_TRANSITION_COUNT] =
{
  {
    CRYPTO_XVIX_XAIX_IDLE_STATE,
    E_OK,
    CRYPTO_XVIX_XAIX_IDLE_STATE
  },
  {
    CRYPTO_XVIX_XAIX_ERROR_STATE,
    E_OK,
    CRYPTO_XVIX_XAIX_IDLE_STATE
  },
  {
    CRYPTO_XVIX_XAIX_CANCEL_STATE,
    E_OK,
    CRYPTO_XVIX_XAIX_IDLE_STATE
  },
  {
    CRYPTO_XVIX_XAIX_START_DONE_STATE,
    E_OK,
    CRYPTO_XVIX_XAIX_START_DONE_STATE
  },
  {
    CRYPTO_XVIX_XAIX_UPDATE_DONE_STATE,
    E_OK,
    CRYPTO_XVIX_XAIX_UPDATE_DONE_STATE
  },
  {
    CRYPTO_XVIX_XAIX_AES_CFB_CALC_ROUND_STATE,
    CRYPTO_E_BUSY,
    CRYPTO_XVIX_XAIX_AES_CFB_CALC_ROUND_STATE
  },
  {
    CRYPTO_XVIX_XAIX_AES_CFB_CALC_ROUND_STATE,
    CRYPTO_E_KEY_SIZE_MISMATCH,
    CRYPTO_XVIX_XAIX_ERROR_STATE
  },
  {
    CRYPTO_XVIX_XAIX_AES_CFB_CALC_ROUND_STATE,
    CRYPTO_E_SMALL_BUFFER,
    CRYPTO_XVIX_XAIX_ERROR_STATE
  },
  {
    CRYPTO_XVIX_XAIX_AES_CFB_CALC_ROUND_STATE,
    E_OK,
    CRYPTO_XVIX_XAIX_UPDATE_DONE_STATE
  },
  {
    CRYPTO_XVIX_XAIX_AES_CFB_FINISH_STATE,
    CRYPTO_E_SMALL_BUFFER,
    CRYPTO_XVIX_XAIX_ERROR_STATE
  },
  {
    CRYPTO_XVIX_XAIX_AES_CFB_FINISH_STATE,
    E_OK,
    CRYPTO_XVIX_XAIX_IDLE_STATE
  }
};

/** \brief Action function-Pointers for AES_CFB encryption
 *
 * This constant array is needed for specifying the functions to be executed in the particular
 * states of AES_CFB. The index corresponds to the value of the states.
 *
 */
static CONST(Crypto_xVIx_xAIx_StateFctPtrType, CRYPTO_xVIx_xAIx_CONST)
  Crypto_xVIx_xAIx_Actions_CfbEncrypt[CRYPTO_XVIX_XAIX_CFB_ACTION_COUNT] =
{
  /* CRYPTO_XVIX_XAIX_IDLE_STATE */
  &Crypto_xVIx_xAIx_StateMachineIdle,
  /* CRYPTO_XVIX_XAIX_ERROR_STATE */
  &Crypto_xVIx_xAIx_ENCRYPT_AES_CFB_Error,
  /* CRYPTO_XVIX_XAIX_CANCEL_STATE */
  &Crypto_xVIx_xAIx_ENCRYPT_AES_CFB_Cancel,
  /* CRYPTO_XVIX_XAIX_START_DONE_STATE */
  &Crypto_xVIx_xAIx_StateMachineIdle,
  /* CRYPTO_XVIX_XAIX_UPDATE_DONE_STATE */
  &Crypto_xVIx_xAIx_StateMachineIdle,
  /* CRYPTO_XVIX_XAIX_AES_CFB_CALC_ROUND_STATE */
  &Crypto_xVIx_xAIx_ENCRYPT_AES_CFB_CalcRound,
  /* CRYPTO_XVIX_XAIX_AES_CFB_FINISH_STATE */
  &Crypto_xVIx_xAIx_ENCRYPT_AES_CFB_Finish
};

#endif /* #if (CRYPTO_XVIX_XAIX_AES_CFB_ENC_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_AES_CFB_DEC_ENABLED == STD_ON)

/** \brief State Transitions for AES_CFB decryption.
 *
 * This constant array is needed for specifying the state transitions of AES_CFB.
 * First entry denotes the source state, 2nd entry the return value of the processing
 * of source state, and 3rd entry names the target state.
 *
 */
static CONST(Crypto_xVIx_xAIx_StateTransitionType, CRYPTO_xVIx_xAIx_CONST)
Crypto_xVIx_xAIx_StateTrans_CfbDecrypt[CRYPTO_XVIX_XAIX_CFB_TRANSITION_COUNT] =
{
  {
    CRYPTO_XVIX_XAIX_IDLE_STATE,
    E_OK,
    CRYPTO_XVIX_XAIX_IDLE_STATE
  },
  {
    CRYPTO_XVIX_XAIX_ERROR_STATE,
    E_OK,
    CRYPTO_XVIX_XAIX_IDLE_STATE
  },
  {
    CRYPTO_XVIX_XAIX_CANCEL_STATE,
    E_OK,
    CRYPTO_XVIX_XAIX_IDLE_STATE
  },
  {
    CRYPTO_XVIX_XAIX_START_DONE_STATE,
    E_OK,
    CRYPTO_XVIX_XAIX_START_DONE_STATE
  },
  {
    CRYPTO_XVIX_XAIX_UPDATE_DONE_STATE,
    E_OK,
    CRYPTO_XVIX_XAIX_UPDATE_DONE_STATE
  },
  {
    CRYPTO_XVIX_XAIX_AES_CFB_CALC_ROUND_STATE,
    CRYPTO_E_BUSY,
    CRYPTO_XVIX_XAIX_AES_CFB_CALC_ROUND_STATE
  },
  {
    CRYPTO_XVIX_XAIX_AES_CFB_CALC_ROUND_STATE,
    CRYPTO_E_KEY_SIZE_MISMATCH,
    CRYPTO_XVIX_XAIX_ERROR_STATE
  },
  {
    CRYPTO_XVIX_XAIX_AES_CFB_CALC_ROUND_STATE,
    CRYPTO_E_SMALL_BUFFER,
    CRYPTO_XVIX_XAIX_ERROR_STATE
  },
  {
    CRYPTO_XVIX_XAIX_AES_CFB_CALC_ROUND_STATE,
    E_OK,
    CRYPTO_XVIX_XAIX_UPDATE_DONE_STATE
  },
  {
    CRYPTO_XVIX_XAIX_AES_CFB_FINISH_STATE,
    CRYPTO_E_SMALL_BUFFER,
    CRYPTO_XVIX_XAIX_ERROR_STATE
  },
  {
    CRYPTO_XVIX_XAIX_AES_CFB_FINISH_STATE,
    E_OK,
    CRYPTO_XVIX_XAIX_IDLE_STATE
  }
};

/** \brief Action function-Pointers for AES_CFB decryption
 *
 * This constant array is needed for specifying the functions to be executed in the particular
 * states of AES_CFB. The index corresponds to the value of the states.
 *
 */
static CONST(Crypto_xVIx_xAIx_StateFctPtrType, CRYPTO_xVIx_xAIx_CONST)
  Crypto_xVIx_xAIx_Actions_CfbDecrypt[CRYPTO_XVIX_XAIX_CFB_ACTION_COUNT] =
{
  /* CRYPTO_XVIX_XAIX_IDLE_STATE */
  &Crypto_xVIx_xAIx_StateMachineIdle,
  /* CRYPTO_XVIX_XAIX_ERROR_STATE */
  &Crypto_xVIx_xAIx_DECRYPT_AES_CFB_Error,
  /* CRYPTO_XVIX_XAIX_CANCEL_STATE */
  &Crypto_xVIx_xAIx_DECRYPT_AES_CFB_Cancel,
  /* CRYPTO_XVIX_XAIX_START_DONE_STATE */
  &Crypto_xVIx_xAIx_StateMachineIdle,
  /* CRYPTO_XVIX_XAIX_UPDATE_DONE_STATE */
  &Crypto_xVIx_xAIx_StateMachineIdle,
  /* CRYPTO_XVIX_XAIX_AES_CFB_CALC_ROUND_STATE */
  &Crypto_xVIx_xAIx_DECRYPT_AES_CFB_CalcRound,
  /* CRYPTO_XVIX_XAIX_AES_CFB_FINISH_STATE */
  &Crypto_xVIx_xAIx_DECRYPT_AES_CFB_Finish
};

#endif /* #if (CRYPTO_XVIX_XAIX_AES_CFB_DEC_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_CONST_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Variables with internal linkage]=============================================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

#if (CRYPTO_XVIX_XAIX_AES_CFB_ENC_ENABLED == STD_ON)
/** \brief Context variable for AES_CFB Encrypt. */
static VAR(Crypto_xVIx_xAIx_AesCfbEncryptContextType, CRYPTO_xVIx_xAIx_VAR)
  Crypto_xVIx_xAIx_AesCfbEncryptContext;
#endif /* #if (CRYPTO_XVIX_XAIX_AES_CFB_ENC_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_AES_CFB_DEC_ENABLED == STD_ON)
/** \brief Context variable for AES_CFB Decrypt. */
static VAR(Crypto_xVIx_xAIx_AesCfbDecryptContextType, CRYPTO_xVIx_xAIx_VAR)
  Crypto_xVIx_xAIx_AesCfbDecryptContext;
#endif /* #if (CRYPTO_XVIX_XAIX_AES_CFB_DEC_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Constants with external linkage]=============================================================*/

/*==[Variables with external linkage]=============================================================*/

/*==[Definition of functions with external linkage]===============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#if (CRYPTO_XVIX_XAIX_AES_CFB_ENC_ENABLED == STD_ON)

FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CFB_Start
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint32 InputBlockLength = CRYPTO_XVIX_XAIX_BLOCK_SIZE;
  /* reset the context */
  /* !LINKSTO SWS_Crypto_00017, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Start, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Start2Start, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Update2Start, 1 */
  Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CFB_ResetCtx();
  Crypto_xVIx_xAIx_AesCfbEncryptContext.BlockValues.Key.length =
    Job->jobPrimitiveInfo->primitiveInfo->algorithm.keyLength;
  Crypto_xVIx_xAIx_AesCfbEncryptContext.Job = Job;
  Crypto_xVIx_xAIx_AesCfbEncryptContext.ObjId = ObjectId;
  /* read the IV and store it in the input block for the first round */
  /* !LINKSTO EB_Crypto_CryptAlgos_1307, 1 */
  RetVal = Crypto_xVIx_xAIx_AL_KeyElementGet
           (
             Job->cryptoKeyId,
             CRYPTO_KE_CIPHER_IV,
             Crypto_xVIx_xAIx_AesCfbEncryptContext.BlockValues.InputBlock,
             &InputBlockLength
           );
  /* check for correct IV size */
  /* !LINKSTO EB_Crypto_CryptAlgos_1314, 1 */
  if(CRYPTO_XVIX_XAIX_BLOCK_SIZE != InputBlockLength)
  {
    RetVal = CRYPTO_E_KEY_SIZE_MISMATCH;
  }
  else
  {
    if(E_OK == RetVal)
    {
      RetVal = Crypto_xVIx_xAIx_AL_KeyElementGet
               (
                 Job->cryptoKeyId,
                 CRYPTO_KE_CIPHER_KEY,
                 Crypto_xVIx_xAIx_AesCfbEncryptContext.BlockValues.Key.data,
                 &Crypto_xVIx_xAIx_AesCfbEncryptContext.BlockValues.Key.length
               );
      /* currently only 16 byte supported */
      /* !LINKSTO EB_Crypto_CryptAlgos_1302, 1 */
      Crypto_xVIx_xAIx_AesCfbEncryptContext.BlockValues.SegmentLength = 16U;
      /* No MainCall is needed after start so it is done immediately. */
      Crypto_xVIx_xAIx_AesCfbEncryptContext.State = CRYPTO_XVIX_XAIX_START_DONE_STATE;
      Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CFB_CallBack(Crypto_xVIx_xAIx_AesCfbEncryptContext.Job, E_OK);
    }
  }
  if(E_OK != RetVal)
  {
    /* This function can only return E_OK */
    (void)Crypto_xVIx_xAIx_ENCRYPT_AES_CFB_Error();
  }
  return RetVal;
}

FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CFB_Update
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = CRYPTO_E_BUSY;
  TS_PARAM_UNUSED(ObjectId);
  /* !LINKSTO EB_Crypto_CryptAlgos_1316, 1 */
  if(
      (CRYPTO_XVIX_XAIX_START_DONE_STATE == Crypto_xVIx_xAIx_AesCfbEncryptContext.State) ||
      (CRYPTO_XVIX_XAIX_UPDATE_DONE_STATE == Crypto_xVIx_xAIx_AesCfbEncryptContext.State)
    )
  {
    Crypto_xVIx_xAIx_AesCfbEncryptContext.WrittenBytes = 0U;
    Crypto_xVIx_xAIx_AesCfbEncryptContext.ReadBytes = 0U;
    Crypto_xVIx_xAIx_AesCfbEncryptContext.PlainTextPtr = Job->jobPrimitiveInputOutput.inputPtr;
    Crypto_xVIx_xAIx_AesCfbEncryptContext.PlainTextLength =
      Job->jobPrimitiveInputOutput.inputLength;
    Crypto_xVIx_xAIx_AesCfbEncryptContext.CipherTextPtr = Job->jobPrimitiveInputOutput.outputPtr;
    Crypto_xVIx_xAIx_AesCfbEncryptContext.CipherTextLengthPtr =
    Job->jobPrimitiveInputOutput.outputLengthPtr;
    Crypto_xVIx_xAIx_AesCfbEncryptContext.State = CRYPTO_XVIX_XAIX_AES_CFB_CALC_ROUND_STATE;
    RetVal = E_OK;
  }
  else
  {
    /* This function can only return E_OK */
    (void)Crypto_xVIx_xAIx_ENCRYPT_AES_CFB_Error();
  }
  return RetVal;
}

FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CFB_Finish
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = CRYPTO_E_BUSY;
  TS_PARAM_UNUSED(ObjectId);
  /* !LINKSTO EB_Crypto_CryptAlgos_1317, 1 */
  if(CRYPTO_XVIX_XAIX_UPDATE_DONE_STATE == Crypto_xVIx_xAIx_AesCfbEncryptContext.State)
  {
    Crypto_xVIx_xAIx_AesCfbEncryptContext.CipherTextPtr = Job->jobPrimitiveInputOutput.outputPtr;
    Crypto_xVIx_xAIx_AesCfbEncryptContext.CipherTextLengthPtr =
      Job->jobPrimitiveInputOutput.outputLengthPtr;
    Crypto_xVIx_xAIx_AesCfbEncryptContext.State = CRYPTO_XVIX_XAIX_AES_CFB_FINISH_STATE;
    RetVal = E_OK;
  }
  else
  {
    /* This function can only return E_OK */
    (void)Crypto_xVIx_xAIx_ENCRYPT_AES_CFB_Error();
  }
  return RetVal;
}

FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CFB_Cancel
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  /* This state indicates, that the Primitive is currently waiting for an Update/Finish call */
  if(
      (CRYPTO_XVIX_XAIX_START_DONE_STATE == Crypto_xVIx_xAIx_AesCfbEncryptContext.State) ||
      (CRYPTO_XVIX_XAIX_UPDATE_DONE_STATE == Crypto_xVIx_xAIx_AesCfbEncryptContext.State)
    )
  {
    /* !LINKSTO SWS_Crypto_00122_CORRECTION, 1 */
    Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CFB_ResetCtx();
    RetVal = E_OK;
  }
  else
  {
    /* job can not be canceled imideatly */
    Crypto_xVIx_xAIx_AesCfbEncryptContext.State = CRYPTO_XVIX_XAIX_CANCEL_STATE;
    /* !LINKSTO SWS_Crypto_00143_CORRECTION_SWS_Crypto_00183, 1 */
    RetVal = CRYPTO_E_JOB_CANCELED;
  }
  TS_PARAM_UNUSED(Job);
  TS_PARAM_UNUSED(ObjectId);
  return RetVal;
}

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CFB_MainFunction
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
)
{
  if(FALSE == Crypto_xVIx_xAIx_CheckSkipMain(
                                              Result,
                                              Busy,
                                              Crypto_xVIx_xAIx_AesCfbEncryptContext.ObjId
                                            )
    )
  {
    Crypto_xVIx_xAIx_AesCfbEncryptContext.Result = Result;
    Crypto_xVIx_xAIx_AesCfbEncryptContext.Busy = Busy;
    if(NULL_PTR != Busy)
    {
      *Busy = TRUE;
    }
    CRYPTO_XVIX_XAIX_DOTRANSITION
    (
      CRYPTO_XVIX_XAIX_CFB_TRANSITION_COUNT,
      Crypto_xVIx_xAIx_AesCfbEncryptContext.State,
      Crypto_xVIx_xAIx_Actions_CfbEncrypt,
      Crypto_xVIx_xAIx_StateTrans_CfbEncrypt
    );
  }
}
#endif /*#if (CRYPTO_XVIX_XAIX_AES_CFB_ENC_ENABLED == STD_ON)*/

#if (CRYPTO_XVIX_XAIX_AES_CFB_DEC_ENABLED == STD_ON)

FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_CFB_Start
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint32 InputBlockLength = CRYPTO_XVIX_XAIX_BLOCK_SIZE;
  /* reset the context */
  /* !LINKSTO SWS_Crypto_00017, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Start, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Start2Start, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Update2Start, 1 */
  Crypto_xVIx_xAIx_AL_DECRYPT_AES_CFB_ResetCtx();
  Crypto_xVIx_xAIx_AesCfbDecryptContext.BlockValues.Key.length =
    Job->jobPrimitiveInfo->primitiveInfo->algorithm.keyLength;
  Crypto_xVIx_xAIx_AesCfbDecryptContext.Job = Job;
  Crypto_xVIx_xAIx_AesCfbDecryptContext.ObjId = ObjectId;
  /* read the IV and store it in the input block for the first round */
  /* !LINKSTO EB_Crypto_CryptAlgos_1324, 1 */
  RetVal = Crypto_xVIx_xAIx_AL_KeyElementGet
           (
             Job->cryptoKeyId,
             CRYPTO_KE_CIPHER_IV,
             Crypto_xVIx_xAIx_AesCfbDecryptContext.BlockValues.InputBlock,
             &InputBlockLength
           );
  /* check for correct IV size */
  /* !LINKSTO EB_Crypto_CryptAlgos_1331, 1 */
  if(CRYPTO_XVIX_XAIX_BLOCK_SIZE != InputBlockLength)
  {
    RetVal = CRYPTO_E_KEY_SIZE_MISMATCH;
  }
  else
  {
    if(E_OK == RetVal)
    {
      RetVal = Crypto_xVIx_xAIx_AL_KeyElementGet
               (
                 Job->cryptoKeyId,
                 CRYPTO_KE_CIPHER_KEY,
                 Crypto_xVIx_xAIx_AesCfbDecryptContext.BlockValues.Key.data,
                 &Crypto_xVIx_xAIx_AesCfbDecryptContext.BlockValues.Key.length
               );
      /* currently only 16 byte supported */
      /* !LINKSTO EB_Crypto_CryptAlgos_1302, 1 */
      Crypto_xVIx_xAIx_AesCfbDecryptContext.BlockValues.SegmentLength = 16U;
      Crypto_xVIx_xAIx_AesCfbDecryptContext.State = CRYPTO_XVIX_XAIX_START_DONE_STATE;
      Crypto_xVIx_xAIx_AL_DECRYPT_AES_CFB_CallBack(Crypto_xVIx_xAIx_AesCfbDecryptContext.Job, E_OK);
    }
  }
  if(E_OK != RetVal)
  {
    /* This function can only return E_OK */
    (void)Crypto_xVIx_xAIx_DECRYPT_AES_CFB_Error();
  }
  return RetVal;
}

FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_CFB_Update
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = CRYPTO_E_BUSY;
  TS_PARAM_UNUSED(ObjectId);
  /* !LINKSTO EB_Crypto_CryptAlgos_1333, 1 */
  if(
      (CRYPTO_XVIX_XAIX_START_DONE_STATE == Crypto_xVIx_xAIx_AesCfbDecryptContext.State) ||
      (CRYPTO_XVIX_XAIX_UPDATE_DONE_STATE == Crypto_xVIx_xAIx_AesCfbDecryptContext.State)
    )
  {
    Crypto_xVIx_xAIx_AesCfbDecryptContext.WrittenBytes = 0U;
    Crypto_xVIx_xAIx_AesCfbDecryptContext.ReadBytes = 0U;
    Crypto_xVIx_xAIx_AesCfbDecryptContext.CipherTextPtr = Job->jobPrimitiveInputOutput.inputPtr;
    Crypto_xVIx_xAIx_AesCfbDecryptContext.CipherTextLength =
      Job->jobPrimitiveInputOutput.inputLength;
    Crypto_xVIx_xAIx_AesCfbDecryptContext.PlainTextPtr = Job->jobPrimitiveInputOutput.outputPtr;
    Crypto_xVIx_xAIx_AesCfbDecryptContext.PlainTextLengthPtr =
      Job->jobPrimitiveInputOutput.outputLengthPtr;
    Crypto_xVIx_xAIx_AesCfbDecryptContext.State = CRYPTO_XVIX_XAIX_AES_CFB_CALC_ROUND_STATE;
    RetVal = E_OK;
  }
  else
  {
    /* This function can only return E_OK */
    (void)Crypto_xVIx_xAIx_DECRYPT_AES_CFB_Error();
  }
  return RetVal;
}

FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_CFB_Finish
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = CRYPTO_E_BUSY;
  TS_PARAM_UNUSED(ObjectId);
  /* !LINKSTO EB_Crypto_CryptAlgos_1334, 1 */
  if(CRYPTO_XVIX_XAIX_UPDATE_DONE_STATE == Crypto_xVIx_xAIx_AesCfbDecryptContext.State)
  {
    Crypto_xVIx_xAIx_AesCfbDecryptContext.PlainTextPtr = Job->jobPrimitiveInputOutput.outputPtr;
    Crypto_xVIx_xAIx_AesCfbDecryptContext.PlainTextLengthPtr =
      Job->jobPrimitiveInputOutput.outputLengthPtr;
    Crypto_xVIx_xAIx_AesCfbDecryptContext.State = CRYPTO_XVIX_XAIX_AES_CFB_FINISH_STATE;
    RetVal = E_OK;
  }
  else
  {
    /* This function can only return E_OK */
    (void)Crypto_xVIx_xAIx_DECRYPT_AES_CFB_Error();
  }
  return RetVal;
}

FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_CFB_Cancel
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  /* This state indicates, that the Primitive is currently waiting for an Update/Finish call */
  if(
      (CRYPTO_XVIX_XAIX_START_DONE_STATE == Crypto_xVIx_xAIx_AesCfbDecryptContext.State) ||
      (CRYPTO_XVIX_XAIX_UPDATE_DONE_STATE == Crypto_xVIx_xAIx_AesCfbDecryptContext.State)
    )
  {
    /* !LINKSTO SWS_Crypto_00122_CORRECTION, 1 */
    Crypto_xVIx_xAIx_AL_DECRYPT_AES_CFB_ResetCtx();
    RetVal = E_OK;
  }
  else
  {
    /* job can not be canceled imideatly */
    Crypto_xVIx_xAIx_AesCfbDecryptContext.State = CRYPTO_XVIX_XAIX_CANCEL_STATE;
    /* !LINKSTO SWS_Crypto_00143_CORRECTION_SWS_Crypto_00183, 1 */
    RetVal = CRYPTO_E_JOB_CANCELED;
  }
  TS_PARAM_UNUSED(Job);
  TS_PARAM_UNUSED(ObjectId);
  return RetVal;
}

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_CFB_MainFunction
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
)
{
  if(FALSE == Crypto_xVIx_xAIx_CheckSkipMain(
                                              Result,
                                              Busy,
                                              Crypto_xVIx_xAIx_AesCfbDecryptContext.ObjId
                                            )
    )
  {
    Crypto_xVIx_xAIx_AesCfbDecryptContext.Result = Result;
    Crypto_xVIx_xAIx_AesCfbDecryptContext.Busy = Busy;
    if(NULL_PTR != Busy)
    {
      *Busy = TRUE;
    }
    CRYPTO_XVIX_XAIX_DOTRANSITION
    (
      CRYPTO_XVIX_XAIX_CFB_TRANSITION_COUNT,
      Crypto_xVIx_xAIx_AesCfbDecryptContext.State,
      Crypto_xVIx_xAIx_Actions_CfbDecrypt,
      Crypto_xVIx_xAIx_StateTrans_CfbDecrypt
    );
  }
}

#endif /* #if (CRYPTO_XVIX_XAIX_AES_CFB_DEC_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Definition of functions with internal linkage]===============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AES_CFB_ShiftBytes
(
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Array,
        uint32                                        ArrayLength,
        uint32                                        ShiftLength
)
{
  TS_PARAM_UNUSED(ShiftLength);
  TS_MemBZero(Array, ArrayLength);

}

static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AES_CFB_CalcSingleRound
(
  P2VAR(Crypto_xVIx_xAIx_AesCfbBlockValuesType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) BlockValues
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint8 i;
  /* !LINKSTO EB_Crypto_CryptAlgos_1304, 1 */
  /* !LINKSTO EB_Crypto_CryptAlgos_1321, 1 */
  /* !LINKSTO EB_Crypto_CryptAlgos_1315, 1 */
  /* !LINKSTO EB_Crypto_CryptAlgos_1332, 1 */
  RetVal = Crypto_xVIx_xAIx_AES_Helper_Encrypt_SyncSingleCall
           (
             BlockValues->InputBlock,
             CRYPTO_XVIX_XAIX_BLOCK_SIZE,
             BlockValues->OutputBlock,
             (P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA))&BlockValues->SegmentLength,
             &BlockValues->Key
           );

  /* XOR the output block with the plaintext block */
  if(E_OK == RetVal)
  {
    /* !LINKSTO EB_Crypto_CryptAlgos_1305, 1 */
    /* !LINKSTO EB_Crypto_CryptAlgos_1322, 1 */
    for(i = 0U; i < BlockValues->SegmentLength; i++)
    {
      BlockValues->OutputBlock[i] ^= BlockValues->XORBlock[i];
    }
  }
  else
  {
    /* Helper can only fail if key has invalid size */
    RetVal = CRYPTO_E_KEY_SIZE_MISMATCH;
  }
  return RetVal;
}

#if (CRYPTO_XVIX_XAIX_AES_CFB_ENC_ENABLED == STD_ON)

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CFB_ResetCtx
(
  void
)
{
  Crypto_xVIx_xAIx_AL_ENCRYPT_AES_ECB_Reset();
  TS_MemBZero(
               &Crypto_xVIx_xAIx_AesCfbEncryptContext,
               sizeof(Crypto_xVIx_xAIx_AesCfbEncryptContext)
             );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CFB_ResetCtxAndCDO
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint32 ObjId
)
{
  /* This function is used to reset the context before resetting the driver object. */
  Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CFB_ResetCtx();
  /* !LINKSTO EB_Crypto_01050, 1 */
  SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
  Crypto_xVIx_xAIx_DriverObjects[ObjId].DriverObjectState =
    CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
  Crypto_xVIx_xAIx_DriverObjects[ObjId].CurrentJob = NULL_PTR;
  Crypto_xVIx_xAIx_DriverObjects[ObjId].SkipPeriodicMainFunction = TRUE;
  SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
  /* !LINKSTO Crypto.Dsn.JobTransition.ActiveCancel2Idle, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Idle, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.ActiveError2Idle, 1 */
  Job->jobState = CRYPTO_JOBSTATE_IDLE;
}

static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ENCRYPT_AES_CFB_Error
(
  void
)
{
  Crypto_JobType* Job = Crypto_xVIx_xAIx_AesCfbEncryptContext.Job;
  Std_ReturnType CallbackResult = Crypto_xVIx_xAIx_AesCfbEncryptContext.CallbackResult;
  if(
      (NULL_PTR != Crypto_xVIx_xAIx_AesCfbEncryptContext.Result) &&
        (NULL_PTR != Crypto_xVIx_xAIx_AesCfbEncryptContext.Busy)
    )
  {
    *Crypto_xVIx_xAIx_AesCfbEncryptContext.Result =
      Crypto_xVIx_xAIx_AesCfbEncryptContext.CallbackResult;
    *Crypto_xVIx_xAIx_AesCfbEncryptContext.Busy = FALSE;
    Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CFB_ResetCtx();
  }
  else
  {
    Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CFB_ResetCtxAndCDO(
                                                        Job,
                                                        Crypto_xVIx_xAIx_AesCfbEncryptContext.ObjId
                                                      );
    CryIf_CallbackNotification(Job, CallbackResult);
  }
  return E_OK;
}

/* !LINKSTO SWS_Crypto_00144_CORRECTION, 1 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ENCRYPT_AES_CFB_Cancel
(
  void
)
{
  Crypto_JobType* Job = Crypto_xVIx_xAIx_AesCfbEncryptContext.Job;
  if(
      CRYPTO_PROCESSING_SYNC ==
        Crypto_xVIx_xAIx_AesCfbEncryptContext.Job->jobPrimitiveInfo->processingType
    )
  {
    *Crypto_xVIx_xAIx_AesCfbEncryptContext.Result = CRYPTO_E_JOB_CANCELED;
    *Crypto_xVIx_xAIx_AesCfbEncryptContext.Busy = FALSE;
    Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CFB_ResetCtx();
  }
  else
  {
    Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CFB_ResetCtxAndCDO(
                                                        Job,
                                                        Crypto_xVIx_xAIx_AesCfbEncryptContext.ObjId
                                                      );
    CryIf_CallbackNotification(Job, CRYPTO_E_JOB_CANCELED);
  }
  return E_OK;
}

static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ENCRYPT_AES_CFB_CalcRound
(
  void
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  /* no more complete segment. Processing can be stoped. */
  /* !LINKSTO EB_Crypto_CryptAlgos_1308, 1 */
  if(
      (
        Crypto_xVIx_xAIx_AesCfbEncryptContext.PlainTextLength -
          Crypto_xVIx_xAIx_AesCfbEncryptContext.ReadBytes
      ) <
      (
        Crypto_xVIx_xAIx_AesCfbEncryptContext.BlockValues.SegmentLength -
          Crypto_xVIx_xAIx_AesCfbEncryptContext.RemainingBytesLength
      )
    )
  {
    /* copy input into XORBlock */
    TS_MemCpy
    (
      &Crypto_xVIx_xAIx_AesCfbEncryptContext.BlockValues.XORBlock
      [
        Crypto_xVIx_xAIx_AesCfbEncryptContext.RemainingBytesLength
      ],
      &Crypto_xVIx_xAIx_AesCfbEncryptContext.PlainTextPtr
      [
        Crypto_xVIx_xAIx_AesCfbEncryptContext.ReadBytes
      ],
      Crypto_xVIx_xAIx_AesCfbEncryptContext.PlainTextLength -
        Crypto_xVIx_xAIx_AesCfbEncryptContext.ReadBytes
    );
    Crypto_xVIx_xAIx_AesCfbEncryptContext.RemainingBytesLength +=
      Crypto_xVIx_xAIx_AesCfbEncryptContext.PlainTextLength -
      Crypto_xVIx_xAIx_AesCfbEncryptContext.ReadBytes;
    /* !LINKSTO EB_Crypto_CryptAlgos_1312, 1 */
    *Crypto_xVIx_xAIx_AesCfbEncryptContext.CipherTextLengthPtr =
      Crypto_xVIx_xAIx_AesCfbEncryptContext.WrittenBytes;
    RetVal = E_OK;
    Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CFB_CallBack(Crypto_xVIx_xAIx_AesCfbEncryptContext.Job, E_OK);
  }
  else
  {
    /* There is atleast one complete segment to process. */
    /* copy input into remaining bytes */
    TS_MemCpy
    (
      &Crypto_xVIx_xAIx_AesCfbEncryptContext.BlockValues.XORBlock
      [
        Crypto_xVIx_xAIx_AesCfbEncryptContext.RemainingBytesLength
      ],
      &Crypto_xVIx_xAIx_AesCfbEncryptContext.PlainTextPtr
      [
        Crypto_xVIx_xAIx_AesCfbEncryptContext.ReadBytes
      ],
      Crypto_xVIx_xAIx_AesCfbEncryptContext.BlockValues.SegmentLength -
        Crypto_xVIx_xAIx_AesCfbEncryptContext.RemainingBytesLength
    );
    Crypto_xVIx_xAIx_AesCfbEncryptContext.ReadBytes +=
      Crypto_xVIx_xAIx_AesCfbEncryptContext.BlockValues.SegmentLength -
      Crypto_xVIx_xAIx_AesCfbEncryptContext.RemainingBytesLength;
    /* Encrypt the input block */
    RetVal = Crypto_xVIx_xAIx_AES_CFB_CalcSingleRound
             (
               &Crypto_xVIx_xAIx_AesCfbEncryptContext.BlockValues
             );
    if(E_OK == RetVal)
    {
      /* set new input block */
      /* shift bytes */
      /* !LINKSTO EB_Crypto_CryptAlgos_1306, 1 */
      Crypto_xVIx_xAIx_AES_CFB_ShiftBytes
      (
        Crypto_xVIx_xAIx_AesCfbEncryptContext.BlockValues.InputBlock,
        CRYPTO_XVIX_XAIX_BLOCK_SIZE,
        Crypto_xVIx_xAIx_AesCfbEncryptContext.BlockValues.SegmentLength
      );
      /* concatenate last ciphertext segment */
      TS_MemCpy
      (
        &Crypto_xVIx_xAIx_AesCfbEncryptContext.BlockValues.InputBlock
        [
          CRYPTO_XVIX_XAIX_BLOCK_SIZE -
            Crypto_xVIx_xAIx_AesCfbEncryptContext.BlockValues.SegmentLength
        ],
        Crypto_xVIx_xAIx_AesCfbEncryptContext.BlockValues.OutputBlock,
        Crypto_xVIx_xAIx_AesCfbEncryptContext.BlockValues.SegmentLength
      );

      /* set output */
      /* !LINKSTO EB_Crypto_CryptAlgos_1310, 1 */
      /* !LINKSTO EB_Crypto_CryptAlgos_1312, 1 */
      if(
          *Crypto_xVIx_xAIx_AesCfbEncryptContext.CipherTextLengthPtr >=
          (
            Crypto_xVIx_xAIx_AesCfbEncryptContext.WrittenBytes +
              Crypto_xVIx_xAIx_AesCfbEncryptContext.BlockValues.SegmentLength
          )
        )
      {
        TS_MemCpy
        (
          &Crypto_xVIx_xAIx_AesCfbEncryptContext.CipherTextPtr
          [
            Crypto_xVIx_xAIx_AesCfbEncryptContext.WrittenBytes
          ],
          Crypto_xVIx_xAIx_AesCfbEncryptContext.BlockValues.OutputBlock,
          Crypto_xVIx_xAIx_AesCfbEncryptContext.BlockValues.SegmentLength
        );
        Crypto_xVIx_xAIx_AesCfbEncryptContext.WrittenBytes +=
          Crypto_xVIx_xAIx_AesCfbEncryptContext.BlockValues.SegmentLength;
        Crypto_xVIx_xAIx_AesCfbEncryptContext.RemainingBytesLength = 0U;
        RetVal = CRYPTO_E_BUSY;
        Crypto_xVIx_xAIx_AesCfbEncryptContext.CallbackResult = RetVal;
      }
      else
      {
        RetVal = CRYPTO_E_SMALL_BUFFER;
        Crypto_xVIx_xAIx_AesCfbEncryptContext.CallbackResult = RetVal;
      }
    }
    else
    {
      Crypto_xVIx_xAIx_AesCfbEncryptContext.CallbackResult = RetVal;
    }
  }
  return RetVal;
}

static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ENCRYPT_AES_CFB_Finish
(
  void
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  /* Encrypt the input block */
  /* The key size was correct in the update call so it has to be correct still. This function
     can not fail in this case. */
  /* !LINKSTO EB_Crypto_CryptAlgos_1309, 1 */
  (void)Crypto_xVIx_xAIx_AES_CFB_CalcSingleRound
        (
          &Crypto_xVIx_xAIx_AesCfbEncryptContext.BlockValues
        );
  /* set output to the remaining byte length */
  /* !LINKSTO EB_Crypto_CryptAlgos_1311, 1 */
  /* !LINKSTO EB_Crypto_CryptAlgos_1313, 1 */
  if(
      *Crypto_xVIx_xAIx_AesCfbEncryptContext.CipherTextLengthPtr >=
        Crypto_xVIx_xAIx_AesCfbEncryptContext.RemainingBytesLength
    )
  {
    /* This variable is needed for resetting the context before calling the callback. */
    Crypto_JobType* Job = Crypto_xVIx_xAIx_AesCfbEncryptContext.Job;
    TS_MemCpy
    (
      Crypto_xVIx_xAIx_AesCfbEncryptContext.CipherTextPtr,
      Crypto_xVIx_xAIx_AesCfbEncryptContext.BlockValues.OutputBlock,
      Crypto_xVIx_xAIx_AesCfbEncryptContext.RemainingBytesLength
    );
    *Crypto_xVIx_xAIx_AesCfbEncryptContext.CipherTextLengthPtr =
      Crypto_xVIx_xAIx_AesCfbEncryptContext.RemainingBytesLength +
      Crypto_xVIx_xAIx_AesCfbEncryptContext.UpdateOutputLength;
    RetVal = E_OK;
    if(
        (NULL_PTR != Crypto_xVIx_xAIx_AesCfbEncryptContext.Result) &&
        (NULL_PTR != Crypto_xVIx_xAIx_AesCfbEncryptContext.Busy)
      )
    {
      *Crypto_xVIx_xAIx_AesCfbEncryptContext.Result = E_OK;
      *Crypto_xVIx_xAIx_AesCfbEncryptContext.Busy = FALSE;
    }
    Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CFB_ResetCtxAndCDO(
                                                        Job,
                                                        Crypto_xVIx_xAIx_AesCfbEncryptContext.ObjId
                                                      );
    Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CFB_CallBack(Job, E_OK);
  }
  else
  {
    RetVal = CRYPTO_E_SMALL_BUFFER;
    Crypto_xVIx_xAIx_AesCfbEncryptContext.CallbackResult = RetVal;
  }
  return RetVal;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CFB_CallBack
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint8                                                        Result
)
{
  if(CRYPTO_XVIX_XAIX_CANCEL_STATE != Crypto_xVIx_xAIx_AesCfbEncryptContext.State)
  {
    if(CRYPTO_PROCESSING_SYNC == Job->jobPrimitiveInfo->processingType)
    {
      if(
          (NULL_PTR != Crypto_xVIx_xAIx_AesCfbEncryptContext.Result) &&
          (NULL_PTR != Crypto_xVIx_xAIx_AesCfbEncryptContext.Busy)
        )
      {
        *Crypto_xVIx_xAIx_AesCfbEncryptContext.Result = Result;
        *Crypto_xVIx_xAIx_AesCfbEncryptContext.Busy = FALSE;
      }
    }
    else
    {
      /* processing for async SingleCall and StreamStart */
      if(
          (
            (
              ((Job->jobPrimitiveInputOutput.mode) & ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)) ==
                ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
            ) ||
            (
              ((Job->jobPrimitiveInputOutput.mode) & ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)) ==
                ((uint8) CRYPTO_OPERATIONMODE_STREAMSTART)
            )
          ) && (CRYPTO_XVIX_XAIX_START_DONE_STATE == Crypto_xVIx_xAIx_AesCfbEncryptContext.State)
        )
      {
        Crypto_xVIx_xAIx_AesCfbEncryptContext.StartOutputLength =
          *Job->jobPrimitiveInputOutput.outputLengthPtr;
        /* Update always returns OK */
        (void)Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CFB_Update(
                                                          Crypto_xVIx_xAIx_AesCfbEncryptContext.ObjId,
                                                          Job
                                                        );
      }
      else if(
               (
                 ((Job->jobPrimitiveInputOutput.mode) & ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)) ==
                   ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
               ) &&
                 (CRYPTO_XVIX_XAIX_AES_CFB_CALC_ROUND_STATE == Crypto_xVIx_xAIx_AesCfbEncryptContext.State)
             )
      {
        /* !LINKSTO EB_Crypto_01029, 1 */
        Crypto_xVIx_xAIx_AesCfbEncryptContext.UpdateOutputLength =
          *Job->jobPrimitiveInputOutput.outputLengthPtr;
        Job->jobPrimitiveInputOutput.outputPtr =
          &(Job->jobPrimitiveInputOutput.outputPtr[*Job->jobPrimitiveInputOutput.outputLengthPtr]);
        *Job->jobPrimitiveInputOutput.outputLengthPtr =
          Crypto_xVIx_xAIx_AesCfbEncryptContext.StartOutputLength -
          *Job->jobPrimitiveInputOutput.outputLengthPtr;
        Crypto_xVIx_xAIx_AesCfbEncryptContext.State = CRYPTO_XVIX_XAIX_UPDATE_DONE_STATE;
        /* Finish always returns OK */
        (void)Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CFB_Finish(
                                                          Crypto_xVIx_xAIx_AesCfbEncryptContext.ObjId,
                                                          Job
                                                        );
      }
      else
      {
        CryIf_CallbackNotification(Job, Result);
      }
    }
  }
}

#endif /* #if (CRYPTO_XVIX_XAIX_AES_CFB_ENC_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_AES_CFB_DEC_ENABLED == STD_ON)

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_CFB_ResetCtx
(
  void
)
{
  Crypto_xVIx_xAIx_AL_ENCRYPT_AES_ECB_Reset();
  TS_MemBZero(
               &Crypto_xVIx_xAIx_AesCfbDecryptContext,
               sizeof(Crypto_xVIx_xAIx_AesCfbDecryptContext)
             );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_CFB_ResetCtxAndCDO
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint32 ObjId
)
{
  /* This function is used to reset the context before resetting the driver object. */
  Crypto_xVIx_xAIx_AL_DECRYPT_AES_CFB_ResetCtx();
  /* !LINKSTO EB_Crypto_01050, 1 */
  SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
  Crypto_xVIx_xAIx_DriverObjects[ObjId].DriverObjectState =
    CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
  Crypto_xVIx_xAIx_DriverObjects[ObjId].CurrentJob = NULL_PTR;
  Crypto_xVIx_xAIx_DriverObjects[ObjId].SkipPeriodicMainFunction = TRUE;
  SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
  /* !LINKSTO Crypto.Dsn.JobTransition.ActiveCancel2Idle, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Idle, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.ActiveError2Idle, 1 */
  Job->jobState = CRYPTO_JOBSTATE_IDLE;
}

static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_DECRYPT_AES_CFB_Error
(
  void
)
{
  Crypto_JobType* Job = Crypto_xVIx_xAIx_AesCfbDecryptContext.Job;
  Std_ReturnType CallbackResult = Crypto_xVIx_xAIx_AesCfbDecryptContext.CallbackResult;
  if(
      (NULL_PTR != Crypto_xVIx_xAIx_AesCfbDecryptContext.Result) &&
        (NULL_PTR != Crypto_xVIx_xAIx_AesCfbDecryptContext.Busy)
    )
  {
    *Crypto_xVIx_xAIx_AesCfbDecryptContext.Result =
      Crypto_xVIx_xAIx_AesCfbDecryptContext.CallbackResult;
    *Crypto_xVIx_xAIx_AesCfbDecryptContext.Busy = FALSE;
    Crypto_xVIx_xAIx_AL_DECRYPT_AES_CFB_ResetCtx();
  }
  else
  {
    Crypto_xVIx_xAIx_AL_DECRYPT_AES_CFB_ResetCtxAndCDO(
                                                        Job,
                                                        Crypto_xVIx_xAIx_AesCfbDecryptContext.ObjId
                                                      );
    CryIf_CallbackNotification(Job, CallbackResult);
  }
  return E_OK;
}

/* !LINKSTO SWS_Crypto_00144_CORRECTION, 1 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_DECRYPT_AES_CFB_Cancel
(
  void
)
{
  Crypto_JobType* Job = Crypto_xVIx_xAIx_AesCfbDecryptContext.Job;
  if(
      CRYPTO_PROCESSING_SYNC ==
        Crypto_xVIx_xAIx_AesCfbDecryptContext.Job->jobPrimitiveInfo->processingType
    )
  {
    *Crypto_xVIx_xAIx_AesCfbDecryptContext.Result = CRYPTO_E_JOB_CANCELED;
    *Crypto_xVIx_xAIx_AesCfbDecryptContext.Busy = FALSE;
    Crypto_xVIx_xAIx_AL_DECRYPT_AES_CFB_ResetCtx();
  }
  else
  {
    Crypto_xVIx_xAIx_AL_DECRYPT_AES_CFB_ResetCtxAndCDO(
                                                        Job,
                                                        Crypto_xVIx_xAIx_AesCfbDecryptContext.ObjId
                                                      );
    CryIf_CallbackNotification(Job, CRYPTO_E_JOB_CANCELED);
  }
  return E_OK;
}

static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_DECRYPT_AES_CFB_CalcRound
(
  void
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  /* no more complete segment. Processing can be stoped. */
  /* !LINKSTO EB_Crypto_CryptAlgos_1325, 1 */
  if(
      (
        Crypto_xVIx_xAIx_AesCfbDecryptContext.CipherTextLength -
          Crypto_xVIx_xAIx_AesCfbDecryptContext.ReadBytes
      ) <
      (
        Crypto_xVIx_xAIx_AesCfbDecryptContext.BlockValues.SegmentLength -
          Crypto_xVIx_xAIx_AesCfbDecryptContext.RemainingBytesLength
      )
    )
  {
    /* copy input into XORBlock */
    TS_MemCpy
    (
      &Crypto_xVIx_xAIx_AesCfbDecryptContext.BlockValues.XORBlock
      [
        Crypto_xVIx_xAIx_AesCfbDecryptContext.RemainingBytesLength
      ],
      &Crypto_xVIx_xAIx_AesCfbDecryptContext.CipherTextPtr
      [
        Crypto_xVIx_xAIx_AesCfbDecryptContext.ReadBytes
      ],
      Crypto_xVIx_xAIx_AesCfbDecryptContext.CipherTextLength -
        Crypto_xVIx_xAIx_AesCfbDecryptContext.ReadBytes
    );
    Crypto_xVIx_xAIx_AesCfbDecryptContext.RemainingBytesLength +=
      Crypto_xVIx_xAIx_AesCfbDecryptContext.CipherTextLength -
      Crypto_xVIx_xAIx_AesCfbDecryptContext.ReadBytes;
    /* !LINKSTO EB_Crypto_CryptAlgos_1329, 1 */
    *Crypto_xVIx_xAIx_AesCfbDecryptContext.PlainTextLengthPtr =
      Crypto_xVIx_xAIx_AesCfbDecryptContext.WrittenBytes;
    RetVal = E_OK;
    Crypto_xVIx_xAIx_AL_DECRYPT_AES_CFB_CallBack(Crypto_xVIx_xAIx_AesCfbDecryptContext.Job, E_OK);
  }
  else
  {
    /* There is atleast one complete segment to process. */
    /* copy input into remaining bytes */
    TS_MemCpy
    (
      &Crypto_xVIx_xAIx_AesCfbDecryptContext.BlockValues.XORBlock
      [
        Crypto_xVIx_xAIx_AesCfbDecryptContext.RemainingBytesLength
      ],
      &Crypto_xVIx_xAIx_AesCfbDecryptContext.CipherTextPtr
      [
        Crypto_xVIx_xAIx_AesCfbDecryptContext.ReadBytes
      ],
      Crypto_xVIx_xAIx_AesCfbDecryptContext.BlockValues.SegmentLength -
        Crypto_xVIx_xAIx_AesCfbDecryptContext.RemainingBytesLength
    );
    Crypto_xVIx_xAIx_AesCfbDecryptContext.ReadBytes +=
      Crypto_xVIx_xAIx_AesCfbDecryptContext.BlockValues.SegmentLength -
      Crypto_xVIx_xAIx_AesCfbDecryptContext.RemainingBytesLength;
    /* Encrypt the input block */
    RetVal = Crypto_xVIx_xAIx_AES_CFB_CalcSingleRound
             (
               &Crypto_xVIx_xAIx_AesCfbDecryptContext.BlockValues
             );
    if(E_OK == RetVal)
    {
      /* set new input block */
      /* shift bytes */
      /* !LINKSTO EB_Crypto_CryptAlgos_1323, 1 */
      Crypto_xVIx_xAIx_AES_CFB_ShiftBytes
      (
        Crypto_xVIx_xAIx_AesCfbDecryptContext.BlockValues.InputBlock,
        CRYPTO_XVIX_XAIX_BLOCK_SIZE,
        Crypto_xVIx_xAIx_AesCfbDecryptContext.BlockValues.SegmentLength
      );
      /* concatenate last ciphertext segment */
      TS_MemCpy
      (
        &Crypto_xVIx_xAIx_AesCfbDecryptContext.BlockValues.InputBlock
        [
          CRYPTO_XVIX_XAIX_BLOCK_SIZE -
            Crypto_xVIx_xAIx_AesCfbDecryptContext.BlockValues.SegmentLength
        ],
        Crypto_xVIx_xAIx_AesCfbDecryptContext.BlockValues.XORBlock,
        Crypto_xVIx_xAIx_AesCfbDecryptContext.BlockValues.SegmentLength
      );

      /* set output */
      /* !LINKSTO EB_Crypto_CryptAlgos_1327, 1 */
      /* !LINKSTO EB_Crypto_CryptAlgos_1329, 1 */
      if(
          *Crypto_xVIx_xAIx_AesCfbDecryptContext.PlainTextLengthPtr >=
          (
            Crypto_xVIx_xAIx_AesCfbDecryptContext.WrittenBytes +
              Crypto_xVIx_xAIx_AesCfbDecryptContext.BlockValues.SegmentLength
          )
        )
      {
        TS_MemCpy
        (
          &Crypto_xVIx_xAIx_AesCfbDecryptContext.PlainTextPtr
          [
            Crypto_xVIx_xAIx_AesCfbDecryptContext.WrittenBytes
          ],
          Crypto_xVIx_xAIx_AesCfbDecryptContext.BlockValues.OutputBlock,
          Crypto_xVIx_xAIx_AesCfbDecryptContext.BlockValues.SegmentLength
        );
        Crypto_xVIx_xAIx_AesCfbDecryptContext.WrittenBytes +=
          Crypto_xVIx_xAIx_AesCfbDecryptContext.BlockValues.SegmentLength;
        Crypto_xVIx_xAIx_AesCfbDecryptContext.RemainingBytesLength = 0U;
        RetVal = CRYPTO_E_BUSY;
      }
      else
      {
        RetVal = CRYPTO_E_SMALL_BUFFER;
        Crypto_xVIx_xAIx_AesCfbDecryptContext.CallbackResult = RetVal;
      }
    }
    else
    {
      Crypto_xVIx_xAIx_AesCfbDecryptContext.CallbackResult = RetVal;
    }
  }
  return RetVal;
}

static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_DECRYPT_AES_CFB_Finish
(
  void
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  Crypto_JobType* Job = Crypto_xVIx_xAIx_AesCfbDecryptContext.Job;
  /* Encrypt the input block */
  /* The key size was correct in the update call so it has to be correct still. This function
     can not fail in this case. */
  /* !LINKSTO EB_Crypto_CryptAlgos_1326, 1 */
  (void)Crypto_xVIx_xAIx_AES_CFB_CalcSingleRound
        (
          &Crypto_xVIx_xAIx_AesCfbDecryptContext.BlockValues
        );
  /* set output to the remaining byte length */
  /* !LINKSTO EB_Crypto_CryptAlgos_1328, 1 */
  /* !LINKSTO EB_Crypto_CryptAlgos_1330, 1 */
  if(
      *Crypto_xVIx_xAIx_AesCfbDecryptContext.PlainTextLengthPtr >=
        Crypto_xVIx_xAIx_AesCfbDecryptContext.RemainingBytesLength
    )
  {
    TS_MemCpy
    (
      Crypto_xVIx_xAIx_AesCfbDecryptContext.PlainTextPtr,
      Crypto_xVIx_xAIx_AesCfbDecryptContext.BlockValues.OutputBlock,
      Crypto_xVIx_xAIx_AesCfbDecryptContext.RemainingBytesLength
    );
    *Crypto_xVIx_xAIx_AesCfbDecryptContext.PlainTextLengthPtr =
      Crypto_xVIx_xAIx_AesCfbDecryptContext.RemainingBytesLength +
      Crypto_xVIx_xAIx_AesCfbDecryptContext.UpdateOutputLength;
    RetVal = E_OK;
    if(
        (NULL_PTR != Crypto_xVIx_xAIx_AesCfbDecryptContext.Result) &&
        (NULL_PTR != Crypto_xVIx_xAIx_AesCfbDecryptContext.Busy)
      )
    {
      *Crypto_xVIx_xAIx_AesCfbDecryptContext.Result = E_OK;
      *Crypto_xVIx_xAIx_AesCfbDecryptContext.Busy = FALSE;
    }
    Crypto_xVIx_xAIx_AL_DECRYPT_AES_CFB_ResetCtxAndCDO(
                                                        Job,
                                                        Crypto_xVIx_xAIx_AesCfbDecryptContext.ObjId
                                                      );
    Crypto_xVIx_xAIx_AL_DECRYPT_AES_CFB_CallBack(Job, E_OK);
  }
  else
  {
    RetVal = CRYPTO_E_SMALL_BUFFER;
    Crypto_xVIx_xAIx_AesCfbDecryptContext.CallbackResult = RetVal;
  }
  return RetVal;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_CFB_CallBack
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint8                                                        Result
)
{
  if(CRYPTO_XVIX_XAIX_CANCEL_STATE != Crypto_xVIx_xAIx_AesCfbDecryptContext.State)
  {
    if(CRYPTO_PROCESSING_SYNC == Job->jobPrimitiveInfo->processingType)
    {
      if(
          (NULL_PTR != Crypto_xVIx_xAIx_AesCfbDecryptContext.Result) &&
          (NULL_PTR != Crypto_xVIx_xAIx_AesCfbDecryptContext.Busy)
        )
      {
        *Crypto_xVIx_xAIx_AesCfbDecryptContext.Result = Result;
        *Crypto_xVIx_xAIx_AesCfbDecryptContext.Busy = FALSE;
      }
    }
    else
    {
      /* processing for async SingleCall and StreamStart */
      if(
          (
            (
              ((Job->jobPrimitiveInputOutput.mode) & ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)) ==
                ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
            ) ||
            (
              ((Job->jobPrimitiveInputOutput.mode) & ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)) ==
                ((uint8) CRYPTO_OPERATIONMODE_STREAMSTART)
            )
          ) && (CRYPTO_XVIX_XAIX_START_DONE_STATE == Crypto_xVIx_xAIx_AesCfbDecryptContext.State)
        )
      {
        Crypto_xVIx_xAIx_AesCfbDecryptContext.StartOutputLength =
          *Job->jobPrimitiveInputOutput.outputLengthPtr;
        /* Update always returns OK */
        (void)Crypto_xVIx_xAIx_AL_DECRYPT_AES_CFB_Update(
                                                          Crypto_xVIx_xAIx_AesCfbDecryptContext.ObjId,
                                                          Job
                                                        );
      }
      else if(
               (
                 ((Job->jobPrimitiveInputOutput.mode) & ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)) ==
                   ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
               ) &&
                 (CRYPTO_XVIX_XAIX_AES_CFB_CALC_ROUND_STATE == Crypto_xVIx_xAIx_AesCfbDecryptContext.State)
             )
      {
        /* !LINKSTO EB_Crypto_01029, 1 */
        Crypto_xVIx_xAIx_AesCfbDecryptContext.UpdateOutputLength =
          *Job->jobPrimitiveInputOutput.outputLengthPtr;
        Job->jobPrimitiveInputOutput.outputPtr =
          &(Job->jobPrimitiveInputOutput.outputPtr[*Job->jobPrimitiveInputOutput.outputLengthPtr]);
        *Job->jobPrimitiveInputOutput.outputLengthPtr =
          Crypto_xVIx_xAIx_AesCfbDecryptContext.StartOutputLength -
          *Job->jobPrimitiveInputOutput.outputLengthPtr;
          Crypto_xVIx_xAIx_AesCfbDecryptContext.State = CRYPTO_XVIX_XAIX_UPDATE_DONE_STATE;
        /* Finish always returns OK */
        (void)Crypto_xVIx_xAIx_AL_DECRYPT_AES_CFB_Finish(
                                                          Crypto_xVIx_xAIx_AesCfbDecryptContext.ObjId,
                                                          Job
                                                        );
      }
      else
      {
        CryIf_CallbackNotification(Job, Result);
      }
    }
  }
}

#endif /* #if (CRYPTO_XVIX_XAIX_AES_CFB_DEC_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#else /* #if((CRYPTO_XVIX_XAIX_AES_CFB_ENC_ENABLED == STD_ON) ||
             (CRYPTO_XVIX_XAIX_AES_CFB_DEC_ENABLED == STD_ON)) */

typedef uint8 Crypto_xVIx_xAIx_PrvtEmptyTranslationUnitType;
#endif

/*==[end of file]================================================================================*/

