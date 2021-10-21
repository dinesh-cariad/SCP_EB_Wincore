/**
 * \file
 *
 * \brief AUTOSAR DoIP
 *
 * This file contains the implementation of the AUTOSAR
 * module DoIP.
 *
 * \version 1.1.21
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef DOIP_CBK_H
#define DOIP_CBK_H

/*==================[includes]==============================================*/

#include <ComStack_Types.h>
/* !LINKSTO DoIP.ASR41.SWS_DoIP_00157, 1 */
#include <SoAd.h>
/* !LINKSTO DoIP.ASR41.SWS_DoIP_00157, 1 */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/
#define DOIP_START_SEC_CODE
#include <DoIP_MemMap.h>

/** \brief API to request data to transmit
 *
 * \param[in] TxPduId - DoIP handle ID to be used for DoIP APIs to be called from SoAd.
 * \param[in] PduInfoPtr - Pointer providing a buffer and length to copy the Tx data.
 * \param[in] Retry - This parameter is expected to be a NULL_PTR as retry is not supported by the
 *                    DoIP.
 * \param[out] AvailableDataPtr - Pointer which returns remaining number of bytes to be copied.

 * \return BufReq_ReturnType
 * \retval BUFREQ_OK - Data is copied.
 * \retval BUFREQ_E_BUSY - Request postponed. No data is copied.
 * \retval BUFREQ_E_NOT_OK - Request failed.
 *
 * \ServiceID{0x06}
 * \Reentrancy{Non reentrant}
 * \Synchronicity{Synchronous}
*/
extern FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_SoAdTpCopyTxData
(
  PduIdType TxPduId,
  /* P2VAR required for compatibility reason to SoAd */
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_CONST) PduInfoPtr,
  P2VAR(RetryInfoType, AUTOMATIC, DOIP_APPL_DATA) Retry,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_DATA) AvailableDataPtr
);

/** \brief API to confirm a TCP transmission
 *
 * \param[in] TxPduId - DoIP handle ID to be used for DoIP APIs to be called from SoAd.
 * \param[in] Result - Parameter indicates the result of the transmission.
 *
 * \ServiceID{0x07}
 * \Reentrancy{Non reentrant}
 * \Synchronicity{Synchronous}
*/
extern FUNC(void, DOIP_CODE) DoIP_SoAdTpTxConfirmation
(
  PduIdType TxPduId,
  NotifResultType Result
);

/** \brief API to provide received data
 *
 * \param[in] RxPduId - DoIP handle ID to be used for DoIP APIs to be called from SoAd.
 * \param[in] PduInfoPtr - Pointer providing received data and data length.
 * \param[out] BufferSizePtr - Amount of data which shall be provided next call.

 * \return BufReq_ReturnType
 * \retval BUFREQ_OK - Data is copied.
 * \retval BUFREQ_E_BUSY - Request postponed. No data is copied.
 * \retval BUFREQ_E_NOT_OK - Request failed.
 *
 * \ServiceID{0x08}
 * \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId}
 * \Synchronicity{Synchronous}
*/
extern FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_SoAdTpCopyRxData
(
  PduIdType RxPduId,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_DATA) BufferSizePtr
);

/** \brief API to start a reception
 *
 * This function is called once by SoAd if a TCP connection to a tester is established.
 *
 * \param[in] RxPduId - DoIP handle ID to be used for DoIP APIs to be called from SoAd.
 * \param[in] TpSduLength - Message length shall always be zero for DoIP.
 * \param[out] BufferSizePtr Available Rx buffer in the DoIP module.

 * \return BufReq_ReturnType
 * \retval BUFREQ_OK - Reception request has been accepted. RxBufferSizePtr indicates the available
 *                     receive buffer.
 * \retval BUFREQ_E_OVFL - No Buffer of the required length can be provided.
 * \retval BUFREQ_E_NOT_OK - Reception request has been rejected. RxBufferSizePtr remains unchanged.
 *
 * \ServiceID{0x09}
 * \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId}
 * \Synchronicity{Synchronous}
*/
extern FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_SoAdTpStartOfReception
(
  PduIdType RxPduId,
  PduLengthType TpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_DATA) BufferSizePtr
);

/** \brief API to indicate that all TCP receptions from this tester are finished
 *
 * \param[in] RxPduId - DoIP handle ID to be used for DoIP APIs to be called from SoAd.
 * \param[in] Result - Result of the finished reception.
 *
 * \ServiceID{0x0A}
 * \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId}
 * \Synchronicity{Synchronous}
*/
extern FUNC(void, DOIP_CODE) DoIP_SoAdTpRxIndication
(
  PduIdType RxPduId,
  NotifResultType Result
);

/** \brief API to indicate SoAd socket connection state change.
 *
 * \param[in] SoConId - Socket connection index.
 * \param[in] Mode - New connection mode.
 *
 * \ServiceID{0x0B}
 * \Reentrancy{Reentrant for different SoConIds. Non reentrant for the same SoConId}
 * \Synchronicity{Synchronous}
*/
extern FUNC(void, DOIP_CODE) DoIP_SoConModeChg
(
  SoAd_SoConIdType SoConId,
  SoAd_SoConModeType Mode
);

/** \brief API to indicate local IP address assignment changes.
 *
 * \param[in] SoConId - Socket connection index.
 * \param[in] State - Returns information if IP address is assigned or not.
 *
 * \ServiceID{0x0C}
 * \Reentrancy{Reentrant for different SoConIds. Non reentrant for the same SoConId}
 * \Synchronicity{Synchronous}
*/
extern FUNC(void, DOIP_CODE) DoIP_LocalIpAddrAssignmentChg
(
  SoAd_SoConIdType SoConId,
  TcpIp_IpAddrStateType State
);

/** \brief API to enable connections.
 *
 * This function opens all configured socket connections.
 *
 * \ServiceID{0x0F}
 * \Reentrancy{Non reentrant}
 * \Synchronicity{Synchronous}
*/
extern FUNC(void, DOIP_CODE) DoIP_ActivationLineSwitchActive(void);

/** \brief API to disable connections.
 *
 * This function closes all configured socket connections.
 *
 * \ServiceID{0x0F}
 * \Reentrancy{Non reentrant}
 * \Synchronicity{Synchronous}
*/
extern FUNC(void, DOIP_CODE) DoIP_ActivationLineSwitchInactive(void);

/** \brief API to confirm a UDP transmission
 *
 * \param[in] TxPduId - DoIP handle ID to be used for DoIP APIs to be called from SoAd.
 *
 * \ServiceID{0x40}
 * \Reentrancy{Non reentrant}
 * \Synchronicity{Synchronous}
*/
extern FUNC(void, DOIP_CODE) DoIP_SoAdIfTxConfirmation
(
  PduIdType TxPduId
);

/** \brief API to indicate a UDP reception
 *
 * \param[in] RxPduId - DoIP handle ID to be used for DoIP APIs to be called from SoAd.
 * \param[in] PduInfoPtr - Pointer to received data.
 *
 * \ServiceID{0x42}
 * \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId}
 * \Synchronicity{Synchronous}
*/
extern FUNC(void, DOIP_CODE) DoIP_SoAdIfRxIndication
(
  PduIdType RxPduId,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_DATA) PduInfoPtr
);

#define DOIP_STOP_SEC_CODE
#include <DoIP_MemMap.h>
/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* DOIP_CBK_H */
/*==================[end of file]===========================================*/

