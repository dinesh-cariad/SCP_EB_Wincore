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

#include <TSAutosar.h>         /* EB specific standard types */
#include <Eth_Version.h>       /* Module Version Info */
#include <Eth_Cfg.h>            /* eth config data */
#include <Eth_Api.h>
#include <Eth_Int.h>           /* internal module API and DET */
#include <Eth_Legacy.h>        /* function declaration of legacy code */
#include <EthIf_Cbk.h>         /* Call back function for TxConfirmation */

/*==================[version check]=========================================*/

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#define ETH_START_SEC_CODE
#include <Eth_MemMap.h>

FUNC(void, ETH_CODE) Eth_Arch_TxConfirmation
(
    uint8 CtrlIdx,
    boolean Irq
)
{
  uint8 u8BufIndx = 0U;
  uint8 u8Feedback;

  TS_PARAM_UNUSED(Irq);
  /*
   * ETH_TX_CONFIRMATION_FIN(255U) is reserved for informing about no buffer to check left,
   * ETH_TX_CONFIRMATION_COMPLETE(1U) is reservied for positive Confirmation
   * ETH_TX_CONFIRMATION_FIN(0U) is reserved for tx not complete or no TxConfirmation requested
   */
  do
  {
    u8Feedback = Eth_Legacy_TxConfirmation(CtrlIdx, u8BufIndx);
    if ( ETH_TX_CONFIRMATION_COMPLETE == u8Feedback )
    {
        EthIf_TxConfirmation(CtrlIdx, u8BufIndx);
    }
      /* else{}*/
    ++u8BufIndx;
  }
  while ( ETH_TX_CONFIRMATION_FIN != u8Feedback );
}

FUNC(Std_ReturnType, ETH_CODE) Eth_Arch_UpdatePhysAddrFilter
(
  uint8 CtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETH_APPL_DATA) PhysAddrPtr,
  Eth_FilterActionType Action
)
{
  TS_PARAM_UNUSED(CtrlIdx);
  TS_PARAM_UNUSED(PhysAddrPtr);
  TS_PARAM_UNUSED(Action);
  return E_NOT_OK;
}

#define ETH_STOP_SEC_CODE
#include <Eth_MemMap.h>
/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/

