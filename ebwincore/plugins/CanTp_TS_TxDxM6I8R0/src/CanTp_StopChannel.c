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
#include <CanTp_InternalCfg.h> /* CanTp internal configuration */
#include <CanTp_Cfg.h>         /* configuration for transmit cancellation */
#include <CanTp_Internal.h>    /* internal macros and variables */

/* do not indirectly include PduR_SymbolicNames_PBcfg.h via PduR_CanTp.h */
#define PDUR_NO_PBCFG_REQUIRED
#include <PduR_CanTp.h>        /* PduR module types and API */
#if(CANTP_USE_GPT == STD_ON)
#include <Gpt.h>               /* Gpt module types and API */
#endif

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

FUNC(void, CANTP_CODE) CanTp_StopChannel
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr,
  CanTp_ULIndicationType ULIndication,
  NotifResultType Result
)
{
  boolean CallPduRCanTpTxConfirmation = FALSE;
  PduIdType NSduID = ChannelPtr->NSduId;
  CanTp_ChStateType ChannelState;
  TS_AtomicAssign8(ChannelState, ChannelPtr->State);

#if(CANTP_USE_GPT == STD_ON)

  /* stop running timer */
  if((ChannelState == CANTP_CH_TX_WAITTMR_CF)||(ChannelState == CANTP_CH_TX_REQUESTEDBUF_CF))
  {
    const CanTp_TxChannelType Channel = CANTP_CFG_GET_TXNSDU_CONFIG(ChannelPtr->NSduId).Channel;

    if(CANTP_CFG_GET_MFCTR_CONFIG(Channel).CallbackType == CANTP_GPT_CALLBACK)
    {
      /* mainfunction timer will be reset by CANTP_INIT_CHANNEL_PTR below */
      CANTP_GPT_STOP_TIMER((Gpt_ChannelType) CANTP_CFG_GET_GPT_CHANNEL(Channel));
    }
  }
#endif

  /* notify upper layer */
  if(ChannelState != CANTP_CH_IDLE)
  {
    if(ULIndication == CANTP_ULIND_STATUS)
    {
      TS_AtomicAssign8(ChannelState, ChannelPtr->State);

      if(CANTP_CH_GET_MODE(ChannelState) == CANTP_CH_MODE_RX)
      {
        CANTP_PDUR_RX_INDICATION
        (
            CANTP_CFG_GET_RXNSDU_CONFIG(ChannelPtr->NSduId).IPduId,
            Result
        );
      }
      else
      {
        CallPduRCanTpTxConfirmation = TRUE;
      }
    }
  }

  CANTP_ENTER_CRITICAL_SECTION();

  /* bring channel to IDLE */
  CanTp_InitChannelPtr(ChannelPtr);

  if(TRUE == CallPduRCanTpTxConfirmation)
  {
    if(CANTP_IS_CHANNEL_LOCKED(ChannelPtr))
    {
      /* Unlock channel */
      ChannelPtr->ChannelLockFlag = 0U;
    }
  }

  CANTP_LEAVE_CRITICAL_SECTION();

  if(TRUE == CallPduRCanTpTxConfirmation)
  {
    CANTP_PDUR_TX_CONFIRMATION
    (
        CANTP_CFG_GET_TXNSDU_CONFIG(NSduID).IPduConfId,
        Result
    );
  }
}

#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */

#if(TS_MERGED_COMPILE == STD_OFF)
#define CANTP_STOP_SEC_CODE
#include <CanTp_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */
/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
