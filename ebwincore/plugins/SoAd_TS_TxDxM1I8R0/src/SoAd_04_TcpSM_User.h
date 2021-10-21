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
/* DOTO: Change this file to SoAd_04_TcpSM_User.h or copy past relevant parts
         to your exisitng file.
         Remove this comment afterwards! */
#ifndef SOAD_04_TCPSM_USER_H
#define SOAD_04_TCPSM_USER_H

/*==================[inclusions]============================================*/

#include <TSAutosar.h> /* EB specific standard types. */
#include <ComStack_Types.h>
#include <SoAd_Trace.h>
#include <SoAd_02_SocketCon_Int.h> /* Common socket handling relevant APIs. */
#include <SoAd.h> /* Module public header file. */
#include <SoAd_Int.h> /* Module internal header file. */

/*==================[macros]================================================*/
/* DOTO: Copy all DBG macros to SoAd_Trace.h
   Remove this comment afterwards! */

#ifndef DBG_SOAD_INT_04_TCPSM_CONNECTING_ENTRY
/** \brief Entry point of function SoAd_Int_04_TcpSM_Connecting() */
#define DBG_SOAD_INT_04_TCPSM_CONNECTING_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_04_TCPSM_CONNECTING_EXIT
/** \brief Exit point of function SoAd_Int_04_TcpSM_Connecting() */
#define DBG_SOAD_INT_04_TCPSM_CONNECTING_EXIT(a,b,c)
#endif

#ifndef DBG_SOAD_INT_04_TCPSM_CLOSED_ENTRY
/** \brief Entry point of function SoAd_Int_04_TcpSM_Closed() */
#define DBG_SOAD_INT_04_TCPSM_CLOSED_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_04_TCPSM_CLOSED_EXIT
/** \brief Exit point of function SoAd_Int_04_TcpSM_Closed() */
#define DBG_SOAD_INT_04_TCPSM_CLOSED_EXIT(a,b,c)
#endif

#ifndef DBG_SOAD_INT_04_TCPSM_LOCLOSING_ENTRY
/** \brief Entry point of function SoAd_Int_04_TcpSM_LoClosing() */
#define DBG_SOAD_INT_04_TCPSM_LOCLOSING_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_04_TCPSM_LOCLOSING_EXIT
/** \brief Exit point of function SoAd_Int_04_TcpSM_LoClosing() */
#define DBG_SOAD_INT_04_TCPSM_LOCLOSING_EXIT(a,b,c)
#endif

#ifndef DBG_SOAD_INT_04_TCPSM_UPCLOSING_ENTRY
/** \brief Entry point of function SoAd_Int_04_TcpSM_UpClosing() */
#define DBG_SOAD_INT_04_TCPSM_UPCLOSING_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_04_TCPSM_UPCLOSING_EXIT
/** \brief Exit point of function SoAd_Int_04_TcpSM_UpClosing() */
#define DBG_SOAD_INT_04_TCPSM_UPCLOSING_EXIT(a,b,c)
#endif

#ifndef DBG_SOAD_INT_04_TCPSM_CONNECTED_ENTRY
/** \brief Entry point of function SoAd_Int_04_TcpSM_Connected() */
#define DBG_SOAD_INT_04_TCPSM_CONNECTED_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_04_TCPSM_CONNECTED_EXIT
/** \brief Exit point of function SoAd_Int_04_TcpSM_Connected() */
#define DBG_SOAD_INT_04_TCPSM_CONNECTED_EXIT(a,b,c)
#endif

#ifndef DBG_SOAD_INT_04_TCPSM_STATE_CLOSED_ENTRY_CLEANUP_ENTRY
/** \brief Entry point of function SoAd_Int_04_TcpSM_State_CLOSED_Entry_Cleanup() */
#define DBG_SOAD_INT_04_TCPSM_STATE_CLOSED_ENTRY_CLEANUP_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_04_TCPSM_STATE_CLOSED_ENTRY_CLEANUP_EXIT
/** \brief Exit point of function SoAd_Int_04_TcpSM_State_CLOSED_Entry_Cleanup() */
#define DBG_SOAD_INT_04_TCPSM_STATE_CLOSED_ENTRY_CLEANUP_EXIT(a,b)
#endif

#ifndef DBG_SOAD_INT_04_TCPSM_STATE_CLOSING_ENTRY_CLOSING_ENTRY
/** \brief Entry point of function SoAd_Int_04_TcpSM_State_CLOSING_Entry_Closing() */
#define DBG_SOAD_INT_04_TCPSM_STATE_CLOSING_ENTRY_CLOSING_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_04_TCPSM_STATE_CLOSING_ENTRY_CLOSING_EXIT
/** \brief Exit point of function SoAd_Int_04_TcpSM_State_CLOSING_Entry_Closing() */
#define DBG_SOAD_INT_04_TCPSM_STATE_CLOSING_ENTRY_CLOSING_EXIT(a,b)
#endif

#ifndef DBG_SOAD_INT_04_TCPSM_STATE_OFFLINE_ENTRY_SOCONMODECHG_ENTRY
/** \brief Entry point of function SoAd_Int_04_TcpSM_State_OFFLINE_Entry_SoConModeChg() */
#define DBG_SOAD_INT_04_TCPSM_STATE_OFFLINE_ENTRY_SOCONMODECHG_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_04_TCPSM_STATE_OFFLINE_ENTRY_SOCONMODECHG_EXIT
/** \brief Exit point of function SoAd_Int_04_TcpSM_State_OFFLINE_Entry_SoConModeChg() */
#define DBG_SOAD_INT_04_TCPSM_STATE_OFFLINE_ENTRY_SOCONMODECHG_EXIT(a,b)
#endif

#ifndef DBG_SOAD_INT_04_TCPSM_STATE_ONLINE_ENTRY_SOCONMODECHG_ENTRY
/** \brief Entry point of function SoAd_Int_04_TcpSM_State_ONLINE_Entry_SoConModeChg() */
#define DBG_SOAD_INT_04_TCPSM_STATE_ONLINE_ENTRY_SOCONMODECHG_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_04_TCPSM_STATE_ONLINE_ENTRY_SOCONMODECHG_EXIT
/** \brief Exit point of function SoAd_Int_04_TcpSM_State_ONLINE_Entry_SoConModeChg() */
#define DBG_SOAD_INT_04_TCPSM_STATE_ONLINE_ENTRY_SOCONMODECHG_EXIT(a,b)
#endif

#ifndef DBG_SOAD_INT_04_TCPSM_STATE_ONLINEENTRY_ONLINE_ENTRY
/** \brief Entry point of function SoAd_Int_04_TcpSM_State_ONLINEentry_Online() */
#define DBG_SOAD_INT_04_TCPSM_STATE_ONLINEENTRY_ONLINE_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_04_TCPSM_STATE_ONLINEENTRY_ONLINE_EXIT
/** \brief Exit point of function SoAd_Int_04_TcpSM_State_ONLINEentry_Online() */
#define DBG_SOAD_INT_04_TCPSM_STATE_ONLINEENTRY_ONLINE_EXIT(a,b)
#endif

#ifndef DBG_SOAD_INT_04_TCPSM_STATE_CONNECTING_ENTRY_CONNECTING_ENTRY
/** \brief Entry point of function SoAd_Int_04_TcpSM_State_CONNECTING_Entry_Connecting() */
#define DBG_SOAD_INT_04_TCPSM_STATE_CONNECTING_ENTRY_CONNECTING_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_04_TCPSM_STATE_CONNECTING_ENTRY_CONNECTING_EXIT
/** \brief Exit point of function SoAd_Int_04_TcpSM_State_CONNECTING_Entry_Connecting() */
#define DBG_SOAD_INT_04_TCPSM_STATE_CONNECTING_ENTRY_CONNECTING_EXIT(a,b)
#endif

#ifndef DBG_SOAD_INT_04_TCPSM_STATE_RESTART_ENTRY_RESET_ENTRY
/** \brief Entry point of function SoAd_Int_04_TcpSM_State_RESTART_Entry_Reset() */
#define DBG_SOAD_INT_04_TCPSM_STATE_RESTART_ENTRY_RESET_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_04_TCPSM_STATE_RESTART_ENTRY_RESET_EXIT
/** \brief Exit point of function SoAd_Int_04_TcpSM_State_RESTART_Entry_Reset() */
#define DBG_SOAD_INT_04_TCPSM_STATE_RESTART_ENTRY_RESET_EXIT(a,b)
#endif

#ifndef DBG_SOAD_INT_04_TCPSM_STATE_SHUTDOWN_ENTRY_SHUTDOWN_ENTRY
/** \brief Entry point of function SoAd_Int_04_TcpSM_State_SHUTDOWN_Entry_ShutDown() */
#define DBG_SOAD_INT_04_TCPSM_STATE_SHUTDOWN_ENTRY_SHUTDOWN_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_04_TCPSM_STATE_SHUTDOWN_ENTRY_SHUTDOWN_EXIT
/** \brief Exit point of function SoAd_Int_04_TcpSM_State_SHUTDOWN_Entry_ShutDown() */
#define DBG_SOAD_INT_04_TCPSM_STATE_SHUTDOWN_ENTRY_SHUTDOWN_EXIT(a,b)
#endif

#ifndef DBG_SOAD_INT_04_TCPSM_STATE_RECONNECT_ENTRY_SOCONMODECHG_ENTRY
/** \brief Entry point of function SoAd_Int_04_TcpSM_State_RECONNECT_Entry_SoConModeChg() */
#define DBG_SOAD_INT_04_TCPSM_STATE_RECONNECT_ENTRY_SOCONMODECHG_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_04_TCPSM_STATE_RECONNECT_ENTRY_SOCONMODECHG_EXIT
/** \brief Exit point of function SoAd_Int_04_TcpSM_State_RECONNECT_Entry_SoConModeChg() */
#define DBG_SOAD_INT_04_TCPSM_STATE_RECONNECT_ENTRY_SOCONMODECHG_EXIT(a,b)
#endif

#ifndef DBG_SOAD_INT_04_TCPSM_STATE_ONLINE_EXIT_CLOSERXTX_ENTRY
/** \brief Entry point of function SoAd_Int_04_TcpSM_State_ONLINE_Exit_CloseRxTx() */
#define DBG_SOAD_INT_04_TCPSM_STATE_ONLINE_EXIT_CLOSERXTX_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_04_TCPSM_STATE_ONLINE_EXIT_CLOSERXTX_EXIT
/** \brief Exit point of function SoAd_Int_04_TcpSM_State_ONLINE_Exit_CloseRxTx() */
#define DBG_SOAD_INT_04_TCPSM_STATE_ONLINE_EXIT_CLOSERXTX_EXIT(a,b)
#endif

/** \brief Transition lock macros
 *
 * These macros can be used to lock the transition execution for other functions.
 * If required, add a function which locks a variable. The state machine will call the
 * LOCK macro at the beginning of the transition and the UNLOCK macro after the transition.
 *
 * \warning It need to be ensured that no memory ordering is possible. Otherwise the lock has no
 *          effect.
 */
#if (defined SOAD_04_TCPSM_TRANSITION_LOCK)
#error SOAD_04_TCPSM_TRANSITION_LOCK is already defined
#endif
#define SOAD_04_TCPSM_TRANSITION_LOCK(SoConPtr, SoConStatePtr)

#if (defined SOAD_04_TCPSM_TRANSITION_UNLOCK)
#error SOAD_04_TCPSM_TRANSITION_UNLOCK is already defined
#endif
#define SOAD_04_TCPSM_TRANSITION_UNLOCK(SoConPtr, SoConStatePtr)

/*==================[type definitions]======================================*/

/*==================[external function declarations]=========================*/
#define SOAD_START_SEC_CODE
#include <SoAd_MemMap.h>

/** \brief Function executed on Entry action of state CLOSED
 *
 * \param[in] SoConPtr - Pointer to socket connection configuration.
 * \param[in/out] SoConStatePtr - Pointer to socket state.
 *
 */
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_State_CLOSED_Entry_Cleanup
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

/** \brief Function executed on Entry action of state CLOSING
 *
 * \param[in] SoConPtr - Pointer to socket connection configuration.
 * \param[in/out] SoConStatePtr - Pointer to socket state.
 *
 */
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_State_CLOSING_Entry_Closing
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

/** \brief Function executed on Entry action of state OFFLINE
 *
 * \param[in] SoConPtr - Pointer to socket connection configuration.
 * \param[in/out] SoConStatePtr - Pointer to socket state.
 *
 */
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_State_OFFLINE_Entry_SoConModeChg
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2CONST(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

/** \brief Function executed on Entry action of state ONLINE
 *
 * \param[in] SoConPtr - Pointer to socket connection configuration.
 * \param[in/out] SoConStatePtr - Pointer to socket state.
 *
 */
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_State_ONLINE_Entry_SoConModeChg
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2CONST(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

/** \brief Function executed on Entry action of state ONLINE
 *
 * \param[in] SoConPtr - Pointer to socket connection configuration.
 * \param[in/out] SoConStatePtr - Pointer to socket state.
 *
 */
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_State_ONLINEentry_Online
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2CONST(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

/** \brief Function executed on Entry action of state CONNECTING
 *
 * \param[in] SoConPtr - Pointer to socket connection configuration.
 * \param[in/out] SoConStatePtr - Pointer to socket state.
 *
 */
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_State_CONNECTING_Entry_Connecting
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

/** \brief Function executed on Entry action of state RESTART
 *
 * \param[in] SoConPtr - Pointer to socket connection configuration.
 * \param[in/out] SoConStatePtr - Pointer to socket state.
 *
 */
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_State_RESTART_Entry_Reset
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

/** \brief Function executed on Entry action of state SHUTDOWN
 *
 * \param[in] SoConPtr - Pointer to socket connection configuration.
 * \param[in/out] SoConStatePtr - Pointer to socket state.
 *
 */
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_State_SHUTDOWN_Entry_ShutDown
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

/** \brief Function executed on Entry action of state RECONNECT
 *
 * \param[in] SoConPtr - Pointer to socket connection configuration.
 * \param[in/out] SoConStatePtr - Pointer to socket state.
 *
 */
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_State_RECONNECT_Entry_SoConModeChg
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2CONST(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

/** \brief Function executed on Exit action of state ONLINE
 *
 * \param[in] SoConPtr - Pointer to socket connection configuration.
 * \param[in/out] SoConStatePtr - Pointer to socket state.
 *
 */
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_State_ONLINE_Exit_CloseRxTx
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

#define SOAD_STOP_SEC_CODE
#include <SoAd_MemMap.h>

#endif /* ifndef(SOAD_04_TCPSM_USER_H) */
/*==================[end of file]===========================================*/
