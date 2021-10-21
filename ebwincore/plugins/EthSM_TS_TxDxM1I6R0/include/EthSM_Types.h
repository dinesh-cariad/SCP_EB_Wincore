/**
 * \file
 *
 * \brief AUTOSAR EthSM
 *
 * This file contains the implementation of the AUTOSAR
 * module EthSM.
 *
 * \version 1.6.14
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef ETHSM_TYPES_H
#define ETHSM_TYPES_H

/*==================[inclusions]============================================*/

#include <TSAutosar.h>         /* EB AUTOSAR standard types */
#include <EthSM_Cfg.h>

/*==================[macros]================================================*/

#if (defined ETHSM_STATE_NUM)
#error  ETHSM_STATE_NUM is already defined
#endif
/** \brief Number of internal states (for EthSM_NetworkModeStateType). */
#define ETHSM_STATE_NUM 8U

/*==================[type definitions]======================================*/

/** \brief State type definition (including definition of state macros). */
#if (defined ETHSM_STATE_OFFLINE)
#error  ETHSM_STATE_OFFLINE is already defined
#endif
#define ETHSM_STATE_OFFLINE 0U

#if (defined ETHSM_STATE_WAIT_TRCVLINK)
#error  ETHSM_STATE_WAIT_TRCVLINK is already defined
#endif
#define ETHSM_STATE_WAIT_TRCVLINK 1U

#if (defined ETHSM_STATE_WAIT_ONLINE)
#error  ETHSM_STATE_WAIT_ONLINE is already defined
#endif
#define ETHSM_STATE_WAIT_ONLINE 2U

#if (defined ETHSM_STATE_ONLINE)
#error  ETHSM_STATE_ONLINE is already defined
#endif
#define ETHSM_STATE_ONLINE 3U

#if (defined ETHSM_STATE_ONHOLD)
#error  ETHSM_STATE_ONHOLD is already defined
#endif
#define ETHSM_STATE_ONHOLD 4U

#if (defined ETHSM_STATE_WAIT_OFFLINE)
#error  ETHSM_STATE_WAIT_OFFLINE is already defined
#endif
#define ETHSM_STATE_WAIT_OFFLINE 5U

#if (defined ETHSM_STATE_WAIT_CTRLMODEIND)
#error  ETHSM_STATE_WAIT_CTRLMODEIND is already defined
#endif
#define ETHSM_STATE_WAIT_CTRLMODEIND 6U

#if (defined ETHSM_STATE_WAIT_LINK_DOWN)
#error  ETHSM_STATE_WAIT_LINK_DOWN is already defined
#endif
#define ETHSM_STATE_WAIT_LINK_DOWN 7U

#if (defined ETHSM_STATE_WAIT_CTRLMIND_DOWN)
#error  ETHSM_STATE_WAIT_CTRLMIND_DOWN is already defined
#endif
#define ETHSM_STATE_WAIT_CTRLMIND_DOWN 8U

#if ETHSM_DEVAUTH_SUPPORT_ENABLE == STD_ON

#if (defined ETHSM_STATE_WAIT_DEVAUTH_OFFLINE)
#error  ETHSM_STATE_WAIT_DEVAUTH_OFFLINE is already defined
#endif
#define ETHSM_STATE_WAIT_DEVAUTH_OFFLINE 9U

#endif /* ETHSM_DEVAUTH_SUPPORT_ENABLE */

/* Note: Adding or removing a state from the list above will affect ETHSM_STATE_NUM. */
typedef uint8 EthSM_NetworkModeStateType;

/** \brief Definition of old state macros.
 *
 *  EthSM used to provide these macros to BswM to be used as values for EthSM_NetworkModeStateType
 *  before AUTOSAR 4.1.2. Since CanSM still provides these values in the same style they shall be
 *  kept to be backwards compatible.
 */
#if (defined ETHSM_BSWM_OFFLINE)
#error  ETHSM_BSWM_OFFLINE is already defined
#endif
#define ETHSM_BSWM_OFFLINE 0U

#if (defined ETHSM_BSWM_WAIT_TRCVLINK)
#error  ETHSM_BSWM_WAIT_TRCVLINK is already defined
#endif
#define ETHSM_BSWM_WAIT_TRCVLINK 1U

#if (defined ETHSM_BSWM_WAIT_ONLINE)
#error  ETHSM_BSWM_WAIT_ONLINE is already defined
#endif
#define ETHSM_BSWM_WAIT_ONLINE 2U

#if (defined ETHSM_BSWM_ONLINE)
#error  ETHSM_BSWM_ONLINE is already defined
#endif
#define ETHSM_BSWM_ONLINE 3U

#if (defined ETHSM_BSWM_ONHOLD)
#error  ETHSM_BSWM_ONHOLD is already defined
#endif
#define ETHSM_BSWM_ONHOLD 4U

#if (defined ETHSM_BSWM_WAIT_OFFLINE)
#error  ETHSM_BSWM_WAIT_OFFLINE is already defined
#endif
#define ETHSM_BSWM_WAIT_OFFLINE 5U

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef ETHSM_TYPES_H */
/*==================[end of file]===========================================*/
