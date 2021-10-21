#ifndef DCCM_CBK_H
#define DCCM_CBK_H

/***************************************************************************
 *
 * \file Dccm_Cbk.h
 *
 * \brief Diagnostic Client Communication Manager implementation of Dccm Callback functions
 *
 * Copyright 2020 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* MISRA-C:2012 Deviation List
 *
 */

#include <ComStack_Types.h>     /* AUTOSAR Standard types */
#include "Dccm_Types.h"

/*==================[macros]=================================================*/

/** \brief Added for backward compatibility with old PduR implementations **/
#define Dccm_ProvideRxBuffer Dccm_CopyRxData
/** \brief Added for backward compatibility with old PduR implementations **/
#define Dccm_ProvideTxBuffer Dccm_CopyTxData

/*==================[external function declarations]=========================*/

#define DCCM_START_SEC_CODE
#include <MemMap.h>

/** \brief API to indicate that all receptions have finished
 **
 ** This function will be called if data has been received.
 **
 ** \param[in] RxPduId - Dccm handle ID to be used for Dccm APIs to be called from PduR.
 ** \param[in] Result - Result of the finished reception.
 **
 ** \ServiceID{::Dccm_RxIndication}
 ** \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(void, DCCM_CODE) Dccm_RxIndication
(
  PduIdType       RxPduId,
  NotifResultType Result
);

/** \brief API to confirm a TCP transmission
 **
 ** This function indicates if the transmission was successful.
 **
 ** \param[in] TxPduId - Dccm handle ID to be used for Dccm APIs to be called from PduR.
 ** \param[in] Result - Parameter indicates the result of the transmission.
 **
 ** \ServiceID{::Dccm_TxConfirmation}
 ** \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(void, DCCM_CODE) Dccm_TxConfirmation
(
  PduIdType       TxPduId,
  NotifResultType Result
);

/** \brief API to copy data from receive buffer
 **
 ** This function copies the TpRx data to the Dccm receive buffer.
 **
 ** \param[in] RxPduId - Dccm handle ID to be used for Dccm APIs to be called from PduR.
 ** \param[in] PduInfoPtr - Pointer providing received data and data length.
 ** \param[out] RxBufferSizePtr - The number of bytes that are still unused in the receive buffer,
 **           and that can be used to store the next data packages that will be received.

 ** \return BufReq_ReturnType
 ** \retval BUFREQ_OK - Data is copied.
 ** \retval BUFREQ_E_NOT_OK - Request failed.
 **
 ** \ServiceID{::Dccm_CopyRxData}
 ** \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(BufReq_ReturnType, DCCM_CODE) Dccm_CopyRxData
(
  PduIdType     RxPduId,
  P2VAR(PduInfoType, AUTOMATIC, DCCM_APPL_DATA) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DCCM_APPL_DATA) RxBufferSizePtr
);

/** \brief API to request data to transmit
 **
 ** This function copies the Dccm transmit data to the CanTp transmit buffer.
 **
 ** \param[in] TxPduId - Dccm handle ID to be used for Dccm APIs to be called from PduR.
 ** \param[in] PduInfoPtr - Pointer providing a buffer and length to copy the Tx data.
 ** \param[in] RetryInfoPtr - This parameter is used to acknowledge transmitted data or to retransmit data after transmission problems.
 **                  Please see the document Autosar SWS for PduRouter for details.
 ** \param[out] AvailableDataPtr - Pointer which returns remaining number of bytes to be copied.
 **           Eg.: from a total of 10 bytes, only 3 were transmitted during the current call of Dccm_CopyTxData(), so
 **           AvailableDataPtr will show that there are 7 bytes that wait to be transmitted,
 **           with further calls to the same function Dccm_CopyTxData().
 **
 ** \return BufReq_ReturnType
 ** \retval BUFREQ_OK - Data is copied.
 ** \retval BUFREQ_E_BUSY - The number of bytes that still wait to be transmitted, after the call to this function ends.
 *          Eg.: from a total of 10 bytes, only 3 were transmitted during the current call of Dccm_CopyTxData(), so
 *          AvailableDataPtr will show that there are 7 bytes that wait to be transmitted,
 *          with further calls to the same function Dccm_CopyTxData().
 ** \retval BUFREQ_E_NOT_OK - Request failed.
 *
 ** \ServiceID{::Dccm_CopyTxData}
 ** \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(BufReq_ReturnType, DCCM_CODE) Dccm_CopyTxData
(
  PduIdType TxPduId,
  P2VAR(PduInfoType, AUTOMATIC, DCCM_APPL_DATA) PduInfoPtr,
  P2VAR(RetryInfoType, AUTOMATIC, DCCM_APPL_DATA) RetryInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DCCM_APPL_DATA) AvailableDataPtr
);

/** \brief API to start a reception
 **
 ** This function is called once by PduR if a connection has been established.
 **
 ** \param[in] RxPduId - Dccm handle ID to be used for Dccm APIs to be called from PduR.
 ** \param[in] TpTotalLength - Message length.
 ** \param[out] RxBufferSizePtr Available Rx buffer in the Dccm module.

 ** \return BufReq_ReturnType
 ** \retval BUFREQ_OK - Reception request has been accepted. RxBufferSizePtr indicates the available
 **                     receive buffer.
 ** \retval BUFREQ_E_NOT_OK - Reception request has been rejected. RxBufferSizePtr remains unchanged.
 **
 ** \ServiceID{Dccm_StartOfReception}
 ** \Reentrancy{::Reentrant for different PduIds. Non reentrant for the same PduId}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(BufReq_ReturnType, DCCM_CODE) Dccm_StartOfReception
(
  PduIdType     RxPduId,
  PduLengthType TpTotalLength,
  P2VAR(PduLengthType, AUTOMATIC, DCCM_APPL_DATA) RxBufferSizePtr
);

#define DCCM_STOP_SEC_CODE
#include <MemMap.h>

#endif
