#ifndef ETH_LEGACY_H
#define ETH_LEGACY_H

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

extern FUNC(uint8, ETH_CODE) Eth_Legacy_TxConfirmation
(
    uint8 CtrlIdx,
    uint8 u8BufIndx
);

#define ETH_STOP_SEC_CODE
#include <Eth_MemMap.h>
/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef ETH_LEGACY_H */
/*==================[end of file]===========================================*/

