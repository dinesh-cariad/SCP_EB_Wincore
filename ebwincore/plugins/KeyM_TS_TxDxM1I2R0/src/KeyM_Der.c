/**
 * \file
 *
 * \brief AUTOSAR KeyM
 *
 * This file contains the implementation of the AUTOSAR
 * module KeyM.
 *
 * \version 1.2.11
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[deviations]==================================================================*/

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 20.7 (required)
 * Expressions resulting from the expansion of macro parameters shall be
 * enclosed in parentheses.
 *
 * Reason:
 * The AUTOSAR compiler abstraction requires these definitions in this way
 * and the arguments cannot be enclosed in parentheses due to C syntax.
 *
 */

/*==================[includes]====================================================================*/

#include <KeyM_Der.h>

/*==================[macros]======================================================================*/

#if (defined KEYM_ASN1_DER_TAG_BOOLEAN)
  #error KEYM_ASN1_DER_TAG_BOOLEAN is already defined
#endif
/** \brief  The DER tag of a boolean. **/
#define KEYM_ASN1_DER_TAG_BOOLEAN 0x01U

#if (defined KEYM_ASN1_DER_TAG_INTEGER)
#error KEYM_ASN1_DER_TAG_INTEGER is already defined
#endif
/** \brief  The DER tag of an integer. **/
#define KEYM_ASN1_DER_TAG_INTEGER 0x02U

#if (defined KEYM_ASN1_DER_TAG_OCTET_STRING)
#error KEYM_ASN1_DER_TAG_OCTET_STRING is already defined
#endif
/** \brief  The DER tag of an octet string. **/
#define KEYM_ASN1_DER_TAG_OCTET_STRING 0x04U

#if (defined KEYM_ASN1_DER_TAG_NULL)
#error KEYM_ASN1_DER_TAG_NULL is already defined
#endif
/** \brief  The DER tag of a null value. **/
#define KEYM_ASN1_DER_TAG_NULL 0x05U

#if (defined KEYM_ASN1_DER_TAG_OID)
#error KEYM_ASN1_DER_TAG_OID is already defined
#endif
/** \brief  The DER tag of an oid. **/
#define KEYM_ASN1_DER_TAG_OID 0x06U

#if (defined KEYM_ASN1_DER_TAG_UTF8STRING)
#error KEYM_ASN1_DER_TAG_UTF8STRING is already defined
#endif
/** \brief  The DER tag of an utf8 string. **/
#define KEYM_ASN1_DER_TAG_UTF8STRING 0x0CU

#if (defined KEYM_ASN1_DER_TAG_PRINTABLE_STRING)
#error KEYM_ASN1_DER_TAG_PRINTABLE_STRING is already defined
#endif
/** \brief  The DER tag of a printable string. **/
#define KEYM_ASN1_DER_TAG_PRINTABLE_STRING 0x13U

#if (defined KEYM_ASN1_DER_TAG_IA5STRING)
#error KEYM_ASN1_DER_TAG_IA5STRING is already defined
#endif
/** \brief  The DER tag of an ia5 string. **/
#define KEYM_ASN1_DER_TAG_IA5STRING 0x16U

#if (defined KEYM_ASN1_DER_TAG_UTCTIME)
#error KEYM_ASN1_DER_TAG_UTCTIME is already defined
#endif
/** \brief  The DER tag of an utcTime. **/
#define KEYM_ASN1_DER_TAG_UTCTIME 0x17U

#if (defined KEYM_ASN1_DER_TAG_GENERALIZEDTIME)
#error KEYM_ASN1_DER_TAG_GENERALIZEDTIME is already defined
#endif
/** \brief  The DER tag of a GeneralizedTime. **/
#define KEYM_ASN1_DER_TAG_GENERALIZEDTIME 0x18U

#if (defined KEYM_ASN1_DER_TAG_SEQUENCE)
#error KEYM_ASN1_DER_TAG_SEQUENCE is already defined
#endif
/** \brief  The DER tag of a sequence. **/
#define KEYM_ASN1_DER_TAG_SEQUENCE 0x30U

#if (defined KEYM_ASN1_DER_TAG_SET)
#error KEYM_ASN1_DER_TAG_SET is already defined
#endif
/** \brief  The DER tag of a set. **/
#define KEYM_ASN1_DER_TAG_SET 0x31U

#if (defined KEYM_ASN1_DER_LENGTH_FORM_MASK)
#error KEYM_ASN1_DER_LENGTH_FORM_MASK is already defined
#endif
/** \brief  The mask value to get the length information. **/
#define KEYM_ASN1_DER_LENGTH_FORM_MASK 0x80U

#if (defined KEYM_ASN1_DER_TAG_CLASS_MASK)
#error KEYM_ASN1_DER_TAG_CLASS_MASK is already defined
#endif
/** \brief  The mask value to get the class of a tag. **/
#define KEYM_ASN1_DER_TAG_CLASS_MASK 0xC0U

#if (defined KEYM_ASN1_DER_P_C_MASK)
#error KEYM_ASN1_DER_P_C_MASK is already defined
#endif
/** \brief  The mask to get the P/C. **/
#define KEYM_ASN1_DER_P_C_MASK 0x20U

/*==================[type definitions]============================================================*/

/*==================[internal function declarations]==============================================*/

/*==================[internal constants]==========================================================*/

/*==================[internal data]===============================================================*/

/*==================[external constants]==========================================================*/

/*==================[external data]===============================================================*/

/*==================[external function definitions]===============================================*/

#define KEYM_START_SEC_CODE
#include <KeyM_MemMap.h>

/* --- KeyM_DecodeDERLength --------------------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserDer_Len_001,1 */

FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_DecodeDERLength
(
  P2CONST(uint8,  AUTOMATIC, KEYM_APPL_DATA) encoding,
          uint32                             encodingLength,
    P2VAR(uint32, AUTOMATIC, KEYM_APPL_DATA) length,
    P2VAR(uint32, AUTOMATIC, KEYM_APPL_DATA) lengthLength
)
{
  KeyM_ParseResultType retVal = KEYM_PARSERESULT_INVALID_FORMAT;

  /* !LINKSTO EB_KeyM_ParserDer_Len_002,1 */
  if (0U == (KEYM_ASN1_DER_LENGTH_FORM_MASK & encoding[0U]))
  {
    /* short length form */
    *length       = (uint32) encoding[0U];
    *lengthLength = 1U;
    retVal        = KEYM_PARSERESULT_OK;
  }
  /* !LINKSTO EB_KeyM_ParserDer_Len_003,1 */
  else
  {
    /* long length form */
    *lengthLength = ((uint32) KEYM_ASN1_DER_LENGTH_FORM_MASK ^ (uint32) encoding[0U]);
    /* we are limited to the size of our return value */
    /* !LINKSTO EB_KeyM_ParserDer_Len_004,1 */
    if (sizeof(*length) >= *lengthLength)
    {
      /* check if the given buffer is big enough to avoid out of bounds access */
      if ((encodingLength - 1U) >= *lengthLength)
      {
        uint8 i;

        /* the length has to be encoded with the minimum number of bytes */
        /* !LINKSTO EB_KeyM_ParserDer_Len_005,1 */
        if (0U != encoding[1U])
        {
          *length = 0U;
          for (i = 0U; i < *lengthLength; i++)
          {
            *length = (*length << 8U) + encoding[1U + i];
          }
          (*lengthLength)++;
          retVal = KEYM_PARSERESULT_OK;
        }
      }
    }
  }
  return retVal;
}

/* --- KeyM_GetDERTagLengthValue ---------------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserDer_TLV_001,1 */

FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetDERTagLengthValue
(
  P2VAR(uint8,                                   AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                                              encodingLength,
  P2VAR(uint8,                                   AUTOMATIC, KEYM_APPL_DATA) tag,
  /* Deviation MISRAC2012-1 */
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) value,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) valueLength
)
{
  KeyM_ParseResultType retVal = KEYM_PARSERESULT_INVALID_FORMAT;
  uint32               length;
  uint32               position;

  /* we need at least tag, length, value */
  /* !LINKSTO EB_KeyM_ParserDer_TLV_002,1 */
  if (3U <= encodingLength)
  {
    /* !LINKSTO EB_KeyM_ParserDer_TLV_003,1 */
    if (KEYM_PARSERESULT_OK == KeyM_DecodeDERLength(&encoding[1U], (encodingLength - 1U), &length, &position))
    {
      /* !LINKSTO EB_KeyM_ParserDer_TLV_004,1 */
      if (encodingLength >= (1U + position + length))
      {
        *tag         = encoding[0U];
        *value       = &encoding[1U + position];
        *valueLength = length;
        retVal       = KEYM_PARSERESULT_OK;
      }
    }
  }
  return retVal;
}

/* --- KeyM_GetDERBoolean ----------------------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserDer_Bool_001,1 */

FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetDERBoolean
(
  P2VAR(uint8,   AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                              encodingLength,
  P2VAR(boolean, AUTOMATIC, KEYM_APPL_DATA) boolValue
)
{
  KeyM_ParseResultType retVal = KEYM_PARSERESULT_INVALID_FORMAT;
  uint32               length;
  uint32               position;

  /* we need at least tag, length, value */
  /* !LINKSTO EB_KeyM_ParserDer_Bool_002,1 */
  if (3U <= encodingLength)
  {
    /* !LINKSTO EB_KeyM_ParserDer_Bool_003,1 */
    if (KEYM_ASN1_DER_TAG_BOOLEAN == encoding[0U])
    {
      if (KEYM_PARSERESULT_OK == KeyM_DecodeDERLength(&encoding[1U], (encodingLength - 1U), &length, &position))
      {
        /* !LINKSTO EB_KeyM_ParserDer_Bool_004,1 */
        if (   (encodingLength >= (1U + position + length))
            && (1U == length)
           )
        {
          *boolValue = (0U != encoding[1U + position]);
          retVal     = KEYM_PARSERESULT_OK;
        }
      }
    }
  }
  return retVal;
}

/* --- KeyM_GetDERInteger ----------------------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserDer_Int_001,1 */

FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetDERInteger
(
  P2VAR(uint8,                                   AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                                              encodingLength,
  /* Deviation MISRAC2012-1 */
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) integer,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) integerLength
)
{
  KeyM_ParseResultType retVal = KEYM_PARSERESULT_INVALID_FORMAT;
  uint32               length;
  uint32               position;

  /* we need at least tag, length, value */
  /* !LINKSTO EB_KeyM_ParserDer_Int_002,1 */
  if (3U <= encodingLength)
  {
    /* !LINKSTO EB_KeyM_ParserDer_Int_003,1 */
    if (KEYM_ASN1_DER_TAG_INTEGER == encoding[0U])
    {
      if (KEYM_PARSERESULT_OK == KeyM_DecodeDERLength(&encoding[1U], (encodingLength - 1U), &length, &position))
      {
        /* !LINKSTO EB_KeyM_ParserDer_Int_004,1 */
        if (encodingLength >= (1U + position + length))
        {
          *integer       = &encoding[1U + position];
          *integerLength = length;
          retVal         = KEYM_PARSERESULT_OK;
        }
      }
    }
  }
  return retVal;
}

/* --- KeyM_GetDERExplicit ---------------------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserDer_Exp_001,1 */

FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetDERExplicit
(
  P2VAR(uint8,                                   AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                                              encodingLength,
        uint8                                                               tagNumber,
  /* Deviation MISRAC2012-1 */
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) explicit,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) explicitLength
)
{
  KeyM_ParseResultType retVal = KEYM_PARSERESULT_INVALID_FORMAT;
  uint32               length;
  uint32               position;

  /* we need at least tag, length */
  /* !LINKSTO EB_KeyM_ParserDer_Exp_002,1 */
  if (2U <= encodingLength)
  {
    /* maximum length of tag is lower 5 bit */
    if (0U == ((KEYM_ASN1_DER_TAG_CLASS_MASK | KEYM_ASN1_DER_P_C_MASK) & tagNumber))
    {
      if ((KEYM_ASN1_DER_TAG_EXPLICIT | tagNumber) == encoding[0U])
      {
        if (KEYM_PARSERESULT_OK == KeyM_DecodeDERLength(&encoding[1U], (encodingLength - 1U), &length, &position))
        {
          /* !LINKSTO EB_KeyM_ParserDer_Exp_003,1 */
          if (encodingLength >= (1U + position + length))
          {
            *explicit       = &encoding[1U + position];
            *explicitLength = length;
            retVal          = KEYM_PARSERESULT_OK;
          }
        }
      }
    }
  }
  return retVal;
}

/* --- KeyM_GetDERSequence ---------------------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserDer_Sequence_001,1 */

FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetDERSequence
(
  P2VAR(uint8,                                   AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                                              encodingLength,
  /* Deviation MISRAC2012-1 */
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) sequence,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) sequenceLength
)
{
  KeyM_ParseResultType retVal = KEYM_PARSERESULT_INVALID_FORMAT;
  uint32               length;
  uint32               position;

  /* we need at least tag, length, value */
  /* !LINKSTO EB_KeyM_ParserDer_Sequence_002,1 */
  if (3U <= encodingLength)
  {
    /* !LINKSTO EB_KeyM_ParserDer_Sequence_003,1 */
    if (KEYM_ASN1_DER_TAG_SEQUENCE == encoding[0U])
    {
      if (KEYM_PARSERESULT_OK == KeyM_DecodeDERLength(&encoding[1U], (encodingLength - 1U), &length, &position))
      {
        /* !LINKSTO EB_KeyM_ParserDer_Sequence_004,1 */
        if (encodingLength >= (1U + position + length))
        {
          *sequence       = &encoding[1U + position];
          *sequenceLength = length;
          retVal          = KEYM_PARSERESULT_OK;
        }
      }
    }
  }
  return retVal;
}

/* --- KeyM_GetDEROID --------------------------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserDer_OID_001,1 */

FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetDEROID
(
  P2VAR(uint8,                                   AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                                              encodingLength,
  /* Deviation MISRAC2012-1 */
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) oid,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) oidLength
)
{
  KeyM_ParseResultType retVal = KEYM_PARSERESULT_INVALID_FORMAT;
  uint32               length;
  uint32               position;

  /* we need at least tag, length, value */
  /* !LINKSTO EB_KeyM_ParserDer_OID_002,1 */
  if (3U <= encodingLength)
  {
    /* !LINKSTO EB_KeyM_ParserDer_OID_003,1 */
    if (KEYM_ASN1_DER_TAG_OID == encoding[0U])
    {
      if (KEYM_PARSERESULT_OK == KeyM_DecodeDERLength(&encoding[1U], (encodingLength - 1U), &length, &position))
      {
        /* !LINKSTO EB_KeyM_ParserDer_OID_004,1 */
        if (encodingLength >= (1U + position + length))
        {
          *oid       = &encoding[1U + position];
          *oidLength = length;
          retVal     = KEYM_PARSERESULT_OK;
        }
      }
    }
  }
  return retVal;
}

/* --- KeyM_CheckDERNULL ------------------------------------------------------------------------ */

/* !LINKSTO EB_KeyM_ParserDer_NULL_001,1 */

FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_CheckDERNULL
(
  P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                            encodingLength
)
{
  KeyM_ParseResultType retVal = KEYM_PARSERESULT_INVALID_FORMAT;

  /* !LINKSTO EB_KeyM_ParserDer_NULL_002,1 */
  if (2U <= encodingLength)
  {
    /* !LINKSTO EB_KeyM_ParserDer_NULL_003,1 */
    if (   (KEYM_ASN1_DER_TAG_NULL == encoding[0U])
        && (0x00U                  == encoding[1U])
       )
    {
      retVal = KEYM_PARSERESULT_OK;
    }
  }
  return retVal;
}

/* --- KeyM_GetDERSet --------------------------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserDer_Set_001,1 */

FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetDERSet
(
  P2VAR(uint8,                                   AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                                              encodingLength,
  /* Deviation MISRAC2012-1 */
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) set,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) setLength
)
{
  KeyM_ParseResultType retVal = KEYM_PARSERESULT_INVALID_FORMAT;
  uint32               length;
  uint32               position;

  /* we need at least tag, length, value */
  /* !LINKSTO EB_KeyM_ParserDer_Set_002,1 */
  if (3U <= encodingLength)
  {
    /* !LINKSTO EB_KeyM_ParserDer_Set_003,1 */
    if (KEYM_ASN1_DER_TAG_SET == encoding[0U])
    {
      if (KEYM_PARSERESULT_OK == KeyM_DecodeDERLength(&encoding[1U], (encodingLength - 1U), &length, &position))
      {
        /* !LINKSTO EB_KeyM_ParserDer_Set_004,1 */
        if (encodingLength >= (1U + position + length))
        {
          *set       = &encoding[1U + position];
          *setLength = length;
          retVal     = KEYM_PARSERESULT_OK;
        }
      }
    }
  }
  return retVal;
}

/* --- KeyM_GetDERString ------------------------------------------------------------------------ */

/* !LINKSTO EB_KeyM_ParserDer_String_001,1 */

FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetDERString
(
  P2VAR(uint8,                                   AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                                              encodingLength,
  /* Deviation MISRAC2012-1 */
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) string,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) stringLength
)
{
  KeyM_ParseResultType retVal = KEYM_PARSERESULT_INVALID_FORMAT;
  uint32               length;
  uint32               position;

  /* we need at least tag, length, value */
  /* !LINKSTO EB_KeyM_ParserDer_String_002,1 */
  if (3U <= encodingLength)
  {
    /* only Utf8String, PrintableString and IA5String are supported */
    /* !LINKSTO EB_KeyM_ParserDer_String_004,1 */
    /* !LINKSTO EB_KeyM_ParserDer_String_005,1 */
    /* !LINKSTO EB_KeyM_ParserDer_String_003,1 */
    if (   (KEYM_ASN1_DER_TAG_UTF8STRING       == encoding[0U])
        || (KEYM_ASN1_DER_TAG_PRINTABLE_STRING == encoding[0U])
        || (KEYM_ASN1_DER_TAG_IA5STRING        == encoding[0U])
       )
    {
      if (KEYM_PARSERESULT_OK == KeyM_DecodeDERLength(&encoding[1U], (encodingLength - 1U), &length, &position))
      {
        /* !LINKSTO EB_KeyM_ParserDer_String_006,1 */
        if (encodingLength >= (1U + position + length))
        {
          *string       = &encoding[1U + position];
          *stringLength = length;
          retVal        = KEYM_PARSERESULT_OK;
        }
      }
    }
  }
  return retVal;
}

/* --- KeyM_GetDERTime -------------------------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserDer_Time_001,1 */

FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetDERTime
(
  P2VAR(uint8,                                   AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                                              encodingLength,
  /* Deviation MISRAC2012-1 */
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) timeValue,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) timeLength
)
{
  KeyM_ParseResultType retVal = KEYM_PARSERESULT_INVALID_FORMAT;
  uint32               length;
  uint32               position;

  /* we need at least tag, length, value */
  /* !LINKSTO EB_KeyM_ParserDer_Time_002,1 */
  if (3U <= encodingLength)
  {
    /* !LINKSTO EB_KeyM_ParserDer_Time_003,1 */
    /* !LINKSTO EB_KeyM_ParserDer_Time_004,1 */
    if (   (KEYM_ASN1_DER_TAG_UTCTIME         == encoding[0U])
        || (KEYM_ASN1_DER_TAG_GENERALIZEDTIME == encoding[0U])
       )
    {
      if (KEYM_PARSERESULT_OK == KeyM_DecodeDERLength(&encoding[1U], (encodingLength - 1U), &length, &position))
      {
         /* !LINKSTO EB_KeyM_ParserDer_Time_005,1 */
        if (encodingLength >= (1U + position + length))
        {
          *timeValue  = &encoding[1U + position];
          *timeLength = length;
          retVal      = KEYM_PARSERESULT_OK;
        }
      }
    }
  }
  return retVal;
}

/* --- KeyM_GetDERBitString --------------------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserDer_BitString_001,1 */

FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetDERBitString
(
  P2VAR(uint8,                                   AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                                              encodingLength,
  /* Deviation MISRAC2012-1 */
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) bitString,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) bitStringLength
)
{
  KeyM_ParseResultType retVal = KEYM_PARSERESULT_INVALID_FORMAT;
  uint32               length;
  uint32               position;

  /* we need at least tag, length, value */
  /* !LINKSTO EB_KeyM_ParserDer_BitString_002,1 */
  if (3U <= encodingLength)
  {
    /* !LINKSTO EB_KeyM_ParserDer_BitString_003,1 */
    if (KEYM_ASN1_DER_TAG_BIT_STRING == encoding[0U])
    {
      if (KEYM_PARSERESULT_OK == KeyM_DecodeDERLength(&encoding[1U], (encodingLength - 1U), &length, &position))
      {
        /* !LINKSTO EB_KeyM_ParserDer_BitString_004,1 */
        if (encodingLength >= (1U + position + length))
        {
          /* only full byte BIT STRINGs are supported */
          /* !LINKSTO EB_KeyM_ParserDer_BitString_005,1 */
          if (0x00U == encoding[1U + position])
          {
            *bitString       = &encoding[2U + position];
            *bitStringLength = length - 1U;
            retVal           = KEYM_PARSERESULT_OK;
          }
        }
      }
    }
  }
  return retVal;
}

/* --- KeyM_GetDEROctetString ------------------------------------------------------------------- */

/* !LINKSTO EB_KeyM_ParserDer_OctetString_001,1 */

FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetDEROctetString
(
  P2VAR(uint8,                                   AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                                              encodingLength,
  /* Deviation MISRAC2012-1 */
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) octetString,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) octetStringLength
)
{
  KeyM_ParseResultType retVal = KEYM_PARSERESULT_INVALID_FORMAT;
  uint32               length;
  uint32               position;

  /* we need at least tag, length, value */
  /* !LINKSTO EB_KeyM_ParserDer_OctetString_002,1 */
  if (3U <= encodingLength)
  {
    /* !LINKSTO EB_KeyM_ParserDer_OctetString_003,1 */
    if (KEYM_ASN1_DER_TAG_OCTET_STRING == encoding[0U])
    {
      if (KEYM_PARSERESULT_OK == KeyM_DecodeDERLength(&encoding[1U], (encodingLength - 1U), &length, &position))
      {
        /* !LINKSTO EB_KeyM_ParserDer_OctetString_004,1 */
        if (encodingLength >= (1U + position + length))
        {
          *octetString       = &encoding[1U + position];
          *octetStringLength = length;
          retVal             = KEYM_PARSERESULT_OK;
        }
      }
    }
  }
  return retVal;
}
#define KEYM_STOP_SEC_CODE
#include <KeyM_MemMap.h>

/*==================[internal function definitions]===============================================*/

/*==================[end of file]=================================================================*/

