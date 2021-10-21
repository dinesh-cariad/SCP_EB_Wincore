/**
 * \file
 *
 * \brief AUTOSAR CanNm
 *
 * This file contains the implementation of the AUTOSAR
 * module CanNm.
 *
 * \version 6.19.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef CANNM_HSMCANNM_H
#define CANNM_HSMCANNM_H

/* Public interface for the CanNm
 * state machine.
 *
 * This file defines the public symbols and functions to use the
 * CanNm state machine.
 */

/*==================[inclusions]============================================*/

/* include state machine driver interface */
#include <CanNm_Hsm.h>

/*==================[macros]================================================*/

/* Events/signal defined in the CanNm state machine. */
#if (defined CANNM_HSM_CANNM_EV_COM_CONTROL)
#error CANNM_HSM_CANNM_EV_COM_CONTROL already defined
#endif
#define CANNM_HSM_CANNM_EV_COM_CONTROL 0U

#if (defined CANNM_HSM_CANNM_EV_TX_CONFIRMATION)
#error CANNM_HSM_CANNM_EV_TX_CONFIRMATION already defined
#endif
#define CANNM_HSM_CANNM_EV_TX_CONFIRMATION 1U

#if (defined CANNM_HSM_CANNM_EV_TX_TIMEOUT)
#error CANNM_HSM_CANNM_EV_TX_TIMEOUT already defined
#endif
#define CANNM_HSM_CANNM_EV_TX_TIMEOUT 2U

#if (defined CANNM_HSM_CANNM_EV_MSG_CYCLE_TIMEOUT)
#error CANNM_HSM_CANNM_EV_MSG_CYCLE_TIMEOUT already defined
#endif
#define CANNM_HSM_CANNM_EV_MSG_CYCLE_TIMEOUT 3U

#if (defined CANNM_HSM_CANNM_EV_NM_TIMEOUT)
#error CANNM_HSM_CANNM_EV_NM_TIMEOUT already defined
#endif
#define CANNM_HSM_CANNM_EV_NM_TIMEOUT 4U

#if (defined CANNM_HSM_CANNM_EV_REPEAT_MESSAGE_REASON)
#error CANNM_HSM_CANNM_EV_REPEAT_MESSAGE_REASON already defined
#endif
#define CANNM_HSM_CANNM_EV_REPEAT_MESSAGE_REASON 5U

#if (defined CANNM_HSM_CANNM_EV_RMS_TIMEOUT)
#error CANNM_HSM_CANNM_EV_RMS_TIMEOUT already defined
#endif
#define CANNM_HSM_CANNM_EV_RMS_TIMEOUT 6U

#if (defined CANNM_HSM_CANNM_EV_NETWORK_START)
#error CANNM_HSM_CANNM_EV_NETWORK_START already defined
#endif
#define CANNM_HSM_CANNM_EV_NETWORK_START 7U

#if (defined CANNM_HSM_CANNM_EV_NET_REQ_STATUS_CHANGED)
#error CANNM_HSM_CANNM_EV_NET_REQ_STATUS_CHANGED already defined
#endif
#define CANNM_HSM_CANNM_EV_NET_REQ_STATUS_CHANGED 8U

#if (defined CANNM_HSM_CANNM_EV_RX_INDICATION)
#error CANNM_HSM_CANNM_EV_RX_INDICATION already defined
#endif
#define CANNM_HSM_CANNM_EV_RX_INDICATION 9U

#if (defined CANNM_HSM_CANNM_EV_UNI_TIMEOUT)
#error CANNM_HSM_CANNM_EV_UNI_TIMEOUT already defined
#endif
#define CANNM_HSM_CANNM_EV_UNI_TIMEOUT 10U

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

#define CANNM_START_SEC_CONST_UNSPECIFIED
#include <CanNm_MemMap.h>

/** \brief Initialization data for the statechart statechart CanNm */
extern CONST(CanNm_HsmStatechartType, CANNM_CONST)
 CanNm_HsmScCanNm;

#define CANNM_STOP_SEC_CONST_UNSPECIFIED
#include <CanNm_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* defined CANNM_HSMCANNM_H */
/*==================[end of file]===========================================*/
