/**
 * \file
 *
 * \brief AUTOSAR SoAd
 *
 * This file contains the implementation of the AUTOSAR
 * module SoAd.
 *
 * \version 1.8.16
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*
 * Misra-C:2012 Deviations:
 *
 * MISRAC2012-9) Deviated Rule: 11.1 (required)
 * Conversion shall not be performed between a pointer to a function and any type
 * other than an integral type.
 *
 * Reason:
 * This violation only occurs in a specific testing scenario only where these macros are used for
 * function trace recoding. Thus this is not contained in production code.
 *
 */

 /* Note: This file was generated from Uml diagram with UmlToSM. Do not change this file! */

/*==================[inclusions]============================================*/
#include <SoAd_04_TcpSM_Int.h> /* Unit public header file. */

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
#include <Det.h> /* Det API */
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[internal function declarations]========================*/
#if(TS_MERGED_COMPILE == STD_OFF)
#define SOAD_START_SEC_CODE
#include <SoAd_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

/** \brief Transition function declaration */

/* Trigger: Connecting */

STATIC FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_Trans_Connecting_CLOSED_CONNECTING
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

STATIC FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_Trans_Connecting_RESTART_CONNECTING
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

/* Trigger: Closed */

STATIC FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_Trans_Closed_CLOSING_CLOSED
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

STATIC FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_Trans_Closed_ONLINE_RESTART
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

STATIC FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_Trans_Closed_CONNECTING_RESTART
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

STATIC FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_Trans_Closed_SHUTDOWN_RESTART
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

/* Trigger: LoClosing */

STATIC FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_Trans_LoClosing_ONLINE_SHUTDOWN
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

STATIC FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_Trans_LoClosing_CONNECTING_SHUTDOWN
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

/* Trigger: UpClosing */

STATIC FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_Trans_UpClosing_ONLINE_CLOSING
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

STATIC FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_Trans_UpClosing_CONNECTING_CLOSING
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

STATIC FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_Trans_UpClosing_RESTART_CLOSED
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

STATIC FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_Trans_UpClosing_SHUTDOWN_CLOSING
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

/* Trigger: Connected */

STATIC FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_Trans_Connected_CONNECTING_ONLINE
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/* === === [State machine trigger functions] === === */

TS_MOD_PRIV_DEFN FUNC(boolean, SOAD_CODE) SoAd_Int_04_TcpSM_Connecting
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  boolean TransPerformed = FALSE;
  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_04_TCPSM_CONNECTING_ENTRY(SoConPtr, SoConStatePtr);

  switch(SoConStatePtr->SocketState)
  {
    case SOAD_SOCKETSTATE_CLOSED:
    {
      SOAD_04_TCPSM_TRANSITION_LOCK(SoConPtr, SoConStatePtr);
      SoAd_Int_04_TcpSM_Trans_Connecting_CLOSED_CONNECTING(SoConPtr, SoConStatePtr);
      TransPerformed = TRUE;
      SOAD_04_TCPSM_TRANSITION_UNLOCK(SoConPtr, SoConStatePtr);
      break;
    }
    case SOAD_SOCKETSTATE_RESTART:
    {
      SOAD_04_TCPSM_TRANSITION_LOCK(SoConPtr, SoConStatePtr);
      SoAd_Int_04_TcpSM_Trans_Connecting_RESTART_CONNECTING(SoConPtr, SoConStatePtr);
      TransPerformed = TRUE;
      SOAD_04_TCPSM_TRANSITION_UNLOCK(SoConPtr, SoConStatePtr);
      break;
    }
    case SOAD_SOCKETSTATE_CLOSING: /* fall through */
    case SOAD_SOCKETSTATE_ONLINE: /* fall through */
    case SOAD_SOCKETSTATE_CONNECTING: /* fall through */
    case SOAD_SOCKETSTATE_SHUTDOWN:
    {
      /* No action for that trigger */
      break;
    }
    /* CHECK: NOPARSE */
    default:  /* should not happen */
    {
      SOAD_UNREACHABLE_CODE_ASSERT(SOAD_MAINFUNCTION_SVCID);
      break;
    }
    /* CHECK: PARSE */
  }

  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_04_TCPSM_CONNECTING_EXIT(TransPerformed, SoConPtr, SoConStatePtr);

  return TransPerformed;
}

TS_MOD_PRIV_DEFN FUNC(boolean, SOAD_CODE) SoAd_Int_04_TcpSM_Closed
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  boolean TransPerformed = FALSE;
  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_04_TCPSM_CLOSED_ENTRY(SoConPtr, SoConStatePtr);

  switch(SoConStatePtr->SocketState)
  {
    case SOAD_SOCKETSTATE_CLOSING:
    {
      SOAD_04_TCPSM_TRANSITION_LOCK(SoConPtr, SoConStatePtr);
      SoAd_Int_04_TcpSM_Trans_Closed_CLOSING_CLOSED(SoConPtr, SoConStatePtr);
      TransPerformed = TRUE;
      SOAD_04_TCPSM_TRANSITION_UNLOCK(SoConPtr, SoConStatePtr);
      break;
    }
    case SOAD_SOCKETSTATE_ONLINE:
    {
      SOAD_04_TCPSM_TRANSITION_LOCK(SoConPtr, SoConStatePtr);
      SoAd_Int_04_TcpSM_Trans_Closed_ONLINE_RESTART(SoConPtr, SoConStatePtr);
      TransPerformed = TRUE;
      SOAD_04_TCPSM_TRANSITION_UNLOCK(SoConPtr, SoConStatePtr);
      break;
    }
    case SOAD_SOCKETSTATE_CONNECTING:
    {
      SOAD_04_TCPSM_TRANSITION_LOCK(SoConPtr, SoConStatePtr);
      SoAd_Int_04_TcpSM_Trans_Closed_CONNECTING_RESTART(SoConPtr, SoConStatePtr);
      TransPerformed = TRUE;
      SOAD_04_TCPSM_TRANSITION_UNLOCK(SoConPtr, SoConStatePtr);
      break;
    }
    case SOAD_SOCKETSTATE_SHUTDOWN:
    {
      SOAD_04_TCPSM_TRANSITION_LOCK(SoConPtr, SoConStatePtr);
      SoAd_Int_04_TcpSM_Trans_Closed_SHUTDOWN_RESTART(SoConPtr, SoConStatePtr);
      TransPerformed = TRUE;
      SOAD_04_TCPSM_TRANSITION_UNLOCK(SoConPtr, SoConStatePtr);
      break;
    }
    case SOAD_SOCKETSTATE_CLOSED: /* fall through */
    case SOAD_SOCKETSTATE_RESTART:
    {
      /* No action for that trigger */
      break;
    }
    /* CHECK: NOPARSE */
    default:  /* should not happen */
    {
      SOAD_UNREACHABLE_CODE_ASSERT(SOAD_MAINFUNCTION_SVCID);
      break;
    }
    /* CHECK: PARSE */
  }

  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_04_TCPSM_CLOSED_EXIT(TransPerformed, SoConPtr, SoConStatePtr);

  return TransPerformed;
}

TS_MOD_PRIV_DEFN FUNC(boolean, SOAD_CODE) SoAd_Int_04_TcpSM_LoClosing
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  boolean TransPerformed = FALSE;
  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_04_TCPSM_LOCLOSING_ENTRY(SoConPtr, SoConStatePtr);

  switch(SoConStatePtr->SocketState)
  {
    case SOAD_SOCKETSTATE_ONLINE:
    {
      SOAD_04_TCPSM_TRANSITION_LOCK(SoConPtr, SoConStatePtr);
      SoAd_Int_04_TcpSM_Trans_LoClosing_ONLINE_SHUTDOWN(SoConPtr, SoConStatePtr);
      TransPerformed = TRUE;
      SOAD_04_TCPSM_TRANSITION_UNLOCK(SoConPtr, SoConStatePtr);
      break;
    }
    case SOAD_SOCKETSTATE_CONNECTING:
    {
      SOAD_04_TCPSM_TRANSITION_LOCK(SoConPtr, SoConStatePtr);
      SoAd_Int_04_TcpSM_Trans_LoClosing_CONNECTING_SHUTDOWN(SoConPtr, SoConStatePtr);
      TransPerformed = TRUE;
      SOAD_04_TCPSM_TRANSITION_UNLOCK(SoConPtr, SoConStatePtr);
      break;
    }
    case SOAD_SOCKETSTATE_CLOSED: /* fall through */
    case SOAD_SOCKETSTATE_CLOSING: /* fall through */
    case SOAD_SOCKETSTATE_RESTART: /* fall through */
    case SOAD_SOCKETSTATE_SHUTDOWN:
    {
      /* No action for that trigger */
      break;
    }
    /* CHECK: NOPARSE */
    default:  /* should not happen */
    {
      SOAD_UNREACHABLE_CODE_ASSERT(SOAD_MAINFUNCTION_SVCID);
      break;
    }
    /* CHECK: PARSE */
  }

  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_04_TCPSM_LOCLOSING_EXIT(TransPerformed, SoConPtr, SoConStatePtr);

  return TransPerformed;
}

TS_MOD_PRIV_DEFN FUNC(boolean, SOAD_CODE) SoAd_Int_04_TcpSM_UpClosing
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  boolean TransPerformed = FALSE;
  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_04_TCPSM_UPCLOSING_ENTRY(SoConPtr, SoConStatePtr);

  switch(SoConStatePtr->SocketState)
  {
    case SOAD_SOCKETSTATE_ONLINE:
    {
      SOAD_04_TCPSM_TRANSITION_LOCK(SoConPtr, SoConStatePtr);
      SoAd_Int_04_TcpSM_Trans_UpClosing_ONLINE_CLOSING(SoConPtr, SoConStatePtr);
      TransPerformed = TRUE;
      SOAD_04_TCPSM_TRANSITION_UNLOCK(SoConPtr, SoConStatePtr);
      break;
    }
    case SOAD_SOCKETSTATE_CONNECTING:
    {
      SOAD_04_TCPSM_TRANSITION_LOCK(SoConPtr, SoConStatePtr);
      SoAd_Int_04_TcpSM_Trans_UpClosing_CONNECTING_CLOSING(SoConPtr, SoConStatePtr);
      TransPerformed = TRUE;
      SOAD_04_TCPSM_TRANSITION_UNLOCK(SoConPtr, SoConStatePtr);
      break;
    }
    case SOAD_SOCKETSTATE_RESTART:
    {
      SOAD_04_TCPSM_TRANSITION_LOCK(SoConPtr, SoConStatePtr);
      SoAd_Int_04_TcpSM_Trans_UpClosing_RESTART_CLOSED(SoConPtr, SoConStatePtr);
      TransPerformed = TRUE;
      SOAD_04_TCPSM_TRANSITION_UNLOCK(SoConPtr, SoConStatePtr);
      break;
    }
    case SOAD_SOCKETSTATE_SHUTDOWN:
    {
      SOAD_04_TCPSM_TRANSITION_LOCK(SoConPtr, SoConStatePtr);
      SoAd_Int_04_TcpSM_Trans_UpClosing_SHUTDOWN_CLOSING(SoConPtr, SoConStatePtr);
      TransPerformed = TRUE;
      SOAD_04_TCPSM_TRANSITION_UNLOCK(SoConPtr, SoConStatePtr);
      break;
    }
    case SOAD_SOCKETSTATE_CLOSED: /* fall through */
    case SOAD_SOCKETSTATE_CLOSING:
    {
      /* No action for that trigger */
      break;
    }
    /* CHECK: NOPARSE */
    default:  /* should not happen */
    {
      SOAD_UNREACHABLE_CODE_ASSERT(SOAD_MAINFUNCTION_SVCID);
      break;
    }
    /* CHECK: PARSE */
  }

  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_04_TCPSM_UPCLOSING_EXIT(TransPerformed, SoConPtr, SoConStatePtr);

  return TransPerformed;
}

TS_MOD_PRIV_DEFN FUNC(boolean, SOAD_CODE) SoAd_Int_04_TcpSM_Connected
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  boolean TransPerformed = FALSE;
  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_04_TCPSM_CONNECTED_ENTRY(SoConPtr, SoConStatePtr);

  switch(SoConStatePtr->SocketState)
  {
    case SOAD_SOCKETSTATE_CONNECTING:
    {
      SOAD_04_TCPSM_TRANSITION_LOCK(SoConPtr, SoConStatePtr);
      SoAd_Int_04_TcpSM_Trans_Connected_CONNECTING_ONLINE(SoConPtr, SoConStatePtr);
      TransPerformed = TRUE;
      SOAD_04_TCPSM_TRANSITION_UNLOCK(SoConPtr, SoConStatePtr);
      break;
    }
    case SOAD_SOCKETSTATE_CLOSED: /* fall through */
    case SOAD_SOCKETSTATE_CLOSING: /* fall through */
    case SOAD_SOCKETSTATE_ONLINE: /* fall through */
    case SOAD_SOCKETSTATE_RESTART: /* fall through */
    case SOAD_SOCKETSTATE_SHUTDOWN:
    {
      /* No action for that trigger */
      break;
    }
    /* CHECK: NOPARSE */
    default:  /* should not happen */
    {
      SOAD_UNREACHABLE_CODE_ASSERT(SOAD_MAINFUNCTION_SVCID);
      break;
    }
    /* CHECK: PARSE */
  }

  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_04_TCPSM_CONNECTED_EXIT(TransPerformed, SoConPtr, SoConStatePtr);

  return TransPerformed;
}

/*==================[internal function definitions]=========================*/

/* === === [State transition functions] === === */

STATIC FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_Trans_Connecting_CLOSED_CONNECTING
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  /* Exit code. */
  /* None */

  /* Parent state exit code. */
  /* None */

  /* Transition code. */
  /* None */

  /* Parent state entry state. */
  SoAd_Int_04_TcpSM_State_RECONNECT_Entry_SoConModeChg(SoConPtr, SoConStatePtr);

  /* Entry code. */
  SoAd_Int_04_TcpSM_State_CONNECTING_Entry_Connecting(SoConPtr, SoConStatePtr);

  SoConStatePtr->SocketState = SOAD_SOCKETSTATE_CONNECTING;
}


STATIC FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_Trans_Connecting_RESTART_CONNECTING
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  /* Exit code. */
  /* None */

  /* Parent state exit code. */
  /* None */

  /* Transition code. */
  /* None */

  /* Parent state entry state. */
  /* None */

  /* Entry code. */
  SoAd_Int_04_TcpSM_State_CONNECTING_Entry_Connecting(SoConPtr, SoConStatePtr);

  SoConStatePtr->SocketState = SOAD_SOCKETSTATE_CONNECTING;
}


STATIC FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_Trans_Closed_CLOSING_CLOSED
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  /* Exit code. */
  /* None */

  /* Parent state exit code. */
  /* None */

  /* Transition code. */
  /* None */

  /* Parent state entry state. */
  /* None */

  /* Entry code. */
  SoAd_Int_04_TcpSM_State_CLOSED_Entry_Cleanup(SoConPtr, SoConStatePtr);

  SoConStatePtr->SocketState = SOAD_SOCKETSTATE_CLOSED;
}


STATIC FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_Trans_Closed_ONLINE_RESTART
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  /* Exit code. */
  SoAd_Int_04_TcpSM_State_ONLINE_Exit_CloseRxTx(SoConPtr, SoConStatePtr);

  /* Parent state exit code. */
  /* None */

  /* Transition code. */
  /* None */

  /* Parent state entry state. */
  SoAd_Int_04_TcpSM_State_RECONNECT_Entry_SoConModeChg(SoConPtr, SoConStatePtr);

  /* Entry code. */
  SoAd_Int_04_TcpSM_State_RESTART_Entry_Reset(SoConPtr, SoConStatePtr);

  SoConStatePtr->SocketState = SOAD_SOCKETSTATE_RESTART;
}


STATIC FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_Trans_Closed_CONNECTING_RESTART
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  /* Exit code. */
  /* None */

  /* Parent state exit code. */
  /* None */

  /* Transition code. */
  /* None */

  /* Parent state entry state. */
  /* None */

  /* Entry code. */
  SoAd_Int_04_TcpSM_State_RESTART_Entry_Reset(SoConPtr, SoConStatePtr);

  SoConStatePtr->SocketState = SOAD_SOCKETSTATE_RESTART;
}


STATIC FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_Trans_Closed_SHUTDOWN_RESTART
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  /* Exit code. */
  /* None */

  /* Parent state exit code. */
  /* None */

  /* Transition code. */
  /* None */

  /* Parent state entry state. */
  /* None */

  /* Entry code. */
  SoAd_Int_04_TcpSM_State_RESTART_Entry_Reset(SoConPtr, SoConStatePtr);

  SoConStatePtr->SocketState = SOAD_SOCKETSTATE_RESTART;
}


STATIC FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_Trans_LoClosing_ONLINE_SHUTDOWN
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  /* Exit code. */
  SoAd_Int_04_TcpSM_State_ONLINE_Exit_CloseRxTx(SoConPtr, SoConStatePtr);

  /* Parent state exit code. */
  /* None */

  /* Transition code. */
  /* None */

  /* Parent state entry state. */
  SoAd_Int_04_TcpSM_State_RECONNECT_Entry_SoConModeChg(SoConPtr, SoConStatePtr);

  /* Entry code. */
  SoAd_Int_04_TcpSM_State_SHUTDOWN_Entry_ShutDown(SoConPtr, SoConStatePtr);

  SoConStatePtr->SocketState = SOAD_SOCKETSTATE_SHUTDOWN;
}


STATIC FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_Trans_LoClosing_CONNECTING_SHUTDOWN
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  /* Exit code. */
  /* None */

  /* Parent state exit code. */
  /* None */

  /* Transition code. */
  /* None */

  /* Parent state entry state. */
  /* None */

  /* Entry code. */
  SoAd_Int_04_TcpSM_State_SHUTDOWN_Entry_ShutDown(SoConPtr, SoConStatePtr);

  SoConStatePtr->SocketState = SOAD_SOCKETSTATE_SHUTDOWN;
}


STATIC FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_Trans_UpClosing_ONLINE_CLOSING
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  /* Exit code. */
  SoAd_Int_04_TcpSM_State_ONLINE_Exit_CloseRxTx(SoConPtr, SoConStatePtr);

  /* Parent state exit code. */
  /* None */

  /* Transition code. */
  /* None */

  /* Parent state entry state. */
  SoAd_Int_04_TcpSM_State_OFFLINE_Entry_SoConModeChg(SoConPtr, SoConStatePtr);

  /* Entry code. */
  SoAd_Int_04_TcpSM_State_CLOSING_Entry_Closing(SoConPtr, SoConStatePtr);

  SoConStatePtr->SocketState = SOAD_SOCKETSTATE_CLOSING;
}


STATIC FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_Trans_UpClosing_CONNECTING_CLOSING
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  /* Exit code. */
  /* None */

  /* Parent state exit code. */
  /* None */

  /* Transition code. */
  /* None */

  /* Parent state entry state. */
  SoAd_Int_04_TcpSM_State_OFFLINE_Entry_SoConModeChg(SoConPtr, SoConStatePtr);

  /* Entry code. */
  SoAd_Int_04_TcpSM_State_CLOSING_Entry_Closing(SoConPtr, SoConStatePtr);

  SoConStatePtr->SocketState = SOAD_SOCKETSTATE_CLOSING;
}


STATIC FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_Trans_UpClosing_RESTART_CLOSED
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  /* Exit code. */
  /* None */

  /* Parent state exit code. */
  /* None */

  /* Transition code. */
  /* None */

  /* Parent state entry state. */
  SoAd_Int_04_TcpSM_State_OFFLINE_Entry_SoConModeChg(SoConPtr, SoConStatePtr);

  /* Entry code. */
  SoAd_Int_04_TcpSM_State_CLOSED_Entry_Cleanup(SoConPtr, SoConStatePtr);

  SoConStatePtr->SocketState = SOAD_SOCKETSTATE_CLOSED;
}


STATIC FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_Trans_UpClosing_SHUTDOWN_CLOSING
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  /* Exit code. */
  /* None */

  /* Parent state exit code. */
  /* None */

  /* Transition code. */
  /* None */

  /* Parent state entry state. */
  SoAd_Int_04_TcpSM_State_OFFLINE_Entry_SoConModeChg(SoConPtr, SoConStatePtr);

  /* Entry code. */
  SoAd_Int_04_TcpSM_State_CLOSING_Entry_Closing(SoConPtr, SoConStatePtr);

  SoConStatePtr->SocketState = SOAD_SOCKETSTATE_CLOSING;
}


STATIC FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_Trans_Connected_CONNECTING_ONLINE
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  /* Exit code. */
  /* None */

  /* Parent state exit code. */
  /* None */

  /* Transition code. */
  /* None */

  /* Parent state entry state. */
  /* None */

  /* Entry code. */
  SoAd_Int_04_TcpSM_State_ONLINE_Entry_SoConModeChg(SoConPtr, SoConStatePtr);
  SoAd_Int_04_TcpSM_State_ONLINEentry_Online(SoConPtr, SoConStatePtr);

  SoConStatePtr->SocketState = SOAD_SOCKETSTATE_ONLINE;

}


#if(TS_MERGED_COMPILE == STD_OFF)
#define SOAD_STOP_SEC_CODE
#include <SoAd_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */
/*==================[end of file]===========================================*/
