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

/*======================[Include Section]====================================*/

#include <TSMem.h>      /* Memory copy operations */
#include <PduR_FrTp.h>  /* get PduR interface for FrTp */
#include <FrIf.h>       /* FrIf interface file */
#include <FrTp_guards.h> /* After foreign/API/Cfg, before internal/private */
#include <FrTp_Trace.h>
#include <FrTp_Priv.h>  /* data types */

/*======================[Local Macros]=======================================*/

/*======================[Local Function prototypes]==========================*/

/*
 * Start code section declaration
 */
#define FRTP_START_SEC_CODE
#include <FrTp_MemMap.h>

STATIC FUNC(void,FRTP_CODE) FrTp_RxSm_RxData_HandleStartFrame
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(uint8,AUTOMATIC) SN,
    P2CONST(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) pPduInfo
);

STATIC FUNC(void,FRTP_CODE) FrTp_RxSm_TriggerTransmit_HandleRxState
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    P2VAR(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) pPduInfo,
    P2CONST(FrTp_ConnectionType,AUTOMATIC,FRTP_APPL_CONST) pConnection,
    VAR(uint8,AUTOMATIC) RxState
);

STATIC FUNC(void,FRTP_CODE) FrTp_RxSm_RxData_HandleSFBufOk
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(uint8,AUTOMATIC) SN,
    P2CONST(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) pPduInfo
);

STATIC FUNC(void,FRTP_CODE) FrTp_RxSm_RxData_HandleSFBufBusy
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(uint8,AUTOMATIC) SN,
    P2CONST(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) pPduInfo
);

STATIC FUNC(void,FRTP_CODE) FrTp_RxSm_RxData_HandleSFBufNOk
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(uint8,AUTOMATIC) SN,
    P2CONST(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) pPduInfo,
    VAR(BufReq_ReturnType,AUTOMATIC) RetCode,
    VAR(boolean,AUTOMATIC) STFRetransmitFlag
);

STATIC FUNC(void,FRTP_CODE) FrTp_RxSm_RxData_HandleConFrame
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(uint8,AUTOMATIC) PCI,
    VAR(uint8,AUTOMATIC) SN,
    P2CONST(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) pPduInfo
);

STATIC FUNC(void,FRTP_CODE) FrTp_RxSm_RxData_HandleLastFrame
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    P2CONST(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) pPduInfo
);

/**
 * \brief Resets all state variables for the given RX channel.
 */
STATIC FUNC(void,FRTP_CODE) FrTp_ResetRxChannel
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel
);

/** \brief Indicate that reception can be continued
 *
 * Send CTS/WAIT or ACK (acknowledged) or just finish silently (unacknowledged)
 * */
STATIC FUNC(void,FRTP_CODE) FrTp_SendFc_Continue
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    uint8 SN,
    uint8 fpl,
    uint16 ml
);


/** \brief Indicate that PduR returned overflow
 *
 * Send OVFL or abort silently (unsegmented, unacknowledged)
 * */
STATIC FUNC(void,FRTP_CODE) FrTp_SendFc_Ovflw
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    uint8 SN,
    uint8 fpl,
    uint16 ml
);


/** \brief Indicate that an error occurred which cannot be recovered
 *
 * Send ABORT or abort silently (unsegmented, unacknowledged)
 * */
STATIC FUNC(void,FRTP_CODE) FrTp_SendFc_Abort
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    uint8 SN,
    uint8 fpl,
    uint16 ml,
    boolean IndicatePduR
);


#if (FRTP_COPY_STF_LOCALBUFFER == STD_ON)
/** \brief Copy data in STF to the local buffer */
STATIC FUNC(void,FRTP_CODE) FrTp_CopyToLocalBuffer_STF
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    uint8 fpl,
    P2CONST(uint8,AUTOMATIC,FRTP_APPL_DATA) pData
);

STATIC FUNC(BufReq_ReturnType,FRTP_CODE) FrTp_RxSm_PduRCopyData
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel
);
#else

/** \brief Indicate that data reception was unsuccessful and request retry if supported
 *
 * Send RETRY (acknowledged) or abort with OVFLW or silently (unacknowledged)
 * */
STATIC FUNC(void,FRTP_CODE) FrTp_SendFc_Retry
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    uint8 SN,
    uint8 fpl,
    uint16 ml
);
#endif


/** \brief Set up timer to respect TimeBR before transmission of RETRY frame
 *
 * \param[in] pChannel     pointer to channel data structure
 * \param[out] pRxSyncFlag flag to indicate if frame transmission shall be requested
*/
STATIC FUNC(void, FRTP_CODE) FrTp_RxSm_TransmitSyncPoint_TimerForStfRetry
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    P2VAR(boolean,AUTOMATIC,AUTOMATIC) pRxSyncFlag
);

/** \brief Check if RETRY frame shall be sent
 *
 * \param[in] pChannel     pointer to channel data structure
 * \param[out] pRxSyncFlag flag to indicate if frame transmission shall be requested
 * \param[out] pPduInfo    if frame transmission shall be requested pPduInfo->SduLength will be set
*/
STATIC FUNC(void, FRTP_CODE) FrTp_RxSm_TransmitSyncPoint_Retry
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    P2VAR(boolean,AUTOMATIC,AUTOMATIC) pRxSyncFlag,
    P2VAR(PduInfoType,AUTOMATIC,AUTOMATIC) pPduInfo
);

/** \brief Check if reception can go on and send CTS or perform wait procedure
 *
 * \param[in] pChannel     pointer to channel data structure
 * \param[out] pRxSyncFlag flag to indicate if frame transmission shall be requested
 * \param[out] pPduInfo    if frame transmission shall be requested pPduInfo->SduLength will be set
*/
STATIC FUNC(void, FRTP_CODE) FrTp_RxSm_TransmitSyncPoint_CheckIfClearToSend
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    P2VAR(boolean,AUTOMATIC,AUTOMATIC) pRxSyncFlag,
    P2VAR(PduInfoType,AUTOMATIC,AUTOMATIC) pPduInfo
);

/** \brief Handle wait situations when reception cannot go on because no buffer is available
 *
 * \param[in] pChannel     pointer to channel data structure
 * \param[out] pRxSyncFlag flag to indicate if frame transmission shall be requested
 * \param[out] pPduInfo    if frame transmission shall be requested pPduInfo->SduLength will be set
*/
STATIC FUNC(void, FRTP_CODE) FrTp_RxSm_TransmitSyncPoint_Wait
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    P2VAR(boolean,AUTOMATIC,AUTOMATIC) pRxSyncFlag,
    P2VAR(PduInfoType,AUTOMATIC,AUTOMATIC) pPduInfo
);

/** \brief Send FC ACK to acknowledge a successful reception
 *
 * \param[out] pPduInfo    if frame transmission shall be requested pPduInfo->SduLength will be set
*/
STATIC FUNC(void, FRTP_CODE) FrTp_RxSm_TransmitSyncPoint_AcknowledgeReception
(
    P2VAR(PduInfoType,AUTOMATIC,AUTOMATIC) pPduInfo
);

/** \brief Send FC frame to abort reception
 *
 * \param[out] pPduInfo    if frame transmission shall be requested pPduInfo->SduLength will be set
*/
STATIC FUNC(void, FRTP_CODE) FrTp_RxSm_TransmitSyncPoint_SendErrorFC
(
    P2VAR(PduInfoType,AUTOMATIC,AUTOMATIC) pPduInfo
);

/** \brief
 *
 * \param[out] pRxSyncFlag flag to indicate if frame transmission shall be requested
*/
STATIC FUNC(void, FRTP_CODE) FrTp_RxSm_TransmitSyncPoint_NoTransmission
(
    P2VAR(boolean,AUTOMATIC,AUTOMATIC) pRxSyncFlag
);

/*
 * Stop code section declaration
 */
#define FRTP_STOP_SEC_CODE
#include <FrTp_MemMap.h>

/*
 * Start code section declaration
 */
#define FRTP_START_SEC_CODE
#include <FrTp_MemMap.h>

/*=====================[Function Definitions]================================*/

FUNC(boolean,FRTP_CODE) FrTp_RxSm_TriggerTransmit
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    P2VAR(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) pPduInfo
)
{
    /* local variable for atomic access */
    VAR(uint8,AUTOMATIC) RxState;
    /* flag indicating transmission pending */
    VAR(boolean,AUTOMATIC) IsTxPending;
    boolean ReturnVal = FALSE;

    /* get Rx-statemachine full state into local variable */
    VAR(uint8,AUTOMATIC) FullState;

    TS_AtomicAssign8(FullState, pChannel->Rx_State);

    /* get Rx-statemachine state into local variable */
    RxState = FrTp_Chnl_GetRxState(FullState);

    /* Has the Rx statemachine FC transmission pending ? */
    IsTxPending = FrTp_Chnl_IsTxPending(FullState);

    FRTP_PRECONDITION_ASSERT(
        (pChannel->ConnectionIdx != FRTP_CONNECTION_INVALID) && (IsTxPending != FALSE),
        FRTP_TRIGGERTRANSMIT_SERVICE_ID);

    /* only if connection is allocated transmission is pending */
    /* CHECK: NOPARSE */
    if((pChannel->ConnectionIdx != FRTP_CONNECTION_INVALID) && (IsTxPending != FALSE))
    /* CHECK: PARSE */
    {
        /* get pointer to associated connection object */
        CONSTP2CONST(FrTp_ConnectionType,AUTOMATIC,FRTP_APPL_CONST) pConnection =
                FRTP_CFG_GET_PTR_TO_CONNECTION(pChannel->ConnectionIdx);
        ReturnVal = TRUE;

        /* set address information into tx-buffer */
        FrTp_SetAI(pPduInfo->SduDataPtr,pConnection->RemoteAddress, pConnection->LocalAddress);

        FrTp_RxSm_TriggerTransmit_HandleRxState(pChannel, pPduInfo, pConnection, RxState);
    }

    return ReturnVal;
}

FUNC(void,FRTP_CODE) FrTp_RxSm_TransmitConfirmation
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(uint8,AUTOMATIC) TxPduIdx
)
{
    /* local variable for atomic access */
    VAR(uint8,AUTOMATIC) RxState;
    /* flag indicating transmission pending */
    VAR(boolean,AUTOMATIC) IsTxPending;
    /* get Rx-statemachine full state into local variable */
    VAR(uint8,AUTOMATIC) FullState;

    TS_AtomicAssign8(FullState, pChannel->Rx_State);

    /* get Rx-statemachine state into local variable */
    RxState = FrTp_Chnl_GetRxSubState(FullState);

    /* Has the Rx statemachine FC transmission pending ? */
    IsTxPending = FrTp_Chnl_IsTxPending(FullState);

    /* only if connection is allocated transmission is pending */
    FRTP_PRECONDITION_ASSERT(
        (pChannel->ConnectionIdx != FRTP_CONNECTION_INVALID) && (IsTxPending != FALSE),
        FRTP_TXCONFIRMATION_SERVICE_ID);
    /* CHECK: NOPARSE */
    if((pChannel->ConnectionIdx != FRTP_CONNECTION_INVALID) && (IsTxPending != FALSE) )
    /* CHECK: PARSE */
    {
        /* get pointer to associated connection object */
        CONSTP2CONST(FrTp_ConnectionType,AUTOMATIC,FRTP_APPL_CONST) pConnection =
                FRTP_CFG_GET_PTR_TO_CONNECTION(pChannel->ConnectionIdx);

        FrTp_StopTimer(pChannel->Rx_Timer1);

        switch(RxState)
        {
            case FRTP_CHNL_RX_ACK:
            {
                /* received complete frame */
                PduR_FrTpRxIndication(pConnection->RxSduIdx,NTFRSLT_OK);

                /* free channel for new receptions */
                FrTp_ResetRxChannel(pChannel);

                break;
            }
            case FRTP_CHNL_RX_CTS: /* fall through */
            case FRTP_CHNL_RX_RETRY:
            {
                /* start timer CR */
                FrTp_StartTimer(pChannel->Rx_Timer2,pConnection->TimeroutCR);

                /* process further received data */
                TS_AtomicAssign8(pChannel->Rx_State, FRTP_CHNL_RX_DATA);

                break;
            }
            case FRTP_CHNL_RX_WAIT:
            {
                TS_AtomicAssign8(pChannel->Rx_State, FRTP_CHNL_RX_CTS);

                break;
            }
            case FRTP_CHNL_RX_ABORT_WRONG_SN:
            {
                /* received complete frame */
                PduR_FrTpRxIndication(pConnection->RxSduIdx,NTFRSLT_E_WRONG_SN);

                /* free channel for new receptions */
                FrTp_ResetRxChannel(pChannel);

                break;
            }
            case FRTP_CHNL_RX_ABORT_ML_MISMATCH:
            {
                /* received complete frame */
                PduR_FrTpRxIndication(pConnection->RxSduIdx,NTFRSLT_E_FR_ML_MISMATCH);

                /* free channel for new receptions */
                FrTp_ResetRxChannel(pChannel);

                break;
            }
            case FRTP_CHNL_RX_ABORT_NO_BUFFER:
            {
                /* received complete frame */
                PduR_FrTpRxIndication(pConnection->RxSduIdx,NTFRSLT_E_NOT_OK);

                /* free channel for new receptions */
                FrTp_ResetRxChannel(pChannel);

                break;
            }
            case FRTP_CHNL_RX_ABORT: /* fall through */
            case FRTP_CHNL_RX_OVFL: /* fall through */
            {
                /* free channel for new receptions */
                FrTp_ResetRxChannel(pChannel);

                break;
            }
            /* CHECK: NOPARSE */
            case FRTP_CHNL_RX_DATA: /* fall through */
            case FRTP_CHNL_RX_IDLE: /* fall through */
                /* Module will not go through these states in this context, so this function will not
                   be called in these states. */
            default:
                FRTP_UNREACHABLE_CODE_ASSERT(FRTP_TXCONFIRMATION_SERVICE_ID);
            break;
            /* CHECK: PARSE */
        }
    }

    /* perform Tx-Pdu deallocation */
    FrTp_RuntimeTxPdu[TxPduIdx].pChannel = NULL_PTR;

}



/** \brief Performs FrIf_Transmit() call for transmission of a FC frame
 *
 * Send RETRY (acknowledged) or abort with OVFLW or silently (unacknowledged)
 * */
STATIC FUNC(void,FRTP_CODE) FrTp_RxSm_FC_Transmit
(
  P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
  P2VAR(PduInfoType,AUTOMATIC,AUTOMATIC) pPduInfo
);

STATIC FUNC(void,FRTP_CODE) FrTp_RxSm_FC_Transmit
(
  P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
  P2VAR(PduInfoType,AUTOMATIC,AUTOMATIC) pPduInfo
)
{
    /* get pointer to associated connection object */
    CONSTP2CONST(FrTp_ConnectionType,AUTOMATIC,FRTP_APPL_CONST) pConnection =
            FRTP_CFG_GET_PTR_TO_CONNECTION(pChannel->ConnectionIdx);

    const uint8 TxPduIdx = FrTp_GetFreeTxPdu(pChannel,pPduInfo->SduLength);

    if(TxPduIdx != FRTP_TXPDU_INVALID)
    {
        /* get TxPdu information */
        CONSTP2CONST(FrTp_TxPduConfigType,AUTOMATIC,FRTP_APPL_CONST) pTxPdu =
                FRTP_CFG_GET_PTR_TO_TXPDU(TxPduIdx);

        const Std_ReturnType retCode = FrIf_Transmit(pTxPdu->PduId, pPduInfo);

        if(retCode == E_OK)
        {
            /* enter exclusive area */
            SchM_Enter_FrTp_SCHM_FRTP_EXCLUSIVE_AREA_0();

            /* perform Tx-Pdu allocation */
            FrTp_RuntimeTxPdu[TxPduIdx].pChannel = pChannel;

            /* this PDU is used for data transmission */
            FrTp_RuntimeTxPdu[TxPduIdx].PduType = FRTP_TXPDUTYPE_FC;

            /* start timer AR */
            FrTp_StartTimer(pChannel->Rx_Timer1,pConnection->TimeroutAR);

            /* mark a transmission as pending */
            pChannel->Rx_State |= FRTP_CHNL_RX_WAITTX_MASK;

            /* reset FrIf retry counter */
            pChannel->TxFc_FrIfRetry = 0U;

            /* exit exclusive area */
            SchM_Exit_FrTp_SCHM_FRTP_EXCLUSIVE_AREA_0();
        }
        else
        {
            pChannel->TxFc_FrIfRetry++;
            if(pChannel->TxFc_FrIfRetry == pConnection->MaxFrIfRetry)
            {
              /* reception is already finished on for this connection */
              PduR_FrTpRxIndication(pConnection->RxSduIdx, NTFRSLT_E_NOT_OK);
              /* free channel */
              FrTp_ResetRxChannel(pChannel);
            }
        }
    }
}

FUNC(void,FRTP_CODE) FrTp_RxSm_TransmitSyncPoint
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel
)
{
    /* local variable for atomic access */
    VAR(uint8,AUTOMATIC) RxState;
    /* flag indicating transmission pending */
    VAR(boolean,AUTOMATIC) IsTxPending;
    /* flag to indicate if a flow control frame shall be transmitted */
    boolean RxSyncFlag = TRUE;

    /* get Rx-statemachine full state into local variable */
    VAR(uint8,AUTOMATIC) FullState;

    TS_AtomicAssign8(FullState, pChannel->Rx_State);

    /* get Rx-statemachine state into local variable */
    RxState = FrTp_Chnl_GetRxState(FullState);

    /* Has the Rx statemachine FC transmission pending ? */
    IsTxPending = FrTp_Chnl_IsTxPending(FullState);

    /* only if connection is allocated and no FC transmission is in progress */
    if((pChannel->ConnectionIdx != FRTP_CONNECTION_INVALID) &&
       (RxState != FRTP_CHNL_RX_IDLE) &&
       (IsTxPending == FALSE)
      )
    {
        /* PduInfo for the flow control frame that will be transmitted if RxSyncFlag=TRUE */
        PduInfoType PduInfo;
        PduInfo.SduLength = 0U;
        PduInfo.SduDataPtr = NULL_PTR;

        switch(RxState)
        {
            case FRTP_CHNL_RX_STF_RETRY:
            {
              /* RETRY for STF required -> first respect TimeBR then transmit frame */
              FrTp_RxSm_TransmitSyncPoint_TimerForStfRetry(pChannel, &RxSyncFlag);
              break;
            }
            case FRTP_CHNL_RX_RETRY:
            {
                /* wait until TimeBR elapses before FC RETRY is sent */
                FrTp_RxSm_TransmitSyncPoint_Retry(pChannel, &RxSyncFlag, &PduInfo);
                break;
            }
            case FRTP_CHNL_RX_CTS:
            {
                /* check if received data was successfully copied and send
                   CTS or start wait procedure */
                FrTp_RxSm_TransmitSyncPoint_CheckIfClearToSend(pChannel, &RxSyncFlag, &PduInfo);
                break;
            }
            case FRTP_CHNL_RX_WAIT:
            {
                /* check if channel can leave the wait state
                   - either because PduR buffer is available
                   - or because of a timeout */
                FrTp_RxSm_TransmitSyncPoint_Wait(pChannel, &RxSyncFlag, &PduInfo);
                break;
            }
            case FRTP_CHNL_RX_ACK:
            {
                /* send FC ACK to acknowledge successful reception */
                FrTp_RxSm_TransmitSyncPoint_AcknowledgeReception(&PduInfo);
                break;
            }
            case FRTP_CHNL_RX_OVFL: /* fall through */
            case FRTP_CHNL_RX_ABORT:
            {
                /* send FC to abort reception */
                FrTp_RxSm_TransmitSyncPoint_SendErrorFC(&PduInfo);
                break;
            }
            case FRTP_CHNL_RX_DATA: /* fall through */
            case FRTP_CHNL_RX_IDLE: /* fall through */
            default:
            {
                /* prevent transmission of FC frame */
                FrTp_RxSm_TransmitSyncPoint_NoTransmission(&RxSyncFlag);
                break;
            }
        }
        /* Checking RxSyncFlag for transmission of FC */
        if (RxSyncFlag == TRUE)
        {
            FrTp_RxSm_FC_Transmit(pChannel, &PduInfo);
        }
    }

}

STATIC FUNC(void, FRTP_CODE) FrTp_RxSm_TransmitSyncPoint_TimerForStfRetry
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    P2VAR(boolean,AUTOMATIC,AUTOMATIC) pRxSyncFlag
)
{
    /* get pointer to associated connection object */
    CONSTP2CONST(FrTp_ConnectionType,AUTOMATIC,FRTP_APPL_CONST) pConnection =
            FRTP_CFG_GET_PTR_TO_CONNECTION(pChannel->ConnectionIdx);

    /* RETRY for STF shall be transmitted after TimeBR
       -> Start TimeBR and wait to send the FC RETRY. */
    FrTp_StartTimer(pChannel->Rx_Timer3,pConnection->TimeBR);
    TS_AtomicAssign8(pChannel->Rx_State, FRTP_CHNL_RX_RETRY);
    *pRxSyncFlag = FALSE;
}

STATIC FUNC(void, FRTP_CODE) FrTp_RxSm_TransmitSyncPoint_CheckIfClearToSend
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    P2VAR(boolean,AUTOMATIC,AUTOMATIC) pRxSyncFlag,
    P2VAR(PduInfoType,AUTOMATIC,AUTOMATIC) pPduInfo
)
{

    if((pChannel->Rx_BfS != 0U) && (pChannel->Rx_CtrDataAck != 0U))
    {
        /* Previously received data was copied and Rx buffer is available - send CTS */
        pPduInfo->SduLength = FrTp_Size_Header_FC_CTS;
    }
    else
    {
        /* get pointer to associated connection object */
        CONSTP2CONST(FrTp_ConnectionType,AUTOMATIC,FRTP_APPL_CONST) pConnection =
                FRTP_CFG_GET_PTR_TO_CONNECTION(pChannel->ConnectionIdx);

        /* No rx buffer is available and previously received STF has not been copied -
           perform WAIT procedure */
        FrTp_StartTimer(pChannel->Rx_Timer3,pConnection->TimeBR);
        TS_AtomicAssign8(pChannel->Rx_State, FRTP_CHNL_RX_WAIT);
        FrTp_RxSm_TransmitSyncPoint_Wait(pChannel, pRxSyncFlag, pPduInfo);
    }

}

STATIC FUNC(void, FRTP_CODE) FrTp_RxSm_TransmitSyncPoint_Retry
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    P2VAR(boolean,AUTOMATIC,AUTOMATIC) pRxSyncFlag,
    P2VAR(PduInfoType,AUTOMATIC,AUTOMATIC) pPduInfo
)
{
    /* get pointer to associated connection object */
    CONSTP2CONST(FrTp_ConnectionType,AUTOMATIC,FRTP_APPL_CONST) pConnection =
            FRTP_CFG_GET_PTR_TO_CONNECTION(pChannel->ConnectionIdx);

    /* invert CF type */
    pChannel->Rx_PCI ^= FRTP_PCITYPE_CFI_MASK;

    /* reset to 15, since it will be incremented before reception */
    pChannel->Rx_PCI |= FRTP_PCITYPE_SN_MASK;

    if((pChannel->Rx_CtrDataAck == 0U) && (pChannel->Rx_Timer3 != 0U))
    {
        /* this is a retry for an STF frame
           -> wait until TimeBR elapses before sending the RETRY frame */
        *pRxSyncFlag = FALSE;
    }
    else if(pChannel->Rx_CtrRetry < pConnection->MaxRetry)
    {
        /* prepare to send the RETRY frame */
        pChannel->Rx_CtrRetry++;
        pPduInfo->SduLength = FrTp_Size_Header_FC_ACK_RET;
    }
    else
    {
        /* max number of RETRY frames was exceeded
           -> prepare to send ABORT and abort connection */
        TS_AtomicAssign8(pChannel->Rx_State, FRTP_CHNL_RX_ABORT_WRONG_SN);
        pPduInfo->SduLength = FrTp_Size_Header_FC;
    }
}

STATIC FUNC(void, FRTP_CODE) FrTp_RxSm_TransmitSyncPoint_AcknowledgeReception
(
    P2VAR(PduInfoType,AUTOMATIC,AUTOMATIC) pPduInfo
)
{
    pPduInfo->SduLength = FrTp_Size_Header_FC_ACK_RET;
}

STATIC FUNC(void, FRTP_CODE) FrTp_RxSm_TransmitSyncPoint_SendErrorFC
(
    P2VAR(PduInfoType,AUTOMATIC,AUTOMATIC) pPduInfo
)
{
    pPduInfo->SduLength = FrTp_Size_Header_FC;
}

STATIC FUNC(void, FRTP_CODE) FrTp_RxSm_TransmitSyncPoint_NoTransmission
(
    P2VAR(boolean,AUTOMATIC,AUTOMATIC) pRxSyncFlag
)
{
    *pRxSyncFlag = FALSE;
}

#if (FRTP_COPY_STF_LOCALBUFFER == STD_ON)
STATIC FUNC(void, FRTP_CODE) FrTp_RxSm_TransmitSyncPoint_Wait
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    P2VAR(boolean,AUTOMATIC,AUTOMATIC) pRxSyncFlag,
    P2VAR(PduInfoType,AUTOMATIC,AUTOMATIC) pPduInfo
)
{
    /* get pointer to associated connection object */
    CONSTP2CONST(FrTp_ConnectionType,AUTOMATIC,FRTP_APPL_CONST) pConnection =
            FRTP_CFG_GET_PTR_TO_CONNECTION(pChannel->ConnectionIdx);

    PduLengthType Bfs;       /* variable stores the buffer size */
    BufReq_ReturnType retcode;

    const boolean isUnsegmented = ((pChannel->Rx_ML == (uint16)pChannel->STFCopied)?TRUE:FALSE);

    /* initialize info structure */
    pPduInfo->SduLength = 0U;
    pPduInfo->SduDataPtr = NULL_PTR;

    /* an error should not happen here, since no data is copied */
    /* we try to re-request a new rx buffer */
    retcode = PduR_FrTpCopyRxData(pConnection->RxSduIdx, pPduInfo, &Bfs);

    pPduInfo->SduLength = FrTp_Size_Header_FC;

    /* Check if the PduR returned buffersize sufficient for the data copied to the
       local buffer. */
    if((retcode == BUFREQ_OK) && ((Bfs != 0U) && (Bfs >= pChannel->STFCopied)))
    {
#if ( TS_SIZE_PduLengthType > TS_SIZE_uint16 )
        pChannel->Rx_BfS = FrTp_Limit_uint16(Bfs); /* limit Bfs to uint16 maximum */
#else
        pChannel->Rx_BfS = Bfs;
#endif

        retcode = FrTp_RxSm_PduRCopyData(pChannel);

        if(retcode != BUFREQ_OK)
        {
            *pRxSyncFlag = FALSE;
        }
        else
        {
            if((pChannel->RxHasAck != 0U) &&
               (pChannel->Rx_ML == pChannel->Rx_CtrDataAck)
              )
            {
                /* Unsegmented Acknowledged reception - prepare to send ACK. */
                pPduInfo->SduLength = FrTp_Size_Header_FC_ACK_RET;
                TS_AtomicAssign8(pChannel->Rx_State, FRTP_CHNL_RX_ACK);
            }
            else if((pChannel->RxHasAck == 0U) &&(isUnsegmented == TRUE))
            {
                /* Unsegmented Unacknowledged reception is successful */
                PduR_FrTpRxIndication(pConnection->RxSduIdx,NTFRSLT_OK);
                FrTp_ResetRxChannel(pChannel);
                *pRxSyncFlag = FALSE;
            }
            else
            {
                /* Rx buffer is now available for Segmented reception - prepare to send
                    CTS. */
                pPduInfo->SduLength = FrTp_Size_Header_FC_CTS;
                TS_AtomicAssign8(pChannel->Rx_State, FRTP_CHNL_RX_CTS);
            }
            FrTp_StopTimer(pChannel->Rx_Timer3);
        }
    }
    else if(pChannel->Rx_Timer3 == 0U)
    {
        if((pChannel->RxHasAck == 0U) &&
         (pChannel->Rx_ML == pChannel->STFCopied)
        )
        {
          /* Data copy request still not successful */
          PduR_FrTpRxIndication(pConnection->RxSduIdx,NTFRSLT_E_NOT_OK);
          /* free channel for new receptions */
          FrTp_ResetRxChannel(pChannel);

          *pRxSyncFlag = FALSE;
        }
        else if((pConnection->MaxWait != 0U) &&
                (pChannel->Rx_CtrWait < pConnection->MaxWait)
               )
        {
          pChannel->Rx_CtrWait++;
        }
        else
        {
          /* Data copy request still not successful */
          PduR_FrTpRxIndication(pConnection->RxSduIdx,NTFRSLT_E_WFT_OVRN);
          /* free channel for new receptions */
          FrTp_ResetRxChannel(pChannel);

          *pRxSyncFlag = FALSE;
        }
    }
    else
    {
        *pRxSyncFlag = FALSE;
    }
}
#else /* FRTP_COPY_STF_LOCALBUFFER == STD_ON */
STATIC FUNC(void, FRTP_CODE) FrTp_RxSm_TransmitSyncPoint_Wait
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    P2VAR(boolean,AUTOMATIC,AUTOMATIC) pRxSyncFlag,
    P2VAR(PduInfoType,AUTOMATIC,AUTOMATIC) pPduInfo
)
{
    /* get pointer to associated connection object */
    CONSTP2CONST(FrTp_ConnectionType,AUTOMATIC,FRTP_APPL_CONST) pConnection =
            FRTP_CFG_GET_PTR_TO_CONNECTION(pChannel->ConnectionIdx);

    PduLengthType Bfs;       /* variable stores the buffer size */
    BufReq_ReturnType retcode;

    /* initialize info structure */
    pPduInfo->SduLength = 0U;
    pPduInfo->SduDataPtr = NULL_PTR;

    /* an error should not happen here, since no data is copied */
    /* we try to re-request a new rx buffer */
    retcode = PduR_FrTpCopyRxData(pConnection->RxSduIdx, pPduInfo, &Bfs);

    pPduInfo->SduLength = FrTp_Size_Header_FC;

    if((retcode == BUFREQ_OK) && (Bfs != 0U))
    {
#if ( TS_SIZE_PduLengthType > TS_SIZE_uint16 )
        pChannel->Rx_BfS = FrTp_Limit_uint16(Bfs); /* limit Bfs to uint16 maximum */
#else
        pChannel->Rx_BfS = Bfs;
#endif
        /* Rx buffer is now available for Segmented reception - prepare to send
           CTS. */
        pPduInfo->SduLength = FrTp_Size_Header_FC_CTS;
        TS_AtomicAssign8(pChannel->Rx_State, FRTP_CHNL_RX_CTS);

        FrTp_StopTimer(pChannel->Rx_Timer3);

    }
    else if(pChannel->Rx_Timer3 == 0U)
    {
        if((pConnection->MaxWait != 0U) &&
           (pChannel->Rx_CtrWait < pConnection->MaxWait)
           )
        {
          pChannel->Rx_CtrWait++;
        }
        else
        {
          /* Data copy request still not successful */
          PduR_FrTpRxIndication(pConnection->RxSduIdx,NTFRSLT_E_WFT_OVRN);
          /* free channel for new receptions */
          FrTp_ResetRxChannel(pChannel);

          *pRxSyncFlag = FALSE;
        }
    }
    else
    {
        *pRxSyncFlag = FALSE;
    }
}
#endif

FUNC(void,FRTP_CODE) FrTp_RxSm_IndicateAndAbort
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(NotifResultType,AUTOMATIC) result
)
{
    /* get pointer to associated connection object */
    CONSTP2CONST(FrTp_ConnectionType,AUTOMATIC,FRTP_APPL_CONST) pConnection =
            FRTP_CFG_GET_PTR_TO_CONNECTION(pChannel->ConnectionIdx);


    /* received complete frame */
    PduR_FrTpRxIndication(pConnection->RxSduIdx,result);

    /* free channel for new receptions */
    FrTp_ResetRxChannel(pChannel);

}


STATIC FUNC(void,FRTP_CODE) FrTp_RxSm_RxData_HandleStartFrame
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(uint8,AUTOMATIC) SN,
    P2CONST(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) pPduInfo
)
{
    /* flag for distinguishing initial STF frame and STF frame as response to RETRY frame */
    boolean stf_retransmission_flag = FALSE;
    /* local variable for frame payload length */
    VAR(uint8,AUTOMATIC) fpl;
    /* local variable for message length */
    VAR(uint16,AUTOMATIC) ml;
    /* local variable for atomic comparison */
    VAR(uint8,AUTOMATIC) RxState;

    /* get local copy of data pointer */
    CONSTP2CONST(uint8,AUTOMATIC,FRTP_APPL_DATA) pData =
        pPduInfo->SduDataPtr;

    /* get pointer to associated connection object */
    CONSTP2CONST(FrTp_ConnectionType,AUTOMATIC,FRTP_APPL_CONST) pConnection =
            FRTP_CFG_GET_PTR_TO_CONNECTION(pChannel->ConnectionIdx);


    /* get the frame payload length value */
    fpl = FrTp_GetFPL(pData);

    /* get Rx-statemachine state into local variable */
    TS_AtomicAssign8(RxState, pChannel->Rx_State);

    /* get the message length field */
    ml = FrTp_GetML(pData);

    /* check for an already ongoing reception */
    if (FrTp_Chnl_GetRxState(RxState) == FRTP_CHNL_RX_IDLE)
    {
        /* STF received in idle state
         * -> this is the start of a new reception */
        pChannel->Rx_CtrRetry = 0U;      /* reset the retry counter */
    }
    else if (pChannel->Rx_CtrDataAck == 0U)
    {
        /* STF received in reception process but no CTS yet received
         * -> this is a STF which is retransmitting data */
        stf_retransmission_flag = TRUE;
    }
    else
    {
        /* STF frame was received in reception process, but it's not a
         * retransmission of a start frame
         * -> abort the current reception - report an unknown Pdu received */
        PduR_FrTpRxIndication(pConnection->RxSduIdx,NTFRSLT_E_UNEXP_PDU);
    }

    pChannel->Rx_CtrWait = 0U;           /* reset wait counter */
    pChannel->Rx_PCI = 0U;               /* reset CF type */
    pChannel->Rx_CtrDataAck = 0U;        /* reset number of data received */
    pChannel->Rx_CtrBlockReceived = 0U;  /* reset num of data received within current block */
    pChannel->Rx_ML = ml;                /* store total message length to expect */
    pChannel->RxHasAck = SN;             /* acknowledge needed? Note that of STF the SN
                                          * parameter contain the information whether an
                                          * acknowledge is needed (1) or not (0).
                                          */
    pChannel->TxFc_FrIfRetry = 0U;       /* reset FrIf retry counter */
    /* stop the timers (if still running) */
    FrTp_StopTimer(pChannel->Rx_Timer3);
    FrTp_StopTimer(pChannel->Rx_Timer2);
    FrTp_StopTimer(pChannel->Rx_Timer1);

    /* ignore inconsistent frames */
    if( (fpl == 0U) || ((uint16)fpl > ml) || (fpl > FrTp_Max_FPL_STF))
    {
        /* ignore invalid frame */
        FrTp_ResetRxChannel(pChannel);
    }
    else if(((pConnection->TpOptions & FRTP_CONNECTION_ISBROADCAST) != 0U) &&
               ((ml != fpl) || (SN != 0U))
           )
    {
        /* ignore segmented or acknowledged broadcast connection */
        FrTp_ResetRxChannel(pChannel);
    }
    else
    {
        /* FPL is consistent */

        PduLengthType Bfs;  /* variable stores the buffer size */

        /* indicate the start of a  new reception (and obtain the existing buffer size */
        BufReq_ReturnType RetCode;

        if (stf_retransmission_flag == FALSE)
        {
            /* just a regular STF -> start reception */
            RetCode = PduR_FrTpStartOfReception(pConnection->RxSduIdx,pChannel->Rx_ML,&Bfs);
        }
        else
        {
            /* retransmission of STF -> just try to copy data */
            PduInfoType pduInfo = {NULL_PTR, 0U};
            RetCode = PduR_FrTpCopyRxData(pConnection->RxSduIdx, &pduInfo, &Bfs);
        }

        /* we got a buffer reserved */
        if ((RetCode == BUFREQ_OK) && (Bfs>=fpl))
        {
            /* PduR has an available buffer and buffer size is large enough to
             * copy the start frame */
            FrTp_RxSm_RxData_HandleSFBufOk(pChannel, SN, pPduInfo);
        }
        else if ( ((RetCode == BUFREQ_OK) && (Bfs<fpl))
                ||(RetCode == BUFREQ_E_BUSY )
                )
        {
            /* PduR does not have a sufficiently large buffer*/
            FrTp_RxSm_RxData_HandleSFBufBusy(pChannel, SN, pPduInfo);
        }
        else
        {
            /* PduR returned NOT_OK or OVFLW */
            FrTp_RxSm_RxData_HandleSFBufNOk(pChannel, SN, pPduInfo, RetCode,
                                            stf_retransmission_flag
                                           );
        }
    }

}

STATIC FUNC(void,FRTP_CODE) FrTp_RxSm_TriggerTransmit_HandleRxState
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    P2VAR(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) pPduInfo,
    P2CONST(FrTp_ConnectionType,AUTOMATIC,FRTP_APPL_CONST) pConnection,
    VAR(uint8,AUTOMATIC) RxState
)
{
    switch(RxState)
    {
        case FRTP_CHNL_RX_ACK:
        {
            FrTp_SetPCI_ACK(pPduInfo->SduDataPtr);
            pPduInfo->SduLength = FrTp_Size_Header_FC_ACK_RET;
            break;
        }
        case FRTP_CHNL_RX_RETRY:
        {
            FrTp_SetPCI_RET(pPduInfo->SduDataPtr,pChannel->Rx_CtrBlockReceived);
            pPduInfo->SduLength = FrTp_Size_Header_FC_ACK_RET;
            break;
        }
        case FRTP_CHNL_RX_CTS:
        {
            const uint16 RemainingPayload = (uint16)(pChannel->Rx_ML - pChannel->Rx_CtrDataAck);
            if(pChannel->Rx_BfS >= RemainingPayload)
            {
              FrTp_SetPCI_CTS(pPduInfo->SduDataPtr,pConnection->BC,0U);
            }
            else
            {
              FrTp_SetPCI_CTS(pPduInfo->SduDataPtr,pConnection->BC,pChannel->Rx_BfS);
            }
            pPduInfo->SduLength = FrTp_Size_Header_FC_CTS;
            break;
        }
        case FRTP_CHNL_RX_OVFL:
        {
            FrTp_SetPCI_OVFLW(pPduInfo->SduDataPtr);
            pPduInfo->SduLength = FrTp_Size_Header_FC;
            break;
        }
        case FRTP_CHNL_RX_ABORT:
        {
            FrTp_SetPCI_ABT(pPduInfo->SduDataPtr);
            pPduInfo->SduLength = FrTp_Size_Header_FC;
            break;
        }
        case FRTP_CHNL_RX_WAIT:
        {
            FrTp_SetPCI_WAIT(pPduInfo->SduDataPtr);
            pPduInfo->SduLength = FrTp_Size_Header_FC;
            break;
        }
        /* CHECK: NOPARSE */
        case FRTP_CHNL_RX_DATA: /* fall through */
        case FRTP_CHNL_RX_IDLE: /* fall through */
        default:
        {
            /* The module will never go through these states in this context, since the channel
              associated with TxPdu will be a NULL_PTR, this function will not be called. */
            FRTP_UNREACHABLE_CODE_ASSERT(FRTP_TRIGGERTRANSMIT_SERVICE_ID);
            break;
        }
        /* CHECK: PARSE */
    }
}

STATIC FUNC(void,FRTP_CODE) FrTp_RxSm_RxData_HandleSFBufOk
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(uint8,AUTOMATIC) SN,
    P2CONST(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) pPduInfo
)
{
    PduLengthType Bfs;
    /* get local copy of data pointer */
    CONSTP2CONST(uint8,AUTOMATIC,FRTP_APPL_DATA) pData =
        pPduInfo->SduDataPtr;

    /* get the frame payload length value */
    const uint8 fpl = FrTp_GetFPL(pData);

    /* get the message length field */
    const uint16 ml = FrTp_GetML(pData);

    /* get pointer to associated connection object */
    CONSTP2CONST(FrTp_ConnectionType,AUTOMATIC,FRTP_APPL_CONST) pConnection =
            FRTP_CFG_GET_PTR_TO_CONNECTION(pChannel->ConnectionIdx);

    /* buffer is large enough for the startframe */

    PduInfoType PduInfo;    /* temporary for passing parameters */
    BufReq_ReturnType RetCode;


    /* create a Pdu info structure for calling the upper layer */
    PduInfo.SduLength = (PduLengthType)fpl;
    PduInfo.SduDataPtr = FrTp_GetPayload_STF(pPduInfo->SduDataPtr);

    /* copy the payload into the receive buffer */
    /* an error should not happen here, since the buffer was previously guaranteed */
    RetCode = PduR_FrTpCopyRxData(pConnection->RxSduIdx, &PduInfo, &Bfs);

    switch(RetCode)
    {
        case BUFREQ_OK:
        {
            /* set number of received data bytes */
            pChannel->Rx_CtrBlockReceived = (uint16)fpl;
            pChannel->Rx_CtrDataAck = (uint16)fpl;

            /* save actual buffersize available */
#if ( TS_SIZE_PduLengthType > TS_SIZE_uint16 )
            pChannel->Rx_BfS = FrTp_Limit_uint16(Bfs);
#else
            pChannel->Rx_BfS = Bfs;
#endif
            /* copy succeeded -> unsegmented reception is finished now,
            * segmented reception is continued with CTS or WAIT frame */
            FrTp_SendFc_Continue(pChannel, SN, fpl, ml);
            break;
        }
        case BUFREQ_E_BUSY:
        {
#if (FRTP_COPY_STF_LOCALBUFFER == STD_ON)
            /* Copy the STF to local buffer and retry for PduR. */
            FrTp_CopyToLocalBuffer_STF(pChannel, fpl, pData);
#else
            /* should not happen: PduR granted this buffer just before and is now
            * returning busy -> same behavior as StartOfReception returning BUSY */
            FrTp_SendFc_Retry(pChannel, SN, fpl, ml);
#endif
            break;
        }
        case BUFREQ_E_NOT_OK:
        {
            FrTp_SendFc_Abort(pChannel, SN, fpl, ml, TRUE);
            break;
        }
        /*  CHECK: NOPARSE */
        default:
        {
            /* unreachable */
            FRTP_UNREACHABLE_CODE_ASSERT(FRTP_RXINDICATION_SERVICE_ID);
            break;
        }
        /* CHECK: PARSE */
    }

}

STATIC FUNC(void,FRTP_CODE) FrTp_RxSm_RxData_HandleSFBufBusy
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(uint8,AUTOMATIC) SN,
    P2CONST(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) pPduInfo
)
{
    /* get local copy of data pointer */
    CONSTP2CONST(uint8,AUTOMATIC,FRTP_APPL_DATA) pData =
        pPduInfo->SduDataPtr;

    /* get the frame payload length value */
    const uint8 fpl = FrTp_GetFPL(pData);

#if (FRTP_COPY_STF_LOCALBUFFER == STD_OFF)
    /* get the message length field */
    const uint16 ml = FrTp_GetML(pData);
#else
    TS_PARAM_UNUSED(SN);
#endif


#if (FRTP_COPY_STF_LOCALBUFFER == STD_ON)
    /* Copy to local buffer and retry for PduR.*/
    FrTp_CopyToLocalBuffer_STF(pChannel, fpl, pData);
#else
    /* Prepare to send a FC RETRY, copy of STF to local buffer is not supported. */
    FrTp_SendFc_Retry(pChannel, SN, fpl, ml);
#endif

}


STATIC FUNC(void,FRTP_CODE) FrTp_RxSm_RxData_HandleSFBufNOk
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(uint8,AUTOMATIC) SN,
    P2CONST(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) pPduInfo,
    VAR(BufReq_ReturnType,AUTOMATIC) RetCode,
    VAR(boolean,AUTOMATIC) STFRetransmitFlag
)
{
    /* get local copy of data pointer */
    CONSTP2CONST(uint8,AUTOMATIC,FRTP_APPL_DATA) pData =
        pPduInfo->SduDataPtr;

    /* get the frame payload length value */
    const uint8 fpl = FrTp_GetFPL(pData);

    /* get the message length field */
    const uint16 ml = FrTp_GetML(pData);


    switch(RetCode)
    {
      case BUFREQ_E_OVFL:
      {
          FrTp_SendFc_Ovflw(pChannel, SN, fpl, ml);
          break;
      }
      case BUFREQ_E_NOT_OK:
      {
          FrTp_SendFc_Abort(pChannel, SN, fpl, ml, STFRetransmitFlag);
          break;
      }
      /* CHECK: NOPARSE */
      default:
      {
          /* unreachable */
          FRTP_UNREACHABLE_CODE_ASSERT(FRTP_RXINDICATION_SERVICE_ID);
          break;
      }
      /* CHECK: PARSE */
    }
}

STATIC FUNC(void,FRTP_CODE) FrTp_SendFc_Continue
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    uint8 SN,
    uint8 fpl,
    uint16 ml
)
{
    if(fpl == ml)
    {
        /* handle unsegmented transfer */
        if(SN != 0U)
        {
            /* if acknowledge required, request ACK transmission */
            TS_AtomicAssign8(pChannel->Rx_State, FRTP_CHNL_RX_ACK);
        }
        else
        {
              /* get pointer to associated connection object */
              CONSTP2CONST(FrTp_ConnectionType,AUTOMATIC,FRTP_APPL_CONST) pConnection =
                      FRTP_CFG_GET_PTR_TO_CONNECTION(pChannel->ConnectionIdx);

              /* if unacknowledged indicate full reception */
              PduR_FrTpRxIndication(pConnection->RxSduIdx,NTFRSLT_OK);

              /* free channel for new receptions */
              FrTp_ResetRxChannel(pChannel);
        }
    }
    else
    {
        /* Segmented transfer, perform CTS handling */
        TS_AtomicAssign8(pChannel->Rx_State, FRTP_CHNL_RX_CTS);
    }

}


STATIC FUNC(void,FRTP_CODE) FrTp_SendFc_Ovflw
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    uint8 SN,
    uint8 fpl,
    uint16 ml
)
{
    /* acknowledged or segmented transmission */

    if( (SN != 0U) || (fpl != ml))
    {
        /* if acknowledged or segmented transmission, then indicate an overflow */
        TS_AtomicAssign8(pChannel->Rx_State, FRTP_CHNL_RX_OVFL);
    }
    else
    {
        /* otherwise abort reception */
        FrTp_ResetRxChannel(pChannel);
    }

}

STATIC FUNC(void,FRTP_CODE) FrTp_SendFc_Abort
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    uint8 SN,
    uint8 fpl,
    uint16 ml,
    boolean IndicatePduR
)
{
    /* acknowledged or segmented transmission */

    if( (SN != 0U) || (fpl != ml))
    {
        /* if acknowledged or segmented transmission, then abort */
        if(IndicatePduR == TRUE)
        {
            TS_AtomicAssign8(pChannel->Rx_State, FRTP_CHNL_RX_ABORT_NO_BUFFER);
        }
        else
        {
            TS_AtomicAssign8(pChannel->Rx_State, FRTP_CHNL_RX_ABORT);
        }
    }
    else
    {
        /* otherwise abort reception */
        FrTp_ResetRxChannel(pChannel);
    }

}

#if (FRTP_COPY_STF_LOCALBUFFER == STD_ON)
STATIC FUNC(void,FRTP_CODE) FrTp_CopyToLocalBuffer_STF
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    uint8 fpl,
    P2CONST(uint8,AUTOMATIC,FRTP_APPL_DATA) pData
)
{


    /* Copy STF payload to the local buffer and prepare to retry for PduR. */
    TS_MemCpy(pChannel->LocalSTFBuffer, &pData[FrTp_Size_Header_STF], (uint16)fpl);
    pChannel->STFCopied = fpl;


    TS_AtomicAssign8(pChannel->Rx_State, FRTP_CHNL_RX_CTS);

}

STATIC FUNC(BufReq_ReturnType,FRTP_CODE) FrTp_RxSm_PduRCopyData
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel
)
{
    BufReq_ReturnType retval = BUFREQ_OK;
    PduInfoType PduInfo;
    PduLengthType Bfs;
    CONSTP2CONST(FrTp_ConnectionType,AUTOMATIC,FRTP_APPL_CONST) pConnection =
            FRTP_CFG_GET_PTR_TO_CONNECTION(pChannel->ConnectionIdx);

    /* Check if STF is copied to the local buffer, pass it to PduR. */
    if(pChannel->STFCopied != 0U)
    {
        PduInfo.SduLength = pChannel->STFCopied;
        PduInfo.SduDataPtr = pChannel->LocalSTFBuffer;

        /* Error should not happen here, buffer was just granted. */
        retval = PduR_FrTpCopyRxData(pConnection->RxSduIdx, &PduInfo, &Bfs);
        if(retval == BUFREQ_OK)
        {
            pChannel->Rx_CtrBlockReceived = pChannel->STFCopied;
            pChannel->Rx_CtrDataAck = pChannel->STFCopied;
            pChannel->Rx_BfS = Bfs;
            pChannel->STFCopied = 0U;
        }
    }
    return retval;
}
#else

STATIC FUNC(void,FRTP_CODE) FrTp_SendFc_Retry
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    uint8 SN,
    uint8 fpl,
    uint16 ml
)
{
    /* get pointer to associated connection object */
    CONSTP2CONST(FrTp_ConnectionType,AUTOMATIC,FRTP_APPL_CONST) pConnection =
            FRTP_CFG_GET_PTR_TO_CONNECTION(pChannel->ConnectionIdx);


    if(SN != 0U)
    {
        /* if acknowledged transmission (segmented or unsegmented),
         * then request a retry */
        TS_AtomicAssign8(pChannel->Rx_State, FRTP_CHNL_RX_STF_RETRY);
    }
    else if(fpl != ml)
    {
        /* if segmented (unacknowledged) transmission, send abort frame and indicate to PduR */
        TS_AtomicAssign8(pChannel->Rx_State, FRTP_CHNL_RX_ABORT_NO_BUFFER);
    }
    else
    {
        /* otherwise (unacknowledged, unsegmented) just
         * - ignore frame
         * - free channel for new receptions
         * - indicate to PduR */
        FrTp_ResetRxChannel(pChannel);
        PduR_FrTpRxIndication(pConnection->RxSduIdx, NTFRSLT_E_NOT_OK);
    }

}
#endif

STATIC FUNC(void,FRTP_CODE) FrTp_RxSm_RxData_HandleConFrame
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(uint8,AUTOMATIC) PCI,
    VAR(uint8,AUTOMATIC) SN,
    P2CONST(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) pPduInfo
)
{

    /* get local copy of data pointer */
    CONSTP2CONST(uint8,AUTOMATIC,FRTP_APPL_DATA) pData =
        pPduInfo->SduDataPtr;

    /* local variable to store return value of PduR call */
    BufReq_ReturnType retval;

    /* get the frame payload length value */
    const uint8 fpl = FrTp_GetFPL(pData);

    /* local variable for atomic comparison */
    VAR(uint8,AUTOMATIC) RxState;

    /* get pointer to associated connection object */
    CONSTP2CONST(FrTp_ConnectionType,AUTOMATIC,FRTP_APPL_CONST) pConnection =
            FRTP_CFG_GET_PTR_TO_CONNECTION(pChannel->ConnectionIdx);


    /* get Rx-statemachine state into local variable */
    TS_AtomicAssign8(RxState, pChannel->Rx_State);

    /* check if data is currently expected */
    if(FrTp_Chnl_GetRxState(RxState) == FRTP_CHNL_RX_DATA)
    {
        /* is this an expected type of CF ? */
        if( ( (PCI&FRTP_PCITYPE_CF_MASK) ==
              (FRTP_PCITYPE_CF1 + (pChannel->Rx_PCI&FRTP_PCITYPE_CFI_MASK))
            )
          ||(PCI == FRTP_PCITYPE_EOB)
          )
        {
            /* increment sequence number */
            if((pChannel->Rx_PCI&FRTP_PCITYPE_SN_MASK) < FRTP_PCITYPE_SN_MASK)
            {
                pChannel->Rx_PCI++;
            }
            else
            {
                pChannel->Rx_PCI &= ~(uint8)FRTP_PCITYPE_SN_MASK;
            }

            /* check for valid sequence number */
            if((pChannel->Rx_PCI&FRTP_PCITYPE_SN_MASK) != SN)
            {
                /* sequence number is invalid */
                if( pChannel->RxHasAck != 0U)
                {
                    /* if acknowledge is enabled, send a retry message */
                    TS_AtomicAssign8(pChannel->Rx_State, FRTP_CHNL_RX_RETRY);
                }
                else
                {
                    /* if no acknowledge is enabled, send an abort message */
                    TS_AtomicAssign8(pChannel->Rx_State, FRTP_CHNL_RX_ABORT_WRONG_SN);
                }
            }
            else
            {
                /* sequence number is valid */

                PduInfoType PduInfo;     /* temporary for passing parameters */
                PduLengthType Bfs;       /* variable stores the buffer size */

                /* initialize info structure */
                PduInfo.SduLength = (PduLengthType)fpl;
                PduInfo.SduDataPtr = FrTp_GetPayload_CF(pPduInfo->SduDataPtr);

                /* copy the payload into the receive buffer */
                /* an error should not happen here, since the buffer was previously guaranteed */
                retval = PduR_FrTpCopyRxData(pConnection->RxSduIdx, &PduInfo, &Bfs);

                if(retval != BUFREQ_OK)
                {
                    TS_AtomicAssign8(pChannel->Rx_State, FRTP_CHNL_RX_ABORT_NO_BUFFER);
                }
                else
                {
#if ( TS_SIZE_PduLengthType > TS_SIZE_uint16 )
                    pChannel->Rx_BfS = FrTp_Limit_uint16(Bfs); /* limit Bfs to uint16 maximum */
#else
                    pChannel->Rx_BfS = Bfs;
#endif

                    pChannel->Rx_CtrDataAck += (uint16)fpl;         /* sum up total data received */
                    pChannel->Rx_CtrBlockReceived += (uint16)fpl;    /* sum up block data received */

                    /* at end of block request CTS transmission */
                    if(PCI == FRTP_PCITYPE_EOB)
                    {
                        /* EOB CF received */

                        /* stop the CR timer */
                        FrTp_StopTimer(pChannel->Rx_Timer2);

                        /* request a CTS transmission */
                        TS_AtomicAssign8(pChannel->Rx_State, FRTP_CHNL_RX_CTS);

                        /* reset block data counter */
                        pChannel->Rx_CtrBlockReceived = 0U;

                        /* reset wait and retry counter */
                        pChannel->Rx_CtrRetry = 0U;
                        pChannel->Rx_CtrWait = 0U;
                    }
                    else
                    {
                        /* normal CF frame received */

                        /* restart CR timer */
                        FrTp_StartTimer(pChannel->Rx_Timer2,pConnection->TimeroutCR);
                    }
                }
            }
        }
    }

}



STATIC FUNC(void,FRTP_CODE) FrTp_RxSm_RxData_HandleLastFrame
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    P2CONST(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) pPduInfo
)
{
    /* local variable for atomic comparison */
    VAR(uint8,AUTOMATIC) RxState;

    /* local variable to store return value of PduR call */
    BufReq_ReturnType retval;

    /* get local copy of data pointer */
    CONSTP2CONST(uint8,AUTOMATIC,FRTP_APPL_DATA) pData =
        pPduInfo->SduDataPtr;

    /* get the frame payload length value */
    const uint8 fpl = FrTp_GetFPL(pData);

    /* get pointer to associated connection object */
    CONSTP2CONST(FrTp_ConnectionType,AUTOMATIC,FRTP_APPL_CONST) pConnection =
            FRTP_CFG_GET_PTR_TO_CONNECTION(pChannel->ConnectionIdx);


    /* get Rx-statemachine state into local variable */
    TS_AtomicAssign8(RxState, pChannel->Rx_State);

    /* check if data is currently expected */
    if(FrTp_Chnl_GetRxState(RxState) == FRTP_CHNL_RX_DATA)
    {

        /* get the message length field */
        const uint16 ml = FrTp_GetML(pData);

        pChannel->Rx_CtrDataAck += (uint16)fpl; /* sum up received bytes */

        /* check message length consistency */
        if((pChannel->Rx_CtrDataAck != ml) || (pChannel->Rx_CtrDataAck != pChannel->Rx_ML))
        {
            if(pChannel->RxHasAck != 0U)
            {
                /* if acknowledge is enabled, send an abort message */
                TS_AtomicAssign8(pChannel->Rx_State, FRTP_CHNL_RX_ABORT_ML_MISMATCH);
            }
            else
            {
                /* indicate an invalid sequence number */
                PduR_FrTpRxIndication(pConnection->RxSduIdx,NTFRSLT_E_FR_ML_MISMATCH);

                /* free channel for new receptions */
                FrTp_ResetRxChannel(pChannel);
            }
        }
        else
        {
            PduInfoType PduInfo;     /* temporary for passing parameters */
            PduLengthType BfsUnused; /* variable stores the buffer size */

            /* initialize info structure */
            PduInfo.SduLength = (PduLengthType)fpl;
            PduInfo.SduDataPtr = FrTp_GetPayload_LF(pPduInfo->SduDataPtr);

            /* copy the payload into the receive buffer */
            /* an error should not happen here, since the buffer was previously guaranteed */
            retval = PduR_FrTpCopyRxData(pConnection->RxSduIdx, &PduInfo, &BfsUnused);

            if(retval != BUFREQ_OK)
            {
                TS_AtomicAssign8(pChannel->Rx_State, FRTP_CHNL_RX_ABORT_NO_BUFFER);
            }
            else
            {
                /* stop the CR timer */
                FrTp_StopTimer(pChannel->Rx_Timer2);

                if(pChannel->RxHasAck != 0U)
                {
                    /* acknowledge is requested */
                    TS_AtomicAssign8(pChannel->Rx_State, FRTP_CHNL_RX_ACK);
                }
                else
                {
                    /* no acknowledge is requested */

                    /* received complete frame */
                    PduR_FrTpRxIndication(pConnection->RxSduIdx,NTFRSLT_OK);

                    /* free channel for new receptions */
                    FrTp_ResetRxChannel(pChannel);
                }
            }
        }
    }

}


FUNC(void,FRTP_CODE) FrTp_RxSm_RxData
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(uint8,AUTOMATIC) PCI,
    VAR(uint8,AUTOMATIC) SN,
    P2CONST(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) pPduInfo
)
{

#if (FRTP_FULLDUPLEX_ENABLE == STD_OFF)

    /* get Rx-statemachine state into local variable */
    VAR(uint8,AUTOMATIC) TxState;

    TS_AtomicAssign8(TxState, pChannel->Tx_State);

    /* no transmission in progress? */
    if(FrTp_Chnl_GetTxState(TxState) == FRTP_CHNL_TX_IDLE)
#endif
    {
        switch(PCI)
        {
            /* received a startframe */
            case FRTP_PCITYPE_STFU:
            {
                FrTp_RxSm_RxData_HandleStartFrame(pChannel,SN,pPduInfo);
                break;
            }

            /* any of the consecutive frames */
            case FRTP_PCITYPE_CF1: /* fall through */
            case FRTP_PCITYPE_CF2: /* fall through */
            case FRTP_PCITYPE_EOB:
            {
                FrTp_RxSm_RxData_HandleConFrame(pChannel,PCI,SN,pPduInfo);
                break;
            }

            case FRTP_PCITYPE_LF:
            {
                /* check lower bytes are not used */
                if( 0U == SN )
                {
                  FrTp_RxSm_RxData_HandleLastFrame(pChannel,pPduInfo);
                }
                break;
            }
            /* CHECK: NOPARSE */
            default:
            {
                /* unreachable */
                FRTP_UNREACHABLE_CODE_ASSERT(FRTP_RXINDICATION_SERVICE_ID);
                break;
            }
            /* CHECK: PARSE */
        }
    }

}

/**
 * \brief Resets all state variables for the given RX channel.
 */
STATIC FUNC(void,FRTP_CODE) FrTp_ResetRxChannel
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel
)
{
#if (FRTP_LIMIT_NUMBER_CONNECTIONS == STD_ON)
    boolean StartBuffered = FALSE;
#endif /* FRTP_LIMIT_NUMBER_CONNECTIONS */

    /* enter exclusive area */
    SchM_Enter_FrTp_SCHM_FRTP_EXCLUSIVE_AREA_0();

    FrTp_StopTimer((pChannel)->Rx_Timer1);
    FrTp_StopTimer((pChannel)->Rx_Timer2);
    FrTp_StopTimer((pChannel)->Rx_Timer3);
#if (FRTP_FULLDUPLEX_ENABLE == STD_ON)
    if((pChannel)->Tx_State == FRTP_CHNL_TX_IDLE)
#endif
    {
#if (FRTP_LIMIT_NUMBER_CONNECTIONS == STD_ON)
        StartBuffered = TRUE;
    }
#else
        (pChannel)->ConnectionIdx = FRTP_CONNECTION_INVALID;
    }
    (pChannel)->Rx_State = FRTP_CHNL_RX_IDLE;
#endif /* FRTP_LIMIT_NUMBER_CONNECTIONS */

    /* exit exclusive area */
    SchM_Exit_FrTp_SCHM_FRTP_EXCLUSIVE_AREA_0();

#if (FRTP_LIMIT_NUMBER_CONNECTIONS == STD_ON)
    if(StartBuffered)
    {
        Std_ReturnType RetVal = FrTp_StartBufferedConnection(pChannel);

        /* enter exclusive area */
        SchM_Enter_FrTp_SCHM_FRTP_EXCLUSIVE_AREA_0();

        (pChannel)->Rx_State = FRTP_CHNL_RX_IDLE;

        if(RetVal == E_NOT_OK)
        {
            (pChannel)->ConnectionIdx = FRTP_CONNECTION_INVALID;
        }

        /* exit exclusive area */
        SchM_Exit_FrTp_SCHM_FRTP_EXCLUSIVE_AREA_0();
    }
    else
    {
        (pChannel)->Rx_State = FRTP_CHNL_RX_IDLE;
    }
#endif
}

/*
 * Stop code section declaration
 */
#define FRTP_STOP_SEC_CODE
#include <FrTp_MemMap.h>


