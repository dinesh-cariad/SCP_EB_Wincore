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
 * ComMPnc.
 *
 * This file contains the definition of all used data structures of the state
 * machine model.  It is generated and must not be edited.
 */

/*==================[inclusions]============================================*/

#include <ComM_Hsm.h> /* state machine driver interface */
#include <ComM_HsmComMPnc.h> /* public statechart model definitions, */
#include <ComM_HsmComMPncInt.h> /* internal statechart model definitions */

/*==================[macros]================================================*/

/** \brief Number of user events */
#if (defined COMM_HSM_COMMPNC_NUM_EVENTS)
#error COMM_HSM_COMMPNC_NUM_EVENTS already defined
#endif
#define COMM_HSM_COMMPNC_NUM_EVENTS 7U

/* Check if number of hsm instances is defined in the user's configuration */
#ifndef COMM_HSM_COMMPNC_NUM_INST
#error COMM_HSM_COMMPNC_NUM_INST must be defined in ComM_HsmCfg.h
#endif

/*==================[type definitions]======================================*/

/*==================[internal function declarations]========================*/

/*==================[external function declarations]========================*/

/*==================[internal data]=========================================*/

#if (COMM_NUM_PNC > 0U)
#define COMM_START_SEC_VAR_CLEARED_8
#include <ComM_MemMap.h>

/* RAM data for event counters */
STATIC VAR(uint8, COMM_VAR_CLEARED)
  ComM_HsmEvCountersComMPnc[
    COMM_HSM_COMMPNC_NUM_INST * COMM_HSM_COMMPNC_NUM_EVENTS];

/* RAM data for event queues */
STATIC VAR(ComM_HsmEventType, COMM_VAR_CLEARED)
  ComM_HsmEvQueuesComMPnc[
    COMM_HSM_COMMPNC_NUM_INST * COMM_HSM_COMMPNC_NUM_EVENTS];

#define COMM_STOP_SEC_VAR_CLEARED_8
#include <ComM_MemMap.h>


#define COMM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <ComM_MemMap.h>

/* RAM data for instances */
STATIC VAR(ComM_HsmInstType, COMM_VAR_CLEARED)
  ComM_HsmInstComMPnc[COMM_HSM_COMMPNC_NUM_INST];

#define COMM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <ComM_MemMap.h>

/*==================[external data]=========================================*/

/*==================[internal constants]====================================*/

#define COMM_START_SEC_CONST_UNSPECIFIED
#include <ComM_MemMap.h>

/* *** array of entry/exit/do and transition actions *** */
STATIC CONST(ComM_HsmActionType, COMM_CONST)
  ComM_HsmComMPncActions[COMM_HSM_COMMPNC_NO_OF_ACTIONS] =
{
  &ComM_HsmComMPncSfPncTopAction1,
  &ComM_HsmComMPncSfPncFullComEntry,
  &ComM_HsmComMPncSfPncPrepareSleepEntry,
  &ComM_HsmComMPncSfPncPrepareSleepAction1,
  &ComM_HsmComMPncSfPncPrepareSleepAction2,
  &ComM_HsmComMPncSfPncPrepareSleepAction3,
  &ComM_HsmComMPncSfPncReadySleepEntry,
  &ComM_HsmComMPncSfPncRequestedEntry,
  &ComM_HsmComMPncSfPncRequestedAction1,
  &ComM_HsmComMPncSfPncNoComEntry,
  &ComM_HsmComMPncSfPncNoComAction1,
  &ComM_HsmComMPncSfPncNoComAction3,
  &ComM_HsmComMPncSfPncNoComAction4,
  &ComM_HsmComMPncSfPncNoComNotifyEntry,
};

/* *** array of guards *** */
STATIC CONST(ComM_HsmGuardType, COMM_CONST)
  ComM_HsmComMPncGuards[COMM_HSM_COMMPNC_NO_OF_GUARDS] =
{
  &ComM_HsmComMPncSfPncPrepareSleepGuard1,
  &ComM_HsmComMPncSfPncReadySleepGuard1,
  &ComM_HsmComMPncSfPncRequestedGuard1,
  &ComM_HsmComMPncSfPncNoComGuard1,
  &ComM_HsmComMPncSfPncNoComGuard3,
};

#define COMM_STOP_SEC_CONST_UNSPECIFIED
#include <ComM_MemMap.h>

#define COMM_START_SEC_CONST_8
#include <ComM_MemMap.h>

/* *** exit/action/entry sequences for transitions *** */
/* trans sequence for transition #1 from state PncTop */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrPncTopSteps1[1] =
{
  COMM_HSM_COMMPNC_AIDX_PNCTOP_ACTION_1,
};
/* trans sequence for transition #1 from state PncPrepareSleep */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrPncPrepareSleepSteps1[2] =
{
  COMM_HSM_COMMPNC_AIDX_PNCPREPARESLEEP_ACTION_1,
  COMM_HSM_COMMPNC_AIDX_PNCREADYSLEEP_ENTRY,
};
/* trans sequence for transition #2 from state PncPrepareSleep */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrPncPrepareSleepSteps2[2] =
{
  COMM_HSM_COMMPNC_AIDX_PNCPREPARESLEEP_ACTION_2,
  COMM_HSM_COMMPNC_AIDX_PNCREQUESTED_ENTRY,
};
/* trans sequence for transition #3 from state PncPrepareSleep */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrPncPrepareSleepSteps3[3] =
{
  COMM_HSM_COMMPNC_AIDX_PNCPREPARESLEEP_ACTION_3,
  COMM_HSM_COMMPNC_AIDX_PNCNOCOM_ENTRY,
  COMM_HSM_COMMPNC_AIDX_PNCNOCOMNOTIFY_ENTRY,
};
/* trans sequence for transition #1 from state PncReadySleep */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrPncReadySleepSteps1[1] =
{
  COMM_HSM_COMMPNC_AIDX_PNCPREPARESLEEP_ENTRY,
};
/* trans sequence for transition #2 from state PncReadySleep */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrPncReadySleepSteps2[1] =
{
  COMM_HSM_COMMPNC_AIDX_PNCREQUESTED_ENTRY,
};
/* trans sequence for transition #1 from state PncRequested */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrPncRequestedSteps1[2] =
{
  COMM_HSM_COMMPNC_AIDX_PNCREQUESTED_ACTION_1,
  COMM_HSM_COMMPNC_AIDX_PNCREADYSLEEP_ENTRY,
};
/* trans sequence for transition #1 from state PncNoCom */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrPncNoComSteps1[3] =
{
  COMM_HSM_COMMPNC_AIDX_PNCNOCOM_ACTION_1,
  COMM_HSM_COMMPNC_AIDX_PNCFULLCOM_ENTRY,
  COMM_HSM_COMMPNC_AIDX_PNCREADYSLEEP_ENTRY,
};
/* trans sequence for transition #2 from state PncNoCom */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrPncNoComSteps2[2] =
{
  COMM_HSM_COMMPNC_AIDX_PNCFULLCOM_ENTRY,
  COMM_HSM_COMMPNC_AIDX_PNCPREPARESLEEP_ENTRY,
};
/* trans sequence for transition #3 from state PncNoCom */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrPncNoComSteps3[3] =
{
  COMM_HSM_COMMPNC_AIDX_PNCNOCOM_ACTION_3,
  COMM_HSM_COMMPNC_AIDX_PNCFULLCOM_ENTRY,
  COMM_HSM_COMMPNC_AIDX_PNCPREPARESLEEP_ENTRY,
};
/* trans sequence for transition #4 from state PncNoCom */
STATIC CONST(ComM_HsmActionIdxType, COMM_CONST)
  ComM_HsmTrPncNoComSteps4[3] =
{
  COMM_HSM_COMMPNC_AIDX_PNCNOCOM_ACTION_4,
  COMM_HSM_COMMPNC_AIDX_PNCFULLCOM_ENTRY,
  COMM_HSM_COMMPNC_AIDX_PNCREQUESTED_ENTRY,
};

#define COMM_STOP_SEC_CONST_8
#include <ComM_MemMap.h>

/* ****** */

#define COMM_START_SEC_CONST_UNSPECIFIED
#include <ComM_MemMap.h>

/* transitions from state PncTop */
STATIC CONST(ComM_HsmTranType, COMM_CONST)
 ComM_HsmTrPncTop[1] =
{
{
  ComM_HsmTrPncTopSteps1,
  sizeof(ComM_HsmTrPncTopSteps1)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMMPNC_EV_PNC_UPDATE_TX_PASSIVE, /* triggering event */
  COMM_HSM_GIDX_INVALID, /* no guard */
  COMM_HSM_SIDX_INVALID /* no target state, internal transition */
},
};
/* transitions from state PncPrepareSleep */
STATIC CONST(ComM_HsmTranType, COMM_CONST)
 ComM_HsmTrPncPrepareSleep[3] =
{
{
  ComM_HsmTrPncPrepareSleepSteps1,
  sizeof(ComM_HsmTrPncPrepareSleepSteps1)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMMPNC_EV_COMCBK, /* triggering event */
  COMM_HSM_COMMPNC_GIDX_PNCPREPARESLEEP_GUARD_1, /* guard idx */
  COMM_HSM_COMMPNC_SIDX_PNCREADYSLEEP /* target state */
},
{
  ComM_HsmTrPncPrepareSleepSteps2,
  sizeof(ComM_HsmTrPncPrepareSleepSteps2)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMMPNC_EV_FULL_COM_REQ_BY_COMM_USER, /* triggering event */
  COMM_HSM_GIDX_INVALID, /* no guard */
  COMM_HSM_COMMPNC_SIDX_PNCREQUESTED /* target state */
},
{
  ComM_HsmTrPncPrepareSleepSteps3,
  sizeof(ComM_HsmTrPncPrepareSleepSteps3)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMMPNC_EV_PREPARE_SLEEP_TIMEOUT, /* triggering event */
  COMM_HSM_GIDX_INVALID, /* no guard */
  COMM_HSM_COMMPNC_SIDX_PNCNOCOMNOTIFY /* target state */
},
};
/* transitions from state PncReadySleep */
STATIC CONST(ComM_HsmTranType, COMM_CONST)
 ComM_HsmTrPncReadySleep[2] =
{
{
  ComM_HsmTrPncReadySleepSteps1,
  sizeof(ComM_HsmTrPncReadySleepSteps1)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMMPNC_EV_COMCBK, /* triggering event */
  COMM_HSM_COMMPNC_GIDX_PNCREADYSLEEP_GUARD_1, /* guard idx */
  COMM_HSM_COMMPNC_SIDX_PNCPREPARESLEEP /* target state */
},
{
  ComM_HsmTrPncReadySleepSteps2,
  sizeof(ComM_HsmTrPncReadySleepSteps2)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMMPNC_EV_FULL_COM_REQ_BY_COMM_USER, /* triggering event */
  COMM_HSM_GIDX_INVALID, /* no guard */
  COMM_HSM_COMMPNC_SIDX_PNCREQUESTED /* target state */
},
};
/* transitions from state PncRequested */
STATIC CONST(ComM_HsmTranType, COMM_CONST)
 ComM_HsmTrPncRequested[1] =
{
{
  ComM_HsmTrPncRequestedSteps1,
  sizeof(ComM_HsmTrPncRequestedSteps1)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMMPNC_EV_NO_COM_REQ_BY_ALL_COMM_USER, /* triggering event */
  COMM_HSM_COMMPNC_GIDX_PNCREQUESTED_GUARD_1, /* guard idx */
  COMM_HSM_COMMPNC_SIDX_PNCREADYSLEEP /* target state */
},
};
/* transitions from state PncNoCom */
STATIC CONST(ComM_HsmTranType, COMM_CONST)
 ComM_HsmTrPncNoCom[4] =
{
{
  ComM_HsmTrPncNoComSteps1,
  sizeof(ComM_HsmTrPncNoComSteps1)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMMPNC_EV_COMCBK, /* triggering event */
  COMM_HSM_COMMPNC_GIDX_PNCNOCOM_GUARD_1, /* guard idx */
  COMM_HSM_COMMPNC_SIDX_PNCREADYSLEEP /* target state */
},
{
  ComM_HsmTrPncNoComSteps2,
  sizeof(ComM_HsmTrPncNoComSteps2)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMMPNC_EV_ECUM_PNCWAKEUP_IND, /* triggering event */
  COMM_HSM_GIDX_INVALID, /* no guard */
  COMM_HSM_COMMPNC_SIDX_PNCPREPARESLEEP /* target state */
},
{
  ComM_HsmTrPncNoComSteps3,
  sizeof(ComM_HsmTrPncNoComSteps3)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMMPNC_EV_ECUM_WAKEUP_IND, /* triggering event */
  COMM_HSM_COMMPNC_GIDX_PNCNOCOM_GUARD_3, /* guard idx */
  COMM_HSM_COMMPNC_SIDX_PNCPREPARESLEEP /* target state */
},
{
  ComM_HsmTrPncNoComSteps4,
  sizeof(ComM_HsmTrPncNoComSteps4)/sizeof(ComM_HsmActionIdxType),
  COMM_HSM_COMMPNC_EV_FULL_COM_REQ_BY_COMM_USER, /* triggering event */
  COMM_HSM_GIDX_INVALID, /* no guard */
  COMM_HSM_COMMPNC_SIDX_PNCREQUESTED /* target state */
},
};



/* statechart model description of the ComMPnc state
 * machine */
STATIC CONST(ComM_HsmStateType, COMM_CONST)
  ComM_HsmScdComMPnc[COMM_HSM_COMMPNC_NO_OF_STATES] =
{
  {
    /* description of state PncTop */
    0U, /* no non-deferred event mask (no leaf state) */
    0U, /* no ignored event mask (no leaf state) */
    ComM_HsmTrPncTop, /* transitions for this state */
    COMM_HSM_SIDX_INVALID, /* no parent state */
    COMM_HSM_COMMPNC_SIDX_PNCNOCOM, /* init state */
      COMM_HSM_AIDX_INVALID, /* no entry action */
    COMM_HSM_AIDX_INVALID, /* no exit action */
    COMM_HSM_AIDX_INVALID, /* no do action */
    1 /* num of transitions */
  },
  {
    /* description of state PncFullCom */
    0U, /* no non-deferred event mask (no leaf state) */
    0U, /* no ignored event mask (no leaf state) */
    NULL_PTR,  /* no transitions */
    COMM_HSM_COMMPNC_SIDX_PNCTOP, /* parent state */
    COMM_HSM_COMMPNC_SIDX_PNCPREPARESLEEP, /* init state */
      COMM_HSM_COMMPNC_AIDX_PNCFULLCOM_ENTRY, /* entry function */
    COMM_HSM_AIDX_INVALID, /* no exit action */
    COMM_HSM_AIDX_INVALID, /* no do action */
    0U  /* no transitions */
  },
  {
    /* description of state PncPrepareSleep */
    /* non-deferred and non-ignored event mask */
   (1UL << COMM_HSM_COMMPNC_EV_PNC_UPDATE_TX_PASSIVE)
    |   (1UL << COMM_HSM_COMMPNC_EV_PREPARE_SLEEP_TIMEOUT)
    |   (1UL << COMM_HSM_COMMPNC_EV_COMCBK)
    |   (1UL << COMM_HSM_COMMPNC_EV_FULL_COM_REQ_BY_COMM_USER)
    | 0UL
    ,
      /* ignored event mask */
   (1UL << COMM_HSM_COMMPNC_EV_NO_COM_REQ_BY_ALL_COMM_USER)
    |   (1UL << COMM_HSM_COMMPNC_EV_ECUM_PNCWAKEUP_IND)
    |   (1UL << COMM_HSM_COMMPNC_EV_ECUM_WAKEUP_IND)
    | 0UL
    ,
    ComM_HsmTrPncPrepareSleep, /* transitions for this state */
    COMM_HSM_COMMPNC_SIDX_PNCFULLCOM, /* parent state */
    COMM_HSM_SIDX_INVALID, /* no init state */
      COMM_HSM_COMMPNC_AIDX_PNCPREPARESLEEP_ENTRY, /* entry function */
    COMM_HSM_AIDX_INVALID, /* no exit action */
    COMM_HSM_AIDX_INVALID, /* no do action */
    3 /* num of transitions */
  },
  {
    /* description of state PncReadySleep */
    /* non-deferred and non-ignored event mask */
   (1UL << COMM_HSM_COMMPNC_EV_PNC_UPDATE_TX_PASSIVE)
    |   (1UL << COMM_HSM_COMMPNC_EV_COMCBK)
    |   (1UL << COMM_HSM_COMMPNC_EV_FULL_COM_REQ_BY_COMM_USER)
    | 0UL
    ,
      /* ignored event mask */
   (1UL << COMM_HSM_COMMPNC_EV_PREPARE_SLEEP_TIMEOUT)
    |   (1UL << COMM_HSM_COMMPNC_EV_NO_COM_REQ_BY_ALL_COMM_USER)
    |   (1UL << COMM_HSM_COMMPNC_EV_ECUM_PNCWAKEUP_IND)
    |   (1UL << COMM_HSM_COMMPNC_EV_ECUM_WAKEUP_IND)
    | 0UL
    ,
    ComM_HsmTrPncReadySleep, /* transitions for this state */
    COMM_HSM_COMMPNC_SIDX_PNCFULLCOM, /* parent state */
    COMM_HSM_SIDX_INVALID, /* no init state */
      COMM_HSM_COMMPNC_AIDX_PNCREADYSLEEP_ENTRY, /* entry function */
    COMM_HSM_AIDX_INVALID, /* no exit action */
    COMM_HSM_AIDX_INVALID, /* no do action */
    2 /* num of transitions */
  },
  {
    /* description of state PncRequested */
    /* non-deferred and non-ignored event mask */
   (1UL << COMM_HSM_COMMPNC_EV_PNC_UPDATE_TX_PASSIVE)
    |   (1UL << COMM_HSM_COMMPNC_EV_NO_COM_REQ_BY_ALL_COMM_USER)
    | 0UL
    ,
      /* ignored event mask */
   (1UL << COMM_HSM_COMMPNC_EV_PREPARE_SLEEP_TIMEOUT)
    |   (1UL << COMM_HSM_COMMPNC_EV_COMCBK)
    |   (1UL << COMM_HSM_COMMPNC_EV_ECUM_PNCWAKEUP_IND)
    |   (1UL << COMM_HSM_COMMPNC_EV_ECUM_WAKEUP_IND)
    |   (1UL << COMM_HSM_COMMPNC_EV_FULL_COM_REQ_BY_COMM_USER)
    | 0UL
    ,
    ComM_HsmTrPncRequested, /* transitions for this state */
    COMM_HSM_COMMPNC_SIDX_PNCFULLCOM, /* parent state */
    COMM_HSM_SIDX_INVALID, /* no init state */
      COMM_HSM_COMMPNC_AIDX_PNCREQUESTED_ENTRY, /* entry function */
    COMM_HSM_AIDX_INVALID, /* no exit action */
    COMM_HSM_AIDX_INVALID, /* no do action */
    1 /* num of transitions */
  },
  {
    /* description of state PncNoCom */
    0U, /* no non-deferred event mask (no leaf state) */
    0U, /* no ignored event mask (no leaf state) */
    ComM_HsmTrPncNoCom, /* transitions for this state */
    COMM_HSM_COMMPNC_SIDX_PNCTOP, /* parent state */
    COMM_HSM_COMMPNC_SIDX_PNCNOCOMINIT, /* init state */
      COMM_HSM_COMMPNC_AIDX_PNCNOCOM_ENTRY, /* entry function */
    COMM_HSM_AIDX_INVALID, /* no exit action */
    COMM_HSM_AIDX_INVALID, /* no do action */
    4 /* num of transitions */
  },
  {
    /* description of state PncNoComInit */
    /* non-deferred and non-ignored event mask */
   (1UL << COMM_HSM_COMMPNC_EV_PNC_UPDATE_TX_PASSIVE)
    |   (1UL << COMM_HSM_COMMPNC_EV_COMCBK)
    |   (1UL << COMM_HSM_COMMPNC_EV_ECUM_PNCWAKEUP_IND)
    |   (1UL << COMM_HSM_COMMPNC_EV_ECUM_WAKEUP_IND)
    |   (1UL << COMM_HSM_COMMPNC_EV_FULL_COM_REQ_BY_COMM_USER)
    | 0UL
    ,
      /* ignored event mask */
   (1UL << COMM_HSM_COMMPNC_EV_PREPARE_SLEEP_TIMEOUT)
    |   (1UL << COMM_HSM_COMMPNC_EV_NO_COM_REQ_BY_ALL_COMM_USER)
    | 0UL
    ,
    NULL_PTR,  /* no transitions */
    COMM_HSM_COMMPNC_SIDX_PNCNOCOM, /* parent state */
    COMM_HSM_SIDX_INVALID, /* no init state */
      COMM_HSM_AIDX_INVALID, /* no entry action */
    COMM_HSM_AIDX_INVALID, /* no exit action */
    COMM_HSM_AIDX_INVALID, /* no do action */
    0U  /* no transitions */
  },
  {
    /* description of state PncNoComNotify */
    /* non-deferred and non-ignored event mask */
   (1UL << COMM_HSM_COMMPNC_EV_PNC_UPDATE_TX_PASSIVE)
    |   (1UL << COMM_HSM_COMMPNC_EV_COMCBK)
    |   (1UL << COMM_HSM_COMMPNC_EV_ECUM_PNCWAKEUP_IND)
    |   (1UL << COMM_HSM_COMMPNC_EV_ECUM_WAKEUP_IND)
    |   (1UL << COMM_HSM_COMMPNC_EV_FULL_COM_REQ_BY_COMM_USER)
    | 0UL
    ,
      /* ignored event mask */
   (1UL << COMM_HSM_COMMPNC_EV_PREPARE_SLEEP_TIMEOUT)
    |   (1UL << COMM_HSM_COMMPNC_EV_NO_COM_REQ_BY_ALL_COMM_USER)
    | 0UL
    ,
    NULL_PTR,  /* no transitions */
    COMM_HSM_COMMPNC_SIDX_PNCNOCOM, /* parent state */
    COMM_HSM_SIDX_INVALID, /* no init state */
      COMM_HSM_COMMPNC_AIDX_PNCNOCOMNOTIFY_ENTRY, /* entry function */
    COMM_HSM_AIDX_INVALID, /* no exit action */
    COMM_HSM_AIDX_INVALID, /* no do action */
    0U  /* no transitions */
  }
};

#if (COMM_HSM_TRACE == STD_ON)
STATIC CONSTP2CONST(char, COMM_CONST, COMM_CONST)
  ComM_HsmComMPncStateNames[] =
{
  "PncTop",
  "PncFullCom",
  "PncPrepareSleep",
  "PncReadySleep",
  "PncRequested",
  "PncNoCom",
  "PncNoComInit",
  "PncNoComNotify",
};

STATIC CONSTP2CONST(char, COMM_CONST, COMM_CONST)
  ComM_HsmComMPncActionNames[] =
{
  "PncTop action 1",
  "PncPrepareSleep entry",
  "PncPrepareSleep action 1",
  "PncPrepareSleep action 2",
  "PncPrepareSleep action 3",
  "PncReadySleep entry",
  "PncRequested entry",
  "PncRequested action 1",
  "PncNoCom action 1",
  "PncNoCom action 3",
  "PncNoCom action 4",
  "PncNoComNotify entry",
};

STATIC CONSTP2CONST(char, COMM_CONST, COMM_CONST)
  ComM_HsmComMPncGuardNames[] =
{
  "PncPrepareSleep guard 1",
  "PncReadySleep guard 1",
  "PncRequested guard 1",
  "PncNoCom guard 1",
  "PncNoCom guard 3",
};

STATIC CONSTP2CONST(char, COMM_CONST, COMM_CONST)
  ComM_HsmComMPncEventNames[] =
{
  "PNC_UPDATE_TX_PASSIVE",
  "PREPARE_SLEEP_TIMEOUT",
  "NO_COM_REQ_BY_ALL_COMM_USER",
  "COMCBK",
  "ECUM_PNCWAKEUP_IND",
  "ECUM_WAKEUP_IND",
  "FULL_COM_REQ_BY_COMM_USER",
};
#endif

/*==================[external constants]====================================*/

/* Top level state chart description data */
CONST(ComM_HsmStatechartType, COMM_CONST)
  ComM_HsmScComMPnc =
{
  ComM_HsmScdComMPnc, /* state description array */
  ComM_HsmInstComMPnc, /* instances RAM */
  ComM_HsmComMPncActions, /* actions */
  ComM_HsmComMPncGuards, /* guards */
  ComM_HsmEvCountersComMPnc, /* event counters RAM */
  ComM_HsmEvQueuesComMPnc, /* event queues RAM */
#if (COMM_HSM_TRACE == STD_ON)
  "ComMPnc", /* name of the HSM */
  ComM_HsmComMPncStateNames,
  ComM_HsmComMPncActionNames,
  ComM_HsmComMPncGuardNames,
  ComM_HsmComMPncEventNames,
#endif
  COMM_HSM_COMMPNC_NUM_EVENTS, /* num of events */
  0U, /* state chart id */
  COMM_HSM_COMMPNC_NUM_INST, /* num of instances */
  COMM_HSM_COMMPNC_SIDX_PNCTOP /* top state index */
};


#define COMM_STOP_SEC_CONST_UNSPECIFIED
#include <ComM_MemMap.h>
#endif /* (COMM_NUM_PNC > 0U) */
/*==================[internal function definitions]=========================*/

/*==================[external function definitions]=========================*/

/*==================[end of file]===========================================*/
