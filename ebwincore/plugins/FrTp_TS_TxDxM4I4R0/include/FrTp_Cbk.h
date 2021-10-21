/**
 * \file
 *
 * \brief AUTOSAR FrTp
 *
 * This file contains the implementation of the AUTOSAR
 * module FrTp.
 *
 * \version 4.4.26
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef FRTP_CBK_H
#define FRTP_CBK_H

/*=====================[Include Section]=====================================*/

#include <ComStack_Types.h>     /* basic commstack types from AUTOSAR */
#include <FrTp_Version.h>       /* FrTp module version information */

/*==============================[Macros]=====================================*/

/*=====================[Global Function Declarations]========================*/

#define FRTP_START_SEC_CODE
#include <FrTp_MemMap.h>

/**
 * \brief This function is called by the FlexRay Interface after the TP-related
 *        Pdu has been transmitted over the network.
 *
 *
 * \param[in] FrTxConfirmationPduId     This parameter contains the confirmation identifier of the
 *                          transmitted Fr N-PDU.
 *
 * \ServiceID{0x40}
 * \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId.}
 * \Synchronicity{Synchronous}
 *
 * \remarks: No critical section macros are used in the implementation since
 * this callback function is called by FrIf, which is executed synchronously
 * with the FlexRay bus and thus synchronously with the FrTp_MainFunction.
 */
extern FUNC(void,FRTP_CODE) FrTp_TxConfirmation
(
    VAR(PduIdType,AUTOMATIC) FrTxConfirmationPduId
);


/**
 * \brief This function is called by the FlexRay Interface for sending out a
 *        FlexRay frame.
 *
 *
 * \param[in] FrTxConfirmationPduId       Confirmation ID of FlexRay L-PDU that is requested to be
 *                          transmitted.
 * \param[out] PduInfoPtr   Pointer Pdu info structure.
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
 * \remarks: No critical section macros are used in the implementation since
 * this callback function is called by FrIf, which is executed synchronously
 * with the FlexRay bus and thus synchronously with the FrTp_MainFunction.
 */
/* DEVIATION:
 * maximum length is known at compile time
 * actual length copied is passed to FrIf in FrIf_Transmit()
 */
extern FUNC(Std_ReturnType,FRTP_CODE) FrTp_TriggerTransmit
(
    VAR(PduIdType,AUTOMATIC) FrTxConfirmationPduId,
    P2VAR(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) PduInfoPtr
);

/**
 * \brief The FlexRay Interface calls this primitive after the reception of an
 *        Fr N-PDU.
 *
 *
 * \param[in] RxPduId       This parameter contains the identifier of the received
 *                          Fr N-PDU.
 * \param[in] PduInfoPtr    Pointer Pdu info structure.
 *
 *
 * \remarks: No critical section macros are used in the implementation since
 * this callback function is called by FrIf, which is executed synchronously
 * with the FlexRay bus and thus synchronously with the FrTp_MainFunction.
 *
 * \ServiceID{0x42}
 * \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId.}
 * \Synchronicity{Synchronous}
 */
/* DEVIATION:
 * assumption that payload always contains a valid PCI
 * actual length received is evaluated from PCI
 */
extern FUNC(void,FRTP_CODE) FrTp_RxIndication
(
    VAR(PduIdType,AUTOMATIC) RxPduId,
    P2VAR(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) PduInfoPtr
);

#define FRTP_STOP_SEC_CODE
#include <FrTp_MemMap.h>

#endif /* FRTP_CBK_H */

