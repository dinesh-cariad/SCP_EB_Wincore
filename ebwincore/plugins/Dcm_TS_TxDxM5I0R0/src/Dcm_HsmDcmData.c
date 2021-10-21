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

/* Definition of the configuration data of the state machine
 * Dcm.
 *
 * This file contains the definition of all used data structures of the state
 * machine model.  It is generated and must not be edited.
 */

/*  MISRA-C:2004 Deviation List
 *
 *  MISRA-1) Deviated Rule: 6.3 (advisory)
 *     'typedefs' that indicate size and signedness should be used in place
 *     of the basic types.
 *
 *     Reason:
 *     Platform depended 'char' type is only used to define debugging
 *     strings on Windows.
 */

/*==================[inclusions]============================================*/

#include <Std_Types.h>                            /* AUTOSAR standard types */

#include <Dcm_Hsm.h> /* state machine driver interface */
#include <Dcm_HsmDcm.h> /* public statechart model definitions, */
#include <Dcm_HsmDcmInt.h> /* internal statechart model definitions */

/*==================[macros]================================================*/

/** \brief Number of user events */
#define DCM_HSM_DCM_NUM_EVENTS 21U

/* Check if number of hsm instances is defined in the user's configuration */
#ifndef DCM_HSM_DCM_NUM_INST
# error DCM_HSM_DCM_NUM_INST must be defined in Dcm_HsmCfg.h
#endif

/* Check if event queue size of hsm instances is defined in the user's
 * configuration */
#ifndef DCM_HSM_DCM_EV_QUEUE_SIZE
# error DCM_HSM_DCM_EV_QUEUE_SIZE must be defined in Dcm_HsmCfg.h
#endif

/*==================[type definitions]======================================*/

/*==================[internal function declarations]========================*/

/*==================[external function declarations]========================*/

/*==================[internal data]=========================================*/

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/* RAM data for instances */
STATIC VAR(Dcm_HsmInstType, DCM_VAR_CLEARED)
  Dcm_HsmInstDcm[DCM_HSM_DCM_NUM_INST];

/* RAM data for event queues */
STATIC VAR(Dcm_HsmEventType, DCM_VAR_CLEARED)
  Dcm_HsmEvQueuesDcm[
    DCM_HSM_DCM_NUM_INST * DCM_HSM_DCM_EV_QUEUE_SIZE];

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/*==================[external data]=========================================*/

/*==================[internal constants]====================================*/

#define DCM_START_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

/* *** array of entry/exit and transition actions *** */
STATIC CONST(Dcm_HsmActionType, DCM_CONST)
  Dcm_HsmDcmActions[DCM_HSM_DCM_NO_OF_ACTIONS] =
{
  &Dcm_HsmDcmSfTopEntry,
  &Dcm_HsmDcmSfWaitForFullComAction1,
  &Dcm_HsmDcmSfWaitForFullComAction2,
  &Dcm_HsmDcmSfWaitForFullComAction3,
  &Dcm_HsmDcmSfOutOfServiceAction2,
  &Dcm_HsmDcmSfOutOfServiceAction4,
  &Dcm_HsmDcmSfOutOfServiceAction7,
  &Dcm_HsmDcmSfOutOfServiceAction8,
  &Dcm_HsmDcmSfOosInitAction1,
  &Dcm_HsmDcmSfOosNotifiedEntry,
  &Dcm_HsmDcmSfInServiceEntry,
  &Dcm_HsmDcmSfInServiceAction1,
  &Dcm_HsmDcmSfConfirmingNOkEntry,
  &Dcm_HsmDcmSfConfirmingOkEntry,
  &Dcm_HsmDcmSfPeriodicProcessingAction1,
  &Dcm_HsmDcmSfPeriodicResponseProcessingEntry,
  &Dcm_HsmDcmSfPeriodicProcessingUUDTAction1,
  &Dcm_HsmDcmSfPeriodicSendingUUDTEntry,
  &Dcm_HsmDcmSfPeriodicSendingUUDTAction1,
  &Dcm_HsmDcmSfPeriodicSendingUUDTFinishingAction1,
  &Dcm_HsmDcmSfPeriodicSendingUUDTFinishingAction2,
  &Dcm_HsmDcmSfROEProcessingAction3,
  &Dcm_HsmDcmSfROEResponseProcessingEntry,
  &Dcm_HsmDcmSfWaitForRoeProcessingCancellationEntry,
  &Dcm_HsmDcmSfRequestProcessingEntry,
  &Dcm_HsmDcmSfRequestProcessingAction3,
  &Dcm_HsmDcmSfRequestCancellingEntry,
  &Dcm_HsmDcmSfWaitForProtocolEntry,
  &Dcm_HsmDcmSfWaitForTxAndProtocolEntry,
  &Dcm_HsmDcmSfCancelWithGeneralRejectEntry,
  &Dcm_HsmDcmSfCancelWithGeneralRejectAction1,
  &Dcm_HsmDcmSfSendRpEntry,
  &Dcm_HsmDcmSfSendRpExit,
  &Dcm_HsmDcmSfSendRpForcedAction1,
  &Dcm_HsmDcmSfSendingEntry,
  &Dcm_HsmDcmSfNormalSendingCancellingEntry,
  &Dcm_HsmDcmSfPgBufSendingCancellingEntry,
  &Dcm_HsmDcmSfPgBufDataAvailableAction1,
  &Dcm_HsmDcmSfPgBufDataRequestedAction1,
};

/* *** array of guards *** */
STATIC CONST(Dcm_HsmGuardType, DCM_CONST)
  Dcm_HsmDcmGuards[DCM_HSM_DCM_NO_OF_GUARDS] =
{
  &Dcm_HsmDcmSfWaitForFullComGuard2,
  &Dcm_HsmDcmSfWaitForFullComGuard3,
  &Dcm_HsmDcmSfOutOfServiceGuard1,
  &Dcm_HsmDcmSfOutOfServiceGuard2,
  &Dcm_HsmDcmSfOutOfServiceGuard3,
  &Dcm_HsmDcmSfOutOfServiceGuard4,
  &Dcm_HsmDcmSfOutOfServiceGuard7,
  &Dcm_HsmDcmSfOutOfServiceGuard8,
  &Dcm_HsmDcmSfWaitForApplGuard2,
  &Dcm_HsmDcmSfWaitForApplGuard3,
  &Dcm_HsmDcmSfWaitForApplGuard4,
  &Dcm_HsmDcmSfWaitForApplGuard5,
};

#define DCM_STOP_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

#define DCM_START_SEC_CONST_8
#include <Dcm_MemMap.h>

/* *** exit/action/entry sequences for transitions *** */
/* trans sequence for transition #1 from state WaitForFullCom */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrWaitForFullComSteps1[1] =
{
  DCM_HSM_DCM_AIDX_WAITFORFULLCOM_ACTION_1,
};
/* trans sequence for transition #2 from state WaitForFullCom */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrWaitForFullComSteps2[3] =
{
  DCM_HSM_DCM_AIDX_WAITFORFULLCOM_ACTION_2,
  DCM_HSM_DCM_AIDX_INSERVICE_ENTRY,
  DCM_HSM_DCM_AIDX_SENDING_ENTRY,
};
/* trans sequence for transition #3 from state WaitForFullCom */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrWaitForFullComSteps3[2] =
{
  DCM_HSM_DCM_AIDX_WAITFORFULLCOM_ACTION_3,
  DCM_HSM_DCM_AIDX_INSERVICE_ENTRY,
};
/* trans sequence for transition #1 from state OutOfService */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrOutOfServiceSteps1[2] =
{
  DCM_HSM_DCM_AIDX_INSERVICE_ENTRY,
  DCM_HSM_DCM_AIDX_PERIODICRESPONSEPROCESSING_ENTRY,
};
/* trans sequence for transition #2 from state OutOfService */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrOutOfServiceSteps2[1] =
{
  DCM_HSM_DCM_AIDX_OUTOFSERVICE_ACTION_2,
};
/* trans sequence for transition #3 from state OutOfService */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrOutOfServiceSteps3[1] =
{
  DCM_HSM_DCM_AIDX_INSERVICE_ENTRY,
};
/* trans sequence for transition #4 from state OutOfService */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrOutOfServiceSteps4[1] =
{
  DCM_HSM_DCM_AIDX_OUTOFSERVICE_ACTION_4,
};
/* trans sequence for transition #5 from state OutOfService */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrOutOfServiceSteps5[2] =
{
  DCM_HSM_DCM_AIDX_INSERVICE_ENTRY,
  DCM_HSM_DCM_AIDX_REQUESTPROCESSING_ENTRY,
};
/* trans sequence for transition #6 from state OutOfService */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrOutOfServiceSteps6[3] =
{
  DCM_HSM_DCM_AIDX_INSERVICE_ENTRY,
  DCM_HSM_DCM_AIDX_REQUESTPROCESSING_ENTRY,
  DCM_HSM_DCM_AIDX_SENDRP_ENTRY,
};
/* trans sequence for transition #7 from state OutOfService */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrOutOfServiceSteps7[3] =
{
  DCM_HSM_DCM_AIDX_OUTOFSERVICE_ACTION_7,
  DCM_HSM_DCM_AIDX_INSERVICE_ENTRY,
  DCM_HSM_DCM_AIDX_ROERESPONSEPROCESSING_ENTRY,
};
/* trans sequence for transition #8 from state OutOfService */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrOutOfServiceSteps8[1] =
{
  DCM_HSM_DCM_AIDX_OUTOFSERVICE_ACTION_8,
};
/* trans sequence for transition #1 from state OosInit */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrOosInitSteps1[1] =
{
  DCM_HSM_DCM_AIDX_OOSINIT_ACTION_1,
};
/* trans sequence for transition #1 from state InService */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrInServiceSteps1[1] =
{
  DCM_HSM_DCM_AIDX_INSERVICE_ACTION_1,
};
/* trans sequence for transition #1 from state Finishing */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrFinishingSteps1[1] =
{
  DCM_HSM_DCM_AIDX_OOSNOTIFIED_ENTRY,
};
/* trans sequence for transition #1 from state PeriodicProcessing */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrPeriodicProcessingSteps1[2] =
{
  DCM_HSM_DCM_AIDX_PERIODICPROCESSING_ACTION_1,
  DCM_HSM_DCM_AIDX_OOSNOTIFIED_ENTRY,
};
/* trans sequence for transition #2 from state PeriodicProcessing */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrPeriodicProcessingSteps2[1] =
{
  DCM_HSM_DCM_AIDX_SENDING_ENTRY,
};
/* trans sequence for transition #1 from state PeriodicProcessingUUDT */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrPeriodicProcessingUUDTSteps1[2] =
{
  DCM_HSM_DCM_AIDX_PERIODICPROCESSINGUUDT_ACTION_1,
  DCM_HSM_DCM_AIDX_OOSNOTIFIED_ENTRY,
};
/* trans sequence for transition #1 from state PeriodicSendingUUDT */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrPeriodicSendingUUDTSteps1[1] =
{
  DCM_HSM_DCM_AIDX_PERIODICSENDINGUUDT_ACTION_1,
};
/* trans sequence for transition #1 from state PeriodicSendingUUDTFinishing */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrPeriodicSendingUUDTFinishingSteps1[1] =
{
  DCM_HSM_DCM_AIDX_PERIODICSENDINGUUDTFINISHING_ACTION_1,
};
/* trans sequence for transition #2 from state PeriodicSendingUUDTFinishing */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrPeriodicSendingUUDTFinishingSteps2[2] =
{
  DCM_HSM_DCM_AIDX_PERIODICSENDINGUUDTFINISHING_ACTION_2,
  DCM_HSM_DCM_AIDX_OOSNOTIFIED_ENTRY,
};
/* trans sequence for transition #1 from state ROEProcessing */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrROEProcessingSteps1[1] =
{
  DCM_HSM_DCM_AIDX_OOSNOTIFIED_ENTRY,
};
/* trans sequence for transition #2 from state ROEProcessing */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrROEProcessingSteps2[1] =
{
  DCM_HSM_DCM_AIDX_SENDING_ENTRY,
};
/* trans sequence for transition #3 from state ROEProcessing */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrROEProcessingSteps3[2] =
{
  DCM_HSM_DCM_AIDX_ROEPROCESSING_ACTION_3,
  DCM_HSM_DCM_AIDX_OOSNOTIFIED_ENTRY,
};
/* trans sequence for transition #4 from state ROEProcessing */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrROEProcessingSteps4[1] =
{
  DCM_HSM_DCM_AIDX_SENDING_ENTRY,
};
/* trans sequence for transition #1 from state ROEResponseProcessing */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrROEResponseProcessingSteps1[1] =
{
  DCM_HSM_DCM_AIDX_WAITFORROEPROCESSINGCANCELLATION_ENTRY,
};
/* trans sequence for transition #1 from state RequestProcessing */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrRequestProcessingSteps1[1] =
{
  DCM_HSM_DCM_AIDX_OOSNOTIFIED_ENTRY,
};
/* trans sequence for transition #2 from state RequestProcessing */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrRequestProcessingSteps2[1] =
{
  DCM_HSM_DCM_AIDX_SENDING_ENTRY,
};
/* trans sequence for transition #3 from state RequestProcessing */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrRequestProcessingSteps3[2] =
{
  DCM_HSM_DCM_AIDX_REQUESTPROCESSING_ACTION_3,
  DCM_HSM_DCM_AIDX_OOSNOTIFIED_ENTRY,
};
/* trans sequence for transition #4 from state RequestProcessing */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrRequestProcessingSteps4[1] =
{
  DCM_HSM_DCM_AIDX_SENDING_ENTRY,
};
/* trans sequence for transition #1 from state CancelWithGeneralReject */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrCancelWithGeneralRejectSteps1[1] =
{
  DCM_HSM_DCM_AIDX_CANCELWITHGENERALREJECT_ACTION_1,
};
/* trans sequence for transition #1 from state SendRp */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrSendRpSteps1[3] =
{
  DCM_HSM_DCM_AIDX_SENDRP_EXIT,
  DCM_HSM_DCM_AIDX_REQUESTCANCELLING_ENTRY,
  DCM_HSM_DCM_AIDX_WAITFORTXANDPROTOCOL_ENTRY,
};
/* trans sequence for transition #2 from state SendRp */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrSendRpSteps2[3] =
{
  DCM_HSM_DCM_AIDX_SENDRP_EXIT,
  DCM_HSM_DCM_AIDX_REQUESTCANCELLING_ENTRY,
  DCM_HSM_DCM_AIDX_WAITFORPROTOCOL_ENTRY,
};
/* trans sequence for transition #1 from state SendRpForced */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrSendRpForcedSteps1[2] =
{
  DCM_HSM_DCM_AIDX_SENDRP_EXIT,
  DCM_HSM_DCM_AIDX_SENDRPFORCED_ACTION_1,
};
/* trans sequence for transition #1 from state SendRpNormal */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrSendRpNormalSteps1[1] =
{
  DCM_HSM_DCM_AIDX_SENDRP_EXIT,
};
/* trans sequence for transition #1 from state WaitForAppl */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrWaitForApplSteps1[2] =
{
  DCM_HSM_DCM_AIDX_REQUESTCANCELLING_ENTRY,
  DCM_HSM_DCM_AIDX_WAITFORPROTOCOL_ENTRY,
};
/* trans sequence for transition #2 from state WaitForAppl */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrWaitForApplSteps2[2] =
{
  DCM_HSM_DCM_AIDX_REQUESTCANCELLING_ENTRY,
  DCM_HSM_DCM_AIDX_CANCELWITHGENERALREJECT_ENTRY,
};
/* trans sequence for transition #3 from state WaitForAppl */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrWaitForApplSteps3[1] =
{
  DCM_HSM_DCM_AIDX_SENDRP_ENTRY,
};
/* trans sequence for transition #4 from state WaitForAppl */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrWaitForApplSteps4[2] =
{
  DCM_HSM_DCM_AIDX_REQUESTCANCELLING_ENTRY,
  DCM_HSM_DCM_AIDX_CANCELWITHGENERALREJECT_ENTRY,
};
/* trans sequence for transition #5 from state WaitForAppl */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrWaitForApplSteps5[1] =
{
  DCM_HSM_DCM_AIDX_SENDRP_ENTRY,
};
/* trans sequence for transition #1 from state Sending */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrSendingSteps1[1] =
{
  DCM_HSM_DCM_AIDX_CONFIRMINGNOK_ENTRY,
};
/* trans sequence for transition #2 from state Sending */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrSendingSteps2[1] =
{
  DCM_HSM_DCM_AIDX_CONFIRMINGNOK_ENTRY,
};
/* trans sequence for transition #3 from state Sending */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrSendingSteps3[1] =
{
  DCM_HSM_DCM_AIDX_CONFIRMINGOK_ENTRY,
};
/* trans sequence for transition #1 from state NormalSendingProcessing */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrNormalSendingProcessingSteps1[1] =
{
  DCM_HSM_DCM_AIDX_NORMALSENDINGCANCELLING_ENTRY,
};
/* trans sequence for transition #1 from state PgBufSendingProcessing */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrPgBufSendingProcessingSteps1[1] =
{
  DCM_HSM_DCM_AIDX_PGBUFSENDINGCANCELLING_ENTRY,
};
/* trans sequence for transition #1 from state PgBufDataAvailable */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrPgBufDataAvailableSteps1[1] =
{
  DCM_HSM_DCM_AIDX_PGBUFDATAAVAILABLE_ACTION_1,
};
/* trans sequence for transition #1 from state PgBufDataRequested */
STATIC CONST(Dcm_HsmActionIdxType, DCM_CONST)
  Dcm_HsmTrPgBufDataRequestedSteps1[1] =
{
  DCM_HSM_DCM_AIDX_PGBUFDATAREQUESTED_ACTION_1,
};

#define DCM_STOP_SEC_CONST_8
#include <Dcm_MemMap.h>


#define DCM_START_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

/* transitions from state WaitForFullCom */
STATIC CONST(Dcm_HsmTranType, DCM_CONST)
 Dcm_HsmTrWaitForFullCom[3] =
{
{
  Dcm_HsmTrWaitForFullComSteps1,
  sizeof(Dcm_HsmTrWaitForFullComSteps1)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_RETRY, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_SIDX_INVALID /* no target state, internal transition */
},
{
  Dcm_HsmTrWaitForFullComSteps2,
  sizeof(Dcm_HsmTrWaitForFullComSteps2)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_BL_TX_RESPONSE, /* triggering event */
  DCM_HSM_DCM_GIDX_WAITFORFULLCOM_GUARD_2, /* guard idx */
  DCM_HSM_DCM_SIDX_NORMALSENDING /* target state */
},
{
  Dcm_HsmTrWaitForFullComSteps3,
  sizeof(Dcm_HsmTrWaitForFullComSteps3)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_BL_TX_RESPONSE, /* triggering event */
  DCM_HSM_DCM_GIDX_WAITFORFULLCOM_GUARD_3, /* guard idx */
  DCM_HSM_DCM_SIDX_FINISHING /* target state */
},
};
/* transitions from state OutOfService */
STATIC CONST(Dcm_HsmTranType, DCM_CONST)
 Dcm_HsmTrOutOfService[8] =
{
{
  Dcm_HsmTrOutOfServiceSteps1,
  sizeof(Dcm_HsmTrOutOfServiceSteps1)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_RDBPI_TYPE1, /* triggering event */
  DCM_HSM_DCM_GIDX_OUTOFSERVICE_GUARD_1, /* guard idx */
  DCM_HSM_DCM_SIDX_PERIODICRESPONSEPROCESSING /* target state */
},
{
  Dcm_HsmTrOutOfServiceSteps2,
  sizeof(Dcm_HsmTrOutOfServiceSteps2)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_RDBPI_TYPE1, /* triggering event */
  DCM_HSM_DCM_GIDX_OUTOFSERVICE_GUARD_2, /* guard idx */
  DCM_HSM_DCM_SIDX_OUTOFSERVICE /* target state */
},
{
  Dcm_HsmTrOutOfServiceSteps3,
  sizeof(Dcm_HsmTrOutOfServiceSteps3)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_RDBPI_TYPE2, /* triggering event */
  DCM_HSM_DCM_GIDX_OUTOFSERVICE_GUARD_3, /* guard idx */
  DCM_HSM_DCM_SIDX_PERIODICPROCESSINGUUDT /* target state */
},
{
  Dcm_HsmTrOutOfServiceSteps4,
  sizeof(Dcm_HsmTrOutOfServiceSteps4)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_RDBPI_TYPE2, /* triggering event */
  DCM_HSM_DCM_GIDX_OUTOFSERVICE_GUARD_4, /* guard idx */
  DCM_HSM_DCM_SIDX_OUTOFSERVICE /* target state */
},
{
  Dcm_HsmTrOutOfServiceSteps5,
  sizeof(Dcm_HsmTrOutOfServiceSteps5)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_REQUEST_RECEIVED, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_DCM_SIDX_REQUESTPROCESSING /* target state */
},
{
  Dcm_HsmTrOutOfServiceSteps6,
  sizeof(Dcm_HsmTrOutOfServiceSteps6)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_REQUEST_RECEIVED_RCRRP, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_DCM_SIDX_SENDRPNORMAL /* target state */
},
{
  Dcm_HsmTrOutOfServiceSteps7,
  sizeof(Dcm_HsmTrOutOfServiceSteps7)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_ROE, /* triggering event */
  DCM_HSM_DCM_GIDX_OUTOFSERVICE_GUARD_7, /* guard idx */
  DCM_HSM_DCM_SIDX_ROERESPONSEPROCESSING /* target state */
},
{
  Dcm_HsmTrOutOfServiceSteps8,
  sizeof(Dcm_HsmTrOutOfServiceSteps8)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_ROE, /* triggering event */
  DCM_HSM_DCM_GIDX_OUTOFSERVICE_GUARD_8, /* guard idx */
  DCM_HSM_DCM_SIDX_OUTOFSERVICE /* target state */
},
};
/* transitions from state OosInit */
STATIC CONST(Dcm_HsmTranType, DCM_CONST)
 Dcm_HsmTrOosInit[1] =
{
{
  Dcm_HsmTrOosInitSteps1,
  sizeof(Dcm_HsmTrOosInitSteps1)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_RETURN_FROM_BL, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_DCM_SIDX_WAITFORFULLCOM /* target state */
},
};
/* transitions from state InService */
STATIC CONST(Dcm_HsmTranType, DCM_CONST)
 Dcm_HsmTrInService[1] =
{
{
  Dcm_HsmTrInServiceSteps1,
  sizeof(Dcm_HsmTrInServiceSteps1)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_RETRY, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_SIDX_INVALID /* no target state, internal transition */
},
};
/* transitions from state Finishing */
STATIC CONST(Dcm_HsmTranType, DCM_CONST)
 Dcm_HsmTrFinishing[1] =
{
{
  Dcm_HsmTrFinishingSteps1,
  sizeof(Dcm_HsmTrFinishingSteps1)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_PROC_DONE, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_DCM_SIDX_OOSNOTIFIED /* target state */
},
};
/* transitions from state PeriodicProcessing */
STATIC CONST(Dcm_HsmTranType, DCM_CONST)
 Dcm_HsmTrPeriodicProcessing[2] =
{
{
  Dcm_HsmTrPeriodicProcessingSteps1,
  sizeof(Dcm_HsmTrPeriodicProcessingSteps1)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_PROC_ABORT, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_DCM_SIDX_OOSNOTIFIED /* target state */
},
{
  Dcm_HsmTrPeriodicProcessingSteps2,
  sizeof(Dcm_HsmTrPeriodicProcessingSteps2)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_PROC_DONE, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_DCM_SIDX_SENDING /* target state */
},
};
/* transitions from state PeriodicProcessingUUDT */
STATIC CONST(Dcm_HsmTranType, DCM_CONST)
 Dcm_HsmTrPeriodicProcessingUUDT[1] =
{
{
  Dcm_HsmTrPeriodicProcessingUUDTSteps1,
  sizeof(Dcm_HsmTrPeriodicProcessingUUDTSteps1)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_PROC_ABORT, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_DCM_SIDX_OOSNOTIFIED /* target state */
},
};
/* transitions from state PeriodicSendingUUDT */
STATIC CONST(Dcm_HsmTranType, DCM_CONST)
 Dcm_HsmTrPeriodicSendingUUDT[2] =
{
{
  Dcm_HsmTrPeriodicSendingUUDTSteps1,
  sizeof(Dcm_HsmTrPeriodicSendingUUDTSteps1)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_PERIODIC_UUDT_NEXT, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_SIDX_INVALID /* no target state, internal transition */
},
{
  NULL_PTR,
  0U,
  DCM_HSM_DCM_EV_PERIODIC_UUDT_DONE, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_DCM_SIDX_PERIODICSENDINGUUDTFINISHING /* target state */
},
};
/* transitions from state PeriodicSendingUUDTFinishing */
STATIC CONST(Dcm_HsmTranType, DCM_CONST)
 Dcm_HsmTrPeriodicSendingUUDTFinishing[2] =
{
{
  Dcm_HsmTrPeriodicSendingUUDTFinishingSteps1,
  sizeof(Dcm_HsmTrPeriodicSendingUUDTFinishingSteps1)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_PERIODIC_UUDT_CONFIRM, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_SIDX_INVALID /* no target state, internal transition */
},
{
  Dcm_HsmTrPeriodicSendingUUDTFinishingSteps2,
  sizeof(Dcm_HsmTrPeriodicSendingUUDTFinishingSteps2)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_PROC_DONE, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_DCM_SIDX_OOSNOTIFIED /* target state */
},
};
/* transitions from state ROEProcessing */
STATIC CONST(Dcm_HsmTranType, DCM_CONST)
 Dcm_HsmTrROEProcessing[4] =
{
{
  Dcm_HsmTrROEProcessingSteps1,
  sizeof(Dcm_HsmTrROEProcessingSteps1)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_CANCELLATION_DONE, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_DCM_SIDX_OOSNOTIFIED /* target state */
},
{
  Dcm_HsmTrROEProcessingSteps2,
  sizeof(Dcm_HsmTrROEProcessingSteps2)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_PROCESS_PAGE, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_DCM_SIDX_PGBUFSENDING /* target state */
},
{
  Dcm_HsmTrROEProcessingSteps3,
  sizeof(Dcm_HsmTrROEProcessingSteps3)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_PROC_ABORT, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_DCM_SIDX_OOSNOTIFIED /* target state */
},
{
  Dcm_HsmTrROEProcessingSteps4,
  sizeof(Dcm_HsmTrROEProcessingSteps4)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_PROC_DONE, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_DCM_SIDX_NORMALSENDING /* target state */
},
};
/* transitions from state ROEResponseProcessing */
STATIC CONST(Dcm_HsmTranType, DCM_CONST)
 Dcm_HsmTrROEResponseProcessing[1] =
{
{
  Dcm_HsmTrROEResponseProcessingSteps1,
  sizeof(Dcm_HsmTrROEResponseProcessingSteps1)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_CANCEL_PROCESSING, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_DCM_SIDX_WAITFORROEPROCESSINGCANCELLATION /* target state */
},
};
/* transitions from state RequestProcessing */
STATIC CONST(Dcm_HsmTranType, DCM_CONST)
 Dcm_HsmTrRequestProcessing[4] =
{
{
  Dcm_HsmTrRequestProcessingSteps1,
  sizeof(Dcm_HsmTrRequestProcessingSteps1)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_CANCELLATION_DONE, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_DCM_SIDX_OOSNOTIFIED /* target state */
},
{
  Dcm_HsmTrRequestProcessingSteps2,
  sizeof(Dcm_HsmTrRequestProcessingSteps2)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_PROCESS_PAGE, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_DCM_SIDX_PGBUFSENDING /* target state */
},
{
  Dcm_HsmTrRequestProcessingSteps3,
  sizeof(Dcm_HsmTrRequestProcessingSteps3)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_PROC_ABORT, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_DCM_SIDX_OOSNOTIFIED /* target state */
},
{
  Dcm_HsmTrRequestProcessingSteps4,
  sizeof(Dcm_HsmTrRequestProcessingSteps4)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_PROC_DONE, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_DCM_SIDX_NORMALSENDING /* target state */
},
};
/* transitions from state CancelWithGeneralReject */
STATIC CONST(Dcm_HsmTranType, DCM_CONST)
 Dcm_HsmTrCancelWithGeneralReject[1] =
{
{
  Dcm_HsmTrCancelWithGeneralRejectSteps1,
  sizeof(Dcm_HsmTrCancelWithGeneralRejectSteps1)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_CANCEL_PROCESSING, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_SIDX_INVALID /* no target state, internal transition */
},
};
/* transitions from state SendRp */
STATIC CONST(Dcm_HsmTranType, DCM_CONST)
 Dcm_HsmTrSendRp[2] =
{
{
  Dcm_HsmTrSendRpSteps1,
  sizeof(Dcm_HsmTrSendRpSteps1)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_CANCEL_PROCESSING, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_DCM_SIDX_WAITFORTXANDPROTOCOL /* target state */
},
{
  Dcm_HsmTrSendRpSteps2,
  sizeof(Dcm_HsmTrSendRpSteps2)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_TX_CONF_NOT_OK, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_DCM_SIDX_WAITFORPROTOCOL /* target state */
},
};
/* transitions from state SendRpForced */
STATIC CONST(Dcm_HsmTranType, DCM_CONST)
 Dcm_HsmTrSendRpForced[1] =
{
{
  Dcm_HsmTrSendRpForcedSteps1,
  sizeof(Dcm_HsmTrSendRpForcedSteps1)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_TX_CONF_OK, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_DCM_SIDX_WAITFORAPPL /* target state */
},
};
/* transitions from state SendRpNormal */
STATIC CONST(Dcm_HsmTranType, DCM_CONST)
 Dcm_HsmTrSendRpNormal[1] =
{
{
  Dcm_HsmTrSendRpNormalSteps1,
  sizeof(Dcm_HsmTrSendRpNormalSteps1)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_TX_CONF_OK, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_DCM_SIDX_WAITFORAPPL /* target state */
},
};
/* transitions from state WaitForAppl */
STATIC CONST(Dcm_HsmTranType, DCM_CONST)
 Dcm_HsmTrWaitForAppl[5] =
{
{
  Dcm_HsmTrWaitForApplSteps1,
  sizeof(Dcm_HsmTrWaitForApplSteps1)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_CANCEL_PROCESSING, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_DCM_SIDX_WAITFORPROTOCOL /* target state */
},
{
  Dcm_HsmTrWaitForApplSteps2,
  sizeof(Dcm_HsmTrWaitForApplSteps2)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_FORCE_RCRRP, /* triggering event */
  DCM_HSM_DCM_GIDX_WAITFORAPPL_GUARD_2, /* guard idx */
  DCM_HSM_DCM_SIDX_CANCELWITHGENERALREJECT /* target state */
},
{
  Dcm_HsmTrWaitForApplSteps3,
  sizeof(Dcm_HsmTrWaitForApplSteps3)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_FORCE_RCRRP, /* triggering event */
  DCM_HSM_DCM_GIDX_WAITFORAPPL_GUARD_3, /* guard idx */
  DCM_HSM_DCM_SIDX_SENDRPFORCED /* target state */
},
{
  Dcm_HsmTrWaitForApplSteps4,
  sizeof(Dcm_HsmTrWaitForApplSteps4)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_TIMEOUT2, /* triggering event */
  DCM_HSM_DCM_GIDX_WAITFORAPPL_GUARD_4, /* guard idx */
  DCM_HSM_DCM_SIDX_CANCELWITHGENERALREJECT /* target state */
},
{
  Dcm_HsmTrWaitForApplSteps5,
  sizeof(Dcm_HsmTrWaitForApplSteps5)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_TIMEOUT2, /* triggering event */
  DCM_HSM_DCM_GIDX_WAITFORAPPL_GUARD_5, /* guard idx */
  DCM_HSM_DCM_SIDX_SENDRPNORMAL /* target state */
},
};
/* transitions from state Sending */
STATIC CONST(Dcm_HsmTranType, DCM_CONST)
 Dcm_HsmTrSending[3] =
{
{
  Dcm_HsmTrSendingSteps1,
  sizeof(Dcm_HsmTrSendingSteps1)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_CANCELLATION_DONE, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_DCM_SIDX_CONFIRMINGNOK /* target state */
},
{
  Dcm_HsmTrSendingSteps2,
  sizeof(Dcm_HsmTrSendingSteps2)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_TX_CONF_NOT_OK, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_DCM_SIDX_CONFIRMINGNOK /* target state */
},
{
  Dcm_HsmTrSendingSteps3,
  sizeof(Dcm_HsmTrSendingSteps3)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_TX_CONF_OK, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_DCM_SIDX_CONFIRMINGOK /* target state */
},
};
/* transitions from state NormalSendingProcessing */
STATIC CONST(Dcm_HsmTranType, DCM_CONST)
 Dcm_HsmTrNormalSendingProcessing[1] =
{
{
  Dcm_HsmTrNormalSendingProcessingSteps1,
  sizeof(Dcm_HsmTrNormalSendingProcessingSteps1)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_CANCEL_PROCESSING, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_DCM_SIDX_NORMALSENDINGCANCELLING /* target state */
},
};
/* transitions from state PgBufSendingProcessing */
STATIC CONST(Dcm_HsmTranType, DCM_CONST)
 Dcm_HsmTrPgBufSendingProcessing[1] =
{
{
  Dcm_HsmTrPgBufSendingProcessingSteps1,
  sizeof(Dcm_HsmTrPgBufSendingProcessingSteps1)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_CANCEL_PROCESSING, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_DCM_SIDX_PGBUFSENDINGCANCELLING /* target state */
},
};
/* transitions from state PgBufDataAvailable */
STATIC CONST(Dcm_HsmTranType, DCM_CONST)
 Dcm_HsmTrPgBufDataAvailable[1] =
{
{
  Dcm_HsmTrPgBufDataAvailableSteps1,
  sizeof(Dcm_HsmTrPgBufDataAvailableSteps1)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_PROVIDE_TX_BUF, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_DCM_SIDX_PGBUFDATAREQUESTED /* target state */
},
};
/* transitions from state PgBufDataRequested */
STATIC CONST(Dcm_HsmTranType, DCM_CONST)
 Dcm_HsmTrPgBufDataRequested[1] =
{
{
  Dcm_HsmTrPgBufDataRequestedSteps1,
  sizeof(Dcm_HsmTrPgBufDataRequestedSteps1)/sizeof(Dcm_HsmActionIdxType),
  DCM_HSM_DCM_EV_PROCESS_PAGE, /* triggering event */
  DCM_HSM_GIDX_INVALID, /* no guard */
  DCM_HSM_DCM_SIDX_PGBUFDATAAVAILABLE /* target state */
},
};



/* statechart model description of the Dcm state
   machine */
STATIC CONST(Dcm_HsmStateType, DCM_CONST)
  Dcm_HsmScdDcm[DCM_HSM_DCM_NO_OF_STATES] =
{
  {
  /* description of state Top */
0, /* no non-deferred event mask (no leaf state) */
0, /* no ignored event mask (no leaf state) */
    NULL_PTR,  /* no transitions */
  DCM_HSM_SIDX_INVALID, /* no parent state */
  DCM_HSM_DCM_SIDX_OUTOFSERVICE, /* init state */
  DCM_HSM_DCM_AIDX_TOP_ENTRY, /* entry function */
  DCM_HSM_AIDX_INVALID, /* no exit action */
    0  /* no transitions */

  },
  {
  /* description of state WaitForFullCom */
  /* non-deferred and non-ignored event mask */
 (1UL << DCM_HSM_DCM_EV_BL_TX_RESPONSE)
  | (1UL << DCM_HSM_DCM_EV_RETRY)
  | 0UL
,
  /* ignored event mask */
 (1UL << DCM_HSM_DCM_EV_RETURN_FROM_BL)
  | (1UL << DCM_HSM_DCM_EV_REQUEST_RECEIVED)
  | (1UL << DCM_HSM_DCM_EV_ROE)
  | (1UL << DCM_HSM_DCM_EV_RDBPI_TYPE1)
  | (1UL << DCM_HSM_DCM_EV_REQUEST_RECEIVED_RCRRP)
  | (1UL << DCM_HSM_DCM_EV_RDBPI_TYPE2)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_NEXT)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_DONE)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_CONFIRM)
  | (1UL << DCM_HSM_DCM_EV_PROC_ABORT)
  | (1UL << DCM_HSM_DCM_EV_PROC_DONE)
  | (1UL << DCM_HSM_DCM_EV_FORCE_RCRRP)
  | (1UL << DCM_HSM_DCM_EV_TIMEOUT2)
  | (1UL << DCM_HSM_DCM_EV_CANCELLATION_DONE)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_NOT_OK)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_OK)
  | (1UL << DCM_HSM_DCM_EV_CANCEL_PROCESSING)
  | (1UL << DCM_HSM_DCM_EV_PROVIDE_TX_BUF)
  | (1UL << DCM_HSM_DCM_EV_PROCESS_PAGE)
  | 0UL
,
    Dcm_HsmTrWaitForFullCom, /* transitions for this state */
  DCM_HSM_DCM_SIDX_TOP, /* parent state */
  DCM_HSM_SIDX_INVALID, /* no init state */
  DCM_HSM_AIDX_INVALID, /* no entry action */
  DCM_HSM_AIDX_INVALID, /* no exit action */
  3 /* num of transitions */

  },
  {
  /* description of state OutOfService */
0, /* no non-deferred event mask (no leaf state) */
0, /* no ignored event mask (no leaf state) */
    Dcm_HsmTrOutOfService, /* transitions for this state */
  DCM_HSM_DCM_SIDX_TOP, /* parent state */
  DCM_HSM_DCM_SIDX_OOSINIT, /* init state */
  DCM_HSM_AIDX_INVALID, /* no entry action */
  DCM_HSM_AIDX_INVALID, /* no exit action */
  8 /* num of transitions */

  },
  {
  /* description of state OosInit */
  /* non-deferred and non-ignored event mask */
 (1UL << DCM_HSM_DCM_EV_RETURN_FROM_BL)
  | (1UL << DCM_HSM_DCM_EV_REQUEST_RECEIVED)
  | (1UL << DCM_HSM_DCM_EV_ROE)
  | (1UL << DCM_HSM_DCM_EV_RDBPI_TYPE1)
  | (1UL << DCM_HSM_DCM_EV_REQUEST_RECEIVED_RCRRP)
  | (1UL << DCM_HSM_DCM_EV_RDBPI_TYPE2)
  | 0UL
,
  /* ignored event mask */
 (1UL << DCM_HSM_DCM_EV_BL_TX_RESPONSE)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_NEXT)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_DONE)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_CONFIRM)
  | (1UL << DCM_HSM_DCM_EV_PROC_ABORT)
  | (1UL << DCM_HSM_DCM_EV_RETRY)
  | (1UL << DCM_HSM_DCM_EV_PROC_DONE)
  | (1UL << DCM_HSM_DCM_EV_FORCE_RCRRP)
  | (1UL << DCM_HSM_DCM_EV_TIMEOUT2)
  | (1UL << DCM_HSM_DCM_EV_CANCELLATION_DONE)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_NOT_OK)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_OK)
  | (1UL << DCM_HSM_DCM_EV_CANCEL_PROCESSING)
  | (1UL << DCM_HSM_DCM_EV_PROVIDE_TX_BUF)
  | (1UL << DCM_HSM_DCM_EV_PROCESS_PAGE)
  | 0UL
,
    Dcm_HsmTrOosInit, /* transitions for this state */
  DCM_HSM_DCM_SIDX_OUTOFSERVICE, /* parent state */
  DCM_HSM_SIDX_INVALID, /* no init state */
  DCM_HSM_AIDX_INVALID, /* no entry action */
  DCM_HSM_AIDX_INVALID, /* no exit action */
  1 /* num of transitions */

  },
  {
  /* description of state OosNotified */
  /* non-deferred and non-ignored event mask */
 (1UL << DCM_HSM_DCM_EV_REQUEST_RECEIVED)
  | (1UL << DCM_HSM_DCM_EV_ROE)
  | (1UL << DCM_HSM_DCM_EV_RDBPI_TYPE1)
  | (1UL << DCM_HSM_DCM_EV_REQUEST_RECEIVED_RCRRP)
  | (1UL << DCM_HSM_DCM_EV_RDBPI_TYPE2)
  | 0UL
,
  /* ignored event mask */
 (1UL << DCM_HSM_DCM_EV_BL_TX_RESPONSE)
  | (1UL << DCM_HSM_DCM_EV_RETURN_FROM_BL)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_NEXT)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_DONE)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_CONFIRM)
  | (1UL << DCM_HSM_DCM_EV_PROC_ABORT)
  | (1UL << DCM_HSM_DCM_EV_RETRY)
  | (1UL << DCM_HSM_DCM_EV_PROC_DONE)
  | (1UL << DCM_HSM_DCM_EV_FORCE_RCRRP)
  | (1UL << DCM_HSM_DCM_EV_TIMEOUT2)
  | (1UL << DCM_HSM_DCM_EV_CANCELLATION_DONE)
  | (1UL << DCM_HSM_DCM_EV_CANCEL_PROCESSING)
  | (1UL << DCM_HSM_DCM_EV_PROVIDE_TX_BUF)
  | (1UL << DCM_HSM_DCM_EV_PROCESS_PAGE)
  | 0UL
,
    NULL_PTR,  /* no transitions */
  DCM_HSM_DCM_SIDX_OUTOFSERVICE, /* parent state */
  DCM_HSM_SIDX_INVALID, /* no init state */
  DCM_HSM_DCM_AIDX_OOSNOTIFIED_ENTRY, /* entry function */
  DCM_HSM_AIDX_INVALID, /* no exit action */
    0  /* no transitions */

  },
  {
  /* description of state InService */
0, /* no non-deferred event mask (no leaf state) */
0, /* no ignored event mask (no leaf state) */
    Dcm_HsmTrInService, /* transitions for this state */
  DCM_HSM_DCM_SIDX_TOP, /* parent state */
  DCM_HSM_DCM_SIDX_REQUESTPROCESSING, /* init state */
  DCM_HSM_DCM_AIDX_INSERVICE_ENTRY, /* entry function */
  DCM_HSM_AIDX_INVALID, /* no exit action */
  1 /* num of transitions */

  },
  {
  /* description of state Finishing */
0, /* no non-deferred event mask (no leaf state) */
0, /* no ignored event mask (no leaf state) */
    Dcm_HsmTrFinishing, /* transitions for this state */
  DCM_HSM_DCM_SIDX_INSERVICE, /* parent state */
  DCM_HSM_DCM_SIDX_CONFIRMINGOK, /* init state */
  DCM_HSM_AIDX_INVALID, /* no entry action */
  DCM_HSM_AIDX_INVALID, /* no exit action */
  1 /* num of transitions */

  },
  {
  /* description of state ConfirmingNOk */
  /* non-deferred and non-ignored event mask */
 (1UL << DCM_HSM_DCM_EV_RETRY)
  | (1UL << DCM_HSM_DCM_EV_PROC_DONE)
  | 0UL
,
  /* ignored event mask */
 (1UL << DCM_HSM_DCM_EV_BL_TX_RESPONSE)
  | (1UL << DCM_HSM_DCM_EV_RETURN_FROM_BL)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_NEXT)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_DONE)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_CONFIRM)
  | (1UL << DCM_HSM_DCM_EV_PROC_ABORT)
  | (1UL << DCM_HSM_DCM_EV_FORCE_RCRRP)
  | (1UL << DCM_HSM_DCM_EV_TIMEOUT2)
  | (1UL << DCM_HSM_DCM_EV_CANCELLATION_DONE)
  | (1UL << DCM_HSM_DCM_EV_CANCEL_PROCESSING)
  | (1UL << DCM_HSM_DCM_EV_PROVIDE_TX_BUF)
  | (1UL << DCM_HSM_DCM_EV_PROCESS_PAGE)
  | 0UL
,
    NULL_PTR,  /* no transitions */
  DCM_HSM_DCM_SIDX_FINISHING, /* parent state */
  DCM_HSM_SIDX_INVALID, /* no init state */
  DCM_HSM_DCM_AIDX_CONFIRMINGNOK_ENTRY, /* entry function */
  DCM_HSM_AIDX_INVALID, /* no exit action */
    0  /* no transitions */

  },
  {
  /* description of state ConfirmingOk */
  /* non-deferred and non-ignored event mask */
 (1UL << DCM_HSM_DCM_EV_RETRY)
  | (1UL << DCM_HSM_DCM_EV_PROC_DONE)
  | 0UL
,
  /* ignored event mask */
 (1UL << DCM_HSM_DCM_EV_BL_TX_RESPONSE)
  | (1UL << DCM_HSM_DCM_EV_RETURN_FROM_BL)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_NEXT)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_DONE)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_CONFIRM)
  | (1UL << DCM_HSM_DCM_EV_PROC_ABORT)
  | (1UL << DCM_HSM_DCM_EV_FORCE_RCRRP)
  | (1UL << DCM_HSM_DCM_EV_TIMEOUT2)
  | (1UL << DCM_HSM_DCM_EV_CANCELLATION_DONE)
  | (1UL << DCM_HSM_DCM_EV_CANCEL_PROCESSING)
  | (1UL << DCM_HSM_DCM_EV_PROVIDE_TX_BUF)
  | (1UL << DCM_HSM_DCM_EV_PROCESS_PAGE)
  | 0UL
,
    NULL_PTR,  /* no transitions */
  DCM_HSM_DCM_SIDX_FINISHING, /* parent state */
  DCM_HSM_SIDX_INVALID, /* no init state */
  DCM_HSM_DCM_AIDX_CONFIRMINGOK_ENTRY, /* entry function */
  DCM_HSM_AIDX_INVALID, /* no exit action */
    0  /* no transitions */

  },
  {
  /* description of state PeriodicProcessing */
0, /* no non-deferred event mask (no leaf state) */
0, /* no ignored event mask (no leaf state) */
    Dcm_HsmTrPeriodicProcessing, /* transitions for this state */
  DCM_HSM_DCM_SIDX_INSERVICE, /* parent state */
  DCM_HSM_DCM_SIDX_PERIODICRESPONSEPROCESSING, /* init state */
  DCM_HSM_AIDX_INVALID, /* no entry action */
  DCM_HSM_AIDX_INVALID, /* no exit action */
  2 /* num of transitions */

  },
  {
  /* description of state PeriodicResponseProcessing */
  /* non-deferred and non-ignored event mask */
 (1UL << DCM_HSM_DCM_EV_PROC_ABORT)
  | (1UL << DCM_HSM_DCM_EV_RETRY)
  | (1UL << DCM_HSM_DCM_EV_PROC_DONE)
  | 0UL
,
  /* ignored event mask */
 (1UL << DCM_HSM_DCM_EV_BL_TX_RESPONSE)
  | (1UL << DCM_HSM_DCM_EV_RETURN_FROM_BL)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_NEXT)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_DONE)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_CONFIRM)
  | (1UL << DCM_HSM_DCM_EV_FORCE_RCRRP)
  | (1UL << DCM_HSM_DCM_EV_TIMEOUT2)
  | (1UL << DCM_HSM_DCM_EV_CANCELLATION_DONE)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_NOT_OK)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_OK)
  | (1UL << DCM_HSM_DCM_EV_PROVIDE_TX_BUF)
  | (1UL << DCM_HSM_DCM_EV_PROCESS_PAGE)
  | 0UL
,
    NULL_PTR,  /* no transitions */
  DCM_HSM_DCM_SIDX_PERIODICPROCESSING, /* parent state */
  DCM_HSM_SIDX_INVALID, /* no init state */
  DCM_HSM_DCM_AIDX_PERIODICRESPONSEPROCESSING_ENTRY, /* entry function */
  DCM_HSM_AIDX_INVALID, /* no exit action */
    0  /* no transitions */

  },
  {
  /* description of state PeriodicProcessingUUDT */
0, /* no non-deferred event mask (no leaf state) */
0, /* no ignored event mask (no leaf state) */
    Dcm_HsmTrPeriodicProcessingUUDT, /* transitions for this state */
  DCM_HSM_DCM_SIDX_INSERVICE, /* parent state */
  DCM_HSM_DCM_SIDX_PERIODICSENDINGUUDT, /* init state */
  DCM_HSM_AIDX_INVALID, /* no entry action */
  DCM_HSM_AIDX_INVALID, /* no exit action */
  1 /* num of transitions */

  },
  {
  /* description of state PeriodicSendingUUDT */
  /* non-deferred and non-ignored event mask */
 (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_NEXT)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_DONE)
  | (1UL << DCM_HSM_DCM_EV_PROC_ABORT)
  | (1UL << DCM_HSM_DCM_EV_RETRY)
  | 0UL
,
  /* ignored event mask */
 (1UL << DCM_HSM_DCM_EV_BL_TX_RESPONSE)
  | (1UL << DCM_HSM_DCM_EV_RETURN_FROM_BL)
  | (1UL << DCM_HSM_DCM_EV_PROC_DONE)
  | (1UL << DCM_HSM_DCM_EV_FORCE_RCRRP)
  | (1UL << DCM_HSM_DCM_EV_TIMEOUT2)
  | (1UL << DCM_HSM_DCM_EV_CANCELLATION_DONE)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_NOT_OK)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_OK)
  | (1UL << DCM_HSM_DCM_EV_CANCEL_PROCESSING)
  | (1UL << DCM_HSM_DCM_EV_PROVIDE_TX_BUF)
  | (1UL << DCM_HSM_DCM_EV_PROCESS_PAGE)
  | 0UL
,
    Dcm_HsmTrPeriodicSendingUUDT, /* transitions for this state */
  DCM_HSM_DCM_SIDX_PERIODICPROCESSINGUUDT, /* parent state */
  DCM_HSM_SIDX_INVALID, /* no init state */
  DCM_HSM_DCM_AIDX_PERIODICSENDINGUUDT_ENTRY, /* entry function */
  DCM_HSM_AIDX_INVALID, /* no exit action */
  2 /* num of transitions */

  },
  {
  /* description of state PeriodicSendingUUDTFinishing */
  /* non-deferred and non-ignored event mask */
 (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_CONFIRM)
  | (1UL << DCM_HSM_DCM_EV_PROC_ABORT)
  | (1UL << DCM_HSM_DCM_EV_RETRY)
  | (1UL << DCM_HSM_DCM_EV_PROC_DONE)
  | 0UL
,
  /* ignored event mask */
 (1UL << DCM_HSM_DCM_EV_BL_TX_RESPONSE)
  | (1UL << DCM_HSM_DCM_EV_RETURN_FROM_BL)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_NEXT)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_DONE)
  | (1UL << DCM_HSM_DCM_EV_FORCE_RCRRP)
  | (1UL << DCM_HSM_DCM_EV_TIMEOUT2)
  | (1UL << DCM_HSM_DCM_EV_CANCELLATION_DONE)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_NOT_OK)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_OK)
  | (1UL << DCM_HSM_DCM_EV_CANCEL_PROCESSING)
  | (1UL << DCM_HSM_DCM_EV_PROVIDE_TX_BUF)
  | (1UL << DCM_HSM_DCM_EV_PROCESS_PAGE)
  | 0UL
,
    Dcm_HsmTrPeriodicSendingUUDTFinishing, /* transitions for this state */
  DCM_HSM_DCM_SIDX_PERIODICPROCESSINGUUDT, /* parent state */
  DCM_HSM_SIDX_INVALID, /* no init state */
  DCM_HSM_AIDX_INVALID, /* no entry action */
  DCM_HSM_AIDX_INVALID, /* no exit action */
  2 /* num of transitions */

  },
  {
  /* description of state ROEProcessing */
0, /* no non-deferred event mask (no leaf state) */
0, /* no ignored event mask (no leaf state) */
    Dcm_HsmTrROEProcessing, /* transitions for this state */
  DCM_HSM_DCM_SIDX_INSERVICE, /* parent state */
  DCM_HSM_DCM_SIDX_ROERESPONSEPROCESSING, /* init state */
  DCM_HSM_AIDX_INVALID, /* no entry action */
  DCM_HSM_AIDX_INVALID, /* no exit action */
  4 /* num of transitions */

  },
  {
  /* description of state ROEResponseProcessing */
  /* non-deferred and non-ignored event mask */
 (1UL << DCM_HSM_DCM_EV_PROC_ABORT)
  | (1UL << DCM_HSM_DCM_EV_RETRY)
  | (1UL << DCM_HSM_DCM_EV_PROC_DONE)
  | (1UL << DCM_HSM_DCM_EV_CANCELLATION_DONE)
  | (1UL << DCM_HSM_DCM_EV_CANCEL_PROCESSING)
  | (1UL << DCM_HSM_DCM_EV_PROCESS_PAGE)
  | 0UL
,
  /* ignored event mask */
 (1UL << DCM_HSM_DCM_EV_BL_TX_RESPONSE)
  | (1UL << DCM_HSM_DCM_EV_RETURN_FROM_BL)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_NEXT)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_DONE)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_CONFIRM)
  | (1UL << DCM_HSM_DCM_EV_FORCE_RCRRP)
  | (1UL << DCM_HSM_DCM_EV_TIMEOUT2)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_NOT_OK)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_OK)
  | (1UL << DCM_HSM_DCM_EV_PROVIDE_TX_BUF)
  | 0UL
,
    Dcm_HsmTrROEResponseProcessing, /* transitions for this state */
  DCM_HSM_DCM_SIDX_ROEPROCESSING, /* parent state */
  DCM_HSM_SIDX_INVALID, /* no init state */
  DCM_HSM_DCM_AIDX_ROERESPONSEPROCESSING_ENTRY, /* entry function */
  DCM_HSM_AIDX_INVALID, /* no exit action */
  1 /* num of transitions */

  },
  {
  /* description of state WaitForRoeProcessingCancellation */
  /* non-deferred and non-ignored event mask */
 (1UL << DCM_HSM_DCM_EV_PROC_ABORT)
  | (1UL << DCM_HSM_DCM_EV_CANCELLATION_DONE)
  | (1UL << DCM_HSM_DCM_EV_PROCESS_PAGE)
  | 0UL
,
  /* ignored event mask */
 (1UL << DCM_HSM_DCM_EV_BL_TX_RESPONSE)
  | (1UL << DCM_HSM_DCM_EV_RETURN_FROM_BL)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_NEXT)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_DONE)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_CONFIRM)
  | (1UL << DCM_HSM_DCM_EV_RETRY)
  | (1UL << DCM_HSM_DCM_EV_PROC_DONE)
  | (1UL << DCM_HSM_DCM_EV_FORCE_RCRRP)
  | (1UL << DCM_HSM_DCM_EV_TIMEOUT2)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_NOT_OK)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_OK)
  | (1UL << DCM_HSM_DCM_EV_CANCEL_PROCESSING)
  | (1UL << DCM_HSM_DCM_EV_PROVIDE_TX_BUF)
  | 0UL
,
    NULL_PTR,  /* no transitions */
  DCM_HSM_DCM_SIDX_ROEPROCESSING, /* parent state */
  DCM_HSM_SIDX_INVALID, /* no init state */
  DCM_HSM_DCM_AIDX_WAITFORROEPROCESSINGCANCELLATION_ENTRY, /* entry function */
  DCM_HSM_AIDX_INVALID, /* no exit action */
    0  /* no transitions */

  },
  {
  /* description of state RequestProcessing */
0, /* no non-deferred event mask (no leaf state) */
0, /* no ignored event mask (no leaf state) */
    Dcm_HsmTrRequestProcessing, /* transitions for this state */
  DCM_HSM_DCM_SIDX_INSERVICE, /* parent state */
  DCM_HSM_DCM_SIDX_WAITFORAPPL, /* init state */
  DCM_HSM_DCM_AIDX_REQUESTPROCESSING_ENTRY, /* entry function */
  DCM_HSM_AIDX_INVALID, /* no exit action */
  4 /* num of transitions */

  },
  {
  /* description of state RequestCancelling */
0, /* no non-deferred event mask (no leaf state) */
0, /* no ignored event mask (no leaf state) */
    NULL_PTR,  /* no transitions */
  DCM_HSM_DCM_SIDX_REQUESTPROCESSING, /* parent state */
  DCM_HSM_DCM_SIDX_CANCELWITHGENERALREJECT, /* init state */
  DCM_HSM_DCM_AIDX_REQUESTCANCELLING_ENTRY, /* entry function */
  DCM_HSM_AIDX_INVALID, /* no exit action */
    0  /* no transitions */

  },
  {
  /* description of state CancelNoGeneralReject */
0, /* no non-deferred event mask (no leaf state) */
0, /* no ignored event mask (no leaf state) */
    NULL_PTR,  /* no transitions */
  DCM_HSM_DCM_SIDX_REQUESTCANCELLING, /* parent state */
  DCM_HSM_DCM_SIDX_WAITFORPROTOCOL, /* init state */
  DCM_HSM_AIDX_INVALID, /* no entry action */
  DCM_HSM_AIDX_INVALID, /* no exit action */
    0  /* no transitions */

  },
  {
  /* description of state WaitForProtocol */
  /* non-deferred and non-ignored event mask */
 (1UL << DCM_HSM_DCM_EV_CANCELLATION_DONE)
  | 0UL
,
  /* ignored event mask */
 (1UL << DCM_HSM_DCM_EV_BL_TX_RESPONSE)
  | (1UL << DCM_HSM_DCM_EV_RETURN_FROM_BL)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_NEXT)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_DONE)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_CONFIRM)
  | (1UL << DCM_HSM_DCM_EV_PROC_ABORT)
  | (1UL << DCM_HSM_DCM_EV_RETRY)
  | (1UL << DCM_HSM_DCM_EV_PROC_DONE)
  | (1UL << DCM_HSM_DCM_EV_FORCE_RCRRP)
  | (1UL << DCM_HSM_DCM_EV_TIMEOUT2)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_NOT_OK)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_OK)
  | (1UL << DCM_HSM_DCM_EV_CANCEL_PROCESSING)
  | (1UL << DCM_HSM_DCM_EV_PROVIDE_TX_BUF)
  | (1UL << DCM_HSM_DCM_EV_PROCESS_PAGE)
  | 0UL
,
    NULL_PTR,  /* no transitions */
  DCM_HSM_DCM_SIDX_CANCELNOGENERALREJECT, /* parent state */
  DCM_HSM_SIDX_INVALID, /* no init state */
  DCM_HSM_DCM_AIDX_WAITFORPROTOCOL_ENTRY, /* entry function */
  DCM_HSM_AIDX_INVALID, /* no exit action */
    0  /* no transitions */

  },
  {
  /* description of state WaitForTxAndProtocol */
  /* non-deferred and non-ignored event mask */
 (1UL << DCM_HSM_DCM_EV_CANCELLATION_DONE)
  | 0UL
,
  /* ignored event mask */
 (1UL << DCM_HSM_DCM_EV_BL_TX_RESPONSE)
  | (1UL << DCM_HSM_DCM_EV_RETURN_FROM_BL)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_NEXT)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_DONE)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_CONFIRM)
  | (1UL << DCM_HSM_DCM_EV_PROC_ABORT)
  | (1UL << DCM_HSM_DCM_EV_RETRY)
  | (1UL << DCM_HSM_DCM_EV_PROC_DONE)
  | (1UL << DCM_HSM_DCM_EV_FORCE_RCRRP)
  | (1UL << DCM_HSM_DCM_EV_TIMEOUT2)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_NOT_OK)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_OK)
  | (1UL << DCM_HSM_DCM_EV_CANCEL_PROCESSING)
  | (1UL << DCM_HSM_DCM_EV_PROVIDE_TX_BUF)
  | (1UL << DCM_HSM_DCM_EV_PROCESS_PAGE)
  | 0UL
,
    NULL_PTR,  /* no transitions */
  DCM_HSM_DCM_SIDX_CANCELNOGENERALREJECT, /* parent state */
  DCM_HSM_SIDX_INVALID, /* no init state */
  DCM_HSM_DCM_AIDX_WAITFORTXANDPROTOCOL_ENTRY, /* entry function */
  DCM_HSM_AIDX_INVALID, /* no exit action */
    0  /* no transitions */

  },
  {
  /* description of state CancelWithGeneralReject */
  /* non-deferred and non-ignored event mask */
 (1UL << DCM_HSM_DCM_EV_CANCELLATION_DONE)
  | (1UL << DCM_HSM_DCM_EV_CANCEL_PROCESSING)
  | 0UL
,
  /* ignored event mask */
 (1UL << DCM_HSM_DCM_EV_BL_TX_RESPONSE)
  | (1UL << DCM_HSM_DCM_EV_RETURN_FROM_BL)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_NEXT)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_DONE)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_CONFIRM)
  | (1UL << DCM_HSM_DCM_EV_PROC_ABORT)
  | (1UL << DCM_HSM_DCM_EV_RETRY)
  | (1UL << DCM_HSM_DCM_EV_PROC_DONE)
  | (1UL << DCM_HSM_DCM_EV_FORCE_RCRRP)
  | (1UL << DCM_HSM_DCM_EV_TIMEOUT2)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_NOT_OK)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_OK)
  | (1UL << DCM_HSM_DCM_EV_PROVIDE_TX_BUF)
  | (1UL << DCM_HSM_DCM_EV_PROCESS_PAGE)
  | 0UL
,
    Dcm_HsmTrCancelWithGeneralReject, /* transitions for this state */
  DCM_HSM_DCM_SIDX_REQUESTCANCELLING, /* parent state */
  DCM_HSM_SIDX_INVALID, /* no init state */
  DCM_HSM_DCM_AIDX_CANCELWITHGENERALREJECT_ENTRY, /* entry function */
  DCM_HSM_AIDX_INVALID, /* no exit action */
  1 /* num of transitions */

  },
  {
  /* description of state SendRp */
0, /* no non-deferred event mask (no leaf state) */
0, /* no ignored event mask (no leaf state) */
    Dcm_HsmTrSendRp, /* transitions for this state */
  DCM_HSM_DCM_SIDX_REQUESTPROCESSING, /* parent state */
  DCM_HSM_DCM_SIDX_SENDRPNORMAL, /* init state */
  DCM_HSM_DCM_AIDX_SENDRP_ENTRY, /* entry function */
  DCM_HSM_DCM_AIDX_SENDRP_EXIT, /* exit function */
  2 /* num of transitions */

  },
  {
  /* description of state SendRpForced */
  /* non-deferred and non-ignored event mask */
 (1UL << DCM_HSM_DCM_EV_PROC_ABORT)
  | (1UL << DCM_HSM_DCM_EV_RETRY)
  | (1UL << DCM_HSM_DCM_EV_CANCELLATION_DONE)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_NOT_OK)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_OK)
  | (1UL << DCM_HSM_DCM_EV_CANCEL_PROCESSING)
  | 0UL
,
  /* ignored event mask */
 (1UL << DCM_HSM_DCM_EV_BL_TX_RESPONSE)
  | (1UL << DCM_HSM_DCM_EV_RETURN_FROM_BL)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_NEXT)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_DONE)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_CONFIRM)
  | (1UL << DCM_HSM_DCM_EV_FORCE_RCRRP)
  | (1UL << DCM_HSM_DCM_EV_PROVIDE_TX_BUF)
  | 0UL
,
    Dcm_HsmTrSendRpForced, /* transitions for this state */
  DCM_HSM_DCM_SIDX_SENDRP, /* parent state */
  DCM_HSM_SIDX_INVALID, /* no init state */
  DCM_HSM_AIDX_INVALID, /* no entry action */
  DCM_HSM_AIDX_INVALID, /* no exit action */
  1 /* num of transitions */

  },
  {
  /* description of state SendRpNormal */
  /* non-deferred and non-ignored event mask */
 (1UL << DCM_HSM_DCM_EV_PROC_ABORT)
  | (1UL << DCM_HSM_DCM_EV_RETRY)
  | (1UL << DCM_HSM_DCM_EV_CANCELLATION_DONE)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_NOT_OK)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_OK)
  | (1UL << DCM_HSM_DCM_EV_CANCEL_PROCESSING)
  | 0UL
,
  /* ignored event mask */
 (1UL << DCM_HSM_DCM_EV_BL_TX_RESPONSE)
  | (1UL << DCM_HSM_DCM_EV_RETURN_FROM_BL)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_NEXT)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_DONE)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_CONFIRM)
  | (1UL << DCM_HSM_DCM_EV_FORCE_RCRRP)
  | (1UL << DCM_HSM_DCM_EV_PROVIDE_TX_BUF)
  | 0UL
,
    Dcm_HsmTrSendRpNormal, /* transitions for this state */
  DCM_HSM_DCM_SIDX_SENDRP, /* parent state */
  DCM_HSM_SIDX_INVALID, /* no init state */
  DCM_HSM_AIDX_INVALID, /* no entry action */
  DCM_HSM_AIDX_INVALID, /* no exit action */
  1 /* num of transitions */

  },
  {
  /* description of state WaitForAppl */
  /* non-deferred and non-ignored event mask */
 (1UL << DCM_HSM_DCM_EV_PROC_ABORT)
  | (1UL << DCM_HSM_DCM_EV_RETRY)
  | (1UL << DCM_HSM_DCM_EV_PROC_DONE)
  | (1UL << DCM_HSM_DCM_EV_FORCE_RCRRP)
  | (1UL << DCM_HSM_DCM_EV_TIMEOUT2)
  | (1UL << DCM_HSM_DCM_EV_CANCELLATION_DONE)
  | (1UL << DCM_HSM_DCM_EV_CANCEL_PROCESSING)
  | (1UL << DCM_HSM_DCM_EV_PROCESS_PAGE)
  | 0UL
,
  /* ignored event mask */
 (1UL << DCM_HSM_DCM_EV_BL_TX_RESPONSE)
  | (1UL << DCM_HSM_DCM_EV_RETURN_FROM_BL)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_NEXT)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_DONE)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_CONFIRM)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_NOT_OK)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_OK)
  | (1UL << DCM_HSM_DCM_EV_PROVIDE_TX_BUF)
  | 0UL
,
    Dcm_HsmTrWaitForAppl, /* transitions for this state */
  DCM_HSM_DCM_SIDX_REQUESTPROCESSING, /* parent state */
  DCM_HSM_SIDX_INVALID, /* no init state */
  DCM_HSM_AIDX_INVALID, /* no entry action */
  DCM_HSM_AIDX_INVALID, /* no exit action */
  5 /* num of transitions */

  },
  {
  /* description of state Sending */
0, /* no non-deferred event mask (no leaf state) */
0, /* no ignored event mask (no leaf state) */
    Dcm_HsmTrSending, /* transitions for this state */
  DCM_HSM_DCM_SIDX_INSERVICE, /* parent state */
  DCM_HSM_DCM_SIDX_NORMALSENDING, /* init state */
  DCM_HSM_DCM_AIDX_SENDING_ENTRY, /* entry function */
  DCM_HSM_AIDX_INVALID, /* no exit action */
  3 /* num of transitions */

  },
  {
  /* description of state NormalSending */
0, /* no non-deferred event mask (no leaf state) */
0, /* no ignored event mask (no leaf state) */
    NULL_PTR,  /* no transitions */
  DCM_HSM_DCM_SIDX_SENDING, /* parent state */
  DCM_HSM_DCM_SIDX_NORMALSENDINGPROCESSING, /* init state */
  DCM_HSM_AIDX_INVALID, /* no entry action */
  DCM_HSM_AIDX_INVALID, /* no exit action */
    0  /* no transitions */

  },
  {
  /* description of state NormalSendingCancelling */
  /* non-deferred and non-ignored event mask */
 (1UL << DCM_HSM_DCM_EV_RETRY)
  | (1UL << DCM_HSM_DCM_EV_CANCELLATION_DONE)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_NOT_OK)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_OK)
  | 0UL
,
  /* ignored event mask */
 (1UL << DCM_HSM_DCM_EV_BL_TX_RESPONSE)
  | (1UL << DCM_HSM_DCM_EV_RETURN_FROM_BL)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_NEXT)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_DONE)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_CONFIRM)
  | (1UL << DCM_HSM_DCM_EV_PROC_ABORT)
  | (1UL << DCM_HSM_DCM_EV_PROC_DONE)
  | (1UL << DCM_HSM_DCM_EV_FORCE_RCRRP)
  | (1UL << DCM_HSM_DCM_EV_TIMEOUT2)
  | (1UL << DCM_HSM_DCM_EV_CANCEL_PROCESSING)
  | (1UL << DCM_HSM_DCM_EV_PROVIDE_TX_BUF)
  | (1UL << DCM_HSM_DCM_EV_PROCESS_PAGE)
  | 0UL
,
    NULL_PTR,  /* no transitions */
  DCM_HSM_DCM_SIDX_NORMALSENDING, /* parent state */
  DCM_HSM_SIDX_INVALID, /* no init state */
  DCM_HSM_DCM_AIDX_NORMALSENDINGCANCELLING_ENTRY, /* entry function */
  DCM_HSM_AIDX_INVALID, /* no exit action */
    0  /* no transitions */

  },
  {
  /* description of state NormalSendingProcessing */
  /* non-deferred and non-ignored event mask */
 (1UL << DCM_HSM_DCM_EV_RETRY)
  | (1UL << DCM_HSM_DCM_EV_CANCELLATION_DONE)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_NOT_OK)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_OK)
  | (1UL << DCM_HSM_DCM_EV_CANCEL_PROCESSING)
  | 0UL
,
  /* ignored event mask */
 (1UL << DCM_HSM_DCM_EV_BL_TX_RESPONSE)
  | (1UL << DCM_HSM_DCM_EV_RETURN_FROM_BL)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_NEXT)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_DONE)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_CONFIRM)
  | (1UL << DCM_HSM_DCM_EV_PROC_ABORT)
  | (1UL << DCM_HSM_DCM_EV_PROC_DONE)
  | (1UL << DCM_HSM_DCM_EV_FORCE_RCRRP)
  | (1UL << DCM_HSM_DCM_EV_TIMEOUT2)
  | (1UL << DCM_HSM_DCM_EV_PROVIDE_TX_BUF)
  | (1UL << DCM_HSM_DCM_EV_PROCESS_PAGE)
  | 0UL
,
    Dcm_HsmTrNormalSendingProcessing, /* transitions for this state */
  DCM_HSM_DCM_SIDX_NORMALSENDING, /* parent state */
  DCM_HSM_SIDX_INVALID, /* no init state */
  DCM_HSM_AIDX_INVALID, /* no entry action */
  DCM_HSM_AIDX_INVALID, /* no exit action */
  1 /* num of transitions */

  },
  {
  /* description of state PgBufSending */
0, /* no non-deferred event mask (no leaf state) */
0, /* no ignored event mask (no leaf state) */
    NULL_PTR,  /* no transitions */
  DCM_HSM_DCM_SIDX_SENDING, /* parent state */
  DCM_HSM_DCM_SIDX_PGBUFSENDINGPROCESSING, /* init state */
  DCM_HSM_AIDX_INVALID, /* no entry action */
  DCM_HSM_AIDX_INVALID, /* no exit action */
    0  /* no transitions */

  },
  {
  /* description of state PgBufSendingCancelling */
  /* non-deferred and non-ignored event mask */
 (1UL << DCM_HSM_DCM_EV_RETRY)
  | (1UL << DCM_HSM_DCM_EV_CANCELLATION_DONE)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_NOT_OK)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_OK)
  | 0UL
,
  /* ignored event mask */
 (1UL << DCM_HSM_DCM_EV_BL_TX_RESPONSE)
  | (1UL << DCM_HSM_DCM_EV_RETURN_FROM_BL)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_NEXT)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_DONE)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_CONFIRM)
  | (1UL << DCM_HSM_DCM_EV_PROC_ABORT)
  | (1UL << DCM_HSM_DCM_EV_PROC_DONE)
  | (1UL << DCM_HSM_DCM_EV_FORCE_RCRRP)
  | (1UL << DCM_HSM_DCM_EV_TIMEOUT2)
  | (1UL << DCM_HSM_DCM_EV_CANCEL_PROCESSING)
  | (1UL << DCM_HSM_DCM_EV_PROVIDE_TX_BUF)
  | (1UL << DCM_HSM_DCM_EV_PROCESS_PAGE)
  | 0UL
,
    NULL_PTR,  /* no transitions */
  DCM_HSM_DCM_SIDX_PGBUFSENDING, /* parent state */
  DCM_HSM_SIDX_INVALID, /* no init state */
  DCM_HSM_DCM_AIDX_PGBUFSENDINGCANCELLING_ENTRY, /* entry function */
  DCM_HSM_AIDX_INVALID, /* no exit action */
    0  /* no transitions */

  },
  {
  /* description of state PgBufSendingProcessing */
0, /* no non-deferred event mask (no leaf state) */
0, /* no ignored event mask (no leaf state) */
    Dcm_HsmTrPgBufSendingProcessing, /* transitions for this state */
  DCM_HSM_DCM_SIDX_PGBUFSENDING, /* parent state */
  DCM_HSM_DCM_SIDX_PGBUFDATAAVAILABLE, /* init state */
  DCM_HSM_AIDX_INVALID, /* no entry action */
  DCM_HSM_AIDX_INVALID, /* no exit action */
  1 /* num of transitions */

  },
  {
  /* description of state PgBufDataAvailable */
  /* non-deferred and non-ignored event mask */
 (1UL << DCM_HSM_DCM_EV_RETRY)
  | (1UL << DCM_HSM_DCM_EV_CANCELLATION_DONE)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_NOT_OK)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_OK)
  | (1UL << DCM_HSM_DCM_EV_CANCEL_PROCESSING)
  | (1UL << DCM_HSM_DCM_EV_PROVIDE_TX_BUF)
  | 0UL
,
  /* ignored event mask */
 (1UL << DCM_HSM_DCM_EV_BL_TX_RESPONSE)
  | (1UL << DCM_HSM_DCM_EV_RETURN_FROM_BL)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_NEXT)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_DONE)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_CONFIRM)
  | (1UL << DCM_HSM_DCM_EV_PROC_ABORT)
  | (1UL << DCM_HSM_DCM_EV_PROC_DONE)
  | (1UL << DCM_HSM_DCM_EV_FORCE_RCRRP)
  | (1UL << DCM_HSM_DCM_EV_TIMEOUT2)
  | (1UL << DCM_HSM_DCM_EV_PROCESS_PAGE)
  | 0UL
,
    Dcm_HsmTrPgBufDataAvailable, /* transitions for this state */
  DCM_HSM_DCM_SIDX_PGBUFSENDINGPROCESSING, /* parent state */
  DCM_HSM_SIDX_INVALID, /* no init state */
  DCM_HSM_AIDX_INVALID, /* no entry action */
  DCM_HSM_AIDX_INVALID, /* no exit action */
  1 /* num of transitions */

  },
  {
  /* description of state PgBufDataRequested */
  /* non-deferred and non-ignored event mask */
 (1UL << DCM_HSM_DCM_EV_RETRY)
  | (1UL << DCM_HSM_DCM_EV_CANCELLATION_DONE)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_NOT_OK)
  | (1UL << DCM_HSM_DCM_EV_TX_CONF_OK)
  | (1UL << DCM_HSM_DCM_EV_CANCEL_PROCESSING)
  | (1UL << DCM_HSM_DCM_EV_PROCESS_PAGE)
  | 0UL
,
  /* ignored event mask */
 (1UL << DCM_HSM_DCM_EV_BL_TX_RESPONSE)
  | (1UL << DCM_HSM_DCM_EV_RETURN_FROM_BL)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_NEXT)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_DONE)
  | (1UL << DCM_HSM_DCM_EV_PERIODIC_UUDT_CONFIRM)
  | (1UL << DCM_HSM_DCM_EV_PROC_ABORT)
  | (1UL << DCM_HSM_DCM_EV_PROC_DONE)
  | (1UL << DCM_HSM_DCM_EV_FORCE_RCRRP)
  | (1UL << DCM_HSM_DCM_EV_TIMEOUT2)
  | (1UL << DCM_HSM_DCM_EV_PROVIDE_TX_BUF)
  | 0UL
,
    Dcm_HsmTrPgBufDataRequested, /* transitions for this state */
  DCM_HSM_DCM_SIDX_PGBUFSENDINGPROCESSING, /* parent state */
  DCM_HSM_SIDX_INVALID, /* no init state */
  DCM_HSM_AIDX_INVALID, /* no entry action */
  DCM_HSM_AIDX_INVALID, /* no exit action */
  1 /* num of transitions */

  }
};

#if (DCM_HSM_TRACE == STD_ON)
/* Deviation MISRA-1 */
STATIC CONSTP2CONST(char, DCM_CONST, DCM_CONST)
  Dcm_HsmDcmStateNames[] =
{
  "Top",
  "WaitForFullCom",
  "OutOfService",
  "OosInit",
  "OosNotified",
  "InService",
  "Finishing",
  "ConfirmingNOk",
  "ConfirmingOk",
  "PeriodicProcessing",
  "PeriodicResponseProcessing",
  "PeriodicProcessingUUDT",
  "PeriodicSendingUUDT",
  "PeriodicSendingUUDTFinishing",
  "ROEProcessing",
  "ROEResponseProcessing",
  "WaitForRoeProcessingCancellation",
  "RequestProcessing",
  "RequestCancelling",
  "CancelNoGeneralReject",
  "WaitForProtocol",
  "WaitForTxAndProtocol",
  "CancelWithGeneralReject",
  "SendRp",
  "SendRpForced",
  "SendRpNormal",
  "WaitForAppl",
  "Sending",
  "NormalSending",
  "NormalSendingCancelling",
  "NormalSendingProcessing",
  "PgBufSending",
  "PgBufSendingCancelling",
  "PgBufSendingProcessing",
  "PgBufDataAvailable",
  "PgBufDataRequested",
};

/* Deviation MISRA-1 */
STATIC CONSTP2CONST(char, DCM_CONST, DCM_CONST)
  Dcm_HsmDcmActionNames[] =
{
  "Top entry",
  "WaitForFullCom action 1",
  "WaitForFullCom action 2",
  "WaitForFullCom action 3",
  "OutOfService action 2",
  "OutOfService action 4",
  "OutOfService action 7",
  "OutOfService action 8",
  "OosInit action 1",
  "OosNotified entry",
  "InService entry",
  "InService action 1",
  "ConfirmingNOk entry",
  "ConfirmingOk entry",
  "PeriodicProcessing action 1",
  "PeriodicResponseProcessing entry",
  "PeriodicProcessingUUDT action 1",
  "PeriodicSendingUUDT entry",
  "PeriodicSendingUUDT action 1",
  "PeriodicSendingUUDTFinishing action 1",
  "PeriodicSendingUUDTFinishing action 2",
  "ROEProcessing action 3",
  "ROEResponseProcessing entry",
  "WaitForRoeProcessingCancellation entry",
  "RequestProcessing entry",
  "RequestProcessing action 3",
  "RequestCancelling entry",
  "WaitForProtocol entry",
  "WaitForTxAndProtocol entry",
  "CancelWithGeneralReject entry",
  "CancelWithGeneralReject action 1",
  "SendRp entry",
  "SendRp exit",
  "SendRpForced action 1",
  "Sending entry",
  "NormalSendingCancelling entry",
  "PgBufSendingCancelling entry",
  "PgBufDataAvailable action 1",
  "PgBufDataRequested action 1",
};

/* Deviation MISRA-1 */
STATIC CONSTP2CONST(char, DCM_CONST, DCM_CONST)
  Dcm_HsmDcmGuardNames[] =
{
  "WaitForFullCom guard 2",
  "WaitForFullCom guard 3",
  "OutOfService guard 1",
  "OutOfService guard 2",
  "OutOfService guard 3",
  "OutOfService guard 4",
  "OutOfService guard 7",
  "OutOfService guard 8",
  "WaitForAppl guard 2",
  "WaitForAppl guard 3",
  "WaitForAppl guard 4",
  "WaitForAppl guard 5",
};

/* Deviation MISRA-1 */
STATIC CONSTP2CONST(char, DCM_CONST, DCM_CONST)
  Dcm_HsmDcmEventNames[] =
{
  "BL_TX_RESPONSE",
  "RETURN_FROM_BL",
  "REQUEST_RECEIVED",
  "ROE",
  "RDBPI_TYPE1",
  "REQUEST_RECEIVED_RCRRP",
  "RDBPI_TYPE2",
  "PERIODIC_UUDT_NEXT",
  "PERIODIC_UUDT_DONE",
  "PERIODIC_UUDT_CONFIRM",
  "PROC_ABORT",
  "RETRY",
  "PROC_DONE",
  "FORCE_RCRRP",
  "TIMEOUT2",
  "CANCELLATION_DONE",
  "TX_CONF_NOT_OK",
  "TX_CONF_OK",
  "CANCEL_PROCESSING",
  "PROVIDE_TX_BUF",
  "PROCESS_PAGE",
};
#endif

/*==================[external constants]====================================*/

/* Top level state chart description data */
CONST(Dcm_HsmStatechartType, DCM_CONST)
  Dcm_HsmScDcm =
{
  Dcm_HsmScdDcm, /* state description array */
  Dcm_HsmDcmActions, /* actions */
  Dcm_HsmDcmGuards, /* guards */
  Dcm_HsmInstDcm, /* instances RAM */
  Dcm_HsmEvQueuesDcm, /* event queues RAM */
#if (DCM_HSM_TRACE == STD_ON)
  "Dcm", /* name of the HSM */
  Dcm_HsmDcmStateNames,
  Dcm_HsmDcmActionNames,
  Dcm_HsmDcmGuardNames,
  Dcm_HsmDcmEventNames,
#endif
  DCM_HSM_DCM_NUM_INST, /* num of instances */
  DCM_HSM_DCM_EV_QUEUE_SIZE, /* event queue size */
  DCM_HSM_DCM_SIDX_TOP, /* top state index */
  DCM_HSM_DCM_NUM_EVENTS /* num of events */

};


#define DCM_STOP_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

/*==================[internal function definitions]=========================*/

/*==================[external function definitions]=========================*/

/*==================[end of file]===========================================*/
