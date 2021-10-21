/**
 * \file
 *
 * \brief AUTOSAR CanTp
 *
 * This file contains the implementation of the AUTOSAR
 * module CanTp.
 *
 * \version 6.8.42
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef CANTP_CBK_H
#define CANTP_CBK_H

/* !LINKSTO CanTp.ASR40.CANTP233,1 */

/*==================[inclusions]=============================================*/

#include <TSAutosar.h>         /* EB specific standard types */
#include <ComStack_Types.h>    /* typedefs for AUTOSAR com stack */
#include <CanTp_Version.h>     /* CanTp version declarations */

#include <CanTp_Cfg.h>         /* CanTp configuration */
#if(CANTP_JUMPTABLE_SUPPORT == STD_ON)
#include <CanTp_EntryTableCbk.h>  /* CanTp callbacks with jumptable */
#endif

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[external function declarations]=========================*/

#define CANTP_START_SEC_CODE
#include <CanTp_MemMap.h>

#if(CANTP_PROVIDE_API_FUNCTIONS == STD_ON)

/**\brief Indicate a successful reception.
 **
 ** This function is called by the CAN Interface after a successful reception
 ** of a Rx CAN L-PDU.
 **
 ** \param[in] CanTpRxPduId ID of CAN L-PDU that has been received.
 **                    Identifies the data that has been received.
 **                    Range: 0..(maximum number of L-PDU IDs received ) - 1.
 ** \param[in] CanTpRxPduPtr Indicator of structure with received
 **                    L-SDU (payload) and data length.
 **
 ** \ServiceID{0x42}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, CANTP_CODE) CanTp_RxIndication
(
  PduIdType CanTpRxPduId,
  P2VAR(PduInfoType, AUTOMATIC, CANTP_APPL_DATA) CanTpRxPduPtr
);


/**\brief Confirm transmitted frame.
 **
 ** This function confirms all transmitted CAN frames belonging to
 ** the CAN Transport Layer.
 **
 ** \param[in] CanTpTxPduId ID of CAN L-PDU that has been transmitted.
 **                   Range: 0...(maximum number of L-PDU IDs received ) - 1.
 **
 ** \ServiceID{0x40}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, CANTP_CODE) CanTp_TxConfirmation
(
  PduIdType CanTpTxPduId
);

#endif /* CANTP_PROVIDE_API_FUNCTIONS == STD_ON */

#define CANTP_STOP_SEC_CODE
#include <CanTp_MemMap.h>

#endif /* ifndef CANTP_CBK_H */
/*==================[end of file]============================================*/
