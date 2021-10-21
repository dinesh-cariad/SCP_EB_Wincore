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

#ifndef EB_INTGR_ETH_IRQUSERCALLOUT_H
#define EB_INTGR_ETH_IRQUSERCALLOUT_H

/*==================[includes]==============================================*/
#include <TSAutosar.h>     /* EB specific standard types */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/* AUTOSAR Memory Mapping - start section for code */
#define ETH_START_SEC_CODE
#include <Eth_MemMap.h>

/** \brief EB integration Eth interrupt rx user callout
*
* This function is called by the Eth driver for each Rx interrupt. The intension of this function
* is to trigger the regular receive frame processing task immediately after receving a frame.
* Inside the triggered task the regualr Rx frame processing itself is done by calling the API
* Eth_Receive().
*
* Prerequest: Following configuration parameters must be enabled:
* EthEnableRxInterruptUsercalloutSupport, EthCtrlEnableRxInterrupt
*
* \param[in] CtrlIdx - Controller index
* \param[in] FifoIdx - Ingress Fifo index
*/
extern FUNC(void, ETH_CODE) Eb_Intgr_EthIrqRxUsercallout
(
  uint8 CtrlIdx,
  uint8 FifoIdx
);

/** \brief EB integration Eth interrupt tx user callout
*
* This function is called by the Eth driver for each Tx interrupt. The intension of this function
* is to trigger the regular transmit frame processing task immediately after transmission of a
* frame.
* Inside the triggered task the regualr Tx frame processing itself is done by calling the API
* Eth_TxConfirmation().
*
* Prerequest: Following configuration parameters must be enabled:
* EthEnableTxInterruptUsercalloutSupport, EthCtrlEnableTxInterrupt
*
* \param[in] CtrlIdx - Controller index
*/
extern FUNC(void, ETH_CODE) Eb_Intgr_EthIrqTxUsercallout
(
  uint8 CtrlIdx
);

#define ETH_STOP_SEC_CODE
#include <Eth_MemMap.h>

/*==================[internal function definitions]=========================*/


#endif /* ifndef EB_INTGR_ETH_IRQUSERCALLOUT_H */

/*==================[end of file]===========================================*/

