/**
 * \file
 *
 * \brief AUTOSAR FrArTp
 *
 * This file contains the implementation of the AUTOSAR
 * module FrArTp.
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2016 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef _FRARTP_CBK_H_
#define _FRARTP_CBK_H_

/* ****************************************************************************
**                      Include Section                                      **
******************************************************************************/

#include <ComStack_Types.h>       /* basic commstack types from AUTOSAR */
#include <FrArTp_Version.h>       /* FrArTp module version information */

/* ****************************************************************************
**                               Macros                                      **
******************************************************************************/

/* ****************************************************************************
**                      Global Function Declarations                         **
******************************************************************************/
#define FRARTP_START_SEC_CODE
#include <FrArTp_MemMap.h>

/* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00153,1 */
/**
 * \brief The lower layer communication interface module confirms
 *        the transmission of an I-PDU.
 *
 *
 * \param[in] TxPduId  ID of the I-PDU that has been transmitted.
 *
 * \ServiceID{0x40}
 * \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId.}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(void,FRARTP_CODE) FrArTp_TxConfirmation
(
    VAR(PduIdType,AUTOMATIC) TxPduId
);

/* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00154,1 */
/**
 * \brief This function is called by the FlexRay Interface for sending out a
 *        FlexRay frame.
 *
 * Within this API, the upper layer module (called module) shall check whether the
 * available data fits into the buffer size reported by PduInfoPtr->SduLength.
 * If it fits, it shall copy its data into the buffer provided by PduInfoPtr->SduDataPtr
 * and update the length of the actual copied data in PduInfoPtr->SduLength.
 * If not, it returns E_NOT_OK without changing PduInfoPtr.
 *
 * \param[in] TxPduId       ID of the SDU that is requested to be transmitted.
 * \param[out] PduInfoPtr   Contains a pointer to a buffer (SduDataPtr) to where the SDU data
 *                          shall be copied, and the available buffer size in SduLengh.
 *                          On return, the service will indicate the length of the copied
 *                          SDU data in SduLength.
 *
 * \return Std_ReturnType
 *
 * \retval E_OK             SDU has been copied and SduLength indicates the number of copied bytes.
 * \retval E_NOT_OK         No SDU has been copied. PduInfoPtr must not be used since it may
 *                          contain a NULL pointer or point to invalid data.
 *
 * \ServiceID{0x41}
 * \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId.}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(Std_ReturnType,FRARTP_CODE) FrArTp_TriggerTransmit
(
    VAR(PduIdType,AUTOMATIC) TxPduId,
    P2VAR(PduInfoType,AUTOMATIC,FRARTP_APPL_DATA) PduInfoPtr
);

/* !LINKSTO FrArTp.EB.FrArTp_RxIndication,1 */
/**
 * \brief Indication of a received I-PDU from a lower layer
 *        communication interface module.
 *
 *
 * \param[in] RxPduId       ID of the received I-PDU.
 * \param[in] PduInfoPtr    Contains the length (SduLength) of the received
 *                          I-PDU and a pointer to a buffer (SduDataPtr)
 *                          containing the I-PDU.
 *
 *
 * \ServiceID{0x42}
 * \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId.}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void,FRARTP_CODE) FrArTp_RxIndication
(
    VAR(PduIdType,AUTOMATIC) RxPduId,
    P2VAR(PduInfoType,AUTOMATIC,FRARTP_APPL_DATA) PduInfoPtr
);

#define FRARTP_STOP_SEC_CODE
#include <FrArTp_MemMap.h>

#endif /* _FRARTP_CBK_H_ */
