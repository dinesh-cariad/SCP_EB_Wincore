/**
 * \file
 *
 * \brief AUTOSAR ComM
 *
 * This file contains the implementation of the AUTOSAR
 * module ComM.
 *
 * \version 5.19.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* Definition of the configuration data of the state machine
 * ComM.
 *
 * This file contains the definition of all used data structures of the state
 * machine model.  It is generated and must not be edited.
 */

/*==================[inclusions]============================================*/

#include <ComM_Hsm.h> /* state machine driver interface */
#include <ComM_HsmComM.h> /* public statechart model definitions, */
#include <ComM_HsmComMInt.h> /* internal statechart model definitions */

/*==================[macros]================================================*/

/** \brief Number of user events */
#if (defined COMM_HSM_COMM_NUM_EVENTS)
#error COMM_HSM_COMM_NUM_EVENTS already defined
#endif
#define COMM_HSM_COMM_NUM_EVENTS 10U

/* Check if number of hsm instances is defined in the user's configuration */
#ifndef COMM_HSM_COMM_NUM_INST
#error COMM_HSM_COMM_NUM_INST must be defined in ComM_HsmCfg.h
#endif


/*==================[type definitions]======================================*/

/*==================[internal function declarations]========================*/

/*==================[external function declarations]========================*/

/*==================[internal data]=========================================*/

#define COMM_START_SEC_VAR_CLEARED_8
#include <ComM_MemMap.h>

/* RAM data for event counters */
STATIC VAR(uint8, COMM_VAR_CLEARED)
  ComM_HsmEvCountersComM[
    COMM_HSM_COMM_NUM_INST * COMM_HSM_COMM_NUM_EVENTS];

/* RAM data for event queues */
STATIC VAR(ComM_HsmEventType, COMM_VAR_CLEARED)
  ComM_HsmEvQueuesComM[
    COMM_HSM_COMM_NUM_INST * COMM_HSM_COMM_NUM_EVENTS];

#define COMM_STOP_SEC_VAR_CLEARED_8
#include <ComM_MemMap.h>


#define COMM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <ComM_MemMap.h>

/* RAM data for instances */
STATIC VAR(ComM_HsmInstType, COMM_VAR_CLEARED)
  ComM_HsmInstComM[COMM_HSM_COMM_NUM_INST];

#define COMM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <ComM_MemMap.h>

/*==================[external data]=========================================*/

/*==================[internal constants]====================================*/

#define COMM_START_SEC_CONST_UNSPECIFIED
#include <ComM_MemMap.h>

/* *** array of entry/exit/do and transition actions *** */
STATIC CONST(ComM_HsmActionType, COMM_CONST)
  ComM_HsmComMActions[COMM_HSM_COMM_NO_OF_ACTIONS] =
{
  &ComM_HsmComMSfTOPAction1,
  &ComM_HsmComMSfFULL_COMMUNICATIONEntry,
  &ComM_HsmComMSfFULL_COM_NETWORK_REQUESTEDEntry,
  &ComM_HsmComMSfFULL_COM_NETWORK_REQUESTEDAction1,
  &ComM_HsmComMSfFULL_COM_NETWORK_REQUESTED_ENTRYEntry,
  &ComM_HsmComMSfFULL_COM_NETWORK_REQUESTED_ENTRYExit,
  &ComM_HsmComMSfFULL_COM_READY_SLEEPEntry,
  &ComM_HsmComMSfFULL_COM_READY_SLEEPExit,
  &ComM_HsmComMSfFULL_COM_READY_SLEEPAction3,
  &ComM_HsmComMSfNO_COMMUNICATIONEntry,
  &ComM_HsmComMSfNO_COM_NO_PENDING_REQUESTEntry,
  &ComM_HsmComMSfNO_COM_NO_PENDING_REQUESTAction1,
  &ComM_HsmComMSfNO_COM_REQUEST_PENDINGEntry,
  &ComM_HsmComMSfNO_COM_REQUEST_PENDINGAction2,
  &ComM_HsmComMSfNO_COM_REQUEST_PENDINGAction3,
  &ComM_HsmComMSfSILENT_COMMUNICATIONEntry,
};

/* *** array of guards *** */
STATIC CONST(ComM_HsmGuardType, COMM_CONST)
  ComM_HsmComMGuards[COMM_HSM_COMM_NO_OF_GUARDS] =
{
  &ComM_HsmComMSfFULL_COMMUNICATIONGuard2,
  &ComM_HsmComMSfFULL_COM_NETWORK_REQUESTEDGuard2,
  &ComM_HsmComMSfFULL_COM_NETWORK_REQUESTEDGuard3,
  &ComM_HsmComMSfFULL_COM_NETWORK_REQUESTED_ENTRYGuard1,
  &ComM_HsmComMSfFULL_COM_NETWORK_REQUESTED_ENTRYGuard2,
  &ComM_HsmComMSfFULL_COM_NETWORK_REQUESTED_ENTRYGuard3,
  &ComM_HsmComMSfFULL_COM_NETWORK_REQUESTED_ENTRYGuard4,
  &ComM_HsmComMSfFULL_COM_NETWORK_REQUESTED_PENDINGGuard1,
  &ComM_HsmComMSfFULL_COM_READY_SLEEPGuard1,
  &ComM_HsmComMSfFULL_COM_READY_SLEEPGuard2,
  &ComM_HsmComMSfNO_COM_NO_PENDING_REQUESTGuard2,
  &ComM_HsmComMSfNO_COM_REQUEST_PENDINGGuard1,
  &ComM_HsmComMSfNO_COM_REQUEST_PENDINGGuard2,
  &ComM_HsmComMSfNO_COM_REQUEST_PENDINGGuard4,
  &ComM_HsmComMSfNO_COM_REQUEST_PENDINGGuard5,
  &ComM_HsmComMSfSILENT_COMMUNICATIONGuard1,
  &ComM_HsmComMSfSILENT_COMMUNICATIONGuard2,
  &ComM_HsmComMSfSILENT_COMMUNICATIONGuard3,
};

#define COMM_STOP_SEC_CONST_UNSPECIFIED
#include <ComM_MemMap.h>

#define COMM_START_SEC_CONST_8
#include <ComM_MemMap.h>

/* *** exit/action/entry sequences for transitions *** */
/* trans sequence for transition #1 from state TOP */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrTOPSteps1[1] =
{
  COMM_HSM_COMM_AIDX_TOP_ACTION_1,
};
/* trans sequence for transition #1 from state FULL_COMMUNICATION */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrFULL_COMMUNICATIONSteps1[1] =
{
  COMM_HSM_COMM_AIDX_NO_COMMUNICATION_ENTRY,
};
/* trans sequence for transition #2 from state FULL_COMMUNICATION */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrFULL_COMMUNICATIONSteps2[1] =
{
  COMM_HSM_COMM_AIDX_NO_COMMUNICATION_ENTRY,
};
/* trans sequence for transition #1 from state FULL_COM_NETWORK_REQUESTED */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrFULL_COM_NETWORK_REQUESTEDSteps1[1] =
{
  COMM_HSM_COMM_AIDX_FULL_COM_NETWORK_REQUESTED_ACTION_1,
};
/* trans sequence for transition #2 from state FULL_COM_NETWORK_REQUESTED */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrFULL_COM_NETWORK_REQUESTEDSteps2[1] =
{
  COMM_HSM_COMM_AIDX_FULL_COM_READY_SLEEP_ENTRY,
};
/* trans sequence for transition #3 from state FULL_COM_NETWORK_REQUESTED */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrFULL_COM_NETWORK_REQUESTEDSteps3[1] =
{
  COMM_HSM_COMM_AIDX_SILENT_COMMUNICATION_ENTRY,
};
/* trans sequence for transition #1 from state FULL_COM_NETWORK_REQUESTED_ENTRY */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrFULL_COM_NETWORK_REQUESTED_ENTRYSteps1[1] =
{
  COMM_HSM_COMM_AIDX_FULL_COM_NETWORK_REQUESTED_ENTRY_EXIT,
};
/* trans sequence for transition #2 from state FULL_COM_NETWORK_REQUESTED_ENTRY */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrFULL_COM_NETWORK_REQUESTED_ENTRYSteps2[2] =
{
  COMM_HSM_COMM_AIDX_FULL_COM_NETWORK_REQUESTED_ENTRY_EXIT,
  COMM_HSM_COMM_AIDX_FULL_COM_READY_SLEEP_ENTRY,
};
/* trans sequence for transition #3 from state FULL_COM_NETWORK_REQUESTED_ENTRY */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrFULL_COM_NETWORK_REQUESTED_ENTRYSteps3[2] =
{
  COMM_HSM_COMM_AIDX_FULL_COM_NETWORK_REQUESTED_ENTRY_EXIT,
  COMM_HSM_COMM_AIDX_FULL_COM_READY_SLEEP_ENTRY,
};
/* trans sequence for transition #4 from state FULL_COM_NETWORK_REQUESTED_ENTRY */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrFULL_COM_NETWORK_REQUESTED_ENTRYSteps4[1] =
{
  COMM_HSM_COMM_AIDX_FULL_COM_NETWORK_REQUESTED_ENTRY_EXIT,
};
/* trans sequence for transition #1 from state FULL_COM_NETWORK_REQUESTED_PENDING */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrFULL_COM_NETWORK_REQUESTED_PENDINGSteps1[1] =
{
  COMM_HSM_COMM_AIDX_FULL_COM_READY_SLEEP_ENTRY,
};
/* trans sequence for transition #1 from state FULL_COM_READY_SLEEP */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrFULL_COM_READY_SLEEPSteps1[2] =
{
  COMM_HSM_COMM_AIDX_FULL_COM_READY_SLEEP_EXIT,
  COMM_HSM_COMM_AIDX_SILENT_COMMUNICATION_ENTRY,
};
/* trans sequence for transition #2 from state FULL_COM_READY_SLEEP */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrFULL_COM_READY_SLEEPSteps2[2] =
{
  COMM_HSM_COMM_AIDX_FULL_COM_READY_SLEEP_EXIT,
  COMM_HSM_COMM_AIDX_FULL_COM_NETWORK_REQUESTED_ENTRY,
};
/* trans sequence for transition #3 from state FULL_COM_READY_SLEEP */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrFULL_COM_READY_SLEEPSteps3[3] =
{
  COMM_HSM_COMM_AIDX_FULL_COM_READY_SLEEP_EXIT,
  COMM_HSM_COMM_AIDX_FULL_COM_READY_SLEEP_ACTION_3,
  COMM_HSM_COMM_AIDX_NO_COMMUNICATION_ENTRY,
};
/* trans sequence for transition #1 from state NO_COM_NO_PENDING_REQUEST */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrNO_COM_NO_PENDING_REQUESTSteps1[2] =
{
  COMM_HSM_COMM_AIDX_NO_COM_NO_PENDING_REQUEST_ACTION_1,
  COMM_HSM_COMM_AIDX_NO_COM_REQUEST_PENDING_ENTRY,
};
/* trans sequence for transition #2 from state NO_COM_NO_PENDING_REQUEST */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrNO_COM_NO_PENDING_REQUESTSteps2[1] =
{
  COMM_HSM_COMM_AIDX_NO_COM_REQUEST_PENDING_ENTRY,
};
/* trans sequence for transition #1 from state NO_COM_REQUEST_PENDING */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrNO_COM_REQUEST_PENDINGSteps1[1] =
{
  COMM_HSM_COMM_AIDX_FULL_COMMUNICATION_ENTRY,
};
/* trans sequence for transition #2 from state NO_COM_REQUEST_PENDING */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrNO_COM_REQUEST_PENDINGSteps2[2] =
{
  COMM_HSM_COMM_AIDX_NO_COM_REQUEST_PENDING_ACTION_2,
  COMM_HSM_COMM_AIDX_NO_COM_NO_PENDING_REQUEST_ENTRY,
};
/* trans sequence for transition #3 from state NO_COM_REQUEST_PENDING */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrNO_COM_REQUEST_PENDINGSteps3[2] =
{
  COMM_HSM_COMM_AIDX_NO_COM_REQUEST_PENDING_ACTION_3,
  COMM_HSM_COMM_AIDX_NO_COM_REQUEST_PENDING_ENTRY,
};
/* trans sequence for transition #4 from state NO_COM_REQUEST_PENDING */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrNO_COM_REQUEST_PENDINGSteps4[1] =
{
  COMM_HSM_COMM_AIDX_NO_COM_NO_PENDING_REQUEST_ENTRY,
};
/* trans sequence for transition #5 from state NO_COM_REQUEST_PENDING */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrNO_COM_REQUEST_PENDINGSteps5[1] =
{
  COMM_HSM_COMM_AIDX_NO_COM_NO_PENDING_REQUEST_ENTRY,
};
/* trans sequence for transition #1 from state SILENT_COMMUNICATION */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrSILENT_COMMUNICATIONSteps1[2] =
{
  COMM_HSM_COMM_AIDX_FULL_COMMUNICATION_ENTRY,
  COMM_HSM_COMM_AIDX_FULL_COM_READY_SLEEP_ENTRY,
};
/* trans sequence for transition #2 from state SILENT_COMMUNICATION */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrSILENT_COMMUNICATIONSteps2[1] =
{
  COMM_HSM_COMM_AIDX_NO_COMMUNICATION_ENTRY,
};
/* trans sequence for transition #3 from state SILENT_COMMUNICATION */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrSILENT_COMMUNICATIONSteps3[1] =
{
  COMM_HSM_COMM_AIDX_FULL_COMMUNICATION_ENTRY,
};

#define COMM_STOP_SEC_CONST_8
#include <ComM_MemMap.h>

/* ****** */

#define COMM_START_SEC_CONST_UNSPECIFIED
#include <ComM_MemMap.h>

/* transitions from state TOP */
STATIC CONST(ComM_HsmTranType, COMM_CONST)
 ComM_HsmTrTOP[1] =
{
{
  ComM_HsmTrTOPSteps1,
  sizeof(ComM_HsmTrTOPSteps1)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMM_EV_BUSSM_MODE_IND, /* triggering event */
  COMM_HSM_GIDX_INVALID, /* no guard */
  COMM_HSM_SIDX_INVALID /* no target state, internal transition */
},
};
/* transitions from state FULL_COMMUNICATION */
STATIC CONST(ComM_HsmTranType, COMM_CONST)
 ComM_HsmTrFULL_COMMUNICATION[2] =
{
{
  ComM_HsmTrFULL_COMMUNICATIONSteps1,
  sizeof(ComM_HsmTrFULL_COMMUNICATIONSteps1)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMM_EV_BUS_SM_SLEEP_INDICATION, /* triggering event */
  COMM_HSM_GIDX_INVALID, /* no guard */
  COMM_HSM_COMM_SIDX_NO_COMMUNICATION /* target state */
},
{
  ComM_HsmTrFULL_COMMUNICATIONSteps2,
  sizeof(ComM_HsmTrFULL_COMMUNICATIONSteps2)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMM_EV_NM_MODE_SWITCHED, /* triggering event */
  COMM_HSM_COMM_GIDX_FULL_COMMUNICATION_GUARD_2, /* guard idx */
  COMM_HSM_COMM_SIDX_NO_COMMUNICATION /* target state */
},
};
/* transitions from state FULL_COM_NETWORK_REQUESTED */
STATIC CONST(ComM_HsmTranType, COMM_CONST)
 ComM_HsmTrFULL_COM_NETWORK_REQUESTED[3] =
{
{
  ComM_HsmTrFULL_COM_NETWORK_REQUESTEDSteps1,
  sizeof(ComM_HsmTrFULL_COM_NETWORK_REQUESTEDSteps1)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMM_EV_PNC_NM_REQUEST, /* triggering event */
  COMM_HSM_GIDX_INVALID, /* no guard */
  COMM_HSM_SIDX_INVALID /* no target state, internal transition */
},
{
  ComM_HsmTrFULL_COM_NETWORK_REQUESTEDSteps2,
  sizeof(ComM_HsmTrFULL_COM_NETWORK_REQUESTEDSteps2)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMM_EV_LIMIT_TO_NO_COM, /* triggering event */
  COMM_HSM_COMM_GIDX_FULL_COM_NETWORK_REQUESTED_GUARD_2, /* guard idx */
  COMM_HSM_COMM_SIDX_FULL_COM_READY_SLEEP /* target state */
},
{
  ComM_HsmTrFULL_COM_NETWORK_REQUESTEDSteps3,
  sizeof(ComM_HsmTrFULL_COM_NETWORK_REQUESTEDSteps3)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMM_EV_NM_MODE_SWITCHED, /* triggering event */
  COMM_HSM_COMM_GIDX_FULL_COM_NETWORK_REQUESTED_GUARD_3, /* guard idx */
  COMM_HSM_COMM_SIDX_SILENT_COMMUNICATION /* target state */
},
};
/* transitions from state FULL_COM_NETWORK_REQUESTED_ENTRY */
STATIC CONST(ComM_HsmTranType, COMM_CONST)
 ComM_HsmTrFULL_COM_NETWORK_REQUESTED_ENTRY[4] =
{
{
  ComM_HsmTrFULL_COM_NETWORK_REQUESTED_ENTRYSteps1,
  sizeof(ComM_HsmTrFULL_COM_NETWORK_REQUESTED_ENTRYSteps1)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMM_EV_REQ_STATUS_CHANGED, /* triggering event */
  COMM_HSM_COMM_GIDX_FULL_COM_NETWORK_REQUESTED_ENTRY_GUARD_1, /* guard idx */
  COMM_HSM_COMM_SIDX_FULL_COM_NETWORK_REQUESTED_PENDING /* target state */
},
{
  ComM_HsmTrFULL_COM_NETWORK_REQUESTED_ENTRYSteps2,
  sizeof(ComM_HsmTrFULL_COM_NETWORK_REQUESTED_ENTRYSteps2)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMM_EV_REQ_STATUS_CHANGED, /* triggering event */
  COMM_HSM_COMM_GIDX_FULL_COM_NETWORK_REQUESTED_ENTRY_GUARD_2, /* guard idx */
  COMM_HSM_COMM_SIDX_FULL_COM_READY_SLEEP /* target state */
},
{
  ComM_HsmTrFULL_COM_NETWORK_REQUESTED_ENTRYSteps3,
  sizeof(ComM_HsmTrFULL_COM_NETWORK_REQUESTED_ENTRYSteps3)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMM_EV_TIMEOUT, /* triggering event */
  COMM_HSM_COMM_GIDX_FULL_COM_NETWORK_REQUESTED_ENTRY_GUARD_3, /* guard idx */
  COMM_HSM_COMM_SIDX_FULL_COM_READY_SLEEP /* target state */
},
{
  ComM_HsmTrFULL_COM_NETWORK_REQUESTED_ENTRYSteps4,
  sizeof(ComM_HsmTrFULL_COM_NETWORK_REQUESTED_ENTRYSteps4)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMM_EV_TIMEOUT, /* triggering event */
  COMM_HSM_COMM_GIDX_FULL_COM_NETWORK_REQUESTED_ENTRY_GUARD_4, /* guard idx */
  COMM_HSM_COMM_SIDX_FULL_COM_NETWORK_REQUESTED_PENDING /* target state */
},
};
/* transitions from state FULL_COM_NETWORK_REQUESTED_PENDING */
STATIC CONST(ComM_HsmTranType, COMM_CONST)
 ComM_HsmTrFULL_COM_NETWORK_REQUESTED_PENDING[1] =
{
{
  ComM_HsmTrFULL_COM_NETWORK_REQUESTED_PENDINGSteps1,
  sizeof(ComM_HsmTrFULL_COM_NETWORK_REQUESTED_PENDINGSteps1)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMM_EV_REQ_STATUS_CHANGED, /* triggering event */
  COMM_HSM_COMM_GIDX_FULL_COM_NETWORK_REQUESTED_PENDING_GUARD_1, /* guard idx */
  COMM_HSM_COMM_SIDX_FULL_COM_READY_SLEEP /* target state */
},
};
/* transitions from state FULL_COM_READY_SLEEP */
STATIC CONST(ComM_HsmTranType, COMM_CONST)
 ComM_HsmTrFULL_COM_READY_SLEEP[3] =
{
{
  ComM_HsmTrFULL_COM_READY_SLEEPSteps1,
  sizeof(ComM_HsmTrFULL_COM_READY_SLEEPSteps1)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMM_EV_NM_MODE_SWITCHED, /* triggering event */
  COMM_HSM_COMM_GIDX_FULL_COM_READY_SLEEP_GUARD_1, /* guard idx */
  COMM_HSM_COMM_SIDX_SILENT_COMMUNICATION /* target state */
},
{
  ComM_HsmTrFULL_COM_READY_SLEEPSteps2,
  sizeof(ComM_HsmTrFULL_COM_READY_SLEEPSteps2)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMM_EV_REQ_STATUS_CHANGED, /* triggering event */
  COMM_HSM_COMM_GIDX_FULL_COM_READY_SLEEP_GUARD_2, /* guard idx */
  COMM_HSM_COMM_SIDX_FULL_COM_NETWORK_REQUESTED /* target state */
},
{
  ComM_HsmTrFULL_COM_READY_SLEEPSteps3,
  sizeof(ComM_HsmTrFULL_COM_READY_SLEEPSteps3)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMM_EV_TIMEOUT, /* triggering event */
  COMM_HSM_GIDX_INVALID, /* no guard */
  COMM_HSM_COMM_SIDX_NO_COMMUNICATION /* target state */
},
};
/* transitions from state NO_COM_NO_PENDING_REQUEST */
STATIC CONST(ComM_HsmTranType, COMM_CONST)
 ComM_HsmTrNO_COM_NO_PENDING_REQUEST[2] =
{
{
  ComM_HsmTrNO_COM_NO_PENDING_REQUESTSteps1,
  sizeof(ComM_HsmTrNO_COM_NO_PENDING_REQUESTSteps1)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMM_EV_PASSIVE_WAKEUP_IND, /* triggering event */
  COMM_HSM_GIDX_INVALID, /* no guard */
  COMM_HSM_COMM_SIDX_NO_COM_REQUEST_PENDING /* target state */
},
{
  ComM_HsmTrNO_COM_NO_PENDING_REQUESTSteps2,
  sizeof(ComM_HsmTrNO_COM_NO_PENDING_REQUESTSteps2)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMM_EV_REQ_STATUS_CHANGED, /* triggering event */
  COMM_HSM_COMM_GIDX_NO_COM_NO_PENDING_REQUEST_GUARD_2, /* guard idx */
  COMM_HSM_COMM_SIDX_NO_COM_REQUEST_PENDING /* target state */
},
};
/* transitions from state NO_COM_REQUEST_PENDING */
STATIC CONST(ComM_HsmTranType, COMM_CONST)
 ComM_HsmTrNO_COM_REQUEST_PENDING[5] =
{
{
  ComM_HsmTrNO_COM_REQUEST_PENDINGSteps1,
  sizeof(ComM_HsmTrNO_COM_REQUEST_PENDINGSteps1)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMM_EV_COMALLOWED_CHANGED, /* triggering event */
  COMM_HSM_COMM_GIDX_NO_COM_REQUEST_PENDING_GUARD_1, /* guard idx */
  COMM_HSM_COMM_SIDX_FULL_COMMUNICATION /* target state */
},
{
  ComM_HsmTrNO_COM_REQUEST_PENDINGSteps2,
  sizeof(ComM_HsmTrNO_COM_REQUEST_PENDINGSteps2)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMM_EV_LIMIT_TO_NO_COM, /* triggering event */
  COMM_HSM_COMM_GIDX_NO_COM_REQUEST_PENDING_GUARD_2, /* guard idx */
  COMM_HSM_COMM_SIDX_NO_COM_NO_PENDING_REQUEST /* target state */
},
{
  ComM_HsmTrNO_COM_REQUEST_PENDINGSteps3,
  sizeof(ComM_HsmTrNO_COM_REQUEST_PENDINGSteps3)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMM_EV_PASSIVE_WAKEUP_IND, /* triggering event */
  COMM_HSM_GIDX_INVALID, /* no guard */
  COMM_HSM_COMM_SIDX_NO_COM_REQUEST_PENDING /* target state */
},
{
  ComM_HsmTrNO_COM_REQUEST_PENDINGSteps4,
  sizeof(ComM_HsmTrNO_COM_REQUEST_PENDINGSteps4)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMM_EV_REQ_STATUS_CHANGED, /* triggering event */
  COMM_HSM_COMM_GIDX_NO_COM_REQUEST_PENDING_GUARD_4, /* guard idx */
  COMM_HSM_COMM_SIDX_NO_COM_NO_PENDING_REQUEST /* target state */
},
{
  ComM_HsmTrNO_COM_REQUEST_PENDINGSteps5,
  sizeof(ComM_HsmTrNO_COM_REQUEST_PENDINGSteps5)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMM_EV_WAKEUP_INHIBITION, /* triggering event */
  COMM_HSM_COMM_GIDX_NO_COM_REQUEST_PENDING_GUARD_5, /* guard idx */
  COMM_HSM_COMM_SIDX_NO_COM_NO_PENDING_REQUEST /* target state */
},
};
/* transitions from state SILENT_COMMUNICATION */
STATIC CONST(ComM_HsmTranType, COMM_CONST)
 ComM_HsmTrSILENT_COMMUNICATION[3] =
{
{
  ComM_HsmTrSILENT_COMMUNICATIONSteps1,
  sizeof(ComM_HsmTrSILENT_COMMUNICATIONSteps1)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMM_EV_NM_MODE_SWITCHED, /* triggering event */
  COMM_HSM_COMM_GIDX_SILENT_COMMUNICATION_GUARD_1, /* guard idx */
  COMM_HSM_COMM_SIDX_FULL_COM_READY_SLEEP /* target state */
},
{
  ComM_HsmTrSILENT_COMMUNICATIONSteps2,
  sizeof(ComM_HsmTrSILENT_COMMUNICATIONSteps2)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMM_EV_NM_MODE_SWITCHED, /* triggering event */
  COMM_HSM_COMM_GIDX_SILENT_COMMUNICATION_GUARD_2, /* guard idx */
  COMM_HSM_COMM_SIDX_NO_COMMUNICATION /* target state */
},
{
  ComM_HsmTrSILENT_COMMUNICATIONSteps3,
  sizeof(ComM_HsmTrSILENT_COMMUNICATIONSteps3)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMM_EV_REQ_STATUS_CHANGED, /* triggering event */
  COMM_HSM_COMM_GIDX_SILENT_COMMUNICATION_GUARD_3, /* guard idx */
  COMM_HSM_COMM_SIDX_FULL_COMMUNICATION /* target state */
},
};



/* statechart model description of the ComM state
 * machine */
STATIC CONST(ComM_HsmStateType, COMM_CONST)
  ComM_HsmScdComM[COMM_HSM_COMM_NO_OF_STATES] =
{
  {
    /* description of state TOP */
    0U, /* no non-deferred event mask (no leaf state) */
    0U, /* no ignored event mask (no leaf state) */
    ComM_HsmTrTOP, /* transitions for this state */
    COMM_HSM_SIDX_INVALID, /* no parent state */
    COMM_HSM_COMM_SIDX_NO_COMMUNICATION, /* init state */
      COMM_HSM_AIDX_INVALID, /* no entry action */
    COMM_HSM_AIDX_INVALID, /* no exit action */
    COMM_HSM_AIDX_INVALID, /* no do action */
    1 /* num of transitions */
  },
  {
    /* description of state FULL_COMMUNICATION */
    0U, /* no non-deferred event mask (no leaf state) */
    0U, /* no ignored event mask (no leaf state) */
    ComM_HsmTrFULL_COMMUNICATION, /* transitions for this state */
    COMM_HSM_COMM_SIDX_TOP, /* parent state */
    COMM_HSM_COMM_SIDX_FULL_COM_NETWORK_REQUESTED, /* init state */
      COMM_HSM_COMM_AIDX_FULL_COMMUNICATION_ENTRY, /* entry function */
    COMM_HSM_AIDX_INVALID, /* no exit action */
    COMM_HSM_AIDX_INVALID, /* no do action */
    2 /* num of transitions */
  },
  {
    /* description of state FULL_COM_NETWORK_REQUESTED */
    0U, /* no non-deferred event mask (no leaf state) */
    0U, /* no ignored event mask (no leaf state) */
    ComM_HsmTrFULL_COM_NETWORK_REQUESTED, /* transitions for this state */
    COMM_HSM_COMM_SIDX_FULL_COMMUNICATION, /* parent state */
    COMM_HSM_COMM_SIDX_FULL_COM_NETWORK_REQUESTED_ENTRY, /* init state */
      COMM_HSM_COMM_AIDX_FULL_COM_NETWORK_REQUESTED_ENTRY, /* entry function */
    COMM_HSM_AIDX_INVALID, /* no exit action */
    COMM_HSM_AIDX_INVALID, /* no do action */
    3 /* num of transitions */
  },
  {
    /* description of state FULL_COM_NETWORK_REQUESTED_ENTRY */
    /* non-deferred and non-ignored event mask */
   (1UL << COMM_HSM_COMM_EV_BUSSM_MODE_IND)
    |   (1UL << COMM_HSM_COMM_EV_BUS_SM_SLEEP_INDICATION)
    |   (1UL << COMM_HSM_COMM_EV_PNC_NM_REQUEST)
    |   (1UL << COMM_HSM_COMM_EV_TIMEOUT)
    |   (1UL << COMM_HSM_COMM_EV_LIMIT_TO_NO_COM)
    |   (1UL << COMM_HSM_COMM_EV_REQ_STATUS_CHANGED)
    | 0UL
    ,
      /* ignored event mask */
   (1UL << COMM_HSM_COMM_EV_COMALLOWED_CHANGED)
    |   (1UL << COMM_HSM_COMM_EV_PASSIVE_WAKEUP_IND)
    |   (1UL << COMM_HSM_COMM_EV_WAKEUP_INHIBITION)
    |   (1UL << COMM_HSM_COMM_EV_NM_MODE_SWITCHED)
    | 0UL
    ,
    ComM_HsmTrFULL_COM_NETWORK_REQUESTED_ENTRY, /* transitions for this state */
    COMM_HSM_COMM_SIDX_FULL_COM_NETWORK_REQUESTED, /* parent state */
    COMM_HSM_SIDX_INVALID, /* no init state */
      COMM_HSM_COMM_AIDX_FULL_COM_NETWORK_REQUESTED_ENTRY_ENTRY, /* entry function */
    COMM_HSM_COMM_AIDX_FULL_COM_NETWORK_REQUESTED_ENTRY_EXIT, /* exit function */
    COMM_HSM_AIDX_INVALID, /* no do action */
    4 /* num of transitions */
  },
  {
    /* description of state FULL_COM_NETWORK_REQUESTED_PENDING */
    /* non-deferred and non-ignored event mask */
   (1UL << COMM_HSM_COMM_EV_BUSSM_MODE_IND)
    |   (1UL << COMM_HSM_COMM_EV_BUS_SM_SLEEP_INDICATION)
    |   (1UL << COMM_HSM_COMM_EV_PNC_NM_REQUEST)
    |   (1UL << COMM_HSM_COMM_EV_LIMIT_TO_NO_COM)
    |   (1UL << COMM_HSM_COMM_EV_NM_MODE_SWITCHED)
    |   (1UL << COMM_HSM_COMM_EV_REQ_STATUS_CHANGED)
    | 0UL
    ,
      /* ignored event mask */
   (1UL << COMM_HSM_COMM_EV_TIMEOUT)
    |   (1UL << COMM_HSM_COMM_EV_COMALLOWED_CHANGED)
    |   (1UL << COMM_HSM_COMM_EV_PASSIVE_WAKEUP_IND)
    |   (1UL << COMM_HSM_COMM_EV_WAKEUP_INHIBITION)
    | 0UL
    ,
    ComM_HsmTrFULL_COM_NETWORK_REQUESTED_PENDING, /* transitions for this state */
    COMM_HSM_COMM_SIDX_FULL_COM_NETWORK_REQUESTED, /* parent state */
    COMM_HSM_SIDX_INVALID, /* no init state */
      COMM_HSM_AIDX_INVALID, /* no entry action */
    COMM_HSM_AIDX_INVALID, /* no exit action */
    COMM_HSM_AIDX_INVALID, /* no do action */
    1 /* num of transitions */
  },
  {
    /* description of state FULL_COM_READY_SLEEP */
    /* non-deferred and non-ignored event mask */
   (1UL << COMM_HSM_COMM_EV_BUSSM_MODE_IND)
    |   (1UL << COMM_HSM_COMM_EV_BUS_SM_SLEEP_INDICATION)
    |   (1UL << COMM_HSM_COMM_EV_TIMEOUT)
    |   (1UL << COMM_HSM_COMM_EV_NM_MODE_SWITCHED)
    |   (1UL << COMM_HSM_COMM_EV_REQ_STATUS_CHANGED)
    | 0UL
    ,
      /* ignored event mask */
   (1UL << COMM_HSM_COMM_EV_PNC_NM_REQUEST)
    |   (1UL << COMM_HSM_COMM_EV_COMALLOWED_CHANGED)
    |   (1UL << COMM_HSM_COMM_EV_LIMIT_TO_NO_COM)
    |   (1UL << COMM_HSM_COMM_EV_PASSIVE_WAKEUP_IND)
    |   (1UL << COMM_HSM_COMM_EV_WAKEUP_INHIBITION)
    | 0UL
    ,
    ComM_HsmTrFULL_COM_READY_SLEEP, /* transitions for this state */
    COMM_HSM_COMM_SIDX_FULL_COMMUNICATION, /* parent state */
    COMM_HSM_SIDX_INVALID, /* no init state */
      COMM_HSM_COMM_AIDX_FULL_COM_READY_SLEEP_ENTRY, /* entry function */
    COMM_HSM_COMM_AIDX_FULL_COM_READY_SLEEP_EXIT, /* exit function */
    COMM_HSM_AIDX_INVALID, /* no do action */
    3 /* num of transitions */
  },
  {
    /* description of state NO_COMMUNICATION */
    0U, /* no non-deferred event mask (no leaf state) */
    0U, /* no ignored event mask (no leaf state) */
    NULL_PTR,  /* no transitions */
    COMM_HSM_COMM_SIDX_TOP, /* parent state */
    COMM_HSM_COMM_SIDX_NO_COM_NO_PENDING_REQUEST, /* init state */
      COMM_HSM_COMM_AIDX_NO_COMMUNICATION_ENTRY, /* entry function */
    COMM_HSM_AIDX_INVALID, /* no exit action */
    COMM_HSM_AIDX_INVALID, /* no do action */
    0U  /* no transitions */
  },
  {
    /* description of state NO_COM_NO_PENDING_REQUEST */
    /* non-deferred and non-ignored event mask */
   (1UL << COMM_HSM_COMM_EV_BUSSM_MODE_IND)
    |   (1UL << COMM_HSM_COMM_EV_PASSIVE_WAKEUP_IND)
    |   (1UL << COMM_HSM_COMM_EV_REQ_STATUS_CHANGED)
    | 0UL
    ,
      /* ignored event mask */
   (1UL << COMM_HSM_COMM_EV_BUS_SM_SLEEP_INDICATION)
    |   (1UL << COMM_HSM_COMM_EV_PNC_NM_REQUEST)
    |   (1UL << COMM_HSM_COMM_EV_TIMEOUT)
    |   (1UL << COMM_HSM_COMM_EV_COMALLOWED_CHANGED)
    |   (1UL << COMM_HSM_COMM_EV_LIMIT_TO_NO_COM)
    |   (1UL << COMM_HSM_COMM_EV_WAKEUP_INHIBITION)
    |   (1UL << COMM_HSM_COMM_EV_NM_MODE_SWITCHED)
    | 0UL
    ,
    ComM_HsmTrNO_COM_NO_PENDING_REQUEST, /* transitions for this state */
    COMM_HSM_COMM_SIDX_NO_COMMUNICATION, /* parent state */
    COMM_HSM_SIDX_INVALID, /* no init state */
      COMM_HSM_COMM_AIDX_NO_COM_NO_PENDING_REQUEST_ENTRY, /* entry function */
    COMM_HSM_AIDX_INVALID, /* no exit action */
    COMM_HSM_AIDX_INVALID, /* no do action */
    2 /* num of transitions */
  },
  {
    /* description of state NO_COM_REQUEST_PENDING */
    /* non-deferred and non-ignored event mask */
   (1UL << COMM_HSM_COMM_EV_BUSSM_MODE_IND)
    |   (1UL << COMM_HSM_COMM_EV_COMALLOWED_CHANGED)
    |   (1UL << COMM_HSM_COMM_EV_LIMIT_TO_NO_COM)
    |   (1UL << COMM_HSM_COMM_EV_PASSIVE_WAKEUP_IND)
    |   (1UL << COMM_HSM_COMM_EV_WAKEUP_INHIBITION)
    |   (1UL << COMM_HSM_COMM_EV_REQ_STATUS_CHANGED)
    | 0UL
    ,
      /* ignored event mask */
   (1UL << COMM_HSM_COMM_EV_BUS_SM_SLEEP_INDICATION)
    |   (1UL << COMM_HSM_COMM_EV_PNC_NM_REQUEST)
    |   (1UL << COMM_HSM_COMM_EV_TIMEOUT)
    |   (1UL << COMM_HSM_COMM_EV_NM_MODE_SWITCHED)
    | 0UL
    ,
    ComM_HsmTrNO_COM_REQUEST_PENDING, /* transitions for this state */
    COMM_HSM_COMM_SIDX_NO_COMMUNICATION, /* parent state */
    COMM_HSM_SIDX_INVALID, /* no init state */
      COMM_HSM_COMM_AIDX_NO_COM_REQUEST_PENDING_ENTRY, /* entry function */
    COMM_HSM_AIDX_INVALID, /* no exit action */
    COMM_HSM_AIDX_INVALID, /* no do action */
    5 /* num of transitions */
  },
  {
    /* description of state SILENT_COMMUNICATION */
    /* non-deferred and non-ignored event mask */
   (1UL << COMM_HSM_COMM_EV_BUSSM_MODE_IND)
    |   (1UL << COMM_HSM_COMM_EV_NM_MODE_SWITCHED)
    |   (1UL << COMM_HSM_COMM_EV_REQ_STATUS_CHANGED)
    | 0UL
    ,
      /* ignored event mask */
   (1UL << COMM_HSM_COMM_EV_BUS_SM_SLEEP_INDICATION)
    |   (1UL << COMM_HSM_COMM_EV_PNC_NM_REQUEST)
    |   (1UL << COMM_HSM_COMM_EV_TIMEOUT)
    |   (1UL << COMM_HSM_COMM_EV_COMALLOWED_CHANGED)
    |   (1UL << COMM_HSM_COMM_EV_LIMIT_TO_NO_COM)
    |   (1UL << COMM_HSM_COMM_EV_PASSIVE_WAKEUP_IND)
    |   (1UL << COMM_HSM_COMM_EV_WAKEUP_INHIBITION)
    | 0UL
    ,
    ComM_HsmTrSILENT_COMMUNICATION, /* transitions for this state */
    COMM_HSM_COMM_SIDX_TOP, /* parent state */
    COMM_HSM_SIDX_INVALID, /* no init state */
      COMM_HSM_COMM_AIDX_SILENT_COMMUNICATION_ENTRY, /* entry function */
    COMM_HSM_AIDX_INVALID, /* no exit action */
    COMM_HSM_AIDX_INVALID, /* no do action */
    3 /* num of transitions */
  }
};

#if (COMM_HSM_TRACE == STD_ON)
STATIC CONSTP2CONST(char, COMM_CONST, COMM_CONST)
  ComM_HsmComMStateNames[] =
{
  "TOP",
  "FULL_COMMUNICATION",
  "FULL_COM_NETWORK_REQUESTED",
  "FULL_COM_NETWORK_REQUESTED_ENTRY",
  "FULL_COM_NETWORK_REQUESTED_PENDING",
  "FULL_COM_READY_SLEEP",
  "NO_COMMUNICATION",
  "NO_COM_NO_PENDING_REQUEST",
  "NO_COM_REQUEST_PENDING",
  "SILENT_COMMUNICATION",
};

STATIC CONSTP2CONST(char, COMM_CONST, COMM_CONST)
  ComM_HsmComMActionNames[] =
{
  "TOP action 1",
  "FULL_COMMUNICATION entry",
  "FULL_COM_NETWORK_REQUESTED entry",
  "FULL_COM_NETWORK_REQUESTED action 1",
  "FULL_COM_NETWORK_REQUESTED_ENTRY entry",
  "FULL_COM_NETWORK_REQUESTED_ENTRY exit",
  "FULL_COM_READY_SLEEP entry",
  "FULL_COM_READY_SLEEP exit",
  "FULL_COM_READY_SLEEP action 3",
  "NO_COMMUNICATION entry",
  "NO_COM_NO_PENDING_REQUEST entry",
  "NO_COM_NO_PENDING_REQUEST action 1",
  "NO_COM_REQUEST_PENDING entry",
  "NO_COM_REQUEST_PENDING action 2",
  "NO_COM_REQUEST_PENDING action 3",
  "SILENT_COMMUNICATION entry",
};

STATIC CONSTP2CONST(char, COMM_CONST, COMM_CONST)
  ComM_HsmComMGuardNames[] =
{
  "FULL_COMMUNICATION guard 2",
  "FULL_COM_NETWORK_REQUESTED guard 2",
  "FULL_COM_NETWORK_REQUESTED guard 3",
  "FULL_COM_NETWORK_REQUESTED_ENTRY guard 1",
  "FULL_COM_NETWORK_REQUESTED_ENTRY guard 2",
  "FULL_COM_NETWORK_REQUESTED_ENTRY guard 3",
  "FULL_COM_NETWORK_REQUESTED_ENTRY guard 4",
  "FULL_COM_NETWORK_REQUESTED_PENDING guard 1",
  "FULL_COM_READY_SLEEP guard 1",
  "FULL_COM_READY_SLEEP guard 2",
  "NO_COM_NO_PENDING_REQUEST guard 2",
  "NO_COM_REQUEST_PENDING guard 1",
  "NO_COM_REQUEST_PENDING guard 2",
  "NO_COM_REQUEST_PENDING guard 4",
  "NO_COM_REQUEST_PENDING guard 5",
  "SILENT_COMMUNICATION guard 1",
  "SILENT_COMMUNICATION guard 2",
  "SILENT_COMMUNICATION guard 3",
};

STATIC CONSTP2CONST(char, COMM_CONST, COMM_CONST)
  ComM_HsmComMEventNames[] =
{
  "BUSSM_MODE_IND",
  "BUS_SM_SLEEP_INDICATION",
  "PNC_NM_REQUEST",
  "TIMEOUT",
  "COMALLOWED_CHANGED",
  "LIMIT_TO_NO_COM",
  "PASSIVE_WAKEUP_IND",
  "WAKEUP_INHIBITION",
  "NM_MODE_SWITCHED",
  "REQ_STATUS_CHANGED",
};
#endif

/*==================[external constants]====================================*/

/* Top level state chart description data */
CONST(ComM_HsmStatechartType, COMM_CONST)
  ComM_HsmScComM =
{
  ComM_HsmScdComM, /* state description array */
  ComM_HsmInstComM, /* instances RAM */
  ComM_HsmComMActions, /* actions */
  ComM_HsmComMGuards, /* guards */
  ComM_HsmEvCountersComM, /* event counters RAM */
  ComM_HsmEvQueuesComM, /* event queues RAM */
#if (COMM_HSM_TRACE == STD_ON)
  "ComM", /* name of the HSM */
  ComM_HsmComMStateNames,
  ComM_HsmComMActionNames,
  ComM_HsmComMGuardNames,
  ComM_HsmComMEventNames,
#endif
  COMM_HSM_COMM_NUM_EVENTS, /* num of events */
  0U, /* state chart id */
  COMM_HSM_COMM_NUM_INST, /* num of instances */
  COMM_HSM_COMM_SIDX_TOP /* top state index */
};


#define COMM_STOP_SEC_CONST_UNSPECIFIED
#include <ComM_MemMap.h>

/*==================[internal function definitions]=========================*/

/*==================[external function definitions]=========================*/

/*==================[end of file]===========================================*/
