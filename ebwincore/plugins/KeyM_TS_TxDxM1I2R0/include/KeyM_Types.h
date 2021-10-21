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

#ifndef KEYM_TYPES_H
#define KEYM_TYPES_H

/*==================[deviations]==================================================================*/

/*==================[includes]====================================================================*/

/* !LINKSTO KeyM.Dsn.Files/KeyM_Types.h/0001,1 */

#include <Std_Types.h>
#include <KeyM_Version.h>

/*==================[macros]======================================================================*/

/* --- KeyM_CertificateStatusType --------------------------------------------------------------- */

/* !LINKSTO KeyM.Dsn.SWS_KeyM_91003,1 */
#if defined(KEYM_CERTIFICATE_VALID)
#error KEYM_CERTIFICATE_VALID is already defined
#endif
/** \brief  Certificate successfully parsed and verified. **/
#define KEYM_CERTIFICATE_VALID 0x00U

/* !LINKSTO KeyM.Dsn.SWS_KeyM_91003,1 */
#if defined(KEYM_CERTIFICATE_INVALID)
#error KEYM_CERTIFICATE_INVALID is already defined
#endif
/** \brief  The certificate is invalid (unspecified failure). **/
#define KEYM_CERTIFICATE_INVALID 0x01U

/* !LINKSTO KeyM.Dsn.SWS_KeyM_91003,1 */
#if defined(KEYM_CERTIFICATE_NOT_PARSED)
#error KEYM_CERTIFICATE_NOT_PARSED is already defined
#endif
/** \brief  Certificate has not been parsed so far. **/
#define KEYM_CERTIFICATE_NOT_PARSED 0x02U

/* !LINKSTO KeyM.Dsn.SWS_KeyM_91003,1 */
#if defined(KEYM_CERTIFICATE_PARSED_NOT_VALIDATED)
#error KEYM_CERTIFICATE_PARSED_NOT_VALIDATED is already defined
#endif
/** \brief  Certificate parsed but not yet validated. **/
#define KEYM_CERTIFICATE_PARSED_NOT_VALIDATED 0x03U

/* !LINKSTO KeyM.Dsn.SWS_KeyM_91003,1 */
#if defined(KEYM_CERTIFICATE_NOT_AVAILABLE)
#error KEYM_CERTIFICATE_NOT_AVAILABLE is already defined
#endif
/** \brief  Certificate not set. **/
#define KEYM_CERTIFICATE_NOT_AVAILABLE 0x04U

/* !LINKSTO KeyM.Dsn.SWS_KeyM_91003,1 */
#if defined(KEYM_E_CERTIFICATE_VALIDITY_PERIOD_FAIL)
#error KEYM_E_CERTIFICATE_VALIDITY_PERIOD_FAIL is already defined
#endif
/** \brief  Certificate verification failed - Invalid Time Period. **/
#define KEYM_E_CERTIFICATE_VALIDITY_PERIOD_FAIL 0x05U

/* !LINKSTO KeyM.Dsn.SWS_KeyM_91003,1 */
#if defined(KEYM_E_CERTIFICATE_SIGNATURE_FAIL)
#error KEYM_E_CERTIFICATE_SIGNATURE_FAIL is already defined
#endif
/** \brief  Certificate verification failed - Invalid Signature. **/
#define KEYM_E_CERTIFICATE_SIGNATURE_FAIL 0x06U

/* !LINKSTO KeyM.Dsn.SWS_KeyM_91003,1 */
#if defined(KEYM_E_CERTIFICATE_INVALID_CHAIN_OF_TRUST)
#error KEYM_E_CERTIFICATE_INVALID_CHAIN_OF_TRUST is already defined
#endif
/** \brief  Certificate verification failed - Invalid Chain of Trust. **/
#define KEYM_E_CERTIFICATE_INVALID_CHAIN_OF_TRUST 0x07U

/* !LINKSTO KeyM.Dsn.SWS_KeyM_91003,1 */
#if defined(KEYM_E_CERTIFICATE_INVALID_TYPE)
#error KEYM_E_CERTIFICATE_INVALID_TYPE is already defined
#endif
/** \brief  Certificate verification failed - Invalid Type. **/
#define KEYM_E_CERTIFICATE_INVALID_TYPE 0x08U

/* !LINKSTO KeyM.Dsn.SWS_KeyM_91003,1 */
#if defined(KEYM_E_CERTIFICATE_INVALID_FORMAT)
#error KEYM_E_CERTIFICATE_INVALID_FORMAT is already defined
#endif
/** \brief  Certificate verification failed - Invalid Format. **/
#define KEYM_E_CERTIFICATE_INVALID_FORMAT 0x09U

/* !LINKSTO KeyM.Dsn.SWS_KeyM_91003,1 */
#if defined(KEYM_E_CERTIFICATE_INVALID_CONTENT)
#error KEYM_E_CERTIFICATE_INVALID_CONTENT is already defined
#endif
/** \brief  Certificate verification failed - Invalid Content. **/
#define KEYM_E_CERTIFICATE_INVALID_CONTENT 0x0AU

/* !LINKSTO KeyM.Dsn.SWS_KeyM_91003,1 */
#if defined(KEYM_E_CERTIFICATE_REVOKED)
#error KEYM_E_CERTIFICATE_REVOKED is already defined
#endif
/** \brief  Certificate verification failed - Invalid Scope. **/
#define KEYM_E_CERTIFICATE_REVOKED 0x0BU

/* --- KeyM_ServiceCertificateType -------------------------------------------------------------- */

/* !LINKSTO KeyM.Dsn.SWS_KeyM_00039,1 */
#if defined(KEYM_SERVICE_CERT_REQUEST_CSR)
#error KEYM_SERVICE_CERT_REQUEST_CSR is already defined
#endif
/** \brief  Key server requests to generate a certificate from the key client. **/
#define KEYM_SERVICE_CERT_REQUEST_CSR 0x01U

/* !LINKSTO KeyM.Dsn.SWS_KeyM_00039,1 */
#if defined(KEYM_SERVICE_CERT_UPDATE_SIGNED_CSR)
#error KEYM_SERVICE_CERT_UPDATE_SIGNED_CSR is already defined
#endif
/** \brief  Key server returns a previously received certificate and has been now signed by the
 **         CA.
 **/
#define KEYM_SERVICE_CERT_UPDATE_SIGNED_CSR 0x02U

/* !LINKSTO KeyM.Dsn.SWS_KeyM_00039,1 */
#if defined(KEYM_SERVICE_CERT_SET_ROOT)
#error KEYM_SERVICE_CERT_SET_ROOT is already defined
#endif
/** \brief  Key server wants to add a new root certificate. **/
#define KEYM_SERVICE_CERT_SET_ROOT 0x03U

/* !LINKSTO KeyM.Dsn.SWS_KeyM_00039,1 */
#if defined(KEYM_SERVICE_CERT_UPDATE_ROOT)
#error KEYM_SERVICE_CERT_UPDATE_ROOT is already defined
#endif
/** \brief  Key server wants to update an existing root certificate. **/
#define KEYM_SERVICE_CERT_UPDATE_ROOT 0x04U

/* !LINKSTO KeyM.Dsn.SWS_KeyM_00039,1 */
#if defined(KEYM_SERVICE_CERT_SET_INTERMEDIATE)
#error KEYM_SERVICE_CERT_SET_INTERMEDIATE is already defined
#endif
/** \brief  Key server wants to add a new CA certificate. pre-requisite: Root certificate shall
 **         have been stored before a successful verification.
 **/
#define KEYM_SERVICE_CERT_SET_INTERMEDIATE 0x05U

/* !LINKSTO KeyM.Dsn.SWS_KeyM_00039,1 */
#if defined(KEYM_SERVICE_CERT_UPDATE_INTERMEDIATE)
#error KEYM_SERVICE_CERT_UPDATE_INTERMEDIATE is already defined
#endif
/** \brief  Key server wants to update an existing CA certificate. **/
#define KEYM_SERVICE_CERT_UPDATE_INTERMEDIATE 0x06U

/* !LINKSTO KeyM.Dsn.SWS_KeyM_00039,1 */
#if defined(KEYM_SERVICE_CERT_UPDATE_CRL)
#error KEYM_SERVICE_CERT_UPDATE_CRL is already defined
#endif
/** \brief  Provide or update a certificate revocation list. **/
#define KEYM_SERVICE_CERT_UPDATE_CRL 0x07U

/* --- KeyM_StartType --------------------------------------------------------------------------- */

/* !LINKSTO KeyM.Dsn.SWS_KeyM_00038,1 */
#if defined(KEYM_START_OEM_PRODUCTIONMODE)
#error KEYM_START_OEM_PRODUCTIONMODE is already defined
#endif
/** \brief  Key operation starts in OEM production mode. **/
#define KEYM_START_OEM_PRODUCTIONMODE 0x01U

/* !LINKSTO KeyM.Dsn.SWS_KeyM_00038,1 */
#if defined(KEYM_START_WORKSHOPMODE)
#error KEYM_START_WORKSHOPMODE is already defined
#endif
/** \brief  Key operation starts in workshop mode. **/
#define KEYM_START_WORKSHOPMODE 0x02U

/* --- KeyM_ResultType -------------------------------------------------------------------------- */

#if defined(KEYM_RT_OK)
#error KEYM_RT_OK is already defined
#endif
/** \brief  Key management operation successful. **/
#define KEYM_RT_OK 0x00U

#if defined(KEYM_RT_NOT_OK)
#error KEYM_RT_NOT_OK is already defined
#endif
/** \brief  General error occurred during key management operation. **/
#define KEYM_RT_NOT_OK 0x01U

#if defined(KEYM_RT_KEY_CERT_INVALID)
#error KEYM_RT_KEY_CERT_INVALID is already defined
#endif
/** \brief  Key or certificate is invalid and cannot be used for the operation. **/
#define KEYM_RT_KEY_CERT_INVALID 0x02U

#if defined(KEYM_RT_KEY_CERT_WRITE_FAIL)
#error KEYM_RT_KEY_CERT_WRITE_FAIL is already defined
#endif
/** \brief  Key or certificate could not be written to designated storage.. **/
#define KEYM_RT_KEY_CERT_WRITE_FAIL 0x03U

#if defined(KEYM_RT_KEY_CERT_UPDATE_FAIL)
#error KEYM_RT_KEY_CERT_UPDATE_FAIL is already defined
#endif
/** \brief  General failure while updating a key or certificate. **/
#define KEYM_RT_KEY_CERT_UPDATE_FAIL 0x04U

#if defined(KEYM_RT_CERT_INVALID_CHAIN_OF_TRUST)
#error KEYM_RT_CERT_INVALID_CHAIN_OF_TRUST is already defined
#endif
/** \brief  Certificate verification failed - Invalid Chain of Trust. **/
#define KEYM_RT_CERT_INVALID_CHAIN_OF_TRUST 0x05U

/* --- Std_ReturnType --------------------------------------------------------------------------- */

/* !LINKSTO SWS_KeyM_00040,1 */

#if (defined KEYM_E_BUSY)
#error KEYM_E_BUSY is already defined
#endif
/** \brief  Key management is busy with other operations. **/
#define KEYM_E_BUSY 0x02U

#if (defined KEYM_E_PENDING)
#error KEYM_E_PENDING is already defined
#endif
/** \brief  Operation request accepted, response is pending. It runs now in asynchronous mode,
 **         response will be given through callback.
 **/
#define KEYM_E_PENDING 0x03U

#if (defined KEYM_E_KEY_CERT_SIZE_MISMATCH)
#error KEYM_E_KEY_CERT_SIZE_MISMATCH is already defined
#endif
/** \brief  Parameter size does not match the expected value. **/
#define KEYM_E_KEY_CERT_SIZE_MISMATCH 0x04U

#if (defined KEYM_E_PARAMETER_MISMATCH)
#error KEYM_E_PARAMETER_MISMATCH is already defined
#endif
/** \brief  Parameter to function does not provide the expected value. **/
#define KEYM_E_PARAMETER_MISMATCH 0x05U

#if (defined KEYM_E_KEY_CERT_INVALID)
#error KEYM_E_KEY_CERT_INVALID is already defined
#endif
/** \brief  Key or certificate is invalid and cannot be used for the operation. **/
#define KEYM_E_KEY_CERT_INVALID 0x06U

#if (defined KEYM_E_KEY_CERT_READ_FAIL)
#error KEYM_E_KEY_CERT_READ_FAIL is already defined
#endif
/** \brief  Certificate or key could not be provided due to a read or permission failure. **/
#define KEYM_E_KEY_CERT_READ_FAIL 0x07U

#if (defined KEYM_E_KEY_CERT_EMPTY)
#error KEYM_E_KEY_CERT_EMPTY is already defined
#endif
/** \brief  The requested key or certificate is not available, slot is empty. **/
#define KEYM_E_KEY_CERT_EMPTY 0x08U

#if (defined KEYM_E_CERT_INVALID_CHAIN_OF_TRUST)
#error KEYM_E_CERT_INVALID_CHAIN_OF_TRUST is already defined
#endif
/** \brief  Certificate verification failed - Invalid Chain of Trust. **/
#define KEYM_E_CERT_INVALID_CHAIN_OF_TRUST 0x09U

/*==================[type definitions]============================================================*/

/** \brief  Enumeration of the result type of verification operations. **/
/* !LINKSTO KeyM.Dsn.SWS_KeyM_91003,1 */
#ifndef RTE_TYPE_KeyM_CertificateStatusType
#define RTE_TYPE_KeyM_CertificateStatusType
typedef uint8 KeyM_CertificateStatusType;
#endif /* #ifndef RTE_TYPE_KeyM_CertificateStatusType */

/** \brief  Certificate handle. **/
/* !LINKSTO KeyM.Dsn.SWS_KeyM_00301,1 */
#ifndef RTE_TYPE_KeyM_CertificateIdType
#define RTE_TYPE_KeyM_CertificateIdType
typedef uint16 KeyM_CertificateIdType;
#endif /* #ifndef RTE_TYPE_KeyM_CertificateIdType */

/** \brief  This type specifies the requested service operation and what information is
 **         provided with this function.
 **/
/* !LINKSTO KeyM.Dsn.SWS_KeyM_00039,1 */
#ifndef RTE_TYPE_KeyM_ServiceCertificateType
#define RTE_TYPE_KeyM_ServiceCertificateType
typedef uint8 KeyM_ServiceCertificateType;
#endif /* #ifndef RTE_TYPE_KeyM_ServiceCertificateType */

/** \brief  This type specifies in which mode the key operation will start. The OEM production
 **         mode provides higher privileges compared to workshop mode.
 **/
/* !LINKSTO KeyM.Dsn.SWS_KeyM_00038,1 */
#ifndef RTE_TYPE_KeyM_StartType
#define RTE_TYPE_KeyM_StartType
typedef uint8 KeyM_StartType;
#endif /* #ifndef RTE_TYPE_KeyM_StartType */

/** \brief  Certificate Element handle. **/
/* !LINKSTO KeyM.Dsn.SWS_KeyM_00300,1 */
#ifndef RTE_TYPE_KeyM_CertElementIdType
#define RTE_TYPE_KeyM_CertElementIdType
typedef uint16 KeyM_CertElementIdType;
#endif /* #ifndef RTE_TYPE_KeyM_CertElementIdType */

/** \brief  KeyM result type. **/
/* !LINKSTO KeyM.Dsn.SWS_KeyM_91008,1 */
#ifndef RTE_TYPE_KeyM_ResultType
#define RTE_TYPE_KeyM_ResultType
typedef uint8 KeyM_ResultType;
#endif /* #ifndef RTE_TYPE_KeyM_ResultType */

/** \brief  Type that is needed because the MMT tooling doesn't allow us to have a pointer
 **         directly inside a structure.
 **         https://jira.autosar.org/browse/AR-15290?focusedCommentId=475101&page=com.atlassian.jira.plugin.system.issuetabpanels:comment-
 **         tabpanel#comment-475101
 **/
#ifndef RTE_TYPE_KeyM_CertDataPointerType
#define RTE_TYPE_KeyM_CertDataPointerType
typedef uint8 *KeyM_CertDataPointerType;
#endif /* #ifndef RTE_TYPE_KeyM_CertDataPointerType */

/** \brief  This structure is used to exchange certificate data through interface functions. **/
/* !LINKSTO KeyM.Dsn.SWS_KeyM_00041,1 */
#ifndef RTE_TYPE_KeyM_CertDataType
#define RTE_TYPE_KeyM_CertDataType
typedef struct
{
  uint32 certDataLength;

  KeyM_CertDataPointerType certData;
}
KeyM_CertDataType;
#endif /* #ifndef RTE_TYPE_KeyM_CertDataType */

/** \brief  This structure is used to iterate through a number of elements of a certificate.
 **         This type is implementation specific.
 **/
/* !LINKSTO KeyM.Dsn.SWS_KeyM_00042,1 */
/* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGetFirstNext/00002,1 */
#ifndef RTE_TYPE_KeyM_CertElementIteratorType
#define RTE_TYPE_KeyM_CertElementIteratorType
typedef struct
{
  KeyM_CertificateIdType certId;
  KeyM_CertElementIdType certElementId;
  uint8  index;
}
KeyM_CertElementIteratorType;
#endif /* #ifndef RTE_TYPE_KeyM_CertElementIteratorType */

/** \brief  The type for initialization. **/
/* !LINKSTO KeyM.Dsn.SWS_KeyM_00157,1 */
#ifndef RTE_TYPE_KeyM_ConfigType
#define RTE_TYPE_KeyM_ConfigType
typedef struct
{
  uint8 dummy;
}
KeyM_ConfigType;
#endif /* #ifndef RTE_TYPE_KeyM_ConfigType */

/*==================[external constants]==========================================================*/

/*==================[external data]===============================================================*/

/*==================[external function declarations]==============================================*/

/*================================================================================================*/

#endif /* #ifndef KEYM_TYPES_H */

/*==================[end of file]=================================================================*/

