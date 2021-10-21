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
* For efficiency reasons a uint32 array is allocated while certain operations access
* it byte-wise. In these cases there is a cast to void* in order to
* satisfy the compilers.
*
*/

/*======================[Include Section]=====================================*/

#include <TSMem.h> /* TS_memset */
#include <SchM_FrIf.h>
#include <FrIf_guards.h> /* Include define guards */
#include <FrIf_Trace.h>
#include <FrIf_Priv.h>

/*======================[Local Macros]========================================*/


#define FRIF_DECRX_VALID_BYTE_OFFSET (0U)  /* offset within dec. Rx memory that holds the valid byte */
#define FRIF_DECRX_PAYLOAD_OFFSET (1U)     /* offset within dec. Rx memory that holds payload */

#define FRIF_DECRX_INVALID (0x00U)          /* Invalid decoupled Rx frame content */

/* temporary buffer length for dec. transmission and dec. reception */
#define FRIF_TMPPAYLOADBUFFER_U32_LENGTH (256U/4U)
#define FRIF_TXSTATUS_MAX_PDUS_PER_FRAME (256U)
#define FRIF_TXSTATUS_WORD_SIZE (32U)
#define FRIF_TXSTATUS_WORD_MASK (32U-1U)
#define FRIF_TXSTATUS_NUM_WORDS (256U/32U)


/*======================[Local Types]=========================================*/

/*======================[Local Functions]=====================================*/

#if (TS_MERGED_COMPILE == STD_OFF)
#define FRIF_START_SEC_CODE
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */
#endif /* TS_MERGED_COMPILE */


#if (FRIF_JOBLIST_PREPARE_LPDU_ENABLE == STD_ON)

/**
 * \brief  'Prepare Frame' job operation implementation
 */
STATIC FUNC(void,FRIF_CODE) FrIf_PrepareFrame
    (
        CONSTP2CONST(FrIf_ComOpType, AUTOMATIC, FRIF_APPL_CONST) FrIf_OperationPtr
    );

STATIC FUNC(void,FRIF_CODE) FrIf_PrepareFrame
    (
        CONSTP2CONST(FrIf_ComOpType, AUTOMATIC, FRIF_APPL_CONST) FrIf_OperationPtr
    )
{

/* if no single controller implementation */
#if (FRIF_SINGLE_CTRL_OPT_ENABLE == STD_OFF)
    /* obtain actual controller index */
    const uint8 FrIf_CtrlIdx = FrIf_OperationPtr->FrIfCtrlIdx;

#endif /* FRIF_SINGLE_CTRL_OPT_ENABLE */


    /* call Fr-API service */
    (void)FrIf_Call_Fr_Arg2(FRIF_FR_INSTANCE_IDX,PrepareLPdu,FRIF_FR_CTRLIDX,FrIf_OperationPtr->LPduIdx);

}

#endif /* FRIF_JOBLIST_PREPARE_LPDU_ENABLE */


#if (FRIF_DECOUPLED_RX_ENABLE == STD_ON)

/**
 * \brief  'Receive Indication' job operation implementation
 */
STATIC FUNC(void,FRIF_CODE) FrIf_ReceiveIndication
    (
        CONSTP2CONST(FrIf_ComOpType, AUTOMATIC, FRIF_APPL_CONST) FrIf_OperationPtr
    );

/**
 * \brief  'Receive and Store' job operation implementation
 */
STATIC FUNC(void,FRIF_CODE) FrIf_ReceiveAndStore
    (
        CONSTP2CONST(FrIf_ComOpType, AUTOMATIC, FRIF_APPL_CONST) FrIf_OperationPtr
    );


STATIC FUNC(void,FRIF_CODE) FrIf_ReceiveIndication
    (
        CONSTP2CONST(FrIf_ComOpType, AUTOMATIC, FRIF_APPL_CONST) FrIf_OperationPtr
    )
{

    /* obtain pointer to configuration data */
    CONSTP2CONST(FrIf_DecRxLPduType, AUTOMATIC,FRIF_APPL_CONST) DecoupledRxLPduPtr =
        FRIF_GET_CONFIG_ADDR(FrIf_DecRxLPduType, FrIf_OperationPtr->ContainerRef);

    /* get byte position of valid information */
    /* Deviation MISRAC2012-1 <+2> */
    CONSTP2VAR(uint8,AUTOMATIC,FRIF_VAR) pValidByte =
        &((P2VAR(uint8,AUTOMATIC,FRIF_VAR))(P2VAR(void,AUTOMATIC,FRIF_VAR)) FrIf_Mem)
          [DecoupledRxLPduPtr->PBRamBufRef + FRIF_DECRX_VALID_BYTE_OFFSET];

    /* get LPdu pointer */
    /* Deviation MISRAC2012-1 <+2> */
    CONSTP2VAR(uint8,AUTOMATIC,FRIF_VAR) LPduPtr =
        &((P2VAR(uint8,AUTOMATIC,FRIF_VAR))(P2VAR(void,AUTOMATIC,FRIF_VAR))(P2VAR(void,AUTOMATIC,FRIF_VAR)) FrIf_Mem)
          [DecoupledRxLPduPtr->PBRamBufRef + FRIF_DECRX_PAYLOAD_OFFSET];

    /* get payload length of received frame */
    uint8 RxLength;

    TS_AtomicAssign8(RxLength,*pValidByte);



    /* check for a valid frame */
    if(RxLength != FRIF_DECRX_INVALID)
    {
        /* create Pdu-iterator */
        uint16_least iPdu;

        /* iterate over Pdus contained in Frame */
        for (iPdu = 0U; iPdu < (uint16_least)FrIf_OperationPtr->PduPositionNum_OwnerIdx; iPdu++)
        {

            /* obtain pointer to configuration data */
            CONSTP2CONST(FrIf_PduPositionType, AUTOMATIC,FRIF_APPL_CONST) PduPositionPtr =
                &FRIF_GET_CONFIG_ADDR(FrIf_PduPositionType,
                                      DecoupledRxLPduPtr->DecRxPduPositionRef)[iPdu];

#if FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE == STD_ON
            /* check if update bit and payload start offset is within received payload */
            /* otherwise, ignore this Pdu */
            if((PduPositionPtr->UB_ByteOffset < RxLength) && (PduPositionPtr->PduOffset < RxLength))
#endif /* FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE */

            {
                /* obtain UB Bit position */
                const uint8 ubBitPos =
                    FRIF_UB_GETBITPOS(PduPositionPtr->UB_BitPos_OwnerIdx);

                if((ubBitPos == FRIF_UPDATEBIT_NONE) ||
                   ((LPduPtr[PduPositionPtr->UB_ByteOffset] & FRIF_UB_GETBITMASK(ubBitPos)) != 0U))
                {
                    /* create PduInfoType pduInfo */
                    PduInfoType pduInfo;

                    /* get owner Idx */
                    const uint8 OwnerIdx = FRIF_GET_OWNERIDX(PduPositionPtr->UB_BitPos_OwnerIdx);

                    /* set pointer to Pdu payload */
                    pduInfo.SduDataPtr = &LPduPtr[PduPositionPtr->PduOffset];

                    /* get length of Pdu */
#if FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE == STD_ON
                    if((RxLength - PduPositionPtr->PduOffset) < PduPositionPtr->PduLength)
                    {
                      const uint8 DynPduLength = RxLength - PduPositionPtr->PduOffset;
                      pduInfo.SduLength = (PduLengthType) DynPduLength;
                    }
                    else
#endif /* FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE */
                    {
                      pduInfo.SduLength = PduPositionPtr->PduLength;
                    }
#if FRIF_UL_ASR43_API_ENABLE == STD_ON
                    if(OwnerIdx >= FrIf_FirstASR43OwnerIdx)
                    {
                      /* provide reception indication to upper layer */
                      FrIf_PduOwnerFuncASR43Ptr[OwnerIdx - FrIf_FirstASR43OwnerIdx]
                          .FrIfRxIndication((PduIdType)PduPositionPtr->PduIdx, &pduInfo);
                    }
                    else
#endif /* FRIF_UL_ASR43_API_ENABLE == STD_ON */
                    {
                      /* provide reception indication to upper layer */
                      FrIf_PduOwnerFuncASR40Ptr[OwnerIdx]
                          .FrIfRxIndication((PduIdType)PduPositionPtr->PduIdx, &pduInfo);
                    }
                }
            }
        }

        /* clear valid information */
        TS_AtomicAssign8(*pValidByte,FRIF_DECRX_INVALID);
    }
}


STATIC FUNC(void,FRIF_CODE) FrIf_ReceiveAndStore
    (
        CONSTP2CONST(FrIf_ComOpType, AUTOMATIC, FRIF_APPL_CONST) FrIf_OperationPtr
    )
{

/* if no single controller implementation */
#if (FRIF_SINGLE_CTRL_OPT_ENABLE == STD_OFF)
    /* obtain actual controller index */
    const uint8 FrIf_CtrlIdx = FrIf_OperationPtr->FrIfCtrlIdx;
#endif /* FRIF_SINGLE_CTRL_OPT_ENABLE */

    /* variable for obtaining RX-Status */
    /* initialize with default error behaviour (nothing received) because
     * it's value stays untouched if reception fails by any reason
     */
    Fr_RxLPduStatusType RxStatus = FR_NOT_RECEIVED;

    /* get Received payload length here */
    uint8 RxLength;

    /* obtain pointer to configuration data */
    CONSTP2CONST(FrIf_DecRxLPduType, AUTOMATIC,FRIF_APPL_CONST) DecoupledRxLPduPtr =
        FRIF_GET_CONFIG_ADDR(FrIf_DecRxLPduType, FrIf_OperationPtr->ContainerRef);

    /* get LPdu pointer */
    /* MCG ensures that byte access is uint32 aligned */
    /* Deviation MISRAC2012-1 <+2> */
    CONSTP2VAR(uint32,AUTOMATIC,FRIF_VAR) LPduPtr =
        &(((P2VAR(uint32,AUTOMATIC,FRIF_VAR))(P2VAR(void,AUTOMATIC,FRIF_VAR)) FrIf_Mem)[(DecoupledRxLPduPtr->PBRamBufRef + FRIF_DECRX_PAYLOAD_OFFSET)/4U]);

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

#if (FRIF_FR_DRIVER_AUTOSAR_VERSION == FRIF_FR_ASR_44_SUPPORT)
    /* !LINKSTO FrIf.EB.BusmirrorSupport.ReceiveAndStore.FrReceiveRxLPdu,1 */
    /* Receive frame from Fr */
    /* don't evaluate return code, since default behaviour is already
     * initialized
     */
    /* Deviation MISRAC2012-1 <+4> */
    (void)FrIf_Call_Fr_Arg6(FRIF_FR_INSTANCE_IDX,ReceiveRxLPdu,
            FRIF_FR_CTRLIDX,
            FrIf_OperationPtr->LPduIdx,
            (P2VAR(uint8,AUTOMATIC,FRIF_VAR))(P2VAR(void,AUTOMATIC,FRIF_VAR)) LPduPtr,
            &RxStatus,
            &RxLength,
            SlotAssignmentPtr
         );
#else
    /* Receive frame from Fr */
    /* don't evaluate return code, since default behaviour is already
     * initialized
     */
    /* Deviation MISRAC2012-1 <+4> */
    (void)FrIf_Call_Fr_Arg5(FRIF_FR_INSTANCE_IDX,ReceiveRxLPdu,
            FRIF_FR_CTRLIDX,
            FrIf_OperationPtr->LPduIdx,
            (P2VAR(uint8,AUTOMATIC,FRIF_VAR))(P2VAR(void,AUTOMATIC,FRIF_VAR)) LPduPtr,
            &RxStatus,
            &RxLength
         );
#endif /* FRIF_FR_DRIVER_AUTOSAR_VERSION == FRIF_FR_ASR_44_SUPPORT */

    /* proceed further only, if we received a frame */
    if(RxStatus != FR_NOT_RECEIVED)
    {

        /* get byte position of valid information */
        /* Deviation MISRAC2012-1 <+2> */
        CONSTP2VAR(uint8,AUTOMATIC,FRIF_VAR) pValidByte =
            &((P2VAR(uint8,AUTOMATIC,FRIF_VAR))(P2VAR(void,AUTOMATIC,FRIF_VAR)) FrIf_Mem)[DecoupledRxLPduPtr->PBRamBufRef + FRIF_DECRX_VALID_BYTE_OFFSET];

        /* mark content as valid */
        TS_AtomicAssign8(*pValidByte,RxLength);

#if (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON)
        {
#if (FRIF_SINGLE_CLST_OPT_ENABLE == STD_OFF)
            uint8 FrIf_ClstIdx = FRIF_GET_CONFIG_ADDR(FrIf_CtrlType, FrIf_RootConfigPtr->FrIf_CtrlRef)[FRIF_FR_CTRLIDX].ClstIdx;
#endif /* FRIF_SINGLE_CLST_OPT_ENABLE == STD_OFF */

            /* create PduInfoType pduInfo */
            PduInfoType pduInfo;

            /* prepare pduInfo for BusMirror */
            pduInfo.SduDataPtr = (P2VAR(uint8, AUTOMATIC,AUTOMATIC))(P2VAR(void,AUTOMATIC,AUTOMATIC))LPduPtr;
            pduInfo.SduLength = RxLength;
#endif /* (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON) */

#if (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON)
            if(FrIf_ClstRuntimeData[FRIF_CLSTIDX].BusmirroringEnabled == TRUE)
            {
                /* !LINKSTO FrIf.EB.BusmirrorSupport.ReceiveAndStore.MirrorReportFlexRayFrame,1 */
                Mirror_ReportFlexRayFrame
                    (
                        FRIF_FR_CTRLIDX,
                        SlotAssignment.SlotId,
                        SlotAssignment.Cycle,
                        SlotAssignment.channelId,
                        &pduInfo,
                        FALSE
                    );
            }
        }
#endif /* (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON) */
    }

}

#endif /* FRIF_DECOUPLED_RX_ENABLE */


#if (FRIF_TXCONFIRMATION_ENABLE == STD_ON)

/**
 * \brief  'Tx Confirmation' call of upper layer
 */
STATIC FUNC(void,FRIF_CODE) FrIf_TxConfirmationCallUL
    (
        const uint8 OwnerIdx,
        const PduIdType ConfPduIdx,
        const Std_ReturnType Result
    );

STATIC FUNC(void,FRIF_CODE) FrIf_TxConfirmationCallUL
    (
        const uint8 OwnerIdx,
        const PduIdType ConfPduIdx,
        const Std_ReturnType Result
    )
{
#if FRIF_UL_ASR43_API_ENABLE == STD_ON
  if(OwnerIdx >= FrIf_FirstASR43OwnerIdx)
  {
    /* call upper layer and indicate transmission */
    FrIf_PduOwnerFuncASR43Ptr[OwnerIdx - FrIf_FirstASR43OwnerIdx]
        .FrIfTxConfirmation(ConfPduIdx, Result);
  }
  else
#endif /* FRIF_UL_ASR43_API_ENABLE == STD_ON */
  {
    TS_PARAM_UNUSED(Result);

    /* call upper layer and indicate transmission */
    FrIf_PduOwnerFuncASR40Ptr[OwnerIdx]
        .FrIfTxConfirmation(ConfPduIdx);
  }
}

#if FRIF_UL_ASR43_API_ENABLE == STD_ON
/**
 * \brief  'Tx Confirmation' job operation implementation for TxConflict
 */
STATIC FUNC(void,FRIF_CODE) FrIf_TxConfirmationHandleTxConflict
    (
        CONSTP2CONST(FrIf_ComOpType, AUTOMATIC, FRIF_APPL_CONST) FrIf_OperationPtr,
        uint8 TxMode
    );

STATIC FUNC(void,FRIF_CODE) FrIf_TxConfirmationHandleTxConflict
    (
        CONSTP2CONST(FrIf_ComOpType, AUTOMATIC, FRIF_APPL_CONST) FrIf_OperationPtr,
        uint8 TxMode
    )
{
  /* care for different configuration data structures */
  /* immediate Pdu */
  if((TxMode == FRIF_CONFIRM_IMM_FRAME) &&
      (FrIf_OperationPtr->PduPositionNum_OwnerIdx >= FrIf_FirstASR43OwnerIdx))
  {
    /* calculate the PduIdx from the Pdu config address */
    const uint8* pElement =
        ((const uint8*)(const void*)FRIF_GET_CONFIG_ADDR(void,FrIf_OperationPtr->ContainerRef));
    const uint8* pBase =
        ((const uint8*)(const void*)FRIF_GET_CONFIG_ADDR(FrIf_TxPduType,FrIf_RootConfigPtr->FrIf_TxPduRef));

    const sint32_least AdressDiffSigned = pElement - pBase;
    const uint32_least AdressDiff = (uint32_least)AdressDiffSigned;
    const uint32_least ElementDiff = AdressDiff / sizeof(FrIf_TxPduType);
    const FrIf_PduIdxType PduIdx = (FrIf_PduIdxType)ElementDiff;


    /* obtain Pdu information pointer */
    CONSTP2CONST(FrIf_TxPduType, AUTOMATIC,FRIF_APPL_CONST) PduPtr =
        &FRIF_GET_CONFIG_ADDR(FrIf_TxPduType, FrIf_RootConfigPtr->FrIf_TxPduRef)[PduIdx];

    /* If Pdu requires tx confirmation and UL requires ASR43 conform TxConfirmation,
               then pending bit must be cleared */
    if(
        PduIdx >= FrIf_RootConfigPtr->FrIf_TxPduNoConfNum &&
        FrIf_OperationPtr->PduPositionNum_OwnerIdx >= FrIf_FirstASR43OwnerIdx
    )
    {
      /* Deviation MISRAC2012-1 <+2> */
      CONSTP2VAR(uint8,AUTOMATIC,FRIF_VAR) pTxPending =
          &((P2VAR(uint8,AUTOMATIC,FRIF_VAR))(P2VAR(void,AUTOMATIC,FRIF_VAR)) FrIf_Mem)[PduIdx];
      boolean CallTxConfirmation = FALSE;

      /* That's why this critical area is protected */
      SchM_Enter_FrIf_SCHM_FRIF_TX_ADMINISTRATION();
      {

        const uint8 TxPending = *pTxPending;

        /* check whether frame was previously transmitted */
        if((TxPending&FRIF_PENDINGBIT_MASK) != FRIF_PENDINGBIT_NONE)
        {
          CallTxConfirmation = TRUE;

          (*pTxPending) = (uint8)(TxPending&FRIF_PENDINGBIT_CLEARBITMASK);

        }
      }
      /* Leave critical area */
      SchM_Exit_FrIf_SCHM_FRIF_TX_ADMINISTRATION();

      if(CallTxConfirmation != FALSE)
      {
        FrIf_TxConfirmationCallUL(
            FrIf_OperationPtr->PduPositionNum_OwnerIdx,
            (PduIdType)PduPtr->ConfPduIdx,
            E_NOT_OK
        );
      }
    }
  }
  /* decoupled Pdu */
  else
  {
    /* obtain pointer to configuration data */
    CONSTP2CONST(FrIf_PduPositionType, AUTOMATIC,FRIF_APPL_CONST) PduPositionStartPtr =
        FRIF_GET_CONFIG_ADDR(FrIf_PduPositionType, FrIf_OperationPtr->ContainerRef);

    /* create Pdu-iterator */
    uint16_least iPdu;

    /* iterate over Pdus contained in Frame */
    for (iPdu = 0U;iPdu < (uint16_least)FrIf_OperationPtr->PduPositionNum_OwnerIdx; iPdu++)
    {
      /* obtain pointer to configuration data */
      CONSTP2CONST(FrIf_PduPositionType, AUTOMATIC,FRIF_APPL_CONST) PduPositionPtr =
          &PduPositionStartPtr[iPdu];

      /* obtain Pdu index */
      const FrIf_PduIdxType PduIdx =
          PduPositionPtr->PduIdx;

      /* get owner Idx */
      const uint8 OwnerIdx = FRIF_GET_OWNERIDX(PduPositionPtr->UB_BitPos_OwnerIdx);

      /* obtain Pdu information pointer */
      /* Deviation MISRAC2012-1 <+2> */
      CONSTP2CONST(FrIf_TxPduType, AUTOMATIC,FRIF_APPL_CONST) PduPtr =
          &FRIF_GET_CONFIG_ADDR(FrIf_TxPduType, FrIf_RootConfigPtr->FrIf_TxPduRef)[PduIdx];

      /* If Pdu requires tx confirmation and UL requires ASR43 conform TxConfirmation,
                   then pending bit must be cleared */
      if(PduIdx >= FrIf_RootConfigPtr->FrIf_TxPduNoConfNum && OwnerIdx >=  FrIf_FirstASR43OwnerIdx)
      {
        /* Deviation MISRAC2012-1 <+2> */
        CONSTP2VAR(uint8,AUTOMATIC,FRIF_VAR) pTxPending =
            &((P2VAR(uint8,AUTOMATIC,FRIF_VAR))(P2VAR(void,AUTOMATIC,FRIF_VAR)) FrIf_Mem)[PduIdx];
        boolean CallTxConfirmation = FALSE;

        /* That's why this critical area is protected */
        SchM_Enter_FrIf_SCHM_FRIF_TX_ADMINISTRATION();
        {

          const uint8 TxPending = *pTxPending;

          /* check whether frame was previously transmitted */
          if((TxPending&FRIF_PENDINGBIT_MASK) != FRIF_PENDINGBIT_NONE)
          {
            CallTxConfirmation = TRUE;

            (*pTxPending) = (uint8)(TxPending&FRIF_PENDINGBIT_CLEARBITMASK);

          }
        }
        /* Leave critical area */
        SchM_Exit_FrIf_SCHM_FRIF_TX_ADMINISTRATION();

        if(CallTxConfirmation != FALSE)
        {
          FrIf_TxConfirmationCallUL(
              OwnerIdx,
              (PduIdType)PduPtr->ConfPduIdx,
              E_NOT_OK
          );
        }
      }
    }
  }
}
#endif /* FRIF_UL_ASR43_API_ENABLE == STD_ON */


/**
 * \brief  'Tx Confirmation' job operation implementation
 */
STATIC FUNC(void,FRIF_CODE) FrIf_TxConfirmation
    (
        CONSTP2CONST(FrIf_ComOpType, AUTOMATIC, FRIF_APPL_CONST) FrIf_OperationPtr,
        uint8 TxMode
    );

STATIC FUNC(void,FRIF_CODE) FrIf_TxConfirmation
    (
        CONSTP2CONST(FrIf_ComOpType, AUTOMATIC, FRIF_APPL_CONST) FrIf_OperationPtr,
        uint8 TxMode
    )
{

/* if no single controller implementation */
#if (FRIF_SINGLE_CTRL_OPT_ENABLE == STD_OFF)
    /* obtain actual controller index */
    const uint8 FrIf_CtrlIdx = FrIf_OperationPtr->FrIfCtrlIdx;
#endif /* FRIF_SINGLE_CTRL_OPT_ENABLE */

    /* variable for obtaining TX-Status */
    /* initialize with default error behavior (not transmitted) because
     * it's value stays untouched if transmission status reading fails by any reason
     */
    Fr_TxLPduStatusType TxStatus = FR_NOT_TRANSMITTED;

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

#if (FRIF_FR_DRIVER_AUTOSAR_VERSION == FRIF_FR_ASR_44_SUPPORT)
    /* !LINKSTO FrIf.EB.BusmirrorSupport.TxConfirmation.FrCheckTxLPduStatus,1 */
    /* check for transmission status */
    (void)FrIf_Call_Fr_Arg4(FRIF_FR_INSTANCE_IDX,CheckTxLPduStatus,
            FRIF_FR_CTRLIDX,
            FrIf_OperationPtr->LPduIdx,
            &TxStatus,
            SlotAssignmentPtr
         );
#else
    /* check for transmission status */
    (void)FrIf_Call_Fr_Arg3(FRIF_FR_INSTANCE_IDX,CheckTxLPduStatus,
            FRIF_FR_CTRLIDX,
            FrIf_OperationPtr->LPduIdx,
            &TxStatus
         );
#endif /* FRIF_FR_DRIVER_AUTOSAR_VERSION == FRIF_FR_ASR_44_SUPPORT */

    /* proceed only if transmission has happened */
    if(TxStatus == FR_TRANSMITTED)
    {
        /* care for different configuration data structures */
        /* immediate Pdu */
        if(TxMode == FRIF_CONFIRM_IMM_FRAME)
        {
            /* calculate the PduIdx from the Pdu config address */
            const uint8* pElement =
              ((const uint8*)(const void*)FRIF_GET_CONFIG_ADDR(void,FrIf_OperationPtr->ContainerRef));
            const uint8* pBase =
              ((const uint8*)(const void*)FRIF_GET_CONFIG_ADDR(FrIf_TxPduType,FrIf_RootConfigPtr->FrIf_TxPduRef));

            const sint32_least AdressDiffSigned = pElement - pBase;
            const uint32_least AdressDiff = (uint32_least)AdressDiffSigned;
            const uint32_least ElementDiff = AdressDiff / sizeof(FrIf_TxPduType);
            const FrIf_PduIdxType PduIdx = (FrIf_PduIdxType)ElementDiff;


            /* obtain Pdu information pointer */
            CONSTP2CONST(FrIf_TxPduType, AUTOMATIC,FRIF_APPL_CONST) PduPtr =
                &FRIF_GET_CONFIG_ADDR(FrIf_TxPduType, FrIf_RootConfigPtr->FrIf_TxPduRef)[PduIdx];

        /* If Pdu requires tx confirmation then pending bit must be cleared */
            if(PduIdx >= FrIf_RootConfigPtr->FrIf_TxPduNoConfNum )
            {
                /* Deviation MISRAC2012-1 <+2> */
                CONSTP2VAR(uint8,AUTOMATIC,FRIF_VAR) pTxPending =
                  &((P2VAR(uint8,AUTOMATIC,FRIF_VAR))(P2VAR(void,AUTOMATIC,FRIF_VAR)) FrIf_Mem)[PduIdx];
                boolean CallTxConfirmation = FALSE;

                /* That's why this critical area is protected */
                SchM_Enter_FrIf_SCHM_FRIF_TX_ADMINISTRATION();
                {

                    const uint8 TxPending = *pTxPending;

                    /* check whether frame was previously transmitted */
                    if((TxPending&FRIF_PENDINGBIT_MASK) != FRIF_PENDINGBIT_NONE)
                    {
                        CallTxConfirmation = TRUE;

                        (*pTxPending) = (uint8)(TxPending&FRIF_PENDINGBIT_CLEARBITMASK);

                    }
                }
                /* Leave critical area */
                SchM_Exit_FrIf_SCHM_FRIF_TX_ADMINISTRATION();

                if(CallTxConfirmation == TRUE)
                {
                  FrIf_TxConfirmationCallUL(
                                             FrIf_OperationPtr->PduPositionNum_OwnerIdx,
                                             (PduIdType)PduPtr->ConfPduIdx,
                                             E_OK
                                           );
                }
            }
        }
        /* decoupled Pdu */
        else
        {
            /* obtain pointer to configuration data */
            CONSTP2CONST(FrIf_PduPositionType, AUTOMATIC,FRIF_APPL_CONST) PduPositionStartPtr =
                FRIF_GET_CONFIG_ADDR(FrIf_PduPositionType, FrIf_OperationPtr->ContainerRef);

            /* create Pdu-iterator */
            uint16_least iPdu;

            /* iterate over Pdus contained in Frame */
            for (iPdu = 0U;iPdu < (uint16_least)FrIf_OperationPtr->PduPositionNum_OwnerIdx; iPdu++)
            {
                /* obtain pointer to configuration data */
                CONSTP2CONST(FrIf_PduPositionType, AUTOMATIC,FRIF_APPL_CONST) PduPositionPtr =
                    &PduPositionStartPtr[iPdu];

                /* obtain Pdu index */
                const FrIf_PduIdxType PduIdx =
                    PduPositionPtr->PduIdx;

                /* obtain Pdu information pointer */
                /* Deviation MISRAC2012-1 <+2> */
                CONSTP2CONST(FrIf_TxPduType, AUTOMATIC,FRIF_APPL_CONST) PduPtr =
                    &FRIF_GET_CONFIG_ADDR(FrIf_TxPduType, FrIf_RootConfigPtr->FrIf_TxPduRef)[PduIdx];

                /* If Pdu requires tx confirmation then pending bit must be cleared */
                if(PduIdx >= FrIf_RootConfigPtr->FrIf_TxPduNoConfNum )
                {
                    /* Deviation MISRAC2012-1 <+2> */
                    CONSTP2VAR(uint8,AUTOMATIC,FRIF_VAR) pTxPending =
                      &((P2VAR(uint8,AUTOMATIC,FRIF_VAR))(P2VAR(void,AUTOMATIC,FRIF_VAR)) FrIf_Mem)[PduIdx];
                    boolean CallTxConfirmation = FALSE;

                    /* That's why this critical area is protected */
                    SchM_Enter_FrIf_SCHM_FRIF_TX_ADMINISTRATION();
                    {

                        const uint8 TxPending = *pTxPending;

                        /* check whether frame was previously transmitted */
                        if((TxPending&FRIF_PENDINGBIT_MASK) != FRIF_PENDINGBIT_NONE)
                        {
                            CallTxConfirmation = TRUE;

                            (*pTxPending) = (uint8)(TxPending&FRIF_PENDINGBIT_CLEARBITMASK);

                        }
                    }
                    /* Leave critical area */
                    SchM_Exit_FrIf_SCHM_FRIF_TX_ADMINISTRATION();

                    if(CallTxConfirmation != FALSE)
                    {
                      FrIf_TxConfirmationCallUL(
                                                 FRIF_GET_OWNERIDX(PduPositionPtr->UB_BitPos_OwnerIdx),
                                                 (PduIdType)PduPtr->ConfPduIdx,
                                                 E_OK
                                               );
                    }
                }
            }
        }
    }
#if (FRIF_UL_ASR43_API_ENABLE == STD_ON) || (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON)
    /* Only necessary if ASR43 conform UL exists */
    else if(TxStatus == FR_TRANSMITTED_CONFLICT)
    {

#if (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON)
#if (FRIF_SINGLE_CLST_OPT_ENABLE == STD_OFF)
    uint8 FrIf_ClstIdx = FRIF_GET_CONFIG_ADDR(FrIf_CtrlType, FrIf_RootConfigPtr->FrIf_CtrlRef)[FRIF_FR_CTRLIDX].ClstIdx;
#endif /* FRIF_SINGLE_CLST_OPT_ENABLE == STD_OFF */

    if(FrIf_ClstRuntimeData[FRIF_CLSTIDX].BusmirroringEnabled == TRUE)
    {
        /* !LINKSTO FrIf.EB.BusmirrorSupport.TxConfirmation.MirrorReportFlexRayFrame,1 */
        Mirror_ReportFlexRayFrame
            (
                FRIF_FR_CTRLIDX,
                SlotAssignment.SlotId,
                SlotAssignment.Cycle,
                SlotAssignment.channelId,
                NULL_PTR,
                TRUE
            );
    }
#endif /* (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON) */

#if FRIF_UL_ASR43_API_ENABLE == STD_ON
      FrIf_TxConfirmationHandleTxConflict(FrIf_OperationPtr, TxMode);
#endif /* FRIF_UL_ASR43_API_ENABLE == STD_ON */

    }
    else
    {
      /* In case of FR_NOT_TRANSMITTED nothing shall be done */
    }
#endif /* FRIF_UL_ASR43_API_ENABLE == STD_ON || FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON */
}

#endif /* FRIF_TXCONFIRMATION_ENABLE */


#if (FRIF_IMMEDIATE_RX_ENABLE == STD_ON)

/**
 * \brief  'Receive and Indicate' job operation implementation
 */
STATIC FUNC(void,FRIF_CODE) FrIf_ReceiveAndIndicateFrame
    (
        CONSTP2CONST(FrIf_ComOpType, AUTOMATIC, FRIF_APPL_CONST) FrIf_OperationPtr
    );

STATIC FUNC(void,FRIF_CODE) FrIf_ReceiveAndIndicateFrame
    (
        CONSTP2CONST(FrIf_ComOpType, AUTOMATIC, FRIF_APPL_CONST) FrIf_OperationPtr
    )
{
/* if no single controller implementation */
#if (FRIF_SINGLE_CTRL_OPT_ENABLE == STD_OFF)
    /* obtain actual controller index */
    const uint8 FrIf_CtrlIdx = FrIf_OperationPtr->FrIfCtrlIdx;
#endif /* FRIF_SINGLE_CTRL_OPT_ENABLE */

    /* variable for obtaining RX-Status */
    /* initialize with default error behaviour (nothing received) because
     * it's value stays untouched if reception fails by any reason
     */
    Fr_RxLPduStatusType RxStatus = FR_NOT_RECEIVED;
    uint16_least RxCounter = 0U;


    do
    {
        /* get Received payload length here */
        uint8 RxLength;

        /* get Received payload length here */
        uint32 aTempBuffer[FRIF_TMPPAYLOADBUFFER_U32_LENGTH];

        /* Deviation MISRAC2012-1 <+2> */
        CONSTP2VAR(uint8,AUTOMATIC,AUTOMATIC) pTempBuffer =
            (P2VAR(uint8,AUTOMATIC,AUTOMATIC))(P2VAR(void,AUTOMATIC,AUTOMATIC))&aTempBuffer[0];

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

#if (FRIF_FR_DRIVER_AUTOSAR_VERSION == FRIF_FR_ASR_44_SUPPORT)
        /* !LINKSTO FrIf.EB.BusmirrorSupport.ReceiveAndIndicate.FrReceiveRxLPdu,1 */
        /* Receive frame from Fr */
        /* don't evaluate return code, since default behaviour is already
         * initialized
         */
        (void)FrIf_Call_Fr_Arg6(FRIF_FR_INSTANCE_IDX,ReceiveRxLPdu,
                FRIF_FR_CTRLIDX,
                FrIf_OperationPtr->LPduIdx,
                pTempBuffer,
                &RxStatus,
                &RxLength,
                SlotAssignmentPtr
             );
#else
        /* Receive frame from Fr */
        /* don't evaluate return code, since default behaviour is already
         * initialized
         */
        (void)FrIf_Call_Fr_Arg5(FRIF_FR_INSTANCE_IDX,ReceiveRxLPdu,
                FRIF_FR_CTRLIDX,
                FrIf_OperationPtr->LPduIdx,
                pTempBuffer,
                &RxStatus,
                &RxLength
             );
#endif /* FRIF_FR_DRIVER_AUTOSAR_VERSION == FRIF_FR_ASR_44_SUPPORT */

        /* proceed further only, if we received a frame */
        if (RxStatus != FR_NOT_RECEIVED)
        {
            /* obtain pointer to configuration data */
            CONSTP2CONST(FrIf_PduPositionType, AUTOMATIC,FRIF_APPL_CONST) PduPositionStartPtr =
                FRIF_GET_CONFIG_ADDR(FrIf_PduPositionType, FrIf_OperationPtr->ContainerRef);

#if (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON)
            {
#if (FRIF_SINGLE_CLST_OPT_ENABLE == STD_OFF)
                uint8 FrIf_ClstIdx = FRIF_GET_CONFIG_ADDR(FrIf_CtrlType, FrIf_RootConfigPtr->FrIf_CtrlRef)[FRIF_FR_CTRLIDX].ClstIdx;
#endif /* FRIF_SINGLE_CLST_OPT_ENABLE == STD_OFF */

                /* create PduInfoType pduInfo */
                PduInfoType pduInfo;

                /* prepare pduInfo for BusMirror */
                pduInfo.SduDataPtr = pTempBuffer;
                pduInfo.SduLength = RxLength;
#endif /* (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON) */

#if (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON)
                if(FrIf_ClstRuntimeData[FRIF_CLSTIDX].BusmirroringEnabled == TRUE)
                {
                    /* !LINKSTO FrIf.EB.BusmirrorSupport.ReceiveAndIndicate.MirrorReportFlexRayFrame,1 */
                    Mirror_ReportFlexRayFrame
                        (
                            FRIF_FR_CTRLIDX,
                            SlotAssignment.SlotId,
                            SlotAssignment.Cycle,
                            SlotAssignment.channelId,
                            &pduInfo,
                            FALSE
                        );
                }
            }
#endif /* (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON) */

            {
                /* create Pdu-iterator */
                uint16_least iPdu;

                /* iterate over Pdus contained in Frame */
                for (iPdu = 0U; iPdu < (uint16_least)FrIf_OperationPtr->PduPositionNum_OwnerIdx; iPdu++)
                {

                    /* obtain pointer to configuration data */
                    CONSTP2CONST(FrIf_PduPositionType, AUTOMATIC,FRIF_APPL_CONST) PduPositionPtr =
                        &PduPositionStartPtr[iPdu];

#if FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE == STD_ON
                    /* check if update bit and payload start offset is within received payload */
                    /* otherwise, ignore this Pdu */
                    if((PduPositionPtr->UB_ByteOffset < RxLength) && (PduPositionPtr->PduOffset < RxLength))
#endif /* FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE */
                    {
                        /* obtain UB Bit position */
                        const uint8 ubBitPos =
                            FRIF_UB_GETBITPOS(PduPositionPtr->UB_BitPos_OwnerIdx);

                        if((ubBitPos == FRIF_UPDATEBIT_NONE) ||
                           ((pTempBuffer[PduPositionPtr->UB_ByteOffset]&FRIF_UB_GETBITMASK(ubBitPos)) != 0U))
                        {
                            /* create PduInfoType pduInfo */
                            PduInfoType pduInfo;

                            /* get owner Idx */
                            const uint8 OwnerIdx = FRIF_GET_OWNERIDX(PduPositionPtr->UB_BitPos_OwnerIdx);

                            /* set pointer to Pdu payload */
                            pduInfo.SduDataPtr = &pTempBuffer[PduPositionPtr->PduOffset];

                            /* get length of Pdu */
#if FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE == STD_ON
                            if((RxLength - PduPositionPtr->PduOffset) < PduPositionPtr->PduLength)
                            {
                              const uint8 DynRxLength = RxLength - PduPositionPtr->PduOffset;
                              pduInfo.SduLength = DynRxLength;
                            }
                            else
#endif /* FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE */
                            {
                              pduInfo.SduLength = PduPositionPtr->PduLength;
                            }

#if FRIF_UL_ASR43_API_ENABLE == STD_ON
                            if(OwnerIdx >= FrIf_FirstASR43OwnerIdx)
                            {
                              /* provide reception indication to upper layer */
                              FrIf_PduOwnerFuncASR43Ptr[OwnerIdx - FrIf_FirstASR43OwnerIdx]
                                  .FrIfRxIndication((PduIdType)PduPositionPtr->PduIdx, &pduInfo);
                            }
                            else
#endif /* FRIF_UL_ASR43_API_ENABLE == STD_ON */
                            {
                              /* provide reception indication to upper layer */
                              FrIf_PduOwnerFuncASR40Ptr[OwnerIdx]
                                  .FrIfRxIndication((PduIdType)PduPositionPtr->PduIdx, &pduInfo);
                            }
                        }
                    }
                }
            }
        }

    RxCounter++;
    /* continue only in case there is more data available in the FIFO
       indicated by RxStatus == FR_RECEIVED_MORE_DATA_AVAILABLE and
       the maximal number of Fr_ReceiveRxLPdu() calls has not been reached */
    } while(    (RxStatus == FR_RECEIVED_MORE_DATA_AVAILABLE)
             && (RxCounter < FRIF_GLOBALRXMAXLOOP) );

}

/**
 * \brief  'Receive and Indicate' job operation implementation for simple frames
 */
STATIC FUNC(void,FRIF_CODE) FrIf_ReceiveAndIndicateSimpleFrame
    (
        CONSTP2CONST(FrIf_ComOpType, AUTOMATIC, FRIF_APPL_CONST) FrIf_OperationPtr
    );

STATIC FUNC(void,FRIF_CODE) FrIf_ReceiveAndIndicateSimpleFrame
    (
        CONSTP2CONST(FrIf_ComOpType, AUTOMATIC, FRIF_APPL_CONST) FrIf_OperationPtr
    )
{
/* if no single controller implementation */
#if (FRIF_SINGLE_CTRL_OPT_ENABLE == STD_OFF)
    /* obtain actual controller index */
    const uint8 FrIf_CtrlIdx = FrIf_OperationPtr->FrIfCtrlIdx;
#endif /* FRIF_SINGLE_CTRL_OPT_ENABLE */


    /* variable for obtaining RX-Status */
    /* initialize with default error behaviour (nothing received) because
     * it's value stays untouched if reception fails by any reason
     */
    Fr_RxLPduStatusType RxStatus = FR_NOT_RECEIVED;
    uint16_least RxCounter = 0U;

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

    do
    {
        /* get Received payload length here */
        uint8 RxLength;

        /* create temporary buffer for constructing the frame */
        uint32 aTempBuffer[FRIF_TMPPAYLOADBUFFER_U32_LENGTH];

        /* create an uint8 pointer to uint32 allocated memory */
        /* Deviation MISRAC2012-1 <+2> */
        CONSTP2VAR(uint8,AUTOMATIC,AUTOMATIC) pTempBuffer =
            (P2VAR(uint8,AUTOMATIC,AUTOMATIC))(P2VAR(void,AUTOMATIC,AUTOMATIC))&aTempBuffer[0];

#if (FRIF_FR_DRIVER_AUTOSAR_VERSION == FRIF_FR_ASR_44_SUPPORT)
        /* !LINKSTO FrIf.EB.BusmirrorSupport.ReceiveAndIndicate.FrReceiveRxLPdu,1 */
        /* Receive frame from Fr */
        /* don't evaluate return code, since default behaviour is already
         * initialized
         */
        (void)FrIf_Call_Fr_Arg6(FRIF_FR_INSTANCE_IDX,ReceiveRxLPdu,
                FRIF_FR_CTRLIDX,
                FrIf_OperationPtr->LPduIdx,
                pTempBuffer,
                &RxStatus,
                &RxLength,
                SlotAssignmentPtr
             );
#else
        /* Receive frame from Fr */
        /* don't evaluate return code, since default behaviour is already
         * initialized
         */
        (void)FrIf_Call_Fr_Arg5(FRIF_FR_INSTANCE_IDX,ReceiveRxLPdu,
                FRIF_FR_CTRLIDX,
                FrIf_OperationPtr->LPduIdx,
                pTempBuffer,
                &RxStatus,
                &RxLength
             );
#endif /* FRIF_FR_DRIVER_AUTOSAR_VERSION == FRIF_FR_ASR_44_SUPPORT */

        /* proceed further only, if we received a frame */
        if (RxStatus != FR_NOT_RECEIVED)
        {
            /* obtain Pdu index */
            const FrIf_PduIdxType PduIdx =
                *((const FrIf_PduIdxType*)(FRIF_GET_CONFIG_ADDR(void,FrIf_OperationPtr->ContainerRef)));

            /* create PduInfoType pduInfo */
            PduInfoType pduInfo;

            /* get pointer to Pdu payload */
            pduInfo.SduDataPtr = pTempBuffer;

#if (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON)
            {
#if (FRIF_SINGLE_CLST_OPT_ENABLE == STD_OFF)
                uint8 FrIf_ClstIdx = FRIF_GET_CONFIG_ADDR(FrIf_CtrlType, FrIf_RootConfigPtr->FrIf_CtrlRef)[FRIF_FR_CTRLIDX].ClstIdx;
#endif /* FRIF_SINGLE_CLST_OPT_ENABLE == STD_OFF */

                /* prepare pduInfo for BusMirror PduData was already prepared */
                pduInfo.SduLength = RxLength;

                if(FrIf_ClstRuntimeData[FRIF_CLSTIDX].BusmirroringEnabled == TRUE)
                {
                    /* !LINKSTO FrIf.EB.BusmirrorSupport.ReceiveAndIndicate.MirrorReportFlexRayFrame,1 */
                    Mirror_ReportFlexRayFrame
                        (
                            FRIF_FR_CTRLIDX,
                            SlotAssignment.SlotId,
                            SlotAssignment.Cycle,
                            SlotAssignment.channelId,
                            &pduInfo,
                            FALSE
                        );
                }
            }
#endif /* (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON) */

            /* get length of Pdu */
#if FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE == STD_ON
            if( RxLength  < FrIf_OperationPtr->LPduLength)
            {
              pduInfo.SduLength = (PduLengthType) RxLength;
            }
            else
#endif /* FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE */
            {
              pduInfo.SduLength = (PduLengthType) FrIf_OperationPtr->LPduLength;
            }

#if FRIF_UL_ASR43_API_ENABLE == STD_ON
            if(FrIf_OperationPtr->PduPositionNum_OwnerIdx >= FrIf_FirstASR43OwnerIdx)
            {
              /* provide reception indication to upper layer */
              FrIf_PduOwnerFuncASR43Ptr[FrIf_OperationPtr->PduPositionNum_OwnerIdx - FrIf_FirstASR43OwnerIdx]
                  .FrIfRxIndication((PduIdType)PduIdx, &pduInfo);
            }
            else
#endif /* FRIF_UL_ASR43_API_ENABLE == STD_ON */
            {
              /* provide reception indication to upper layer */
              FrIf_PduOwnerFuncASR40Ptr[FrIf_OperationPtr->PduPositionNum_OwnerIdx]
                  .FrIfRxIndication((PduIdType)PduIdx, &pduInfo);
            }
        }

        RxCounter++;
    /* continue only in case there is more data available in the FIFO
       indicated by RxStatus == FR_RECEIVED_MORE_DATA_AVAILABLE and
       the maximal number of Fr_ReceiveRxLPdu() calls has not been reached */
    } while(    (RxStatus == FR_RECEIVED_MORE_DATA_AVAILABLE)
             && (RxCounter < FRIF_GLOBALRXMAXLOOP) );

}

#endif /* FRIF_IMMEDIATE_RX_ENABLE */


#if (FRIF_DECOUPLED_TX_ENABLE == STD_ON)

#if FRIF_UL_ASR43_API_ENABLE == STD_ON
/**
 * \brief  'Transmit Frame' job operation implementation handling E_NOT_OK from
 *         TransmitTxLpdu
 */
STATIC FUNC(void,FRIF_CODE) FrIf_TransmitFrameHandle_E_NOT_OK
    (
        CONSTP2CONST(FrIf_ComOpType, AUTOMATIC, FRIF_APPL_CONST) FrIf_OperationPtr,
        CONSTP2CONST(FrIf_PduPositionType, AUTOMATIC, FRIF_APPL_CONST) PduPositionStartPtr
    );

STATIC FUNC(void,FRIF_CODE) FrIf_TransmitFrameHandle_E_NOT_OK
    (
        CONSTP2CONST(FrIf_ComOpType, AUTOMATIC, FRIF_APPL_CONST) FrIf_OperationPtr,
        CONSTP2CONST(FrIf_PduPositionType, AUTOMATIC, FRIF_APPL_CONST) PduPositionStartPtr
    )
{
  uint16_least iPdu;
  /* iterate once more over Pdus */
  for(iPdu = 0U; iPdu < (uint16_least)FrIf_OperationPtr->PduPositionNum_OwnerIdx; iPdu++)
  {
    /* get frame structure information pointer */
    CONSTP2CONST(FrIf_PduPositionType, AUTOMATIC,FRIF_APPL_CONST) PduPositionPtr =
        &PduPositionStartPtr[iPdu];

    /* obtain Pdu index */
    const FrIf_PduIdxType PduIdx = PduPositionPtr->PduIdx;

    /* obtain Pdu information pointer */
    CONSTP2CONST(FrIf_TxPduType, AUTOMATIC,FRIF_APPL_CONST) PduPtr =
        &FRIF_GET_CONFIG_ADDR(FrIf_TxPduType, FrIf_RootConfigPtr->FrIf_TxPduRef)[PduIdx];

    /* get owner Idx */
    const uint8 OwnerIdx = FRIF_GET_OWNERIDX(PduPositionPtr->UB_BitPos_OwnerIdx);

    if(OwnerIdx >= FrIf_FirstASR43OwnerIdx)
    {
      /* If Pdu requires tx confirmation and has not been transmitted then UL has to be informed */
      if(PduIdx >= FrIf_RootConfigPtr->FrIf_TxPduNoConfNum)
      {
        /* obtain trigger counter and pending bit */
        uint8 txCnt;

        /* access to FrIf_Mem[PduPtr->TxCounterDIdx] is critical (FrIf_Transmit)  */
        /* therefore protect by critical area */
        SchM_Enter_FrIf_SCHM_FRIF_TX_ADMINISTRATION();

        /* Deviation MISRAC2012-1 */
        TS_AtomicAssign8(txCnt,((P2VAR(uint8,AUTOMATIC,FRIF_VAR))(P2VAR(void,AUTOMATIC,FRIF_VAR)) FrIf_Mem)[PduIdx]);

        /* decrease transmit counter --> the PDU is
         * transmitted, regarding the counter */
        txCnt = ((txCnt&FRIF_TXCNT_MASK) > 0x00U) ? (txCnt - 1U) : 0U;

        /* call upper layer and indicate failing transmission */
        FrIf_PduOwnerFuncASR43Ptr[OwnerIdx - FrIf_FirstASR43OwnerIdx]
                                  .FrIfTxConfirmation((PduIdType)PduPtr->ConfPduIdx, E_NOT_OK);

        /* write back transmission counter */
        /* Deviation MISRAC2012-1 */
        TS_AtomicAssign8(((P2VAR(uint8,AUTOMATIC,FRIF_VAR))(P2VAR(void,AUTOMATIC,FRIF_VAR)) FrIf_Mem)[PduIdx],txCnt);

        /* Leave critical area */
        SchM_Exit_FrIf_SCHM_FRIF_TX_ADMINISTRATION();
      }
    }
  }
}
#endif /* FRIF_UL_ASR43_API_ENABLE == STD_ON */

/**
 * \brief  'Transmit Frame' job operation implementation
 */
STATIC FUNC(void,FRIF_CODE) FrIf_TransmitFrame
    (
        CONSTP2CONST(FrIf_ComOpType, AUTOMATIC, FRIF_APPL_CONST) FrIf_OperationPtr,
        uint8 TransmitMode
    );

STATIC FUNC(void,FRIF_CODE) FrIf_TransmitFrame
    (
        CONSTP2CONST(FrIf_ComOpType, AUTOMATIC, FRIF_APPL_CONST) FrIf_OperationPtr,
        uint8 TransmitMode
    )
{

/* if no single controller implementation */
#if (FRIF_SINGLE_CTRL_OPT_ENABLE == STD_OFF)
    /* obtain actual controller index */
    const uint8 FrIf_CtrlIdx = FrIf_OperationPtr->FrIfCtrlIdx;
#endif /* FRIF_SINGLE_CTRL_OPT_ENABLE */

    /* create Pdu iterator */
    uint16_least iPdu;

    /* remember whether a single Pdu was updated */
    boolean PduUpdated = FALSE;

    /* obtain configuration data pointers */
    CONSTP2CONST(FrIf_PduPositionType, AUTOMATIC,FRIF_APPL_CONST) PduPositionStartPtr =
        FRIF_GET_CONFIG_ADDR(FrIf_PduPositionType, FrIf_OperationPtr->ContainerRef);

    /* create temporary buffer for constructing the frame */
    uint32 aTempBuffer[FRIF_TMPPAYLOADBUFFER_U32_LENGTH];

    /* create an uint8 pointer to uint32 allocated memory */
    /* Deviation MISRAC2012-1 <+2> */
    CONSTP2VAR(uint8, AUTOMATIC,AUTOMATIC) pTempBuffer =
        (P2VAR(uint8, AUTOMATIC,AUTOMATIC))(P2VAR(void,AUTOMATIC,AUTOMATIC))&aTempBuffer[0];

    /* calculate real payload length for transmission */
    uint8 TxPayloadLength = 0x0U;

    uint32 PduTxStatusArray[FRIF_TXSTATUS_NUM_WORDS];

/* using dynamic payload length support for FrIf ? */
#if (FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE == STD_ON)

    /* is this a dynamic lenbth LPDu or not ? */
    uint8_least IsDynamicLengthLPdu = 0U;

#endif /* FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE */

#if (FRIF_TX_FRAME_DEFAULT_VALUE_ENABLE == STD_ON)

    /* first of all clear all memory used for transmission */
    TS_MemSet32_NoCheck(aTempBuffer,FRIF_TX_FRAME_DEFAULT_VALUE,FrIf_OperationPtr->LPduLength);

#endif /* FRIF_TX_FRAME_DEFAULT_VALUE_ENABLE */

    /* first of all clear all memory used for transmission */
    TS_MemSet32_NoCheck(PduTxStatusArray,0x00U,(FRIF_TXSTATUS_NUM_WORDS*FRIF_TXSTATUS_WORD_SIZE)/8U);

    /* iterate over all Pdus */
    for(iPdu = 0U; iPdu < (uint16_least)FrIf_OperationPtr->PduPositionNum_OwnerIdx; iPdu++)
    {
        /* get frame structure information pointer */
        CONSTP2CONST(FrIf_PduPositionType, AUTOMATIC,FRIF_APPL_CONST) PduPositionPtr =
            &PduPositionStartPtr[iPdu];

        /* obtain Pdu index */
        const FrIf_PduIdxType PduIdx = PduPositionPtr->PduIdx;

        /* obtain Pdu information pointer */
        CONSTP2CONST(FrIf_TxPduType, AUTOMATIC,FRIF_APPL_CONST) PduPtr =
            &FRIF_GET_CONFIG_ADDR(FrIf_TxPduType, FrIf_RootConfigPtr->FrIf_TxPduRef)[PduIdx];

        uint8 txCnt;

        /* Deviation MISRAC2012-1 */
        TS_AtomicAssign8(txCnt,((P2VAR(uint8,AUTOMATIC,FRIF_VAR))(P2VAR(void,AUTOMATIC,FRIF_VAR)) FrIf_Mem)[PduIdx]);

/* using dynamic payload length support for FrIf ? */
#if (FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE == STD_ON)
        {
            /* set UB_FrameLength to 0 if no update bit is configured, otherwise to the
             * framelength required to cover the related update bit.
             */
            const uint8 UB_FrameLength =
              FRIF_UB_GETBITMASK(FRIF_UB_GETBITPOS(PduPositionPtr->UB_BitPos_OwnerIdx)) > 0U ?
                PduPositionPtr->UB_ByteOffset + 1U :
                0U;

            /* consider the position of the update bit for tx-length evaluation */
            if(UB_FrameLength > TxPayloadLength)
            {
                TxPayloadLength = UB_FrameLength;
            }

            /* if this is a dynamic Pdu, then consider the offset and SduLength of TriggerTransmit
             * call of the last updated Pdu and ByteOffset of update bits.
             */
            if(PduPtr->DynLenEnable_LPduIdx != 0U)
            {
                IsDynamicLengthLPdu = 1U;
            }
        }
#endif /* FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE */

        /* check if a transmission was triggered, or that frame shall always be transmitted */
        if (((txCnt&FRIF_TXCNT_MASK) > 0x00U) || (TransmitMode == FRIF_TRANSMIT_FRAME_NONE_MODE))
        {
            /* create PduInfoType pduInfo */
            PduInfoType pduInfo;
            Std_ReturnType ULTriggerTransmitRetVal = E_NOT_OK;

            /* get owner Idx */
            const uint8 OwnerIdx = FRIF_GET_OWNERIDX(PduPositionPtr->UB_BitPos_OwnerIdx);

            /* set pointer to temp buffer for Pdu data */
            pduInfo.SduDataPtr = &pTempBuffer[PduPositionPtr->PduOffset];

            /* set maximal length for Pdu data */
            pduInfo.SduLength = PduPositionPtr->PduLength;

            /* try to get PDU data, store to temp buffer, transmit only on success (E_OK)*/
#if FRIF_UL_ASR43_API_ENABLE == STD_ON
            if(OwnerIdx >= FrIf_FirstASR43OwnerIdx)
            {
              ULTriggerTransmitRetVal = FrIf_PduOwnerFuncASR43Ptr[OwnerIdx - FrIf_FirstASR43OwnerIdx]
                .FrIfTriggerTransmit(PduPtr->ConfPduIdx, &pduInfo);
            }
            else
#endif /* FRIF_UL_ASR43_API_ENABLE == STD_ON */
            {
              ULTriggerTransmitRetVal = FrIf_PduOwnerFuncASR40Ptr[OwnerIdx]
                .FrIfTriggerTransmit(PduPtr->ConfPduIdx, &pduInfo);
            }

            if(ULTriggerTransmitRetVal == E_OK)
            {
                /* set update bit */
                pTempBuffer[PduPositionPtr->UB_ByteOffset] |=
                    FRIF_UB_SETBITMASK(FRIF_UB_GETBITMASK(FRIF_UB_GETBITPOS(PduPositionPtr->UB_BitPos_OwnerIdx)));

                /* OK, we had at least one up-to-date Pdu */
                PduUpdated = TRUE;

                if(PduIdx >= FrIf_RootConfigPtr->FrIf_TxPduNoConfNum)
                {
                    /* mark for each Pdu whether which needs confirmation whether Pdu is actually transmitted or not */
                    const uint16_least TxStatusWordIndex = (uint16_least)(iPdu/FRIF_TXSTATUS_WORD_SIZE);
                    const uint32 TxStatusBitMask = (uint32)(((uint32)0x01U)<<(iPdu&FRIF_TXSTATUS_WORD_MASK));

                    /* Pdu is transmitted - save this information */
                    PduTxStatusArray[TxStatusWordIndex] |= TxStatusBitMask;
                }

/* using dynamic payload length support for FrIf ? */
#if (FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE == STD_ON)
                if((PduPositionPtr->PduOffset + (uint8)pduInfo.SduLength) > TxPayloadLength)
                {
                  TxPayloadLength = PduPositionPtr->PduOffset + (uint8)pduInfo.SduLength;
                }

#endif /* FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE */

            }
            else /* FrIfTriggerTransmit returned E_NOT_OK */
            {
                /* reset trigger counter */
                /* Deviation MISRAC2012-1 */
                TS_AtomicAssign8(txCnt,((P2VAR(uint8,AUTOMATIC,FRIF_VAR))(P2VAR(void,AUTOMATIC,FRIF_VAR)) FrIf_Mem)[PduIdx]);

                txCnt &= ~(uint8)FRIF_TXCNT_MASK;

                /* Deviation MISRAC2012-1 */
                TS_AtomicAssign8(((P2VAR(uint8,AUTOMATIC,FRIF_VAR))(P2VAR(void,AUTOMATIC,FRIF_VAR)) FrIf_Mem)[PduIdx],txCnt);

                /* clear update bit */
                pTempBuffer[PduPositionPtr->UB_ByteOffset] &=
                    FRIF_UB_CLEARBITMASK(FRIF_UB_GETBITMASK(FRIF_UB_GETBITPOS(PduPositionPtr->UB_BitPos_OwnerIdx)));
            }

        }
        else
        {
#if (FRIF_TX_FRAME_DEFAULT_VALUE_ENABLE == STD_OFF) || \
    (FRIF_TX_FRAME_DEFAULT_VALUE != 0x00UL)

            /* clear update bit in temp buffer */
            pTempBuffer[PduPositionPtr->UB_ByteOffset] &=
                FRIF_UB_CLEARBITMASK(FRIF_UB_GETBITMASK(FRIF_UB_GETBITPOS(PduPositionPtr->UB_BitPos_OwnerIdx)));

#endif /* FRIF_TX_FRAME_DEFAULT_VALUE_ENABLE && FRIF_TX_FRAME_DEFAULT_VALUE */
        }
    }

    /* if at least one PDU is up-to-date */
    if ((PduUpdated != FALSE) || (TransmitMode == FRIF_ALWAYS_TRANSMIT_FRAME))
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

/* using dynamic payload length support for FrIf ? */
#if (FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE == STD_ON)

        /* no dynamic LPdu length? then use configured payload length */
        if(IsDynamicLengthLPdu == 0U)

#endif /* FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE */

        {
            TxPayloadLength = FrIf_OperationPtr->LPduLength;
        }

        {
#if (FRIF_FR_DRIVER_AUTOSAR_VERSION == FRIF_FR_ASR_44_SUPPORT)
            /* !LINKSTO FrIf.EB.BusmirrorSupport.DecoupledTransmission.FrTransmitTxLPdu,1 */
            /* transmit frame built within this function */
            const Std_ReturnType RetCode =
                FrIf_Call_Fr_Arg5(FRIF_FR_INSTANCE_IDX,TransmitTxLPdu,
                        FRIF_FR_CTRLIDX,
                        FrIf_OperationPtr->LPduIdx,
                        pTempBuffer,
                        TxPayloadLength,
                        SlotAssignmentPtr
                     );
#else
            /* transmit frame built within this function */
            const Std_ReturnType RetCode =
                FrIf_Call_Fr_Arg4(FRIF_FR_INSTANCE_IDX,TransmitTxLPdu,
                        FRIF_FR_CTRLIDX,
                        FrIf_OperationPtr->LPduIdx,
                        pTempBuffer,
                        TxPayloadLength
                     );
#endif /* FRIF_FR_DRIVER_AUTOSAR_VERSION == FRIF_FR_ASR_44_SUPPORT */

            /* if transmission succeeded */
            if(RetCode == E_OK)
            {
                /* access to FrIf_Mem[PduPtr->TxCounterDIdx] is critical (FrIf_Transmit)  */
                /* therefore protect by critical area */
                SchM_Enter_FrIf_SCHM_FRIF_TX_ADMINISTRATION();

                /* iterate once more over Pdus */
                for(iPdu = 0U; iPdu < (uint16_least)FrIf_OperationPtr->PduPositionNum_OwnerIdx; iPdu++)
                {

                    /* get frame structure information pointer */
                    CONSTP2CONST(FrIf_PduPositionType, AUTOMATIC,FRIF_APPL_CONST) PduPositionPtr =
                        &PduPositionStartPtr[iPdu];

                    /* obtain Pdu index */
                    const FrIf_PduIdxType PduIdx = PduPositionPtr->PduIdx;

                    /* obtain trigger counter and pending bit */
                    uint8 txCnt;

                    /* Deviation MISRAC2012-1 */
                    TS_AtomicAssign8(txCnt,((P2VAR(uint8,AUTOMATIC,FRIF_VAR))(P2VAR(void,AUTOMATIC,FRIF_VAR)) FrIf_Mem)[PduIdx]);

                    /* decrease transmit counter --> the PDU is
                     * transmitted, regarding the counter */
                    txCnt = ((txCnt&FRIF_TXCNT_MASK) > 0x00U) ? (txCnt - 1U) : 0U;

                    /* If Pdu requires tx confirmation and has been transmitted then pending bit must be set */
                    if(PduIdx >= FrIf_RootConfigPtr->FrIf_TxPduNoConfNum)
                    {
                        /* check for each Pdu which needs confirmation whether Pdu was actually transmitted or not */
                        const uint16_least TxStatusWordIndex = (uint16_least)(iPdu/FRIF_TXSTATUS_WORD_SIZE);
                        const uint32 TxStatusBitMask = (uint32)(((uint32)0x01U)<<(iPdu&FRIF_TXSTATUS_WORD_MASK));

                        if((PduTxStatusArray[TxStatusWordIndex]&TxStatusBitMask) != 0x00U)
                        {
                            /* mark Pdu transmission pending information */
                            txCnt |= FRIF_PENDINGBIT_SETBITMASK;
                        }
                    }

                    /* write back transmission counter and pending bit */
                    /* Deviation MISRAC2012-1 */
                    TS_AtomicAssign8(((P2VAR(uint8,AUTOMATIC,FRIF_VAR))(P2VAR(void,AUTOMATIC,FRIF_VAR)) FrIf_Mem)[PduIdx],txCnt);

                }

                /* Leave critical area */
                SchM_Exit_FrIf_SCHM_FRIF_TX_ADMINISTRATION();

#if (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON)
                {
#if (FRIF_SINGLE_CLST_OPT_ENABLE == STD_OFF)
                    uint8 FrIf_ClstIdx = FRIF_GET_CONFIG_ADDR(FrIf_CtrlType, FrIf_RootConfigPtr->FrIf_CtrlRef)[FRIF_FR_CTRLIDX].ClstIdx;
#endif /* FRIF_SINGLE_CLST_OPT_ENABLE == STD_OFF */

                    /* create PduInfoType pduInfo */
                    PduInfoType pduInfo;

                    /* prepare pduInfo for BusMirror */
                    pduInfo.SduDataPtr = pTempBuffer;
                    pduInfo.SduLength = TxPayloadLength;

                    if(FrIf_ClstRuntimeData[FRIF_CLSTIDX].BusmirroringEnabled == TRUE)
                    {
                        /* !LINKSTO FrIf.EB.BusmirrorSupport.DecoupledTransmission.MirrorReportFlexRayFrame,1 */
                        Mirror_ReportFlexRayFrame
                            (
                                FRIF_FR_CTRLIDX,
                                SlotAssignment.SlotId,
                                SlotAssignment.Cycle,
                                SlotAssignment.channelId,
                                &pduInfo,
                                FALSE
                            );
                    }
                }
#endif /* (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON) */

            }
#if FRIF_UL_ASR43_API_ENABLE == STD_ON
            /* If Pdu owner is a ASR43 conform upper layer, discard message and inform UL with TXConf NOT_OK */
            else
            {
              FrIf_TransmitFrameHandle_E_NOT_OK(FrIf_OperationPtr, PduPositionStartPtr);
            }
#endif /* FRIF_UL_ASR43_API_ENABLE == STD_ON */
        }
    }
}

/**
 * \brief  'Transmit Frame' job operation implementation
 */
STATIC FUNC(void,FRIF_CODE) FrIf_TransmitSimpleFrame
    (
        CONSTP2CONST(FrIf_ComOpType, AUTOMATIC, FRIF_APPL_CONST) FrIf_OperationPtr,
        uint8 TransmitMode
    );

STATIC FUNC(void,FRIF_CODE) FrIf_TransmitSimpleFrame
    (
        CONSTP2CONST(FrIf_ComOpType, AUTOMATIC, FRIF_APPL_CONST) FrIf_OperationPtr,
        uint8 TransmitMode
    )
{

/* if no single controller implementation */
#if (FRIF_SINGLE_CTRL_OPT_ENABLE == STD_OFF)
    /* obtain actual controller index */
    uint8 FrIf_CtrlIdx = FrIf_OperationPtr->FrIfCtrlIdx;
#endif /* FRIF_SINGLE_CTRL_OPT_ENABLE */

    /* calculate the PduIdx from the Pdu config address */
    const uint8* pElement =
      ((const uint8*)(const void*)FRIF_GET_CONFIG_ADDR(void,FrIf_OperationPtr->ContainerRef));
    const uint8* pBase =
      ((const uint8*)(const void*)FRIF_GET_CONFIG_ADDR(FrIf_TxPduType,FrIf_RootConfigPtr->FrIf_TxPduRef));

    const sint32_least AdressDiffSigned = pElement - pBase;
    const uint32_least AdressDiff = (uint32_least)AdressDiffSigned;
    const uint32_least ElementDiff = AdressDiff / sizeof(FrIf_TxPduType);
    const FrIf_PduIdxType PduIdx = (FrIf_PduIdxType)ElementDiff;

    /* obtain Pdu information pointer */
    /* Deviation MISRAC2012-1 <+2> */
    CONSTP2CONST(FrIf_TxPduType, AUTOMATIC,FRIF_APPL_CONST) PduPtr =
        &FRIF_GET_CONFIG_ADDR(FrIf_TxPduType, FrIf_RootConfigPtr->FrIf_TxPduRef)[PduIdx];

    /* create temporary buffer for constructing the frame */
    uint32 aTempBuffer[FRIF_TMPPAYLOADBUFFER_U32_LENGTH];

    /* create an uint8 pointer to uint32 allocated memory */
    /* Deviation MISRAC2012-1 <+2> */
    CONSTP2VAR(uint8, AUTOMATIC,AUTOMATIC) pTempBuffer =
        (P2VAR(uint8, AUTOMATIC,AUTOMATIC))(P2VAR(void,AUTOMATIC,AUTOMATIC))&aTempBuffer[0];

    /* true if a transmission was triggered, or the frame shall always be transmitted */
    boolean isTriggerTx;

    uint8 txCnt;

    /* calculate real payload length for transmission */
    uint8 TxPayloadLength = 0x0U;

    /* remember whether the Pdu was updated */
    boolean PduUpdated = FALSE;

    /* Deviation MISRAC2012-1 */
    TS_AtomicAssign8(txCnt,((P2VAR(uint8,AUTOMATIC,FRIF_VAR))(P2VAR(void,AUTOMATIC,FRIF_VAR)) FrIf_Mem)[PduIdx]);

    isTriggerTx = (((txCnt)&FRIF_TXCNT_MASK) > 0x00U) || (TransmitMode == FRIF_TRANSMIT_SIMPLE_FRAME_NONE_MODE);

#if (FRIF_TX_FRAME_DEFAULT_VALUE_ENABLE == STD_ON)

    TS_MemSet32_NoCheck(aTempBuffer,FRIF_TX_FRAME_DEFAULT_VALUE,FrIf_OperationPtr->LPduLength);

#endif /* FRIF_TX_FRAME_DEFAULT_VALUE_ENABLE */

#if (FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE == STD_ON)

    if(PduPtr->DynLenEnable_LPduIdx == 0U)

#endif /* FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE */
    {
        TxPayloadLength = FrIf_OperationPtr->LPduLength;
    }

            /* check if a transmission was triggered, or that frame shall always be transmitted */
    if ((isTriggerTx != FALSE) || (TransmitMode == FRIF_ALWAYS_TRANSMIT_SIMPLE_FRAME))
    {
        if (isTriggerTx != FALSE)
        {
            /* create PduInfoType pduInfo */
            PduInfoType pduInfo;
            Std_ReturnType ULTriggerTransmitRetVal = E_NOT_OK;

            /* set pointer to temp buffer for Pdu data */
            pduInfo.SduDataPtr = pTempBuffer;

            /* set maximal length for Pdu data */
            pduInfo.SduLength = FrIf_OperationPtr->LPduLength;

#if FRIF_UL_ASR43_API_ENABLE == STD_ON
            if(FrIf_OperationPtr->PduPositionNum_OwnerIdx >= FrIf_FirstASR43OwnerIdx)
            {
              ULTriggerTransmitRetVal = FrIf_PduOwnerFuncASR43Ptr[FrIf_OperationPtr->PduPositionNum_OwnerIdx - FrIf_FirstASR43OwnerIdx]
                   .FrIfTriggerTransmit(PduPtr->ConfPduIdx, &pduInfo);
            }
            else
#endif /* FRIF_UL_ASR43_API_ENABLE == STD_ON */
            {
              ULTriggerTransmitRetVal = FrIf_PduOwnerFuncASR40Ptr[FrIf_OperationPtr->PduPositionNum_OwnerIdx]
                   .FrIfTriggerTransmit(PduPtr->ConfPduIdx, &pduInfo);
            }
            /* get PDU data, store to temp buffer */
            if( ULTriggerTransmitRetVal == E_OK )
            {
                /* OK, Pdu is up-to-date */
                PduUpdated = TRUE;

                /* using dynamic payload length support for FrIf ? */
#if (FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE == STD_ON)

                /* if this is a dynamic byte Pdu, then evaluate dynamic LPDu length */
                if(PduPtr->DynLenEnable_LPduIdx != 0U)
                {
                    /* in Flexray at maximum 254 byte Pdus are supported */
                    TxPayloadLength = (uint8)pduInfo.SduLength;
                }

#endif /* FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE */

            }
            else
            {
                /* reset trigger counter */
                /* Deviation MISRAC2012-1 */
                TS_AtomicAssign8(txCnt,((P2VAR(uint8,AUTOMATIC,FRIF_VAR))(P2VAR(void,AUTOMATIC,FRIF_VAR)) FrIf_Mem)[PduIdx]);

                txCnt &= ~(uint8)FRIF_TXCNT_MASK;

                /* Deviation MISRAC2012-1 */
                TS_AtomicAssign8(((P2VAR(uint8,AUTOMATIC,FRIF_VAR))(P2VAR(void,AUTOMATIC,FRIF_VAR)) FrIf_Mem)[PduIdx],txCnt);
            }

        }

        if((PduUpdated != FALSE) || (TransmitMode == FRIF_ALWAYS_TRANSMIT_SIMPLE_FRAME))
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
                /* !LINKSTO FrIf.EB.BusmirrorSupport.DecoupledTransmission.FrTransmitTxLPdu,1 */
                /* transmit frame built within this function */
                const Std_ReturnType RetCode =
                    FrIf_Call_Fr_Arg5(FRIF_FR_INSTANCE_IDX,TransmitTxLPdu,
                            FRIF_FR_CTRLIDX,
                            FrIf_OperationPtr->LPduIdx,
                            pTempBuffer,
                            TxPayloadLength,
                            SlotAssignmentPtr
                         );
#else
                /* transmit frame built within this function */
                const Std_ReturnType RetCode =
                    FrIf_Call_Fr_Arg4(FRIF_FR_INSTANCE_IDX,TransmitTxLPdu,
                            FRIF_FR_CTRLIDX,
                            FrIf_OperationPtr->LPduIdx,
                            pTempBuffer,
                            TxPayloadLength
                         );
#endif /* FRIF_FR_DRIVER_AUTOSAR_VERSION == FRIF_FR_ASR_44_SUPPORT */

                /* if transmission succeeded */
                if(RetCode == E_OK)
                {
                    /* access to FrIf_Mem[PduIdx] is critical (cancel transmission or FrIf_Transmit) */
                    /* therefore protect by critical area */
                    SchM_Enter_FrIf_SCHM_FRIF_TX_ADMINISTRATION();

                    {
                        /* obtain trigger counter and pending bit */
                        /* Deviation MISRAC2012-1 */
                        TS_AtomicAssign8(txCnt,((P2VAR(uint8,AUTOMATIC,FRIF_VAR))(P2VAR(void,AUTOMATIC,FRIF_VAR)) FrIf_Mem)[PduIdx]);

                        /* decrease transmit counter --> the PDU is
                         * transmitted, regarding the counter */
                        if (isTriggerTx != FALSE)
                        {
                            txCnt = ((txCnt&FRIF_TXCNT_MASK) > 0x00U) ? (txCnt - 1U) : 0U;

                            /* If Pdu requires tx confirmation then pending bit must be set */
                            if(PduIdx >= FrIf_RootConfigPtr->FrIf_TxPduNoConfNum)
                            {
                                /* mark Pdu transmission pending information */
                                txCnt |= FRIF_PENDINGBIT_SETBITMASK;
                            }
                        }

                        /* write back transmission counter and pending bit */
                        /* Deviation MISRAC2012-1 */
                        TS_AtomicAssign8(((P2VAR(uint8,AUTOMATIC,FRIF_VAR))(P2VAR(void,AUTOMATIC,FRIF_VAR)) FrIf_Mem)[PduIdx],txCnt);
                    }

                    /* Leave critical area */
                    SchM_Exit_FrIf_SCHM_FRIF_TX_ADMINISTRATION();

#if (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON)
                    {
#if (FRIF_SINGLE_CLST_OPT_ENABLE == STD_OFF)
                        uint8 FrIf_ClstIdx = FRIF_GET_CONFIG_ADDR(FrIf_CtrlType, FrIf_RootConfigPtr->FrIf_CtrlRef)[FRIF_FR_CTRLIDX].ClstIdx;
#endif /* FRIF_SINGLE_CLST_OPT_ENABLE == STD_OFF */

                        /* create PduInfoType pduInfo */
                        PduInfoType pduInfo;

                        /* prepare pduInfo for BusMirror */
                        pduInfo.SduDataPtr = pTempBuffer;
                        pduInfo.SduLength = TxPayloadLength;

                        if(FrIf_ClstRuntimeData[FRIF_CLSTIDX].BusmirroringEnabled == TRUE)
                        {
                            /* !LINKSTO FrIf.EB.BusmirrorSupport.DecoupledTransmission.MirrorReportFlexRayFrame,1 */
                            Mirror_ReportFlexRayFrame
                                (
                                    FRIF_FR_CTRLIDX,
                                    SlotAssignment.SlotId,
                                    SlotAssignment.Cycle,
                                    SlotAssignment.channelId,
                                    &pduInfo,
                                    FALSE
                                );
                        }
                    }
#endif /* (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON) */

                }
#if FRIF_UL_ASR43_API_ENABLE == STD_ON
                /* If Pdu owner is a ASR43 conform upper layer, discard message and inform UL with TXConf NOT_OK */
                else
                {
                  if(FrIf_OperationPtr->PduPositionNum_OwnerIdx >= FrIf_FirstASR43OwnerIdx)
                  {
                      /* If Pdu requires tx confirmation and has not been transmitted then UL has to be informed */
                      if(PduIdx >= FrIf_RootConfigPtr->FrIf_TxPduNoConfNum)
                      {
                        /* call upper layer and indicate failing transmission */
                        FrIf_PduOwnerFuncASR43Ptr[FrIf_OperationPtr->PduPositionNum_OwnerIdx - FrIf_FirstASR43OwnerIdx]
                            .FrIfTxConfirmation((PduIdType)PduPtr->ConfPduIdx, E_NOT_OK);

                        /* access to FrIf_Mem[PduPtr->TxCounterDIdx] is critical (FrIf_Transmit)  */
                        /* therefore protect by critical area */
                        SchM_Enter_FrIf_SCHM_FRIF_TX_ADMINISTRATION();

                        /* Deviation MISRAC2012-1 */
                        TS_AtomicAssign8(txCnt,((P2VAR(uint8,AUTOMATIC,FRIF_VAR))(P2VAR(void,AUTOMATIC,FRIF_VAR)) FrIf_Mem)[PduIdx]);

                        /* decrease transmit counter --> the PDU is
                         * transmitted, regarding the counter */
                        txCnt = ((txCnt&FRIF_TXCNT_MASK) > 0x00U) ? (txCnt - 1U) : 0U;

                        /* write back transmission counter */
                        /* Deviation MISRAC2012-1 */
                        TS_AtomicAssign8(((P2VAR(uint8,AUTOMATIC,FRIF_VAR))(P2VAR(void,AUTOMATIC,FRIF_VAR)) FrIf_Mem)[PduIdx],txCnt);

                        /* Leave critical area */
                        SchM_Exit_FrIf_SCHM_FRIF_TX_ADMINISTRATION();
                      }
                    }
                }
#endif /* FRIF_UL_ASR43_API_ENABLE == STD_ON */
            }
        }

    }

}

#endif /* FRIF_DECOUPLED_TX_ENABLE */

#if (FRIF_EXTENDED_RXFIFO_ENABLE == STD_ON)

/**
 * \brief  This function returns the frame structure information base
 */
STATIC FUNC_P2CONST(FrIf_PduPositionType, AUTOMATIC, FRIF_CODE) FrIf_LookUpRxFIFOEntry(
    uint8 FrIf_CtrlIdx,
    uint16 FrameID,
    uint8 Channel,
    uint8 Cycle,
    P2VAR(uint8,AUTOMATIC,FRIF_APPL_DATA) PduPositionNumPtr
    );


STATIC FUNC_P2CONST(FrIf_PduPositionType, AUTOMATIC, FRIF_CODE) FrIf_LookUpRxFIFOEntry(
    uint8 FrIf_CtrlIdx,
    uint16 FrameID,
    uint8 Channel,
    uint8 Cycle,
    P2VAR(uint8,AUTOMATIC,FRIF_APPL_DATA) PduPositionNumPtr
    )
{
    /* initialize default output values */
    uint8 PduPositionNum = 0U;
    P2CONST(FrIf_PduPositionType,AUTOMATIC,FRIF_APPL_CONST) pPduPosition = NULL_PTR;

    /* obtain pointer to FIFO config */
    CONSTP2CONST(FrIf_RxFIFOType,AUTOMATIC,FRIF_APPL_CONST) pRxFIFO =
        &(FRIF_GET_CONFIG_ADDR(FrIf_CtrlType, FrIf_RootConfigPtr->FrIf_CtrlRef)[FRIF_CTRLIDX].ChannelRxFIFO[Channel]);

    /* obtain pointer to FrameID Array */
    CONSTP2CONST(FrIf_RxFIFOFrameIDType,AUTOMATIC,FRIF_APPL_CONST) pFrameIDArray =
        FRIF_GET_CHECKED_CONFIG_ADDR(FrIf_RxFIFOFrameIDType, pRxFIFO->FrameIDRef);

    /* calculate FrameID table lookup index */
     const sint16 LookUpIndex = (sint16)FrameID - (sint16)pRxFIFO->LowerFrameID;


    /* check if a FIFO demultiplex table is configured */
    if( ( (pFrameIDArray != NULL_PTR) && (LookUpIndex >= 0) ) && (LookUpIndex < ((sint16)pRxFIFO->FrameIDNum)) )
    {
        CONSTP2CONST(FrIf_LPduType,AUTOMATIC,FRIF_APPL_CONST) pLPduArray =
            FRIF_GET_CHECKED_CONFIG_ADDR(FrIf_LPduType, pFrameIDArray[LookUpIndex].CycleToLPduRef);

        /* check if this FrameID is configured or shall be discarded */
        if(pLPduArray != NULL_PTR)
        {
            /* get the pointer of the LPdus structure of the cycle demultiplexed LPdu */
            pPduPosition =
              FRIF_GET_CONFIG_ADDR(FrIf_PduPositionType,pLPduArray[Cycle&(pFrameIDArray[LookUpIndex].CycleRepetition -1U)].PduPositionRef);

            /* get the number of Pdus in the LPdu */
            PduPositionNum = pLPduArray[Cycle&(pFrameIDArray[LookUpIndex].CycleRepetition -1U)].PduPositionNum;
        }
    }

    *PduPositionNumPtr = PduPositionNum;  /* store number of Pdus in this frame */

    return pPduPosition;                  /* return pointer to Pdu structure */
}


/**
 * \brief  'ConsumeRxFIFO' job operation implementation
 */
STATIC FUNC(void,FRIF_CODE) FrIf_ConsumeRxFIFO
    (
        CONSTP2CONST(FrIf_ComOpType, AUTOMATIC, FRIF_APPL_CONST) FrIf_OperationPtr
    );

STATIC FUNC(void,FRIF_CODE) FrIf_ConsumeRxFIFO
    (
        CONSTP2CONST(FrIf_ComOpType, AUTOMATIC, FRIF_APPL_CONST) FrIf_OperationPtr
    )
{
/* if no single controller implementation */
#if (FRIF_SINGLE_CTRL_OPT_ENABLE == STD_OFF)
    /* obtain actual controller index */
    const uint8 FrIf_CtrlIdx = FrIf_OperationPtr->FrIfCtrlIdx;
#endif /* FRIF_SINGLE_CTRL_OPT_ENABLE */

    /* variable for obtaining RX-Status */
    Fr_RxLPduStatusType RxStatus = FR_NOT_RECEIVED;
    uint16_least RxCounter = 0U;

    do
    {
        /* get Received payload length here */
        uint8 RxLength;

        /* get Received payload length here */
        uint32 aTempBuffer[FRIF_TMPPAYLOADBUFFER_U32_LENGTH];

        /* Deviation MISRAC2012-1 <+2> */
        CONSTP2VAR(uint8,AUTOMATIC,AUTOMATIC) pTempBuffer =
          (P2VAR(uint8,AUTOMATIC,AUTOMATIC))(P2VAR(void,AUTOMATIC,AUTOMATIC))&aTempBuffer[0];

        uint16 FrameID;         /* local variable to save the FrameID for RxFIFO entry */
        Fr_ChannelType Channel; /* local variable to save the Channel for RxFIFO entry */
        uint8 Cycle;            /* local variable to save the Cycle for RxFIFO entry */

        /* initialize with default error behaviour (nothing received) because
         * it's value stays untouched if reception fails by any reason
         */
        RxStatus = FR_NOT_RECEIVED;

        /* Receive frame from Fr */
        /* don't evaluate return code, since default behaviour is already
         * initialized
         */
        FrIf_Call_Fr_Arg8(FRIF_FR_INSTANCE_IDX,ReceiveRxFIFO,
                FRIF_FR_CTRLIDX,
                (uint8)FrIf_OperationPtr->LPduIdx,
                &FrameID,
                &Channel,
                &Cycle,
                pTempBuffer,
                &RxStatus,
                &RxLength
             );

        /* proceed further only, if we received a frame */
        if(RxStatus != FR_NOT_RECEIVED)
        {
            uint8 PduPositionNum; /* number of Pdus stored in the LPdu (number of Pdu positions) */

            /* obtain pointer to configuration data */
            CONSTP2CONST(FrIf_PduPositionType, AUTOMATIC,FRIF_APPL_CONST) PduPositionStartPtr =
                FrIf_LookUpRxFIFOEntry(FRIF_FR_CTRLIDX, FrameID, (uint8)Channel, Cycle, &PduPositionNum);

            /* create Pdu-iterator */
            uint16_least iPdu;

            /* iterate over Pdus contained in Frame */
            for (iPdu = 0U; iPdu < (uint16_least)PduPositionNum; iPdu++)
            {

                /* obtain pointer to configuration data */
                CONSTP2CONST(FrIf_PduPositionType, AUTOMATIC,FRIF_APPL_CONST) PduPositionPtr =
                    &PduPositionStartPtr[iPdu];

#if FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE == STD_ON
                /* check if update bit and payload start offset is within received payload */
                /* otherwise, ignore this Pdu */
                if((PduPositionPtr->UB_ByteOffset < RxLength) && (PduPositionPtr->PduOffset < RxLength))
#endif /* FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE */
                {
                    /* obtain UB Bit position */
                    const uint8 ubBitPos =
                        FRIF_UB_GETBITPOS(PduPositionPtr->UB_BitPos_OwnerIdx);

                    if((ubBitPos == FRIF_UPDATEBIT_NONE) ||
                       ((pTempBuffer[PduPositionPtr->UB_ByteOffset]&FRIF_UB_GETBITMASK(ubBitPos)) != 0U))
                    {
                        /* create PduInfoType pduInfo */
                        PduInfoType pduInfo;

                        /* get owner Idx */
                        const uint8 OwnerIdx = FRIF_GET_OWNERIDX(PduPositionPtr->UB_BitPos_OwnerIdx);

                        /* set pointer to Pdu payload */
                        pduInfo.SduDataPtr = &pTempBuffer[PduPositionPtr->PduOffset];

#if FRIF_UL_ASR43_API_ENABLE == STD_ON
                        if(OwnerIdx >= FrIf_FirstASR43OwnerIdx)
                        {
                          /* provide reception indication to upper layer */
                          FrIf_PduOwnerFuncASR43Ptr[OwnerIdx - FrIf_FirstASR43OwnerIdx]
                              .FrIfRxIndication((PduIdType)PduPositionPtr->PduIdx, &pduInfo);
                        }
                        else
#endif /* FRIF_UL_ASR43_API_ENABLE == STD_ON */
                        {
                          /* provide reception indication to upper layer */
                          FrIf_PduOwnerFuncASR40Ptr[OwnerIdx]
                              .FrIfRxIndication((PduIdType)PduPositionPtr->PduIdx, &pduInfo);
                        }
                    }
                }
            }
        }

        RxCounter++;
    /* continue only in case there is more data available in the FIFO
       indicated by RxStatus == FR_RECEIVED_MORE_DATA_AVAILABLE and
       the maximal number of Fr_ReceiveRxLPdu() calls has not been reached */
    } while(    (RxStatus == FR_RECEIVED_MORE_DATA_AVAILABLE)
             && (RxCounter < FRIF_GLOBALRXMAXLOOP) );

}

#endif /* FRIF_EXTENDED_RXFIFO_ENABLE */


/**
 * \brief  'Transmit Frame' job operation implementation
 */
STATIC FUNC(void,FRIF_CODE) FrIf_InternDispatchComOps
    (
        P2CONST(FrIf_JobType, AUTOMATIC, FRIF_APPL_CONST) FrIf_JobPtr,
        FrIf_StateType FrIf_CurrentState
    );

STATIC FUNC(void,FRIF_CODE) FrIf_InternDispatchComOps
    (
        P2CONST(FrIf_JobType, AUTOMATIC, FRIF_APPL_CONST) FrIf_JobPtr,
        FrIf_StateType FrIf_CurrentState
    )
{
    /* get start pointer to ComOps of this job */
    P2CONST(FrIf_ComOpType,AUTOMATIC,FRIF_APPL_CONST) OperationStartPtr =
        FRIF_GET_CONFIG_ADDR(FrIf_ComOpType, FrIf_JobPtr->ComOpRef);

    /* variable for ComOp iterating */
    uint16_least iOperation;

    /* iterate over ComOps */
    for(iOperation = 0U; iOperation < (uint16_least)FrIf_JobPtr->ComOpNum; iOperation++)
    {

        /* get pointer to ComOp */
        P2CONST(FrIf_ComOpType,AUTOMATIC,FRIF_APPL_CONST) OperationPtr =
            &OperationStartPtr[iOperation];

        /* evaluate communication operation */
        switch(OperationPtr->Action)
        {

            /* handle transmission ComOps */
#if (FRIF_DECOUPLED_TX_ENABLE == STD_ON)
            case FRIF_TRANSMIT_FRAME:
            case FRIF_ALWAYS_TRANSMIT_FRAME:
            case FRIF_TRANSMIT_FRAME_NONE_MODE:
            {
                if(FrIf_CurrentState == FRIF_STATE_ONLINE)
                {
                    FrIf_TransmitFrame(OperationPtr,OperationPtr->Action);
                }
                break;
            }
            /* optimized ComOps for simple frames (frames with one pdu and no update bit) */
            case FRIF_TRANSMIT_SIMPLE_FRAME:
            case FRIF_ALWAYS_TRANSMIT_SIMPLE_FRAME:
            case FRIF_TRANSMIT_SIMPLE_FRAME_NONE_MODE:
            {
                if(FrIf_CurrentState == FRIF_STATE_ONLINE)
                {
                    FrIf_TransmitSimpleFrame(OperationPtr,OperationPtr->Action);
                }
                break;
            }
#endif /* FRIF_DECOUPLED_TX_ENABLE */

            /* handle tx confirmation ComOps */
#if (FRIF_TXCONFIRMATION_ENABLE == STD_ON)
            case FRIF_CONFIRM_IMM_FRAME: /* equals FRIF_CONFIRM_IMM_SIMPLE_FRAME */
            case FRIF_CONFIRM_DEC_FRAME: /* equals FRIF_CONFIRM_DEC_SIMPLE_FRAME */
            {
                if(FrIf_CurrentState == FRIF_STATE_ONLINE)
                {
                    FrIf_TxConfirmation(OperationPtr,OperationPtr->Action);
                }
                break;
            }
#endif /* FRIF_TXCONFIRMATION_ENABLE */

            /* handle rx ComOps */
#if (FRIF_DECOUPLED_RX_ENABLE == STD_ON)
            case FRIF_RECEIVE_FRAME:
            {
                if(FrIf_CurrentState == FRIF_STATE_ONLINE)
                {
                    FrIf_ReceiveAndStore(OperationPtr);
                }
                break;
            }
            case FRIF_INDICATE_FRAME:
            {
                if(FrIf_CurrentState == FRIF_STATE_ONLINE)
                {
                    FrIf_ReceiveIndication(OperationPtr);
                }
                break;
            }
#endif /* FRIF_DECOUPLED_RX_ENABLE */


            /* handle rx ComOps Rx and indicate*/
#if (FRIF_IMMEDIATE_RX_ENABLE == STD_ON)
            case FRIF_RECEIVE_INDICATE_FRAME:
            {
                if(FrIf_CurrentState == FRIF_STATE_ONLINE)
                {
                    FrIf_ReceiveAndIndicateFrame(OperationPtr);
                }
                break;
            }
            case FRIF_RECEIVE_INDICATE_SIMPLE_FRAME:
            {
                if(FrIf_CurrentState == FRIF_STATE_ONLINE)
                {
                    FrIf_ReceiveAndIndicateSimpleFrame(OperationPtr);
                }
                break;
            }
#endif /* FRIF_IMMEDIATE_RX_ENABLE */

            /* handle reconfiguration ComOp */
#if (FRIF_JOBLIST_PREPARE_LPDU_ENABLE == STD_ON)
            case FRIF_PREPARE_FRAME: /* equals FRIF_PREPARE_SIMPLE_FRAME */
            {
                FrIf_PrepareFrame(OperationPtr);
                break;
            }
#endif /* FRIF_JOBLIST_PREPARE_LPDU_ENABLE */

#if (FRIF_EXTENDED_RXFIFO_ENABLE == STD_ON)
            case FRIF_RECEIVE_INDICATE_RXFIFO:
            {
                if(FrIf_CurrentState == FRIF_STATE_ONLINE)
                {
                    FrIf_ConsumeRxFIFO(OperationPtr);
                }
                break;
            }
#endif /* FRIF_EXTENDED_RXFIFO_ENABLE */

            /* CHECK: NOPARSE */
            /* 'default' cannot be reached */
            default:
            {
                FRIF_UNREACHABLE_CODE_ASSERT(FRIF_UNKNOWN_SERVICE_ID);
                break;
            }
            /* CHECK: PARSE */
        }
    }

}

/*======================[Global Functions]====================================*/

FUNC(Std_ReturnType,FRIF_CODE) FrIf_DispatchComOps
    (
        uint8 FrIf_ClstIdx,
        uint16 FrIf_JobIdx,
        uint8 FrIf_Cycle
    )
{
    Std_ReturnType RetValue = E_NOT_OK;

    DBG_FRIF_DISPATCHCOMOPS_ENTRY(FrIf_ClstIdx,FrIf_JobIdx,FrIf_Cycle);

/* check if development error detection is disabled */
#if (FRIF_DEV_ERROR_DETECT == STD_OFF)

#if (FRIF_SINGLE_CLST_OPT_ENABLE == STD_ON)

    TS_PARAM_UNUSED(FrIf_ClstIdx);

#endif /* FRIF_SINGLE_CLST_OPT_ENABLE */

    TS_PARAM_UNUSED(FrIf_Cycle);

#else /* FRIF_DEV_ERROR_DETECT == STD_OFF */
    /* check for successfully initialized module */
    /* Report to DET and return Error in case module was not initialized before */
    if(FrIf_ModuleInitialized == FRIF_UNINIT)
    {
        /* Report to DET */
        FrIf_Det_ReportError(FRIF_JOBLISTEXECUTION_SERVICE_ID, FRIF_E_NOT_INITIALIZED);
    }

#if (FRIF_SINGLE_CLST_OPT_ENABLE == STD_ON)

    /* check that cluster index is 0 */
    else if (FrIf_ClstIdx != (uint8) 0U)

#else /* FRIF_SINGLE_CLST_OPT_ENABLE */

    /* check whether configuration has an entry for this FlexRay Cluster Idx */
    else if(FRIF_CLSTIDX >= FrIf_RootConfigPtr->FrIf_ClstNum)

#endif  /* FRIF_SINGLE_CLST_OPT_ENABLE */
    {

        /* Report to DET */
        FrIf_Det_ReportError(FRIF_JOBLISTEXECUTION_SERVICE_ID, FRIF_E_INV_CLST_IDX);
    }

    /* check whether a correct cycle value is passed */
    else if(FrIf_Cycle >= 64U)
    {

        /* Report to DET */
        FrIf_Det_ReportError(FRIF_JOBLISTEXECUTION_SERVICE_ID, FRIF_E_INV_CYCLE);
    }

    else
    {
        /* get cluster configuration pointer */
        CONSTP2CONST(FrIf_ClstType, AUTOMATIC, FRIF_APPL_CONST) ClstPtr =
            &FRIF_GET_CONFIG_ADDR(FrIf_ClstType, FrIf_RootConfigPtr->FrIf_ClstRef)[FRIF_CLSTIDX];

        /* check whether configuration has an entry for this Job Idx */
        if(FrIf_JobIdx >= ClstPtr->JobNum)
        {

            /* Report to DET */
            FrIf_Det_ReportError(FRIF_JOBLISTEXECUTION_SERVICE_ID, FRIF_E_INV_JOB_IDX);
        }

        else
        {
            RetValue = E_OK;
        }
    }

    if(RetValue == E_OK)

#endif  /* FRIF_DEV_ERROR_DETECT == STD_OFF */
    {
        /* get cluster configuration pointer */
        CONSTP2CONST(FrIf_ClstType, AUTOMATIC, FRIF_APPL_CONST) ClstPtr =
            &FRIF_GET_CONFIG_ADDR(FrIf_ClstType, FrIf_RootConfigPtr->FrIf_ClstRef)[FRIF_CLSTIDX];

        /* get Job Pointer  */
        CONSTP2CONST(FrIf_JobType, AUTOMATIC, FRIF_APPL_CONST) JobPtr =
            &FRIF_GET_CONFIG_ADDR(FrIf_JobType, ClstPtr->JobRef)[FrIf_JobIdx];

        /* get current state */
        FrIf_StateType CurrentState;

        uint8 State;

        /* Read FrIf_State in an atomic fashion */
        TS_AtomicAssign8(State,FrIf_ClstRuntimeData[FRIF_CLSTIDX].FrIf_State);

        /* convert state to enum */
        CurrentState = (FrIf_StateType)State;

        FrIf_InternDispatchComOps(JobPtr,
                                  CurrentState);

        RetValue = E_OK;
    }

    /* return result */

    DBG_FRIF_DISPATCHCOMOPS_EXIT(RetValue,FrIf_ClstIdx,FrIf_JobIdx,FrIf_Cycle);
    return RetValue;
}


FUNC(void,FRIF_CODE) FrIf_JobListExec( uint8 FrIf_ClstIdx)
{
    DBG_FRIF_JOBLISTEXEC_ENTRY(FrIf_ClstIdx);

/* check if development error detection is enabled */
#if (FRIF_DEV_ERROR_DETECT == STD_ON)

    /* check for successfully initialized module */
    /* Report to DET and return Error in case module was not initialized before */

    if(FrIf_ModuleInitialized == FRIF_UNINIT)
    {
        /* Report to DET */
        FrIf_Det_ReportError(FRIF_JOBLISTEXECUTION_SERVICE_ID, FRIF_E_NOT_INITIALIZED);
    }

    else

#endif  /* FRIF_DEV_ERROR_DETECT */
    {
        /* get cluster configuration pointer */
        CONSTP2CONST(FrIf_ClstType, AUTOMATIC, FRIF_APPL_CONST) ClstPtr =
            &FRIF_GET_CONFIG_ADDR(FrIf_ClstType, FrIf_RootConfigPtr->FrIf_ClstRef)[FRIF_CLSTIDX];

#if (FRIF_SINGLE_CTRL_OPT_ENABLE == STD_OFF)
        /* get the index of the Cluster-responsible controller */
        const uint8 FrIf_CtrlIdx = ClstPtr->JLE_FrIf_CtrlIdx;
#endif /* FRIF_SINGLE_CTRL_OPT_ENABLE */

        /* get the index of the Cluster-responsible timer */
        const uint8 AbsTimerIdx = ClstPtr->JLE_FrIf_AbsTimerIdx;

#if (FRIF_SINGLE_CLST_OPT_ENABLE == STD_ON)

        TS_PARAM_UNUSED(FrIf_ClstIdx);

#endif /* FRIF_SINGLE_CLST_OPT_ENABLE */
        {
/* if joblist IRQ multiplexing is enabled, check whether the responsible interrupt is pending */
#if (FRIF_JOBLIST_IRQMUX_ENABLE == STD_ON)
          /* variable for the pending status */
          /* initialize with error default value */
          boolean IRQPending = FALSE;

          {
              (void)FrIf_GetAbsoluteTimerIRQStatus(FRIF_CTRLIDX,AbsTimerIdx,&IRQPending);
          }
          if(IRQPending == TRUE)

#endif /* FRIF_JOBLIST_IRQMUX_ENABLE */
          {
              uint8 JobListTimeoutCounter;
              uint8 FrIf_SyncState;

              /* acknowledge timer invocation */
              (void)FrIf_AckAbsoluteTimerIRQ(FRIF_CTRLIDX,AbsTimerIdx);

              TS_AtomicAssign8(JobListTimeoutCounter,FrIf_ClstRuntimeData[FRIF_CLSTIDX].JobListTimeoutCounter);
              TS_AtomicAssign8(FrIf_SyncState,FrIf_ClstRuntimeData[FRIF_CLSTIDX].FrIf_SyncState);

              /* now let's execute the joblist - if no timeout has been detected prior */
              if((JobListTimeoutCounter < ClstPtr->JobListTimeoutLimit) &&
                 (FrIf_SyncState == FRIF_SYNCSTATE_SYNC))
              {

                  /* get JobStart Pointer  */
                  CONSTP2CONST(FrIf_JobType, AUTOMATIC, FRIF_APPL_CONST) JobStartPtr =
                      FRIF_GET_CONFIG_ADDR(FrIf_JobType, ClstPtr->JobRef);

                  /* get current joblist entry */
                  const uint16_least CurrentJob = FrIf_ClstRuntimeData[FRIF_CLSTIDX].NextJob;

                  /* get Job Pointer */
                  CONSTP2CONST(FrIf_JobType, AUTOMATIC, FRIF_APPL_CONST) JobPtr =
                      &JobStartPtr[CurrentJob];

                  /* get next joblist entry */
                  uint16_least NextJob = CurrentJob;

                  /* get current joblist cycle base */
                  const uint8 CurrentJobCycleBase =
                      FrIf_ClstRuntimeData[FRIF_CLSTIDX].JobListCycleBase;

                  /* get next joblist cycle base */
                  uint8 NextJobCycleBase = CurrentJobCycleBase;

#if (FRIF_JOBLIST_IRQDELAYCHECK_ENABLE == STD_ON)

                   boolean AbortFunctionFlag = FALSE;

                  /* variable for reading the current cycle */
                  uint8 CurrentCycle;

                  /* variable for reading the current offset */
                  uint16 CurrentOffset;

                  /* read the global time */
                  const Std_ReturnType RetCode =
                      FrIf_GetGlobalTime(FRIF_CTRLIDX,&CurrentCycle,&CurrentOffset);

                  /* check for synchronized joblist */
                  const uint16 MTPerCycle = FrIf_GetMacroticksPerCycle(FRIF_CTRLIDX);

                  const uint8 CurrentExpandedJob = JobPtr->Cycle + CurrentJobCycleBase;

                  const sint32 PlannedInvocationTime =
                      (((sint32)MTPerCycle)*
                      ((sint32)CurrentExpandedJob)) +
                      ((sint32)JobPtr->Offset);

                  /* calculate total FlexRay cycle length in units of macroticks */
                  /* = 64 communication cycles */
                  const sint32 TotalCycleLength =
                      (sint32)(((sint32)MTPerCycle)*((sint32)64));


                  /* calculate half of total FlexRay cycle length in units of macroticks */
                  const sint32 HalfTotalCycleLength = TotalCycleLength/2;

                  /* calculate absolute current invocation time */
                  const sint32 CurrentInvocationTime =
                      (((sint32)MTPerCycle)*((sint32)CurrentCycle)) + ((sint32)CurrentOffset);

                  /* perform joblist synchronization check */
                  if(RetCode != E_OK)
                  {

#if (FRIF_DISABLEIRQ_API_ENABLE == STD_ON)

                  /* absolute timer shall be disabled here */
                  (void)FrIf_DisableAbsoluteTimerIRQ(FRIF_CTRLIDX,AbsTimerIdx);

#endif /* FRIF_DISABLEIRQ_API_ENABLE */

                      /* access to timeout variable is critical (MainFunction) */
                      /* enter critical area */
                      SchM_Enter_FrIf_SCHM_FRIF_JOBLIST_EXECUTION();

                      /* set to timeout */
                      TS_AtomicAssign8(FrIf_ClstRuntimeData[FRIF_CLSTIDX].JobListTimeoutCounter,
                          ClstPtr->JobListTimeoutLimit);

                      /* leave critical area */
                      SchM_Exit_FrIf_SCHM_FRIF_JOBLIST_EXECUTION();

                      /* Abort JobList execution */
                      AbortFunctionFlag = TRUE;
                  }

                  if(AbortFunctionFlag == FALSE)
                  {
                      /* variable to calculate difference to MaxISRDelay */
                      sint32 DiffTime;

                      /* create absolute difference value */
                      if(PlannedInvocationTime >= CurrentInvocationTime)
                      {
                           DiffTime = PlannedInvocationTime - CurrentInvocationTime;
                      }
                      else
                      {
                          DiffTime = CurrentInvocationTime - PlannedInvocationTime;
                      }

                      /* consider wrap around */
                      if(DiffTime > HalfTotalCycleLength)
                      {
                          DiffTime = TotalCycleLength - DiffTime;
                      }

                      /* check if difference is larger thanMaxISRDelay */
                      if(DiffTime > ((sint32)ClstPtr->MaxISRDelay))
                      {
#if (FRIF_DISABLEIRQ_API_ENABLE == STD_ON)

                          /* absolute timer shall be disabled here */
                          (void)FrIf_DisableAbsoluteTimerIRQ(FRIF_CTRLIDX,AbsTimerIdx);

#endif /* FRIF_DISABLEIRQ_API_ENABLE */

                          /* access to timeout variable is critical (MainFunction) */
                          /* enter critical area */
                          SchM_Enter_FrIf_SCHM_FRIF_JOBLIST_EXECUTION();

                          /* set to timeout */
                          TS_AtomicAssign8(FrIf_ClstRuntimeData[FRIF_CLSTIDX].JobListTimeoutCounter,ClstPtr->JobListTimeoutLimit);

                          /* leave critical area */
                          SchM_Exit_FrIf_SCHM_FRIF_JOBLIST_EXECUTION();

                          /* Abort JobList execution */
                          AbortFunctionFlag = TRUE;
                      }
#if (FRIF_PROD_ERR_HANDLING_JLE_SYNC == TS_PROD_ERR_REP_TO_DEM)
#if (FRIF_JLE_SYNC_PASSED == DEM_EVENT_STATUS_PREPASSED)
                      else /* Job list is in sync */
                      {
                        /* check if DEM reporting is enabled for this cluster */
                        if (ClstPtr->DemEventId_JLE_SYNC != 0U)
                        {
                          /* Report a Joblist sync error to the DEM */
                          Dem_ReportErrorStatus(ClstPtr->DemEventId_JLE_SYNC , FRIF_JLE_SYNC_PASSED);
                        }
                      }
#endif /* (FRIF_JLE_SYNC_PASSED == DEM_EVENT_STATUS_PREPASSED) */
#endif /* (FRIF_PROD_ERR_HANDLING_JLE_SYNC == TS_PROD_ERR_REP_TO_DEM) */
                  }
                  if(AbortFunctionFlag == FALSE)
#endif /* FRIF_JOBLIST_IRQDELAYCHECK_ENABLE */
                  {
                      /* Forward job to next entry */
                      NextJob++;

                      /* wrap around if end of list reached */
                      if(NextJob >= ClstPtr->JobNum)
                      {
                           NextJob = 0U;
                          /* %64 */
                          NextJobCycleBase = (CurrentJobCycleBase +
                                             (ClstPtr->JobListCycleRepetition))&0x3FU;
                      }

                      /* save next job pointer to global context */
                      FrIf_ClstRuntimeData[FRIF_CLSTIDX].NextJob = (uint16)NextJob;
                      FrIf_ClstRuntimeData[FRIF_CLSTIDX].JobListCycleBase = NextJobCycleBase;

                      SchM_Enter_FrIf_SCHM_FRIF_JOBLIST_EXECUTION();

                      /* joblist still executed - so reset execution timeout detection */
                      TS_AtomicAssign8(FrIf_ClstRuntimeData[FRIF_CLSTIDX].JobListTimeoutCounter,0U);

                      SchM_Exit_FrIf_SCHM_FRIF_JOBLIST_EXECUTION();

                      /* program next timer invocation */
                      (void)FrIf_SetAbsoluteTimer(
                          FRIF_CTRLIDX,
                          AbsTimerIdx,
                          (uint8)(NextJobCycleBase + JobStartPtr[NextJob].Cycle),
                          JobStartPtr[NextJob].Offset);

                      {
                          /* get current state */
                          FrIf_StateType CurrentState;

                          uint8 State;

                          /* Read FrIf_State in an atomic fashion */
                          TS_AtomicAssign8(State,FrIf_ClstRuntimeData[FRIF_CLSTIDX].FrIf_State);

                          /* convert state to enum */
                          CurrentState = (FrIf_StateType)State;

                          /* dispatch the communication operations for this job */
                          FrIf_InternDispatchComOps(JobPtr,
                                        CurrentState);

                      }
                  }
              }
          }
        }
    }

    DBG_FRIF_JOBLISTEXEC_EXIT(FrIf_ClstIdx);
}


FUNC(uint16,FRIF_CODE) FrIf_GetMacroticksPerCycle
    (
        uint8 FrIf_CtrlIdx
    )
{
    uint16 RetValue = 0U;

    DBG_FRIF_GETMACROTICKSPERCYCLE_ENTRY(FrIf_CtrlIdx);

/* check if development error detection is disabled */
#if (FRIF_DEV_ERROR_DETECT == STD_OFF)

    TS_PARAM_UNUSED(FrIf_CtrlIdx);

#else /* FRIF_DEV_ERROR_DETECT == STD_OFF */
    /* check for successfully initialized module */
    /* Report to DET and return Error in case module was not initialized before */
    if(FrIf_ModuleInitialized == FRIF_UNINIT)
    {
        /* report error to DET */
        FrIf_Det_ReportError(FRIF_GETMACROTICKSPERCYCLE_SERVICE_ID, FRIF_E_NOT_INITIALIZED);
    }

#if (FRIF_SINGLE_CTRL_OPT_ENABLE == STD_ON)

    /* check that controller index is 0 */
    else if (FrIf_CtrlIdx != (uint8) 0U)

#else /* FRIF_SINGLE_CTRL_OPT_ENABLE */

    /* check whether configuration has an entry for this FlexRay CC Idx */
    else if(FRIF_CTRLIDX >= FrIf_RootConfigPtr->FrIf_CtrlNum)

#endif  /* FRIF_SINGLE_CTRL_OPT_ENABLE */
    {

        /* Report to DET */
        FrIf_Det_ReportError(FRIF_GETMACROTICKSPERCYCLE_SERVICE_ID, FRIF_E_INV_CTRL_IDX);

    }

    else

#endif  /* FRIF_DEV_ERROR_DETECT == STD_OFF */
    {
        /* get the macroticks per cycle for that controller */
        RetValue = FRIF_GET_CONFIG_ADDR(FrIf_CtrlType, FrIf_RootConfigPtr->FrIf_CtrlRef)
                 [FRIF_CTRLIDX].MTperCycle;
    }

    /* return result */

    DBG_FRIF_GETMACROTICKSPERCYCLE_EXIT(RetValue,FrIf_CtrlIdx);
    return RetValue;
}


/* stop code section declaration */
#if (TS_MERGED_COMPILE == STD_OFF)
#define FRIF_STOP_SEC_CODE
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */
#endif /* TS_MERGED_COMPILE */

