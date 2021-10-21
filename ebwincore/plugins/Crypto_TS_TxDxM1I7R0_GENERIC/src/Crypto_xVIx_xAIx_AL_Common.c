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

/* !LINKSTO EB_Crypto_CryptAlgos_9001,1 */

/*==[Includes]====================================================================================*/

#include <Std_Types.h>
#include <TSAutosar.h>
#include <TSMem.h>
#include <Crypto_xVIx_xAIx_AL_Mac.h>
#include <Crypto_xVIx_xAIx_AL_Common.h>
#include <Crypto_xVIx_xAIx_AL_KeyManagement.h>

/* !LINKSTO Crypto.Req.Gen/NvM/01019, 1 */
#if (CRYPTO_XVIX_XAIX_ENABLED_NVM == STD_ON)
#include <NvM.h>
#endif /* #if (CRYPTO_XVIX_XAIX_ENABLED_NVM == STD_ON) */

/*==[Macros]======================================================================================*/

/*--LN--------------------------------------------------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_LN_LENGTH_POS)
#error CRYPTO_XVIX_XAIX_LN_LENGTH_POS is already defined
#endif
/** \brief The position of the length in a number. */
#define CRYPTO_XVIX_XAIX_LN_LENGTH_POS 0U

#if (defined CRYPTO_XVIX_XAIX_LN_LSW_POS)
#error CRYPTO_XVIX_XAIX_LN_LSW_POS is already defined
#endif
/** \brief The position of the least significant word in a number. */
#define CRYPTO_XVIX_XAIX_LN_LSW_POS 1U

#if (CPU_TYPE_32 == CPU_TYPE)

#if (defined CRYPTO_XVIX_XAIX_LN_WORD_MAX)
#error CRYPTO_XVIX_XAIX_LN_WORD_MAX already defined
#endif
/** \brief The maximal value which can be stored in an unsigned word. */
#define CRYPTO_XVIX_XAIX_LN_WORD_MAX 0xFFFFFFFFU

#elif (CPU_TYPE_16 == CPU_TYPE)

#if (defined CRYPTO_XVIX_XAIX_LN_WORD_MAX)
#error CRYPTO_XVIX_XAIX_LN_WORD_MAX already defined
#endif
/** \brief The maximal value which can be stored in an unsigned word. */
#define CRYPTO_XVIX_XAIX_LN_WORD_MAX 0xFFFFU

#endif

/** \brief  Compute the amount of words that the long number representation of a
 **    number will need from the length of the number in bytes. This is the value
 **    "(float)x / (float)CSM_LN_WORD_LEN_BYTES" rounded up. We compute this with
 **    the following formula: ((x - 1) / CRY_LN_WORD_LEN_BYTES) + 1. .
 **
 **/
#define CRYPTO_XVIX_XAIX_LNNUMBERLEN(x)             \
(                                                   \
   (Crypto_xVIx_xAIx_LNWordType)                    \
   (                                                \
      (uint32)                                      \
      (                                             \
         (uint32)                                   \
         (                                          \
            (uint32)(x) - (uint32)1                 \
         ) /                                        \
         (uint32)CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES \
      ) +                                           \
     (uint32)1                                      \
   )                                                \
)

#if (defined CRYPTO_XVIX_XAIX_MAX_KEY_NUM_LEN_WORDS)
#error CRYPTO_XVIX_XAIX_MAX_KEY_NUM_LEN_WORDS is already defined
#endif
/** \brief The maximum length of the key in words. */
#define CRYPTO_XVIX_XAIX_MAX_KEY_NUM_LEN_WORDS \
(                                                                                                \
  (Crypto_xVIx_xAIx_LNWordType)                                                                  \
    (                                                                                            \
        CRYPTO_XVIX_XAIX_LNNUMBERLEN(CRYPTO_XVIX_XAIX_AL_ASYM_PRIVATE_MAX_KEY_ELEMENT_SIZE) +   \
    (Crypto_xVIx_xAIx_LNWordType)1UL                                                             \
  )                                                                                              \
)

#if (defined CRYPTO_XVIX_XAIX_MONT_CALCMU)
#error CRYPTO_XVIX_XAIX_MONT_CALCMU is already defined
#endif
/** \brief Calculate Number * Rho. */
#define CRYPTO_XVIX_XAIX_MONT_CALCMU(X, Rho) Crypto_xVIx_xAIx_MontgomeryReductionC[X] * (Rho)

#if (defined CRYPTO_XVIX_XAIX_MONT_INNERMUL)
#error CRYPTO_XVIX_XAIX_MONT_INNERMUL is already defined
#endif
/** \brief Calculate the product and handle the carry. */
#if (CRYPTO_XVIX_XAIX_LN_PLATFORM_DOUBLE_WORD_SUPPORT == STD_ON)
#define CRYPTO_XVIX_XAIX_MONT_INNERMUL(CPtr, Carry, Mu, ModPtr, Y)                                 \
do                                                                                                 \
{                                                                                                  \
  Crypto_xVIx_xAIx_LNDoubleWordType Temp;                                                          \
  Temp =                                                                                           \
    ((Crypto_xVIx_xAIx_LNDoubleWordType)(CPtr)[0U] + (Crypto_xVIx_xAIx_LNDoubleWordType)(Carry)) + \
    (((Crypto_xVIx_xAIx_LNDoubleWordType)(Mu)) * (Crypto_xVIx_xAIx_LNDoubleWordType)(ModPtr)[Y]);  \
  CPtr[0U] = (Crypto_xVIx_xAIx_LNWordType)(Temp & CRYPTO_XVIX_XAIX_LN_WORD_MAX);                   \
  Carry = (Crypto_xVIx_xAIx_LNWordType)(Temp >> CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS);                \
} while (0U)

#else

#define CRYPTO_XVIX_XAIX_MONT_INNERMUL(CPtr, Carry, Mu, ModPtr, Y) \
do                                                                 \
{                                                                  \
  CRYPTO_XVIX_XAIX_U32_MUL(                                        \
                            (ModPtr)[(Y)],                           \
                            (Mu),                                  \
                            Crypto_xVIx_xAIx_TempBuffer[0U],       \
                            Crypto_xVIx_xAIx_TempBuffer[1U]        \
                          );                                       \
  CRYPTO_XVIX_XAIX_U32_ADD(                                        \
                            Crypto_xVIx_xAIx_TempBuffer[0U],       \
                            Crypto_xVIx_xAIx_TempBuffer[1U],       \
                            (Carry),                               \
                            Crypto_xVIx_xAIx_TempBuffer[0U],       \
                            Crypto_xVIx_xAIx_TempBuffer[1U]        \
                          );                                       \
  CRYPTO_XVIX_XAIX_U32_ADD(                                        \
                            Crypto_xVIx_xAIx_TempBuffer[0U],       \
                            Crypto_xVIx_xAIx_TempBuffer[1U],       \
                            (CPtr)[0U],                            \
                            (CPtr)[0U],                            \
                            (Carry)                                \
                          );                                       \
} while (0U)
#endif

#if (defined CRYPTO_XVIX_XAIX_MONT_PROPCARRY)
#error CRYPTO_XVIX_XAIX_MONT_PROPCARRY is already defined
#endif
/** \brief Propagate the carry. */
#define CRYPTO_XVIX_XAIX_MONT_PROPCARRY(CPtr, Carry)                                              \
do                                                                                                \
{                                                                                                 \
  Crypto_xVIx_xAIx_LNWordType Temp;                                                               \
  CPtr[0U] += (Carry);                                                                            \
  Temp = (CPtr)[0U];                                                                              \
  Carry = (Temp < (Carry));                                                                       \
} while (0U)

#if (defined CRYPTO_XVIX_XAIX_MONT_POWOF2)
#error CRYPTO_XVIX_XAIX_MONT_POWOF2 is already defined
#endif
/** \brief Compute 2 ** Exponent */
#define CRYPTO_XVIX_XAIX_MONT_POWOF2(Number, Exponent)                                       \
do                                                                                           \
{                                                                                            \
  uint8 Carry = (((Exponent) % CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS) > 0U);                     \
  Crypto_xVIx_xAIx_LNWordType WordOffset =                                                   \
    (Crypto_xVIx_xAIx_LNWordType)((Exponent) / ((uint32)CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS)); \
  Number[WordOffset + Carry] =                                                               \
    (Crypto_xVIx_xAIx_LNWordType)1U << ((Exponent) % CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS);     \
  Number[0U] = WordOffset + Carry;                                                           \
} while (0U)

#if (defined CRYPTO_XVIX_XAIX_COMBA_FORWARD)
#error CRYPTO_XVIX_XAIX_COMBA_FORWARD is already defined
#endif
/** \brief Forward shift the carry word */
#define CRYPTO_XVIX_XAIX_COMBA_FORWARD(C0, C1, C2) \
do                                                 \
{                                                  \
    C0 = (C1);                                     \
    C1 = (C2);                                     \
    C2 = (0U);                                     \
} while (0U)

#if (defined CRYPTO_XVIX_XAIX_COMBA_MULADD)
#error CRYPTO_XVIX_XAIX_COMBA_MULADD is already defined
#endif
/** \brief Compute the column product */
#if (CRYPTO_XVIX_XAIX_LN_PLATFORM_DOUBLE_WORD_SUPPORT == STD_ON)
#define CRYPTO_XVIX_XAIX_COMBA_MULADD(I, J, C0, C1, C2)                                          \
do                                                                                               \
{                                                                                                \
  Crypto_xVIx_xAIx_LNDoubleWordType Temp;                                                        \
  Temp  = (Crypto_xVIx_xAIx_LNDoubleWordType)(C0) +                                              \
    ((Crypto_xVIx_xAIx_LNDoubleWordType)(I) * (Crypto_xVIx_xAIx_LNDoubleWordType)(J));           \
  C0 = (Crypto_xVIx_xAIx_LNWordType)(Temp & CRYPTO_XVIX_XAIX_LN_WORD_MAX);                       \
  Temp  = (Crypto_xVIx_xAIx_LNDoubleWordType)(C1) + (Temp >> CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS); \
  C1 = (Crypto_xVIx_xAIx_LNWordType)(Temp & CRYPTO_XVIX_XAIX_LN_WORD_MAX);                       \
  C2 += (Crypto_xVIx_xAIx_LNWordType)(Temp >> CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS);                \
} while (0U)

#else

#define CRYPTO_XVIX_XAIX_COMBA_MULADD(I, J, C0, C1, C2)                                         \
do                                                                                              \
{                                                                                               \
  CRYPTO_XVIX_XAIX_U32_MUL(                                                                     \
                            (I),                                                                \
                            (J),                                                                \
                            Crypto_xVIx_xAIx_TempBuffer[0U],                                    \
                            Crypto_xVIx_xAIx_TempBuffer[1U]                                     \
                          );                                                                    \
  CRYPTO_XVIX_XAIX_U32_ADD(                                                                     \
                            Crypto_xVIx_xAIx_TempBuffer[0U],                                    \
                            Crypto_xVIx_xAIx_TempBuffer[1U],                                    \
                            (C0),                                                               \
                            Crypto_xVIx_xAIx_TempBuffer[2U],                                    \
                            Crypto_xVIx_xAIx_TempBuffer[3U]                                     \
                          );                                                                    \
  C0 = Crypto_xVIx_xAIx_TempBuffer[2U];                                                         \
  CRYPTO_XVIX_XAIX_U32_ADD(                                                                     \
                            Crypto_xVIx_xAIx_TempBuffer[3U],                                    \
                            0U,                                                                 \
                            (C1),                                                               \
                            Crypto_xVIx_xAIx_TempBuffer[2U],                                    \
                            Crypto_xVIx_xAIx_TempBuffer[3U]                                     \
                          );                                                                    \
  C1 = Crypto_xVIx_xAIx_TempBuffer[2U];                                                         \
  CRYPTO_XVIX_XAIX_U32_ADD(                                                                     \
                            Crypto_xVIx_xAIx_TempBuffer[3U],                                    \
                            0U,                                                                 \
                            (C2),                                                               \
                            Crypto_xVIx_xAIx_TempBuffer[0U],                                    \
                            Crypto_xVIx_xAIx_TempBuffer[1U]                                     \
                          );                                                                    \
  C2 = Crypto_xVIx_xAIx_TempBuffer[0U];                                                         \
} while (0U)
#endif

#if (defined CRYPTO_XVIX_XAIX_COMBA_SQRADD2)
#error CRYPTO_XVIX_XAIX_COMBA_SQRADD2 is already defined
#endif
/** \brief For squaring some of the terms are doubled */
#if (CRYPTO_XVIX_XAIX_LN_PLATFORM_DOUBLE_WORD_SUPPORT == STD_ON)
#define CRYPTO_XVIX_XAIX_COMBA_SQRADD2(I, J, C0, C1, C2)                                          \
do                                                                                                \
{                                                                                                 \
  Crypto_xVIx_xAIx_LNDoubleWordType Temp, Temp2;                                                  \
  Temp  = ((Crypto_xVIx_xAIx_LNDoubleWordType)(I) * (Crypto_xVIx_xAIx_LNDoubleWordType)(J));      \
  Temp2 = (Crypto_xVIx_xAIx_LNDoubleWordType)(C0) + Temp;                                         \
  C0 = (Crypto_xVIx_xAIx_LNWordType)(Temp2 & CRYPTO_XVIX_XAIX_LN_WORD_MAX);                       \
  Temp2 = (Crypto_xVIx_xAIx_LNDoubleWordType)(C1) + (Temp2 >> CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS); \
  C1 = (Crypto_xVIx_xAIx_LNWordType)(Temp2 & CRYPTO_XVIX_XAIX_LN_WORD_MAX);                       \
  C2 += (Crypto_xVIx_xAIx_LNWordType)(Temp2 >> CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS);                \
  Temp2 = (Crypto_xVIx_xAIx_LNDoubleWordType)(C0) + Temp;                                         \
  C0 = (Crypto_xVIx_xAIx_LNWordType)(Temp2 & CRYPTO_XVIX_XAIX_LN_WORD_MAX);                       \
  Temp2 = (Crypto_xVIx_xAIx_LNDoubleWordType)(C1) + (Temp2 >> CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS); \
  C1 = (Crypto_xVIx_xAIx_LNWordType)(Temp2 & CRYPTO_XVIX_XAIX_LN_WORD_MAX);                       \
  C2 += (Crypto_xVIx_xAIx_LNWordType)(Temp2 >> CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS);                \
} while (0U)
#else
#define CRYPTO_XVIX_XAIX_COMBA_SQRADD2(I, J, C0, C1, C2)     \
do                                                           \
{                                                            \
  CRYPTO_XVIX_XAIX_U32_MUL(                                  \
                            (I),                             \
                            (J),                             \
                            Crypto_xVIx_xAIx_TempBuffer[2U], \
                            Crypto_xVIx_xAIx_TempBuffer[3U]  \
                          );                                 \
  CRYPTO_XVIX_XAIX_U32_ADD(                                  \
                            Crypto_xVIx_xAIx_TempBuffer[2U], \
                            Crypto_xVIx_xAIx_TempBuffer[3U], \
                            (C0),                            \
                            Crypto_xVIx_xAIx_TempBuffer[4U], \
                            Crypto_xVIx_xAIx_TempBuffer[5U]  \
                          );                                 \
  C0 = Crypto_xVIx_xAIx_TempBuffer[4U];                      \
  CRYPTO_XVIX_XAIX_U32_ADD(                                  \
                            Crypto_xVIx_xAIx_TempBuffer[5U], \
                            0U,                              \
                            (C1),                            \
                            Crypto_xVIx_xAIx_TempBuffer[4U], \
                            Crypto_xVIx_xAIx_TempBuffer[5U]  \
                          );                                 \
  C1 = Crypto_xVIx_xAIx_TempBuffer[4U];                      \
  CRYPTO_XVIX_XAIX_U32_ADD(                                  \
                            Crypto_xVIx_xAIx_TempBuffer[5U], \
                            0U,                              \
                            (C2),                            \
                            Crypto_xVIx_xAIx_TempBuffer[0U], \
                            Crypto_xVIx_xAIx_TempBuffer[1U]  \
                          );                                 \
  C2 = Crypto_xVIx_xAIx_TempBuffer[0U];                      \
  CRYPTO_XVIX_XAIX_U32_ADD(                                  \
                            Crypto_xVIx_xAIx_TempBuffer[2U], \
                            Crypto_xVIx_xAIx_TempBuffer[3U], \
                            (C0),                            \
                            Crypto_xVIx_xAIx_TempBuffer[4U], \
                            Crypto_xVIx_xAIx_TempBuffer[5U]  \
                          );                                 \
  C0 = Crypto_xVIx_xAIx_TempBuffer[4U];                      \
  CRYPTO_XVIX_XAIX_U32_ADD(                                  \
                            Crypto_xVIx_xAIx_TempBuffer[5U], \
                            0U,                              \
                            (C1),                            \
                            Crypto_xVIx_xAIx_TempBuffer[4U], \
                            Crypto_xVIx_xAIx_TempBuffer[5U]  \
                          );                                 \
  C1 = Crypto_xVIx_xAIx_TempBuffer[4U];                      \
  CRYPTO_XVIX_XAIX_U32_ADD(                                  \
                            Crypto_xVIx_xAIx_TempBuffer[5U], \
                            0U,                              \
                            (C2),                            \
                            Crypto_xVIx_xAIx_TempBuffer[0U], \
                            Crypto_xVIx_xAIx_TempBuffer[1U]  \
                          );                                 \
  C2 = Crypto_xVIx_xAIx_TempBuffer[0U];                      \
} while (0U)
#endif

#if (CRYPTO_XVIX_XAIX_LN_PLATFORM_DOUBLE_WORD_SUPPORT == STD_OFF)
#if (defined CRYPTO_XVIX_XAIX_U32_MUL)
#error CRYPTO_XVIX_XAIX_U32_MUL is already defined
#endif
/** \brief Calculate P * Q and put the 'MSW' in ResH and the 'LSW' in ResL */
#define CRYPTO_XVIX_XAIX_U32_MUL(P, Q, ResL, ResH)                                                 \
do                                                                                                 \
{                                                                                                  \
  Crypto_xVIx_xAIx_TempBuffer[10U] =                                                               \
    CRYPTO_XVIX_XAIX_LNLOWHALFWORD((Q)) * CRYPTO_XVIX_XAIX_LNLOWHALFWORD((P));                     \
  Crypto_xVIx_xAIx_TempBuffer[6U] =                                                                \
    CRYPTO_XVIX_XAIX_LNLOWHALFWORD(Crypto_xVIx_xAIx_TempBuffer[10U]);                              \
  Crypto_xVIx_xAIx_TempBuffer[10U] =                                                               \
    (CRYPTO_XVIX_XAIX_LNHIGHHALFWORD((Q)) * CRYPTO_XVIX_XAIX_LNLOWHALFWORD((P))) +                 \
    CRYPTO_XVIX_XAIX_LNHIGHHALFWORD(Crypto_xVIx_xAIx_TempBuffer[10U]);                             \
  Crypto_xVIx_xAIx_TempBuffer[7U] =                                                                \
    CRYPTO_XVIX_XAIX_LNLOWHALFWORD(Crypto_xVIx_xAIx_TempBuffer[10U]);                              \
  Crypto_xVIx_xAIx_TempBuffer[8U] =                                                                \
    CRYPTO_XVIX_XAIX_LNHIGHHALFWORD(Crypto_xVIx_xAIx_TempBuffer[10U]);                             \
  Crypto_xVIx_xAIx_TempBuffer[10U] =                                                               \
    Crypto_xVIx_xAIx_TempBuffer[7U] +                                                              \
    (CRYPTO_XVIX_XAIX_LNLOWHALFWORD((Q)) * CRYPTO_XVIX_XAIX_LNHIGHHALFWORD((P)));                  \
  Crypto_xVIx_xAIx_TempBuffer[7U] =                                                                \
    CRYPTO_XVIX_XAIX_LNLOWHALFWORD(Crypto_xVIx_xAIx_TempBuffer[10U]);                              \
  Crypto_xVIx_xAIx_TempBuffer[10U] =                                                               \
    (                                                                                              \
      Crypto_xVIx_xAIx_TempBuffer[8U] +                                                            \
      CRYPTO_XVIX_XAIX_LNHIGHHALFWORD(Crypto_xVIx_xAIx_TempBuffer[10U])                            \
    ) + (CRYPTO_XVIX_XAIX_LNHIGHHALFWORD((Q)) * CRYPTO_XVIX_XAIX_LNHIGHHALFWORD((P)));             \
  Crypto_xVIx_xAIx_TempBuffer[8U] =                                                                \
    CRYPTO_XVIX_XAIX_LNLOWHALFWORD(Crypto_xVIx_xAIx_TempBuffer[10U]);                              \
  Crypto_xVIx_xAIx_TempBuffer[9U] =                                                                \
    CRYPTO_XVIX_XAIX_LNHIGHHALFWORD(Crypto_xVIx_xAIx_TempBuffer[10U]);                             \
  (ResL) =                                                                                         \
    (Crypto_xVIx_xAIx_LNWordType)(                                                                 \
                                   Crypto_xVIx_xAIx_TempBuffer[7U] <<                              \
                                   CRYPTO_XVIX_XAIX_LN_HALFWORD_LEN_BITS                           \
                                 ) | Crypto_xVIx_xAIx_TempBuffer[6U];                              \
  (ResH) =                                                                                         \
    (Crypto_xVIx_xAIx_LNWordType)(                                                                 \
                                   (Crypto_xVIx_xAIx_LNWordType)Crypto_xVIx_xAIx_TempBuffer[9U] << \
                                   CRYPTO_XVIX_XAIX_LN_HALFWORD_LEN_BITS                           \
                                 ) | Crypto_xVIx_xAIx_TempBuffer[8U];                              \
} while (0U)

#if (defined CRYPTO_XVIX_XAIX_U32_ADD)
#error CRYPTO_XVIX_XAIX_U32_ADD is already defined
#endif
/** \brief Calculate [QH:QL] + P and put the 'MSW' in ResH and the 'LSW' in ResL */
#define CRYPTO_XVIX_XAIX_U32_ADD(QL, QH, P, ResL, ResH)                                         \
do                                                                                              \
{                                                                                               \
  Crypto_xVIx_xAIx_TempBuffer[7U] =                                                             \
    CRYPTO_XVIX_XAIX_LNLOWHALFWORD((QL)) + CRYPTO_XVIX_XAIX_LNLOWHALFWORD((P));                 \
  Crypto_xVIx_xAIx_TempBuffer[6U] =                                                             \
    CRYPTO_XVIX_XAIX_LNHIGHHALFWORD(Crypto_xVIx_xAIx_TempBuffer[7U]);                           \
  Crypto_xVIx_xAIx_TempBuffer[8U] =                                                             \
    (CRYPTO_XVIX_XAIX_LNHIGHHALFWORD((QL)) + CRYPTO_XVIX_XAIX_LNHIGHHALFWORD((P))) +            \
    Crypto_xVIx_xAIx_TempBuffer[6U];                                                            \
  (ResL) = ((Crypto_xVIx_xAIx_TempBuffer[7U]) & CRYPTO_XVIX_XAIX_LN_HALFWORD_MAX) |             \
    (Crypto_xVIx_xAIx_LNWordType)                                                               \
    (                                                                                           \
      (Crypto_xVIx_xAIx_LNWordType)(Crypto_xVIx_xAIx_TempBuffer[8U]) <<                         \
      CRYPTO_XVIX_XAIX_LN_HALFWORD_LEN_BITS                                                     \
    );                                                                                          \
  (ResH) = (QH) + ((Crypto_xVIx_xAIx_TempBuffer[8U]) >> CRYPTO_XVIX_XAIX_LN_HALFWORD_LEN_BITS); \
} while (0U)
#endif /* #if (CRYPTO_XVIX_XAIX_LN_PLATFORM_DOUBLE_WORD_SUPPORT == STD_OFF) */

#if (CRYPTO_XVIX_XAIX_RSAPKCS_VRFY_ENABLED == STD_ON)  || \
    (CRYPTO_XVIX_XAIX_RSAPKCS_GEN_ENABLED  == STD_ON)  || \
    (CRYPTO_XVIX_XAIX_RSAPSS_VRFY_ENABLED  == STD_ON)  || \
    (CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_ENABLED == STD_ON) || \
    (CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_ENABLED == STD_ON)
#if (defined CRYPTO_XVIX_XAIX_AL_DER_SEQUENCE_TAG)
#error CRYPTO_XVIX_XAIX_AL_DER_SEQUENCE_TAG is already defined
#endif
/** \brief The DER tag of a Sequence. */
#define CRYPTO_XVIX_XAIX_AL_DER_SEQUENCE_TAG 0x30U

#if (defined CRYPTO_XVIX_XAIX_AL_DER_INTEGER_TAG)
#error CRYPTO_XVIX_XAIX_AL_DER_INTEGER_TAG is already defined
#endif
/** \brief The DER tag of a Integer. */
#define CRYPTO_XVIX_XAIX_AL_DER_INTEGER_TAG 0x02U

#endif /*  (CRYPTO_XVIX_XAIX_RSAPKCS_VRFY_ENABLED == STD_ON)  || \
           (CRYPTO_XVIX_XAIX_RSAPKCS_GEN_ENABLED  == STD_ON)  || \
           (CRYPTO_XVIX_XAIX_RSAPSS_VRFY_ENABLED  == STD_ON)  || \
           (CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_ENABLED == STD_ON) || \
           (CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_ENABLED == STD_ON)*/

#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
    (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)   || \
    (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON)

#if (defined CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1)
#error CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1 is already defined
#endif
/** \brief The offset of the first temporary storage in the elliptic curve temporary buffer. */
#define CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1 (0U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS)

#if (defined CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_2)
#error CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_2 is already defined
#endif
/** \brief The offset of the second temporary storage in the elliptic curve temporary buffer. */
#define CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_2 (1U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS)

#if (defined CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_3)
#error CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_3 is already defined
#endif
/** \brief The offset of the third temporary storage in the elliptic curve temporary buffer. */
#define CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_3 (2U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS)

#if (defined CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_4)
#error CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_4 is already defined
#endif
/** \brief The offset of the fourth temporary storage in the elliptic curve temporary buffer. */
#define CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_4 (3U * CRYPTO_XVIX_XAIX_ECC_TEMP_LEN_WORDS)

#endif /*#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
             (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)   || \
             (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON) */

/*==[Types]=======================================================================================*/

/*==[Declaration of functions with internal linkage]==============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief This function subtracts two numbers.
 *
 * This function subtracts two numbers from each other.
 *
 * \param[out] Result    A pointer to a memory location which will hold the
 *                       result of the subtraction.
 * \param[in] Minuend    A pointer to the minuend of the subtraction.
 * \param[in] Subtrahend A pointer to the subtrahend of the subtraction.
 * \param[in] CutOff     Maximal length of the subtrahend.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNBarretSubtractNumbers
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Result,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Minuend,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Subtrahend,
  Crypto_xVIx_xAIx_LNWordType                                       CutOff
);

/** \brief Helper function to reduce the complexity of the Crypto_xVIx_xAIx_LNModulo function.
 *
 * This is used to reduce the related SW metric values.
 * For more details see the documentation of the Crypto_xVIx_xAIx_LNModulo function.
 *
 * \param[in,out] Number A pointer to the long number representation of the number on which the
 *                       modulo operation should be performed and also the place where the result
 *                       should be stored.
 * \param[in] Modulus    A pointer to the long number representation of the modulus.
 * \param[in,out] CtxPtr A pointer to a context which holds the current state of the modulo
 *                       operation.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNSModuloShift
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Number,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Modulus,
  P2VAR(Crypto_xVIx_xAIx_LNCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    CtxPtr
);

/** \brief Helper function to reduce the complexity of the Crypto_xVIx_xAIx_LNModulo function.
 *
 * This is used to reduce the related SW metric values.
 * For more details see the documentation of the Crypto_xVIx_xAIx_LNModulo function.
 *
 * \param[in,out] Number A pointer to the long number representation of the number on which the
 *                       modulo operation should be performed and also the place where the result
 *                       should be stored.
 * \param[in] Modulus    A pointer to the long number representation of the modulus.
 * \param[in,out] CtxPtr A pointer to a context which holds the current state of the modulo
 *                       operation.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNSModuloShift2
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Number,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Modulus,
  P2VAR(Crypto_xVIx_xAIx_LNCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    CtxPtr
);

/** \brief Helper function to reduce the complexity of the Crypto_xVIx_xAIx_LNModulo function.
 *
 * This is used to reduce the related SW metric values.
 * For more details see the documentation of the Crypto_xVIx_xAIx_LNModulo function.
 *
 * \param[in,out] Number A pointer to the long number representation of the number on which the
 *                       modulo operation should be performed and also the place where the result
 *                       should be stored.
 * \param[in] Modulus    A pointer to the long number representation of the modulus.
 * \param[in,out] CtxPtr A pointer to a context which holds the current state of the modulo
 *                       operation.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNSModuloSubtract
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Number,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Modulus,
  P2VAR(Crypto_xVIx_xAIx_LNCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    CtxPtr
);

/** \brief Helper function to reduce the complexity of the Crypto_xVIx_xAIx_LNModulo function.
 *
 * This is used to reduce the related SW metric values.
 * For more details see the documentation of the Crypto_xVIx_xAIx_LNModulo function.
 *
 * \param[in,out] Number A pointer to the long number representation of the number on which the
 *                       modulo operation should be performed and also the place where the result
 *                       should be stored.
 * \param[in] Modulus    A pointer to the long number representation of the modulus.
 * \param[in,out] CtxPtr A pointer to a context which holds the current state of the modulo
 *                       operation.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNSModuloSubtract2
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Number,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Modulus,
  P2VAR(Crypto_xVIx_xAIx_LNCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    CtxPtr
);

/** \brief Helper function to reduce the complexity of the Crypto_xVIx_xAIx_LNModulo function.
 *
 * This is used to reduce the related SW metric values.
 * For more details see the documentation of the Crypto_xVIx_xAIx_LNModulo function.
 *
 * \param[in,out] Number A pointer to the long number representation of the number on which the
 *                       modulo operation should be performed and also the place where the result
 *                       should be stored.
 * \param[in,out] CtxPtr A pointer to a context which holds the current state of the modulo
 *                       operation.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNSModuloCarry
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Number,
  P2VAR(Crypto_xVIx_xAIx_LNCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  CtxPtr
);

#if (CRYPTO_XVIX_XAIX_RSAPKCS_VRFY_ENABLED == STD_ON) || \
    (CRYPTO_XVIX_XAIX_RSAPKCS_GEN_ENABLED == STD_ON)
/** \brief  Creates the part of a DER header which encodes the payload length
 **
 ** The DER header of an ASN.1 element consists of several parts, one being
 ** the encoding of the length of the payload of the element. This function
 ** creates the length encoding given the payload length
 **
 ** \param[out] LengthEncodingBuffer Holds a pointer to the start of a buffer where the
 ** length encoding should be stored
 ** \param[in] PayloadLength Length of the payload of the ASN.1 element
 **
 ** \returns Length of the length encoding in bytes
 **
 **/
static FUNC(uint32, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CommonDerLengthEncoding
(
   P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) LengthEncodingBuffer,
   uint32                                              PayloadLength
);
#endif /* (CRYPTO_XVIX_XAIX_RSAPKCS_VRFY_ENABLED == STD_ON) */



#if (CRYPTO_XVIX_XAIX_RSAPKCS_VRFY_ENABLED == STD_ON)  || \
    (CRYPTO_XVIX_XAIX_RSAPKCS_GEN_ENABLED  == STD_ON)  || \
    (CRYPTO_XVIX_XAIX_RSAPSS_VRFY_ENABLED  == STD_ON)  || \
    (CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_ENABLED == STD_ON) || \
    (CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_ENABLED == STD_ON)
/** \brief This function converts a DER encoded length into a length in uint32.
 *
 * \param[in] LengthOctets  A pointer to the beginning of the DER encoded length.
 * \param[out] Result       A pointer to store the length in Byte.
 *
 * \return The read Bytes, 0 represents an error
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(uint32, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_GetDERLength
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) LengthOctets,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Result
);

/** \brief This function converts a DER encoded Integer to an array of type LNWordType.
 *
 * \param[in] Data           A pointer to the beginning of the DER encoded data.
 * \param[out] IntegerArray  A pointer to the location where the result shall be stored.
 *
 * \return The read Bytes, 0 represents an error
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(uint32, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_GetDERInteger
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                     Data,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) IntegerArray
);
#endif /*  (CRYPTO_XVIX_XAIX_RSAPKCS_VRFY_ENABLED == STD_ON)  || \
           (CRYPTO_XVIX_XAIX_RSAPKCS_GEN_ENABLED  == STD_ON)  || \
           (CRYPTO_XVIX_XAIX_RSAPSS_VRFY_ENABLED  == STD_ON)  || \
           (CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_ENABLED == STD_ON) || \
           (CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_ENABLED == STD_ON) */

/*----- Elliptic Curves---------------------------------------------------------------------------*/

/*----- Ed25519 /X25519 Fast Reduction -----------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
    (CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED == STD_ON)   || \
    (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON)
/** \brief Reduce point A.
 *
 * \param[in,out] Tmp   Pointer to a buffer which will be used as temporary storage
 *                      during the reduction.
 * \param[in,out] Value Value to be reduced.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAFastReductionCompA
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value
);

/** \brief Reduce point B.
 *
 * \param[out]    Tmp      Pointer to a buffer which will be used as temporary storage
 *                         during the reduction.
 * \param[in,out] Value    Value to be reduced.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAFastReductionCompB
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value
);
#endif /* #if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
              (CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED == STD_ON)   || \
              (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON) */

/*----- ECC P-256 Fast Reduction -----------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
    (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)   || \
    (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON)
/** \brief Calculate s1, s2 and s3 integers.
 *
 * \param[in,out] Value  Value to be reduced.
 * \param[in,out] Temp   Pointer to a buffer which will be used as temporary
 *                       storage during the reduction.
 * \param[in,out] Buffer Pointer to a buffer which will be used as temporary
 *                       storage during the reduction.
 * \param[in,out] CtxPtr Pointer to the elliptic curve context.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECP256FastReductionCalcValue1
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Temp,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Buffer,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Calculate s4, s5 and s6 integers.
 *
 * \param[in,out] Value  Value to be reduced.
 * \param[in,out] Temp   Pointer to a buffer which will be used as temporary
 *                       storage during the reduction.
 * \param[in,out] Buffer Pointer to a buffer which will be used as temporary
 *                       storage during the reduction.
 * \param[in,out] CtxPtr Pointer to the elliptic curve context.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECP256FastReductionCalcValue2
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Temp,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Buffer,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Calculate s4, s5 and s6 integers.
 *
 * \param[in,out] Value  Value to be reduced.
 * \param[in,out]  Temp   Pointer to a buffer which will be used as temporary
 *                       storage during the reduction.
 * \param[in,out]  Buffer Pointer to a buffer which will be used as temporary
 *                       storage during the reduction.
 * \param[in,out] CtxPtr Pointer to the elliptic curve context.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECP256FastReductionCalcValue3
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Temp,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Buffer,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);
#endif /* #if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
              (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)   || \
              (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON) */

/*----- ECC NIST 384 -----------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON)
/** \brief Calculate s1, s2 and s3 integers.
 *
 * \param[in,out] Value  Value to be reduced.
 * \param[in,out] Temp   Pointer to a buffer which will be used as temporary
 *                       storage during the reduction.
 * \param[in,out] Buffer Pointer to a buffer which will be used as temporary
 *                       storage during the reduction.
 * \param[in,out] CtxPtr Pointer to the elliptic curve context.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECP384FastReductionCalcValue1
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Temp,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Buffer,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Calculate s4, s5 and s6 integers.
 *
 * \param[in,out] Value  Value to be reduced.
 * \param[in,out] Temp   Pointer to a buffer which will be used as temporary
 *                       storage during the reduction.
 * \param[in,out] Buffer Pointer to a buffer which will be used as temporary
 *                       storage during the reduction.
 * \param[in,out] CtxPtr Pointer to the elliptic curve context.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECP384FastReductionCalcValue2
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Temp,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Buffer,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Calculate s7 and s8 integers.
 *
 * \param[in,out] Value  Value to be reduced.
 * \param[in,out] Temp   Pointer to a buffer which will be used as temporary
 *                       storage during the reduction.
 * \param[in,out] Buffer Pointer to a buffer which will be used as temporary
 *                       storage during the reduction.
 * \param[in,out] CtxPtr Pointer to the elliptic curve context.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECP384FastReductionCalcValue3
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Temp,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Buffer,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Calculate s9 and s10 integers.
 *
 * \param[in,out] Value  Value to be reduced.
 * \param[in,out] Temp   Pointer to a buffer which will be used as temporary
 *                       storage during the reduction.
 * \param[in,out]  Buffer Pointer to a buffer which will be used as temporary
 *                       storage during the reduction.
 * \param[in,out] CtxPtr Pointer to the elliptic curve context.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECP384FastReductionCalcValue4
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Temp,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Buffer,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);
#endif /* #if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) */

/*----- ECC NIST Point Addition ------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
    (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)   || \
    (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON)
/** \brief Calculate A and B
 *
 *         This function calculates temporary results for the point addition.
 *
 * \param[in,out] Result  Pointer to an Elliptic Curve Point where the Result of the
 *                        addition will be stored in projective coordinates.
 * \param[in]     Addend1 Pointer to an Elliptic Curve Point given in projective coordinates
 *                        which should be added. This parameter may Point to the same memory location
 *                        as the Result Point "Result".
 * \param[in]     Addend2 Pointer to an Elliptic Curve Point given in projective coordinates
 *                        which should be added. This parameter may Point to the same memory location
 *                        as the Result Point "Result".
 * \param[in,out] Tmp     An array where buffers are used as temporary storage during the addition.
 * \param[in,out] CtxPtr  Pointer to a context which holds the current state of the addition.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECPointAdditionCalcAB
(
    P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend1,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend2,
    P2VAR(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
    P2VAR(Crypto_xVIx_xAIx_ECCtxType,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Calculate C
 *
 *         This function calculates temporary results for the point addition.
 *
 * \param[in,out] Result  Pointer to an Elliptic Curve Point where the Result of the
 *                        addition will be stored in projective coordinates.
 * \param[in,out] Tmp     An array where buffers are used as temporary storage during the addition.
 * \param[in,out] CtxPtr  Pointer to a context which holds the current state of the addition.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECPointAdditionCalcC
(
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Calculate final result of Z3
 *
 *         This function calculates the final result of Z3.
 *
 * \param[in,out] Result  Pointer to an Elliptic Curve Point where the Result of the
 *                        addition will be stored in projective coordinates.
 * \param[in,out] CtxPtr  Pointer to a context which holds the current state of the addition.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECPointAdditionCalcFinalZ3
(
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Calculate temporary Point for the point addition
 *
 * \param[in,out] Result  Pointer to an Elliptic Curve Point where the Result of the
 *                        addition will be stored in projective coordinates.
 * \param[in,out] Tmp     Pointer to a buffer which will be used as temporary storage.
 * \param[in,out] CtxPtr  Pointer to a context which holds the current state of the addition.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECPointAdditionCalcInterimTmp4
(
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);


/** \brief Calculate final results of X3 and Y3
 *
 *         This function calculates the final results of X3 and Y3.
 *
 * \param[in,out] Result  Pointer to an Elliptic Curve Point where the Result of the
 *                        addition will be stored in projective coordinates.
 * \param[in,out] Tmp     An array where buffers are used as temporary storage during the addition.
 * \param[in,out] CtxPtr  Pointer to a context which holds the current state of the addition.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECPointAdditionCalcFinalX3Y3
(
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/*----- ECC NIST Point Doubling ------------------------------------------------------------------*/

/** \brief Point doubling
 *
 *         This function doubles a Point on an Elliptic Curve which is given in
 *         projective coordinates and returns the Result in projective coordinates.
 *         The doubling is performed in slices and the current slice is stored in the
 *         given context.
 *
 * \param[in,out] Result  Pointer to an Elliptic Curve Point where the Result of the
 *                        doubling will be stored in projective coordinates.
 * \param[in]     Value   Pointer to an Elliptic Curve Point given in projective
 *                        coordinates which should be doubled.
 * \param[in,out] Tmp     An array where buffers are used as temporary storage during the doubling.
 * \param[in,out] CtxPtr  Pointer to a context which holds the current state of the doubling.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECPointDoubling
(
    P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
    P2VAR(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
    P2VAR(Crypto_xVIx_xAIx_ECCtxType,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Calculate A and B
 *
 *         This function calculates temporary results for the point doubling.
 *
 * \param[in]     Value  Pointer to an Elliptic Curve Point given in projective
 *                       coordinates which should be doubled.
 * \param[in,out] Tmp    An array where buffers are used as temporary storage during the addition.
 * \param[in,out] CtxPtr Pointer to a context which holds the current state of the addition.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECPointDoublingCalcAB
(
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
    P2VAR(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
    P2VAR(Crypto_xVIx_xAIx_ECCtxType,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Calculate intern results
 *
 *         This function calculates temporary results for the point doubling.
 *
 * \param[in,out] Result Pointer to an Elliptic Curve Point where the Result of the
 *                       doubling will be stored in projective coordinates.
 * \param[in]     Value  Pointer to an Elliptic Curve Point given in projective
 *                       coordinates which should be doubled.
 * \param[in,out] Tmp    An array where buffers are used as temporary storage during the addition.
 * \param[in,out] CtxPtr Pointer to a context which holds the current state of the addition.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECPointDoublingCalcInternTmp
(
    P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
    P2VAR(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
    P2VAR(Crypto_xVIx_xAIx_ECCtxType,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Calculate final result of Y3
 *
 *         This function calculates final result of Y3.
 *
 * \param[in,out] Result Pointer to an Elliptic Curve Point where the Result of the
 *                       doubling will be stored in projective coordinates.
 * \param[in,out] Tmp    An array where buffers are used as temporary storage during the addition.
 * \param[in,out] CtxPtr Pointer to a context which holds the current state of the addition.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECPointDoublingCalcFinalY3
(
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Calculate final result of X3 and Z3
 *
 *         This function calculates final result of X3 and Z3.
 *
 * \param[in,out] Result Pointer to an Elliptic Curve Point where the Result of the
 *                       doubling will be stored in projective coordinates.
 * \param[in,out] Tmp    An array where buffers are used as temporary storage during the addition.
 * \param[in,out] CtxPtr Pointer to a context which holds the current state of the addition.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous/Asynchronous}
 *
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECPointDoublingCalcFinalX3Z3
(
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);
#endif /* #if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
              (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)   || \
              (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Constants with internal linkage]=============================================================*/

/*==[Variables with internal linkage]=============================================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_INIT_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

#if (CRYPTO_XVIX_XAIX_LN_PLATFORM_DOUBLE_WORD_SUPPORT == STD_OFF)
/* \brief This variable is used as temporary buffer. */
static VAR(Crypto_xVIx_xAIx_LNWordType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_TempBuffer[11U] = { 0U };

#endif


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
/** \brief Local result buffer for LNSquareComba. */
static VAR(Crypto_xVIx_xAIx_LNWordType, CRYPTO_xVIx_xAIx_VAR)
  Crypto_xVIx_xAIx_LocalResultSquareComba[(CRYPTO_XVIX_XAIX_MAX_KEY_NUM_LEN_WORDS * 2U) - 1U] =
  {
    0U
  };
#endif

/** \brief Local result buffer for LNMontgomeryReduction. */
static VAR(Crypto_xVIx_xAIx_LNWordType, CRYPTO_xVIx_xAIx_VAR)
  Crypto_xVIx_xAIx_MontgomeryReductionC[(CRYPTO_XVIX_XAIX_MAX_KEY_NUM_LEN_WORDS * 2U) - 1U] =
  { 0U };

#if                                                     \
(                                                       \
  (CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_ENABLED == STD_ON) || \
  (CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_ENABLED == STD_ON) || \
  (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED   == STD_ON)  || \
  (CRYPTO_XVIX_XAIX_RSAPSS_VRFY_ENABLED  == STD_ON)  || \
  (CRYPTO_XVIX_XAIX_RSAPKCS_VRFY_ENABLED == STD_ON)  || \
  (CRYPTO_XVIX_XAIX_RSAPKCS_GEN_ENABLED == STD_ON)      \
)
/** \brief Local buffer for LNModExp. */
static VAR(Crypto_xVIx_xAIx_LNWordType, CRYPTO_xVIx_xAIx_VAR)
  Crypto_xVIx_xAIx_ModExpLocalBase[(CRYPTO_XVIX_XAIX_MAX_KEY_NUM_LEN_WORDS * 2U) - 1U] = { 0U };
#endif

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
/** \brief First carry word. */
static VAR(Crypto_xVIx_xAIx_LNWordType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_C0 = 0U;

/** \brief Second carry word. */
static VAR(Crypto_xVIx_xAIx_LNWordType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_C1 = 0U;

/** \brief Third carry word. */
static VAR(Crypto_xVIx_xAIx_LNWordType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_C2 = 0U;
#endif

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Constants with external linkage]=============================================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CONST_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
    (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)   || \
    (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON)  || \
    (CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED == STD_ON)   || \
    (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON)
/** \brief The elliptic curve informations.
 *         When a new curve is added, the default values have to be adapted.
 */
CONST(Crypto_xVIx_xAIx_EllipticCurveInfoType, CRYPTO_xVIx_xAIx_CONST) Crypto_xVIx_xAIx_EllipticCurveInfo[] =
{
  /* Elliptic Curve SecP256r1 / NIST P-256 / Prime256v1 Informations */
  {
    /* !LINKSTO EB_Crypto_CryptAlgos_1602, 1 */
    /* Prime number of Elliptic Curve P-256. */
    {
      0x00000008UL,
      0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL, 0x00000000UL,
      0x00000000UL, 0x00000000UL, 0x00000001UL, 0xFFFFFFFFUL,
      0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL
    },
    /* !LINKSTO EB_Crypto_CryptAlgos_1603, 1 */
    /* Base Point */
    {
      /* X */
      {
        0x00000008UL,
        0xD898C296UL, 0xF4A13945UL, 0x2DEB33A0UL, 0x77037D81UL,
        0x63A440F2UL, 0xF8BCE6E5UL, 0xE12C4247UL, 0x6B17D1F2UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL
      },
      /* Y */
      {
        0x00000008UL,
        0x37BF51F5UL, 0xCBB64068UL, 0x6B315ECEUL, 0x2BCE3357UL,
        0x7C0F9E16UL, 0x8EE7EB4AUL, 0xFE1A7F9BUL, 0x4FE342E2UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL
      },
      /* Z */
      {
        0x00000001UL,
        0x00000001UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL
      }
    },
    /* !LINKSTO EB_Crypto_CryptAlgos_1604, 1 */
    /* Modulus */
    {
      0x00000008UL,
      0xFC632551UL, 0xF3B9CAC2UL, 0xA7179E84UL, 0xBCE6FAADUL,
      0xFFFFFFFFUL, 0xFFFFFFFFUL, 0x00000000UL, 0xFFFFFFFFUL,
      0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL
    },
    /* !LINKSTO EB_Crypto_CryptAlgos_1605, 1 */
    /* Coeff1 - A: coefficient of y^2 = x^3 + Ax + B */
    {
      0x00000008UL,
      0xFFFFFFFCUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL, 0x00000000UL,
      0x00000000UL, 0x00000000UL, 0x00000001UL, 0xFFFFFFFFUL,
      0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL
    },
    /* !LINKSTO EB_Crypto_CryptAlgos_1606, 1 */
    /* Coeff2 - B: coefficient of y^2 = x^3 + Ax + B */
    {
      0x00000008UL,
      0x27D2604BUL, 0x3BCE3C3EUL, 0xCC53B0F6UL, 0x651D06B0UL,
      0x769886BCUL, 0xB3EBBD55UL, 0xAA3A93E7UL, 0x5AC635D8UL,
      0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL
    },
    /* !LINKSTO EB_Crypto_CryptAlgos_1635, 1 */
    /* DER-Encoding of the Curve */
    {
      8U,
      {
        0x2AU, 0x86U, 0x48U, 0xCEU, 0x3DU, 0x03U, 0x01U, 0x07U,
        0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U
      }
    },
    CRYPTO_ALGOFAM_ECCNIST,
    0U,
    0U,
    /* Length of private key */
    CRYPTO_XVIX_XAIX_CURVE_SECP256R1_PRIVKEY_LEN_BYTES,
    /* Length of public key */
    CRYPTO_XVIX_XAIX_CURVE_SECP256R1_PUBKEY_LEN_BYTES,
#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
    (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)   || \
    (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON)
    Crypto_xVIx_xAIx_ECP256FastReduction
#else
    NULL_PTR
#endif /* #if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
              (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)   || \
              (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON) */
  },
  /* Elliptic Curve SecP384r1 / NIST P-384 Informations */
  {
    /* !LINKSTO EB_Crypto_CryptAlgos_1642, 1 */
    /* Prime number of Elliptic Curve P-384. */
    {
      0x0000000CUL,
      0xFFFFFFFFUL, 0x00000000UL, 0x00000000UL, 0xFFFFFFFFUL,
      0xFFFFFFFEUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL,
      0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL
    },
    /* !LINKSTO EB_Crypto_CryptAlgos_1643, 1 */
    /* Base Point */
    {
      /* X */
      {
        0x0000000CUL,
        0x72760AB7UL, 0x3A545E38UL, 0xBF55296CUL, 0x5502F25DUL,
        0x82542A38UL, 0x59F741E0UL, 0x8BA79B98UL, 0x6E1D3B62UL,
        0xF320AD74UL, 0x8EB1C71EUL, 0xBE8B0537UL, 0xAA87CA22UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL
      },
      /* Y */
      {
        0x0000000CUL,
        0x90EA0E5FUL, 0x7A431D7CUL, 0x1D7E819DUL, 0x0A60B1CEUL,
        0xB5F0B8C0UL, 0xE9DA3113UL, 0x289A147CUL, 0xF8F41DBDUL,
        0x9292DC29UL, 0x5D9E98BFUL, 0x96262C6FUL, 0x3617DE4AUL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL
      },
      /* Z */
      {
        0x00000001UL,
        0x00000001UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL
      }
    },
    /* !LINKSTO EB_Crypto_CryptAlgos_1644, 1 */
    /* Modulus */
    {
      0x0000000CUL,
      0xCCC52973UL, 0xECEC196AUL, 0x48B0A77AUL, 0x581A0DB2UL,
      0xF4372DDFUL, 0xC7634D81UL, 0xFFFFFFFFUL, 0xFFFFFFFFUL,
      0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL
    },
    /* !LINKSTO EB_Crypto_CryptAlgos_1645, 1 */
    /* Coeff1 - A: coefficient of y^2 = x^3 + Ax + B */
    {
      0x0000000CUL,
      0xFFFFFFFCUL, 0x00000000UL, 0x00000000UL, 0xFFFFFFFFUL,
      0xFFFFFFFEUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL,
      0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL
    },
    /* !LINKSTO EB_Crypto_CryptAlgos_1646, 1 */
    /* Coeff2 - B: coefficient of y^2 = x^3 + Ax + B */
    {
      0x0000000CUL,
      0xD3EC2AEFUL, 0x2A85C8EDUL, 0x8A2ED19DUL, 0xC656398DUL,
      0x5013875AUL, 0x0314088FUL, 0xFE814112UL, 0x181D9C6EUL,
      0xE3F82D19UL, 0x988E056BUL, 0xE23EE7E4UL, 0xB3312FA7UL
    },
    /* !LINKSTO EB_Crypto_CryptAlgos_1636, 1 */
    /* DER-Encoding of the Curve */
    {
      5U,
      {
        0x2BU, 0x81U, 0x04U, 0x00U, 0x22U, 0x00U, 0x00U, 0x00U,
        0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U
      }
    },
    CRYPTO_ALGOFAM_ECCNIST,
    0U,
    0U,
    /* Length of private key */
    CRYPTO_XVIX_XAIX_CURVE_SECP384R1_PRIVKEY_LEN_BYTES,
    /* Length of public key */
    CRYPTO_XVIX_XAIX_CURVE_SECP384R1_PUBKEY_LEN_BYTES,
#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON)
    Crypto_xVIx_xAIx_ECP384FastReduction
#else
    NULL_PTR
#endif /* #if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) */
  },
  /* X25519 Informations, This curve is birationally equivalent to a twisted Edwards Curve  */
  {
    /* !LINKSTO EB_Crypto_CryptAlgos_0404, 1 */
    /* Prime number of the Curve. */
    {
      0x00000008UL,
      0xFFFFFFEDUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL,
      0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL, 0x7FFFFFFFUL,
      0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
    },
    /* !LINKSTO EB_Crypto_CryptAlgos_0710, 1 */
    /* Base Point */
    {
      /* X */
      {
        0x00000008UL,
        0x8F25D51AUL, 0xC9562D60UL, 0x9525A7B2UL, 0x692CC760UL,
        0xFDD6DC5CUL, 0xC0A4E231UL, 0xCD6E53FEUL, 0x216936D3UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL
      },
      /* Y */
      {
        0x00000008UL,
        0x66666658UL, 0x66666666UL, 0x66666666UL, 0x66666666UL,
        0x66666666UL, 0x66666666UL, 0x66666666UL, 0x66666666UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL
      },
      /* Z */
      {
        0x00000001UL,
        0x00000001UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL
      }
    },
    /* !LINKSTO EB_Crypto_CryptAlgos_0708,1 */
    /* Modulus */
    {
      0x00000008UL,
      0x5CF5D3EDUL, 0x5812631AUL, 0xA2F79CD6UL, 0x14DEF9DEUL,
      0x00000000UL, 0x00000000UL, 0x00000000UL, 0x10000000UL,
      0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
    },
    /* !LINKSTO EB_Crypto_CryptAlgos_0403,1 */
    /* Coeff1 - UCoordinate: The u-coordinate of the base Point.*/
    {
      0x00000001UL,
      0x00000009UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
      0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
      0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL
    },
    /* !LINKSTO EB_Crypto_CryptAlgos_0405,1 */
    /* Coeff2 - A24: An element in the finite field GF(p), not equal to -2 or 2. */
    {
      0x00000001UL,
      0x0001DB41UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
      0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
      0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL
    },
    /* !LINKSTO EB_Crypto_CryptAlgos_0406,1 */
    /* DER-Encoding of the Curve */
    {
      3U,
      {
        0x2BU, 0x65U, 0x6EU, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
        0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U
      }
    },
    CRYPTO_ALGOFAM_ED25519,
    0U,
    0U,
    /* Length of private key */
    CRYPTO_XVIX_XAIX_CURVE_X25519_PRIVKEY_LEN_BYTES,
    /* Length of public and secret shared key */
    CRYPTO_XVIX_XAIX_CURVE_X25519_PUBKEY_LEN_BYTES,
#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
    (CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED == STD_ON)   || \
    (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON)
    Crypto_xVIx_xAIx_EdDSAFastReduction
#else
    NULL_PTR
#endif /* #if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
              (CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED == STD_ON)   || \
              (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON) */
  },
  /* Ed25519 Informations */
  {
    /* !LINKSTO EB_Crypto_CryptAlgos_0704,1 */
    /* Prime number of Edwards Curve Ed25519. */
    {
      0x00000008UL,
      0xFFFFFFEDUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL,
      0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL, 0x7FFFFFFFUL,
      0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL
    },
    /* !LINKSTO EB_Crypto_CryptAlgos_0710,1 */
    /* Base Point */
    {
      /* X */
      {
        0x00000008UL,
        0x8F25D51AUL, 0xC9562D60UL, 0x9525A7B2UL, 0x692CC760UL,
        0xFDD6DC5CUL, 0xC0A4E231UL, 0xCD6E53FEUL, 0x216936D3UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL
      },
      /* Y */
      {
        0x00000008UL,
        0x66666658UL, 0x66666666UL, 0x66666666UL, 0x66666666UL,
        0x66666666UL, 0x66666666UL, 0x66666666UL, 0x66666666UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL
      },
      /* Z */
      {
        0x00000001UL,
        0x00000001UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL
      }
    },
    /* !LINKSTO EB_Crypto_CryptAlgos_0708,1 */
    /* Modulus - LValue: The order of G(p). */
    {
      0x00000008UL,
      0x5CF5D3EDUL, 0x5812631AUL, 0xA2F79CD6UL, 0x14DEF9DEUL,
      0x00000000UL, 0x00000000UL, 0x00000000UL, 0x10000000UL,
      0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL
    },
    /* !LINKSTO EB_Crypto_CryptAlgos_0709,1 */
    /* Coeff1 - DValue: A non-square element d of G(p). */
    {
      0x00000008UL,
      0x135978A3UL, 0x75EB4DCAUL, 0x4141D8ABUL, 0x00700A4DUL,
      0x7779E898UL, 0x8CC74079UL, 0x2B6FFE73UL, 0x52036CEEUL,
      0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL
    },
    /* !LINKSTO EB_Crypto_CryptAlgos_0706,1 */
    /* Coeff2 - NValue: An integer n with c <= n <= b. */
    {
      0x00000001UL,
      0x000000FEUL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
      0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
      0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL
    },
    /* !LINKSTO EB_Crypto_CryptAlgos_0711, 1 */
    /* DER-Encoding of the Curve */
    {
      3U,
      {
        0x2BU, 0x65U, 0x70U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
        0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U
      }
    },
    CRYPTO_ALGOFAM_ED25519,
    /* !LINKSTO EB_Crypto_CryptAlgos_0707,1 */
    /* BValue: An integer b with 2^(b-1) > Prime. EdDSA public keys have exactly b
     *         bits, and EdDSA Signatures have exactly 2*b bits.
     */
    256U,
    /* !LINKSTO EB_Crypto_CryptAlgos_0705,1 */
    /* C: The integer in c = {2, 3}. */
    3U,
    /* Length of private key */
    CRYPTO_XVIX_XAIX_CURVE_ED25519_PRIVKEY_LEN_BYTES,
    /* Length of public key */
    CRYPTO_XVIX_XAIX_CURVE_ED25519_PUBKEY_LEN_BYTES,
#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
    (CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED == STD_ON)   || \
    (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON)
    Crypto_xVIx_xAIx_EdDSAFastReduction
#else
    NULL_PTR
#endif /* #if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
              (CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED == STD_ON)   || \
              (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON) */
  }
};
#endif /* #if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
              (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)   || \
              (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON)  || \
              (CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED == STD_ON)   || \
              (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_CONST_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Variables with external linkage]=============================================================*/

/*==[Definition of functions with external linkage]===============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_MemCmp
(
  CONSTP2CONST(uint8, AUTOMATIC, BASE_APPL_DATA) A,
  CONSTP2CONST(uint8, AUTOMATIC, BASE_APPL_DATA) B,
               uint32                            Length
)
{
  return TS_MemCmp(A, B, Length);
}

FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_MemCmp_Sec
(
  CONSTP2CONST(uint8, AUTOMATIC, BASE_APPL_DATA) A,
  CONSTP2CONST(uint8, AUTOMATIC, BASE_APPL_DATA) B,
               uint32                            Length
)
{
  Std_ReturnType RetVal;
  uint8          Result = 0U;
  uint32         Counter;

  for (Counter = 0U; Counter < Length; Counter++)
  {
    Result |= A[Counter] ^ B[Counter];
  }
  if (0U == Result)
  {
    RetVal = E_OK;
  }
  else
  {
    RetVal = E_NOT_OK;
  }

  return RetVal;
}

FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_StateMachineIdle
(
  void
)
{
  return E_OK;
}

FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CheckSkipMain
(
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy,
  CONST(uint32, CRYPTO_xVIx_xAIx_CONST)                       ObjId
)
{
  boolean SkipMain = FALSE;
  if (
       (NULL_PTR == Result) &&
       (NULL_PTR == Busy) &&
       (TRUE == Crypto_xVIx_xAIx_DriverObjects[ObjId].SkipPeriodicMainFunction)
     )
  {
    SkipMain = TRUE;
  }
  return SkipMain;
}

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
FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNSquareComba
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Result,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Number
)
{
  Crypto_xVIx_xAIx_LNWordType i, j, Tx, Ty, NumMultiplications;
  Crypto_xVIx_xAIx_LNWordType MultiplicandLength = Number[CRYPTO_XVIX_XAIX_LN_LENGTH_POS];
  Crypto_xVIx_xAIx_LNWordType ProdLen            = MultiplicandLength + MultiplicandLength;

  /* reset ctx variable */
  TS_MemBZero(
               Crypto_xVIx_xAIx_LocalResultSquareComba,
               sizeof(Crypto_xVIx_xAIx_LocalResultSquareComba)
             );
  Crypto_xVIx_xAIx_C0 = 0U;
  Crypto_xVIx_xAIx_C1 = 0U;
  Crypto_xVIx_xAIx_C2 = 0U;
  for (i = 0U; i < ProdLen; i++)
  {
    Ty = CRYPTO_XVIX_XAIX_MINIMUM((MultiplicandLength - 1U), i);
    Tx = (i - Ty);

    NumMultiplications =
      CRYPTO_XVIX_XAIX_MINIMUM(
                                (Crypto_xVIx_xAIx_LNWordType)(MultiplicandLength - Tx),
                                (Crypto_xVIx_xAIx_LNWordType)(Ty + 1U)
                              );
    NumMultiplications = CRYPTO_XVIX_XAIX_MINIMUM(NumMultiplications, (((Ty - Tx) + 1U) >> 1U));

    /* Forward shift the carry word */
    CRYPTO_XVIX_XAIX_COMBA_FORWARD(Crypto_xVIx_xAIx_C0, Crypto_xVIx_xAIx_C1, Crypto_xVIx_xAIx_C2);

    for (j = 0U; j < NumMultiplications; j++)
    {
      /* Double the terms */
      CRYPTO_XVIX_XAIX_COMBA_SQRADD2(
                                      (Number[(Tx + j) + 1U]),
                                      (Number[(Ty - j) + 1U]),
                                      Crypto_xVIx_xAIx_C0,
                                      Crypto_xVIx_xAIx_C1,
                                      Crypto_xVIx_xAIx_C2
                                    );
    }

    if ((i & 1U) == 0U)
    {
      Crypto_xVIx_xAIx_LNWordType Offset = (i >> 1U) + (Crypto_xVIx_xAIx_LNWordType)1U;
      Crypto_xVIx_xAIx_LNWordType ADp = Number[Offset];
      CRYPTO_XVIX_XAIX_COMBA_MULADD(
                                     (ADp),
                                     (Number[Offset]),
                                     Crypto_xVIx_xAIx_C0,
                                     Crypto_xVIx_xAIx_C1,
                                     Crypto_xVIx_xAIx_C2
                                   );
    }

    /* Propagate carry */
    Crypto_xVIx_xAIx_LocalResultSquareComba[i + 1U] = Crypto_xVIx_xAIx_C0;
  }

  Crypto_xVIx_xAIx_LocalResultSquareComba[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] =
    MultiplicandLength + MultiplicandLength;

  Crypto_xVIx_xAIx_LNNormalizeNumber(Crypto_xVIx_xAIx_LocalResultSquareComba);

  TS_MemCpy(
             Result,
             Crypto_xVIx_xAIx_LocalResultSquareComba,
             (
               (
                 (uint32)Crypto_xVIx_xAIx_LocalResultSquareComba[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] +
                 (uint32)1U
               ) * (uint32)CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES
             )
           );
}
#endif

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNModuloBarrett
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Number,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Modulus,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Barrett,
  P2VAR(Crypto_xVIx_xAIx_LNCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    CtxPtr
)
{
  Crypto_xVIx_xAIx_LNWordType LoopCount;
  Crypto_xVIx_xAIx_LNWordType* TempNumber;

  /* The input 'Number' is needed twice during the computation, but once it is shifted
     'Modulus[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]-1' words to the right and once it is cropped to the
     last 'Modulus[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]+1' words. This means that on the first occasion
     all the words of 'Number' except for the last 'Modulus[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]-1' are
     needed and on the second occasion only the last 'Modulus[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]+1'
     words of 'Number' are needed. So, with the exception of a two word overlap, two
     distinct parts of 'Number' are needed during the computation. So, we
     begin by splitting 'Number' into the two distinct parts with a little
     shifting. Then we have both parts at our disposal. */

  /* The more significant part of 'Number' is shifted 3 blocks to the left.  */
  for (
        LoopCount = Number[CRYPTO_XVIX_XAIX_LN_LENGTH_POS];
        LoopCount >= Modulus[CRYPTO_XVIX_XAIX_LN_LENGTH_POS];
        LoopCount--
      )
  {
    Number[LoopCount + (Crypto_xVIx_xAIx_LNWordType)3U] = Number[LoopCount];
  }

  /* The length of the two parts is computed. The left part starts at
     Number[Modulus[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]+2]. */
  if (Modulus[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] > Number[CRYPTO_XVIX_XAIX_LN_LENGTH_POS])
  {
    /* The Number is so small that the right shifted part is zero and the
       cropped part is the Number itself. */
    Number[Modulus[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] + (Crypto_xVIx_xAIx_LNWordType)2U] =
    (Crypto_xVIx_xAIx_LNWordType)0U;
  }
  else
  {
    /* The length of the right shifted part. */
    Number[Modulus[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] + (Crypto_xVIx_xAIx_LNWordType)2U] =
    (Crypto_xVIx_xAIx_LNWordType)
    (
      Number[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] +
      (Crypto_xVIx_xAIx_LNWordType)1U
    ) - Modulus[CRYPTO_XVIX_XAIX_LN_LENGTH_POS];

    /* The length of the cropped part. */
    Number[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] =
    Modulus[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] + (Crypto_xVIx_xAIx_LNWordType)1U;
  }

  /* TempNumber is set to the start of the newly created Number. */
  TempNumber = &Number[Modulus[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] + (Crypto_xVIx_xAIx_LNWordType)2U];

  /* "TempNumber" now contains the value of "Number" shifted
     "Modulus[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]-1" words to the right, while "Number" itself now
     contains the old value of Number cropped to the last
     "Modulus[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]+1" words. So, in the notation of Menezes et al.,
     'TempNumber' contains q1, while 'Number' contains r1. */

  /* q2 = q1 * mu. q2 is stored in "TempNumber". */
  Crypto_xVIx_xAIx_LNMultiplyNumbers(TempNumber, TempNumber, Barrett, CtxPtr);

  /* q3 = q2 / b**(k+1). This is a right shift by 'Modulus[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]+1'
     words. q3 is stored in "TempNumber".
  */
  if (Modulus[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] >= TempNumber[CRYPTO_XVIX_XAIX_LN_LENGTH_POS])
  {
    /* 'TempNumber' is so small that the right shift makes it 0. */
    TempNumber[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] = (Crypto_xVIx_xAIx_LNWordType)0U;
  }
  else
  {
    for (
          LoopCount = (Modulus[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] + (Crypto_xVIx_xAIx_LNWordType)2U);
          LoopCount <= TempNumber[CRYPTO_XVIX_XAIX_LN_LENGTH_POS];
          LoopCount++
        )
    {
      TempNumber[
                  LoopCount - (Modulus[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] +
                  (Crypto_xVIx_xAIx_LNWordType)1U)
                ] = TempNumber[LoopCount];
    }

    TempNumber[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] -=
    (Crypto_xVIx_xAIx_LNWordType)
    (
      Modulus[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] + (Crypto_xVIx_xAIx_LNWordType)1U
    );
  }

  /* r3 = q3 * m. This step is not in the book. There, it is combined with
     the next step and we go directly to r2 = (q3 * m) mod b**(k+1). r3 is
     stored in TempNumber. */
  Crypto_xVIx_xAIx_LNMultiplyNumbers(TempNumber, TempNumber, Modulus, CtxPtr);

  /* r2 = r3 mod b**(k+1). r2 is stored in TempNumber. */
  if(
      (Modulus[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] + (Crypto_xVIx_xAIx_LNWordType)1U) <
      TempNumber[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]
    )
  {
    TempNumber[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] =
    Modulus[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] + (Crypto_xVIx_xAIx_LNWordType)1U;
  }

  /* We now compute the following two steps of the algorithm in one go:
       3. r = r1 - r2.
       4. if r < 0 then r = r + b**(k+1).
     r is stored in "Number". */
  Crypto_xVIx_xAIx_LNBarretSubtractNumbers
  (
     Number,
     Number,
     TempNumber,
     Modulus[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] + (Crypto_xVIx_xAIx_LNWordType)1U
  );

  /* We know that the following while loop can never be repeated more than
     twice with the correct Barrett value. So, we count the repetitions. */
  LoopCount = (Crypto_xVIx_xAIx_LNWordType)0U;

  /* while r > m do r = r - m. r is stored in Number. After this while-loop
     has finished, the modulo computation is finished and the result is in
     "Number". */
  while
  (
     (CRYPTO_XVIX_XAIX_E_SMALLER != Crypto_xVIx_xAIx_LNCompareNumbers(Number,Modulus)) &&
     (LoopCount < (Crypto_xVIx_xAIx_LNWordType)4U)
  )
  {
    Crypto_xVIx_xAIx_LNBarretSubtractNumbers
    (
       Number,
       Number,
       Modulus,
       CRYPTO_XVIX_XAIX_LN_WORD_MAX
    );

     LoopCount++;
  }

  /* Did we do too many loop repetitions? */
  if ((Crypto_xVIx_xAIx_LNWordType)4U == LoopCount)
  {
    /* Something went wrong. Probably an incorrect Barrett value. We set
       the result to 0. */
    Number[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] = (Crypto_xVIx_xAIx_LNWordType)0U;
  }
}

#if \
(\
  (CRYPTO_XVIX_XAIX_RSAPSS_VRFY_ENABLED  == STD_ON) || \
  (CRYPTO_XVIX_XAIX_RSAPKCS_VRFY_ENABLED == STD_ON)    \
)
FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNModExpBarrett
(
   P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Result,
   P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Base,
   P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Modulus,
   P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Exponent,
   P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Barrett,
   P2VAR(Crypto_xVIx_xAIx_LNCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    CtxPtr
)
{
  uint32 Bits = Crypto_xVIx_xAIx_LNBitLengthOfNumber(Exponent);

  TS_PARAM_UNUSED(CtxPtr);

  Result[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] = 1U;
  Result[CRYPTO_XVIX_XAIX_LN_LSW_POS] = 1U;
  while ((Bits) > 0U)
  {
    --(Bits);

    /* Result = Result**2 mod Modulus */
#if (CRYPTO_XVIX_XAIX_LN_PLATFORM_DOUBLE_WORD_SUPPORT == STD_ON)
    Crypto_xVIx_xAIx_LNSquareComba(Result, Result);
#else
    Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                        Result,
                                        Result,
                                        Result,
                                        CtxPtr
                                      );
#endif

    Crypto_xVIx_xAIx_LNModuloBarrett(Result, Modulus, Barrett, CtxPtr);

    if ((Crypto_xVIx_xAIx_LNWordType)1U == CRYPTO_XVIX_XAIX_LNGETBIT(Exponent, Bits))
    {
      Crypto_xVIx_xAIx_LNMultiplyNumbers(Result, Result, Base, CtxPtr);

      Crypto_xVIx_xAIx_LNModuloBarrett(Result, Modulus, Barrett, CtxPtr);
    }
  }

  return TRUE;
}
#endif /* #if (STD_ON == CRYPTO_XVIX_XAIX_RSAPSS_VRFY_ENABLED) */

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CommonXOR
(
  P2VAR(void, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Destination,
  P2CONST(void, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Source,
  uint32                                     ByteLength
)
{
  uint32_least Len; /* internal Length Value */

  /* loop over the block Length */
  for(Len = 0U; Len < ByteLength; ++Len)
  {
    /* XOR byte-wise */
    ((P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA))Destination)[Len] ^=
    ((P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA))Source)[Len];
  }
}

/*--LN--------------------------------------------------------------------------------------------*/

FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNIsValid
(
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) LongNumber,
  uint32                                                                      PermittedMaxLength
)
{
  boolean Status = FALSE;
  if (
       (LongNumber[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] < PermittedMaxLength) &&
       (
         (0U != LongNumber[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]) &&
         (0U != LongNumber[LongNumber[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]])
       )
     )
  {
    Status = TRUE;
  }
  return Status;
}

FUNC(uint32, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNBitLengthOfNumber
(
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Number
)
{
  Crypto_xVIx_xAIx_LNWordType TempWord; /* the most significant word of "Number" */
  Crypto_xVIx_xAIx_LNWordType Length; /* the length of the number */

  if ( 0U == Number[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] )
  {
    Length = 0U;
  }
  else
  {
    /* All words of the number except for the most significant word contribute
       CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS bits to the Length of the number. */
    Length = (Crypto_xVIx_xAIx_LNWordType)
      (Number[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]-(Crypto_xVIx_xAIx_LNWordType)1U) *
      (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS;

    /* We compute the amount of bits which the most significant word of
       "Number" contributes to its length. In each round of this for-loop,
       the most significant word is shifted one bit to the right and we test
       it still is non-zero. */
    for (
          TempWord = Number[Number[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]];
          ((Crypto_xVIx_xAIx_LNWordType)0U < TempWord);
          TempWord >>= 1U
        )
    {
      Length++;
    }
  }
  return Length;
}

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNNormalizeNumber
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Number
)
{
  Crypto_xVIx_xAIx_LNWordType i; /* loop counter */

  i = Number[CRYPTO_XVIX_XAIX_LN_LENGTH_POS];

  while(((Crypto_xVIx_xAIx_LNWordType)0U == Number[i]) && ((Crypto_xVIx_xAIx_LNWordType)0U < i))
  {
    i--;
  }

  Number[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] = i;
}

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNMultiplyNumbers
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Result,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Multiplicand,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Multiplicator,
  P2VAR(Crypto_xVIx_xAIx_LNCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    CtxPtr
)
{
#if (CRYPTO_XVIX_XAIX_LN_PLATFORM_DOUBLE_WORD_SUPPORT == STD_ON)
  /* check if size is ok*/
  Crypto_xVIx_xAIx_LNWordType i, j, NumMultiplications, Tx, Ty;
  Crypto_xVIx_xAIx_LNWordType MultiplicandLength  =
    Multiplicand[CRYPTO_XVIX_XAIX_LN_LENGTH_POS];
  Crypto_xVIx_xAIx_LNWordType MultiplicatorLength =
    Multiplicator[CRYPTO_XVIX_XAIX_LN_LENGTH_POS];
  Crypto_xVIx_xAIx_LNWordType ProdLen = MultiplicandLength + MultiplicatorLength;

  Crypto_xVIx_xAIx_LNWordType C0 = 0U;
  Crypto_xVIx_xAIx_LNWordType C1 = 0U;
  Crypto_xVIx_xAIx_LNWordType C2 = 0U;

  TS_PARAM_UNUSED(CtxPtr);

  /* reset ctx variable */
  TS_MemBZero(CtxPtr->LocalResultMultiply, CRYPTO_XVIX_XAIX_RESULT_MULTI_LENGTH);

  for (i = 0U; i < ProdLen; i++)
  {
    /* Word offset of the Multiplicator */
    Ty = CRYPTO_XVIX_XAIX_MINIMUM((MultiplicatorLength - 1U), i);
    /* Word offset of the Multiplicand */
    Tx = i - Ty;

    NumMultiplications =
      CRYPTO_XVIX_XAIX_MINIMUM((Crypto_xVIx_xAIx_LNWordType)(MultiplicandLength - Tx), (Ty + 1U));

    /* Forward shift the carry word */
    CRYPTO_XVIX_XAIX_COMBA_FORWARD(C0, C1, C2);

    for (j = 0U; j < NumMultiplications; j++)
    {
      /* Compute the column product */
      CRYPTO_XVIX_XAIX_COMBA_MULADD(
                                     (Multiplicand[(Tx + j) + 1U]),
                                     (Multiplicator[(Ty - j) + 1U]),
                                     C0,
                                     C1,
                                     C2
                                   );
    }

    /* Propagate carry */
    CtxPtr->LocalResultMultiply[i + 1U] = C0;
  }

  CtxPtr->LocalResultMultiply[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] =
    MultiplicandLength + MultiplicatorLength;

  Crypto_xVIx_xAIx_LNNormalizeNumber(CtxPtr->LocalResultMultiply);

  TS_MemCpy(
             Result,
             CtxPtr->LocalResultMultiply,
             (
               (
                 (uint32)CtxPtr->LocalResultMultiply[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] +
                 (uint32)1U
               ) * (uint32)CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES
             )
           );

#else

  Crypto_xVIx_xAIx_LNWordType TempI = 0U;
  Crypto_xVIx_xAIx_LNWordType TempJ = 0U;
  Crypto_xVIx_xAIx_LNWordType Temp_Word = 0U;

  if(
      ((Crypto_xVIx_xAIx_LNWordType)0U == Multiplicand[0U]) ||
      ((Crypto_xVIx_xAIx_LNWordType)0U == Multiplicator[0U])
    )
  {
    /* One of the inputs was 0, so the result is also 0. */
    Result[0U] = 0U;
  }
  else
  {
    /* The highest word of the result is initialized to 0. This word is
       not set directly during the following multiplication, but can
       be reached through overflows during the computation of the lower
       words. */
    Result[Multiplicand[0U] + Multiplicator[0U]] = 0U;

    TempI =
    (Crypto_xVIx_xAIx_LNWordType)(Multiplicand[0U] + Multiplicator[0U]) -
    (Crypto_xVIx_xAIx_LNWordType)1U;
    /* Compute OSUM*/
    while(0U < TempI)
    {
      /* We compute the outer sum "\sum_{i=1}^{n+m-1}" from top to bottom. */
      if(TempI >= Multiplicand[0U])
      {
        TempJ =
        (Crypto_xVIx_xAIx_LNWordType)(TempI - Multiplicand[0U]) + (Crypto_xVIx_xAIx_LNWordType)1U;
      }
      else
      {
        TempJ = 1U;
      }
      CtxPtr->TempResult = (Crypto_xVIx_xAIx_LNWordType)0U;

      /* Compute ISUM */
      /* We compute the inner sum "\sum_{j=\max\{1,i-n+1\}}^{\min\{i,m\}}". */
      /* Multiply 'Multiplicand[i-j+1]' with 'Multiplicator[j]' and store
         the lower word of the result in 'rl' and the higher word  in
         'Carry'. 'temp' is a temporary variable. */
      while((TempJ <= TempI) && (TempJ <= Multiplicator[0U]))
      {
        Temp_Word =
        CRYPTO_XVIX_XAIX_LN_LOW_HALF_WORD(
                                           Multiplicand[
                                                         (TempI - TempJ) +
                                                         (Crypto_xVIx_xAIx_LNWordType)1U
                                                       ]
                                         ) *
        CRYPTO_XVIX_XAIX_LN_HIGH_HALF_WORD(Multiplicator[TempJ]);

        CtxPtr->Rl =
        CRYPTO_XVIX_XAIX_LN_HIGH_HALF_WORD(
                                            Multiplicand[
                                                          (TempI - TempJ) +
                                                          (Crypto_xVIx_xAIx_LNWordType)1U
                                                        ]
                                          ) *
        CRYPTO_XVIX_XAIX_LN_LOW_HALF_WORD(Multiplicator[TempJ]);

        Temp_Word += CtxPtr->Rl;

        if (Temp_Word < CtxPtr->Rl)
        {
          CtxPtr->Carry = CRYPTO_XVIX_XAIX_LN_HIGH_HALF_WORD(Temp_Word) |
          (Crypto_xVIx_xAIx_LNWordType)
          (
            (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LN_HALFWORD_MAX +
            (Crypto_xVIx_xAIx_LNWordType)1U
          );
        }
        else
        {
          CtxPtr->Carry = CRYPTO_XVIX_XAIX_LN_HIGH_HALF_WORD(Temp_Word);
        }

        /* Compute ISUM2 */
        CtxPtr->Rl =
        (Crypto_xVIx_xAIx_LNWordType)
        (CRYPTO_XVIX_XAIX_LN_LOW_HALF_WORD(Temp_Word) << (CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS >> 1U));

        Temp_Word = CRYPTO_XVIX_XAIX_LN_LOW_HALF_WORD(
                                                       Multiplicand[
                                                                     (TempI - TempJ) +
                                                                     (Crypto_xVIx_xAIx_LNWordType)1U
                                                                   ]
                                                     ) *
        CRYPTO_XVIX_XAIX_LN_LOW_HALF_WORD(Multiplicator[TempJ]);

        /* The low word of the multiplication result is computed. */
        CtxPtr->Rl += Temp_Word;

        if (CtxPtr->Rl < Temp_Word)
        {
          CtxPtr->Carry++;
        }

        /* The high word of the multiplication result is stored in
           'Carry'. */
        CtxPtr->Carry +=
        (Crypto_xVIx_xAIx_LNWordType)
        (
          CRYPTO_XVIX_XAIX_LN_HIGH_HALF_WORD
          (
            Multiplicand[(TempI - TempJ) + (Crypto_xVIx_xAIx_LNWordType)1U]
          ) *
          CRYPTO_XVIX_XAIX_LN_HIGH_HALF_WORD(Multiplicator[TempJ])
        );

        /* The low word of the multiplication result is added to
           'TempResult'. */
        CtxPtr->TempResult += CtxPtr->Rl;

        if(CtxPtr->TempResult < CtxPtr->Rl)
        {
          /* There was an overflow. So, we increase the Carry by one.
             This increment cannot overflow since the Carry, being the
             high word of the multiplication of two words, is at least
             one smaller than the maximal possible Value. */
          CtxPtr->Carry++;
        }

        /* We Carry the Carry upward until it has trickled away. */
        Temp_Word = (Crypto_xVIx_xAIx_LNWordType)1U;

        /* Multiply Carry */
        while((Crypto_xVIx_xAIx_LNWordType)0U != CtxPtr->Carry)
        {
          Result[TempI + Temp_Word] += CtxPtr->Carry;

          if(Result[TempI + Temp_Word] < CtxPtr->Carry)
          {
            /* We had another overflow. */
            CtxPtr->Carry = 1U;
          }
          else
          {
            /* No overflow, the Carry has trickled away. */
            CtxPtr->Carry = 0U;
          }
          Temp_Word++;
        }
        TempJ++;
      }
      /* Now we are sure that we do not need values of the input arrays up
         to position i anymore, so we can write the result array. */
      Result[TempI] = CtxPtr->TempResult;
      TempI--;
    }
    /* We give the result the maximal possible Length. */
    Result[0U] = Multiplicand[0U] + Multiplicator[0U];

    /* We remove leading zeroes from the result. */
    Crypto_xVIx_xAIx_LNNormalizeNumber(Result);
  }
#endif
}

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNMultByTwo
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Number
)
{
  Crypto_xVIx_xAIx_LNWordType x, OldUsed, Carry, NextCarry, *SrcPtr, *DestPtr;

  OldUsed = Result[CRYPTO_XVIX_XAIX_LN_LENGTH_POS];
  Result[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] = Number[CRYPTO_XVIX_XAIX_LN_LENGTH_POS];

  /* Aliases for source and destination */
  SrcPtr = &Number[CRYPTO_XVIX_XAIX_LN_LSW_POS];
  DestPtr = &Result[CRYPTO_XVIX_XAIX_LN_LSW_POS];

  Carry = 0U;
  for (x = 0; x < Number[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]; x++)
  {
    /* Get what will be the *next* carry bit from the
       MSB of the current digit */
    NextCarry = SrcPtr[x] >>
      ((Crypto_xVIx_xAIx_LNWordType)((CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS) - 1U));

    /* Now shift up this digit, add in the carry [from the previous] */
    DestPtr[x] = (
                   (Crypto_xVIx_xAIx_LNWordType)(
                                                  (Crypto_xVIx_xAIx_LNWordType)SrcPtr[x] <<
                                                  ((Crypto_xVIx_xAIx_LNWordType)1U)
                                                ) | Carry
                 );

    /* Copy the carry that would be from the source
     * digit into the next iteration */
    Carry = NextCarry;
  }

  /* New leading digit? */
  if (Carry != 0)
  {
    /* Add B MSB which is always 1 at this point */
    DestPtr[x] = 1U;
    ++(Result[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]);
  }

  /* Now zero any excess digits on the destination
   * that we didn't write to */
  DestPtr = &(Result[Result[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] + 1U]);

  for (x = Result[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]; x < OldUsed; x++)
  {
    DestPtr[x] = 0U;
  }
}

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNMontgomerySetup
(
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Modulo,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Rho
)
{
  Crypto_xVIx_xAIx_LNWordType X, B;

  /* Fast inversion mod 2**k
   *
   * Based on the fact that
   *
   * XA = 1 (mod 2**n)  =>  (X(2-XA)) A = 1 (mod 2**2n)
   *                    =>  2*X*A - X*X*A*A = 1
   *                    =>  2*(1) - (1)     = 1
   */
  B = Modulo[CRYPTO_XVIX_XAIX_LN_LSW_POS];

  /* here x*a==1 mod 2**4 */
  X = (Crypto_xVIx_xAIx_LNWordType)((Crypto_xVIx_xAIx_LNWordType)((B + 2U) & 4U) << 1U) + B;
  /* here x*a==1 mod 2**8 */
  X *= (Crypto_xVIx_xAIx_LNWordType)((Crypto_xVIx_xAIx_LNWordType)2U - (B * X));
  /* here x*a==1 mod 2**16 */
  X *= (Crypto_xVIx_xAIx_LNWordType)((Crypto_xVIx_xAIx_LNWordType)2U - (B * X));
  /* here x*a==1 mod 2**32 */
  X *= (Crypto_xVIx_xAIx_LNWordType)((Crypto_xVIx_xAIx_LNWordType)2U - (B * X));

  /* rho = -1/m mod b */
  *Rho = (Crypto_xVIx_xAIx_LNWordType)((Crypto_xVIx_xAIx_LNWordType)(~X) + 1U);
}

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNMontgomeryNormalization
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Result,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Modulo
)
{
  uint32 Bits;
  Crypto_xVIx_xAIx_LNWordType x;

  Bits = Crypto_xVIx_xAIx_LNBitLengthOfNumber(Modulo) % CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS;
  if (Bits == 0U)
  {
    Bits = CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS;
  }

  /* compute A = B^(n-1) * 2^(bits-1) */
  /* The else branch cannot be reached in a real world situation */

  if (Modulo[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] > 1U)

  {
    uint32 Exponent = (
                        (
                          ((uint32)Modulo[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] - 1U) *
                            CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS
                        ) + Bits
                      ) - 1U;

    for (x = CRYPTO_XVIX_XAIX_LN_LSW_POS; x <= Result[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]; x++)
    {
      Result[x] = 0U;
    }

    CRYPTO_XVIX_XAIX_MONT_POWOF2(Result, Exponent);
  }
  else
  {
    Result[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] = 1U;
    Result[CRYPTO_XVIX_XAIX_LN_LSW_POS] = 1U;
    Bits = 1U;
  }

  /* Now compute C = A * B mod b */
  for (
        x = (Crypto_xVIx_xAIx_LNWordType)(Bits - 1U);
        x < (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS;
        x++
      )
  {
    /* Find R */
    Crypto_xVIx_xAIx_LNMultByTwo(Result, Result);

    /* If the result is larger than Modulo reduce it as we only need the quantity */
    /* that, when addded to the input makes it divisible by R without remainder. */
    if (Crypto_xVIx_xAIx_LNCompareNumbers(Result, Modulo) != CRYPTO_XVIX_XAIX_E_SMALLER)
    {
      Crypto_xVIx_xAIx_LNSubtraction(Result, Result, Modulo);
    }
  }
}

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNMontgomeryReduction
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Remainder,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Number,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Modulo,
  Crypto_xVIx_xAIx_LNWordType                                       Rho
)
{
  Crypto_xVIx_xAIx_LNWordType Mu, x, y, OldUsed, Carry, *CPtr, *DestPtr;
  Crypto_xVIx_xAIx_LNWordType Pa = Modulo[CRYPTO_XVIX_XAIX_LN_LENGTH_POS];

  /* reset ctx variable */
  TS_MemBZero(Crypto_xVIx_xAIx_MontgomeryReductionC, sizeof(Crypto_xVIx_xAIx_MontgomeryReductionC));
  /* Copy the input */
  OldUsed = Number[CRYPTO_XVIX_XAIX_LN_LENGTH_POS];
  for (x = CRYPTO_XVIX_XAIX_LN_LSW_POS; x <= OldUsed; x++)
  {
    Crypto_xVIx_xAIx_MontgomeryReductionC[x - 1U] = Number[x];
  }

  for (x = 0U; x < Pa; x++)
  {
    Carry = 0U;

    /* Get Mu for this round */
    Mu = CRYPTO_XVIX_XAIX_MONT_CALCMU(x, Rho);

    CPtr = &(Crypto_xVIx_xAIx_MontgomeryReductionC[x]);

    for (y = 0U; y < Pa; y++)
    {
      CRYPTO_XVIX_XAIX_MONT_INNERMUL(CPtr, Carry, Mu, &(Modulo[1U]), y);
      CPtr = &(CPtr[1U]);
    }

    while (Carry != 0U)
    {
      CRYPTO_XVIX_XAIX_MONT_PROPCARRY(CPtr, Carry);
      CPtr = &(CPtr[1U]);
    }
  }

  /* Now copy out the result */
  CPtr = &(Crypto_xVIx_xAIx_MontgomeryReductionC[Pa]);
  DestPtr = &(Remainder[CRYPTO_XVIX_XAIX_LN_LSW_POS]);
  for (x = 0U; x < (Pa + 1U); x++)
  {
    DestPtr[x] = CPtr[x];
  }

  /* Zero out the remaining words */
  for (; x < OldUsed; x++)
  {
    DestPtr[x] = 0U;
  }

  Remainder[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] = Pa + 1U;

  Crypto_xVIx_xAIx_LNNormalizeNumber(Remainder);

  /* if A >= m then A = A - m */
  if (Crypto_xVIx_xAIx_LNCompareNumbers(Remainder, Modulo) != CRYPTO_XVIX_XAIX_E_SMALLER)
  {
    Crypto_xVIx_xAIx_LNSubtraction(Remainder, Remainder, Modulo);
  }
}

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNSubtraction
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Result,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Minuend,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Subtrahend
)
{
#if (CRYPTO_XVIX_XAIX_LN_PLATFORM_DOUBLE_WORD_SUPPORT == STD_ON)
  Crypto_xVIx_xAIx_LNDoubleWordType TempDifference;
  Crypto_xVIx_xAIx_LNWordType LoopCount;
  Crypto_xVIx_xAIx_LNWordType Borrow = 0U;

  /* Calculate the differences up to the maximum available number of common digits
   * (Subtrahend is either smaller or equal to Minuend)
   */
  for (
        LoopCount = CRYPTO_XVIX_XAIX_LN_LSW_POS;
        LoopCount <= Subtrahend[CRYPTO_XVIX_XAIX_LN_LENGTH_POS];
        LoopCount++
      )
  {
    /* Calculate the difference */
    TempDifference = (
                       (Crypto_xVIx_xAIx_LNDoubleWordType)Minuend[LoopCount] -
                       (Crypto_xVIx_xAIx_LNDoubleWordType)Subtrahend[LoopCount]
                     ) - (Crypto_xVIx_xAIx_LNDoubleWordType)Borrow;

    /* Extract the borrow bit */
    Borrow =
      ((Crypto_xVIx_xAIx_LNWordType)(TempDifference >> CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS) & 1U);

    /* The result of the column is equal to the lower part of the difference */
    Result[LoopCount] = (Crypto_xVIx_xAIx_LNWordType)TempDifference;
  }

  /* Copy the remaining digits */
  for (; LoopCount <= Minuend[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]; LoopCount++)
  {
    /* Calculate the difference */
    TempDifference = (Crypto_xVIx_xAIx_LNDoubleWordType)Minuend[LoopCount] -
      (Crypto_xVIx_xAIx_LNDoubleWordType)Borrow;

    /* Extract the borrow bit */
    Borrow =
      ((Crypto_xVIx_xAIx_LNWordType)(TempDifference >> CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS) & 1U);

    /* The result of the column is equal to the lower part of the difference */
    Result[LoopCount] = (Crypto_xVIx_xAIx_LNWordType)TempDifference;
  }
#else
  Crypto_xVIx_xAIx_LNWordType LoopCount;
  Crypto_xVIx_xAIx_LNWordType Borrow = 0U;

  /* Calculate the differences up to the maximum available number of common digits
   * (Subtrahend is either smaller or equal to Minuend)
   */
  for (
        LoopCount = CRYPTO_XVIX_XAIX_LN_LSW_POS;
        LoopCount <= Subtrahend[CRYPTO_XVIX_XAIX_LN_LENGTH_POS];
        LoopCount++
      )
  {
    Crypto_xVIx_xAIx_LNWordType LowerDifference;
    Crypto_xVIx_xAIx_LNWordType UpperDifference;

    /* Lower half words */
    /* Calculate the difference */
    LowerDifference =
      (
        (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LNLOWHALFWORD(Minuend[LoopCount]) -
        (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LNLOWHALFWORD(Subtrahend[LoopCount])
      ) - (Crypto_xVIx_xAIx_LNWordType)Borrow;

    /* Extract the borrow bit */
    Borrow = (LowerDifference >> CRYPTO_XVIX_XAIX_LN_HALFWORD_LEN_BITS) & 1U;

    /* The lower difference is equal to the lower part of the difference */
    LowerDifference &= CRYPTO_XVIX_XAIX_LN_HALFWORD_MAX;

    /* Upper half words */
    /* Calculate the difference */
    UpperDifference =
      (
        (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LNHIGHHALFWORD(Minuend[LoopCount]) -
        (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LNHIGHHALFWORD(Subtrahend[LoopCount])
      ) - (Crypto_xVIx_xAIx_LNWordType)Borrow;

    /* Extract the borrow bit */
    Borrow = (UpperDifference >> CRYPTO_XVIX_XAIX_LN_HALFWORD_LEN_BITS) & 1U;

    /* The upper difference is equal to the lower part of the difference */
    UpperDifference &= CRYPTO_XVIX_XAIX_LN_HALFWORD_MAX;

    /* Concatenate the partial differences */
    Result[LoopCount] =
      (Crypto_xVIx_xAIx_LNWordType)(UpperDifference << CRYPTO_XVIX_XAIX_LN_HALFWORD_LEN_BITS) |
      LowerDifference;
  }

  /* Copy the remaining digits */
  for (; LoopCount <= Minuend[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]; LoopCount++)
  {
    Crypto_xVIx_xAIx_LNWordType LowerDifference;
    Crypto_xVIx_xAIx_LNWordType UpperDifference;

    /* Calculate the difference */
    LowerDifference =
      (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LNLOWHALFWORD(Minuend[LoopCount]) -
      (Crypto_xVIx_xAIx_LNWordType)Borrow;

    /* Extract the borrow bit */
    Borrow = (LowerDifference >> CRYPTO_XVIX_XAIX_LN_HALFWORD_LEN_BITS) & 1U;

    /* The lower difference is equal to the lower part of the difference */
    LowerDifference &= CRYPTO_XVIX_XAIX_LN_HALFWORD_MAX;

    /* Upper half words */
    /* Calculate the difference */
    UpperDifference =
      (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LNHIGHHALFWORD(Minuend[LoopCount]) -
      (Crypto_xVIx_xAIx_LNWordType)Borrow;

    /* Extract the borrow bit */
    Borrow = (UpperDifference >> CRYPTO_XVIX_XAIX_LN_HALFWORD_LEN_BITS) & 1U;

    /* The upper difference is equal to the lower part of the difference */
    UpperDifference &= CRYPTO_XVIX_XAIX_LN_HALFWORD_MAX;

    /* Concatenate the partial differences */
    Result[LoopCount] = (UpperDifference << CRYPTO_XVIX_XAIX_LN_HALFWORD_LEN_BITS) |
      LowerDifference;
  }
#endif

  /* Set the length of the result */
  Result[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] = LoopCount - (Crypto_xVIx_xAIx_LNWordType)1U;

  Crypto_xVIx_xAIx_LNNormalizeNumber(Result);
}

/* !LINKSTO EB_Crypto_CryptAlgos_0950, 1 */
FUNC(Crypto_xVIx_xAIx_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNCompareNumbers
(
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Number1,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Number2
)
{
  Crypto_xVIx_xAIx_LNWordType NumberPos; /* the current position in the numbers */
  Crypto_xVIx_xAIx_ReturnType Compare; /* the result of the comparison */

  /* First, the lengths of the two numbers are compared. If we find a
     difference here, the comparison is over. */
  if(Number1[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] > Number2[CRYPTO_XVIX_XAIX_LN_LENGTH_POS])
  {
    Compare = CRYPTO_XVIX_XAIX_E_LARGER;
  }
  else if(Number1[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] < Number2[CRYPTO_XVIX_XAIX_LN_LENGTH_POS])
  {
    Compare = CRYPTO_XVIX_XAIX_E_SMALLER;
  }
  else
  {
    /* Both numbers have an identical Length. Now, we compare the words of
       those numbers from most to least significant word. */

    Compare = CRYPTO_XVIX_XAIX_E_EQUAL;

    for
    (
      NumberPos = Number1[CRYPTO_XVIX_XAIX_LN_LENGTH_POS];
      (NumberPos > (Crypto_xVIx_xAIx_LNWordType)0U) && (CRYPTO_XVIX_XAIX_E_EQUAL == Compare);
      NumberPos--
    )
    {
      if(Number1[NumberPos] > Number2[NumberPos])
      {
        Compare = CRYPTO_XVIX_XAIX_E_LARGER;
      }
      else if(Number1[NumberPos] < Number2[NumberPos])
      {
        Compare = CRYPTO_XVIX_XAIX_E_SMALLER;
      }
      else
      {
        /* Do nothing. The numbers are identical up to here and this is
           reflected by the fact that 'compare' is EQUAL at the moment. */
      }
    }
  }
  return Compare;
}

/* !LINKSTO EB_Crypto_CryptAlgos_0955, 1 */
FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNModulo
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Number,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Modulus,
  P2VAR(Crypto_xVIx_xAIx_LNCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    CtxPtr
)
{
  uint32 LengthOfNum; /* the Length of number in bits */
  uint32 LengthOfMod; /* the Length of modulus in bits */

  /* We compute the Length of number in bits. */
  LengthOfNum = Crypto_xVIx_xAIx_LNBitLengthOfNumber(Number);

  /* We compute the Length of modulus in bits. */
  LengthOfMod = Crypto_xVIx_xAIx_LNBitLengthOfNumber(Modulus);

  /* We only do something if number is at least as long as modulus.
     Otherwise, result = number mod modulus = number.
     If the Modulus is 0 -> the result is 0. */
  if (0U == LengthOfMod)
  {
    Number[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] = LengthOfMod;
  }
  else if (LengthOfMod <= LengthOfNum)
  {
    /* We compute the Length difference of number and modulus in bits.
       We add 1 because we will subtract 1 at the beginning of the
       do-while-loop. */
    CtxPtr->LengthDifference = (Crypto_xVIx_xAIx_LNWordType)(LengthOfNum - LengthOfMod + 1U);

    /* Binary division: We shift modulus an amount of
       CtxPtr->LengthDifference bits to the left. Then we check whether
       number is at least as large as the shifted modulus. If this is
       the case we subtract the shifted modulus from number. We decrease
       CtxPtr->LengthDifference by one and repeat this game until
       CtxPtr->LengthDifference is 0.
       The modulus is not really shifted but the individual words of the
       shifted modulus are computed when they are needed and stored in
       CtxPtr->WordOfShiftedMod. */
    do
    {
      Crypto_xVIx_xAIx_LNSModuloShift(Number, Modulus, CtxPtr);
    }while((Crypto_xVIx_xAIx_LNWordType)0U < (CtxPtr->LengthDifference));
    Crypto_xVIx_xAIx_LNNormalizeNumber(Number);
  }
  else
  {
    Crypto_xVIx_xAIx_LNNormalizeNumber(Number);
  }
}

/* !LINKSTO EB_Crypto_CryptAlgos_0954, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNBEByteArrayToNumber
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                     Array,
  uint32                                                                    ArrayLength,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Number,
  Crypto_xVIx_xAIx_LNWordType                                               NumberLength
)
{
   Crypto_xVIx_xAIx_LNWordType NumberPos; /* the position in 'number' */
   Crypto_xVIx_xAIx_LNWordType WordPos;  /* the byte position in the current word of 'number' */
   uint32 ArrayPos; /* the position in the byte array */
   Std_ReturnType RetVal = E_NOT_OK;

  if (0U != ArrayLength)
  {
    if (NumberLength < (CRYPTO_XVIX_XAIX_LN_NUMBER_LEN(ArrayLength) + 1U))
    {
      RetVal = E_NOT_OK;
    }
    else
    {
      /* We cycle through all of the words of the resulting number */
      for
      (
        NumberPos = 1U;
        NumberPos <= CRYPTO_XVIX_XAIX_LN_NUMBER_LEN(ArrayLength);
        NumberPos++
      )
      {
        /* The current word of number is initialized to '0'. */
        Number[NumberPos] = 0U;

        /* "CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES" bytes of the array are combined to make up
           the current word of the number. */
        for
        (
          WordPos = 0U;
          WordPos < (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES;
          WordPos++
        )
        {
          /* We compute the current position in 'array'. */
          ArrayPos = CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES;
          ArrayPos *= ((uint32)NumberPos - 1U);
          ArrayPos += (uint32)WordPos;
          ArrayPos = (ArrayLength - ArrayPos) - 1U;

          if(ArrayPos < ArrayLength)
          {
            /* If we are not over the edge of 'array', we incorporate the
               current byte of 'array' into the current word of the number. */
            Number[NumberPos] |=
              (Crypto_xVIx_xAIx_LNWordType)
              (
                (Crypto_xVIx_xAIx_LNWordType)Array[ArrayPos] <<
                (Crypto_xVIx_xAIx_LNWordType)((Crypto_xVIx_xAIx_LNWordType)WordPos << 3U)
              );
          }
        }
      }

      Number[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] = CRYPTO_XVIX_XAIX_LN_NUMBER_LEN(ArrayLength);

      Crypto_xVIx_xAIx_LNNormalizeNumber(Number);
      RetVal = E_OK;
    }
  }
  else
  {
    if (NumberLength == 0U)
    {
      RetVal = E_NOT_OK;
    }
    else
    {
      /* The length of 'array' is '0', so the resulting number is also '0'. */
      Number[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] = 0U;
      RetVal = E_OK;
    }
  }
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_0951, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNNumberToBEByteArray
(
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Number,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                         Array,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                        ArrayLengthPtr
)
{
  Crypto_xVIx_xAIx_LNWordType NumberPos; /* the position in 'number' */
  Crypto_xVIx_xAIx_LNWordType WordPos; /* the byte position in the current word of 'number' */
  Crypto_xVIx_xAIx_LNWordType ArrayPos; /* the position in the byte array */
  Std_ReturnType RetVal = E_OK;

  if(
      ((*ArrayLengthPtr) / CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES) <
      Number[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]
    )
  {
    RetVal = E_NOT_OK;
  }
  else
  {
    /* We cycle through all the words of 'number'. */
    for(
         NumberPos = (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LN_LSW_POS;
         NumberPos <= Number[CRYPTO_XVIX_XAIX_LN_LENGTH_POS];
         NumberPos++
       )
    {
      /* Begin computing the index of the byte which will be updated. */
      ArrayPos = Number[CRYPTO_XVIX_XAIX_LN_LENGTH_POS];
      ArrayPos -= NumberPos;
      ArrayPos += (Crypto_xVIx_xAIx_LNWordType)1U;
      ArrayPos *= (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES;

      /* Each word of 'number' is split into 'CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES' bytes of
         'array'. */
      for
      (
        WordPos = (Crypto_xVIx_xAIx_LNWordType)0U;
        (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES > WordPos;
        WordPos++
      )
      {
        /* Finish computing the index of the byte which will be updated. */
        ArrayPos -= (Crypto_xVIx_xAIx_LNWordType)1U;

        /* The current byte of 'array' is computed from the current word of
           'number'. */
        Array[ArrayPos] = (uint8)(Number[NumberPos] >> (uint8)((uint8)WordPos << 3U));
      }
    }

    /* Return the correct Length of the byte array. */
    *ArrayLengthPtr =
      (uint16)(
                Number[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] *
                  (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES
              );
  }

  return RetVal;
}

#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON)
/* !LINKSTO EB_Crypto_CryptAlgos_0976, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNNumberToBEByteArrayWithSize
(
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Number,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                         Array,
  uint32                                                                      ArrayLength
)
{
  Crypto_xVIx_xAIx_LNWordType NumberPos; /* the position in 'number' */
  Crypto_xVIx_xAIx_LNWordType WordPos; /* the byte position in the current word of 'number' */
  Crypto_xVIx_xAIx_LNWordType ArrayPos; /* the position in the byte array */
  Std_ReturnType RetVal = E_OK;

  if(
      (ArrayLength / CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES) <
      Number[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]
    )
  {
    RetVal = E_NOT_OK;
  }
  else
  {
    TS_MemBZero(Array, ArrayLength);

    /* We cycle through all the words of 'number'. */
    for(
         NumberPos = (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LN_LSW_POS;
         NumberPos <= Number[CRYPTO_XVIX_XAIX_LN_LENGTH_POS];
         NumberPos++
       )
    {
      /* Begin computing the index of the byte which will be updated. */
      ArrayPos  = (Crypto_xVIx_xAIx_LNWordType)ArrayLength;
      ArrayPos -= (NumberPos - (Crypto_xVIx_xAIx_LNWordType)1U) * (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES;

      /* Each word of 'number' is split into 'CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES' bytes of
         'array'. */
      for
      (
        WordPos = (Crypto_xVIx_xAIx_LNWordType)0U;
        (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES > WordPos;
        WordPos++
      )
      {
        /* Finish computing the index of the byte which will be updated. */
        ArrayPos -= (Crypto_xVIx_xAIx_LNWordType)1U;

        /* The current byte of 'array' is computed from the current word of
           'number'. */
        Array[ArrayPos] = (uint8)(Number[NumberPos] >> (uint8)((uint8)WordPos << 3U));
      }
    }
  }

  return RetVal;
}
#endif /* #if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) */

/* !LINKSTO EB_Crypto_CryptAlgos_0953, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNLEByteArrayToNumber
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                     Array,
  uint32                                                                    ArrayLength,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Number,
  Crypto_xVIx_xAIx_LNWordType                                               NumberLength
)
{
  Crypto_xVIx_xAIx_LNWordType NumberPos; /* the position in 'Number' */
  Crypto_xVIx_xAIx_LNWordType WordPos; /* byte position in the current word of 'Number' */
  uint16 ArrayPos; /* the position in the byte Array */
  Std_ReturnType RetVal = E_OK;

  if (0U != ArrayLength)
  {
    if(NumberLength < (CRYPTO_XVIX_XAIX_LN_NUMBER_LEN(ArrayLength) + 1U))
    {
      RetVal = E_NOT_OK;
    }
    else
    {
      /* We cycle through all of the words of the resulting Number. */
      for
      (
        NumberPos = (Crypto_xVIx_xAIx_LNWordType)1U;
        NumberPos <= CRYPTO_XVIX_XAIX_LN_NUMBER_LEN(ArrayLength);
        NumberPos++
      )
      {
        /* The current word of Number is initialized to '0'. */
        Number[NumberPos] = (Crypto_xVIx_xAIx_LNWordType)0U;

        /* "CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES" bytes of the Array are combined to make
           up the current word of the Number. */
        for
        (
          WordPos = (Crypto_xVIx_xAIx_LNWordType)0U;
          WordPos < (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES;
          WordPos++
        )
        {
          ArrayPos = (uint16)CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES;
          ArrayPos *= (uint16)(NumberPos - (Crypto_xVIx_xAIx_LNWordType)1U);
          ArrayPos += (uint16)WordPos;

          if(ArrayPos < ArrayLength)
          {
            /* If we are not over the edge of 'Array', we incorporate the
               current byte of 'Array' into the current word of the Number. */
            Number[NumberPos] |=
              (Crypto_xVIx_xAIx_LNWordType)
              (
                (Crypto_xVIx_xAIx_LNWordType)Array[ArrayPos] <<
                  (Crypto_xVIx_xAIx_LNWordType)((Crypto_xVIx_xAIx_LNWordType)WordPos << 3U)
              );
          }
        }
      }

      Number[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] = CRYPTO_XVIX_XAIX_LN_NUMBER_LEN(ArrayLength);

      Crypto_xVIx_xAIx_LNNormalizeNumber(Number);
    }
  }
  else
  {
    if (NumberLength == 0U)
    {
      RetVal = E_NOT_OK;
    }
    else
    {
      /* The Length of 'Array' is '0U', so the resulting Number is also '0U'. */
      Number[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] = (Crypto_xVIx_xAIx_LNWordType)0U;
    }
  }
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_0952, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNNumberToLEByteArray
(
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Number,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                         Array,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                        ArrayLengthPtr
)
{
  Crypto_xVIx_xAIx_LNWordType NumberPos; /* the position in 'Number' */
  Crypto_xVIx_xAIx_LNWordType WordPos; /* byte position in the current word of 'Number' */
  Crypto_xVIx_xAIx_LNWordType ArrayPos; /* the position in the byte Array */
  Std_ReturnType RetVal = E_OK;

  if(
      ((*ArrayLengthPtr) / CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES) <
        Number[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]
    )
  {
    RetVal = E_NOT_OK;
  }
  else
  {
    /* we cycle through all the words of 'Number'. */
    for(
         NumberPos = (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LN_LSW_POS;
         NumberPos <= Number[CRYPTO_XVIX_XAIX_LN_LENGTH_POS];
         NumberPos++
       )
    {
      /* Begin computing the index of the byte which will be updated. */
      ArrayPos = NumberPos;
      ArrayPos -= (Crypto_xVIx_xAIx_LNWordType)1U;
      ArrayPos *= (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES;

      /* Each word of 'Number' is split into 'CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES' bytes
         of 'Array'. */
      for
      (
        WordPos = (Crypto_xVIx_xAIx_LNWordType)0U;
        WordPos < (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES;
        WordPos++
      )
      {
        /* The current byte of 'Array' is computed from the current word of
           'Number'. */
        Array[ArrayPos] =
          (uint8)(
                   Number[NumberPos] >>
                     (Crypto_xVIx_xAIx_LNWordType)((Crypto_xVIx_xAIx_LNWordType)WordPos << 3U)
                 );

        /* Finish computing the index of the byte which will be updated. */
        ArrayPos += (Crypto_xVIx_xAIx_LNWordType)1U;
      }
    }

    /* Return the correct Length of the byte array. */
    *ArrayLengthPtr =
      (uint16)(Number[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] *
      (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LN_WORD_LEN_BYTES);
  }
  return RetVal;
}

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNDivByTwo
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
  Crypto_xVIx_xAIx_LNWordType                                               Overflow
)
{
  Crypto_xVIx_xAIx_LNWordType i;
  Crypto_xVIx_xAIx_LNWordType DiffTmpVal1;
  Crypto_xVIx_xAIx_LNWordType DiffTmpVal2;

  DiffTmpVal2 = Overflow;

  for(i = Value[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]; i > 0U; i--)
  {
    DiffTmpVal1 = Value[i] & 1U;
    Value[i] = Value[i] >> 1U;
    Value[i] |= (DiffTmpVal2 << (CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS - 1U));
    DiffTmpVal2 = DiffTmpVal1;
  }
  Crypto_xVIx_xAIx_LNNormalizeNumber(Value);
}

/* !LINKSTO EB_Crypto_CryptAlgos_0957, 1 */
FUNC(uint8, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNAddition
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Sum,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend1,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend2
)
{
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) LargestOperand;
#if (CRYPTO_XVIX_XAIX_LN_PLATFORM_DOUBLE_WORD_SUPPORT == STD_OFF)
  Crypto_xVIx_xAIx_LNWordType LoopCount;
  Crypto_xVIx_xAIx_LNWordType MinLength;
  Crypto_xVIx_xAIx_LNWordType MaxLength;
  Crypto_xVIx_xAIx_LNWordType Carry = 0U;

  /* Find the larger of the two operands */
  if (Addend1[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] > Addend2[CRYPTO_XVIX_XAIX_LN_LENGTH_POS])
  {
    MinLength = Addend2[CRYPTO_XVIX_XAIX_LN_LENGTH_POS];
    MaxLength = Addend1[CRYPTO_XVIX_XAIX_LN_LENGTH_POS];
    LargestOperand = Addend1;
  }
  else
  {
    MinLength = Addend1[CRYPTO_XVIX_XAIX_LN_LENGTH_POS];
    MaxLength = Addend2[CRYPTO_XVIX_XAIX_LN_LENGTH_POS];
    LargestOperand = Addend2;
  }

  /* Calculate the sums up to the maximum available number of common digits */
  for (LoopCount = CRYPTO_XVIX_XAIX_LN_LSW_POS; LoopCount <= MinLength; LoopCount++)
  {
    Crypto_xVIx_xAIx_LNWordType LowerSum;
    Crypto_xVIx_xAIx_LNWordType UpperSum;

    /* Calculate the lower sum */
    LowerSum = (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LNLOWHALFWORD(Addend1[LoopCount]) +
      (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LNLOWHALFWORD(Addend2[LoopCount]) +
      (Crypto_xVIx_xAIx_LNWordType)Carry;

    /* Extract the carry bit */
    Carry = LowerSum >> CRYPTO_XVIX_XAIX_LN_HALFWORD_LEN_BITS;

    /* The lower sum is equal to the lower half of the difference */
    LowerSum &= CRYPTO_XVIX_XAIX_LN_HALFWORD_MAX;

    /* Calculate the upper sum */
    UpperSum = (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LNHIGHHALFWORD(Addend1[LoopCount]) +
      (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LNHIGHHALFWORD(Addend2[LoopCount]) +
      (Crypto_xVIx_xAIx_LNWordType)Carry;

    /* Extract the carry bit */
    Carry = UpperSum >> CRYPTO_XVIX_XAIX_LN_HALFWORD_LEN_BITS;

    /* The upper sum is equal to the lower half of the difference */
    UpperSum &= CRYPTO_XVIX_XAIX_LN_HALFWORD_MAX;

    /* Concatenate the partial sums */
    Sum[LoopCount] = ((UpperSum << CRYPTO_XVIX_XAIX_LN_HALFWORD_LEN_BITS) | LowerSum);
  }

  /* Copy the remaining digits */
  for (; LoopCount <= MaxLength; LoopCount++)
  {
    Crypto_xVIx_xAIx_LNWordType LowerSum;
    Crypto_xVIx_xAIx_LNWordType UpperSum;

    /* Calculate the lower sum */
    LowerSum =
      (Crypto_xVIx_xAIx_LNWordType)
      CRYPTO_XVIX_XAIX_LNLOWHALFWORD(LargestOperand[LoopCount]) +
      (Crypto_xVIx_xAIx_LNWordType)Carry;

    /* Extract the carry bit */
    Carry = LowerSum >> CRYPTO_XVIX_XAIX_LN_HALFWORD_LEN_BITS;

    /* The lower sum is equal to the lower half of the difference */
    LowerSum &= CRYPTO_XVIX_XAIX_LN_HALFWORD_MAX;

    /* Calculate the upper sum */
    UpperSum =
      (Crypto_xVIx_xAIx_LNWordType)
      CRYPTO_XVIX_XAIX_LNHIGHHALFWORD(LargestOperand[LoopCount]) +
      (Crypto_xVIx_xAIx_LNWordType)Carry;

    /* Extract the carry bit */
    Carry = UpperSum >> CRYPTO_XVIX_XAIX_LN_HALFWORD_LEN_BITS;

    /* The upper sum is equal to the lower half of the difference */
    UpperSum &= CRYPTO_XVIX_XAIX_LN_HALFWORD_MAX;

    /* Concatenate the partial sums */
    Sum[LoopCount] = (UpperSum << CRYPTO_XVIX_XAIX_LN_HALFWORD_LEN_BITS) | LowerSum;
  }

  Sum[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] = MaxLength;
#else /* (CRYPTO_XVIX_XAIX_LN_PLATFORM_DOUBLE_WORD_SUPPORT == STD_ON) */

  Crypto_xVIx_xAIx_LNDoubleWordType TempSum;
  Crypto_xVIx_xAIx_LNWordType LoopCount, MinLength, MaxLength, Carry = 0U;

  /* Find the larger of the two operands */
  if (Addend1[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] > Addend2[CRYPTO_XVIX_XAIX_LN_LENGTH_POS])
  {
    MinLength = Addend2[CRYPTO_XVIX_XAIX_LN_LENGTH_POS];
    MaxLength = Addend1[CRYPTO_XVIX_XAIX_LN_LENGTH_POS];
    LargestOperand = Addend1;
  }
  else
  {
    MinLength = Addend1[CRYPTO_XVIX_XAIX_LN_LENGTH_POS];
    MaxLength = Addend2[CRYPTO_XVIX_XAIX_LN_LENGTH_POS];
    LargestOperand = Addend2;
  }

  /* Calculate the sums up to the maximum available number of common digits */
  for (LoopCount = CRYPTO_XVIX_XAIX_LN_LSW_POS; LoopCount <= MinLength; LoopCount++)
  {
    /* Calculate the sum */
    TempSum = (Crypto_xVIx_xAIx_LNDoubleWordType)Addend1[LoopCount] +
      (Crypto_xVIx_xAIx_LNDoubleWordType)Addend2[LoopCount] +
      (Crypto_xVIx_xAIx_LNDoubleWordType)Carry;

    /* The result of the column is equal to the lower part of the difference */
    Sum[LoopCount] = (Crypto_xVIx_xAIx_LNWordType)TempSum;

    /* Extract the carry bit */
    Carry = (Crypto_xVIx_xAIx_LNWordType)(TempSum >> CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS);
  }

  /* Copy the remaining digits */
  for (; LoopCount <= MaxLength; LoopCount++)
  {
    /* Calculate the sum */
    TempSum = (Crypto_xVIx_xAIx_LNDoubleWordType)LargestOperand[LoopCount] +
      (Crypto_xVIx_xAIx_LNDoubleWordType)Carry;

    /* The result of the column is equal to the lower part of the difference */
    Sum[LoopCount] = (Crypto_xVIx_xAIx_LNWordType)TempSum;

    /* Extract the carry bit */
    Carry = (Crypto_xVIx_xAIx_LNWordType)(TempSum >> CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS);

  }

  Sum[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] = MaxLength;

#endif /* (CRYPTO_XVIX_XAIX_LN_PLATFORM_DOUBLE_WORD_SUPPORT == STD_ON) */
  return (uint8)Carry;
}

/* !LINKSTO EB_Crypto_CryptAlgos_0956, 1 */
FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNAdditionModular
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Sum,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend1,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend2,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Modulus
)
{
  Crypto_xVIx_xAIx_LNWordType i;
  Crypto_xVIx_xAIx_LNWordType Tmp;
  uint8 Carry = 0U;
  uint8 Borrow = 0U;

  Carry = Crypto_xVIx_xAIx_LNAddition(Sum, Addend1, Addend2);

  for (
        i = (Sum[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] + 1U);
        i <= Modulus[CRYPTO_XVIX_XAIX_LN_LENGTH_POS];
        i++
      )
  {
    Sum[i] = 0U;
  }

  /* check if there is a Carry after the last addition */

  /* if the Carry is 1 and the sum is smaller then the modulus, the Carry can
     be added in the highest position. */
  if (
       (Sum[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] < Modulus[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]) &&
       (1U == Carry)
     )
  {
    Sum[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]++;
    Sum[Sum[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]] = 1U;
  }
  /* else the Carry is set to one. Then the sum Length equals the modulus
     Length and it is necessary to subtract. Also, if the Length of the sum
     equals the modulus and the most significant position is larger than the
     modulus a subtraction is needed. */
  else if(
           (CRYPTO_XVIX_XAIX_E_SMALLER != Crypto_xVIx_xAIx_LNCompareNumbers(Sum, Modulus)) ||
           (1U == Carry)
         )
  {
    for (i = CRYPTO_XVIX_XAIX_LN_LSW_POS; i <= Modulus[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]; i++)
    {
      Tmp = Sum[i];
      Sum[i] = (Sum[i] - Modulus[i]) - Borrow;

      if (Sum[i] >= Tmp)
      {
        if ((0U != Modulus[i]) || ( 0U != Borrow))
        {
          Borrow = 1U;
        }
      }
      else
      {
        Borrow = 0U;
      }
    }
  }
  else
  {
    /* Caution: this 'else' is required by MISRA-C:2012 */
  }
  Crypto_xVIx_xAIx_LNNormalizeNumber(Sum);
}

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNSubtractionModular
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Difference,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Minuend,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Subtrahend,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Modulus
)
{
  Crypto_xVIx_xAIx_LNWordType i;
  uint8 Borrow = 0U;
  Crypto_xVIx_xAIx_LNWordType Tmp;
  Crypto_xVIx_xAIx_LNWordType Tmp2;

  for (i = CRYPTO_XVIX_XAIX_LN_LSW_POS; i <= Modulus[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]; i++)
  {
    if(i > Minuend[CRYPTO_XVIX_XAIX_LN_LENGTH_POS])
    {
      Tmp = 0U;
    }
    else
    {
      Tmp = Minuend[i];
    }
    if(i > Subtrahend[CRYPTO_XVIX_XAIX_LN_LENGTH_POS])
    {
      Tmp2 = 0U;
    }
    else
    {
      Tmp2 = Subtrahend[i];
    }
    Difference[i] = (Tmp - Tmp2) - Borrow;

    if (Difference[i] >= Tmp)
    {
      if ((0U != Tmp2) || ( 0U != Borrow))
      {
        Borrow = 1U;
      }
    }
    else
    {
      Borrow = 0U;
    }
  }

  Difference[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] = Modulus[CRYPTO_XVIX_XAIX_LN_LENGTH_POS];

  /* if the number is 'negative', which equals a remaining Borrow, the modulus
     must be added. */
  if (1U == Borrow)
  {
    do
    {
      /* Add modulus until a carry is returned, which signals a positive value */
      Borrow = Crypto_xVIx_xAIx_LNAddition(Difference, Difference, Modulus);
    } while (0U == Borrow);
  }

  Crypto_xVIx_xAIx_LNNormalizeNumber(Difference);
}

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNInversion
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   InvValue,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp1Value,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp2Value,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Tmp3Value,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Modulus,
  P2VAR(Crypto_xVIx_xAIx_LNCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    CtxPtr
)
{
  Crypto_xVIx_xAIx_LNWordType AddResult, i;
  Crypto_xVIx_xAIx_ReturnType CompNumRes;
  TS_PARAM_UNUSED(CtxPtr);

  /* Copy the input */
  /* u = a */
  for (i = 0U; i <= Value[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]; i++)
  {
    Tmp1Value[i] = Value[i];
  }
  /* v = p */
  for (i = 0U; i <= Modulus[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]; i++)
  {
    Tmp2Value[i] = Modulus[i];
  }

  /* x1 = 1 */
  Tmp3Value[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] = 1U;
  Tmp3Value[CRYPTO_XVIX_XAIX_LN_LSW_POS] = 1U;

  /* x2 = 0 */
  InvValue[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] = 0U;
  InvValue[CRYPTO_XVIX_XAIX_LN_LSW_POS] = 0U;

  /* while (u != 1 and v != 1) */
  while (
          (
            !(
               (1U == Tmp1Value[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]) &&
               (1U == Tmp1Value[CRYPTO_XVIX_XAIX_LN_LSW_POS])
             ) && !(
                     (1U == Tmp2Value[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]) &&
                     (1U == Tmp2Value[CRYPTO_XVIX_XAIX_LN_LSW_POS])
                   )
          ) && (0U != Tmp1Value[CRYPTO_XVIX_XAIX_LN_LENGTH_POS])
        )
  {
    /* while u is even */
    while (0U == (Tmp1Value[CRYPTO_XVIX_XAIX_LN_LSW_POS] & 1U))
    {
      /* u = u/2 */
      Crypto_xVIx_xAIx_LNDivByTwo(Tmp1Value, 0U);

      /* if x1 is even */
      if (0U == (Tmp3Value[CRYPTO_XVIX_XAIX_LN_LSW_POS] & 1U))
      {
        /* x1 = x1 / 2 */
        Crypto_xVIx_xAIx_LNDivByTwo(Tmp3Value, 0U);
      }
      else
      {
        /* x1 = (x1 + p) / 2 */
        AddResult = Crypto_xVIx_xAIx_LNAddition(Tmp3Value, Tmp3Value, Modulus);
        Crypto_xVIx_xAIx_LNDivByTwo(Tmp3Value, AddResult);
      }
    }

    /* while v is even */
    while (0U == (Tmp2Value[CRYPTO_XVIX_XAIX_LN_LSW_POS] & 1U))
    {
      /* v = v / 2 */
      Crypto_xVIx_xAIx_LNDivByTwo(Tmp2Value, 0U);

      /* if x2 is even */
      if (0U == (InvValue[CRYPTO_XVIX_XAIX_LN_LSW_POS] & 1U))
      {
        /* x2 = x2 / 2 */
        Crypto_xVIx_xAIx_LNDivByTwo(InvValue, 0U);
      }
      else
      {
        /* x2 = (x2 + p) / 2 */
        AddResult = Crypto_xVIx_xAIx_LNAddition(InvValue, InvValue, Modulus);
        Crypto_xVIx_xAIx_LNDivByTwo(InvValue, AddResult);
      }
    }

    /* if (u >= v) */
    CompNumRes = Crypto_xVIx_xAIx_LNCompareNumbers(Tmp1Value, Tmp2Value);
    if
    (
      (CRYPTO_XVIX_XAIX_E_LARGER == CompNumRes) ||
      (CRYPTO_XVIX_XAIX_E_EQUAL == CompNumRes)
    )
    {
      /* u = u - v */
      Crypto_xVIx_xAIx_LNSubtractionModular(Tmp1Value, Tmp1Value, Tmp2Value, Modulus);
      /* x1 = x1 - x2 */
      Crypto_xVIx_xAIx_LNSubtractionModular(Tmp3Value, Tmp3Value, InvValue, Modulus);
    }
    else
    {
      /* v = v - u */
      Crypto_xVIx_xAIx_LNSubtractionModular(Tmp2Value, Tmp2Value, Tmp1Value, Modulus);
      /* x2 = x2 - x1 */
      Crypto_xVIx_xAIx_LNSubtractionModular(InvValue, InvValue, Tmp3Value, Modulus);
    }
  }

  /* if (u == 1) */
  if (
       (1U == Tmp1Value[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]) &&
       (1U == Tmp1Value[CRYPTO_XVIX_XAIX_LN_LSW_POS])
     )
  {
    /* return (x1 mod p) */
    for (i = 0U; i <= Tmp3Value[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]; i++)
    {
      InvValue[i] = Tmp3Value[i];
    }
  }

  Crypto_xVIx_xAIx_LNModulo(InvValue, Modulus, CtxPtr);
}

#if                                                     \
(                                                       \
  (CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_ENABLED == STD_ON) || \
  (CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_ENABLED == STD_ON) || \
  (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED   == STD_ON)  || \
  (CRYPTO_XVIX_XAIX_RSAPSS_VRFY_ENABLED  == STD_ON)  || \
  (CRYPTO_XVIX_XAIX_RSAPKCS_VRFY_ENABLED == STD_ON)  || \
  (CRYPTO_XVIX_XAIX_RSAPKCS_GEN_ENABLED == STD_ON)      \
)
FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNModExp
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Result,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Base,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Modulus,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Exponent,
  P2VAR(Crypto_xVIx_xAIx_LNCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    CtxPtr
)
{
  uint32 Bits = Crypto_xVIx_xAIx_LNBitLengthOfNumber(Exponent);
  Crypto_xVIx_xAIx_LNWordType Rho;
  Crypto_xVIx_xAIx_LNWordType i;

  /* reset ctx variable */
  TS_MemBZero(Crypto_xVIx_xAIx_ModExpLocalBase, sizeof(Crypto_xVIx_xAIx_ModExpLocalBase));
  for (i = 0U; i <= Base[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]; i++)
  {
    Crypto_xVIx_xAIx_ModExpLocalBase[i] = Base[i];
  }
  TS_PARAM_UNUSED(CtxPtr);

  /* Calculate Rho */
  Crypto_xVIx_xAIx_LNMontgomerySetup(Modulus, &Rho);

  /* Calculate R mod m */
  Crypto_xVIx_xAIx_LNMontgomeryNormalization(Result, Modulus);

  /* G > P so we reduce it first */
  if (
       Crypto_xVIx_xAIx_LNCompareNumbers(Modulus, Crypto_xVIx_xAIx_ModExpLocalBase) !=
         CRYPTO_XVIX_XAIX_E_LARGER
     )
  {
    Crypto_xVIx_xAIx_LNModulo(Crypto_xVIx_xAIx_ModExpLocalBase, Modulus, CtxPtr);
  }

  /* x * (R mod P) */
  Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                      Crypto_xVIx_xAIx_ModExpLocalBase,
                                      Result,
                                      Crypto_xVIx_xAIx_ModExpLocalBase,
                                      CtxPtr
                                    );
  Crypto_xVIx_xAIx_LNModulo(Crypto_xVIx_xAIx_ModExpLocalBase, Modulus, CtxPtr);

  while ((Bits) > 0U)
  {
    --(Bits);

    /* Result = Result**2 mod Modulus  */
#if (CRYPTO_XVIX_XAIX_LN_PLATFORM_DOUBLE_WORD_SUPPORT == STD_ON)
    Crypto_xVIx_xAIx_LNSquareComba(Result, Result);
#else
    Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                        Result,
                                        Result,
                                        Result,
                                        CtxPtr
                                      );
#endif
    Crypto_xVIx_xAIx_LNMontgomeryReduction(Result, Result, Modulus, Rho);

    if ((Crypto_xVIx_xAIx_LNWordType)1U == CRYPTO_XVIX_XAIX_LNGETBIT(Exponent, Bits))
    {
      Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                          Result,
                                          Result,
                                          Crypto_xVIx_xAIx_ModExpLocalBase,
                                          CtxPtr
                                        );
      Crypto_xVIx_xAIx_LNMontgomeryReduction(Result, Result, Modulus, Rho);
    }
  }

  /* Apply Trans**(-1) to return from Montgomery system */
  Crypto_xVIx_xAIx_LNMontgomeryReduction(Result, Result, Modulus, Rho);
}
#endif

#if (STD_ON == CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT)
/* !LINKSTO EB_Crypto_00057,1, EB_Crypto_01021,1, Crypto.Req.Gen/NvM/01019, 1 */
FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_Init
(
  void
)
{
#if (CRYPTO_XVIX_XAIX_ENABLED_NVM == STD_ON)
  NvM_RequestResultType ReadAllResult = NVM_REQ_NOT_OK;
  Std_ReturnType GetErrorStatusReturn = E_NOT_OK;
#endif /* #if (CRYPTO_XVIX_XAIX_ENABLED_NVM == STD_ON) */

  uint32 CryptoKeyId = 0U;
  uint32 KeyElementIndex = 0U;
  uint32 KeyElementId = 0U;
#if (CRYPTO_XVIX_XAIX_ENABLED_NVM == STD_ON)
  boolean InitKeyElem = TRUE;
  boolean KeySetValid = TRUE;
#endif

#if (1U < CRYPTO_XVIX_XAIX_KEY_COUNT)
  for (CryptoKeyId = 0U; CryptoKeyId < CRYPTO_XVIX_XAIX_KEY_COUNT; CryptoKeyId++)
#endif /* #if (1U < CRYPTO_XVIX_XAIX_KEY_COUNT) */
  {
    for (
          KeyElementIndex = 0U;
          KeyElementIndex < Crypto_xVIx_xAIx_AL_Keys_Data[CryptoKeyId].NumKeyData;
          KeyElementIndex++
        )
    {
#if (CRYPTO_XVIX_XAIX_ENABLED_NVM == STD_ON)
      /* if NVram block ID is different than 0 that means the key data is persistent */
      if (
           0U !=
           Crypto_xVIx_xAIx_AL_Keys_Data[CryptoKeyId].KeyData[
                                                               KeyElementIndex
                                                             ].RfKeyElemDataNvramBlockId
         )
      {
        /* !LINKSTO Crypto.Req.Gen/NvM/01010,1 */
        /* !LINKSTO Crypto.Req.Gen/NvM/01012,1 */
        GetErrorStatusReturn =
        NvM_GetErrorStatus
        (
          Crypto_xVIx_xAIx_AL_Keys_Data[
                                        CryptoKeyId
                                       ].KeyData[KeyElementIndex].RfKeyElemDataNvramBlockId,
          &ReadAllResult
        );
        if (
             E_OK == GetErrorStatusReturn &&
             (NVM_REQ_OK == ReadAllResult || NVM_REQ_RESTORED_FROM_ROM == ReadAllResult)
           )
        {
          InitKeyElem = FALSE; /* already read from NvM or initialized by NvM */
        }
        else /* empty block or a problem occured during NvMRealAll() or NvM_GetErrorStatus() */
        {
          KeySetValid = FALSE;
        }
      }
      if (TRUE == InitKeyElem)  /* init: non-persistent, empty, or erroneous blocks */
#endif /* #if (CRYPTO_XVIX_XAIX_ENABLED_NVM == STD_ON) */
      {
        KeyElementId = Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyType[KeyElementIndex]->Id;
        /* !LINKSTO Crypto.Req.Gen/NvM/01011,1 */
        /* !LINKSTO Crypto.Req.Gen/NvM/01013,1 */
        /* !LINKSTO Crypto.Req.Gen/NvM/01016,1 */
        /* This call can never fail, because all the passed parameters are known to be valid.*/
        (void)Crypto_xVIx_xAIx_AL_KeyElementSet
              (
                CryptoKeyId,
                KeyElementId,
                Crypto_xVIx_xAIx_AL_Keys_Data[
                                               CryptoKeyId
                                             ].KeyData[KeyElementIndex].RfKeyElemDataInit,
                Crypto_xVIx_xAIx_AL_Keys_Data[
                                               CryptoKeyId
                                             ].KeyData[KeyElementIndex].RfKeyElemDataSize
              );
      }
#if (CRYPTO_XVIX_XAIX_ENABLED_NVM == STD_ON)
      else
      {
        InitKeyElem = TRUE; /* reset var */
      }
#endif /* #if (CRYPTO_XVIX_XAIX_ENABLED_NVM == STD_ON) */
    } /* end: for all key elements */

#if ((CRYPTO_XVIX_XAIX_CMAC_GEN_ENABLED  == STD_ON) || \
     (CRYPTO_XVIX_XAIX_CMAC_VRFY_ENABLED == STD_ON)    \
    )
    /* !LINKSTO EB_Crypto_CryptAlgos_0502,1 */
    Crypto_xVIx_xAIx_AL_AESCMAC_PrecalculateKeys(CryptoKeyId);
#endif
#if (CRYPTO_XVIX_XAIX_ENABLED_NVM == STD_ON)
    if (TRUE == KeySetValid)
#endif /* #if (CRYPTO_XVIX_XAIX_ENABLED_NVM == STD_ON) */
    {
      /* !LINKSTO Crypto.Req.Gen/NvM/01014,1 */
      Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyState = CRYPTO_XVIX_XAIX_KEY_STATE_VALID;
    }
#if (CRYPTO_XVIX_XAIX_ENABLED_NVM == STD_ON)
    else
    {
      /* !LINKSTO Crypto.Req.Gen/NvM/01015,1 */
      Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyState = CRYPTO_XVIX_XAIX_KEY_STATE_INVALID;
#if (1U < CRYPTO_XVIX_XAIX_KEY_COUNT)
      KeySetValid = TRUE; /* reset flag var */
#endif /* #if (1U < CRYPTO_XVIX_XAIX_KEY_COUNT) */
    }
#endif /* #if (CRYPTO_XVIX_XAIX_ENABLED_NVM == STD_ON) */
  } /* end: for all keys */
}
#endif /* #if (STD_ON == CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT) */

#if (CRYPTO_XVIX_XAIX_RSAPKCS_VRFY_ENABLED == STD_ON) || \
    (CRYPTO_XVIX_XAIX_RSAPKCS_GEN_ENABLED == STD_ON)
FUNC(uint32, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CommonDerHeaderSequence
(
   P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) HeaderBuffer,
   uint32                                              PayloadLength
)
{
   uint32 HeaderLength;

   HeaderBuffer[0] = 0x30;

   HeaderLength = Crypto_xVIx_xAIx_CommonDerLengthEncoding(&HeaderBuffer[1], PayloadLength);

   return HeaderLength + 1;
}
FUNC(uint32, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CommonDerHeaderOctetString
(
   P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) HeaderBuffer,
   uint32                                              PayloadLength
)
{
   uint32 HeaderLength;

   HeaderBuffer[0] = 0x04;

   HeaderLength = Crypto_xVIx_xAIx_CommonDerLengthEncoding(&HeaderBuffer[1], PayloadLength);

   return HeaderLength + 1;
}
#endif /* #if (CRYPTO_XVIX_XAIX_RSAPKCS_VRFY_ENABLED == STD_ON) ||
              (CRYPTO_XVIX_XAIX_RSAPKCS_GEN_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_RSAPKCS_VRFY_ENABLED == STD_ON)  || \
    (CRYPTO_XVIX_XAIX_RSAPKCS_GEN_ENABLED  == STD_ON)  || \
    (CRYPTO_XVIX_XAIX_RSAPSS_VRFY_ENABLED  == STD_ON)  || \
    (CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_ENABLED == STD_ON) || \
    (CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_ENABLED == STD_ON)

FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DecodeRsaKey
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                     Key,
  uint32                                                                    KeyLength,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Modulus,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Exponent

)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint32 CurrentPosition = 0U;
  uint32 RemainingSequenceLength = 0U;
  uint32 ReadBytes = 0U;

  /* we check at least the first 4 Byte including the first tag and three Byte length */
  if(4U <= KeyLength)
  {
    if(CRYPTO_XVIX_XAIX_AL_DER_SEQUENCE_TAG == Key[CurrentPosition])
    {
      CurrentPosition++;
      ReadBytes = Crypto_xVIx_xAIx_AL_GetDERLength(&Key[CurrentPosition], &RemainingSequenceLength);
      /* ReadBytes can not be 0 */
      if(0U != ReadBytes)
      {
        CurrentPosition += ReadBytes;
        /* The complete KeyLength has to match the specified range + the already parsed Bytes */
        if(KeyLength >= RemainingSequenceLength + CurrentPosition)
        {
          ReadBytes = Crypto_xVIx_xAIx_AL_GetDERInteger(
                                                         &Key[CurrentPosition],
                                                         Modulus
                                                       );
          /* ReadBytes can not be 0 */
          if(0U != ReadBytes)
          {
            CurrentPosition += ReadBytes;
            ReadBytes = Crypto_xVIx_xAIx_AL_GetDERInteger(
                                                           &Key[CurrentPosition],
                                                           Exponent
                                                         );
            /* ReadBytes can not be 0 */
            if(0U != ReadBytes)
            {
              RetVal = E_OK;
            }
          }
        }
      }
    }
  }
  return RetVal;
}
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
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_GetECIndex
(
          uint8                                         ServiceInfo,
          uint8                                         AlgorithmFamily,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CurveDER,
          uint8                                         CurveDERLength,
    P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CurveIndex
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint8 i;

  if(NULL_PTR == CurveDER)
  {
    /* Signature generation / Signature verification */
    if((CRYPTO_SIGNATUREGENERATE == ServiceInfo) || (CRYPTO_SIGNATUREVERIFY == ServiceInfo))
    {
      /* Use Ed25519ph as default for Signature generation or verification */
      if(CRYPTO_ALGOFAM_ED25519 == AlgorithmFamily)
      {
        *CurveIndex = CRYPTO_XVIX_XAIX_SIGNATURE_ED25519_DEFAULT_CURVE;
        RetVal = E_OK;
      }
      /* Use SecP256r1 / NIST P-256 / Prime256v1 as default for Signature generation or verification */
      else
      if(CRYPTO_ALGOFAM_ECCNIST == AlgorithmFamily)
      {
        *CurveIndex = CRYPTO_XVIX_XAIX_SIGNATURE_ECCNIST_DEFAULT_CURVE;
        RetVal = E_OK;
      }
      /* Already we do not support any Brainpool Curves */
      else
      {
        RetVal = E_NOT_OK;
      }
    }
    /* Key Exchange */
    /* Use X25519 as default for key exchange */
    else
    if(
        (CRYPTO_SIGNATUREGENERATE != ServiceInfo    ) &&
        (CRYPTO_SIGNATUREVERIFY   != ServiceInfo    ) &&
        (CRYPTO_ALGOFAM_ED25519   == AlgorithmFamily)
      )
      {
        *CurveIndex = CRYPTO_XVIX_XAIX_KEYEXCHANGE_X25519_DEFAULT_CURVE;
        RetVal = E_OK;
      }
    /* Use SecP256r1 / NIST P-256 / Prime256v1 as default for key exchange */
    else
    if(
        (CRYPTO_SIGNATUREGENERATE != ServiceInfo    ) &&
        (CRYPTO_SIGNATUREVERIFY   != ServiceInfo    ) &&
        (CRYPTO_ALGOFAM_ECCNIST   == AlgorithmFamily)
      )
      {
        *CurveIndex = CRYPTO_XVIX_XAIX_KEYEXCHANGE_ECCNIST_DEFAULT_CURVE;
        RetVal = E_OK;
      }
    else
    {
      /* Unsupported Algorithm */
      RetVal = E_NOT_OK;
    }
  }
  else  /* NULL_PTR != CurveDER */
  {
    /* The first element of the DER-Encoding contains 0x06.
     * The second element contains the actual length of the DER-Encoding.
     */
    if(0x06U != CurveDER[0U])
    {
      RetVal = E_NOT_OK;
    }
    else
    if(CurveDER[1U] + 2U > CurveDERLength)
    {
      RetVal = E_NOT_OK;
    }
    else
    {
      for(i = 0; i < CRYPTO_XVIX_XAIX_SUPPORTED_EC_NUM; i++)
      {
        /* Check the length of DER-Encoding and the family type */
        if(
            (Crypto_xVIx_xAIx_EllipticCurveInfo[i].ECDER.Length == CurveDER[1U]) &&
            (Crypto_xVIx_xAIx_EllipticCurveInfo[i].AlgoFam == AlgorithmFamily  )
          )
          {
            /* Check the Value of the DER-Encoding from key Element */
            if(E_OK == Crypto_xVIx_xAIx_MemCmp(
                                                Crypto_xVIx_xAIx_EllipticCurveInfo[i].ECDER.Value,
                                                &(CurveDER[2U]),
                                                Crypto_xVIx_xAIx_EllipticCurveInfo[i].ECDER.Length
                                              )
              )
              {
                *CurveIndex = i;
                RetVal = E_OK;
              }
          }
      }
    }
  }
  return RetVal;
}
#endif /* #if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
              (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)   || \
              (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON)  || \
              (CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED == STD_ON)   || \
              (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON) */

/*----- Ed25519 /X25519 --------------------------------------------------------------------------*/
#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
    (CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED == STD_ON)   || \
    (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON)
FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAFastReduction
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  Crypto_xVIx_xAIx_LNWordType Tmp[CRYPTO_XVIX_XAIX_ECDHX25519_TEMP_LEN_WORDS];

#if (CPU_TYPE_32 == CPU_TYPE)

  Crypto_xVIx_xAIx_LNWordType Temp1;

  /* set the higher positions of the Value to zero if necessary */
  for (Temp1 = (Value[0U] + 1U); Temp1 < CRYPTO_XVIX_XAIX_ECDHX25519_TEMP_LEN_WORDS; Temp1++)
  {
    Value[Temp1] = 0U;
  }

  Crypto_xVIx_xAIx_EdDSAFastReductionCompA(Tmp, Value);

  /* Value = A1 + A2 */
  Crypto_xVIx_xAIx_LNAdditionModular(Value, Tmp, Value, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);

  /* Value = Value + A1 = 2*A1 + A2 */
  Crypto_xVIx_xAIx_LNAdditionModular(Value, Value, Tmp, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);

  /* Value = Value + A1 = 3*A1 + A2 */
  Crypto_xVIx_xAIx_LNAdditionModular(Value, Value, Tmp, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);

  Crypto_xVIx_xAIx_EdDSAFastReductionCompB(&Temp1, Tmp);

  /* Value = Value + B2 = 3.A1 + A2 + B2 */
  Crypto_xVIx_xAIx_LNAdditionModular(Value, Value, Tmp, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);

  /* Tmp = B1 */
  Tmp[1U] = Temp1;
  Tmp[0U] = 1U;

  Crypto_xVIx_xAIx_LNNormalizeNumber(Tmp);

  /* Value = 3*A1 + A2 + B2 + 3*B1 */
  Crypto_xVIx_xAIx_LNAdditionModular(Value, Value, Tmp, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);
  Crypto_xVIx_xAIx_LNAdditionModular(Value, Value, Tmp, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);
  Crypto_xVIx_xAIx_LNAdditionModular(Value, Value, Tmp, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);

  /* Tmp = B1*(2^4) */
  Tmp[1U] = (Crypto_xVIx_xAIx_LNWordType)(Tmp[1U] << 4U);
  Tmp[0U] = 1U;

  Crypto_xVIx_xAIx_LNNormalizeNumber(Tmp);

  /* Value = Value + Tmp = 3*A1 + A2 + B2 + 3*B1 + B1*(2^4) */
  Crypto_xVIx_xAIx_LNAdditionModular(Value, Value, Tmp, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);
#else

#error Unsupported CPU_TYPE

#endif
}
#endif /* #if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
              (CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED == STD_ON)   || \
              (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON) */

/*----- ECC NIST --------------------------------------------------------------------------------*/

/*----- Fast Reduction SecP256r1 / NIST P-256 ----------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
    (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)   || \
    (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON)
FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECP256FastReduction
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  Crypto_xVIx_xAIx_LNWordType Temp[CRYPTO_XVIX_XAIX_LN_NUMBER_LEN(CRYPTO_XVIX_XAIX_CURVE_SECP256R1_PRIVKEY_LEN_BYTES) + 1U];

#if (CPU_TYPE == CPU_TYPE_32)

  Crypto_xVIx_xAIx_LNWordType Buffer[CRYPTO_XVIX_XAIX_LN_NUMBER_LEN(CRYPTO_XVIX_XAIX_CURVE_SECP256R1_PRIVKEY_LEN_BYTES)];

  /* Set the higher positions of the value to zero if necessary */
  for (Buffer[0U] = (Value[0U] + 1U); Buffer[0U] < CRYPTO_XVIX_XAIX_MAX_TEMP_LEN_WORDS(CRYPTO_XVIX_XAIX_CURVE_SECP256R1_PRIVKEY_LEN_BYTES); Buffer[0U]++)
  {
    Value[Buffer[0U]] = 0U;
  }

  /* Calculate s1, s2 and s3 */
  Crypto_xVIx_xAIx_ECP256FastReductionCalcValue1(Value, Temp, Buffer, CtxPtr);

  /* Value = s1 + 2*s2 + s3 */
  Crypto_xVIx_xAIx_LNAdditionModular(Value, Value, Temp, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);

  /* Value = s1 + 2*s2 + 2*s3 */
  Crypto_xVIx_xAIx_LNAdditionModular(Value, Value, Temp, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);

  /* Calculate s4, s5 and s6 */
  Crypto_xVIx_xAIx_ECP256FastReductionCalcValue2(Value, Temp, Buffer, CtxPtr);

  /* Value = s1 + 2*s2 + 2*s3 + s4 + s5 - s6 */
  Crypto_xVIx_xAIx_LNSubtractionModular(Value, Value, Temp, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);

  /* Calculate s7, s8 and s9 */
  Crypto_xVIx_xAIx_ECP256FastReductionCalcValue3(Value, Temp, Buffer, CtxPtr);

  /* Value = s1 + 2*s2 + 2*s3 + s4 + s5 - s6 - s7  - s8 - s9 */
  Crypto_xVIx_xAIx_LNSubtractionModular(Value, Value, Temp, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);
#else

#error Unsupported CPU_TYPE

#endif
}
#endif /* #if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
              (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)   || \
              (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON) */

/*----- Fast Reduction SecP384r1 / NIST P-384 ----------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON)
FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECP384FastReduction
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  Crypto_xVIx_xAIx_LNWordType Temp[CRYPTO_XVIX_XAIX_LN_NUMBER_LEN(CRYPTO_XVIX_XAIX_CURVE_SECP384R1_PRIVKEY_LEN_BYTES) + 1U];

#if (CPU_TYPE == CPU_TYPE_32)

  Crypto_xVIx_xAIx_LNWordType Buffer[CRYPTO_XVIX_XAIX_LN_NUMBER_LEN(CRYPTO_XVIX_XAIX_CURVE_SECP384R1_PRIVKEY_LEN_BYTES)];

  /* Set the higher positions of the value to zero if necessary */
  for (Buffer[0U] = (Value[0U] + 1U); Buffer[0U] < CRYPTO_XVIX_XAIX_MAX_TEMP_LEN_WORDS(CRYPTO_XVIX_XAIX_CURVE_SECP384R1_PRIVKEY_LEN_BYTES); Buffer[0U]++)
  {
    Value[Buffer[0U]] = 0U;
  }

  /* Calculate s1, s2 and s3 */
  Crypto_xVIx_xAIx_ECP384FastReductionCalcValue1(Value, Temp, Buffer, CtxPtr);

  /* Value = s1 + 2*s2 + s3 */
  Crypto_xVIx_xAIx_LNAdditionModular(Value, Value, Temp, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);

  /* Calculate s4, s5 and s6 */
  Crypto_xVIx_xAIx_ECP384FastReductionCalcValue2(Value, Temp, Buffer, CtxPtr);

  /* Value = s1 + 2*s2 + s3 + s4 + s5 + s6 */
  Crypto_xVIx_xAIx_LNAdditionModular(Value, Value, Temp, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);

  /* Calculate s7 and s8 */
  Crypto_xVIx_xAIx_ECP384FastReductionCalcValue3(Value, Temp, Buffer, CtxPtr);

  /* Value = s1 + 2*s2 + s3 + s4 + s5 + s6 + s7 - s8 */
  Crypto_xVIx_xAIx_LNSubtractionModular(Value, Value, Temp, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);

  /* Calculate s9 and s10 */
  Crypto_xVIx_xAIx_ECP384FastReductionCalcValue4(Value, Temp, Buffer, CtxPtr);

  /* Value = s1 + 2*s2 + s3 + s4 + s5 + s6 + s7 - s8 - s9 - s10 */
  Crypto_xVIx_xAIx_LNSubtractionModular(Value, Value, Temp, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);
#else

#error Unsupported CPU_TYPE

#endif
}
#endif /* #if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
    (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)   || \
    (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON)
FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECPointConvertProj2Aff
(
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Point,
  P2VAR(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  /* Z = Z^(-1) */
  Crypto_xVIx_xAIx_LNInversion
           (
              Point->ZValue,
              Point->ZValue,
              &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1],
              &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_2],
              &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_3],
              Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime,
              &CtxPtr->LNCtx
           );

  /* X = X * Z */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
           (
              Point->XValue,
              Point->XValue,
              Point->ZValue,
              &CtxPtr->LNCtx
           );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(Point->XValue, CtxPtr);

  /* Y = Y * Z */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
           (
              Point->YValue,
              Point->YValue,
              Point->ZValue,
              &CtxPtr->LNCtx
           );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(Point->YValue, CtxPtr);
}

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECPointMultiplication
(
    P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Point,
  P2CONST(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Multiplicand,
    P2VAR(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
    P2VAR(Crypto_xVIx_xAIx_ECCtxType,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  /* first the result shall be set to the point of infinity. */
  Result->XValue[0U] = 0U;
  Result->XValue[1U] = 0U;
  Result->YValue[0U] = 1U;
  Result->YValue[1U] = 1U;
  Result->ZValue[0U] = 0U;
  Result->ZValue[1U] = 0U;

  CtxPtr->CurrentBit = Crypto_xVIx_xAIx_LNBitLengthOfNumber(Multiplicand);
  while(0U < CtxPtr->CurrentBit)
  {
    CtxPtr->CurrentBit--;
    Crypto_xVIx_xAIx_ECPointDoubling(
                                         Result,
                                         Result,
                                         Tmp,
                                         CtxPtr
                                       );

    if (1U == CRYPTO_XVIX_XAIX_LN_GET_BIT(Multiplicand, CtxPtr->CurrentBit))
    {
      Crypto_xVIx_xAIx_ECPointAddition(
                                           Result,
                                           Result,
                                           Point,
                                           Tmp,
                                           CtxPtr
                                         );
    }
  }
}

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECPointAdditionCheckInfinity
(
    P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend1,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend2,
    P2VAR(boolean,                      AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Finished
)
{
  /* if the Addend1 is the Point of infinity, the Result is the Addend2 */
  if (TRUE == Crypto_xVIx_xAIx_ECProjectivePointOfInfinity(Addend1))
  {

    TS_MemCpy(Result->XValue, Addend2->XValue, (Addend2->XValue[0U] + 1U) << 2U);
    TS_MemCpy(Result->YValue, Addend2->YValue, (Addend2->YValue[0U] + 1U) << 2U);
    TS_MemCpy(Result->ZValue, Addend2->ZValue, (Addend2->ZValue[0U] + 1U) << 2U);

    *Finished = TRUE;
  }
  /* if the Addend2 is the Point of infinity, the Result is the Addend1 */
  else if (TRUE == Crypto_xVIx_xAIx_ECProjectivePointOfInfinity(Addend2))
  {
    TS_MemCpy(Result->XValue, Addend1->XValue, (Addend1->XValue[0U] + 1U) << 2U);
    TS_MemCpy(Result->YValue, Addend1->YValue, (Addend1->YValue[0U] + 1U) << 2U);
    TS_MemCpy(Result->ZValue, Addend1->ZValue, (Addend1->ZValue[0U] + 1U) << 2U);

    *Finished = TRUE;
  }
  else
  {
    /* Nothing to do. */
  }
}

/*----- ECC NIST Point Addition ------------------------------------------------------------------*/

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECPointAddition
(
    P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend1,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend2,
    P2VAR(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
    P2VAR(Crypto_xVIx_xAIx_ECCtxType,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  boolean Finished = FALSE;

  Crypto_xVIx_xAIx_ECPointAdditionCheckInfinity(
                                                 Result,
                                                 Addend1,
                                                 Addend2,
                                                 &Finished
                                                );
  if(FALSE == Finished)
  {
    /* Calculate A and B */
    Crypto_xVIx_xAIx_ECPointAdditionCalcAB(Result, Addend1, Addend2, Tmp, CtxPtr);

    /* Z3 = Z1 * Z2 */
    Crypto_xVIx_xAIx_LNMultiplyNumbers
    (
      Result->ZValue,
      Addend1->ZValue,
      Addend2->ZValue,
      &CtxPtr->LNCtx
    );
    Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(Result->ZValue, CtxPtr);

    /* Calculate C */
    Crypto_xVIx_xAIx_ECPointAdditionCalcC(Result, Tmp, CtxPtr);

    /* Z3: Calculate Final result */
    Crypto_xVIx_xAIx_ECPointAdditionCalcFinalZ3(Result, CtxPtr);

    /* Calculate intern results for the point addition */
    Crypto_xVIx_xAIx_ECPointAdditionCalcInterimTmp4(Result, Tmp, CtxPtr);

    /* X3 and Y3: Calculate final results */
    Crypto_xVIx_xAIx_ECPointAdditionCalcFinalX3Y3(Result, Tmp, CtxPtr);
  }
}

/*----- ECC NIST Point of Infinity ----------------------------------------------------------------*/

FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECProjectivePointOfInfinity
(
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Point
)
{
  boolean Result;

  /* In projective coordinates the point of infintity corresponds to
   * the point (X : Y : Z) = (0 : 1 : 0).
   */
  if
  (
    (0U == Point->XValue[0U])                                &&
   ((1U == Point->YValue[0U])  && (1U == Point->YValue[1U])) &&
    (0U == Point->ZValue[0U])
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
#endif /* #if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
              (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)   || \
              (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON)  */

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Definition of functions with internal linkage]===============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/* !LINKSTO EB_Crypto_CryptAlgos_0958, 1 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNBarretSubtractNumbers
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Result,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Minuend,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Subtrahend,
  Crypto_xVIx_xAIx_LNWordType                                       CutOff
)
{
  Crypto_xVIx_xAIx_LNWordType LoopCount;
  Crypto_xVIx_xAIx_LNWordType Borrow = 0U;

  /* We subtract subtrahend from minuend (least significant word to most
     significant word) until either the subtrahend has been exhausted or
     "cutOff" words have been done, whichever comes first. */
  for (
        LoopCount = CRYPTO_XVIX_XAIX_LN_LSW_POS;
        (LoopCount <= Subtrahend[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]) && (LoopCount <= CutOff);
        LoopCount++
      )
  {
    Crypto_xVIx_xAIx_LNWordType LowerDifference;
    Crypto_xVIx_xAIx_LNWordType UpperDifference;

    /* Lower half words */
    /* Calculate the difference */
    LowerDifference =
    (
      (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LNLOWHALFWORD(Minuend[LoopCount]) -
      (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LNLOWHALFWORD(Subtrahend[LoopCount])
    ) - (Crypto_xVIx_xAIx_LNWordType)Borrow;

    /* Extract the Borrow bit */
    Borrow = (LowerDifference >> CRYPTO_XVIX_XAIX_LN_HALFWORD_LEN_BITS) & 1U;

    /* The lower difference is equal to the lower part of the difference */
    LowerDifference &= CRYPTO_XVIX_XAIX_LN_HALFWORD_MAX;

    /* Upper half words */
    /* Calculate the difference */
    UpperDifference =
      (
        (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LNHIGHHALFWORD(Minuend[LoopCount]) -
        (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LNHIGHHALFWORD(Subtrahend[LoopCount])
      ) - (Crypto_xVIx_xAIx_LNWordType)Borrow;

    /* Extract the Borrow bit */
    Borrow = (UpperDifference >> CRYPTO_XVIX_XAIX_LN_HALFWORD_LEN_BITS) & 1U;

    /* The upper difference is equal to the lower part of the difference */
    UpperDifference &= CRYPTO_XVIX_XAIX_LN_HALFWORD_MAX;

    /* Concatenate the partial differences */
    Result[LoopCount] =
      (Crypto_xVIx_xAIx_LNWordType)(UpperDifference << CRYPTO_XVIX_XAIX_LN_HALFWORD_LEN_BITS) |
      LowerDifference;
  }

  /* Copy the remaining digits */
  for (; LoopCount <= Minuend[CRYPTO_XVIX_XAIX_LN_LENGTH_POS]; LoopCount++)
  {
    Crypto_xVIx_xAIx_LNWordType LowerDifference;
    Crypto_xVIx_xAIx_LNWordType UpperDifference;

    /* Calculate the difference */
    LowerDifference =
      (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LNLOWHALFWORD(Minuend[LoopCount]) -
      (Crypto_xVIx_xAIx_LNWordType)Borrow;

    /* Extract the Borrow bit */
    Borrow = (LowerDifference >> CRYPTO_XVIX_XAIX_LN_HALFWORD_LEN_BITS) & 1U;

    /* The lower difference is equal to the lower part of the difference */
    LowerDifference &= CRYPTO_XVIX_XAIX_LN_HALFWORD_MAX;

    /* Upper half words */
    /* Calculate the difference */
    UpperDifference =
      (Crypto_xVIx_xAIx_LNWordType)CRYPTO_XVIX_XAIX_LNHIGHHALFWORD(Minuend[LoopCount]) -
      (Crypto_xVIx_xAIx_LNWordType)Borrow;

    /* Extract the Borrow bit */
    Borrow = (UpperDifference >> CRYPTO_XVIX_XAIX_LN_HALFWORD_LEN_BITS) & 1U;

    /* The upper difference is equal to the lower part of the difference */
    UpperDifference &= CRYPTO_XVIX_XAIX_LN_HALFWORD_MAX;

    /* Concatenate the partial differences */
    Result[LoopCount] =
      (Crypto_xVIx_xAIx_LNWordType)(UpperDifference << CRYPTO_XVIX_XAIX_LN_HALFWORD_LEN_BITS) |
      LowerDifference;
  }

  Result[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] = LoopCount - (Crypto_xVIx_xAIx_LNWordType)1U;

  Crypto_xVIx_xAIx_LNNormalizeNumber(Result);
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNSModuloShift
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Number,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Modulus,
  P2VAR(Crypto_xVIx_xAIx_LNCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    CtxPtr
)
{
  /* At the beginning of each round, we decrease
     CtxPtr->LengthDifference by one. */
  (CtxPtr->LengthDifference)--;

  /* We extract from CtxPtr->LengthDifference how many complete
     words to shift modulus and how many bits to shift modulus. */
  CtxPtr->ShiftWords = CRYPTO_XVIX_XAIX_LN_DIV_BY_WORD_LEN(CtxPtr->LengthDifference);
  CtxPtr->ShiftBits = CRYPTO_XVIX_XAIX_LN_MOD_WORD_LEN(CtxPtr->LengthDifference);

  /* The first position of modulus from which to create the shifted
     modulus is computed. Note that this is one larger than the
     most significant word of modulus. */
  CtxPtr->PosInMod = Modulus[CRYPTO_XVIX_XAIX_LN_LENGTH_POS] + (Crypto_xVIx_xAIx_LNWordType)1U;

  /* The first position of number which to compare to the shifted
     modulus is computed. */
  CtxPtr->PosInNum = CtxPtr->PosInMod + CtxPtr->ShiftWords;

  /* For each word of modulus, the shifted modulus is computed and
     compared to the corresponding word of number. */
  Crypto_xVIx_xAIx_LNSModuloShift2(Number, Modulus, CtxPtr);
  Crypto_xVIx_xAIx_LNSModuloSubtract(Number, Modulus, CtxPtr);
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNSModuloShift2
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Number,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Modulus,
  P2VAR(Crypto_xVIx_xAIx_LNCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    CtxPtr
)
{
  do
  {
    Crypto_xVIx_xAIx_LNWordType WordOfNumber; /* one word of number */
    /* We assemble the current word of the shifted modulus from the
       words modulus[CtxPtr->PosInMod] and modulus[CtxPtr->PosInMod - 1]. */

    /* We test whether the current position in modulus is an allowed position. */
    if(CtxPtr->PosInMod <= Modulus[CRYPTO_XVIX_XAIX_LN_LENGTH_POS])
    {
      /* The current position in modulus is allowed. We take some
         bits from the word of modulus at that position. */
      CtxPtr->WordOfShiftedMod =
      Modulus[CtxPtr->PosInMod] << CtxPtr->ShiftBits;
    }
    else
    {
      /* The current position in modulus is too high. We are at a
         "leading zero". So, all the bits which go into
         CtxPtr->WordOfShiftedMod are zero. */
      CtxPtr->WordOfShiftedMod = (Crypto_xVIx_xAIx_LNWordType)0U;
    }

    /* The needed bits from modulus[CtxPtr->PosInMod - 1] are incorporated
       into CtxPtr->WordOfShiftedMod. If we shift the modulus by complete
       words (which means "CtxPtr->ShiftBits == 0U") or if CtxPtr->PosInMod
       is the lowest possible position (which means CtxPtr->PosInMod == 1U)
       no bits from modulus[CtxPtr->PosInMod - 1U] are needed. */
    if
    (
      ((uint8)0U != CtxPtr->ShiftBits) && ((Crypto_xVIx_xAIx_LNWordType)1U < CtxPtr->PosInMod)
    )
    {
      CtxPtr->WordOfShiftedMod |=
      (Crypto_xVIx_xAIx_LNWordType)
      (
        Modulus[CtxPtr->PosInMod - (Crypto_xVIx_xAIx_LNWordType)1U] >>
        (uint8)((uint8)CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS - CtxPtr->ShiftBits)
      );
    }

    /* We compute the word of number which to compare to the word
       of the shifted modulus. This is normally simply
       number[CtxPtr->PosInNum], but the position can be too high which
       means that we are at a "leading zero" of number. */
    if(CtxPtr->PosInNum > Number[CRYPTO_XVIX_XAIX_LN_LENGTH_POS])
    {
      WordOfNumber = (Crypto_xVIx_xAIx_LNWordType)0U;
    }
    else
    {
      WordOfNumber = Number[CtxPtr->PosInNum];
    }

    /* Now, we compare the word of the shifted modulus to the current
       word of number. */
    if(CtxPtr->WordOfShiftedMod == WordOfNumber)
    {
      CtxPtr->CmpNumMod = CRYPTO_XVIX_XAIX_E_EQUAL;
    }
    else if (CtxPtr->WordOfShiftedMod > WordOfNumber)
    {
      CtxPtr->CmpNumMod = CRYPTO_XVIX_XAIX_E_SMALLER;
    }
    else
    {
      CtxPtr->CmpNumMod = CRYPTO_XVIX_XAIX_E_LARGER;
    }

    /* The positions are decreased by 1. */
    CtxPtr->PosInMod--;
    CtxPtr->PosInNum--;
  }while(
          (CRYPTO_XVIX_XAIX_E_EQUAL == CtxPtr->CmpNumMod) &&
          ((Crypto_xVIx_xAIx_LNWordType)0U < CtxPtr->PosInMod)
        );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNSModuloSubtract
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Number,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Modulus,
  P2VAR(Crypto_xVIx_xAIx_LNCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    CtxPtr
)
{
  /* If number is at least as large as the shifted modulus, we
     subtract modulus from number. */
  if(CRYPTO_XVIX_XAIX_E_SMALLER != CtxPtr->CmpNumMod)
  {
    CtxPtr->WordOfShiftedMod = (Crypto_xVIx_xAIx_LNWordType)0U;

    CtxPtr->Carry = (Crypto_xVIx_xAIx_LNWordType)0U;

    /* When subtracting the shifted modulus from number we can
       skip the least significant words of number because the
       shifted modulus is 0, there. We compute the least
       significant word of number which is affected by the
       subtraction. */
    CtxPtr->PosInNum = (Crypto_xVIx_xAIx_LNWordType)1U + CtxPtr->ShiftWords;

    /* We go through every word of the shifted modulus. */
    CtxPtr->PosInMod = (Crypto_xVIx_xAIx_LNWordType)1U;

    Crypto_xVIx_xAIx_LNSModuloSubtract2(Number, Modulus, CtxPtr);
    Crypto_xVIx_xAIx_LNSModuloCarry(Number, CtxPtr);
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNSModuloSubtract2
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Number,
  P2CONST(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Modulus,
  P2VAR(Crypto_xVIx_xAIx_LNCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    CtxPtr
)
{
  uint8 TempCarry; /* Used to store whether the Carry has to be  */
  /* set for the next word.                     */
  while(CtxPtr->PosInMod <= Modulus[CRYPTO_XVIX_XAIX_LN_LENGTH_POS])
  {
    TempCarry = (uint8)0U;

    CtxPtr->WordOfShiftedMod |=
    (Crypto_xVIx_xAIx_LNWordType)(Modulus[CtxPtr->PosInMod] << CtxPtr->ShiftBits);

    /* We test whether a CtxPtr->Carry forms at this position. This
       happens when "CtxPtr->WordOfShiftedMod + CtxPtr->Carry >
       number[CtxPtr->PosInNum]".  Since we cannot test this condition
       directly because of possible overflows. We use a more complicated
       test which actually does what we want. */
    if(
        (CtxPtr->WordOfShiftedMod >= Number[CtxPtr->PosInNum]) &&
        (
          (CtxPtr->WordOfShiftedMod > Number[CtxPtr->PosInNum]) ||
          (CtxPtr->Carry > (Crypto_xVIx_xAIx_LNWordType)0U)
        )
      )
    {
      TempCarry = (uint8)1U;
    }

    /* We subtract "CtxPtr->WordOfShiftedMod" and "CtxPtr->Carry" from
       the number. */
    Number[CtxPtr->PosInNum] -= CtxPtr->WordOfShiftedMod;
    Number[CtxPtr->PosInNum] -= (Crypto_xVIx_xAIx_LNWordType)CtxPtr->Carry;

    CtxPtr->Carry = (Crypto_xVIx_xAIx_LNWordType)TempCarry;

    if((uint8)0U == CtxPtr->ShiftBits)
    {
      CtxPtr->WordOfShiftedMod = (Crypto_xVIx_xAIx_LNWordType)0U;
    }
    else
    {
      CtxPtr->WordOfShiftedMod = Modulus[CtxPtr->PosInMod] >>
        (uint8)(CRYPTO_XVIX_XAIX_LN_WORD_LEN_BITS - CtxPtr->ShiftBits);
    }

    CtxPtr->PosInNum++;
    CtxPtr->PosInMod++;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_LNSModuloCarry
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Number,
  P2VAR(Crypto_xVIx_xAIx_LNCtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  CtxPtr
)
{
  /* Now we have to deal with leftover carries and a possible last
     word of the shifted modulus. Since the CtxPtr->Carry is at most one
     and CtxPtr->WordOfShiftedMod is at least one bit shorter than the
     largest possible word (making it at most half as large), we
     can add both numbers in "CtxPtr->Carry" without overflow. */
  CtxPtr->Carry += CtxPtr->WordOfShiftedMod;

  /* Now we deal with the leftover carries. */
  /* The CtxPtr->Carry has trickled away. */
  Number[CtxPtr->PosInNum] -= CtxPtr->Carry;
  CtxPtr->Carry = (Crypto_xVIx_xAIx_LNWordType)0U;
}

#if (CRYPTO_XVIX_XAIX_RSAPKCS_VRFY_ENABLED == STD_ON) || \
    (CRYPTO_XVIX_XAIX_RSAPKCS_GEN_ENABLED == STD_ON)
static FUNC(uint32, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CommonDerLengthEncoding
(
   P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) LengthEncodingBuffer,
   uint32                                              PayloadLength
)
{
   uint32 LengthEncodingLength;

   if(PayloadLength <= 0x7fu)
   {
      LengthEncodingBuffer[0u] = (uint8)PayloadLength;
      LengthEncodingLength = 1u;
   }
   else
   {
     /* Values greater than 0x7f need a control byte */
     uint32 numberOfBytes = 0u;
     uint32 locPayloadLength = PayloadLength;
     uint32 curPos = 0u;
     while (locPayloadLength != 0u)
     {
       locPayloadLength >>= 8u;
       numberOfBytes++;
     }
     LengthEncodingBuffer[0u] = (uint8) 0x80u | (uint8) numberOfBytes;
     /* Encode the length with big endian byte order */
     for (curPos = 1u; curPos <= numberOfBytes; curPos++)
     {
       LengthEncodingBuffer[curPos] = (uint8)(PayloadLength >> (8u * (numberOfBytes - curPos)));
     }
     LengthEncodingLength = 1u + numberOfBytes;
   }

   return LengthEncodingLength;
}
#endif /* #if (CRYPTO_XVIX_XAIX_RSAPKCS_VRFY_ENABLED == STD_ON) ||
              (CRYPTO_XVIX_XAIX_RSAPKCS_GEN_ENABLED == STD_ON) */


#if (CRYPTO_XVIX_XAIX_RSAPKCS_VRFY_ENABLED == STD_ON)  || \
    (CRYPTO_XVIX_XAIX_RSAPKCS_GEN_ENABLED  == STD_ON)  || \
    (CRYPTO_XVIX_XAIX_RSAPSS_VRFY_ENABLED  == STD_ON)  || \
    (CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_ENABLED == STD_ON) || \
    (CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_ENABLED == STD_ON)
static FUNC(uint32, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_GetDERLength
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) LengthOctets,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Result
)
{
  uint32 ReadBytes = 0U;

  /* long form of length */
  if(0U != (0x80U & LengthOctets[0U]))
  {
    /* only length of 2 Byte is supported */
    if(0x82U == LengthOctets[0U])
    {
      /* length would be negative */
      if(0U == (0x80U & LengthOctets[1U]))
      {
        *Result = (uint32)((uint32)LengthOctets[1U] << 8U) + (uint32)LengthOctets[2U];
        ReadBytes = 3U;
      }
    }
  }
  /* short form of length */
  else
  {
    *Result = LengthOctets[0U];
    ReadBytes = 1U;
  }
  return ReadBytes;
}

static FUNC(uint32, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_GetDERInteger
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                     Data,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) IntegerArray
)
{
  uint32 ReadBytes = 0U;
  uint32 Length = 0U;

  if(CRYPTO_XVIX_XAIX_AL_DER_INTEGER_TAG == Data[0U])
  {
    ReadBytes = Crypto_xVIx_xAIx_AL_GetDERLength(&Data[1U], &Length);
    if(0U != ReadBytes)
    {
      if(
          E_OK == Crypto_xVIx_xAIx_LNBEByteArrayToNumber(
                                                          &Data[1U + ReadBytes],
                                                          Length,
                                                          IntegerArray,
                                                          IntegerArray[0U] + 1U
                                                        )
        )
      {
        ReadBytes += 1U + Length;
      }
      else
      {
        ReadBytes = 0U;
      }
    }
  }
  return ReadBytes;
}
#endif /*  (CRYPTO_XVIX_XAIX_RSAPKCS_VRFY_ENABLED == STD_ON)  || \
           (CRYPTO_XVIX_XAIX_RSAPKCS_GEN_ENABLED  == STD_ON)  || \
           (CRYPTO_XVIX_XAIX_RSAPSS_VRFY_ENABLED  == STD_ON)  || \
           (CRYPTO_XVIX_XAIX_RSAESOAEP_ENC_ENABLED == STD_ON) || \
           (CRYPTO_XVIX_XAIX_RSAESOAEP_DEC_ENABLED == STD_ON)*/


/*----- Ed25519 /X25519 Fast Reduction -----------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
    (CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED == STD_ON)   || \
    (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON)
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAFastReductionCompA
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value
)
{
  /* A1, the last 8 items of Value */
  Tmp[8U] = (Crypto_xVIx_xAIx_LNWordType)(Value[16U] << 1U) |
            (Crypto_xVIx_xAIx_LNWordType)(Value[15U] >> 31U);
  Tmp[7U] = (Crypto_xVIx_xAIx_LNWordType)(Value[15U] << 1U) |
            (Crypto_xVIx_xAIx_LNWordType)(Value[14U] >> 31U);
  Tmp[6U] = (Crypto_xVIx_xAIx_LNWordType)(Value[14U] << 1U) |
            (Crypto_xVIx_xAIx_LNWordType)(Value[13U] >> 31U);
  Tmp[5U] = (Crypto_xVIx_xAIx_LNWordType)(Value[13U] << 1U) |
            (Crypto_xVIx_xAIx_LNWordType)(Value[12U] >> 31U);
  Tmp[4U] = (Crypto_xVIx_xAIx_LNWordType)(Value[12U] << 1U) |
            (Crypto_xVIx_xAIx_LNWordType)(Value[11U] >> 31U);
  Tmp[3U] = (Crypto_xVIx_xAIx_LNWordType)(Value[11U] << 1U) |
            (Crypto_xVIx_xAIx_LNWordType)(Value[10U] >> 31U);
  Tmp[2U] = (Crypto_xVIx_xAIx_LNWordType)(Value[10U] << 1U) |
            (Crypto_xVIx_xAIx_LNWordType)(Value[9U]  >> 31U);
  Tmp[1U] = (Crypto_xVIx_xAIx_LNWordType)(Value[9U]  << 1U) |
            (Crypto_xVIx_xAIx_LNWordType)(Value[8U]  >> 31U);
  Tmp[0U] = (Crypto_xVIx_xAIx_LNWordType)8U; /* the size of the number */

  /* normalize Tmp */
  Crypto_xVIx_xAIx_LNNormalizeNumber(Tmp);

  /* Value = A2 */
  Value[0U] = 8U;
  Value[8U] &= (Crypto_xVIx_xAIx_LNWordType)0x7FFFFFFFUL;

  /* normalize Value */
  Crypto_xVIx_xAIx_LNNormalizeNumber(Value);
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EdDSAFastReductionCompB
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value
)
{
  /* Store for B1 : 4-bits */
  *Tmp = Value[8U] >> 27U;

  /* Value = B2 */
  Value[8U] = (Crypto_xVIx_xAIx_LNWordType)(Value[8U] << 4U) |
              (Crypto_xVIx_xAIx_LNWordType)(Value[7U] >> 28U);
  Value[7U] = (Crypto_xVIx_xAIx_LNWordType)(Value[7U] << 4U) |
              (Crypto_xVIx_xAIx_LNWordType)(Value[6U] >> 28U);
  Value[6U] = (Crypto_xVIx_xAIx_LNWordType)(Value[6U] << 4U) |
              (Crypto_xVIx_xAIx_LNWordType)(Value[5U] >> 28U);
  Value[5U] = (Crypto_xVIx_xAIx_LNWordType)(Value[5U] << 4U) |
              (Crypto_xVIx_xAIx_LNWordType)(Value[4U] >> 28U);
  Value[4U] = (Crypto_xVIx_xAIx_LNWordType)(Value[4U] << 4U) |
              (Crypto_xVIx_xAIx_LNWordType)(Value[3U] >> 28U);
  Value[3U] = (Crypto_xVIx_xAIx_LNWordType)(Value[3U] << 4U) |
              (Crypto_xVIx_xAIx_LNWordType)(Value[2U] >> 28U);
  Value[2U] = (Crypto_xVIx_xAIx_LNWordType)(Value[2U] << 4U) |
              (Crypto_xVIx_xAIx_LNWordType)(Value[1U] >> 28U);
  Value[1U] = (Crypto_xVIx_xAIx_LNWordType)(Value[1U] << 4U);
  Value[0U] = 8U;

  Value[8U] &= (Crypto_xVIx_xAIx_LNWordType)0x7FFFFFFFUL;

  Crypto_xVIx_xAIx_LNNormalizeNumber(Value);
}
#endif /* #if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
              (CRYPTO_XVIX_XAIX_EDDSA_GEN_ENABLED == STD_ON)   || \
              (CRYPTO_XVIX_XAIX_EDDSA_VRFY_ENABLED == STD_ON) */

/*----- ECC P-256 Fast Reduction -----------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
    (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)   || \
    (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON)
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECP256FastReductionCalcValue1
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Temp,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Buffer,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  /* Save some elements of Value */
  Buffer[7U] = Value[16U];
  Buffer[6U] = Value[15U];
  Buffer[5U] = Value[14U];
  Buffer[4U] = Value[13U];
  Buffer[3U] = Value[12U];
  Buffer[2U] = Value[11U];
  Buffer[1U] = Value[10U];
  Buffer[0U] = Value[9U];

  /* Temp = s2 */
  Temp[8U] = Value[16U];
  Temp[7U] = Value[15U];
  Temp[6U] = Value[14U];
  Temp[5U] = Value[13U];
  Temp[4U] = Value[12U];
  Temp[3U] = 0x00U;
  Temp[2U] = 0x00U;
  Temp[1U] = 0x00U;
  Temp[0U] = 0x08U;

  /* Value = s1 */
  Value[0U] = 8U;

  /* Value = s1 + s2 */
  Crypto_xVIx_xAIx_LNAdditionModular(Value, Value, Temp, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);

  /* Value = s1 + 2*s2 */
  Crypto_xVIx_xAIx_LNAdditionModular(Value, Value, Temp, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);

  /* s3 */
  Temp[8U] = 0x00U;
  Temp[7U] = Buffer[7U];
  Temp[6U] = Buffer[6U];
  Temp[5U] = Buffer[5U];
  Temp[4U] = Buffer[4U];
  Temp[3U] = 0x00U;
  Temp[2U] = 0x00U;
  Temp[1U] = 0x00U;
  Temp[0U] = 0x07U;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECP256FastReductionCalcValue2
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Temp,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Buffer,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  /* s4 */
  Temp[8U] = Buffer[7U];
  Temp[7U] = Buffer[6U];
  Temp[6U] = 0x00U;
  Temp[5U] = 0x00U;
  Temp[4U] = 0x00U;
  Temp[3U] = Buffer[2U];
  Temp[2U] = Buffer[1U];
  Temp[1U] = Buffer[0U];
  Temp[0U] = 0x08U;

  /* Value = s1 + 2*s2 + 2*s3 + s4 */
  Crypto_xVIx_xAIx_LNAdditionModular(Value, Value, Temp, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);

  /* s5 */
  Temp[8U] = Buffer[0U];
  Temp[7U] = Buffer[5U];
  Temp[6U] = Buffer[7U];
  Temp[5U] = Buffer[6U];
  Temp[4U] = Buffer[5U];
  Temp[3U] = Buffer[3U];
  Temp[2U] = Buffer[2U];
  Temp[1U] = Buffer[1U];
  Temp[0U] = 0x08U;

  /* Value = s1 + 2*s2 + 2*s3 + s4 + s5 */
  Crypto_xVIx_xAIx_LNAdditionModular(Value, Value, Temp, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);

  /* s6 */
  Temp[8U] = Buffer[2U];
  Temp[7U] = Buffer[0U];
  Temp[6U] = 0x00U;
  Temp[5U] = 0x00U;
  Temp[4U] = 0x00U;
  Temp[3U] = Buffer[5U];
  Temp[2U] = Buffer[4U];
  Temp[1U] = Buffer[3U];
  Temp[0U] = 0x08U;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECP256FastReductionCalcValue3
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Temp,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Buffer,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  /* s7 */
  Temp[8U] = Buffer[3U];
  Temp[7U] = Buffer[1U];
  Temp[6U] = 0x00U;
  Temp[5U] = 0x00U;
  Temp[4U] = Buffer[7U];
  Temp[3U] = Buffer[6U];
  Temp[2U] = Buffer[5U];
  Temp[1U] = Buffer[4U];
  Temp[0U] = 0x08U;

  /* Value = s1 + 2*s2 + 2*s3 + s4 + s5 - s6 - s7 */
  Crypto_xVIx_xAIx_LNSubtractionModular(Value, Value, Temp, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);

  /* s8 */
  Temp[8U] = Buffer[4U];
  Temp[7U] = 0x00U;
  Temp[6U] = Buffer[2U];
  Temp[5U] = Buffer[1U];
  Temp[4U] = Buffer[0U];
  Temp[3U] = Buffer[7U];
  Temp[2U] = Buffer[6U];
  Temp[1U] = Buffer[5U];
  Temp[0U] = 0x08U;

  /* Value = s1 + 2*s2 + 2*s3 + s4 + s5 - s6 - s7  - s8 */
  Crypto_xVIx_xAIx_LNSubtractionModular(Value, Value, Temp, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);

  /* s9 */
  Temp[8U] = Buffer[5U];
  Temp[7U] = 0x00U;
  Temp[6U] = Buffer[3U];
  Temp[5U] = Buffer[2U];
  Temp[4U] = Buffer[1U];
  Temp[3U] = 0x00U;
  Temp[2U] = Buffer[7U];
  Temp[1U] = Buffer[6U];
  Temp[0U] = 0x08U;
}
#endif /* #if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
              (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)   || \
              (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON) */

/*----- SecP384r1 / NIST P-384 -------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON)
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECP384FastReductionCalcValue1
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Temp,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Buffer,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  /* Save some elements of Value */
  Buffer[11U] = Value[24U];
  Buffer[10U] = Value[23U];
  Buffer[9U ] = Value[22U];
  Buffer[8U ] = Value[21U];
  Buffer[7U ] = Value[20U];
  Buffer[6U ] = Value[19U];
  Buffer[5U ] = Value[18U];
  Buffer[4U ] = Value[17U];
  Buffer[3U ] = Value[16U];
  Buffer[2U ] = Value[15U];
  Buffer[1U ] = Value[14U];
  Buffer[0U ] = Value[13U];

  /* Temp = s2 */
  Temp[12U] = 0x00U;
  Temp[11U] = 0x00U;
  Temp[10U] = 0x00U;
  Temp[9U ] = 0x00U;
  Temp[8U ] = 0x00U;
  Temp[7U ] = Value[24U];
  Temp[6U ] = Value[23U];
  Temp[5U ] = Value[22U];
  Temp[4U ] = 0x00U;
  Temp[3U ] = 0x00U;
  Temp[2U ] = 0x00U;
  Temp[1U ] = 0x00U;
  Temp[0U ] = 7U;

  /* Value = s1 */
  Value[0U] = 12U;

  /* Value = s1 + s2 */
  Crypto_xVIx_xAIx_LNAdditionModular(Value, Value, Temp, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);

  /* Value = s1 + 2*s2 */
  Crypto_xVIx_xAIx_LNAdditionModular(Value, Value, Temp, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);

  /* s3 */
  Temp[12U] = Buffer[11U];
  Temp[11U] = Buffer[10U];
  Temp[10U] = Buffer[9U];
  Temp[9U ] = Buffer[8U];
  Temp[8U ] = Buffer[7U];
  Temp[7U ] = Buffer[6U];
  Temp[6U ] = Buffer[5U];
  Temp[5U ] = Buffer[4U];
  Temp[4U ] = Buffer[3U];
  Temp[3U ] = Buffer[2U];
  Temp[2U ] = Buffer[1U];
  Temp[1U ] = Buffer[0U];
  Temp[0U ] = 12U;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECP384FastReductionCalcValue2
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Temp,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Buffer,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  /* s4 */
  Temp[12U] = Buffer[8U];
  Temp[11U] = Buffer[7U];
  Temp[10U] = Buffer[6U];
  Temp[9U ] = Buffer[5U];
  Temp[8U ] = Buffer[4U];
  Temp[7U ] = Buffer[3U];
  Temp[6U ] = Buffer[2U];
  Temp[5U ] = Buffer[1U];
  Temp[4U ] = Buffer[0U];
  Temp[3U ] = Buffer[11U];
  Temp[2U ] = Buffer[10U];
  Temp[1U ] = Buffer[9U];
  Temp[0U ] = 12U;

  /* Value = s1 + 2*s2 + s3 + s4 */
  Crypto_xVIx_xAIx_LNAdditionModular(Value, Value, Temp, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);

  /* s5 */
  Temp[12U] = Buffer[7U];
  Temp[11U] = Buffer[6U];
  Temp[10U] = Buffer[5U];
  Temp[9U ] = Buffer[4U];
  Temp[8U ] = Buffer[3U];
  Temp[7U ] = Buffer[2U];
  Temp[6U ] = Buffer[1U];
  Temp[5U ] = Buffer[0U];
  Temp[4U ] = Buffer[8U];
  Temp[3U ] = 0x00U;
  Temp[2U ] = Buffer[11U];
  Temp[1U ] = 0x00U;
  Temp[0U ] = 12U;

  /* Value = s1 + 2*s2 + s3 + s4 + s5 */
  Crypto_xVIx_xAIx_LNAdditionModular(Value, Value, Temp, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);

  /* s6 */
  Temp[12U] = 0x00U;
  Temp[11U] = 0x00U;
  Temp[10U] = 0x00U;
  Temp[9U ] = 0x00U;
  Temp[8U ] = Buffer[11U];
  Temp[7U ] = Buffer[10U];
  Temp[6U ] = Buffer[9U ];
  Temp[5U ] = Buffer[8U ];
  Temp[4U ] = 0x00U;
  Temp[3U ] = 0x00U;
  Temp[2U ] = 0x00U;
  Temp[1U ] = 0x00U;
  Temp[0U ] = 8U;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECP384FastReductionCalcValue3
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Temp,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Buffer,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  /* s7 */
  Temp[12U] = 0x00U;
  Temp[11U] = 0x00U;
  Temp[10U] = 0x00U;
  Temp[9U ] = 0x00U;
  Temp[8U ] = 0x00U;
  Temp[7U ] = 0x00U;
  Temp[6U ] = Buffer[11U];
  Temp[5U ] = Buffer[10U];
  Temp[4U ] = Buffer[9U ];
  Temp[3U ] = 0x00U;
  Temp[2U ] = 0x00U;
  Temp[1U ] = Buffer[8U ];
  Temp[0U ] = 6U;

  /* Value = s1 + 2*s2 + s3 + s4 + s5 + s6 + s7 */
  Crypto_xVIx_xAIx_LNAdditionModular(Value, Value, Temp, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);

  /* s8 */
  Temp[12U] = Buffer[10U];
  Temp[11U] = Buffer[9U ];
  Temp[10U] = Buffer[8U ];
  Temp[9U ] = Buffer[7U ];
  Temp[8U ] = Buffer[6U ];
  Temp[7U ] = Buffer[5U ];
  Temp[6U ] = Buffer[4U ];
  Temp[5U ] = Buffer[3U ];
  Temp[4U ] = Buffer[2U ];
  Temp[3U ] = Buffer[1U ];
  Temp[2U ] = Buffer[0U ];
  Temp[1U ] = Buffer[11U];
  Temp[0U ] = 12U;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECP384FastReductionCalcValue4
(
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Temp,
  P2VAR(Crypto_xVIx_xAIx_LNWordType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Buffer,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  /* s9 */
  Temp[12U] = 0x00U;
  Temp[11U] = 0x00U;
  Temp[10U] = 0x00U;
  Temp[9U ] = 0x00U;
  Temp[8U ] = 0x00U;
  Temp[7U ] = 0x00U;
  Temp[6U ] = 0x00U;
  Temp[5U ] = Buffer[11U];
  Temp[4U ] = Buffer[10U];
  Temp[3U ] = Buffer[9U ];
  Temp[2U ] = Buffer[8U ];
  Temp[1U ] = 0x00U;
  Temp[0U ] = 5U;

  /* Value = s1 + 2*s2 + s3 + s4 + s5 + s6 + s7 - s8 - s9 */
  Crypto_xVIx_xAIx_LNSubtractionModular(Value, Value, Temp, Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime);

  /* s10 */
  Temp[12U] = 0x00U;
  Temp[11U] = 0x00U;
  Temp[10U] = 0x00U;
  Temp[9U ] = 0x00U;
  Temp[8U ] = 0x00U;
  Temp[7U ] = 0x00U;
  Temp[6U ] = 0x00U;
  Temp[5U ] = Buffer[11U];
  Temp[4U ] = Buffer[11U];
  Temp[3U ] = 0x00U;
  Temp[2U ] = 0x00U;
  Temp[1U ] = 0x00U;
  Temp[0U ] = 5U;
}
#endif /* #if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
    (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)   || \
    (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON)
/*----- ECC NIST Point Addition ------------------------------------------------------------------*/

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECPointAdditionCalcAB
(
    P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend1,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Addend2,
    P2VAR(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
    P2VAR(Crypto_xVIx_xAIx_ECCtxType,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  /* Tmp1 = Y1 * Z2 */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1],
    Addend1->YValue,
    Addend2->ZValue,
    &CtxPtr->LNCtx
  );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(&Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1], CtxPtr);

  /* Tmp2 = X1 * Z2 */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_2],
    Addend1->XValue,
    Addend2->ZValue,
    &CtxPtr->LNCtx
  );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(&Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_2], CtxPtr);

  /* We are using the result array as temporary storage. But we have to
   * be careful because the input and output points can be the same
   * storage. So, e.g., after setting Result->XValue we can no longer use
   * Addend1->XValue nor Addend2->XValue.
   */

  /* Y3 = Y2 * Z1 - Tmp1 = A */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    Result->YValue,
    Addend2->YValue,
    Addend1->ZValue,
    &CtxPtr->LNCtx
  );

  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(Result->YValue, CtxPtr);

  Crypto_xVIx_xAIx_LNSubtractionModular
  (
    Result->YValue,
    Result->YValue,
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1],
    Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime
  );

  /* X3 = X2 * Z1 - Tmp2 = B */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    Result->XValue,
    Addend2->XValue,
    Addend1->ZValue,
    &CtxPtr->LNCtx
  );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(Result->XValue, CtxPtr);

  Crypto_xVIx_xAIx_LNSubtractionModular
  (
    Result->XValue,
    Result->XValue,
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_2],
    Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime
  );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECPointAdditionCalcC
(
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  /* Tmp4 = Y3^2 */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_4],
    Result->YValue,
    Result->YValue,
    &CtxPtr->LNCtx
  );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(&Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_4], CtxPtr);

  /* Tmp4 = Tmp4 * Z3 = (Y3^2) * Z3 */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_4],
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_4],
    Result->ZValue,
    &CtxPtr->LNCtx
  );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(&Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_4], CtxPtr);

  /* Tmp3 = X3 + 2 * Tmp2 */
  Crypto_xVIx_xAIx_LNAdditionModular
  (
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_3],
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_2],
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_2],
    Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime
  );

  Crypto_xVIx_xAIx_LNAdditionModular
  (
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_3],
    Result->XValue,
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_3],
    Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime
  );

  /* Tmp3 = X3 * Tmp3 */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_3],
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_3],
    Result->XValue,
    &CtxPtr->LNCtx
  );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(&Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_3], CtxPtr);

  /* Tmp3 = X3 * Tmp3 */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_3],
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_3],
    Result->XValue,
    &CtxPtr->LNCtx
  );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(&Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_3], CtxPtr);

  /* Tmp3 = Tmp4 - Tmp3 = C */
  Crypto_xVIx_xAIx_LNSubtractionModular
  (
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_3],
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_4],
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_3],
    Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime
  );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECPointAdditionCalcFinalZ3
(
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  /* Z3 = X3 * Z3 */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    Result->ZValue,
    Result->XValue,
    Result->ZValue,
    &CtxPtr->LNCtx
  );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(Result->ZValue, CtxPtr);

  /* Z3 = X3 * Z3 */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    Result->ZValue,
    Result->XValue,
    Result->ZValue,
    &CtxPtr->LNCtx
  );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(Result->ZValue, CtxPtr);

  /* Z3 = X3 * Z3 (final result) */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    Result->ZValue,
    Result->XValue,
    Result->ZValue,
    &CtxPtr->LNCtx
  );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(Result->ZValue, CtxPtr);
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECPointAdditionCalcInterimTmp4
(
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  /* Tmp4 = X3^2 */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_4],
    Result->XValue,
    Result->XValue,
    &CtxPtr->LNCtx
  );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(&Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_4], CtxPtr);

  /* Tmp4 = Tmp4 * Tmp2 */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_4],
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_4],
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_2],
    &CtxPtr->LNCtx
    );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(&Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_4], CtxPtr);

  /* Tmp4 = Tmp4 - Tmp3 */
  Crypto_xVIx_xAIx_LNSubtractionModular
  (
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_4],
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_4],
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_3],
    Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime
  );

  /* Tmp4 = Y3 * Tmp4 */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_4],
    Result->YValue,
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_4],
    &CtxPtr->LNCtx
  );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(&Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_4], CtxPtr);
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECPointAdditionCalcFinalX3Y3
(
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  /* Y3 = X3^2 */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    Result->YValue,
    Result->XValue,
    Result->XValue,
    &CtxPtr->LNCtx
  );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(Result->YValue, CtxPtr);

  /* Y3 = Y3 * X3 = X3^3 */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    Result->YValue,
    Result->YValue,
    Result->XValue,
    &CtxPtr->LNCtx
  );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(Result->YValue, CtxPtr);

  /* Y3 = Y3 * Tmp1 */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    Result->YValue,
    Result->YValue,
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1],
    &CtxPtr->LNCtx
  );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(Result->YValue, CtxPtr);

  /* Y3 = Tmp4 - Y3 (final result) */
  Crypto_xVIx_xAIx_LNSubtractionModular
  (
     Result->YValue,
     &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_4],
     Result->YValue,
     Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime
  );

  /* X3 = X3*Tmp3 (final result) */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    Result->XValue,
    Result->XValue,
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_3],
    &CtxPtr->LNCtx
  );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(Result->XValue, CtxPtr);
}

/*----- ECC NIST Point Doubling ------------------------------------------------------------------*/

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECPointDoubling
(
    P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
    P2VAR(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
    P2VAR(Crypto_xVIx_xAIx_ECCtxType,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  /* If the point is the point in infinity, there is no calculation
   * necessary.
   */
  if (TRUE == Crypto_xVIx_xAIx_ECProjectivePointOfInfinity(Value))
  {
    Result->XValue[0U] = 0U;
    Result->XValue[1U] = 0U;
    Result->YValue[0U] = 1U;
    Result->YValue[1U] = 1U;
    Result->ZValue[0U] = 0U;
    Result->ZValue[1U] = 0U;
  }
  else
  {
    /* Calculate A */
    Crypto_xVIx_xAIx_ECPointDoublingCalcAB(Value, Tmp, CtxPtr);

    /* Calculate intern results for the point doubling */
    Crypto_xVIx_xAIx_ECPointDoublingCalcInternTmp(Result, Value, Tmp, CtxPtr);

    /* Calculate D and Y3 (final result) */
    Crypto_xVIx_xAIx_ECPointDoublingCalcFinalY3(Result, Tmp, CtxPtr);

    /* X3 and Z3: Calculate final results */
    Crypto_xVIx_xAIx_ECPointDoublingCalcFinalX3Z3(Result, Tmp, CtxPtr);
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECPointDoublingCalcAB
(
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
    P2VAR(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
    P2VAR(Crypto_xVIx_xAIx_ECCtxType,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  /* Tmp1 = X1 - Z1 */
  Crypto_xVIx_xAIx_LNSubtractionModular
  (
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1],
    Value->XValue,
    Value->ZValue,
    Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime
  );

  /* Tmp2 = X1 + Z1 */
  Crypto_xVIx_xAIx_LNAdditionModular
  (
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_2],
    Value->XValue,
    Value->ZValue,
    Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime
  );

  /* Tmp1 = Tmp1*Tmp2 = X1^2 - Z1^2 */
  Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                      &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1],
                                      &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1],
                                      &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_2],
                                      &CtxPtr->LNCtx
                                    );

  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(&Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1], CtxPtr);

  /* Tmp4 = 3*Tmp1 = 3(X1^2 - Z1^2) = A */
  Crypto_xVIx_xAIx_LNAdditionModular
  (
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_4],
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1],
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1],
    Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime
  );

  Crypto_xVIx_xAIx_LNAdditionModular
  (
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_4],
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_4],
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1],
    Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime
  );

  /* Tmp1 = Y1*Z1 = B */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1],
    Value->YValue,
    Value->ZValue,
    &CtxPtr->LNCtx
  );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(&Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1], CtxPtr);

}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECPointDoublingCalcInternTmp
(
    P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2CONST(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Value,
    P2VAR(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
    P2VAR(Crypto_xVIx_xAIx_ECCtxType,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  /* Tmp3 = 2*Tmp1 = 2B */
  Crypto_xVIx_xAIx_LNAdditionModular
  (
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_3],
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1],
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1],
    Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime
  );

  /* Tmp1 = Y1*Tmp3 = 2B*Y1 */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1],
    Value->YValue,
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_3],
    &CtxPtr->LNCtx
  );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(&Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1], CtxPtr);

  /* Tmp2 = Tmp1^2 */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_2],
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1],
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1],
    &CtxPtr->LNCtx
  );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(&Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_2], CtxPtr);

  /* Y3 = 2*Tmp2 */
  Crypto_xVIx_xAIx_LNAdditionModular
  (
    Result->YValue,
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_2],
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_2],
    Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime
  );

  /* Tmp1 = 2*X1*Tmp1 = 4C */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1],
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1],
    Value->XValue,
    &CtxPtr->LNCtx
  );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(&Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1], CtxPtr);

  Crypto_xVIx_xAIx_LNAdditionModular
  (
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1],
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1],
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1],
    Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime
  );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECPointDoublingCalcFinalY3
(
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  /* Tmp2 = Tmp4^2 - 2*Tmp1 = D */
  Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                      &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_2],
                                      &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_4],
                                      &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_4],
                                      &CtxPtr->LNCtx
                                    );

  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(&Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_2], CtxPtr);

  Crypto_xVIx_xAIx_LNSubtractionModular
  (
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_2],
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_2],
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1],
    Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime
  );

  Crypto_xVIx_xAIx_LNSubtractionModular
  (
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_2],
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_2],
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1],
    Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime
  );

  /* Tmp1 = Tmp1 - Tmp2 */
  Crypto_xVIx_xAIx_LNSubtractionModular
  (
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1],
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1],
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_2],
    Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime
  );

  /* Tmp1 = Tmp4 * Tmp1 */
  Crypto_xVIx_xAIx_LNMultiplyNumbers(
                                      &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1],
                                      &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1],
                                      &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_4],
                                      &CtxPtr->LNCtx
                                    );

  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(&Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1], CtxPtr);

  /* Y3 = Tmp1 - Y3 (final result) */
  Crypto_xVIx_xAIx_LNSubtractionModular
  (
    Result->YValue,
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_1],
    Result->YValue,
    Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].Prime
  );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ECPointDoublingCalcFinalX3Z3
(
  P2VAR(Crypto_xVIx_xAIx_ECPointType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(Crypto_xVIx_xAIx_LNWordType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Tmp,
  P2VAR(Crypto_xVIx_xAIx_ECCtxType,   AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  /* Z3 = Tmp3^2 */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    Result->ZValue,
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_3],
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_3],
    &CtxPtr->LNCtx
  );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(Result->ZValue, CtxPtr);

  /* Z3 = Z3 * Tmp3 (final result) */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    Result->ZValue,
    Result->ZValue,
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_3],
    &CtxPtr->LNCtx
  );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(Result->ZValue, CtxPtr);

  /* X3 = Tmp2 * Tmp3 (final result) */
  Crypto_xVIx_xAIx_LNMultiplyNumbers
  (
    Result->XValue,
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_2],
    &Tmp[CRYPTO_XVIX_XAIX_ECTEMP_OFFSET_3],
    &CtxPtr->LNCtx
  );
  Crypto_xVIx_xAIx_EllipticCurveInfo[CtxPtr->CurveIndex].FastReductionFun(Result->XValue, CtxPtr);
}
#endif /* #if (CRYPTO_XVIX_XAIX_KEYEXCHANGE_ENABLED == STD_ON) || \
              (CRYPTO_XVIX_XAIX_ECDSA_GEN_ENABLED == STD_ON)   || \
              (CRYPTO_XVIX_XAIX_ECDSA_VRFY_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[end of file]=================================================================================*/

