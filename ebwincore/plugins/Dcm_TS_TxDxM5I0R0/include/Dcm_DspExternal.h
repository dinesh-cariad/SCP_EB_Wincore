/**
 * \file
 *
 * \brief AUTOSAR Dcm
 *
 * This file contains the implementation of the AUTOSAR
 * module Dcm.
 *
 * \version 5.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef DCM_DSPEXTERNAL_H
#define DCM_DSPEXTERNAL_H

/*==================[inclusions]=================================================================*/

#include <Std_Types.h>         /* AUTOSAR standard types */

#include <Dcm.h>
#include <Dcm_SecurityAccess_Cfg.h>

#if (DCM_INCLUDE_RTE == STD_ON)
/* RTE_PTR2ARRAYBASETYPE_PASSING is a switch which activates some of options/definitions in RTE */
#if (defined RTE_PTR2ARRAYBASETYPE_PASSING)
  #error "RTE_PTR2ARRAYBASETYPE_PASSING is already defined"
#endif
#define RTE_PTR2ARRAYBASETYPE_PASSING
#include <Rte_Dcm.h>                                /* API generated by RTE */
#endif

/*==================[macros]=====================================================================*/

/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/

#define DCM_START_SEC_CALLOUT_CODE
#include <Dcm_MemMap.h>
#if(DCM_EXTSVC_ENABLED == STD_ON)
/** \brief Callout function to indicate the transmission of user implemented diagnostic service
 ** response
 **
 ** \param idContext Context identifier to retrieve the relation between request and confirmation
 ** \param dcmRxPduId Reception PDU Id of the diagnoctic service request
 ** \param status Result transmission of diagnostic response
 */
extern FUNC(void, DCM_APPL_CODE) Dcm_Confirmation(
  Dcm_IdContextType idContext, PduIdType dcmRxPduId, Dcm_ConfirmationStatusType status);
#endif /* #if(DCM_EXTSVC_ENABLED == STD_ON) */

/** \brief Request permission for changeing a session.
 **
 ** \param SesCtrlTypeActive Type of active session
 ** \param SesCtrlTypeNew Type of new session
 **
 ** \return Permission of application
 **         - ::E_OK: session can be started
 **         - ::DCM_E_SESSION_NOT_ALLOWED: session is not allowed to start
 **         - ::DCM_E_PENDING: don't send a reponse at all and
 **           start sending 'Response Pending' after P2Max. This return value
 **           may be used for realization of a bootloader sequence conforming
 **           to HIS specification.
 */
extern FUNC(Std_ReturnType, DCM_APPL_CODE) Rte_DcmGetSesChgPermission(
  Dcm_SesCtrlType SesCtrlTypeActive, Dcm_SesCtrlType SesCtrlTypeNew);


/** \brief Indication sent to Central Diagnostic SwC when a session is changed
 **
 ** \param SesCtrlTypeOld Type of old session
 ** \param SesCtrlTypeNew Type of new session
 **
 ** \return Change Indication Status
 **         - ::E_OK: Change Indication Success
 **         - ::E_NOT_OK: Change Indication Failed
 */
extern FUNC(Std_ReturnType, DCM_APPL_CODE) Rte_DcmSesCtrlChangeIndication(
  Dcm_SesCtrlType SesCtrlTypeOld, Dcm_SesCtrlType SesCtrlTypeNew);


/** \brief Request to application for provision of seed value
 **
 ** \param SecurityLevel Requested security level
 ** \param SeedLen Seed length
 ** \param Seed Pointer for provided seed.
 **
 ** \return Seed Request Status
 **         - ::E_OK: Seed Request Success
 **         - ::E_NOT_OK: Seed Request Failed
 **         - ::DCM_E_PENDING: application process not yet completed
 */
extern FUNC(Std_ReturnType, DCM_APPL_CODE) Rte_DcmGetSeed(
  Dcm_SecLevelType SecurityLevel, uint8 SeedLen, P2VAR(uint8, AUTOMATIC, DCM_VAR) Seed);


/** \brief Request to application for provision of seed value
 **
 ** \param SecurityLevel Requested security level

 ** \param KeyLen Key length
 ** \param Seed Pointer for provided seed.
 **
 ** \return Result of comparision:
 **         - ::E_OK: compare was successful
 **         - ::E_NOT_OK: CompareKey function failed
 **         - ::DCM_E_PENDING: application process not yet completed
 **         - ::DCM_E_COMPARE_KEY_FAILED: compare failed
 */
extern FUNC(Std_ReturnType, DCM_APPL_CODE) Rte_DcmCompareKey(
  Dcm_SecLevelType SecurityLevel,uint8 KeyLen, P2CONST(uint8, AUTOMATIC, DCM_APPL_CONST) Key);

#if (DCM_DSP_SECURITYACCESS_LEGACY_ATTEMPT_COUNTERS_HANDLING == STD_ON)

/** \brief Get number of failed attempts of a certain security level
 **
 ** The function Rte_DcmSetNumAtt() shall set the number of failed attempts for security access.The
 ** central diagnostic SwC has to store the number of failed attempts value into some kind of
 ** non-volatile memory.
 **
 ** \param SecurityLevel Requested security level
 **
 ** \return number of failed attempts
 */
extern FUNC(uint8, DCM_APPL_CODE) Rte_DcmSecGetNumAtt(Dcm_SecLevelType SecurityLevel);

/** \brief Set number of failed attempts of a certain security level
 **
 ** The function Rte_DcmSetNumAtt() shall set the number of failed attempts for security access.The
 ** central diagnostic SwC has to store the number of failed attempts value into some kind of
 ** non-volatile memory.
 **
 ** \param SecurityLevel Requested security level
 ** \param NumAtt Number of failed attempts
 */
extern FUNC(void, DCM_APPL_CODE) Rte_DcmSecSetNumAtt(Dcm_SecLevelType SecurityLevel, uint8 NumAtt);

#endif /* #if (DCM_DSP_SECURITYACCESS_LEGACY_ATTEMPT_COUNTERS_HANDLING == STD_ON) */

/** \brief Manfacturer notification function
 **
 ** The purpose of this functionality is that, just after receiving the diagnostic request, the
 ** Manufacturer Application is requested to check permission/environment.
 ** E.g. in after-run ECU state, it might be not allowed to process OBD requests.
 **
 ** \param Sid           : Value of service identifier
 ** \param RequestData   : This parameter contains the complete request data (diagnostic buffer),
 **                        except the service ID.
 ** \param DataSize      : This parameter defines how many bytes in the RequestData
 **                        parameter are valid
 ** \param ReqType       : Addressing type of the request(0=physical request
                           1=functional request)
 ** \param SourceAddress : Tester source address configured per protocol
 ** \param ErrorCode     : Error code set from application
 **
 ** \return Result of comparision:
 **         - ::E_OK: Operation successful
 **         - ::E_NOT_OK: Operation not successful
 **         - ::DCM_E_REQUEST_NOT_ACCEPTED: Request not accepted
 */
extern FUNC(Std_ReturnType, DCM_APPL_CODE) Rte_DcmManufacturerNotification(
  uint8 Sid,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) RequestData,
  uint16 DataSize,
  uint8 ReqType,
  uint16 SourceAddress,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Supplier notification function
 **
 ** The purpose of this functionality is that, right before processing the diagnostic
 ** message, the Supplier Application is requested to check permission/environment.
 ** E.g. in after-run ECU state, it might be not allowed to process OBD requests.
 **
 ** \param Sid           : Value of service identifier
 ** \param RequestData   : This parameter contains the complete request data (diagnostic buffer),
 **                        except the service ID.
 ** \param DataSize      : This parameter defines how many bytes in the RequestData
 **                        parameter are valid
 ** \param ReqType       : Addressing type of the request(0=physical request
                           1=functional request)
 ** \param SourceAddress : Tester source address configured per protocol
 ** \param ErrorCode     : Error code set from application
 **
 ** \return Result of comparision:
 **         - ::E_OK: Operation successful
 **         - ::E_NOT_OK: Operation not successful
 **         - ::DCM_E_REQUEST_NOT_ACCEPTED: Request not accepted
 */
extern FUNC(Std_ReturnType, DCM_APPL_CODE) Rte_DcmSupplierNotification(
  uint8 Sid,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) RequestData,
  uint16 DataSize,
  uint8 ReqType,
  uint16 SourceAddress,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Confirmation function
 **
 ** Confirmation to be provided to the user when RTE is OFF.
 **
 ** \param SID Value of service identifier
 ** \param ReqType Address type from RxPdu
 ** \param SourceAddress Tester source address
 ** \param ConfirmationStatus status of the confirmation
 **
 ** \return None
 */

extern FUNC(void, DCM_APPL_CODE) Rte_DcmConfirmation(
  uint8 SID,
  uint8 ReqType,
  uint16 SourceAddress,
  Dcm_ConfirmationStatusType ConfirmationStatus);

#define DCM_STOP_SEC_CALLOUT_CODE
#include <Dcm_MemMap.h>


/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /* ifndef DCM_DSPEXTERNAL_H */
/*==================[end of file]================================================================*/
