/**
 * \file
 *
 * \brief AUTOSAR UdpNm
 *
 * This file contains the implementation of the AUTOSAR
 * module UdpNm.
 *
 * \version 2.9.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef UDPNM_HSMUDPNM_H
#define UDPNM_HSMUDPNM_H

/* Public interface for the UdpNm
 * state machine.
 *
 * This file defines the public symbols and functions to use the
 * UdpNm state machine.
 */

/*==================[inclusions]============================================*/

/* include state machine driver interface */
#include <UdpNm_Hsm.h>
#include <UdpNm_Adaptive.h>
/*==================[macros]================================================*/

/* Events/signal defined in the UdpNm state machine. */
#if (defined UDPNM_HSM_UDPNM_EV_COM_CONTROL)
#error UDPNM_HSM_UDPNM_EV_COM_CONTROL already defined
#endif
#define UDPNM_HSM_UDPNM_EV_COM_CONTROL 0U

#if (defined UDPNM_HSM_UDPNM_EV_TX_CONFIRMATION)
#error UDPNM_HSM_UDPNM_EV_TX_CONFIRMATION already defined
#endif
#define UDPNM_HSM_UDPNM_EV_TX_CONFIRMATION 1U

#if (defined UDPNM_HSM_UDPNM_EV_NM_TIMEOUT)
#error UDPNM_HSM_UDPNM_EV_NM_TIMEOUT already defined
#endif
#define UDPNM_HSM_UDPNM_EV_NM_TIMEOUT 2U

#if (defined UDPNM_HSM_UDPNM_EV_MSG_CYCLE_TIMEOUT)
#error UDPNM_HSM_UDPNM_EV_MSG_CYCLE_TIMEOUT already defined
#endif
#define UDPNM_HSM_UDPNM_EV_MSG_CYCLE_TIMEOUT 3U

#if (defined UDPNM_HSM_UDPNM_EV_REPEAT_MESSAGE_REASON)
#error UDPNM_HSM_UDPNM_EV_REPEAT_MESSAGE_REASON already defined
#endif
#define UDPNM_HSM_UDPNM_EV_REPEAT_MESSAGE_REASON 4U

#if (defined UDPNM_HSM_UDPNM_EV_RMS_TIMEOUT)
#error UDPNM_HSM_UDPNM_EV_RMS_TIMEOUT already defined
#endif
#define UDPNM_HSM_UDPNM_EV_RMS_TIMEOUT 5U

#if (defined UDPNM_HSM_UDPNM_EV_NETWORK_START)
#error UDPNM_HSM_UDPNM_EV_NETWORK_START already defined
#endif
#define UDPNM_HSM_UDPNM_EV_NETWORK_START 6U

#if (defined UDPNM_HSM_UDPNM_EV_NET_REQ_STATUS_CHANGED)
#error UDPNM_HSM_UDPNM_EV_NET_REQ_STATUS_CHANGED already defined
#endif
#define UDPNM_HSM_UDPNM_EV_NET_REQ_STATUS_CHANGED 7U

#if (defined UDPNM_HSM_UDPNM_EV_RX_INDICATION)
#error UDPNM_HSM_UDPNM_EV_RX_INDICATION already defined
#endif
#define UDPNM_HSM_UDPNM_EV_RX_INDICATION 8U

#if (defined UDPNM_HSM_UDPNM_EV_UNI_TIMEOUT)
#error UDPNM_HSM_UDPNM_EV_UNI_TIMEOUT already defined
#endif
#define UDPNM_HSM_UDPNM_EV_UNI_TIMEOUT 9U

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/
#if (UDPNM_ADAPTIVE == STD_OFF)
#define UDPNM_START_SEC_CONST_UNSPECIFIED
#include <UdpNm_MemMap.h>

/** \brief Initialization data for the statechart statechart UdpNm */
extern CONST(UdpNm_HsmStatechartType, UDPNM_CONST)
 UdpNm_HsmScUdpNm;

#define UDPNM_STOP_SEC_CONST_UNSPECIFIED
#include <UdpNm_MemMap.h>
#else /* #if (UDPNM_ADAPTIVE == STD_OFF) */
#define UDPNM_START_SEC_VAR_INIT_UNSPECIFIED
#include <UdpNm_MemMap.h>
extern VAR(UdpNm_HsmStatechartType, UDPNM_APPL_DATA) UdpNm_HsmScUdpNm;
#define UDPNM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <UdpNm_MemMap.h>
#endif

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* defined UDPNM_HSMUDPNM_H */
/*==================[end of file]===========================================*/
