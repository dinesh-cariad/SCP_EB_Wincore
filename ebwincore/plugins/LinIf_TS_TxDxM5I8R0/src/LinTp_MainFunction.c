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

/*==================[inclusions]============================================*/

#include <LinIf_Trace.h>

#include <LinIf_Cfg.h>        /* Generated configuration */
#include <TSAutosar.h>        /* EB specific standard types */

#if (LINIF_TP_SUPPORTED == STD_ON)

#define LINIF_NO_PBCFG_REQUIRED
#include <LinIf.h>            /* Module library API */
#include <LinIf_Int.h>        /* LinIf internal data */
#include <LinTp_Int.h>        /* LinTp internal data */

#include <PduR_LinTp.h>         /* PDU Router LIN-TP Interface */
#include <SchM_LinIf.h>         /* for protecting exclusive areas */
#if (LINIF_MASTER_SUPPORTED == STD_ON)
#include <BswM_LinTp.h>         /* Base software manager header file */
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

#if ((LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON))
#include <Det.h>              /* Default Error Tracer */
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON) */

/*==================[macros]================================================*/

/** \brief Aborts an ongoing RX transmission and informs the PduR that
 *         an unexpected PDU was received.
 */
#if (defined LinTp_StopRxUnexpPdu)
#error LinTp_StopRxUnexpPdu already defined!
#endif /* #if( defined LinTp_StopRxUnexpPdu)*/
#define LinTp_StopRxUnexpPdu(currentChannel)                             \
        (LinTp_StopRx((currentChannel), NTFRSLT_E_UNEXP_PDU))            \

/** \brief Aborts an ongoing RX transmission and informs the PduR that
 *         an invalid sequence number was received.
 */
#if (defined LinTp_StopRxWrongSn)
#error LinTp_StopRxWrongSn already defined!
#endif /* #if( defined LinTp_StopRxWrongSn)*/
#define LinTp_StopRxWrongSn(currentChannel)                              \
        (LinTp_StopRx((currentChannel), NTFRSLT_E_WRONG_SN))             \

/** \brief Aborts an ongoing RX transmission and informs the PduR that
 *         something went wrong
 */
#if (defined LinTp_StopRxNotOk)
#error LinTp_StopRxNotOk already defined!
#endif /* #if( defined LinTp_StopRxNotOk)*/
#define LinTp_StopRxNotOk(currentChannel)                                \
        (LinTp_StopRx((currentChannel), NTFRSLT_E_NOT_OK))               \

/** \brief Ends an ongoing RX transmission and informs the PduR that
 *         message was received completely
 */
#if (defined LinTp_StopRxOk)
#error LinTp_StopRxOk already defined!
#endif /* #if( defined LinTp_StopRxOk)*/
#define LinTp_StopRxOk(currentChannel)                                   \
        (LinTp_StopRx((currentChannel), NTFRSLT_OK))                     \

/** \brief Aborts an ongoing TX transmission and informs the PduR that
 *         something went wrong
 */
#if (defined LinTp_StopTxNotOk)
#error LinTp_StopTxNotOk already defined!
#endif /* #if( defined LinTp_StopTxNotOk)*/
#define LinTp_StopTxNotOk(currentChannel)                                \
        (LinTp_StopTx((currentChannel), NTFRSLT_E_NOT_OK))               \

/** \brief Aborts an ongoing TX transmission because a CS timeout occurs
 *         and informs the PduR
 */
#if (defined LinTp_StopTxTimeoutCS)
#error LinTp_StopTxTimeoutCS already defined!
#endif /* #if( definedLinTp_StopTxTimeoutCS)*/
#define LinTp_StopTxTimeoutCS(currentChannel)                            \
        (LinTp_StopTx((currentChannel), NTFRSLT_E_NOT_OK))               \

/** \brief Aborts an ongoing TX transmission because a AS timeout occurs
 *         and informs the PduR
 */
#if (defined LinTp_StopTxTimeoutAS)
#error LinTp_StopTxTimeoutAS already defined!
#endif /* #if( defined LinTp_StopTxTimeoutAS)*/
#define LinTp_StopTxTimeoutAS(currentChannel)                            \
        (LinTp_StopTx((currentChannel), NTFRSLT_E_TIMEOUT_A))            \

/** \brief Aborts an ongoing RX transmission because a CR timeout occurs
 *         and informs the PduR
 */
#if (defined LinTp_StopRxTimeoutCR)
#error LinTp_StopRxTimeoutCR already defined!
#endif /* #if( defined LinTp_StopRxTimeoutCR)*/
#define LinTp_StopRxTimeoutCR(currentChannel)                            \
        (LinTp_StopRx((currentChannel), NTFRSLT_E_TIMEOUT_CR))           \

#if (LINIF_MASTER_SUPPORTED == STD_ON)
/** \brief Ends an ongoing functional request
 */
#if (defined LinTp_StopFuncTransmissionOk)
#error LinTp_StopFuncTransmissionOk already defined!
#endif /* #if( defined LinTp_StopFuncTransmissionOk)*/
#define LinTp_StopFuncTransmissionOk(currentChannel)                     \
        (LinTp_StopFuncTransmission((currentChannel), NTFRSLT_OK))       \

/** \brief Aborts an ongoing functional request
 */
#if (defined LinTp_StopFuncTransmissionNotOk)
#error LinTp_StopFuncTransmissionNotOk already defined!
#endif /* #if( defined LinTp_StopFuncTransmissionNotOk)*/
#define LinTp_StopFuncTransmissionNotOk(currentChannel)                  \
        (LinTp_StopFuncTransmission((currentChannel), NTFRSLT_E_NOT_OK)) \

/** \brief Aborts an ongoing functional request because a AS timeout occurs
 *         and informs the PduR
 */
#if (defined LinTp_StopFuncTransmissionTimeoutAS)
#error LinTp_StopFuncTransmissionTimeoutAS already defined!
#endif /* #if( defined LinTp_StopFuncTransmissionTimeoutAS)*/
#define LinTp_StopFuncTransmissionTimeoutAS(currentChannel)                 \
        (LinTp_StopFuncTransmission((currentChannel), NTFRSLT_E_TIMEOUT_A)) \

#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

/** \brief Starts the CS timer, which starts to run after a TX confirmation
 *         and runs until the next MRF is due.
 */
#if (defined LinTp_StartTimerCS)
#error LinTp_StartTimerCS already defined!
#endif /* #if( defined LinTp_StartTimerCS)*/
#define LinTp_StartTimerCS(pTxTpConfig, pTpData)                         \
        ((pTpData)->Timer = (pTxTpConfig)->LinTpNcs)                     \


/** \brief Starts the AS timer, which starts to run after copying data from
 *         the PduR and runs until the next TX confirmation
 */
#if (defined LinTp_StartTimerAS)
#error LinTp_StartTimerAS already defined!
#endif /* #if( defined LinTp_StartTimerAS)*/
#define LinTp_StartTimerAS(pTpTxConfig, pTpData)                         \
        ((pTpData)->Timer = (pTpTxConfig)->LinTpNas)                     \

#if (LINIF_MASTER_SUPPORTED == STD_ON)

/** \brief Starts the P2 timer, which starts after whole message is transmitted
 *         completely and runs until RX indication with valid RSID is received.
 */
#if (defined LinTp_StartTimerP2)
#error LinTp_StartTimerP2 already defined!
#endif /* #if( defined LinTp_StartTimerP2)*/
#define LinTp_StartTimerP2(pTpConfig, pTpData)                           \
        ((pTpData)->P2Timer = (pTpConfig)->LinTpP2Timing)                \

/** \brief Starts the P2 max timer, which starts after reception of
*          response pending frame and runs until RX indication with valid
 *         RSID is received.
 */
#if (defined LinTp_StartTimerP2MAX)
#error LinTp_StartTimerP2MAX already defined!
#endif /* #if( defined LinTp_StartTimerP2MAX)*/
#define LinTp_StartTimerP2MAX(pTpConfig, pTpData)                         \
        ((pTpData)->P2Timer = (pTpConfig)->LinTpP2Max)                    \

#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

/** \brief Starts the CR timer, which starts after CF or FF was received and
 *         runs until next RX indication
 */
#if (defined LinTp_StartTimerCR)
#error LinTp_StartTimerCR already defined!
#endif /* #if( defined LinTp_StartTimerCR)*/
#define LinTp_StartTimerCR(pTpRxConfig, pTpData)                         \
        ((pTpData)->Timer = (pTpRxConfig)->LinTpNcr)                     \

/** \brief Stops the CS timer
 */
#if (defined LinTp_StopTimerCS)
#error LinTp_StopTimerCS already defined!
#endif /* #if( defined LinTp_StopTimerCS)*/
#define LinTp_StopTimerCS(pTpData)                                       \
        (LinTp_StopTimer((pTpData)))                                     \

/** \brief Stops the AS timer
 */
#if (defined LinTp_StopTimerAS)
#error LinTp_StopTimerAS already defined!
#endif /* #if( defined LinTp_StopTimerAS)*/
#define LinTp_StopTimerAS(pTpData)                                       \
        (LinTp_StopTimer((pTpData)))                                     \

#if (LINIF_MASTER_SUPPORTED == STD_ON)

/** \brief Stops the P2 timer
 */
#if (defined LinTp_StopTimerP2)
#error LinTp_StopTimerP2 already defined!
#endif /* #if( defined LinTp_StopTimerP2)*/
#define LinTp_StopTimerP2(pTpData)                                       \
        ((pTpData)->P2Timer = 0U)                                        \

#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

/** \brief Stops the CR timer
 */
#if (defined LinTp_StopTimerCR)
#error LinTp_StopTimerCR already defined!
#endif /* #if( defined LinTp_StopTimerCR)*/
#define LinTp_StopTimerCR(pTpData)                                      \
        (LinTp_StopTimer((pTpData)))                                    \

/** \brief Stops the timer
 */
#if (defined LinTp_StopTimer)
#error LinTp_StopTimer already defined!
#endif /* #if( defined LinTp_StopTimer)*/
#define LinTp_StopTimer(pTpData)                                        \
        ((pTpData)->Timer = 0U)                                         \

/*==================[type definitions]======================================*/

/*==================[internal function declarations]========================*/

#if (TS_MERGED_COMPILE == STD_OFF)
/* AUTOSAR Memory Mapping - start section for code */
#define LINIF_START_SEC_CODE
#include <LinIf_MemMap.h>
#endif /* TS_MERGED_COMPILE */


STATIC FUNC(void, LINIF_CODE) LinTp_StopRx
(
    uint8 currentChannel,
    NotifResultType Result
);

STATIC FUNC(void, LINIF_CODE) LinTp_CancelResponse
(
#if (LINIF_MASTER_SUPPORTED == STD_ON)
    uint8 currentChannel,
    uint8 LinTpOptions
#else
    uint8 currentChannel
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */
);

STATIC FUNC(void, LINIF_CODE) LinTp_StopTx
(
    uint8 currentChannel,
    NotifResultType Result
);

STATIC FUNC(void, LINIF_CODE) LinTp_StopTxOk
(
    uint8 currentChannel
);

STATIC FUNC(void, LINIF_CODE) LinTp_BuildHeader
(
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo,
    const LinTp_PCIType currentPCIType,
    const uint8 NAD,
    const uint16 PciAdditionalInfo
);

STATIC FUNC(void, LINIF_CODE) LinTp_GetNextFrame
(
    const uint16 MsgLength,
    const uint16 MsgLengthRemain,
    LinTp_PCIType *pciType,
    uint8 *PayloadLength
);

STATIC FUNC(LinTp_FrameReturnType, LINIF_CODE) LinTp_VerifyCF
(
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo,
    const uint8 FrameCounter
);

STATIC FUNC(Std_ReturnType, LINIF_CODE) LinTp_GetAvailablePduRTxBufferLength
(
    const PduIdType PduRDestPduHandleId,
    PduLengthType *AvailablePduRBufferLength
);

STATIC FUNC(Std_ReturnType, LINIF_CODE) LinTp_GetAvailablePduRRxBufferLength
(
    const PduIdType PduRPduHandleId,
    PduLengthType *AvailablePduRBufferLength
);

STATIC FUNC(Std_ReturnType, LINIF_CODE) LinTp_CopyTxPayloadPduR
(
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo,
    const LinTp_PCIType PciType,
    const uint8 PayloadLength,
    const PduIdType PduRPduHandleId,
    PduLengthType *AvailablePduRBufferLength
);

STATIC FUNC(Std_ReturnType, LINIF_CODE) LinTp_CopyRxPayloadPduR
(
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo,
    const LinTp_PCIType PciType,
    const uint8 PayloadLength,
    const PduIdType PduRPduHandleId,
    PduLengthType *AvailablePduRBufferLength
);

STATIC FUNC(Std_ReturnType, LINIF_CODE) LinTp_GetPduInfo
(
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo,
    LinTp_PCIType *PciType,
    uint16 *MsgLength,
    uint8 *PayloadLength
);

STATIC FUNC(Std_ReturnType, LINIF_CODE) LinTp_GetLinTpRxSduId
(
    const uint8 currentChannel,
    const uint8 NAD,
    PduIdType *LinTpRxSduId
);

#if (LINIF_MASTER_SUPPORTED == STD_ON)
STATIC FUNC(LinTp_Response_Type, LINIF_CODE) LinTp_GetResponseType
(
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo,
    const LinTp_PCIType PciType,
    const uint8 SID
);
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

STATIC FUNC(void, LINIF_CODE) LinTp_TimerHandling
(
    uint8 currentChannel
);

STATIC FUNC(void, LINIF_CODE) LinTp_StartReceptionHandling
(
    uint8 currentChannel,
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo
);

STATIC FUNC(void, LINIF_CODE) LinTp_CFHandling
(
    uint8 currentChannel,
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo
);

STATIC FUNC(Std_ReturnType, LINIF_CODE) LinTp_PhysTriggerTransmit
(
    uint8 currentChannel,
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo
);

#if (LINIF_MASTER_SUPPORTED == STD_ON)
STATIC FUNC(Std_ReturnType, LINIF_CODE) LinTp_FuncTriggerTransmit
(
    uint8 currentChannel,
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo
);

STATIC FUNC(void, LINIF_CODE) LinTp_StopFuncTransmission
(
    uint8 currentChannel,
    NotifResultType Result
);
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

STATIC FUNC(LinTp_FrameReturnType, LINIF_CODE) LinTp_AssemblePdu
(
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo,
    CONSTP2CONST(LinTpTxNSduType, AUTOMATIC, LINIF_VAR) pTpTxConfig,
    const uint16 PciAdditionalInfo,
    const LinTp_PCIType PciType,
    const uint8 PayloadLength,
    const uint8 NAD,
    PduLengthType *AvailablePduRBufferLength
);

/*==================[external function declarations]========================*/

/*==================[internal constants]====================================*/

/*==================[external variable definitions]=========================*/

/*==================[external function definitions]=========================*/

/** \brief This function cancel an ongoing reception if
 *         LinTp_CanelReceive was called
 *
 *  \param currentChannel Channel where reception is aborted
 */
FUNC(void, LINIF_CODE) LinTp_CancelReceiveHandling
(
    uint8 currentChannel
)
{
    CONSTP2VAR(LinTp_ChannelInfoType, AUTOMATIC, LINIF_VAR) pTpChannel =
        &LinTp_ChannelInfo[currentChannel];

    uint8 ChannelFlags;

    TS_AtomicAssign8(ChannelFlags, pTpChannel->ChannelFlags);

    /* LinTp_CanelReceive was called, abort ongoing reception */
    if((ChannelFlags & LINTP_FLAG_CANCEL_RECEIVE_REQUESTED) ==
        LINTP_FLAG_CANCEL_RECEIVE_REQUESTED
      )
    {
       /* check if a cancel receive request is still valid,
          maybe the reception was finished */
       if((ChannelFlags & LINTP_FLAG_CANCEL_RECEIVE_ALLOWED) ==
          LINTP_FLAG_CANCEL_RECEIVE_ALLOWED
         )
       {
           LinTp_StopTimerCR(&pTpChannel->PhysChannelInfo);
           LinTp_StopRxNotOk(currentChannel);
       }

       SchM_Enter_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();
       {
           /* clear flag to allow new cancel receive requests */
           TS_AtomicAssign8(ChannelFlags, pTpChannel->ChannelFlags);
           ChannelFlags &= ~(uint8)LINTP_FLAG_CANCEL_RECEIVE_REQUESTED;
           TS_AtomicAssign8(pTpChannel->ChannelFlags, ChannelFlags);
       }
       SchM_Exit_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();
    }

}

#if (LINIF_MASTER_SUPPORTED == STD_ON)
/** \brief This function handles functional transmission requests
 *
 *  \param currentChannel Channel where functional transmission was requested
 */
FUNC(void, LINIF_CODE) LinTp_FuncTransmitHandling
(
    uint8 currentChannel
)
{
    CONSTP2VAR(LinTp_ChannelInfoType, AUTOMATIC, LINIF_VAR) pTpChannel =
        &LinTp_ChannelInfo[currentChannel];

    uint8 ChannelFlags;


    TS_AtomicAssign8(ChannelFlags, pTpChannel->ChannelFlags);

    /* Check if functional transmission was requested */
    if((ChannelFlags & LINTP_FLAG_FUNC_TRANSMIT_REQUESTED) ==
        LINTP_FLAG_FUNC_TRANSMIT_REQUESTED)
    {
        CONSTP2VAR(LinTp_PhysChannelInfoType, AUTOMATIC, LINIF_VAR) pPhysData =
            &pTpChannel->PhysChannelInfo;

        CONSTP2VAR(LinTp_FuncChannelInfoType, AUTOMATIC, LINIF_VAR) pFuncData =
            &pTpChannel->FuncChannelInfo;

        const LinTp_PhysStates_Type PhysState = pPhysData->State;

        /* Verify that LinTp currently is not waiting for a Tx confirmation,
           functional request should not be initiated in this state */
        if(PhysState != LINTP_PHYS_STATE_TXPENDING)
        {

#if (LINTP_SCHEDULE_CHANGE_DIAG_API == STD_ON)
            /* check there is no ongoing BswM schedule change */
            if((ChannelFlags&LINTP_BSWM_SCHEDULE_SWITCH_FALGS_MASK) == 0U)
            {

                if((PhysState == LINTP_PHYS_STATE_IDLE) ||
                   (PhysState == LINTP_PHYS_STATE_WAITRX)
                  )
                {
                    /* request a diagnostic schedule which includes MRF's to trigger
                       a tx transmission in the next LinIf_MainFunction call */
                    if((LINTP_CFG_GET_TXNSDU[pFuncData->SduId].LinTpOptions & LINTP_SCHEDULE_CHANGE_DIAG)
                          == LINTP_SCHEDULE_CHANGE_DIAG)
                    {
                        BswM_LinTp_RequestMode(LINIF_GET_COMM_CHANNEL(currentChannel),
                                               LINTP_DIAG_REQUEST
                                              );
                    }
                }
#endif /* LINTP_SCHEDULE_CHANGE_DIAG_API == STD_ON */

                SchM_Enter_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

                /* Indicate that a functional transmission is ongoing */
                pFuncData->State = LINTP_FUNC_STATE_WAITTX;

                /* clear flag to allow new functional requests */
                TS_AtomicAssign8(ChannelFlags, pTpChannel->ChannelFlags);
                ChannelFlags &= ~(uint8)LINTP_FLAG_FUNC_TRANSMIT_REQUESTED;
                TS_AtomicAssign8(pTpChannel->ChannelFlags, ChannelFlags);

                SchM_Exit_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

#if (LINTP_SCHEDULE_CHANGE_DIAG_API == STD_ON)
            }
#endif /* LINTP_SCHEDULE_CHANGE_DIAG_API == STD_ON */
        }
    }

}

/** \brief This function handles physical transmission requests
 *
 *  \param currentChannel Channel where physical transmission was requested
 */
FUNC(void, LINIF_CODE) LinTp_PhysTransmitHandling
(
    uint8 currentChannel
)
{
    CONSTP2VAR(LinTp_ChannelInfoType, AUTOMATIC, LINIF_VAR) pTpChannel =
        &LinTp_ChannelInfo[currentChannel];

    CONSTP2VAR(LinTp_PhysChannelInfoType, AUTOMATIC, LINIF_VAR) pPhysData =
        &pTpChannel->PhysChannelInfo;

    const LinTp_PhysStates_Type currentState = pPhysData->State;
    const LinTp_FuncStates_Type FuncState = pTpChannel->FuncChannelInfo.State;


    if((FuncState == LINTP_FUNC_STATE_IDLE) && (currentState != LINTP_PHYS_STATE_TXPENDING))
    {

        uint8 ChannelFlags;
        uint8 ChannelClearFlags;
        uint16 requestedMsgLength;
        PduIdType requestedSduId;

#if (LINTP_SCHEDULE_CHANGE_DIAG_API == STD_ON)

        TS_AtomicAssign8(ChannelFlags, pTpChannel->ChannelFlags);

        /* check there is no ongoing BswM schedule change */
        if((ChannelFlags&LINTP_BSWM_SCHEDULE_SWITCH_FALGS_MASK) == 0U)
        {
#endif /* LINTP_SCHEDULE_CHANGE_DIAG_API == STD_ON */

            SchM_Enter_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

            requestedSduId = pPhysData->PendingSduId;
            requestedMsgLength = pPhysData->PendingMsgLength;

            TS_AtomicAssign8(ChannelFlags, pTpChannel->ChannelFlags);
            ChannelClearFlags = ChannelFlags;
            ChannelClearFlags &= (~(uint8)LINTP_FLAG_PHYS_TRANSMIT_REQUESTED);
            TS_AtomicAssign8(pTpChannel->ChannelFlags, ChannelClearFlags);

            SchM_Exit_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

            /* LinTp_Transmit was called, initiate new tx transmission
               if there currently no functional transmit ongoing */
            if((ChannelFlags & LINTP_FLAG_PHYS_TRANSMIT_REQUESTED) ==
               LINTP_FLAG_PHYS_TRANSMIT_REQUESTED)
            {
                switch(currentState)
                {
                    case LINTP_PHYS_STATE_IDLE:
                    {
                        /* stop timer of an ongoing reception of a SF or FF */
                        LinTp_StopTimerP2(pPhysData);

#if (LINTP_SCHEDULE_CHANGE_DIAG_API == STD_ON)
                        {
                            CONSTP2CONST(LinTpTxNSduType, TYPEDEF, LINIF_APPL_CONST) pTpTxConfig =
                                &LINTP_CFG_GET_TXNSDU[requestedSduId];

                            /* request a diagnostic schedule which includes MRF's to trigger
                               a tx transmission in the next LinIf_MainFunction call */
                            if((pTpTxConfig->LinTpOptions & LINTP_SCHEDULE_CHANGE_DIAG)
                                  == LINTP_SCHEDULE_CHANGE_DIAG)
                            {
                                BswM_LinTp_RequestMode(LINIF_GET_COMM_CHANNEL(currentChannel),
                                                       LINTP_DIAG_REQUEST
                                                      );
                            }
                        }
#endif /* LINTP_SCHEDULE_CHANGE_DIAG_API == STD_ON */

                        break;
                    }
                    case LINTP_PHYS_STATE_WAITRX:
                    {
                        const uint16 currentPduHandleId =
                            LINTP_CFG_GET_RXNSDU[pPhysData->SduId].PduRScrPduHandleId;

                        /* stop timer which waits for the next LinTp_RxIndication call*/
                        LinTp_StopTimerCR(pPhysData);

                        /* Inform PduR that ongoing reception is aborted */
                        PduR_LinTpRxIndication(currentPduHandleId, NTFRSLT_E_NOT_OK);

#if (LINTP_SCHEDULE_CHANGE_DIAG_API == STD_ON)
                        {
                            CONSTP2CONST(LinTpTxNSduType, TYPEDEF, LINIF_APPL_CONST) pTpTxConfig =
                                &LINTP_CFG_GET_TXNSDU[requestedSduId];

                            if((pTpTxConfig->LinTpOptions & LINTP_SCHEDULE_CHANGE_DIAG)
                                == LINTP_SCHEDULE_CHANGE_DIAG)
                            {
                                BswM_LinTp_RequestMode(LINIF_GET_COMM_CHANNEL(currentChannel),
                                                       LINTP_DIAG_REQUEST
                                                      );
                            }
                        }
#endif /* LINTP_SCHEDULE_CHANGE_DIAG_API == STD_ON */

                        break;
                    }
                    case LINTP_PHYS_STATE_WAITTX:
                    {
                        const uint16 currentPduHandleId =
                            LINTP_CFG_GET_TXNSDU[pPhysData->SduId].PduRDestPduHandleId;

                        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00803_2,1 */
                        LinTp_StopTimerCS(pPhysData);


                        /* Inform PduR that ongoing transmission is aborted */
                        PduR_LinTpTxConfirmation(currentPduHandleId, NTFRSLT_E_NOT_OK);
                        break;
                    }
                    /* CHECK: NOPARSE */
                    case LINTP_PHYS_STATE_TXPENDING:
                    default:
                    {
                        /*should not happen*/
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
                        LINIF_UNREACHABLE_CODE_ASSERT(LINIF_API_MAINFUNCTION);
#endif /* LINIF_DEV_ERROR_DETECT == STD_ON */
                        break;
                    }
                    /* CHECK: PARSE */

                }
                /* set state for new transmission */
                DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_STATE_GRP(currentChannel,
                                                                (pPhysData->State),
                                                                (LINTP_PHYS_STATE_WAITTX)
                                                               );
                pPhysData->State = LINTP_PHYS_STATE_WAITTX;
                pPhysData->SduId = requestedSduId;
                pPhysData->MsgLengthTotal = requestedMsgLength;
                pPhysData->MsgLengthRemain = requestedMsgLength;
            }
#if (LINTP_SCHEDULE_CHANGE_DIAG_API == STD_ON)
        }
#endif /* LINTP_SCHEDULE_CHANGE_DIAG_API == STD_ON */
    }

}
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

#if(LINIF_SLAVE_SUPPORTED == STD_ON)
/** \brief This function handles physical transmission requests for Slave Nodes
 *
 *  \param currentChannel Channel where physical transmission was requested
 */
FUNC(void, LINIF_CODE) LinTp_SlavePhysTransmitHandling
(
    uint8 currentChannel
)
{
    CONSTP2VAR(LinTp_ChannelInfoType, AUTOMATIC, LINIF_VAR) pTpChannel =
        &LinTp_ChannelInfo[currentChannel];

    CONSTP2VAR(LinTp_PhysChannelInfoType, AUTOMATIC, LINIF_VAR) pPhysData =
        &pTpChannel->PhysChannelInfo;

    const LinTp_PhysStates_Type currentState = pPhysData->State;

    boolean switchedToTx = FALSE;

    uint8 ChannelFlags;
    uint8 ChannelClearFlags;

    SchM_Enter_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

    TS_AtomicAssign8(ChannelFlags, pTpChannel->ChannelFlags);
    ChannelClearFlags = ChannelFlags;

    SchM_Exit_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

    /* LinTp_Transmit was called, initiate new tx transmission
       if there currently no functional reception ongoing */
    if((ChannelFlags & LINTP_FLAG_PHYS_TRANSMIT_REQUESTED) ==
       LINTP_FLAG_PHYS_TRANSMIT_REQUESTED)
    {
        switch(currentState)
        {
            case LINTP_PHYS_STATE_IDLE:
            {
              /* set state for new transmission */
              DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_STATE_GRP(currentChannel,
                                                              (pPhysData->State),
                                                              (LINTP_PHYS_STATE_WAITTX)
                                                             );

              pPhysData->State = LINTP_PHYS_STATE_WAITTX;

              switchedToTx = TRUE;
              break;
            }
            case LINTP_PHYS_STATE_TXPENDING: /* intended fallthrough */
            case LINTP_PHYS_STATE_WAITTX:
            {
                const uint16 currentPduHandleId =
                    LINTP_CFG_GET_TXNSDU[pPhysData->SduId].PduRDestPduHandleId;

                /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00803_2,1 */
                LinTp_StopTimerCS(pPhysData);

                /* Inform PduR that ongoing transmission is aborted */
                PduR_LinTpTxConfirmation(currentPduHandleId, NTFRSLT_E_NOT_OK);

                pPhysData->State = LINTP_PHYS_STATE_WAITTX; /* because of TXPENDING fallthrough */

                switchedToTx = TRUE;
                break;
            }
            case LINTP_PHYS_STATE_WAITRX:
            {
                /* nothing to do */
                break;
            }
            /* CHECK: NOPARSE */
            default:
            {
                /*should not happen*/
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
                LINIF_UNREACHABLE_CODE_ASSERT(LINIF_API_MAINFUNCTION);
#endif /* LINIF_DEV_ERROR_DETECT == STD_ON */
                break;
            }
            /* CHECK: PARSE */
        }
    }

    if(FALSE != switchedToTx)
    {
      uint16 requestedMsgLength;

      SchM_Enter_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

      pPhysData->SduId = pPhysData->PendingSduId;
      requestedMsgLength = pPhysData->PendingMsgLength;
      pPhysData->MsgLengthTotal = requestedMsgLength;
      pPhysData->MsgLengthRemain = requestedMsgLength;

      ChannelClearFlags &= (~(uint8)LINTP_FLAG_PHYS_TRANSMIT_REQUESTED);
      TS_AtomicAssign8(pTpChannel->ChannelFlags, ChannelClearFlags);

      SchM_Exit_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

    }
}
#endif /* LINIF_SLAVE_SUPPORTED == STD_ON */

FUNC(Std_ReturnType, LINIF_CODE) LinTp_TriggerTransmit
(
    uint8 currentChannel,
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo
)
{
    Std_ReturnType RetCode = E_NOT_OK;


    if(LinTp_InitState == LINTP_INIT)
    {
#if (LINIF_MASTER_SUPPORTED == STD_ON)
        const LinTp_FuncStates_Type FuncState =
            LinTp_ChannelInfo[currentChannel].FuncChannelInfo.State;

        if(FuncState == LINTP_FUNC_STATE_IDLE)
        {
            /* handle physical transmission */
            RetCode = LinTp_PhysTriggerTransmit(currentChannel, pPduInfo);
        }
        else if(FuncState == LINTP_FUNC_STATE_WAITTX)
        {
            /* physical transmission was interrupted by functional transmission,
               process the functional request */
            RetCode = LinTp_FuncTriggerTransmit(currentChannel, pPduInfo);
        }
        else
        {
            /* illegal state, do nothing */
        }
#else
        RetCode = LinTp_PhysTriggerTransmit(currentChannel, pPduInfo);
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */
    }


    return RetCode;
}

#if (LINIF_MASTER_SUPPORTED == STD_ON)
FUNC(void, LINIF_CODE) LinTp_BswMScheduleChangePerformed
(
    uint8 currentChannel
)
{

#if (LINTP_SCHEDULE_CHANGE_DIAG_API == STD_ON)

    uint8 ChannelFlags;
    boolean goToApplicative = FALSE;

    TS_AtomicAssign8(ChannelFlags, LinTp_ChannelInfo[currentChannel].ChannelFlags);

    /* Check if a BswM schedule switch ended (that happened because a physical reception ended) */
    if((ChannelFlags&LINTP_FLAG_PHYS_RX_BSWM_SCHEDULE_CHANGE_REQUESTED) == LINTP_FLAG_PHYS_RX_BSWM_SCHEDULE_CHANGE_REQUESTED)
    {
        CONSTP2VAR(LinTp_ChannelInfoType, AUTOMATIC, LINIF_VAR) pTpData =
        &LinTp_ChannelInfo[currentChannel];

        CONSTP2VAR(LinTp_PhysChannelInfoType, AUTOMATIC, LINIF_VAR) pPhysData =
            &pTpData->PhysChannelInfo;

        CONSTP2CONST(LinTpRxNSduType, TYPEDEF, LINIF_APPL_CONST) pTpRxConfig =
            &LINTP_CFG_GET_RXNSDU[pPhysData->SduId];

        const PduIdType PduRScrPduHandleId = pTpRxConfig->PduRScrPduHandleId;

        /* If LINTP_NO_PDUR_RESULT is set , it means this BswM Schedule Change happens because of P2 Timeout.
           This means no call to PduR */
        if(pPhysData->PduRResult != LINTP_NO_PDUR_RESULT)
        {
            /* Inform the PduR that a reception was stopped */
            PduR_LinTpRxIndication(PduRScrPduHandleId, pPhysData->PduRResult);
        }

        DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_NAD_GRP(currentChannel,
                                                      (pPhysData->NAD),
                                                      (LINTP_INVALID_NAD)
                                                     );

        DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_SID_GRP(currentChannel,
                                                      (pPhysData->SID),
                                                      (LINTP_INVALID_SID)
                                                     );

        DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_STATE_GRP(currentChannel,
                                                        (pPhysData->State),
                                                        (LINTP_PHYS_STATE_IDLE)
                                                       );
        SchM_Enter_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

        TS_AtomicAssign8(ChannelFlags, LinTp_ChannelInfo[currentChannel].ChannelFlags);

        ChannelFlags &= ~(uint8)LINTP_FLAG_PHYS_RX_BSWM_SCHEDULE_CHANGE_REQUESTED;

        pPhysData->PduRResult = LINTP_INVALID_PDUR_RESULT;

        pPhysData->NAD = LINTP_INVALID_NAD;

        pPhysData->SID = LINTP_INVALID_SID;

        pPhysData->State = LINTP_PHYS_STATE_IDLE;

        TS_AtomicAssign8(LinTp_ChannelInfo[currentChannel].ChannelFlags, ChannelFlags);

        SchM_Exit_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();
    }
    /* Check if a BswM schedule switch ended (that happened because a functional transmission ended) */
    else if((ChannelFlags&LINTP_FLAG_FUNC_BSWM_SCHEDULE_CHANGE_REQUESTED) == LINTP_FLAG_FUNC_BSWM_SCHEDULE_CHANGE_REQUESTED)
    {

        CONSTP2VAR(LinTp_FuncChannelInfoType, AUTOMATIC, LINIF_VAR) pFuncData =
          &LinTp_ChannelInfo[currentChannel].FuncChannelInfo;

        CONSTP2VAR(LinTp_PhysChannelInfoType, AUTOMATIC, LINIF_VAR) pPhysData =
            &LinTp_ChannelInfo[currentChannel].PhysChannelInfo;

        CONSTP2CONST(LinTpTxNSduType, TYPEDEF, LINIF_APPL_CONST) pTpTxConfig =
            &LINTP_CFG_GET_TXNSDU[pFuncData->SduId];

        const PduIdType PduRDestPduHandleId = pTpTxConfig->PduRDestPduHandleId;

        /* Indicate the PduR that a functional transmission was stopped */
        PduR_LinTpTxConfirmation(PduRDestPduHandleId, pFuncData->PduRResult);

        DBG_LINTP_CHANNELINFO_FUNCCHANNELINFO_STATE_GRP(currentChannel,
                                                      (pFuncData->State),
                                                      (LINTP_FUNC_STATE_IDLE)
                                                     );

        SchM_Enter_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

        TS_AtomicAssign8(ChannelFlags, LinTp_ChannelInfo[currentChannel].ChannelFlags);

        pFuncData->PduRResult = LINTP_INVALID_PDUR_RESULT;

        /* If P2 timeout happened during this schedule table change */
        if((ChannelFlags&LINTP_FLAG_P2_BSWM_SCHEDULE_CHANGE_REQUESTED) == LINTP_FLAG_P2_BSWM_SCHEDULE_CHANGE_REQUESTED)
        {
            /* switch to APPLICATIVE immediately */
            goToApplicative = TRUE;

            ChannelFlags &= ~(uint8)LINTP_FLAG_P2_BSWM_SCHEDULE_CHANGE_REQUESTED;

            ChannelFlags &= ~(uint8)LINTP_FLAG_FUNC_BSWM_SCHEDULE_CHANGE_REQUESTED;

            /* Also set this flag, so all Rx data is cleared after the switch to APPLICATIVE */
            ChannelFlags |= (uint8) LINTP_FLAG_PHYS_RX_BSWM_SCHEDULE_CHANGE_REQUESTED;

            pPhysData->PduRResult = LINTP_NO_PDUR_RESULT;
        }
        else
        {

            ChannelFlags &= ~(uint8)LINTP_FLAG_FUNC_BSWM_SCHEDULE_CHANGE_REQUESTED;

            pFuncData->State = LINTP_FUNC_STATE_IDLE;

        }

        TS_AtomicAssign8(LinTp_ChannelInfo[currentChannel].ChannelFlags, ChannelFlags);

        SchM_Exit_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();
    }
    /* Check if a BswM schedule switch ended (that happened because a physical transmission ended) */
    else if((ChannelFlags&LINTP_FLAG_PHYS_BSWM_SCHEDULE_CHANGE_REQUESTED) == LINTP_FLAG_PHYS_BSWM_SCHEDULE_CHANGE_REQUESTED)
    {
        CONSTP2VAR(LinTp_PhysChannelInfoType, AUTOMATIC, LINIF_VAR) pPhysData =
            &LinTp_ChannelInfo[currentChannel].PhysChannelInfo;

        const PduIdType LinTpSduId = pPhysData->SduId;

        CONSTP2CONST(LinTpTxNSduType, TYPEDEF, LINIF_APPL_CONST) pTpTxConfig =
            &LINTP_CFG_GET_TXNSDU[LinTpSduId];

        const PduIdType PduRDestPduHandleId = pTpTxConfig->PduRDestPduHandleId;

        /* Inform the PduR that a transmission was stopped */
        PduR_LinTpTxConfirmation(PduRDestPduHandleId, pPhysData->PduRResult);

        DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_STATE_GRP(currentChannel,
                                                        (pPhysData->State),
                                                        (LINTP_PHYS_STATE_IDLE)
                                                       );

        SchM_Enter_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

        TS_AtomicAssign8(ChannelFlags, LinTp_ChannelInfo[currentChannel].ChannelFlags);

        pPhysData->PduRResult = LINTP_INVALID_PDUR_RESULT;

        /* If P2 timeout happened during this schedule table change */
        if((ChannelFlags&LINTP_FLAG_P2_BSWM_SCHEDULE_CHANGE_REQUESTED) == LINTP_FLAG_P2_BSWM_SCHEDULE_CHANGE_REQUESTED)
        {
            /* switch to APPLICATIVE immediately */
            goToApplicative = TRUE;

            ChannelFlags &= ~(uint8)LINTP_FLAG_P2_BSWM_SCHEDULE_CHANGE_REQUESTED;

            /* Also set this flag, so all Rx data is cleared after the switch to APPLICATIVE */
            ChannelFlags |= (uint8) LINTP_FLAG_PHYS_RX_BSWM_SCHEDULE_CHANGE_REQUESTED;

            pPhysData->PduRResult = LINTP_NO_PDUR_RESULT;
        }
        else
        {

            pPhysData->State = LINTP_PHYS_STATE_IDLE;

        }

        ChannelFlags &= ~(uint8)LINTP_FLAG_PHYS_BSWM_SCHEDULE_CHANGE_REQUESTED;

        TS_AtomicAssign8(LinTp_ChannelInfo[currentChannel].ChannelFlags, ChannelFlags);

        SchM_Exit_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();
    }
    else
    {
        /* do nothing */
    }

    /* !LINKSTO LinTp.EB.P2Timeout.BswMScheduleChange,2 */
    /* Switch to APPLICATIVE because of P2 timeout interrupting another schedule table */
    if(TRUE == goToApplicative)
    {
        const NetworkHandleType ComMChannelId = LINIF_GET_COMM_CHANNEL(currentChannel);

        /* Switch to applicative schedule */
        BswM_LinTp_RequestMode(ComMChannelId, LINTP_APPLICATIVE_SCHEDULE);
    }

#else

    TS_PARAM_UNUSED(currentChannel);

#endif /* LINTP_SCHEDULE_CHANGE_DIAG_API == STD_ON */

}
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

FUNC(Std_ReturnType, LINIF_CODE) LinTp_TriggerReceive
(
    uint8 currentChannel
)
{
    Std_ReturnType RetCode = E_NOT_OK;


    if(LinTp_InitState == LINTP_INIT)
    {
        CONSTP2VAR(LinTp_PhysChannelInfoType, AUTOMATIC, LINIF_VAR) pPhsyicData =
            &LinTp_ChannelInfo[currentChannel].PhysChannelInfo;

        const LinTp_PhysStates_Type currentState = pPhsyicData->State;

        if(currentState == LINTP_PHYS_STATE_IDLE)
        {
            /* PduR ready to copy data*/
            RetCode = E_OK;
        }
        else if(currentState == LINTP_PHYS_STATE_WAITRX)
        {
            /* before reception of next CF frame verify if enough
               buffer is available to copy message to PduR */
            const PduIdType PduRPduHandleId =
                LINTP_CFG_GET_RXNSDU[pPhsyicData->SduId].PduRScrPduHandleId;
            const uint16 MsgLengthRemain = pPhsyicData->MsgLengthRemain;

            PduLengthType AvailablePduRBufferLength = pPhsyicData->PduRBufferAvailable;
            uint16 NextFramePayload;

            RetCode = E_OK;

            if(MsgLengthRemain < LINTP_PCI_DATA_LENGTH)
            {
                /* next payload of Pdu is the remaining data length */
                NextFramePayload = MsgLengthRemain;
            }
            else
            {
                /* Remaining data length is to big to fit into a single Pdu,
                   next payload is max length which fits into CF */
                NextFramePayload = LINTP_PCI_DATA_LENGTH;
            }

            /* check if available buffer length from the last PduR_LinTpCopyTxData
               is bigger than the payload of the next Pdu */
            if(NextFramePayload > AvailablePduRBufferLength)
            {
                /* get available data length from PduR */
                RetCode = LinTp_GetAvailablePduRRxBufferLength(PduRPduHandleId,
                                                               &AvailablePduRBufferLength);
            }

            if(RetCode != E_OK)
            {
                /* error occurred during PduR call */
                LinTp_StopTimerCR(pPhsyicData);
                LinTp_StopRxNotOk(currentChannel);
            }
            else if(NextFramePayload > AvailablePduRBufferLength)
            {
                /* PduR is not ready to receive data, retry the next SRF */
                RetCode = E_NOT_OK;
            }
            else
            {
                /* enough space available */
            }
        }
        else
        {
            /* LinTp_TriggerReceive called in illegal state */
        }
    }


    return RetCode;
}

#if (LINIF_MASTER_SUPPORTED == STD_ON)
FUNC(void, LINIF_CODE) LinTp_TxRxMainFunction
(
    uint8 currentChannel
)
{

    if(LinTp_InitState == LINTP_INIT)
    {
        LinTp_CancelReceiveHandling(currentChannel);
        LinTp_FuncTransmitHandling(currentChannel);
        LinTp_PhysTransmitHandling(currentChannel);
    }

}
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

FUNC(void, LINIF_CODE) LinTp_TimerMainFunction
(
    uint8 currentChannel
)
{

    if(LinTp_InitState == LINTP_INIT)
    {
        LinTp_TimerHandling(currentChannel);
    }

}


FUNC(void, LINIF_CODE) LinTp_TxConfirmation
(
    uint8 currentChannel,
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo
)
{
    CONSTP2VAR(LinTp_PhysChannelInfoType, AUTOMATIC, LINIF_VAR) pPhysData =
          &LinTp_ChannelInfo[currentChannel].PhysChannelInfo;

#if (LINIF_MASTER_SUPPORTED == STD_ON)
    CONSTP2VAR(LinTp_FuncChannelInfoType, AUTOMATIC, LINIF_VAR) pFuncData =
          &LinTp_ChannelInfo[currentChannel].FuncChannelInfo;

    const LinTp_FuncStates_Type FuncState = pFuncData->State;
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */
    const LinTp_PhysStates_Type PhysState = pPhysData->State;
    const uint16 MsgLengthRemain = pPhysData->MsgLengthRemain;


    /* pPduInfo not used because after MRF, slave does not respond */
    TS_PARAM_UNUSED(pPduInfo);

#if (LINIF_MASTER_SUPPORTED == STD_ON)
    if(FuncState == LINTP_FUNC_STATE_IDLE)
#endif
    {
        if(PhysState == LINTP_PHYS_STATE_TXPENDING)
        {
            /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00800_4,1 */
            LinTp_StopTimerAS(pPhysData);

            if(MsgLengthRemain == 0U)
            {
                /* message sent completely, end the tx transmission*/
              /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00068.Master.SF,1,
                          LinIf.ASR20-11.SWS_LinIf_00068.Master.CF,1,
                          LinIf.ASR20-11.SWS_LinIf_00068.Slave.SF,1,
                          LinIf.ASR20-11.SWS_LinIf_00068.Slave.CF,1
               */
                LinTp_StopTxOk(currentChannel);
            }
            else
            {
                CONSTP2CONST(LinTpTxNSduType, TYPEDEF, LINIF_APPL_CONST) pTxTpConfig =
                    &LINTP_CFG_GET_TXNSDU[pPhysData->SduId];

                /* get ready for the next frame*/
                DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_STATE_GRP(currentChannel,
                                                                (pPhysData->State),
                                                                (LINTP_PHYS_STATE_WAITTX)
                                                               );
                pPhysData->State = LINTP_PHYS_STATE_WAITTX;
                /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00803_1,1 */
                LinTp_StartTimerCS(pTxTpConfig, pPhysData);
            }
        }
    }
#if (LINIF_MASTER_SUPPORTED == STD_ON)
    else if(FuncState == LINTP_FUNC_STATE_TXPENDING)
    {
        /* SF was successfully sent */
        LinTp_StopTimerAS(pFuncData);
        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00068.Master.SF,1 */
        LinTp_StopFuncTransmissionOk(currentChannel);
    }
    else
    {
        /* function called in invalid state, do nothing */
    }
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */
}


FUNC(void, LINIF_CODE) LinTp_RxIndication
(
    uint8 currentChannel,
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo
)
{
    CONSTP2VAR(LinTp_PhysChannelInfoType, AUTOMATIC, LINIF_VAR) pPhysData =
        &LinTp_ChannelInfo[currentChannel].PhysChannelInfo;

    const LinTp_PhysStates_Type currentState = pPhysData->State;

    switch(currentState)
    {
      case LINTP_PHYS_STATE_WAITRX:
      {
        /* CF was received */
#if (LINIF_SLAVE_SUPPORTED == STD_ON)
        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00871,1,
                    LinIf.ASR20-11.SWS_LinIf_00807,1,
                    LinIf.ASR20-11.SWS_LinIf_00798,1
         */
        if(LINIF_FUNCTIONAL_NAD != pPduInfo->SduDataPtr[LINIF_PDU_BYTE_NAD])
#endif
        {
          LinTp_CFHandling(currentChannel, pPduInfo);
        }
        break;
      }
      case LINTP_PHYS_STATE_IDLE:
      {
        /* SF or FF was received */
        LinTp_StartReceptionHandling(currentChannel, pPduInfo);
        break;
      }
      case LINTP_PHYS_STATE_TXPENDING: /* intended fallthrough */
      case LINTP_PHYS_STATE_WAITTX:
      {
#if (LINIF_SLAVE_SUPPORTED == STD_ON)
#if (LINIF_MASTER_SUPPORTED == STD_ON)
        if(currentChannel >= LinIf_ConfigPtr->NumChannelsMaster)
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */
        {
          /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00797_1_1,1,
                      LinIf.ASR20-11.SWS_LinIf_00797_1_2,1,
                      LinIf.ASR20-11.SWS_LinIf_00797_2_1,1,
                      LinIf.ASR20-11.SWS_LinIf_00797_2_2,1,
                      LinIf.ASR20-11.SWS_LinIf_00798,1
           */
          if(LINIF_FUNCTIONAL_NAD != pPduInfo->SduDataPtr[LINIF_PDU_BYTE_NAD])
          {
            LinTp_StopTxTimeoutCS(currentChannel);
            LinTp_StartReceptionHandling(currentChannel, pPduInfo);
          }
        }
#endif /* LINIF_SLAVE_SUPPORTED == STD_ON */
        break;
      }
      /* CHECK: NOPARSE */
      default:
      {
      /*should not happen*/
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
      LINIF_UNREACHABLE_CODE_ASSERT(LINIF_API_RXIND);
#endif /* LINIF_DEV_ERROR_DETECT == STD_ON */
      break;
      /* CHECK: PARSE */
      }
    }

}

FUNC(void, LINIF_CODE) LinTp_Status_Err
(
    uint8 currentChannel,
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo
)
{
    LinTp_Err_Tx(currentChannel, pPduInfo);

#if (LINIF_RUNTIME_ERROR_REPORTING == STD_ON)
    (void)Det_ReportRuntimeError( LINIF_MODULE_ID,
                                  LINIF_INSTANCE_ID,
                                  LINIF_API_MAINFUNCTION,
                                  LINIF_E_RESPONSE
                                );
#endif /* (LINIF_RUNTIME_ERROR_REPORTING == STD_ON) */
}

FUNC(void, LINIF_CODE) LinTp_Err_Tx
(
    uint8 currentChannel,
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo
)
{
    CONSTP2VAR(LinTp_PhysChannelInfoType, AUTOMATIC, LINIF_VAR) pPhysData =
        &LinTp_ChannelInfo[currentChannel].PhysChannelInfo;

#if (LINIF_MASTER_SUPPORTED == STD_ON)
    CONSTP2VAR(LinTp_FuncChannelInfoType, AUTOMATIC, LINIF_VAR) pFuncData =
        &LinTp_ChannelInfo[currentChannel].FuncChannelInfo;
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

    TS_PARAM_UNUSED(pPduInfo);

#if (LINIF_MASTER_SUPPORTED == STD_ON)
    if(pFuncData->State == LINTP_FUNC_STATE_IDLE)
#endif
    {
        LinTp_StopTimerAS(pPhysData);
        LinTp_StopTxNotOk(currentChannel);
    }
#if (LINIF_MASTER_SUPPORTED == STD_ON)
    else
    {
        LinTp_StopTimerAS(pFuncData);
        LinTp_StopFuncTransmissionNotOk(currentChannel);
    }
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */
}

#if (LINIF_SLAVE_SUPPORTED == STD_ON)
FUNC(void, LINIF_CODE) LinTp_Err_Rx
(
    uint8 currentChannel
)
{
    CONSTP2VAR(LinTp_PhysChannelInfoType, AUTOMATIC, LINIF_VAR) pPhysData =
        &LinTp_ChannelInfo[currentChannel].PhysChannelInfo;

    if(LINTP_PHYS_STATE_WAITRX == pPhysData->State)
    {
      LinTp_StopTimerCR(pPhysData);
      LinTp_StopRxNotOk(currentChannel);
    }
}
#endif /* LINIF_SLAVE_SUPPORTED == STD_ON */
/*==================[internal function definitions]=========================*/


/** \brief This function handles physical Tx transmissions.
 *         It copies data from PduR which should be sent over the bus.
 *
 *  \param currentChannel  Channel which is processed
 *  \param pPduInfo        Data which is copied from the PduR
 */
STATIC FUNC(Std_ReturnType, LINIF_CODE) LinTp_PhysTriggerTransmit
(
    uint8 currentChannel,
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo
)
{
    Std_ReturnType RetCode = E_NOT_OK;

    CONSTP2VAR(LinTp_PhysChannelInfoType, AUTOMATIC, LINIF_VAR) pPhysData =
          &LinTp_ChannelInfo[currentChannel].PhysChannelInfo;

    CONSTP2CONST(LinTpTxNSduType, AUTOMATIC, LINIF_VAR) pTpTxConfig =
          &LINTP_CFG_GET_TXNSDU[pPhysData->SduId];

    const LinTp_PhysStates_Type currentState = pPhysData->State;


    if(currentState == LINTP_PHYS_STATE_WAITTX)
    {
        LinTp_FrameReturnType GetPduRetCode = LINTP_E_NOT_OK;
        const uint16 MsgLength = pPhysData->MsgLengthTotal;
        const uint16 MsgLengthRemain = pPhysData->MsgLengthRemain;
        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00422_1,1 */
#if (LINIF_SLAVE_SUPPORTED == STD_OFF)
          const uint8 NAD = pTpTxConfig->LinTpTxNSduNad;
#else
#if(LINIF_MASTER_SUPPORTED == STD_ON)
          const uint8 NAD = (currentChannel < LinIf_ConfigPtr->NumChannelsMaster) ? pTpTxConfig->LinTpTxNSduNad : LinTp_GetConfNad(currentChannel);
#else
          const uint8 NAD = LinTp_GetConfNad(currentChannel);
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */
#endif /* LINIF_SLAVE_SUPPORTED == STD_OFF */


        PduLengthType AvailablePduRBufferLength = 0;
        uint16 PciAdditionalInfo = MsgLength;
        uint8 FrameCounter = 0;
        LinTp_PCIType PciType;
        uint8 PayloadLength;

        /* Get PCI type and payload length of the next frame */
        LinTp_GetNextFrame(MsgLength, MsgLengthRemain, &PciType, &PayloadLength);

        if(PciType == LINTP_PCI_CF)
        {
            /* continue transmission, set values from last PDU */
            AvailablePduRBufferLength = pPhysData->PduRBufferAvailable;
            FrameCounter = pPhysData->FrameCounter;
            PciAdditionalInfo = FrameCounter;
        }

        /* Get data from the PduR */
        GetPduRetCode = LinTp_AssemblePdu(pPduInfo, pTpTxConfig, PciAdditionalInfo, PciType,
                                          PayloadLength, NAD, &AvailablePduRBufferLength
                                         );

        switch(GetPduRetCode)
        {
            case LINTP_OK:
                /* copying successful, save important information */

                if(PciType == LINTP_PCI_SF)
                {
                    /* save SID, needed to verify RSID during reception */
                    DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_SID_GRP(currentChannel,
                                                                  (pPhysData->SID),
                                                                  (pPduInfo->SduDataPtr[LINIF_PDU_BYTE_SID])
                                                                 );

                    pPhysData->SID = pPduInfo->SduDataPtr[LINIF_PDU_BYTE_SID];

                    DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_NAD_GRP(currentChannel,
                                                                  (pPhysData->NAD),
                                                                  (pTpTxConfig->LinTpTxNSduNad)
                                                                 );

                    pPhysData->NAD = NAD;
                }
                else
                {
                    if(PciType == LINTP_PCI_FF)
                    {

                        DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_SID_GRP(currentChannel,
                                                                      (pPhysData->SID),
                                                                      (pPduInfo->SduDataPtr[LINIF_PDU_BYTE_SID_FF])
                                                                     );

                        /* save SID, needed to verify RSID during reception */
                        pPhysData->SID =
                            pPduInfo->SduDataPtr[LINIF_PDU_BYTE_SID_FF];

                        DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_NAD_GRP(currentChannel,
                                                                      (pPhysData->NAD),
                                                                      (pTpTxConfig->LinTpTxNSduNad)
                                                                     );

                        pPhysData->NAD = NAD;
                    }
#if (LINIF_SLAVE_SUPPORTED == STD_ON)
                    else
                    {
                      if(currentChannel >= LinIf_ConfigPtr->NumChannelsMaster)
                      {
                        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00800_3,1,
                                    LinIf.ASR20-11.SWS_LinIf_00803_2,1
                         */
                        LinTp_StopTimerCS(pPhysData);
                        LinTp_StartTimerAS(pTpTxConfig, pPhysData);
                      }
                    }
#endif /* LINIF_SLAVE_SUPPORTED == STD_ON */

                    /* increment the frame counter for next PDU
                      (SF or FF is treated like PDU with frame count 0) */
                    FrameCounter++;

                    if(FrameCounter > LINTP_CF_MAX_SEQUENCE_NUMBER)
                    {
                        FrameCounter = 0U;
                    }

                    pPhysData->FrameCounter = FrameCounter;
                    pPhysData->PduRBufferAvailable = AvailablePduRBufferLength;
                }

                pPhysData->MsgLengthRemain -= PayloadLength;

                DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_STATE_GRP(currentChannel,
                                                                (pPhysData->State),
                                                                (LINTP_PHYS_STATE_TXPENDING)
                                                               );

                pPhysData->State = LINTP_PHYS_STATE_TXPENDING;

#if (LINIF_SLAVE_SUPPORTED == STD_ON)
                if(currentChannel < LinIf_ConfigPtr->NumChannelsMaster)
#endif /* LINIF_SLAVE_SUPPORTED == STD_ON */
                {
                  LinTp_StopTimerCS(pPhysData);
                  LinTp_StartTimerAS(pTpTxConfig, pPhysData);
                }
                RetCode = E_OK;
                break;
            case LINTP_E_PDUR:
                /* an error occurred during copying, abort transmission */
                /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00803_2,1 */
                LinTp_StopTimerCS(pPhysData);
                /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00073.Slave,1,
                            LinIf.ASR20-11.SWS_LinIf_00073.Master,1
                 */
                LinTp_StopTxNotOk(currentChannel);
                break;
            case LINTP_E_NOT_OK:
                /*  PduR can not provide enough data, do nothing */
                break;
            /* CHECK: NOPARSE */
            default:
                /*should not happen*/
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
                LINIF_UNREACHABLE_CODE_ASSERT(LINIF_API_MAINFUNCTION);
#endif /* LINIF_DEV_ERROR_DETECT == STD_ON */
                break;
            /* CHECK: PARSE */
        }
    }



    return RetCode;
}

#if (LINIF_MASTER_SUPPORTED == STD_ON)
/** \brief This function handles functional Tx transmissions.
 *         It copies data from PduR which should be sent over the bus.
 *
 *  \param currentChannel  Channel which is processed
 *  \param pPduInfo        Data which is copied from the PduR
 */
STATIC FUNC(Std_ReturnType, LINIF_CODE) LinTp_FuncTriggerTransmit
(
    uint8 currentChannel,
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo
)
{
    Std_ReturnType RetCode = E_NOT_OK;
    LinTp_FrameReturnType GetPduRetCode = LINTP_E_NOT_OK;

    CONSTP2VAR(LinTp_FuncChannelInfoType, AUTOMATIC, LINIF_VAR) pFuncData =
          &LinTp_ChannelInfo[currentChannel].FuncChannelInfo;

    CONSTP2CONST(LinTpTxNSduType, AUTOMATIC, LINIF_VAR) pTpTxConfig =
          &LINTP_CFG_GET_TXNSDU[pFuncData->SduId];

    const uint16 MsgLength = pFuncData->MsgLengthTotal;
    PduLengthType AvailablePduRBufferLength = 0U;
    LinTp_PCIType PciType = LINTP_PCI_SF;
    uint8 PayloadLength = (uint8) MsgLength;
    uint16 PciAdditionalInfo = MsgLength;


    /* Get data from the PduR */
    GetPduRetCode = LinTp_AssemblePdu(pPduInfo, pTpTxConfig, PciAdditionalInfo, PciType,
                                      PayloadLength, LINIF_FUNCTIONAL_NAD, &AvailablePduRBufferLength
                                     );

    switch(GetPduRetCode)
    {
        case LINTP_OK:
            /* copying successful */
            DBG_LINTP_CHANNELINFO_FUNCCHANNELINFO_STATE_GRP(currentChannel,
                                                            (pFuncData->State),
                                                            (LINTP_FUNC_STATE_TXPENDING)
                                                           );
            pFuncData->State = LINTP_FUNC_STATE_TXPENDING;
            LinTp_StartTimerAS(pTpTxConfig, pFuncData);
            RetCode = E_OK;
            break;
        case LINTP_E_PDUR:
            /* an error occurred during copying, abort transmission */
            LinTp_StopFuncTransmissionNotOk(currentChannel);
            break;
        case LINTP_E_NOT_OK:
            /* PduR can not provide enough data, do nothing */
            break;
        /* CHECK: NOPARSE */
        default:
            /*should not happen*/
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
            LINIF_UNREACHABLE_CODE_ASSERT(LINIF_API_MAINFUNCTION);
#endif /* LINIF_DEV_ERROR_DETECT == STD_ON */
            break;
        /* CHECK: PARSE */
    }

    return RetCode;
}
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

/** \brief This function builds the header of Pdu and copies data from
 *         the PduR if it can provide data which fits into the next Pdu.
 *
 *  \param pPduInfo                   Data which is copied from the PduR
 *  \param pTpTxConfig                Pointer to LinTp configuration
 *  \param PciAdditionalInfo          Includes the whole message length in case
 *                                    of SF and FF or Sequence number of CF
 *  \param PciType                    PciType of the next Pdu
 *  \param PayloadLength              Payload length of the next Pdu
 *  \param AvailablePduRBufferLength  returns available data length
 *  \param NAD                        NAD of the PDU which is sent next
 *
 *  \return Result of operation
 *  \retval LINTP_OK                  Copying successful
 *  \retval LINTP_E_NOT_OK            PduR can not provide enough data
 *  \retval LINTP_E_PDUR              Error occurred during PduR call
 */
STATIC FUNC(LinTp_FrameReturnType, LINIF_CODE) LinTp_AssemblePdu
(
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo,
    CONSTP2CONST(LinTpTxNSduType, AUTOMATIC, LINIF_VAR) pTpTxConfig,
    const uint16 PciAdditionalInfo,
    const LinTp_PCIType PciType,
    const uint8 PayloadLength,
    const uint8 NAD,
    PduLengthType *AvailablePduRBufferLength
)
{
    Std_ReturnType PduRRetCode = E_OK;
    LinTp_FrameReturnType FunctionRetCode = LINTP_E_NOT_OK;
    const PduIdType PduRDestPduHandleId = pTpTxConfig->PduRDestPduHandleId;


    /* check if available buffer length from the last PduR_LinTpCopyTxData
       is bigger than the payload of the next pdu */
    if(*AvailablePduRBufferLength < PayloadLength)
    {
        /* get current available buffer length from the PduR */
        PduRRetCode = LinTp_GetAvailablePduRTxBufferLength(PduRDestPduHandleId,
                                                           AvailablePduRBufferLength
                                                          );
    }

    if(PduRRetCode == E_OK)
    {
        /* PduR can provide enough data, copy message from PduR */
        if(*AvailablePduRBufferLength >= PayloadLength)
        {
            /* determine PCI type, message length (SF, FF), frame counter (CF)
               and fill information into the next PDU */
            LinTp_BuildHeader(pPduInfo, PciType, NAD, PciAdditionalInfo);

            /* copy payload from PduR to PDU */
            PduRRetCode = LinTp_CopyTxPayloadPduR(pPduInfo,
                                                  PciType,
                                                  PayloadLength,
                                                  PduRDestPduHandleId,
                                                  AvailablePduRBufferLength
                                                 );

            /* message successfully copied wait for status*/
            if(PduRRetCode == E_OK)
            {
                FunctionRetCode = LINTP_OK;
            }
            else
            {
                /* error occurred during copying from PduR*/
                FunctionRetCode = LINTP_E_PDUR;
            }
        }
        else
        {
            /* PduR cannot provide requested length of data */
            FunctionRetCode = LINTP_E_NOT_OK;
        }
    }
    else
    {
        /* error occurred during call of PduR for available data length*/
        FunctionRetCode = LINTP_E_PDUR;
    }

    return FunctionRetCode;
}

#if (LINIF_MASTER_SUPPORTED == STD_ON)
/** \brief This function ends a functional TX transmission if the the
 *         SF was sent successfully or aborts the functional request.
 *
 *  \param Result         Indicates why the transmission should be finished
 *  \param currentChannel Channel where the transmission is closed
 */
STATIC FUNC(void, LINIF_CODE) LinTp_StopFuncTransmission
(
    uint8 currentChannel,
    NotifResultType Result
)
{
    CONSTP2VAR(LinTp_FuncChannelInfoType, AUTOMATIC, LINIF_VAR) pFuncData =
        &LinTp_ChannelInfo[currentChannel].FuncChannelInfo;

    CONSTP2CONST(LinTpTxNSduType, TYPEDEF, LINIF_APPL_CONST) pTpTxConfig =
        &LINTP_CFG_GET_TXNSDU[pFuncData->SduId];

#if (LINTP_SCHEDULE_CHANGE_DIAG_API == STD_ON)

    if((pTpTxConfig->LinTpOptions & LINTP_SCHEDULE_CHANGE_DIAG) ==
        LINTP_SCHEDULE_CHANGE_DIAG
      )
    {
        CONSTP2VAR(LinTp_PhysChannelInfoType, AUTOMATIC, LINIF_VAR) pPhysData =
            &LinTp_ChannelInfo[currentChannel].PhysChannelInfo;

        const LinTp_PhysStates_Type PhysState = pPhysData->State;
        const NetworkHandleType ComMChannelId = LINIF_GET_COMM_CHANNEL(currentChannel);

        uint8 ChannelFlags;

        /* Request schedule which was active before the functional request */

        if(PhysState == LINTP_PHYS_STATE_IDLE)
        {
            const uint16 TimerCount =  pPhysData->P2Timer;

            SchM_Enter_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

            TS_AtomicAssign8(ChannelFlags, LinTp_ChannelInfo[currentChannel].ChannelFlags);

            /* Set a flag to prevent interruption while we make the schedule switch */
            ChannelFlags |= LINTP_FLAG_FUNC_BSWM_SCHEDULE_CHANGE_REQUESTED;

            TS_AtomicAssign8(LinTp_ChannelInfo[currentChannel].ChannelFlags, ChannelFlags);

            pFuncData->PduRResult = Result;

            SchM_Exit_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

            if(TimerCount > 0)
            {
                /* If LinTp is waiting for a response, P2 timer running
                   change to response schedule */
                BswM_LinTp_RequestMode(ComMChannelId, LINTP_DIAG_RESPONSE);
            }
            else
            {
                /* There is no ongoing transmission, switch to applicative schedule */
                BswM_LinTp_RequestMode(ComMChannelId, LINTP_APPLICATIVE_SCHEDULE);
            }
        }
        else if(PhysState == LINTP_PHYS_STATE_WAITRX)
        {
            SchM_Enter_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

            TS_AtomicAssign8(ChannelFlags, LinTp_ChannelInfo[currentChannel].ChannelFlags);

            /* Set a flag to prevent interruption while we make the schedule switch */
            ChannelFlags |= LINTP_FLAG_FUNC_BSWM_SCHEDULE_CHANGE_REQUESTED;

            TS_AtomicAssign8(LinTp_ChannelInfo[currentChannel].ChannelFlags, ChannelFlags);

            pFuncData->PduRResult = Result;

            SchM_Exit_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

            BswM_LinTp_RequestMode(ComMChannelId, LINTP_DIAG_RESPONSE);
        }
        else
        {

            const PduIdType PduRDestPduHandleId = pTpTxConfig->PduRDestPduHandleId;
            /* keep active schedule */
            /* Indicate the PduR that a functional transmission was stopped */
            PduR_LinTpTxConfirmation(PduRDestPduHandleId, Result);

            DBG_LINTP_CHANNELINFO_FUNCCHANNELINFO_STATE_GRP(currentChannel,
                                                            (pFuncData->State),
                                                            (LINTP_FUNC_STATE_IDLE)
                                                           );
            pFuncData->State = LINTP_FUNC_STATE_IDLE;
        }
    }
    else
    {

        const PduIdType PduRDestPduHandleId = pTpTxConfig->PduRDestPduHandleId;
        /* keep active schedule */
        /* Indicate the PduR that a functional transmission was stopped */
        PduR_LinTpTxConfirmation(PduRDestPduHandleId, Result);

        DBG_LINTP_CHANNELINFO_FUNCCHANNELINFO_STATE_GRP(currentChannel,
                                                        (pFuncData->State),
                                                        (LINTP_FUNC_STATE_IDLE)
                                                       );
        pFuncData->State = LINTP_FUNC_STATE_IDLE;
    }
#else

    const PduIdType PduRDestPduHandleId = pTpTxConfig->PduRDestPduHandleId;

    /* Indicate the PduR that a functional transmission was stopped */
    PduR_LinTpTxConfirmation(PduRDestPduHandleId, Result);

    DBG_LINTP_CHANNELINFO_FUNCCHANNELINFO_STATE_GRP(currentChannel,
                                                    (pFuncData->State),
                                                    (LINTP_FUNC_STATE_IDLE)
                                                   );
    pFuncData->State = LINTP_FUNC_STATE_IDLE;

#endif /* LINTP_SCHEDULE_CHANGE_DIAG_API == STD_ON */

}
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

/** \brief This function handles the reception of a SF or FF.
 *
 *  \param currentChannel  Channel which is processed
 *  \param pPduInfo        data which was received from the bus
 */
STATIC FUNC(void, LINIF_CODE) LinTp_StartReceptionHandling
(
    uint8 currentChannel,
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo
)
{
    Std_ReturnType retVal;

    CONSTP2VAR(LinTp_ChannelInfoType, AUTOMATIC, LINIF_VAR) pTpData =
        &LinTp_ChannelInfo[currentChannel];

    CONSTP2VAR(LinTp_PhysChannelInfoType, AUTOMATIC, LINIF_VAR) pPhysData =
        &pTpData->PhysChannelInfo;

#if (LINIF_MASTER_SUPPORTED == STD_ON)
    CONSTP2CONST(LinTp_ConfigType, TYPEDEF, LINIF_APPL_CONST) pTpConfig =
        LinTp_ConfigPtr;
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

    LinTp_PCIType PciType = LINTP_PCI_INVALID;
    uint16 MsgLength;
    uint8 PayloadLength = 0U;
    PduIdType LinTpRxSduId = LINTP_INVALID_SDU_ID;

    /* determine Pci type, message length and payload of received frame */
    retVal = LinTp_GetPduInfo(pPduInfo, &PciType, &MsgLength, &PayloadLength);

    if(retVal == E_OK)
    {
        const uint8 receivedNAD = pPduInfo->SduDataPtr[LINIF_PDU_BYTE_NAD];
#if (LINIF_SLAVE_SUPPORTED == STD_OFF)
        const uint8 expectedNAD = pPhysData->NAD;

        if(receivedNAD == expectedNAD)
#else
        const uint8 expectedNAD = (currentChannel < LinIf_ConfigPtr->NumChannelsMaster) ? pPhysData->NAD : LinTp_GetConfNad(currentChannel);

        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00797_2_1,1,
                    LinIf.ASR20-11.SWS_LinIf_00797_2_2,1,
                    LinIf.ASR20-11.SWS_LinIf_00774,1
         */
        if(((receivedNAD == expectedNAD) || (receivedNAD == LINIF_FUNCTIONAL_NAD)) || (receivedNAD == LINIF_BROADCAST_NAD))
#endif /* LINIF_SLAVE_SUPPORTED == STD_OFF */
        {
            /* determine LinTpRxSduId through received NAD */
            /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00076.SRF,1 */
            retVal = LinTp_GetLinTpRxSduId(currentChannel, receivedNAD, &LinTpRxSduId);

            pPhysData->SduId = LinTpRxSduId;
        }
        else
        {
            retVal = E_NOT_OK;
        }
    }

    if(retVal == E_OK)
    {
        CONSTP2CONST(LinTpRxNSduType, TYPEDEF, LINIF_APPL_CONST) pTpRxConfig =
            &LINTP_CFG_GET_RXNSDU[LinTpRxSduId];

#if (LINIF_MASTER_SUPPORTED == STD_ON)
#if (LINIF_SLAVE_SUPPORTED == STD_OFF)
        const LinTp_Response_Type responseType =
          LinTp_GetResponseType(pPduInfo, PciType, pPhysData->SID);
#else
        const LinTp_Response_Type responseType = (currentChannel < LinIf_ConfigPtr->NumChannelsMaster) ?
                  LinTp_GetResponseType(pPduInfo, PciType, pPhysData->SID) : LINTP_NORMAL_RESPONSE;
#endif /* LINIF_SLAVE_SUPPORT == STD_OFF */
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

        BufReq_ReturnType BuffRetVal;
        PduLengthType AvailablePduRBufferLength = 0U;
        const PduIdType PduRSourcePduHandleId = pTpRxConfig->PduRScrPduHandleId;

#if (LINIF_MASTER_SUPPORTED == STD_ON)
        LinTp_StopTimerP2(pPhysData);
#endif

        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00075.Master.SF,1,
                    LinIf.ASR20-11.SWS_LinIf_00075.Master.FF,1,
                    LinIf.ASR20-11.SWS_LinIf_00075.Slave.SF,1,
                    LinIf.ASR20-11.SWS_LinIf_00075.Slave.FF,1
         */
        BuffRetVal = PduR_LinTpStartOfReception(PduRSourcePduHandleId,
                                                (PduLengthType)MsgLength,
                                                &AvailablePduRBufferLength
                                               );

        if((BuffRetVal == BUFREQ_OK) || (BuffRetVal == BUFREQ_E_BUSY))
        {
             SchM_Enter_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();
             {
                   uint8 ChannelFlags;

                   /* reception started, LinTp_CancelReceive calls are allowed */
                   TS_AtomicAssign8(ChannelFlags, pTpData->ChannelFlags);
                   ChannelFlags |= LINTP_FLAG_CANCEL_RECEIVE_ALLOWED;
                   TS_AtomicAssign8(pTpData->ChannelFlags, ChannelFlags);
             }
             SchM_Exit_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

             if(AvailablePduRBufferLength >= PayloadLength)
             {
                 /* copy payload to PduR if buffer can provide enough space */
                 /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00674.MRF.FF,1,
                             LinIf.ASR20-11.SWS_LinIf_00674.MRF.SF,1,
                             LinIf.ASR20-11.SWS_LinIf_00674.SRF.SF,1,
                             LinIf.ASR20-11.SWS_LinIf_00674.SRF.FF,1
                  */
                 retVal = LinTp_CopyRxPayloadPduR(pPduInfo,
                                                  PciType,
                                                  PayloadLength,
                                                  PduRSourcePduHandleId,
                                                  &AvailablePduRBufferLength
                                                 );

                 if(retVal == E_OK)
                 {
#if (LINIF_MASTER_SUPPORTED == STD_ON)
                     if(responseType == LINTP_PENDING_RESPONSE)
                     {
                         uint16 NumberOfRespPendingFrames =
                             pPhysData->NumberOfRespPendingFrames;

                         NumberOfRespPendingFrames--;

                         if(NumberOfRespPendingFrames == 0U)
                         {
                             /* number of respond pending frames
                                reached abort transmission */
                             LinTp_StopRxNotOk(currentChannel);
                         }
                         else
                         {
                             /* start P2 timer and wait for next Pdu */
                             LinTp_StartTimerP2MAX(pTpConfig, pPhysData);
                             PduR_LinTpRxIndication(PduRSourcePduHandleId, NTFRSLT_OK);
                         }

                         pPhysData->NumberOfRespPendingFrames =
                             NumberOfRespPendingFrames;
                     }
                     else
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */
                     {
                         if(PciType == LINTP_PCI_SF)
                         {
                             /* message received completely, end the rx transmission*/
                             /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00078.MRF.SF,1 */
                             LinTp_StopRxOk(currentChannel);
                         }
                         else /* get ready for next frame */
                         {
                             pPhysData->MsgLengthTotal = MsgLength;
                             pPhysData->PduRBufferAvailable = AvailablePduRBufferLength;
                             pPhysData->MsgLengthRemain = MsgLength - PayloadLength;
                             pPhysData->FrameCounter = 0;

                             DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_STATE_GRP(currentChannel,
                                                                             (pPhysData->State),
                                                                             (LINTP_PHYS_STATE_WAITRX)
                                                                            );
                             /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00323.MRF,1,
                                         LinIf.ASR20-11.SWS_LinIf_00323.SRF,1
                              */
                             pPhysData->State = LINTP_PHYS_STATE_WAITRX;

                             /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00805.N_Cr.Start.FF,1 */
                             LinTp_StartTimerCR(pTpRxConfig, pPhysData);
                         }
                     }
                 }
                 else
                 {
                     /* error occurred during copying of data */
                     /* !LINKSTO LinTp.ASR20-11.CopyRxError.Slave,1 */
                     LinTp_StopRxNotOk(currentChannel);
                 }
             }
             else
             {
                 /* PduR cannot provide enough data */
                 LinTp_StopRxNotOk(currentChannel);
             }
        }
        else
        {
           /* error occurred during start of reception */
           /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00676.MRF.E_NOT_OK,1,
                       LinIf.ASR20-11.SWS_LinIf_00676.MRF.E_OVFL,1
            */
#if (LINIF_MASTER_SUPPORTED == STD_ON)
           LinTp_CancelResponse(currentChannel, pTpRxConfig->LinTpOptions);
#else
           LinTp_CancelResponse(currentChannel);
#endif /*  */
        }
    }

}


/** \brief This function handles the reception of a CF.
 *
 *  \param currentChannel  Channel which is processed
 *  \param pPduInfo        data which was received from the bus
 *
 */
STATIC FUNC(void, LINIF_CODE) LinTp_CFHandling
(
    uint8 currentChannel,
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo
)
{
    Std_ReturnType retVal;
    CONSTP2VAR(LinTp_PhysChannelInfoType, AUTOMATIC, LINIF_VAR) pPhysData =
        &LinTp_ChannelInfo[currentChannel].PhysChannelInfo;

    CONSTP2CONST(LinTpRxNSduType, TYPEDEF, LINIF_APPL_CONST) pTpRxConfig =
        &LINTP_CFG_GET_RXNSDU[pPhysData->SduId];

    const PduIdType PduRSourcePduHandleId = pTpRxConfig->PduRScrPduHandleId;
    LinTp_FrameReturnType retPciVal;
    const LinTp_PCIType PciType = LINTP_PCI_CF;

    uint8 FrameCounter = pPhysData->FrameCounter + 1U;

    if(FrameCounter > LINTP_CF_MAX_SEQUENCE_NUMBER)
    {
        FrameCounter = 0U;
    }

    /* verify that a CF with correct frame count was received */
    retPciVal = LinTp_VerifyCF(pPduInfo, FrameCounter);

    switch(retPciVal)
    {
        case LINTP_OK:
          {
              const uint8 currentNAD = pPduInfo->SduDataPtr[LINIF_PDU_BYTE_NAD];
#if (LINIF_SLAVE_SUPPORTED == STD_OFF)
              const uint8 lastNAD = pPhysData->NAD;
#else
              const uint8 lastNAD = (currentChannel < LinIf_ConfigPtr->NumChannelsMaster) ? pPhysData->NAD : LinTp_GetConfNad(currentChannel);
#endif /* LINIF_SLAVE_SUPPORTED == STD_OFF */

              /* check if NAD equals NAD from last frame */
              if(currentNAD == lastNAD)
              {
                  uint16 MsgLengthRemain = pPhysData->MsgLengthRemain;
                  PduLengthType AvailablePduRBufferLength;
                  uint8 PayloadLength;

#if(LINIF_SLAVE_SUPPORTED == STD_ON)
                  if(currentChannel < LinIf_ConfigPtr->NumChannelsMaster)
#endif
                  {
                    LinTp_StopTimerCR(pPhysData);
                  }

                  /* determine the length of the payload */
                  if(MsgLengthRemain < LINTP_PCI_DATA_LENGTH)
                  {
                      PayloadLength = (uint8) MsgLengthRemain;
                  }
                  else
                  {
                      PayloadLength = LINTP_PCI_DATA_LENGTH;
                  }

                  /* copy received payload to PduR */
                  /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00674.MRF.CF,1,
                              LinIf.ASR20-11.SWS_LinIf_00674.MRF.lastCF,1,
                              LinIf.ASR20-11.SWS_LinIf_00674.SRF.CF,1,
                              LinIf.ASR20-11.SWS_LinIf_00674.SRF.lastCF,1
                   */
                  retVal = LinTp_CopyRxPayloadPduR(pPduInfo,
                                                   PciType,
                                                   PayloadLength,
                                                   PduRSourcePduHandleId,
                                                   &AvailablePduRBufferLength
                                                  );

                  if(retVal == E_OK)
                  {
                      MsgLengthRemain -= PayloadLength;

                      if(MsgLengthRemain == 0U)
                      {
                          /* message received completely, end the rx transmission*/
                          /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00078.MRF.lastCF,1,
                                      LinIf.ASR20-11.SWS_LinIf_00078.SRF.lastCF,1
                           */
#if(LINIF_SLAVE_SUPPORTED == STD_ON)
                          if(currentChannel < LinIf_ConfigPtr->NumChannelsMaster)
                          {
                            /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00805.N_Cr.Stop,1 */
                            LinTp_StopTimerCR(pPhysData);
                          }
#endif /* LINIF_SLAVE_SUPPORTED == STD_ON */

                          LinTp_StopRxOk(currentChannel);
                      }
                      else
                      {
                         /* get ready for the next frame*/
                          pPhysData->PduRBufferAvailable = AvailablePduRBufferLength;
                          pPhysData->MsgLengthRemain = MsgLengthRemain;
                          pPhysData->FrameCounter = FrameCounter;

                          DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_STATE_GRP(currentChannel,
                                                                          (pPhysData->State),
                                                                          (LINTP_PHYS_STATE_WAITRX)
                                                                         );
                          /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00323.MRF,1,
                                      LinIf.ASR20-11.SWS_LinIf_00323.SRF,1
                           */
                          pPhysData->State = LINTP_PHYS_STATE_WAITRX;

                          /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00805.N_Cr.Start.CF,1 */
                          LinTp_StartTimerCR(pTpRxConfig, pPhysData);
                      }
                  }
                  else
                  {
                      /* error occurred during copying of data,
                         abort reception */
                      /* !LINKSTO LinTp.ASR20-11.CopyRxError.Slave,1 */
                      LinTp_StopRxNotOk(currentChannel);
                  }
              }
#if (LINIF_MASTER_SUPPORTED == STD_ON)
              else
              {
                  /* wrong NAD received, abort transmission only
                     if LinTpDropNotRequestedNad is false*/
                  if((pTpRxConfig->LinTpOptions & LINTP_DROP_NOT_REQ_NAD) == 0U)
                  {
                      LinTp_StopTimerCR(pPhysData);

                      LinTp_StopRxUnexpPdu(currentChannel);
                  }
                  else
                  {
                     /* do nothing */
                  }
              }
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */
          }
          break;

        case LINTP_E_SN:
          /* CF with invalid Frame counter, inform PduR */
          LinTp_StopTimerCR(pPhysData);

          /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00795_1,1 */
          LinTp_StopRxWrongSn(currentChannel);
          break;

        case LINTP_NEW_RECEPTION:
          /* SF, FF instead of CF was received, inform Pdu,
             initiate new reception, continue reception in idle state */
          /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00808_1_1,1,
                      LinIf.ASR20-11.SWS_LinIf_00808_1_2,1,
                      LinIf.ASR20-11.SWS_LinIf_00808_2_1,1,
                      LinIf.ASR20-11.SWS_LinIf_00808_2_2,1,
                      LinIf.ASR20-11.SWS_LinIf_00774,1
           */
          LinTp_StopTimerCR(pPhysData);
          PduR_LinTpRxIndication(PduRSourcePduHandleId, NTFRSLT_E_UNEXP_PDU);
          LinTp_StartReceptionHandling(currentChannel, pPduInfo);
          break;
        case LINTP_E_NOT_OK:
          /* unexpected PDU was received, do nothing */
          break;
        /* CHECK: NOPARSE */
        default:
          /*should not happen*/
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
          LINIF_UNREACHABLE_CODE_ASSERT(LINIF_API_MAINFUNCTION);
#endif /* LINIF_DEV_ERROR_DETECT == STD_ON */
          break;
        /* CHECK: PARSE */
    }

}

#if (LINIF_MASTER_SUPPORTED == STD_ON)
/** \brief This function handles negative and positive responses.
 *         It verifies if the received RSID conforms to the sent SID
 *
 *  \param pPduInfo data which was received from the bus
 *  \param PciType  PCI type of the received PDU
 *  \param SID      SID of the received PDU
 *
 *  \return Type of response
 *  \retval LINTP_PENDING_RESPONSE     A response pending frame has been received
 *  \retval LINTP_NORMAL_RESPONSE      Anything else (no response pending) has been received
 */
STATIC FUNC(LinTp_Response_Type, LINIF_CODE) LinTp_GetResponseType
(
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo,
    const LinTp_PCIType PciType,
    const uint8 SID
)
{
    LinTp_Response_Type responseType;


    if((((SID <= LINTP_STANDARD_SID_UPPER_LIMIT) &&
         (PciType == LINTP_PCI_SF)) &&
         (pPduInfo->SduDataPtr[LINIF_PDU_BYTE_SID] == LINIF_STANDARD_RSID_NEGATIVE_RESPONSE)) &&
         (pPduInfo->SduDataPtr[LINIF_PDU_BYTE_SID + 1U] == SID) &&
         (pPduInfo->SduDataPtr[LINIF_PDU_BYTE_SID + 2U] == LINIF_STANDARD_ERRC_RESPONSE_PENDING))
    {
        /* this is a negative response - response pending contained in a SF */
        responseType = LINTP_PENDING_RESPONSE;
    }
    else
    {
        /* either a standard negative or positive response */
        responseType = LINTP_NORMAL_RESPONSE;
    }

    return responseType;
}
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

/** \brief This function returns the PCI type,
 *         message length and payload of received SF and FF.
 *         It verifies propriety of Pdu bytes.
 *
 *  \param pPduInfo      data which was received from the bus
 *  \param PciType       returns PCI type of the received PDU
 *  \param MsgLength     returns whole message length
 *  \param PayloadLength returns payload of received PDU
 *
 *  \return Result of operation
 *  \retval E_OK      PDU is valid
 *  \retval E_NOT_OK  PDU contains invalid data
 */
STATIC FUNC(Std_ReturnType, LINIF_CODE) LinTp_GetPduInfo
(
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo,
    LinTp_PCIType *PciType,
    uint16 *MsgLength,
    uint8 *PayloadLength
)
{
    Std_ReturnType retCode = E_NOT_OK;
    const uint8 PciTypeByte = pPduInfo->SduDataPtr[LINIF_PDU_BYTE_PCI] & 0xF0U;
    const uint8 PduLength = (uint8) pPduInfo->SduLength;

    *MsgLength = ((uint16) pPduInfo->SduDataPtr[LINIF_PDU_BYTE_PCI]) & 0x0FU;

    if(PduLength == LINTP_PDU_LENGTH)
    {
        switch(PciTypeByte)
        {
            case LINTP_PCI_SF_FRAME:
                  *PciType = LINTP_PCI_SF;

                  /* data should fit into a SF */
                  if((*MsgLength > 0U) &&
                     (*MsgLength <= LINTP_PCI_DATA_LENGTH)
                    )
                  {
                      *PayloadLength = (uint8) *MsgLength;
                      retCode = E_OK;
                  }
                  break;
            case LINTP_PCI_FF_FRAME:

                  *PciType = LINTP_PCI_FF;
                  *MsgLength <<= 8U;
                  *MsgLength |= (uint16)pPduInfo->SduDataPtr[LINIF_PDU_BYTE_LEN];

                  /* length should not be smaller than 7 */
                  if(*MsgLength >= LINTP_FF_DATA_MIN_LENGTH)
                  {
                      *PayloadLength = LINTP_PCI_FF_DATA_LENGTH;
                      retCode = E_OK;
                  }
                  break;
            default :  /* default case intended to be empty */
                  break;
        }
    }

    return retCode;
}


/** \brief This function verifies correct reception of a CF.
 *
 *  \param pPduInfo     data which was received from the bus
 *  \param FrameCounter accepted sequence number of CF
 *
 *  \return Result of operation
 *  \retval LINTP_OK             received CF is valid
 *  \retval LINTP_E_NOT_OK       unknown PCI received
 *  \retval LINTP_E_SN           CF has wrong sequence number
 *  \retval LINTP_NEW_RECEPTION  SF or FF was received instead of CF
 */
STATIC FUNC(LinTp_FrameReturnType, LINIF_CODE) LinTp_VerifyCF
(
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo,
    const uint8 FrameCounter
)
{
    LinTp_FrameReturnType retCode = LINTP_E_NOT_OK;
    const uint8 PciTypeByte =
                    pPduInfo->SduDataPtr[LINIF_PDU_BYTE_PCI] & 0xF0U;
    const uint8 PduLength = (uint8) pPduInfo->SduLength;
    const uint8 currentFrameCounter =
                    pPduInfo->SduDataPtr[LINIF_PDU_BYTE_PCI] & 0x0FU;

    if(PduLength == LINTP_PDU_LENGTH)
    {
        switch(PciTypeByte)
        {
            case LINTP_PCI_CF_FRAME:
                  /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00079.SRF,1,
                              LinIf.ASR20-11.SWS_LinIf_00079.MRF,1
                   */
                  if(currentFrameCounter == FrameCounter)
                  {
                      /* received CF is correct */
                      retCode = LINTP_OK;
                  }
                  else
                  {
                      /* received CF has invalid sequence number */
                      retCode = LINTP_E_SN;
                  }
                  break;
            case LINTP_PCI_SF_FRAME:
            case LINTP_PCI_FF_FRAME:
                  /* SF, FF was received instead CF,
                     initiate new reception */
                  retCode = LINTP_NEW_RECEPTION;
                  break;
            default :
                  /* unknown frame was received */
                  break;
        }
    }

    return retCode;
}


/** \brief This function determines the LinTpRxSduId through received NAD.
 *
 *  \param currentChannel  Channel which is processed
 *  \param NAD             received NAD
 *  \param LinTpRxSduId    returns SDU id with the associated NAD
 *
 *  \return Result of operation
 *  \retval LINTP_E_OK           NAD could be converted to LinTpRxSduId
 *  \retval LINTP_E_NOT_OK       unknown NAD received
 */
STATIC FUNC(Std_ReturnType, LINIF_CODE) LinTp_GetLinTpRxSduId
(
    const uint8 currentChannel,
    const uint8 NAD,
    PduIdType *LinTpRxSduId
)
{
    Std_ReturnType RetCode = E_NOT_OK;

    CONSTP2CONST(LinTp_ConfigType, TYPEDEF, LINIF_APPL_CONST) pTpConfig =
        LinTp_ConfigPtr;

    CONSTP2CONST(LinTpRxNSduType, TYPEDEF, LINIF_APPL_CONST) pTpRxConfig =
        LINTP_CFG_GET_RXNSDU;

    const uint16 LinTpNumberOfRxNSdu = pTpConfig->LinTpNumberOfRxNSdu;
#if (LINIF_SLAVE_SUPPORTED == STD_ON)
#if (LINIF_MASTER_SUPPORTED == STD_ON)
    const uint8 lookupNad = (currentChannel < LinIf_ConfigPtr->NumChannelsMaster) ? NAD : 0U;
#else
    TS_PARAM_UNUSED(NAD);
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */
#endif /* LINIF_SLAVE_SUPPORTED == STD_ON */

    for(*LinTpRxSduId = 0U; *LinTpRxSduId < LinTpNumberOfRxNSdu; (*LinTpRxSduId)++)
    {
        if(pTpRxConfig[*LinTpRxSduId].LinIfChannel == currentChannel)
        {
#if(LINIF_MASTER_SUPPORTED == STD_ON)
#if(LINIF_SLAVE_SUPPORTED == STD_OFF)
            if(pTpRxConfig[*LinTpRxSduId].LinTpRxNSduNad == NAD)
#else
            if(pTpRxConfig[*LinTpRxSduId].LinTpRxNSduNad == lookupNad)
#endif /* LINIF_SLAVE_SUPPORTED == STD_OFF */
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */
            {
                RetCode = E_OK;
                break;
            }
        }
    }

    return RetCode;
}


/** \brief This function ends or aborts an ongoing rx transmission
 *
 *  \param Result         Indicates why the transmission should be finished
 *  \param currentChannel Channel where the transmission is closed
 */
STATIC FUNC(void, LINIF_CODE) LinTp_StopRx
(
    uint8 currentChannel,
    NotifResultType Result
)
{
    CONSTP2VAR(LinTp_ChannelInfoType, AUTOMATIC, LINIF_VAR) pTpData =
        &LinTp_ChannelInfo[currentChannel];

    CONSTP2VAR(LinTp_PhysChannelInfoType, AUTOMATIC, LINIF_VAR) pPhysData =
        &pTpData->PhysChannelInfo;

    CONSTP2CONST(LinTpRxNSduType, TYPEDEF, LINIF_APPL_CONST) pTpRxConfig =
        &LINTP_CFG_GET_RXNSDU[pPhysData->SduId];

    const PduIdType PduRScrPduHandleId = pTpRxConfig->PduRScrPduHandleId;

    /* Perform schedule table switch to applicative schedule */

#if (LINTP_SCHEDULE_CHANGE_DIAG_API == STD_ON)
    {
      const uint8 LinTpOptions = pTpRxConfig->LinTpOptions;

      if((LinTpOptions & LINTP_SCHEDULE_CHANGE_DIAG) == LINTP_SCHEDULE_CHANGE_DIAG)
      {
          uint8 ChannelFlags;

          SchM_Enter_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

          TS_AtomicAssign8(ChannelFlags, LinTp_ChannelInfo[currentChannel].ChannelFlags);

          /* Set a flag to prevent interruption while we make the schedule switch */
          ChannelFlags |= LINTP_FLAG_PHYS_RX_BSWM_SCHEDULE_CHANGE_REQUESTED;

          TS_AtomicAssign8(LinTp_ChannelInfo[currentChannel].ChannelFlags, ChannelFlags);

          pPhysData->PduRResult = Result;

          SchM_Exit_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

          /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00078.SRF.lastCF,1,
                      LinIf.ASR20-11.SWS_LinIf_00701.Master.SF.SCD.TRUE,1,
                      LinIf.ASR20-11.SWS_LinIf_00701.Master.FF.SCD.TRUE,1
           */
          BswM_LinTp_RequestMode(LINIF_GET_COMM_CHANNEL(currentChannel),
                                 LINTP_APPLICATIVE_SCHEDULE
                                );
      }
      else
      {
          /* Inform the PduR that a reception was stopped */
          /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00701.Master.FF.SCD.FALSE,1,
                      LinIf.ASR20-11.SWS_LinIf_00701.Master.SF.SCD.FALSE,1
           */
          PduR_LinTpRxIndication(PduRScrPduHandleId, Result);

          DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_NAD_GRP(currentChannel,
                                                        (pPhysData->NAD),
                                                        (LINTP_INVALID_NAD)
                                                       );

          pPhysData->NAD = LINTP_INVALID_NAD;

          DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_SID_GRP(currentChannel,
                                                        (pPhysData->SID),
                                                        (LINTP_INVALID_SID)
                                                       );

          pPhysData->SID = LINTP_INVALID_SID;

          DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_STATE_GRP(currentChannel,
                                                          (pPhysData->State),
                                                          (LINTP_PHYS_STATE_IDLE)
                                                         );

          pPhysData->State = LINTP_PHYS_STATE_IDLE;
      }
    }

#else /* LINTP_SCHEDULE_CHANGE_DIAG_API == STD_ON */

    /* Inform the PduR that a reception was stopped */
    /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00701.Slave.SF,1,
                LinIf.ASR20-11.SWS_LinIf_00701.Slave.FF,1
     */
    PduR_LinTpRxIndication(PduRScrPduHandleId, Result);

    DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_NAD_GRP(currentChannel,
                                                  (pPhysData->NAD),
                                                  (LINTP_INVALID_NAD)
                                                 );

    pPhysData->NAD = LINTP_INVALID_NAD;

    DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_SID_GRP(currentChannel,
                                                  (pPhysData->SID),
                                                  (LINTP_INVALID_SID)
                                                 );

    pPhysData->SID = LINTP_INVALID_SID;

    DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_STATE_GRP(currentChannel,
                                                    (pPhysData->State),
                                                    (LINTP_PHYS_STATE_IDLE)
                                                   );

    /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00414,1 */
    pPhysData->State = LINTP_PHYS_STATE_IDLE;

#endif /* LINTP_SCHEDULE_CHANGE_DIAG_API == STD_ON */

    SchM_Enter_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();
    {
        uint8 ChannelFlags;

        /* LinTp_CancelReceive calls are not allowed anymore */
        TS_AtomicAssign8(ChannelFlags, pTpData->ChannelFlags);
        ChannelFlags &= ~(uint8)LINTP_FLAG_CANCEL_RECEIVE_ALLOWED;
        TS_AtomicAssign8(pTpData->ChannelFlags, ChannelFlags);
    }
    SchM_Exit_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

}

/** \brief This function calls BswM_LinTp_RequestMode to end an ongoing
 *         reception and resets runtime variables
 *
 *  \param currentChannel Channel where the transmission is closed
 *  \param LinTpOptions   Indicates if BswM_LinTp_RequestMode calls
 *                        are activated for the current Pdu
 */
STATIC FUNC(void, LINIF_CODE) LinTp_CancelResponse
(
#if (LINIF_MASTER_SUPPORTED == STD_ON)
    uint8 currentChannel,
    uint8 LinTpOptions
#else
    uint8 currentChannel
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */
)
{
    CONSTP2VAR(LinTp_ChannelInfoType, AUTOMATIC, LINIF_VAR) pTpData =
        &LinTp_ChannelInfo[currentChannel];

    CONSTP2VAR(LinTp_PhysChannelInfoType, AUTOMATIC, LINIF_VAR) pPhysData =
        &pTpData->PhysChannelInfo;


#if (LINTP_SCHEDULE_CHANGE_DIAG_API == STD_ON)

    if((LinTpOptions & LINTP_SCHEDULE_CHANGE_DIAG) == LINTP_SCHEDULE_CHANGE_DIAG)
    {

        uint8 ChannelFlags;

        SchM_Enter_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

        TS_AtomicAssign8(ChannelFlags, LinTp_ChannelInfo[currentChannel].ChannelFlags);

        ChannelFlags |= LINTP_FLAG_PHYS_RX_BSWM_SCHEDULE_CHANGE_REQUESTED;

        TS_AtomicAssign8(LinTp_ChannelInfo[currentChannel].ChannelFlags, ChannelFlags);

        pPhysData->PduRResult = LINTP_NO_PDUR_RESULT;

        SchM_Exit_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00701.Master.FF.SCD.TRUE,1 */
        BswM_LinTp_RequestMode(LINIF_GET_COMM_CHANNEL(currentChannel),
                               LINTP_APPLICATIVE_SCHEDULE
                              );
    }
    else
    {

        DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_NAD_GRP(currentChannel,
                                                  (pPhysData->NAD),
                                                  (LINTP_INVALID_NAD)
                                                 );

        pPhysData->NAD = LINTP_INVALID_NAD;

        DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_SID_GRP(currentChannel,
                                                      (pPhysData->SID),
                                                      (LINTP_INVALID_SID)
                                                     );

        pPhysData->SID = LINTP_INVALID_SID;

        DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_STATE_GRP(currentChannel,
                                                        (pPhysData->State),
                                                        (LINTP_PHYS_STATE_IDLE)
                                                       );

        pPhysData->State = LINTP_PHYS_STATE_IDLE;

    }

#else /* LINTP_SCHEDULE_CHANGE_DIAG_API == STD_ON */

#if (LINIF_MASTER_SUPPORTED == STD_ON)
    TS_PARAM_UNUSED(LinTpOptions);
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

    DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_NAD_GRP(currentChannel,
                                                  (pPhysData->NAD),
                                                  (LINTP_INVALID_NAD)
                                                 );

    pPhysData->NAD = LINTP_INVALID_NAD;

    DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_SID_GRP(currentChannel,
                                                  (pPhysData->SID),
                                                  (LINTP_INVALID_SID)
                                                 );

    pPhysData->SID = LINTP_INVALID_SID;

    DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_STATE_GRP(currentChannel,
                                                    (pPhysData->State),
                                                    (LINTP_PHYS_STATE_IDLE)
                                                   );

    pPhysData->State = LINTP_PHYS_STATE_IDLE;

#endif /* LINTP_SCHEDULE_CHANGE_DIAG_API == STD_ON */

    SchM_Enter_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();
    {
        uint8 ChannelFlags;

        /* LinTp_CancelReceive calls are not allowed anymore */
        TS_AtomicAssign8(ChannelFlags, pTpData->ChannelFlags);
        ChannelFlags &= ~(uint8)LINTP_FLAG_CANCEL_RECEIVE_ALLOWED;
        TS_AtomicAssign8(pTpData->ChannelFlags, ChannelFlags);
    }
    SchM_Exit_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

}


/** \brief This function gets the PCI type and data length
 *         for the next PDU which should be sent
 *
 *  \param MsgLength      Length of whole message
 *  \param MsgLengthRamin Length of the massage which still needs to be sent
 *  \param pciType        returns PCI type for PDU which should be sent next
 *  \param PayloadLength  returns payload length for PDU which should be sent next
 */
STATIC FUNC(void, LINIF_CODE) LinTp_GetNextFrame
(
    const uint16 MsgLength,
    const uint16 MsgLengthRemain,
    LinTp_PCIType *pciType,
    uint8 *PayloadLength
)
{

    if(MsgLength == MsgLengthRemain)
    {
        if(MsgLengthRemain <= LINTP_PCI_DATA_LENGTH)
        {
            *pciType = LINTP_PCI_SF;
            *PayloadLength = (uint8) MsgLengthRemain;
        }
        else
        {
            *pciType = LINTP_PCI_FF;
            *PayloadLength = LINTP_PCI_FF_DATA_LENGTH;
        }
    }
    else
    {
        *pciType = LINTP_PCI_CF;

        if(MsgLengthRemain < LINTP_PCI_DATA_LENGTH)
        {
            *PayloadLength = (uint8) MsgLengthRemain;
        }
        else
        {
            *PayloadLength = LINTP_PCI_DATA_LENGTH;
        }
    }

}


/** \brief This function ends a TX transmission if the the
 *         whole message was sent successfully and initiate
 *         a RX reception if SID is in valid range.
 *
 *  \param currentChannel Channel where transmission should be ended
 */
STATIC FUNC(void, LINIF_CODE) LinTp_StopTxOk
(
    uint8 currentChannel
)
{
    CONSTP2VAR(LinTp_PhysChannelInfoType, AUTOMATIC, LINIF_VAR) pPhysData =
         &LinTp_ChannelInfo[currentChannel].PhysChannelInfo;

    const PduIdType LinTpSduId = pPhysData->SduId;

    CONSTP2CONST(LinTpTxNSduType, TYPEDEF, LINIF_APPL_CONST) pTpTxConfig =
         &LINTP_CFG_GET_TXNSDU[LinTpSduId];

    /* SID between 0x00 and 0x7E are valid,
       only initiate a reception if SID is valid */
#if (LINTP_SCHEDULE_CHANGE_DIAG_API == STD_ON)

    if((pTpTxConfig->LinTpOptions & LINTP_SCHEDULE_CHANGE_DIAG) ==
       LINTP_SCHEDULE_CHANGE_DIAG
      )
    {
        uint8 ChannelFlags;

        SchM_Enter_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

        TS_AtomicAssign8(ChannelFlags, LinTp_ChannelInfo[currentChannel].ChannelFlags);

        /* Set a flag to prevent interruption while we make the schedule switch */
        ChannelFlags |= LINTP_FLAG_PHYS_BSWM_SCHEDULE_CHANGE_REQUESTED;

        TS_AtomicAssign8(LinTp_ChannelInfo[currentChannel].ChannelFlags, ChannelFlags);

        pPhysData->PduRResult = NTFRSLT_OK;

        SchM_Exit_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

        /* Initiate reception by requesting the response schedule */
        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00078.SRF.SF,1 */
        BswM_LinTp_RequestMode(LINIF_GET_COMM_CHANNEL(currentChannel),
                               LINTP_DIAG_RESPONSE
                              );
    }
    else
    {
        const PduIdType PduRDestPduHandleId = pTpTxConfig->PduRDestPduHandleId;

        /* Inform the PduR that a transmission was stopped */
        PduR_LinTpTxConfirmation(PduRDestPduHandleId, NTFRSLT_OK);

        DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_STATE_GRP(currentChannel,
                                                        (pPhysData->State),
                                                        (LINTP_PHYS_STATE_IDLE)
                                                       );
        pPhysData->State = LINTP_PHYS_STATE_IDLE;
    }
#else

    const PduIdType PduRDestPduHandleId = pTpTxConfig->PduRDestPduHandleId;

    /* Inform the PduR that a transmission was stopped */
    PduR_LinTpTxConfirmation(PduRDestPduHandleId, NTFRSLT_OK);

    DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_STATE_GRP(currentChannel,
                                                    (pPhysData->State),
                                                    (LINTP_PHYS_STATE_IDLE)
                                                   );
    /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00414,1 */
    pPhysData->State = LINTP_PHYS_STATE_IDLE;

#endif /* LINTP_SCHEDULE_CHANGE_DIAG_API == STD_ON */

#if (LINIF_MASTER_SUPPORTED == STD_ON)
    {
      CONSTP2CONST(LinTp_ConfigType, TYPEDEF, LINIF_APPL_CONST) pTpConfig =
               LinTp_ConfigPtr;

      LinTp_StartTimerP2(pTpConfig, pPhysData);

      pPhysData->NumberOfRespPendingFrames =
          pTpConfig->LinTpMaxNumberOfRespPendingFrames;
    }
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */
}


/** \brief This function aborts a TX transmission if an error occurs
 *         during TX transmission.
 *
 *  \param Result         Reason of the abortion
 *  \param currentChannel Channel where TX transmission should be
 *                        aborted
 */
STATIC FUNC(void, LINIF_CODE) LinTp_StopTx
(
    uint8 currentChannel,
    NotifResultType Result
)
{
    CONSTP2VAR(LinTp_PhysChannelInfoType, AUTOMATIC, LINIF_VAR) pPhysData =
        &LinTp_ChannelInfo[currentChannel].PhysChannelInfo;

    const PduIdType LinTpSduId = pPhysData->SduId;

    CONSTP2CONST(LinTpTxNSduType, TYPEDEF, LINIF_APPL_CONST) pTpTxConfig =
        &LINTP_CFG_GET_TXNSDU[LinTpSduId];


#if (LINTP_SCHEDULE_CHANGE_DIAG_API == STD_ON)

    if((pTpTxConfig->LinTpOptions & LINTP_SCHEDULE_CHANGE_DIAG) ==
       LINTP_SCHEDULE_CHANGE_DIAG
      )
    {
        uint8 ChannelFlags;

        SchM_Enter_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

        TS_AtomicAssign8(ChannelFlags, LinTp_ChannelInfo[currentChannel].ChannelFlags);

        ChannelFlags |= LINTP_FLAG_PHYS_BSWM_SCHEDULE_CHANGE_REQUESTED;

        TS_AtomicAssign8(LinTp_ChannelInfo[currentChannel].ChannelFlags, ChannelFlags);

        pPhysData->PduRResult = Result;

        SchM_Exit_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

        BswM_LinTp_RequestMode(LINIF_GET_COMM_CHANNEL(currentChannel),
                               LINTP_APPLICATIVE_SCHEDULE
                              );
    }
    else
    {
        const PduIdType PduRDestPduHandleId = pTpTxConfig->PduRDestPduHandleId;

        /* Inform the PduR that a transmission was stopped */
        PduR_LinTpTxConfirmation(PduRDestPduHandleId, Result);

        DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_STATE_GRP(currentChannel,
                                                        (pPhysData->State),
                                                        (LINTP_PHYS_STATE_IDLE)
                                                       );
        pPhysData->State = LINTP_PHYS_STATE_IDLE;
    }

#else

    const PduIdType PduRDestPduHandleId = pTpTxConfig->PduRDestPduHandleId;

    /* Inform the PduR that a transmission was stopped */
    PduR_LinTpTxConfirmation(PduRDestPduHandleId, Result);

    DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_STATE_GRP(currentChannel,
                                                    (pPhysData->State),
                                                    (LINTP_PHYS_STATE_IDLE)
                                                   );
    /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00688,1 */
    pPhysData->State = LINTP_PHYS_STATE_IDLE;

#endif /* LINTP_SCHEDULE_CHANGE_DIAG_API == STD_ON */

}


/** \brief This function copies data from PduR to PDU
 *
 *  \param pPduInfo                  pointer where data is copied
 *  \param PciType                   PCI type of the PDU
 *  \param PayloadLength             Indicates how many bytes should be copied
 *  \param PduRPduHandleId           Indicates from which PduR buffer data is copied
 *  \param AvailablePduRBufferLength returns the available data length after copying
 *
 *  \return Result of operation
 *  \retval LINTP_E_OK           Copying successful
 *  \retval LINTP_E_NOT_OK       Error occurred during copying
 */
STATIC FUNC(Std_ReturnType, LINIF_CODE) LinTp_CopyTxPayloadPduR
(
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo,
    const LinTp_PCIType PciType,
    const uint8 PayloadLength,
    const PduIdType PduRPduHandleId,
    PduLengthType *AvailablePduRBufferLength
)
{
    BufReq_ReturnType BuffState;
    Std_ReturnType retCode = E_NOT_OK;
    PduInfoType pduBuffer;
    uint8 i;


    /* number of bytes which should be copied from PduR */
    pduBuffer.SduLength = PayloadLength;

     /* get position from where data should be copied,
        always after information of data length */
     switch(PciType)
     {
         case LINTP_PCI_FF:
             pduBuffer.SduDataPtr = &pPduInfo->SduDataPtr[LINIF_PDU_BYTE_SID_FF];
             break;
         case LINTP_PCI_CF:
         case LINTP_PCI_SF:
             pduBuffer.SduDataPtr = &pPduInfo->SduDataPtr[LINIF_PDU_BYTE_SID];
             break;
         /* CHECK: NOPARSE */
         default:
            /*should not happen*/
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
            LINIF_UNREACHABLE_CODE_ASSERT(LINIF_API_MAINFUNCTION);
#endif /* LINIF_DEV_ERROR_DETECT == STD_ON */
            break;
         /* CHECK: PARSE */
     }

     /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00705,1 */
     BuffState = PduR_LinTpCopyTxData(
                                      PduRPduHandleId,
                                      &pduBuffer,
                                      NULL_PTR,
                                      AvailablePduRBufferLength
                                     );

     if(BuffState == BUFREQ_OK)
     {
         /* if PDU length is bigger than payload fill unused bytes with 0xFF */
         if((PciType == LINTP_PCI_SF) || (PciType == LINTP_PCI_CF))
         {
             for(i = PayloadLength; i < LINTP_PCI_DATA_LENGTH; i++)
             {
                 pduBuffer.SduDataPtr[i] = LINIF_UNUSED_DATA;
             }
         }

         retCode = E_OK;
     }

    return retCode;
}


/** \brief This function copies data from received PDU to PduR
 *
 *  \param pPduInfo                  pointer where data is copied
 *  \param PciType                   PCI type of the PDU
 *  \param PayloadLength             Indicates how many bytes should be copied
 *  \param PduRPduHandleId           Indicates to which PduR buffer data is copied
 *  \param AvailablePduRBufferLength returns the available data length after copying
 *
 *  \return Result of operation
 *  \retval LINTP_E_OK           Copying successful
 *  \retval LINTP_E_NOT_OK       Error occurred during copying
 */
STATIC FUNC(Std_ReturnType, LINIF_CODE) LinTp_CopyRxPayloadPduR
(
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo,
    const LinTp_PCIType PciType,
    const uint8 PayloadLength,
    const PduIdType PduRPduHandleId,
    PduLengthType *AvailablePduRBufferLength
)
{
    BufReq_ReturnType BuffState;
    Std_ReturnType retCode = E_NOT_OK;
    PduInfoType pduBuffer;


    pduBuffer.SduLength = PayloadLength;

     /* get position from where data should be copied,
        always after information of data length */
     switch(PciType)
     {
         case LINTP_PCI_FF:
             pduBuffer.SduDataPtr = &pPduInfo->SduDataPtr[LINIF_PDU_BYTE_SID_FF];
             break;
         case LINTP_PCI_CF:
         case LINTP_PCI_SF:
             pduBuffer.SduDataPtr = &pPduInfo->SduDataPtr[LINIF_PDU_BYTE_SID];
             break;
         /* CHECK: NOPARSE */
         default:
             /*should not happen*/
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
             LINIF_UNREACHABLE_CODE_ASSERT(LINIF_API_MAINFUNCTION);
#endif /* LINIF_DEV_ERROR_DETECT == STD_ON */
             break;
         /* CHECK: PARSE */
     }

     BuffState = PduR_LinTpCopyRxData(
                                      PduRPduHandleId,
                                      &pduBuffer,
                                      AvailablePduRBufferLength
                                     );

     if(BuffState == BUFREQ_OK)
     {
         retCode = E_OK;
     }

    return retCode;
}


/** \brief This function evaluates the NAD, PCI and data
 *         length of the next Pdu
 *
 *  \param pPduInfo           Pdu which is sent next
 *  \param currentPCIType     PCI type of the PDU which is sent next
 *  \param NAD                NAD of the PDU which is sent next
 *  \param PciAdditionalInfo  Includes the whole message length in case
 *                            of SF and FF or Sequence number of CF
 */
STATIC FUNC(void, LINIF_CODE) LinTp_BuildHeader
(
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo,
    const LinTp_PCIType currentPCIType,
    const uint8 NAD,
    const uint16 PciAdditionalInfo
)
{

    /* length of Pdu is always 8 bytes */

    pPduInfo->SduLength = LINTP_PDU_LENGTH;

    /* first byte of PDU is NAD */
    pPduInfo->SduDataPtr[LINIF_PDU_BYTE_NAD] = NAD;

    /* second byte of PDU consists of PCI type and data length (SF, FF)
       or Frame Counter (CF) */
    switch(currentPCIType)
    {
        case LINTP_PCI_SF:
            pPduInfo->SduDataPtr[LINIF_PDU_BYTE_PCI] = LINIF_PCI_SF_FRAME |
                                                       (uint8) PciAdditionalInfo;
            break;

        case LINTP_PCI_CF:
            pPduInfo->SduDataPtr[LINIF_PDU_BYTE_PCI]  = LINIF_PCI_CF_FRAME |
                                                        (uint8) PciAdditionalInfo;
            break;

        case LINTP_PCI_FF:
            pPduInfo->SduDataPtr[LINIF_PDU_BYTE_PCI]  = LINIF_PCI_FF_FRAME |
                                                        (uint8)(PciAdditionalInfo >> 8U);
            pPduInfo->SduDataPtr[LINIF_PDU_BYTE_LEN] = (uint8)(PciAdditionalInfo & 0xFFU);
            break;

        /* CHECK: NOPARSE */
        default:
            /*should not happen*/
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
            LINIF_UNREACHABLE_CODE_ASSERT(LINIF_API_MAINFUNCTION);
#endif /* LINIF_DEV_ERROR_DETECT == STD_ON */
            break;
        /* CHECK: PARSE */
    }

}


/** \brief This function calls the PduR to get the available data
 *         length of the TX PduR buffer
 *
 *  \param PduRDestPduHandleId        Id of the PduR buffer
 *  \param AvailablePduRBufferLength  returns available data length
 *
 *  \return Result of operation
 *  \retval LINTP_E_OK           No error occurred
 *  \retval LINTP_E_NOT_OK       Error occurred during PduR call
 */
STATIC FUNC(Std_ReturnType, LINIF_CODE) LinTp_GetAvailablePduRTxBufferLength
(
    const PduIdType PduRDestPduHandleId,
    PduLengthType *AvailablePduRBufferLength
)
{
    Std_ReturnType RetCode = E_NOT_OK;
    BufReq_ReturnType BuffState = BUFREQ_E_NOT_OK;
    PduInfoType pduInfo;


    pduInfo.SduLength = LINTP_EMPTY_SDU_LENGTH;
    pduInfo.SduDataPtr = NULL_PTR;

    /* call PduR_LinTpCopyTxData to get available data length */
    /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00705,1 */
    BuffState = PduR_LinTpCopyTxData(PduRDestPduHandleId,
                                     &pduInfo,
                                     NULL_PTR,
                                     AvailablePduRBufferLength
                                    );

    if((BuffState == BUFREQ_OK) || (BuffState ==  BUFREQ_E_BUSY))
    {
        RetCode = E_OK;
    }

    return RetCode;
}


/** \brief This function calls the PduR to get the available data
 *         length of the Rx PduR buffer
 *
 *  \param PduRDestPduHandleId        Id of the PduR buffer
 *  \param AvailablePduRBufferLength  returns available data length
 *
 *  \return Result of operation
 *  \retval LINTP_E_OK           No error occurred
 *  \retval LINTP_E_NOT_OK       Error occurred during PduR call
 */
STATIC FUNC(Std_ReturnType, LINIF_CODE) LinTp_GetAvailablePduRRxBufferLength
(
    const PduIdType PduRPduHandleId,
    PduLengthType *AvailablePduRBufferLength
)
{
    Std_ReturnType RetCode = E_NOT_OK;
    BufReq_ReturnType BuffState = BUFREQ_E_NOT_OK;
    PduInfoType pduInfo;


    pduInfo.SduLength = LINTP_EMPTY_SDU_LENGTH;

    /* call PduR_LinTpCopyRxData to get available data length */
    BuffState = PduR_LinTpCopyRxData(
                                     PduRPduHandleId,
                                     &pduInfo,
                                     AvailablePduRBufferLength
                                    );

    if((BuffState == BUFREQ_OK) || (BuffState ==  BUFREQ_E_BUSY))
    {
        RetCode = E_OK;
    }

    return RetCode;
}


/** \brief This function handles the counter state of the timers,
 *         if a timeout occurs the transmission is aborted by
 *         informing the upper layers.
 *
 *  \param currentChannel Channel where timeouts are checked
 */
STATIC FUNC(void, LINIF_CODE) LinTp_TimerHandling
(
    uint8 currentChannel
)
{
    CONSTP2VAR(LinTp_PhysChannelInfoType, AUTOMATIC, LINIF_VAR) pPhysData =
        &LinTp_ChannelInfo[currentChannel].PhysChannelInfo;

#if (LINIF_MASTER_SUPPORTED == STD_ON)
    CONSTP2VAR(LinTp_FuncChannelInfoType, AUTOMATIC, LINIF_VAR) pFuncData =
        &LinTp_ChannelInfo[currentChannel].FuncChannelInfo;

    uint16 PhysP2TimerState = pPhysData->P2Timer;
    uint16 FuncTimerState = pFuncData->Timer;
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */
    uint16 PhysTimerState = pPhysData->Timer;


#if (LINTP_SCHEDULE_CHANGE_DIAG_API == STD_ON)
    uint8 ChannelFlags;
#endif /* LINTP_SCHEDULE_CHANGE_DIAG_API == STD_ON */

#if (LINIF_MASTER_SUPPORTED == STD_ON)
    /* Check the counter state of the P2 physical timer */
    if(PhysP2TimerState > 0U)
    {
        PhysP2TimerState--;

        /* Timeout occurred */
        if(PhysP2TimerState == 0U)
        {
            const uint8 LinTpOptions =
                        LINTP_CFG_GET_TXNSDU[pPhysData->SduId].LinTpOptions;

#if (LINTP_SCHEDULE_CHANGE_DIAG_API == STD_ON)

            TS_AtomicAssign8(ChannelFlags, LinTp_ChannelInfo[currentChannel].ChannelFlags);

            /* !LINKSTO LinTp.EB.P2Timeout.BswMScheduleChange,2 */
            /* Check if there is an undergoing BswM schedule change */
            if((ChannelFlags&LINTP_BSWM_SCHEDULE_SWITCH_NOT_FROM_P2_FLAGS_MASK) != 0U)
            {
                /* let the MainFunction know that we need to change to APPLICATIVE after
                the current schedule change finishes, because P2 timeout expired */
                ChannelFlags |= LINTP_FLAG_P2_BSWM_SCHEDULE_CHANGE_REQUESTED;

                TS_AtomicAssign8(LinTp_ChannelInfo[currentChannel].ChannelFlags, ChannelFlags);
            }
            else
            {
#endif /* LINTP_SCHEDULE_CHANGE_DIAG_API == STD_ON */
                /* Cancel response now */
                LinTp_CancelResponse(currentChannel, LinTpOptions);

#if (LINTP_SCHEDULE_CHANGE_DIAG_API == STD_ON)
            }
#endif /* LINTP_SCHEDULE_CHANGE_DIAG_API == STD_ON */
        }
    }

#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

    /* Check the counter state of the physical timer */
    if(PhysTimerState > 0U)
    {
        PhysTimerState--;

        /* timeout occurred */
        if(PhysTimerState == 0U)
        {
            const LinTp_PhysStates_Type State = pPhysData->State;

            switch(State)
            {
                case LINTP_PHYS_STATE_WAITRX:
                {
                    /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00806_1,1,
                                LinIf.ASR20-11.SWS_LinIf_00806_2,1
                     */
                    LinTp_StopRxTimeoutCR(currentChannel);
                    break;
                }
                case LINTP_PHYS_STATE_WAITTX:
                {
#if (LINIF_SLAVE_SUPPORTED == STD_ON)
                    /* SF /FF Slave transmission without SRF */
                    if(pPhysData->MsgLengthTotal == pPhysData->MsgLengthRemain)
                    {
                      /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00801_1,1 */
                      LinTp_StopTxTimeoutAS(currentChannel);
                    }
                    else
#endif /* LINIF_SLAVE_SUPPORTED == STD_ON */
                    {
                      /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00803_3,1 */
                      LinTp_StopTxTimeoutCS(currentChannel);
                    }
                    break;
                }
                case LINTP_PHYS_STATE_TXPENDING:
                {
                    /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00801_1,1 */
                    LinTp_StopTxTimeoutAS(currentChannel);
                    break;
                }
                /* CHECK: NOPARSE */
                case LINTP_PHYS_STATE_IDLE:
                default:
                    /*should not happen*/
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
            LINIF_UNREACHABLE_CODE_ASSERT(LINIF_API_MAINFUNCTION);
#endif /* LINIF_DEV_ERROR_DETECT == STD_ON */
                    break;
                /* CHECK: PARSE */
            }
        }
    }

#if (LINIF_MASTER_SUPPORTED == STD_ON)
    /* Check the counter state of the functional timer */
    if(FuncTimerState > 0U)
    {
        FuncTimerState--;

        if(FuncTimerState == 0U)
        {
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
            LINIF_PRECONDITION_ASSERT(pFuncData->State ==
                                      LINTP_FUNC_STATE_TXPENDING, LINIF_API_MAINFUNCTION
                                     );
#endif /* LINIF_DEV_ERROR_DETECT == STD_ON */

            LinTp_StopFuncTransmissionTimeoutAS(currentChannel);
        }
    }

    pPhysData->P2Timer = PhysP2TimerState;
    pFuncData->Timer = FuncTimerState;
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */
    pPhysData->Timer = PhysTimerState;

}

#if (LINIF_SLAVE_SUPPORTED == STD_ON)
FUNC(uint8, LINIF_CODE) LinTp_GetConfNad
(
    uint8 currentChannel
)
{
    CONSTP2VAR(LinIf_SlaveChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannelSlave =
        LINIF_GET_CHANNEL_SLAVE(currentChannel);
    uint8 NAD;

    TS_AtomicSetBit_8(&pIfChannelSlave->Flags, LINIF_BIT_FLG_CONF_NAD_ALTER);
    /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00076.MRF,1 */
    NAD = pIfChannelSlave->ConfNad;
    TS_AtomicClearBit_8(&pIfChannelSlave->Flags, LINIF_BIT_FLG_CONF_NAD_ALTER);

    return NAD;
}
#endif /* LINIF_SLAVE_SUPPORTED == STD_ON */

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


/*==================[end of file]===========================================*/
