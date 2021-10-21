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

/*
 *  MISRA-C:2012 Deviation List
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
#include <Crypto_xVIx_xAIx_AL_Hash.h>
#include <Crypto_xVIx_xAIx_AL_Common.h>
#include <Crypto_xVIx_xAIx_AL_Cfg.h>

#if(                                                                                                   \
     (CRYPTO_XVIX_XAIX_SHA2_224_ENABLED == STD_ON) || (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON) || \
     (CRYPTO_XVIX_XAIX_SHA2_384_ENABLED == STD_ON) || (CRYPTO_XVIX_XAIX_SHA2_512_ENABLED == STD_ON)    \
   )

/*==[Macros]======================================================================================*/

#if (defined CRYPTO_XVIX_XAIX_HASH_DEFAULT_CURRENT_MODE)
#error CRYPTO_XVIX_XAIX_HASH_DEFAULT_CURRENT_MODE is already defined
#endif
#define CRYPTO_XVIX_XAIX_HASH_DEFAULT_CURRENT_MODE 0x00U

/*--SHACommon-------------------------------------------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_SHA2_224_256_WORD_SIZE)
#error CRYPTO_XVIX_XAIX_SHA2_224_256_WORD_SIZE is already defined
#endif
/** \brief Word size for SHA 224 and SHA 256 */
#define CRYPTO_XVIX_XAIX_SHA2_224_256_WORD_SIZE 32U

#if (defined CRYPTO_XVIX_XAIX_SHA2_384_512_WORD_SIZE)
#error CRYPTO_XVIX_XAIX_SHA2_384_512_WORD_SIZE is already defined
#endif
/** \brief Word size for SHA 384 and SHA  */
#define CRYPTO_XVIX_XAIX_SHA2_384_512_WORD_SIZE 64U

#if (defined CRYPTO_XVIX_XAIX_SHA_STATE_WORDS)
#error CRYPTO_XVIX_XAIX_SHA_STATE_WORDS is already defined
#endif
/** \brief Size of the internal state*/
#define CRYPTO_XVIX_XAIX_SHA_STATE_WORDS 8U

/*--SHA2------------------------------------------------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_SHA_2_MASK_CLEAR_LEFTMOST_6_BITS)
#error CRYPTO_XVIX_XAIX_SHA_2_MASK_CLEAR_LEFTMOST_6_BITS is already defined
#endif
/** \brief Used to clear the 6 leftmost bits*/
#define CRYPTO_XVIX_XAIX_SHA_2_MASK_CLEAR_LEFTMOST_6_BITS 0xffffffc0UL

#if (defined CRYPTO_XVIX_XAIX_SHA_2_MASK_CLEAR_LEFTMOST_7_BITS)
#error CRYPTO_XVIX_XAIX_SHA_2_MASK_CLEAR_LEFTMOST_7_BITS is already defined
#endif
/** \brief Used to clear the 7 leftmost bits*/
#define CRYPTO_XVIX_XAIX_SHA_2_MASK_CLEAR_LEFTMOST_7_BITS 0xffffff80UL

#if (defined CRYPTO_XVIX_XAIX_SHA_224_256_BLOCK_LEN_BYTES)
#error CRYPTO_XVIX_XAIX_SHA_224_256_BLOCK_LEN_BYTES is already defined
#endif
/** \brief Size of the SHA2 in byte*/
#define CRYPTO_XVIX_XAIX_SHA_224_256_BLOCK_LEN_BYTES 64U

#if (defined CRYPTO_XVIX_XAIX_SHA_224_256_BLOCK_LEN_WORDS)
#error CRYPTO_XVIX_XAIX_SHA_224_256_BLOCK_LEN_WORDS is already defined
#endif
/** \brief Size of the SHA2 in word*/
#define CRYPTO_XVIX_XAIX_SHA_224_256_BLOCK_LEN_WORDS 16U

#if (defined CRYPTO_XVIX_XAIX_SHA_384_512_BW_BLOCK_LEN_BYTES)
#error CRYPTO_XVIX_XAIX_SHA_384_512_BW_BLOCK_LEN_BYTES is already defined
#endif
/** \brief Size of the SHA2 in byte*/
#define CRYPTO_XVIX_XAIX_SHA_384_512_BW_BLOCK_LEN_BYTES 128U

#if (defined CRYPTO_XVIX_XAIX_SHA_384_512_BW_BLOCK_LEN_WORDS)
#error CRYPTO_XVIX_XAIX_SHA_384_512_BW_BLOCK_LEN_WORDS is already defined
#endif
/** \brief Size of the SHA2 in byte*/
#define CRYPTO_XVIX_XAIX_SHA_384_512_BW_BLOCK_LEN_WORDS 160U

#if (defined CRYPTO_XVIX_XAIX_SHA224_HASH_LEN_BYTES)
#error CRYPTO_XVIX_XAIX_SHA224_HASH_LEN_BYTES is already defined
#endif
/** \brief  The length of a message digest computed with the SHA-224 algorithm in  bytes.*/
#define CRYPTO_XVIX_XAIX_SHA224_HASH_LEN_BYTES 28U

#if (defined CRYPTO_XVIX_XAIX_SHA256_HASH_LEN_BYTES)
#error CRYPTO_XVIX_XAIX_SHA256_HASH_LEN_BYTES is already defined
#endif
/** \brief  The length of a message digest computed with the SHA-256 algorithm in  bytes.*/
#define CRYPTO_XVIX_XAIX_SHA256_HASH_LEN_BYTES 32U

#if (defined CRYPTO_XVIX_XAIX_SHA384_HASH_LEN_BYTES)
#error CRYPTO_XVIX_XAIX_SHA384_HASH_LEN_BYTES is already defined
#endif
/** \brief  The length of a message digest computed with the SHA-384 algorithm in  bytes.*/
#define CRYPTO_XVIX_XAIX_SHA384_HASH_LEN_BYTES 48U

#if (defined CRYPTO_XVIX_XAIX_SHA512_HASH_LEN_BYTES)
#error CRYPTO_XVIX_XAIX_SHA512_HASH_LEN_BYTES is already defined
#endif
/** \brief  The length of a message digest computed with the SHA-512 algorithm in  bytes.*/
#define CRYPTO_XVIX_XAIX_SHA512_HASH_LEN_BYTES 64U

#if (defined CRYPTO_XVIX_XAIX_SHA2_224_256_BLOCK_LEN_BYTES)
#error CRYPTO_XVIX_XAIX_SHA2_224_256_BLOCK_LEN_BYTES is already defined
#endif
/** \brief  A message which should be hashed is divided into blocks which are processed
 * individually. This macro gives the length of a block in bytes.
 */
#define CRYPTO_XVIX_XAIX_SHA2_224_256_BLOCK_LEN_BYTES CRYPTO_XVIX_XAIX_SHA_224_256_BLOCK_LEN_BYTES

#if (defined CRYPTO_XVIX_XAIX_SHA2_384_512_BLOCK_LEN_BYTES)
#error CRYPTO_XVIX_XAIX_SHA2_384_512_BLOCK_LEN_BYTES is already defined
#endif
/** \brief  A message which should be hashed is divided into blocks which are processed
 * individually. This macro gives the length of a block in bytes.
 */
#define CRYPTO_XVIX_XAIX_SHA2_384_512_BLOCK_LEN_BYTES CRYPTO_XVIX_XAIX_SHA_384_512_BLOCK_LEN_BYTES

#if (defined CRYPTO_XVIX_XAIX_SHA2_224_256_BLOCK_LEN_WORDS)
#error CRYPTO_XVIX_XAIX_SHA2_224_256_BLOCK_LEN_WORDS is already defined
#endif
/** \brief   A message which should be hashed is divided into blocks which are processed
 * individually. This macro gives the length of a block in words.
 */
#define CRYPTO_XVIX_XAIX_SHA2_224_256_BLOCK_LEN_WORDS CRYPTO_XVIX_XAIX_SHA_224_256_BLOCK_LEN_WORDS

#if (defined CRYPTO_XVIX_XAIX_SHA2_384_512_BLOCK_LEN_WORDS)
#error CRYPTO_XVIX_XAIX_SHA2_384_512_BLOCK_LEN_WORDS is already defined
#endif
/** \brief   A message which should be hashed is divided into blocks which are processed
 * individually. This macro gives the length of a block in words.
 */
#define CRYPTO_XVIX_XAIX_SHA2_384_512_BLOCK_LEN_WORDS CRYPTO_XVIX_XAIX_SHA_384_512_BLOCK_LEN_WORDS

#if (defined CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_224_ALGORITHM_ID)
#error CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_224_ALGORITHM_ID is already defined
#endif
/** \brief   The X.509 algorithm identifier of the SHA-224 hash algorithm*/
#define CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_224_ALGORITHM_ID    \
{                                                         \
  0x30U, 0x0dU, 0x06U, 0x09U, 0x60U, 0x86U, 0x48U, 0x01U, \
  0x65U, 0x03U, 0x04U, 0x02U, 0x04U, 0x05U, 0x00U         \
}

#if (defined CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_256_ALGORITHM_ID)
#error CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_256_ALGORITHM_ID is already defined
#endif
/** \brief   The X.509 algorithm identifier of the SHA-256 hash algorithm*/
#define CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_256_ALGORITHM_ID    \
{                                                         \
  0x30U, 0x0dU, 0x06U, 0x09U, 0x60U, 0x86U, 0x48U, 0x01U, \
  0x65U, 0x03U, 0x04U, 0x02U, 0x01U, 0x05U, 0x00U         \
}

#if (defined CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_384_ALGORITHM_ID)
#error CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_384_ALGORITHM_ID is already defined
#endif
/** \brief   The X.509 algorithm identifier of the SHA-384 hash algorithm*/
#define CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_384_ALGORITHM_ID    \
{                                                         \
  0x30U, 0x0dU, 0x06U, 0x09U, 0x2aU, 0x86U, 0x48U, 0x86U, \
  0xf7U, 0x0dU, 0x01U, 0x01U, 0x0cU, 0x05U, 0x00U         \
}

#if (defined CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_512_ALGORITHM_ID)
#error CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_512_ALGORITHM_ID is already defined
#endif
/** \brief   The X.509 algorithm identifier of the SHA-512 hash algorithm*/
#define CRYPTO_XVIX_XAIX_ALGOFAM_SHA2_512_ALGORITHM_ID    \
{                                                         \
  0x30U, 0x0dU, 0x06U, 0x09U, 0x2aU, 0x86U, 0x48U, 0x86U, \
  0xf7U, 0x0dU, 0x01U, 0x01U, 0x0dU, 0x05U, 0x00U         \
}

#if (defined CRYPTO_XVIX_XAIX_SHA224256_ROUNDS)
#error CRYPTO_XVIX_XAIX_SHA224256_ROUNDS is already defined
#endif
/** \brief The number of rounds performed by the SHA-256 or SHA-224 algorithm*/
#define CRYPTO_XVIX_XAIX_SHA224256_ROUNDS 64U

#if (defined CRYPTO_XVIX_XAIX_SHA224256_ROTR)
#error CRYPTO_XVIX_XAIX_SHA224256_ROTR is already defined
#endif
/** \brief Rotate a 32-bit word by a given amount of bits to the right*/
#define CRYPTO_XVIX_XAIX_SHA224256_ROTR(Word, Amount)                      \
((uint32) (((Word) >> (Amount)) | ((Word) << (32U - (Amount)))))

#if (defined CRYPTO_XVIX_XAIX_SHA224256_ROTL)
#error CRYPTO_XVIX_XAIX_SHA224256_ROTL is already defined
#endif
/** \brief Rotate a 32-bit word by a given amount of bits to the left.*/
#define CRYPTO_XVIX_XAIX_SHA224256_ROTL(Word, Amount)                      \
((uint32) (((Word) << (Amount)) | ((Word) >> (32U - (Amount)))))

#if (defined CRYPTO_XVIX_XAIX_SHA224256_CAPITAL_SIGMA_0)
#error CRYPTO_XVIX_XAIX_SHA224256_CAPITAL_SIGMA_0 is already defined
#endif
/** \brief The function (in LaTex notation) $\Sigma^{\{256 or 224\}}_0(x)$ from the
 * description of SHA-256 or SHA-224 in chapter 4.1.2 of the standard FIPS 180-2
 */
#define CRYPTO_XVIX_XAIX_SHA224256_CAPITAL_SIGMA_0(Word)  \
(                                               \
  (uint32) (                                    \
            CRYPTO_XVIX_XAIX_SHA224256_ROTR(Word,  2U) ^  \
            CRYPTO_XVIX_XAIX_SHA224256_ROTR(Word, 13U) ^  \
            CRYPTO_XVIX_XAIX_SHA224256_ROTR(Word, 22U)    \
          )                                     \
)

#if (defined CRYPTO_XVIX_XAIX_SHA224256_CAPITAL_SIGMA_1)
#error CRYPTO_XVIX_XAIX_SHA224256_CAPITAL_SIGMA_1 is already defined
#endif
/** \brief The function (in LaTex notation) $\Sigma^{\{256 or 224\}}_1(x)$ from the
 * description of SHA-256 or SHA-224 in chapter 4.1.2 of the standard FIPS 180-2
 */
#define CRYPTO_XVIX_XAIX_SHA224256_CAPITAL_SIGMA_1(Word)  \
(                                               \
  (uint32) (                                    \
            CRYPTO_XVIX_XAIX_SHA224256_ROTR(Word,  6U) ^  \
            CRYPTO_XVIX_XAIX_SHA224256_ROTR(Word, 11U) ^  \
            CRYPTO_XVIX_XAIX_SHA224256_ROTR(Word, 25U)    \
          )                                     \
)

#if (defined CRYPTO_XVIX_XAIX_SHA224256_SMALL_SIGMA_0)
#error CRYPTO_XVIX_XAIX_SHA224256_SMALL_SIGMA_0 is already defined
#endif
/** \brief The function (in LaTex notation) $\sigma^{\{256 or 224\}}_0(x)$ from the
 *  description of SHA-256 or SHA-224 in chapter 4.1.2 of the standard FIPS 180-2
 */
#define CRYPTO_XVIX_XAIX_SHA224256_SMALL_SIGMA_0(Word)   \
(                                              \
  (uint32) (                                   \
            CRYPTO_XVIX_XAIX_SHA224256_ROTR(Word,  7U) ^ \
            CRYPTO_XVIX_XAIX_SHA224256_ROTR(Word, 18U) ^ \
            (uint32) ((Word) >> 3U)            \
          )                                    \
)

#if (defined CRYPTO_XVIX_XAIX_SHA224256_SMALL_SIGMA_1)
#error CRYPTO_XVIX_XAIX_SHA224256_SMALL_SIGMA_1 is already defined
#endif
/** \brief The function (in LaTex notation) $\sigma^{\{256 or 224\}}_1(x)$ from the
 * description of SHA-256 or SHA-224 in chapter 4.1.2 of the standard FIPS 180-2
 */
#define CRYPTO_XVIX_XAIX_SHA224256_SMALL_SIGMA_1(Word)   \
(                                              \
  (uint32) (                                   \
            CRYPTO_XVIX_XAIX_SHA224256_ROTR(Word, 17U) ^ \
            CRYPTO_XVIX_XAIX_SHA224256_ROTR(Word, 19U) ^ \
            (uint32) ((Word) >> 10U)           \
          )                                    \
)

#if (defined CRYPTO_XVIX_XAIX_SHA384512_ROUNDS)
#error CRYPTO_XVIX_XAIX_SHA384512_ROUNDS is already defined
#endif
/** \brief The number of rounds performed by the SHA-384 or SHA-512 algorithm*/
#define CRYPTO_XVIX_XAIX_SHA384512_ROUNDS 80U

/*--Crypto_xVIx_xAIx_SHAProcessInputStateType----------------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_SHA_S_PROCESS_IDLE)
#error CRYPTO_XVIX_XAIX_SHA_S_PROCESS_IDLE is already defined
#endif
/** \brief The Idle state of Crypto_xVIx_xAIx_SHAProcessInput*/
#define CRYPTO_XVIX_XAIX_SHA_S_PROCESS_IDLE 0U

#if (defined CRYPTO_XVIX_XAIX_SHA_S_PROCESS)
#error CRYPTO_XVIX_XAIX_SHA_S_PROCESS is already defined
#endif
/** \brief The Process state of Crypto_xVIx_xAIx_SHAProcessInput*/
#define CRYPTO_XVIX_XAIX_SHA_S_PROCESS 1U

#if (defined CRYPTO_XVIX_XAIX_SHA_S_PROCESS_COPY1)
#error CRYPTO_XVIX_XAIX_SHA_S_PROCESS_COPY1 is already defined
#endif
/** \brief The first Copy state of Crypto_xVIx_xAIx_SHAProcessInput*/
#define CRYPTO_XVIX_XAIX_SHA_S_PROCESS_COPY1 2U

#if (defined CRYPTO_XVIX_XAIX_SHA_S_PROCESS_COMPRESS1)
#error CRYPTO_XVIX_XAIX_SHA_S_PROCESS_COMPRESS1 is already defined
#endif
/** \brief The fist Compress state of Crypto_xVIx_xAIx_SHAProcessInput*/
#define CRYPTO_XVIX_XAIX_SHA_S_PROCESS_COMPRESS1 3U

#if (defined CRYPTO_XVIX_XAIX_SHA_S_PROCESS_COMPRESS2)
#error CRYPTO_XVIX_XAIX_SHA_S_PROCESS_COMPRESS2 is already defined
#endif
/** \brief The second Compress state of Crypto_xVIx_xAIx_SHAProcessInput*/
#define CRYPTO_XVIX_XAIX_SHA_S_PROCESS_COMPRESS2 4U

#if (defined CRYPTO_XVIX_XAIX_SHA_S_PROCESS_COMPRESS3)
#error CRYPTO_XVIX_XAIX_SHA_S_PROCESS_COMPRESS3 is already defined
#endif
/** \brief The third Compress state of Crypto_xVIx_xAIx_SHAProcessInput*/
#define CRYPTO_XVIX_XAIX_SHA_S_PROCESS_COMPRESS3 5U

#if (defined CRYPTO_XVIX_XAIX_SHA_S_PROCESS_COPY2)
#error CRYPTO_XVIX_XAIX_SHA_S_PROCESS_COPY2 is already defined
#endif
/** \brief The second Copy state of Crypto_xVIx_xAIx_SHAProcessInput*/
#define CRYPTO_XVIX_XAIX_SHA_S_PROCESS_COPY2 6U

/*--Crypto_xVIx_xAIx_SHAStateType----------------------------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_SHA_S_IDLE)
#error CRYPTO_XVIX_XAIX_SHA_S_IDLE is already defined
#endif
/** \brief The Idle state of the SHA computation*/
#define CRYPTO_XVIX_XAIX_SHA_S_IDLE 0U

#if (defined CRYPTO_XVIX_XAIX_SHA_S_INIT)
#error CRYPTO_XVIX_XAIX_SHA_S_INIT is already defined
#endif
/** \brief The Init state of the SHA computation*/
#define CRYPTO_XVIX_XAIX_SHA_S_INIT 1U

#if (defined CRYPTO_XVIX_XAIX_SHA_S_INITIALIZED)
#error CRYPTO_XVIX_XAIX_SHA_S_INITIALIZED is already defined
#endif
/** \brief The Initialized state of the SHA computation*/
#define CRYPTO_XVIX_XAIX_SHA_S_INITIALIZED 2U

#if (defined CRYPTO_XVIX_XAIX_SHA_S_UPDATE)
#error CRYPTO_XVIX_XAIX_SHA_S_UPDATE is already defined
#endif
/** \brief The Update state of the SHA computation*/
#define CRYPTO_XVIX_XAIX_SHA_S_UPDATE 3U

#if (defined CRYPTO_XVIX_XAIX_SHA_S_FINISH)
#error CRYPTO_XVIX_XAIX_SHA_S_FINISH is already defined
#endif
/** \brief The Finish state of the SHA computation*/
#define CRYPTO_XVIX_XAIX_SHA_S_FINISH 4U

#if (defined CRYPTO_XVIX_XAIX_SHA_S_FINISH_UPDATE1)
#error CRYPTO_XVIX_XAIX_SHA_S_FINISH_UPDATE1 is already defined
#endif
/** \brief The Finish Update1 state of the SHA computation*/
#define CRYPTO_XVIX_XAIX_SHA_S_FINISH_UPDATE1 5U

#if (defined CRYPTO_XVIX_XAIX_SHA_S_FINISH_UPDATE2)
#error CRYPTO_XVIX_XAIX_SHA_S_FINISH_UPDATE2 is already defined
#endif
/** \brief The Finish Update2 state of the SHA computation*/
#define CRYPTO_XVIX_XAIX_SHA_S_FINISH_UPDATE2 6U

#if (defined YPTO_SHA_S_FINISH_EXPORT)
#error YPTO_SHA_S_FINISH_EXPORT is already defined
#endif
/** \brief The Finish Export state of the SHA computation*/
#define CRYPTO_XVIX_XAIX_SHA_S_FINISH_EXPORT 7U

/*--Crypto_xVIx_xAIx_SHACompressStateType--------------------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_IDLE)
#error CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_IDLE is already defined
#endif
/** \brief The Idle state of SHACompress*/
#define CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_IDLE 0U

#if (defined CRYPTO_XVIX_XAIX_SHA_S_COMPRESS)
#error CRYPTO_XVIX_XAIX_SHA_S_COMPRESS is already defined
#endif
/** \brief The Compress state of SHACompress*/
#define CRYPTO_XVIX_XAIX_SHA_S_COMPRESS 1U

#if (defined CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_PRECSMC)
#error CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_PRECSMC is already defined
#endif
/** \brief The Precsmc state of SHACompress*/
#define CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_PRECSMC 2U

#if (defined CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_TRANSFORM1)
#error CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_TRANSFORM1 is already defined
#endif
/** \brief The Transform1 state of SHACompress*/
#define CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_TRANSFORM1 3U

#if (defined CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_EXPANSION)
#error CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_EXPANSION is already defined
#endif
/** \brief The Expansion state of SHACompress*/
#define CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_EXPANSION 4U

#if (defined CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_TRANSFORM2)
#error CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_TRANSFORM2 is already defined
#endif
/** \brief The Transform2 state of SHACompress*/
#define CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_TRANSFORM2 5u

#if (defined CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_TRANSFORM3)
#error CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_TRANSFORM3 is already defined
#endif
/** \brief The Transform3 state of SHACompress*/
#define CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_TRANSFORM3 6U

#if (defined CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_EXPORT)
#error CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_EXPORT is already defined
#endif
/** \brief The Export state of SHACompress*/
#define CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_EXPORT 7U

#if (defined CRYPTO_XVIX_XAIX_SHA224256_CH)
#error CRYPTO_XVIX_XAIX_SHA224256_CH is already defined
#endif
/** \brief The function Ch(x, y, z) from the description of SHA-256 in chapter 4.1.2
 **        of the standard FIPS 180-2
 **/
#define CRYPTO_XVIX_XAIX_SHA224256_CH(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))

#if (defined CRYPTO_XVIX_XAIX_SHA224256_MAJ)
#error CRYPTO_XVIX_XAIX_SHA224256_MAJ is already defined
#endif
/** \brief The function Maj(x, y, z) from the description of SHA-256 in chapter 4.1.2
 **        of the standard FIPS 180-2
 **/
#define CRYPTO_XVIX_XAIX_SHA224256_MAJ(x, y, z) ((((x) | (y)) & (z)) | ((x) & (y)))

#if (defined CRYPTO_XVIX_XAIX_SHA224256_READ_WORD)
#error CRYPTO_XVIX_XAIX_SHA224256_READ_WORD is already defined
#endif
/** \brief Converts a byte array to a word
 **/
#define CRYPTO_XVIX_XAIX_SHA224256_READ_WORD(Data, BufferIndex)         \
      (uint32)((uint32)Data[(uint8)(BufferIndex)            ] << 24U) | \
      (uint32)((uint32)Data[(uint8)(BufferIndex) + (uint8)1U] << 16U) | \
      (uint32)((uint32)Data[(uint8)(BufferIndex) + (uint8)2U] <<  8U) | \
               (uint32)Data[(uint8)(BufferIndex) + (uint8)3U];          \

#if (defined CRYPTO_XVIX_XAIX_SHA224256_CALCULATE_SCHEDULE_WORD)
#error CRYPTO_XVIX_XAIX_SHA224256_CALCULATE_SCHEDULE_WORD is already defined
#endif
/** \brief Calculates a SHA-256 message schedule word, as described in chapter 6.2.2
 **        of the standards FIPS 180-2
 **/
#define CRYPTO_XVIX_XAIX_SHA224256_CALCULATE_SCHEDULE_WORD(WordPos)                                           \
   do                                                                                                         \
   {                                                                                                          \
      Crypto_xVIx_xAIx_SHA2Ctx.W[(WordPos)] =                                                                 \
        CRYPTO_XVIX_XAIX_SHA224256_SMALL_SIGMA_1(Crypto_xVIx_xAIx_SHA2Ctx.W[  (uint8)((WordPos) + (uint8)14U) \
                                                                            & (uint8)0x0FU                    \
                                                                           ]                                  \
                                                )                                                             \
      + Crypto_xVIx_xAIx_SHA2Ctx.W[(uint8)((WordPos) + (uint8)9U) & (uint8)0x0FU]                             \
      + CRYPTO_XVIX_XAIX_SHA224256_SMALL_SIGMA_0(Crypto_xVIx_xAIx_SHA2Ctx.W[  (uint8)((WordPos) +  (uint8)1U) \
                                                                            & (uint8)0x0FU                    \
                                                                           ]                                  \
                                                )                                                             \
      + Crypto_xVIx_xAIx_SHA2Ctx.W[(WordPos)];                                                                \
   } while (0)

#if (defined CRYPTO_XVIX_XAIX_SHA224256_ROUND_OPERATION)
#error CRYPTO_XVIX_XAIX_SHA224256_ROUND_OPERATION is already defined
#endif
/** \brief Performs a SHA-256 round operation
 **/
#define CRYPTO_XVIX_XAIX_SHA224256_ROUND_OPERATION(a, b, c, d, e, f, g, h, i)               \
   do                                                                                       \
   {                                                                                        \
      Crypto_xVIx_xAIx_SHA2Ctx.T1[0U] =   (h)                                               \
                                        + CRYPTO_XVIX_XAIX_SHA224256_CAPITAL_SIGMA_1((e))   \
                                        + CRYPTO_XVIX_XAIX_SHA224256_CH((e), (f), (g))      \
                                        + Crypto_xVIx_xAIx_SHA224256K[(i)]                  \
                                        + Crypto_xVIx_xAIx_SHA2Ctx.W[((i) & (uint8)0x0FU)]; \
      Crypto_xVIx_xAIx_SHA2Ctx.T2[0U] =   CRYPTO_XVIX_XAIX_SHA224256_CAPITAL_SIGMA_0((a))   \
                                        + CRYPTO_XVIX_XAIX_SHA224256_MAJ((a), (b), (c));    \
      (d) += Crypto_xVIx_xAIx_SHA2Ctx.T1[0U];                                               \
      (h) = Crypto_xVIx_xAIx_SHA2Ctx.T1[0U] + Crypto_xVIx_xAIx_SHA2Ctx.T2[0U];              \
   } while (0)

/*==[Types]=======================================================================================*/

/** \brief Configuration type for SHA
 *
 * \var Crypto_xVIx_xAIx_SHAConfigType::AlgorithmIdLength
 *  \brief The length of the algorithm identifier for the configured algorithm
 *
 * \var Crypto_xVIx_xAIx_SHAConfigType::HashLength
 *  \brief The length of the Hash algorithm
 *
 * \var Crypto_xVIx_xAIx_SHAConfigType::ObjId
 *  \brief The driver object ID
 *
 * \var Crypto_xVIx_xAIx_SHAConfigType::Job
 *  \brief The processed job
 *
 * \var Crypto_xVIx_xAIx_SHAConfigType::AlgorithmId
 *  \brief The algorithm identifier for the configured algorithm
 *
 * \var Crypto_xVIx_xAIx_SHAConfigType::Crypto_Generic_Callback
 *  \brief The pointer to the callback function
 *
 */
typedef struct
{
  uint32                                                         AlgorithmIdLength;
  uint32                                                         HashLength;
  uint32                                                         ObjId;
  P2VAR(Crypto_JobType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA)     Job;
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_CONST)              AlgorithmId;
  P2FUNC(void, CRYPTO_xVIx_xAIx_CODE, Crypto_Generic_Callback)
  (
    P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
    uint8                                                        Error
  );
}
Crypto_xVIx_xAIx_SHAConfigType;

/** \brief Type of functions to initialize the states*/
typedef P2FUNC(void, CRYPTO_xVIx_xAIx_CODE, Crypto_xVIx_xAIx_SHAInitFctType)
(
  void
);

/** \brief Type of compress functions
 *
 * \var uint8::Data
 *  \brief Input data to be compressed
 *
 */
typedef P2FUNC(boolean, CRYPTO_xVIx_xAIx_CODE, Crypto_xVIx_xAIx_SHACompressFctType)
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Data
);

/** \brief The internal states of Crypto_xVIx_xAIx_SHAProcessInput()*/
typedef uint8 Crypto_xVIx_xAIx_SHAProcessInputStateType;

/** \brief The internal states of the SHA computation*/
typedef uint8 Crypto_xVIx_xAIx_SHAStateType;

/** \brief The internal states of Crypto_xVIx_xAIx_SHACompress()*/
typedef uint8 Crypto_xVIx_xAIx_SHACompressStateType;

/** \struct Crypto_xVIx_xAIx_SHACtxType
 * \brief The context of the SHA hash
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::CfgPtr
 * \brief A pointer to the current SHA configuration which has to be used
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::ErrorCode
 * \brief The error code which should be returned when the current task
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::TruncationAllowed
 * \brief Is truncation of the resultError allowed or should an error be
 * returned when the Result buffer is too small
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::Result
 * \brief Pointer to the memory location in which to store the resultError
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::ResultLengthPtr
 * \brief Pointer to a variable which contains the maximal ResultError
 * length and where the length of the resultError will be stored
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::Data
 * \brief Pointer to the input data
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::DataLength
 * \brief Length of the input data in bytes
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::State
 * \brief The current state of the hash computation
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::Count
 * \brief Number of bytes already hashed
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::P
 * \brief Pointer into data
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::Left
 * \brief Number of bytes left
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::Copy
 * \brief Number of bytes to copy
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::Offset
 * \brief Offset into temporal buffer in Ctx
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::ProcessInputState
 * \brief Internal state of the function Crypto_xVIx_xAIx_SHAProcessInput()
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::SHACompressState
 * \brief Internal state of the function Crypto_xVIx_xAIx_SHACompress()
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::Length
 * \brief 64-bit representation of length of data
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::SHAPad
 * \brief Buffer to store the padded input data
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::Partial
 * \brief Input data to be hashed
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::Internalstate
 * \brief The internal state of the hash computation, i.e. the hash
 *  value of the data stored so far.
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::W
 * \brief Internal copy of the user data
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::A
 * \brief Variable to hold the internal state
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::B
 * \brief Variable to hold the internal state
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::C
 * \brief Variable to hold the internal state
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::D
 * \brief Variable to hold the internal state
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::E
 * \brief Variable to hold the internal state
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::F
 * \brief Variable to hold the internal state
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::G
 * \brief Variable to hold the internal state
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::H
 * \brief Variable to hold the internal state
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::T1
 * \brief Temporary variable
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::T2
 * \brief Temporary variable
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::I
 * \brief Loop variable
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::K
 * \brief Loop variable
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::L
 * \brief Variable for internal loop calculations
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::Job
 * \brief The processed job
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::Crypto_GenericCallack
 * \brief The pointer to the Callback Function
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::IsBusy
 * \brief MainFunction is busy
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::ResultError
 * \brief Error value of MainFunction
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::ObjId
 * \brief The Driver object ID
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::CurrentMode
 * \brief The current processing mode
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::CurrentWordSize
 * \brief The current word size
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::CancelFlag
 * \brief This Flag indicates if processing shall be canceled.
 *
 * \var Crypto_xVIx_xAIx_SHACtxType::CallbackFlag
 * \brief This Flag indicates if a callback shall be called.
 */
typedef struct
{
  P2CONST(Crypto_xVIx_xAIx_SHAConfigType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA) CfgPtr;
  Std_ReturnType                                                               ErrorCode;
  boolean                                                                      TruncationAllowed;
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                          Result;
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                         ResultLengthPtr;
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                        Data;
  uint32                                                                       DataLength;
  Crypto_xVIx_xAIx_SHAStateType                                                State;
  uint32                                                                       Count;
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                        P;
  uint32                                                                       Left;
  uint8                                                                        Copy;
  uint8                                                                        Offset;
  Crypto_xVIx_xAIx_SHAProcessInputStateType                                    ProcessInputState;
  Crypto_xVIx_xAIx_SHACompressStateType                                        SHACompressState;
#if ((CRYPTO_XVIX_XAIX_SHA2_384_ENABLED == STD_ON) || (CRYPTO_XVIX_XAIX_SHA2_512_ENABLED == STD_ON))
  uint8  Length[16U];
  uint8  SHAPad[CRYPTO_XVIX_XAIX_SHA_384_512_BW_BLOCK_LEN_BYTES];
  uint8  Partial[CRYPTO_XVIX_XAIX_SHA_384_512_BW_BLOCK_LEN_BYTES];
  uint32 Internalstate[CRYPTO_XVIX_XAIX_SHA_STATE_WORDS << 1U];
  uint32 W[CRYPTO_XVIX_XAIX_SHA_384_512_BW_BLOCK_LEN_WORDS];
  uint32 A[2U];
  uint32 B[2U];
  uint32 C[2U];
  uint32 D[2U];
  uint32 E[2U];
  uint32 F[2U];
  uint32 G[2U];
  uint32 H[2U];
  uint32 T1[2U];
  uint32 T2[2U];
#elif ((CRYPTO_XVIX_XAIX_SHA2_224_ENABLED == STD_ON) || (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON))
  uint8  Length[8U];
  uint8  SHAPad[CRYPTO_XVIX_XAIX_SHA_224_256_BLOCK_LEN_BYTES];
  uint8  Partial[CRYPTO_XVIX_XAIX_SHA_224_256_BLOCK_LEN_BYTES];
  uint32 Internalstate[CRYPTO_XVIX_XAIX_SHA_STATE_WORDS];
  uint32 W[CRYPTO_XVIX_XAIX_SHA_224_256_BLOCK_LEN_WORDS];
  uint32 A[1U];
  uint32 B[1U];
  uint32 C[1U];
  uint32 D[1U];
  uint32 E[1U];
  uint32 F[1U];
  uint32 G[1U];
  uint32 H[1U];
  uint32 T1[1U];
  uint32 T2[1U];
#endif
  uint8                                                I;
  uint8                                                K;
  uint8                                                L;
  P2VAR(Crypto_JobType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA)     Job;
  P2FUNC(void, CSM_APPL_CODE, Crypto_Generic_Callback)
  (
      P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
      uint8                                            Error
  );
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)            IsBusy;
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)     ResultError;
  uint32                                               ObjId;
  Crypto_OperationModeType                             CurrentMode;
  uint8                                                CurrentWordSize;
  boolean                                              CancelFlag;
  boolean                                              CallbackFlag;
}
Crypto_xVIx_xAIx_SHACtxType;

/*==[Declaration of functions with internal linkage]==============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief Statemachine for current SHA
 *
 *         This function is a Statemachine and controls the step by step task to complete the SHA
 *         computation.
 *
 * \param[in,out] CtxPtr    A pointer to the context of the algorithm
 * \param[in] InitFct       A pointer to the Init Function
 * \param[in] CompressFct   A pointer to the Compress Function
 * \param[out] ResultError  A pointer to store the result error
 * \param[out] Busy         A pointer to store the if the Main Function is busy
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHAMainFunction
(
  P2VAR(Crypto_xVIx_xAIx_SHACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  Crypto_xVIx_xAIx_SHAInitFctType                                           InitFct,
  Crypto_xVIx_xAIx_SHACompressFctType                                       CompressFct,
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)               ResultError,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                      Busy
);

/** \brief Processes the given input data
 *
 *         This function processes the input data stored in the context of the algorithm
 *
 * \param[in,out] CtxPtr   A pointer to the context of the algorithm
 * \param[in] CompressFct  A pointer to the Compress Function
 *
 * \returns If process input has been finished or not
 * \retval TRUE   finished
 * \retval FALSE  Not finished
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHAProcessInput
(
  P2VAR(Crypto_xVIx_xAIx_SHACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  Crypto_xVIx_xAIx_SHACompressFctType                                       CompressFct
);

/** \brief Process Input state helper function to perform the state processing for the
 *         CRYPTO_XVIX_XAIX_SHA_S_PROCESS state. This helper function is implemented to
 *         reduce the complexity of the Crypto_xVIx_xAIx_SHAProcessInput function.
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
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHAProcessInputProcess
(
  P2VAR(Crypto_xVIx_xAIx_SHACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Process Input state helper function to perform the state processing for the
 *         CRYPTO_XVIX_XAIX_SHA_S_PROCESS_COPY1 state. This helper function is implemented to
 *         reduce the complexity of the Crypto_xVIx_xAIx_SHAProcessInput function.
 *
 * \param[in,out] CtxPtr   A pointer to the context of the algorithm
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHAProcessInputCopy1
(
  P2VAR(Crypto_xVIx_xAIx_SHACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Process Input state helper function to perform the state processing for the
 *         CRYPTO_XVIX_XAIX_SHA_S_PROCESS_COMPRESS1 state. This helper function is implemented to
 *         reduce the complexity of the Crypto_xVIx_xAIx_SHAProcessInput function.
 *
 *
 * \param[in,out] CtxPtr   A pointer to the context of the algorithm
 *
 * \returns If process input has been finished or not
 * \retval TRUE   finished
 * \retval FALSE  Not finished
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHAProcessInputCompress1
(
  P2VAR(Crypto_xVIx_xAIx_SHACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Process Input state helper function to perform the state processing for the
 *         CRYPTO_XVIX_XAIX_SHA_S_PROCESS_COMPRESS2 state. This helper function is implemented to
 *         reduce the complexity of the Crypto_xVIx_xAIx_SHAProcessInput function.
 *
 * \param[in,out] CtxPtr   A pointer to the context of the algorithm
 * \param[in] CompressFct  A pointer to the Compress Function
 *
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHAProcessInputCompress2
(
  P2VAR(Crypto_xVIx_xAIx_SHACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  Crypto_xVIx_xAIx_SHACompressFctType CompressFct
);

/** \brief Process Input state helper function to perform the state processing for the
 *         CRYPTO_XVIX_XAIX_SHA_S_PROCESS_COMPRESS3 state. This helper function is implemented to
 *         reduce the complexity of the Crypto_xVIx_xAIx_SHAProcessInput function.
 *
 * \param[in,out] CtxPtr   A pointer to the context of the algorithm
 * \param[in] CompressFct  A pointer to the Compress Function
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHAProcessInputCompress3
(
  P2VAR(Crypto_xVIx_xAIx_SHACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  Crypto_xVIx_xAIx_SHACompressFctType CompressFct
);

/** \brief Process Input state helper function to perform the state processing for the
 *         CRYPTO_XVIX_XAIX_SHA_S_PROCESS_COPY2 state. This helper function is implemented to
 *         reduce the complexity of the Crypto_xVIx_xAIx_SHAProcessInput function.
 *
 *
 * \param[in,out] CtxPtr   A pointer to the context of the algorithm
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHAProcessInputCopy2
(
  P2VAR(Crypto_xVIx_xAIx_SHACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

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
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHACompressStart
(
  P2VAR(Crypto_xVIx_xAIx_SHACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
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
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHAExport
(
  P2VAR(Crypto_xVIx_xAIx_SHACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Function to perform CRYPTO_XVIX_XAIX_SHA_S_INIT state processing in a single function
 *
 *     This function is implemented in order to perform the whole processing of
 *     CRYPTO_XVIX_XAIX_SHA_S_INIT state in a single function. It enables the state
 *     to have a single function call which performes its whole processing.
 *
 * \param[in,out] CtxPtr  A pointer to the context of the algorithm
 * \param[in]     InitFct Function pointer to the internal init function for the algorithm
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHASInit
(
  P2VAR(Crypto_xVIx_xAIx_SHACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  Crypto_xVIx_xAIx_SHAInitFctType                                 InitFct
);

/** \brief Function to perform CRYPTO_XVIX_XAIX_SHA_S_UPDATE state processing in a single function
 *
 *     This function is implemented in order to perform the whole processing of
 *     CRYPTO_XVIX_XAIX_SHA_S_UPDATE state in a single function. It enables the state
 *     to have a single function call which performes its whole processing.
 *
 * \param[in,out] CtxPtr      A pointer to the context of the algorithm
 * \param[in]     CompressFct Type of compress functions
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHASUpdate
(
  P2VAR(Crypto_xVIx_xAIx_SHACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  Crypto_xVIx_xAIx_SHACompressFctType                             CompressFct
);

/** \brief Function to perform CRYPTO_XVIX_XAIX_SHA_S_FINISH state processing in a single function
 *
 *     This function is implemented in order to perform the whole processing of
 *     CRYPTO_XVIX_XAIX_SHA_S_FINISH state in a single function. It enables the state
 *     to have a single function call which performes its whole processing.
 *
 * \param[in,out] CtxPtr  A pointer to the context of the algorithm
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHASFinish
(
  P2VAR(Crypto_xVIx_xAIx_SHACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

/** \brief Function to perform the callback and job cancellation.
 *
 *     This function is implemented in order to perform the job cancellation
 *     and callback functionality is a single function call, instead of having it in the
 *     Crypto_xVIx_xAIx_SHAMainFunction.
 *
 * \param[in,out] CtxPtr  A pointer to the context of the algorithm
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHACallbackCancel
(
  P2VAR(Crypto_xVIx_xAIx_SHACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
);

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*--SHA2 Helper-----------------------------------------------------------------------------------*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>
#if (CRYPTO_XVIX_XAIX_SHA2_224_ENABLED == STD_ON)

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
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2224HelperSynchronousMainCall
(
  void
);

#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_224_ENABLED == STD_ON) */
#if (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON)
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
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2256HelperSynchronousMainCall
(
  void
);

/** \brief The Callback for using hash inside another algorithm
 *
 *         This function gets called from another primitive to step into the next step
 *
 * \param[in,out] Job      A pointer to the current Job
 * \param[in] ResultError  The result of the process
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2256HelperCallbackNotification
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  Std_ReturnType                                               ResultError
);

#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON) */
#if (CRYPTO_XVIX_XAIX_SHA2_384_ENABLED == STD_ON)

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
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2384HelperSynchronousMainCall
(
  void
);

#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_384_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_SHA2_512_ENABLED == STD_ON)

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
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2512HelperSynchronousMainCall
(
  void
);

#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_512_ENABLED == STD_ON) */
#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*--SHA2------------------------------------------------------------------------------------------*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#if (CRYPTO_XVIX_XAIX_SHA2_224_ENABLED == STD_ON)

/** \brief AL Callback for other primitives
 *
 *         This function is a callbackfunction for other primitives
 *
 * \param[in,out] Job  A pointer to the current Job
 * \param[in] Result   The result of the process
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_224_NOT_SET_CallBack
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint8                                                        Result
);

#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_224_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON)

/** \brief AL Callback for other primitives
 *
 *         This function is a callbackfunction for other primitives
 *
 * \param[in,out] Job  A pointer to the current Job
 * \param[in] Result   The result of the process
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_CallBack
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint8                                                        Result
);

#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_SHA2_384_ENABLED == STD_ON)

/** \brief AL Callback for other primitives
 *
 *         This function is a callbackfunction for other primitives
 *
 * \param[in,out] Job  A pointer to the current Job
 * \param[in] Result   The result of the process
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_384_NOT_SET_CallBack
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint8                                                        Result
);

#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_384_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_SHA2_512_ENABLED == STD_ON)

/** \brief AL Callback for other primitives
 *
 *         This function is a callbackfunction for other primitives
 *
 * \param[in,out] Job  A pointer to the current Job
 * \param[in] Result   The result of the process
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_512_NOT_SET_CallBack
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint8                                                        Result
);

#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_512_ENABLED == STD_ON) */

/** \brief Initialize Processing of hash request
 *
 *         This function initializes the internal states to prepare for a hash calculation
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2Init
(
  void
);

#if ((CRYPTO_XVIX_XAIX_SHA2_224_ENABLED == STD_ON) || (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON))

/** \brief Processes one block of data to hash
 *
 *         This function compresses one block of data to hash according to the rules
 *         of SHA-2 and updates the internal state accordingly. The function
 *         Crypto_xVIx_xAIx_SHACompressStart() has to be called before the first call to this
 *         function.
 *
 * \param[in] Data  A pointer to a block of data of size
 *                  ::CRYPTO_XVIX_XAIX_SHA2_BLOCK_LEN_BYTES bytes which has to be processed
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
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA224256Compress
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Data
);
#endif /* #if ((CRYPTO_XVIX_XAIX_SHA2_224_ENABLED == STD_ON) ||
               (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON)) */

#if ((CRYPTO_XVIX_XAIX_SHA2_384_ENABLED == STD_ON) || (CRYPTO_XVIX_XAIX_SHA2_512_ENABLED == STD_ON))

/** \brief Addition modulo 2^64 where an uint64 is implemented as an array of two uint32.
 *
 * \param[out] Ans  A pointer to the result
 * \param[in] N     A pointer to first input
 * \param[in] M     A pointer to second input
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_Add
(
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Ans,
  P2CONST(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) N,
  P2CONST(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) M
);

/** \brief Rotate a 32-bit word by a given amount of bits to the right
 *
 * \param[in,out] Word  A pointer to the word to be changed
 * \param[in] Offset1   The amount of bits to be rotated by
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA384512RotR
(
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Word,
  uint32                                               Offset1
);

/** \brief The function (in LaTex notation) $\SHR^{\{384 or 512\}}_0(x)$ from the
 *         description of SHA-384 or SHA-512 in chapter 4.1.2 of the standard FIPS 180-2
 *
 * \param[in,out] Word  A pointer to the word to be changed
 * \param[in] Offset2   The amout of bits for the change
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA384512ShR
(
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Word,
  uint32                                               Offset2
);

/** \brief The function (in LaTex notation) $\Sigma^{\{384 or 512\}}_0(x)$ from the
 *         description of SHA-384 or SHA-512 in chapter 4.1.2 of the standard FIPS 180-2
 *
 * \param[in,out] Word  A pointer to the word to be changed
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA384512CapitalSigma_0
(
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Word
);

/** \brief The function (in LaTex notation) $\Sigma^{\{384 or 512\}}_1(x)$ from the
 *         description of SHA-384 or SHA-512 in chapter 4.1.2 of the standard FIPS 180-2
 *
 * \param[in,out] Word  A pointer to the word to be changed
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA384512CapitalSigma_1
(
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Word
);

/** \brief The function (in LaTex notation) $\sigma^{\{384 or 512\}}_0(x)$ from the
 *         description of SHA-384 or SHA-512 in chapter 4.1.2 of the standard FIPS 180-2
 *
 * \param[in/out] Word  A pointer to the word to be changed
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA384512SmallSigma_0
(
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Word
);

/** \brief The function (in LaTex notation) $\sigma^{\{384 or 512\}}_1(x)$ from the
 *         description of SHA-384 or SHA-512 in chapter 4.1.2 of the standard FIPS 180-2
 *
 * \param[in,out] Word  A pointer to the word to be changed
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA384512SmallSigma_1
(
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Word
);

/** \brief Helper function for Crypto_xVIx_xAIx_SHA384512Compress
 *
 *         This function perform the computations in the state CRYPTO_XVIX_XAIX_SHA_S_COMPRESS.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA384512StateCompress
(
  void
);

/** \brief Helper function for Crypto_xVIx_xAIx_SHA384512Compress
 *
 *         This function perform the computations in the state CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_PRECSMC.
 *
 * \param[in] Data  A pointer to a block of data of size
 *                  ::CRYPTO_XVIX_XAIX_SHA2_BLOCK_LEN_BYTES bytes which has to be processed
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA384512StateCompressPrecsmc
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Data
);

/** \brief Helper function for Crypto_xVIx_xAIx_SHA384512Compress
 *
 *         This function perform the computations in the state CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_EXPANSION.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA384512StateCompressExpansion
(
  void
);

/** \brief Helper function for Crypto_xVIx_xAIx_SHA384512Compress
 *
 *         This function perform the computations in the state CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_TRANSFORM2.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA384512StateCompressTransform2
(
  void
);

/** \brief Helper function for Crypto_xVIx_xAIx_SHA384512Compress
 *
 *         This function perform the computations in the state CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_TRANSFORM3.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA384512StateCompressTransform3
(
  void
);

/** \brief Helper function for Crypto_xVIx_xAIx_SHA384512Compress
 *
 *         This function perform the computations in the state CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_EXPORT.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA384512StateCompressExport
(
  void
);

/** \brief Processes one block of data to hash
 *
 *         This function compresses one block of data to hash according to the rules
 *         of SHA-2 and updates the internal state accordingly. The function
 *         Crypto_xVIx_xAIx_SHACompressStart() has to be called before the first call to this
 *         function.
 *
 * \param[in] Data  A pointer to a block of data of size
 *                  ::CRYPTO_XVIX_XAIX_SHA2_BLOCK_LEN_BYTES bytes which has to be processed
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
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA384512Compress
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Data
);

#endif /* #if ((CRYPTO_XVIX_XAIX_SHA2_384_ENABLED == STD_ON) ||
               (CRYPTO_XVIX_XAIX_SHA2_512_ENABLED == STD_ON)) */

/** \brief This is the internal Cancel-Function for Sha2
 *
 * This function actually performs the request for service HASH with algorithm family SHA" and.
 * It cancels the Job and removes it from the queue.
 *
 *
 * \returns  void
 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2Cancel
(
  void
);

/** \brief Start SHA2 computation
 *
 *         This function requests the start of the SHA computation for the
 *         given configuration. The start is performed in Crypto_xVIx_xAIx_SHA2MainFunction()
 *
 * \param[in] CfgPtr  A pointer to the configuration for which the start of the SHA
 *                    computation should be requested.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2Start
(
  P2CONST(void, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CfgPtr
);

/** \brief Update SHA2 computation.
 *
 *         This function requests the update of the SHA computation for the
 *         given data. The update is performed in Crypto_xVIx_xAIx_SHA2MainFunction().
 *
 * \param[in] DataPtr     A pointer to the start of an array which contains a part of the
 *                        data for which the SHA digest should be created.
 * \param[in] DataLength  The amount of bytes of data
 *
 * \returns  Error code
 * \retval E_OK           If the update was successfully requested.
 * \retval CRYPTO_E_BUSY  If the main function is processing a requested service at the moment
 * \retval _E_NOT_OK      If no SHA computation has been started via Crypto_xVIx_xAIx_SHA2Start(), yet.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2Update
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) DataPtr,
  uint32                                                DataLength
);

/** \brief Finish SHA2 computation
 *
 *         This function requests the finishing of the SHA computation and
 *         the storing of the hash digest in the given resultError buffer. The finishing
 *         is performed in Crypto_xVIx_xAIx_SHA2MainFunction().
 *
 * \param[out] ResultErrorPtr           A pointer to the start of a buffer
 *                                      where the hash digest should be stored
 * \param[in,out] ResultErrorLengthPtr  A pointer to a variable which contains the maximal allowed
 *                                      length in bytes for the hash and where the actual length in
 *                                      bytes of the hash should be stored
 * \param[in] TruncationAllowed         If this flag is ::TRUE and the hash digest is longer than
 *                                      the given resultError buffer, the hash is truncated to the
 *                                      buffer length. If this flag is ::FALSE and the hash digest
 *                                      is longer than the given resultError buffer, an error code
 *                                      of ::CRYPTO_E_SMALL_BUFFER is returned
 *
 * \returns Error code
 * \retval E_OK           If the finish was successfully requested.
 * \retval CRYPTO_E_BUSY  If the main function is processing a requested service at the moment
 * \retval E_NOT_OK       If no SHA computation has been started via Crypto_xVIx_xAIx_SHA2Start(), yet.
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2Finish
(
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  ResultErrorPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultErrorLengthPtr,
  boolean                                              TruncationAllowed
);

/** \brief Perform the SHA2 tasks
 *
 *         This function performs the actual SHA computation tasks which
 *         have been requested by the service functions. When a task has finished, the
 *         function CRYPTO_HashServiceCallbackNotification() is called to
 *         inform the CRYPTO of the resultError. If the complete SHA2 computation has
 *         finished, additionally the function CRYPTO_HashServiceFinishNotification() is
 *         called.
 *
 * \param[out] ResultError  A pointer to store the result
 * \param[out} Busy         A pointer to store if the Main Function is busy
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2MainFunction
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultError,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
);

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Constants with internal linkage]=============================================================*/

/*--SHA2------------------------------------------------------------------------------------------*/

#define CRYPTO_xVIx_xAIx_START_SEC_CONST_32
#include <Crypto_xVIx_xAIx_MemMap.h>

#if ((CRYPTO_XVIX_XAIX_SHA2_224_ENABLED == STD_ON) || (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON))

/* !LINKSTO EB_Crypto_CryptAlgos_0383,1 */
/* !LINKSTO EB_Crypto_CryptAlgos_0373,1 */
/** \brief Array "K" of SHA-256 or SHA-224 computation
 *
 * For each computation round i, the SHA-256 or SHA-224 computation uses a constant K_i
 * in the computation of T_1. This array contains those constants
 */
static CONST(uint32, CRYPTO_xVIx_xAIx_CONST) Crypto_xVIx_xAIx_SHA224256K[CRYPTO_XVIX_XAIX_SHA224256_ROUNDS] =
{
  0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL,
  0x3956c25bUL, 0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL,
  0xd807aa98UL, 0x12835b01UL, 0x243185beUL, 0x550c7dc3UL,
  0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL, 0xc19bf174UL,
  0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL,
  0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL,
  0x983e5152UL, 0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL,
  0xc6e00bf3UL, 0xd5a79147UL, 0x06ca6351UL, 0x14292967UL,
  0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL, 0x53380d13UL,
  0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
  0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL,
  0xd192e819UL, 0xd6990624UL, 0xf40e3585UL, 0x106aa070UL,
  0x19a4c116UL, 0x1e376c08UL, 0x2748774cUL, 0x34b0bcb5UL,
  0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL, 0x682e6ff3UL,
  0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
  0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL
};

#endif /* #if ((CRYPTO_XVIX_XAIX_SHA2_224_ENABLED == STD_ON) ||
               (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON)) */

#if ((CRYPTO_XVIX_XAIX_SHA2_384_ENABLED == STD_ON) || (CRYPTO_XVIX_XAIX_SHA2_512_ENABLED == STD_ON))

/* !LINKSTO EB_Crypto_CryptAlgos_0303,1 */
/* !LINKSTO EB_Crypto_CryptAlgos_0393,1 */
/** \brief Array "K" of SHA-384 or SHA-512 computation
 *
 * For each computation round i, the SHA-384 or SHA-512 computation uses a constant K_i
 * in the computation of T_1. This array contains those constants
 */
/* Deviation MISRAC2012-1 */
static CONST(uint32, CRYPTO_xVIx_xAIx_CONST) Crypto_xVIx_xAIx_SHA384512K[CRYPTO_XVIX_XAIX_SHA384512_ROUNDS << 1U] =
{
  0xd728ae22UL, 0x428a2f98UL, 0x23ef65cdUL, 0x71374491UL,
  0xec4d3b2fUL, 0xb5c0fbcfUL, 0x8189dbbcUL, 0xe9b5dba5UL,
  0xf348b538UL, 0x3956c25bUL, 0xb605d019UL, 0x59f111f1UL,
  0xaf194f9bUL, 0x923f82a4UL, 0xda6d8118UL, 0xab1c5ed5UL,
  0xa3030242UL, 0xd807aa98UL, 0x45706fbeUL, 0x12835b01UL,
  0x4ee4b28cUL, 0x243185beUL, 0xd5ffb4e2UL, 0x550c7dc3UL,
  0xf27b896fUL, 0x72be5d74UL, 0x3b1696b1UL, 0x80deb1feUL,
  0x25c71235UL, 0x9bdc06a7UL, 0xcf692694UL, 0xc19bf174UL,
  0x9ef14ad2UL, 0xe49b69c1UL, 0x384f25e3UL, 0xefbe4786UL,
  0x8b8cd5b5UL, 0x0fc19dc6UL, 0x77ac9c65UL, 0x240ca1ccUL,
  0x592b0275UL, 0x2de92c6fUL, 0x6ea6e483UL, 0x4a7484aaUL,
  0xbd41fbd4UL, 0x5cb0a9dcUL, 0x831153b5UL, 0x76f988daUL,
  0xee66dfabUL, 0x983e5152UL, 0x2db43210UL, 0xa831c66dUL,
  0x98fb213fUL, 0xb00327c8UL, 0xbeef0ee4UL, 0xbf597fc7UL,
  0x3da88fc2UL, 0xc6e00bf3UL, 0x930aa725UL, 0xd5a79147UL,
  0xe003826fUL, 0x06ca6351UL, 0x0a0e6e70UL, 0x14292967UL,
  0x46d22ffcUL, 0x27b70a85UL, 0x5c26c926UL, 0x2e1b2138UL,
  0x5ac42aedUL, 0x4d2c6dfcUL, 0x9d95b3dfUL, 0x53380d13UL,
  0x8baf63deUL, 0x650a7354UL, 0x3c77b2a8UL, 0x766a0abbUL,
  0x47edaee6UL, 0x81c2c92eUL, 0x1482353bUL, 0x92722c85UL,
  0x4cf10364UL, 0xa2bfe8a1UL, 0xbc423001UL, 0xa81a664bUL,
  0xd0f89791UL, 0xc24b8b70UL, 0x0654be30UL, 0xc76c51a3UL,
  0xd6ef5218UL, 0xd192e819UL, 0x5565a910UL, 0xd6990624UL,
  0x5771202aUL, 0xf40e3585UL, 0x32bbd1b8UL, 0x106aa070UL,
  0xb8d2d0c8UL, 0x19a4c116UL, 0x5141ab53UL, 0x1e376c08UL,
  0xdf8eeb99UL, 0x2748774cUL, 0xe19b48a8UL, 0x34b0bcb5UL,
  0xc5c95a63UL, 0x391c0cb3UL, 0xe3418acbUL, 0x4ed8aa4aUL,
  0x7763e373UL, 0x5b9cca4fUL, 0xd6b2b8a3UL, 0x682e6ff3UL,
  0x5defb2fcUL, 0x748f82eeUL, 0x43172f60UL, 0x78a5636fUL,
  0xa1f0ab72UL, 0x84c87814UL, 0x1a6439ecUL, 0x8cc70208UL,
  0x23631e28UL, 0x90befffaUL, 0xde82bde9UL, 0xa4506cebUL,
  0xb2c67915UL, 0xbef9a3f7UL, 0xe372532bUL, 0xc67178f2UL,
  0xea26619cUL, 0xca273eceUL, 0x21c0c207UL, 0xd186b8c7UL,
  0xcde0eb1eUL, 0xeada7dd6UL, 0xee6ed178UL, 0xf57d4f7fUL,
  0x72176fbaUL, 0x06f067aaUL, 0xa2c898a6UL, 0x0a637dc5UL,
  0xbef90daeUL, 0x113f9804UL, 0x131c471bUL, 0x1b710b35UL,
  0x23047d84UL, 0x28db77f5UL, 0x40c72493UL, 0x32caab7bUL,
  0x15c9bebcUL, 0x3c9ebe0aUL, 0x9c100d4cUL, 0x431d67c4UL,
  0xcb3e42b6UL, 0x4cc5d4beUL, 0xfc657e2aUL, 0x597f299cUL,
  0x3ad6faecUL, 0x5fcb6fabUL, 0x4a475817UL, 0x6c44198cUL
};

#endif /* #if ((CRYPTO_XVIX_XAIX_SHA2_384_ENABLED == STD_ON) ||
               (CRYPTO_XVIX_XAIX_SHA2_512_ENABLED == STD_ON)) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_CONST_32
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Variables with internal linkage]=============================================================*/

/*--SHA2------------------------------------------------------------------------------------------*/

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_INIT_BOOLEAN
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief States if Helper is busy */
static VAR(boolean, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_HelperBusy = FALSE;

#if (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON)
/** \brief Flag for the Helper Callback Notification */
static volatile VAR(boolean, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_HelperHashFlgCallbackNotification = FALSE;
#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_INIT_BOOLEAN
#include <Crypto_xVIx_xAIx_MemMap.h>

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_INIT_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

#if (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON)
/** \brief The PrimitiveInfo for the job used in the Sha helper. */
static VAR(Crypto_PrimitiveInfoType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_HelperHashPrimitiveInfo =
{
  32U,                       /* resultLength */
  CRYPTO_ENCRYPT,            /* service */
  {
    CRYPTO_ALGOFAM_SHA2_256, /* family */
    CRYPTO_ALGOFAM_NOT_SET,  /* secondaryFamily */
    0U,                      /* keyLength */
    CRYPTO_ALGOMODE_NOT_SET  /* mode*/
  }                          /* algorithm */
};

/** \brief The JobPrimitiveInfo for the job used in the Sha helper. */
static VAR(Crypto_JobPrimitiveInfoType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_HelperHashJobPrimitiveInfo =
{
  0U,                                    /* callbackId */
  &Crypto_xVIx_xAIx_HelperHashPrimitiveInfo, /* primitiveInfo */
  0U,                                    /* secureCounterId */
  0U,                                    /* cryIfKeyId */
  CRYPTO_PROCESSING_SYNC,                /* processingType */
  FALSE                                  /* callbackUpdateNotification*/
};

/** \brief The JobInfo for the job used in the Sha helper. */
static VAR(Crypto_JobInfoType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_HelperHashJobInfo =
{
  0U, /* jobId */
  0U  /* jobPriority*/
};
#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief Result of the Helper */
static VAR(Std_ReturnType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_HelperResult;

#if (CRYPTO_XVIX_XAIX_SHA2_224_ENABLED == STD_ON)
/** \brief Configuration for SHA2 224 */
static VAR(Crypto_xVIx_xAIx_SHAConfigType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_AL_HASH_SHA2_224_NOT_SET_Config;
#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_224_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON)
/** \brief Configuration for SHA2 256 */
static VAR(Crypto_xVIx_xAIx_SHAConfigType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_Config;
#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_SHA2_384_ENABLED == STD_ON)
/** \brief Configuration for SHA2 384 */
static VAR(Crypto_xVIx_xAIx_SHAConfigType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_AL_HASH_SHA2_384_NOT_SET_Config;
#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_384_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_SHA2_512_ENABLED == STD_ON)
/** \brief Configuration for SHA2 512 */
static VAR(Crypto_xVIx_xAIx_SHAConfigType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_AL_HASH_SHA2_512_NOT_SET_Config;
#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_512_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON)
/** \brief Synchronous mode callback notification return value */
static volatile VAR(Std_ReturnType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_HelperHashRetCallbackNotification;
#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON) */

/** \brief SHA2 Ctx
 *
 * The context variable which is used to store the internal state of the
 * SHA2 computation. This context is used for SHA-224 and SHA-256.
 */
static VAR(Crypto_xVIx_xAIx_SHACtxType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_SHA2Ctx;

#if (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON)
/** \brief Callback for primary primitive. */
static P2FUNC(void, CRYPTO_xVIx_xAIx_CODE, Crypto_xVIx_xAIx_SHA_CallBack)
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint8                                              Error
);
#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Constants with external linkage]=============================================================*/

/*==[Variables with external linkage]=============================================================*/

/*==[Definition of functions with external linkage]===============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*--SHA-Common------------------------------------------------------------------------------------*/

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_NOT_SET_Reset
(
  void
)
{
  SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
  TS_MemBZero(&Crypto_xVIx_xAIx_SHA2Ctx, sizeof(Crypto_xVIx_xAIx_SHA2Ctx));
  SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
}

/*--SHA2 Helper-----------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_SHA2_224_ENABLED == STD_ON)

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2224HelperStart
(
  const uint32                                                   ObjId,
  P2FUNC(void, CRYPTO_xVIx_xAIx_CODE, Crypto_Generic_Callback)
  (
    P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
    uint8                                                        Error
  ),
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Job,
  boolean                                                        ProcessingType
)
{
  Crypto_xVIx_xAIx_AL_HASH_SHA2_224_NOT_SET_Config.HashLength = 28U;
  Crypto_xVIx_xAIx_AL_HASH_SHA2_224_NOT_SET_Config.Job = Job;
  Crypto_xVIx_xAIx_AL_HASH_SHA2_224_NOT_SET_Config.ObjId = ObjId;
  Crypto_xVIx_xAIx_AL_HASH_SHA2_224_NOT_SET_Config.Crypto_Generic_Callback =
  &Crypto_xVIx_xAIx_AL_HASH_SHA2_224_NOT_SET_CallBack;
  Crypto_xVIx_xAIx_AL_HASH_SHA2_224_NOT_SET_Config.Crypto_Generic_Callback =
  Crypto_Generic_Callback;
  Crypto_xVIx_xAIx_SHA2Start(&Crypto_xVIx_xAIx_AL_HASH_SHA2_224_NOT_SET_Config);
  if(CRYPTO_PROCESSING_SYNC == ProcessingType)
  {
    /* This function can not fail for the first state */
    (void)Crypto_xVIx_xAIx_SHA2224HelperSynchronousMainCall();
  }
}

FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2224HelperUpdate
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) DataPtr,
  uint32                                                DataLength,
  boolean                                               ProcessingType
)
{
  Std_ReturnType RetVal;
  RetVal = Crypto_xVIx_xAIx_SHA2Update(DataPtr, DataLength);
  if((CRYPTO_PROCESSING_SYNC == ProcessingType) && (E_OK == RetVal))
  {
    RetVal = Crypto_xVIx_xAIx_SHA2224HelperSynchronousMainCall();
  }
  return RetVal;
}

FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2224HelperFinish
(
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  ResultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultLengthPtr,
  boolean                                              TruncationAllowed,
  boolean                                              ProcessingType
)
{
  Std_ReturnType RetVal;
  RetVal = Crypto_xVIx_xAIx_SHA2Finish(ResultPtr, ResultLengthPtr, TruncationAllowed);
  if((CRYPTO_PROCESSING_SYNC == ProcessingType) && (E_OK == RetVal))
  {
    RetVal = Crypto_xVIx_xAIx_SHA2224HelperSynchronousMainCall();
  }
  return RetVal;
}

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2224HelperMainFunction
(
  void
)
{
  Crypto_xVIx_xAIx_SHA2MainFunction(&Crypto_xVIx_xAIx_HelperResult, &Crypto_xVIx_xAIx_HelperBusy);
}

#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_224_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON)

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2256HelperStart
(
  const uint32                                                   ObjId,
  P2FUNC(void, CRYPTO_xVIx_xAIx_CODE, Crypto_Generic_Callback)
  (
    P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
    uint8                                                        Error
  ),
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Job,
  boolean                                                        ProcessingType
)
{
  Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_Config.HashLength = 32U;
  Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_Config.Job = Job;
  Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_Config.ObjId = ObjId;
  Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_Config.Crypto_Generic_Callback =
    &Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_CallBack;
  Crypto_xVIx_xAIx_SHA_CallBack = Crypto_Generic_Callback;

  Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_Config.Crypto_Generic_Callback = Crypto_Generic_Callback;
  Crypto_xVIx_xAIx_SHA2Start(&Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_Config);
  if(CRYPTO_PROCESSING_SYNC == ProcessingType)
  {
    /* This function can not fail for the first state */
    (void)Crypto_xVIx_xAIx_SHA2256HelperSynchronousMainCall();
  }
}

FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2256HelperUpdate
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) DataPtr,
  uint32                                                DataLength,
  boolean                                               ProcessingType
)
{
  Std_ReturnType RetVal;
  RetVal = Crypto_xVIx_xAIx_SHA2Update(DataPtr, DataLength);
  if((CRYPTO_PROCESSING_SYNC == ProcessingType) && (E_OK == RetVal))
  {
    RetVal = Crypto_xVIx_xAIx_SHA2256HelperSynchronousMainCall();
  }
  return RetVal;
}

FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2256HelperFinish
(
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  ResultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultLengthPtr,
  boolean                                              TruncationAllowed,
  boolean                                              ProcessingType
)
{
  Std_ReturnType RetVal;

  RetVal = Crypto_xVIx_xAIx_SHA2Finish(ResultPtr, ResultLengthPtr, TruncationAllowed);
  if((CRYPTO_PROCESSING_SYNC == ProcessingType) && (E_OK == RetVal))
  {
    RetVal = Crypto_xVIx_xAIx_SHA2256HelperSynchronousMainCall();
  }
  return RetVal;
}

FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2256HelperSyncSingleCall
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) InputPtr,
  uint32 InputLength,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) OutputPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) OutputLengthPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint32 ObjId = 0U;

  Crypto_JobType Job =
  {
    0U,                                       /* jobId */
    CRYPTO_JOBSTATE_IDLE,                     /* jobState */
    {
      NULL_PTR,                               /* inputPtr */
      0U,                                     /* inputLength */
      NULL_PTR,                               /* secondaryInputPtr */
      0U,                                     /* secondaryInputLength */
      NULL_PTR,                               /* tertiaryInputPtr */
      0U,                                     /* tertiaryInputLength */
      NULL_PTR,                               /* outputPtr */
      NULL_PTR,                               /* outputLengthPtr */
      NULL_PTR,                               /* secondaryOutputPtr */
      NULL_PTR,                               /* secondaryOutputLengthPtr */
      NULL_PTR,                               /* verifyPtr */
      CRYPTO_OPERATIONMODE_SINGLECALL         /* mode */
    },
    &Crypto_xVIx_xAIx_HelperHashJobPrimitiveInfo, /* jobPrimitiveInfo */
    &Crypto_xVIx_xAIx_HelperHashJobInfo,          /* jobInfo */
    0U                                        /* cryptoKeyId */
  };

  Job.jobPrimitiveInputOutput.inputPtr = InputPtr;
  Job.jobPrimitiveInputOutput.inputLength = InputLength;
  Job.jobPrimitiveInputOutput.outputPtr = OutputPtr;
  Job.jobPrimitiveInputOutput.outputLengthPtr = OutputLengthPtr;
  /* only process if the DriverObjectState is not busy. */
  SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
  if(
      CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE !=
      Crypto_xVIx_xAIx_DriverObjects[ObjId].DriverObjectState
    )
  {
    SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
    RetVal = CRYPTO_E_BUSY;
  }
  else
  {
    RetVal = E_OK;
    Crypto_xVIx_xAIx_DriverObjects[ObjId].DriverObjectState =
      CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_BUSY;
    Crypto_xVIx_xAIx_DriverObjects[ObjId].CurrentJob = &Job;
    SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
    /* !LINKSTO Crypto.Dsn.JobTransition.Idle2Active, 1 */
    /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Start, 1 */
    /* !LINKSTO Crypto.Dsn.JobTransition.Start2Start, 1 */
    /* !LINKSTO Crypto.Dsn.JobTransition.Update2Start, 1 */
    Job.jobState = CRYPTO_JOBSTATE_ACTIVE;

    Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_Config.HashLength = 32U;
    Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_Config.Job = &Job;
    Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_Config.ObjId = ObjId;
    Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_Config.Crypto_Generic_Callback =
      &Crypto_xVIx_xAIx_SHA2256HelperCallbackNotification;

    Crypto_xVIx_xAIx_SHA2Start(&Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_Config);
    /* SHA2MainFunction only needs to be called once */
    Crypto_xVIx_xAIx_SHA2MainFunction(
                                       &Crypto_xVIx_xAIx_HelperResult,
                                       &Crypto_xVIx_xAIx_HelperBusy
                                     );
    /* The update and finish can fail only in case the SHA2Ctx.State is not
       CRYPTO_XVIX_XAIX_SHA_S_INITIALIZED. As before this there is a valid Start and main call it will
       always be in CRYPTO_XVIX_XAIX_SHA_S_INITIALIZED */
    (void)Crypto_xVIx_xAIx_SHA2Update(
                                       Job.jobPrimitiveInputOutput.inputPtr,
                                       Job.jobPrimitiveInputOutput.inputLength
                                     );
    do
    {
      Crypto_xVIx_xAIx_SHA2MainFunction(
                                         &Crypto_xVIx_xAIx_HelperResult,
                                         &Crypto_xVIx_xAIx_HelperBusy
                                       );
    }while(TRUE == Crypto_xVIx_xAIx_HelperBusy && E_OK == Crypto_xVIx_xAIx_HelperResult);

    (void)Crypto_xVIx_xAIx_SHA2Finish(
                                       Job.jobPrimitiveInputOutput.outputPtr,
                                       Job.jobPrimitiveInputOutput.outputLengthPtr,
                                       TRUE
                                     );
    do
    {
      Crypto_xVIx_xAIx_SHA2MainFunction(
                                         &Crypto_xVIx_xAIx_HelperResult,
                                         &Crypto_xVIx_xAIx_HelperBusy
                                       );
    }while(TRUE == Crypto_xVIx_xAIx_HelperBusy && E_OK == Crypto_xVIx_xAIx_HelperResult);

    /* set DriverObjectState back to idle after processing of the function. Also
       set the currentJob to NULL_PTR. */
    SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
    Crypto_xVIx_xAIx_DriverObjects[ObjId].DriverObjectState =
      CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
    Crypto_xVIx_xAIx_DriverObjects[ObjId].CurrentJob = NULL_PTR;
    Crypto_xVIx_xAIx_DriverObjects[ObjId].SkipPeriodicMainFunction = TRUE;
    SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
  }
  return RetVal;
}

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2256HelperMainFunction
(
  void
)
{
  Crypto_xVIx_xAIx_SHA2MainFunction(&Crypto_xVIx_xAIx_HelperResult, &Crypto_xVIx_xAIx_HelperBusy);
}

#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_SHA2_384_ENABLED == STD_ON)

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2384HelperStart
(
  const uint32                                                   ObjId,
  P2FUNC(void, CRYPTO_xVIx_xAIx_CODE, Crypto_Generic_Callback)
  (
    P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
    uint8                                                        Error
  ),
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Job,
  boolean                                                        ProcessingType
)
{
  Crypto_xVIx_xAIx_AL_HASH_SHA2_384_NOT_SET_Config.HashLength = 48U;
  Crypto_xVIx_xAIx_AL_HASH_SHA2_384_NOT_SET_Config.Job = Job;
  Crypto_xVIx_xAIx_AL_HASH_SHA2_384_NOT_SET_Config.ObjId = ObjId;
  Crypto_xVIx_xAIx_AL_HASH_SHA2_384_NOT_SET_Config.Crypto_Generic_Callback =
  &Crypto_xVIx_xAIx_AL_HASH_SHA2_384_NOT_SET_CallBack;

  /* helper is only called asynchronous */
  Crypto_xVIx_xAIx_AL_HASH_SHA2_384_NOT_SET_Config.Crypto_Generic_Callback = Crypto_Generic_Callback;
  Crypto_xVIx_xAIx_SHA2Start(&Crypto_xVIx_xAIx_AL_HASH_SHA2_384_NOT_SET_Config);
  if(CRYPTO_PROCESSING_SYNC == ProcessingType)
  {
    /* This function can not fail for the first state */
    (void)Crypto_xVIx_xAIx_SHA2384HelperSynchronousMainCall();
  }
}

FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2384HelperUpdate
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) DataPtr,
  uint32                                                DataLength,
  boolean                                               ProcessingType
)
{
  Std_ReturnType RetVal;

  RetVal = Crypto_xVIx_xAIx_SHA2Update(DataPtr, DataLength);
  if((CRYPTO_PROCESSING_SYNC == ProcessingType) && (E_OK == RetVal))
  {
    RetVal = Crypto_xVIx_xAIx_SHA2384HelperSynchronousMainCall();
  }
  return RetVal;
}

FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2384HelperFinish
(
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  ResultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultLengthPtr,
  boolean                                              TruncationAllowed,
  boolean                                              ProcessingType
)
{
  Std_ReturnType RetVal;
  RetVal = Crypto_xVIx_xAIx_SHA2Finish(ResultPtr, ResultLengthPtr, TruncationAllowed);
  if((CRYPTO_PROCESSING_SYNC == ProcessingType) && (E_OK == RetVal))
  {
    RetVal = Crypto_xVIx_xAIx_SHA2384HelperSynchronousMainCall();
  }
  return RetVal;
}

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2384HelperMainFunction
(
  void
)
{
  Crypto_xVIx_xAIx_SHA2MainFunction(&Crypto_xVIx_xAIx_HelperResult, &Crypto_xVIx_xAIx_HelperBusy);
}

#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_384_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_SHA2_512_ENABLED == STD_ON)

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2512HelperStart
(
  const uint32                                                 ObjId,
  P2FUNC(void, CRYPTO_xVIx_xAIx_CODE, Crypto_Generic_Callback)
  (
    P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
    uint8                                                        Error
  ),
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Job,
  boolean                                                        ProcessingType
)
{
  Crypto_xVIx_xAIx_AL_HASH_SHA2_512_NOT_SET_Config.HashLength = 64U;
  Crypto_xVIx_xAIx_AL_HASH_SHA2_512_NOT_SET_Config.Job = Job;
  Crypto_xVIx_xAIx_AL_HASH_SHA2_512_NOT_SET_Config.ObjId = ObjId;
  Crypto_xVIx_xAIx_AL_HASH_SHA2_512_NOT_SET_Config.Crypto_Generic_Callback =
  &Crypto_xVIx_xAIx_AL_HASH_SHA2_512_NOT_SET_CallBack;
  Crypto_xVIx_xAIx_AL_HASH_SHA2_512_NOT_SET_Config.Crypto_Generic_Callback = Crypto_Generic_Callback;
  Crypto_xVIx_xAIx_SHA2Start(&Crypto_xVIx_xAIx_AL_HASH_SHA2_512_NOT_SET_Config);
  if(CRYPTO_PROCESSING_SYNC == ProcessingType)
  {
    /* This function can not fail for the first state */
    (void)Crypto_xVIx_xAIx_SHA2512HelperSynchronousMainCall();
  }
}

FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2512HelperUpdate
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) DataPtr,
  uint32                                                DataLength,
  boolean                                               ProcessingType
)
{
  Std_ReturnType RetVal;
  RetVal = Crypto_xVIx_xAIx_SHA2Update(DataPtr, DataLength);
  if((CRYPTO_PROCESSING_SYNC == ProcessingType) && (E_OK == RetVal))
  {
    RetVal = Crypto_xVIx_xAIx_SHA2512HelperSynchronousMainCall();
  }
  return RetVal;
}

FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2512HelperFinish
(
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  ResultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultLengthPtr,
  boolean                                              TruncationAllowed,
  boolean                                              ProcessingType
)
{
  Std_ReturnType RetVal;
  RetVal = Crypto_xVIx_xAIx_SHA2Finish(ResultPtr, ResultLengthPtr, TruncationAllowed);
  if((CRYPTO_PROCESSING_SYNC == ProcessingType) && (E_OK == RetVal))
  {
    RetVal = Crypto_xVIx_xAIx_SHA2512HelperSynchronousMainCall();
  }
  return RetVal;
}

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2512HelperMainFunction
(
  void
)
{
  Crypto_xVIx_xAIx_SHA2MainFunction(&Crypto_xVIx_xAIx_HelperResult, &Crypto_xVIx_xAIx_HelperBusy);
}

#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_512_ENABLED == STD_ON) */

/* SHA2 AL Functions*/

#if (CRYPTO_XVIX_XAIX_SHA2_224_ENABLED == STD_ON)

/* !LINKSTO EB_Crypto_CryptAlgos_1131, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_224_NOT_SET_Start
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  /** \brief Configuration for SHA2 224 */
  Crypto_xVIx_xAIx_AL_HASH_SHA2_224_NOT_SET_Config.HashLength = CRYPTO_XVIX_XAIX_SHA224_HASH_LEN_BYTES;
  Crypto_xVIx_xAIx_AL_HASH_SHA2_224_NOT_SET_Config.Crypto_Generic_Callback =
    &Crypto_xVIx_xAIx_AL_HASH_SHA2_224_NOT_SET_CallBack;
  Crypto_xVIx_xAIx_AL_HASH_SHA2_224_NOT_SET_Config.ObjId = ObjectId;
  Crypto_xVIx_xAIx_AL_HASH_SHA2_224_NOT_SET_Config.Job = Job;
  Crypto_xVIx_xAIx_SHA2Start(&Crypto_xVIx_xAIx_AL_HASH_SHA2_224_NOT_SET_Config);
  return E_OK;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1152, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_224_NOT_SET_Update
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  TS_PARAM_UNUSED(ObjectId);
  RetVal = Crypto_xVIx_xAIx_SHA2Update(
                              Job->jobPrimitiveInputOutput.inputPtr,
                              Job->jobPrimitiveInputOutput.inputLength
                            );
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1173, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_224_NOT_SET_Finish
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  TS_PARAM_UNUSED(ObjectId);
  RetVal = Crypto_xVIx_xAIx_SHA2Finish(
                              Job->jobPrimitiveInputOutput.outputPtr,
                              Job->jobPrimitiveInputOutput.outputLengthPtr,
                              FALSE
                            );
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1213, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_224_NOT_SET_Cancel
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  RetVal = Crypto_xVIx_xAIx_SHA2Cancel();
  TS_PARAM_UNUSED(ObjectId);
  TS_PARAM_UNUSED(Job);
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1234, 1 */
FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_224_NOT_SET_MainFunction
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
)
{
  Crypto_xVIx_xAIx_SHA2MainFunction(Result, Busy);
}

#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_224_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON)

/* !LINKSTO EB_Crypto_CryptAlgos_1132, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_Start
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  /** \brief Configuration for SHA2 256 */
  Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_Config.HashLength = CRYPTO_XVIX_XAIX_SHA256_HASH_LEN_BYTES;
  Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_Config.Crypto_Generic_Callback =
    &Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_CallBack;
  Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_Config.ObjId = ObjectId;
  Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_Config.Job = Job;
  Crypto_xVIx_xAIx_SHA2Start(&Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_Config);
  return E_OK;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1153, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_Update
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  TS_PARAM_UNUSED(ObjectId);
  RetVal = Crypto_xVIx_xAIx_SHA2Update(
                              Job->jobPrimitiveInputOutput.inputPtr,
                              Job->jobPrimitiveInputOutput.inputLength
                            );
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1174, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_Finish
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  TS_PARAM_UNUSED(ObjectId);
  RetVal = Crypto_xVIx_xAIx_SHA2Finish(
                              Job->jobPrimitiveInputOutput.outputPtr,
                              Job->jobPrimitiveInputOutput.outputLengthPtr,
                              FALSE
                            );
  return RetVal;
}
/* !LINKSTO EB_Crypto_CryptAlgos_1214, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_Cancel
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  RetVal = Crypto_xVIx_xAIx_SHA2Cancel();
  TS_PARAM_UNUSED(ObjectId);
  TS_PARAM_UNUSED(Job);
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1235, 1 */
FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_MainFunction
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
)
{
  Crypto_xVIx_xAIx_SHA2MainFunction(Result, Busy);
}

#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_SHA2_384_ENABLED == STD_ON)

/* !LINKSTO EB_Crypto_CryptAlgos_1133, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_384_NOT_SET_Start
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  /** \brief Configuration for SHA2 384 */
  Crypto_xVIx_xAIx_AL_HASH_SHA2_384_NOT_SET_Config.HashLength = CRYPTO_XVIX_XAIX_SHA384_HASH_LEN_BYTES;
  Crypto_xVIx_xAIx_AL_HASH_SHA2_384_NOT_SET_Config.Crypto_Generic_Callback =
    &Crypto_xVIx_xAIx_AL_HASH_SHA2_384_NOT_SET_CallBack;
  Crypto_xVIx_xAIx_AL_HASH_SHA2_384_NOT_SET_Config.ObjId = ObjectId;
  Crypto_xVIx_xAIx_AL_HASH_SHA2_384_NOT_SET_Config.Job = Job;
  Crypto_xVIx_xAIx_SHA2Start(&Crypto_xVIx_xAIx_AL_HASH_SHA2_384_NOT_SET_Config);
  return E_OK;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1154, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_384_NOT_SET_Update
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  TS_PARAM_UNUSED(ObjectId);
  RetVal = Crypto_xVIx_xAIx_SHA2Update(
                              Job->jobPrimitiveInputOutput.inputPtr,
                              Job->jobPrimitiveInputOutput.inputLength
                            );
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1175, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_384_NOT_SET_Finish
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  TS_PARAM_UNUSED(ObjectId);
  RetVal = Crypto_xVIx_xAIx_SHA2Finish(
                              Job->jobPrimitiveInputOutput.outputPtr,
                              Job->jobPrimitiveInputOutput.outputLengthPtr,
                              FALSE
                            );
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1215, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_384_NOT_SET_Cancel
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  RetVal = Crypto_xVIx_xAIx_SHA2Cancel();
  TS_PARAM_UNUSED(ObjectId);
  TS_PARAM_UNUSED(Job);
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1236, 1 */
FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_384_NOT_SET_MainFunction
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
)
{
  Crypto_xVIx_xAIx_SHA2MainFunction(Result, Busy);
}

#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_384_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_SHA2_512_ENABLED == STD_ON)

/* !LINKSTO EB_Crypto_CryptAlgos_1134, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_512_NOT_SET_Start
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  /** \brief Configuration for SHA2 512 */
  Crypto_xVIx_xAIx_AL_HASH_SHA2_512_NOT_SET_Config.HashLength = CRYPTO_XVIX_XAIX_SHA512_HASH_LEN_BYTES;
  Crypto_xVIx_xAIx_AL_HASH_SHA2_512_NOT_SET_Config.Crypto_Generic_Callback =
  &Crypto_xVIx_xAIx_AL_HASH_SHA2_512_NOT_SET_CallBack;
  Crypto_xVIx_xAIx_AL_HASH_SHA2_512_NOT_SET_Config.ObjId = ObjectId;
  Crypto_xVIx_xAIx_AL_HASH_SHA2_512_NOT_SET_Config.Job = Job;
  Crypto_xVIx_xAIx_SHA2Start(&Crypto_xVIx_xAIx_AL_HASH_SHA2_512_NOT_SET_Config);
  return E_OK;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1155, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_512_NOT_SET_Update
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  TS_PARAM_UNUSED(ObjectId);
  RetVal = Crypto_xVIx_xAIx_SHA2Update(
                              Job->jobPrimitiveInputOutput.inputPtr,
                              Job->jobPrimitiveInputOutput.inputLength
                            );
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1176, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_512_NOT_SET_Finish
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  TS_PARAM_UNUSED(ObjectId);
  RetVal = Crypto_xVIx_xAIx_SHA2Finish(
                              Job->jobPrimitiveInputOutput.outputPtr,
                              Job->jobPrimitiveInputOutput.outputLengthPtr,
                              FALSE
                            );
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1216, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_512_NOT_SET_Cancel
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  RetVal = Crypto_xVIx_xAIx_SHA2Cancel();
  TS_PARAM_UNUSED(ObjectId);
  TS_PARAM_UNUSED(Job);
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1237, 1 */
FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_512_NOT_SET_MainFunction
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
)
{
  Crypto_xVIx_xAIx_SHA2MainFunction(Result, Busy);
}

#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_512_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Definition of functions with internal linkage]===============================================*/

/*--SHACommon-------------------------------------------------------------------------------------*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHAMainFunction
(
  P2VAR(Crypto_xVIx_xAIx_SHACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  Crypto_xVIx_xAIx_SHAInitFctType                                 InitFct,
  Crypto_xVIx_xAIx_SHACompressFctType                             CompressFct,
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)     ResultError,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)            Busy
)
{
  boolean finished = FALSE;

  CtxPtr->IsBusy = Busy;
  CtxPtr->ResultError = ResultError;
  if((NULL_PTR != ResultError) && (NULL_PTR != Busy))
  {
    *CtxPtr->IsBusy = TRUE;
    *CtxPtr->ResultError = E_OK;
  }

  /* process as much data s possible */
  while (FALSE == finished)
  {
    switch(CtxPtr->State)
    {
      case CRYPTO_XVIX_XAIX_SHA_S_INIT:
      Crypto_xVIx_xAIx_SHASInit(CtxPtr, InitFct);
      finished = TRUE;
      break;

      case CRYPTO_XVIX_XAIX_SHA_S_UPDATE:
      Crypto_xVIx_xAIx_SHASUpdate(CtxPtr, CompressFct);
      finished = TRUE;
      break;

      case CRYPTO_XVIX_XAIX_SHA_S_FINISH:
      Crypto_xVIx_xAIx_SHASFinish(CtxPtr);
      break;

      case CRYPTO_XVIX_XAIX_SHA_S_FINISH_UPDATE1:
      if(Crypto_xVIx_xAIx_SHAProcessInput(CtxPtr, CompressFct) == TRUE)
      {
        CtxPtr->ProcessInputState = CRYPTO_XVIX_XAIX_SHA_S_PROCESS;

        /* 3. append length information and complete last block */
        CtxPtr->Data = CtxPtr->Length;
        if(CRYPTO_XVIX_XAIX_SHA2_224_256_WORD_SIZE == CtxPtr->CurrentWordSize)
        {
          CtxPtr->DataLength = 8U;
        }
        /* CHECK: NOPARSE */
        /*This is depending upon the configuration and the configuration limits it to only valid
           values*/
        else if(CRYPTO_XVIX_XAIX_SHA2_384_512_WORD_SIZE == CtxPtr->CurrentWordSize)
        {
          CtxPtr->DataLength = 16U;
        }
        else
        {
          /* unreachable code but needed for MISRAC2012*/
        }
        /* CHECK: PARSE */

        CtxPtr->State = CRYPTO_XVIX_XAIX_SHA_S_FINISH_UPDATE2;
      }
      break;

      case CRYPTO_XVIX_XAIX_SHA_S_FINISH_UPDATE2:
      if(Crypto_xVIx_xAIx_SHAProcessInput(CtxPtr, CompressFct) == TRUE)
      {
        CtxPtr->State = CRYPTO_XVIX_XAIX_SHA_S_FINISH_EXPORT;
      }
      break;

      case CRYPTO_XVIX_XAIX_SHA_S_FINISH_EXPORT:
      /* 4. export CtxPtr->Common.resultError to user */
      Crypto_xVIx_xAIx_SHAExport(CtxPtr);

      CtxPtr->State = CRYPTO_XVIX_XAIX_SHA_S_IDLE;
      CtxPtr->CallbackFlag = TRUE;
      finished = TRUE;
      break;

      case CRYPTO_XVIX_XAIX_SHA_S_INITIALIZED:
      default:
      {
        finished = TRUE;
      }
      break;
    }
  }
  Crypto_xVIx_xAIx_SHACallbackCancel(CtxPtr);
}

/** \brief intern function for sha computation*/
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHAProcessInput
(
  P2VAR(Crypto_xVIx_xAIx_SHACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  Crypto_xVIx_xAIx_SHACompressFctType CompressFct
)
{
  boolean finished = FALSE;

  while (FALSE == finished)
  {
    switch(CtxPtr->ProcessInputState)
    {
      case CRYPTO_XVIX_XAIX_SHA_S_PROCESS:
      finished = Crypto_xVIx_xAIx_SHAProcessInputProcess(CtxPtr);
      break;

      case CRYPTO_XVIX_XAIX_SHA_S_PROCESS_COPY1:
      Crypto_xVIx_xAIx_SHAProcessInputCopy1(CtxPtr);
      break;

      case CRYPTO_XVIX_XAIX_SHA_S_PROCESS_COMPRESS1:
      finished = Crypto_xVIx_xAIx_SHAProcessInputCompress1(CtxPtr);
      break;

      case CRYPTO_XVIX_XAIX_SHA_S_PROCESS_COMPRESS2:
      Crypto_xVIx_xAIx_SHAProcessInputCompress2(CtxPtr, CompressFct);
      break;

      case CRYPTO_XVIX_XAIX_SHA_S_PROCESS_COMPRESS3:
      Crypto_xVIx_xAIx_SHAProcessInputCompress3(CtxPtr, CompressFct);
      break;

      case CRYPTO_XVIX_XAIX_SHA_S_PROCESS_COPY2:
      Crypto_xVIx_xAIx_SHAProcessInputCopy2(CtxPtr);
      finished = TRUE;
      break;
      /* CHECK: NOPARSE */
      /*All the valid states are addressed in the above cases. This is internal state machine and the
        states are changes internally. It can not be called from outside mainfunction, to test the
        default case using CRYPTO_XVIX_XAIX_SHA_S_PROCESS_IDLE.*/
      /* Defensive Programming - unreachable code. This default case is required by MISRA-C:2012 */
      default:
      {}
      break;
      /* CHECK: PARSE */
    }
  }

  return finished;
}

/** \brief intern function for sha computation*/
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHAProcessInputProcess
(
  P2VAR(Crypto_xVIx_xAIx_SHACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  boolean Finished = FALSE;
  if(
      CRYPTO_ALGOFAM_SHA2_224 == CtxPtr->Job->jobPrimitiveInfo->primitiveInfo->algorithm.family ||
      CRYPTO_ALGOFAM_SHA2_224 ==
      CtxPtr->Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily ||
      CRYPTO_ALGOFAM_SHA2_256 == CtxPtr->Job->jobPrimitiveInfo->primitiveInfo->algorithm.family ||
      CRYPTO_ALGOFAM_SHA2_256 ==
      CtxPtr->Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily
    )
  {
    CtxPtr->CurrentWordSize = CRYPTO_XVIX_XAIX_SHA2_224_256_WORD_SIZE;
  }
  /* CHECK: NOPARSE */
  /* Only valid values are possible in the configuration. The else case can not tested.*/
  else if
  (
    CRYPTO_ALGOFAM_SHA2_384 == CtxPtr->Job->jobPrimitiveInfo->primitiveInfo->algorithm.family ||
    CRYPTO_ALGOFAM_SHA2_384 ==
     CtxPtr->Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily ||
    CRYPTO_ALGOFAM_SHA2_512 == CtxPtr->Job->jobPrimitiveInfo->primitiveInfo->algorithm.family ||
    CRYPTO_ALGOFAM_SHA2_512 ==
    CtxPtr->Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily
  )
  {
    CtxPtr->CurrentWordSize = CRYPTO_XVIX_XAIX_SHA2_384_512_WORD_SIZE;
  }
  else
  {
    /* unreachable code but needed for MISRAC2012*/
  }
  /* CHECK: PARSE */
  if (0U != CtxPtr->DataLength)
  {
    /* initialize temporal data */
    CtxPtr->P = CtxPtr->Data;
    CtxPtr->Left = CtxPtr->DataLength;
    CtxPtr->Copy = 0U;

    /* 1. get unprocessed bytes from last block and update the counter */
    if(CRYPTO_XVIX_XAIX_SHA2_224_256_WORD_SIZE == CtxPtr->CurrentWordSize)
    {
      CtxPtr->Offset = (uint8) ((CtxPtr->Count) & 0x3fUL);
    }
    /* CHECK: NOPARSE */
    /*CurrentWordSize can have only valid possible values from configuration.*/
    else if(CRYPTO_XVIX_XAIX_SHA2_384_512_WORD_SIZE == CtxPtr->CurrentWordSize)
    {
      CtxPtr->Offset = (uint8) ((CtxPtr->Count) & 0x7fUL);
    }
    else
    {
      /* unreachable code but needed for MISRAC2012*/
    }
    /* CHECK: PARSE */

    /* Ignore a possible overflow here */
    CtxPtr->Count += CtxPtr->DataLength;
    CtxPtr->ProcessInputState = CRYPTO_XVIX_XAIX_SHA_S_PROCESS_COPY1;
  }
  else
  {
    /* if there is no data, there is no work to do */
    Finished = TRUE;
  }
  return Finished;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHAProcessInputCopy1
(
  P2VAR(Crypto_xVIx_xAIx_SHACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  /* 2. if existent, process incomplete blocks */
  if (CtxPtr->Offset > 0U)
  {
    /* find out how much data we need to copy */
    if(
        (CRYPTO_XVIX_XAIX_SHA2_224_256_WORD_SIZE == CtxPtr->CurrentWordSize) &&
        (uint32) CRYPTO_XVIX_XAIX_SHA_224_256_BLOCK_LEN_BYTES <
        (uint32) ((uint32) CtxPtr->Offset + CtxPtr->DataLength)
     )
    {
      CtxPtr->Copy = (uint8) CRYPTO_XVIX_XAIX_SHA_224_256_BLOCK_LEN_BYTES - CtxPtr->Offset;
    }
    else if(
              (CRYPTO_XVIX_XAIX_SHA2_384_512_WORD_SIZE == CtxPtr->CurrentWordSize) &&
              (uint32) CRYPTO_XVIX_XAIX_SHA_384_512_BW_BLOCK_LEN_BYTES <
              (uint32) ((uint32) CtxPtr->Offset + CtxPtr->DataLength)
           )
    {
      CtxPtr->Copy = (uint8) CRYPTO_XVIX_XAIX_SHA_384_512_BW_BLOCK_LEN_BYTES - CtxPtr->Offset;
    }
    else
    {
      /* cast is ok here: we tested dataBytes "< 64" above */
      CtxPtr->Copy = (uint8) CtxPtr->DataLength;
    }

    /* fill up the temporary buffer with as much data as possible */
    TS_MemCpy
    (
      &CtxPtr->Partial[CtxPtr->Offset],
      CtxPtr->P,
      (uint16) CtxPtr->Copy
    );

    CtxPtr->ProcessInputState = CRYPTO_XVIX_XAIX_SHA_S_PROCESS_COMPRESS1;
  }
  else
  {
    Crypto_xVIx_xAIx_SHACompressStart(CtxPtr);

    CtxPtr->ProcessInputState = CRYPTO_XVIX_XAIX_SHA_S_PROCESS_COMPRESS3;
  }
}

static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHAProcessInputCompress1
(
  P2VAR(Crypto_xVIx_xAIx_SHACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  boolean Finished = FALSE;
  /* look if we have one block to compress */
  if(
       (CRYPTO_XVIX_XAIX_SHA2_224_256_WORD_SIZE == CtxPtr->CurrentWordSize) &&
       (uint8) CRYPTO_XVIX_XAIX_SHA_224_256_BLOCK_LEN_BYTES >
       (uint8) (CtxPtr->Offset + CtxPtr->Copy)
    )
  {
    Finished = TRUE;
  }
  else if(
            (CRYPTO_XVIX_XAIX_SHA2_384_512_WORD_SIZE == CtxPtr->CurrentWordSize) &&
            (uint8) (CtxPtr->Offset + CtxPtr->Copy) <
            (uint8) CRYPTO_XVIX_XAIX_SHA_384_512_BW_BLOCK_LEN_BYTES
         )
  {
    Finished = TRUE;
  }
  else
  {
    /* we have one complete block, so compress it */
    Crypto_xVIx_xAIx_SHACompressStart(CtxPtr);

    CtxPtr->P = &CtxPtr->P[CtxPtr->Copy];
    CtxPtr->Left -= (uint32) CtxPtr->Copy;

    CtxPtr->ProcessInputState = CRYPTO_XVIX_XAIX_SHA_S_PROCESS_COMPRESS2;
  }
  return Finished;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHAProcessInputCompress2
(
  P2VAR(Crypto_xVIx_xAIx_SHACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  Crypto_xVIx_xAIx_SHACompressFctType CompressFct
)
{
  if(CompressFct(CtxPtr->Partial) == TRUE)
  {
    Crypto_xVIx_xAIx_SHACompressStart(CtxPtr);
    CtxPtr->ProcessInputState = CRYPTO_XVIX_XAIX_SHA_S_PROCESS_COMPRESS3;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHAProcessInputCompress3
(
  P2VAR(Crypto_xVIx_xAIx_SHACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  Crypto_xVIx_xAIx_SHACompressFctType CompressFct
)
{
  /* 3. process complete blocks */
  if (
       (CRYPTO_XVIX_XAIX_SHA2_224_256_WORD_SIZE == CtxPtr->CurrentWordSize) &&
       (
         (CtxPtr->Left & (uint32) CRYPTO_XVIX_XAIX_SHA_2_MASK_CLEAR_LEFTMOST_6_BITS) !=
         (uint32) 0U
       )
     )
  {
    if(CompressFct(CtxPtr->P) == TRUE)
    {
      CtxPtr->P = &CtxPtr->P[CRYPTO_XVIX_XAIX_SHA_224_256_BLOCK_LEN_BYTES];
      CtxPtr->Left -= (uint32) CRYPTO_XVIX_XAIX_SHA_224_256_BLOCK_LEN_BYTES;

      if (
           (CtxPtr->Left & (uint32) CRYPTO_XVIX_XAIX_SHA_2_MASK_CLEAR_LEFTMOST_6_BITS) !=
           (uint32) 0U
         )
      {
        Crypto_xVIx_xAIx_SHACompressStart(CtxPtr);
      }
    }
  }
  else if (
            (CRYPTO_XVIX_XAIX_SHA2_384_512_WORD_SIZE == CtxPtr->CurrentWordSize) &&
            (
              (CtxPtr->Left & (uint32) CRYPTO_XVIX_XAIX_SHA_2_MASK_CLEAR_LEFTMOST_7_BITS) !=
              (uint32) 0U
            )
          )
  {
    if(CompressFct(CtxPtr->P) == TRUE)
    {
      CtxPtr->P = &CtxPtr->P[CRYPTO_XVIX_XAIX_SHA_384_512_BW_BLOCK_LEN_BYTES];
      CtxPtr->Left -= (uint32) CRYPTO_XVIX_XAIX_SHA_384_512_BW_BLOCK_LEN_BYTES;

      /* If there is data for another block, start processing. */
      if (
           (CtxPtr->Left & (uint32) CRYPTO_XVIX_XAIX_SHA_2_MASK_CLEAR_LEFTMOST_7_BITS) !=
           (uint32) 0U
         )
      {
        Crypto_xVIx_xAIx_SHACompressStart(CtxPtr);
      }
    }
  }
  else
  {
    CtxPtr->ProcessInputState = CRYPTO_XVIX_XAIX_SHA_S_PROCESS_COPY2;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHAProcessInputCopy2
(
  P2VAR(Crypto_xVIx_xAIx_SHACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  /* 4. Check if there is still data */
  if (CtxPtr->Left > (uint32) 0U)
  {
    /* Copy data into partial buffer */
    TS_MemCpy
    (
      CtxPtr->Partial,
      CtxPtr->P,
      (uint16) CtxPtr->Left
    );
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHACompressStart
(
  P2VAR(Crypto_xVIx_xAIx_SHACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  CtxPtr->SHACompressState = CRYPTO_XVIX_XAIX_SHA_S_COMPRESS;
}

/** \brief intern function for sha computation*/
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHAExport
(
  P2VAR(Crypto_xVIx_xAIx_SHACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  uint8_least Index; /* loop variable */
  uint32 HashLength;

  HashLength = CtxPtr->CfgPtr->HashLength;

  if(HashLength <= *(CtxPtr->ResultLengthPtr))
  {
    *(CtxPtr->ResultLengthPtr) = HashLength;
  }
  else
  {

    /* Hardcoded to FALSE in this Version*/
    if(FALSE == CtxPtr->TruncationAllowed)
    {
      CtxPtr->ErrorCode = CRYPTO_E_SMALL_BUFFER;
    }

  }

  if(
     CRYPTO_ALGOFAM_SHA2_224 == CtxPtr->Job->jobPrimitiveInfo->primitiveInfo->algorithm.family ||
     CRYPTO_ALGOFAM_SHA2_224 ==
     CtxPtr->Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily ||
     CRYPTO_ALGOFAM_SHA2_256 == CtxPtr->Job->jobPrimitiveInfo->primitiveInfo->algorithm.family ||
     CRYPTO_ALGOFAM_SHA2_256 ==
     CtxPtr->Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily
   )
  {
    Index = (uint8_least) 0U;
    do
    {
      uint8_least TmpIndex; /* tmp space to save loop resultErrors */

      TmpIndex = Index;
      TmpIndex <<= 2U;
      if(TmpIndex < *(CtxPtr->ResultLengthPtr))
      {
        CtxPtr->Result[TmpIndex] = (uint8) (CtxPtr->Internalstate[Index] >> 24U);
        TmpIndex++;
      }

      if(TmpIndex < *(CtxPtr->ResultLengthPtr))
      {
        CtxPtr->Result[TmpIndex] = (uint8) (CtxPtr->Internalstate[Index] >> 16U);
        TmpIndex++;
      }

      if(TmpIndex < *(CtxPtr->ResultLengthPtr))
      {
        CtxPtr->Result[TmpIndex] = (uint8) (CtxPtr->Internalstate[Index] >> 8U);
        TmpIndex++;
      }

      if(TmpIndex < *(CtxPtr->ResultLengthPtr))
      {
        CtxPtr->Result[TmpIndex] = (uint8) (CtxPtr->Internalstate[Index] );
      }

      Index++;
    }while (Index <= (uint8_least)(*(CtxPtr->ResultLengthPtr) >> 2U));
  }
  /* CHECK: NOPARSE */
  else if
  (
    CRYPTO_ALGOFAM_SHA2_384 == CtxPtr->Job->jobPrimitiveInfo->primitiveInfo->algorithm.family ||
    CRYPTO_ALGOFAM_SHA2_384 ==
    CtxPtr->Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily ||
    CRYPTO_ALGOFAM_SHA2_512 == CtxPtr->Job->jobPrimitiveInfo->primitiveInfo->algorithm.family ||
    CRYPTO_ALGOFAM_SHA2_512 ==
    CtxPtr->Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily
  )
  {
    Index = (uint8_least) 0U;
    do
    {
      uint8_least TmpIndex; /* tmp space to save loop resultErrors */

      TmpIndex = Index;
      TmpIndex <<= 2U;

      if(TmpIndex < *(CtxPtr->ResultLengthPtr))
      {
        CtxPtr->Result[TmpIndex] = (uint8) (CtxPtr->Internalstate[Index + 1U] >> 24U);
        TmpIndex++;
      }

      if(TmpIndex < *(CtxPtr->ResultLengthPtr))
      {
        CtxPtr->Result[TmpIndex] = (uint8) (CtxPtr->Internalstate[Index + 1U] >> 16U);
        TmpIndex++;
      }

      if(TmpIndex < *(CtxPtr->ResultLengthPtr))
      {
        CtxPtr->Result[TmpIndex] = (uint8) (CtxPtr->Internalstate[Index + 1U] >> 8U);
        TmpIndex++;
      }

      if(TmpIndex < *(CtxPtr->ResultLengthPtr))
      {
        CtxPtr->Result[TmpIndex] = (uint8) (CtxPtr->Internalstate[Index + 1U]);
        TmpIndex++;
      }

      if(TmpIndex < *(CtxPtr->ResultLengthPtr))
      {
        CtxPtr->Result[TmpIndex] = (uint8) (CtxPtr->Internalstate[Index] >> 24U);
        TmpIndex++;
      }

      if(TmpIndex < *(CtxPtr->ResultLengthPtr))
      {
        CtxPtr->Result[TmpIndex] = (uint8) (CtxPtr->Internalstate[Index] >> 16U);
        TmpIndex++;
      }

      if(TmpIndex < *(CtxPtr->ResultLengthPtr))
      {
        CtxPtr->Result[TmpIndex] = (uint8) (CtxPtr->Internalstate[Index] >> 8U);
        TmpIndex++;
      }

      if(TmpIndex < *(CtxPtr->ResultLengthPtr))
      {
        CtxPtr->Result[TmpIndex] = (uint8) (CtxPtr->Internalstate[Index]);
      }

      Index += 2U;
    }while (Index <= (uint8_least)(*(CtxPtr->ResultLengthPtr) >> 2U));
  }
  else
  {
    /* unreachable code but needed for MISRAC2012*/
  }
  /* CHECK: PARSE */
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHASInit
(
  P2VAR(Crypto_xVIx_xAIx_SHACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  Crypto_xVIx_xAIx_SHAInitFctType                                           InitFct
)
{
  if(
     (NULL_PTR != CtxPtr->Job->jobPrimitiveInfo) &&
     (
       CRYPTO_ALGOFAM_SHA2_224 ==
       CtxPtr->Job->jobPrimitiveInfo->primitiveInfo->algorithm.family ||
       CRYPTO_ALGOFAM_SHA2_224 ==
       CtxPtr->Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily ||
       CRYPTO_ALGOFAM_SHA2_256 ==
       CtxPtr->Job->jobPrimitiveInfo->primitiveInfo->algorithm.family ||
       CRYPTO_ALGOFAM_SHA2_256 ==
       CtxPtr->Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily
     )
   )
  {
    CtxPtr->CurrentWordSize = CRYPTO_XVIX_XAIX_SHA2_224_256_WORD_SIZE;
  }
  /* CHECK: NOPARSE */
  /*All the possible values for the algorithm family/secondary algorithm family are covered. Else
    will not be executed.*/
  else if
  (
    (NULL_PTR != CtxPtr->Job->jobPrimitiveInfo) &&
    (
      CRYPTO_ALGOFAM_SHA2_384 == CtxPtr->Job->jobPrimitiveInfo->primitiveInfo->algorithm.family ||
      CRYPTO_ALGOFAM_SHA2_384 ==
      CtxPtr->Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily ||
      CRYPTO_ALGOFAM_SHA2_512 == CtxPtr->Job->jobPrimitiveInfo->primitiveInfo->algorithm.family ||
      CRYPTO_ALGOFAM_SHA2_512 ==
      CtxPtr->Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily
    )
  )
  {
    CtxPtr->CurrentWordSize = CRYPTO_XVIX_XAIX_SHA2_384_512_WORD_SIZE;
  }
  else
  {
    /* unreachable code but needed for MISRAC2012*/
  }
  /* CHECK: PARSE */
  /* 1. Init the number of processed bytes */
  CtxPtr->Count = (uint32) 0U;

  /* 2. Initialize the internal state of the hash function */
  (InitFct)();

  CtxPtr->State = CRYPTO_XVIX_XAIX_SHA_S_INITIALIZED;
  CtxPtr->CallbackFlag = TRUE;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHASUpdate
(
  P2VAR(Crypto_xVIx_xAIx_SHACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr,
  Crypto_xVIx_xAIx_SHACompressFctType                                       CompressFct
)
{
  if((NULL_PTR != CtxPtr->IsBusy))
  {
    *CtxPtr->IsBusy = TRUE;
  }
  Crypto_xVIx_xAIx_SHA2Ctx.CurrentMode = CRYPTO_OPERATIONMODE_UPDATE;
  if(Crypto_xVIx_xAIx_SHAProcessInput(CtxPtr, CompressFct) == TRUE)
  {
    CtxPtr->State = CRYPTO_XVIX_XAIX_SHA_S_INITIALIZED;
    CtxPtr->CallbackFlag = TRUE;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHASFinish
(
  P2VAR(Crypto_xVIx_xAIx_SHACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  /* !LINKSTO EB_Crypto_CryptAlgos_0304,2 */
  /* Pad the buffer */
  if((NULL_PTR != CtxPtr->IsBusy))
  {
    *CtxPtr->IsBusy = TRUE;
  }
  /* Set current mode to call Callback correctly */
  CtxPtr->CurrentMode = CRYPTO_OPERATIONMODE_FINISH;
  /* initialize the Pad block */
  /* !LINKSTO EB_Crypto_CryptAlgos_0305,1 */
  CtxPtr->SHAPad[0U] = 0x80U;
  CtxPtr->Length[0U] = 1U;
  if(CRYPTO_XVIX_XAIX_SHA2_224_256_WORD_SIZE == CtxPtr->CurrentWordSize)
  {
    /* !LINKSTO EB_Crypto_CryptAlgos_0387,1 */
    /* !LINKSTO EB_Crypto_CryptAlgos_0377,1 */
    do
    {
      CtxPtr->SHAPad[CtxPtr->Length[0U]] = 0U;
      ++CtxPtr->Length[0U];
    }
    while (CRYPTO_XVIX_XAIX_SHA_224_256_BLOCK_LEN_BYTES > CtxPtr->Length[0U]);
  }
  /* CHECK: NOPARSE */
  /*CurrentWordSize can only have valid values from the configurations. else case never executed.*/
  else if(CRYPTO_XVIX_XAIX_SHA2_384_512_WORD_SIZE == CtxPtr->CurrentWordSize)
  {
    /* !LINKSTO EB_Crypto_CryptAlgos_0307,2 */
    /* !LINKSTO EB_Crypto_CryptAlgos_0397,1 */
    do
    {
      CtxPtr->SHAPad[CtxPtr->Length[0U]] = 0U;
      ++CtxPtr->Length[0U];
    }
    while (CRYPTO_XVIX_XAIX_SHA_384_512_BW_BLOCK_LEN_BYTES > CtxPtr->Length[0U]);
  }
  else
  {
    /* unreachable code but needed for MISRAC2012*/
  }
  /* CHECK: PARSE */

  /* 1. convert length information */
  /* convert into bits */
  CtxPtr->Count <<= 3U;

  /* Copy the resultError. We only have the lower 32 bits, so the upper 32/96 bits
   * are zero.
   */
  /* !LINKSTO EB_Crypto_CryptAlgos_0386,1 */
  /* !LINKSTO EB_Crypto_CryptAlgos_0376,1 */
  if(CRYPTO_XVIX_XAIX_SHA2_224_256_WORD_SIZE == CtxPtr->CurrentWordSize)
  {
  #if ((CRYPTO_XVIX_XAIX_SHA2_224_ENABLED == STD_ON) || (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON))
    CtxPtr->Length[0U] = (uint8) 0U;
    CtxPtr->Length[1U] = (uint8) 0U;
    CtxPtr->Length[2U] = (uint8) 0U;
    CtxPtr->Length[3U] = (uint8) 0U;
    CtxPtr->Length[4U] = (uint8) (CtxPtr->Count >> 24U);
    CtxPtr->Length[5U] = (uint8) (CtxPtr->Count >> 16U);
    CtxPtr->Length[6U] = (uint8) (CtxPtr->Count >> 8U);
    CtxPtr->Length[7U] = (uint8) (CtxPtr->Count);
  #endif
  }
  /* !LINKSTO EB_Crypto_CryptAlgos_0306,2 */
  /* !LINKSTO EB_Crypto_CryptAlgos_0396,1 */
  /* CHECK: NOPARSE */
  /*CurrentWordSize can only have valid value from configuration.*/
  else if(CRYPTO_XVIX_XAIX_SHA2_384_512_WORD_SIZE == CtxPtr->CurrentWordSize)
  {
  #if ((CRYPTO_XVIX_XAIX_SHA2_384_ENABLED == STD_ON) || (CRYPTO_XVIX_XAIX_SHA2_512_ENABLED == STD_ON))
    CtxPtr->Length[0U] = (uint8) 0U;
    CtxPtr->Length[1U] = (uint8) 0U;
    CtxPtr->Length[2U] = (uint8) 0U;
    CtxPtr->Length[3U] = (uint8) 0U;
    CtxPtr->Length[4U] = (uint8) 0U;
    CtxPtr->Length[5U] = (uint8) 0U;
    CtxPtr->Length[6U] = (uint8) 0U;
    CtxPtr->Length[7U] = (uint8) 0U;
    CtxPtr->Length[8U] = (uint8) 0U;
    CtxPtr->Length[9U] = (uint8) 0U;
    CtxPtr->Length[10U] = (uint8) 0U;
    CtxPtr->Length[11U] = (uint8) 0U;
    CtxPtr->Length[12U] = (uint8) (CtxPtr->Count >> 24U);
    CtxPtr->Length[13U] = (uint8) (CtxPtr->Count >> 16U);
    CtxPtr->Length[14U] = (uint8) (CtxPtr->Count >> 8U);
    CtxPtr->Length[15U] = (uint8) (CtxPtr->Count);
  #endif
  }
  else
  {
    /* unreachable code but needed for MISRAC2012*/
  }
   /* CHECK: PARSE */

  CtxPtr->Count >>= 3U;
  /* !LINKSTO EB_Crypto_CryptAlgos_0308,2 */
  /* !LINKSTO EB_Crypto_CryptAlgos_0398,1 */
  /* !LINKSTO EB_Crypto_CryptAlgos_0388,1 */
  /* !LINKSTO EB_Crypto_CryptAlgos_0378,1 */
  /* 2. append padding to CtxPtr->Common.resultError_context */
  CtxPtr->Data = CtxPtr->SHAPad;

  if(CRYPTO_XVIX_XAIX_SHA2_224_256_WORD_SIZE == CtxPtr->CurrentWordSize)
  {
    CtxPtr->DataLength =
    (((CRYPTO_XVIX_XAIX_SHA_224_256_BLOCK_LEN_BYTES - 9U) - (CtxPtr->Count)) & (0x3fU)) + 1U;
  }
  /* CHECK: NOPARSE */
  /*CurrentWordSize can only have valid value from configuration.*/
  else if(CRYPTO_XVIX_XAIX_SHA2_384_512_WORD_SIZE == CtxPtr->CurrentWordSize)
  {
    CtxPtr->DataLength =
    (((uint32) (CRYPTO_XVIX_XAIX_SHA_384_512_BW_BLOCK_LEN_BYTES - 17U) - (CtxPtr->Count)) & (0x7fU)) + 1U;
  }
  else
  {
    /* unreachable code but needed for MISRAC2012*/
  }
  /* CHECK: PARSE */

  CtxPtr->State = CRYPTO_XVIX_XAIX_SHA_S_FINISH_UPDATE1;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHACallbackCancel
(
  P2VAR(Crypto_xVIx_xAIx_SHACtxType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CtxPtr
)
{
  /* !LINKSTO SWS_Crypto_00143_CORRECTION_SWS_Crypto_00181, 1, SWS_Crypto_00144_CORRECTION, 1 */
  if(TRUE == CtxPtr->CancelFlag)
  {
    if (
         (NULL_PTR != CtxPtr->ResultError)
         && (NULL_PTR != CtxPtr->IsBusy)
       )
    {
      *CtxPtr->IsBusy = FALSE;
      *CtxPtr->ResultError = CRYPTO_E_JOB_CANCELED;
      /* processingType is synchronous */
      /* !LINKSTO Crypto.Dsn.JobTransition.ActiveCancel2Idle, 1 */
      CtxPtr->CfgPtr->Job->jobState = CRYPTO_JOBSTATE_IDLE;
      SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      Crypto_xVIx_xAIx_DriverObjects[CtxPtr->ObjId].DriverObjectState =
      CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
      Crypto_xVIx_xAIx_DriverObjects[CtxPtr->ObjId].CurrentJob =
      NULL_PTR;
    Crypto_xVIx_xAIx_DriverObjects[CtxPtr->ObjId].SkipPeriodicMainFunction = TRUE;
      SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
    }
    CtxPtr->CfgPtr->Crypto_Generic_Callback
    (
      CtxPtr->CfgPtr->Job,
      CRYPTO_E_JOB_CANCELED
    );
  }
  else
  {
    if(TRUE == CtxPtr->CallbackFlag)
    {
      if (
           (NULL_PTR != CtxPtr->ResultError)
           && (NULL_PTR != CtxPtr->IsBusy)
         )
      {
        *CtxPtr->IsBusy = FALSE;
        *CtxPtr->ResultError = CtxPtr->ErrorCode;
      }
      if(
          (
            (CRYPTO_PROCESSING_ASYNC == CtxPtr->Job->jobPrimitiveInfo->processingType) &&
            (
               CRYPTO_HASH ==
               CtxPtr->Job->jobPrimitiveInfo->primitiveInfo->service
            ) &&
            (
               CRYPTO_ALGOFAM_NOT_SET ==
               CtxPtr->Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily
            )
          ) && (
                  (
                     ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL) ==
                     (
                       ((uint8) CtxPtr->Job->jobPrimitiveInputOutput.mode) &
                       ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
                     )
                  ) || (
                          ((uint8) CRYPTO_OPERATIONMODE_STREAMSTART) ==
                          (
                            ((uint8) CtxPtr->Job->jobPrimitiveInputOutput.mode) &
                            ((uint8) CRYPTO_OPERATIONMODE_STREAMSTART)
                          )
                       )
               ) && (CtxPtr->CurrentMode == CRYPTO_OPERATIONMODE_START)
        )
      {
        switch(CtxPtr->Job->jobPrimitiveInfo->primitiveInfo->algorithm.family)
        {
#if (CRYPTO_XVIX_XAIX_SHA2_224_ENABLED == STD_ON)
          case CRYPTO_ALGOFAM_SHA2_224:
          (void)Crypto_xVIx_xAIx_AL_HASH_SHA2_224_NOT_SET_Update(CtxPtr->ObjId, CtxPtr->Job);
          break;
#endif

#if (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON)
          case CRYPTO_ALGOFAM_SHA2_256:
          (void)Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_Update(CtxPtr->ObjId, CtxPtr->Job);
          break;
#endif

#if (CRYPTO_XVIX_XAIX_SHA2_384_ENABLED == STD_ON)
          case CRYPTO_ALGOFAM_SHA2_384:
          (void)Crypto_xVIx_xAIx_AL_HASH_SHA2_384_NOT_SET_Update(CtxPtr->ObjId, CtxPtr->Job);
          break;
#endif

#if (CRYPTO_XVIX_XAIX_SHA2_512_ENABLED == STD_ON)
          case CRYPTO_ALGOFAM_SHA2_512:
          (void)Crypto_xVIx_xAIx_AL_HASH_SHA2_512_NOT_SET_Update(CtxPtr->ObjId, CtxPtr->Job);
          break;
#endif
          /* CHECK: NOPARSE */
          /*All the possible valid algorithm families addressed in the cases above.*/
          default:
          {
            /* Caution: this 'default' is required by MISRA-C:2012 */
          }
          break;
          /* CHECK: PARSE */
        }
      }
      else
      {
        if(
            (
              (CRYPTO_PROCESSING_ASYNC == CtxPtr->Job->jobPrimitiveInfo->processingType) &&
              (
                 CRYPTO_HASH ==
                 CtxPtr->Job->jobPrimitiveInfo->primitiveInfo->service
              ) && (
                      CRYPTO_ALGOFAM_NOT_SET ==
                      CtxPtr->Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily
                   )
            ) && (
                    (
                       ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL) ==
                       (
                          ((uint8) CtxPtr->Job->jobPrimitiveInputOutput.mode) &
                          ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
                       )
                    )
                 ) && (CtxPtr->CurrentMode == CRYPTO_OPERATIONMODE_UPDATE)
          )
        {
          switch(CtxPtr->Job->jobPrimitiveInfo->primitiveInfo->algorithm.family)
          {
#if (CRYPTO_XVIX_XAIX_SHA2_224_ENABLED == STD_ON)
            case CRYPTO_ALGOFAM_SHA2_224:
            (void)Crypto_xVIx_xAIx_AL_HASH_SHA2_224_NOT_SET_Finish(CtxPtr->ObjId, CtxPtr->Job);
            break;
#endif

#if (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON)
            case CRYPTO_ALGOFAM_SHA2_256:
            (void)Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_Finish(CtxPtr->ObjId, CtxPtr->Job);
            break;
#endif

#if (CRYPTO_XVIX_XAIX_SHA2_384_ENABLED == STD_ON)
            case CRYPTO_ALGOFAM_SHA2_384:
            (void)Crypto_xVIx_xAIx_AL_HASH_SHA2_384_NOT_SET_Finish(CtxPtr->ObjId, CtxPtr->Job);
            break;
#endif

#if (CRYPTO_XVIX_XAIX_SHA2_512_ENABLED == STD_ON)
            case CRYPTO_ALGOFAM_SHA2_512:
            (void)Crypto_xVIx_xAIx_AL_HASH_SHA2_512_NOT_SET_Finish(CtxPtr->ObjId, CtxPtr->Job);
            break;
#endif

            /* CHECK: NOPARSE */
            /*All the possible valid algorithm families addressed in the cases above.*/
            default:
            {
              /* Caution: this 'default' is required by MISRA-C:2012 */
            }
            break;
            /* CHECK: PARSE */
          }
        }
      }
      CtxPtr->CfgPtr->Crypto_Generic_Callback
      (
        CtxPtr->CfgPtr->Job,
        CtxPtr->ErrorCode
      );
      CtxPtr->CallbackFlag = FALSE;
    }
  }
}

#if (CRYPTO_XVIX_XAIX_SHA2_224_ENABLED == STD_ON)
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2224HelperSynchronousMainCall
(
  void
)
{
  Crypto_xVIx_xAIx_HelperBusy = TRUE;
  do
  {
    Crypto_xVIx_xAIx_SHA2MainFunction(&Crypto_xVIx_xAIx_HelperResult, &Crypto_xVIx_xAIx_HelperBusy);
  }
  while ( TRUE == Crypto_xVIx_xAIx_HelperBusy );
  return Crypto_xVIx_xAIx_HelperResult;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_224_NOT_SET_CallBack
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint8                                            Result
)
{
  if(CRYPTO_PROCESSING_ASYNC == Job->jobPrimitiveInfo->processingType)
  {
    if(
        (
          CRYPTO_OPERATIONMODE_FINISH ==
          (CRYPTO_OPERATIONMODE_FINISH & Crypto_xVIx_xAIx_SHA2Ctx.CurrentMode)
        ) || (E_OK != Result)
      )
    {
      /* !LINKSTO SWS_Crypto_00025, 1 */
      /* !LINKSTO SWS_Crypto_00119, 1 */
      /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Idle, 1 */
      /* !LINKSTO Crypto.Dsn.JobTransition.ActiveError2Idle, 1 */
      Job->jobState = CRYPTO_JOBSTATE_IDLE;
      Crypto_xVIx_xAIx_SHA2Ctx.CurrentMode = CRYPTO_XVIX_XAIX_HASH_DEFAULT_CURRENT_MODE;
      /* !LINKSTO EB_Crypto_01050, 1 */
      SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_SHA2Ctx.ObjId].DriverObjectState =
        CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_SHA2Ctx.ObjId].CurrentJob = NULL_PTR;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_SHA2Ctx.ObjId].SkipPeriodicMainFunction = TRUE;
      SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      CryIf_CallbackNotification(Job, Result);
      Crypto_xVIx_xAIx_AL_HASH_SHA2_NOT_SET_Reset();
    }
    if(
        (
          CRYPTO_OPERATIONMODE_UPDATE ==
            (CRYPTO_OPERATIONMODE_UPDATE & Crypto_xVIx_xAIx_SHA2Ctx.CurrentMode)
        ) && (
              (
                ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL) !=
                  (
                    ((uint8) Crypto_xVIx_xAIx_SHA2Ctx.Job->jobPrimitiveInputOutput.mode) &
                      ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
                   )
              )
             )
        )
    {
      CryIf_CallbackNotification(Job, Result);
    }
  }
}

#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_224_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON)
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2256HelperSynchronousMainCall
(
  void
)
{
  Crypto_xVIx_xAIx_HelperBusy = TRUE;
  do
  {
    Crypto_xVIx_xAIx_SHA2MainFunction(&Crypto_xVIx_xAIx_HelperResult, &Crypto_xVIx_xAIx_HelperBusy);
  }
  while ( TRUE == Crypto_xVIx_xAIx_HelperBusy );
  return Crypto_xVIx_xAIx_HelperResult;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2256HelperCallbackNotification
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  Std_ReturnType                                               ResultError
)
{
  Crypto_xVIx_xAIx_HelperHashFlgCallbackNotification = TRUE;
  Crypto_xVIx_xAIx_HelperHashRetCallbackNotification = ResultError;
  if(NULL_PTR != Crypto_xVIx_xAIx_SHA_CallBack)
  {
    Crypto_xVIx_xAIx_SHA_CallBack(Job, ResultError);
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_256_NOT_SET_CallBack
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint8                                                        Result
)
{
  if(CRYPTO_PROCESSING_ASYNC == Job->jobPrimitiveInfo->processingType)
  {
    if(
        (
          CRYPTO_OPERATIONMODE_FINISH ==
          (CRYPTO_OPERATIONMODE_FINISH & Crypto_xVIx_xAIx_SHA2Ctx.CurrentMode)
        ) || (E_OK != Result)
      )
    {
      /* !LINKSTO SWS_Crypto_00025, 1 */
      /* !LINKSTO SWS_Crypto_00119, 1 */
      /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Idle, 1 */
      /* !LINKSTO Crypto.Dsn.JobTransition.ActiveError2Idle, 1 */
      Job->jobState = CRYPTO_JOBSTATE_IDLE;
      Crypto_xVIx_xAIx_SHA2Ctx.CurrentMode = CRYPTO_XVIX_XAIX_HASH_DEFAULT_CURRENT_MODE;
      /* !LINKSTO EB_Crypto_01050, 1 */
      SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_SHA2Ctx.ObjId].DriverObjectState =
        CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_SHA2Ctx.ObjId].CurrentJob = NULL_PTR;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_SHA2Ctx.ObjId].SkipPeriodicMainFunction = TRUE;
      SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      CryIf_CallbackNotification(Job, Result);
      Crypto_xVIx_xAIx_AL_HASH_SHA2_NOT_SET_Reset();
    }
    if(
        (
          CRYPTO_OPERATIONMODE_UPDATE ==
            (CRYPTO_OPERATIONMODE_UPDATE & Crypto_xVIx_xAIx_SHA2Ctx.CurrentMode)
        ) && (
              (
                ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL) !=
                  (
                    ((uint8) Crypto_xVIx_xAIx_SHA2Ctx.Job->jobPrimitiveInputOutput.mode) &
                      ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
                   )
              )
             )
        )
    {
      CryIf_CallbackNotification(Job, Result);
    }
  }
}

#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_SHA2_384_ENABLED == STD_ON)
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2384HelperSynchronousMainCall
(
  void
)
{
  Crypto_xVIx_xAIx_HelperBusy = TRUE;
  do
  {
    Crypto_xVIx_xAIx_SHA2MainFunction(&Crypto_xVIx_xAIx_HelperResult, &Crypto_xVIx_xAIx_HelperBusy);
  }
  while( TRUE == Crypto_xVIx_xAIx_HelperBusy );
  return Crypto_xVIx_xAIx_HelperResult;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_384_NOT_SET_CallBack
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint8                                                        Result
)
{
  if(CRYPTO_PROCESSING_ASYNC == Job->jobPrimitiveInfo->processingType)
  {
    if(
        (
          CRYPTO_OPERATIONMODE_FINISH ==
          (CRYPTO_OPERATIONMODE_FINISH & Crypto_xVIx_xAIx_SHA2Ctx.CurrentMode)
        ) || (E_OK != Result)
      )
    {
      /* !LINKSTO SWS_Crypto_00025, 1 */
      /* !LINKSTO SWS_Crypto_00119, 1 */
      /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Idle, 1 */
      /* !LINKSTO Crypto.Dsn.JobTransition.ActiveError2Idle, 1 */
      Job->jobState = CRYPTO_JOBSTATE_IDLE;
      Crypto_xVIx_xAIx_SHA2Ctx.CurrentMode = CRYPTO_XVIX_XAIX_HASH_DEFAULT_CURRENT_MODE;
      /* !LINKSTO EB_Crypto_01050, 1 */
      SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_SHA2Ctx.ObjId].DriverObjectState =
        CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_SHA2Ctx.ObjId].CurrentJob = NULL_PTR;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_SHA2Ctx.ObjId].SkipPeriodicMainFunction = TRUE;
      SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      CryIf_CallbackNotification(Job, Result);
      Crypto_xVIx_xAIx_AL_HASH_SHA2_NOT_SET_Reset();
    }
    if(
        (
          CRYPTO_OPERATIONMODE_UPDATE ==
            (CRYPTO_OPERATIONMODE_UPDATE & Crypto_xVIx_xAIx_SHA2Ctx.CurrentMode)
        ) && (
              (
                ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL) !=
                  (
                    ((uint8) Crypto_xVIx_xAIx_SHA2Ctx.Job->jobPrimitiveInputOutput.mode) &
                      ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
                   )
              )
             )
        )
    {
      CryIf_CallbackNotification(Job, Result);
    }
  }
}

#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_384_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_SHA2_512_ENABLED == STD_ON)
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2512HelperSynchronousMainCall
(
  void
)
{
  Crypto_xVIx_xAIx_HelperBusy = TRUE;
  do
  {
    Crypto_xVIx_xAIx_SHA2MainFunction(&Crypto_xVIx_xAIx_HelperResult, &Crypto_xVIx_xAIx_HelperBusy);
  }
  while ( TRUE == Crypto_xVIx_xAIx_HelperBusy );
  return Crypto_xVIx_xAIx_HelperResult;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_HASH_SHA2_512_NOT_SET_CallBack
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint8                                                        Result
)
{
  if(CRYPTO_PROCESSING_ASYNC == Job->jobPrimitiveInfo->processingType)
  {
    if(
        (
          CRYPTO_OPERATIONMODE_FINISH ==
          (CRYPTO_OPERATIONMODE_FINISH & Crypto_xVIx_xAIx_SHA2Ctx.CurrentMode)
        ) || (E_OK != Result)
      )
    {
      /* !LINKSTO SWS_Crypto_00025, 1 */
      /* !LINKSTO SWS_Crypto_00119, 1 */
      /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Idle, 1 */
      /* !LINKSTO Crypto.Dsn.JobTransition.ActiveError2Idle, 1 */
      Job->jobState = CRYPTO_JOBSTATE_IDLE;
      Crypto_xVIx_xAIx_SHA2Ctx.CurrentMode = CRYPTO_XVIX_XAIX_HASH_DEFAULT_CURRENT_MODE;
      /* !LINKSTO EB_Crypto_01050, 1 */
      SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_SHA2Ctx.ObjId].DriverObjectState =
        CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_SHA2Ctx.ObjId].CurrentJob = NULL_PTR;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_SHA2Ctx.ObjId].SkipPeriodicMainFunction = TRUE;
      SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      CryIf_CallbackNotification(Job, Result);
      Crypto_xVIx_xAIx_AL_HASH_SHA2_NOT_SET_Reset();
    }
    if(
        (
          CRYPTO_OPERATIONMODE_UPDATE ==
            (CRYPTO_OPERATIONMODE_UPDATE & Crypto_xVIx_xAIx_SHA2Ctx.CurrentMode)
        ) && (
              (
                ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL) !=
                  (
                    ((uint8) Crypto_xVIx_xAIx_SHA2Ctx.Job->jobPrimitiveInputOutput.mode) &
                      ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
                   )
              )
             )
        )
    {
      CryIf_CallbackNotification(Job, Result);
    }
  }
}

#endif /* #if (CRYPTO_XVIX_XAIX_SHA2_512_ENABLED == STD_ON) */

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2Init
(
  void
)
{
  switch (Crypto_xVIx_xAIx_SHA2Ctx.CfgPtr->HashLength)
  {
#if (CRYPTO_XVIX_XAIX_SHA2_224_ENABLED == STD_ON)
    /* 2. Initialize the internal state of the hash function */
    case CRYPTO_XVIX_XAIX_SHA224_HASH_LEN_BYTES:
    /* !LINKSTO EB_Crypto_CryptAlgos_0379,1 */
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[0U] = (uint32) 0xc1059ed8UL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[1U] = (uint32) 0x367cd507UL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[2U] = (uint32) 0x3070dd17UL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[3U] = (uint32) 0xf70e5939UL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[4U] = (uint32) 0xffc00b31UL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[5U] = (uint32) 0x68581511UL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[6U] = (uint32) 0x64f98fa7UL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[7U] = (uint32) 0xbefa4fa4UL;
    break;
#endif

#if (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON)
    case CRYPTO_XVIX_XAIX_SHA256_HASH_LEN_BYTES:
    /* !LINKSTO EB_Crypto_CryptAlgos_0389,1 */
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[0U] = (uint32) 0x6a09e667UL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[1U] = (uint32) 0xbb67ae85UL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[2U] = (uint32) 0x3c6ef372UL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[3U] = (uint32) 0xa54ff53aUL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[4U] = (uint32) 0x510e527fUL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[5U] = (uint32) 0x9b05688cUL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[6U] = (uint32) 0x1f83d9abUL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[7U] = (uint32) 0x5be0cd19UL;
    break;
#endif

#if (CRYPTO_XVIX_XAIX_SHA2_384_ENABLED == STD_ON)
    case CRYPTO_XVIX_XAIX_SHA384_HASH_LEN_BYTES:
    /* !LINKSTO EB_Crypto_CryptAlgos_0399,1 */
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[0U] = (uint32) 0xc1059ed8UL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[1U] = (uint32) 0xcbbb9d5dUL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[2U] = (uint32) 0x367cd507UL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[3U] = (uint32) 0x629a292aUL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[4U] = (uint32) 0x3070dd17UL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[5U] = (uint32) 0x9159015aUL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[6U] = (uint32) 0xf70e5939UL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[7U] = (uint32) 0x152fecd8UL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[8U] = (uint32) 0xffc00b31UL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[9U] = (uint32) 0x67332667UL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[10U] = (uint32) 0x68581511UL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[11U] = (uint32) 0x8eb44a87UL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[12U] = (uint32) 0x64f98fa7UL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[13U] = (uint32) 0xdb0c2e0dUL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[14U] = (uint32) 0xbefa4fa4UL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[15U] = (uint32) 0x47b5481dUL;
    break;
#endif

#if (CRYPTO_XVIX_XAIX_SHA2_512_ENABLED == STD_ON)
    case CRYPTO_XVIX_XAIX_SHA512_HASH_LEN_BYTES:
    /* !LINKSTO EB_Crypto_CryptAlgos_0309,1 */
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[0U] = (uint32) 0xf3bcc908UL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[1U] = (uint32) 0x6a09e667UL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[2U] = (uint32) 0x84caa73bUL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[3U] = (uint32) 0xbb67ae85UL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[4U] = (uint32) 0xfe94f82bUL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[5U] = (uint32) 0x3c6ef372UL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[6U] = (uint32) 0x5f1d36f1UL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[7U] = (uint32) 0xa54ff53aUL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[8U] = (uint32) 0xade682d1UL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[9U] = (uint32) 0x510e527fUL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[10U] = (uint32) 0x2b3e6c1fUL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[11U] = (uint32) 0x9b05688cUL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[12U] = (uint32) 0xfb41bd6bUL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[13U] = (uint32) 0x1f83d9abUL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[14U] = (uint32) 0x137e2179UL;
    Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[15U] = (uint32) 0x5be0cd19UL;
    break;
#endif

    /* CHECK: NOPARSE */
    /*all the valid hash lengths are addressed in the cases above.*/
    default:
    {
      /* Caution: this 'default' is required by MISRA-C:2012 */
    }
    break;
    /* CHECK: PARSE */
  }
}

#if ((CRYPTO_XVIX_XAIX_SHA2_224_ENABLED == STD_ON) || \
     (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON))

static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA224256Compress
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Data
)
{
  Crypto_xVIx_xAIx_SHA2Ctx.A[0U] = Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[0U];
  Crypto_xVIx_xAIx_SHA2Ctx.B[0U] = Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[1U];
  Crypto_xVIx_xAIx_SHA2Ctx.C[0U] = Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[2U];
  Crypto_xVIx_xAIx_SHA2Ctx.D[0U] = Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[3U];
  Crypto_xVIx_xAIx_SHA2Ctx.E[0U] = Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[4U];
  Crypto_xVIx_xAIx_SHA2Ctx.F[0U] = Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[5U];
  Crypto_xVIx_xAIx_SHA2Ctx.G[0U] = Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[6U];
  Crypto_xVIx_xAIx_SHA2Ctx.H[0U] = Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[7U];

  /* compress expansion */
  for (Crypto_xVIx_xAIx_SHA2Ctx.I = 0U;
       Crypto_xVIx_xAIx_SHA2Ctx.I < CRYPTO_XVIX_XAIX_SHA224256_ROUNDS;
       Crypto_xVIx_xAIx_SHA2Ctx.I += 8U
      )
  {
    /* Prepare the message schedule */
    if (Crypto_xVIx_xAIx_SHA2Ctx.I < (uint8)(CRYPTO_XVIX_XAIX_SHA_224_256_BLOCK_LEN_BYTES >> 2U))
    {
       /* copy the first 64 bytes from the user input to the message schedule */
       Crypto_xVIx_xAIx_SHA2Ctx.W[(uint8)(Crypto_xVIx_xAIx_SHA2Ctx.I + 0U)] =
        CRYPTO_XVIX_XAIX_SHA224256_READ_WORD(Data, (uint8)((Crypto_xVIx_xAIx_SHA2Ctx.I << 2U) +  0U));
       Crypto_xVIx_xAIx_SHA2Ctx.W[(uint8)(Crypto_xVIx_xAIx_SHA2Ctx.I + 1U)] =
        CRYPTO_XVIX_XAIX_SHA224256_READ_WORD(Data, (uint8)((Crypto_xVIx_xAIx_SHA2Ctx.I << 2U) +  4U));
       Crypto_xVIx_xAIx_SHA2Ctx.W[(uint8)(Crypto_xVIx_xAIx_SHA2Ctx.I + 2U)] =
        CRYPTO_XVIX_XAIX_SHA224256_READ_WORD(Data, (uint8)((Crypto_xVIx_xAIx_SHA2Ctx.I << 2U) +  8U));
       Crypto_xVIx_xAIx_SHA2Ctx.W[(uint8)(Crypto_xVIx_xAIx_SHA2Ctx.I + 3U)] =
        CRYPTO_XVIX_XAIX_SHA224256_READ_WORD(Data, (uint8)((Crypto_xVIx_xAIx_SHA2Ctx.I << 2U) + 12U));
       Crypto_xVIx_xAIx_SHA2Ctx.W[(uint8)(Crypto_xVIx_xAIx_SHA2Ctx.I + 4U)] =
        CRYPTO_XVIX_XAIX_SHA224256_READ_WORD(Data, (uint8)((Crypto_xVIx_xAIx_SHA2Ctx.I << 2U) + 16U));
       Crypto_xVIx_xAIx_SHA2Ctx.W[(uint8)(Crypto_xVIx_xAIx_SHA2Ctx.I + 5U)] =
        CRYPTO_XVIX_XAIX_SHA224256_READ_WORD(Data, (uint8)((Crypto_xVIx_xAIx_SHA2Ctx.I << 2U) + 20U));
       Crypto_xVIx_xAIx_SHA2Ctx.W[(uint8)(Crypto_xVIx_xAIx_SHA2Ctx.I + 6U)] =
        CRYPTO_XVIX_XAIX_SHA224256_READ_WORD(Data, (uint8)((Crypto_xVIx_xAIx_SHA2Ctx.I << 2U) + 24U));
       Crypto_xVIx_xAIx_SHA2Ctx.W[(uint8)(Crypto_xVIx_xAIx_SHA2Ctx.I + 7U)] =
        CRYPTO_XVIX_XAIX_SHA224256_READ_WORD(Data, (uint8)((Crypto_xVIx_xAIx_SHA2Ctx.I << 2U) + 28U));
    }
    else
    {
       /* each word is constructed recursively from 4 previous words */
       CRYPTO_XVIX_XAIX_SHA224256_CALCULATE_SCHEDULE_WORD((Crypto_xVIx_xAIx_SHA2Ctx.I & (uint8)0x0fU) + 0U);
       CRYPTO_XVIX_XAIX_SHA224256_CALCULATE_SCHEDULE_WORD((Crypto_xVIx_xAIx_SHA2Ctx.I & (uint8)0x0fU) + 1U);
       CRYPTO_XVIX_XAIX_SHA224256_CALCULATE_SCHEDULE_WORD((Crypto_xVIx_xAIx_SHA2Ctx.I & (uint8)0x0fU) + 2U);
       CRYPTO_XVIX_XAIX_SHA224256_CALCULATE_SCHEDULE_WORD((Crypto_xVIx_xAIx_SHA2Ctx.I & (uint8)0x0fU) + 3U);
       CRYPTO_XVIX_XAIX_SHA224256_CALCULATE_SCHEDULE_WORD((Crypto_xVIx_xAIx_SHA2Ctx.I & (uint8)0x0fU) + 4U);
       CRYPTO_XVIX_XAIX_SHA224256_CALCULATE_SCHEDULE_WORD((Crypto_xVIx_xAIx_SHA2Ctx.I & (uint8)0x0fU) + 5U);
       CRYPTO_XVIX_XAIX_SHA224256_CALCULATE_SCHEDULE_WORD((Crypto_xVIx_xAIx_SHA2Ctx.I & (uint8)0x0fU) + 6U);
       CRYPTO_XVIX_XAIX_SHA224256_CALCULATE_SCHEDULE_WORD((Crypto_xVIx_xAIx_SHA2Ctx.I & (uint8)0x0fU) + 7U);
    }

    /* Perform the round operations */
    CRYPTO_XVIX_XAIX_SHA224256_ROUND_OPERATION(Crypto_xVIx_xAIx_SHA2Ctx.A[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.B[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.C[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.D[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.E[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.F[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.G[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.H[0U],
                                               (Crypto_xVIx_xAIx_SHA2Ctx.I + 0U)
                                              );

    CRYPTO_XVIX_XAIX_SHA224256_ROUND_OPERATION(Crypto_xVIx_xAIx_SHA2Ctx.H[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.A[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.B[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.C[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.D[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.E[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.F[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.G[0U],
                                               (Crypto_xVIx_xAIx_SHA2Ctx.I + 1U)
                                              );

    CRYPTO_XVIX_XAIX_SHA224256_ROUND_OPERATION(Crypto_xVIx_xAIx_SHA2Ctx.G[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.H[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.A[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.B[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.C[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.D[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.E[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.F[0U],
                                               (Crypto_xVIx_xAIx_SHA2Ctx.I + 2U)
                                              );

    CRYPTO_XVIX_XAIX_SHA224256_ROUND_OPERATION(Crypto_xVIx_xAIx_SHA2Ctx.F[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.G[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.H[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.A[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.B[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.C[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.D[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.E[0U],
                                               (Crypto_xVIx_xAIx_SHA2Ctx.I + 3U)
                                              );

    CRYPTO_XVIX_XAIX_SHA224256_ROUND_OPERATION(Crypto_xVIx_xAIx_SHA2Ctx.E[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.F[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.G[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.H[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.A[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.B[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.C[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.D[0U],
                                               (Crypto_xVIx_xAIx_SHA2Ctx.I + 4U)
                                              );

    CRYPTO_XVIX_XAIX_SHA224256_ROUND_OPERATION(Crypto_xVIx_xAIx_SHA2Ctx.D[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.E[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.F[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.G[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.H[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.A[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.B[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.C[0U],
                                               (Crypto_xVIx_xAIx_SHA2Ctx.I + 5U)
                                              );

    CRYPTO_XVIX_XAIX_SHA224256_ROUND_OPERATION(Crypto_xVIx_xAIx_SHA2Ctx.C[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.D[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.E[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.F[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.G[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.H[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.A[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.B[0U],
                                               (Crypto_xVIx_xAIx_SHA2Ctx.I + 6U)
                                              );

    CRYPTO_XVIX_XAIX_SHA224256_ROUND_OPERATION(Crypto_xVIx_xAIx_SHA2Ctx.B[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.C[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.D[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.E[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.F[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.G[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.H[0U],
                                               Crypto_xVIx_xAIx_SHA2Ctx.A[0U],
                                               (Crypto_xVIx_xAIx_SHA2Ctx.I + 7U)
                                              );
  }

  /* 3. copy internal state back to Ctx */
  Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[0U] += Crypto_xVIx_xAIx_SHA2Ctx.A[0U];
  Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[1U] += Crypto_xVIx_xAIx_SHA2Ctx.B[0U];
  Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[2U] += Crypto_xVIx_xAIx_SHA2Ctx.C[0U];
  Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[3U] += Crypto_xVIx_xAIx_SHA2Ctx.D[0U];
  Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[4U] += Crypto_xVIx_xAIx_SHA2Ctx.E[0U];
  Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[5U] += Crypto_xVIx_xAIx_SHA2Ctx.F[0U];
  Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[6U] += Crypto_xVIx_xAIx_SHA2Ctx.G[0U];
  Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[7U] += Crypto_xVIx_xAIx_SHA2Ctx.H[0U];

  return TRUE;
}

#endif /* #if ((CRYPTO_XVIX_XAIX_SHA2_224_ENABLED == STD_ON) ||
               (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON)) */

#if ((CRYPTO_XVIX_XAIX_SHA2_384_ENABLED == STD_ON) || \
     (CRYPTO_XVIX_XAIX_SHA2_512_ENABLED == STD_ON))

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_Add
(
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Ans,
  P2CONST(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) N,
  P2CONST(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) M
)
{
  Ans[1U] = N[1U] + M[1U] +
  (
    (((N[0U] + M[0U]) < N[0U]) || ((N[0U] + M[0U]) < M[0U])) ? (uint32) 1U : (uint32) 0U
  );
  Ans[0U] = N[0U] + M[0U];
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA384512RotR
(
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Word,
  uint32                                               Offset1
)
{
  uint32 AccLower = 0U, AccUpper = 0U;

  if(Offset1 > 32U)
  {
    uint32 Tmp0, Tmp1;

    Tmp0 = Word[0U];
    Tmp1 = Word[1U];

    Offset1 -= 32U;

    AccLower = Word[0U] << (32U - Offset1);
    AccUpper = Word[1U] << (32U - Offset1);

    Word[1U] = AccUpper | (Tmp0 >> Offset1);
    Word[0U] = AccLower | (Tmp1 >> Offset1);
  }
  else
  {
    AccLower = Word[0U] << (32U - Offset1);
    AccUpper = Word[1U] << (32U - Offset1);

    Word[0U] = AccUpper | (Word[0U] >> Offset1);
    Word[1U] = AccLower | (Word[1U] >> Offset1);
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA384512ShR
(
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Word,
  uint32                                               Offset2
)
{
  uint32 AccUpper = 0U;

  AccUpper = Word[1U] << (32U - Offset2);

  Word[0U] = AccUpper | (Word[0U] >> Offset2);
  Word[1U] = (Word[1U] >> Offset2);
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA384512CapitalSigma_0
(
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Word
)
{
  uint32 W[2U], Rot1[2U], Rot2[2U], Rot3[2U];
  W[0U] = Word[0U];
  W[1U] = Word[1U];

  Crypto_xVIx_xAIx_SHA384512RotR(Word, 28U);
  Rot1[0U] = Word[0U];
  Rot1[1U] = Word[1U];

  Word[0U] = W[0U];
  Word[1U] = W[1U];
  Crypto_xVIx_xAIx_SHA384512RotR(Word, 34U);
  Rot2[0U] = Word[0U];
  Rot2[1U] = Word[1U];

  Word[0U] = W[0U];
  Word[1U] = W[1U];
  Crypto_xVIx_xAIx_SHA384512RotR(Word, 39U);
  Rot3[0U] = Word[0U];
  Rot3[1U] = Word[1U];

  Word[0U] = Rot1[0U] ^ Rot2[0U] ^ Rot3[0U];
  Word[1U] = Rot1[1U] ^ Rot2[1U] ^ Rot3[1U];
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA384512CapitalSigma_1
(
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Word
)
{
  uint32 W[2U], Rot1[2U], Rot2[2U], Rot3[2U];
  W[0U] = Word[0U];
  W[1U] = Word[1U];

  Crypto_xVIx_xAIx_SHA384512RotR(Word, 14U);
  Rot1[0U] = Word[0U];
  Rot1[1U] = Word[1U];

  Word[0U] = W[0U];
  Word[1U] = W[1U];
  Crypto_xVIx_xAIx_SHA384512RotR(Word, 18U);
  Rot2[0U] = Word[0U];
  Rot2[1U] = Word[1U];

  Word[0U] = W[0U];
  Word[1U] = W[1U];
  Crypto_xVIx_xAIx_SHA384512RotR(Word, 41U);
  Rot3[0U] = Word[0U];
  Rot3[1U] = Word[1U];

  Word[0U] = Rot1[0U] ^ Rot2[0U] ^ Rot3[0U];
  Word[1U] = Rot1[1U] ^ Rot2[1U] ^ Rot3[1U];
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA384512SmallSigma_0
(
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Word
)
{
  uint32 W[2U], Rot1[2U], Rot2[2U], Shr[2U];
  W[0U] = Word[0U];
  W[1U] = Word[1U];

  Crypto_xVIx_xAIx_SHA384512RotR(Word, 1U);
  Rot1[0U] = Word[0U];
  Rot1[1U] = Word[1U];

  Word[0U] = W[0U];
  Word[1U] = W[1U];
  Crypto_xVIx_xAIx_SHA384512RotR(Word, 8U);
  Rot2[0U] = Word[0U];
  Rot2[1U] = Word[1U];

  Word[0U] = W[0U];
  Word[1U] = W[1U];
  Crypto_xVIx_xAIx_SHA384512ShR(Word, 7U);
  Shr[0U] = Word[0U];
  Shr[1U] = Word[1U];

  Word[0U] = Rot1[0U] ^ Rot2[0U] ^ Shr[0U];
  Word[1U] = Rot1[1U] ^ Rot2[1U] ^ Shr[1U];
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA384512SmallSigma_1
(
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Word
)
{
  uint32 W[2U], Rot1[2U], Rot2[2U], Shr[2U];
  W[0U] = Word[0U];
  W[1U] = Word[1U];

  Crypto_xVIx_xAIx_SHA384512RotR(Word, 19U);
  Rot1[0U] = Word[0U];
  Rot1[1U] = Word[1U];

  Word[0U] = W[0U];
  Word[1U] = W[1U];
  Crypto_xVIx_xAIx_SHA384512RotR(Word, 61U);
  Rot2[0U] = Word[0U];
  Rot2[1U] = Word[1U];

  Word[0U] = W[0U];
  Word[1U] = W[1U];
  Crypto_xVIx_xAIx_SHA384512ShR(Word, 6U);
  Shr[0U] = Word[0U];
  Shr[1U] = Word[1U];

  Word[0U] = Rot1[0U] ^ Rot2[0U] ^ Shr[0U];
  Word[1U] = Rot1[1U] ^ Rot2[1U] ^ Shr[1U];
}

static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA384512Compress
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Data
)
{
  boolean Finished = FALSE;

  switch(Crypto_xVIx_xAIx_SHA2Ctx.SHACompressState)
  {
    case CRYPTO_XVIX_XAIX_SHA_S_COMPRESS:
    Crypto_xVIx_xAIx_SHA384512StateCompress();
    break;

    case CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_PRECSMC:
    Crypto_xVIx_xAIx_SHA384512StateCompressPrecsmc(Data);
    break;

    case CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_EXPANSION:
    Crypto_xVIx_xAIx_SHA384512StateCompressExpansion();
    break;

    case CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_TRANSFORM2:
    Crypto_xVIx_xAIx_SHA384512StateCompressTransform2();
    break;

    case CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_TRANSFORM3:
    Crypto_xVIx_xAIx_SHA384512StateCompressTransform3();
    break;

    case CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_EXPORT:
    Crypto_xVIx_xAIx_SHA384512StateCompressExport();
    Finished = TRUE;
    break;
    /* CHECK: NOPARSE */
    /*All the valid states are addressed in the above cases. This is internal state machine and the
      states are changed internally. It can not be called from outside mainfunction, to test the
      default case using CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_IDLE.*/
    /* Defensive Programming - unreachable code. This default case is required by MISRA-C:2012 */
    default:
    {}
    break;
    /* CHECK: PARSE */
  }
  return Finished;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA384512StateCompress
(
  void
)
{
  Crypto_xVIx_xAIx_SHA2Ctx.A[0U] = Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[0U];
  Crypto_xVIx_xAIx_SHA2Ctx.A[1U] = Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[1U];
  Crypto_xVIx_xAIx_SHA2Ctx.B[0U] = Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[2U];
  Crypto_xVIx_xAIx_SHA2Ctx.B[1U] = Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[3U];
  Crypto_xVIx_xAIx_SHA2Ctx.C[0U] = Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[4U];
  Crypto_xVIx_xAIx_SHA2Ctx.C[1U] = Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[5U];
  Crypto_xVIx_xAIx_SHA2Ctx.D[0U] = Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[6U];
  Crypto_xVIx_xAIx_SHA2Ctx.D[1U] = Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[7U];
  Crypto_xVIx_xAIx_SHA2Ctx.E[0U] = Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[8U];
  Crypto_xVIx_xAIx_SHA2Ctx.E[1U] = Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[9U];
  Crypto_xVIx_xAIx_SHA2Ctx.F[0U] = Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[10U];
  Crypto_xVIx_xAIx_SHA2Ctx.F[1U] = Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[11U];
  Crypto_xVIx_xAIx_SHA2Ctx.G[0U] = Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[12U];
  Crypto_xVIx_xAIx_SHA2Ctx.G[1U] = Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[13U];
  Crypto_xVIx_xAIx_SHA2Ctx.H[0U] = Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[14U];
  Crypto_xVIx_xAIx_SHA2Ctx.H[1U] = Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[15U];

  /* copy data to local buffer */
  Crypto_xVIx_xAIx_SHA2Ctx.I = (uint8) 0U;

  Crypto_xVIx_xAIx_SHA2Ctx.SHACompressState = CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_PRECSMC;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA384512StateCompressPrecsmc
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Data
)
{
  /* precalculate 8 * i once during the loop */
  Crypto_xVIx_xAIx_SHA2Ctx.K = Crypto_xVIx_xAIx_SHA2Ctx.I;
  Crypto_xVIx_xAIx_SHA2Ctx.K <<= 3U;

  Crypto_xVIx_xAIx_SHA2Ctx.W[(uint8) (Crypto_xVIx_xAIx_SHA2Ctx.I << (uint8) 1U)] =
  (uint32) ((uint32) Data[Crypto_xVIx_xAIx_SHA2Ctx.K + (uint8) 4U] << 24U) |
  (uint32) ((uint32) Data[Crypto_xVIx_xAIx_SHA2Ctx.K + (uint8) 5U] << 16U) |
  (uint32) ((uint32) Data[Crypto_xVIx_xAIx_SHA2Ctx.K + (uint8) 6U] << 8U) |
  (uint32) Data[Crypto_xVIx_xAIx_SHA2Ctx.K + (uint8) 7U];

  Crypto_xVIx_xAIx_SHA2Ctx.W[(uint8) (Crypto_xVIx_xAIx_SHA2Ctx.I << (uint8) 1U) + 1U] =
  (uint32) ((uint32) Data[Crypto_xVIx_xAIx_SHA2Ctx.K ] << 24U) |
  (uint32) ((uint32) Data[Crypto_xVIx_xAIx_SHA2Ctx.K + (uint8) 1U] << 16U) |
  (uint32) ((uint32) Data[Crypto_xVIx_xAIx_SHA2Ctx.K + (uint8) 2U] << 8U) |
  (uint32) Data[Crypto_xVIx_xAIx_SHA2Ctx.K + (uint8) 3U];

  ++Crypto_xVIx_xAIx_SHA2Ctx.I;

  if (Crypto_xVIx_xAIx_SHA2Ctx.I >= (uint8) (16U))
  {
    Crypto_xVIx_xAIx_SHA2Ctx.SHACompressState = CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_EXPANSION;

    /* 2. the message Crypto_xVIx_xAIx_SHA2Ctx.resultError transformation loop */
    Crypto_xVIx_xAIx_SHA2Ctx.I = (uint8) 0U;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA384512StateCompressExpansion
(
  void
)
{
  /* 2.2 user data expansion:
   * during the first 16 rounds, no expansion is needed. sha384 or sha512 uses
   * the user input data directly
   * from rounds 17 to 80, calculate the necessary word
   */
  if((uint8) (Crypto_xVIx_xAIx_SHA2Ctx.I & (uint8) 0xf0U) != (uint8) 0U)
  {
    uint32 Sigm0[2U];
    uint32 Sigm1[2U];
    uint32 AddTmp0[2U];
    uint32 AddTmp1[2U];

    /* The data is saved in W in machine endianness */
    AddTmp0[0U] = Crypto_xVIx_xAIx_SHA2Ctx.W[(uint8) ((Crypto_xVIx_xAIx_SHA2Ctx.I - (uint8) 2U) << 1U)];
    AddTmp0[1U] = Crypto_xVIx_xAIx_SHA2Ctx.W[((uint8) ((Crypto_xVIx_xAIx_SHA2Ctx.I - (uint8) 2U) << 1U) + 1U)];
    Crypto_xVIx_xAIx_SHA384512SmallSigma_1(AddTmp0);
    Sigm0[0U] = AddTmp0[0U];
    Sigm0[1U] = AddTmp0[1U];

    AddTmp0[0U] = Crypto_xVIx_xAIx_SHA2Ctx.W[(uint8) ((Crypto_xVIx_xAIx_SHA2Ctx.I - (uint8) 15U) << 1U)];
    AddTmp0[1U] = Crypto_xVIx_xAIx_SHA2Ctx.W[((uint8) ((Crypto_xVIx_xAIx_SHA2Ctx.I - (uint8) 15U) << 1U)) + 1U];
    Crypto_xVIx_xAIx_SHA384512SmallSigma_0(AddTmp0);
    Sigm1[0U] = AddTmp0[0U];
    Sigm1[1U] = AddTmp0[1U];

    /* the word is constructed recursively from 4 previous words */
    Crypto_xVIx_xAIx_Add(
                          AddTmp0,
                          Sigm0,
                          &Crypto_xVIx_xAIx_SHA2Ctx.W[(uint8) ((Crypto_xVIx_xAIx_SHA2Ctx.I - (uint8) 7U) << 1U)]
                        );
    Crypto_xVIx_xAIx_Add(
                          AddTmp1,
                          Sigm1,
                          &Crypto_xVIx_xAIx_SHA2Ctx.W[(uint8) ((Crypto_xVIx_xAIx_SHA2Ctx.I - (uint8) 16U) << 1U)]
                        );
    Crypto_xVIx_xAIx_Add(
                          &Crypto_xVIx_xAIx_SHA2Ctx.W[(uint8) (Crypto_xVIx_xAIx_SHA2Ctx.I << (uint8) 1U)],
                          AddTmp0,
                          AddTmp1
                        );
  }
  Crypto_xVIx_xAIx_SHA2Ctx.SHACompressState = CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_TRANSFORM2;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA384512StateCompressTransform2
(
  void
)
{
  uint32 Sigm[2U];
  uint32 Ch[2U];
  uint32 Maj[2U];
  uint32 AddTmp0[2U];
  uint32 AddTmp1[2U];
  uint32 AddTmp2[2U];
  uint32 AddTmp3[2U];

  /* Capital sigma 1 */
  AddTmp0[0U] = Crypto_xVIx_xAIx_SHA2Ctx.E[0U];
  AddTmp0[1U] = Crypto_xVIx_xAIx_SHA2Ctx.E[1U];
  Crypto_xVIx_xAIx_SHA384512CapitalSigma_1(AddTmp0);
  Sigm[0U] = AddTmp0[0U];
  Sigm[1U] = AddTmp0[1U];

  /* 2.3 round transformation */
  Ch[0U] = (uint32) (
                      (uint32) (
                                 (uint32) (Crypto_xVIx_xAIx_SHA2Ctx.F[0U] ^ Crypto_xVIx_xAIx_SHA2Ctx.G[0U]) &
                                 Crypto_xVIx_xAIx_SHA2Ctx.E[0U]
                               ) ^ Crypto_xVIx_xAIx_SHA2Ctx.G[0U]
                    );
  Ch[1U] = (uint32) (
                      (uint32) (
                                 (uint32) (Crypto_xVIx_xAIx_SHA2Ctx.F[1U] ^ Crypto_xVIx_xAIx_SHA2Ctx.G[1U]) &
                                 Crypto_xVIx_xAIx_SHA2Ctx.E[1U]
                               ) ^ Crypto_xVIx_xAIx_SHA2Ctx.G[1U]
                    );

  Crypto_xVIx_xAIx_Add(AddTmp1, Crypto_xVIx_xAIx_SHA2Ctx.H, Sigm);
  Crypto_xVIx_xAIx_Add(AddTmp2, AddTmp1, Ch);
  Crypto_xVIx_xAIx_Add(
                        AddTmp3,
                        &Crypto_xVIx_xAIx_SHA384512K[(uint8) (Crypto_xVIx_xAIx_SHA2Ctx.I << (uint8) 1U)],
                        &Crypto_xVIx_xAIx_SHA2Ctx.W[(uint8) (Crypto_xVIx_xAIx_SHA2Ctx.I << (uint8) 1U)]
                      );
  Crypto_xVIx_xAIx_Add(Crypto_xVIx_xAIx_SHA2Ctx.T1, AddTmp2, AddTmp3);

  /* Capital sigma 0 */
  AddTmp0[0U] = Crypto_xVIx_xAIx_SHA2Ctx.A[0U];
  AddTmp0[1U] = Crypto_xVIx_xAIx_SHA2Ctx.A[1U];
  Crypto_xVIx_xAIx_SHA384512CapitalSigma_0(AddTmp0);
  Sigm[0U] = AddTmp0[0U];
  Sigm[1U] = AddTmp0[1U];

  Maj[0U] = (uint32) (
                       (uint32) (
                                  (uint32) (Crypto_xVIx_xAIx_SHA2Ctx.A[0U] & Crypto_xVIx_xAIx_SHA2Ctx.B[0U]) |
                                  (uint32) (Crypto_xVIx_xAIx_SHA2Ctx.A[0U] & Crypto_xVIx_xAIx_SHA2Ctx.C[0U])
                                ) | (uint32) (Crypto_xVIx_xAIx_SHA2Ctx.B[0U] & Crypto_xVIx_xAIx_SHA2Ctx.C[0U])
                     );
  Maj[1U] = (uint32) (
                       (uint32) (
                                  (uint32) (Crypto_xVIx_xAIx_SHA2Ctx.A[1U] & Crypto_xVIx_xAIx_SHA2Ctx.B[1U]) |
                                  (uint32) (Crypto_xVIx_xAIx_SHA2Ctx.A[1U] & Crypto_xVIx_xAIx_SHA2Ctx.C[1U])
                                ) | (uint32)  (Crypto_xVIx_xAIx_SHA2Ctx.B[1U] & Crypto_xVIx_xAIx_SHA2Ctx.C[1U])
                     );

  Crypto_xVIx_xAIx_Add(Crypto_xVIx_xAIx_SHA2Ctx.T2, Maj, Sigm);

  Crypto_xVIx_xAIx_SHA2Ctx.SHACompressState = CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_TRANSFORM3;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA384512StateCompressTransform3
(
  void
)
{
  /* 2.4 permute the words for next round */
  Crypto_xVIx_xAIx_SHA2Ctx.H[0U] = Crypto_xVIx_xAIx_SHA2Ctx.G[0U];
  Crypto_xVIx_xAIx_SHA2Ctx.H[1U] = Crypto_xVIx_xAIx_SHA2Ctx.G[1U];

  Crypto_xVIx_xAIx_SHA2Ctx.G[0U] = Crypto_xVIx_xAIx_SHA2Ctx.F[0U];
  Crypto_xVIx_xAIx_SHA2Ctx.G[1U] = Crypto_xVIx_xAIx_SHA2Ctx.F[1U];

  Crypto_xVIx_xAIx_SHA2Ctx.F[0U] = Crypto_xVIx_xAIx_SHA2Ctx.E[0U];
  Crypto_xVIx_xAIx_SHA2Ctx.F[1U] = Crypto_xVIx_xAIx_SHA2Ctx.E[1U];

  Crypto_xVIx_xAIx_Add(Crypto_xVIx_xAIx_SHA2Ctx.E, Crypto_xVIx_xAIx_SHA2Ctx.D, Crypto_xVIx_xAIx_SHA2Ctx.T1);

  Crypto_xVIx_xAIx_SHA2Ctx.D[0U] = Crypto_xVIx_xAIx_SHA2Ctx.C[0U];
  Crypto_xVIx_xAIx_SHA2Ctx.D[1U] = Crypto_xVIx_xAIx_SHA2Ctx.C[1U];

  Crypto_xVIx_xAIx_SHA2Ctx.C[0U] = Crypto_xVIx_xAIx_SHA2Ctx.B[0U];
  Crypto_xVIx_xAIx_SHA2Ctx.C[1U] = Crypto_xVIx_xAIx_SHA2Ctx.B[1U];

  Crypto_xVIx_xAIx_SHA2Ctx.B[0U] = Crypto_xVIx_xAIx_SHA2Ctx.A[0U];
  Crypto_xVIx_xAIx_SHA2Ctx.B[1U] = Crypto_xVIx_xAIx_SHA2Ctx.A[1U];

  Crypto_xVIx_xAIx_Add(Crypto_xVIx_xAIx_SHA2Ctx.A, Crypto_xVIx_xAIx_SHA2Ctx.T1, Crypto_xVIx_xAIx_SHA2Ctx.T2);

  Crypto_xVIx_xAIx_SHA2Ctx.I++;

  if (Crypto_xVIx_xAIx_SHA2Ctx.I != (uint8) CRYPTO_XVIX_XAIX_SHA384512_ROUNDS)
  {
    Crypto_xVIx_xAIx_SHA2Ctx.SHACompressState = CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_EXPANSION;
  }
  else
  {
    Crypto_xVIx_xAIx_SHA2Ctx.SHACompressState = CRYPTO_XVIX_XAIX_SHA_S_COMPRESS_EXPORT;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA384512StateCompressExport
(
  void
)
{
  /* 3. copy internal state back to Ctx */
  Crypto_xVIx_xAIx_Add(
              &Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[0U],
              &Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[0U],
              Crypto_xVIx_xAIx_SHA2Ctx.A
            );
  Crypto_xVIx_xAIx_Add(
              &Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[2U],
              &Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[2U],
              Crypto_xVIx_xAIx_SHA2Ctx.B
            );
  Crypto_xVIx_xAIx_Add(
              &Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[4U],
              &Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[4U],
              Crypto_xVIx_xAIx_SHA2Ctx.C
            );
  Crypto_xVIx_xAIx_Add(
              &Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[6U],
              &Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[6U],
              Crypto_xVIx_xAIx_SHA2Ctx.D
            );
  Crypto_xVIx_xAIx_Add(
              &Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[8U],
              &Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[8U],
              Crypto_xVIx_xAIx_SHA2Ctx.E
            );
  Crypto_xVIx_xAIx_Add(
              &Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[10U],
              &Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[10U],
              Crypto_xVIx_xAIx_SHA2Ctx.F
            );
  Crypto_xVIx_xAIx_Add(
              &Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[12U],
              &Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[12U],
              Crypto_xVIx_xAIx_SHA2Ctx.G
            );
  Crypto_xVIx_xAIx_Add(
              &Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[14U],
              &Crypto_xVIx_xAIx_SHA2Ctx.Internalstate[14U],
              Crypto_xVIx_xAIx_SHA2Ctx.H
            );
}

#endif /* #if ((CRYPTO_XVIX_XAIX_SHA2_384_ENABLED == STD_ON) || \
               (CRYPTO_XVIX_XAIX_SHA2_512_ENABLED == STD_ON)) */

static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2Cancel
(
  void
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  /* This state indicates, that the Primitive is currently waiting for an Update/Finish call */
  if(CRYPTO_XVIX_XAIX_SHA_S_INITIALIZED == Crypto_xVIx_xAIx_SHA2Ctx.State)
  {
    /* !LINKSTO SWS_Crypto_00122_CORRECTION, 1 */
    Crypto_xVIx_xAIx_AL_HASH_SHA2_NOT_SET_Reset();
    /* !LINKSTO EB_Crypto_00063, 1 */
    RetVal = E_OK;
  }
  else
  {
    /* job can not be canceled imideatly */
    Crypto_xVIx_xAIx_SHA2Ctx.CancelFlag = TRUE;
    /* !LINKSTO SWS_Crypto_00143_CORRECTION_SWS_Crypto_00183, 1 */
    RetVal = CRYPTO_E_JOB_CANCELED;
  }
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_0965, 1 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2Start
(
  P2CONST(void, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CfgPtr
)
{
  /* !LINKSTO SWS_Crypto_00017, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Start, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Start2Start, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Update2Start, 1 */
  Crypto_xVIx_xAIx_AL_HASH_SHA2_NOT_SET_Reset();
  Crypto_xVIx_xAIx_SHA2Ctx.CfgPtr = (P2CONST(Crypto_xVIx_xAIx_SHAConfigType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA))CfgPtr;
  Crypto_xVIx_xAIx_SHA2Ctx.Crypto_Generic_Callback = Crypto_xVIx_xAIx_SHA2Ctx.CfgPtr->Crypto_Generic_Callback;
  Crypto_xVIx_xAIx_SHA2Ctx.State = CRYPTO_XVIX_XAIX_SHA_S_INIT;
  Crypto_xVIx_xAIx_SHA2Ctx.CurrentMode = CRYPTO_OPERATIONMODE_START;
  Crypto_xVIx_xAIx_SHA2Ctx.Job = Crypto_xVIx_xAIx_SHA2Ctx.CfgPtr->Job;
  Crypto_xVIx_xAIx_SHA2Ctx.ObjId = Crypto_xVIx_xAIx_SHA2Ctx.CfgPtr->ObjId;

  Crypto_xVIx_xAIx_SHA2Ctx.ErrorCode = E_OK;
}

static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2Update
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) DataPtr,
  uint32 DataLength
)
{
  Std_ReturnType RetVal;

  if(CRYPTO_XVIX_XAIX_SHA_S_INITIALIZED == Crypto_xVIx_xAIx_SHA2Ctx.State)
  {
    /* Store data into context */
    Crypto_xVIx_xAIx_SHA2Ctx.Data = DataPtr;
    Crypto_xVIx_xAIx_SHA2Ctx.DataLength = DataLength;

    /* Set state of the state machine to update */
    Crypto_xVIx_xAIx_SHA2Ctx.State = CRYPTO_XVIX_XAIX_SHA_S_UPDATE;
    Crypto_xVIx_xAIx_SHA2Ctx.ProcessInputState = CRYPTO_XVIX_XAIX_SHA_S_PROCESS;
    Crypto_xVIx_xAIx_SHA2Ctx.ErrorCode = E_OK;
    RetVal = E_OK;
  }
  else
  {
    RetVal = CRYPTO_E_BUSY;
  }
  return RetVal;
}

static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2Finish
(
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  ResultErrorPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultErrorLengthPtr,
  boolean                                              TruncationAllowed
)
{
  Std_ReturnType RetVal;

  if(CRYPTO_XVIX_XAIX_SHA_S_INITIALIZED == Crypto_xVIx_xAIx_SHA2Ctx.State)
  {
    /* Store data into context */
    Crypto_xVIx_xAIx_SHA2Ctx.Result = ResultErrorPtr;
    Crypto_xVIx_xAIx_SHA2Ctx.ResultLengthPtr = ResultErrorLengthPtr;
    Crypto_xVIx_xAIx_SHA2Ctx.TruncationAllowed = TruncationAllowed;
    /* Set state of the state machine to finish */
    Crypto_xVIx_xAIx_SHA2Ctx.State = CRYPTO_XVIX_XAIX_SHA_S_FINISH;
    Crypto_xVIx_xAIx_SHA2Ctx.ProcessInputState = CRYPTO_XVIX_XAIX_SHA_S_PROCESS;
    Crypto_xVIx_xAIx_SHA2Ctx.ErrorCode = E_OK;
    RetVal = E_OK;
  }
  else
  {
    RetVal = CRYPTO_E_BUSY;
  }
  return RetVal;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_SHA2MainFunction
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultError,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
)
{
  if(
      !Crypto_xVIx_xAIx_CheckSkipMain(ResultError, Busy, Crypto_xVIx_xAIx_SHA2Ctx.ObjId) &&
      (NULL_PTR != Crypto_xVIx_xAIx_SHA2Ctx.Job)
    )
  {
    if
    (
      (
        CRYPTO_ALGOFAM_SHA2_224 ==
        Crypto_xVIx_xAIx_SHA2Ctx.Job->jobPrimitiveInfo->primitiveInfo->algorithm.family
      ) ||
      (
        CRYPTO_ALGOFAM_SHA2_224 ==
        Crypto_xVIx_xAIx_SHA2Ctx.Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily
      ) ||
      (
        CRYPTO_ALGOFAM_SHA2_256 ==
        Crypto_xVIx_xAIx_SHA2Ctx.Job->jobPrimitiveInfo->primitiveInfo->algorithm.family
      ) ||
      (
        CRYPTO_ALGOFAM_SHA2_256 ==
        Crypto_xVIx_xAIx_SHA2Ctx.Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily
      )
    )
    {
#if ((CRYPTO_XVIX_XAIX_SHA2_224_ENABLED == STD_ON) || (CRYPTO_XVIX_XAIX_SHA2_256_ENABLED == STD_ON))
      Crypto_xVIx_xAIx_SHAMainFunction(
                                        &Crypto_xVIx_xAIx_SHA2Ctx,
                                        &Crypto_xVIx_xAIx_SHA2Init,
                                        &Crypto_xVIx_xAIx_SHA224256Compress,
                                        ResultError,
                                        Busy
                                      );
#endif
    }
    else
    {
      /* defensive programming: only these HASH primitives are configurable and there are no
         possibilities to configure more (as in the configuration, the primitives list is not
         editable) so the else will be not reached. */
      /* CHECK: NOPARSE */
      if
      (
        (
          CRYPTO_ALGOFAM_SHA2_384 ==
          Crypto_xVIx_xAIx_SHA2Ctx.Job->jobPrimitiveInfo->primitiveInfo->algorithm.family
        ) ||
        (
          CRYPTO_ALGOFAM_SHA2_384 ==
          Crypto_xVIx_xAIx_SHA2Ctx.Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily
        ) ||
        (
          CRYPTO_ALGOFAM_SHA2_512 ==
          Crypto_xVIx_xAIx_SHA2Ctx.Job->jobPrimitiveInfo->primitiveInfo->algorithm.family
        ) ||
        (
          CRYPTO_ALGOFAM_SHA2_512 ==
          Crypto_xVIx_xAIx_SHA2Ctx.Job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily
        )
      )
      {
#if ((CRYPTO_XVIX_XAIX_SHA2_384_ENABLED == STD_ON) || (CRYPTO_XVIX_XAIX_SHA2_512_ENABLED == STD_ON))
        Crypto_xVIx_xAIx_SHAMainFunction(
                                          &Crypto_xVIx_xAIx_SHA2Ctx,
                                          &Crypto_xVIx_xAIx_SHA2Init,
                                          &Crypto_xVIx_xAIx_SHA384512Compress,
                                          ResultError,
                                          Busy
                                        );
#endif
      }
      /* CHECK: PARSE */
    }
  }
}

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#else /* #if ((CRYPTO_XVIX_XAIX_SHA1_ENABLED == STD_ON) || (CRYPTO_XVIX_XAIX_SHA2_ENABLED == STD_ON)) */
typedef uint8 Crypto_xVIx_xAIx_SHA_PrvtEmptyTranslationUnitType;
#endif /* #if ((CRYPTO_XVIX_XAIX_SHA1_ENABLED == STD_ON) || (CRYPTO_XVIX_XAIX_SHA2_ENABLED == STD_ON)) #else */

/*==[end of file]=================================================================================*/

