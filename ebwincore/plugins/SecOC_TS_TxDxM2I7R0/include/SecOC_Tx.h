/**
 * \file
 *
 * \brief AUTOSAR SecOC
 *
 * This file contains the implementation of the AUTOSAR
 * module SecOC.
 *
 * \version 2.7.4
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef SECOC_TX_H
#define SECOC_TX_H

/*==================[misra deviations]============================================================*/

/*==================[includes]====================================================================*/
/* !LINKSTO SecOC.Dsn.BasicSWStructure.Tx.SecOC_Txh,1 */
#include <SecOC_Int.h>

/*==================[macros]======================================================================*/

/** \brief Macro which indicates the number of states that the Tx side state machine has
 */
#if (defined SECOC_NUMBER_OF_TXSTATES)
#error SECOC_NUMBER_OF_TXSTATES already defined
#endif
#if (SECOC_TX_CSM_ASYNC == STD_ON)
#define SECOC_NUMBER_OF_TXSTATES                                                        7U
#else
#define SECOC_NUMBER_OF_TXSTATES                                                        6U
#endif

/** \brief SecOC Tx state machine index for SecOC_TxIdle function
 */
#if (defined SECOC_TX_STATE_IDLE)
#error SECOC_TX_STATE_IDLE already defined
#endif
#define SECOC_TX_STATE_IDLE                                                            0U

/** \brief SecOC Tx state machine index for SecOC_Tx_GetFreshness function
 */
#if (defined SECOC_TX_STATE_GETFRESHNESS)
#error SECOC_TX_STATE_GETFRESHNESS already defined
#endif
#define SECOC_TX_STATE_GETFRESHNESS                                                    1U

/** \brief SecOC Tx state machine index for SecOC_TxAuthentication function
 */
#if (defined SECOC_TX_STATE_AUTHENTICATION)
#error SECOC_TX_STATE_AUTHENTICATION already defined
#endif
#define SECOC_TX_STATE_AUTHENTICATION                                                  2U

/** \brief SecOC Tx state machine index for SecOC_TxConstructSecuredPdu function
 */
#if (defined SECOC_TX_STATE_CONSTRUCTSECUREDPDU)
#error SECOC_TX_STATE_CONSTRUCTSECUREDPDU already defined
#endif
#define SECOC_TX_STATE_CONSTRUCTSECUREDPDU                                             3U

/** \brief SecOC Tx state machine index for SecOC_TxBroadcastSecuredPdu function
 */
#if (defined SECOC_TX_STATE_BROADCASTSECUREDPDU)
#error SECOC_TX_STATE_BROADCASTSECUREDPDU already defined
#endif
#define SECOC_TX_STATE_BROADCASTSECUREDPDU                                             4U

/** \brief SecOC Tx state machine index for SecOC_Tx_StateWaitForConfirmation function
 */
#if (defined SECOC_TX_STATE_WAITFORCONFIRMATION)
#error SECOC_TX_STATE_WAITFORCONFIRMATION already defined
#endif
#define SECOC_TX_STATE_WAITFORCONFIRMATION                                             5U

#if (SECOC_TX_CSM_ASYNC == STD_ON)
/** \brief SecOC Tx state machine index for SecOC_TxWaitForAuthenticationCallback function
 */
#if (defined SECOC_TX_STATE_WAITFORAUTHENTICATIONCALLBACK)
#error SECOC_TX_STATE_WAITFORAUTHENTICATIONCALLBACK already defined
#endif
#define SECOC_TX_STATE_WAITFORAUTHENTICATIONCALLBACK                                   6U
#endif

/** \brief SecOC Tx state machine event, which indicates, that no event occurred
 */
#if (defined SECOC_TXEVENT_NONE)
#error SECOC_TXEVENT_NONE already defined
#endif
#define SECOC_TXEVENT_NONE                0U

/** \brief SecOC Tx state machine event, which is emitted, when SecOC_Transmit is called.
 */
#if (defined SECOC_TXEVENT_TRANSMIT)
#error SECOC_TXEVENT_TRANSMIT already defined
#endif
#define SECOC_TXEVENT_TRANSMIT            1U

/** \brief SecOC Tx state machine event, which is emitted on a TxConfirmation
 */
#if (defined SECOC_TXEVENT_TXCONFIRMATION_OK)
#error SECOC_TXEVENT_TXCONFIRMATION_OK already defined
#endif
#define SECOC_TXEVENT_TXCONFIRMATION_OK      2U

/** \brief SecOC Tx state machine event, which is emitted on a TxConfirmation
 */
#if (defined SECOC_TXEVENT_TXCONFIRMATION_NOT_OK)
#error SECOC_TXEVENT_TXCONFIRMATION_NOT_OK already defined
#endif
#define SECOC_TXEVENT_TXCONFIRMATION_NOT_OK      3U

/** \brief SecOC Tx state machine event, which cancels a transmission
 */
#if (defined SECOC_TXEVENT_CANCEL)
#error SECOC_TXEVENT_CANCEL already defined
#endif
#define SECOC_TXEVENT_CANCEL              4U

#if (SECOC_TX_CSM_ASYNC == STD_ON)
/** \brief SecOC Tx state machine event, which is emitted by a Csm Callback with the MAC
 *         authentication result, which is E_OK
 */
#if (defined SECOC_TXEVENT_AUTHCALLBACK_OK)
#error SECOC_TXEVENT_AUTHCALLBACK_OK already defined
#endif
#define SECOC_TXEVENT_AUTHCALLBACK_OK     5U

/** \brief SecOC Tx state machine event, which is emitted by a Csm Callback with the MAC
 *         authentication result, which is not E_OK
 */
#if (defined SECOC_TXEVENT_AUTHCALLBACK_NOT_OK)
#error SECOC_TXEVENT_AUTHCALLBACK_NOT_OK already defined
#endif
#define SECOC_TXEVENT_AUTHCALLBACK_NOT_OK 6U

#endif /* #if (SECOC_TX_CSM_ASYNC == STD_ON) */

/*==================[type definitions]============================================================*/

/*==================[external constants]==========================================================*/

/*==================[external data]===============================================================*/

/*==================[external function declarations]==============================================*/

#define SECOC_START_SEC_CODE
#include <SecOC_MemMap.h>

/* !LINKSTO SecOC_EB_Tx_xxx22,1 */
/**
 * \brief This function performs the processing of the SecOC module's
 *        authentication for the Tx path Pdu processing. In case of synchronous
 *        Pdu processing it is called directly by the PduR calls of SecOC
 *        interface or, in case of asynchronous Pdu processing, by
 *        SecOC_MainFunctionTx respectively.
 *
 * \param[in]  pduIdx  the index of the processed Pdu
 */
extern FUNC(void, SECOC_CODE) SecOC_Tx_ProcessPduStates
(
  uint16 pduIdx
);

#define SECOC_STOP_SEC_CODE
#include <SecOC_MemMap.h>

#endif /* #ifndef SECOC_TX_H */

/*==================[end of file]=================================================================*/
