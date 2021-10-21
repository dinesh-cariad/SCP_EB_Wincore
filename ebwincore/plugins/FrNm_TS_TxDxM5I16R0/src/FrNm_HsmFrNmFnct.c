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

/* Implementation of the state handler functions of the state machine
 * FrNm.
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
 * The macro is used in function parameter declarations and definitions or as structure member
 * where the number of parentheses matter.
 */

/*==================[inclusions]============================================*/

#define FRNM_NO_CFGCLASSMIX_REQUIRED
#ifndef FRNM_NO_PBCFG_REQUIRED
#define FRNM_NO_PBCFG_REQUIRED
#endif

#include <FrNm_Hsm.h>  /* state machine driver interface */
#include <FrNm_HsmFrNm.h> /* public statechart model definitions */
#include <FrNm_HsmFrNmInt.h> /* internal statechart model definitions */
#include <FrNm_Int.h>         /* Module intenal definitions */
#include <SchM_FrNm.h>        /* SchM-header for FrNm */
#include <Nm_Cbk.h>           /* Nm call back API */
#include <TSMem.h>
#if (FRNM_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>
#endif
/*==================[macros]================================================*/

#if (defined FRNM_PL_TIMER)
#error FRNM_PL_TIMER already defined
#endif
#if (FRNM_HSM_INST_MULTI_ENABLED == STD_ON)
/* parameter list for internal functions */
#define FRNM_PL_TIMER(a, b)          a, b
#else
#define FRNM_PL_TIMER(a, b)          b
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define FRNM_START_SEC_CODE
#include <FrNm_MemMap.h>

/** \brief Initialize Internal variables of respective channel.
 ** \param instIdx index of state machine instance to work on */
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, FRNM_CODE) FrNm_InitInternalVar(
  FRNM_PDL_SF(uint8 instIdx));
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, FRNM_CODE) FrNm_ReadySleepTimerStart(
  FRNM_PL_TIMER(uint8 instIdx, FrNm_TimeType RsTimer));
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, FRNM_CODE) FrNm_ReadySleepTimerStop(
  FRNM_PDL_SF(uint8 instIdx));
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, FRNM_CODE) FrNm_UniTimerStart(
  FRNM_PL_TIMER(uint8 instIdx, FrNm_TimeType UniTimer));
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, FRNM_CODE) FrNm_UniTimerStop(
  FRNM_PDL_SF(uint8 instIdx));
/* Deviation MISRAC2012-1 */
STATIC FUNC(boolean, FRNM_CODE) FrNm_IsTransitionRmsAllowed(
  FRNM_PDL_SF(uint8 instIdx));
/* Deviation MISRAC2012-1 */
STATIC FUNC(boolean, FRNM_CODE) FrNm_IsTransitionNoOpAllowed(
  FRNM_PDL_SF(uint8 instIdx));
/* Deviation MISRAC2012-1 */
STATIC FUNC(boolean, FRNM_CODE) FrNm_IsTransitionAllowed(
  FRNM_PDL_SF(uint8 instIdx));

#if (FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON)
/** \brief Function calls Nm RemoteSleepCancellation
 ** \param instIdx zero based instance index */
STATIC FUNC(void, FRNM_CODE) FrNm_Call_Nm_RemotesleepCancellation(uint8 instIdx);
#endif
#if (FRNM_STATE_CHANGE_INDICATION_ENABLED == STD_ON)
/** \brief Function calls Nm RemoteSleepCancellation
 ** \param instIdx zero based instance index
 ** \param PreviousState Previous state machine state
 ** \param CurrentState current state machine state */
STATIC FUNC(void, FRNM_CODE) FrNm_Call_Nm_StateChangeNotification(
  uint8 instIdx,
  Nm_StateType PreviousState,
  Nm_StateType CurrentState);
#endif

STATIC FUNC(void, FRNM_CODE) FrNm_RepeatMessageStateDoAction(uint8 instIdx);

#define FRNM_STOP_SEC_CODE
#include <FrNm_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#define FRNM_START_SEC_CODE
#include <FrNm_MemMap.h>

/* ************************ state functions ******************************* */

/* ************************************************************************
 * State: TOP
 * Parent state: none
 * Init substate: BusSleepMode
 * Transitions originating from this state:
 * 1) RX_INDICATION[]/#if (FRNM_PDU_RX_INDIATION_ENABLED == STD_ON) Nm_PduRxIndication(); #endif =>FRNM189 FRNM190
 */

/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfTOPEntry(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* entry action 'UniTimerStop();
   * /InitInternalVar();/CurState = NM_STATE_UNINIT;' */
  FrNm_UniTimerStop(FRNM_PL_SF(instIdx));
  FRNM_CHANNEL_STATUS(instIdx).CurState = NM_STATE_UNINIT;
  FrNm_InitInternalVar(FRNM_PL_SF(instIdx));
}
/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfTOPAction1(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* action '#if (FRNM_PDU_RX_INDIATION_ENABLED == STD_ON) Nm_PduRxIndication();
   * #endif =>FRNM189 FRNM190'
   * for RX_INDICATION[]/...
   * internal transition */
#if (FRNM_PDU_RX_INDICATION_ENABLED == STD_ON)
#if (FRNM_MULTICORE_ENABLED == STD_OFF)
  FRNM_CALL_NM_PDURXINDICATION(FRNM_INST(instIdx));
#else
  if (FRNM_CALL_NM_PDURXINDICATION(FRNM_INST(instIdx)) != SCHM_E_OK)
  {
#if (FRNM_DEV_ERROR_DETECT == STD_ON)
    /* !LINKSTO FrNm.EB.SchM_Call_DET,1 */
    FRNM_DET_REPORT_ERROR(FRNM_INST(instIdx),
                          (FRNM_SERVID_MAINFUNCTION_X + FRNM_INST(instIdx)),
                          FRNM_E_PDURXINDICATION
                         );
#endif
  }
#endif
#elif (FRNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}

/* ************************************************************************
 * State: BusSleepMode
 * Parent state: TOP
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 * 1) VOTE_RECEIVED[]/Nm_NetworkStartIndication(); => FRNM175
 * 2) BusSleepMode -> SynchronizeMode: NETWORK_START[]/
 *    if(FRNM_NETWORK_REQUEST_BIT) SetActiveWakeUpFlag == true
 */

/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfBusSleepModeEntry(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* entry action 'ChanStatus&=~REPEAT_MSG_REQUESTED; => FRNM320 */
  const Nm_StateType PreviousState = FRNM_CHANNEL_STATUS(instIdx).CurState;
  /* Critical section needs to be extended also for current state
   * since API FrNm_RepeatMessageRequest preemts beween clearing the
   * flag FRNM_REPEAT_MSG_REQUESTED and assigning the new state, flag FRNM_REPEAT_MSG_REQUESTED
   * will be set again
   */
  /* Enter critical section to protect from concurrent access
   * to different bits in 'ChanStatus' in different APIs.
   */
  SchM_Enter_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();
#if (FRNM_ACTIVE_WAKEUP_BIT_ENABLED == STD_ON)
  FRNM_CHANNEL_STATUS(instIdx).ActiveWakeUp1 = 0U;
  FRNM_CHANNEL_STATUS(instIdx).ActiveWakeUp2 = 0U;
  FRNM_CHANNEL_STATUS(instIdx).FrNm_SetActiveWakeUpFlag = FALSE;
#endif

  /* !LINKSTO FRNM320,1 */
  FRNM_CHANNEL_STATUS(instIdx).ChanStatus &= (uint8)(~FRNM_REPEAT_MSG_REQUESTED);
  FRNM_CHANNEL_STATUS(instIdx).CurState = NM_STATE_BUS_SLEEP;
  SchM_Exit_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();

  if(PreviousState != NM_STATE_UNINIT)
  {
#if (FRNM_STATE_CHANGE_INDICATION_ENABLED == STD_ON)
    FrNm_Call_Nm_StateChangeNotification(FRNM_INST(instIdx),
    PreviousState,
    NM_STATE_BUS_SLEEP);
#endif
#if (FRNM_MULTICORE_ENABLED == STD_OFF)
    FRNM_CALL_NM_BUSSLEEPMODE(FRNM_INST(instIdx));
#else
    if (FRNM_CALL_NM_BUSSLEEPMODE(FRNM_INST(instIdx)) != SCHM_E_OK)
    {
#if (FRNM_DEV_ERROR_DETECT == STD_ON)
      /* !LINKSTO FrNm.EB.SchM_Call_DET,1 */
      FRNM_DET_REPORT_ERROR(FRNM_INST(instIdx),
                            (FRNM_SERVID_MAINFUNCTION_X + FRNM_INST(instIdx)),
                             FRNM_E_BUSSLEEPMODE
                             );
#endif
    }
#endif
  }
}
/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfBusSleepModeAction1(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* action 'Nm_NetworkStartIndication(); => FRNM175'
   * for VOTE_RECEIVED[]/...
   * internal transition */
#if (FRNM_MULTICORE_ENABLED == STD_OFF)
  FRNM_CALL_NM_NETWORKSTARTINDICATION(FRNM_INST(instIdx));
#else
  if (FRNM_CALL_NM_NETWORKSTARTINDICATION(FRNM_INST(instIdx)) != SCHM_E_OK)
  {
#if (FRNM_DEV_ERROR_DETECT == STD_ON)
    /* !LINKSTO FrNm.EB.SchM_Call_DET,1 */
    FRNM_DET_REPORT_ERROR(FRNM_INST(instIdx),
                          (FRNM_SERVID_MAINFUNCTION_X + FRNM_INST(instIdx)),
                          FRNM_E_NETWORKSTARTINDICATION
                         );
#endif
  }
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfBusSleepModeAction2(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* action 'if(ChanStatus & NETWORK_REQUESTED) SetActiveWakeUpFlag = true'
   * for NETWORK_START[]/...
   * external transition to state SynchronizeMode */
#if (FRNM_ACTIVE_WAKEUP_BIT_ENABLED == STD_ON)
  SchM_Enter_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();
  if(FRNM_CHANNEL_STATUS(instIdx).ActiveWakeUp1 == FRNM_NETWORK_REQUEST_AWB)
  {
    FRNM_CHANNEL_STATUS(instIdx).FrNm_SetActiveWakeUpFlag = TRUE;
  }
  SchM_Exit_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();
#elif (FRNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}

/* ************************************************************************
 * State: NetworkMode
 * Parent state: TOP
 * Init substate: SendingSubMode
 * Transitions originating from this state:
 * 1) STARTUP_ERROR[]/FrNm_HandleSynchError();
 * 2) NetworkMode -> BusSleepMode: FAILSAFE_SLEEP_MODE[]/InitInternalVar();
 * 3) NetworkMode -> SynchronizeMode:
 *     FAILSAFE_SYNCH_MODE[]/ClearActiveWakeupBit()
 */

/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfNetworkModeEntry(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* entry action 'ChanStatus |= FRNM_REPEAT_MSG_REQUESTED; => FRNM109 ,
   * ChanStatus |=FRNM_COM_ENABLED;Nm_NetworkMode(); => FRNM110
   * if(SetActiveWakeFlag == true) SetActiveWakeupBit();
   *  SetActiveWakeFlag = false;'
   */
  SchM_Enter_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();
  /* Enter critical section to protect from concurrent access
   * to different bits in 'ChanStatus' in different APIs.
   */
  FRNM_CHANNEL_STATUS(instIdx).ChanStatus |= (uint8)FRNM_REPEAT_MSG_REQUESTED;
  /* Enable data or vote transmission */
#if (FRNM_ACTIVE_WAKEUP_BIT_ENABLED == STD_ON)
  /* !LINKSTO FrNm.EB.FrNm_NetworkGwEraRequest.ActiveWakeupBit.BusSleepMode,1 */
  /* !LINKSTO FrNm.EB.PassiveStartup.NetworkRequest.Sequence.ActiveWakeupBit.BusSleepMode,1 */


  if(((FRNM_CHANNEL_PROPERTY(instIdx) & FRNM_CONTROL_BIT_VECTOR) != 0U) &&
     (FRNM_CHANNELID_CONFIG(instIdx).FrNmActiveWakeupBitEnabled == TRUE) &&
     (FRNM_CHANNEL_STATUS(instIdx).FrNm_SetActiveWakeUpFlag == TRUE)
    )
  {
    FRNM_CHANNEL_STATUS(instIdx).ControlBitVector |= (uint8)FRNM_CBV_ACTIVEWAKEUPBIT_MASK;
  }
#endif
  SchM_Exit_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();

  FRNM_CHANNEL_STATUS(instIdx).ComEnabled = TRUE;
#if (FRNM_MULTICORE_ENABLED == STD_OFF)
  FRNM_CALL_NM_NETWORKMODE(FRNM_INST(instIdx));
#else
  if (FRNM_CALL_NM_NETWORKMODE(FRNM_INST(instIdx)) != SCHM_E_OK)
  {
#if (FRNM_DEV_ERROR_DETECT == STD_ON)
    /* !LINKSTO FrNm.EB.SchM_Call_DET,1 */
    FRNM_DET_REPORT_ERROR(FRNM_INST(instIdx),
                          (FRNM_SERVID_MAINFUNCTION_X + FRNM_INST(instIdx)),
                          FRNM_E_NETWORKMODE
                         );
#endif
  }
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfNetworkModeAction1(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* action 'FrNm_HandleSynchError();'
   * for STARTUP_ERROR[]/...
   * internal transition */
  FrNm_HandleSynchError(FRNM_INST(instIdx), TRUE);
}
/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfNetworkModeAction2(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* action 'InitInternalVar();'
   * for FAILSAFE_SLEEP_MODE[]/...
   * external transition to state BusSleepMode */
  FrNm_InitInternalVar(FRNM_PL_SF(instIdx));
}
/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfNetworkModeAction3(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* action 'ClearActiveWakeupBit()'
   * for FAILSAFE_SYNCH_MODE[]/...
   * external transition to state SynchronizeMode */
#if (FRNM_PASSIVE_MODE_ENABLED == STD_OFF)
  if ((FRNM_CHANNEL_PROPERTY(instIdx) & FRNM_CONTROL_BIT_VECTOR) != 0U)
  {
    TS_AtomicClearBit_8(&FRNM_CHANNEL_STATUS(instIdx).ControlBitVector, FRNM_CBV_ACTIVEWAKEUPBIT);
  }
#elif (FRNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}

/* ************************************************************************
 * State: ReadySleepState
 * Parent state: NetworkMode
 * Init substate: ReadySleepRemoteActivity
 * Transitions originating from this state:
 * 1) ReadySleepState -> BusSleepMode:
 *    REP_CYCLE_COMPLETED[ReadySleepTimer == 0]/ClearActiveWakeupBit()
 */

/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfReadySleepStateEntry(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* entry action '#ifdef (SCI == STD_ON)
   * Nm_StateChangeNotification(LastState, ActState) ;
   * #endif CurState = ReadySleepState;'
   */

#if (FRNM_STATE_CHANGE_INDICATION_ENABLED == STD_ON)
  const Nm_StateType PreviousState = FRNM_CHANNEL_STATUS(instIdx).CurState;
  FRNM_CHANNEL_STATUS(instIdx).CurState = NM_STATE_READY_SLEEP;
  FrNm_Call_Nm_StateChangeNotification(FRNM_INST(instIdx),
    PreviousState,
    NM_STATE_READY_SLEEP);
#else
  FRNM_CHANNEL_STATUS(instIdx).CurState = NM_STATE_READY_SLEEP;
#endif

}
/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfReadySleepStateExit(
   FRNM_PDL_SF(const uint8 instIdx))
{
  /* exit action 'ReadySleepTimerStop();' */
  FrNm_ReadySleepTimerStop(FRNM_PL_SF(instIdx));
}
/* Deviation MISRAC2012-1 */
FUNC(boolean, FRNM_CODE) FrNm_HsmFrNmSfReadySleepStateGuard1(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* guard condition 'ReadySleepTimer == 0'
   * for REP_CYCLE_COMPLETED[...]/ClearActiveWakeupBit()
   * external transition to state BusSleepMode */
  return (boolean)((FRNM_CHANNEL_STATUS(instIdx).ReadySleepTimer == 0U) ? TRUE : FALSE);
}
/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfReadySleepStateAction1(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* action 'ClearActiveWakeupBit()'
   * for REP_CYCLE_COMPLETED[ReadySleepTimer == 0]/...
   * external transition to state BusSleepMode */
#if (FRNM_PASSIVE_MODE_ENABLED == STD_OFF)
  if ((FRNM_CHANNEL_PROPERTY(instIdx) & FRNM_CONTROL_BIT_VECTOR) != 0U)
  {
    TS_AtomicClearBit_8(&FRNM_CHANNEL_STATUS(instIdx).ControlBitVector, FRNM_CBV_ACTIVEWAKEUPBIT);
  }
#elif (FRNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}

/* ************************************************************************
 * State: ReadySleepRemoteActivity
 * Parent state: ReadySleepState
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 * 1) VOTE_RECEIVED[]/ReadySleepTimerStart(READY_SLEEP_COUNT);
 * 2) ReadySleepRemoteActivity -> RepeatMessageState:
 *    REP_CYCLE_COMPLETED[IsTransitionRmsAllowed() == true]/
 * 3) ReadySleepRemoteActivity -> NormalOperationRemoteActivity:
 *    REP_CYCLE_COMPLETED[IsTransitionNoOpAllowed() == true]/
 */

/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfReadySleepRemoteActivityEntry(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* entry action 'ReadySleepTimerStart(READY_SLEEP_COUNT) +1;' */
  /* !LINKSTO SWS_FrNm_00101,1 */
  /* Deviation MISRAC2012-1 */
  FrNm_ReadySleepTimerStart(
    FRNM_PL_TIMER(instIdx, FRNM_CHANNELTIME_CONFIG(instIdx).ReadySleepCnt + 1));
}
/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfReadySleepRemoteActivityAction1(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* action 'ReadySleepTimerStart(READY_SLEEP_COUNT);'
   * for VOTE_RECEIVED[]/...
   * internal transition */
  /* !LINKSTO SWS_FrNm_00101,1 */
  /* Deviation MISRAC2012-1 */
  FrNm_ReadySleepTimerStart(
    FRNM_PL_TIMER(instIdx, FRNM_CHANNELTIME_CONFIG(instIdx).ReadySleepCnt + 1));
}
/* Deviation MISRAC2012-1 */
FUNC(boolean, FRNM_CODE) FrNm_HsmFrNmSfReadySleepRemoteActivityGuard2(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* guard condition 'IsTransitionRmsAllowed() == true'
   * for REP_CYCLE_COMPLETED[...]/
   * external transition to state RepeatMessageState */
  return (boolean)((FrNm_IsTransitionRmsAllowed(FRNM_PL_SF(instIdx)) == TRUE) ? TRUE : FALSE);
}
/* Deviation MISRAC2012-1 */
FUNC(boolean, FRNM_CODE) FrNm_HsmFrNmSfReadySleepRemoteActivityGuard3(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* guard condition 'IsTransitionNoOpAllowed() == true'
   * for REP_CYCLE_COMPLETED[...]/
   * external transition to state NormalOperationRemoteActivity */
  return (boolean)((FrNm_IsTransitionNoOpAllowed(FRNM_PL_SF(instIdx)) == TRUE) ? TRUE : FALSE);
}

/* ************************************************************************
 * State: ReadySleepRemoteSleep
 * Parent state: ReadySleepState
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 * 1) ReadySleepRemoteSleep -> RepeatMessageState:
 *    REP_CYCLE_COMPLETED[IsTransitionRmsAllowed() == true]/
 *    Nm_RemoteSleepCancellation();
 * 2) ReadySleepRemoteSleep -> NormalOperationRemoteSleep:
 *    REP_CYCLE_COMPLETED[IsTransitionNoOpAllowed() == true]/
 * 3) ReadySleepRemoteSleep -> ReadySleepRemoteActivity:
 *    VOTE_RECEIVED[]/Nm_RemoteSleepCancellation();
 *    ChanStatus &= ~FRNM_REMOTE_SLEEP_INDICATION;
 */

/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfReadySleepRemoteSleepEntry(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* entry action 'ReadySleepTimerStart(READY_SLEEP_COUNT) +1;' */
  /* !LINKSTO SWS_FrNm_00101,1 */
  /* Deviation MISRAC2012-1 */
  FrNm_ReadySleepTimerStart(
    FRNM_PL_TIMER(instIdx, FRNM_CHANNELTIME_CONFIG(instIdx).ReadySleepCnt + 1));
}
/* Deviation MISRAC2012-1 */
FUNC(boolean, FRNM_CODE) FrNm_HsmFrNmSfReadySleepRemoteSleepGuard1(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* guard condition 'IsTransitionRmsAllowed() == true'
   * for REP_CYCLE_COMPLETED[...]/Nm_RemoteSleepCancellation();
   * external transition to state RepeatMessageState */
  return (boolean) ((FrNm_IsTransitionRmsAllowed(FRNM_PL_SF(instIdx)) == TRUE) ? TRUE : FALSE);
}
/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfReadySleepRemoteSleepAction1(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* action 'Nm_RemoteSleepCancellation();'
   * for REP_CYCLE_COMPLETED[IsTransitionRmsAllowed() == true]/...
   * external transition to state RepeatMessageState */
#if (FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON)
  FrNm_Call_Nm_RemotesleepCancellation(FRNM_INST(instIdx));
#elif (FRNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(boolean, FRNM_CODE) FrNm_HsmFrNmSfReadySleepRemoteSleepGuard2(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* guard condition 'IsTransitionNoOpAllowed() == true'
   * for REP_CYCLE_COMPLETED[...]/
   * external transition to state NormalOperationRemoteSleep */
  return (boolean)((FrNm_IsTransitionNoOpAllowed(FRNM_PL_SF(instIdx)) == TRUE) ? TRUE : FALSE);
}
/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfReadySleepRemoteSleepAction3(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* action 'Nm_RemoteSleepCancellation();
   * ChanStatus &= ~FRNM_REMOTE_SLEEP_INDICATION;'
   * for VOTE_RECEIVED[]/...
   * external transition to state ReadySleepRemoteActivity */
#if (FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON)
  FrNm_Call_Nm_RemotesleepCancellation(FRNM_INST(instIdx));
  /* Enter critical section to protect from concurrent access
   * to different bits in 'ChanStatus' in different APIs.
   */
  TS_AtomicClearBit_8(&FRNM_CHANNEL_STATUS(instIdx).ChanStatus, FRNM_REMOTE_SLEEP_IND_BIT);
#elif (FRNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}

/* ************************************************************************
 * State: SendingSubMode
 * Parent state: NetworkMode
 * Init substate: RepeatMessageState
 * Transitions originating from this state:
 * 1) DATA_CYCLE_TIMESLOT[]/#if ((PASSIVE_MODE == STD_OFF))
 *    if ((ChanStatus == COM_ENABLED) && (ChanProperty == NM_DATA_ENABLED))
 *    {HandleDataCycle();}#endif
 * 2) VOTE_CYCLE_TIMESLOT[]/#if ((PASSIVE_MODE == STD_OFF))
 *    if ((ChanStatus == FRNM_COM_ENABLED) {HandleVoteCycle();}#endif
 * 3) TX_CONFIRMATION[]/Restart TimeoutTimer;
 * 4) TX_TIMEOUT[]/Nm_TxTimeoutException();
 */

/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfSendingSubModeEntry(
  FRNM_PDL_SF(const uint8 instIdx))
{
 /* entry action 'SetVoteBit(); Start TimeoutTimer;' */
#if (FRNM_PASSIVE_MODE_ENABLED == STD_OFF)
#if (FRNM_FORCE_VOTE_BIT == STD_ON)
  if(((FRNM_CHANNEL_PROPERTY(instIdx) & FRNM_PDU_SCHEDULE_VARIANT_MASK) == FRNM_PDU_SCHEDULE_VARIANT_2) ||
     ((FRNM_CHANNEL_PROPERTY(instIdx) & FRNM_PDU_SCHEDULE_VARIANT_MASK) == FRNM_PDU_SCHEDULE_VARIANT_6))
  {
    /* !LINKSTO FrNm.EB.VoteBitValueScheduleVariant2,2 */
    /* !LINKSTO FrNm.EB.VoteBitValueScheduleVariant6,1 */
#if (FRNM_VOTE_BIT != 0x00U)
    {
      TS_AtomicSetBit_8(&FRNM_CHANNEL_STATUS(instIdx).ControlBitVector, FRNM_VOTE_BIT);
    }
#endif
  }
  else
  {
    TS_AtomicSetBit_8(&FRNM_CHANNEL_STATUS(instIdx).ControlBitVector, FRNM_PDU_VOTE_BIT);
  }
#else
  TS_AtomicSetBit_8(&FRNM_CHANNEL_STATUS(instIdx).ControlBitVector, FRNM_PDU_VOTE_BIT);
#endif

  /* Start timeout timer to detect any transmission error */
  FRNM_CHANNEL_STATUS(instIdx).TimeoutTimer = FRNM_CHANNEL_MSGTIMEOUTTIME(instIdx);
#elif (FRNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfSendingSubModeExit(
   FRNM_PDL_SF(const uint8 instIdx))
{
  /* exit action 'clearVoteBit(); Stop TimeoutTimer;' */
#if (FRNM_PASSIVE_MODE_ENABLED == STD_OFF)
#if (FRNM_FORCE_VOTE_BIT == STD_OFF)
  TS_AtomicClearBit_8(&FRNM_CHANNEL_STATUS(instIdx).ControlBitVector, FRNM_PDU_VOTE_BIT);
#else
  /* !LINKSTO FrNm.EB.VoteBitValueScheduleVariant2,2 */
  /* !LINKSTO FrNm.EB.VoteBitValueScheduleVariant6,1 */
  /* In case the Schedule Variant is 2 or 6 the vote bit does not need to be changed since
   * it's set in the SendingSubmodeEntry */
  if(((FRNM_CHANNEL_PROPERTY(instIdx) & FRNM_PDU_SCHEDULE_VARIANT_MASK) != FRNM_PDU_SCHEDULE_VARIANT_2) &&
     ((FRNM_CHANNEL_PROPERTY(instIdx) & FRNM_PDU_SCHEDULE_VARIANT_MASK) != FRNM_PDU_SCHEDULE_VARIANT_6))
  {
    TS_AtomicClearBit_8(&FRNM_CHANNEL_STATUS(instIdx).ControlBitVector, FRNM_PDU_VOTE_BIT);
  }
#endif

  /* Stop timeout timer */
  FRNM_CHANNEL_STATUS(instIdx).TimeoutTimer = 0U;
#elif (FRNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif

}
/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfSendingSubModeAction1(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* action '#if ((PASSIVE_MODE == STD_OFF))
   * if ((ChanStatus == COM_ENABLED) && (ChanProperty == NM_DATA_ENABLED))
   * {HandleDataCycle();}
   * #endif' for DATA_CYCLE_TIMESLOT[]/...
   * internal transition */
#if (FRNM_PASSIVE_MODE_ENABLED == STD_OFF)
    if ((FRNM_CHANNEL_STATUS(instIdx).ComEnabled == TRUE) &&
        ((FRNM_CHANNEL_PROPERTY(instIdx) & FRNM_NM_DATA_ENABLED) != 0U))
   {
    FrNm_HandleDataCycle(FRNM_PL_SF(instIdx));
   }
#elif (FRNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfSendingSubModeAction2(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* action '#if ((PASSIVE_MODE == STD_OFF))
   * if (ChanStatus == FRNM_COM_ENABLED) {HandleVoteCycle();}#endif'
   * for VOTE_CYCLE_TIMESLOT[]/...
   * internal transition */
#if (FRNM_PASSIVE_MODE_ENABLED == STD_OFF)

  if (FRNM_CHANNEL_STATUS(instIdx).ComEnabled == TRUE)
  {
    FrNm_HandleVoteCycle(FRNM_PL_SF(instIdx));
  }
#elif (FRNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfSendingSubModeAction3(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* action 'Restart TimeoutTimer;'
   * for TX_CONFIRMATION[]/...
   * internal transition */
  /* Restart timeout timer */
  FRNM_CHANNEL_STATUS(instIdx).TimeoutTimer = FRNM_CHANNEL_MSGTIMEOUTTIME(instIdx);
}
/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfSendingSubModeAction4(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* action 'Nm_TxTimeoutException();'
   * for TX_TIMEOUT[]/...
   * internal transition */
#if (FRNM_MULTICORE_ENABLED == STD_OFF)
  FRNM_CALL_NM_TXTIMEOUTEXCEPTION(FRNM_INST(instIdx));
#else
  if (FRNM_CALL_NM_TXTIMEOUTEXCEPTION(FRNM_INST(instIdx)) != SCHM_E_OK)
  {
#if (FRNM_DEV_ERROR_DETECT == STD_ON)
    /* !LINKSTO FrNm.EB.SchM_Call_DET,1 */
    FRNM_DET_REPORT_ERROR(FRNM_INST(instIdx),
                          (FRNM_SERVID_MAINFUNCTION_X + FRNM_INST(instIdx)),
                          FRNM_E_TXTIMEOUTEXCEPTION
                         );
#endif
  }
#endif
  /* Restart timeout timer */
  FRNM_CHANNEL_STATUS(instIdx).TimeoutTimer = FRNM_CHANNEL_MSGTIMEOUTTIME(instIdx);
}
/* ************************************************************************
 * State: NormalOperationState
 * Parent state: SendingSubMode
 * Init substate: NormalOperationRemoteActivity
 * Transitions originating from this state:
 */

/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfNormalOperationStateEntry(
  FRNM_PDL_SF(const uint8 instIdx))
{

  /* entry action '#ifdef (SCI == STD_ON)
   * Nm_StateChangeNotification(LastState, ActState); #endif
   * CurState = NormalOperationState;'
   */

#if (FRNM_STATE_CHANGE_INDICATION_ENABLED == STD_ON)
  const Nm_StateType PreviousState = FRNM_CHANNEL_STATUS(instIdx).CurState;
  FRNM_CHANNEL_STATUS(instIdx).CurState = NM_STATE_NORMAL_OPERATION;
  FrNm_Call_Nm_StateChangeNotification(FRNM_INST(instIdx),
    PreviousState,
    NM_STATE_NORMAL_OPERATION);
#else
  FRNM_CHANNEL_STATUS(instIdx).CurState = NM_STATE_NORMAL_OPERATION;
#endif

}

/* ************************************************************************
 * State: NormalOperationRemoteActivity
 * Parent state: NormalOperationState
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 * 1) VOTE_RECEIVED[]/#if (RSI == STD_ON)
 *    UniTimerStart(FRNM_REMOTE_SLEEP_IND_TIME); #endif
 * 2) NormalOperationRemoteActivity -> RepeatMessageState:
 *    REP_CYCLE_COMPLETED[ChanStatus & REPEAT_MESSAGE_REQUESTED]/
 * 3) NormalOperationRemoteActivity -> NormalOperationRemoteSleep:
 *    REP_CYCLE_COMPLETED[(ChanStatus.UniversalTimer == 0) &&
 *    ChanConfig.RemoteSleepIndTime != 0)]/
 * 4) NormalOperationRemoteActivity -> ReadySleepRemoteActivity:
 *    REP_CYCLE_COMPLETED[(! ChanStatus & NETWORK_REQUESTED) &&
 *    (! ChanStatus & REPEAT_MSG_REQUESTED)]/
 */

/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfNormalOperationRemoteActivityEntry(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* entry action '#if (RSI == STD_ON) if (ChanStatus & R_S_I)
   * {Nm_RemoteSleepCancellation();
   * ChanStatus &= ~R_S_I;} #endif#if (RSI == STD_ON)
   * UniTimerStart(FRNM_REMOTE_SLEEP_IND_TIME); #endif'
   */
#if (FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON)
  /* clear the internal status flag, before starting the universal timer */
  /* Enter critical section to protect from concurrent access
   * to different bits in 'ChanStatus' in different APIs.
   */

  TS_AtomicClearBit_8(&FRNM_CHANNEL_STATUS(instIdx).ChanStatus, FRNM_UNI_TIMEOUT_BIT);

  /* Deviation MISRAC2012-1 */
  FrNm_UniTimerStart(
    FRNM_PL_TIMER(instIdx, FRNM_CHANNELTIME_CONFIG(instIdx).RemoteSleepIndTime));

  if ((FRNM_CHANNEL_STATUS(instIdx).ChanStatus & FRNM_REMOTE_SLEEP_INDICATION) != 0U)
  {
    FrNm_Call_Nm_RemotesleepCancellation(FRNM_INST(instIdx));
    TS_AtomicClearBit_8(&FRNM_CHANNEL_STATUS(instIdx).ChanStatus, FRNM_REMOTE_SLEEP_IND_BIT);
  }
#elif (FRNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfNormalOperationRemoteActivityAction1(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* action '#if (RSI == STD_ON) UniTimerStart(FRNM_REMOTE_SLEEP_IND_TIME);
   * #endif'
   * for VOTE_RECEIVED[]/...
   * internal transition */
#if (FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON)
  /* clear the internal status flag, before starting the universal timer */
  /* Enter critical section to protect from concurrent access
   * to different bits in 'ChanStatus' in different APIs.
   */
  TS_AtomicClearBit_8(&FRNM_CHANNEL_STATUS(instIdx).ChanStatus, FRNM_UNI_TIMEOUT_BIT);

  /* Deviation MISRAC2012-1 */
  FrNm_UniTimerStart(
    FRNM_PL_TIMER(instIdx, FRNM_CHANNELTIME_CONFIG(instIdx).RemoteSleepIndTime));
#elif (FRNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(boolean, FRNM_CODE) FrNm_HsmFrNmSfNormalOperationRemoteActivityGuard2(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* guard condition 'ChanStatus & REPEAT_MESSAGE_REQUESTED'
   * for REP_CYCLE_COMPLETED[...]/
   * external transition to state RepeatMessageState */
  return (boolean)
    (((FRNM_CHANNEL_STATUS(instIdx).ChanStatus & FRNM_REPEAT_MSG_REQUESTED)!= 0U) ? TRUE : FALSE);
}
/* Deviation MISRAC2012-1 */
FUNC(boolean, FRNM_CODE) FrNm_HsmFrNmSfNormalOperationRemoteActivityGuard3(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* guard condition '(ChanStatus.UniversalTimer == 0) &&
   *  ChanConfig.RemoteSleepIndTime != 0)'
   * for REP_CYCLE_COMPLETED[...]/
   * external transition to state NormalOperationRemoteSleep */
  return (boolean)(((FRNM_CHANNEL_STATUS(instIdx).UniversalTimer == 0U) &&
   (FRNM_CHANNELTIME_CONFIG(instIdx).RemoteSleepIndTime != 0U)) ? TRUE : FALSE);
}
/* Deviation MISRAC2012-1 */
FUNC(boolean, FRNM_CODE) FrNm_HsmFrNmSfNormalOperationRemoteActivityGuard4(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* guard condition '(! ChanStatus & NETWORK_REQUESTED) &&
   * (! ChanStatus & REPEAT_MSG_REQUESTED)'
   * for REP_CYCLE_COMPLETED[...]/
   * external transition to state ReadySleepRemoteActivity */
  return (boolean)
    ((((FRNM_CHANNEL_STATUS(instIdx).ChanStatus & FRNM_NETWORK_REQUESTED) == 0U) &&
     ((FRNM_CHANNEL_STATUS(instIdx).ChanStatus & FRNM_REPEAT_MSG_REQUESTED) == 0U)) ? TRUE : FALSE);
}

/* ************************************************************************
 * State: NormalOperationRemoteSleep
 * Parent state: NormalOperationState
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 * 1) NormalOperationRemoteSleep -> RepeatMessageState:
 *    REP_CYCLE_COMPLETED[ChanStatus & REPEAT_MSG_REQUESTED]/
 *    Nm_RemoteSleepCancellation(); => FRNM230
 * 2) NormalOperationRemoteSleep -> ReadySleepRemoteSleep:
 *    REP_CYCLE_COMPLETED[(! ChanStatus & NETWORK_REQUESTED) &&
 *    (! ChanStatus & REPEAT_MSG_REQUESTED)]/
 * 3) NormalOperationRemoteSleep -> NormalOperationRemoteActivity:
 *    VOTE_RECEIVED[]/Uni timer (RSI) is started by entry action
 */

/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfNormalOperationRemoteSleepEntry(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* entry action 'ChanStatus |= FRNM_REMOTE_SLEEP_INDICATION;
   * Nm_RemoteSleepIndication();' */
#if (FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON)
  TS_AtomicSetBit_8(&FRNM_CHANNEL_STATUS(instIdx).ChanStatus, FRNM_REMOTE_SLEEP_IND_BIT);
#if (FRNM_MULTICORE_ENABLED == STD_OFF)
  FRNM_CALL_NM_REMOTESLEEPINDICATION(FRNM_INST(instIdx));
#else
  if (FRNM_CALL_NM_REMOTESLEEPINDICATION(FRNM_INST(instIdx)) != SCHM_E_OK)
  {
#if (FRNM_DEV_ERROR_DETECT == STD_ON)
    FRNM_DET_REPORT_ERROR(FRNM_INST(instIdx),
                          (FRNM_SERVID_MAINFUNCTION_X + FRNM_INST(instIdx)),
                          FRNM_E_REMOTESLEEPINDICATION
                         );
#endif
  }
#endif
#elif (FRNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(boolean, FRNM_CODE) FrNm_HsmFrNmSfNormalOperationRemoteSleepGuard1(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* guard condition 'ChanStatus & REPEAT_MSG_REQUESTED'
   * for REP_CYCLE_COMPLETED[...]/Nm_RemoteSleepCancellation(); => FRNM230
   * external transition to state RepeatMessageState */
  return (boolean)
    (((FRNM_CHANNEL_STATUS(instIdx).ChanStatus & FRNM_REPEAT_MSG_REQUESTED) != 0U) ? TRUE : FALSE);
}
/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfNormalOperationRemoteSleepAction1(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* action 'Nm_RemoteSleepCancellation(); => FRNM230 '
   * for REP_CYCLE_COMPLETED[ChanStatus & REPEAT_MSG_REQUESTED]/...
   * external transition to state RepeatMessageState */
#if (FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON)
  FrNm_Call_Nm_RemotesleepCancellation(FRNM_INST(instIdx));
#elif (FRNM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif
}
/* Deviation MISRAC2012-1 */
FUNC(boolean, FRNM_CODE) FrNm_HsmFrNmSfNormalOperationRemoteSleepGuard2(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* guard condition '(! ChanStatus & NETWORK_REQUESTED) &&
   * (! ChanStatus & REPEAT_MSG_REQUESTED)'
   * for REP_CYCLE_COMPLETED[...]/
   * external transition to state ReadySleepRemoteSleep */
  return (boolean)
    ((((FRNM_CHANNEL_STATUS(instIdx).ChanStatus & FRNM_NETWORK_REQUESTED) == 0U) &&
     ((FRNM_CHANNEL_STATUS(instIdx).ChanStatus & FRNM_REPEAT_MSG_REQUESTED) == 0U)) ? TRUE : FALSE);
}
/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfNormalOperationRemoteSleepAction3(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* action 'Uni timer (RSI) is started by entry action'
   * for VOTE_RECEIVED[]/...
   * external transition to state NormalOperationRemoteActivity */
  /* clear the internal status flag, before starting the universal timer */
  TS_AtomicClearBit_8(&FRNM_CHANNEL_STATUS(instIdx).ChanStatus, FRNM_UNI_TIMEOUT_BIT);
}

/* ************************************************************************
 * State: RepeatMessageState
 * Parent state: SendingSubMode
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 * 1) UNI_TIMEOUT[]/ChanStatus |= UNI_TIMEOUT_PASSED;
 * 2) RepeatMessageState -> NormalOperationState:
 *    REP_CYCLE_COMPLETED[(ChanStatus & NETWORK_REQUESTED) &&
 *    (ChanStatus & UNI_TIMEOUT_PASSED)]/
 * 3) RepeatMessageState -> ReadySleepState:
 *    REP_CYCLE_COMPLETED[( !(ChanStatus & NETWORK_REQUESTED) &&
 *    (ChanStatus & UNI_TIMEOUT_PASSED) )]/
 */

/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfRepeatMessageStateEntry(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* entry action '#if (RSI == STD_ON) ChanStatus &= REMOTE_SLEEP_INDICATION;
   * #endif
   * #if (SCI == STD_ON) Nm_StateChangeNotification(LastState, ActState); #endif
   * CurState = RepeatMessageState;HandleRepeatMessageStateEntry();
   * UniTimerStart(FRNM_REPEAT_MSG_TIME); => FRNM117 '
   */
#if (FRNM_STATE_CHANGE_INDICATION_ENABLED == STD_ON)
   const Nm_StateType PreviousState = FRNM_CHANNEL_STATUS(instIdx).CurState;
#endif

  FrNm_DoActFkp[FRNM_INST(instIdx)] = FrNm_RepeatMessageStateDoAction;
  /* Refer FrNm0030_Conf, timeout for Repeat Message State with value  "0"
   * denotes that no Repeat Message State is configured, which means that
   * Repeat Message State is transient and implies that it is left immediately
   * after entry and consequently no startup stability is guaranteed and no
   * node detection procedure is possible.
   */

  if (FRNM_CHANNELTIME_CONFIG(instIdx).RepeatMessageTime != 0U)
  {
    /* Enter critical section to protect from concurrent access
     * to different bits in 'ChanStatus' in different APIs.
     */
    /* Critical section needs to be extended also for current state
     * since if somebody decides to change the order of operations
     * clearing the flag and setting the current state
     * API FrNm_CheckRemoteSleepIndication could return inccorect value.
     */
    SchM_Enter_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();
#if (FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON)
    FRNM_CHANNEL_STATUS(instIdx).ChanStatus &= (uint8)(~FRNM_REMOTE_SLEEP_INDICATION);
#endif
    FRNM_CHANNEL_STATUS(instIdx).CurState = NM_STATE_REPEAT_MESSAGE;
    SchM_Exit_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();
#if (FRNM_STATE_CHANGE_INDICATION_ENABLED == STD_ON)
    /* !LINKSTO FrNm.EB.StateChangeNotification,1 */
    FrNm_Call_Nm_StateChangeNotification(FRNM_INST(instIdx),
      PreviousState, NM_STATE_REPEAT_MESSAGE);
#endif
    /* clear the internal status flag, before starting the universal timer */
    TS_AtomicClearBit_8(&FRNM_CHANNEL_STATUS(instIdx).ChanStatus, FRNM_UNI_TIMEOUT_BIT);
    /* -->FRNM117; */
    /* Deviation MISRAC2012-1 */
    FrNm_UniTimerStart(
      FRNM_PL_TIMER(instIdx, FRNM_CHANNELTIME_CONFIG(instIdx).RepeatMessageTime));
  }
  else
  {
    /* Repeat message time is 0.
     * This condition is handled in the do action 'HandleRepeatMessageTimer()'.
     */
  }
}
/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfRepeatMessageStateExit(
   FRNM_PDL_SF(const uint8 instIdx))
{
  /* exit action 'HandleRepeatMessageStateExit();' */
  /* It is ensured that repeat message bit and node detection feature can be
   * active only when passive mode is disabled.
   */

  /* clear the internal status flag, repeat message request */
  TS_AtomicClearBit_8(&FRNM_CHANNEL_STATUS(instIdx).ChanStatus, FRNM_REPEAT_MSG_REQUEST_BIT);

#if ((FRNM_REPEAT_MSG_BIT_ENABLED == STD_ON) && (FRNM_NODE_DETECTION_ENABLED == STD_ON))
  /* Clear repeat message bit in the transmit buffer */
  if ((FRNM_CHANNEL_PROPERTY(instIdx) & FRNM_CONTROL_BIT_VECTOR) != 0U)
  {
    TS_AtomicClearBit_8(&FRNM_CHANNEL_STATUS(instIdx).ControlBitVector, FRNM_PDU_REPEAT_MSG_BIT);
  }
#endif
  FrNm_DoActFkp[FRNM_INST(instIdx)] = NULL_PTR;
}
/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfRepeatMessageStateAction1(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* action 'ChanStatus |= UNI_TIMEOUT_PASSED;'
   * for UNI_TIMEOUT[]/...
   * internal transition */
  /* Enter critical section to protect from concurrent access
   * to different bits in 'ChanStatus' in different APIs.
   */
  TS_AtomicSetBit_8(&FRNM_CHANNEL_STATUS(instIdx).ChanStatus, FRNM_UNI_TIMEOUT_BIT);
}
/* Deviation MISRAC2012-1 */
FUNC(boolean, FRNM_CODE) FrNm_HsmFrNmSfRepeatMessageStateGuard2(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* guard condition '(ChanStatus & NETWORK_REQUESTED) &&
   * (ChanStatus & UNI_TIMEOUT_PASSED)'
   * for REP_CYCLE_COMPLETED[...]/
   * external transition to state NormalOperationState */
  return (boolean)
    ((((FRNM_CHANNEL_STATUS(instIdx).ChanStatus & FRNM_NETWORK_REQUESTED) != 0U) &&
     ((FRNM_CHANNEL_STATUS(instIdx).ChanStatus & FRNM_UNI_TIMEOUT_PASSED) != 0U)) ? TRUE : FALSE);
}
/* Deviation MISRAC2012-1 */
FUNC(boolean, FRNM_CODE) FrNm_HsmFrNmSfRepeatMessageStateGuard3(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* guard condition '( !(ChanStatus & NETWORK_REQUESTED) &&
   * (ChanStatus & UNI_TIMEOUT_PASSED) )'
   * for REP_CYCLE_COMPLETED[...]/
   * external transition to state ReadySleepState */
  return (boolean)
    ((((FRNM_CHANNEL_STATUS(instIdx).ChanStatus & FRNM_NETWORK_REQUESTED) == 0U) &&
     ((FRNM_CHANNEL_STATUS(instIdx).ChanStatus & FRNM_UNI_TIMEOUT_PASSED) != 0U)) ? TRUE : FALSE);
}

/* ************************************************************************
 * State: SynchronizeMode
 * Parent state: TOP
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 * 1) SynchronizeMode -> BusSleepMode:
      FAILSAFE_SLEEP_MODE[]/FrNm_InitInternalVar();
 * 2) SynchronizeMode -> NetworkMode:
 *    REP_CYCLE_COMPLETED[(FrNm_SyncLoss == false) ]/
 * 3) SynchronizeMode -> BusSleepMode:
 *    STARTUP_ERROR[ ! (ChanStatus & NETWORK_REQUESTED)]/
 */

/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfSynchronizeModeEntry(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* entry action '#if (SCI == STD_ON)
   * Nm_StateChangeNotification(LastState, ActState);
   * => FRNM191 FRNM192 CurState = SynchronizeState;
   * ChanStatus &=~STARTUP_ERROR;'
   */

#if (FRNM_STATE_CHANGE_INDICATION_ENABLED == STD_ON)
  const Nm_StateType PreviousState = FRNM_CHANNEL_STATUS(instIdx).CurState;
  FRNM_CHANNEL_STATUS(instIdx).CurState = NM_STATE_SYNCHRONIZE;
  FrNm_Call_Nm_StateChangeNotification(FRNM_INST(instIdx),
    PreviousState, NM_STATE_SYNCHRONIZE);
#else
  FRNM_CHANNEL_STATUS(instIdx).CurState = NM_STATE_SYNCHRONIZE;
#endif
  /* Enter critical section to protect from concurrent access
   * to different bits in 'ChanStatus' in different APIs.
   */
  TS_AtomicClearBit_8(&FRNM_CHANNEL_STATUS(instIdx).ChanStatus, FRNM_STARTUP_ERROR_BIT);
}
/* Deviation MISRAC2012-1 */
FUNC(void, FRNM_CODE) FrNm_HsmFrNmSfSynchronizeModeAction1(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* action 'InitInternalVar();'
   * for FAILSAFE_SLEEP_MODE[]/...
   * external transition to state BusSleepMode */
  FrNm_InitInternalVar(FRNM_PL_SF(instIdx));
}
/* Deviation MISRAC2012-1 */
FUNC(boolean, FRNM_CODE) FrNm_HsmFrNmSfSynchronizeModeGuard2(
  FRNM_PDL_SF(const uint8 instIdx))
{
  boolean retVal = FALSE;
  /* guard condition '(FrNm_SyncLoss == false)'
   * for REP_CYCLE_COMPLETED[...]/
   * external transition to state NetworkMode */
  /* !LINKSTO FrNm.EB.SynchLoss,1 */
  if(FRNM_CHANNEL_STATUS(instIdx).FrNm_SyncLoss == FALSE)
  {
    retVal = TRUE;
  }
  else
  {
    retVal = FALSE;
  }
  return retVal;
}
/* Deviation MISRAC2012-1 */
FUNC(boolean, FRNM_CODE) FrNm_HsmFrNmSfSynchronizeModeGuard3(
  FRNM_PDL_SF(const uint8 instIdx))
{
  /* guard condition '! (ChanStatus & NETWORK_REQUESTED)'
   * for STARTUP_ERROR[...]/
   * external transition to state BusSleepMode */

  /* !LINKSTO SWS_FrNm_00340,1, SWS_FrNm_00376,1 */
  return (boolean)
    (((FRNM_CHANNEL_STATUS(instIdx).ChanStatus & FRNM_NETWORK_REQUESTED) == 0U) ? TRUE : FALSE);
}
/*==================[internal function definitions]=========================*/
#if (FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON)
STATIC FUNC(void, FRNM_CODE) FrNm_Call_Nm_RemotesleepCancellation(uint8 instIdx)
{
#if (FRNM_HSM_INST_MULTI_ENABLED == STD_OFF)
  TS_PARAM_UNUSED(instIdx);
#endif
#if (FRNM_MULTICORE_ENABLED == STD_OFF)
  FRNM_CALL_NM_REMOTESLEEPCANCELLATION(instIdx);
#else
  if (FRNM_CALL_NM_REMOTESLEEPCANCELLATION(instIdx) != SCHM_E_OK)
  {
#if (FRNM_DEV_ERROR_DETECT == STD_ON)
    /* !LINKSTO FrNm.EB.SchM_Call_DET,1 */
    FRNM_DET_REPORT_ERROR(instIdx,
                          (FRNM_SERVID_MAINFUNCTION_X + instIdx),
                          FRNM_E_REMOTESLEEPCANCELLATION
                         );
#endif
  }
#endif
}
#endif

#if (FRNM_STATE_CHANGE_INDICATION_ENABLED == STD_ON)
STATIC FUNC(void, FRNM_CODE) FrNm_Call_Nm_StateChangeNotification(
  uint8 instIdx,
  Nm_StateType PreviousState,
  Nm_StateType CurrentState)
{
#if (FRNM_HSM_INST_MULTI_ENABLED == STD_OFF)
  TS_PARAM_UNUSED(instIdx);
#endif
#if (FRNM_MULTICORE_ENABLED == STD_OFF)
    FRNM_CALL_NM_STATECHANGENOTIFICATION(instIdx,
    PreviousState,
    CurrentState);
#else
    if (FRNM_CALL_NM_STATECHANGENOTIFICATION(instIdx,
        PreviousState,
        CurrentState) != SCHM_E_OK)
    {
#if (FRNM_DEV_ERROR_DETECT == STD_ON)
      /* !LINKSTO FrNm.EB.SchM_Call_DET,1 */
      FRNM_DET_REPORT_ERROR(instIdx,
                            (FRNM_SERVID_MAINFUNCTION_X + instIdx),
                            FRNM_E_STATECHANGENOTIFICATION
                           );
#endif
    }
#endif
}
#endif
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, FRNM_CODE) FrNm_InitInternalVar(
  FRNM_PDL_SF(uint8 instIdx))
{
  FRNM_CHANNEL_STATUS(instIdx).ChanStatus  = 0U;
  FRNM_CHANNEL_STATUS(instIdx).ActCycle = 0U;
  FRNM_CHANNEL_STATUS(instIdx).ReadySleepTimer = 0U;
  FRNM_CHANNEL_STATUS(instIdx).ComEnabled = TRUE;
#if (FRNM_PASSIVE_MODE_ENABLED == STD_OFF)
  FRNM_CHANNEL_STATUS(instIdx).TimeoutTimer = 0U;
  FRNM_CHANNEL_STATUS(instIdx).UniversalTimer = 0U;
  /* !LINKSTO FRNM042,1 */
  FRNM_CHANNEL_STATUS(instIdx).ControlBitVector = 0;

#if (FRNM_SOURCE_NODE_IDENTIFIER_ENABLED == STD_ON)
  if (FRNM_CHANNELID_CONFIG(instIdx).FrNmSourceNodeIdentifierEnabled == TRUE)
  {
    FRNM_CHANNEL_STATUS(instIdx).TxPduPtr[FRNM_SOURCE_NODE_IDENTIFIER_POS] =
      FRNM_CHANNEL_NODEID(instIdx);
  }
#endif

#if ((FRNM_USER_DATA_ENABLED  == STD_ON) || (FRNM_COM_USER_DATA_SUPPORT == STD_ON))
  /* FRNM045 */
  TS_MemSet(
      &FRNM_CHANNEL_STATUS(instIdx).TxPduPtr[FRNM_RESERVED_BYTES(instIdx)],
      0xFFU, (PduLengthType)(FRNM_CHANNEL_PDULENGTH(instIdx) - FRNM_RESERVED_BYTES(instIdx)));
#endif /*((FRNM_USER_DATA_ENABLED  == STD_ON) || ......*/
#if (FRNM_PN_SUPPORTED == STD_ON)
  if (((FRNM_CHANNEL_PROPERTY(instIdx) & FRNM_CONTROL_BIT_VECTOR) != 0U) &&
      (FRNM_CHANNEL_PNENABLED(instIdx) == TRUE)
     )
  {
    /* FRNM409, Set CRI bit */
    FRNM_CHANNEL_STATUS(instIdx).ControlBitVector |=  FRNM_CBV_PNINFOBIT_MASK;
  }
#endif
#endif /*(FRNM_PASSIVE_MODE_ENABLED == STD_OFF)*/
#if (FRNM_PN_ERA_CALC_ENABLED == STD_ON)
  /* Initialise external and internal requests */
  /* !LINKSTO FRNM432,1 */
  TS_MemSet(FRNM_CHANNEL_STATUS(instIdx).PnEra.PnTimer, 0U, (FRNM_EIRA_MAX_TIMER_SIZE * sizeof(FrNm_TimeType)));
  TS_MemSet(FRNM_CHANNEL_STATUS(instIdx).PnEra.PnInfo, 0U, FRNM_PN_INFO_LENGTH);
  TS_MemSet(FRNM_CHANNEL_STATUS(instIdx).PnEra.PnValue, 0U, FRNM_PN_INFO_LENGTH);
#endif
#if ((FRNM_CYCLE_COUNTER_EMULATION == STD_ON) && (FRNM_SYNCH_ERR_EXTENDED == STD_ON))
  FRNM_CHANNEL_STATUS(instIdx).FrNm_EmulatedCounterValid = FALSE;
  FRNM_CHANNEL_STATUS(instIdx).FrNm_EmulatedCounter = 0U;
#endif
}
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, FRNM_CODE) FrNm_ReadySleepTimerStart(
  FRNM_PL_TIMER(uint8 instIdx, FrNm_TimeType RsTimer))
{
  /* load timer value */
  FRNM_CHANNEL_STATUS(instIdx).ReadySleepTimer = RsTimer;
}
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, FRNM_CODE) FrNm_ReadySleepTimerStop(
  FRNM_PDL_SF(uint8 instIdx))
{
  /* Stop the Universal Timer */
  FRNM_CHANNEL_STATUS(instIdx).ReadySleepTimer = 0U;
}
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, FRNM_CODE) FrNm_UniTimerStart(
  FRNM_PL_TIMER(uint8 instIdx, FrNm_TimeType UniTimer))
{
  /* load timer value */
  FRNM_CHANNEL_STATUS(instIdx).UniversalTimer = UniTimer;
}
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, FRNM_CODE) FrNm_UniTimerStop(
  FRNM_PDL_SF(uint8 instIdx))
{
  /* Stop the Universal Timer */
  FRNM_CHANNEL_STATUS(instIdx).UniversalTimer = 0U;
}
/* Deviation MISRAC2012-1 */
STATIC FUNC(boolean, FRNM_CODE) FrNm_IsTransitionRmsAllowed(
  FRNM_PDL_SF(uint8 instIdx))
{
  return (boolean)
    (((FrNm_IsTransitionAllowed(FRNM_PL_SF(instIdx)) == TRUE) &&
    ((FRNM_CHANNEL_STATUS(instIdx).ChanStatus & FRNM_REPEAT_MSG_REQUESTED) != 0U)) ? TRUE : FALSE);
}
/* Deviation MISRAC2012-1 */
STATIC FUNC(boolean, FRNM_CODE) FrNm_IsTransitionNoOpAllowed(
  FRNM_PDL_SF(uint8 instIdx))
{
  /* Note: Extra braces are used to get rid of misra waring.
   * Rule 12.5(required): The operands of a logical && or || shall be
   * primar-expressions.
   */
 return (boolean) (((FrNm_IsTransitionAllowed(FRNM_PL_SF(instIdx)) == TRUE) &&
   (((FRNM_CHANNEL_STATUS(instIdx).ChanStatus & FRNM_NETWORK_REQUESTED) != 0U) &&
   ((FRNM_CHANNEL_STATUS(instIdx).ChanStatus & FRNM_REPEAT_MSG_REQUESTED) == 0U))) ? TRUE : FALSE);
}
/* Deviation MISRAC2012-1 */
STATIC FUNC(boolean, FRNM_CODE) FrNm_IsTransitionAllowed(
  FRNM_PDL_SF(uint8 instIdx))
{
  boolean retVal = FALSE;
#if ( FRNM_DUAL_CHANNEL_PDU_ENABLE == STD_ON)
  if ((FRNM_CHANNEL_PROPERTY(instIdx) & FRNM_VOTE_INHIBITION_ENABLED) != 0U)
  {
    /* FRNM357 */
    if (FRNM_CHANNEL_STATUS(instIdx).ReadySleepTimer > 1U)
    {
     retVal = TRUE;
    }
  }
else
#endif
  {
    if (FRNM_CHANNEL_STATUS(instIdx).ReadySleepTimer > 0U)
    {
      retVal = TRUE;
    }
  }
  return retVal;
}

/* Deviation MISRAC2012-1 */
STATIC FUNC(void, FRNM_CODE) FrNm_RepeatMessageStateDoAction(uint8 instIdx)
{
#if (FRNM_HSM_INST_MULTI_ENABLED == STD_OFF)
  TS_PARAM_UNUSED(instIdx);
#endif
  /* do action 'HandleRepeatMessageTimer();' */

  /* Refer FrNm0030_Conf, timeout for Repeat Message State with value  "0"
   * denotes that no Repeat Message State is configured, which means that
   * Repeat Message State is transient and implies that it is left immediately
   * after entry and consequently no startup stability is guaranteed and no
   * node detection procedure is possible.
   */

  if (FRNM_CHANNELTIME_CONFIG(instIdx).RepeatMessageTime == 0U)
  {
    /* The configured Repeat message time is 0.
     * Emit repetition cycle event in order to exit from 'repeat message state'
     * immediately.
     * Set channel status with repeat message time out bit set, to satisfy the
     * guard condition to exit from repeat message state.
     */
    /* !LINKSTO FrNm.EB.RepeatMessageTimeZero.NoMessages,1 */
    FRNM_HSMEMITTOSELFINST(&FrNm_HsmScFrNm, instIdx, FRNM_HSM_FRNM_EV_REP_CYCLE_COMPLETED);
    /* Enter critical section to protect from concurrent access
     * to different bits in 'ChanStatus' in different APIs.
     */
    TS_AtomicSetBit_8(&FRNM_CHANNEL_STATUS(instIdx).ChanStatus, FRNM_UNI_TIMEOUT_BIT);
  }
}

#define FRNM_STOP_SEC_CODE
#include <FrNm_MemMap.h>

/*==================[end of file]===========================================*/
