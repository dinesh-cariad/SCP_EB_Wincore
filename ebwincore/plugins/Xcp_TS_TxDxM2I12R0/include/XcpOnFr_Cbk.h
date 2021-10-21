/**
 * \file
 *
 * \brief AUTOSAR Xcp
 *
 * This file contains the implementation of the AUTOSAR
 * module Xcp.
 *
 * \version 2.12.0
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef XCPONFR_CBK_H
#define XCPONFR_CBK_H

/*==================[includes]===============================================*/

#include <Xcp_Cfg.h>

#if (XCP_ON_FLEXRAY_ENABLED == STD_ON)

#include <ComStack_Types.h>     /* AUTOSAR Com Stack header */

/*==================[macros]=================================================*/

/*------------------[error codes]--------------------------------------------*/

/*------------------[macros for service IDs]---------------------------------*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

#define XCP_START_SEC_CODE
#include <Xcp_MemMap.h>

/** \brief XCP PDU is received
 **
 ** This function is called by the lower layer FrIf when an AUTOSAR XCP PDU has
 ** been received.
 **
 ** Precondition: ::XCP_ON_FLEXRAY_ENABLED = ::STD_ON
 **
 ** \param[in]  XcpRxPduId  PDU-ID that has been received
 ** \param[in]  XcpRxPduPtr  Contains the length (SduLength) of the received I-PDU and a pointer to
 **                          a buffer (SduDataPtr) containing the I-PDU
 **
 ** \ProductionError{XCP_E_PDU_LOST, thrown\, if an XCP message is received
 ** but it is discarded by XCP because it is detected as incorrect.}
 ** \ProductionError{XCP_E_PDU_BUFFER_FULL, thrown\, if an XCP message is received
 ** but it is discarded because the reception buffer is already full.}
 ** \ProductionError{XCP_E_PDU_BUFFER_LOCKED, thrown\, if the callback function
 ** was called for a PDU that is not mapped to the active connection.}
 ** \ProductionError{XCP_E_RESP_CTO_QUEUE_FULL, thrown\, if a command response
 ** or event package cannot be sent to the master because the internal queue is full.}
 ** \ProductionError{XCP_E_STIMULATION_DATA_LOST, thrown\, if new stimulation data is
 ** received before previous data is processed.}
 ** \ProductionError{XCP_E_ACQUISITION_DATA_LOST, thrown\, if the requested acquisition data
 ** exceeds the connection MAX_DTO and cannot fit into a package.}
 **
 ** \ServiceID{::XCP_SID_IF_RX_INDICATION}
 ** \Reentrancy{Reentrant for different XcpRxPduId. Non-reentrant for the same XcpRxPduId}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, XCP_CODE) Xcp_FrIfRxIndication
(
  PduIdType XcpRxPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCP_APPL_DATA) XcpRxPduPtr
);

/** \brief XCP PDU transmit confirmation
 **
 ** This function is called by the lower layer FrIf when an AUTOSAR XCP PDU has
 ** been transmitted.
 **
 ** Precondition: ::XCP_ON_FLEXRAY_ENABLED = ::STD_ON
 **
 ** \param[in]  XcpTxPduId  PDU-ID that has been transmitted
 **
 ** \ProductionError{XCP_E_RETRY_FAILED, thrown\, if the XCP message transmission
 ** (including the configured number of retries) fails.}
 ** \ProductionError{XCP_E_PDU_BUFFER_LOCKED, thrown\, if the callback function
 ** was called for a PDU that is not mapped to the active connection.}
 **
 ** \ServiceID{::XCP_SID_IF_TX_CONFIRMATION}
 ** \Reentrancy{Reentrant for different XcpTxPduId. Non-reentrant for the same XcpTxPduId}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, XCP_CODE) Xcp_FrIfTxConfirmation
(
  PduIdType XcpTxPduId
);

#if (XCP_TRIGGER_TX_API_ENABLED == STD_ON)
/** \brief Call when an AUTOSAR XCP PDU shall be transmitted
 **
 ** This function is called by the lower layer FrIf when an AUTOSAR XCP PDU
 ** shall be transmitted. The function Xcp_FrIfTriggerTransmit is called by
 ** the Flexray Interface for requesting the I-PDU before transmission.
 ** Whether the function Xcp_FrIfTriggerTransmit is called or not is
 ** statically configured for each I-PDU in the configuration.
 **
 ** Precondition: ::XCP_ON_FLEXRAY_ENABLED = ::STD_ON
 **
 ** \param[in]  XcpTxPduId  PDU-ID that has been requested to be transmitted
 ** \param[out]  PduInfoPtr  Pointer to PduInfo SDU (pointer to SDU buffer and
 **                          SDU length)
 ** \return Function result
 ** \retval E_OK SDU has been copied and SduLength indicates the number of
 **              copied bytes
 ** \retval E_NOT_OK No SDU has been copied. PduInfoPtr must not be used since
 **                  it may contain a NULL pointer or point to invalid data
 **
 ** \ProductionError{XCP_E_PDU_BUFFER_LOCKED, thrown\, if the callback function
 ** was called for a PDU that is not mapped to the active connection.}
 **
 ** \ServiceID{::XCP_SID_IF_TRIGGER_TRANSMIT}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, XCP_CODE) Xcp_FrIfTriggerTransmit
(
  PduIdType XcpTxPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCP_APPL_DATA) PduInfoPtr
);
#endif /* XCP_TRIGGER_TX_API_ENABLED == STD_ON */

#define XCP_STOP_SEC_CODE
#include <Xcp_MemMap.h>

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* (XCP_ON_FLEXRAY_ENABLED == STD_ON) */

#endif /* ifndef XCPONFR_CBK_H */
/*==================[end of file]============================================*/
