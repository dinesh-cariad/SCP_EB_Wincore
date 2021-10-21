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

FUNC(void, LINSM_CODE) LinSM_Init
(
  P2CONST(LinSM_ConfigType, AUTOMATIC, LINSM_APPL_CONST) ConfigPtr
)
{
  LinSM_ChannelIdType id;

  /* ConfigPtr is ignored */
  TS_PARAM_UNUSED(ConfigPtr); /* !LINKSTO LinSM.ASR40.LINSM0218_EB, 1 */

  DBG_LINSM_INIT_ENTRY(ConfigPtr);

  /* enter critical section */
  LINSM_ENTER_CRITICAL_SECTION(); /* protect channel state and timer */

  for (id = 0U; id < LINSM_NUM_CHANNELS; id++)
  {
    /* stop timer */
    LINSM_CHANNEL_TIMER(id) = 0U;
    /* stop repetition counter */
    LINSM_CHANNEL_REPETITION_TIME(id) = 0U;
    /* reset channel state */
    DBG_LINSM_CHANNELDATA_STATE_GRP(id, (LINSM_CHANNEL_STATE(id)), (LINSM_NO_COM));
    LINSM_CHANNEL_STATE(id) = LINSM_NO_COM;
#if (LINSM_MASTER_NODES_ENABLED == STD_ON)
    LINSM_CHANNEL_LASTSCHEDULE(id) = 0U;
    LINSM_CHANNEL_NOTIFYLASTSCHEDULE(id) = FALSE;
    LINSM_CHANNEL_SCHEDULEREQUESTACTIVE(id) = FALSE;
#endif
    LINSM_CHANNEL_STORED_COM(id) = NOTHING_STORED;
#if (LINSM_SLAVE_NODES_ENABLED == STD_ON)
    LINSM_CHANNEL_SILENCE_TIME(id) = LINSM_INVALID_SILENCE_AFTER_WAKEUP;
    LINSM_CHANNEL_SLAVE_WAITNOCOMREQ(id) = FALSE;
    LINSM_CHANNEL_SLAVE_WAITSLEEPCONF(id) = TRUE;
    LINSM_CHANNEL_SLAVE_GOTOSLEEPINPROGRESS(id) = FALSE;
#endif
  }

  DBG_LINSM_GLOBALSTATE((LinSM_GlobalState),(LINSM_GLOBAL_INIT))
  LinSM_GlobalState = LINSM_GLOBAL_INIT;

  /* leave critical section */
  LINSM_LEAVE_CRITICAL_SECTION();


  DBG_LINSM_INIT_EXIT(ConfigPtr);
}

#if (TS_MERGED_COMPILE == STD_OFF)
#define LINSM_STOP_SEC_CODE
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */
#endif /* TS_MERGED_COMPILE */

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/

