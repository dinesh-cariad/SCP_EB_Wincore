/**
 * \file
 *
 * \brief AUTOSAR FrNm
 *
 * This file contains the implementation of the AUTOSAR
 * module FrNm.
 *
 * \version 5.16.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
/* !LINKSTO FRNM368,1 */
#ifndef FRNM_CBK_H
#define FRNM_CBK_H

/*==================[includes]===========================================*/
#include <ComStack_Types.h>     /* AUTOSAR ComStack types (PduIdType) */
#include <FrNm_Version.h>       /* Module's version declaration */

#include <FrNm_Cfg.h>          /* Module configuration */

/*==================[macro definitions]==================================*/

/*==================[type definitions]===================================*/

/*==================[external function declarations]=====================*/

#define FRNM_START_SEC_CODE
#include <FrNm_MemMap.h>
/** \brief Indication of a received I-PDU from a lower layer communication module.
 **
 ** This function shall copy the received FlexRay NM PDU
 ** and store it locally associated with the received FlexRay NM PDU ID.
 **
 ** This function might be called by the FrNm module's environment in an interrupt context.
 **
 ** \param[in] RxPduId ID of the received I-PDU.
 ** \param[in] PduInfoPtr Contains the length of the received I-PDU and
 **                       a pointer to a buffer containing the I-PDU.
 **
 ** \ServiceID{0x42}
 ** \Reentrancy{Reentrant for different PduIds but not for same}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, FRNM_CODE) FrNm_RxIndication
(
   PduIdType RxPduId,
   P2VAR(PduInfoType, AUTOMATIC, FRNM_APPL_DATA) PduInfoPtr
);

/** \brief The lower layer communication module requests the buffer
 **        of the SDU for transmission from the upper layer module.
 **
 ** This function shall copy the triggered FlexRay NM PDU with respect
 ** to the triggered FlexRay NM PDU ID.
 **
 ** This function might be called by the FrNm module's environment in an interrupt context.
 **
 ** \param[in] TxPduId ID of the SDU that is requested to be transmitted.
 ** \param[in] PduInfoPtr Contains a pointer to a buffer to where the SDU shall be copied to.
 **            On return, the service will indicate the length of the copied SDU data in SduLength.
 **
 ** \return Std_returnType
 ** \retval E_OK SDU has been copied and SduLength indicates the number of copied bytes
 ** \retval E_NOT_OK No SDU has been copied. PduInfoPtr must not be used since
 **                   it may contain a NULL pointer or point to invalid data
 **
 ** \ServiceID{0x41}
 ** \Reentrancy{Reentrant for different PduIds but not for same}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, FRNM_CODE) FrNm_TriggerTransmit
(
   PduIdType TxPduId,
   P2VAR(PduInfoType, AUTOMATIC, FRNM_APPL_DATA) PduInfoPtr
);

/** \brief The lower layer communication module confirms the transmission of an I-PDU.
 **
 ** \param[in] TxPduId ID of the I-PDU that has been transmitted.
 **
 ** \ServiceID{0x40}
 ** \Reentrancy{Reentrant for different PduIds but not for same}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, FRNM_CODE) FrNm_TxConfirmation
(
   PduIdType TxPduId
);

#define FRNM_STOP_SEC_CODE
#include <FrNm_MemMap.h>
/*==================[internal function declarations]=====================*/

/*==================[external constants declarations]====================*/

/*==================[internal constants declarations]====================*/

/*==================[external data declarations]=========================*/

#endif /* ifndef FRNM_CBK_H */
/*==============================[end of file]============================*/
