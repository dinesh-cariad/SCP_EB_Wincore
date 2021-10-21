/**
 * \file
 *
 * \brief AUTOSAR PduR
 *
 * This file contains the implementation of the AUTOSAR
 * module PduR.
 *
 * \version 5.3.46
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef PDUR_WRAPASR32_H
#define PDUR_WRAPASR32_H

/*==================[inclusions]============================================*/
#include <ComStack_Types.h>       /* AUTOSAR Communication Stack types */
#include <PduR_Types.h>           /* type definitions of PduR module   */

/*==================[version check]=========================================*/

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/
 /* Avoid nested compiler abstractions due to requirement COMPILER058 */
typedef P2VAR(PduInfoType, TYPEDEF, PDUR_APPL_DATA) PduR_WrapASR32PduInfoPtrType;

/* Function pointer type to wrap to AUTOSAR 3.2 ProvideRxBuffer */
typedef P2FUNC(BufReq_ReturnType, PDUR_APPL_CODE, PduR_ProvRxBufFpType)
(
   PduIdType RxPduId,
   PduLengthType TpSduLength,
   P2VAR(PduR_WrapASR32PduInfoPtrType, AUTOMATIC, PDUR_APPL_DATA) PduInfoPtrPtr
);

/* Function pointer type to wrap to AUTOSAR 3.2 ProvideTxBuffer */
typedef P2FUNC(BufReq_ReturnType, PDUR_APPL_CODE, PduR_ProvTxBufFpType)
(
   PduIdType TxPduId,
   P2VAR(PduR_WrapASR32PduInfoPtrType, AUTOMATIC, PDUR_APPL_DATA) PduInfoPtrPtr,
   PduLengthType Length
);

/* Reception session data to wrap to AUTOSAR 3.2 */
typedef struct {
   P2VAR(uint8, AUTOMATIC, PDUR_APPL_DATA) BufferPtr;
   PduLengthType TpSduLength;
   PduLengthType AvailableBufferSize;
} PduR_WrapASR32RxSessionInfoType;

/* Transmission session data to wrap to AUTOSAR 3.2 */
typedef struct {
   P2VAR(uint8, AUTOMATIC, PDUR_APPL_DATA) BufferBeginPtr;
   PduLengthType BufferSize;
   PduLengthType AvailableDataSize;
} PduR_WrapASR32TxSessionInfoType;


/*==================[external function declarations]========================*/
#if (TS_MERGED_COMPILE == STD_OFF)
#define PDUR_START_SEC_CODE
#include <PduR_MemMap.h>
#endif

/**
 ** \brief Generic AUTOSAR 3.2 wrapper function for Up_StartOfReception
 **
 ** Generic StartOfReception function called within generated StartOfReception wrapper function
 ** that wraps to an AUTOSAR 3.2 upper layer module with TP interface.
 **
 ** \param[in]  RxPduId             Identification of the received I-PDU.
 ** \param[in]  TpSduLength         Total length of the PDU to be received.
 ** \param[out] AvailableBufferSizePtr  Available receive buffer in the receiving module.
 **                                 This parameter will be used to compute the Block Size (BS)
 **                                 in the transport protocol module.
 ** \param[in]  ProvideRxBufferFp   Function pointer to ProvideRxBuffer function of AUTOSAR 3.2
 **                                 upper layer module.
 ** \param[in]  RxIndicationFp      Function pointer to RxIndication function of AUTOSAR 3.2
 **                                 upper layer module. Necessary since AUTOSAR 4.0 considers
 **                                 the connection to be closed if StartOfReception is not
 **                                 successful.
 ** \param[in,out] RxSessionInfoPtr  Pointer to the session data of this RxPduId.
 **
 ** \return Result of buffer request
 ** \retval BUFREQ_OK        Connection has been accepted.
 **                          BufferSizePtr indicates the available receive buffer.
 **                          Reception is continued.
 ** \retval BUFREQ_E_OVFL    No Buffer of the required length can be provided. Reception is aborted.
 **                          BufferSizePtr remains unchanged.
 ** \retval BUFREQ_E_NOT_OK  Connection has been rejected. Reception is aborted.
 **                          BufferSizePtr remains unchanged.
 **
 **/
extern FUNC(BufReq_ReturnType, PDUR_CODE) PduR_WrapASR32StartOfReception
(
  PduIdType RxPduId,
  PduLengthType TpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) AvailableBufferSizePtr,
  PduR_ProvRxBufFpType ProvideRxBufferFp,
  PduR_TpRxIndFpType RxIndicationFp,
  P2VAR(PduR_WrapASR32RxSessionInfoType, AUTOMATIC, PDUR_APPL_DATA) RxSessionInfoPtr
);


/**
 ** \brief Generic AUTOSAR 3.2 wrapper function for Up_CopyRxData
 **
 ** Generic CopyRxData function called within generated CopyRxData wrapper function
 ** that wraps to an AUTOSAR 3.2 upper layer module with TP interface.
 **
 ** \param[in]  RxPduId             Identification of the received I-PDU.
 ** \param[in]  PduInfoPtr          Pointer to the buffer (SduDataPtr) and its length (SduLength)
 **                                 containing the data to be copied by PDU Router module in
 **                                 case of gateway or upper layer module in case of reception.
 ** \param[out] AvailableBufferSizePtr  Available receive buffer in the receiving module.
 **                                 This parameter will be used to compute the Block Size (BS)
 **                                 in the transport protocol module.
 ** \param[in]  ProvideRxBufferFp   Function pointer to ProvideRxBuffer function of AUTOSAR 3.2
 **                                 upper layer module.
 ** \param[in,out] RxSessionInfoPtr  Pointer to the session data of this RxPduId.
 **
 ** \return Result of buffer request
 ** \retval BUFREQ_OK        Data copied successfully.
 ** \retval BUFREQ_E_NOT_OK  Data was not copied because an error occurred.
 **
 **/
extern FUNC(BufReq_ReturnType, PDUR_CODE) PduR_WrapASR32CopyRxData
(
  PduIdType RxPduId,
  P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) AvailableBufferSizePtr,
  PduR_ProvRxBufFpType ProvideRxBufferFp,
  P2VAR(PduR_WrapASR32RxSessionInfoType, AUTOMATIC, PDUR_APPL_DATA) RxSessionInfoPtr
);


/**
 ** \brief Generic AUTOSAR 3.2 wrapper function for Up_TpRxIndication
 **
 ** Generic RxIndication function called within generated RxIndication wrapper function
 ** that wraps to an AUTOSAR 3.2 upper layer module with TP interface.
 **
 ** \param[in]  RxPduId             Identification of the received I-PDU.
 ** \param[in]  Result              Result of the reception.
 ** \param[in]  RxIndicationFp      Function pointer to RxIndication function of AUTOSAR 3.2
 **                                 upper layer module. Necessary since AUTOSAR 4.0 considers
 **                                 the connection to be closed if StartOfReception is not
 **                                 successful.
 ** \param[in,out] RxSessionInfoPtr  Pointer to the session data of this RxPduId.
 **
 **/
extern FUNC(void, PDUR_CODE) PduR_WrapASR32RxIndication
(
  PduIdType RxPduId,
  NotifResultType Result,
  PduR_TpRxIndFpType RxIndicationFp,
  P2VAR(PduR_WrapASR32RxSessionInfoType, AUTOMATIC, PDUR_APPL_DATA) RxSessionInfoPtr
);


/**
 ** \brief Generic AUTOSAR 3.2 wrapper function for Up_CopyTxData
 **
 ** Generic CopyTxData function called within generated CopyTxData wrapper function
 ** that wraps to an AUTOSAR 3.2 upper layer module with TP interface.
 **
 ** \param[in]  TxPduId             Identification of the transmitted I-PDU.
 ** \param[out] PduInfoPtr          Provides destination buffer and the number of bytes to copy.
 **                                 In case of gateway the PDU Router module will copy,
 **                                 otherwise the source upper layer module will copy the data.
 **                                 If not enough transmit data is available,
 **                                 no data is copied. The transport protocol module will retry.
 **                                 A copy size of 0 can be used to indicate state changes in the
 **                                 retry parameter.
 ** \param[in]  RetryInfoPtr        This parameter is used to retransmit data because problems
 **                                 during the last service call.
 **                                 If the I-PDU is transmitted from a local module (e.g. DCM) the
 **                                 PDU router module will just forward the parameter value
 **                                 without check. If the I-PDU is gatewayed from another bus,
 **                                 the PDU Router module will make the following interpretation:
 **                                 If the transmitted TP I-PDU does not support the retry feature
 **                                 a NULL_PTR is provided. It indicates that the copied transmit
 **                                 data can be removed from the buffer after it has been copied.
 **                                 If the retry feature is used by the Tx I-PDU, RetryInfoPtr must
 **                                 point to a valid RetryInfoType element.
 **                                 If TpDataState indicates TP_CONFPENDING, the previously
 **                                 copied data must remain in the TP buffer to be available for
 **                                 error recovery.
 **                                 TP_DATACONF indicates that all data that have been copied
 **                                 so far are confirmed and can be removed from the TP buffer.
 **                                 Data copied by this API call are excluded and will be
 **                                 confirmed later.
 **                                 TP_DATARETRY indicates that this API call shall copy
 **                                 already copied data in order to recover from an error. In this
 **                                 case AvailableDataPtr specifies the offset of the first byte to
 **                                 be copied by the API call.
 ** \param[out] AvailableDataSizePtr    Indicates the remaining number of bytes that are available
 **                                 in the PduR Tx buffer. AvailableDataPtr can be used by
 **                                 TP modules that support dynamic payload lengths (e.g. FlexRay
 **                                 ISO Transport Layer) to determine the size of the following CFs.
 ** \param[in]  ProvideTxBufferFp   Function pointer to ProvideTxBuffer function of AUTOSAR 3.2
 **                                 upper layer module.
 ** \param[in,out] TxSessionInfoPtr  Pointer to the session data of this TxPduId.
 **
 ** \return Result of buffer request
 ** \retval BUFREQ_OK        Data has been copied to the transmit buffer completely as requested.
 ** \retval BUFREQ_E_BUSY    Request could not be fulfilled, because the required amount of
 **                          Tx data is not available. TP layer might retry later on.
 **                          No data has been copied.
 ** \retval BUFREQ_E_NOT_OK  Data has not been copied. Request failed.
 **
 **/
extern FUNC(BufReq_ReturnType, PDUR_CODE) PduR_WrapASR32CopyTxData
(
  PduIdType TxPduId,
  P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) PduInfoPtr,
  P2VAR(RetryInfoType, AUTOMATIC, PDUR_APPL_DATA) RetryInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) AvailableDataSizePtr,
  PduR_ProvTxBufFpType ProvideTxBufferFp,
  P2VAR(PduR_WrapASR32TxSessionInfoType, AUTOMATIC, PDUR_APPL_DATA) TxSessionInfoPtr
);


/**
 ** \brief Generic AUTOSAR 3.2 wrapper function for Up_TpTxConfirmation
 **
 ** Generic TxConfirmation function called within generated TxConfirmation wrapper function
 ** that wraps to an AUTOSAR 3.2 upper layer module with TP interface.
 **
 ** \param[in]  TxPduId             Tx-PDU ID of LoTp Module.
 ** \param[in]  Result              Result of the TP transmission.
 ** \param[in]  TxConfirmationFp    Function pointer to TxConfirmation function of AUTOSAR 3.2
 **                                 upper layer module.
 ** \param[in,out] TxSessionInfoPtr  Pointer to the session data of this TxPduId.
 **/
extern FUNC(void, PDUR_CODE) PduR_WrapASR32TxConfirmation
(
  PduIdType TxPduId,
  NotifResultType Result,
  PduR_TpTxConfFpType TxConfirmationFp,
  P2VAR(PduR_WrapASR32TxSessionInfoType, AUTOMATIC, PDUR_APPL_DATA) TxSessionInfoPtr
);

#if (TS_MERGED_COMPILE == STD_OFF)
#define PDUR_STOP_SEC_CODE
#include <PduR_MemMap.h>
#endif
/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef PDUR_WRAPASR32_H */
/*==================[end of file]===========================================*/
