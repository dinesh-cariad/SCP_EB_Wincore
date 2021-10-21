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
#ifndef CANSM_DCM_H
#define CANSM_DCM_H
/*==================[inclusions]============================================*/

#include <CanSM_Cfg.h>         /* Generated configuration */
#include <ComM.h>              /* Communication Manager's types */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function declarations]========================*/

#if (CANSM_SET_BAUDRATE_API == STD_ON)

#define CANSM_START_SEC_CODE
#include <CanSM_MemMap.h>

/** \brief Requests change of baudrate for indicated network.
 **
 ** This service shall start an asynchronous process to change the baud rate for the
 ** configured  CAN  controllers  of  a  certain  CAN  network.  Depending  on  necessary
 ** baud rate modifications the controllers might have to reset.
 **
 ** \param[in] network Handle of destined communication network for request.
 ** \param[in] BaudRateConfigID References a baud rate configuration by ID (see CanControllerBaudRateConfigID).
 **
 ** \return Std_ReturnType
 ** \retval E_OK Service request accepted, setting of (new) baud rate.
 ** \retval E_NOT_OK Service request not accepted.
 **
 ** \ServiceID{13}
 ** \Reentrancy{Non-Reentrant for same network}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CANSM_CODE) CanSM_SetBaudrate (
  NetworkHandleType network,
  uint16 BaudRateConfigID
);

#define CANSM_STOP_SEC_CODE
#include <CanSM_MemMap.h>

#endif /* CANSM_SET_BAUDRATE_API */


#endif /* ifndef CANSM_DCM_H */
/*==================[end of file]===========================================*/

