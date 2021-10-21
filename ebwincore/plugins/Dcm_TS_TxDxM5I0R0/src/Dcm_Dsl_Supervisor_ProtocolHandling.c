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

/* !LINKSTO Dcm.Dsn.File.Supervisor.ProtocolHandling.Impl,1 */
/* This file contains the implementation of the Supervisor ProtocolHandling software unit. */

/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true violated
 *
 * Reason:
 * This warnings is a false positive.
 * The compiler optimizes the 'for' loop (which has only 1 iteration) in the following way
 * - it prepares in advance the index for the next iteration
 * - if the index is equal to '0' it goes back to check the 'for' loop conditions, otherwise it just moves on
 * - since the index is already equal to '1', it never jumps back to check the exit condition
 *
 * TASKING-2) Deviated Rule: W549
 * W549: condition is always true violated
 *
 * Reason:
 * The warning is triggered because of the DET INVARIANT_ASSERT macro.
 * This macro is used specifically to ensure that the condition is always true.
 * The false condition is not allowed and shall trigger a DET error.
 */
/*===============================[includes]======================================================*/

#include <Dcm_Trace_Internal.h>
#include <Dcm_Dsl_Supervisor_ProtocolHandling.h>
#include <Dcm_Dsl_Supervisor_MainConnectionHandling.h>
#include <Dcm_Dsl_Supervisor_CommunicationSessionHandling.h>
#include <Dcm_Dsl_SecurityLevelHandler.h>

#include <SchM_Dcm.h>                                                  /* SchM interface for Dcm */

#if ( (DCM_DSP_USE_SERVICE_0X03 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X07 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X0A == STD_ON) )
#include <Dcm_Dsp_DemFacade.h>
#endif

#if (DCM_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>                     /* Development Error Tracer (DCM_DET_REPORT_ERROR() macros) */
#endif

/*===========================[macro definitions]=================================================*/

/** @defgroup Macro_Dcm_SupervisorProtocolStateType Macros for Dcm_SupervisorProtocolStateType
 ** \brief   These macros are used as values for Dcm_SupervisorProtocolStateType
 **  @{
 */
/** \brief The protocol is in invalid state. Not reachable in normal conditions. */
#if (defined DCM_SUPERVISOR_PROTOCOL_INVALID_STATE)
  #error "DCM_SUPERVISOR_PROTOCOL_INVALID_STATE is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_INVALID_STATE 0x00U

/** \brief The protocol is stopped. It is free to handle requests from the queue. */
#if (defined DCM_SUPERVISOR_PROTOCOL_STOPPED_IDLE)
  #error "DCM_SUPERVISOR_PROTOCOL_STOPPED_IDLE is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_STOPPED_IDLE 0x01U

/** \brief The protocol is stopped. It is processing a TYPE2 ROE request. */
#if (defined DCM_SUPERVISOR_PROTOCOL_STOPPED_ACTIVE_TYPE2_ROE)
  #error "DCM_SUPERVISOR_PROTOCOL_STOPPED_ACTIVE_TYPE2_ROE is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_STOPPED_ACTIVE_TYPE2_ROE 0x02U

/** \brief The protocol is stopped. It is processing a TYPE2 Periodic request. */
#if (defined DCM_SUPERVISOR_PROTOCOL_STOPPED_ACTIVE_TYPE2_PERIODIC)
  #error "DCM_SUPERVISOR_PROTOCOL_STOPPED_ACTIVE_TYPE2_PERIODIC is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_STOPPED_ACTIVE_TYPE2_PERIODIC 0x03U

/** \brief The protocol is stopped. It is cancelling a running TYPE2 ROE request. */
#if (defined DCM_SUPERVISOR_PROTOCOL_STOPPED_CANCELLING_TYPE2_ROE)
  #error "DCM_SUPERVISOR_PROTOCOL_STOPPED_CANCELLING_TYPE2_ROE is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_STOPPED_CANCELLING_TYPE2_ROE 0x04U

/** \brief The protocol is stopped. It is cancelling a running TYPE2 Periodic request. */
#if (defined DCM_SUPERVISOR_PROTOCOL_STOPPED_CANCELLING_TYPE2_PERIODIC)
  #error "DCM_SUPERVISOR_PROTOCOL_STOPPED_CANCELLING_TYPE2_PERIODIC is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_STOPPED_CANCELLING_TYPE2_PERIODIC 0x05U

/** \brief The protocol is started. It is the CurrentProtocol, free to handle requests from
 ** the queue. */
#if (defined DCM_SUPERVISOR_PROTOCOL_STARTED_IDLE)
  #error "DCM_SUPERVISOR_PROTOCOL_STARTED_IDLE is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_STARTED_IDLE 0x06U

/** \brief The protocol is started. It is the CurrentProtocol, processing/terminating the request
 ** that caused an ECU restart (jump from bootloader). */
#if (defined DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_POST_BOOT)
  #error "DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_POST_BOOT is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_POST_BOOT 0x08U

/** \brief The protocol is started. It is the CurrentProtocol, processing an external request. */
#if (defined DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_EXTERNAL)
  #error "DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_EXTERNAL is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_EXTERNAL 0x09U

/** \brief The protocol is started. It is the CurrentProtocol, processing a TYPE1 ROE request. */
#if (defined DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_ROE)
  #error "DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_ROE is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_ROE 0x0AU

/** \brief The protocol is started. It is the CurrentProtocol, processing a TYPE1 periodic
 ** request. */
#if (defined DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_PERIODIC)
  #error "DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_PERIODIC is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_PERIODIC 0x0BU

/** \brief The protocol is started. It is the CurrentProtocol, processing a TYPE2 ROE request. */
#if (defined DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE2_ROE)
  #error "DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE2_ROE is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE2_ROE 0x0CU

/** \brief The protocol is started. It is the CurrentProtocol, processing a TYPE2 periodic
 ** request. */
#if (defined DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE2_PERIODIC)
  #error "DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE2_PERIODIC is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE2_PERIODIC 0x0DU

/** \brief The Protocol is started. A cancellation of the external request processing was
 ** initiated. The protocol should remain started after the cancellation terminates. */
#if (defined DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_EXTERNAL)
  #error "DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_EXTERNAL is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_EXTERNAL 0x0EU

/** \brief The Protocol is started. A cancellation of the TYPE1 ROE request processing was
 ** initiated. The protocol should remain started after the cancellation terminates. */
#if (defined DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE1_ROE)
  #error "DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE1_ROE is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE1_ROE 0x0FU

/** \brief The Protocol is started. A cancellation of the TYPE1 periodic request processing was
 ** initiated. The protocol should remain started after the cancellation terminates. */
#if (defined DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE1_PERIODIC)
  #error "DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE1_PERIODIC is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE1_PERIODIC 0x10U

/** \brief The Protocol is started. A cancellation of the TYPE2 ROE request processing was
 ** initiated. The protocol should remain started after the cancellation terminates. */
#if (defined DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE2_ROE)
  #error "DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE2_ROE is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE2_ROE 0x11U

/** \brief The Protocol is started. A cancellation of the TYPE2 periodic request processing was
 ** initiated. The protocol should remain started after the cancellation terminates. */
#if (defined DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE2_PERIODIC)
  #error "DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE2_PERIODIC is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE2_PERIODIC 0x12U

/** \brief The Protocol is started. A cancellation of the external request processing was
 ** initiated. The protocol should be stopped after the cancellation terminates. */
#if (defined DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_EXTERNAL)
  #error "DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_EXTERNAL is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_EXTERNAL 0x13U

/** \brief The Protocol is started. A cancellation of the TYPE1 ROE request processing was
 ** initiated. The protocol should be stopped after the cancellation terminates. */
#if (defined DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE1_ROE)
  #error "DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE1_ROE is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE1_ROE 0x14U

/** \brief The Protocol is started. A cancellation of the TYPE1 periodic request processing was
 ** initiated. The protocol should be stopped after the cancellation terminates. */
#if (defined DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE1_PERIODIC)
  #error "DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE1_PERIODIC is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE1_PERIODIC 0x15U

/** \brief The Protocol is started. It should be stopped after the cancellation of the active
 ** TYPE2 ROE request terminates. */
#if (defined DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE2_ROE)
  #error "DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE2_ROE is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE2_ROE 0x16U

/** \brief The Protocol is started. It should be stopped after the cancellation of the active
 ** TYPE2 periodic request terminates. */
#if (defined DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE2_PERIODIC)
  #error "DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE2_PERIODIC is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE2_PERIODIC 0x17U

/** \brief The Protocol is started. It is ready to be stopped. No processing/transmission is
 ** ongoing. */
#if (defined DCM_SUPERVISOR_PROTOCOL_STOPPING_IDLE)
  #error "DCM_SUPERVISOR_PROTOCOL_STOPPING_IDLE is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_STOPPING_IDLE 0x18U

/** \brief The Protocol is started. It is processing a TYPE2 ROE request and it can be stopped
 ** at this point from the MainFunction context. */
#if (defined DCM_SUPERVISOR_PROTOCOL_STOPPING_ACTIVE_TYPE2_ROE)
  #error "DCM_SUPERVISOR_PROTOCOL_STOPPING_ACTIVE_TYPE2_ROE is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_STOPPING_ACTIVE_TYPE2_ROE 0x19U

/** \brief The Protocol is started. It is processing a TYPE2 periodic request, and it can be
 ** stopped at this point from the MainFunction context. */
#if (defined DCM_SUPERVISOR_PROTOCOL_STOPPING_ACTIVE_TYPE2_PERIODIC)
  #error "DCM_SUPERVISOR_PROTOCOL_STOPPING_ACTIVE_TYPE2_PERIODIC is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_STOPPING_ACTIVE_TYPE2_PERIODIC 0x1AU

/** \brief The Protocol is started. A cancellation of the TYPE2 ROE request processing was
 ** initiated. The protocol can be stopped at this point from the MainFunction context. */
#if (defined DCM_SUPERVISOR_PROTOCOL_STOPPING_CANCELLING_TYPE2_ROE)
  #error "DCM_SUPERVISOR_PROTOCOL_STOPPING_CANCELLING_TYPE2_ROE is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_STOPPING_CANCELLING_TYPE2_ROE 0x1BU

/* @} */
/* End of macros definitions for Dcm_SupervisorProtocolStateType */

/** @defgroup Macro_Dcm_SupervisorProtocolQueueStateType Macros for
 **           Dcm_SupervisorProtocolQueueStateType
 ** \brief    These macros are used as values for Dcm_SupervisorProtocolQueueStateType
 **  @{
 */

/** \brief The protocol queue is in invalid state. Not reachable in normal conditions. */
#if (defined DCM_SUPERVISOR_PROTOCOL_QUEUE_INVALID_STATE)
  #error "DCM_SUPERVISOR_PROTOCOL_QUEUE_INVALID_STATE is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_QUEUE_INVALID_STATE 0x00U

/** \brief The protocol queue is empty. Ready to receive a new request. */
#if (defined DCM_SUPERVISOR_PROTOCOL_QUEUE_IDLE)
  #error "DCM_SUPERVISOR_PROTOCOL_QUEUE_IDLE is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_QUEUE_IDLE 0x01U

/** \brief A jump from bootloader response request has been enqueued for the protocol. */
#if (defined DCM_SUPERVISOR_PROTOCOL_QUEUE_POST_BOOT_QUEUED)
  #error "DCM_SUPERVISOR_PROTOCOL_QUEUE_POST_BOOT_QUEUED is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_QUEUE_POST_BOOT_QUEUED 0x02U

/** \brief An external request has been enqueued for the protocol. */
#if (defined DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED)
  #error "DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED 0x03U

/** \brief A RCRRP NRC needs to be sent out for the enqueued external request. */
#if (defined DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED_RCRRP_QUEUED)
  #error "DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED_RCRRP_QUEUED is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED_RCRRP_QUEUED 0x04U

/** \brief A RCRRP NRC transmission is initiated for the enqueued external request. */
#if (defined DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED_RCRRP_TRANSMITTING)
  #error "DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED_RCRRP_TRANSMITTING is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED_RCRRP_TRANSMITTING 0x05U

/** \brief A TYPE1 ROE request has been enqueued for the protocol. */
#if (defined DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE1_ROE_QUEUED)
  #error "DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE1_ROE_QUEUED is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE1_ROE_QUEUED 0x06U

/** \brief A TYPE1 Periodic request has been enqueued for the protocol. */
#if (defined DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE1_PERIODIC_QUEUED)
  #error "DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE1_PERIODIC_QUEUED is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE1_PERIODIC_QUEUED 0x07U

/** \brief A TYPE2 ROE request has been enqueued for the protocol. */
#if (defined DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE2_ROE_QUEUED)
  #error "DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE2_ROE_QUEUED is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE2_ROE_QUEUED 0x08U

/** \brief A TYPE2 Periodic request has been enqueued for the protocol. */
#if (defined DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE2_PERIODIC_QUEUED)
  #error "DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE2_PERIODIC_QUEUED is already defined"
#endif
#define DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE2_PERIODIC_QUEUED 0x09U

/* @} */
/* End of macros definitions for Dcm_SupervisorProtocolQueueStateType */

/*===========================[type definitions]==================================================*/

typedef uint8 Dcm_SupervisorProtocolStateType;

typedef uint8 Dcm_SupervisorProtocolQueueStateType;

/** \brief Timeout of preempting a protocol in ms. Range: 0 .. 1000. */
typedef uint16 Dcm_ProtocolPreemptTimeoutType;

typedef struct
{
  Dcm_ProtocolPreemptTimeoutType PreemptionTimeout;
#if (DCM_HAS_ROE_TYPE2 == STD_ON)
  Dcm_ProtocolPreemptTimeoutType ProcessingCancellingTimeout;
#endif /* #if (DCM_HAS_ROE_TYPE2 == STD_ON) */
  /** \brief Rx connection which received the request **/
  Dcm_BufferUserType RxConnectionId;
  /** \brief Overall state of the preemption algorithm. **/
  Dcm_SupervisorProtocolQueueStateType ProtocolQueueState;

  /** \brief Type of the request **/
  Dcm_RequestTypeType RequestType;

  uint16 RCRRPsSent;

  uint16 RCRRPsConfirmed;

  Dcm_BufferType RequestBuffer;

} Dcm_ProtocolQueueType;

typedef struct
{
  Dcm_BufferUserType RxConnectionId;

  Dcm_SupervisorProtocolStateType ProtocolState;

  Dcm_RequestTypeType RequestType;

} Dcm_ProtocolStatusType;

typedef uint8 Dcm_StackNumberType;

/*=====================[external constants declarations]=========================================*/

/*=====================[internal constants declarations]=========================================*/

/*==================[internal function declarations]=============================================*/
#define DCM_START_SEC_CODE
#include <MemMap.h>

STATIC FUNC (boolean, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_ProtocolIsPresumptive(
  Dcm_ProtocolIndexType ProtocolIndex
);

STATIC FUNC (boolean, DCM_CODE)
  Dcm_Dsl_Supervisor_ProtocolHandling_ProtocolIsPresumptiveWithNonROE(
    Dcm_ProtocolIndexType ProtocolIndex
);

STATIC FUNC (boolean, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_ProtocolIsBusy(
  Dcm_ProtocolIndexType ProtocolIndex
);

STATIC FUNC (boolean, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_ProtocolIsBusyWithNonROE(
  Dcm_ProtocolIndexType ProtocolIndex
);

STATIC FUNC (Dcm_ProtocolIndexType, DCM_CODE)
  Dcm_Dsl_Supervisor_ProtocolHandling_GetCurrentProtocolOnStack(
    Dcm_ProtocolStackNumberType StackNumber
);

STATIC FUNC (boolean, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_StackCanAcceptTYPE1Request(
  Dcm_StackNumberType StackNumber
);

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON))
STATIC FUNC (boolean, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_ProtocolCanAcceptTYPE2Request(
  Dcm_ProtocolIndexType ProtocolIndex
);
#endif /* #if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON)) */

STATIC FUNC (Dcm_ProtocolIndexType, DCM_CODE) Dcm_Dsl_Supervisor_GetTYPE1PresumptiveProtocolOnStack(
  Dcm_StackNumberType StackNumber
);

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON))
STATIC FUNC (Dcm_ProtocolIndexType, DCM_CODE) Dcm_Dsl_Supervisor_GetTYPE2PresumptiveProtocolOnStack(
  Dcm_StackNumberType StackNumber
);
#endif /* #if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON)) */

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON))
STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_MakeTYPE2ProtocolProcess(
  Dcm_ProtocolIndexType ProtocolIndex
);
#endif /* #if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON)) */

STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_MakeStartedProtocolActive(
  Dcm_ProtocolIndexType ProtocolIndex
);

STATIC FUNC (boolean, DCM_CODE)
  Dcm_Dsl_Supervisor_ProtocolHandling_ProtocolOfSameTypeRunningOnDifferentStack(
    Dcm_ProtocolIndexType RxProtocolIndex
);

#if (DCM_DSP_USE_ROESERVICES == STD_ON)
STATIC FUNC (boolean, DCM_CODE)
  Dcm_Dsl_Supervisor_ProtocolHandling_ProtocolOfDifferentTypeRunningOnDifferentStack(
    Dcm_ProtocolIndexType RxProtocolIndex
);
#endif

STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_GiveTYPE1RequestBufferToProtocol(
  Dcm_SupervisorProtocolQueueStateType ProtocolQueueState,
  Dcm_BufferUserType ProtocolToStartId,
  Dcm_BufferIdType BufferId
);

STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_DispatchTYPE1RequestToProtocol(
  Dcm_ProtocolIndexType ProtocolIndex
);

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON))
STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_DispatchTYPE2RequestToProtocol(
  Dcm_ProtocolIndexType ProtocolIndex
);
#endif /*#if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON)) */

STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_CancelPresumptiveProtocol(
  Dcm_ProtocolIndexType ProtocolIndex
);

STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_AttemptToStopStoppingProtocols(
  void
);

STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_AttemptToStartPresumptiveTYPE1ProtocolOnStack(
  Dcm_StackNumberType StackNumber
);

STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_AttemptToDispatchPresumptiveTYPE1ProtocolOnStack(
  Dcm_StackNumberType StackNumber
);

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON))
STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_AttemptToDispatchPresumptiveTYPE2ProtocolOnStack(
  Dcm_StackNumberType StackNumber
);
#endif /* #if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON)) */

STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_ResetToDefaultIfSessionWasSetWithProtocolFromAnotherStack(
  Dcm_ProtocolIndexType ProtocolIndex
);

STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_DispatchPresumptiveTYPE1Protocol(
  Dcm_ProtocolIndexType TYPE1PresumptiveProtocolIndex
);

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON))
STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_DispatchPresumptiveTYPE2Protocol(
  Dcm_ProtocolIndexType TYPE2PresumptiveProtocolIndex
);
#endif /* #if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON)) */

#if (DCM_CALLBACK_REQUEST_SIZE > 0)
STATIC FUNC (boolean, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_CallStopProtocolOperations(
  Dcm_ProtocolIndexType StoppingProtocolIndex
);

STATIC FUNC (boolean, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_CallStartProtocolOperations(
  Dcm_ProtocolIndexType StartingProtocolIndex
);
#endif /* #if (DCM_CALLBACK_REQUEST_SIZE > 0) */

STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_PreemptionPendingProcess(
  Dcm_ProtocolIndexType ProtocolIndex
);

STATIC FUNC (void, DCM_CODE)
  Dcm_Dsl_Supervisor_ProtocolHandling_CancelPresumptiveUDSProtocolsInNonDefaultSession(
    void
);

STATIC FUNC (Std_ReturnType, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_MainConnNotBusy_StackCanAcceptRequest(
    Dcm_StackNumberType StackNumber,
    P2VAR(Dcm_ProtocolIndexType, AUTOMATIC, DCM_VAR) ProtocolIndex,
    boolean RequestIsType1
);
#define DCM_STOP_SEC_CODE
#include <MemMap.h>
/*=======================[external data declarations]============================================*/

/*=======================[internal data declarations]============================================*/
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

STATIC VAR(Dcm_ProtocolStatusType, DCM_VAR) Dcm_ProtocolStatus[DCM_NUM_PROTOCOLS];

STATIC VAR(Dcm_ProtocolQueueType, DCM_VAR) Dcm_ProtocolQueue[DCM_NUM_PROTOCOLS];

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

#define DCM_START_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

STATIC VAR(Dcm_ProtocolIndexType, DCM_VAR) Dcm_LatestExternalUDSProtocolIndex;
STATIC VAR(boolean, DCM_VAR) Dcm_PresumptiveProtocolToBeStartedOrDispatched;

#define DCM_STOP_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>
/*==================[external function definitions]==============================================*/
#define DCM_START_SEC_CODE
#include <MemMap.h>

FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_Init(
  void
)
{
  uint8_least ProtocolIndex;

 /* Deviation TASKING-1 */
  for (ProtocolIndex = 0U; ProtocolIndex < DCM_NUM_PROTOCOLS; ProtocolIndex++)
  {
    Dcm_ProtocolStatus[ProtocolIndex].RxConnectionId = DCM_INVALID_USER_ID;
    Dcm_ProtocolStatus[ProtocolIndex].ProtocolState = DCM_SUPERVISOR_PROTOCOL_STOPPED_IDLE;
    Dcm_ProtocolStatus[ProtocolIndex].RequestType = DCM_INVALID_REQUEST_TYPE;

    Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState = DCM_SUPERVISOR_PROTOCOL_QUEUE_IDLE;
    Dcm_ProtocolQueue[ProtocolIndex].RxConnectionId = DCM_INVALID_USER_ID;
    Dcm_ProtocolQueue[ProtocolIndex].PreemptionTimeout = 0U;
#if (DCM_HAS_ROE_TYPE2 == STD_ON)
    Dcm_ProtocolQueue[ProtocolIndex].ProcessingCancellingTimeout = 0U;
#endif /* #if (DCM_HAS_ROE_TYPE2 == STD_ON) */
    Dcm_ProtocolQueue[ProtocolIndex].RequestType = DCM_INVALID_REQUEST_TYPE;
    Dcm_ProtocolQueue[ProtocolIndex].RCRRPsSent = 0U;
    Dcm_ProtocolQueue[ProtocolIndex].RCRRPsConfirmed = 0U;
    Dcm_ProtocolQueue[ProtocolIndex].RequestBuffer.BufferId = DCM_BUFFER_INVALID_ID;
  }

  Dcm_LatestExternalUDSProtocolIndex = DCM_INVALID_PROTOCOL_INDEX;
  Dcm_PresumptiveProtocolToBeStartedOrDispatched = FALSE;
}

FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_MainFunction(
  void
)
{
  uint8_least StackNumber;

  Dcm_Dsl_Supervisor_ProtocolHandling_AttemptToStopStoppingProtocols();

  if (Dcm_PresumptiveProtocolToBeStartedOrDispatched == TRUE)
  {
    /* Work per stack. There's a maximum of one presumptive on each stack.
     */
    /* Deviation TASKING-1 */
    for (StackNumber = 0U; StackNumber < DCM_NUM_STACKS; StackNumber++)
    {
      Dcm_ProtocolIndexType CurrentProtocolIndex =
        Dcm_Dsl_Supervisor_ProtocolHandling_GetCurrentProtocolOnStack(
            (Dcm_ProtocolStackNumberType)(StackNumber)
          );

      if (DCM_INVALID_PROTOCOL_INDEX == CurrentProtocolIndex)
      {
        /* If the stack has no Current Protocol, see if there is any presumptive waiting and
           start the protocol. */
        Dcm_Dsl_Supervisor_ProtocolHandling_AttemptToStartPresumptiveTYPE1ProtocolOnStack(
            (Dcm_ProtocolStackNumberType)(StackNumber)
          );
      }

      Dcm_Dsl_Supervisor_ProtocolHandling_AttemptToDispatchPresumptiveTYPE1ProtocolOnStack(
          (Dcm_ProtocolStackNumberType)(StackNumber)
        );

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON))
      Dcm_Dsl_Supervisor_ProtocolHandling_AttemptToDispatchPresumptiveTYPE2ProtocolOnStack(
          (Dcm_ProtocolStackNumberType)(StackNumber)
      );
#endif /* #if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON)) */
    }
  }
}

FUNC (Std_ReturnType, DCM_CODE)
  Dcm_Dsl_Supervisor_ProtocolHandling_GetLatestExternalUDSProtocolConfig(
    Dcm_ProtocolConfigPtrType CurrentProtocolConfig
)
{
  Std_ReturnType GetProtocolConfigResult = DCM_E_NOT_OK;

  if (Dcm_LatestExternalUDSProtocolIndex != DCM_INVALID_PROTOCOL_INDEX)
  {
    *CurrentProtocolConfig = DCM_PROTOCOL_CONFIG(Dcm_LatestExternalUDSProtocolIndex);
    GetProtocolConfigResult = DCM_E_OK;
  }

  return GetProtocolConfigResult;
}

FUNC (boolean, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_PreemptionNecessaryOnStack(
  Dcm_ProtocolIndexType NextProtocolIndex
)
{
  boolean PreemptionNecessary = FALSE;
  Dcm_ProtocolIndexType CurrentProtocolIndex;
  Dcm_StackNumberType StackNumber = Dcm_ProtocolConfig[NextProtocolIndex].StackNumber;

  /* We identify which one the CurrentProtocol is on the CurrentStack */
  CurrentProtocolIndex =
        Dcm_Dsl_Supervisor_ProtocolHandling_GetCurrentProtocolOnStack(StackNumber);

  if (DCM_INVALID_PROTOCOL_INDEX != CurrentProtocolIndex)
  {
    /* There is a CurrentProtocol. The request got accepted so maybe it needs
       to be processed somehow. */

    if (CurrentProtocolIndex != NextProtocolIndex)
    {
      /* Make a note that preemption will be necessary. Preemption means that the current
         protocol is replaced. Cancellation can happen without preemption! */
      PreemptionNecessary = TRUE;
    }
  }
  else
  {
    /* When there is no current protocol, preemption is necessary. */
    PreemptionNecessary = TRUE;
  }

  return PreemptionNecessary;
}

/* !LINKSTO Dcm.Dsn.IB.Supervisor.ProtocolHandling.TerminateCurrentProtocolOnSameStack,1 */
FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_TerminateCurrentProtocolOnSameStack(
  Dcm_ProtocolIndexType NextProtocolIndex
)
{
  Dcm_ProtocolIndexType CurrentProtocolIndex;

  Dcm_ProtocolStackNumberType StackNumber;

  /* Fetch the stack number of the Protocol which will process next. */
  StackNumber = Dcm_ProtocolConfig[NextProtocolIndex].StackNumber;

  /* This function, when called, will make the stack ready for processing a TYPE1 request by, if necessary:
  - triggering a cancellation for the current Protocol if it is running a TYPE1 request.
    TYPE2 requests are not cancelled as they run in the background independent of whether
    the Protocol is current or not.
  - scheduling the current Protocol to be stopped
  */

  /* We identify which one the CurrentProtocol is on the CurrentStack */
  CurrentProtocolIndex =
        Dcm_Dsl_Supervisor_ProtocolHandling_GetCurrentProtocolOnStack(StackNumber);

  if (DCM_INVALID_PROTOCOL_INDEX != CurrentProtocolIndex)
  {

    /* There is a CurrentProtocol. The request got accepted so maybe it needs
       to be processed somehow. */

    /* PostBoot processing can't be preempted since it blocks any other reception. */
    DCM_PRECONDITION_ASSERT(
        (DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_POST_BOOT !=
           Dcm_ProtocolStatus[CurrentProtocolIndex].ProtocolState),
           DCM_INTERNAL_API_ID);

    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.CancelProcessing, 1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.CancelProcessing, 1 */

    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.CancelTransmission, 1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.CancelTransmission, 1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusyTYPE1Periodic.CancelTransmission, 1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusyTYPE1Periodic.CancelTransmission, 1 */

    switch (Dcm_ProtocolStatus[CurrentProtocolIndex].ProtocolState)
    {
      /* TYPE1 external/internal. Cancel and schedule
         to stop after cancellation.
       */
      case DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_EXTERNAL:
        /* Give Cancel signal to Protocol. */
        Dcm_Dsp_ProtocolCancel(DCM_PROTOCOL_ID_FROM_INDEX(CurrentProtocolIndex));
        Dcm_ProtocolStatus[CurrentProtocolIndex].ProtocolState = DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_EXTERNAL;
        break;

      case DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_PERIODIC:

        /* Give Cancel signal to Protocol. */
        Dcm_Dsp_ProtocolCancel(DCM_PROTOCOL_ID_FROM_INDEX(CurrentProtocolIndex));
        Dcm_ProtocolStatus[CurrentProtocolIndex].ProtocolState = DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE1_PERIODIC;
        break;

      case DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_ROE:
        /* Give Cancel signal to Protocol. ROE processing always gets cancelled by any external
           request or a higher priority Periodic request. */
        Dcm_Dsp_ProtocolCancel(DCM_PROTOCOL_ID_FROM_INDEX(CurrentProtocolIndex));

        /* If it's not the same as the RxProtocol we preempt it and the Protocol goes to a Stopped state
           once cancellation is done.
         */
        Dcm_ProtocolStatus[CurrentProtocolIndex].ProtocolState =
          DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE1_ROE;
        break;

      case DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE2_ROE:

        /* Preempt the CurrentProtocol but keep it in a TYPE2 ROE running state. */
          Dcm_ProtocolStatus[CurrentProtocolIndex].ProtocolState =
            DCM_SUPERVISOR_PROTOCOL_STOPPING_ACTIVE_TYPE2_ROE;
        break;

      case DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE2_PERIODIC:

        /* Preempt the CurrentProtocol but keep it in a TYPE2 Periodic running state. */
        Dcm_ProtocolStatus[CurrentProtocolIndex].ProtocolState =
            DCM_SUPERVISOR_PROTOCOL_STOPPING_ACTIVE_TYPE2_PERIODIC;
        break;

      /* Protocols which are being cancelled with the intent to stay started are
         already cancelling so let the cancellation finish. Then mark them so as
         soon as they are done cancelling they will stop. */
      case DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_EXTERNAL:

        /* Mark to stop. */
        Dcm_ProtocolStatus[CurrentProtocolIndex].ProtocolState =
          DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_EXTERNAL;
        break;

      case DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE1_ROE:

        /* Mark to stop. */
        Dcm_ProtocolStatus[CurrentProtocolIndex].ProtocolState =
          DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE1_ROE;
        break;

      case DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE1_PERIODIC:

        /* Mark to stop. */
        Dcm_ProtocolStatus[CurrentProtocolIndex].ProtocolState =
          DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE1_PERIODIC;
        break;

      case DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE2_ROE:

        /* Mark to stop. */
        Dcm_ProtocolStatus[CurrentProtocolIndex].ProtocolState =
          DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE2_ROE;
        break;

      case DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE2_PERIODIC:

        /* Mark to stop. */
        Dcm_ProtocolStatus[CurrentProtocolIndex].ProtocolState =
          DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE2_PERIODIC;
        break;

      /* Protocols which are being cancelled with the intent to stop are simple:
         they're already cancelling so let the cancellation finish, as soon as they are
         done they will stop. */
      case DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_EXTERNAL:
      case DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE1_ROE:
      case DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE1_PERIODIC:
      case DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE2_ROE:
      case DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE2_PERIODIC:
        /* Just let cancellation finish. The stopping will proceed */
        break;

      /* If the CurrentProtocol is Started but not Active, mark it to stop. */
      case DCM_SUPERVISOR_PROTOCOL_STARTED_IDLE:
        Dcm_ProtocolStatus[CurrentProtocolIndex].ProtocolState =
            DCM_SUPERVISOR_PROTOCOL_STOPPING_IDLE;
        break;

      /* If the CurrentProtocol is already stopping, nothing more to do. */
      case DCM_SUPERVISOR_PROTOCOL_STOPPING_ACTIVE_TYPE2_ROE:
      case DCM_SUPERVISOR_PROTOCOL_STOPPING_ACTIVE_TYPE2_PERIODIC:
      case DCM_SUPERVISOR_PROTOCOL_STOPPING_CANCELLING_TYPE2_ROE:
      case DCM_SUPERVISOR_PROTOCOL_STOPPING_IDLE:
        /* Let the stopping proceed. */
        break;

      /* CHECK: NOPARSE */
      default:
        /* Defensive branch. Default case at the end of the switch statement */
        DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
        break;
      /* CHECK: PARSE */

    }
  }
}

FUNC (Dcm_ProtocolGroupType, DCM_CODE)
  Dcm_Dsl_Supervisor_ProtocolHandling_GetProtocolGroupTypeForProtocol(
    Dcm_ProtocolIndexType ProtocolIndex
)
{
  /* Might need some adjustment for hybrid Protocols, i.e. we will have a function for running
     Protocols and a function for Presumptive Protocols so as to look at service ID. */

  Dcm_ProtocolGroupType ProtocolGroup = DCM_PROTOCOL_UDS_GROUP;

  if(
     (DCM_OBD_ON_CAN == Dcm_ProtocolConfig[ProtocolIndex].Type) ||
     (DCM_OBD_ON_FLEXRAY == Dcm_ProtocolConfig[ProtocolIndex].Type) ||
     (DCM_OBD_ON_IP == Dcm_ProtocolConfig[ProtocolIndex].Type)
    )
  {
    ProtocolGroup = DCM_PROTOCOL_OBD_GROUP;
  }

  return ProtocolGroup;
}

#if (DCM_HAS_ROE_TYPE2 == STD_ON)
FUNC (boolean, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_IsTYPE2ROEPresent(
  Dcm_ProtocolIndexType ProtocolIndex
)
{
  boolean TYPE2ROEPresent = FALSE;
  const Dcm_SupervisorProtocolStateType ProtocolState =
    Dcm_ProtocolStatus[ProtocolIndex].ProtocolState;

  if ((DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE2_ROE == ProtocolState) ||
      (DCM_SUPERVISOR_PROTOCOL_STOPPED_ACTIVE_TYPE2_ROE == ProtocolState) ||
      (DCM_SUPERVISOR_PROTOCOL_STOPPED_CANCELLING_TYPE2_ROE == ProtocolState) ||
      (DCM_SUPERVISOR_PROTOCOL_STOPPING_ACTIVE_TYPE2_ROE == ProtocolState) ||
      (DCM_SUPERVISOR_PROTOCOL_STOPPING_CANCELLING_TYPE2_ROE == ProtocolState) ||
      (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE2_ROE == ProtocolState) ||
      (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE2_ROE == ProtocolState))
  {
    TYPE2ROEPresent = TRUE;
  }

  return TYPE2ROEPresent;
}
#endif

FUNC(Dcm_SupervisorProtocolRequestTypeType, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_GetProtocolRequestType(
  Dcm_ProtocolIndexType ProtocolIndex)
{
  DCM_PRECONDITION_ASSERT(ProtocolIndex <= DCM_NUM_PROTOCOLS, DCM_INTERNAL_API_ID);

  return Dcm_ProtocolStatus[ProtocolIndex].RequestType;
}

FUNC (Dcm_SessionSwitchPermissionType, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_IsSessionClearedToSwitch(
  Dcm_SesCtrlType NewSession
)
{
  uint8_least ProtocolIndex;
  Dcm_SessionSwitchPermissionType IsSessionClearedToSwitch = DCM_SUPERVISOR_SESSION_SWITCH_ACCEPT;

  /* OBD Protocols want the Session to stay the DefaultSession. Therefore, while any
     OBD Protocols are Running, the Session won't switch.
   */
  /* Deviation TASKING-1 <+2> */
  for (ProtocolIndex = 0U;
       ProtocolIndex < DCM_NUM_PROTOCOLS;
       ProtocolIndex++)
  {
    if (
        (DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_POST_BOOT ==
          Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
        (DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_EXTERNAL ==
          Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
        (DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_ROE ==
          Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
        (DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_PERIODIC ==
          Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
        (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_EXTERNAL ==
          Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
        (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE1_ROE ==
          Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
        (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE1_PERIODIC ==
          Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
        (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_EXTERNAL ==
          Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
        (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE1_ROE ==
          Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
        (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE1_PERIODIC ==
          Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
        /* If any of these is true, a preemption is taking place and the session will
           reset after the preemption. */
        (DCM_SUPERVISOR_PROTOCOL_STOPPING_IDLE ==
          Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
        (DCM_SUPERVISOR_PROTOCOL_STOPPING_ACTIVE_TYPE2_ROE ==
          Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
        (DCM_SUPERVISOR_PROTOCOL_STOPPING_ACTIVE_TYPE2_PERIODIC ==
          Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
        (DCM_SUPERVISOR_PROTOCOL_QUEUE_IDLE != Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState)
       )
    {
      const Dcm_ProtocolGroupType ProcessingProtocolGroup =
            Dcm_Dsl_Supervisor_ProtocolHandling_GetProtocolGroupTypeForProtocol(
                (Dcm_ProtocolIndexType)(ProtocolIndex)
              );

      if (
          (DCM_PROTOCOL_OBD_GROUP == ProcessingProtocolGroup) &&
          (DCM_DEFAULT_SESSION != NewSession)
         )
      {
        /* Busy with OBD and the request is to switch to non default
           means we discard the request to switch the session.
         */
        IsSessionClearedToSwitch = DCM_SUPERVISOR_SESSION_SWITCH_BUSY_DISCARD;

        /* Also break the search loop. Doesn't make sense to go on further.
         */
        break;
      }
    }
  }

  return IsSessionClearedToSwitch;
}

FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_ProtocolFree(
  Dcm_ProtocolIndexType ProtocolIndex
)
{
  boolean ResetMainConnection = FALSE;

  /* A Protocol reports it's done with whatever it was doing */

  switch (Dcm_ProtocolStatus[ProtocolIndex].ProtocolState)
  {
    case DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_POST_BOOT:
      /* Set the flag to allow request */
      Dcm_Dsl_Supervisor_DisinhibitRequestProcessing();
      /* Such Protocols become active as a result of an external request. Do not
         Terminate the communication session as this will be done as part of the
         confirmation actions. */

      ResetMainConnection = TRUE;
      /* Protocol finished its task. It becomes the StartedProtocol. */
      Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
        DCM_SUPERVISOR_PROTOCOL_STARTED_IDLE;

      Dcm_Dsl_Supervisor_CommunicationSessionHandling_EndOfCommunicationSession(
        Dcm_ProtocolStatus[ProtocolIndex].RxConnectionId);
      break;

    case DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_EXTERNAL:
      /* Such Protocols become active as a result of an external request. Do not
         Terminate the communication session as this will be done as part of the
         confirmation actions. */

      ResetMainConnection = TRUE;
      /* Protocol finished its task. It becomes the StartedProtocol. */
      Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
        DCM_SUPERVISOR_PROTOCOL_STARTED_IDLE;

      Dcm_Dsl_Supervisor_CommunicationSessionHandling_EndOfCommunicationSession(
        Dcm_ProtocolStatus[ProtocolIndex].RxConnectionId);
      break;

    case DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_ROE:
    case DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_PERIODIC:
      ResetMainConnection = TRUE;
      /* Protocol finished its task. It becomes the StartedProtocol. */
      Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
        DCM_SUPERVISOR_PROTOCOL_STARTED_IDLE;
      break;

    case DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE2_ROE:
    case DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE2_PERIODIC:
      /* Only reachable via TYPE2 Processing. No MainConnection to stop. */
      /* Protocol finished its task. It becomes the StartedProtocol. */
      Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
        DCM_SUPERVISOR_PROTOCOL_STARTED_IDLE;
      break;

    case DCM_SUPERVISOR_PROTOCOL_STOPPED_ACTIVE_TYPE2_ROE:
    case DCM_SUPERVISOR_PROTOCOL_STOPPED_ACTIVE_TYPE2_PERIODIC:
      /* Only reachable via TYPE2 Processing. No MainConnection to stop. */
      /* Protocol finished its task. It becomes the StartedProtocol. */
      Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
        DCM_SUPERVISOR_PROTOCOL_STOPPED_IDLE;
      break;

    case DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE2_ROE:

      /* Only reachable via TYPE2 Processing. No MainConnection to stop. */
      /* Protocol finished its task. It becomes the StartedProtocol. */
      Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
        DCM_SUPERVISOR_PROTOCOL_STARTED_IDLE;
      break;

    case DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE2_PERIODIC:

      /* Only reachable via TYPE2 Processing. No MainConnection to stop. */
      /* Protocol finished its task. It becomes the StartedProtocol. */
      Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
        DCM_SUPERVISOR_PROTOCOL_STARTED_IDLE;
      break;

    case DCM_SUPERVISOR_PROTOCOL_STOPPING_ACTIVE_TYPE2_ROE:
    case DCM_SUPERVISOR_PROTOCOL_STOPPING_ACTIVE_TYPE2_PERIODIC:

      /* Only reachable via TYPE2 Processing. No MainConnection to stop. */
      /* Protocol finished its task. It becomes the StartedProtocol. */
      Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
        DCM_SUPERVISOR_PROTOCOL_STOPPING_IDLE;
      break;
    case DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE2_ROE:

      /* Only reachable via TYPE2 Processing. No MainConnection to stop. */
      /* Protocol finished its task. It becomes the StartedProtocol. */
      Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
        DCM_SUPERVISOR_PROTOCOL_STOPPING_IDLE;
      break;
    case DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE2_PERIODIC:

      /* Only reachable via TYPE2 Processing. No MainConnection to stop. */
      /* Protocol finished its task. It becomes the StartedProtocol. */
      Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
        DCM_SUPERVISOR_PROTOCOL_STOPPING_IDLE;
      break;
    case DCM_SUPERVISOR_PROTOCOL_STOPPED_CANCELLING_TYPE2_ROE:

      /* Only reachable via TYPE2 Processing. No MainConnection to stop. */
      /* Protocol finished its task. It becomes the StartedProtocol. */
      Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
        DCM_SUPERVISOR_PROTOCOL_STOPPED_IDLE;
      break;
    case DCM_SUPERVISOR_PROTOCOL_STOPPED_CANCELLING_TYPE2_PERIODIC:

      /* Only reachable via TYPE2 Processing. No MainConnection to stop. */
      /* Protocol finished its task. It becomes the StartedProtocol. */
      Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
        DCM_SUPERVISOR_PROTOCOL_STOPPED_IDLE;
      break;

    case DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_EXTERNAL:
      ResetMainConnection = TRUE;
      /* Protocol finished its task. It becomes the StartedProtocol. */
      Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
        DCM_SUPERVISOR_PROTOCOL_STOPPING_IDLE;

      Dcm_Dsl_Supervisor_CommunicationSessionHandling_EndOfCommunicationSession(
        Dcm_ProtocolStatus[ProtocolIndex].RxConnectionId);
      break;

    case DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE1_PERIODIC:

      ResetMainConnection = TRUE;
      /* Protocol finished its task. It becomes the StartedProtocol. */
      Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
        DCM_SUPERVISOR_PROTOCOL_STOPPING_IDLE;
      break;
    case DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE1_ROE:
      ResetMainConnection = TRUE;
      /* Protocol finished its task. It becomes the StartedProtocol. */
      Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
        DCM_SUPERVISOR_PROTOCOL_STOPPING_IDLE;
      break;
    case DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_EXTERNAL:
      ResetMainConnection = TRUE;
      /* Protocol finished its task. It becomes the StartedProtocol. */
      Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
        DCM_SUPERVISOR_PROTOCOL_STARTED_IDLE;

      Dcm_Dsl_Supervisor_CommunicationSessionHandling_EndOfCommunicationSession(
        Dcm_ProtocolStatus[ProtocolIndex].RxConnectionId);
      break;
    case DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE1_ROE:
    case DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE1_PERIODIC:

      ResetMainConnection = TRUE;
      /* Protocol finished its task. It becomes the StartedProtocol. */
      Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
        DCM_SUPERVISOR_PROTOCOL_STARTED_IDLE;
      break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  if (TRUE == ResetMainConnection)
  {
    Dcm_RxPduIdInfoType RxPduIdInfo;
    Dcm_MainConnectionIndexType MainConnectionIndex;

    /* Now also reset the MainConnection state for the MainConnection
       to which the RxConnection causing this processing belonged. */
    Dcm_Dsl_RxConnectionGetRxPduIdInformation(Dcm_ProtocolStatus[ProtocolIndex].RxConnectionId, &RxPduIdInfo);
    MainConnectionIndex = RxPduIdInfo.MainConnectionIndex;

    Dcm_Dsl_Supervisor_MainConnectionHandling_ClearMainConnection(MainConnectionIndex);
  }

  /* And clean up Protocol state data. */
  Dcm_ProtocolStatus[ProtocolIndex].RxConnectionId = DCM_INVALID_USER_ID;
  Dcm_ProtocolStatus[ProtocolIndex].RequestType = DCM_INVALID_REQUEST_TYPE;
}

/* !LINKSTO Dcm.Dsn.IB.Supervisor.ProtocolHandling.TYPE1RequestPriorityAssessment,1 */
FUNC (Dcm_ReceptionArbitrationResultType, DCM_CODE)
  Dcm_Dsl_Supervisor_ProtocolHandling_TYPE1RequestPriorityAssessment(
    Dcm_ProtocolIndexType RequestedProtocolIndex,
    Dcm_RequestOriginType RequestOrigin
)
{
  /* The request can at least be accepted now for some processing. */
  boolean ProtocolPresumptive;
  boolean ProtocolBusy;
#if (DCM_DSP_USE_ROESERVICES == STD_ON)
  boolean ProtocolBusyOnOtherStack = FALSE;
#endif
  Dcm_ReceptionArbitrationResultType ArbitrationResult =
    DCM_SUPERVISOR_ARBITRATION_REJECT_PRIORITY;

  if (
      (DCM_EXTERNAL == RequestOrigin) ||
      (DCM_INTERNAL_POST_BOOT_NO_ANSWER == RequestOrigin) ||
      (DCM_INTERNAL_POST_BOOT_WITH_ANSWER == RequestOrigin)
     )
  {
    /* External and PostBoot requests cancel ROE ServiceToRespondTos and should be accepted. */
    ProtocolPresumptive = Dcm_Dsl_Supervisor_ProtocolHandling_ProtocolIsPresumptiveWithNonROE(RequestedProtocolIndex);
    ProtocolBusy = Dcm_Dsl_Supervisor_ProtocolHandling_ProtocolIsBusyWithNonROE(RequestedProtocolIndex);
  }
  else
  {
    /* Internal requests don't. ROE Processing counts here. */
    ProtocolPresumptive = Dcm_Dsl_Supervisor_ProtocolHandling_ProtocolIsPresumptive(RequestedProtocolIndex);
    ProtocolBusy = Dcm_Dsl_Supervisor_ProtocolHandling_ProtocolIsBusy(RequestedProtocolIndex);
  }

  /* Next, if the request is for a Protocol that's already busy (Active or Presumptive),
     this causes a rejection (which is Priority-based, i.e. request for a Protocol of
     the same Priority as the running one).
     Protocols which are Presumptive or Active with ROE requests do not count as blocking
     for external requests but do for internals. */

#if (DCM_DSP_USE_ROESERVICES == STD_ON)
    /* For TYPE1 ROE check if another protocol is busy processing a request on a different stack. */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1ROEEvent.NoDcmDslRoeConnectionRef.OBDAlreadyRunningOnDifferentStack.Postpone,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1ROEEvent.DcmDslRoeConnectionRef.OBDAlreadyRunningOnDifferentStack.Postpone,1 */
    ProtocolBusyOnOtherStack = Dcm_Dsl_Supervisor_ProtocolHandling_ProtocolOfDifferentTypeRunningOnDifferentStack(RequestedProtocolIndex);

  if(
     (TRUE == ProtocolPresumptive) ||
     (TRUE == ProtocolBusy) ||
     ((TRUE == ProtocolBusyOnOtherStack) && (DCM_INTERNAL_ROE == RequestOrigin))
    )
#else
  if(
     (TRUE == ProtocolPresumptive) ||
     (TRUE == ProtocolBusy)
    )
#endif /* #if (DCM_DSP_USE_ROESERVICES == STD_ON) */
  {
    ArbitrationResult = DCM_SUPERVISOR_ARBITRATION_REJECT_PRIORITY;
  }
  else
  {
    /* The RxProtocol is not busy. There's a limitation that if there's another protocol of the same
       kind running on a different stack, we need to decline. Of course, if this Protocol of the same
       kind would be running on this same stack, we'd simply follow prioritization rules.
     */
    const boolean ProtocolOfSameTypeRunningOnDifferentStack =
      Dcm_Dsl_Supervisor_ProtocolHandling_ProtocolOfSameTypeRunningOnDifferentStack(RequestedProtocolIndex);

    if (TRUE == ProtocolOfSameTypeRunningOnDifferentStack)
    {

      /* Protocol on different stack of same type running or presumptive found. Reject. */
      ArbitrationResult = DCM_SUPERVISOR_ARBITRATION_REJECT_SAME_PROTOCOL_TYPE;
    }
    else
    {
      /* Begin priority assessment. */
        Dcm_ProtocolStackNumberType ProcessingStackNumber =
          Dcm_ProtocolConfig[RequestedProtocolIndex].StackNumber;

      /* If there is already a PresumptiveProtocol on this stack, find it and
         compare priorities with it. If the current request has a lower priority,
         reject */

      /* Get current PresumptiveProtocol on current stack if there is one. */
      Dcm_ProtocolIndexType CurrentPresumptiveProtocolIndex =
        Dcm_Dsl_Supervisor_GetTYPE1PresumptiveProtocolOnStack(ProcessingStackNumber);

      if (DCM_INVALID_PROTOCOL_INDEX != CurrentPresumptiveProtocolIndex)
      {
        /* Presumptive Protocol rules:
           0. If there is already a Presumptive, than this Protocol, in order to become Presumptive,
              has already triggered any necessary actions on its stack, such as cancelling other running
              Protocols. This basically means that if there is a TYPE1 presumptive there, all TYPE1 processing
              Protocols on that stack are either getting cancelled or are not processing anything.
           1. Only TYPE1 Presumptives are considered.
           2. If there is no Presumptive, then accept the request.
           3. If there is a Presumptive, then:
             3.1 If the Presumptive has a lower priority than the RequestedProtocol, then:
               3.1.1 If the request is External or Periodic then ACCEPT
               3.1.2 If the request is ROE then REJECT
             3.2 If the Presumptive has a higher or equal priority to the RequestedProtocol, then:
               3.1.1 If the request is External or Periodic and the Presumptive is ROE then ACCEPT
               3.1.2 If the request is External and the Presumptive is External or Periodic then REJECT
               3.1.2 If the request is ROE then REJECT
        */

        if (Dcm_ProtocolConfig[CurrentPresumptiveProtocolIndex].Priority >
            Dcm_ProtocolConfig[RequestedProtocolIndex].Priority)
        {
          if (
              (DCM_EXTERNAL == RequestOrigin) ||
              (DCM_INTERNAL_POST_BOOT_NO_ANSWER == RequestOrigin) ||
              (DCM_INTERNAL_POST_BOOT_WITH_ANSWER == RequestOrigin) ||
              (DCM_INTERNAL_PERIODIC == RequestOrigin)
             )
          {
            /* If the request coming in is not an ROE request, accept it on the spot. */

            /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.RequestedProtocolNotRunningTYPE2ROE.BufferFree.AcceptRx,1 */
            /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.RequestedProtocolRunningTYPE2ROE.BufferFree.AcceptRx,1 */

            ArbitrationResult = DCM_SUPERVISOR_ARBITRATION_ACCEPT;
          }
          else if (
                   (DCM_INTERNAL_ROE == RequestOrigin) &&
                   (
                    (DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED !=
                     Dcm_ProtocolQueue[CurrentPresumptiveProtocolIndex].ProtocolQueueState) ||
                    (DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED_RCRRP_QUEUED !=
                     Dcm_ProtocolQueue[CurrentPresumptiveProtocolIndex].ProtocolQueueState) ||
                    (DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED_RCRRP_TRANSMITTING !=
                     Dcm_ProtocolQueue[CurrentPresumptiveProtocolIndex].ProtocolQueueState) ||
                    (DCM_SUPERVISOR_PROTOCOL_QUEUE_POST_BOOT_QUEUED !=
                     Dcm_ProtocolQueue[CurrentPresumptiveProtocolIndex].ProtocolQueueState)
                   )
                  )
          {
            /* If the request coming in is an ROE request, only accept it if the Presumptive
               isn't external. */
            ArbitrationResult = DCM_SUPERVISOR_ARBITRATION_ACCEPT;
          }
          else
          {

            /* ROEs are basically stopped by any Presumptive EXCEPT Periodic. */
            ArbitrationResult = DCM_SUPERVISOR_ARBITRATION_REJECT_PRIORITY;
          }
        }
        else
        {
          if (
              (DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE1_ROE_QUEUED ==
               Dcm_ProtocolQueue[CurrentPresumptiveProtocolIndex].ProtocolQueueState) &&
              (
               (DCM_EXTERNAL == RequestOrigin) ||
               (DCM_INTERNAL_POST_BOOT_NO_ANSWER == RequestOrigin) ||
               (DCM_INTERNAL_POST_BOOT_WITH_ANSWER == RequestOrigin)
              )
             )
          {

            /* If the Presumptive is ROE, its priority won't matter much... */

            /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolLowerPrio.CurrentProtocolRunningTYPE1ROE.DefaultSession.BufferFree.AcceptRx,1 */

            ArbitrationResult = DCM_SUPERVISOR_ARBITRATION_ACCEPT;
          }
          else
          {
            /* In all other cases reject. */
            ArbitrationResult = DCM_SUPERVISOR_ARBITRATION_REJECT_PRIORITY;
          }
        }
      }
      else
      {
        /* There is no PresumptiveProtocol. Time to consider the priority schema. */

        /* Get CurrentProtocol on the current stack if there is one. */
        Dcm_ProtocolIndexType CurrentProtocolIndex =
          Dcm_Dsl_Supervisor_ProtocolHandling_GetCurrentProtocolOnStack(ProcessingStackNumber);

        if (DCM_INVALID_PROTOCOL_INDEX != CurrentProtocolIndex)
        {
          /* There is a CurrentProtocol. See what it's doing.*/

          switch (Dcm_ProtocolStatus[CurrentProtocolIndex].ProtocolState)
          {
            /* First ActiveProtocols. If the CurrentProtocol is Active, if the priority
               of the CurrentProtocol is greater than or equal to the priority of the
               ProcessingProtocol AND the CurrentProtocol is not busy processing an ROE request,
               reject.
             */
            case DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_EXTERNAL:
            case DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_POST_BOOT:
              /* Active Cancelling Protocols remain the Active ones until they're done cancelling. But we make
                 a difference between non-ROE and ROE here. */
            case DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_EXTERNAL:
            case DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_EXTERNAL:
              if (
                  (
                   /* The requesting Protocol has a lower priority */
                   (Dcm_ProtocolConfig[CurrentProtocolIndex].Priority <=
                    Dcm_ProtocolConfig[RequestedProtocolIndex].Priority)
                  )
                  ||
                  /* The requesting Protocol is an ROE. */
                  (
                   (DCM_INTERNAL_ROE == RequestOrigin)
                  )
                 )
              {
                ArbitrationResult = DCM_SUPERVISOR_ARBITRATION_REJECT_PRIORITY;
              }
              else
              {
                /* Since the the RxProtocol cannot have a greater Priority than the
                   CurrentProtocol if they are one and the same, accept the request.
                 */

                /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.RequestedProtocolNotRunningTYPE2ROE.BufferFree.AcceptRx,1 */
                /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.RequestedProtocolRunningTYPE2ROE.BufferFree.AcceptRx,1 */

                ArbitrationResult = DCM_SUPERVISOR_ARBITRATION_ACCEPT;
              }
              break;
            case DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_PERIODIC:
            case DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE1_PERIODIC:
            case DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE1_PERIODIC:

              /* Active Periodic protocols can "fight" for priority with ROEs. */
              if (
                  /* The requesting Protocol has a lower priority*/
                  (Dcm_ProtocolConfig[CurrentProtocolIndex].Priority <=
                   Dcm_ProtocolConfig[RequestedProtocolIndex].Priority)
                 )
              {
                ArbitrationResult = DCM_SUPERVISOR_ARBITRATION_REJECT_PRIORITY;
              }
              else
              {
                /* Since the the RxProtocol cannot have a greater Priority than the
                   CurrentProtocol if they are one and the same, accept the request.
                   ROE can preempt Periodic if its priority is big enough.
                 */

                /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.RequestedProtocolNotRunningTYPE2ROE.BufferFree.AcceptRx,1 */
                /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.RequestedProtocolRunningTYPE2ROE.BufferFree.AcceptRx,1 */

                ArbitrationResult = DCM_SUPERVISOR_ARBITRATION_ACCEPT;
              }
              break;

            case DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_ROE:
            case DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE1_ROE:
            case DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE1_ROE:
              /* The CurrentProtocol is active processing a TYPE1 ROE request. In this
                 case...
               */
              if (
                  (DCM_EXTERNAL == RequestOrigin) ||
                  (DCM_INTERNAL_POST_BOOT_NO_ANSWER == RequestOrigin) ||
                  (DCM_INTERNAL_POST_BOOT_WITH_ANSWER == RequestOrigin)
                 )
              {
                const Dcm_SesCtrlType CurrentSession =
                        Dcm_Dsl_DiagnosticSessionHandler_GetCurrentSession();

                /* ... for an External or PostBoot request, give the permission for reception since
                   an external reception shall cancel an ROE processing UNLESS the session is the non-default
                   session. If the session is a non-default session, consider the priority schema since this
                   is the equivalent case of a request coming in while we have a current Protocol.
                 */
                if (
                    (DCM_DEFAULT_SESSION == CurrentSession) ||
                    (Dcm_ProtocolConfig[CurrentProtocolIndex].Priority >
                     Dcm_ProtocolConfig[RequestedProtocolIndex].Priority)
                   )
                {
                  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolLowerPrio.CurrentProtocolRunningTYPE1ROE.DefaultSession.BufferFree.AcceptRx,1 */
                  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.RequestedProtocolNotRunningTYPE2ROE.BufferFree.AcceptRx,1 */
                  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.RequestedProtocolRunningTYPE2ROE.BufferFree.AcceptRx,1 */
                  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.OnCurrentProtocol.RequestedProtocolRunningTYPE1ROE.BufferFree.AcceptRx,1 */

                  ArbitrationResult = DCM_SUPERVISOR_ARBITRATION_ACCEPT;
                }
                else
                {

                  ArbitrationResult = DCM_SUPERVISOR_ARBITRATION_REJECT_PRIORITY;
                }

              }
              else
              {

                /* An internal request, on the other hand, will consider priorities.
                   Note: since only one ROE instance is running, in case an ROE vs ROE
                   conflict appears, the prios will always be the same anyway. */
                if ((Dcm_ProtocolConfig[CurrentProtocolIndex].Priority <=
                     Dcm_ProtocolConfig[RequestedProtocolIndex].Priority))
                {
                  ArbitrationResult = DCM_SUPERVISOR_ARBITRATION_REJECT_PRIORITY;
                }
                else
                {
                  /* Since the the RxProtocol cannot have a greater Priority than the
                     CurrentProtocol if they are one and the same, accept the request.
                   */
                  ArbitrationResult = DCM_SUPERVISOR_ARBITRATION_ACCEPT;
                }
              }

              break;
              /* Now StartedProtocols. If the CurrentProtocol is Started, if the priority
                 of the CurrentProtocol is greater than or higher than the priority of the
                 RxProtocol AND the CurrentProtocol is not in a default session,
                 reject. The fact that a StartedProtocol might be running a ROE request makes
                 no difference here since the Started/Stopped state counts, and not the
                 ROE state. */
            case DCM_SUPERVISOR_PROTOCOL_STARTED_IDLE:
            case DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE2_ROE:
            case DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE2_PERIODIC:
              /* Until they finish stopping, stopping Protocols are still considered as Started... */
            case DCM_SUPERVISOR_PROTOCOL_STOPPING_ACTIVE_TYPE2_ROE:
            case DCM_SUPERVISOR_PROTOCOL_STOPPING_ACTIVE_TYPE2_PERIODIC:
            case DCM_SUPERVISOR_PROTOCOL_STOPPING_IDLE:
              /* Same goes for Protocols which are cancelling their TYPE2 operations, but were in a Started
                 state, whether they'll stay started... */
            case DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE2_ROE:
            case DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE2_PERIODIC:
              /* ... or Stop afterwards. */
            case DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE2_ROE:
            case DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE2_PERIODIC:
            {
              const Dcm_SesCtrlType CurrentSession =
                Dcm_Dsl_DiagnosticSessionHandler_GetCurrentSession();

              if ((Dcm_ProtocolConfig[CurrentProtocolIndex].Priority <=
                   Dcm_ProtocolConfig[RequestedProtocolIndex].Priority) &&
                  /* The request might be on the same Protocol. In this case this
                     rule is not taken into account. */
                  (CurrentProtocolIndex != RequestedProtocolIndex) &&
                  (DCM_DEFAULT_SESSION != CurrentSession)
                 )
              {
                ArbitrationResult = DCM_SUPERVISOR_ARBITRATION_REJECT_PRIORITY;
              }
              else
              {
                /* Accept request. If a preemption is needed and the StartedProtocol
                   needs to be cancelled, this will be handled elswhere */
                /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.OnCurrentProtocol.RequestedProtocolIdle.BufferFree.AcceptRx,1 */
                /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.OnCurrentProtocol.RequestedProtocolRunningTYPE2ROE.BufferFree.AcceptRx,1 */
                /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.RequestedProtocolNotRunningTYPE2ROE.BufferFree.AcceptRx,1 */
                /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.RequestedProtocolRunningTYPE2ROE.BufferFree.AcceptRx,1 */
                /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.NonDefaultSession.RequestedProtocolNotRunningTYPE2ROE.BufferFree.AcceptRx,1 */
                /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.NonDefaultSession.RequestedProtocolRunningTYPE2ROE.BufferFree.AcceptRx,1 */

                ArbitrationResult = DCM_SUPERVISOR_ARBITRATION_ACCEPT;
              }
              break;
            }

            /* CHECK: NOPARSE */
            default:
              /* Defensive branch. Default case at the end of the switch statement */
              DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
              break;
            /* CHECK: PARSE */
          }
        }
        else
        {
          /* No CurrentProtocol. Accept request. */

          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NoCurrentProtocol.RequestedProtocolNotRunningTYPE2ROE.BufferFree.AcceptRx,1 */

          /* Note that determination whether the Protocol is busy is done differently depending on whether the request is internal or external. */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NoCurrentProtocol.RequestedProtocolRunningTYPE2ROE.BufferFree.AcceptRx,1 */
          ArbitrationResult = DCM_SUPERVISOR_ARBITRATION_ACCEPT;
        }
      }
    }
  }

  return ArbitrationResult;
}

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON))
FUNC (Dcm_ReceptionArbitrationResultType, DCM_CODE)
  Dcm_Dsl_Supervisor_ProtocolHandling_TYPE2RequestPriorityAssessment(
    Dcm_ProtocolIndexType RequestedProtocolIndex
)
{
  Dcm_ReceptionArbitrationResultType ArbitrationResult =
    DCM_SUPERVISOR_ARBITRATION_REJECT_PRIORITY;

  if (DCM_SUPERVISOR_PROTOCOL_QUEUE_IDLE !=
     Dcm_ProtocolQueue[RequestedProtocolIndex].ProtocolQueueState)
  {
    ArbitrationResult = DCM_SUPERVISOR_ARBITRATION_REJECT_PRIORITY;
  }
  else
  {
    if (
        !(
          (DCM_SUPERVISOR_PROTOCOL_STOPPED_IDLE == Dcm_ProtocolStatus[RequestedProtocolIndex].ProtocolState) ||
          (DCM_SUPERVISOR_PROTOCOL_STARTED_IDLE == Dcm_ProtocolStatus[RequestedProtocolIndex].ProtocolState)
         )
       )
    {
      ArbitrationResult = DCM_SUPERVISOR_ARBITRATION_REJECT_PRIORITY;
    }
    else
    {
      ArbitrationResult = DCM_SUPERVISOR_ARBITRATION_ACCEPT;
    }
  }

  return ArbitrationResult;
}
#endif /* #if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON)) */

/* !LINKSTO Dcm.Dsn.IB.Supervisor.ProtocolHandling.CancelOngoingReceptionsOnLowerPriorityProtocolsOfSameStack,1 */
FUNC (void, DCM_CODE)
  Dcm_Dsl_Supervisor_ProtocolHandling_CancelOngoingReceptionsOnLowerPriorityProtocolsOfSameStack(
    Dcm_BufferUserType RxConnectionId
)
{
  uint16_least RxConnectionIndex;
  Dcm_RxPduIdInfoType RxPduIdInfo;
  Dcm_ProtocolPriorityType RxProtocolPriority;
  Dcm_StackNumberType ProcessingStackNumber;

  Dcm_Dsl_RxConnectionGetRxPduIdInformation(DCM_RX_CONNECTION_ID_FROM_INDEX(RxConnectionId), &RxPduIdInfo);

  ProcessingStackNumber = Dcm_ProtocolConfig[RxPduIdInfo.ProtocolIndex].StackNumber;

  RxProtocolPriority = Dcm_ProtocolConfig[RxPduIdInfo.ProtocolIndex].Priority;

  /* IDs may be used as indexes as for a particular type of User, they are consecutive. */
  /* Deviation TASKING-1 <+2> */
  for (RxConnectionIndex = 0;
       RxConnectionIndex < DCM_NUM_RX_CONNECTIONS;
       RxConnectionIndex++)
  {
    Dcm_StackNumberType ReceptionStackNumber;
    Dcm_Dsl_RxConnectionGetRxPduIdInformation(DCM_RX_CONNECTION_ID_FROM_INDEX(RxConnectionIndex), &RxPduIdInfo);
    ReceptionStackNumber = Dcm_ProtocolConfig[RxPduIdInfo.ProtocolIndex].StackNumber;

    if (
        (DCM_RX_CONNECTION_RECEIVING == Dcm_Dsl_RxConnectionGetState(DCM_RX_CONNECTION_ID_FROM_INDEX(RxConnectionIndex))) &&
        (Dcm_ProtocolConfig[RxPduIdInfo.ProtocolIndex].Priority >= RxProtocolPriority) &&
        (DCM_RX_CONNECTION_ID_FROM_INDEX(RxConnectionIndex) != RxConnectionId) &&
        (ProcessingStackNumber == ReceptionStackNumber)
       )
    {
      /* Cancellation shall be requested in sync with the MainFunction. */

      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.CancelReceptionsOnLowerPriorityProtocolsOnSameStack,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.CancelReceptionsOnLowerPriorityProtocolsOnSameStack,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.AcceptedForProcessing.CancelReceptionsOnLowerPriorityProtocolsOnSameStack,1 */
      Dcm_Dsl_Supervisor_CommunicationSessionHandling_EnqueueCancellationOfRxConnection(
          (Dcm_RxConnectionIndexType)(RxConnectionIndex)
        );
    }
  }
}

FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_ClearPresumptiveProtocol(
  Dcm_ProtocolIndexType ProtocolIndex,
  boolean RequestDispatched
)
{
  Dcm_ProtocolQueue[ProtocolIndex].RxConnectionId = DCM_INVALID_USER_ID;
  Dcm_ProtocolQueue[ProtocolIndex].PreemptionTimeout = 0U;
#if (DCM_HAS_ROE_TYPE2 == STD_ON)
  Dcm_ProtocolQueue[ProtocolIndex].ProcessingCancellingTimeout = 0U;
#endif /* #if (DCM_HAS_ROE_TYPE2 == STD_ON) */
  Dcm_ProtocolQueue[ProtocolIndex].RCRRPsSent = 0U;
  Dcm_ProtocolQueue[ProtocolIndex].RCRRPsConfirmed = 0U;

  if (FALSE == RequestDispatched)
  {
    /* Free any buffer we might have. A PresumptiveProtocol always has an RxBuffer
       allocated, even if it is virtual. */
    (void)Dcm_Dsl_BufferFree(DCM_SUPERVISOR_ID, Dcm_ProtocolQueue[ProtocolIndex].RequestBuffer.BufferId);

    /* If the Queue was busy with a post-boot response request, clear the request inhibition flag.*/
    if (DCM_SUPERVISOR_PROTOCOL_QUEUE_POST_BOOT_QUEUED == Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState)
    {
      Dcm_Dsl_Supervisor_DisinhibitRequestProcessing();
    }
  }

  Dcm_ProtocolQueue[ProtocolIndex].RequestType = DCM_INVALID_REQUEST_TYPE;

  Dcm_ProtocolQueue[ProtocolIndex].RequestBuffer.BufferId = DCM_BUFFER_INVALID_ID;

  Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState = DCM_SUPERVISOR_PROTOCOL_QUEUE_IDLE;

  Dcm_PresumptiveProtocolToBeStartedOrDispatched = FALSE;
}

FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_CancelTYPE1PresumptiveProtocolsOnSameStack(
  Dcm_ProtocolIndexType ProcessingProtocolIndex
)
{
  uint8_least ProtocolIndex;
  Dcm_ProtocolStackNumberType ProcessingStackNumber;

  /* Fetch the stack number of the Protocol. */
  ProcessingStackNumber = Dcm_ProtocolConfig[ProcessingProtocolIndex].StackNumber;

 /* Deviation TASKING-1 */
  for (ProtocolIndex = 0U; ProtocolIndex < DCM_NUM_PROTOCOLS; ProtocolIndex++)
  {
    if (
        /* Only work on Protocols of the given stack. */
        (Dcm_ProtocolConfig[ProtocolIndex].StackNumber == ProcessingStackNumber) &&
        (DCM_SUPERVISOR_PROTOCOL_QUEUE_IDLE != Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState) &&
        /* Allow TYPE2 Periodic and ROE requests to run in parallel. Don't cancel them.
           Presumptive ROE requests will be cancelled anyway in a later stage if needed. */
        (DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE2_PERIODIC_QUEUED != Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState) &&
        (DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE2_ROE_QUEUED != Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState)
       )
    {
      Dcm_Dsl_Supervisor_ProtocolHandling_CancelPresumptiveProtocol(
          (Dcm_ProtocolIndexType)(ProtocolIndex)
        );
    }
  }
}

FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_EnqueueAsPresumptive(
  Dcm_ProtocolIndexType ProcessingProtocolIndex,
  Dcm_BufferUserType RxConnectionId,
  Dcm_RequestTypeType RequestType,
  Dcm_BufferType RequestBuffer,
  boolean PreemptionNecessary
)
{
  Dcm_RxPduIdInfoType RxPduIdInfo;

  Dcm_Dsl_RxConnectionGetRxPduIdInformation(RxConnectionId, &RxPduIdInfo);

  DCM_PRECONDITION_ASSERT(DCM_INVALID_PROTOCOL_INDEX != ProcessingProtocolIndex,
                          DCM_INTERNAL_API_ID);

  if (TRUE == PreemptionNecessary)
  {
    /* The Preemption Timeout is only started if there is actually a preemption to perform */
    Dcm_ProtocolQueue[ProcessingProtocolIndex].PreemptionTimeout =
      Dcm_ProtocolConfig[ProcessingProtocolIndex].PreemptionTimeout;
  }
#if (DCM_HAS_ROE_TYPE2 == STD_ON)
  else
  {
    if (Dcm_ProtocolStatus[ProcessingProtocolIndex].ProtocolState ==
        DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE2_ROE)
    {
      /* The Cancelling Timeout is only started if there is actually a cancellation to perform */
      Dcm_ProtocolQueue[ProcessingProtocolIndex].ProcessingCancellingTimeout =
        Dcm_ProtocolConfig[ProcessingProtocolIndex].PreemptionTimeout;
    }
  }
#endif /* if (DCM_HAS_ROE_TYPE2 == STD_ON) */

  Dcm_ProtocolQueue[ProcessingProtocolIndex].RxConnectionId = RxConnectionId;

  switch (RequestType)
  {
    case DCM_NORMAL_PHYSICAL_REQUEST:
    case DCM_NORMAL_FUNCTIONAL_REQUEST:
      Dcm_ProtocolQueue[ProcessingProtocolIndex].ProtocolQueueState =
        DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED;
      /* MainConnection becomes busy with waiting. Clear the rest.  */
      Dcm_Dsl_Supervisor_MainConnectionHandling_BusyWithRequest(
        RxPduIdInfo.MainConnectionIndex,
        RxConnectionId);
      break;

    case DCM_JUMP_FROM_BL_RESPONSE_REQUEST:
    case DCM_JUMP_FROM_BL_NO_RESPONSE_REQUEST:
      Dcm_ProtocolQueue[ProcessingProtocolIndex].ProtocolQueueState =
        DCM_SUPERVISOR_PROTOCOL_QUEUE_POST_BOOT_QUEUED;
      /* MainConnection becomes busy with waiting this request. Clear the rest.  */
      Dcm_Dsl_Supervisor_MainConnectionHandling_BusyWithRequest(
        RxPduIdInfo.MainConnectionIndex,
        RxConnectionId);
      break;

    case DCM_ROE_TYPE1_PHYSICAL_REQUEST:
    case DCM_ROE_TYPE1_FUNCTIONAL_REQUEST:
      Dcm_ProtocolQueue[ProcessingProtocolIndex].ProtocolQueueState =
        DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE1_ROE_QUEUED;
      /* MainConnection becomes busy with waiting for the processing of this request. Clear the rest.  */
      Dcm_Dsl_Supervisor_MainConnectionHandling_BusyWithRequest(
        RxPduIdInfo.MainConnectionIndex,
        RxConnectionId);
      break;

    case DCM_PERIODIC_TYPE1_REQUEST:
      Dcm_ProtocolQueue[ProcessingProtocolIndex].ProtocolQueueState =
        DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE1_PERIODIC_QUEUED;
      Dcm_Dsl_Supervisor_MainConnectionHandling_BusyWithRequest(
        RxPduIdInfo.MainConnectionIndex,
        RxConnectionId);
      break;

    case DCM_ROE_TYPE2_PHYSICAL_REQUEST:
    case DCM_ROE_TYPE2_FUNCTIONAL_REQUEST:
      Dcm_ProtocolQueue[ProcessingProtocolIndex].ProtocolQueueState =
        DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE2_ROE_QUEUED;
      break;

    case DCM_PERIODIC_TYPE2_REQUEST:
      Dcm_ProtocolQueue[ProcessingProtocolIndex].ProtocolQueueState =
        DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE2_PERIODIC_QUEUED;
      break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */

  }

  Dcm_ProtocolQueue[ProcessingProtocolIndex].RequestType = RequestType;
  Dcm_ProtocolQueue[ProcessingProtocolIndex].RCRRPsSent = 0U;
  Dcm_ProtocolQueue[ProcessingProtocolIndex].RCRRPsConfirmed = 0U;
  Dcm_ProtocolQueue[ProcessingProtocolIndex].RequestBuffer = RequestBuffer;

  Dcm_PresumptiveProtocolToBeStartedOrDispatched = TRUE;

}

/* !LINKSTO Dcm.Dsn.IB.Supervisor.ProtocolHandling.CancelROEProcessingOnAllProtocols,1 */
FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_CancelROEProcessingOnAllProtocols(
  void
)
{
  uint8_least ProtocolIndex;

  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.TYPE1ROE.CancelProcessing,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.TYPE1ROE.CancelProcessing,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.TYPE1ROE.CancelTransmission,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.TYPE1ROE.CancelTransmission,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.TYPE2ROE.CancelProcessing,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.TYPE2ROE.CancelProcessing,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.TYPE2ROE.CancelTransmission,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.TYPE2ROE.CancelTransmission,1 */

  /* First Presumptives with ROE. */
  /* Deviation TASKING-1 */
  for (ProtocolIndex = 0U; ProtocolIndex < DCM_NUM_PROTOCOLS; ProtocolIndex++)
  {
    if (
        (DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE2_ROE_QUEUED == Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState) ||
        (DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE1_ROE_QUEUED == Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState)
       )
    {
      Dcm_Dsl_Supervisor_ProtocolHandling_CancelPresumptiveProtocol(
          (Dcm_ProtocolIndexType)(ProtocolIndex)
        );
    }
  }

  /* Now for Protocols which are currently running ROE requests. Cancel them but do
     not change their CurrentProtocol status. */
  /* Deviation TASKING-1 */
  for (ProtocolIndex = 0U; ProtocolIndex < DCM_NUM_PROTOCOLS; ProtocolIndex++)
  {
    switch (Dcm_ProtocolStatus[ProtocolIndex].ProtocolState)
    {
      case DCM_SUPERVISOR_PROTOCOL_STOPPED_ACTIVE_TYPE2_ROE:
        /* Give Cancel signal to Protocol. */
        Dcm_Dsp_ProtocolCancel(DCM_PROTOCOL_ID_FROM_INDEX(ProtocolIndex));
        Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
          DCM_SUPERVISOR_PROTOCOL_STOPPED_CANCELLING_TYPE2_ROE;
        break;

      case DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_ROE:

        /* Give Cancel signal to Protocol. */
        Dcm_Dsp_ProtocolCancel(DCM_PROTOCOL_ID_FROM_INDEX(ProtocolIndex));
        Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
          DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE1_ROE;
        break;

      case DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE2_ROE:

        /* Give Cancel signal to Protocol. */
        Dcm_Dsp_ProtocolCancel(DCM_PROTOCOL_ID_FROM_INDEX(ProtocolIndex));
        Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
          DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE2_ROE;
        break;

      case DCM_SUPERVISOR_PROTOCOL_STOPPING_ACTIVE_TYPE2_ROE:

        /* Give Cancel signal to Protocol. */
        Dcm_Dsp_ProtocolCancel(DCM_PROTOCOL_ID_FROM_INDEX(ProtocolIndex));
        Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
          DCM_SUPERVISOR_PROTOCOL_STOPPING_CANCELLING_TYPE2_ROE;
        break;

      default:
        /* Ignore other protocol states and types. */
        break;
    }
  }
}

/* !LINKSTO Dcm.Dsn.IB.Supervisor.ProtocolHandling.CancelUDSProtocolsInNonDefaultSession,1 */
FUNC (void, DCM_CODE)
  Dcm_Dsl_Supervisor_ProtocolHandling_CancelUDSProtocolsInNonDefaultSession(
    void
)
{
  uint8_least ProtocolIndex;

  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.OBDRequest.NonDefaultSession.ExternalUDS.CancelProcessing,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.OBDRequest.NonDefaultSession.ExternalUDS.CancelProcessing,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.OBDRequest.NonDefaultSession.ExternalUDS.CancelTransmission,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.OBDRequest.NonDefaultSession.ExternalUDS.CancelTransmission,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.OBDRequest.NonDefaultSession.ApplicationUDS.CancelProcessing,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.OBDRequest.NonDefaultSession.ApplicationUDS.CancelProcessing,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.OBDRequest.NonDefaultSession.ApplicationUDS.CancelTransmission,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.OBDRequest.NonDefaultSession.ApplicationUDS.CancelTransmission,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.OBDRequest.NonDefaultSession.TYPE1Periodic.CancelTransmission,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.OBDRequest.NonDefaultSession.TYPE1Periodic.CancelTransmission,1 */

  /* Start with presumptives for UDS. */
  Dcm_Dsl_Supervisor_ProtocolHandling_CancelPresumptiveUDSProtocolsInNonDefaultSession();

  /* And go on with running UDS Protocols. */
  /* Deviation TASKING-1 */
  for (ProtocolIndex = 0U; ProtocolIndex < DCM_NUM_PROTOCOLS; ProtocolIndex++)
  {
    const Dcm_ProtocolGroupType ProtocolGroup =
      Dcm_Dsl_Supervisor_ProtocolHandling_GetProtocolGroupTypeForProtocol(
          (Dcm_ProtocolIndexType)(ProtocolIndex)
        );

    if(DCM_PROTOCOL_UDS_GROUP == ProtocolGroup)
    {
      if (
          (Dcm_ProtocolStatus[ProtocolIndex].ProtocolState ==
            DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_EXTERNAL) ||
          (Dcm_ProtocolStatus[ProtocolIndex].ProtocolState ==
            DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_ROE) ||
          (Dcm_ProtocolStatus[ProtocolIndex].ProtocolState ==
            DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_PERIODIC) ||
          (Dcm_ProtocolStatus[ProtocolIndex].ProtocolState ==
            DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE2_ROE) ||
          (Dcm_ProtocolStatus[ProtocolIndex].ProtocolState ==
            DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE2_PERIODIC) ||
          (Dcm_ProtocolStatus[ProtocolIndex].ProtocolState ==
            DCM_SUPERVISOR_PROTOCOL_STOPPED_ACTIVE_TYPE2_ROE) ||
          (Dcm_ProtocolStatus[ProtocolIndex].ProtocolState ==
            DCM_SUPERVISOR_PROTOCOL_STOPPED_ACTIVE_TYPE2_PERIODIC)
         )
      {
        /* Give Cancel signal to Protocol. */
        Dcm_Dsp_ProtocolCancel(DCM_PROTOCOL_ID_FROM_INDEX(ProtocolIndex));
      }

      /* Change the state of protocol */
      switch (Dcm_ProtocolStatus[ProtocolIndex].ProtocolState)
      {
        case DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_EXTERNAL:

          /* Active Protocols stay started */
          Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
            DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_EXTERNAL;
          break;

        case DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_ROE:

          /* Active Protocols stay started */
          Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
            DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE1_ROE;
          break;

        case DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_PERIODIC:

          /* Active Protocols stay started */
          Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
            DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE1_PERIODIC;
          break;

        case DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE2_ROE:

          Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
            DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE2_ROE;
          break;

        case DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE2_PERIODIC:

          Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
            DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE2_PERIODIC;
          break;

        case DCM_SUPERVISOR_PROTOCOL_STOPPED_ACTIVE_TYPE2_ROE:

          Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
            DCM_SUPERVISOR_PROTOCOL_STOPPED_CANCELLING_TYPE2_ROE;
          break;

        case DCM_SUPERVISOR_PROTOCOL_STOPPED_ACTIVE_TYPE2_PERIODIC:

          Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
            DCM_SUPERVISOR_PROTOCOL_STOPPED_CANCELLING_TYPE2_PERIODIC;
          break;

        default:
          /*
          That's all possible processing states. Don't do anything for protocols which
          are not running.
          */
        break;
      }
    }
  }
}

FUNC (Dcm_ProtocolIndexType, DCM_CODE)
  Dcm_Dsl_Supervisor_ProtocolHandling_GetLatestExternalUDSProtocolIndex(
    void
)
{
  return Dcm_LatestExternalUDSProtocolIndex;
}

/* !LINKSTO Dcm.Dsn.IB.Supervisor.ProtocolHandling.P2TimerTimeout,1 */
FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_P2TimerTimeout(
  Dcm_TimerIndexType TimerIndex
)
{

  /* TimerIndex handle defined by the Supervisor too so this is OK. */

  /* Every Protocol has a P2Timer. A P2Timer is started when we get a Presumptive on the Protocol and is stopped
     by either:
     - a transmission
     - a preemption */

  Dcm_ProtocolIndexType ProtocolIndex = TimerIndex;
  Dcm_NegativeResponseCodeType NRCToSend;
  uint16 Max_RCRRP;

  DCM_INVARIANT_ASSERT(
    (
     (DCM_NORMAL_PHYSICAL_REQUEST == Dcm_ProtocolQueue[ProtocolIndex].RequestType) ||
     (DCM_NORMAL_FUNCTIONAL_REQUEST == Dcm_ProtocolQueue[ProtocolIndex].RequestType)
    ), DCM_INTERNAL_API_ID);

  /* ENTER critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

#if ( (DCM_DSP_USE_SERVICE_0X01 == STD_ON) || (DCM_DSP_USE_SERVICE_0X02 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X03 == STD_ON) || (DCM_DSP_USE_SERVICE_0X06 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X07 == STD_ON) || (DCM_DSP_USE_SERVICE_0X08 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X0A == STD_ON) )

  if ( (DCM_DSP_OBD_SERVICE_0X01 != Dcm_ProtocolQueue[ProtocolIndex].RequestBuffer.BufferPtr[0U]) &&
       (DCM_DSP_OBD_SERVICE_0X02 != Dcm_ProtocolQueue[ProtocolIndex].RequestBuffer.BufferPtr[0U]) &&
       (DCM_DSP_OBD_SERVICE_0X03 != Dcm_ProtocolQueue[ProtocolIndex].RequestBuffer.BufferPtr[0U]) &&
       (DCM_DSP_OBD_SERVICE_0X06 != Dcm_ProtocolQueue[ProtocolIndex].RequestBuffer.BufferPtr[0U]) &&
       (DCM_DSP_OBD_SERVICE_0X07 != Dcm_ProtocolQueue[ProtocolIndex].RequestBuffer.BufferPtr[0U]) &&
       (DCM_DSP_OBD_SERVICE_0X08 != Dcm_ProtocolQueue[ProtocolIndex].RequestBuffer.BufferPtr[0U]) &&
       (DCM_DSP_OBD_SERVICE_0X0A != Dcm_ProtocolQueue[ProtocolIndex].RequestBuffer.BufferPtr[0U]) )
  {
#endif /* #if ( (DCM_DSP_USE_SERVICE_0X01 == STD_ON) || (DCM_DSP_USE_SERVICE_0X02 == STD_ON) || \
                (DCM_DSP_USE_SERVICE_0X03 == STD_ON) || (DCM_DSP_USE_SERVICE_0X06 == STD_ON) || \
                (DCM_DSP_USE_SERVICE_0X07 == STD_ON) || (DCM_DSP_USE_SERVICE_0X08 == STD_ON) || \
                (DCM_DSP_USE_SERVICE_0X0A == STD_ON) ) */

    Max_RCRRP = Dcm_Dsl_DiagnosticSessionHandler_GetSessionMaxRcrrp(ProtocolIndex);
    if ((Max_RCRRP == Dcm_ProtocolQueue[ProtocolIndex].RCRRPsSent) &&
        (DCM_DIAG_DSL_INFINITY_LIMIT_RESP_PEND != Max_RCRRP))
    {
      /* This Protocol has already sent the maximum allowed number of RCRRPs. Here comes
         a GeneralReject. Allowed timeout for this is of course zero. This function is called
         synchronously with the Dcm_MainFunction() so transmission of the NRC shall be immediate.
       */
#if ((DCM_DSP_USE_SERVICE_0X04 == STD_ON) || (DCM_DSP_USE_SERVICE_0X09 == STD_ON))
       /* for the $04 and $09 the NRC GeneralReject is not supported according to
          the Table 10, Table 11, Table 16 from ISO 15031-5- 2011-04-15 */
      if ((DCM_SID_CLEAR_DTC != Dcm_ProtocolQueue[ProtocolIndex].RequestBuffer.BufferPtr[0U]) &&
          (DCM_SID_REQUEST_VEHICLE_INFORMATION != Dcm_ProtocolQueue[ProtocolIndex].RequestBuffer.BufferPtr[0U]))
      {
#endif
        /* !LINKSTO Dcm.EB.ResponseHandling.ExternalRequest.MaxNumOfRCRRP.GeneralReject,3 */
        /* !LINKSTO Dcm.EB.ResponseHandling.ApplicationRequest.MaxNumOfRCRRP.GeneralReject,3 */
        NRCToSend = DCM_E_GENERALREJECT;
#if ((DCM_DSP_USE_SERVICE_0X04 == STD_ON) || (DCM_DSP_USE_SERVICE_0X09 == STD_ON))
      }
      else
      {
        /* !LINKSTO Dcm.EB.ResponseHandling.OBD0x04ExternalRequest.MaxNumOfRCRRP.ConditionsNotCorrect,1 */
        /* !LINKSTO Dcm.EB.ResponseHandling.OBD0x04ApplicationRequest.MaxNumOfRCRRP.ConditionsNotCorrect,1 */
        /* !LINKSTO Dcm.EB.ResponseHandling.OBD0x09ExternalRequest.MaxNumOfRCRRP.ConditionsNotCorrect,1 */
        /* !LINKSTO Dcm.EB.ResponseHandling.OBD0x09ApplicationRequest.MaxNumOfRCRRP.ConditionsNotCorrect,1 */
        NRCToSend = DCM_E_CONDITIONSNOTCORRECT;
      }
#endif
      /* Free P2Timer, it is not running. */
      (void)Dcm_Dsl_DiagnosticSessionHandler_FreeP2Timer(DCM_SUPERVISOR_ID,
                                                         ProtocolIndex);
    }
    else
    {
      /* Initiate a RCRRP Transmission. This function is called synchronously with
         the Dcm_MainFunction() so transmission of the NRC shall be immediate. */

      /* !LINKSTO Dcm.EB.ResponseHandling.ExternalRequest.FirstRCRRP,3 */
      /* !LINKSTO Dcm.EB.ResponseHandling.ApplicationRequest.FirstRCRRP,3 */
      /* !LINKSTO Dcm.EB.ResponseHandling.ExternalRequest.SubsequentRCRRP,1 */
      /* !LINKSTO Dcm.EB.ResponseHandling.ApplicationRequest.SubsequentRCRRP,1 */
      NRCToSend = DCM_E_REQUESTCORRECTLYRECEIVEDRESPONSEPENDING;

      Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState = DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED_RCRRP_QUEUED;
      Dcm_PresumptiveProtocolToBeStartedOrDispatched = TRUE;
    }

    /* We avoid calling PduR APIs from an exclusive area and we allow all transmission to be done by the Supervisor. */
    Dcm_Dsl_Supervisor_MainConnectionHandling_InitiateNRC(
        Dcm_ProtocolQueue[ProtocolIndex].RxConnectionId,
        Dcm_ProtocolQueue[ProtocolIndex].RequestBuffer,
        NRCToSend,
        0U
      );

    /* If maximum number of RCRRP has been reached presumptive protocol needs to be cleared. */
#if (DCM_DSP_USE_SERVICE_0X04 == STD_ON)
    if ( (NRCToSend == DCM_E_GENERALREJECT) || (NRCToSend == DCM_E_CONDITIONSNOTCORRECT) )
#else
    if (NRCToSend == DCM_E_GENERALREJECT)
#endif
    {
      /* No use keeping this Protocol as Presumptive anymore. */
      Dcm_Dsl_Supervisor_ProtocolHandling_ClearPresumptiveProtocol(ProtocolIndex, FALSE);
    }

#if ( (DCM_DSP_USE_SERVICE_0X01 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X02 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X03 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X06 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X07 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X08 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X0A == STD_ON) )
  }
  else
  {
    /* Get the main connection index */
    Dcm_RxPduIdInfoType RxPduIdInfo;
    Dcm_Dsl_RxConnectionGetRxPduIdInformation(Dcm_ProtocolQueue[ProtocolIndex].RxConnectionId,
                                              &RxPduIdInfo);

    /* !LINKSTO Dcm.EB.ResponseHandling.OBD0x03ExternalRequest.P2Timeout.Discard,1 */
    /* !LINKSTO Dcm.EB.ResponseHandling.OBD0x07ExternalRequest.P2Timeout.Discard,1 */
    /* !LINKSTO Dcm.EB.ResponseHandling.OBD0x0AExternalRequest.P2Timeout.Discard,1 */
    /* !LINKSTO Dcm.EB.ResponseHandling.OBD0x01ExternalRequest.P2Timeout.Discard,1 */
    /* !LINKSTO Dcm.EB.ResponseHandling.OBD0x02ExternalRequest.P2Timeout.Discard,1 */
    /* !LINKSTO Dcm.EB.ResponseHandling.OBD0x06ExternalRequest.P2Timeout.Discard,1 */
    /* !LINKSTO Dcm.EB.ResponseHandling.OBD0x08ExternalRequest.P2Timeout.Discard,1 */
    /* !LINKSTO Dcm.EB.ResponseHandling.OBD0x03ApplicationRequest.P2Timeout.Discard,1 */
    /* !LINKSTO Dcm.EB.ResponseHandling.OBD0x07ApplicationRequest.P2Timeout.Discard,1 */
    /* !LINKSTO Dcm.EB.ResponseHandling.OBD0x0AApplicationRequest.P2Timeout.Discard,1 */
    /* !LINKSTO Dcm.EB.ResponseHandling.OBD0x01ApplicationRequest.P2Timeout.Discard,1 */
    /* !LINKSTO Dcm.EB.ResponseHandling.OBD0x02ApplicationRequest.P2Timeout.Discard,1 */
    /* !LINKSTO Dcm.EB.ResponseHandling.OBD0x06ApplicationRequest.P2Timeout.Discard,1 */
    /* !LINKSTO Dcm.EB.ResponseHandling.OBD0x08ApplicationRequest.P2Timeout.Discard,1 */
    Dcm_Dsl_Supervisor_MainConnectionHandling_End(RxPduIdInfo.MainConnectionIndex);

    /* No use keeping this Protocol as Presumptive anymore. */
    Dcm_Dsl_Supervisor_ProtocolHandling_ClearPresumptiveProtocol(ProtocolIndex, FALSE);

    /* Free P2Timer, it is not running. */
    (void)Dcm_Dsl_DiagnosticSessionHandler_FreeP2Timer(DCM_SUPERVISOR_ID,
                                                       ProtocolIndex);
  }
#endif

  /* EXIT critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
}

FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_RCRRPSent(
  Dcm_ProtocolIndexType ProtocolIndex
)
{
  DBG_DCM_DSL_SUPERVISOR_PROTOCOLHANDLING_RCRRPSENT_ENTRY(ProtocolIndex);

  DCM_PRECONDITION_ASSERT(Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState
                          == DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED_RCRRP_QUEUED,
                          DCM_INTERNAL_API_ID);

  /* Increment number of RCRRPs which have been sent. */
  Dcm_ProtocolQueue[ProtocolIndex].RCRRPsSent++;

  /* Move Protocol to necessary state. */
  Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState =
      DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED_RCRRP_TRANSMITTING;

  DBG_DCM_DSL_SUPERVISOR_PROTOCOLHANDLING_RCRRPSENT_EXIT(
     Dcm_ProtocolQueue[ProtocolIndex].RCRRPsSent);
}

FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_RCRRPTransmissionFinished(
  Dcm_ProtocolIndexType ProtocolIndex
)
{
  DBG_DCM_DSL_SUPERVISOR_PROTOCOLHANDLING_RCRRPTRANSMISSIONFINISHED_ENTRY(ProtocolIndex);

  /* Instead of incrementing RCRRPsConfirmed counter decrement RCRRPsSent counter. */
  if ((DCM_DIAG_DSL_INFINITY_LIMIT_RESP_PEND ==
       Dcm_Dsl_DiagnosticSessionHandler_GetSessionMaxRcrrp(ProtocolIndex)) &&
      (Dcm_ProtocolQueue[ProtocolIndex].RCRRPsSent ==
       DCM_DIAG_DSL_INFINITY_LIMIT_RESP_PEND))
  {
    /* Decrement number of RCRRPs which have been sent. */
    Dcm_ProtocolQueue[ProtocolIndex].RCRRPsSent--;
  }
  else
  {
    /* Increment number of RCRRPs which have been confirmed. */
    Dcm_ProtocolQueue[ProtocolIndex].RCRRPsConfirmed++;
  }
  /* Protocol goes back to external-queued state. */
  Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState =
      DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED;

  Dcm_PresumptiveProtocolToBeStartedOrDispatched = TRUE;

  DBG_DCM_DSL_SUPERVISOR_PROTOCOLHANDLING_RCRRPTRANSMISSIONFINISHED_EXIT
     (Dcm_ProtocolQueue[ProtocolIndex].RCRRPsConfirmed,
      Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState);
}

FUNC (boolean, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_ProtocolIsBusyWithType1ROE(
  Dcm_ProtocolIndexType ProtocolIndex
)
{
  boolean ProtocolBusyWithType1ROE = FALSE;

  if (
      (TRUE == Dcm_Dsl_Supervisor_ProtocolHandling_ProtocolIsBusy(ProtocolIndex)) &&
      ((DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_ROE ==
          Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
       (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE1_ROE ==
          Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
       (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE1_ROE ==
          Dcm_ProtocolStatus[ProtocolIndex].ProtocolState))
     )
  {
    ProtocolBusyWithType1ROE = TRUE;
  }

  return ProtocolBusyWithType1ROE;
}

FUNC (void, DCM_CODE)
  Dcm_Dsl_Supervisor_ProtocolHandling_PresumptiveProtocolEnd(
    Dcm_ProtocolIndexType TYPE1PresumptiveProtocolIndex
)
{
    /* Stop and release P2Timer if needed */
    if (
        (DCM_NORMAL_PHYSICAL_REQUEST == Dcm_ProtocolQueue[TYPE1PresumptiveProtocolIndex].RequestType) ||
        (DCM_NORMAL_FUNCTIONAL_REQUEST == Dcm_ProtocolQueue[TYPE1PresumptiveProtocolIndex].RequestType)
       )
    {
      /* ...as this Protocol's P2Timer was running since when it became Presumptive. Stop it. */
      (void)Dcm_Dsl_DiagnosticSessionHandler_StopTimer(DCM_SUPERVISOR_ID,
                                                       TYPE1PresumptiveProtocolIndex,
                                                       DCM_P2TIMER);

      (void)Dcm_Dsl_DiagnosticSessionHandler_FreeP2Timer(DCM_SUPERVISOR_ID,
                                                         TYPE1PresumptiveProtocolIndex);
    }

    /* ... and then drop the request. */
    Dcm_Dsl_Supervisor_ProtocolHandling_ClearPresumptiveProtocol(TYPE1PresumptiveProtocolIndex, FALSE);
}

#if (DCM_META_DATA_HANDLING_ENABLED == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.Supervisor.ProtocolHandling.MetaDataInfoHandling,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_MetaDataInfoHandling(
  Dcm_ProtocolIndexType PresumptiveProtocolIndex,
  Dcm_MainConnectionIndexType ReceivedMainConnectionIndex)
{
  Dcm_MainConnectionIndexType MainConnId;
  Dcm_BufferUserType RxConnId;

  /* If Main Connection Index is not provided, obtain it from RxPduIdConfig.
   * This is used when the function is called from ProtocolHandling. */
  if (DCM_NUM_MAIN_CONNECTIONS == ReceivedMainConnectionIndex)
  {
    /* Main Connection Index is not valid. Update it from RxPduIdConfig.*/
    RxConnId = Dcm_ProtocolQueue[PresumptiveProtocolIndex].RxConnectionId;
    MainConnId = Dcm_RxPduIdConfig[RxConnId].MainConnectionIndex;
  }
  else
  {
    /* The received Main Connection Index is valid.*/
    MainConnId = ReceivedMainConnectionIndex;
    RxConnId = Dcm_Dsl_Supervisor_MainConnectionHandling_GetRxConnectionId(MainConnId);
  }
  if (TRUE == Dcm_MainConnectionConfig[MainConnId].MetaDataIsConfigured)
  {

    Std_ReturnType GetMetaDataInfo = Dcm_Dsl_GetEcuCMetaDataInfo(RxConnId);
    Dcm_GenericConnectionIndexType GenericConnIdx = Dcm_MainConnectionConfig[MainConnId].GenericConnectionIndex;

    if (DCM_E_OK != GetMetaDataInfo)
    {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
      /* !LINKSTO Dcm.EB.LowerLayerInterfaces.DcmGenericConnectionUsage.GetMetaDataSourceAddr.ReturnsNotE_OK.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.LowerLayerInterfaces.DcmGenericConnectionUsage.GetMetaDataTargetAddr.ReturnsNotE_OK.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.LowerLayerInterfaces.DcmGenericConnectionUsage.StartProtocol.ReturnsNotE_OK.GetMetaDataSourceAddr.ReturnsNotE_OK.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.LowerLayerInterfaces.DcmGenericConnectionUsage.StartProtocol.ReturnsNotE_OK.GetMetaDataTargetAddr.ReturnsNotE_OK.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.LowerLayerInterfaces.DcmGenericConnectionUsage.StopProtocol.ReturnsNotE_OK.GetMetaDataSourceAddr.ReturnsNotE_OK.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.LowerLayerInterfaces.DcmGenericConnectionUsage.StopProtocol.ReturnsNotE_OK.GetMetaDataTargetAddr.ReturnsNotE_OK.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.LowerLayerInterfaces.DcmGenericConnectionUsage.TransmitBRR.GetMetaDataSourceAddr.ReturnsNotE_OK.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.LowerLayerInterfaces.DcmGenericConnectionUsage.TransmitBRR.GetMetaDataTargetAddr.ReturnsNotE_OK.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.LowerLayerInterfaces.DcmGenericConnectionUsage.TransmitRCRRP.GetMetaDataSourceAddr.ReturnsNotE_OK.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.LowerLayerInterfaces.DcmGenericConnectionUsage.TransmitRCRRP.GetMetaDataTargetAddr.ReturnsNotE_OK.ReportDETError,1 */
      DCM_DET_REPORT_ERROR(DCM_INTERNAL_API_ID, DCM_E_INTERFACE_RETURN_VALUE);
#endif
    }
    else
    {
#if (DCM_HAS_ROE_TYPE2 == STD_ON)
      Dcm_RoeConnectionIndexType RoeConnIdx;
#endif /* #if (DCM_HAS_ROE_TYPE2 == STD_ON) */
#if (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON)
      Dcm_PeriodicTransmissionIndexType PeriodicTranIdx;
#endif /* #if (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) */

#if (DCM_HAS_ROE_TYPE2 == STD_ON)
      RoeConnIdx = Dcm_MainConnectionConfig[MainConnId].RoeConnectionIndex;
      if ( (DCM_INVALID_ROE_CONNECTION != RoeConnIdx) &&
           (TRUE == Dcm_RoeConnectionConfig[RoeConnIdx].Type2PduIdsPresent) &&
           (0U != Dcm_RoeConnectionConfig[RoeConnIdx].GenericConnectionIndex) )
      {
        Dcm_PduInfo[Dcm_RoeConnectionConfig[RoeConnIdx].GenericConnectionIndex] = Dcm_PduInfo[GenericConnIdx];
      }
#endif /* #if (DCM_HAS_ROE_TYPE2 == STD_ON) */
#if (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON)
      PeriodicTranIdx = Dcm_MainConnectionConfig[MainConnId].PeriodicTransmissionIndex;
      if (DCM_INVALID_PERIODIC_TRANSMISSION != PeriodicTranIdx)
      {
        if (0U != Dcm_PeriodicTransmissionConfig[PeriodicTranIdx].NumberOfPeriodicConnections)
        {
          const Dcm_PeriodicConnectionIndexType FirstPeriodicIdx =
              Dcm_PeriodicTransmissionConfig[PeriodicTranIdx].IndexOfFirstPeriodicConnection;
          const Dcm_PeriodicConnectionIndexType LastPeriodicConnIdx =
              FirstPeriodicIdx + Dcm_PeriodicTransmissionConfig[PeriodicTranIdx].NumberOfPeriodicConnections;

          Dcm_PeriodicConnectionIndexType PerTransIdx;

          /* Go through all connections. */
          for (PerTransIdx = FirstPeriodicIdx; PerTransIdx < LastPeriodicConnIdx; PerTransIdx++)
          {
            /* If it has metadata, retrieve the index. */
            if (0U != Dcm_PeriodicConnectionConfig[PerTransIdx].GenericConnectionIndex)
            {
              Dcm_PduInfo[Dcm_PeriodicConnectionConfig[PerTransIdx].GenericConnectionIndex] =
                  Dcm_PduInfo[GenericConnIdx];
            }
          }
        }
      }
#endif /* #if (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) */

    }
  }
  else
  {
    /* GenericConnection is not enabled for this connection. */
  }
}
#endif /* #if (DCM_META_DATA_HANDLING_ENABLED == STD_ON) */

#define DCM_STOP_SEC_CODE
#include <MemMap.h>
/*==================[internal function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <MemMap.h>
STATIC FUNC (boolean, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_ProtocolIsPresumptive(
  Dcm_ProtocolIndexType ProtocolIndex
)
{
  boolean ProtocolPresumptive = FALSE;
  /* Can be optimized via bitmasking. */
  if (
      (DCM_SUPERVISOR_PROTOCOL_QUEUE_POST_BOOT_QUEUED ==
        Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState) ||
      (DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED ==
        Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState) ||
      (DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED_RCRRP_QUEUED ==
        Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState) ||
      (DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED_RCRRP_TRANSMITTING ==
        Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState) ||
      (DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE1_PERIODIC_QUEUED ==
        Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState) ||
      (DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE2_PERIODIC_QUEUED ==
        Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState) ||
      (DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE1_ROE_QUEUED ==
        Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState) ||
      (DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE2_ROE_QUEUED ==
        Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState)
     )
  {
    ProtocolPresumptive = TRUE;
  }

  return ProtocolPresumptive;
}

STATIC FUNC (boolean, DCM_CODE)
  Dcm_Dsl_Supervisor_ProtocolHandling_ProtocolIsPresumptiveWithNonROE(
    Dcm_ProtocolIndexType ProtocolIndex
)
{
  boolean ProtocolPresumptiveWithNonROE = FALSE;

  if (
      (TRUE == Dcm_Dsl_Supervisor_ProtocolHandling_ProtocolIsPresumptive(ProtocolIndex)) &&
      (DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE1_ROE_QUEUED != Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState) &&
      (DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE2_ROE_QUEUED != Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState)
     )
  {
    ProtocolPresumptiveWithNonROE = TRUE;
  }

  return ProtocolPresumptiveWithNonROE;
}

STATIC FUNC (boolean, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_ProtocolIsBusy(
  Dcm_ProtocolIndexType ProtocolIndex
)
{
  boolean ProtocolBusy = FALSE;
  /* Can be optimized via bitmasking. */

  /* If the protocol is cancelling anyway, we're just gonna let it finish. */
  if (
      (DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_POST_BOOT == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
      (DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_EXTERNAL == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||

      (DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_ROE == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
      (DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_PERIODIC == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||

      (DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE2_ROE == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState)||
      (DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE2_PERIODIC == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||

      (DCM_SUPERVISOR_PROTOCOL_STOPPING_ACTIVE_TYPE2_ROE == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState)||
      (DCM_SUPERVISOR_PROTOCOL_STOPPING_ACTIVE_TYPE2_PERIODIC == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||

      (DCM_SUPERVISOR_PROTOCOL_STOPPED_ACTIVE_TYPE2_ROE == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState)||
      (DCM_SUPERVISOR_PROTOCOL_STOPPED_ACTIVE_TYPE2_PERIODIC == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState)
     )
  {
    ProtocolBusy = TRUE;
  }

  return ProtocolBusy;
}

STATIC FUNC (boolean, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_ProtocolIsBusyWithNonROE(
  Dcm_ProtocolIndexType ProtocolIndex
)
{
  boolean ProtocolBusyWithNonROE = FALSE;

  if (
      (TRUE == Dcm_Dsl_Supervisor_ProtocolHandling_ProtocolIsBusy(ProtocolIndex)) &&
      (DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_ROE != Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) &&
      (DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE2_ROE != Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) &&
      (DCM_SUPERVISOR_PROTOCOL_STOPPED_ACTIVE_TYPE2_ROE != Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) &&
      (DCM_SUPERVISOR_PROTOCOL_STOPPING_ACTIVE_TYPE2_ROE != Dcm_ProtocolStatus[ProtocolIndex].ProtocolState)
     )
  {
    ProtocolBusyWithNonROE = TRUE;
  }

  return ProtocolBusyWithNonROE;
}

STATIC FUNC (Dcm_ProtocolIndexType, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_GetCurrentProtocolOnStack(
  Dcm_ProtocolStackNumberType StackNumber
)
{
  uint8_least ProtocolIndex;

  /* Deviation TASKING-1 */
  for (ProtocolIndex = 0U; ProtocolIndex < DCM_NUM_PROTOCOLS; ProtocolIndex++)
  {
    if (
        (StackNumber == Dcm_ProtocolConfig[ProtocolIndex].StackNumber)&&
        (
         /* Protocol is the Current one, Active or Started. Cancelling protocols won't count since
            if a request arrives it'll wait patiently in a Presumptive state. */
         (DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_POST_BOOT == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
         (DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_EXTERNAL == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||

         (DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_ROE == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
         (DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_PERIODIC == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||

         (DCM_SUPERVISOR_PROTOCOL_STARTED_IDLE == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
         (DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE2_ROE == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
         (DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE2_PERIODIC == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
         /* Started and Active Protocols which are undergoing cancellation are still the current protocols
            until the cancellation is done and they are stopped, if this ever happens. */
         (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_EXTERNAL == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
         (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE1_ROE == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
         (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE1_PERIODIC == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||

         (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_EXTERNAL == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
         (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE1_ROE == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
         (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE1_PERIODIC == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||

         (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE2_ROE == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
         (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE2_PERIODIC == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
         (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE2_ROE == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
         (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE2_PERIODIC == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||

          /* As are Protocols which are undergoing stopping but are still not stopped. */
         (DCM_SUPERVISOR_PROTOCOL_STOPPING_ACTIVE_TYPE2_ROE == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
         (DCM_SUPERVISOR_PROTOCOL_STOPPING_ACTIVE_TYPE2_PERIODIC == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||

         (DCM_SUPERVISOR_PROTOCOL_STOPPING_IDLE == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState)

        )
       )
    {
      break;
    }
  }

  return (Dcm_ProtocolIndexType)(ProtocolIndex);
}

STATIC FUNC (boolean, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_StackCanAcceptTYPE1Request(
  Dcm_StackNumberType StackNumber
)
{
  /* Checks whether a new TYPE1 request can be injected into this stack.

     This is possible when the stack is in a stable state. The stack is in a stable state when:
     - No External or TYPE1 internal request is running anymore
     - No External or TYPE1 internal request is being cancelled
     - No formerly Active Protocol is being cancelled. TYPE2 Periodics which are being cancelled are accepted.
     - No TYPE2 ROE is being cancelled (ServiceToRespondTos are cancelled)
     - TYPE2s running on the stack are ignored mostly
     - No Protocol is Stopping, as this would mean that the makeup of CurrentProtocols is being changed.
   */
  uint8_least ProtocolIndex;
  boolean StackCanAcceptRequest = TRUE; /* It's better if we search for a NO condition at least for now. OPTIMIZE this */

  /* Deviation TASKING-1 */
  for (ProtocolIndex = 0U;
       ProtocolIndex < DCM_NUM_PROTOCOLS;
       ProtocolIndex++)
  {
    if (StackNumber == Dcm_ProtocolConfig[ProtocolIndex].StackNumber)
    {
      if (
          /* No TYPE1 request is running in the stack. */
          (DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_POST_BOOT ==
            Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
          (DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_EXTERNAL ==
            Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
          (DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_ROE ==
            Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
          (DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_PERIODIC ==
            Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||

          /* No TYPE1 request is being cancelled in the stack. */

          /* The regular preemption case, in which a higher prio Protocol
             preempted and cancelled a running Protocol on the same Stack. */
          (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_EXTERNAL ==
            Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
          (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE1_ROE ==
            Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
          (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE1_PERIODIC ==
            Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
          /* The case in which an OBD request on another stack while in non-default
             session caused this cancellation. Also the case for TYPE1 ROE when it
             was cancelled by an external or Periodic request on the same Protocol. */
          /* First when the request is being run on the current Protocol. */
          (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE1_ROE ==
            Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
          (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE1_PERIODIC ==
            Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
          (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_EXTERNAL ==
            Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||

          /* No TYPE2 requests processed on a current Protocol */
          (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE2_ROE ==
            Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
          (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE2_PERIODIC ==
            Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||

          /* And for TYPE2 requests processed on a non-current Protocol. */
          (DCM_SUPERVISOR_PROTOCOL_STOPPED_CANCELLING_TYPE2_ROE ==
            Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
          (DCM_SUPERVISOR_PROTOCOL_STOPPED_CANCELLING_TYPE2_PERIODIC ==
            Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||

          /* The previous cancellation scenario, but followed immediately by another request
             on this stack which also causes preemption. */
          (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE2_ROE ==
            Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
          (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE2_PERIODIC ==
            Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
          /* Any preemption ongoing. */
          (DCM_SUPERVISOR_PROTOCOL_STOPPING_IDLE ==
            Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
          /* Stopping the CurrentProtocol while it's running a TYPE2 request. The TYPE2
             request is allowed to run in the background though. Also a preemption scenario. */
          (DCM_SUPERVISOR_PROTOCOL_STOPPING_ACTIVE_TYPE2_ROE ==
            Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
          (DCM_SUPERVISOR_PROTOCOL_STOPPING_ACTIVE_TYPE2_PERIODIC ==
            Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||

          /* Or the same with TYPE2s cancelling. */
          (DCM_SUPERVISOR_PROTOCOL_STOPPING_CANCELLING_TYPE2_ROE ==
            Dcm_ProtocolStatus[ProtocolIndex].ProtocolState)

         )
      {
        /* Operations ongoing on this stack preclude an external request starting.
           The stack is not stabilized yet. */
        StackCanAcceptRequest = FALSE;
      }
    }
  }

  return StackCanAcceptRequest;
}

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON))
STATIC FUNC (boolean, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_ProtocolCanAcceptTYPE2Request(
  Dcm_ProtocolIndexType ProtocolIndex
)
{
  /* Checks whehter a new TYPE2 request can be injected into this Protocol.

     This is possible when the Protocol is in an idle state.
   */
  boolean ProtocolCanAcceptRequest = FALSE; /* It's better if we search for a NO condition. */

  if (
      (DCM_SUPERVISOR_PROTOCOL_STARTED_IDLE ==
        Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
      (DCM_SUPERVISOR_PROTOCOL_STOPPED_IDLE ==
        Dcm_ProtocolStatus[ProtocolIndex].ProtocolState)
      )
    {

      /* Protocol can accept TYPE2 request. */
      ProtocolCanAcceptRequest = TRUE;
    }

  return ProtocolCanAcceptRequest;
}
#endif /*#if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON)) */


STATIC FUNC (Dcm_ProtocolIndexType, DCM_CODE) Dcm_Dsl_Supervisor_GetTYPE1PresumptiveProtocolOnStack(
  Dcm_StackNumberType StackNumber
)
{
  /* Checks whehter a stack has a TYPE1 Presumptive.
   */
  uint8_least ProtocolIndex;
  Dcm_ProtocolIndexType FoundProtocol = DCM_INVALID_PROTOCOL_INDEX;

  /* Deviation TASKING-1 */
  for (ProtocolIndex = 0U;
       ProtocolIndex < DCM_NUM_PROTOCOLS;
       ProtocolIndex++)
  {
    if (StackNumber == Dcm_ProtocolConfig[ProtocolIndex].StackNumber)
    {
      if (
          (DCM_SUPERVISOR_PROTOCOL_QUEUE_POST_BOOT_QUEUED ==
            Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState) ||
          (DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED ==
            Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState) ||
          (DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED_RCRRP_QUEUED ==
            Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState) ||
          (DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED_RCRRP_TRANSMITTING ==
            Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState) ||
          (DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE1_PERIODIC_QUEUED ==
            Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState) ||
          (DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE1_ROE_QUEUED ==
            Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState)
         )
      {
        FoundProtocol = (Dcm_ProtocolIndexType)ProtocolIndex;
        break;
      }
    }
  }

  return FoundProtocol;
}

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON))
STATIC FUNC (Dcm_ProtocolIndexType, DCM_CODE) Dcm_Dsl_Supervisor_GetTYPE2PresumptiveProtocolOnStack(
  Dcm_StackNumberType StackNumber
)
{
  /* Checks whehter a stack has a TYPE2 Presumptive.
   */
  uint8_least ProtocolIndex;
  Dcm_ProtocolIndexType FoundProtocol = DCM_INVALID_PROTOCOL_INDEX;

  /* Deviation TASKING-1 */
  for (ProtocolIndex = 0U;
       ProtocolIndex < DCM_NUM_PROTOCOLS;
       ProtocolIndex++)
  {
    if (StackNumber == Dcm_ProtocolConfig[ProtocolIndex].StackNumber)
    {

      if (
          (DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE2_PERIODIC_QUEUED == Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState) ||
          (DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE2_ROE_QUEUED == Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState)
         )
      {
        FoundProtocol = (Dcm_ProtocolIndexType)ProtocolIndex;
        break;
      }
    }
  }

  return FoundProtocol;
}
#endif /* #if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON)) */

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON))
STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_MakeTYPE2ProtocolProcess(
  Dcm_ProtocolIndexType ProtocolIndex
)
{
  /* A tad more complicated than making a Started Protocol active, a TYPE2
     Protocol may start processing in other states as well... */

  /* Only still states (non-processing) may go on to process a TYPE2 request.
     There's precisely 2 of these. */
  switch (Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState)
  {
    case DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE2_ROE_QUEUED:
      if (DCM_SUPERVISOR_PROTOCOL_STOPPED_IDLE ==
           Dcm_ProtocolStatus[ProtocolIndex].ProtocolState)
      {
        Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
          DCM_SUPERVISOR_PROTOCOL_STOPPED_ACTIVE_TYPE2_ROE;
      }
      else if (DCM_SUPERVISOR_PROTOCOL_STARTED_IDLE ==
                Dcm_ProtocolStatus[ProtocolIndex].ProtocolState)
      {
        Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
          DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE2_ROE;
      }
      /* CHECK: NOPARSE */
      else
      {
        DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      }
      /* CHECK: PARSE */

      break;

    case DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE2_PERIODIC_QUEUED:
      if (DCM_SUPERVISOR_PROTOCOL_STOPPED_IDLE ==
           Dcm_ProtocolStatus[ProtocolIndex].ProtocolState)
      {
        Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
          DCM_SUPERVISOR_PROTOCOL_STOPPED_ACTIVE_TYPE2_PERIODIC;
      }
      else if (DCM_SUPERVISOR_PROTOCOL_STARTED_IDLE ==
                Dcm_ProtocolStatus[ProtocolIndex].ProtocolState)
      {
        Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
          DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE2_PERIODIC;
      }
      /* CHECK: NOPARSE */
      else
      {
        DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      }
      /* CHECK: PARSE */

      break;

    /* CHECK: NOPARSE */
    default:
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }
}
#endif /* #if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON)) */

STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_MakeStartedProtocolActive(
  Dcm_ProtocolIndexType ProtocolIndex
)
{
  /* Only a stopped Protocol will ever be started, and only by a TYPE1 request. */

  DCM_PRECONDITION_ASSERT(DCM_SUPERVISOR_PROTOCOL_STARTED_IDLE ==
                            Dcm_ProtocolStatus[ProtocolIndex].ProtocolState,
                          DCM_INTERNAL_API_ID);

  switch (Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState)
  {
    case DCM_SUPERVISOR_PROTOCOL_QUEUE_POST_BOOT_QUEUED:
      Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
        DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_POST_BOOT;
      break;

    case DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED:
      Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
        DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_EXTERNAL;
      break;

    case DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED_RCRRP_QUEUED:
    case DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED_RCRRP_TRANSMITTING:

      Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
        DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_EXTERNAL;
      break;

    case DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE1_ROE_QUEUED:
      Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
        DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_ROE;
      break;

    case DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE1_PERIODIC_QUEUED:
      Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
        DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_PERIODIC;
      break;

    /* CHECK: NOPARSE */
    default:
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }
}

STATIC FUNC (boolean, DCM_CODE)
  Dcm_Dsl_Supervisor_ProtocolHandling_ProtocolOfSameTypeRunningOnDifferentStack(
    Dcm_ProtocolIndexType RxProtocolIndex
)
{
  uint8_least ProtocolIndex;
  /* Fetch the stack number of the Protocol. */
  const Dcm_ProtocolStackNumberType StackNumber = Dcm_ProtocolConfig[RxProtocolIndex].StackNumber;

  /* Fetch the current Protocol type (UDS group or OBD group). */
  const Dcm_ProtocolGroupType RxProtocolGroup =
    Dcm_Dsl_Supervisor_ProtocolHandling_GetProtocolGroupTypeForProtocol(RxProtocolIndex);

  boolean ProtocolOfSameTypeRunningOnDifferentStack = FALSE;

  /* Search on all other stacks whether there is a Protocol of the same type already running or presumptive. */
  /* TYPE2s don't count as they're allowed, by definition, to run in parallel. */
  /* Deviation TASKING-1 */
  for (ProtocolIndex = 0; ProtocolIndex < DCM_NUM_PROTOCOLS; ProtocolIndex++)
  {
    const Dcm_ProtocolGroupType OtherProtocolGroup =
      Dcm_Dsl_Supervisor_ProtocolHandling_GetProtocolGroupTypeForProtocol(
          (Dcm_ProtocolIndexType)(ProtocolIndex)
        );
    if (
        /* Protocol is on a different stack... */
        (Dcm_ProtocolConfig[ProtocolIndex].StackNumber != StackNumber) &&
        (OtherProtocolGroup == RxProtocolGroup) &&
        (
         (
          /* ... being active... */
          (DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_POST_BOOT == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
          (DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_EXTERNAL == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
          (DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_ROE == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
          (DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_PERIODIC == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
          (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_EXTERNAL == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
          (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE1_ROE == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
          (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE1_PERIODIC == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
          (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_EXTERNAL == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
          (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE1_ROE == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
          (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE1_PERIODIC == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState)
         )
         ||
         (
          /*... or being presumptive.*/
          (DCM_SUPERVISOR_PROTOCOL_QUEUE_POST_BOOT_QUEUED ==
            Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState) ||
          (DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED ==
            Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState) ||
          (DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED_RCRRP_QUEUED ==
            Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState) ||
          (DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED_RCRRP_TRANSMITTING ==
            Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState) ||
          (DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE1_ROE_QUEUED ==
            Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState) ||
          (DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE1_PERIODIC_QUEUED ==
            Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState)
          /* TYPE2 requests may run in parallel, so they won't matter. */
         )
        )
       )
    {

      ProtocolOfSameTypeRunningOnDifferentStack = TRUE;
      break;
    }
  }

  return ProtocolOfSameTypeRunningOnDifferentStack;
}

STATIC FUNC (Std_ReturnType, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_MainConnNotBusy_StackCanAcceptRequest(
    Dcm_StackNumberType StackNumber,
    P2VAR(Dcm_ProtocolIndexType, AUTOMATIC, DCM_VAR) ProtocolIndex,
    boolean RequestIsType1
)
{
  Std_ReturnType RequestCanBeAccepted = DCM_E_ABORT;
  boolean MainConnectionIsBusyWithNrc;

  if (TRUE == RequestIsType1)
  {
    /* Obtain the protocol index. */
   *ProtocolIndex = Dcm_Dsl_Supervisor_GetTYPE1PresumptiveProtocolOnStack(StackNumber);
    /* If the protocol index is invalid, do nothing. */
    if (DCM_INVALID_PROTOCOL_INDEX != *ProtocolIndex)
    {
      MainConnectionIsBusyWithNrc =
        Dcm_Dsl_Supervisor_MainConnectionHandling_IsBusyWithNRC(Dcm_ProtocolQueue[*ProtocolIndex].RxConnectionId);

      if ((TRUE == Dcm_Dsl_Supervisor_ProtocolHandling_StackCanAcceptTYPE1Request(StackNumber)) &&
          (FALSE == MainConnectionIsBusyWithNrc))
      {
        RequestCanBeAccepted = DCM_E_OK;
      }
      else
      {
        RequestCanBeAccepted = DCM_E_NOT_OK;
      }

    }
  }
#if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON))
  else
  {
    /* Obtain the protocol index for type 2 protocol. */
    *ProtocolIndex = Dcm_Dsl_Supervisor_GetTYPE2PresumptiveProtocolOnStack(StackNumber);
    /* If the protocol index is invalid, do nothing. */
    if (DCM_INVALID_PROTOCOL_INDEX != *ProtocolIndex)
    {
       if (TRUE == Dcm_Dsl_Supervisor_ProtocolHandling_ProtocolCanAcceptTYPE2Request(*ProtocolIndex))
       {
         RequestCanBeAccepted = DCM_E_OK;
       }
       else
       {
         RequestCanBeAccepted = DCM_E_NOT_OK;
       }

    }
  }
#endif /* #if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON)) */

  return RequestCanBeAccepted;
}

#if (DCM_DSP_USE_ROESERVICES == STD_ON)
STATIC FUNC (boolean, DCM_CODE)
  Dcm_Dsl_Supervisor_ProtocolHandling_ProtocolOfDifferentTypeRunningOnDifferentStack(
    Dcm_ProtocolIndexType RxProtocolIndex
)
{
  uint8_least ProtocolIndex;
  /* Fetch the stack number of the Protocol. */
  const Dcm_ProtocolStackNumberType StackNumber = Dcm_ProtocolConfig[RxProtocolIndex].StackNumber;

  /* Fetch the current Protocol type (UDS group or OBD group). */
  const Dcm_ProtocolGroupType RxProtocolGroup =
    Dcm_Dsl_Supervisor_ProtocolHandling_GetProtocolGroupTypeForProtocol(RxProtocolIndex);

  boolean ProtocolOfSameTypeRunningOnDifferentStack = FALSE;

  /* Search for busy protocols of different type on other stacks. */
  /* TYPE2s don't count as they're allowed, by definition, to run in parallel. */
  /* Deviation TASKING-1 */
  for (ProtocolIndex = 0; ProtocolIndex < DCM_NUM_PROTOCOLS; ProtocolIndex++)
  {
    const Dcm_ProtocolGroupType OtherProtocolGroup =
      Dcm_Dsl_Supervisor_ProtocolHandling_GetProtocolGroupTypeForProtocol(
          (Dcm_ProtocolIndexType)(ProtocolIndex)
        );
    if (
        /* Protocol is on a different stack... */
        (Dcm_ProtocolConfig[ProtocolIndex].StackNumber != StackNumber) &&
        (OtherProtocolGroup != RxProtocolGroup) &&
        (
         (
          /* ... being active... */
          (DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_POST_BOOT == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
          (DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_EXTERNAL == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
          (DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_ROE == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
          (DCM_SUPERVISOR_PROTOCOL_STARTED_ACTIVE_TYPE1_PERIODIC == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
          (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_EXTERNAL == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
          (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE1_ROE == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
          (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STOP_TYPE1_PERIODIC == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
          (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_EXTERNAL == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
          (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE1_ROE == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
          (DCM_SUPERVISOR_PROTOCOL_STARTED_CANCELLING_STAY_STARTED_TYPE1_PERIODIC == Dcm_ProtocolStatus[ProtocolIndex].ProtocolState)
         )
         ||
         (
          /*... or being presumptive.*/
          (DCM_SUPERVISOR_PROTOCOL_QUEUE_POST_BOOT_QUEUED ==
            Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState) ||
          (DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED ==
            Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState) ||
          (DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED_RCRRP_QUEUED ==
            Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState) ||
          (DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED_RCRRP_TRANSMITTING ==
            Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState) ||
          (DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE1_ROE_QUEUED ==
            Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState) ||
          (DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE1_PERIODIC_QUEUED ==
            Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState)
          /* TYPE2 requests may run in parallel, so they won't matter. */
         )
        )
       )
    {

      ProtocolOfSameTypeRunningOnDifferentStack = TRUE;
      break;
    }
  }

  return ProtocolOfSameTypeRunningOnDifferentStack;
}
#endif /* #if (DCM_DSP_USE_ROESERVICES == STD_ON) */

STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_GiveTYPE1RequestBufferToProtocol(
  Dcm_SupervisorProtocolQueueStateType ProtocolQueueState,
  Dcm_BufferUserType ProtocolToStartId,
  Dcm_BufferIdType BufferId
)
{
  switch (ProtocolQueueState)
  {
    case DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED:
    case DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED_RCRRP_QUEUED:
    case DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED_RCRRP_TRANSMITTING:

      (void)Dcm_Dsl_BufferChangeState(DCM_SUPERVISOR_ID,
                                      DCM_BUFFER_PROCESSING_NORMAL,
                                      BufferId);

      break;

    case DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE1_ROE_QUEUED:
      (void)Dcm_Dsl_BufferChangeState(DCM_SUPERVISOR_ID,
                                      DCM_BUFFER_PROCESSING_ROE,
                                      BufferId);

      break;

    case DCM_SUPERVISOR_PROTOCOL_QUEUE_POST_BOOT_QUEUED:
      /* The Protocol isn't given a normal buffer here therefore there
         is no state change is necessary*/
    case DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE1_PERIODIC_QUEUED:
      /* The Protocol isn't given a normal buffer here therefore there
         is no state change is necessary*/
      break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */

  }

  /* Change the owner of the reception buffer to the Protocol. */
  (void)Dcm_Dsl_BufferGive(DCM_SUPERVISOR_ID,
                           ProtocolToStartId,
                           BufferId);
}

/* !LINKSTO Dcm.Dsn.IB.Supervisor.ProtocolHandling.DispatchTYPE1RequestToProtocol,1 */
STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_DispatchTYPE1RequestToProtocol(
  Dcm_ProtocolIndexType ProtocolIndex
)
{
  Dcm_RxPduIdInfoType RxPduIdInfo;
  Dcm_BufferUserType TxConnectionId;
  PduIdType ConfirmationTxPduId;
  Dcm_ProtocolRCRRPTransmissionType ProtocolRCRRPTransmission = DCM_PROTOCOL_RCRRP_INVALID;

  Dcm_Dsl_RxConnectionGetRxPduIdInformation(
    Dcm_ProtocolQueue[ProtocolIndex].RxConnectionId,
    &RxPduIdInfo);

  /* Get the Tx connection related to the main connection */
  ConfirmationTxPduId = Dcm_MainConnectionConfig[RxPduIdInfo.MainConnectionIndex].ConfirmationTxPduId;
  /* Translate the ConfirmationTxPduID into a TxConnectionId */
  TxConnectionId = DCM_TX_CONNECTION_ID_FROM_INDEX(ConfirmationTxPduId);

  /* Dispatches the Presumptive of the given Protocol. A Presumptive must exist (asserted by the default
     case). */

  /* For TYPE1 requests, the Protocol's state has already been changed to the corresponding Active state
     by the act of Starting the Protocol!*/

  /* Give the RxBuffer to the Protocol */
  Dcm_Dsl_Supervisor_ProtocolHandling_GiveTYPE1RequestBufferToProtocol(
      Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState,
      DCM_PROTOCOL_ID_FROM_INDEX(ProtocolIndex),
      Dcm_ProtocolQueue[ProtocolIndex].RequestBuffer.BufferId
    );

  switch (Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState)
  {

    case DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED:
      /* Give P2Timer to Protocol. */

      (void)Dcm_Dsl_DiagnosticSessionHandler_GiveP2Timer(DCM_SUPERVISOR_ID,
                                                         DCM_PROTOCOL_ID_FROM_INDEX(ProtocolIndex),
                                                         ProtocolIndex);

      ProtocolRCRRPTransmission = DCM_PROTOCOL_NO_RCRRP_ON_ENTRY;

      break;

    case DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED_RCRRP_QUEUED:
    case DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED_RCRRP_TRANSMITTING:

      /* Give P2Timer to Protocol. */
      (void)Dcm_Dsl_DiagnosticSessionHandler_GiveP2Timer(DCM_SUPERVISOR_ID,
                                                         DCM_PROTOCOL_ID_FROM_INDEX(ProtocolIndex),
                                                         ProtocolIndex);

      if (DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED_RCRRP_QUEUED == Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState)
      {
        /* At the end of this function the MainConnection is reset, therefore
           it will not retry to send the RCRRP in the next cycle, but leave it to the Protocol
           to attempt this. */

        /* Dispatch request to Protocol as RCRRP pending transmit. */
        ProtocolRCRRPTransmission = DCM_PROTOCOL_BEGIN_TO_TRANSMIT_RCRRP_ON_ENTRY;
      }
      else
      {
        DCM_PRECONDITION_ASSERT(
            Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState ==
                DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED_RCRRP_TRANSMITTING,
            DCM_INTERNAL_API_ID
          );

        /* Dispatch request to Protocol as RCRRP beeing sent, but not confirmed. */
        ProtocolRCRRPTransmission = DCM_PROTOCOL_TRANSMITTING_RCRRP_ON_ENTRY;
      }
      break;

    case DCM_SUPERVISOR_PROTOCOL_QUEUE_POST_BOOT_QUEUED:
      /* There is no P2Timer to give here and no RCRRP transmission to handle */
    case DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE1_ROE_QUEUED:
      /* There is no P2Timer to give here and no RCRRP transmission to handle */
    case DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE1_PERIODIC_QUEUED:
      /* There is no P2Timer to give here and no RCRRP transmission to handle */
      break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */

  }

  Dcm_ProtocolStatus[ProtocolIndex].RequestType = Dcm_ProtocolQueue[ProtocolIndex].RequestType;
  Dcm_ProtocolStatus[ProtocolIndex].RxConnectionId = Dcm_ProtocolQueue[ProtocolIndex].RxConnectionId;

  Dcm_Dsl_Supervisor_MainConnectionHandling_RequestDispatchedToProtocol(RxPduIdInfo.MainConnectionIndex);

  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NoCurrentProtocol.StartProtocol.ReturnsE_OK.ProcessRequest,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NoCurrentProtocol.StartProtocol.ReturnsE_OK.ProcessRequest,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.WarmStart.AcceptedForProcessing.NoCurrentProtocol.StartProtocol.ReturnsE_OK.ProcessRequest,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NoCurrentProtocol.StartProtocol.ReturnsE_OK.ProcessRequest,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NoCurrentProtocol.StartProtocol.ReturnsE_OK.ProcessRequest,1 */

  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.OnCurrentProtocol.ProcessRequest,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.OnCurrentProtocol.ProcessRequest,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.OnCurrentProtocol.ProcessRequest,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.OnCurrentProtocol.ProcessRequest,1 */

  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StartProtocol.ReturnsE_OK.ProcessRequest,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StartProtocol.ReturnsE_OK.ProcessRequest,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StartProtocol.ReturnsE_OK.ProcessRequest,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StartProtocol.ReturnsE_OK.ProcessRequest,1 */

  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StartProtocol.ReturnsE_OK.ProcessRequest,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StartProtocol.ReturnsE_OK.ProcessRequest,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StartProtocol.ReturnsE_OK.ProcessRequest,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StartProtocol.ReturnsE_OK.ProcessRequest,1 */

  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.StartProtocol.ReturnsE_OK.ProcessRequest,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.StartProtocol.ReturnsE_OK.ProcessRequest,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusyTYPE1Periodic.StartProtocol.ReturnsE_OK.ProcessRequest,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusyTYPE1Periodic.StartProtocol.ReturnsE_OK.ProcessRequest,1 */

  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForDispatching.OBDRequestUDSAlreadyRunningOnDifferentStack.ProcessInParallel,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForDispatching.UDSRequestOBDAlreadyRunningOnDifferentStack.ProcessInParallel,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForDispatching.OBDRequestUDSAlreadyRunningOnDifferentStack.ProcessInParallel,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForDispatching.UDSRequestOBDAlreadyRunningOnDifferentStack.ProcessInParallel,1 */

  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.OnCurrentProtocol.ProcessRequest,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.NoDcmDslPeriodicTransmissionConRef.OnCurrentProtocol.ProcessRequest,1 */

  Dcm_Dsp_ProtocolStart(
    DCM_PROTOCOL_ID_FROM_INDEX(ProtocolIndex),
    Dcm_ProtocolQueue[ProtocolIndex].RxConnectionId,
    TxConnectionId,
    Dcm_ProtocolQueue[ProtocolIndex].RequestBuffer,
    Dcm_ProtocolQueue[ProtocolIndex].RequestType,
    ProtocolRCRRPTransmission,
    Dcm_ProtocolQueue[ProtocolIndex].RCRRPsSent,
	Dcm_ProtocolQueue[ProtocolIndex].RCRRPsConfirmed);

}

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON))
/* !LINKSTO Dcm.Dsn.IB.Supervisor.ProtocolHandling.DispatchTYPE2RequestToProtocol,1 */
STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_DispatchTYPE2RequestToProtocol(
  Dcm_ProtocolIndexType ProtocolIndex
)
{
  /* Dispatches the Presumptive of the given Protocol. A Presumptive must exist (asserted
     by the default case). */

  Dcm_RxPduIdInfoType RxPduIdInfo;
  Dcm_BufferUserType TxConnectionId;
  PduIdType ConfirmationTxPduId = DCM_NUM_TX_CONNECTIONS;

  Dcm_Dsl_RxConnectionGetRxPduIdInformation(
    Dcm_ProtocolQueue[ProtocolIndex].RxConnectionId,
    &RxPduIdInfo);

  /* For TYPE1 requests, the Protocol's state has already been changed to the corresponding Active state
     by the act of Starting the Protocol. */

  /* If the Protocol in question is an OBD Protocol, also clear the OBD blocking flag if it
     was set. */
  switch (Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState)
  {
#if (DCM_HAS_ROE_TYPE2 == STD_ON)
    case DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE2_ROE_QUEUED:
    {
      const Dcm_RoeConnectionIndexType RoeConnectionIndex =
        Dcm_MainConnectionConfig[RxPduIdInfo.MainConnectionIndex].RoeConnectionIndex;
      /* Get the Tx connection*/

      /* !LINKSTO SWS_Dcm_00928.Ref,1 */
      ConfirmationTxPduId = Dcm_RoeConnectionConfig[RoeConnectionIndex].ConfirmationTxPduId;

      (void)Dcm_Dsl_BufferChangeState(
          DCM_SUPERVISOR_ID,
          DCM_BUFFER_PROCESSING_ROE,
          Dcm_ProtocolQueue[ProtocolIndex].RequestBuffer.BufferId
        );

      break;
    }
#endif /* #if (DCM_HAS_ROE_TYPE2 == STD_ON) */

#if (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON)
    case DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE2_PERIODIC_QUEUED:
    {
      const Dcm_PeriodicTransmissionIndexType PeriodicTransmissionIndex =
        Dcm_MainConnectionConfig[RxPduIdInfo.MainConnectionIndex].PeriodicTransmissionIndex;

      const Dcm_PeriodicConnectionIndexType FirstPeriodicConnectionIndex =
        Dcm_PeriodicTransmissionConfig[PeriodicTransmissionIndex]
        .IndexOfFirstPeriodicConnection;

      ConfirmationTxPduId =
        Dcm_PeriodicConnectionConfig[FirstPeriodicConnectionIndex].ConfirmationTxPduId;

      break;
    }
#endif /* #if (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) */

    /* CHECK: NOPARSE */
    default:
      /* Unreachable */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  /* Translate the ConfirmationTxPduID into a TxConnectionId */
  TxConnectionId = DCM_TX_CONNECTION_ID_FROM_INDEX(ConfirmationTxPduId);

  /* Change the owner of the transmission buffer back to the Protocol. */
  (void)Dcm_Dsl_BufferGive(
      DCM_SUPERVISOR_ID,
      DCM_PROTOCOL_ID_FROM_INDEX(ProtocolIndex),
      Dcm_ProtocolQueue[ProtocolIndex].RequestBuffer.BufferId
    );
      /* There is no P2Timer to give here. */

      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE2Periodic.OnCurrentProtocol.ProcessRequest,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE2Periodic.NotOnCurrentProtocol.ProcessRequest,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE2Periodic.AcceptedForDispatching.UDSAlreadyRunningOnDifferentStack.ProcessInParallel,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE2Periodic.AcceptedForDispatching.UDSAlreadyRunningOnSameStack.ProcessInParallel,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE2Periodic.AcceptedForDispatching.OBDAlreadyRunningOnDifferentStack.ProcessInParallel,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE2Periodic.AcceptedForDispatching.OBDAlreadyRunningOnSameStack.ProcessInParallel,1 */

      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE2ROEEvent.NotOnCurrentProtocol.ProcessRequest,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE2ROEEvent.OnCurrentProtocol.ProcessRequest,1 */

      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE2ROEEvent.AcceptedForDispatching.UDSAlreadyRunningOnDifferentStack.ProcessInParallel,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE2ROEEvent.AcceptedForDispatching.UDSAlreadyRunningOnSameStack.ProcessInParallel,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE2ROEEvent.AcceptedForDispatching.OBDAlreadyRunningOnDifferentStack.ProcessInParallel,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE2ROEEvent.AcceptedForDispatching.OBDAlreadyRunningOnSameStack.ProcessInParallel,1 */
  Dcm_Dsp_ProtocolStart(
      DCM_PROTOCOL_ID_FROM_INDEX(ProtocolIndex),
      Dcm_ProtocolQueue[ProtocolIndex].RxConnectionId,
      TxConnectionId,
      Dcm_ProtocolQueue[ProtocolIndex].RequestBuffer,
      Dcm_ProtocolQueue[ProtocolIndex].RequestType,
      DCM_PROTOCOL_RCRRP_INVALID,
	  Dcm_ProtocolQueue[ProtocolIndex].RCRRPsSent,
	  Dcm_ProtocolQueue[ProtocolIndex].RCRRPsConfirmed
    );

  Dcm_ProtocolStatus[ProtocolIndex].RequestType = Dcm_ProtocolQueue[ProtocolIndex].RequestType;
  Dcm_ProtocolStatus[ProtocolIndex].RxConnectionId = Dcm_ProtocolQueue[ProtocolIndex].RxConnectionId;

  /* MainConnection is not affected by TYPE2 requests. */
}
#endif /* #if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON)) */

STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_CancelPresumptiveProtocol(
  Dcm_ProtocolIndexType ProtocolIndex
)
{
  switch (Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState)
  {
    case DCM_SUPERVISOR_PROTOCOL_QUEUE_POST_BOOT_QUEUED:
    /* Post-boot response queued. This cannot normally be cancelled
       as it will block all other receptions. */
    case DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED:
    {
      /* This Protocol's P2timer was running since when it became Presumptive. Any NRC shall be
         transmitted considering the P2Timer value already elapsed.
       */
      const Dcm_TimerValueType  NRCTimeout = Dcm_Dsl_DiagnosticSessionHandler_GetTimerValue(ProtocolIndex, DCM_P2TIMER);

      /* First trigger an NRC for this cancellation if configured OR if a RCRRP
         NRC has ever been sent for this request. An RCRRP forces a final response.
         Since this is done in the context of an ISR, the NRC transmission
         must be delayed until the next Dcm_MainFunction() cycle.
       */

      if (
          (0U != Dcm_ProtocolQueue[ProtocolIndex].RCRRPsConfirmed) ||
          (DCM_RESPOND_WITH_BUSYREPEATREQUEST == STD_ON)
         )
      {
        Dcm_Dsl_Supervisor_MainConnectionHandling_InitiateNRC(
            Dcm_ProtocolQueue[ProtocolIndex].RxConnectionId,
            Dcm_ProtocolQueue[ProtocolIndex].RequestBuffer,
            DCM_E_BUSYREPEATREQUEST,
            NRCTimeout
          );
      }
      else
      {
        Dcm_RxPduIdInfoType RxPduIdInfo;

        Dcm_Dsl_RxConnectionGetRxPduIdInformation(Dcm_ProtocolQueue[ProtocolIndex].RxConnectionId, &RxPduIdInfo);

        /* Advertize end of communication sequence to SessionManager. */
        Dcm_Dsl_Supervisor_CommunicationSessionHandling_EndOfCommunicationSession(
          Dcm_ProtocolQueue[ProtocolIndex].RxConnectionId);
        /* And that all processing has been finished pertaining to this communication session.
           This checks to see whether S3 timers or ComMChannels need any handling. */
        Dcm_Dsl_Supervisor_CommunicationSessionHandling_ProcessingEnd(
          Dcm_ProtocolQueue[ProtocolIndex].RxConnectionId);

        /* And clear the MainConnection */
        Dcm_Dsl_Supervisor_MainConnectionHandling_ClearMainConnection(RxPduIdInfo.MainConnectionIndex);
      }

      /* Clear presumptive protocol. */
      Dcm_Dsl_Supervisor_ProtocolHandling_ClearPresumptiveProtocol(ProtocolIndex, FALSE);

      /* This Protocol's P2Timer was running since when it became Presumptive. Stop it.
       */
      (void)Dcm_Dsl_DiagnosticSessionHandler_StopTimer(DCM_SUPERVISOR_ID,
                                                       ProtocolIndex,
                                                       DCM_P2TIMER);

      (void)Dcm_Dsl_DiagnosticSessionHandler_FreeP2Timer(DCM_SUPERVISOR_ID,
                                                         ProtocolIndex);

      break;
    }

    case DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED_RCRRP_QUEUED:
    case DCM_SUPERVISOR_PROTOCOL_QUEUE_EXTERNAL_QUEUED_RCRRP_TRANSMITTING:
    {

      Dcm_TimerValueType  NRCTimeout = Dcm_Dsl_Supervisor_MainConnectionHandling_GetNRCTimeout(ProtocolIndex);

      /* The MainConnection is busy sending a RCRRP. If this happens, a BusyRepeatRequest
         (0x21) must be sent as a RCRRP has to be followed by some final response. The
          NRC shall be automatically sent after the RCRRP is confirmed.
          Being sent automatically, its Timeout will be the maximum possible. Also, the value
          given to the InitiateNRC function is not used.
          Since this is done in the context of an ISR, the NRC transmission
          must be delayed until the next Dcm_MainFunction() cycle.
       */
      Dcm_Dsl_Supervisor_MainConnectionHandling_InitiateNRC(
          Dcm_ProtocolQueue[ProtocolIndex].RxConnectionId,
          Dcm_ProtocolQueue[ProtocolIndex].RequestBuffer,
          DCM_E_BUSYREPEATREQUEST,
          NRCTimeout
        );

      /* Clear presumptive protocol. */
      /* This Protocol's P2Timer was NOT running anymore since it's stopped during RCRRP
         transmission.
       */
      Dcm_Dsl_Supervisor_ProtocolHandling_ClearPresumptiveProtocol(ProtocolIndex, FALSE);
      break;
    }

    case DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE1_ROE_QUEUED:
    case DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE1_PERIODIC_QUEUED:
    {
      Dcm_RxPduIdInfoType RxPduIdInfo;

      Dcm_Dsl_RxConnectionGetRxPduIdInformation(Dcm_ProtocolQueue[ProtocolIndex].RxConnectionId, &RxPduIdInfo);
      /* And TYPE2 ROE Presumptives are also cancelled by default as ROE
         processing always gets halted by an external request.
       */

      /*
      These require no further actions. Clear.
      */

      /* This Protocol's P2Timer was NOT running. */
      Dcm_Dsl_Supervisor_ProtocolHandling_ClearPresumptiveProtocol(ProtocolIndex, FALSE);

      /* And clear the MainConnection */
      Dcm_Dsl_Supervisor_MainConnectionHandling_ClearMainConnection(RxPduIdInfo.MainConnectionIndex);

      break;
    }
    case DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE2_ROE_QUEUED:
    case DCM_SUPERVISOR_PROTOCOL_QUEUE_TYPE2_PERIODIC_QUEUED:
    {
      /*
      These require no further actions. Clear.
      */
      /* This Protocol's P2Timer was NOT running. */
      Dcm_Dsl_Supervisor_ProtocolHandling_ClearPresumptiveProtocol(ProtocolIndex, FALSE);
      break;
    }

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. That's all the presumptive states.
         Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }
}

#if (DCM_CALLBACK_REQUEST_SIZE > 0)
/* !LINKSTO Dcm.Dsn.IB.Supervisor.ProtocolHandling.CallStopProtocolOperations,1 */
STATIC FUNC (boolean, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_CallStopProtocolOperations(
  Dcm_ProtocolIndexType StoppingProtocolIndex
)
{
  /* Synchronized call of the XXX_Stop operations for this Protocol
     In case the XXX_Stop operation returns an error, remember this so as
     to return it to the PresumptiveProtocol.
   */
   boolean StopOperationFailed = FALSE;
   uint8_least StopOperationIndex;

  /* Call all configured Xxx_StopProtocol() functions */

  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StopProtocol.Call,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StopProtocol.Call,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StopProtocol.Call,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StopProtocol.Call,1 */

  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StopProtocol.Call,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StopProtocol.Call,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StopProtocol.Call,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StopProtocol.Call,1 */

  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.StopProtocol.Call,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.StopProtocol.Call,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusyTYPE1Periodic.StopProtocol.Call,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusyTYPE1Periodic.StopProtocol.Call,1 */

  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.DefaultSession.NotOnCurrentProtocol.CurrentProtocolIdle.StopProtocol.Call,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.NonDefaultSession.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StopProtocol.Call,1 */

  /* Deviation TASKING-1 */
  for (StopOperationIndex = 0U;
       StopOperationIndex < DCM_CALLBACK_REQUEST_SIZE;
       StopOperationIndex++)
  {
    /* Load the Xxx_StopProtocol function to call */
    const Dcm_DslStopProtocolFncType StopProtocolFnc =
      Dcm_CallbackRequests[StopOperationIndex].StopProtocol;

    /* Get the type of the protocol from configuration */
    const Dcm_ProtocolType ProtocolType =
      Dcm_ProtocolConfig[StoppingProtocolIndex].Type;

    Std_ReturnType Result = StopProtocolFnc(ProtocolType);

    /* Update ProtocolLocked if result != E_OK */
    if (DCM_E_NOT_OK == Result)
    {
      StopOperationFailed = TRUE;
    }
    else if (DCM_E_OK != Result)
    {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StopProtocol.ReturnsUnexpected.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StopProtocol.ReturnsUnexpected.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StopProtocol.ReturnsUnexpected.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StopProtocol.ReturnsUnexpected.ReportDETError,1 */

      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StopProtocol.ReturnsUnexpected.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StopProtocol.ReturnsUnexpected.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StopProtocol.ReturnsUnexpected.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StopProtocol.ReturnsUnexpected.ReportDETError,1 */

      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.StopProtocol.ReturnsUnexpected.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.StopProtocol.ReturnsUnexpected.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusyTYPE1Periodic.StopProtocol.ReturnsUnexpected.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusyTYPE1Periodic.StopProtocol.ReturnsUnexpected.ReportDETError,1 */

      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.DefaultSession.NotOnCurrentProtocol.CurrentProtocolIdle.StopProtocol.ReturnsUnexpected.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.NonDefaultSession.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StopProtocol.ReturnsUnexpected.ReportDETError,1 */

      DCM_DET_REPORT_ERROR(DCM_INTERNAL_API_ID, DCM_E_INTERFACE_RETURN_VALUE);
#endif
      StopOperationFailed = TRUE;
    }
    else
    {
      /* Deviation TASKING-2 */
      DCM_INVARIANT_ASSERT(DCM_E_OK == Result, DCM_INTERNAL_API_ID);
    }
  }

  return StopOperationFailed;
}
#endif /* #if (DCM_CALLBACK_REQUEST_SIZE > 0) */

/* !LINKSTO Dcm.Dsn.IB.Supervisor.ProtocolHandling.AttemptToStopStoppingProtocols,1 */
STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_AttemptToStopStoppingProtocols(
  void
)
{
  /* Switches active protocols. Basically whenever a Protocol shall stop,
     another shall (attempt to) take its place.
   */

  uint8_least ProtocolIndex;

  /* Only one Protocol on any given stack may be in a STOPPING state. */
  /* Deviation TASKING-1 */
  for (ProtocolIndex = 0U; ProtocolIndex < DCM_NUM_PROTOCOLS; ProtocolIndex++)
  {

    /* Nothing takes a Protocol out of a stopping state. It's sufficient to
       know it's stopping, hence the lack of exclusive area. */

    if((DCM_SUPERVISOR_PROTOCOL_STOPPING_IDLE ==
         Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
       (DCM_SUPERVISOR_PROTOCOL_STOPPING_ACTIVE_TYPE2_ROE ==
         Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
       (DCM_SUPERVISOR_PROTOCOL_STOPPING_ACTIVE_TYPE2_PERIODIC ==
         Dcm_ProtocolStatus[ProtocolIndex].ProtocolState) ||
       (DCM_SUPERVISOR_PROTOCOL_STOPPING_CANCELLING_TYPE2_ROE ==
         Dcm_ProtocolStatus[ProtocolIndex].ProtocolState)
      )
    {
#if (DCM_CALLBACK_REQUEST_SIZE > 0)
      /* Found a stopping Protocol. Call the Stop interfaces. */
      boolean StopOperationFailed = Dcm_Dsl_Supervisor_ProtocolHandling_CallStopProtocolOperations(
                                        (Dcm_ProtocolIndexType)(ProtocolIndex)
                                     );

      if (FALSE == StopOperationFailed)
#endif /* #if (DCM_CALLBACK_REQUEST_SIZE > 0) */
      {
        /* ENTER critical section */
        SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

        /* At the end of this, if the Stop interfaces all succeeded, we stop the Protocol. Note
           that in this case we need to protect the operation since protocol states may
           change as a result of responses coming in. We can be sure though that the
           Protocol stayed in a Stopping state since nothing can get it out of this state.
         */

        /* The Protocol is now Stopped. */
        switch (Dcm_ProtocolStatus[ProtocolIndex].ProtocolState)
        {
          case DCM_SUPERVISOR_PROTOCOL_STOPPING_IDLE:
            Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
              DCM_SUPERVISOR_PROTOCOL_STOPPED_IDLE;
            break;

          case DCM_SUPERVISOR_PROTOCOL_STOPPING_ACTIVE_TYPE2_ROE:

            Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
              DCM_SUPERVISOR_PROTOCOL_STOPPED_ACTIVE_TYPE2_ROE;
            break;

          case DCM_SUPERVISOR_PROTOCOL_STOPPING_ACTIVE_TYPE2_PERIODIC:
            Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
              DCM_SUPERVISOR_PROTOCOL_STOPPED_ACTIVE_TYPE2_PERIODIC;
            break;

          case DCM_SUPERVISOR_PROTOCOL_STOPPING_CANCELLING_TYPE2_ROE:

            Dcm_ProtocolStatus[ProtocolIndex].ProtocolState =
              DCM_SUPERVISOR_PROTOCOL_STOPPED_CANCELLING_TYPE2_ROE;
            break;

          /* CHECK: NOPARSE */
          default:
            /* Defensive branch. Default case at the end of the switch statement */
            DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
            break;
          /* CHECK: PARSE */
        }
      }
#if (DCM_CALLBACK_REQUEST_SIZE > 0)
      else
      {
        /* This means that an NRC needs to be transmitted.
           to the current PresumptiveProtocol, if still there.
         */

        Dcm_ProtocolIndexType TYPE1PresumptiveProtocolIndex;

        /* ENTER critical section */
        SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

        TYPE1PresumptiveProtocolIndex =
          Dcm_Dsl_Supervisor_GetTYPE1PresumptiveProtocolOnStack(
              Dcm_ProtocolConfig[ProtocolIndex].StackNumber
            );

        if (DCM_INVALID_PROTOCOL_INDEX != TYPE1PresumptiveProtocolIndex)
        {
          const Dcm_TimerValueType  NRCTimeout =
            Dcm_Dsl_DiagnosticSessionHandler_GetTimerValue(TYPE1PresumptiveProtocolIndex, DCM_P2TIMER);

          if (DCM_PERIODIC_TYPE1_REQUEST != Dcm_ProtocolQueue[TYPE1PresumptiveProtocolIndex].RequestType)
          {
            /* Periodic transmissions shall not result in NRCs. */

            /* Stopping the previous CurrentProtocol resulted in an error.
               Queue an NRC. Since this is done in the MainFunction, the NRC transmission
               may be made immediate...
             */

            /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StopProtocol.ReturnsNotE_OK.TransmitNRC,1 */
            /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StopProtocol.ReturnsNotE_OK.TransmitNRC,1 */
            /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StopProtocol.ReturnsNotE_OK.TransmitNRC,1 */
            /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StopProtocol.ReturnsNotE_OK.TransmitNRC,1 */

            /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StopProtocol.ReturnsNotE_OK.TransmitNRC,1 */
            /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StopProtocol.ReturnsNotE_OK.TransmitNRC,1 */
            /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StopProtocol.ReturnsNotE_OK.TransmitNRC,1 */
            /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StopProtocol.ReturnsNotE_OK.TransmitNRC,1 */

            /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.StopProtocol.ReturnsNotE_OK.TransmitNRC,1 */
            /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.StopProtocol.ReturnsNotE_OK.TransmitNRC,1 */
            /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusyTYPE1Periodic.StopProtocol.ReturnsNotE_OK.TransmitNRC,1 */
            /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusyTYPE1Periodic.StopProtocol.ReturnsNotE_OK.TransmitNRC,1 */

            /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.DefaultSession.NotOnCurrentProtocol.CurrentProtocolIdle.StopProtocol.ReturnsNotE_OK.Discard,1 */
            /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.NonDefaultSession.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StopProtocol.ReturnsNotE_OK.Discard,1 */

            Dcm_Dsl_Supervisor_MainConnectionHandling_InitiateNRC(
                Dcm_ProtocolQueue[TYPE1PresumptiveProtocolIndex].RxConnectionId,
                Dcm_ProtocolQueue[TYPE1PresumptiveProtocolIndex].RequestBuffer,
                DCM_E_CONDITIONSNOTCORRECT,
                NRCTimeout
              );

            /* And don't clear the MainConnection since we're waiting for an NRC to be sent out. */
          }
          else
          {
            Dcm_RxPduIdInfoType RxPduIdInfo;

            Dcm_Dsl_RxConnectionGetRxPduIdInformation(Dcm_ProtocolQueue[TYPE1PresumptiveProtocolIndex].RxConnectionId,
                                                      &RxPduIdInfo);

            /* For a Periodic TYPE1 transmission also clear the MainConnection and RxConnection now. */
            Dcm_Dsl_Supervisor_MainConnectionHandling_ClearMainConnection(RxPduIdInfo.MainConnectionIndex);
          }

          if (
              (DCM_NORMAL_PHYSICAL_REQUEST == Dcm_ProtocolQueue[TYPE1PresumptiveProtocolIndex].RequestType) ||
              (DCM_NORMAL_FUNCTIONAL_REQUEST == Dcm_ProtocolQueue[TYPE1PresumptiveProtocolIndex].RequestType)
             )
          {

            /* ...as this Protocol's P2Timer was running since when it became Presumptive. Stop it...
             */
            (void)Dcm_Dsl_DiagnosticSessionHandler_StopTimer(DCM_SUPERVISOR_ID,
                                                             TYPE1PresumptiveProtocolIndex,
                                                             DCM_P2TIMER);

            (void)Dcm_Dsl_DiagnosticSessionHandler_FreeP2Timer(DCM_SUPERVISOR_ID,
                                                               TYPE1PresumptiveProtocolIndex);
          }

          /* ... and then drop the request. */
          Dcm_Dsl_Supervisor_ProtocolHandling_ClearPresumptiveProtocol(TYPE1PresumptiveProtocolIndex, FALSE);
        }
        else
        {
          /* It is possible that this presumptive has been killed off by now (maybe by an OBD
             request on another stack since the preemption was ordered. */
        }
      }
#endif /* #if (DCM_CALLBACK_REQUEST_SIZE > 0) */

      /* EXIT critical section */
      SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
    }
  }
}

#if (DCM_CALLBACK_REQUEST_SIZE > 0)
/* !LINKSTO Dcm.Dsn.IB.Supervisor.ProtocolHandling.CallStartProtocolOperations,1 */
/* !LINKSTO Dcm.Dsn.IB.Supervisor.ProtocolHandling.CallStartProtocolOperations.DET,1 */
STATIC FUNC (boolean, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_CallStartProtocolOperations(
  Dcm_ProtocolIndexType StartingProtocolIndex
)
{
  boolean StartOperationFailed = FALSE;
  uint8_least StartOperationIndex;

  /* call all configured Xxx_StartProtocol() functions */

  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NoCurrentProtocol.StartProtocol.Call,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NoCurrentProtocol.StartProtocol.Call,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.WarmStart.AcceptedForProcessing.NoCurrentProtocol.StartProtocol.Call,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NoCurrentProtocol.StartProtocol.Call,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NoCurrentProtocol.StartProtocol.Call,1 */

  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StopProtocol.ReturnsE_OK.StartProtocol.Call,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StopProtocol.ReturnsE_OK.StartProtocol.Call,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StopProtocol.ReturnsE_OK.StartProtocol.Call,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StopProtocol.ReturnsE_OK.StartProtocol.Call,1 */

  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StopProtocol.ReturnsE_OK.StartProtocol.Call,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StopProtocol.ReturnsE_OK.StartProtocol.Call,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StopProtocol.ReturnsE_OK.StartProtocol.Call,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StopProtocol.ReturnsE_OK.StartProtocol.Call,1 */

  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.StopProtocol.ReturnsE_OK.StartProtocol.Call,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.StopProtocol.ReturnsE_OK.StartProtocol.Call,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusyTYPE1Periodic.StopProtocol.ReturnsE_OK.StartProtocol.Call,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusyTYPE1Periodic.StopProtocol.ReturnsE_OK.StartProtocol.Call,1 */

  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.NoCurrentProtocol.StartProtocol.Call,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.DefaultSession.NotOnCurrentProtocol.CurrentProtocolIdle.StopProtocol.ReturnsE_OK.StartProtocol.Call,1 */
  /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.NonDefaultSession.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StopProtocol.ReturnsE_OK.StartProtocol.Call,1 */

  /* Deviation TASKING-1 */
  for (StartOperationIndex = 0U;
       StartOperationIndex < DCM_CALLBACK_REQUEST_SIZE;
       StartOperationIndex++)
  {
    /* load the Xxx_StartProtocol function to call */
    const Dcm_DslStartProtocolFncType StartProtocolFnc =
      Dcm_CallbackRequests[StartOperationIndex].StartProtocol;

    /* get the type of the protocol from configuration */
    const Dcm_ProtocolType ProtocolType =
      Dcm_ProtocolConfig[StartingProtocolIndex].Type;

    Std_ReturnType Result = StartProtocolFnc(ProtocolType);
    if ((DCM_E_NOT_OK == Result) ||
        (DCM_E_PROTOCOL_NOT_ALLOWED == Result))
    {
       StartOperationFailed = TRUE;
    }
    else if (DCM_E_OK != Result)
    {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NoCurrentProtocol.StartProtocol.ReturnsUnexpected.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NoCurrentProtocol.StartProtocol.ReturnsUnexpected.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.WarmStart.AcceptedForProcessing.NoCurrentProtocol.StartProtocol.ReturnsUnexpected.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NoCurrentProtocol.StartProtocol.ReturnsUnexpected.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NoCurrentProtocol.StartProtocol.ReturnsUnexpected.ReportDETError,1 */

      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StartProtocol.ReturnsUnexpected.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StartProtocol.ReturnsUnexpected.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StartProtocol.ReturnsUnexpected.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StartProtocol.ReturnsUnexpected.ReportDETError,1 */

      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StartProtocol.ReturnsUnexpected.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StartProtocol.ReturnsUnexpected.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StartProtocol.ReturnsUnexpected.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StartProtocol.ReturnsUnexpected.ReportDETError,1 */

      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.StartProtocol.ReturnsUnexpected.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.StartProtocol.ReturnsUnexpected.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusyTYPE1Periodic.StartProtocol.ReturnsUnexpected.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusyTYPE1Periodic.StartProtocol.ReturnsUnexpected.ReportDETError,1 */

      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.DefaultSession.NotOnCurrentProtocol.CurrentProtocolIdle.StartProtocol.ReturnsUnexpected.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.NonDefaultSession.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StartProtocol.ReturnsUnexpected.ReportDETError,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.NoCurrentProtocol.StartProtocol.ReturnsUnexpected.ReportDETError,1 */

      DCM_DET_REPORT_ERROR(DCM_INTERNAL_API_ID, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
      StartOperationFailed = TRUE;
    }
    else
    {
      /* Linked here a the started Protocol becomes the CurrentProtocol. */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.External.ProcessingEnd.CurrentProtocol,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.ProcessingEnd.CurrentProtocol,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.WarmStart.ProcessingEnd.CurrentProtocol,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.ProcessingEnd.CurrentProtocol,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.ProcessingEnd.CurrentProtocol,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.ProcessingEnd.CurrentProtocol,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.NoDcmDslPeriodicTransmissionConRef.ProcessingEnd.CurrentProtocol,1 */

      /* Deviation TASKING-2 */
      DCM_INVARIANT_ASSERT(DCM_E_OK == Result, DCM_INTERNAL_API_ID);
    }
  }

  return StartOperationFailed;
}
#endif /* #if (DCM_CALLBACK_REQUEST_SIZE > 0) */

/* !LINKSTO Dcm.Dsn.IB.Supervisor.ProtocolHandling.AttemptToStartPresumptiveTYPE1ProtocolOnStack.ResetToDefault,1 */
/* !LINKSTO Dcm.Dsn.IB.Supervisor.ProtocolHandling.AttemptToStartPresumptiveTYPE1ProtocolOnStack.Security,1 */
/* !LINKSTO Dcm.Dsn.IB.Supervisor.ProtocolHandling.AttemptToStartPresumptiveTYPE1ProtocolOnStack.Start,1 */
/* !LINKSTO Dcm.Dsn.IB.Supervisor.ProtocolHandling.AttemptToStartPresumptiveTYPE1ProtocolOnStack.Error,1 */
STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_AttemptToStartPresumptiveTYPE1ProtocolOnStack(
  Dcm_StackNumberType StackNumber
)
{
  Dcm_ProtocolIndexType TYPE1PresumptiveProtocolIndex;
  boolean ResetSessionToDefault = FALSE;

  TYPE1PresumptiveProtocolIndex =
          Dcm_Dsl_Supervisor_GetTYPE1PresumptiveProtocolOnStack(
              StackNumber
            );

  if (DCM_INVALID_PROTOCOL_INDEX != TYPE1PresumptiveProtocolIndex)
  {
    if (TRUE == Dcm_Dsl_Supervisor_ProtocolHandling_StackCanAcceptTYPE1Request(StackNumber))
    {
#if (DCM_CALLBACK_REQUEST_SIZE > 0)
      boolean StartOperationFailed;
#endif /* #if (DCM_CALLBACK_REQUEST_SIZE > 0) */
      DCM_PRECONDITION_ASSERT(
          Dcm_ProtocolStatus[TYPE1PresumptiveProtocolIndex].ProtocolState ==
            DCM_SUPERVISOR_PROTOCOL_STOPPED_IDLE,
          DCM_INTERNAL_API_ID
        );
#if (DCM_CALLBACK_REQUEST_SIZE > 0)
      StartOperationFailed = Dcm_Dsl_Supervisor_ProtocolHandling_CallStartProtocolOperations(
                                         TYPE1PresumptiveProtocolIndex
                                       );

      if (FALSE == StartOperationFailed)
#endif /* #if (DCM_CALLBACK_REQUEST_SIZE > 0) */
      {

        /* ENTER critical section */
        SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
        /* This is always in the context of the MainFunction. Attempt to
           switch session to default immediately, and if this is not
           possible, just enqueue this event for later.
         */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NoCurrentProtocol.StartProtocol.ReturnsE_OK.ResetToDefaultSession,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NoCurrentProtocol.StartProtocol.ReturnsE_OK.ResetToDefaultSession,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.WarmStart.AcceptedForProcessing.NoCurrentProtocol.StartProtocol.ReturnsE_OK.ResetToDefaultSession,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NoCurrentProtocol.StartProtocol.ReturnsE_OK.ResetToDefaultSession,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NoCurrentProtocol.StartProtocol.ReturnsE_OK.ResetToDefaultSession,1 */

        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StartProtocol.ReturnsE_OK.ResetToDefaultSession,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StartProtocol.ReturnsE_OK.ResetToDefaultSession,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StartProtocol.ReturnsE_OK.ResetToDefaultSession,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StartProtocol.ReturnsE_OK.ResetToDefaultSession,1 */

        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StartProtocol.ReturnsE_OK.ResetToDefaultSession,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StartProtocol.ReturnsE_OK.ResetToDefaultSession,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StartProtocol.ReturnsE_OK.ResetToDefaultSession,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StartProtocol.ReturnsE_OK.ResetToDefaultSession,1 */

        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.StartProtocol.ReturnsE_OK.ResetToDefaultSession,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.StartProtocol.ReturnsE_OK.ResetToDefaultSession,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusyTYPE1Periodic.StartProtocol.ReturnsE_OK.ResetToDefaultSession,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusyTYPE1Periodic.StartProtocol.ReturnsE_OK.ResetToDefaultSession,1 */

        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.DefaultSession.NotOnCurrentProtocol.CurrentProtocolIdle.StartProtocol.ReturnsE_OK.ResetToDefaultSession,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.NonDefaultSession.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StartProtocol.ReturnsE_OK.ResetToDefaultSession,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.NoCurrentProtocol.StartProtocol.ReturnsE_OK.ResetToDefaultSession,1 */

        ResetSessionToDefault = TRUE;

        /* And reset security */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NoCurrentProtocol.StartProtocol.ReturnsE_OK.LockSecurityLevel,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NoCurrentProtocol.StartProtocol.ReturnsE_OK.LockSecurityLevel,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.WarmStart.AcceptedForProcessing.NoCurrentProtocol.StartProtocol.ReturnsE_OK.LockSecurityLevel,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NoCurrentProtocol.StartProtocol.ReturnsE_OK.LockSecurityLevel,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NoCurrentProtocol.StartProtocol.ReturnsE_OK.LockSecurityLevel,1 */

        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StartProtocol.ReturnsE_OK.LockSecurityLevel,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StartProtocol.ReturnsE_OK.LockSecurityLevel,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StartProtocol.ReturnsE_OK.LockSecurityLevel,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StartProtocol.ReturnsE_OK.LockSecurityLevel,1 */

        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StartProtocol.ReturnsE_OK.LockSecurityLevel,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StartProtocol.ReturnsE_OK.LockSecurityLevel,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StartProtocol.ReturnsE_OK.LockSecurityLevel,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StartProtocol.ReturnsE_OK.LockSecurityLevel,1 */

        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.StartProtocol.ReturnsE_OK.LockSecurityLevel,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.StartProtocol.ReturnsE_OK.LockSecurityLevel,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusyTYPE1Periodic.StartProtocol.ReturnsE_OK.LockSecurityLevel,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusyTYPE1Periodic.StartProtocol.ReturnsE_OK.LockSecurityLevel,1 */

        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.NoCurrentProtocol.StartProtocol.ReturnsE_OK.LockSecurityLevel,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.DefaultSession.NotOnCurrentProtocol.CurrentProtocolIdle.StartProtocol.ReturnsE_OK.LockSecurityLevel,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.NonDefaultSession.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StartProtocol.ReturnsE_OK.LockSecurityLevel,1 */

        Dcm_Dsl_SecurityLevelHandler_SetSecurityLevel(DCM_SEC_LEV_LOCKED);

        /* Take PresumptiveProtocol to Started state */
        Dcm_ProtocolStatus[TYPE1PresumptiveProtocolIndex].ProtocolState =
          DCM_SUPERVISOR_PROTOCOL_STARTED_IDLE;
      }
#if (DCM_CALLBACK_REQUEST_SIZE > 0)
      else
      {
        Dcm_TimerValueType  NRCTimeout;

        /* ENTER critical section */
        SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

        NRCTimeout =
          Dcm_Dsl_DiagnosticSessionHandler_GetTimerValue(TYPE1PresumptiveProtocolIndex, DCM_P2TIMER);

        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.DefaultSession.NotOnCurrentProtocol.CurrentProtocolIdle.StartProtocol.ReturnsNotE_OK.Discard,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.NonDefaultSession.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StartProtocol.ReturnsNotE_OK.Discard,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.NoCurrentProtocol.StartProtocol.ReturnsNotE_OK.Discard,1 */
        if (DCM_PERIODIC_TYPE1_REQUEST != Dcm_ProtocolQueue[TYPE1PresumptiveProtocolIndex].RequestType)
        {
          /* Starting the Protocol resulted in an error.
             Queue an NRC. Since this is done in the MainFunction, the NRC transmission
             may be made immediate...
           */

          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NoCurrentProtocol.StartProtocol.ReturnsNotE_OK.TransmitNRC,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NoCurrentProtocol.StartProtocol.ReturnsNotE_OK.TransmitNRC,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.WarmStart.AcceptedForProcessing.NoCurrentProtocol.StartProtocol.ReturnsNotE_OK.TransmitNRC,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NoCurrentProtocol.StartProtocol.ReturnsNotE_OK.TransmitNRC,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NoCurrentProtocol.StartProtocol.ReturnsNotE_OK.TransmitNRC,1 */

          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StartProtocol.ReturnsNotE_OK.TransmitNRC,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StartProtocol.ReturnsNotE_OK.TransmitNRC,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StartProtocol.ReturnsNotE_OK.TransmitNRC,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StartProtocol.ReturnsNotE_OK.TransmitNRC,1 */

          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StartProtocol.ReturnsNotE_OK.TransmitNRC,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StartProtocol.ReturnsNotE_OK.TransmitNRC,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StartProtocol.ReturnsNotE_OK.TransmitNRC,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StartProtocol.ReturnsNotE_OK.TransmitNRC,1 */

          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.StartProtocol.ReturnsNotE_OK.TransmitNRC,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.StartProtocol.ReturnsNotE_OK.TransmitNRC,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.NoDcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusyTYPE1Periodic.StartProtocol.ReturnsNotE_OK.TransmitNRC,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.NewTYPE1ROEEvent.DcmDslRoeConnectionRef.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusyTYPE1Periodic.StartProtocol.ReturnsNotE_OK.TransmitNRC,1 */

          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.DefaultSession.NotOnCurrentProtocol.CurrentProtocolIdle.StartProtocol.ReturnsNotE_OK.Discard,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.NonDefaultSession.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StartProtocol.ReturnsNotE_OK.Discard,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.DcmDslPeriodicTransmissionConRef.NoCurrentProtocol.StartProtocol.ReturnsNotE_OK.Discard,1 */

          Dcm_Dsl_Supervisor_MainConnectionHandling_InitiateNRC(
              Dcm_ProtocolQueue[TYPE1PresumptiveProtocolIndex].RxConnectionId,
              Dcm_ProtocolQueue[TYPE1PresumptiveProtocolIndex].RequestBuffer,
              DCM_E_CONDITIONSNOTCORRECT,
              NRCTimeout
            );

          /* And don't clear the MainConnection since we're waiting for an NRC to be sent out. */
        }
        else
        {
          Dcm_RxPduIdInfoType RxPduIdInfo;

          Dcm_Dsl_RxConnectionGetRxPduIdInformation(Dcm_ProtocolQueue[TYPE1PresumptiveProtocolIndex].RxConnectionId,
                                                    &RxPduIdInfo);

          /* For a Periodic TYPE1 transmission also clear the MainConnection and RxConnection now. */
          Dcm_Dsl_Supervisor_MainConnectionHandling_ClearMainConnection(RxPduIdInfo.MainConnectionIndex);
        }

        /* Release P2Timer and remove protocol from presumptive queue */
        Dcm_Dsl_Supervisor_ProtocolHandling_PresumptiveProtocolEnd(TYPE1PresumptiveProtocolIndex);

      }
#endif /* #if (DCM_CALLBACK_REQUEST_SIZE > 0) */

      /* EXIT critical section */
      SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
    }
  }

  if (TRUE == ResetSessionToDefault)
  {
    /* Surely no Protocol was running when this flag was set. Therefore
       it's certain that by this moment no processing would have started even
       though presumptive protocols might have been enqueued in the meantime.*/

    /* Transiting to default session will lock the security level */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NoCurrentProtocol.StartProtocol.ReturnsE_OK.LockSecurityLevel,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NoCurrentProtocol.StartProtocol.ReturnsE_OK.LockSecurityLevel,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.WarmStart.AcceptedForProcessing.NoCurrentProtocol.StartProtocol.ReturnsE_OK.LockSecurityLevel,1 */

    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StartProtocol.ReturnsE_OK.LockSecurityLevel,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StartProtocol.ReturnsE_OK.LockSecurityLevel,1 */

    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StartProtocol.ReturnsE_OK.LockSecurityLevel,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StartProtocol.ReturnsE_OK.LockSecurityLevel,1 */

    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.StartProtocol.ReturnsE_OK.LockSecurityLevel,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.StartProtocol.ReturnsE_OK.LockSecurityLevel,1 */

    Dcm_Dsl_DiagnosticSessionHandler_ResetToDefaultSession(
                      DCM_RESETTODEFAULTSESSIONREQUESTER_SUPERVISOR);
  }
}

/* !LINKSTO Dcm.Dsn.IB.Supervisor.ProtocolHandling.ResetToDefaultIfSessionWasSetWithProtocolFromAnotherStack,1 */
STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_ResetToDefaultIfSessionWasSetWithProtocolFromAnotherStack(
  Dcm_ProtocolIndexType ProtocolIndex
)
{
  const Dcm_ProtocolIndexType NonDefaultSessionProtocolIndex =
              Dcm_Dsl_DiagnosticSessionHandler_GetNonDefaultSessionProtocolIndex();
  const Dcm_ProtocolStackNumberType CurrentProtocolStackNumber =
              Dcm_ProtocolConfig[ProtocolIndex].StackNumber;
  const Dcm_SesCtrlType CurrentSession =
              Dcm_Dsl_DiagnosticSessionHandler_GetCurrentSession();

  if( ( (Dcm_ProtocolQueue[ProtocolIndex].RequestType == DCM_NORMAL_PHYSICAL_REQUEST) ||
        (Dcm_ProtocolQueue[ProtocolIndex].RequestType == DCM_NORMAL_FUNCTIONAL_REQUEST) ) &&
      (CurrentSession != DCM_DEFAULT_SESSION) &&
      (DCM_INVALID_PROTOCOL_INDEX != NonDefaultSessionProtocolIndex) &&
      (CurrentProtocolStackNumber != Dcm_ProtocolConfig[NonDefaultSessionProtocolIndex].StackNumber) )
      {
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.OnCurrentProtocol.ProcessRequest.ResetToDefaultSession,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.OnCurrentProtocol.ProcessRequest.ResetToDefaultSession,1 */
        Dcm_Dsl_Supervisor_EnqueueResetOfDiagnosticSession();
      }
}

STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_DispatchPresumptiveTYPE1Protocol(
  Dcm_ProtocolIndexType TYPE1PresumptiveProtocolIndex
)
{
  /* Dispatch the request by winding up the correct Protocol instance,
     giving it ownership of the RequestBuffer, giving it ownership
     of the TxConnection (if needed) and sending the appropriate
     start signal.
   */

  /* Only a STARTED_IDLE Protocol shall be made ACTIVE. */
  DCM_PRECONDITION_ASSERT(
      Dcm_ProtocolStatus[TYPE1PresumptiveProtocolIndex].ProtocolState ==
        DCM_SUPERVISOR_PROTOCOL_STARTED_IDLE,
      DCM_INTERNAL_API_ID
    );

  Dcm_Dsl_Supervisor_ProtocolHandling_MakeStartedProtocolActive(TYPE1PresumptiveProtocolIndex);

  Dcm_Dsl_Supervisor_ProtocolHandling_DispatchTYPE1RequestToProtocol(TYPE1PresumptiveProtocolIndex);

  Dcm_Dsl_Supervisor_ProtocolHandling_ResetToDefaultIfSessionWasSetWithProtocolFromAnotherStack(TYPE1PresumptiveProtocolIndex);

  /* Request got dispatched, clear Presumptive. */
  Dcm_Dsl_Supervisor_ProtocolHandling_ClearPresumptiveProtocol(TYPE1PresumptiveProtocolIndex, TRUE);

}

STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_AttemptToDispatchPresumptiveTYPE1ProtocolOnStack(
  Dcm_StackNumberType StackNumber
)
{
  Dcm_ProtocolIndexType TYPE1PresumptiveProtocolIndex;
  Std_ReturnType StackCanAcceptRequest;

#if (DCM_META_DATA_HANDLING_ENABLED == STD_ON)
  StackCanAcceptRequest =
      Dcm_Dsl_Supervisor_ProtocolHandling_MainConnNotBusy_StackCanAcceptRequest(
          StackNumber, &TYPE1PresumptiveProtocolIndex,
          TRUE);

  if (DCM_E_OK == StackCanAcceptRequest)
  {
    Dcm_Dsl_Supervisor_ProtocolHandling_MetaDataInfoHandling(
        TYPE1PresumptiveProtocolIndex, DCM_NUM_MAIN_CONNECTIONS);
  }
#endif /* #if (DCM_META_DATA_HANDLING_ENABLED == STD_ON) */

  /* Attempt to dispatch the presumptive protocol, if there is one. */
  /* ENTER critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  StackCanAcceptRequest =
      Dcm_Dsl_Supervisor_ProtocolHandling_MainConnNotBusy_StackCanAcceptRequest(
          StackNumber, &TYPE1PresumptiveProtocolIndex, TRUE);

  if (DCM_E_ABORT != StackCanAcceptRequest)
  {
    if (DCM_E_NOT_OK != StackCanAcceptRequest)
    {
      DCM_PRECONDITION_ASSERT(
          Dcm_ProtocolStatus[TYPE1PresumptiveProtocolIndex].ProtocolState ==
            DCM_SUPERVISOR_PROTOCOL_STARTED_IDLE,
          DCM_INTERNAL_API_ID
        );

      if (DCM_PROTOCOL_UDS_GROUP ==
            Dcm_Dsl_Supervisor_GetProtocolGroupTypeForProtocol(TYPE1PresumptiveProtocolIndex))
      {
        Dcm_LatestExternalUDSProtocolIndex = TYPE1PresumptiveProtocolIndex;
      }

      Dcm_Dsl_Supervisor_ProtocolHandling_DispatchPresumptiveTYPE1Protocol(TYPE1PresumptiveProtocolIndex);
    }
    else
    {
      /* The request can't be accepted this cycle. */

      /* Check preemption timeout counter. It is positive value if preemption is needed. */
      if (Dcm_ProtocolQueue[TYPE1PresumptiveProtocolIndex].PreemptionTimeout > 0U)
      {
        /* Decrease preemption timeout counter */
        Dcm_ProtocolQueue[TYPE1PresumptiveProtocolIndex].PreemptionTimeout--;

        /* If preemption timeout counter elapsed reject request. */
        if (0U == Dcm_ProtocolQueue[TYPE1PresumptiveProtocolIndex].PreemptionTimeout)
        {
          /* Preemption has officially timed out, reject request. */
          Dcm_Dsl_Supervisor_ProtocolHandling_PreemptionPendingProcess(
            TYPE1PresumptiveProtocolIndex);
        }
      }
#if (DCM_HAS_ROE_TYPE2 == STD_ON)
      else
      {
        if (Dcm_ProtocolQueue[TYPE1PresumptiveProtocolIndex].ProcessingCancellingTimeout > 0U)
        {
          /* Decrease preemption timeout counter */
          Dcm_ProtocolQueue[TYPE1PresumptiveProtocolIndex].ProcessingCancellingTimeout--;

          /* If processing cancelling timeout counter elapsed reject request. */
          if (0U == Dcm_ProtocolQueue[TYPE1PresumptiveProtocolIndex].ProcessingCancellingTimeout)
          {
            /* Cancellation has officially timed out, reject request. */
            Dcm_Dsl_Supervisor_ProtocolHandling_PreemptionPendingProcess(
              TYPE1PresumptiveProtocolIndex);
          }
        }
      }
#endif /* #if (DCM_HAS_ROE_TYPE2 == STD_ON) */
    }
  }

  /* EXIT critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
}

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON))
STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_DispatchPresumptiveTYPE2Protocol(
  Dcm_ProtocolIndexType TYPE2PresumptiveProtocolIndex
)
{
  /* Switch state of the Protocol to a processing state. */
  Dcm_Dsl_Supervisor_ProtocolHandling_MakeTYPE2ProtocolProcess(TYPE2PresumptiveProtocolIndex);

  /* Dispatch request to Protocol*/
  Dcm_Dsl_Supervisor_ProtocolHandling_DispatchTYPE2RequestToProtocol(TYPE2PresumptiveProtocolIndex);

  /* Request got dispatched, clear Presumptive. */
  Dcm_Dsl_Supervisor_ProtocolHandling_ClearPresumptiveProtocol(TYPE2PresumptiveProtocolIndex, TRUE);
}
#endif /* #if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON)) */

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON))
STATIC FUNC (void, DCM_CODE) Dcm_Dsl_Supervisor_ProtocolHandling_AttemptToDispatchPresumptiveTYPE2ProtocolOnStack(
  Dcm_StackNumberType StackNumber
)
{
  Dcm_ProtocolIndexType TYPE2PresumptiveProtocolIndex;
  Std_ReturnType StackCanAcceptRequest;
#if (DCM_META_DATA_HANDLING_ENABLED == STD_ON)
  StackCanAcceptRequest =
      Dcm_Dsl_Supervisor_ProtocolHandling_MainConnNotBusy_StackCanAcceptRequest(
          StackNumber, &TYPE2PresumptiveProtocolIndex,
          FALSE);
  if(DCM_E_OK == StackCanAcceptRequest)
  {
    Dcm_Dsl_Supervisor_ProtocolHandling_MetaDataInfoHandling(
                                TYPE2PresumptiveProtocolIndex, DCM_NUM_MAIN_CONNECTIONS);
  }
#endif /* #if (DCM_META_DATA_HANDLING_ENABLED == STD_ON) */

 /* Attempt to dispatch the presumptive protocol, if there is one. */
  /* ENTER critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  StackCanAcceptRequest =
      Dcm_Dsl_Supervisor_ProtocolHandling_MainConnNotBusy_StackCanAcceptRequest(
          StackNumber, &TYPE2PresumptiveProtocolIndex,
          FALSE);

  if (DCM_E_ABORT != StackCanAcceptRequest)
  {
    if (DCM_E_NOT_OK != StackCanAcceptRequest)
    {
      /*
       * TYPE2 requests can be accepted in any configuration as long as the requested
       * Protocol is free to receive the request. If the Protocol became busy in the
       * meantime, drop the request.
       */
      Dcm_Dsl_Supervisor_ProtocolHandling_DispatchPresumptiveTYPE2Protocol(TYPE2PresumptiveProtocolIndex);
    }
    else
    {
      /* clear Presumptive. */
      Dcm_Dsl_Supervisor_ProtocolHandling_ClearPresumptiveProtocol(TYPE2PresumptiveProtocolIndex, TRUE);
    }
  }

  /* ENTER critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
}
#endif /* #if ((DCM_HAS_ROE_TYPE2 == STD_ON) || (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON)) */

/* !LINKSTO Dcm.Dsn.IB.Supervisor.ProtocolHandling.PreemptionPendingProcess.Discard,1 */
/* !LINKSTO Dcm.Dsn.IB.Supervisor.ProtocolHandling.PreemptionPendingProcess.TransmitBRR,1 */
STATIC FUNC (void, DCM_CODE)Dcm_Dsl_Supervisor_ProtocolHandling_PreemptionPendingProcess(
  Dcm_ProtocolIndexType ProtocolIndex)
{
  /* Preemption has officially timed out. Clear this PresumptiveProtocol and
     send an NRC if needed. */
  if ( ( (0U != Dcm_ProtocolQueue[ProtocolIndex].RCRRPsSent) ||
         (DCM_RESPOND_WITH_BUSYREPEATREQUEST == STD_ON)
        ) &&
        /* Periodic requests shall not trigger NRCs. */
        (DCM_PERIODIC_TYPE1_REQUEST !=
           Dcm_ProtocolQueue[ProtocolIndex].RequestType) &&
        /* ROE requests should not trigger the BRR NRC, since there really is no tester here
           attempting a request. */
        (DCM_ROE_TYPE1_PHYSICAL_REQUEST !=
         Dcm_ProtocolQueue[ProtocolIndex].RequestType) &&
        (DCM_ROE_TYPE1_FUNCTIONAL_REQUEST !=
         Dcm_ProtocolQueue[ProtocolIndex].RequestType)
     )
  {
    const Dcm_TimerValueType  NRCTimeout =
            Dcm_Dsl_DiagnosticSessionHandler_GetTimerValue(ProtocolIndex, DCM_P2TIMER);

    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NoCurrentProtocol.PreemptionTimeout.BRREnabled.TransmitBRR,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NoCurrentProtocol.PreemptionTimeout.BRRDisabled.RCRRPSent.TransmitBRR,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.OnCurrentProtocol.PreemptionTimeout.BRREnabled.TransmitBRR,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.OnCurrentProtocol.PreemptionTimeout.BRRDisabled.RCRRPSent.TransmitBRR,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.PreemptionTimeout.BRREnabled.TransmitBRR,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.PreemptionTimeout.BRRDisabled.RCRRPSent.TransmitBRR,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.PreemptionTimeout.BRREnabled.TransmitBRR,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.PreemptionTimeout.BRRDisabled.RCRRPSent.TransmitBRR,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.PreemptionTimeout.BRREnabled.TransmitBRR,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.PreemptionTimeout.BRRDisabled.RCRRPSent.TransmitBRR,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NoCurrentProtocol.PreemptionTimeout.BRREnabled.TransmitBRR,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NoCurrentProtocol.PreemptionTimeout.BRRDisabled.RCRRPSent.TransmitBRR,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.OnCurrentProtocol.PreemptionTimeout.BRREnabled.TransmitBRR,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.OnCurrentProtocol.PreemptionTimeout.BRRDisabled.RCRRPSent.TransmitBRR,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.PreemptionTimeout.BRREnabled.TransmitBRR,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.PreemptionTimeout.BRRDisabled.RCRRPSent.TransmitBRR,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.PreemptionTimeout.BRREnabled.TransmitBRR,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.PreemptionTimeout.BRRDisabled.RCRRPSent.TransmitBRR,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.PreemptionTimeout.BRREnabled.TransmitBRR,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.PreemptionTimeout.BRRDisabled.RCRRPSent.TransmitBRR,1 */

    Dcm_Dsl_Supervisor_MainConnectionHandling_InitiateNRC(
            Dcm_ProtocolQueue[ProtocolIndex].RxConnectionId,
            Dcm_ProtocolQueue[ProtocolIndex].RequestBuffer,
            DCM_E_BUSYREPEATREQUEST,
            NRCTimeout
          );

    (void)Dcm_Dsl_DiagnosticSessionHandler_StopTimer(DCM_SUPERVISOR_ID,
                                                     ProtocolIndex,
                                                     DCM_P2TIMER);
    /* If confirmation of RCRRP is missing P2Timer cannot be released. */
    if (Dcm_ProtocolQueue[ProtocolIndex].RCRRPsSent == Dcm_ProtocolQueue[ProtocolIndex].RCRRPsConfirmed)
    {
      (void)Dcm_Dsl_DiagnosticSessionHandler_FreeP2Timer(DCM_SUPERVISOR_ID,
                                                         ProtocolIndex);

      /* ... and then drop the request. */
      Dcm_Dsl_Supervisor_ProtocolHandling_ClearPresumptiveProtocol(ProtocolIndex, FALSE);
    }
  }
  else
  {
    Dcm_RxPduIdInfoType RxPduIdInfo;

    Dcm_Dsl_RxConnectionGetRxPduIdInformation(Dcm_ProtocolQueue[ProtocolIndex].RxConnectionId,
                                              &RxPduIdInfo);

    if (
        (DCM_NORMAL_PHYSICAL_REQUEST == Dcm_ProtocolQueue[ProtocolIndex].RequestType) ||
        (DCM_NORMAL_FUNCTIONAL_REQUEST == Dcm_ProtocolQueue[ProtocolIndex].RequestType) ||
        (DCM_JUMP_FROM_BL_RESPONSE_REQUEST == Dcm_ProtocolQueue[ProtocolIndex].RequestType) ||
        (DCM_JUMP_FROM_BL_NO_RESPONSE_REQUEST == Dcm_ProtocolQueue[ProtocolIndex].RequestType)
       )
    {
      if ((DCM_NORMAL_PHYSICAL_REQUEST == Dcm_ProtocolQueue[ProtocolIndex].RequestType) ||
          (DCM_NORMAL_FUNCTIONAL_REQUEST == Dcm_ProtocolQueue[ProtocolIndex].RequestType))
      {
        /* Stop P2Timer */
        (void)Dcm_Dsl_DiagnosticSessionHandler_StopTimer(DCM_SUPERVISOR_ID,
                                                         ProtocolIndex,
                                                         DCM_P2TIMER);

        /* Release P2Timer */
        (void)Dcm_Dsl_DiagnosticSessionHandler_FreeP2Timer(DCM_SUPERVISOR_ID,
                                                           ProtocolIndex);
      }

      /* Advertize end of communication sequence to SessionManager. */
      Dcm_Dsl_Supervisor_CommunicationSessionHandling_EndOfCommunicationSession(
              Dcm_ProtocolQueue[ProtocolIndex].RxConnectionId);

      /* And that all processing has been finished pertaining to this communication session.
         This checks to see whether S3 timers or ComMChannels need any handling. */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NoCurrentProtocol.PreemptionTimeout.BRRDisabled.NoRCRRPSent.Discard,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.OnCurrentProtocol.PreemptionTimeout.BRRDisabled.NoRCRRPSent.Discard,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.PreemptionTimeout.BRRDisabled.NoRCRRPSent.Discard,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.PreemptionTimeout.BRRDisabled.NoRCRRPSent.Discard,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.PreemptionTimeout.BRRDisabled.NoRCRRPSent.Discard,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NoCurrentProtocol.PreemptionTimeout.BRRDisabled.NoRCRRPSent.Discard,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.OnCurrentProtocol.PreemptionTimeout.BRRDisabled.NoRCRRPSent.Discard,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.PreemptionTimeout.BRRDisabled.NoRCRRPSent.Discard,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.PreemptionTimeout.BRRDisabled.NoRCRRPSent.Discard,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.PreemptionTimeout.BRRDisabled.NoRCRRPSent.Discard,1 */
      Dcm_Dsl_Supervisor_CommunicationSessionHandling_ProcessingEnd(
               Dcm_ProtocolQueue[ProtocolIndex].RxConnectionId);
    }
    /* And clear the MainConnection */
    Dcm_Dsl_Supervisor_MainConnectionHandling_ClearMainConnection(
            RxPduIdInfo.MainConnectionIndex);

    /* ... and then drop the request. */
    Dcm_Dsl_Supervisor_ProtocolHandling_ClearPresumptiveProtocol(ProtocolIndex, FALSE);
  }

}

STATIC FUNC (void, DCM_CODE)
  Dcm_Dsl_Supervisor_ProtocolHandling_CancelPresumptiveUDSProtocolsInNonDefaultSession(
    void
)
{
  uint8_least ProtocolIndex;

  /* Deviation TASKING-1 */
  for (ProtocolIndex = 0U; ProtocolIndex < DCM_NUM_PROTOCOLS; ProtocolIndex++)
  {
    const Dcm_ProtocolGroupType ProtocolGroup =
      Dcm_Dsl_Supervisor_ProtocolHandling_GetProtocolGroupTypeForProtocol(
          (Dcm_ProtocolIndexType)(ProtocolIndex)
        );

    if (
        (DCM_SUPERVISOR_PROTOCOL_QUEUE_IDLE != Dcm_ProtocolQueue[ProtocolIndex].ProtocolQueueState) &&
        (DCM_PROTOCOL_UDS_GROUP == ProtocolGroup)
       )
    {

      /* Cancellation of PresumptiveProtocols is more or less instantaneous, there's nothing to
         wait for here... */
      Dcm_Dsl_Supervisor_ProtocolHandling_CancelPresumptiveProtocol(
          (Dcm_ProtocolIndexType)(ProtocolIndex)
        );
    }
  }

}

#define DCM_STOP_SEC_CODE
#include <MemMap.h>
/*==================[end of file]================================================================*/
