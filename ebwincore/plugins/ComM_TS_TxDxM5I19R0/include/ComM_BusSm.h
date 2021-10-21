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
/* !LINKSTO ComM463_Refine_ComM_BusSm,1 */
#ifndef COMM_BUSSM_H
#define COMM_BUSSM_H

/*==================[inclusions]============================================*/

#include <ComStack_Types.h> /* AUTOSAR standard types */
#include <ComM_Version.h>   /* ComM module version information */
#include <ComM_Cfg.h>       /* ComM configuration parameters */

/*==================[macros]================================================*/

#if (defined COMM_SID_BUS_SM_INDICATION)
#error COMM_SID_BUS_SM_INDICATION already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for ComM_BusSM_ModeIndication(). */
#define COMM_SID_BUS_SM_INDICATION  51U

#if (defined COMM_SID_BUS_SM_BUS_SLEEP_MODE)
#error COMM_SID_BUS_SM_BUS_SLEEP_MODE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for ComM_BusSM_BusSleepMode(). */
#define COMM_SID_BUS_SM_BUS_SLEEP_MODE  246U

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define COMM_START_SEC_CODE
#include <ComM_MemMap.h>

#ifndef RTE_SCHM_SWCBSW_COMM_BUSSM_MODEINDICATION
#define RTE_SCHM_SWCBSW_COMM_BUSSM_MODEINDICATION
/** \brief Indicate that BusSM has changed its communication mode.
 **
 ** This function provides an indication that the BusSM has changed
 ** its communication mode. The new mode is supplied as a parameter.
 **
 ** \param[in] Channel Network channel on which the mode transition
 **                    has occurred.
 ** \param[in] ComMode New communication mode of the BusSM.
 **
 ** \ServiceID{::COMM_SID_BUS_SM_INDICATION}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Asynchronous} */
extern FUNC(void, COMM_CODE) ComM_BusSM_ModeIndication
(
    NetworkHandleType Channel,
    P2VAR(uint8, AUTOMATIC, COMM_APPL_DATA) ComMode
);
#endif /* RTE_SCHM_SWCBSW_COMM_BUSSM_MODEINDICATION */


/** \brief Notify that the bus state management has entered Bus Sleep Mode.
 **
 ** This function provides a notification that the bus state management
 ** has entered Bus Sleep Mode.
 **
 ** \param[in] Channel Network channel on which the mode transition
 **            has occurred.
 **
 ** \ServiceID{::COMM_SID_BUSSM_BUS_SLEEP_MODE}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, COMM_CODE) ComM_BusSM_BusSleepMode
(
  NetworkHandleType Channel
);

#define COMM_STOP_SEC_CODE
#include <ComM_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef COMM_BUSSM_H */
/*==================[end of file]===========================================*/
