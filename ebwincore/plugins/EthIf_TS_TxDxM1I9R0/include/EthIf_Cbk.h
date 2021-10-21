/**
 * \file
 *
 * \brief AUTOSAR EthIf
 *
 * This file contains the implementation of the AUTOSAR
 * module EthIf.
 *
 * \version 1.9.18
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef ETHIF_CBK_H
#define ETHIF_CBK_H

/*==================[inclusions]============================================*/

#include <EthIf_Version.h>     /* Module Version Info */
#include <ComStack_Types.h>
#include <EthIf_Cfg.h>         /* this module's configuration */
#include <EthIf_Types.h>       /* Module public types */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define ETHIF_START_SEC_CODE
#include <EthIf_MemMap.h>

/**
 * \brief   calls EthIf_RxIndication
 *
 * This service directly calls EthIf_RxIndication.
 *
 * \param[in] CtrlIdx             Index of the controller within the context of the EthIf.
 * \param[in] DataPtr             Address of the received payload (including ethernet header).
 * \param[in] LenByte             Length of the payload contained in the received rx buffer.
 *
 * \ServiceID{0x10}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, ETHIF_CODE) EthIf_Cbk_RxIndication
(
  uint8 CtrlIdx,
  P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_DATA) DataPtr,
  uint16 LenByte
);

/**
 * \brief   Rx-Indication function
 *
 * This service is called by the ethernet driver in case a reception is indicated.
 *
 * \param[in] CtrlIdx             Index of the controller within the context of the EthIf.
 * \param[in] FrameType           Ethernet Frame type.
 * \param[in] isBroadcast         Indicates whether ethernet destination address is a broadcast
 *                                address or not.
 * \param[in] PhysAddrPtr         Ethernet source address.
 * \param[in] DataPtr             Address of the received payload.
 * \param[in] LenByte             Length of the payload contained in the received rx buffer.
 *
 * \ServiceID{0x10}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, ETHIF_CODE) EthIf_RxIndication
(
  uint8 CtrlIdx,
  Eth_FrameType FrameType,
  boolean isBroadcast,
  P2CONST(uint8, AUTOMATIC, ETHIF_APPL_DATA) PhysAddrPtr,
  P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_DATA) DataPtr,
  uint16 LenByte
);

/**
 * \brief   Tx-Confirmation callback function
 *
 * This service Confirms the transmission of an Ethernet frame.
 *
 * \param[in] CtrlIdx   Index of the Ethernet controller within the context of the Ethernet Interface.
 * \param[in] BufIdx    Index of the transmitted buffer.
 *
 * \ServiceID{0x11}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, ETHIF_CODE) EthIf_Cbk_TxConfirmation
(
  uint8 CtrlIdx,
  Eth_BufIdxType BufIdx
);

#if (ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430)
/**
 * \brief   Tx-Confirmation function.
 *
 * This service confirms frame transmission by the indexed controller.
 *
 * \param[in] CtrlIdx   Index of the Ethernet controller within the context of the Ethernet Interface.
 * \param[in] BufIdx    Index of the transmitted buffer.
 * \param[in] Result    E_OK: The transmission was successful,
 *                      E_NOT_OK: The transmission failed.
 *
 * \ServiceID{0x11}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, ETHIF_CODE) EthIf_TxConfirmation
(
  uint8 CtrlIdx,
  Eth_BufIdxType BufIdx,
  Std_ReturnType Result
);
#else
/**
 * \brief   Tx-Confirmation function.
 *
 * This service confirms frame transmission by the indexed controller.
 *
 * \param[in] CtrlIdx    Index of the Ethernet controller within the context of the Ethernet Interface.
 * \param[in] BufIdx     Index of the transmitted buffer.
 *
 * \ServiceID{0x11}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, ETHIF_CODE) EthIf_TxConfirmation
(
  uint8 CtrlIdx,
  Eth_BufIdxType BufIdx
);
#endif /* (ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430) */

/**
 * \brief   Callback function to indicate controller mode change.
 *
 * Called asynchronously when mode has been read out. Triggered by previous Eth_SetControllerMode()
 * call. Can be called directly within the trigger functions.
 *
 * \param[in] CtrlIdx             Index of the physical Ethernet controller within the context
 *                                of the Ethernet Interface.
 * \param[in] CtrlMode            Notified Ethernet controller mode:
 *                                ETH_MODE_DOWN
 *                                ETH_MODE_ACTIVE.
 *
 * \ServiceID{0x0E}
 * \Reentrancy{Non Reentrant for the same CtrlIdx, reentrant for different}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, ETHIF_CODE) EthIf_CtrlModeIndication
(
  uint8 CtrlIdx,
  Eth_ModeType CtrlMode
);

/**
 * \brief   Callback function to indicate transceiver mode change.
 *
 * Called asynchronously when mode has been read out. Triggered by previous Eth_SetTransceiverMode()
 * call. Can be called directly within the trigger functions.
 *
 * \param[in] TrcvIdx             Index of the Ethernet transceiver within the context of
 *                                the Ethernet Interface
 * \param[in] TrcvMode            Notified Ethernet transceiver mode:
 *                                ETHTRCV_MODE_DOWN
 *                                ETHTRCV_MODE_ACTIVE.
 *
 * \ServiceID{0x0F}
 * \Reentrancy{Non Reentrant for the same CtrlIdx, reentrant for different}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, ETHIF_CODE) EthIf_TrcvModeIndication
(
  uint8 TrcvIdx,
  EthTrcv_ModeType TrcvMode
);


#if (ETHIF_SWITCHMANAGEMENTSUPPORT == STD_ON)
/**
 * \brief   Callback function to indicate switch management info.
 *
 * Ingress Switch management info indication redirected call to upper layers who registered
 * for the call.
 *
 * \param[in] CtrlIdx             Index of an Ethernet Interface controller.
 * \param[in] DataPtr             Ethernet data pointer where the management information belongs.
 * \param[out] MgmtInfoPtr        Pointer to Management information.
 *
 * \ServiceID{0x3A}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, ETHIF_CODE) EthIf_SwitchMgmtInfoIndication
(
  uint8 CtrlIdx,
  P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_DATA) DataPtr,
  P2VAR(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr
);
#endif /* ETHIF_SWITCHMANAGEMENTSUPPORT == STD_ON */


#define ETHIF_STOP_SEC_CODE
#include <EthIf_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef ETHIF_CBK_H */
/*==================[end of file]===========================================*/
