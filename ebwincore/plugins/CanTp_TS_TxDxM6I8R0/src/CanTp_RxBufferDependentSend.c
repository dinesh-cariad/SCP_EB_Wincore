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

/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true
 *
 * Reason: False-positive from the Tasking v5.0r2 compiler (TCVX-41885)
 */

/*==================[inclusions]============================================*/

#include <CanTp_Trace.h>
#include <TSAutosar.h>         /* EB specific standard types */
#include <ComStack_Types.h>    /* typedefs for AUTOSAR com stack */
#include <CanTp_InternalLib.h> /* internal macros and variables */
#include <CanTp_Internal.h>    /* internal macros and variables */

/* do not indirectly include PduR_SymbolicNames_PBcfg.h via PduR_CanTp.h */
#define PDUR_NO_PBCFG_REQUIRED
#include <PduR_CanTp.h>        /* PduR module API */
#include <TSMem.h>             /* TS_MemCpy */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

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


FUNC(Std_ReturnType, CANTP_CODE) CanTp_ProcessStartOfReception
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
)
{
  Std_ReturnType Ret;
  BufReq_ReturnType Ret_StartOfReception;


  /* CANTP079v4.0.0 Inform upper layer about Rx frame*/
  Ret_StartOfReception = CANTP_PDUR_START_OF_RECEPTION
    (
      CANTP_CFG_GET_RXNSDU_CONFIG(ChannelPtr->NSduId).IPduId,
      ChannelPtr->TotalLength,
      &ChannelPtr->ActualBufferSize
    );

  switch(Ret_StartOfReception)
  {
    case BUFREQ_OK:
    {
      /* Reception started */
      Ret = E_OK;
      break;
    }
    case BUFREQ_E_NOT_OK:
    {
      /* Stop channel and don't inform upper layer (send no flow control
         CanTp.ASR40.CANTP081).
       */
      CanTp_StopChannel(
                         ChannelPtr,
                         CANTP_ULIND_NONE,
                         NTFRSLT_E_NOT_OK
                       );
      Ret = E_NOT_OK;
      break;
    }
    case BUFREQ_E_OVFL:
    {

      CanTp_ChStateType ChannelState;
      TS_AtomicAssign8(ChannelState, ChannelPtr->State);

      /* FC shall only be sent for FF and not for SF */
      if(CANTP_CH_RX_BUFREQ_FF == ChannelState)
      {
        CanTp_TryToSendFC(ChannelPtr, CANTP_CH_FRAME_FC_OVFLW);
      }
      else
      {
        /* Stop channel and don't inform upper layer (send no flow control for SF) */
        CanTp_StopChannel(
                           ChannelPtr,
                           CANTP_ULIND_NONE,
                           NTFRSLT_E_NOT_OK
                         );
      }

      Ret = E_NOT_OK;
      break;
    }
    case BUFREQ_E_BUSY:
    {
      /* Wait for free buffer */
      /* BUFREQ_E_BUSY will be removed in future (see RFC52007)*/
      /* The value of upper layer buffer size is undefined for this return value */
      ChannelPtr->ActualBufferSize = 0U;
      Ret = E_OK;
      break;
    }
    default:
    {
      /* unknown return value -> Abort reception */
      CanTp_StopChannel(ChannelPtr, CANTP_ULIND_STATUS, NTFRSLT_E_NOT_OK);
      Ret = E_NOT_OK;
      break;
    }
  }

  return Ret;
}

FUNC(BufReq_ReturnType, CANTP_CODE) CanTp_ProcessStartOfReceptionData
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr,
  P2VAR(uint8, AUTOMATIC, CANTP_VAR) NPduDataPtr,
  uint8 NSduDataLength
)
{
  BufReq_ReturnType Ret_ReqRxBuffer;

  if(NSduDataLength == 0U)
  {
    /* All data are already copied but bigger buffer required. */
    Ret_ReqRxBuffer = CanTp_RxProcessData(ChannelPtr, NULL_PTR, 0U);
  }
  else
  {
    if(ChannelPtr->ActualBufferSize < NSduDataLength)
    {
      /* Actual buffer is smaller as received data. Request bigger buffer. */
      Ret_ReqRxBuffer = CanTp_RxProcessData(ChannelPtr, NULL_PTR, 0U);
    }
    else
    {
      Ret_ReqRxBuffer = BUFREQ_OK; /* Enough buffer from last call available */
    }

    if(BUFREQ_OK == Ret_ReqRxBuffer)
    {
      if(ChannelPtr->ActualBufferSize >= NSduDataLength)
      {
        Ret_ReqRxBuffer = CanTp_RxProcessData(ChannelPtr, NPduDataPtr, NSduDataLength);
      }
      else
      {
        /* Not enough buffer available. Set return value to BUFREQ_E_BUSY to indicate
           that no data have been copied.
         */
        Ret_ReqRxBuffer = BUFREQ_E_BUSY;
      }
    }
  }

  return Ret_ReqRxBuffer;
}

FUNC(BufReq_ReturnType, CANTP_CODE) CanTp_RxProcessData
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr,
  P2VAR(uint8, AUTOMATIC, CANTP_VAR) NPduDataPtr,
  uint8 NSduDataLength
)
{
  BufReq_ReturnType Ret_CopyRxData;
  PduLengthType UL_BufferSize;
  PduInfoType PduInfo; /* info structure for next buffer */

  PduInfo.SduLength = NSduDataLength;
  PduInfo.SduDataPtr = NPduDataPtr;

  Ret_CopyRxData = CANTP_PDUR_COPY_RX_DATA
    (
      CANTP_CFG_GET_RXNSDU_CONFIG(ChannelPtr->NSduId).IPduId,
      &PduInfo,
      &UL_BufferSize
    );

  if(Ret_CopyRxData == BUFREQ_OK)
  {
    ChannelPtr->ActualBufferSize = UL_BufferSize;
  }
  else
  {
    /* The value of upper layer buffer size is undefined for this return value. */
    ChannelPtr->ActualBufferSize = 0U;
  }

  return Ret_CopyRxData;
}

#if(CANTP_STALLHANDLING_SUPPORT == STD_ON)
FUNC(void, CANTP_CODE) CanTp_RxStateHandling
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
)
{

  CanTp_ChStateType ChannelState;

  TS_AtomicAssign8(ChannelState, ChannelPtr->State);

  switch(ChannelState)
  {
    case CANTP_CH_RX_BUFREQ_SF:
    {
      BufReq_ReturnType Ret_ReqRxBuffer;

      /* lock channel */
      CANTP_LOCK_CHANNEL(ChannelPtr);

      Ret_ReqRxBuffer = CanTp_ProcessStartOfReceptionData
                                 (
                                   ChannelPtr,
                                   ChannelPtr->CanIfData,
                                   ChannelPtr->CanIfDLC
                                 );
      switch(Ret_ReqRxBuffer)
      {
        case BUFREQ_OK:
        {
          /* single frame reception completed. The whole message was copied to
             upper layer successfully.
           */
          CanTp_StopChannel(ChannelPtr, CANTP_ULIND_STATUS, NTFRSLT_OK);

          break;
        }
        case BUFREQ_E_BUSY:
        {
          /* Wait until buffer is available or timeout occurs.*/
          break;
        }
        default:
        {
          /* Unexpected return value. Close channel. */
          CanTp_StopChannel(ChannelPtr, CANTP_ULIND_STATUS, NTFRSLT_E_NOT_OK);
          break;
        }
      }
      /* unlock channel */
      CANTP_UNLOCK_CHANNEL(ChannelPtr);

      break;
    }
    case CANTP_CH_RX_BUFREQ_FF:
    {
      BufReq_ReturnType Ret_ReqRxBuffer;

      /* lock channel */
      CANTP_LOCK_CHANNEL(ChannelPtr);

      Ret_ReqRxBuffer = CanTp_ProcessStartOfReceptionData
                                 (
                                   ChannelPtr,
                                   ChannelPtr->CanIfData,
                                   ChannelPtr->CanIfDLC
                                 );
      switch(Ret_ReqRxBuffer)
      {
        case BUFREQ_OK:
        {
          /* Data are copied to UL if not already done with CanTP_RxIndication().
             In this case ChannelPtr->CanIfDLC was already zero.
           */
          Std_ReturnType BSCalc_Ok; /* Indication of the actual BS calculation */

          ChannelPtr->ProcessedLength += (uint16) ChannelPtr->CanIfDLC;
          ChannelPtr->CanIfDLC = 0U;

          /* Only calculate actual block size if FF data already processed. */
          BSCalc_Ok = CanTp_CalculateBlockSize(ChannelPtr, &(ChannelPtr->BlockSize));

          if(BSCalc_Ok == E_OK)
          {
            CanTp_TryToSendFC(ChannelPtr, CANTP_CH_FRAME_FC_CTS);
          }
          else
          {
            /* Send wait frame if timeout elapsed */
            CanTp_CheckAndHandleWaitFrameTimeOut(ChannelPtr);
          }
          break;
        }
        case BUFREQ_E_BUSY:
        {
          /* Wait until buffer is available or timeout occurs.*/
          /* Send wait frame if timeout elapsed */
          CanTp_CheckAndHandleWaitFrameTimeOut(ChannelPtr);
          break;
        }
        default:
        {
          /* Unexpected return value. Close channel. */
          CanTp_StopChannel(ChannelPtr, CANTP_ULIND_STATUS, NTFRSLT_E_NOT_OK);
          break;
        }
      }
      /* unlock channel */
      CANTP_UNLOCK_CHANNEL(ChannelPtr);

      break;
    }
    case CANTP_CH_RX_BUFREQ_CF:
    {
      /* This state the channel waits for enough buffer to request the next block. */
      BufReq_ReturnType Ret_ReqRxBuffer;

      /* lock channel */
      CANTP_LOCK_CHANNEL(ChannelPtr);

      Ret_ReqRxBuffer = CanTp_RxProcessData(ChannelPtr, NULL_PTR, 0U);

      switch(Ret_ReqRxBuffer)
      {
        case BUFREQ_OK:
        {
          boolean ContinueWithNextBlock;

          ContinueWithNextBlock = CanTp_VerifyBlockSize(ChannelPtr);

          if(ContinueWithNextBlock)
          {
            CanTp_TryToSendFC(ChannelPtr, CANTP_CH_FRAME_FC_CTS);
          }
          else
          {
            /* Send wait frame if timeout elapsed */
            CanTp_CheckAndHandleWaitFrameTimeOut(ChannelPtr);
          }
          break;
        }
        case BUFREQ_E_BUSY:
        {
          /* Wait until buffer is available or timeout occurs.*/
          /* Send wait frame if timeout elapsed */
          CanTp_CheckAndHandleWaitFrameTimeOut(ChannelPtr);
          break;
        }
        default:
        {
          /* Unexpected return value. Close channel. */
          CanTp_StopChannel(ChannelPtr, CANTP_ULIND_STATUS, NTFRSLT_E_NOT_OK);
          break;
        }
      }
      /* unlock channel */
      CANTP_UNLOCK_CHANNEL(ChannelPtr);

      break;
    }
    case CANTP_CH_RX_CONFIRM_FC_CTS:
    case CANTP_CH_RX_CONFIRM_FC_OVFLW:
    case CANTP_CH_RX_CONFIRM_FC_WT:
    {
      /* Wait for confirm. CanTp_TxConfirmation() or timeout will take care of
         changing state. */
      break;
    }
    case CANTP_CH_RX_RECEIVE_CF:
    {
      /* wait for next CF or timeout */
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
#endif

#if(CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON)
/** \brief Get the Tx channel index for the given Tx PDU ID */
FUNC(Std_ReturnType, CANTP_CODE) CanTp_GetChannelOfPostponedTxPdu
(
  PduIdType CanTpTxPduId,
  P2VAR(CanTp_TxChannelType, AUTOMATIC, AUTOMATIC) ChannelIdxPtr
)
{
  Std_ReturnType Result = E_NOT_OK;
  CanTp_TxChannelType idx;

  /* Deviation TASKING-1 */
  for(idx = 0U; idx < CANTP_MAX_TX_CHANNELS; idx++)
  {
    P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) parallelChPtr = &CanTp_Channel[idx];

    /* Check if locked channels, were already locked for the current TxNSduId */
    if(CANTP_CFG_GET_TXNSDU_CONFIG(parallelChPtr->NSduId).NPduConfIdxF == CanTpTxPduId)
    {
      CanTp_ChStateType CurrentState;
      TS_AtomicAssign8(CurrentState, parallelChPtr->State);

      /* Check if current TxPdu transmission was postponed or TxPdu confirmation is awaited */
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
#endif/* CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON */

FUNC(void, CANTP_CODE) CanTp_TryToSendFC
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr,
  uint8 Type
)
{
  CanTp_TxChannelType Tx_ChannelID = INVALID_TX_CHANNEL_ID;
  boolean Ret_Postponed = FALSE;

#if(CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON)
  PduIdType PduId = CANTP_CFG_GET_RXNSDU_CONFIG(ChannelPtr->NSduId).NPduConfIdFc;
  /* Get the corresponding Tx Channel */
  if(E_OK == CanTp_GetChannelOfPostponedTxPdu(PduId, &Tx_ChannelID))
  {
    CANTP_IS_TX_CH_CONFIRM_ACTION(Tx_ChannelID, TRUE, Ret_Postponed);
  }
#else
  Tx_ChannelID = CANTP_CFG_GET_RXNSDU_CONFIG(ChannelPtr->NSduId).Tx_Channel;
  CANTP_IS_TX_CH_CONFIRM_ACTION(Tx_ChannelID, TRUE, Ret_Postponed);
#endif/* CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON */

  switch(Type)
  {
    case CANTP_CH_FRAME_FC_CTS:
    {
      /* Check if a transmission is ongoing on related Tx_ChannelID channel*/
      if( Ret_Postponed == TRUE)
      {
        DBG_CANTP_CHANNEL_STATE_GRP(
                                    DBG_CANTP_GET_CHANNEL_INDEX(ChannelPtr),
                                    ChannelPtr->State,
                                    CANTP_CH_RX_POSTPONED_FC_CTS
                                   );
        TS_AtomicAssign8(ChannelPtr->State, CANTP_CH_RX_POSTPONED_FC_CTS);
      }
      else
      {
        DBG_CANTP_CHANNEL_STATE_GRP(
                                    DBG_CANTP_GET_CHANNEL_INDEX(ChannelPtr),
                                    ChannelPtr->State,
                                    CANTP_CH_RX_TRANSMIT_FC_CTS
                                   );
        /* atomic assign */
        TS_AtomicAssign8(ChannelPtr->State, CANTP_CH_RX_TRANSMIT_FC_CTS);

        /* CanTp_TxConfirmation will set state to CANTP_CH_RX_RECEIVE_CF */
        CanTp_PrepareFC(ChannelPtr, CANTP_N_PCI_FC_CTS);
      }
      break;
    }
    case CANTP_CH_FRAME_FC_WT:
    {
      /* Check if a transmission is ongoing on related Tx_ChannelID channel*/
      if( Ret_Postponed == TRUE)
      {
        DBG_CANTP_CHANNEL_STATE_GRP(
                                    DBG_CANTP_GET_CHANNEL_INDEX(ChannelPtr),
                                    ChannelPtr->State,
                                    CANTP_CH_RX_POSTPONED_FC_WT
                                   );
        TS_AtomicAssign8(ChannelPtr->State, CANTP_CH_RX_POSTPONED_FC_WT);
      }
      else
      {
        DBG_CANTP_CHANNEL_STATE_GRP(
                                    DBG_CANTP_GET_CHANNEL_INDEX(ChannelPtr),
                                    ChannelPtr->State,
                                    CANTP_CH_RX_TRANSMIT_FC_WT
                                   );
        /* set new state, timeout remains */
        TS_AtomicAssign8(ChannelPtr->State, CANTP_CH_RX_TRANSMIT_FC_WT);

        CanTp_PrepareFC(ChannelPtr, CANTP_N_PCI_FC_WT);
      }
      break;
    }
    default:
    {
      /* case CANTP_CH_FRAME_FC_OVFLW */
      ChannelPtr->ActualBufferSize = 0U;

      /* Check if a transmission is ongoing on related Tx_ChannelID channel*/
      if( Ret_Postponed == TRUE)
      {
        DBG_CANTP_CHANNEL_STATE_GRP(
                                    DBG_CANTP_GET_CHANNEL_INDEX(ChannelPtr),
                                    ChannelPtr->State,
                                    CANTP_CH_RX_POSTPONED_FC_OVFLW
                                   );
        TS_AtomicAssign8(ChannelPtr->State, CANTP_CH_RX_POSTPONED_FC_OVFLW);
      }
      else
      {
      DBG_CANTP_CHANNEL_STATE_GRP(
                                  DBG_CANTP_GET_CHANNEL_INDEX(ChannelPtr),
                                  ChannelPtr->State,
                                  CANTP_CH_RX_TRANSMIT_FC_OVFLW
                                 );
      /* atomic assign */
      TS_AtomicAssign8(ChannelPtr->State, CANTP_CH_RX_TRANSMIT_FC_OVFLW);

      CanTp_PrepareFC(ChannelPtr, CANTP_N_PCI_FC_OVFLW);
      /* channel will be closed automatically after FC frame is sent correctly
         or timeout.
       */
      }
      break;
    }
  }
}
#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */

#if(TS_MERGED_COMPILE == STD_OFF)
#define CANTP_STOP_SEC_CODE
#include <CanTp_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
