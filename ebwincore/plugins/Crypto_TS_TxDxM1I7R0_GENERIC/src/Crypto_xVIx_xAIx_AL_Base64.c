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

/*==[Includes]================================================================*/

#include <Std_Types.h>

#include <Crypto_xVIx_xAIx_AL_Base64.h>

/*==[Macros]==================================================================*/

#if (defined CRYPTO_XVIX_XAIX_BASE64_ENCODING_BLOCK_LEN)
#error CRYPTO_XVIX_XAIX_BASE64_ENCODING_BLOCK_LEN is already defined
#endif
#define CRYPTO_XVIX_XAIX_BASE64_ENCODING_BLOCK_LEN 4U

#if (defined CRYPTO_XVIX_XAIX_BASE64_DECODED_BLOCK_LEN)
#error CRYPTO_XVIX_XAIX_BASE64_DECODED_BLOCK_LEN is already defined
#endif
#define CRYPTO_XVIX_XAIX_BASE64_DECODED_BLOCK_LEN  3U

/*==[Types]===================================================================*/

/*==[Declaration of functions with internal linkage]==========================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/**  \brief  Decode one block of characters
 **
 ** This function decodes one block of Base64 encoded characters.
 **
 ** \param[in]    EncodingBlock        The block to be decoded
 ** \param[in]    EncodingLength       The length of the block to be decoded (in bytes)
 **
 ** \param[out]      ResultBlock       The decoded block
 ** \param[in]       ResultBlockLength The length of the decoded block (in bytes)
 **
 ** \returns  Error code
 **
 ** \retval   CSM_E_OK      Decoding was successful
 ** \retval   CSM_E_NOT_OK  The input did not conform to the expected format
 **/
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_Base64DecodeOneBlock
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) EncodingBlock,
  uint32 EncodingBlockLength,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultBlock,
  uint32 ResultBlockLength
);

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Constants with internal linkage]=========================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CONST_8
#include <Crypto_xVIx_xAIx_MemMap.h>

static CONST(uint8, CRYPTO_xVIx_xAIx_CONST) Crypto_xVIx_xAIx_Base64Mapping[] =
{
  0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, /* 0x00 */
  0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, /* 0x08 */
  0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, /* 0x10 */
  0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, /* 0x18 */
  0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, /* 0x20 */
  0xFFU, 0xFFU, 0xFFU,   62U, 0xFFU, 0xFFU, 0xFFU,   63U, /* 0x28 */
    52U,   53U,   54U,   55U,   56U,   57U,   58U,   59U, /* 0x30 */
    60U,   61U, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, /* 0x38 */
  0xFFU,    0U,    1U,    2U,    3U,    4U,    5U,    6U, /* 0x40 */
     7U,    8U,    9U,   10U,   11U,   12U,   13U,   14U, /* 0x48 */
    15U,   16U,   17U,   18U,   19U,   20U,   21U,   22U, /* 0x50 */
    23U,   24U,   25U, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, /* 0x58 */
  0xFFU,   26U,   27U,   28U,   29U,   30U,   31U,   32U, /* 0x60 */
    33U,   34U,   35U,   36U,   37U,   38U,   39U,   40U, /* 0x68 */
    41U,   42U,   43U,   44U,   45U,   46U,   47U,   48U, /* 0x70 */
    49U,   50U,   51U, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, /* 0x78 */
  0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, /* 0x80 */
  0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, /* 0x88 */
  0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, /* 0x90 */
  0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, /* 0x98 */
  0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, /* 0xA0 */
  0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, /* 0xA8 */
  0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, /* 0xB0 */
  0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, /* 0xB8 */
  0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, /* 0xC0 */
  0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, /* 0xC8 */
  0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, /* 0xD0 */
  0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, /* 0xD8 */
  0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, /* 0xE0 */
  0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, /* 0xE8 */
  0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, /* 0xF0 */
  0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU  /* 0xF8 */
};

#define CRYPTO_xVIx_xAIx_STOP_SEC_CONST_8
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Variables with internal linkage]=========================================*/

/*==[Constants with external linkage]=========================================*/

/*==[Variables with external linkage]=========================================*/

/*==[Definition of functions with external linkage]===========================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_Base64Decode
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding,
  uint32 EncodingLength,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultLengthPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  uint32 EncodingPosition;
  uint32 ResultPosition;
  uint32 EncodingBlockLength;
  uint32 EncodingBlockRealLength;
  uint32 ResultBlockLength;
  uint32 ResultBufferLength;
  uint32 TotalPaddingChars;
  uint32 PaddingCharsAtEnd;
  boolean Finished;

  EncodingPosition = EncodingLength;
  EncodingLength = 0U;

  Finished = FALSE;
  while ((0U < EncodingPosition) && (FALSE == Finished))
  {
    EncodingPosition--;
    if (
         (Encoding[EncodingPosition] != '\n') &&
         (Encoding[EncodingPosition] != '\r')
       )
    {
      Finished = TRUE;

      EncodingLength = EncodingPosition + 1U;
    }
  }

  PaddingCharsAtEnd = 0U;
  Finished = FALSE;
  EncodingPosition = EncodingLength;
  while ((0U < EncodingPosition) && (FALSE == Finished))
  {
    EncodingPosition--;

    if (Encoding[EncodingPosition] == '=')
    {
      PaddingCharsAtEnd++;
    }
    else
    {
      if (
           (Encoding[EncodingPosition] != '\n') &&
           (Encoding[EncodingPosition] != '\r')
         )
      {
        Finished = TRUE;
      }
    }
  }

  RetVal = E_OK;
  EncodingPosition = 0U;
  ResultPosition = 0U;
  TotalPaddingChars = 0U;
  ResultBufferLength = *ResultLengthPtr;
  ResultBlockLength = 0U;

  EncodingBlockRealLength = 0U;
  while
  (
    (EncodingPosition < EncodingLength) &&
    (E_OK == RetVal)
  )
  {
    EncodingBlockLength = 0U;
    EncodingBlockRealLength = 0U;

    while
    (
      ((EncodingBlockLength + EncodingPosition) < EncodingLength) &&
      (CRYPTO_XVIX_XAIX_BASE64_ENCODING_BLOCK_LEN > EncodingBlockRealLength)
    )
    {
      if
      (
        (Encoding[EncodingPosition + EncodingBlockLength] != '\r') &&
        (Encoding[EncodingPosition + EncodingBlockLength] != '\n') &&
        (Encoding[EncodingPosition + EncodingBlockLength] != '=')
      )
      {
        EncodingBlockRealLength++;
      }

      if (Encoding[EncodingPosition + EncodingBlockLength] == '=')
      {
        TotalPaddingChars++;
      }

      EncodingBlockLength++;
    }

    if((EncodingBlockRealLength < 2U) || (TotalPaddingChars > PaddingCharsAtEnd))
    {
       RetVal = E_NOT_OK;
    }
    else
    {
      ResultBlockLength = EncodingBlockRealLength - 1U;
    }

    if(E_OK == RetVal)
    {
      if ((ResultPosition + ResultBlockLength) <= ResultBufferLength)
      {
        RetVal = Crypto_xVIx_xAIx_Base64DecodeOneBlock
                 (
                   &Encoding[EncodingPosition],
                   EncodingBlockLength,
                   &Result[ResultPosition],
                   ResultBlockLength
                 );

        EncodingPosition += EncodingBlockLength;
        ResultPosition += ResultBlockLength;
      }
      else
      {
        RetVal = E_NOT_OK;
      }
    }
  }

  if ((E_OK == RetVal) && (0U < PaddingCharsAtEnd) &&
      ((CRYPTO_XVIX_XAIX_BASE64_ENCODING_BLOCK_LEN != (PaddingCharsAtEnd + EncodingBlockRealLength)) ||
       (2U < PaddingCharsAtEnd)
      )
     )
  {
    RetVal = E_NOT_OK;
  }

  *ResultLengthPtr = ResultPosition;

  return RetVal;
}

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Definition of functions with internal linkage]===========================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_Base64DecodeOneBlock
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) EncodingBlock,
  uint32 EncodingBlockLength,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultBlock,
  uint32 ResultBlockLength
)
{
  uint32 DecodedBytes = 0U;
  Std_ReturnType RetVal = E_OK;
  uint32 i = 0U;
  uint32 j = 0U;
  uint8 SixDecodedBits = 0U;

  while(i < EncodingBlockLength)
  {
    while
    (
      (i < EncodingBlockLength) &&
      (
        ('\r' == EncodingBlock[i]) ||
        ('\n' == EncodingBlock[i]) ||
        ('='  == EncodingBlock[i])
      )
    )
    {
      i++;
    }

    if(i < EncodingBlockLength)
    {
      SixDecodedBits = Crypto_xVIx_xAIx_Base64Mapping[EncodingBlock[i]];

      if(0xFFU == SixDecodedBits)
      {
        RetVal = E_NOT_OK;
      }
      else
      {
        DecodedBytes |=
            ((uint32)SixDecodedBits << (6U * (CRYPTO_XVIX_XAIX_BASE64_ENCODING_BLOCK_LEN - (j + 1U))));
      }

      i++;
      j++;
    }
  }

  if(E_OK == RetVal)
  {
    for(i = 0U; i < ResultBlockLength; i++)
    {
      ResultBlock[i] = (uint8) (DecodedBytes >> (8U * (CRYPTO_XVIX_XAIX_BASE64_DECODED_BLOCK_LEN- (i + 1U))));
    }
  }

  return RetVal;
}

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

