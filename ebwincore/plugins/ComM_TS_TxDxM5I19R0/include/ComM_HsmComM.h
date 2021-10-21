/**
 * \file
 *
 * \brief AUTOSAR ComM
 *
 * This file contains the implementation of the AUTOSAR
 * module ComM.
 *
 * \version 5.19.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef COMM_HSMCOMM_H
#define COMM_HSMCOMM_H

/* Public interface for the ComM
 * state machine.
 *
 * This file defines the public symbols and functions to use the
 * ComM state machine.
 */

/*==================[inclusions]============================================*/

/* include state machine driver interface */
#include <ComM_Hsm.h>

/*==================[macros]================================================*/

/* Events/signal defined in the ComM state machine. */
#if (defined COMM_HSM_COMM_EV_BUSSM_MODE_IND)
#error COMM_HSM_COMM_EV_BUSSM_MODE_IND already defined
#endif
#define COMM_HSM_COMM_EV_BUSSM_MODE_IND 0U

#if (defined COMM_HSM_COMM_EV_BUS_SM_SLEEP_INDICATION)
#error COMM_HSM_COMM_EV_BUS_SM_SLEEP_INDICATION already defined
#endif
#define COMM_HSM_COMM_EV_BUS_SM_SLEEP_INDICATION 1U

#if (defined COMM_HSM_COMM_EV_PNC_NM_REQUEST)
#error COMM_HSM_COMM_EV_PNC_NM_REQUEST already defined
#endif
#define COMM_HSM_COMM_EV_PNC_NM_REQUEST 2U

#if (defined COMM_HSM_COMM_EV_TIMEOUT)
#error COMM_HSM_COMM_EV_TIMEOUT already defined
#endif
#define COMM_HSM_COMM_EV_TIMEOUT 3U

#if (defined COMM_HSM_COMM_EV_COMALLOWED_CHANGED)
#error COMM_HSM_COMM_EV_COMALLOWED_CHANGED already defined
#endif
#define COMM_HSM_COMM_EV_COMALLOWED_CHANGED 4U

#if (defined COMM_HSM_COMM_EV_LIMIT_TO_NO_COM)
#error COMM_HSM_COMM_EV_LIMIT_TO_NO_COM already defined
#endif
#define COMM_HSM_COMM_EV_LIMIT_TO_NO_COM 5U

#if (defined COMM_HSM_COMM_EV_PASSIVE_WAKEUP_IND)
#error COMM_HSM_COMM_EV_PASSIVE_WAKEUP_IND already defined
#endif
#define COMM_HSM_COMM_EV_PASSIVE_WAKEUP_IND 6U

#if (defined COMM_HSM_COMM_EV_WAKEUP_INHIBITION)
#error COMM_HSM_COMM_EV_WAKEUP_INHIBITION already defined
#endif
#define COMM_HSM_COMM_EV_WAKEUP_INHIBITION 7U

#if (defined COMM_HSM_COMM_EV_NM_MODE_SWITCHED)
#error COMM_HSM_COMM_EV_NM_MODE_SWITCHED already defined
#endif
#define COMM_HSM_COMM_EV_NM_MODE_SWITCHED 8U

#if (defined COMM_HSM_COMM_EV_REQ_STATUS_CHANGED)
#error COMM_HSM_COMM_EV_REQ_STATUS_CHANGED already defined
#endif
#define COMM_HSM_COMM_EV_REQ_STATUS_CHANGED 9U

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

#define COMM_START_SEC_CONST_UNSPECIFIED
#include <ComM_MemMap.h>

/** \brief Initialization data for the statechart statechart ComM */
extern CONST(ComM_HsmStatechartType, COMM_CONST)
 ComM_HsmScComM;

#define COMM_STOP_SEC_CONST_UNSPECIFIED
#include <ComM_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* defined COMM_HSMCOMM_H */
/*==================[end of file]===========================================*/
