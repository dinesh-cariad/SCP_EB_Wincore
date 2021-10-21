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

/* Implementation of the state handler functions of the state machine
 * UdpNm.
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
 *
 * MISRAC2012-2) Deviated Rule: 2.2 (required)
 * There shall be no dead code.
 *
 * Reason: UdpNm_HsmUdpNmSfTOPEntry is always accessed during initialization.
 */

/* CHECK: RULE 301 OFF (this file is partly generated, some lines may be
 * longer then 100 characters) */

/*==================[inclusions]============================================*/
#define UDPNM_NO_CFGCLASSMIX_REQUIRED
#ifndef UDPNM_NO_PBCFG_REQUIRED
#define UDPNM_NO_PBCFG_REQUIRED
#endif

#include <UdpNm_Trace.h>
#include <TSMem.h>
#include <UdpNm_Hsm.h>                  /* state machine driver interface */
#include <UdpNm_HsmUdpNm.h>             /* public statechart model definitions */
#include <UdpNm_HsmUdpNmInt.h>          /* internal statechart model definitions */
#include <UdpNm_Int.h>                  /* Module intenal definitions */

/* !LINKSTO EB_SWS_UdpNm_00083_1,1 */
#include <SoAd.h>

/* !LINKSTO EB_SWS_UdpNm_00083_2,1 */
#include <Nm_Cbk.h>                     /* Declaration of Nm callback APIs */
#include <SchM_UdpNm.h>                 /* SchM-header for UdpNm */
#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>                        /* Det API */
#endif

/*==================[macros]================================================*/
/** \brief (Re-)Start Nm Timeout Timer.
 ** \param instIdx index of state machine instance to work on
 ** \param TimeoutTime value of the Nm Timeout Timer */

#if (defined UDPNM_NM_TIMER_START)
#error UDPNM_NM_TIMER_START already defined
#endif
/* !LINKSTO SWS_UdpNm_00206,1, UdpNm.EB.UdpNmTimeoutTime,1 */
#define UDPNM_NM_TIMER_START(instIdx, TimeoutTime)                                              \
  do                                                                                            \
  {                                                                                             \
    if (!TS_IsBitSet(&UDPNM_CHANNEL_STATUS(instIdx).ChanStatus, UDPNM_COM_DISABLED_BIT, uint8)) \
    {                                                                                           \
      TS_AtomicAssign16(UDPNM_CHANNEL_STATUS(instIdx).UdpNmTimeoutTimer, (TimeoutTime));        \
    }                                                                                           \
  } while (0)

/** \brief Stop Nm Timeout Timer.
 ** \param instIdx index of state machine instance to work on */
#if (defined UDPNM_NM_TIMER_STOP)
#error UDPNM_NM_TIMER_STOP already defined
#endif
#define UDPNM_NM_TIMER_STOP(instIdx)                                                      \
  do                                                                                      \
  {                                                                                       \
    TS_AtomicAssign16(UDPNM_CHANNEL_STATUS(instIdx).UdpNmTimeoutTimer, 0U);               \
  } while (0)

/** \brief Start Universal Timer. This is used for:
 **        UDPNM_REMOTE_SLEEP_IND_TIME if in Normal Operation state
 **        UDPNM_WAIT_BUS_SLEEP_TIME if in Prepare Bus Sleep state
 ** \param instIdx index of state machine instance to work on
 ** \param TimeoutTime Universal Timer value */
#if (defined UDPNM_UNI_TIMER_START)
#error UDPNM_UNI_TIMER_START already defined
#endif
#define UDPNM_UNI_TIMER_START(instIdx, TimeoutTime)                                       \
  do                                                                                      \
  {                                                                                       \
    TS_AtomicAssign16(UDPNM_CHANNEL_STATUS(instIdx).UniversalTimer, (TimeoutTime));       \
  } while (0)

/** \brief Stop Universal Timer. This is used for:
 **        UDPNM_REMOTE_SLEEP_IND_TIME if in Normal Operation state
 **        UDPNM_WAIT_BUS_SLEEP_TIME if in Prepare Bus Sleep state
 ** \param instIdx index of state machine instance to work on */
#if (defined UDPNM_UNI_TIMER_STOP)
#error UDPNM_UNI_TIMER_STOP already defined
#endif
#define UDPNM_UNI_TIMER_STOP(instIdx)                                                     \
  do                                                                                      \
  {                                                                                       \
    TS_AtomicAssign16(UDPNM_CHANNEL_STATUS(instIdx).UniversalTimer, 0U);                  \
  } while (0)

/** \brief Start Repeat Message Timer.
 ** \param instIdx index of state machine instance to work on
 ** \param TimeoutTime Repeat Message Timer value */
#if (defined UDPNM_RMS_TIMER_START)
#error UDPNM_RMS_TIMER_START already defined
#endif
#define UDPNM_RMS_TIMER_START(instIdx, TimeoutTime)                                       \
  do                                                                                      \
  {                                                                                       \
    TS_AtomicAssign16(UDPNM_CHANNEL_STATUS(instIdx).RmsTimer, (TimeoutTime));             \
    if ((TimeoutTime) == 0U)                                                              \
    {                                                                                     \
      /* Emit the event RMS_TIMEOUT if timer expires immediately */                       \
      (void)UDPNM_HSMEMITTOSELFINST(&UdpNm_HsmScUdpNm,                                    \
                                    instIdx,                                              \
                                    UDPNM_HSM_UDPNM_EV_RMS_TIMEOUT                        \
                                   );                                                     \
    }                                                                                     \
  } while (0)

/** \brief Start Repeat Message Timer.
 ** \param instIdx index of state machine instance to work on
 ** \param TimeoutTime Repeat Message Timer value */
#if (defined UDPNM_RMS_TIMER_START_NO_EMIT)
#error UDPNM_RMS_TIMER_START_NO_EMIT already defined
#endif
#define UDPNM_RMS_TIMER_START_NO_EMIT(instIdx, TimeoutTime)                               \
  do                                                                                      \
  {                                                                                       \
    TS_AtomicAssign16(UDPNM_CHANNEL_STATUS(instIdx).RmsTimer, (TimeoutTime));             \
  } while (0)

#if (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)

/** \brief Start UdpNm Message Cycle Timer.
 ** \param instIdx index of state machine instance to work on
 ** \param TimeoutTime UdpNm Message Cycle Timer value */
#if (defined UDPNM_MSG_CYCLE_TIMER_START)
#error UDPNM_MSG_CYCLE_TIMER_START already defined
#endif
#define UDPNM_MSG_CYCLE_TIMER_START(instIdx, TimeoutTime)                                          \
  do                                                                                               \
  {                                                                                                \
    TS_AtomicAssign16(UDPNM_CHANNEL_STATUS(instIdx).MsgCycleTimer, ((UdpNm_TimeType)(TimeoutTime))); \
    if((TimeoutTime) == 0U)                                                                        \
    {                                                                                              \
      /* Emit the event MSG_CYCLE_TIMEOUT if timer expires immediately */                          \
      (void)UDPNM_HSMEMITTOSELFINST(&UdpNm_HsmScUdpNm,                                             \
                                    (instIdx),                                                     \
                                    UDPNM_HSM_UDPNM_EV_MSG_CYCLE_TIMEOUT                           \
                                   );                                                              \
    }                                                                                              \
  } while (0)

/** \brief Start UdpNm Message Cycle Timer.
 ** \param instIdx index of state machine instance to work on
 ** \param TimeoutTime UdpNm Message Cycle Timer value */
#if (defined UDPNM_MSG_CYCLE_TIMER_START_NO_EMIT)
#error UDPNM_MSG_CYCLE_TIMER_START_NO_EMIT already defined
#endif
#define UDPNM_MSG_CYCLE_TIMER_START_NO_EMIT(instIdx, TimeoutTime)                         \
  do                                                                                      \
  {                                                                                       \
    TS_AtomicAssign16(UDPNM_CHANNEL_STATUS(instIdx).MsgCycleTimer, (TimeoutTime));        \
  } while (0)

/** \brief Stop UdpNm Message Cycle Timer.
 ** \param instIdx index of state machine instance to work on */
#if (defined UDPNM_MSG_CYCLE_TIMER_STOP)
#error UDPNM_MSG_CYCLE_TIMER_STOP already defined
#endif
#define UDPNM_MSG_CYCLE_TIMER_STOP(instIdx)                                               \
  do                                                                                      \
  {                                                                                       \
    TS_AtomicAssign16(UDPNM_CHANNEL_STATUS(instIdx).MsgCycleTimer, 0U);                   \
  } while (0)

#endif

/** \brief Stop UdpNm Repeat Message Timer .
 ** \param instIdx index of state machine instance to work on */
#if (defined UDPNM_RMS_TIMER_STOP)
#error UDPNM_RMS_TIMER_STOP already defined
#endif
#define UDPNM_RMS_TIMER_STOP(instIdx)                                               \
  do                                                                                      \
  {                                                                                       \
    TS_AtomicAssign16(UDPNM_CHANNEL_STATUS(instIdx).RmsTimer, 0U);                   \
  } while (0)

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/
#define UDPNM_START_SEC_CODE
#include <UdpNm_MemMap.h>

/** \brief Initialize Internal variables of respective channel.
 ** \param instIdx index of state machine instance to work on */
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, UDPNM_CODE) UdpNm_InitIntVar(UDPNM_PDL_SF(uint8 instIdx));

/** \brief Handle the common actions on event Rx Indication in
 **        all sub-states in Network Mode
 ** \param instIdx index of state machine instance to work on */
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, UDPNM_CODE) UdpNm_HandleRxIndicationCommon(UDPNM_PDL_SF(uint8 instIdx));

/** \brief Handle the action on event Rx Indication in
 **        Normal Operation Remote Active state
 ** \param instIdx index of state machine instance to work on */
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, UDPNM_CODE) UdpNm_HandleRxIndication_NOState_RA(UDPNM_PDL_SF(uint8 instIdx));

#if (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
/** \brief Handle the action on event Tx Confirmation
 ** \param instIdx index of state machine instance to work on */
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, UDPNM_CODE) UdpNm_HandleTxConfirmation(UDPNM_PDL_SF(uint8 instIdx));
#endif

#if ((UDPNM_COM_CONTROL_ENABLED == STD_ON) && (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF))
/** \brief Handle the action on event communication ability.
 ** \param instIdx index of state machine instance to work on */
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, UDPNM_CODE) UdpNm_HandleComControl(UDPNM_PDL_SF(uint8 instIdx));
#endif

#if (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
/** \brief Transmit Nm message.
 ** \param instIdx index of state machine instance to work on */
/* Deviation MISRAC2012-1 */
FUNC(void, UDPNM_CODE) UdpNm_HandleTransmit(UDPNM_PDL_SF(uint8 instIdx));

#endif

#if (UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
STATIC FUNC(void, UDPNM_CODE) UdpNm_Call_Nm_RemoteSleepCancellation
(
  uint8 instIdx
);
#endif

#if (UDPNM_PDU_RX_INDICATION_ENABLED == STD_ON)
STATIC FUNC(void, UDPNM_CODE) UdpNm_Call_Nm_PduRxIndication
(
  uint8 instIdx
);
#endif

#if (UDPNM_STATE_CHANGE_IND_ENABLED == STD_ON)
STATIC FUNC(void, UDPNM_CODE) UdpNm_Call_Nm_StateChangeNotification
(
  uint8 instIdx,
  Nm_StateType nmPreviousState,
  Nm_StateType nmCurrentState,
  boolean      compare
);
#endif

#define UDPNM_STOP_SEC_CODE
#include <UdpNm_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#define UDPNM_START_SEC_CODE
#include <UdpNm_MemMap.h>

/* ************************ state functions ******************************* */

/* ************************************************************************
 * State: TOP
 * Parent state: none
 * Init substate: BusSleepMode
 * Transitions originating from this state:
 */

/* Deviation MISRAC2012-1 */
FUNC(void, UDPNM_CODE) UdpNm_HsmUdpNmSfTOPEntry(UDPNM_PDL_SF(const uint8 instIdx))
{
  /* entry action 'InitIntVar();TX_TIMER_STOP(); NmTimerStop(); UniTimerStop(); MsgCycleTimerStop();' */

  /* Deviation MISRAC2012-2 */
  UdpNm_InitIntVar(UDPNM_PL_SF(instIdx));

  UDPNM_NM_TIMER_STOP(instIdx);

  UDPNM_UNI_TIMER_STOP(instIdx);
}

/* ************************************************************************
 * State: BusSleepMode
 * Parent state: TOP
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 * 1) RX_INDICATION[]/Nm_NetworkStartIndication(); #ifdef (...) Nm_PduRxIndication(); #endif --> SWS_UdpNm_00127; SWS_UdpNm_00037; SWS_UdpNm_00164;
 * 2) BusSleepMode -> NetworkMode: NETWORK_START[]/
 * 3) BusSleepMode -> NetworkMode: NET_REQ_STATUS_CHANGED[NetworkRequested==TRUE]/
 */

/* Deviation MISRAC2012-1 */
FUNC(void, UDPNM_CODE) UdpNm_HsmUdpNmSfBusSleepModeEntry(UDPNM_PDL_SF(const uint8 instIdx))
{
  /* entry action '#ifdef (SCI) Nm_StateChangeNotification(ActState, BusSleepState); --> SWS_UdpNm_00166;CurState = BusSleepState; --> SWS_UdpNm_00141;if(prevState == PrepareBusSleep) Nm_BusSleepMode(); --> SWS_UdpNm_00126;' */

#if (UDPNM_ACTIVE_WAKEUP_BIT_ENABLED == STD_ON)
  SchM_Enter_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();
  UDPNM_CHANNEL_STATUS(instIdx).ActiveWakeUp1 = 0U;
  UDPNM_CHANNEL_STATUS(instIdx).ActiveWakeUp2 = 0U;
  SchM_Exit_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();
#endif /* UDPNM_ACTIVE_WAKEUP_BIT_ENABLED == STD_ON */

#if (UDPNM_STATE_CHANGE_IND_ENABLED == STD_ON)
  /* !LINKSTO SWS_UdpNm_00166,1, SWS_UdpNm_00144,1 */
  UdpNm_Call_Nm_StateChangeNotification(UDPNM_INST(instIdx),
                                        UDPNM_CHANNEL_STATUS(instIdx).CurState,
                                        NM_STATE_BUS_SLEEP,
                                        FALSE
                                       );
#endif

  if (UDPNM_CHANNEL_STATUS(instIdx).CurState == NM_STATE_PREPARE_BUS_SLEEP)
  {
    /* !LINKSTO SWS_UdpNm_00126,1 */
#if (UDPNM_MULTICORE_SUPPORT == STD_OFF)
    UDPNM_CALL_NM_BUSSLEEPMODE(UDPNM_INST(instIdx));
#else
    if (UDPNM_CALL_NM_BUSSLEEPMODE(UDPNM_INST(instIdx)) != SCHM_E_OK)
    {
#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
      /* !LINKSTO UdpNm.EB.SchM_Call_DET,1 */
      UDPNM_DET_REPORT_ERROR(UDPNM_INST(instIdx), UDPNM_SERVID_MAINFUNCTION_X, UDPNM_E_BUSSLEEPMODE);
#endif
    }
#endif
  }

  /* !LINKSTO SWS_UdpNm_00141,2 */
  TS_AtomicAssign8(UDPNM_CHANNEL_STATUS(instIdx).CurState, NM_STATE_BUS_SLEEP);
}
/* Deviation MISRAC2012-1 */
FUNC(void, UDPNM_CODE) UdpNm_HsmUdpNmSfBusSleepModeAction1(UDPNM_PDL_SF(const uint8 instIdx))
{
  /* action 'Nm_NetworkStartIndication(); #ifdef (...) Nm_PduRxIndication(); #endif --> SWS_UdpNm_00127; SWS_UdpNm_00037; SWS_UdpNm_00164;'
   * for RX_INDICATION[]/...
   * internal transition */

  /* !LINKSTO SWS_UdpNm_00127,2 */
#if (UDPNM_MULTICORE_SUPPORT == STD_OFF)
  UDPNM_CALL_NM_NETWORKSTARTINDICATION(UDPNM_INST(instIdx));
#else
  if (UDPNM_CALL_NM_NETWORKSTARTINDICATION(UDPNM_INST(instIdx)) != SCHM_E_OK)
  {
#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
    /* !LINKSTO UdpNm.EB.SchM_Call_DET,1 */
    UDPNM_DET_REPORT_ERROR(UDPNM_INST(instIdx), UDPNM_SERVID_MAINFUNCTION_X, UDPNM_E_NETWORKSTARTINDICATION);
#endif
  }
#endif

#if (UDPNM_PDU_RX_INDICATION_ENABLED == STD_ON)
  /* !LINKSTO SWS_UdpNm_00037,1, SWS_UdpNm_00165,1 */
  UdpNm_Call_Nm_PduRxIndication(UDPNM_INST(instIdx));
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(boolean, UDPNM_CODE) UdpNm_HsmUdpNmSfBusSleepModeGuard3(UDPNM_PDL_SF(const uint8 instIdx))
{
  boolean ret;

  /* guard condition 'NetworkRequested==TRUE'
   * for NET_REQ_STATUS_CHANGED[...]/
   * external transition to state NetworkMode */

  /* !LINKSTO SWS_UdpNm_00129,1 */
  ret = (boolean)((TS_IsBitSet(&UDPNM_CHANNEL_STATUS(instIdx).ChanStatus,
                               UDPNM_NETWORK_REQUESTED_BIT,
                               uint8
                              )
                  ) ? TRUE : FALSE
                 );

  return ret;
}

/* ************************************************************************
 * State: NetworkMode
 * Parent state: TOP
 * Init substate: SendingSubMode
 * Transitions originating from this state:
 * 1) COM_CONTROL[]/#if (UDPNM_COM_CONTROL_ENABLED == STD_ON) UdpNm_HandleComControl() -> SWS_UdpNm_00176; SWS_UdpNm_00170;
 * 2) RX_INDICATION[]/HandleRxIndicationCommon() --> SWS_UdpNm_00037; SWS_UdpNm_00098;SWS_UdpNm_00164;
 * 3) TX_CONFIRMATION[]/HandleTxConfirmation(); --> SWS_UdpNm_00099;
 * 4) NetworkMode -> NetworkMode: NET_REQ_STATUS_CHANGED[PnHandleMultipleNetworkRequests==TRUE && NetworkRequested==TRUE]/
 */

/* Deviation MISRAC2012-1 */
FUNC(void, UDPNM_CODE) UdpNm_HsmUdpNmSfNetworkModeEntry(UDPNM_PDL_SF(const uint8 instIdx))
{
  /* entry action 'NmTimerStart(UDPNM_TIMEOUT_TIME) --> SWS_UdpNm_00096; Nm_NetworkMode(); -->SWS_UdpNm_00097; if(ChanStatus & NETWORK_REQUESTED) SetActiveWakeupBit(); */

  /* !LINKSTO SWS_UdpNm_00096,1, SWS_UdpNm_00206,1 */
  UDPNM_NM_TIMER_START(instIdx, UDPNM_CHANNEL_CONFIG(instIdx).UdpNmTimeoutTime);

  /* !LINKSTO SWS_UdpNm_00097,1 */
#if (UDPNM_MULTICORE_SUPPORT == STD_OFF)
  UDPNM_CALL_NM_NETWORKMODE(UDPNM_INST(instIdx));
#else
  if(UDPNM_CALL_NM_NETWORKMODE(UDPNM_INST(instIdx)) != SCHM_E_OK)
  {
#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
    /* !LINKSTO UdpNm.EB.SchM_Call_DET,1 */
    UDPNM_DET_REPORT_ERROR(UDPNM_INST(instIdx), UDPNM_SERVID_MAINFUNCTION_X, UDPNM_E_NETWORKMODE);
#endif
  }
#endif

#if (UDPNM_ACTIVE_WAKEUP_BIT_ENABLED == STD_ON)
  /* !LINKSTO UdpNm.EB.UdpNm_NetworkGwEraRequest.ActiveWakeupBit.BusSleepMode,1 */
  /* !LINKSTO UdpNm.EB.UdpNm_NetworkGwEraRequest.ActiveWakeupBit.PrepareBusSleepMode,1 */
  /* !LINKSTO UdpNm.EB.PassiveStartup.NetworkRequest.Sequence.ActiveWakeupBit.BusSleepMode,1 */
  /* !LINKSTO UdpNm.EB.PassiveStartup.NetworkRequest.Sequence.ActiveWakeupBit.PrepareBusSleepMode,1 */
  if((UDPNM_CHANNEL_STATUS(instIdx).ActiveWakeUp1 == UDPNM_NETWORK_REQUEST_AWB) &&
     ((UDPNM_CHANNEL_STATUS(instIdx).CurState == NM_STATE_PREPARE_BUS_SLEEP) ||
      (UDPNM_CHANNEL_STATUS(instIdx).CurState == NM_STATE_BUS_SLEEP))
    )
  {
    /* !LINKSTO EB_SWS_UdpNm_00366_1,1, EB_SWS_UdpNm_00366_2,1 */
    TS_AtomicSetBit_8(&UDPNM_CHANNEL_STATUS(instIdx).TxPduPtr[UDPNM_CHANNEL_CONFIG(instIdx).CbvPos],
                      UDPNM_CBV_ACTIVEWAKEUPBIT
                     );
  }
#endif
#if (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
  UDPNM_CHANNEL_STATUS(instIdx).NmTimerExpired = FALSE;
  UDPNM_CHANNEL_STATUS(instIdx).FirstSoAdIfTransmitOk = FALSE;
#endif
  UDPNM_CHANNEL_STATUS(instIdx).MessageFlags |= (uint8)UDPNM_RMS_ENTERED_FROM_NM;
}
/* Deviation MISRAC2012-1 */
FUNC(void, UDPNM_CODE) UdpNm_HsmUdpNmSfNetworkModeAction1(UDPNM_PDL_SF(const uint8 instIdx))
{
  /* action '#if (UDPNM_COM_CONTROL_ENABLED == STD_ON) UdpNm_HandleComControl() -> SWS_UdpNm_00176; SWS_UdpNm_00170;'
   * for COM_CONTROL[]/...
   * internal transition */

#if ((UDPNM_COM_CONTROL_ENABLED == STD_ON) && (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF))
  /* !LINKSTO SWS_UdpNm_00176,1, SWS_UdpNm_00170,2 */
  UdpNm_HandleComControl(UDPNM_PL_SF(instIdx));
#elif (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(void, UDPNM_CODE) UdpNm_HsmUdpNmSfNetworkModeAction2(UDPNM_PDL_SF(const uint8 instIdx))
{
  /* action 'HandleRxIndicationCommon() --> SWS_UdpNm_00037; SWS_UdpNm_00098;SWS_UdpNm_00164;'
   * for RX_INDICATION[]/...
   * internal transition */

  /* !LINKSTO SWS_UdpNm_00098,1 */
  UdpNm_HandleRxIndicationCommon(UDPNM_PL_SF(instIdx));
}
/* Deviation MISRAC2012-1 */
FUNC(void, UDPNM_CODE) UdpNm_HsmUdpNmSfNetworkModeAction3(UDPNM_PDL_SF(const uint8 instIdx))
{
  /* action 'HandleTxConfirmation(); --> SWS_UdpNm_00099;'
   * for TX_CONFIRMATION[]/...
   * internal transition */

#if (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
  /* !LINKSTO SWS_UdpNm_00099,2 */
  UdpNm_HandleTxConfirmation(UDPNM_PL_SF(instIdx));
#elif (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(boolean, UDPNM_CODE) UdpNm_HsmUdpNmSfNetworkModeGuard4(UDPNM_PDL_SF(const uint8 instIdx))
{
  boolean ret = FALSE;

  /* guard condition 'PnHandleMultipleNetworkRequests==TRUE && NetworkRequested==TRUE'
   * for NET_REQ_STATUS_CHANGED[...]/
   * external transition to state NetworkMode */
  /* !LINKSTO EB_SWS_UdpNm_00362_1,1, EB_SWS_UdpNm_00362_2,1, EB_SWS_UdpNm_00362_3,1 */

#if (UDPNM_PN_SUPPORTED == STD_ON)
  if ((UDPNM_CHANNEL_PN_HANDLE_MULTI_REQ(instIdx) == TRUE) &&
      (TS_IsBitSet(&UDPNM_CHANNEL_STATUS(instIdx).ChanStatus, UDPNM_NETWORK_REQUESTED_BIT, uint8))
     )
  {
    ret = TRUE;
  }
  else
  {
    ret = FALSE;
  }
#elif (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
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
FUNC(void, UDPNM_CODE) UdpNm_HsmUdpNmSfReadySleepStateEntry(UDPNM_PDL_SF(const uint8 instIdx))
{
  /* entry action '#ifdef (SCI) Nm_StateChangeNotification(LastState, ActState) --> SWS_UdpNm_00166;CurState = ReadySleepState;' */

#if (UDPNM_STATE_CHANGE_IND_ENABLED == STD_ON)
  /* !LINKSTO SWS_UdpNm_00166,1 */
  UdpNm_Call_Nm_StateChangeNotification(UDPNM_INST(instIdx),
                                        UDPNM_CHANNEL_STATUS(instIdx).CurState,
                                        NM_STATE_READY_SLEEP,
                                        TRUE
                                       );
#endif

  TS_AtomicAssign8(UDPNM_CHANNEL_STATUS(instIdx).CurState, NM_STATE_READY_SLEEP);
}
/* Deviation MISRAC2012-1 */
FUNC(boolean, UDPNM_CODE) UdpNm_HsmUdpNmSfReadySleepStateGuard1(UDPNM_PDL_SF(const uint8 instIdx))
{
  boolean ret;

  /* guard condition 'NmTimer == 0'
   * for NM_TIMEOUT[...]/
   * external transition to state PrepareBusSleepMode */
  /* !LINKSTO SWS_UdpNm_00109,1 */
  ret = (boolean)((UDPNM_CHANNEL_STATUS(instIdx).UdpNmTimeoutTimer == 0U) ? TRUE : FALSE);

  return ret;
}

/* ************************************************************************
 * State: ReadySleepRemoteActivity
 * Parent state: ReadySleepState
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 * 1) ReadySleepRemoteActivity -> NormalOperationRemoteActivity: NET_REQ_STATUS_CHANGED[PnHandleMultipleNetworkRequests==FALSE && NetworkRequested==TRUE]/HandleTransmit()
 * 2) ReadySleepRemoteActivity -> RepeatMessageState: REPEAT_MESSAGE_REASON[]/
 */

/* Deviation MISRAC2012-1 */
FUNC(void, UDPNM_CODE) UdpNm_HsmUdpNmSfReadySleepRemoteActivityEntry(
  UDPNM_PDL_SF(const uint8 instIdx))
{
  /* entry action 'Clear RSI' */
#if (UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
  /* clear RSI Status */
  TS_AtomicClearBit_8(&UDPNM_CHANNEL_STATUS(instIdx).ChanStatus, UDPNM_RSI_CALLED_BIT);
#elif (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}

/* Deviation MISRAC2012-1 */
FUNC(boolean, UDPNM_CODE) UdpNm_HsmUdpNmSfReadySleepRemoteActivityGuard1(
  UDPNM_PDL_SF(const uint8 instIdx))
{
  boolean ret;

  /* guard condition 'PnHandleMultipleNetworkRequests==FALSE && NetworkRequested==TRUE'
   * for NET_REQ_STATUS_CHANGED[...]/HandleTransmit()
   * external transition to state NormalOperationRemoteActivity */
  /* !LINKSTO SWS_UdpNm_00110,1 */
#if (UDPNM_PN_SUPPORTED == STD_ON)
  if ((UDPNM_CHANNEL_PN_HANDLE_MULTI_REQ(instIdx) == FALSE) &&
      (TS_IsBitSet(&UDPNM_CHANNEL_STATUS(instIdx).ChanStatus, UDPNM_NETWORK_REQUESTED_BIT, uint8))
     )
#else
  if (TS_IsBitSet(&UDPNM_CHANNEL_STATUS(instIdx).ChanStatus, UDPNM_NETWORK_REQUESTED_BIT, uint8))
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
FUNC(void, UDPNM_CODE) UdpNm_HsmUdpNmSfReadySleepRemoteActivityAction1(UDPNM_PDL_SF(const uint8 instIdx))
{

  /* action 'HandleTransmit()'
   * for NET_REQ_STATUS_CHANGED[PnHandleMultipleNetworkRequests==FALSE && NetworkRequested==TRUE]/...
   * external transition to state NormalOperationRemoteActivity */
#if (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
  if (!TS_IsBitSet(&UDPNM_CHANNEL_STATUS(instIdx).ChanStatus, UDPNM_COM_DISABLED_BIT, uint8))
  {
    /* !LINKSTO SWS_UdpNm_00006,1, SWS_UdpNm_00116_1,1, SWS_UdpNm_00116_2,1, SWS_UdpNm_00072,2 */
    UDPNM_MSG_CYCLE_TIMER_START(instIdx, 0U);
  }
#elif (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}

/* ************************************************************************
 * State: ReadySleepRemoteSleep
 * Parent state: ReadySleepState
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 * 1) ReadySleepRemoteSleep -> NormalOperationRemoteSleep: NET_REQ_STATUS_CHANGED[PnHandleMultipleNetworkRequests==FALSE && NetworkRequested==TRUE]/HandleTransmit()
 * 2) ReadySleepRemoteSleep -> RepeatMessageState: REPEAT_MESSAGE_REASON[]/Nm_RemoteSleepCancellation();
 * 3) ReadySleepRemoteSleep -> ReadySleepRemoteActivity: RX_INDICATION[]/Nm_RemoteSleepCancellation(); HandleRxIndicationCommon();
 */

/* Deviation MISRAC2012-1 */
FUNC(boolean, UDPNM_CODE) UdpNm_HsmUdpNmSfReadySleepRemoteSleepGuard1(UDPNM_PDL_SF(const uint8 instIdx))
{
  boolean ret;

  /* guard condition 'PnHandleMultipleNetworkRequests==FALSE && NetworkRequested==TRUE'
   * for NET_REQ_STATUS_CHANGED[...]/HandleTransmit()
   * external transition to state NormalOperationRemoteSleep */
  /* !LINKSTO SWS_UdpNm_00110,1 */
#if (UDPNM_PN_SUPPORTED == STD_ON)
  if ((UDPNM_CHANNEL_PN_HANDLE_MULTI_REQ(instIdx) == FALSE) &&
      (TS_IsBitSet(&UDPNM_CHANNEL_STATUS(instIdx).ChanStatus, UDPNM_NETWORK_REQUESTED_BIT, uint8))
     )
#else
  if (TS_IsBitSet(&UDPNM_CHANNEL_STATUS(instIdx).ChanStatus, UDPNM_NETWORK_REQUESTED_BIT, uint8))
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
FUNC(void, UDPNM_CODE) UdpNm_HsmUdpNmSfReadySleepRemoteSleepAction1(UDPNM_PDL_SF(const uint8 instIdx))
{

  /* action 'HandleTransmit()'
   * for NET_REQ_STATUS_CHANGED[PnHandleMultipleNetworkRequests==FALSE && NetworkRequested==TRUE]/...
   * external transition to state NormalOperationRemoteSleep */
#if (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
  if (!TS_IsBitSet(&UDPNM_CHANNEL_STATUS(instIdx).ChanStatus, UDPNM_COM_DISABLED_BIT, uint8))
  {
    /* !LINKSTO SWS_UdpNm_00006,1, SWS_UdpNm_00116_1,1, SWS_UdpNm_00116_2,1, SWS_UdpNm_00072,2 */
    UDPNM_MSG_CYCLE_TIMER_START(instIdx, 0U);
  }
#elif (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(void, UDPNM_CODE) UdpNm_HsmUdpNmSfReadySleepRemoteSleepAction2(UDPNM_PDL_SF(const uint8 instIdx))
{
  /* action 'Nm_RemoteSleepCancellation();
   * for REPEAT_MESSAGE_REASON[]/...
   * external transition to state RepeatMessageState */

#if (UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
  UdpNm_Call_Nm_RemoteSleepCancellation(UDPNM_INST(instIdx));
#elif (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(void, UDPNM_CODE) UdpNm_HsmUdpNmSfReadySleepRemoteSleepAction3(UDPNM_PDL_SF(const uint8 instIdx))
{
  /* action 'Nm_RemoteSleepCancellation(); HandleRxIndicationCommon();'
   * for RX_INDICATION[]/...
   * external transition to state ReadySleepRemoteActivity */

#if (UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
  /* !LINKSTO EB_SWS_UdpNm_00151_2,1 */
  UdpNm_Call_Nm_RemoteSleepCancellation(UDPNM_INST(instIdx));
#endif

  /* !LINKSTO SWS_UdpNm_00098,1 */
  UdpNm_HandleRxIndicationCommon(UDPNM_PL_SF(instIdx));
}

/* ************************************************************************
 * State: SendingSubMode
 * Parent state: NetworkMode
 * Init substate: RepeatMessageState
 * Transitions originating from this state:
 * 1) NM_TIMEOUT[]/if (NmTimeout == 0) {HandleNmTimeout();} -->SWS_UdpNm_00193; SWS_UdpNm_00194; SWS_UdpNm_00101; SWS_UdpNm_00117; SWS_UdpNm_00206;
 * 2) MSG_CYCLE_TIMEOUT[]/HandleMsgCycleTimeout() --> SWS_UdpNm_00032_Refine;
 */

/* Deviation MISRAC2012-1 */
FUNC(void, UDPNM_CODE) UdpNm_HsmUdpNmSfSendingSubModeExit(UDPNM_PDL_SF(const uint8 instIdx))
{
  /* exit action 'MsgCycleTimerStop(); --> SWS_UdpNm_00051;' */

#if (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
  /* !LINKSTO SWS_UdpNm_00051,1, EB_SWS_UdpNm_00108_2,1 */
  UDPNM_MSG_CYCLE_TIMER_STOP(instIdx);
#elif (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(void, UDPNM_CODE) UdpNm_HsmUdpNmSfSendingSubModeAction1(UDPNM_PDL_SF(const uint8 instIdx))
{
  /* action 'if (NmTimeout == 0) {HandleNmTimeout();} -->SWS_UdpNm_00193; SWS_UdpNm_00194; SWS_UdpNm_00101; SWS_UdpNm_00117; SWS_UdpNm_00206;'
   * for NM_TIMEOUT[]/...
   * internal transition */

  if (UDPNM_CHANNEL_STATUS(instIdx).UdpNmTimeoutTimer == 0U)
  {
    /* !LINKSTO SWS_UdpNm_00206,1, SWS_UdpNm_00117,1, SWS_UdpNm_00101,1 */
    UDPNM_NM_TIMER_START(instIdx, UDPNM_CHANNEL_CONFIG(instIdx).UdpNmTimeoutTime);
  }
}
/* Deviation MISRAC2012-1 */
FUNC(void, UDPNM_CODE) UdpNm_HsmUdpNmSfSendingSubModeAction2(UDPNM_PDL_SF(const uint8 instIdx))
{
  /* action 'UdpNm_HandleTransmit() --> SWS_UdpNm_00032_Refine;'
   * for MSG_CYCLE_TIMEOUT[]/...
   * internal transition */

#if (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
  if (!TS_IsBitSet(&UDPNM_CHANNEL_STATUS(instIdx).ChanStatus, UDPNM_COM_DISABLED_BIT, uint8))
  {
    /* !LINKSTO SWS_UdpNm_00237,2, SWS_UdpNm_00032_Refine,1, SWS_UdpNm_00072,2 */
    UdpNm_HandleTransmit(UDPNM_PL_SF(instIdx));
  }
#elif (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}

/* ************************************************************************
 * State: NormalOperationState
 * Parent state: SendingSubMode
 * Init substate: NormalOperationRemoteActivity
 * Transitions originating from this state:
 */

/* Deviation MISRAC2012-1 */
FUNC(void, UDPNM_CODE) UdpNm_HsmUdpNmSfNormalOperationStateEntry(UDPNM_PDL_SF(const uint8 instIdx))
{
  /* entry action '#ifdef (SCI) Nm_StateChangeNotification(LastState, ActState); --> SWS_UdpNm_00166;CurState = NormalOperationState;' */

#if (UDPNM_STATE_CHANGE_IND_ENABLED == STD_ON)
  /* !LINKSTO SWS_UdpNm_00166,1 */
  UdpNm_Call_Nm_StateChangeNotification(UDPNM_INST(instIdx),
                                        UDPNM_CHANNEL_STATUS(instIdx).CurState,
                                        NM_STATE_NORMAL_OPERATION,
                                        TRUE
                                       );
#endif

  TS_AtomicAssign8(UDPNM_CHANNEL_STATUS(instIdx).CurState, NM_STATE_NORMAL_OPERATION);
}


/* ************************************************************************
 * State: NormalOperationRemoteActivity
 * Parent state: NormalOperationState
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 * 1) RX_INDICATION[]/HandleRxIndication_NOState_RA(); --> SWS_UdpNm_00037;SWS_UdpNm_00164;
 * 2) NormalOperationRemoteActivity -> ReadySleepRemoteActivity: NET_REQ_STATUS_CHANGED[NetworkRequested==FALSE]/
 * 3) NormalOperationRemoteActivity -> RepeatMessageState: REPEAT_MESSAGE_REASON[]/
 * 4) NormalOperationRemoteActivity -> NormalOperationRemoteSleep: UNI_TIMEOUT[(!(ChanStatus  & UDPNM_COM_DISABLED)) && (UniTimer == 0)]/
 */

/* Deviation MISRAC2012-1 */
FUNC(void, UDPNM_CODE) UdpNm_HsmUdpNmSfNormalOperationRemoteActivityEntry(UDPNM_PDL_SF(const uint8 instIdx))
{

  /* entry action '#ifdef (RSI) UniTimerStart(UDPNM_REMOTE_SLEEP_IND_TIME) --> SWS_UdpNm_00150;#ifdef RSI if (RsiStatus == UDPNM_RSI_TRUE) {Nm_RemoteSleepCancellation();} RsiStatus = UDPNM_RSI_FALSE; Clear RSI' */

#if (UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON)

  /* !LINKSTO SWS_UdpNm_00150,1 */
  UDPNM_UNI_TIMER_START(instIdx, UDPNM_CHANNEL_CONFIG(instIdx).RSITime);

  if (TS_IsBitSet(&UDPNM_CHANNEL_STATUS(instIdx).ChanStatus, UDPNM_RSI_CALLED_BIT, uint8))
  {
    UdpNm_Call_Nm_RemoteSleepCancellation(UDPNM_INST(instIdx));
  }

  /* clear RSI Status */
  TS_AtomicClearBit_8(&UDPNM_CHANNEL_STATUS(instIdx).ChanStatus, UDPNM_RSI_CALLED_BIT);

#elif (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(void, UDPNM_CODE) UdpNm_HsmUdpNmSfNormalOperationRemoteActivityAction1(UDPNM_PDL_SF(const uint8 instIdx))
{
  /* action 'HandleRxIndication_NOState_RA(); --> SWS_UdpNm_00037;SWS_UdpNm_00164;'
   * for RX_INDICATION[]/...
   * internal transition */

  /* !LINKSTO SWS_UdpNm_00037,1 */
  UdpNm_HandleRxIndication_NOState_RA(UDPNM_PL_SF(instIdx));
}
/* Deviation MISRAC2012-1 */
FUNC(boolean, UDPNM_CODE) UdpNm_HsmUdpNmSfNormalOperationRemoteActivityGuard2(UDPNM_PDL_SF(const uint8 instIdx))
{
  boolean ret;

  /* guard condition 'NetworkRequested==FALSE'
   * for NET_REQ_STATUS_CHANGED[...]/
   * external transition to state ReadySleepRemoteActivity */

  /* !LINKSTO SWS_UdpNm_00118,1 */
  ret = (boolean)((!TS_IsBitSet(&UDPNM_CHANNEL_STATUS(instIdx).ChanStatus,
                                UDPNM_NETWORK_REQUESTED_BIT,
                                uint8
                               )
                  ) ? TRUE : FALSE
                 );

  return ret;
}
/* Deviation MISRAC2012-1 */
FUNC(boolean, UDPNM_CODE) UdpNm_HsmUdpNmSfNormalOperationRemoteActivityGuard4(UDPNM_PDL_SF(const uint8 instIdx))
{
  boolean ret;

  /* guard condition '(!(ChanStatus & UDPNM_COM_DISABLED)) && (UniTimer == 0)'
   * for UNI_TIMEOUT[...]/
   * external transition to state NormalOperationRemoteSleep */

  ret = (boolean)((!TS_IsBitSet(&UDPNM_CHANNEL_STATUS(instIdx).ChanStatus,
                                UDPNM_COM_DISABLED_BIT,
                                uint8
                               )
                  ) ? TRUE : FALSE
                 );

  return ret;
}
/* Deviation MISRAC2012-1 */
FUNC(void, UDPNM_CODE) UdpNm_HsmUdpNmSfNormalOperationRemoteActivityAction4(UDPNM_PDL_SF(const uint8 instIdx))
{
  /* action 'Nm_RemoteSleepIndication();'
   * for UNI_TIMEOUT[(!(ChanStatus & UDPNM_COM_DISABLED)) && (UniTimer == 0)]/...
   * external transition to state NormalOperationRemoteSleep */

#if (UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
  /* !LINKSTO SWS_UdpNm_00150,1 */
#if (UDPNM_MULTICORE_SUPPORT == STD_OFF)
  UDPNM_CALL_NM_REMOTESLEEPINDICATION(UDPNM_INST(instIdx));
#else
  if (UDPNM_CALL_NM_REMOTESLEEPINDICATION(UDPNM_INST(instIdx)) != SCHM_E_OK)
  {
#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
    /* !LINKSTO UdpNm.EB.SchM_Call_DET,1 */
    UDPNM_DET_REPORT_ERROR(UDPNM_INST(instIdx), UDPNM_SERVID_MAINFUNCTION_X, UDPNM_E_REMOTESLEEPINDICATION);
#endif
  }
#endif
#elif (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
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
 * 3) NormalOperationRemoteSleep -> NormalOperationRemoteActivity: RX_INDICATION[]/UdpNm_HandleRxIndicationCommon() RSI timer is started by entry action /
 */

/* Deviation MISRAC2012-1 */
FUNC(void, UDPNM_CODE) UdpNm_HsmUdpNmSfNormalOperationRemoteSleepEntry(UDPNM_PDL_SF(const uint8 instIdx))
{
  /* entry action 'RsiStatus = UDPNM_RSI_TRUE;Set RSI' */

#if (UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON)

  /* set RSI Status */
  TS_AtomicSetBit_8(&UDPNM_CHANNEL_STATUS(instIdx).ChanStatus, UDPNM_RSI_CALLED_BIT);

#elif (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(boolean, UDPNM_CODE) UdpNm_HsmUdpNmSfNormalOperationRemoteSleepGuard1(UDPNM_PDL_SF(const uint8 instIdx))
{
  boolean ret;

  /* guard condition 'NetworkRequested==FALSE'
   * for NET_REQ_STATUS_CHANGED[...]/
   * external transition to state ReadySleepRemoteSleep */

  /* !LINKSTO SWS_UdpNm_00118,1 */
  ret = (boolean)((!TS_IsBitSet(&UDPNM_CHANNEL_STATUS(instIdx).ChanStatus,
                                UDPNM_NETWORK_REQUESTED_BIT,
                                uint8
                               )
                  ) ? TRUE : FALSE
                 );

  return ret;
}
/* Deviation MISRAC2012-1 */
FUNC(void, UDPNM_CODE) UdpNm_HsmUdpNmSfNormalOperationRemoteSleepAction2(UDPNM_PDL_SF(const uint8 instIdx))
{
  /* action 'Nm_RemoteSleepCancellation();'
   * for REPEAT_MESSAGE_REASON[]/...
   * external transition to state RepeatMessageState */

#if (UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
  /* !LINKSTO SWS_UdpNm_00119,1, SWS_UdpNm_00152,1, EB_SWS_UdpNm_00151_1,1 */
  UdpNm_Call_Nm_RemoteSleepCancellation(UDPNM_INST(instIdx));
#elif (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(void, UDPNM_CODE) UdpNm_HsmUdpNmSfNormalOperationRemoteSleepAction3(UDPNM_PDL_SF(const uint8 instIdx))
{
  /* action 'UdpNm_HandleRxIndicationCommon() RSI timer is started by entry action /'
   * for RX_INDICATION[]/...
   * external transition to state NormalOperationRemoteActivity */

  UdpNm_HandleRxIndicationCommon(UDPNM_PL_SF(instIdx));
}

/* ************************************************************************
 * State: RepeatMessageState
 * Parent state: SendingSubMode
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 * 1) RepeatMessageState -> ReadySleepState: RMS_TIMEOUT[(NetworkRequested==FALSE) && (RMS timer == 0))]/
 * 2) RepeatMessageState -> NormalOperationState: RMS_TIMEOUT[((NetworkRequested==TRUE) && (RMS timer == 0))]/
 */

/* Deviation MISRAC2012-1 */
/* !LINKSTO EB_SWS_UdpNm_00095_1,1 */
FUNC(void, UDPNM_CODE) UdpNm_HsmUdpNmSfRepeatMessageStateEntry(UDPNM_PDL_SF(const uint8 instIdx))
{
  /* entry action '#ifdev (SCI) Nm_StateChangeNotification(LastState, ActState) --> SWS_UdpNm_00166;CurState = RepeatMessageState; RMSTimerStart() --> SWS_UdpNm_00102;Clear RSI' */
#if (UDPNM_IMMEDIATE_RESTART_ENABLED == STD_ON)
  Nm_StateType PrevState = UDPNM_CHANNEL_STATUS(instIdx).CurState;
#endif /* (UDPNM_IMMEDIATE_RESTART_ENABLED == STD_ON) */

  UDPNM_CHANNEL_STATUS(instIdx).MessageFlags &= (uint8)(~(uint8)(UDPNM_MESSAGE_SENT_ON_CYCLE));
  /* !LINKSTO SWS_UdpNm_00102,1 */
  UDPNM_RMS_TIMER_START(instIdx, UDPNM_CHANNEL_CONFIG(instIdx).RMSTime);
  if (UDPNM_CHANNEL_CONFIG(instIdx).RMSTime > 0U)
  {
    UDPNM_CHANNEL_STATUS(instIdx).MessageFlags |= UDPNM_IN_REPEAT_MESSAGE_STATE;
  }

#if (UDPNM_STATE_CHANGE_IND_ENABLED == STD_ON)
  /* !LINKSTO SWS_UdpNm_00166,1, UdpNm.EB.StateChangeNotification,1 */
  UdpNm_Call_Nm_StateChangeNotification(UDPNM_INST(instIdx),
                                        UDPNM_CHANNEL_STATUS(instIdx).CurState,
                                        NM_STATE_REPEAT_MESSAGE,
                                        TRUE
                                       );
#endif

#if (UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
  /* clear RSI Status */
  TS_AtomicClearBit_8(&UDPNM_CHANNEL_STATUS(instIdx).ChanStatus, UDPNM_RSI_CALLED_BIT);

#endif

  TS_AtomicAssign8(UDPNM_CHANNEL_STATUS(instIdx).CurState, NM_STATE_REPEAT_MESSAGE);
/* !LINKSTO EB_SWS_UdpNm_00100_1,1, EB_SWS_UdpNm_00100_2,1 */
/* !LINKSTO EB_SWS_UdpNm_00100_3,1, EB_SWS_UdpNm_00100_4,1 */
#if (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
  /* !LINKSTO SWS_UdpNm_00169,1 */
  if (!TS_IsBitSet(&UDPNM_CHANNEL_STATUS(instIdx).ChanStatus, UDPNM_COM_DISABLED_BIT, uint8))
  {
    if ((UDPNM_CHANNEL_STATUS(instIdx).MessageFlags & UDPNM_RMS_ENTERED_FROM_NM) != 0)
    {
#if (UDPNM_IMMEDIATE_TRANSMISSION == STD_ON)
      /* !LINKSTO EB_SWS_UdpNm_00334_1,1, EB_SWS_UdpNm_00334_2,1, SWS_UdpNm_00454,1 */
      /* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00330,1 */
      /* !LINKSTO UdpNm.EB.UdpNm_NetworkGwEraRequest.UdpNmImmediateNmTransmissions.BusSleepMode,1 */
      /* !LINKSTO UdpNm.EB.UdpNm_NetworkGwEraRequest.UdpNmImmediateNmTransmissions.PrepareBusSleepMode,1 */
      /* !LINKSTO UdpNm.EB.PassiveStartup.NetworkRequest.Sequence.UdpNmImmediateNmTransmissions.BusSleepMode,1 */
      /* !LINKSTO UdpNm.EB.PassiveStartup.NetworkRequest.Sequence.UdpNmImmediateNmTransmissions.PrepareBusSleepMode,1 */
      if(((UDPNM_CHANNEL_STATUS(instIdx).ChanStatus & UDPNM_NETWORK_REQUESTED) != 0) &&
          (UDPNM_CHANNEL_CONFIG(instIdx).ImmediateNmTransmissions != 0))
      {
        /* Load immediate transmission counter with configured value */
        UDPNM_CHANNEL_STATUS(instIdx).ImmediateNmTransmissionCounter =
          UDPNM_CHANNEL_CONFIG(instIdx).ImmediateNmTransmissions;
        /* Start transmission immediately
         * Start message cycle timer with zero.
         * After transition is done to repeat message state
         * (state change notification is called)
         * handling of transmission is done.
         * This way firstly state change notification is called
         * and after that UdpIf_Transmit is called with the updated
         * information.
         */
        UDPNM_MSG_CYCLE_TIMER_START(instIdx, 0U);
      }
      else
#endif /* (UDPNM_IMMEDIATE_TRANSMISSION == STD_ON) */
#if (UDPNM_IMMEDIATE_RESTART_ENABLED == STD_ON)
      /* !LINKSTO UdpNm.EB.UdpNm_NetworkGwEraRequest.UdpNmImmediateRestartEnabled.PrepareBusSleepMode,1 */
      /* !LINKSTO UdpNm.EB.PassiveStartup.NetworkRequest.Sequence.UdpNmImmediateRestartEnabled.PrepareBusSleepMode,1 */
      if (((UDPNM_CHANNEL_STATUS(instIdx).ChanStatus & UDPNM_NETWORK_REQUESTED) != 0) &&
          (PrevState == NM_STATE_PREPARE_BUS_SLEEP))
      {
        /* !LINKSTO SWS_UdpNm_00122,2 */
        UDPNM_CHANNEL_STATUS(instIdx).MessageFlags |= UDPNM_EXECUTE_IMMEDIATE_RESTART;
        /* Start transmission immediately
         * Start message cycle timer with zero.
         * After transition is done to repeat message state
         * (state change notification is called)
         * handling of transmission is done.
         * This way firstly state change notification is called
         * and after that UdpIf_Transmit is called with the updated
         * information.
         */
        UDPNM_MSG_CYCLE_TIMER_START(instIdx, 0U);
      }
      else
#endif /* (UDPNM_IMMEDIATE_RESTART_ENABLED == STD_ON) */
      {
        /* !LINKSTO EB_SWS_UdpNm_00005_1,1 */
        UDPNM_MSG_CYCLE_TIMER_START(instIdx, UDPNM_CHANNEL_MSGCYCLEOFFSET(UDPNM_INST(instIdx)));
      }
    }
    else
    {
      /* !LINKSTO EB_SWS_UdpNm_00005_2,1 */
      UDPNM_MSG_CYCLE_TIMER_START(instIdx, UDPNM_CHANNEL_MSGCYCLEOFFSET(UDPNM_INST(instIdx)));
    }
  }
#endif
  UDPNM_CHANNEL_STATUS(instIdx).MessageFlags &= (uint8)(~(uint8)(UDPNM_RMS_ENTERED_FROM_NM));
}
/* Deviation MISRAC2012-1 */
FUNC(void, UDPNM_CODE) UdpNm_HsmUdpNmSfRepeatMessageStateExit(UDPNM_PDL_SF(const uint8 instIdx))
{
  /* exit action '#if (NODE_DETECTION_ENABLED == true) <Unset Repeat Message Bit> --> SWS_UdpNm_00107;' */
#if ((UDPNM_NODE_DETECTION_ENABLED == STD_ON) && (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF))
  /* !LINKSTO SWS_UdpNm_00107,1 */
  if(UDPNM_CHANNEL_CONFIG(instIdx).UdpNmNodeDetectionEnabled == TRUE)
  {
    TS_AtomicClearBit_8(&UDPNM_CHANNEL_STATUS(instIdx).TxPduPtr[UDPNM_CHANNEL_CONFIG(instIdx).CbvPos],
                      UDPNM_CBV_REPEATMESSAGEBIT
                     );
  }
  else
  {
  }
#elif (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
#if (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
  UDPNM_CHANNEL_STATUS(instIdx).ImmediateNmTransmissionCounter = 0U;
#endif /* (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF) */
  UDPNM_CHANNEL_STATUS(instIdx).MessageFlags &= (uint8)(~(uint8)(UDPNM_IN_REPEAT_MESSAGE_STATE));
  UDPNM_RMS_TIMER_STOP(instIdx);
}
/* Deviation MISRAC2012-1 */
FUNC(boolean, UDPNM_CODE) UdpNm_HsmUdpNmSfRepeatMessageStateGuard1(UDPNM_PDL_SF(const uint8 instIdx))
{
  boolean ret;

  /* guard condition '(NetworkRequested==FALSE) && (RMS timer == 0))'
   * for RMS_TIMEOUT[...]/
   * external transition to state ReadySleepState */

  /* !LINKSTO SWS_UdpNm_00102,1, EB_SWS_UdpNm_00108_1,1, SWS_UdpNm_00106,1 */
  ret = (boolean)(((!TS_IsBitSet(&UDPNM_CHANNEL_STATUS(instIdx).ChanStatus,
                                 UDPNM_NETWORK_REQUESTED_BIT,
                                 uint8
                                )
                              ) && (UDPNM_CHANNEL_STATUS(instIdx).RmsTimer == 0U)) ? TRUE : FALSE);

  return ret;
}
/* Deviation MISRAC2012-1 */
FUNC(boolean, UDPNM_CODE) UdpNm_HsmUdpNmSfRepeatMessageStateGuard2(UDPNM_PDL_SF(const uint8 instIdx))
{
  boolean ret;

  /* guard condition '((NetworkRequested==TRUE) && (RMS timer == 0))'
   * for RMS_TIMEOUT[...]/
   * external transition to state NormalOperationState */

  /* !LINKSTO SWS_UdpNm_00103,1 */
  ret = (boolean)(((TS_IsBitSet(&UDPNM_CHANNEL_STATUS(instIdx).ChanStatus,
                                UDPNM_NETWORK_REQUESTED_BIT,
                                uint8
                               )
                              ) && (UDPNM_CHANNEL_STATUS(instIdx).RmsTimer == 0U)) ? TRUE : FALSE);

  return ret;
}

/* ************************************************************************
 * State: PrepareBusSleepMode
 * Parent state: TOP
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 * 1) PrepareBusSleepMode -> NetworkMode: NETWORK_START[]/
 * 2) PrepareBusSleepMode -> NetworkMode: NET_REQ_STATUS_CHANGED[NetworkRequested==TRUE]/#if (UDPNM_IMMEDIATE_RESTART_ENABLED == true) <Send NM message> #endif
 * 3) PrepareBusSleepMode -> NetworkMode: RX_INDICATION[]/#ifdef (...) Nm_PduRxIndication(); #endif
 * 4) PrepareBusSleepMode -> BusSleepMode: UNI_TIMEOUT[]/
 */

/* Deviation MISRAC2012-1 */
FUNC(void, UDPNM_CODE) UdpNm_HsmUdpNmSfPrepareBusSleepModeEntry(UDPNM_PDL_SF(const uint8 instIdx))
{
  /* entry action '#ifdef (SCI) Nm_StateChangeNotification(LastState, ActState); --> SWS_UdpNm_00166;CurState = PrepareBusSleepMode; Nm_PrepareBusSleepMode(); --> SWS_UdpNm_00114;UniTimerStart(UDPNM_WAIT_BUS_SLEEP_TIME); --> SWS_UdpNm_00115;ClearActiveWakeUpBit()' */
#if (UDPNM_ACTIVE_WAKEUP_BIT_ENABLED == STD_ON)
  SchM_Enter_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();
  UDPNM_CHANNEL_STATUS(instIdx).ActiveWakeUp1 = 0U;
  UDPNM_CHANNEL_STATUS(instIdx).ActiveWakeUp2 = 0U;
  SchM_Exit_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();
#endif /* UDPNM_ACTIVE_WAKEUP_BIT_ENABLED == STD_ON */

  /* !LINKSTO SWS_UdpNm_00115,1 */
  UDPNM_UNI_TIMER_START(instIdx, UDPNM_CHANNEL_CONFIG(instIdx).WBSTime);

#if (UDPNM_STATE_CHANGE_IND_ENABLED == STD_ON)
  /* !LINKSTO SWS_UdpNm_00166,1 */
  UdpNm_Call_Nm_StateChangeNotification(UDPNM_INST(instIdx),
                                        UDPNM_CHANNEL_STATUS(instIdx).CurState,
                                        NM_STATE_PREPARE_BUS_SLEEP,
                                        TRUE
                                       );
#endif

  /* !LINKSTO SWS_UdpNm_00114,1 */
#if (UDPNM_MULTICORE_SUPPORT == STD_OFF)
  UDPNM_CALL_NM_PREPAREBUSSLEEPMODE(UDPNM_INST(instIdx));
#else
  if (UDPNM_CALL_NM_PREPAREBUSSLEEPMODE(UDPNM_INST(instIdx)) != SCHM_E_OK)
  {
#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
    /* !LINKSTO UdpNm.EB.SchM_Call_DET,1 */
    UDPNM_DET_REPORT_ERROR(UDPNM_INST(instIdx), UDPNM_SERVID_MAINFUNCTION_X, UDPNM_E_PREPAREBUSSLEEPMODE);
#endif
  }
#endif

  TS_AtomicAssign8(UDPNM_CHANNEL_STATUS(instIdx).CurState, NM_STATE_PREPARE_BUS_SLEEP);
#if (UDPNM_ACTIVE_WAKEUP_BIT_ENABLED == STD_ON)
  /* Clearing of active wakeup bit has been moved
   * into Prepare Bus Sleep Mode entry
   * from network mode exit since
   * network mode can be re-entered.
   * Active Wakeup bit is set on network mode entry.
   * If UdpNmPnHandleMultipleNetworkRequests is set to true,
   * and if for the first time the network mode was entered
   * with passive startup and while in network mode the network
   * is requested, network mode is re-entered
   * (if UdpNmPnHandleMultipleNetworkRequests is set to true)
   * and in this case active wakeup shall not be set
   */
  /* !LINKSTO SWS_UdpNm_00367,1 */
  TS_AtomicClearBit_8(&UDPNM_CHANNEL_STATUS(instIdx).TxPduPtr[UDPNM_CHANNEL_CONFIG(instIdx).CbvPos],
                      UDPNM_CBV_ACTIVEWAKEUPBIT
                     );
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(boolean, UDPNM_CODE) UdpNm_HsmUdpNmSfPrepareBusSleepModeGuard2(UDPNM_PDL_SF(const uint8 instIdx))
{
  boolean ret;

  /* guard condition 'NetworkRequested==TRUE'
   * for NET_REQ_STATUS_CHANGED[...]/
   * external transition to state NetworkMode */

  /* !LINKSTO SWS_UdpNm_00123,1, EB_SWS_UdpNm_00095_2,1 */
  ret = (boolean)((TS_IsBitSet(&UDPNM_CHANNEL_STATUS(instIdx).ChanStatus,
                               UDPNM_NETWORK_REQUESTED_BIT,
                               uint8
                              )
                  ) ? TRUE : FALSE
                 );

  return ret;
}
/* Deviation MISRAC2012-1 */
FUNC(void, UDPNM_CODE) UdpNm_HsmUdpNmSfPrepareBusSleepModeAction3(UDPNM_PDL_SF(const uint8 instIdx))
{
  /* action '#ifdef (...) Nm_PduRxIndication(); #endif'
   * for RX_INDICATION[]/...
   * external transition to state NetworkMode */

  /* !LINKSTO SWS_UdpNm_00124,1 */
#if (UDPNM_PDU_RX_INDICATION_ENABLED == STD_ON)
  /* !LINKSTO SWS_UdpNm_00037,1, SWS_UdpNm_00164,1 */
  UdpNm_Call_Nm_PduRxIndication(UDPNM_INST(instIdx));
#elif (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}

/*==================[internal function definitions]=========================*/

#if (UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
STATIC FUNC(void, UDPNM_CODE) UdpNm_Call_Nm_RemoteSleepCancellation
(
  uint8 instIdx
)
{
#if (UDPNM_MULTICORE_SUPPORT == STD_OFF)
  UDPNM_CALL_NM_REMOTESLEEPCANCELLATION(instIdx);
#else
  if (UDPNM_CALL_NM_REMOTESLEEPCANCELLATION(instIdx) != SCHM_E_OK)
  {
#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
    /* !LINKSTO UdpNm.EB.SchM_Call_DET,1 */
    UDPNM_DET_REPORT_ERROR(instIdx, UDPNM_SERVID_MAINFUNCTION_X, UDPNM_E_REMOTESLEEPCANCELLATION);
#endif
  }
#endif
#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_OFF)
  TS_PARAM_UNUSED(instIdx);
#endif
}
#endif

#if (UDPNM_PDU_RX_INDICATION_ENABLED == STD_ON)
STATIC FUNC(void, UDPNM_CODE) UdpNm_Call_Nm_PduRxIndication
(
  uint8 instIdx
)
{
#if (UDPNM_MULTICORE_SUPPORT == STD_OFF)
  UDPNM_CALL_NM_PDURXINDICATION(instIdx);
#else
  if (UDPNM_CALL_NM_PDURXINDICATION(instIdx) != SCHM_E_OK)
  {
#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
    /* !LINKSTO UdpNm.EB.SchM_Call_DET,1 */
    UDPNM_DET_REPORT_ERROR(UDPNM_INST(instIdx), UDPNM_SERVID_MAINFUNCTION_X, UDPNM_E_PDURXINDICATION);
#endif
  }
#endif
#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_OFF)
  TS_PARAM_UNUSED(instIdx);
#endif
}
#endif

#if (UDPNM_STATE_CHANGE_IND_ENABLED == STD_ON)
STATIC FUNC(void, UDPNM_CODE) UdpNm_Call_Nm_StateChangeNotification
(
  uint8 instIdx,
  Nm_StateType nmPreviousState,
  Nm_StateType nmCurrentState,
  boolean      compare
)
{
  boolean callStateChangeNotification = FALSE;
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
#if (UDPNM_MULTICORE_SUPPORT == STD_OFF)
    UDPNM_CALL_NM_STATECHANGENOTIFICATION(instIdx,
                                          nmPreviousState,
                                          nmCurrentState
                                         );
#else
    if (UDPNM_CALL_NM_STATECHANGENOTIFICATION(UDPNM_INST(instIdx),
                                              nmPreviousState,
                                              nmCurrentState
                                             ) != SCHM_E_OK)
    {
#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
      /* !LINKSTO UdpNm.EB.SchM_Call_DET,1 */
      UDPNM_DET_REPORT_ERROR(UDPNM_INST(instIdx), UDPNM_SERVID_MAINFUNCTION_X, UDPNM_E_STATECHANGENOTIFICATION);
#endif
    }
#endif
  }
#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_OFF)
  TS_PARAM_UNUSED(instIdx);
#endif
}
#endif

/* Deviation MISRAC2012-1 */
STATIC FUNC(void, UDPNM_CODE) UdpNm_InitIntVar(UDPNM_PDL_SF(uint8 instIdx))
{
#if ((UDPNM_NODE_ID_ENABLED == STD_ON) && (UDPNM_NODEID_CALLBACK_ENABLED == STD_ON))
  uint8 NodeId;
#endif

#if (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
#if ((UDPNM_USER_DATA_ENABLED == STD_ON) || (UDPNM_COM_USER_DATA_SUPPORT == STD_ON))
  uint32 UserDataLength = (uint32)UDPNM_CHANNEL_PDULENGTH(instIdx) - \
                          (uint32)UDPNM_CHANNEL_CONFIG(instIdx).UDFBPos;
  /* !LINKSTO EB_SWS_UdpNm_00025,1 */
  TS_MemSet(&UDPNM_CHANNEL_STATUS(instIdx).TxPduPtr[UDPNM_CHANNEL_CONFIG(instIdx).UDFBPos],
            0xFFU,
            UserDataLength
           );

  TS_MemSet(&UDPNM_CHANNEL_STATUS(instIdx).RxPduPtr[UDPNM_CHANNEL_CONFIG(instIdx).UDFBPos],
            0xFFU,
            UserDataLength
           );
#endif

  /* An X-Path check in the configuration schema already ensures that
   * the position of the CBV is not set to OFF when Node Detection support
   * is enabled. So an additional check is not required here.
   */
  /* !LINKSTO EB_SWS_UdpNm_00085,1, SWS_UdpNm_00333,1 */
  TS_AtomicAssign8(UDPNM_CHANNEL_STATUS(instIdx).TxPduPtr[UDPNM_CHANNEL_CONFIG(instIdx).CbvPos],
                   0x00U
                  );

#if (UDPNM_NODE_ID_ENABLED == STD_ON)
  if(UDPNM_CHANNEL_CONFIG(instIdx).UdpNmNodeIdEnabled == TRUE)
  {
#if (UDPNM_NODEID_CALLBACK_ENABLED == STD_ON)
  NodeId = UDPNM_NODEID_CALLBACK_NAME(UDPNM_CHANNEL_CONFIG(instIdx).nmChannelId);

  /* !LINKSTO UdpNm.EB.UdpNmNodeIdCallback_2,1 */
  /* Set the value returned by Node Id call back function in CBV */
  TS_AtomicAssign8(UDPNM_CHANNEL_STATUS(instIdx).TxPduPtr[UDPNM_CHANNEL_CONFIG(instIdx).NidPos],
                   NodeId
                  );
#else
  /* !LINKSTO SWS_UdpNm_00013,1 */
  TS_AtomicAssign8(UDPNM_CHANNEL_STATUS(instIdx).TxPduPtr[UDPNM_CHANNEL_CONFIG(instIdx).NidPos],
                   UDPNM_CHANNEL_NODEID(UDPNM_INST(instIdx))
                  );
#endif
  }
#endif
#if (UDPNM_PN_SUPPORTED == STD_ON)
  if (UDPNM_CHANNEL_PNENABLED(instIdx) == TRUE)
  {
    /* !LINKSTO SWS_UdpNm_00332,2 */
    TS_AtomicSetBit_8(&UDPNM_CHANNEL_STATUS(instIdx).TxPduPtr[UDPNM_CHANNEL_CONFIG(instIdx).CbvPos],
                      UDPNM_CBV_PNINFOBIT
                     );
  }
#endif
#elif (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif

#if (UDPNM_PN_ERA_CALC_ENABLED == STD_ON)
  /* !LINKSTO SWS_UdpNm_00355,1, UdpNm.ASR431.SWS_UdpNm_00355,1 */
  TS_MemSet(UDPNM_CHANNEL_STATUS(instIdx).Era.PnInfo, 0U, UDPNM_PN_INFO_LENGTH);
  TS_MemSet(UDPNM_CHANNEL_STATUS(instIdx).Era.PnValue, 0U, UDPNM_PN_INFO_LENGTH);
  TS_MemSet(UDPNM_CHANNEL_STATUS(instIdx).Era.PnTimer,
            0U,
            (UDPNM_EIRA_MAX_TIMER_SIZE * sizeof(UdpNm_TimeType))
           );
#endif
}

/* Deviation MISRAC2012-1 */
STATIC FUNC(void, UDPNM_CODE) UdpNm_HandleRxIndicationCommon(UDPNM_PDL_SF(uint8 instIdx))
{

  /* Note: UdpNmTimeoutTime-1 is assigned to reduce the extra main cycle delay
   * during the transition from Ready Sleep State to Prepare Bus-Sleep Mode
   */
  /* !LINKSTO SWS_UdpNm_00206,1, SWS_UdpNm_00098,1 */
  UDPNM_NM_TIMER_START(instIdx, (UDPNM_CHANNEL_CONFIG(instIdx).UdpNmTimeoutTime - 1U));

#if (UDPNM_PDU_RX_INDICATION_ENABLED == STD_ON)
  /* !LINKSTO SWS_UdpNm_00037,1 */
  UdpNm_Call_Nm_PduRxIndication(UDPNM_INST(instIdx));
#endif
}

/* Deviation MISRAC2012-1 */
STATIC FUNC(void, UDPNM_CODE) UdpNm_HandleRxIndication_NOState_RA(UDPNM_PDL_SF(uint8 instIdx))
{

#if (UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
  UDPNM_UNI_TIMER_START(instIdx, UDPNM_CHANNEL_CONFIG(instIdx).RSITime);
#endif

  UdpNm_HandleRxIndicationCommon(UDPNM_PL_SF(instIdx));

}

#if (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, UDPNM_CODE) UdpNm_HandleTxConfirmation(UDPNM_PDL_SF(uint8 instIdx))
{

  /* Note: UDPNM_NM_TIMER_START() occurs only after one main cycle
   * once the TX_CONFIRMATION event is emited. So Nm timer is restarted
   * with UdpNmTimeoutTime - 1 to avoid an extra main cycle delay.
   */
  /* !LINKSTO SWS_UdpNm_00206,1, SWS_UdpNm_00099,2 */
  UDPNM_NM_TIMER_START(instIdx, (UDPNM_CHANNEL_CONFIG(instIdx).UdpNmTimeoutTime - 1U));
}
#endif

#if ((UDPNM_COM_CONTROL_ENABLED == STD_ON) && (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF))
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, UDPNM_CODE) UdpNm_HandleComControl(UDPNM_PDL_SF(uint8 instIdx))
{

  if (!TS_IsBitSet(&UDPNM_CHANNEL_STATUS(instIdx).ChanStatus, UDPNM_COM_DISABLED_BIT, uint8))
  {
    /* !LINKSTO SWS_UdpNm_00178,1 */
    UDPNM_MSG_CYCLE_TIMER_START(instIdx, UDPNM_CHANNEL_MSGCYCLEOFFSET(UDPNM_INST(instIdx)));

    /* !LINKSTO SWS_UdpNm_00206,1, SWS_UdpNm_00179,1 */
    UDPNM_NM_TIMER_START(instIdx, UDPNM_CHANNEL_CONFIG(instIdx).UdpNmTimeoutTime);

    /* !LINKSTO EB_SWS_UdpNm_00180,1 */
    if(UDPNM_CHANNEL_STATUS(instIdx).CurState == NM_STATE_NORMAL_OPERATION)
    {
      UDPNM_UNI_TIMER_START(instIdx, UDPNM_CHANNEL_CONFIG(instIdx).RSITime);
    }
  }
  else
  {
    /* !LINKSTO SWS_UdpNm_00173,1 */
    UDPNM_MSG_CYCLE_TIMER_STOP(instIdx);

    /* !LINKSTO SWS_UdpNm_00174,1 */
    UDPNM_NM_TIMER_STOP(instIdx);

    /* Stop Remote Sleep Indication Timer or Wait Bus Sleep Timer */
    /* !LINKSTO EB_SWS_UdpNm_00175,1, EB_SWS_UdpNm_00180,1 */
    UDPNM_UNI_TIMER_STOP(instIdx);
  }
}
#endif

#if (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
/* Deviation MISRAC2012-1 */
FUNC(void, UDPNM_CODE) UdpNm_HandleTransmit(UDPNM_PDL_SF(uint8 instIdx))
{
  PduInfoType pduInfo;
  Std_ReturnType retVal = E_NOT_OK;
  pduInfo.SduDataPtr = UDPNM_CHANNEL_STATUS(instIdx).TxPduPtr;
  pduInfo.SduLength  = UDPNM_CHANNEL_PDULENGTH(instIdx);
  if ((UDPNM_CHANNEL_STATUS(instIdx).MessageFlags & UDPNM_MESSAGE_SENT_ON_CYCLE) == 0)
  {
#if ( UDPNM_COM_USER_DATA_SUPPORT == STD_ON )
    /* !LINKSTO SWS_UdpNm_00317,1 */
    UdpNm_GetPduUserData(UDPNM_INST(instIdx), &pduInfo);
#endif
    /* !LINKSTO UdpNm.EB.SoAd_IfTransmit_Called_Once,1 */
    retVal = SoAd_IfTransmit(UDPNM_CHANNEL_TXPDUID(instIdx), &pduInfo);

    if(retVal == E_OK)
    {
      UDPNM_CHANNEL_STATUS(instIdx).FirstSoAdIfTransmitOk = TRUE;
      UDPNM_CHANNEL_STATUS(instIdx).MessageFlags |= (uint8)UDPNM_MESSAGE_SENT_ON_CYCLE;
    }

  }
  /* !LINKSTO UdpNm.EB.UdpNmRetryFirstMessageRequest_3,1 */
  /* !LINKSTO UdpNm.EB.UdpNmRetryFirstMessageRequest_4,1 */
  /* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00007,1 */
  if( (UDPNM_CHANNEL_STATUS(instIdx).FirstSoAdIfTransmitOk == FALSE) &&
    (UDPNM_CHANNEL_STATUS(instIdx).NmTimerExpired == FALSE) &&
    (UDPNM_CHANNEL_CONFIG(instIdx).UdpNmRetryFirstMessageRequest == TRUE))
  {
    if ((UDPNM_CHANNEL_STATUS(instIdx).MessageFlags & UDPNM_IN_REPEAT_MESSAGE_STATE) != 0U)
    {
      /* !LINKSTO UdpNm.EB.UdpNmRetryFirstMessageRequest_2,1 */
      UDPNM_RMS_TIMER_START_NO_EMIT(instIdx, UDPNM_CHANNEL_CONFIG(instIdx).RMSTime + 1U);
    }
    /* !LINKSTO UdpNm.EB.UdpNmRetryFirstMessageRequest_1,1 */
    UDPNM_MSG_CYCLE_TIMER_START_NO_EMIT(instIdx, 1U);
  }
  else
  {
    if ((UDPNM_CHANNEL_STATUS(instIdx).MessageFlags & UDPNM_EXECUTE_IMMEDIATE_RESTART) != 0U)
    {
      UDPNM_MSG_CYCLE_TIMER_START(instIdx, UDPNM_CHANNEL_MSGCYCLEOFFSET(UDPNM_INST(instIdx)));
      UDPNM_CHANNEL_STATUS(instIdx).MessageFlags &= \
        (uint8)(~(uint8)(UDPNM_EXECUTE_IMMEDIATE_RESTART));
    }
    else
    {
      /* !LINKSTO UdpNm.EB.SoAd_IfTransmit_E_NOT_OK,1 */
      if (UDPNM_CHANNEL_STATUS(instIdx).ImmediateNmTransmissionCounter > 0)
      {
        if (retVal == E_OK)
        {
          UDPNM_CHANNEL_STATUS(instIdx).ImmediateNmTransmissionCounter--;

          if (UDPNM_CHANNEL_STATUS(instIdx).ImmediateNmTransmissionCounter > 0U)
          {
            UDPNM_MSG_CYCLE_TIMER_START(instIdx, UDPNM_CHANNEL_CONFIG(instIdx).ImmediateNmCycleTime);
          }
          else
          {
            UDPNM_MSG_CYCLE_TIMER_START(instIdx, UDPNM_CHANNEL_CONFIG(instIdx).MsgCycleTime);
          }
        }
        else
        {
          /* ImmediateNmTransmissionCounter is set to zero when repeat message state is leaved.
           * Retrying to send immediate message on the next mainfunction is limited to repeat message state only
           */
          /* !LINKSTO UdpNm.EB.SWS_UdpNm_00330,1 */
          if ((UDPNM_CHANNEL_STATUS(instIdx).MessageFlags & UDPNM_MESSAGE_SENT_ON_CYCLE) == 0)
          {
            UDPNM_MSG_CYCLE_TIMER_START_NO_EMIT(instIdx, 1U);
          }
          else
          {
            UDPNM_MSG_CYCLE_TIMER_START(instIdx, UDPNM_CHANNEL_CONFIG(instIdx).ImmediateNmCycleTime);
          }
        }
      }
      else
      {
        /* !LINKSTO SWS_UdpNm_00040,1 */
        UDPNM_MSG_CYCLE_TIMER_START(instIdx, UDPNM_CHANNEL_CONFIG(instIdx).MsgCycleTime);
      }
    }
  }
}
#endif

#define UDPNM_STOP_SEC_CODE
#include <UdpNm_MemMap.h>

/*==================[end of file]===========================================*/
