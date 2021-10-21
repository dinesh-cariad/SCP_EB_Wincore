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

#ifndef SECOC_RX_H
#define SECOC_RX_H

/*==================[misra deviations]============================================================*/

/*==================[includes]====================================================================*/
/* !LINKSTO SecOC.Dsn.BasicSWStructure.Rx.SecOC_Rxh,1 */
#include <SecOC_Int.h>

/*==================[macros]======================================================================*/

/** \brief Macro which indicates the number of states that the Rx side state machine has
 */
#if (defined SECOC_NUMBER_OF_RXSTATES)
#error SECOC_NUMBER_OF_RXSTATES already defined
#endif
#if (SECOC_RX_CSM_ASYNC == STD_ON)
#define SECOC_NUMBER_OF_RXSTATES                                                         7U
#else
#define SECOC_NUMBER_OF_RXSTATES                                                         6U
#endif

/** \brief SecOC Rx state machine index for SecOC_RxIdle function
 */
#if (defined SECOC_RX_STATE_IDLE)
#error SECOC_RX_STATE_IDLE already defined
#endif
#define SECOC_RX_STATE_IDLE                                                              0U

/** \brief SecOC Rx state machine index for SecOC_RxConstructDataToAuthenticator function
 */
#if (defined SECOC_RX_STATE_CONSTRUCTDATATOAUTHENTICATOR)
#error SECOC_RX_STATE_CONSTRUCTDATATOAUTHENTICATOR already defined
#endif
#define SECOC_RX_STATE_CONSTRUCTDATATOAUTHENTICATOR                                      1U

/** \brief SecOC Rx state machine index for SecOC_RxVerification function
 */
#if (defined SECOC_RX_STATE_VERIFICATION)
#error SECOC_RX_STATE_VERIFICATION already defined
#endif
#define SECOC_RX_STATE_VERIFICATION                                                      2U

/** \brief SecOC Rx state machine index for SecOC_Rx_StateCheckVerificationResult function
 */
#if (defined SECOC_RX_STATE_CHECKVERIFICATIONRESULT)
#error SECOC_RX_STATE_CHECKVERIFICATIONRESULT already defined
#endif
#define SECOC_RX_STATE_CHECKVERIFICATIONRESULT                                           3U

/** \brief SecOC Rx state machine index for SecOC_Rx_StateBroadcastPdu function
 */
#if (defined SECOC_RX_STATE_BROADCASTPDU)
#error SECOC_RX_STATE_BROADCASTPDU already defined
#endif
#define SECOC_RX_STATE_BROADCASTPDU                                                      4U

/** \brief SecOC Rx state machine index for SecOC_Rx_StateRetryToObtainFreshness function
 */
#if (defined SECOC_RX_STATE_RETRYTOOBTAINFRESHNESS)
#error SECOC_RX_STATE_RETRYTOOBTAINFRESHNESS already defined
#endif
#define SECOC_RX_STATE_RETRYTOOBTAINFRESHNESS                                            5U

#if (SECOC_RX_CSM_ASYNC == STD_ON)
/** \brief SecOC Rx state machine index for SecOC_RxWaitForVerificationCallback function
 */
#if (defined SECOC_RX_STATE_WAITFORVERIFICATIONCALLBACK)
#error SECOC_RX_STATE_WAITFORVERIFICATIONCALLBACK already defined
#endif
#define SECOC_RX_STATE_WAITFORVERIFICATIONCALLBACK                                       6U
#endif

/** \brief SecOC Rx state machine event, which indicates, that no event occurred
 */
#if (defined SECOC_RXEVENT_NONE)
#error SECOC_RXEVENT_NONE already defined
#endif
#define SECOC_RXEVENT_NONE                                                               0U

/** \brief SecOC Rx state machine event, which occurs on an RX indication
 */
#if (defined SECOC_RXEVENT_RXINDICATION)
#error SECOC_RXEVENT_RXINDICATION already defined
#endif
#define SECOC_RXEVENT_RXINDICATION                                                       1U

/** \brief SecOC Rx state machine event, which occurs on a StartOfReception
 */
#if (defined SECOC_RXEVENT_STARTRECEPTION)
#error SECOC_RXEVENT_STARTRECEPTION already defined
#endif
#define SECOC_RXEVENT_STARTRECEPTION                                                     2U

#if (SECOC_RX_CSM_ASYNC == STD_ON)
/** \brief SecOC Rx state machine event, which is emitted by a Csm Callback with the MAC
 *         verification result, which is E_OK
 */
#if (defined SECOC_RXEVENT_VERCALLBACK_OK)
#error SECOC_RXEVENT_VERCALLBACK_OK already defined
#endif
#define SECOC_RXEVENT_VERCALLBACK_OK                                                     3U

/** \brief SecOC Rx state machine event, which is emitted by a Csm Callback with the MAC
 *         verification result, which is not E_OK
 */
#if (defined SECOC_RXEVENT_VERCALLBACK_NOT_OK)
#error SECOC_RXEVENT_VERCALLBACK_NOT_OK already defined
#endif
#define SECOC_RXEVENT_VERCALLBACK_NOT_OK                                                 4U
#endif /* #if (SECOC_RX_CSM_ASYNC == STD_ON) */

/*==================[type definitions]============================================================*/

/*==================[external constants]==========================================================*/

/*==================[external data]===============================================================*/

/*==================[external function declarations]==============================================*/

#define SECOC_START_SEC_CODE
#include <SecOC_MemMap.h>

/* !LINKSTO SecOC_EB_Rx_xxx16,1 */
/**
 * \brief This function performs the processing of the SecOC module's
 *        authentication and verification for the Rx path Pdu processing. In
 *        case of synchronous Pdu processing it is called directly by the PduR
 *        calls of SecOC interface or, in case of asynchronous Pdu processing,
 *        by SecOC_MainFunctionRx respectively.
 *
 * \param[in]  pduIdx  the index of the processed Pdu
 */
extern FUNC(void, SECOC_CODE) SecOC_Rx_ProcessPduStates
(
  uint16 pduIdx
);

#define SECOC_STOP_SEC_CODE
#include <SecOC_MemMap.h>

#endif /* #ifndef SECOC_RX_H */

/*==================[end of file]=================================================================*/
