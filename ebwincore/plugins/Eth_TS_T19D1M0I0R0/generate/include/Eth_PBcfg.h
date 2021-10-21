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

#ifndef ETH_PBCFG_H
#define ETH_PBCFG_H

/*==================[inclusions]=============================================*/

#include <Eth_SymbolicNames_PBcfg.h>
#include <Std_Types.h>     /* Autosar standard data types */
#include <Eth_TypesInt.h>


/*==================[macros]=================================================*/
[!VAR "EthConfigName" = "name(EthConfigSet/*[1])"!]

#if (defined [!"$EthConfigName"!]) /* To prevent double definition */
#error [!"$EthConfigName"!] already defined
#endif /* (defined [!"$EthConfigName"!]) */

#define [!"$EthConfigName"!] (Eth_ConfigLayout.RootCfg)


/*==================[type definitions]=======================================*/

typedef struct /* Eth_ConfigLayoutType */ {
    VAR( Eth_ConfigType, TYPEDEF ) RootCfg;
    VAR( Eth_CtrlType, TYPEDEF ) aCtrls[1];
} Eth_ConfigLayoutType;

typedef CONST( Eth_ConfigLayoutType, ETH_APPL_CONST ) Eth_ConstConfigLayoutType;

/*==================[external function declarations]=========================*/


/*==================[internal function declarations]=========================*/


/*==================[external constants]=====================================*/

#define ETH_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <Eth_MemMap.h>

extern Eth_ConstConfigLayoutType Eth_ConfigLayout;

#define ETH_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <Eth_MemMap.h>


/*==================[internal constants]=====================================*/


/*==================[external data]==========================================*/


/*==================[internal data]==========================================*/


/*==================[external function definitions]==========================*/


/*==================[internal function definitions]==========================*/


#endif /* ETH_PBCFG_H */

/*==================[end of file]============================================*/

