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
#include <Crypto_xVIx_xAIx_AL_Cfg.h>

#include <Crypto_xVIx_xAIx_AL_Hash.h>
#include <Crypto_xVIx_xAIx_AL_Common.h>
#include <Crypto_xVIx_xAIx_AL_Random.h>
#include <Crypto_xVIx_xAIx_AL_ECC_NISTP256.h>
#include <Crypto_xVIx_xAIx_AL_KeyManagement.h>

#if (                                                   \
     (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED  == STD_ON) || \
     (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON)    \
    )

/*==[Macros]======================================================================================*/

#if (defined CRYPTO_XVIX_XAIX_ECDSA_SIGNATURE_DEFAULT_CURRENT_MODE)
#error CRYPTO_XVIX_XAIX_ECDSA_SIGNATURE_DEFAULT_CURRENT_MODE is already defined
#endif
/** \brief To set the default current mode */
#define CRYPTO_XVIX_XAIX_ECDSA_SIGNATURE_DEFAULT_CURRENT_MODE  0x00U

#if (defined CRYPTO_XVIX_XAIX_LNNUMBERLEN)
#error CRYPTO_XVIX_XAIX_LNNUMBERLEN is already defined
#endif
/** \brief Compute the amount of words that the long number representation of a
  *        number will need from the length of the number in bytes.
  */
#define CRYPTO_XVIX_XAIX_LNNUMBERLEN(X)             \
(                                                   \
   (Crypto_xVIx_xAIx_LNWordType)                    \
   (                                                \
      (uint32)                                      \
      (                                             \
         (uint32)                                   \
         (                                          \
            (uint32)(X) - (uint32)1U                \
         ) /                                        \
         (uint32)CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES \
      ) +                                           \
     (uint32)1U                                     \
   )                                                \
)

/*--ECDSAP256-------------------------------------------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_ECDSAP256_HASH_MAX_LEN)
#error CRYPTO_XVIX_XAIX_ECDSAP256_HASH_MAX_LEN is already defined
#endif
/** \brief The maximal allowed length for a hash digest in bytes. */
#define CRYPTO_XVIX_XAIX_ECDSAP256_HASH_MAX_LEN                32U

#if (defined CRYPTO_XVIX_XAIX_ECDSAP256_SIGN_SIZE)
#error CRYPTO_XVIX_XAIX_ECDSAP256_SIGN_SIZE is already defined
#endif
/** \brief The ECDSA Signature size in bytes. */
#define CRYPTO_XVIX_XAIX_ECDSAP256_SIGN_SIZE                   64U

/*--ECDSAP256Gen----------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)

#if (defined CRYPTO_XVIX_XAIX_ECDSAP256_PRIKEY_LEN)
#error CRYPTO_XVIX_XAIX_ECDSAP256_PRIKEY_LEN is already defined
#endif
/** \brief The length of the private key in bytes. */
#define CRYPTO_XVIX_XAIX_ECDSAP256_PRIKEY_LEN                32U

#if (defined CRYPTO_XVIX_XAIX_ECDSA_G_RANDOM_FAILING)
#error CRYPTO_XVIX_XAIX_ECDSA_G_RANDOM_FAILING is already defined
#endif
/** \brief Handle failing component validation while processing the Signature */
#define CRYPTO_XVIX_XAIX_ECDSA_G_RANDOM_FAILING do                                              \
{                                                                                               \
  Crypto_xVIx_xAIx_ECDSAGenCtx.ErrorCode = E_NOT_OK;                                            \
  if(NULL_PTR != Crypto_xVIx_xAIx_ECDSAGenCtx.ResultError)                                      \
  {                                                                                             \
    *Crypto_xVIx_xAIx_ECDSAGenCtx.ResultError = Crypto_xVIx_xAIx_ECDSAGenCtx.ErrorCode;         \
  }                                                                                             \
  if(NULL_PTR != Crypto_xVIx_xAIx_ECDSAGenCtx.IsBusy)                                           \
  {                                                                                             \
    *Crypto_xVIx_xAIx_ECDSAGenCtx.IsBusy = FALSE;                                               \
  }                                                                                             \
  Crypto_xVIx_xAIx_ECDSAGenCtx.State          = CRYPTO_XVIX_XAIX_ECDSA_G_IDLE;                  \
  Crypto_xVIx_xAIx_ECDSAGenCtx.CallbackFlag   = TRUE;                                           \
  Crypto_xVIx_xAIx_ECDSAGenCtx.CallbackResult = Crypto_xVIx_xAIx_ECDSAGenCtx.ErrorCode;         \
} while(0U)

/*--GenerationState-------------------------------------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_ECDSA_G_IDLE)
#error CRYPTO_XVIX_XAIX_ECDSA_G_IDLE is already defined
#endif
/** \brief Waiting for new operation */
#define CRYPTO_XVIX_XAIX_ECDSA_G_IDLE                          0U

#if (defined CRYPTO_XVIX_XAIX_ECDSA_G_INIT_WAIT)
#error CRYPTO_XVIX_XAIX_ECDSA_G_INIT_WAIT is already defined
#endif
/** \brief The initialization of the subservices is in progress. */
#define CRYPTO_XVIX_XAIX_ECDSA_G_INIT_WAIT                     1U

#if (defined CRYPTO_XVIX_XAIX_ECDSA_G_INITIALIZED)
#error CRYPTO_XVIX_XAIX_ECDSA_G_INITIALIZED is already defined
#endif
/** \brief The Signature generation is initialized */
#define CRYPTO_XVIX_XAIX_ECDSA_G_INITIALIZED                   2U

#if (defined CRYPTO_XVIX_XAIX_ECDSA_G_UDAPTE_HASH_MESSAGE)
#error CRYPTO_XVIX_XAIX_ECDSA_G_UDAPTE_HASH_MESSAGE is already defined
#endif
/** \brief Hash Message blocks */
#define CRYPTO_XVIX_XAIX_ECDSA_G_UDAPTE_HASH_MESSAGE           3U

#if (defined CRYPTO_XVIX_XAIX_ECDSA_G_WAIT_FOR_UPDATE_HASH_MESSAGE)
#error CRYPTO_XVIX_XAIX_ECDSA_G_WAIT_FOR_UPDATE_HASH_MESSAGE is already defined
#endif
/** \brief Wait for the HashUpdate callback */
#define CRYPTO_XVIX_XAIX_ECDSA_G_WAIT_FOR_UPDATE_HASH_MESSAGE  4U

#if (defined CRYPTO_XVIX_XAIX_ECDSA_G_FINISH_HASH_MESSAGE)
#error CRYPTO_XVIX_XAIX_ECDSA_G_FINISH_HASH_MESSAGE is already defined
#endif
/** \brief Finish the Message hashing */
#define CRYPTO_XVIX_XAIX_ECDSA_G_FINISH_HASH_MESSAGE           5U

#if (defined CRYPTO_XVIX_XAIX_ECDSA_G_WAIT_FOR_FINISH_HASH_MESSAGE)
#error CRYPTO_XVIX_XAIX_ECDSA_G_WAIT_FOR_FINISH_HASH_MESSAGE is already defined
#endif
/** \brief Wait for the HashFinish callback */
#define CRYPTO_XVIX_XAIX_ECDSA_G_WAIT_FOR_FINISH_HASH_MESSAGE  6U

#if (defined CRYPTO_XVIX_XAIX_ECDSA_G_RANDOM)
#error CRYPTO_XVIX_XAIX_ECDSA_G_RANDOM is already defined
#endif
/** \brief Generates a random number */
#define CRYPTO_XVIX_XAIX_ECDSA_G_RANDOM                        7U

#if (defined CRYPTO_XVIX_XAIX_ECDSA_G_VALIDATEK)
#error CRYPTO_XVIX_XAIX_ECDSA_G_VALIDATEK is already defined
#endif
/** \brief Check the validation of K */
#define CRYPTO_XVIX_XAIX_ECDSA_G_VALIDATEK                     8U

#if (defined CRYPTO_XVIX_XAIX_ECDSA_G_KMULTP)
#error CRYPTO_XVIX_XAIX_ECDSA_G_KMULTP is already defined
#endif
/** \brief Perform the multiplication R = K * Base point */
#define CRYPTO_XVIX_XAIX_ECDSA_G_KMULTP                        9U

#if (defined CRYPTO_XVIX_XAIX_ECDSA_G_FINISHR)
#error CRYPTO_XVIX_XAIX_ECDSA_G_FINISHR is already defined
#endif
/** \brief Calculate first half of the Signature (R) */
#define CRYPTO_XVIX_XAIX_ECDSA_G_FINISHR                       10U

#if (defined CRYPTO_XVIX_XAIX_ECDSA_G_FINISH_SIG)
#error CRYPTO_XVIX_XAIX_ECDSA_G_FINISH_SIG is already defined
#endif
/** \brief Calculate second half of the Signature (S) */
# define CRYPTO_XVIX_XAIX_ECDSA_G_FINISH_SIG                   11U

#if (defined CRYPTO_XVIX_XAIX_ECDSA_G_EXPORT_SIG)
#error CRYPTO_XVIX_XAIX_ECDSA_G_EXPORT_SIG is already defined
#endif
/** \brief Export the Signature */
#define CRYPTO_XVIX_XAIX_ECDSA_G_EXPORT_SIG                    12U

#endif /* #if (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON) */

/*--ECDSAP256Vrfy---------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON)

#if (defined CRYPTO_XVIX_XAIX_ECDSAP256_PUBKEY_LEN)
#error CRYPTO_XVIX_XAIX_ECDSAP256_PUBKEY_LEN is already defined
#endif
/** \brief The length of the public key in bytes. */
#define CRYPTO_XVIX_XAIX_ECDSAP256_PUBKEY_LEN                64U

#if (defined CRYPTO_XVIX_XAIX_ECDSA_V_HANDLE_FAILING_VALIDATE_SIG)
#error CRYPTO_XVIX_XAIX_ECDSA_V_HANDLE_FAILING_VALIDATE_SIG is already defined
#endif
/** \brief Handle failing component validation while processing the Signature */
#define CRYPTO_XVIX_XAIX_ECDSA_V_HANDLE_FAILING_VALIDATE_SIG do                                 \
{                                                                                               \
  Crypto_xVIx_xAIx_ECDSAVrfyCtx.ErrorCode = E_OK;                                               \
  if(NULL_PTR != Crypto_xVIx_xAIx_ECDSAVrfyCtx.ResultError)                                     \
  {                                                                                             \
    *Crypto_xVIx_xAIx_ECDSAVrfyCtx.ResultError = Crypto_xVIx_xAIx_ECDSAVrfyCtx.ErrorCode;       \
  }                                                                                             \
  if(NULL_PTR != Crypto_xVIx_xAIx_ECDSAVrfyCtx.IsBusy)                                          \
  {                                                                                             \
    *Crypto_xVIx_xAIx_ECDSAVrfyCtx.IsBusy = FALSE;                                              \
  }                                                                                             \
  Crypto_xVIx_xAIx_ECDSAVrfyCtx.State          = CRYPTO_XVIX_XAIX_ECDSA_V_IDLE;                 \
  *Crypto_xVIx_xAIx_ECDSAVrfyCtx.ResultPtr     = CRYPTO_E_VER_NOT_OK;                           \
  Crypto_xVIx_xAIx_ECDSAVrfyCtx.CallbackFlag   = TRUE;                                          \
  Crypto_xVIx_xAIx_ECDSAVrfyCtx.CallbackResult = Crypto_xVIx_xAIx_ECDSAVrfyCtx.ErrorCode;       \
} while(0U)

#if (defined CRYPTO_XVIX_XAIX_ECDSA_V_HANDLE_FAILING_VALIDATE_X)
#error CRYPTO_XVIX_XAIX_ECDSA_V_HANDLE_FAILING_VALIDATE_X is already defined
#endif
/** \brief Handle failing component validation while processing the Signature */
#define CRYPTO_XVIX_XAIX_ECDSA_V_HANDLE_FAILING_VALIDATE_X do                                   \
{                                                                                               \
  Crypto_xVIx_xAIx_ECDSAVrfyCtx.ErrorCode = E_OK;                                               \
  if(NULL_PTR != Crypto_xVIx_xAIx_ECDSAVrfyCtx.ResultError)                                     \
  {                                                                                             \
    *Crypto_xVIx_xAIx_ECDSAVrfyCtx.ResultError = Crypto_xVIx_xAIx_ECDSAVrfyCtx.ErrorCode;       \
  }                                                                                             \
  if(NULL_PTR != Crypto_xVIx_xAIx_ECDSAVrfyCtx.IsBusy)                                          \
  {                                                                                             \
    *Crypto_xVIx_xAIx_ECDSAVrfyCtx.IsBusy = FALSE;                                              \
  }                                                                                             \
  Crypto_xVIx_xAIx_ECDSAVrfyCtx.State          = CRYPTO_XVIX_XAIX_ECDSA_V_IDLE;                 \
  *Crypto_xVIx_xAIx_ECDSAVrfyCtx.ResultPtr     = CRYPTO_E_VER_NOT_OK;                           \
  Crypto_xVIx_xAIx_ECDSAVrfyCtx.CallbackFlag   = TRUE;                                          \
  Crypto_xVIx_xAIx_ECDSAVrfyCtx.CallbackResult = Crypto_xVIx_xAIx_ECDSAVrfyCtx.ErrorCode;       \
} while(0U)

#if (defined CRYPTO_XVIX_XAIX_ECDSA_V_HANDLE_FAILING_VALIDATE_PUIBKEY)
#error CRYPTO_XVIX_XAIX_ECDSA_V_HANDLE_FAILING_VALIDATE_PUIBKEY is already defined
#endif
/** \brief Handle failing component validation while processing the public key */
#define CRYPTO_XVIX_XAIX_ECDSA_V_HANDLE_FAILING_VALIDATE_PUIBKEY do                             \
{                                                                                               \
  Crypto_xVIx_xAIx_ECDSAVrfyCtx.ErrorCode = E_NOT_OK;                                           \
  if(NULL_PTR != Crypto_xVIx_xAIx_ECDSAVrfyCtx.ResultError)                                     \
  {                                                                                             \
    *Crypto_xVIx_xAIx_ECDSAVrfyCtx.ResultError = Crypto_xVIx_xAIx_ECDSAVrfyCtx.ErrorCode;       \
  }                                                                                             \
  if(NULL_PTR != Crypto_xVIx_xAIx_ECDSAVrfyCtx.IsBusy)                                          \
  {                                                                                             \
    *Crypto_xVIx_xAIx_ECDSAVrfyCtx.IsBusy = FALSE;                                              \
  }                                                                                             \
  Crypto_xVIx_xAIx_ECDSAVrfyCtx.State          = CRYPTO_XVIX_XAIX_ECDSA_V_IDLE;                 \
  Crypto_xVIx_xAIx_ECDSAVrfyCtx.CallbackFlag   = TRUE;                                          \
  Crypto_xVIx_xAIx_ECDSAVrfyCtx.CallbackResult = Crypto_xVIx_xAIx_ECDSAVrfyCtx.ErrorCode;       \
} while(0U)

/*--VerificationState-----------------------------------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_ECDSA_V_IDLE)
#error CRYPTO_XVIX_XAIX_ECDSA_V_IDLE is already defined
#endif
/** \brief Waiting for new operation */
#define CRYPTO_XVIX_XAIX_ECDSA_V_IDLE                          0U

#if (defined CRYPTO_XVIX_XAIX_ECDSA_V_INIT_WAIT)
#error CRYPTO_XVIX_XAIX_ECDSA_V_INIT_WAIT is already defined
#endif
/** \brief The initialization of the subservices is in progress. */
#define CRYPTO_XVIX_XAIX_ECDSA_V_INIT_WAIT                     1U

#if (defined CRYPTO_XVIX_XAIX_ECDSA_V_INITIALIZED)
#error CRYPTO_XVIX_XAIX_ECDSA_V_INITIALIZED is already defined
#endif
/** \brief The Signature generation is initialized */
#define CRYPTO_XVIX_XAIX_ECDSA_V_INITIALIZED                   2U

#if (defined CRYPTO_XVIX_XAIX_ECDSA_V_UPDATE_HASH_MESSAGE)
#error CRYPTO_XVIX_XAIX_ECDSA_V_UPDATE_HASH_MESSAGE is already defined
#endif
/** \brief Hash Message blocks */
#define CRYPTO_XVIX_XAIX_ECDSA_V_UPDATE_HASH_MESSAGE           3U

#if (defined CRYPTO_XVIX_XAIX_ECDSA_V_WAIT_FOR_UPDATE_HASH_MESSAGE)
#error CRYPTO_XVIX_XAIX_ECDSA_V_WAIT_FOR_UPDATE_HASH_MESSAGE is already defined
#endif
/** \brief Wait for the HashUpdate callback */
#define CRYPTO_XVIX_XAIX_ECDSA_V_WAIT_FOR_UPDATE_HASH_MESSAGE  4U

#if (defined CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_HASH_MESSAGE)
#error CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_HASH_MESSAGE is already defined
#endif
/** \brief Finish the Message hashing */
#define CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_HASH_MESSAGE           5U

#if (defined CRYPTO_XVIX_XAIX_ECDSA_V_WAIT_FOR_FINISH_HASH_MESSAGE)
#error CRYPTO_XVIX_XAIX_ECDSA_V_WAIT_FOR_FINISH_HASH_MESSAGE is already defined
#endif
/** \brief Wait for the HashFinish callback */
#define CRYPTO_XVIX_XAIX_ECDSA_V_WAIT_FOR_FINISH_HASH_MESSAGE  6U

#if (defined CRYPTO_XVIX_XAIX_ECDSA_V_RFROMSIGNATURE)
#error CRYPTO_XVIX_XAIX_ECDSA_V_RFROMSIGNATURE is already defined
#endif
/** \brief Check the validity of R. */
#define CRYPTO_XVIX_XAIX_ECDSA_V_RFROMSIGNATURE                7U

#if (defined CRYPTO_XVIX_XAIX_ECDSA_V_SFROMSIGNATURE)
#error CRYPTO_XVIX_XAIX_ECDSA_V_SFROMSIGNATURE is already defined
#endif
/** \brief Check the validity of S. */
#define CRYPTO_XVIX_XAIX_ECDSA_V_SFROMSIGNATURE                8U

#if (defined CRYPTO_XVIX_XAIX_ECDSA_V_INVERSION_S)
#error CRYPTO_XVIX_XAIX_ECDSA_V_INVERSION_S is already defined
#endif
/** \brief Calculate inversion of S. */
#define CRYPTO_XVIX_XAIX_ECDSA_V_INVERSION_S                   9U

#if (defined CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_EQUATION_1)
#error CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_EQUATION_1 is already defined
#endif
/** \brief Calculate  U1 = E * W. */
#define CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_EQUATION_1             10U

#if (defined CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_X1)
#error CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_X1 is already defined
#endif
/** \brief Calculate U1 * Base Point. */
#define CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_X1                     11U

#if (defined CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_EQUATION_2)
#error CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_EQUATION_2 is already defined
#endif
/** \brief Calculate U2 = R * inversion of S. */
#define CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_EQUATION_2             12U

#if (defined CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_X2)
#error CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_X2 is already defined
#endif
/** \brief calculate U2 * public key. */
#define CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_X2                     13U

#if (defined CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_X)
#error CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_X is already defined
#endif
/** \brief Calculate X. */
#define CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_X                      14U

#if (defined CRYPTO_XVIX_XAIX_ECDSA_V_FINISHANDCOMPARE)
#error CRYPTO_XVIX_XAIX_ECDSA_V_FINISHANDCOMPARE is already defined
#endif
/** \brief Compare the result. */
#define CRYPTO_XVIX_XAIX_ECDSA_V_FINISHANDCOMPARE              15U

#endif /* #if (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON) */

/*==[Types]=======================================================================================*/

/*--ECDSAP256Gen----------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)

/** \brief State machine for Signature generation */
typedef uint8 Crypto_xVIx_xAIx_ECDSAGenStateType;

/** \brief Configuration type for Signature generation.
 *
 * \var Crypto_xVIx_xAIx_ECDSAGenConfigType::ObjId
 *  \brief The driver object ID
 *
 * \var Crypto_xVIx_xAIx_ECDSAGenConfigType::ProcessingType
 *  \brief The processing Type
 *
 * \var Crypto_xVIx_xAIx_ECDSAGenConfigType::Job
 *  \brief The processed Job
 *
 * \var Crypto_xVIx_xAIx_ECDSAGenConfigType::HashCfg
 *  \brief The configuration for the hash primitive
 *
 */
typedef struct
{
  uint32                                                         ObjId;
  Crypto_ProcessingType                                          ProcessingType;
  P2VAR(Crypto_JobType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA)     Job;
  Csm_ConfigIdType                                               HashCfg;
}
Crypto_xVIx_xAIx_ECDSAGenConfigType;

/** \brief Context for the Signature generation
 *
 * \var Crypto_xVIx_xAIx_ECDSAGenCtxType::State
 *  \brief The current state the ECDSA Signature generation is in.
 *
 * \var Crypto_xVIx_xAIx_ECDSAGenCtxType::Signature
 *  \brief Pointer to a variable where the Result of the Signature generation is stored.
 *
 * \var Crypto_xVIx_xAIx_ECDSAGenCtxType::SignatureLength
 *  \brief Pointer to a variable where the Result length of the Signature generation is stored.
 *
 * \var Crypto_xVIx_xAIx_ECDSAGenCtxType::Data
 *  \brief Pointer to Message whose Signature has to be generated.
 *
 * \var Crypto_xVIx_xAIx_ECDSAGenCtxType::DataLength
 *  \brief The length of the data whose Signature has to be generated.
 *
 * \var Crypto_xVIx_xAIx_ECDSAGenCtxType::PriKey
 *  \brief An array which is used to store the private key of the signature scheme.
 *
 * \var Crypto_xVIx_xAIx_ECDSAGenCtxType::HashResult
 *  \brief Pointer to the buffer where the hash Value of the data will be saved.
 *
 * \var Crypto_xVIx_xAIx_ECDSAGenCtxType::HashResultLength
 *  \brief The length of the hash digest.
 *
 * \var Crypto_xVIx_xAIx_ECDSAGenCtxType::Tmp1Array
 *  \brief An array which is used as a buffer during Signature generation.
 *
 * \var Crypto_xVIx_xAIx_ECDSAGenCtxType::Tmp2Array
 *  \brief An array which is used as a buffer during Signature generation.
 *
 * \var Crypto_xVIx_xAIx_ECDSAGenCtxType::Tmp3Array
 *  \brief An array which is used as a buffer during Signature generation.
 *
 * \var Crypto_xVIx_xAIx_ECDSAGenCtxType::Tmp4Array
 *  \brief An array which is used as a buffer during Signature generation.
 *
 * \var Crypto_xVIx_xAIx_ECDSAGenCtxType::Tmp5Array
 *  \brief An array which is used as a buffer during Signature generation.
 *
 * \var Crypto_xVIx_xAIx_ECDSAGenCtxType::K
 *  \brief An array which is used to store the random number K.
 *
 * \var Crypto_xVIx_xAIx_ECDSAGenCtxType::KP
 *  \brief A Point, which is used to store the Result of KP = K * Base Point.
 *
 * \var Crypto_xVIx_xAIx_ECDSAGenCtxType::ECCtx
 *  \brief The context which is used for elliptic curve arithmetic.
 *
 * \var Crypto_xVIx_xAIx_ECDSAGenCtxType::ErrorCode
 *  \brief The error code which should be returned when the current task finishes.
 *
 * \var Crypto_xVIx_xAIx_ECDSAGenCtxType::ObjId
 *  \brief The driver object ID.
 *
 * \var Crypto_xVIx_xAIx_ECDSAGenCtxType::Job
 *  \brief The processed Job.
 *
 * \var Crypto_xVIx_xAIx_ECDSAGenCtxType::ProcessingType
 *  \brief The processing type.
 *
 * \var Crypto_xVIx_xAIx_ECDSAGenCtxType::ResultError
 *  \brief The pointer to the error code of the Result.
 *
 * \var Crypto_xVIx_xAIx_ECDSAGenCtxType::IsBusy
 *  \brief The pointer to a variable which indicates if the primitive is Busy.
 *
 * \var Crypto_xVIx_xAIx_ECDSAGenCtxType::CurrentMode
 *  \brief The current operation mode.
 *
 *  \var Crypto_xVIx_xAIx_ECDSAGenCtxType::CancelFlag
 *  \brief This flag indicates if the processing shall be canceled.
 *
 *  \var Crypto_xVIx_xAIx_ECDSAGenCtxType::CallbackFlag
 *  \brief This flag indicates if a callback shall be called at the end of the MainFunction.
 *
 *  \brief Crypto_xVIx_xAIx_ECDSAGenCtxType::CallbackResult
 *  \var The value in this variable will be forwarded to the callback.
 *
 * \var Crypto_xVIx_xAIx_ECDSAGenCtxType::CfgPtr
 *  \brief Pointer to the current ECDSA Signature generation configuration.
 */
typedef struct
{
  Crypto_xVIx_xAIx_ECDSAGenStateType                                        State;
  P2VAR(uint8,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                     Signature;
  P2VAR(uint32,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                     SignatureLength;
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_CONST)                    Data;
  uint32                                                                    DataLength;
  Crypto_xVIx_xAIx_LNWordType                                               PriKey[CRYPTO_XVIX_XAIX_ECC_NUM_LEN_WORDS];
  uint8                                                                     HashResult[CRYPTO_XVIX_XAIX_ECDSAP256_HASH_MAX_LEN];
  uint32                                                                    HashResultLength;
  Crypto_xVIx_xAIx_LNWordType                                               TmpValue[5U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS];
  Crypto_xVIx_xAIx_LNWordType                                               K[CRYPTO_XVIX_XAIX_ECC_NUM_LEN_WORDS];
  Crypto_xVIx_xAIx_ECPointType                                              KP;
  Crypto_xVIx_xAIx_ECCtxType                                                ECCtx;
  Std_ReturnType                                                            ErrorCode;
  uint32                                                                    ObjId;
  P2VAR(Crypto_JobType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA)                Job;
  boolean                                                                   ProcessingType;
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)              ResultError;
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                     IsBusy;
  Crypto_OperationModeType                                                  CurrentMode;
  boolean                                                                   CancelFlag;
  boolean                                                                   CallbackFlag;
  Std_ReturnType                                                            CallbackResult;
  P2CONST(Crypto_xVIx_xAIx_ECDSAGenConfigType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_CONST)  CfgPtr;
}
Crypto_xVIx_xAIx_ECDSAGenCtxType;

#endif /* #if (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON) */

/*--ECDSAP256Vrfy---------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON)

/** \brief State machine for Signature verification */
typedef uint8 Crypto_xVIx_xAIx_ECDSAVrfyStateType;

/** \brief The public Key used for verifying a Signature
 *
 * \var Crypto_xVIx_xAIx_ECDSAVrfyConfigType::ObjId
 *  \brief The driver object ID.
 *
 * \var Crypto_xVIx_xAIx_ECDSAVrfyConfigType::ProcessingType
 *  \brief The processing type.
 *
 * \var Crypto_xVIx_xAIx_ECDSAVrfyConfigType::Job
 *  \brief The pointer to the current Job.
 *
 * \var Crypto_xVIx_xAIx_ECDSAVrfyConfigType::HashCfg
 *  \brief The configuration of the hash primitive
 *
 */
typedef struct
{
  uint32                                                         ObjId;
  Crypto_ProcessingType                                          ProcessingType;
  P2VAR(Crypto_JobType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA)     Job;
  Csm_ConfigIdType                                               HashCfg;
}
Crypto_xVIx_xAIx_ECDSAVrfyConfigType;

/** \brief Context for the Signature verification
 *
 * \var Crypto_xVIx_xAIx_ECDSAVrfyCtxType::State
 *  \brief The current state the ECDSA Signature verification is in.
 *
 * \var Crypto_xVIx_xAIx_ECDSAVrfyCtxType::Signature
 *  \brief Pointer to the buffer where the created Signature should be stored.
 *
 * \var Crypto_xVIx_xAIx_ECDSAVrfyCtxType::SignatureLength
 *  \brief Length of the Signature to verify.
 *
 * \var Crypto_xVIx_xAIx_ECDSAVrfyCtxType::Data
 *  \brief Pointer to Message whose Signature has to be verified.
 *
 * \var Crypto_xVIx_xAIx_ECDSAVrfyCtxType::DataLength
 *  \brief The length of the data whose Signature has to be verified.
 *
 * \var Crypto_xVIx_xAIx_ECDSAVrfyCtxType::ResultPtr
 *  \brief Pointer to a variable where the Result of the Signature verification is stored.
 *
 * \var Crypto_xVIx_xAIx_ECDSAVrfyCtxType::HashResult
 *  brief Pointer to the buffer where the hash Value of the data will be saved.
 *
 * \var Crypto_xVIx_xAIx_ECDSAVrfyCtxType::HashResultLength
 *  \brief The length of the hash digest.
 *
 * \var Crypto_xVIx_xAIx_ECDSAVrfyCtxType::R
 *  \brief An array which is used to store the first half of the signature during Signature verification.
 *
 * \var Crypto_xVIx_xAIx_ECDSAVrfyCtxType::Tmp1Array
 *  \brief An array which is used as a buffer during Signature verification.
 *
 * \var Crypto_xVIx_xAIx_ECDSAVrfyCtxType::Tmp2Array
 *  \brief An array which is used as a buffer during Signature verification.
 *
 * \var Crypto_xVIx_xAIx_ECDSAVrfyCtxType::Tmp3Array
 *  \brief An array which is used as a buffer during Signature verification.
 *
 * \var Crypto_xVIx_xAIx_ECDSAVrfyCtxType::Tmp4Array
 *  \brief An array which is used as a buffer during Signature verification.
 *
 * \var Crypto_xVIx_xAIx_ECDSAVrfyCtxType::Tmp5Array
 *  \brief An array which is used as a buffer during Signature verification.
 *
 * \var Crypto_xVIx_xAIx_ECDSAVrfyCtxType::X
 *  \brief A Point, which is used to store the Result X = U1P + U2Q.
 *
 * \var Crypto_xVIx_xAIx_ECDSAVrfyCtxType::Tmp1Point
 *  \brief A Point, which is used as a buffer during the signature verification.
 *
 * \var Crypto_xVIx_xAIx_ECDSAVrfyCtxType::PubKey
 *  \brief A Point, which is used to store the public key of the signature scheme.
 *
 * \var Crypto_xVIx_xAIx_ECDSAVrfyCtxType::ECCtx
 *  \brief The context which is used for elliptic curve arithmetic.
 *
 * \var Crypto_xVIx_xAIx_ECDSAVrfyCtxType::ErrorCode
 *  \brief The error code which should be returned when the current task finishes.
 *
 * \var Crypto_xVIx_xAIx_ECDSAVrfyCtxType::ObjId
 *  \brief The driver object ID.
 *
 * \var Crypto_xVIx_xAIx_ECDSAVrfyCtxType::Job
 *  \brief The processed Job.
 *
 * \var Crypto_xVIx_xAIx_ECDSAVrfyCtxType::ProcessingType
 *  \brief The processing type.
 *
 * \var Crypto_xVIx_xAIx_ECDSAVrfyCtxType::ResultError
 *  \brief The pointer to the error code of the Result.
 *
 * \var Crypto_xVIx_xAIx_ECDSAVrfyCtxType::IsBusy
 *  \brief The pointer to a variable which indicates if the primitive is Busy.
 *
 * \var Crypto_xVIx_xAIx_ECDSAVrfyCtxType::CurrentMode
 *  \brief The current operation mode.
 *
 *  \var Crypto_xVIx_xAIx_ECDSAVrfyCtxType::CancelFlag
 *  \brief This flag indicates if the processing shall be canceled.
 *
 *  \var Crypto_xVIx_xAIx_ECDSAVrfyCtxType::CallbackFlag
 *  \brief This flag indicates if a callback shall be called at the end of the MainFunction.
 *
 *  \brief Crypto_xVIx_xAIx_ECDSAVrfyCtxType::CallbackResult
 *  \var The value in this variable will be forwarded to the callback.
 *
 * \var Crypto_xVIx_xAIx_ECDSAVrfyCtxType::CfgPtr
 *  \brief Pointer to the current ECDSA Signature generation configuration.
 */
typedef struct
{
  Crypto_xVIx_xAIx_ECDSAVrfyStateType                                       State;
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_CONST)                    Signature;
  uint32                                                                    SignatureLength;
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_CONST)                    Data;
  uint32                                                                    DataLength;
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)     ResultPtr;
  uint8                                                                     HashResult[CRYPTO_XVIX_XAIX_ECDSAP256_HASH_MAX_LEN];
  uint32                                                                    HashResultLength;
  Crypto_xVIx_xAIx_LNWordType                                               R[CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS];
  Crypto_xVIx_xAIx_LNWordType                                               TmpValue[5U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS];
  Crypto_xVIx_xAIx_ECPointType                                              X;
  Crypto_xVIx_xAIx_ECPointType                                              Tmp1Point;
  Crypto_xVIx_xAIx_ECPointType                                              PubKey;
  Crypto_xVIx_xAIx_ECCtxType                                                ECCtx;
  Std_ReturnType                                                            ErrorCode;
  uint32                                                                    ObjId;
  P2VAR(Crypto_JobType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA)                Job;
  boolean                                                                   ProcessingType;
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)              ResultError;
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                     IsBusy;
  Crypto_OperationModeType                                                  CurrentMode;
  boolean                                                                   CancelFlag;
  boolean                                                                   CallbackFlag;
  Std_ReturnType                                                            CallbackResult;
  P2CONST(Crypto_xVIx_xAIx_ECDSAVrfyConfigType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA)  CfgPtr;
}
Crypto_xVIx_xAIx_ECDSAVrfyCtxType;

#endif /* #if (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON) */

/*==[Declaration of functions with internal linkage]==============================================*/
/*--ECDSAP256Gen----------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief AL Callback for other primitives
 *
 *         This function is a callback function for other primitives
 *
 * \param[in,out] Job  A pointer to the current Job
 * \param[in] Result   The Result of the process
 *
 * \Reentrancy Nonreentrant
 * \Synchronicity Synchronous/Asynchronous
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ECCNIST_NOT_SET_CallBack
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint8                                                        Result
);

/**  \brief  Crypto ECDSA Generate Reset
 *
 *           This Function resets the Context of ECDSA Generate.
 *
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ECCNIST_NOT_SET_Reset
(
  void
);

/** \brief State helper function: This function performs the required processing for the
 *         CRYPTO_XVIX_XAIX_ECDSA_G_UDAPTE_HASH_MESSAGE state.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSAGenUpdateMessage
(
  void
);

/** \brief State helper function: This function performs the required processing for the
 *         CRYPTO_XVIX_XAIX_ECDSA_G_FINISH_HASH_MESSAGE state.
 *
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSAGenFinishMessage
(
  void
);

/** \brief State helper function: This function performs the required processing
 *         for the state CRYPTO_XVIX_XAIX_ECDSA_G_RANDOM.
 *
 *         This helper function generates a random number and performs the multiply
 *         when performing the signature generation.
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSACalculateK
(
  void
);

/** \brief State helper function: This function performs the required processing
 *         for the state CRYPTO_XVIX_XAIX_ECDSA_G_VALIDATEK.
 *
 *         This helper function generates a random number and performs the multiply
 *         when performing the signature generation.
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSAValidateK
(
  void
);

/** \brief State helper function: This function performs the required processing
 *         for the state CRYPTO_XVIX_XAIX_ECDSA_G_KMULTP.
 *
 *         This helper function performs the multiply R = k * Base point
 *         when performing the signature generation.
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSACalculateR1
(
  void
);

/** \brief State helper function: This function performs the required processing for the state
 *         CRYPTO_XVIX_XAIX_ECDSA_G_FINISHR.
 *         This helper function This helper function converts R in affine coordinates and checks its
 *         validity when performing the signature generation.
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSACalculateR2
(
  void
);

/** \brief State helper function: This function performs the required processing for
 *         the state CRYPTO_XVIX_XAIX_ECDSA_G_FINISH_SIG.
 *         This helper function calculates the second half of the signature
 *         when performing the signature generation.
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSACalculateS
(
  void
);

/** \brief State helper function: This function performs the required processing for the state
 *         CRYPTO_XVIX_XAIX_ECDSA_G_EXPORT_SIG.
 *         This helper function prepares the final signature
 *         when performing the signature generation.
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSAFinishSignature
(
  void
);

/** \brief Helper function for the state Default state.
 *
 *         This function is used to perform the common processing for many state,
 *         which have the same processing.
 *
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECCNIST_GenDefaultWaitState
(
  void
);

/** \brief This function calls the helper hash start function.
 *
 * \param[in] Ctx  A pointer to the context.
 *
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}

 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECCNIST_GEN_HASH_Start
(
  P2VAR(Crypto_xVIx_xAIx_ECDSAGenCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Ctx
);

/**  \brief This function calls the helper hash update function.
 *
 * \param[in] Ctx         A pointer to the context.
 * \param[in] DataPtr     A pointer to the data.
 * \param[in] DataLength  The length of the data.
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECCNIST_GEN_HASH_Update
(
  P2CONST(Crypto_xVIx_xAIx_ECDSAGenCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Ctx,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                            DataPtr,
  uint32                                                                           DataLength
);

/** \brief This function calls the helper hash finish function.
 *
 * \param[in] Ctx        A pointer to the context.
 * \param[out] ResPtr    A pointer to where the result is to be stored.
 * \param[in] ResLenPtr  A pointer where the length of the result is to be stored.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECCNIST_GEN_HASH_Finish
(
  P2CONST(Crypto_xVIx_xAIx_ECDSAGenCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Ctx,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                              ResPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                             ResLenPtr
);

/** \brief This is the callback function of the underlying hash primitive.
 *
 * \param[in,out] Job     Reference to the currently processed job.
 * \param[in]     Result  The error value.
 *
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECCNIST_GEN_HASH_Callback
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  Std_ReturnType                                               Result
);

/** \brief Function to perform the callback and job cancellation.
 *
 *         This function is implemented in order to perform the job cancellation
 *         and callback functionality is a single call, instead of having it in the
 *         Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ECCNIST_NOT_SET_MainFunction.
 *
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECCNIST_GEN_Callback_Cancel
(
  void
);

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON) */

/*--ECDSAP256Vrfy---------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON)

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief AL Callback for other primitives
 *
 *         This function is a callback function for other primitives
 *
 * \param[in,out] Job  A pointer to the current Job
 * \param[in] Result   The Result of the process
 *
 * \Reentrancy Nonreentrant
 * \Synchronicity Synchronous/Asynchronous
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ECCNIST_NOT_SET_CallBack
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint8                                                        Result
);

/**  \brief  Crypto ECDSA Verify Reset
 *
 *           This Function resets the Context of ECDSA verification.
 *
 * \returns  void
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ECCNIST_NOT_SET_Reset
(
  void
);

/**  \brief This function calls the helper hash start function.
 *
 * \param[in] Ctx  A pointer to the context.
 *
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}

 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECCNIST_VRFY_HASH_Start
(
  P2VAR(Crypto_xVIx_xAIx_ECDSAVrfyCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Ctx
);

/**  \brief This function calls the helper hash update function.
 *
 * \param[in] Ctx         A pointer to the context.
 * \param[in] DataPtr     A pointer to the data.
 * \param[in] DataLength  The length of the data.
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECCNIST_VRFY_HASH_Update
(
  P2CONST(Crypto_xVIx_xAIx_ECDSAVrfyCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Ctx,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                             DataPtr,
  uint32                                                                            DataLength
);

/** \brief This function calls the helper hash finish function.
 *
 * \param[in] Ctx        A pointer to the context.
 * \param[out] ResPtr    A pointer to where the result is to be stored.
 * \param[in] ResLenPtr  A pointer where the length of the result is to be stored.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECCNIST_VRFY_HASH_Finish
(
  P2CONST(Crypto_xVIx_xAIx_ECDSAVrfyCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Ctx,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                               ResPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                              ResLenPtr
);

/**  \brief This function checks the validation of public key.
 *
 * \param[in] Point  A pointer to the point, which should be validated.
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSAPubKeyValidation
(
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Point,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)     CtxPtr
);

/**  \brief This function checks if the point lies on the Elliptic Curve defined by A and B.
 *
 * \param[in] Point  A pointer to the point, which should be validated.
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSAPubKeyCheckEquation
(
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Point,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)     CtxPtr
);

/** \brief State helper function: This function performs the required processing for the
 *         CRYPTO_XVIX_XAIX_ECDSA_V_UAPTE_HASH_MESSAGE state.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSAVrfyUpdateMessage
(
  void
);

/** \brief State helper function: This function performs the required processing for the
 *         CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_HASH_MESSAGE state.
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSAVrfyFinishMessage
(
  void
);

/** \brief State helper function: This function performs the required processing for the
 *         CRYPTO_XVIX_XAIX_ECDSA_V_RFROMSIGNATURE state.
 *         This function checks the validation of first half of the signature, R.
 *
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSAFinishR
(
  void
);

/** \brief State helper function: This function performs the required processing for the
 *         CRYPTO_XVIX_XAIX_ECDSA_V_SFROMSIGNATURE state.
 *         This function checks the validation of last half of the signature, S.
 *
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSAFinishS
(
  void
);

/** \brief State helper function: This function performs the required processing for the
 *         CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_EQUATION_1 state.
 *         This function calculates U1 = H(M) * inversion of S.
 *
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSACalculateU1
(
  void
);

/** \brief State helper function: This function performs the required processing for the
 *         CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_EQUATION_2 state.
 *         This function calculates U1 = R * inversion of S.
 *
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSACalculateU2
(
  void
);

/** \brief State helper function: This function performs the required processing for the
 *         CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_X state.
 *         This function calculates X = U1*Base Point + U2*public key
 *
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSACalculateX
(
  void
);

/** \brief State helper function: This function performs the required processing for the
 *         CRYPTO_XVIX_XAIX_ECDSA_V_FINISHANDCOMPARE state.
 *         This function validates the received signature.
 *
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSAValidateSignature
(
  void
);

/** \brief This is the callback function of the underlying hash primitive.
 *
 * \param[in,out] Job     Reference to the currently processed job.
 * \param[in]     Result  The error value.
 *
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECCNIST_VRFY_HASH_Callback
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  Std_ReturnType                                     Result
);

/** \brief Helper function for the state Default state.
 *
 *         This function is used to perform the common processing for many state,
 *         which have the same processing.
 *
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECCNIST_VrfyDefaultWaitState
(
  void
);

/** \brief Function to perform the callback and job cancellation.
 *
 *     This function is implemented in order to perform the job cancellation
 *     and callback functionality is a single call, instead of having it in the
 *     Crypto_xVIx_xAIx_AL_SIGNATUREGVERIFY_ECCNIST_NOT_SET_MainFunction.
 *
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECCNIST_VRFY_Callback_Cancel
(
  void
);

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON) */

/*==[Constants with internal linkage]=============================================================*/

/*==[Variables with internal linkage]=============================================================*/

/*--ECDSAP256Gen----------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief ECDSA Signature generation context. */
static VAR(Crypto_xVIx_xAIx_ECDSAGenCtxType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_ECDSAGenCtx;

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_CLEARED_8
#include <Crypto_xVIx_xAIx_MemMap.h>

static VAR(uint8, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_RetryCounter = 0U;

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_CLEARED_8
#include <Crypto_xVIx_xAIx_MemMap.h>

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_INIT_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief The PrimitiveInfo for the job used in the ECDSA signature generation helper. */
static VAR(Crypto_PrimitiveInfoType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_HelperECDSAGenPrimitiveInfo =
{
  CRYPTO_XVIX_XAIX_ECDSAP256_SIGN_SIZE,                    /* resultLength */
  CRYPTO_SIGNATUREGENERATE,                                /* service */
  {
    CRYPTO_ALGOFAM_ECCNIST,                                /* family */
    CRYPTO_ALGOFAM_SHA2_256,                               /* secondaryFamily */
    CRYPTO_XVIX_XAIX_AL_ASYM_PRIVATE_MAX_KEY_ELEMENT_SIZE, /* keyLength */
    CRYPTO_ALGOMODE_NOT_SET                                /* mode */
  }                                                        /* algorithm */
};

/** \brief The JobPrimitiveInfo for the job used in the ECDSA signature generation helper. */
static VAR(Crypto_JobPrimitiveInfoType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_HelperECDSAGenJobPrimitiveInfo =
{
  0U,                                            /* callbackId */
  &Crypto_xVIx_xAIx_HelperECDSAGenPrimitiveInfo, /* primitiveInfo */
  0U,                                            /* secureCounterId */
  0U,                                            /* cryIfKeyId */
  CRYPTO_PROCESSING_SYNC,                        /* processingType */
  FALSE                                          /* callbackUpdateNotification */
};

/** \brief The JobInfo for the job used in the ECDSA signature generation helper. */
static VAR(Crypto_JobInfoType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_HelperECDSAGenJobInfo =
{
  0U, /* jobId */
  0U  /* jobPriority*/
};

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON) */

/*--ECDSAP256Vrfy---------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON)

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief ECDSA Signature verification context. */
static VAR(Crypto_xVIx_xAIx_ECDSAVrfyCtxType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_ECDSAVrfyCtx;

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_INIT_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief The PrimitiveInfo for the job used in the ECDSA signature verification helper. */
static VAR(Crypto_PrimitiveInfoType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_HelperECDSAVrfyPrimitiveInfo =
{
  CRYPTO_XVIX_XAIX_ECDSAP256_SIGN_SIZE,                   /* resultLength */
  CRYPTO_SIGNATUREVERIFY,                                 /* service */
  {
    CRYPTO_ALGOFAM_ECCNIST,                               /* family */
    CRYPTO_ALGOFAM_SHA2_256,                              /* secondaryFamily */
    CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_MAX_KEY_ELEMENT_SIZE, /* keyLength */
    CRYPTO_ALGOMODE_NOT_SET                               /* mode */
  }                                                       /* algorithm */
};

/** \brief The JobPrimitiveInfo for the job used in the ECDSA signature verification helper. */
static VAR(Crypto_JobPrimitiveInfoType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_HelperECDSAVrfyJobPrimitiveInfo =
{
  0U,                                             /* callbackId */
  &Crypto_xVIx_xAIx_HelperECDSAVrfyPrimitiveInfo, /* primitiveInfo */
  0U,                                             /* secureCounterId */
  0U,                                             /* cryIfKeyId */
  CRYPTO_PROCESSING_SYNC,                         /* processingType */
  FALSE                                           /* callbackUpdateNotification */
};

/** \brief The JobInfo for the job used in the ECDSA signature verification helper. */
static VAR(Crypto_JobInfoType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_HelperECDSAVrfyJobInfo =
{
  0U, /* jobId */
  0U  /* jobPriority*/
};

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON) */

/*==[Constants with external linkage]=============================================================*/

/*==[Variables with external linkage]=============================================================*/

/*==[Definition of functions with external linkage]===============================================*/

/*--ECDSAP256Gen----------------------------------------------------------------------------------*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#if (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)

/* !LINKSTO EB_Crypto_CryptAlgos_1255, 1, EB_Crypto_CryptAlgos_0974, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ECCNIST_NOT_SET_Start
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint8 Key[CRYPTO_XVIX_XAIX_ECDSAP256_PRIKEY_LEN] = { 0U };
  uint32 KeyLength = CRYPTO_XVIX_XAIX_ECDSAP256_PRIKEY_LEN;
  Crypto_xVIx_xAIx_ECDSAGenConfigType Cfg = { 0U };

  /* !LINKSTO SWS_Crypto_00017, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Start, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Start2Start, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Update2Start, 1 */
  Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ECCNIST_NOT_SET_Reset();
  Crypto_xVIx_xAIx_ECDSAGenCtx.CfgPtr = &Cfg;

  /* The function can only return E_OK here. */
  (void)Crypto_xVIx_xAIx_GetECIndex
        (
          Job->jobPrimitiveInfo->primitiveInfo->service,
          Job->jobPrimitiveInfo->primitiveInfo->algorithm.family,
          NULL_PTR,
          0U,
          &(Crypto_xVIx_xAIx_ECDSAGenCtx.ECCtx.CurveIndex)
        );

  RetVal = Crypto_xVIx_xAIx_AL_KeyElementGet
           (
             Job->cryptoKeyId,
             CRYPTO_KE_SIGNATURE_KEY,
             Key,
             &KeyLength
           );

  /* !LINKSTO EB_Crypto_CryptAlgos_1610, 1 */
  if (CRYPTO_XVIX_XAIX_ECDSAP256_PRIKEY_LEN != KeyLength)
  {
    RetVal = CRYPTO_E_KEY_SIZE_MISMATCH;
  }
  else
  {
    if (E_OK == RetVal)
    {
      Crypto_xVIx_xAIx_ECDSAGenCtx.ObjId       = ObjectId;
      Crypto_xVIx_xAIx_ECDSAGenCtx.Job         = Job;
      Crypto_xVIx_xAIx_ECDSAGenCtx.CurrentMode = CRYPTO_OPERATIONMODE_START;

      /* The return value is ignored because the parameters are either from the context where they are
         properly defined or local variables which are properly defined before passing them to
         the function.*/
      (void)Crypto_xVIx_xAIx_LNBEByteArrayToNumber
      (
        Key,
        KeyLength,
        Crypto_xVIx_xAIx_ECDSAGenCtx.PriKey,
        CRYPTO_XVIX_XAIX_ECC_NUM_LEN_WORDS
      );

      Crypto_xVIx_xAIx_ECCNIST_GEN_HASH_Start(&Crypto_xVIx_xAIx_ECDSAGenCtx);

      if(
          CRYPTO_PROCESSING_SYNC ==
          Crypto_xVIx_xAIx_ECDSAGenCtx.Job->jobPrimitiveInfo->processingType
        )
      {
        Crypto_xVIx_xAIx_ECDSAGenCtx.State = CRYPTO_XVIX_XAIX_ECDSA_G_INITIALIZED;
        Crypto_xVIx_xAIx_ECDSAGenCtx.CallbackFlag = TRUE;
        Crypto_xVIx_xAIx_ECDSAGenCtx.CallbackResult = E_OK;
      }
      else
      {
        Crypto_xVIx_xAIx_ECDSAGenCtx.State = CRYPTO_XVIX_XAIX_ECDSA_G_INIT_WAIT;
      }
      Crypto_xVIx_xAIx_ECDSAGenCtx.ErrorCode = E_OK;
    }
    else
    {
      RetVal = E_NOT_OK;
    }
  }
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1256, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ECCNIST_NOT_SET_Update
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal;
  TS_PARAM_UNUSED(ObjectId);

  if(CRYPTO_XVIX_XAIX_ECDSA_G_INITIALIZED == Crypto_xVIx_xAIx_ECDSAGenCtx.State)
  {
    /* Store data into context */
    Crypto_xVIx_xAIx_ECDSAGenCtx.Data       = Job->jobPrimitiveInputOutput.inputPtr;
    Crypto_xVIx_xAIx_ECDSAGenCtx.DataLength = Job->jobPrimitiveInputOutput.inputLength;

    /* Set state of the state machine to update */
    Crypto_xVIx_xAIx_ECDSAGenCtx.State  = CRYPTO_XVIX_XAIX_ECDSA_G_UDAPTE_HASH_MESSAGE;
    Crypto_xVIx_xAIx_ECDSAGenCtx.ErrorCode = E_OK;
    RetVal = E_OK;
  }
  else
  {
    RetVal = CRYPTO_E_BUSY;
  }
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1257, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ECCNIST_NOT_SET_Finish
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  TS_PARAM_UNUSED(ObjectId);

  if(
      (CRYPTO_XVIX_XAIX_ECDSA_G_INITIALIZED == Crypto_xVIx_xAIx_ECDSAGenCtx.State) &&
      (CRYPTO_OPERATIONMODE_UPDATE == Crypto_xVIx_xAIx_ECDSAGenCtx.CurrentMode)
     )
  {
    /* Store data into context */
    Crypto_xVIx_xAIx_ECDSAGenCtx.Signature       = Job->jobPrimitiveInputOutput.outputPtr;
    Crypto_xVIx_xAIx_ECDSAGenCtx.SignatureLength = Job->jobPrimitiveInputOutput.outputLengthPtr;

    /* Set state of the state machine to Update */
    Crypto_xVIx_xAIx_ECDSAGenCtx.State     = CRYPTO_XVIX_XAIX_ECDSA_G_FINISH_HASH_MESSAGE;
    Crypto_xVIx_xAIx_ECDSAGenCtx.ErrorCode = E_OK;
    RetVal = E_OK;
  }
  else
  {
    RetVal = CRYPTO_E_BUSY;
  }
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1258, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ECCNIST_NOT_SET_Cancel
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  /* This state indicates, that the Primitive is currently waiting for an Update/Finish call */
  if(
      CRYPTO_XVIX_XAIX_ECDSA_G_INITIALIZED ==
      Crypto_xVIx_xAIx_ECDSAGenCtx.State
    )
  {
    /* !LINKSTO SWS_Crypto_00122_CORRECTION, 1 */
    Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ECCNIST_NOT_SET_Reset();
    /* !LINKSTO EB_Crypto_00063, 1 */
    RetVal = E_OK;
  }
  else
  {
    /* Job can not be canceled imideatly */
    Crypto_xVIx_xAIx_ECDSAGenCtx.CancelFlag = TRUE;
    /* !LINKSTO SWS_Crypto_00143_CORRECTION_SWS_Crypto_00183, 1 */
    RetVal = CRYPTO_E_JOB_CANCELED;
  }
  TS_PARAM_UNUSED(Job);
  TS_PARAM_UNUSED(ObjectId);
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1259, 1 */
FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ECCNIST_NOT_SET_MainFunction
(
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
)
{
  if(!Crypto_xVIx_xAIx_CheckSkipMain(
                                      Result,
                                      Busy,
                                      Crypto_xVIx_xAIx_ECDSAGenCtx.ObjId
                                    )
    )
  {
    Crypto_xVIx_xAIx_ECDSAGenCtx.IsBusy = Busy;
    Crypto_xVIx_xAIx_ECDSAGenCtx.ResultError = Result;
    if((NULL_PTR != Result) && (NULL_PTR != Busy))
    {
      *Crypto_xVIx_xAIx_ECDSAGenCtx.IsBusy = TRUE;
      *Crypto_xVIx_xAIx_ECDSAGenCtx.ResultError = E_OK;
    }
    switch (Crypto_xVIx_xAIx_ECDSAGenCtx.State)
    {
      case CRYPTO_XVIX_XAIX_ECDSA_G_UDAPTE_HASH_MESSAGE:
      {
        Crypto_xVIx_xAIx_ECDSAGenUpdateMessage();
        break;
      }

      case CRYPTO_XVIX_XAIX_ECDSA_G_FINISH_HASH_MESSAGE:
      {
        Crypto_xVIx_xAIx_ECDSAGenFinishMessage();
        break;
      }

      case CRYPTO_XVIX_XAIX_ECDSA_G_RANDOM:
      {
        Crypto_xVIx_xAIx_ECDSACalculateK();
        break;
      }

      case CRYPTO_XVIX_XAIX_ECDSA_G_VALIDATEK:
      {
        Crypto_xVIx_xAIx_ECDSAValidateK();
        break;
      }

      case CRYPTO_XVIX_XAIX_ECDSA_G_KMULTP:
      {
        Crypto_xVIx_xAIx_ECDSACalculateR1();
        break;
      }

      case CRYPTO_XVIX_XAIX_ECDSA_G_FINISHR:
      {
        Crypto_xVIx_xAIx_ECDSACalculateR2();
        break;
      }

      case CRYPTO_XVIX_XAIX_ECDSA_G_FINISH_SIG:
      {
        Crypto_xVIx_xAIx_ECDSACalculateS();
        break;
      }

      case CRYPTO_XVIX_XAIX_ECDSA_G_EXPORT_SIG:
      {
        Crypto_xVIx_xAIx_ECDSAFinishSignature();
        break;
      }

      default:
      {
        Crypto_xVIx_xAIx_ECCNIST_GenDefaultWaitState();
        break;
      }
    }

   Crypto_xVIx_xAIx_ECCNIST_GEN_Callback_Cancel();
  }
}

/* !LINKSTO EB_Crypto_CryptAlgos_1633, 1 */
/* CHECK: NOPARSE */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE)
  Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ECCNIST_NOT_SET_Helper_SyncSingleCall
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                            InputPtr,
  uint32                                                                           InputLength,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                              SignaturePtr,
  uint32                                                                           SignatureLength,
  P2VAR(Crypto_xVIx_xAIx_AsymPublicKeyType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Key
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint32 ObjId          = 0U;
  boolean Busy          = FALSE;
  Crypto_xVIx_xAIx_ECDSAGenConfigType Cfg = { 0U };

  Crypto_JobType Job =
  {
    0U,                                               /* jobId */
    CRYPTO_JOBSTATE_IDLE,                             /* jobState */
    {
      NULL_PTR,                                       /* inputPtr */
      0U,                                             /* inputLength */
      NULL_PTR,                                       /* secondaryInputPtr */
      0U,                                             /* secondaryInputLength */
      NULL_PTR,                                       /* tertiaryInputPtr */
      0U,                                             /* tertiaryInputLength */
      NULL_PTR,                                       /* outputPtr */
      NULL_PTR,                                       /* outputLengthPtr */
      NULL_PTR,                                       /* secondaryOutputPtr */
      NULL_PTR,                                       /* secondaryOutputLengthPtr */
      NULL_PTR,                                       /* verifyPtr */
      CRYPTO_OPERATIONMODE_SINGLECALL                 /* mode */
    },
    &Crypto_xVIx_xAIx_HelperECDSAGenJobPrimitiveInfo, /* jobPrimitiveInfo */
    &Crypto_xVIx_xAIx_HelperECDSAGenJobInfo,          /* jobInfo */
    0U                                                /* cryptoKeyId */
  };

  /* Set the job */
  Job.jobPrimitiveInputOutput.inputPtr        = InputPtr;
  Job.jobPrimitiveInputOutput.inputLength     = InputLength;
  Job.jobPrimitiveInputOutput.outputPtr       = SignaturePtr;
  Job.jobPrimitiveInputOutput.outputLengthPtr = &SignatureLength;
  /*only process if the DriverObjectState is not busy.*/
  SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
  if(
      CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE ==
        Crypto_xVIx_xAIx_DriverObjects[ObjId].DriverObjectState
    )
  {
    Crypto_xVIx_xAIx_DriverObjects[ObjId].DriverObjectState =
      CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_BUSY;
    Crypto_xVIx_xAIx_DriverObjects[ObjId].CurrentJob = &Job;
    SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
    /* !LINKSTO Crypto.Dsn.JobTransition.Idle2Active, 1 */
    /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Start, 1 */
    /* !LINKSTO Crypto.Dsn.JobTransition.Start2Start, 1 */
    /* !LINKSTO Crypto.Dsn.JobTransition.Update2Start, 1 */
    Job.jobState = CRYPTO_JOBSTATE_ACTIVE;
    /*perform start tasks */
    Crypto_xVIx_xAIx_ECDSAGenCtx.CfgPtr = &Cfg;

    /* !LINKSTO EB_Crypto_CryptAlgos_1610, 1 */
    if (CRYPTO_XVIX_XAIX_ECDSAP256_PRIKEY_LEN != Key->length)
    {
      RetVal = E_NOT_OK;
    }
    else
    {
      Crypto_xVIx_xAIx_ECDSAGenCtx.ObjId       = ObjId;
      Crypto_xVIx_xAIx_ECDSAGenCtx.Job         = &Job;
      Crypto_xVIx_xAIx_ECDSAGenCtx.CurrentMode = CRYPTO_OPERATIONMODE_START;
      /* The return value is ignored because the parameters are either from the context where they are
         properly defined or local variables which are properly defined before passing them to
         the function.*/
      (void)Crypto_xVIx_xAIx_LNBEByteArrayToNumber
      (
        Key->data,
        Key->length,
        Crypto_xVIx_xAIx_ECDSAGenCtx.PriKey,
        CRYPTO_XVIX_XAIX_ECC_NUM_LEN_WORDS
      );

      /* Call the helper hash start function. */
      Crypto_xVIx_xAIx_ECCNIST_GEN_HASH_Start(&Crypto_xVIx_xAIx_ECDSAGenCtx);

      /* ProcessingType is set to CRYPTO_PROCESSING_SYNC*/
      Crypto_xVIx_xAIx_ECDSAGenCtx.State = CRYPTO_XVIX_XAIX_ECDSA_G_INITIALIZED;
      Crypto_xVIx_xAIx_ECDSAGenCtx.CallbackFlag = TRUE;
      Crypto_xVIx_xAIx_ECDSAGenCtx.CallbackResult = E_OK;

      Crypto_xVIx_xAIx_ECDSAGenCtx.ErrorCode = E_OK;
      /* In this context for the Hash Start the MainFunction has only to be called once */
      Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ECCNIST_NOT_SET_MainFunction(&RetVal, &Busy);

      if(E_OK == RetVal)
      {
        RetVal = Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ECCNIST_NOT_SET_Update(ObjId, &Job);
        do
        {
          Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ECCNIST_NOT_SET_MainFunction(&RetVal, &Busy);
        }while((TRUE == Busy) && (E_OK == RetVal));
      }

      if(E_OK == RetVal)
      {
        RetVal = Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ECCNIST_NOT_SET_Finish(ObjId, &Job);
        do
        {
          Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ECCNIST_NOT_SET_MainFunction(&RetVal, &Busy);
        }while((TRUE == Busy) && (E_OK == RetVal));
      }
    }
    /*set DriverObjectState back to idle after processing of the function. Also
    set the currentJob to NULL_PTR.*/
    SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
    Crypto_xVIx_xAIx_DriverObjects[ObjId].DriverObjectState =
      CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
    Crypto_xVIx_xAIx_DriverObjects[ObjId].CurrentJob = NULL_PTR;
    Crypto_xVIx_xAIx_DriverObjects[ObjId].SkipPeriodicMainFunction = TRUE;
    SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
  }
  else
  {
    SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
    RetVal = CRYPTO_E_BUSY;
  }
  return RetVal;
}
/* CHECK: PARSE */

#endif /* #if (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON) */

/*--ECDSAP256Vrfy---------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON)

/* !LINKSTO EB_Crypto_CryptAlgos_1260, 1, EB_Crypto_CryptAlgos_0974, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ECCNIST_NOT_SET_Start
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  Crypto_xVIx_xAIx_ECDSAVrfyConfigType Cfg = { 0U };
  uint8 Key[CRYPTO_XVIX_XAIX_ECDSAP256_PUBKEY_LEN] = { 0U };
  uint32 KeyLength = CRYPTO_XVIX_XAIX_ECDSAP256_PUBKEY_LEN;
  uint32 PubKeyLength;

  /* !LINKSTO SWS_Crypto_00017, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Start, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Start2Start, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Update2Start, 1 */
  Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ECCNIST_NOT_SET_Reset();
  Crypto_xVIx_xAIx_ECDSAVrfyCtx.CfgPtr = &Cfg;

  /* The function can only return E_OK here. */
  (void)Crypto_xVIx_xAIx_GetECIndex
        (
          Job->jobPrimitiveInfo->primitiveInfo->service,
          Job->jobPrimitiveInfo->primitiveInfo->algorithm.family,
          NULL_PTR,
          0U,
          &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.ECCtx.CurveIndex)
        );

  RetVal = Crypto_xVIx_xAIx_AL_KeyElementGet
          (
            Job->cryptoKeyId,
            CRYPTO_KE_SIGNATURE_KEY,
            Key,
            &KeyLength
          );
  /* !LINKSTO EB_Crypto_CryptAlgos_1611, 1 */
  if (CRYPTO_XVIX_XAIX_ECDSAP256_PUBKEY_LEN != KeyLength)
  {
    RetVal = CRYPTO_E_KEY_SIZE_MISMATCH;
  }
  else
  {
    if (E_OK == RetVal)
    {
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.ObjId       = ObjectId;
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.Job         = Job;
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.CurrentMode = CRYPTO_OPERATIONMODE_START;
      /* !LINKSTO EB_Crypto_CryptAlgos_1607, 1 */
      /* Set the public key */
      PubKeyLength = ((uint32)((uint32)KeyLength) >> 3U);
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.PubKey.XValue[0U] = PubKeyLength;
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.PubKey.YValue[0U] = PubKeyLength;
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.PubKey.ZValue[0U] = 0x01U;
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.PubKey.ZValue[1U] = 0x01U;

      /* The return value is ignored because the parameters are either from the context where they are
         properly defined or local variables which are properly defined before passing them to
         the function.*/
      /* !LINKSTO EB_Crypto_CryptAlgos_1608, 1 */
      (void)Crypto_xVIx_xAIx_LNBEByteArrayToNumber
      (
        Key,
        (KeyLength >> (uint32)1UL),
        Crypto_xVIx_xAIx_ECDSAVrfyCtx.PubKey.XValue,
        CRYPTO_XVIX_XAIX_ECC_NUM_LEN_WORDS
      );
      /* !LINKSTO EB_Crypto_CryptAlgos_1609, 1 */
      (void)Crypto_xVIx_xAIx_LNBEByteArrayToNumber
      (
        &(Key[(CRYPTO_XVIX_XAIX_ECDSAP256_PUBKEY_LEN >> 1U)]),
        (KeyLength >> (uint32)1UL),
        Crypto_xVIx_xAIx_ECDSAVrfyCtx.PubKey.YValue,
        CRYPTO_XVIX_XAIX_ECC_NUM_LEN_WORDS
      );

      Crypto_xVIx_xAIx_ECCNIST_VRFY_HASH_Start(&Crypto_xVIx_xAIx_ECDSAVrfyCtx);

      if(
          CRYPTO_PROCESSING_SYNC ==
          Crypto_xVIx_xAIx_ECDSAVrfyCtx.Job->jobPrimitiveInfo->processingType
        )
      {
        Crypto_xVIx_xAIx_ECDSAVrfyCtx.State = CRYPTO_XVIX_XAIX_ECDSA_V_INITIALIZED;
        Crypto_xVIx_xAIx_ECDSAVrfyCtx.CallbackFlag = TRUE;
        Crypto_xVIx_xAIx_ECDSAVrfyCtx.CallbackResult = E_OK;
      }
      else
      {
        Crypto_xVIx_xAIx_ECDSAVrfyCtx.State = CRYPTO_XVIX_XAIX_ECDSA_V_INIT_WAIT;
      }
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.ErrorCode = E_OK;
      /* !LINKSTO EB_Crypto_CryptAlgos_1613, 1 */
      /* Check the validity of public key */
      RetVal = Crypto_xVIx_xAIx_ECDSAPubKeyValidation(&Crypto_xVIx_xAIx_ECDSAVrfyCtx.PubKey, &Crypto_xVIx_xAIx_ECDSAVrfyCtx.ECCtx);
    }
    else
    {
      RetVal = E_NOT_OK;
    }
  }
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1261, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ECCNIST_NOT_SET_Update
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal;
  TS_PARAM_UNUSED(ObjectId);

  if(CRYPTO_XVIX_XAIX_ECDSA_V_INITIALIZED == Crypto_xVIx_xAIx_ECDSAVrfyCtx.State)
  {
    /* Store data into context */
    Crypto_xVIx_xAIx_ECDSAVrfyCtx.Data       = Job->jobPrimitiveInputOutput.inputPtr;
    Crypto_xVIx_xAIx_ECDSAVrfyCtx.DataLength = Job->jobPrimitiveInputOutput.inputLength;

    /* Set state of the state machine to update */
    Crypto_xVIx_xAIx_ECDSAVrfyCtx.State     = CRYPTO_XVIX_XAIX_ECDSA_V_UPDATE_HASH_MESSAGE;
    Crypto_xVIx_xAIx_ECDSAVrfyCtx.ErrorCode = E_OK;
    RetVal = E_OK;
  }
  else
  {
    RetVal = CRYPTO_E_BUSY;
  }
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1262, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ECCNIST_NOT_SET_Finish
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  TS_PARAM_UNUSED(ObjectId);

  if(
       (CRYPTO_XVIX_XAIX_ECDSA_V_INITIALIZED == Crypto_xVIx_xAIx_ECDSAVrfyCtx.State) &&
       (CRYPTO_OPERATIONMODE_UPDATE == Crypto_xVIx_xAIx_ECDSAVrfyCtx.CurrentMode)
     )
  {
    /* Store data into context */
    Crypto_xVIx_xAIx_ECDSAVrfyCtx.Signature       = Job->jobPrimitiveInputOutput.secondaryInputPtr;
    Crypto_xVIx_xAIx_ECDSAVrfyCtx.SignatureLength = Job->jobPrimitiveInputOutput.secondaryInputLength;
    Crypto_xVIx_xAIx_ECDSAVrfyCtx.ResultPtr       = Job->jobPrimitiveInputOutput.verifyPtr;

    /* Set state of the state machine to Update */
    Crypto_xVIx_xAIx_ECDSAVrfyCtx.State     = CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_HASH_MESSAGE;
    Crypto_xVIx_xAIx_ECDSAVrfyCtx.ErrorCode = E_OK;
    RetVal = E_OK;
  }
  else
  {
    RetVal = CRYPTO_E_BUSY;
  }
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1263, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ECCNIST_NOT_SET_Cancel
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  /* This state indicates, that the Primitive is currently waiting for an Update/Finish call */
  if(
      CRYPTO_XVIX_XAIX_ECDSA_V_INITIALIZED ==
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.State
    )
  {
    /* !LINKSTO SWS_Crypto_00122_CORRECTION, 1 */
    Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ECCNIST_NOT_SET_Reset();
    RetVal = E_OK;
  }
  else
  {
    /* Job can not be canceled imideatly */
    Crypto_xVIx_xAIx_ECDSAVrfyCtx.CancelFlag = TRUE;
    /* !LINKSTO SWS_Crypto_00143_CORRECTION_SWS_Crypto_00183, 1 */
    RetVal = CRYPTO_E_JOB_CANCELED;
  }
  TS_PARAM_UNUSED(Job);
  TS_PARAM_UNUSED(ObjectId);
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1264, 1 */
FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ECCNIST_NOT_SET_MainFunction
(
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
)
{
  if(!Crypto_xVIx_xAIx_CheckSkipMain(
                                      Result,
                                      Busy,
                                      Crypto_xVIx_xAIx_ECDSAVrfyCtx.ObjId
                                    )
    )
  {
    Crypto_xVIx_xAIx_ECDSAVrfyCtx.IsBusy = Busy;
    Crypto_xVIx_xAIx_ECDSAVrfyCtx.ResultError = Result;
    if((NULL_PTR != Result) && (NULL_PTR != Busy))
    {
      *Crypto_xVIx_xAIx_ECDSAVrfyCtx.IsBusy = TRUE;
      *Crypto_xVIx_xAIx_ECDSAVrfyCtx.ResultError = E_OK;
    }
    switch (Crypto_xVIx_xAIx_ECDSAVrfyCtx.State)
    {
      case CRYPTO_XVIX_XAIX_ECDSA_V_UPDATE_HASH_MESSAGE:
      {
        Crypto_xVIx_xAIx_ECDSAVrfyUpdateMessage();
        break;
      }

      case CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_HASH_MESSAGE:
      {
        Crypto_xVIx_xAIx_ECDSAVrfyFinishMessage();
        break;
      }

      case CRYPTO_XVIX_XAIX_ECDSA_V_RFROMSIGNATURE:
      {
        Crypto_xVIx_xAIx_ECDSAFinishR();
        break;
      }

      case CRYPTO_XVIX_XAIX_ECDSA_V_SFROMSIGNATURE:
      {
        Crypto_xVIx_xAIx_ECDSAFinishS();
        break;
      }

      case CRYPTO_XVIX_XAIX_ECDSA_V_INVERSION_S:
      {
        /* Calculate the inversion of S = W */
        Crypto_xVIx_xAIx_LNInversion
        (
          Crypto_xVIx_xAIx_ECDSAVrfyCtx.Tmp1Point.XValue,        /* S ^ (-1) */
          &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue[0U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS]),
          &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue[1U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS]),
          &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue[2U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS]),
          &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue[3U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS]),
          Crypto_xVIx_xAIx_EllipticCurveInfo[Crypto_xVIx_xAIx_ECDSAVrfyCtx.ECCtx.CurveIndex].Modulus,
          &Crypto_xVIx_xAIx_ECDSAVrfyCtx.ECCtx.LNCtx
        );
        Crypto_xVIx_xAIx_ECDSAVrfyCtx.State = CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_EQUATION_1;
        break;
      }

      case CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_EQUATION_1:
      {
        Crypto_xVIx_xAIx_ECDSACalculateU1();
        break;
      }

      case CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_X1:
      {
        /* X = U1*P */
        Crypto_xVIx_xAIx_ECPointMultiplication
        (
          &Crypto_xVIx_xAIx_ECDSAVrfyCtx.X,
          &(Crypto_xVIx_xAIx_EllipticCurveInfo[Crypto_xVIx_xAIx_ECDSAVrfyCtx.ECCtx.CurveIndex].P),
          &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue[4U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS]),
          Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue,
          &Crypto_xVIx_xAIx_ECDSAVrfyCtx.ECCtx
        );
        Crypto_xVIx_xAIx_ECDSAVrfyCtx.State = CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_EQUATION_2;
        break;
      }

      case CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_EQUATION_2:
      {
        Crypto_xVIx_xAIx_ECDSACalculateU2();
        break;
      }

      case CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_X2:
      {
        /* Tmp1Point = U2*Q */
        Crypto_xVIx_xAIx_ECPointMultiplication
        (
          &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.Tmp1Point),
          &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.PubKey),
          &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue[4U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS]),
          Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue,
          &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.ECCtx)
        );
        Crypto_xVIx_xAIx_ECDSAVrfyCtx.State = CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_X;
        break;
      }

      case CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_X:
      {
        Crypto_xVIx_xAIx_ECDSACalculateX();
        break;
      }

      case CRYPTO_XVIX_XAIX_ECDSA_V_FINISHANDCOMPARE:
      {
        Crypto_xVIx_xAIx_ECDSAValidateSignature();
        break;
      }

      default:
      {
        Crypto_xVIx_xAIx_ECCNIST_VrfyDefaultWaitState();
        break;
      }

    }
      Crypto_xVIx_xAIx_ECCNIST_VRFY_Callback_Cancel();
  }
}

/* !LINKSTO EB_Crypto_CryptAlgos_1634, 1 */
/* CHECK: NOPARSE */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE)
  Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ECCNIST_NOT_SET_Helper_SyncSingleCall
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                            InputPtr,
  uint32                                                                           InputLength,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                            SignaturePtr,
  uint32                                                                           SignatureLength,
  P2VAR(Crypto_xVIx_xAIx_AsymPublicKeyType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Key,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)            VerifyPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint32 ObjId          = 0U;
  boolean Busy          = FALSE;
  Crypto_xVIx_xAIx_ECDSAVrfyConfigType Cfg = { 0U };

  Crypto_JobType Job =
  {
    0U,                                                /* jobId */
    CRYPTO_JOBSTATE_IDLE,                              /* jobState */
    {
      NULL_PTR,                                        /* inputPtr */
      0U,                                              /* inputLength */
      NULL_PTR,                                        /* secondaryInputPtr */
      0U,                                              /* secondaryInputLength */
      NULL_PTR,                                        /* tertiaryInputPtr */
      0U,                                              /* tertiaryInputLength */
      NULL_PTR,                                        /* outputPtr */
      NULL_PTR,                                        /* outputLengthPtr */
      NULL_PTR,                                        /* secondaryOutputPtr */
      NULL_PTR,                                        /* secondaryOutputLengthPtr */
      NULL_PTR,                                        /* verifyPtr */
      CRYPTO_OPERATIONMODE_SINGLECALL                  /* mode */
    },
    &Crypto_xVIx_xAIx_HelperECDSAVrfyJobPrimitiveInfo, /* jobPrimitiveInfo */
    &Crypto_xVIx_xAIx_HelperECDSAVrfyJobInfo,          /* jobInfo */
    0U                                                 /* cryptoKeyId */
  };

  /* Set the job */
  Job.jobPrimitiveInputOutput.inputPtr             = InputPtr;
  Job.jobPrimitiveInputOutput.inputLength          = InputLength;
  Job.jobPrimitiveInputOutput.secondaryInputPtr    = SignaturePtr;
  Job.jobPrimitiveInputOutput.secondaryInputLength = SignatureLength;
  Job.jobPrimitiveInputOutput.verifyPtr            = VerifyPtr;
  /*only process if the DriverObjectState is not busy.*/
  SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
  if(
      CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE ==
        Crypto_xVIx_xAIx_DriverObjects[ObjId].DriverObjectState
    )
  {
    Crypto_xVIx_xAIx_DriverObjects[ObjId].DriverObjectState =
      CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_BUSY;
    Crypto_xVIx_xAIx_DriverObjects[ObjId].CurrentJob = &Job;
    SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
    /* !LINKSTO Crypto.Dsn.JobTransition.Idle2Active, 1 */
    /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Start, 1 */
    /* !LINKSTO Crypto.Dsn.JobTransition.Start2Start, 1 */
    /* !LINKSTO Crypto.Dsn.JobTransition.Update2Start, 1 */
    Job.jobState = CRYPTO_JOBSTATE_ACTIVE;
    /*perform start tasks */
    Crypto_xVIx_xAIx_ECDSAVrfyCtx.CfgPtr = &Cfg;

    /* !LINKSTO EB_Crypto_CryptAlgos_1611, 1 */
    if (CRYPTO_XVIX_XAIX_ECDSAP256_PUBKEY_LEN != Key->length)
    {
      RetVal = E_NOT_OK;
    }
    else
    {
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.ObjId        = ObjId;
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.Job          = &Job;
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.CurrentMode  = CRYPTO_OPERATIONMODE_START;
      /* !LINKSTO EB_Crypto_CryptAlgos_1607, 1 */
      /* set the public key */
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.PubKey.XValue[0U] = ((uint32)((uint32)Key->length) >> 3U);
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.PubKey.YValue[0U] = ((uint32)((uint32)Key->length) >> 3U);
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.PubKey.ZValue[0U] = 0x01U;
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.PubKey.ZValue[1U] = 0x01U;

      /* The return value is ignored because the parameters are either from the context where they are
         properly defined or local variables which are properly defined before passing them to
         the function.*/
      /* !LINKSTO EB_Crypto_CryptAlgos_1608, 1 */
      (void)Crypto_xVIx_xAIx_LNBEByteArrayToNumber
      (
        Key->data,
        (Key->length >> (uint32)1UL),
        Crypto_xVIx_xAIx_ECDSAVrfyCtx.PubKey.XValue,
        CRYPTO_XVIX_XAIX_ECC_NUM_LEN_WORDS
      );
      /* !LINKSTO EB_Crypto_CryptAlgos_1609, 1 */
      (void)Crypto_xVIx_xAIx_LNBEByteArrayToNumber
      (
        &(Key->data[(CRYPTO_XVIX_XAIX_ECDSAP256_PUBKEY_LEN >> 1U)]),
        (Key->length >> (uint32)1UL),
        Crypto_xVIx_xAIx_ECDSAVrfyCtx.PubKey.YValue,
        CRYPTO_XVIX_XAIX_ECC_NUM_LEN_WORDS
      );

      Crypto_xVIx_xAIx_ECCNIST_VRFY_HASH_Start(&Crypto_xVIx_xAIx_ECDSAVrfyCtx);

      /* ProcessingType is set to CRYPTO_PROCESSING_SYNC*/
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.State = CRYPTO_XVIX_XAIX_ECDSA_V_INITIALIZED;
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.CallbackFlag = TRUE;
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.CallbackResult = E_OK;

      Crypto_xVIx_xAIx_ECDSAVrfyCtx.ErrorCode = E_OK;
      /* !LINKSTO EB_Crypto_CryptAlgos_1613, 1 */
      /* Check the validity of public key */
      RetVal = Crypto_xVIx_xAIx_ECDSAPubKeyValidation(&Crypto_xVIx_xAIx_ECDSAVrfyCtx.PubKey, &Crypto_xVIx_xAIx_ECDSAVrfyCtx.ECCtx);
      if(E_OK == RetVal)
      {
        /* In this context for the Hash Start the MainFunction has only to be called once */
        Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ECCNIST_NOT_SET_MainFunction(&RetVal, &Busy);
      }

      if(E_OK == RetVal)
      {
        RetVal = Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ECCNIST_NOT_SET_Update(ObjId, &Job);
        do
        {
          Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ECCNIST_NOT_SET_MainFunction(&RetVal, &Busy);
        }while((TRUE == Busy) && (E_OK == RetVal));
      }

      if(E_OK == RetVal)
      {
        RetVal = Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ECCNIST_NOT_SET_Finish(ObjId, &Job);
        do
        {
          Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ECCNIST_NOT_SET_MainFunction(&RetVal, &Busy);
        }while((TRUE == Busy) && (E_OK == RetVal));
      }
    }
    /* set DriverObjectState back to idle after processing of the function. Also
     * set the currentJob to NULL_PTR.
     */
    SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
    Crypto_xVIx_xAIx_DriverObjects[ObjId].DriverObjectState =
      CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
    Crypto_xVIx_xAIx_DriverObjects[ObjId].CurrentJob = NULL_PTR;
    Crypto_xVIx_xAIx_DriverObjects[ObjId].SkipPeriodicMainFunction = TRUE;
    SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
  }
  else
  {
    SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
    RetVal = CRYPTO_E_BUSY;
  }
  return RetVal;
}
/* CHECK: PARSE */

#endif /* #if (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Definition of functions with internal linkage]===============================================*/
/*--ECDSAP256Gen----------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ECCNIST_NOT_SET_CallBack
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint8                                                        Result
)
{
  if(
      (NULL_PTR != Crypto_xVIx_xAIx_ECDSAGenCtx.IsBusy) &&
      (NULL_PTR != Crypto_xVIx_xAIx_ECDSAGenCtx.ResultError)
    )
  {
    *Crypto_xVIx_xAIx_ECDSAGenCtx.IsBusy = FALSE;
    *Crypto_xVIx_xAIx_ECDSAGenCtx.ResultError = Result;
  }
  if(CRYPTO_PROCESSING_ASYNC == Job->jobPrimitiveInfo->processingType)
  {
    if(
        (
          CRYPTO_OPERATIONMODE_FINISH ==
          (CRYPTO_OPERATIONMODE_FINISH & Crypto_xVIx_xAIx_ECDSAGenCtx.CurrentMode)
        ) || (E_OK != Result)
      )
    {
      /* !LINKSTO SWS_Crypto_00025, 1 */
      /* !LINKSTO SWS_Crypto_00119, 1 */
      /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Idle, 1 */
      /* !LINKSTO Crypto.Dsn.JobTransition.ActiveError2Idle, 1 */
      Job->jobState = CRYPTO_JOBSTATE_IDLE;
      Crypto_xVIx_xAIx_ECDSAGenCtx.CurrentMode = CRYPTO_XVIX_XAIX_ECDSA_SIGNATURE_DEFAULT_CURRENT_MODE;
      /* !LINKSTO EB_Crypto_01050, 1 */
      SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_ECDSAGenCtx.ObjId].DriverObjectState =
      CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_ECDSAGenCtx.ObjId].CurrentJob = NULL_PTR;
    Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_ECDSAGenCtx.ObjId].SkipPeriodicMainFunction = TRUE;
      SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      CryIf_CallbackNotification(Job, Result);
      Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ECCNIST_NOT_SET_Reset();
    }
    if(
        (
          CRYPTO_OPERATIONMODE_UPDATE ==
          (CRYPTO_OPERATIONMODE_UPDATE & Crypto_xVIx_xAIx_ECDSAGenCtx.CurrentMode)
        ) && (
               ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL) !=
               (
                 ((uint8)Crypto_xVIx_xAIx_ECDSAGenCtx.Job->jobPrimitiveInputOutput.mode) &
                 ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
               )
             )
      )
    {
      CryIf_CallbackNotification(Job, Result);
    }
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ECCNIST_NOT_SET_Reset
(
  void
)
{
  Crypto_xVIx_xAIx_AL_HASH_SHA2_NOT_SET_Reset();
  SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
  TS_MemBZero(
               &Crypto_xVIx_xAIx_ECDSAGenCtx,
               sizeof(Crypto_xVIx_xAIx_ECDSAGenCtx)
             );
  SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSAGenUpdateMessage
(
  void
)
{
  Crypto_xVIx_xAIx_ECDSAGenCtx.CurrentMode = CRYPTO_OPERATIONMODE_UPDATE;
  /* !LINKSTO EB_Crypto_CryptAlgos_1612, 1 */
  Crypto_xVIx_xAIx_ECCNIST_GEN_HASH_Update
  (
    &Crypto_xVIx_xAIx_ECDSAGenCtx,
    Crypto_xVIx_xAIx_ECDSAGenCtx.Data,
    Crypto_xVIx_xAIx_ECDSAGenCtx.DataLength
  );
  if(
      CRYPTO_PROCESSING_SYNC ==
      Crypto_xVIx_xAIx_ECDSAGenCtx.Job->jobPrimitiveInfo->processingType
    )
  {
    Crypto_xVIx_xAIx_ECDSAGenCtx.State = CRYPTO_XVIX_XAIX_ECDSA_G_INITIALIZED;
    Crypto_xVIx_xAIx_ECDSAGenCtx.CallbackFlag = TRUE;
    Crypto_xVIx_xAIx_ECDSAGenCtx.CallbackResult = E_OK;
  }
  else
  {
    Crypto_xVIx_xAIx_ECDSAGenCtx.State = CRYPTO_XVIX_XAIX_ECDSA_G_WAIT_FOR_UPDATE_HASH_MESSAGE;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSAGenFinishMessage
(
  void
)
{
  Crypto_xVIx_xAIx_ECDSAGenCtx.CurrentMode = CRYPTO_OPERATIONMODE_FINISH;
  /* !LINKSTO EB_Crypto_CryptAlgos_1617, 1 */
  if(CRYPTO_XVIX_XAIX_ECDSAP256_SIGN_SIZE > *Crypto_xVIx_xAIx_ECDSAGenCtx.SignatureLength)
  {
    Crypto_xVIx_xAIx_ECDSAGenCtx.CallbackFlag   = TRUE;
    Crypto_xVIx_xAIx_ECDSAGenCtx.CallbackResult = CRYPTO_E_SMALL_BUFFER;
  }
  else
  {
    Crypto_xVIx_xAIx_ECCNIST_GEN_HASH_Finish
    (
      &Crypto_xVIx_xAIx_ECDSAGenCtx,
      Crypto_xVIx_xAIx_ECDSAGenCtx.HashResult,
      &(Crypto_xVIx_xAIx_ECDSAGenCtx.HashResultLength)
    );
    if(
        CRYPTO_PROCESSING_SYNC ==
        Crypto_xVIx_xAIx_ECDSAGenCtx.Job->jobPrimitiveInfo->processingType
      )
    {
      Crypto_xVIx_xAIx_ECDSAGenCtx.State = CRYPTO_XVIX_XAIX_ECDSA_G_RANDOM;
    }
    else
    {
      Crypto_xVIx_xAIx_ECDSAGenCtx.State = CRYPTO_XVIX_XAIX_ECDSA_G_WAIT_FOR_FINISH_HASH_MESSAGE;
    }
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSACalculateK
(
  void
)
{
  /* Temporary buffer to store the random result */
  uint8  K[CRYPTO_XVIX_XAIX_ECC_NUM_LEN_WORDS * CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES];
  uint32 RandomLength =
      Crypto_xVIx_xAIx_LNBitLengthOfNumber(Crypto_xVIx_xAIx_EllipticCurveInfo[Crypto_xVIx_xAIx_ECDSAGenCtx.ECCtx.CurveIndex].Modulus);

  /* Make a byte length from the bit length. */
  RandomLength >>= (uint32)3U;

  /* !LINKSTO EB_Crypto_CryptAlgos_1618, 1 */
  /* Fill K with pseudo-random bytes. */
  if(
      E_OK !=
      Crypto_xVIx_xAIx_AL_RANDOMGENERATE_AES_CTRDRBG_HelperSyncSingleCall
      (
        Crypto_xVIx_xAIx_ECDSAGenCtx.ObjId,
        Crypto_xVIx_xAIx_ECDSAGenCtx.Job,
        K,
        &RandomLength
      )
    )
  {
    CRYPTO_XVIX_XAIX_ECDSA_G_RANDOM_FAILING;
  }
  else
  {
    /* Convert uint8 to LN type independent of endianness */
    Crypto_xVIx_xAIx_LNLEByteArrayToNumber
    (
     K,
     RandomLength,
     Crypto_xVIx_xAIx_ECDSAGenCtx.K,
     CRYPTO_XVIX_XAIX_ECC_NUM_LEN_WORDS
    );
    Crypto_xVIx_xAIx_LNNormalizeNumber(Crypto_xVIx_xAIx_ECDSAGenCtx.K);
    Crypto_xVIx_xAIx_ECDSAGenCtx.State = CRYPTO_XVIX_XAIX_ECDSA_G_VALIDATEK;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSAValidateK
(
  void
)
{
  Crypto_xVIx_xAIx_ReturnType ComparisonZero;
  Crypto_xVIx_xAIx_ReturnType ComparisonModulus;

  /* Tmp1   Offset of the temporary storage */
  const uint32 Tmp1 = (0U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS);

  /* Check the validity of K */
  /* Set Tmp1 to zero */
  Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp1] = 0U;

  /* Compare K with Zero */
  ComparisonZero = Crypto_xVIx_xAIx_LNCompareNumbers
  (
    Crypto_xVIx_xAIx_ECDSAGenCtx.K,
    &(Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp1])
  );

  /* Compare K with Modulus */
  ComparisonModulus = Crypto_xVIx_xAIx_LNCompareNumbers
  (
    Crypto_xVIx_xAIx_ECDSAGenCtx.K,
    Crypto_xVIx_xAIx_EllipticCurveInfo[Crypto_xVIx_xAIx_ECDSAGenCtx.ECCtx.CurveIndex].Modulus
  );

  /* !LINKSTO EB_Crypto_CryptAlgos_1637, 1 */
  /* Check if K is in the range [1, Modulus - 1] */
  if (
      (CRYPTO_XVIX_XAIX_E_LARGER == ComparisonZero)
      &&
      (CRYPTO_XVIX_XAIX_E_SMALLER == ComparisonModulus)
     )
  {
    Crypto_xVIx_xAIx_ECDSAGenCtx.State = CRYPTO_XVIX_XAIX_ECDSA_G_KMULTP;
  }
  else
  if (CRYPTO_XVIX_XAIX_E_EQUAL == ComparisonZero)
  {
    /* !LINKSTO EB_Crypto_CryptAlgos_1638, 1 */
    if (0U == Crypto_xVIx_xAIx_RetryCounter)
    {
      Crypto_xVIx_xAIx_ECDSAGenCtx.State = CRYPTO_XVIX_XAIX_ECDSA_G_RANDOM;
      Crypto_xVIx_xAIx_RetryCounter++;
    }
    /* !LINKSTO EB_Crypto_CryptAlgos_1639, 1 */
    /* If the random number is for the second time zero */
    else
    {
      /* Set Tmp1 to one */
      Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp1] = 1U;
      Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp1 + 1U] = 1U;
      /* subtracts Tmp1 from Modulus */
      Crypto_xVIx_xAIx_LNSubtraction
      (
        Crypto_xVIx_xAIx_ECDSAGenCtx.K,
        Crypto_xVIx_xAIx_EllipticCurveInfo[Crypto_xVIx_xAIx_ECDSAGenCtx.ECCtx.CurveIndex].Modulus,
        &(Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp1])
       );
      Crypto_xVIx_xAIx_ECDSAGenCtx.State = CRYPTO_XVIX_XAIX_ECDSA_G_KMULTP;
    }
  }
  else
  if (CRYPTO_XVIX_XAIX_E_SMALLER != ComparisonModulus)
  {
    Crypto_xVIx_xAIx_LNModulo
    (
      Crypto_xVIx_xAIx_ECDSAGenCtx.K,
      Crypto_xVIx_xAIx_EllipticCurveInfo[Crypto_xVIx_xAIx_ECDSAGenCtx.ECCtx.CurveIndex].Modulus,
      &Crypto_xVIx_xAIx_ECDSAGenCtx.ECCtx.LNCtx
    );

    /* Set Tmp1 to zero */
    Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp1] = 0U;

    /* Check if K is a multiple of modulo */
    if (CRYPTO_XVIX_XAIX_E_EQUAL == Crypto_xVIx_xAIx_LNCompareNumbers
                                    (
                                      Crypto_xVIx_xAIx_ECDSAGenCtx.K,
                                      &(Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp1])
                                    )
    )
    {
      /* !LINKSTO EB_Crypto_CryptAlgos_1640, 1 */
      if (0U == Crypto_xVIx_xAIx_RetryCounter)
      {
        Crypto_xVIx_xAIx_ECDSAGenCtx.State = CRYPTO_XVIX_XAIX_ECDSA_G_RANDOM;
        Crypto_xVIx_xAIx_RetryCounter++;
      }
      /* !LINKSTO EB_Crypto_CryptAlgos_1641, 1 */
      /* If the random number is for the second time a multiple of Modulus */
      else
      {
        /* Set Tmp1 to one */
        Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp1] = 1U;
        Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp1 + 1U] = 1U;
        /* subtracts Tmp1 from Modulus */
        Crypto_xVIx_xAIx_LNSubtraction
        (
          Crypto_xVIx_xAIx_ECDSAGenCtx.K,
          Crypto_xVIx_xAIx_EllipticCurveInfo[Crypto_xVIx_xAIx_ECDSAGenCtx.ECCtx.CurveIndex].Modulus,
          &(Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp1])
         );
        Crypto_xVIx_xAIx_ECDSAGenCtx.State = CRYPTO_XVIX_XAIX_ECDSA_G_KMULTP;
      }
    }
  }
  /* CHECK: NOPARSE */
  else
  {
    /* unreachable code but needed for MISRAC2012 */
  }
  /* CHECK: PARSE */
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSACalculateR1
(
  void
)
{
  /* Reset the counter */
  Crypto_xVIx_xAIx_RetryCounter = 0U;

  /* R = K * base point P, R is in projective coordinate */
  Crypto_xVIx_xAIx_ECPointMultiplication
  (
    &Crypto_xVIx_xAIx_ECDSAGenCtx.KP,        /* R */
    &Crypto_xVIx_xAIx_EllipticCurveInfo[Crypto_xVIx_xAIx_ECDSAGenCtx.ECCtx.CurveIndex].P,
    Crypto_xVIx_xAIx_ECDSAGenCtx.K,
    Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue,
    &Crypto_xVIx_xAIx_ECDSAGenCtx.ECCtx
  );
  Crypto_xVIx_xAIx_ECDSAGenCtx.State = CRYPTO_XVIX_XAIX_ECDSA_G_FINISHR;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSACalculateR2
(
  void
)
{
  /* !LINKSTO EB_Crypto_CryptAlgos_1620, 1 */
  /* Since the calculation returns a point in projective
   * coordinates, a transformation to affine coordinates is needed.
   */
  Crypto_xVIx_xAIx_ECPointConvertProj2Aff
  (
    &Crypto_xVIx_xAIx_ECDSAGenCtx.KP,          /* affine coordinate */
    Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue,
    &Crypto_xVIx_xAIx_ECDSAGenCtx.ECCtx
  );

  /* X.XValue = R */
  Crypto_xVIx_xAIx_LNModulo
  (
    Crypto_xVIx_xAIx_ECDSAGenCtx.KP.XValue,
    Crypto_xVIx_xAIx_EllipticCurveInfo[Crypto_xVIx_xAIx_ECDSAGenCtx.ECCtx.CurveIndex].Modulus,
    &Crypto_xVIx_xAIx_ECDSAGenCtx.ECCtx.LNCtx
  );

  /* !LINKSTO EB_Crypto_CryptAlgos_1621, 1 */
  /* if R = 0 generate a new random number */
  if(0U == Crypto_xVIx_xAIx_ECDSAGenCtx.KP.XValue[0U])
  {
    Crypto_xVIx_xAIx_ECDSAGenCtx.State = CRYPTO_XVIX_XAIX_ECDSA_G_RANDOM;
  }
  else
  {
    Crypto_xVIx_xAIx_ECDSAGenCtx.State = CRYPTO_XVIX_XAIX_ECDSA_G_FINISH_SIG;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSACalculateS
(
  void
)
{
  Crypto_xVIx_xAIx_LNWordType Carry;

  /* Tmp1    Offset of the storage where K^(-1) is stored. */
  const uint32 Tmp1 = (0U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS);
  /* Tmp2    Offset of the temporary storage. */
  const uint32 Tmp2 = (1U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS);
  /* Tmp2    Offset of the temporary storage. */
  const uint32 Tmp3 = (2U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS);
  /* Tmp2    Offset of the storage where S is stored. */
  const uint32 Tmp4 = (3U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS);
  /* Tmp2    Offset of the storage where hash of message is stored. */
  const uint32 Tmp5 = (4U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS);

  /* E: Tmp5Value = hashed message */
  /* The return value is ignored because the parameters are either from the context where they are
     properly defined or local variables which are properly defined before passing them to
     the function.*/
  (void)Crypto_xVIx_xAIx_LNBEByteArrayToNumber
  (
    Crypto_xVIx_xAIx_ECDSAGenCtx.HashResult,
    Crypto_xVIx_xAIx_ECDSAGenCtx.HashResultLength,
    &(Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp5]),
    CRYPTO_XVIX_XAIX_ECC_NUM_LEN_WORDS
  );

  /* Tmp1Value = K^(-1) */
  Crypto_xVIx_xAIx_LNInversion
  (
    &(Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp1]),
    Crypto_xVIx_xAIx_ECDSAGenCtx.K,
    &(Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp2]),
    &(Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp3]),
    &(Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp4]),
    Crypto_xVIx_xAIx_EllipticCurveInfo[Crypto_xVIx_xAIx_ECDSAGenCtx.ECCtx.CurveIndex].Modulus,
    &Crypto_xVIx_xAIx_ECDSAGenCtx.ECCtx.LNCtx
  );

  /* Tmp4Value = D*R */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    &(Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp4]),
    Crypto_xVIx_xAIx_ECDSAGenCtx.KP.XValue,
    Crypto_xVIx_xAIx_ECDSAGenCtx.PriKey,
    &Crypto_xVIx_xAIx_ECDSAGenCtx.ECCtx.LNCtx
  );

  /* Tmp4Value = Tmp4Value % Modulus*/
  Crypto_xVIx_xAIx_LNModulo
  (
    &(Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp4]),
    Crypto_xVIx_xAIx_EllipticCurveInfo[Crypto_xVIx_xAIx_ECDSAGenCtx.ECCtx.CurveIndex].Modulus,
    &Crypto_xVIx_xAIx_ECDSAGenCtx.ECCtx.LNCtx
  );

  /* Tmp4Value = (E + DR) */
  Carry = Crypto_xVIx_xAIx_LNAddition
  (
    &(Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp4]),
    &(Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp4]),
    &(Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp5])
  );
  if(Carry != 0)
  {
    Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp4]++;
    Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[(Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp4] + Tmp4)] = Carry;
  }

  Crypto_xVIx_xAIx_LNModulo
  (
    &(Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp4]),
    Crypto_xVIx_xAIx_EllipticCurveInfo[Crypto_xVIx_xAIx_ECDSAGenCtx.ECCtx.CurveIndex].Modulus,
    &Crypto_xVIx_xAIx_ECDSAGenCtx.ECCtx.LNCtx
  );

  /* Tmp4Value = S */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    &(Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp4]),
    &(Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp4]),
    &(Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp1]),
    &Crypto_xVIx_xAIx_ECDSAGenCtx.ECCtx.LNCtx
  );

  Crypto_xVIx_xAIx_LNModulo
  (
    &(Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp4]),
    Crypto_xVIx_xAIx_EllipticCurveInfo[Crypto_xVIx_xAIx_ECDSAGenCtx.ECCtx.CurveIndex].Modulus,
    &Crypto_xVIx_xAIx_ECDSAGenCtx.ECCtx.LNCtx
  );

  /* !LINKSTO EB_Crypto_CryptAlgos_1622, 1 */
  /* if S = 0 generate a new random number */
  if(0U == Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp4])
  {
    Crypto_xVIx_xAIx_ECDSAGenCtx.State = CRYPTO_XVIX_XAIX_ECDSA_G_RANDOM;
  }
  else
  {
    Crypto_xVIx_xAIx_ECDSAGenCtx.State = CRYPTO_XVIX_XAIX_ECDSA_G_EXPORT_SIG;
  }
}

  /* !LINKSTO EB_Crypto_CryptAlgos_1619, 1 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSAFinishSignature
(
  void
)
{
  uint32 i;
  uint32 Temp = (Crypto_xVIx_xAIx_ECDSAGenCtx.KP.XValue[0U] << 2U);
  const uint32 Tmp4 = 3U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS;

  if(Crypto_xVIx_xAIx_ECDSAGenCtx.KP.XValue[0U] < Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp4])
  {
    for
    (
      i  = Crypto_xVIx_xAIx_ECDSAGenCtx.KP.XValue[0U] + (Crypto_xVIx_xAIx_LNWordType)1UL;
      i <= Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp4];
      i++
    )
    {
      Crypto_xVIx_xAIx_ECDSAGenCtx.KP.XValue[i] = 0U;
    }

    Crypto_xVIx_xAIx_ECDSAGenCtx.KP.XValue[0U] = Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp4];
  }

  if(Crypto_xVIx_xAIx_ECDSAGenCtx.KP.XValue[0U] > Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp4])
  {
    for
    (
      i  = Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp4] + (Crypto_xVIx_xAIx_LNWordType)1UL;
      i <= Crypto_xVIx_xAIx_ECDSAGenCtx.KP.XValue[0U];
      i++
    )
    {
      Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[(Tmp4 + i)] = 0U;
    }

    Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp4] = Crypto_xVIx_xAIx_ECDSAGenCtx.KP.XValue[0U];
  }

  /* The return value is ignored because the parameters are either from the context where they are
     properly defined or local variables which are properly defined before passing them to
     the function.*/
  /* Store R in the Signature */
  (void)Crypto_xVIx_xAIx_LNNumberToBEByteArray
  (
    Crypto_xVIx_xAIx_ECDSAGenCtx.KP.XValue,
    Crypto_xVIx_xAIx_ECDSAGenCtx.Signature,
    &Temp
  );

  /* Store S in the Signature */
  (void)Crypto_xVIx_xAIx_LNNumberToBEByteArray
  (
    &Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp4],
    &Crypto_xVIx_xAIx_ECDSAGenCtx.Signature[Temp],
    &Temp
  );

  /* !LINKSTO EB_Crypto_CryptAlgos_1623, 1 */
  /* !LINKSTO EB_Crypto_01030, 1 */
  *Crypto_xVIx_xAIx_ECDSAGenCtx.SignatureLength =
      (Crypto_xVIx_xAIx_ECDSAGenCtx.TmpValue[Tmp4] << 2U) + Temp;

  Crypto_xVIx_xAIx_ECDSAGenCtx.ErrorCode      = E_OK;
  Crypto_xVIx_xAIx_ECDSAGenCtx.CallbackFlag   = TRUE;
  Crypto_xVIx_xAIx_ECDSAGenCtx.State          = CRYPTO_XVIX_XAIX_ECDSA_G_IDLE;
  Crypto_xVIx_xAIx_ECDSAGenCtx.CallbackResult = Crypto_xVIx_xAIx_ECDSAGenCtx.ErrorCode;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECCNIST_GEN_HASH_Start
(
  P2VAR(Crypto_xVIx_xAIx_ECDSAGenCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Ctx
)
{
  Crypto_xVIx_xAIx_SHA2256HelperStart(
                                      Ctx->ObjId,
                                      &Crypto_xVIx_xAIx_ECCNIST_GEN_HASH_Callback,
                                      Ctx->Job,
                                      Ctx->Job->jobPrimitiveInfo->processingType
                                     );
  /* Set selected hash length in byte */
  Ctx->HashResultLength = CRYPTO_XVIX_XAIX_ECDSAP256_HASH_MAX_LEN;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECCNIST_GEN_HASH_Update
(
  P2CONST(Crypto_xVIx_xAIx_ECDSAGenCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Ctx,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                            DataPtr,
  uint32                                                                           DataLength
)
{
/* we are in the correct state when this is called so it can not fail */
(void)Crypto_xVIx_xAIx_SHA2256HelperUpdate(
                                            DataPtr,
                                            DataLength,
                                            Ctx->Job->jobPrimitiveInfo->processingType
                                          );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECCNIST_GEN_HASH_Finish
(
  P2CONST(Crypto_xVIx_xAIx_ECDSAGenCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Ctx,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                              ResPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                             ResLenPtr
)
{
/* we are in the correct state when this is called so it can not fail */
(void)Crypto_xVIx_xAIx_SHA2256HelperFinish(
                                            ResPtr,
                                            ResLenPtr,
                                            FALSE,
                                            Ctx->Job->jobPrimitiveInfo->processingType
                                          );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECCNIST_GenDefaultWaitState
(
  void
)
{
  switch(Crypto_xVIx_xAIx_ECDSAGenCtx.State)
  {
    case CRYPTO_XVIX_XAIX_ECDSA_G_INIT_WAIT:
    case CRYPTO_XVIX_XAIX_ECDSA_G_WAIT_FOR_UPDATE_HASH_MESSAGE:
    case CRYPTO_XVIX_XAIX_ECDSA_G_WAIT_FOR_FINISH_HASH_MESSAGE:
    {
      Crypto_xVIx_xAIx_SHA2256HelperMainFunction();
      break;
    }
    case CRYPTO_XVIX_XAIX_ECDSA_G_IDLE:
    case CRYPTO_XVIX_XAIX_ECDSA_G_INITIALIZED:
    {
      break;
    }
    /* CHECK: NOPARSE */
    /* Defensive Programming - unreachable code. This default case is required by MISRA-C:2012 */
    default:
    {
      /* default case required by MISRA-C */
      break;
    }
    /* CHECK: PARSE */
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECCNIST_GEN_HASH_Callback
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  Std_ReturnType                                               Result
)
{
  TS_PARAM_UNUSED(Job);
  switch(Crypto_xVIx_xAIx_ECDSAGenCtx.State)
  {
    case CRYPTO_XVIX_XAIX_ECDSA_G_INIT_WAIT:
    case CRYPTO_XVIX_XAIX_ECDSA_G_WAIT_FOR_UPDATE_HASH_MESSAGE:
    {
      Crypto_xVIx_xAIx_ECDSAGenCtx.State = CRYPTO_XVIX_XAIX_ECDSA_G_INITIALIZED;
      Crypto_xVIx_xAIx_ECDSAGenCtx.CallbackFlag = TRUE;
      Crypto_xVIx_xAIx_ECDSAGenCtx.CallbackResult = Result;
      break;
    }
    case CRYPTO_XVIX_XAIX_ECDSA_G_WAIT_FOR_FINISH_HASH_MESSAGE:
    {
      Crypto_xVIx_xAIx_ECDSAGenCtx.State = CRYPTO_XVIX_XAIX_ECDSA_G_RANDOM;
      break;
    }
    /* CHECK: NOPARSE */
    /*All the valid (wait) states have been addressed in the cases above.*/
    default:
    {
      /* default case required by MISRA-C */
      break;
    }
    /* CHECK: PARSE */
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECCNIST_GEN_Callback_Cancel
(
  void
)
{
  /* !LINKSTO SWS_Crypto_00143_CORRECTION_SWS_Crypto_00181, 1, SWS_Crypto_00144_CORRECTION, 1 */
  if(TRUE == Crypto_xVIx_xAIx_ECDSAGenCtx.CancelFlag)
  {
    if (
         (NULL_PTR != Crypto_xVIx_xAIx_ECDSAGenCtx.ResultError)
         && (NULL_PTR != Crypto_xVIx_xAIx_ECDSAGenCtx.IsBusy)
       )
    {
      *Crypto_xVIx_xAIx_ECDSAGenCtx.IsBusy = FALSE;
      *Crypto_xVIx_xAIx_ECDSAGenCtx.ResultError = CRYPTO_E_JOB_CANCELED;
      /* processingType is synchronous */
      /* !LINKSTO Crypto.Dsn.JobTransition.ActiveCancel2Idle, 1 */
      Crypto_xVIx_xAIx_ECDSAGenCtx.Job->jobState = CRYPTO_JOBSTATE_IDLE;
      SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_ECDSAGenCtx.ObjId].DriverObjectState =
        CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_ECDSAGenCtx.ObjId].CurrentJob =
        NULL_PTR;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_ECDSAGenCtx.ObjId].SkipPeriodicMainFunction = TRUE;
      SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
    }
    Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ECCNIST_NOT_SET_CallBack
    (
      Crypto_xVIx_xAIx_ECDSAGenCtx.Job,
      CRYPTO_E_JOB_CANCELED
    );
    Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ECCNIST_NOT_SET_Reset();
  }
  else if(TRUE == Crypto_xVIx_xAIx_ECDSAGenCtx.CallbackFlag)
  {
    if (
         (NULL_PTR != Crypto_xVIx_xAIx_ECDSAGenCtx.ResultError)
         && (NULL_PTR != Crypto_xVIx_xAIx_ECDSAGenCtx.IsBusy)
       )
    {
      *Crypto_xVIx_xAIx_ECDSAGenCtx.IsBusy = FALSE;
      *Crypto_xVIx_xAIx_ECDSAGenCtx.ResultError =
      Crypto_xVIx_xAIx_ECDSAGenCtx.CallbackResult;
    }
    if(
        (
          CRYPTO_PROCESSING_ASYNC ==
          Crypto_xVIx_xAIx_ECDSAGenCtx.Job->jobPrimitiveInfo->processingType
        ) &&
        (
          (
            (
              ((uint8)Crypto_xVIx_xAIx_ECDSAGenCtx.Job->jobPrimitiveInputOutput.mode) &
                ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
            ) == ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
          ) ||
          (
            (
              ((uint8)Crypto_xVIx_xAIx_ECDSAGenCtx.Job->jobPrimitiveInputOutput.mode) &
              ((uint8) CRYPTO_OPERATIONMODE_STREAMSTART)
            ) == ( (uint8) CRYPTO_OPERATIONMODE_STREAMSTART)
          )
        ) && (CRYPTO_OPERATIONMODE_START == Crypto_xVIx_xAIx_ECDSAGenCtx.CurrentMode)
      )
    {
      (void)Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ECCNIST_NOT_SET_Update
            (
              Crypto_xVIx_xAIx_ECDSAGenCtx.ObjId,
              Crypto_xVIx_xAIx_ECDSAGenCtx.Job
            );
    }
    else if
    (
      (
        CRYPTO_PROCESSING_ASYNC ==
        Crypto_xVIx_xAIx_ECDSAGenCtx.Job->jobPrimitiveInfo->processingType
      ) &&
      (
        (
          (
            ((uint8)Crypto_xVIx_xAIx_ECDSAGenCtx.Job->jobPrimitiveInputOutput.mode) &
            ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
          ) == ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
        )
      ) && (CRYPTO_OPERATIONMODE_UPDATE == Crypto_xVIx_xAIx_ECDSAGenCtx.CurrentMode)
    )
    {
      (void)Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ECCNIST_NOT_SET_Finish
            (
              Crypto_xVIx_xAIx_ECDSAGenCtx.ObjId,
              Crypto_xVIx_xAIx_ECDSAGenCtx.Job
            );
    }
    else
    {
      /* Nothing to do.  */
    }

    Crypto_xVIx_xAIx_AL_SIGNATUREGENERATE_ECCNIST_NOT_SET_CallBack
    (
      Crypto_xVIx_xAIx_ECDSAGenCtx.Job,
      Crypto_xVIx_xAIx_ECDSAGenCtx.CallbackResult
    );
    Crypto_xVIx_xAIx_ECDSAGenCtx.CallbackFlag = FALSE;
  }

  else
  {
    /* Nothing to do.  */
  }
}

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON) */

/*--ECDSAP256Vrfy---------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON)

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ECCNIST_NOT_SET_CallBack
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint8                                                        Result
)
{
  if(
      (NULL_PTR != Crypto_xVIx_xAIx_ECDSAVrfyCtx.IsBusy) &&
      (NULL_PTR != Crypto_xVIx_xAIx_ECDSAVrfyCtx.ResultError)
    )
  {
    *Crypto_xVIx_xAIx_ECDSAVrfyCtx.IsBusy = FALSE;
    *Crypto_xVIx_xAIx_ECDSAVrfyCtx.ResultError = Result;
  }
  if(CRYPTO_PROCESSING_ASYNC == Job->jobPrimitiveInfo->processingType)
  {
    if(
        (
          CRYPTO_OPERATIONMODE_FINISH ==
          (CRYPTO_OPERATIONMODE_FINISH & Crypto_xVIx_xAIx_ECDSAVrfyCtx.CurrentMode)
        ) || (E_OK != Result)
      )
    {
      /* !LINKSTO SWS_Crypto_00025, 1 */
      /* !LINKSTO SWS_Crypto_00119, 1 */
      /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Idle, 1 */
      /* !LINKSTO Crypto.Dsn.JobTransition.ActiveError2Idle, 1 */
      Job->jobState = CRYPTO_JOBSTATE_IDLE;
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.CurrentMode = CRYPTO_XVIX_XAIX_ECDSA_SIGNATURE_DEFAULT_CURRENT_MODE;
      /* !LINKSTO EB_Crypto_01050, 1 */
      SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_ECDSAVrfyCtx.ObjId].DriverObjectState =
      CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_ECDSAVrfyCtx.ObjId].CurrentJob = NULL_PTR;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_ECDSAVrfyCtx.ObjId].SkipPeriodicMainFunction = TRUE;
      SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      CryIf_CallbackNotification(Job, Result);
      Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ECCNIST_NOT_SET_Reset();
    }
    if(
        (
          CRYPTO_OPERATIONMODE_UPDATE ==
          (CRYPTO_OPERATIONMODE_UPDATE & Crypto_xVIx_xAIx_ECDSAVrfyCtx.CurrentMode)
        ) && (
               ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL) !=
               (
                 ((uint8)Crypto_xVIx_xAIx_ECDSAVrfyCtx.Job->jobPrimitiveInputOutput.mode) &
                 ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
               )
             )
      )
    {
      CryIf_CallbackNotification(Job, Result);
    }
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ECCNIST_NOT_SET_Reset
(
  void
)
{
  Crypto_xVIx_xAIx_AL_HASH_SHA2_NOT_SET_Reset();
  SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
  TS_MemBZero(
               &Crypto_xVIx_xAIx_ECDSAVrfyCtx,
               sizeof(Crypto_xVIx_xAIx_ECDSAVrfyCtx)
             );
  SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECCNIST_VRFY_HASH_Start
(
  P2VAR(Crypto_xVIx_xAIx_ECDSAVrfyCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Ctx
)
{
  Crypto_xVIx_xAIx_SHA2256HelperStart(
                                       Ctx->ObjId,
                                       &Crypto_xVIx_xAIx_ECCNIST_VRFY_HASH_Callback,
                                       Ctx->Job,
                                       Ctx->Job->jobPrimitiveInfo->processingType
                                     );
  /* set selected hash length in byte */
  Ctx->HashResultLength = CRYPTO_XVIX_XAIX_ECDSAP256_HASH_MAX_LEN;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECCNIST_VRFY_HASH_Update
(
  P2CONST(Crypto_xVIx_xAIx_ECDSAVrfyCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Ctx,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                             DataPtr,
  uint32                                                                            DataLength
)
{
/* we are in the correct state when this is called so it can not fail */
(void)Crypto_xVIx_xAIx_SHA2256HelperUpdate(
                                            DataPtr,
                                            DataLength,
                                            Ctx->Job->jobPrimitiveInfo->processingType
                                          );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECCNIST_VRFY_HASH_Finish
(
  P2CONST(Crypto_xVIx_xAIx_ECDSAVrfyCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Ctx,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                               ResPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                              ResLenPtr
)
{
/* we are in the correct state when this is called so it can not fail */
(void)Crypto_xVIx_xAIx_SHA2256HelperFinish(
                                            ResPtr,
                                            ResLenPtr,
                                            FALSE,
                                            Ctx->Job->jobPrimitiveInfo->processingType
                                          );
}

static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSAPubKeyValidation
(
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Point,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)     CtxPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  Crypto_xVIx_xAIx_ReturnType Comparison;

  /* !LINKSTO EB_Crypto_CryptAlgos_1614, 1 */
  /* Compare XValue of public key with prime. */
  Comparison = Crypto_xVIx_xAIx_LNCompareNumbers
                  (
                    Point->XValue,
                    Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime
                  );

  /* Check if XValue is in the range [0, prime - 1]
   * We use unsigned initeger, therefore it is no need to compare the Value with Zero.
   */
  if (CRYPTO_XVIX_XAIX_E_SMALLER != Comparison)
  {
    CRYPTO_XVIX_XAIX_ECDSA_V_HANDLE_FAILING_VALIDATE_PUIBKEY;
  }
  else
  {
    /* !LINKSTO EB_Crypto_CryptAlgos_1615, 1 */
    /* Compare YValue of public key with prime. */
    Comparison = Crypto_xVIx_xAIx_LNCompareNumbers
                    (
                      Point->YValue,
                      Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime
                    );

    /* Check if YValue is in the range [0, prime - 1]
     * We use unsigned initeger, therefore it is no need to compare the Value with Zero.
     */
    if (CRYPTO_XVIX_XAIX_E_SMALLER != Comparison)
    {
      CRYPTO_XVIX_XAIX_ECDSA_V_HANDLE_FAILING_VALIDATE_PUIBKEY;
    }
    else
    {
      RetVal = Crypto_xVIx_xAIx_ECDSAPubKeyCheckEquation(Point, CtxPtr);
    }
  }

  return RetVal;
}

static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSAPubKeyCheckEquation
(
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Point,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)     CtxPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  Crypto_xVIx_xAIx_ReturnType Comparison;
  /* Tmp3   Offset of the temporary storage */
  const uint32 Tmp1 = (0U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS);
  /* Tmp3   Offset of the temporary storage */
  const uint32 Tmp2 = (1U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS);
  /* !LINKSTO EB_Crypto_CryptAlgos_1616, 1 */
  /* Check if the public key lies on the Elliptic Curve E: y^2 = x^3 + A*x + B */
  /* Tmp1: X^2 = X * X */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue[Tmp1]),
    Point->XValue,
    Point->XValue,
    &CtxPtr->LNCtx
  );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(&(Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue[Tmp1]), CtxPtr);

  /* Tmp1: X^3 = Tmp1 * X */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue[Tmp1]),
    &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue[Tmp1]),
    Point->XValue,
    &CtxPtr->LNCtx
  );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(&(Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue[Tmp1]), CtxPtr);

  /* Tmp2 = A * X */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue[Tmp2]),
    Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Coeff1,
    Point->XValue,
    &CtxPtr->LNCtx
  );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(&(Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue[Tmp2]), CtxPtr);

  /* Tmp1 = Tmp1 + Tmp2: X^3 + A*x */
  Crypto_xVIx_xAIx_LNAdditionModular
  (
    &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue[Tmp1]),
    &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue[Tmp1]),
    &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue[Tmp2]),
    Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime
  );

  /* Tmp1 = Tmp1 + B: X^3 + A*x + B*/
  Crypto_xVIx_xAIx_LNAdditionModular
  (
    &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue[Tmp1]),
    &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue[Tmp1]),
    Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Coeff2,
    Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime
  );

  /* Tmp2: Y^2 */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue[Tmp2]),
    Point->YValue,
    Point->YValue,
    &CtxPtr->LNCtx
  );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(&(Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue[Tmp2]), CtxPtr);

  /* Compare Tmp2 with Tmp1 */
  Comparison = Crypto_xVIx_xAIx_LNCompareNumbers
  (
    &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue[Tmp1]),
    &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue[Tmp2])
  );
  if (CRYPTO_XVIX_XAIX_E_EQUAL != Comparison)
  {
    CRYPTO_XVIX_XAIX_ECDSA_V_HANDLE_FAILING_VALIDATE_PUIBKEY;
  }
  else
  {
    RetVal = E_OK;
  }

  return RetVal;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSAVrfyUpdateMessage
(
  void
)
{
  Crypto_xVIx_xAIx_ECDSAVrfyCtx.CurrentMode = CRYPTO_OPERATIONMODE_UPDATE;
  /* !LINKSTO EB_Crypto_CryptAlgos_1624, 1 */
  Crypto_xVIx_xAIx_ECCNIST_VRFY_HASH_Update
  (
    &Crypto_xVIx_xAIx_ECDSAVrfyCtx,
    Crypto_xVIx_xAIx_ECDSAVrfyCtx.Data,
    Crypto_xVIx_xAIx_ECDSAVrfyCtx.DataLength
  );
  if(
      CRYPTO_PROCESSING_SYNC ==
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.Job->jobPrimitiveInfo->processingType
    )
  {
    Crypto_xVIx_xAIx_ECDSAVrfyCtx.State = CRYPTO_XVIX_XAIX_ECDSA_V_INITIALIZED;
    Crypto_xVIx_xAIx_ECDSAVrfyCtx.CallbackFlag = TRUE;
    Crypto_xVIx_xAIx_ECDSAVrfyCtx.CallbackResult = E_OK;
  }
  else
  {
    Crypto_xVIx_xAIx_ECDSAVrfyCtx.State = CRYPTO_XVIX_XAIX_ECDSA_V_WAIT_FOR_UPDATE_HASH_MESSAGE;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSAVrfyFinishMessage
(
  void
)
{
  Crypto_xVIx_xAIx_ECDSAVrfyCtx.CurrentMode = CRYPTO_OPERATIONMODE_FINISH;
  /* !LINKSTO EB_Crypto_CryptAlgos_1625, 1 */
  if(CRYPTO_XVIX_XAIX_ECDSAP256_SIGN_SIZE != Crypto_xVIx_xAIx_ECDSAVrfyCtx.SignatureLength)
  {
    *Crypto_xVIx_xAIx_ECDSAVrfyCtx.ResultPtr     = CRYPTO_E_VER_NOT_OK;
    Crypto_xVIx_xAIx_ECDSAVrfyCtx.CallbackFlag   = TRUE;
    Crypto_xVIx_xAIx_ECDSAVrfyCtx.CallbackResult = E_OK;
  }
  else
  {
    Crypto_xVIx_xAIx_ECCNIST_VRFY_HASH_Finish
    (
      &Crypto_xVIx_xAIx_ECDSAVrfyCtx,
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.HashResult,
      &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.HashResultLength)
    );
    if(
      CRYPTO_PROCESSING_SYNC ==
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.Job->jobPrimitiveInfo->processingType
      )
    {
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.State = CRYPTO_XVIX_XAIX_ECDSA_V_RFROMSIGNATURE;
    }
    else
    {
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.State = CRYPTO_XVIX_XAIX_ECDSA_V_WAIT_FOR_FINISH_HASH_MESSAGE;
    }
  }

}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSAFinishR
(
  void
)
{
  Crypto_xVIx_xAIx_ReturnType ComparisonZero;
  Crypto_xVIx_xAIx_ReturnType ComparisonModulus;

  /* Tmp3   Offset of the temporary storage */
  const uint32 Tmp3 = (2U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS);
  /* !LINKSTO EB_Crypto_CryptAlgos_1626, 1 */
  /* R = first 32 bytes of Signature */
  /* The return value is ignored because the parameters are either from the context where they are
   * properly defined or local variables which are properly defined before passing them to
   * the function.
   */
  (void)Crypto_xVIx_xAIx_LNBEByteArrayToNumber
  (
    Crypto_xVIx_xAIx_ECDSAVrfyCtx.Signature,
    (Crypto_xVIx_xAIx_ECDSAVrfyCtx.SignatureLength >> (uint32)1UL),
    Crypto_xVIx_xAIx_ECDSAVrfyCtx.R,
    CRYPTO_XVIX_XAIX_ECC_NUM_LEN_WORDS
  );

  /* Check the validity of R */
  /* Set Tmp3 to zero */
  Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue[Tmp3] = 0U;

  /* Compare R with Zero */
  ComparisonZero = Crypto_xVIx_xAIx_LNCompareNumbers
                  (
                    Crypto_xVIx_xAIx_ECDSAVrfyCtx.R,
                    &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue[Tmp3])
                  );
  /* Compare R with Modulus */
  ComparisonModulus = Crypto_xVIx_xAIx_LNCompareNumbers
                  (
                    Crypto_xVIx_xAIx_ECDSAVrfyCtx.R,
                    Crypto_xVIx_xAIx_EllipticCurveInfo[Crypto_xVIx_xAIx_ECDSAVrfyCtx.ECCtx.CurveIndex].Modulus
                  );

  /* !LINKSTO EB_Crypto_CryptAlgos_1627, 1 */
  /* Check if R is in the range [1, Modulus - 1] */
  if (
      (CRYPTO_XVIX_XAIX_E_LARGER == ComparisonZero)
      &&
      (CRYPTO_XVIX_XAIX_E_SMALLER == ComparisonModulus)
     )
  {
    Crypto_xVIx_xAIx_ECDSAVrfyCtx.State = CRYPTO_XVIX_XAIX_ECDSA_V_SFROMSIGNATURE;
  }
  else
  {
    CRYPTO_XVIX_XAIX_ECDSA_V_HANDLE_FAILING_VALIDATE_SIG;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSAFinishS
(
  void
)
{
  Crypto_xVIx_xAIx_ReturnType ComparisonZero;
  Crypto_xVIx_xAIx_ReturnType ComparisonModulus;

  /* !LINKSTO EB_Crypto_CryptAlgos_1628, 1 */
  /* Tmp1: S = last 32 bytes of Signature */
  /* The return value is ignored because the parameters are either from the context where they are
   properly defined or local variables which are properly defined before passing them to
   the function.*/
  (void)Crypto_xVIx_xAIx_LNBEByteArrayToNumber
  (
    &Crypto_xVIx_xAIx_ECDSAVrfyCtx.Signature
          [
            (Crypto_xVIx_xAIx_ECDSAVrfyCtx.SignatureLength >> (uint32)1UL)
          ],
    (Crypto_xVIx_xAIx_ECDSAVrfyCtx.SignatureLength >> (uint32)1UL),
    &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue)
          [
            (0U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS)
          ],
    CRYPTO_XVIX_XAIX_ECC_NUM_LEN_WORDS
  );

  /* Check the validity of S */
  /* Compare S with Zero */
  ComparisonZero = Crypto_xVIx_xAIx_LNCompareNumbers
                (
                  &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue[0U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS]),
                  &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue[2U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS])
                );
  /* Compare S with Modulus */
  ComparisonModulus = Crypto_xVIx_xAIx_LNCompareNumbers
                (
                  &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue[0U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS]),
                  Crypto_xVIx_xAIx_EllipticCurveInfo[Crypto_xVIx_xAIx_ECDSAVrfyCtx.ECCtx.CurveIndex].Modulus
                );

  /* !LINKSTO EB_Crypto_CryptAlgos_1629, 1 */
  /* Check if S is in the range [1, Modulus - 1] */
  if (
      (CRYPTO_XVIX_XAIX_E_LARGER == ComparisonZero)
      &&
      (CRYPTO_XVIX_XAIX_E_SMALLER == ComparisonModulus)
     )
  {
    Crypto_xVIx_xAIx_ECDSAVrfyCtx.State = CRYPTO_XVIX_XAIX_ECDSA_V_INVERSION_S;
  }
  else
  {
    CRYPTO_XVIX_XAIX_ECDSA_V_HANDLE_FAILING_VALIDATE_SIG;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSACalculateU1
(
  void
)
{
  /* Tmp2Value = E = Hash(data) */
  /* The return value is ignored because the parameters are either from the context where they are
   properly defined or local variables which are properly defined before passing them to
   the function.*/
  (void)Crypto_xVIx_xAIx_LNBEByteArrayToNumber
  (
    Crypto_xVIx_xAIx_ECDSAVrfyCtx.HashResult,
    Crypto_xVIx_xAIx_ECDSAVrfyCtx.HashResultLength,
    &Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue
    [
      (1U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS)
    ],
    CRYPTO_XVIX_XAIX_ECC_NUM_LEN_WORDS
  );

  /* Tmp5Value = E*W = U1 */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue[4U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS]),
    &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue[1U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS]),
    Crypto_xVIx_xAIx_ECDSAVrfyCtx.Tmp1Point.XValue,
    &Crypto_xVIx_xAIx_ECDSAVrfyCtx.ECCtx.LNCtx
  );

  Crypto_xVIx_xAIx_LNModulo
  (
    &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue[4U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS]),
    Crypto_xVIx_xAIx_EllipticCurveInfo[Crypto_xVIx_xAIx_ECDSAVrfyCtx.ECCtx.CurveIndex].Modulus,
    &Crypto_xVIx_xAIx_ECDSAVrfyCtx.ECCtx.LNCtx
  );

  Crypto_xVIx_xAIx_ECDSAVrfyCtx.State = CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_X1;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSACalculateU2
(
  void
)
{
  /* Tmp5Value = R * W = U2 */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue[4U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS]),
    Crypto_xVIx_xAIx_ECDSAVrfyCtx.R,
    Crypto_xVIx_xAIx_ECDSAVrfyCtx.Tmp1Point.XValue,
    &Crypto_xVIx_xAIx_ECDSAVrfyCtx.ECCtx.LNCtx
  );

  /* Tmp5Value = (R * W) mod Modulus */
  Crypto_xVIx_xAIx_LNModulo
  (
    &(Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue[4U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS]),
    Crypto_xVIx_xAIx_EllipticCurveInfo[Crypto_xVIx_xAIx_ECDSAVrfyCtx.ECCtx.CurveIndex].Modulus,
    &Crypto_xVIx_xAIx_ECDSAVrfyCtx.ECCtx.LNCtx
  );

  Crypto_xVIx_xAIx_ECDSAVrfyCtx.State = CRYPTO_XVIX_XAIX_ECDSA_V_FINISH_X2;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSACalculateX
(
  void
)
{
  /* X = U1*P + U2*Q */
  Crypto_xVIx_xAIx_ECPointAddition
  (
     &Crypto_xVIx_xAIx_ECDSAVrfyCtx.X,
     &Crypto_xVIx_xAIx_ECDSAVrfyCtx.X,
     &Crypto_xVIx_xAIx_ECDSAVrfyCtx.Tmp1Point,
     Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue,
     &Crypto_xVIx_xAIx_ECDSAVrfyCtx.ECCtx
  );
  /* !LINKSTO EB_Crypto_CryptAlgos_1630, 1 */
  if(
      TRUE ==
         Crypto_xVIx_xAIx_ECProjectivePointOfInfinity(&Crypto_xVIx_xAIx_ECDSAVrfyCtx.X)
    )
  {
    CRYPTO_XVIX_XAIX_ECDSA_V_HANDLE_FAILING_VALIDATE_X;
  }
  else
  {
    /* !LINKSTO EB_Crypto_CryptAlgos_1631, 1 */
    /* Since the calculation returns a point in standard projective
     * coordinates, a transformation to affine coordinates is needed.
     */
    Crypto_xVIx_xAIx_ECPointConvertProj2Aff
    (
      &Crypto_xVIx_xAIx_ECDSAVrfyCtx.X,
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.TmpValue,
      &Crypto_xVIx_xAIx_ECDSAVrfyCtx.ECCtx
    );

    Crypto_xVIx_xAIx_ECDSAVrfyCtx.State = CRYPTO_XVIX_XAIX_ECDSA_V_FINISHANDCOMPARE;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECDSAValidateSignature
(
  void
)
{
  uint32                      i;
  Crypto_xVIx_xAIx_LNWordType CompareResult = 0U;
  Crypto_VerifyResultType     Result        = CRYPTO_E_VER_OK;

  /* X.XValue = V */
  Crypto_xVIx_xAIx_LNModulo
  (
    Crypto_xVIx_xAIx_ECDSAVrfyCtx.X.XValue,
    Crypto_xVIx_xAIx_EllipticCurveInfo[Crypto_xVIx_xAIx_ECDSAVrfyCtx.ECCtx.CurveIndex].Modulus,
    &Crypto_xVIx_xAIx_ECDSAVrfyCtx.ECCtx.LNCtx
  );
  /* !LINKSTO EB_Crypto_CryptAlgos_1632, 1 */
  if(Crypto_xVIx_xAIx_ECDSAVrfyCtx.X.XValue[0U] != Crypto_xVIx_xAIx_ECDSAVrfyCtx.R[0U])
   {
      Result = CRYPTO_E_VER_NOT_OK;
   }
   else
   {
      for(i = 1U; i <= Crypto_xVIx_xAIx_ECDSAVrfyCtx.R[0U]; i++)
      {
        CompareResult |= Crypto_xVIx_xAIx_ECDSAVrfyCtx.X.XValue[i] ^Crypto_xVIx_xAIx_ECDSAVrfyCtx.R[i];
      }
      if(0U != CompareResult)
      {
        Result = CRYPTO_E_VER_NOT_OK;
      }
   }

  *Crypto_xVIx_xAIx_ECDSAVrfyCtx.ResultPtr = Result;

  Crypto_xVIx_xAIx_ECDSAVrfyCtx.State     = CRYPTO_XVIX_XAIX_ECDSA_V_IDLE;
  Crypto_xVIx_xAIx_ECDSAVrfyCtx.ErrorCode = E_OK;

  Crypto_xVIx_xAIx_ECDSAVrfyCtx.CallbackFlag   = TRUE;
  Crypto_xVIx_xAIx_ECDSAVrfyCtx.CallbackResult = Crypto_xVIx_xAIx_ECDSAVrfyCtx.ErrorCode;

  if (
      (NULL_PTR != Crypto_xVIx_xAIx_ECDSAVrfyCtx.ResultError) &&
      (NULL_PTR != Crypto_xVIx_xAIx_ECDSAVrfyCtx.IsBusy)
     )
  {
    *Crypto_xVIx_xAIx_ECDSAVrfyCtx.IsBusy      = FALSE;
    *Crypto_xVIx_xAIx_ECDSAVrfyCtx.ResultError = Crypto_xVIx_xAIx_ECDSAVrfyCtx.ErrorCode;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECCNIST_VRFY_HASH_Callback
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  Std_ReturnType                                     Result
)
{
  TS_PARAM_UNUSED(Job);
  switch(Crypto_xVIx_xAIx_ECDSAVrfyCtx.State)
  {
    case CRYPTO_XVIX_XAIX_ECDSA_V_INIT_WAIT:
    case CRYPTO_XVIX_XAIX_ECDSA_V_WAIT_FOR_UPDATE_HASH_MESSAGE:
    {
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.State = CRYPTO_XVIX_XAIX_ECDSA_V_INITIALIZED;
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.CallbackFlag = TRUE;
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.CallbackResult = Result;
      break;
    }
    case CRYPTO_XVIX_XAIX_ECDSA_V_WAIT_FOR_FINISH_HASH_MESSAGE:
    {
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.State = CRYPTO_XVIX_XAIX_ECDSA_V_RFROMSIGNATURE;
      break;
    }
    /* CHECK: NOPARSE */
    /*All the valid (wait) states have been addressed in the cases above.*/
    default:
    {
      /* default case required by MISRA-C */
      break;
    }
    /* CHECK: PARSE */
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECCNIST_VrfyDefaultWaitState
(
  void
)
{
  switch(Crypto_xVIx_xAIx_ECDSAVrfyCtx.State)
  {
    case CRYPTO_XVIX_XAIX_ECDSA_V_INIT_WAIT:
    case CRYPTO_XVIX_XAIX_ECDSA_V_WAIT_FOR_UPDATE_HASH_MESSAGE:
    case CRYPTO_XVIX_XAIX_ECDSA_V_WAIT_FOR_FINISH_HASH_MESSAGE:
    {
      Crypto_xVIx_xAIx_SHA2256HelperMainFunction();
      break;
    }
    case CRYPTO_XVIX_XAIX_ECDSA_V_IDLE:
    case CRYPTO_XVIX_XAIX_ECDSA_V_INITIALIZED:
    {
      break;
    }
    /* CHECK: NOPARSE */
    /* Defensive Programming - unreachable code. This default case is required by MISRA-C:2012 */
    default:
    {
      /* default case required by MISRA-C */
      break;
    }
    /* CHECK: PARSE */
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECCNIST_VRFY_Callback_Cancel
(
  void
)
{
  /* !LINKSTO SWS_Crypto_00143_CORRECTION_SWS_Crypto_00181, 1, SWS_Crypto_00144_CORRECTION, 1 */
  if(TRUE == Crypto_xVIx_xAIx_ECDSAVrfyCtx.CancelFlag)
  {
    if (
         (NULL_PTR != Crypto_xVIx_xAIx_ECDSAVrfyCtx.ResultError)
         && (NULL_PTR != Crypto_xVIx_xAIx_ECDSAVrfyCtx.IsBusy)
       )
    {
      *Crypto_xVIx_xAIx_ECDSAVrfyCtx.IsBusy = FALSE;
      *Crypto_xVIx_xAIx_ECDSAVrfyCtx.ResultError = CRYPTO_E_JOB_CANCELED;
      /* processingType is synchronous */
      /* !LINKSTO Crypto.Dsn.JobTransition.ActiveCancel2Idle, 1 */
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.Job->jobState = CRYPTO_JOBSTATE_IDLE;
      SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_ECDSAVrfyCtx.ObjId].DriverObjectState =
        CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_ECDSAVrfyCtx.ObjId].CurrentJob =
        NULL_PTR;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_ECDSAVrfyCtx.ObjId].SkipPeriodicMainFunction = TRUE;
      SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
    }
    Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ECCNIST_NOT_SET_CallBack
    (
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.Job,
      CRYPTO_E_JOB_CANCELED
    );
    Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ECCNIST_NOT_SET_Reset();
  }
  else if(TRUE == Crypto_xVIx_xAIx_ECDSAVrfyCtx.CallbackFlag)
  {
    if (
         (NULL_PTR != Crypto_xVIx_xAIx_ECDSAVrfyCtx.ResultError)
         && (NULL_PTR != Crypto_xVIx_xAIx_ECDSAVrfyCtx.IsBusy)
       )
    {
      *Crypto_xVIx_xAIx_ECDSAVrfyCtx.IsBusy = FALSE;
      *Crypto_xVIx_xAIx_ECDSAVrfyCtx.ResultError =
        Crypto_xVIx_xAIx_ECDSAVrfyCtx.CallbackResult;
    }
    if(
        (
          CRYPTO_PROCESSING_ASYNC ==
          Crypto_xVIx_xAIx_ECDSAVrfyCtx.Job->jobPrimitiveInfo->processingType
        ) &&
        (
          (
            (
              ((uint8)Crypto_xVIx_xAIx_ECDSAVrfyCtx.Job->jobPrimitiveInputOutput.mode) &
                ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
            ) == ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
          ) ||
          (
            (
              ((uint8)Crypto_xVIx_xAIx_ECDSAVrfyCtx.Job->jobPrimitiveInputOutput.mode) &
              ((uint8) CRYPTO_OPERATIONMODE_STREAMSTART)
            ) == ( (uint8) CRYPTO_OPERATIONMODE_STREAMSTART)
          )
        ) && (CRYPTO_OPERATIONMODE_START == Crypto_xVIx_xAIx_ECDSAVrfyCtx.CurrentMode)
      )
    {
      (void)Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ECCNIST_NOT_SET_Update
            (
              Crypto_xVIx_xAIx_ECDSAVrfyCtx.ObjId,
              Crypto_xVIx_xAIx_ECDSAVrfyCtx.Job
            );
    }
    else if
    (
      (
        CRYPTO_PROCESSING_ASYNC ==
        Crypto_xVIx_xAIx_ECDSAVrfyCtx.Job->jobPrimitiveInfo->processingType
      ) &&
      (
        (
          (
            ((uint8)Crypto_xVIx_xAIx_ECDSAVrfyCtx.Job->jobPrimitiveInputOutput.mode) &
            ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
          ) == ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
        )
      ) && (CRYPTO_OPERATIONMODE_UPDATE == Crypto_xVIx_xAIx_ECDSAVrfyCtx.CurrentMode)
    )
    {
      (void)Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ECCNIST_NOT_SET_Finish
            (
              Crypto_xVIx_xAIx_ECDSAVrfyCtx.ObjId,
              Crypto_xVIx_xAIx_ECDSAVrfyCtx.Job
            );
    }
    else
    {
      /* Nothing to do.  */
    }

    Crypto_xVIx_xAIx_AL_SIGNATUREVERIFY_ECCNIST_NOT_SET_CallBack
    (
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.Job,
      Crypto_xVIx_xAIx_ECDSAVrfyCtx.CallbackResult
    );
    Crypto_xVIx_xAIx_ECDSAVrfyCtx.CallbackFlag = FALSE;
  }

  else
  {
    /* Nothing to do.  */
  }
}

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON) */

#endif  /* #if (
               (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED  == STD_ON) ||
               (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON)
               )
        */

/*==[end of file]=================================================================================*/


