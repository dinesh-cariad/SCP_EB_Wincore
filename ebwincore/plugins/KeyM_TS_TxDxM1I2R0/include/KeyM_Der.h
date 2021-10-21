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

#ifndef KEYM_DER_H
#define KEYM_DER_H

/*==================[deviations]==================================================================*/

/*==================[includes]====================================================================*/

#include <Std_Types.h> /* AUTOSAR standard types */
#include <TSAutosar.h> /* EB specific standard types */
#include <KeyM_Int_Types.h>

/*==================[macros]======================================================================*/

#if (defined KEYM_ASN1_DER_TAG_EXPLICIT)
#error KEYM_ASN1_DER_TAG_EXPLICIT is already defined
#endif
/** \brief  The DER tag of an explicit encoded value. **/
#define KEYM_ASN1_DER_TAG_EXPLICIT 0xA0U

#if (defined KEYM_ASN1_DER_TAG_BIT_STRING)
#error KEYM_ASN1_DER_TAG_BIT_STRING is already defined
#endif
/** \brief  The DER tag of a bit string. **/
#define KEYM_ASN1_DER_TAG_BIT_STRING 0x03U

/*==================[type definitions]============================================================*/

/*==================[external constants]==========================================================*/

/*==================[external data]===============================================================*/

/*==================[external function declarations]==============================================*/

#define KEYM_START_SEC_CODE
#include <KeyM_MemMap.h>

/** \brief  Function to decode a DER length. **/
extern FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_DecodeDERLength
(
  P2CONST(uint8,  AUTOMATIC, KEYM_APPL_DATA) encoding,
          uint32                             encodingLength,
    P2VAR(uint32, AUTOMATIC, KEYM_APPL_DATA) length,
    P2VAR(uint32, AUTOMATIC, KEYM_APPL_DATA) lengthLength
);

/** \brief  Function to get the tag, length and value of a DER element. **/
extern FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetDERTagLengthValue
(
  P2VAR(uint8,                                   AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                                              encodingLength,
  P2VAR(uint8,                                   AUTOMATIC, KEYM_APPL_DATA) tag,
  /* Deviation MISRAC2012-1 */
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) value,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) valueLength
);

/** \brief  Function to decode a DER boolean. **/
extern FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetDERBoolean
(
  P2VAR(uint8,   AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                              encodingLength,
  P2VAR(boolean, AUTOMATIC, KEYM_APPL_DATA) boolValue
);

/** \brief  Function to decode a DER integer. **/
extern FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetDERInteger
(
  P2VAR(uint8,                                   AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                                              encodingLength,
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) integer,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) integerLength
);

/** \brief  Function to decode a DER explicit. **/
extern FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetDERExplicit
(
  P2VAR(uint8,                                   AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                                              encodingLength,
        uint8                                                               tagNumber,
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) explicit,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) explicitLength
);

/** \brief  Function to decode a DER sequence. **/
extern FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetDERSequence
(
  P2VAR(uint8,                                   AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                                              encodingLength,
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) sequence,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) sequenceLength
);

/** \brief  Function to decode DER oid. **/
extern FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetDEROID
(
  P2VAR(uint8,                                   AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                                              encodingLength,
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) oid,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) oidLength
);

/** \brief  Function to check if NULL is correctly encoded in DER. **/
extern FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_CheckDERNULL
(
  P2VAR(uint8,                                   AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                                              encodingLength
);

/** \brief  Function to decode a DER set. **/
extern FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetDERSet
(
  P2VAR(uint8,                                   AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                                              encodingLength,
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) set,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) setLength
);

/** \brief  Function to decode a DER string. **/
extern FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetDERString
(
  P2VAR(uint8,                                   AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                                              encodingLength,
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) string,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) stringLength
);

/** \brief  Function to decode a DER utcTime. **/
extern FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetDERTime
(
  P2VAR(uint8,                                   AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                                              encodingLength,
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) timeValue,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) timeLength
);

/** \brief  Function to decode a DER bitString. **/
extern FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetDERBitString
(
  P2VAR(uint8,                                   AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                                              encodingLength,
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) bitString,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) bitStringLength
);

/** \brief  Function to decode a DER octetString. **/
extern FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetDEROctetString
(
  P2VAR(uint8,                                   AUTOMATIC, KEYM_APPL_DATA) encoding,
        uint32                                                              encodingLength,
  P2VAR(P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA), AUTOMATIC, KEYM_APPL_DATA) octetString,
  P2VAR(uint32,                                  AUTOMATIC, KEYM_APPL_DATA) octetStringLength
);

#define KEYM_STOP_SEC_CODE
#include <KeyM_MemMap.h>

/*================================================================================================*/

#endif /* #ifndef KEYM_DER_H */

/*==================[end of file]=================================================================*/

