/**
 * \file
 *
 * \brief AUTOSAR CanNm
 *
 * This file contains the implementation of the AUTOSAR
 * module CanNm.
 *
 * \version 6.19.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef CANNM_CBK_H
#define CANNM_CBK_H

/*===============================[includes]=================================*/

/* !LINKSTO CANNM305,1 */
#include <ComStack_Types.h>     /* AUTOSAR ComStack types (PduIdType) */
#include <CanNm_Version.h>      /* Module's version declaration */

#include <CanNm_Cfg.h>          /* Module configuration */

/*===========================[macro definitions]============================*/

#if (defined CANNM_API_ID_TXCONFIRMATION) /* To prevent double declaration */
#error CANNM_API_ID_TXCONFIRMATION already defined
#endif /* if (defined CANNM_API_ID_TXCONFIRMATION) */

/** \brief CanNM API ID.
 **
 ** Definition of CANNM_API_ID_TXCONFIRMATION.
 */
#define CANNM_API_ID_TXCONFIRMATION             15U

#if (defined CANNM_API_ID_RXINDICATION) /* To prevent double declaration */
#error CANNM_API_ID_RXINDICATION already defined
#endif /* if (defined CANNM_API_ID_RXINDICATION) */

/** \brief CanNM API ID.
 **
 ** Definition of CANNM_API_ID_RXINDICATION.
 */
#define CANNM_API_ID_RXINDICATION               16U

/*============================[type definitions]============================*/

/*=====================[external function declarations]=====================*/
/* !LINKSTO CANNM303,1 */

#define CANNM_START_SEC_CODE
#include <CanNm_MemMap.h>

#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF) && (CANNM_IMMEDIATE_TXCONF_ENABLED == STD_OFF)

/** \brief Confirms a transmission.
 **
 ** This function confirms the transmission of a NM-package.
 **
 ** \param[in] TxPduId Identification of the network through PDU-ID.
 **
 ** \return Standard Return Code
 ** \retval void
 **
 ** \ServiceID{15}
 ** \Reentrancy{Reentrant (But not within the same channel)}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, CANNM_CODE) CanNm_TxConfirmation(PduIdType TxPduId);

#endif /* (CANNM_PASSIVE_MODE_ENABLED == STD_OFF) &&
          (CANNM_IMMEDIATE_TXCONF_ENABLED == STD_OFF) */

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
 ** \ServiceID{16}
 ** \Reentrancy{Reentrant (But not within the same channel)}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, CANNM_CODE) CanNm_RxIndication(PduIdType RxPduId,
    P2VAR(PduInfoType, AUTOMATIC, CANNM_APPL_DATA) PduInfoPtr);

#define CANNM_STOP_SEC_CODE
#include <CanNm_MemMap.h>

/*=====================[internal function declarations]=====================*/

/*=====================[external constants declarations]====================*/

/*=====================[internal constants declarations]====================*/

/*=======================[external data declarations]=======================*/

#endif /* ifndef CANNM_CBK_H */
/*==============================[end of file]===============================*/
