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

#ifndef KEYM_X509_H
#define KEYM_X509_H

/*==================[deviations]==================================================================*/

/*==================[includes]====================================================================*/

#include <Std_Types.h> /* AUTOSAR standard types */
#include <TSAutosar.h> /* EB specific standard types */
#include <KeyM_Int_Types.h>

/*==================[macros]======================================================================*/

/*==================[type definitions]============================================================*/

/** \brief  Typedef to store the x509 integer type. **/
typedef struct
{
  uint32 length;

  P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) integer;
}
KeyM_X509INTEGERType;

/** \brief  Typedef to store the x509 oid type. **/
typedef struct
{
  uint32 length;

  P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) oid;
}
KeyM_X509OIDType;

/** \brief  Typedef to store the x509 algorithmIdentifier type. **/
typedef struct
{
  uint32 length;

  P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) algorithmIdentifier;

  KeyM_X509OIDType algorithm;
}
KeyM_X509AlgorithmIdentifierType;

/** \brief  Typedef to store the x509 attributeTypeAndValue type. **/
typedef struct
{
  uint32 length;
  P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) value;

  uint32 oidLength;
  P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) oid;

  uint8 tag;
}
KeyM_X509AttributeTypeAndValueType;

/** \brief  Typedef to store the x509 time type. **/
typedef struct
{
  uint32 length;

  P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) time;
}
KeyM_X509TimeType;

/** \brief  Typedef to store the x509 validity type. **/
typedef struct
{
  KeyM_X509TimeType notBefore;
  KeyM_X509TimeType notAfter;
}
KeyM_X509ValidityType;

/** \brief  Typedef to store the x509 bit string type. **/
typedef struct
{
  uint32 length;

  P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) bitString;
}
KeyM_X509BITSTRINGType;

/** \brief  Typedef to store the x509 subjectPublicKeyInfo type. **/
typedef struct
{
  KeyM_X509AlgorithmIdentifierType algorithm;
  KeyM_X509BITSTRINGType           subjectPublicKey;
}
KeyM_X509SubjectPublicKeyInfoType;

/** \brief  Typedef to store the full x509 tbsCertificate type. **/
typedef struct
{
  uint32 length;

  P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) data;

  KeyM_X509INTEGERType              version;
  KeyM_X509INTEGERType              serialNumber;
  KeyM_X509AlgorithmIdentifierType  signature;
  KeyM_X509ValidityType             validity;
  KeyM_X509SubjectPublicKeyInfoType subjectPublicKeyInfo;
  KeyM_X509BITSTRINGType            issuerUniqueID; /* these types should be uniqueIdentifier and then bitstring to be consistent */
  KeyM_X509BITSTRINGType            subjectUniqueID;
  KeyM_X509BITSTRINGType            extensions;
}
KeyM_X509FullTBSCertificateType;

/** \brief  Typedef to store the full x509 certificate type. **/
typedef struct
{
  KeyM_X509FullTBSCertificateType  tbsCertificate;
  KeyM_X509AlgorithmIdentifierType signatureAlgorithm;
  KeyM_X509BITSTRINGType           signatureValue;
}
KeyM_X509FullCertificateType;

/*==================[external constants]==========================================================*/

/*==================[external data]===============================================================*/

/*==================[external function declarations]==============================================*/

#define KEYM_START_SEC_CODE
#include <KeyM_MemMap.h>

/** \brief  Function to parse a DER encoded x509 certificate. **/
extern FUNC(KeyM_ParseResultType, KEYM_CODE) KeyM_GetX509Certificate
(
         P2VAR(uint8,                    AUTOMATIC, KEYM_APPL_DATA) encoding,
               uint32                                               encodingLength,
  CONSTP2CONST(KeyM_CertificateCnfgType, AUTOMATIC, KEYM_CONST)     certificateCnfg
);

#define KEYM_STOP_SEC_CODE
#include <KeyM_MemMap.h>

/*================================================================================================*/

#endif /* #ifndef KEYM_X509_H */

/*==================[end of file]=================================================================*/

