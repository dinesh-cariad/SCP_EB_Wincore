/**
 * \file
 *
 * \brief AUTOSAR FrNm
 *
 * This file contains the implementation of the AUTOSAR
 * module FrNm.
 *
 * \version 5.16.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* Definition of the configuration data of the state machine
 * FrNm.
 *
 * This file contains the definition of all used data structures of the state
 * machine model.  It is generated and must not be edited.
 */
/*==================[inclusions]============================================*/
#define FRNM_NO_CFGCLASSMIX_REQUIRED
#ifndef FRNM_NO_PBCFG_REQUIRED
#define FRNM_NO_PBCFG_REQUIRED
#endif

#include <FrNm_Hsm.h> /* state machine driver interface */
#include <FrNm_HsmFrNm.h> /* public statechart model definitions, */
#include <FrNm_HsmFrNmInt.h> /* internal statechart model definitions */

/*==================[macros]================================================*/

/** \brief Number of user events */
#if (defined FRNM_HSM_FRNM_NUM_EVENTS)
#error FRNM_HSM_FRNM_NUM_EVENTS already defined
#endif
#define FRNM_HSM_FRNM_NUM_EVENTS 12U

/* Check if number of hsm instances is defined in the user's configuration */
#ifndef FRNM_HSM_FRNM_NUM_INST
#error FRNM_HSM_FRNM_NUM_INST must be defined in FrNm_HsmCfg.h
#endif


/*==================[type definitions]======================================*/

/*==================[internal function declarations]========================*/

/*==================[external function declarations]========================*/

/*==================[internal data]=========================================*/

#define FRNM_START_SEC_VAR_CLEARED_8
#include <FrNm_MemMap.h>

/* RAM data for event counters */
STATIC VAR(uint8, FRNM_VAR_CLEARED)
  FrNm_HsmEvCountersFrNm[
    FRNM_HSM_FRNM_NUM_INST * FRNM_HSM_FRNM_NUM_EVENTS];

#define FRNM_STOP_SEC_VAR_CLEARED_8
#include <FrNm_MemMap.h>


#define FRNM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <FrNm_MemMap.h>

/* RAM data for instances */
STATIC VAR(FrNm_HsmInstType, FRNM_VAR_CLEARED)
  FrNm_HsmInstFrNm[FRNM_HSM_FRNM_NUM_INST];

/* RAM data for event queues */
STATIC VAR(FrNm_HsmEventType, FRNM_VAR_CLEARED)
  FrNm_HsmEvQueuesFrNm[
    FRNM_HSM_FRNM_NUM_INST * FRNM_HSM_FRNM_NUM_EVENTS];

#define FRNM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <FrNm_MemMap.h>

/*==================[external data]=========================================*/

/*==================[internal constants]====================================*/

#define FRNM_START_SEC_CONST_UNSPECIFIED
#include <FrNm_MemMap.h>

/* *** array of entry/exit/do and transition actions *** */
STATIC CONST(FrNm_HsmActionType, FRNM_CONST)
  FrNm_HsmFrNmActions[FRNM_HSM_FRNM_NO_OF_ACTIONS] =
{
  &FrNm_HsmFrNmSfTOPEntry,
  &FrNm_HsmFrNmSfTOPAction1,
  &FrNm_HsmFrNmSfBusSleepModeEntry,
  &FrNm_HsmFrNmSfBusSleepModeAction1,
  &FrNm_HsmFrNmSfBusSleepModeAction2,
  &FrNm_HsmFrNmSfNetworkModeEntry,
  &FrNm_HsmFrNmSfNetworkModeAction1,
  &FrNm_HsmFrNmSfNetworkModeAction2,
  &FrNm_HsmFrNmSfNetworkModeAction3,
  &FrNm_HsmFrNmSfReadySleepStateEntry,
  &FrNm_HsmFrNmSfReadySleepStateExit,
  &FrNm_HsmFrNmSfReadySleepStateAction1,
  &FrNm_HsmFrNmSfReadySleepRemoteActivityEntry,
  &FrNm_HsmFrNmSfReadySleepRemoteActivityAction1,
  &FrNm_HsmFrNmSfReadySleepRemoteSleepEntry,
  &FrNm_HsmFrNmSfReadySleepRemoteSleepAction1,
  &FrNm_HsmFrNmSfReadySleepRemoteSleepAction3,
  &FrNm_HsmFrNmSfSendingSubModeEntry,
  &FrNm_HsmFrNmSfSendingSubModeExit,
  &FrNm_HsmFrNmSfSendingSubModeAction1,
  &FrNm_HsmFrNmSfSendingSubModeAction2,
  &FrNm_HsmFrNmSfSendingSubModeAction3,
  &FrNm_HsmFrNmSfSendingSubModeAction4,
  &FrNm_HsmFrNmSfNormalOperationStateEntry,
  &FrNm_HsmFrNmSfNormalOperationRemoteActivityEntry,
  &FrNm_HsmFrNmSfNormalOperationRemoteActivityAction1,
  &FrNm_HsmFrNmSfNormalOperationRemoteSleepEntry,
  &FrNm_HsmFrNmSfNormalOperationRemoteSleepAction1,
  &FrNm_HsmFrNmSfNormalOperationRemoteSleepAction3,
  &FrNm_HsmFrNmSfRepeatMessageStateEntry,
  &FrNm_HsmFrNmSfRepeatMessageStateExit,
  &FrNm_HsmFrNmSfRepeatMessageStateAction1,
  &FrNm_HsmFrNmSfSynchronizeModeEntry,
  &FrNm_HsmFrNmSfSynchronizeModeAction1,
};

/* *** array of guards *** */
STATIC CONST(FrNm_HsmGuardType, FRNM_CONST)
  FrNm_HsmFrNmGuards[FRNM_HSM_FRNM_NO_OF_GUARDS] =
{
  &FrNm_HsmFrNmSfReadySleepStateGuard1,
  &FrNm_HsmFrNmSfReadySleepRemoteActivityGuard2,
  &FrNm_HsmFrNmSfReadySleepRemoteActivityGuard3,
  &FrNm_HsmFrNmSfReadySleepRemoteSleepGuard1,
  &FrNm_HsmFrNmSfReadySleepRemoteSleepGuard2,
  &FrNm_HsmFrNmSfNormalOperationRemoteActivityGuard2,
  &FrNm_HsmFrNmSfNormalOperationRemoteActivityGuard3,
  &FrNm_HsmFrNmSfNormalOperationRemoteActivityGuard4,
  &FrNm_HsmFrNmSfNormalOperationRemoteSleepGuard1,
  &FrNm_HsmFrNmSfNormalOperationRemoteSleepGuard2,
  &FrNm_HsmFrNmSfRepeatMessageStateGuard2,
  &FrNm_HsmFrNmSfRepeatMessageStateGuard3,
  &FrNm_HsmFrNmSfSynchronizeModeGuard2,
  &FrNm_HsmFrNmSfSynchronizeModeGuard3,
};

#define FRNM_STOP_SEC_CONST_UNSPECIFIED
#include <FrNm_MemMap.h>

#define FRNM_START_SEC_CONST_8
#include <FrNm_MemMap.h>

/* *** exit/action/entry sequences for transitions *** */
/* trans sequence for transition #1 from state TOP */
STATIC CONST(FrNm_HsmActionIdxType, FRNM_CONST)
  FrNm_HsmTrTOPSteps1[1] =
{
  FRNM_HSM_FRNM_AIDX_TOP_ACTION_1,
};
/* trans sequence for transition #1 from state BusSleepMode */
STATIC CONST(FrNm_HsmActionIdxType, FRNM_CONST)
  FrNm_HsmTrBusSleepModeSteps1[1] =
{
  FRNM_HSM_FRNM_AIDX_BUSSLEEPMODE_ACTION_1,
};
/* trans sequence for transition #2 from state BusSleepMode */
STATIC CONST(FrNm_HsmActionIdxType, FRNM_CONST)
  FrNm_HsmTrBusSleepModeSteps2[2] =
{
  FRNM_HSM_FRNM_AIDX_BUSSLEEPMODE_ACTION_2,
  FRNM_HSM_FRNM_AIDX_SYNCHRONIZEMODE_ENTRY,
};
/* trans sequence for transition #1 from state NetworkMode */
STATIC CONST(FrNm_HsmActionIdxType, FRNM_CONST)
  FrNm_HsmTrNetworkModeSteps1[1] =
{
  FRNM_HSM_FRNM_AIDX_NETWORKMODE_ACTION_1,
};
/* trans sequence for transition #2 from state NetworkMode */
STATIC CONST(FrNm_HsmActionIdxType, FRNM_CONST)
  FrNm_HsmTrNetworkModeSteps2[2] =
{
  FRNM_HSM_FRNM_AIDX_NETWORKMODE_ACTION_2,
  FRNM_HSM_FRNM_AIDX_BUSSLEEPMODE_ENTRY,
};
/* trans sequence for transition #3 from state NetworkMode */
STATIC CONST(FrNm_HsmActionIdxType, FRNM_CONST)
  FrNm_HsmTrNetworkModeSteps3[2] =
{
  FRNM_HSM_FRNM_AIDX_NETWORKMODE_ACTION_3,
  FRNM_HSM_FRNM_AIDX_SYNCHRONIZEMODE_ENTRY,
};
/* trans sequence for transition #1 from state ReadySleepState */
STATIC CONST(FrNm_HsmActionIdxType, FRNM_CONST)
  FrNm_HsmTrReadySleepStateSteps1[3] =
{
  FRNM_HSM_FRNM_AIDX_READYSLEEPSTATE_EXIT,
  FRNM_HSM_FRNM_AIDX_READYSLEEPSTATE_ACTION_1,
  FRNM_HSM_FRNM_AIDX_BUSSLEEPMODE_ENTRY,
};
/* trans sequence for transition #1 from state ReadySleepRemoteActivity */
STATIC CONST(FrNm_HsmActionIdxType, FRNM_CONST)
  FrNm_HsmTrReadySleepRemoteActivitySteps1[1] =
{
  FRNM_HSM_FRNM_AIDX_READYSLEEPREMOTEACTIVITY_ACTION_1,
};
/* trans sequence for transition #2 from state ReadySleepRemoteActivity */
STATIC CONST(FrNm_HsmActionIdxType, FRNM_CONST)
  FrNm_HsmTrReadySleepRemoteActivitySteps2[3] =
{
  FRNM_HSM_FRNM_AIDX_READYSLEEPSTATE_EXIT,
  FRNM_HSM_FRNM_AIDX_SENDINGSUBMODE_ENTRY,
  FRNM_HSM_FRNM_AIDX_REPEATMESSAGESTATE_ENTRY,
};
/* trans sequence for transition #3 from state ReadySleepRemoteActivity */
STATIC CONST(FrNm_HsmActionIdxType, FRNM_CONST)
  FrNm_HsmTrReadySleepRemoteActivitySteps3[4] =
{
  FRNM_HSM_FRNM_AIDX_READYSLEEPSTATE_EXIT,
  FRNM_HSM_FRNM_AIDX_SENDINGSUBMODE_ENTRY,
  FRNM_HSM_FRNM_AIDX_NORMALOPERATIONSTATE_ENTRY,
  FRNM_HSM_FRNM_AIDX_NORMALOPERATIONREMOTEACTIVITY_ENTRY,
};
/* trans sequence for transition #1 from state ReadySleepRemoteSleep */
STATIC CONST(FrNm_HsmActionIdxType, FRNM_CONST)
  FrNm_HsmTrReadySleepRemoteSleepSteps1[4] =
{
  FRNM_HSM_FRNM_AIDX_READYSLEEPSTATE_EXIT,
  FRNM_HSM_FRNM_AIDX_READYSLEEPREMOTESLEEP_ACTION_1,
  FRNM_HSM_FRNM_AIDX_SENDINGSUBMODE_ENTRY,
  FRNM_HSM_FRNM_AIDX_REPEATMESSAGESTATE_ENTRY,
};
/* trans sequence for transition #2 from state ReadySleepRemoteSleep */
STATIC CONST(FrNm_HsmActionIdxType, FRNM_CONST)
  FrNm_HsmTrReadySleepRemoteSleepSteps2[4] =
{
  FRNM_HSM_FRNM_AIDX_READYSLEEPSTATE_EXIT,
  FRNM_HSM_FRNM_AIDX_SENDINGSUBMODE_ENTRY,
  FRNM_HSM_FRNM_AIDX_NORMALOPERATIONSTATE_ENTRY,
  FRNM_HSM_FRNM_AIDX_NORMALOPERATIONREMOTESLEEP_ENTRY,
};
/* trans sequence for transition #3 from state ReadySleepRemoteSleep */
STATIC CONST(FrNm_HsmActionIdxType, FRNM_CONST)
  FrNm_HsmTrReadySleepRemoteSleepSteps3[2] =
{
  FRNM_HSM_FRNM_AIDX_READYSLEEPREMOTESLEEP_ACTION_3,
  FRNM_HSM_FRNM_AIDX_READYSLEEPREMOTEACTIVITY_ENTRY,
};
/* trans sequence for transition #1 from state SendingSubMode */
STATIC CONST(FrNm_HsmActionIdxType, FRNM_CONST)
  FrNm_HsmTrSendingSubModeSteps1[1] =
{
  FRNM_HSM_FRNM_AIDX_SENDINGSUBMODE_ACTION_1,
};
/* trans sequence for transition #2 from state SendingSubMode */
STATIC CONST(FrNm_HsmActionIdxType, FRNM_CONST)
  FrNm_HsmTrSendingSubModeSteps2[1] =
{
  FRNM_HSM_FRNM_AIDX_SENDINGSUBMODE_ACTION_2,
};
/* trans sequence for transition #3 from state SendingSubMode */
STATIC CONST(FrNm_HsmActionIdxType, FRNM_CONST)
  FrNm_HsmTrSendingSubModeSteps3[1] =
{
  FRNM_HSM_FRNM_AIDX_SENDINGSUBMODE_ACTION_3,
};
/* trans sequence for transition #4 from state SendingSubMode */
STATIC CONST(FrNm_HsmActionIdxType, FRNM_CONST)
  FrNm_HsmTrSendingSubModeSteps4[1] =
{
  FRNM_HSM_FRNM_AIDX_SENDINGSUBMODE_ACTION_4,
};
/* trans sequence for transition #1 from state NormalOperationRemoteActivity */
STATIC CONST(FrNm_HsmActionIdxType, FRNM_CONST)
  FrNm_HsmTrNormalOperationRemoteActivitySteps1[1] =
{
  FRNM_HSM_FRNM_AIDX_NORMALOPERATIONREMOTEACTIVITY_ACTION_1,
};
/* trans sequence for transition #2 from state NormalOperationRemoteActivity */
STATIC CONST(FrNm_HsmActionIdxType, FRNM_CONST)
  FrNm_HsmTrNormalOperationRemoteActivitySteps2[1] =
{
  FRNM_HSM_FRNM_AIDX_REPEATMESSAGESTATE_ENTRY,
};
/* trans sequence for transition #3 from state NormalOperationRemoteActivity */
STATIC CONST(FrNm_HsmActionIdxType, FRNM_CONST)
  FrNm_HsmTrNormalOperationRemoteActivitySteps3[1] =
{
  FRNM_HSM_FRNM_AIDX_NORMALOPERATIONREMOTESLEEP_ENTRY,
};
/* trans sequence for transition #4 from state NormalOperationRemoteActivity */
STATIC CONST(FrNm_HsmActionIdxType, FRNM_CONST)
  FrNm_HsmTrNormalOperationRemoteActivitySteps4[3] =
{
  FRNM_HSM_FRNM_AIDX_SENDINGSUBMODE_EXIT,
  FRNM_HSM_FRNM_AIDX_READYSLEEPSTATE_ENTRY,
  FRNM_HSM_FRNM_AIDX_READYSLEEPREMOTEACTIVITY_ENTRY,
};
/* trans sequence for transition #1 from state NormalOperationRemoteSleep */
STATIC CONST(FrNm_HsmActionIdxType, FRNM_CONST)
  FrNm_HsmTrNormalOperationRemoteSleepSteps1[2] =
{
  FRNM_HSM_FRNM_AIDX_NORMALOPERATIONREMOTESLEEP_ACTION_1,
  FRNM_HSM_FRNM_AIDX_REPEATMESSAGESTATE_ENTRY,
};
/* trans sequence for transition #2 from state NormalOperationRemoteSleep */
STATIC CONST(FrNm_HsmActionIdxType, FRNM_CONST)
  FrNm_HsmTrNormalOperationRemoteSleepSteps2[3] =
{
  FRNM_HSM_FRNM_AIDX_SENDINGSUBMODE_EXIT,
  FRNM_HSM_FRNM_AIDX_READYSLEEPSTATE_ENTRY,
  FRNM_HSM_FRNM_AIDX_READYSLEEPREMOTESLEEP_ENTRY,
};
/* trans sequence for transition #3 from state NormalOperationRemoteSleep */
STATIC CONST(FrNm_HsmActionIdxType, FRNM_CONST)
  FrNm_HsmTrNormalOperationRemoteSleepSteps3[2] =
{
  FRNM_HSM_FRNM_AIDX_NORMALOPERATIONREMOTESLEEP_ACTION_3,
  FRNM_HSM_FRNM_AIDX_NORMALOPERATIONREMOTEACTIVITY_ENTRY,
};
/* trans sequence for transition #1 from state RepeatMessageState */
STATIC CONST(FrNm_HsmActionIdxType, FRNM_CONST)
  FrNm_HsmTrRepeatMessageStateSteps1[1] =
{
  FRNM_HSM_FRNM_AIDX_REPEATMESSAGESTATE_ACTION_1,
};
/* trans sequence for transition #2 from state RepeatMessageState */
STATIC CONST(FrNm_HsmActionIdxType, FRNM_CONST)
  FrNm_HsmTrRepeatMessageStateSteps2[2] =
{
  FRNM_HSM_FRNM_AIDX_REPEATMESSAGESTATE_EXIT,
  FRNM_HSM_FRNM_AIDX_NORMALOPERATIONSTATE_ENTRY,
};
/* trans sequence for transition #3 from state RepeatMessageState */
STATIC CONST(FrNm_HsmActionIdxType, FRNM_CONST)
  FrNm_HsmTrRepeatMessageStateSteps3[3] =
{
  FRNM_HSM_FRNM_AIDX_REPEATMESSAGESTATE_EXIT,
  FRNM_HSM_FRNM_AIDX_SENDINGSUBMODE_EXIT,
  FRNM_HSM_FRNM_AIDX_READYSLEEPSTATE_ENTRY,
};
/* trans sequence for transition #1 from state SynchronizeMode */
STATIC CONST(FrNm_HsmActionIdxType, FRNM_CONST)
  FrNm_HsmTrSynchronizeModeSteps1[2] =
{
  FRNM_HSM_FRNM_AIDX_SYNCHRONIZEMODE_ACTION_1,
  FRNM_HSM_FRNM_AIDX_BUSSLEEPMODE_ENTRY,
};
/* trans sequence for transition #2 from state SynchronizeMode */
STATIC CONST(FrNm_HsmActionIdxType, FRNM_CONST)
  FrNm_HsmTrSynchronizeModeSteps2[1] =
{
  FRNM_HSM_FRNM_AIDX_NETWORKMODE_ENTRY,
};
/* trans sequence for transition #3 from state SynchronizeMode */
STATIC CONST(FrNm_HsmActionIdxType, FRNM_CONST)
  FrNm_HsmTrSynchronizeModeSteps3[1] =
{
  FRNM_HSM_FRNM_AIDX_BUSSLEEPMODE_ENTRY,
};

#define FRNM_STOP_SEC_CONST_8
#include <FrNm_MemMap.h>

/* ****** */

#define FRNM_START_SEC_CONST_UNSPECIFIED
#include <FrNm_MemMap.h>

/* transitions from state TOP */
STATIC CONST(FrNm_HsmTranType, FRNM_CONST)
 FrNm_HsmTrTOP[1] =
{
{
  FrNm_HsmTrTOPSteps1,
  sizeof(FrNm_HsmTrTOPSteps1)/sizeof(FrNm_HsmActionIdxType),
  FRNM_HSM_FRNM_EV_RX_INDICATION, /* triggering event */
  FRNM_HSM_GIDX_INVALID, /* no guard */
  FRNM_HSM_SIDX_INVALID /* no target state, internal transition */
},
};
/* transitions from state BusSleepMode */
STATIC CONST(FrNm_HsmTranType, FRNM_CONST)
 FrNm_HsmTrBusSleepMode[2] =
{
{
  FrNm_HsmTrBusSleepModeSteps1,
  sizeof(FrNm_HsmTrBusSleepModeSteps1)/sizeof(FrNm_HsmActionIdxType),
  FRNM_HSM_FRNM_EV_VOTE_RECEIVED, /* triggering event */
  FRNM_HSM_GIDX_INVALID, /* no guard */
  FRNM_HSM_SIDX_INVALID /* no target state, internal transition */
},
{
  FrNm_HsmTrBusSleepModeSteps2,
  sizeof(FrNm_HsmTrBusSleepModeSteps2)/sizeof(FrNm_HsmActionIdxType),
  FRNM_HSM_FRNM_EV_NETWORK_START, /* triggering event */
  FRNM_HSM_GIDX_INVALID, /* no guard */
  FRNM_HSM_FRNM_SIDX_SYNCHRONIZEMODE /* target state */
},
};
/* transitions from state NetworkMode */
STATIC CONST(FrNm_HsmTranType, FRNM_CONST)
 FrNm_HsmTrNetworkMode[3] =
{
{
  FrNm_HsmTrNetworkModeSteps1,
  sizeof(FrNm_HsmTrNetworkModeSteps1)/sizeof(FrNm_HsmActionIdxType),
  FRNM_HSM_FRNM_EV_STARTUP_ERROR, /* triggering event */
  FRNM_HSM_GIDX_INVALID, /* no guard */
  FRNM_HSM_SIDX_INVALID /* no target state, internal transition */
},
{
  FrNm_HsmTrNetworkModeSteps2,
  sizeof(FrNm_HsmTrNetworkModeSteps2)/sizeof(FrNm_HsmActionIdxType),
  FRNM_HSM_FRNM_EV_FAILSAFE_SLEEP_MODE, /* triggering event */
  FRNM_HSM_GIDX_INVALID, /* no guard */
  FRNM_HSM_FRNM_SIDX_BUSSLEEPMODE /* target state */
},
{
  FrNm_HsmTrNetworkModeSteps3,
  sizeof(FrNm_HsmTrNetworkModeSteps3)/sizeof(FrNm_HsmActionIdxType),
  FRNM_HSM_FRNM_EV_FAILSAFE_SYNCH_MODE, /* triggering event */
  FRNM_HSM_GIDX_INVALID, /* no guard */
  FRNM_HSM_FRNM_SIDX_SYNCHRONIZEMODE /* target state */
},
};
/* transitions from state ReadySleepState */
STATIC CONST(FrNm_HsmTranType, FRNM_CONST)
 FrNm_HsmTrReadySleepState[1] =
{
{
  FrNm_HsmTrReadySleepStateSteps1,
  sizeof(FrNm_HsmTrReadySleepStateSteps1)/sizeof(FrNm_HsmActionIdxType),
  FRNM_HSM_FRNM_EV_REP_CYCLE_COMPLETED, /* triggering event */
  FRNM_HSM_FRNM_GIDX_READYSLEEPSTATE_GUARD_1, /* guard idx */
  FRNM_HSM_FRNM_SIDX_BUSSLEEPMODE /* target state */
},
};
/* transitions from state ReadySleepRemoteActivity */
STATIC CONST(FrNm_HsmTranType, FRNM_CONST)
 FrNm_HsmTrReadySleepRemoteActivity[3] =
{
{
  FrNm_HsmTrReadySleepRemoteActivitySteps1,
  sizeof(FrNm_HsmTrReadySleepRemoteActivitySteps1)/sizeof(FrNm_HsmActionIdxType),
  FRNM_HSM_FRNM_EV_VOTE_RECEIVED, /* triggering event */
  FRNM_HSM_GIDX_INVALID, /* no guard */
  FRNM_HSM_SIDX_INVALID /* no target state, internal transition */
},
{
  FrNm_HsmTrReadySleepRemoteActivitySteps2,
  sizeof(FrNm_HsmTrReadySleepRemoteActivitySteps2)/sizeof(FrNm_HsmActionIdxType),
  FRNM_HSM_FRNM_EV_REP_CYCLE_COMPLETED, /* triggering event */
  FRNM_HSM_FRNM_GIDX_READYSLEEPREMOTEACTIVITY_GUARD_2, /* guard idx */
  FRNM_HSM_FRNM_SIDX_REPEATMESSAGESTATE /* target state */
},
{
  FrNm_HsmTrReadySleepRemoteActivitySteps3,
  sizeof(FrNm_HsmTrReadySleepRemoteActivitySteps3)/sizeof(FrNm_HsmActionIdxType),
  FRNM_HSM_FRNM_EV_REP_CYCLE_COMPLETED, /* triggering event */
  FRNM_HSM_FRNM_GIDX_READYSLEEPREMOTEACTIVITY_GUARD_3, /* guard idx */
  FRNM_HSM_FRNM_SIDX_NORMALOPERATIONREMOTEACTIVITY /* target state */
},
};
/* transitions from state ReadySleepRemoteSleep */
STATIC CONST(FrNm_HsmTranType, FRNM_CONST)
 FrNm_HsmTrReadySleepRemoteSleep[3] =
{
{
  FrNm_HsmTrReadySleepRemoteSleepSteps1,
  sizeof(FrNm_HsmTrReadySleepRemoteSleepSteps1)/sizeof(FrNm_HsmActionIdxType),
  FRNM_HSM_FRNM_EV_REP_CYCLE_COMPLETED, /* triggering event */
  FRNM_HSM_FRNM_GIDX_READYSLEEPREMOTESLEEP_GUARD_1, /* guard idx */
  FRNM_HSM_FRNM_SIDX_REPEATMESSAGESTATE /* target state */
},
{
  FrNm_HsmTrReadySleepRemoteSleepSteps2,
  sizeof(FrNm_HsmTrReadySleepRemoteSleepSteps2)/sizeof(FrNm_HsmActionIdxType),
  FRNM_HSM_FRNM_EV_REP_CYCLE_COMPLETED, /* triggering event */
  FRNM_HSM_FRNM_GIDX_READYSLEEPREMOTESLEEP_GUARD_2, /* guard idx */
  FRNM_HSM_FRNM_SIDX_NORMALOPERATIONREMOTESLEEP /* target state */
},
{
  FrNm_HsmTrReadySleepRemoteSleepSteps3,
  sizeof(FrNm_HsmTrReadySleepRemoteSleepSteps3)/sizeof(FrNm_HsmActionIdxType),
  FRNM_HSM_FRNM_EV_VOTE_RECEIVED, /* triggering event */
  FRNM_HSM_GIDX_INVALID, /* no guard */
  FRNM_HSM_FRNM_SIDX_READYSLEEPREMOTEACTIVITY /* target state */
},
};
/* transitions from state SendingSubMode */
STATIC CONST(FrNm_HsmTranType, FRNM_CONST)
 FrNm_HsmTrSendingSubMode[4] =
{
{
  FrNm_HsmTrSendingSubModeSteps1,
  sizeof(FrNm_HsmTrSendingSubModeSteps1)/sizeof(FrNm_HsmActionIdxType),
  FRNM_HSM_FRNM_EV_DATA_CYCLE_TIMESLOT, /* triggering event */
  FRNM_HSM_GIDX_INVALID, /* no guard */
  FRNM_HSM_SIDX_INVALID /* no target state, internal transition */
},
{
  FrNm_HsmTrSendingSubModeSteps2,
  sizeof(FrNm_HsmTrSendingSubModeSteps2)/sizeof(FrNm_HsmActionIdxType),
  FRNM_HSM_FRNM_EV_VOTE_CYCLE_TIMESLOT, /* triggering event */
  FRNM_HSM_GIDX_INVALID, /* no guard */
  FRNM_HSM_SIDX_INVALID /* no target state, internal transition */
},
{
  FrNm_HsmTrSendingSubModeSteps3,
  sizeof(FrNm_HsmTrSendingSubModeSteps3)/sizeof(FrNm_HsmActionIdxType),
  FRNM_HSM_FRNM_EV_TX_CONFIRMATION, /* triggering event */
  FRNM_HSM_GIDX_INVALID, /* no guard */
  FRNM_HSM_SIDX_INVALID /* no target state, internal transition */
},
{
  FrNm_HsmTrSendingSubModeSteps4,
  sizeof(FrNm_HsmTrSendingSubModeSteps4)/sizeof(FrNm_HsmActionIdxType),
  FRNM_HSM_FRNM_EV_TX_TIMEOUT, /* triggering event */
  FRNM_HSM_GIDX_INVALID, /* no guard */
  FRNM_HSM_SIDX_INVALID /* no target state, internal transition */
},
};
/* transitions from state NormalOperationRemoteActivity */
STATIC CONST(FrNm_HsmTranType, FRNM_CONST)
 FrNm_HsmTrNormalOperationRemoteActivity[4] =
{
{
  FrNm_HsmTrNormalOperationRemoteActivitySteps1,
  sizeof(FrNm_HsmTrNormalOperationRemoteActivitySteps1)/sizeof(FrNm_HsmActionIdxType),
  FRNM_HSM_FRNM_EV_VOTE_RECEIVED, /* triggering event */
  FRNM_HSM_GIDX_INVALID, /* no guard */
  FRNM_HSM_SIDX_INVALID /* no target state, internal transition */
},
{
  FrNm_HsmTrNormalOperationRemoteActivitySteps2,
  sizeof(FrNm_HsmTrNormalOperationRemoteActivitySteps2)/sizeof(FrNm_HsmActionIdxType),
  FRNM_HSM_FRNM_EV_REP_CYCLE_COMPLETED, /* triggering event */
  FRNM_HSM_FRNM_GIDX_NORMALOPERATIONREMOTEACTIVITY_GUARD_2, /* guard idx */
  FRNM_HSM_FRNM_SIDX_REPEATMESSAGESTATE /* target state */
},
{
  FrNm_HsmTrNormalOperationRemoteActivitySteps3,
  sizeof(FrNm_HsmTrNormalOperationRemoteActivitySteps3)/sizeof(FrNm_HsmActionIdxType),
  FRNM_HSM_FRNM_EV_REP_CYCLE_COMPLETED, /* triggering event */
  FRNM_HSM_FRNM_GIDX_NORMALOPERATIONREMOTEACTIVITY_GUARD_3, /* guard idx */
  FRNM_HSM_FRNM_SIDX_NORMALOPERATIONREMOTESLEEP /* target state */
},
{
  FrNm_HsmTrNormalOperationRemoteActivitySteps4,
  sizeof(FrNm_HsmTrNormalOperationRemoteActivitySteps4)/sizeof(FrNm_HsmActionIdxType),
  FRNM_HSM_FRNM_EV_REP_CYCLE_COMPLETED, /* triggering event */
  FRNM_HSM_FRNM_GIDX_NORMALOPERATIONREMOTEACTIVITY_GUARD_4, /* guard idx */
  FRNM_HSM_FRNM_SIDX_READYSLEEPREMOTEACTIVITY /* target state */
},
};
/* transitions from state NormalOperationRemoteSleep */
STATIC CONST(FrNm_HsmTranType, FRNM_CONST)
 FrNm_HsmTrNormalOperationRemoteSleep[3] =
{
{
  FrNm_HsmTrNormalOperationRemoteSleepSteps1,
  sizeof(FrNm_HsmTrNormalOperationRemoteSleepSteps1)/sizeof(FrNm_HsmActionIdxType),
  FRNM_HSM_FRNM_EV_REP_CYCLE_COMPLETED, /* triggering event */
  FRNM_HSM_FRNM_GIDX_NORMALOPERATIONREMOTESLEEP_GUARD_1, /* guard idx */
  FRNM_HSM_FRNM_SIDX_REPEATMESSAGESTATE /* target state */
},
{
  FrNm_HsmTrNormalOperationRemoteSleepSteps2,
  sizeof(FrNm_HsmTrNormalOperationRemoteSleepSteps2)/sizeof(FrNm_HsmActionIdxType),
  FRNM_HSM_FRNM_EV_REP_CYCLE_COMPLETED, /* triggering event */
  FRNM_HSM_FRNM_GIDX_NORMALOPERATIONREMOTESLEEP_GUARD_2, /* guard idx */
  FRNM_HSM_FRNM_SIDX_READYSLEEPREMOTESLEEP /* target state */
},
{
  FrNm_HsmTrNormalOperationRemoteSleepSteps3,
  sizeof(FrNm_HsmTrNormalOperationRemoteSleepSteps3)/sizeof(FrNm_HsmActionIdxType),
  FRNM_HSM_FRNM_EV_VOTE_RECEIVED, /* triggering event */
  FRNM_HSM_GIDX_INVALID, /* no guard */
  FRNM_HSM_FRNM_SIDX_NORMALOPERATIONREMOTEACTIVITY /* target state */
},
};
/* transitions from state RepeatMessageState */
STATIC CONST(FrNm_HsmTranType, FRNM_CONST)
 FrNm_HsmTrRepeatMessageState[3] =
{
{
  FrNm_HsmTrRepeatMessageStateSteps1,
  sizeof(FrNm_HsmTrRepeatMessageStateSteps1)/sizeof(FrNm_HsmActionIdxType),
  FRNM_HSM_FRNM_EV_UNI_TIMEOUT, /* triggering event */
  FRNM_HSM_GIDX_INVALID, /* no guard */
  FRNM_HSM_SIDX_INVALID /* no target state, internal transition */
},
{
  FrNm_HsmTrRepeatMessageStateSteps2,
  sizeof(FrNm_HsmTrRepeatMessageStateSteps2)/sizeof(FrNm_HsmActionIdxType),
  FRNM_HSM_FRNM_EV_REP_CYCLE_COMPLETED, /* triggering event */
  FRNM_HSM_FRNM_GIDX_REPEATMESSAGESTATE_GUARD_2, /* guard idx */
  FRNM_HSM_FRNM_SIDX_NORMALOPERATIONSTATE /* target state */
},
{
  FrNm_HsmTrRepeatMessageStateSteps3,
  sizeof(FrNm_HsmTrRepeatMessageStateSteps3)/sizeof(FrNm_HsmActionIdxType),
  FRNM_HSM_FRNM_EV_REP_CYCLE_COMPLETED, /* triggering event */
  FRNM_HSM_FRNM_GIDX_REPEATMESSAGESTATE_GUARD_3, /* guard idx */
  FRNM_HSM_FRNM_SIDX_READYSLEEPSTATE /* target state */
},
};
/* transitions from state SynchronizeMode */
STATIC CONST(FrNm_HsmTranType, FRNM_CONST)
 FrNm_HsmTrSynchronizeMode[3] =
{
{
  FrNm_HsmTrSynchronizeModeSteps1,
  sizeof(FrNm_HsmTrSynchronizeModeSteps1)/sizeof(FrNm_HsmActionIdxType),
  FRNM_HSM_FRNM_EV_FAILSAFE_SLEEP_MODE, /* triggering event */
  FRNM_HSM_GIDX_INVALID, /* no guard */
  FRNM_HSM_FRNM_SIDX_BUSSLEEPMODE /* target state */
},
{
  FrNm_HsmTrSynchronizeModeSteps2,
  sizeof(FrNm_HsmTrSynchronizeModeSteps2)/sizeof(FrNm_HsmActionIdxType),
  FRNM_HSM_FRNM_EV_REP_CYCLE_COMPLETED, /* triggering event */
  FRNM_HSM_FRNM_GIDX_SYNCHRONIZEMODE_GUARD_2, /* guard idx */
  FRNM_HSM_FRNM_SIDX_NETWORKMODE /* target state */
},
{
  FrNm_HsmTrSynchronizeModeSteps3,
  sizeof(FrNm_HsmTrSynchronizeModeSteps3)/sizeof(FrNm_HsmActionIdxType),
  FRNM_HSM_FRNM_EV_STARTUP_ERROR, /* triggering event */
  FRNM_HSM_FRNM_GIDX_SYNCHRONIZEMODE_GUARD_3, /* guard idx */
  FRNM_HSM_FRNM_SIDX_BUSSLEEPMODE /* target state */
},
};



/* statechart model description of the FrNm state
 * machine */
STATIC CONST(FrNm_HsmStateType, FRNM_CONST)
  FrNm_HsmScdFrNm[FRNM_HSM_FRNM_NO_OF_STATES] =
{
  {
    /* description of state TOP */
    0U, /* no non-deferred event mask (no leaf state) */
    0U, /* no ignored event mask (no leaf state) */
    FrNm_HsmTrTOP, /* transitions for this state */
    FRNM_HSM_SIDX_INVALID, /* no parent state */
    FRNM_HSM_FRNM_SIDX_BUSSLEEPMODE, /* init state */
      FRNM_HSM_FRNM_AIDX_TOP_ENTRY, /* entry function */
    FRNM_HSM_AIDX_INVALID, /* no exit action */
    FRNM_HSM_AIDX_INVALID, /* no do action */
    1 /* num of transitions */
  },
  {
    /* description of state BusSleepMode */
    /* non-deferred and non-ignored event mask */
   (1UL << FRNM_HSM_FRNM_EV_RX_INDICATION)
    |   (1UL << FRNM_HSM_FRNM_EV_NETWORK_START)
    |   (1UL << FRNM_HSM_FRNM_EV_VOTE_RECEIVED)
    | 0UL
    ,
      /* ignored event mask */
   (1UL << FRNM_HSM_FRNM_EV_FAILSAFE_SYNCH_MODE)
    |   (1UL << FRNM_HSM_FRNM_EV_DATA_CYCLE_TIMESLOT)
    |   (1UL << FRNM_HSM_FRNM_EV_VOTE_CYCLE_TIMESLOT)
    |   (1UL << FRNM_HSM_FRNM_EV_TX_CONFIRMATION)
    |   (1UL << FRNM_HSM_FRNM_EV_TX_TIMEOUT)
    |   (1UL << FRNM_HSM_FRNM_EV_UNI_TIMEOUT)
    |   (1UL << FRNM_HSM_FRNM_EV_FAILSAFE_SLEEP_MODE)
    |   (1UL << FRNM_HSM_FRNM_EV_REP_CYCLE_COMPLETED)
    |   (1UL << FRNM_HSM_FRNM_EV_STARTUP_ERROR)
    | 0UL
    ,
    FrNm_HsmTrBusSleepMode, /* transitions for this state */
    FRNM_HSM_FRNM_SIDX_TOP, /* parent state */
    FRNM_HSM_SIDX_INVALID, /* no init state */
      FRNM_HSM_FRNM_AIDX_BUSSLEEPMODE_ENTRY, /* entry function */
    FRNM_HSM_AIDX_INVALID, /* no exit action */
    FRNM_HSM_AIDX_INVALID, /* no do action */
    2 /* num of transitions */
  },
  {
    /* description of state NetworkMode */
    0U, /* no non-deferred event mask (no leaf state) */
    0U, /* no ignored event mask (no leaf state) */
    FrNm_HsmTrNetworkMode, /* transitions for this state */
    FRNM_HSM_FRNM_SIDX_TOP, /* parent state */
    FRNM_HSM_FRNM_SIDX_SENDINGSUBMODE, /* init state */
      FRNM_HSM_FRNM_AIDX_NETWORKMODE_ENTRY, /* entry function */
    FRNM_HSM_AIDX_INVALID, /* no exit action */
    FRNM_HSM_AIDX_INVALID, /* no do action */
    3 /* num of transitions */
  },
  {
    /* description of state ReadySleepState */
    0U, /* no non-deferred event mask (no leaf state) */
    0U, /* no ignored event mask (no leaf state) */
    FrNm_HsmTrReadySleepState, /* transitions for this state */
    FRNM_HSM_FRNM_SIDX_NETWORKMODE, /* parent state */
    FRNM_HSM_FRNM_SIDX_READYSLEEPREMOTEACTIVITY, /* init state */
      FRNM_HSM_FRNM_AIDX_READYSLEEPSTATE_ENTRY, /* entry function */
    FRNM_HSM_FRNM_AIDX_READYSLEEPSTATE_EXIT, /* exit function */
    FRNM_HSM_AIDX_INVALID, /* no do action */
    1 /* num of transitions */
  },
  {
    /* description of state ReadySleepRemoteActivity */
    /* non-deferred and non-ignored event mask */
   (1UL << FRNM_HSM_FRNM_EV_RX_INDICATION)
    |   (1UL << FRNM_HSM_FRNM_EV_FAILSAFE_SYNCH_MODE)
    |   (1UL << FRNM_HSM_FRNM_EV_VOTE_RECEIVED)
    |   (1UL << FRNM_HSM_FRNM_EV_FAILSAFE_SLEEP_MODE)
    |   (1UL << FRNM_HSM_FRNM_EV_REP_CYCLE_COMPLETED)
    |   (1UL << FRNM_HSM_FRNM_EV_STARTUP_ERROR)
    | 0UL
    ,
      /* ignored event mask */
   (1UL << FRNM_HSM_FRNM_EV_NETWORK_START)
    |   (1UL << FRNM_HSM_FRNM_EV_DATA_CYCLE_TIMESLOT)
    |   (1UL << FRNM_HSM_FRNM_EV_VOTE_CYCLE_TIMESLOT)
    |   (1UL << FRNM_HSM_FRNM_EV_TX_CONFIRMATION)
    |   (1UL << FRNM_HSM_FRNM_EV_TX_TIMEOUT)
    |   (1UL << FRNM_HSM_FRNM_EV_UNI_TIMEOUT)
    | 0UL
    ,
    FrNm_HsmTrReadySleepRemoteActivity, /* transitions for this state */
    FRNM_HSM_FRNM_SIDX_READYSLEEPSTATE, /* parent state */
    FRNM_HSM_SIDX_INVALID, /* no init state */
      FRNM_HSM_FRNM_AIDX_READYSLEEPREMOTEACTIVITY_ENTRY, /* entry function */
    FRNM_HSM_AIDX_INVALID, /* no exit action */
    FRNM_HSM_AIDX_INVALID, /* no do action */
    3 /* num of transitions */
  },
  {
    /* description of state ReadySleepRemoteSleep */
    /* non-deferred and non-ignored event mask */
   (1UL << FRNM_HSM_FRNM_EV_RX_INDICATION)
    |   (1UL << FRNM_HSM_FRNM_EV_FAILSAFE_SYNCH_MODE)
    |   (1UL << FRNM_HSM_FRNM_EV_VOTE_RECEIVED)
    |   (1UL << FRNM_HSM_FRNM_EV_FAILSAFE_SLEEP_MODE)
    |   (1UL << FRNM_HSM_FRNM_EV_REP_CYCLE_COMPLETED)
    |   (1UL << FRNM_HSM_FRNM_EV_STARTUP_ERROR)
    | 0UL
    ,
      /* ignored event mask */
   (1UL << FRNM_HSM_FRNM_EV_NETWORK_START)
    |   (1UL << FRNM_HSM_FRNM_EV_DATA_CYCLE_TIMESLOT)
    |   (1UL << FRNM_HSM_FRNM_EV_VOTE_CYCLE_TIMESLOT)
    |   (1UL << FRNM_HSM_FRNM_EV_TX_CONFIRMATION)
    |   (1UL << FRNM_HSM_FRNM_EV_TX_TIMEOUT)
    |   (1UL << FRNM_HSM_FRNM_EV_UNI_TIMEOUT)
    | 0UL
    ,
    FrNm_HsmTrReadySleepRemoteSleep, /* transitions for this state */
    FRNM_HSM_FRNM_SIDX_READYSLEEPSTATE, /* parent state */
    FRNM_HSM_SIDX_INVALID, /* no init state */
      FRNM_HSM_FRNM_AIDX_READYSLEEPREMOTESLEEP_ENTRY, /* entry function */
    FRNM_HSM_AIDX_INVALID, /* no exit action */
    FRNM_HSM_AIDX_INVALID, /* no do action */
    3 /* num of transitions */
  },
  {
    /* description of state SendingSubMode */
    0U, /* no non-deferred event mask (no leaf state) */
    0U, /* no ignored event mask (no leaf state) */
    FrNm_HsmTrSendingSubMode, /* transitions for this state */
    FRNM_HSM_FRNM_SIDX_NETWORKMODE, /* parent state */
    FRNM_HSM_FRNM_SIDX_REPEATMESSAGESTATE, /* init state */
      FRNM_HSM_FRNM_AIDX_SENDINGSUBMODE_ENTRY, /* entry function */
    FRNM_HSM_FRNM_AIDX_SENDINGSUBMODE_EXIT, /* exit function */
    FRNM_HSM_AIDX_INVALID, /* no do action */
    4 /* num of transitions */
  },
  {
    /* description of state NormalOperationState */
    0U, /* no non-deferred event mask (no leaf state) */
    0U, /* no ignored event mask (no leaf state) */
    NULL_PTR,  /* no transitions */
    FRNM_HSM_FRNM_SIDX_SENDINGSUBMODE, /* parent state */
    FRNM_HSM_FRNM_SIDX_NORMALOPERATIONREMOTEACTIVITY, /* init state */
      FRNM_HSM_FRNM_AIDX_NORMALOPERATIONSTATE_ENTRY, /* entry function */
    FRNM_HSM_AIDX_INVALID, /* no exit action */
    FRNM_HSM_AIDX_INVALID, /* no do action */
    0U  /* no transitions */
  },
  {
    /* description of state NormalOperationRemoteActivity */
    /* non-deferred and non-ignored event mask */
   (1UL << FRNM_HSM_FRNM_EV_RX_INDICATION)
    |   (1UL << FRNM_HSM_FRNM_EV_FAILSAFE_SYNCH_MODE)
    |   (1UL << FRNM_HSM_FRNM_EV_DATA_CYCLE_TIMESLOT)
    |   (1UL << FRNM_HSM_FRNM_EV_VOTE_CYCLE_TIMESLOT)
    |   (1UL << FRNM_HSM_FRNM_EV_TX_CONFIRMATION)
    |   (1UL << FRNM_HSM_FRNM_EV_TX_TIMEOUT)
    |   (1UL << FRNM_HSM_FRNM_EV_VOTE_RECEIVED)
    |   (1UL << FRNM_HSM_FRNM_EV_FAILSAFE_SLEEP_MODE)
    |   (1UL << FRNM_HSM_FRNM_EV_REP_CYCLE_COMPLETED)
    |   (1UL << FRNM_HSM_FRNM_EV_STARTUP_ERROR)
    | 0UL
    ,
      /* ignored event mask */
   (1UL << FRNM_HSM_FRNM_EV_NETWORK_START)
    |   (1UL << FRNM_HSM_FRNM_EV_UNI_TIMEOUT)
    | 0UL
    ,
    FrNm_HsmTrNormalOperationRemoteActivity, /* transitions for this state */
    FRNM_HSM_FRNM_SIDX_NORMALOPERATIONSTATE, /* parent state */
    FRNM_HSM_SIDX_INVALID, /* no init state */
      FRNM_HSM_FRNM_AIDX_NORMALOPERATIONREMOTEACTIVITY_ENTRY, /* entry function */
    FRNM_HSM_AIDX_INVALID, /* no exit action */
    FRNM_HSM_AIDX_INVALID, /* no do action */
    4 /* num of transitions */
  },
  {
    /* description of state NormalOperationRemoteSleep */
    /* non-deferred and non-ignored event mask */
   (1UL << FRNM_HSM_FRNM_EV_RX_INDICATION)
    |   (1UL << FRNM_HSM_FRNM_EV_FAILSAFE_SYNCH_MODE)
    |   (1UL << FRNM_HSM_FRNM_EV_DATA_CYCLE_TIMESLOT)
    |   (1UL << FRNM_HSM_FRNM_EV_VOTE_CYCLE_TIMESLOT)
    |   (1UL << FRNM_HSM_FRNM_EV_TX_CONFIRMATION)
    |   (1UL << FRNM_HSM_FRNM_EV_TX_TIMEOUT)
    |   (1UL << FRNM_HSM_FRNM_EV_VOTE_RECEIVED)
    |   (1UL << FRNM_HSM_FRNM_EV_FAILSAFE_SLEEP_MODE)
    |   (1UL << FRNM_HSM_FRNM_EV_REP_CYCLE_COMPLETED)
    |   (1UL << FRNM_HSM_FRNM_EV_STARTUP_ERROR)
    | 0UL
    ,
      /* ignored event mask */
   (1UL << FRNM_HSM_FRNM_EV_NETWORK_START)
    |   (1UL << FRNM_HSM_FRNM_EV_UNI_TIMEOUT)
    | 0UL
    ,
    FrNm_HsmTrNormalOperationRemoteSleep, /* transitions for this state */
    FRNM_HSM_FRNM_SIDX_NORMALOPERATIONSTATE, /* parent state */
    FRNM_HSM_SIDX_INVALID, /* no init state */
      FRNM_HSM_FRNM_AIDX_NORMALOPERATIONREMOTESLEEP_ENTRY, /* entry function */
    FRNM_HSM_AIDX_INVALID, /* no exit action */
    FRNM_HSM_AIDX_INVALID, /* no do action */
    3 /* num of transitions */
  },
  {
    /* description of state RepeatMessageState */
    /* non-deferred and non-ignored event mask */
   (1UL << FRNM_HSM_FRNM_EV_RX_INDICATION)
    |   (1UL << FRNM_HSM_FRNM_EV_FAILSAFE_SYNCH_MODE)
    |   (1UL << FRNM_HSM_FRNM_EV_DATA_CYCLE_TIMESLOT)
    |   (1UL << FRNM_HSM_FRNM_EV_VOTE_CYCLE_TIMESLOT)
    |   (1UL << FRNM_HSM_FRNM_EV_TX_CONFIRMATION)
    |   (1UL << FRNM_HSM_FRNM_EV_TX_TIMEOUT)
    |   (1UL << FRNM_HSM_FRNM_EV_UNI_TIMEOUT)
    |   (1UL << FRNM_HSM_FRNM_EV_FAILSAFE_SLEEP_MODE)
    |   (1UL << FRNM_HSM_FRNM_EV_REP_CYCLE_COMPLETED)
    |   (1UL << FRNM_HSM_FRNM_EV_STARTUP_ERROR)
    | 0UL
    ,
      /* ignored event mask */
   (1UL << FRNM_HSM_FRNM_EV_NETWORK_START)
    |   (1UL << FRNM_HSM_FRNM_EV_VOTE_RECEIVED)
    | 0UL
    ,
    FrNm_HsmTrRepeatMessageState, /* transitions for this state */
    FRNM_HSM_FRNM_SIDX_SENDINGSUBMODE, /* parent state */
    FRNM_HSM_SIDX_INVALID, /* no init state */
      FRNM_HSM_FRNM_AIDX_REPEATMESSAGESTATE_ENTRY, /* entry function */
    FRNM_HSM_FRNM_AIDX_REPEATMESSAGESTATE_EXIT, /* exit function */
    FRNM_HSM_AIDX_INVALID, /* no do action */
    3 /* num of transitions */
  },
  {
    /* description of state SynchronizeMode */
    /* non-deferred and non-ignored event mask */
   (1UL << FRNM_HSM_FRNM_EV_RX_INDICATION)
    |   (1UL << FRNM_HSM_FRNM_EV_FAILSAFE_SLEEP_MODE)
    |   (1UL << FRNM_HSM_FRNM_EV_REP_CYCLE_COMPLETED)
    |   (1UL << FRNM_HSM_FRNM_EV_STARTUP_ERROR)
    | 0UL
    ,
      /* ignored event mask */
   (1UL << FRNM_HSM_FRNM_EV_NETWORK_START)
    |   (1UL << FRNM_HSM_FRNM_EV_FAILSAFE_SYNCH_MODE)
    |   (1UL << FRNM_HSM_FRNM_EV_DATA_CYCLE_TIMESLOT)
    |   (1UL << FRNM_HSM_FRNM_EV_VOTE_CYCLE_TIMESLOT)
    |   (1UL << FRNM_HSM_FRNM_EV_TX_CONFIRMATION)
    |   (1UL << FRNM_HSM_FRNM_EV_TX_TIMEOUT)
    |   (1UL << FRNM_HSM_FRNM_EV_VOTE_RECEIVED)
    |   (1UL << FRNM_HSM_FRNM_EV_UNI_TIMEOUT)
    | 0UL
    ,
    FrNm_HsmTrSynchronizeMode, /* transitions for this state */
    FRNM_HSM_FRNM_SIDX_TOP, /* parent state */
    FRNM_HSM_SIDX_INVALID, /* no init state */
      FRNM_HSM_FRNM_AIDX_SYNCHRONIZEMODE_ENTRY, /* entry function */
    FRNM_HSM_AIDX_INVALID, /* no exit action */
    FRNM_HSM_AIDX_INVALID, /* no do action */
    3 /* num of transitions */
  }
};

#if (FRNM_HSM_TRACE == STD_ON)
STATIC CONSTP2CONST(char, FRNM_CONST, FRNM_CONST)
  FrNm_HsmFrNmStateNames[] =
{
  "TOP",
  "BusSleepMode",
  "NetworkMode",
  "ReadySleepState",
  "ReadySleepRemoteActivity",
  "ReadySleepRemoteSleep",
  "SendingSubMode",
  "NormalOperationState",
  "NormalOperationRemoteActivity",
  "NormalOperationRemoteSleep",
  "RepeatMessageState",
  "SynchronizeMode",
};

STATIC CONSTP2CONST(char, FRNM_CONST, FRNM_CONST)
  FrNm_HsmFrNmActionNames[] =
{
  "TOP entry",
  "TOP action 1",
  "BusSleepMode entry",
  "BusSleepMode action 1",
  "BusSleepMode action 2",
  "NetworkMode entry",
  "NetworkMode action 1",
  "NetworkMode action 2",
  "NetworkMode action 3",
  "ReadySleepState entry",
  "ReadySleepState exit",
  "ReadySleepState action 1",
  "ReadySleepRemoteActivity entry",
  "ReadySleepRemoteActivity action 1",
  "ReadySleepRemoteSleep entry",
  "ReadySleepRemoteSleep action 1",
  "ReadySleepRemoteSleep action 3",
  "SendingSubMode entry",
  "SendingSubMode exit",
  "SendingSubMode action 1",
  "SendingSubMode action 2",
  "SendingSubMode action 3",
  "SendingSubMode action 4",
  "NormalOperationState entry",
  "NormalOperationRemoteActivity entry",
  "NormalOperationRemoteActivity action 1",
  "NormalOperationRemoteSleep entry",
  "NormalOperationRemoteSleep action 1",
  "NormalOperationRemoteSleep action 3",
  "RepeatMessageState entry",
  "RepeatMessageState exit",
  "RepeatMessageState action 1",
  "SynchronizeMode entry",
  "SynchronizeMode action 1",
};

STATIC CONSTP2CONST(char, FRNM_CONST, FRNM_CONST)
  FrNm_HsmFrNmGuardNames[] =
{
  "ReadySleepState guard 1",
  "ReadySleepRemoteActivity guard 2",
  "ReadySleepRemoteActivity guard 3",
  "ReadySleepRemoteSleep guard 1",
  "ReadySleepRemoteSleep guard 2",
  "NormalOperationRemoteActivity guard 2",
  "NormalOperationRemoteActivity guard 3",
  "NormalOperationRemoteActivity guard 4",
  "NormalOperationRemoteSleep guard 1",
  "NormalOperationRemoteSleep guard 2",
  "RepeatMessageState guard 2",
  "RepeatMessageState guard 3",
  "SynchronizeMode guard 2",
  "SynchronizeMode guard 3",
};

STATIC CONSTP2CONST(char, FRNM_CONST, FRNM_CONST)
  FrNm_HsmFrNmEventNames[] =
{
  "RX_INDICATION",
  "NETWORK_START",
  "FAILSAFE_SYNCH_MODE",
  "DATA_CYCLE_TIMESLOT",
  "VOTE_CYCLE_TIMESLOT",
  "TX_CONFIRMATION",
  "TX_TIMEOUT",
  "VOTE_RECEIVED",
  "UNI_TIMEOUT",
  "FAILSAFE_SLEEP_MODE",
  "REP_CYCLE_COMPLETED",
  "STARTUP_ERROR",
};
#endif

/*==================[external constants]====================================*/

/* Top level state chart description data */
CONST(FrNm_HsmStatechartType, FRNM_CONST)
  FrNm_HsmScFrNm =
{
  FrNm_HsmScdFrNm, /* state description array */
  FrNm_HsmInstFrNm, /* instances RAM */
  FrNm_HsmFrNmActions, /* actions */
  FrNm_HsmFrNmGuards, /* guards */
  FrNm_HsmEvCountersFrNm, /* event counters RAM */
  FrNm_HsmEvQueuesFrNm, /* event queues RAM */
#if (FRNM_HSM_TRACE == STD_ON)
  "FrNm", /* name of the HSM */
  FrNm_HsmFrNmStateNames,
  FrNm_HsmFrNmActionNames,
  FrNm_HsmFrNmGuardNames,
  FrNm_HsmFrNmEventNames,
#endif
  FRNM_HSM_FRNM_NUM_EVENTS, /* num of events */
  0U, /* state chart id */
  FRNM_HSM_FRNM_NUM_INST, /* num of instances */
  FRNM_HSM_FRNM_SIDX_TOP /* top state index */
};


#define FRNM_STOP_SEC_CONST_UNSPECIFIED
#include <FrNm_MemMap.h>

/*==================[internal function definitions]=========================*/

/*==================[external function definitions]=========================*/

/*==================[end of file]===========================================*/
