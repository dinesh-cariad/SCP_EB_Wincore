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

#if (LINSM_SLAVE_NODES_ENABLED == STD_ON)
/* !LINKSTO LinSM.ASRR20-11.SWS_LinSM_00243,1 */

#include <BswM_LinSM.h>/* !LINKSTO LinSM.ASR40.LINSM201_EB, 1 */
#include <ComM.h>

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

/* !LINKSTO LinSM.ASRR20-11.SWS_LinSM_91000_EB,1 */
FUNC(void, LINSM_CODE) LinSM_GotoSleepIndication
(
  NetworkHandleType Channel
)
{
  LinSM_ChannelIdType id;

  DBG_LINSM_GOTOSLEEPINDICATION_ENTRY(Channel);

#if (LINSM_DEV_ERROR_DETECT == STD_ON)
  if ( LinSM_GlobalState == LINSM_GLOBAL_UNINIT )
  {
    /* !LINKSTO LinSM.ASRR20-11.SWS_LinSM_00240,1 */
    LINSM_DET(LINSM_SID_GOTOSLEEPINDICATION, LINSM_E_UNINIT);
  }
  else
  {
    if ( ! (LINSM_COMM_NW_IS_VALID(Channel)) )
    {
      /* !LINKSTO LinSM.ASRR20-11.SWS_LinSM_00239,1 */
      LINSM_DET(LINSM_SID_GOTOSLEEPINDICATION, LINSM_E_NONEXISTENT_NETWORK);
    }
    else
#endif
    {
      uint8 currentState;

      id = LINSM_COMM_NW_TO_ID(Channel);

      LINSM_ENTER_CRITICAL_SECTION(); /* protect channel state and timer */

      currentState = LINSM_CHANNEL_STATE(id);

      LINSM_LEAVE_CRITICAL_SECTION();

      /* !LINKSTO LinSM.ASRR20-11.SWS_LinSM_00232,1 */
      if ( currentState == LINSM_FULL_COM )
      {
        /* !LINKSTO LinSM.ASRR20-11.SWS_LinSM_00231,1 */
        LinSM_GotoSleep(id);

      }
    }
#if (LINSM_DEV_ERROR_DETECT == STD_ON)
  }
#endif

  DBG_LINSM_GOTOSLEEPINDICATION_EXIT(Channel);
}

#if (TS_MERGED_COMPILE == STD_OFF)
#define LINSM_STOP_SEC_CODE
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */
#endif /* TS_MERGED_COMPILE */

#endif /* (LINSM_SLAVE_NODES_ENABLED == STD_ON) */

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/

