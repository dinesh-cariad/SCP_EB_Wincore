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

#include <PduR_FrTp.h>  /* get PduR interface for FrTp */
#include <FrIf.h>       /* FrIf interface file */
#include <FrTp_guards.h> /* After foreign/API/Cfg, before internal/private */
#include <FrTp_Trace.h>
#include <FrTp_Priv.h>  /* get internal macros and function declarations */

/*======================[Local Macros]=======================================*/

/*======================[Local Function prototypes]==========================*/

/*
 * Start code section declaration
 */
#define FRTP_START_SEC_CODE
#include <FrTp_MemMap.h>

STATIC FUNC(boolean,FRTP_CODE) FrTp_TxSm_TransmitSyncPointSF
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel
);

STATIC FUNC(boolean,FRTP_CODE) FrTp_TxSm_TransmitSyncPointCFLF
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel
);

STATIC FUNC(void,FRTP_CODE) FrTp_TxSm_HandleTriggerTxFrame
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    P2VAR(FrTp_RuntimeTxPduType, AUTOMATIC, FRTP_VAR) pTxPdu,
    P2VAR(PduInfoType, AUTOMATIC, FRTP_VAR) DestinationInfo,
    P2VAR(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) pPduInfo
);

STATIC FUNC(boolean,FRTP_CODE) FrTp_TxSm_TxBufferHandling
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(uint8,AUTOMATIC) TxPduIdx,
    VAR(BufReq_ReturnType,AUTOMATIC) PduRBuffStatus
);

/**
 * \brief Resets all state variables for the given TX channel.
 */
STATIC FUNC(void,FRTP_CODE) FrTp_ResetTxChannel
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel
);

/*
 * Stop code section declaration
 */
#define FRTP_STOP_SEC_CODE
#include <FrTp_MemMap.h>

/*=====================[Function Definitions]================================*/
/*
 * Start code section declaration
 */
#define FRTP_START_SEC_CODE
#include <FrTp_MemMap.h>

FUNC(void, FRTP_CODE) FrTp_TxSm_RequestTransmission
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(uint16,AUTOMATIC) msgLength
)
{
    /* store total message length to transmit */
    pChannel->Tx_ML = msgLength;

    /* reset counters of transmitted message length */
    pChannel->Tx_CtrDataAck = 0U;

    /* reset separation cycle settings (immediate transmission, no separation cyles) */
    pChannel->Tx_BCCtrCycleDistance = 0U;

    /* reset Tx pending counter */
    pChannel->Tx_CtrTxPdusPending = 0U;

    /* reset consecutive frame type 0 */
    pChannel->Tx_PCI = 0x00U;

    /* reset Tx_FrIfRetry counter */
    pChannel->Tx_FrIfRetry = 0U;

    /* reset Tx_CtrBufReq counter */
    pChannel->Tx_CtrBufReq = 0U;

    /* reset Tx-state machine related timers */
    FrTp_StopTimer(pChannel->Tx_Timer1);
    FrTp_StopTimer(pChannel->Tx_Timer2);

    /* reset retry frame marker */
    pChannel->Tx_Retry = 0U;

    /* initialize bandwith control (disabled in the beginning) */
    pChannel->Tx_BCMaxPduPerCycle = 255U;
    pChannel->Tx_BCMinCycleDistance = 0U;

    /* set state machine to tranmission of data */
    /* Indicating all members are initialized and tranmission can proceed */
    TS_AtomicAssign8(pChannel->Tx_State, FRTP_CHNL_TX_DATA);
}


/* return TRUE if still data to transmit available */
/* return FALSE if no more data to transmit available */
FUNC(boolean,FRTP_CODE) FrTp_TxSm_TransmitSyncPoint
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(uint8,AUTOMATIC) NewCycle
)
{
    /* flag to indicate if more data is available to transmit */
    boolean fMoreDataAvailable = TRUE;

    /* get current channel state into local variable */
    VAR(uint8,AUTOMATIC) TxState;

    TS_AtomicAssign8(TxState, pChannel->Tx_State);

    if(FrTp_Chnl_GetTxState(TxState) != FRTP_CHNL_TX_IDLE)
    {
        /* performance requirement calculations */
        /* is a transmission in this cycle forbidden due to performance requirements (BC) ? */
        if(NewCycle != FALSE)
        {
            /* are we allowed to transmit within this cycle ? */
            if(pChannel->Tx_BCCtrCycleDistance > 0U)
            {
                /* decrement transmission denied counter */
                pChannel->Tx_BCCtrCycleDistance--;

                /* no further transmission is requested by this channel for now */
                fMoreDataAvailable = FALSE;
            }
            else
            {
                /* reset number of TxPdus transmitted for this cycle */
                pChannel->Tx_BCCtrPdu = 0x0U;
            }
        }
        else
        {
            /* increment number of TxPdus transmitted for this cycle */
            pChannel->Tx_BCCtrPdu++;

            /* if maximum number of Pdus have been transmitted within this cycle,
             * then stop transmitting for this cycle
             */
            if(pChannel->Tx_BCCtrPdu >= pChannel->Tx_BCMaxPduPerCycle)
            {
                fMoreDataAvailable = FALSE;
            }
        }
    }

    /* this channel doesn't want to transmit currently */
    if(TxState != FRTP_CHNL_TX_DATA)
    {
        /* no further tx call required for now */
        fMoreDataAvailable = FALSE;
    }

    /* if there is still a transmission pending, abort
       further transmission processing within this cycle */
    if((NewCycle!=0U) && (pChannel->Tx_CtrTxPdusPending > 0U))
    {
        fMoreDataAvailable = FALSE;
    }

    if ( fMoreDataAvailable == TRUE )
    {
        /* no data acknowledged, no data transmitted this is a SF */
        if(pChannel->Tx_CtrDataAck == 0U)
        {
            fMoreDataAvailable = FrTp_TxSm_TransmitSyncPointSF(pChannel);
        }
        else
        {
            fMoreDataAvailable = FrTp_TxSm_TransmitSyncPointCFLF(pChannel);
        }
    }

    /* return whether we need another transmission request */

    return fMoreDataAvailable;
}


STATIC FUNC(boolean,FRTP_CODE) FrTp_TxSm_TransmitSyncPointSF
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel
)
{
    /* get pointer to associated connection object */
    CONSTP2CONST(FrTp_ConnectionType,AUTOMATIC,FRTP_APPL_CONST) pConnection =
            FRTP_CFG_GET_PTR_TO_CONNECTION(pChannel->ConnectionIdx);

    /* store the message length to transmit into a temporary */
    const uint16_least Length = pChannel->Tx_ML;

    /* amount of payload that is provided by the upper layer */
    PduLengthType ActualLengthTmp = 0U;

    /* amount of data limited to requested total message length */
    uint32_least ActualLength;

    /* total Pdu length required for a startframe */
    uint32_least Length_STF;

    /* Pdu index for transmission */
    uint8 TxPduIdx;

    /* pointer to the selected TxPdu configuration data */
    P2CONST(FrTp_TxPduConfigType, AUTOMATIC, FRTP_APPL_CONST) pTxPdu;

    /* length of the Tx-Pdu */
    uint8_least PduLength;

    /* return value of PduR call */
    BufReq_ReturnType retval;

    /* PduInfo structure required for calling PduR_FrTpCopyTxData */
    PduInfoType BufferCopyInfo = { NULL_PTR, 0U };

    /* assume this channel has no retry per default */
    RetryInfoType RetryInfo = {TP_DATACONF, 0U};
    P2VAR(RetryInfoType,AUTOMATIC,FRTP_APPL_DATA) RetryInfoPtr = NULL_PTR;


    /* if acknowledge is enabled we use TP_DATACONF as default for startup frames */
    /* this will enable the UL to fetch new data */
    if( (pConnection->TpOptions&FRTP_CONNECTION_HASACK)!=0U )
    {
        RetryInfoPtr = &RetryInfo;
    }

    /* route to PduR function to get actual amount of data available */

    retval = PduR_FrTpCopyTxData( pConnection->TxSduIdx,
                                  &BufferCopyInfo,
                                  RetryInfoPtr,
                                  &ActualLengthTmp
                                );

    if(retval == BUFREQ_E_NOT_OK)
    {
        /* transmission is already finished on for this connection */
        /* free channel */
        FrTp_ResetTxChannel(pChannel);
        PduR_FrTpTxConfirmation(pConnection->TxSduIdx,NTFRSLT_E_NOT_OK);
    }
    else if((retval == BUFREQ_E_BUSY) ||
            ((retval == BUFREQ_OK) && (ActualLengthTmp == 0U))
           )
    {
        if(pChannel->Tx_CtrBufReq >= pConnection->MaxBufReq)
        {
            FrTp_TxSm_IndicateAndAbort(pChannel, NTFRSLT_E_NOT_OK);
        }
        pChannel->Tx_CtrBufReq++;
    }
    else
    {
        /* limit length to process to available data and message length */
        ActualLength = (ActualLengthTmp > Length) ? Length : ActualLengthTmp;

        /* calculate best length of a startframe */
        Length_STF = (uint32_least)ActualLength + (uint32_least)FrTp_Size_Header_STF;

        /* lookup a Tx-Pdu */
        TxPduIdx =
            FrTp_GetFreeTxPdu(pChannel, (PduLengthType)FrTp_Size_Header_STF + 1U);

        /* -) Transmit SF only if free TxPdu of sufficient size is available. */
        if(TxPduIdx != FRTP_TXPDU_INVALID)
        {
            /* get Tx-Pdu info structure */
            pTxPdu = FRTP_CFG_GET_PTR_TO_TXPDU(TxPduIdx);

            /* Get Tx-Pdu length */
            PduLength = pTxPdu->PduLength;

            /* check SF transmit condition: */
            /* cannot transmit either because: */
            /* -) this is a broadcast message and */
            /*    the payload isn't available at once. */
            if( !(((pConnection->TpOptions & FRTP_CONNECTION_ISBROADCAST) != 0U) &&
                  ((Length != ActualLength) || (PduLength < Length_STF))
                 )
              )
            {
                /* ok let's send a SF */

                /* we must construct a PduInfo structure to pass to FrIf_Transmit */
                PduInfoType TxPduInfo;

                /* we support only decoupled transmission, thus no
                   data is contained in the FrIf_transmit call */
                TxPduInfo.SduDataPtr = NULL_PTR;

                /* calculate the length of the SF */
                if(Length_STF > (uint32)PduLength)
                {
                    /* payload is limited by PduLength */
                    Length_STF = PduLength;
                    ActualLength = Length_STF - (uint32)FrTp_Size_Header_STF;
                }

                /* save SF length to PduInfo */
                TxPduInfo.SduLength = (PduLengthType)Length_STF;

                {
                    /* initiate transmission on lower layer */
                    const Std_ReturnType RetVal = FrIf_Transmit(pTxPdu->PduId,&TxPduInfo);

                    if(RetVal == E_OK)
                    {
                        /* rearm bandwidth control monitor */
                        pChannel->Tx_BCCtrCycleDistance = pChannel->Tx_BCMinCycleDistance;

                        /* start timer AS - there is indeed some new data to
                           transmit so we can reload the timer immediately */
                        FrTp_StartTimer(pChannel->Tx_Timer1, pConnection->TimeroutAS);

                        /* perform Tx-Pdu allocation */
                        FrTp_RuntimeTxPdu[TxPduIdx].pChannel = pChannel;

                        /* save number of bytes to transmit */
                        /* needed again in FrTp_TriggerTransmit */
                        FrTp_RuntimeTxPdu[TxPduIdx].PayloadLength = (uint8)ActualLength;

                        /* this PDU is used for data transmission */
                        FrTp_RuntimeTxPdu[TxPduIdx].PduType = FRTP_TXPDUTYPE_DATA_SF;

                        /* for the first frame, buffersize is equal to first frame length */
                        pChannel->Tx_BfS = (uint16)ActualLength;

                        /* save currently requested tx data */
                        pChannel->Tx_CtrBlockTransmit = (uint16)ActualLength;

                        /* save currently copied tx data */
                        pChannel->Tx_CtrBlockTriggerTransmit = 0U;

                        /* increment number of pending tx Pdus */
                        pChannel->Tx_CtrTxPdusPending++;

                        /* reset FrIf retry counter */
                        pChannel->Tx_FrIfRetry = 0U;

                        /* reset Tx_CtrBufReq counter */
                        pChannel->Tx_CtrBufReq = 0U;

                        /* reset Tx_BfSPduR */
                        pChannel->Tx_BfSPduR = 0U;
                    }
                    else
                    {
                      pChannel->Tx_FrIfRetry++;
                      if(pChannel->Tx_FrIfRetry == pConnection->MaxFrIfRetry)
                      {
                        /* transmission is already finished on for this connection */
                        /* free channel */
                        FrTp_ResetTxChannel(pChannel);
                        PduR_FrTpTxConfirmation(pConnection->TxSduIdx,NTFRSLT_E_NOT_OK);
                      }
                    }
                }
            }
            else
            {
              if(pChannel->Tx_CtrBufReq >= pConnection->MaxBufReq)
              {
                  FrTp_TxSm_IndicateAndAbort(pChannel, NTFRSLT_E_NOT_OK);
              }
              pChannel->Tx_CtrBufReq++;
            }
        }
    }

    /* we don't need another transmission request */

    return FALSE;
}

STATIC FUNC(boolean,FRTP_CODE) FrTp_TxSm_TransmitSyncPointCFLF
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel
)
{
    boolean TxComplete = TRUE;

    /* get pointer to associated connection object */
    CONSTP2CONST(FrTp_ConnectionType,AUTOMATIC,FRTP_APPL_CONST) pConnection =
            FRTP_CFG_GET_PTR_TO_CONNECTION(pChannel->ConnectionIdx);


    /* start of new block of data and more data available? */
    /* if no more data is available a last frame without payload is sent to finalize the transmission */
    if( /* The beginning of a new block AND the whole message has not been acknowledged yet */
        ((pChannel->Tx_CtrBlockTransmit == 0U) && (pChannel->Tx_ML != pChannel->Tx_CtrDataAck)) ||
        ( /* The PduR indicated a buffer underrun AND the whole message has not been sent yet */
            (pChannel->Tx_BfSPduR <= pChannel->Tx_CtrBlockTransmit) &&
            ((pChannel->Tx_CtrBlockTransmit + pChannel->Tx_CtrDataAck) < pChannel->Tx_ML)
        )
      )
    {
        /* yes, new block of data, calculate Buffersize */

        /* amount of payload that is already provided by the upper layer */
        PduLengthType BfsTmp = 0U;

        /* return value of PduR call */
        BufReq_ReturnType retval;

        /* PduInfo structure required for calling PduR_FrTpCopyTxData */
        PduInfoType BufferCopyInfo = { NULL_PTR, 0U };

        /* assume this channel has no retry per default */
        RetryInfoType RetryInfo = {TP_DATACONF, 0U};
        P2VAR(RetryInfoType,AUTOMATIC,FRTP_APPL_DATA) RetryInfoPtr = NULL_PTR;

        /* if acknowledge is enabled we use TP_DATACONF as default for startup frames */
        /* this will enable the UL to fetch new data */
        if( (pConnection->TpOptions&FRTP_CONNECTION_HASACK)!=0U )
        {
            RetryInfoPtr = &RetryInfo;
        }

        /* route to PduR function to get the actual amount of data available */
        retval = PduR_FrTpCopyTxData( pConnection->TxSduIdx,
                                      &BufferCopyInfo,
                                      RetryInfoPtr,
                                      &BfsTmp
                                    );
        if(retval == BUFREQ_E_NOT_OK)
        {
            FrTp_TxSm_IndicateAndAbort(pChannel, NTFRSLT_E_NOT_OK);
            TxComplete = FALSE;
        }
        else if((retval == BUFREQ_E_BUSY) ||
                ((retval == BUFREQ_OK) && (BfsTmp == 0U))
               )
        {
            if(pChannel->Tx_CtrBufReq >= pConnection->MaxBufReq)
            {
                FrTp_TxSm_IndicateAndAbort(pChannel, NTFRSLT_E_NOT_OK);
            }
            pChannel->Tx_CtrBufReq++;
            TxComplete = FALSE;
        }
        else
        {
            pChannel->Tx_BfSPduR += (uint16) BfsTmp;
        }
    }
    if(TxComplete == TRUE)
    {
        /* minimum buffer size of receiver and PduR */
        const uint16 BfsMin =
            (pChannel->Tx_BfS > pChannel->Tx_BfSPduR) ? pChannel->Tx_BfSPduR : pChannel->Tx_BfS;

        /* get outstanding buffersize for this segment */
        const uint16 BfsOut = BfsMin - pChannel->Tx_CtrBlockTransmit;

        /* calculate outstanding message length */
        const uint16 MLOut =
            pChannel->Tx_ML - pChannel->Tx_CtrDataAck - pChannel->Tx_CtrBlockTransmit;

        /* get next free Pdu */
        const uint8 TxPduIdx = FrTp_GetFreeTxPdu(pChannel, (PduLengthType) FrTp_Size_Header_CF + 1U);
        TxComplete = FALSE;
        /* proceed if we got a valid PduIdx */
        if(TxPduIdx != FRTP_TXPDU_INVALID)
        {
            /* get a pointer to the selected TxPdu configuration data */
            CONSTP2CONST(FrTp_TxPduConfigType, AUTOMATIC, FRTP_APPL_CONST) pTxPdu =
                    FRTP_CFG_GET_PTR_TO_TXPDU(TxPduIdx);

            /* get length of Pdu into a local variable (can't be larger than uint8)*/
            const uint8 PduLength = (uint8)pTxPdu->PduLength;

            /* variable storing actual FrTp payload length to transmit */
            uint8 ActualLength;

            /* type of Tx-Pdu */
            uint8 TxPduType;

            /* we must construct a PduInfo structure to pass to FrIf_Transmit */
            PduInfoType TxPduInfo;

            /* has a whole block transfer completed ? */
            boolean BlockCompleted = TRUE;
            /* we support only decoupled transmission, thus no data is
               contained in the FrIf_transmit call */
            TxPduInfo.SduDataPtr = NULL_PTR;

            /* check if the rest of the message fits into the Pdu */
            /* ... and does not exceed the BufferSize */
            if((PduLength >= (MLOut + (uint32) FrTp_Size_Header_LF)) && (MLOut <= BfsOut))
            {
                ActualLength = (uint8)MLOut;
                TxPduInfo.SduLength = (PduLengthType)(ActualLength + FrTp_Size_Header_LF);

                TxPduType = FRTP_TXPDUTYPE_DATA_LF;
            }
            /* If the rest of this block fits into the Pdu and there's data for a following block */
            else if((PduLength >= (BfsOut + (uint32) FrTp_Size_Header_CF)) && (MLOut > BfsOut))
            {
                ActualLength = (uint8)BfsOut;
                TxPduInfo.SduLength = (PduLengthType)(BfsOut + FrTp_Size_Header_CF);

                /* If the receiver buffer size is limiting the transmission */
                if(pChannel->Tx_BfSPduR >= pChannel->Tx_BfS)
                {
                    TxPduType = FRTP_TXPDUTYPE_DATA_CFEOB;
                }
                /* If the PduR buffer size is limiting the transmission */
                else
                {
                    TxPduType = FRTP_TXPDUTYPE_DATA_CF;
                }
            }
            /* transmit an ordinary consecutive frame */
            else
            {
                ActualLength = PduLength - (uint8)FrTp_Size_Header_CF;
                /* ActualLength could be greater than number of bytes that shall be transmitted */
                if(ActualLength > MLOut)
                {
                    ActualLength = (uint8) MLOut;
                }
                TxPduInfo.SduLength = (PduLengthType)(ActualLength + FrTp_Size_Header_CF);
                BlockCompleted = FALSE;

                TxPduType = FRTP_TXPDUTYPE_DATA_CF;
            }

            {
                /* initiate transmission on lower layer */
                const Std_ReturnType RetVal = FrIf_Transmit(pTxPdu->PduId, &TxPduInfo);

                if(RetVal == E_OK)
                {
                    /* rearm bandwidth control monitor */
                    pChannel->Tx_BCCtrCycleDistance = pChannel->Tx_BCMinCycleDistance;

                    /* start timer AS - there is indeed some new data to transmit
                       so we can reload the timer immediately */
                    FrTp_StartTimer(pChannel->Tx_Timer1, pConnection->TimeroutAS);

                    /* perform Tx-Pdu allocation */
                    FrTp_RuntimeTxPdu[TxPduIdx].pChannel = pChannel;

                    /* save number of bytes to transmit */
                    /* needed again in FrTp_TriggerTransmit */
                    FrTp_RuntimeTxPdu[TxPduIdx].PayloadLength = ActualLength;

                    /* this PDU is used for data transmission */
                    FrTp_RuntimeTxPdu[TxPduIdx].PduType = TxPduType;

                    /* save currently processing tx data */
                    pChannel->Tx_CtrBlockTransmit += ActualLength;

                    /* increment number of pending tx Pdus */
                    pChannel->Tx_CtrTxPdusPending++;

                    /* reset FrIf retry counter */
                    pChannel->Tx_FrIfRetry = 0U;

                    /* reset Tx_CtrBufReq counter */
                    pChannel->Tx_CtrBufReq = 0U;

                    TxComplete = !BlockCompleted;
                }
                else
                {
                    pChannel->Tx_FrIfRetry++;
                    if(pChannel->Tx_FrIfRetry == pConnection->MaxFrIfRetry)
                    {
                        /* transmission is already finished on for this connection */
                        /* free channel */
                        FrTp_ResetTxChannel(pChannel);
                        PduR_FrTpTxConfirmation(pConnection->TxSduIdx, NTFRSLT_E_NOT_OK);
                    }
                    TxComplete = FALSE;
                }
            }
        }
    }

    return TxComplete;
}

FUNC(boolean,FRTP_CODE) FrTp_TxSm_TriggerTransmit
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(uint8,AUTOMATIC) TxPduIdx,
    P2VAR(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) pPduInfo
)
{
    boolean TxFlag = TRUE;

    /* get runtime information from TxPdu */
    CONSTP2VAR(FrTp_RuntimeTxPduType, AUTOMATIC, FRTP_VAR) pTxPdu = &FrTp_RuntimeTxPdu[TxPduIdx];

    /* local variable for atomic comparison */
    VAR(uint8,AUTOMATIC) localTx_State;

    TS_AtomicAssign8(localTx_State, pChannel->Tx_State);

    /* check for correct state been aborted */
    if(localTx_State == FRTP_CHNL_TX_DATA)
    {
        /* get pointer to associated connection object */
        CONSTP2CONST(FrTp_ConnectionType,AUTOMATIC,FRTP_APPL_CONST) pConnection =
                FRTP_CFG_GET_PTR_TO_CONNECTION(pChannel->ConnectionIdx);

        /* calculate number of payload bytes to transmit */
        const uint8 ActualLength = pTxPdu->PayloadLength;

        /* assume this channel has no retry per default */
        RetryInfoType RetryInfo = {TP_CONFPENDING, 0U};
        P2VAR(RetryInfoType,AUTOMATIC,AUTOMATIC) RetryInfoPtr = NULL_PTR;
        BufReq_ReturnType RetCode;

        /* currently not used */
        PduLengthType AvailableData;

        /* Pdu info struct for destination buffer */
        PduInfoType TxDestinationInfo;

        /* set payload length that shall be written */
        TxDestinationInfo.SduLength = ActualLength;

        /* length to be transmitted */
        pPduInfo->SduLength = ActualLength;

        /* set address information into tx-buffer */
        FrTp_SetAI(pPduInfo->SduDataPtr,pConnection->RemoteAddress, pConnection->LocalAddress);

        /* Update the header for each frame and verify whether transmission can be proceeded */
        FrTp_TxSm_HandleTriggerTxFrame(pChannel, pTxPdu, &TxDestinationInfo, pPduInfo);

        /* if acknowledge is enabled we use TP_DATACONF as default for startup frames */
        /* this will enable the UL to fetch new data */
        if( (pConnection->TpOptions&FRTP_CONNECTION_HASACK)!=0U )
        {
            RetryInfoPtr = &RetryInfo;

            /* check if we have a retry situation */
            if(pChannel->Tx_Retry != 0U)
            {
                /* yes we have a retry - pass negative offset to current position to UL */
                RetryInfo.TxTpDataCnt = pChannel->Tx_Retry;
                RetryInfo.TpDataState = TP_DATARETRY;
                pChannel->Tx_Retry = 0U;
            }
            else if (pChannel->Tx_CtrBlockTriggerTransmit == 0U)
            {
              /* no retry. This is the 1st CF of a new block, old data can be discarded
               * and will not be requested again */
              RetryInfo.TpDataState = TP_DATACONF;
            }
            else
            {
                ; /* no retry, keep data in buffer in case retry occurs (TP_CONFPENDING) */
            }
        }

        /* route to PduR function to copy payload data */
         RetCode = PduR_FrTpCopyTxData( pConnection->TxSduIdx,
                                        &TxDestinationInfo,
                                        RetryInfoPtr,
                                        &AvailableData
                                      );
         /* Proceed according to the PduR buffer availability */
         TxFlag = FrTp_TxSm_TxBufferHandling( pChannel,
                                              TxPduIdx,
                                              RetCode
                                            );
    }
    else
    {
          /* In other states, this function will not be called since the PDU will be free. */
          FRTP_PRECONDITION_ASSERT(localTx_State == FRTP_CHNL_TX_ABORT_PENDING,
              FRTP_TRIGGERTRANSMIT_SERVICE_ID);

          /* de-allocate the affected TxPdu */
          pTxPdu->pChannel = NULL_PTR;

          /* decrement the TxPduPending counter */
          pChannel->Tx_CtrTxPdusPending--;

          TxFlag = FALSE;
    }
    return TxFlag;
}

FUNC(void,FRTP_CODE) FrTp_TxSm_TransmitConfirmation
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(uint8,AUTOMATIC) TxPduIdx
)
{
    /* get runtime information from TxPdu */
    CONSTP2VAR(FrTp_RuntimeTxPduType, AUTOMATIC, FRTP_VAR) pTxPdu = &FrTp_RuntimeTxPdu[TxPduIdx];

    /* This condition will always be true because this function will not be called if all the PDUs
     * are confirmed */
    FRTP_PRECONDITION_ASSERT(pChannel->Tx_CtrTxPdusPending > 0U, FRTP_TXCONFIRMATION_SERVICE_ID);
    /* If there are multiple Tx pdus pending, decrement pending counter */
    /* CHECK: NOPARSE */
    if(pChannel->Tx_CtrTxPdusPending > 0U)
    /* CHECK: PARSE */
    {
        /* get pointer to associated connection object */
        CONSTP2CONST(FrTp_ConnectionType,AUTOMATIC,FRTP_APPL_CONST) pConnection =
                FRTP_CFG_GET_PTR_TO_CONNECTION(pChannel->ConnectionIdx);

        /* decrement the Tx-Pending counter */
        pChannel->Tx_CtrTxPdusPending--;

        /* if all LPDus of the last transmission cylce have
           been transmitted, then stop the timer */
        if(pChannel->Tx_CtrTxPdusPending == 0U)
        {
            /* stop timer AS */
            FrTp_StopTimer(pChannel->Tx_Timer1);
        }

        if (pChannel->Tx_State == FRTP_CHNL_TX_ABORT_PENDING)
        {
            /* transmission was aborted and we are waiting for pending TxConfirmations
             * before resetting channel */
            if (pChannel->Tx_CtrTxPdusPending == 0U)
            {
                /* no more TxConfirmations pending
                 * -> safely reset channel and indicate E_NOT_OK */
                FrTp_TxSm_IndicateAndAbort(pChannel, NTFRSLT_E_NOT_OK);
            }
        }
        else
        {
            switch(pTxPdu->PduType)
            {
                case FRTP_TXPDUTYPE_DATA_SF:
                {
                    /* is this unsegmented transfer without acknowledge ? */
                    if( (pChannel->Tx_CtrDataAck + pChannel->Tx_CtrBlockTriggerTransmit)
                        >= pChannel->Tx_ML
                      )
                    {

                        if(((pConnection->TpOptions&FRTP_CONNECTION_HASACK) == 0U) ||
                           ((pConnection->TpOptions&FRTP_CONNECTION_ISBROADCAST) != 0U)
                           )
                        {
                            /* provide transmission confirmation */
                            /* transmission is already finished on for this connection */
                            /* free channel */
                            FrTp_ResetTxChannel(pChannel);
                            PduR_FrTpTxConfirmation(pConnection->TxSduIdx,NTFRSLT_OK);
                        }
                        else
                        {

                            /* start timer BS */
                            FrTp_StartTimer(pChannel->Tx_Timer2,pConnection->TimeroutBS);

                            /* change into state that waits for an acknowledge */
                            TS_AtomicAssign8(pChannel->Tx_State, FRTP_CHNL_TX_WAIT_ACK);
                        }
                    }
                    else
                    {
                        /* start timer BS */
                        FrTp_StartTimer(pChannel->Tx_Timer2,pConnection->TimeroutBS);

                        /* change into state that waits for a flow control */
                        TS_AtomicAssign8(pChannel->Tx_State, FRTP_CHNL_TX_WAIT_FC);
                    }

                    break;
                }
                case FRTP_TXPDUTYPE_DATA_CFEOB:
                {
                    /* buffer is filled, wait for a FC message */

                    /* start timer BS */
                    FrTp_StartTimer(pChannel->Tx_Timer2,pConnection->TimeroutBS);

                    /* change into state that waits for a flow control */
                    TS_AtomicAssign8(pChannel->Tx_State, FRTP_CHNL_TX_WAIT_FC);

                    break;
                }
                case FRTP_TXPDUTYPE_DATA_CF:
                {
                    /* continue with transmission */
                    TS_AtomicAssign8(pChannel->Tx_State, FRTP_CHNL_TX_DATA);

                    break;
                }
                case FRTP_TXPDUTYPE_DATA_LF:
                {
                    if( (pConnection->TpOptions&FRTP_CONNECTION_HASACK) == 0U )
                    {
                        /* provide transmission confirmation */
                        /* free channel */
                        FrTp_ResetTxChannel(pChannel);
                        PduR_FrTpTxConfirmation(pConnection->TxSduIdx,NTFRSLT_OK);
                    }
                    else
                    {
                        /* start timer BS */
                        FrTp_StartTimer(pChannel->Tx_Timer2,pConnection->TimeroutBS);

                        /* change into state that waits for a flow control */
                        TS_AtomicAssign8(pChannel->Tx_State, FRTP_CHNL_TX_WAIT_ACK);
                    }

                    break;
                }
                /* CHECK: NOPARSE */
                default:
                {
                    /* unreachable */
                    FRTP_UNREACHABLE_CODE_ASSERT(FRTP_TXCONFIRMATION_SERVICE_ID);
                    break;
                }
                /* CHECK: PARSE */
            }
        }
    }
    /* deallocate the affected TX-Pdu */
    pTxPdu->pChannel = NULL_PTR;
}

FUNC(void,FRTP_CODE) FrTp_TxSm_CTS
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(uint16,AUTOMATIC) bufferSize,
    VAR(uint8,AUTOMATIC) bandwidthControl
)
{
    /* local variable for atomic comparison */
    VAR(uint8,AUTOMATIC) localTx_State;

    TS_AtomicAssign8(localTx_State, pChannel->Tx_State);

    /* perform only if the state is correct - ignore otherwise */
    if((localTx_State == FRTP_CHNL_TX_WAIT_FC) &&
       (pChannel->Tx_BfS == pChannel->Tx_CtrBlockTriggerTransmit) &&
       (pChannel->Tx_CtrTxPdusPending == 0U)
      )
    {
        /* table that maps the SCexp parameter to the actual number of separation cycles */
        CONST(uint8, AUTOMATIC) SCExpTable[8] = {0U,1U,3U,7U,15U,31U,63U,127U};

        /* temporary with maximum number of Pdus Per cycle */
        const uint8 MNPC = (bandwidthControl&0xF8U)>>3U;

        /* temporary with separation cycle exponent */
        const uint8 SCexp = (bandwidthControl&0x07U);

        /* stop timer BS */
        FrTp_StopTimer(pChannel->Tx_Timer2);

        /* check if receiver has still buffer limitations or not */
        if(bufferSize != 0U)
        {
            /* save buffer size obtained from receiver side */
            pChannel->Tx_BfS = bufferSize;
        }
        else
        {
            /* set buffer size to maximum */
            pChannel->Tx_BfS = 0xFFFFU;
        }

        /* Reset the PduR buffer size */
        pChannel->Tx_BfSPduR = 0U;

        /* save newest bandwith control parameter */
        /* MNPC = 0 means that bandwith control is disabled */
        if(MNPC == 0U)
        {
            pChannel->Tx_BCMinCycleDistance= 0U;      /* disabled: no seperation cycles   */
            pChannel->Tx_BCMaxPduPerCycle= 0xFFU;    /* disabled: max 256 Pdus per cycle */
        }
        else
        {
            /* enabled: get value from receiver */
            pChannel->Tx_BCMinCycleDistance = ((uint16)SCExpTable[SCexp])*((uint16)FRTP_MAINFUNCTIONS_PER_FLEXRAYCYCLE);
            /* enabled: get value from receiver */
            pChannel->Tx_BCMaxPduPerCycle = MNPC;
            /* Reset the CycleDistance counter value */
            pChannel->Tx_BCCtrCycleDistance = 0U;
        }

        /* transmitted data is acknowledged */
        pChannel->Tx_CtrDataAck += pChannel->Tx_CtrBlockTriggerTransmit;

        /* reset actual data transmit request counter */
        pChannel->Tx_CtrBlockTransmit = 0U;

        /* reset actual data transmit copy counter */
        pChannel->Tx_CtrBlockTriggerTransmit = 0U;

        /* continue transmission */
        TS_AtomicAssign8(pChannel->Tx_State, FRTP_CHNL_TX_DATA);
    }

}

FUNC(void,FRTP_CODE) FrTp_TxSm_ACK
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel
)
{
    /* get pointer to associated connection object */
    CONSTP2CONST(FrTp_ConnectionType,AUTOMATIC,FRTP_APPL_CONST) pConnection =
            FRTP_CFG_GET_PTR_TO_CONNECTION(pChannel->ConnectionIdx);

    /* local variable for atomic comparison */
    VAR(uint8,AUTOMATIC) localTx_State;

    TS_AtomicAssign8(localTx_State, pChannel->Tx_State);

    /* perform only if the state is correct (and acknowledge is configured) - ignore otherwise */
    if( (localTx_State == FRTP_CHNL_TX_WAIT_ACK)
      &&( (pConnection->TpOptions&FRTP_CONNECTION_HASACK) != 0U)
      )
    {
        /* transmission successfully finished for this connection */
        /* reset the transmission channel */
        /* this implicitly stops the TimerBS */
        FrTp_ResetTxChannel(pChannel);
        PduR_FrTpTxConfirmation(pConnection->TxSduIdx,NTFRSLT_OK);
    }

}

FUNC(void,FRTP_CODE) FrTp_TxSm_RETRY
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(uint16,AUTOMATIC) bufferPointer
)
{
    /* get pointer to associated connection object */
    CONSTP2CONST(FrTp_ConnectionType,AUTOMATIC,FRTP_APPL_CONST) pConnection =
            FRTP_CFG_GET_PTR_TO_CONNECTION(pChannel->ConnectionIdx);

    /* get current channel state into local variable */
    VAR(uint8,AUTOMATIC) TxState;

    TS_AtomicAssign8(TxState, pChannel->Tx_State);

    /* perform only if the state is correct (and acknowledge is configured) - ignore otherwise */
    if( (FrTp_Chnl_GetTxState(TxState) != FRTP_CHNL_TX_IDLE)
      &&( (pConnection->TpOptions&FRTP_CONNECTION_HASACK) != 0U )
      )
    {
        if ( (bufferPointer == 0U)
           &&(pChannel->Tx_CtrDataAck == 0U)
           )
        {
            /* no CTS yet received
             * -> only STF frame was sent yet, retry is requested from BP=0
             * -> send another STF */

            /* mark that the next frame send is a retry frame */
            pChannel->Tx_Retry = pChannel->Tx_CtrBlockTriggerTransmit;

            /* reset the already transmitted data to already acknowledged data */
            pChannel->Tx_CtrBlockTransmit = 0U;

            /* reset the already transmitted data to already acknowledged data */
            pChannel->Tx_CtrBlockTriggerTransmit = 0U;

            /* reset PCI */
            pChannel->Tx_PCI = 0U;

            /* immediately continue with transmission of STF */
            TS_AtomicAssign8(pChannel->Tx_State, FRTP_CHNL_TX_DATA);
        }
        /* check if the reported bufferPointer is out of the last transferred datablock */
        else if(bufferPointer < pChannel->Tx_CtrBlockTriggerTransmit)
        {
            /* stop timers */
            FrTp_StopTimer(pChannel->Tx_Timer1);
            FrTp_StopTimer(pChannel->Tx_Timer2);


            /* mark that the next frame send is a retry frame */
            pChannel->Tx_Retry = pChannel->Tx_CtrBlockTriggerTransmit - bufferPointer;
            /* reset the already transmitted data to already acknowledged data */
            pChannel->Tx_CtrBlockTransmit = bufferPointer;

            /* reset the already transmitted data to already acknowledged data */
            pChannel->Tx_CtrBlockTriggerTransmit = bufferPointer;

            /* toggle consecutive frame type */
            pChannel->Tx_PCI ^= 0x10U;

            /* reset sequence number */
            pChannel->Tx_PCI &= 0xF0U;

            /* immediately continue with transmission of next block */
            TS_AtomicAssign8(pChannel->Tx_State, FRTP_CHNL_TX_DATA);

        }
        else
        {
            /* transmission failed for this connection */
            /* reset transmission on this channel */
            /* this implicitly stops the TimerBS */
            FrTp_ResetTxChannel(pChannel);
            PduR_FrTpTxConfirmation(pConnection->TxSduIdx,NTFRSLT_E_FR_WRONG_BP);
        }
    }

}

FUNC(void,FRTP_CODE) FrTp_TxSm_WT
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel
)
{
    /* local variable for atomic comparison */
    VAR(uint8,AUTOMATIC) localTx_State;

    TS_AtomicAssign8(localTx_State, pChannel->Tx_State);

    /* perform only if the state is correct - ignore otherwise */
    if(localTx_State == FRTP_CHNL_TX_WAIT_FC)
    {
        /* get pointer to associated connection object */
        CONSTP2CONST(FrTp_ConnectionType,AUTOMATIC,FRTP_APPL_CONST) pConnection =
                FRTP_CFG_GET_PTR_TO_CONNECTION(pChannel->ConnectionIdx);

        /* restart Timer BS */
        FrTp_StartTimer(pChannel->Tx_Timer2,pConnection->TimeroutBS);
    }

}


FUNC(void,FRTP_CODE) FrTp_TxSm_OVFLW
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel
)
{
    /* local variable for atomic comparison */
    VAR(uint8,AUTOMATIC) localTx_State;

    TS_AtomicAssign8(localTx_State,pChannel->Tx_State);

    if(localTx_State == FRTP_CHNL_TX_WAIT_FC)
    {
        /* get pointer to associated connection object */
        CONSTP2CONST(FrTp_ConnectionType,AUTOMATIC,FRTP_APPL_CONST) pConnection =
                FRTP_CFG_GET_PTR_TO_CONNECTION(pChannel->ConnectionIdx);

        /* transmission aborted for this connection - no receive buffer with matching size */
        /* reset transmission channel */
        FrTp_ResetTxChannel(pChannel);
        PduR_FrTpTxConfirmation(pConnection->TxSduIdx,NTFRSLT_E_NO_BUFFER);
    }
}


FUNC(void,FRTP_CODE) FrTp_TxSm_IndicateAndAbort
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(NotifResultType,AUTOMATIC) result
)
{
    /* get pointer to associated connection object */
    CONSTP2CONST(FrTp_ConnectionType,AUTOMATIC,FRTP_APPL_CONST) pConnection =
            FRTP_CFG_GET_PTR_TO_CONNECTION(pChannel->ConnectionIdx);

    /* get current channel state into local variable */
    VAR(uint8,AUTOMATIC) TxState;

    TS_AtomicAssign8(TxState, pChannel->Tx_State);

    if(FrTp_Chnl_GetTxState(TxState) != FRTP_CHNL_TX_IDLE)
    {
        /* transmission aborted for this connection */
        /* reset transmission channel */
        FrTp_ResetTxChannel(pChannel);
        PduR_FrTpTxConfirmation(pConnection->TxSduIdx,result);
    }
}

/**
 * \brief Resets all state variables for the given TX channel.
 */
STATIC FUNC(void,FRTP_CODE) FrTp_ResetTxChannel
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel
)
{
#if (FRTP_LIMIT_NUMBER_CONNECTIONS == STD_ON)
    boolean StartBuffered = FALSE;
#endif /* FRTP_LIMIT_NUMBER_CONNECTIONS */

    /* enter exclusive area */
    SchM_Enter_FrTp_SCHM_FRTP_EXCLUSIVE_AREA_0();

    FrTp_StopTimer((pChannel)->Tx_Timer1);
    FrTp_StopTimer((pChannel)->Tx_Timer2);

#if (FRTP_FULLDUPLEX_ENABLE == STD_ON)
    if((pChannel)->Rx_State == FRTP_CHNL_RX_IDLE)
#endif /* FRTP_FULLDUPLEX_ENABLE */
    {
#if (FRTP_LIMIT_NUMBER_CONNECTIONS == STD_ON)
        StartBuffered = TRUE;
    }
#else
        (pChannel)->ConnectionIdx = FRTP_CONNECTION_INVALID;
    }
    (pChannel)->Tx_State = FRTP_CHNL_TX_IDLE;
#endif /* FRTP_LIMIT_NUMBER_CONNECTIONS */

    /* exit exclusive area */
    SchM_Exit_FrTp_SCHM_FRTP_EXCLUSIVE_AREA_0();

#if (FRTP_LIMIT_NUMBER_CONNECTIONS == STD_ON)
    if(StartBuffered)
    {
        Std_ReturnType RetVal = FrTp_StartBufferedConnection(pChannel);

        if(RetVal == E_NOT_OK)
        {
            /* enter exclusive area */
            SchM_Enter_FrTp_SCHM_FRTP_EXCLUSIVE_AREA_0();

            (pChannel)->ConnectionIdx = FRTP_CONNECTION_INVALID;
            (pChannel)->Tx_State = FRTP_CHNL_TX_IDLE;

            /* exit exclusive area */
            SchM_Exit_FrTp_SCHM_FRTP_EXCLUSIVE_AREA_0();
        }
    }
    else
    {
        (pChannel)->Tx_State = FRTP_CHNL_TX_IDLE;
    }
#endif
}

STATIC FUNC(void,FRTP_CODE) FrTp_TxSm_HandleTriggerTxFrame
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    P2VAR(FrTp_RuntimeTxPduType, AUTOMATIC, FRTP_VAR) pTxPdu,
    P2VAR(PduInfoType, AUTOMATIC, FRTP_VAR) DestinationInfo,
    P2VAR(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) pPduInfo
)
{
    const uint8 ActualLength = pTxPdu->PayloadLength;
    /* get pointer to associated connection object */
    CONSTP2CONST(FrTp_ConnectionType,AUTOMATIC,FRTP_APPL_CONST) pConnection =
            FRTP_CFG_GET_PTR_TO_CONNECTION(pChannel->ConnectionIdx);


    /* Update the destination pointer and the frame header for different Tx frames */
    switch(pTxPdu->PduType)
    {
        case FRTP_TXPDUTYPE_DATA_SF:
        {
            /* set destination data pointer */
            DestinationInfo->SduDataPtr = &(pPduInfo->SduDataPtr[FrTp_Size_Header_STF]);

            /* set PCI into tx-buffer */
            FrTp_SetPCI_SF(pPduInfo->SduDataPtr,
                           ((pConnection->TpOptions&FRTP_CONNECTION_HASACK)!=0U),
                           ActualLength,
                           pChannel->Tx_ML
                          );

            /* add header length to transmitted payload length */
            pPduInfo->SduLength += FrTp_Size_Header_STF;

            /* wait for transmission confirmation */
            TS_AtomicAssign8(pChannel->Tx_State, FRTP_CHNL_TX_WAIT_TX);

            break;
        }
        case FRTP_TXPDUTYPE_DATA_CFEOB:
        {
            /* set destination data pointer */
            DestinationInfo->SduDataPtr = &(pPduInfo->SduDataPtr[FrTp_Size_Header_CF]);

            /* set PCI into tx-buffer */
            FrTp_SetPCI_CF(pPduInfo->SduDataPtr,
                           ActualLength,
                           FRTP_PCITYPE_EOB + pChannel->Tx_PCI
                          );

            /* add header length to transmitted payload length */
            pPduInfo->SduLength += FrTp_Size_Header_CF;

            /* wait for transmition confirmations */
            TS_AtomicAssign8(pChannel->Tx_State, FRTP_CHNL_TX_WAIT_TX);

            break;
        }
        case FRTP_TXPDUTYPE_DATA_CF:
        {
            /* set destination data pointer */
            DestinationInfo->SduDataPtr = &(pPduInfo->SduDataPtr[FrTp_Size_Header_CF]);

            /* set PCI into tx-buffer */
            FrTp_SetPCI_CF(pPduInfo->SduDataPtr,
                           ActualLength,
                           (FRTP_PCITYPE_CF1 + pChannel->Tx_PCI)
                          );

            /* add header length to transmitted payload length */
            pPduInfo->SduLength += FrTp_Size_Header_CF;

            /* transmit further data */
            TS_AtomicAssign8(pChannel->Tx_State, FRTP_CHNL_TX_DATA);

            break;
        }
        case FRTP_TXPDUTYPE_DATA_LF:
        {
            /* set destination data pointer */
            DestinationInfo->SduDataPtr = &(pPduInfo->SduDataPtr[FrTp_Size_Header_LF]);

            /* set PCI into tx-buffer */
            FrTp_SetPCI_LF(pPduInfo->SduDataPtr,ActualLength,pChannel->Tx_ML);

            /* add header length to transmitted payload length */
            pPduInfo->SduLength += FrTp_Size_Header_LF;

            /* set new tx-state */
            TS_AtomicAssign8(pChannel->Tx_State, FRTP_CHNL_TX_WAIT_TX);

            break;
        }
        /* CHECK: NOPARSE */
        default:
        {
            /* This function is called only in above states, this part is not executed. */
            FRTP_UNREACHABLE_CODE_ASSERT(FRTP_TRIGGERTRANSMIT_SERVICE_ID);
            break;
        }
        /* CHECK: PARSE */
    }

}

STATIC FUNC(boolean,FRTP_CODE) FrTp_TxSm_TxBufferHandling
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel,
    VAR(uint8,AUTOMATIC) TxPduIdx,
    VAR(BufReq_ReturnType,AUTOMATIC) PduRBuffStatus
)
{
    boolean TxFlag = TRUE;
    /* get runtime information from TxPdu */
    CONSTP2VAR(FrTp_RuntimeTxPduType, AUTOMATIC, FRTP_VAR) pTxPdu = &FrTp_RuntimeTxPdu[TxPduIdx];
    /* calculate number of payload bytes to transmit */
    const uint8 ActualLength = pTxPdu->PayloadLength;

    if(PduRBuffStatus == BUFREQ_OK)
    {
        /* forward BufferSize counter */
        pChannel->Tx_CtrBlockTriggerTransmit += ActualLength;

        /* increment sequence number */
        if((pChannel->Tx_PCI&0x0FU) < 0x0FU)
        {
            pChannel->Tx_PCI++;
        }
        else
        {
            pChannel->Tx_PCI &= 0xF0U;
        }
    }
    else if (PduRBuffStatus == BUFREQ_E_BUSY)
    {
        /* PduR could not provide the number of bytes that it granted before
         * -> abort */
        (void)FrTp_FrIfCancelTransmit(pChannel);
        FrTp_TxSm_IndicateAndAbort(pChannel, NTFRSLT_E_NOT_OK);
        TxFlag = FALSE;
    }
    else
    {
        if (pChannel->Tx_CtrTxPdusPending == 1U)
        {
            /* just one TxConfirmation pending for this Pdu
             * -> abort immediately */
            (void)FrTp_FrIfCancelTransmit(pChannel);
            FrTp_TxSm_IndicateAndAbort(pChannel, NTFRSLT_E_NOT_OK);
        }
        /* CHECK: NOPARSE */
        else if (pChannel->Tx_CtrTxPdusPending > 1U)
        {
            /* TxConfirmations are pending -> wait for TxConfirmations before aborting */
            pChannel->Tx_State = FRTP_CHNL_TX_ABORT_PENDING;

            /* de-allocate the affected TxPdu */
            pTxPdu->pChannel = NULL_PTR;

            /* decrement the TxPduPending counter */
            pChannel->Tx_CtrTxPdusPending--;
        }
        else
        {
            /* unreachable (something is wrong if TriggerTransmit is
             * called but no Pdu is pending for transmission) */
            FRTP_UNREACHABLE_CODE_ASSERT(FRTP_TRIGGERTRANSMIT_SERVICE_ID);
        }
        /* CHECK: PARSE */
        TxFlag = FALSE;
    }

    return TxFlag;
}

FUNC(Std_ReturnType, FRTP_CODE) FrTp_FrIfCancelTransmit
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel
)
{
    /* Pdu iterator */
    uint8 iTxPdu;
    Std_ReturnType retVal = E_NOT_OK;


    /* iterate over all Pdus in TxPduPool */
    for(iTxPdu = 0U; iTxPdu < FrTp_RootPtr->nNumTxPdus; iTxPdu++)
    {
        /* look for transmissions on this channel */
        if(FrTp_RuntimeTxPdu[iTxPdu].pChannel == pChannel)
        {
#if 0 /* FrIf_CancelTransmit currently not implemented */
            /* cancel transmission with TxPduId */
            FrIf_CancelTransmit(iTxPdu);
#endif /* FrIf_CancelTransmit currently not implemented */

            /* deallocate Tx-Pdu */
            FrTp_RuntimeTxPdu[iTxPdu].pChannel = NULL_PTR;

            /* indicate transmission was canceled */
            retVal = E_OK;
        }
    }

    return retVal;
}

#if (FRTP_LIMIT_NUMBER_CONNECTIONS == STD_ON)
FUNC(Std_ReturnType, FRTP_CODE) FrTp_StartBufferedConnection
(
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel
)
{
    P2CONST(FrTp_ConnectionType, AUTOMATIC, FRTP_APPL_CONST) pConnectionBuffered;
    VAR(PduIdType, FRTP_VAR) TxSduId = 0U;
    VAR(PduLengthType, FRTP_VAR) SduLength = 0U;
    VAR(boolean, FRTP_VAR) StartBuffered = FALSE;
    Std_ReturnType RetVal = E_NOT_OK;

    /* Pointer to associated connection object */
    CONSTP2CONST(FrTp_ConnectionType, AUTOMATIC, FRTP_APPL_CONST) pConnectionFinished =
            FRTP_CFG_GET_PTR_TO_CONNECTION(pChannel->ConnectionIdx);

    /* Pointer to associated buffer information */
    P2VAR(FrTp_BufferInfoType, AUTOMATIC, FRTP_VAR) pBufferInfo =
        &FrTp_ConnectionBuffer.BufferInfos[pConnectionFinished->ConnectionLimitConfigIdx];

    SchM_Enter_FrTp_SCHM_FRTP_EXCLUSIVE_AREA_0();

    /* Decrement the active connection counter for the connection's remote address */
    if
    (
        (pChannel->Tx_State != FRTP_CHNL_TX_IDLE) &&
#if (FRTP_FULLDUPLEX_ENABLE == STD_OFF)
        !(pChannel->Tx_State & FRTP_CHNL_STATE_DIRECTION_RX) &&
#endif /* (FRTP_FULLDUPLEX_ENABLE == STD_OFF) */
        (pConnectionFinished->ConnectionLimitConfigIdx != FRTP_NUMBER_CONNECTION_LIMITS)
    )
    {
        FRTP_PRECONDITION_ASSERT
        (
            FrTp_ActiveConnectionsCnt[pConnectionFinished->ConnectionLimitConfigIdx] > 0,
            FRTP_SID_UNKNOWN_API
        );

        FrTp_ActiveConnectionsCnt[pConnectionFinished->ConnectionLimitConfigIdx]--;
    }

    /* If connection buffer for this remote address is not empty */
    if(pBufferInfo->IdxHead != pBufferInfo->IdxTail)
    {
        TxSduId = pBufferInfo->BufferPtr[pBufferInfo->IdxHead].TxSduId;
        SduLength = pBufferInfo->BufferPtr[pBufferInfo->IdxHead].SduLength;
        pConnectionBuffered = FRTP_CFG_GET_PTR_TO_CONNECTION(TxSduId);

        /* Increment number of active connections */
        if(pConnectionBuffered->ConnectionLimitConfigIdx != FRTP_NUMBER_CONNECTION_LIMITS)
        {
            FrTp_ActiveConnectionsCnt[pConnectionBuffered->ConnectionLimitConfigIdx]++;
        }

        /* Advance head index by one */
        pBufferInfo =
            &FrTp_ConnectionBuffer.BufferInfos[pConnectionBuffered->ConnectionLimitConfigIdx];
        pBufferInfo->IdxHead = (pBufferInfo->IdxHead + 1U) % pBufferInfo->BufferSize;

        StartBuffered = TRUE;
    }
    /* If no connections can be started from buffer for this remote address, search other buffers */
    else
    {
        FrTp_ConnectionLimitConfigIdxType iBuffer;
        for(iBuffer = 0U; iBuffer < FRTP_NUMBER_CONNECTION_BUFFERS; iBuffer++)
        {
            pBufferInfo = &FrTp_ConnectionBuffer.BufferInfos[iBuffer];

            /* Check if this buffer has already been checked before */
            if(iBuffer == pConnectionFinished->ConnectionLimitConfigIdx)
            {
                continue;
            }

            /* Check if the buffer is empty */
            if(pBufferInfo->IdxHead == pBufferInfo->IdxTail)
            {
                continue;
            }

            TxSduId = pBufferInfo->BufferPtr[pBufferInfo->IdxHead].TxSduId;
            SduLength = pBufferInfo->BufferPtr[pBufferInfo->IdxHead].SduLength;
            pConnectionBuffered = FRTP_CFG_GET_PTR_TO_CONNECTION(TxSduId);

            /* Check if active connections limit is reached for this remote address */
            if(pConnectionBuffered->ConnectionLimitConfigIdx != FRTP_NUMBER_CONNECTION_LIMITS)
            {
                uint8 connectionLimit =
                    FRTP_CFG_GET_PTR_TO_CONNECTION_LIMIT_CONFIG(iBuffer)->Limit;
                /* Check if connection limit is already reached */
                if(FrTp_ActiveConnectionsCnt[iBuffer] >= connectionLimit)
                {
                    continue;
                }
                else
                {
                    /* Increment number of active connections */
                    FrTp_ActiveConnectionsCnt[iBuffer]++;
                }
            }

            /* Advance head index by one */
            pBufferInfo =
                &FrTp_ConnectionBuffer.BufferInfos[pConnectionBuffered->ConnectionLimitConfigIdx];
            pBufferInfo->IdxHead = (pBufferInfo->IdxHead + 1U) % pBufferInfo->BufferSize;

            StartBuffered = TRUE;
            break;
        }
    }
    SchM_Exit_FrTp_SCHM_FRTP_EXCLUSIVE_AREA_0();

    if(StartBuffered)
    {
        pChannel->ConnectionIdx = (uint8) TxSduId;
        FrTp_TxSm_RequestTransmission(pChannel, (uint16) SduLength);
        RetVal = E_OK;
    }
    return RetVal;
}
#endif /* FRTP_LIMIT_NUMBER_CONNECTIONS */

/*
 * Stop code section declaration
 */
#define FRTP_STOP_SEC_CODE
#include <FrTp_MemMap.h>
