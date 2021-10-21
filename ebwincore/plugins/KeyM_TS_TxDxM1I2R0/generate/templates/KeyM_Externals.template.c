[!/* ====================================================================================== */!][!//
[!INCLUDE "include/KeyM_Macros.m"!][!//
[!AUTOSPACING!][!//
[!/* ====================================================================================== */!][!//
[!//
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

/*==================[misra deviations]============================================================*/

/*==================[includes]====================================================================*/

#include <KeyM.h>

/*==================[macros]======================================================================*/

/*==================[type definitions]============================================================*/

/*==================[internal function declarations]==============================================*/

/*==================[internal constants]==========================================================*/

/*==================[internal data]===============================================================*/

/*==================[external constants]==========================================================*/

/*==================[external data]===============================================================*/

/*==================[external function definitions]===============================================*/

#define KEYM_START_SEC_CODE
#include <KeyM_MemMap.h>

[!IF "('true' = node:value(./KeyMGeneral/KeyMCertificateManagerEnabled)) and ('true' = node:exists(./KeyMGeneral/KeyMServiceCertificateFunctionEnabled)) and ('true' = node:value(./KeyMGeneral/KeyMServiceCertificateFunctionEnabled)) and (0 < count(./KeyMCertificate/*/KeyMServiceCertificateCallbackNotificationFunc))"!]
/* --- KeyM_ServiceCertificateCallbackNotification -----------------------------------------------*/

[!LOOP "node:order(KeyMCertificate/*/KeyMServiceCertificateCallbackNotificationFunc, 'node:value(.)')"!]
[!IF "not(var:defined(node:value(.)))"!]
[!IF "var:set(node:value(.), '')"!][!ENDIF!]
[!VAR "CBKNAME" = "node:value(.)"!]
/** \brief  This callback function is called to notify the application that the certificate service
 **         operation of certificate(s)
[!LOOP "../../../KeyMCertificate/*[./KeyMServiceCertificateCallbackNotificationFunc = $CBKNAME]"!]
 **          - [!"node:pos(.)"!]/[!"node:name(.)"!] ... [!"node:value(./KeyMCertificateId)"!]/[!"node:value(./KeyMCertificateName)"!]
[!ENDLOOP!]
 **         has been finished.
 **
 ** \param[in]  CertId            The certificate identifier where this service was started for.
 ** \param[in]  Result            Contains information about the result of the operation.
 ** \param[in]  ResultDataLength  Contains the length of the resulting data of this operation if any.
 ** \param[in]  ResultDataPtr     Pointer to the data of the result.
 **/
FUNC(void, KEYM_CODE) [!"node:value(.)"!]
(
        KeyM_CertificateIdType                             CertId,
        KeyM_ResultType                                    Result,
        uint16                                             ResultDataLength,
  P2VAR(uint8,                  AUTOMATIC, KEYM_APPL_DATA) ResultDataPtr
)
{
  /* empty */
}

[!ENDIF!]
[!ENDLOOP!]
[!ENDIF!]
[!IF "('true' = node:value(./KeyMGeneral/KeyMCertificateManagerEnabled)) and (0 < count(./KeyMCertificate/*/KeyMCertificateVerifyCallbackNotificationFunc))"!]
/* --- KeyM_CertificateVerifyCallbackNotification ------------------------------------------------*/

[!LOOP "node:order(KeyMCertificate/*/KeyMCertificateVerifyCallbackNotificationFunc, 'node:value(.)')"!]
[!IF "not(var:defined(node:value(.)))"!]
[!IF "var:set(node:value(.), '')"!][!ENDIF!]
[!VAR "CBKNAME" = "node:value(.)"!]
/** \brief  This callback function is called to notify the application that the verification of
 **         certificate(s)
[!LOOP "../../../KeyMCertificate/*[./KeyMCertificateVerifyCallbackNotificationFunc = $CBKNAME]"!]
 **          - [!"node:pos(.)"!]/[!"node:name(.)"!] ... [!"node:value(./KeyMCertificateId)"!]/[!"node:value(./KeyMCertificateName)"!]
[!ENDLOOP!]
 **         has been finished.
 **
 ** \param[in]  CertId  The certificate identifier that has been verified.
 ** \param[in]  Result  Contains information about the result of the operation.
 **
 ** \returns  Error value.
 **
 ** \retval   E_OK
 **/
FUNC(Std_ReturnType, KEYM_CODE) [!"node:value(.)"!]
(
  KeyM_CertificateIdType     CertId,
  KeyM_CertificateStatusType Result
)
{
  return E_OK;
}

[!ENDIF!]
[!ENDLOOP!]
[!ENDIF!]
#define KEYM_STOP_SEC_CODE
#include <KeyM_MemMap.h>

/*==================[internal function definitions]===============================================*/

/*==================[end of file]=================================================================*/

