#ifndef SD_CBK_H
#define SD_CBK_H

/**
 * \file
 *
 * \brief AUTOSAR Sd
 *
 * This file contains the implementation of the AUTOSAR
 * module Sd.
 *
 * \version 1.4.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/

#include <ComStack_Types.h>

#include <SoAd.h>


/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/
/* !LINKSTO Sd.ASR40.callback,1 */
#define SD_START_SEC_CODE
#include <Sd_MemMap.h>

/** \brief Sd Rx Indication
 **
 ** Indication of a received I-PDU from a lower layer communication interface module.
 **
 ** \param[in] RxPduId      ID of the received I-PDU.
 ** \param[in] PduInfoPtr   Contains the length (SduLength) of the received I-PDU and a pointer to
 **                         a buffer (SduDataPtr) containing the I-PDU.
 **
 ** \ServiceID{0x42}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, SD_CODE) Sd_RxIndication
(
  PduIdType RxPduId,
  P2VAR(PduInfoType, AUTOMATIC, SD_APPL_DATA) PduInfoPtr
);



/* !LINKSTO Sd.ASR40.SWS_SD_00412,1 */
/** \brief Local IP address assignment change indication
 **
 ** This function gets called by the SoAd if an IP address assignment related to a
 ** socket connection changes (i.e. new address assigned or assigned address
 ** becomes invalid).
 **
 ** \param[in] SoConId       Socket connection index specifying the socket connection where the IP
 **                          address assigment has changed.
 ** \param[in] State         State of IP address assignment.
 **
 ** \ServiceID{0x05}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, SD_CODE) Sd_LocalIpAddrAssignmentChg(
    SoAd_SoConIdType SoConId,
    TcpIp_IpAddrStateType State
);

#define SD_STOP_SEC_CODE
#include <Sd_MemMap.h>

/*==================[internal function definitions]=========================*/

#endif /* ifndef SD_CBK_H */
/*==================[end of file]===========================================*/
