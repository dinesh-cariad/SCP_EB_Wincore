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

/*  MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 8.9 (advisory)
 * An object should be defined at block scope if its identifier only appears in a single function.
 *
 * Reason:
 * The AUTOSAR memory mapping requires that functions are mapped to SEC_CODE memory sections.
 * Objects at block scope require a different mapping, e.g. to a SEC_VAR section, which leads
 * to nested memory sections, which is not supported by some compilers.
 *
 */

/*==[Includes]====================================================================================*/

#include <Std_Types.h>
#include <TSAutosar.h>
#include <Csm_Types.h>
#include <CryIf_Cbk.h>
#include <SchM_Crypto_xVIx_xAIx.h>
#include <TSMem.h>

#include <Crypto_xVIx_xAIx.h>
#include <Crypto_xVIx_xAIx_AL_Cfg.h>
#include <Crypto_xVIx_xAIx_AL_Signature.h>
#include <Crypto_xVIx_xAIx_AL_Hash.h>
#include <Crypto_xVIx_xAIx_AL_Common.h>
#include <Crypto_xVIx_xAIx_AL_KeyManagement.h>

#if ((CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED  == STD_ON) || \
     (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON)    \
    )

/*==[Macros]======================================================================================*/

#if (defined CRYPTO_XVIX_XAIX_SIGNATURE_DEFAULT_CURRENT_MODE)
#error CRYPTO_XVIX_XAIX_SIGNATURE_DEFAULT_CURRENT_MODE is already defined
#endif
/** \brief To set the default current mode */
#define CRYPTO_XVIX_XAIX_SIGNATURE_DEFAULT_CURRENT_MODE 0x00U

/*--EdDSA-----------------------------------------------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_EDDSA_DOM_LEN)
#error CRYPTO_XVIX_XAIX_EDDSA_DOM_LEN is already defined
#endif
/** \brief The length of the Context string for Ed25519ph. */
#define CRYPTO_XVIX_XAIX_EDDSA_DOM_LEN 34U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_NUMBER_LEN)
#error CRYPTO_XVIX_XAIX_EDDSA_NUMBER_LEN is already defined
#endif
/** \brief The length of the Number format */
#define CRYPTO_XVIX_XAIX_EDDSA_NUMBER_LEN 32U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_SIGN_SIZE)
#error CRYPTO_XVIX_XAIX_EDDSA_SIGN_SIZE is already defined
#endif
/** \brief The EdDSA Signature size in bytes. */
#define CRYPTO_XVIX_XAIX_EDDSA_SIGN_SIZE 64U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_NUM_LEN_WORDS)
#error CRYPTO_XVIX_XAIX_EDDSA_NUM_LEN_WORDS is already defined
#endif
/** \brief The length, in words, which is needed for the long number representation of
 *         the longest possible stored number. This length depends on the configured edwards curves.
 */
#define CRYPTO_XVIX_XAIX_EDDSA_NUM_LEN_WORDS                          \
(                                                           \
   (Crypto_xVIx_xAIx_LNWordType)                                      \
   (                                                        \
      CRYPTO_XVIX_XAIX_LN_NUMBER_LEN(CRYPTO_XVIX_XAIX_EDDSA_NUMBER_LEN) +       \
      (Crypto_xVIx_xAIx_LNWordType)1UL                                \
   )                                                        \
)

#if (defined CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS)
#error CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS is already defined
#endif
/** \brief The length, in words, which is needed for the long number representation of
 *         the longest possible temporary number. This length depends on the
 *         configured edwards curves.
 */
#define CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS                           \
(                                                             \
   (Crypto_xVIx_xAIx_LNWordType)                                        \
   (                                                          \
      (CRYPTO_XVIX_XAIX_EDDSA_NUM_LEN_WORDS * (Crypto_xVIx_xAIx_LNWordType)2UL) - \
      (Crypto_xVIx_xAIx_LNWordType)1UL                                  \
   )                                                          \
)

/*--EdDSAGen--------------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED == STD_ON)

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_PRIKEY_LEN)
#error CRYPTO_XVIX_XAIX_EDDSA_G_PRIKEY_LEN is already defined
#endif
/** \brief The length of the private key in bytes. */
#define CRYPTO_XVIX_XAIX_EDDSA_G_PRIKEY_LEN 32U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_ARRAY_LEN)
#error CRYPTO_XVIX_XAIX_EDDSA_G_ARRAY_LEN is already defined
#endif
/** \brief The maximal allowed length for a Byte-array in bytes. */
#define CRYPTO_XVIX_XAIX_EDDSA_G_ARRAY_LEN 32U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_HASH_MAX_LEN)
#error CRYPTO_XVIX_XAIX_EDDSA_G_HASH_MAX_LEN is already defined
#endif
/** \brief The maximal allowed length for a hash digest in bytes. */
#define CRYPTO_XVIX_XAIX_EDDSA_G_HASH_MAX_LEN (CRYPTO_XVIX_XAIX_EDDSA_G_ARRAY_LEN << 1U)

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_HANDLE_FAILING_HASH_PRIVK)
#error CRYPTO_XVIX_XAIX_EDDSA_G_HANDLE_FAILING_HASH_PRIVK is already defined
#endif
/** \brief Handle failing Hash service while processing the private Key */
#define CRYPTO_XVIX_XAIX_EDDSA_G_HANDLE_FAILING_HASH_PRIVK do                                 \
{                                                                                             \
  Crypto_xVIx_xAIx_EdDSAGenCtx.ErrorCode = E_NOT_OK;                                          \
  Crypto_xVIx_xAIx_EdDSAGenCtx.GenState = CRYPTO_XVIX_XAIX_EDDSA_G_S_IDLE;                    \
  Crypto_xVIx_xAIx_EdDSAGenCtx.PrivKeyHashState = CRYPTO_XVIX_XAIX_EDDSA_G_S_INIT_1;          \
  Crypto_xVIx_xAIx_EdDSAGenCtx.CallbackFlag = TRUE;                                           \
  Crypto_xVIx_xAIx_EdDSAGenCtx.CallbackResult = Crypto_xVIx_xAIx_EdDSAGenCtx.ErrorCode;       \
} while(0U)

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_HANDLE_FAILING_HASH_MSG)
#error CRYPTO_XVIX_XAIX_EDDSA_G_HANDLE_FAILING_HASH_MSG is already defined
#endif
/** \brief Handle failing Hash service while processing the Message */
#define CRYPTO_XVIX_XAIX_EDDSA_G_HANDLE_FAILING_HASH_MSG do                                   \
{                                                                                             \
  Crypto_xVIx_xAIx_EdDSAGenCtx.ErrorCode = E_NOT_OK;                                          \
  Crypto_xVIx_xAIx_EdDSAGenCtx.GenState = CRYPTO_XVIX_XAIX_EDDSA_G_S_IDLE;                    \
  Crypto_xVIx_xAIx_EdDSAGenCtx.MsgHashState = CRYPTO_XVIX_XAIX_EDDSA_G_S_INIT_HASH_MESSAGE;   \
  Crypto_xVIx_xAIx_EdDSAGenCtx.CallbackFlag = TRUE;                                           \
    Crypto_xVIx_xAIx_EdDSAGenCtx.CallbackResult = Crypto_xVIx_xAIx_EdDSAGenCtx.ErrorCode;     \
} while(0U)

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_HANDLE_FAILING_HASH_LITR)
#error CRYPTO_XVIX_XAIX_EDDSA_G_HANDLE_FAILING_HASH_LITR is already defined
#endif
/** \brief Handle failing Hash service while processing r */
#define CRYPTO_XVIX_XAIX_EDDSA_G_HANDLE_FAILING_HASH_LITR do                                  \
{                                                                                             \
  Crypto_xVIx_xAIx_EdDSAGenCtx.ErrorCode = E_NOT_OK;                                          \
  Crypto_xVIx_xAIx_EdDSAGenCtx.GenState = CRYPTO_XVIX_XAIX_EDDSA_G_S_IDLE;                    \
  Crypto_xVIx_xAIx_EdDSAGenCtx.LittleRState = CRYPTO_XVIX_XAIX_EDDSA_G_S_INIT_2;              \
  Crypto_xVIx_xAIx_EdDSAGenCtx.CallbackFlag = TRUE;                                           \
    Crypto_xVIx_xAIx_EdDSAGenCtx.CallbackResult = Crypto_xVIx_xAIx_EdDSAGenCtx.ErrorCode;     \
} while(0U)

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_HANDLE_FAILING_HASH_LITK)
#error CRYPTO_XVIX_XAIX_EDDSA_G_HANDLE_FAILING_HASH_LITK is already defined
#endif
/** \brief Handle failing Hash service while processing k */
#define CRYPTO_XVIX_XAIX_EDDSA_G_HANDLE_FAILING_HASH_LITK do                                  \
{                                                                                             \
  Crypto_xVIx_xAIx_EdDSAGenCtx.ErrorCode = E_NOT_OK;                                          \
  Crypto_xVIx_xAIx_EdDSAGenCtx.GenState = CRYPTO_XVIX_XAIX_EDDSA_G_S_IDLE;                    \
  Crypto_xVIx_xAIx_EdDSAGenCtx.LittleKState = CRYPTO_XVIX_XAIX_EDDSA_G_S_INIT_3;              \
  Crypto_xVIx_xAIx_EdDSAGenCtx.CallbackFlag = TRUE;                                           \
    Crypto_xVIx_xAIx_EdDSAGenCtx.CallbackResult = Crypto_xVIx_xAIx_EdDSAGenCtx.ErrorCode;     \
} while(0U)

#endif /* #if (CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED == STD_ON) */

/*--EdDSAVrfy-------------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON)

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_PUBKEY_LEN)
#error CRYPTO_XVIX_XAIX_EDDSA_V_PUBKEY_LEN is already defined
#endif
/** \brief The length of the public key in bytes. */
#define CRYPTO_XVIX_XAIX_EDDSA_V_PUBKEY_LEN 32U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_ARRAY_LEN)
#error CRYPTO_XVIX_XAIX_EDDSA_V_ARRAY_LEN is already defined
#endif
/** \brief The maximal allowed length for a Byte-array in bytes. */
#define CRYPTO_XVIX_XAIX_EDDSA_V_ARRAY_LEN 32U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_HASH_MAX_LEN)
#error CRYPTO_XVIX_XAIX_EDDSA_V_HASH_MAX_LEN is already defined
#endif
/** \brief The maximal allowed length for a hash digest in bytes. */
#define CRYPTO_XVIX_XAIX_EDDSA_V_HASH_MAX_LEN (CRYPTO_XVIX_XAIX_EDDSA_V_ARRAY_LEN << 1U)

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_SIG_S_START_POSITION)
#error CRYPTO_XVIX_XAIX_EDDSA_V_SIG_S_START_POSITION is already defined
#endif
/** \brief Defines the start position of the S component in the reference Signature */
#define CRYPTO_XVIX_XAIX_EDDSA_V_SIG_S_START_POSITION 32U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_HANDLE_FAILING_PUB_KEY_DECODING)
#error CRYPTO_XVIX_XAIX_EDDSA_V_HANDLE_FAILING_PUB_KEY_DECODING is already defined
#endif
/** \brief Handle failing public key decoding */
#define CRYPTO_XVIX_XAIX_EDDSA_V_HANDLE_FAILING_PUB_KEY_DECODING do                             \
{                                                                                               \
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.ErrorCode = E_NOT_OK;                                           \
  if(NULL_PTR != Crypto_xVIx_xAIx_EdDSAVrfyCtx.ResultError)                                     \
  {                                                                                             \
    *Crypto_xVIx_xAIx_EdDSAVrfyCtx.ResultError = Crypto_xVIx_xAIx_EdDSAVrfyCtx.ErrorCode;       \
  }                                                                                             \
  if(NULL_PTR != Crypto_xVIx_xAIx_EdDSAVrfyCtx.IsBusy)                                          \
  {                                                                                             \
    *Crypto_xVIx_xAIx_EdDSAVrfyCtx.IsBusy = FALSE;                                              \
  }                                                                                             \
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.VrfyState = CRYPTO_XVIX_XAIX_EDDSA_V_S_IDLE;                    \
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.DecPubKeyState = CRYPTO_XVIX_XAIX_EDDSA_V_S_CONVERT_PUBLICKEY;  \
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.CallbackFlag = TRUE;                                            \
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.CallbackResult = Crypto_xVIx_xAIx_EdDSAVrfyCtx.ErrorCode;       \
} while(0U)

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_HANDLE_FAILING_HASH_MSG)
#error CRYPTO_XVIX_XAIX_EDDSA_V_HANDLE_FAILING_HASH_MSG is already defined
#endif
/** \brief Handle failing Hash service while processing the Message */
#define CRYPTO_XVIX_XAIX_EDDSA_V_HANDLE_FAILING_HASH_MSG do                                     \
{                                                                                               \
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.ErrorCode = E_NOT_OK;                                           \
  if(NULL_PTR != Crypto_xVIx_xAIx_EdDSAVrfyCtx.ResultError)                                     \
  {                                                                                             \
    *Crypto_xVIx_xAIx_EdDSAVrfyCtx.ResultError = Crypto_xVIx_xAIx_EdDSAVrfyCtx.ErrorCode;       \
  }                                                                                             \
  if(NULL_PTR != Crypto_xVIx_xAIx_EdDSAVrfyCtx.IsBusy)                                          \
  {                                                                                             \
    *Crypto_xVIx_xAIx_EdDSAVrfyCtx.IsBusy = FALSE;                                              \
  }                                                                                             \
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.VrfyState = CRYPTO_XVIX_XAIX_EDDSA_V_S_IDLE;                    \
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.HashMsgState = CRYPTO_XVIX_XAIX_EDDSA_V_S_INIT_HASH_MESSAGE;    \
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.CallbackFlag = TRUE;                                            \
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.CallbackResult = Crypto_xVIx_xAIx_EdDSAVrfyCtx.ErrorCode;       \
} while(0U)

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_HANDLE_FAILING_VALIDATE_SIG_CMP)
#error CRYPTO_XVIX_XAIX_EDDSA_V_HANDLE_FAILING_VALIDATE_SIG_CMP is already defined
#endif
/** \brief Handle failing component validation while processing the Signature */
#define CRYPTO_XVIX_XAIX_EDDSA_V_HANDLE_FAILING_VALIDATE_SIG_CMP do                             \
{                                                                                               \
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.ErrorCode = E_OK;                                               \
  if(NULL_PTR != Crypto_xVIx_xAIx_EdDSAVrfyCtx.ResultError)                                     \
  {                                                                                             \
    *Crypto_xVIx_xAIx_EdDSAVrfyCtx.ResultError = Crypto_xVIx_xAIx_EdDSAVrfyCtx.ErrorCode;       \
  }                                                                                             \
  if(NULL_PTR != Crypto_xVIx_xAIx_EdDSAVrfyCtx.IsBusy)                                          \
  {                                                                                             \
    *Crypto_xVIx_xAIx_EdDSAVrfyCtx.IsBusy = FALSE;                                              \
  }                                                                                             \
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.VrfyState       = CRYPTO_XVIX_XAIX_EDDSA_V_S_IDLE;              \
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.ValSigCmpState  = CRYPTO_XVIX_XAIX_EDDSA_V_S_CONVERT_S;         \
  *Crypto_xVIx_xAIx_EdDSAVrfyCtx.ResultPtr      = CRYPTO_E_VER_NOT_OK;                          \
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.CallbackFlag = TRUE;                                            \
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.CallbackResult = Crypto_xVIx_xAIx_EdDSAVrfyCtx.ErrorCode;       \
} while(0U)

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_HANDLE_FAILING_HASH_LITK)
#error CRYPTO_XVIX_XAIX_EDDSA_V_HANDLE_FAILING_HASH_LITK is already defined
#endif
/** \brief Handle failing Hash service while processing k */
#define CRYPTO_XVIX_XAIX_EDDSA_V_HANDLE_FAILING_HASH_LITK do                                \
{                                                                                           \
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.ErrorCode    = E_NOT_OK;                                    \
  if(NULL_PTR != Crypto_xVIx_xAIx_EdDSAVrfyCtx.ResultError)                                 \
  {                                                                                         \
    *Crypto_xVIx_xAIx_EdDSAVrfyCtx.ResultError = Crypto_xVIx_xAIx_EdDSAVrfyCtx.ErrorCode;   \
  }                                                                                         \
  if(NULL_PTR != Crypto_xVIx_xAIx_EdDSAVrfyCtx.IsBusy)                                      \
  {                                                                                         \
    *Crypto_xVIx_xAIx_EdDSAVrfyCtx.IsBusy = FALSE;                                          \
  }                                                                                         \
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.VrfyState    = CRYPTO_XVIX_XAIX_EDDSA_V_S_IDLE;             \
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.LittleKState = CRYPTO_XVIX_XAIX_EDDSA_V_S_INIT_SIGN_HASH;   \
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.CallbackFlag = TRUE;                                        \
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.CallbackResult = Crypto_xVIx_xAIx_EdDSAVrfyCtx.ErrorCode;   \
} while(0U)

#endif /* #if (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON) */

/*--Crypto_xVIx_xAIx_EdDSAAdditionStateType-------------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_EDDSA_ADD_S_START)
#error CRYPTO_XVIX_XAIX_EDDSA_ADD_S_START is already defined
#endif
/** \brief Start of the addition; check if Addend1 or Addend2 are at infinity */
#define CRYPTO_XVIX_XAIX_EDDSA_ADD_S_START 0U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_ADD_S_CDE)
#error CRYPTO_XVIX_XAIX_EDDSA_ADD_S_CDE is already defined
#endif
/** \brief Calculate C, D and E */
#define CRYPTO_XVIX_XAIX_EDDSA_ADD_S_CDE 2U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_ADD_S_5)
#error CRYPTO_XVIX_XAIX_EDDSA_ADD_S_5 is already defined
#endif
/** \brief Calculate Z1 * Z2 */
#define  CRYPTO_XVIX_XAIX_EDDSA_ADD_S_5 3U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_ADD_S_6)
#error CRYPTO_XVIX_XAIX_EDDSA_ADD_S_6 is already defined
#endif
/** \brief Calculate B, F, G */
#define CRYPTO_XVIX_XAIX_EDDSA_ADD_S_6 4U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_ADD_S_7)
#error CRYPTO_XVIX_XAIX_EDDSA_ADD_S_7 is already defined
#endif
/** \brief Calculate final Z3 */
#define CRYPTO_XVIX_XAIX_EDDSA_ADD_S_7 5U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_ADD_S_8)
#error CRYPTO_XVIX_XAIX_EDDSA_ADD_S_8 is already defined
#endif
/** \brief Calculate X3 = A * F */
#define CRYPTO_XVIX_XAIX_EDDSA_ADD_S_8 6U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_ADD_S_9)
#error CRYPTO_XVIX_XAIX_EDDSA_ADD_S_9 is already defined
#endif
/** \brief Calculate B */
#define CRYPTO_XVIX_XAIX_EDDSA_ADD_S_9 7U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_ADD_S_10)
#error CRYPTO_XVIX_XAIX_EDDSA_ADD_S_10 is already defined
#endif
/** \brief Calculate final X3 */
#define CRYPTO_XVIX_XAIX_EDDSA_ADD_S_10 8U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_ADD_S_11)
#error CRYPTO_XVIX_XAIX_EDDSA_ADD_S_11 is already defined
#endif
/** \brief Calculate A * G */
#define CRYPTO_XVIX_XAIX_EDDSA_ADD_S_11 9U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_ADD_S_12)
#error CRYPTO_XVIX_XAIX_EDDSA_ADD_S_12 is already defined
#endif
/** \brief Calculate final Y3 */
#define CRYPTO_XVIX_XAIX_EDDSA_ADD_S_12 10U

/*--Crypto_xVIx_xAIx_EdDSAAdditionCDEStateType----------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_EDDSA_ADD_S_1)
#error CRYPTO_XVIX_XAIX_EDDSA_ADD_S_1 is already defined
#endif
/** \brief Calculate X1 * X2 */
#define CRYPTO_XVIX_XAIX_EDDSA_ADD_S_1 0U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_ADD_S_2)
#error CRYPTO_XVIX_XAIX_EDDSA_ADD_S_2 is already defined
#endif
/** \brief Calculate Y1 * Y2 */
#define CRYPTO_XVIX_XAIX_EDDSA_ADD_S_2 1U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_ADD_S_3)
#error CRYPTO_XVIX_XAIX_EDDSA_ADD_S_3 is already defined
#endif
/** \brief Calculate D * X1 * X2 */
#define CRYPTO_XVIX_XAIX_EDDSA_ADD_S_3 2U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_ADD_S_4)
#error CRYPTO_XVIX_XAIX_EDDSA_ADD_S_4 is already defined
#endif
/** \brief Calculate X1 * X2 * D * Y1 * Y2 */
#define CRYPTO_XVIX_XAIX_EDDSA_ADD_S_4 3U

/*--Crypto_xVIx_xAIx_EdDSADoublingStateType-------------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_EDDSA_DOUB_S_START)
#error CRYPTO_XVIX_XAIX_EDDSA_DOUB_S_START is already defined
#endif
/** \brief Start of Point doubling and calculate X1 + Y1 */
#define CRYPTO_XVIX_XAIX_EDDSA_DOUB_S_START 0U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_DOUB_S_1)
#error CRYPTO_XVIX_XAIX_EDDSA_DOUB_S_1 is already defined
#endif
/** \brief Calculate X1 * Y1 */
#define CRYPTO_XVIX_XAIX_EDDSA_DOUB_S_1 1U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_DOUB_S_2)
#error CRYPTO_XVIX_XAIX_EDDSA_DOUB_S_2 is already defined
#endif
/** \brief Calculate Y1 * Y1 */
#define CRYPTO_XVIX_XAIX_EDDSA_DOUB_S_2 2U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_DOUB_S_3)
#error CRYPTO_XVIX_XAIX_EDDSA_DOUB_S_3 is already defined
#endif
/** \brief Calculate F = E + D */
#define CRYPTO_XVIX_XAIX_EDDSA_DOUB_S_3 3U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_DOUB_S_4)
#error CRYPTO_XVIX_XAIX_EDDSA_DOUB_S_4 is already defined
#endif
/** \brief Calculate (X1 + Y1)^2 */
#define CRYPTO_XVIX_XAIX_EDDSA_DOUB_S_4 4U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_DOUB_S_5)
#error CRYPTO_XVIX_XAIX_EDDSA_DOUB_S_5 is already defined
#endif
/** \brief Calculate final Y3 Value */
#define CRYPTO_XVIX_XAIX_EDDSA_DOUB_S_5 5U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_DOUB_S_6)
#error CRYPTO_XVIX_XAIX_EDDSA_DOUB_S_6 is already defined
#endif
/** \brief Calculate J */
#define CRYPTO_XVIX_XAIX_EDDSA_DOUB_S_6 6U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_DOUB_S_7)
#error CRYPTO_XVIX_XAIX_EDDSA_DOUB_S_7 is already defined
#endif
/** \brief Calculate final Z3 Value */
#define CRYPTO_XVIX_XAIX_EDDSA_DOUB_S_7 7U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_DOUB_S_8)
#error CRYPTO_XVIX_XAIX_EDDSA_DOUB_S_8 is already defined
#endif
/** \brief Calculate final X3 Value */
#define CRYPTO_XVIX_XAIX_EDDSA_DOUB_S_8 8U

/*--Crypto_xVIx_xAIx_EdDSAMultiplicationStateType-------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_EDDSA_MULT_S_START)
#error CRYPTO_XVIX_XAIX_EDDSA_MULT_S_START is already defined
#endif
/** \brief Start of multiplication; set the Point to infinity */
#define CRYPTO_XVIX_XAIX_EDDSA_MULT_S_START 0U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_MULT_S_LOOP)
#error CRYPTO_XVIX_XAIX_EDDSA_MULT_S_LOOP is already defined
#endif
/** \brief Verify if the Multiplicand is 0 */
#define CRYPTO_XVIX_XAIX_EDDSA_MULT_S_LOOP 1U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_MULT_S_DOUBLE)
#error CRYPTO_XVIX_XAIX_EDDSA_MULT_S_DOUBLE is already defined
#endif
/** \brief Calculate the double of the Point */
#define CRYPTO_XVIX_XAIX_EDDSA_MULT_S_DOUBLE 2U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_MULT_S_ADD)
#error CRYPTO_XVIX_XAIX_EDDSA_MULT_S_ADD is already defined
#endif
/** \brief Point addition */
#define CRYPTO_XVIX_XAIX_EDDSA_MULT_S_ADD 3U

/*--Crypto_xVIx_xAIx_EdDSAConvertProj2AffStateType------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_EDDSA_CONV_S_START)
#error CRYPTO_XVIX_XAIX_EDDSA_CONV_S_START is already defined
#endif
/** \brief The start of the coordinate conversion.*/
#define CRYPTO_XVIX_XAIX_EDDSA_CONV_S_START 0U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_CONV_S_INVERT)
#error CRYPTO_XVIX_XAIX_EDDSA_CONV_S_INVERT is already defined
#endif
/** \brief Calculate LN inversion. */
#define CRYPTO_XVIX_XAIX_EDDSA_CONV_S_INVERT 1U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_CONV_S_X)
#error CRYPTO_XVIX_XAIX_EDDSA_CONV_S_X is already defined
#endif
/** \brief Calculate X^2. */
#define CRYPTO_XVIX_XAIX_EDDSA_CONV_S_X 2U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_CONV_S_Y)
#error CRYPTO_XVIX_XAIX_EDDSA_CONV_S_Y is already defined
#endif
/** \brief Calculate Y^2. */
#define CRYPTO_XVIX_XAIX_EDDSA_CONV_S_Y 3U

/*--Crypto_xVIx_xAIx_EdDSADecodingStateType-------------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_EDDSA_DEC_S_START)
#error CRYPTO_XVIX_XAIX_EDDSA_DEC_S_START is already defined
#endif
/** \brief Start of Point decoding */
#define CRYPTO_XVIX_XAIX_EDDSA_DEC_S_START 0U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_DEC_S_1)
#error CRYPTO_XVIX_XAIX_EDDSA_DEC_S_1 is already defined
#endif
/** \brief Convert from byte array to big int */
#define CRYPTO_XVIX_XAIX_EDDSA_DEC_S_1 1U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_DEC_S_2)
#error CRYPTO_XVIX_XAIX_EDDSA_DEC_S_2 is already defined
#endif
/** \brief Prepare some of the curve parameters */
#define CRYPTO_XVIX_XAIX_EDDSA_DEC_S_2 2U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_DEC_S_3)
#error CRYPTO_XVIX_XAIX_EDDSA_DEC_S_3 is already defined
#endif
/** \brief Calculate (u / v) ^ ((p + 3) / 8) */
#define CRYPTO_XVIX_XAIX_EDDSA_DEC_S_3 3U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_DEC_S_4)
#error CRYPTO_XVIX_XAIX_EDDSA_DEC_S_4 is already defined
#endif
/** \brief Calculate root candidate */
#define CRYPTO_XVIX_XAIX_EDDSA_DEC_S_4 4U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_DEC_S_5)
#error CRYPTO_XVIX_XAIX_EDDSA_DEC_S_5 is already defined
#endif
/** \brief Calculate Tmp6^2 */
#define CRYPTO_XVIX_XAIX_EDDSA_DEC_S_5 5U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_DEC_S_6)
#error CRYPTO_XVIX_XAIX_EDDSA_DEC_S_6 is already defined
#endif
/** \brief Calculate x = (u/v) ^ ((p+3) / 8) */
#define CRYPTO_XVIX_XAIX_EDDSA_DEC_S_6 6U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_DEC_S_7)
#error CRYPTO_XVIX_XAIX_EDDSA_DEC_S_7 is already defined
#endif
/** \brief Calculate Tmp3 = XValue^2 */
#define CRYPTO_XVIX_XAIX_EDDSA_DEC_S_7 7U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_DEC_S_8)
#error CRYPTO_XVIX_XAIX_EDDSA_DEC_S_8 is already defined
#endif
/** \brief Calculate Tmp3 = v * XValue^2 */
#define CRYPTO_XVIX_XAIX_EDDSA_DEC_S_8 8U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_DEC_S_9)
#error CRYPTO_XVIX_XAIX_EDDSA_DEC_S_9 is already defined
#endif
/** \brief Calculate Tmp5 = Prime - 1 */
#define CRYPTO_XVIX_XAIX_EDDSA_DEC_S_9 9U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_DEC_S_10)
#error CRYPTO_XVIX_XAIX_EDDSA_DEC_S_10 is already defined
#endif
/** \brief Calculate the modulo exponentiation  */
#define CRYPTO_XVIX_XAIX_EDDSA_DEC_S_10 10U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_DEC_S_11)
#error CRYPTO_XVIX_XAIX_EDDSA_DEC_S_11 is already defined
#endif
/** \brief Calculate XValue = XValue * (2^Tmp5) */
#define CRYPTO_XVIX_XAIX_EDDSA_DEC_S_11 11U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_DEC_S_12)
#error CRYPTO_XVIX_XAIX_EDDSA_DEC_S_12 is already defined
#endif
/** \brief Finish the decoding */
#define CRYPTO_XVIX_XAIX_EDDSA_DEC_S_12 12U

/*--Crypto_xVIx_xAIx_EdDSADecodingParamsStateType-------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_EDDSA_DEC_S_INT_2)
#error CRYPTO_XVIX_XAIX_EDDSA_DEC_S_INT_2 is already defined
#endif
/** \brief Calculate (y^2 - 1) */
#define CRYPTO_XVIX_XAIX_EDDSA_DEC_S_INT_2 0U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_DEC_S_P_3)
#error CRYPTO_XVIX_XAIX_EDDSA_DEC_S_P_3 is already defined
#endif
/** \brief Calculate (d * (y^2)) + 1 */
#define CRYPTO_XVIX_XAIX_EDDSA_DEC_S_P_3 1U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_DEC_S_P_4)
#error CRYPTO_XVIX_XAIX_EDDSA_DEC_S_P_4 is already defined
#endif
/** \brief Calculate v^2 */
#define CRYPTO_XVIX_XAIX_EDDSA_DEC_S_P_4 2U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_DEC_S_P_5)
#error CRYPTO_XVIX_XAIX_EDDSA_DEC_S_P_5 is already defined
#endif
/** \brief Calculate v^3 */
#define CRYPTO_XVIX_XAIX_EDDSA_DEC_S_P_5 3U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_DEC_S_P_6)
#error CRYPTO_XVIX_XAIX_EDDSA_DEC_S_P_6 is already defined
#endif
/** \brief Calculate ((y^2) - 1) * v^3 */
#define CRYPTO_XVIX_XAIX_EDDSA_DEC_S_P_6 4U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_DEC_S_P_7)
#error CRYPTO_XVIX_XAIX_EDDSA_DEC_S_P_7 is already defined
#endif
/** \brief Calculate v^7 */
#define CRYPTO_XVIX_XAIX_EDDSA_DEC_S_P_7 5U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_DEC_S_P_8)
#error CRYPTO_XVIX_XAIX_EDDSA_DEC_S_P_8 is already defined
#endif
/** \brief Calculate (u * v^7)^((p - 5) / 8)  */
#define CRYPTO_XVIX_XAIX_EDDSA_DEC_S_P_8 6U

/*--Crypto_xVIx_xAIx_EdDSAEncodingStateType------------------------------------------------------ */

#if (defined CRYPTO_XVIX_XAIX_EDDSA_ENC_S_START)
#error CRYPTO_XVIX_XAIX_EDDSA_ENC_S_START is already defined
#endif
/** \brief Start of Point encoding */
#define CRYPTO_XVIX_XAIX_EDDSA_ENC_S_START 0U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_ENC_S_1)
#error CRYPTO_XVIX_XAIX_EDDSA_ENC_S_1 is already defined
#endif
/** \brief Y Point encoding */
#define CRYPTO_XVIX_XAIX_EDDSA_ENC_S_1 1U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_ENC_S_2)
#error CRYPTO_XVIX_XAIX_EDDSA_ENC_S_2 is already defined
#endif
/** \brief X Point encoding */
#define CRYPTO_XVIX_XAIX_EDDSA_ENC_S_2 2U

/*--Crypto_xVIx_xAIx_EdDSAGenStateType------------------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_IDLE)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_IDLE is already defined
#endif
/** \brief Waiting for new operation */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_IDLE 0U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_INITIALIZED)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_INITIALIZED is already defined
#endif
/** \brief The Signature generation is initialized */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_INITIALIZED 1U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_HASH_PRIV_K)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_HASH_PRIV_K is already defined
#endif
/** \brief Start hashing of the private Key */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_HASH_PRIV_K 2U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_HASH_MESSAGE)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_HASH_MESSAGE is already defined
#endif
/** \brief Start hashing of the Message */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_HASH_MESSAGE 3U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_LITTLE_R)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_LITTLE_R is already defined
#endif
/** \brief Calculate r */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_LITTLE_R 4U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_SCALARR1)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_SCALARR1 is already defined
#endif
/** \brief Convert scalar r to LN */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_SCALARR1 5U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_LITTLE_K)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_LITTLE_K is already defined
#endif
/** \brief Calculate k */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_LITTLE_K 6U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_SIG)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_SIG is already defined
#endif
/** \brief Convert the calculated hash into LN */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_SIG 7U

/*--Crypto_xVIx_xAIx_EdDSAGenScalarsStateType-----------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_SCALARR1_INT)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_SCALARR1_INT is already defined
#endif
/** \brief Convert scalar r to LN */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_SCALARR1_INT 0U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_SCALARR2)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_SCALARR2 is already defined
#endif
/** \brief Calculate r = r % LValue */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_SCALARR2 1U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_SCALARR3)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_SCALARR3 is already defined
#endif
/** \brief Calculate R = r * Base Point B */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_SCALARR3 2U

/*--Crypto_xVIx_xAIx_EdDSAGenFinishSigStateType---------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_S1)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_S1 is already defined
#endif
/** \brief Convert the calculated hash into LN */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_S1 0U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_S2_TEMP)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_S2_TEMP is already defined
#endif
/** \brief Calculate hash % LValue */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_S2_TEMP 1U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_S2)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_SCALARR3 is already defined
#endif
/** \brief Calculate HashResult * Scalar s */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_S2 2U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_EXPORT)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_EXPORT is already defined
#endif
/** \brief Calculate r + (k * s) */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_EXPORT 3U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_EXPORT_TEMP)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_EXPORT_TEMP is already defined
#endif
/** \brief Export the Signature */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_EXPORT_TEMP 4U

/*--Crypto_xVIx_xAIx_EdDSAGenHashMsgStateType-----------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_INIT_HASH_MESSAGE)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_INIT_HASH_MESSAGE is already defined
#endif
/** \brief Start hashing of the Message */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_INIT_HASH_MESSAGE 0U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_INIT_HASH_MESSAGE)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_INIT_HASH_MESSAGE is already defined
#endif
/** \brief Wait for the HashStart callback */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_INIT_HASH_MESSAGE 1U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_HASH_MESSAGE)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_HASH_MESSAGE is already defined
#endif
/** \brief Hash Message blocks */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_HASH_MESSAGE 2U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_HASH_MESSAGE)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_HASH_MESSAGE is already defined
#endif
/** \brief Wait for the HashUpdate callback */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_HASH_MESSAGE 3U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_HASH_MESSAGE)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_HASH_MESSAGE is already defined
#endif
/** \brief Finish the Message hashing */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_HASH_MESSAGE 4U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_FINISH_HASH_MESSAGE)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_FINISH_HASH_MESSAGE is already defined
#endif
/** \brief Wait for the HashFinish callback */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_FINISH_HASH_MESSAGE 5U

/*--Crypto_xVIx_xAIx_EdDSAGenPrivKeyHashStateType-------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_INIT_1)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_INIT_1 is already defined
#endif
/** \brief Start hashing of the private Key */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_INIT_1 0U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_INIT_1)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_INIT_1 is already defined
#endif
/** \brief Wait for the HashStart callback */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_INIT_1 1U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_K)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_K is already defined
#endif
/** \brief Hash the private Key */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_K 2U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_K)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_K is already defined
#endif
/** \brief Wait for the HashUpdate callback */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_K 3U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_HASH_1)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_HASH_1 is already defined
#endif
/** \brief Finish the private Key hashing */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_HASH_1 4U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_FINISH_HASH_1)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_FINISH_HASH_1 is already defined
#endif
/** \brief Wait for the HashFinish callback */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_FINISH_HASH_1 5U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_SCALARS)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_SCALARS is already defined
#endif
/** \brief Calculate the private scalar s */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_SCALARS 6U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_PUBLIC_KEY_1)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_PUBLIC_KEY_1 is already defined
#endif
/** \brief Calculate A = s * Base Point B */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_PUBLIC_KEY_1 7U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_PUBLIC_KEY_2)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_PUBLIC_KEY_2 is already defined
#endif
/** \brief Encode A */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_PUBLIC_KEY_2 8U

/*--Crypto_xVIx_xAIx_EdDSAGenLittleRStateType-----------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_INIT_2)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_INIT_2 is already defined
#endif
/** \brief Start hashing h_b to h_(2*b-1) */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_INIT_2 0U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_INIT_2)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_INIT_2 is already defined
#endif
/** \brief Wait for the HashStart callback */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_INIT_2 1U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_DOM2_1)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_DOM2_1 is already defined
#endif
/** \brief Hash Context string for Ed25519ph */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_DOM2_1 2U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_DOM2_1)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_DOM2_1 is already defined
#endif
/** \brief Wait for the HashUpdate callback */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_DOM2_1 3U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_HB)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_HB is already defined
#endif
/** \brief Hash h_b to h_(2*b-1) */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_HB 4U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_HB)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_HB is already defined
#endif
/** \brief Wait for the HashUpdate callback */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_HB 5U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_M)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_M is already defined
#endif
/** \brief Hash again H(M) */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_M 6U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_M)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_M is already defined
#endif
/** \brief Wait for the HashUpdate callback */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_M 7U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_HASH_2)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_HASH_2 is already defined
#endif
/** \brief Finish hashing, SHA512(Crypto_xVIx_xAIx_Dom||h_b,...,h_(2*b-1)||PH(M))*/
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_HASH_2 8U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_FINISH_HASH_2)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_FINISH_HASH_2 is already defined
#endif
/** \brief Wait for the HashFinish callback */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_FINISH_HASH_2 9U

/*--Crypto_xVIx_xAIx_EdDSAGenLittleKStateType-----------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_INIT_3)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_INIT_3 is already defined
#endif
/** \brief Start hashing of (Crypto_xVIx_xAIx_Dom||R||A||PH(M)) */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_INIT_3 0U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_INIT_3)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_INIT_3 is already defined
#endif
/** \brief Wait for the HashStart callback */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_INIT_3 1U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_DOM2_2)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_DOM2_2 is already defined
#endif
/** \brief Hash Context string for Ed25519ph. */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_DOM2_2 2U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_DOM2_2)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_DOM2_2 is already defined
#endif
/** \brief Wait for the HashUpdate callback */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_DOM2_2 3U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_R)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_R is already defined
#endif
/** \brief Encode R and hash it */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_R 4U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_R)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_R is already defined
#endif
/** \brief Wait for the HashUpdate callback */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_R 5U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_A)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_A is already defined
#endif
/** \brief Hash A */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_A 6U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_A)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_A is already defined
#endif
/** \brief Wait for the HashUpdate callback */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_A 7U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_M2)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_M2 is already defined
#endif
/** \brief Hash PH(M) */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_M2 8U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_M2)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_M2 is already defined
#endif
/** \brief Wait for the HashUpdate callback */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_M2 9U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_HASH_3)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_HASH_3 is already defined
#endif
/** \brief Finish hashing */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_HASH_3 10U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_FINISH_HASH_3)
#error CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_FINISH_HASH_3 is already defined
#endif
/** \brief Wait for the HashFinish callback */
#define CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_FINISH_HASH_3 11U

/*--Crypto_xVIx_xAIx_EdDSAVrfyStateType-----------------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_IDLE)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_IDLE is already defined
#endif
/** \brief Waiting for new operation */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_IDLE 0U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_INITIALIZED)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_INITIALIZED is already defined
#endif
/** \brief The Signature verification is initialized */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_INITIALIZED 1U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_VERIFY_START)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_VERIFY_START is already defined
#endif
/** \brief Start the Signature verification */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_VERIFY_START 2U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_HASH_MSG)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_HASH_MSG is already defined
#endif
/** \brief Hash the Message */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_HASH_MSG 3U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_VALIDATE_SIG_COMPONENTS)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_VALIDATE_SIG_COMPONENTS is already defined
#endif
/** \brief Validate Signature components (R and S) */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_VALIDATE_SIG_COMPONENTS 4U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_LITTLE_K)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_LITTLE_K is already defined
#endif
/** \brief Calculate k */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_LITTLE_K 5U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_GROUP_EQUATIONS)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_GROUP_EQUATIONS is already defined
#endif
/** \brief Calculate group equation */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_GROUP_EQUATIONS 6U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_COMPARE)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_COMPARE is already defined
#endif
/** \brief Compare S * B == R + (k * A) */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_COMPARE 7U

/*--Crypto_xVIx_xAIx_EdDSAVrfyDecodePublicKeyStateType--------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_CONVERT_PUBLICKEY)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_CONVERT_PUBLICKEY is already defined
#endif
/** \brief Convert the public key (A) from Long Number format to byte array */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_CONVERT_PUBLICKEY 0U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_DECODE_PUBLICKEY)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_DECODE_PUBLICKEY is already defined
#endif
/** \brief Decode the public key point */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_DECODE_PUBLICKEY 1U

/*--Crypto_xVIx_xAIx_EdDSAVrfyValSigCmpStateType--------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_CONVERT_S)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_CONVERT_S is already defined
#endif
/** \brief Convert the S component to Long Number format */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_CONVERT_S 0U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_CHECK_S)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_CHECK_S is already defined
#endif
/** \brief Check the range of S */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_CHECK_S 1U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_COPY_R)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_COPY_R is already defined
#endif
/** \brief Copy the R component of the Signature */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_COPY_R 2U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_DECODE_R)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_DECODE_R is already defined
#endif
/** \brief Decode point R */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_DECODE_R 3U

/*--Crypto_xVIx_xAIx_EdDSAVrfyFinishEquationStateType---------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_HASH)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_HASH is already defined
#endif
/** \brief Convert the hash Result to a big integer */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_HASH 0U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_HASH_RED)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_HASH_RED is already defined
#endif
/** \brief Calculate hashValue % LValue */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_HASH_RED 1U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_EQUATION_1)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_EQUATION_1 is already defined
#endif
/** \brief Calculate h * A */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_EQUATION_1 2U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_EQUATION_2)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_EQUATION_2 is already defined
#endif
/** \brief Calculate R + (h * A) */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_EQUATION_2 3U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_EQUATION_2_ENC)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_EQUATION_2_ENC is already defined
#endif
/** \brief Encode R + (h * A) */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_EQUATION_2_ENC 4U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_EQUATION_3)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_EQUATION_3 is already defined
#endif
/** \brief Calculate S * B */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_EQUATION_3 5U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_EQUATION_3_ENC)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_EQUATION_3_ENC is already defined
#endif
/** \brief Encode S * B */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_EQUATION_3_ENC 6U

/*--Crypto_xVIx_xAIx_EdDSAVrfyHashMsgStateType----------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_INIT_HASH_MESSAGE)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_INIT_HASH_MESSAGE is already defined
#endif
/** \brief Start hashing of the Message */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_INIT_HASH_MESSAGE 0U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_INIT_HASH_MESSAGE)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_INIT_HASH_MESSAGE is already defined
#endif
/** \brief Wait for the HashStart callback */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_INIT_HASH_MESSAGE 1U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_UPDATE_MESSAGE)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_UPDATE_MESSAGE is already defined
#endif
/** \brief Hash Message blocks */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_UPDATE_MESSAGE 2U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_UPDATE_MESSAGE)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_UPDATE_MESSAGE is already defined
#endif
/** \brief Wait for the HashUpdate callback */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_UPDATE_MESSAGE 3U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_MESSAGE)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_MESSAGE is already defined
#endif
/** \brief Finish the Message hashing */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_MESSAGE 4U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_FINISH_MESSAGE)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_FINISH_MESSAGE is already defined
#endif
/** \brief Wait for the HashFinish callback */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_FINISH_MESSAGE 5U

/*--Crypto_xVIx_xAIx_EdDSAVrfyLittleKStateType----------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_INIT_SIGN_HASH)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_INIT_SIGN_HASH is already defined
#endif
/** \brief Start the Signature hashing */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_INIT_SIGN_HASH 0U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_INIT)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_INIT is already defined
#endif
/** \brief Wait for the HashStart callback */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_INIT 1U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_UPDATE_DOM2)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_UPDATE_DOM2 is already defined
#endif
/** \brief Hash Context string for Ed25519ph. */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_UPDATE_DOM2 2U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_UPDATE_DOM2)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_UPDATE_DOM2 is already defined
#endif
/** \brief Wait for the HashUpdate callback */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_UPDATE_DOM2 3U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_UPDATE_ENC_R)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_UPDATE_ENC_R is already defined
#endif
/** \brief Hash the R block of the Signature */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_UPDATE_ENC_R 4U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_UPDATE_ENC_R)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_UPDATE_ENC_R is already defined
#endif
/** \brief Wait for the HashUpdate callback */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_UPDATE_ENC_R 5U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_UPDATE_ENC_A)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_UPDATE_ENC_A is already defined
#endif
/** \brief Encode and hash the pubKey A */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_UPDATE_ENC_A 6U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_UPDATE_ENC_A)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_UPDATE_ENC_A is already defined
#endif
/** \brief Wait for the HashUpdate callback */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_UPDATE_ENC_A 7U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_UPDATE_M)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_UPDATE_M is already defined
#endif
/** \brief Hash again the H(M) */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_UPDATE_M 8U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_UPDATE_M)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_UPDATE_M is already defined
#endif
/** \brief Wait for the HashUpdate callback */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_UPDATE_M 9U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH is already defined
#endif
/** \brief Finish the SHA512(Crypto_xVIx_xAIx_Dom||R||A||PH(M)) hashing */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH 10U

#if (defined CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_FINISH)
#error CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_FINISH is already defined
#endif
/** \brief Wait for the HashFinish callback */
#define CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_FINISH 11U

/*==[Types]=======================================================================================*/

/*--EdDSA-----------------------------------------------------------------------------------------*/

/** \brief Internal states of the Point addition */
typedef uint8 Crypto_xVIx_xAIx_EdDSAAdditionStateType;

/** \brief Intermediary internal states for the Point addition */
typedef uint8 Crypto_xVIx_xAIx_EdDSAAdditionCDEStateType;

/** \brief Internal states of the Point doubling */
typedef uint8 Crypto_xVIx_xAIx_EdDSADoublingStateType;

/** \brief Internal state of Point multiplication  */
typedef uint8 Crypto_xVIx_xAIx_EdDSAMultiplicationStateType;

/** \brief Internal states of the coordinate conversion. */
typedef uint8 Crypto_xVIx_xAIx_EdDSAConvertProj2AffStateType;

/** \brief Internal state of decoding */
typedef uint8 Crypto_xVIx_xAIx_EdDSADecodingStateType;

/** \brief Internal state of parameters calculation for Point decoding */
typedef uint8 Crypto_xVIx_xAIx_EdDSADecodingParamsStateType;

/** \brief Internal state of Point encoding */
typedef uint8 Crypto_xVIx_xAIx_EdDSAEncodingStateType;

/** \brief The context of the edwards curve functions.
 *
 * \var Crypto_xVIx_xAIx_EdDSACtxType::ECCtx
 *  \brief The context which is used for elliptic curve arithmetic.
 *
 * \var Crypto_xVIx_xAIx_EdDSACtxType::MuliplicationState
 *  \brief Internal state of the function Crypto_xVIx_xAIx_EdDSAPPointMultiplication()
 *
 * \var Crypto_xVIx_xAIx_EdDSACtxType::ConvertProj2AffState
 *  \brief Internal state of the function Crypto_xVIx_xAIx_EdDSAPPointConvertProj2Aff()
 *
 * \var Crypto_xVIx_xAIx_EdDSACtxType::DecodingParamsState
 *  \brief Intermediary internal state of the function Crypto_xVIx_xAIx_EdDSAPPointDecoding().
 *
 * \var Crypto_xVIx_xAIx_EdDSACtxType::DecodingState
 *  \brief Internal state of the function Crypto_xVIx_xAIx_EdDSAPPointDecoding().
 *
 * \var Crypto_xVIx_xAIx_EdDSACtxType::EncodingState
 *  \brief Internal state of the function Crypto_xVIx_xAIx_EdDSAPPointEncoding().
 *
 * \var Crypto_xVIx_xAIx_EdDSACtxType::AdditionCDEState
 *  \brief Intermediary internal state of the function Crypto_xVIx_xAIx_EdDSAPPointAddition().
 *
 * \var Crypto_xVIx_xAIx_EdDSACtxType::AdditionState
 *  \brief Internal state of the function Crypto_xVIx_xAIx_EdDSAPPointAddition().
 *
 * \var Crypto_xVIx_xAIx_EdDSACtxType::DoublingState
 *  \brief Internal state of the function Crypto_xVIx_xAIx_EdDSAPPointDoubling().
 *
 * \var Crypto_xVIx_xAIx_EdDSACtxType::X0
 *  \brief The least significant bit of the X coordinate.
 *
 */
typedef struct
{
  Crypto_xVIx_xAIx_ECCtxType                     ECCtx;
  Crypto_xVIx_xAIx_EdDSAMultiplicationStateType  MultiplicationState;
  Crypto_xVIx_xAIx_EdDSAConvertProj2AffStateType ConvertProj2AffState;
  Crypto_xVIx_xAIx_EdDSADecodingParamsStateType  DecodingParamsState;
  Crypto_xVIx_xAIx_EdDSADecodingStateType        DecodingState;
  Crypto_xVIx_xAIx_EdDSAEncodingStateType        EncodingState;
  Crypto_xVIx_xAIx_EdDSAAdditionStateType        AdditionCDEState;
  Crypto_xVIx_xAIx_EdDSAAdditionStateType        AdditionState;
  Crypto_xVIx_xAIx_EdDSADoublingStateType        DoublingState;
  uint8                                X0;
}
Crypto_xVIx_xAIx_EdDSACtxType;

/*--EdDSAGen--------------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED == STD_ON)

/** \brief Configuration type for Signature generation.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenConfigType::ObjId
 *  \brief The driver object ID
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenConfigType::ProcessingType
 *  \brief The processing Type
 *
 * * \var Crypto_xVIx_xAIx_EdDSAGenConfigType::Job
 *  \brief The processed Job
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenConfigType::HashCfg
 *  \brief The configuration for the hash primitive
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenConfigType::Crypto_Generic_Callback
 *  \brief The pointer to the callback function
 *
 */
typedef struct
{
  uint32                                                       ObjId;
  Crypto_ProcessingType                                        ProcessingType;
  P2VAR(Crypto_JobType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA)   Job;
  Csm_ConfigIdType                                             HashCfg;
  P2FUNC(void, CRYPTO_xVIx_xAIx_CODE, Crypto_Generic_Callback)
  (
    P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
    uint8                                                        Error
  );
}
Crypto_xVIx_xAIx_EdDSAGenConfigType;

/** \brief State machine for Signature generation */
typedef uint8 Crypto_xVIx_xAIx_EdDSAGenStateType;

/** \brief Internal state of scalar calculation */
typedef uint8 Crypto_xVIx_xAIx_EdDSAGenScalarsStateType;

/** \brief Internal state for the finish of Signature generation */
typedef uint8 Crypto_xVIx_xAIx_EdDSAGenFinishSigStateType;

/** \brief Internal state of the Message hashing */
typedef uint8 Crypto_xVIx_xAIx_EdDSAGenHashMsgStateType;

/** \brief Internal state of the private Key hashing */
typedef uint8 Crypto_xVIx_xAIx_EdDSAGenPrivKeyHashStateType;

/** \brief Internal state of the r calculation */
typedef uint8 Crypto_xVIx_xAIx_EdDSAGenLittleRStateType;

/** \brief Internal state of the k calculation */
typedef uint8 Crypto_xVIx_xAIx_EdDSAGenLittleKStateType;

/** \brief Context for the Signature generation
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::HashResultLength
 *  \brief The length of the hash digest.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::MessageLength
 *  \brief The length of the data whose Signature has to be generated.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::Tmp2ArrayLength
 *  \brief Length of the byte array.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::Tmp1ArrayLength
 *  \brief Length of the byte array.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::Tmp3ArrayLength
 *  \brief Length of the byte array.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::CfgPtr
 *  \brief Pointer to the current EdDSA Signature generation configuration.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::Message
 *  \brief Pointer to Message whose Signature has to be generated.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::Result
 *  \brief Pointer to a variable where the Result of the Signature generation is stored.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::ResultLength
 *  \brief Pointer to a variable where the Result length of the Signature generation is stored.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::EdDSACtx
 *  \brief The context which has to be used for Edwards curve arithmetic.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::R
 *  \brief A Point, which is used to store the Result of R = ScalarR * Base Point.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::A
 *  \brief A Point which is used as a buffer during Signature generation.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::Tmp
 *  \brief The secret which has to be a b-bit string.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::D
 *  \brief An array which is used as a buffer during Signature generation.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::ScalarR
 *  \brief An array which is used to store the scalar R.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::ScalarS
 *  \brief An array which is used to store the scalar S.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::GenState
 *  \brief The current state the EdDSA Signature generation is in.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::GenFinishSig
 *  \brief The current state the EdDSA Signature generation is in.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::ScalarsState
 *  \brief The current state the EdDSA scalar generation is in.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::PrivKeyHashState
 *  \brief The current state the private Key hashing is in.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::MsgHashState
 *  \brief The current state the Message hashing is in.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::LittleRState
 *  \brief The current state the calculation of r is in.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::LittleKState
 *  \brief The current state the calculation of k is in.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::HashResult
 *  brief Pointer to the buffer where the hash Value of the data will be saved.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::Tmp1Array
 *  \brief A byte array which is used as a buffer during Signature generation.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::Tmp2Array
 *  \brief A byte array which is used as a buffer during Signature generation.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::Tmp3Array
 *  \brief A byte array which is used as a buffer during Signature generation.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::HashMessage
 *  \brief Pointer to the buffer where the Message to be hashed is stored.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::MsgHashStarted
 *  \brief Flag to indicate if the hashing of the Message started.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::ErrorCode
 *  \brief The error code which should be returned when the current task finishes.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::ObjId
 *  \brief The driver object ID.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::Job
 *  \brief The processed Job.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::ProcessingType
 *  \brief The processing type.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::Crypto_Generic_Callback
 *  \brief The pointer to the callback function.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::ResultError
 *  \brief The pointer to the error code of the Result.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::IsBusy
 *  \brief The pointer to a variable which indicates if the primitive is Busy.
 *
 * \var Crypto_xVIx_xAIx_EdDSAGenCtxType::CurrentMode
 *  \brief The current operation mode.
 *
 *  \var Crypto_xVIx_xAIx_EdDSAGenCtxType::CancelFlag
 *  \brief This flag indicates if the processing shall be canceled.
 *
 *  \var Crypto_xVIx_xAIx_EdDSAGenCtxType::CallbackFlag
 *  \brief This flag indicates if a callback shall be called at the end of the MainFunction.
 *
 *  \brief Crypto_xVIx_xAIx_EdDSAGenCtxType::CallbackResult
 *  \var The value in this variable will be forwarded to the callback.
 */
typedef struct
{
  uint32                                                   HashResultLength;
  uint32                                                   MessageLength;
  uint32                                                   Tmp2ArrayLength;
  uint32                                                   Tmp1ArrayLength;
  uint32                                                   Tmp3ArrayLength;
  P2CONST(Crypto_xVIx_xAIx_EdDSAGenConfigType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_CONST) CfgPtr;
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_CONST)   Message;
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)      Result;
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)     ResultLength;
  Crypto_xVIx_xAIx_EdDSACtxType                            EdDSACtx;
  Crypto_xVIx_xAIx_ECPointType                             R;
  Crypto_xVIx_xAIx_ECPointType                             A;
  Crypto_xVIx_xAIx_LNWordType                              Tmp[6U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS];
  Crypto_xVIx_xAIx_LNWordType                              ScalarR[CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS];
  Crypto_xVIx_xAIx_LNWordType                              ScalarS[CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS];
  Crypto_xVIx_xAIx_EdDSAGenStateType                       GenState;
  Crypto_xVIx_xAIx_EdDSAGenFinishSigStateType              GenFinishSig;
  Crypto_xVIx_xAIx_EdDSAGenScalarsStateType                ScalarsState;
  Crypto_xVIx_xAIx_EdDSAGenPrivKeyHashStateType            PrivKeyHashState;
  Crypto_xVIx_xAIx_EdDSAGenHashMsgStateType                MsgHashState;
  Crypto_xVIx_xAIx_EdDSAGenLittleRStateType                LittleRState;
  Crypto_xVIx_xAIx_EdDSAGenLittleKStateType                LittleKState;
  uint8                                                    HashResult[CRYPTO_XVIX_XAIX_EDDSA_G_HASH_MAX_LEN];
  uint8                                                    Tmp1Array[CRYPTO_XVIX_XAIX_EDDSA_G_ARRAY_LEN];
  uint8                                                    Tmp2Array[CRYPTO_XVIX_XAIX_EDDSA_G_ARRAY_LEN];
  uint8                                                    Tmp3Array[CRYPTO_XVIX_XAIX_EDDSA_G_ARRAY_LEN];
  uint8                                                    HashMessage[CRYPTO_XVIX_XAIX_EDDSA_G_HASH_MAX_LEN];
  boolean                                                  MsgHashStarted;
  Std_ReturnType                                           ErrorCode;
  uint32                                                   ObjId;
  P2VAR(Crypto_JobType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA) Job;
  boolean                                                  ProcessingType;
  P2FUNC(void, CRYPTO_xVIx_xAIx_CODE, Crypto_Generic_Callback)
  (
    P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)       Job,
    uint8                                                              Error
  );
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  ResultError;
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    IsBusy;
  Crypto_OperationModeType                                 CurrentMode;
  boolean                                                  CancelFlag;
  boolean                                                  CallbackFlag;
  Std_ReturnType                                           CallbackResult;
}
Crypto_xVIx_xAIx_EdDSAGenCtxType;

#endif /* #if (CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED == STD_ON) */

/*--EdDSAVrfy-------------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON)

/** \brief The public Key used for verifying a Signature
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyConfigType::ObjId
 *  \brief The driver object ID.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyConfigType::ProcessingType
 *  \brief The processing type.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyConfigType::Job
 *  \brief The pointer to the current Job.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyConfigType::HashCfg
 *  \brief The configuration of the hash primitive
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyConfigType::Crypto_Generic_Callback
 *  \brief The pointer to the callback function.
 *
 */
typedef struct
{
  uint32                                                       ObjId;
  Crypto_ProcessingType                                        ProcessingType;
  P2VAR(Crypto_JobType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA)   Job;
  Csm_ConfigIdType                                             HashCfg;
  P2FUNC(void, CRYPTO_xVIx_xAIx_CODE, Crypto_Generic_Callback)
  (
    P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
    uint8                                                        Error
  );
}
Crypto_xVIx_xAIx_EdDSAVrfyConfigType;

/** \brief Internal states of Signature verification */
typedef uint8 Crypto_xVIx_xAIx_EdDSAVrfyStateType;

/** \brief Internal states for public key decoding */
typedef uint8 Crypto_xVIx_xAIx_EdDSAVrfyDecodePublicKeyStateType;

/** \brief Internal states for public key decoding */
typedef uint8 Crypto_xVIx_xAIx_EdDSAVrfyValSigCmpStateType;

/** \brief Internal states of Signature verification final equation */
typedef uint8 Crypto_xVIx_xAIx_EdDSAVrfyFinishEquationStateType;

/** \brief Internal states for Message hashing */
typedef uint8 Crypto_xVIx_xAIx_EdDSAVrfyHashMsgStateType;

/** \brief Internal state for little k calculation */
typedef uint8 Crypto_xVIx_xAIx_EdDSAVrfyLittleKStateType;

/** \brief Structure which contains the context of the EdDSA Signature verification.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::MessageLength
 *  \brief The length of the data whose Signature has to be verified.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::SignatureLength
 *  \brief Length of the Signature to verify.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::HashResultLength
 *  \brief The length of the hash digest.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::Tmp1ArrayLength
 *  \brief Length of the byte array.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::Tmp2ArrayLength
 *  \brief Length of the byte array.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::Tmp3ArrayLength
 *  \brief Length of the byte array.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::CfgPtr
 *  \brief Pointer to the current EdDSA Signature verification configuration.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::Message
 *  \brief Pointer to Message whose Signature has to be verified.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::Signature
 *  \brief Pointer to the buffer where the created Signature should be stored.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::ResultPtr
 *  \brief Pointer to a variable where the Result of the Signature verification is stored.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::EdDSACtx
 *  \brief The context which has to be used for Edwards curve arithmetic.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::Tmp1Point
 *  \brief A Point which is used as a buffer during Signature verification.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::Tmp2Point
 *  \brief A Point which is used as a buffer during Signature verification.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::Tmp3Point
 *  \brief A Point which is used as a buffer during Signature verification.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::Tmp4Point
 *  \brief A Point which is used as a buffer during Signature verification.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::Tmp
 *  \brief An array which is used as a buffer during Signature verification.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::HashResult
 *  \brief Pointer to the buffer where the hash Value of the data will be saved.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::HashMessage
 *  \brief Pointer to the buffer where the Message to be hashed is stored.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::Tmp1Array
 *  \brief A byte array which is used as a buffer during Signature verification.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::Tmp2Array
 *  \brief A byte array which is used as a buffer during Signature verification.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::Tmp3Array
 *  \brief A byte array which is used as a buffer during Signature verification.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::VrfyState
 *  \brief The current state of the EdDSA verification.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::DecPubKeyState
 *  \brief Internal states for public key decoding.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::ValSigCmpState
 *  \brief Internal states for Signature components (R and S) validation.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::FinishEquationState
 *  \brief Internal states of Signature verification final equation
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::HashMsgState
 *  \brief Internal states for Message hashing
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::LittleKState
 *  \brief Internal state for little k calculation.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::MsgHashStarted
 *  \brief Flag to indicate if the hashing of the Message started.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::ErrorCode
 *  \brief The error code which should be returned when the current task finishes.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::ObjId
 *  \brief The driver object ID.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::ProcessingType
 *  \brief The current processing type.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::Crypto_Generic_Callback
 *  \brief The pointer to the callback function.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::IsBusy
 *  \brief The pointer for the IsBusy variable when calling Main synchron.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::ResultError
 *  \brief The pointer for the error when calling Main synchron.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::Job
 *  \brief The pointer to the current Job.
 *
 * \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::CurrentMode
 *  \brief The current mode.
 *
 *  \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::CancelFlag
 *  \brief This flag indicates if the processing shall be canceled.
 *
 *  \var Crypto_xVIx_xAIx_EdDSAVrfyCtxType::CallbackFlag
 *  \brief This flag indicates if a callback shall be called at the end of the MainFunction.
 *
 *  \brief Crypto_xVIx_xAIx_EdDSAVrfyCtxType::CallbackResult
 *  \var The value in this variable will be forwarded to the callback.
 */
typedef struct
{
  uint32                                                       MessageLength;
  uint32                                                       SignatureLength;
  uint32                                                       HashResultLength;
  uint32                                                       Tmp1ArrayLength;
  uint32                                                       Tmp2ArrayLength;
  uint32                                                       Tmp3ArrayLength;
  P2CONST(Crypto_xVIx_xAIx_EdDSAVrfyConfigType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_CONST) CfgPtr;
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_CONST)                 Message;
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_CONST)                 Signature;
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  ResultPtr;
  Crypto_xVIx_xAIx_EdDSACtxType                                EdDSACtx;
  Crypto_xVIx_xAIx_ECPointType                                 Tmp1Point;
  Crypto_xVIx_xAIx_ECPointType                                 Tmp2Point;
  Crypto_xVIx_xAIx_ECPointType                                 Tmp3Point;
  Crypto_xVIx_xAIx_ECPointType                                 Tmp4Point;
  Crypto_xVIx_xAIx_LNWordType                                  Tmp[8U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS];
  uint8                                                        HashResult[CRYPTO_XVIX_XAIX_EDDSA_V_HASH_MAX_LEN];
  uint8                                                        HashMessage[CRYPTO_XVIX_XAIX_EDDSA_V_HASH_MAX_LEN];
  uint8                                                        Tmp1Array[CRYPTO_XVIX_XAIX_EDDSA_V_ARRAY_LEN];
  uint8                                                        Tmp2Array[CRYPTO_XVIX_XAIX_EDDSA_V_ARRAY_LEN];
  uint8                                                        Tmp3Array[CRYPTO_XVIX_XAIX_EDDSA_V_ARRAY_LEN];
  Crypto_xVIx_xAIx_EdDSAVrfyStateType                          VrfyState;
  Crypto_xVIx_xAIx_EdDSAVrfyDecodePublicKeyStateType           DecPubKeyState;
  Crypto_xVIx_xAIx_EdDSAVrfyValSigCmpStateType                 ValSigCmpState;
  Crypto_xVIx_xAIx_EdDSAVrfyFinishEquationStateType            FinishEquationState;
  Crypto_xVIx_xAIx_EdDSAVrfyHashMsgStateType                   HashMsgState;
  Crypto_xVIx_xAIx_EdDSAVrfyLittleKStateType                   LittleKState;
  boolean                                                      MsgHashStarted;
  Std_ReturnType                                               ErrorCode;
  uint32                                                       ObjId;
  boolean                                                      ProcessingType;
  P2FUNC(void, CRYPTO_xVIx_xAIx_CODE, Crypto_Generic_Callback)
  (
    P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
    uint8                                                      Error
  );
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        IsBusy;
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultError;
  P2VAR(Crypto_JobType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA)   Job;
  Crypto_OperationModeType                                     CurrentMode;
  boolean                                                      CancelFlag;
  boolean                                                      CallbackFlag;
  Std_ReturnType                                               CallbackResult;
}
Crypto_xVIx_xAIx_EdDSAVrfyCtxType;

#endif /* #if (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON) */

/*==[Declaration of functions with internal linkage]==============================================*/

/*--EdDSA-----------------------------------------------------------------------------------------*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief Check if either of the input points are at infinity.
 *
 * \param[out] Result   Pointer to an edwards curve point where the result of the
 *                      addition will be stored in projective coordinates.
 * \param[in]  Addend1  Pointer to an edwards curve point given in projective coordinates
 *                      which should be added. This parameter may Point to the same memory location
 *                      as the result point "Result".
 * \param[in]  Addend2  Pointer to an edwards curve point given in projective coordinates
 *                      which should be added. This parameter may point to the same memory location
 *                      as the result point "Result".
 * \param[out] Finished Pointer to a flag that indicates if the operation is finished.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointAdditionCheckInfinity
(
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Result,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend1,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend2,
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                        Finished
);

/** \brief Calculate A^2
 *
 * \param[in,out] CtxPtr Pointer to a context which holds the current state of the addition.
 * \param[in,out] Tmp    Pointer to a buffer which will be used as temporary storage.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointAdditionCalculateA2
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp
);

/** \brief Calculate Z^3
 *
 * \param[in,out] CtxPtr  Pointer to a context which holds the current state of the addition.
 * \param[out]    Result  Pointer to an edwards curve point where the result of the
 *                        addition will be stored in projective coordinates.
 * \param[in]     Addend1 Pointer to an edwards curve point given in projective coordinates
 *                        which should be added. This parameter may point to the same memory location
 *                        as the result point "Result".
 * \param[in,out] Tmp     Pointer to a buffer which will be used as temporary storage.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointAdditionCalculateZ3
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Result,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend1,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    Tmp
);

/** \brief Calculate A * G
 *
 * \param[in,out] CtxPtr Pointer to a context which holds the current state of the addition.
 * \param[in,out] Tmp    Pointer to a buffer which will be used as temporary storage.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointAdditionCalculateAG
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp
);

/** \brief Calculate temporary point
 *
 * \param[in,out] CtxPtr Pointer to a context which holds the current state of the addition.
 * \param[in,out] Tmp    Pointer to a buffer which will be used as temporary storage.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointAdditionCalculateInterimTmp5
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp
);

/** \brief Calculate A * F
 *
 * \param[in,out] CtxPtr  Pointer to a context which holds the current state of the addition.
 * \param[out]    Result  Pointer to an edwards curve point where the result of the
 *                        addition will be stored in projective coordinates.
 * \param[in]     Addend2 Pointer to an edwards curve point given in projective coordinates
 *                        which should be added. This parameter may point to the same memory location
 *                        as the result point "Result".
 * \param[in,out] Tmp     Pointer to a buffer which will be used as temporary storage.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointAdditionCalculateAF
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Result,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend2,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    Tmp
);

/** \brief Calculate X3
 *
 * \param[in,out] CtxPtr Pointer to a context which holds the current state of the addition.
 * \param[out]    Result Pointer to an edwards curve point where the result of the
 *                       addition will be stored in projective coordinates.
 * \param[in,out] Tmp    Pointer to a buffer which will be used as temporary storage.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointAdditionCalculateFinalX3
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Result,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp
);

/** \brief Calculate Z3
 *
 * \param[in,out] CtxPtr Pointer to a context which holds the current state of the doubling.
 * \param[out]    Result Pointer to an edwards curve point where the result of the
 *                       doubling will be stored in projective coordinates.
 * \param[in,out] Tmp    Pointer to a buffer which will be used as temporary storage.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointDoublingCalculateZ3
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Result,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp
);

/** \brief Calculate H
 *
 * \param[out]    Value  Pointer to an edwards curve point where the result of the
 *                       doubling will be stored in projective coordinates.
 * \param[in,out] Tmp    Pointer to a buffer which will be used as temporary storage.
 * \param[in,out] CtxPtr Pointer to a context which holds the current state of the doubling.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointDoublingCalculateH
(
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Value,
  P2VAR(Crypto_xVIx_xAIx_LNWordType,    AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Tmp,
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  CtxPtr
);

/** \brief Calculate B
 *
 * \param[in,out] Tmp    Pointer to a buffer which will be used as temporary storage.
 * \param[in,out] CtxPtr Pointer to a context which holds the current state of the doubling.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointDoublingCalculateB
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Calculate E and F
 *
 * \param[in,out] CtxPtr Pointer to a context which holds the current state of the doubling.
 * \param[in,out] Tmp    Pointer to a buffer which will be used as temporary storage.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointDoublingCalculateEF
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp
);

/** \brief Calculate C, D and E
 *
 * \param[in,out] CtxPtr  Pointer to a context which holds the current state of the addition.
 * \param[in]     Addend1 Pointer to an edwards curve point given in projective coordinates
 *                        which should be added.
 * \param[in]     Addend2 Pointer to an edwards curve point given in projective coordinates
 *                        which should be added.
 * \param[in,out] Tmp     Pointer to a buffer which will be used as temporary storage.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointAdditionCalculateCDE
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  CtxPtr,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend1,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend2,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    Tmp
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
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAProjectivePointOfInfinity
(
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Point
);

/** \brief Point doubling
 *
 *         This function doubles a point on an edwards curve which is given in
 *         projective coordinates and returns the result in projective coordinates.
 *         The doubling is performed in slices and the current slice is stored in the given context.
 *
 * \param[in,out] CtxPtr  Pointer to a context which holds the current state of the
 *                        doubling. Before the first call of this function, the context has to
 *                        be initialized by a call to Crypto_xVIx_xAIx_EdDSAPointDoublingStart().
 * \param[out]    Result  Pointer to an edwards curve point where the result of the
 *                        doubling will be stored in projective coordinates.
 * \param[in]     Value   Pointer to an edwards curve point given in projective coordinates
 *                        which should be doubled.
 * \param[in,out] Tmp     Pointer to a buffer which will be used as temporary storage
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointDoubling
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Result,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    Tmp
);

/** \brief Start the encoding.
 *
 *         This function modifies the given context such that, when the function
 *         Crypto_xVIx_xAIx_EdDSAPointEncoding() is called with that context,
 *         it will perform a Point encoding.
 *
 * \param[in,out] CtxPtr Pointer to a context which will be initialized such that
 *                       the function Crypto_xVIx_xAIx_EdDSAPointEncoding() can be called with it.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointEncodingStart
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief This function encodes a Point.
 *
 * \param[in,out] CtxPtr          Pointer to a context which holds the current state of the encoding.
 * \param[in]     Value           Pointer to the point to be encoded.
 * \param[in,out] Tmp             Pointer to a buffer which is used as temporary storage for the calculation.
 * \param[out]    Result          Pointer to an edwards curve point where the result of the
 *                                encoding will be stored in projective coordinates.
 * \param[in,out] Tmp1Array       Pointer to a byte-array which is used as a buffer during Signature generation.
 * \param[in]     Tmp1ArrayLength The length of the byte-array.
 * \param[in]     ResultLength    Length of the Result.
 *
 * \returns If encoding has finished
 * \retval FALSE  The encoding has not yet finished. It has to be called again with the same parameters.
 * \retval TRUE   The encoding has Finished. Now, "Result" contains the result of the encoding.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointEncoding
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    Value,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)     Tmp,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                           Result,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                           Tmp1Array,
  uint32                                                                        Tmp1ArrayLength,
  uint32                                                                        ResultLength
);

/** \brief Point addition
 *
 *         This function adds two points on an elliptic curve which are given in
 *         projective coordinates and returns the result again in
 *         projective coordinates. The addition is performed in slices and the
 *         current slice is stored in the given context.
 *
 * \param[in,out] CtxPtr  Pointer to a context which holds the current state of the
 *                        addition. Before the first call of this function, the context has to
 *                        be initialized by a call to Crypto_xVIx_xAIx_EdDSAPointAdditionStart().
 * \param[out]    Result  Pointer to an edwards curve point where the Result of the
 *                        addition will be stored in projective coordinates.
 * \param[in]     Addend1 Pointer to an edwards curve point given in projective coordinates
 *                        which should be added. This parameter may Point to the same memory location
 *                        as the result point "Result".
 * \param[in]     Addend2 Pointer to an edwards curve point given in projective coordinates
 *                        which should be added. This parameter may point to the same memory location
 *                        as the result point "Result".
 * \param[in,out] Tmp     Pointer to a buffer which is used as temporary storage for the calculation.
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointAddition
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Result,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend1,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend2,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    Tmp
);

/** \brief Point multiplication.
 *
 *         This function multiplies a point on an edwards curve which is given in
 *         projective coordinates with an integer and returns the result again in
 *         projective coordinates. The multiplication is performed
 *         in slices and the current slice is stored in the given context.
 *
 * \param[in,out] CtxPtr       Pointer to a context which holds the current state of the
 *                             multiplication. Before the first call of this function, the context has
 *                             to be initialized by a call to Crypto_xVIx_xAIx_EdDSAPointMultiplicationStart().
 * \param[out]    Result       Pointer to an edwards curve point where the result of the
 *                             multiplication will be stored in projective coordinates.
 * \param[in]     Point        Pointer to an edwards curve point given in projective coordinates
 *                             which should multiplied. This parameter must not point to the same
 *                             memory location as the result "Result".
 * \param[in]     Multiplicand Pointer to an edwards curve point with which the point should be multiplied.
 * \param[in,out] Tmp          Pointer to a buffer which is used as temporary storage for the calculation.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointMultiplication
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Result,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Point,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Multiplicand,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    Tmp
);

/** \brief Start the point conversion.
 *
 *         This function modifies the given context such that, when the function
 *         Crypto_xVIx_xAIx_EdDSAPointConvertProj2Aff() is called with that context, it will perform
 *         a Point conversion.
 *
 * \param[in,out] CtxPtr Pointer to a context which will be initialized such that
 *                       the function Crypto_xVIx_xAIx_EdDSAPointConvertProj2Aff() can be called with it.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointConvertProj2AffStart
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Point conversion.
 *
 *         This function converts a point given in projective coordinates to the
 *         same point in affine coordinates. The conversion is performed
 *         in slices and the current slice is stored in the given context.
 *
 * \param[in,out] CtxPtr Pointer to a context which holds the current state of the conversion.
 * \param[in,out] Point  Pointer to an edwards curve point given in projective coordinates
 *                       which should converted.
 * \param[in,out] Tmp1   Pointer to a buffer which is used as temporary storage during the conversion.
 * \param[in,out] Tmp2   Pointer to a buffer which is used as temporary storage during the conversion.
 * \param[in,out] Tmp3   Pointer to a buffer which is used as temporary storage during the conversion.
 *
 * \returns If the conversion has finished
 * \retval FALSE  The conversion has not yet finished. It has to be called again with
 *                the same parameters.
 * \retval TRUE   The conversion has finished. Now, "Point" contains the result of the conversion.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointConvertProj2Aff
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Point,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp1,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp2,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp3
);

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*--EdDSAGen--------------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED == STD_ON)

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/**  \brief  Crypto EdDSA Generate Reset
 *
 *           This function resets the context of EdDSA Generate.
 *
 * \returns  void
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ED25519_NOT_SET_Reset
(
  void
);

/** \brief Calculate private scalar for an Edwards-curve
 *
 *         This function is implemented to calculate the secret scalar for an Edwards-curve.
 *         scalar = 2^n + sum(2^i * h_i) for c <= i < n
 *
 * \param[in,out] CtxPtr         Pointer to the edwards curve context which is used to store the current state.
 * \param[out]    Result         Pointer to an edwards curve point where the result will be stored.
 * \param[in]     HashPtr        Pointer to the hash result.
 * \param[in]     HashPtrLength  Pointer to the hash length.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPrivateScalar
(
  uint8                                                                     CurveIndex,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                       HashPtr,
  uint32                                                                    HashPtrLength
);

/** \brief Process the private Key.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAProcessPrivKey
(
  void
);

/** \brief Hash the Message.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAGenHashMsg
(
  void
);

/** \brief Calculate little r.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSACalculateLittleR
(
  void
);

/** \brief Calculate little k.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAGenCalculateLittleK
(
  void
);

/** \brief Calculate little k state helper function to perform the processing
 *         of the CRYPTO_XVIX_XAIX_EDDSA_G_S_INIT_3 state.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAGenCalculateLittleK_Init3
(
  void
);

/** \brief Calculate little k state helper function to perform the processing
 *         of the CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_DOM2_2 state.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAGenCalculateLittleK_UpdateDom2
(
  void
);

/** \brief Calculate little k state helper function to perform the processing
 *         of the CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_A state.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAGenCalculateLittleK_UpdateA
(
  void
);

/** \brief Calculate little k state helper function to perform the processing
 *         of the CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_M2 state.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAGenCalculateLittleK_UpdateM2
(
  void
);

/** \brief Calculate little k state helper function to perform the processing
 *         of the CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_HASH_3 state.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAGenCalculateLittleK_FinishHash
(
  void
);

/** \brief This is a default state helper function.  This functions takes care of the default state
 *         execution where many states have the same processing.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAGenCalculateLittleK_DefaultState
(
  void
);
/** \brief Convert R to a byte array and encode it for the HashUpdate function.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAEncodeAndHashR
(
  void
);

/** \brief Calculate the scalars.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSACalculateScalars
(
  void
);

/** \brief Finish the Signature generation.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAFinishSignature
(
  void
);

/** \brief Callback for the hash APIs in the k calculation.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSALittleKCallback
(
  void
);

/** \brief Callback for the hash APIs in the r calculation.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSALittleRCallback
(
  void
);

/** \brief Callback for the hash APIs in the Message hashing.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAHashMsgCallback
(
  void
);

/** \brief Callback for the hash APIs in the private Key hashing.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPrivKeyCallback
(
  void
);

/** \brief Callback function for EdDSA generation.
 *
 * \param[in] Job     A pointer to the Job
 * \param[in] Result  The Result of the underlying cryptographic service.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAGenCallback
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  Std_ReturnType                                   Result
);

/** \brief Prepare the final Signature.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAExportSign
(
  void
);

/** \brief Prepare S from digest.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAEncodeSFromHash
(
  void
);

/** \brief Start Signature generation.
 *
 *         This function requests the start of the EdDSA Signature
 *         generation for the given configuration and Key.
 *         The start is performed in Crypto_xVIx_xAIx_EdDSAGenMainFunction().
 *
 * \param[in] CfgPtr         A pointer to the configuration for which the start of the
 *                           Signature generation should be requested.
 * \param[in] PrivateKeyPtr  A pointer to the Key which should be used in the Signature generation.
 *
 *  \returns Error Value.
 *  \retval  E_OK           If the service can be started.
 *  \retval  E_NOT_OK       If the Key is invalid.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAGenStart
(
  P2CONST(void, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                             CfgPtr,
  P2CONST(Crypto_xVIx_xAIx_AsymPrivateKeyType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PrivateKeyPtr
);

/** \brief Update Signature generation.
 *
 *         This function requests the update of the EdDSA Signature generation for the given data.
 *         The update is performed in Crypto_xVIx_xAIx_EdDSAGenMainFunction().
 *
 * \param[in] DataPtr     A pointer to the start of an array which contains data or
 *                        a part of the data for which the Signature should be created.
 * \param[in] DataLength  The amount of bytes of data.
 *
 *  \returns Error Value.
 *  \retval  E_OK           If the update was successfully requested.
 *  \retval  E_NOT_OK       If no Signature generation has been started via Crypto_xVIx_xAIx_EdDSAGenStart().
 *  \retval  CRYPTO_E_BUSY  If the main function is processing a requested service at the moment.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAGenUpdate
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) DataPtr,
  uint32                                                DataLength
);

/** \brief Finish Signature generation.
 *
 *         This function requests the finishing of the EdDSA Signature generation and the storing
 *         of the Signature in the given Result buffer.
 *         The finishing is performed in Crypto_xVIx_xAIx_EdDSAGenMainFunction().
 *
 * \param[out] ResultPtr        A pointer to the start of a buffer which holds the Signature.
 * \param[out] ResultLengthPtr  Length of the Signature in bytes.
 *
 *  \returns Error Value.
 *  \retval E_OK           If the finish was successfully requested.
 *  \retval E_NOT_OK       If no Signature generation has been started via Crypto_xVIx_xAIx_EdDSAGenStart().
 *  \retval CRYPTO_E_BUSY  If the main function is processing a requested service at the moment.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAGenFinish
(
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  ResultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultLengthPtr
);

/** \brief Perform the EdDSAGen tasks.
 *
 *         This function performs the actual EdDSA Signature generation tasks which have been
 *         requested by the service functions. When a task has Finished, the function
 *         Csm_SignatureGenerateCallbackNotification() is called to inform the CSM of the Result.
 *         If the complete Signature generation has Finished, additionally the function
 *         Csm_SignatureGenerateServiceFinishNotification() is called.
 *
 * \param[out] Result  A pointer to where to store the Result of the function call
 * \param[out] Busy    A pointer to where to store if the functio is Busy or idle
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAGenMainFunction
(
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
);

/** \brief AL Callback for other primitives
 *
 *         This function is a callbackfunction for other primitives
 *
 * \param[in,out] Job  A pointer to the current Job
 * \param[in] Result   The Result of the process
 *
 * \Reentrancy Nonreentrant
 * \Synchronicity Synchronous/Asynchronous
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATURE_GENERATE_ED25519_NOT_SET_CallBack
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint8                                                        Result
);

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if (CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED == STD_ON) */

/*--EdDSAVrfy-------------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON)

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/**  \brief  Crypto EdDSA Verify Reset
 *
 *           This Function resets the Context of EdDSA Verify.
 *
 * \returns  void
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ED25519_NOT_SET_Reset
(
  void
);

/** \brief Decode the public key.
  *
  * \Reentrancy{Nonreentrant}
  * \Synchronicity{Synchronous/Asynchronous}
  *
  */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAVrfyDecodePublicKey
(
  void
);

/** \brief Hash the message.
  *
  * \Reentrancy{Nonreentrant}
  * \Synchronicity{Synchronous/Asynchronous}
  *
  */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAVrfyHashMsg
(
  void
);

/** \brief Check the validity of the R and S componets of the Signature.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAVrfyValSigCmp
(
  void
);

/** \brief Calculate little k.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAVrfyCalculateLittleK
(
  void
);

/** \brief little k state helper function for the processing of
 *         CRYPTO_XVIX_XAIX_EDDSA_V_S_INIT_SIGN_HASH state.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAVrfyCalculateLittleK_InitSignHash
(
  void
);

/** \brief little k state helper function for the processing of
 *         CRYPTO_XVIX_XAIX_EDDSA_V_S_UPDATE_DOM2 state.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAVrfyCalculateLittleK_UpdateDom2
(
  void
);

/** \brief little k state helper function for the processing of
 *         CRYPTO_XVIX_XAIX_EDDSA_V_S_UPDATE_ENC_R state.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAVrfyCalculateLittleK_UpdateEncR
(
  void
);

/** \brief little k state helper function for the processing of
 *         CRYPTO_XVIX_XAIX_EDDSA_V_S_UPDATE_ENC_A state.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAVrfyCalculateLittleK_UpdateEncA
(
  void
);

/** \brief little k state helper function for the processing of
 *         CRYPTO_XVIX_XAIX_EDDSA_V_S_UPDATE_M state.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAVrfyCalculateLittleK_UpdateM
(
  void
);

/** \brief little k state helper function for the processing of
 *         CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH state.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAVrfyCalculateLittleK_Finish
(
  void
);

/** \brief This is a default state helper function.  This functions takes care of the default state
 *         execution where many states have the same processing.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAVrfyCalculateLittleK_DeafultState
(
  void
);

/** \brief Finish the Signature verification Signature.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAFinishEquation
(
  void
);

/** \brief Calculate the s * Base Point.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSACalculateSB
(
  void
);

/** \brief Calculate the k * A.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSACalculateKA
(
  void
);

/** \brief Convert the hash digest to LN representation.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAHashByteToNum
(
  void
);

/** \brief Calculate R + (k * A).
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSACalculateRkA
(
  void
);

/** \brief Validate the computed Signature.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAValidateSignature
(
  void
);

/** \brief Calculate X^2 and do the fast reduction
 *
 * \param[in,out] CtxPtr           Pointer to a context which holds the current state.
 * \param[in]     Base             Pointer to the base which should be exponentiated.
 * \param[in]     CheckHighestBit  Pointer a to buffer on which the highest bit will be checked.
 * \param[in,out] ValueToDouble    Pointer a to buffer that will be doubled.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAStatePowerAndReduce
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Base,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CheckHighestBit,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   ValueToDouble
);

/** \brief Calculate Result of (u * v^7) ^ ((p - 5) / 8)
 *
 * \param[in,out] CtxPtr Pointer to a context which holds the current state.
 * \param[in]     Base   Pointer to the base which should be exponentiated.
 * \param[out]    Result Pointer to the location where the Result is stored.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAStateCalculateEQ1
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Base,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Result
);

/** \brief Calculate x = (u/v) ^ ((p + 3) / 8)
 *
 *         This can be done using the following equation:
 *         (u * v^3) * (u * v^7) ^ ((p - 5) / 8)
 *
 * \param[in,out] CtxPtr        Pointer to a context which holds the current state.
 * \param[out]    Point         Pointer to an edwards curve point where the result will be stored.
 * \param[in]     Multiplicand  Pointer to the result of (u * v^3).
 * \param[in]     Multiplicator Pointer to the result of (u * v^7) ^ ((p - 5) / 8).
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAStateCalculateEQ2
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Point,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Multiplicand,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Multiplicator
);

/** \brief Calculate the square of XValue
 *
 * \param[in,out] CtxPtr    Pointer to a context which holds the current state.
 * \param[out]    Point     Pointer to an edwards curve point where the result will be stored.
 * \param[in]     SquareOfX Pointer to the result of the squaring of XValue.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAStateCalculateEQ3
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Point,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   SquareOfX
);

/** \brief Select the correct Result for calculate the square root of XValue
 *
 * \param[in,out] CtxPtr    Pointer to a context which holds the current state.
 * \param[in,out] Tmp       Pointer to a buffer which will be used as temporary storage.
 * \param[out]    Result    Pointer to an edwards curve point where the result will be stored.
 * \param[out]    ResultPtr Pointer to an edwards curve point where the result will be stored.
 * \param[out]    Finished  Pointer to a flag that indicates if the operation is finished.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAStateCalculateEQ4
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    Tmp,
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Result,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        ResultPtr,
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                        Finished
);

/** \brief Calculate 2^((p - 1) / 4)
 *
 * \param[in,out] CtxPtr   Pointer to a context which holds the current state.
 * \param[out]    Point    Pointer to an edwards curve point where the result will be stored.
 * \param[in]     Base     Pointer to the base which should be exponentiated.
 * \param[in]     Exponent Pointer to the location where the result of ((p - 1) / 4) is stored.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAStateCalculateEQ5
(
  uint8                                                             CurveIndex,
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Point,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Base,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Exponent
);

/** \brief Calculate Tmp5 = Prime - 1
 *
 * \param[in,out] CtxPtr Pointer to a context which holds the current state.
 * \param[out]    Result Pointer to an edwards curve point where the result will be stored.
 * \param[in]     Tmp    Pointer to a buffer which will be used as temporary storage.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPrepareCurveParams
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  CtxPtr,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    Tmp
);

/** \brief Calculate Y ^ 2
 *
 * \param[out]    Result Pointer to an edwards curve point where the result of the
 *                       decoding will be stored in projective coordinates.
 * \param[in,out] Tmp    Pointer to a buffer which will be used as temporary storage.
 * \param[in,out] CtxPtr Pointer to a context which holds the current state of the doubling.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPrepareCurveParamsY2
(
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    Tmp,
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  CtxPtr
);

/** \brief Calculate D * (Y ^ 2)
 *
 * \param[in,out] CtxPtr Pointer to a context which holds the current state of the decoding.
 * \param[out]    Result Pointer to an edwards curve point where the result of the
 *                       decoding will be stored in projective coordinates.
 * \param[in,out] Tmp    Pointer to a buffer which will be used as temporary storage.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPrepareCurveParamsDY2
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  CtxPtr,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    Tmp
);

/** \brief Calculate V ^ 2
 *
 * \param[in,out] Tmp    Pointer to a buffer which will be used as temporary storage.
 * \param[in,out] CtxPtr Pointer to a context which holds the current state of the doubling.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPrepareCurveParamsV2
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp,
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Calculate V ^ 3
 *
 * \param[in,out] CtxPtr Pointer to a context which holds the current state of the decoding.
 * \param[in,out] Tmp    Pointer to a buffer which will be used as temporary storage.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPrepareCurveParamsV3
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp
);

/** \brief Calculate XValue * (2^Tmp5)
 *
 * \param[in,out] CtxPtr Pointer to a context which holds the current state of the decoding.
 * \param[out]    Result Pointer to an edwards curve point where the result of the
 *                       decoding will be stored in projective coordinates.
 * \param[in,out] Tmp    Pointer to a buffer which will be used as temporary storage.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointDecodingXValue2
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Result,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp
);

/** \brief Calculate the exponentiation
 *
 * \param[in,out] CtxPtr Pointer to a context which holds the current state of the decoding.
 * \param[in,out] Tmp    Pointer to a buffer which will be used as temporary storage.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointDecodingExp
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp
);

/** \brief Do the state management for the point decoding
 *
 * \param[in] CtxPtr Pointer to a context which holds the current state of the decoding.
 * \param[in] Tmp    Pointer to a buffer which will be used as temporary storage.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointDecodingStateMngmt
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp
);

/** \brief Start point decoding
 *
 *         This function modifies the given context such that, when the function
 *         Crypto_xVIx_xAIx_EdDSAPointDecoding() is called with that context, it will perform
 *         a point decoding.
 *
 * \param[in,out] CtxPtr Pointer to a context which will be initialized such that
 *                       the function Crypto_xVIx_xAIx_EdDSAPointDecoding() can be called with it.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointDecodingStart
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Point decoding.
 *
 *         This function decode a given byte-array in a Point an edwards curve in which
 *         the Result is in projective coordinates.
 *
 * \param[in,out] CtxPtr       Pointer to a context which holds the current state of the decoding.
 * \param[out]    Result       Pointer to an edwards curve point where the Result of the
 *                             decoding will be stored in projective coordinates.
 * \param[in,out] Tmp          Pointer to a buffer which is used as temporary storage during the multiplication.
 * \param[out]    ResultPtr    Pointer to the verification result.
 * \param[in]     Value        Pointer to an edwards curve point given in projective coordinates
 *                             which should decoded. This parameter must not point to the same
 *                             memory location as the result "Result".
 * \param[in]     ValueLength  Length of point to be decoded.
 *
 * \returns If decoding has finished
 * \retval FALSE  The decoding has not yet finished. It has to be called again with
 *                the same parameters.
 * \retval TRUE   The decoding has finished. Now, "Result" contains the result
 *                of the decoding.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointDecoding
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Result,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultPtr,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                   Value,
  uint32                                                      ValueLength
);

/** \brief Start Signature verification.
 *
 *         This function requests the start of the EdDSA Signature
 *         verification for the given configuration and Key.
 *         The start is performed in Crypto_xVIx_xAIx_EdDSAVrfyMainFunction().
 *
 * \param[in] CfgPtr  A pointer to the configuration for which the start of the
 *                    signature verification should be requested.
 * \param[in] KeyPtr  A pointer to the key which should be used in the signature
 *                    verification.
 *
 *  \returns Error Value.
 *  \retval E_OK           If the service can be started.
 *  \retval E_NOT_OK       If the Key is invalid.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAVrfyStart
(
  P2CONST(void, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                  CfgPtr,
  P2CONST(Crypto_xVIx_xAIx_AsymPublicKeyType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyPtr
);

/** \brief Update Signature verification.
 *
 *
 *         This function requests the update of the EdDSA Signature
 *         verification for the given data.
 *         The update is performed in Crypto_xVIx_xAIx_EdDSAVrfyMainFunction().
 *
 * \param[in] DataPtr     A pointer to the start of an array which contains a part of the
 *                        data for which the signature should be created.
 * \param[in] DataLength  The amount of bytes of data.
 *
 *  \returns Error Value.
 *  \retval E_OK           If the service can be started.
 *  \retval E_NOT_OK       If the key is invalid.
 *  \retval CRYPTO_E_BUSY  If another instance of this service is already running.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAVrfyUpdate
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) DataPtr,
  uint32                                                DataLength
);

/** \brief Finish Signature verification.
 *
 *         This function requests the finishing of the EdDSA Signature
 *         verification and the storing of the signature in the given result buffer.
 *         The finishing is performed in Crypto_xVIx_xAIx_EdDSAVrfyMainFunction().
 *
 * \param[in]  SignaturePtr    A pointer to the start of a buffer which holds the signature.
 * \param[in]  SignatureLength Length of the signature in bytes.
 * \param[out] ResultPtr       Verification result.
 *
 *  \returns Error Value.
 *  \retval E_OK           If the service can be started.
 *  \retval E_NOT_OK       If the key is invalid.
 *  \retval CRYPTO_E_BUSY  If another instance of this service is already running.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAVrfyFinish
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                 SignaturePtr,
  uint32                                                                SignatureLength,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultPtr
);

/** \brief Perform the EdDSAVrfy tasks.
 *
 *         This function performs the actual EdDSA Signature verification tasks which
 *         have been requested by the service functions. When a task has finished, the
 *         function Csm_SignatureVerifyCallbackNotification() is called to inform the
 *         CSM of the Result. If the complete Signature verification has finished,
 *         additionally the function Csm_SignatureVerifyServiceFinishNotification() is called.
 *
 * \param[out] Result  A pointer to where to store the result of the function call
 * \param[out] Busy    A pointer to where to store if the function is busy or idle
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAVrfyMainFunction
(
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
);

/** \brief AL Callback
 *
 *         This function is a callbackfunction
 *
 * \param[in,out] Job    A pointer to the current job
 * \param[in]     Result The result of the process
 *
 * \Reentrancy Nonreentrant
 * \Synchronicity Synchronous/Asynchronous
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATURE_VERIFY_ED25519_NOT_SET_CallBack
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  VAR(uint8,                       CRYPTO_xVIx_xAIx_VAR)       Result
);

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON) */

/*==[Constants with internal linkage]=============================================================*/

/*--EdDSA-----------------------------------------------------------------------------------------*/

#define CRYPTO_xVIx_xAIx_START_SEC_CONST_8
#include <Crypto_xVIx_xAIx_MemMap.h>

/**
 * \brief Context string for Ed25519ph.
 *
 *        This variable is needed for Ed25519ph and contains:
 *        The ASCII string "SigEd25519 no Ed25519
 *        collisions" is converted into 32 octets. 0100 are 2 octets.
 *        Hence, the variable Crypto_xVIx_xAIx_Dom consists of 34 octets.
 *
 */
/* Deviation MISRAC2012-1 */
static CONST(uint8, CRYPTO_xVIx_xAIx_CONST) Crypto_xVIx_xAIx_Dom[CRYPTO_XVIX_XAIX_EDDSA_DOM_LEN] =
{
  0x53U, 0x69U, 0x67U, 0x45U, 0x64U, 0x32U, 0x35U, 0x35U,
  0x31U, 0x39U, 0x20U, 0x6EU, 0x6FU, 0x20U, 0x45U, 0x64U,
  0x32U, 0x35U, 0x35U, 0x31U, 0x39U, 0x20U, 0x63U, 0x6FU,
  0x6CU, 0x6CU, 0x69U, 0x73U, 0x69U, 0x6FU, 0x6EU, 0x73U,
  0x01U, 0x00U
};

#define CRYPTO_xVIx_xAIx_STOP_SEC_CONST_8
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Variables with internal linkage]=============================================================*/

/*--EdDSAGen--------------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED == STD_ON)

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief EdDSA Signature generation configuration. */
/* Deviation MISRAC2012-1 */
static VAR(Crypto_xVIx_xAIx_EdDSAGenConfigType, CRYPTO_xVIx_xAIx_VAR)
Crypto_xVIx_xAIx_AL_SIGNATURE_GENERATE_ED25519_NOT_SET_Config;

/** \brief EdDSA Signature generation context. */
static VAR(Crypto_xVIx_xAIx_EdDSAGenCtxType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_EdDSAGenCtx;

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if (CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED == STD_ON) */

/*--EdDSAVrfy-------------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON)

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief EdDSA Signature verification configuration. */
/* Deviation MISRAC2012-1 */
static VAR(Crypto_xVIx_xAIx_EdDSAVrfyConfigType, CRYPTO_xVIx_xAIx_VAR)
Crypto_xVIx_xAIx_AL_SIGNATURE_VERIFY_ED25519_NOT_SET_Config;

/** \brief EdDSA Signature verification context. */
static VAR(Crypto_xVIx_xAIx_EdDSAVrfyCtxType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_EdDSAVrfyCtx;

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON) */

/*==[Constants with external linkage]=============================================================*/

/*==[Variables with external linkage]=============================================================*/

/*==[Definition of functions with external linkage]===============================================*/

/*--EdDSAGen--------------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED == STD_ON)

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/* !LINKSTO EB_Crypto_CryptAlgos_1142, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ED25519_NOT_SET_Start
(
  uint32                                                     ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  Crypto_xVIx_xAIx_AsymPrivateKeyType Key = { { 0U }, 32U };
  /* !LINKSTO SWS_Crypto_00017, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Start, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Start2Start, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Update2Start, 1 */
  Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ED25519_NOT_SET_Reset();

  Crypto_xVIx_xAIx_AL_SIGNATURE_GENERATE_ED25519_NOT_SET_Config.Crypto_Generic_Callback =
  Crypto_xVIx_xAIx_AL_SIGNATURE_GENERATE_ED25519_NOT_SET_CallBack;
  Crypto_xVIx_xAIx_AL_SIGNATURE_GENERATE_ED25519_NOT_SET_Config.ObjId = ObjectId;
  Crypto_xVIx_xAIx_AL_SIGNATURE_GENERATE_ED25519_NOT_SET_Config.Job = Job;
  /*Update and Finish helpers are hardcoded with CRYPTO_PROCESSING_ASYNC. Inorder to make is
    consistant, here also CRYPTO_PROCESSING_ASYNC is used.*/
  Crypto_xVIx_xAIx_AL_SIGNATURE_GENERATE_ED25519_NOT_SET_Config.ProcessingType =
  CRYPTO_PROCESSING_ASYNC;

  RetVal = Crypto_xVIx_xAIx_AL_KeyElementGet
          (
             Job->cryptoKeyId,
             CRYPTO_KE_SIGNATURE_KEY,
             Key.data,
             &Key.length
          );

  if (CRYPTO_XVIX_XAIX_EDDSA_G_PRIKEY_LEN != Key.length)
  {
    RetVal = CRYPTO_E_KEY_SIZE_MISMATCH;
  }
  else
  {
    if (E_OK == RetVal)
    {
      RetVal = Crypto_xVIx_xAIx_EdDSAGenStart(&Crypto_xVIx_xAIx_AL_SIGNATURE_GENERATE_ED25519_NOT_SET_Config, &Key);
    }
    else
    {
      RetVal = E_NOT_OK;
    }
  }
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1163, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ED25519_NOT_SET_Update
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  TS_PARAM_UNUSED(ObjectId);
  RetVal = Crypto_xVIx_xAIx_EdDSAGenUpdate(
                                  Job->jobPrimitiveInputOutput.inputPtr,
                                  Job->jobPrimitiveInputOutput.inputLength
                                );
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1184, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ED25519_NOT_SET_Finish
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  TS_PARAM_UNUSED(ObjectId);
  RetVal = Crypto_xVIx_xAIx_EdDSAGenFinish(
                                  Job->jobPrimitiveInputOutput.outputPtr,
                                  Job->jobPrimitiveInputOutput.outputLengthPtr
                                );
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1224, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ED25519_NOT_SET_Cancel
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  /* This state indicates, that the Primitive is currently waiting for an Update/Finish call */
  if(
      CRYPTO_XVIX_XAIX_EDDSA_G_S_INITIALIZED ==
      Crypto_xVIx_xAIx_EdDSAGenCtx.GenState
    )
  {
    /* !LINKSTO SWS_Crypto_00122_CORRECTION, 1 */
    Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ED25519_NOT_SET_Reset();
    /* !LINKSTO EB_Crypto_00063, 1 */
    RetVal = E_OK;
  }
  else
  {
    /* job can not be canceled imideatly */
    Crypto_xVIx_xAIx_EdDSAGenCtx.CancelFlag = TRUE;
    /* !LINKSTO SWS_Crypto_00143_CORRECTION_SWS_Crypto_00183, 1 */
    RetVal = CRYPTO_E_JOB_CANCELED;
  }
  TS_PARAM_UNUSED(Job);
  TS_PARAM_UNUSED(ObjectId);
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1245, 1 */
FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ED25519_NOT_SET_MainFunction
(
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
)
{
  Crypto_xVIx_xAIx_EdDSAGenMainFunction(Result, Busy);
}

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if (CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED == STD_ON) */

/*--EdDSAVrfy-------------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON)

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/* !LINKSTO EB_Crypto_CryptAlgos_1143, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ED25519_NOT_SET_Start
(
  uint32                                                     ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal;
  Crypto_xVIx_xAIx_AsymPublicKeyType Key = { { 0U }, 32U };
  /* !LINKSTO SWS_Crypto_00017, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Start, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Start2Start, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Update2Start, 1 */
  Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ED25519_NOT_SET_Reset();

  Crypto_xVIx_xAIx_AL_SIGNATURE_VERIFY_ED25519_NOT_SET_Config.Crypto_Generic_Callback =
  &Crypto_xVIx_xAIx_AL_SIGNATURE_VERIFY_ED25519_NOT_SET_CallBack;
  Crypto_xVIx_xAIx_AL_SIGNATURE_VERIFY_ED25519_NOT_SET_Config.ObjId = ObjectId;
  Crypto_xVIx_xAIx_AL_SIGNATURE_VERIFY_ED25519_NOT_SET_Config.Job = Job;
  Crypto_xVIx_xAIx_AL_SIGNATURE_VERIFY_ED25519_NOT_SET_Config.ProcessingType =
  CRYPTO_PROCESSING_ASYNC;

  RetVal = Crypto_xVIx_xAIx_AL_KeyElementGet
          (
             Job->cryptoKeyId,
             CRYPTO_KE_SIGNATURE_KEY,
             Key.data,
             &(Key.length)
          );

  if (CRYPTO_XVIX_XAIX_EDDSA_V_PUBKEY_LEN != Key.length)
  {
    RetVal = CRYPTO_E_KEY_SIZE_MISMATCH;
  }
  else
  {
    if (E_OK == RetVal)
    {
    RetVal = Crypto_xVIx_xAIx_EdDSAVrfyStart(&Crypto_xVIx_xAIx_AL_SIGNATURE_VERIFY_ED25519_NOT_SET_Config, &Key);
    }
    else
    {
      RetVal = E_NOT_OK;
    }
  }
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1164, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ED25519_NOT_SET_Update
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal;
  TS_PARAM_UNUSED(ObjectId);
  RetVal = Crypto_xVIx_xAIx_EdDSAVrfyUpdate(
                                   Job->jobPrimitiveInputOutput.inputPtr,
                                   Job->jobPrimitiveInputOutput.inputLength
                                 );
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1185, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ED25519_NOT_SET_Finish
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal;
  TS_PARAM_UNUSED(ObjectId);
  RetVal = Crypto_xVIx_xAIx_EdDSAVrfyFinish(
                                   Job->jobPrimitiveInputOutput.secondaryInputPtr,
                                   Job->jobPrimitiveInputOutput.secondaryInputLength,
                                   Job->jobPrimitiveInputOutput.verifyPtr
                                 );
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1225, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ED25519_NOT_SET_Cancel
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  /* This state indicates, that the Primitive is currently waiting for an Update/Finish call */
  if(
      CRYPTO_XVIX_XAIX_EDDSA_V_S_INITIALIZED ==
      Crypto_xVIx_xAIx_EdDSAVrfyCtx.VrfyState
    )
  {
    /* !LINKSTO SWS_Crypto_00122_CORRECTION, 1 */
    Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ED25519_NOT_SET_Reset();
    RetVal = E_OK;
  }
  else
  {
    /* job can not be canceled imideatly */
    Crypto_xVIx_xAIx_EdDSAVrfyCtx.CancelFlag = TRUE;
    /* !LINKSTO SWS_Crypto_00143_CORRECTION_SWS_Crypto_00183, 1 */
    RetVal = CRYPTO_E_JOB_CANCELED;
  }
  TS_PARAM_UNUSED(Job);
  TS_PARAM_UNUSED(ObjectId);
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1246, 1 */
FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ED25519_NOT_SET_MainFunction
(
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
)
{
  Crypto_xVIx_xAIx_EdDSAVrfyMainFunction(Result, Busy);
}

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON) */

/*==[Definition of functions with internal linkage]===============================================*/

/*--EdDSAGen--------------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED == STD_ON)

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ED25519_NOT_SET_Reset
(
  void
)
{
  Crypto_xVIx_xAIx_AL_HASH_SHA2_NOT_SET_Reset();
  SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
  TS_MemBZero(
               &Crypto_xVIx_xAIx_EdDSAGenCtx,
               sizeof(Crypto_xVIx_xAIx_EdDSAGenCtx)
             );
  SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPrivateScalar
(
  uint8                                                                     CurveIndex,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                       HashPtr,
  uint32                                                                    HashPtrLength
)
{
  /* clear the first 3 bits */
  HashPtr[0U] &= (uint8)0xf8U;

  /* clear the highest bit of the last octet from the first hash half */
  HashPtr[31U] &=  (uint8)(~(uint8)((uint8)1U << 7U));

  /* set the second highest bit of the final octet from the first hash half */
  HashPtr[31U] |= (uint8)(1U << 6U);

  /* Set the expected length of the Long Number before converting the bytearray
   * to a little-endian integer */
  Result[0U] = (Crypto_xVIx_xAIx_LNWordType)
               ((Crypto_xVIx_xAIx_LNWordType)((Crypto_xVIx_xAIx_LNWordType)Crypto_xVIx_xAIx_EllipticCurveInfo[CurveIndex].BValue >> 5U) + 1U);

  /* copy the Hashresult to Result */
  /* The return value is ignored because the parameters are either from the context where they are
     properly defined or local variables which are properly defined before passing them to
     the function.*/
  (void)Crypto_xVIx_xAIx_LNLEByteArrayToNumber(
                                      HashPtr,
                                      HashPtrLength,
                                      Result,
                                      Result[0U]
                                    );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAProcessPrivKey
(
  void
)
{

  switch (Crypto_xVIx_xAIx_EdDSAGenCtx.PrivKeyHashState)
  {
    case CRYPTO_XVIX_XAIX_EDDSA_G_S_INIT_1:
    Crypto_xVIx_xAIx_EdDSAGenCtx.PrivKeyHashState = CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_INIT_1;
    Crypto_xVIx_xAIx_SHA2512HelperStart(
                                         Crypto_xVIx_xAIx_EdDSAGenCtx.ObjId,
                                         &Crypto_xVIx_xAIx_EdDSAGenCallback,
                                         Crypto_xVIx_xAIx_EdDSAGenCtx.Job,
                                         Crypto_xVIx_xAIx_EdDSAGenCtx.Job->jobPrimitiveInfo->processingType
                                       );
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_K:
    Crypto_xVIx_xAIx_EdDSAGenCtx.PrivKeyHashState = CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_K;
    /* we are in the correct state when this is called so it can not fail */
    (void)Crypto_xVIx_xAIx_SHA2512HelperUpdate
    (
      Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp1Array,
      Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp1ArrayLength,
      Crypto_xVIx_xAIx_EdDSAGenCtx.Job->jobPrimitiveInfo->processingType
    );
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_HASH_1:
    /* finish the hashing function and receive the hashed Value */
    Crypto_xVIx_xAIx_EdDSAGenCtx.HashResultLength = sizeof(Crypto_xVIx_xAIx_EdDSAGenCtx.HashResult);
    Crypto_xVIx_xAIx_EdDSAGenCtx.PrivKeyHashState = CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_FINISH_HASH_1;
    /* we are in the correct state when this is called so it can not fail */
    (void)Crypto_xVIx_xAIx_SHA2512HelperFinish(
                                         Crypto_xVIx_xAIx_EdDSAGenCtx.HashResult,
                                         &(Crypto_xVIx_xAIx_EdDSAGenCtx.HashResultLength),
                                         FALSE,
                                         Crypto_xVIx_xAIx_EdDSAGenCtx.Job->jobPrimitiveInfo->processingType
                                       );
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_SCALARS:
    /* calculate the private scalar */
    Crypto_xVIx_xAIx_EdDSAPrivateScalar(
                               Crypto_xVIx_xAIx_EdDSAGenCtx.EdDSACtx.ECCtx.CurveIndex,
                               Crypto_xVIx_xAIx_EdDSAGenCtx.ScalarS,
                               Crypto_xVIx_xAIx_EdDSAGenCtx.HashResult,
                               (Crypto_xVIx_xAIx_EdDSAGenCtx.HashResultLength >> 1U)
                             );

    Crypto_xVIx_xAIx_EdDSAGenCtx.PrivKeyHashState = CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_PUBLIC_KEY_1;
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_PUBLIC_KEY_1:
    /* A = s * the Point B */
    Crypto_xVIx_xAIx_EdDSAPointMultiplication(
                                               &Crypto_xVIx_xAIx_EdDSAGenCtx.EdDSACtx,
                                               &Crypto_xVIx_xAIx_EdDSAGenCtx.A,
                                               &(Crypto_xVIx_xAIx_EllipticCurveInfo[Crypto_xVIx_xAIx_EdDSAGenCtx.EdDSACtx.ECCtx.CurveIndex].P),
                                               Crypto_xVIx_xAIx_EdDSAGenCtx.ScalarS,
                                               Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp
                                             );
    Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp2ArrayLength = (uint32)((uint32)Crypto_xVIx_xAIx_EllipticCurveInfo[Crypto_xVIx_xAIx_EdDSAGenCtx.EdDSACtx.ECCtx.CurveIndex].BValue >> 3U);
    Crypto_xVIx_xAIx_EdDSAPointEncodingStart(&Crypto_xVIx_xAIx_EdDSAGenCtx.EdDSACtx);
    Crypto_xVIx_xAIx_EdDSAGenCtx.PrivKeyHashState = CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_PUBLIC_KEY_2;
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_PUBLIC_KEY_2:
    /* converting the A to a byte array and encoding it for the HashUpdate function */
    if (
         TRUE ==
         Crypto_xVIx_xAIx_EdDSAPointEncoding(
                                    &Crypto_xVIx_xAIx_EdDSAGenCtx.EdDSACtx,
                                    &Crypto_xVIx_xAIx_EdDSAGenCtx.A,
                                    Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp,
                                    Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp2Array,  /* Result = the public Key */
                                    Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp1Array,
                                    Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp2ArrayLength,
                                    Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp1ArrayLength
                                  )
       )
    {
      Crypto_xVIx_xAIx_EdDSAGenCtx.GenState = CRYPTO_XVIX_XAIX_EDDSA_G_S_INITIALIZED;
      Crypto_xVIx_xAIx_EdDSAGenCtx.PrivKeyHashState = CRYPTO_XVIX_XAIX_EDDSA_G_S_INIT_1;
      Crypto_xVIx_xAIx_EdDSAGenCtx.ErrorCode = E_OK;

      Crypto_xVIx_xAIx_EdDSAGenCtx.CallbackFlag = TRUE;
      Crypto_xVIx_xAIx_EdDSAGenCtx.CallbackResult = Crypto_xVIx_xAIx_EdDSAGenCtx.ErrorCode;
    }
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_INIT_1:
    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_K:
    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_FINISH_HASH_1:
    /* gets called only if processing Type is ASYNC*/
    Crypto_xVIx_xAIx_SHA2512HelperMainFunction();
    break;
    /* CHECK: NOPARSE */
    /*All the possible states have been addressed in the cases above.*/
    default:
    {
      /* Defensive Programming - unreachable code. This default case is required by  MISRA-C:2012 */
    }
    break;
    /* CHECK: PARSE */
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAGenHashMsg
(
  void
)
{

  switch (Crypto_xVIx_xAIx_EdDSAGenCtx.MsgHashState)
  {
    case CRYPTO_XVIX_XAIX_EDDSA_G_S_INIT_HASH_MESSAGE:
    Crypto_xVIx_xAIx_EdDSAGenCtx.MsgHashState =
    CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_INIT_HASH_MESSAGE;
    Crypto_xVIx_xAIx_SHA2512HelperStart(
                                         Crypto_xVIx_xAIx_EdDSAGenCtx.ObjId,
                                         &Crypto_xVIx_xAIx_EdDSAGenCallback,
                                         Crypto_xVIx_xAIx_EdDSAGenCtx.Job,
                                         Crypto_xVIx_xAIx_EdDSAGenCtx.Job->jobPrimitiveInfo->processingType
                                       );
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_HASH_MESSAGE:
    Crypto_xVIx_xAIx_EdDSAGenCtx.CurrentMode = CRYPTO_OPERATIONMODE_UPDATE;
    Crypto_xVIx_xAIx_EdDSAGenCtx.MsgHashState = CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_HASH_MESSAGE;
    /* we are in the correct state when this is called so it can not fail */
    (void)Crypto_xVIx_xAIx_SHA2512HelperUpdate(
                                         Crypto_xVIx_xAIx_EdDSAGenCtx.Message,
                                         Crypto_xVIx_xAIx_EdDSAGenCtx.MessageLength,
                                         Crypto_xVIx_xAIx_EdDSAGenCtx.Job->jobPrimitiveInfo->processingType
                                       );
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_HASH_MESSAGE:
    Crypto_xVIx_xAIx_EdDSAGenCtx.CurrentMode = CRYPTO_OPERATIONMODE_FINISH;
    if(CRYPTO_XVIX_XAIX_EDDSA_SIGN_SIZE > *Crypto_xVIx_xAIx_EdDSAGenCtx.ResultLength)
    {
      Crypto_xVIx_xAIx_EdDSAGenCtx.CallbackFlag = TRUE;
      Crypto_xVIx_xAIx_EdDSAGenCtx.CallbackResult = CRYPTO_E_SMALL_BUFFER;
    }
    Crypto_xVIx_xAIx_EdDSAGenCtx.HashResultLength = sizeof(Crypto_xVIx_xAIx_EdDSAGenCtx.HashMessage);
    Crypto_xVIx_xAIx_EdDSAGenCtx.MsgHashState = CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_FINISH_HASH_MESSAGE;
    /* we are in the correct state when this is called so it can not fail */
    (void)Crypto_xVIx_xAIx_SHA2512HelperFinish(
                                         Crypto_xVIx_xAIx_EdDSAGenCtx.HashMessage,
                                         &(Crypto_xVIx_xAIx_EdDSAGenCtx.HashResultLength),
                                         FALSE,
                                         Crypto_xVIx_xAIx_EdDSAGenCtx.Job->jobPrimitiveInfo->processingType
                                       );
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_INIT_HASH_MESSAGE:
    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_HASH_MESSAGE:
    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_FINISH_HASH_MESSAGE:
    /* gets called only if processing Type is ASYNC */
    Crypto_xVIx_xAIx_SHA2512HelperMainFunction();
    break;

    /* CHECK: NOPARSE */
    /*All the possible states have been addressed in the cases above.*/
    default:
    {
      /* Defensive Programming - unreachable code. This default case is required by  MISRA-C:2012 */
    }
    break;
    /* CHECK: PARSE */
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSACalculateLittleR
(
  void
)
{

  switch (Crypto_xVIx_xAIx_EdDSAGenCtx.LittleRState)
  {
    case CRYPTO_XVIX_XAIX_EDDSA_G_S_INIT_2:
    Crypto_xVIx_xAIx_EdDSAGenCtx.LittleRState = CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_INIT_2;
    Crypto_xVIx_xAIx_SHA2512HelperStart(
                                         Crypto_xVIx_xAIx_EdDSAGenCtx.ObjId,
                                         &Crypto_xVIx_xAIx_EdDSAGenCallback,
                                         Crypto_xVIx_xAIx_EdDSAGenCtx.Job,
                                         Crypto_xVIx_xAIx_EdDSAGenCtx.Job->jobPrimitiveInfo->processingType
                                       );
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_DOM2_1:
    /* update the parameter Crypto_xVIx_xAIx_Dom */
    Crypto_xVIx_xAIx_EdDSAGenCtx.LittleRState = CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_DOM2_1;
    /* we are in the correct state when this is called so it can not fail */
    (void)Crypto_xVIx_xAIx_SHA2512HelperUpdate(
                                         Crypto_xVIx_xAIx_Dom,
                                         sizeof(Crypto_xVIx_xAIx_Dom),
                                         Crypto_xVIx_xAIx_EdDSAGenCtx.Job->jobPrimitiveInfo->processingType
                                       );
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_HB:
    /* update the Hashfunction h_b to h_(2*b-1) */
    Crypto_xVIx_xAIx_EdDSAGenCtx.LittleRState = CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_HB;
    /* we are in the correct state when this is called so it can not fail */
    (void)Crypto_xVIx_xAIx_SHA2512HelperUpdate(
                                         &Crypto_xVIx_xAIx_EdDSAGenCtx.HashResult[
                                         Crypto_xVIx_xAIx_EdDSAGenCtx.HashResultLength / (uint32) 2U],
                                         (Crypto_xVIx_xAIx_EdDSAGenCtx.HashResultLength / (uint32) 2U),
                                         Crypto_xVIx_xAIx_EdDSAGenCtx.Job->jobPrimitiveInfo->processingType
                                       );
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_M:
    /* update the hashfunction, H(M) */
    Crypto_xVIx_xAIx_EdDSAGenCtx.MessageLength = sizeof(Crypto_xVIx_xAIx_EdDSAGenCtx.HashMessage);
    Crypto_xVIx_xAIx_EdDSAGenCtx.LittleRState  = CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_M;
    /* we are in the correct state when this is called so it can not fail */
    (void)Crypto_xVIx_xAIx_SHA2512HelperUpdate(
                                         Crypto_xVIx_xAIx_EdDSAGenCtx.HashMessage,
                                         Crypto_xVIx_xAIx_EdDSAGenCtx.MessageLength,
                                         Crypto_xVIx_xAIx_EdDSAGenCtx.Job->jobPrimitiveInfo->processingType
                                       );
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_HASH_2:
    /* finish the hashing function and receive the hashed Value */
    Crypto_xVIx_xAIx_EdDSAGenCtx.LittleRState = CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_FINISH_HASH_2;
    /* we are in the correct state when this is called so it can not fail */
    (void)Crypto_xVIx_xAIx_SHA2512HelperFinish(
                                         Crypto_xVIx_xAIx_EdDSAGenCtx.HashResult,
                                         &(Crypto_xVIx_xAIx_EdDSAGenCtx.HashResultLength),
                                         FALSE,
                                         Crypto_xVIx_xAIx_EdDSAGenCtx.Job->jobPrimitiveInfo->processingType
                                       );
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_INIT_2:
    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_DOM2_1:
    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_HB:
    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_M:
    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_FINISH_HASH_2:
    /* gets called only if processing Type is ASYNC */
    Crypto_xVIx_xAIx_SHA2512HelperMainFunction();
    break;

    /* CHECK: NOPARSE */
    /*All the possible states have been addressed in the cases above.*/
    default:
    {
      /* Defensive Programming - unreachable code. This default case is required by  MISRA-C:2012 */
    }
    break;
    /* CHECK: PARSE */

  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAGenCalculateLittleK
(
  void
)
{
  switch (Crypto_xVIx_xAIx_EdDSAGenCtx.LittleKState)
  {
    case CRYPTO_XVIX_XAIX_EDDSA_G_S_INIT_3:
    Crypto_xVIx_xAIx_EdDSAGenCalculateLittleK_Init3();
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_DOM2_2:
    Crypto_xVIx_xAIx_EdDSAGenCalculateLittleK_UpdateDom2();
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_R:
    /* converting the R to a byte array and encoding it for the HashUpdate function */
    Crypto_xVIx_xAIx_EdDSAEncodeAndHashR();
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_A:
    Crypto_xVIx_xAIx_EdDSAGenCalculateLittleK_UpdateA();
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_M2:
    Crypto_xVIx_xAIx_EdDSAGenCalculateLittleK_UpdateM2();
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_HASH_3:
    Crypto_xVIx_xAIx_EdDSAGenCalculateLittleK_FinishHash();
    break;
    default:
    Crypto_xVIx_xAIx_EdDSAGenCalculateLittleK_DefaultState();
    break;

  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAGenCalculateLittleK_Init3
(
  void
)
{
  /* initialize the hash interface again for the computation of the S */
  Crypto_xVIx_xAIx_EdDSAGenCtx.LittleKState = CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_INIT_3;
  Crypto_xVIx_xAIx_SHA2512HelperStart(
                                       Crypto_xVIx_xAIx_EdDSAGenCtx.ObjId,
                                       &Crypto_xVIx_xAIx_EdDSAGenCallback,
                                       Crypto_xVIx_xAIx_EdDSAGenCtx.Job,
                                       Crypto_xVIx_xAIx_EdDSAGenCtx.Job->jobPrimitiveInfo->processingType
                                     );
  Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp3ArrayLength =
  (uint32) ((uint32) Crypto_xVIx_xAIx_EllipticCurveInfo[Crypto_xVIx_xAIx_EdDSAGenCtx.EdDSACtx.ECCtx.CurveIndex].BValue >> 3U);
  Crypto_xVIx_xAIx_EdDSAPointEncodingStart(&Crypto_xVIx_xAIx_EdDSAGenCtx.EdDSACtx);
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAGenCalculateLittleK_UpdateDom2
(
  void
)
{
  /* update the parameter Dom */
  Crypto_xVIx_xAIx_EdDSAGenCtx.LittleKState = CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_DOM2_2;
  /* we are in the correct state when this is called so it can not fail */
  (void)Crypto_xVIx_xAIx_SHA2512HelperUpdate(
                                       Crypto_xVIx_xAIx_Dom,
                                       sizeof(Crypto_xVIx_xAIx_Dom),
                                       Crypto_xVIx_xAIx_EdDSAGenCtx.Job->jobPrimitiveInfo->processingType
                                     );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAGenCalculateLittleK_UpdateA
(
  void
)
{
  Crypto_xVIx_xAIx_EdDSAGenCtx.LittleKState = CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_A;
  /* we are in the correct state when this is called so it can not fail */
  (void)Crypto_xVIx_xAIx_SHA2512HelperUpdate
           (
             Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp2Array,
             Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp2ArrayLength,
             Crypto_xVIx_xAIx_EdDSAGenCtx.Job->jobPrimitiveInfo->processingType
           );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAGenCalculateLittleK_UpdateM2
(
  void
)
{
  /* Hash the pre-hashed Message (PH(M)) */
  Crypto_xVIx_xAIx_EdDSAGenCtx.LittleKState = CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_M2;
  /* we are in the correct state when this is called so it can not fail */
  (void)Crypto_xVIx_xAIx_SHA2512HelperUpdate
           (
             Crypto_xVIx_xAIx_EdDSAGenCtx.HashMessage,
             sizeof(Crypto_xVIx_xAIx_EdDSAGenCtx.HashMessage),
             Crypto_xVIx_xAIx_EdDSAGenCtx.Job->jobPrimitiveInfo->processingType
           );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAGenCalculateLittleK_FinishHash
(
  void
)
{
  /* finish the hashing function and receive the hashed Value */
  Crypto_xVIx_xAIx_EdDSAGenCtx.LittleKState = CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_FINISH_HASH_3;
  /* we are in the correct state when this is called so it can not fail */
  (void)Crypto_xVIx_xAIx_SHA2512HelperFinish
           (
             Crypto_xVIx_xAIx_EdDSAGenCtx.HashResult,
             &(Crypto_xVIx_xAIx_EdDSAGenCtx.HashResultLength),
             FALSE,
             Crypto_xVIx_xAIx_EdDSAGenCtx.Job->jobPrimitiveInfo->processingType
           );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAGenCalculateLittleK_DefaultState
(
  void
)
{
  switch(Crypto_xVIx_xAIx_EdDSAGenCtx.LittleKState)
  {
    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_INIT_3:
    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_DOM2_2:
    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_R:
    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_A:
    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_M2:
    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_FINISH_HASH_3:
    /* gets called only if processing Type is ASYNC */
    Crypto_xVIx_xAIx_SHA2512HelperMainFunction();
    break;

    /* CHECK: NOPARSE */
    /*All the possible (wait)states have been addressed in the cases above.*/
    default:
    {
      /* Defensive Programming - unreachable code. This default case is required by  MISRA-C:2012 */
    }
    break;
    /* CHECK: PARSE */
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAEncodeAndHashR
(
  void
)
{

  if (
       TRUE ==
       Crypto_xVIx_xAIx_EdDSAPointEncoding(
                                  &Crypto_xVIx_xAIx_EdDSAGenCtx.EdDSACtx,
                                  &Crypto_xVIx_xAIx_EdDSAGenCtx.R,
                                  Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp,
                                  Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp1Array,
                                  Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp3Array,
                                  Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp1ArrayLength,
                                  Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp3ArrayLength
                                )
     )
  {
    /* Result = ENC(R) */
    Crypto_xVIx_xAIx_EdDSAGenCtx.LittleKState = CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_R;
    /* we are in the correct state when this is called so it can not fail */
    (void)Crypto_xVIx_xAIx_SHA2512HelperUpdate(
                                         Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp1Array,
                                         Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp1ArrayLength,
                                         Crypto_xVIx_xAIx_EdDSAGenCtx.Job->jobPrimitiveInfo->processingType
                                       );
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSACalculateScalars
(
  void
)
{
  switch (Crypto_xVIx_xAIx_EdDSAGenCtx.ScalarsState)
  {
    case CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_SCALARR1_INT:
    /* Scalar r */
    /* This call can never fail because we have enough memory allocated in the context
     * to store the byte representation of the LN. */
    (void)Crypto_xVIx_xAIx_LNLEByteArrayToNumber
    (
      Crypto_xVIx_xAIx_EdDSAGenCtx.HashResult,
      Crypto_xVIx_xAIx_EdDSAGenCtx.HashResultLength,
      Crypto_xVIx_xAIx_EdDSAGenCtx.ScalarR,
      sizeof(Crypto_xVIx_xAIx_EdDSAGenCtx.ScalarR) / sizeof(Crypto_xVIx_xAIx_EdDSAGenCtx.ScalarR[0U])
    );
    /* start the modulo function */
    Crypto_xVIx_xAIx_EdDSAGenCtx.ScalarsState = CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_SCALARR2;
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_SCALARR2:
    /* r = r % LValue */

    Crypto_xVIx_xAIx_LNModulo(
                               Crypto_xVIx_xAIx_EdDSAGenCtx.ScalarR,
                               Crypto_xVIx_xAIx_EllipticCurveInfo[Crypto_xVIx_xAIx_EdDSAGenCtx.EdDSACtx.ECCtx.CurveIndex].Modulus,
                               &Crypto_xVIx_xAIx_EdDSAGenCtx.EdDSACtx.ECCtx.LNCtx
                             );
    Crypto_xVIx_xAIx_EdDSAGenCtx.ScalarsState = CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_SCALARR3;
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_SCALARR3:
    /* R = r * the Point B */
    Crypto_xVIx_xAIx_EdDSAPointMultiplication(
                                               &Crypto_xVIx_xAIx_EdDSAGenCtx.EdDSACtx,
                                               &Crypto_xVIx_xAIx_EdDSAGenCtx.R,
                                               &(Crypto_xVIx_xAIx_EllipticCurveInfo[Crypto_xVIx_xAIx_EdDSAGenCtx.EdDSACtx.ECCtx.CurveIndex].P),
                                               Crypto_xVIx_xAIx_EdDSAGenCtx.ScalarR,
                                               Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp
                                             );
    Crypto_xVIx_xAIx_EdDSAGenCtx.GenState = CRYPTO_XVIX_XAIX_EDDSA_G_S_LITTLE_K;
    Crypto_xVIx_xAIx_EdDSAGenCtx.ScalarsState = CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_SCALARR1_INT;
    break;

    /* CHECK: NOPARSE */
    /*All the possible states have been addressed in the cases above.*/
    default:
    {
      /* Defensive Programming - unreachable code. This default case is required by  MISRA-C:2012 */
    }
    break;
    /* CHECK: PARSE */
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAFinishSignature
(
  void
)
{
  switch (Crypto_xVIx_xAIx_EdDSAGenCtx.GenFinishSig)
  {
    case CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_S1:
    Crypto_xVIx_xAIx_EdDSAEncodeSFromHash();
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_S2_TEMP:
    /* Tmp1 = Tmp1 % LValue */

    Crypto_xVIx_xAIx_LNModulo
    (
      &Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp[1U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS],
      Crypto_xVIx_xAIx_EllipticCurveInfo[Crypto_xVIx_xAIx_EdDSAGenCtx.EdDSACtx.ECCtx.CurveIndex].Modulus,
      &Crypto_xVIx_xAIx_EdDSAGenCtx.EdDSACtx.ECCtx.LNCtx
    );

    Crypto_xVIx_xAIx_EdDSAGenCtx.GenFinishSig = CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_S2;
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_S2:
    /* Tmp1 = HashResult * Scalar s */
    Crypto_xVIx_xAIx_LNMultiplyNumbers
    (
      &Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp[1U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS],
      &Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp[1U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS],
      Crypto_xVIx_xAIx_EdDSAGenCtx.ScalarS,
      &Crypto_xVIx_xAIx_EdDSAGenCtx.EdDSACtx.ECCtx.LNCtx
    );
    Crypto_xVIx_xAIx_EdDSAGenCtx.GenFinishSig = CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_EXPORT_TEMP;
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_EXPORT_TEMP:
    /* Tmp1 = Tmp1 % LValue */

    Crypto_xVIx_xAIx_LNModulo
    (
      &Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp[1U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS],
      Crypto_xVIx_xAIx_EllipticCurveInfo[Crypto_xVIx_xAIx_EdDSAGenCtx.EdDSACtx.ECCtx.CurveIndex].Modulus,
      &Crypto_xVIx_xAIx_EdDSAGenCtx.EdDSACtx.ECCtx.LNCtx
    );

    /* Tmp2 = Scalar r + Tmp1 */
    Crypto_xVIx_xAIx_LNAdditionModular(
                              &Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp[2U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS],
                              Crypto_xVIx_xAIx_EdDSAGenCtx.ScalarR,
                              &Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp[1U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS],
                              Crypto_xVIx_xAIx_EllipticCurveInfo[Crypto_xVIx_xAIx_EdDSAGenCtx.EdDSACtx.ECCtx.CurveIndex].Modulus
                            );
    Crypto_xVIx_xAIx_EdDSAGenCtx.GenFinishSig = CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_EXPORT;
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_EXPORT:
    Crypto_xVIx_xAIx_EdDSAExportSign();
    break;

    /* CHECK: NOPARSE */
    /*All the possible states have been addressed in the cases above.*/
    default:
    {
      /* Defensive Programming - unreachable code. This default case is required by  MISRA-C:2012 */
    }
    break;
     /* CHECK: PARSE */
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPrivKeyCallback
(
  void
)
{
  switch (Crypto_xVIx_xAIx_EdDSAGenCtx.PrivKeyHashState)
  {
    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_INIT_1:
    Crypto_xVIx_xAIx_EdDSAGenCtx.PrivKeyHashState = CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_K;
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_K:
    Crypto_xVIx_xAIx_EdDSAGenCtx.PrivKeyHashState = CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_HASH_1;
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_FINISH_HASH_1:
    Crypto_xVIx_xAIx_EdDSAGenCtx.PrivKeyHashState = CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_SCALARS;
    break;

     /* CHECK: NOPARSE */
     /*All the possible (wait) states have been addressed in the cases above.*/
    default:
    {
      /* Defensive Programming - unreachable code. This default case is required by  MISRA-C:2012 */
    }
    break;
    /* CHECK: PARSE */
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAHashMsgCallback
(
  void
)
{
  switch (Crypto_xVIx_xAIx_EdDSAGenCtx.MsgHashState)
  {
    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_INIT_HASH_MESSAGE:
    Crypto_xVIx_xAIx_EdDSAGenCtx.MsgHashState = CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_HASH_MESSAGE;
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_HASH_MESSAGE:
    /* Wait for another Update call. */
    Crypto_xVIx_xAIx_EdDSAGenCtx.GenState = CRYPTO_XVIX_XAIX_EDDSA_G_S_INITIALIZED;

    Crypto_xVIx_xAIx_EdDSAGenCtx.ErrorCode = E_OK;
    Crypto_xVIx_xAIx_EdDSAGenCtx.CallbackFlag = TRUE;
    Crypto_xVIx_xAIx_EdDSAGenCtx.CallbackResult = Crypto_xVIx_xAIx_EdDSAGenCtx.ErrorCode;
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_FINISH_HASH_MESSAGE:
    Crypto_xVIx_xAIx_EdDSAGenCtx.MsgHashState = CRYPTO_XVIX_XAIX_EDDSA_G_S_INIT_HASH_MESSAGE;
    Crypto_xVIx_xAIx_EdDSAGenCtx.GenState = CRYPTO_XVIX_XAIX_EDDSA_G_S_LITTLE_R;
    break;

     /* CHECK: NOPARSE */
     /*All the possible (wait) states have been addressed in the cases above.*/
    default:
    {
      /* Defensive Programming - unreachable code. This default case is required by  MISRA-C:2012 */
    }
    break;
    /* CHECK: PARSE */
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSALittleRCallback
(
  void
)
{
  switch (Crypto_xVIx_xAIx_EdDSAGenCtx.LittleRState)
  {
    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_INIT_2:
    Crypto_xVIx_xAIx_EdDSAGenCtx.LittleRState = CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_DOM2_1;
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_DOM2_1:
    Crypto_xVIx_xAIx_EdDSAGenCtx.LittleRState = CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_HB;
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_HB:
    Crypto_xVIx_xAIx_EdDSAGenCtx.LittleRState = CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_M;
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_M:
    Crypto_xVIx_xAIx_EdDSAGenCtx.LittleRState = CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_HASH_2;
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_FINISH_HASH_2:
    Crypto_xVIx_xAIx_EdDSAGenCtx.LittleRState = CRYPTO_XVIX_XAIX_EDDSA_G_S_INIT_2;
    Crypto_xVIx_xAIx_EdDSAGenCtx.GenState     = CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_SCALARR1;
    break;

    /* CHECK: NOPARSE */
    /*All the possible (wait) states have been addressed in the cases above.*/
    default:
    {
      /* Defensive Programming - unreachable code. This default case is required by  MISRA-C:2012 */
    }
    break;
    /* CHECK: PARSE */
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSALittleKCallback
(
  void
)
{
  switch (Crypto_xVIx_xAIx_EdDSAGenCtx.LittleKState)
  {
    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_INIT_3:
    Crypto_xVIx_xAIx_EdDSAGenCtx.LittleKState = CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_DOM2_2;
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_DOM2_2:
    Crypto_xVIx_xAIx_EdDSAGenCtx.LittleKState = CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_R;
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_R:
    Crypto_xVIx_xAIx_EdDSAGenCtx.LittleKState = CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_A;
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_A:
    Crypto_xVIx_xAIx_EdDSAGenCtx.LittleKState = CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_M2;
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_UPDATE_M2:
    Crypto_xVIx_xAIx_EdDSAGenCtx.LittleKState = CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_HASH_3;
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_WAIT_FOR_FINISH_HASH_3:
    Crypto_xVIx_xAIx_EdDSAGenCtx.LittleKState = CRYPTO_XVIX_XAIX_EDDSA_G_S_INIT_3;
    Crypto_xVIx_xAIx_EdDSAGenCtx.GenState = CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_SIG;
    break;

    /* CHECK: NOPARSE */
    /*All the possible (wait) states have been addressed in the cases above*/
    default:
    {
      /* Defensive Programming - unreachable code. This default case is required by  MISRA-C:2012 */
    }
    break;
    /* CHECK: PARSE */
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAExportSign
(
  void
)
{
  uint32 Temp;

  /* ENC(R): Tmp1Array - Little endian byte Array */
  Temp = Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp1ArrayLength;

  TS_MemCpy(
      Crypto_xVIx_xAIx_EdDSAGenCtx.Result,
      Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp1Array,
      Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp1ArrayLength
      );

  /* S: Tmp2 in little endian byte array */
  /* The return value is ignored because the parameters are either from the context where they are
     properly defined or local variables which are properly defined before passing them to
     the function.*/
  (void)Crypto_xVIx_xAIx_LNNumberToLEByteArray(
                                      &Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp[2U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS],
                                      &Crypto_xVIx_xAIx_EdDSAGenCtx.Result[Temp],
                                      &Temp
                                    );

  /* !LINKSTO EB_Crypto_01030, 1 */
  *Crypto_xVIx_xAIx_EdDSAGenCtx.ResultLength = Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp1ArrayLength + Temp;

  Crypto_xVIx_xAIx_EdDSAGenCtx.GenFinishSig = CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_S1;
  Crypto_xVIx_xAIx_EdDSAGenCtx.GenState = CRYPTO_XVIX_XAIX_EDDSA_G_S_IDLE;

  Crypto_xVIx_xAIx_EdDSAGenCtx.ErrorCode = E_OK;
  Crypto_xVIx_xAIx_EdDSAGenCtx.CallbackFlag = TRUE;
  Crypto_xVIx_xAIx_EdDSAGenCtx.CallbackResult = Crypto_xVIx_xAIx_EdDSAGenCtx.ErrorCode;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAEncodeSFromHash
(
  void
)
{

  /* This call can never fail because we have enough memory allocated in the context
   * to store the byte representation of the LN. */
  (void)Crypto_xVIx_xAIx_LNLEByteArrayToNumber
       (
         Crypto_xVIx_xAIx_EdDSAGenCtx.HashResult,
         Crypto_xVIx_xAIx_EdDSAGenCtx.HashResultLength,
         &Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp[1U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS],
         CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS
       );
  Crypto_xVIx_xAIx_EdDSAGenCtx.GenFinishSig = CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_S2_TEMP;
}

/* !LINKSTO EB_Crypto_CryptAlgos_0963, 1 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAGenStart
(
  P2CONST(void, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                                CfgPtr,
  P2CONST(Crypto_xVIx_xAIx_AsymPrivateKeyType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PrivateKeyPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  /* Store data into context */
  P2CONST(Crypto_xVIx_xAIx_EdDSAGenConfigType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) TempConfig =
  (P2CONST(Crypto_xVIx_xAIx_EdDSAGenConfigType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA))CfgPtr;

  Crypto_xVIx_xAIx_EdDSAGenCtx.Job = TempConfig->Job;
  Crypto_xVIx_xAIx_EdDSAGenCtx.ObjId = TempConfig->ObjId;
  Crypto_xVIx_xAIx_EdDSAGenCtx.Crypto_Generic_Callback = TempConfig->Crypto_Generic_Callback;
  Crypto_xVIx_xAIx_EdDSAGenCtx.ProcessingType = TempConfig->ProcessingType;

  Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp1ArrayLength = PrivateKeyPtr->length;
  TS_MemCpy(Crypto_xVIx_xAIx_EdDSAGenCtx.Tmp1Array, PrivateKeyPtr->data, PrivateKeyPtr->length);

  /* Set state of the state machine to init */
  Crypto_xVIx_xAIx_EdDSAGenCtx.GenState = CRYPTO_XVIX_XAIX_EDDSA_G_S_HASH_PRIV_K;
  Crypto_xVIx_xAIx_EdDSAGenCtx.CurrentMode = CRYPTO_OPERATIONMODE_START;
  Crypto_xVIx_xAIx_EdDSAGenCtx.MsgHashStarted = FALSE;
  Crypto_xVIx_xAIx_EdDSAGenCtx.ErrorCode = E_OK;

  /* The function can only return E_OK here. */
  (void)Crypto_xVIx_xAIx_GetECIndex
        (
          Crypto_xVIx_xAIx_EdDSAGenCtx.Job->jobPrimitiveInfo->primitiveInfo->service,
          Crypto_xVIx_xAIx_EdDSAGenCtx.Job->jobPrimitiveInfo->primitiveInfo->algorithm.family,
          NULL_PTR,
          0U,
          &(Crypto_xVIx_xAIx_EdDSAGenCtx.EdDSACtx.ECCtx.CurveIndex)
        );

  RetVal = E_OK;

  return RetVal;
}

static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAGenUpdate
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) DataPtr,
  uint32                                                DataLength
)
{
  Std_ReturnType RetVal;

  if(CRYPTO_XVIX_XAIX_EDDSA_G_S_INITIALIZED == Crypto_xVIx_xAIx_EdDSAGenCtx.GenState)
  {
    /* Store data into context */
    Crypto_xVIx_xAIx_EdDSAGenCtx.Message = DataPtr;
    Crypto_xVIx_xAIx_EdDSAGenCtx.MessageLength = DataLength;

    /* First Message block to be hashed */
    if(FALSE == Crypto_xVIx_xAIx_EdDSAGenCtx.MsgHashStarted)
    {
      Crypto_xVIx_xAIx_EdDSAGenCtx.MsgHashStarted = TRUE;
      Crypto_xVIx_xAIx_EdDSAGenCtx.MsgHashState = CRYPTO_XVIX_XAIX_EDDSA_G_S_INIT_HASH_MESSAGE;
    }
    else
    {
      /* Subsequent Message blocks to be hashed */
      Crypto_xVIx_xAIx_EdDSAGenCtx.MsgHashState = CRYPTO_XVIX_XAIX_EDDSA_G_S_UPDATE_HASH_MESSAGE;
    }

    /* Set state of the state machine to update */
    Crypto_xVIx_xAIx_EdDSAGenCtx.GenState = CRYPTO_XVIX_XAIX_EDDSA_G_S_HASH_MESSAGE;
    Crypto_xVIx_xAIx_EdDSAGenCtx.ErrorCode = E_OK;
    RetVal = E_OK;
  }
  else
  {
    RetVal = CRYPTO_E_BUSY;
  }
  return RetVal;
}

static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAGenFinish
(
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  ResultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultLengthPtr
)
{
  Std_ReturnType RetVal;

  if(
       (TRUE == Crypto_xVIx_xAIx_EdDSAGenCtx.MsgHashStarted) &&
       (CRYPTO_XVIX_XAIX_EDDSA_G_S_INITIALIZED == Crypto_xVIx_xAIx_EdDSAGenCtx.GenState)
     )
  {
    /* Store data into context */
    Crypto_xVIx_xAIx_EdDSAGenCtx.Result = ResultPtr;

    Crypto_xVIx_xAIx_EdDSAGenCtx.ResultLength = ResultLengthPtr;

    /* Set state of the state machine to Update */
    Crypto_xVIx_xAIx_EdDSAGenCtx.GenState = CRYPTO_XVIX_XAIX_EDDSA_G_S_HASH_MESSAGE;
    Crypto_xVIx_xAIx_EdDSAGenCtx.MsgHashState = CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_HASH_MESSAGE;
    Crypto_xVIx_xAIx_EdDSAGenCtx.MsgHashStarted = FALSE;
    Crypto_xVIx_xAIx_EdDSAGenCtx.ErrorCode = E_OK;
    RetVal = E_OK;
  }
  else
  {
    RetVal = CRYPTO_E_BUSY;
  }
  return RetVal;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAGenMainFunction
(
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
)
{
  if(!Crypto_xVIx_xAIx_CheckSkipMain(Result, Busy, Crypto_xVIx_xAIx_EdDSAGenCtx.ObjId))
  {
    Crypto_xVIx_xAIx_EdDSAGenCtx.IsBusy = Busy;
    Crypto_xVIx_xAIx_EdDSAGenCtx.ResultError = Result;
    if((NULL_PTR != Result) && (NULL_PTR != Busy))
    {
      *Crypto_xVIx_xAIx_EdDSAGenCtx.IsBusy = TRUE;
      *Crypto_xVIx_xAIx_EdDSAGenCtx.ResultError = E_OK;
    }
    switch (Crypto_xVIx_xAIx_EdDSAGenCtx.GenState)
    {
      case CRYPTO_XVIX_XAIX_EDDSA_G_S_HASH_PRIV_K:
      Crypto_xVIx_xAIx_EdDSAProcessPrivKey();
      break;

      case CRYPTO_XVIX_XAIX_EDDSA_G_S_HASH_MESSAGE:
      Crypto_xVIx_xAIx_EdDSAGenHashMsg();
      break;

      case CRYPTO_XVIX_XAIX_EDDSA_G_S_LITTLE_R:
      Crypto_xVIx_xAIx_EdDSACalculateLittleR();
      break;

      case CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_SCALARR1:
      Crypto_xVIx_xAIx_EdDSACalculateScalars();
      break;

      case CRYPTO_XVIX_XAIX_EDDSA_G_S_LITTLE_K:
      Crypto_xVIx_xAIx_EdDSAGenCalculateLittleK();
      break;

      case CRYPTO_XVIX_XAIX_EDDSA_G_S_FINISH_SIG:
      Crypto_xVIx_xAIx_EdDSAFinishSignature();
      break;

      case CRYPTO_XVIX_XAIX_EDDSA_G_S_INITIALIZED:
      case CRYPTO_XVIX_XAIX_EDDSA_G_S_IDLE:
      break;

      /* CHECK: NOPARSE */
      /*All the possible states of the state machine are addressed by the cases above.*/
      default:
      {
        /*Defensive Programming - unreachable code. This default case is required by  MISRA-C:2012*/
      }
      break;
      /* CHECK: PARSE */
    }
    /* !LINKSTO SWS_Crypto_00143_CORRECTION_SWS_Crypto_00181, 1, SWS_Crypto_00144_CORRECTION, 1 */
    if(TRUE == Crypto_xVIx_xAIx_EdDSAGenCtx.CancelFlag)
    {
      if (
           (NULL_PTR != Crypto_xVIx_xAIx_EdDSAGenCtx.ResultError)
           && (NULL_PTR != Crypto_xVIx_xAIx_EdDSAGenCtx.IsBusy)
         )
      {
        *Crypto_xVIx_xAIx_EdDSAGenCtx.IsBusy = FALSE;
        *Crypto_xVIx_xAIx_EdDSAGenCtx.ResultError = CRYPTO_E_JOB_CANCELED;
        /* processingType is synchronous */
        /* !LINKSTO Crypto.Dsn.JobTransition.ActiveCancel2Idle, 1 */
        Crypto_xVIx_xAIx_EdDSAGenCtx.Job->jobState = CRYPTO_JOBSTATE_IDLE;
        SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
        Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_EdDSAGenCtx.ObjId].DriverObjectState =
        CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
        Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_EdDSAGenCtx.ObjId].CurrentJob =
        NULL_PTR;
        Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_EdDSAGenCtx.ObjId].SkipPeriodicMainFunction = TRUE;
        SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      }
      Crypto_xVIx_xAIx_EdDSAGenCtx.Crypto_Generic_Callback
      (
        Crypto_xVIx_xAIx_EdDSAGenCtx.Job,
        CRYPTO_E_JOB_CANCELED
      );
    }
    else
    {
      if(TRUE == Crypto_xVIx_xAIx_EdDSAGenCtx.CallbackFlag)
      {
        if (
             (NULL_PTR != Crypto_xVIx_xAIx_EdDSAGenCtx.ResultError)
             && (NULL_PTR != Crypto_xVIx_xAIx_EdDSAGenCtx.IsBusy)
           )
        {
          *Crypto_xVIx_xAIx_EdDSAGenCtx.IsBusy = FALSE;
          *Crypto_xVIx_xAIx_EdDSAGenCtx.ResultError =
          Crypto_xVIx_xAIx_EdDSAGenCtx.CallbackResult;
        }
        if(
            (
              CRYPTO_PROCESSING_ASYNC ==
              Crypto_xVIx_xAIx_EdDSAGenCtx.Job->jobPrimitiveInfo->processingType
            ) &&
            (
              (
                (
                  ((uint8)Crypto_xVIx_xAIx_EdDSAGenCtx.Job->jobPrimitiveInputOutput.mode) &
                    ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
                ) == ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
              ) ||
              (
                (
                  ((uint8)Crypto_xVIx_xAIx_EdDSAGenCtx.Job->jobPrimitiveInputOutput.mode) &
                  ((uint8) CRYPTO_OPERATIONMODE_STREAMSTART)
                ) == ( (uint8) CRYPTO_OPERATIONMODE_STREAMSTART)
              )
            ) && (CRYPTO_OPERATIONMODE_START == Crypto_xVIx_xAIx_EdDSAGenCtx.CurrentMode)
          )
        {
          /* This is only called if start finished successfully so we are in the correct state */
          (void)Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ED25519_NOT_SET_Update
                (
                  Crypto_xVIx_xAIx_EdDSAGenCtx.ObjId,
                  Crypto_xVIx_xAIx_EdDSAGenCtx.Job
                );
        }
        else
        {
          if
          (
            (
              CRYPTO_PROCESSING_ASYNC ==
                  Crypto_xVIx_xAIx_EdDSAGenCtx.Job->jobPrimitiveInfo->processingType
            ) &&
            (
              (
                (
                  ((uint8)Crypto_xVIx_xAIx_EdDSAGenCtx.Job->jobPrimitiveInputOutput.mode) &
                  ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
                ) == ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
              )
            ) && (CRYPTO_OPERATIONMODE_UPDATE == Crypto_xVIx_xAIx_EdDSAGenCtx.CurrentMode)
          )
          {
            (void)Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ED25519_NOT_SET_Finish
                  (
                    Crypto_xVIx_xAIx_EdDSAGenCtx.ObjId,
                    Crypto_xVIx_xAIx_EdDSAGenCtx.Job
                  );
          }
        }
        Crypto_xVIx_xAIx_EdDSAGenCtx.Crypto_Generic_Callback
        (
          Crypto_xVIx_xAIx_EdDSAGenCtx.Job,
          Crypto_xVIx_xAIx_EdDSAGenCtx.CallbackResult
        );
        Crypto_xVIx_xAIx_EdDSAGenCtx.CallbackFlag = FALSE;
      }
    }
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAGenCallback
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  Std_ReturnType                                               Result
)
{
  TS_PARAM_UNUSED(Job);
  Crypto_xVIx_xAIx_EdDSAGenCtx.ErrorCode = Result;


  /* This function shall always be called with Result E_OK, because the secondary algorithm is
  called through helpers which are always called with valid argument and shall never fail. */
  switch (Crypto_xVIx_xAIx_EdDSAGenCtx.GenState)
  {
    case CRYPTO_XVIX_XAIX_EDDSA_G_S_HASH_PRIV_K:
    Crypto_xVIx_xAIx_EdDSAPrivKeyCallback();
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_HASH_MESSAGE:
    Crypto_xVIx_xAIx_EdDSAHashMsgCallback();
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_LITTLE_R:
    Crypto_xVIx_xAIx_EdDSALittleRCallback();
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_G_S_LITTLE_K:
    Crypto_xVIx_xAIx_EdDSALittleKCallback();
    break;

    /* CHECK: NOPARSE */
    /* All the states that the state machine can have when the this callback is called are already
      covered in the cases above.*/
    default:
    {
      /* Defensive Programming - unreachable code. This default case is required by  MISRA-C:2012 */
    }
    break;
   /* CHECK: PARSE */
 }

}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATURE_GENERATE_ED25519_NOT_SET_CallBack
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
    VAR(uint8, CRYPTO_xVIx_xAIx_VAR)                           Result
)
{
  if(CRYPTO_PROCESSING_ASYNC == Job->jobPrimitiveInfo->processingType)
  {
    if(
        (
          CRYPTO_OPERATIONMODE_FINISH ==
          (CRYPTO_OPERATIONMODE_FINISH & Crypto_xVIx_xAIx_EdDSAGenCtx.CurrentMode)
        ) || (E_OK != Result)
      )
    {
      /* !LINKSTO SWS_Crypto_00025, 1 */
      /* !LINKSTO SWS_Crypto_00119, 1 */
      /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Idle, 1 */
      /* !LINKSTO Crypto.Dsn.JobTransition.ActiveError2Idle, 1 */
      Job->jobState = CRYPTO_JOBSTATE_IDLE;
      Crypto_xVIx_xAIx_EdDSAGenCtx.CurrentMode = CRYPTO_XVIX_XAIX_SIGNATURE_DEFAULT_CURRENT_MODE;
      /* !LINKSTO EB_Crypto_01050, 1 */
      SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_EdDSAGenCtx.ObjId].DriverObjectState =
      CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_EdDSAGenCtx.ObjId].CurrentJob = NULL_PTR;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_EdDSAGenCtx.ObjId].SkipPeriodicMainFunction = TRUE;
      SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      CryIf_CallbackNotification(Job, Result);
      Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ED25519_NOT_SET_Reset();
    }
    if(
        (
          CRYPTO_OPERATIONMODE_UPDATE ==
          (CRYPTO_OPERATIONMODE_UPDATE & Crypto_xVIx_xAIx_EdDSAGenCtx.CurrentMode)
        ) && (
               ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL) !=
               (
                 ((uint8)Crypto_xVIx_xAIx_EdDSAGenCtx.Job->jobPrimitiveInputOutput.mode) &
                 ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
               )
             )
      )
    {
      CryIf_CallbackNotification(Job, Result);
    }
  }
}

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if (CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*--EdDSA-----------------------------------------------------------------------------------------*/

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointDoubling
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Result,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
  P2VAR(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp
)
{
  /* Tmp1    Offset where G is stored. */
  const uint32 Tmp1 = (0U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  /* Tmp2    Offset where G is stored. */
  const uint32 Tmp2 = (1U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  /* Tmp3    Offset where G is stored. */
  const uint32 Tmp3 = (2U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  /* Tmp4    Offset where A is stored. */
  const uint32 Tmp4 = (3U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  /* Tmp5    Offset where B is stored. */
  const uint32 Tmp5 = (4U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);

  /* if the Point is infinity, no calculation is necessary */
  if (TRUE == Crypto_xVIx_xAIx_EdDSAProjectivePointOfInfinity(Value))
  {
    Result->XValue[0U] = 0U;
    Result->XValue[1U] = 0U;
    Result->YValue[0U] = 1U;
    Result->YValue[1U] = 1U;
    Result->ZValue[0U] = 1U;
    Result->ZValue[1U] = 1U;
  }
  else
  {
    /* Tmp1 = X1 + Y1 */
    Crypto_xVIx_xAIx_LNAdditionModular(
                              &Tmp[Tmp1],
                              Value->XValue,
                              Value->YValue,
                              Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].Prime
                            );
    /* Tmp2 = X1 * X1 */
#if (CRYPTO_XVIX_XAIX_LN_PLATFORM_DOUBLE_WORD_SUPPORT == STD_ON)
    Crypto_xVIx_xAIx_LNSquareComba(&Tmp[Tmp2], Value->XValue);
#else
    Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                        &Tmp[Tmp2],
                                        Value->XValue,
                                        Value->XValue,
                                        &CtxPtr->ECCtx.LNCtx
                                      );
#endif
    Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(&Tmp[Tmp2], &CtxPtr->ECCtx);

    /* Tmp3 = Y1 * Y1 */
#if (CRYPTO_XVIX_XAIX_LN_PLATFORM_DOUBLE_WORD_SUPPORT == STD_ON)
    Crypto_xVIx_xAIx_LNSquareComba(&Tmp[Tmp3], Value->YValue);
#else
    Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                        &Tmp[Tmp3],
                                        Value->YValue,
                                        Value->YValue,
                                        &CtxPtr->ECCtx.LNCtx
                                      );
#endif

    Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(&Tmp[Tmp3], &CtxPtr->ECCtx);

    /* a = -1 Tmp4 = Prime - 1*/
    (&Tmp[Tmp5])[0U] = 1U;
    (&Tmp[Tmp5])[1U] = 1U;

    Crypto_xVIx_xAIx_LNSubtractionModular(
                                 &Tmp[Tmp4],
                                 Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].Prime,
                                 &Tmp[Tmp5],
                                 Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].Prime
                               );

    /* E : Tmp4 = Tmp4 * Tmp2 */
    /* F = E + D: Tmp4 + Tmp3 */
    Crypto_xVIx_xAIx_EdDSAPointDoublingCalculateEF(CtxPtr, Tmp);

    /* B : Tmp1 ^ 2 = (X1 + Y1)^2 */
    Crypto_xVIx_xAIx_EdDSAPointDoublingCalculateB(Tmp, CtxPtr);

    /* Y3 = F * (E - D): Tmp5 * Tmp4 */
    Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                        Result->YValue,
                                        &Tmp[Tmp5],
                                        &Tmp[Tmp4],
                                        &CtxPtr->ECCtx.LNCtx
                                      );

    Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(Result->YValue, &CtxPtr->ECCtx);

    /* H: Tmp4 = Z1 * Z1 */
    Crypto_xVIx_xAIx_EdDSAPointDoublingCalculateH(
                                         Value,
                                         Tmp,
                                         CtxPtr
                                       );

    /* Z3 = F * J: Tmp5 * Tmp4 - Final Result */
    Crypto_xVIx_xAIx_EdDSAPointDoublingCalculateZ3(
                                          CtxPtr,
                                          Result,
                                          Tmp
                                        );

    /* X3 = Tmp1 * J - Final Result */
    Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                        Result->XValue,
                                        &Tmp[Tmp1],
                                        &Tmp[Tmp4],
                                        &CtxPtr->ECCtx.LNCtx
                                      );
    Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(Result->XValue, &CtxPtr->ECCtx);
  }
}

static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAProjectivePointOfInfinity
(
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Point
)
{
  boolean Result;

  /* In projective coordinates the Point of infinity corresponds to
   * the Point (X : Y : Z) = (0 : 1 : 1).
   */
  if (
       (0U == Point->XValue[0U]        ) &&
       ((1U == Point->YValue[0U]) && (1U == Point->YValue[1U])) &&
       ((1U == Point->ZValue[0U]) && (1U == Point->ZValue[1U]))
     )
  {
    Result = TRUE;
  }
  else
  {
    Result = FALSE;
  }
  return Result;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointAdditionCheckInfinity
(
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Result,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend1,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend2,
  P2VAR(boolean,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                Finished
)
{

  /* if the Addend1 is the Point of infinity, the Result is the Addend2 */
  if (TRUE == Crypto_xVIx_xAIx_EdDSAProjectivePointOfInfinity(Addend1))
  {

    TS_MemCpy(Result->XValue, Addend2->XValue, (Addend2->XValue[0U] + 1U) << 2);
    TS_MemCpy(Result->YValue, Addend2->YValue, (Addend2->YValue[0U] + 1U) << 2);
    TS_MemCpy(Result->ZValue, Addend2->ZValue, (Addend2->ZValue[0U] + 1U) << 2);

    *Finished = TRUE;
  }

  /* This function is called only in point addition function.
   * If the point addition is called in point multiplication, always is the first
   * addend equal to point of infinity.
   * Nevertheless I think we need to keep this error check for calculating R + (k * A)
   */
  /* if the Addend2 is the Point of infinity, the Result is the Addend1 */
  else if (TRUE == Crypto_xVIx_xAIx_EdDSAProjectivePointOfInfinity(Addend2))
  {
    TS_MemCpy(Result->XValue, Addend1->XValue, (Addend1->XValue[0U] + 1U) << 2);
    TS_MemCpy(Result->YValue, Addend1->YValue, (Addend1->YValue[0U] + 1U) << 2);
    TS_MemCpy(Result->ZValue, Addend1->ZValue, (Addend1->ZValue[0U] + 1U) << 2);

    *Finished = TRUE;
  }
  else
  {
    /* Nothing to do. */
  }

}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointAdditionCalculateA2
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp
)
{
  /* Tmp3    Offset where G is stored. */
  const uint32 Tmp3 = (2U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  /* Tmp4    Offset where A is stored. */
  const uint32 Tmp4 = (3U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  /* Tmp5    Offset where B is stored. */
  const uint32 Tmp5 = (4U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  /* Tmp6    Offset where F is stored. */
  const uint32 Tmp6 = (5U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);

  /* B = A^2: Tmp5 = Tmp4 * Tmp4 */

#if (CRYPTO_XVIX_XAIX_LN_PLATFORM_DOUBLE_WORD_SUPPORT == STD_ON)
  Crypto_xVIx_xAIx_LNSquareComba(&Tmp[Tmp5], &Tmp[Tmp4]);
#else
  Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                      &Tmp[Tmp5],
                                      &Tmp[Tmp4],
                                      &Tmp[Tmp4],
                                      &CtxPtr->ECCtx.LNCtx
                                    );
#endif
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(&Tmp[Tmp5], &CtxPtr->ECCtx);

  /* F: Tmp6 = Tmp5 - Tmp3 */
  Crypto_xVIx_xAIx_LNSubtractionModular(
                               &Tmp[Tmp6],
                               &Tmp[Tmp5],
                               &Tmp[Tmp3],
                               Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].Prime
                             );

  /* G: Tmp3 = Tmp5 + Tmp3 */
  Crypto_xVIx_xAIx_LNAdditionModular(
                            &Tmp[Tmp3],
                            &Tmp[Tmp5],
                            &Tmp[Tmp3],
                            Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].Prime
                          );

  CtxPtr->AdditionState = CRYPTO_XVIX_XAIX_EDDSA_ADD_S_7;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointAdditionCalculateZ3
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Result,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend1,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    Tmp
)
{
  const uint32 Tmp3 = (2U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  const uint32 Tmp5 = (4U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  const uint32 Tmp6 = (5U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);

  /* Z3 = Tmp6 * Tmp3 : Final Result */

  Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                      Result->ZValue,
                                      &Tmp[Tmp6],
                                      &Tmp[Tmp3],
                                      &CtxPtr->ECCtx.LNCtx
                                    );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(Result->ZValue, &CtxPtr->ECCtx);

  /* Tmp5 = X1 + Y1 */
  Crypto_xVIx_xAIx_LNAdditionModular(
                            &Tmp[Tmp5],
                            Addend1->XValue,
                            Addend1->YValue,
                            Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].Prime
                          );
  CtxPtr->AdditionState = CRYPTO_XVIX_XAIX_EDDSA_ADD_S_8;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointAdditionCalculateAG
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp
)
{
  const uint32 Tmp3 = (2U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  const uint32 Tmp4 = (3U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);

  Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                      &Tmp[Tmp4],
                                      &Tmp[Tmp4],
                                      &Tmp[Tmp3],
                                      &CtxPtr->ECCtx.LNCtx
                                    );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(&Tmp[Tmp4], &CtxPtr->ECCtx);
  CtxPtr->AdditionState = CRYPTO_XVIX_XAIX_EDDSA_ADD_S_12;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointAdditionCalculateInterimTmp5
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp
)
{
  const uint32 Tmp1 = (0U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  const uint32 Tmp2 = (1U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  const uint32 Tmp5 = (4U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  const uint32 Tmp6 = (5U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);

  /* Tmp5 = Tmp5 * Tmp6 */

  Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                      &Tmp[Tmp5],
                                      &Tmp[Tmp5],
                                      &Tmp[Tmp6],
                                      &CtxPtr->ECCtx.LNCtx
                                    );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(&Tmp[Tmp5], &CtxPtr->ECCtx);

  /* Tmp5 = Tmp5 - C */
  Crypto_xVIx_xAIx_LNSubtractionModular(
                               &Tmp[Tmp5],
                               &Tmp[Tmp5],
                               &Tmp[Tmp1],
                               Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].Prime
                             );

  /* Tmp5 = Tmp5 - Tmp2 */
  Crypto_xVIx_xAIx_LNSubtractionModular(
                               &Tmp[Tmp5],
                               &Tmp[Tmp5],
                               &Tmp[Tmp2],
                               Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].Prime
                             );

  CtxPtr->AdditionState = CRYPTO_XVIX_XAIX_EDDSA_ADD_S_10;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointAdditionCalculateAF
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Result,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend2,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    Tmp
)
{
  const uint32 Tmp4 = (3U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  const uint32 Tmp6 = (5U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);

  Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                      Result->XValue,
                                      &Tmp[Tmp4],
                                      &Tmp[Tmp6],
                                      &CtxPtr->ECCtx.LNCtx
                                    );

  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(Result->XValue, &CtxPtr->ECCtx);

  /* Tmp6 = X2 + Y2 */
  Crypto_xVIx_xAIx_LNAdditionModular(
                            &Tmp[Tmp6],
                            Addend2->XValue,
                            Addend2->YValue,
                            Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].Prime
                          );
  CtxPtr->AdditionState = CRYPTO_XVIX_XAIX_EDDSA_ADD_S_9;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointAdditionCalculateFinalX3
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Result,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp
)
{
  const uint32 Tmp1 = (0U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  const uint32 Tmp2 = (1U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  const uint32 Tmp5 = (4U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);

  Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                      Result->XValue,
                                      Result->XValue,
                                      &Tmp[Tmp5],
                                      &CtxPtr->ECCtx.LNCtx
                                    );

  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(Result->XValue, &CtxPtr->ECCtx);

  /* Tmp5 = D + C: Tmp2 + Tmp1 */
  Crypto_xVIx_xAIx_LNAdditionModular(
                            &Tmp[Tmp5],
                            &Tmp[Tmp2],
                            &Tmp[Tmp1],
                            Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].Prime
                          );

  CtxPtr->AdditionState = CRYPTO_XVIX_XAIX_EDDSA_ADD_S_11;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointDoublingCalculateZ3
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Result,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp
)
{
  const uint32 Tmp1 = (0U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  const uint32 Tmp2 = (1U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  const uint32 Tmp3 = (2U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  const uint32 Tmp4 = (3U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  const uint32 Tmp5 = (4U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);

  Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                      Result->ZValue,
                                      &Tmp[Tmp5],
                                      &Tmp[Tmp4],
                                      &CtxPtr->ECCtx.LNCtx
                                    );

  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(Result->ZValue, &CtxPtr->ECCtx);

  /* Tmp1 = B - C */
  Crypto_xVIx_xAIx_LNSubtractionModular(
                               &Tmp[Tmp1],
                               &Tmp[Tmp1],
                               &Tmp[Tmp2],
                               Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].Prime
                             );

  /* Tmp1 = Tmp1 - D */
  Crypto_xVIx_xAIx_LNSubtractionModular(
                               &Tmp[Tmp1],
                               &Tmp[Tmp1],
                               &Tmp[Tmp3],
                               Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].Prime
                             );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointDoublingCalculateH
(
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    Tmp,
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  CtxPtr
)
{
  const uint32 Tmp4 = (3U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  const uint32 Tmp5 = (4U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);

#if (CRYPTO_XVIX_XAIX_LN_PLATFORM_DOUBLE_WORD_SUPPORT == STD_ON)
  TS_PARAM_UNUSED(CtxPtr);
  Crypto_xVIx_xAIx_LNSquareComba(&Tmp[Tmp4], Value->ZValue);
#else
  Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                      &Tmp[Tmp4],
                                      Value->ZValue,
                                      Value->ZValue,
                                      &CtxPtr->ECCtx.LNCtx
                                    );
#endif

  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(&Tmp[Tmp4], &CtxPtr->ECCtx);

  /* Tmp4 = 2 * H */
  Crypto_xVIx_xAIx_LNAdditionModular(
                            &Tmp[Tmp4],
                            &Tmp[Tmp4],
                            &Tmp[Tmp4],
                            Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].Prime
                          );

  /* J: Tmp4 = Tmp5 - Tmp4 */
  Crypto_xVIx_xAIx_LNSubtractionModular(
                               &Tmp[Tmp4],
                               &Tmp[Tmp5],
                               &Tmp[Tmp4],
                               Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].Prime
                             );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointDoublingCalculateB
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  const uint32 Tmp1 = (0U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  const uint32 Tmp3 = (2U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  const uint32 Tmp4 = (3U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);

#if (CRYPTO_XVIX_XAIX_LN_PLATFORM_DOUBLE_WORD_SUPPORT == STD_ON)
  TS_PARAM_UNUSED(CtxPtr);
  Crypto_xVIx_xAIx_LNSquareComba(&Tmp[Tmp1], &Tmp[Tmp1]);
#else
  Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                      &Tmp[Tmp1],
                                      &Tmp[Tmp1],
                                      &Tmp[Tmp1],
                                      &CtxPtr->ECCtx.LNCtx
                                    );
#endif

  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(&Tmp[Tmp1], &CtxPtr->ECCtx);

  /* Tmp4 = E - D: Tmp4 - Tmp3 */
  Crypto_xVIx_xAIx_LNSubtractionModular(
                               &Tmp[Tmp4],
                               &Tmp[Tmp4],
                               &Tmp[Tmp3],
                               Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].Prime
                             );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointDoublingCalculateEF
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp
)
{
  const uint32 Tmp2 = (1U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  const uint32 Tmp3 = (2U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  const uint32 Tmp4 = (3U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  const uint32 Tmp5 = (4U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);

  /* E : Tmp4 = Tmp4 * Tmp2 */
  Crypto_xVIx_xAIx_LNMultiplyNumbers(&Tmp[Tmp4], &Tmp[Tmp4], &Tmp[Tmp2], &CtxPtr->ECCtx.LNCtx);
    Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(&Tmp[Tmp4], &CtxPtr->ECCtx);

  /* F = E + D: Tmp4 + Tmp3 */
  Crypto_xVIx_xAIx_LNAdditionModular(
                            &Tmp[Tmp5],
                            &Tmp[Tmp4],
                            &Tmp[Tmp3],
                            Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].Prime
                          );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointAdditionCalculateCDE
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  CtxPtr,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend1,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend2,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    Tmp
)
{
  /* Tmp1    Offset where C is stored. */
  const uint32 Tmp1 = (0U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  /* Tmp2    Offset where D is stored. */
  const uint32 Tmp2 = (1U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  /* Tmp3    Offset where E is stored. */
  const uint32 Tmp3 = (2U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);

  /* C: Tmp1 = X1 * X2 */
  Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                      &Tmp[Tmp1],
                                      Addend1->XValue,
                                      Addend2->XValue,
                                      &CtxPtr->ECCtx.LNCtx
                                    );

  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(&Tmp[Tmp1], &CtxPtr->ECCtx);

  /* D: Tmp2 = Y1 * Y2 */
  Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                      &Tmp[Tmp2],
                                      Addend1->YValue,
                                      Addend2->YValue,
                                      &CtxPtr->ECCtx.LNCtx
                                    );

  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(&Tmp[Tmp2], &CtxPtr->ECCtx);

  /* E: Tmp3 = DValue * Tmp1 */
  Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                      &Tmp[Tmp3],
                                      Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].Coeff1,
                                      &Tmp[Tmp1],
                                      &CtxPtr->ECCtx.LNCtx
                                    );

  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(&Tmp[Tmp3], &CtxPtr->ECCtx);

  /* E: Tmp3 = Tmp3 * Tmp2 */
  Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                      &Tmp[Tmp3],
                                      &Tmp[Tmp3],
                                      &Tmp[Tmp2],
                                      &CtxPtr->ECCtx.LNCtx
                                    );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(&Tmp[Tmp3], &CtxPtr->ECCtx);
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointEncodingStart
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  CtxPtr->EncodingState = CRYPTO_XVIX_XAIX_EDDSA_ENC_S_START;
}

static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointEncoding
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Value,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                 Result,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                 Tmp1Array,
  uint32                                                    Tmp1ArrayLength,
  uint32                                                    ResultLength
)
{
  boolean Finished;
  uint8 X0;

  Finished = FALSE;

  switch(CtxPtr->EncodingState)
  {
    case CRYPTO_XVIX_XAIX_EDDSA_ENC_S_START:
    Crypto_xVIx_xAIx_EdDSAPointConvertProj2AffStart(CtxPtr);
    CtxPtr->EncodingState = CRYPTO_XVIX_XAIX_EDDSA_ENC_S_1;
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_ENC_S_1:
    if (
         TRUE ==
         Crypto_xVIx_xAIx_EdDSAPointConvertProj2Aff(
                                           CtxPtr,
                                           Value,
                                           &Tmp[0U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS],
                                           &Tmp[1U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS],
                                           &Tmp[2U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS]
                                         )
       )
    {
      /* convert the YValue in little endian byte array */
      /* Call shall never fail because we have enough memory allocated in the context
       * to store the byte representation of the LN. */
      (void)Crypto_xVIx_xAIx_LNNumberToLEByteArray(Value->YValue, Result, &ResultLength);
      CtxPtr->EncodingState = CRYPTO_XVIX_XAIX_EDDSA_ENC_S_2;
    }
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_ENC_S_2:

    /* Call shall never fail because we have enough memory allocated in the context
     * to store the byte representation of the LN. */
    (void) Crypto_xVIx_xAIx_LNNumberToLEByteArray(Value->XValue, Tmp1Array, &Tmp1ArrayLength);
    /* the least significant bit of the x-coordinate */
    X0 = (uint8)(Tmp1Array[0U] & 0x01U);

    if(0U != X0)
    {
      Result[(Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].BValue >> 3U) - 1U] |= (uint8)((uint8)X0 << 7U);
    }
    Finished = TRUE;

    break;

    /* CHECK: NOPARSE */
    /*All the possible states have been addressed in the cases above.*/
    /* Defensive Programming - unreachable code. This default case is required by  MISRA-C:2012 */
    default:
    Finished = TRUE;
    break;
   /* CHECK: PARSE */
  }
  return Finished;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointAddition
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Result,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend1,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend2,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    Tmp
)
{
  boolean Finished = FALSE;

  Crypto_xVIx_xAIx_EdDSAPointAdditionCheckInfinity(
                                                    Result,
                                                    Addend1,
                                                    Addend2,
                                                    &Finished
                                                  );
  if(FALSE == Finished)
  {
    /* calculate C, D and E */
    Crypto_xVIx_xAIx_EdDSAPointAdditionCalculateCDE(
                                                     CtxPtr,
                                                     Addend1,
                                                     Addend2,
                                                     Tmp
                                                   );

    /* A: Tmp4 = Z1 * Z2 */
    Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                        &Tmp[3U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS],
                                        Addend1->ZValue,
                                        Addend2->ZValue,
                                        &CtxPtr->ECCtx.LNCtx
                                      );

    Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(&Tmp[3U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS], &CtxPtr->ECCtx);

    Crypto_xVIx_xAIx_EdDSAPointAdditionCalculateA2(CtxPtr, Tmp);

    /* Z3 = Tmp6 * Tmp3 : Final Result */
    Crypto_xVIx_xAIx_EdDSAPointAdditionCalculateZ3(
                                                    CtxPtr,
                                                    Result,
                                                    Addend1,
                                                    Tmp
                                                  );

    /* X3 = A * F: Tmp4 * Tmp6 */
    Crypto_xVIx_xAIx_EdDSAPointAdditionCalculateAF(
                                                    CtxPtr,
                                                    Result,
                                                    Addend2,
                                                    Tmp
                                                  );

    /* Tmp5 = Tmp5 * Tmp6 */
    Crypto_xVIx_xAIx_EdDSAPointAdditionCalculateInterimTmp5(CtxPtr, Tmp);

    /* X3 = X3 * Tmp5 - Final Result */
    Crypto_xVIx_xAIx_EdDSAPointAdditionCalculateFinalX3(
                                                         CtxPtr,
                                                         Result,
                                                         Tmp
                                                       );

    /* Tmp4 = A * G: Tmp4 * Tmp3 */
    Crypto_xVIx_xAIx_EdDSAPointAdditionCalculateAG(CtxPtr, Tmp);

    /* Y3 = Tmp4 * Tmp5 - Final Result */

    Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                        Result->YValue,
                                        &Tmp[3U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS],
                                        &Tmp[4U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS],
                                        &CtxPtr->ECCtx.LNCtx
                                      );

    Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(Result->YValue, &CtxPtr->ECCtx);
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointMultiplication
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Result,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Point,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Multiplicand,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    Tmp

)
{
  /* first the Result shall be set to the Point of infinity. */
  Result->XValue[0U] = 0U;
  Result->XValue[1U] = 0U;
  Result->YValue[0U] = 1U;
  Result->YValue[1U] = 1U;
  Result->ZValue[0U] = 1U;
  Result->ZValue[1U] = 1U;

  CtxPtr->ECCtx.CurrentBit = Crypto_xVIx_xAIx_LNBitLengthOfNumber(Multiplicand);
  while(0U < CtxPtr->ECCtx.CurrentBit)
  {
    CtxPtr->ECCtx.CurrentBit--;
    Crypto_xVIx_xAIx_EdDSAPointDoubling(
                                         CtxPtr,
                                         Result,
                                         Result,
                                         Tmp
                                       );

    if (1U == CRYPTO_XVIX_XAIX_LN_GET_BIT(Multiplicand, CtxPtr->ECCtx.CurrentBit))
    {
      Crypto_xVIx_xAIx_EdDSAPointAddition(
                                           CtxPtr,
                                           Result,
                                           Result,
                                           Point,
                                           Tmp
                                         );
    }
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointConvertProj2AffStart
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  CtxPtr->ConvertProj2AffState = CRYPTO_XVIX_XAIX_EDDSA_CONV_S_START;
}

static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointConvertProj2Aff
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Point,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp1,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp2,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp3
)
{
  boolean Finished = FALSE;

  switch(CtxPtr->ConvertProj2AffState)
  {
    case CRYPTO_XVIX_XAIX_EDDSA_CONV_S_START:
    /* z-coordinate = 0 then the conversion is the infinity */

    /* The Proj2Aff function is called just once, at the encoding of A = s * B.
     * As the Point to infinity is a neutral Point on the eliptic curve and B is
     * given as a "constant" from the algorithm specification we cannot have a Point
     * to infinity as an argument. This part is not removed as it might be useful
     * in future extensions. */
    if (0U == Point->ZValue[0U])
    {
      Point->XValue[0U] = 0U;
      Point->XValue[1U] = 0U;
      Point->YValue[0U] = 1U;
      Point->YValue[1U] = 1U;

      Finished = TRUE;
    }
    else

    {
      CtxPtr->ConvertProj2AffState = CRYPTO_XVIX_XAIX_EDDSA_CONV_S_INVERT;
    }
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_CONV_S_INVERT:
    /* Z = Z^(-1) */
    Crypto_xVIx_xAIx_LNInversion(
                                  Point->ZValue,
                                  Point->ZValue,
                                  Tmp1,
                                  Tmp2,
                                  Tmp3,
                                  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].Prime,
                                  &CtxPtr->ECCtx.LNCtx
                                );
    CtxPtr->ConvertProj2AffState = CRYPTO_XVIX_XAIX_EDDSA_CONV_S_X;
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_CONV_S_X:
    /* X = X * Z */
    Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                        Point->XValue,
                                        Point->XValue,
                                        Point->ZValue,
                                        &CtxPtr->ECCtx.LNCtx
                                      );

    Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(Point->XValue, &CtxPtr->ECCtx);
    CtxPtr->ConvertProj2AffState = CRYPTO_XVIX_XAIX_EDDSA_CONV_S_Y;
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_CONV_S_Y:
    /* Y = Y * Z */
    Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                        Point->YValue,
                                        Point->YValue,
                                        Point->ZValue,
                                        &CtxPtr->ECCtx.LNCtx
                                      );
    Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(Point->YValue, &CtxPtr->ECCtx);
    Finished = TRUE;
    break;

    /* CHECK: NOPARSE */
    /*All the possible states for this state machine are addressed in the cases above.*/
    /* Defensive Programming - unreachable code. This default case is required by  MISRA-C:2012 */
    default:
    Finished = TRUE;
    break;
    /* CHECK: PARSE */
  }
  return Finished;
}

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*--EdDSA-Verify----------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON)

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ED25519_NOT_SET_Reset
(
  void
)
{
  Crypto_xVIx_xAIx_AL_HASH_SHA2_NOT_SET_Reset();
  SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
  TS_MemBZero(
               &Crypto_xVIx_xAIx_EdDSAVrfyCtx,
               sizeof(Crypto_xVIx_xAIx_EdDSAVrfyCtx)
             );
  SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAVrfyCallback
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  Std_ReturnType                                   Result
)
{
  TS_PARAM_UNUSED(Job);
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.ErrorCode = Result;

  switch (Crypto_xVIx_xAIx_EdDSAVrfyCtx.VrfyState)
  {
    case CRYPTO_XVIX_XAIX_EDDSA_V_S_HASH_MSG:
    switch (Crypto_xVIx_xAIx_EdDSAVrfyCtx.HashMsgState)
    {
      case CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_INIT_HASH_MESSAGE:
      Crypto_xVIx_xAIx_EdDSAVrfyCtx.HashMsgState = CRYPTO_XVIX_XAIX_EDDSA_V_S_UPDATE_MESSAGE;
      break;

      case CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_UPDATE_MESSAGE:
      Crypto_xVIx_xAIx_EdDSAVrfyCtx.VrfyState = CRYPTO_XVIX_XAIX_EDDSA_V_S_INITIALIZED;
      Crypto_xVIx_xAIx_EdDSAVrfyCtx.HashMsgState = CRYPTO_XVIX_XAIX_EDDSA_V_S_INIT_HASH_MESSAGE;
      Crypto_xVIx_xAIx_EdDSAVrfyCtx.ErrorCode = E_OK;

      if(
          (CRYPTO_PROCESSING_SYNC == Crypto_xVIx_xAIx_EdDSAVrfyCtx.ProcessingType) &&
          (NULL_PTR != Crypto_xVIx_xAIx_EdDSAVrfyCtx.IsBusy)
        )
      {
        *Crypto_xVIx_xAIx_EdDSAVrfyCtx.IsBusy = FALSE;
      }

      Crypto_xVIx_xAIx_EdDSAVrfyCtx.CallbackFlag = TRUE;
      Crypto_xVIx_xAIx_EdDSAVrfyCtx.CallbackResult = Crypto_xVIx_xAIx_EdDSAVrfyCtx.ErrorCode;
      break;

      case CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_FINISH_MESSAGE:
      Crypto_xVIx_xAIx_EdDSAVrfyCtx.VrfyState = CRYPTO_XVIX_XAIX_EDDSA_V_S_VALIDATE_SIG_COMPONENTS;
      Crypto_xVIx_xAIx_EdDSAVrfyCtx.ValSigCmpState = CRYPTO_XVIX_XAIX_EDDSA_V_S_CONVERT_S;
      Crypto_xVIx_xAIx_EdDSAVrfyCtx.HashMsgState = CRYPTO_XVIX_XAIX_EDDSA_V_S_INIT_HASH_MESSAGE;
      break;

      /* CHECK: NOPARSE */
      /*All the possible (wait) states are addressed in the cases above.*/
      default:
      {
        /* Defensive Programming - unreachable code. This default case is required by  MISRA-C:2012 */
      }
      break;
      /* CHECK: PARSE */
    }
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_V_S_LITTLE_K:
    switch (Crypto_xVIx_xAIx_EdDSAVrfyCtx.LittleKState)
    {
      case CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_INIT:
      Crypto_xVIx_xAIx_EdDSAVrfyCtx.LittleKState = CRYPTO_XVIX_XAIX_EDDSA_V_S_UPDATE_DOM2;
      break;

      case CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_UPDATE_DOM2:
      Crypto_xVIx_xAIx_EdDSAVrfyCtx.LittleKState = CRYPTO_XVIX_XAIX_EDDSA_V_S_UPDATE_ENC_R;
      break;

      case CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_UPDATE_ENC_R:
      Crypto_xVIx_xAIx_EdDSAVrfyCtx.LittleKState = CRYPTO_XVIX_XAIX_EDDSA_V_S_UPDATE_ENC_A;
      break;

      case CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_UPDATE_ENC_A:
      Crypto_xVIx_xAIx_EdDSAVrfyCtx.LittleKState = CRYPTO_XVIX_XAIX_EDDSA_V_S_UPDATE_M;
      break;

      case CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_UPDATE_M:
      Crypto_xVIx_xAIx_EdDSAVrfyCtx.LittleKState = CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH;
      break;

      case CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_FINISH:
      Crypto_xVIx_xAIx_EdDSAVrfyCtx.LittleKState = CRYPTO_XVIX_XAIX_EDDSA_V_S_INIT_SIGN_HASH;
      Crypto_xVIx_xAIx_EdDSAVrfyCtx.VrfyState = CRYPTO_XVIX_XAIX_EDDSA_V_S_GROUP_EQUATIONS;
      break;


      /* CHECK: NOPARSE */
      /*All the posible (wait) states are addresed in the above cases.*/
      default:
      {
        /* Defensive Programming - unreachable code. This default case is required by  MISRA-C:2012 */
      }
      break;
      /* CHECK: PARSE */
    }
    break;

    /* CHECK: NOPARSE */
    /*All the states that the state machine can have, when this callback is called, are covered in
      the cases above.*/
    default:
    {
      /* Defensive Programming - unreachable code. This default case is required by  MISRA-C:2012 */
    }
    break;
    /* CHECK: PARSE */
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAVrfyDecodePublicKey
(
  void
)
{
  Crypto_VerifyResultType DecodeResult = CRYPTO_E_VER_NOT_OK;

  switch (Crypto_xVIx_xAIx_EdDSAVrfyCtx.DecPubKeyState)
  {
    case CRYPTO_XVIX_XAIX_EDDSA_V_S_CONVERT_PUBLICKEY:
    /* Copy Tmp1Array to Tmp2ArrayLength, as Crypto_xVIx_xAIx_EdDSAPointDecoding
     * changes the value of the byte array
     */
    Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp2ArrayLength = Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp1ArrayLength;
    TS_MemCpy(
        Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp2Array,
        Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp1Array,
        Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp2ArrayLength
        );

    /* Start the point decoding */
    Crypto_xVIx_xAIx_EdDSAPointDecodingStart(&Crypto_xVIx_xAIx_EdDSAVrfyCtx.EdDSACtx);

    Crypto_xVIx_xAIx_EdDSAVrfyCtx.DecPubKeyState = CRYPTO_XVIX_XAIX_EDDSA_V_S_DECODE_PUBLICKEY;
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_V_S_DECODE_PUBLICKEY:
    /* Check if the public key is a valid point on the Edwards curve */
    if (
         TRUE ==
         Crypto_xVIx_xAIx_EdDSAPointDecoding(
                                    &Crypto_xVIx_xAIx_EdDSAVrfyCtx.EdDSACtx,
                                    &Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp1Point,
                                    Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp,
                                    &DecodeResult,
                                    Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp2Array,
                                    Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp2ArrayLength
                                  )
       )
    {
      if (CRYPTO_E_VER_OK == DecodeResult)
      {
        /* Point A decoded successfully */
        Crypto_xVIx_xAIx_EdDSAVrfyCtx.DecPubKeyState = CRYPTO_XVIX_XAIX_EDDSA_V_S_CONVERT_PUBLICKEY;

        Crypto_xVIx_xAIx_EdDSAVrfyCtx.VrfyState = CRYPTO_XVIX_XAIX_EDDSA_V_S_INITIALIZED;
        Crypto_xVIx_xAIx_EdDSAVrfyCtx.CallbackFlag = TRUE;
        Crypto_xVIx_xAIx_EdDSAVrfyCtx.CallbackResult = Crypto_xVIx_xAIx_EdDSAVrfyCtx.ErrorCode;

        if((NULL_PTR != Crypto_xVIx_xAIx_EdDSAVrfyCtx.ResultError) && (NULL_PTR != Crypto_xVIx_xAIx_EdDSAVrfyCtx.IsBusy))
        {
          *Crypto_xVIx_xAIx_EdDSAVrfyCtx.IsBusy = FALSE;
          *Crypto_xVIx_xAIx_EdDSAVrfyCtx.ResultError = Crypto_xVIx_xAIx_EdDSAVrfyCtx.ErrorCode;
        }
      }
      else
      {
        CRYPTO_XVIX_XAIX_EDDSA_V_HANDLE_FAILING_PUB_KEY_DECODING;
      }
    }
    break;

    /* CHECK: NOPARSE */
    /*All the possible states are covered in the cases above.*/
    default:
    {
      /* Defensive Programming - unreachable code. This default case is required by  MISRA-C:2012 */
    }
    break;
    /* CHECK: PARSE */
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAVrfyHashMsg
(
  void
)
{
  switch (Crypto_xVIx_xAIx_EdDSAVrfyCtx.HashMsgState)
  {
    case CRYPTO_XVIX_XAIX_EDDSA_V_S_INIT_HASH_MESSAGE:
    Crypto_xVIx_xAIx_EdDSAVrfyCtx.CurrentMode = CRYPTO_OPERATIONMODE_UPDATE;
    Crypto_xVIx_xAIx_EdDSAVrfyCtx.HashMsgState =
    CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_INIT_HASH_MESSAGE;
    Crypto_xVIx_xAIx_SHA2512HelperStart(
                                         Crypto_xVIx_xAIx_EdDSAVrfyCtx.ObjId,
                                         &Crypto_xVIx_xAIx_EdDSAVrfyCallback,
                                         Crypto_xVIx_xAIx_EdDSAVrfyCtx.Job,
                                         Crypto_xVIx_xAIx_EdDSAVrfyCtx.ProcessingType
                                       );
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_V_S_UPDATE_MESSAGE:
    Crypto_xVIx_xAIx_EdDSAVrfyCtx.HashMsgState = CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_UPDATE_MESSAGE;
    /* we are in the correct state when this is called so it can not fail */
    (void)Crypto_xVIx_xAIx_SHA2512HelperUpdate(
                                         Crypto_xVIx_xAIx_EdDSAVrfyCtx.Message,
                                         Crypto_xVIx_xAIx_EdDSAVrfyCtx.MessageLength,
                                         Crypto_xVIx_xAIx_EdDSAVrfyCtx.Job->jobPrimitiveInfo->processingType
                                       );
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_MESSAGE:
    /* HashMessage = H(M) */
    Crypto_xVIx_xAIx_EdDSAVrfyCtx.CurrentMode = CRYPTO_OPERATIONMODE_FINISH;
    if(CRYPTO_XVIX_XAIX_EDDSA_SIGN_SIZE > Crypto_xVIx_xAIx_EdDSAVrfyCtx.SignatureLength)
    {
      *Crypto_xVIx_xAIx_EdDSAVrfyCtx.ResultPtr = CRYPTO_E_VER_NOT_OK;
      Crypto_xVIx_xAIx_EdDSAVrfyCtx.CallbackFlag = TRUE;
      Crypto_xVIx_xAIx_EdDSAVrfyCtx.CallbackResult = CRYPTO_E_SMALL_BUFFER;
    }
    else if (CRYPTO_XVIX_XAIX_EDDSA_SIGN_SIZE < Crypto_xVIx_xAIx_EdDSAVrfyCtx.SignatureLength)
    {
      *Crypto_xVIx_xAIx_EdDSAVrfyCtx.ResultPtr = CRYPTO_E_VER_NOT_OK;
      Crypto_xVIx_xAIx_EdDSAVrfyCtx.CallbackFlag = TRUE;
      Crypto_xVIx_xAIx_EdDSAVrfyCtx.CallbackResult = E_NOT_OK;
    }
    else
    {
      Crypto_xVIx_xAIx_EdDSAVrfyCtx.HashResultLength = sizeof(Crypto_xVIx_xAIx_EdDSAVrfyCtx.HashMessage);
      Crypto_xVIx_xAIx_EdDSAVrfyCtx.HashMsgState = CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_FINISH_MESSAGE;
      /* we are in the correct state when this is called so it can not fail */
      (void)Crypto_xVIx_xAIx_SHA2512HelperFinish(
                                           Crypto_xVIx_xAIx_EdDSAVrfyCtx.HashMessage,
                                           &Crypto_xVIx_xAIx_EdDSAVrfyCtx.HashResultLength,
                                           FALSE,
                                           Crypto_xVIx_xAIx_EdDSAVrfyCtx.Job->jobPrimitiveInfo->processingType
                                         );
    }
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_INIT_HASH_MESSAGE:
    case CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_UPDATE_MESSAGE:
    case CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_FINISH_MESSAGE:
    /* gets called only if processing Type is ASYNC */
    Crypto_xVIx_xAIx_SHA2512HelperMainFunction();
    break;

    /* CHECK: NOPARSE */
    /*All the possible states are addresses in the cases above.*/
    default:
    {
      /* Defensive Programming - unreachable code. This default case is required by  MISRA-C:2012 */
    }
    break;
    /* CHECK: PARSE */
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAVrfyValSigCmp
(
  void
)
{
  Crypto_VerifyResultType DecodeResult = CRYPTO_E_VER_NOT_OK;
  Crypto_xVIx_xAIx_ReturnType ComparisonZero;
  Crypto_xVIx_xAIx_ReturnType ComparisonL;

  switch (Crypto_xVIx_xAIx_EdDSAVrfyCtx.ValSigCmpState)
  {
    case CRYPTO_XVIX_XAIX_EDDSA_V_S_CONVERT_S:
    /* Convert S to Long Number format */
    /* we have enough memory allocated in the context to store the byte representation of the LN. */
    (void)Crypto_xVIx_xAIx_LNLEByteArrayToNumber
    (
      &(Crypto_xVIx_xAIx_EdDSAVrfyCtx.Signature[CRYPTO_XVIX_XAIX_EDDSA_V_SIG_S_START_POSITION]),
      CRYPTO_XVIX_XAIX_EDDSA_V_ARRAY_LEN,
      &Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp[7U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS],
      CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS
    );
    Crypto_xVIx_xAIx_EdDSAVrfyCtx.ValSigCmpState = CRYPTO_XVIX_XAIX_EDDSA_V_S_CHECK_S;
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_V_S_CHECK_S:
    /* Use the second word of Tmp as a zero value Long Number */
    Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp[CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS] = 1U;
    Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp[CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS + 1U] = 0U;

    /* Compare S with zero */
    ComparisonZero = Crypto_xVIx_xAIx_LNCompareNumbers
                     (
                      &(Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp[7U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS]),
                      &(Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp[CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS])
                     );
    /* Compare S with L */
    ComparisonL = Crypto_xVIx_xAIx_LNCompareNumbers
                  (
                    &(Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp[7U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS]),
                    &(Crypto_xVIx_xAIx_EllipticCurveInfo[Crypto_xVIx_xAIx_EdDSAVrfyCtx.EdDSACtx.ECCtx.CurveIndex].Modulus[0U])
                  );

    /* Check if S is in the range [0; L) */
    if (
        ((CRYPTO_XVIX_XAIX_E_EQUAL == ComparisonZero) || (CRYPTO_XVIX_XAIX_E_LARGER == ComparisonZero))
         &&
        (CRYPTO_XVIX_XAIX_E_SMALLER == ComparisonL)
       )
    {
      Crypto_xVIx_xAIx_EdDSAVrfyCtx.ValSigCmpState = CRYPTO_XVIX_XAIX_EDDSA_V_S_COPY_R;
    }
    else
    {
      CRYPTO_XVIX_XAIX_EDDSA_V_HANDLE_FAILING_VALIDATE_SIG_CMP;
    }
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_V_S_COPY_R:
    /* Tmp2Array : first part of Signature, ENC(R) */
    Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp2ArrayLength = (uint32)((uint32)Crypto_xVIx_xAIx_EllipticCurveInfo[Crypto_xVIx_xAIx_EdDSAVrfyCtx.EdDSACtx.ECCtx.CurveIndex].BValue >> 3U);

    TS_MemCpy(
        Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp2Array,
        Crypto_xVIx_xAIx_EdDSAVrfyCtx.Signature,
        Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp2ArrayLength
        );

    Crypto_xVIx_xAIx_EdDSAPointDecodingStart(&Crypto_xVIx_xAIx_EdDSAVrfyCtx.EdDSACtx);
    Crypto_xVIx_xAIx_EdDSAVrfyCtx.ValSigCmpState = CRYPTO_XVIX_XAIX_EDDSA_V_S_DECODE_R;
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_V_S_DECODE_R:
    if (
         TRUE ==
         Crypto_xVIx_xAIx_EdDSAPointDecoding(
                                    &Crypto_xVIx_xAIx_EdDSAVrfyCtx.EdDSACtx,
                                    &Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp2Point,
                                    Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp,
                                    &DecodeResult,
                                    Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp2Array,
                                    Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp2ArrayLength
                                  )
       )
    {
      if (CRYPTO_E_VER_OK == DecodeResult)
      {
        /* Point R decoded successfully, proceed with calculating k */
        Crypto_xVIx_xAIx_EdDSAVrfyCtx.ValSigCmpState = CRYPTO_XVIX_XAIX_EDDSA_V_S_CONVERT_S;

        Crypto_xVIx_xAIx_EdDSAVrfyCtx.VrfyState = CRYPTO_XVIX_XAIX_EDDSA_V_S_LITTLE_K;
        Crypto_xVIx_xAIx_EdDSAVrfyCtx.LittleKState = CRYPTO_XVIX_XAIX_EDDSA_V_S_INIT_SIGN_HASH;
      }
      else
      {
        CRYPTO_XVIX_XAIX_EDDSA_V_HANDLE_FAILING_VALIDATE_SIG_CMP;
      }
    }
    break;

    /* CHECK: NOPARSE */
    /*All the possible states are addressed in the cases above.*/
    default:
    {
      /* Defensive Programming - unreachable code. This default case is required by  MISRA-C:2012 */
    }
    break;
    /* CHECK: PARSE */
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAFinishEquation
(
  void
)
{
  switch (Crypto_xVIx_xAIx_EdDSAVrfyCtx.FinishEquationState)
  {
    case CRYPTO_XVIX_XAIX_EDDSA_V_S_HASH:
    Crypto_xVIx_xAIx_EdDSAHashByteToNum();
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_V_S_HASH_RED:
    /* Tmp6 = hashValue % LValue */

    Crypto_xVIx_xAIx_LNModulo
    (
      &Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp[6U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS],
      Crypto_xVIx_xAIx_EllipticCurveInfo[Crypto_xVIx_xAIx_EdDSAVrfyCtx.EdDSACtx.ECCtx.CurveIndex].Modulus,
      &Crypto_xVIx_xAIx_EdDSAVrfyCtx.EdDSACtx.ECCtx.LNCtx
    );
    Crypto_xVIx_xAIx_EdDSAVrfyCtx.FinishEquationState = CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_EQUATION_1;
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_EQUATION_1:
    Crypto_xVIx_xAIx_EdDSACalculateKA();
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_EQUATION_2:
    /* Tmp1Point = R + (k * A): Tmp2Point + Tmp3Point */
    Crypto_xVIx_xAIx_EdDSACalculateRkA();
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_EQUATION_2_ENC:
    /* Tmp1Array = ENC(R + (k * A)) */
    if (
         TRUE ==
         Crypto_xVIx_xAIx_EdDSAPointEncoding(
                                    &Crypto_xVIx_xAIx_EdDSAVrfyCtx.EdDSACtx,
                                    &Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp4Point,
                                    Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp,
                                    Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp1Array,
                                    Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp2Array,
                                    Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp1ArrayLength,
                                    Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp2ArrayLength
                                  )
       )
    {
      Crypto_xVIx_xAIx_EdDSAVrfyCtx.FinishEquationState = CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_EQUATION_3;
    }
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_EQUATION_3:
    /* Tmp2Point = S * B: Tmp4 * Base Point */
    Crypto_xVIx_xAIx_EdDSACalculateSB();
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_EQUATION_3_ENC:
    /* Tmp3Array = ENC(S * B) */
    Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp3ArrayLength = (uint32)((uint32)Crypto_xVIx_xAIx_EllipticCurveInfo[Crypto_xVIx_xAIx_EdDSAVrfyCtx.EdDSACtx.ECCtx.CurveIndex].BValue >> 3U);

    if (
         TRUE ==
         Crypto_xVIx_xAIx_EdDSAPointEncoding(
                                    &Crypto_xVIx_xAIx_EdDSAVrfyCtx.EdDSACtx,
                                    &Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp3Point,
                                    Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp,
                                    Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp3Array,
                                    Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp2Array,
                                    Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp3ArrayLength,
                                    Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp2ArrayLength
                                  )
       )
    {
      Crypto_xVIx_xAIx_EdDSAVrfyCtx.FinishEquationState = CRYPTO_XVIX_XAIX_EDDSA_V_S_HASH;
      Crypto_xVIx_xAIx_EdDSAVrfyCtx.VrfyState = CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_COMPARE;
    }
    break;

    /* CHECK: NOPARSE */
    /*All the possible states are addressed in the cases above.*/
    default:
    {
      /* Defensive Programming - unreachable code. This default case is required by  MISRA-C:2012 */
    }
    break;
    /* CHECK: PARSE */
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSACalculateSB
(
  void
)
{
  Crypto_xVIx_xAIx_EdDSAPointMultiplication(
                                             &Crypto_xVIx_xAIx_EdDSAVrfyCtx.EdDSACtx,
                                             &Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp3Point,
                                             &(Crypto_xVIx_xAIx_EllipticCurveInfo[Crypto_xVIx_xAIx_EdDSAVrfyCtx.EdDSACtx.ECCtx.CurveIndex].P),
                                             &Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp[7U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS],
                                             Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp
                                           );

  Crypto_xVIx_xAIx_EdDSAPointEncodingStart(&Crypto_xVIx_xAIx_EdDSAVrfyCtx.EdDSACtx);
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.FinishEquationState = CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_EQUATION_3_ENC;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSACalculateKA
(
  void
)
{
  /* Tmp3Point = k * A: Tmp6 * Tmp1Point */

  Crypto_xVIx_xAIx_EdDSAPointMultiplication
  (
    &Crypto_xVIx_xAIx_EdDSAVrfyCtx.EdDSACtx,
    &Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp3Point,
    &Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp1Point,
    &Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp[6U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS],
    Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp
  );
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.FinishEquationState = CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_EQUATION_2;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAHashByteToNum
(
  void
)
{

  /* call shall never fail because we have enough memory allocated in the context
   * to store the byte representation of the LN. */
  (void)Crypto_xVIx_xAIx_LNLEByteArrayToNumber
  (
    Crypto_xVIx_xAIx_EdDSAVrfyCtx.HashResult,
    Crypto_xVIx_xAIx_EdDSAVrfyCtx.HashResultLength,
    &Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp[6U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS],
    CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS
  );
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.FinishEquationState = CRYPTO_XVIX_XAIX_EDDSA_V_S_HASH_RED;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSACalculateRkA
(
  void
)
{
  Crypto_xVIx_xAIx_EdDSAPointAddition(
                                       &Crypto_xVIx_xAIx_EdDSAVrfyCtx.EdDSACtx,
                                       &Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp4Point,
                                       &Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp2Point,
                                       &Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp3Point,
                                       Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp
                                     );
  Crypto_xVIx_xAIx_EdDSAPointEncodingStart(&Crypto_xVIx_xAIx_EdDSAVrfyCtx.EdDSACtx);
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.FinishEquationState = CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_EQUATION_2_ENC;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAValidateSignature
(
  void
)
{
  Crypto_VerifyResultType     Result;
  Crypto_xVIx_xAIx_LNWordType CompareResult = 0U;
  uint32                      i;

  /*
   *  S * B == R + (k * A): Tmp3Point == Tmp4Point
   *  The other possibility would be:
   *  Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp3Array == Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp1Array
   *  The points are converted to Affine coordinate. There is no need to
   *  check the z-coordinate of the points.
   *
   */
  Result = CRYPTO_E_VER_OK;

  /* Defensive Programming - unreachable code.
   * This default case is required by  MISRA-C:2012
   * An invalid public Key that it's still a valid Point on the curve need to be found.
   */
  if (
       (Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp3Point.XValue[0U] != Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp4Point.XValue[0U]) ||
       (Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp3Point.YValue[0U] != Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp4Point.YValue[0U])
     )
  {
    Result = CRYPTO_E_VER_NOT_OK;
  }
  else
  {
    for(i = 1U; Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp3Point.XValue[0U] >= i; i++)
    {
      CompareResult |= Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp3Point.XValue[i] ^ Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp4Point.XValue[i];
    }

    for(i = 1U; Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp3Point.YValue[0U] >= i; i++)
    {
      CompareResult |= Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp3Point.YValue[i] ^ Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp4Point.YValue[i];
    }

    if(0U != CompareResult)
    {
      Result = CRYPTO_E_VER_NOT_OK;
    }
  }

  *Crypto_xVIx_xAIx_EdDSAVrfyCtx.ResultPtr = Result;

  Crypto_xVIx_xAIx_EdDSAVrfyCtx.VrfyState = CRYPTO_XVIX_XAIX_EDDSA_V_S_IDLE;
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.ErrorCode = E_OK;

  Crypto_xVIx_xAIx_EdDSAVrfyCtx.CallbackFlag = TRUE;
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.CallbackResult = Crypto_xVIx_xAIx_EdDSAVrfyCtx.ErrorCode;

  if ((NULL_PTR != Crypto_xVIx_xAIx_EdDSAVrfyCtx.ResultError) && (NULL_PTR != Crypto_xVIx_xAIx_EdDSAVrfyCtx.IsBusy))
  {
    *Crypto_xVIx_xAIx_EdDSAVrfyCtx.IsBusy = FALSE;
    *Crypto_xVIx_xAIx_EdDSAVrfyCtx.ResultError = Crypto_xVIx_xAIx_EdDSAVrfyCtx.ErrorCode;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAVrfyCalculateLittleK
(
  void
)
{
  switch (Crypto_xVIx_xAIx_EdDSAVrfyCtx.LittleKState)
  {
    case CRYPTO_XVIX_XAIX_EDDSA_V_S_INIT_SIGN_HASH:
    Crypto_xVIx_xAIx_EdDSAVrfyCalculateLittleK_InitSignHash();
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_V_S_UPDATE_DOM2:
    Crypto_xVIx_xAIx_EdDSAVrfyCalculateLittleK_UpdateDom2();

    break;

    case CRYPTO_XVIX_XAIX_EDDSA_V_S_UPDATE_ENC_R:
    Crypto_xVIx_xAIx_EdDSAVrfyCalculateLittleK_UpdateEncR();
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_V_S_UPDATE_ENC_A:
    Crypto_xVIx_xAIx_EdDSAVrfyCalculateLittleK_UpdateEncA();
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_V_S_UPDATE_M:
    Crypto_xVIx_xAIx_EdDSAVrfyCalculateLittleK_UpdateM();
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH:
    Crypto_xVIx_xAIx_EdDSAVrfyCalculateLittleK_Finish();
    break;
    default:
    Crypto_xVIx_xAIx_EdDSAVrfyCalculateLittleK_DeafultState();
    break;

  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAVrfyCalculateLittleK_Finish
(
  void
)
{
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.HashResultLength = sizeof(Crypto_xVIx_xAIx_EdDSAVrfyCtx.HashResult);
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.LittleKState = CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_FINISH;
  /* we are in the correct state when this is called so it can not fail */
  (void)Crypto_xVIx_xAIx_SHA2512HelperFinish
           (
             Crypto_xVIx_xAIx_EdDSAVrfyCtx.HashResult,
             &(Crypto_xVIx_xAIx_EdDSAVrfyCtx.HashResultLength),
             TRUE,
             Crypto_xVIx_xAIx_EdDSAVrfyCtx.Job->jobPrimitiveInfo->processingType
           );

}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAVrfyCalculateLittleK_UpdateM
(
  void
)
{
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.LittleKState = CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_UPDATE_M;
  /* we are in the correct state when this is called so it can not fail */
  (void)Crypto_xVIx_xAIx_SHA2512HelperUpdate
           (
             Crypto_xVIx_xAIx_EdDSAVrfyCtx.HashMessage,
             sizeof(Crypto_xVIx_xAIx_EdDSAVrfyCtx.HashMessage),
             Crypto_xVIx_xAIx_EdDSAVrfyCtx.Job->jobPrimitiveInfo->processingType
           );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAVrfyCalculateLittleK_UpdateEncA
(
  void
)
{
  /* Tmp1Array: the public Key, ENC(A) */
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.LittleKState = CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_UPDATE_ENC_A;
  /* we are in the correct state when this is called so it can not fail */
  (void)Crypto_xVIx_xAIx_SHA2512HelperUpdate
           (
             Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp1Array,
             Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp1ArrayLength,
             Crypto_xVIx_xAIx_EdDSAVrfyCtx.Job->jobPrimitiveInfo->processingType
           );

}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAVrfyCalculateLittleK_UpdateEncR
(
  void
)
{
  /* Hash ENC(R), the first half of the Signature */
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.LittleKState = CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_UPDATE_ENC_R;
  /* we are in the correct state when this is called so it can not fail */
  (void)Crypto_xVIx_xAIx_SHA2512HelperUpdate
           (
             Crypto_xVIx_xAIx_EdDSAVrfyCtx.Signature,
             (Crypto_xVIx_xAIx_EdDSAVrfyCtx.SignatureLength >> 1U),
             Crypto_xVIx_xAIx_EdDSAVrfyCtx.Job->jobPrimitiveInfo->processingType
           );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAVrfyCalculateLittleK_UpdateDom2
(
  void
)
{
  /* Hash the parameter "Crypto_xVIx_xAIx_Dom" */
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.LittleKState = CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_UPDATE_DOM2;
  /* we are in the correct state when this is called so it can not fail */
  (void)Crypto_xVIx_xAIx_SHA2512HelperUpdate
           (
             Crypto_xVIx_xAIx_Dom,
             sizeof(Crypto_xVIx_xAIx_Dom),
             Crypto_xVIx_xAIx_EdDSAVrfyCtx.Job->jobPrimitiveInfo->processingType
           );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAVrfyCalculateLittleK_InitSignHash
(
  void
)
{
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.LittleKState = CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_INIT;
  Crypto_xVIx_xAIx_SHA2512HelperStart(
                                       Crypto_xVIx_xAIx_EdDSAVrfyCtx.ObjId,
                                       &Crypto_xVIx_xAIx_EdDSAVrfyCallback,
                                       Crypto_xVIx_xAIx_EdDSAVrfyCtx.Job,
                                       Crypto_xVIx_xAIx_EdDSAVrfyCtx.Job->jobPrimitiveInfo->processingType
                                     );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAVrfyCalculateLittleK_DeafultState
(
  void
)
{
  switch(Crypto_xVIx_xAIx_EdDSAVrfyCtx.LittleKState)
  {
    case CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_INIT:
    case CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_UPDATE_DOM2:
    case CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_UPDATE_ENC_R:
    case CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_UPDATE_ENC_A:
    case CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_UPDATE_M:
    case CRYPTO_XVIX_XAIX_EDDSA_V_S_WAIT_FOR_FINISH:
    /* gets called only if processing Type is ASYNC */
    Crypto_xVIx_xAIx_SHA2512HelperMainFunction();
    break;

    /* CHECK: NOPARSE */
    /*For all the wait states helper main function shall be called, whiich has been addressed in the
      casees above.*/
    default:
    {
      /* Defensive Programming - unreachable code. This default case is required by  MISRA-C:2012 */
    }
    break;
    /* CHECK: PARSE */
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointDecodingStart
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  CtxPtr->DecodingState = CRYPTO_XVIX_XAIX_EDDSA_DEC_S_START;
}

static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointDecoding
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Result,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultPtr,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                   Value,
  uint32                                                      ValueLength
)
{
  uint8 Sign;
  boolean Finished = FALSE;

  switch (CtxPtr->DecodingState)
  {
    case CRYPTO_XVIX_XAIX_EDDSA_DEC_S_START:

    Result->ZValue[0U] = 1U;
    Result->ZValue[1U] = 1U;

    /* bit 255 of Value is the least significant bit of the x-coordinate */
    CtxPtr->X0 = (uint8)((uint8)(Value[31U] & 0x80U) >> 7U);

    /* y-coordinate is recovered by clearing the bit 255 */
    Value[31U] &= (uint8)(~(uint8)((uint8)1U << 7U));

    CtxPtr->DecodingState = CRYPTO_XVIX_XAIX_EDDSA_DEC_S_1;
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_DEC_S_1:
    /* we have enough memory allocated in the context to store the byte representation of the LN. */
    (void)Crypto_xVIx_xAIx_LNLEByteArrayToNumber(
                                            Value,
                                            ValueLength,
                                            Result->YValue,
                                            sizeof(Result->YValue) / sizeof(Result->YValue[0U])
                                          );
    /* Check if the resulting value is < p */
    if (CRYPTO_XVIX_XAIX_E_LARGER == Crypto_xVIx_xAIx_LNCompareNumbers(Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].Prime, Result->YValue))
    {
      /* The y coordinate is valid */
      Finished = FALSE;
      CtxPtr->DecodingState       = CRYPTO_XVIX_XAIX_EDDSA_DEC_S_2;
    }
    else
    {
      /* The y coordinate is out of range */
      *ResultPtr = CRYPTO_E_VER_NOT_OK;

      Finished = TRUE;
      CtxPtr->DecodingState = CRYPTO_XVIX_XAIX_EDDSA_DEC_S_START;
    }
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_DEC_S_2:
    /* calculation of u * v^3 * (u * v^7)^((p - 5) / 8) */
    Crypto_xVIx_xAIx_EdDSAPrepareCurveParams(
                                    CtxPtr,
                                    Result,
                                    Tmp
                                  );
    CtxPtr->DecodingParamsState = CRYPTO_XVIX_XAIX_EDDSA_DEC_S_P_3;
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_DEC_S_3:
    /* state management for calculation of (u/v) ^ ((p + 3) / 8) */
    Crypto_xVIx_xAIx_EdDSAPointDecodingStateMngmt(CtxPtr, Tmp);
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_DEC_S_6:
    /* calculate Tmp4 * Tmp6 : (u * v^3) * (p - 5) */
    Crypto_xVIx_xAIx_EdDSAStateCalculateEQ2(
                                   CtxPtr,
                                   Result,
                                   &Tmp[3U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS],
                                   &Tmp[5U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS]
                                 );
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_DEC_S_7:
    /* Tmp3 = XValue^2 */
    Crypto_xVIx_xAIx_EdDSAStateCalculateEQ3(
                                   CtxPtr,
                                   Result,
                                   &Tmp[2U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS]
                                 );
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_DEC_S_8:
    /* Tmp3 = v * XValue^2 */
    Crypto_xVIx_xAIx_EdDSAStateCalculateEQ4(
                                   CtxPtr,
                                   Tmp,
                                   Result,
                                   ResultPtr,
                                   &Finished
                                 );
    break;

    case CRYPTO_XVIX_XAIX_EDDSA_DEC_S_12:

    /* Unable to cover as we need to fulfill (XValue * (2^(P - 1)) == 0) or
     * ((v * XValue^2 - u) % Prime == 0) in the same time.
     * Nevertheless I think we need to keep this error check. */
    if ((0U == Result->XValue[0U]) && (1U == CtxPtr->X0))
    {
      Finished = TRUE;
      *ResultPtr = CRYPTO_E_VER_NOT_OK;
      CtxPtr->DecodingState = CRYPTO_XVIX_XAIX_EDDSA_DEC_S_START;
    }
    else

    {
      Sign = (uint8)(Result->XValue[1U] & 0x01U);

      /* x = prime - x */
      if (Sign != CtxPtr->X0)
      {
        Crypto_xVIx_xAIx_LNSubtractionModular(
                                     Result->XValue,
                                     Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].Prime,
                                     Result->XValue,
                                     Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].Prime
                                   );
      }
      *ResultPtr = CRYPTO_E_VER_OK;
      Finished = TRUE;
    }
    break;

    /* CHECK: NOPARSE */
    /*All the possible states have been addressed in the cases above.*/
    default:
    {
      /* Defensive Programming - unreachable code. This default case is required by  MISRA-C:2012 */
    }
    break;
    /* CHECK: PARSE */
  }
  return Finished;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAStatePowerAndReduce
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Base,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CheckHighestBit,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   ValueToDouble
)
{
  /* calculate x^2 */
#if (CRYPTO_XVIX_XAIX_LN_PLATFORM_DOUBLE_WORD_SUPPORT == STD_ON)
  Crypto_xVIx_xAIx_LNSquareComba(ValueToDouble, ValueToDouble);
#else
  Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                      ValueToDouble,
                                      ValueToDouble,
                                      ValueToDouble,
                                      &CtxPtr->ECCtx.LNCtx
                                    );
#endif

  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(ValueToDouble, &CtxPtr->ECCtx);

  if ((Crypto_xVIx_xAIx_LNWordType)1U == CRYPTO_XVIX_XAIX_LN_GET_BIT(CheckHighestBit, ((&CtxPtr->ECCtx.LNCtx)->Bits)))
  {
    /* calculate x^2 and reduce */
    Crypto_xVIx_xAIx_EdDSAStateCalculateEQ1(
                                             CtxPtr,
                                             Base,
                                             ValueToDouble
                                           );
  }
  CtxPtr->DecodingState = CRYPTO_XVIX_XAIX_EDDSA_DEC_S_3;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointDecodingXValue2
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Result,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp
)
{
  const uint32 Tmp6 = (5U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);

  Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                      Result->XValue,
                                      Result->XValue,
                                      &Tmp[Tmp6],
                                      &CtxPtr->ECCtx.LNCtx
                                    );

  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(Result->XValue, &CtxPtr->ECCtx);
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointDecodingExp
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp
)
{
  const uint32 Tmp1 = (0U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  const uint32 Tmp5 = (4U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  const uint32 Tmp6 = (5U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);

  Crypto_xVIx_xAIx_LNModExp(
                             &Tmp[Tmp6],
                             &Tmp[Tmp1],
                             Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].Prime,
                             &Tmp[Tmp5],
                             &CtxPtr->ECCtx.LNCtx
                           );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPointDecodingStateMngmt
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp
)
{
  /* state management for calculation of (u/v) ^ ((p + 3) / 8) */
  while((Crypto_xVIx_xAIx_LNWordType)0U < ((&CtxPtr->ECCtx.LNCtx)->Bits))
  {
    --(&CtxPtr->ECCtx.LNCtx)->Bits;
    /* calculate x^2 and reduce, recalculate the root candidate if necessary */
    Crypto_xVIx_xAIx_EdDSAStatePowerAndReduce
    (
      CtxPtr,
      &Tmp[2U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS],
      &Tmp[4U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS],
      &Tmp[5U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS]
    );
  }
  CtxPtr->DecodingState = CRYPTO_XVIX_XAIX_EDDSA_DEC_S_6;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAStateCalculateEQ1
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Base,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Result
)
{

  Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                      Result,
                                      Result,
                                      Base,
                                      &CtxPtr->ECCtx.LNCtx
                                    );

  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(Result, &CtxPtr->ECCtx);

  CtxPtr->DecodingState = CRYPTO_XVIX_XAIX_EDDSA_DEC_S_3;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAStateCalculateEQ2
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Point,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Multiplicand,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Multiplicator
)
{

  Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                      Point->XValue,
                                      Multiplicand,
                                      Multiplicator,
                                      &CtxPtr->ECCtx.LNCtx
                                    );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(Point->XValue, &CtxPtr->ECCtx);
  CtxPtr->DecodingState = CRYPTO_XVIX_XAIX_EDDSA_DEC_S_7;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAStateCalculateEQ3
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Point,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   SquareOfX
)
{
#if (CRYPTO_XVIX_XAIX_LN_PLATFORM_DOUBLE_WORD_SUPPORT == STD_ON)
  Crypto_xVIx_xAIx_LNSquareComba(SquareOfX, Point->XValue);
#else
  Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                      SquareOfX,
                                      Point->XValue,
                                      Point->XValue,
                                      &CtxPtr->ECCtx.LNCtx
                                    );
#endif
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(SquareOfX, &CtxPtr->ECCtx);
  CtxPtr->DecodingState = CRYPTO_XVIX_XAIX_EDDSA_DEC_S_8;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAStateCalculateEQ4
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Tmp,
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Result,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultPtr,
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                 Finished
)
{
  /* VValue    Offset where v is stored. */
  const uint32 VValue = (0U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  /* UValue    Offset where u is stored. */
  const uint32 UValue = (1U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  /* SquareOfX Offset where the Result of XValue^2 is stored. */
  const uint32 SquareOfX = (2U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  /* Value1    Offset where the Result of (v * XValue^2 - u) % Prime is stored. */
  const uint32 Value1 = (3U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  /* Value2    Offset where the Result of (v * XValue^2 + u) % Prime is stored. */
  const uint32 Value2 = (4U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);

  /* calculate v * SquareOfX : v* XValue^2 */

  Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                      &Tmp[SquareOfX],
                                      &Tmp[VValue],
                                      &Tmp[SquareOfX],
                                      &CtxPtr->ECCtx.LNCtx
                                    );

  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(&Tmp[SquareOfX], &CtxPtr->ECCtx);

  /* Value1 = (v * XValue^2 - u) % Prime */
  Crypto_xVIx_xAIx_LNSubtractionModular(
                               &Tmp[Value1],
                               &Tmp[SquareOfX],
                               &Tmp[UValue],
                               Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].Prime
                             );

  /* Value2 = (v * XValue^2 + u) % Prime */
  Crypto_xVIx_xAIx_LNAdditionModular(&Tmp[Value2], &Tmp[SquareOfX], &Tmp[UValue], Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].Prime);

  /* v * XValue^2 == u */
  if(0U == (&Tmp[Value1])[0U])
  {
    *ResultPtr = CRYPTO_E_VER_OK;


    /* Unable to cover as we need to fulfill both ((v * XValue^2 + u) % Prime == 0) and
     * ((v * XValue^2 - u) % Prime == 0) in the same time.
     * Nevertheless I think we need to keep this error check. */
    if(0U == (&Tmp[Value2])[0U])
    {
      /* invalid Point, exit with CRYPTO_E_VER_NOT_OK. */
      *Finished = TRUE;
      *ResultPtr = CRYPTO_E_VER_NOT_OK;
      CtxPtr->DecodingState = CRYPTO_XVIX_XAIX_EDDSA_DEC_S_START;
    }


    /* check the sign */
    CtxPtr->DecodingState = CRYPTO_XVIX_XAIX_EDDSA_DEC_S_12;
  }
  else
  {
    if (0U == (&Tmp[Value2])[0U])
    {
      /* calculate x = x * 2^((p-1)/4) */
      /* Tmp5 = Prime - 1 */
      Crypto_xVIx_xAIx_EdDSAStateCalculateEQ5(
                                               CtxPtr->ECCtx.CurveIndex,
                                               Result,
                                               &Tmp[0U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS],
                                               &Tmp[4U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS]
                                             );
      Crypto_xVIx_xAIx_EdDSAPointDecodingExp(CtxPtr, Tmp);
      /* XValue = XValue * (2^Tmp5) */
      Crypto_xVIx_xAIx_EdDSAPointDecodingXValue2(CtxPtr, Result, Tmp);
      *ResultPtr            = CRYPTO_E_VER_OK;
      /* check the sign */
      CtxPtr->DecodingState = CRYPTO_XVIX_XAIX_EDDSA_DEC_S_12;
    }
    else
    {
      /* invalid Point, exit with CRYPTO_E_VER_NOT_OK. */
      *Finished = TRUE;
      *ResultPtr = CRYPTO_E_VER_NOT_OK;
      CtxPtr->DecodingState = CRYPTO_XVIX_XAIX_EDDSA_DEC_S_START;
    }
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAStateCalculateEQ5
(
  uint8                                                             CurveIndex,
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Point,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Base,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Exponent
)
{
  uint32 i;

  Crypto_xVIx_xAIx_LNSubtractionModular(
                                         Exponent,
                                         Crypto_xVIx_xAIx_EllipticCurveInfo[CurveIndex].Prime,
                                         Point->ZValue,
                                         Crypto_xVIx_xAIx_EllipticCurveInfo[CurveIndex].Prime
                                       );

  /* Exponent = Exponent / 4 */
  for(i = 1U ; Exponent[0U] > i; i++)
  {
    Exponent[i] = (Crypto_xVIx_xAIx_LNWordType)(Exponent[i] >> 2U) |
                  (Crypto_xVIx_xAIx_LNWordType)(Exponent[i + 1U] << 30U);
  }

  Exponent[Exponent[0U]] = (Crypto_xVIx_xAIx_LNWordType)(Exponent[Exponent[0U]] >> 2U);

  /*Tmp6 = 2^Exponent */

  Base[0U] = 1U;
  Base[1U] = 2U;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPrepareCurveParams
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  CtxPtr,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    Tmp
)
{
  uint32  i;
  const uint32 TempIndex0 = (0U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  const uint32 TempIndex1 = (1U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  const uint32 TempIndex2 = (2U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  const uint32 TempIndex3 = (3U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  const uint32 TempIndex4 = (4U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  const uint32 TempIndex5 = (5U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);

  /* Tmp2 = u = y^2 - 1*/
  Crypto_xVIx_xAIx_EdDSAPrepareCurveParamsY2(Result, Tmp, CtxPtr);

  /* Tmp1 = v = d * (y^2)  + 1 */
  Crypto_xVIx_xAIx_EdDSAPrepareCurveParamsDY2(CtxPtr, Result, Tmp);

  /* Tmp3 = Tmp1^2 : v^2 */
  Crypto_xVIx_xAIx_EdDSAPrepareCurveParamsV2(Tmp, CtxPtr);

  /* Tmp3 = Tmp1^3 : v^3 */
  Crypto_xVIx_xAIx_EdDSAPrepareCurveParamsV3(CtxPtr, Tmp);

  /* Tmp4 = Tmp2 * Tmp3 : u * v^3  */
  Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                      &Tmp[TempIndex3],
                                      &Tmp[TempIndex1],
                                      &Tmp[TempIndex2],
                                      &CtxPtr->ECCtx.LNCtx
                                    );

  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(&Tmp[TempIndex3], &CtxPtr->ECCtx);

  (&Tmp[TempIndex4])[0U] = 1U;
  (&Tmp[TempIndex4])[1U] = 7U;

  /* Tmp3 = Tmp1^7 : v^7 */
  Crypto_xVIx_xAIx_LNModExp(
                             &Tmp[TempIndex2],
                             &Tmp[TempIndex0],
                             Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].Prime,
                             &Tmp[TempIndex4],
                             &CtxPtr->ECCtx.LNCtx
                           );

  /* Tmp3 = Tmp2 * Tmp1^7 : u * v^7  */
  Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                      &Tmp[TempIndex2],
                                      &Tmp[TempIndex1],
                                      &Tmp[TempIndex2],
                                      &CtxPtr->ECCtx.LNCtx
                                    );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(&Tmp[TempIndex2], &CtxPtr->ECCtx);

  (&Tmp[TempIndex5])[0U] = 1U;
  (&Tmp[TempIndex5])[1U] = 5U;

  /* Tmp5 = primeModulus - 5 */
  Crypto_xVIx_xAIx_LNSubtractionModular(
                               &Tmp[TempIndex4],
                               Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].Prime,
                               &Tmp[TempIndex5],
                               Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].Prime
                             );

  /* Tmp5 = Tmp5 / 8 : (p - 5) / 8 */
  for(i = 1U; (&Tmp[TempIndex4])[0U] > i; i++)
  {
    (&Tmp[TempIndex4])[i] = (Crypto_xVIx_xAIx_LNWordType)((&Tmp[TempIndex4])[i] >> 3U) |
                            (Crypto_xVIx_xAIx_LNWordType)((&Tmp[TempIndex4])[i + 1U] << 29U);
  }
  (&Tmp[TempIndex4])[(&Tmp[TempIndex4])[0U]] =
  (Crypto_xVIx_xAIx_LNWordType)((&Tmp[TempIndex4])[(&Tmp[TempIndex4])[0U]] >> 3U);

  (&CtxPtr->ECCtx.LNCtx)->Bits = Crypto_xVIx_xAIx_LNBitLengthOfNumber(&Tmp[TempIndex4]);

  /* Tmp6 = Result */
  (&Tmp[TempIndex5])[0U] = 1U;
  (&Tmp[TempIndex5])[1U] = 1U;
  CtxPtr->DecodingState = CRYPTO_XVIX_XAIX_EDDSA_DEC_S_3;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPrepareCurveParamsY2
(
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    Tmp,
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  CtxPtr
)
{
  const uint32 TempIndex0 = (0U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  const uint32 TempIndex1 = (1U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);

  /* Tmp1 = y^2 */
#if (CRYPTO_XVIX_XAIX_LN_PLATFORM_DOUBLE_WORD_SUPPORT == STD_ON)
  TS_PARAM_UNUSED(CtxPtr);
  Crypto_xVIx_xAIx_LNSquareComba(&Tmp[TempIndex0], Result->YValue);
#else
  Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                      &Tmp[TempIndex0],
                                      Result->YValue,
                                      Result->YValue,
                                      &CtxPtr->ECCtx.LNCtx
                                    );
#endif
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(&Tmp[TempIndex0], &CtxPtr->ECCtx);

  /* Tmp2: (y^2) - 1 = Tmp1 - 1 */
  Crypto_xVIx_xAIx_LNSubtractionModular(
                               &Tmp[TempIndex1],
                               &Tmp[TempIndex0],
                               Result->ZValue,
                               Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].Prime
                             );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPrepareCurveParamsDY2
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  CtxPtr,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    Tmp
)
{
  const uint32 TempIndex0 = (0U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);

  /* Tmp1 = d * (y^2) */
  Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                      &Tmp[TempIndex0],
                                      Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].Coeff1,
                                      &Tmp[TempIndex0],
                                      &CtxPtr->ECCtx.LNCtx
                                    );

  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(&Tmp[TempIndex0], &CtxPtr->ECCtx);

  /* a = -1 : Tmp1 = (d * (y^2)) + 1 */
  Crypto_xVIx_xAIx_LNAdditionModular(
                            &Tmp[TempIndex0],
                            &Tmp[TempIndex0],
                            Result->ZValue,
                            Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].Prime
                          );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPrepareCurveParamsV2
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  const uint32 TempIndex0 = (0U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  const uint32 TempIndex2 = (2U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);

  /* Tmp3 = Tmp1^2 : v^2 */
#if (CRYPTO_XVIX_XAIX_LN_PLATFORM_DOUBLE_WORD_SUPPORT == STD_ON)
  TS_PARAM_UNUSED(CtxPtr);
  Crypto_xVIx_xAIx_LNSquareComba(&Tmp[TempIndex2], &Tmp[TempIndex0]);
#else
  Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                      &Tmp[TempIndex2],
                                      &Tmp[TempIndex0],
                                      &Tmp[TempIndex0],
                                      &CtxPtr->ECCtx.LNCtx
                                    );
#endif

  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(&Tmp[TempIndex2], &CtxPtr->ECCtx);
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAPrepareCurveParamsV3
(
  P2VAR(Crypto_xVIx_xAIx_EdDSACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp
)
{
  const uint32 TempIndex0 = (0U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);
  const uint32 TempIndex2 = (2U * CRYPTO_XVIX_XAIX_EDDSA_TEMP_LEN_WORDS);

  /* Tmp3 = Tmp1^3 : v^3 */
  Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                      &Tmp[TempIndex2],
                                      &Tmp[TempIndex2],
                                      &Tmp[TempIndex0],
                                      &CtxPtr->ECCtx.LNCtx
                                    );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->ECCtx.CurveIndex].FastReductionFun(&Tmp[TempIndex2], &CtxPtr->ECCtx);
}

/* !LINKSTO EB_Crypto_CryptAlgos_0963, 1 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAVrfyStart
(
  P2CONST(void, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                  CfgPtr,
  P2CONST(Crypto_xVIx_xAIx_AsymPublicKeyType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  Crypto_xVIx_xAIx_EdDSAVrfyCtx.CfgPtr =
  (P2CONST(Crypto_xVIx_xAIx_EdDSAVrfyConfigType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_CONST))CfgPtr;
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.ObjId = Crypto_xVIx_xAIx_EdDSAVrfyCtx.CfgPtr->ObjId;
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.Crypto_Generic_Callback = Crypto_xVIx_xAIx_EdDSAVrfyCtx.CfgPtr->Crypto_Generic_Callback;
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.ProcessingType = Crypto_xVIx_xAIx_EdDSAVrfyCtx.CfgPtr->ProcessingType;

  Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp1ArrayLength = KeyPtr->length;
  TS_MemCpy(Crypto_xVIx_xAIx_EdDSAVrfyCtx.Tmp1Array, KeyPtr->data, KeyPtr->length);

  Crypto_xVIx_xAIx_EdDSAVrfyCtx.Job = Crypto_xVIx_xAIx_EdDSAVrfyCtx.CfgPtr->Job;
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.VrfyState  = CRYPTO_XVIX_XAIX_EDDSA_V_S_VERIFY_START;
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.DecPubKeyState = CRYPTO_XVIX_XAIX_EDDSA_V_S_CONVERT_PUBLICKEY;
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.CurrentMode = CRYPTO_OPERATIONMODE_START;
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.MsgHashStarted = FALSE;
  Crypto_xVIx_xAIx_EdDSAVrfyCtx.ErrorCode = E_OK;

  /* The function can only return E_OK here. */
  (void)Crypto_xVIx_xAIx_GetECIndex
        (
          Crypto_xVIx_xAIx_EdDSAVrfyCtx.Job->jobPrimitiveInfo->primitiveInfo->service,
          Crypto_xVIx_xAIx_EdDSAVrfyCtx.Job->jobPrimitiveInfo->primitiveInfo->algorithm.family,
          NULL_PTR,
          0U,
          &(Crypto_xVIx_xAIx_EdDSAVrfyCtx.EdDSACtx.ECCtx.CurveIndex)
        );

  RetVal = E_OK;

  return RetVal;
}

static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAVrfyUpdate
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) DataPtr,
  uint32                                      DataLength
)
{
  Std_ReturnType RetVal;

  if(CRYPTO_XVIX_XAIX_EDDSA_V_S_INITIALIZED == Crypto_xVIx_xAIx_EdDSAVrfyCtx.VrfyState)
  {
    /* Store data into context */
    Crypto_xVIx_xAIx_EdDSAVrfyCtx.Message = DataPtr;
    Crypto_xVIx_xAIx_EdDSAVrfyCtx.MessageLength = DataLength;

    /* First Message block to be hashed */
    if(FALSE == Crypto_xVIx_xAIx_EdDSAVrfyCtx.MsgHashStarted)
    {
      Crypto_xVIx_xAIx_EdDSAVrfyCtx.MsgHashStarted = TRUE;
      Crypto_xVIx_xAIx_EdDSAVrfyCtx.HashMsgState = CRYPTO_XVIX_XAIX_EDDSA_V_S_INIT_HASH_MESSAGE;
    }
    else
    {
      /* Subsequent Message blocks to be hashed */
      Crypto_xVIx_xAIx_EdDSAVrfyCtx.HashMsgState = CRYPTO_XVIX_XAIX_EDDSA_V_S_UPDATE_MESSAGE;
    }

    /* Set state of the state machine to update */
    Crypto_xVIx_xAIx_EdDSAVrfyCtx.VrfyState = CRYPTO_XVIX_XAIX_EDDSA_V_S_HASH_MSG;
    Crypto_xVIx_xAIx_EdDSAVrfyCtx.ErrorCode = E_OK;

    RetVal = E_OK;
  }
  else
  {
    RetVal = CRYPTO_E_BUSY;
  }
  return RetVal;
}

static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAVrfyFinish
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                 SignaturePtr,
  uint32                                                      SignatureLength,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultPtr
)
{
  Std_ReturnType RetVal;

  if(
           (TRUE == Crypto_xVIx_xAIx_EdDSAVrfyCtx.MsgHashStarted ) &&
           (CRYPTO_XVIX_XAIX_EDDSA_V_S_INITIALIZED == Crypto_xVIx_xAIx_EdDSAVrfyCtx.VrfyState)
         )
  {
    /* Store data into context */
    Crypto_xVIx_xAIx_EdDSAVrfyCtx.Signature = SignaturePtr;
    Crypto_xVIx_xAIx_EdDSAVrfyCtx.SignatureLength = SignatureLength;
    Crypto_xVIx_xAIx_EdDSAVrfyCtx.ResultPtr = ResultPtr;

    /* Set state of the state machine to Update */
    Crypto_xVIx_xAIx_EdDSAVrfyCtx.VrfyState = CRYPTO_XVIX_XAIX_EDDSA_V_S_HASH_MSG;
    Crypto_xVIx_xAIx_EdDSAVrfyCtx.HashMsgState = CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_MESSAGE;

    Crypto_xVIx_xAIx_EdDSAVrfyCtx.MsgHashStarted = FALSE;

    RetVal = E_OK;
  }
  else
  {
    RetVal = CRYPTO_E_BUSY;
  }
  return RetVal;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAVrfyMainFunction
(
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
)
{
  if(!Crypto_xVIx_xAIx_CheckSkipMain(Result, Busy, Crypto_xVIx_xAIx_EdDSAVrfyCtx.ObjId))
  {
    Crypto_xVIx_xAIx_EdDSAVrfyCtx.IsBusy = Busy;
    Crypto_xVIx_xAIx_EdDSAVrfyCtx.ResultError = Result;
    if((NULL_PTR != Result) && (NULL_PTR != Busy))
    {
      *Crypto_xVIx_xAIx_EdDSAVrfyCtx.IsBusy = TRUE;
      *Crypto_xVIx_xAIx_EdDSAVrfyCtx.ResultError = E_OK;
    }
    switch (Crypto_xVIx_xAIx_EdDSAVrfyCtx.VrfyState)
    {
      case CRYPTO_XVIX_XAIX_EDDSA_V_S_VERIFY_START:
      Crypto_xVIx_xAIx_EdDSAVrfyDecodePublicKey();
      break;

      case CRYPTO_XVIX_XAIX_EDDSA_V_S_HASH_MSG:
      Crypto_xVIx_xAIx_EdDSAVrfyHashMsg();
      break;

      case CRYPTO_XVIX_XAIX_EDDSA_V_S_VALIDATE_SIG_COMPONENTS:
      Crypto_xVIx_xAIx_EdDSAVrfyValSigCmp();
      break;

      case CRYPTO_XVIX_XAIX_EDDSA_V_S_LITTLE_K:
      Crypto_xVIx_xAIx_EdDSAVrfyCalculateLittleK();
      break;

      case CRYPTO_XVIX_XAIX_EDDSA_V_S_GROUP_EQUATIONS:
      Crypto_xVIx_xAIx_EdDSAFinishEquation();
      break;

      case CRYPTO_XVIX_XAIX_EDDSA_V_S_FINISH_COMPARE:
      Crypto_xVIx_xAIx_EdDSAValidateSignature();
      break;

      case CRYPTO_XVIX_XAIX_EDDSA_V_S_INITIALIZED:
      case CRYPTO_XVIX_XAIX_EDDSA_V_S_IDLE:
      break;

      /* CHECK: NOPARSE */
      /*All the possible states are addressed in the cases above.*/
      /* Defensive Programming - unreachable code. This default case is required by  MISRA-C:2012 */
      default:
      {
      /* Defensive Programming - unreachable code. This default case is required by  MISRA-C:2012 */
      }
      break;
      /* CHECK: PARSE */
    }
    /* !LINKSTO SWS_Crypto_00143_CORRECTION_SWS_Crypto_00181, 1, SWS_Crypto_00144_CORRECTION, 1 */
    if(TRUE == Crypto_xVIx_xAIx_EdDSAVrfyCtx.CancelFlag)
    {
      if (
           (NULL_PTR != Crypto_xVIx_xAIx_EdDSAVrfyCtx.ResultError)
           && (NULL_PTR != Crypto_xVIx_xAIx_EdDSAVrfyCtx.IsBusy)
         )
      {
        *Crypto_xVIx_xAIx_EdDSAVrfyCtx.IsBusy = FALSE;
        *Crypto_xVIx_xAIx_EdDSAVrfyCtx.ResultError = CRYPTO_E_JOB_CANCELED;
        /* processingType is synchronous */
        /* !LINKSTO Crypto.Dsn.JobTransition.ActiveCancel2Idle, 1 */
        Crypto_xVIx_xAIx_EdDSAVrfyCtx.Job->jobState = CRYPTO_JOBSTATE_IDLE;
        SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
        Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_EdDSAVrfyCtx.ObjId].DriverObjectState =
        CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
        Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_EdDSAVrfyCtx.ObjId].CurrentJob =
        NULL_PTR;
        Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_EdDSAVrfyCtx.ObjId].SkipPeriodicMainFunction = TRUE;
        SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      }
      Crypto_xVIx_xAIx_EdDSAVrfyCtx.Crypto_Generic_Callback
      (
        Crypto_xVIx_xAIx_EdDSAVrfyCtx.Job,
        CRYPTO_E_JOB_CANCELED
      );
    }
    else
    {
      if(TRUE == Crypto_xVIx_xAIx_EdDSAVrfyCtx.CallbackFlag)
      {
        if (
             (NULL_PTR != Crypto_xVIx_xAIx_EdDSAVrfyCtx.ResultError)
             && (NULL_PTR != Crypto_xVIx_xAIx_EdDSAVrfyCtx.IsBusy)
           )
        {
          *Crypto_xVIx_xAIx_EdDSAVrfyCtx.IsBusy = FALSE;
          *Crypto_xVIx_xAIx_EdDSAVrfyCtx.ResultError =
          Crypto_xVIx_xAIx_EdDSAVrfyCtx.CallbackResult;
        }
        if(
            (
              CRYPTO_PROCESSING_ASYNC ==
              Crypto_xVIx_xAIx_EdDSAVrfyCtx.Job->jobPrimitiveInfo->processingType
            ) &&
            (
              (
                (
                  ((uint8)Crypto_xVIx_xAIx_EdDSAVrfyCtx.Job->jobPrimitiveInputOutput.mode) &
                    ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
                ) == ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
              ) ||
              (
                (
                  ((uint8)Crypto_xVIx_xAIx_EdDSAVrfyCtx.Job->jobPrimitiveInputOutput.mode) &
                  ((uint8) CRYPTO_OPERATIONMODE_STREAMSTART)
                ) == ( (uint8) CRYPTO_OPERATIONMODE_STREAMSTART)
              )
            ) && (CRYPTO_OPERATIONMODE_START == Crypto_xVIx_xAIx_EdDSAVrfyCtx.CurrentMode)
          )
        {
          /*In case of asynchronous single call or stream start processing, callbacks are used for
             communicating status. The return type can be ignored here because incase of failure
             upper layers will be informed through callback.*/
          (void)Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ED25519_NOT_SET_Update
                (
                  Crypto_xVIx_xAIx_EdDSAVrfyCtx.ObjId,
                  Crypto_xVIx_xAIx_EdDSAVrfyCtx.Job
                );
        }
        else
        {
          if
          (
            (
              CRYPTO_PROCESSING_ASYNC ==
                  Crypto_xVIx_xAIx_EdDSAVrfyCtx.Job->jobPrimitiveInfo->processingType
            ) &&
            (
              (
                (
                  ((uint8)Crypto_xVIx_xAIx_EdDSAVrfyCtx.Job->jobPrimitiveInputOutput.mode) &
                  ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
                ) == ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
              )
            ) && (CRYPTO_OPERATIONMODE_UPDATE == Crypto_xVIx_xAIx_EdDSAVrfyCtx.CurrentMode)
          )
          {
            /*In case of asynchronous single call processing , callbacks are used for
             communicating status. The return type can be ignored here because incase of failure
             upper layers will be informed through callback.*/
            (void)Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ED25519_NOT_SET_Finish
                  (
                    Crypto_xVIx_xAIx_EdDSAVrfyCtx.ObjId,
                    Crypto_xVIx_xAIx_EdDSAVrfyCtx.Job
                  );
          }
        }
        Crypto_xVIx_xAIx_EdDSAVrfyCtx.Crypto_Generic_Callback
        (
          Crypto_xVIx_xAIx_EdDSAVrfyCtx.Job,
          Crypto_xVIx_xAIx_EdDSAVrfyCtx.CallbackResult
        );
        Crypto_xVIx_xAIx_EdDSAVrfyCtx.CallbackFlag = FALSE;
      }
    }
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATURE_VERIFY_ED25519_NOT_SET_CallBack
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint8                                            Result
)
{
  if(CRYPTO_PROCESSING_ASYNC == Job->jobPrimitiveInfo->processingType)
  {
    if (
         (
           CRYPTO_OPERATIONMODE_FINISH ==
           (CRYPTO_OPERATIONMODE_FINISH & Crypto_xVIx_xAIx_EdDSAVrfyCtx.CurrentMode)
         )|| (E_OK != Result)
       )
    {
      /* !LINKSTO SWS_Crypto_00025, 1 */
      /* !LINKSTO SWS_Crypto_00119, 1 */
      /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Idle, 1 */
      /* !LINKSTO Crypto.Dsn.JobTransition.ActiveError2Idle, 1 */
      Job->jobState = CRYPTO_JOBSTATE_IDLE;
      Crypto_xVIx_xAIx_EdDSAVrfyCtx.CurrentMode = CRYPTO_XVIX_XAIX_SIGNATURE_DEFAULT_CURRENT_MODE;
      /* !LINKSTO EB_Crypto_01030, 1 */
      SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_EdDSAVrfyCtx.ObjId].DriverObjectState =
      CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_EdDSAVrfyCtx.ObjId].CurrentJob = NULL_PTR;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_EdDSAVrfyCtx.ObjId].SkipPeriodicMainFunction = TRUE;
      SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      CryIf_CallbackNotification(Job, Result);
      Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ED25519_NOT_SET_Reset();
    }
    if(
        (
          CRYPTO_OPERATIONMODE_UPDATE ==
          (CRYPTO_OPERATIONMODE_UPDATE & Crypto_xVIx_xAIx_EdDSAVrfyCtx.CurrentMode)
        ) && (
               ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL) !=
               (
                 ((uint8)Crypto_xVIx_xAIx_EdDSAVrfyCtx.Job->jobPrimitiveInputOutput.mode) &
                 ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
               )
             )
      )
    {
      CryIf_CallbackNotification(Job, Result);
    }
  }
}

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON) */

#else /* #if ((CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED  == STD_ON) ||
              (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON)) */
typedef uint8 Crypto_xVIx_xAIx_Signature_PrvtEmptyTranslationUnitType;

#endif /* #if ((CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED  == STD_ON) ||
               (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON)) */

/*==[end of file]=================================================================================*/

