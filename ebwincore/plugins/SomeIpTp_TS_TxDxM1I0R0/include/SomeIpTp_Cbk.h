/**
 * \file
 *
 * \brief AUTOSAR SomeIpTp
 *
 * This file contains the implementation of the AUTOSAR
 * module SomeIpTp.
 *
 * \version 1.0.28
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef SOMEIPTP_CBK_H
#define SOMEIPTP_CBK_H

/*==================[inclusions]============================================*/

#include <SomeIpTp_Types.h>       /* SomeIpTp specific types */

/*==================[macros]================================================*/

/** \brief Defines API id of function SomeIpTp_TxConfirmation(). */
#if (defined SOMEIPTP_SID_TXCONFIRMATION)
#error SOMEIPTP_SID_TXCONFIRMATION already defined!
#endif /* #if (defined SOMEIPTP_SID_TXCONFIRMATION)*/
#define SOMEIPTP_SID_TXCONFIRMATION 0x40U

/** \brief Defines API id of function SomeIpTp_TriggerTransmit(). */
#if (defined SOMEIPTP_SID_TRIGGERTRANSMIT)
#error SOMEIPTP_SID_TRIGGERTRANSMIT already defined!
#endif /* #if (defined SOMEIPTP_SID_TRIGGERTRANSMIT)*/
#define SOMEIPTP_SID_TRIGGERTRANSMIT 0x41U

/** \brief Defines API id of function SomeIpTp_RxIndication(). */
#if (defined SOMEIPTP_SID_RXINDICATION)
#error SOMEIPTP_SID_RXINDICATION already defined!
#endif /* #if (defined SOMEIPTP_SID_RXINDICATION)*/
#define SOMEIPTP_SID_RXINDICATION 0x42U

/** \brief Defines Dummy API id of function SomeIpTp_PduRTpTxConfirmation(). */
#if (defined SOMEIPTP_SID_PDURTPTXCONFIRMATION)
#error SOMEIPTP_SID_PDURTPTXCONFIRMATION already defined!
#endif /* #if (defined SOMEIPTP_SID_PDURTPTXCONFIRMATION)*/
#define SOMEIPTP_SID_PDURTPTXCONFIRMATION 0xFE

/** \brief Defines Dummy API id of function SomeIpTp_PduRTpCopyTxData(). */
#if (defined SOMEIPTP_SID_PDURTPCOPYTXDATA)
#error SOMEIPTP_SID_PDURTPCOPYTXDATA already defined!
#endif /* #if (defined SOMEIPTP_SID_PDURTPCOPYTXDATA)*/
#define SOMEIPTP_SID_PDURTPCOPYTXDATA 0xFD

/** \brief Defines Dummy API id of function SomeIpTp_PduRTpStartOfReception(). */
#if (defined SOMEIPTP_SID_PDURTPSTARTOFRECEPTION)
#error SOMEIPTP_SID_PDURTPSTARTOFRECEPTION already defined!
#endif /* #if (defined SOMEIPTP_SID_PDURTPSTARTOFRECEPTION)*/
#define SOMEIPTP_SID_PDURTPSTARTOFRECEPTION 0xFC

/** \brief Defines Dummy API id of function SomeIpTp_PduRTpCopyRxData(). */
#if (defined SOMEIPTP_SID_PDURTPCOPYRXDATA)
#error SOMEIPTP_SID_PDURTPCOPYRXDATA already defined!
#endif /* #if (defined SOMEIPTP_SID_PDURTPCOPYRXDATA)*/
#define SOMEIPTP_SID_PDURTPCOPYRXDATA 0xFB

/** \brief Defines Dummy API id of function SomeIpTp_PduRTpRxIndication(). */
#if (defined SOMEIPTP_SID_PDURTPRXINDICATION)
#error SOMEIPTP_SID_PDURTPRXINDICATION already defined!
#endif /* #if (defined SOMEIPTP_SID_PDURTPRXINDICATION)*/
#define SOMEIPTP_SID_PDURTPRXINDICATION 0xFA

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define SOMEIPTP_START_SEC_CODE
#include <SomeIpTp_MemMap.h>

/**
 * \brief SomeIpTp module Transmission confirmation callback function.
 *
 * This callback function is called by the lower interface module to confirm the transmission
 * of a PDU if succeded or failed.
 *
 * \param[in] TxPduId - ID of the PDU that has been transmitted.
 *
 * \retval void
 *
 * \ServiceID{0x40}
 * \Reentrancy{  Reentrant for different PduIds
 *               Non Reentrant for the same PduId  }
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, SOMEIPTP_CODE) SomeIpTp_TxConfirmation
(
  PduIdType TxPduId
);

/**
 * \brief SomeIpTp module trigger transmit callback function.
 *
 * This callback is called by the upper layer to check whether the available data fits
 * into the buffer size
 * If it fits, it shall copy its data into the buffer and update the length of the actual
 * copied data
 *
 * \param[in] TxPduId - ID of the SDU that is requested to be transmitted.
 * \param[inout] PduInfoPtr - pointer to a buffer to where the SDU data shall be copied,
 *               and the available buffer size in SduLengh.
 *               On return, the service will indicate the length of
 *               the copied SDU data in SduLength.
 *
 * \retval Std_ReturnType E_OK if SDU has been copied.
 *                        E_NOT_OK if not copied.
 *
 * \ServiceID{0x41}
 * \Reentrancy{  Reentrant for different PduIds
 *               Reentrant for the same PduId  }
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, SOMEIPTP_CODE) SomeIpTp_TriggerTransmit
(
  PduIdType TxPduId,
  P2VAR(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr
);

/**
 * \brief SomeIpTp module reception indication callback function.
 *
 * This callback function is called to indicate a received PDU from a lower layer communication
 * interface module
 *
 * \param[in] RxPduId - ID received PDU.
 * \param[in] PduInfoPtr - pointer to a buffer containing the PDU data, and
 *                         length of the received PDU.
 *
 * \retval void
 *
 * \ServiceID{0x42}
 * \Reentrancy{  Reentrant for different PduIds
 *               Non Reentrant for the same PduId  }
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, SOMEIPTP_CODE) SomeIpTp_RxIndication
(
  PduIdType RxPduId,
  P2VAR(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr
);

/*------------------[Dummy PduR functions]-----------------*/
/**
 * \brief Dummy funtion for PduR.
 *
 * This function shall not be called, and if so it shall report developement error.
 *
 * \ServiceID{0xFE}
 */
extern FUNC(void, SOMEIPTP_CODE ) SomeIpTp_PduRTpTxConfirmation
(
    PduIdType PduId,
    NotifResultType Result
);

/**
 * \brief Dummy funtion for PduR.
 *
 * This function shall not be called, and if so it shall report developement error.
 *
 * \ServiceID{0xFD}
 */
extern FUNC(BufReq_ReturnType, SOMEIPTP_CODE) SomeIpTp_PduRTpCopyTxData
(
   PduIdType PduId,
   P2VAR(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr,
   P2VAR(RetryInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) RetryInfoPtr,
   P2VAR(PduLengthType, AUTOMATIC, SOMEIPTP_APPL_DATA) TxDataCntPtr
);

/**
 * \brief Dummy funtion for PduR.
 *
 * This function shall not be called, and if so it shall report developement error.
 *
 * \ServiceID{0xFC}
 */
extern FUNC(BufReq_ReturnType, SOMEIPTP_CODE) SomeIpTp_PduRTpStartOfReception
(
   PduIdType PduId,
   PduLengthType PduLength,
   P2VAR(PduLengthType, AUTOMATIC, SOMEIPTP_APPL_DATA) RxBufferSizePtr
);

/**
 * \brief Dummy funtion for PduR.
 *
 * This function shall not be called, and if so it shall report developement error.
 *
 * \ServiceID{0xFB}
 */
extern FUNC(BufReq_ReturnType, SOMEIPTP_CODE) SomeIpTp_PduRTpCopyRxData
(
   PduIdType PduId,
   P2CONST(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPointer,
   P2VAR(PduLengthType, AUTOMATIC, SOMEIPTP_APPL_DATA) RxBufferSizePtr
);

/**
 * \brief Dummy funtion for PduR.
 *
 * This function shall not be called, and if so it shall report developement error.
 *
 * \ServiceID{0xFA}
 */
extern FUNC(void, SOMEIPTP_CODE) SomeIpTp_PduRTpRxIndication
(
    PduIdType RxPduId,
    NotifResultType Result
);


#define SOMEIPTP_STOP_SEC_CODE
#include <SomeIpTp_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef SOMEIPTP_CBK_H */
/*==================[end of file]===========================================*/
