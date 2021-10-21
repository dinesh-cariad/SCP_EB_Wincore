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

#ifndef CRYPTO_XVIX_XAIX_AL_DER_H
#define CRYPTO_XVIX_XAIX_AL_DER_H

/*==[Includes]====================================================================================*/

#include <Std_Types.h> /* standard types */

/*==[Macros]======================================================================================*/

#if (defined CRYPTO_XVIX_XAIX_DER_CHECK_FUNCTION)
#error CRYPTO_XVIX_XAIX_DER_CHECK_FUNCTION is already defined
#endif
/** \brief Calls the check function for the given component.
 *
 * Given a description of a component and the start of the encoding of the
 * component, this macro calls the check function for that component. The check
 * function is extracted from the component description.
 *
 * \param[in] ComponentPtr pointer to the description of a component to check
 * \param[in] Encoding pointer to the start of the encoding of the component to
 *                     check
 * \param[in]  MaxLength The maximal allowed length of the encoding of the
 *                      component.
 * \param[out] LengthPtr A pointer to a variable where the length of the
 *                       encoding of the value will be stored if the
 *                       encoding is valid
 *
 *
 * \return The validity of the encoding of the component
 *
 * \retval ::TRUE If the encoding is valid
 * \retval ::FALSE If the encoding is invalid
 */
#define CRYPTO_XVIX_XAIX_DER_CHECK_FUNCTION(ComponentPtr, Encoding, MaxLength, LengthPtr)  \
  (                                                                                        \
    (ComponentPtr)->ParseFunctionsPtr->CheckFunction(                                      \
                                                      (ComponentPtr),                      \
                                                      (Encoding),                          \
                                                      (MaxLength),                         \
                                                      (LengthPtr)                          \
                                                    )                                      \
  )

#if (defined CRYPTO_XVIX_XAIX_DER_FIND_FUNCTION)
#error CRYPTO_XVIX_XAIX_DER_FIND_FUNCTION is already defined
#endif
/** \brief Calls the find function for the given component.
 *
 * Given a description of a component, the start of the encoding of the
 * component, and a description of the subcomponent to find, this macro calls
 * the find function for the component. The find function is extracted from the
 * component description.
 *
 * \param[in]  ComponentPtr A pointer to the description of the component which
 *                          has to be searched
 * \param[in]  Encoding A pointer to the start of the encoding of the component
 * \param[in]  SubComponentToFindPtr A pointer to the description of the
 *                                   subcomponent which has to be found
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
#define CRYPTO_XVIX_XAIX_DER_FIND_FUNCTION(                                   \
                                            ComponentPtr,                     \
                                            Encoding,                         \
                                            SubComponentToFindPtr,            \
                                            PositionPtr,                      \
                                            StartSearch                       \
                                          )                                   \
  (                                                                           \
    (ComponentPtr)->ParseFunctionsPtr->FindFunction(                          \
                                                     (ComponentPtr),          \
                                                     (Encoding),              \
                                                     (SubComponentToFindPtr), \
                                                     (PositionPtr),           \
                                                     (StartSearch)            \
                                                   )                          \
  )

#if (defined CRYPTO_XVIX_XAIX_DER_1ST_SUB_CMP_CONTENTS)
#error CRYPTO_XVIX_XAIX_DER_1ST_SUB_CMP_CONTENTS is already defined
#endif
/** \brief Returns the contents of a subcomponent stored in a given component.
 *
 * Given a pointer to the start of the DER encoding of a component, and a
 * description of a subcomponent, this function returns the byte position of the
 * start of the contents of the subcomponent and it also returns the length of
 * the contents of the subcomponent.
 *
 * \param[in]  ComponentPtr A pointer to the description of the component which
 *                          has to be searched
 * \param[in]  Encoding A pointer to the start of the encoding
 * \param[in]  ComponentToFindPtr A pointer to the description of the
 *                                subcomponent to find
 * \param[out] ContentsLengthPtr A pointer to a variable where the length of
 *                               the contents of the subcomponent will be stored
 *
 * \return the amount of bytes to skip in the encoding of the component
 *         to reach the start of the contents of the encoding of the
 *         first instance of the subcomponent
 */
#define CRYPTO_XVIX_XAIX_DER_1ST_SUB_CMP_CONTENTS(                        \
                                                   ComponentPtr,          \
                                                   Encoding,              \
                                                   SubComponentToFindPtr, \
                                                   ContentsLengthPtr      \
                                                 )                        \
  Crypto_xVIx_xAIx_AL_DerSubComponentContents(                            \
                                               (ComponentPtr),            \
                                               (Encoding),                \
                                               (SubComponentToFindPtr),   \
                                               (ContentsLengthPtr),       \
                                               0U                         \
                                             )

#if (defined CRYPTO_XVIX_XAIX_DER_ARRAY_INFO)
#error CRYPTO_XVIX_XAIX_DER_ARRAY_INFO is already defined
#endif
/** \brief Prefixes a given array with its length information.
 *
 * Crypto_xVIx_xAIx_DerComponentStruct stores pointers to the start of arrays. Those arrays
 * must be traversed completely. To be able to do this, the struct also stores
 * the sizes of those arrays. This macro, when given an array, computes the
 * size of the array and returns a struct initializer which contains the size
 * of the array followed by the array itself. The value returned by this macro
 * can then be used in the initializer for a constant of type
 * Crypto_xVIx_xAIx_DerComponentStruct.
 *
 * \param[in]  array An array who is used in the initialization of a constant of
 *             type Crypto_xVIx_xAIx_DerComponentStruct.
 *
 * \return A struct initializer which contains the size of the array followed by
 *         the array itself
 */
#define CRYPTO_XVIX_XAIX_DER_ARRAY_INFO(Array)                         \
  (((uint32) sizeof(Array)) / ((uint32) sizeof((Array)[0U]))), (Array) \

#if (defined CRYPTO_XVIX_XAIX_DER_TAG_LENGTH)
#error CRYPTO_XVIX_XAIX_DER_TAG_LENGTH is already defined
#endif
/** \brief Tag length which is supported by DER parser */
#define  CRYPTO_XVIX_XAIX_DER_TAG_LENGTH ((uint8) sizeof(uint32))

#if (defined CRYPTO_XVIX_XAIX_DER_TAG_AUTHREF)
#error CRYPTO_XVIX_XAIX_DER_TAG_AUTHREF is already defined
#endif
/** \brief Encoding of the class "Authority Reference" in an ASN.1 tag */
#define CRYPTO_XVIX_XAIX_DER_TAG_AUTHREF ((uint32) 0x42U)

#if (defined CRYPTO_XVIX_XAIX_DER_TAG_DISCRETIONDATA)
#error CRYPTO_XVIX_XAIX_DER_TAG_DISCRETIONDATA is already defined
#endif
/** \brief Encoding of the class "Discretionary Data" in an ASN.1 tag */
#define CRYPTO_XVIX_XAIX_DER_TAG_DISCRETIONDATA ((uint32) 0x53U)

#if (defined CRYPTO_XVIX_XAIX_DER_TAG_MODULUS)
#error CRYPTO_XVIX_XAIX_DER_TAG_MODULUS is already defined
#endif
/** \brief Encoding of the class "Key Modulus" in an ASN.1 tag */
#define CRYPTO_XVIX_XAIX_DER_TAG_MODULUS ((uint32) 0x81U)

#if (defined CRYPTO_XVIX_XAIX_DER_TAG_EXP)
#error CRYPTO_XVIX_XAIX_DER_TAG_EXP is already defined
#endif
/** \brief Encoding of the class "Key Exponent" in an ASN.1 tag */
#define CRYPTO_XVIX_XAIX_DER_TAG_EXP ((uint32) 0x82U)

#if (defined CRYPTO_XVIX_XAIX_DER_TAG_PRIME_MODULUS)
#error CRYPTO_XVIX_XAIX_DER_TAG_PRIME_MODULUS is already defined
#endif
/** \brief Encoding of the class "Key Modulus" in an ASN.1 tag */
#define CRYPTO_XVIX_XAIX_DER_TAG_PRIME_MODULUS ((uint32) 0x81U)

#if (defined CRYPTO_XVIX_XAIX_DER_TAG_FIRST_COEFF)
#error CRYPTO_XVIX_XAIX_DER_TAG_FIRST_COEFF is already defined
#endif
/** \brief Encoding of the class "Key Exponent" in an ASN.1 tag */
#define CRYPTO_XVIX_XAIX_DER_TAG_FIRST_COEFF ((uint32) 0x82U)

#if (defined CRYPTO_XVIX_XAIX_DER_TAG_SECOND_COEFF)
#error CRYPTO_XVIX_XAIX_DER_TAG_SECOND_COEFF is already defined
#endif
/** \brief Encoding of the class "Key Exponent" in an ASN.1 tag */
#define CRYPTO_XVIX_XAIX_DER_TAG_SECOND_COEFF ((uint32) 0x83U)

#if (defined CRYPTO_XVIX_XAIX_DER_TAG_BASE_POINT)
#error CRYPTO_XVIX_XAIX_DER_TAG_BASE_POINT is already defined
#endif
/** \brief Encoding of the class "Key Exponent" in an ASN.1 tag */
#define CRYPTO_XVIX_XAIX_DER_TAG_BASE_POINT ((uint32) 0x84U)

#if (defined CRYPTO_XVIX_XAIX_DER_TAG_BASE_POINT_ORDER)
#error CRYPTO_XVIX_XAIX_DER_TAG_BASE_POINT_ORDER is already defined
#endif
/** \brief Encoding of the class "Key Exponent" in an ASN.1 tag */
#define CRYPTO_XVIX_XAIX_DER_TAG_BASE_POINT_ORDER ((uint32) 0x85U)

#if (defined CRYPTO_XVIX_XAIX_DER_TAG_PUBLIC_POINT)
#error CRYPTO_XVIX_XAIX_DER_TAG_PUBLIC_POINT is already defined
#endif
/** \brief Encoding of the class "Key Exponent" in an ASN.1 tag */
#define CRYPTO_XVIX_XAIX_DER_TAG_PUBLIC_POINT ((uint32) 0x86U)

#if (defined CRYPTO_XVIX_XAIX_DER_TAG_COFACTOR)
#error CRYPTO_XVIX_XAIX_DER_TAG_COFACTOR is already defined
#endif
/** \brief Encoding of the class "Key Exponent" in an ASN.1 tag */
#define CRYPTO_XVIX_XAIX_DER_TAG_COFACTOR ((uint32) 0x87U)

#if (defined CRYPTO_XVIX_XAIX_DER_TAG_HOLDREF)
#error CRYPTO_XVIX_XAIX_DER_TAG_HOLDREF is already defined
#endif
/** \brief Encoding of the class "Holder Reference" in an ASN.1 tag */
#define CRYPTO_XVIX_XAIX_DER_TAG_HOLDREF ((uint32) 0x5F20U)

#if (defined CRYPTO_XVIX_XAIX_DER_TAG_EXPIRATIONDATE)
#error CRYPTO_XVIX_XAIX_DER_TAG_EXPIRATIONDATE is already defined
#endif
/** \brief Encoding of the class "Expiration Data" in an ASN.1 tag */
#define CRYPTO_XVIX_XAIX_DER_TAG_EXPIRATIONDATE ((uint32) 0x5F24U)

#if (defined CRYPTO_XVIX_XAIX_DER_TAG_EFFECTIVEDATE)
#error CRYPTO_XVIX_XAIX_DER_TAG_EFFECTIVEDATE is already defined
#endif
/** \brief Encoding of the class "Effective Data" in an ASN.1 tag */
#define CRYPTO_XVIX_XAIX_DER_TAG_EFFECTIVEDATE ((uint32) 0x5F25U)

#if (defined CRYPTO_XVIX_XAIX_DER_TAG_PROFILE_ID)
#error CRYPTO_XVIX_XAIX_DER_TAG_PROFILE_ID is already defined
#endif
/** \brief Encoding of the class "Profile Identifier" in an ASN.1 tag */
#define CRYPTO_XVIX_XAIX_DER_TAG_PROFILE_ID ((uint32) 0x5F29U)

#if (defined CRYPTO_XVIX_XAIX_DER_TAG_SIGNATURE)
#error CRYPTO_XVIX_XAIX_DER_TAG_SIGNATURE is already defined
#endif
/** \brief Encoding of the class "Signature" in an ASN.1 tag */
#define CRYPTO_XVIX_XAIX_DER_TAG_SIGNATURE ((uint32) 0x5F37U)

#if (defined CRYPTO_XVIX_XAIX_DER_TAG_PUBKEY)
#error CRYPTO_XVIX_XAIX_DER_TAG_PUBKEY is already defined
#endif
/** \brief Encoding of the class "Public Key" in an ASN.1 tag */
#define CRYPTO_XVIX_XAIX_DER_TAG_PUBKEY ((uint32) 0x7F49U)

#if (defined CRYPTO_XVIX_XAIX_DER_TAG_HOLDAUTHTEMPL)
#error CRYPTO_XVIX_XAIX_DER_TAG_HOLDAUTHTEMPL is already defined
#endif
/** \brief Encoding of the class "Holder Authorization Template" in an ASN.1 tag */
#define CRYPTO_XVIX_XAIX_DER_TAG_HOLDAUTHTEMPL ((uint32) 0x7F4CU)

#if (defined CRYPTO_XVIX_XAIX_DER_TAG_CERTBODY)
#error CRYPTO_XVIX_XAIX_DER_TAG_CERTBODY is already defined
#endif
/** \brief Encoding of the class "Body" in an ASN.1 tag */
#define CRYPTO_XVIX_XAIX_DER_TAG_CERTBODY ((uint32) 0x7F4EU)

#if (defined CRYPTO_XVIX_XAIX_DER_TAG_OID)
#error CRYPTO_XVIX_XAIX_DER_TAG_OID is already defined
#endif
/** \brief Encoding of the class "Object Identifier" in an ASN.1 tag */
#define CRYPTO_XVIX_XAIX_DER_TAG_OID ((uint32) 0x06U)

#if (defined CRYPTO_XVIX_XAIX_DER_TAG_IGNORED)
#error CRYPTO_XVIX_XAIX_DER_TAG_IGNORED is already defined
#endif
/** \brief ASN.1 tag which is ignored */
#define  CRYPTO_XVIX_XAIX_DER_TAG_IGNORED ((uint8) 0x00U)

/*==[Types]=======================================================================================*/

/** \struct Crypto_xVIx_xAIx_DerRangeType
 *  \brief A range of allowed values.
 *
 * This is used in when restricting the allowed values of several types. Types
 * whose allowed values can be restricted are "integer", "enumerated", and
 * "set-of".
 */
/** \var Crypto_xVIx_xAIx_DerRangeType::Min
 *  \brief The minimal allowed value.
 */
/** \var Crypto_xVIx_xAIx_DerRangeType::Max
 *  \brief The maximal allowed value. Only used when Crypto_xVIx_xAIx_DerRangeType::Unbounded
 *         is ::FALSE
 */
/** \var Crypto_xVIx_xAIx_DerRangeType::Unbounded
 *  \brief If this is ::TRUE, the range is not bounded from above. This means
 *         that Crypto_xVIx_xAIx_DerRangeType::Max will be ignored.
 */
typedef struct
{
  uint32  Min;
  uint32  Max;
  boolean Unbounded;
} Crypto_xVIx_xAIx_DerRangeType;

/* These structs are defined below. This declaration is needed because otherwise we
 * would have circular dependencies in the definitions of
 * Crypto_xVIx_xAIx_DerFindFctPtrType, Crypto_xVIx_xAIx_DerParseFunctionsType,
 * Crypto_xVIx_xAIx_DerComponentStruct, and Crypto_xVIx_xAIx_DerSubComponentStruct.
 */
struct Crypto_xVIx_xAIx_DerComponentStruct;
struct Crypto_xVIx_xAIx_DerSubComponentStruct;

/** \brief The type used for all find functions of the parser.
 *
 * Given a description of a component, the start of the encoding of the
 * component, and a description of the subcomponent to find, the function
 * searches the subcomponent in the encoding of the component.
 *
 * \param[in]  ComponentPtr A pointer to the description of the component which
 *                          has to be searched
 * \param[in]  Encoding A pointer to the start of the encoding of the component
 * \param[in]  SubComponentToFindPtr A pointer to the description of the
 *                                  subcomponent which has to be found
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
typedef P2FUNC(boolean, CRYPTO_xVIx_xAIx_CODE, Crypto_xVIx_xAIx_DerFindFctPtrType)
(

  P2CONST(struct Crypto_xVIx_xAIx_DerComponentStruct, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ComponentPtr,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding,
  P2CONST(struct Crypto_xVIx_xAIx_DerSubComponentStruct, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)
  SubComponentToFindPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PositionPtr,
  uint32 StartSearch
);

/** \brief The type used for all check functions of the parser.
 *
 * Given a description of a component and the start of the encoding of the
 * component, the function checks whether the component is valid and whether
 * its encoding is not longer than the maximal allowed length.
 *
 * \param[in] ComponentPtr pointer to the description of a component to check
 * \param[in] Encoding pointer to the start of the encoding of the component to
 *                     check
 * \param[in]  MaxLength The maximal allowed length of the encoding of the
 *                      component.
 * \param[out] LengthPtr A pointer to a variable where the length of the
 *                       encoding of the value will be stored if the
 *                       encoding is valid
 *
 *
 * \return The validity of the encoding of the component
 *
 * \retval ::TRUE If the encoding is valid
 * \retval ::FALSE If the encoding is invalid
 */
typedef P2FUNC(boolean, CRYPTO_xVIx_xAIx_CODE, Crypto_xVIx_xAIx_DerCheckFctPtrType)
(
  P2CONST(struct Crypto_xVIx_xAIx_DerComponentStruct, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ComponentPtr,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding,
  uint32 MaxLength,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) LengthPtr
);

/** \struct Crypto_xVIx_xAIx_DerParseFunctionsType
 *  \brief The functions needed to parse the DER encoding of an ASN.1 component
 *         of a certain type.
 *
 *//** \var Crypto_xVIx_xAIx_DerParseFunctionsType::CheckFunction
 *  \brief The function to check the validity of the encoding of the component.
 *
 *//** \var Crypto_xVIx_xAIx_DerParseFunctionsType::FindFunction
 *  \brief The function to search a given subcomponent in the encoding of the
 *         component.
 */
typedef struct
{
  Crypto_xVIx_xAIx_DerCheckFctPtrType CheckFunction;
  Crypto_xVIx_xAIx_DerFindFctPtrType  FindFunction;
} Crypto_xVIx_xAIx_DerParseFunctionsType;

/** \struct Crypto_xVIx_xAIx_DerComponentStruct
 *  \brief A description of an ASN.1 component.
 *
 *//** \var Crypto_xVIx_xAIx_DerComponentStruct::ParseFunctionsPtr
 *  \brief A pointer to the parse functions which have to be used for this
 *         component
 *
 *//** \var Crypto_xVIx_xAIx_DerComponentStruct::NrOfRanges
 *  \brief The size of the array Crypto_xVIx_xAIx_DerComponentStruct::Ranges.
 *
 *//** \var Crypto_xVIx_xAIx_DerComponentStruct::Ranges
 *  \brief An array containing the allowed values if the component is
 *         restricted.
 *
 * Some components can be restricted in certain ways. This is a
 * pointer to the start of an array which contains the allowed ranges for the
 * restrictions:
 *
 * \li Components of type "enumerated" must have one of several given values.
 *
 * \li Components of type "integer" can sometimes be restricted to several given
 *     values.
 *
 * \li Components of type "set-of" may sometimes only have a certain amount of
 *     subcomponents.
 *
 *//** \var Crypto_xVIx_xAIx_DerComponentStruct::NrOfSubComponents
 *  \brief The size of the array Crypto_xVIx_xAIx_DerComponentStruct::SubComponents.
 *
 *//** \var Crypto_xVIx_xAIx_DerComponentStruct::SubComponents
 *  \brief An pointer to the start of an array which contains the description of
 *         the subcomponents of this component.
 *
 * Several "constructed" component types consist of subcomponents. This array
 * describes the subcomponents:
 *
 * \li A component of type "sequence" consists of a list of subcomponents. This
 *     array must then be a list of those subcomponents.
 *
 * \li A component of type "choice" consists of one subcomponent from a list of
 *     allowed subcomponents. This array must then be a list of those allowed
 *     subcomponents.
 *
 * \li A component of type "set-of" consists of a group of subcomponents of a
 *     given type. This array must then contain a single entry which is the
 *     description of the allowed subcomponent type.
 */
struct Crypto_xVIx_xAIx_DerComponentStruct
{
  P2CONST(Crypto_xVIx_xAIx_DerParseFunctionsType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA) ParseFunctionsPtr;
  uint32 NrOfRanges;
  P2CONST(Crypto_xVIx_xAIx_DerRangeType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA) Ranges;
  uint32 NrOfSubComponents;
  P2CONST(struct Crypto_xVIx_xAIx_DerSubComponentStruct, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA) SubComponents;
};

/** \struct Crypto_xVIx_xAIx_DerSubComponentStruct
 *  \brief A description of the subcomponent of an ASN.1 component.
 *
 *//** \var Crypto_xVIx_xAIx_DerSubComponentStruct::Tag
 *  \brief The ASN.1 tag of the subcomponent
 *
 *//** \var Crypto_xVIx_xAIx_DerSubComponentStruct::ComponentPtr
 *  \brief A description of the subcomponent
 *
 *//** \var Crypto_xVIx_xAIx_DerSubComponentStruct::IsOptional
 *  \brief Is this subcomponent optional in the component?
 *
 */
struct Crypto_xVIx_xAIx_DerSubComponentStruct
{
  uint32 Tag;
  P2CONST(struct Crypto_xVIx_xAIx_DerComponentStruct, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA) ComponentPtr;
  boolean IsOptional;
};

/** \brief A description of an ASN.1 subcomponent of a component, identical to
 *         Crypto_xVIx_xAIx_DerSubComponentStruct.
 */
typedef struct Crypto_xVIx_xAIx_DerSubComponentStruct Crypto_xVIx_xAIx_DerSubComponentType;

/** \brief A description of an ASN.1 component, identical to
 *         Crypto_xVIx_xAIx_DerComponentStruct.
 */
typedef struct Crypto_xVIx_xAIx_DerComponentStruct Crypto_xVIx_xAIx_DerComponentType;

/*==[Constants with external linkage]=============================================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CONST_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief The functions needed to parse the encoding of a "sequence".
 *
 * See X.690, chapter 8.9.
 */
extern CONST(Crypto_xVIx_xAIx_DerParseFunctionsType, CRYPTO_xVIx_xAIx_CONST)
  Crypto_xVIx_xAIx_DerParseFunctionsSequence;

/** \brief The functions needed to parse the encoding of a "sequence" without a tag.
 */
extern CONST(Crypto_xVIx_xAIx_DerParseFunctionsType, CRYPTO_xVIx_xAIx_CONST)
  Crypto_xVIx_xAIx_DerParseFunctionsSequenceIgnoreTag;

extern CONST(Crypto_xVIx_xAIx_DerParseFunctionsType, CRYPTO_xVIx_xAIx_CONST)
  Crypto_xVIx_xAIx_DerParseFunctionsPublicKey;

/** \brief The functions needed to parse the encoding of a "octetstring".
 *
 * See X.690, chapter 8.7.
 */
extern CONST(Crypto_xVIx_xAIx_DerParseFunctionsType, CRYPTO_xVIx_xAIx_CONST)
  Crypto_xVIx_xAIx_DerParseFunctionsOctetstring;

/** \brief The functions needed to parse the encoding of a "CharacterString".
 *
 * See X.690, chapter 8.21.
 */
extern CONST(Crypto_xVIx_xAIx_DerParseFunctionsType, CRYPTO_xVIx_xAIx_CONST)
  Crypto_xVIx_xAIx_DerParseFunctionsCharacterS;

/** \brief The functions needed to parse the encoding of an "integer".
 *
 * See X.690, chapter 8.3.
 */
extern CONST(Crypto_xVIx_xAIx_DerParseFunctionsType, CRYPTO_xVIx_xAIx_CONST)
  Crypto_xVIx_xAIx_DerParseFunctionsInteger;

/** \brief The functions needed to parse the encoding of an "object identifier".
 *
 * See X.690, chapter 8.19.
 */
extern CONST(Crypto_xVIx_xAIx_DerParseFunctionsType, CRYPTO_xVIx_xAIx_CONST)
  Crypto_xVIx_xAIx_DerParseFunctionsObjectIdentifier;

/** \brief The functions needed to parse the BCD encoding of a date.
 *
 */
extern CONST(Crypto_xVIx_xAIx_DerParseFunctionsType, CRYPTO_xVIx_xAIx_CONST)
  Crypto_xVIx_xAIx_DerParseFunctionsDate;

/** \brief A description of an "OBJECT IDENTIFIER" */
extern CONST(Crypto_xVIx_xAIx_DerComponentType, CRYPTO_xVIx_xAIx_CONST) Crypto_xVIx_xAIx_DerCptObjIdentifier;

/** \brief A description of a "OCTET STRING" */
extern CONST(Crypto_xVIx_xAIx_DerComponentType, CRYPTO_xVIx_xAIx_CONST) Crypto_xVIx_xAIx_DerCptOctetString;

#define CRYPTO_xVIx_xAIx_STOP_SEC_CONST_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Variables with external linkage]=============================================================*/

/*==[Declaration of functions with external linkage]==============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief Returns the contents of a subcomponent stored in a given component.
 *
 * Given a pointer to the start of the DER encoding of a component, and a
 * description of a subcomponent, this function returns the byte position of the
 * start of the contents of the subcomponent and it also returns the length of
 * the contents of the subcomponent.
 *
 * \param[in]  ComponentPtr A pointer to the description of the component which
 *                          has to be searched
 * \param[in]  Encoding A pointer to the start of the encoding
 * \param[in]  ComponentToFindPtr A pointer to the description of the
 *                                subcomponent to find
 * \param[out] ContentsLengthPtr A pointer to a variable where the length of
 *                               the contents of the sub component will be stored
 * \param[in]  StartSearch Subcomponents which start before this position in
 *                         the encoding are ignored in the search.
 *
 * \return the amount of bytes to skip in the encoding of the component
 *         to reach the start of the contents of the encoding of the first
 *         subcomponent whose start position fulfills the requirement
 *         "position >= StartSearch".
 */
extern FUNC(uint32, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerSubComponentContents
(
  P2CONST(Crypto_xVIx_xAIx_DerComponentType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ComponentPtr,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding,
  P2CONST(Crypto_xVIx_xAIx_DerSubComponentType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) SubComponentToFindPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ContentsLengthPtr,
  uint32 StartSearch
);

/** \brief Find the start of the contents of a given DER encoding of a value.
 *
 * See X.690, chapter 8.1.1
 *
 * \param[in]  Encoding A pointer to the start of the encoding of the value
 *
 * \return The byte position of the start of the contents in the encoding
 */
extern FUNC(uint32, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerEncGetContentsStart
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding
);

/** \brief Find the length of the contents of a given DER encoding of a value.
 *
 * See X.690, chapter 8.1.1
 *
 * Since the return value is a uint32 value, only lengths smaller than 2^32
 * can be handled.
 *
 * \param[in]  Encoding A pointer to the start of the encoding of the value
 *
 * \return The length of the contents of the encoding
 */
extern FUNC(uint32, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DerEncGetContentsLength
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Encoding
);

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #ifndef CRYPTO_XVIX_XAIX_AL_DER_H */

/*==[end of file]=================================================================================*/

