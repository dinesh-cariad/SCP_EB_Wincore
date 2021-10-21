/**
 * \file
 *
 * \brief AUTOSAR UdpNm
 *
 * This file contains the implementation of the AUTOSAR
 * module UdpNm.
 *
 * \version 2.9.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* Definition of the configuration data of the state machine
 * UdpNm.
 *
 * This file contains the definition of all used data structures of the state
 * machine model.  It is generated and must not be edited.
 */
/*==================[inclusions]============================================*/
#define UDPNM_NO_CFGCLASSMIX_REQUIRED
#ifndef UDPNM_NO_PBCFG_REQUIRED
#define UDPNM_NO_PBCFG_REQUIRED
#endif

#include <UdpNm_Hsm.h> /* state machine driver interface */
#include <UdpNm_HsmUdpNm.h> /* public statechart model definitions, */
#include <UdpNm_HsmUdpNmInt.h> /* internal statechart model definitions */
#include <UdpNm_Adaptive.h>

/*==================[macros]================================================*/

/** \brief Number of user events */
#if (defined UDPNM_HSM_UDPNM_NUM_EVENTS)
#error UDPNM_HSM_UDPNM_NUM_EVENTS already defined
#endif
#define UDPNM_HSM_UDPNM_NUM_EVENTS 10U

/* Check if number of hsm instances is defined in the user's configuration */
#ifndef UDPNM_HSM_UDPNM_NUM_INST
#error UDPNM_HSM_UDPNM_NUM_INST must be defined in UdpNm_HsmCfg.h
#endif


/*==================[type definitions]======================================*/

/*==================[internal function declarations]========================*/

/*==================[external function declarations]========================*/

/*==================[internal data]=========================================*/

#if (UDPNM_ADAPTIVE == STD_OFF)
#define UDPNM_START_SEC_VAR_CLEARED_8
#include <UdpNm_MemMap.h>

/* RAM data for event counters */
STATIC VAR(uint8, UDPNM_VAR_CLEARED)
  UdpNm_HsmEvCountersUdpNm[
    UDPNM_HSM_UDPNM_NUM_INST * UDPNM_HSM_UDPNM_NUM_EVENTS];

#define UDPNM_STOP_SEC_VAR_CLEARED_8
#include <UdpNm_MemMap.h>


#define UDPNM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <UdpNm_MemMap.h>

/* RAM data for instances */
STATIC VAR(UdpNm_HsmInstType, UDPNM_VAR_CLEARED)
  UdpNm_HsmInstUdpNm[UDPNM_HSM_UDPNM_NUM_INST];

/* RAM data for event queues */
STATIC VAR(UdpNm_HsmEventType, UDPNM_VAR_CLEARED)
  UdpNm_HsmEvQueuesUdpNm[
    UDPNM_HSM_UDPNM_NUM_INST * UDPNM_HSM_UDPNM_NUM_EVENTS];

#define UDPNM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <UdpNm_MemMap.h>
#endif

/*==================[external data]=========================================*/

/*==================[internal constants]====================================*/

#define UDPNM_START_SEC_CONST_UNSPECIFIED
#include <UdpNm_MemMap.h>

/* *** array of entry/exit/do and transition actions *** */
STATIC CONST(UdpNm_HsmActionType, UDPNM_CONST)
  UdpNm_HsmUdpNmActions[UDPNM_HSM_UDPNM_NO_OF_ACTIONS] =
{
  &UdpNm_HsmUdpNmSfTOPEntry,
  &UdpNm_HsmUdpNmSfBusSleepModeEntry,
  &UdpNm_HsmUdpNmSfBusSleepModeAction1,
  &UdpNm_HsmUdpNmSfNetworkModeEntry,
  &UdpNm_HsmUdpNmSfNetworkModeAction1,
  &UdpNm_HsmUdpNmSfNetworkModeAction2,
  &UdpNm_HsmUdpNmSfNetworkModeAction3,
  &UdpNm_HsmUdpNmSfReadySleepStateEntry,
  &UdpNm_HsmUdpNmSfReadySleepRemoteActivityEntry,
  &UdpNm_HsmUdpNmSfReadySleepRemoteActivityAction1,
  &UdpNm_HsmUdpNmSfReadySleepRemoteSleepAction1,
  &UdpNm_HsmUdpNmSfReadySleepRemoteSleepAction2,
  &UdpNm_HsmUdpNmSfReadySleepRemoteSleepAction3,
  &UdpNm_HsmUdpNmSfSendingSubModeExit,
  &UdpNm_HsmUdpNmSfSendingSubModeAction1,
  &UdpNm_HsmUdpNmSfSendingSubModeAction2,
  &UdpNm_HsmUdpNmSfNormalOperationStateEntry,
  &UdpNm_HsmUdpNmSfNormalOperationRemoteActivityEntry,
  &UdpNm_HsmUdpNmSfNormalOperationRemoteActivityAction1,
  &UdpNm_HsmUdpNmSfNormalOperationRemoteActivityAction4,
  &UdpNm_HsmUdpNmSfNormalOperationRemoteSleepEntry,
  &UdpNm_HsmUdpNmSfNormalOperationRemoteSleepAction2,
  &UdpNm_HsmUdpNmSfNormalOperationRemoteSleepAction3,
  &UdpNm_HsmUdpNmSfRepeatMessageStateEntry,
  &UdpNm_HsmUdpNmSfRepeatMessageStateExit,
  &UdpNm_HsmUdpNmSfPrepareBusSleepModeEntry,
  &UdpNm_HsmUdpNmSfPrepareBusSleepModeAction3,
};

/* *** array of guards *** */
STATIC CONST(UdpNm_HsmGuardType, UDPNM_CONST)
  UdpNm_HsmUdpNmGuards[UDPNM_HSM_UDPNM_NO_OF_GUARDS] =
{
  &UdpNm_HsmUdpNmSfBusSleepModeGuard3,
  &UdpNm_HsmUdpNmSfNetworkModeGuard4,
  &UdpNm_HsmUdpNmSfReadySleepStateGuard1,
  &UdpNm_HsmUdpNmSfReadySleepRemoteActivityGuard1,
  &UdpNm_HsmUdpNmSfReadySleepRemoteSleepGuard1,
  &UdpNm_HsmUdpNmSfNormalOperationRemoteActivityGuard2,
  &UdpNm_HsmUdpNmSfNormalOperationRemoteActivityGuard4,
  &UdpNm_HsmUdpNmSfNormalOperationRemoteSleepGuard1,
  &UdpNm_HsmUdpNmSfRepeatMessageStateGuard1,
  &UdpNm_HsmUdpNmSfRepeatMessageStateGuard2,
  &UdpNm_HsmUdpNmSfPrepareBusSleepModeGuard2,
};

#define UDPNM_STOP_SEC_CONST_UNSPECIFIED
#include <UdpNm_MemMap.h>

#define UDPNM_START_SEC_CONST_8
#include <UdpNm_MemMap.h>

/* *** exit/action/entry sequences for transitions *** */
/* trans sequence for transition #1 from state BusSleepMode */
STATIC CONST(UdpNm_HsmActionIdxType, UDPNM_CONST)
  UdpNm_HsmTrBusSleepModeSteps1[1] =
{
  UDPNM_HSM_UDPNM_AIDX_BUSSLEEPMODE_ACTION_1,
};
/* trans sequence for transition #2 from state BusSleepMode */
STATIC CONST(UdpNm_HsmActionIdxType, UDPNM_CONST)
  UdpNm_HsmTrBusSleepModeSteps2[1] =
{
  UDPNM_HSM_UDPNM_AIDX_NETWORKMODE_ENTRY,
};
/* trans sequence for transition #3 from state BusSleepMode */
STATIC CONST(UdpNm_HsmActionIdxType, UDPNM_CONST)
  UdpNm_HsmTrBusSleepModeSteps3[1] =
{
  UDPNM_HSM_UDPNM_AIDX_NETWORKMODE_ENTRY,
};
/* trans sequence for transition #1 from state NetworkMode */
STATIC CONST(UdpNm_HsmActionIdxType, UDPNM_CONST)
  UdpNm_HsmTrNetworkModeSteps1[1] =
{
  UDPNM_HSM_UDPNM_AIDX_NETWORKMODE_ACTION_1,
};
/* trans sequence for transition #2 from state NetworkMode */
STATIC CONST(UdpNm_HsmActionIdxType, UDPNM_CONST)
  UdpNm_HsmTrNetworkModeSteps2[1] =
{
  UDPNM_HSM_UDPNM_AIDX_NETWORKMODE_ACTION_2,
};
/* trans sequence for transition #3 from state NetworkMode */
STATIC CONST(UdpNm_HsmActionIdxType, UDPNM_CONST)
  UdpNm_HsmTrNetworkModeSteps3[1] =
{
  UDPNM_HSM_UDPNM_AIDX_NETWORKMODE_ACTION_3,
};
/* trans sequence for transition #4 from state NetworkMode */
STATIC CONST(UdpNm_HsmActionIdxType, UDPNM_CONST)
  UdpNm_HsmTrNetworkModeSteps4[1] =
{
  UDPNM_HSM_UDPNM_AIDX_NETWORKMODE_ENTRY,
};
/* trans sequence for transition #1 from state ReadySleepState */
STATIC CONST(UdpNm_HsmActionIdxType, UDPNM_CONST)
  UdpNm_HsmTrReadySleepStateSteps1[1] =
{
  UDPNM_HSM_UDPNM_AIDX_PREPAREBUSSLEEPMODE_ENTRY,
};
/* trans sequence for transition #1 from state ReadySleepRemoteActivity */
STATIC CONST(UdpNm_HsmActionIdxType, UDPNM_CONST)
  UdpNm_HsmTrReadySleepRemoteActivitySteps1[3] =
{
  UDPNM_HSM_UDPNM_AIDX_READYSLEEPREMOTEACTIVITY_ACTION_1,
  UDPNM_HSM_UDPNM_AIDX_NORMALOPERATIONSTATE_ENTRY,
  UDPNM_HSM_UDPNM_AIDX_NORMALOPERATIONREMOTEACTIVITY_ENTRY,
};
/* trans sequence for transition #2 from state ReadySleepRemoteActivity */
STATIC CONST(UdpNm_HsmActionIdxType, UDPNM_CONST)
  UdpNm_HsmTrReadySleepRemoteActivitySteps2[1] =
{
  UDPNM_HSM_UDPNM_AIDX_REPEATMESSAGESTATE_ENTRY,
};
/* trans sequence for transition #1 from state ReadySleepRemoteSleep */
STATIC CONST(UdpNm_HsmActionIdxType, UDPNM_CONST)
  UdpNm_HsmTrReadySleepRemoteSleepSteps1[3] =
{
  UDPNM_HSM_UDPNM_AIDX_READYSLEEPREMOTESLEEP_ACTION_1,
  UDPNM_HSM_UDPNM_AIDX_NORMALOPERATIONSTATE_ENTRY,
  UDPNM_HSM_UDPNM_AIDX_NORMALOPERATIONREMOTESLEEP_ENTRY,
};
/* trans sequence for transition #2 from state ReadySleepRemoteSleep */
STATIC CONST(UdpNm_HsmActionIdxType, UDPNM_CONST)
  UdpNm_HsmTrReadySleepRemoteSleepSteps2[2] =
{
  UDPNM_HSM_UDPNM_AIDX_READYSLEEPREMOTESLEEP_ACTION_2,
  UDPNM_HSM_UDPNM_AIDX_REPEATMESSAGESTATE_ENTRY,
};
/* trans sequence for transition #3 from state ReadySleepRemoteSleep */
STATIC CONST(UdpNm_HsmActionIdxType, UDPNM_CONST)
  UdpNm_HsmTrReadySleepRemoteSleepSteps3[2] =
{
  UDPNM_HSM_UDPNM_AIDX_READYSLEEPREMOTESLEEP_ACTION_3,
  UDPNM_HSM_UDPNM_AIDX_READYSLEEPREMOTEACTIVITY_ENTRY,
};
/* trans sequence for transition #1 from state SendingSubMode */
STATIC CONST(UdpNm_HsmActionIdxType, UDPNM_CONST)
  UdpNm_HsmTrSendingSubModeSteps1[1] =
{
  UDPNM_HSM_UDPNM_AIDX_SENDINGSUBMODE_ACTION_1,
};
/* trans sequence for transition #2 from state SendingSubMode */
STATIC CONST(UdpNm_HsmActionIdxType, UDPNM_CONST)
  UdpNm_HsmTrSendingSubModeSteps2[1] =
{
  UDPNM_HSM_UDPNM_AIDX_SENDINGSUBMODE_ACTION_2,
};
/* trans sequence for transition #1 from state NormalOperationRemoteActivity */
STATIC CONST(UdpNm_HsmActionIdxType, UDPNM_CONST)
  UdpNm_HsmTrNormalOperationRemoteActivitySteps1[1] =
{
  UDPNM_HSM_UDPNM_AIDX_NORMALOPERATIONREMOTEACTIVITY_ACTION_1,
};
/* trans sequence for transition #2 from state NormalOperationRemoteActivity */
STATIC CONST(UdpNm_HsmActionIdxType, UDPNM_CONST)
  UdpNm_HsmTrNormalOperationRemoteActivitySteps2[3] =
{
  UDPNM_HSM_UDPNM_AIDX_SENDINGSUBMODE_EXIT,
  UDPNM_HSM_UDPNM_AIDX_READYSLEEPSTATE_ENTRY,
  UDPNM_HSM_UDPNM_AIDX_READYSLEEPREMOTEACTIVITY_ENTRY,
};
/* trans sequence for transition #3 from state NormalOperationRemoteActivity */
STATIC CONST(UdpNm_HsmActionIdxType, UDPNM_CONST)
  UdpNm_HsmTrNormalOperationRemoteActivitySteps3[1] =
{
  UDPNM_HSM_UDPNM_AIDX_REPEATMESSAGESTATE_ENTRY,
};
/* trans sequence for transition #4 from state NormalOperationRemoteActivity */
STATIC CONST(UdpNm_HsmActionIdxType, UDPNM_CONST)
  UdpNm_HsmTrNormalOperationRemoteActivitySteps4[2] =
{
  UDPNM_HSM_UDPNM_AIDX_NORMALOPERATIONREMOTEACTIVITY_ACTION_4,
  UDPNM_HSM_UDPNM_AIDX_NORMALOPERATIONREMOTESLEEP_ENTRY,
};
/* trans sequence for transition #1 from state NormalOperationRemoteSleep */
STATIC CONST(UdpNm_HsmActionIdxType, UDPNM_CONST)
  UdpNm_HsmTrNormalOperationRemoteSleepSteps1[2] =
{
  UDPNM_HSM_UDPNM_AIDX_SENDINGSUBMODE_EXIT,
  UDPNM_HSM_UDPNM_AIDX_READYSLEEPSTATE_ENTRY,
};
/* trans sequence for transition #2 from state NormalOperationRemoteSleep */
STATIC CONST(UdpNm_HsmActionIdxType, UDPNM_CONST)
  UdpNm_HsmTrNormalOperationRemoteSleepSteps2[2] =
{
  UDPNM_HSM_UDPNM_AIDX_NORMALOPERATIONREMOTESLEEP_ACTION_2,
  UDPNM_HSM_UDPNM_AIDX_REPEATMESSAGESTATE_ENTRY,
};
/* trans sequence for transition #3 from state NormalOperationRemoteSleep */
STATIC CONST(UdpNm_HsmActionIdxType, UDPNM_CONST)
  UdpNm_HsmTrNormalOperationRemoteSleepSteps3[2] =
{
  UDPNM_HSM_UDPNM_AIDX_NORMALOPERATIONREMOTESLEEP_ACTION_3,
  UDPNM_HSM_UDPNM_AIDX_NORMALOPERATIONREMOTEACTIVITY_ENTRY,
};
/* trans sequence for transition #1 from state RepeatMessageState */
STATIC CONST(UdpNm_HsmActionIdxType, UDPNM_CONST)
  UdpNm_HsmTrRepeatMessageStateSteps1[3] =
{
  UDPNM_HSM_UDPNM_AIDX_REPEATMESSAGESTATE_EXIT,
  UDPNM_HSM_UDPNM_AIDX_SENDINGSUBMODE_EXIT,
  UDPNM_HSM_UDPNM_AIDX_READYSLEEPSTATE_ENTRY,
};
/* trans sequence for transition #2 from state RepeatMessageState */
STATIC CONST(UdpNm_HsmActionIdxType, UDPNM_CONST)
  UdpNm_HsmTrRepeatMessageStateSteps2[2] =
{
  UDPNM_HSM_UDPNM_AIDX_REPEATMESSAGESTATE_EXIT,
  UDPNM_HSM_UDPNM_AIDX_NORMALOPERATIONSTATE_ENTRY,
};
/* trans sequence for transition #1 from state PrepareBusSleepMode */
STATIC CONST(UdpNm_HsmActionIdxType, UDPNM_CONST)
  UdpNm_HsmTrPrepareBusSleepModeSteps1[1] =
{
  UDPNM_HSM_UDPNM_AIDX_NETWORKMODE_ENTRY,
};
/* trans sequence for transition #2 from state PrepareBusSleepMode */
STATIC CONST(UdpNm_HsmActionIdxType, UDPNM_CONST)
  UdpNm_HsmTrPrepareBusSleepModeSteps2[1] =
{
  UDPNM_HSM_UDPNM_AIDX_NETWORKMODE_ENTRY,
};
/* trans sequence for transition #3 from state PrepareBusSleepMode */
STATIC CONST(UdpNm_HsmActionIdxType, UDPNM_CONST)
  UdpNm_HsmTrPrepareBusSleepModeSteps3[2] =
{
  UDPNM_HSM_UDPNM_AIDX_PREPAREBUSSLEEPMODE_ACTION_3,
  UDPNM_HSM_UDPNM_AIDX_NETWORKMODE_ENTRY,
};
/* trans sequence for transition #4 from state PrepareBusSleepMode */
STATIC CONST(UdpNm_HsmActionIdxType, UDPNM_CONST)
  UdpNm_HsmTrPrepareBusSleepModeSteps4[1] =
{
  UDPNM_HSM_UDPNM_AIDX_BUSSLEEPMODE_ENTRY,
};

#define UDPNM_STOP_SEC_CONST_8
#include <UdpNm_MemMap.h>

/* ****** */

#define UDPNM_START_SEC_CONST_UNSPECIFIED
#include <UdpNm_MemMap.h>

/* transitions from state BusSleepMode */
STATIC CONST(UdpNm_HsmTranType, UDPNM_CONST)
 UdpNm_HsmTrBusSleepMode[3] =
{
  {
    UdpNm_HsmTrBusSleepModeSteps1,
    sizeof(UdpNm_HsmTrBusSleepModeSteps1)/sizeof(UdpNm_HsmActionIdxType),
    UDPNM_HSM_UDPNM_EV_RX_INDICATION, /* triggering event */
    UDPNM_HSM_GIDX_INVALID, /* no guard */
    UDPNM_HSM_SIDX_INVALID /* no target state, internal transition */
  },
  {
    UdpNm_HsmTrBusSleepModeSteps2,
    sizeof(UdpNm_HsmTrBusSleepModeSteps2)/sizeof(UdpNm_HsmActionIdxType),
    UDPNM_HSM_UDPNM_EV_NETWORK_START, /* triggering event */
    UDPNM_HSM_GIDX_INVALID, /* no guard */
    UDPNM_HSM_UDPNM_SIDX_NETWORKMODE /* target state */
  },
  {
    UdpNm_HsmTrBusSleepModeSteps3,
    sizeof(UdpNm_HsmTrBusSleepModeSteps3)/sizeof(UdpNm_HsmActionIdxType),
    UDPNM_HSM_UDPNM_EV_NET_REQ_STATUS_CHANGED, /* triggering event */
    UDPNM_HSM_UDPNM_GIDX_BUSSLEEPMODE_GUARD_3, /* guard idx */
    UDPNM_HSM_UDPNM_SIDX_NETWORKMODE /* target state */
  },
};
/* transitions from state NetworkMode */
STATIC CONST(UdpNm_HsmTranType, UDPNM_CONST)
 UdpNm_HsmTrNetworkMode[4] =
{
  {
    UdpNm_HsmTrNetworkModeSteps1,
    sizeof(UdpNm_HsmTrNetworkModeSteps1)/sizeof(UdpNm_HsmActionIdxType),
    UDPNM_HSM_UDPNM_EV_COM_CONTROL, /* triggering event */
    UDPNM_HSM_GIDX_INVALID, /* no guard */
    UDPNM_HSM_SIDX_INVALID /* no target state, internal transition */
  },
  {
    UdpNm_HsmTrNetworkModeSteps2,
    sizeof(UdpNm_HsmTrNetworkModeSteps2)/sizeof(UdpNm_HsmActionIdxType),
    UDPNM_HSM_UDPNM_EV_RX_INDICATION, /* triggering event */
    UDPNM_HSM_GIDX_INVALID, /* no guard */
    UDPNM_HSM_SIDX_INVALID /* no target state, internal transition */
  },
  {
    UdpNm_HsmTrNetworkModeSteps3,
    sizeof(UdpNm_HsmTrNetworkModeSteps3)/sizeof(UdpNm_HsmActionIdxType),
    UDPNM_HSM_UDPNM_EV_TX_CONFIRMATION, /* triggering event */
    UDPNM_HSM_GIDX_INVALID, /* no guard */
    UDPNM_HSM_SIDX_INVALID /* no target state, internal transition */
  },
  {
    UdpNm_HsmTrNetworkModeSteps4,
    sizeof(UdpNm_HsmTrNetworkModeSteps4)/sizeof(UdpNm_HsmActionIdxType),
    UDPNM_HSM_UDPNM_EV_NET_REQ_STATUS_CHANGED, /* triggering event */
    UDPNM_HSM_UDPNM_GIDX_NETWORKMODE_GUARD_4, /* guard idx */
    UDPNM_HSM_UDPNM_SIDX_NETWORKMODE /* target state */
  },
};
/* transitions from state ReadySleepState */
STATIC CONST(UdpNm_HsmTranType, UDPNM_CONST)
 UdpNm_HsmTrReadySleepState[1] =
{
  {
    UdpNm_HsmTrReadySleepStateSteps1,
    sizeof(UdpNm_HsmTrReadySleepStateSteps1)/sizeof(UdpNm_HsmActionIdxType),
    UDPNM_HSM_UDPNM_EV_NM_TIMEOUT, /* triggering event */
    UDPNM_HSM_UDPNM_GIDX_READYSLEEPSTATE_GUARD_1, /* guard idx */
    UDPNM_HSM_UDPNM_SIDX_PREPAREBUSSLEEPMODE /* target state */
  },
};
/* transitions from state ReadySleepRemoteActivity */
STATIC CONST(UdpNm_HsmTranType, UDPNM_CONST)
 UdpNm_HsmTrReadySleepRemoteActivity[2] =
{
  {
    UdpNm_HsmTrReadySleepRemoteActivitySteps1,
    sizeof(UdpNm_HsmTrReadySleepRemoteActivitySteps1)/sizeof(UdpNm_HsmActionIdxType),
    UDPNM_HSM_UDPNM_EV_NET_REQ_STATUS_CHANGED, /* triggering event */
    UDPNM_HSM_UDPNM_GIDX_READYSLEEPREMOTEACTIVITY_GUARD_1, /* guard idx */
    UDPNM_HSM_UDPNM_SIDX_NORMALOPERATIONREMOTEACTIVITY /* target state */
  },
  {
    UdpNm_HsmTrReadySleepRemoteActivitySteps2,
    sizeof(UdpNm_HsmTrReadySleepRemoteActivitySteps2)/sizeof(UdpNm_HsmActionIdxType),
    UDPNM_HSM_UDPNM_EV_REPEAT_MESSAGE_REASON, /* triggering event */
    UDPNM_HSM_GIDX_INVALID, /* no guard */
    UDPNM_HSM_UDPNM_SIDX_REPEATMESSAGESTATE /* target state */
  },
};
/* transitions from state ReadySleepRemoteSleep */
STATIC CONST(UdpNm_HsmTranType, UDPNM_CONST)
 UdpNm_HsmTrReadySleepRemoteSleep[3] =
{
  {
    UdpNm_HsmTrReadySleepRemoteSleepSteps1,
    sizeof(UdpNm_HsmTrReadySleepRemoteSleepSteps1)/sizeof(UdpNm_HsmActionIdxType),
    UDPNM_HSM_UDPNM_EV_NET_REQ_STATUS_CHANGED, /* triggering event */
    UDPNM_HSM_UDPNM_GIDX_READYSLEEPREMOTESLEEP_GUARD_1, /* guard idx */
    UDPNM_HSM_UDPNM_SIDX_NORMALOPERATIONREMOTESLEEP /* target state */
  },
  {
    UdpNm_HsmTrReadySleepRemoteSleepSteps2,
    sizeof(UdpNm_HsmTrReadySleepRemoteSleepSteps2)/sizeof(UdpNm_HsmActionIdxType),
    UDPNM_HSM_UDPNM_EV_REPEAT_MESSAGE_REASON, /* triggering event */
    UDPNM_HSM_GIDX_INVALID, /* no guard */
    UDPNM_HSM_UDPNM_SIDX_REPEATMESSAGESTATE /* target state */
  },
  {
    UdpNm_HsmTrReadySleepRemoteSleepSteps3,
    sizeof(UdpNm_HsmTrReadySleepRemoteSleepSteps3)/sizeof(UdpNm_HsmActionIdxType),
    UDPNM_HSM_UDPNM_EV_RX_INDICATION, /* triggering event */
    UDPNM_HSM_GIDX_INVALID, /* no guard */
    UDPNM_HSM_UDPNM_SIDX_READYSLEEPREMOTEACTIVITY /* target state */
  },
};
/* transitions from state SendingSubMode */
STATIC CONST(UdpNm_HsmTranType, UDPNM_CONST)
 UdpNm_HsmTrSendingSubMode[2] =
{
  {
    UdpNm_HsmTrSendingSubModeSteps1,
    sizeof(UdpNm_HsmTrSendingSubModeSteps1)/sizeof(UdpNm_HsmActionIdxType),
    UDPNM_HSM_UDPNM_EV_NM_TIMEOUT, /* triggering event */
    UDPNM_HSM_GIDX_INVALID, /* no guard */
    UDPNM_HSM_SIDX_INVALID /* no target state, internal transition */
  },
  {
    UdpNm_HsmTrSendingSubModeSteps2,
    sizeof(UdpNm_HsmTrSendingSubModeSteps2)/sizeof(UdpNm_HsmActionIdxType),
    UDPNM_HSM_UDPNM_EV_MSG_CYCLE_TIMEOUT, /* triggering event */
    UDPNM_HSM_GIDX_INVALID, /* no guard */
    UDPNM_HSM_SIDX_INVALID /* no target state, internal transition */
  },
};
/* transitions from state NormalOperationRemoteActivity */
STATIC CONST(UdpNm_HsmTranType, UDPNM_CONST)
 UdpNm_HsmTrNormalOperationRemoteActivity[4] =
{
  {
    UdpNm_HsmTrNormalOperationRemoteActivitySteps1,
    sizeof(UdpNm_HsmTrNormalOperationRemoteActivitySteps1)/sizeof(UdpNm_HsmActionIdxType),
    UDPNM_HSM_UDPNM_EV_RX_INDICATION, /* triggering event */
    UDPNM_HSM_GIDX_INVALID, /* no guard */
    UDPNM_HSM_SIDX_INVALID /* no target state, internal transition */
  },
  {
    UdpNm_HsmTrNormalOperationRemoteActivitySteps2,
    sizeof(UdpNm_HsmTrNormalOperationRemoteActivitySteps2)/sizeof(UdpNm_HsmActionIdxType),
    UDPNM_HSM_UDPNM_EV_NET_REQ_STATUS_CHANGED, /* triggering event */
    UDPNM_HSM_UDPNM_GIDX_NORMALOPERATIONREMOTEACTIVITY_GUARD_2, /* guard idx */
    UDPNM_HSM_UDPNM_SIDX_READYSLEEPREMOTEACTIVITY /* target state */
  },
  {
    UdpNm_HsmTrNormalOperationRemoteActivitySteps3,
    sizeof(UdpNm_HsmTrNormalOperationRemoteActivitySteps3)/sizeof(UdpNm_HsmActionIdxType),
    UDPNM_HSM_UDPNM_EV_REPEAT_MESSAGE_REASON, /* triggering event */
    UDPNM_HSM_GIDX_INVALID, /* no guard */
    UDPNM_HSM_UDPNM_SIDX_REPEATMESSAGESTATE /* target state */
  },
  {
    UdpNm_HsmTrNormalOperationRemoteActivitySteps4,
    sizeof(UdpNm_HsmTrNormalOperationRemoteActivitySteps4)/sizeof(UdpNm_HsmActionIdxType),
    UDPNM_HSM_UDPNM_EV_UNI_TIMEOUT, /* triggering event */
    UDPNM_HSM_UDPNM_GIDX_NORMALOPERATIONREMOTEACTIVITY_GUARD_4, /* guard idx */
    UDPNM_HSM_UDPNM_SIDX_NORMALOPERATIONREMOTESLEEP /* target state */
  },
};
/* transitions from state NormalOperationRemoteSleep */
STATIC CONST(UdpNm_HsmTranType, UDPNM_CONST)
 UdpNm_HsmTrNormalOperationRemoteSleep[3] =
{
  {
    UdpNm_HsmTrNormalOperationRemoteSleepSteps1,
    sizeof(UdpNm_HsmTrNormalOperationRemoteSleepSteps1)/sizeof(UdpNm_HsmActionIdxType),
    UDPNM_HSM_UDPNM_EV_NET_REQ_STATUS_CHANGED, /* triggering event */
    UDPNM_HSM_UDPNM_GIDX_NORMALOPERATIONREMOTESLEEP_GUARD_1, /* guard idx */
    UDPNM_HSM_UDPNM_SIDX_READYSLEEPREMOTESLEEP /* target state */
  },
  {
    UdpNm_HsmTrNormalOperationRemoteSleepSteps2,
    sizeof(UdpNm_HsmTrNormalOperationRemoteSleepSteps2)/sizeof(UdpNm_HsmActionIdxType),
    UDPNM_HSM_UDPNM_EV_REPEAT_MESSAGE_REASON, /* triggering event */
    UDPNM_HSM_GIDX_INVALID, /* no guard */
    UDPNM_HSM_UDPNM_SIDX_REPEATMESSAGESTATE /* target state */
  },
  {
    UdpNm_HsmTrNormalOperationRemoteSleepSteps3,
    sizeof(UdpNm_HsmTrNormalOperationRemoteSleepSteps3)/sizeof(UdpNm_HsmActionIdxType),
    UDPNM_HSM_UDPNM_EV_RX_INDICATION, /* triggering event */
    UDPNM_HSM_GIDX_INVALID, /* no guard */
    UDPNM_HSM_UDPNM_SIDX_NORMALOPERATIONREMOTEACTIVITY /* target state */
  },
};
/* transitions from state RepeatMessageState */
STATIC CONST(UdpNm_HsmTranType, UDPNM_CONST)
 UdpNm_HsmTrRepeatMessageState[2] =
{
  {
    UdpNm_HsmTrRepeatMessageStateSteps1,
    sizeof(UdpNm_HsmTrRepeatMessageStateSteps1)/sizeof(UdpNm_HsmActionIdxType),
    UDPNM_HSM_UDPNM_EV_RMS_TIMEOUT, /* triggering event */
    UDPNM_HSM_UDPNM_GIDX_REPEATMESSAGESTATE_GUARD_1, /* guard idx */
    UDPNM_HSM_UDPNM_SIDX_READYSLEEPSTATE /* target state */
  },
  {
    UdpNm_HsmTrRepeatMessageStateSteps2,
    sizeof(UdpNm_HsmTrRepeatMessageStateSteps2)/sizeof(UdpNm_HsmActionIdxType),
    UDPNM_HSM_UDPNM_EV_RMS_TIMEOUT, /* triggering event */
    UDPNM_HSM_UDPNM_GIDX_REPEATMESSAGESTATE_GUARD_2, /* guard idx */
    UDPNM_HSM_UDPNM_SIDX_NORMALOPERATIONSTATE /* target state */
  },
};
/* transitions from state PrepareBusSleepMode */
STATIC CONST(UdpNm_HsmTranType, UDPNM_CONST)
 UdpNm_HsmTrPrepareBusSleepMode[4] =
{
  {
    UdpNm_HsmTrPrepareBusSleepModeSteps1,
    sizeof(UdpNm_HsmTrPrepareBusSleepModeSteps1)/sizeof(UdpNm_HsmActionIdxType),
    UDPNM_HSM_UDPNM_EV_NETWORK_START, /* triggering event */
    UDPNM_HSM_GIDX_INVALID, /* no guard */
    UDPNM_HSM_UDPNM_SIDX_NETWORKMODE /* target state */
  },
  {
    UdpNm_HsmTrPrepareBusSleepModeSteps2,
    sizeof(UdpNm_HsmTrPrepareBusSleepModeSteps2)/sizeof(UdpNm_HsmActionIdxType),
    UDPNM_HSM_UDPNM_EV_NET_REQ_STATUS_CHANGED, /* triggering event */
    UDPNM_HSM_UDPNM_GIDX_PREPAREBUSSLEEPMODE_GUARD_2, /* guard idx */
    UDPNM_HSM_UDPNM_SIDX_NETWORKMODE /* target state */
  },
  {
    UdpNm_HsmTrPrepareBusSleepModeSteps3,
    sizeof(UdpNm_HsmTrPrepareBusSleepModeSteps3)/sizeof(UdpNm_HsmActionIdxType),
    UDPNM_HSM_UDPNM_EV_RX_INDICATION, /* triggering event */
    UDPNM_HSM_GIDX_INVALID, /* no guard */
    UDPNM_HSM_UDPNM_SIDX_NETWORKMODE /* target state */
  },
  {
    UdpNm_HsmTrPrepareBusSleepModeSteps4,
    sizeof(UdpNm_HsmTrPrepareBusSleepModeSteps4)/sizeof(UdpNm_HsmActionIdxType),
    UDPNM_HSM_UDPNM_EV_UNI_TIMEOUT, /* triggering event */
    UDPNM_HSM_GIDX_INVALID, /* no guard */
    UDPNM_HSM_UDPNM_SIDX_BUSSLEEPMODE /* target state */
  },
};



/* statechart model description of the UdpNm state
 * machine */
STATIC CONST(UdpNm_HsmStateType, UDPNM_CONST)
  UdpNm_HsmScdUdpNm[UDPNM_HSM_UDPNM_NO_OF_STATES] =
{
  {
    /* description of state TOP */
    0U, /* no non-deferred event mask (no leaf state) */
    0U, /* no ignored event mask (no leaf state) */
    NULL_PTR,  /* no transitions */
    UDPNM_HSM_SIDX_INVALID, /* no parent state */
    UDPNM_HSM_UDPNM_SIDX_BUSSLEEPMODE, /* init state */
      UDPNM_HSM_UDPNM_AIDX_TOP_ENTRY, /* entry function */
    UDPNM_HSM_AIDX_INVALID, /* no exit action */
    UDPNM_HSM_AIDX_INVALID, /* no do action */
    0U  /* no transitions */
  },
  {
    /* description of state BusSleepMode */
    /* non-deferred and non-ignored event mask */
   (1UL << UDPNM_HSM_UDPNM_EV_NETWORK_START)
    |   (1UL << UDPNM_HSM_UDPNM_EV_NET_REQ_STATUS_CHANGED)
    |   (1UL << UDPNM_HSM_UDPNM_EV_RX_INDICATION)
    | 0UL
    ,
      /* ignored event mask */
   (1UL << UDPNM_HSM_UDPNM_EV_COM_CONTROL)
    |   (1UL << UDPNM_HSM_UDPNM_EV_TX_CONFIRMATION)
    |   (1UL << UDPNM_HSM_UDPNM_EV_NM_TIMEOUT)
    |   (1UL << UDPNM_HSM_UDPNM_EV_MSG_CYCLE_TIMEOUT)
    |   (1UL << UDPNM_HSM_UDPNM_EV_REPEAT_MESSAGE_REASON)
    |   (1UL << UDPNM_HSM_UDPNM_EV_RMS_TIMEOUT)
    |   (1UL << UDPNM_HSM_UDPNM_EV_UNI_TIMEOUT)
    | 0UL
    ,
    UdpNm_HsmTrBusSleepMode, /* transitions for this state */
    UDPNM_HSM_UDPNM_SIDX_TOP, /* parent state */
    UDPNM_HSM_SIDX_INVALID, /* no init state */
      UDPNM_HSM_UDPNM_AIDX_BUSSLEEPMODE_ENTRY, /* entry function */
    UDPNM_HSM_AIDX_INVALID, /* no exit action */
    UDPNM_HSM_AIDX_INVALID, /* no do action */
    3 /* num of transitions */
  },
  {
    /* description of state NetworkMode */
    0U, /* no non-deferred event mask (no leaf state) */
    0U, /* no ignored event mask (no leaf state) */
    UdpNm_HsmTrNetworkMode, /* transitions for this state */
    UDPNM_HSM_UDPNM_SIDX_TOP, /* parent state */
    UDPNM_HSM_UDPNM_SIDX_SENDINGSUBMODE, /* init state */
      UDPNM_HSM_UDPNM_AIDX_NETWORKMODE_ENTRY, /* entry function */
    UDPNM_HSM_AIDX_INVALID, /* no exit action */
    UDPNM_HSM_AIDX_INVALID, /* no do action */
    4 /* num of transitions */
  },
  {
    /* description of state ReadySleepState */
    0U, /* no non-deferred event mask (no leaf state) */
    0U, /* no ignored event mask (no leaf state) */
    UdpNm_HsmTrReadySleepState, /* transitions for this state */
    UDPNM_HSM_UDPNM_SIDX_NETWORKMODE, /* parent state */
    UDPNM_HSM_UDPNM_SIDX_READYSLEEPREMOTEACTIVITY, /* init state */
      UDPNM_HSM_UDPNM_AIDX_READYSLEEPSTATE_ENTRY, /* entry function */
    UDPNM_HSM_AIDX_INVALID, /* no exit action */
    UDPNM_HSM_AIDX_INVALID, /* no do action */
    1 /* num of transitions */
  },
  {
    /* description of state ReadySleepRemoteActivity */
    /* non-deferred and non-ignored event mask */
   (1UL << UDPNM_HSM_UDPNM_EV_COM_CONTROL)
    |   (1UL << UDPNM_HSM_UDPNM_EV_TX_CONFIRMATION)
    |   (1UL << UDPNM_HSM_UDPNM_EV_NM_TIMEOUT)
    |   (1UL << UDPNM_HSM_UDPNM_EV_REPEAT_MESSAGE_REASON)
    |   (1UL << UDPNM_HSM_UDPNM_EV_NET_REQ_STATUS_CHANGED)
    |   (1UL << UDPNM_HSM_UDPNM_EV_RX_INDICATION)
    | 0UL
    ,
      /* ignored event mask */
   (1UL << UDPNM_HSM_UDPNM_EV_MSG_CYCLE_TIMEOUT)
    |   (1UL << UDPNM_HSM_UDPNM_EV_RMS_TIMEOUT)
    |   (1UL << UDPNM_HSM_UDPNM_EV_NETWORK_START)
    |   (1UL << UDPNM_HSM_UDPNM_EV_UNI_TIMEOUT)
    | 0UL
    ,
    UdpNm_HsmTrReadySleepRemoteActivity, /* transitions for this state */
    UDPNM_HSM_UDPNM_SIDX_READYSLEEPSTATE, /* parent state */
    UDPNM_HSM_SIDX_INVALID, /* no init state */
      UDPNM_HSM_UDPNM_AIDX_READYSLEEPREMOTEACTIVITY_ENTRY, /* entry function */
    UDPNM_HSM_AIDX_INVALID, /* no exit action */
    UDPNM_HSM_AIDX_INVALID, /* no do action */
    2 /* num of transitions */
  },
  {
    /* description of state ReadySleepRemoteSleep */
    /* non-deferred and non-ignored event mask */
   (1UL << UDPNM_HSM_UDPNM_EV_COM_CONTROL)
    |   (1UL << UDPNM_HSM_UDPNM_EV_TX_CONFIRMATION)
    |   (1UL << UDPNM_HSM_UDPNM_EV_NM_TIMEOUT)
    |   (1UL << UDPNM_HSM_UDPNM_EV_REPEAT_MESSAGE_REASON)
    |   (1UL << UDPNM_HSM_UDPNM_EV_NET_REQ_STATUS_CHANGED)
    |   (1UL << UDPNM_HSM_UDPNM_EV_RX_INDICATION)
    | 0UL
    ,
      /* ignored event mask */
   (1UL << UDPNM_HSM_UDPNM_EV_MSG_CYCLE_TIMEOUT)
    |   (1UL << UDPNM_HSM_UDPNM_EV_RMS_TIMEOUT)
    |   (1UL << UDPNM_HSM_UDPNM_EV_NETWORK_START)
    |   (1UL << UDPNM_HSM_UDPNM_EV_UNI_TIMEOUT)
    | 0UL
    ,
    UdpNm_HsmTrReadySleepRemoteSleep, /* transitions for this state */
    UDPNM_HSM_UDPNM_SIDX_READYSLEEPSTATE, /* parent state */
    UDPNM_HSM_SIDX_INVALID, /* no init state */
      UDPNM_HSM_AIDX_INVALID, /* no entry action */
    UDPNM_HSM_AIDX_INVALID, /* no exit action */
    UDPNM_HSM_AIDX_INVALID, /* no do action */
    3 /* num of transitions */
  },
  {
    /* description of state SendingSubMode */
    0U, /* no non-deferred event mask (no leaf state) */
    0U, /* no ignored event mask (no leaf state) */
    UdpNm_HsmTrSendingSubMode, /* transitions for this state */
    UDPNM_HSM_UDPNM_SIDX_NETWORKMODE, /* parent state */
    UDPNM_HSM_UDPNM_SIDX_REPEATMESSAGESTATE, /* init state */
      UDPNM_HSM_AIDX_INVALID, /* no entry action */
    UDPNM_HSM_UDPNM_AIDX_SENDINGSUBMODE_EXIT, /* exit function */
    UDPNM_HSM_AIDX_INVALID, /* no do action */
    2 /* num of transitions */
  },
  {
    /* description of state NormalOperationState */
    0U, /* no non-deferred event mask (no leaf state) */
    0U, /* no ignored event mask (no leaf state) */
    NULL_PTR,  /* no transitions */
    UDPNM_HSM_UDPNM_SIDX_SENDINGSUBMODE, /* parent state */
    UDPNM_HSM_UDPNM_SIDX_NORMALOPERATIONREMOTEACTIVITY, /* init state */
      UDPNM_HSM_UDPNM_AIDX_NORMALOPERATIONSTATE_ENTRY, /* entry function */
    UDPNM_HSM_AIDX_INVALID, /* no exit action */
    UDPNM_HSM_AIDX_INVALID, /* no do action */
    0U  /* no transitions */
  },
  {
    /* description of state NormalOperationRemoteActivity */
    /* non-deferred and non-ignored event mask */
   (1UL << UDPNM_HSM_UDPNM_EV_COM_CONTROL)
    |   (1UL << UDPNM_HSM_UDPNM_EV_TX_CONFIRMATION)
    |   (1UL << UDPNM_HSM_UDPNM_EV_NM_TIMEOUT)
    |   (1UL << UDPNM_HSM_UDPNM_EV_MSG_CYCLE_TIMEOUT)
    |   (1UL << UDPNM_HSM_UDPNM_EV_REPEAT_MESSAGE_REASON)
    |   (1UL << UDPNM_HSM_UDPNM_EV_NET_REQ_STATUS_CHANGED)
    |   (1UL << UDPNM_HSM_UDPNM_EV_RX_INDICATION)
    |   (1UL << UDPNM_HSM_UDPNM_EV_UNI_TIMEOUT)
    | 0UL
    ,
      /* ignored event mask */
   (1UL << UDPNM_HSM_UDPNM_EV_RMS_TIMEOUT)
    |   (1UL << UDPNM_HSM_UDPNM_EV_NETWORK_START)
    | 0UL
    ,
    UdpNm_HsmTrNormalOperationRemoteActivity, /* transitions for this state */
    UDPNM_HSM_UDPNM_SIDX_NORMALOPERATIONSTATE, /* parent state */
    UDPNM_HSM_SIDX_INVALID, /* no init state */
      UDPNM_HSM_UDPNM_AIDX_NORMALOPERATIONREMOTEACTIVITY_ENTRY, /* entry function */
    UDPNM_HSM_AIDX_INVALID, /* no exit action */
    UDPNM_HSM_AIDX_INVALID, /* no do action */
    4 /* num of transitions */
  },
  {
    /* description of state NormalOperationRemoteSleep */
    /* non-deferred and non-ignored event mask */
   (1UL << UDPNM_HSM_UDPNM_EV_COM_CONTROL)
    |   (1UL << UDPNM_HSM_UDPNM_EV_TX_CONFIRMATION)
    |   (1UL << UDPNM_HSM_UDPNM_EV_NM_TIMEOUT)
    |   (1UL << UDPNM_HSM_UDPNM_EV_MSG_CYCLE_TIMEOUT)
    |   (1UL << UDPNM_HSM_UDPNM_EV_REPEAT_MESSAGE_REASON)
    |   (1UL << UDPNM_HSM_UDPNM_EV_NET_REQ_STATUS_CHANGED)
    |   (1UL << UDPNM_HSM_UDPNM_EV_RX_INDICATION)
    | 0UL
    ,
      /* ignored event mask */
   (1UL << UDPNM_HSM_UDPNM_EV_RMS_TIMEOUT)
    |   (1UL << UDPNM_HSM_UDPNM_EV_NETWORK_START)
    |   (1UL << UDPNM_HSM_UDPNM_EV_UNI_TIMEOUT)
    | 0UL
    ,
    UdpNm_HsmTrNormalOperationRemoteSleep, /* transitions for this state */
    UDPNM_HSM_UDPNM_SIDX_NORMALOPERATIONSTATE, /* parent state */
    UDPNM_HSM_SIDX_INVALID, /* no init state */
      UDPNM_HSM_UDPNM_AIDX_NORMALOPERATIONREMOTESLEEP_ENTRY, /* entry function */
    UDPNM_HSM_AIDX_INVALID, /* no exit action */
    UDPNM_HSM_AIDX_INVALID, /* no do action */
    3 /* num of transitions */
  },
  {
    /* description of state RepeatMessageState */
    /* non-deferred and non-ignored event mask */
   (1UL << UDPNM_HSM_UDPNM_EV_COM_CONTROL)
    |   (1UL << UDPNM_HSM_UDPNM_EV_TX_CONFIRMATION)
    |   (1UL << UDPNM_HSM_UDPNM_EV_NM_TIMEOUT)
    |   (1UL << UDPNM_HSM_UDPNM_EV_MSG_CYCLE_TIMEOUT)
    |   (1UL << UDPNM_HSM_UDPNM_EV_RMS_TIMEOUT)
    |   (1UL << UDPNM_HSM_UDPNM_EV_NET_REQ_STATUS_CHANGED)
    |   (1UL << UDPNM_HSM_UDPNM_EV_RX_INDICATION)
    | 0UL
    ,
      /* ignored event mask */
   (1UL << UDPNM_HSM_UDPNM_EV_REPEAT_MESSAGE_REASON)
    |   (1UL << UDPNM_HSM_UDPNM_EV_NETWORK_START)
    |   (1UL << UDPNM_HSM_UDPNM_EV_UNI_TIMEOUT)
    | 0UL
    ,
    UdpNm_HsmTrRepeatMessageState, /* transitions for this state */
    UDPNM_HSM_UDPNM_SIDX_SENDINGSUBMODE, /* parent state */
    UDPNM_HSM_SIDX_INVALID, /* no init state */
      UDPNM_HSM_UDPNM_AIDX_REPEATMESSAGESTATE_ENTRY, /* entry function */
    UDPNM_HSM_UDPNM_AIDX_REPEATMESSAGESTATE_EXIT, /* exit function */
    UDPNM_HSM_AIDX_INVALID, /* no do action */
    2 /* num of transitions */
  },
  {
    /* description of state PrepareBusSleepMode */
    /* non-deferred and non-ignored event mask */
   (1UL << UDPNM_HSM_UDPNM_EV_NETWORK_START)
    |   (1UL << UDPNM_HSM_UDPNM_EV_NET_REQ_STATUS_CHANGED)
    |   (1UL << UDPNM_HSM_UDPNM_EV_RX_INDICATION)
    |   (1UL << UDPNM_HSM_UDPNM_EV_UNI_TIMEOUT)
    | 0UL
    ,
      /* ignored event mask */
   (1UL << UDPNM_HSM_UDPNM_EV_COM_CONTROL)
    |   (1UL << UDPNM_HSM_UDPNM_EV_TX_CONFIRMATION)
    |   (1UL << UDPNM_HSM_UDPNM_EV_NM_TIMEOUT)
    |   (1UL << UDPNM_HSM_UDPNM_EV_MSG_CYCLE_TIMEOUT)
    |   (1UL << UDPNM_HSM_UDPNM_EV_REPEAT_MESSAGE_REASON)
    |   (1UL << UDPNM_HSM_UDPNM_EV_RMS_TIMEOUT)
    | 0UL
    ,
    UdpNm_HsmTrPrepareBusSleepMode, /* transitions for this state */
    UDPNM_HSM_UDPNM_SIDX_TOP, /* parent state */
    UDPNM_HSM_SIDX_INVALID, /* no init state */
      UDPNM_HSM_UDPNM_AIDX_PREPAREBUSSLEEPMODE_ENTRY, /* entry function */
    UDPNM_HSM_AIDX_INVALID, /* no exit action */
    UDPNM_HSM_AIDX_INVALID, /* no do action */
    4 /* num of transitions */
  }
};

#if (UDPNM_HSM_TRACE == STD_ON)
STATIC CONSTP2CONST(char, UDPNM_CONST, UDPNM_CONST)
  UdpNm_HsmUdpNmStateNames[] =
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

STATIC CONSTP2CONST(char, UDPNM_CONST, UDPNM_CONST)
  UdpNm_HsmUdpNmActionNames[] =
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

STATIC CONSTP2CONST(char, UDPNM_CONST, UDPNM_CONST)
  UdpNm_HsmUdpNmGuardNames[] =
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

STATIC CONSTP2CONST(char, UDPNM_CONST, UDPNM_CONST)
  UdpNm_HsmUdpNmEventNames[] =
{
  "COM_CONTROL",
  "TX_CONFIRMATION",
  "NM_TIMEOUT",
  "MSG_CYCLE_TIMEOUT",
  "REPEAT_MESSAGE_REASON",
  "RMS_TIMEOUT",
  "NETWORK_START",
  "NET_REQ_STATUS_CHANGED",
  "RX_INDICATION",
  "UNI_TIMEOUT",
};
#endif
#define UDPNM_STOP_SEC_CONST_UNSPECIFIED
#include <UdpNm_MemMap.h>

/*==================[external constants]====================================*/
#if (UDPNM_ADAPTIVE == STD_OFF)
#define UDPNM_START_SEC_CONST_UNSPECIFIED
#include <UdpNm_MemMap.h>
/* Top level state chart description data */
CONST(UdpNm_HsmStatechartType, UDPNM_CONST)
  UdpNm_HsmScUdpNm =
{
  UdpNm_HsmScdUdpNm, /* state description array */
  UdpNm_HsmInstUdpNm, /* instances RAM */
  UdpNm_HsmUdpNmActions, /* actions */
  UdpNm_HsmUdpNmGuards, /* guards */
  UdpNm_HsmEvCountersUdpNm, /* event counters RAM */
  UdpNm_HsmEvQueuesUdpNm, /* event queues RAM */
#if (UDPNM_HSM_TRACE == STD_ON)
  "UdpNm", /* name of the HSM */
  UdpNm_HsmUdpNmStateNames,
  UdpNm_HsmUdpNmActionNames,
  UdpNm_HsmUdpNmGuardNames,
  UdpNm_HsmUdpNmEventNames,
#endif
  UDPNM_HSM_UDPNM_NUM_EVENTS, /* num of events */
  0U, /* state chart id */
  UDPNM_HSM_UDPNM_NUM_INST, /* num of instances */
  UDPNM_HSM_UDPNM_SIDX_TOP /* top state index */
};


#define UDPNM_STOP_SEC_CONST_UNSPECIFIED
#include <UdpNm_MemMap.h>
#else /* #if (UDPNM_ADAPTIVE == STD_OFF) */

#define UDPNM_START_SEC_VAR_INIT_UNSPECIFIED
#include <UdpNm_MemMap.h>
/* Top level state chart description data */
VAR(UdpNm_HsmStatechartType, UDPNM_APPL_DATA)
  UdpNm_HsmScUdpNm =
{
  UdpNm_HsmScdUdpNm, /* state description array */
  NULL_PTR,
  UdpNm_HsmUdpNmActions, /* actions */
  UdpNm_HsmUdpNmGuards, /* guards */
  NULL_PTR,
  NULL_PTR,
#if (UDPNM_HSM_TRACE == STD_ON)
  "UdpNm", /* name of the HSM */
  UdpNm_HsmUdpNmStateNames,
  UdpNm_HsmUdpNmActionNames,
  UdpNm_HsmUdpNmGuardNames,
  UdpNm_HsmUdpNmEventNames,
#endif
  UDPNM_HSM_UDPNM_NUM_EVENTS, /* num of events */
  0U, /* state chart id */
  0U, /* num of instances */
  UDPNM_HSM_UDPNM_SIDX_TOP /* top state index */
};
#define UDPNM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <UdpNm_MemMap.h>
#endif

/*==================[internal function definitions]=========================*/

/*==================[external function definitions]=========================*/

/*==================[end of file]===========================================*/
