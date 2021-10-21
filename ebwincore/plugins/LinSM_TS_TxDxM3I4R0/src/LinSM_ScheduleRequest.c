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
/* !LINKSTO LinSM.ASRR20-11.SWS_LinSM_00241,1 */

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

FUNC(Std_ReturnType, LINSM_CODE) LinSM_ScheduleRequest
(
  NetworkHandleType network,
  LinIf_SchHandleType schedule
)
{
  boolean FullCommunication = FALSE;
  Std_ReturnType retval = E_NOT_OK;
  LinSM_ChannelIdType id;

  DBG_LINSM_SCHEDULEREQUEST_ENTRY(network,schedule);

#if (LINSM_DEV_ERROR_DETECT == STD_ON)
  if ( LinSM_GlobalState == LINSM_GLOBAL_UNINIT )
  {
    LINSM_DET(LINSM_SID_SCHEDULEREQUEST, LINSM_E_UNINIT);
  }
  else if ( ! (LINSM_COMM_NW_IS_VALID(network)) )
  {
    LINSM_DET(LINSM_SID_SCHEDULEREQUEST, LINSM_E_NONEXISTENT_NETWORK);
  }
  else
#endif
  {
    id = LINSM_COMM_NW_TO_ID(network);
#if (LINSM_SLAVE_NODES_ENABLED == STD_ON)
    if (LINSM_CHANNEL_IS_SLAVE(id) == TRUE)
    {
      /* !LINKSTO LinSM.EB.LINSM_ScheduleRequestOnlyOnMasterNodes,1 */
      retval = E_NOT_OK;
    }
    else
#endif
    {
      boolean OtherRequestInProgress = FALSE;

      /* enter critical section */
      LINSM_ENTER_CRITICAL_SECTION(); /* protect channel state and timer */

      if (LINSM_CHANNEL_STATE(id) == LINSM_FULL_COM)
      {
        FullCommunication = TRUE;

        if((LINSM_CHANNEL_SCHEDULEREQUESTACTIVE(id) == TRUE)
           || (LINSM_CHANNEL_STORED_COM(id) != NOTHING_STORED)
          )
        {
          OtherRequestInProgress = TRUE;
        }
        else
        {
          /* wait for a confirmation */
          LINSM_CHANNEL_TIMER(id) = LINSM_CHANNEL_CONFTIMEOUT(id);
          LINSM_CHANNEL_SCHEDULEREQUESTACTIVE(id) = TRUE;
        }
      }
      /* leave critical section */
      LINSM_LEAVE_CRITICAL_SECTION();

      if ((OtherRequestInProgress == FALSE) && (FullCommunication == TRUE))
      {
        retval = LinIf_ScheduleRequest(LINSM_CHANNEL_LINIF_CHANNEL(id), schedule);

        /* enter critical section */
        LINSM_ENTER_CRITICAL_SECTION(); /* protect channel state and timer */

        if (retval != E_OK)
        {
          /* LinIf denied the request: reset channel timer and notify BswM in
          * the next main function call */

          /* reset channel timer */
          LINSM_CHANNEL_TIMER(id) = 0U;

          /* reset request active, do not expect a confirmation */
          LINSM_CHANNEL_SCHEDULEREQUESTACTIVE(id) = FALSE;

          /* set flag to call BswM_LinSM_CurrentSchedule in next main function
          * call */
          LINSM_CHANNEL_NOTIFYLASTSCHEDULE(id) = TRUE;
        }

        /* leave critical section */
        LINSM_LEAVE_CRITICAL_SECTION();
      }
#if (LINSM_DEV_ERROR_DETECT == STD_ON)
      if (FullCommunication == FALSE)
      {
        LINSM_DET(LINSM_SID_SCHEDULEREQUEST, LINSM_E_NOT_IN_RUN_SCHEDULE);
        retval = E_NOT_OK;
      }
#endif

    }

  }

  DBG_LINSM_SCHEDULEREQUEST_EXIT(retval,network,schedule);
  return retval;
}

#if (TS_MERGED_COMPILE == STD_OFF)
#define LINSM_STOP_SEC_CODE
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */
#endif /* TS_MERGED_COMPILE */

#endif

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/

