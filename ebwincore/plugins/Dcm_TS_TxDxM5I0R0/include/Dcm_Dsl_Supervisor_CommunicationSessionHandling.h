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

/* !LINKSTO Dcm.Dsn.File.Supervisor.CommunicationSessionHandling.PublicApi,1 */
/* This file contains the public API of the Supervisor CommunicationSessionHandling software unit. */

#ifndef DCM_DSL_SUPERVISOR_COMMUNICATIONSESSIONHANDLING_H
#define DCM_DSL_SUPERVISOR_COMMUNICATIONSESSIONHANDLING_H
/*==================[inclusions]=================================================================*/

#include <Dcm_Dsl_CommunicationServices_BufferManager.h>
#include <Dcm_Dsl_CommunicationServices_RxConnections.h>

/*==================[macros]=====================================================================*/

/*==================[type definitions]===========================================================*/

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <MemMap.h>

extern FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_CommunicationSessionHandling_Init(
  void
);

extern FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_CommunicationSessionHandling_MainFunction(
  void
);

extern FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_CommunicationSessionHandling_DefaultSessionEntered(
  void
);

extern FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_CommunicationSessionHandling_PrepareApplicationDiagnosticRequest(
  Dcm_BufferUserType RxConnectionId
);

extern FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_CommunicationSessionHandling_ReceptionBegin(
  Dcm_BufferUserType RxConnectionId
);

extern FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_CommunicationSessionHandling_ReceptionEnd(
  Dcm_BufferUserType RxConnectionId
);

extern FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_CommunicationSessionHandling_ProcessingBegin(
  Dcm_BufferUserType RxConnectionId,
  boolean SendingBusyRepeatRequest
);

extern FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_CommunicationSessionHandling_ProcessingEnd(
  Dcm_BufferUserType RxConnectionId
);

extern FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_CommunicationSessionHandling_EndOfCommunicationSession(
  Dcm_BufferUserType RxConnectionId
);

extern FUNC (void, DCM_CODE)
  Dcm_Dsl_Supervisor_CommunicationSessionHandling_EnqueueCancellationOfRxConnection(
    Dcm_RxConnectionIndexType RxConnectionIndex
);

extern FUNC (boolean, DCM_CODE)
  Dcm_Dsl_Supervisor_CommunicationSessionHandling_IsCancellationEnqueuedForRxConnection(
    Dcm_RxConnectionIndexType RxConnectionIndex
);

#define DCM_STOP_SEC_CODE
#include <MemMap.h>

/*==================[internal function definitions]==============================================*/

#endif /* ifndef DCM_DSL_SUPERVISOR_COMMUNICATIONSESSIONHANDLING_H */
/*==================[end of file]================================================================*/
