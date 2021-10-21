/**
 * \file
 *
 * \brief AUTOSAR LinSM
 *
 * This file contains the implementation of the AUTOSAR
 * module LinSM.
 *
 * \version 3.4.19
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*
 *  MISRA-C:2012 Deviations:
 *
 *  MISRAC2012-1) Deviated Rule: 2.2
 *    There shall be no dead code
 *
 *    Reason:
 *    - If transceiver notification is disabled the macro is
 *      expanded into an empty statement.
 */

/*==================[inclusions]============================================*/

#include <LinSM_Trace.h>
#include <LinSM_Internal.h>
#include <BswM_LinSM.h>

/*==================[macros]================================================*/

#if (defined LINSM_TRANSIT_TO_FULL_COM) /* to prevent double declaration */
#error LINSM_TRANSIT_TO_FULL_COM already defined
#endif /* if (defined LINSM_TRANSIT_TO_FULL_COM) */

#if (defined LINSM_TRANSIT_TO_NO_COM) /* to prevent double declaration */
#error LINSM_TRANSIT_TO_NO_COM already defined
#endif /* if (defined LINSM_TRANSIT_TO_NO_COM) */

#if (LINSM_SLEEPSUPPORT_TYPE != LINSM_ALL)

/** \brief shortcut to improve readability: transition from NO_COM to FULL_COM */
#define LINSM_TRANSIT_TO_FULL_COM(id)                                           \
  do {                                                                          \
    LinSM_ImmediateTransition( (id), LINSM_FULL_COM, COMM_FULL_COMMUNICATION ); \
    LINSM_NOTIF_TRCV_NORMAL(id);                                                \
  } while(0U)

/** \brief shortcut to improve readability: transition from FULL_COM to NO_COM */
#define LINSM_TRANSIT_TO_NO_COM(id)                                         \
  do {                                                                      \
    LinSM_ImmediateTransition( (id), LINSM_NO_COM, COMM_NO_COMMUNICATION ); \
    LINSM_NOTIF_TRCV_SLEEP(id);                                             \
  } while (0U)

#endif /* LINSM_SLEEPSUPPORT_TYPE != LINSM_ALL */

/*==================[type definitions]======================================*/

/*==================[internal function declarations]========================*/

#if (TS_MERGED_COMPILE == STD_OFF)
#define LINSM_START_SEC_CODE
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */
#endif /* TS_MERGED_COMPILE */

#if (LINSM_SLEEPSUPPORT_TYPE != LINSM_ALL)

/** \brief change immediately to LINSM_NO_COM or LINSM_FULL_COM
 *
 * immediately sets channel state. used if channel doesn't support sleep mode.
 *
 * Assumes that channnel is not in state \p to and sets state to \p to
 * and signals mode \p mode to ComM
 *
 * \param[in] id  LinSM channel id of channel to operate on
 * \param[in] to  state to change to
 * \param[in] mode  mode to report to ComM in case transition is successful
 */
STATIC FUNC(void, LINSM_CODE) LinSM_ImmediateTransition
(
  LinSM_ChannelIdType id,
  LinSM_ModeType to,
  ComM_ModeType  mode
);

#endif /* LINSM_SLEEPSUPPORT_TYPE != LINSM_ALL */

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

FUNC(Std_ReturnType, LINSM_CODE) LinSM_RequestComMode
(
  NetworkHandleType  network,
  ComM_ModeType  mode
)
{
  Std_ReturnType retval;
  LinSM_ChannelIdType id;

  DBG_LINSM_REQUESTCOMMODE_ENTRY(network,mode);

#if (LINSM_DEV_ERROR_DETECT == STD_ON)
  if ( LinSM_GlobalState == LINSM_GLOBAL_UNINIT )
  {
    LINSM_DET(LINSM_SID_REQUESTCOMMODE, LINSM_E_UNINIT);
    retval = E_NOT_OK;
  }
  else if ( ! (LINSM_COMM_NW_IS_VALID(network)) )
  {
    LINSM_DET(LINSM_SID_REQUESTCOMMODE, LINSM_E_NONEXISTENT_NETWORK);
    retval = E_NOT_OK;
  }
  else
#endif
  {
    uint8 currentState;
    uint8 StoredMode;
#if (LINSM_SLAVE_NODES_ENABLED == STD_ON)
  LinSM_TimerValueType SilenceAfterWakeupTimer;
#endif

    id = LINSM_COMM_NW_TO_ID(network);

    LINSM_ENTER_CRITICAL_SECTION(); /* protect channel state and timer */

    currentState = LINSM_CHANNEL_STATE(id);
    StoredMode = LINSM_CHANNEL_STORED_COM(id);
#if (LINSM_SLAVE_NODES_ENABLED == STD_ON)
    SilenceAfterWakeupTimer = LINSM_CHANNEL_SILENCE_TIME(id);
#endif

    LINSM_LEAVE_CRITICAL_SECTION();

    switch (mode)
    {
      case COMM_FULL_COMMUNICATION:
      {
        if (((currentState == LINSM_NO_COM)&&(StoredMode == NOTHING_STORED))
           || (currentState == LINSM_GOTO_SLEEP)
           )
        {
          /* !LINKSTO LinSM.ASRR20-11.SWS_LinSM_00237,1 */
#if (LINSM_SLAVE_NODES_ENABLED == STD_ON)
          /* if   there was a failed wakeup and it waits a configured time before retrying
             AND  the request did not come between the driver going to sleep and LinSM's call to ComM to indicate this */
          if ((SilenceAfterWakeupTimer == LINSM_INVALID_SILENCE_AFTER_WAKEUP) && (LINSM_CHANNEL_SLAVE_GOTOSLEEPINPROGRESS(id) == FALSE))
#endif
          {
#if (LINSM_SLAVE_NODES_ENABLED == STD_ON)
            LINSM_CHANNEL_SLAVE_WAITNOCOMREQ(id) = FALSE;
            LINSM_CHANNEL_SLAVE_WAITSLEEPCONF(id) = TRUE;
#endif
            /* state is either
               - NO_COM and no request stored or
               - FULL_COM and NO_COM requested (i.e. LinIf_GotoSleep called)
               -> call LinIf_Wakeup or make direct transition to FULL_COM */
#if (LINSM_SLEEPSUPPORT_TYPE == LINSM_ALL)
            LinSM_WakeUp(id);
#elif (LINSM_SLEEPSUPPORT_TYPE == LINSM_NONE)
            /* Deviation MISRAC2012-1 */
            LINSM_TRANSIT_TO_FULL_COM(id);
#else /* LINSM_SLEEPSUPPORT_TYPE == LINSM_MIXED */
            if (LINSM_CHANNEL_SLEEPSUPPORT(id) != FALSE)
            {
              LinSM_WakeUp(id);
            }
            else
            {
              /* Deviation MISRAC2012-1 */
              LINSM_TRANSIT_TO_FULL_COM(id);
            }
#endif /* LINSM_SLEEPSUPPORT_TYPE */
            LINSM_CHANNEL_REPETITION_TIME(id) = LINSM_CHANNEL_MODEREQUQSTREPETITIONTIME(id);
          }
        }
        else
        {
          /* already in state FULL_COM or LinIf_Wakeup called and waiting for confirmation
             -> just reset repetition counter */
          LINSM_CHANNEL_REPETITION_TIME(id) = 0U;
          LINSM_CHANNEL_STORED_COM(id) = NOTHING_STORED;
        }
        retval = E_OK;
        break;
      }

      case COMM_NO_COMMUNICATION:
      {
          if ( ((currentState == LINSM_FULL_COM)&&(StoredMode == NOTHING_STORED))
             ||(currentState == LINSM_WAKEUP)
             )
          {
#if (LINSM_SLAVE_NODES_ENABLED == STD_ON)
            if (LINSM_CHANNEL_IS_SLAVE(id) == TRUE)
            {
              /* !LINKSTO LinSM.ASRR20-11.SWS_LinSM_00230_EB,1 */
              LINSM_CHANNEL_STORED_COM(id) = NOTHING_STORED;
            }
            else
#endif
            {
              /* state is either
                 - FULL_COM and no request stored or
                 - NO_COM and FULL_COM requested (i.e. LinIf_Wakeup called)
                 -> call LinIf_GotoSleep or make direct transition to NO_COM */
#if (LINSM_SLEEPSUPPORT_TYPE == LINSM_ALL)
              LinSM_GotoSleep(id);
#elif (LINSM_SLEEPSUPPORT_TYPE == LINSM_NONE)
              /* Deviation MISRAC2012-1 */
              LINSM_TRANSIT_TO_NO_COM(id);
#else /* LINSM_SLEEPSUPPORT_TYPE == LINSM_MIXED */
              if (LINSM_CHANNEL_SLEEPSUPPORT(id) != FALSE)
              {
                LinSM_GotoSleep(id);
              }
              else
              {
                /* Deviation MISRAC2012-1 */
                LINSM_TRANSIT_TO_NO_COM(id);
              }
#endif /* LINSM_SLEEPSUPPORT_TYPE */
              LINSM_CHANNEL_REPETITION_TIME(id) = LINSM_CHANNEL_MODEREQUQSTREPETITIONTIME(id);
            }
          }
          else
          {
#if (LINSM_SLAVE_NODES_ENABLED == STD_ON)
            if ((LINSM_CHANNEL_IS_SLAVE(id) == TRUE) && (currentState == LINSM_GOTO_SLEEP))
            {
              if (LINSM_CHANNEL_SLAVE_WAITNOCOMREQ(id) == TRUE)
              {
                if (LINSM_CHANNEL_SLAVE_WAITSLEEPCONF(id) == FALSE)
                {
                  /* confirmation is expected - stop confirmation timeout */
                  LINSM_CHANNEL_TIMER(id) = 0U;
                  LINSM_CHANNEL_STATE(id) = LINSM_NO_COM;

                  LINSM_CHANNEL_SLAVE_WAITNOCOMREQ(id) = FALSE;
                  LINSM_CHANNEL_SLAVE_WAITSLEEPCONF(id) = TRUE;
                  LINSM_CHANNEL_SLAVE_GOTOSLEEPINPROGRESS(id) = FALSE;

                  /* signal ComM and BswM */
                  LINSM_CALL_SM_MODEINDICATION(network, mode);
                  BswM_LinSM_CurrentState(network, LINSM_NO_COM);
                  /* Deviation MISRAC2012-1 */
                  LINSM_NOTIF_TRCV_SLEEP(id);
                }
                else
                {
                  LINSM_CHANNEL_SLAVE_WAITNOCOMREQ(id) = FALSE;
                }
              }
              else
              {
                LINSM_CHANNEL_STORED_COM(id) = NOTHING_STORED;
              }
            }
            else
#endif
            {
              /* already in state NO_COM or LinIf_GotoSleep called and waiting for confirmation
                 -> just reset repetition counter */
              LINSM_CHANNEL_REPETITION_TIME(id) = 0U;
              LINSM_CHANNEL_STORED_COM(id) = NOTHING_STORED;
            }
          }
          retval = E_OK;
          break;
      }
      case COMM_SILENT_COMMUNICATION:
      {
        retval = E_OK;
        break;
      }
      default:
      {
        /* an invalid mode transition was requested: request not accepted */
#if (LINSM_DEV_ERROR_DETECT == STD_ON)
        LINSM_DET(LINSM_SID_REQUESTCOMMODE, LINSM_E_PARAMETER);
#endif/* LINSM_DEV_ERROR_DETECT */
        retval = E_NOT_OK;
        break;
      }
    }
  }

  DBG_LINSM_REQUESTCOMMODE_EXIT(retval,network,mode);
  return retval;
}

/*==================[internal function definitions]=========================*/

#if (LINSM_SLEEPSUPPORT_TYPE != LINSM_NONE)

FUNC(void, LINSM_CODE) LinSM_WakeUp
(
  LinSM_ChannelIdType id
)
{
  Std_ReturnType retval = E_NOT_OK;
  uint8 currentState;

  /* enter critical section */
  LINSM_ENTER_CRITICAL_SECTION(); /* protect channel state and timer */

  /* note: state must be set before calling LinIf, as the confirmation may
     reach us before the LinIf call returns */
  DBG_LINSM_CHANNELDATA_STATE_GRP(id, (LINSM_CHANNEL_STATE(id)), (LINSM_WAKEUP));
  /* save current state */
  currentState = LINSM_CHANNEL_STATE(id);
  /* assign wakeup state */
  LINSM_CHANNEL_STATE(id) = LINSM_WAKEUP;
  /* start confirmation timeout */
  LINSM_CHANNEL_TIMER(id) = LINSM_CHANNEL_CONFTIMEOUT(id);

  /* leave critical section */
  LINSM_LEAVE_CRITICAL_SECTION();

  retval = LinIf_Wakeup(LINSM_CHANNEL_LINIF_CHANNEL(id));

  /* LinIf denied the request: reset channel state to old value */
  /* enter critical section */
  LINSM_ENTER_CRITICAL_SECTION();

  if (retval != E_OK)
  {
    /* LinIf denied the request: reset channel state to old value */
    /* enter critical section */

    /* it is possible that the wakeup confirmation timer expires before we
       get to this point (this means that we have been preempted by
       LinSM_MainFunction enough times). However, as the timeout will
       reset the state to NO_COM anyways, we need no special handling for
       this here. */
    DBG_LINSM_CHANNELDATA_STATE_GRP(id, (LINSM_CHANNEL_STATE(id)), (currentState));
    /* set to previous state */
    LINSM_CHANNEL_STATE(id) = currentState;
    LINSM_CHANNEL_TIMER(id) = 0U;

    LINSM_CHANNEL_STORED_COM(id) = FULL_COM_STORED;

  }
  else
  {
    LINSM_CHANNEL_STORED_COM(id) = NOTHING_STORED;
  }
  /* leave critical section */
  LINSM_LEAVE_CRITICAL_SECTION();

}


FUNC(void, LINSM_CODE) LinSM_GotoSleep
(
  LinSM_ChannelIdType id
)
{
  Std_ReturnType retval;
  uint8 currentState;

  /* enter critical section */
  LINSM_ENTER_CRITICAL_SECTION(); /* protect channel state and timer */

  /* note: state must be set before calling LinIf, as the confirmation may
     reach us before the LinIf call returns */
  DBG_LINSM_CHANNELDATA_STATE_GRP(id, (LINSM_CHANNEL_STATE(id)), (LINSM_GOTO_SLEEP));
  /* save current state */
  currentState = LINSM_CHANNEL_STATE(id);
  /* assign gotosleep state */
  LINSM_CHANNEL_STATE(id) = LINSM_GOTO_SLEEP;
  /* start new confirmation timeout, remember old timer value */
  LINSM_CHANNEL_TIMER(id) = LINSM_CHANNEL_CONFTIMEOUT(id);

  /* leave critical section */
  LINSM_LEAVE_CRITICAL_SECTION();

  retval = LinIf_GotoSleep(LINSM_CHANNEL_LINIF_CHANNEL(id));

  /* LinIf denied the request: reset channel state to old value */
  /* enter critical section */
  LINSM_ENTER_CRITICAL_SECTION();

  if (retval != E_OK)
  {

    /* reset old state */
    DBG_LINSM_CHANNELDATA_STATE_GRP(id, (LINSM_CHANNEL_STATE(id)), (currentState));
    /* set to previous state */
    LINSM_CHANNEL_STATE(id) = currentState;
#if (LINSM_SLAVE_NODES_ENABLED == STD_ON)
    if (LINSM_CHANNEL_IS_SLAVE(id) == TRUE)
    {
      /* !LINKSTO LinSM.ASRR20-11.SWS_LinSM_00230_EB,1 */
      LINSM_CHANNEL_STORED_COM(id) = NOTHING_STORED;
    }
    else
#endif
    {
      LINSM_CHANNEL_STORED_COM(id) = NO_COM_STORED;
    }
    LINSM_CHANNEL_TIMER(id) = 0U;
  }
  else
  {
    LINSM_CHANNEL_STORED_COM(id) = NOTHING_STORED;

#if (LINSM_SLAVE_NODES_ENABLED == STD_ON)
    if (LINSM_CHANNEL_IS_SLAVE(id) == TRUE)
    {
      LINSM_CHANNEL_SLAVE_WAITNOCOMREQ(id) = TRUE;
      LINSM_CHANNEL_SLAVE_GOTOSLEEPINPROGRESS(id) = TRUE;
    }
#endif
  }
  /* leave critical section */

  LINSM_LEAVE_CRITICAL_SECTION();

#if (LINSM_SLAVE_NODES_ENABLED == STD_ON)
  if (LINSM_CHANNEL_IS_SLAVE(id) == TRUE)
  {
    if (retval == E_OK)
    {
      /* !LINKSTO LinSM.ASRR20-11.SWS_LinSM_00233_EB,1 */
      LINSM_CALL_SM_BUSSLEEPMODE(LINSM_CHANNEL_COMM_NW(id));
    }
  }
#endif

}

#endif /* LINSM_SLEEPSUPPORT_TYPE != LINSM_NONE */

#if (LINSM_SLEEPSUPPORT_TYPE != LINSM_ALL)

STATIC FUNC(void, LINSM_CODE) LinSM_ImmediateTransition
(
  LinSM_ChannelIdType id,
  LinSM_ModeType to,
  ComM_ModeType  mode
)
{

  /* enter critical section */
  LINSM_ENTER_CRITICAL_SECTION(); /* protect channel state */

  DBG_LINSM_CHANNELDATA_STATE_GRP(id, (LINSM_CHANNEL_STATE(id)), (to));
  LINSM_CHANNEL_STATE(id) = to;
  LINSM_CHANNEL_TIMER(id) = 0U;

  /* leave critical section */
  LINSM_LEAVE_CRITICAL_SECTION();

  /* signal ComM and BswM */
  LINSM_CALL_SM_MODEINDICATION(LINSM_CHANNEL_COMM_NW(id), mode);
  BswM_LinSM_CurrentState(LINSM_CHANNEL_COMM_NW(id), to);

}

#endif /* LINSM_SLEEPSUPPORT_TYPE != LINSM_ALL */

#if (TS_MERGED_COMPILE == STD_OFF)
#define LINSM_STOP_SEC_CODE
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */
#endif /* TS_MERGED_COMPILE */

/*==================[end of file]===========================================*/

