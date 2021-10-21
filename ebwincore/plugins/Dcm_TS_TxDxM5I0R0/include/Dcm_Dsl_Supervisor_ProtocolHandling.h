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

/* !LINKSTO Dcm.Dsn.File.Supervisor.ProtocolHandling.PublicApi,1 */
/* This file contains the public API of the Supervisor ProtocolHandling software unit. */

#ifndef DCM_DSL_SUPERVISOR_PROTOCOLHANDLING_H
#define DCM_DSL_SUPERVISOR_PROTOCOLHANDLING_H

/*==================[inclusions]=================================================================*/

#include <Dcm_Dsl_Supervisor.h>
#include <Dcm_Dsl_CommunicationServices.h>
#include <Dcm_Dsl_CommunicationServices_BufferManager.h>
#include <Dcm_Dsl_CommunicationServices_RxConnections.h>

#if (DCM_META_DATA_HANDLING_ENABLED == STD_ON)
#include <Dcm_Dsl_CommunicationServices_MetaData.h>       /* Dcm Dsl MetaData header file */
#endif /* #if (DCM_META_DATA_HANDLING_ENABLED == STD_ON) */
#if (DCM_DEV_ERROR_DETECT == STD_ON)
/* Development Error Tracer (DCM_DET_REPORT_ERROR() macros) */
#include <Det.h>
#endif
#if ( (DCM_DSP_USE_SERVICE_0X01 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X02 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X03 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X06 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X07 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X08 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X0A == STD_ON) )
#include <Dcm_Dsp_DemFacade.h>
#endif
#if (DCM_DSP_USE_SERVICE_0X04 == STD_ON)
#include <Dcm_Int.h>
#endif
/*==================[macros]=====================================================================*/

/** @defgroup Macro_Dcm_SessionSwitchPermissionType Macros for
 **           Dcm_SessionSwitchPermissionType
 ** \brief    These macros are used as values for Dcm_SessionSwitchPermissionType
 **  @{
 */
/** \brief Invalid generic value */
#if (defined DCM_SUPERVISOR_SESSION_SWITCH_INVALID)
  #error "DCM_SUPERVISOR_SESSION_SWITCH_INVALID is already defined"
#endif
#define DCM_SUPERVISOR_SESSION_SWITCH_INVALID 0x00U

/** \brief Protocol stack busy, retry session switch later */
#if (defined DCM_SUPERVISOR_SESSION_SWITCH_BUSY_RETRY)
  #error "DCM_SUPERVISOR_SESSION_SWITCH_BUSY_RETRY is already defined"
#endif
#define DCM_SUPERVISOR_SESSION_SWITCH_BUSY_RETRY 0x01U

/** \brief Protocol stack busy, discard session switch (this is because the request
 ** is to switch to a non-default session and an OBD protocol is either running
 ** or is presumptive. */
#if (defined DCM_SUPERVISOR_SESSION_SWITCH_BUSY_DISCARD)
  #error "DCM_SUPERVISOR_SESSION_SWITCH_BUSY_DISCARD is already defined"
#endif
#define DCM_SUPERVISOR_SESSION_SWITCH_BUSY_DISCARD 0x02U

/** \brief Accept session switch. */
#if (defined DCM_SUPERVISOR_SESSION_SWITCH_ACCEPT)
  #error "DCM_SUPERVISOR_SESSION_SWITCH_ACCEPT is already defined"
#endif
#define DCM_SUPERVISOR_SESSION_SWITCH_ACCEPT 0x03U
/* @} */
/* End of macros definitions for Dcm_SessionSwitchPermissionType */

/*==================[type definitions]===========================================================*/

typedef uint8 Dcm_SessionSwitchPermissionType;

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <MemMap.h>

/* ProtocolHandling API */

/** \brief Initializes the global variables of the unit.
 */
extern FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_Init(
  void
);

/** \brief Stops protocols that are in state to be stopped, starts the one that needs to be started
 **        and dispaches the waiting requests (TYPE1 or TYPE2) if any.
 */
extern FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_MainFunction(
  void
);

/** \brief Returns a pointer to the configuration of the last served UDS protocol
 **
 ** \param[out] CurrentProtocolConfig  Pointer of type Dcm_ProtocolConfigPtrType which will be set
 **                                    to the descriptor of last used UDS protocol.
 **
 ** \return Success of the operation
 **         - ::DCM_E_OK:     The pointer was updated to point to the descriptor of the last used
 **                           UDS protocol.
 **         - ::DCM_E_NOT_OK: The last used UDS protocol index was invalid and the pointer was not1
 **                           updated.
 **
 */
extern FUNC (Std_ReturnType, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_GetLatestExternalUDSProtocolConfig(
  Dcm_ProtocolConfigPtrType CurrentProtocolConfig
);

/** \brief Returns whether a preemption is necessary to serve the incoming request on
 **        NextProtocolIndex
 **
 ** \param[in] NextProtocolIndex  The index of the protocol where the new request was received.
 **
 ** \return
 **         - ::TRUE:  Preemption is necessary to serve the new request
 **         - ::FALSE: Preemption is not necessary to serve the new request
 **
 */
extern FUNC (boolean, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_PreemptionNecessaryOnStack(
  Dcm_ProtocolIndexType NextProtocolIndex
);

/** \brief Makes the stack ready for processing a TYPE1 request
 **
 ** This function, will make the stack ready for processing a TYPE1 request by, if necessary:
 **  - triggering a cancellation for the current Protocol (if it is running a TYPE1 request)
 **  - TYPE2 requests are not cancelled as they run in the background independent of whether
 **    the Protocol is current or not.
 **  - scheduling the current Protocol to be stopped
 **
 ** \param[in] NextProtocolIndex  The index of the protocol where the request was received.
 **
 */
extern FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_TerminateCurrentProtocolOnSameStack(
  Dcm_ProtocolIndexType NextProtocolIndex
);

/** \brief Returns the type of the ProtocolGroup for the given protocol index
 **
 ** \param[in] ProtocolIndex  The index of the protocol.
 **
 ** \return
 **         - ::DCM_PROTOCOL_OBD_GROUP:  If the protocol type is one of OBD_ON_CAN, OBD_ON_FLEXRAY
 **                                      or OBD_ON_IP
 **         - ::DCM_PROTOCOL_UDS_GROUP:  If the protocol is not OBD
 **
 */
extern FUNC (Dcm_ProtocolGroupType, DCM_CODE)
  Dcm_Dsl_Supervisor_ProtocolHandling_GetProtocolGroupTypeForProtocol(
    Dcm_ProtocolIndexType ProtocolIndex
);

#if (DCM_HAS_ROE_TYPE2 == STD_ON)
/** \brief Returns whether the indicated protocol has any active or cancelling ROE TYPE2 request.
 **
 ** \param[in] ProtocolIndex  The index of the protocol.
 **
 ** \return
 **         - ::TRUE:  The indicated protocol has a TYPE2 ROE request that is active, or being
 **                    cancelled
 **         - ::FALSE: ROE TYPE2 is not running or cancelling on the indicated protocol
 **
 */
extern FUNC (boolean, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_IsTYPE2ROEPresent(
  Dcm_ProtocolIndexType ProtocolIndex
);
#endif

/** \brief Returns the type of the request running on the indicated protocol.
 **
 ** \param[in] ProtocolIndex  The index of the protocol.
 **
 ** \return  Type of the request.
 **
 */
extern FUNC (Dcm_SupervisorProtocolRequestTypeType, DCM_CODE)
    Dcm_Dsl_Supervisor_ProtocolHandling_GetProtocolRequestType(
    Dcm_ProtocolIndexType ProtocolIndex
);

/** \brief Returns whether the DCM module can be switched to the indicated session.
 **
 ** If the DCM is busy with an OBD request and the intention is to switch to non default session
 ** the session switch is not accepted.
 **
 ** \return
 **         - ::DCM_SUPERVISOR_SESSION_SWITCH_ACCEPT:        Session switch is allowed.
 **         - ::DCM_SUPERVISOR_SESSION_SWITCH_BUSY_DISCARD:  Session switch is not allowed.
 **
 */
extern FUNC (Dcm_SessionSwitchPermissionType, DCM_CODE)
  Dcm_Dsl_Supervisor_ProtocolHandling_IsSessionClearedToSwitch(
    Dcm_SesCtrlType NewSession
);

/** \brief Signals to the internal implementation that the indicated protocol became free.
 **
 ** The function updates the state of the internal protocol state machine and forwards the signal
 ** to the CommunicationSessionHandling and MainConnectionHandling if it is required.
 **
 ** \param[in] ProtocolIndex  The index of the protocol.
 **
 */
extern FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_ProtocolFree(
  Dcm_ProtocolIndexType ProtocolIndex
);

/** \brief Returns whether the Type1 request can be accepted for some processing on the indicated
 **        protocol.
 **
 ** \param[in] RequestedProtocolIndex  The index of the protocol that should process the request.
 ** \param[in] RequestOrigin           The origin of the request.
 **
 ** \return
 **         - ::DCM_SUPERVISOR_ARBITRATION_REJECT_PRIORITY: Request can not be accepted.
 **         - ::DCM_SUPERVISOR_ARBITRATION_REJECT_SAME_PROTOCOL_TYPE: Request can not be accepted.
 **                Protocol on a different stack of same type running or presumptive found.
 **         - ::DCM_SUPERVISOR_ARBITRATION_ACCEPT:  Request can be accepted.
 **
 */
extern FUNC (Dcm_ReceptionArbitrationResultType, DCM_CODE)
  Dcm_Dsl_Supervisor_ProtocolHandling_TYPE1RequestPriorityAssessment(
    Dcm_ProtocolIndexType RequestedProtocolIndex,
    Dcm_RequestOriginType RequestOrigin
);

/** \brief Returns whether the protocol is busy processing a Type1 ROE request
 **
 ** \param[in]  ProtocolIndex The index of the protocol which is checked.
 **
 ** \retval TRUE       Protocol is busy processing a Type1 ROE request
 ** \retval FALSE      Protocol is NOT busy processing a Type1 ROE request
 **
 */
extern FUNC (boolean, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_ProtocolIsBusyWithType1ROE(
  Dcm_ProtocolIndexType ProtocolIndex
);

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON))
/** \brief Returns whether the Type2 request can be accepted for some processing on the indicated
 **        protocol.
 **
 ** \param[in] RequestedProtocolIndex  The index of the protocol that should process the request.
 ** \param[in] RequestOrigin           The origin of the request.
 **
 ** \return
 **         - ::DCM_SUPERVISOR_ARBITRATION_REJECT_PRIORITY:  Request can not be accepted.
 **         - ::DCM_SUPERVISOR_ARBITRATION_ACCEPT:           Request can be accepted.
 **
 */
extern FUNC (Dcm_ReceptionArbitrationResultType, DCM_CODE)
  Dcm_Dsl_Supervisor_ProtocolHandling_TYPE2RequestPriorityAssessment(
    Dcm_ProtocolIndexType RequestedProtocolIndex
);
#endif /* #if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON)) */

/** \brief Cancels all receptions pending on protocols which have lower priorities than the
 **        provided RxConnection
 **
 ** \param[in] RxConnectionId  The id of the RxConnection to be compared to.
 **
 */
extern FUNC (void, DCM_CODE)
  Dcm_Dsl_Supervisor_ProtocolHandling_CancelOngoingReceptionsOnLowerPriorityProtocolsOfSameStack(
    Dcm_BufferUserType RxConnectionId
);

/** \brief Clears the enqueued request of the provided ProtocolIndex
 **
 ** \param[in] ProtocolIndex      The index of the protocol that should be cleared.
 ** \param[in] RequestDispatched  TRUE if the request was already dispatched, FALSE otherwise.
 **
 */
extern FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_ClearPresumptiveProtocol(
  Dcm_ProtocolIndexType ProtocolIndex,
  boolean RequestDispatched
);

/** \brief Cancels the enqueued requests of the protocols belonging to the same stack as the
 **        provided ProcessingProtocolIndex
 **
 ** \param[in] ProcessingProtocolIndex  The index of the protocol.
 **
 */
extern FUNC (void, DCM_CODE)
  Dcm_Dsl_Supervisor_ProtocolHandling_CancelTYPE1PresumptiveProtocolsOnSameStack(
    Dcm_ProtocolIndexType ProcessingProtocolIndex
);

/** \brief Enqueue the request to the provided protocol for execution when the protocol becomes
 **        free.
 **
 ** \param[in] ProcessingProtocolIndex  Index of the protocol that should be cleared.
 ** \param[in] RxConnectionId           Id of the RxConnection where the request was received.
 ** \param[in] RequestType              Type of the request.
 ** \param[in] RequestBuffer            Buffer that contains the request.
 ** \param[in] PreemptionNecessary      TRUE if a protocol preemption is necessary.
 **
 */
extern FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_EnqueueAsPresumptive(
  Dcm_ProtocolIndexType ProcessingProtocolIndex,
  Dcm_BufferUserType RxConnectionId,
  Dcm_RequestTypeType RequestType,
  Dcm_BufferType RequestBuffer,
  boolean PreemptionNecessary
);

/** \brief Cancels all the ROE Type1/Type2 running/enqueued requests.
 **
 */
extern FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_CancelROEProcessingOnAllProtocols(
  void
);

/** \brief Cancels all UDS protocols.
 **
 */
extern FUNC (void, DCM_CODE)
  Dcm_Dsl_Supervisor_ProtocolHandling_CancelUDSProtocolsInNonDefaultSession(
    void
);

/** \brief Returns the index of the last served UDS protocol.
 **
 ** \return Index of the last served UDS protocol.
 **
 */
extern FUNC (Dcm_ProtocolIndexType, DCM_CODE)
  Dcm_Dsl_Supervisor_ProtocolHandling_GetLatestExternalUDSProtocolIndex(
    void
);

/** \brief Signals to the internal implementation that the P2Timer assigned to the TimerIndex
 **        protocol expired to take the required actions.
 **
 ** Transmits a NRC depending on the type of the enqueued request and RCRRPs already sent out. Also
 ** can remove the enqueued request.
 **
 ** \param[in] TimerIndex  The index of the protocol.
 **
 */
extern FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_P2TimerTimeout(
  Dcm_TimerIndexType TimerIndex
);

/** \brief Indicates to the internal implementation that the RCRRP transmission was completed for
 **        the given ProtocolIndex
 **
 ** \param[in] ProtocolIndex  The index of the protocol.
 **
 */
extern FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_RCRRPTransmissionFinished(
  Dcm_ProtocolIndexType ProtocolIndex
);

/** \brief Indicates to the internal implementation that an RCRRP transmission was initiated for
 **        the given ProtocolIndex
 **
 ** \param[in] ProtocolIndex  The index of the protocol.
 **
 */
extern FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_RCRRPSent(
  Dcm_ProtocolIndexType ProtocolIndex
);

/** \brief Stops and releases P2Timer of the given TYPE1PresumptiveProtocolIndex and
 **        removes it from presumptive queue.
 **
 ** \param[in] TYPE1PresumptiveProtocolIndex  The index of the protocol.
 **
 */
extern FUNC (void, DCM_CODE)
  Dcm_Dsl_Supervisor_ProtocolHandling_PresumptiveProtocolEnd(
    Dcm_ProtocolIndexType TYPE1PresumptiveProtocolIndex
);

#if (DCM_META_DATA_HANDLING_ENABLED == STD_ON)
/** \brief              Retrieves MetaDataInfo (Source and Target address) from EcuC
 **                     in case a generic connection is active for the requested connection.
 **
 **
 ** \param[in]          PresumptiveProtocolIndex The index of the protocol. Needed for the
 **                     cases in which the function is called from ProtocolHandling.
 ** \param[in]          ReceivedMainConnectionIndex The index of the the MainConnection. Needed for the
 **                     cases in which the function is called from MainConnectionHandling
 **                     (i.e.NRC handling).
 **
 */
extern FUNC(void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_MetaDataInfoHandling(
  Dcm_ProtocolIndexType PresumptiveProtocolIndex,
  Dcm_MainConnectionIndexType ReceivedMainConnectionIndex
);
#endif /* #if (DCM_META_DATA_HANDLING_ENABLED == STD_ON) */

#define DCM_STOP_SEC_CODE
#include <MemMap.h>

/*==================[internal function definitions]==============================================*/

#endif /* ifndef DCM_DSL_SUPERVISOR_PROTOCOLHANDLING_H */
/*==================[end of file]================================================================*/
