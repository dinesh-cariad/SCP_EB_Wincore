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
/* !LINKSTO Dcm.Dsn.File.ApplicationDiagnosticRequests.PublicApi,1 */
/*==================[inclusions]=================================================================*/

#include <Std_Types.h>                                                 /* AUTOSAR standard types */
#include <Dcm_Int.h>
#include <ComStack_Types.h>

/*==================[macros]=====================================================================*/

/*==================[type declarations]===========================================================*/

/** \brief Structure contains relevant information for the completion of an application injection:
*
* - Dcm_BufferType RxBuffer: this structure on itself contains a pointer to the buffer to be
*                            injected and the size of the data to be injected.
* - Dcm_RequestTypeType RequestType: This field contains information about the type of request
*                                    (Physical or Functional), relevant for the setup of the
*                                    communication.
* - PduIdType InjectionRxPduId: This field contains the RxPduId used to perform the injection of
*                               the request.
*
* */
typedef struct
{
  /** \brief buffer handle used for the Injected Request */
  Dcm_BufferType  RxBuffer;

  /** \brief Size of original request */
  PduLengthType OriginalRequestSize;

  /** \brief PduId used for the Application Injected request */
  PduIdType InjectionRxPduId;

  /** \brief request type for supervisor */
  Dcm_RequestTypeType RequestType;

} Dcm_InjectedRxDataType;

/*==================[external function declarations]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief Initialization function for the Application Diagnostic Requests Software Unit data structures.
 *         This function clears the re-attempt flags for this SW unit, and also the RxPduId information
 *         and the internal buffer containing the request to be injected.
 *
 *  \param none.
 *  \return none
 */
extern FUNC(void, DCM_CODE) Dcm_Dsl_ApplicationDiagnosticRequests_Init(void);

/** \brief Main function of Application Diagnostic Requests Software Unit.
 *         This function checks if an Application request is pending, and it will attempt to dispatch it.
 *
 *  \param none.
 *  \return none.
 */
extern FUNC(void, DCM_CODE) Dcm_Dsl_ApplicationDiagnosticRequests_MainFunction(void);

/** \brief Injector Interface function to CDD
 *         This function collects the relevant information to execute the Application Diagnostic request.
 *
 *  \param[in] RxPduID: RxPduId to be used for the request to be injected.
 *  \param[in] RequestData: Pointer to array containing the request to be injected.
 *  \param[in] RequestLength: Lenght of the array containing the request to be injected.
 *
 *  \return BufReq_ReturnType Result of the injection request:
 *  \retval BUFREQ_OK               Injection Sucessful. Request will be processed.
 *  \retval BUFREQ_E_NOT_OK         Injection failed. Request will not be processed.
 *  \retval BUFREQ_E_BUSY           Injection already queued. No furhter injection can be accepted.
 *  \retval BUFREQ_E_OVFL           Injection failed. Request cannot be handled by DCM available buffers.
 */
extern FUNC(BufReq_ReturnType, DCM_CODE) Dcm_Dsl_ApplicationDiagnosticRequests_Injector
(
  PduIdType RxPduID,
  P2CONST(uint8, AUTOMATIC, DCM_VAR) RequestData,
  PduLengthType RequestLength
);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[end of file]================================================================*/
