#ifndef ETH_WINCAP_API_H
#define ETH_WINCAP_API_H

/**
 * \file
 *
 * \brief AUTOSAR Eth
 *
 * This file contains the implementation of the AUTOSAR
 * module Eth.
 *
 * \version 0.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2011 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/

#include <Std_Types.h>     /* Autosar standard data types */

/*==================[macros]================================================*/

/*==================[macro checks]==========================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define ETH_START_SEC_CODE
#include <Eth_MemMap.h>

/** \brief Print Available Interfaces
 **
 ** Prints a list of all available interfaces
 **
 **/
extern FUNC(void, EBTEST_CODE) Eth_PrintAvialableInterfaces
(
  void
);

#define ETH_STOP_SEC_CODE
#include <Eth_MemMap.h>
/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

#define ETH_START_SEC_VAR_INIT_8
#include <MemMap.h>

extern VAR(uint8, TCPIP_VAR_NOINIT ) Eth_InterfaceId;

#define ETH_STOP_SEC_VAR_INIT_8
#include <MemMap.h>

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef ETH_WINCAP_API_H */
/*==================[end of file]===========================================*/

