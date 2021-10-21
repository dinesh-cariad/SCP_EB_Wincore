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
#ifndef ETHSM_CBK_H
#define ETHSM_CBK_H

/*==================[inclusions]============================================*/

#include <TSAutosar.h>          /* EB specific standard types */
#include <EthSM_Version.h>      /* Module Version Info */
#include <Eth_GeneralTypes.h>
#include <EthSM_Cfg.h>          /* Module configuration */

/*==================[macro checks]==========================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define ETHSM_START_SEC_CODE
#include <EthSM_MemMap.h>

/**
 * \brief Transceiver link state change indication.
 *
 * This service indicates a change of the transceiver link.
 *
 * \param[in] CtrlIdx - Controller index which changed the TcpIp state.
 * \param[in] LinkState - New state the transceiver link has changed to.
 *
 * \ServiceID{0x06}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, ETHSM_CODE) EthSM_TrcvLinkStateChg
(
  uint8 CtrlIdx,
  EthTrcv_LinkStateType LinkState
);

/**
 * \brief Control mode change indication.
 *
 * This service indicates a change of the Ethernet controler mode.
 *
 * \param[in] CtrlIdx - Ethernet controller whose mode has changed.
 * \param[in] CtrlMode - Notified Ethernet controller mode.
 *
 * \ServiceID{0x09}
 * \Reentrancy{Reentrant only for different Ethernet controllers}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, ETHSM_CODE) EthSM_CtrlModeIndication
(
  uint8 CtrlIdx,
  Eth_ModeType CtrlMode
);

#if (ETHSM_DEVAUTH_SUPPORT_ENABLE == STD_ON)
/**
 * \brief DevAuth NoCom indication.
 *
 * This service is confirmation of EAPoL-Logoff message transmission from Device Authentication
 * module. Upon receiving it EthSM can perform EthIfCtrl shutdown.
 *
 * \param[in] CtrlIdx - Ethernet controller used.
 *
 * \ServiceID{0x11}
 * \Reentrancy{Reentrant only for different Ethernet controllers}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, ETHSM_CODE) EthSM_DevAuthNoComModeIndication
(
  uint8 CtrlIdx
);
#endif /* ETHSM_DEVAUTH_SUPPORT_ENABLE == STD_ON */

#define ETHSM_STOP_SEC_CODE
#include <EthSM_MemMap.h>


/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef ETHSM_CBK_H */
/*==================[end of file]===========================================*/
