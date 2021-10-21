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

#ifndef CRYPTO_XVIX_XAIX_AL_BASE64_H
#define CRYPTO_XVIX_XAIX_AL_BASE64_H

/*==[Includes]================================================================*/

#include <Std_Types.h>

/*==[Declaration of functions with external linkage]==========================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/**  \brief  Decode a Base64 encoded string
 **
 ** This function decodes a Base64 encoded string. The following holds:
 ** 1) The newline characters '\n' and '\r' are ignored in the input string.
 ** 2) If a padding is used, the padding must use the correct number of padding
 **    characters at the end of the input string.
 ** 3) If other characters than '\n' and '\r' occur after the last padding
 **    character, this is treated as an error.
 ** 4) If the output buffer is too small to accomodate the result,
 **    CSM_E_SMALL_BUFFER is returned.
 ** 5) If the last block of encoded characters only contains a single
 **    byte, this is considered an error (since the last decoded byte is
 **    missing two bits)
 **
 ** \param[in]    Encoding           The string to be decoded
 ** \param[in]    EncodingLength     The length of the encoded string
 **
 ** \param[out]      Result          The result of the decoding
 ** \param[in,out]   ResultLengthPtr Must contain the length of the result buffer
 **                                  (on calling). Shall contain the length of the
 **                                  decoded string on returning.
 **
 ** \returns  Error code
 **
 ** \retval   CSM_E_OK      Decoding was successful
 ** \retval   CSM_E_NOT_OK  The input did not conform to the expected format
 ** \retval   CSM_E_SMALL_BUFFER The output buffer was too small to accommodate
 **                              the result
 **/

extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_Base64Decode
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding,
  uint32                                                EncodingLength,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Result,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  ResultLengthPtr
);

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #ifndef CRYPTO_XVIX_XAIX_AL_BASE64_H */

