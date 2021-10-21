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

/*==================[inclusions]=============================================*/

#include <CanTp_Trace.h>
#include <TSAutosar.h>         /* EB specific standard types */
#include <TSAtomic_Assign.h>   /* atomic assign macros */
#include <ComStack_Types.h>    /* typedefs for AUTOSAR com stack */
#include <CanTp_Api.h>         /* CanTp API declarations */
#include <CanTp_Internal.h>    /* internal macros and variables */
#if(CANTP_JUMPTABLE_SUPPORT == STD_ON)
#include <CanTp_EntryTable.h>  /* CanTp entry jumptable */
#endif /* CANTP_JUMPTABLE_SUPPORT == STD_ON */

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

#if(TS_MERGED_COMPILE == STD_OFF)
#define CANTP_START_SEC_CODE
#include <CanTp_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)

/** \brief CanTp_ChannelHandling()
 **
 ** This function is to handle each channel Stall,Timeout and State
 ** by calling the following functions: CanTp_StallHandling(),
 ** CanTp_TimeoutHandling() , CanTp_RxStateHandling(),
 ** CanTp_TxStateHandling()
 **
 */
STATIC FUNC(void, CANTP_CODE) CanTp_ChannelHandling
(
  CanTp_ChType Channel
);

/** \brief CanTp_StallHandling()
 **
 ** This function is to reduce the possibility of CF bursts stalling
 ** the transmission of FC frames in concurrent connections
 **
 */
STATIC FUNC(Std_ReturnType, CANTP_CODE) CanTp_StallHandling
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
);

#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */

#if(TS_MERGED_COMPILE == STD_OFF)
#define CANTP_STOP_SEC_CODE
#include <CanTp_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

#if(TS_MERGED_COMPILE == STD_OFF)
#define CANTP_START_SEC_CODE
#include <CanTp_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

#if(CANTP_PROVIDE_API_FUNCTIONS == STD_ON)

/** \brief CanTp_MainFunction()
 **
 ** This API-Service is called cyclically, takes further individual steps for each
 ** channel and checks the timeouts */
FUNC(void, CANTP_CODE) CanTp_MainFunction(void)
#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)
{
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr;
  CanTp_ChType Channel;  /* counter for channels */
  CanTp_ChType No_of_nonIdleChannels = 0U;

  DBG_CANTP_MAINFUNCTION_ENTRY();

  CANTP_GET_NON_IDLE_CHANNELS(No_of_nonIdleChannels);

  /* !LINKSTO CanTp.EB.IdleChannels,1 */
  /* only do something in state CANTP_ON and if at least one channel is not idle */
  if((CanTp_InitVars.State == CANTP_ON) && (0U != No_of_nonIdleChannels))
  {

    /* check counter for every parallel channel, call callback routine if necessary */
    /* Deviation TASKING-1 */
    for(Channel = 0U; Channel < MAX_NUMBER_OF_TX_CHANNELS; Channel++)
    {
      uint32 MfCounterTicks;

      ChannelPtr = &CanTp_Channel[Channel];
      TS_AtomicAssign32(MfCounterTicks, ChannelPtr->MfCounterTicks);

      if(MfCounterTicks > 0U)
      {
        /* decrease STmin counter and call notification function if 0 */
        /* No need to protect the decrement since MfCounterTicks is set upon */
        /* a TX confirmation of a CF to ensure STmin => MfCounterTicks is only */
        /* decremented here when a TX confirmation of a CF cannot happen */
        MfCounterTicks--;

        TS_AtomicAssign32(ChannelPtr->MfCounterTicks, MfCounterTicks);

        if(MfCounterTicks == 0U)
        {
          /* call callback routine */
          CanTp_STminCallback(Channel);
        }
      }
    }

    /* check all connection channels */
    /* Annotation: We are counting down to first traverse the Rx channels,
       followed by the Tx channels. Aim is to reduce the possibility of CF
       bursts stalling the transmission of FC frames in concurrent connections
       (compare the requirement CanTp.EB.FullDuplex.1). */

    Channel = MAX_NUMBER_OF_CHANNELS;
    do
    {
      Channel--;
      CanTp_ChannelHandling(Channel);
    }while(Channel > 0U);
  } /* if CanTp is on */

  DBG_CANTP_MAINFUNCTION_EXIT();

}/* CanTp_MainFunction() */
#else /* => CANTP_JUMPTABLE_MODE == CANTP_JUMPTABLE_CLIENT */
{

  DBG_CANTP_MAINFUNCTION_ENTRY();
  (CanTp_EntryTablePtr->MainFunction)();

  DBG_CANTP_MAINFUNCTION_EXIT();
}
#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */
#endif /* CANTP_PROVIDE_API_FUNCTIONS == STD_ON */


#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)
FUNC(void, CANTP_CODE) CanTp_TimeoutHandling
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
)
{

  CanTp_ChStateType ChannelState;

  CanTp_TxChannelType Tx_ChannelID = INVALID_TX_CHANNEL_ID;
  CanTp_ChType Rx_ChannelID = INVALID_RX_CHANNEL_ID;

  TS_AtomicAssign8(ChannelState, ChannelPtr->State);

/* ---------------------Rx States ------------------*/
  switch(ChannelState)
  {
    case CANTP_CH_RX_BUFREQ_CF:
    case CANTP_CH_RX_BUFREQ_FF:
    {
      /* CanTp.ASR40.CANTP223.2: Can only be reached if maximum number of wait frames exceeded.
       * -> abort with result NTFRSLT_E_WFT_OVRN
       */
      CanTp_StopChannel(
                         ChannelPtr,
                         CANTP_ULIND_STATUS,
                         NTFRSLT_E_WFT_OVRN
                       );
      break;
    }
    case CANTP_CH_RX_BUFREQ_SF:
    {
      /* Possible timeout for SF */
      CanTp_StopChannel(
                         ChannelPtr,
                         CANTP_ULIND_STATUS,
                         NTFRSLT_E_NOT_OK
                       );
      break;
    }
    case CANTP_CH_RX_CONFIRM_FC_CTS:
    case CANTP_CH_RX_CONFIRM_FC_OVFLW:
    case CANTP_CH_RX_CONFIRM_FC_WT:
    {
#if(CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON)
      PduIdType PduId = CANTP_CFG_GET_RXNSDU_CONFIG(ChannelPtr->NSduId).NPduConfIdFc;
#else
      Tx_ChannelID = CANTP_CFG_GET_RXNSDU_CONFIG(ChannelPtr->NSduId).Tx_Channel;
#endif
      /* CanTp.ASR40.CANTP311: no confirmation for FC frame
         -> abort with NTFRSLT_E_TIMEOUT_A
       */
      CanTp_StopChannel(
                         ChannelPtr,
                         CANTP_ULIND_STATUS,
                         NTFRSLT_E_TIMEOUT_A
                       );
#if(CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON)
      /* Get the corresponding Tx Channel */
      if(E_OK == CanTp_GetChannelOfPostponedTxPdu(PduId, &Tx_ChannelID))
#endif /* CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON */
      {
        /* Check if there is a Postponed frame on the Tx Channel */
        CanTp_TransmitPostponedFrame(Tx_ChannelID);
      }
      break;
    }
    case CANTP_CH_RX_RECEIVE_CF:
    {
      /* CanTp.ASR40.CANTP313: timeout while waiting for CF
         -> abort with NTFRSLT_E_TIMEOUT_CR
       */
      CanTp_StopChannel(
                         ChannelPtr,
                         CANTP_ULIND_STATUS,
                         NTFRSLT_E_TIMEOUT_CR
                       );
      break;
    }

    /* ---------------------Tx States ------------------*/
    case CANTP_CH_TX_CONFIRM_SF:
    case CANTP_CH_TX_CONFIRM_FF:
    case CANTP_CH_TX_CONFIRM_CF:
    {
#if(CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON)
      PduIdType PduId = CANTP_CFG_GET_TXNSDU_CONFIG(ChannelPtr->NSduId).NPduConfIdxF;
#else
      Rx_ChannelID = CANTP_CFG_GET_TXNSDU_CONFIG(ChannelPtr->NSduId).Rx_Channel;
#endif

      /* CanTp.ASR40.CANTP310: no confirmation from CAN driver
               -> abort with NTFRSLT_E_TIMEOUT_A
             */
      CanTp_StopChannel(
                         ChannelPtr,
                         CANTP_ULIND_STATUS,
                         NTFRSLT_E_TIMEOUT_A
                       );

#if(CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON)
      /* Get the corresponding Rx Channel */
      if(E_OK == CanTp_GetChannelOfPostponedTxFcPdu(PduId, &Rx_ChannelID))
#endif /* CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON */
      {
        /*Check if there is a Postponed FC on the Rx Channel*/
        CanTp_TransmitPostponedFlowControl(Rx_ChannelID);
      }
    break;
    }
    case CANTP_CH_TX_POSTPONED_SF:
    case CANTP_CH_TX_POSTPONED_FF:
    case CANTP_CH_TX_POSTPONED_CF:
    case CANTP_CH_RX_POSTPONED_FC_CTS:
    case CANTP_CH_RX_POSTPONED_FC_OVFLW:
    case CANTP_CH_RX_POSTPONED_FC_WT:
    {
      CanTp_StopChannel(
                         ChannelPtr,
                         CANTP_ULIND_STATUS,
                         NTFRSLT_E_TIMEOUT_A
                       );
      break;
    }
    case CANTP_CH_TX_RECEIVE_FC:
    {
      /* CanTp.ASR40.CANTP316: Timeout between indication of FC WT
         and next FC */
      CanTp_StopChannel(
                         ChannelPtr,
                         CANTP_ULIND_STATUS,
                         NTFRSLT_E_TIMEOUT_BS
                       );
      break;
    }
    case CANTP_CH_TX_WAITTMR_CF:
    case CANTP_CH_TX_BUFREQ_SF:
    case CANTP_CH_TX_BUFREQ_FF:
    case CANTP_CH_TX_BUFREQ_CF:
    default:
    {
      /* Possible timeout while in buff request */
      CanTp_StopChannel(
                         ChannelPtr,
                         CANTP_ULIND_STATUS,
                         NTFRSLT_E_NOT_OK
                       );
      break;
    }
  }
}
#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT ) */

/*==================[internal function definitions]==========================*/

#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)

STATIC FUNC(void, CANTP_CODE) CanTp_ChannelHandling
(
  CanTp_ChType Channel
)
{
  CanTp_ChStateType ChannelState;
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr;

  Std_ReturnType RetVal = E_NOT_OK;
  ChannelPtr = &CanTp_Channel[Channel];

  TS_AtomicAssign8(ChannelState, ChannelPtr->State);

  /* a timeout is not considered in idle mode */
  if(ChannelState != CANTP_CH_IDLE)
  {
    /* Handling stall condition in concurrent connections */
    RetVal = CanTp_StallHandling(ChannelPtr);

    if(RetVal == E_OK)
    {
      /* has a timeout occured ? */
      if(ChannelPtr->Ticks == 0U)
      {
        /* lock channel */
        CANTP_LOCK_CHANNEL(ChannelPtr);

        CanTp_TimeoutHandling(ChannelPtr);

        /* unlock the channel */
        CANTP_UNLOCK_CHANNEL(ChannelPtr);
      }
      else
      {
        /* countdown ticks */
        ChannelPtr->Ticks--;

        /* Try to bring state machine into next state */
        if(CANTP_CH_GET_MODE(ChannelState) == CANTP_CH_MODE_RX)
        {
#if(CANTP_STALLHANDLING_SUPPORT == STD_ON)
          CanTp_RxStateHandling(ChannelPtr);
#endif
        }
        else
        {
          /* lock channel */
          CANTP_LOCK_CHANNEL(ChannelPtr);

          CanTp_TxStateHandling(ChannelPtr,FALSE);

          /* unlock the channel */
          CANTP_UNLOCK_CHANNEL(ChannelPtr);
        }
      }
    CanTp_HandlePostponedFlags(ChannelPtr);

    }
  }
}

STATIC FUNC(Std_ReturnType, CANTP_CODE) CanTp_StallHandling
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  CANTP_ENTER_CRITICAL_SECTION();

  /* if channel is locked */
  if(CANTP_IS_CHANNEL_LOCKED(ChannelPtr))
  {
    /* add a post tick to the channel */
    ChannelPtr->PostTicks++;
  }
  else
  {
    RetVal = E_OK;
  }/* if(channel locked) */

  /* leave critical section */
  CANTP_LEAVE_CRITICAL_SECTION();

  return RetVal;
}
#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT ) */

#if(TS_MERGED_COMPILE == STD_OFF)
#define CANTP_STOP_SEC_CODE
#include <CanTp_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

/*==================[end of file]============================================*/
