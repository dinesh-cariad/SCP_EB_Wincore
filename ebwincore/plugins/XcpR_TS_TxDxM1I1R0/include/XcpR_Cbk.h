#ifndef XCPR_CBK_H
#define XCPR_CBK_H

/**
 * \file
 *
 * \brief AUTOSAR XcpR
 *
 * This file contains the implementation of the AUTOSAR
 * module XcpR.
 *
 * \version 1.1.0
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==[Includes]================================================================*/

#include <XcpR_Cfg.h>

/*==[Macros definitions]======================================================*/

/*==[Types declarations]======================================================*/

/*==[Declaration of constants with external linkage]==========================*/

/*==[Declaration of variables with external linkage]==========================*/

/*==[Declaration of functions with external linkage]==========================*/

#define XCPR_START_SEC_CODE
#include <XcpR_MemMap.h>

/** \brief XCPR PDU is received.
 **
 ** This function is called by the lower layer module when an
 ** XCPR PDU has been received.
 **
 ** \param[in]  XcpRRxSrcPduId  PDU-ID that has been received.
 ** \param[out] XcpRRxSrcPduPtr Pointer to SDU (Buffer of received payload).
 ** \ServiceID{::XCPR_SID_IF_RX_INDICATION}
 ** \Reentrancy{Reentrant for different XcpRRxSrcPduId and non-Reentrant for the same
 ** XcpRRxSrcPduId}
 ** \Synchronicity{Synchronous} */
/* !LINKSTO XcpR.API.RxIndication, 1 */
extern FUNC(void, XCPR_CODE) XcpR_RxIndication
(
  PduIdType XcpRRxSrcPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCPR_APPL_DATA) XcpRRxSrcPduPtr
);

/** \brief XCPR PDU transmit confirmation.
 **
 ** This function is called by the lower layer module when an
 ** XCPR PDU has been transmitted.
 **
 ** \param[in] XcpRTxPduId PDU-ID that has been transmitted.
 ** \ServiceID{::XCPR_SID_IF_TX_CONFIRMATION}
 ** \Reentrancy{Reentrant for different XcpRTxPduId and non-Reentrant for the same
 ** XcpRTxPduId}
 ** \Synchronicity{Synchronous} */
/* !LINKSTO XcpR.API.TxConfirmation, 1 */
extern FUNC(void, XCPR_CODE) XcpR_TxConfirmation
(
  PduIdType XcpRTxPduId
);

#if ((XCPR_FR_TRIGGER_TX_API_ENABLED == STD_ON) || (XCPR_CDD_TRIGGER_TX_API_ENABLED == STD_ON))
/** \brief Call when an XCPR PDU shall be transmitted.
 **
 ** This function is called by the lower layer module when an
 ** XCPR PDU shall be transmitted. The function XcpR_TriggerTransmit
 ** is called by the lower layer module for requesting the I-PDU before
 ** transmission. Whether the function XcpR_TriggerTransmit is called or not
 ** is statically configured for each I-PDU in the configuration.
 **
 ** \param[in]  XcpRTxPduId PDU-ID that has been requested to be transmitted.
 ** \param[out] PduInfoPtr  Pointer to PduInfo SDU (pointer to SDU buffer and
 **                         SDU length).
 ** \ServiceID{::XCPR_SID_IF_TRIGGER_TRANSMIT}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
/* !LINKSTO XcpR.API.TriggerTransmit, 1 */
extern FUNC(Std_ReturnType, XCPR_CODE) XcpR_TriggerTransmit
(
  PduIdType XcpRTxPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCPR_APPL_DATA) PduInfoPtr
);
#endif /* (XCPR_FR_TRIGGER_TX_API_ENABLED == STD_ON) || (XCPR_CDD_TRIGGER_TX_API_ENABLED == STD_ON) */

#if ((XCPR_SRC_ON_ETHERNET_FROM_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_DEST_ON_ETHERNET_LOWER_LAYER_ENABLED == STD_ON))
#if (XCPR_SOAD_PROTOCOL_TCP_SUPPORT == STD_ON)

/** \brief Socket connection mode change notification
 **
 ** This function is called by the lower layer Socket Adapter when the socket
 ** connection mode has changed. If the socket is closed while in XcpR connected
 ** state, the XcpR module will perform an autonomous XcpR disconnect, which means that all
 ** communication will be stopped.
 **
 ** This function is available only for the TCP protocol (not available for UDP because it is
 ** a broadcasting protocol)
 **
 ** \param[in]  SoConId  Socket connection ID of socket that has changed
 ** \param[in]  Mode  Socket connection mode (ONLINE, OFFLINE or RECONNECT)
 **
 ** \ServiceID{::XCPR_SID_SOAD_SO_CON_MODE_CHG}
 ** \Reentrancy{Reentrant for different SoConIds. Non reentrant for the same
 ** SoConId}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, XCP_CODE) XcpR_SoAdSoConModeChg
(
  SoAd_SoConIdType SoConId,
  SoAd_SoConModeType Mode
);

#endif /* (XCPR_SOAD_PROTOCOL_TCP_SUPPORT == STD_ON) */
#endif

#define XCPR_STOP_SEC_CODE
#include <XcpR_MemMap.h>

#endif /* XCPR_CBK_H */

