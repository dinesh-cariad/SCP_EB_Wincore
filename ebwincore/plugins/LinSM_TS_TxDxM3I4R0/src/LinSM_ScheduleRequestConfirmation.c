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

#if (LINSM_MASTER_NODES_ENABLED == STD_ON)
/* !LINKSTO LinSM.ASRR20-11.SWS_LinSM_00242,1 */

#include <BswM_LinSM.h>

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[internal function declarations]========================*/

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#if (TS_MERGED_COMPILE == STD_OFF)
#define LINSM_START_SEC_CODE
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */
#endif /* TS_MERGED_COMPILE */

FUNC(void, LINSM_CODE) LinSM_ScheduleRequestConfirmation
(
  NetworkHandleType network,
  LinIf_SchHandleType schedule
)
{
  LinSM_ChannelIdType id;
  ComM_ModeType  mode;

  DBG_LINSM_SCHEDULEREQUESTCONFIRMATION_ENTRY(network,schedule);

#if (LINSM_DEV_ERROR_DETECT == STD_ON)
  if ( LinSM_GlobalState == LINSM_GLOBAL_UNINIT )
  {
    LINSM_DET(LINSM_SID_SCHEDULEREQUESTCONF, LINSM_E_UNINIT);
  }
  else
#endif
  {
#if (LINSM_DEV_ERROR_DETECT == STD_ON)
    if ( ! (LINSM_COMM_NW_IS_VALID(network)) )
    {
      LINSM_DET(LINSM_SID_SCHEDULEREQUESTCONF, LINSM_E_NONEXISTENT_NETWORK);
    }
    else
#endif
    {
      id = LINSM_COMM_NW_TO_ID(network);

      /* enter critical section */
      LINSM_ENTER_CRITICAL_SECTION(); /* protect channel state and schedule */

      mode = LINSM_CHANNEL_STATE(id);
      if (mode == LINSM_FULL_COM)
      {
        LINSM_CHANNEL_LASTSCHEDULE(id) = schedule;
        /* stop confirmation timeout */
        LINSM_CHANNEL_TIMER(id) = 0U;
      }
      /* leave critical section */
      LINSM_LEAVE_CRITICAL_SECTION();

#if (LINSM_DEV_ERROR_DETECT == STD_ON)
      /* state not as expected */
      if (mode != LINSM_FULL_COM)
      {
        /* the LinIf may signal switching to NULL_SCHEDULE during
          initialization or after a goto sleep request - therefore only
          signal DET for non-NULL_SCHEDULE confirmations */
        if (schedule != 0U)
        {
          LINSM_DET(LINSM_SID_SCHEDULEREQUESTCONF, LINSM_E_UNEXPECTED_CALLOUT);
        }

      }
#endif
      /* notify BswM that schedule table changed */
      BswM_LinSM_CurrentSchedule(network, schedule);

      TS_AtomicAssign8(LINSM_CHANNEL_SCHEDULEREQUESTACTIVE(id), FALSE);
    }
  }

  DBG_LINSM_SCHEDULEREQUESTCONFIRMATION_EXIT(network,schedule);
}

#if (TS_MERGED_COMPILE == STD_OFF)
#define LINSM_STOP_SEC_CODE
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */
#endif /* TS_MERGED_COMPILE */

#endif /* (LINSM_MASTER_NODES_ENABLED == STD_ON) */

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/

