/**
 * \file
 *
 * \brief AUTOSAR LdCom
 *
 * This file contains the implementation of the AUTOSAR
 * module LdCom.
 *
 * \version 1.0.26
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef LDCOM_CBK_H
#define LDCOM_CBK_H


/*==================[inclusions]============================================*/
#include <TSAutosar.h>          /* EB specific standard types */
#include <ComStack_Types.h>

/*==========================================================================*/
#include <LdCom_Cfg.h>           /* Module configurable parameters */
#include <LdCom_Version.h>       /* Module Version Info */

/*==================[macros]================================================*/

/*==================[Error codes]===========================================*/

/*==================[macro checks]==========================================*/

/*==================[type definitions]======================================*/

/* !LINKSTO LdCom.ASR42.SWS_LDCOM_00038,1 */
typedef P2FUNC(BufReq_ReturnType, LDCOM_RTE_CODE, LdCom_RteCbkStartOfReceptionFpType)
(
  PduLengthType SduLength,
  P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_DATA) RxBufferSizePtr
);

/* !LINKSTO LdCom.ASR42.SWS_LDCOM_00039,1 */
typedef P2FUNC(BufReq_ReturnType, LDCOM_RTE_CODE, LdCom_RteCbkCopyRxDataFpType)
(
  P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) SduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_DATA) RxBufferSizePtr
);

/* !LINKSTO LdCom.ASR42.SWS_LDCOM_00040,1 */
typedef P2FUNC(void, LDCOM_RTE_CODE, LdCom_RteCbkTpRxIndicationFpType)
(
  NotifResultType Result
);

/* !LINKSTO LdCom.ASR42.SWS_LDCOM_00036,1 */
typedef P2FUNC(BufReq_ReturnType, LDCOM_RTE_CODE, LdCom_RteCbkCopyTxDataFpType)
(
  P2VAR(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) SduInfoPtr,
  P2VAR(RetryInfoType, AUTOMATIC, LDCOM_APPL_DATA) RetryInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_DATA) TxDataCntPtr
);

/* !LINKSTO LdCom.ASR42.SWS_LDCOM_00037,1 */
typedef P2FUNC(void, LDCOM_RTE_CODE, LdCom_RteCbkTpTxConfirmationFpType)
(
    NotifResultType Result
);

/* !LINKSTO LdCom.ASR42.SWS_LDCOM_00041,1 */
typedef P2FUNC(void, LDCOM_RTE_CODE, LdCom_RteCbkRxIndicationFpType)
(
  P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) PduInfoPtr
);

/* !LINKSTO LdCom.ASR42.SWS_LDCOM_00042,1 */
typedef P2FUNC(Std_ReturnType, LDCOM_RTE_CODE, LdCom_RteCbkTriggerTransmitFpType)
(
  P2VAR(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) PduInfoPtr
);

/* !LINKSTO LdCom.ASR42.SWS_LDCOM_00053,1 */
typedef P2FUNC(void, LDCOM_RTE_CODE, LdCom_RteCbkTxConfirmationFpType)
(
    void
);

/* Type definition comprising all Rx APIs related to transport protocol modules */
typedef struct
{
  LdCom_RteCbkStartOfReceptionFpType StartOfRecFp; /* Pointer to target function */
  LdCom_RteCbkCopyRxDataFpType CopyRxDataFp; /* Pointer to target function */
  LdCom_RteCbkTpRxIndicationFpType TpRxIndFp; /* Pointer to target function */
} LdCom_ApiTpRxType;

/* Type definition comprising all Tx APIs related to transport protocol modules */
typedef struct
{
  LdCom_RteCbkCopyTxDataFpType CopyTxDataFp; /* Pointer to target function */
  LdCom_RteCbkTpTxConfirmationFpType TpTxConfFp; /* Pointer to target function */
} LdCom_ApiTpTxType;

/* Type definition comprising all Rx APIs related to communication interface modules */
typedef struct
{
  LdCom_RteCbkRxIndicationFpType RxIndFp; /* Pointer to target function */
} LdCom_ApiIfRxType;

/* Type definition comprising all Tx APIs related to communication interface modules */
typedef struct
{
  LdCom_RteCbkTriggerTransmitFpType TrigTxFp; /* Pointer to target function */
  LdCom_RteCbkTxConfirmationFpType TxConfFp; /* Pointer to target function */
} LdCom_ApiIfTxType;


/*==================[external function declarations]========================*/

#define LDCOM_START_SEC_CODE
#include <LdCom_MemMap.h>

/**
 ** \brief This function is called at the start of receiving an I-PDU.
 **
 ** This function will be called by the transport protocol module at the start of receiving
 ** an I-PDU. The I-PDU might be fragmented into multiple N-PDUs (FF with one or
 ** more following CFs) or might consist of a single N-PDU (SF).
 ** The service shall provide the currently available maximum buffer size when
 ** invoked with TpSduLength equal to 0.
 **
 ** \param[in]  Id                Identification of the received I-PDU.
 ** \param[in]  TpSduLength       Total length of the PDU to be received.
 ** \param[out] BufferSizePtr     Available receive buffer in the receiving module.
 **                               This parameter will be used to compute the Block Size (BS) in
 **                               the transport protocol module.
 **
 ** \return Result of buffer request
 ** \retval BUFREQ_OK        Connection has been accepted.
 **                          BufferSizePtr indicates the available receive buffer.
 **                          Reception is continued. If no buffer of the requested
 **                          size is available, a receive buffer size of 0 shall be indicated
 **                          by BufferSizePtr.
 ** \retval BUFREQ_E_OVFL    No Buffer of the required length can be provided. Reception is aborted.
 **                          BufferSizePtr remains unchanged.
 ** \retval BUFREQ_E_NOT_OK  Connection has been rejected. Reception is aborted.
 **                          BufferSizePtr remains unchanged.
 **
 ** \ServiceID{0x46}
 ** \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId.}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(BufReq_ReturnType, LDCOM_CODE) LdCom_StartOfReception
(
  PduIdType Id,
  PduLengthType TpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_DATA) BufferSizePtr
);


/**
 ** \brief This function is called to provide the received data of an I-PDU segment (N-PDU)
 ** to the upper layer.
 **
 ** This function is called when a transport protocol module has data to copy for the
 ** receiving module. Several calls may be made during one transportation of an I-PDU.
 ** The service shall provide the currently available buffer size when invoked with
 ** Info.SduLength equal to 0.
 **
 ** \param[in]  Id                Identification of the received I-PDU.
 ** \param[in]  Info              Provides the source buffer (SduDataPtr) and the number of
 **                               bytes to be copied (SduLength). An SduLength of 0 can be used
 **                               to query the current amount of available buffer in the upper
 **                               layer module. In this case, the SduDataPtr may be a NULL_PTR.
 ** \param[out] BufferSizePtr     Available receive buffer after data has been copied.
 **
 ** \return Result of buffer request
 ** \retval BUFREQ_OK        Data copied successfully.
 ** \retval BUFREQ_E_NOT_OK  Data was not copied because an error occurred.
 **
 ** \ServiceID{0x44}
 ** \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId.}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(BufReq_ReturnType, LDCOM_CODE) LdCom_CopyRxData
(
  PduIdType Id,
  P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) Info,
  P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_DATA) BufferSizePtr
);


/**
 ** \brief This function is called after an I-PDU has been received via the TP API.
 **
 ** This service is called by the transport protocol module after an I-PDU has been received
 ** successfully or when an error occurred. It is also used to confirm cancellation of an I-PDU.
 **
 ** \param[in]  Id       Identification of the received I-PDU.
 ** \param[in]  Result   Result of the reception.
 **
 ** \ServiceID{0x45}
 ** \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId.}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(void, LDCOM_CODE) LdCom_TpRxIndication
(
  PduIdType Id,
  NotifResultType Result
);


/**
 ** \brief This function is called to acquire the transmit data of an I-PDU segment (N-PDU).
 **
 ** Each call to this function provides the next part of the I-PDU data unless
 ** Retry->TpDataState is TP_DATARETRY. In this case the function restarts to copy
 ** the data beginning at the offset from the current position indicated by Retry->TxTpDataCnt.
 ** The size of the remaining data is written to the position indicated by AvailableDataPtr.
 **
 ** \param[in]  Id             Identification of the transmitted I-PDU.
 ** \param[out] Info           Provides the destination buffer (SduDataPtr) and the number of bytes to
 **                            be copied (SduLength). If not enough transmit data is available, no data
 **                            is copied by the upper layer module and BUFREQ_E_BUSY is returned.
 **                            The lower layer module may Retry the call. An SduLength of 0 can
 **                            be used to indicate state changes in the Retry parameter or to query
 **                            the current amount of available data in the upper layer module. In this
 **                            case, the SduDataPtr may be a NULL_PTR.
 ** \param[in]  Retry          This parameter is used to acknowledge transmitted data or to retransmit data
 **                            after transmission problems. If the Retry parameter is a NULL_PTR, it
 **                            indicates that the transmit data can be removed from the buffer immediately
 **                            after it has been copied. Otherwise, the Retry parameter must point to
 **                            a valid RetryInfoType element. If TpDataState indicates TP_CONFPENDING, the
 **                            previously copied data must remain in the TP buffer to be available for error
 **                            recovery. TP_DATACONF indicates that all data that has been copied before this
 **                            call is confirmed and can be removed from the TP buffer. Data copied by this
 **                            API call is excluded and will be confirmed later. TP_DATARETRY indicates that
 **                            this API call shall copy previously copied data in order to recover from an
 **                            error. In this case TxTpDataCnt specifies the offset in bytes from the current
 **                            data copy position.
 ** \param[out] AvailableDataPtr   Indicates the remaining number of bytes that are available in the upper
 **                                layer module's Tx buffer. AvailableDataPtr can be used by TP modules that
 **                                support dynamic payload lengths (e.g. FrIsoTp) to determine the size of the
 **                                following CFs.
 **
 ** \return Result of buffer request
 ** \retval BUFREQ_OK        Data has been copied to the transmit buffer completely as requested.
 ** \retval BUFREQ_E_BUSY    Request could not be fulfilled, because the required amount of
 **                          Tx data is not available. The lower layer module may retry this
 **                          call later on. No data has been copied.
 ** \retval BUFREQ_E_NOT_OK  Data has not been copied. Request failed.
 **
 ** \ServiceID{0x43}
 ** \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId.}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(BufReq_ReturnType, LDCOM_CODE) LdCom_CopyTxData
(
  PduIdType Id,
  P2VAR(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) Info,
  P2VAR(RetryInfoType, AUTOMATIC, LDCOM_APPL_DATA) Retry,
  P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_DATA) AvailableDataPtr
);


/**
 ** \brief This function is called after the I-PDU has been transmitted on its network.
 **
 ** This service is called by a transport protocol module after the I-PDU has been
 ** transmitted on its network, the result indicates whether the transmission was successful or not.
 **
 ** \param[in]  Id       Identification of the transmitted I-PDU.
 ** \param[in]  Result   Result of the transmission of the I-PDU.
 **
 ** \ServiceID{0x48}
 ** \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId.}
 ** \Synchronicity{Synchronous }
 **/
extern FUNC(void, LDCOM_CODE) LdCom_TpTxConfirmation
(
  PduIdType Id,
  NotifResultType Result
);


/**
 ** \brief  This function indicates a received I-PDU from a lower layer communication interface
 ** module.
 **
 ** \param[in]  RxPduId      ID of the received I-PDU.
 ** \param[in]  PduInfoPtr   Contains the length (SduLength) of the received I-PDU and a pointer to a
 **                          buffer (SduDataPtr) containing the I-PDU.
 **
 ** \ServiceID{0x42}
 ** \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId.}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(void, LDCOM_CODE) LdCom_RxIndication
(
  PduIdType RxPduId,
  P2VAR(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) PduInfoPtr
);


/**
 ** \brief This function requests the buffer of the SDU for transmission from the upper layer
 ** module.
 **
 ** Within this API, the upper layer module (called module) shall copy its data into the
 ** buffer provided by PduInfoPtr->SduDataPtr and update the length of the actual copied
 ** data in PduInfoPtr->SduLength.
 **
 ** \param[in]  TxPduId      ID of the SDU that is requested to be transmitted.
 ** \param[in]  PduInfoPtr   Contains a pointer to a buffer (SduDataPtr) to where the SDU data shall be copied.
 **                          On return, the service will indicate the length of the copied SDU data in SduLength.
 **
 ** \return Function execution success status
 ** \retval E_OK        SDU has been copied and SduLength indicates the number of copied bytes.
 ** \retval E_NOT_OK    No SDU has been copied. PduInfoPtr must not be used since it may contain a NULL pointer
 **                     or point to invalid data.
 **
 ** \ServiceID{0x41}
 ** \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId.}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, LDCOM_CODE) LdCom_TriggerTransmit
(
  PduIdType TxPduId,
  P2VAR(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) PduInfoPtr
);


/**
 ** \brief The lower layer communication interface module confirms the transmission of an I-PDU.
 **
 ** \param[in]  TxPduId   ID of the I-PDU that has been transmitted.
 **
 ** \ServiceID{0x40}
 ** \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId.}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(void, LDCOM_CODE) LdCom_TxConfirmation
(
  PduIdType TxPduId
);


#define LDCOM_STOP_SEC_CODE
#include <LdCom_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef LDCOM_CBK_H */
/*==================[end of file]===========================================*/
