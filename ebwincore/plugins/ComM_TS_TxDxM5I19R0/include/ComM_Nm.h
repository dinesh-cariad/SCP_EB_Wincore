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
/* !LINKSTO ComM463_Refine_ComM_Nm,1, ComM261,1 */
#ifndef COMM_NM_H
#define COMM_NM_H

/*==================[inclusions]============================================*/

#include <ComStack_Types.h>   /* AUTOSAR COM stack standard types */
#include <ComM_Version.h>     /* ComM module version information */
#include <ComM_Cfg.h>         /* ComM configuration parameters */

/*==================[macros]================================================*/

#if (COMM_NM_ACCESS_NEEDED == STD_ON)

#if (defined COMM_SID_NM_NETWORK_START_IND)
#error COMM_SID_NM_NETWORK_START_IND already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for ComM_Nm_NetworkStartIndication(). */
#define COMM_SID_NM_NETWORK_START_IND  21U

#if (defined COMM_SID_NM_NETWORK_MODE)
#error COMM_SID_NM_NETWORK_MODE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for ComM_Nm_NetworkMode(). */
#define COMM_SID_NM_NETWORK_MODE  24U

#if (defined COMM_SID_NM_PREPARE_BUS_SLEEP)
#error COMM_SID_NM_PREPARE_BUS_SLEEP already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for ComM_Nm_PrepareBusSleepMode(). */
#define COMM_SID_NM_PREPARE_BUS_SLEEP  25U

#if (defined COMM_SID_NM_BUS_SLEEP_MODE)
#error COMM_SID_NM_BUS_SLEEP_MODE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for ComM_Nm_BusSleepMode(). */
#define COMM_SID_NM_BUS_SLEEP_MODE  26U

#if (defined COMM_SID_NM_RESTART_IND)
#error COMM_SID_NM_RESTART_IND already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for ComM_Nm_RestartIndication(). */
#define COMM_SID_NM_RESTART_IND  27U

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define COMM_START_SEC_CODE
#include <ComM_MemMap.h>

/** \brief Indicate reception of NM PDU in Bus Sleep Mode.
 **
 ** This function provides an indication that a NM PDU has been
 ** received in the Bus Sleep Mode, which indicates that some
 ** nodes in the network have already entered the Network Mode.
 **
 ** \param[in] Channel Network channel on which the NM PDU
 **                    has been received.
 **
 ** \ServiceID{::COMM_SID_NM_NETWORK_START_IND}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, COMM_CODE) ComM_Nm_NetworkStartIndication
(
  NetworkHandleType Channel
);


/** \brief Notify that the network management has entered Network Mode.
 **
 ** This function provides a notification that the network management
 ** has entered Network Mode.
 **
 ** \param[in] Channel Network channel on which the mode transition
 **                    has occurred.
 **
 ** \ServiceID{::COMM_SID_NM_NETWORK_MODE}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, COMM_CODE) ComM_Nm_NetworkMode
(
  NetworkHandleType Channel
);


/** \brief Notify that the network management has entered Prepare Bus Sleep Mode.
 **
 ** This function provides a notification that the network
 ** management has entered Prepare Bus Sleep Mode.
 **
 ** \param[in] Channel Network channel on which the mode transition
 **                    has occurred.
 **
 ** \ServiceID{::COMM_SID_NM_PREPARE_BUS_SLEEP}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, COMM_CODE) ComM_Nm_PrepareBusSleepMode
(
  NetworkHandleType Channel
);


/** \brief Notify that the network management has entered Bus Sleep Mode.
 **
 ** This function provides a notification that the network management
 ** has entered Bus Sleep Mode.
 **
 ** \param[in] Channel Network channel on which the mode transition
 **            has occurred.
 **
 ** \ServiceID{::COMM_SID_NM_BUS_SLEEP_MODE}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, COMM_CODE) ComM_Nm_BusSleepMode
(
  NetworkHandleType Channel
);


/** \brief Indicate a restart of NM.
 **
 ** This function provides an indication that NmIf has started
 ** to shut down the coordinated buses, and not all coordinated
 ** buses have indicated bus sleep state, and on at least one of
 ** the coordinated buses NM is restarted.
 **
 ** \param[in] Channel Network channel on which the NM PDU
 **            has been received.
 **
 ** \ServiceID{::COMM_SID_NM_RESTART_IND}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, COMM_CODE) ComM_Nm_RestartIndication
(
  NetworkHandleType Channel
);

#define COMM_STOP_SEC_CODE
#include <ComM_MemMap.h>

#endif /* (COMM_NM_ACCESS_NEEDED == STD_ON) */

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef COMM_NM_H */
/*==================[end of file]===========================================*/
