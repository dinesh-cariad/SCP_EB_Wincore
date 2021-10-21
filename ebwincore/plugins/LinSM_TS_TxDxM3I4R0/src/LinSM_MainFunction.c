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

/*==================[inclusions]============================================*/

#include <LinSM_Trace.h>
#include <LinSM_Internal.h>
#include <BswM_LinSM.h>

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[internal function declarations]========================*/

#if (TS_MERGED_COMPILE == STD_OFF)
#define LINSM_START_SEC_CODE
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */
#endif /* TS_MERGED_COMPILE */

#if (LINSM_SLAVE_NODES_ENABLED == STD_ON)

/** \brief decrement (if needed) the silence after wakeup timer
 *
 * in certain conditions, it decrements the silence after wakeup timer
 * for a specific channel
 *
 * \param[in] id  LinSM channel id of channel to operate on
 */
STATIC FUNC(boolean, LINSM_CODE) LinSM_DecrementSilenceTimer
(
  LinSM_ChannelIdType id
);

/** \brief set (if needed) the silence after wakeup timer to the configured value
 *
 * in certain conditions, it sets the silence after wakeup timer to the configured
 * for a specific channel
 *
 * \param[in] id  LinSM channel id of channel to operate on
 */
STATIC FUNC(void, LINSM_CODE) LinSM_SetSilenceTimer
(
  LinSM_ChannelIdType id,
  uint8 currentState,
  uint8 StoredMode
);
#endif /* (LINSM_SLAVE_NODES_ENABLED == STD_ON) */

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

FUNC(void, LINSM_CODE) LinSM_MainFunction
(
  void
)
{
  LinSM_ChannelIdType id;

  DBG_LINSM_MAINFUNCTION_ENTRY();

  /* Do not execute main function until module is initialized,
     this may be the case during ECU startup */
  if ( LinSM_GlobalState == LINSM_GLOBAL_UNINIT )
  {
    /* LinSM_Init() was not called. Leave main function without a
       call of Det_ReportError() */
  }
  else
  {
    for (id = 0U; id < LINSM_NUM_CHANNELS; id++)
    {
      LinSM_TimerValueType timer;
#if (LINSM_MASTER_NODES_ENABLED == STD_ON)
      boolean notifyCurrentSchedule;
      LinIf_SchHandleType currentSchedule;
#endif
      uint8 StoredMode;
      uint8 currentState;
      uint8 ModeRequestRepetitionTime;

#if (LINSM_DEV_ERROR_DETECT == STD_ON)
      boolean TimerIsZero = FALSE;
#endif

#if LINSM_SLEEPSUPPORT_TYPE != LINSM_NONE
      boolean Wakeup = FALSE;
      boolean Sleep = FALSE;
#endif

      /* enter critical section */
      LINSM_ENTER_CRITICAL_SECTION(); /* protect channel state and timer */
#if (LINSM_MASTER_NODES_ENABLED == STD_ON)
      notifyCurrentSchedule = LINSM_CHANNEL_NOTIFYLASTSCHEDULE(id);
      currentSchedule = LINSM_CHANNEL_LASTSCHEDULE(id);
#endif
      timer = LINSM_CHANNEL_TIMER(id);
      currentState = LINSM_CHANNEL_STATE(id);
      ModeRequestRepetitionTime = LINSM_CHANNEL_REPETITION_TIME(id);
      StoredMode = LINSM_CHANNEL_STORED_COM(id);

#if (LINSM_MASTER_NODES_ENABLED == STD_ON)
#if (LINSM_SLAVE_NODES_ENABLED == STD_ON)
      if (LINSM_CHANNEL_IS_SLAVE(id) == FALSE)
#endif
      {
        /* will notify BswM in this main function call */
        LINSM_CHANNEL_NOTIFYLASTSCHEDULE(id) = FALSE;
      }
#endif

#if (LINSM_SLAVE_NODES_ENABLED == STD_ON)
      Wakeup = LinSM_DecrementSilenceTimer(id);
#endif

#if LINSM_SLEEPSUPPORT_TYPE != LINSM_NONE
      if (
            ((currentState == LINSM_NO_COM)&&(StoredMode == FULL_COM_STORED))
         || ((currentState == LINSM_FULL_COM)&&(StoredMode == NO_COM_STORED))
         || ((currentState == LINSM_WAKEUP)&&(StoredMode == FULL_COM_STORED))
         || ((currentState == LINSM_GOTO_SLEEP)&&(StoredMode == NO_COM_STORED))
         )
      {
        /* !LINKSTO LinSM.ASRR20-11.SWS_LinSM_00234_EB,1 */
        /* A mode request was stored after an unsuccessful call to LinIf in LinSM_RequestComMode */
        if(ModeRequestRepetitionTime == 0U)
        {
#if (LINSM_SLAVE_NODES_ENABLED == STD_ON)
          LinSM_SetSilenceTimer(id, currentState, StoredMode);
#endif
          /* The maximum number of repetitions of calling LinIf is succeeded
             -> signal DET */
#if (LINSM_DEV_ERROR_DETECT == STD_ON)
          LINSM_DET(LINSM_SID_MAINFUNCTION, LINSM_E_REPETITION_MAX_REACHED);
#endif /* LINSM_DEV_ERROR_DETECT == STD_ON */
          LINSM_CHANNEL_STORED_COM(id) = NOTHING_STORED;
          DBG_LINSM_CHANNELDATA_STATE_GRP(id, (LINSM_CHANNEL_STATE(id)), (LINSM_NO_COM));
          LINSM_CHANNEL_STATE(id) = LINSM_NO_COM;
        }
        else
        {
          /* retry to call LinIf to reach the stored mode */
          ModeRequestRepetitionTime--;
          LINSM_CHANNEL_REPETITION_TIME(id) = ModeRequestRepetitionTime;
          if (StoredMode == FULL_COM_STORED)
          {
            Wakeup = TRUE;
          }
          else
          {
            Sleep = TRUE;
          }
        }
      }
#endif /* #if LINSM_SLEEPSUPPORT_TYPE != LINSM_NONE */

      if ( ((currentState != LINSM_NO_COM)&&(StoredMode==NOTHING_STORED))
         &&(timer > 0U)
         )
      {
        /* A schedule request or mode request is in progress
           -> check the timer */
        timer--;

        if (timer == 0U)
        {
            /* timeout occured - this could be a schedule request or a mode request timeout */
            if (currentState == LINSM_FULL_COM)
            {
#if (LINSM_MASTER_NODES_ENABLED == STD_ON)
                /* timer elapsed in state FULL_COM -> this is a schedule request timeout */
                notifyCurrentSchedule = TRUE;
#endif
                TS_AtomicAssign8(LINSM_CHANNEL_TIMER(id),0U);
#if (LINSM_DEV_ERROR_DETECT == STD_ON)
                TimerIsZero = TRUE;
#endif
            }
            else
            {
#if (LINSM_SLAVE_NODES_ENABLED == STD_ON)
              if ((LINSM_CHANNEL_IS_SLAVE(id) == TRUE) && ((currentState == LINSM_GOTO_SLEEP)))
              {
#if (LINSM_DEV_ERROR_DETECT == STD_ON)
                TimerIsZero = TRUE;
#endif
                LINSM_CHANNEL_SLAVE_WAITNOCOMREQ(id) = FALSE;
                LINSM_CHANNEL_SLAVE_WAITSLEEPCONF(id) = TRUE;
                LINSM_CHANNEL_SLAVE_GOTOSLEEPINPROGRESS(id) = FALSE;
              }
#endif
              if (ModeRequestRepetitionTime == 0U)
              {
#if (LINSM_SLAVE_NODES_ENABLED == STD_ON)
                  LinSM_SetSilenceTimer(id, currentState, StoredMode);
#endif
                  /* timer elapsed in state WAKEUP or GOTO_SLEEP and max number of repetitions exceeded
                     -> set mode NO_COM and report CONFIRMATION_TIMEOUT DET error */
#if (LINSM_DEV_ERROR_DETECT == STD_ON)
                  /* !LINKSTO LinSM.ASRR20-11.SWS_LinSM_00307,1 */
                  TimerIsZero = TRUE;
#endif

                  /* timeout occured */
                  LINSM_CHANNEL_TIMER(id) = 0U;
                  DBG_LINSM_CHANNELDATA_STATE_GRP(id, (LINSM_CHANNEL_TIMER(id)), 0U);

                  DBG_LINSM_CHANNELDATA_STATE_GRP(id, (LINSM_CHANNEL_STATE(id)), LINSM_NO_COM);
                  LINSM_CHANNEL_STATE(id) = LINSM_NO_COM;

              }
              else
              {
#if LINSM_SLEEPSUPPORT_TYPE != LINSM_NONE
                /* timer elapsed in state WAKEUP or GOTO_SLEEP
                   -> retry to call LinIf */

                ModeRequestRepetitionTime--;
                switch(currentState)
                {
                    case LINSM_WAKEUP:
                    {
                        Wakeup = TRUE;
                        break;
                    }
                    case LINSM_GOTO_SLEEP:
                    {
#if (LINSM_SLAVE_NODES_ENABLED == STD_ON)
                      if (LINSM_CHANNEL_IS_SLAVE(id) == FALSE)
#endif
                      {
                        Sleep = TRUE;
                      }
                      break;
                    }
                    /* CHECK: NOPARSE */
                    default:
                    {
                        /* unreachable - timeout while in state NO_COM */
#if (LINSM_DEV_ERROR_DETECT == STD_ON)
                        LINSM_UNREACHABLE_CODE_ASSERT(LINSM_SID_MAINFUNCTION);
#endif
                        break;
                    }
                    /* CHECK: PARSE */
                }

                LINSM_CHANNEL_REPETITION_TIME(id) = ModeRequestRepetitionTime;
#endif /* #if LINSM_SLEEPSUPPORT_TYPE != LINSM_NONE */
              }
            }
        }
        else
        {
          /* update timer value */
          TS_AtomicAssign8(LINSM_CHANNEL_TIMER(id),timer);
        }
      }

      /* leave critical section */
      LINSM_LEAVE_CRITICAL_SECTION();

#if LINSM_SLEEPSUPPORT_TYPE != LINSM_NONE

      if(Wakeup == TRUE)
      {
        LinSM_WakeUp(id);
      }
      if(Sleep == TRUE)
      {
        LinSM_GotoSleep(id);
      }
#endif /* LINSM_SLEEPSUPPORT_TYPE != LINSM_NONE */


/* signal DET */
#if (LINSM_DEV_ERROR_DETECT == STD_ON)
      if (TimerIsZero == TRUE)
      {
        LINSM_DET(LINSM_SID_MAINFUNCTION, LINSM_E_CONFIRMATION_TIMEOUT);
      }
#endif /* LINSM_DEV_ERROR_DETECT == STD_ON */

#if (LINSM_MASTER_NODES_ENABLED == STD_ON)
      if (notifyCurrentSchedule != FALSE)
      {
        BswM_LinSM_CurrentSchedule(LINSM_CHANNEL_COMM_NW(id), currentSchedule);
      }
#endif
    }
  }

  DBG_LINSM_MAINFUNCTION_EXIT();
}

#if (LINSM_SLAVE_NODES_ENABLED == STD_ON)
STATIC FUNC(boolean, LINSM_CODE) LinSM_DecrementSilenceTimer
(
  LinSM_ChannelIdType id
)
{
  boolean ret = FALSE;
  LinSM_TimerValueType SilenceAfterWakeupTimer = LINSM_CHANNEL_SILENCE_TIME(id);

  if (SilenceAfterWakeupTimer != LINSM_INVALID_SILENCE_AFTER_WAKEUP)
  {
    if (SilenceAfterWakeupTimer > 0U)
    {
      SilenceAfterWakeupTimer--;
      LINSM_CHANNEL_SILENCE_TIME(id) = SilenceAfterWakeupTimer;
    }
    else
    {
      /* !LINKSTO LinSM.ASRR20-11.SWS_LinSM_00236,1 */
      /* start confirmation timeout */
      LINSM_CHANNEL_TIMER(id) = LINSM_CHANNEL_CONFTIMEOUT(id);
      LINSM_CHANNEL_REPETITION_TIME(id) = LINSM_CHANNEL_MODEREQUQSTREPETITIONTIME(id);
      ret = TRUE;
      LINSM_CHANNEL_SILENCE_TIME(id) = LINSM_INVALID_SILENCE_AFTER_WAKEUP;
    }
  }

  return ret;
}

STATIC FUNC(void, LINSM_CODE) LinSM_SetSilenceTimer
(
  LinSM_ChannelIdType id,
  uint8 currentState,
  uint8 StoredMode
)
{
  if ((LINSM_CHANNEL_IS_SLAVE(id) == TRUE) && ((currentState == LINSM_NO_COM) || ((currentState == LINSM_WAKEUP)&&(StoredMode == NOTHING_STORED))))
  {
    /* !LINKSTO LinSM.ASRR20-11.SWS_LinSM_00235,1 */
    LINSM_CHANNEL_SILENCE_TIME(id) = LINSM_CHANNEL_SILENCEAFTERWUPTIMEOUT(id);
  }
}
#endif /* (LINSM_SLAVE_NODES_ENABLED == STD_ON) */


#if (TS_MERGED_COMPILE == STD_OFF)
#define LINSM_STOP_SEC_CODE
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */
#endif /* TS_MERGED_COMPILE */

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/

