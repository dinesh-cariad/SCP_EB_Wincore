/**
 * \file
 *
 * \brief AUTOSAR Mirror
 *
 * This file contains the implementation of the AUTOSAR
 * module Mirror.
 *
 * \version 1.0.17
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef MIRROR_CBK_H
#define MIRROR_CBK_H

/*==================[inclusions]============================================*/

#include <Mirror_Types.h>       /* Mirror specific types */

/*==================[macros]================================================*/

/** \brief Defines API id of function Mirror_TxConfirmation(). */
#if (defined MIRROR_SID_TXCONFIRMATION)
#error MIRROR_SID_TXCONFIRMATION already defined!
#endif /* #if (defined MIRROR_SID_TXCONFIRMATION)*/
#define MIRROR_SID_TXCONFIRMATION 0x40U

#if (defined MIRROR_SID_PDURIFRXINDICATION)
#error MIRROR_SID_PDURIFRXINDICATION already defined!
#endif /* #if (defined MIRROR_SID_PDURIFRXINDICATION)*/
#define MIRROR_SID_PDURIFRXINDICATION 0x41U

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define MIRROR_START_SEC_CODE
#include <Mirror_MemMap.h>

/**
 * \brief Mirror module Transmission confirmation callback function.
 *
 * This callback function is called by the lower interface module to confirm the transmission
 * of a PDU.
 *
 * \param[in] TxPduId - ID of the PDU that has been transmitted.
 *
 * \ServiceID{0x40}
 * \Reentrancy{  Reentrant for different PduIds
 *               Non Reentrant for the same PduId  }
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, MIRROR_CODE) Mirror_TxConfirmation
(
  PduIdType TxPduId
);

/**
 * \brief Mirror module Reception indication callback function.
 *
 * This callback function is called by the lower interface module to confirm the reception
 * of a PDU.
 * "This is a dummy function that should never be called and if it's called a DET  error is reported".
 *
 * \param[in] MirrorRxPduId - ID of the PDU that has been received.
 * \param[in] PduInfoPtr - Pointer to the data which has been received.
 *
 * \ServiceID{0x41}
 * \Reentrancy{  Reentrant for different PduIds
 *               Non Reentrant for the same PduId  }
 * \Synchronicity{Synchronous}
 */
FUNC(void, COM_CODE) Mirror_PduRIfRxIndication
(
   PduIdType MirrorRxPduId,
   P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr
);

#define MIRROR_STOP_SEC_CODE
#include <Mirror_MemMap.h>




/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef MIRROR_CBK_H */
/*==================[end of file]===========================================*/
