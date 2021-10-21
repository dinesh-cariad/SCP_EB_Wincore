/**
 * \file
 *
 * \brief AUTOSAR CanTp
 *
 * This file contains the implementation of the AUTOSAR
 * module CanTp.
 *
 * \version 6.8.42
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/
#include <CanTp_Trace.h>
#include <TSAutosar.h>         /* EB specific standard types */
#include <ComStack_Types.h>    /* typedefs for AUTOSAR com stack */
#include <CanTp_Internal.h>    /* internal macros and variables */

/* do not indirectly include PduR_SymbolicNames_PBcfg.h via PduR_CanTp.h */
#define PDUR_NO_PBCFG_REQUIRED
#include <PduR_CanTp.h>        /* PduR module API */
#include <TSMem.h>             /* TS_MemCpy */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/
#if(TS_MERGED_COMPILE == STD_OFF)
#define CANTP_START_SEC_CODE
#include <CanTp_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)

/** \brief Request Tx frame data.
 **
 ** This function shall request for Tx data until frame is ready to send.
 **
 ** \param[in] ChannelPtr  Pointer to channel data structure to ease lookups.
 **
 ** \return BUFREQ_OK: Frame is ready to send.
 **         BUFREQ_E_BUSY: Frame not ready to send. More data required.
 **         BUFREQ_E_*: Upper layer error.
 */
STATIC FUNC(BufReq_ReturnType, CANTP_CODE) CanTp_RequestTxFrameData
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
);

/** \brief Request data from upper layer.
 **
 ** This function will execute the API call PduR_CanTpCopyTxData() and will return requested data
 ** and set available data for next call.
 **
 ** \param[in] ChannelPtr  Pointer to channel data structure to ease lookups.
 ** \param[in/out] PduInfoPtr  Pointer to be used to request and store upper layer data.
 **
 ** \return The function will return the return value of PduR_CanTpCopyTxData().
 */
STATIC FUNC(BufReq_ReturnType, CANTP_CODE) CanTp_ProcessCopyTxData
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr,
  P2VAR(PduInfoType, AUTOMATIC, CANTP_VAR) PduInfoPtr
);

#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */

#if(TS_MERGED_COMPILE == STD_OFF)
#define CANTP_STOP_SEC_CODE
#include <CanTp_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */
/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#if(TS_MERGED_COMPILE == STD_OFF)
#define CANTP_START_SEC_CODE
#include <CanTp_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)

FUNC(void, CANTP_CODE) CanTp_TxStateHandling
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr,
  boolean CFBufferRequested
)
{
  CanTp_ChStateType ChannelState;
  CanTp_ChType Rx_ChannelID = INVALID_RX_CHANNEL_ID;
  boolean Ret_Postponed = FALSE;
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) Rx_ChannelPtr;

  TS_AtomicAssign8(ChannelState, ChannelPtr->State);

  switch(ChannelState)
  {
    case CANTP_CH_TX_BUFREQ_SF:
    case CANTP_CH_TX_BUFREQ_FF:
    case CANTP_CH_TX_BUFREQ_CF:
    {
      const PduIdType NSduId = ChannelPtr->NSduId;

      /* check if buffer hasn't been already requested. */
      if(CFBufferRequested != TRUE)
      {
        /* Try to request enough data to send next frame. */
        RetVal_DataReq = CanTp_RequestTxFrameData(ChannelPtr);
      }
      else
      {
        /* do nothing */
      }

      switch(RetVal_DataReq)
      {
        case BUFREQ_OK:
        {
#if(CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON)
          PduIdType PduId = CANTP_CFG_GET_TXNSDU_CONFIG(ChannelPtr->NSduId).NPduConfIdxF;
          /* Get the corresponding Rx Channel */
          if(E_OK == CanTp_GetChannelOfPostponedTxFcPdu(PduId, &Rx_ChannelID))
          {
            CANTP_IS_TX_CH_CONFIRM_ACTION(Rx_ChannelID, FALSE, Ret_Postponed);
          }
#else
          Rx_ChannelID = CANTP_CFG_GET_TXNSDU_CONFIG(ChannelPtr->NSduId).Rx_Channel;
          CANTP_IS_TX_CH_CONFIRM_ACTION(Rx_ChannelID, FALSE, Ret_Postponed);
#endif/* CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON */

          /* Check and fill frame with padding bytes if required. */
#if(CANTP_CANFD_ENABLED == STD_ON)
          CanTp_PreparePadding(ChannelPtr, CANTP_CFG_GET_TXNSDU_CONFIG(NSduId).PaddingActivation, CANTP_CFG_GET_TXNSDU_CONFIG(NSduId).CanIdType,NULL_PTR);
#else
          CanTp_PreparePadding(ChannelPtr, CANTP_CFG_GET_TXNSDU_CONFIG(NSduId).PaddingActivation, CANTP_TX_INVALID_CANID,NULL_PTR);
#endif /* CANTP_CANFD_ENABLED == STD_ON */

          /* Check if a reception is ongoing on related Rx_ChannelID channel*/
          if( Ret_Postponed == TRUE)
          {
            Rx_ChannelPtr = &CanTp_Channel[Rx_ChannelID];
            if((ChannelState == CANTP_CH_TX_BUFREQ_SF)||(ChannelState == CANTP_CH_TX_BUFREQ_FF))
            {
              /*In case of an ongoing FC transmission on the same PDU id, if the time period(N_Cs) of the postponed SF/FF is smaller
                than the remaining time period to transmit the FC, the CanTp will use remaining time of (N_Ar+N_Br)  to send the SF/FF.*/
              if(ChannelPtr->Ticks < Rx_ChannelPtr->Ticks)
              {
                ChannelPtr->Ticks = Rx_ChannelPtr->Ticks;
              }
            }
            CANTP_CH_SET_ACTION(ChannelPtr, CANTP_CH_ACTION_POSTPONE_FRAME);
          }
          else
          {
            ChannelPtr->Ticks = CANTP_GET_TX_PDU_N_AS(&CANTP_CFG_GET_TXNSDU_CONFIG(NSduId));
            /* Change to next state, set new timer and start transmission */
            CANTP_CH_SET_ACTION(ChannelPtr, CANTP_CH_ACTION_CONFIRM);
            CanTp_TransmitFrame(ChannelPtr,NULL_PTR);
          }
          break;
        }
        case BUFREQ_E_BUSY:
        {
          /* Wait in this state */
          break;
        }
        default:
        {
          /* Unexpected return value -> Abort reception */
          CanTp_StopChannel(ChannelPtr, CANTP_ULIND_STATUS, NTFRSLT_E_NOT_OK);
          break;
        }
      }
      break;
    }

    case CANTP_CH_TX_CONFIRM_SF: /* Fall through */
    case CANTP_CH_TX_CONFIRM_FF: /* Fall through */
    case CANTP_CH_TX_CONFIRM_CF:
    {
      /* Wait for confirm. CanTp_TxConfirmation() or timeout will take care of
         changing state.
       */
      break;
    }

    case CANTP_CH_TX_RECEIVE_FC:
    {
      /* Wait until FC frame received or timeout occurs */
      break;
    }

    case CANTP_CH_TX_WAITTMR_CF:
    {
      /* Request buffer for next CF. */
      TS_AtomicAssign8(ChannelPtr->State, CANTP_CH_TX_BUFREQ_CF);
      RetVal_DataReq = CanTp_RequestTxFrameData(ChannelPtr);

      switch(RetVal_DataReq)
      {
        case BUFREQ_OK:
        {
          /* Check if STmin is greater than N_Cs. */
          if(ChannelPtr->MfCounterTicks > ChannelPtr->Ticks)
          {
            ChannelPtr->Ticks = ChannelPtr->MfCounterTicks;
          }
          TS_AtomicAssign8(ChannelPtr->State, CANTP_CH_TX_REQUESTEDBUF_CF);
          break;
        }
        case BUFREQ_E_BUSY:
        {
          TS_AtomicAssign8(ChannelPtr->State, CANTP_CH_TX_WAITTMR_CF);
          break;
        }
        default:
        {
          /* Unexpected return value -> Abort reception */
          CanTp_StopChannel(ChannelPtr, CANTP_ULIND_STATUS, NTFRSLT_E_NOT_OK);
          break;
        }
      }
      /* Wait until time elapsed to request buffer for next CF.
         Main function takes care of the timeout.
       */
      break;
    }
    case CANTP_CH_TX_REQUESTEDBUF_CF:
    {
    /* Wait until STmin expires in order to transmit next CF. */
      break;
    }
    /* CHECK: NOPARSE */
    default:
    {
      /* unexpected state - should never be reached */
      CANTP_UNREACHABLE_CODE_ASSERT(CANTP_INTERNAL_API_ID);
      break;
    }
    /* CHECK: PARSE */
  }
}

#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */

#if(TS_MERGED_COMPILE == STD_OFF)
#define CANTP_STOP_SEC_CODE
#include <CanTp_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

/*==================[internal function definitions]=========================*/
#if(TS_MERGED_COMPILE == STD_OFF)
#define CANTP_START_SEC_CODE
#include <CanTp_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)

STATIC FUNC(BufReq_ReturnType, CANTP_CODE) CanTp_RequestTxFrameData
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
)
{
  BufReq_ReturnType Ret_CopyTxData = BUFREQ_E_NOT_OK;
  PduInfoType PduInfo; /* info structure to provide data to upper layer */

  if(ChannelPtr->CanIfDLC == 0U)
  {
    CanTp_PreparexF(ChannelPtr);
  }

  if(ChannelPtr->ActualBufferSize == 0U)
  {
    /* Ask if more data is available. */
    PduInfo.SduDataPtr = NULL_PTR;
    PduInfo.SduLength = 0U;

    Ret_CopyTxData = CanTp_ProcessCopyTxData(ChannelPtr, &PduInfo);

    if((Ret_CopyTxData == BUFREQ_OK) && (ChannelPtr->ActualBufferSize == 0U))
    {
      /* Call was only successful if data are available */
      Ret_CopyTxData = BUFREQ_E_BUSY;
    }
  }

  if(ChannelPtr->ActualBufferSize > 0U)
  {
    const PduLengthType RemainingMsgBytes = ChannelPtr->TotalLength - ChannelPtr->ProcessedLength;
    /* Requesting bytes: Initial value is the remaining free space of the actual frame. */
    uint8 ReqTxFrameBytes
      = CANTP_CFG_GET_TXNSDU_CONFIG(ChannelPtr->NSduId).LSduMaxLength - ChannelPtr->CanIfDLC;
    boolean FrameReadyToSend;

    if(ReqTxFrameBytes > RemainingMsgBytes)
    {
      ReqTxFrameBytes = (uint8) RemainingMsgBytes;
    }

    if(ReqTxFrameBytes > ChannelPtr->ActualBufferSize)
    {
      /* Not enough data available to send frame. Request only the available data. */
      ReqTxFrameBytes = (uint8) ChannelPtr->ActualBufferSize;
      FrameReadyToSend = FALSE;
    }
    else
    {
      /* Enough data available to fill the frame. Start sending if data request was successful. */
      FrameReadyToSend = TRUE;
    }

    PduInfo.SduLength = ReqTxFrameBytes;
    PduInfo.SduDataPtr = &ChannelPtr->CanIfData[ChannelPtr->CanIfDLC];

    Ret_CopyTxData = CanTp_ProcessCopyTxData(ChannelPtr, &PduInfo);

    if(Ret_CopyTxData == BUFREQ_OK)
    {
      ChannelPtr->CanIfDLC += ReqTxFrameBytes;
      ChannelPtr->ProcessedLength += ReqTxFrameBytes;

      if(FrameReadyToSend == (boolean)FALSE)
      {
        /* Not enough data are copied to send next frame. */
        Ret_CopyTxData = BUFREQ_E_BUSY;
      }
    }
  }

  return Ret_CopyTxData;
}

STATIC FUNC(BufReq_ReturnType, CANTP_CODE) CanTp_ProcessCopyTxData
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr,
  P2VAR(PduInfoType, AUTOMATIC, CANTP_VAR) PduInfoPtr
)
{
  BufReq_ReturnType Ret_CopyTxData;
  PduLengthType UL_BufferSize = 0U;

  Ret_CopyTxData = CANTP_PDUR_COPY_TX_DATA
                          (
                            CANTP_CFG_GET_TXNSDU_CONFIG(ChannelPtr->NSduId).IPduConfId,
                            PduInfoPtr,
                            NULL_PTR,
                            &UL_BufferSize
                          );
  if((Ret_CopyTxData == BUFREQ_OK) || (Ret_CopyTxData == BUFREQ_E_BUSY))
  {
    ChannelPtr->ActualBufferSize = UL_BufferSize;
  }

  return Ret_CopyTxData;
}

#if(CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON)
/** \brief Get the Rx channel index for the given TxFc PDU ID */
FUNC(Std_ReturnType, CANTP_CODE) CanTp_GetChannelOfPostponedTxFcPdu
(
  PduIdType CanTpTxPduId,
  P2VAR(CanTp_ChType, AUTOMATIC, AUTOMATIC) ChannelIdxPtr
)
{
  Std_ReturnType Result = E_NOT_OK;
  CanTp_ChType idx;

  for(idx = CANTP_MAX_TX_CHANNELS; idx < CANTP_MAX_PARALLEL_CHANNELS; idx++)
  {
    P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) parallelChPtr = &CanTp_Channel[idx];

    /* Check if locked channels, were already locked for the current RxNSduId */
    if(CANTP_CFG_GET_RXNSDU_CONFIG(parallelChPtr->NSduId).NPduConfIdFc == CanTpTxPduId)
    {
      CanTp_ChStateType CurrentState;

      TS_AtomicAssign8(CurrentState, parallelChPtr->State);

      /* Check if current TxFcPdu transmission was postponed or TxFcPdu confirmation is awaited */
      if((CANTP_CH_GET_ACTION(CurrentState) == CANTP_CH_ACTION_POSTPONE_FRAME) ||
        (CANTP_CH_GET_ACTION(CurrentState) == CANTP_CH_ACTION_CONFIRM))
      {
        /* read the channel if allowed */
        *ChannelIdxPtr = idx;
        Result = E_OK;
        break;
      }
    }
  }
  return Result;
}
#endif /* CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON */

#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */

#if(TS_MERGED_COMPILE == STD_OFF)
#define CANTP_STOP_SEC_CODE
#include <CanTp_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */
/*==================[end of file]===========================================*/
