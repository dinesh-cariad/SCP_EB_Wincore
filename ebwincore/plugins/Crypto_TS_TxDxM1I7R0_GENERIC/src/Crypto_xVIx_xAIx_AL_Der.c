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
 * Misra-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 8.9 (advisory)
 * An object should be defined at block scope if its identifier only appears in a single function.
 *
 * Reason:
 * The object is actually used in multiple functions which are only enabled depending on the
 * module configuration. Additionaly, the AUTOSAR memory mapping requires that functions are
 * mapped to SEC_CODE memory sections. Objects at block scope require a different mapping,
 * e.g. to a SEC_VAR section, which leads to nested memory sections, which is not supported
 * by some compilers. Stack usage would also increase in defined in the block scope.
 *
 */

/*==[Includes]====================================================================================*/

#include <Std_Types.h>
#include <TSAutosar.h>
#include <Crypto_xVIx_xAIx_AL_Der.h>
#include <Crypto_xVIx_xAIx_AL_Common.h>

/*==[Macros]======================================================================================*/

#if (defined CRYPTO_XVIX_XAIX_DER_ENC_LEN_THRESHOLD)
#error CRYPTO_XVIX_XAIX_DER_ENC_LEN_THRESHOLD is already defined
#endif
/** \brief The threshold below which a length encoding is one byte long
 *
 * The length of the encoding of a value is encoded in "length octets". The
 * first of these octets contains either the length of the encoding of the
 * value if it is smaller than this threshold macro, or it contains the length
 * of the encoding of the value length. See X.690, chapter 8.1.3.
 */
#define CRYPTO_XVIX_XAIX_DER_ENC_LEN_THRESHOLD (uint8) 0x7FU

#if (defined CRYPTO_XVIX_XAIX_DER_SUBTRACT_NO_OVERFLOW)
#error CRYPTO_XVIX_XAIX_DER_SUBTRACT_NO_OVERFLOW is already defined
#endif
/** \brief Subtract two values; do not overflow at zero
 *
 * Subtract one value from another but return 0 if the subtrahend is larger than
 * the minuend.
 *
 * \param[in] Minuend The number from which to subtract
 * \param[in] Subtrahend The number which to subtract
 *
 * \return The difference if Minuend >= Subtrahend. 0, otherwise.
 */
#define CRYPTO_XVIX_XAIX_DER_SUBTRACT_NO_OVERFLOW(Minuend, Subtrahend) \
              (((Minuend) >= (Subtrahend)) ? ((Minuend) - (Subtrahend)) : (uint32)0U)

#if (defined CRYPTO_XVIX_XAIX_DER_DATELENGTH)
#error CRYPTO_XVIX_XAIX_DER_DATELENGTH is already defined
#endif
/** \brief Length of the date (YYMMDD)
 *
 */
#define CRYPTO_XVIX_XAIX_DER_DATELENGTH 6U

/*==[Types]=======================================================================================*/

/*==[Declaration of functions with internal linkage]==============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief Checks the identifier of an encoding
 *
 * Given a pointer to the start of a DER encoding of a value and a description
 * of a subcomponent, this function checks whether the tag of the type of the
 * value is identical to the one given in the subcomponent description.
 * See X.690, chapter 8.1.2.
 *
 * \param[in] EncodingPtr pointer to the start of the encoding of the value
 * \param[in] SubComponentPtr pointer to the description of a subcomponent
 *
 * \return Is the identifier correct?
 *
 * \retval ::TRUE If the identifier is correct
 * \retval ::FALSE If the identifier is not correct
 */
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerCheckIdentifier
(
  P2CONST(uint8,                                 AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) EncodingPtr,
  P2CONST(Crypto_xVIx_xAIx_DerSubComponentType,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) SubComponentPtr
);

/** \brief Skips the identifier of an encoding
 *
 * Given a pointer to the start of a DER encoding of a value, this macro returns
 * how many bytes in the encoding have to be skipped to get past the identifier.
 * See X.690, chapter 8.1.2.
 *
 * \param[in] EncodingPtr pointer to the start of the encoding of the value
 *
 * \return The length of the identifier octets which can from 1 byte to max 4 byte.
 *         If identifier bigger than 4, return 0.
 */
static FUNC(uint32, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerSkipIdentifier
(
    P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) EncodingPtr
);

/** \brief Searches a given subcomponent in a DER encoded "sequence".
 *
 * Given a pointer to the start of the DER encoding of an ASN1 sequence, a
 * description of a subcomponent, and a search start indicator, this function
 * searches the sequence for the first occurrence of the described subcomponent
 * following the search start indicator and, if found, returns its position in
 * the encoding.
 *
 * \param[in]  ComponentPtr A pointer to the description of the sequence which
 *                          has to be searched
 * \param[in]  Encoding A pointer to the start of the encoding of the sequence
 * \param[in]  SubComponentToFindPtr A pointer to the description of the
 *                                subcomponent which has to be found
 * \param[out] PositionPtr A pointer to a variable where the following will
 *                         be stored if the search was successful: the byte
 *                         position of the encoding of the first occurrence
 *                         of the given subcomponent which fulfills the
 *                         requirement "position >= StartSearch"
 * \param[in]  StartSearch Subcomponents which start before this position in
 *                         the encoding are ignored in the search.
 *
 * \return The success value of the search
 *
 * \retval ::TRUE If the subcomponent has been found
 * \retval ::FALSE If the subcomponent has not been found
 */
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerFindInSequence
(
  P2CONST(Crypto_xVIx_xAIx_DerComponentType,    AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ComponentPtr,
  P2CONST(uint8,                                AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding,
  P2CONST(Crypto_xVIx_xAIx_DerSubComponentType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) SubComponentToFindPtr,
    P2VAR(uint32,                               AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PositionPtr,
          uint32                                             StartSearch
);

/** \brief Searches a given subcomponent in a DER encoded "sequence" ignoring the tag of the given
 * component.
 *
 * Given a pointer to the start of the DER encoding of an ASN1 sequence, a
 * description of a subcomponent, and a search start indicator, this function
 * searches the sequence for the first occurrence of the described subcomponent
 * following the search start indicator and, if found, returns its position in
 * the encoding.
 *
 * \param[in]  ComponentPtr A pointer to the description of the sequence which
 *                          has to be searched
 * \param[in]  Encoding A pointer to the start of the encoding of the sequence
 * \param[in]  SubComponentToFindPtr A pointer to the description of the
 *                                subcomponent which has to be found
 * \param[out] PositionPtr A pointer to a variable where the following will
 *                         be stored if the search was successful: the byte
 *                         position of the encoding of the first occurrence
 *                         of the given subcomponent which fulfills the
 *                         requirement "position >= StartSearch"
 * \param[in]  StartSearch Subcomponents which start before this position in
 *                         the encoding are ignored in the search.
 *
 * \return The success value of the search
 *
 * \retval ::TRUE If the subcomponent has been found
 * \retval ::FALSE If the subcomponent has not been found
 */
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerFindInSequenceIgnoreTag
(
  P2CONST(Crypto_xVIx_xAIx_DerComponentType,    AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ComponentPtr,
  P2CONST(uint8,                                AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding,
  P2CONST(Crypto_xVIx_xAIx_DerSubComponentType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) SubComponentToFindPtr,
    P2VAR(uint32,                               AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PositionPtr,
          uint32                                             StartSearch
);

/** \brief Finds nothing in a DER encoded component.
 *
 * This function is used for searching a subcomponent in a DER component which
 * has a primitive encoding. Since primitive encodings, by definition, do not
 * contain subcomponents, this function always returns "not found".
 *
 * \param[in]  ComponentPtr Parameter is ignored
 * \param[in]  Encoding Parameter is ignored
 * \param[in]  SubComponentToFindPtr Parameter is ignored
 * \param[out] PositionPtr A pointer to a variable where "0" will be stored
 * \param[in]  StartSearch Parameter is ignored
 *
 * \return The success value of the search
 *
 * \retval  FALSE always
 */
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerFindNothing
(
  P2CONST(Crypto_xVIx_xAIx_DerComponentType,    AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ComponentPtr,
  P2CONST(uint8,                                AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding,
  P2CONST(Crypto_xVIx_xAIx_DerSubComponentType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) SubComponentToFindPtr,
    P2VAR(uint32,                               AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PositionPtr,
          uint32                                                                       StartSearch
);

/** \brief Checks the validity of a DER encoded subcomponent.
 *
 * This function checks whether the subcomponent has the correct tag and whether
 * it is valid itself.
 *
 * \param[in]  SubComponentPtr A pointer to the description of the subcomponent
 *                             which has to be checked
 * \param[in]  Encoding A pointer to the start of the encoding of the subcomponent
 * \param[in]  MaxLength The maximal allowed length of the encoding of the
 *                       subcomponent.
 * \param[out] LengthPtr A pointer to a variable where the length of the
 *                       encoding of the subcomponent will be stored if the
 *                       encoding is valid
 *
 * \return The validity of the encoding
 *
 * \retval ::TRUE If the encoding is valid
 * \retval ::FALSE If the encoding is invalid
 */
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerCheckSubComponent
(
  P2CONST(Crypto_xVIx_xAIx_DerSubComponentType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) SubComponentPtr,
  P2CONST(uint8,                                AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding,
          uint32                                                                       MaxLength,
    P2VAR(uint32,                               AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) LengthPtr
);

/** \brief Checks the validity of a DER encoded "octetstring".
 *
 * Given a pointer to the start of a byte array, and a description of an ASN1
 * component of type octetstring, this function checks whether the given byte
 * array is a valid encoding of the octetstring. The type of the encoding is
 * checked and it is checked whether the encoding is of the correct format.
 *
 * \param[in]  ComponentPtr A pointer to the description of the octetstring which
 *                          has to be checked (ignored)
 * \param[in]  Encoding A pointer to the start of the encoding of the octetstring
 * \param[in]  MaxLength The maximal allowed length of the encoding of the
 *                       octetstring.
 * \param[out] LengthPtr A pointer to a variable where the length of the
 *                       encoding of the value will be stored if the
 *                       encoding is valid
 *
 * \return The validity of the encoding
 *
 * \retval ::TRUE If the encoding is valid
 * \retval ::FALSE If the encoding is invalid
 */
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerCheckOctetstring
(
  P2CONST(Crypto_xVIx_xAIx_DerComponentType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ComponentPtr,
  P2CONST(uint8,                             AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding,
  uint32                                                                            MaxLength,
  P2VAR(uint32,                              AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) LengthPtr
);

/** \brief Checks the validity of a DER encoded "object identifier".
 *
 * Given a pointer to the start of a byte array, and a description of an ASN1
 * component of type object identifier, this function checks whether the given byte
 * array is a valid encoding of the object identifier. The type of the encoding is
 * checked and it is checked whether the encoding is of the correct format.
 *
 * \param[in]  ComponentPtr A pointer to the description of the object identifier which
 *                          has to be checked (ignored)
 * \param[in]  Encoding A pointer to the start of the encoding of the object identifier
 * \param[in]  MaxLength The maximal allowed length of the encoding of the
 *                       object identifier.
 * \param[out] LengthPtr A pointer to a variable where the length of the
 *                       encoding of the value will be stored if the
 *                       encoding is valid
 *
 * \return The validity of the encoding
 *
 * \retval ::TRUE If the encoding is valid
 * \retval ::FALSE If the encoding is invalid
 */
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerCheckObjectIdentifier
(
  P2CONST(Crypto_xVIx_xAIx_DerComponentType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ComponentPtr,
  P2CONST(uint8,                             AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding,
          uint32                                                                    MaxLength,
  P2VAR(uint32,                              AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) LengthPtr
);

/** \brief Checks the validity of a DER encoded "CharacterString".
 *
 * Given a pointer to the start of a byte array, and a description of an ASN1
 * component of type CharacterString, this function checks whether the given
 * byte array is a valid encoding of the CharacterString. The type of the
 * encoding is checked.
 *
 * \param[in]  ComponentPtr A pointer to the description of the CharacterString
 *                          which has to be checked (ignored)
 * \param[in]  Encoding A pointer to the start of the encoding of the
 *                      CharacterString
 * \param[in]  MaxLength The maximal allowed length of the encoding of the
 *                       CharacterString.
 * \param[out] LengthPtr A pointer to a variable where the length of the
 *                       encoding of the value will be stored if the
 *                       encoding is valid
 *
 * \return The validity of the encoding
 *
 * \retval ::TRUE If the encoding is valid
 * \retval ::FALSE If the encoding is invalid
 */
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerCheckCharacterString
(
  P2CONST(Crypto_xVIx_xAIx_DerComponentType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ComponentPtr,
  P2CONST(uint8,                             AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding,
          uint32                                                                    MaxLength,
    P2VAR(uint32,                            AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) LengthPtr
);

/** \brief Checks the validity of a DER encoded "sequence".
 *
 * Given a pointer to the start of a byte array, and a description of an ASN1
 * component of type sequence, this function checks whether the given byte
 * array is a valid encoding of the sequence. The type of the encoding is
 * checked and all subcomponents of the sequence are themselves checked.
 *
 * \param[in]  ComponentPtr A pointer to the description of the sequence which
 *                          has to be checked
 * \param[in]  Encoding A pointer to the start of the encoding of the sequence
 * \param[in]  MaxLength The maximal allowed length of the encoding of the
 *                       sequence.
 * \param[out] LengthPtr A pointer to a variable where the length of the
 *                       encoding of the value will be stored if the
 *                       encoding is valid
 *
 * \return The validity of the encoding
 *
 * \retval ::TRUE If the encoding is valid
 * \retval ::FALSE If the encoding is invalid
 */
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerCheckSequence
(
  P2CONST(Crypto_xVIx_xAIx_DerComponentType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ComponentPtr,
  P2CONST(uint8,                             AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding,
          uint32                                                                    MaxLength,
    P2VAR(uint32,                            AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) LengthPtr
);

/** \brief Checks the validity of the public key.
 *
 * Given a pointer to the start of a byte array, and a description of an ASN1 component of type
 * public key, this function checks whether the given byte array is a valid encoding of the public
 * key. The type of the encoding is checked and all subcomponents of the sequence are themselves
 * checked.
 *
 * \param[in]  ComponentPtr A pointer to the description of the public key which
 *                          has to be checked
 * \param[in]  Encoding     A pointer to the start of the encoding of the public key
 * \param[in]  MaxLength    The maximal allowed length of the encoding of the
 *                          public key
 * \param[out] LengthPtr    A pointer to a variable where the length of the
 *                          encoding of the value will be stored if the
 *                          encoding is valid
 *
 * \return The validity of the encoding
 *
 * \retval ::TRUE If the encoding is valid
 * \retval ::FALSE If the encoding is invalid
 */
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerCheckPublicKey
(
  P2CONST(Crypto_xVIx_xAIx_DerComponentType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ComponentPtr,
  P2CONST(uint8,                AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding,
          uint32                                          MaxLength,
    P2VAR(uint32,               AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) LengthPtr
);

/** \brief Checks the validity of a DER encoded "integer" or "enumerated value".
 *
 * Given a pointer to the start of a byte array, and a description of an ASN1
 * component of type "enumerated value" or "integer", this function checks
 * whether the given byte array is a valid encoding of an integer. If the
 * component description contains range restrictions, the value is checked
 * against the allowed ranges by calling Crypto_xVIx_xAIx_AL_DerCheckRanges().
 *
 * Note that our code only allows non-negative range restrictions. So, if the
 * encoded value is negative and the component description contains range
 * restrictions, this check will fail.
 *
 * \param[in]  ComponentPtr A pointer to the description of the value which has
 *                          to be checked
 * \param[in]  Encoding A pointer to the start of the encoding of the value
 * \param[in]  MaxLength The maximal allowed length of the encoding of the
 *                       value
 * \param[out] LengthPtr A pointer to a variable where the length of the
 *                       encoding of the value will be stored if the
 *                       encoding is valid
 *
 * \return The validity of the encoding
 *
 * \retval ::TRUE If the encoding is valid
 * \retval ::FALSE If the encoding is invalid
 */
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerCheckInteger
(
  P2CONST(Crypto_xVIx_xAIx_DerComponentType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ComponentPtr,
  P2CONST(uint8,                             AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding,
          uint32                                                                    MaxLength,
    P2VAR(uint32,                            AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) LengthPtr
);

/** \brief Checks the validity of a DER date encoded in BCD.
 *
 * Given a pointer to the start of a byte array, and a description of an ASN1
 * component of type BCD, this function exteaxts the date and checks if it is a
 * valid date
 *
 * \param[in]  ComponentPtr A pointer to the description of the value which has
 *                          to be checked
 * \param[in]  Encoding A pointer to the start of the encoding of the value
 * \param[in]  MaxLength The maximal allowed length of the encoding of the
 *                       value
 * \param[out] LengthPtr A pointer to a variable where the length of the
 *                       encoding of the value will be stored if the
 *                       encoding is valid
 *
 * \return The validity of the encoding
 *
 * \retval ::TRUE If the encoding is valid
 * \retval ::FALSE If the encoding is invalid
 */
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerCheckDate
(
  P2CONST(Crypto_xVIx_xAIx_DerComponentType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ComponentPtr,
  P2CONST(uint8,                             AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding,
          uint32                                                                    MaxLength,
    P2VAR(uint32,                            AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) LengthPtr
);

/** \brief Checks whether a given value belongs to a given set of ranges.
 *
 * Given a value and a set of ranges, this function checks whether the value
 * is contained in at least one of the given ranges.
 *
 * \param[in]  Value The value to check
 * \param[in]  Overflow If this is true, the value is too large to fit into
 *                      the given argument
 * \param[in]  NrOfRanges The number of ranges to check against
 * \param[in]  Ranges A pointer to the start of an array which contains
 *                    the ranges to check against.
 *
 * \retval ::TRUE If the value is contained in at least one of the ranges
 * \retval ::FALSE If the value is not contained in any of the ranges
 */
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerCheckRanges
(
          uint32                                      Value,
          boolean                                     Overflow,
          uint32                                      NrOfRanges,
  P2CONST(Crypto_xVIx_xAIx_DerRangeType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Ranges
);

/** \brief Checks whether a given date belongs to a valid date.
 *
 * \param[in]  Year  in YY format GMT
 * \param[in]  Month in MM format GMT
 * \param[in]  Day   in DD format GMT
 *
 * \retval ::TRUE  If the date valid
 * \retval ::FALSE If the date not valid
 */
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerCheckValidDate
(
  uint8 Year,
  uint8 Month,
  uint8 Day
);

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Constants with internal linkage]=============================================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CONST_8
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief A constant which maps a month to the number of days. */
/* Deviation MISRAC2012-1 */
static CONST(uint8, CRYPTO_xVIx_xAIx_CONST) Crypto_xVIx_xAIx_DaysPerMonth[12U] =
{ 31U, 28U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U };

#define CRYPTO_xVIx_xAIx_STOP_SEC_CONST_8
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Variables with internal linkage]=============================================================*/

/*==[Constants with external linkage]=============================================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CONST_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief Functions used to check validity and parse a component within a sequence */
CONST(Crypto_xVIx_xAIx_DerParseFunctionsType, CRYPTO_xVIx_xAIx_CONST)
  Crypto_xVIx_xAIx_DerParseFunctionsSequence =
{
  &Crypto_xVIx_xAIx_AL_DerCheckSequence,
  &Crypto_xVIx_xAIx_AL_DerFindInSequence
};

/** \brief Functions used to check validity and parse a component which shall be ignored within a
 * sequence, corresponding tag shall be set to CRYPTO_XVIX_XAIX_DER_TAG_IGNORED
 */
CONST(Crypto_xVIx_xAIx_DerParseFunctionsType, CRYPTO_xVIx_xAIx_CONST)
  Crypto_xVIx_xAIx_DerParseFunctionsSequenceIgnoreTag =
{
  &Crypto_xVIx_xAIx_AL_DerCheckSequence,
  &Crypto_xVIx_xAIx_AL_DerFindInSequenceIgnoreTag
};

/** \brief Functions used to check validity and parse a component containing a public key */
CONST(Crypto_xVIx_xAIx_DerParseFunctionsType, CRYPTO_xVIx_xAIx_CONST)
  Crypto_xVIx_xAIx_DerParseFunctionsPublicKey =
{
  &Crypto_xVIx_xAIx_AL_DerCheckPublicKey,
  &Crypto_xVIx_xAIx_AL_DerFindInSequence
};

/** \brief Functions used to check validity and parse a component containing an octet string */
CONST(Crypto_xVIx_xAIx_DerParseFunctionsType, CRYPTO_xVIx_xAIx_CONST)
  Crypto_xVIx_xAIx_DerParseFunctionsOctetstring =
{
  &Crypto_xVIx_xAIx_AL_DerCheckOctetstring,
  &Crypto_xVIx_xAIx_AL_DerFindNothing
};

/** \brief Functions used to check validity and parse a component containing a string */
CONST(Crypto_xVIx_xAIx_DerParseFunctionsType, CRYPTO_xVIx_xAIx_CONST)
  Crypto_xVIx_xAIx_DerParseFunctionsCharacterS =
{
  &Crypto_xVIx_xAIx_AL_DerCheckCharacterString,
  &Crypto_xVIx_xAIx_AL_DerFindNothing
};

/** \brief Functions used to check validity and parse a component containing an object identifier */
CONST(Crypto_xVIx_xAIx_DerParseFunctionsType, CRYPTO_xVIx_xAIx_CONST)
  Crypto_xVIx_xAIx_DerParseFunctionsObjectIdentifier =
{
  &Crypto_xVIx_xAIx_AL_DerCheckObjectIdentifier,
  &Crypto_xVIx_xAIx_AL_DerFindNothing
};

/** \brief Functions used to check validity and parse a component containing an integer */
CONST(Crypto_xVIx_xAIx_DerParseFunctionsType, CRYPTO_xVIx_xAIx_CONST)
  Crypto_xVIx_xAIx_DerParseFunctionsInteger =
{
  &Crypto_xVIx_xAIx_AL_DerCheckInteger,
  &Crypto_xVIx_xAIx_AL_DerFindNothing
};

/** \brief Functions used to check validity and parse a component containing a date */
CONST(Crypto_xVIx_xAIx_DerParseFunctionsType, CRYPTO_xVIx_xAIx_CONST) Crypto_xVIx_xAIx_DerParseFunctionsDate =
{
  &Crypto_xVIx_xAIx_AL_DerCheckDate,
  &Crypto_xVIx_xAIx_AL_DerFindNothing
};

/** \brief Component containing an object identifier */
CONST(Crypto_xVIx_xAIx_DerComponentType, CRYPTO_xVIx_xAIx_CONST) Crypto_xVIx_xAIx_DerCptObjIdentifier =
{
  &Crypto_xVIx_xAIx_DerParseFunctionsObjectIdentifier,
  0U,
  0U,
  0U,
  0U
};

/** \brief Component containing an octet string */
CONST(Crypto_xVIx_xAIx_DerComponentType, CRYPTO_xVIx_xAIx_CONST) Crypto_xVIx_xAIx_DerCptOctetString =
{
  &Crypto_xVIx_xAIx_DerParseFunctionsOctetstring,
  0U,
  0U,
  0U,
  0U
};

#define CRYPTO_xVIx_xAIx_STOP_SEC_CONST_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Variables with external linkage]=============================================================*/

/*==[Definition of functions with external linkage]===============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

FUNC(uint32, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerSubComponentContents
(
  P2CONST(Crypto_xVIx_xAIx_DerComponentType,    AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ComponentPtr,
  P2CONST(uint8,                                AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding,
  P2CONST(Crypto_xVIx_xAIx_DerSubComponentType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) SubComponentToFindPtr,
    P2VAR(uint32,                               AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ContentsLengthPtr,
          uint32                                                                       StartSearch
)
{
  uint32 CurrentPos; /* The current byte position in the encoding */

  /* Find the start of the encoding of the subcomponent. */
  if
  (
    CRYPTO_XVIX_XAIX_DER_FIND_FUNCTION
    (
      ComponentPtr,
      Encoding,
      SubComponentToFindPtr,
      &CurrentPos,
      StartSearch
    )
  )
  {
    /* Write the length of the contents of the encoding of the subcomponent.
     */
    *ContentsLengthPtr =
                   Crypto_xVIx_xAIx_AL_DerEncGetContentsLength(&Encoding[CurrentPos]);

    /* Skip to the start of the contents of the encoding of the subcomponent.
     */
    CurrentPos += Crypto_xVIx_xAIx_AL_DerEncGetContentsStart(&Encoding[CurrentPos]);

  }
  else
  {
    /* We did not find the subcomponent. */
    *ContentsLengthPtr = 0U;
    CurrentPos = 0U;
  }

  return CurrentPos;
}

/*------------------------------------------------------------------------------------------------*/

FUNC(uint32, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerEncGetContentsStart
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding
)
{
  uint32 Position; /* The current position in the encoding */

  /* Go to the position of the first length octet in the encoding. */
  Position = Crypto_xVIx_xAIx_AL_DerSkipIdentifier(Encoding);
  /* We check whether the encoding of the contents length is a single byte
   * long.
   */
  if (0U != Position)
  {
    if (CRYPTO_XVIX_XAIX_DER_ENC_LEN_THRESHOLD >= Encoding[Position])
    {
      /* The contents of the encoding start directly after the single length
       * octet.
       */
      Position += (uint32) 1U;
    }
    else
    {
      /* The contents of the encoding start after the length octets. The amount
       * of length octets is encoded in the first byte.
       */
      Position +=
           (uint32) 1U +
           (uint32) (Encoding[Position] & (uint32) (CRYPTO_XVIX_XAIX_DER_ENC_LEN_THRESHOLD));
    }
  }

  return Position;
}

/*------------------------------------------------------------------------------------------------*/

FUNC(uint32, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerEncGetContentsLength
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding
)
{
  uint8  i;            /* A loop counter */
  uint32 Length = 0U;   /* The length of the encoding */
  uint8  LengthLength; /* The length of length of the encoding */
  uint32 Position;     /* The current position in the encoding */

  /* Go to the position of the first length octet in the encoding. */
  Position = Crypto_xVIx_xAIx_AL_DerSkipIdentifier(Encoding);

  if (0U != Position) /* if Position 0 return Length 0 */
  {
    /* We check whether the encoding of the contents length is a single byte
     * long.
     */
    if (CRYPTO_XVIX_XAIX_DER_ENC_LEN_THRESHOLD >= Encoding[Position])
    {
      /* The contents length is given in the first and only length octet. */
      Length = (uint32) Encoding[Position];
    }
    else
    {
      /* The length of the length of the encoding is given in the first length
       * octet.
       */
      LengthLength = Encoding[Position] & CRYPTO_XVIX_XAIX_DER_ENC_LEN_THRESHOLD;

      /* Go to the start of the encoding of the length. */
      Position++;

      /* Skip leading zeroes in the encoding of the length.*/
      while ((0U == Encoding[Position]) && (0U < LengthLength))
      {
        Position++;
        LengthLength--;
      }

      if (LengthLength > (uint8) sizeof(Length))
      {
        /* The contents length is encoded in more bytes than we can handle. */
        Length = 0U;
      }
      else
      {
        Length = 0U;

        /* Assemble the length of the contents from the length octets. */
        for (i = 0U; i < LengthLength; i++)
        {
          /* The length is stored with most significant byte first. */
          Length = (Length << 8U) + (uint32) Encoding[Position + (uint32) i];
        }
      }
    }
  }

  return Length;
}

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Definition of functions with internal linkage]===============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerCheckIdentifier
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                                 EncodingPtr,
  P2CONST(Crypto_xVIx_xAIx_DerSubComponentType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA ) SubComponentPtr
)
{
  boolean RetVal = FALSE;
  uint32 TagValue = EncodingPtr[0U]; /* default is the one byte length tag value */
  uint32 OctetIndex = 1U;
  /* if last 5 bits are '1' that means the tag is more than one byte */
  if (0x1FU == (EncodingPtr[0U] & 0x1FU))
  {
    /* setting the rest and check if 0x80 is set (if set means we have more) */
    for (OctetIndex = 1U; CRYPTO_XVIX_XAIX_DER_TAG_LENGTH > OctetIndex; OctetIndex++)
    {
      TagValue <<= 8U;
      TagValue |= EncodingPtr[OctetIndex];

      if (0x80U > EncodingPtr[OctetIndex])
      {
        break;
      }
    }
  }

  if ((CRYPTO_XVIX_XAIX_DER_TAG_LENGTH > OctetIndex) && (TagValue == (SubComponentPtr)->Tag))
  {
    RetVal = TRUE;
  }

  return RetVal;
}

/*------------------------------------------------------------------------------------------------*/

static FUNC(uint32, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerSkipIdentifier
(
    P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) EncodingPtr
)
{
  uint32 NumOctets = 1U; /* we have at least one octet (byte) */
  /* if last 5 bits are '1' that means the tag is more than one byte */
  if (0x1FU == (EncodingPtr[0U] & 0x1FU))
  {
    for (NumOctets = 1U; CRYPTO_XVIX_XAIX_DER_TAG_LENGTH > NumOctets; NumOctets++)
    {
      if (0x80U > EncodingPtr[NumOctets])
      {
        break;
      }
    }
    /* numOctet is only the last index in array but the size is +1 */
    NumOctets += 1U;
  }

  if (CRYPTO_XVIX_XAIX_DER_TAG_LENGTH < NumOctets)
  {
    NumOctets = 0U;
  }

  return NumOctets;
}

/*------------------------------------------------------------------------------------------------*/

static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerFindNothing
(
  P2CONST(Crypto_xVIx_xAIx_DerComponentType,    AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ComponentPtr,
  P2CONST(uint8,                                AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding,
  P2CONST(Crypto_xVIx_xAIx_DerSubComponentType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) SubComponentToFindPtr,
    P2VAR(uint32,                               AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PositionPtr,
          uint32                                                                       StartSearch
)
{

  TS_PARAM_UNUSED(ComponentPtr);
  TS_PARAM_UNUSED(Encoding);
  TS_PARAM_UNUSED(SubComponentToFindPtr);
  TS_PARAM_UNUSED(StartSearch);

  /* It does not matter what position we return, since we did not find
   * anything.
   */
  *PositionPtr = 0U;

  /* We do not search but simply say that we did not find anything. */
  return FALSE;
}

static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerFindInSequenceIgnoreTag
(
  P2CONST(Crypto_xVIx_xAIx_DerComponentType,    AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ComponentPtr,
  P2CONST(uint8,                                AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding,
  P2CONST(Crypto_xVIx_xAIx_DerSubComponentType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) SubComponentToFindPtr,
    P2VAR(uint32,                               AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PositionPtr,
          uint32                                                                       StartSearch
)
{
  uint32 i;              /* A counter for the current subcomponent */
  uint32 FoundPos;       /* The position where the component has been found */
  uint32 TempPositionVal = 0U;
  boolean Found;

  Found = FALSE;
  i = 0U;

  /* Skip to the start of the contents of the sequence. */
  *PositionPtr = 0U;

  /* We go through the sequence until all subcomponents have been searched. */
  while
  (
    (FALSE == Found) &&
    (i < ComponentPtr->NrOfSubComponents)
  )
  {
    /* Check whether the current subcomponent is the component we want and
     * whether we may already start with our search at the current position.
     */
    if
    (
      (&(ComponentPtr->SubComponents[i]) == SubComponentToFindPtr) &&
      (*PositionPtr >= StartSearch)
    )
    {
      /* We have found what we were looking for and return the information.
       */
       Found = TRUE;
    }
    else
    {
      /* We recursively call the search function for this subcomponent. */
      TempPositionVal = *PositionPtr;
      if
      (
        CRYPTO_XVIX_XAIX_DER_FIND_FUNCTION
        (
          ComponentPtr->SubComponents[i].ComponentPtr,
          &Encoding[TempPositionVal],
          SubComponentToFindPtr,
          &FoundPos,
          CRYPTO_XVIX_XAIX_DER_SUBTRACT_NO_OVERFLOW(StartSearch, TempPositionVal)
        )
      )
      {
        /* The searched component was found in this subcomponent of the
         * sequence. The position of the found component is the offset of
         * the subcomponent in this sequence added to the offset in the
         * subcomponent itself.
         */
        *PositionPtr += FoundPos;
        Found = TRUE;
        break;
      }
      else
      {
        /* We did not find the component, yet. Skip this subcomponent of the
         * sequence and try the next. But if it is an optional subcomponent,
         * first check whether it actually exists.
         */
        boolean Identifier =  Crypto_xVIx_xAIx_AL_DerCheckIdentifier
            (
              &Encoding[*PositionPtr],
              &(ComponentPtr->SubComponents[i])
            );
        if
        (
          (FALSE == ComponentPtr->SubComponents[i].IsOptional) ||
          (TRUE == Identifier)
        )
        {
          *PositionPtr += Crypto_xVIx_xAIx_AL_DerEncGetContentsStart(&Encoding[*PositionPtr]) +
                          Crypto_xVIx_xAIx_AL_DerEncGetContentsLength(&Encoding[*PositionPtr]);
        }
      }
      i++;
    }
  }

  return Found;
}

/*------------------------------------------------------------------------------------------------*/

static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerFindInSequence
(
  P2CONST(Crypto_xVIx_xAIx_DerComponentType,    AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ComponentPtr,
  P2CONST(uint8,                                AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding,
  P2CONST(Crypto_xVIx_xAIx_DerSubComponentType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) SubComponentToFindPtr,
    P2VAR(uint32,                               AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PositionPtr,
          uint32                                                                       StartSearch
)
{
  uint32 i = 0U;              /* A counter for the current subcomponent */
  uint32 FoundPos = 0U;       /* The position where the component has been found */
  boolean Found = FALSE;
  uint32 EncodingLength = 0U;
  uint32 TempPositionVal = 0U;

  /* Skip to the start of the contents of the sequence. */
  *PositionPtr = Crypto_xVIx_xAIx_AL_DerEncGetContentsStart(Encoding);

  if (0U != *PositionPtr)
  {

    EncodingLength = *PositionPtr + Crypto_xVIx_xAIx_AL_DerEncGetContentsLength(Encoding);

    /* We go through the sequence until all subcomponents have been searched. */

    while
    (
      (FALSE == Found) &&
      (i < ComponentPtr->NrOfSubComponents) &&
      (*PositionPtr < EncodingLength)
    )

    {
      /* Check whether the current subcomponent is the component we want and
       * whether we may already start with our search at the current position.
       */
      if
      (
        (&(ComponentPtr->SubComponents[i]) == SubComponentToFindPtr) &&
        (*PositionPtr >= StartSearch)
      )
      {
        /* We have found what we were looking for and return the information.
         */
         Found = TRUE;
      }
      else
      {
        /* We recursively call the search function for this subcomponent. */
        TempPositionVal = *PositionPtr;
        if
        (
          TRUE == CRYPTO_XVIX_XAIX_DER_FIND_FUNCTION
            (
              ComponentPtr->SubComponents[i].ComponentPtr,
              &Encoding[TempPositionVal],
              SubComponentToFindPtr,
              &FoundPos,
              CRYPTO_XVIX_XAIX_DER_SUBTRACT_NO_OVERFLOW(StartSearch, TempPositionVal)
            )
        )
        {
          /* The searched component was found in this subcomponent of the
           * sequence. The position of the found component is the offset of
           * the subcomponent in this sequence added to the offset in the
           * subcomponent itself.
           */
          *PositionPtr += FoundPos;
          Found = TRUE;
          break;
        }
        else
        {
          /* We did not find the component, yet. Skip this subcomponent of the
           * sequence and try the next. But if it is an optional subcomponent,
           * first check whether it actually exists.
           */
          boolean Identifier =  Crypto_xVIx_xAIx_AL_DerCheckIdentifier
            (
              &Encoding[*PositionPtr],
              &(ComponentPtr->SubComponents[i])
            );
          if
          (
              (FALSE == ComponentPtr->SubComponents[i].IsOptional) ||
              (TRUE == Identifier)
          )
          {
            *PositionPtr += Crypto_xVIx_xAIx_AL_DerEncGetContentsStart(&Encoding[*PositionPtr]) +
                            Crypto_xVIx_xAIx_AL_DerEncGetContentsLength(&Encoding[*PositionPtr]);
          }
        }
      }

      i++;
    }
  }

  return Found;
}

/*------------------------------------------------------------------------------------------------*/

static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerCheckSubComponent
(
  P2CONST(Crypto_xVIx_xAIx_DerSubComponentType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) SubComponentPtr,
  P2CONST(uint8,                                AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding,
          uint32                                                                       MaxLength,
    P2VAR(uint32,                               AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) LengthPtr
)
{
  boolean Valid;
  boolean Identifier = Crypto_xVIx_xAIx_AL_DerCheckIdentifier(Encoding, SubComponentPtr);

  /* We check whether the tag in the encoding can be ignored or, if not, whether
   * the tag is the correct one for the subcomponent.
   */
  if
  (
    (CRYPTO_XVIX_XAIX_DER_TAG_IGNORED != SubComponentPtr->Tag) &&
    (FALSE == Identifier)
  )
  {
    /* The tag cannot be ignored and it is not correct. The only chance we have is that
     * the subcomponent is optional. Then we simply assume that the optional subcomponent
     * does not exist.
     */
    *LengthPtr = 0U;
    Valid = SubComponentPtr->IsOptional;
  }
  else
  {
    /* The tag is correct. Check whether the subcomponent is valid. */
    Valid =  CRYPTO_XVIX_XAIX_DER_CHECK_FUNCTION
             (
               SubComponentPtr->ComponentPtr,
               Encoding,
               MaxLength,
               LengthPtr
             );
  }

  return Valid;
}

/*------------------------------------------------------------------------------------------------*/

static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerCheckSequence
(
  P2CONST(Crypto_xVIx_xAIx_DerComponentType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ComponentPtr,
  P2CONST(uint8,                             AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding,
          uint32                                                                    MaxLength,
    P2VAR(uint32,                            AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) LengthPtr
)
{
  uint32 i;                  /* A counter for the current subcomponent */
  uint32 Position;     /* The length of the encoding of the sequence */
  uint32 SubComponentLength; /* The length of the encoding of the subcomp. */
  boolean Valid;

  Valid = TRUE;

  i = 0U;

  /* Skip to the start of the contents of the sequence. */
  Position = Crypto_xVIx_xAIx_AL_DerEncGetContentsStart(Encoding);

  /* Compute the complete length of the encoding of this sequence. */
  *LengthPtr = Position + Crypto_xVIx_xAIx_AL_DerEncGetContentsLength(Encoding);

  /* The length of the encoding may not exceed the maximal allowed length. */
  if ((*LengthPtr > MaxLength) || (0U == Position))
  {
    Valid = FALSE;
  }
  else
  {
    /* Check all subcomponents of the sequence but stop if we exceed the
     * computed length of the encoding.
     */

    while
    (
      (TRUE == Valid) &&
      (i < ComponentPtr->NrOfSubComponents) &&
      (Position <= *LengthPtr)
    )

    {
      /* Check if the current subcomponent is valid. */
      if
      (
        Crypto_xVIx_xAIx_AL_DerCheckSubComponent
        (
          &(ComponentPtr->SubComponents[i]),
          &Encoding[Position],
          *LengthPtr - Position,
          &SubComponentLength
        )
      )
      {
        /* Add the length of the encoding of the subcomponent. */
        Position += SubComponentLength;

        /* Check the next subcomponent. */
        i++;
      }
      else
      {
        Valid = FALSE;
      }
    }

    if ((Position != *LengthPtr) || (i != ComponentPtr->NrOfSubComponents))
    {
      /* Either the length of the sequence or the number of its subcomponents
       * was incorrect. The sequence was invalid.
       */
      Valid = FALSE;
    }
  }

  return Valid;
}

static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerCheckPublicKey
(
  P2CONST(Crypto_xVIx_xAIx_DerComponentType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ComponentPtr,
  P2CONST(uint8,                             AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding,
          uint32                                                                    MaxLength,
    P2VAR(uint32,                            AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) LengthPtr
)
{
  uint32 i;                  /* A counter for the current subcomponent */
  uint32 Position;     /* The length of the encoding of the sequence */
  uint32 SubComponentLength; /* The length of the encoding of the subcomp. */
  boolean Found;

  Found = FALSE;

  i = 0U;

  /* Skip to the start of the contents of the sequence. */
  Position = Crypto_xVIx_xAIx_AL_DerEncGetContentsStart(Encoding);

  /* Compute the complete length of the encoding of this sequence. */
  *LengthPtr = Position + Crypto_xVIx_xAIx_AL_DerEncGetContentsLength(Encoding);

  /* The length of the encoding may not exceed the maximal allowed length. */
  if ((*LengthPtr > MaxLength) || (0U == Position))
  {
    Found = FALSE;
  }
  else
  {
    /* Check if exaclty one valid subcomponent exists but stop if we exceed the
     * computed length of the encoding.
     */

    while
    (
      (FALSE == Found) &&
      (i < ComponentPtr->NrOfSubComponents) &&
      (Position <= *LengthPtr)
    )

    {
      Position = 0U;
      /* Check if the current subcomponent is valid. */
      if
      (
        Crypto_xVIx_xAIx_AL_DerCheckSubComponent
        (
          &(ComponentPtr->SubComponents[i]),
          &Encoding[Position],
          *LengthPtr - Position,
          &SubComponentLength
        )
      )
      {
        Found = TRUE;
        Position += SubComponentLength;
      }
      else
      {
        /* Check the next subcomponent. */
        i++;
      }
    }

    if (Position != *LengthPtr)
    {
      /* The length of the sequence was incorrect. The sequence was invalid. */
      Found = FALSE;
    }
  }

  return Found;
}

/*------------------------------------------------------------------------------------------------*/

static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerCheckOctetstring
(
  P2CONST(Crypto_xVIx_xAIx_DerComponentType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ComponentPtr,
  P2CONST(uint8,                             AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding,
          uint32                                                                    MaxLength,
    P2VAR(uint32,                            AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) LengthPtr
)
{
  uint32 Position;
  boolean Valid;

  TS_PARAM_UNUSED(ComponentPtr);

  Valid = TRUE;

  /* Skip to the start of the contents of the octetstring. */
  Position = Crypto_xVIx_xAIx_AL_DerEncGetContentsStart(Encoding);

  /* Compute the complete length of the encoding of this octetstring. */
  *LengthPtr = Position + Crypto_xVIx_xAIx_AL_DerEncGetContentsLength(Encoding);

  /* The length of the encoding may not exceed the maximal allowed length. */
  if ((*LengthPtr > MaxLength) || (0U == *LengthPtr))
  {
    Valid = FALSE;
  }

  return Valid;
}

/*------------------------------------------------------------------------------------------------*/

static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerCheckObjectIdentifier
(
  P2CONST(Crypto_xVIx_xAIx_DerComponentType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ComponentPtr,
  P2CONST(uint8,                             AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding,
          uint32                                                                    MaxLength,
    P2VAR(uint32,                            AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) LengthPtr
)
{
  uint32 Position;
  boolean Valid;

  TS_PARAM_UNUSED(ComponentPtr);

  Valid = TRUE;

  /* Skip to the start of the contents of the object identifier. */
  Position = Crypto_xVIx_xAIx_AL_DerEncGetContentsStart(Encoding);

  /* Compute the complete length of the encoding of this object identifier. */
  *LengthPtr = Position + Crypto_xVIx_xAIx_AL_DerEncGetContentsLength(Encoding);

  /* The length of the encoding may not exceed the maximal allowed length. */
  if ((*LengthPtr > MaxLength) || (0U == *LengthPtr))
  {
    Valid = FALSE;
  }

  return Valid;
}

/*------------------------------------------------------------------------------------------------*/

/* !LINKSTO EB_Crypto_CryptAlgos_0971, 1 */
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerCheckCharacterString
(
  P2CONST(Crypto_xVIx_xAIx_DerComponentType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ComponentPtr,
  P2CONST(uint8,                             AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding,
          uint32                                                                    MaxLength,
    P2VAR(uint32,                            AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) LengthPtr
)
{
  uint32 Position;
  uint8  CurrentByte;    /* The current byte of the string */
  boolean Valid;

  TS_PARAM_UNUSED(ComponentPtr);

  Valid = TRUE;

  /* Skip to the start of the contents of the CharacterString. */
  Position = Crypto_xVIx_xAIx_AL_DerEncGetContentsStart(Encoding);

  /* Compute the complete length of the encoding of this CharacterString. */
  *LengthPtr = Position + Crypto_xVIx_xAIx_AL_DerEncGetContentsLength(Encoding);

  /* The length of the encoding may not exceed the maximal allowed length. */
  if ((*LengthPtr > MaxLength) || (0U == *LengthPtr))
  {
    Valid = FALSE;
  }
  else
  {
    /* Check all bytes of the string. */
    while (Position < *LengthPtr)
    {
      CurrentByte = Encoding[Position];

      if
      (
        /* The character codes 0x00-0x1F and 0x7F-0x9F are unassigned
         * and MUST NOT be used. D.2.1.4 (BSI_TR-03110_Part-3-V2_2.pdf)
         */
        ((uint8) 0x20U > CurrentByte) ||
        (((uint8) 0x7EU < CurrentByte) && ((uint8) 0xA0U > CurrentByte))
      )

      {
        /* This byte is not allowed in a CharacterString. */
        Valid = FALSE;
      }

      Position++;
    }
  }

  /* The CharacterString is valid. Return the length of its encoding. */
  return Valid;
}

/*------------------------------------------------------------------------------------------------*/

/* !LINKSTO EB_Crypto_CryptAlgos_0972, 1 */
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerCheckInteger
(
  P2CONST(Crypto_xVIx_xAIx_DerComponentType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ComponentPtr,
  P2CONST(uint8,                             AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding,
          uint32                                                                    MaxLength,
    P2VAR(uint32,                            AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) LengthPtr
)
{
  uint32  Value;          /* The encoded value */
  boolean Overflow;       /* Is the encoded value too large? */
  uint32  Position;
  boolean Valid;

  Valid = TRUE;

  /* Skip to the start of the contents. */
  Position = Crypto_xVIx_xAIx_AL_DerEncGetContentsStart(Encoding);

  /* Compute the complete length of the encoding. */
  *LengthPtr = Position + Crypto_xVIx_xAIx_AL_DerEncGetContentsLength(Encoding);

  /* The length of the encoding may not exceed the maximal allowed length. */
  if ((*LengthPtr > MaxLength) || (0U == *LengthPtr))
  {
    Valid = FALSE;
  }
  else
  {
    /* Check if the contents are at least two bytes long. */
    if ((uint32) 1U < (*LengthPtr - Position))
    {
      /* The first nine bits of the contents of an integer may not all be
       * zeroes.
       */
      if
      (
        (0U ==  Encoding[Position]) &&
        (0U == (Encoding[Position + (uint32) 1U] & (uint8) 0x80U))
      )
      {
        Valid = FALSE;
      }

      /* The first nine bits of the contents of an integer may not all be ones.
       */
      if
      (
        ((uint8) 0xFFU ==  Encoding[Position]) &&
        ((uint8) 0x80U == (Encoding[Position + (uint32) 1U] & (uint8) 0x80U))
      )
      {
        Valid = FALSE;
      }
    }

    /* Does this value contain restrictions on the allowed values? */
    if ((TRUE == Valid) && (NULL_PTR != ComponentPtr->Ranges))
    {
      /* We only allow positive values as restrictions. So, any negative number
       * must fail the restriction check. We check whether this
       * is a negative integer. Since two's complement encoding is used, this
       * amounts to checking the first bit of the encoding, the sign bit.
       */
      if (0U != (Encoding[Position] & (uint8) 0x80U))
      {
        Valid = FALSE;
      }
      else
      {
        /* Skip a possible leading zero in the encoding of the value. */
        if (0U == Encoding[Position])
        {
          (Position)++;
        }

        /* Check whether the encoded number is too large to fit into "Value". */
        if ((uint32) sizeof(Value) < (*LengthPtr - Position))
        {
          Overflow = TRUE;
          Value = 0U;
        }
        else
        {
          Overflow = FALSE;

          /* Write the value into "Value". */
          Value = 0U;
          while (Position < *LengthPtr)
          {
            Value = (Value << 8U) + (uint32) Encoding[Position];

            Position++;
          }
        }

        /* Check whether the value honors the restrictions. */
        if
        (
          FALSE ==
          Crypto_xVIx_xAIx_AL_DerCheckRanges
          (
             Value,
             Overflow,
             ComponentPtr->NrOfRanges,
             ComponentPtr->Ranges
          )
        )
        {
          Valid = FALSE;
        }
      }
    }
  }

  /* The value is valid. Return the length of its encoding. */
  return Valid;
}

/*------------------------------------------------------------------------------------------------*/

/* !LINKSTO EB_Crypto_CryptAlgos_0973, 1 */
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerCheckDate
(
  P2CONST(Crypto_xVIx_xAIx_DerComponentType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ComponentPtr,
  P2CONST(uint8,                             AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding,
          uint32                                                                    MaxLength,
    P2VAR(uint32,                            AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) LengthPtr
)
{
  uint32  Position;
  boolean Valid;

  TS_PARAM_UNUSED(ComponentPtr);

  Valid = TRUE;

  /* Skip to the start of the contents. */
  Position = Crypto_xVIx_xAIx_AL_DerEncGetContentsStart(Encoding);

  /* Compute the complete length of the encoding. */
  *LengthPtr = Position + Crypto_xVIx_xAIx_AL_DerEncGetContentsLength(Encoding);

  /* The length of the encoding may not exceed the maximal allowed length. */
  /* A date is encoded in 6 digits plus tag and length is 9 (D.2.1.3) */
  if ((*LengthPtr > MaxLength) ||
      (*LengthPtr != (CRYPTO_XVIX_XAIX_DER_DATELENGTH + Position))
     )
  {
    Valid = FALSE;
  }
  else
  {
    uint8 Year;
    uint8 Month;
    uint8 Day;

    Year  = (Encoding[Position]      * 10U) + Encoding[Position + 1U];
    Month = (Encoding[Position + 2U] * 10U) + Encoding[Position + 3U];
    Day   = (Encoding[Position + 4U] * 10U) + Encoding[Position + 5U];

    if (FALSE == Crypto_xVIx_xAIx_AL_DerCheckValidDate(Year, Month, Day))
    {
      Valid = FALSE;
    }
  }

  /* The value is valid. Return the length of its encoding. */
  return Valid;
}

/*------------------------------------------------------------------------------------------------*/

static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerCheckRanges
(
          uint32                                      Value,
          boolean                                     Overflow,
          uint32                                      NrOfRanges,
  P2CONST(Crypto_xVIx_xAIx_DerRangeType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Ranges
)
{
  uint32 i; /* A counter for the current range to test. */
  boolean Valid;

  Valid = FALSE;

  /* Check every valid range. */
  for (i = 0U; i < NrOfRanges; i++)
  {
    if (FALSE != Overflow)
    {
      /* If the given value has overflow, it is in the current
       * range if and only if the range is unbounded from above. The lower
       * bound can be ignored because an overflowing value is guaranteed
       * to be larger than the lower bound.
       */
      if (FALSE != Ranges[i].Unbounded)
      {
        Valid = TRUE;
      }
    }
    else
    {
      /* The value has not overflown. */
      if
      (
        /* The value must not be smaller than the lower bound. */
        (Value >= Ranges[i].Min) &&
        (
           /* The range must either be unbounded from above or the value
            * must not be larger than the upper bound.
            */
           (FALSE != Ranges[i].Unbounded) ||
           (Value <= Ranges[i].Max)
        )
      )
      {
        /* The value belongs to one of the valid ranges. */
        Valid = TRUE;
      }
    }
  }

  return Valid;
}

/*------------------------------------------------------------------------------------------------*/

static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerCheckValidDate
(
  uint8 Year,
  uint8 Month,
  uint8 Day
)
{
  boolean DateValid = FALSE;
  if
  (
    (0U < Month) &&
    (
      Month <
        ((sizeof(Crypto_xVIx_xAIx_DaysPerMonth) / sizeof(Crypto_xVIx_xAIx_DaysPerMonth[0U])) + 1U)
    )
  )
  {
    uint8 DaysInMonth = Crypto_xVIx_xAIx_DaysPerMonth[Month - 1U];

    /* leap year checking, if ok add 29 days to february */
    /* February is index 2U */
    if ((0U == (Year % 4U)) && (2U == Month))
    {
      DaysInMonth++;
    }

    if ((0U < Day) && (Day <= DaysInMonth))
    {
      DateValid = TRUE;
    }
  }

  return DateValid;
}

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[end of file]=================================================================================*/

