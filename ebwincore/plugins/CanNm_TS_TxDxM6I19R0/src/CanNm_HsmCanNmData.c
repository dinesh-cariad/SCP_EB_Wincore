/**
 * \file
 *
 * \brief AUTOSAR CanNm
 *
 * This file contains the implementation of the AUTOSAR
 * module CanNm.
 *
 * \version 6.19.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* Definition of the configuration data of the state machine
 * CanNm.
 *
 * This file contains the definition of all used data structures of the state
 * machine model.  It is generated and must not be edited.
 */
/*==================[inclusions]============================================*/
#define CANNM_NO_CFGCLASSMIX_REQUIRED
#ifndef CANNM_NO_PBCFG_REQUIRED
#define CANNM_NO_PBCFG_REQUIRED
#endif

#include <CanNm_Hsm.h> /* state machine driver interface */
#include <CanNm_HsmCanNm.h> /* public statechart model definitions, */
#include <CanNm_HsmCanNmInt.h> /* internal statechart model definitions */

/*==================[macros]================================================*/

/** \brief Number of user events */
#if (defined CANNM_HSM_CANNM_NUM_EVENTS)
#error CANNM_HSM_CANNM_NUM_EVENTS already defined
#endif
#define CANNM_HSM_CANNM_NUM_EVENTS 11U

/* Check if number of hsm instances is defined in the user's configuration */
#ifndef CANNM_HSM_CANNM_NUM_INST
#error CANNM_HSM_CANNM_NUM_INST must be defined in CanNm_HsmCfg.h
#endif


/*==================[type definitions]======================================*/

/*==================[internal function declarations]========================*/

/*==================[external function declarations]========================*/

/*==================[internal data]=========================================*/

#define CANNM_START_SEC_VAR_CLEARED_8
#include <CanNm_MemMap.h>

/* RAM data for event counters */
STATIC VAR(uint8, CANNM_VAR_CLEARED)
  CanNm_HsmEvCountersCanNm[
    CANNM_HSM_CANNM_NUM_INST * CANNM_HSM_CANNM_NUM_EVENTS];

#define CANNM_STOP_SEC_VAR_CLEARED_8
#include <CanNm_MemMap.h>


#define CANNM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <CanNm_MemMap.h>

/* RAM data for instances */
STATIC VAR(CanNm_HsmInstType, CANNM_VAR_CLEARED)
  CanNm_HsmInstCanNm[CANNM_HSM_CANNM_NUM_INST];

/* RAM data for event queues */
STATIC VAR(CanNm_HsmEventType, CANNM_VAR_CLEARED)
  CanNm_HsmEvQueuesCanNm[
    CANNM_HSM_CANNM_NUM_INST * CANNM_HSM_CANNM_NUM_EVENTS];

#define CANNM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <CanNm_MemMap.h>

/*==================[external data]=========================================*/

/*==================[internal constants]====================================*/

#define CANNM_START_SEC_CONST_UNSPECIFIED
#include <CanNm_MemMap.h>

/* *** array of entry/exit/do and transition actions *** */
STATIC CONST(CanNm_HsmActionType, CANNM_CONST)
  CanNm_HsmCanNmActions[CANNM_HSM_CANNM_NO_OF_ACTIONS] =
{
  &CanNm_HsmCanNmSfTOPEntry,
  &CanNm_HsmCanNmSfBusSleepModeEntry,
  &CanNm_HsmCanNmSfBusSleepModeAction1,
  &CanNm_HsmCanNmSfNetworkModeEntry,
  &CanNm_HsmCanNmSfNetworkModeAction1,
  &CanNm_HsmCanNmSfNetworkModeAction2,
  &CanNm_HsmCanNmSfNetworkModeAction3,
  &CanNm_HsmCanNmSfReadySleepStateEntry,
  &CanNm_HsmCanNmSfReadySleepRemoteActivityEntry,
  &CanNm_HsmCanNmSfReadySleepRemoteActivityAction1,
  &CanNm_HsmCanNmSfReadySleepRemoteSleepAction1,
  &CanNm_HsmCanNmSfReadySleepRemoteSleepAction2,
  &CanNm_HsmCanNmSfReadySleepRemoteSleepAction3,
  &CanNm_HsmCanNmSfSendingSubModeExit,
  &CanNm_HsmCanNmSfSendingSubModeAction1,
  &CanNm_HsmCanNmSfSendingSubModeAction2,
  &CanNm_HsmCanNmSfSendingSubModeAction3,
  &CanNm_HsmCanNmSfNormalOperationStateEntry,
  &CanNm_HsmCanNmSfNormalOperationRemoteActivityEntry,
  &CanNm_HsmCanNmSfNormalOperationRemoteActivityAction1,
  &CanNm_HsmCanNmSfNormalOperationRemoteActivityAction4,
  &CanNm_HsmCanNmSfNormalOperationRemoteSleepEntry,
  &CanNm_HsmCanNmSfNormalOperationRemoteSleepAction2,
  &CanNm_HsmCanNmSfNormalOperationRemoteSleepAction3,
  &CanNm_HsmCanNmSfRepeatMessageStateEntry,
  &CanNm_HsmCanNmSfRepeatMessageStateExit,
  &CanNm_HsmCanNmSfPrepareBusSleepModeEntry,
  &CanNm_HsmCanNmSfPrepareBusSleepModeAction3,
};

/* *** array of guards *** */
STATIC CONST(CanNm_HsmGuardType, CANNM_CONST)
  CanNm_HsmCanNmGuards[CANNM_HSM_CANNM_NO_OF_GUARDS] =
{
  &CanNm_HsmCanNmSfBusSleepModeGuard3,
  &CanNm_HsmCanNmSfNetworkModeGuard4,
  &CanNm_HsmCanNmSfReadySleepStateGuard1,
  &CanNm_HsmCanNmSfReadySleepRemoteActivityGuard1,
  &CanNm_HsmCanNmSfReadySleepRemoteSleepGuard1,
  &CanNm_HsmCanNmSfNormalOperationRemoteActivityGuard2,
  &CanNm_HsmCanNmSfNormalOperationRemoteActivityGuard4,
  &CanNm_HsmCanNmSfNormalOperationRemoteSleepGuard1,
  &CanNm_HsmCanNmSfRepeatMessageStateGuard1,
  &CanNm_HsmCanNmSfRepeatMessageStateGuard2,
  &CanNm_HsmCanNmSfPrepareBusSleepModeGuard2,
};

#define CANNM_STOP_SEC_CONST_UNSPECIFIED
#include <CanNm_MemMap.h>

#define CANNM_START_SEC_CONST_8
#include <CanNm_MemMap.h>

/* *** exit/action/entry sequences for transitions *** */
/* trans sequence for transition #1 from state BusSleepMode */
STATIC CONST(CanNm_HsmActionIdxType, CANNM_CONST)
  CanNm_HsmTrBusSleepModeSteps1[1] =
{
  CANNM_HSM_CANNM_AIDX_BUSSLEEPMODE_ACTION_1,
};
/* trans sequence for transition #2 from state BusSleepMode */
STATIC CONST(CanNm_HsmActionIdxType, CANNM_CONST)
  CanNm_HsmTrBusSleepModeSteps2[1] =
{
  CANNM_HSM_CANNM_AIDX_NETWORKMODE_ENTRY,
};
/* trans sequence for transition #3 from state BusSleepMode */
STATIC CONST(CanNm_HsmActionIdxType, CANNM_CONST)
  CanNm_HsmTrBusSleepModeSteps3[1] =
{
  CANNM_HSM_CANNM_AIDX_NETWORKMODE_ENTRY,
};
/* trans sequence for transition #1 from state NetworkMode */
STATIC CONST(CanNm_HsmActionIdxType, CANNM_CONST)
  CanNm_HsmTrNetworkModeSteps1[1] =
{
  CANNM_HSM_CANNM_AIDX_NETWORKMODE_ACTION_1,
};
/* trans sequence for transition #2 from state NetworkMode */
STATIC CONST(CanNm_HsmActionIdxType, CANNM_CONST)
  CanNm_HsmTrNetworkModeSteps2[1] =
{
  CANNM_HSM_CANNM_AIDX_NETWORKMODE_ACTION_2,
};
/* trans sequence for transition #3 from state NetworkMode */
STATIC CONST(CanNm_HsmActionIdxType, CANNM_CONST)
  CanNm_HsmTrNetworkModeSteps3[1] =
{
  CANNM_HSM_CANNM_AIDX_NETWORKMODE_ACTION_3,
};
/* trans sequence for transition #4 from state NetworkMode */
STATIC CONST(CanNm_HsmActionIdxType, CANNM_CONST)
  CanNm_HsmTrNetworkModeSteps4[1] =
{
  CANNM_HSM_CANNM_AIDX_NETWORKMODE_ENTRY,
};
/* trans sequence for transition #1 from state ReadySleepState */
STATIC CONST(CanNm_HsmActionIdxType, CANNM_CONST)
  CanNm_HsmTrReadySleepStateSteps1[1] =
{
  CANNM_HSM_CANNM_AIDX_PREPAREBUSSLEEPMODE_ENTRY,
};
/* trans sequence for transition #1 from state ReadySleepRemoteActivity */
STATIC CONST(CanNm_HsmActionIdxType, CANNM_CONST)
  CanNm_HsmTrReadySleepRemoteActivitySteps1[3] =
{
  CANNM_HSM_CANNM_AIDX_READYSLEEPREMOTEACTIVITY_ACTION_1,
  CANNM_HSM_CANNM_AIDX_NORMALOPERATIONSTATE_ENTRY,
  CANNM_HSM_CANNM_AIDX_NORMALOPERATIONREMOTEACTIVITY_ENTRY,
};
/* trans sequence for transition #2 from state ReadySleepRemoteActivity */
STATIC CONST(CanNm_HsmActionIdxType, CANNM_CONST)
  CanNm_HsmTrReadySleepRemoteActivitySteps2[1] =
{
  CANNM_HSM_CANNM_AIDX_REPEATMESSAGESTATE_ENTRY,
};
/* trans sequence for transition #1 from state ReadySleepRemoteSleep */
STATIC CONST(CanNm_HsmActionIdxType, CANNM_CONST)
  CanNm_HsmTrReadySleepRemoteSleepSteps1[3] =
{
  CANNM_HSM_CANNM_AIDX_READYSLEEPREMOTESLEEP_ACTION_1,
  CANNM_HSM_CANNM_AIDX_NORMALOPERATIONSTATE_ENTRY,
  CANNM_HSM_CANNM_AIDX_NORMALOPERATIONREMOTESLEEP_ENTRY,
};
/* trans sequence for transition #2 from state ReadySleepRemoteSleep */
STATIC CONST(CanNm_HsmActionIdxType, CANNM_CONST)
  CanNm_HsmTrReadySleepRemoteSleepSteps2[2] =
{
  CANNM_HSM_CANNM_AIDX_READYSLEEPREMOTESLEEP_ACTION_2,
  CANNM_HSM_CANNM_AIDX_REPEATMESSAGESTATE_ENTRY,
};
/* trans sequence for transition #3 from state ReadySleepRemoteSleep */
STATIC CONST(CanNm_HsmActionIdxType, CANNM_CONST)
  CanNm_HsmTrReadySleepRemoteSleepSteps3[2] =
{
  CANNM_HSM_CANNM_AIDX_READYSLEEPREMOTESLEEP_ACTION_3,
  CANNM_HSM_CANNM_AIDX_READYSLEEPREMOTEACTIVITY_ENTRY,
};
/* trans sequence for transition #1 from state SendingSubMode */
STATIC CONST(CanNm_HsmActionIdxType, CANNM_CONST)
  CanNm_HsmTrSendingSubModeSteps1[1] =
{
  CANNM_HSM_CANNM_AIDX_SENDINGSUBMODE_ACTION_1,
};
/* trans sequence for transition #2 from state SendingSubMode */
STATIC CONST(CanNm_HsmActionIdxType, CANNM_CONST)
  CanNm_HsmTrSendingSubModeSteps2[1] =
{
  CANNM_HSM_CANNM_AIDX_SENDINGSUBMODE_ACTION_2,
};
/* trans sequence for transition #3 from state SendingSubMode */
STATIC CONST(CanNm_HsmActionIdxType, CANNM_CONST)
  CanNm_HsmTrSendingSubModeSteps3[1] =
{
  CANNM_HSM_CANNM_AIDX_SENDINGSUBMODE_ACTION_3,
};
/* trans sequence for transition #1 from state NormalOperationRemoteActivity */
STATIC CONST(CanNm_HsmActionIdxType, CANNM_CONST)
  CanNm_HsmTrNormalOperationRemoteActivitySteps1[1] =
{
  CANNM_HSM_CANNM_AIDX_NORMALOPERATIONREMOTEACTIVITY_ACTION_1,
};
/* trans sequence for transition #2 from state NormalOperationRemoteActivity */
STATIC CONST(CanNm_HsmActionIdxType, CANNM_CONST)
  CanNm_HsmTrNormalOperationRemoteActivitySteps2[3] =
{
  CANNM_HSM_CANNM_AIDX_SENDINGSUBMODE_EXIT,
  CANNM_HSM_CANNM_AIDX_READYSLEEPSTATE_ENTRY,
  CANNM_HSM_CANNM_AIDX_READYSLEEPREMOTEACTIVITY_ENTRY,
};
/* trans sequence for transition #3 from state NormalOperationRemoteActivity */
STATIC CONST(CanNm_HsmActionIdxType, CANNM_CONST)
  CanNm_HsmTrNormalOperationRemoteActivitySteps3[1] =
{
  CANNM_HSM_CANNM_AIDX_REPEATMESSAGESTATE_ENTRY,
};
/* trans sequence for transition #4 from state NormalOperationRemoteActivity */
STATIC CONST(CanNm_HsmActionIdxType, CANNM_CONST)
  CanNm_HsmTrNormalOperationRemoteActivitySteps4[2] =
{
  CANNM_HSM_CANNM_AIDX_NORMALOPERATIONREMOTEACTIVITY_ACTION_4,
  CANNM_HSM_CANNM_AIDX_NORMALOPERATIONREMOTESLEEP_ENTRY,
};
/* trans sequence for transition #1 from state NormalOperationRemoteSleep */
STATIC CONST(CanNm_HsmActionIdxType, CANNM_CONST)
  CanNm_HsmTrNormalOperationRemoteSleepSteps1[2] =
{
  CANNM_HSM_CANNM_AIDX_SENDINGSUBMODE_EXIT,
  CANNM_HSM_CANNM_AIDX_READYSLEEPSTATE_ENTRY,
};
/* trans sequence for transition #2 from state NormalOperationRemoteSleep */
STATIC CONST(CanNm_HsmActionIdxType, CANNM_CONST)
  CanNm_HsmTrNormalOperationRemoteSleepSteps2[2] =
{
  CANNM_HSM_CANNM_AIDX_NORMALOPERATIONREMOTESLEEP_ACTION_2,
  CANNM_HSM_CANNM_AIDX_REPEATMESSAGESTATE_ENTRY,
};
/* trans sequence for transition #3 from state NormalOperationRemoteSleep */
STATIC CONST(CanNm_HsmActionIdxType, CANNM_CONST)
  CanNm_HsmTrNormalOperationRemoteSleepSteps3[2] =
{
  CANNM_HSM_CANNM_AIDX_NORMALOPERATIONREMOTESLEEP_ACTION_3,
  CANNM_HSM_CANNM_AIDX_NORMALOPERATIONREMOTEACTIVITY_ENTRY,
};
/* trans sequence for transition #1 from state RepeatMessageState */
STATIC CONST(CanNm_HsmActionIdxType, CANNM_CONST)
  CanNm_HsmTrRepeatMessageStateSteps1[3] =
{
  CANNM_HSM_CANNM_AIDX_REPEATMESSAGESTATE_EXIT,
  CANNM_HSM_CANNM_AIDX_SENDINGSUBMODE_EXIT,
  CANNM_HSM_CANNM_AIDX_READYSLEEPSTATE_ENTRY,
};
/* trans sequence for transition #2 from state RepeatMessageState */
STATIC CONST(CanNm_HsmActionIdxType, CANNM_CONST)
  CanNm_HsmTrRepeatMessageStateSteps2[2] =
{
  CANNM_HSM_CANNM_AIDX_REPEATMESSAGESTATE_EXIT,
  CANNM_HSM_CANNM_AIDX_NORMALOPERATIONSTATE_ENTRY,
};
/* trans sequence for transition #1 from state PrepareBusSleepMode */
STATIC CONST(CanNm_HsmActionIdxType, CANNM_CONST)
  CanNm_HsmTrPrepareBusSleepModeSteps1[1] =
{
  CANNM_HSM_CANNM_AIDX_NETWORKMODE_ENTRY,
};
/* trans sequence for transition #2 from state PrepareBusSleepMode */
STATIC CONST(CanNm_HsmActionIdxType, CANNM_CONST)
  CanNm_HsmTrPrepareBusSleepModeSteps2[1] =
{
  CANNM_HSM_CANNM_AIDX_NETWORKMODE_ENTRY,
};
/* trans sequence for transition #3 from state PrepareBusSleepMode */
STATIC CONST(CanNm_HsmActionIdxType, CANNM_CONST)
  CanNm_HsmTrPrepareBusSleepModeSteps3[2] =
{
  CANNM_HSM_CANNM_AIDX_PREPAREBUSSLEEPMODE_ACTION_3,
  CANNM_HSM_CANNM_AIDX_NETWORKMODE_ENTRY,
};
/* trans sequence for transition #4 from state PrepareBusSleepMode */
STATIC CONST(CanNm_HsmActionIdxType, CANNM_CONST)
  CanNm_HsmTrPrepareBusSleepModeSteps4[1] =
{
  CANNM_HSM_CANNM_AIDX_BUSSLEEPMODE_ENTRY,
};

#define CANNM_STOP_SEC_CONST_8
#include <CanNm_MemMap.h>

/* ****** */

#define CANNM_START_SEC_CONST_UNSPECIFIED
#include <CanNm_MemMap.h>

/* transitions from state BusSleepMode */
STATIC CONST(CanNm_HsmTranType, CANNM_CONST)
 CanNm_HsmTrBusSleepMode[3] =
{
  {
    CanNm_HsmTrBusSleepModeSteps1,
    sizeof(CanNm_HsmTrBusSleepModeSteps1)/sizeof(CanNm_HsmActionIdxType),
    CANNM_HSM_CANNM_EV_RX_INDICATION, /* triggering event */
    CANNM_HSM_GIDX_INVALID, /* no guard */
    CANNM_HSM_SIDX_INVALID /* no target state, internal transition */
  },
  {
    CanNm_HsmTrBusSleepModeSteps2,
    sizeof(CanNm_HsmTrBusSleepModeSteps2)/sizeof(CanNm_HsmActionIdxType),
    CANNM_HSM_CANNM_EV_NETWORK_START, /* triggering event */
    CANNM_HSM_GIDX_INVALID, /* no guard */
    CANNM_HSM_CANNM_SIDX_NETWORKMODE /* target state */
  },
  {
    CanNm_HsmTrBusSleepModeSteps3,
    sizeof(CanNm_HsmTrBusSleepModeSteps3)/sizeof(CanNm_HsmActionIdxType),
    CANNM_HSM_CANNM_EV_NET_REQ_STATUS_CHANGED, /* triggering event */
    CANNM_HSM_CANNM_GIDX_BUSSLEEPMODE_GUARD_3, /* guard idx */
    CANNM_HSM_CANNM_SIDX_NETWORKMODE /* target state */
  },
};
/* transitions from state NetworkMode */
STATIC CONST(CanNm_HsmTranType, CANNM_CONST)
 CanNm_HsmTrNetworkMode[4] =
{
  {
    CanNm_HsmTrNetworkModeSteps1,
    sizeof(CanNm_HsmTrNetworkModeSteps1)/sizeof(CanNm_HsmActionIdxType),
    CANNM_HSM_CANNM_EV_COM_CONTROL, /* triggering event */
    CANNM_HSM_GIDX_INVALID, /* no guard */
    CANNM_HSM_SIDX_INVALID /* no target state, internal transition */
  },
  {
    CanNm_HsmTrNetworkModeSteps2,
    sizeof(CanNm_HsmTrNetworkModeSteps2)/sizeof(CanNm_HsmActionIdxType),
    CANNM_HSM_CANNM_EV_RX_INDICATION, /* triggering event */
    CANNM_HSM_GIDX_INVALID, /* no guard */
    CANNM_HSM_SIDX_INVALID /* no target state, internal transition */
  },
  {
    CanNm_HsmTrNetworkModeSteps3,
    sizeof(CanNm_HsmTrNetworkModeSteps3)/sizeof(CanNm_HsmActionIdxType),
    CANNM_HSM_CANNM_EV_TX_CONFIRMATION, /* triggering event */
    CANNM_HSM_GIDX_INVALID, /* no guard */
    CANNM_HSM_SIDX_INVALID /* no target state, internal transition */
  },
  {
    CanNm_HsmTrNetworkModeSteps4,
    sizeof(CanNm_HsmTrNetworkModeSteps4)/sizeof(CanNm_HsmActionIdxType),
    CANNM_HSM_CANNM_EV_NET_REQ_STATUS_CHANGED, /* triggering event */
    CANNM_HSM_CANNM_GIDX_NETWORKMODE_GUARD_4, /* guard idx */
    CANNM_HSM_CANNM_SIDX_NETWORKMODE /* target state */
  },
};
/* transitions from state ReadySleepState */
STATIC CONST(CanNm_HsmTranType, CANNM_CONST)
 CanNm_HsmTrReadySleepState[1] =
{
  {
    CanNm_HsmTrReadySleepStateSteps1,
    sizeof(CanNm_HsmTrReadySleepStateSteps1)/sizeof(CanNm_HsmActionIdxType),
    CANNM_HSM_CANNM_EV_NM_TIMEOUT, /* triggering event */
    CANNM_HSM_CANNM_GIDX_READYSLEEPSTATE_GUARD_1, /* guard idx */
    CANNM_HSM_CANNM_SIDX_PREPAREBUSSLEEPMODE /* target state */
  },
};
/* transitions from state ReadySleepRemoteActivity */
STATIC CONST(CanNm_HsmTranType, CANNM_CONST)
 CanNm_HsmTrReadySleepRemoteActivity[2] =
{
  {
    CanNm_HsmTrReadySleepRemoteActivitySteps1,
    sizeof(CanNm_HsmTrReadySleepRemoteActivitySteps1)/sizeof(CanNm_HsmActionIdxType),
    CANNM_HSM_CANNM_EV_NET_REQ_STATUS_CHANGED, /* triggering event */
    CANNM_HSM_CANNM_GIDX_READYSLEEPREMOTEACTIVITY_GUARD_1, /* guard idx */
    CANNM_HSM_CANNM_SIDX_NORMALOPERATIONREMOTEACTIVITY /* target state */
  },
  {
    CanNm_HsmTrReadySleepRemoteActivitySteps2,
    sizeof(CanNm_HsmTrReadySleepRemoteActivitySteps2)/sizeof(CanNm_HsmActionIdxType),
    CANNM_HSM_CANNM_EV_REPEAT_MESSAGE_REASON, /* triggering event */
    CANNM_HSM_GIDX_INVALID, /* no guard */
    CANNM_HSM_CANNM_SIDX_REPEATMESSAGESTATE /* target state */
  },
};
/* transitions from state ReadySleepRemoteSleep */
STATIC CONST(CanNm_HsmTranType, CANNM_CONST)
 CanNm_HsmTrReadySleepRemoteSleep[3] =
{
  {
    CanNm_HsmTrReadySleepRemoteSleepSteps1,
    sizeof(CanNm_HsmTrReadySleepRemoteSleepSteps1)/sizeof(CanNm_HsmActionIdxType),
    CANNM_HSM_CANNM_EV_NET_REQ_STATUS_CHANGED, /* triggering event */
    CANNM_HSM_CANNM_GIDX_READYSLEEPREMOTESLEEP_GUARD_1, /* guard idx */
    CANNM_HSM_CANNM_SIDX_NORMALOPERATIONREMOTESLEEP /* target state */
  },
  {
    CanNm_HsmTrReadySleepRemoteSleepSteps2,
    sizeof(CanNm_HsmTrReadySleepRemoteSleepSteps2)/sizeof(CanNm_HsmActionIdxType),
    CANNM_HSM_CANNM_EV_REPEAT_MESSAGE_REASON, /* triggering event */
    CANNM_HSM_GIDX_INVALID, /* no guard */
    CANNM_HSM_CANNM_SIDX_REPEATMESSAGESTATE /* target state */
  },
  {
    CanNm_HsmTrReadySleepRemoteSleepSteps3,
    sizeof(CanNm_HsmTrReadySleepRemoteSleepSteps3)/sizeof(CanNm_HsmActionIdxType),
    CANNM_HSM_CANNM_EV_RX_INDICATION, /* triggering event */
    CANNM_HSM_GIDX_INVALID, /* no guard */
    CANNM_HSM_CANNM_SIDX_READYSLEEPREMOTEACTIVITY /* target state */
  },
};
/* transitions from state SendingSubMode */
STATIC CONST(CanNm_HsmTranType, CANNM_CONST)
 CanNm_HsmTrSendingSubMode[3] =
{
  {
    CanNm_HsmTrSendingSubModeSteps1,
    sizeof(CanNm_HsmTrSendingSubModeSteps1)/sizeof(CanNm_HsmActionIdxType),
    CANNM_HSM_CANNM_EV_TX_TIMEOUT, /* triggering event */
    CANNM_HSM_GIDX_INVALID, /* no guard */
    CANNM_HSM_SIDX_INVALID /* no target state, internal transition */
  },
  {
    CanNm_HsmTrSendingSubModeSteps2,
    sizeof(CanNm_HsmTrSendingSubModeSteps2)/sizeof(CanNm_HsmActionIdxType),
    CANNM_HSM_CANNM_EV_MSG_CYCLE_TIMEOUT, /* triggering event */
    CANNM_HSM_GIDX_INVALID, /* no guard */
    CANNM_HSM_SIDX_INVALID /* no target state, internal transition */
  },
  {
    CanNm_HsmTrSendingSubModeSteps3,
    sizeof(CanNm_HsmTrSendingSubModeSteps3)/sizeof(CanNm_HsmActionIdxType),
    CANNM_HSM_CANNM_EV_NM_TIMEOUT, /* triggering event */
    CANNM_HSM_GIDX_INVALID, /* no guard */
    CANNM_HSM_SIDX_INVALID /* no target state, internal transition */
  },
};
/* transitions from state NormalOperationRemoteActivity */
STATIC CONST(CanNm_HsmTranType, CANNM_CONST)
 CanNm_HsmTrNormalOperationRemoteActivity[4] =
{
  {
    CanNm_HsmTrNormalOperationRemoteActivitySteps1,
    sizeof(CanNm_HsmTrNormalOperationRemoteActivitySteps1)/sizeof(CanNm_HsmActionIdxType),
    CANNM_HSM_CANNM_EV_RX_INDICATION, /* triggering event */
    CANNM_HSM_GIDX_INVALID, /* no guard */
    CANNM_HSM_SIDX_INVALID /* no target state, internal transition */
  },
  {
    CanNm_HsmTrNormalOperationRemoteActivitySteps2,
    sizeof(CanNm_HsmTrNormalOperationRemoteActivitySteps2)/sizeof(CanNm_HsmActionIdxType),
    CANNM_HSM_CANNM_EV_NET_REQ_STATUS_CHANGED, /* triggering event */
    CANNM_HSM_CANNM_GIDX_NORMALOPERATIONREMOTEACTIVITY_GUARD_2, /* guard idx */
    CANNM_HSM_CANNM_SIDX_READYSLEEPREMOTEACTIVITY /* target state */
  },
  {
    CanNm_HsmTrNormalOperationRemoteActivitySteps3,
    sizeof(CanNm_HsmTrNormalOperationRemoteActivitySteps3)/sizeof(CanNm_HsmActionIdxType),
    CANNM_HSM_CANNM_EV_REPEAT_MESSAGE_REASON, /* triggering event */
    CANNM_HSM_GIDX_INVALID, /* no guard */
    CANNM_HSM_CANNM_SIDX_REPEATMESSAGESTATE /* target state */
  },
  {
    CanNm_HsmTrNormalOperationRemoteActivitySteps4,
    sizeof(CanNm_HsmTrNormalOperationRemoteActivitySteps4)/sizeof(CanNm_HsmActionIdxType),
    CANNM_HSM_CANNM_EV_UNI_TIMEOUT, /* triggering event */
    CANNM_HSM_CANNM_GIDX_NORMALOPERATIONREMOTEACTIVITY_GUARD_4, /* guard idx */
    CANNM_HSM_CANNM_SIDX_NORMALOPERATIONREMOTESLEEP /* target state */
  },
};
/* transitions from state NormalOperationRemoteSleep */
STATIC CONST(CanNm_HsmTranType, CANNM_CONST)
 CanNm_HsmTrNormalOperationRemoteSleep[3] =
{
  {
    CanNm_HsmTrNormalOperationRemoteSleepSteps1,
    sizeof(CanNm_HsmTrNormalOperationRemoteSleepSteps1)/sizeof(CanNm_HsmActionIdxType),
    CANNM_HSM_CANNM_EV_NET_REQ_STATUS_CHANGED, /* triggering event */
    CANNM_HSM_CANNM_GIDX_NORMALOPERATIONREMOTESLEEP_GUARD_1, /* guard idx */
    CANNM_HSM_CANNM_SIDX_READYSLEEPREMOTESLEEP /* target state */
  },
  {
    CanNm_HsmTrNormalOperationRemoteSleepSteps2,
    sizeof(CanNm_HsmTrNormalOperationRemoteSleepSteps2)/sizeof(CanNm_HsmActionIdxType),
    CANNM_HSM_CANNM_EV_REPEAT_MESSAGE_REASON, /* triggering event */
    CANNM_HSM_GIDX_INVALID, /* no guard */
    CANNM_HSM_CANNM_SIDX_REPEATMESSAGESTATE /* target state */
  },
  {
    CanNm_HsmTrNormalOperationRemoteSleepSteps3,
    sizeof(CanNm_HsmTrNormalOperationRemoteSleepSteps3)/sizeof(CanNm_HsmActionIdxType),
    CANNM_HSM_CANNM_EV_RX_INDICATION, /* triggering event */
    CANNM_HSM_GIDX_INVALID, /* no guard */
    CANNM_HSM_CANNM_SIDX_NORMALOPERATIONREMOTEACTIVITY /* target state */
  },
};
/* transitions from state RepeatMessageState */
STATIC CONST(CanNm_HsmTranType, CANNM_CONST)
 CanNm_HsmTrRepeatMessageState[2] =
{
  {
    CanNm_HsmTrRepeatMessageStateSteps1,
    sizeof(CanNm_HsmTrRepeatMessageStateSteps1)/sizeof(CanNm_HsmActionIdxType),
    CANNM_HSM_CANNM_EV_RMS_TIMEOUT, /* triggering event */
    CANNM_HSM_CANNM_GIDX_REPEATMESSAGESTATE_GUARD_1, /* guard idx */
    CANNM_HSM_CANNM_SIDX_READYSLEEPSTATE /* target state */
  },
  {
    CanNm_HsmTrRepeatMessageStateSteps2,
    sizeof(CanNm_HsmTrRepeatMessageStateSteps2)/sizeof(CanNm_HsmActionIdxType),
    CANNM_HSM_CANNM_EV_RMS_TIMEOUT, /* triggering event */
    CANNM_HSM_CANNM_GIDX_REPEATMESSAGESTATE_GUARD_2, /* guard idx */
    CANNM_HSM_CANNM_SIDX_NORMALOPERATIONSTATE /* target state */
  },
};
/* transitions from state PrepareBusSleepMode */
STATIC CONST(CanNm_HsmTranType, CANNM_CONST)
 CanNm_HsmTrPrepareBusSleepMode[4] =
{
  {
    CanNm_HsmTrPrepareBusSleepModeSteps1,
    sizeof(CanNm_HsmTrPrepareBusSleepModeSteps1)/sizeof(CanNm_HsmActionIdxType),
    CANNM_HSM_CANNM_EV_NETWORK_START, /* triggering event */
    CANNM_HSM_GIDX_INVALID, /* no guard */
    CANNM_HSM_CANNM_SIDX_NETWORKMODE /* target state */
  },
  {
    CanNm_HsmTrPrepareBusSleepModeSteps2,
    sizeof(CanNm_HsmTrPrepareBusSleepModeSteps2)/sizeof(CanNm_HsmActionIdxType),
    CANNM_HSM_CANNM_EV_NET_REQ_STATUS_CHANGED, /* triggering event */
    CANNM_HSM_CANNM_GIDX_PREPAREBUSSLEEPMODE_GUARD_2, /* guard idx */
    CANNM_HSM_CANNM_SIDX_NETWORKMODE /* target state */
  },
  {
    CanNm_HsmTrPrepareBusSleepModeSteps3,
    sizeof(CanNm_HsmTrPrepareBusSleepModeSteps3)/sizeof(CanNm_HsmActionIdxType),
    CANNM_HSM_CANNM_EV_RX_INDICATION, /* triggering event */
    CANNM_HSM_GIDX_INVALID, /* no guard */
    CANNM_HSM_CANNM_SIDX_NETWORKMODE /* target state */
  },
  {
    CanNm_HsmTrPrepareBusSleepModeSteps4,
    sizeof(CanNm_HsmTrPrepareBusSleepModeSteps4)/sizeof(CanNm_HsmActionIdxType),
    CANNM_HSM_CANNM_EV_UNI_TIMEOUT, /* triggering event */
    CANNM_HSM_GIDX_INVALID, /* no guard */
    CANNM_HSM_CANNM_SIDX_BUSSLEEPMODE /* target state */
  },
};



/* statechart model description of the CanNm state
 * machine */
STATIC CONST(CanNm_HsmStateType, CANNM_CONST)
  CanNm_HsmScdCanNm[CANNM_HSM_CANNM_NO_OF_STATES] =
{
  {
    /* description of state TOP */
    0U, /* no non-deferred event mask (no leaf state) */
    0U, /* no ignored event mask (no leaf state) */
    NULL_PTR,  /* no transitions */
    CANNM_HSM_SIDX_INVALID, /* no parent state */
    CANNM_HSM_CANNM_SIDX_BUSSLEEPMODE, /* init state */
      CANNM_HSM_CANNM_AIDX_TOP_ENTRY, /* entry function */
    CANNM_HSM_AIDX_INVALID, /* no exit action */
    CANNM_HSM_AIDX_INVALID, /* no do action */
    0U  /* no transitions */
  },
  {
    /* description of state BusSleepMode */
    /* non-deferred and non-ignored event mask */
   (1UL << CANNM_HSM_CANNM_EV_NETWORK_START)
    |   (1UL << CANNM_HSM_CANNM_EV_NET_REQ_STATUS_CHANGED)
    |   (1UL << CANNM_HSM_CANNM_EV_RX_INDICATION)
    | 0UL
    ,
      /* ignored event mask */
   (1UL << CANNM_HSM_CANNM_EV_COM_CONTROL)
    |   (1UL << CANNM_HSM_CANNM_EV_TX_CONFIRMATION)
    |   (1UL << CANNM_HSM_CANNM_EV_TX_TIMEOUT)
    |   (1UL << CANNM_HSM_CANNM_EV_MSG_CYCLE_TIMEOUT)
    |   (1UL << CANNM_HSM_CANNM_EV_NM_TIMEOUT)
    |   (1UL << CANNM_HSM_CANNM_EV_REPEAT_MESSAGE_REASON)
    |   (1UL << CANNM_HSM_CANNM_EV_RMS_TIMEOUT)
    |   (1UL << CANNM_HSM_CANNM_EV_UNI_TIMEOUT)
    | 0UL
    ,
    CanNm_HsmTrBusSleepMode, /* transitions for this state */
    CANNM_HSM_CANNM_SIDX_TOP, /* parent state */
    CANNM_HSM_SIDX_INVALID, /* no init state */
      CANNM_HSM_CANNM_AIDX_BUSSLEEPMODE_ENTRY, /* entry function */
    CANNM_HSM_AIDX_INVALID, /* no exit action */
    CANNM_HSM_AIDX_INVALID, /* no do action */
    3 /* num of transitions */
  },
  {
    /* description of state NetworkMode */
    0U, /* no non-deferred event mask (no leaf state) */
    0U, /* no ignored event mask (no leaf state) */
    CanNm_HsmTrNetworkMode, /* transitions for this state */
    CANNM_HSM_CANNM_SIDX_TOP, /* parent state */
    CANNM_HSM_CANNM_SIDX_SENDINGSUBMODE, /* init state */
      CANNM_HSM_CANNM_AIDX_NETWORKMODE_ENTRY, /* entry function */
    CANNM_HSM_AIDX_INVALID, /* no exit action */
    CANNM_HSM_AIDX_INVALID, /* no do action */
    4 /* num of transitions */
  },
  {
    /* description of state ReadySleepState */
    0U, /* no non-deferred event mask (no leaf state) */
    0U, /* no ignored event mask (no leaf state) */
    CanNm_HsmTrReadySleepState, /* transitions for this state */
    CANNM_HSM_CANNM_SIDX_NETWORKMODE, /* parent state */
    CANNM_HSM_CANNM_SIDX_READYSLEEPREMOTEACTIVITY, /* init state */
      CANNM_HSM_CANNM_AIDX_READYSLEEPSTATE_ENTRY, /* entry function */
    CANNM_HSM_AIDX_INVALID, /* no exit action */
    CANNM_HSM_AIDX_INVALID, /* no do action */
    1 /* num of transitions */
  },
  {
    /* description of state ReadySleepRemoteActivity */
    /* non-deferred and non-ignored event mask */
   (1UL << CANNM_HSM_CANNM_EV_COM_CONTROL)
    |   (1UL << CANNM_HSM_CANNM_EV_TX_CONFIRMATION)
    |   (1UL << CANNM_HSM_CANNM_EV_NM_TIMEOUT)
    |   (1UL << CANNM_HSM_CANNM_EV_REPEAT_MESSAGE_REASON)
    |   (1UL << CANNM_HSM_CANNM_EV_NET_REQ_STATUS_CHANGED)
    |   (1UL << CANNM_HSM_CANNM_EV_RX_INDICATION)
    | 0UL
    ,
      /* ignored event mask */
   (1UL << CANNM_HSM_CANNM_EV_TX_TIMEOUT)
    |   (1UL << CANNM_HSM_CANNM_EV_MSG_CYCLE_TIMEOUT)
    |   (1UL << CANNM_HSM_CANNM_EV_RMS_TIMEOUT)
    |   (1UL << CANNM_HSM_CANNM_EV_NETWORK_START)
    |   (1UL << CANNM_HSM_CANNM_EV_UNI_TIMEOUT)
    | 0UL
    ,
    CanNm_HsmTrReadySleepRemoteActivity, /* transitions for this state */
    CANNM_HSM_CANNM_SIDX_READYSLEEPSTATE, /* parent state */
    CANNM_HSM_SIDX_INVALID, /* no init state */
      CANNM_HSM_CANNM_AIDX_READYSLEEPREMOTEACTIVITY_ENTRY, /* entry function */
    CANNM_HSM_AIDX_INVALID, /* no exit action */
    CANNM_HSM_AIDX_INVALID, /* no do action */
    2 /* num of transitions */
  },
  {
    /* description of state ReadySleepRemoteSleep */
    /* non-deferred and non-ignored event mask */
   (1UL << CANNM_HSM_CANNM_EV_COM_CONTROL)
    |   (1UL << CANNM_HSM_CANNM_EV_TX_CONFIRMATION)
    |   (1UL << CANNM_HSM_CANNM_EV_NM_TIMEOUT)
    |   (1UL << CANNM_HSM_CANNM_EV_REPEAT_MESSAGE_REASON)
    |   (1UL << CANNM_HSM_CANNM_EV_NET_REQ_STATUS_CHANGED)
    |   (1UL << CANNM_HSM_CANNM_EV_RX_INDICATION)
    | 0UL
    ,
      /* ignored event mask */
   (1UL << CANNM_HSM_CANNM_EV_TX_TIMEOUT)
    |   (1UL << CANNM_HSM_CANNM_EV_MSG_CYCLE_TIMEOUT)
    |   (1UL << CANNM_HSM_CANNM_EV_RMS_TIMEOUT)
    |   (1UL << CANNM_HSM_CANNM_EV_NETWORK_START)
    |   (1UL << CANNM_HSM_CANNM_EV_UNI_TIMEOUT)
    | 0UL
    ,
    CanNm_HsmTrReadySleepRemoteSleep, /* transitions for this state */
    CANNM_HSM_CANNM_SIDX_READYSLEEPSTATE, /* parent state */
    CANNM_HSM_SIDX_INVALID, /* no init state */
      CANNM_HSM_AIDX_INVALID, /* no entry action */
    CANNM_HSM_AIDX_INVALID, /* no exit action */
    CANNM_HSM_AIDX_INVALID, /* no do action */
    3 /* num of transitions */
  },
  {
    /* description of state SendingSubMode */
    0U, /* no non-deferred event mask (no leaf state) */
    0U, /* no ignored event mask (no leaf state) */
    CanNm_HsmTrSendingSubMode, /* transitions for this state */
    CANNM_HSM_CANNM_SIDX_NETWORKMODE, /* parent state */
    CANNM_HSM_CANNM_SIDX_REPEATMESSAGESTATE, /* init state */
      CANNM_HSM_AIDX_INVALID, /* no entry action */
    CANNM_HSM_CANNM_AIDX_SENDINGSUBMODE_EXIT, /* exit function */
    CANNM_HSM_AIDX_INVALID, /* no do action */
    3 /* num of transitions */
  },
  {
    /* description of state NormalOperationState */
    0U, /* no non-deferred event mask (no leaf state) */
    0U, /* no ignored event mask (no leaf state) */
    NULL_PTR,  /* no transitions */
    CANNM_HSM_CANNM_SIDX_SENDINGSUBMODE, /* parent state */
    CANNM_HSM_CANNM_SIDX_NORMALOPERATIONREMOTEACTIVITY, /* init state */
      CANNM_HSM_CANNM_AIDX_NORMALOPERATIONSTATE_ENTRY, /* entry function */
    CANNM_HSM_AIDX_INVALID, /* no exit action */
    CANNM_HSM_AIDX_INVALID, /* no do action */
    0U  /* no transitions */
  },
  {
    /* description of state NormalOperationRemoteActivity */
    /* non-deferred and non-ignored event mask */
   (1UL << CANNM_HSM_CANNM_EV_COM_CONTROL)
    |   (1UL << CANNM_HSM_CANNM_EV_TX_CONFIRMATION)
    |   (1UL << CANNM_HSM_CANNM_EV_TX_TIMEOUT)
    |   (1UL << CANNM_HSM_CANNM_EV_MSG_CYCLE_TIMEOUT)
    |   (1UL << CANNM_HSM_CANNM_EV_NM_TIMEOUT)
    |   (1UL << CANNM_HSM_CANNM_EV_REPEAT_MESSAGE_REASON)
    |   (1UL << CANNM_HSM_CANNM_EV_NET_REQ_STATUS_CHANGED)
    |   (1UL << CANNM_HSM_CANNM_EV_RX_INDICATION)
    |   (1UL << CANNM_HSM_CANNM_EV_UNI_TIMEOUT)
    | 0UL
    ,
      /* ignored event mask */
   (1UL << CANNM_HSM_CANNM_EV_RMS_TIMEOUT)
    |   (1UL << CANNM_HSM_CANNM_EV_NETWORK_START)
    | 0UL
    ,
    CanNm_HsmTrNormalOperationRemoteActivity, /* transitions for this state */
    CANNM_HSM_CANNM_SIDX_NORMALOPERATIONSTATE, /* parent state */
    CANNM_HSM_SIDX_INVALID, /* no init state */
      CANNM_HSM_CANNM_AIDX_NORMALOPERATIONREMOTEACTIVITY_ENTRY, /* entry function */
    CANNM_HSM_AIDX_INVALID, /* no exit action */
    CANNM_HSM_AIDX_INVALID, /* no do action */
    4 /* num of transitions */
  },
  {
    /* description of state NormalOperationRemoteSleep */
    /* non-deferred and non-ignored event mask */
   (1UL << CANNM_HSM_CANNM_EV_COM_CONTROL)
    |   (1UL << CANNM_HSM_CANNM_EV_TX_CONFIRMATION)
    |   (1UL << CANNM_HSM_CANNM_EV_TX_TIMEOUT)
    |   (1UL << CANNM_HSM_CANNM_EV_MSG_CYCLE_TIMEOUT)
    |   (1UL << CANNM_HSM_CANNM_EV_NM_TIMEOUT)
    |   (1UL << CANNM_HSM_CANNM_EV_REPEAT_MESSAGE_REASON)
    |   (1UL << CANNM_HSM_CANNM_EV_NET_REQ_STATUS_CHANGED)
    |   (1UL << CANNM_HSM_CANNM_EV_RX_INDICATION)
    | 0UL
    ,
      /* ignored event mask */
   (1UL << CANNM_HSM_CANNM_EV_RMS_TIMEOUT)
    |   (1UL << CANNM_HSM_CANNM_EV_NETWORK_START)
    |   (1UL << CANNM_HSM_CANNM_EV_UNI_TIMEOUT)
    | 0UL
    ,
    CanNm_HsmTrNormalOperationRemoteSleep, /* transitions for this state */
    CANNM_HSM_CANNM_SIDX_NORMALOPERATIONSTATE, /* parent state */
    CANNM_HSM_SIDX_INVALID, /* no init state */
      CANNM_HSM_CANNM_AIDX_NORMALOPERATIONREMOTESLEEP_ENTRY, /* entry function */
    CANNM_HSM_AIDX_INVALID, /* no exit action */
    CANNM_HSM_AIDX_INVALID, /* no do action */
    3 /* num of transitions */
  },
  {
    /* description of state RepeatMessageState */
    /* non-deferred and non-ignored event mask */
   (1UL << CANNM_HSM_CANNM_EV_COM_CONTROL)
    |   (1UL << CANNM_HSM_CANNM_EV_TX_CONFIRMATION)
    |   (1UL << CANNM_HSM_CANNM_EV_TX_TIMEOUT)
    |   (1UL << CANNM_HSM_CANNM_EV_MSG_CYCLE_TIMEOUT)
    |   (1UL << CANNM_HSM_CANNM_EV_NM_TIMEOUT)
    |   (1UL << CANNM_HSM_CANNM_EV_RMS_TIMEOUT)
    |   (1UL << CANNM_HSM_CANNM_EV_NET_REQ_STATUS_CHANGED)
    |   (1UL << CANNM_HSM_CANNM_EV_RX_INDICATION)
    | 0UL
    ,
      /* ignored event mask */
   (1UL << CANNM_HSM_CANNM_EV_REPEAT_MESSAGE_REASON)
    |   (1UL << CANNM_HSM_CANNM_EV_NETWORK_START)
    |   (1UL << CANNM_HSM_CANNM_EV_UNI_TIMEOUT)
    | 0UL
    ,
    CanNm_HsmTrRepeatMessageState, /* transitions for this state */
    CANNM_HSM_CANNM_SIDX_SENDINGSUBMODE, /* parent state */
    CANNM_HSM_SIDX_INVALID, /* no init state */
      CANNM_HSM_CANNM_AIDX_REPEATMESSAGESTATE_ENTRY, /* entry function */
    CANNM_HSM_CANNM_AIDX_REPEATMESSAGESTATE_EXIT, /* exit function */
    CANNM_HSM_AIDX_INVALID, /* no do action */
    2 /* num of transitions */
  },
  {
    /* description of state PrepareBusSleepMode */
    /* non-deferred and non-ignored event mask */
   (1UL << CANNM_HSM_CANNM_EV_NETWORK_START)
    |   (1UL << CANNM_HSM_CANNM_EV_NET_REQ_STATUS_CHANGED)
    |   (1UL << CANNM_HSM_CANNM_EV_RX_INDICATION)
    |   (1UL << CANNM_HSM_CANNM_EV_UNI_TIMEOUT)
    | 0UL
    ,
      /* ignored event mask */
   (1UL << CANNM_HSM_CANNM_EV_COM_CONTROL)
    |   (1UL << CANNM_HSM_CANNM_EV_TX_CONFIRMATION)
    |   (1UL << CANNM_HSM_CANNM_EV_TX_TIMEOUT)
    |   (1UL << CANNM_HSM_CANNM_EV_MSG_CYCLE_TIMEOUT)
    |   (1UL << CANNM_HSM_CANNM_EV_NM_TIMEOUT)
    |   (1UL << CANNM_HSM_CANNM_EV_REPEAT_MESSAGE_REASON)
    |   (1UL << CANNM_HSM_CANNM_EV_RMS_TIMEOUT)
    | 0UL
    ,
    CanNm_HsmTrPrepareBusSleepMode, /* transitions for this state */
    CANNM_HSM_CANNM_SIDX_TOP, /* parent state */
    CANNM_HSM_SIDX_INVALID, /* no init state */
      CANNM_HSM_CANNM_AIDX_PREPAREBUSSLEEPMODE_ENTRY, /* entry function */
    CANNM_HSM_AIDX_INVALID, /* no exit action */
    CANNM_HSM_AIDX_INVALID, /* no do action */
    4 /* num of transitions */
  }
};

#if (CANNM_HSM_TRACE == STD_ON)
STATIC CONSTP2CONST(char, CANNM_CONST, CANNM_CONST)
  CanNm_HsmCanNmStateNames[] =
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
  "PrepareBusSleepMode",
};

STATIC CONSTP2CONST(char, CANNM_CONST, CANNM_CONST)
  CanNm_HsmCanNmActionNames[] =
{
  "TOP entry",
  "BusSleepMode entry",
  "BusSleepMode action 1",
  "NetworkMode entry",
  "NetworkMode action 1",
  "NetworkMode action 2",
  "NetworkMode action 3",
  "ReadySleepState entry",
  "ReadySleepRemoteActivity entry",
  "ReadySleepRemoteActivity action 1",
  "ReadySleepRemoteSleep action 1",
  "ReadySleepRemoteSleep action 2",
  "ReadySleepRemoteSleep action 3",
  "SendingSubMode exit",
  "SendingSubMode action 1",
  "SendingSubMode action 2",
  "SendingSubMode action 3",
  "NormalOperationState entry",
  "NormalOperationRemoteActivity entry",
  "NormalOperationRemoteActivity action 1",
  "NormalOperationRemoteActivity action 4",
  "NormalOperationRemoteSleep entry",
  "NormalOperationRemoteSleep action 2",
  "NormalOperationRemoteSleep action 3",
  "RepeatMessageState entry",
  "RepeatMessageState exit",
  "PrepareBusSleepMode entry",
  "PrepareBusSleepMode action 3",
};

STATIC CONSTP2CONST(char, CANNM_CONST, CANNM_CONST)
  CanNm_HsmCanNmGuardNames[] =
{
  "BusSleepMode guard 3",
  "NetworkMode guard 4",
  "ReadySleepState guard 1",
  "ReadySleepRemoteActivity guard 1",
  "ReadySleepRemoteSleep guard 1",
  "NormalOperationRemoteActivity guard 2",
  "NormalOperationRemoteActivity guard 4",
  "NormalOperationRemoteSleep guard 1",
  "RepeatMessageState guard 1",
  "RepeatMessageState guard 2",
  "PrepareBusSleepMode guard 2",
};

STATIC CONSTP2CONST(char, CANNM_CONST, CANNM_CONST)
  CanNm_HsmCanNmEventNames[] =
{
  "COM_CONTROL",
  "TX_CONFIRMATION",
  "TX_TIMEOUT",
  "MSG_CYCLE_TIMEOUT",
  "NM_TIMEOUT",
  "REPEAT_MESSAGE_REASON",
  "RMS_TIMEOUT",
  "NETWORK_START",
  "NET_REQ_STATUS_CHANGED",
  "RX_INDICATION",
  "UNI_TIMEOUT",
};
#endif

/*==================[external constants]====================================*/

/* Top level state chart description data */
CONST(CanNm_HsmStatechartType, CANNM_CONST)
  CanNm_HsmScCanNm =
{
  CanNm_HsmScdCanNm, /* state description array */
  CanNm_HsmInstCanNm, /* instances RAM */
  CanNm_HsmCanNmActions, /* actions */
  CanNm_HsmCanNmGuards, /* guards */
  CanNm_HsmEvCountersCanNm, /* event counters RAM */
  CanNm_HsmEvQueuesCanNm, /* event queues RAM */
#if (CANNM_HSM_TRACE == STD_ON)
  "CanNm", /* name of the HSM */
  CanNm_HsmCanNmStateNames,
  CanNm_HsmCanNmActionNames,
  CanNm_HsmCanNmGuardNames,
  CanNm_HsmCanNmEventNames,
#endif
  CANNM_HSM_CANNM_NUM_EVENTS, /* num of events */
  0U, /* state chart id */
  CANNM_HSM_CANNM_NUM_INST, /* num of instances */
  CANNM_HSM_CANNM_SIDX_TOP /* top state index */
};


#define CANNM_STOP_SEC_CONST_UNSPECIFIED
#include <CanNm_MemMap.h>

/*==================[internal function definitions]=========================*/

/*==================[external function definitions]=========================*/

/*==================[end of file]===========================================*/
