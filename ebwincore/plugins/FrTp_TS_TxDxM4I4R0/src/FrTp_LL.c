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
#include <PduR_FrTp.h>  /* PduR API called by FrTp */
#include <FrTp_guards.h> /* After foreign/API/Cfg, before internal/private */
#include <FrTp_Trace.h>
#include <FrTp_Priv.h>  /* data types */ /* !LINKSTO FRTP1135_1,1 */

/*======================[Local Macros]=======================================*/

/*=====================[Variable Definitions]================================*/

/*=====================[Local Function Prototypes]===========================*/

/*
 * Start code section declaration
 */
#define FRTP_START_SEC_CODE
#include <FrTp_MemMap.h>

STATIC FUNC_P2VAR(FrTp_ChannelType, AUTOMATIC, FRTP_CODE)
    FrTp_FindRxChannel(VAR(uint16,AUTOMATIC) targetAddress, VAR(uint16,AUTOMATIC) sourceAddress);

STATIC FUNC(uint8,FRTP_CODE)
    FrTp_FindConnection(uint16 targetAddress, uint16 sourceAddress);

STATIC FUNC(Std_ReturnType, FRTP_CODE) FrTp_CheckPduLength
(
    P2CONST(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) PduInfoPtr
);

STATIC FUNC(void,FRTP_CODE) FrTp_ProcessRxFrame
(
    P2CONST(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) PduInfoPtr,
    VAR(uint8,AUTOMATIC) PCI,
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel
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

/*=====================[Local Function Definitions]==========================*/

STATIC FUNC_P2VAR(FrTp_ChannelType, AUTOMATIC, FRTP_CODE)
    FrTp_FindRxChannel(VAR(uint16,AUTOMATIC) targetAddress, VAR(uint16,AUTOMATIC) sourceAddress)
{
    uint8 iChannel = 0U;
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannelCandidate = NULL_PTR;

    do
    {
        /* get pointer to right channel structure */
        CONSTP2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel = &FrTp_Channel[iChannel];

        /* get connection associated with this channel */
        const uint8 ConnectionIdx = pChannel->ConnectionIdx;

        /* if this is an unused channel, save as candidate for usage */
        if(ConnectionIdx == FRTP_CONNECTION_INVALID)
        {
            /* save free channel as candidate for allocation */
            pChannelCandidate = pChannel;
        }
        else
        {
            /* get pointer to associated connection object */
            CONSTP2CONST(FrTp_ConnectionType,AUTOMATIC,FRTP_APPL_CONST) pConnection =
                    FRTP_CFG_GET_PTR_TO_CONNECTION(ConnectionIdx);

            /* check if address information matches an existing channel */
            if((pConnection->LocalAddress == targetAddress) &&
               (pConnection->RemoteAddress == sourceAddress)
              )
            {
                pChannelCandidate = pChannel;
                break;
            }
        }
        iChannel++;
    } while(iChannel < FRTP_CHAN_NUM);

    /* return index of allocated channel */

    return pChannelCandidate;
}

STATIC FUNC(uint8,FRTP_CODE) FrTp_FindConnection
(
    uint16 targetAddress,
    uint16 sourceAddress
)
{
    uint8_least iConnection;
    uint8 Connection = FRTP_CONNECTION_INVALID;


    for(iConnection = 0U;
        iConnection < FrTp_RootPtr->nNumConnections;
        iConnection++
       )
    {
        CONSTP2CONST(FrTp_ConnectionType,AUTOMATIC,FRTP_APPL_CONST) pConnection =
                FRTP_CFG_GET_PTR_TO_CONNECTION(iConnection);

        /* check if address information matches an existing channel
           and if a FrTpRxSdu is configured for this channel */
        if((pConnection->LocalAddress == targetAddress) &&
           (pConnection->RemoteAddress == sourceAddress) &&
           (pConnection->RxSduIdx != 0xFFFFU)
          )
        {
            /* the following cast poses no problem since FrTp_RootPtr->nNumConnections
             * is also of type uint8 */
            Connection = (uint8)iConnection;
            break;
        }
    }

    /* return index of allocated channel */

    return Connection;
}

STATIC FUNC(Std_ReturnType, FRTP_CODE) FrTp_CheckPduLength
(
    P2CONST(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) PduInfoPtr
)
{
    Std_ReturnType retVal = E_NOT_OK;

    /* Address information and minimum PCI is there */
    if(PduInfoPtr->SduLength >= FrTp_Size_Header_PCI)
    {
        /* Get PCI type byte */
        const uint8 PCI = FrTp_GetPCI(PduInfoPtr->SduDataPtr);

        switch(PCI&0xF0U)
        {
            case FRTP_PCITYPE_STFU: /* fall through */
            case FRTP_PCITYPE_LF:
            {
                /* first check that SduLength is large enough that header is valid */
                if (PduInfoPtr->SduLength >= FrTp_Size_Header_STF_LF)
                {
                    const uint8 fpl = FrTp_GetFPL(PduInfoPtr->SduDataPtr);

                    /* now check that SduLength is large enough that payload is valid */
                    if (PduInfoPtr->SduLength >= (FrTp_Size_Header_STF_LF + fpl) )
                    {
                        retVal = E_OK;
                    }
                }
                break;
            }
            case FRTP_PCITYPE_CF1: /* fall through */
            case FRTP_PCITYPE_CF2: /* fall through */
            case FRTP_PCITYPE_EOB:
            {
                /* first check that SduLength is large enough that header is valid */
                if (PduInfoPtr->SduLength >= FrTp_Size_Header_CF)
                {
                    const uint8 fpl = FrTp_GetFPL(PduInfoPtr->SduDataPtr);

                    /* now check that SduLength is large enough that payload is valid */
                    if (PduInfoPtr->SduLength >= (FrTp_Size_Header_CF + fpl))
                    {
                        retVal = E_OK;
                    }
                }
                break;
            }
            default:
            {
                retVal = E_OK;
                break;
            }
        }
    }

    return retVal;
}

STATIC FUNC(void,FRTP_CODE) FrTp_ProcessRxFrame
(
    P2CONST(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) PduInfoPtr,
    VAR(uint8,AUTOMATIC) PCI,
    P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel
)
{
    const PduInfoType PduInfo = *PduInfoPtr;

    /* check the high nibble of the PCI Type field */
    switch(PCI&0xF0U)
    {
        case FRTP_PCITYPE_STFU: /* fall through */
        case FRTP_PCITYPE_LF: /* fall through */
        case FRTP_PCITYPE_CF1: /* fall through */
        case FRTP_PCITYPE_CF2: /* fall through */
        case FRTP_PCITYPE_EOB:
        {
            /* decode most parts of the FrTp header and call RxData function */
            FrTp_RxSm_RxData(pChannel,PCI&0xF0U,PCI&0x0FU,&PduInfo);
            break;
        }
        case FRTP_PCITYPE_FC:
        {
            if(PCI == FRTP_PCITYPE_FC_ABT)
            {
                    FrTp_TxSm_IndicateAndAbort(pChannel,NTFRSLT_E_ABORT);
            }
            else if(PCI == FRTP_PCITYPE_FC_ACK_RET)
            {
                /* check that SduLength is large enough that header is always valid */
                if (PduInfo.SduLength >= FrTp_Size_Header_FC_ACK_RET)
                {
                    const uint8 AckType = FrTp_GetPCIAckType(PduInfo.SduDataPtr);
                    const uint16 BufferPointer = FrTp_GetAckRet_BP(PduInfo.SduDataPtr);

                    if(AckType == FRTP_PCITYPE_ACK_RET_ACK)
                    {
                        FrTp_TxSm_ACK(pChannel);
                    }
                    else if(AckType == FRTP_PCITYPE_ACK_RET_RETRY)
                    {
                        FrTp_TxSm_RETRY(pChannel,BufferPointer);
                    }
                    else
                    {
                        FrTp_TxSm_IndicateAndAbort(pChannel,NTFRSLT_E_INVALID_FS);
                    }
                }
            }
            else if(PCI == FRTP_PCITYPE_FC_CTS)
            {
                /* check that SduLength is large enough that header is always valid */
                if (PduInfo.SduLength >= FrTp_Size_Header_FC_CTS)
                {
                    FrTp_TxSm_CTS(pChannel,
                                  FrTp_GetCTS_BfS(PduInfo.SduDataPtr),
                                  FrTp_GetCTS_BC(PduInfo.SduDataPtr)
                                 );
                }
            }
            else if(PCI == FRTP_PCITYPE_FC_OVFLW)
            {
                FrTp_TxSm_OVFLW(pChannel);
            }
            else if(PCI == FRTP_PCITYPE_FC_WT)
            {
                FrTp_TxSm_WT(pChannel);
            }
            else
            {
                FrTp_TxSm_IndicateAndAbort(pChannel,NTFRSLT_E_INVALID_FS);
            }
            break;
        }
        default:
        {
            /* ignore in case the PCIType is not supported */
            break;
        }
    }
}

/*=====================[Function Definitions]================================*/

FUNC(Std_ReturnType,FRTP_CODE) FrTp_TriggerTransmit
(
    VAR(PduIdType,AUTOMATIC) FrTxConfirmationPduId,
    P2VAR(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) PduInfoPtr
)
{
    Std_ReturnType result = E_NOT_OK;

    DBG_FRTP_TRIGGERTRANSMIT_ENTRY(FrTxConfirmationPduId,PduInfoPtr);

/* check if development error detection is enabled */
#if (FRTP_DEV_ERROR_DETECT == STD_ON)

    /* check for successfully initialized module */
    /* Report to DET and return Error in case module was not initialized before */
    if (FrTp_InitStatus == FRTP_UNINIT)
    {
        (void)Det_ReportError(FRTP_MODULE_ID,
                              FRTP_DET_GENERIC_INSTANCE_ID,
                              FRTP_TRIGGERTRANSMIT_SERVICE_ID,
                              FRTP_E_UNINIT
                             );
    }
    /* check whether that FrTxConfirmationPduId is supported */
    else if(FrTxConfirmationPduId >= FrTp_RootPtr->nNumTxPdus)
    {
        /* passed an invalid FrTxConfirmationPduId, report to DET */
        (void)Det_ReportError(FRTP_MODULE_ID,
                              FRTP_DET_GENERIC_INSTANCE_ID,
                              FRTP_TRIGGERTRANSMIT_SERVICE_ID,
                              FRTP_E_INVALID_PDU_SDU_ID
                             );
    }
    /* check whether PduInfoPtr or SduDataPtr is a NULL_PTR */
    else if( (PduInfoPtr == NULL_PTR)
           ||(PduInfoPtr->SduDataPtr == NULL_PTR)
           )
    {
        /* found a NULL_PTR, report to DET */
        (void)Det_ReportError(FRTP_MODULE_ID,
                              FRTP_DET_GENERIC_INSTANCE_ID,
                              FRTP_TRIGGERTRANSMIT_SERVICE_ID,
                              FRTP_E_NULL_PTR
                             );
    }
    else
#endif /* FRTP_DEV_ERROR_DETECT */
    {
        /* lookup internal PduIdx from FrTxConfirmationPduId */
        const uint8 iTxPdu = FRTP_CFG_GET_TXCONFLOOKUPTABLE(FrTxConfirmationPduId);

        /* get pointer to right channel structure */
        CONSTP2VAR(FrTp_ChannelType, AUTOMATIC, FRTP_VAR) pChannel =
                FrTp_RuntimeTxPdu[iTxPdu].pChannel;


        if(pChannel != NULL_PTR)
        {
            boolean success;

            if(FrTp_RuntimeTxPdu[iTxPdu].PduType != FRTP_TXPDUTYPE_FC)
            {
                /* yes it is an FrTp Tx Pdu */
                success = FrTp_TxSm_TriggerTransmit(
                                pChannel,
                                iTxPdu,
                                PduInfoPtr);
            }
            else
            {
                /* call Rx state machine function */
                success = FrTp_RxSm_TriggerTransmit(
                                pChannel,
                                PduInfoPtr);
            }

            if (success == TRUE)
            {
                result = E_OK;
            }
            else
            {
                FrTp_RuntimeTxPdu[iTxPdu].pChannel = NULL_PTR;
            }
        }
    }


    DBG_FRTP_TRIGGERTRANSMIT_EXIT(result,FrTxConfirmationPduId,PduInfoPtr);
    return result;
}


FUNC(void,FRTP_CODE) FrTp_RxIndication
(
    VAR(PduIdType,AUTOMATIC) RxPduId,
    P2VAR(PduInfoType,AUTOMATIC,FRTP_APPL_DATA) PduInfoPtr
)
{

    DBG_FRTP_RXINDICATION_ENTRY(RxPduId,PduInfoPtr);
    TS_PARAM_UNUSED(RxPduId);

/* check if development error detection is enabled */
#if (FRTP_DEV_ERROR_DETECT == STD_ON)

    /* check for successfully initialized module */
    /* Report to DET and return Error in case module was not initialized before */
    if (FrTp_InitStatus == FRTP_UNINIT)
    {
        (void)Det_ReportError(FRTP_MODULE_ID,
                              FRTP_DET_GENERIC_INSTANCE_ID,
                              FRTP_RXINDICATION_SERVICE_ID,
                              FRTP_E_UNINIT
                             );
    }
    /* check whether PduInfoPtr or SduDataPtr is a NULL_PTR */
    else if(
        (PduInfoPtr == NULL_PTR)
        ||
        (PduInfoPtr->SduDataPtr == NULL_PTR)
      )
    {
        /* found a NULL_PTR, report to DET */
        (void)Det_ReportError(FRTP_MODULE_ID,
                              FRTP_DET_GENERIC_INSTANCE_ID,
                              FRTP_RXINDICATION_SERVICE_ID,
                              FRTP_E_NULL_PTR
                             );
    }
    else
#endif /* FRTP_DEV_ERROR_DETECT */

    {
        /* create a local copy of the data pointer */
        const PduInfoType PduInfo = *PduInfoPtr;

        /* check that SduLength is large enough that access to address and PCI is always valid */
        const Std_ReturnType retVal = FrTp_CheckPduLength(PduInfoPtr);

        if(retVal == E_OK)
        {
            /* create target address contained in frame address information field */
            const uint16 targetAddress = FrTp_GetAI_Target(PduInfo.SduDataPtr);

            /* create source address contained in frame address information field */
            const uint16 sourceAddress = FrTp_GetAI_Source(PduInfo.SduDataPtr);

            boolean IgnoreFrame = FALSE;

            /* get PCI type byte */
            const uint8 PCI = FrTp_GetPCI(PduInfo.SduDataPtr);

            P2VAR(FrTp_ChannelType,AUTOMATIC,FRTP_VAR) pChannel;

            /* enter exclusive area */
            SchM_Enter_FrTp_SCHM_FRTP_EXCLUSIVE_AREA_0();

            /* get a channel for this AI */
            pChannel = FrTp_FindRxChannel(targetAddress,sourceAddress);

            if( pChannel != NULL_PTR )
            {
                /* general check */
                if(pChannel->ConnectionIdx == FRTP_CONNECTION_INVALID)
                {
                    if((PCI&0xFEU) == FRTP_PCITYPE_STFU)
                    {
                        const uint8 ConnectionIdx = FrTp_FindConnection(targetAddress,sourceAddress);

                        if(ConnectionIdx != FRTP_CONNECTION_INVALID)
                        {
                            pChannel->ConnectionIdx = ConnectionIdx;
                        }
                        else
                        {
                            IgnoreFrame = TRUE;
                        }

                    }
                    else
                    {
                        IgnoreFrame = TRUE;
                    }
                }
            }
            else
            {
                IgnoreFrame = TRUE;
            }

            /* exit exclusive area */
            SchM_Exit_FrTp_SCHM_FRTP_EXCLUSIVE_AREA_0();

            /* Check whether received frame has to be processed */
            if(IgnoreFrame == FALSE)
            {
                FrTp_ProcessRxFrame(PduInfoPtr, PCI, pChannel);
            }
        }
    }

    DBG_FRTP_RXINDICATION_EXIT(RxPduId,PduInfoPtr);
}



FUNC(void,FRTP_CODE) FrTp_TxConfirmation
(
    VAR(PduIdType,AUTOMATIC) FrTxConfirmationPduId
)
{
    DBG_FRTP_TXCONFIRMATION_ENTRY(FrTxConfirmationPduId);

    /* check if development error detection is enabled */
#if (FRTP_DEV_ERROR_DETECT == STD_ON)

    /* check for successfully initialized module */
    /* Report to DET and return Error in case module was not initialized before */

    if (FrTp_InitStatus == FRTP_UNINIT)
    {
        (void)Det_ReportError(FRTP_MODULE_ID,
                              FRTP_DET_GENERIC_INSTANCE_ID,
                              FRTP_TXCONFIRMATION_SERVICE_ID,
                              FRTP_E_UNINIT
                             );
    }
    /* check whether that FrTxConfirmationPduId is supported */
    else if(FrTxConfirmationPduId >= FrTp_RootPtr->nNumTxPdus)
    {
        /* passed an invalid FrTxConfirmationPduId, report to DET */
        (void)Det_ReportError(FRTP_MODULE_ID,
                              FRTP_DET_GENERIC_INSTANCE_ID,
                              FRTP_TXCONFIRMATION_SERVICE_ID,
                              FRTP_E_INVALID_PDU_SDU_ID
                             );
    }
    else
#endif /* FRTP_DEV_ERROR_DETECT */

    {
        /* lookup internal PduIdx from FrTxConfirmationPduId */
        const uint8 iTxPdu = FRTP_CFG_GET_TXCONFLOOKUPTABLE(FrTxConfirmationPduId);

        /* get pointer to right channel structure */
        CONSTP2VAR(FrTp_ChannelType, AUTOMATIC, FRTP_VAR) pChannel =
                FrTp_RuntimeTxPdu[iTxPdu].pChannel;

        if(pChannel != NULL_PTR)
        {
            if(FrTp_RuntimeTxPdu[iTxPdu].PduType != FRTP_TXPDUTYPE_FC)
            {
                /* yes it is an FrTp Tx Pdu */
                FrTp_TxSm_TransmitConfirmation(pChannel,
                                               iTxPdu
                                              );
            }
            else
            {
                /* call Rx state machine function */
                FrTp_RxSm_TransmitConfirmation(pChannel,
                                               iTxPdu
                                              );
            }
        }
    }

    DBG_FRTP_TXCONFIRMATION_EXIT(FrTxConfirmationPduId);
}

/*
 * Stop code section declaration
 */
#define FRTP_STOP_SEC_CODE
#include <FrTp_MemMap.h>
