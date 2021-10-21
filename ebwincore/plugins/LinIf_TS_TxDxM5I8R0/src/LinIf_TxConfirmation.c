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

#define LINIF_NO_PBCFG_REQUIRED
#include <LinIf.h>            /* Module library API */
#include <LinIf_Int.h>        /* LinIf internal data */
#include <LinTp_Int.h>        /* LinTp internal data */
/* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00865.Enabled,1 */
#if (LINIF_SLAVE_SUPPORTED == STD_ON)
#include <LinIf_Trace.h>
#include <LinIf_Externals.h>  /* for callouts */
#include <SchM_LinIf.h>       /* for protecting exclusive areas */
#include <Com.h>              /* for Com_SendSignal */

#if ((LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON))
#include <LinIf_Version.h>
#include <Det.h>              /* Default Error Tracer */
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON) */

#if (LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON)
#if (LINIF_CDD_REPORTING_ENABLE == STD_OFF)
#include <Mirror.h>
#endif /* LINIF_CDD_REPORTING_ENABLE == STD_OFF */
#endif /* LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#if (TS_MERGED_COMPILE == STD_OFF)
/* AUTOSAR Memory Mapping - start section for code */
#define LINIF_START_SEC_CODE
#include <LinIf_MemMap.h>
#endif /* TS_MERGED_COMPILE */

FUNC(void, LINIF_CODE) LinIf_TxConfirmation
(
  NetworkHandleType Channel
)
{
  DBG_LINIF_TXCONFIRMATION_ENTRY(Channel);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
  /* check DET errors first */
  if(LINIF_UNINIT_ACCESS)
  {
    /* !LINKSTO LinIf.EB.UnInit.TxConfirmation,1,
                LinIf.ASR20-11.SWS_LinIf_00376_1.LINIF_E_UNINIT,1
     */
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
                          LINIF_API_TXCONF, LINIF_E_UNINIT);
  }
  else if ((Channel < LinIf_ConfigPtr->NumChannelsMaster)
       ||  (Channel >= (LinIf_ConfigPtr->NumChannelsMaster + LinIf_ConfigPtr->NumSlaveChannels)))
  {
    /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00853,1 */
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
              LINIF_API_TXCONF, LINIF_E_NONEXISTENT_CHANNEL);
  }
  else
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */
  {
    CONSTP2VAR(LinIf_ChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannel =
      &LinIf_ChannelInfo[Channel];

    /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00189,1 */
    if(pIfChannel->cFSM == LINIF_CHANNEL_OPERATIONAL)
    {
      CONSTP2CONST(LinIfSlaveChannelType, AUTOMATIC, LINIF_VAR) pIfChConfSlave =
        LINIF_CFG_GET_CHANNEL_SLAVE(Channel);
      CONSTP2VAR(LinIf_SlaveChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannelSlave =
        LINIF_GET_CHANNEL_SLAVE(Channel);

      /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00754.TxConfirmation,1 */
      LinIf_StartTimerBI(pIfChConfSlave, pIfChannelSlave);

      switch(pIfChannel->LastHeaderType)
      {
        case LINIF_FRM_EMPTY:
        {
          /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00852,1 */
          break;
        }
        case LINIF_FRM_TX:
        {
          /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00741_1,1 */
          LinIf_Status_PduRTxConf(Channel, NULL_PTR); /* Safe, param unused */

#if (LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON)
          {
            uint16 enabledChannels;

            TS_AtomicAssign16(enabledChannels, LinIf_BusMiroringChannelEnabled);
            if (LINIF_CHANNEL_ENABLED == LINIF_MIRROR_CHANNEL_ENABLED(enabledChannels, Channel))
            {
              PduInfoType pdu;
              pdu.SduDataPtr = pIfChannel->FrameInfo.FrameData;
              pdu.SduLength = pIfChannel->FrameInfo.FrameLength;
              /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00839,1 */
              LINIF_UL_REPORTING(LINIF_GET_COMM_CHANNEL(Channel), pIfChannel->FrameInfo.Pid, &pdu, LIN_TX_OK);
            }
          }
#endif /* LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON */

          {
            uint8 flags;
            TS_AtomicAssign8(flags, pIfChannelSlave->Flags);
            /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00747,1,
                        LinIf.ASR20-11.SWS_LinIf_00763,1
             */
            if((flags & (uint8)(LINIF_FLG_RESP_ERR_SET | LINIF_FLG_RESP_ERR_TX)) == (uint8)(LINIF_FLG_RESP_ERR_SET | LINIF_FLG_RESP_ERR_TX))
            {
              TS_AtomicClearBit_8(&pIfChannelSlave->Flags, LINIF_BIT_FLG_RESP_ERR_SET);
              TS_AtomicClearBit_8(&pIfChannelSlave->Flags, LINIF_BIT_FLG_RESP_ERR_TX);

              {
                /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00764,1 */
                uint8 sig = 0U;
                (void)Com_SendSignal(pIfChConfSlave->ErrRespSigId, &sig);
              }
              /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00766,1 */
#if (LINIF_ERR_RES_CHG_CO_SUPPORTED == STD_ON)
              /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00765,1 */
              LINIF_ERR_RES_CHG_CO_FUNC(Channel, FALSE);
#endif /* LINIF_ERR_RES_CHG_CO_SUPPORTED == STD_ON */
            }
          }

          break;
        }
        case LINIF_FRM_SRF:
        {
          /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00785_2,1 */
          LinIf_StopTimerNas(pIfChannelSlave);
          pIfChannelSlave->RSID = 0U;
#if (LINIF_TP_SUPPORTED == STD_ON)
          LinTp_TxConfirmation(Channel, NULL_PTR);
#endif
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

      pIfChannel->LastHeaderType = LINIF_FRM_EMPTY;
    }
  }

  DBG_LINIF_TXCONFIRMATION_EXIT(Channel);
}

#if (TS_MERGED_COMPILE == STD_OFF)
/* AUTOSAR Memory Mapping - end section for code */
#define LINIF_STOP_SEC_CODE
#include <LinIf_MemMap.h>
#endif /* TS_MERGED_COMPILE */

#else
#if (TS_MERGED_COMPILE == STD_OFF)
#include <TSCompiler.h>
TS_PREVENTEMPTYTRANSLATIONUNIT
#endif /* TS_MERGED_COMPILE */
/* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00865.Disabled,1 */
#endif /* LINIF_SLAVE_SUPPORTED == STD_ON */
/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
