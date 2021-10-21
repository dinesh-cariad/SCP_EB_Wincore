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

#ifndef CRYPTO_XVIX_XAIX_AL_COMMON_H
#define CRYPTO_XVIX_XAIX_AL_COMMON_H

/*==[Includes]====================================================================================*/

#include <Std_Types.h>
#include <Crypto_xVIx_xAIx_Cfg.h>
#include <Crypto_xVIx_xAIx_Version.h>
#include <Crypto_xVIx_xAIx_Int.h>

/* !LINKSTO SWS_Crypto_00126, 1 */
#if (CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>
#endif

/*==[Macros]======================================================================================*/

#if (defined CRYPTO_XVIX_XAIX_E_EQUAL)
#error CRYPTO_XVIX_XAIX_E_EQUAL already defined
#endif
/** \brief Memory blocks are equal. */
#define CRYPTO_XVIX_XAIX_E_EQUAL 2U

#if (defined CRYPTO_XVIX_XAIX_E_NOT_EQUAL)
#error CRYPTO_XVIX_XAIX_E_NOT_EQUAL already defined
#endif
/** \brief Memory blocks are not equal. */
#define CRYPTO_XVIX_XAIX_E_NOT_EQUAL 3U

#if (defined CRYPTO_XVIX_XAIX_E_SMALLER)
#error CRYPTO_XVIX_XAIX_E_SMALLER already defined
#endif
/** \brief If number1 smaller than number2. */
#define CRYPTO_XVIX_XAIX_E_SMALLER 4U

#if (defined CRYPTO_XVIX_XAIX_E_LARGER)
#error CRYPTO_XVIX_XAIX_E_LARGER already defined
#endif
/** \brief If number1 is greater than number2. */
#define CRYPTO_XVIX_XAIX_E_LARGER 5U

/* !LINKSTO EB_Crypto_CryptAlgos_0103,1 */
/* !LINKSTO EB_Crypto_CryptAlgos_0104,1 */
#if (defined CRYPTO_XVIX_XAIX_AES_BLOCK_LENGTH)
#error CRYPTO_XVIX_XAIX_AES_BLOCK_LENGTH already defined
#endif
/** \brief The Block length for the AES. */
#define CRYPTO_XVIX_XAIX_AES_BLOCK_LENGTH 16U

#if (defined CRYPTO_XVIX_XAIX_COMMON_DER_HEADER_MAX_LEN)
#error CRYPTO_XVIX_XAIX_COMMON_DER_HEADER_MAX_LEN already defined
#endif
/** \brief The maximal possible length of a DER header as returned by
 * Crypto_xVIx_xAIx_CommonDerHeaderSequence() or Crypto_xVIx_xAIx_CommonDerHeaderOctetString().
 */
#define CRYPTO_XVIX_XAIX_COMMON_DER_HEADER_MAX_LEN 6U

#if (defined CRYPTO_XVIX_XAIX_LEN_IN_ALIGN_TYPE)
#error CRYPTO_XVIX_XAIX_LEN_IN_ALIGN_TYPE already defined
#endif
/** \brief Macro to convert byte length to corresponding length of Csm_AlignType. */
#define CRYPTO_XVIX_XAIX_LEN_IN_ALIGN_TYPE(ByteLength) \
((((uint32)(ByteLength) - 1UL) / (uint32)sizeof(Csm_AlignType)) + 1UL)

/*--LN--------------------------------------------------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_LN_PLATFORM_DOUBLE_WORD_SUPPORT)
#error CRYPTO_XVIX_XAIX_LN_PLATFORM_DOUBLE_WORD_SUPPORT is already defined
#endif
/** \brief This macro states if double word support is enabled. */
#ifndef HAS_64BIT_TYPES
#define CRYPTO_XVIX_XAIX_LN_PLATFORM_DOUBLE_WORD_SUPPORT STD_OFF
#else
#define CRYPTO_XVIX_XAIX_LN_PLATFORM_DOUBLE_WORD_SUPPORT STD_ON
#endif

#if (CPU_TYPE == CPU_TYPE_32)

#if (defined CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES)
#error CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES already defined
#endif
/** \brief The length of a word of the long number representation in bytes. */
#define CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES 4U

#if (defined CRYPTO_XVIX_XAIX_LN_WORD_LEN_LEN_BITS)
#error CRYPTO_XVIX_XAIX_LN_WORD_LEN_LEN_BITS already defined
#endif
/** \brief The length of the length of a word in the long number representation in bits. */
#define CRYPTO_XVIX_XAIX_LN_WORD_LEN_LEN_BITS 5U

#if (defined CRYPTO_XVIX_XAIX_LN_HALFWORD_MAX)
#error CRYPTO_XVIX_XAIX_LN_HALFWORD_MAX already defined
#endif
/** \brief The maximal value which can fit into one half of a word. */
#define CRYPTO_XVIX_XAIX_LN_HALFWORD_MAX 0xFFFFU

#if (defined CRYPTO_XVIX_XAIX_LN_WORD_HIGH_BIT)
#error CRYPTO_XVIX_XAIX_LN_WORD_HIGH_BIT already defined
#endif
/** \brief A word where only the most significant bit is set. */
#define CRYPTO_XVIX_XAIX_LN_WORD_HIGH_BIT (Crypto_xVIx_xAIx_LNWordType)0x80000000UL

#elif (CPU_TYPE == CPU_TYPE_16)

#if (defined CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES)
#error CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES already defined
#endif
/** \brief The length of a word of the long number representation in bytes. */
#define CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES 2U

#if (defined CRYPTO_XVIX_XAIX_LN_WORD_LEN_LEN_BITS)
#error CRYPTO_XVIX_XAIX_LN_WORD_LEN_LEN_BITS already defined
#endif
/** \brief The length of the length of a word in the long number representation in bits. */
#define CRYPTO_XVIX_XAIX_LN_WORD_LEN_LEN_BITS 4U

#if (defined CRYPTO_XVIX_XAIX_LN_HALFWORD_MAX)
#error CRYPTO_XVIX_XAIX_LN_HALFWORD_MAX already defined
#endif
/** \brief The maximal value which can fit into one half of a word. */
#define CRYPTO_XVIX_XAIX_LN_HALFWORD_MAX 0xFFU

#if (defined CRYPTO_XVIX_XAIX_LN_WORD_HIGH_BIT)
#error CRYPTO_XVIX_XAIX_LN_WORD_HIGH_BIT already defined
#endif
/** \brief A word where only the most significant bit is set. */
#define CRYPTO_XVIX_XAIX_LN_WORD_HIGH_BIT (Crypto_xVIx_xAIx_LNWordType)0x8000U

#else

#error Unknown CPU_TYPE
#endif

#if (defined CRYPTO_XVIX_XAIX_LNGETBIT)
#error CRYPTO_XVIX_XAIX_LNGETBIT is already defined
#endif
/** \brief  Returns a given bit of a given long number. */
#define CRYPTO_XVIX_XAIX_LNGETBIT(Number,Bit)                                             \
(                                                                                         \
  (Crypto_xVIx_xAIx_LNWordType)                                                           \
  (                                                                                       \
    (Crypto_xVIx_xAIx_LNWordType)                                                         \
    (                                                                                     \
      (Number)[CRYPTO_XVIX_XAIX_LNDIVBYWORDLEN((Bit)) + (Crypto_xVIx_xAIx_LNWordType)1U] >> \
      CRYPTO_XVIX_XAIX_LNMODWORDLEN(Bit)                                                  \
    ) & (Crypto_xVIx_xAIx_LNWordType)1U                                                   \
  )                                                                                       \
)

#if (defined CRYPTO_XVIX_XAIX_MINIMUM)
#error CRYPTO_XVIX_XAIX_MINIMUM is already defined
/** \brief Returns the minimum of two values */
#endif
#define CRYPTO_XVIX_XAIX_MINIMUM(x, y) ((x) < (y)) ? (x) : (y)

#if (defined CRYPTO_XVIX_XAIX_LNLOWHALFWORD)
#error CRYPTO_XVIX_XAIX_LNLOWHALFWORD is already defined
#endif
/** \brief  Extracts the low half word of a given word. */
#define CRYPTO_XVIX_XAIX_LNLOWHALFWORD(A)                                                       \
(                                                                                               \
  (Crypto_xVIx_xAIx_LNWordType)(                                                                \
                                 (A) &                                                          \
                                 (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LN_HALFWORD_MAX  \
                               )                                                                \
)

#if (defined CRYPTO_XVIX_XAIX_LNHIGHHALFWORD)
#error CRYPTO_XVIX_XAIX_LNHIGHHALFWORD is already defined
#endif
/** \brief  Extracts the high half word of a given word. */
#define CRYPTO_XVIX_XAIX_LNHIGHHALFWORD(A) \
((Crypto_xVIx_xAIx_LNWordType)((A) >> (CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS >> 1U)))

#if (defined CRYPTO_XVIX_XAIX_LNDIVBYWORDLEN)
#error CRYPTO_XVIX_XAIX_LNDIVBYWORDLEN is already defined
#endif
/** \brief  Divide a value by the word length in bits. */
#define CRYPTO_XVIX_XAIX_LNDIVBYWORDLEN(X) \
((Crypto_xVIx_xAIx_LNWordType)((X) >> CRYPTO_XVIX_XAIX_LN_WORD_LEN_LEN_BITS))

#if (defined CRYPTO_XVIX_XAIX_LNMODWORDLEN)
#error CRYPTO_XVIX_XAIX_LNMODWORDLEN is already defined
#endif
/** \brief  Compute "x mod CRYPTO_XVIX_XAIX_WORD_LEN_BITS". This only works because
 *  ::CRYPTO_XVIX_XAIX_WORD_LEN_BITS is a power of 2.
 *
 */
#define CRYPTO_XVIX_XAIX_LNMODWORDLEN(X) \
((uint8)((X) & (Crypto_xVIx_xAIx_LNWordType)(CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS - 1U)))

#if (defined CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS)
#error CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS already defined
#endif
/** \brief The length of a word of the long number representation in bits. */
#define CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS (CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES * 8U)

#if (defined CRYPTO_XVIX_XAIX_LN_HALFWORD_LEN_BITS)
#error CRYPTO_XVIX_XAIX_LN_HALFWORD_LEN_BITS is already defined
#endif
/** \brief The length of a half word of the long number representation in bits. */
#define CRYPTO_XVIX_XAIX_LN_HALFWORD_LEN_BITS (CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS / 2U)

#if (defined CRYPTO_XVIX_XAIX_LN_LOW_HALF_WORD)
#error CRYPTO_XVIX_XAIX_LN_LOW_HALF_WORD already defined
#endif
/** \brief Extracts the low half word of a given word. */
#define CRYPTO_XVIX_XAIX_LN_LOW_HALF_WORD(A)                            \
(                                                                       \
  (Crypto_xVIx_xAIx_LNWordType)                                         \
  (                                                                     \
    (A) & (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LN_HALFWORD_MAX \
  )                                                                     \
)

#if (defined CRYPTO_XVIX_XAIX_LN_HIGH_HALF_WORD)
#error CRYPTO_XVIX_XAIX_LN_HIGH_HALF_WORD already defined
#endif
/** \brief Extracts the high half word of a given word. */
#define CRYPTO_XVIX_XAIX_LN_HIGH_HALF_WORD(A) \
((Crypto_xVIx_xAIx_LNWordType)((A) >> (CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS >> 1U)))

#if (defined CRYPTO_XVIX_XAIX_LN_DIV_BY_WORD_LEN)
#error CRYPTO_XVIX_XAIX_LN_DIV_BY_WORD_LEN already defined
#endif
/** \brief Divide a value by the word length in bits. */
#define CRYPTO_XVIX_XAIX_LN_DIV_BY_WORD_LEN(X) \
((Crypto_xVIx_xAIx_LNWordType)((X) >> CRYPTO_XVIX_XAIX_LN_WORD_LEN_LEN_BITS))

#if (defined CRYPTO_XVIX_XAIX_LN_MOD_WORD_LEN)
#error CRYPTO_XVIX_XAIX_LN_MOD_WORD_LEN already defined
#endif
/** \brief Compute "x mod CRYPTO_WORD_LEN_BITS". This only works because
 *         CRYPTO_WORD_LEN_BITS is a power of 2.
 */
#define CRYPTO_XVIX_XAIX_LN_MOD_WORD_LEN(X) \
((uint8)((X) & (Crypto_xVIx_xAIx_LNWordType)(CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS - 1U)))

#if (defined CRYPTO_XVIX_XAIX_LN_NUMBER_LEN)
#error CRYPTO_XVIX_XAIX_LN_NUMBER_LEN already defined
#endif
/** \brief Compute the amount of words that the long number representation of a number will need
 *         from the length of the number in bytes.
 *
 * This is the value "(float)x / (float)CSM_LN_WORD_LEN_BYTES" rounded up.
 * We compute this with the following formula: ((x - 1U) / CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES) + 1U.
 */
#define CRYPTO_XVIX_XAIX_LN_NUMBER_LEN(X)                                               \
(                                                                                       \
  (Crypto_xVIx_xAIx_LNWordType)                                                         \
    (((uint32)(X) - 1U) / CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES) + 1U                      \
)

#if (defined CRYPTO_XVIX_XAIX_LN_GET_BIT)
#error CRYPTO_XVIX_XAIX_LN_GET_BIT already defined
#endif
/** \brief Returns a given bit of a given long number. */
#define CRYPTO_XVIX_XAIX_LN_GET_BIT(Number,Bit)                                               \
(                                                                                             \
  (Crypto_xVIx_xAIx_LNWordType)                                                               \
  (                                                                                           \
    (Crypto_xVIx_xAIx_LNWordType)                                                             \
    (                                                                                         \
      (Number)[CRYPTO_XVIX_XAIX_LN_DIV_BY_WORD_LEN((Bit)) + (Crypto_xVIx_xAIx_LNWordType)1U] >> \
      CRYPTO_XVIX_XAIX_LN_MOD_WORD_LEN((Bit))                                                   \
    ) & (Crypto_xVIx_xAIx_LNWordType)1U                                                       \
  )                                                                                           \
)

#if (defined CRYPTO_XVIX_XAIX_RESULT_MULTI_LENGTH)
#error CRYPTO_XVIX_XAIX_RESULT_MULTI_LENGTH already defined
#endif
/** \brief  Macro representing the length of the buffer which hold the current state of
 **         the multiplication.
 **/
#define CRYPTO_XVIX_XAIX_RESULT_MULTI_LENGTH \
((CRYPTO_XVIX_XAIX_AL_SYM_MAX_KEY_ELEMENT_SIZE * 3U) - 1U)

/*--Statemachine----------------------------------------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_INVALID_STATE)
#error CRYPTO_XVIX_XAIX_INVALID_STATE is already defined
#endif
/** \brief To set the invalid state of a state machine */
#define CRYPTO_XVIX_XAIX_INVALID_STATE 0x99U

#if (defined CRYPTO_XVIX_XAIX_IDLE_STATE)
#error CRYPTO_XVIX_XAIX_IDLE_STATE is already defined
#endif
/** \brief No processing is done in this state. */
#define CRYPTO_XVIX_XAIX_IDLE_STATE 0U

#if (defined CRYPTO_XVIX_XAIX_ERROR_STATE)
#error CRYPTO_XVIX_XAIX_ERROR_STATE is already defined
#endif
/** \brief Processing finsihed with an error. */
#define CRYPTO_XVIX_XAIX_ERROR_STATE 1U

#if (defined CRYPTO_XVIX_XAIX_CANCEL_STATE)
#error CRYPTO_XVIX_XAIX_CANCEL_STATE is already defined
#endif
/** \brief Processing shall be canceled. */
#define CRYPTO_XVIX_XAIX_CANCEL_STATE 2U

#if (defined CRYPTO_XVIX_XAIX_START_DONE_STATE)
#error CRYPTO_XVIX_XAIX_START_DONE_STATE is already defined
#endif
/** \brief Processing of start operation is done. */
#define CRYPTO_XVIX_XAIX_START_DONE_STATE 3U

#if (defined CRYPTO_XVIX_XAIX_UPDATE_DONE_STATE)
#error CRYPTO_XVIX_XAIX_UPDATE_DONE_STATE is already defined
#endif
/** \brief Processing of update operation is done. */
#define CRYPTO_XVIX_XAIX_UPDATE_DONE_STATE 4U

#if (defined CRYPTO_XVIX_XAIX_DOTRANSITION)
#error CRYPTO_XVIX_XAIX_DOTRANSITION already defined
#endif
/** \brief This macro handles the transition.
 *
 * First, the action for the current state is executed (call of the corresponding function pointer
 * in ActionMatrix). Then, TransitionMatrix is searched for the current state and for the return
 * value of the executed action. If found, the next state is set. If not, an invalid state is
 * reached, and that should not happen. Before setting the next state, it is checked, if meanwhile
 * the state has been modified from outside this macro. Then, the 'foreign' state
 * (e.g. cancel state) shall not be the next state.
 *
 */
#define CRYPTO_XVIX_XAIX_DOTRANSITION(TransitionCount, StateVar, ActionMatrix, TransitionMatrix)   \
do                                                                                                 \
{                                                                                                  \
  Std_ReturnType RetVal = E_NOT_OK;                                                                \
  Crypto_xVIx_xAIx_StateType StateAtBegin = (StateVar);                                            \
  Crypto_xVIx_xAIx_StateType NextState = CRYPTO_XVIX_XAIX_INVALID_STATE;                           \
  uint8 i;                                                                                         \
  RetVal = (ActionMatrix)[StateAtBegin]();                                                         \
  for (i = 0U; i < (TransitionCount); i++)                                                         \
  {                                                                                                \
    if (                                                                                           \
         (StateAtBegin == (TransitionMatrix)[i].SrcState) &&                                       \
         (RetVal == (TransitionMatrix)[i].RetVal)                                                  \
       )                                                                                           \
    {                                                                                              \
      NextState = (TransitionMatrix)[i].DstState;                                                  \
      break;                                                                                       \
    }                                                                                              \
  }                                                                                                \
  if (StateAtBegin == (StateVar))                                                                  \
  {                                                                                                \
    (StateVar) = NextState;                                                                        \
  }                                                                                                \
  CRYPTO_XVIX_XAIX_POSTCONDITION_ASSERT(                                                           \
                                         CRYPTO_XVIX_XAIX_INVALID_STATE != NextState,              \
                                         CRYPTO_XVIX_XAIX_MODULE_ID                                \
                                       );                                                          \
}while(0U)

/*--Elliptic Curve--------------------------------------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_KE_KEYEXCHANGE_CURVE)
#error CRYPTO_XVIX_XAIX_KE_KEYEXCHANGE_CURVE is already defined
#endif
/** \brief The corresponding key ID for key element CRYPTO_XVIX_XAIX_KE_KEYEXCHANGE_CURVE. */
#define CRYPTO_XVIX_XAIX_KE_KEYEXCHANGE_CURVE 0x1EU

#if (defined CRYPTO_XVIX_XAIX_EC_DER_LENGTH)
#error CRYPTO_XVIX_XAIX_EC_DER_LENGTH is already defined
#endif
/** \brief The length of DER-Encoding. */
#define CRYPTO_XVIX_XAIX_EC_DER_LENGTH 16U

#if (defined CRYPTO_XVIX_XAIX_SUPPORTED_EC_NUM)
#error CRYPTO_XVIX_XAIX_SUPPORTED_EC_NUM is already defined
#endif
/** \brief The number of supported Elliptic curves. */
#define CRYPTO_XVIX_XAIX_SUPPORTED_EC_NUM 4U

#if(defined CRYPTO_XVIX_XAIX_ECDHX25519_TEMP_LEN_WORDS)
#error CRYPTO_XVIX_XAIX_ECDHX25519_TEMP_LEN_WORDS is already defined
#endif
/** \brief The length, in words, which is needed for the long number representation of
 *         the longest possible temporary number.
 */
#define CRYPTO_XVIX_XAIX_ECDHX25519_TEMP_LEN_WORDS 17U

#if(defined CRYPTO_XVIX_XAIX_ECDH_ARRAY_LEN)
#error CRYPTO_XVIX_XAIX_ECDH_ARRAY_LEN is already defined
#endif
/** \brief The maximal length, in words, which is needed for the long number representation of
 *         the longest possible temporary number.
 *         the value have to be set to 48 for supporting other Elliptic Curves
 */
#define CRYPTO_XVIX_XAIX_ECDH_ARRAY_LEN 48U

#if (defined CRYPTO_XVIX_XAIX_ECC_NUM_LEN_WORDS)
#error CRYPTO_XVIX_XAIX_ECC_NUM_LEN_WORDS is already defined
#endif
/** \brief The length, in words, which is needed for the long number
 *         representation of the longest possible stored number.
 */
#define CRYPTO_XVIX_XAIX_ECC_NUM_LEN_WORDS                                    \
(                                                                             \
  (Crypto_xVIx_xAIx_LNWordType)                                               \
  (                                                                           \
    CRYPTO_XVIX_XAIX_LN_NUMBER_LEN(CRYPTO_XVIX_XAIX_ECDH_ARRAY_LEN) +         \
    (Crypto_xVIx_xAIx_LNWordType)1UL                                          \
  )                                                                           \
)

#if (defined CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS)
#error CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS is already defined
#endif
/** \brief The length, in words, which is needed for the long number
 *         representation of the longest possible temporary number.
 */
#define CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS                                         \
(                                                                                   \
  (Crypto_xVIx_xAIx_LNWordType)                                                     \
  (                                                                                 \
    (CRYPTO_XVIX_XAIX_ECC_NUM_LEN_WORDS * (Crypto_xVIx_xAIx_LNWordType)2UL) -       \
    (Crypto_xVIx_xAIx_LNWordType)1UL                                                \
  )                                                                                 \
)

#if (defined CRYPTO_XVIX_XAIX_MAX_TEMP_LEN_WORDS)
#error CRYPTO_XVIX_XAIX_MAX_TEMP_LEN_WORDS is already defined
#endif
/** \brief Calculates the maximum needed length of a temporary buffer, which is needed for the long
 *         number representation of the longest possible temporary number.
 */
#define CRYPTO_XVIX_XAIX_MAX_TEMP_LEN_WORDS(BYTES)                                \
(                                                                                 \
  (Crypto_xVIx_xAIx_LNWordType)                                                   \
  (                                                                               \
    (CRYPTO_XVIX_XAIX_LN_NUMBER_LEN(BYTES) * (Crypto_xVIx_xAIx_LNWordType)2UL) +  \
      (Crypto_xVIx_xAIx_LNWordType)1UL                                            \
  )                                                                               \
)

#if(defined CRYPTO_XVIX_XAIX_CURVE_SECP256R1_PRIVKEY_LEN_BYTES)
#error CRYPTO_XVIX_XAIX_CURVE_SECP256R1_PRIVKEY_LEN_BYTES is already defined
#endif
/** \brief The private key length of the secp256r1 curve in bytes. */
#define CRYPTO_XVIX_XAIX_CURVE_SECP256R1_PRIVKEY_LEN_BYTES 32U

#if(defined CRYPTO_XVIX_XAIX_CURVE_SECP256R1_PUBKEY_LEN_BYTES)
  #error CRYPTO_XVIX_XAIX_CURVE_SECP256R1_PUBKEY_LEN_BYTES is already defined
#endif
/** \brief The public key length of the secp256r1 curve in bytes. */
#define CRYPTO_XVIX_XAIX_CURVE_SECP256R1_PUBKEY_LEN_BYTES 64U

#if(defined CRYPTO_XVIX_XAIX_CURVE_SECP384R1_PRIVKEY_LEN_BYTES)
  #error CRYPTO_XVIX_XAIX_CURVE_SECP384R1_PRIVKEY_LEN_BYTES is already defined
#endif
/** \brief The private key length of the secp384r1 curve in bytes. */
#define CRYPTO_XVIX_XAIX_CURVE_SECP384R1_PRIVKEY_LEN_BYTES 48U

#if(defined CRYPTO_XVIX_XAIX_CURVE_SECP384R1_PUBKEY_LEN_BYTES)
  #error CRYPTO_XVIX_XAIX_CURVE_SECP384R1_PUBKEY_LEN_BYTES is already defined
#endif
/** \brief The public key length of the secp384r1 curve in bytes. */
#define CRYPTO_XVIX_XAIX_CURVE_SECP384R1_PUBKEY_LEN_BYTES 96U

#if(defined CRYPTO_XVIX_XAIX_CURVE_X25519_PRIVKEY_LEN_BYTES)
  #error CRYPTO_XVIX_XAIX_CURVE_X25519_PRIVKEY_LEN_BYTES is already defined
#endif
/** \brief The private key length of the x25519 curve in bytes. */
#define CRYPTO_XVIX_XAIX_CURVE_X25519_PRIVKEY_LEN_BYTES 32U

#if(defined CRYPTO_XVIX_XAIX_CURVE_X25519_PUBKEY_LEN_BYTES)
  #error CRYPTO_XVIX_XAIX_CURVE_X25519_PUBKEY_LEN_BYTES is already defined
#endif
/** \brief The public key length of the x25519 curve in bytes. */
#define CRYPTO_XVIX_XAIX_CURVE_X25519_PUBKEY_LEN_BYTES 32U

#if(defined CRYPTO_XVIX_XAIX_CURVE_ED25519_PRIVKEY_LEN_BYTES)
  #error CRYPTO_XVIX_XAIX_CURVE_ED25519_PRIVKEY_LEN_BYTES is already defined
#endif
/** \brief The private key length of the Ed25519 curve in bytes. */
#define CRYPTO_XVIX_XAIX_CURVE_ED25519_PRIVKEY_LEN_BYTES 32U

#if(defined CRYPTO_XVIX_XAIX_CURVE_ED25519_PUBKEY_LEN_BYTES)
  #error CRYPTO_XVIX_XAIX_CURVE_ED25519_PUBKEY_LEN_BYTES is already defined
#endif
/** \brief The public key length of the Ed25519 curve in bytes. */
#define CRYPTO_XVIX_XAIX_CURVE_ED25519_PUBKEY_LEN_BYTES 32U

/*-- Key Exchange Default Curves -----------------------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_KEYEXCHANGE_ECCNIST_DEFAULT_CURVE)
#error CRYPTO_XVIX_XAIX_KEYEXCHANGE_ECCNIST_DEFAULT_CURVE is already defined
#endif
/** \brief The default NIST Curve for KeyExchange, of the Algorithm Family CRYPTO_ALGOFAM_ECCNIST
 *         which is supported.
 *         SecP256r1 / NIST P-256 / Prime256v1 Informations
 */
#define CRYPTO_XVIX_XAIX_KEYEXCHANGE_ECCNIST_DEFAULT_CURVE 0U

#if (defined CRYPTO_XVIX_XAIX_KEYEXCHANGE_X25519_DEFAULT_CURVE)
#error CRYPTO_XVIX_XAIX_KEYEXCHANGE_X25519_DEFAULT_CURVE is already defined
#endif
/** \brief The default Curve of the Algorithm Family CRYPTO_ALGOFAM_ED25519
 *         for KeyExchange, which is supported.
 */
#define CRYPTO_XVIX_XAIX_KEYEXCHANGE_X25519_DEFAULT_CURVE 2U

/*-- Signature Gen/Vrfy Default Curves -----------------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_SIGNATURE_ECCNIST_DEFAULT_CURVE)
#error CRYPTO_XVIX_XAIX_SIGNATURE_ECCNIST_DEFAULT_CURVE is already defined
#endif
/** \brief The default NIST Curve of the Algorithm Family CRYPTO_ALGOFAM_ECCNIST
 *         for Signature generation and verification, which is supported.
 *         SecP256r1 / NIST P-256 / Prime256v1 Informations
 */
#define CRYPTO_XVIX_XAIX_SIGNATURE_ECCNIST_DEFAULT_CURVE 0U

#if (defined CRYPTO_XVIX_XAIX_SIGNATURE_ED25519_DEFAULT_CURVE)
#error CRYPTO_XVIX_XAIX_SIGNATURE_ED25519_DEFAULT_CURVE is already defined
#endif
/** \brief The default Curve of the Algorithm Family CRYPTO_ALGOFAM_ED25519
 *         for Signature generation and verification, which is supported.
 */
#define CRYPTO_XVIX_XAIX_SIGNATURE_ED25519_DEFAULT_CURVE 3U

/*==[Types]=======================================================================================*/

/** \brief The crypto return type for compare functions. */
typedef uint8 Crypto_xVIx_xAIx_ReturnType;

/*--LN--------------------------------------------------------------------------------------------*/

#if (CPU_TYPE_32 == CPU_TYPE)

/** \brief The word type. */
typedef uint32 Crypto_xVIx_xAIx_LNWordType;

#if (CRYPTO_XVIX_XAIX_LN_PLATFORM_DOUBLE_WORD_SUPPORT == STD_ON)
/** \brief  The double word type */
typedef uint64 Crypto_xVIx_xAIx_LNDoubleWordType;
#endif

#elif (CPU_TYPE_16 == CPU_TYPE)

/** \brief The word type. */
typedef uint16 Crypto_xVIx_xAIx_LNWordType;

#if (CRYPTO_XVIX_XAIX_LN_PLATFORM_DOUBLE_WORD_SUPPORT == STD_ON)
/** \brief  The double word type */
typedef uint32 Crypto_xVIx_xAIx_LNDoubleWordType;
#endif

#else
#error Unsupported CPU_TYPE
#endif

/** \brief Internal states of the multiplication. */
typedef uint8 Crypto_xVIx_xAIx_LNMultiplyStateType;

/** \brief Internal states of the inversion. */
typedef uint8 Crypto_xVIx_xAIx_LNInversionStateType;

/** \brief Internal states of the modulo computation. */
typedef uint8 Crypto_xVIx_xAIx_LNModuloStateType;

/** \brief Internal states of the modulo exponentiation. */
typedef uint8 Crypto_xVIx_xAIx_LNModExpStateType;

/** \brief Internal states of the Barrett modulo computation. */
typedef uint8 Crypto_xVIx_xAIx_LNModuloBarrettStateType;

/** \brief The context of the long number arithmetic functions.
 *
 * \var LNMultiplyNumbersState
 * \brief Internal state of the function Crypto_xVIx_xAIx_LNMultiplyNumbers().
 *
 * \var LNInversionState
 * \brief State of the function Crypto_xVIx_xAIx_LNInversion().
 *
 * \var LNModExpState
 * \brief Internal state of the modular exponentiation.
 *
 * \var ILoopCounter
 * \brief Loop counter of the arithmetic routines.
 *
 * \var JLoopCounter
 * \brief Loop counter of the arithmetic routines.
 *
 * \var Temp
 * \brief Temporary value in one of the arithmetic routines.
 *
 * \var Rl
 * \brief The low word of a multiplication result of the arithmetic routines.
 *
 * \var Carry
 * \brief A carry used in an addition.
 *
 * \var TempResult
 * \brief Temporary value in one of the arithmetic routines.
 *
 * \var LNModuloState
 * \brief Internal state of the function Crypto_xVIx_xAIx_LNModulo().
 *
 * \var LengthDifference
 * \brief Difference in length of two numbers in the modulo computation.
 *
 * \var WordOfShiftedMod
 * \brief one word of the shifted modulus in the modulo computation.
 *
 * \var CmpNumMod
 * \brief Stores whether number is larger than the shifted modulus in the modulo computation.
 *
 * \var ShiftBits
 * \brief The amount of bits by which each word of modulus is to be shifted in the modulo
 *        computation.
 *
 * \var ShiftWords
 * \brief The amount of full words by which modulus is to be shifted in the modulo computation.
 *
 * \var PosInNum
 * \brief The current position in number in the modulo computation.
 *
 * \var PosInMod
 * \brief The current position in modulus in the modulo computation.
 *
 * \var Bits
 * \brief The current position in the exponent during the fast exponentiation computation.
 *
 * \var LNModuloBarrettState
 * \brief Internal state of the function Crypto_xVIx_xAIx_LNModuloBarrett().
 *
 * \var TempNumber
 * \brief Used for temporary storage of intermediate results.
 */
typedef struct
{
  Crypto_xVIx_xAIx_LNMultiplyStateType                                      LNMultiplyNumbersState;
  Crypto_xVIx_xAIx_LNInversionStateType                                     LNInversionState;
  Crypto_xVIx_xAIx_LNModExpStateType                                        LNModExpState;
  Crypto_xVIx_xAIx_LNWordType                                               ILoopCounter;
  Crypto_xVIx_xAIx_LNWordType                                               JLoopCounter;
  Crypto_xVIx_xAIx_LNWordType                                               Temp;
  Crypto_xVIx_xAIx_LNWordType                                               Rl;
  Crypto_xVIx_xAIx_LNWordType                                               Carry;
  Crypto_xVIx_xAIx_LNWordType                                               TempResult;
  Crypto_xVIx_xAIx_LNModuloStateType                                        LNModuloState;
  Crypto_xVIx_xAIx_LNWordType                                               LengthDifference;
  Crypto_xVIx_xAIx_LNWordType                                               WordOfShiftedMod;
  Crypto_xVIx_xAIx_ReturnType                                               CmpNumMod;
  uint8                                                                     ShiftBits;
  Crypto_xVIx_xAIx_LNWordType                                               ShiftWords;
  Crypto_xVIx_xAIx_LNWordType                                               PosInNum;
  Crypto_xVIx_xAIx_LNWordType                                               PosInMod;
  Crypto_xVIx_xAIx_LNWordType                                               Bits;
  Crypto_xVIx_xAIx_LNModuloBarrettStateType                                 LNModuloBarrettState;
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) TempNumber;
  Crypto_xVIx_xAIx_LNWordType                                               LocalResultMultiply[CRYPTO_XVIX_XAIX_RESULT_MULTI_LENGTH];
}
Crypto_xVIx_xAIx_LNCtxType;

/*--Statemachine----------------------------------------------------------------------------------*/

/** \brief State transition information
 *
 * \var Crypto_xVIx_xAIx_StateTransition::SrcState
 *  \brief Source state
 *
 * \var Crypto_xVIx_xAIx_StateTransition::RetVal
 *  \brief Result of function that processed the source state.
 *
 * \var Crypto_xVIx_xAIx_StateTransition::DstState
 *  \brief Destination state, depending on source state and function's result
 */
typedef struct
{
  uint8 SrcState;
  Std_ReturnType RetVal;
  uint8 DstState;
} Crypto_xVIx_xAIx_StateTransitionType;

/** \brief State function pointer type */
typedef P2FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE, Crypto_xVIx_xAIx_StateFctPtrType) (void);

/** \brief State type of a general state machine */
typedef uint8 Crypto_xVIx_xAIx_StateType;

/*--Elliptic Curves-------------------------------------------------------------------------------*/

/** \brief type of parameters for Ed2551 */
typedef uint16 Crypto_xVIx_xAIx_Ed25519ParamType;

/** \brief The Algorithm Family of the Elliptic Curve */
typedef uint8 Crypto_xVIx_xAIx_ECAlgoFamType;

/** \brief A Point on an Elliptic Curve.
 *
 * \var Crypto_xVIx_xAIx_ECPointType::XValue
 * \brief The x-coordinate of the Point on the curve.
 *
 * \var Crypto_xVIx_xAIx_ECPointType::YValue
 * \brief The y-coordinate of the Point on the curve.
 *
 * \var Crypto_xVIx_xAIx_ECPointType::ZValue
 * \brief If the point is in projective coordinates, this is the z-coordinate of the Point.
 */
typedef struct
{
  Crypto_xVIx_xAIx_LNWordType XValue[CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS];
  Crypto_xVIx_xAIx_LNWordType YValue[CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS];
  Crypto_xVIx_xAIx_LNWordType ZValue[CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS];
}
Crypto_xVIx_xAIx_ECPointType;

/** \brief The OID of the Elliptic Curve.
 *
 * \var Crypto_xVIx_xAIx_ECDEREncodingType::Length
 * \brief The length of OID.
 *
 * \var Crypto_xVIx_xAIx_ECDEREncodingType::Value
 * \brief The OID of the Elliptic Curve.
 */
typedef struct
{
  uint8      Length;
  uint8      Value[CRYPTO_XVIX_XAIX_EC_DER_LENGTH];
}
Crypto_xVIx_xAIx_ECDEREncodingType;

/** \brief The context which has to be used for Elliptic Curve arithmetic.
 *
 * \var Crypto_xVIx_xAIx_ECCtxType::LNCtx
 * \brief Context of underlying long number arithmetic functions.
 *
 * \var Crypto_xVIx_xAIx_ECCtxType::CurrentBit
 * \brief Current bit of the multiplicand in the multiplication.
 *
 * \var Crypto_xVIx_xAIx_ECCtxType::CurveIndex
 * \brief Index of the curve in the elliptic curve information array.
 *
 */
typedef struct
{
  Crypto_xVIx_xAIx_LNCtxType       LNCtx;
  Crypto_xVIx_xAIx_LNWordType      CurrentBit;
  uint8                            CurveIndex;
}
Crypto_xVIx_xAIx_ECCtxType;

/** \brief The different reduction functions for different elliptic curves all have an
 *         identical API. This function pointer type describes that API.
 */
typedef P2FUNC(void, CRYPTO_xVIx_xAIx_APPL_CODE, Crypto_xVIx_xAIx_ECFastReductionFctType)
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  CtxPtr
);

/** \brief Additional information for Elliptic Curve.
 *
 * \var Crypto_xVIx_xAIx_EllipticCurveInfoType::Prime
 * \brief Prime number of Elliptic Curve.
 *
 * \var Crypto_xVIx_xAIx_EllipticCurveInfoType::P
 * \brief The Base Point for the Elliptic Curve.
 *
 * \var Crypto_xVIx_xAIx_EllipticCurveInfoType::Modulus
 * \brief The Order of P.
 *
 * \var Crypto_xVIx_xAIx_EllipticCurveInfoType::Coeff1
 * \brief A real coefficient.
 *
 * \var Crypto_xVIx_xAIx_EllipticCurveInfoType::Coeff2
 * \brief A real coefficient.
 *
 * \var Crypto_xVIx_xAIx_EllipticCurveInfoType::ECDER
 * \brief The DER-Encoding of the corresponding Elliptic Curve.
 *
 * \var Crypto_xVIx_xAIx_EllipticCurveInfoType::AlgoFam
 * \brief The Family Algorithm of the corresponding Elliptic Curve.
 *
 * \var Crypto_xVIx_xAIx_EllipticCurveInfoType::BValue
 *  \brief An integer b with 2^(b-1) > Prime. Ed25519 public keys have exactly b
 *         bits and the Signature has exactly 2*b bits (just needed for Ed25519ph).
 *
 * \var Crypto_xVIx_xAIx_EllipticCurveInfoType::C
 * \brief The integer in c = {2, 3} (just needed for Ed25519ph).
 *
 * \var Crypto_xVIx_xAIx_EllipticCurveInfoType::PrivateKeyLength
 * \brief The Length of the private key in LN-format.
 *
 * \var Crypto_xVIx_xAIx_EllipticCurveInfoType::PublicKeyLength
 * \brief The Length of the Public key in LN-format.
 *
 * \var Crypto_xVIx_xAIx_EllipticCurveInfoType::FastReductionFun
 * \brief For each Curve, we have a function which can perform fast
 *        reduction modulo that prime.
 *
 */
typedef struct
{
  Crypto_xVIx_xAIx_LNWordType              Prime[CRYPTO_XVIX_XAIX_ECC_NUM_LEN_WORDS];
  Crypto_xVIx_xAIx_ECPointType             P;
  Crypto_xVIx_xAIx_LNWordType              Modulus[CRYPTO_XVIX_XAIX_ECC_NUM_LEN_WORDS];
  Crypto_xVIx_xAIx_LNWordType              Coeff1[CRYPTO_XVIX_XAIX_ECC_NUM_LEN_WORDS];
  Crypto_xVIx_xAIx_LNWordType              Coeff2[CRYPTO_XVIX_XAIX_ECC_NUM_LEN_WORDS];
  Crypto_xVIx_xAIx_ECDEREncodingType       ECDER;
  Crypto_xVIx_xAIx_ECAlgoFamType           AlgoFam;
  Crypto_xVIx_xAIx_Ed25519ParamType        BValue;
  Crypto_xVIx_xAIx_Ed25519ParamType        C;
  uint8                                    PrivateKeyLength;
  uint8                                    PublicKeyLength;
  Crypto_xVIx_xAIx_ECFastReductionFctType  FastReductionFun;
}
Crypto_xVIx_xAIx_EllipticCurveInfoType;

/** \brief The context of the Elliptic Curve functions.
 *
 * \var Crypto_xVIx_xAIx_EllipticCurveCtxType::Result
 * \brief A Point, which is used to store the Result of point multiplication.
 *
 * \var Crypto_xVIx_xAIx_EllipticCurveCtxType::PubPoint
 * \brief A Point, which is used to for the public key.
 *
 * \var Crypto_xVIx_xAIx_EllipticCurveCtxType::ECCtx
 * \brief Context of the underlying long number arithmetic functions and curve index informations.
 *
 * \var Crypto_xVIx_xAIx_EllipticCurveCtxType::TmpValue
 * \brief An array which is used as a buffer during point multiplication.
 *
 */
typedef struct
{
  Crypto_xVIx_xAIx_ECPointType     Result;
  Crypto_xVIx_xAIx_ECPointType     PubPoint;
  Crypto_xVIx_xAIx_ECCtxType       ECCtx;
  Crypto_xVIx_xAIx_LNWordType      TmpValue[5U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS];
}
Crypto_xVIx_xAIx_EllipticCurveCtxType;

/*==[Constants with external linkage]=============================================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CONST_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
    (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)   || \
    (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON)  || \
    (CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED == STD_ON)   || \
    (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON)
/** \brief The Elliptic Curves informations. */
extern CONST(Crypto_xVIx_xAIx_EllipticCurveInfoType, CRYPTO_xVIx_xAIx_CONST) Crypto_xVIx_xAIx_EllipticCurveInfo[];
#endif /* #if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
              (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)   || \
              (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON)  || \
              (CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED == STD_ON)   || \
              (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_CONST_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Variables with external linkage]=============================================================*/

/*==[Declaration of functions with external linkage]==============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief Compares the two buffers with the given length.
 *
 * This function compares the two buffers and returns after the first difference.
 *
 * \returns Result of the comparison.
 * \retval E_OK The two buffers are equal.
 * \retval E_NOT_OK The two buffers are not equal.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_MemCmp
(
  CONSTP2CONST(uint8, AUTOMATIC, BASE_APPL_DATA) A,
  CONSTP2CONST(uint8, AUTOMATIC, BASE_APPL_DATA) B,
               uint32                            Length
);

/** \brief Compares the two buffers with the given length.
 *
 * This function compares the two buffers. The function will always compare the full length
 * of the buffers to avoid side channel attacks.
 *
 * \returns Result of the comparison.
 * \retval E_OK The two buffers are equal.
 * \retval E_NOT_OK The two buffers are not equal.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_MemCmp_Sec
(
  CONSTP2CONST(uint8, AUTOMATIC, BASE_APPL_DATA) A,
  CONSTP2CONST(uint8, AUTOMATIC, BASE_APPL_DATA) B,
               uint32                            Length
);

/** \brief State function for CRYPTO_XVIX_XAIX_IDLE_STATE.
 *
 * \returns Result of the processing.
 * \retval E_OK Nothing to do here.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_StateMachineIdle
(
  void
);

/** \brief This function checks if the main call shall be skipped.
 *
 * \param[in] Result  pointer to the result of the sync call.
 * \param[in] Busy    pointer to boolean containing state of main.
 * \param[in] ObjId   Id of the driver object.
 *
 * \returns Skip condition
 * \retval TRUE if the main shall be skipped.
 * \retval FALSE if main shall be processed.
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CheckSkipMain
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy,
  CONST(uint32, CRYPTO_xVIx_xAIx_CONST)                       ObjId
);

/** \brief This function computes a modulo operation
 *
 * This function computes the modulus operation for configurations with activated
 * Barrett reduction.
 *
 * \param[in,out] Number A pointer to the long number representation of the number
 *                       on which the modulo operation should be performed and also
 *                       the place where the result should be stored.
 *                       Crypto_xVIx_xAIx_LNModuloBarrett()
 *                       executes computations on Number, which makes it neccessary,
 *                       that the provided space in memory for Number is at least twice
 *                       the length of the Modulus.
 * \param[in] Modulus    A pointer to the long number representation of the Modulus.
 *                       The Modulus must never be 0.
 * \param[in] Barrett    A pointer to the long number representation of the Barrett value.
 *                       The Barrett value must never be 0.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNModuloBarrett
(
   P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Number,
   P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Modulus,
   P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Barrett,
   P2VAR(Crypto_xVIx_xAIx_LNCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    CtxPtr
);

#if (\
    (CRYPTO_XVIX_XAIX_RSAPSS_VRFY_ENABLED  == STD_ON) || \
    (CRYPTO_XVIX_XAIX_RSAPKCS_VRFY_ENABLED == STD_ON)    \
    )
/** \brief Computes a^b mod c of a given value a, exponent b, and modulus c
 *
 * Performs a modular exponentiation using the square-and-multiply algorithm.
 * The modulo operation uses Barrett reduction.
 *
 * \param[out] Result    A pointer to the buffer where the result is stored
 *                       and also where intermediate results are stored. Because
 *                       of this its length has to be twice the length of Base.
 * \param[in] Base       The base which is exponentiated.
 * \param[in] Modulus    The modulus which is used in the modular exponentiation.
 * \param[in] Exponent   The exponent with which the base is exponentiated.
 * \param[in] Barrett    The Barrett value for the modulo operation
 * \param[in,out] CtxPtr A pointer to a context which holds the current state of the
 *                       inversion. Before the first call of this function, the context
 *                       has to be initialized by a call to Crypto_LNModExpStart().
 *
 * \returns State of the computation
 * \retval TRUE  The exponentiation has finished. Now, "Result" contains the result
 *               of the operation.
 * \retval FALSE The exponentiation has not yet finished. It has to be called again
 *               with the same parameters.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNModExpBarrett
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Result,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Base,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Modulus,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Exponent,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Barrett,
  P2VAR(Crypto_xVIx_xAIx_LNCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    CtxPtr
);
#endif

/** \brief Checks whether a LongNumber representation is valid
 *
 * Performs a length information check for the given LongNumber representation
 *
 * \param[in] LongNumber         The LongNumber to check
 * \param[in] PermittedMaxLength The max length the LongNumber shall have in [Crypto_xVIx_xAIx_LNWordType]
 *
 * \returns Statement regarding the validity of the LongNumber
 * \retval FALSE The LongNumber is not valid
 * \retval TRUE  The LongNumber is valid
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNIsValid
(
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) LongNumber,
  uint32                                                                      PermittedMaxLength
);

/**  \brief Multiply a number in long number representation by two
 *
 * \param[out] Result The multiplied number.
 * \param[in] Number  A number to multiplied.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNMultByTwo
(
 P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
 P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Number
);

/** \brief This function XOR a block of data to another block of data.
 *
 * \param[in,out] Destination A pointer to the memory area which should hold the XOR-ed data.
 * \param[in] Source          A pointer to the block of data which should be XOR-ed to the
 *                            destination.
 * \param[in] ByteLength      Holds the amount of bytes which should be XOR-ed.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CommonXOR
(
  P2VAR(void, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Destination,
  P2CONST(void, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Source,
  uint32                                     ByteLength
);

/*--LN------------------------------------------------------------------------*/

/** \brief This function computes the binary representation of a number in LN representation.
 *
 * This function computes the length of the binary representation of a number given in long number
 * Representation. Since this function only uses the length information stored in
 * <code>Number[0]</code> and the information stored in <code>Number[Number[0]]</code>,
 * the number must have been normalized beforehand, for example by a call to
 * Crypto_xVIx_xAIx_LNNormalizeNumer(), to obtain a correct result.
 *
 * \param[in] Number A pointer to the number in long number representation,whose bit-length
 *                   shall be computed. It must be a normalized number.
 *
 * \returns The length of the binary representation of the number.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(uint32, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNBitLengthOfNumber
(
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Number
);

/** \brief The most significant words of the long number representation of a number can contain
 *         zeros.This function removes those leading zeros.
 *
 * The most significant words of the long number representation of a number can contain zeros.
 * This function removes those leading zeros.
 *
 * \param[in] Number A pointer to the number in long number representation, whose leading zeros
 *                   shall be removed.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNNormalizeNumber
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Number
);

/** \brief This function multiplies two numbers given in long number representation. The function
 *         is implemented in such a way that multiplicand, multiplicator and result can all point
 *         to the same memory location.
 *
 * \param[out] Result       A pointer to the memory area which should cotain the long number
 *                          representation of the result of the multiplication.
 * \param[in] Multiplicand  A pointer to the long number representation of the multiplicand.
 * \param[in] Multiplicator A pointer to the long number representation of the multiplicator.
 * \param[in] CtxPtr        A pointer to a context which holds the current state of the
 *                          multiplication. Before the first call of this function, the context
 *                          has to be initialized by a call to
 *                          Crypto_xVIx_xAIx_LNMultiplyNumbersStart().
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNMultiplyNumbers
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Result,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Multiplicand,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Multiplicator,
  P2VAR(Crypto_xVIx_xAIx_LNCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    CtxPtr
);

/** \brief This function compares two numbers given in long number representation.
 *
 * \param[in] Number1 A pointer to the long number representation of the first number to be
 *                    compared.
 * \param[in] Number2 A pointer to the long number representation of the second number to be
 *                    compared.
 *
 * \returns Result of the comparison.
 * \retval CRYPTO_XVIX_XAIX_E_SMALLER If the first number is smaller than the second number.
 * \retval CRYPTO_XVIX_XAIX_E_EQUAL   If both numbers are equal.
 * \retval CRYPTO_XVIX_XAIX_E_LARGER  If the first number is larger than the second number.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(Crypto_xVIx_xAIx_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNCompareNumbers
(
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Number1,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Number2
);

/** \brief This function computes a modulo operation.
 *
 * \param[in,out] Number A pointer to the long number representation of the number on which the
 *                       modulo operation should be performed and also the place where the result
 *                       should be stored.
 * \param[in] Modulus    A pointer to the long number representation of the modulus.
 * \param[in,out] CtxPtr A pointer to a context which holds the current state of the modulo
 *                       operation. Before the first call of this function, the context has to be
 *                       initialized by a call to Crypto_xVIx_xAIx_LNModuloStart().
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNModulo
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Number,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Modulus,
  P2VAR(Crypto_xVIx_xAIx_LNCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    CtxPtr
);

/** \brief This function converts a byte array which contains a number in big endian
 *         notation to long number representation.
 *
 * \param[in] Array        A pointer to the memory area which holds the number as a byte array in
 *                         big endian notation.
 * \param[in] ArrayLength  The length of the array in bytes.
 * \param[out] Number      A pointer to the memory area which will hold the long number
 *                         representation of the converted number after the function has finished.
 * \param[in] NumberLength The length of the memory area where the number can be stored in double
 *                         words. (including the first word containing the length of long number
 *                         representation. e.g. 32Byte = 9Word).
 *
 * \returns Error value
 * \retval E_OK     The conversion succeeded.
 * \retval E_NOT_OK There was not enough space to store the number.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNBEByteArrayToNumber
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                     Array,
  uint32                                                                    ArrayLength,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Number,
  Crypto_xVIx_xAIx_LNWordType                                               NumberLength
);

/** \brief This function converts the long number representation of a number to a byte array in
 *         big endian notation.
 *
 * \param[in] Number         A pointer to the long number representation which should be converted.
 * \param[in] Array          A pointer to the memory area which will hold the number as a byte
 *                           array in big endian notation. This means that the most significant
 *                           byte is at array[0].
 * \param[in] ArrayLengthPtr A pointer to a variable which has to contain the length of the memory
 *                           area where the number will be stored. When the function returns it,
 *                           the variable contains the length of the stored number.
 *
 * \returns Error value
 * \retval E_OK     The conversion succeeded.
 * \retval E_NOT_OK There was not enough space to store the number.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNNumberToBEByteArray
(
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Number,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                         Array,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                        ArrayLengthPtr
);

#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON)
/** \brief This function converts the long number representation of a number to a byte array in
 *         big endian notation.
 *
 * \param[in] Number         A pointer to the long number representation which should be converted.
 * \param[in] Array          A pointer to the memory area which will hold the number as a byte
 *                           array in big endian notation. This means that the most significant
 *                           byte is at array[0], that is maybe a leading zero.
 * \param[in] ArrayLength    The length of the memory area where the number will be stored and the
 *                           length the byte array shall have (incl. leading zeros).
 *
 * \returns Error value
 * \retval E_OK     The conversion succeeded.
 * \retval E_NOT_OK There was not enough space to store the number.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNNumberToBEByteArrayWithSize
(
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Number,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                         Array,
  uint32                                                                      ArrayLength
);
#endif /* #if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) */

/** \brief This function converts a byte array which contains a number in little endian
 *         notation to long number representation.
 *
 * \param[in] Array        A pointer to the memory area which holds the number as a byte array in
 *                         little endian notation.
 * \param[in] ArrayLength  The length of the array in bytes.
 * \param[in] Number       A pointer to the memory area which will hold the long number
 *                         representation of the converted number after the function has finished.
 * \param[in] NumberLength The length of the memory area where the number can be stored in double
 *                         words. (including the first word containing the length of long number
 *                         representation. e.g. 32Byte = 9Word).
 *
 * \returns Error value
 * \retval E_OK     The conversion succeeded.
 * \retval E_NOT_OK There was not enough space to store the number.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNLEByteArrayToNumber
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                     Array,
  uint32                                                                    ArrayLength,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Number,
  Crypto_xVIx_xAIx_LNWordType                                               NumberLength
);

/** \brief This function converts the long number representation of a number to a byte array in
 *         little endian notation.
 *
 * \param[in] Number         A pointer to the long number representation which should be converted.
 * \param[in] Array          A pointer to the memory area which will hold the number as a byte
 *                           array in little endian notation.
 * \param[in] ArrayLengthPtr A pointer to a variable which has to contain the length of the memory
 *                           area where the number will be stored. When the function returns it,
 *                           the variable contains the length of the stored number.
 *
 * \returns Error value
 * \retval E_OK     The conversion succeeded.
 * \retval E_NOT_OK There was not enough space to store the number.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNNumberToLEByteArray
(
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Number,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                         Array,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                        ArrayLengthPtr
);

/** \brief Divides a given Value by two.
 *
 * \param[in,out] Value The byte array which holds the number to be divided.
 * \param[in] Overflow  0 or 1 depending on what shall be added in the highest position.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNDivByTwo
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
  Crypto_xVIx_xAIx_LNWordType                                               Overflow
);

/** \brief The function is implemented in such a way that addend1, addend2 and result can all
 *         point to the same memory location.
 *
 * \param[in,out] Sum The location where the result of the addition will be stored to.
 * \param[in] Addend1 The array to the addend1
 * \param[in] Addend2 The array to the addend2.
 *
 * \returns The carry value.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(uint8, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNAddition
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Sum,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend1,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend2
);

/** \brief The function is implemented in such a way that addend1, addend2 and result can all point
 *         to the same memory location.
 *
 * \param[in,out] Sum The location where the result of the addition will be stored to.
 * \param[in] Addend1 The array to the addend1
 * \param[in] Addend2 The array to the addend2.
 * \param[in] Modulus The array to the modulus of the addition.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNAdditionModular
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Sum,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend1,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend2,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Modulus
);

/** \brief This function subtracts two numbers
 *
 * This function subtracts two numbers from each other.
 * The implementation of the function assumes that |Minuend| >= |Subtrahend|.
 *
 * \param[out] Result    A pointer to a memory location which will hold the
 *                       result of the subtraction.
 * \param[in] Minuend    A pointer to the minuend of the subtraction.
 * \param[in] Subtrahend A pointer to the subtrahend of the subtraction.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNSubtraction
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Minuend,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Subtrahend
);

/** \brief The function is implemented in such a way that minuend, subtrahend and result can all
 *         point to the same memory location. Performs the subtraction of two values in long number
 *         representation.
 *
 * \param[in,out] Difference The location where the result of the subtraction will be stored to.
 * \param[in] Minuend        The array to the minuend.
 * \param[in] Subtrahend     The array to the subtrahend.
 * \param[in] Modulus        The array to the modulus of the subtraction.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNSubtractionModular
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Difference,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Minuend,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Subtrahend,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Modulus
);

/** \brief Performs the inversion of a values in long number representation modulo a given value.
 *
 * \param[in,out] InvValue The value the operation will store the result of the invertion to.
 * \param[in] Value        The array to the value to be inverted.
 * \param[in] Tmp1Value    The array to the first temporary Value needed within the computation.
 * \param[in] Tmp2Value    The array to the second temporary Value needed within the computation
 * \param[in] Tmp3Value    The array to the third temporary Value needed within the computation.
 * \param[in] Modulus      The array to the modulus of the subtraction.
 * \param[in] CtxPtr       A pointer to a context which holds the current state of the inversion.
 *                         Before the first call of this function, the context has to be
 *                         initialized by a call to Crypto_xVIx_xAIx_LNInversionStart().
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNInversion
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   InvValue,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp1Value,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp2Value,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp3Value,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Modulus,
  P2VAR(Crypto_xVIx_xAIx_LNCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    CtxPtr
);

#if                                                     \
(                                                       \
  (CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_ENABLED == STD_ON) || \
  (CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_ENABLED == STD_ON) || \
  (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED   == STD_ON)  || \
  (CRYPTO_XVIX_XAIX_RSAPSS_VRFY_ENABLED  == STD_ON)  || \
  (CRYPTO_XVIX_XAIX_RSAPKCS_VRFY_ENABLED == STD_ON)  || \
  (CRYPTO_XVIX_XAIX_RSAPKCS_GEN_ENABLED == STD_ON)      \
)
/** \brief Performs a modular exponentiation using the square-and-multiply algorithm.
 *
 * \param[out] Result  A pointer to the buffer where the result will be stored and also where
 *                     intermediate results will be stored. Because of this its length has to be
 *                     twice the length of Base.
 * \param[in] Base     The base which should be exponentiated.
 * \param[in] Modulus  The modulus which should be used in the modular exponentiation.
 * \param[in] Exponent The exponent with which the base should be exponentiated.
 * \paran[in] CtxPtr   A pointer to a context which holds the current state of the inversion.
 *                     Before the first call of this function, the context has to be initialized
 *                     by a call to Crypto_xVIx_xAIx_LNModExpStart().
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNModExp
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Result,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Base,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Modulus,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Exponent,
  P2VAR(Crypto_xVIx_xAIx_LNCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    CtxPtr
);
#endif

#if (STD_ON == CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT)
/** \brief Crypto_xVIx_xAIx_AL_Init function to initialize the key elements with their init
 *         values if the NvM blocks were failing.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_Init
(
  void
);
#endif /* #if (STD_ON == CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT) */

#if                                                            \
(                                                              \
  ((CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_ENABLED == STD_ON) ||       \
   (CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_ENABLED == STD_ON) ||       \
   (CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED == STD_ON)     ||       \
   (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON)    ||       \
   (CRYPTO_XVIX_XAIX_RSAPSS_VRFY_ENABLED  == STD_ON)  ||       \
   (CRYPTO_XVIX_XAIX_RSAPKCS_VRFY_ENABLED == STD_ON)  ||       \
   (CRYPTO_XVIX_XAIX_RSAPKCS_GEN_ENABLED == STD_ON)            \
  )                                                            \
  &&                                                           \
  (CRYPTO_XVIX_XAIX_LN_PLATFORM_DOUBLE_WORD_SUPPORT == STD_ON) \
)
/** \brief This function squares a number
 *
 * \param[out] Result  The squared Number.
 * \param[in] Number  A number to be squared.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNSquareComba
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Result,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Number
);
#endif

/** \brief This function calculates the Rho value (equal to (-1 / Modulo[0]) mod (word base))
 *
 * The implementation of the function assumes that the modulo is odd.
 *
 * \param[in] Modulo A pointer to the modulo used for the reduction.
 * \param[out] Rho   A pointer to a memory location which will hold
 *                   the value of the calculated Rho.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNMontgomerySetup
(
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Modulo,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Rho
);

/** \brief This function computes R mod m, in order to convert the input number
 *         into Montgomery domain.
 *
 * \param[inout] Result A pointer to number to be converted; This memory location will
 *                      also hold the conversion result.
 * \param[in] Modulo    A pointer to the modulo used for the reduction.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNMontgomeryNormalization
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Result,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Modulo
);

/** \brief This function computes the modular reduction of the number, in the Montgomery domain.
 *         into Montgomery domain.
 *
 * \param[out] Remainder A pointer to a memory location which will hold
 *                       the value of the remainder.
 * \param[in] Number     A pointer to the number to be reduced.
 * \param[in] Modulo     A pointer to the modulo used for the reduction.
 * \param[in] Rho        The Rho value used to calculate the reduction.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNMontgomeryReduction
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Remainder,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Number,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Modulo,
  Crypto_xVIx_xAIx_LNWordType                                                 Rho
);

#if (CRYPTO_XVIX_XAIX_RSAPKCS_VRFY_ENABLED == STD_ON) || \
    (CRYPTO_XVIX_XAIX_RSAPKCS_GEN_ENABLED == STD_ON)
/** \brief Creates a DER header for a sequence
 **
 ** This function creates a DER header for an ASN.1 sequence. The header
 ** contains a byte which identifies the sequence followed by several bytes
 ** which encode the length of the payload of the sequence
 **
 ** \param[out] HeaderBuffer Holds a pointer to the start of a buffer where the header
 ** should be stored
 ** \param[in] PayloadLength Length of the payload of the sequence
 **
 ** \returns Length of the header in bytes
 **
 **/
extern FUNC(uint32, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CommonDerHeaderSequence
(
   P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) HeaderBuffer,
   uint32                                              PayloadLength
);
/** \brief Creates a DER header for an octet string
 **
 ** This function creates a DER header for an ASN.1 octet string. The header
 ** contains a byte which identifies the octet string followed by several bytes
 ** which encode the length of the payload of the octet string
 **
 ** \param[out] HeaderBuffer Holds a pointer to the start of a buffer where the header
 ** should be stored
 ** \param[in] PayloadLength Length of the payload of the octet string
 **
 ** \returns Length of the header in bytes
 **
 **/
extern FUNC(uint32, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CommonDerHeaderOctetString
(
   P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) HeaderBuffer,
   uint32                                              PayloadLength
);
#endif /* (CRYPTO_XVIX_XAIX_RSAPKCS_VRFY_ENABLED == STD_ON) */


#if (CRYPTO_XVIX_XAIX_RSAPKCS_VRFY_ENABLED == STD_ON)  || \
    (CRYPTO_XVIX_XAIX_RSAPKCS_GEN_ENABLED  == STD_ON)  || \
    (CRYPTO_XVIX_XAIX_RSAPSS_VRFY_ENABLED  == STD_ON)  || \
    (CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_ENABLED == STD_ON) || \
    (CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_ENABLED == STD_ON)
/** \brief This function converts a DER RsaPubKey to Modulus and Exponent of LNWordType.
 *
 *         The RsaPubKey has to present in the correct Form as specified.
 *         RSAPublicKey ::= SEQUENCE {
 *                                     modulus         INTEGER,  -- n
 *                                     publicExponent  INTEGER   -- e
 *                                   }
 *
 * \param[in] Key        A pointer to the input key.
 * \param[in] KeyLength  The length of the input key.
 * \param[out] Modulus   A pointer to where the result of the modulus shall be stored.
 * \param[out] Exponent  A pointer to where the result of the exponent shall be stored.
 *
 * \return Error value
 * \retval E_OK      The conversion succeeded.
 * \retval E_NOT_OK  Format is wrong or output buffers are to small.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DecodeRsaKey
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                     Key,
  uint32                                                                    KeyLength,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Modulus,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Exponent

);
#endif /*  (CRYPTO_XVIX_XAIX_RSAPKCS_VRFY_ENABLED == STD_ON)  || \
           (CRYPTO_XVIX_XAIX_RSAPKCS_GEN_ENABLED  == STD_ON)  || \
           (CRYPTO_XVIX_XAIX_RSAPSS_VRFY_ENABLED  == STD_ON)  || \
           (CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_ENABLED == STD_ON) || \
           (CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_ENABLED == STD_ON)*/

/*----- Elliptic Curves --------------------------------------------------------------------------*/
#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
    (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)   || \
    (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON)  || \
    (CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED == STD_ON)   || \
    (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON)
/** \brief Retrieve the corresponding Elliptic Curve's index
 *
 *         This Function retrieves the corresponding Elliptic Curve's index according to the OID,
 *         Which will be used as KeyExchange.
 *
 *
 *  \param[in]     ServiceInfo     The used Service of the processed Job.
 *  \param[in]     AlgorithmFamily The Family type of the processed Job.
 *  \param[in]     CurveDER        Pointer to the DER-Encoding of the Elliptic Curve in KeyElement,
 *                                 which has to be compared.
 *  \param[in]     CurveDERLength  The Length of the DER-Encoding, which has to be compared.
 *  \param[in,out] CurveIndex      Pointer to the index of the corresponding Elliptic Curve, which
 *                                 will be used for key exchange.
 *
 *  \returns Error value
 *  \RetVal E_OK      The Elliptic Curve is supported.
 *  \RetVal E_NOT_OK  The Elliptic Curve is not supported.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_GetECIndex
(
          uint8                                         ServiceInfo,
          uint8                                         AlgorithmFamily,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CurveDER,
          uint8                                         CurveDERLength,
    P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CurveIndex
);
#endif /* #if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
              (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)   || \
              (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON)  || \
              (CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED == STD_ON)   || \
              (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON) */

/*----- Ed25519 / X25519 -------------------------------------------------------------------------*/
#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
    (CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED == STD_ON)   || \
    (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON)
/** \brief Fast reduction modulo with the polynomial of the Edwards curve
 *
 *         This function performs the fast reduction modulo by using the reduction polynomial which
 *         is the base of the Edwards curve 25519.
 *         It is based on the following observations:
 *         Let "value" have the binary notation
 *         value = c509*(2^509) + c508*(2^508) + ... + c1*2 + c0.
 *         where each cX is either 0 or 1.
 *         Then we can write value as the sum value = A1*(2^255) + A2 with the two 160-bit integers
 *         A1 = c509*(2^254) + c508*(2^253) + ... + c256*2 + c255 and
 *         A2 = c254*(2^254) + c253*(2^253) + ... + c1*2   + c0.
 *         We know that p = 2^255 - 19.
 *         From this follows that 2^255 = 2^4 + 3 (mod p) and therefore
 *         x*(2^255) = x*(2^4 + 3) (mod p) for all x.
 *         But this means that
 *         value = A1*(2^255)      + A2 (mod p)
 *               = A1*(2^4 + 3)    + A2 (mod p)
 *               = A1*(2^4) + 3*A1 + A2 (mod p).
 *         A1 + A2 (mod p) can be computed easily because A1 and A2 are both
 *         smaller than p. Now we want to reduce A1*(2^4) further.
 *         For this we write:
 *         A1*(2^4) = B
 *         A1*(2^4) = B1*(2^255) + B2
 *         Again we use the congruence from above to get
 *         A1*(2^4) = B1*(2^4) + 3*B1 + B2 (mod p)
 *         Everything taken together this yields
 *         value =  3*A1 + A2 + B1*(2^4) + 3*B1 + B2 (mod p)
 *         where all summands are smaller than p. We use this last congruence to
 *         compute the reduction.
 *
 * \param[in,out] Value  Pointer to the start of an array which holds the number to be reduced.
 * \param[in,out] CtxPtr Pointer to a context which holds the current state of the conversion.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAFastReduction
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);
#endif /* #if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
              (CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED == STD_ON)   || \
              (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON) */

/*----- ECC NIST --------------------------------------------------------------------------------*/
#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
    (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)   || \
    (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON)
/** \brief    Fast reduction modulo the polynomial of Elliptic Curve secp256r1
 *
 *            This function performs the fast reduction modulo by using the reduction polynomial
 *            which is the base of the elliptic curve secp256r1 / NIST P-256.
 *
 * \param[in,out] Value  Pointer to the start of an array which holds the number to be reduced.
 * \param[in,out] CtxPtr Pointer to a context which holds the current state of the conversion.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECP256FastReduction
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);
#endif /* #if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
              (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)   || \
              (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON)
/** \brief    Fast reduction modulo the polynomial of Elliptic Curve secp384r1
 *
 *            This function performs the fast reduction modulo by using the reduction polynomial
 *            which is the base of the Elliptic Curve NIST polynomial P-384 / SecP384r1
 *
 * \param[in,out] Value  Pointer to the start of an array which holds the number to be reduced.
 * \param[in,out] CtxPtr Pointer to a context which holds the current state of the conversion.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECP384FastReduction
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);
#endif /* #if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
    (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)   || \
    (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON)
/** \brief Point conversion.
 *
 *         This function converts a Point given in projective coordinates to the
 *         same Point in affine coordinates. The conversion is performed
 *         in slices and the current slice is stored in the given context.
 *
 * \param[in]     Point   Pointer to an Elliptic Curve Point given in projective coordinates
 *                        which should converted.
 * \param[in,out] Tmp     Pointer to a buffer is used as temporary storage during the conversion.
 * \param[in,out] CtxPtr  Pointer to a context which holds the current state of the conversion.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECPointConvertProj2Aff
(
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Point,
  P2VAR(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Point multiplication.
 *
 *         This function multiplies a Point on an Elliptic Curve which is given in
 *         projective coordinates with an integer and returns the Result again in
 *         projective coordinates. The multiplication is performed
 *         in slices and the current slice is stored in the given context.
 *
 * \param[out]    Result       Pointer to an Elliptic Curve Point where the Result of the
 *                             multiplication will be stored in projective coordinates.
 * \param[in]     Point        Pointer to an Elliptic Curve Point given in projective coordinates
 *                             which should multiplied. This parameter must not Point to the same
 *                             memory location as the Result "Result".
 * \param[in]     Multiplicand Pointer an integer with which the Point should be multiplied.
 * \param[in,out] Tmp          An array where buffers are used as temporary storage
 *                             during the multiplication.
 * \param[in,out] CtxPtr       Pointer to a context which holds the current state of the
 *                             multiplication.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECPointMultiplication
(
    P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Point,
  P2CONST(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Multiplicand,
    P2VAR(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
    P2VAR(Crypto_xVIx_xAIx_ECCtxType,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Check if either of the input points are at infinity.
 *
 * \param[in,out] Result   Pointer to an Elliptic Curve Point where the Result of the
 *                         addition will be stored in projective coordinates.
 * \param[in]     Addend1  Pointer to an Elliptic Curve Point given in projective coordinates
 *                         which should be added. This parameter may Point to the
 *                         same memory location as the Result Point "Result".
 * \param[in]     Addend2  Pointer to an Elliptic Curve Point given in projective coordinates
 *                         which should be added. This parameter may Point to
 *                         the same memory location as the Result Point "Result".
 * \param[in,out] Finished Pointer to a flag that indicates if the operation is Finished.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECPointAdditionCheckInfinity
(
    P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend1,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend2,
    P2VAR(boolean,                      AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Finished
);

/** \brief Point addition
 *
 *         This function adds two points on an Elliptic Curve which are given in
 *         projective coordinates and returns the Result again in
 *         projective coordinates. The addition is performed in slices and the
 *         current slice is stored in the given context.
 *
 * \param[in,out] Result  Pointer to an Elliptic Curve Point where the Result of the
 *                        addition will be stored in projective coordinates.
 * \param[in]     Addend1 Pointer to an Elliptic Curve Point given in projective coordinates
 *                        which should be added. This parameter may Point to the
 *                        same memory location as the Result Point "Result".
 * \param[in]     Addend2 Pointer to an Elliptic Curve Point given in projective coordinates
 *                        which should be added. This parameter may Point to the
 *                        same memory location as the Result Point "Result".
 * \param[in,out] Tmp     An array where buffers are used as temporary storage during the addition.
 * \param[in,out] CtxPtr  Pointer to a context which holds the current state of the addition.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECPointAddition
(
    P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend1,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend2,
    P2VAR(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
    P2VAR(Crypto_xVIx_xAIx_ECCtxType,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Check if the input Point is at infinity.
 *
 * \param[in] Point  Point to be checked.
 *
 * \returns If Point is at infinity
 * \retval FALSE     Point is not at infinity.
 * \retval TRUE      Point is at infinity.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
extern FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECProjectivePointOfInfinity
(
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Point
);
#endif /* #if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
              (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)   || \
              (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* CRYPTO_XVIX_XAIX_AL_COMMON_H */

/*==[end of file]=================================================================================*/

