/**
 * \file
 *
 * \brief AUTOSAR Dcm
 *
 * This file contains the implementation of the AUTOSAR
 * module Dcm.
 *
 * \version 5.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef DCM_CBK_H
#define DCM_CBK_H

/*==================[inclusions]=================================================================*/

#include <ComStack_Types.h>                                       /* AUTOSAR Com Stack types */

#include <Dcm_Version.h>                                          /* Module version declarations */

#include <Dcm_Types.h>                                            /* DCM type definition */

#include <Dcm_Dsl_ComMChannelStateManager_External_Interface.h>   /* Dcm Dsl CComMChannelStateManager
                                                                     public header file */

/*==================[macros]=====================================================================*/

/*==================[type definitions]===========================================================*/

/** \brief Helper type definition to be used to prevent nested compiler abstraction macros which
 **        may lead to problems for some tool chains. */
typedef P2VAR(PduInfoType, TYPEDEF, DCM_APPL_DATA) Dcm_PduInfoPtrType;

/*==================[external function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* !LINKSTO SWS_Dcm_01066,1 */

/** \brief Called once to initialize the reception of a diagnostic request.
 **
 ** \param[in] DcmRxPduId Identifies the DCM data to be received. This information is used within
 ** the DCM to distinguish two or more receptions at the same time.
 **
 ** \param[in] TpSduLength This length identifies the overall number of bytes to be received.
 **
 ** \param[out] RxBufferSizePtr pointer to PduLengthType containing length of the available buffer.
 **
 ** \return Buffer request status
 ** \retval BUFREQ_OK Buffer request accomplished successful.
 ** \retval BUFREQ_E_NOT_OK Buffer request not successful. Buffer cannot be accessed by TP.
 ** \retval BUFREQ_E_OVFL DCM is not capable to receive the number of TpSduLength Bytes.
 **
 ** \ServiceID{::DCM_SVCID_STARTOFRECEPTION}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(BufReq_ReturnType, DCM_CODE) Dcm_StartOfReception(
  PduIdType DcmRxPduId,
  PduLengthType TpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) RxBufferSizePtr);

/** \brief Called once upon reception of each segment. Within this call, the received data
 **  is copied from the receive TP buffer to the DCM receive buffer.
 **
 ** \param[in] DcmRxPduId Identifies the DCM data to be received. This information is used within
 ** the DCM to distinguish two or more receptions at the same time.
 **
 ** \param[in] PduInfoPtr Pointer to a PduInfoType which indicates the number of bytes to be
 ** copied (SduLength) and the location of the source data (SduDataPtr).
 **
 ** \param[out] RxBufferSizePtr pointer to Remaining free place in receive buffer after
 ** completion of this call.
 **
 ** \return Request data copy status
 ** \retval BUFREQ_OK Data has been copied to the receive buffer completely as requested.
 ** \retval BUFREQ_E_NOT_OK Data has not been copied. Request failed.
 **
 ** \ServiceID{::DCM_SVCID_COPYRXDATA}
 ** \Reentrancy{Reentrant for different PduIds.Non reentrant for the same PduId.}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(BufReq_ReturnType, DCM_CODE) Dcm_CopyRxData(
  PduIdType DcmRxPduId,
  P2CONST(PduInfoType, AUTOMATIC, DCM_APPL_DATA) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) RxBufferSizePtr);

/** \brief Indicate that reception of data is finished
 **
 ** This function is called by the lower layer (in general the PDU Router):
 ** - with Result = ::NTFSRSLT_OK after the complete DCM I-PDU has successfully been received, i.e.
 **   at the very end of the segmented TP receive cycle or after receiving an unsegmented N-PDU.
 ** - with Result = ::NTFSRSLT_NOT_OK it is indicated that an error (e.g. timeout) has occurred
 **   during the reception of the DCM I-PDU. This passes the receive buffer back to DCM and allows
 **   error handling.  It is undefined which part of the buffer contains valid data in this case,
 **   so the DCM shall not evaluate that buffer.  By calling this service only the DCM is allowed
 **   to access the buffer.
 **
 ** \param[in] DcmRxPduId ID of DCM I-PDU that has been received.
 **
 ** \param[in] Result Result of the N-PDU reception
 **
 ** \ServiceID{::DCM_SVCID_TPRXINDICATION}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, DCM_CODE) Dcm_TpRxIndication(
  PduIdType DcmRxPduId,
  NotifResultType Result);

/** \brief This funtion is only a stub to provide the function symbol */
extern FUNC(void, DCM_CODE) Dcm_RxIndication(PduIdType DcmRxPduId,
                                        P2VAR(PduInfoType, AUTOMATIC, DCM_APPL_DATA) PduInfoPtr);

/** \brief Request transmission buffer
 **
 ** At invocation of Dcm_CopyTxData the DCM module copies the requested transmit data with ID PduId
 ** from its internal transmit buffer to the location specified by the PduInfoPtr. The function
 ** Dcm_CopyTxData also calculates and sets the TxDataCntPtr to the amount of remaining bytes for
 ** the transmission of this data.
 ** If RetryInfoPtr is NULL_PTR or if TpDataState is not equal TP_DATARETRY, the Dcm shall always
 ** copy the next fragment of data to the SduDataPtr.
 ** If TpDataState equals TP_DATARETRY, the Dcm shall copy previously copied data again, beginning
 ** from the offset position given in RetryInfoPtr->TxTpDataCnt from the current position.
 **
 ** \param[in] DcmTxPduId Identifies the DCM data to be sent. This information is used to derive
 ** the PCI information within the transport protocol. The value has to be same as in the according
 ** service call PduR_DcmTransmit().
 **
 ** \param[in] PduInfoPtr Pointer to a PduInfoType, which indicates the number of bytes to be copied
 ** (SduLength) and the location where the data have to be copied to (SduDataPtr).
 ** An SduLength of 0 is possible in order to poll the available transmit data count. In this case
 ** no data are to be copied and SduDataPtr might be invalid.
 **
 ** \param[in] RetryInfoPtr If the transmitted TP I-PDU does not support the retry feature a
 ** NULL_PTR can be provided. This indicates that the copied transmit data can be removed from the
 ** buffer after it has been copied.
 **
 ** \param[out] TxDataCntPtr Remaining Tx data after completion of this call
 ** \return Response data copy status
 ** \retval BUFREQ_OK Data has been copied to the transmit buffer completely as requested.
 ** \retval BUFREQ_E_NOT_OK Data has not been copied. Requestfailed,
 **         in case the corresponding I-PDU was stopped.
 ** \retval BUFREQ_E_BUSY Request could not be fulfilled, because
 **         the required amount of Tx data is not available. The lower
 **         layer module may retry this call later on. No data has been
 **         copied.
 **
 ** \ServiceID{::DCM_SVCID_COPYTXDATA}
 ** \Reentrancy{Reentrant for different PduIds.Non reentrant for the same PduId.}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(BufReq_ReturnType, DCM_CODE) Dcm_CopyTxData(
  PduIdType DcmTxPduId,
  P2VAR(PduInfoType, AUTOMATIC, DCM_APPL_DATA) PduInfoPtr,
  P2VAR(RetryInfoType, AUTOMATIC, DCM_APPL_DATA) RetryInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) TxDataCntPtr);

/** \brief Comfirm (un)successfull transmission
 **
 ** This function is called by the lower layer (in general the PDU Router):
 ** - with Result = ::NTFSRSLT_OK after the complete DCM I-PDU has successfully been transmitted,
 ** i.e. at the very end of the segmented TP transmit cycle. Within this function, the DCM shall
 ** unlock the transmit buffer.
 ** - with Result = ::NTFSRSLT_NOT_OK if an error (e.g. timeout) has occurred during the
 ** transmission of the DCM I-PDU. This enables unlocking
 ** of the transmit buffer and error handling.
 **
 ** \param[in] DcmTxPduId ID of DCM I-PDU that has been transmitted.
 **
 ** \param[in] Result Result of the N-PDU transmission.
 **
 ** \ServiceID{::DCM_SVCID_TPTXCONFIRMATION}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, DCM_CODE) Dcm_TpTxConfirmation(PduIdType DcmTxPduId, NotifResultType Result);

/** \brief Comfirm (un)successfull transmission
 **
 ** This function is called by the lower layer (in general the PDU Router). This enables unlocking
 ** of the transmit buffer and error handling.
 **
 ** \param[in] DcmTxPduId ID of DCM I-PDU that has been transmitted.
 **
 ** \ServiceID{::DCM_SVCID_TXCONFIRMATION}
 ** \Reentrancy{Reentrant for different PduIds.Non reentrant for the same PduId.}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, DCM_CODE) Dcm_TxConfirmation(PduIdType DcmTxPduId);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /* ifndef DCM_CBK_H */
/*==================[end of file]================================================================*/
