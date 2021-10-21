/**
 * \file
 *
 * \brief AUTOSAR EthTSyn
 *
 * This file contains the implementation of the AUTOSAR
 * module EthTSyn.
 *
 * \version 2.2.4
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef ETHTSYN_INT02_SWT_H_H
#define ETHTSYN_INT02_SWT_H_H

/*==================[inclusions]============================================*/

#include <TSAutosar.h> /* Specific standard types */
#include <ComStack_Types.h> /* Communication stack types */
#include <EthTSyn_Cfg.h> /* Module configuration */
#include <EthTSyn_Int.h>
#include <StbM.h> /* Module public API */
#include <EthIf.h> /* Module public API */


/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/
#define ETHTSYN_START_SEC_CODE
#include <EthTSyn_MemMap.h>

#if(ETHTSYN_SWITCH_USED == STD_ON)

/**
 * \brief Service to transmit a Sync frame to a specific port of a switch
 *
 * \param[in] EthTSynCtrlIdx - Index of the EthTSyn controller.
 * \param[in] PortIdx - Index of the PortIdx of the switch.
 *
 */
TS_MOD_PRIV_DECL FUNC(void, ETHTSYN_CODE) EthTSyn_Inc02_TransmitSwtSyncFramePerPort
(
  uint8 EthTSynCtrlIdx,
  uint8 PortIdx
);

/**
 * \brief Service to transmit a FuP frame to a specific port of a switch
 *
 * \param[in] EthTSynCtrlIdx - Index of the EthTSyn controller.
 * \param[in] PortIdx - Index of the PortIdx of the switch.
 *
 */
TS_MOD_PRIV_DECL FUNC(void, ETHTSYN_CODE) EthTSyn_Inc02_TransmitSyncSwtFUpFramePerPort
(
  uint8 EthTSynCtrlIdx,
  EthTSyn_IntPortIdxType PortIdx
);

/**
 * \brief Service to transmit a Sync frame to a specific port of a switch
 *
 * \param[in] EthTSynCtrlIdx - Index of the EthTSyn controller.
 *
 */
TS_MOD_PRIV_DECL FUNC(void, ETHTSYN_CODE) EthTSyn_Inc02_TransmitSwtOnHostPortSyncFrame
(
  uint8 EthTSynCtrlIdx
);

/**
 * \brief Service to transmit a Pdelay_req frame to a specific port of a switch
 *
 * \param[in] EthTSynCtrlIdx - Index of the EthTSyn controller.
 * \param[in] PortIdx - Index of the PortIdx of the switch.
 *
 */
TS_MOD_PRIV_DECL FUNC(void, ETHTSYN_CODE) EthTSyn_Inc02_TransmitSwtPdelayReqFramePerPort
(
  uint8 EthTSynCtrlIdx,
  uint8 PortIdx
);

/**
 * \brief Service to transmit a Pdelay_resp frame to a specific port of a switch
 *
 * \param[in] EthTSynCtrlIdx - Index of the EthTSyn controller.
 * \param[in] PortIdx - Index of the PortIdx of the switch.
 *
 */
TS_MOD_PRIV_DECL FUNC(void, ETHTSYN_CODE) EthTSyn_Inc02_TransmitSwtPdelayRespFramePerPort
(
  uint8 EthTSynCtrlIdx,
  uint8 PortIdx
);

/**
 * \brief Service to transmit a Pdelay_resp_fup frame to a specific port of a switch
 *
 * \param[in] EthTSynCtrlIdx - Index of the EthTSyn controller.
 * \param[in] PortIdx - Index of the PortIdx of the switch.
 *
 */
TS_MOD_PRIV_DECL FUNC(void, ETHTSYN_CODE) EthTSyn_Inc02_TransmitSwtPdelayRespFupFramePerPort
(
  uint8 EthTSynCtrlIdx,
  uint8 PortIdx
);

/**
 * \brief Service to get Swt Tx buffer and fill the PTP header
 *
 * \param[in] EthTSynCtrlIdx - Index of the EthTSyn controller.
 * \param[in] PayloadLength - Length of the frame payload.
 * \param[in] IntPortIdx - Internal unique port Idx.
 * \param[out] BufIdxPtr - Pointer to the buffer index of the requested Tx frame buffer.
 * \param[out] BufPtr - Pointer to return the pointer of the Ethernet Tx frame buffer.
 *
 * \return E_OK: Tx buffer available and big enough.
 *         E_NOT_OK: No suitable Tx buffer available.
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Inc02_GetSwtTxBuffer
(
  uint8 EthTSynCtrlIdx,
  uint16 PayloadLength,
  EthTSyn_IntPortIdxType IntPortIdx,
  P2VAR(Eth_BufIdxType, AUTOMATIC, ETHTSYN_APPL_DATA) BufIdxPtr,
  P2VAR(EthTSyn_PtrUint8, AUTOMATIC, ETHIF_APPL_DATA) BufPtr
);

/**
 * \brief Service to transmit a frame over switch ports.
 *
 * \param[in] EthTSynCtrlIdx - Index of the EthTSyn controller.
 * \param[in] BufIdx - Buffer index of the requested Tx frame buffer.
 * \param[in] RequestTimeStamp - Boolean to request switch time stamp
 * \param[in] IntPortIdx - Internal unique port Idx.
 * \param[in] ActivateTimeStamp - Activates egress time stamping.
 * \param[in] LenByte - Length of the frame to be transmitted.
 * \param[in] IsPdelayResp - Identify if the transmitted frame is a pdelay_resp
 *
 * \return E_OK: Tx was successful
 *         E_NOT_OK: Tx was not successful
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Inc02_TransmitSwtFrame
(
  uint8 EthTSynCtrlIdx,
  Eth_BufIdxType BufIdx,
  boolean RequestTimeStamp,
  EthTSyn_IntPortIdxType IntPortIdx,
  boolean ActivateTimeStamp,
  uint16 LenByte,
  boolean IsPdelayResp
);

TS_MOD_PRIV_DECL FUNC(void, ETHTSYN_CODE) EthTSyn_Inc02_SwtCorrFieldPTPHeader
(
  uint8 EthTSynCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr,
  uint8 PortIdx
);

/**
 * \brief Service handle syn Tx state machine
 *
 * \param[in] EthTSynCtrlIdx - Index of the EthTSyn controller.
 */
TS_MOD_PRIV_DECL FUNC(void, ETHTSYN_CODE) EthTSyn_Inc02_SwtSynStateHandling
(
  uint8 EthTSynCtrlIdx
);

/**
 * \brief Service to perform transmit swt frame after debounce was perfomed
 *
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 * \param[in] PortIdx - Port index the frame belongs to.
 *
 */
TS_MOD_PRIV_DECL FUNC(void, ETHTSYN_CODE) EthTSyn_Inc02_TransmitSwtFrameWithDebounce
(
  uint8 EthTSynCtrlIdx,
  uint8 PortIdx
);

/**
 * \brief Service to reset pdelay state machine of a port.
 *
 * \param[in] PortIdx - Port index the frame belongs to.
 *
 */
TS_MOD_PRIV_DECL FUNC(void, ETHTSYN_CODE) EthTSyn_Inc02_ResetPortPdelayVars
(
  uint8 PortIdx
);

/**
 * \brief Service to verify the SubTLV data when using a simple bridge configuration.
 *
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 * \param[in] DataPtr - Pointer to store the received FUp information.
 * \param[inout] Crc_Time_1 - Pointer to store the calculated Crc_Time_1.
 *
 */
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
TS_MOD_PRIV_DECL FUNC(void, ETHTSYN_CODE) EthTSyn_Inc02_BridgeCalculateCrcTime1
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) Crc_Time_1
);
#endif /* (ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON) */

/**
 * \brief Function used to process the received frame from EthSwt.
 *
 * \param[in] MgmtInfoPtr - Switch and Port ID.
 * \param[in] IngressTimeStampPtr - Ingress timestamp provided by the EthSwt.
 * \param[in] DataPtr - Address of the received payload.
 * \param[in] Len - Length of the payload contained in the received Rx buffer.
 * \param[in] EthTSynCtrlIdx - Index of the EthTSyn controller.
 * \param[in] UniqueId - UniqueId provided by EthSwt used for identification.
 * \return true: TimeStamp is expected.
 *         false: TimeStamp is not expected.
 */
TS_MOD_PRIV_DECL FUNC(boolean, ETHTSYN_CODE) EthTSyn_Inc02_ProcessRxSwtMsg
( 
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHTSYN_APPL_DATA) MgmtInfoPtr,
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) IngressTimeStampPtr,
  P2VAR(Eth_DataType, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint8 EthTSynCtrlIdx,
  uint16 Len,
  uint32 UniqueId
);

#endif /* (ETHTSYN_SWITCH_USED == STD_ON) */

#define ETHTSYN_STOP_SEC_CODE
#include <EthTSyn_MemMap.h>

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#endif /* ifndef ETHTSYN_INT02_SWT_H_H */
/*==================[end of file]===========================================*/
