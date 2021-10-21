/**
 * \file
 *
 * \brief AUTOSAR IpduM
 *
 * This file contains the implementation of the AUTOSAR
 * module IpduM.
 *
 * \version 3.3.40
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef IPDUM_CBK_H
#define IPDUM_CBK_H

/*==================[inclusions]============================================*/
#include <ComStack_Types.h>
#include <IpduM_Version.h>

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/
#define IPDUM_START_SEC_CODE
#include <IpduM_MemMap.h>

/** \brief Receive indication callback function
 **
 ** This is the receive indication callback function. It de-multiplex the incoming
 ** PDU and calls the corresponding upper layer receive indication callback function
 **
 ** \param[in]  RxPduId      ID of I-PDU that has been received.
 ** \param[in]  PduInfoPtr   Contains the length (SduLength) of the received I-PDU and a pointer to
 **                          a buffer (SduDataPtr) containing the I-PDU.
 **
 ** \ServiceID{0x42}
 ** \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId}
 ** \Synchronicity{synchronous}
 **
 */
extern FUNC(void, IPDUM_CODE) IpduM_RxIndication
(
  PduIdType RxPduId,
  P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr
);

/** \brief Copy data to PDU-router memory
 **
 ** The lower layer communication module requests the buffer of the SDU for transmission from the
 ** upper layer module.
 **
 ** \param[in]  TxPduId      ID of IpduM I-PDU that is requested to be transmitted by IpduM
 ** \param[in]  PduInfoPtr   Contains a pointer to a buffer (SduDataPtr) to where the SDU shall be
 **                          copied to. On return, the service will indicate the length of the
 **                          copied SDU data in SduLength.
 **
 ** \retval E_OK      SDU has been copied and SduLength indicates the number of copied bytes.
 ** \retval E_NOT_OK  No SDU has been copied. PduInfoPtr must not be used since it may contain a
 **                   NULL pointer or point to invalid data.
 **
 ** \ServiceID{0x41}
 ** \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId}
 ** \Synchronicity{synchronous}
 **
 */
extern FUNC(Std_ReturnType, IPDUM_CODE) IpduM_TriggerTransmit
(
  PduIdType TxPduId,
  P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr
);

/** \brief Transmit confirmation callback function
 **
 ** This is the transmit confirmation callback function. It gets the PDU
 ** handle for the transmitted I-PDU, translates it for the upper layer and
 ** then calls the upper layer callback functions configured for this handle.
 **
 ** \param[in]  TxPduId      ID of multiplexed I-PDU that has been transmitted.
 **
 ** \ServiceID{0x40}
 ** \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId}
 ** \Synchronicity{synchronous}
 **
 */
extern FUNC(void, IPDUM_CODE) IpduM_TxConfirmation (PduIdType TxPduId);

#define IPDUM_STOP_SEC_CODE
#include <IpduM_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef IPDUM_CBK_H */
/*==================[end of file]===========================================*/

