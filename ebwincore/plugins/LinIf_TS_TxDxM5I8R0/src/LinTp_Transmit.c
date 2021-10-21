/**
 * \file
 *
 * \brief AUTOSAR LinIf
 *
 * This file contains the implementation of the AUTOSAR
 * module LinIf.
 *
 * \version 5.8.27
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]=============================================*/

#include <LinIf_Trace.h>

#include <LinIf_Cfg.h>        /* Generated configuration */
#include <TSAutosar.h>        /* EB specific standard types */

#if (LINIF_TP_SUPPORTED == STD_ON)

#define LINIF_NO_PBCFG_REQUIRED
#include <LinIf.h>            /* Module library API */
#include <LinIf_Int.h>        /* LinIf internal data */
#include <LinTp_Int.h>        /* LinTp internal data */
#include <SchM_LinIf.h>       /* for protecting exclusive areas */

#if ((LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON))
#include <Det.h>              /* Default Error Tracer */
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON) */

#include <PduR_LinTp.h>       /* PDU Router LIN-TP Interface */

/*==================[macros]=================================================*/

/*==================[external function definitions]==========================*/

#if (TS_MERGED_COMPILE == STD_OFF)
/* AUTOSAR Memory Mapping - start section for code */
#define LINIF_START_SEC_CODE
#include <LinIf_MemMap.h>
#endif /* TS_MERGED_COMPILE */

/** \brief Start a TP transmission
 *
 * This function starts a LinTP-Transmission if there is currently no other
 * transmission ongoing on the channel identified by the PDU Id.
 *
 * \param[in] LinTpTxSduId The PDU Id of the message to be sent
 * \param[in] LinTpTxInfoPtr A PduInfoType to pass the length of the message
 */
FUNC(Std_ReturnType, LINIF_CODE) LinTp_Transmit
(
    PduIdType LinTpTxSduId,
    P2CONST(PduInfoType, AUTOMATIC, LINIF_APPL_DATA) LinTpTxInfoPtr
)
{
    Std_ReturnType RetVal = E_NOT_OK;

    DBG_LINTP_TRANSMIT_ENTRY(LinTpTxSduId,LinTpTxInfoPtr);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    /* check DET errors first */
    if (LINTP_UNINIT_ACCESS)
    {
        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00376_1.LINIF_E_UNINIT,1 */
        (void)Det_ReportError(LINIF_MODULE_ID,
                              LINIF_INSTANCE_ID,
                              LINTP_API_TRANSMIT,
                              LINIF_E_UNINIT
                             );
    }

    else if ( LinTpTxSduId >= LinTp_ConfigPtr->LinTpNumberOfTxNSdu )
    {
        (void)Det_ReportError(LINIF_MODULE_ID,
                              LINIF_INSTANCE_ID,
                              LINTP_API_TRANSMIT,
                              LINIF_E_PARAMETER
                             );
    }

    else if ( LinTpTxInfoPtr == NULL_PTR )
    {
        (void)Det_ReportError(LINIF_MODULE_ID,
                              LINIF_INSTANCE_ID,
                              LINTP_API_TRANSMIT,
                              LINIF_E_PARAMETER_POINTER
                             );
    }

    /* check maximum message length */
    else if ( LinTpTxInfoPtr->SduLength > LINTP_MAX_MSG_LENGTH )
    {
        (void)Det_ReportError(LINIF_MODULE_ID,
                              LINIF_INSTANCE_ID,
                              LINTP_API_TRANSMIT,
                              LINIF_E_PARAMETER
                             );
    }

    else
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */
    {
        CONSTP2CONST(LinTpTxNSduType, TYPEDEF, LINIF_APPL_CONST) pTpTxConfig =
            LINTP_CFG_GET_TXNSDU;

#if ((LINIF_MASTER_SUPPORTED == STD_ON) && (LINIF_DEV_ERROR_DETECT == STD_ON))
        /* functional requests are always SF, check if message fit into SF */
        if ( (pTpTxConfig[LinTpTxSduId].LinTpTxNSduNad == LINIF_FUNCTIONAL_NAD) &&
                (LinTpTxInfoPtr->SduLength > LINTP_PCI_DATA_LENGTH)
              )
        {
          (void)Det_ReportError(LINIF_MODULE_ID,
                                LINIF_INSTANCE_ID,
                                LINTP_API_TRANSMIT,
                                LINIF_E_PARAMETER
                               );
        }
        else
#endif /* (LINIF_MASTER_SUPPORTED == STD_ON) && (LINIF_DEV_ERROR_DETECT == STD_ON) */
        {
            const uint8 Channel = pTpTxConfig[LinTpTxSduId].LinIfChannel;
            PduIdType SudIdCanceled = LINTP_INVALID_SDU_ID;

            CONSTP2VAR(LinIf_ChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannel =
            &LinIf_ChannelInfo[Channel];

            CONSTP2VAR(LinTp_ChannelInfoType, AUTOMATIC, LINIF_VAR) pTpData =
                        &LinTp_ChannelInfo[Channel];

            /* !LINKSTO LinTp.EB.LinTp_Transmit.CHANNEL_NOT_OPERATIONAL,1 */
            if(pIfChannel->cFSM == LINIF_CHANNEL_OPERATIONAL)
            {

                SchM_Enter_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();
                {
#if (LINIF_MASTER_SUPPORTED == STD_ON)
#if (LINIF_SLAVE_SUPPORTED == STD_ON)
                    const uint8 newNAD = (Channel < LinIf_ConfigPtr->NumChannelsMaster) ? pTpTxConfig[LinTpTxSduId].LinTpTxNSduNad : LinTp_GetConfNad(Channel);
#else
                    const uint8 newNAD = pTpTxConfig[LinTpTxSduId].LinTpTxNSduNad;
#endif /* LINIF_SLAVE_SUPPORTED == STD_OFF */
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

                    uint8 ChannelFlags;

                    TS_AtomicAssign8(ChannelFlags, pTpData->ChannelFlags);

#if (LINIF_MASTER_SUPPORTED == STD_ON)
                    if(newNAD == LINIF_FUNCTIONAL_NAD)
                    {
                        const LinTp_FuncStates_Type FuncState = pTpData->FuncChannelInfo.State;
                        boolean FuncTrans;

                        FuncTrans = (ChannelFlags & LINTP_FLAG_FUNC_TRANSMIT_REQUESTED) ==
                            LINTP_FLAG_FUNC_TRANSMIT_REQUESTED;

                        /* request a functional transmission, functional request can not abort
                           an ongoing functional transmission */
                        if((!FuncTrans) && (FuncState == LINTP_FUNC_STATE_IDLE))
                        {
                            /* indicates the main function to initiate a functional transmission */
                            ChannelFlags |= LINTP_FLAG_FUNC_TRANSMIT_REQUESTED;
                            TS_AtomicAssign8(pTpData->ChannelFlags, ChannelFlags);
                            pTpData->FuncChannelInfo.SduId = LinTpTxSduId;
                            pTpData->FuncChannelInfo.MsgLengthTotal = (uint8) LinTpTxInfoPtr->SduLength;
                            RetVal = E_OK;
                        }
                    }
                    else
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */
                    {
                        /* store transmit request and transmit information in an atomic fashion */

                        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00616.SRF,1,
                                    LinIf.ASR20-11.SWS_LinIf_00616.MRF,1
                         */
                        if((ChannelFlags&LINTP_FLAG_PHYS_TRANSMIT_REQUESTED) == LINTP_FLAG_PHYS_TRANSMIT_REQUESTED)
                        {
                            SudIdCanceled = pTpData->PhysChannelInfo.PendingSduId;
                        }

                        ChannelFlags |= LINTP_FLAG_PHYS_TRANSMIT_REQUESTED;
                        pTpData->PhysChannelInfo.PendingSduId = LinTpTxSduId;
                        pTpData->PhysChannelInfo.PendingMsgLength = (uint16)LinTpTxInfoPtr->SduLength;

#if(LINIF_SLAVE_SUPPORTED == STD_ON)
                        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00800_1,1,
                                    LinIf.ASR20-11.SWS_LinIf_00800_2,1
                         */
                        if(Channel >= LinIf_ConfigPtr->NumChannelsMaster)
                        {
                          pTpData->PhysChannelInfo.Timer = pTpTxConfig[LinTpTxSduId].LinTpNas;
                        }
#endif /* LINIF_SLAVE_SUPPORTED == STD_ON */

                        TS_AtomicAssign8(pTpData->ChannelFlags, ChannelFlags);

                        RetVal = E_OK;
                    }
                }
                SchM_Exit_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

                if(SudIdCanceled != LINTP_INVALID_SDU_ID)
                {
                    /* Indicate the PduR that a functional transmission was stopped */
                    PduR_LinTpTxConfirmation(pTpTxConfig[SudIdCanceled].PduRDestPduHandleId, NTFRSLT_E_NOT_OK);
                }
            }
        }
    }

    DBG_LINTP_TRANSMIT_EXIT(RetVal,LinTpTxSduId,LinTpTxInfoPtr);

    return RetVal;
}

#if (TS_MERGED_COMPILE == STD_OFF)
/* AUTOSAR Memory Mapping - end section for code */
#define LINIF_STOP_SEC_CODE
#include <LinIf_MemMap.h>
#endif /* TS_MERGED_COMPILE */

#else /* (LINIF_TP_SUPPORTED == STD_ON) */

/* prevent empty translation unit in case LinIf_Merged.c is not used */
#if (TS_MERGED_COMPILE == STD_OFF)

#include <TSCompiler.h>
TS_PREVENTEMPTYTRANSLATIONUNIT

#endif /* TS_MERGED_COMPILE */

#endif /* (LINIF_TP_SUPPORTED == STD_ON) */
/*==================[end of file]============================================*/
