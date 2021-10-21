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
#ifndef ETH_LCFG_H
#define ETH_LCFG_H

/*==================[includes]==============================================*/

#include <Std_Types.h>     /* Autosar standard data types */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/* start data section declaration */
#define ETH_START_SEC_CONST_32
#include <Eth_MemMap.h>

/* value used to validate post build configuration against link time configuration */
extern CONST(uint32, ETH_CONST) Eth_LcfgSignature;

/* stop data section declaration */
#define ETH_STOP_SEC_CONST_32
#include <Eth_MemMap.h>

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef ETH_LCFG_H */
/*==================[end of file]===========================================*/

