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
#include <CanTp_Api.h>         /* Module public API, config dependent */
#include <CanTp_Internal.h>    /* internal macros and variables */
#if(CANTP_JUMPTABLE_SUPPORT == STD_ON)
#include <CanTp_EntryTable.h>  /* CanTp entry jumptable */
#endif /* CANTP_JUMPTABLE_SUPPORT == STD_ON */
#if(CANTP_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>
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

#if(CANTP_TX_CANCELLATION_API == STD_ON)
#if(TS_MERGED_COMPILE == STD_OFF)
#define CANTP_START_SEC_CODE
#include <CanTp_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

#if(CANTP_PROVIDE_API_FUNCTIONS == STD_ON)

FUNC(Std_ReturnType, CANTP_CODE) CanTp_CancelTransmit
(
  PduIdType CanTpTxSduId
)
#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)
{
  Std_ReturnType  ret_result = E_NOT_OK; /* return value */

  DBG_CANTP_CANCELTRANSMIT_ENTRY(CanTpTxSduId);

#if(CANTP_DEV_ERROR_DETECT == STD_ON)
  /* check CanTp state - possible DET and return on error */
  if(CanTp_InitVars.State != CANTP_ON)
  {
    CANTP_DET_REPORT_ERROR(
                            CANTP_MODULE_ID,
                            CANTP_INSTANCE_ID,
                            CANTP_API_CANCELTRANSMIT,
                            CANTP_E_UNINIT
                          );
  }
  /* check PduId range - possible DET and return on error */
  else if(CanTpTxSduId >= CanTp_CfgPtr->NumberTxNSdus)
  {
    CANTP_DET_REPORT_ERROR(
                            CANTP_MODULE_ID,
                            CANTP_INSTANCE_ID,
                            CANTP_API_CANCELTRANSMIT,
                            CANTP_E_PARAM_ID
                          );
  }
  else
#endif
  {
    /* Process only if Tx cancel is enabled for Tx-NSdu */
    if((boolean)TRUE == CANTP_CFG_GET_TXNSDU_CONFIG(CanTpTxSduId).CanTpTc)
    {
      CanTp_TxChannelType Channel;  /* counter for channels */
      boolean  ChannelLockSuccess;
      Std_ReturnType IsChannelAvailable = E_NOT_OK;
      P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr;

      /* Channel state */
      CanTp_ChStateType ChannelState;

      CANTP_GET_LOCKED_TX_CH_INDEX(Channel, CanTpTxSduId, IsChannelAvailable);

#if(CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON)
      if(E_OK == IsChannelAvailable)
#endif /* CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON */
      {
        ChannelPtr = &CanTp_Channel[Channel];

        CANTP_ENTER_CRITICAL_SECTION();

        /* lock channel if not currently locked */
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
          /* Check that the cancel request corresponds to the actual N-Sdu in progress.
           In case that another N-Sdu is processed at the moment return E_NOT_OK.
           */
          if(ChannelPtr->NSduId == CanTpTxSduId)
          {
            /* atomic assing */
            TS_AtomicAssign8(ChannelState, ChannelPtr->State);
            switch(ChannelState)
            {
            /* Only accept cancellation for segmented N-Sdus. */
            case CANTP_CH_TX_BUFREQ_SF:
            case CANTP_CH_TX_POSTPONED_SF:
            {
              ret_result = E_NOT_OK;
              break;
            }

            case CANTP_CH_TX_WAITTMR_CF: /* Fall through */
            case CANTP_CH_TX_BUFREQ_FF: /* Fall through */
            case CANTP_CH_TX_BUFREQ_CF: /* Fall through */
            case CANTP_CH_TX_POSTPONED_FF:
            case CANTP_CH_TX_POSTPONED_CF:
            case CANTP_CH_TX_REQUESTEDBUF_CF:
            {
              ret_result = E_OK;
              break;
            }

            /* Don't accept cancellation while waiting for FC(CTS). A consecutive
               transmission may get this CTS as a response to its FF.
             */
            case CANTP_CH_TX_RECEIVE_FC: /* Fall through */
              /* Don't accept cancellation while waiting for lower layer response. */
            case CANTP_CH_TX_CONFIRM_SF: /* Fall through */
            case CANTP_CH_TX_CONFIRM_FF: /* Fall through */
            case CANTP_CH_TX_CONFIRM_CF:
            {
              ret_result = E_NOT_OK;
              break;
            }
            default:
            {
              /* Unknown TX state (N-Sdu is not on transmission process). */
              /* !LINKSTO CanTp.ASR40.CANTP254_2, 1 */
#if(CANTP_DEV_ERROR_DETECT == STD_ON)
              CANTP_DET_REPORT_ERROR(
                  CANTP_MODULE_ID,
                  CANTP_INSTANCE_ID,
                  CANTP_API_CANCELTRANSMIT,
                  CANTP_E_OPER_NOT_SUPPORTED
              );
#endif /* CANTP_DEV_ERROR_DETECT == STD_ON */
              ret_result = E_NOT_OK;
              break;
            }
            }

            if(E_OK == ret_result)
            {
              /* !LINKSTO CanTp.ASR40.CANTP256, 1 */
              CanTp_StopChannel(
                  ChannelPtr,
                  CANTP_ULIND_STATUS,
                  NTFRSLT_E_NOT_OK
              );
            }
          } /* if(ChannelPtr->NSduId == CanTpTxSduId) */
          else
          {
            /* N-Sdu is not on transmission process */
            /* !LINKSTO CanTp.ASR40.CANTP254_2, 1 */
#if(CANTP_DEV_ERROR_DETECT == STD_ON)
            CANTP_DET_REPORT_ERROR(
                CANTP_MODULE_ID,
                CANTP_INSTANCE_ID,
                CANTP_API_CANCELTRANSMIT,
                CANTP_E_OPER_NOT_SUPPORTED
            );
#endif /* CANTP_DEV_ERROR_DETECT == STD_ON */
            ret_result = E_NOT_OK;
          }

          CanTp_HandlePostponedFlags(ChannelPtr);

          /* unlock channel*/
          CANTP_UNLOCK_CHANNEL(ChannelPtr);
        }
      }
#if(CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON)
      else /* all channels locked with other NSdus */
      {
        /* N-Sdu is not on transmission process */
        /* !LINKSTO CanTp.ASR40.CANTP254_2, 1 */
#if(CANTP_DEV_ERROR_DETECT == STD_ON)
        CANTP_DET_REPORT_ERROR(
            CANTP_MODULE_ID,
            CANTP_INSTANCE_ID,
            CANTP_API_CANCELTRANSMIT,
            CANTP_E_OPER_NOT_SUPPORTED
        );
#endif /* CANTP_DEV_ERROR_DETECT == STD_ON */
        ret_result = E_NOT_OK;
      }
#endif /* CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON */
    }
  }

  DBG_CANTP_CANCELTRANSMIT_EXIT(ret_result, CanTpTxSduId);

  return ret_result;
} /* CanTp_CancelTransmit() */
#else /* => CANTP_JUMPTABLE_MODE == CANTP_JUMPTABLE_CLIENT */
{
  Std_ReturnType ret_result = E_NOT_OK;
  DBG_CANTP_CANCELTRANSMIT_ENTRY(CanTpTxSduId);
  ret_result = (CanTp_EntryTablePtr->CancelTransmit)(CanTpTxSduId);
  DBG_CANTP_CANCELTRANSMIT_EXIT(ret_result, CanTpTxSduId);
  return ret_result;
}
#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */

#endif /* CANTP_PROVIDE_API_FUNCTIONS == STD_ON */

#if(TS_MERGED_COMPILE == STD_OFF)
#define CANTP_STOP_SEC_CODE
#include <CanTp_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

#endif /* CANTP_TX_CANCELLATION_API */

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/
