/**
 * \file
 *
 * \brief AUTOSAR UdpNm
 *
 * This file contains the implementation of the AUTOSAR
 * module UdpNm.
 *
 * \version 2.9.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef UDPNM_CBK_H
#define UDPNM_CBK_H
/* !LINKSTO EB_SWS_UdpNm_00044_2,1 */

/*===============================[includes]=================================*/

/* !LINKSTO EB_SWS_UdpNm_00082_1,1, EB_SWS_UdpNm_00082_2,1 */
#include <ComStack_Types.h>     /* AUTOSAR ComStack types (PduIdType) */
#include <UdpNm_Version.h>      /* Module's version declaration */

#include <UdpNm_Cfg.h>          /* Module configuration */

/*===========================[macro definitions]============================*/

/*============================[type definitions]============================*/

/*=====================[external function declarations]=====================*/

#define UDPNM_START_SEC_CODE
/* !LINKSTO EB_SWS_UdpNm_00082_8,1 */
#include <UdpNm_MemMap.h>

#if (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)

/** \brief Confirms a transmission.
 **
 ** This function confirms the transmission of a NM-package.
 **
 ** Caveats: - The call context is either on interrupt level (interrupt mode)
 **            or on task level (polling mode).
 **          - The UdpNm module is initialized correctly.
 **
 ** \param[in] TxPduId Identification of the network through PDU-ID.
 **
 ** \return Standard Return Code
 ** \retval void
 **
 ** \ServiceID{0x0f}
 ** \Reentrancy{Reentrant (But not within the same channel)}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, UDPNM_CODE) UdpNm_SoAdIfTxConfirmation(PduIdType TxPduId);

#endif /* (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF) */

/** \brief Indicates a received transmission.
 **
 ** This function indicates the reception of an NM-message
 ** PDU.
 **
 ** \param[in] RxPduId Identification of the network
 **                through PDU-ID.
 ** \param[in] PduInfoPtr Contains the length of the received I-PDU and
 **            a pointer to a buffer containing the I-PDU.
 **
 ** \return Standard Return Code
 ** \retval void
 **
 ** \ServiceID{0x10}
 ** \Reentrancy{Reentrant (But not within the same channel)}
 ** \Synchronicity{Synchronous}
 */
#if(UDPNM_SOADIFRXINDICATION_ASR412 == STD_OFF)
extern FUNC(void, UDPNM_CODE) UdpNm_SoAdIfRxIndication
(
 PduIdType RxPduId,
 P2CONST(PduInfoType, AUTOMATIC, UDPNM_APPL_DATA) PduInfoPtr
);
#else
extern FUNC(void, UDPNM_CODE) UdpNm_SoAdIfRxIndication
(
 PduIdType                                        RxPduId,
 P2VAR(PduInfoType, AUTOMATIC, UDPNM_APPL_DATA) PduInfoPtr
);
#endif
#define UDPNM_STOP_SEC_CODE
#include <UdpNm_MemMap.h>

/*=====================[internal function declarations]=====================*/

/*=====================[external constants declarations]====================*/

/*=====================[internal constants declarations]====================*/

/*=======================[external data declarations]=======================*/

#endif /* ifndef UDPNM_CBK_H */
/*==============================[end of file]===============================*/
