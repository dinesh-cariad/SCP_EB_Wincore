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
#ifndef SOAD_01_UDPSM_USER_H
#define SOAD_01_UDPSM_USER_H

/*==================[inclusions]============================================*/

#include <TSAutosar.h> /* EB specific standard types. */
#include <ComStack_Types.h>
#include <SoAd_Trace.h>
#include <SoAd_01_UdpSM_User.h> /* Unit public header file. */
#include <SoAd_02_SocketCon_Int.h> /* Common socket handling relevant APIs. */
#include <SoAd.h> /* Module public header file. */
#include <SoAd_Int.h> /* Module internal header file. */

/*==================[macros]================================================*/

/** \brief Transition lock macros
 *
 * These macros can be used to lock the transition execution for other functions.
 * If required, add a function which locks a variable. The state machine will call the
 * LOCK macro at the beginning of the transition and the UNLOCK macro after the transition.
 *
 * \warning It need to be ensured that no memory ordering is possible. Otherwise the lock has no
 *          effect.
 */
#if (defined SOAD_01_UDPSM_TRANSITION_LOCK)
#error SOAD_01_UDPSM_TRANSITION_LOCK is already defined
#endif
#define SOAD_01_UDPSM_TRANSITION_LOCK(SoConPtr, SoConStatePtr)

#if (defined SOAD_01_UDPSM_TRANSITION_UNLOCK)
#error SOAD_01_UDPSM_TRANSITION_UNLOCK is already defined
#endif
#define SOAD_01_UDPSM_TRANSITION_UNLOCK(SoConPtr, SoConStatePtr)

/*==================[type definitions]======================================*/

/*==================[external function declarations]=========================*/
#define SOAD_START_SEC_CODE
#include <SoAd_MemMap.h>

/** \brief Function calculating the TXING_OK Status Flag
 *
 * \param[in/out] SoConStatePtr - Pointer to socket state.
 *
 */
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_Int_01_Setup_UDP_TXING_SoConState
(
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

/** \brief Function executed on Entry action of state CLOSED
 *
 * \param[in] SoConPtr - Pointer to socket connection configuration.
 * \param[in/out] SoConStatePtr - Pointer to socket state.
 *
 */
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_Int_01_UdpSM_State_CLOSED_Entry_Cleanup
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
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_Int_01_UdpSM_State_CLOSING_Entry_Closing
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
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_Int_01_UdpSM_State_OFFLINE_Entry_SoConModeChg
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
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_Int_01_UdpSM_State_ONLINE_Entry_SoConModeChg
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
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_Int_01_UdpSM_State_ONLINE_Entry_Online
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
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_Int_01_UdpSM_State_CONNECTING_Entry_Connecting
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
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_Int_01_UdpSM_State_RESTART_Entry_Restart
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
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_Int_01_UdpSM_State_SHUTDOWN_Entry_ShutDown
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
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_Int_01_UdpSM_State_RECONNECT_Entry_SoConModeChg
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
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_Int_01_UdpSM_State_ONLINE_Exit_CloseRxTx
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

#define SOAD_STOP_SEC_CODE
#include <SoAd_MemMap.h>

#endif /* ifndef(SOAD_01_UDPSM_USER_H) */
/*==================[end of file]===========================================*/
