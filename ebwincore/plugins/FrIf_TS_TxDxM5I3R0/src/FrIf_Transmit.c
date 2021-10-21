/**
 * \file
 *
 * \brief AUTOSAR FrIf
 *
 * This file contains the implementation of the AUTOSAR
 * module FrIf.
 *
 * \version 5.3.25
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*
* Misra-C:2012 Deviations:
*
* MISRAC2012-1) Deviated Rule: 11.5 (required)
* A conversion should not be performed from pointer to void into pointer to object.
*
* Reason:
* For efficiency reasons a uint32 array is allocated while certain operatioons access
* it byte-wise. In these cases there is a cast to void* in order to
* satisfy the compilers.
*
*/

/*======================[Include Section]=====================================*/

#include <SchM_FrIf.h>
#include <FrIf_guards.h> /* Include define guards */
#include <FrIf_Trace.h>
#include <FrIf_Priv.h>

/*======================[Local Macros]========================================*/

#if (TS_MERGED_COMPILE == STD_OFF)
#define FRIF_START_SEC_CODE
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */
#endif /* TS_MERGED_COMPILE */

/**
 * \brief  Transmit Service
 *
 * \param FrIf_TxPduId (in)         Id of Pdu to be transmitted.
 * \param FrIf_PduInfoPtr (in)      Description of Pdu content to be transmitted.
 *
 * \retval  E_OK               Function serviced successfully.
 * \retval  E_NOT_OK           Function execution failed.
 */

FUNC(Std_ReturnType,FRIF_CODE) FrIf_Transmit
    (
        PduIdType FrIf_TxPduId,
        P2CONST(PduInfoType,AUTOMATIC,FRIF_APPL_DATA) FrIf_PduInfoPtr
    )
{
    Std_ReturnType RetValue = E_NOT_OK;

    DBG_FRIF_TRANSMIT_ENTRY(FrIf_TxPduId,FrIf_PduInfoPtr);

/* check if development error detection is enabled */
#if (FRIF_DEV_ERROR_DETECT == STD_ON)

    /* check for successfully initialized module */
    /* Report to DET and return Error in case module was not initialized before */
    if(FrIf_ModuleInitialized == FRIF_UNINIT)
    {
        /* Report to DET */
        FrIf_Det_ReportError(FRIF_TRANSMIT_SERVICE_ID, FRIF_E_NOT_INITIALIZED);
    }

    else if(FrIf_TxPduId >= FrIf_RootConfigPtr->FrIf_TxPduNum)
    {
        /* Report to DET */
        FrIf_Det_ReportError(FRIF_TRANSMIT_SERVICE_ID, FRIF_E_INV_TXPDUID);
    }

    else
#endif  /* FRIF_DEV_ERROR_DETECT */
    {
        /* get pointer to Pdu configuration */
        CONSTP2CONST(FrIf_TxPduType,AUTOMATIC,FRIF_APPL_CONST)
            PduPtr = &FRIF_GET_CONFIG_ADDR(FrIf_TxPduType,
                        FrIf_RootConfigPtr->FrIf_TxPduRef)[FrIf_TxPduId];

        uint8 State = (uint8) FRIF_STATE_OFFLINE;

#if (FRIF_SINGLE_CLST_OPT_ENABLE == STD_OFF)
        /* check if at least a single cluster is ONLINE */
        uint8_least FrIf_ClstIdx;
        for( (FrIf_ClstIdx = 0U);
             ((FrIf_ClstIdx < FrIf_RootConfigPtr->FrIf_ClstNum) && (State == (uint8)FRIF_STATE_OFFLINE));
             (FrIf_ClstIdx++))
#endif
        {
            /* Read FrIf_State in an atomic fashion */
            TS_AtomicAssign8(State,FrIf_ClstRuntimeData[FRIF_CLSTIDX].FrIf_State);
        }

        if((State == (uint8)FRIF_STATE_ONLINE) && (PduPtr->isDummyPdu == 0x0U))
        {

#if (FRIF_IMMEDIATE_TX_ENABLE == STD_ON)

            if((PduPtr->TxType_TxCntLimit_CtrlIdx & FRIF_TXTYPE_MASK)
               != FRIF_TXTYPE_DECOUPLED)
            {

#if (FRIF_SINGLE_CTRL_OPT_ENABLE == STD_OFF)
                const uint8 FrIf_CtrlIdx =
                    (uint8)((PduPtr->TxType_TxCntLimit_CtrlIdx) & FRIF_CTRLIDX_MASK);
#endif /* FRIF_SINGLE_CTRL_OPT_ENABLE */

#if FRIF_DEV_ERROR_DETECT == STD_ON

                if (NULL_PTR == FrIf_PduInfoPtr)
                {
                    /* Report to DET */
                    FrIf_Det_ReportError(FRIF_TRANSMIT_SERVICE_ID, FRIF_E_INV_POINTER);
                }
                else if (NULL_PTR == FrIf_PduInfoPtr->SduDataPtr)
                {
                    /* Report to DET */
                    FrIf_Det_ReportError(FRIF_TRANSMIT_SERVICE_ID, FRIF_E_INV_POINTER);
                }

                else
#endif /* FRIF_DEV_ERROR_DETECT */
                {

#if (FRIF_FR_DRIVER_AUTOSAR_VERSION == FRIF_FR_ASR_44_SUPPORT)
                    P2VAR(Fr_SlotAssignmentType,AUTOMATIC,FRIF_VAR) SlotAssignmentPtr = NULL_PTR;
#endif

#if (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON)
                    Fr_SlotAssignmentType SlotAssignment;

                    SlotAssignment.SlotId = 0U;
                    SlotAssignment.Cycle = 0U;
                    SlotAssignment.channelId = FR_CHANNEL_A;

                    SlotAssignmentPtr = &SlotAssignment;
#endif /* (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON) */

                    {
#if (FRIF_FR_DRIVER_AUTOSAR_VERSION == FRIF_FR_ASR_44_SUPPORT)
                        /* !LINKSTO FrIf.EB.BusmirrorSupport.FrIfTransmitImmediate.FrTransmitTxLPdu,1 */
                        const Std_ReturnType RetCode =
                            FrIf_Call_Fr_Arg5(FRIF_FR_INSTANCE_IDX,TransmitTxLPdu,
                                   FRIF_FR_CTRLIDX,
                                   PduPtr->DynLenEnable_LPduIdx,
                                   FrIf_PduInfoPtr->SduDataPtr,
                                   (uint8)FrIf_PduInfoPtr->SduLength,
                                   SlotAssignmentPtr
                         );
#else
                        const Std_ReturnType RetCode =
                            FrIf_Call_Fr_Arg4(FRIF_FR_INSTANCE_IDX,TransmitTxLPdu,
                                   FRIF_FR_CTRLIDX,
                                   PduPtr->DynLenEnable_LPduIdx,
                                   FrIf_PduInfoPtr->SduDataPtr,
                                   (uint8)FrIf_PduInfoPtr->SduLength
                         );
#endif /* FRIF_FR_DRIVER_AUTOSAR_VERSION == FRIF_FR_ASR_44_SUPPORT */

                        if(E_OK == RetCode)
                        {

#if ( (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON) && (FRIF_SINGLE_CLST_OPT_ENABLE == STD_OFF) )
                            FrIf_ClstIdx = FRIF_GET_CONFIG_ADDR(FrIf_CtrlType, FrIf_RootConfigPtr->FrIf_CtrlRef)[FRIF_FR_CTRLIDX].ClstIdx;
#endif /* (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON) && (FRIF_SINGLE_CLST_OPT_ENABLE == STD_OFF) */

                            /* If Pdu requires tx confirmation then pending bit must be set */
                            if(FrIf_TxPduId >= FrIf_RootConfigPtr->FrIf_TxPduNoConfNum )
                            {
                                /* if a transmission confirmation accesses this data at the same time,
                                 * it will be corrupted!
                                 */
                                /* That's why this critical area is protected */
                                SchM_Enter_FrIf_SCHM_FRIF_TX_ADMINISTRATION();

                                /* Deviation MISRAC2012-1 */
                                ((P2VAR(uint8,AUTOMATIC,FRIF_VAR))(P2VAR(void,AUTOMATIC,FRIF_VAR)) FrIf_Mem)[FrIf_TxPduId]
                                  |= FRIF_PENDINGBIT_SETBITMASK;

                                /* Leave critical area */
                                SchM_Exit_FrIf_SCHM_FRIF_TX_ADMINISTRATION();
                            }

#if (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON)
                            if(FrIf_ClstRuntimeData[FRIF_CLSTIDX].BusmirroringEnabled == TRUE)
                            {
                                /* !LINKSTO FrIf.EB.BusmirrorSupport.FrIfTransmitImmediate.MirrorReportFlexRayFrame,1 */
                                Mirror_ReportFlexRayFrame
                                    (
                                        FRIF_FR_CTRLIDX,
                                        SlotAssignment.SlotId,
                                        SlotAssignment.Cycle,
                                        SlotAssignment.channelId,
                                        FrIf_PduInfoPtr,
                                        FALSE
                                    );
                            }
#endif /* (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON) */

                            RetValue = E_OK;
                        }
                    }
                }
            }
            /* this is a decoupled Pdu */
            else
#endif /* FRIF_IMMEDIATE_TX_ENABLE */
            {
                /* for decoupled transmission, FrIf_PduInfoPtr is not used */
                TS_PARAM_UNUSED(FrIf_PduInfoPtr);

                /*
                 * just increment the TxCounter but the counter should not
                 * exceed MaxCounter.
                 * The MCG ensures that the transmit counter values are placed in the beginning
                 * of the FrIf memory w.r.t the tx-pdu id.
                 */

                SchM_Enter_FrIf_SCHM_FRIF_TX_ADMINISTRATION();

                /* Deviation MISRAC2012-1 */
                if((((P2VAR(uint8,AUTOMATIC,FRIF_VAR))(P2VAR(void,AUTOMATIC,FRIF_VAR)) FrIf_Mem)[FrIf_TxPduId] & FRIF_TXCNT_MASK)
                            < FRIF_GET_CNTLIMIT(PduPtr->TxType_TxCntLimit_CtrlIdx))
                {
                    /* increment transmission request counter */
                    /* Deviation MISRAC2012-1 */
                    ((P2VAR(uint8,AUTOMATIC,FRIF_VAR))(P2VAR(void,AUTOMATIC,FRIF_VAR)) FrIf_Mem)[FrIf_TxPduId]++;

                    RetValue = E_OK;
                }
                SchM_Exit_FrIf_SCHM_FRIF_TX_ADMINISTRATION();
            }
        }
    }

    DBG_FRIF_TRANSMIT_EXIT(RetValue,FrIf_TxPduId,FrIf_PduInfoPtr);
    return RetValue;
}


/* stop code section declaration */
#if (TS_MERGED_COMPILE == STD_OFF)
#define FRIF_STOP_SEC_CODE
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */
#endif /* TS_MERGED_COMPILE */

