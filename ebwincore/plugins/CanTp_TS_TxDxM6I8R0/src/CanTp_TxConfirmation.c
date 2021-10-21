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

/*==================[inclusions]=============================================*/

#include <CanTp_Trace.h>
#include <TSAutosar.h>         /* EB specific standard types */
#include <ComStack_Types.h>    /* typedefs for AUTOSAR com stack */
#include <CanTp_Cbk.h>         /* callback definition of this module */
#include <CanTp_Internal.h>    /* internal macros and variables */
#include <CanTp_InternalCfg.h> /* CanTp internal configuration */
#if(CANTP_JUMPTABLE_SUPPORT == STD_ON)
#include <CanTp_EntryTable.h>  /* CanTp entry jumptable */
#endif /* CANTP_JUMPTABLE_SUPPORT == STD_ON */

#if(CANTP_USE_GPT == STD_ON)
#include <Gpt.h>               /* Gpt module types and API */
#endif

#if(CANTP_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>
#endif

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/
#if(TS_MERGED_COMPILE == STD_OFF)
#define CANTP_START_SEC_CODE
#include <CanTp_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)

#if(CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON)
/** \brief Get the channel index for the given Tx PDU ID
 **
 ** This function returns the channel index for the Tx PDU ID \p CanTpTxPduId.
 **
 ** \param[in] CanTpTxPduId, N-PDU Id of transmitted N-PDU
 ** \param[out] ChannelIdxPtr Address to write the channel index to
 **
 ** \return Result of the function
 ** \retval E_OK The channel was found and written to the address
 ** \retval E_NOT_OK No channel was found
 */
STATIC FUNC(Std_ReturnType, CANTP_CODE) CanTp_GetTxPduChannel
(
  PduIdType CanTpTxPduId,
  P2VAR(CanTp_TxChannelType, AUTOMATIC, AUTOMATIC) ChannelIdxPtr
);
#endif /* CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON */

/** \brief CanTp_ProcessConfirmedCF()
 **
 ** This function handles a confirmation of an CF from CanIf
 **
 ** Preconditions:
 ** - channel must be in state CANTP_CH_TX_CONFIRM_CF and reserved
 ** - confirmed PDU must match expected PDU (no check again)
 **
 ** \param[in] ChannelPtr  pointer to channel data structure in RAM
 */
STATIC FUNC(void, CANTP_CODE) CanTp_ProcessConfirmedCF
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
);

/** \brief CanTp_ProcessTxPdu()
 **
 ** This function handles different channel states
 **
 ** \param[in] ChannelPtr  pointer to channel data structure in RAM
 ** \param[in] CanTpTxNPduConfId  NPdu confirmation id
 */
STATIC FUNC(void, CANTP_CODE) CanTp_ProcessTxPdu
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr,
  PduIdType CanTpTxNPduConfId
);
#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

#if(CANTP_PROVIDE_API_FUNCTIONS == STD_ON)

/** \brief CanTp_TxConfirmation()
 **
 ** This function is called by the CanIf to confirm a successful transmit.
 ** It looks up the right channel and executes further steps depending on the
 ** state. No error will be raised if no channel could be associated.
 **
 ** \param[in] CanTpTxPduId, N-PDU Id of transmitted N-PDU
 */
FUNC(void, CANTP_CODE) CanTp_TxConfirmation
(
  PduIdType CanTpTxPduId
)
#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)
{
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr;
  CanTp_ChType  Channel;       /* counter for channels */
  boolean TxConfirmFlag = FALSE;
#if(CANTP_DEV_ERROR_DETECT == STD_ON)
  boolean PduHandled = TRUE;   /* flag to indicate if the PDU was handled */
#endif /* CANTP_DEV_ERROR_DETECT == STD_ON */

  DBG_CANTP_TXCONFIRMATION_ENTRY(CanTpTxPduId);

#if(CANTP_DEV_ERROR_DETECT == STD_ON)
  /* check CanTp state - possible DET and return on error */
  if(CanTp_InitVars.State != CANTP_ON)
  {
    CANTP_DET_REPORT_ERROR(
        CANTP_MODULE_ID,
        CANTP_INSTANCE_ID,
        CANTP_API_TXCONFIRMATION,
        CANTP_E_UNINIT
    );
  }
  else
#endif
  {
    if(CanTpTxPduId < CanTp_CfgPtr->NumberTxConfNPdus)
    {
      /* get the channel to which the Tx PDU ID belongs */
#if(CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON)
      if(CanTp_GetTxPduChannel(CanTpTxPduId, &Channel) == E_OK)
#else
      Channel = CANTP_CFG_GET_TXNPDU_TPCHANNEL(CanTpTxPduId);   /*get Tx channel*/

      if (Channel != INVALID_TX_CHANNEL_ID)
      {
        CanTp_ChType  ChannelLcl;
        CanTp_ChStateType ChannelState;
        P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtrLcl;

        ChannelLcl = CANTP_CFG_GET_TXFCNPDU_TPCHANNEL(CanTpTxPduId);
        if(ChannelLcl != INVALID_RX_CHANNEL_ID)
        {
          ChannelPtrLcl = &CanTp_Channel[CANTP_CFG_GET_TXFCNPDU_TPCHANNEL(CanTpTxPduId)];
          TS_AtomicAssign8(ChannelState, ChannelPtrLcl->State);

          switch(ChannelState)
          {
            case CANTP_CH_RX_CONFIRM_FC_CTS:
            case CANTP_CH_RX_CONFIRM_FC_WT:
            case CANTP_CH_RX_CONFIRM_FC_OVFLW:
            {
              /* a FC confirmation is awaited and the Rx channel should be taken*/
              Channel = CANTP_CFG_GET_TXFCNPDU_TPCHANNEL(CanTpTxPduId);
              break;
            }
            default:
            {
              /* Nothing to be done. */
              break;
            }
          }
        }
      }
      else
      {
        /* a FC confirmation is awaited and the Rx channel should be taken*/
        Channel = CANTP_CFG_GET_TXFCNPDU_TPCHANNEL(CanTpTxPduId);
      }
#endif /* CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON */

      {
        ChannelPtr = &CanTp_Channel[Channel];

        CANTP_ENTER_CRITICAL_SECTION();

        /* lock channel if possible */
        if(CANTP_IS_CHANNEL_LOCKED(ChannelPtr))
        {
          CanTp_ChStateType ChannelState;
          TS_AtomicAssign8(ChannelState, ChannelPtr->State);

          /* memorize confirmation if this channel would await a confirmation */
          if(CANTP_CH_GET_ACTION(ChannelState) == CANTP_CH_ACTION_CONFIRM)
          {
            CANTP_SET_FLAG(ChannelPtr->PostFlag,CANTP_POST_TX_CONF_MASK);
            ChannelPtr->PostTxConfNPduId = CanTpTxPduId;
          }
        }
        else
        {
          CANTP_LOCK_CHANNEL(ChannelPtr);
          /* Setting TxConfirmFlag to get the channel to which the PDU ID belongs */
          TxConfirmFlag = TRUE;
        }

        CANTP_LEAVE_CRITICAL_SECTION();
        /* Checking TxConfirmFlag to get the channel to which the PDU ID belongs */
        if(TxConfirmFlag == (boolean)TRUE)
        {
          CanTp_ProcessTxPdu(ChannelPtr, CanTpTxPduId);

          CanTp_HandlePostponedFlags(ChannelPtr);

          /* unlock channel */
          CANTP_UNLOCK_CHANNEL(ChannelPtr);
        }
      }
    }
#if(CANTP_DEV_ERROR_DETECT == STD_ON)
    else
    {
      /* no channel for the Tx PDU ID */
      PduHandled = FALSE;
    }
#endif /* CANTP_DEV_ERROR_DETECT == STD_ON */

#if(CANTP_DEV_ERROR_DETECT == STD_ON)
    if(PduHandled == (boolean)FALSE)
    {
      /* N-PDU id not found in configuration -> signal DET */
      CANTP_DET_REPORT_ERROR(
          CANTP_MODULE_ID,
          CANTP_INSTANCE_ID,
          CANTP_API_TXCONFIRMATION,
          CANTP_E_INVALID_TX_ID
      );
    }
#endif /* CANTP_DEV_ERROR_DETECT == STD_ON */
}

  DBG_CANTP_TXCONFIRMATION_EXIT(CanTpTxPduId);

} /* CanTp_TxConfirmation() */
#else /* => CANTP_JUMPTABLE_MODE == CANTP_JUMPTABLE_CLIENT */
{

  DBG_CANTP_TXCONFIRMATION_ENTRY(CanTpTxPduId);
  (CanTp_EntryTablePtr->TxConfirmation)(CanTpTxPduId);

  DBG_CANTP_TXCONFIRMATION_EXIT(CanTpTxPduId);
}
#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */

#endif /* CANTP_PROVIDE_API_FUNCTIONS == STD_ON */

#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)
FUNC(void, CANTP_CODE) CanTp_PostTxConfirmation
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
)
{
  PduIdType CanTpTxNPduConfId; /* NPdu confirmation id */
  boolean PendingTxConf = FALSE; /* indicates pending Tx confirmation */

  /* Initialization required to avoid "usage of uninitialized variables" warning. */
  CanTpTxNPduConfId = 0U;

  CANTP_ENTER_CRITICAL_SECTION();

  if((CANTP_CHECK_FLAG(ChannelPtr->PostFlag,CANTP_POST_TX_CONF_MASK)) != (boolean)FALSE)
  {
    CanTpTxNPduConfId = ChannelPtr->PostTxConfNPduId;
    CANTP_CLEAR_FLAG(ChannelPtr->PostFlag,CANTP_POST_TX_CONF_MASK);
    PendingTxConf = TRUE;
  }
  CANTP_LEAVE_CRITICAL_SECTION();

  if(PendingTxConf)
  {
    CanTp_ProcessTxPdu(ChannelPtr, CanTpTxNPduConfId);
  }
}
#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */

/*==================[internal function definitions]==========================*/

#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)

STATIC FUNC(void, CANTP_CODE) CanTp_ProcessTxPdu
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr,
  PduIdType CanTpTxNPduConfId
)
{
  PduIdType  SearchTxNPduId;  /* PduId to search (depends on direction) */
  CanTp_ChStateType ChannelState; /* Channel State */
  const PduIdType  NSduId = ChannelPtr->NSduId;

  CanTp_TxChannelType Tx_ChannelID = INVALID_TX_CHANNEL_ID;
  CanTp_ChType Rx_ChannelID = INVALID_RX_CHANNEL_ID;

  TS_AtomicAssign8(ChannelState, ChannelPtr->State);

  if(CANTP_CH_GET_MODE(ChannelState) == CANTP_CH_MODE_RX)
  {
    SearchTxNPduId = CANTP_CFG_GET_RXNSDU_CONFIG(NSduId).NPduConfIdFc;

#if(CANTP_PARALLEL_CHANNELS_ENABLED == STD_OFF)
    Tx_ChannelID = CANTP_CFG_GET_RXNSDU_CONFIG(ChannelPtr->NSduId).Tx_Channel;
#endif
  }
  else
  {
    SearchTxNPduId = CANTP_CFG_GET_TXNSDU_CONFIG(NSduId).NPduConfIdxF;

#if(CANTP_PARALLEL_CHANNELS_ENABLED == STD_OFF)
    Rx_ChannelID = CANTP_CFG_GET_TXNSDU_CONFIG(ChannelPtr->NSduId).Rx_Channel;
#endif
  }

  /* Check if the confirmation is for the NSdu of this channel. If not ignore confirmation. */
  if(CanTpTxNPduConfId == SearchTxNPduId)
  {
    TS_AtomicAssign8(ChannelState, ChannelPtr->State);

    /* switch channel state */
    switch(ChannelState)
    {
    case CANTP_CH_TX_CONFIRM_SF:
    {
      /* that was a successful SF transmission */
      CanTp_StopChannel(ChannelPtr, CANTP_ULIND_STATUS, NTFRSLT_OK);
#if(CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON)
      /* Get the corresponding Rx Channel */
      if(E_OK == CanTp_GetChannelOfPostponedTxFcPdu(SearchTxNPduId, &Rx_ChannelID))
#endif /* CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON */
      {
        CanTp_TransmitPostponedFlowControl(Rx_ChannelID);
      }
      break;
    }
    case CANTP_CH_TX_CONFIRM_FF:
    {
      /* Check if the FC has not been yet received. */
      if ((CANTP_CHECK_FLAG(ChannelPtr->ConnectionFlags,CANTP_FCRECEIVED_MASK)) == (boolean)FALSE)
      {
        DBG_CANTP_CHANNEL_STATE_GRP(
            DBG_CANTP_GET_CHANNEL_INDEX(ChannelPtr),
            ChannelPtr->State,
            CANTP_CH_TX_RECEIVE_FC
        );

        /* well done for the first frame - lets wait for FC */
        TS_AtomicAssign8(ChannelPtr->State, CANTP_CH_TX_RECEIVE_FC);

        ChannelPtr->Ticks = CANTP_GET_TX_PDU_N_BS(&CANTP_CFG_GET_TXNSDU_CONFIG(NSduId));
      }
      else
      {
        DBG_CANTP_CHANNEL_STATE_GRP(
            DBG_CANTP_GET_CHANNEL_INDEX(ChannelPtr),
            ChannelPtr->State,
            CANTP_CH_TX_BUFREQ_CF
        );

        /* FC already received, set the state for buffer request to be processed in the next main-function. */
        TS_AtomicAssign8(ChannelPtr->State, CANTP_CH_TX_BUFREQ_CF);

        /* Reset the FC flag. */
        CANTP_CLEAR_FLAG(ChannelPtr->ConnectionFlags,CANTP_FCRECEIVED_MASK);
      }
#if(CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON)
      /* Get the corresponding Rx Channel */
      if(E_OK == CanTp_GetChannelOfPostponedTxFcPdu(SearchTxNPduId, &Rx_ChannelID))
#endif /* CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON */
      {
        CanTp_TransmitPostponedFlowControl(Rx_ChannelID);
      }
      break;
    }
    case CANTP_CH_TX_CONFIRM_CF:
    {
      CanTp_ProcessConfirmedCF(ChannelPtr);

#if(CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON)
      /* Get the corresponding Rx Channel */
      if(E_OK == CanTp_GetChannelOfPostponedTxFcPdu(SearchTxNPduId, &Rx_ChannelID))
#endif /* CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON */
      {
        CanTp_TransmitPostponedFlowControl(Rx_ChannelID);
      }
      break;
    }
    case CANTP_CH_RX_CONFIRM_FC_CTS:
    {
      DBG_CANTP_CHANNEL_STATE_GRP(
          DBG_CANTP_GET_CHANNEL_INDEX(ChannelPtr),
          ChannelPtr->State,
          CANTP_CH_RX_RECEIVE_CF
      );

      /* ok - FC cts was send successfully */
      TS_AtomicAssign8(ChannelPtr->State, CANTP_CH_RX_RECEIVE_CF);

      ChannelPtr->Ticks = CANTP_GET_RX_PDU_N_CR(&CANTP_CFG_GET_RXNSDU_CONFIG(NSduId));

#if(CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON)
      /* Get the corresponding Tx Channel */
      if(E_OK == CanTp_GetChannelOfPostponedTxPdu(SearchTxNPduId, &Tx_ChannelID))
#endif /* CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON */
      {
        /* Check if there is a Postponed frame on the Tx Channel */
        CanTp_TransmitPostponedFrame(Tx_ChannelID);
      }
      break;
    }
    case CANTP_CH_RX_CONFIRM_FC_WT:
    {
      /* FC(WT) was send successfully */

      /* Pointer  to N-Sdu config */
      CONSTP2CONST(CanTp_RxNSduConfigType, AUTOMATIC, CANTP_APPL_CONST) RxNSduCfgPtr =
          &CANTP_CFG_GET_RXNSDU_CONFIG(ChannelPtr->NSduId);
      /* Calculate the number of data bytes in the first frame
           (depends on the addressing format)
       */
      const uint8 DataLength_FF = RxNSduCfgPtr->NPduMaxLength
          - CANTP_GET_FIRST_N_PCI_BYTE(RxNSduCfgPtr)
          - CANTP_FF_NPCI_SIZE;

      ChannelPtr->WftCounter--;

#if(CANTP_N_BR_WAIT_SUPPORT == STD_ON)
      /* check if the previous FC with status WAIT was sent */
      if((CANTP_CHECK_FLAG(ChannelPtr->ConnectionFlags,CANTP_FCWAIT_REPEATED_MASK)) != (boolean)FALSE)
      {
        /* load the N_Br_WT timer */
        ChannelPtr->Ticks = CANTP_CFG_GET_RXNSDU_CONFIG(NSduId).N_Br_WT;
      }
      else
#endif /* (CANTP_N_BR_WAIT_SUPPORT == STD_ON) */
      {
        ChannelPtr->Ticks = CANTP_GET_RX_PDU_N_BR(&CANTP_CFG_GET_RXNSDU_CONFIG(NSduId));
      }

      if(ChannelPtr->WftCounter != 0U)
      {
        /* more wait frames allowed - timeout is N_Br+N_Ar */
        ChannelPtr->Ticks +=
            CANTP_GET_RX_PDU_N_AR(&CANTP_CFG_GET_RXNSDU_CONFIG(NSduId));
      }
      if(ChannelPtr->ProcessedLength > DataLength_FF)
      {
        DBG_CANTP_CHANNEL_STATE_GRP(
            DBG_CANTP_GET_CHANNEL_INDEX(ChannelPtr),
            ChannelPtr->State,
            CANTP_CH_RX_BUFREQ_CF
        );

        /* The FC(WT) was send after last CF of block was received. */
            TS_AtomicAssign8(ChannelPtr->State, CANTP_CH_RX_BUFREQ_CF);
      }
      else
      {
        DBG_CANTP_CHANNEL_STATE_GRP(
            DBG_CANTP_GET_CHANNEL_INDEX(ChannelPtr),
            ChannelPtr->State,
            CANTP_CH_RX_BUFREQ_FF
        );

        /* The FC(WT) was send after FF was received. */
            TS_AtomicAssign8(ChannelPtr->State, CANTP_CH_RX_BUFREQ_FF);
      }

#if(CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON)
      /* Get the corresponding Tx Channel */
      if(E_OK == CanTp_GetChannelOfPostponedTxPdu(SearchTxNPduId, &Tx_ChannelID))
#endif /* CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON */
      {
        /* Check if there is a Postponed frame on the Tx Channel */
        CanTp_TransmitPostponedFrame(Tx_ChannelID);
      }
      break;
    }
    case CANTP_CH_RX_CONFIRM_FC_OVFLW:
    {
      /* Abort reception don't inform Upper layer and send FC(OVFL)
           (see CanTp.ASR40.CANTP318).
       */
      CanTp_StopChannel(ChannelPtr, CANTP_ULIND_NONE, NTFRSLT_E_NOT_OK);

#if(CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON)
      /* Get the corresponding Tx Channel */
      if(E_OK == CanTp_GetChannelOfPostponedTxPdu(SearchTxNPduId, &Tx_ChannelID))
#endif /* CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON */
      {
        /* Check if there is a Postponed frame on the Tx Channel */
        CanTp_TransmitPostponedFrame(Tx_ChannelID);
      }
      break;
    }
    default:
    {
      /* Ignore all confirms when in unexpected states */
      break;
    }
    } /* switch */
  }
}

#if(CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON)
/** \brief Get the channel index for the given Tx PDU ID */
STATIC FUNC(Std_ReturnType, CANTP_CODE) CanTp_GetTxPduChannel
(
  PduIdType CanTpTxPduId,
  P2VAR(CanTp_TxChannelType, AUTOMATIC, AUTOMATIC) ChannelIdxPtr
)
{
  Std_ReturnType Result = E_NOT_OK;
  CanTp_TxChannelType idx;

  for(idx = 0; idx < CANTP_MAX_PARALLEL_CHANNELS; idx++)
  {
    P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) parallelChPtr = &CanTp_Channel[idx];
    CanTp_ChStateType CurrentState;
    boolean ChannelFound = FALSE;

    TS_AtomicAssign8(CurrentState, parallelChPtr->State);

    switch(CurrentState)
    {
    case CANTP_CH_TX_CONFIRM_SF:
    case CANTP_CH_TX_CONFIRM_FF:
    case CANTP_CH_TX_CONFIRM_CF:
    {
      /* Check if locked channels, were already locked for the current TxNSduId, when TxConfirmation arrives */
      if(CANTP_CFG_GET_TXNSDU_CONFIG(parallelChPtr->NSduId).NPduConfIdxF == CanTpTxPduId)
      {
        /* read the channel if allowed */
        *ChannelIdxPtr = idx;
        ChannelFound = TRUE;
        break;
      }
    }
    break;
    case CANTP_CH_RX_CONFIRM_FC_CTS:
    case CANTP_CH_RX_CONFIRM_FC_WT:
    case CANTP_CH_RX_CONFIRM_FC_OVFLW:
    {
      /* Check if locked channels, were already locked for the current TxNSduId, when TxConfirmation arrives */
      if(CANTP_CFG_GET_RXNSDU_CONFIG(parallelChPtr->NSduId).NPduConfIdFc == CanTpTxPduId)
      {
        /* read the channel if allowed */
        *ChannelIdxPtr = idx;
        ChannelFound = TRUE;
        break;
      }
    }
    break;
    default: /* default case intended to be empty */
      break;
    }

    if(ChannelFound == (boolean)TRUE)
    {
      Result = E_OK;
      break;
    }
  }

  return Result;
}
#endif /* CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON */


/** \brief CanTp_ProcessConfirmedCF() */
STATIC FUNC(void, CANTP_CODE) CanTp_ProcessConfirmedCF
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
)
{
  uint32 GPTticks;  /* ticks for GPT Timer */
  PduLengthType RemainingBytes; /* size of remaining channel data */
  const PduIdType NSduId = ChannelPtr->NSduId;

  RemainingBytes = ChannelPtr->TotalLength - ChannelPtr->ProcessedLength;

  /* maybe the whole message is already transmitted */
  if(RemainingBytes == 0U)
  {
    /* well done, message transmitted - lets quit */
    CanTp_StopChannel(ChannelPtr, CANTP_ULIND_STATUS, NTFRSLT_OK);
  }
  else
  {
    /* do we expect a FC frame now ? */
    if(
        (ChannelPtr->BlockSize != 0U)
        && (ChannelPtr->BlockSize == ChannelPtr->ProcessedBlockFrames)
    )
    {
      DBG_CANTP_CHANNEL_STATE_GRP(
          DBG_CANTP_GET_CHANNEL_INDEX(ChannelPtr),
          ChannelPtr->State,
          CANTP_CH_TX_RECEIVE_FC
      );

      /* lets wait for the next FC */
      TS_AtomicAssign8(ChannelPtr->State, CANTP_CH_TX_RECEIVE_FC);

      ChannelPtr->Ticks = CANTP_GET_TX_PDU_N_BS(&CANTP_CFG_GET_TXNSDU_CONFIG(NSduId));
    }
    else
    {
#if(CANTP_CHANGE_TX_PARAMETER_REQ_API == STD_ON)
      uint8 StMinValue;

      TS_AtomicAssign8(StMinValue, CanTp_TxNSduStMinValues[ChannelPtr->NSduId]);
      if(0xFFU == StMinValue)
      {
        ChannelPtr->StMin = ChannelPtr->LastStMinFC;
      }
      else
      {
        ChannelPtr->StMin = StMinValue;
      }
#endif /* (CANTP_CHANGE_TX_PARAMETER_REQ_API == STD_ON) */

      /* Reset the FC flag. */
      CANTP_CLEAR_FLAG(ChannelPtr->ConnectionFlags,CANTP_FCRECEIVED_MASK);

      /* do we have to wait StMin ? */
      if(ChannelPtr->StMin == 0U)
      {
        DBG_CANTP_CHANNEL_STATE_GRP(
            DBG_CANTP_GET_CHANNEL_INDEX(ChannelPtr),
            ChannelPtr->State,
            CANTP_CH_TX_BUFREQ_CF
        );

        /* no - send next CF immediately */
        TS_AtomicAssign8(ChannelPtr->State, CANTP_CH_TX_BUFREQ_CF);
        CanTp_TxStateHandling(ChannelPtr,FALSE);
      }
      else
      {
        const CanTp_TxChannelType Channel = CANTP_CFG_GET_TXNSDU_CONFIG(ChannelPtr->NSduId).Channel;

        if(ChannelPtr->StMin < 0x80U)
        {
          GPTticks = (uint32) ((uint32) ChannelPtr->StMin * (uint32) 1000000UL);
        }
        else
        {
          GPTticks = (uint32) (((uint32) ChannelPtr->StMin - 0xF0U) * (uint32) 100000UL);
        }
        GPTticks = (GPTticks + CANTP_CFG_GET_MFCTR_CONFIG(Channel).Resolution) - 1U;

        /* Check if this is the first CF after a FC with STmin. */
        if ((CANTP_CHECK_FLAG(ChannelPtr->ConnectionFlags,CANTP_STMIN_ROUNDUP_MASK)) != (boolean)FALSE)
        {
          /* wait StMin - calculate ticks: round_up(STmin / Resolution) + 1
          "+1" is used because the next main function tick might come
          immediately afterwards and also if the Gpt is used, it is not
          entirely sure, that the first tick lasts for the correct time.
          This is used to ensure the STmin between the first CF and second.
          The following STmins will be ensured by the mainfunction cycle. */
          GPTticks = (GPTticks / CANTP_CFG_GET_MFCTR_CONFIG(Channel).Resolution) + 1U;

          /* Reset the flag. */
          CANTP_CLEAR_FLAG(ChannelPtr->ConnectionFlags,CANTP_STMIN_ROUNDUP_MASK);
        }
        else
        {
          GPTticks = (GPTticks / CANTP_CFG_GET_MFCTR_CONFIG(Channel).Resolution);
        }

        DBG_CANTP_CHANNEL_STATE_GRP(Channel, ChannelPtr->State, CANTP_CH_TX_WAITTMR_CF);

        TS_AtomicAssign8(ChannelPtr->State, CANTP_CH_TX_WAITTMR_CF);

        ChannelPtr->Ticks = CANTP_GET_TX_PDU_N_CS(&CANTP_CFG_GET_TXNSDU_CONFIG(NSduId));

        if(CANTP_CFG_GET_MFCTR_CONFIG(Channel).CallbackType == CANTP_MAINFUNCTION_CALLBACK)
        {
          TS_AtomicAssign32(ChannelPtr->MfCounterTicks, GPTticks);
        }
        else
        {
#if(CANTP_USE_GPT == STD_ON)
          CANTP_GPT_START_TIMER((Gpt_ChannelType) CANTP_CFG_GET_GPT_CHANNEL(Channel), GPTticks);
#endif
        }
      } /* if StMin == 0 */
    }
  } /* if whole message transmitted */
}

#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */

#if(TS_MERGED_COMPILE == STD_OFF)
#define CANTP_STOP_SEC_CODE
#include <CanTp_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

/*==================[end of file]============================================*/
