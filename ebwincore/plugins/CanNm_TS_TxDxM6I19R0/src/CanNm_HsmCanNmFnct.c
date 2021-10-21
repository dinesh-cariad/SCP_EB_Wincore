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

/* Implementation of the state handler functions of the state machine
 * CanNm.
 *
 * This file contains the implementation of the state functions.  It
 * is generated but must be edited to implement the real actions.  If the
 * state machine model is updated and the code generator regenerates the
 * files the user must manually merge the changes with any existing hand
 * written code.
 */

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 20.7 (required)
 * Expressions resulting from the expansion of macro parameters shall be enclosed in parentheses.
 *
 * Reason:
 * The macro is used in function parameter declarations and definitions where the number of
 * parentheses matter.
 */

/* CHECK: RULE 301 OFF (this file is partly generated, some lines may be
 * longer then 100 characters) */

/*==================[inclusions]============================================*/
#define CANNM_NO_CFGCLASSMIX_REQUIRED
#ifndef CANNM_NO_PBCFG_REQUIRED
#define CANNM_NO_PBCFG_REQUIRED
#endif

#include <CanNm_Trace.h>
#include <TSMem.h>
#include <CanNm_Hsm.h>                  /* state machine driver interface */
#include <CanNm_HsmCanNm.h>             /* public statechart model definitions */
#include <CanNm_HsmCanNmInt.h>          /* internal statechart model definitions */
#include <CanNm_Int.h>                  /* Module intenal definitions */

/* !LINKSTO CANNM312,1 */
#include <CanIf.h>                      /* CanIf API, CanIf_Transmit() */
/* !LINKSTO CANNM307,1 */
#include <Nm_Cbk.h>                     /* Declaration of Nm callback APIs */
#include <SchM_CanNm.h>                 /* SchM-header for CanNm */

#if (CANNM_DEV_ERROR_DETECT == STD_ON)
/* !LINKSTO CANNM308,1 */
#include <Det.h>                        /* Det API */
#endif

#if (CANNM_PN_SUPPORTED == STD_ON)
#include <CanSM_TxTimeoutException.h>   /* Callback function CanSM_TxTimeoutException() */
#endif

/*==================[macros]================================================*/
#if (defined CANNM_PL_TIMER)
#error CANNM_PL_TIMER is already defined
#endif

#if (CANNM_HSM_INST_MULTI_ENABLED == STD_ON)
#define CANNM_PL_TIMER(a, b)          a, b
#else
#define CANNM_PL_TIMER(a, b)          b
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define CANNM_START_SEC_CODE
#include <CanNm_MemMap.h>

/** \brief Initialize Internal variables of respective channel.
 ** \param instIdx index of state machine instance to work on */
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, CANNM_CODE) CanNm_InitIntVar(
  CANNM_PDL_SF(uint8 instIdx));

/** \brief Start Nm Timeout Timer.
 ** \param instIdx index of state machine instance to work on */
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, CANNM_CODE) CanNm_NmTimerStart(
  CANNM_PL_TIMER(uint8 instIdx, CanNm_TimeType CanNmTime));

/** \brief Stop Nm Timeout Timer.
 ** \param instIdx index of state machine instance to work on */
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, CANNM_CODE) CanNm_NmTimerStop(
  CANNM_PDL_SF(uint8 instIdx));

/** \brief Start Universal Timer.
 ** \param instIdx index of state machine instance to work on
 ** \param UniTimer Universal Timer value */
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, CANNM_CODE) CanNm_UniTimerStart(
  CANNM_PL_TIMER(uint8 instIdx, CanNm_TimeType UniTimer));

/** \brief Stop Universal Timer.
 ** \param instIdx index of state machine instance to work on */
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, CANNM_CODE) CanNm_UniTimerStop(
  CANNM_PDL_SF(uint8 instIdx));

/* Deviation MISRAC2012-1 */
STATIC FUNC(void, CANNM_CODE) CanNm_RmsTimerStart(
  CANNM_PL_TIMER(uint8 instIdx, CanNm_TimeType RmsTime));


#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
/** \brief Start CanNm Message Cycle Timer.
 ** \param instIdx index of state machine instance to work on
 ** \param MsgCycleTimer CanNm Message Cycle Timer value */
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, CANNM_CODE) CanNm_MsgCycleTimerStart(
  CANNM_PL_TIMER(uint8 instIdx, CanNm_TimeType MsgCycleTimer));

/** \brief Stop CanNm Message Cycle Timer.
 ** \param instIdx index of state machine instance to work on */
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, CANNM_CODE) CanNm_MsgCycleTimerStop(
  CANNM_PDL_SF(uint8 instIdx));
#endif

/** \brief Handle the common actions on event Rx Indication in
 **        all sub-states in Network Mode
 ** \param instIdx index of state machine instance to work on */
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, CANNM_CODE) CanNm_HandleRxIndicationCommon(
  CANNM_PDL_SF(uint8 instIdx));

/** \brief Handle the action on event Rx Indication in
 **        Normal Operation Remote Active state
 ** \param instIdx index of state machine instance to work on */
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, CANNM_CODE) CanNm_HandleRxIndication_NOState_RA(
  CANNM_PDL_SF(uint8 instIdx));

/** \brief Handle the action on event Rx Indication in
 **        Normal Operation Remote Sleep state
 ** \param instIdx index of state machine instance to work on */
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, CANNM_CODE) CanNm_HandleRxIndication_NOState(
  CANNM_PDL_SF(uint8 instIdx));

#if ((CANNM_PASSIVE_MODE_ENABLED == STD_OFF) && (CANNM_IMMEDIATE_TXCONF_ENABLED == STD_OFF))
/** \brief Handle the action on event Tx Confirmation
 ** \param instIdx index of state machine instance to work on */
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, CANNM_CODE) CanNm_HandleTxConfirmation(
  CANNM_PDL_SF(uint8 instIdx));
#endif

#if ((CANNM_COM_CONTROL_ENABLED == STD_ON) && (CANNM_PASSIVE_MODE_ENABLED == STD_OFF))
  /** \brief Handle the action on event communication ability.
 ** \param instIdx index of state machine instance to work on */
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, CANNM_CODE) CanNm_HandleComControl(
 CANNM_PDL_SF(uint8 instIdx));
#endif

/** \brief Transmit Nm message.
 ** \param instIdx index of state machine instance to work on */
/* Deviation MISRAC2012-1 */
#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
FUNC(void, CANNM_CODE) CanNm_HandleTransmit(
  CANNM_PDL_SF(uint8 instIdx));
#endif

#if (CANNM_STATE_CHANGE_IND_ENABLED == STD_ON)
STATIC FUNC(void, CANNM_CODE) CanNm_Call_Nm_StateChangeNotification
(
  uint8 instIdx,
  Nm_StateType nmPreviousState,
  Nm_StateType nmCurrentState,
  boolean      compare
);
#endif
#define CANNM_STOP_SEC_CODE
#include <CanNm_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#define CANNM_START_SEC_CODE
#include <CanNm_MemMap.h>

/* ************************ state functions ******************************* */

/* ************************************************************************
 * State: TOP
 * Parent state: none
 * Init substate: BusSleepMode
 * Transitions originating from this state:
 */

/* Deviation MISRAC2012-1 */
FUNC(void, CANNM_CODE) CanNm_HsmCanNmSfTOPEntry(
  CANNM_PDL_SF(const uint8 instIdx))
{
  /* entry action 'InitIntVar();TxTimerStop(); NmTimerStop(); UniTimerStop(); MsgCycleTimerStop();' */
  CanNm_InitIntVar(CANNM_PL_SF(instIdx));
#if ((CANNM_PASSIVE_MODE_ENABLED == STD_OFF)            \
  && (CANNM_IMMEDIATE_TXCONF_ENABLED == STD_OFF))
  CANNM_TX_TIMER_STOP(instIdx);
#endif
  /* !LINKSTO CANNM174,1 */
  CanNm_NmTimerStop(CANNM_PL_SF(instIdx));
  /* !LINKSTO CANNM175,1 */
  CanNm_UniTimerStop(CANNM_PL_SF(instIdx));
#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
  /* !LINKSTO CANNM173,1 */
  CanNm_MsgCycleTimerStop(CANNM_PL_SF(instIdx));
#endif
}

/* ************************************************************************
 * State: BusSleepMode
 * Parent state: TOP
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 * 1) RX_INDICATION[]/Nm_NetworkStartIndication(); #ifdef (...) Nm_PduRxIndication; #endif #if(Det == STD_ON);DET_REPORT_ERROR();#endif --> CANNM127 CANNM037 CANNM336 CANNM337
 * 2) BusSleepMode -> NetworkMode: NETWORK_START[]/
 * 3) BusSleepMode -> NetworkMode: NET_REQ_STATUS_CHANGED[NetworkRequested==TRUE]/
 */

/* Deviation MISRAC2012-1 */
FUNC(void, CANNM_CODE) CanNm_HsmCanNmSfBusSleepModeEntry(
  CANNM_PDL_SF(const uint8 instIdx))
{
  /* entry action '#ifdef (SCI) Nm_StateChangeNotification(ActState, BusSleepState);CurState = BusSleepState; Nm_BusSleepMode(); --> CANNM126' */
#if (CANNM_ACTIVE_WAKEUP_BIT_ENABLED == STD_ON)
  SchM_Enter_CanNm_SCHM_CANNM_EXCLUSIVE_AREA_0();
  CANNM_CHANNEL_STATUS(instIdx).ActiveWakeUp1 = 0U;
  CANNM_CHANNEL_STATUS(instIdx).ActiveWakeUp2 = 0U;
  SchM_Exit_CanNm_SCHM_CANNM_EXCLUSIVE_AREA_0();
#endif /* CANNM_ACTIVE_WAKEUP_BIT_ENABLED == STD_ON */

#if (CANNM_STATE_CHANGE_IND_ENABLED == STD_ON)
  CanNm_Call_Nm_StateChangeNotification(CANNM_INST(instIdx),
                                        CANNM_CHANNEL_STATUS(instIdx).CurState,
                                        NM_STATE_BUS_SLEEP,
                                        FALSE);
#endif
  if (CANNM_CHANNEL_STATUS(instIdx).CurState == NM_STATE_PREPARE_BUS_SLEEP)
  {
    /* !LINKSTO CANNM126,1, CANNM093,1, CANNM324,1 */
#if (CANNM_MULTICORE_ENABLED == STD_OFF)
    CANNM_CALL_NM_BUSSLEEPMODE(CANNM_INST(instIdx));
#else
    if (CANNM_CALL_NM_BUSSLEEPMODE(CANNM_INST(instIdx)) != SCHM_E_OK)
    {
#if (CANNM_DEV_ERROR_DETECT == STD_ON)
      /* !LINKSTO CanNm.EB.SchM_Call_DET,1 */
      CANNM_DET_REPORT_ERROR(CANNM_INST(instIdx),
                             CANNM_SERVID_MAINFUNCTION_X,
                             CANNM_E_BUSSLEEPMODE);
#endif
    }
#endif
  }
  CANNM_CHANNEL_STATUS(instIdx).CurState = NM_STATE_BUS_SLEEP;
}
/* Deviation MISRAC2012-1 */
FUNC(void, CANNM_CODE) CanNm_HsmCanNmSfBusSleepModeAction1(
  CANNM_PDL_SF(const uint8 instIdx))
{
  /* action 'Nm_NetworkStartIndication(); #ifdef (...) Nm_PduRxIndication; #endif #if(Det == STD_ON);DET_REPORT_ERROR();#endif --> CANNM127 CANNM037 CANNM336 CANNM337'
   * for RX_INDICATION[]/...
   * internal transition */
  /* !LINKSTO CANNM127,1, CanNm.EB.Dsn.StateDynamics.Internal_Transition_BUS_SLEEP_MODE,1 */
#if (CANNM_MULTICORE_ENABLED == STD_OFF)
  CANNM_CALL_NM_NETWORKSTARTINDICATION(CANNM_INST(instIdx));
#else
  if (CANNM_CALL_NM_NETWORKSTARTINDICATION(CANNM_INST(instIdx)) != SCHM_E_OK)
  {
#if (CANNM_DEV_ERROR_DETECT == STD_ON)
    /* !LINKSTO CanNm.EB.SchM_Call_DET,1 */
    CANNM_DET_REPORT_ERROR(CANNM_INST(instIdx),
                           CANNM_SERVID_MAINFUNCTION_X,
                           CANNM_E_NETWORKSTARTINDICATION);
#endif
  }
#endif

#if (CANNM_PDU_RX_INDICATION_ENABLED == STD_ON)
  /* !LINKSTO CANNM037,1 */
#if (CANNM_MULTICORE_ENABLED == STD_OFF)
  CANNM_CALL_NM_PDURXINDICATION(CANNM_INST(instIdx));
#else
  if (CANNM_CALL_NM_PDURXINDICATION(CANNM_INST(instIdx)) != SCHM_E_OK)
  {
#if (CANNM_DEV_ERROR_DETECT == STD_ON)
    /* !LINKSTO CanNm.EB.SchM_Call_DET,1 */
    CANNM_DET_REPORT_ERROR(CANNM_INST(instIdx),
                           CANNM_SERVID_MAINFUNCTION_X,
                           CANNM_E_PDURXINDICATION);
#endif
  }
#endif
#endif
#if (CANNM_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO CANNM336,1, CANNM337,1 */
  (void)Det_ReportError
  (CANNM_MODULE_ID, CANNM_INSTANCE_ID,
   CANNM_SERVID_RXINDICATION, CANNM_E_NET_START_IND);
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(boolean, CANNM_CODE) CanNm_HsmCanNmSfBusSleepModeGuard3(
  CANNM_PDL_SF(const uint8 instIdx))
{
  boolean ret;
  /* guard condition 'NetworkRequested==TRUE'
   * for NET_REQ_STATUS_CHANGED[...]/
   * external transition to state NetworkMode */
  /* !LINKSTO CANNM129,1 */
  /* !LINKSTO CanNm.EB.Dsn.StateDynamics.NET_REQSTATUS_CHANGED_BUS_SLEEP_MODE_NetworkRequest,1 */
  /* !LINKSTO CanNm.EB.Dsn.StateDynamics.NET_REQSTATUS_CHANGED_BUS_SLEEP_MODE_NetworkGwEraRequest,1 */
  ret = (boolean)(((CANNM_CHANNEL_STATUS(instIdx).ChanStatus &
                     CANNM_NETWORK_REQUESTED) != 0U) ? TRUE : FALSE);
  return ret;
}

/* ************************************************************************
 * State: NetworkMode
 * Parent state: TOP
 * Init substate: SendingSubMode
 * Transitions originating from this state:
 * 1) COM_CONTROL[]/#if (CANNM_COM_CONTROL_ENABLED == STD_ON)
 *    HandleComControl()
 * 2) RX_INDICATION[]/HandleRxIndicationCommon()
 * 3) TX_CONFIRMATION[]/HandleTxConfirmation();
 */

/* Deviation MISRAC2012-1 */
FUNC(void, CANNM_CODE) CanNm_HsmCanNmSfNetworkModeEntry(
  CANNM_PDL_SF(const uint8 instIdx))
{
  /* entry action '#if (!(PASSIVE_MODE == true)) && (ChanStatus & CANNM_NETWORK_REQUESTED) &&(IMM_TRAN_ENBLD)
   * {Chanstatus &= IMM_TRANS_ACTIVE} */
  /* entry action 'NmTimerStart(CANNM_TIMEOUT_TIME) --> CANNM096; Nm_NetworkMode(); -->CANNM97; if(ChanStatus & NETWORK_REQUESTED) SetActiveWakeupBit() -->CANNM401; */
  /* !LINKSTO CanNm.EB.NmTimer,1, CANNM096,1 */
  if ((CANNM_CHANNEL_STATUS(instIdx).ChanStatus & CANNM_COM_DISABLED) == 0U)
  {
    /* !LINKSTO CANNM206,1 */
    /* Deviation MISRAC2012-1 */
    CanNm_NmTimerStart(CANNM_PL_TIMER(instIdx, CANNM_CHANNEL_CONFIG(instIdx).CanNmTime));
  }
  /* !LINKSTO CANNM097,1, CANNM093,1 */
#if (CANNM_MULTICORE_ENABLED == STD_OFF)
  CANNM_CALL_NM_NETWORKMODE(CANNM_INST(instIdx));
#else
  if (CANNM_CALL_NM_NETWORKMODE(CANNM_INST(instIdx)) != SCHM_E_OK)
  {
#if (CANNM_DEV_ERROR_DETECT == STD_ON)
    /* !LINKSTO CanNm.EB.SchM_Call_DET,1 */
    CANNM_DET_REPORT_ERROR(CANNM_INST(instIdx),
                           CANNM_SERVID_MAINFUNCTION_X,
                           CANNM_E_NETWORKMODE);
#endif
  }
#endif
#if (CANNM_ACTIVE_WAKEUP_BIT_ENABLED == STD_ON)
  /* !LINKSTO CanNm.EB.CanNm_NetworkGwEraRequest.ActiveWakeupBit.BusSleepMode,1 */
  /* !LINKSTO CanNm.EB.CanNm_NetworkGwEraRequest.ActiveWakeupBit.PrepareBusSleepMode,1 */
  /* !LINKSTO CanNm.EB.PassiveStartup.NetworkRequest.Sequence.ActiveWakeupBit.BusSleepMode,1 */
  /* !LINKSTO CanNm.EB.PassiveStartup.NetworkRequest.Sequence.ActiveWakeupBit.PrepareBusSleepMode,1 */
  if((CANNM_CHANNEL_STATUS(instIdx).ActiveWakeUp1 == CANNM_NETWORK_REQUEST_AWB) &&
     ((CANNM_CHANNEL_STATUS(instIdx).CurState == NM_STATE_PREPARE_BUS_SLEEP) ||
      (CANNM_CHANNEL_STATUS(instIdx).CurState == NM_STATE_BUS_SLEEP))
    )
  {
    /* !LINKSTO EB_CANNM401_Refine_1,1, EB_CANNM401_Refine_2,1 */
    TS_AtomicSetBit_8(&CANNM_CHANNEL_STATUS(instIdx).TxPduPtr[CANNM_CHANNEL_CONFIG(instIdx).CbvPos],
        CANNM_CBV_ACTIVEWAKEUPBIT);
  }
#endif

#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
  CANNM_CHANNEL_STATUS(instIdx).NmTimerExpired = FALSE;
  CANNM_CHANNEL_STATUS(instIdx).FirstCanIfTransmitOk = FALSE;
#endif
  CANNM_CHANNEL_STATUS(instIdx).MessageFlags |= (uint8)CANNM_RMS_ENTERED_FROM_NM;

}
/* Deviation MISRAC2012-1 */
FUNC(void, CANNM_CODE) CanNm_HsmCanNmSfNetworkModeAction1(
  CANNM_PDL_SF(const uint8 instIdx))
{
  /* action '#if (CANNM_COM_CONTROL_ENABLED == STD_ON) HandleComControl()'
   * for COM_CONTROL[]/...
   * internal transition */
  /* !LINKSTO CanNm.EB.Dsn.StateDynamics.Internal_Transition_NETWORK_MODE_0,1 */
  /* !LINKSTO CanNm.EB.Dsn.StateDynamics.Internal_Transition_NETWORK_MODE_1,1 */
#if ((CANNM_COM_CONTROL_ENABLED == STD_ON) && (CANNM_PASSIVE_MODE_ENABLED == STD_OFF))
  CanNm_HandleComControl(CANNM_PL_SF(instIdx));
#elif (CANNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(void, CANNM_CODE) CanNm_HsmCanNmSfNetworkModeAction2(
  CANNM_PDL_SF(const uint8 instIdx))
{
  /* action 'HandleRxIndicationCommon()'
   * for RX_INDICATION[]/...
   * internal transition */
  /* !LINKSTO CanNm.EB.Dsn.StateDynamics.Internal_Transition_NETWORK_MODE_2,1 */
  CanNm_HandleRxIndicationCommon(CANNM_PL_SF(instIdx));
}
/* Deviation MISRAC2012-1 */
FUNC(void, CANNM_CODE) CanNm_HsmCanNmSfNetworkModeAction3(
  CANNM_PDL_SF(const uint8 instIdx))
{
  /* action 'HandleTxConfirmation();'
   * for TX_CONFIRMATION[]/...
   * internal transition */
  /* !LINKSTO CanNm.EB.Dsn.StateDynamics.Internal_Transition_NETWORK_MODE_3,1 */
#if ((CANNM_PASSIVE_MODE_ENABLED == STD_OFF) && (CANNM_IMMEDIATE_TXCONF_ENABLED == STD_OFF))
  CanNm_HandleTxConfirmation(CANNM_PL_SF(instIdx));
#elif (CANNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif

}
/* Deviation MISRAC2012-1 */
FUNC(boolean, CANNM_CODE) CanNm_HsmCanNmSfNetworkModeGuard4(
  CANNM_PDL_SF(const uint8 instIdx))
{
  boolean ret = FALSE;
  /* guard condition 'PnHandleMultipleNetworkRequests==TRUE && NetworkRequested==TRUE'
   * for NET_REQ_STATUS_CHANGED[...]/
   * external transition to state NetworkMode */
#if (CANNM_PN_SUPPORTED == STD_ON)
  /* !LINKSTO CANNM444_Refine,1, CanNm.EB.Dsn.StateDynamics.NET_REQ_STATUS_CHANGED_NETWORK_MODE,1 */
  if ((CANNM_CHANNEL_PN_HANDLE_MULTI_REQ(instIdx) == TRUE)
      && ((CANNM_CHANNEL_STATUS(instIdx).ChanStatus & CANNM_NETWORK_REQUESTED) != 0U))
  {
    ret = TRUE;
  }
  else
  {
    ret = FALSE;
  }
#elif (CANNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif

  return ret;
}

/* ************************************************************************
 * State: ReadySleepState
 * Parent state: NetworkMode
 * Init substate: ReadySleepRemoteActivity
 * Transitions originating from this state:
 * 1) ReadySleepState -> PrepareBusSleepMode: NM_TIMEOUT[NmTimer == 0]/
 */

/* Deviation MISRAC2012-1 */
FUNC(void, CANNM_CODE) CanNm_HsmCanNmSfReadySleepStateEntry(
  CANNM_PDL_SF(const uint8 instIdx))
{
  /* entry action '#ifdef (SCI) Nm_StateChangeNotification(LastState, ActState) --> CANNM166;CurState = ReadySleepState;' */

#if (CANNM_STATE_CHANGE_IND_ENABLED == STD_ON)
  CanNm_Call_Nm_StateChangeNotification(CANNM_INST(instIdx),
                                        CANNM_CHANNEL_STATUS(instIdx).CurState,
                                        NM_STATE_READY_SLEEP,
                                        TRUE);
#endif
  CANNM_CHANNEL_STATUS(instIdx).CurState = NM_STATE_READY_SLEEP;
}
/* Deviation MISRAC2012-1 */
FUNC(boolean, CANNM_CODE) CanNm_HsmCanNmSfReadySleepStateGuard1(
  CANNM_PDL_SF(const uint8 instIdx))
{
  boolean ret;
  /* guard condition 'NmTimer == 0'
   * for NM_TIMEOUT[...]/
   * external transition to state PrepareBusSleepMode */
  /* !LINKSTO CANNM109,1, CanNm.EB.Dsn.StateDynamics.NM_TIMEOUT_READY_SLEEP_STATE,1 */
  ret = (boolean)((CANNM_CHANNEL_STATUS(instIdx).CanNmTimer == 0U) ? TRUE : FALSE);
  return ret;
}

/* ************************************************************************
 * State: ReadySleepRemoteActivity
 * Parent state: ReadySleepState
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 * 1) ReadySleepRemoteActivity -> NormalOperationRemoteActivity: NET_REQ_STATUS_CHANGED[PnHandleMultipleNetworkRequests==FALSE && NetworkRequested==TRUE]/CanNm_MsgCycleTimerStart(MsgCycleOffset)
 * 2) ReadySleepRemoteActivity -> RepeatMessageState: REPEAT_MESSAGE_REASON[]/
 */

/* Deviation MISRAC2012-1 */
FUNC(void, CANNM_CODE) CanNm_HsmCanNmSfReadySleepRemoteActivityEntry(
  CANNM_PDL_SF(const uint8 instIdx))
{
  /* entry action '#ifdef (RSI) clear RSI flag' */
#if (CANNM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
  /* clear RSI Status */
  TS_AtomicClearBit_8(&CANNM_CHANNEL_STATUS(instIdx).ChanStatus, CANNM_RSI_TRUE_BIT);
#elif (CANNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(boolean, CANNM_CODE) CanNm_HsmCanNmSfReadySleepRemoteActivityGuard1(
  CANNM_PDL_SF(const uint8 instIdx))
{
  boolean ret;
  /* guard condition 'PnHandleMultipleNetworkRequests==FALSE && NetworkRequested==TRUE'
   * for NET_REQ_STATUS_CHANGED[...]/CanNm_MsgCycleTimerStart(MsgCycleOffset)
   * external transition to state NormalOperationRemoteActivity */
  /* !LINKSTO CanNm.EB.Dsn.StateDynamics.NET_REQ_STATUS_CHANGED_READY_SLEEP_STATE,1 */
  /* !LINKSTO CANNM110,1 */
#if (CANNM_PN_SUPPORTED == STD_ON)
  if ((CANNM_CHANNEL_PN_HANDLE_MULTI_REQ(instIdx) == FALSE)
      && ((CANNM_CHANNEL_STATUS(instIdx).ChanStatus & CANNM_NETWORK_REQUESTED) != 0U))
#else
  if ((CANNM_CHANNEL_STATUS(instIdx).ChanStatus & CANNM_NETWORK_REQUESTED) != 0U)
#endif
  {
    ret = TRUE;
  }
  else
  {
    ret = FALSE;
  }

  return ret;
}
/* Deviation MISRAC2012-1 */
FUNC(void, CANNM_CODE) CanNm_HsmCanNmSfReadySleepRemoteActivityAction1(
  CANNM_PDL_SF(const uint8 instIdx))
{
  /* action 'CanNm_MsgCycleTimerStart(MsgCycleOffset)'
   * for NET_REQ_STATUS_CHANGED[PnHandleMultipleNetworkRequests==FALSE && NetworkRequested==TRUE]/...
   * external transition to state NormalOperationRemoteActivity */
#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
   CanNm_MsgCycleTimerStart(
     /* Deviation MISRAC2012-1 */
     CANNM_PL_TIMER(instIdx, CANNM_CHANNEL_MSGCYCLEOFFSET(CANNM_INST(instIdx))));
#elif (CANNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}

/* ************************************************************************
 * State: ReadySleepRemoteSleep
 * Parent state: ReadySleepState
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 * 1) ReadySleepRemoteSleep -> NormalOperationRemoteSleep: NET_REQ_STATUS_CHANGED[PnHandleMultipleNetworkRequests==FALSE && NetworkRequested==TRUE]/
 * 2) ReadySleepRemoteSleep -> RepeatMessageState: REPEAT_MESSAGE_REASON[]/Nm_RemoteSleepCancellation();
 * 3) ReadySleepRemoteSleep -> ReadySleepRemoteActivity: RX_INDICATION[]/Nm_RemoteSleepCancellation(); HandleRxIndicationCommon();
 */

/* Deviation MISRAC2012-1 */
FUNC(boolean, CANNM_CODE) CanNm_HsmCanNmSfReadySleepRemoteSleepGuard1(
  CANNM_PDL_SF(const uint8 instIdx))
{
  boolean ret;
  /* guard condition 'PnHandleMultipleNetworkRequests==FALSE && NetworkRequested==TRUE'
   * for NET_REQ_STATUS_CHANGED[...]/CanNm_MsgCycleTimerStart(MsgCycleOffset)
   * external transition to state NormalOperationRemoteSleep */
  /* !LINKSTO CanNm.EB.Dsn.StateDynamics.NET_REQ_STATUS_CHANGED_READY_SLEEP_STATE,1 */
#if (CANNM_PN_SUPPORTED == STD_ON)
  if ((CANNM_CHANNEL_PN_HANDLE_MULTI_REQ(instIdx) == FALSE)
      && ((CANNM_CHANNEL_STATUS(instIdx).ChanStatus & CANNM_NETWORK_REQUESTED) != 0U))
#else
  if ((CANNM_CHANNEL_STATUS(instIdx).ChanStatus & CANNM_NETWORK_REQUESTED) != 0U)
#endif
  {
    ret = TRUE;
  }
  else
  {
    ret = FALSE;
  }

  return ret;
}
/* Deviation MISRAC2012-1 */
FUNC(void, CANNM_CODE) CanNm_HsmCanNmSfReadySleepRemoteSleepAction1(
  CANNM_PDL_SF(const uint8 instIdx))
{
  /* action 'CanNm_MsgCycleTimerStart(MsgCycleOffset)'
   * for NET_REQ_STATUS_CHANGED[PnHandleMultipleNetworkRequests==FALSE && NetworkRequested==TRUE]/...
   * external transition to state NormalOperationRemoteSleep */
#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
  CanNm_MsgCycleTimerStart(
    /* Deviation MISRAC2012-1 */
    CANNM_PL_TIMER(instIdx, CANNM_CHANNEL_MSGCYCLEOFFSET(CANNM_INST(instIdx))));
#elif (CANNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(void, CANNM_CODE) CanNm_HsmCanNmSfReadySleepRemoteSleepAction2(
  CANNM_PDL_SF(const uint8 instIdx))
{
  /* action 'Nm_RemoteSleepCancellation();'
   * for REPEAT_MESSAGE_REASON[]/...
   * external transition to state RepeatMessageState */
  /* !LINKSTO EB_CANNM152_Refine_2,1, CANNM111,1, CANNM112,1 */
  /* !LINKSTO CanNm.EB.Dsn.StateDynamics.REPEAT_MESSAGE_REASON_READY_SLEEP_STATE,1 */
  /* !LINKSTO CanNm.EB.Dsn.StateDynamics.REPEAT_MESSAGE_REASON_READY_SLEEP_STATE_1,1 */
  /* !LINKSTO CanNm.EB.Dsn.StateDynamics.REPEAT_MESSAGE_REASON_READY_SLEEP_STATE_2,1 */
#if (CANNM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
#if (CANNM_MULTICORE_ENABLED == STD_OFF)
  CANNM_CALL_NM_REMOTESLEEPCANCELLATION(CANNM_INST(instIdx));
#else
  if (CANNM_CALL_NM_REMOTESLEEPCANCELLATION(CANNM_INST(instIdx)) != SCHM_E_OK)
  {
#if (CANNM_DEV_ERROR_DETECT == STD_ON)
    /* !LINKSTO CanNm.EB.SchM_Call_DET,1 */
    CANNM_DET_REPORT_ERROR(CANNM_INST(instIdx),
                           CANNM_SERVID_MAINFUNCTION_X,
                           CANNM_E_REMOTESLEEPCANCELLATION);
#endif
  }
#endif
#elif (CANNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(void, CANNM_CODE) CanNm_HsmCanNmSfReadySleepRemoteSleepAction3(
  CANNM_PDL_SF(const uint8 instIdx))
{
  /* action 'Nm_RemoteSleepCancellation(); HandleRxIndicationCommon();'
   * for RX_INDICATION[]/...
   * external transition to state ReadySleepRemoteActivity */
#if (CANNM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
  /* !LINKSTO EB_CANNM151_Refine_2,1, CanNm.EB.Dsn.StateDynamics.RX_INDICATION_READY_SLEEP_STATE,1 */
#if (CANNM_MULTICORE_ENABLED == STD_OFF)
  CANNM_CALL_NM_REMOTESLEEPCANCELLATION(CANNM_INST(instIdx));
#else
  if (CANNM_CALL_NM_REMOTESLEEPCANCELLATION(CANNM_INST(instIdx)) != SCHM_E_OK)
  {
#if (CANNM_DEV_ERROR_DETECT == STD_ON)
    /* !LINKSTO CanNm.EB.SchM_Call_DET,1 */
    CANNM_DET_REPORT_ERROR(CANNM_INST(instIdx),
                           CANNM_SERVID_MAINFUNCTION_X,
                           CANNM_E_REMOTESLEEPCANCELLATION);
#endif
  }
#endif
#elif (CANNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
  CanNm_HandleRxIndicationCommon(CANNM_PL_SF(instIdx));
}

/* ************************************************************************
 * State: SendingSubMode
 * Parent state: NetworkMode
 * Init substate: RepeatMessageState
 * Transitions originating from this state:
 * 1) TX_TIMEOUT[]/#if ((PASSIVE_MODE == false) && (IMM_TRANCONF_ENBLD == false)) Nm_TxTimeoutException();
 * 2) MSG_CYCLE_TIMEOUT[]/HandleMsgCycleTimeout() --> CANNM032
 * 3) NM_TIMEOUT[]/if (NmTimeout == 0) {HandleNmTimeout();}
 *    #if (CANNM_PN_SUPPORTED == STD_ON) CanSM_TxTimeoutException();
 */

/* Deviation MISRAC2012-1 */
FUNC(void, CANNM_CODE) CanNm_HsmCanNmSfSendingSubModeExit(
   CANNM_PDL_SF(const uint8 instIdx))
{
  /* exit action 'MsgCycleTimerStop(); --> CANNM051' */
#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
  /* !LINKSTO CANNM173,1, CANNM051,1, EB_CANNM108_Refine_1,1, EB_CANNM108_Refine_2,1 */
  CanNm_MsgCycleTimerStop(CANNM_PL_SF(instIdx));

#if (CANNM_IMMEDIATE_RESTART_ENABLED == STD_ON)
  CANNM_CHANNEL_STATUS(instIdx).MessageFlags &= \
    (uint8)(~(uint8)(CANNM_EXECUTE_IMMEDIATE_RESTART));
#endif /* CANNM_IMMEDIATE_RESTART_ENABLED == STD_ON */
#elif (CANNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(void, CANNM_CODE) CanNm_HsmCanNmSfSendingSubModeAction1(
  CANNM_PDL_SF(const uint8 instIdx))
{
  /* action '#if ((PASSIVE_MODE == false) && (IMM_TRANCONF_ENBLD == false)) Nm_TxTimeoutException();
   * #if (CANNM_PN_SUPPORTED == STD_ON) CanSM_TxTimeoutException();'
   * for TX_TIMEOUT[]/...
   * internal transition */
  /* !LINKSTO CanNm.EB.Dsn.StateDynamics.Internal_Transition_SENDING_SUB_MODE_1,1 */
#if ((CANNM_HSM_INST_MULTI_ENABLED == STD_ON) && \
    ((CANNM_PASSIVE_MODE_ENABLED == STD_ON) || \
     (CANNM_IMMEDIATE_TXCONF_ENABLED == STD_ON)))
  TS_PARAM_UNUSED(instIdx);
#endif

#if ((CANNM_PASSIVE_MODE_ENABLED == STD_OFF) && \
     (CANNM_IMMEDIATE_TXCONF_ENABLED == STD_OFF))
  /* !LINKSTO CANNM066,1 */
#if (CANNM_MULTICORE_ENABLED == STD_OFF)
  CANNM_CALL_NM_TXTIMEOUTEXCEPTION(CANNM_INST(instIdx));
#else
  if (CANNM_CALL_NM_TXTIMEOUTEXCEPTION(CANNM_INST(instIdx)) != SCHM_E_OK)
  {
#if (CANNM_DEV_ERROR_DETECT == STD_ON)
    /* !LINKSTO CanNm.EB.SchM_Call_DET,1 */
    CANNM_DET_REPORT_ERROR(CANNM_INST(instIdx),
                           CANNM_SERVID_MAINFUNCTION_X,
                           CANNM_E_TXTIMEOUTEXCEPTION);
#endif
  }
#endif
#endif
#if (CANNM_PN_SUPPORTED == STD_ON)
  if (TRUE == CANNM_CHANNEL_PNENABLED(instIdx))
  {
    /* !LINKSTO CANNM446,1 */
    CanSM_TxTimeoutException(CANNM_CHANNEL_CONFIG(instIdx).nmChannelId);
  }
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(void, CANNM_CODE) CanNm_HsmCanNmSfSendingSubModeAction2(
  CANNM_PDL_SF(const uint8 instIdx))
{
  /* action 'HandleMsgCycleTimeout() --> CANNM032'
   * for MSG_CYCLE_TIMEOUT[]/...
   * internal transition */
  /* if timer was not yet reactivated before */
  /* !LINKSTO CanNm.EB.Dsn.StateDynamics.Internal_Transition_SENDING_SUB_MODE_2,1 */
#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
  if ((CANNM_CHANNEL_STATUS(instIdx).ChanStatus & CANNM_COM_DISABLED) == 0U)
  {
    /* Do transmission */
    /* !LINKSTO CANNM032,1, EB_CANNM100_Refine_1,1, EB_CANNM100_Refine_2,1 */
    /* !LINKSTO EB_CANNM100_Refine_3,1, EB_CANNM100_Refine_4,1 */
    CanNm_HandleTransmit(CANNM_PL_SF(instIdx));
  }
#elif (CANNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(void, CANNM_CODE) CanNm_HsmCanNmSfSendingSubModeAction3(
  CANNM_PDL_SF(const uint8 instIdx))
{
  /* action 'if (NmTimeout == 0) {HandleNmTimeout();}'
   * for NM_TIMEOUT[]/...
   * internal transition */
  /* !LINKSTO CanNm.EB.Dsn.StateDynamics.Internal_Transition_SENDING_SUB_MODE_3,1 */
  if (CANNM_CHANNEL_STATUS(instIdx).CanNmTimer == 0U)
  {
    if ((CANNM_CHANNEL_STATUS(instIdx).ChanStatus & CANNM_COM_DISABLED) == 0U)
    {
#if (CANNM_DEV_ERROR_DETECT == STD_ON)
      /* Service ID CANNM_SERVID_MAINFUNCTION_X is used as this function
       * is state machine generated */
      /* !LINKSTO CANNM321,1, CANNM193,1, CANNM194,1 */
      (void)Det_ReportError
      (CANNM_MODULE_ID, CANNM_INSTANCE_ID,
       CANNM_SERVID_MAINFUNCTION_X, CANNM_E_NETWORK_TIMEOUT);
#endif
      /* !LINKSTO CANNM101,1, CANNM117,1, CANNM206,1 */
      /* Deviation MISRAC2012-1 */
      CanNm_NmTimerStart(CANNM_PL_TIMER(instIdx, CANNM_CHANNEL_CONFIG(instIdx).CanNmTime));
    }
  }
}

/* ************************************************************************
 * State: NormalOperationState
 * Parent state: SendingSubMode
 * Init substate: NormalOperationRemoteActivity
 * Transitions originating from this state:
 */

/* Deviation MISRAC2012-1 */
FUNC(void, CANNM_CODE) CanNm_HsmCanNmSfNormalOperationStateEntry(
  CANNM_PDL_SF(const uint8 instIdx))
{
  /* entry action '#ifdef (SCI) Nm_StateChangeNotification(LastState, ActState);CurState = NormalOperationState;' */
#if (CANNM_STATE_CHANGE_IND_ENABLED == STD_ON)
  CanNm_Call_Nm_StateChangeNotification(CANNM_INST(instIdx),
                                        CANNM_CHANNEL_STATUS(instIdx).CurState,
                                        NM_STATE_NORMAL_OPERATION,
                                        TRUE);
#endif
  CANNM_CHANNEL_STATUS(instIdx).CurState = NM_STATE_NORMAL_OPERATION;
}

/* ************************************************************************
 * State: NormalOperationRemoteActivity
 * Parent state: NormalOperationState
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 * 1) RX_INDICATION[]/HandleRxIndication_NOState_RA();
 * 2) NormalOperationRemoteActivity -> ReadySleepRemoteActivity: NET_REQ_STATUS_CHANGED[NetworkRequested==FALSE]/
 * 3) NormalOperationRemoteActivity -> RepeatMessageState: REPEAT_MESSAGE_REASON[]/
 * 4) NormalOperationRemoteActivity -> NormalOperationRemoteSleep: UNI_TIMEOUT[(!(ChanStatus  & CANNM_COM_DISABLED)) && (UniTimer == 0)]/Nm_RemoteSleepIndication
 */

/* Deviation MISRAC2012-1 */
FUNC(void, CANNM_CODE) CanNm_HsmCanNmSfNormalOperationRemoteActivityEntry(
  CANNM_PDL_SF(const uint8 instIdx))
{
  /* entry action '#ifdef (RSI) UniTimerStart(CANNM_REMOTE_SLEEP_IND_TIME) -->CANNM150 #ifdef (RSI) clear RSI flag' */

#if (CANNM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
  /* !LINKSTO CANNM180_Refine,1 */
  if ((CANNM_CHANNEL_STATUS(instIdx).ChanStatus & CANNM_COM_DISABLED) == 0U)
  {
    /* Deviation MISRAC2012-1 */
    CanNm_UniTimerStart(CANNM_PL_TIMER(instIdx, CANNM_CHANNEL_CONFIG(instIdx).RSITime));
  }
  /* clear RSI Status */
  TS_AtomicClearBit_8(&CANNM_CHANNEL_STATUS(instIdx).ChanStatus, CANNM_RSI_TRUE_BIT);
#elif (CANNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(void, CANNM_CODE) CanNm_HsmCanNmSfNormalOperationRemoteActivityAction1(
  CANNM_PDL_SF(const uint8 instIdx))
{
  /* action 'HandleRxIndication_NOState_RA();'
   * for RX_INDICATION[]/...
   * internal transition */
  /* !LINKSTO CanNm.EB.Dsn.StateDynamics.Internal_Transition_NORMAL_OPERATION_STATE,1 */
  CanNm_HandleRxIndication_NOState_RA(CANNM_PL_SF(instIdx));
}
/* Deviation MISRAC2012-1 */
FUNC(boolean, CANNM_CODE) CanNm_HsmCanNmSfNormalOperationRemoteActivityGuard2(
  CANNM_PDL_SF(const uint8 instIdx))
{
  boolean ret;
  /* guard condition 'NetworkRequested==FALSE'
   * for NET_REQ_STATUS_CHANGED[...]/
   * external transition to state ReadySleepRemoteActivity */
   /* !LINKSTO CANNM118,1, CanNm.EB.Dsn.StateDynamics.NET_REQ_STATUS_CHANGED_NORMAL_OPERATION_STATE,1 */
  ret = (boolean)(((CANNM_CHANNEL_STATUS(instIdx).ChanStatus &
                     CANNM_NETWORK_REQUESTED) == 0U) ? TRUE : FALSE);
  return ret;
}
/* Deviation MISRAC2012-1 */
FUNC(boolean, CANNM_CODE) CanNm_HsmCanNmSfNormalOperationRemoteActivityGuard4(
  CANNM_PDL_SF(const uint8 instIdx))
{
  boolean ret;
  /* guard condition '(!(ChanStatus & CANNM_COM_DISABLED)) && (UniTimer == 0)'
   * for UNI_TIMEOUT[...]/Nm_RemoteSleepIndication
   * external transition to state NormalOperationRemoteSleep */
  ret = (boolean)
    (((CANNM_CHANNEL_STATUS(instIdx).ChanStatus & CANNM_COM_DISABLED) == 0U) ? TRUE : FALSE);
  return ret;
}
/* Deviation MISRAC2012-1 */
FUNC(void, CANNM_CODE) CanNm_HsmCanNmSfNormalOperationRemoteActivityAction4(
  CANNM_PDL_SF(const uint8 instIdx))
{
  /* action 'Nm_RemoteSleepIndication'
   * for UNI_TIMEOUT[(!(ChanStatus & CANNM_COM_DISABLED)) && (UniTimer == 0)]/Nm_RemoteSleepIndication
   * external transition to state NormalOperationRemoteSleep */
  /* !LINKSTO CanNm.EB.Dsn.StateDynamics.UNI_TIMEOUT_NORMAL_OPERATION_REMOTE_ACTIVITY,1 */
#if (CANNM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
  /* !LINKSTO CANNM150,1 */
#if (CANNM_MULTICORE_ENABLED == STD_OFF)
  CANNM_CALL_NM_REMOTESLEEPINDICATION(CANNM_INST(instIdx));
#else
  if (CANNM_CALL_NM_REMOTESLEEPINDICATION(CANNM_INST(instIdx)) != SCHM_E_OK)
  {
#if (CANNM_DEV_ERROR_DETECT == STD_ON)
    /* !LINKSTO CanNm.EB.SchM_Call_DET,1 */
    CANNM_DET_REPORT_ERROR(CANNM_INST(instIdx),
                           CANNM_SERVID_MAINFUNCTION_X,
                           CANNM_E_REMOTESLEEPINDICATION);
#endif
  }
#endif
#endif

#if ((CANNM_HSM_INST_MULTI_ENABLED == STD_ON) && (CANNM_REMOTE_SLEEP_IND_ENABLED == STD_OFF))
  TS_PARAM_UNUSED(instIdx);
#endif
}
/* ************************************************************************
 * State: NormalOperationRemoteSleep
 * Parent state: NormalOperationState
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 * 1) NormalOperationRemoteSleep -> ReadySleepRemoteSleep: NET_REQ_STATUS_CHANGED[NetworkRequested==FALSE]/
 * 2) NormalOperationRemoteSleep -> RepeatMessageState: REPEAT_MESSAGE_REASON[]/Nm_RemoteSleepCancellation();
 * 3) NormalOperationRemoteSleep -> NormalOperationRemoteActivity: RX_INDICATION[]/HandleRxIndication_NOState() RSI timer is started by entry action #ifdef (RSI) Nm_RemoteSleepCancellation
 */

/* Deviation MISRAC2012-1 */
FUNC(void, CANNM_CODE) CanNm_HsmCanNmSfNormalOperationRemoteSleepEntry(
  CANNM_PDL_SF(const uint8 instIdx))
{
  /* entry action '#if defined(RSI) RsiStatus = CANNM_RSI_TRUE;' */
#if (CANNM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
  /* set RSI Status to CANNM_RSI_TRUE */
  TS_AtomicSetBit_8(&CANNM_CHANNEL_STATUS(instIdx).ChanStatus, CANNM_RSI_TRUE_BIT);

#elif (CANNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(boolean, CANNM_CODE) CanNm_HsmCanNmSfNormalOperationRemoteSleepGuard1(
  CANNM_PDL_SF(const uint8 instIdx))
{
  boolean ret;
  /* guard condition 'NetworkRequested==FALSE'
   * for NET_REQ_STATUS_CHANGED[...]/
   * external transition to state ReadySleepRemoteSleep */
  /* !LINKSTO CanNm.EB.Dsn.StateDynamics.NET_REQ_STATUS_CHANGED_NORMAL_OPERATION_STATE,1 */
  ret = (boolean)(((CANNM_CHANNEL_STATUS(instIdx).ChanStatus &
                     CANNM_NETWORK_REQUESTED) == 0U) ? TRUE : FALSE);
  return ret;
}
/* Deviation MISRAC2012-1 */
FUNC(void, CANNM_CODE) CanNm_HsmCanNmSfNormalOperationRemoteSleepAction2(
  CANNM_PDL_SF(const uint8 instIdx))
{
  /* action 'Nm_RemoteSleepCancellation();'
   * for REPEAT_MESSAGE_REASON[]/...
   * external transition to state RepeatMessageState */
  /* !LINKSTO EB_CANNM152_Refine_1,1, CANNM119,1, CANNM120,1 */
  /* !LINKSTO CanNm.EB.Dsn.StateDynamics.REPEAT_MESSAGE_REASON_NORMAL_OPERATION_REMOTE_SLEEP,1 */
  /* !LINKSTO CanNm.EB.Dsn.StateDynamics.REPEAT_MESSAGE_REASON_NORMAL_OPERATION_REMOTE_ACTIVITY_1,1 */
  /* !LINKSTO CanNm.EB.Dsn.StateDynamics.REPEAT_MESSAGE_REASON_NORMAL_OPERATION_REMOTE_ACTIVITY_2,1 */
#if (CANNM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
#if (CANNM_MULTICORE_ENABLED == STD_OFF)
  CANNM_CALL_NM_REMOTESLEEPCANCELLATION(CANNM_INST(instIdx));
#else
  if (CANNM_CALL_NM_REMOTESLEEPCANCELLATION(CANNM_INST(instIdx)) != SCHM_E_OK)
  {
#if (CANNM_DEV_ERROR_DETECT == STD_ON)
    /* !LINKSTO CanNm.EB.SchM_Call_DET,1 */
    CANNM_DET_REPORT_ERROR(CANNM_INST(instIdx),
                           CANNM_SERVID_MAINFUNCTION_X,
                           CANNM_E_REMOTESLEEPCANCELLATION);
#endif
  }
#endif
#elif (CANNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(void, CANNM_CODE) CanNm_HsmCanNmSfNormalOperationRemoteSleepAction3(
  CANNM_PDL_SF(const uint8 instIdx))
{
  /* action 'HandleRxIndication_NOState() RSI timer is started by entry action #ifdef (RSI) Nm_RemoteSleepCancellation'
   * for RX_INDICATION[]/...
   * external transition to state NormalOperationRemoteActivity */
  CanNm_HandleRxIndication_NOState(CANNM_PL_SF(instIdx));

#if (CANNM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
  /* !LINKSTO EB_CANNM151_Refine_1,1, CanNm.EB.Dsn.StateDynamics.RX_INDICATION_NORMAL_OPERATION_REMOTE_SLEEP,1 */
#if (CANNM_MULTICORE_ENABLED == STD_OFF)
  CANNM_CALL_NM_REMOTESLEEPCANCELLATION(CANNM_INST(instIdx));
#else
  if (CANNM_CALL_NM_REMOTESLEEPCANCELLATION(CANNM_INST(instIdx)) != SCHM_E_OK)
  {
#if (CANNM_DEV_ERROR_DETECT == STD_ON)
    /* !LINKSTO CanNm.EB.SchM_Call_DET,1 */
    CANNM_DET_REPORT_ERROR(CANNM_INST(instIdx),
                           CANNM_SERVID_MAINFUNCTION_X,
                           CANNM_E_REMOTESLEEPCANCELLATION);
#endif
  }
#endif
#endif
}

/* ************************************************************************
 * State: RepeatMessageState
 * Parent state: SendingSubMode
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 * 1) RepeatMessageState -> ReadySleepState: RMS_TIMEOUT[(NetworkRequested==FALSE) && (RMS timer == 0))]/
 * 2) RepeatMessageState -> NormalOperationState: RMS_TIMEOUT[((NetworkRequested==TRUE) && (RMS timer == 0))]/
 */

/* !LINKSTO CANNM314,1 */
/* Deviation MISRAC2012-1 */
FUNC(void, CANNM_CODE) CanNm_HsmCanNmSfRepeatMessageStateEntry(
  CANNM_PDL_SF(const uint8 instIdx))
{
  /* entry action '#ifdev (SCI) Nm_StateChangeNotification(LastState, ActState);CurState = RepeatMessageState; RMSTimerStart() -->CANNM102;MsgCycleTimerStart' */
#if (CANNM_IMMEDIATE_RESTART_ENABLED == STD_ON)
  Nm_StateType PrevState = CANNM_CHANNEL_STATUS(instIdx).CurState;
#endif /* (CANNM_IMMEDIATE_RESTART_ENABLED == STD_ON) */

  CANNM_CHANNEL_STATUS(instIdx).MessageFlags &= (uint8)(~(uint8)(CANNM_MESSAGE_SENT_ON_CYCLE));

  /* !LINKSTO CANNM102,1 */
  /* Deviation MISRAC2012-1 */
  CanNm_RmsTimerStart(CANNM_PL_TIMER(instIdx, CANNM_CHANNEL_CONFIG(instIdx).RMSTime));
  if (CANNM_CHANNEL_CONFIG(instIdx).RMSTime > 0U)
  {
    CANNM_CHANNEL_STATUS(instIdx).MessageFlags |= CANNM_IN_REPEAT_MESSAGE_STATE;
  }

#if (CANNM_STATE_CHANGE_IND_ENABLED == STD_ON)
  CanNm_Call_Nm_StateChangeNotification(CANNM_INST(instIdx),
                                        CANNM_CHANNEL_STATUS(instIdx).CurState,
                                        NM_STATE_REPEAT_MESSAGE,
                                        TRUE);
#endif

#if (CANNM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
  /* clear RSI Status */
  TS_AtomicClearBit_8(&CANNM_CHANNEL_STATUS(instIdx).ChanStatus, CANNM_RSI_TRUE_BIT);
#endif
  CANNM_CHANNEL_STATUS(instIdx).CurState = NM_STATE_REPEAT_MESSAGE;

  /* !LINKSTO CANNM116,1 */
#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
  if ((CANNM_CHANNEL_STATUS(instIdx).ChanStatus & CANNM_COM_DISABLED) == 0U)
  {
    /* Check if immediate transmission feature is active for this channel. */
    /* !LINKSTO CANNM005_Implicit,1 */
    if ((CANNM_CHANNEL_STATUS(instIdx).MessageFlags & CANNM_RMS_ENTERED_FROM_NM) != 0)
    {
#if (CANNM_IMMEDIATE_RESTART_ENABLED == STD_ON)
      /* !LINKSTO CanNm.EB.CanNm_NetworkGwEraRequest.CanNmImmediateRestartEnabled.PrepareBusSleepMode,1 */
      /* !LINKSTO CanNm.EB.PassiveStartup.NetworkRequest.Sequence.CanNmImmediateRestartEnabled.PrepareBusSleepMode,1 */
      if (((CANNM_CHANNEL_STATUS(instIdx).ChanStatus & CANNM_NETWORK_REQUESTED) != 0) &&
          (PrevState == NM_STATE_PREPARE_BUS_SLEEP))
      {
        /* !LINKSTO CANNM122,1, CanNm.EB.Dsn.StateDynamics.NET_REQ_STATUS_CHANGED_PREPARE_BUS_SLEEP_MODE,1 */
        CANNM_CHANNEL_STATUS(instIdx).MessageFlags |= CANNM_EXECUTE_IMMEDIATE_RESTART;
        /* Start transmission immediately
         * Start message cycle timer with zero.
         * After transition is done to repeat message state
         * (state change notification is called)
         * handling of transmission is done.
         * This way firstly state change notification is called
         * and after that CanIf_Transmit is called with the updated
         * information.
         */
        CanNm_MsgCycleTimerStart(CANNM_PL_TIMER(instIdx, 0U));
      }
      else
#endif /* (CANNM_IMMEDIATE_RESTART_ENABLED == STD_ON) */
#if (CANNM_IMMEDIATE_TRANSMISSION == STD_ON)
      /* !LINKSTO CanNm.EB.CanNm_NetworkGwEraRequest.CanNmImmediateNmTransmissions.BusSleepMode,1 */
      /* !LINKSTO CanNm.EB.CanNm_NetworkGwEraRequest.CanNmImmediateNmTransmissions.PrepareBusSleepMode,1 */
      /* !LINKSTO CanNm.EB.PassiveStartup.NetworkRequest.Sequence.CanNmImmediateNmTransmissions.BusSleepMode,1 */
      /* !LINKSTO CanNm.EB.PassiveStartup.NetworkRequest.Sequence.CanNmImmediateNmTransmissions.PrepareBusSleepMode,1 */
      if(((CANNM_CHANNEL_STATUS(instIdx).ChanStatus & CANNM_NETWORK_REQUESTED) != 0) &&
          (CANNM_CHANNEL_CONFIG(instIdx).ImmediateNmTransmissions != 0))
      {
        /* Load immediate transmission counter with configured value */
        CANNM_CHANNEL_STATUS(instIdx).ImmediateNmTransmissionCounter =
          CANNM_CHANNEL_CONFIG(instIdx).ImmediateNmTransmissions;
        /* Start transmission immediately
         * Start message cycle timer with zero.
         * After transition is done to repeat message state
         * (state change notification is called)
         * handling of transmission is done.
         * This way firstly state change notification is called
         * and after that CanIf_Transmit is called with the updated
         * information.
         */
        CanNm_MsgCycleTimerStart(CANNM_PL_TIMER(instIdx, 0U));
      }
      else
#endif /* (CANNM_IMMEDIATE_TRANSMISSION == STD_ON) */
      {
        /* !LINKSTO CANNM023,1, CANNM178_ImmediateNmTransmissions,1, EB_CANNM005_Refine_1,1, EB_CANNM005_Refine_2,1 */
        /* !LINKSTO EB_CANNM156_Refine_1,1, EB_CANNM156_Refine_2,1, EB_CANNM156_Refine_3,1, EB_CANNM156_Refine_4,1 */
        /* Deviation MISRAC2012-1 */
        CanNm_MsgCycleTimerStart(
          CANNM_PL_TIMER(instIdx, CANNM_CHANNEL_MSGCYCLEOFFSET(CANNM_INST(instIdx))));
      }
    }
    else
    {
      /* !LINKSTO CANNM023,1, CANNM178_ImmediateNmTransmissions,1, EB_CANNM005_Refine_1,1, EB_CANNM005_Refine_2,1 */
      /* !LINKSTO EB_CANNM156_Refine_1,1, EB_CANNM156_Refine_2,1, EB_CANNM156_Refine_3,1, EB_CANNM156_Refine_4,1 */
      /* Deviation MISRAC2012-1 */
      CanNm_MsgCycleTimerStart(
        CANNM_PL_TIMER(instIdx, CANNM_CHANNEL_MSGCYCLEOFFSET(CANNM_INST(instIdx))));
    }
  }
#endif /* (CANNM_PASSIVE_MODE_ENABLED == STD_OFF) */
  CANNM_CHANNEL_STATUS(instIdx).MessageFlags &= (uint8)(~(uint8)(CANNM_RMS_ENTERED_FROM_NM));
}
/* Deviation MISRAC2012-1 */
FUNC(void, CANNM_CODE) CanNm_HsmCanNmSfRepeatMessageStateExit(
   CANNM_PDL_SF(const uint8 instIdx))
{
  /* exit action '#if (NODE_DETECTION_ENABLED == true) <Unset Repeat Message Bit> --> CANNM107' */
#if (CANNM_NODE_DETECTION_ENABLED == STD_ON)
  CONSTP2VAR(uint8, AUTOMATIC, CANNM_VAR_CLEARED) TxPduPtr
    = &CANNM_CHANNEL_STATUS(instIdx).TxPduPtr[
      CANNM_CHANNEL_CONFIG(instIdx).CbvPos];
#endif

#if ((CANNM_IMMEDIATE_TRANSMISSION == STD_ON) && (CANNM_PASSIVE_MODE_ENABLED == STD_OFF))
/* ImmediateNmTransmissionCounter is set to zero when repeat message state is leaved.
 * Retrying to send immediate message on the next mainfunction is limited to repeat message state only 
 */
/* !LINKSTO CanNm.EB.SWS_CanNm_00335,1 */
  CANNM_CHANNEL_STATUS(instIdx).ImmediateNmTransmissionCounter = 0U;
#endif
  CANNM_CHANNEL_STATUS(instIdx).MessageFlags &= (uint8)(~(uint8)(CANNM_IN_REPEAT_MESSAGE_STATE));
  CANNM_CHANNEL_STATUS(instIdx).RmsTimer = 0U;

#if (CANNM_NODE_DETECTION_ENABLED == STD_ON)
  /* Clear RepeatMessageBit */
  /* !LINKSTO CANNM107,1 */
  if(CANNM_CHANNEL_CONFIG(instIdx).CanNmNodeDetectionEnabled == TRUE)
  {
    TS_AtomicClearBit_8(TxPduPtr, CANNM_CBV_REPEATMESSAGEBIT);
  }
  else
  {
  }

#elif (CANNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(boolean, CANNM_CODE) CanNm_HsmCanNmSfRepeatMessageStateGuard1(
  CANNM_PDL_SF(const uint8 instIdx))
{
  boolean ret;
  /* guard condition '(NetworkRequested==FALSE) && (RMS timer == 0))'
   * for RMS_TIMEOUT[...]/
   * external transition to state ReadySleepState */
   /* !LINKSTO CANNM106,1, CanNm.EB.Dsn.StateDynamics.RMS_TIMEOUT_READY_SLEEP_STATE,1 */
  ret = (boolean)( ( ( (CANNM_CHANNEL_STATUS(instIdx).ChanStatus &
                         CANNM_NETWORK_REQUESTED) == 0U) &&
                       (CANNM_CHANNEL_STATUS(instIdx).RmsTimer == 0U)
                   ) ? TRUE : FALSE
                 );
  return ret;
}
/* Deviation MISRAC2012-1 */
FUNC(boolean, CANNM_CODE) CanNm_HsmCanNmSfRepeatMessageStateGuard2(
  CANNM_PDL_SF(const uint8 instIdx))
{
  boolean ret;
  /* guard condition '((NetworkRequested==TRUE) && (RMS timer == 0))'
   * for RMS_TIMEOUT[...]/
   * external transition to state NormalOperationState */
   /* !LINKSTO CANNM103,1, CanNm.EB.Dsn.StateDynamics.RMS_TIMEOUT_REPEAT_MESSAGE_STATE,1 */
  ret = (boolean)( ( ( (CANNM_CHANNEL_STATUS(instIdx).ChanStatus &
                        CANNM_NETWORK_REQUESTED) != 0U) &&
                     (CANNM_CHANNEL_STATUS(instIdx).RmsTimer == 0U)
                   )? TRUE : FALSE
                 );
  return ret;
}

/* ************************************************************************
 * State: PrepareBusSleepMode
 * Parent state: TOP
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 * 1) PrepareBusSleepMode -> NetworkMode: NETWORK_START[]/
 * 2) PrepareBusSleepMode -> NetworkMode: NET_REQ_STATUS_CHANGED[NetworkRequested==TRUE]/
 * 3) PrepareBusSleepMode -> NetworkMode: RX_INDICATION[]/#ifdef (...) Nm_PduRxIndication(); #endif --> CANNM037
 * 4) PrepareBusSleepMode -> BusSleepMode: UNI_TIMEOUT[]/
 */

/* Deviation MISRAC2012-1 */
FUNC(void, CANNM_CODE) CanNm_HsmCanNmSfPrepareBusSleepModeEntry(
  CANNM_PDL_SF(const uint8 instIdx))
{
  /* entry action '#ifdef (SCI) Nm_StateChangeNotification(LastState, ActState);CurState = PrepareBusSleepMode; Nm_PrepareBusSleepMode(); --> CANNM114UniTimerStart(CANNM_WAIT_BUS_SLEEP_TIME); --> CANNM115' ClearActiveWakeUpBit()*/
#if (CANNM_ACTIVE_WAKEUP_BIT_ENABLED == STD_ON)
  SchM_Enter_CanNm_SCHM_CANNM_EXCLUSIVE_AREA_0();
  CANNM_CHANNEL_STATUS(instIdx).ActiveWakeUp1 = 0U;
  CANNM_CHANNEL_STATUS(instIdx).ActiveWakeUp2 = 0U;
  SchM_Exit_CanNm_SCHM_CANNM_EXCLUSIVE_AREA_0();
#endif /* CANNM_ACTIVE_WAKEUP_BIT_ENABLED == STD_ON */
  /* !LINKSTO CANNM115,1, CANNM088,1, CanNm.EB.Dsn.StateDynamics.UNI_TIMEOUT_PREPARE_BUS_SLEEP_MODE,1 */
  /* Deviation MISRAC2012-1 */
  CanNm_UniTimerStart(CANNM_PL_TIMER(instIdx, CANNM_CHANNEL_CONFIG(instIdx).WBSTime));
#if (CANNM_STATE_CHANGE_IND_ENABLED == STD_ON)
  CanNm_Call_Nm_StateChangeNotification(CANNM_INST(instIdx),
                                        CANNM_CHANNEL_STATUS(instIdx).CurState,
                                        NM_STATE_PREPARE_BUS_SLEEP,
                                        TRUE);
#endif
  /* !LINKSTO CANNM114,1, CANNM093,1 */
#if (CANNM_MULTICORE_ENABLED == STD_OFF)
  CANNM_CALL_NM_PREPAREBUSSLEEPMODE(CANNM_INST(instIdx));
#else
  if (CANNM_CALL_NM_PREPAREBUSSLEEPMODE(CANNM_INST(instIdx)) != SCHM_E_OK)
  {
#if (CANNM_DEV_ERROR_DETECT == STD_ON)
    /* !LINKSTO CanNm.EB.SchM_Call_DET,1 */
    CANNM_DET_REPORT_ERROR(CANNM_INST(instIdx),
                           CANNM_SERVID_MAINFUNCTION_X,
                           CANNM_E_PREPAREBUSSLEEPMODE);
#endif
  }
#endif
  CANNM_CHANNEL_STATUS(instIdx).CurState = NM_STATE_PREPARE_BUS_SLEEP;
  /* 'ClearActiveWakeUpBit() -->CANNM402;' */
  /* Clearing of active wakeup bit has been moved
   * into Prepare Bus Sleep Mode entry
   * from network mode exit
   * since network mode can be re-entered.
   * Active Wakeup bit is set on network mode entry.
   * If CanNmPnHandleMultipleNetworkRequests is set to true,
   * and if for the first time the network mode was entered
   * with passive startup and while in network mode the network
   * is requested, network mode is re-entered
   * (if CanNmPnHandleMultipleNetworkRequests is set to true)
   * and in this case active wakeup shall not be set
   */
#if (CANNM_ACTIVE_WAKEUP_BIT_ENABLED == STD_ON)
  /* !LINKSTO CANNM402,1 */

  TS_AtomicClearBit_8(&CANNM_CHANNEL_STATUS(instIdx).TxPduPtr[CANNM_CHANNEL_CONFIG(instIdx).CbvPos],
    CANNM_CBV_ACTIVEWAKEUPBIT);
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(boolean, CANNM_CODE) CanNm_HsmCanNmSfPrepareBusSleepModeGuard2(
  CANNM_PDL_SF(const uint8 instIdx))
{
  boolean ret;
  /* guard condition 'NetworkRequested==TRUE'
   * for NET_REQ_STATUS_CHANGED[...]/
   * external transition to state NetworkMode */
   /* !LINKSTO CANNM315,1, CANNM123,1 */
  ret = (boolean)(((CANNM_CHANNEL_STATUS(instIdx).ChanStatus &
                     CANNM_NETWORK_REQUESTED) != 0U) ? TRUE : FALSE);
  return ret;
}
/* Deviation MISRAC2012-1 */
FUNC(void, CANNM_CODE) CanNm_HsmCanNmSfPrepareBusSleepModeAction3(
  CANNM_PDL_SF(const uint8 instIdx))
{
  /* action '#ifdef (...) Nm_PduRxIndication(); #endif --> CANNM037'
   * for RX_INDICATION[]/...
   * external transition to state NetworkMode */
   /* !LINKSTO CANNM315,1, CANNM124,1, CanNm.EB.Dsn.StateDynamics.RX_INDICATION_PREPARE_BUS_SLEEP_MODE,1 */
#if (CANNM_PDU_RX_INDICATION_ENABLED == STD_ON)
  /* !LINKSTO CANNM037,1 */
#if (CANNM_MULTICORE_ENABLED == STD_OFF)
  CANNM_CALL_NM_PDURXINDICATION(CANNM_INST(instIdx));
#else
  if (CANNM_CALL_NM_PDURXINDICATION(CANNM_INST(instIdx)) != SCHM_E_OK)
  {
#if (CANNM_DEV_ERROR_DETECT == STD_ON)
    /* !LINKSTO CanNm.EB.SchM_Call_DET,1 */
    CANNM_DET_REPORT_ERROR(CANNM_INST(instIdx),
                           CANNM_SERVID_MAINFUNCTION_X,
                           CANNM_E_PDURXINDICATION);
#endif
  }
#endif
#elif (CANNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}


/*==================[internal function definitions]=========================*/

/* Deviation MISRAC2012-1 */
STATIC FUNC(void, CANNM_CODE) CanNm_InitIntVar(
  CANNM_PDL_SF(uint8 instIdx))
{
  /* !LINKSTO CANNM143,1 */
  CANNM_CHANNEL_STATUS(instIdx).ChanStatus    = 0U;
#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
  CANNM_CHANNEL_STATUS(instIdx).MsgCycleTimer = 0U;
#if (CANNM_IMMEDIATE_TXCONF_ENABLED == STD_OFF)
  /* !LINKSTO CANNM061,1 */
  CANNM_CHANNEL_STATUS(instIdx).TimeoutTimer  = 0U;
#endif
  CANNM_CHANNEL_STATUS(instIdx).RmsTimer      = 0U;
#endif

#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
#if ((CANNM_USER_DATA_ENABLED == STD_ON) || (CANNM_COM_USER_DATA_SUPPORT == STD_ON))
  /* !LINKSTO CANNM025,1 */
  /* Set user data to 0xFF */
  TS_MemSet(
    &CANNM_CHANNEL_STATUS(instIdx).TxPduPtr[CANNM_CHANNEL_CONFIG(instIdx).UDFBPos],
    0xFFU, (uint32)CANNM_CHANNEL_USERDATALENGTH(instIdx));

  TS_MemSet(
    &CANNM_CHANNEL_STATUS(instIdx).RxPduPtr[CANNM_CHANNEL_CONFIG(instIdx).UDFBPos],
    0xFFU, (uint32)CANNM_CHANNEL_USERDATALENGTH(instIdx));
#endif

#if (CANNM_NODE_DETECTION_ENABLED == STD_ON)
  /* !LINKSTO CANNM085,1 */
  /* An X-Path check in the configuration schema already ensures that
   * the position of the CBV is not set to OFF when Node Detection support
   * is enabled. So an additional check is not required here.
   */
  CANNM_CHANNEL_STATUS(instIdx).TxPduPtr[CANNM_CHANNEL_CONFIG(instIdx).CbvPos] = 0x00U;
#endif

#if (CANNM_NODE_ID_ENABLED == STD_ON)
#if (CANNM_NODEID_CALLBACK_ENABLED == STD_ON)
    /* !LINKSTO CanNm.EB.CanNmNodeIdCallback_2,1 */
    /* Set the value returned by Node Id call back function in CBV */
    CANNM_CHANNEL_STATUS(instIdx).TxPduPtr[CANNM_CHANNEL_CONFIG(instIdx).NidPos] =
      CANNM_NODEID_CALLBACK_NAME(CANNM_CHANNEL_CONFIG(instIdx).nmChannelId);
#else
    /* !LINKSTO CANNM013,1 */
    CANNM_CHANNEL_STATUS(instIdx).TxPduPtr[CANNM_CHANNEL_CONFIG(instIdx).NidPos]
      = CANNM_CHANNEL_NODEID(CANNM_INST(instIdx));
#endif
#endif
#if (CANNM_PN_SUPPORTED == STD_ON)
    /* !LINKSTO CANNM414,1 */
    if (CANNM_CHANNEL_PNENABLED(instIdx) == TRUE)
    {
      /* !LINKSTO CANNM413,1 */
      CANNM_CHANNEL_STATUS(instIdx).TxPduPtr[CANNM_CHANNEL_CONFIG(instIdx).CbvPos] |=
                                                                            CANNM_CBV_PNINFOBITMASK;
    }
#endif
#endif

#if (CANNM_PN_SUPPORTED == STD_ON)
  /* Disable filtering of partial network info */
  CANNM_CHANNEL_STATUS(instIdx).PnFilterEnabled = FALSE;
#endif

#if (CANNM_PN_ERA_CALC_ENABLED == STD_ON)
  /* Initialise external requests */
  /* !LINKSTO CANNM435,1 */
  TS_MemSet(CANNM_CHANNEL_STATUS(instIdx).Era.PnInfo, 0U, CANNM_PN_INFO_LENGTH);
  TS_MemSet(CANNM_CHANNEL_STATUS(instIdx).Era.PnValue, 0U, CANNM_PN_INFO_LENGTH);
  TS_MemSet(CANNM_CHANNEL_STATUS(instIdx).Era.PnTimer, 0U,
            (CANNM_EIRA_MAX_TIMER_SIZE * sizeof(CanNm_TimeType)));
#endif
}

/* Deviation MISRAC2012-1 */
STATIC FUNC(void, CANNM_CODE) CanNm_NmTimerStart(
  CANNM_PL_TIMER(uint8 instIdx, CanNm_TimeType CanNmTime))
{
  CANNM_CHANNEL_STATUS(instIdx).CanNmTimer = CanNmTime;
}

/* Deviation MISRAC2012-1 */
STATIC FUNC(void, CANNM_CODE) CanNm_NmTimerStop(
  CANNM_PDL_SF(uint8 instIdx))
{
  /* Stop the Nm Timer */
  CANNM_CHANNEL_STATUS(instIdx).CanNmTimer = 0U;
}

/* Deviation MISRAC2012-1 */
STATIC FUNC(void, CANNM_CODE) CanNm_UniTimerStart(
  CANNM_PL_TIMER(uint8 instIdx, CanNm_TimeType UniTimer))
{
  /* load timer value */
  CANNM_CHANNEL_STATUS(instIdx).UniversalTimer = UniTimer;
}

/* Deviation MISRAC2012-1 */
STATIC FUNC(void, CANNM_CODE) CanNm_UniTimerStop(
  CANNM_PDL_SF(uint8 instIdx))
{
  /* Stop the Universal Timer */
  CANNM_CHANNEL_STATUS(instIdx).UniversalTimer = 0U;
}

/* Deviation MISRAC2012-1 */
STATIC FUNC(void, CANNM_CODE) CanNm_RmsTimerStart(
  CANNM_PL_TIMER(uint8 instIdx, CanNm_TimeType RmsTime))
{
  CANNM_CHANNEL_STATUS(instIdx).RmsTimer = RmsTime;
  if (RmsTime == 0U)
  {
    /* Emit the event MSG_CYCLE_TIMEOUT if timer expires immediately */
    (void)CANNM_HSMEMITTOSELFINST(
      &CanNm_HsmScCanNm, instIdx, CANNM_HSM_CANNM_EV_RMS_TIMEOUT);
  }
}

#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, CANNM_CODE) CanNm_MsgCycleTimerStart(
  CANNM_PL_TIMER(uint8 instIdx, CanNm_TimeType MsgCycleTimer))
{
  /* load timer value */
  CANNM_CHANNEL_STATUS(instIdx).MsgCycleTimer = MsgCycleTimer;

  if (MsgCycleTimer == 0U)
  {
    /* Emit the event MSG_CYCLE_TIMEOUT if timer expires immediately */
    (void)CANNM_HSMEMITTOSELFINST(
      &CanNm_HsmScCanNm, instIdx, CANNM_HSM_CANNM_EV_MSG_CYCLE_TIMEOUT);
  }
}

/* Deviation MISRAC2012-1 */
STATIC FUNC(void, CANNM_CODE) CanNm_MsgCycleTimerStop(
  CANNM_PDL_SF(uint8 instIdx))
{
  /* Stop the Msg Cycle Timer */
  CANNM_CHANNEL_STATUS(instIdx).MsgCycleTimer = 0U;
}
#endif

/* Deviation MISRAC2012-1 */
STATIC FUNC(void, CANNM_CODE) CanNm_HandleRxIndicationCommon(
  CANNM_PDL_SF(uint8 instIdx))
{
  /* !LINKSTO CANNM098_Refine,1 */
  /* Note: CanNmTime-1 is assigned to reduce the extra main cycle delay
   * during the transition from Ready Sleep State to Prepare Bus-Sleep Mode.
   * Check for communication dissabled added for defensive programming
   */
  if ((CANNM_CHANNEL_STATUS(instIdx).ChanStatus & CANNM_COM_DISABLED) == 0U)
  {
    /* !LINKSTO CANNM206,1 */
    CanNm_NmTimerStart(CANNM_PL_TIMER(instIdx, (CANNM_CHANNEL_CONFIG(instIdx).CanNmTime - 1U)));
  }

#if (CANNM_PDU_RX_INDICATION_ENABLED == STD_ON)
  /* !LINKSTO CANNM037,1 */
#if (CANNM_MULTICORE_ENABLED == STD_OFF)
  CANNM_CALL_NM_PDURXINDICATION(CANNM_INST(instIdx));
#else
  if (CANNM_CALL_NM_PDURXINDICATION(CANNM_INST(instIdx)) != SCHM_E_OK)
  {
#if (CANNM_DEV_ERROR_DETECT == STD_ON)
    /* !LINKSTO CanNm.EB.SchM_Call_DET,1 */
    CANNM_DET_REPORT_ERROR(CANNM_INST(instIdx),
                           CANNM_SERVID_MAINFUNCTION_X,
                           CANNM_E_PDURXINDICATION);
#endif
  }
#endif
#endif
}

/* Deviation MISRAC2012-1 */
STATIC FUNC(void, CANNM_CODE) CanNm_HandleRxIndication_NOState_RA(
  CANNM_PDL_SF(uint8 instIdx))
{
#if (CANNM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
  if ((CANNM_CHANNEL_STATUS(instIdx).ChanStatus & CANNM_COM_DISABLED) == 0U)
  {
    /* !LINKSTO CANNM180_Refine,1 */
    /* Deviation MISRAC2012-1 */
    CanNm_UniTimerStart(CANNM_PL_TIMER(instIdx, CANNM_CHANNEL_CONFIG(instIdx).RSITime));
  }
#endif
  CanNm_HandleRxIndication_NOState(CANNM_PL_SF(instIdx));
}

/* Deviation MISRAC2012-1 */
STATIC FUNC(void, CANNM_CODE) CanNm_HandleRxIndication_NOState(
  CANNM_PDL_SF(uint8 instIdx))
{
#if (CANNM_BUS_LOAD_REDUCTION_ENABLED == STD_ON)
  if (CANNM_CHANNEL_CONFIG_BUSLOADREDACTIVE(instIdx))
  {
    /* !LINKSTO CANNM238,1, CANNM069_Refine,1, EB_CANNM157_Refine_1,1 */
    /* !LINKSTO EB_CANNM157_Refine_2,1 */
    if ((CANNM_CHANNEL_STATUS(instIdx).ChanStatus & CANNM_COM_DISABLED) == 0U)
    {
      /* Deviation MISRAC2012-1 */
      CanNm_MsgCycleTimerStart(CANNM_PL_TIMER(instIdx, CANNM_CHANNEL_CONFIG(instIdx).ReducedTime));
    }
  }
#endif
  /* Execute the common actions for Rx Indication */
  CanNm_HandleRxIndicationCommon(CANNM_PL_SF(instIdx));
}

#if ((CANNM_PASSIVE_MODE_ENABLED == STD_OFF) && (CANNM_IMMEDIATE_TXCONF_ENABLED == STD_OFF))
/* !LINKSTO CANNM071,1 */
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, CANNM_CODE) CanNm_HandleTxConfirmation(
  CANNM_PDL_SF(uint8 instIdx))
{
  /* Note: CanNm_NmTimerStart() occurs only after one main cycle
   * once the TX_CONFIRMATION event is emited. So Nm timer is restarted
   * with CanNmTime-1 to avoid an extra main cycle delay.
   */
  /* !LINKSTO CanNm.EB.NmTimer,1, CANNM099_Refine_1,1 */
  if ((CANNM_CHANNEL_STATUS(instIdx).ChanStatus & CANNM_COM_DISABLED) == 0U)
  {
    /* !LINKSTO CANNM206,1 */
    CanNm_NmTimerStart(CANNM_PL_TIMER(instIdx, (CANNM_CHANNEL_CONFIG(instIdx).CanNmTime - 1U)));
  }
  /* !LINKSTO CANNM065,1 */
  SchM_Enter_CanNm_SCHM_CANNM_EXCLUSIVE_AREA_0();
  CANNM_TX_TIMER_STOP(instIdx);
  SchM_Exit_CanNm_SCHM_CANNM_EXCLUSIVE_AREA_0();
}
#endif

#if ((CANNM_COM_CONTROL_ENABLED == STD_ON) && (CANNM_PASSIVE_MODE_ENABLED == STD_OFF))
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, CANNM_CODE) CanNm_HandleComControl(
 CANNM_PDL_SF(uint8 instIdx))
{
  if ((CANNM_CHANNEL_STATUS(instIdx).ChanStatus & CANNM_COM_DISABLED) == 0U)
  {
    /* !LINKSTO CANNM178,1 */
    CanNm_MsgCycleTimerStart(
      /* Deviation MISRAC2012-1 */
      CANNM_PL_TIMER(instIdx, CANNM_CHANNEL_MSGCYCLEOFFSET(CANNM_INST(instIdx))));
    /* !LINKSTO CANNM179,1, CANNM206,1 */
    /* Deviation MISRAC2012-1 */
    CanNm_NmTimerStart(CANNM_PL_TIMER(instIdx, CANNM_CHANNEL_CONFIG(instIdx).CanNmTime));
    /* !LINKSTO CANNM180_Refine,1 */
    CanNm_UniTimerStart(
      /* Deviation MISRAC2012-1 */
      CANNM_PL_TIMER(instIdx, CANNM_CHANNEL_CONFIG(instIdx).RSITime));
  }
  else
  {
    /* !LINKSTO CANNM173,1, CANNM051,1 */
    CanNm_MsgCycleTimerStop(CANNM_PL_SF(instIdx));
    /* !LINKSTO CANNM174,1 */
    CanNm_NmTimerStop(CANNM_PL_SF(instIdx));
    /* !LINKSTO CANNM175,1 */
    CanNm_UniTimerStop(CANNM_PL_SF(instIdx));
#if (CANNM_IMMEDIATE_TRANSMISSION == STD_ON)
    CANNM_CHANNEL_STATUS(instIdx).ImmediateNmTransmissionCounter = 0U;
#endif /* (CANNM_IMMEDIATE_TRANSMISSION == STD_ON) */
#if (CANNM_IMMEDIATE_RESTART_ENABLED == STD_ON)
    CANNM_CHANNEL_STATUS(instIdx).MessageFlags &= \
      (uint8)(~(uint8)(CANNM_EXECUTE_IMMEDIATE_RESTART));
#endif /* CANNM_IMMEDIATE_RESTART_ENABLED == STD_ON */
  }
}
#endif

/* !LINKSTO CANNM072,1, CANNM237,1 */
/* Deviation MISRAC2012-1 */
#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
FUNC(void, CANNM_CODE) CanNm_HandleTransmit(
  CANNM_PDL_SF(uint8 instIdx))
{
  PduInfoType pduInfo;
  Std_ReturnType retVal = E_NOT_OK;

  if ((CANNM_CHANNEL_STATUS(instIdx).MessageFlags & CANNM_MESSAGE_SENT_ON_CYCLE) == 0)
  {
    /* prepare pduInfo */
    pduInfo.SduDataPtr = CANNM_CHANNEL_STATUS(instIdx).TxPduPtr;
    pduInfo.SduLength  = CANNM_CHANNEL_PDULENGTH(instIdx);

#if ( CANNM_COM_USER_DATA_SUPPORT == STD_ON )
    CanNm_GetPduUserData(CANNM_INST(instIdx), &pduInfo);
#endif

    retVal = CanIf_Transmit(CANNM_CHANNEL_TXPDUID(instIdx), &pduInfo);
    /*this is used to verify if current or a previous CanIf_Transmit returned E_OK*/
    if(retVal == E_OK)
    {
      CANNM_CHANNEL_STATUS(instIdx).FirstCanIfTransmitOk = TRUE;
#if (CANNM_IMMEDIATE_TXCONF_ENABLED == STD_OFF)
      /* !LINKSTO CANNM064_Refine,2 */
      SchM_Enter_CanNm_SCHM_CANNM_EXCLUSIVE_AREA_0();
      CANNM_TX_TIMER_START(instIdx, CANNM_CHANNEL_CONFIG(instIdx).MsgTimeoutTime);
      SchM_Exit_CanNm_SCHM_CANNM_EXCLUSIVE_AREA_0();
#else
      /* !LINKSTO CANNM099_Refine_2,1 */
      /* Deviation MISRAC2012-1 */
      CanNm_NmTimerStart(CANNM_PL_TIMER(instIdx, CANNM_CHANNEL_CONFIG(instIdx).CanNmTime));
#endif
      CANNM_CHANNEL_STATUS(instIdx).MessageFlags |= (uint8)CANNM_MESSAGE_SENT_ON_CYCLE;
    }
  }

  /* !LINKSTO CanNm.EB.CanNmRetryFirstMessageRequest,2 */
  /* !LINKSTO CanNm.EB.CanNmRetryFirstMessageRequestReadySleepState_1,2 */
  /* !LINKSTO CanNm.EB.CanNmRetryFirstMessageRequestNormalOperationState,2 */
  /* !LINKSTO CanNm.EB.CanNmRetryFirstMessageRequestReadySleepState_2,2 */
  if( (CANNM_CHANNEL_STATUS(instIdx).FirstCanIfTransmitOk == FALSE) &&
      (CANNM_CHANNEL_STATUS(instIdx).NmTimerExpired == FALSE))
  {
    /* !LINKSTO CanNm.EB.CanNmRetryFirstMessageRequestRepeatMessageState,2 */
    if ((CANNM_CHANNEL_STATUS(instIdx).MessageFlags & CANNM_IN_REPEAT_MESSAGE_STATE) != 0U)
    {
      /* Deviation MISRAC2012-1 */
      CanNm_RmsTimerStart(CANNM_PL_TIMER(instIdx, CANNM_CHANNEL_CONFIG(instIdx).RMSTime + 1U));
    }
    CanNm_MsgCycleTimerStart(CANNM_PL_TIMER(instIdx, 1U));
  }
  else
  {
#if (CANNM_IMMEDIATE_RESTART_ENABLED == STD_ON)
    if ((CANNM_CHANNEL_STATUS(instIdx).MessageFlags & CANNM_EXECUTE_IMMEDIATE_RESTART) != 0U)
    {
      /* Deviation MISRAC2012-1 */
      CanNm_MsgCycleTimerStart(CANNM_PL_TIMER(instIdx, CANNM_CHANNEL_MSGCYCLEOFFSET(CANNM_INST(instIdx))));
      CANNM_CHANNEL_STATUS(instIdx).MessageFlags &= \
        (uint8)(~(uint8)(CANNM_EXECUTE_IMMEDIATE_RESTART));
    }
    else
#endif /* CANNM_IMMEDIATE_RESTART_ENABLED == STD_ON */
#if (CANNM_IMMEDIATE_TRANSMISSION == STD_ON)
    if (CANNM_CHANNEL_STATUS(instIdx).ImmediateNmTransmissionCounter > 0)
    {
      if (retVal == E_OK)
      {
        /* Decrement the counter */
        /* !LINKSTO EB_CANNM334_Refine_1,1, EB_CANNM334_Refine_2,1, SWS_CanNm_00335,1 */
        --CANNM_CHANNEL_STATUS(instIdx).ImmediateNmTransmissionCounter;
        if (CANNM_CHANNEL_STATUS(instIdx).ImmediateNmTransmissionCounter > 0U)
        {
          /* Deviation MISRAC2012-1 */
          CanNm_MsgCycleTimerStart(
            CANNM_PL_TIMER(instIdx, CANNM_CHANNEL_CONFIG(instIdx).ImmediateNmCycleTime));
        }
        else
        {
          /* Deviation MISRAC2012-1 */
          CanNm_MsgCycleTimerStart(
            CANNM_PL_TIMER(instIdx, CANNM_CHANNEL_CONFIG(instIdx).MsgCycleTime));
        }
      }
      else
      {
        if ((CANNM_CHANNEL_STATUS(instIdx).MessageFlags & CANNM_MESSAGE_SENT_ON_CYCLE) == 0)
        {
          CanNm_MsgCycleTimerStart(CANNM_PL_TIMER(instIdx, 1U));
        }
        else
        {
          /* Deviation MISRAC2012-1 */
          CanNm_MsgCycleTimerStart(
            CANNM_PL_TIMER(instIdx, CANNM_CHANNEL_CONFIG(instIdx).ImmediateNmCycleTime));
        }
      }
    }
    else
#endif /* (CANNM_IMMEDIATE_TRANSMISSION == STD_ON) */
    {
      /* !LINKSTO CANNM040,1 */
      /* Deviation MISRAC2012-1 */
      CanNm_MsgCycleTimerStart(
        CANNM_PL_TIMER(instIdx, CANNM_CHANNEL_CONFIG(instIdx).MsgCycleTime));
    }
  }
}
#endif

#if (CANNM_STATE_CHANGE_IND_ENABLED == STD_ON)
STATIC FUNC(void, CANNM_CODE) CanNm_Call_Nm_StateChangeNotification
(
  uint8 instIdx,
  Nm_StateType nmPreviousState,
  Nm_StateType nmCurrentState,
  boolean      compare
)
{
  boolean callStateChangeNotification = FALSE;
#if (CANNM_HSM_INST_MULTI_ENABLED == STD_OFF)
  TS_PARAM_UNUSED(instIdx);
#endif
  if (compare == FALSE)
  {
    callStateChangeNotification = TRUE;
  }
  else
  {
    if (nmPreviousState != nmCurrentState)
    {
      callStateChangeNotification = TRUE;
    }
  }
  if (callStateChangeNotification == TRUE)
  {
  /* !LINKSTO CANNM166,1 */
#if (CANNM_MULTICORE_ENABLED == STD_OFF)
    CANNM_CALL_NM_STATECHANGENOTIFICATION(
      instIdx,
      nmPreviousState,
      nmCurrentState);
#else
    if (CANNM_CALL_NM_STATECHANGENOTIFICATION(
          instIdx,
          nmPreviousState,
          nmCurrentState) != SCHM_E_OK)
    {
#if (CANNM_DEV_ERROR_DETECT == STD_ON)
      /* !LINKSTO CanNm.EB.SchM_Call_DET,1 */
      CANNM_DET_REPORT_ERROR(instIdx,
                             CANNM_SERVID_MAINFUNCTION_X,
                             CANNM_E_STATECHANGENOTIFICATION);
#endif
    }
#endif
  }
}
#endif

#define CANNM_STOP_SEC_CODE
#include <CanNm_MemMap.h>

/*==================[end of file]===========================================*/
