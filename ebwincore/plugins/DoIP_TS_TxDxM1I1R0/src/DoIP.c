/**
 * \file
 *
 * \brief AUTOSAR DoIP
 *
 * This file contains the implementation of the AUTOSAR
 * module DoIP.
 *
 * \version 1.1.21
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */


/*
 *  Misra-C:2012 Deviations:
 *
 *  MISRAC2012-1) Deviated Rule: 8.9 (Advisory)
 *    An object should be defined at block scope if its identifier only appears in a single
 *    function.
 *
 *    Reason:
 *    - Although an object is used by a single function only, thus could have been defined
 *      at block scope (local variable), due to its potential size of 64 Kb, it is defined
 *      at global scope (global variable).
 *
 *  MISRAC2012-2) Deviated Rule: 12.2 (Required)
 *    The right hand operand of a shift operator shall lie in the range zero to one less than the
 *    width in bits of the essential type of the left hand operand.
 *
 *    Reason:
 *    - If a cast to the correct range was done before the shift, then this rule is a false
 *      positive.
 *
 */

/*==================[includes]==============================================*/
#include <DoIP.h> /* Module public header file */
#include <DoIP_Cbk.h> /* Module public callback declarations header file */
#include <DoIP_Int.h> /* Module internal header file */
#include <DoIP_Types_Int.h> /* Module XGEN generated internal types file */
#include <DoIP_Trace.h> /* Debug and trace */
#include <TSAtomic_Assign.h> /* Atomic assignment macros */
#include <TSMem.h>
#include <DoIP_CustomPayloadType.h>

#include <TSPBConfig_Signature.h> /* relocatable post-build macros */

#if (DOIP_PBCFGM_SUPPORT_ENABLED == STD_ON)
#define PBCFGM_NO_PBCFG_REQUIRED
#include <PbcfgM.h>               /* Post build configuration manager API */
#endif /* DOIP_PBCFGM_SUPPORT_ENABLED */

#include <PduR_DoIP.h>
/* !LINKSTO DoIP.ASR41.SWS_DoIP_00157, 1 */
#include <SoAd.h>
/* !LINKSTO DoIP.ASR41.SWS_DoIP_00157, 1 */
#include <SchM_DoIP.h>
#if (DOIP_CUSTOM_PAYLOAD_TYPE_ENABLED == STD_ON)
#include <EcuC.h>
#endif /* DOIP_CUSTOM_PAYLOAD_TYPE_ENABLED == STD_ON */
#if(DOIP_DEV_ERROR_DETECT == STD_ON)
#include <Det.h> /* DET support */
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */

/*==================[macros]================================================*/

/* Maximum timeout to recover from missing DoIP_SoAdIfTxConfirmation() after
   SoAd_IfTransmit() was called. */
#if (defined DOIP_RELEASE_REMOTEADDR_TIMEOUT)
#error DOIP_RELEASE_REMOTEADDR_TIMEOUT is already defined
#endif
#define DOIP_RELEASE_REMOTEADDR_TIMEOUT 50U

/* Returns if the channel is pending on routing activation auth/conf callback function. */
#if (defined DOIP_IS_CHANNEL_PENDING)
#error DOIP_IS_CHANNEL_PENDING is already defined
#endif
#define DOIP_IS_CHANNEL_PENDING(ChannelPtr) \
  ((ChannelPtr->RoutingActivationState == DOIP_ROUTING_PENDING_AUTHENTICATION) || \
  (ChannelPtr->RoutingActivationState == DOIP_ROUTING_PENDING_CONFIRMATION))

/*==================[type definitions]======================================*/

/** \brief Activation line state type. **/
typedef uint8 DoIP_ActivationLineStateType;
#if (defined DOIP_ACT_LINE_STATE_INACTIVE)
#error DOIP_ACT_LINE_STATE_INACTIVE is already defined
#endif
#define DOIP_ACT_LINE_STATE_INACTIVE 0U

#if (defined DOIP_ACT_LINE_STATE_ACTIVE)
#error DOIP_ACT_LINE_STATE_ACTIVE is already defined
#endif
#define DOIP_ACT_LINE_STATE_ACTIVE 1U

#if (defined DOIP_ACT_LINE_STATE_REQ_ACTIVE)
#error DOIP_ACT_LINE_STATE_REQ_ACTIVE is already defined
#endif
#define DOIP_ACT_LINE_STATE_REQ_ACTIVE 2U


/** \brief Internal type for operation function argument. **/
typedef uint8 DoIP_OperationType;
#if (defined DOIP_REQUEST_IP_ADDR_OP)
#error DOIP_REQUEST_IP_ADDR_OP is already defined
#endif
#define DOIP_REQUEST_IP_ADDR_OP 0U

#if (defined DOIP_RELEASE_IP_ADDR_OP)
#error DOIP_RELEASE_IP_ADDR_OP is already defined
#endif
#define DOIP_RELEASE_IP_ADDR_OP 1U

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/
#define DOIP_START_SEC_CODE
#include <DoIP_MemMap.h>

/** \brief Internal service to check if timer is active
* \param[in] TimerIdx - Index of Timer
* \return boolean
* \RetVal TRUE - Timer is active.
* \RetVal FALSE - Timer is inactive.
*/
STATIC FUNC(boolean, DOIP_CODE) DoIP_IsTimerActive
(
  DoIP_TimeoutIdxType TimerIdx
);

/** \brief Internal service to start timer
* \param[in] TimerIdx - Index of Timer
* \param[in] Timeout - Value of timeout time
*/
STATIC FUNC(void, DOIP_CODE) DoIP_StartTimer
(
  DoIP_TimeoutIdxType TimerIdx,
  DoIP_TimeoutType Timeout
);

/** \brief Internal service to stop timer
* \param[in] TimerIdx - Index of Timer

*/
STATIC FUNC(void, DOIP_CODE) DoIP_StopTimer
(
  DoIP_TimeoutIdxType TimerIdx
);

/** \brief Internal service to process timeout
*/
STATIC FUNC(void, DOIP_CODE) DoIP_ProcessTimeout(void);

/** \brief Internal service to process Confirmation/Authentication pending routing activations
*/
STATIC FUNC(void, DOIP_CODE) DoIP_ProcessAuthConfPendingRoutingActivations(void);

/** \brief Internal service to process Alive check pending routing activations
*/
STATIC FUNC(void, DOIP_CODE) DoIP_ProcessAliveCheck(void);


#if(DOIP_ENABLE_MAIN_FUNCTION_TX != STD_ON)
/** \brief Transmit part of the main function if disabled to be external
*/
STATIC FUNC(void, DOIP_CODE) DoIP_MainFunctionTx(void);
#endif

/** \brief Internal service to continue Routing activation process after Alive check is cleared
 *
 * \param[in] TcpConIdx - Index of Tcp connection pending on Alive check
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(void, DOIP_CODE) DoIP_ContinueRoutingActivation
(
  DoIP_TcpConIdxType TcpConIdx
);

/** \brief Internal service to process Alive check state machines during connection reset.
 *
 * \param[in] TcpConIdx - Index of reseted connection
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(void, DOIP_CODE) DoIP_HandleAliveCheckConReset
(
  DoIP_TcpConIdxType TcpConIdx
);

/** \brief Internal service to verify received VIN
 *
 * If a user callback function for getting VIN is configured, it will be called to retrieve VIN,
 * which is going to be compared with the received one.
 * If a user callback function is not provided, or it returns E_NOT_OK, received VIN will be
 * compared against configured VIN invalidity pattern.
 *
 * \param[In] ReceivedVinPtr - A pointer to a memory location where received VIN is stored
 *
 * \return Std_ReturnType
 * \retval E_OK - Received VIN matches VIN retrieved with User_DoIPGetVin(), or VIN
 *                invalidity pattern.
 * \retval E_NOT_OK - Received VIN does not match local VIN.
 *
*/
STATIC FUNC(Std_ReturnType, DOIP_CODE) DoIP_Verify_VIN
(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) ReceivedVinPtr
);

/** \brief Internal service to copy VIN into specified location
 *
 * The function calls user callback function for retrieving VIN if it is configured, otherwise VIN
 * is set to the configured VIN invalidity pattern.
 *
 * \param[out] VinPtr - A pointer to a memory location where VIN needs to be copied
 *
 * \return Std_ReturnType
 * \retval E_OK - Copied VIN is retrieved from User_DoIPGetVin()
 * \retval E_NOT_OK - Valid VIN could not be retrieved from User_GetVin(), VIN invalidity pattern
 *                    used instead.
*/
STATIC FUNC(Std_ReturnType, DOIP_CODE) DoIP_Fill_VIN
(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) VinPtr
);

/** \brief Internal service to perform generic DoIP header checks.
 *
 * This internal service checks the generic header and sends NACK messages if an error is detected.
 * Additionally it will return the payload type and payload length from the header.
 *
 * \param[in] DoIP_SoAdRxPduId - SoAd Rx PDU ID the header belongs to.
 * \param[in] PduInfoPtr - Pointer to the header.
 * \param[out] PayloadTypePtr - Pointer to be used to return the payload type.
 * \param[out] PayloadLengthPtr - Pointer to be used to return the payload length.
 * \param[out] Ret_IfTransmitPtr - Pointer to SoAd_IfTransmit() return value if called.
 *
 * \return Std_ReturnType
 * \retval E_OK - Message header is okay.
 * \retval E_NOT_OK - Message header is not okay. Message shall not be processed any further.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(Std_ReturnType, DOIP_CODE) DoIP_Udp_Check_GenHeader
(
  PduIdType DoIP_SoAdRxPduId,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_DATA) PduInfoPtr,
  P2VAR(uint16, AUTOMATIC, DOIP_APPL_DATA) PayloadTypePtr,
  P2VAR(uint32, AUTOMATIC, DOIP_APPL_DATA) PayloadLengthPtr,
  P2VAR(Std_ReturnType, AUTOMATIC, DOIP_APPL_DATA) Ret_IfTransmitPtr
);

/** \brief Internal service to perform generic DoIP header checks.
 *
 * This internal service checks the generic header and sends NACK messages if an error is detected.
 * Additionally it will return the payload type and payload length from the header.
 *
 * \param[in] DoIP_SoAdRxPduId - SoAd Rx PDU ID the header belongs to.
 * \param[in] PduInfoPtr - Pointer to the header.
 * \param[out] PayloadTypePtr - Pointer to be used to return the payload type.
 * \param[out] PayloadLengthPtr - Pointer to be used to return the payload length.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(void, DOIP_CODE) DoIP_Tcp_Check_GenHeader
(
  DoIP_TcpConIdxType TcpConIdx,
  P2CONST(uint8, AUTOMATIC, DOIP_APPL_DATA) GenHdrPtr
);

/** \brief Internal service to find available payload length.
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[in] FillDiagMsgPldFp - Function pointer for filling diagnostic message payload.
 * \param[out] AvailablePldDataPtr - Pointer to the available payload data.
 *
 * \return BufReq_ReturnType
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_GetAvailPayloadLength
(
  DoIP_TcpConIdxType TcpConIdx,
  DoIP_FillDiagMsgPldFpType FillDiagMsgPldFp,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_DATA) AvailablePldDataPtr
);

/** \brief Internal service to fill DoIP message during transmission process
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[in] HdrLength - The length of a message that needs to be generated (filled).
 * \param[in/out] PduInfoType - Pointer to a structure where message should be filled.
 * \param[out] AvailableDataPtr - The remaining size of the message that needs to be transmitted.
 * \param[in] FillMsgHdrFp - Pointer to a function that fills message headers.
 * \param[in] FillDiagMsgPldFp - Pointer to a function that fills diagnostic message payload.
 *
 * \return BufReq_ReturnType
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_FillMsg
(
  DoIP_TcpConIdxType TcpConIdx,
  PduLengthType HdrLength,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_DATA) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_DATA) AvailableDataPtr,
  DoIP_FillMsgHdrFpType FillMsgHdrFp,
  DoIP_FillDiagMsgPldFpType FillDiagMsgPldFp
);

/** \brief Internal service to fill routing activation response message.
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[out] SduDataPtr - Pointer where message should be filled.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(void, DOIP_CODE) DoIP_FillRoutingActResponse
(
  DoIP_TcpConIdxType TcpConIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) SduDataPtr
);

/** \brief Internal service to fill generic header negative acknowledge message
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[out] SduDataPtr - Pointer where message should be filled.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(void, DOIP_CODE) DoIP_FillGenHeaderNack
(
  DoIP_TcpConIdxType TcpConIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) SduDataPtr
);

/** \brief Internal service to fill Alive check request message
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[out] SduDataPtr - Pointer where message should be filled.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(void, DOIP_CODE) DoIP_FillAliveCheckReq
(
  DoIP_TcpConIdxType TcpConIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) SduDataPtr
);

/** \brief Internal service to fill diagnostic header.
 *
 * \param[out] PduPtr - Pointer to store header.
 * \param[in] TcpConIdx - Index of the used TCP connection.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(void, DOIP_CODE) DoIP_Fill_DiagHeader
(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) PduPtr,
  DoIP_TcpConIdxType TcpConIdx
);

/** \brief Internal service to fill diagnostic positive/negative acknowledge message header.
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[out] PduPtr - Address where header shell be copied.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(void, DOIP_CODE) DoIP_Fill_DiagAckNackHeader
(
  DoIP_TcpConIdxType TcpConIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) PduPtr
);


/** \brief Internal service to send vehicle identification response and announcement.
 *
 * \param[in] SoAdTxPduId - SoAd Tx PDU ID for sending.
 *
 * \return  Returns value of SoAd_IfTransmit().
 *
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(Std_ReturnType, DOIP_CODE) DoIP_Send_VID_Resp_Ann
(
  PduIdType SoAdTxPduId
);

/** \brief Internal service to send Diagnostic entity status response.
 *
 * \param[in] SoAdTxPduId - SoAd Tx PDU ID for sending.
 *
 * \return  Returns value of SoAd_IfTransmit().
 *
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(Std_ReturnType, DOIP_CODE) DoIP_Send_Entity_Status_Resp
(
  PduIdType SoAdTxPduId
);

#if(DOIP_POWER_MODE == STD_ON)
/** \brief Internal service to send Diagnostic power mode information response.
 *
 * \param[in] SoAdTxPduId - SoAd Tx PDU ID for sending.
 *
 * \return  Returns value of SoAd_IfTransmit().
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(Std_ReturnType, DOIP_CODE) DoIP_Send_PowerMode_Resp
(
  PduIdType SoAdTxPduId
);
#endif /* DOIP_POWER_MODE == STD_ON */



/** \brief Internal service to reset a TCP connection.
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(void, DOIP_CODE) DoIP_ResetTcpCon
(
  DoIP_TcpConIdxType TcpConIdx
);

/** \brief Internal service to perform routing activation.
 *
 * This internal service evaluates a routing activation request, calculates activation response
 * code and enables routes if applicable.
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[in] PduPtr - Pointer to routing activation request payload.
 * \param[in] PayloadLength - Payload length of a routing activation request message (7 or 11)
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(void, DOIP_CODE) DoIP_RoutingActivation
(
  DoIP_TcpConIdxType TcpConIdx,
  P2CONST(uint8, AUTOMATIC, DOIP_APPL_DATA) PduPtr,
  PduLengthType PayloadLength
);

/** \brief Internal service to perform routing activation of standard channels.
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[in] PayloadLength - Payload length of a routing activation request message (7 or 11).
 * \param[in] PduPtr - Pointer to routing activation request payload.
 * \param[out] TxStatePtr - Pointer to TxState for sending acknowledge message.
 *
 * \return BufReq_ReturnType
 * \retval BUFREQ_OK - Function execution was successful.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(uint8, DOIP_CODE) DoIP_RoutingActivationStandardChannels
(
  DoIP_TcpConIdxType TcpConIdx,
  PduLengthType PayloadLength,
  P2CONST(uint8, AUTOMATIC, DOIP_APPL_DATA) PduPtr,
  P2VAR(DoIP_TcpConTxStateType, AUTOMATIC, DOIP_APPL_DATA) TxStatePtr
);

/** \brief Internal service to fill diagnostic message positive/negative acknowledge message headers
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[out] SduDataPtr - Pointer where message should be filled.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(void, DOIP_CODE) DoIP_FillDiagAckNackHdr
(
  DoIP_TcpConIdxType TcpConIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) SduDataPtr
);

/** \brief Internal service to fill the next chunk of previous diag. message
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[out] PduInfoPtr - Pointer to the buffer where message shall be copied
 * \param[out] AvailableDataPtr - The remaining number of bytes
 *
 * \return BufReq_ReturnType
 * \retval BUFREQ_OK - Function execution was successful.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_FillDiagAckNackPld
(
  DoIP_TcpConIdxType TcpConIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_DATA) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_DATA) AvailableDataPtr
);

/** \brief Internal service to verify routing activation authentication and confirmation
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[in] ChannelPtr - Pointer to the channel that needs to be verified
 * \param[out] TxStatePtr - Pointer to TxState for sending acknowledge message
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(uint8, DOIP_CODE) DoIP_VerifyAuthenticationConfirmation
(
  DoIP_TcpConIdxType TcpConIdx,
  P2VAR(DoIP_ChannelType, AUTOMATIC, DOIP_APPL_DATA) ChannelPtr,
  P2VAR(DoIP_TcpConTxStateType, AUTOMATIC, DOIP_APPL_DATA) TxStatePtr
);

/** \brief Internal service to verify routing activation authentication
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[in] ChannelPtr - Pointer to the channel that needs to be authentified
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(uint8, DOIP_CODE) DoIP_VerifyAuthentication
(
  DoIP_TcpConIdxType TcpConIdx,
  P2VAR(DoIP_ChannelType, AUTOMATIC, DOIP_APPL_DATA) ChannelPtr
);

/** \brief Internal service to verify routing activation confirmation
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[in] ChannelPtr - Pointer to the channel that needs to be confirmed
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(uint8, DOIP_CODE) DoIP_VerifyConfirmation
(
  DoIP_TcpConIdxType TcpConIdx,
  P2VAR(DoIP_ChannelType, AUTOMATIC, DOIP_APPL_DATA) ChannelPtr
);

/** \brief Internal service to call the list of routing activation callback functions
 *
 * \param[in] TcpConPtr - Pointer to Tcp connection run-time data where routing activation was
 *                        successfully performed.
 * \param[in] ActivationType - Activation type from the received routing activation message.
 * \param[in] OemSpecificPtr - Pointer to OEM specific field from the received routing activation
 *                             message. NULL_PTR if OEM specific field was not received.
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
#if (DOIP_ROUTING_ACTIVATION_CALLBACK_NUM > 0U)
STATIC FUNC(void, DOIP_CODE) DoIP_CallRoutingActivationCallbacks
(
  P2CONST(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr,
  uint8 ActivationType,
  P2CONST(uint8, AUTOMATIC, DOIP_APPL_DATA) OemSpecificPtr
);
#endif /* DOIP_ROUTING_ACTIVATION_CALLBACK_NUM > 0U */

/** \brief Internal service to handle receiving routing activation mesasage.
 *
 * This internal service evaluates a routing activation message and assembles it if segmented.
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[in] PduInfoPtr - Pointer to routing activation request payload.
 * \return BufReq_ReturnType
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_HandleRoutingActivation
(
  DoIP_TcpConIdxType TcpConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_DATA) PduInfoPtr
);

/** \brief Internal service to send Alive check request message on specified Tcp connection
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 *
 * \return Std_ReturnType - E_OK - Alive check request message transmission initiated
 *                          E_NOT_OK - Alive check request not transmitted - connection
 *                                     shall be considered alive.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(Std_ReturnType, DOIP_CODE) DoIP_SendAliveCheckRequest
(
  DoIP_TcpConIdxType TcpConIdx
);

/** \brief Internal service to handle receiving Generic Header.
 *
 * This internal service handles receiving of Generic Header in state DOIP_TCPCON_RX_READY.
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[in] PduInfoPtr - Pointer to routing activation request payload.
 * \return BufReq_ReturnType
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_HandleRxStartReady
(
  DoIP_TcpConIdxType TcpConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_DATA) PduInfoPtr
);

/** \brief Internal service to handle receiving Diagnostic Header.
 *
 * This internal service handles receiving of Diagnostic Header and assembles it if segmented.
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[in] PduInfoPtr - Pointer to routing activation request payload.
 * \return BufReq_ReturnType
 * \retval BUFREQ_OK - Function execution was successful.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_HandleDiagHeader
(
  DoIP_TcpConIdxType TcpConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_DATA) PduInfoPtr
);

/** \brief Internal service to find a route (channel) upon reception of diagnostic message
 *
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[in] SourceAddr - Source address from the received diagnostic message
 * \param[in] TargetAddr - Target address from the received diagnostic message
 * \param[out] RespCodePtr - Pointer to Diagnostic message response code
 *
 * \return BufReq_ReturnType
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_FindDiagMsgRoute
(
  DoIP_TcpConIdxType TcpConIdx,
  uint16 SourceAddr,
  uint16 TargetAddr,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) RespCodePtr
);

/** \brief Internal service to start a diagnostic message reception.
 *
 * This internal service searches for the corresponding route to PduR and starts a reception at the
 * upper layer.
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[in] PduPtr - Pointer to the diagnostic message header.
 *
 * \return Std_ReturnType
 * \retval BUFREQ_OK - Function execution was successful.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_StartDiagReception
(
  DoIP_TcpConIdxType TcpConIdx,
  P2CONST(uint8, AUTOMATIC, DOIP_APPL_DATA) PduPtr
);

/** \brief Internal service to copy a diagnostic message reception.
 *
 * This internal service passes the received diagnostic message to the PduR and sends a diagnostic
 * acknowledgment when finished.
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[in] PduPtr - Pointer to the diagnostic message.
 *
 * \return Std_ReturnType - Passes the return value of the PduR API call.
 * \retval BUFREQ_OK - Copy was successful
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_CopyDiagMsg
(
  DoIP_TcpConIdxType TcpConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_DATA) PduInfoPtr
);

/** \brief Internal service to discard diagnostic message payload
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[in] PduInfoPtr - Pointer to the diagnostic message.
 *
 * \return BufReq_ReturnType
 * \retval BUFREQ_OK - Discard was successful.
 * \retval BUFREQ_E_NOT_OK - More data are provided than expected.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_DiscardDiagMsg
(
  DoIP_TcpConIdxType TcpConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_DATA) PduInfoPtr
);

/** \brief Internal service to discard DoIP message payload after generic header
 *
 * In case DoIP message needs to be discarded (ignored) it is necessary to consume the remaining
 * of the message from SoAd, in order to free its buffers, and synchronize to the beginning
 * of the next generic header.
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[in] PduInfoPtr - Pointer to the diagnostic message.
 *
 * \return BufReq_ReturnType
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_DiscardMsg
(
  DoIP_TcpConIdxType TcpConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_DATA) PduInfoPtr
);


/** \brief Internal service to handle reception of Alive check response message
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[in] PduInfoPtr - Pointer to the diagnostic message.
 *
 * \return BufReq_ReturnType
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_HandleAliveCheckResp
(
  DoIP_TcpConIdxType TcpConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_DATA) PduInfoPtr
);

/** \brief Internal service to calculate Tx message SduLength
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[in] TxState - Internal TCP Tx connection state
 *
 * \return PduLengthType - Tx message length (SduLength)
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(PduLengthType, DOIP_CODE) DoIP_GetTxSduLength
(
  DoIP_TcpConIdxType TcpConIdx,
  DoIP_TcpConTxStateType TxState
);

/** \brief Internal service to fill generic and diagnostic header into provided buffer
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[out] SduDataPtr - Pointer where message should be filled.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(void, DOIP_CODE) DoIP_FillDiagMsgHdr
(
  DoIP_TcpConIdxType TcpConIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) SduDataPtr
);

/** \brief Internal service to fill diagnostic message payload into provided buffer
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[out] PduInfoPtr - Pointer where Tx diagnostic message should be stored.
 * \param[out] AvailableDataPtr - Pointer to the remaining number of bytes available in PduR.

 * \return BufReq_ReturnType - Passes the return value of the PduR API call.
 * \retval BUFREQ_OK - Copy was successful
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_FillDiagMsgPld
(
  DoIP_TcpConIdxType TcpConIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_DATA) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_DATA) AvailableDataPtr
);

/** \brief Internal service to request/release Ip address assignment for a single socket connection
 *
 * \param[in] SoConId - Socket connection id for which Ip address request/release should be done
 * \param[in] Operation - DOIP_REQUEST_IP_ADDR_OP / DOIP_RELEASE_IP_ADDR_OP
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(void, DOIP_CODE) DoIP_SoAdRequestReleaseIpAddr
(
  SoAd_SoConIdType SoConId,
  DoIP_OperationType Operation
);

/** \brief Internal service to request/release Ip address assignment for all Tcp/Udp connections
 *
 * \param[in] Operation - DOIP_REQUEST_IP_ADDR_OP / DOIP_RELEASE_IP_ADDR_OP
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(void, DOIP_CODE) DoIP_RequestReleaseIpAddrAllConn
(
    DoIP_OperationType Operation
);

/** \brief Internal service to retrieve the value of DoIPUpdateDhcpHostNameOption for SoConId
 *
 * \param[in] SoConId - Socket connection id for which configuration field should be retrieved
 *
 * \return The value of DoIPUpdateDhcpHostNameOption
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(boolean, DOIP_CODE) DoIP_GetUpdateHostnameOption
(
  SoAd_SoConIdType SoConId
);

/** \brief Internal service to open all configured Tcp/Udp connections
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(void, DOIP_CODE) DoIP_OpenAllCon(void);

/** \brief Internal service to close all configured Tcp/Udp connections
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(void, DOIP_CODE) DoIP_CloseAllCon(void);

/** \brief Helper function to construct a 32 bit value from bytes.
 *
 * \param[in] Byte3_Msb - Most significant byte of the 32 bit value.
 * \param[in] Byte2 - Next higher byte of the 32 bit value.
 * \param[in] Byte1 - Next higher byte of the 32 bit value.
 * \param[in] Byte0_Lsb - Least significant byte of the 32 bit value.
 *
 * \return The function returns the put together 32 bit value.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(uint32, DOIP_CODE) DoIP_Make_uint32
(
  uint8 Byte3_Msb,
  uint8 Byte2,
  uint8 Byte1,
  uint8 Byte0_Lsb
);

/** \brief Internal service to process confirmation of Diagnostic message
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[in] Result - Parameter indicates the result of the transmission.
 *
 * \return ContinueTransmission - TRUE in case of a long message, where transmission is continued
 *
 * \Reentrancy{Non reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(boolean, DOIP_CODE) DoIP_HandleTxDiagMsgConfirmation
(
  DoIP_TcpConIdxType TcpConIdx,
  NotifResultType Result
);

/** \brief Internal service to release a remote address of a UDP connection.
 *
 * \param[in] DoIPTxPduId - Index corresponding DoIP Tx Pdu for the UDP connection.
 *
 * \Reentrancy{Non reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(void, DOIP_CODE) DoIP_UdpReleaseRemoteAddr
(
  PduIdType DoIPTxPduId
);

#if (DOIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)
/** \brief Internal service to increase given Measurement drop data counter.
 *
 * \param[in] MeasurementIdx - Index of corresponding measurement data counter.
 *
 * \Reentrancy{Non reentrant}
 * \Synchronicity{Synchronous}
*/
STATIC FUNC(void, DOIP_CODE) DoIP_IncMeasurementDropDataCounter
(
  DoIP_MeasurementIdxType MeasurementIdx
);
#endif /* DOIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */

#define DOIP_STOP_SEC_CODE
#include <DoIP_MemMap.h>
/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

#define DOIP_START_SEC_VAR_INIT_UNSPECIFIED
#include <DoIP_MemMap.h>

P2CONST(DoIP_ConfigType, DOIP_VAR, DOIP_APPL_CONST) DoIP_RootPtr = NULL_PTR;

#define DOIP_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <DoIP_MemMap.h>

#define DOIP_START_SEC_VAR_INIT_8
#include <DoIP_MemMap.h>

VAR(boolean, DOIP_VAR) DoIP_Initialized = FALSE;

#define DOIP_STOP_SEC_VAR_INIT_8
#include <DoIP_MemMap.h>

#define DOIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <DoIP_MemMap.h>

/* Data common to standard and custom payload channels. */
VAR(DoIP_ChannelType, DOIP_VAR) DoIP_Channel[DOIP_MAX_CHANNELS];

/* Allow DOIP_MAX_PARALLEL_TESTER parallel TCP connections to be handled and use the +1 for
 * Alive check processing. */
VAR(DoIP_TcpConType, DOIP_VAR) DoIP_TcpCon[DOIP_MAX_PARALLEL_TESTER + 1U];

#define DOIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <DoIP_MemMap.h>

/*==================[internal data]=========================================*/

#define DOIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <DoIP_MemMap.h>

/* DoIP entity Group identifier. */
STATIC VAR(uint8, DOIP_VAR) DoIP_GID[DOIP_GID_SIZE];

/* Entity identification */
STATIC VAR(uint8, DOIP_VAR) DoIP_EID[DOIP_EID_SIZE];

#define DOIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <DoIP_MemMap.h>

#define DOIP_START_SEC_VAR_CLEARED_8
#include <DoIP_MemMap.h>

STATIC VAR(DoIP_ActivationLineStateType, DOIP_VAR) DoIP_ActivationLineState;

STATIC VAR(uint8, DOIP_VAR) DoIP_FurtherAction;

/* Setting this variable triggers Alive check on all registered connections. */
STATIC VAR(boolean, DOIP_VAR) DoIP_StartAllConnAliveCheck;

/* Holds information if Dynamic synchronization process finished. */
STATIC VAR(boolean, DOIP_VAR) DoIP_GIDSyncCompleted;

/* Holds count of sent Vehicle announcement response messages */
STATIC VAR(uint8, DOIP_VAR) DoIP_VehicleAnnSendCounter[DOIP_MAX_VEHICLE_ANNOUNCEMENT_CON];

/* DHCP HostName in ASCII */
STATIC VAR(uint8, DOIP_VAR) DoIP_DhcpHostName[DOIP_HOSTNAME_SIZE_MAX];

#define DOIP_STOP_SEC_VAR_CLEARED_8
#include <DoIP_MemMap.h>

#define DOIP_START_SEC_VAR_CLEARED_16
#include <DoIP_MemMap.h>

/* Index of a connection that initiated all connection Alive check. */
STATIC VAR(DoIP_TcpConIdxType, DOIP_VAR) DoIP_NoFreeTcpConIdx;

/* Number of connections with active single connection Alive check. */
STATIC VAR(DoIP_TcpConIdxType, DOIP_VAR) DoIP_NumActiveSingleAliveCheck;

/* Counter of active Alive check timers. */
STATIC VAR(uint16, DOIP_VAR) DoIP_AliveCheckTimerCount;

/* Index of the first Tcp connection (smallest TcpConIdx) that has channels pending on routing
 * activation / confirmation callback functions. */
STATIC VAR(DoIP_TcpConIdxType, DOIP_VAR) DoIP_PendingAuthConfTcpConIdx;

#define DOIP_STOP_SEC_VAR_CLEARED_16
#include <DoIP_MemMap.h>

#define DOIP_START_SEC_VAR_CLEARED_32
#include <DoIP_MemMap.h>

/* General DoIP running timer */
STATIC VAR(uint32, DOIP_VAR) DoIP_GeneralTimer;

/* Timer for monitoring multiple types of deadlines. Timeout array is filled with 4 different types
 * of timeouts and they are filled in following order: UDP Release, Inactivity timer, Alive check
 * and Vehicle announcement timer. */
STATIC VAR(DoIP_TimeoutType, DOIP_VAR) DoIP_TimeoutArr[DOIP_MAX_TIMEOUT_VALUES];
STATIC VAR(DoIP_TimeoutType, DOIP_VAR) DoIP_NextTimeout;

#define DOIP_STOP_SEC_VAR_CLEARED_32
#include <DoIP_MemMap.h>

#define DOIP_START_SEC_VAR_INIT_32
#include <DoIP_MemMap.h>

#if (DOIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)
/* Counter for number of dropped invalid Generic hdr messages */
VAR(uint32, DOIP_VAR) DoIP_MeasurementDropData_GenHdr = 0U;
/* Counter for number of dropped Diag messages */
VAR(uint32, DOIP_VAR) DoIP_MeasurementDropData_DiagMsg = 0U;
#endif /* DOIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */

#define DOIP_STOP_SEC_VAR_INIT_32
#include <DoIP_MemMap.h>

/*==================[external function definitions]=========================*/

#if DOIP_ISVALIDCONFIG_MMAP_ENABLED == STD_ON
#define DOIP_START_SEC_CODE_CC_BLOCK
#else
#define DOIP_START_SEC_CODE
#endif
#include <DoIP_MemMap.h>

FUNC(Std_ReturnType, DOIP_CODE) DoIP_IsValidConfig
(
  P2CONST(void, AUTOMATIC, DOIP_APPL_CONST) voidConfigPtr
)
{
  /* Assume an invalid configuration. */
  Std_ReturnType RetVal = E_NOT_OK;
  P2CONST(DoIP_ConfigType, AUTOMATIC, DOIP_APPL_CONST) ConfigPtr = voidConfigPtr;

  DBG_DOIP_ISVALIDCONFIG_ENTRY(voidConfigPtr);

  if(ConfigPtr != NULL_PTR)
  {
    /* Check if the configuration fits to the platform. */
    if(TS_PlatformSigIsValid(ConfigPtr->PlatformSignature) == TRUE)
    {
      /* Validate the post build configuration against the compile time configuration. */
      if(DOIP_CFG_SIGNATURE == ConfigPtr->CfgSignature)
      {
        /* Validate the post build configuration against the link time configuration. */
        if(DoIP_LcfgSignature == ConfigPtr->LcfgSignature)
        {
          /* Validate the post build configuration against the compile time published information
             configuration. */
          if(DOIP_PUBLIC_INFO_SIGNATURE == ConfigPtr->PublicInfoSignature)
          {
            /* Indicate that the configuration is valid. */
            RetVal = E_OK;
          }
        }
      }
    }
  }

  DBG_DOIP_ISVALIDCONFIG_EXIT(RetVal, voidConfigPtr);
  return RetVal;
}

#if DOIP_ISVALIDCONFIG_MMAP_ENABLED == STD_ON
#define DOIP_STOP_SEC_CODE_CC_BLOCK
#include <DoIP_MemMap.h>

#define DOIP_START_SEC_CODE
#include <DoIP_MemMap.h>
#endif

#if(DOIP_VERSION_INFO_API == STD_ON)
FUNC(void, DOIP_CODE) DoIP_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, DOIP_APPL_DATA) VersionInfo
)
{
  DBG_DOIP_GETVERSIONINFO_ENTRY(VersionInfo);
#if(DOIP_DEV_ERROR_DETECT == STD_ON)
  if(VersionInfo == NULL_PTR)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_GETVERSIONINFO, DOIP_E_PARAM_POINTER);
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* Return version information */
    VersionInfo->vendorID = DOIP_VENDOR_ID;
    VersionInfo->moduleID = DOIP_MODULE_ID;
    VersionInfo->sw_major_version = DOIP_SW_MAJOR_VERSION;
    VersionInfo->sw_minor_version = DOIP_SW_MINOR_VERSION;
    VersionInfo->sw_patch_version = DOIP_SW_PATCH_VERSION;
  }
  DBG_DOIP_GETVERSIONINFO_EXIT(VersionInfo);
}
#endif /* DOIP_VERSION_INFO_API */

FUNC(void, DOIP_CODE) DoIP_Init
(
  P2CONST(DoIP_ConfigType, AUTOMATIC, DOIP_APPL_CONST) DoIP_ConfigPtr
)
{
  P2CONST(DoIP_ConfigType, AUTOMATIC, DOIP_APPL_CONST) LocalConfigPtr = DoIP_ConfigPtr;
  uint16 Idx;
  uint8 InvalidityVin[DOIP_VIN_SIZE];

  DBG_DOIP_INIT_ENTRY(DoIP_ConfigPtr);

  /* Save the configuration pointer */
#if (DOIP_PBCFGM_SUPPORT_ENABLED == STD_ON)
  /* If the initialization function is called with a null pointer get the configuration from the
   * post build configuration manager. */
  if(LocalConfigPtr == NULL_PTR)
  {
    PbcfgM_ModuleConfigPtrType ModuleConfigPtr = NULL_PTR;
    if(E_OK == PbcfgM_GetConfig(
                                 DOIP_MODULE_ID,
                                 DOIP_INSTANCE_ID,
                                 &ModuleConfigPtr)
                               )
    {
      LocalConfigPtr = (P2CONST(DoIP_ConfigType, AUTOMATIC, DOIP_APPL_CONST)) ModuleConfigPtr;
    }
  }
#endif /* DOIP_PBCFGM_SUPPORT_ENABLED == STD_ON */

#if(DOIP_DEV_ERROR_DETECT == STD_ON)
  if(LocalConfigPtr == NULL_PTR)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_INIT, DOIP_E_PARAM_POINTER);
  }
  /* check that configuration pointer is valid */
  else if (E_OK != DoIP_IsValidConfig(LocalConfigPtr))
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_INIT, DOIP_E_PARAM_POINTER);
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    SchM_Enter_DoIP_SCHM_DOIP_EXCLUSIVE_AREA_0();
    /* Store the post-build pointer */
    DoIP_RootPtr = LocalConfigPtr;

    DoIP_ActivationLineState = DOIP_ACT_LINE_STATE_INACTIVE;
    if(DoIP_RootPtr->FurtherActionReq == TRUE)
    {
      DoIP_FurtherAction = DOIP_FURTHER_ACTION_ROUTE_ACT;
    }
    else
    {
      DoIP_FurtherAction = DOIP_FURTHER_ACTION_NON;
    }

    /* Initialize timeout array. */
    for(Idx = 0; Idx < DOIP_MAX_TIMEOUT_VALUES; Idx++)
    {
      DoIP_TimeoutArr[Idx] = DOIP_INVALID_TIMEOUT_VALUE;
    }

    DoIP_NextTimeout = DOIP_INVALID_TIMEOUT_VALUE; /* No timeout is scheduled. */

    /* Initialize vehicle announcement array. */
    for(Idx = 0U; Idx < DoIP_RootPtr->UdpAnnConMax; Idx++)
    {
      DoIP_VehicleAnnSendCounter[Idx] = 0U;
    }

    /* Initialize channels */
    for(Idx = 0U; Idx < DOIP_MAX_CHANNELS; Idx++)
    {
      DoIP_Channel[Idx].PendingRequest = FALSE;
      DoIP_Channel[Idx].RoutingActivationState = DOIP_ROUTING_INACTIVE;
    }

    /* Initialize TCP connection manager */
    for(Idx = 0U; Idx < (DOIP_MAX_PARALLEL_TESTER + 1U); Idx++)
    {
      CONSTP2VAR(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr = &DoIP_TcpCon[Idx];
      TcpConPtr->RxState = DOIP_TCPCON_RX_OFFLINE;
      TcpConPtr->TxState = DOIP_TCPCON_TX_OFFLINE;
      TcpConPtr->NextDoIPPduRPduId = DOIP_INVALID_PDUID;
      TcpConPtr->LastDoIPPduRPduId = DOIP_INVALID_PDUID;
      TcpConPtr->DoIPSoAdPduId = DOIP_INVALID_PDUID;
      TcpConPtr->DoIPPduRRxPduId = DOIP_INVALID_PDUID;
      TcpConPtr->DoIPPduRTxPduId = DOIP_INVALID_PDUID;
      TcpConPtr->ConnectionState = DOIP_TCP_CONSTATE_INIT;
      TcpConPtr->TxMsgPos = 0U;
      TcpConPtr->ReadPos = 0U;
      TcpConPtr->WritePos = 0U;
      TcpConPtr->NextPendingRouteIdx = DOIP_INVALID_ROUTE_IDX;
      TcpConPtr->MultipleAuthConfPending = FALSE;
      TcpConPtr->TesterPtr = NULL_PTR;
    }

    /* No Tcp connection has channels pending on routing activation. */
    DoIP_PendingAuthConfTcpConIdx = DOIP_INVALID_TCP_CON_IDX;

    /* Fill DoIPVinInvalidityPattern */
    TS_MemSet(&InvalidityVin[0U], DOIP_VIN_INVALIDITY_PATTERN, DOIP_VIN_SIZE);

    /* Alive check is disabled. */
    DoIP_AliveCheckTimerCount = 0U;
    DoIP_NoFreeTcpConIdx = DOIP_INVALID_TCP_CON_IDX;
    DoIP_NumActiveSingleAliveCheck = 0U;
    DoIP_StartAllConnAliveCheck = FALSE;
    DoIP_GeneralTimer = 0U;

    /* Get the MAC address if set or use DoIPEid if configured, otherwise DoIPGIDInvalidityPattern */
#if(DOIP_USE_MAC_ADDR_ENABLE == STD_ON)
    {
      uint8 PhysAddr[DOIP_EID_SIZE];
      SoAd_SoConIdType SoConId = SOAD_INVALID_SOCON_ID;

      (void) SoAd_GetSoConId(DOIP_PBCFG_UDPANNCON(0U).SoAdTxPduId, &SoConId);

      if(SoAd_GetPhysAddr(SoConId, PhysAddr) == E_OK)
      {
        /* Set EID to received MacAddress */
        TS_MemCpy(DoIP_EID, PhysAddr, DOIP_EID_SIZE);
      }
      else
      {
        /* Set EID to DoIPEid if configured, otherwise DoIPGIDInvalidityPattern */
        TS_MemCpy(DoIP_EID, DoIP_RootPtr->EntityId, DOIP_EID_SIZE);
      }
    }
#else
    TS_MemCpy(DoIP_EID, DoIP_RootPtr->EntityId, DOIP_EID_SIZE);
#endif /* DOIP_USE_MAC_ADDR_ENABLE == STD_ON */

#if(DOIP_USE_EID_AS_GID_ENABLED == STD_ON)
    /* Set GID according to EID. */
    TS_MemCpy(DoIP_GID, DoIP_EID, DOIP_GID_SIZE);
#else
    /* Set DoIP_GID according to DoIPGid if configured, to DoIPGIDInvalidityPattern otherwise. */
    TS_MemCpy(DoIP_GID, DoIP_RootPtr->GroupId, DOIP_GID_SIZE);
#endif /* DOIP_USE_EID_AS_GID_ENABLED == STD_ON */

    DoIP_GIDSyncCompleted = FALSE;

#if (DOIP_DYN_GID_SYNC_ENABLED == STD_ON)
    {
      boolean DoIP_IsGIDMaster = FALSE;

      DoIP_User_SyncGid_Fp(&DoIP_IsGIDMaster, &DoIP_GID[0U]);

      if(DoIP_IsGIDMaster == TRUE)
      {
        DoIP_GIDSyncCompleted = TRUE;
      }
    }
#endif  /* DOIP_DYN_GID_SYNC_ENABLED == STD_ON */

    TS_AtomicAssign8(DoIP_Initialized, TRUE);

    SchM_Exit_DoIP_SCHM_DOIP_EXCLUSIVE_AREA_0();
  }
  DBG_DOIP_INIT_EXIT(DoIP_ConfigPtr);
}

#if(DOIP_ENABLE_MAIN_FUNCTION_TX != STD_ON)
STATIC FUNC(void, DOIP_CODE) DoIP_MainFunctionTx(void)
#else
FUNC(void, DOIP_CODE) DoIP_MainFunctionTx(void)
#endif
{
  DBG_DOIP_MAINFUNCTIONTX_ENTRY();

  if(DoIP_Initialized == TRUE)
  {
    DoIP_TcpConIdxType Idx;

    for(Idx = 0U; Idx < (DOIP_MAX_PARALLEL_TESTER + 1U); Idx++)
    {
      CONSTP2VAR(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr = &DoIP_TcpCon[Idx];
      PduIdType DoIPSoAdPduId;
      PduIdType NextDoIPPduRPduId;
      PduLengthType TxDiagMsgLength = 0U;
      boolean PendingRequest = FALSE;
      DoIP_TcpConTxStateType TxState = DOIP_TCPCON_TX_OFFLINE;
      PduLengthType HeaderSize = DOIP_GEN_HEADER_SIZE;
      SchM_Enter_DoIP_SCHM_DOIP_EXCLUSIVE_AREA_0();

      DoIPSoAdPduId = TcpConPtr->DoIPSoAdPduId;
      NextDoIPPduRPduId = TcpConPtr->NextDoIPPduRPduId;

      if(NextDoIPPduRPduId != DOIP_INVALID_PDUID)
      {
        TxDiagMsgLength = DoIP_Channel[NextDoIPPduRPduId].TxDiagMsgLength;
        PendingRequest = DoIP_Channel[NextDoIPPduRPduId].PendingRequest;

        TxState = TcpConPtr->TxState;
        if(DOIP_TCPCON_TX_IDLE == TxState)
        {
          if(PendingRequest)
          {
#if (DOIP_CUSTOM_PAYLOAD_TYPE_ENABLED == STD_ON)
            if(NextDoIPPduRPduId >= DoIP_RootPtr->StdChannelMax)
            {
              TS_AtomicAssign8(TcpConPtr->TxState, DOIP_TCPCON_TX_CUSTOM_MSG_START);
              /* No diagnostic header in case of custom payload type messages. */
            }
            else
#endif
            {
              TS_AtomicAssign8(TcpConPtr->TxState, DOIP_TCPCON_TX_DIAG_MSG_START);
              HeaderSize += DOIP_DIAG_HEADERSIZE;
            }
            TcpConPtr->DoIPPduRTxPduId = NextDoIPPduRPduId;
          }
        }
      }

      SchM_Exit_DoIP_SCHM_DOIP_EXCLUSIVE_AREA_0();

      if(NextDoIPPduRPduId != DOIP_INVALID_PDUID)
      {
        /* Check if no other transmission on this TCP connection is in progress. */
        if(DOIP_TCPCON_TX_IDLE == TxState)
        {
          if(PendingRequest)
          {
            Std_ReturnType Ret;
            PduInfoType SoAdPduInfo;
            const PduIdType SoAdTxPduId = DOIP_PBCFG_TCPCON(DoIPSoAdPduId).SoAdTxPduId;
            SoAdPduInfo.SduDataPtr = NULL_PTR;

            /* Size of a DiagMsg received from upper layer is limited to 64Kb, so as PDU size sent to
             * SoAd. In addition to the original message DoIP inserts Generic and DiagMsg headers
             * (8 + 4 bytes), thus the limit below. */
            if((TxDiagMsgLength + HeaderSize) > DOIP_PDULENGTHTYPE_MAX)
            {
              SoAdPduInfo.SduLength = DOIP_PDULENGTHTYPE_MAX;
            }
            else
            {
              SoAdPduInfo.SduLength = TxDiagMsgLength + HeaderSize;
            }
            Ret = SoAd_TpTransmit(SoAdTxPduId, &SoAdPduInfo);

            if(E_OK == Ret)
            {
              /* Reset timeout */
              TS_AtomicAssign8(TcpConPtr->ResetGeneralInactivityTimer, TRUE);
            }
            else
            {
              TS_AtomicAssign8(TcpConPtr->TxState, DOIP_TCPCON_TX_IDLE);
            }
          }
        }
      }
    }
  }
  DBG_DOIP_MAINFUNCTIONTX_EXIT();
}

FUNC(void, DOIP_CODE) DoIP_MainFunction(void)
{
  DBG_DOIP_MAINFUNCTION_ENTRY();
  /* Proceed only if initialized */
  if(DoIP_Initialized == TRUE)
  {
    DoIP_TcpConIdxType Idx;

#if(DOIP_DYN_GID_SYNC_ENABLED == STD_ON)
    if(DoIP_GIDSyncCompleted == FALSE)
    {
      Std_ReturnType RetVal;

      RetVal = DoIP_User_GetGid_Fp(&DoIP_GID[0U]);
      if(RetVal == E_OK)
      {
        DoIP_GIDSyncCompleted = TRUE;
      }
    }
#endif /* DOIP_DYN_GID_SYNC_ENABLED == STD_ON */

    /* Increment general timer. */
    DoIP_GeneralTimer = (DoIP_GeneralTimer + 1U) % DOIP_UINT32_MAX;

    /* call the transmit part of the main function */
    DoIP_MainFunctionTx();

    for(Idx = 0U; Idx < (DOIP_MAX_PARALLEL_TESTER + 1U); Idx++)
    {
      CONSTP2VAR(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr = &DoIP_TcpCon[Idx];

      /* TCP connection timeout handling */
      if(DOIP_TCPCON_RX_OFFLINE != TcpConPtr->RxState)
      {
        if(TcpConPtr->ResetGeneralInactivityTimer)
        {
          /* General inactivity timer is started only after routing activation process is
           * initiated, i.e. routing activation message received and source address registered. */
          if((TcpConPtr->ConnectionState == DOIP_TCP_CONSTATE_SA_REGISTERED_ROUTING_ACTIVE) ||
             (TcpConPtr->ConnectionState == DOIP_TCP_CONSTATE_SA_REGISTERED_PENDING))
          {
            DoIP_StartTimer(
                             DOIP_INACTIVITY_TIMER_INDEX(Idx),
                             DOIP_TCP_GENERAL_INACTIVITY_TIMEOUT
                            );
          }
          TS_AtomicAssign8(TcpConPtr->ResetGeneralInactivityTimer, FALSE);
        }
      }
    }

    if(DoIP_NextTimeout == DoIP_GeneralTimer)
    {
      DoIP_ProcessTimeout();
    }

    DoIP_ProcessAliveCheck();

    if(DoIP_PendingAuthConfTcpConIdx != DOIP_INVALID_TCP_CON_IDX)
    {
      /* Process pending routing activations waiting for authentication/confirmation results. */
      DoIP_ProcessAuthConfPendingRoutingActivations();
    }
  }
  DBG_DOIP_MAINFUNCTION_EXIT();
}

STATIC FUNC(boolean, DOIP_CODE) DoIP_IsTimerActive
(
  DoIP_TimeoutIdxType TimerIdx
)
{
  boolean RetVal = FALSE;

  if(DoIP_TimeoutArr[TimerIdx] != DOIP_INVALID_TIMEOUT_VALUE)
  {
    RetVal = TRUE;
  }

  return RetVal;
}

STATIC FUNC(void, DOIP_CODE) DoIP_StartTimer
(
  DoIP_TimeoutIdxType TimerIdx,
  DoIP_TimeoutType Timeout
)
{
  /* Set timeout value for the specified timer relative to the general timer. */
  DoIP_TimeoutArr[TimerIdx] =
    (DoIP_GeneralTimer + Timeout) % DOIP_UINT32_MAX;

  if(DoIP_NextTimeout == DOIP_INVALID_TIMEOUT_VALUE)
  {
    /* First active timer -> set it as the next (nearest). */
    DoIP_NextTimeout = DoIP_TimeoutArr[TimerIdx];
  }
  else if(((uint32) (DoIP_TimeoutArr[TimerIdx] - DoIP_GeneralTimer)) <
          ((uint32) (DoIP_NextTimeout - DoIP_GeneralTimer)))
  {
    /* Schedule it for the next timeout when shorter then the currently set. */
    DoIP_NextTimeout = DoIP_TimeoutArr[TimerIdx];
  }
  else
  {
    /* do_nothing */
  }

  /* Count number of active alive check timers */
  if((TimerIdx >= DOIP_ALIVE_CHECK_TIMER_INDEX_START) &&
      (TimerIdx < DOIP_ANNOUNCEMENT_TIMER_INDEX_START))
  {
    DoIP_AliveCheckTimerCount++;
  }

}

STATIC FUNC(void, DOIP_CODE) DoIP_StopTimer
(
  DoIP_TimeoutIdxType TimerIdx
)
{
  DoIP_TimeoutIdxType Idx;

  if(DoIP_TimeoutArr[TimerIdx] != DOIP_INVALID_TIMEOUT_VALUE)
  {
    if(DoIP_NextTimeout == DoIP_TimeoutArr[TimerIdx])
    {
      /* The timer we need to disable was scheduled for the next (nearest) one. */
      DoIP_TimeoutType Diff, DiffMin = DOIP_UINT32_MAX;

      /* Disable the timer. */
      DoIP_TimeoutArr[TimerIdx] = DOIP_INVALID_TIMEOUT_VALUE;

      DoIP_NextTimeout = DOIP_INVALID_TIMEOUT_VALUE;
      /* Find the next (with nearest expiration) timer. */
      for(Idx = 0U; Idx < DOIP_MAX_TIMEOUT_VALUES; Idx++)
      {
        if(DoIP_TimeoutArr[Idx] != DOIP_INVALID_TIMEOUT_VALUE)
        {
          /* Calculate next timeout. */
          Diff = (uint32) (DoIP_TimeoutArr[Idx] - DoIP_GeneralTimer);
          if(Diff < DiffMin)
          {
            DiffMin = Diff;
            DoIP_NextTimeout = DoIP_TimeoutArr[Idx];
          }
        }
      }
    }
    else
    {
      /* Simply disabled the timer. */
      DoIP_TimeoutArr[TimerIdx] = DOIP_INVALID_TIMEOUT_VALUE;
    }

    /* Count number of active alive check timers */
    if((TimerIdx >= DOIP_ALIVE_CHECK_TIMER_INDEX_START) &&
        (TimerIdx < DOIP_ANNOUNCEMENT_TIMER_INDEX_START))
    {
      DoIP_AliveCheckTimerCount--;
    }
  }
}

STATIC FUNC(void, DOIP_CODE) DoIP_ProcessTimeout(void)
{
  DoIP_TimeoutIdxType Idx;
  DoIP_TimeoutType Diff, DiffMin = DOIP_UINT32_MAX;

  DoIP_NextTimeout = DOIP_INVALID_TIMEOUT_VALUE;

  for(Idx = 0U; Idx < DOIP_MAX_TIMEOUT_VALUES; Idx++)
  {
    const DoIP_TimeoutType TimeoutValue = DoIP_TimeoutArr[Idx];

    if(TimeoutValue == DoIP_GeneralTimer)
    {
      boolean ResetTimer = TRUE;

      /* UDP release timeout handling */
      if(Idx < DOIP_INACTIVITY_TIMER_INDEX_START)
      {
        DoIP_UdpReleaseRemoteAddr(Idx);
      }
      /* Inactivity timer timeout handling */
      else if(Idx < DOIP_ALIVE_CHECK_TIMER_INDEX_START)
      {
        DoIP_ResetTcpCon(Idx - DOIP_INACTIVITY_TIMER_INDEX_START);
      }
      /* Alive check timeout handling */
      else if(Idx < DOIP_ANNOUNCEMENT_TIMER_INDEX_START)
      {
        DoIP_ResetTcpCon(Idx - DOIP_ALIVE_CHECK_TIMER_INDEX_START);
      }
      /* Vehicle announcement timeout handling */
      else
      {
        const DoIP_TimeoutIdxType VehicleAnnTimerIdx = Idx - DOIP_ANNOUNCEMENT_TIMER_INDEX_START;

        if(DOIP_VEHICLE_ANN_REPETITION !=
           DoIP_VehicleAnnSendCounter[VehicleAnnTimerIdx])
        {
          /* Return value ignored. No error handling done since vehicle
           * announcement can be configured to be resent multiple times. */
          (void) DoIP_Send_VID_Resp_Ann(DOIP_PBCFG_UDPANNCON(VehicleAnnTimerIdx).SoAdTxPduId);

          /* Prepare for next announcement. */
          DoIP_VehicleAnnSendCounter[VehicleAnnTimerIdx]++;
          DoIP_StartTimer(Idx, DOIP_VEHICLE_ANN_TIMEOUT);
          ResetTimer = FALSE;
        }
      }

      /* Disable timeout if Vehicle announcement didn't activate a new one. */
      if(ResetTimer == TRUE)
      {
        DoIP_TimeoutArr[Idx] = DOIP_INVALID_TIMEOUT_VALUE;
      }

    }
    else if(TimeoutValue != DOIP_INVALID_TIMEOUT_VALUE)
    {
      /* Calculate next timeout. */
      Diff = (uint32) (TimeoutValue - DoIP_GeneralTimer);
      if(Diff < DiffMin)
      {
        DiffMin = Diff;
        DoIP_NextTimeout = TimeoutValue;
      }
    }
    else
    {
      /* do_nothing, timer disabled. */
    }
  }
}

STATIC FUNC(void, DOIP_CODE) DoIP_ProcessAuthConfPendingRoutingActivations(void)
{
  DoIP_TcpConIdxType TcpConIdx = DoIP_PendingAuthConfTcpConIdx;
  uint8 RespCode;
  DoIP_TcpConTxStateType TxState;

  DBG_DOIP_PROCESSAUTHCONFPENDINGROUTINGACTIVATIONS_ENTRY();

  DoIP_PendingAuthConfTcpConIdx = DOIP_INVALID_TCP_CON_IDX;

  /* Search for Tcp connections that have pending channels. */
  for(; TcpConIdx < DOIP_INVALID_TCP_CON_IDX; TcpConIdx++)
  {
    CONSTP2VAR(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr = &DoIP_TcpCon[TcpConIdx];
    boolean TcpConPending = FALSE;

    if(TcpConPtr->NextPendingRouteIdx != DOIP_INVALID_ROUTE_IDX)
    {
      /* Pending auth/conf channel exist for this Tcp connection. */
      P2CONST(DoIP_RouteType, AUTOMATIC, DOIP_APPL_CONST) RoutePtr =
          &DOIP_PBCFG_ROUTE(TcpConPtr->NextPendingRouteIdx);
      P2VAR(DoIP_ChannelType, AUTOMATIC, DOIP_APPL_DATA) ChannelPtr =
          &DoIP_Channel[RoutePtr->DoIPPduRPduId];
      const DoIP_RoutingStateType PrevRoutingActState = ChannelPtr->RoutingActivationState;

      RespCode = DoIP_VerifyAuthenticationConfirmation(TcpConIdx, ChannelPtr, &TxState);

      if(PrevRoutingActState != ChannelPtr->RoutingActivationState)
      {
        /* Propagate the change to all channels pending on the same routing activation object. */
        uint16 RouteIdx_Begin = TcpConPtr->NextPendingRouteIdx + 1U;
        const uint16 RouteIdx_End = TcpConPtr->TesterPtr->RouteIdx +
                                    TcpConPtr->TesterPtr->NumberOfRoutesPerSource;
        uint16 RouteIdx;

        for(RouteIdx = RouteIdx_Begin; RouteIdx < RouteIdx_End; RouteIdx++)
        {
          P2CONST(DoIP_RouteType, AUTOMATIC, DOIP_APPL_CONST) Route2Ptr =
              &DOIP_PBCFG_ROUTE(RouteIdx);
          CONSTP2VAR(DoIP_ChannelType, AUTOMATIC, DOIP_APPL_DATA) Channel2Ptr =
              &DoIP_Channel[Route2Ptr->DoIPPduRPduId];

          if(Route2Ptr->RoutingActivationIdx == ChannelPtr->RoutingActivationIdx)
          {
            if(Channel2Ptr->RoutingActivationIdx == ChannelPtr->RoutingActivationIdx)
            {
              /* Propagate result if the channel is pending on the same routing activation. */
              TS_MemCpy(
                         Channel2Ptr->TxOemSpecific,
                         ChannelPtr->TxOemSpecific,
                         DOIP_ROU_ACT_OEM_SPECIFIC_LEN
                       );
              Channel2Ptr->RoutingActivationState = ChannelPtr->RoutingActivationState;
            }
          }
          else
          {
            /* Processed all relevant elements in DoIP_Route[]-stop search for this Tcp connection.*/
            break;
          }
        }

        if(RespCode == DOIP_ROU_ACT_RESP_SUCCESS)
        {
          if(TcpConPtr->MultipleAuthConfPending == TRUE)
          {
            /* More then 1 routing activation was pending on this Tcp connection ->
             * find the one to be processed in the next MainFunction cycle.*/
            DoIP_RoutingActivationIdxType FirstPendingRoutingActivationIdx =
                DOIP_INVALID_ROUTING_ACTIVATION_IDX;

            TcpConPtr->NextPendingRouteIdx = DOIP_INVALID_ROUTE_IDX;
            TcpConPtr->MultipleAuthConfPending = FALSE;

            /* Search all routes of this Tester (Tcp connection). */
            RouteIdx_Begin = TcpConPtr->TesterPtr->RouteIdx;
            for(RouteIdx = RouteIdx_Begin; RouteIdx < RouteIdx_End; RouteIdx++)
            {
              RoutePtr = &DOIP_PBCFG_ROUTE(RouteIdx);
              ChannelPtr = &DoIP_Channel[RoutePtr->DoIPPduRPduId];

              if((ChannelPtr->RoutingActivationIdx == RoutePtr->RoutingActivationIdx) &&
                 (DOIP_IS_CHANNEL_PENDING(ChannelPtr) == TRUE))
              {
                TcpConPending = TRUE;
                if(TcpConPtr->NextPendingRouteIdx == DOIP_INVALID_ROUTE_IDX)
                {
                  TcpConPtr->NextPendingRouteIdx = RouteIdx;
                  FirstPendingRoutingActivationIdx = RoutePtr->RoutingActivationIdx;
                }
                else
                {
                  if(FirstPendingRoutingActivationIdx != RoutePtr->RoutingActivationIdx)
                  {
                    /* Found multiple pending routing activations - stop the search. */
                    TcpConPtr->MultipleAuthConfPending = TRUE;
                    break;
                  }
                }
              }
            }
          }
          else
          {
            TcpConPtr->NextPendingRouteIdx = DOIP_INVALID_ROUTE_IDX;
          }
        }
        else
        {
          /* Channel(s) still pending on the same routing activation - try again in the next
           * MainFunction cycle. */
          TcpConPending = TRUE;
        }

        /* Successful confirmation is not reported after PENDING_CONFIRMATION. */
        if((RespCode != DOIP_DO_NOT_SEND_RESP) &&
           ((PrevRoutingActState != DOIP_ROUTING_PENDING_CONFIRMATION) ||
            (RespCode != DOIP_ROU_ACT_RESP_SUCCESS)))
        {
          /* Trigger sending Routing activation response message. */
          DoIP_SendTcpNonDiagMsg(TcpConIdx, TxState, RespCode);
        }
/* !LINKSTO DoIP.dsn.Func0129.RoutingActivationCallback_Call, 1 */
#if (DOIP_ROUTING_ACTIVATION_CALLBACK_NUM > 0U)
        if(RespCode == DOIP_ROU_ACT_RESP_SUCCESS)
        {
          CONSTP2CONST(uint8, AUTOMATIC, DOIP_APPL_DATA) OemSpecificPtr =
              (ChannelPtr->OemSpecificValid == TRUE) ? ChannelPtr->RxOemSpecific : NULL_PTR;
          CONSTP2CONST(DoIP_RoutingActType, AUTOMATIC, DOIP_APPL_CONST) RoutingActPtr =
              &DOIP_PBCFG_ROUTINGACT(ChannelPtr->RoutingActivationIdx);
          DoIP_CallRoutingActivationCallbacks(
                                               TcpConPtr,
                                               RoutingActPtr->ActivationNumber,
                                               OemSpecificPtr
                                             );
        }
#endif /* DOIP_ROUTING_ACTIVATION_CALLBACK_NUM > 0U */
      }
      else
      {
        /* Routing activation state has not changed -> proceed again with the next Tcp conn.*/
        TcpConPending = TRUE;
      }

    }
    /* else - no channel is pending on this Tcp connection -> proceed with the next Tcp conn.*/

    if(TcpConPending == TRUE)
    {
      DoIP_PendingAuthConfTcpConIdx = DOIP_MIN(DoIP_PendingAuthConfTcpConIdx, TcpConIdx);
    }
  }

  DBG_DOIP_PROCESSAUTHCONFPENDINGROUTINGACTIVATIONS_EXIT();
}

STATIC FUNC(void, DOIP_CODE) DoIP_ProcessAliveCheck(void)
{
  DoIP_TcpConIdxType Idx;

  DBG_DOIP_PROCESSALIVECHECK_ENTRY();

  if(DoIP_StartAllConnAliveCheck == TRUE)
  {
    /* Initiate all connection Alive check - send Alive check request on all active connections. */
    for(Idx = 0U; Idx < (DOIP_MAX_PARALLEL_TESTER + 1U); Idx++)
    {
      (void) DoIP_SendAliveCheckRequest(Idx);
    }
    DoIP_StartAllConnAliveCheck = FALSE;
  }

  if((DoIP_AliveCheckTimerCount == 0U) &&
     (DoIP_NoFreeTcpConIdx != DOIP_INVALID_TCP_CON_IDX))
  {
    /* No more active Alive check connections, send NACK to the connection that started
     * all connection Alive check. */
    DoIP_SendTcpNonDiagMsg(
                            DoIP_NoFreeTcpConIdx,
                            DOIP_TCPCON_TX_ROUTE_ACT,
                            DOIP_ROU_ACT_RESP_NO_FREE_TCP
                          );
    DoIP_NoFreeTcpConIdx = DOIP_INVALID_TCP_CON_IDX;
  }

  DBG_DOIP_PROCESSALIVECHECK_EXIT();
}


STATIC FUNC(void, DOIP_CODE) DoIP_ContinueRoutingActivation
(
  DoIP_TcpConIdxType TcpConIdx
)
{
  DBG_DOIP_CONTINUEROUTINGACTIVATION_ENTRY(TcpConIdx);

  DoIP_TcpCon[TcpConIdx].ConnectionState = DOIP_TCP_CONSTATE_SA_REGISTERED_PENDING;

  DoIP_RoutingActivation(
                          TcpConIdx,
                          DoIP_GetBufferDataPtr(TcpConIdx),
                          DoIP_GetBufferDataSize(TcpConIdx)
                        );

  /* Clear connection buffer. */
  DoIP_ClearBuffer(TcpConIdx);

  DBG_DOIP_CONTINUEROUTINGACTIVATION_EXIT(TcpConIdx);
}

STATIC FUNC(void, DOIP_CODE) DoIP_HandleAliveCheckConReset
(
  DoIP_TcpConIdxType TcpConIdx
)
{
  DoIP_TcpConIdxType Idx;

  DBG_DOIP_HANDLEALIVECHECKCONRESET_ENTRY(TcpConIdx);

  /* After the reset one free Tcp connection is available. If connection waiting for it exist ->
   * proceed with its Routing activation. */
  if(DoIP_NoFreeTcpConIdx != DOIP_INVALID_TCP_CON_IDX)
  {
    DoIP_ContinueRoutingActivation(DoIP_NoFreeTcpConIdx);

    DoIP_NoFreeTcpConIdx = DOIP_INVALID_TCP_CON_IDX;
  }

  /* Check if connection exist that is waiting on this source address. */
  if((DoIP_IsTimerActive(DOIP_ALIVE_CHECK_TIMER_INDEX(TcpConIdx)) == TRUE) &&
     (DoIP_NumActiveSingleAliveCheck > 0))
  {
    boolean Found = FALSE;
    for(Idx = 0U; (Idx < (DOIP_MAX_PARALLEL_TESTER + 1U)) && (Found == FALSE); Idx++)
    {
      if(TcpConIdx == Idx)
      {
        continue;
      }

      if((DoIP_TcpCon[Idx].RxState == DOIP_TCPCON_RX_SINGLE_ALIVE_CHECK_WAIT) &&
         (DoIP_TcpCon[Idx].Source == DoIP_TcpCon[TcpConIdx].Source))
      {
        DoIP_ContinueRoutingActivation(Idx);

        DoIP_NumActiveSingleAliveCheck--;

        Found = TRUE;
      }
    }
  }

  DBG_DOIP_HANDLEALIVECHECKCONRESET_EXIT(TcpConIdx);
}

/* !LINKSTO DoIP.dsn.Func0117.TxCustomMsg.EcuC_GetMetaDataPayloadType, 1 */
/* !LINKSTO DoIP.dsn.Func0117.TxCustomMsg.QueueTxRequest, 2 */
FUNC(Std_ReturnType, DOIP_CODE) DoIP_TpTransmit
(
  PduIdType DoIPPduRTxId,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_CONST) DoIPPduRTxInfoPtr
)
{
  Std_ReturnType Ret = E_NOT_OK;

  DBG_DOIP_TPTRANSMIT_ENTRY(DoIPPduRTxId, DoIPPduRTxInfoPtr);

#if(DOIP_DEV_ERROR_DETECT == STD_ON)
  if(DoIP_Initialized != TRUE)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_TPTRANSMIT, DOIP_E_UNINIT);
  }
  else if(DoIPPduRTxInfoPtr == NULL_PTR)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_TPTRANSMIT, DOIP_E_PARAM_POINTER);
  }
  else if(DoIP_RootPtr->PduRTxPduMax <= DoIPPduRTxId)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_TPTRANSMIT, DOIP_E_INVALID_PDU_SDU_ID);
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    PduIdType DoIPPduRPduId = DoIPPduRTxId;

#if (DOIP_CUSTOM_PAYLOAD_TYPE_ENABLED == STD_ON)
    Std_ReturnType Res = E_OK;
    if(DoIPPduRTxId >= DoIP_RootPtr->PduRTxPduStd)
    {
      PduIdType PbCustChIdx;
      uint16 PayloadType;
      PduInfoType PduInfo;

      /* Tx request for a custom channel. For standard channels it is possible to configure
         only RX side, for custom channels both TX and RX side need to be configured. */
      DoIPPduRPduId = DoIPPduRTxId + DoIP_RootPtr->StdChannelMax - DoIP_RootPtr->PduRTxPduStd;

      PbCustChIdx = DoIPPduRPduId - DoIP_RootPtr->StdChannelMax;

      Res = EcuC_GetMetaDataPayloadType(
                                         DOIP_PBCFG_CUSTOMCHANNEL(PbCustChIdx).ECUC_TxPduId,
                                         &PduInfo,
                                         &PayloadType
                                       );
      if(Res == E_OK)
      {
        DoIP_Channel[DoIPPduRPduId].PayloadType = PayloadType;
      }
    }

    if(Res == E_OK)
#endif /* (DOIP_CUSTOM_PAYLOAD_TYPE_ENABLED == STD_ON) */
    {
      SchM_Enter_DoIP_SCHM_DOIP_EXCLUSIVE_AREA_0();

      if(DoIP_Channel[DoIPPduRPduId].RoutingActivationState == DOIP_ROUTING_ACTIVE)
      {
        /* Check if Tx request is pending. */
        if(FALSE == DoIP_Channel[DoIPPduRPduId].PendingRequest)
        {
          /* Get corresponding TCP connection index. */
          const DoIP_TcpConIdxType TcpConIdx = DoIP_Channel[DoIPPduRPduId].TcpConIdx;
          CONSTP2VAR(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr = &DoIP_TcpCon[TcpConIdx];
          /* Get last DoIP_TpTransmit() request in queue. */
          const PduIdType LastDoIPPduRPduId = TcpConPtr->LastDoIPPduRPduId;

          TcpConPtr->LastDoIPPduRPduId = DoIPPduRPduId;

          if(LastDoIPPduRPduId == DOIP_INVALID_PDUID)
          {
            /* First Tx request for this Tcp connection -> this channel will go first. */
            TcpConPtr->NextDoIPPduRPduId = DoIPPduRPduId;
          }
          else
          {
            /* Check if last Tx request has already been processed. */
            if(DoIP_Channel[LastDoIPPduRPduId].PendingRequest)
            {
              /* Append this Tx request at the end of the queue (linked list). */
              DoIP_Channel[LastDoIPPduRPduId].NextDoIPPduRPduId = DoIPPduRPduId;
            }
            else
            {
              /* Queue is empty. This Tx request shall be processed next */
              TcpConPtr->NextDoIPPduRPduId = DoIPPduRPduId;
            }
          }

          DoIP_Channel[DoIPPduRPduId].TxDiagMsgLength = DoIPPduRTxInfoPtr->SduLength;
          DoIP_Channel[DoIPPduRPduId].PendingRequest = TRUE;
          Ret = E_OK;
        }
      }

      SchM_Exit_DoIP_SCHM_DOIP_EXCLUSIVE_AREA_0();
    }
  }

  DBG_DOIP_TPTRANSMIT_EXIT(Ret, DoIPPduRTxId, DoIPPduRTxInfoPtr);

  return Ret;
}

FUNC(Std_ReturnType, DOIP_CODE) DoIP_CancelTransmit
(
  PduIdType DoIPPduRTxId
)
{
  Std_ReturnType Ret = E_NOT_OK;

  DBG_DOIP_CANCELTRANSMIT_ENTRY(DoIPPduRTxId);

#if(DOIP_DEV_ERROR_DETECT == STD_ON)
  if(DoIP_Initialized != TRUE)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_CANCELTRANSMIT, DOIP_E_UNINIT);
  }
  else if(DoIP_RootPtr->PduRTxPduMax <= DoIPPduRTxId)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_CANCELTRANSMIT, DOIP_E_INVALID_PDU_SDU_ID);
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    TS_PARAM_UNUSED(DoIPPduRTxId);

    /* Implement functionality */
  }

  DBG_DOIP_CANCELTRANSMIT_EXIT(Ret, DoIPPduRTxId);

  return Ret;
}

FUNC(Std_ReturnType, DOIP_CODE) DoIP_CancelReceive
(
  PduIdType DoIPPduRRxId
)
{
  Std_ReturnType Ret = E_NOT_OK;

  DBG_DOIP_CANCELRECEIVE_ENTRY(DoIPPduRRxId);

#if(DOIP_DEV_ERROR_DETECT == STD_ON)
  if(DoIP_Initialized != TRUE)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_CANCELRECEIVE, DOIP_E_UNINIT);
  }
  else if(DoIP_RootPtr->PduRRxPduMax <= DoIPPduRRxId)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_CANCELRECEIVE, DOIP_E_INVALID_PDU_SDU_ID);
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    TS_PARAM_UNUSED(DoIPPduRRxId);

    /* Implement functionality */
  }

  DBG_DOIP_CANCELRECEIVE_EXIT(Ret, DoIPPduRRxId);

  return Ret;
}


FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_SoAdTpCopyTxData
(
  PduIdType TxPduId,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_CONST) PduInfoPtr,
  P2VAR(RetryInfoType, AUTOMATIC, DOIP_APPL_DATA) Retry,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_DATA) AvailableDataPtr
)
{
  BufReq_ReturnType BufReq_Ret = BUFREQ_E_NOT_OK;

  DBG_DOIP_SOADTPCOPYTXDATA_ENTRY(TxPduId, PduInfoPtr, Retry, AvailableDataPtr);

#if(DOIP_DEV_ERROR_DETECT == STD_ON)
  if(DoIP_Initialized != TRUE)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_SOADTPCOPYTXDATA, DOIP_E_UNINIT);
  }
  else if(PduInfoPtr == NULL_PTR)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_SOADTPCOPYTXDATA, DOIP_E_PARAM_POINTER);
  }
  else if(Retry != NULL_PTR) /* Note: Retry must always be NULL_PTR for DoIP. */
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_SOADTPCOPYTXDATA, DOIP_E_INVALID_PARAMETER);
  }
  else if(AvailableDataPtr == NULL_PTR)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_SOADTPCOPYTXDATA, DOIP_E_PARAM_POINTER);
  }
  else if(DoIP_RootPtr->TcpConMax <= TxPduId)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_SOADTPCOPYTXDATA, DOIP_E_INVALID_PDU_SDU_ID);
  }
  else
#else /* DOIP_DEV_ERROR_DETECT == STD_ON */
  TS_PARAM_UNUSED(Retry);
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    const PduIdType DoIPSoAdPduId = TxPduId;
    const DoIP_TcpConIdxType TcpConIdx = DoIP_FindTcpCon(DoIPSoAdPduId);
    CONSTP2VAR(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr = &DoIP_TcpCon[TcpConIdx];

    if(TcpConIdx != DOIP_INVALID_TCP_CON_IDX)
    {
      switch(TcpConPtr->TxState)
      {
        case DOIP_TCPCON_TX_ROUTE_ACT: /* Also the TcpCon[] deny shall be handled here. */
        case DOIP_TCPCON_TX_ROUTE_ACT_OEM:
        {
          const PduLengthType MsgLength = (PduLengthType) DOIP_GEN_HEADER_SIZE +
              ((TcpConPtr->TxState == DOIP_TCPCON_TX_ROUTE_ACT) ?
               DOIP_PL_LENGTH_ROUTE_ACT_RESP : DOIP_PL_LENGTH_ROUTE_ACT_RESP_OEM);

          BufReq_Ret = DoIP_FillMsg(
                                     TcpConIdx,
                                     MsgLength,
                                     PduInfoPtr,
                                     AvailableDataPtr,
                                     DoIP_FillRoutingActResponse,
                                     NULL_PTR
                                   );
          break;
        }
        case DOIP_TCPCON_TX_DIAG_MSG_START:
        {
          BufReq_Ret = DoIP_FillMsg(
                                     TcpConIdx,
                                     DOIP_GEN_HEADER_SIZE + DOIP_DIAG_HEADERSIZE,
                                     PduInfoPtr,
                                     AvailableDataPtr,
                                     DoIP_FillDiagMsgHdr,
                                     DoIP_FillDiagMsgPld
                                   );

          break;
        }
        case DOIP_TCPCON_TX_DIAG_MSG:
        {
          BufReq_Ret = DoIP_FillDiagMsgPld(TcpConIdx, PduInfoPtr, AvailableDataPtr);
          break;
        }
#if (DOIP_CUSTOM_PAYLOAD_TYPE_ENABLED == STD_ON)
        case DOIP_TCPCON_TX_CUSTOM_MSG_START:
        {
          BufReq_Ret = DoIP_FillMsg(
                                     TcpConIdx,
                                     DOIP_GEN_HEADER_SIZE,
                                     PduInfoPtr,
                                     AvailableDataPtr,
                                     DoIP_FillCustomMsgHdr,
                                     DoIP_FillCustomMsgPld
                                   );
          break;
        }
        case DOIP_TCPCON_TX_CUSTOM_MSG:
        {
          BufReq_Ret = DoIP_FillCustomMsgPld(TcpConIdx, PduInfoPtr, AvailableDataPtr);
          break;
        }
#endif /* DOIP_CUSTOM_PAYLOAD_TYPE_ENABLED == STD_ON */
        case DOIP_TCPCON_TX_DIAG_ACK_NACK_START:
        {
          BufReq_Ret = DoIP_FillMsg(
                                     TcpConIdx,
                                     DOIP_GEN_HEADER_SIZE + DOIP_PL_LENGTH_DIAG_ACK_NACK,
                                     PduInfoPtr,
                                     AvailableDataPtr,
                                     DoIP_FillDiagAckNackHdr,
                                     DoIP_FillDiagAckNackPld
                                   );
          break;
        }
        case DOIP_TCPCON_TX_DIAG_ACK_NACK:
        {
          BufReq_Ret = DoIP_FillDiagAckNackPld(TcpConIdx, PduInfoPtr, AvailableDataPtr);
          break;
        }
        case DOIP_TCPCON_TX_GENHEADER_NACK:
        {
          BufReq_Ret = DoIP_FillMsg(
                                     TcpConIdx,
                                     DOIP_GEN_HEADER_SIZE + DOIP_PL_LENGTH_GEN_HEADER_NACK,
                                     PduInfoPtr,
                                     AvailableDataPtr,
                                     DoIP_FillGenHeaderNack,
                                     NULL_PTR
                                   );
          break;
        }
        case DOIP_TCPCON_TX_ALIVE_CHECK_REQ:
        {
          BufReq_Ret = DoIP_FillMsg(
                                     TcpConIdx,
                                     DOIP_GEN_HEADER_SIZE + DOIP_PL_LENGTH_ALIVE_REQ,
                                     PduInfoPtr,
                                     AvailableDataPtr,
                                     DoIP_FillAliveCheckReq,
                                     NULL_PTR
                                   );
          break;
        }

        case DOIP_TCPCON_TX_IDLE: /* fall through */
        default:
        {
          break; /* No transmission expected. Return BUFREQ_E_NOT_OK. */
        }
      }
    }
  }

  DBG_DOIP_SOADTPCOPYTXDATA_EXIT(BufReq_Ret, TxPduId, PduInfoPtr, Retry, AvailableDataPtr);

  return BufReq_Ret;
}

STATIC FUNC(boolean, DOIP_CODE) DoIP_HandleTxDiagMsgConfirmation
(
  DoIP_TcpConIdxType TcpConIdx,
  NotifResultType Result
)
{
  boolean ContinueTransmission = FALSE;
  NotifResultType TxResult = Result;
  CONSTP2VAR(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr = &DoIP_TcpCon[TcpConIdx];

  /* During Tx state is in diagnostic message transmission
     TcpConPtr->NextDoIPPduRPduId is equal to TcpConPtr->DoIPPduRTxPduId. */
  const PduIdType DoIPPduRPduId = TcpConPtr->DoIPPduRTxPduId;
  const PduIdType DoIPSoAdPduId = TcpConPtr->DoIPSoAdPduId;
  CONSTP2VAR(DoIP_ChannelType, AUTOMATIC, DOIP_APPL_DATA) ChannelPtr = &DoIP_Channel[DoIPPduRPduId];

  DBG_DOIP_HANDLETXDIAGMSGCONFIRMATION_ENTRY(TcpConIdx, Result);

  if((ChannelPtr->TxDiagMsgLength > 0) && (Result == NTFRSLT_OK))
  {
    /* Since the complete DiagMsg received from the upper layer could not been transmitted
     * in one SoAd_TpTransmit() / DoIP_SoAdTpCopyTxData(1..*) / DoIP_SoAdTpTxConfirmation()
     * cycle, a new SoAd_TpTransmit() cycle will be started here. */
    PduInfoType PduInfo;
    Std_ReturnType Ret;
    const PduIdType SoAdTxPduId = DOIP_PBCFG_TCPCON(DoIPSoAdPduId).SoAdTxPduId;

    PduInfo.SduDataPtr = NULL_PTR;
    PduInfo.SduLength = ChannelPtr->TxDiagMsgLength;

    Ret = SoAd_TpTransmit(SoAdTxPduId, &PduInfo);

    if(E_OK == Ret)
    {
      /* Do not send confirmation yet -> transmit process continues. */
      ContinueTransmission = TRUE;

      /* Reset timeout */
      TS_AtomicAssign8(TcpConPtr->ResetGeneralInactivityTimer, TRUE);
    }
    else
    {
      TxResult = NTFRSLT_E_NOT_OK;
    }
  }

  if(ContinueTransmission == FALSE)
  {
    SchM_Enter_DoIP_SCHM_DOIP_EXCLUSIVE_AREA_0();

    if(DoIPPduRPduId != TcpConPtr->LastDoIPPduRPduId)
    {
      /* Linked list is not empty. Prepare for next queued Tx request. */
      TcpConPtr->NextDoIPPduRPduId = ChannelPtr->NextDoIPPduRPduId;
    }
    /* else: Nothing to do if linked list is empty. */

    TS_AtomicAssign8(ChannelPtr->PendingRequest, FALSE);

    SchM_Exit_DoIP_SCHM_DOIP_EXCLUSIVE_AREA_0();

    /* Inform upper layer */
    PduR_DoIPTpTxConfirmation(DOIP_PBCFG_PDURTXPDUID(DoIPPduRPduId), TxResult);
  }

  DBG_DOIP_HANDLETXDIAGMSGCONFIRMATION_EXIT(ContinueTransmission, TcpConIdx, Result);

  return ContinueTransmission;
}

/* !LINKSTO DoIP.dsn.Func0117.TxCustomMsg.SoAdTpTxConfirmation, 2 */
FUNC(void, DOIP_CODE) DoIP_SoAdTpTxConfirmation
(
  PduIdType TxPduId,
  NotifResultType Result
)
{
  boolean ResetConn = FALSE;

  DBG_DOIP_SOADTPTXCONFIRMATION_ENTRY(TxPduId, Result);

#if(DOIP_DEV_ERROR_DETECT == STD_ON)
  if(DoIP_Initialized != TRUE)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_SOADTPTXCONFIRMATION, DOIP_E_UNINIT);
  }
  else if(DoIP_RootPtr->TcpConMax <= TxPduId)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_SOADTPTXCONFIRMATION, DOIP_E_INVALID_PDU_SDU_ID);
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    const PduIdType DoIPSoAdPduId = TxPduId;
    const DoIP_TcpConIdxType TcpConIdx = DoIP_FindTcpCon(DoIPSoAdPduId);

    if(TcpConIdx != DOIP_INVALID_TCP_CON_IDX)
    {
      /* Continue transmission in case of long PDUs */
      boolean ContinueTransmission = FALSE;
      CONSTP2VAR(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr = &DoIP_TcpCon[TcpConIdx];

      switch(TcpConPtr->TxState)
      {
        case DOIP_TCPCON_TX_DIAG_MSG_START: /* fall through */
        case DOIP_TCPCON_TX_DIAG_MSG:
#if (DOIP_CUSTOM_PAYLOAD_TYPE_ENABLED == STD_ON)
        case DOIP_TCPCON_TX_CUSTOM_MSG_START:
        case DOIP_TCPCON_TX_CUSTOM_MSG:
#endif
        {
          ContinueTransmission = DoIP_HandleTxDiagMsgConfirmation(TcpConIdx, Result);
          break;
        }
        case DOIP_TCPCON_TX_DIAG_ACK_NACK_START:
        case DOIP_TCPCON_TX_DIAG_ACK_NACK:
        {
          if(TcpConPtr->TxRespCode == DOIP_DIAG_MSG_NACK_INV_SRC_ADDR)
          {
            ResetConn = TRUE;
          }
          else
          {
            /* Release Rx buffer(s) */
            DoIP_ReleaseBuffer(TcpConIdx);

            /* ready to receive next generic header */
            TS_AtomicAssign8(TcpConPtr->RxState, DOIP_TCPCON_RX_READY);
            TS_AtomicAssign32(TcpConPtr->RxBufferSize, DOIP_GEN_HEADER_SIZE);
          }
          break;
        }
        case DOIP_TCPCON_TX_GENHEADER_NACK:
        {
          if((TcpConPtr->TxRespCode == DOIP_NACK_CODE_INCORRECT_PATTERN) ||
             (TcpConPtr->TxRespCode == DOIP_NACK_CODE_INVALID_PL_LENGTH))
          {
            ResetConn = TRUE;
          }
          else
          {
            /* ready to receive next generic header */
            TS_AtomicAssign8(TcpConPtr->RxState, DOIP_TCPCON_RX_READY);
            TS_AtomicAssign32(TcpConPtr->RxBufferSize, DOIP_GEN_HEADER_SIZE);
          }
          break;
        }
        case DOIP_TCPCON_TX_ROUTE_ACT:
        case DOIP_TCPCON_TX_ROUTE_ACT_OEM:
        {
          if((TcpConPtr->TxRespCode == DOIP_ROU_ACT_RESP_SUCCESS) ||
             (TcpConPtr->TxRespCode == DOIP_ROU_ACT_RESP_NO_AUTHENTICATION) ||
             (TcpConPtr->TxRespCode == DOIP_ROU_ACT_RESP_NO_CONFIRMATION) ||
             (TcpConPtr->TxRespCode == DOIP_ROU_ACT_RESP_PENDING_CONFIRMATION))
          {
            /* ready to receive next generic header */
            TS_AtomicAssign8(TcpConPtr->RxState, DOIP_TCPCON_RX_READY);
            TS_AtomicAssign32(TcpConPtr->RxBufferSize, DOIP_GEN_HEADER_SIZE);
          }
          else
          {
            /* For all other routing activation response codes - reset connection. */
            ResetConn = TRUE;
          }
          break;
        }
        case DOIP_TCPCON_TX_ALIVE_CHECK_REQ:
        case DOIP_TCPCON_TX_IDLE:
        default:
        {
          /* nothing to do */
          break;
        }
      }

      if(ContinueTransmission == TRUE)
      {
        /* Keep TxState for further transmission processing .*/
      }
      else if(ResetConn == TRUE)
      {
        DoIP_ResetTcpCon(TcpConIdx);
      }
      else
      {
        /* Process postponed Tx state */
        TS_AtomicAssign8(TcpConPtr->TxState, TcpConPtr->RequestedTxState);
        TS_AtomicAssign8(TcpConPtr->RequestedTxState, DOIP_TCPCON_TX_IDLE);

        /* Reset connection counter used for segmented transmission of headers and non-diagnostic
         * message payloads. */
        TcpConPtr->TxMsgPos = 0U;

        switch(TcpConPtr->TxState)
        {
          case DOIP_TCPCON_TX_DIAG_ACK_NACK_START:
          case DOIP_TCPCON_TX_GENHEADER_NACK:
          case DOIP_TCPCON_TX_ROUTE_ACT:
          case DOIP_TCPCON_TX_ROUTE_ACT_OEM:
          {
            PduInfoType SoAdPduInfo;
            const PduIdType SoAdTxPduId = DOIP_PBCFG_TCPCON(DoIPSoAdPduId).SoAdTxPduId;
            Std_ReturnType Ret;

            /* Since Tx state is not IDLE no further protection needed */

            SoAdPduInfo.SduLength = DoIP_GetTxSduLength(TcpConIdx, TcpConPtr->TxState);
            SoAdPduInfo.SduDataPtr = NULL_PTR;

            TcpConPtr->DoIPPduRTxPduId = TcpConPtr->RequestedTxState_DoIPPduRTxPduId;

            Ret = SoAd_TpTransmit(SoAdTxPduId, &SoAdPduInfo);

            if(E_OK != Ret)
            {
              /* Release default Rx buffer */
              DoIP_ReleaseBuffer(TcpConIdx);
              TS_AtomicAssign32(TcpConPtr->RxBufferSize, DOIP_GEN_HEADER_SIZE);
              TS_AtomicAssign8(TcpConPtr->RxState, DOIP_TCPCON_RX_READY);
              TS_AtomicAssign8(TcpConPtr->TxState, DOIP_TCPCON_TX_IDLE);
            }
            break;
          }
          default:
          {
            TS_AtomicAssign8(TcpConPtr->TxState, DOIP_TCPCON_TX_IDLE);
            break;
          }
        }
      }
    }
    else
    {
      SoAd_SoConIdType SoConId = SOAD_INVALID_SOCON_ID;

      /* Reset TCP connection. */
      /* Ignore return value: No error handling defined. */
      (void) SoAd_GetSoConId(DOIP_PBCFG_TCPCON(DoIPSoAdPduId).SoAdTxPduId, &SoConId);
      (void) SoAd_CloseSoCon(SoConId, TRUE);
      (void) SoAd_OpenSoCon(SoConId);
    }
  }

  DBG_DOIP_SOADTPTXCONFIRMATION_EXIT(TxPduId, Result);
}

FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_SoAdTpCopyRxData
(
  PduIdType RxPduId,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_DATA) BufferSizePtr
)
{
  BufReq_ReturnType BufReq_Ret = BUFREQ_E_NOT_OK;

  DBG_DOIP_SOADTPCOPYRXDATA_ENTRY(RxPduId, PduInfoPtr, BufferSizePtr);

#if(DOIP_DEV_ERROR_DETECT == STD_ON)
  if(DoIP_Initialized != TRUE)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_SOADTPCOPYRXDATA, DOIP_E_UNINIT);
  }
  else if(PduInfoPtr == NULL_PTR)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_SOADTPCOPYRXDATA, DOIP_E_PARAM_POINTER);
  }
  else if((PduInfoPtr->SduLength != 0U) && (PduInfoPtr->SduDataPtr == NULL_PTR))
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_SOADTPCOPYRXDATA, DOIP_E_PARAM_POINTER);
  }
  else if(BufferSizePtr == NULL_PTR)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_SOADTPCOPYRXDATA, DOIP_E_PARAM_POINTER);
  }
  else if(DoIP_RootPtr->TcpConMax <= RxPduId)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_SOADTPCOPYRXDATA, DOIP_E_INVALID_PDU_SDU_ID);
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    const PduIdType DoIPSoAdPduId = RxPduId;

    /* Check if the socket connection is already registered. */
    DoIP_TcpConIdxType TcpConIdx = DoIP_FindTcpCon(DoIPSoAdPduId);
    CONSTP2VAR(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr = &DoIP_TcpCon[TcpConIdx];

    if(TcpConIdx != DOIP_INVALID_TCP_CON_IDX)
    {
      switch(TcpConPtr->RxState)
      {
        case DOIP_TCPCON_RX_READY:
        {
          BufReq_Ret = DoIP_HandleRxStartReady(TcpConIdx, PduInfoPtr);
          break;
        }
        case DOIP_TCPCON_RX_DISCARD_MSG:
        {
          /* Read and discard message payload to free SoAd buffers. */
          BufReq_Ret = DoIP_DiscardMsg(TcpConIdx, PduInfoPtr);
          break;
        }
        case DOIP_TCPCON_RX_ROUTE_ACT:
        {
          BufReq_Ret = DoIP_HandleRoutingActivation(TcpConIdx, PduInfoPtr);
          break;
        }
        case DOIP_TCPCON_RX_DIAG_HEADER:
        {
          BufReq_Ret = DoIP_HandleDiagHeader(TcpConIdx, PduInfoPtr);
          break;
        }
        case DOIP_TCPCON_RX_DIAG_MSG:
        {
          BufReq_Ret = DoIP_CopyDiagMsg(TcpConIdx, PduInfoPtr);
          break;
        }
#if (DOIP_CUSTOM_PAYLOAD_TYPE_ENABLED == STD_ON)
        case DOIP_TCPCON_RX_CUSTOM_MSG_PAYLOAD:
        {
          BufReq_Ret = DoIP_CopyCustomMsgPayload(TcpConIdx, PduInfoPtr);
          break;
        }
#endif /* DOIP_CUSTOM_PAYLOAD_TYPE_ENABLED == STD_ON */
        case DOIP_TCPCON_RX_DISCARD_DIAG_MSG:
        {
          /* Read and discard diagnostic message payload to free SoAd buffers. */
          BufReq_Ret = DoIP_DiscardDiagMsg(TcpConIdx, PduInfoPtr);
          break;
        }
        case DOIP_TCPCON_RX_ALIVE_CHECK_RESP:
        {
          BufReq_Ret = DoIP_HandleAliveCheckResp(TcpConIdx, PduInfoPtr);
          break;
        }
        case DOIP_TCPCON_RX_ALL_CON_ALIVE_CHECK_WAIT: /* Block until Alive check (all) finishes. */
        case DOIP_TCPCON_RX_SINGLE_ALIVE_CHECK_WAIT: /* Block until Alive check (single) finishes.*/
        case DOIP_TCPCON_RX_START: /* Block until connection startup finishes. */
        case DOIP_TCPCON_RX_ACK_CONF_WAIT: /* Block until acknowledge/response msg .is confirmed. */
        {
          BufReq_Ret = BUFREQ_E_BUSY;
          break;
        }
        /* CHECK: NOPARSE */
        case DOIP_TCPCON_RX_OFFLINE: /* fall through */
        default:
        {
          BufReq_Ret = BUFREQ_E_NOT_OK;
          break;
        }
        /* CHECK: PARSE */
      }

      /* Set the buffer for the next call (limited with PduLengthType). */
      *BufferSizePtr = (PduLengthType) DOIP_MIN(
                                                 TcpConPtr->RxBufferSize,
                                                 DOIP_PDULENGTHTYPE_MAX
                                               );
    }
    else
    {
      *BufferSizePtr = DOIP_GEN_HEADER_SIZE;
    }
  }

  DBG_DOIP_SOADTPCOPYRXDATA_EXIT(BufReq_Ret, RxPduId, PduInfoPtr, BufferSizePtr);

  return BufReq_Ret;
}

FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_SoAdTpStartOfReception
(
  PduIdType RxPduId,
  PduLengthType TpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_DATA) BufferSizePtr
)
{
  BufReq_ReturnType BufReq_Ret = BUFREQ_E_NOT_OK;

  DBG_DOIP_SOADTPSTARTOFRECEPTION_ENTRY(RxPduId, TpSduLength, BufferSizePtr);

#if(DOIP_DEV_ERROR_DETECT == STD_ON)
  if(DoIP_Initialized != TRUE)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_SOADTPSTARTOFRECEPTION, DOIP_E_UNINIT);
  }
  else if(TpSduLength != 0U) /* Note: TpSduLength must always be 0U for DoIP. */
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_SOADTPSTARTOFRECEPTION, DOIP_E_INVALID_PARAMETER);
  }
  else if(BufferSizePtr == NULL_PTR)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_SOADTPSTARTOFRECEPTION, DOIP_E_PARAM_POINTER);
  }
  else if(DoIP_RootPtr->TcpConMax <= RxPduId)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_SOADTPSTARTOFRECEPTION, DOIP_E_INVALID_PDU_SDU_ID);
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    if(DOIP_ACT_LINE_STATE_ACTIVE == DoIP_ActivationLineState)
    {
      DoIP_TcpConIdxType Idx;
      boolean Registered = FALSE;
      const PduIdType DoIPSoAdPduId = RxPduId;

      TS_PARAM_UNUSED(TpSduLength);

      SchM_Enter_DoIP_SCHM_DOIP_EXCLUSIVE_AREA_0();
      for(Idx = 0U; Idx < (DOIP_MAX_PARALLEL_TESTER + 1U); Idx++)
      {
        if(DoIP_TcpCon[Idx].RxState == DOIP_TCPCON_RX_OFFLINE)
        {
          DoIP_TcpCon[Idx].RxState = DOIP_TCPCON_RX_START;
          Registered = TRUE;
          break;
        }
      }
      SchM_Exit_DoIP_SCHM_DOIP_EXCLUSIVE_AREA_0();

      if(Registered)
      {
        DoIP_PrepareTcpCon(Idx, DoIPSoAdPduId);

        *BufferSizePtr = DOIP_GEN_HEADER_SIZE;
        BufReq_Ret = BUFREQ_OK;
      }
    }
  }

  DBG_DOIP_SOADTPSTARTOFRECEPTION_EXIT(BufReq_Ret, RxPduId, TpSduLength, BufferSizePtr);

  return BufReq_Ret;
}

FUNC(void, DOIP_CODE) DoIP_SoAdTpRxIndication
(
  PduIdType RxPduId,
  NotifResultType Result
)
{
  DBG_DOIP_SOADTPRXINDICATION_ENTRY(RxPduId, Result);

#if(DOIP_DEV_ERROR_DETECT == STD_ON)
  if(DoIP_Initialized != TRUE)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_SOADTPRXINDICATION, DOIP_E_UNINIT);
  }
  else if(DoIP_RootPtr->TcpConMax <= RxPduId)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_SOADTPRXINDICATION, DOIP_E_INVALID_PDU_SDU_ID);
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    const PduIdType DoIPSoAdPduId = RxPduId;
    const DoIP_TcpConIdxType TcpConIdx = DoIP_FindTcpCon(DoIPSoAdPduId);

    TS_PARAM_UNUSED(Result);

    if(TcpConIdx != DOIP_INVALID_TCP_CON_IDX)
    {
      DoIP_ResetTcpCon(TcpConIdx);
    }
    else
    {
      SoAd_SoConIdType SoConId = SOAD_INVALID_SOCON_ID;

      /* Reset TCP connection. */
      /* Ignore return value: No error handling defined. */
      (void) SoAd_GetSoConId(DOIP_PBCFG_TCPCON(DoIPSoAdPduId).SoAdTxPduId, &SoConId);
      (void) SoAd_CloseSoCon(SoConId, TRUE);
      (void) SoAd_OpenSoCon(SoConId);
    }
  }

  DBG_DOIP_SOADTPRXINDICATION_EXIT(RxPduId, Result);
}

FUNC(void, DOIP_CODE) DoIP_SoConModeChg
(
  SoAd_SoConIdType SoConId,
  SoAd_SoConModeType Mode
)
{
  DBG_DOIP_SOCONMODECHG_ENTRY(SoConId, Mode);

#if(DOIP_DEV_ERROR_DETECT == STD_ON)
  if(DoIP_Initialized != TRUE)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_SOCONMODECHG, DOIP_E_UNINIT);
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    DoIP_TcpConIdxType Idx;
    SoAd_SoConIdType SoConId_read;

    switch(Mode)
    {
      case SOAD_SOCON_ONLINE: /* Fall through */
      {
        for(Idx = 0U; Idx < DoIP_RootPtr->UdpAnnConMax; Idx++)
        {
          if(SoAd_GetSoConId(DOIP_PBCFG_UDPANNCON(Idx).SoAdTxPduId, &SoConId_read) == E_OK)
          {
            if(SoConId == SoConId_read)
            {
              /* Enable sending vehicle announcement messages for this connection. */
              DoIP_StartTimer(
                               DOIP_ANNOUNCEMENT_TIMER_INDEX(Idx),
                               DOIP_VEHICLE_ANN_RESP_MAX_INIT_TIMEOUT
                             );
            }
          }
        }
        break;
      }
      case SOAD_SOCON_RECONNECT: /* Fall through */
      case SOAD_SOCON_OFFLINE:
      {
        Std_ReturnType RetVal;
        boolean Found = FALSE;
        /* Reset registered Tcp connection that is no longer ONLINE. */
        for(Idx = 0U; (Idx < (DOIP_MAX_PARALLEL_TESTER + 1U)) && (Found == FALSE); Idx++)
        {
          if(DOIP_TCPCON_RX_OFFLINE != DoIP_TcpCon[Idx].RxState)
          {
            const PduIdType DoIPSoAdPduId = DoIP_TcpCon[Idx].DoIPSoAdPduId;
            RetVal = SoAd_GetSoConId(
                                      DOIP_PBCFG_TCPCON(DoIPSoAdPduId).SoAdTxPduId,
                                      &SoConId_read
                                    );
            if((RetVal == E_OK) && (SoConId == SoConId_read))
            {
              DoIP_ResetTcpCon(Idx);
              Found = TRUE;
            }
          }
        }
        break;
      }
      default:
      {
#if(DOIP_DEV_ERROR_DETECT == STD_ON)
        DOIP_DET_REPORT_ERROR(DOIP_SID_SOCONMODECHG, DOIP_E_INVALID_PARAMETER);
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
        break;
      }
    }

#if (DOIP_USER_TCP_SOCON_MODECHG_CALLBACK == STD_ON)
    /* !LINKSTO DoIP.dsn.Func0117.Call_User_TcpSoConModeChg, 1 */
    DoIP_User_TcpSoConModeChg_Fp(SoConId, Mode);
#endif /* DOIP_USER_TCP_SOCON_MODECHG_CALLBACK == STD_ON */

    TS_PARAM_UNUSED(SoConId);
  }

  DBG_DOIP_SOCONMODECHG_EXIT(SoConId, Mode);
}

FUNC(void, DOIP_CODE) DoIP_LocalIpAddrAssignmentChg
(
  SoAd_SoConIdType SoConId,
  TcpIp_IpAddrStateType State
)
{
  const uint8 DoIP_DhcpMinName[DOIP_DHCP_MIN_HOSTNAME_SIZE] = {'D','o','I','P','-'};

  DBG_DOIP_LOCALIPADDRASSIGNMENTCHG_ENTRY(SoConId, State);

#if(DOIP_DEV_ERROR_DETECT == STD_ON)
  if(DoIP_Initialized != TRUE)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_LOCALIPADDRASSIGNMENTCHG, DOIP_E_UNINIT);
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    switch(State)
    {
      case TCPIP_IPADDR_STATE_ASSIGNED:
      {
        if((DoIP_GetUpdateHostnameOption(SoConId) == TRUE) &&
           (DoIP_ActivationLineState != DOIP_ACT_LINE_STATE_INACTIVE))
        {
          Std_ReturnType Ret;
          uint8 DhcpHostNameLength = DOIP_HOSTNAME_SIZE_MAX;
          boolean WriteHostName;

          Ret = SoAd_ReadDhcpHostNameOption(SoConId, &DhcpHostNameLength, &DoIP_DhcpHostName[0U]);

          WriteHostName = TRUE;

          if(Ret == E_OK)
          {
            if(DOIP_DHCP_MIN_HOSTNAME_SIZE <= DhcpHostNameLength)
            {
              /* Check if the actual host name starts with 'DoIP-'. */
              Ret = TS_MemCmp(&DoIP_DhcpHostName[0U], &DoIP_DhcpMinName[0U], DOIP_DHCP_MIN_HOSTNAME_SIZE);
              if(E_OK == Ret)
              {
                WriteHostName = FALSE;
              }
            }
          }

          if(WriteHostName)
          {
            uint8 HostNameSize = DOIP_DHCP_MIN_HOSTNAME_SIZE + DOIP_DHCP_HOSTNAME_PREFIX_SIZE;
            /* Copy "DoIP-". */
            TS_MemCpy(&DoIP_DhcpHostName[0U], &DoIP_DhcpMinName[0U], DOIP_DHCP_MIN_HOSTNAME_SIZE);

            /* Copy host name prefix if set. */
            TS_MemCpy(
                       &DoIP_DhcpHostName[DOIP_DHCP_MIN_HOSTNAME_SIZE],
                       &DOIP_DHCP_HOSTNAME_PREFIX[0U],
                       DOIP_DHCP_HOSTNAME_PREFIX_SIZE
                     );
#if(DOIP_DHCP_VIN_USE_ENABLE == STD_ON)
            /* Fill VIN. */
            (void) DoIP_Fill_VIN(&DoIP_DhcpHostName[DOIP_DHCP_MIN_HOSTNAME_SIZE +
                                 DOIP_DHCP_HOSTNAME_PREFIX_SIZE]);
            HostNameSize += DOIP_VIN_SIZE;
#endif

            /* Ignore return value: No error handling defined. */
            (void) SoAd_WriteDhcpHostNameOption(
                                                 SoConId,
                                                 HostNameSize,
                                                 &DoIP_DhcpHostName[0U]
                                               );

          }
        }
        break;
      }
      case TCPIP_IPADDR_STATE_ONHOLD: /* Fall through */
      case TCPIP_IPADDR_STATE_UNASSIGNED:
      {
        break; /* Nothing to do. */
      }
      default:
      {
#if(DOIP_DEV_ERROR_DETECT == STD_ON)
        DOIP_DET_REPORT_ERROR(DOIP_SID_LOCALIPADDRASSIGNMENTCHG, DOIP_E_INVALID_PARAMETER);
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
        break;
      }
    }
  }

  DBG_DOIP_LOCALIPADDRASSIGNMENTCHG_EXIT(SoConId, State);
}

FUNC(void, DOIP_CODE) DoIP_ActivationLineSwitchActive(void)
{
  DBG_DOIP_ACTIVATIONLINESWITCHACTIVE_ENTRY();

#if(DOIP_DEV_ERROR_DETECT == STD_ON)
  if(DoIP_Initialized != TRUE)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_ACTIVATIONLINESWITCH, DOIP_E_UNINIT);
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    if(DOIP_ACT_LINE_STATE_INACTIVE == DoIP_ActivationLineState)
    {
      TS_AtomicAssign8(DoIP_ActivationLineState, DOIP_ACT_LINE_STATE_REQ_ACTIVE);

      /* Request Ip address assignment for all Udp and Tcp connections for which configuration
       * field RequestAddressAssignment is enabled. */
      DoIP_RequestReleaseIpAddrAllConn(DOIP_REQUEST_IP_ADDR_OP);

      /* Open all Udp and Tcp connections. */
      DoIP_OpenAllCon();

      TS_AtomicAssign8(DoIP_ActivationLineState, DOIP_ACT_LINE_STATE_ACTIVE);
    }
  }

  DBG_DOIP_ACTIVATIONLINESWITCHACTIVE_EXIT();
}

FUNC(void, DOIP_CODE) DoIP_ActivationLineSwitchInactive(void)
{
  DBG_DOIP_ACTIVATIONLINESWITCHINACTIVE_ENTRY();

#if(DOIP_DEV_ERROR_DETECT == STD_ON)
  if(DoIP_Initialized != TRUE)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_ACTIVATIONLINESWITCH, DOIP_E_UNINIT);
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    if(DOIP_ACT_LINE_STATE_ACTIVE == DoIP_ActivationLineState)
    {
      /* Close all Udp and Tcp connections. */
      DoIP_CloseAllCon();

      /* Release Ip address assignment for all Udp and Tcp connections for which configuration
       * field RequestAddressAssignment is enabled. */
      DoIP_RequestReleaseIpAddrAllConn(DOIP_RELEASE_IP_ADDR_OP);

      TS_AtomicAssign8(DoIP_ActivationLineState, DOIP_ACT_LINE_STATE_INACTIVE);
    }
  }

  DBG_DOIP_ACTIVATIONLINESWITCHINACTIVE_EXIT();
}

FUNC(void, DOIP_CODE) DoIP_SoAdIfTxConfirmation
(
  PduIdType TxPduId
)
{
  DBG_DOIP_SOADIFTXCONFIRMATION_ENTRY(TxPduId);

#if(DOIP_DEV_ERROR_DETECT == STD_ON)
  if(DoIP_Initialized != TRUE)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_SOADIFTXCONFIRMATION, DOIP_E_UNINIT);
  }
  else if(DoIP_RootPtr->UdpMaxIdx <= TxPduId)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_SOADIFTXCONFIRMATION, DOIP_E_INVALID_PDU_SDU_ID);
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    if(TxPduId < DoIP_RootPtr->UdpConMax)
    {
      /* Release remote address of UDP connection (not relevant for UDP announcement connections).*/
      DoIP_UdpReleaseRemoteAddr(TxPduId);

      /* UDP connection was successfully released. Stop deadline monitoring for this UDP connection. */
      DoIP_StopTimer(DOIP_UDP_RELEASE_TIMER_INDEX(TxPduId));
    }
  }

  DBG_DOIP_SOADIFTXCONFIRMATION_EXIT(TxPduId);
}

FUNC(void, DOIP_CODE) DoIP_SoAdIfRxIndication
(
  PduIdType RxPduId,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_DATA) PduInfoPtr
)
{
  DBG_DOIP_SOADIFRXINDICATION_ENTRY(RxPduId, PduInfoPtr);

#if(DOIP_DEV_ERROR_DETECT == STD_ON)
  if(DoIP_Initialized != TRUE)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_SOADIFRXINDICATION, DOIP_E_UNINIT);
  }
  else if(PduInfoPtr == NULL_PTR)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_SOADIFRXINDICATION, DOIP_E_PARAM_POINTER);
  }
  else if(DoIP_RootPtr->UdpConMax < RxPduId)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_SOADIFRXINDICATION, DOIP_E_INVALID_PDU_SDU_ID);
  }
  else if(PduInfoPtr->SduDataPtr == NULL_PTR)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_SOADIFRXINDICATION, DOIP_E_PARAM_POINTER);
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */
  {
    Std_ReturnType Ret_IfTransmit = E_NOT_OK;

    if(DOIP_ACT_LINE_STATE_ACTIVE == DoIP_ActivationLineState)
    {
      Std_ReturnType Ret;
      uint16 PayloadType;
      uint32 PayloadLength;
      uint8_least Idx;

      Ret = DoIP_Udp_Check_GenHeader
      (
        RxPduId, PduInfoPtr, &PayloadType, &PayloadLength, &Ret_IfTransmit
      );

      if(E_OK == Ret)
      {
#if(DOIP_USER_VID_REQ_RECEIVED_CALLBACK == STD_ON)
        switch(PayloadType)
        {
          case DOIP_PL_TYPE_VEHICLE_ID_REQ: /*fall through */
          case DOIP_PL_TYPE_VEHICLE_ID_EID_REQ: /*fall through */
          case DOIP_PL_TYPE_VEHICLE_ID_VIN_REQ:
          {
            /* !LINKSTO DoIP.dsn.Func0125.Call_User_VIDRequestReceived, 1 */
            DoIP_User_VIDRequestReceived_Fp(0U);
            break;
          }
          default:
          {
            break;
          }
        }
#endif /* DOIP_USER_VID_REQ_RECEIVED_CALLBACK == STD_ON */

        switch(PayloadType)
        {
          case DOIP_PL_TYPE_VEHICLE_ID_REQ:
          {
            Ret_IfTransmit = DoIP_Send_VID_Resp_Ann(DOIP_PBCFG_UDPCON(RxPduId).SoAdTxPduId);
            break;
          }
          case DOIP_PL_TYPE_VEHICLE_ID_EID_REQ:
          {
            Ret = E_OK;
            /* Check entity ID of incoming request */
            for(Idx = 0U; Idx < DOIP_EID_SIZE; Idx++)
            {
              if(DoIP_EID[Idx] != PduInfoPtr->SduDataPtr[DOIP_GEN_HEADER_B8_PAYLOAD + Idx])
              {
                Ret = E_NOT_OK;
                break;
              }
            }
            if(E_OK == Ret)
            {
              Ret_IfTransmit = DoIP_Send_VID_Resp_Ann(DOIP_PBCFG_UDPCON(RxPduId).SoAdTxPduId);
            }
            break;
          }
          case DOIP_PL_TYPE_VEHICLE_ID_VIN_REQ:
          {
            Ret = DoIP_Verify_VIN(&PduInfoPtr->SduDataPtr[DOIP_GEN_HEADER_B8_PAYLOAD]);
            if(Ret == E_OK)
            {
              Ret_IfTransmit = DoIP_Send_VID_Resp_Ann(DOIP_PBCFG_UDPCON(RxPduId).SoAdTxPduId);
            }
            break;
          }
          case DOIP_PL_TYPE_ENTITY_STATUS_REQ:
          {
            Ret_IfTransmit = DoIP_Send_Entity_Status_Resp(DOIP_PBCFG_UDPCON(RxPduId).SoAdTxPduId);
            break;
          }
#if(DOIP_POWER_MODE == STD_ON)
          case DOIP_PL_TYPE_POWERMODE_REQ:
          {
            Ret_IfTransmit = DoIP_Send_PowerMode_Resp(DOIP_PBCFG_UDPCON(RxPduId).SoAdTxPduId);
            break;
          }
#endif
          /* CHECK: NOPARSE */
          default:
          {
            DOIP_UNREACHABLE_CODE_ASSERT(DOIP_SID_SOADIFRXINDICATION);
            break;
          }
          /* CHECK: PARSE */
        }
      }
      /* else: Ignore frame. */
    }

    /* In case that transmission of response is not done/failed, release remote
       address. */
    if(Ret_IfTransmit != E_OK)
    {
      /* Release remote address of UDP connection. DoIPRxPduId and DoIPTxPduId have
         the same value (MCG restriction). */
      DoIP_UdpReleaseRemoteAddr(RxPduId);
    }
    else
    {
      DoIP_StartTimer(DOIP_UDP_RELEASE_TIMER_INDEX(RxPduId), DOIP_RELEASE_REMOTEADDR_TIMEOUT);
    }
  }

  DBG_DOIP_SOADIFRXINDICATION_EXIT(RxPduId, PduInfoPtr);

}

FUNC(void, DOIP_CODE) DoIP_PrepareTcpCon
(
  DoIP_TcpConIdxType TcpConIdx,
  PduIdType DoIPSoAdPduId
)
{
  CONSTP2VAR(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr = &DoIP_TcpCon[TcpConIdx];

  DBG_DOIP_PREPARETCPCON_ENTRY(TcpConIdx, DoIPSoAdPduId);

  TcpConPtr->ConnectionState = DOIP_TCP_CONSTATE_INIT;

  TcpConPtr->RequestedTxState = DOIP_TCPCON_TX_IDLE;

  TcpConPtr->DoIPSoAdPduId = DoIPSoAdPduId;
  TcpConPtr->ResetGeneralInactivityTimer = FALSE;
  TcpConPtr->TxMsgPos = 0U;

  /* Provide enough space for the routing activation frame. */
  TcpConPtr->RxBufferSize = DOIP_GEN_HEADER_SIZE;

  DoIP_StartTimer(DOIP_INACTIVITY_TIMER_INDEX(TcpConIdx), DOIP_TCP_INITIAL_INACTIVITY_TIMEOUT);

  TS_AtomicAssign8(TcpConPtr->TxState, DOIP_TCPCON_TX_IDLE);
  TS_AtomicAssign8(TcpConPtr->RxState, DOIP_TCPCON_RX_READY);

#if (DOIP_CUSTOM_PAYLOAD_TYPE_ENABLED == STD_ON)
  DoIP_PrepareTcpConCustom(TcpConIdx, DoIPSoAdPduId);
#endif

  DBG_DOIP_PREPARETCPCON_EXIT(TcpConIdx, DoIPSoAdPduId);
}

FUNC(Std_ReturnType, DOIP_CODE) DoIP_User_GetVinDummy
(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) DataPtr
)
{
  TS_PARAM_UNUSED(DataPtr);
  return E_NOT_OK;
}

FUNC(Std_ReturnType, DOIP_CODE) DoIP_RoutingActivationAuthDummy
(
  P2VAR(boolean, AUTOMATIC, DOIP_APPL_DATA) Authentified,
  P2CONST(uint8, AUTOMATIC, DOIP_APPL_DATA) AuthenticationReqData,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) AuthenticationResData
)
{
  TS_PARAM_UNUSED(AuthenticationReqData);
  TS_PARAM_UNUSED(AuthenticationResData);

  *Authentified = TRUE;

  return E_OK;
}

FUNC(Std_ReturnType, DOIP_CODE) DoIP_RoutingActivationConfDummy
(
  P2VAR(boolean, AUTOMATIC, DOIP_APPL_DATA) Confirmed,
  P2CONST(uint8, AUTOMATIC, DOIP_APPL_DATA) ConfirmationReqData,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) ConfirmationResData
)
{
  TS_PARAM_UNUSED(ConfirmationReqData);
  TS_PARAM_UNUSED(ConfirmationResData);

  *Confirmed = TRUE;

  return E_OK;
}

#if (DOIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)
FUNC(Std_ReturnType, DOIP_CODE) DoIP_GetAndResetMeasurementData
(
  DoIP_MeasurementIdxType MeasurementIdx,
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, DOIP_APPL_DATA) MeasurementDataPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_DOIP_GETANDRESETMEASUREMENTDATA_ENTRY(
                                              MeasurementIdx,
                                              MeasurementResetNeeded,
                                              MeasurementDataPtr
                                            );

#if(DOIP_DEV_ERROR_DETECT == STD_ON)
  if(DoIP_Initialized != TRUE)
  {
    DOIP_DET_REPORT_ERROR(DOIP_SID_GETANDRESETMEASUREMENTDATA, DOIP_E_UNINIT);
  }
  else
#endif /* DOIP_DEV_ERROR_DETECT */
  {
    SchM_Enter_DoIP_SCHM_DOIP_EXCLUSIVE_AREA_0();

    switch(MeasurementIdx)
    {
      case DOIP_MEAS_DROP_GENHDR:
      {
        if(MeasurementDataPtr != NULL_PTR)
        {
          *MeasurementDataPtr = DoIP_MeasurementDropData_GenHdr;
        }
        if(MeasurementResetNeeded)
        {
          DoIP_MeasurementDropData_GenHdr = 0U;
        }
        RetVal = E_OK;
        break;
      }
      case DOIP_MEAS_DROP_DIAGMSG:
      {
        if(MeasurementDataPtr != NULL_PTR)
        {
          *MeasurementDataPtr = DoIP_MeasurementDropData_DiagMsg;
        }
        if(MeasurementResetNeeded)
        {
          DoIP_MeasurementDropData_DiagMsg = 0U;
        }
        RetVal = E_OK;
        break;
      }
      case DOIP_MEAS_ALL:
      {
        if(MeasurementResetNeeded)
        {
          DoIP_MeasurementDropData_GenHdr = 0U;
          DoIP_MeasurementDropData_DiagMsg = 0U;
        }
        RetVal = E_OK;
        break;
      }
      default:
      {
        break;
      }
    }
    SchM_Exit_DoIP_SCHM_DOIP_EXCLUSIVE_AREA_0();
  }

  DBG_DOIP_GETANDRESETMEASUREMENTDATA_EXIT(
                                             RetVal,
                                             MeasurementIdx,
                                             MeasurementResetNeeded,
                                             MeasurementDataPtr
                                           );

  return RetVal;
}
#endif /* #if (DOIP_GETANDRESETMEASUREMENTDATA_API == STD_ON) */

/*==================[internal function definitions]=========================*/

STATIC FUNC(Std_ReturnType, DOIP_CODE) DoIP_Verify_VIN
(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) ReceivedVinPtr
)
{
  Std_ReturnType Ret;

#if (DOIP_USER_GET_VIN == STD_ON)
  uint8 UserVin[DOIP_VIN_SIZE];
#endif

  DBG_DOIP_VERIFY_VIN_ENTRY(ReceivedVinPtr);

#if (DOIP_USER_GET_VIN == STD_ON)
  Ret = DoIP_User_GetVin_Fp(&UserVin[0U]);
  if(Ret == E_OK)
  {
    Ret = TS_MemCmp(&ReceivedVinPtr[0U], &UserVin[0U], DOIP_VIN_SIZE);
  }
#else
  TS_PARAM_UNUSED(ReceivedVinPtr);

  /* User_GetVin() not provided -> return E_OK since comparison is not performed. */
  Ret = E_OK;
#endif /* DOIP_USER_GET_VIN == STD_ON */

  DBG_DOIP_VERIFY_VIN_EXIT(Ret, ReceivedVinPtr);

  return Ret;
}

STATIC FUNC(Std_ReturnType, DOIP_CODE) DoIP_Fill_VIN
(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) VinPtr
)
{
  Std_ReturnType Ret = E_NOT_OK;

  DBG_DOIP_FILL_VIN_ENTRY(VinPtr);

  Ret = DoIP_User_GetVin_Fp(&VinPtr[0U]);
  if(Ret == E_NOT_OK)
  {
    /* If the user callback function is not provided, or it failed to provide VIN ->
     * put VIN invalidity pattern instead. */
    TS_MemSet(&VinPtr[0U], DOIP_VIN_INVALIDITY_PATTERN, DOIP_VIN_SIZE);
  }

  DBG_DOIP_FILL_VIN_EXIT(Ret, VinPtr);

  return Ret;
}

STATIC FUNC(Std_ReturnType, DOIP_CODE) DoIP_Udp_Check_GenHeader
(
  PduIdType DoIP_SoAdRxPduId,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_DATA) PduInfoPtr,
  P2VAR(uint16, AUTOMATIC, DOIP_APPL_DATA) PayloadTypePtr,
  P2VAR(uint32, AUTOMATIC, DOIP_APPL_DATA) PayloadLengthPtr,
  P2VAR(Std_ReturnType, AUTOMATIC, DOIP_APPL_DATA) Ret_IfTransmitPtr
)
{
  Std_ReturnType Ret = E_NOT_OK; /* By default return not okay. */
  boolean SendNACK = FALSE;
  uint8 NACKCode = 0xFFU; /* Invalid value */

  DBG_DOIP_UDP_CHECK_GENHEADER_ENTRY(
                                      DoIP_SoAdRxPduId,
                                      PduInfoPtr,
                                      PayloadTypePtr,
                                      PayloadLengthPtr,
                                      Ret_IfTransmitPtr
                                    );

  if(PduInfoPtr->SduLength < DOIP_GEN_HEADER_SIZE)
  {
    Ret = E_NOT_OK; /* Ignore unknown frame. */
  }
  else
  {
    const uint16 PayloadType =
            DoIP_Make_uint16(
                              PduInfoPtr->SduDataPtr[DOIP_GEN_HEADER_B2_TYPE_MSB],
                              PduInfoPtr->SduDataPtr[DOIP_GEN_HEADER_B3_TYPE_LSB]
                            );

    const uint8 CalcProtocolVersion
      = PduInfoPtr->SduDataPtr[DOIP_GEN_HEADER_B1_PR_INV_VERSION] ^ 0xFFU;
    if((CalcProtocolVersion != PduInfoPtr->SduDataPtr[DOIP_GEN_HEADER_B0_PR_VERSION]) ||
        ((DOIP_PR_VERSION != PduInfoPtr->SduDataPtr[DOIP_GEN_HEADER_B0_PR_VERSION]) &&
         (DOIP_DEFAULT_PR_VERSION != PduInfoPtr->SduDataPtr[DOIP_GEN_HEADER_B0_PR_VERSION])) ||
        ((DOIP_PL_TYPE_VEHICLE_ID_REQ != PayloadType) &&
         (DOIP_PL_TYPE_VEHICLE_ID_EID_REQ != PayloadType) &&
         (DOIP_PL_TYPE_VEHICLE_ID_VIN_REQ !=PayloadType) &&
         (DOIP_DEFAULT_PR_VERSION == PduInfoPtr->SduDataPtr[DOIP_GEN_HEADER_B0_PR_VERSION])))
    {
      SendNACK = TRUE;
      NACKCode = DOIP_NACK_CODE_INCORRECT_PATTERN;
    }
    else
    {
      const uint32 PayloadLength =
        DoIP_Make_uint32(
                          PduInfoPtr->SduDataPtr[DOIP_GEN_HEADER_B4_LENGTH_3_MSB],
                          PduInfoPtr->SduDataPtr[DOIP_GEN_HEADER_B5_LENGTH_2],
                          PduInfoPtr->SduDataPtr[DOIP_GEN_HEADER_B6_LENGTH_1],
                          PduInfoPtr->SduDataPtr[DOIP_GEN_HEADER_B7_LENGTH_0_LSB]
                        );
      *PayloadTypePtr = PayloadType;
      *PayloadLengthPtr = PayloadLength;

      /* Check payload length against configuration parameter DoIPMaxRequestBytes. */
      if(PayloadLength > DOIP_MAX_PAYLOADSIZE)
      {
        NACKCode = DOIP_NACK_CODE_MSG_TO_LONG;
        SendNACK = TRUE;
      }
      else
      {
        /* Check payload length for a specific payload type */
        switch(PayloadType)
        {
          case DOIP_PL_TYPE_VEHICLE_ID_REQ:
          {
            if(DOIP_PL_LENGTH_VEHICLE_ID_REQ == PayloadLength)
            {
              Ret = E_OK;
            }
            else
            {
              NACKCode = DOIP_NACK_CODE_INVALID_PL_LENGTH;
              SendNACK = TRUE;
            }
            break;
          }
          case DOIP_PL_TYPE_VEHICLE_ID_EID_REQ:
          {
            if(DOIP_PL_LENGTH_VEHICLE_ID_EID_REQ == PayloadLength)
            {
              /* !LINKSTO DoIP.EB.SoAdIfRxIndication.IncompleteUdpFrame, 1 */
              if(PduInfoPtr->SduLength >= (PayloadLength + DOIP_GEN_HEADER_SIZE))
              {
                /* UDP frame is large enough. */
                Ret = E_OK;
              }
              /* else - incomplete UDP frame -> discard the message. */
            }
            else
            {
              NACKCode = DOIP_NACK_CODE_INVALID_PL_LENGTH;
              SendNACK = TRUE;
            }
            break;
          }
          case DOIP_PL_TYPE_VEHICLE_ID_VIN_REQ:
          {
            if(DOIP_PL_LENGTH_VEHICLE_ID_VIN_REQ == PayloadLength)
            {
              /* !LINKSTO DoIP.EB.SoAdIfRxIndication.IncompleteUdpFrame, 1 */
              if(PduInfoPtr->SduLength >= (PayloadLength + DOIP_GEN_HEADER_SIZE))
              {
                /* UDP frame is large enough. */
                Ret = E_OK;
              }
              /* else - incomplete UDP frame -> discard the message. */
            }
            else
            {
              NACKCode = DOIP_NACK_CODE_INVALID_PL_LENGTH;
              SendNACK = TRUE;
            }
            break;
          }
          case DOIP_PL_TYPE_ENTITY_STATUS_REQ:
          {
            if(DOIP_PL_LENGTH_ENTITY_STATUS_REQ == PayloadLength)
            {
              Ret = E_OK;
            }
            else
            {
              NACKCode = DOIP_NACK_CODE_INVALID_PL_LENGTH;
              SendNACK = TRUE;
            }
            break;
          }
          case DOIP_PL_TYPE_POWERMODE_REQ:
          {
#if(DOIP_POWER_MODE == STD_ON)
            if(DOIP_PL_LENGTH_POWERMODE_REQ == PayloadLength)
            {
              Ret = E_OK;
            }
            else
            {
              NACKCode = DOIP_NACK_CODE_INVALID_PL_LENGTH;
              SendNACK = TRUE;
            }
#else
            Ret = E_NOT_OK; /* Ignore if user/SVC interface power mode callback is not configured.*/
#endif
            break;
          }
          case DOIP_PL_TYPE_GEN_HEADER_NACK:
          {
            /* Ignore Generic NACK message for the case of two DoIP entities on the same network.*/
            break;
          }
          case DOIP_PL_TYPE_VEHICLE_ID_RESP_ANN:
          {
            /* Ignore Vehicle announcement message for the case of two DoIP entities
             * on the same network. */
            break;
          }
          default:
          {
            NACKCode = DOIP_NACK_CODE_UNKNOWN_PL_TYPE;
            SendNACK = TRUE;
            break;
          }
        }
      }
    }

    if(SendNACK)
    {
      PduInfoType TxPdu;
      const PduIdType SoAdTxPduId = DOIP_PBCFG_UDPCON(DoIP_SoAdRxPduId).SoAdTxPduId;
      uint8 PduData[DOIP_GEN_HEADER_SIZE + DOIP_PL_LENGTH_GEN_HEADER_NACK];
      TxPdu.SduLength = DOIP_GEN_HEADER_SIZE + DOIP_PL_LENGTH_GEN_HEADER_NACK;
      TxPdu.SduDataPtr = &PduData[0U];

      /* Fill generic header */
      DoIP_Fill_GenHeader(
                           TxPdu.SduDataPtr,
                           DOIP_PL_LENGTH_GEN_HEADER_NACK,
                           DOIP_PL_TYPE_GEN_HEADER_NACK
                         );

      /* Fill payload */
      TxPdu.SduDataPtr[DOIP_GEN_HEADER_B8_PAYLOAD] = NACKCode;

      *Ret_IfTransmitPtr = SoAd_IfTransmit(SoAdTxPduId, &TxPdu);

#if(DOIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)
      /* increase Generic header MeasurementData counter */
      DoIP_IncMeasurementDropDataCounter(DOIP_MEAS_DROP_GENHDR);
#endif /* DOIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */
    }
  }

  DBG_DOIP_UDP_CHECK_GENHEADER_EXIT(
                                     Ret,
                                     DoIP_SoAdRxPduId,
                                     PduInfoPtr,
                                     PayloadTypePtr,
                                     PayloadLengthPtr,
                                     Ret_IfTransmitPtr
                                   );

  return Ret;
}

STATIC FUNC(void, DOIP_CODE) DoIP_Tcp_Check_GenHeader
(
  DoIP_TcpConIdxType TcpConIdx,
  P2CONST(uint8, AUTOMATIC, DOIP_APPL_DATA) GenHdrPtr
)
{
  Std_ReturnType Result = E_NOT_OK;
  boolean SendNACK = FALSE;
  boolean ResetConn = FALSE;
  uint8 NACKCode = 0xFFU; /* Invalid value */
  uint8 CalcProtocolVersion;
  uint8 ReceivedProtocolVersion;
  uint16 PayloadType;
  uint32 PayloadLength;

  CONSTP2VAR(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr = &DoIP_TcpCon[TcpConIdx];

  DBG_DOIP_TCP_CHECK_GENHEADER_ENTRY(TcpConIdx, GenHdrPtr);

  CalcProtocolVersion = GenHdrPtr[DOIP_GEN_HEADER_B1_PR_INV_VERSION] ^ 0xFFU;
  ReceivedProtocolVersion = GenHdrPtr[DOIP_GEN_HEADER_B0_PR_VERSION];
  PayloadType = DoIP_Make_uint16(
                                  GenHdrPtr[DOIP_GEN_HEADER_B2_TYPE_MSB],
                                  GenHdrPtr[DOIP_GEN_HEADER_B3_TYPE_LSB]
                                );

  PayloadLength = DoIP_Make_uint32(
                                    GenHdrPtr[DOIP_GEN_HEADER_B4_LENGTH_3_MSB],
                                    GenHdrPtr[DOIP_GEN_HEADER_B5_LENGTH_2],
                                    GenHdrPtr[DOIP_GEN_HEADER_B6_LENGTH_1],
                                    GenHdrPtr[DOIP_GEN_HEADER_B7_LENGTH_0_LSB]
                                  );

  if((CalcProtocolVersion != ReceivedProtocolVersion) ||
      (DOIP_PR_VERSION != ReceivedProtocolVersion))
  {
    SendNACK = TRUE;
    ResetConn = TRUE;
    NACKCode = DOIP_NACK_CODE_INCORRECT_PATTERN;
  }
  else
  {
    /* Check payload length against configuration parameter DoIPMaxRequestBytes. */
    if(PayloadLength > DOIP_MAX_PAYLOADSIZE)
    {
      NACKCode = DOIP_NACK_CODE_MSG_TO_LONG;
      SendNACK = TRUE;
    }
    else
    {
      /* Check payload length for a specific payload type */
      switch(PayloadType)
      {
        case DOIP_PL_TYPE_ROUTE_ACT_REQ:
        {
          if(DOIP_PL_LENGTH_ROUTE_ACT_REQ == PayloadLength)
          {
            Result = E_OK;
          }
          else if(DOIP_PL_LENGTH_ROUTE_ACT_REQ_OEM == PayloadLength)
          {
            Result = E_OK;
          }
          else
          {
            NACKCode = DOIP_NACK_CODE_INVALID_PL_LENGTH;
            SendNACK = TRUE;
            ResetConn = TRUE;
          }
          break;
        }
        case DOIP_PL_TYPE_DIAG_MSG:
        {
          if(DOIP_PL_LENGTH_DIAG_MIN <= PayloadLength)
          {
            Result = E_OK;
          }
          else
          {
            NACKCode = DOIP_NACK_CODE_INVALID_PL_LENGTH;
            SendNACK = TRUE;
            ResetConn = TRUE;
          }
          break;
        }
        case DOIP_PL_TYPE_GEN_HEADER_NACK:
        {
          Result = E_OK;
          break;
        }
        case DOIP_PL_TYPE_ALIVE_RESP:
        {
          if(DOIP_PL_LENGTH_ALIVE_RESP == PayloadLength)
          {
            Result = E_OK;
          }
          else
          {
            NACKCode = DOIP_NACK_CODE_INVALID_PL_LENGTH;
            SendNACK = TRUE;
            ResetConn = TRUE;
          }
          break;
        }
        default:
        {
#if (DOIP_CUSTOM_PAYLOAD_TYPE_ENABLED == STD_ON)
          /* !LINKSTO DoIP.dsn.Func0117.RxCustomMsg.CustomPayloadType_OK, 2 */
          /* check range: 0xF000 - 0xFFFF Reserved range */
          if(PayloadType > 0xEFFF)
          {
            Result = E_OK;
            break;
          }
#endif /* DOIP_CUSTOM_PAYLOAD_TYPE_ENABLED == STD_ON */
          NACKCode = DOIP_NACK_CODE_UNKNOWN_PL_TYPE;
          SendNACK = TRUE;
          break;
        }
      }
    }
  }

  if(Result == E_OK)
  {
    switch(PayloadType)
    {
      case DOIP_PL_TYPE_ROUTE_ACT_REQ:
      {
        TS_AtomicAssign8(TcpConPtr->RxState, DOIP_TCPCON_RX_ROUTE_ACT);
        TcpConPtr->RxBufferSize = PayloadLength;
        break;
      }
      case DOIP_PL_TYPE_DIAG_MSG:
      {
        TS_AtomicAssign8(TcpConPtr->RxState, DOIP_TCPCON_RX_DIAG_HEADER);
        TcpConPtr->RxDiagMsgLength = PayloadLength - DOIP_DIAG_HEADERSIZE;
        TcpConPtr->RxBufferSize = DOIP_DIAG_HEADERSIZE;
        break;
      }
      case DOIP_PL_TYPE_GEN_HEADER_NACK:
      {
        /* Consume message payload to release SoAd buffers. */
        TcpConPtr->RxBufferSize = PayloadLength;
        TcpConPtr->TxRespCode = DOIP_DO_NOT_SEND_RESP;
        TS_AtomicAssign8(TcpConPtr->RxState, DOIP_TCPCON_RX_DISCARD_MSG);
        break;
      }
      case DOIP_PL_TYPE_ALIVE_RESP:
      {
        TcpConPtr->RxBufferSize = DOIP_PL_LENGTH_ALIVE_RESP;
        TS_AtomicAssign8(TcpConPtr->RxState, DOIP_TCPCON_RX_ALIVE_CHECK_RESP);
        break;
      }
      /* CHECK: NOPARSE */
      default:
      {
#if (DOIP_CUSTOM_PAYLOAD_TYPE_ENABLED == STD_ON)
        /* !LINKSTO DoIP.dsn.Func0117.RxCustomMsg.CustomPayloadType_OK, 2 */
        DoIP_HandleCustomMsg(TcpConIdx, PayloadType, PayloadLength);
        break;
#else
        DOIP_UNREACHABLE_CODE_ASSERT(DOIP_SID_SOADTPCOPYRXDATA);
        break;
#endif /* DOIP_CUSTOM_PAYLOAD_TYPE_ENABLED == STD_ON */

      }
      /* CHECK: PARSE */
    }
  }

  if(SendNACK)
  {
    if((ResetConn == FALSE) && (PayloadLength > 0))
    {
      /* This is a kind of generic header error that will not lead to connection reset, i.e.
       * traffic continues. Since message payload is still in SoAd buffer -> it needs to be
       * consumed in order to release buffers and synchronize with the next generic header. */
      TS_AtomicAssign8(TcpConPtr->RxState, DOIP_TCPCON_RX_DISCARD_MSG);

      /* NACK will be send when the complete message is delivered to DoIP - save response code */
      TS_AtomicAssign8(TcpConPtr->TxRespCode, NACKCode);

      /* The size of a message that has to be consumed. */
      TcpConPtr->RxBufferSize = PayloadLength;
    }
    else
    {
      TcpConPtr->RxBufferSize = DOIP_GEN_HEADER_SIZE;

      /* Send NACK immediately. */
      DoIP_SendTcpNonDiagMsg(TcpConIdx, DOIP_TCPCON_TX_GENHEADER_NACK, NACKCode);
    }

#if(DOIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)
    /* increase Generic header MeasurementData counter */
    DoIP_IncMeasurementDropDataCounter(DOIP_MEAS_DROP_GENHDR);
#endif /* DOIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */
  }

  DBG_DOIP_TCP_CHECK_GENHEADER_EXIT(TcpConIdx, GenHdrPtr);
}


STATIC FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_GetAvailPayloadLength
(
  DoIP_TcpConIdxType TcpConIdx,
  DoIP_FillDiagMsgPldFpType FillDiagMsgPldFp,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_DATA) AvailablePldDataPtr
)
{
  BufReq_ReturnType BufReq_Ret;

  DBG_DOIP_GETAVAILPAYLOADLENGTH_ENTRY(TcpConIdx, FillDiagMsgPldFp, AvailablePldDataPtr);

  if(FillDiagMsgPldFp != NULL_PTR)
  {

    PduInfoType PduInfo;

    /* Query for available data size. */
    PduInfo.SduDataPtr = NULL_PTR;
    PduInfo.SduLength = 0U;

    BufReq_Ret = FillDiagMsgPldFp(
                                   TcpConIdx,
                                   &PduInfo,
                                   AvailablePldDataPtr
                                 );
    if(BufReq_Ret != BUFREQ_OK)
    {
      *AvailablePldDataPtr = 0U;
    }
  }
  else
  {
    *AvailablePldDataPtr = 0U;
    BufReq_Ret = BUFREQ_OK;
  }

  DBG_DOIP_GETAVAILPAYLOADLENGTH_EXIT(BufReq_Ret, TcpConIdx, FillDiagMsgPldFp, AvailablePldDataPtr);

  return BufReq_Ret;
}

STATIC FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_FillMsg
(
  DoIP_TcpConIdxType TcpConIdx,
  PduLengthType HdrLength,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_DATA) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_DATA) AvailableDataPtr,
  DoIP_FillMsgHdrFpType FillMsgHdrFp,
  DoIP_FillDiagMsgPldFpType FillDiagMsgPldFp
)
{
  BufReq_ReturnType BufReq_Ret;
  CONSTP2VAR(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr = &DoIP_TcpCon[TcpConIdx];

  DBG_DOIP_FILLMSG_ENTRY(
                          TcpConIdx,
                          HdrLength,
                          PduInfoPtr,
                          AvailableDataPtr,
                          FillMsgHdrFp,
                          FillDiagMsgPldFp
                        );

  if(PduInfoPtr->SduLength == 0U)
  {
    PduLengthType AvailablePldData = 0U;
    uint32 Length;

    BufReq_Ret = DoIP_GetAvailPayloadLength(TcpConIdx, FillDiagMsgPldFp, &AvailablePldData);

    Length = (uint32) HdrLength - (uint32) TcpConPtr->TxMsgPos + AvailablePldData;

    *AvailableDataPtr = (PduLengthType) DOIP_MIN(Length, DOIP_PDULENGTHTYPE_MAX);
  }
  else if((TcpConPtr->TxMsgPos == 0U) && (PduInfoPtr->SduLength >= HdrLength))
  {
    /* This is the start of the message and there is enough space for headers at least. */
    FillMsgHdrFp(TcpConIdx, PduInfoPtr->SduDataPtr);

    if(FillDiagMsgPldFp != NULL_PTR)
    {
      /* Message with payload -> copy it up to available space, and find out the remaining size. */
      PduInfoType PduInfo;
      PduInfo.SduDataPtr = &PduInfoPtr->SduDataPtr[HdrLength];
      PduInfo.SduLength = PduInfoPtr->SduLength - HdrLength;

      BufReq_Ret = FillDiagMsgPldFp(
                                     TcpConIdx,
                                     &PduInfo,
                                     AvailableDataPtr
                                   );
    }
    else
    {
      /* The message without payload is complete. */
      *AvailableDataPtr = 0U;
      BufReq_Ret = BUFREQ_OK;
    }
  }
  else
  {
    /* Segmented message transmission -> generate the complete non-diag message (diag headers),
     * and copy only a part of it. */
    uint8 Msg[DOIP_MAX_TX_NON_DIAG_MSG_SIZE]; /* Temporary buffer for message generation */

    const PduLengthType CopySize = DOIP_MIN(
                                             HdrLength - TcpConPtr->TxMsgPos,
                                             PduInfoPtr->SduLength
                                           );

    FillMsgHdrFp(TcpConIdx, &Msg[0U]);     /* generate non-diag message (diag. msg. headers) */

    TS_MemCpy(PduInfoPtr->SduDataPtr, &Msg[TcpConPtr->TxMsgPos], CopySize);

    TcpConPtr->TxMsgPos += CopySize;

    if((TcpConPtr->TxMsgPos == HdrLength) && (FillDiagMsgPldFp != NULL_PTR))
    {
      /* The message has payload while headers are copied -> copy payload too. */
      PduInfoType PduInfo;
      PduInfo.SduDataPtr = &PduInfoPtr->SduDataPtr[CopySize];
      PduInfo.SduLength = PduInfoPtr->SduLength - CopySize;

      BufReq_Ret = FillDiagMsgPldFp(
                                     TcpConIdx,
                                     &PduInfo,
                                     AvailableDataPtr
                                   );
    }
    else
    {
      PduLengthType AvailablePldData;

      BufReq_Ret = DoIP_GetAvailPayloadLength(TcpConIdx, FillDiagMsgPldFp, &AvailablePldData);

      *AvailableDataPtr = HdrLength - TcpConPtr->TxMsgPos + AvailablePldData;
    }
  }

  DBG_DOIP_FILLMSG_EXIT(
                          BufReq_Ret,
                          TcpConIdx,
                          HdrLength,
                          PduInfoPtr,
                          AvailableDataPtr,
                          FillMsgHdrFp,
                          FillDiagMsgPldFp
                        );

  return BufReq_Ret;
}

FUNC(void, DOIP_CODE) DoIP_Fill_GenHeader
(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) PduPtr,
  uint32 PayloadLength,
  uint16 PayloadType
)
{
  DBG_DOIP_FILL_GENHEADER_ENTRY(PduPtr, PayloadLength, PayloadType);

  PduPtr[DOIP_GEN_HEADER_B0_PR_VERSION] = DOIP_PR_VERSION;
  PduPtr[DOIP_GEN_HEADER_B1_PR_INV_VERSION] = DOIP_PR_INV_VERSION;
  PduPtr[DOIP_GEN_HEADER_B2_TYPE_MSB] = (uint8) (PayloadType >> 8U);
  PduPtr[DOIP_GEN_HEADER_B3_TYPE_LSB] = (uint8) PayloadType;
  PduPtr[DOIP_GEN_HEADER_B4_LENGTH_3_MSB] = (uint8) (PayloadLength >> 24U);
  PduPtr[DOIP_GEN_HEADER_B5_LENGTH_2] = (uint8) (PayloadLength >> 16U);
  PduPtr[DOIP_GEN_HEADER_B6_LENGTH_1] = (uint8) (PayloadLength >> 8U);
  PduPtr[DOIP_GEN_HEADER_B7_LENGTH_0_LSB] = (uint8) PayloadLength;

  DBG_DOIP_FILL_GENHEADER_EXIT(PduPtr, PayloadLength, PayloadType);
}

STATIC FUNC(void, DOIP_CODE) DoIP_FillRoutingActResponse
(
  DoIP_TcpConIdxType TcpConIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) SduDataPtr
)
{
  CONSTP2VAR(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr = &DoIP_TcpCon[TcpConIdx];
  const uint32 PayloadLength = (TcpConPtr->TxState == DOIP_TCPCON_TX_ROUTE_ACT) ?
      DOIP_PL_LENGTH_ROUTE_ACT_RESP : DOIP_PL_LENGTH_ROUTE_ACT_RESP_OEM;

  uint8_least Idx;
  CONSTP2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) PayloadPtr = &SduDataPtr[DOIP_GEN_HEADER_B8_PAYLOAD];

  DBG_DOIP_FILLROUTINGACTRESPONSE_ENTRY(TcpConIdx, SduDataPtr)

  DoIP_Fill_GenHeader(
                       SduDataPtr,
                       PayloadLength,
                       DOIP_PL_TYPE_ROUTE_ACT_RESP
                     );

  PayloadPtr[DOIP_ROU_ACT_RESP_B0_TESTER_ADDR_MSB] = (uint8)(TcpConPtr->RoutActReqSA >> 8U);
  PayloadPtr[DOIP_ROU_ACT_RESP_B1_TESTER_ADDR_LSB] = (uint8)TcpConPtr->RoutActReqSA;
  PayloadPtr[DOIP_ROU_ACT_RESP_B2_ENTITY_ADDR_MSB] = DoIP_RootPtr->LogicalAddr[1U];
  PayloadPtr[DOIP_ROU_ACT_RESP_B3_ENTITY_ADDR_LSB] = DoIP_RootPtr->LogicalAddr[0U];

  TS_AtomicAssign8(PayloadPtr[DOIP_ROU_ACT_RESP_B4_ACT_CODE], TcpConPtr->TxRespCode);

  for(Idx = 0U; Idx < DOIP_ROU_ACT_ISO_RESERVED_LEN; Idx++)
  {
    PayloadPtr[DOIP_ROU_ACT_RESP_B5_RESERVED + Idx] = 0U;
  }

  if(TcpConPtr->TxState == DOIP_TCPCON_TX_ROUTE_ACT_OEM)
  {
    /* Add OEM specific field */
    const PduIdType DoIPPduRPduId = TcpConPtr->DoIPPduRRxPduId;

    for(Idx = 0U; Idx < DOIP_ROU_ACT_OEM_SPECIFIC_LEN; Idx++)
    {
      PayloadPtr[DOIP_ROU_ACT_RESP_B9_OEM_SPECIFIC + Idx] =
          DoIP_Channel[DoIPPduRPduId].TxOemSpecific[Idx];
    }

  }

  DBG_DOIP_FILLROUTINGACTRESPONSE_EXIT(TcpConIdx, SduDataPtr);
}


STATIC FUNC(void, DOIP_CODE) DoIP_FillGenHeaderNack
(
  DoIP_TcpConIdxType TcpConIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) SduDataPtr
)
{
  DBG_DOIP_FILLGENHEADERNACK_ENTRY(TcpConIdx, SduDataPtr);

  /* Fill generic header */
  DoIP_Fill_GenHeader(
                       SduDataPtr,
                       DOIP_PL_LENGTH_GEN_HEADER_NACK,
                       DOIP_PL_TYPE_GEN_HEADER_NACK
                     );

  /* Fill NACK Code */
  TS_AtomicAssign8(
                    SduDataPtr[DOIP_GEN_HEADER_B8_PAYLOAD],
                    DoIP_TcpCon[TcpConIdx].TxRespCode
                  );

  DBG_DOIP_FILLGENHEADERNACK_EXIT(TcpConIdx, SduDataPtr);
}


STATIC FUNC(void, DOIP_CODE) DoIP_FillAliveCheckReq
(
  DoIP_TcpConIdxType TcpConIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) SduDataPtr
)
{
  DBG_DOIP_FILLALIVECHECKREQ_ENTRY(TcpConIdx, SduDataPtr);

  /* Fill generic header */
  DoIP_Fill_GenHeader(
                       SduDataPtr,
                       DOIP_PL_LENGTH_ALIVE_REQ,
                       DOIP_PL_TYPE_ALIVE_REQ
                     );

  TS_PARAM_UNUSED(TcpConIdx);

  DBG_DOIP_FILLALIVECHECKREQ_EXIT(TcpConIdx, SduDataPtr);
}

STATIC FUNC(void, DOIP_CODE) DoIP_Fill_DiagHeader
(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) PduPtr,
  DoIP_TcpConIdxType TcpConIdx
)
{
  CONSTP2VAR(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr = &DoIP_TcpCon[TcpConIdx];
  const PduIdType DoIPPduRPduId = TcpConPtr->DoIPPduRTxPduId;
  const uint16 SenderAddr = DOIP_PBCFG_TARGET(DoIPPduRPduId);

  DBG_DOIP_FILL_DIAGHEADER_ENTRY(PduPtr, TcpConIdx);

  /* Sending diagnostic message back: Source becomes target and target becomes source. */
  PduPtr[DOIP_DIAG_MSG_B0_SOURCE_ADDR_MSB] = (uint8) (SenderAddr >> 8U);
  PduPtr[DOIP_DIAG_MSG_B1_SOURCE_ADDR_LSB] = (uint8) (SenderAddr);
  PduPtr[DOIP_DIAG_MSG_B2_TARGET_ADDR_MSB] = (uint8) (TcpConPtr->Source >> 8U);
  PduPtr[DOIP_DIAG_MSG_B3_TARGET_ADDR_LSB] = (uint8) (TcpConPtr->Source);

  DBG_DOIP_FILL_DIAGHEADER_EXIT(PduPtr, TcpConIdx);
}

STATIC FUNC(void, DOIP_CODE) DoIP_Fill_DiagAckNackHeader
(
  DoIP_TcpConIdxType TcpConIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) PduPtr
)
{
  CONSTP2VAR(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr = &DoIP_TcpCon[TcpConIdx];

  DBG_DOIP_FILL_DIAGACKNACKHEADER_ENTRY(TcpConIdx, PduPtr);

  if(TcpConPtr->TxRespCode == DOIP_DIAG_MSG_CONF_ACK)
  {
    /* Fill diagnostic header for a positive acknowledge message. */
    DoIP_Fill_DiagHeader(PduPtr, TcpConIdx);
  }
  else
  {
    /* Fill diagnostic message header for a negative acknowledge message - use source and target
     * addresses from the received message. */
    PduPtr[DOIP_DIAG_MSG_B0_SOURCE_ADDR_MSB] = (uint8) (TcpConPtr->TxDiagNackMsgTA>>8U);
    PduPtr[DOIP_DIAG_MSG_B1_SOURCE_ADDR_LSB] = (uint8) (TcpConPtr->TxDiagNackMsgTA);
    PduPtr[DOIP_DIAG_MSG_B2_TARGET_ADDR_MSB] = (uint8) (TcpConPtr->TxDiagNackMsgSA>>8U);
    PduPtr[DOIP_DIAG_MSG_B3_TARGET_ADDR_LSB] = (uint8) (TcpConPtr->TxDiagNackMsgSA);
  }

  /* Put ACK/NACK code */
  TS_AtomicAssign8(PduPtr[DOIP_DIAG_HEADERSIZE], TcpConPtr->TxRespCode);

  DBG_DOIP_FILL_DIAGACKNACKHEADER_EXIT(TcpConIdx, PduPtr);
}

STATIC FUNC(Std_ReturnType, DOIP_CODE) DoIP_Send_VID_Resp_Ann
(
  PduIdType SoAdTxPduId
)
{
  PduInfoType TxPdu;
  uint8 VinGidSyncStatus;
  Std_ReturnType Ret;

#if(DOIP_VIN_STATUS_ENABLE == STD_ON)
  uint8 PduData[DOIP_GEN_HEADER_SIZE + DOIP_PL_LENGTH_VEHICLE_ID_RESP_ANN_EXT];
  const uint32 VID_Resp_PayloadLength = DOIP_PL_LENGTH_VEHICLE_ID_RESP_ANN_EXT;
#else
  uint8 PduData[DOIP_GEN_HEADER_SIZE + DOIP_PL_LENGTH_VEHICLE_ID_RESP_ANN];
  const uint32 VID_Resp_PayloadLength = DOIP_PL_LENGTH_VEHICLE_ID_RESP_ANN;
#endif /* DOIP_VIN_STATUS_ENABLE == STD_ON */

  DBG_DOIP_SEND_VID_RESP_ANN_ENTRY(SoAdTxPduId);

  TxPdu.SduDataPtr = &PduData[0U];

  /* Fill generic header. */
  DoIP_Fill_GenHeader(
                       TxPdu.SduDataPtr,
                       VID_Resp_PayloadLength,
                       DOIP_PL_TYPE_VEHICLE_ID_RESP_ANN
                     );

  /* Fill payload. */
  Ret = DoIP_Fill_VIN(&PduData[DOIP_VID_RESP_B8_VIN_16_MSB]);

  /* Add logical address. */
  PduData[DOIP_VID_RESP_B25_LOG_ADDR_1_MSB] = DoIP_RootPtr->LogicalAddr[1U];
  PduData[DOIP_VID_RESP_B26_LOG_ADDR_0_LSB] = DoIP_RootPtr->LogicalAddr[0U];

  /* Fill entity ID. */
  TS_MemCpy(&PduData[DOIP_VID_RESP_B27_EID_6_MSB], &DoIP_EID[0U], DOIP_EID_SIZE);

  /* Fill group ID. */
  TS_MemCpy(&PduData[DOIP_VID_RESP_B33_GID_6_MSB], &DoIP_GID[0U], DOIP_GID_SIZE);

  /* Set further action byte */
  PduData[DOIP_VID_RESP_B39_FURTHER_ACTION] = (Ret == E_OK) ? DoIP_FurtherAction : DOIP_FURTHER_ACTION_NON;

  /* Set VIN/GID status */
  if((Ret == E_OK) || (DoIP_GIDSyncCompleted == TRUE))
  {
    /* VIN successfully retrieved or GID successfully synchronized. */
    VinGidSyncStatus = DOIP_VID_RESP_VIN_GID_STATUS_SYNC;
  }
  else
  {
    /* VIN invalidity pattern used. */
    VinGidSyncStatus = DOIP_VID_RESP_VIN_GID_STATUS_NOT_SYNC;
  }

#if(DOIP_VIN_STATUS_ENABLE == STD_ON)
  /* Add VIN/GID status */
  TxPdu.SduLength = DOIP_GEN_HEADER_SIZE + DOIP_PL_LENGTH_VEHICLE_ID_RESP_ANN_EXT;
  PduData[DOIP_VID_RESP_B40_VIN_GID_STATUS] = VinGidSyncStatus;
#else
  TxPdu.SduLength = DOIP_GEN_HEADER_SIZE + DOIP_PL_LENGTH_VEHICLE_ID_RESP_ANN;
#endif /* DOIP_VIN_STATUS_ENABLE == STD_ON */

  Ret = SoAd_IfTransmit(SoAdTxPduId, &TxPdu);

  DBG_DOIP_SEND_VID_RESP_ANN_EXIT(Ret, SoAdTxPduId);

  return Ret;
}

STATIC FUNC(Std_ReturnType, DOIP_CODE) DoIP_Send_Entity_Status_Resp
(
  PduIdType SoAdTxPduId
)
{
  Std_ReturnType Ret;
  DoIP_TcpConIdxType Idx;
  uint8 OpenSoc = 0U;
  PduInfoType TxPdu;
  uint8 PduData[DOIP_GEN_HEADER_SIZE + DOIP_PL_LENGTH_ENTITY_STATUS_RESP];
  const uint32 EntStatus_Resp_PayloadLength = DOIP_PL_LENGTH_ENTITY_STATUS_RESP;

  DBG_DOIP_SEND_ENTITY_STATUS_RESP_ENTRY(SoAdTxPduId);

  TxPdu.SduDataPtr = &PduData[0U];

  /* Fill generic header. */
  DoIP_Fill_GenHeader(
                       TxPdu.SduDataPtr,
                       EntStatus_Resp_PayloadLength,
                       DOIP_PL_TYPE_ENTITY_STATUS_RESP
                     );

  /* Fill payload. */
  /* Add Node Type */
  PduData[DOIP_ENT_STATUS_RESP_B0_NODE_TYPE] = DOIP_NODE_TYPE;

  /* Add Max open sockets from DoIPMaxTesterConnections */
  PduData[DOIP_ENT_STATUS_RESP_B1_MAX_OPEN_SOCKETS] = DOIP_MAX_PARALLEL_TESTER;

  /* Add Currently open Tcp sockets */
  for(Idx = 0U; Idx < (DOIP_MAX_PARALLEL_TESTER + 1U); Idx++)
  {
    if((DoIP_TcpCon[Idx].ConnectionState == DOIP_TCP_CONSTATE_SA_REGISTERED_ROUTING_ACTIVE) ||
       (DoIP_TcpCon[Idx].ConnectionState == DOIP_TCP_CONSTATE_SA_REGISTERED_PENDING))
    {
      OpenSoc++;
    }
  }
  PduData[DOIP_ENT_STATUS_RESP_B2_CUR_OPEN_SOCKETS] = OpenSoc;

  /* Add Max data size if DoIPEntityStatusMaxByteFieldUse is set */
#if(DOIP_DIAG_ENT_STATUS_MAXBYTE_USE == STD_ON)
  /* Deviation MISRAC2012-2 <+3> */
  PduData[DOIP_ENT_STATUS_RESP_B3_MAX_DATA_SIZE_3_MSB] = (uint8) (DOIP_MAX_PAYLOADSIZE >> 24U);
  PduData[DOIP_ENT_STATUS_RESP_B4_MAX_DATA_SIZE_2] = (uint8) (DOIP_MAX_PAYLOADSIZE >> 16U);
  PduData[DOIP_ENT_STATUS_RESP_B5_MAX_DATA_SIZE_1] = (uint8) (DOIP_MAX_PAYLOADSIZE >> 8U);
  PduData[DOIP_ENT_STATUS_RESP_B6_MAX_DATA_SIZE_0_LSB] = (uint8) DOIP_MAX_PAYLOADSIZE;
#endif /* DOIP_DIAG_ENT_STATUS_MAXBYTE_USE == STD_ON */

  TxPdu.SduLength = DOIP_GEN_HEADER_SIZE + DOIP_PL_LENGTH_ENTITY_STATUS_RESP;

  Ret = SoAd_IfTransmit(SoAdTxPduId, &TxPdu);

  DBG_DOIP_SEND_ENTITY_STATUS_RESP_EXIT(Ret, SoAdTxPduId);

  return Ret;
}

#if(DOIP_POWER_MODE == STD_ON)
STATIC FUNC(Std_ReturnType, DOIP_CODE) DoIP_Send_PowerMode_Resp
(
  PduIdType SoAdTxPduId
)
{
  Std_ReturnType Ret;
  PduInfoType TxPdu;
  DoIP_PowerStateType PowerState;
  uint8 PduData[DOIP_GEN_HEADER_SIZE + DOIP_PL_LENGTH_POWERMODE_RESP];

  DBG_DOIP_SEND_POWERMODE_RESP_ENTRY(SoAdTxPduId);

  TxPdu.SduDataPtr = &PduData[0U];

  /* Fill generic header. */
  DoIP_Fill_GenHeader(
                       TxPdu.SduDataPtr,
                       DOIP_PL_LENGTH_POWERMODE_RESP,
                       DOIP_PL_TYPE_POWERMODE_RESP
                     );

  /* Fill payload. */
  /* Add Power Mode byte */
  Ret = DoIP_PowerMode_Fp(&PowerState);
  if(E_OK == Ret)
  {
    PduData[DOIP_ENT_STATUS_RESP_B0_POWER_MODE] = PowerState;
  }
  else
  {
    PduData[DOIP_ENT_STATUS_RESP_B0_POWER_MODE] = 0U;
  }

  TxPdu.SduLength = DOIP_GEN_HEADER_SIZE + DOIP_PL_LENGTH_POWERMODE_RESP;

  Ret = SoAd_IfTransmit(SoAdTxPduId, &TxPdu);

  DBG_DOIP_SEND_POWERMODE_RESP_EXIT(Ret, SoAdTxPduId);

  return Ret;
}
#endif /* (DOIP_POWER_MODE == STD_ON) */

FUNC(DoIP_TcpConIdxType, DOIP_CODE) DoIP_FindTcpCon
(
  PduIdType DoIPSoAdPduId
)
{
  DoIP_TcpConIdxType Idx;

  DBG_DOIP_FINDTCPCON_ENTRY(DoIPSoAdPduId);

  for(Idx = 0U; Idx < (DOIP_MAX_PARALLEL_TESTER + 1U); Idx++)
  {
    if(DoIP_TcpCon[Idx].DoIPSoAdPduId == DoIPSoAdPduId)
    {
      break;
    }
  }

  DBG_DOIP_FINDTCPCON_EXIT(Idx, DoIPSoAdPduId);

  return Idx;
}

STATIC FUNC(void, DOIP_CODE) DoIP_ResetTcpCon
(
  DoIP_TcpConIdxType TcpConIdx
)
{
  CONSTP2VAR(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr = &DoIP_TcpCon[TcpConIdx];
  const PduIdType DoIPSoAdPduId = TcpConPtr->DoIPSoAdPduId;
  SoAd_SoConIdType SoConId = SOAD_INVALID_SOCON_ID;

  DBG_DOIP_RESETTCPCON_ENTRY(TcpConIdx);

  if(TcpConPtr->ConnectionState == DOIP_TCP_CONSTATE_SA_REGISTERED_ROUTING_ACTIVE)
  {
    const uint16 RouteIdx_Start = TcpConPtr->TesterPtr->RouteIdx;
    const uint16 RouteIdx_End = RouteIdx_Start + TcpConPtr->TesterPtr->NumberOfRoutesPerSource;
    uint16 RouteIdx;

    for(RouteIdx = RouteIdx_Start; RouteIdx < RouteIdx_End; RouteIdx++)
    {
      const PduIdType DoIPPduRPduId = DOIP_PBCFG_ROUTE(RouteIdx).DoIPPduRPduId;
      CONSTP2VAR(DoIP_ChannelType, AUTOMATIC, DOIP_APPL_DATA) ChannelPtr =
          &DoIP_Channel[DoIPPduRPduId];

      TS_AtomicAssign8(ChannelPtr->RoutingActivationState, DOIP_ROUTING_INACTIVE);

      if(ChannelPtr->PendingRequest)
      {
        TS_AtomicAssign8(ChannelPtr->PendingRequest, FALSE);
        PduR_DoIPTpTxConfirmation(
                                   DOIP_PBCFG_PDURTXPDUID(DoIPPduRPduId),
                                   NTFRSLT_E_NOT_OK
                                 );
      }
    }
  }

#if (DOIP_CUSTOM_PAYLOAD_TYPE_ENABLED == STD_ON)
  DoIP_ResetTcpConCustom(TcpConIdx);
#endif

  /* Release default Rx buffer */
  DoIP_ReleaseBuffer(TcpConIdx);

  /* Reset TcpCon[] entry. */
  if(TcpConPtr->RxState == DOIP_TCPCON_RX_DIAG_MSG)
  {
    const PduIdType DoIPPduRPduId = TcpConPtr->DoIPPduRRxPduId;
    PduR_DoIPTpRxIndication(DOIP_PBCFG_PDURRXPDUID(DoIPPduRPduId), NTFRSLT_E_NOT_OK);
  }

  /* Reset TCP connection. */
  /* Ignore return value: No error handling defined. */
  (void) SoAd_GetSoConId(DOIP_PBCFG_TCPCON(DoIPSoAdPduId).SoAdTxPduId, &SoConId);
  (void) SoAd_CloseSoCon(SoConId, TRUE);
  (void) SoAd_OpenSoCon(SoConId);

  DoIP_HandleAliveCheckConReset(TcpConIdx);

  /* Stop Inactivity and Alive check timers on this Tcp connections. */
  DoIP_StopTimer(DOIP_ALIVE_CHECK_TIMER_INDEX(TcpConIdx));
  DoIP_StopTimer(DOIP_INACTIVITY_TIMER_INDEX(TcpConIdx));

  SchM_Enter_DoIP_SCHM_DOIP_EXCLUSIVE_AREA_0();

  TcpConPtr->NextDoIPPduRPduId = DOIP_INVALID_PDUID;
  TcpConPtr->LastDoIPPduRPduId = DOIP_INVALID_PDUID;
  TcpConPtr->DoIPSoAdPduId = DOIP_INVALID_PDUID;
  TcpConPtr->DoIPPduRRxPduId = DOIP_INVALID_PDUID;
  TcpConPtr->DoIPPduRTxPduId = DOIP_INVALID_PDUID;
  TcpConPtr->ConnectionState = DOIP_TCP_CONSTATE_INIT;
  TcpConPtr->RxBufferSize = DOIP_GEN_HEADER_SIZE;

  TcpConPtr->RxState = DOIP_TCPCON_RX_OFFLINE;
  TcpConPtr->TxState = DOIP_TCPCON_TX_OFFLINE;
  TcpConPtr->MultipleAuthConfPending = FALSE;
  TcpConPtr->TesterPtr = NULL_PTR;
  TcpConPtr->NextPendingRouteIdx = DOIP_INVALID_ROUTE_IDX;

  if(DoIP_PendingAuthConfTcpConIdx == TcpConIdx)
  {
    DoIP_TcpConIdxType Idx, PendingTcpConIdx = DOIP_INVALID_TCP_CON_IDX;

    for(Idx = DoIP_PendingAuthConfTcpConIdx + 1U; Idx < DOIP_INVALID_TCP_CON_IDX; Idx++)
    {
      if(DoIP_TcpCon[Idx].NextPendingRouteIdx != DOIP_INVALID_ROUTE_IDX)
      {
        PendingTcpConIdx = Idx;
        break;
      }
    }

    DoIP_PendingAuthConfTcpConIdx = PendingTcpConIdx;
  }

  SchM_Exit_DoIP_SCHM_DOIP_EXCLUSIVE_AREA_0();

  DBG_DOIP_RESETTCPCON_EXIT(TcpConIdx);
}


STATIC FUNC(void, DOIP_CODE) DoIP_RoutingActivation
(
  DoIP_TcpConIdxType TcpConIdx,
  P2CONST(uint8, AUTOMATIC, DOIP_APPL_DATA) PduPtr,
  PduLengthType PayloadLength
)
{
  uint8 RespCode = DOIP_INVALID_RESPONSE_CODE;
  uint8 RespCode_std = DOIP_INVALID_RESPONSE_CODE;
  DoIP_TcpConTxStateType TxState = DOIP_TCPCON_TX_ROUTE_ACT;
  CONSTP2VAR(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr = &DoIP_TcpCon[TcpConIdx];

  DBG_DOIP_ROUTINGACTIVATION_ENTRY(TcpConIdx, PduPtr, PayloadLength);

  RespCode_std = DoIP_RoutingActivationStandardChannels(
                                                         TcpConIdx,
                                                         PayloadLength,
                                                         PduPtr,
                                                         &TxState
                                                       );

  if(RespCode_std == DOIP_ROU_ACT_RESP_SUCCESS_AUTO)
  {
    RespCode = DOIP_ROU_ACT_RESP_SUCCESS;
  }
  else
  {
    RespCode = RespCode_std;
  }

  TcpConPtr->RxBufferSize = DOIP_GEN_HEADER_SIZE;

  /* General inactivity timer is started or reset (in case it is already running) only in case
   * that a valid routing activation message is received. Valid means that source address is
   * registered, without taking into account eventual authentication/confirmation processes. */
  if((RespCode == DOIP_DO_NOT_SEND_RESP) ||
     (RespCode == DOIP_ROU_ACT_RESP_NO_AUTHENTICATION) ||
     (RespCode == DOIP_ROU_ACT_RESP_NO_CONFIRMATION) ||
     (RespCode == DOIP_ROU_ACT_RESP_SUCCESS) ||
     (RespCode == DOIP_ROU_ACT_RESP_PENDING_CONFIRMATION))
  {
    TcpConPtr->ResetGeneralInactivityTimer = TRUE;
  }

  if(RespCode == DOIP_DO_NOT_SEND_RESP)
  {
    /* No response message is sent, but ready to receive further routing activation requests. */
    TS_AtomicAssign8(TcpConPtr->RxState, DOIP_TCPCON_RX_READY);
  }
  else
  {
    /* Trigger sending Routing activation response message */
    DoIP_SendTcpNonDiagMsg(TcpConIdx, TxState, RespCode);
  }

/* !LINKSTO DoIP.dsn.Func0129.RoutingActivationCallback_Call, 1 */
#if (DOIP_ROUTING_ACTIVATION_CALLBACK_NUM > 0U)
  if(RespCode == DOIP_ROU_ACT_RESP_SUCCESS)
  {
    CONSTP2CONST(uint8, AUTOMATIC, DOIP_APPL_DATA) OemSpecificPtr =
    (PayloadLength == DOIP_PL_LENGTH_ROUTE_ACT_REQ_OEM) ? &PduPtr[DOIP_ROU_ACT_REQ_B7_OEM_SPECIFIC] :
                                                          NULL_PTR;

    DoIP_CallRoutingActivationCallbacks(
                                         TcpConPtr,
                                         PduPtr[DOIP_ROU_ACT_REQ_B2_ACT_TYPE],
                                         OemSpecificPtr
                                       );
  }
#endif /* DOIP_ROUTING_ACTIVATION_CALLBACK_NUM > 0U */

  DBG_DOIP_ROUTINGACTIVATION_EXIT(TcpConIdx, PduPtr, PayloadLength);
}

STATIC FUNC(uint8, DOIP_CODE) DoIP_RoutingActivationStandardChannels
(
  DoIP_TcpConIdxType TcpConIdx,
  PduLengthType PayloadLength,
  P2CONST(uint8, AUTOMATIC, DOIP_APPL_DATA) PduPtr,
  P2VAR(DoIP_TcpConTxStateType, AUTOMATIC, DOIP_APPL_DATA) TxStatePtr
)
{
  const uint8 ActivationNumber = PduPtr[DOIP_ROU_ACT_REQ_B2_ACT_TYPE];
  uint8 RespCode = DOIP_INVALID_RESPONSE_CODE;

  /* Indicates if a channel with automatic routing activation was activated. */
  boolean AutomaticChannelActivated = FALSE;
  uint16 Idx;
  CONSTP2VAR(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr = &DoIP_TcpCon[TcpConIdx];

  DBG_DOIP_ROUTINGACTIVATIONSTANDARDCHANNELS_ENTRY(
                                                    TcpConIdx,
                                                    PayloadLength,
                                                    PduPtr,
                                                    TxStatePtr
                                                  );

  if(TcpConPtr->TesterPtr == NULL_PTR)
  {
    for(Idx = 0U; Idx < DoIP_RootPtr->TesterMax; Idx++)
    {
      CONSTP2CONST(DoIP_TesterType, AUTOMATIC, DOIP_APPL_CONST) TesterPtr = &DOIP_PBCFG_TESTER(Idx);
      if(TesterPtr->Source == TcpConPtr->Source)
      {
        TS_AtomicAssign16(
                           TcpConPtr->PrevDiagMsgSize,
                           TesterPtr->NumBytesDiagNAck
                         );
        TcpConPtr->TesterPtr = TesterPtr;
        break;
      }
    }
  }

  if(TcpConPtr->TesterPtr != NULL_PTR)
  {
    const uint16 RouteIdx_Start = TcpConPtr->TesterPtr->RouteIdx;
    const uint16 RouteIdx_End = RouteIdx_Start + TcpConPtr->TesterPtr->NumberOfRoutesPerSource;
    boolean ActivationNumberFound = FALSE;
    boolean UpdatePending = !(TcpConPtr->MultipleAuthConfPending);
    P2VAR(DoIP_ChannelType, AUTOMATIC, DOIP_APPL_DATA) ChannelProcessedPtr = NULL_PTR;
    DoIP_RoutingActivationIdxType PendingRoutingActIdx = 0U;

    if(TcpConPtr->NextPendingRouteIdx != DOIP_INVALID_ROUTE_IDX)
    {
      PendingRoutingActIdx = DOIP_PBCFG_ROUTE(TcpConPtr->NextPendingRouteIdx).RoutingActivationIdx;
    }

    for(Idx = RouteIdx_Start; Idx < RouteIdx_End; Idx++)
    {
      const PduIdType DoIPPduRPduId = DOIP_PBCFG_ROUTE(Idx).DoIPPduRPduId;
      const DoIP_RoutingActivationIdxType RoutingActIdx =
          DOIP_PBCFG_ROUTE(Idx).RoutingActivationIdx;

      if(RoutingActIdx != DOIP_AUTOMATIC_ROUTING_ACTIVATION_IDX)
      {
        /* Channel is activated by routing activation message. */
        CONSTP2CONST(DoIP_RoutingActType, AUTOMATIC, DOIP_APPL_CONST) RoutingActPtr =
            &DOIP_PBCFG_ROUTINGACT(RoutingActIdx);

        if(ActivationNumber == RoutingActPtr->ActivationNumber)
        {
          const boolean RoutingActSecRequired =
              DOIP_PBCFG_ROUTINGACT(RoutingActIdx).RoutingActivationSecurityRequired;
          const boolean TcpConSecRequired =
              DOIP_PBCFG_TCPCON(TcpConPtr->DoIPSoAdPduId).TcpConnectionSecurityRequired;
          ActivationNumberFound = TRUE;

          if((TcpConSecRequired == FALSE) && (RoutingActSecRequired == TRUE))
          {
            RespCode = DOIP_ROU_ACT_RESP_TCP_CONN_UNSECURED;
          }
          else
          {
            DoIP_RoutingActivationChannel(
                                           TcpConIdx,
                                           DoIPPduRPduId,
                                           PayloadLength,
                                           RoutingActIdx,
                                           &ChannelProcessedPtr,
                                           PduPtr,
                                           TxStatePtr,
                                           &RespCode
                                         );

            if(UpdatePending == TRUE)
            {
              CONSTP2VAR(DoIP_ChannelType, AUTOMATIC, DOIP_APPL_DATA) ChannelPtr =
                &DoIP_Channel[DoIPPduRPduId];

              if(DOIP_IS_CHANNEL_PENDING(ChannelPtr))
              {
                if(TcpConPtr->NextPendingRouteIdx == DOIP_INVALID_ROUTE_IDX)
                {
                  TcpConPtr->NextPendingRouteIdx = Idx;

                  DoIP_PendingAuthConfTcpConIdx = DOIP_MIN(DoIP_PendingAuthConfTcpConIdx, TcpConIdx);

                  UpdatePending = FALSE;
                }
                else if(RoutingActIdx != PendingRoutingActIdx)
                {
                  TcpConPtr->MultipleAuthConfPending = TRUE;
                  UpdatePending = FALSE;
                }
                else
                {
                  /* do nothing */
                }
              }
            }
          }
        }
      }
      else
      {
        /* Automatic routing activation for this channel -> activate it. */
        AutomaticChannelActivated = TRUE;
        ActivationNumberFound = TRUE; /* At least one channel activated -> skip sending NACK. */
        DoIP_Channel[DoIPPduRPduId].TcpConIdx = TcpConIdx;
        TS_AtomicAssign8(DoIP_Channel[DoIPPduRPduId].RoutingActivationState, DOIP_ROUTING_ACTIVE);
        TS_AtomicAssign8(
                          TcpConPtr->ConnectionState,
                          DOIP_TCP_CONSTATE_SA_REGISTERED_ROUTING_ACTIVE
                        );
      }
    }
    if(ActivationNumberFound == FALSE)
    {
      /* Received Activation number does not match any DoIPRoutingActivationNumber referenced
       * in Testers - send NACK. */
      RespCode = DOIP_ROU_ACT_RESP_UNKNOWN_ACT_TYPE;
    }
  }
  else
  {
    /* Source address does not match any configured DoIPTesterSA - send NACK.*/
    RespCode = DOIP_ROU_ACT_RESP_UNKNOWN_SRC;
  }

  if((RespCode == DOIP_INVALID_RESPONSE_CODE) && (AutomaticChannelActivated == TRUE))
  {
    /* Report SUCCESS only if no routing activation objects have been processed. */
    RespCode = DOIP_ROU_ACT_RESP_SUCCESS_AUTO;
  }

  DBG_DOIP_ROUTINGACTIVATIONSTANDARDCHANNELS_EXIT(
                                                    RespCode,
                                                    TcpConIdx,
                                                    PayloadLength,
                                                    PduPtr,
                                                    TxStatePtr
                                                  );

  return RespCode;
}

FUNC(void, DOIP_CODE) DoIP_RoutingActivationChannel
(
  DoIP_TcpConIdxType TcpConIdx,
  PduIdType DoIPPduRPduId,
  PduLengthType PayloadLength,
  DoIP_RoutingActivationIdxType RoutingActIdx,
  P2VAR(DoIP_ChannelPtrType, AUTOMATIC, DOIP_APPL_DATA) ChannelProcessedPtrPtr,
  P2CONST(uint8, AUTOMATIC, DOIP_APPL_DATA) PduPtr,
  P2VAR(DoIP_TcpConTxStateType, AUTOMATIC, DOIP_APPL_DATA) TxStatePtr,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) RespCodePtr
)
{
  CONSTP2VAR(DoIP_ChannelType, AUTOMATIC, DOIP_APPL_DATA) ChannelPtr = &DoIP_Channel[DoIPPduRPduId];

  DBG_DOIP_ROUTINGACTIVATIONCHANNEL_ENTRY(
                                           TcpConIdx,
                                           DoIPPduRPduId,
                                           PayloadLength,
                                           RoutingActIdx,
                                           ChannelProcessedPtrPtr,
                                           PduPtr,
                                           TxStatePtr,
                                           RespCodePtr
                                         );

  if(ChannelPtr->RoutingActivationState == DOIP_ROUTING_INACTIVE)
  {
    P2VAR(DoIP_ChannelType, AUTOMATIC, DOIP_APPL_DATA) ChannelProcessedPtr =
        *ChannelProcessedPtrPtr;
    /* If routing activation is already active -> no need for further processing.
     * If routing activation is pending, it will be processed in MainFunction.
     * This new routing activation request will be silently discarded. */
    if(ChannelProcessedPtr != NULL_PTR)
    {
      /* Requested routing activation is already processed -> just copy results. */
      TS_MemCpy(
                 &(ChannelPtr->RxOemSpecific[0U]),
                 &(ChannelProcessedPtr->RxOemSpecific[0U]),
                 DOIP_ROU_ACT_OEM_SPECIFIC_LEN
               );
      TS_MemCpy(
                 &(ChannelPtr->TxOemSpecific[0U]),
                 &(ChannelProcessedPtr->TxOemSpecific[0U]),
                 DOIP_ROU_ACT_OEM_SPECIFIC_LEN
               );
      ChannelPtr->TcpConIdx = ChannelProcessedPtr->TcpConIdx;

      ChannelPtr->RoutingActivationIdx = ChannelProcessedPtr->RoutingActivationIdx;

      ChannelPtr->OemSpecificValid = ChannelProcessedPtr->OemSpecificValid;

      TS_AtomicAssign8(
                        ChannelPtr->RoutingActivationState,
                        ChannelProcessedPtr->RoutingActivationState
                      );
    }
    else
    {
      ChannelPtr->RoutingActivationIdx = RoutingActIdx;

      DoIP_TcpCon[TcpConIdx].DoIPPduRRxPduId = DoIPPduRPduId;

      if(PayloadLength == DOIP_PL_LENGTH_ROUTE_ACT_REQ_OEM)
      {
        /* If received routing activation message contains OEM specific field, save it
         * for further processing in case of pending activation. */
        TS_MemCpy(
                   &(ChannelPtr->RxOemSpecific[0U]),
                   &PduPtr[DOIP_ROU_ACT_REQ_B7_OEM_SPECIFIC],
                   DOIP_ROU_ACT_OEM_SPECIFIC_LEN
                 );
        ChannelPtr->OemSpecificValid = TRUE;
      }
      else
      {
        ChannelPtr->OemSpecificValid = FALSE;
      }

      /* Clear OEM specific field of response message. */
      TS_MemBZero(&ChannelPtr->TxOemSpecific[0U], DOIP_ROU_ACT_OEM_SPECIFIC_LEN);

      *RespCodePtr = DoIP_VerifyAuthenticationConfirmation(TcpConIdx, ChannelPtr, TxStatePtr);
      *ChannelProcessedPtrPtr = ChannelPtr; /* Mark processed channel. */
    }
  }
  else
  {
    /* Routing activation already processed. */
    if(ChannelPtr->RoutingActivationState == DOIP_ROUTING_ACTIVE)
    {
      /* Routing activation is already active -> send positive response message. */
      *RespCodePtr = DOIP_ROU_ACT_RESP_SUCCESS;
    }
    else if(ChannelPtr->RoutingActivationState == DOIP_ROUTING_PENDING_CONFIRMATION)
    {
      /* Routing activation is pending on confirmation -> send positive response message.*/
      *RespCodePtr = DOIP_ROU_ACT_RESP_PENDING_CONFIRMATION;
    }
    else
    {
      /* Do not send any response messages. */
      *RespCodePtr = DOIP_DO_NOT_SEND_RESP;
    }
  }

  DBG_DOIP_ROUTINGACTIVATIONCHANNEL_EXIT(
                                          TcpConIdx,
                                          DoIPPduRPduId,
                                          PayloadLength,
                                          RoutingActIdx,
                                          ChannelProcessedPtrPtr,
                                          PduPtr,
                                          TxStatePtr,
                                          RespCodePtr
                                         );
}


STATIC FUNC(uint8, DOIP_CODE) DoIP_VerifyAuthenticationConfirmation
(
  DoIP_TcpConIdxType TcpConIdx,
  P2VAR(DoIP_ChannelType, AUTOMATIC, DOIP_APPL_DATA) ChannelPtr,
  P2VAR(DoIP_TcpConTxStateType, AUTOMATIC, DOIP_APPL_DATA) TxStatePtr
)
{
  uint8 RespCode = DOIP_DO_NOT_SEND_RESP; /* do not send response message (pending) */

  const DoIP_RoutingActivationIdxType RoutingActIdx = ChannelPtr->RoutingActivationIdx;

  CONSTP2CONST(DoIP_RoutingActType, AUTOMATIC, DOIP_APPL_CONST) RoutingActPtr =
      &DOIP_PBCFG_ROUTINGACT(RoutingActIdx);

  DBG_DOIP_VERIFYAUTHENTICATIONCONFIRMATION_ENTRY(TcpConIdx, ChannelPtr, TxStatePtr);

  switch(ChannelPtr->RoutingActivationState)
  {
    case DOIP_ROUTING_INACTIVE:
    case DOIP_ROUTING_PENDING_AUTHENTICATION:
    {
      RespCode = DoIP_VerifyAuthentication(TcpConIdx, ChannelPtr);

      if(ChannelPtr->RoutingActivationState == DOIP_ROUTING_AUTHENTIFIED)
      {
        RespCode = DoIP_VerifyConfirmation(TcpConIdx, ChannelPtr);
      }
      break;
    }

    case DOIP_ROUTING_AUTHENTIFIED:
    case DOIP_ROUTING_PENDING_CONFIRMATION:
    {
      /* Authentication verified - check confirmation. */
      RespCode = DoIP_VerifyConfirmation(TcpConIdx, ChannelPtr);
      break;
    }
    /* CHECK: NOPARSE */
    default:
    {
      DOIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    }
    /* CHECK: PARSE */
  }

  if((RoutingActPtr->AuthResLength > 0) || (RoutingActPtr->ConfResLength > 0))
  {
    /* Attach OEM specific field into response message. */
    *TxStatePtr = DOIP_TCPCON_TX_ROUTE_ACT_OEM;
  }
  else
  {
    /* Send response message without OEM specific. */
    *TxStatePtr =  DOIP_TCPCON_TX_ROUTE_ACT;
  }

  DBG_DOIP_VERIFYAUTHENTICATIONCONFIRMATION_EXIT(RespCode, TcpConIdx, ChannelPtr, TxStatePtr);

  return RespCode;
}

STATIC FUNC(uint8, DOIP_CODE) DoIP_VerifyAuthentication
(
  DoIP_TcpConIdxType TcpConIdx,
  P2VAR(DoIP_ChannelType, AUTOMATIC, DOIP_APPL_DATA) ChannelPtr
)
{
  CONSTP2CONST(DoIP_RoutingActType, AUTOMATIC, DOIP_APPL_CONST) RoutingActPtr =
      &DOIP_PBCFG_ROUTINGACT(ChannelPtr->RoutingActivationIdx);

  /* Do not send response message (authentication is verified or pending). */
  uint8 RespCode = DOIP_DO_NOT_SEND_RESP;
  uint8 ResData[DOIP_ROU_ACT_OEM_SPECIFIC_LEN];
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) ReqDataPtr = NULL_PTR;
  boolean Authentified = TRUE;
  Std_ReturnType RetVal;

  DBG_DOIP_VERIFYAUTHENTICATION_ENTRY(TcpConIdx, ChannelPtr);

  if((RoutingActPtr->AuthReqLength > 0) && (ChannelPtr->OemSpecificValid == TRUE))
  {
    ReqDataPtr = &(ChannelPtr->RxOemSpecific[0U]);
  }

  /* Call authentication function if provided, dummy function otherwise. */
  RetVal = DoIP_RoutingActivationAuth_FpList[ChannelPtr->RoutingActivationIdx](
                                                                                &Authentified,
                                                                                ReqDataPtr,
                                                                                &ResData[0U]
                                                                              );

  if(RetVal == E_OK)
  {
    /* Copy AuthenticationResLength bytes into response message OEM specific field. */
    TS_MemCpy(
               &(ChannelPtr->TxOemSpecific[0U]),
               &ResData[0U],
               RoutingActPtr->AuthResLength
             );

    if(Authentified == FALSE)
    {
      /* Routing activation authentication failed -> send response message. */
      RespCode = DOIP_ROU_ACT_RESP_NO_AUTHENTICATION;
      TS_AtomicAssign8(ChannelPtr->RoutingActivationState, DOIP_ROUTING_INACTIVE);
    }
    else
    {
      /* Authentication verified -> it can be proceed with confirmation verification. */
      TS_AtomicAssign8(ChannelPtr->RoutingActivationState, DOIP_ROUTING_AUTHENTIFIED);
    }
  }
  else if(RetVal == DOIP_E_PENDING)
  {
    /* Try again in MainFunction(). */
    ChannelPtr->TcpConIdx = TcpConIdx;
    TS_AtomicAssign8(ChannelPtr->RoutingActivationState, DOIP_ROUTING_PENDING_AUTHENTICATION);
  }
  else
  {
    /* e.g. E_NOT_OK */
    /* Routing activation authentication failed -> send response message. */
    RespCode = DOIP_ROU_ACT_RESP_NO_AUTHENTICATION;
    TS_AtomicAssign8(ChannelPtr->RoutingActivationState, DOIP_ROUTING_INACTIVE);
  }

  DBG_DOIP_VERIFYAUTHENTICATION_EXIT(RespCode, TcpConIdx, ChannelPtr);

  return RespCode;
}

STATIC FUNC(uint8, DOIP_CODE) DoIP_VerifyConfirmation
(
  DoIP_TcpConIdxType TcpConIdx,
  P2VAR(DoIP_ChannelType, AUTOMATIC, DOIP_APPL_DATA) ChannelPtr
)
{
  CONSTP2CONST(DoIP_RoutingActType, AUTOMATIC, DOIP_APPL_CONST) RoutingActPtr =
      &DOIP_PBCFG_ROUTINGACT(ChannelPtr->RoutingActivationIdx);

  const DoIP_RoutingStateType PrevRoutingActivationState = ChannelPtr->RoutingActivationState;

  /* Do not send response message (authentication is verified or pending). */
  uint8 RespCode = DOIP_DO_NOT_SEND_RESP;
  uint8 ResData[DOIP_ROU_ACT_OEM_SPECIFIC_LEN];
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) ReqDataPtr = NULL_PTR;
  /* Authentication verified - check confirmation. */
  boolean Confirmed = TRUE;
  const uint8 ReqOffset = (uint8) DOIP_ROU_ACT_OEM_SPECIFIC_LEN - RoutingActPtr->ConfReqLength;
  Std_ReturnType RetVal;

  DBG_DOIP_VERIFYCONFIRMATION_ENTRY(TcpConIdx, ChannelPtr);

  if((RoutingActPtr->ConfReqLength > 0) && (ChannelPtr->OemSpecificValid == TRUE))
  {
    ReqDataPtr = &(ChannelPtr->RxOemSpecific[ReqOffset]);
  }

  /* Call confirmation function if provided, dummy function otherwise. */
  RetVal = DoIP_RoutingActivationConf_FpList[ChannelPtr->RoutingActivationIdx](
                                                                                &Confirmed,
                                                                                ReqDataPtr,
                                                                                &ResData[0U]
                                                                              );
  if(RetVal == E_OK)
  {
    const uint8 ResOffset = (uint8) DOIP_ROU_ACT_OEM_SPECIFIC_LEN - RoutingActPtr->ConfResLength;
    TS_MemCpy(
               &(ChannelPtr->TxOemSpecific[ResOffset]),
               &ResData[0U],
               RoutingActPtr->ConfResLength
             );

    if(Confirmed == TRUE)
    {
      /* Routing activation both authentified and confirmed -> send positive response message. */
      RespCode = DOIP_ROU_ACT_RESP_SUCCESS;

      SchM_Enter_DoIP_SCHM_DOIP_EXCLUSIVE_AREA_0();
      ChannelPtr->TcpConIdx = TcpConIdx;
      ChannelPtr->RoutingActivationState = DOIP_ROUTING_ACTIVE;
      DoIP_TcpCon[TcpConIdx].ConnectionState = DOIP_TCP_CONSTATE_SA_REGISTERED_ROUTING_ACTIVE;
      SchM_Exit_DoIP_SCHM_DOIP_EXCLUSIVE_AREA_0();

      if(RoutingActPtr->ActivationNumber == DOIP_FURTHER_ACTION_ROUTING_ACT_NUM)
      {
        DoIP_FurtherAction = DOIP_FURTHER_ACTION_NON;
      }
    }
    else
    {
      /* Routing activation confirmation failed -> send response message. */
      RespCode = DOIP_ROU_ACT_RESP_NO_CONFIRMATION;
      TS_AtomicAssign8(ChannelPtr->RoutingActivationState, DOIP_ROUTING_INACTIVE);
    }
  }
  else if(RetVal == DOIP_E_PENDING)
  {
    /* Routing activation confirmation pending -> try again in MainFunction(). */
    ChannelPtr->TcpConIdx = TcpConIdx;
    TS_AtomicAssign8(ChannelPtr->RoutingActivationState, DOIP_ROUTING_PENDING_CONFIRMATION);
    /* Send response message with response code 0x11 (pending confirmation) only once.
     * Subsequent evaluations in MainFunction() shall not result in message generation. */
    if(PrevRoutingActivationState != DOIP_ROUTING_PENDING_CONFIRMATION)
    {
      RespCode = DOIP_ROU_ACT_RESP_PENDING_CONFIRMATION;
    }
  }
  else
  {
    /* e.g. E_NOT_OK */
    /* Routing activation confirmation failed -> send response message. */
    RespCode = DOIP_ROU_ACT_RESP_NO_CONFIRMATION;
    TS_AtomicAssign8(ChannelPtr->RoutingActivationState, DOIP_ROUTING_INACTIVE);
  }

  DBG_DOIP_VERIFYCONFIRMATION_EXIT(RespCode, TcpConIdx, ChannelPtr);

  return RespCode;
}

#if (DOIP_ROUTING_ACTIVATION_CALLBACK_NUM > 0)
STATIC FUNC(void , DOIP_CODE) DoIP_CallRoutingActivationCallbacks
(
  P2CONST(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr,
  uint8 ActivationType,
  P2CONST(uint8, AUTOMATIC, DOIP_APPL_DATA) OemSpecificPtr
)
{
  SoAd_SoConIdType SoConId = SOAD_INVALID_SOCON_ID;
  Std_ReturnType RetVal;

  DBG_DOIP_CALLROUTINGACTIVATIONCALLBACKS_ENTRY(TcpConPtr, ActivationType, OemSpecificPtr);

  /* !LINKSTO DoIP.dsn.Func0129.RoutingActivationCallback_GetSoConId, 1 */
  RetVal = SoAd_GetSoConId(DOIP_PBCFG_TCPCON(TcpConPtr->DoIPSoAdPduId).SoAdTxPduId, &SoConId);
  if(RetVal == E_OK)
  {
    uint8 Idx;
    for(Idx = 0U; Idx < DOIP_ROUTING_ACTIVATION_CALLBACK_NUM; Idx++)
    {

      DoIP_RoutingActivationCallback_FpList[Idx](
                                                  SoConId,
                                                  TcpConPtr->Source,
                                                  ActivationType,
                                                  OemSpecificPtr
                                                );
    }
  }

  /* SoAd_GetSoConId return NOT_OK only if configuration is broken - which is protected with */
  /* xdm checks. */
  DOIP_POSTCONDITION_ASSERT(RetVal == E_OK, DOIP_SID_SOADTPCOPYRXDATA);

  DBG_DOIP_CALLROUTINGACTIVATIONCALLBACKS_EXIT(TcpConPtr, ActivationType, OemSpecificPtr);
}
#endif /* DOIP_ROUTING_ACTIVATION_CALLBACK_NUM > 0) */

FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_HandleSegmentedRx
(
  DoIP_TcpConIdxType TcpConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_DATA) PduInfoPtr,
  P2VAR(DoIP_DataPtrType, AUTOMATIC, DOIP_APPL_DATA) RxMsgPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_DATA) MsgLengthPtr
)
{
  BufReq_ReturnType BufReq_Ret = BUFREQ_E_NOT_OK;
  CONSTP2VAR(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr = &DoIP_TcpCon[TcpConIdx];

  *RxMsgPtr = NULL_PTR; /* message not yet completed */

  DBG_DOIP_HANDLESEGMENTEDRX_ENTRY(TcpConIdx, PduInfoPtr, RxMsgPtr, MsgLengthPtr);

  if(0U == PduInfoPtr->SduLength)
  {
    /* This is a query for the available buffer size -> return the value stored in RxBufferSize. */
    BufReq_Ret = BUFREQ_OK;
  }
  else if(PduInfoPtr->SduLength <= TcpConPtr->RxBufferSize)
  {
    const PduLengthType DataInBuffer = DoIP_GetBufferDataSize(TcpConIdx);
    if((PduInfoPtr->SduLength < TcpConPtr->RxBufferSize) || (DataInBuffer > 0U))
    {
      /* !LINKSTO DoIP.ASR41.SWS_DoIP_00214, 1 */
      /* Segmented reception -> store data in a default RX buffer. */
      DoIP_WriteBuffer(TcpConIdx, PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);
      TcpConPtr->RxBufferSize -= PduInfoPtr->SduLength;

      if(TcpConPtr->RxBufferSize == 0U)
      {
        /* Message received and buffered completely -> retrieve buffer address. */
        *RxMsgPtr = DoIP_GetBufferDataPtr(TcpConIdx);
        *MsgLengthPtr = DoIP_GetBufferDataSize(TcpConIdx);
      }
    }
    else
    {
      /* The complete message received at once - no need for buffering. */
      *RxMsgPtr = PduInfoPtr->SduDataPtr;
      *MsgLengthPtr = PduInfoPtr->SduLength;
    }

    BufReq_Ret = BUFREQ_OK;
  }
  else
  {
    /* SoAd has provided more data then requested -> return E_NOT_OK. */
  }

  DBG_DOIP_HANDLESEGMENTEDRX_EXIT(BufReq_Ret, TcpConIdx, PduInfoPtr, RxMsgPtr, MsgLengthPtr);

  return BufReq_Ret;
}

STATIC FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_HandleRoutingActivation
(
  DoIP_TcpConIdxType TcpConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_DATA) PduInfoPtr
)
{
  DoIP_DataPtrType RoutActReqPtr = NULL_PTR;
  PduLengthType PayloadLength;
  DoIP_TcpConIdxType Idx;
  Std_ReturnType RetVal;
  BufReq_ReturnType BufReq_Ret;

  CONSTP2VAR(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr = &DoIP_TcpCon[TcpConIdx];

  DBG_DOIP_HANDLEROUTINGACTIVATION_ENTRY(TcpConIdx, PduInfoPtr);

  BufReq_Ret = DoIP_HandleSegmentedRx(TcpConIdx, PduInfoPtr, &RoutActReqPtr, &PayloadLength);

  if((BufReq_Ret == BUFREQ_OK) && (RoutActReqPtr != NULL_PTR))
  {
    boolean Continue = FALSE;
    const uint16 ReceivedSA = DoIP_Make_uint16
      (
        RoutActReqPtr[DOIP_ROU_ACT_REQ_B0_TESTER_ADDR_MSB],
        RoutActReqPtr[DOIP_ROU_ACT_REQ_B1_TESTER_ADDR_LSB]
      );

    /* Save tester source address for generating response message. */
    TcpConPtr->RoutActReqSA = ReceivedSA;

    TcpConPtr->RxBufferSize = DOIP_GEN_HEADER_SIZE;

    if(TcpConPtr->ConnectionState == DOIP_TCP_CONSTATE_INIT)
    {
      /* SA is not assigned on this connection -> check if the received SA is assigned
       * to some other connection. */
      uint8 NumConSA = 0U;    /* Number of connections that have SA equal to the received SA. */
      DoIP_TcpConIdxType TcpConIdx_DoAliveCheck = 0U, NumUnassignedSA = 0U;

      /* Assign received SA to the connection. */
      TS_AtomicAssign8(TcpConPtr->ConnectionState, DOIP_TCP_CONSTATE_SA_REQUESTED);
      TS_AtomicAssign16(TcpConPtr->Source, ReceivedSA);

      for(Idx = 0U; Idx < (DOIP_MAX_PARALLEL_TESTER + 1U); Idx++)
      {
        if(Idx == TcpConIdx)
        {
          continue;
        }

        if(DoIP_TcpCon[Idx].ConnectionState == DOIP_TCP_CONSTATE_INIT)
        {
          NumUnassignedSA++;
        }
        else if(DoIP_TcpCon[Idx].Source == ReceivedSA)
        {
          NumConSA++;
          TcpConIdx_DoAliveCheck = Idx;   /* Perform Alive check on this connection. */
        }
        else
        {
          /* nothing to do */
        }
      }

      if(NumConSA == 0U)
      {
        /* Received SA is not assigned to any other connection. */
        if(NumUnassignedSA == 0U)
        {
          /* No free Tcp connection -> run Alive check on all registered connections. */
          DoIP_StartAllConnAliveCheck = TRUE;

          DoIP_NoFreeTcpConIdx = TcpConIdx;

          if(RoutActReqPtr == PduInfoPtr->SduDataPtr)
          {
            /* Store Routing activation message in buffer for later processing... */
            DoIP_WriteBuffer(TcpConIdx, PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);
          }

          /* ... and wait all connections Alive check to finish. */
          TcpConPtr->RxState = DOIP_TCPCON_RX_ALL_CON_ALIVE_CHECK_WAIT;
          /* Indicate SoAd that DoIP is not ready to receive new message (BufferSizePtr == 0). */
          TS_AtomicAssign32(TcpConPtr->RxBufferSize, 0U);
        }
        else
        {
          /* There is free connection -> assign SA and continue with routing activation process. */
          TS_AtomicAssign8(TcpConPtr->ConnectionState, DOIP_TCP_CONSTATE_SA_REGISTERED_PENDING);
          Continue = TRUE;
        }
      }
      else if (NumConSA == 1U)
      {
        /* Other connection with received SA exist -> run single connection Alive check. */
        RetVal = DoIP_SendAliveCheckRequest(TcpConIdx_DoAliveCheck);
        if(RetVal == E_OK)
        {
          /* Alive check request successfully transmitted. */
          DoIP_NumActiveSingleAliveCheck++;

          if(RoutActReqPtr == PduInfoPtr->SduDataPtr)
          {
            /* Store Routing activation message in buffer for later processing... */
            DoIP_WriteBuffer(TcpConIdx, PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);
          }

          /* ... and wait single connection alive check to finish. */
          TcpConPtr->RxState = DOIP_TCPCON_RX_SINGLE_ALIVE_CHECK_WAIT;
          /* Indicate SoAd that DoIP is not ready to receive new message (BufferSizePtr == 0). */
          TS_AtomicAssign32(TcpConPtr->RxBufferSize, 0U);
        }
        else
        {
          /* Alive check request not transmitted because TX side is busy -> connection is alive. */
          DoIP_SendTcpNonDiagMsg(TcpConIdx, DOIP_TCPCON_TX_ROUTE_ACT, DOIP_ROU_ACT_RESP_SRC_IN_USE);
        }
      }
      else /* NumConSA > 1U */
      {
        /* More then one connection with received SA exist -> there is already a connection waiting
         * on Alive check -> send Routing activation NACK. This will lead to a connection reset. */
        DoIP_SendTcpNonDiagMsg(TcpConIdx, DOIP_TCPCON_TX_ROUTE_ACT, DOIP_ROU_ACT_RESP_SRC_IN_USE);
      }
    }
    else if (TcpConPtr->Source != ReceivedSA)
    {
      /* SA is assigned to this connection but different from the received SA -> send NACK. */
      DoIP_SendTcpNonDiagMsg(TcpConIdx, DOIP_TCPCON_TX_ROUTE_ACT, DOIP_ROU_ACT_RESP_WRONG_SRC);
    }
    else
    {
      Continue = TRUE;
    }

    if(Continue == TRUE)
    {
      DoIP_RoutingActivation(
                              TcpConIdx,
                              RoutActReqPtr,
                              PayloadLength
                            );

      /* Clear connection buffer. */
      DoIP_ClearBuffer(TcpConIdx);
    }
  }

  DBG_DOIP_HANDLEROUTINGACTIVATION_EXIT(BufReq_Ret, TcpConIdx, PduInfoPtr);

  return BufReq_Ret;
}


STATIC FUNC(Std_ReturnType, DOIP_CODE) DoIP_SendAliveCheckRequest
(
  DoIP_TcpConIdxType TcpConIdx
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  CONSTP2VAR(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr = &DoIP_TcpCon[TcpConIdx];

  DBG_DOIP_SENDALIVECHECKREQUEST_ENTRY(TcpConIdx);

  /* Request is transmitted only on connection with assigned source address.
   * No need to repeat the request if it is already in progress. */
  if((DoIP_IsTimerActive(DOIP_ALIVE_CHECK_TIMER_INDEX(TcpConIdx)) == FALSE) &&
     ((TcpConPtr->ConnectionState == DOIP_TCP_CONSTATE_SA_REGISTERED_ROUTING_ACTIVE) ||
      (TcpConPtr->ConnectionState == DOIP_TCP_CONSTATE_SA_REGISTERED_PENDING)))
  {
    if(DOIP_TCPCON_TX_IDLE == TcpConPtr->TxState)
    {
      const PduIdType SoAdTxPduId = DOIP_PBCFG_TCPCON(TcpConPtr->DoIPSoAdPduId).SoAdTxPduId;
      PduInfoType SoAdPduInfo;

      TS_AtomicAssign8(TcpConPtr->TxState, DOIP_TCPCON_TX_ALIVE_CHECK_REQ);

      /* Set Tx message size. */
      SoAdPduInfo.SduLength = DOIP_GEN_HEADER_SIZE + DOIP_PL_LENGTH_ALIVE_REQ;
      SoAdPduInfo.SduDataPtr = NULL_PTR;

      RetVal = SoAd_TpTransmit(SoAdTxPduId, &SoAdPduInfo);

      if(RetVal == E_OK)
      {
        /* Alive check request has been accepted - start counting. */
        DoIP_StartTimer(
                         DOIP_ALIVE_CHECK_TIMER_INDEX(TcpConIdx),
                         DOIP_ALIVE_CHECK_RESPONSE_TIMEOUT
                        );
      }
      else
      {
        TS_AtomicAssign8(TcpConPtr->TxState, DOIP_TCPCON_TX_IDLE);
      }
    }
    /* else - TX is busy -> connection is alive -> no need for Alive check. */
  }

  DBG_DOIP_SENDALIVECHECKREQUEST_EXIT(RetVal, TcpConIdx);

  return RetVal;
}

STATIC FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_HandleRxStartReady
(
  DoIP_TcpConIdxType TcpConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_DATA) PduInfoPtr
)
{
  BufReq_ReturnType BufReq_Ret;
  DoIP_DataPtrType GenHeadPtr = NULL_PTR;
  PduLengthType Length; /* not needed for generic header */

  DBG_DOIP_HANDLERXSTARTREADY_ENTRY(TcpConIdx, PduInfoPtr);

  BufReq_Ret = DoIP_HandleSegmentedRx(TcpConIdx, PduInfoPtr, &GenHeadPtr, &Length);

  if((BufReq_Ret == BUFREQ_OK) && (GenHeadPtr != NULL_PTR))
  {
    /* complete generic header available */
    DoIP_Tcp_Check_GenHeader(
                              TcpConIdx,
                              GenHeadPtr
                            );

    /* Clear connection buffer. */
    DoIP_ClearBuffer(TcpConIdx);
  }

  DBG_DOIP_HANDLERXSTARTREADY_EXIT(BufReq_Ret, TcpConIdx, PduInfoPtr);

  return BufReq_Ret;
}

STATIC FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_HandleDiagHeader
(
  DoIP_TcpConIdxType TcpConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_DATA) PduInfoPtr
)
{
  BufReq_ReturnType BufReq_Ret;
  DoIP_DataPtrType DiagHeaderPtr = NULL_PTR;
  PduLengthType Length; /* not needed here*/

  DBG_DOIP_HANDLEDIAGHEADER_ENTRY(TcpConIdx, PduInfoPtr);

  BufReq_Ret = DoIP_HandleSegmentedRx(TcpConIdx, PduInfoPtr, &DiagHeaderPtr, &Length);

  if((BufReq_Ret == BUFREQ_OK) && (DiagHeaderPtr != NULL_PTR))
  {
    BufReq_Ret = DoIP_StartDiagReception(
                                          TcpConIdx,
                                          DiagHeaderPtr
                                        );

    /* Clear connection buffer. */
    DoIP_ClearBuffer(TcpConIdx);
  }

  DBG_DOIP_HANDLEDIAGHEADER_EXIT(BufReq_Ret, TcpConIdx, PduInfoPtr);

  return BufReq_Ret;
}

STATIC FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_FindDiagMsgRoute
(
  DoIP_TcpConIdxType TcpConIdx,
  uint16 SourceAddr,
  uint16 TargetAddr,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) RespCodePtr
)
{
  BufReq_ReturnType BufReq_Ret;
  uint16 Idx = 0;

  CONSTP2VAR(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr = &DoIP_TcpCon[TcpConIdx];
  P2CONST(DoIP_TesterType, AUTOMATIC, DOIP_APPL_CONST) TesterPtr = TcpConPtr->TesterPtr;

  if(TesterPtr == NULL_PTR)
  {
    for(Idx = 0U; Idx < DoIP_RootPtr->TesterMax; Idx++)
    {
      if(DOIP_PBCFG_TESTER(Idx).Source == SourceAddr)
      {
        TesterPtr = &DOIP_PBCFG_TESTER(Idx);
        TS_AtomicAssign16(TcpConPtr->PrevDiagMsgSize, TesterPtr->NumBytesDiagNAck);
        break;
      }
    }
  }

  if(TesterPtr != NULL_PTR)
  {
    const uint16 RouteIdx_Start = TesterPtr->RouteIdx;
    const uint16 RouteIdx_End = RouteIdx_Start + TesterPtr->NumberOfRoutesPerSource;
    boolean TargetAddrFound = FALSE;

    BufReq_Ret = BUFREQ_OK;

    for(Idx = RouteIdx_Start; (Idx < RouteIdx_End) && (TargetAddrFound == FALSE); Idx++)
    {
      PduIdType DoIPPduRPduId = DOIP_PBCFG_ROUTE(Idx).DoIPPduRPduId;
      if(TargetAddr == DOIP_PBCFG_TARGET(DoIPPduRPduId))
      {
        /* Target address found. */
        TargetAddrFound = TRUE;
        if(DoIP_Channel[DoIPPduRPduId].RoutingActivationState == DOIP_ROUTING_INACTIVE)
        {
          /* Activate channel in case routing activation type is AUTOMATIC. */
          if(DOIP_PBCFG_ROUTE(Idx).RoutingActivationIdx ==
              DOIP_AUTOMATIC_ROUTING_ACTIVATION_IDX)
          {
            uint16 Idx2;

            /* When diagnostic message is received on an INACTIVE channel with Automatic routing
             * activation it means that Tcp connection is in UNASSIGNED state (in REQUESTED
             * state reception would be blocked, in ASSIGNED state all automatic channels would be
             * ACTIVE).
             * Consequently the received Source address is not assigned to this, but also to
             * any other Tcp connection - it is safe safe to assign it without further checks. */

            /* Assign received Source address to the Tcp connection. */
            TcpConPtr->Source = SourceAddr;
            TcpConPtr->ConnectionState = DOIP_TCP_CONSTATE_SA_REGISTERED_ROUTING_ACTIVE;
            TcpConPtr->TesterPtr = TesterPtr;

            /* All channels originating from this source address have AUTOMATIC routing activation
               - set them to active. */
            for(Idx2 = RouteIdx_Start; Idx2 < RouteIdx_End; Idx2++)
            {
              const DoIP_RouteType Route = DOIP_PBCFG_ROUTE(Idx2);

              /* Activate channel and assign it Tcp connection. */
              DoIP_Channel[Route.DoIPPduRPduId].RoutingActivationState = DOIP_ROUTING_ACTIVE;
              DoIP_Channel[Route.DoIPPduRPduId].TcpConIdx = TcpConIdx;
            }
          }
        }

        if((DoIP_Channel[DoIPPduRPduId].RoutingActivationState == DOIP_ROUTING_ACTIVE) &&
            (TcpConPtr->ConnectionState == DOIP_TCP_CONSTATE_SA_REGISTERED_ROUTING_ACTIVE))
        {
          PduLengthType BufferSize;

          TcpConPtr->DoIPPduRRxPduId = DoIPPduRPduId;

          BufReq_Ret = PduR_DoIPStartOfReception
          (
            DOIP_PBCFG_PDURRXPDUID(DoIPPduRPduId),
            (PduLengthType) TcpConPtr->RxDiagMsgLength,
            &BufferSize
          );

          if(BufReq_Ret != BUFREQ_OK)
          {
            *RespCodePtr = (BufReq_Ret == BUFREQ_E_OVFL) ?
                        DOIP_DIAG_MSG_NACK_PDUR_BUF_OVFL : DOIP_DIAG_MSG_NACK_PDUR_BUF_NOT_OK;
          }
          else
          {
            TcpConPtr->RxBufferSize = DOIP_MIN(TcpConPtr->RxDiagMsgLength, BufferSize);
            *RespCodePtr = DOIP_DIAG_MSG_HDR_OK;
          }
        }
        else
        {
          /* Diagnostic message received but the according route is not activated -
           * discard the message and send NACK if allowed. */
#if (DOIP_RESPONSE_BEFORE_ROUTACT == DOIP_ENABLE_ALL_DIAG_NACK)
          *RespCodePtr = DOIP_DIAG_MSG_NACK_ROUTE_INACTIVE;
#else
          *RespCodePtr = TcpConPtr->ConnectionState == DOIP_TCP_CONSTATE_SA_REGISTERED_ROUTING_ACTIVE ?
                         DOIP_DIAG_MSG_NACK_ROUTE_INACTIVE : DOIP_DO_NOT_SEND_RESP;
#endif
        }
      }
    }

    if(TargetAddrFound == FALSE)
    {
      /* Diagnostic message received with unknown target address - send NACK. */
      *RespCodePtr = DOIP_DIAG_MSG_NACK_INV_TAR_ADDR;
    }
  }
  else
  {
    /* No Tester (DoIPTesterSA) is configured with received source address - send NACK. */
    *RespCodePtr = DOIP_DIAG_MSG_NACK_INV_SRC_ADDR;
    BufReq_Ret = BUFREQ_OK;
  }

  return BufReq_Ret;
}

STATIC FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_StartDiagReception
(
  DoIP_TcpConIdxType TcpConIdx,
  P2CONST(uint8, AUTOMATIC, DOIP_APPL_DATA) PduPtr
)
{
  BufReq_ReturnType BufReq_Ret;
  uint8 RespCode = 0xFFU;  /* no errors found */

  CONSTP2VAR(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr = &DoIP_TcpCon[TcpConIdx];

  const uint16 SourceAddr
    = DoIP_Make_uint16(
                        PduPtr[DOIP_DIAG_MSG_B0_SOURCE_ADDR_MSB],
                        PduPtr[DOIP_DIAG_MSG_B1_SOURCE_ADDR_LSB]
                      );
  const uint16 TargetAddr
    = DoIP_Make_uint16(
                        PduPtr[DOIP_DIAG_MSG_B2_TARGET_ADDR_MSB],
                        PduPtr[DOIP_DIAG_MSG_B3_TARGET_ADDR_LSB]
                      );

  DBG_DOIP_STARTDIAGRECEPTION_ENTRY(TcpConIdx, PduPtr);

  /* Save values of received SA and TA for eventual sending of NACK. */
  TS_AtomicAssign16(TcpConPtr->RxDiagMsgSA, SourceAddr);
  TS_AtomicAssign16(TcpConPtr->RxDiagMsgTA, TargetAddr);

  TcpConPtr->DoIPPduRRxPduId = DOIP_INVALID_PDUID;

  if((SourceAddr != TcpConPtr->Source) && (TcpConPtr->ConnectionState != DOIP_TCP_CONSTATE_INIT))
  {
    /* Connection is already registered to another source address - send NACK. */
    RespCode = DOIP_DIAG_MSG_NACK_INV_SRC_ADDR;
    BufReq_Ret = BUFREQ_OK;
  }
  else
  {
    BufReq_Ret = DoIP_FindDiagMsgRoute(TcpConIdx, SourceAddr, TargetAddr, &RespCode);
  }

  if(RespCode == DOIP_DIAG_MSG_HDR_OK)
  {
    /* No NACK -> continue with a reception of diagnostic message. */
    TS_AtomicAssign8(TcpConPtr->RxState, DOIP_TCPCON_RX_DIAG_MSG);
  }
  else if((RespCode == DOIP_DIAG_MSG_NACK_INV_SRC_ADDR) || (TcpConPtr->RxDiagMsgLength == 0U))
  {
    /* Send NACK immediately - the connection will be reset or message length is 0 ->
     * nothing to discard. */
    TcpConPtr->RxBufferSize = DOIP_GEN_HEADER_SIZE;
    DoIP_SendTcpNonDiagMsg(TcpConIdx, DOIP_TCPCON_TX_DIAG_ACK_NACK_START, RespCode);

#if(DOIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)
    /* Increase Diag message MeasurementData counter. */
    DoIP_IncMeasurementDropDataCounter(DOIP_MEAS_DROP_DIAGMSG);
#endif /* DOIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */
  }
  else
  {
    if(RespCode == DOIP_DIAG_MSG_NACK_PDUR_BUF_OVFL)
    {
      /* Global inactivity timer is reset in case UL reject reception due to
       * insufficient buffers. */
      TcpConPtr->ResetGeneralInactivityTimer = TRUE;
    }
    /* Discard the message and release SoAd buffers. */
    TS_AtomicAssign8(TcpConPtr->RxState, DOIP_TCPCON_RX_DISCARD_DIAG_MSG);

    /* NACK will be sent when the complete message is delivered to DoIP - save the response code */
    TS_AtomicAssign8(TcpConPtr->TxRespCode, RespCode);
    TcpConPtr->RxBufferSize = TcpConPtr->RxDiagMsgLength;

#if(DOIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)
    /* Increase Diag message MeasurementData counter. */
    DoIP_IncMeasurementDropDataCounter(DOIP_MEAS_DROP_DIAGMSG);
#endif /* DOIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */

    BufReq_Ret = BUFREQ_OK;
  }

  DBG_DOIP_STARTDIAGRECEPTION_EXIT(BufReq_Ret, TcpConIdx, PduPtr);

  return BufReq_Ret;
}

/* !LINKSTO DoIP.ASR41.SWS_DoIP_00259, 1 */
STATIC FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_CopyDiagMsg
(
  DoIP_TcpConIdxType TcpConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_DATA) PduInfoPtr
)
{
  BufReq_ReturnType BufReq_Ret = BUFREQ_E_NOT_OK;
  CONSTP2VAR(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr = &DoIP_TcpCon[TcpConIdx];

  DBG_DOIP_COPYDIAGMSG_ENTRY(TcpConIdx, PduInfoPtr);

  if(PduInfoPtr->SduLength <= TcpConPtr->RxDiagMsgLength)
  {
    const PduLengthType PduLength = PduInfoPtr->SduLength;
    const PduIdType DoIPPduRPduId = TcpConPtr->DoIPPduRRxPduId;
    PduLengthType BufferSize;

    /* Save a part of the original msg. payload, as defined in parameter DoIPNumByteDiagAckNack.*/
    const PduLengthType CopySize = DOIP_MIN
        (
          TcpConPtr->PrevDiagMsgSize - DoIP_GetBufferDataSize(TcpConIdx),
          PduInfoPtr->SduLength
        );

    DoIP_WriteBuffer(TcpConIdx, PduInfoPtr->SduDataPtr, CopySize);

    BufReq_Ret = PduR_DoIPCopyRxData(
                                      DOIP_PBCFG_PDURRXPDUID(DoIPPduRPduId),
                                      PduInfoPtr,
                                      &BufferSize
                                    );

    TcpConPtr->RxDiagMsgLength -= PduLength;

    if(BufReq_Ret == BUFREQ_OK)
    {
      if(0U == TcpConPtr->RxDiagMsgLength)
      {
        TcpConPtr->RxBufferSize = DOIP_GEN_HEADER_SIZE;

        /* General inactivity timer is reset upon reception of valid diagnostic message. */
        TS_AtomicAssign8(TcpConPtr->ResetGeneralInactivityTimer, TRUE);

          /* Send positive diagnostic acknowledge message and notify UL */
          DoIP_SendTcpNonDiagMsg(
                                  TcpConIdx,
                                  DOIP_TCPCON_TX_DIAG_ACK_NACK_START,
                                  DOIP_DIAG_MSG_CONF_ACK
                                );

        PduR_DoIPTpRxIndication(DOIP_PBCFG_PDURRXPDUID(DoIPPduRPduId), NTFRSLT_OK);
      }
      else
      {
        TcpConPtr->RxBufferSize = DOIP_MIN(TcpConPtr->RxDiagMsgLength, BufferSize);
      }
    }
    else
    {
      uint8 RespCode;

      if(BufReq_Ret == BUFREQ_E_OVFL)
      {
        /* Global inactivity timer is reset in case UL reject reception due to
         * insufficient buffers. */
        TcpConPtr->ResetGeneralInactivityTimer = TRUE;
        RespCode = DOIP_DIAG_MSG_NACK_PDUR_BUF_OVFL;
      }
      else
      {
        RespCode = DOIP_DIAG_MSG_NACK_PDUR_BUF_NOT_OK;
      }

      if(0U == TcpConPtr->RxDiagMsgLength)
      {
        /* The complete message is delivered by SoAd -> SoAd buffers are empty -> send NACK.*/
        TcpConPtr->RxBufferSize = DOIP_GEN_HEADER_SIZE;

        DoIP_SendTcpNonDiagMsg( TcpConIdx, DOIP_TCPCON_TX_DIAG_ACK_NACK_START, RespCode);
      }
      else
      {
        /* Part of the message is still in SoAd, consume it to release SoAd buffers. */
        TS_AtomicAssign8(TcpConPtr->RxState, DOIP_TCPCON_RX_DISCARD_DIAG_MSG);

        /* NACK will be send when the complete message is delivered to DoIP - save response code */
        TS_AtomicAssign8(TcpConPtr->TxRespCode, RespCode);

        TcpConPtr->RxBufferSize = TcpConPtr->RxDiagMsgLength;
      }

      PduR_DoIPTpRxIndication(DOIP_PBCFG_PDURRXPDUID(DoIPPduRPduId), NTFRSLT_E_NOT_OK);

      BufReq_Ret = BUFREQ_OK;

#if(DOIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)
      /* Increase Diag message MeasurementData counter. */
      DoIP_IncMeasurementDropDataCounter(DOIP_MEAS_DROP_DIAGMSG);
#endif /* DOIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */
    }
  }
  /* else - return BUFREQ_E_NOT_OK */

  DBG_DOIP_COPYDIAGMSG_EXIT(BufReq_Ret, TcpConIdx, PduInfoPtr);

  return BufReq_Ret;
}


STATIC FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_DiscardDiagMsg
(
  DoIP_TcpConIdxType TcpConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_DATA) PduInfoPtr
)
{
  BufReq_ReturnType BufReq_Ret = BUFREQ_E_NOT_OK;
  CONSTP2VAR(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr = &DoIP_TcpCon[TcpConIdx];

  DBG_DOIP_DISCARDDIAGMSG_ENTRY(TcpConIdx, PduInfoPtr);

  if(PduInfoPtr->SduLength <= TcpConPtr->RxDiagMsgLength)
  {
    /* Save a part of the original msg. payload, as defined in parameter DoIPNumByteDiagAckNack.*/
    const PduLengthType CopySize = DOIP_MIN
        (
          TcpConPtr->PrevDiagMsgSize - DoIP_GetBufferDataSize(TcpConIdx),
          PduInfoPtr->SduLength
        );

    DoIP_WriteBuffer(TcpConIdx, PduInfoPtr->SduDataPtr, CopySize);

    TcpConPtr->RxDiagMsgLength -= PduInfoPtr->SduLength;

    if(0U == TcpConPtr->RxDiagMsgLength)
    {
      TcpConPtr->RxBufferSize = DOIP_GEN_HEADER_SIZE;

      if(TcpConPtr->TxRespCode != DOIP_DO_NOT_SEND_RESP)
      {
        /* All data from SoAd have been consumed - send NACK and notify UL. */
        DoIP_SendTcpNonDiagMsg(
                                TcpConIdx,
                                DOIP_TCPCON_TX_DIAG_ACK_NACK_START,
                                TcpConPtr->TxRespCode
                              );
      }
      else
      {
        DoIP_ReleaseBuffer(TcpConIdx);
        TS_AtomicAssign8(TcpConPtr->RxState, DOIP_TCPCON_RX_READY);
      }
    }
    else
    {
      TcpConPtr->RxBufferSize = TcpConPtr->RxDiagMsgLength;
    }

    BufReq_Ret = BUFREQ_OK;
  }
  /* else - return BUFREQ_E_NOT_OK */

  DBG_DOIP_DISCARDDIAGMSG_EXIT(BufReq_Ret, TcpConIdx, PduInfoPtr);

  return BufReq_Ret;
}


STATIC FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_DiscardMsg
(
  DoIP_TcpConIdxType TcpConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_DATA) PduInfoPtr
)
{
  BufReq_ReturnType BufReq_Ret = BUFREQ_E_NOT_OK;
  CONSTP2VAR(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr = &DoIP_TcpCon[TcpConIdx];

  DBG_DOIP_DISCARDMSG_ENTRY(TcpConIdx, PduInfoPtr);

  if(PduInfoPtr->SduLength <= TcpConPtr->RxBufferSize)
  {
    TcpConPtr->RxBufferSize -= PduInfoPtr->SduLength;

    if(0U == TcpConPtr->RxBufferSize)
    {
      /* The complete message has been read-out, prepare for the next generic header */
      TcpConPtr->RxBufferSize = DOIP_GEN_HEADER_SIZE;

      if(TcpConPtr->TxRespCode != DOIP_DO_NOT_SEND_RESP)
      {
        /* All data from SoAd have been consumed - send NACK if required and notify UL. */
        DoIP_SendTcpNonDiagMsg(TcpConIdx, DOIP_TCPCON_TX_GENHEADER_NACK, TcpConPtr->TxRespCode);
      }
      else
      {
        /* prepare for the next generic header */
        TS_AtomicAssign32(TcpConPtr->RxBufferSize, DOIP_GEN_HEADER_SIZE);
        TS_AtomicAssign8(TcpConPtr->RxState, DOIP_TCPCON_RX_READY);
      }
    }

    BufReq_Ret = BUFREQ_OK;
  }
  /* else - return BUFREQ_E_NOT_OK */

  DBG_DOIP_DISCARDMSG_EXIT(BufReq_Ret, TcpConIdx, PduInfoPtr);

  return BufReq_Ret;
}

STATIC FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_HandleAliveCheckResp
(
  DoIP_TcpConIdxType TcpConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_DATA) PduInfoPtr
)
{
  BufReq_ReturnType BufReq_Ret;
  DoIP_DataPtrType AliveCheckRespPtr = NULL_PTR;
  PduLengthType PayloadLength;
  CONSTP2VAR(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr = &DoIP_TcpCon[TcpConIdx];

  DBG_DOIP_HANDLEALIVECHECKRESP_ENTRY(TcpConIdx, PduInfoPtr);

  BufReq_Ret = DoIP_HandleSegmentedRx(TcpConIdx, PduInfoPtr, &AliveCheckRespPtr, &PayloadLength);

  if((BufReq_Ret == BUFREQ_OK) && (AliveCheckRespPtr != NULL_PTR))
  {
    uint16 ReceivedSA = DoIP_Make_uint16
    (
      AliveCheckRespPtr[DOIP_ALIVE_RESP_MSG_B0_SOURCE_ADDR_MSB],
      AliveCheckRespPtr[DOIP_ALIVE_RESP_MSG_B1_SOURCE_ADDR_LSB]
    );

    TcpConPtr->RxBufferSize = DOIP_GEN_HEADER_SIZE;

    if((TcpConPtr->ConnectionState == DOIP_TCP_CONSTATE_SA_REGISTERED_ROUTING_ACTIVE) ||
       (TcpConPtr->ConnectionState == DOIP_TCP_CONSTATE_SA_REGISTERED_PENDING))
    {
      if(TcpConPtr->Source == ReceivedSA)
      {
        /* General inactivity timer is reset when valid Alive check response is received. */
        TS_AtomicAssign8(TcpConPtr->ResetGeneralInactivityTimer, TRUE);

        if(DoIP_IsTimerActive(DOIP_ALIVE_CHECK_TIMER_INDEX(TcpConIdx)) == TRUE)
        {
          DoIP_TcpConIdxType Idx;

          /* Check if connection exist that is waiting on this source address. */
          if(DoIP_NumActiveSingleAliveCheck > 0)
          {
            boolean Found = FALSE;

            for(Idx = 0U; (Idx < (DOIP_MAX_PARALLEL_TESTER + 1U)) && (Found == FALSE); Idx++)
            {
              if(TcpConIdx == Idx)
              {
                continue;
              }

              if((DoIP_TcpCon[Idx].RxState == DOIP_TCPCON_RX_SINGLE_ALIVE_CHECK_WAIT) &&
                 (DoIP_TcpCon[Idx].Source == TcpConPtr->Source))
              {
                /* Since Alive check succeeded, send Routing activation NACK. */
                DoIP_SendTcpNonDiagMsg(Idx, DOIP_TCPCON_TX_ROUTE_ACT, DOIP_ROU_ACT_RESP_SRC_IN_USE);

                DoIP_NumActiveSingleAliveCheck--;

                Found = TRUE;
              }
            }
          }

          /* Disable Alive check for this connection. */
          DoIP_StopTimer(DOIP_ALIVE_CHECK_TIMER_INDEX(TcpConIdx));
        }

        /* Connection is ready for new messages. */
        TS_AtomicAssign8(TcpConPtr->RxState, DOIP_TCPCON_RX_READY);
      }
      else
      {
        /* Received source address does not match registered one. */
        DoIP_ResetTcpCon(TcpConIdx);
      }
    }
    else
    {
      /* Connection is ready for new messages. */
      TS_AtomicAssign8(TcpConPtr->RxState, DOIP_TCPCON_RX_READY);
    }

    /* Clear connection buffer. */
    DoIP_ClearBuffer(TcpConIdx);
  }

  DBG_DOIP_HANDLEALIVECHECKRESP_EXIT(BufReq_Ret, TcpConIdx, PduInfoPtr);

  return BufReq_Ret;
}

FUNC(void, DOIP_CODE) DoIP_SendTcpNonDiagMsg
(
  DoIP_TcpConIdxType TcpConIdx,
  DoIP_TcpConTxStateType TxState,
  uint8 RespCode
)
{
  CONSTP2VAR(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr = &DoIP_TcpCon[TcpConIdx];
  const PduIdType DoIPPduRPduId = TcpConPtr->DoIPPduRRxPduId;

  DBG_DOIP_SENDTCPNONDIAGMSG_ENTRY(TcpConIdx, TxState, RespCode);

  if(DOIP_TCPCON_TX_IDLE == TcpConPtr->TxState)
  {
    PduInfoType SoAdPduInfo;
    const PduIdType SoAdTxPduId = DOIP_PBCFG_TCPCON(TcpConPtr->DoIPSoAdPduId).SoAdTxPduId;
    Std_ReturnType Ret;

    SchM_Enter_DoIP_SCHM_DOIP_EXCLUSIVE_AREA_0();
    TcpConPtr->DoIPPduRTxPduId = DoIPPduRPduId;

    /* Block reception until response / acknowledge is transmitted */
    TcpConPtr->RxState = DOIP_TCPCON_RX_ACK_CONF_WAIT;
    /* Indicate SoAd that DoIP is not ready to receive new message (BufferSizePtr == 0). */
    TcpConPtr->RxBufferSize = 0U;

    TcpConPtr->TxState = TxState;
    TcpConPtr->TxRespCode = RespCode;

    if((TxState == DOIP_TCPCON_TX_DIAG_ACK_NACK_START) && (RespCode != DOIP_DIAG_MSG_CONF_ACK))
    {
      /* Source and Target addresses from the received DiagMsg -> necessary when sending NACK */
      TS_AtomicAssign16(TcpConPtr->TxDiagNackMsgSA, TcpConPtr->RxDiagMsgSA);
      TS_AtomicAssign16(TcpConPtr->TxDiagNackMsgTA, TcpConPtr->RxDiagMsgTA);
    }
    SchM_Exit_DoIP_SCHM_DOIP_EXCLUSIVE_AREA_0();

    /* Calculate Tx message size */
    SoAdPduInfo.SduLength = DoIP_GetTxSduLength(TcpConIdx, TxState);
    SoAdPduInfo.SduDataPtr = NULL_PTR;

    Ret = SoAd_TpTransmit(SoAdTxPduId, &SoAdPduInfo);

    if(E_OK != Ret)
    {
      TS_AtomicAssign8(TcpConPtr->TxState, DOIP_TCPCON_TX_IDLE);

      /* Release default Rx buffer */
      DoIP_ReleaseBuffer(TcpConIdx);
      TS_AtomicAssign8(TcpConPtr->RxState, DOIP_TCPCON_RX_READY);
      TS_AtomicAssign32(TcpConPtr->RxBufferSize, DOIP_GEN_HEADER_SIZE);
    }
  }
  else
  {
    SchM_Enter_DoIP_SCHM_DOIP_EXCLUSIVE_AREA_0();

    TcpConPtr->RequestedTxState_DoIPPduRTxPduId = DoIPPduRPduId;

    /* Block reception until response / acknowledge is transmitted */
    TcpConPtr->RxState = DOIP_TCPCON_RX_ACK_CONF_WAIT;
    /* Indicate SoAd that DoIP is not ready to receive new message (BufferSizePtr == 0). */
    TcpConPtr->RxBufferSize = 0U;

    TcpConPtr->TxRespCode = RespCode;
    TcpConPtr->RequestedTxState = TxState;

    if(RespCode != DOIP_DIAG_MSG_CONF_ACK)
    {
      TcpConPtr->TxDiagNackMsgSA = TcpConPtr->RxDiagMsgSA;
      TcpConPtr->TxDiagNackMsgTA = TcpConPtr->RxDiagMsgTA;
    }
    SchM_Exit_DoIP_SCHM_DOIP_EXCLUSIVE_AREA_0();
  }

  DBG_DOIP_SENDTCPNONDIAGMSG_EXIT(TcpConIdx, TxState, RespCode);
}


STATIC FUNC(PduLengthType, DOIP_CODE) DoIP_GetTxSduLength
(
  DoIP_TcpConIdxType TcpConIdx,
  DoIP_TcpConTxStateType TxState
)
{
  PduLengthType SduLength = DOIP_GEN_HEADER_SIZE;

  DBG_DOIP_GETTXSDULENGTH_ENTRY(TcpConIdx, TxState);

  if(TxState == DOIP_TCPCON_TX_DIAG_ACK_NACK_START)
  {
    /* Payload length depends on a number of bytes that has been successfully copied from the
     * original message. */
    SduLength += DOIP_PL_LENGTH_DIAG_ACK_NACK + DoIP_GetBufferDataSize(TcpConIdx);
  }
  else if(TxState == DOIP_TCPCON_TX_GENHEADER_NACK)
  {
    SduLength += DOIP_PL_LENGTH_GEN_HEADER_NACK;
  }
  else if(TxState == DOIP_TCPCON_TX_ROUTE_ACT)
  {
    SduLength += DOIP_PL_LENGTH_ROUTE_ACT_RESP;
  }
  else
  {
    DOIP_PRECONDITION_ASSERT(TxState == DOIP_TCPCON_TX_ROUTE_ACT_OEM, DET_INTERNAL_API_ID);
    SduLength += DOIP_PL_LENGTH_ROUTE_ACT_RESP_OEM;
  }

  DBG_DOIP_GETTXSDULENGTH_EXIT(SduLength, TcpConIdx, TxState);

  return SduLength;
}

STATIC FUNC(void, DOIP_CODE) DoIP_FillDiagMsgHdr
(
  DoIP_TcpConIdxType TcpConIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) SduDataPtr
)
{
  const PduIdType DoIPPduRPduId = DoIP_TcpCon[TcpConIdx].DoIPPduRTxPduId;

  const uint32 PayloadLength
    = (uint32) DoIP_Channel[DoIPPduRPduId].TxDiagMsgLength + DOIP_DIAG_HEADERSIZE;

  DBG_DOIP_FILLDIAGMSGHDR_ENTRY(TcpConIdx, SduDataPtr);

  /* Fill generic header */
  DoIP_Fill_GenHeader(SduDataPtr, PayloadLength, DOIP_PL_TYPE_DIAG_MSG);

  /* Fill diagnostic header: Source and target address */
  DoIP_Fill_DiagHeader(
                        &SduDataPtr[DOIP_GEN_HEADER_B8_PAYLOAD],
                        TcpConIdx
                      );

  DBG_DOIP_FILLDIAGMSGHDR_EXIT(TcpConIdx, SduDataPtr);
}


STATIC FUNC(void, DOIP_CODE) DoIP_FillDiagAckNackHdr
(
  DoIP_TcpConIdxType TcpConIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) SduDataPtr
)
{
  uint16 PayloadType;
  uint32 PayloadLength;

  /* At least there is enough space for generic and diagnostic headers. */
  PayloadType = (DoIP_TcpCon[TcpConIdx].TxRespCode == DOIP_DIAG_MSG_CONF_ACK) ?
                 DOIP_PL_TYPE_DIAG_ACK : DOIP_PL_TYPE_DIAG_NACK;

  PayloadLength = (uint32) DOIP_PL_LENGTH_DIAG_ACK_NACK + DoIP_GetBufferDataSize(TcpConIdx);

  DBG_DOIP_FILLDIAGACKNACKHDR_ENTRY(TcpConIdx, SduDataPtr);

  /* Fill generic header */
  DoIP_Fill_GenHeader(SduDataPtr, PayloadLength, PayloadType);

  /* Fill diagnostic header: Source and target address + acknowledge code. */
  DoIP_Fill_DiagAckNackHeader(
                               TcpConIdx,
                               &SduDataPtr[DOIP_GEN_HEADER_SIZE]
                             );

  DBG_DOIP_FILLDIAGACKNACKHDR_EXIT(TcpConIdx, SduDataPtr);

}

STATIC FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_FillDiagMsgPld
(
  DoIP_TcpConIdxType TcpConIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_DATA) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_DATA) AvailableDataPtr
)
{
  CONSTP2VAR(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr = &DoIP_TcpCon[TcpConIdx];
  const PduIdType DoIPPduRPduId = TcpConPtr->DoIPPduRTxPduId;
  BufReq_ReturnType BufReq_Ret;

  DBG_DOIP_FILLDIAGMSGPLD_ENTRY(TcpConIdx, PduInfoPtr, AvailableDataPtr);

  BufReq_Ret = PduR_DoIPCopyTxData(
                                    DOIP_PBCFG_PDURTXPDUID(DoIPPduRPduId),
                                    PduInfoPtr,
                                    NULL_PTR,
                                    AvailableDataPtr
                                  );
  if(BUFREQ_OK == BufReq_Ret)
  {
    /* All requested data copied -> reduce the remaining DiagMsg size. */
    DoIP_Channel[DoIPPduRPduId].TxDiagMsgLength -= PduInfoPtr->SduLength;

    /* Reset timeout */
    TS_AtomicAssign8(TcpConPtr->ResetGeneralInactivityTimer, TRUE);

    /* Function is called to copy data (and not only to provide data size) ->
     * headers are already copied. */
    if(PduInfoPtr->SduDataPtr != NULL_PTR)
    {
      TS_AtomicAssign8(TcpConPtr->TxState, DOIP_TCPCON_TX_DIAG_MSG);
    }
  }

  DBG_DOIP_FILLDIAGMSGPLD_EXIT(BufReq_Ret, TcpConIdx, PduInfoPtr, AvailableDataPtr);

  return BufReq_Ret;
}


STATIC FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_FillDiagAckNackPld
(
  DoIP_TcpConIdxType TcpConIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_DATA) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_DATA) AvailableDataPtr
)
{
  DBG_DOIP_FILLDIAGACKNACKPLD_ENTRY(TcpConIdx, PduInfoPtr, AvailableDataPtr);

  /* Function is called to copy data (and not only to provide data size) ->
   * headers are already copied. */
  if(PduInfoPtr->SduDataPtr != NULL_PTR)
  {
    TS_AtomicAssign8(DoIP_TcpCon[TcpConIdx].TxState, DOIP_TCPCON_TX_DIAG_ACK_NACK);
  }

  *AvailableDataPtr = DoIP_ReadBuffer(TcpConIdx, PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);

  if(*AvailableDataPtr == 0U)
  {
    DoIP_ReleaseBuffer(TcpConIdx);
  }

  DBG_DOIP_FILLDIAGACKNACKPLD_EXIT(BUFREQ_OK, TcpConIdx, PduInfoPtr, AvailableDataPtr);

  return BUFREQ_OK;
}


STATIC FUNC(void, DOIP_CODE) DoIP_SoAdRequestReleaseIpAddr
(
  SoAd_SoConIdType SoConId,
  DoIP_OperationType Operation
)
{
  DBG_DOIP_SOADREQUESTRELEASEIPADDR_ENTRY(SoConId, Operation);

  if(Operation == DOIP_REQUEST_IP_ADDR_OP)
  {
    (void) SoAd_RequestIpAddrAssignment(SoConId, TCPIP_IPADDR_ASSIGNMENT_ALL, NULL_PTR);
  }
  else
  {
    (void) SoAd_ReleaseIpAddrAssignment(SoConId);
  }

  DBG_DOIP_SOADREQUESTRELEASEIPADDR_EXIT(SoConId, Operation);
}


STATIC FUNC(void, DOIP_CODE) DoIP_RequestReleaseIpAddrAllConn
(
    DoIP_OperationType Operation
)
{
  DoIP_TcpConIdxType Idx;
  SoAd_SoConIdType SoConId;

  DBG_DOIP_REQUESTRELEASEIPADDRALLCONN_ENTRY(Operation);

  /* Request/Release Ip address assignment for all Udp and Tcp connections for which
   * configuration parameter RequestAddressAssignment is enabled. */
  for(Idx = 0U; Idx < DoIP_RootPtr->UdpConMax; Idx++)
  {
    if(DOIP_PBCFG_UDPCON(Idx).IpAddrAssignReq == TRUE)
    {
      if(SoAd_GetSoConId(DOIP_PBCFG_UDPCON(Idx).SoAdTxPduId, &SoConId) == E_OK)
      {
        DoIP_SoAdRequestReleaseIpAddr(SoConId, Operation);
      }
    }
  }

  for(Idx = 0U; Idx < DoIP_RootPtr->UdpAnnConMax; Idx++)
  {
    if(DOIP_PBCFG_UDPANNCON(Idx).IpAddrAssignReq == TRUE)
    {
      if(SoAd_GetSoConId(DOIP_PBCFG_UDPANNCON(Idx).SoAdTxPduId, &SoConId) == E_OK)
      {
        DoIP_SoAdRequestReleaseIpAddr(SoConId, Operation);
      }
    }
  }

  for(Idx = 0U; Idx < DoIP_RootPtr->TcpConMax; Idx++)
  {
    if(DOIP_PBCFG_TCPCON(Idx).IpAddrAssignReq == TRUE)
    {
      if(SoAd_GetSoConId(DOIP_PBCFG_TCPCON(Idx).SoAdTxPduId, &SoConId) == E_OK)
      {
        DoIP_SoAdRequestReleaseIpAddr(SoConId, Operation);
      }
    }
  }

  DBG_DOIP_REQUESTRELEASEIPADDRALLCONN_EXIT(Operation);
}

STATIC FUNC(boolean, DOIP_CODE) DoIP_GetUpdateHostnameOption
(
  SoAd_SoConIdType SoConId
)
{
  DoIP_TcpConIdxType Idx;
  SoAd_SoConIdType SoConId_read;
  boolean UpdateHostnameOption = FALSE, Found = FALSE;

  DBG_DOIP_GETUPDATEHOSTNAMEOPTION_ENTRY(SoConId);

  for(Idx = 0U; ((Idx < DoIP_RootPtr->UdpConMax) && (Found == FALSE)); Idx++)
  {
    if(SoAd_GetSoConId(DOIP_PBCFG_UDPCON(Idx).SoAdTxPduId, &SoConId_read) == E_OK)
    {
      if(SoConId == SoConId_read)
      {
        UpdateHostnameOption = DOIP_PBCFG_UDPCON(Idx).UpdateHostName;
        Found = TRUE;
      }
    }
  }

  for(Idx = 0U; ((Idx < DoIP_RootPtr->UdpAnnConMax) && (Found == FALSE)); Idx++)
  {
    if(SoAd_GetSoConId(DOIP_PBCFG_UDPANNCON(Idx).SoAdTxPduId, &SoConId_read) == E_OK)
    {
      if(SoConId == SoConId_read)
      {
        UpdateHostnameOption = DOIP_PBCFG_UDPANNCON(Idx).UpdateHostName;
        Found = TRUE;
      }
    }
  }

  for(Idx = 0U; ((Idx < DoIP_RootPtr->TcpConMax) && (Found == FALSE)); Idx++)
  {
    if(SoAd_GetSoConId(DOIP_PBCFG_TCPCON(Idx).SoAdTxPduId, &SoConId_read) == E_OK)
    {
      if(SoConId == SoConId_read)
      {
        UpdateHostnameOption = DOIP_PBCFG_TCPCON(Idx).UpdateHostName;
        Found = TRUE;
      }
    }
  }

  DBG_DOIP_GETUPDATEHOSTNAMEOPTION_EXIT(SoConId);

  return UpdateHostnameOption;
}

STATIC FUNC(void, DOIP_CODE) DoIP_CloseAllCon(void)
{
  DoIP_TcpConIdxType Idx;
  SoAd_SoConIdType SoConId;

  DBG_DOIP_CLOSEALLCON_ENTRY();

  /* Open/close all configured UdpAnnouncement, Udp and Tcp connections. */
  for(Idx = 0U; Idx < DoIP_RootPtr->UdpConMax; Idx++)
  {
    if(SoAd_GetSoConId(DOIP_PBCFG_UDPCON(Idx).SoAdTxPduId, &SoConId) == E_OK)
    {
      (void) SoAd_CloseSoCon(SoConId, TRUE);
    }
  }

  for(Idx = 0U; Idx < DoIP_RootPtr->UdpAnnConMax; Idx++)
  {
    if(SoAd_GetSoConId(DOIP_PBCFG_UDPANNCON(Idx).SoAdTxPduId, &SoConId) == E_OK)
    {
      (void) SoAd_CloseSoCon(SoConId, TRUE);
    }
  }

  for(Idx = 0U; Idx < DoIP_RootPtr->TcpConMax; Idx++)
  {
    if(SoAd_GetSoConId(DOIP_PBCFG_TCPCON(Idx).SoAdTxPduId, &SoConId) == E_OK)
    {
      (void) SoAd_CloseSoCon(SoConId, TRUE);
    }
  }

  DBG_DOIP_CLOSEALLCON_ENTRY();
}

STATIC FUNC(void, DOIP_CODE) DoIP_OpenAllCon(void)
{
  DoIP_TcpConIdxType Idx;
  SoAd_SoConIdType SoConId;

  DBG_DOIP_OPENALLCON_ENTRY();

  /* Open/close all configured UdpAnnouncement, Udp and Tcp connections. */
  for(Idx = 0U; Idx < DoIP_RootPtr->UdpConMax; Idx++)
  {
    if(SoAd_GetSoConId(DOIP_PBCFG_UDPCON(Idx).SoAdTxPduId, &SoConId) == E_OK)
    {
      (void) SoAd_OpenSoCon(SoConId);
    }
  }

  for(Idx = 0U; Idx < DoIP_RootPtr->UdpAnnConMax; Idx++)
  {
    if(SoAd_GetSoConId(DOIP_PBCFG_UDPANNCON(Idx).SoAdTxPduId, &SoConId) == E_OK)
    {
      (void) SoAd_OpenSoCon(SoConId);
    }
  }

  for(Idx = 0U; Idx < DoIP_RootPtr->TcpConMax; Idx++)
  {
    if(SoAd_GetSoConId(DOIP_PBCFG_TCPCON(Idx).SoAdTxPduId, &SoConId) == E_OK)
    {
      (void) SoAd_OpenSoCon(SoConId);
    }
  }

  DBG_DOIP_OPENALLCON_EXIT();

}

FUNC(uint16, DOIP_CODE) DoIP_Make_uint16(uint8 Byte1_Msb, uint8 Byte0_Lsb)
{
  const uint16 Ret = (uint16)((uint16) Byte1_Msb << 8U) | (uint16) Byte0_Lsb;

  DBG_DOIP_MAKE_UINT16_ENTRY(Byte1_Msb, Byte0_Lsb);

  DBG_DOIP_MAKE_UINT16_EXIT(Ret, Byte1_Msb, Byte0_Lsb);
  return Ret;
}

STATIC FUNC(uint32, DOIP_CODE) DoIP_Make_uint32
(
  uint8 Byte3_Msb,
  uint8 Byte2,
  uint8 Byte1,
  uint8 Byte0_Lsb
)
{
  const uint32 Ret = (uint32) (
                                ((uint32) Byte3_Msb << 24U)
                                | ((uint32) Byte2 << 16U)
                                | ((uint32) Byte1 << 8U)
                                | (uint32) Byte0_Lsb
                              );
  DBG_DOIP_MAKE_UINT32_ENTRY(Byte3_Msb, Byte2, Byte1, Byte0_Lsb);

  DBG_DOIP_MAKE_UINT32_EXIT(Ret, Byte3_Msb, Byte2, Byte1, Byte0_Lsb);
  return Ret;
}

STATIC FUNC(void, DOIP_CODE) DoIP_UdpReleaseRemoteAddr
(
  PduIdType DoIPTxPduId
)
{
  SoAd_SoConIdType SoConId;

  /* Release remote address of UDP connection.*/
  if(SoAd_GetSoConId(DOIP_PBCFG_UDPCON(DoIPTxPduId).SoAdTxPduId, &SoConId) == E_OK)
  {
    SoAd_ReleaseRemoteAddr(SoConId);
  }
}

#if (DOIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)
STATIC FUNC(void, DOIP_CODE) DoIP_IncMeasurementDropDataCounter
(
  DoIP_MeasurementIdxType MeasurementIdx
)
{
  SchM_Enter_DoIP_SCHM_DOIP_EXCLUSIVE_AREA_0();

  switch(MeasurementIdx)
  {
    case DOIP_MEAS_DROP_GENHDR:
    {
      /* !LINKSTO DoIP.dsn.Func0126.EB.DoIPGetAndResetMeasurementData_NoOverrun, 1 */
      if(DoIP_MeasurementDropData_GenHdr < DOIP_MAX_MEAS_COUNTER)
      {
        DoIP_MeasurementDropData_GenHdr++;
      }
      break;
    }
    case DOIP_MEAS_DROP_DIAGMSG:
    {
      /* !LINKSTO DoIP.dsn.Func0126.EB.DoIPGetAndResetMeasurementData_NoOverrun, 1 */
      if(DoIP_MeasurementDropData_DiagMsg < DOIP_MAX_MEAS_COUNTER)
      {
        DoIP_MeasurementDropData_DiagMsg++;
      }
      break;
    }
    /* CHECK: NOPARSE */
    default:
    {
      DOIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    }
    /* CHECK: PARSE */
  }
  SchM_Exit_DoIP_SCHM_DOIP_EXCLUSIVE_AREA_0();

}
#endif /* DOIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */

#define DOIP_STOP_SEC_CODE
#include <DoIP_MemMap.h>

/*==================[checks]================================================*/

/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef DOIP_VENDOR_ID /* configuration check */
#error DOIP_VENDOR_ID must be defined
#endif

#if (DOIP_VENDOR_ID != 1U) /* vendor check */
#error DOIP_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef DOIP_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error DOIP_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (DOIP_AR_RELEASE_MAJOR_VERSION != 4U)
#error DOIP_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef DOIP_AR_RELEASE_MINOR_VERSION /* configuration check */
#error DOIP_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (DOIP_AR_RELEASE_MINOR_VERSION != 1U )
#error DOIP_AR_RELEASE_MINOR_VERSION wrong (!= 1U)
#endif

#ifndef DOIP_AR_RELEASE_REVISION_VERSION /* configuration check */
#error DOIP_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (DOIP_AR_RELEASE_REVISION_VERSION != 3U )
#error DOIP_AR_RELEASE_REVISION_VERSION wrong (!= 3U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef DOIP_SW_MAJOR_VERSION /* configuration check */
#error DOIP_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (DOIP_SW_MAJOR_VERSION != 1U)
#error DOIP_SW_MAJOR_VERSION wrong (!= 1U)
#endif

#ifndef DOIP_SW_MINOR_VERSION /* configuration check */
#error DOIP_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (DOIP_SW_MINOR_VERSION < 1U)
#error DOIP_SW_MINOR_VERSION wrong (< 1U)
#endif

#ifndef DOIP_SW_PATCH_VERSION /* configuration check */
#error DOIP_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (DOIP_SW_PATCH_VERSION < 21U)
#error DOIP_SW_PATCH_VERSION wrong (< 21U)
#endif

/*==================[end of file]===========================================*/

