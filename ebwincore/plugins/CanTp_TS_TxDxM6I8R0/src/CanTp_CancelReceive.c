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
#include <ComStack_Types.h>    /* ComStack types */

#include <CanTp_Api.h>         /* Module public API, config dependent */
#include <CanTp_Cfg.h>         /* Module configuration */
#include <CanTp_Internal.h>    /* internal macros and variables */
#if(CANTP_JUMPTABLE_SUPPORT == STD_ON)
#include <CanTp_EntryTable.h>  /* CanTp entry jumptable */
#endif /* CANTP_JUMPTABLE_SUPPORT == STD_ON */

/* do not indirectly include PduR_SymbolicNames_PBcfg.h via PduR_CanTp.h */
#define PDUR_NO_PBCFG_REQUIRED
#include <PduR_CanTp.h>        /* PduR module types and API */
#if(CANTP_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>               /* API functions of DET */
#endif

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

#if(CANTP_RX_CANCELLATION_API == STD_ON)

#if(TS_MERGED_COMPILE == STD_OFF)
#define CANTP_START_SEC_CODE
#include <CanTp_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

#if(CANTP_PROVIDE_API_FUNCTIONS == STD_ON)

FUNC(Std_ReturnType, CANTP_CODE) CanTp_CancelReceive
(
  PduIdType CanTpRxSduId
)
#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)
{

  Std_ReturnType ret_result = E_NOT_OK;

  DBG_CANTP_CANCELRECEIVE_ENTRY(CanTpRxSduId);

#if(CANTP_DEV_ERROR_DETECT == STD_ON)

  if(CANTP_ON != CanTp_InitVars.State)
  {
    CANTP_DET_REPORT_ERROR(
                            CANTP_MODULE_ID,
                            CANTP_INSTANCE_ID,
                            CANTP_API_CANCELRECEIVE,
                            CANTP_E_UNINIT
                          );
  }
  /* Check Id range - possible DET and return on error */
  else if(CanTpRxSduId >= CanTp_CfgPtr->NumberRxNSdus)
  {
    /* call DET with error ERRORCODE */
    CANTP_DET_REPORT_ERROR(
                            CANTP_MODULE_ID,
                            CANTP_INSTANCE_ID,
                            CANTP_API_CANCELRECEIVE,
                            CANTP_E_PARAM_ID
                          );
  }
  else
#endif /* CANTP_DEV_ERROR_DETECT == STD_ON */
  {
    CanTp_ChType Channel;
    boolean ChannelLockSuccess;
    Std_ReturnType ChannelLockedFound = E_NOT_OK;
    P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr;  /* Pointer to channel struct */

    /* Channel state */
    CanTp_ChStateType ChannelState;

    /* Get the first release Rx channel */
    CANTP_GET_LOCKED_RX_CH_INDEX(Channel, CanTpRxSduId, ChannelLockedFound);

#if(CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON)
    if(E_OK == ChannelLockedFound)
#endif /* CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON */
    {
      /* get the channel structure first */
      ChannelPtr = &CanTp_Channel[Channel];

      /* Enter critical section now to ensure consistency of reception check and channel lock */
      CANTP_ENTER_CRITICAL_SECTION();

      /* lock channel if channel is used and not currently locked */
      if(CANTP_IS_CHANNEL_LOCKED(ChannelPtr))
      {
        ChannelLockSuccess = FALSE;
        ret_result = E_NOT_OK;
      }
      else
      {
        CANTP_LOCK_CHANNEL(ChannelPtr);
        ChannelLockSuccess = TRUE;
      }
      CANTP_LEAVE_CRITICAL_SECTION();

      if(ChannelLockSuccess)
      {
        /* Check that the cancel request corresponds to the actual N-SDU in progress.
         In case that another N-SDU is processed at the moment return E_NOT_OK.
         */
        if(ChannelPtr->NSduId == CanTpRxSduId)
        {
          uint8 MaxCfLength;   /* maximum length of a CF */

          if(CANTP_IS_NORMAL_ADDRESSING(CANTP_GET_ADDR_FORMAT(&CANTP_CFG_GET_RXNSDU_CONFIG(ChannelPtr->NSduId))))
          {
            MaxCfLength = 7U;
          }
          else
          {
            MaxCfLength = 6U;
          }

          /* atomic assign */
          TS_AtomicAssign8(ChannelState, ChannelPtr->State);

          switch(ChannelState)
          {
          case CANTP_CH_RX_BUFREQ_SF:
          {
            /* !LINKSTO CanTp.ASR40.CANTP262_1, 1 */
            ret_result = E_NOT_OK;
            break;
          }

          case CANTP_CH_RX_BUFREQ_FF: /* Fall through */
          case CANTP_CH_RX_TRANSMIT_FC_CTS: /* Fall through */
          case CANTP_CH_RX_TRANSMIT_FC_OVFLW: /* Fall through */
          case CANTP_CH_RX_TRANSMIT_FC_WT:
          case CANTP_CH_RX_POSTPONED_FC_CTS:
          case CANTP_CH_RX_POSTPONED_FC_OVFLW:
          case CANTP_CH_RX_POSTPONED_FC_WT:
          {
            /* In these states a reception is not expected (N_Cr is not started).
               Process cancellation.
             */
            ret_result = E_OK;
            break;
          }
          case CANTP_CH_RX_RECEIVE_CF:
          {
            if((ChannelPtr->TotalLength - ChannelPtr->ProcessedLength)
                <= MaxCfLength
            )
            {
              /* Reception of last CF is expected (N_Cr started).
                 Cancellation will not be executed.
               */
              ret_result = E_NOT_OK;
            }
            else
            {
              ret_result = E_OK;
            }
            break;
          }

          case CANTP_CH_RX_BUFREQ_CF:
          {
            /* Channel is requesting buffer for next block. */
            ret_result = E_OK;
            break;
          }

          /* Don't accept cancellation while waiting for lower layer response. */
          case CANTP_CH_RX_CONFIRM_FC_CTS: /* Fall through */
          case CANTP_CH_RX_CONFIRM_FC_OVFLW: /* Fall through */
          case CANTP_CH_RX_CONFIRM_FC_WT:
          {
            ret_result = E_NOT_OK;
            break;
          }
          default:
          {
            /* Unknown RX state (N-Sdu is not on reception process). */
            /* !LINKSTO CanTp.ASR40.CANTP260_2, 1 */
#if(CANTP_DEV_ERROR_DETECT == STD_ON)
            CANTP_DET_REPORT_ERROR(
                CANTP_MODULE_ID,
                CANTP_INSTANCE_ID,
                CANTP_API_CANCELRECEIVE,
                CANTP_E_OPER_NOT_SUPPORTED
            );
#endif /* CANTP_DEV_ERROR_DETECT == STD_ON */
            ret_result = E_NOT_OK;
            break;
          }
          }
          if(E_OK == ret_result)
          {
            CanTp_StopChannel(
                ChannelPtr,
                CANTP_ULIND_STATUS,
                NTFRSLT_E_NOT_OK
            );
          }
        } /* if(ChannelPtr->NSduId == CanTpRxSduId) */
        else
        {
          /* N-Sdu is not on reception process. */
          /* !LINKSTO CanTp.ASR40.CANTP260_2, 1 */
#if(CANTP_DEV_ERROR_DETECT == STD_ON)
          CANTP_DET_REPORT_ERROR(
              CANTP_MODULE_ID,
              CANTP_INSTANCE_ID,
              CANTP_API_CANCELRECEIVE,
              CANTP_E_OPER_NOT_SUPPORTED
          );
#endif /* CANTP_DEV_ERROR_DETECT == STD_ON */
          ret_result = E_NOT_OK;
        }

        CanTp_HandlePostponedFlags(ChannelPtr);
        /* free channel */
        CANTP_UNLOCK_CHANNEL(ChannelPtr);
      }
      /* else: deny cancellation of channel is locked - this is a deviation to AUTOSAR
       * but is conform to the actual implementation of Transmit Cancellation.
       * Return E_NOT_OK (see Deviations.xml).
       */

    }
#if(CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON)
    else
    {
      /* N-Sdu is not on reception process. */
      /* !LINKSTO CanTp.ASR40.CANTP260_2, 1 */
#if(CANTP_DEV_ERROR_DETECT == STD_ON)
      CANTP_DET_REPORT_ERROR(
          CANTP_MODULE_ID,
          CANTP_INSTANCE_ID,
          CANTP_API_CANCELRECEIVE,
          CANTP_E_OPER_NOT_SUPPORTED
      );
#endif /* CANTP_DEV_ERROR_DETECT == STD_ON */
      ret_result = E_NOT_OK;
    }
#endif /* CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON */

  }

  DBG_CANTP_CANCELRECEIVE_EXIT(ret_result, CanTpRxSduId);

  return ret_result;
} /* CanTp_CancelReceive() */

#else /* => CANTP_JUMPTABLE_MODE == CANTP_JUMPTABLE_CLIENT */
{
  Std_ReturnType ret_result = E_NOT_OK;
  DBG_CANTP_CANCELRECEIVE_ENTRY(CanTpRxSduId);
  ret_result = (CanTp_EntryTablePtr->CancelReceive)(CanTpRxSduId);
  DBG_CANTP_CANCELRECEIVE_EXIT(ret_result, CanTpRxSduId);
  return ret_result;
}

#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */

#endif /* CANTP_PROVIDE_API_FUNCTIONS == STD_ON */

#if(TS_MERGED_COMPILE == STD_OFF)
#define CANTP_STOP_SEC_CODE
#include <CanTp_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */
#endif /* CANTP_RX_CANCELLATION_API */

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/
