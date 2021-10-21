/**
 * \file
 *
 * \brief AUTOSAR CanSM
 *
 * This file contains the implementation of the AUTOSAR
 * module CanSM.
 *
 * \version 3.7.5
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/

#include <CanSM_Trace.h>
#include <TSAutosar.h>              /* EB specific standard types */
#include <ComStack_Types.h>         /* AUTOSAR communication stack types */
#include <TSAtomic_Assign.h>        /* Atomic assignment macros */

#include <CanSM_Cfg.h>              /* DET configuration */
#include <CanSM_ComM.h>             /* Module ComM API */
#include <CanSM_Internal.h>         /* Module private API */
#include <CanSM_TxTimeoutException.h>  /* API declaration */

#include <ComM.h>                   /* Communication Manager types */
#include <ComM_BusSm.h>             /* ComM BusSm API */

#if (CANSM_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>                    /* Development Error Tracer */
#endif

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[internal function declarations]========================*/

#define CANSM_START_SEC_CODE
#include <CanSM_MemMap.h>

/**
 * \brief Subroutine of CanSM_RequestComMode.
 *
 * \param[in] nw Network index
 * \param[in] ComM_Mode CanSM_RequestComMode parameter
 */
STATIC FUNC(void, CANSM_CODE) CanSM_RequestComModeInternal (
  const CanSM_NetworkIndexType nw,
  const ComM_ModeType ComM_Mode
);

#define CANSM_STOP_SEC_CODE
#include <CanSM_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#define CANSM_START_SEC_CODE
#include <CanSM_MemMap.h>

FUNC(Std_ReturnType, CANSM_CODE) CanSM_RequestComMode (
  NetworkHandleType  network,
  ComM_ModeType  ComM_Mode
)
{
  Std_ReturnType retval = E_NOT_OK;

  DBG_CANSM_REQUESTCOMMODE_ENTRY(network,ComM_Mode);

  if ( CanSM_GlobalInit == CANSM_GLOBAL_UNINIT )
  {
#if (CANSM_DEV_ERROR_DETECT == STD_ON)
    /* CANSM184 */
    CANSM_DET(CANSM_SID_REQUESTCOMMODE, CANSM_E_UNINIT);
#endif
  }
  /* CANSM369 */
  else if ( ! (CANSM_NW_HANDLE_IS_VALID(network)) )
  {
#if (CANSM_DEV_ERROR_DETECT == STD_ON)
    /* CANSM183 */
    CANSM_DET(CANSM_SID_REQUESTCOMMODE, CANSM_E_INVALID_NETWORK_HANDLE);
#endif
  }
  else if (
            (ComM_Mode != COMM_NO_COMMUNICATION)
            &&
            (
              (ComM_Mode != COMM_SILENT_COMMUNICATION)
              &&
              (ComM_Mode != COMM_FULL_COMMUNICATION)
            )
          )
  {
#if (CANSM_DEV_ERROR_DETECT == STD_ON)
    CANSM_DET(CANSM_SID_REQUESTCOMMODE, CANSM_E_INVALID_COMM_REQUEST);
#endif /* CANSM_DEV_ERROR_DETECT == STD_ON */
  }
  else
  {
    CanSM_NetworkIndexType nw = CANSM_NW_IDX_FROM_HANDLE(network);
    CanSM_RequestComModeInternal(nw, ComM_Mode);
    retval = E_OK;
  }


  DBG_CANSM_REQUESTCOMMODE_EXIT(retval,network,ComM_Mode);
  return retval;
} /* CanSM_RequestComMode */


#if (CANSM_TX_TIMEOUT_EXCEPTION_API == STD_ON)
FUNC(void, CANSM_CODE) CanSM_TxTimeoutException
(
  NetworkHandleType Channel
)
{
  DBG_CANSM_TXTIMEOUTEXCEPTION_ENTRY(Channel);

#if (CANSM_DEV_ERROR_DETECT == STD_ON)
  if ( CanSM_GlobalInit == CANSM_GLOBAL_UNINIT )
  {
    /* CANSM411 */
    CANSM_DET(CANSM_SID_TXTIMEOUTEXCEPTION, CANSM_E_UNINIT);
  }
  else if ( ! (CANSM_NW_HANDLE_IS_VALID(Channel)) )
  {
    /* CANSM412 */
    CANSM_DET(CANSM_SID_TXTIMEOUTEXCEPTION, CANSM_E_INVALID_NETWORK_HANDLE);
  }
  else
#endif /* CANSM_DEV_ERROR_DETECT == STD_ON */

  {
    CanSM_NetworkIndexType nw = CANSM_NW_IDX_FROM_HANDLE(Channel);
    boolean DoTxTimeoutException = FALSE;

    /* decide if Txtimeout Exception can be done
     *
     * Check and set TransitionOngoing flag in critical sections to avoid side effects with
     * other functions also updating the internal state (SetBaudRate, RequestComMode,
     * MainFunction).
     */
    CANSM_ENTER_CRITICAL_SECTION();
    {
      /* caches if a transition is already ongoing */
      boolean TransitionOngoing = CANSM_NW_INFO_TRANSITION_ONGOING(nw);

      /* CanSM_TxTimeoutException is only accepted in state NO_BUS_OFF */
      if ( (TransitionOngoing == FALSE)
           &&
           (CANSM_S_FUCO_NO_BUS_OFF == CANSM_NW_INFO_INTERNALSTATE(nw))
         )
      {
        /* protect the synchronous transition against other API functions */
        CANSM_NW_INFO_TRANSITION_ONGOING(nw) = TRUE;
        DoTxTimeoutException = TRUE;
      }
    }
    CANSM_EXIT_CRITICAL_SECTION();

    if (DoTxTimeoutException == TRUE)
    {
      /* do synchronous transition now */
      CanSM_DoTransitionSequence (nw, CANSM_T_TX_TIMEOUT_INITIAL, FALSE);
      /* synchronous part of transition done, unlock protection */
      TS_AtomicAssign8(CANSM_NW_INFO_TRANSITION_ONGOING(nw), FALSE);
    }

  }

  DBG_CANSM_TXTIMEOUTEXCEPTION_EXIT(Channel);

} /* CanSM_TxTimeoutException */
#endif /* CANSM_TX_TIMEOUT_EXCEPTION_API == STD_ON */

#define CANSM_STOP_SEC_CODE
#include <CanSM_MemMap.h>

/*==================[internal function definitions]=========================*/

#define CANSM_START_SEC_CODE
#include <CanSM_MemMap.h>

STATIC FUNC(void, CANSM_CODE) CanSM_RequestComModeInternal (
  const CanSM_NetworkIndexType nw,
  const ComM_ModeType ComM_Mode
)
{
  /* by default, don't make a transition */
  CanSM_TransitionType nextTransition = CANSM_T_NONE;

  /* decide if mode request can be done synchron
   *
   * Check and set TransitionOngoing flag in critical sections to avoid side effects with
   * other functions also updating the internal state (SetBaudRate, TxTimeoutException,
   * MainFunction).
   */
  CANSM_ENTER_CRITICAL_SECTION();
  {
    /* caches if a transition is already ongoing */
    boolean TransitionOngoing = CANSM_NW_INFO_TRANSITION_ONGOING(nw);

    if (TransitionOngoing == FALSE)
    {
      /* caches CANSM_NW_INFO_INTERNALSTATE */
      CanSM_InternalStateType internalState = CANSM_NW_INFO_INTERNALSTATE(nw);

      /* Check if the requested mode leads to a transition */
      CanSM_VerifyRequestedComMode( ComM_Mode, internalState, &nextTransition );

      /* update if there is an ongoing transition now */
      if (nextTransition != CANSM_T_NONE)
      {
        /* protect the synchronous transition against other API functions */
        CANSM_NW_INFO_TRANSITION_ONGOING(nw) = TRUE;
      }
    } /* TransitionOngoing == FALSE */

    /* store the requested ComM mode in any case */
    CANSM_NW_INFO_REQCOMMODE(nw) = ComM_Mode;
  }
  CANSM_EXIT_CRITICAL_SECTION();

  if (nextTransition != CANSM_T_NONE)
  {
    /* do synchronous transition now */
    CanSM_DoTransitionSequence (nw, nextTransition, FALSE);
    /* synchronous part of transition done, unlock protection */
    TS_AtomicAssign8(CANSM_NW_INFO_TRANSITION_ONGOING(nw), FALSE);
  }

}

#define CANSM_STOP_SEC_CODE
#include <CanSM_MemMap.h>

/*==================[end of file]===========================================*/

