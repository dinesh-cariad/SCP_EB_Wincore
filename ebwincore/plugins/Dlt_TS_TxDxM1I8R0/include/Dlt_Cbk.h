/**
 * \file
 *
 * \brief AUTOSAR Dlt
 *
 * This file contains the implementation of the AUTOSAR
 * module Dlt.
 *
 * \version 1.8.6
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef DLT_CBK_H
#define DLT_CBK_H

/* This file contains or includes all external identifiers of
 * the AUTOSAR module Dlt. */

/*==================[inclusions]============================================*/

#include <ComStack_Types.h>                    /* Communication Stack types */

#include <Dlt_Cfg.h>

#if (DLT_IMPLEMENT_NV_RAM_STORAGE == STD_ON)
#include <NvM_Types.h>                                         /* NvM types */
#endif

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

typedef struct
{
  /** \brief Holds the index of valid data in bytes to be transmitted from the current message.
   *         This also represents the amount of data which has already been copied to the lower
   *         layer from the current message, though not confirmed.
   */
  PduLengthType PositionInMessage;

  /** \brief Holds the index of confirmed data in bytes to be transmitted from this message.
   *         This also represents the amount of data which has already been copied to the lower
   *         layer from the current message and the transmission of which has been confirmed
   *         by the lower layer.
   */
  PduLengthType ConfirmedPositionInMessage;
} Dlt_TxConnectionType;

/*==================[external function declarations]========================*/

#define DLT_START_SEC_CODE
#include <Dlt_MemMap.h>

extern FUNC(void, DLT_CODE) Dlt_ComRxIndication(
    PduIdType DltRxPduId,
    NotifResultType Result);

extern FUNC(void, DLT_CODE) Dlt_ComTxConfirmation(
    PduIdType DltTxPduId,
    NotifResultType Result);

/** \brief Copy transmit data to PduR buffer
 **
 ** This function is called to acquire the transmit data of an I-PDU segment
 ** (N-PDU). Each call to this function provides the next part of the I-PDU
 ** data unless retry->TpDataState is TP_DATARETRY. In this case, the function
 ** restarts to copy the data beginning at the offset from the current position
 ** indicated by retry->TxTpDataCnt. The size of the remaining data is written
 ** to the position indicated by availableDataPtr.
 **
 ** \param[in] id Identification of the transmitted I-PDU
 ** \param[in] info Provides the destination buffer (SduDataPtr) and the
 **            number of bytes to be copied (SduLength).
 **            If not enough transmit data is available, no data is
 **            copied by the upper layer module and BUFREQ_E_BUSY is
 **            returned. The lower layer module may retry the call.
 **            An Sdulength of 0 can be used to indicate state changes
 **            in the retry parameter or to query the current amount
 **            of available data in the upper layer module. In this case,
 **            the SduDataPtr may be a NULL_PTR.
 ** \param[in] retry This parameter is used to acknowledge transmitted data
 **            or to retransmit data after transmission problems.
 **            If the retry parameter is a NULL_PTR, it indicates that
 **            the transmitted data can be removed from the buffer
 **            immediately after it has been copied. Otherwise, the
 **            retry parameter must point to a valid RetryInfoType
 **            element.
 **            If TpDataState indicates TP_CONFPENDING, the previously
 **            copied data must remain in the TP buffer to be available
 **            for error recovery.
 **            TP_DATACONF indicates that all data that has been copied
 **            before this call is confirmed and can be removed from
 **            the TP buffer. Data copied by this API call is excluded
 **            and will be confirmed later.
 **            TP_DATARETRY indicates that this API call shall copy
 **            previously copied data in order to recover from an
 **            error. In this case TxTpDataCnt specifies the offset in
 **            bytes from the current data copy position.
 ** \param[out] availableDataPtr Indicates the remaining number of bytes
 **             that are available in the upper layer module's Tx buffer.
 **             availableDataPtr can be used by TP modules that support
 **             dynamic payload lengths (e.g. FrIso Tp) to determine the
 **             size of the following CFs.
 **
 ** \return The success state of Dlt_ComCopyTxData
 ** \retval BUFREQ_OK Data has been copied to the transmit buffer
 **         completely as requested.
 ** \retval BUFREQ_E_BUSY Request could not be fulfilled, because the
 **         required amount of Tx data is not available. The lower layer
 **         module may retry this call later on. No data has been copied.
 ** \retval BUFREQ_E_NOT_OK Data has not been copied. Request failed.
 ** \ServiceID{::DLT_SID_ComCopyTxData}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 **/
/* !LINKSTO Dlt.ImportedTypes,1 */
extern FUNC(BufReq_ReturnType, DLT_CODE) Dlt_ComCopyTxData(
    PduIdType id,
    P2VAR(PduInfoType, AUTOMATIC, DLT_APPL_DATA) info,
    P2VAR(RetryInfoType, AUTOMATIC, DLT_APPL_DATA) retry,
    P2VAR(PduLengthType, AUTOMATIC, DLT_APPL_DATA) availableDataPtr);

/** \brief Trigger the reception of a message
 **
 ** This function is called at the start of receiving an N_SDU. The N-SDU
 ** might be fragmented into multiple N-PDUs (FF with one or more following
 ** CFs) or might consist of a single N-PDU (SF).
 **
 ** \param[in] id Identification of the I-PDU.
 ** \param[in] TpSduLength Total length of the N-SDU to be received.
 ** \param[out] bufferSizePtr Available receive buffer in the receiving
 **             module. This parameter will be used to compute the Block Size
 **             (BS) in the transport protocol module.
 **
 ** \return The success state of Dlt_ComStartofReception
 ** \retval BUFREQ_OK Connection has been accepted. bufferSizePtr indicates
 **         the available receive buffer; reception is continued. If no
 **         buffer of the requested size is available, a receive buffer
 **         size 0 shall be indicated by bufferSizePtr.
 ** \retval BUFREQ_E_NOT_OK Connection has been rejected; reception is
 **         aborted. bufferSizePtr remains unchanged.
 ** \retval BUFREQ_E_OVFL No buffer of the required length can be provided;
 **         reception is aborted. bufferSizePtr remains unchanged.
 **
 ** \ServiceID{::DLT_SID_ComStartOfReception}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(BufReq_ReturnType, DLT_CODE) Dlt_ComStartOfReception(
    PduIdType id,
    PduLengthType TpSduLength,
    P2VAR(PduLengthType, AUTOMATIC, DLT_APPL_DATA) bufferSizePtr);

/** \brief Copy received data from PduR buffer
 **
 ** This function is called to provide the received data of an I-PDU
 ** segment (N-PDU) to the upper layer.
 ** Each call to this function provides the next part of the I-PDU data.
 ** The size of the remaining data is written to the position indicated
 ** by bufferSizePtr.
 **
 ** \param[in] id Identification of the received I-PDU.
 ** \param[in] info Provides the source buffer (SduDataPtr) and the number
 **            of bytes to be copied (SduDataLength).
 **            An SduLength of 0 can be used to query the current amount
 **            of available buffer in the upper layer module. In this
 **            case, the SduDataPtr may be a NULL_PTR.
 ** \param[out] bufferSizePtr Available receive buffer after data has been
 **             copied
 **
 ** \return The success state of Dlt_ComCopyRxData
 ** \retval BUFREQ_OK Data copied successfully
 ** \retval BUFREQ_E_NOT_OK Data was not copied because an error occurred.
 **
 ** \ServiceID{::DLT_SID_ComCopyRxData}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(BufReq_ReturnType, DLT_CODE) Dlt_ComCopyRxData(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, DLT_APPL_DATA) info,
    P2VAR(PduLengthType, AUTOMATIC, DLT_APPL_DATA) bufferSizePtr);
#if (DLT_IMPLEMENT_NV_RAM_STORAGE == STD_ON)

/** \brief Callback function from NvM for native data block
 ** to request the data to be copied from the NvM RAM during
 ** NvM_ReadBlock().
 **
 ** This callback function shall copy information stored in the native
 ** NvM block (runtime configurable variables) to the Dlt RAM.
 **
 ** The approach used in this case is to copy the information one by one
 ** thus not using TS_MemCpy and more resources shall be saved, because only
 ** data is copied directly from NvM buffer to Dlt runtime variables without using
 ** additional buffers in Dlt.
 **
 ** \param[in] NvMBuffer  Pointer to NvM RAM mirror
 **
 ** \return always E_OK
 **
 ** \ServiceID{::DLT_SID_InternalReadFromNativeApiId}
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, DLT_CODE) Dlt_NvMReadRamBlockFromNvMNativeCbk(
  P2CONST(void, AUTOMATIC, NVM_APPL_DATA) NvMBuffer);

/** \brief Callback function from NvM for data set block to request the data to be
 ** copied from NvM RAM during NvM_ReadBlock().
 **
 ** This callback function shall copy information stored in the data set
 ** NvM block (the values of log level and trace status explicitly set by
 ** a pair of ApplicationID and ContextID registered via Dlt_RegisterContext)
 ** to the Dlt RAM.
 **
 ** The approach used in this case is to copy the information one by one
 ** thus not using TS_MemCpy and more resources shall be saved, because only data
 ** is copied directly from NvM buffer to Dlt runtime variables without using
 ** additional buffers in Dlt.
 **
 ** \param[in] NvMBuffer  Pointer to NvM RAM mirror
 **
 ** \return always E_OK
 **
 ** \ServiceID{::DLT_SID_InternalReadFromDataSetApiId}
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, DLT_CODE) Dlt_NvMReadRamBlockFromNvMDataSetCbk(
  P2CONST(void, AUTOMATIC, NVM_APPL_DATA) NvMBuffer);
/** \brief Callback function from NvM for data set block to request the data to be
 ** copied from Dlt module's RAM into non-volatile RAM during NvM_WriteBlock().
 **
 ** This callback function shall copy information stored in Dlt module's RAM
 ** (the values of log level and trace status explicitly set by an external client for
 ** a pair of ApplicationID and ContextID registered via Dlt_RegisterContext)
 **
 ** The approach used in this case is to copy the information one by one
 ** and not using TS_MemCpy thus more resources shall be saved, because
 ** no auxiliary buffer is used to sore the data to be copied.
 **
 ** \param[in] NvMBuffer  Pointer to NvM RAM mirror
 **
 ** \return always E_OK
 **
 ** \ServiceID{::DLT_SID_InternalWriteToDataSetApiId}
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, DLT_CODE) Dlt_NvMWriteRamBlockToNvMDataSetCbk(
  P2VAR(void, AUTOMATIC, NVM_APPL_DATA) NvMBuffer);
/** \brief Callback function from NvM for native data block
 ** to request data to be copied into NvM RAM during
 ** NvM_WriteBlock().
 **
 ** This callback function shall copy information (the runtime configurable variables)
 ** from Dlt module's RAM in the native NvM block.
 **
 ** The approach used it this case is to copy the information one by one
 ** and not using TS_MemCpy thus resources are saved, because no
 ** auxiliary buffer is used.
 **
 ** \param[in] NvMBuffer  Pointer to NvM RAM mirror
 **
 ** \return always E_OK
 **
 ** \ServiceID{::DLT_SID_InternalWriteToNativeApiId}
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, DLT_CODE) Dlt_NvMWriteRamBlockToNvMNativeCbk(
  P2VAR(void, AUTOMATIC, NVM_APPL_DATA) NvMBuffer);
/** \brief Callback-Function from NvM that will be called if no ROM data
 ** is available for initialization of the NVRAM block.
 ** \return returns E_OK
 **
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, DLT_CODE) Dlt_NvMInitDataSetBlockCbk(void);
/** \brief Callback-Function from NvM that will be called if no ROM data
 ** is available for initialization of the NVRAM block.
 ** \return returns E_OK
 **
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, DLT_CODE) Dlt_NvMInitNativeBlockCbk(void);
/** \brief Callback-Function from NvM for general data data set block
 ** notifying successful completion of single block request
 **
 ** \param[in] ServiceId
 ** \param[in] JobResult
 **
 ** \return returns always E_OK
 **
 ** \ServiceID{::DLT_SID_NvMSingleBlockCallbackNative}
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, DLT_CODE) Dlt_NvMSingleBlockCallbackNative(uint8 ServiceId, NvM_RequestResultType JobResult);

/** \brief Callback-Function from NvM for data set block information
 ** notifying successful completion of single block request.
 **
 ** \param[in] ServiceId
 ** \param[in] JobResult
 **
 ** \return returns always E_OK
 **
 ** \ServiceID{::DLT_SID_SingleBlockCallbackDataSet}
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, DLT_CODE) Dlt_NvMSingleBlockCallbackDataSet(
  uint8                 ServiceId,
  NvM_RequestResultType JobResult);
#endif/*if (DLT_IMPLEMENT_NV_RAM_STORAGE == STD_ON)*/
#define DLT_STOP_SEC_CODE
#include <Dlt_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef DLT_CBK_H */
/*==================[end of file]===========================================*/
