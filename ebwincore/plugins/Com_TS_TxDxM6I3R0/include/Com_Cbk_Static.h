/**
 * \file
 *
 * \brief AUTOSAR Com
 *
 * This file contains the implementation of the AUTOSAR
 * module Com.
 *
 * \version 6.3.50
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef COM_CBK_STATIC_H
#define COM_CBK_STATIC_H


/*==================[inclusions]=============================================*/

#include <TSAutosar.h>          /* EB specific standard types */
#include <ComStack_Types.h>     /* AUTOSAR standard types */

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

#define COM_START_SEC_CODE
#include <Com_MemMap.h>

/**
 * \brief Com_TriggerTransmit - copy data to PDU-router memory
 *
 * a function to be used to fetch data from the COM immediately
 * Preconditions:
 * - COM should be initialized
 *
 * \param[in] ComTxPduId ID of the PDU which's data shall be copied
 * \param[in] PduInfoPtr Contains a pointer to a buffer (SduDataPtr) to where the SDU
 * shall be copied to. On return, the service will indicate the length of
 * the copied SDU data in SduLength.
 * \return
 * \retval E_OK: SDU has been copied and SduLength indicates the
 * number of copied bytes.
 * \retval E_NOT_OK: No SDU has been copied. PduInfoPtr must not be
 * used since it may contain a NULL pointer or point to invalid data.
 *
 * \ServiceID{0x41}
 * \Reentrancy{Re-entrant for different values of ComTxPduId only.}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, COM_CODE) Com_TriggerTransmit
(
   PduIdType ComTxPduId,
   P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr
);


/**
 * \brief Com_RxIndication - Signal the COM a PDU has arrived
 *
 * This functions signals the COM that a PDU has arrived
 * Preconditions:
 * - COM should be initialized
 *
 * \param[in] ComRxPduId ID of the received I-PDU.
 * \param[in] PduInfoPtr Contains the length (SduLength) of the received I-PDU and a pointer to
 * a buffer (SduDataPtr) containing the I-PDU.
 *
 * \ServiceID{0x42}
 * \Reentrancy{Re-entrant for different values of ComRxPduId only.}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, COM_CODE) Com_RxIndication
(
   PduIdType ComRxPduId,
   P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr
);


/**
 * \brief Com_TpRxIndication - indicating the correct, or incorrect,
 *                             end of the reception process.
 *
 * This functions signals the COM the correct, or incorrect,
 * end of the reception process.
 * Preconditions:
 * - COM should be initialized
 *
 * \param[in] PduId - ID of the AUTOSAR COM module's I-PDU that has been received.
 *                    Identifies the data that has been received.
 * \param[in] Result - NTFRSLT_OK: the complete I-PDU has been received and is
 *                                 stored in the receive buffer.
 *                     ANY OTHER VALUE: the I-PDU has not been received;
 *                     the receive buffer can be unlocked by the AUTOSAR COM
 *
 * \ServiceID{0x1E}
 * \Reentrancy{Re-entrant for different values of ComRxPduId only.}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, COM_CODE) Com_TpRxIndication
(
    PduIdType PduId,
    NotifResultType Result
);


/**
 * \brief Com_TxConfirmation
 *
 * Function to signal the COM that an IPDU has been transmitted
 * Preconditions:
 * - COM should be initialized
 *
 * \param[in] ComTxPduId ID of the PDU which was transmitted successfully
 *
 * \ServiceID{0x40}
 * \Reentrancy{Re-entrant for different values of ComTxPduId only.}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, COM_CODE ) Com_TxConfirmation
(
   PduIdType ComTxPduId
);


/** \brief TpTxConfirmation
 * Function to signal the COM that an large IPDU has been transmitted
 * Preconditions:
 * - COM should be initialized
 * \param[in] PduId - ID of the large PDU which was transmitted successfully
 * \param[in] Result - Result of the transmission of the I-PDU
 */
extern FUNC(void, COM_CODE ) Com_TpTxConfirmation
(
    PduIdType PduId,
    NotifResultType Result
);


/** \brief Com_CopyTxData
 * function which copy the requested transmit data of the large IPDU
 * Preconditions:
 * - prior to this call, the COM module shall be initialized
 * - the Pdu must be started before
 * \param[in] PduId - ID of Tp I-PDU to be transmitted
 * \param[in] PduInfoPtr - Pointer to a PduInfoType, which indicates the number
 *            of bytes to be copied (SduLength) and the location where the data
 *            have to be copied to (SduDataPtr). An SduLength of 0 is possible in
 *            order to poll the available transmit data count. In this case no
 *            data are to be copied and SduDataPtr might be invalid.
 * \param[in] RetryInfoPtr - The COM module ignores the value of this pointer, since
 *            it always keeps the complete buffer until the transmission of a large
 *            I-PDU is either confirmed or aborted.
 * \param[out] TxDataCntPtr - Remaining Tx data after successful completion of this call
 *             (Com_CopyTxData returns BUFREQ_OK otherwise out put parameter TxDataCntPtr
 *             does not change).
 * \retval BUFREQ_OK - Data has been copied to the transmit buffer completely as requested.
 * \retval BUFREQ_E_BUSY - The transmission buffer is actually not available (implementation specific).
 * \retval BUFREQ_E_NOT_OK - Data has not been copied. Request failed, in case the corresponding I-PDU was stopped.
 */
extern FUNC(BufReq_ReturnType, COM_CODE) Com_CopyTxData
(
   PduIdType PduId,
   P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
   P2VAR(RetryInfoType, AUTOMATIC, COM_APPL_DATA) RetryInfoPtr,
   P2VAR(PduLengthType, AUTOMATIC, COM_APPL_DATA) TxDataCntPtr
);


/** \brief Com_CopyRxData
 * Called once upon reception of each segment. Within this call, the received
 * data is copied to the receive TP buffer. The function must only be called
 * if the connection has been accepted by an initial call to Com_StartOfReception.
 * Preconditions:
 * - prior to this call, the COM module shall be initialized
 * - the Pdu must be started before
 * \param[in] PduId - ID of Tp I-PDU to be transmitted
 * \param[in] PduInfoPointer - Pointer to a PduInfoType, which indicates the number
 *            of bytes to be copied (SduLength) and the location of the source
 *            data (SduDataPtr).
 *            An SduLength of 0 is possible in order to poll the available receive
 *            buffer size. In this case no data are to be copied and PduInfoPtr
 *            might be invalid.
 * \param[out] RxBufferSizePtr - Remaining receive buffer after successful completion of this call
 *             (Com_CopyRxData returns BUFREQ_OK otherwise out put parameter RxBufferSizePtr does
 *             not change).
 * \retval BUFREQ_OK - Data has been copied to the receive buffer completely as requested.
 * \retval BUFREQ_E_BUSY - The receive buffer is actually not available (implementation specific).
 * \retval BUFREQ_E_NOT_OK - Data has not been copied. Request failed.
 */
extern FUNC(BufReq_ReturnType, COM_CODE) Com_CopyRxData
(
   PduIdType PduId,
   P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPointer,
   P2VAR(PduLengthType, AUTOMATIC, COM_APPL_DATA) RxBufferSizePtr
);


/** \brief Com_StartOfReception
 * returns the pointer to the size of the AUTOSAR COM module's internal
 * receive buffer for the I-PDU with ID ComRxPduId.
 * Preconditions:
 * - prior to this call, the COM module shall be initialized
 * - the Pdu must be started before
 * \param[in] ComRxPduId - ID of Tp I-PDU to be received.
 * \param[in] TpSduLength - complete length of the TP I-PDU to be received.
 * \param[out] RxBufferSizePtr - Pointer to the size of internal TP-receive buffer
 * \retval BUFREQ_OK - Connection has been accepted. RxBufferSizePtr indicates the
 *                     available receive buffer.
 * \retval BUFREQ_E_NOT_OK - Connection has been rejected. RxBufferSizePtr remains
 *                           unchanged.
 * \retval BUFREQ_E_OVFL - In case the configured buffer size as specified via
 *                         ComPduIdRef.PduLength is smaller than TpSduLength.
 * \retval BUFREQ_E_BUSY - In case the reception buffer is actually not available for
 *                        a new reception (implementation specific).
 */
extern FUNC(BufReq_ReturnType, COM_CODE) Com_StartOfReception
(
   PduIdType ComRxPduId,
   PduLengthType TpSduLength,
   P2VAR(PduLengthType, AUTOMATIC, COM_APPL_DATA) RxBufferSizePtr
);


#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/



/*==================[internal function definitions]==========================*/

#endif /* ifndef COM_CBK_STATIC_H */
/*==================[end of file]============================================*/
