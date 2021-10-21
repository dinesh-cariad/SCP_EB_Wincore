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

/* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00866.Enabled,1 */
#if (LINIF_SLAVE_SUPPORTED == STD_ON)
#include <LinIf_Int.h>        /* LinIf internal data */
#include <LinTp_Int.h>        /* LinTp internal data */
#include <LinIf_Trace.h>
#include <LinIf_Externals.h>  /* Callouts */
#include <Com.h>              /* for Com_SendSignal */
#if (LINIF_LIN_ERROR_CALLOUT == STD_ON)
#include LINIF_LIN_ERROR_CALLOUT_INCLUDE
#endif /* (LINIF_LIN_ERROR_CALLOUT == STD_ON) */

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

STATIC FUNC(void, LINIF_CODE) LinIf_HandleErrorStatus
(
  NetworkHandleType Channel,
  Lin_SlaveErrorType ErrorStatus,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) ErrSig,
  P2VAR(Lin_StatusType, AUTOMATIC, AUTOMATIC) Status
);

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

/* !LINKSTO LinIf.ASR20-11.SWS_LinIf_91007,1 */
FUNC(void, LINIF_CODE) LinIf_LinErrorIndication
(
  NetworkHandleType Channel,
  Lin_SlaveErrorType ErrorStatus
)
{
  DBG_LINIF_LINERRORINDICATION_ENTRY(Channel, ErrorStatus);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
  /* check DET errors first */
  if(LINIF_UNINIT_ACCESS)
  {
      /* !LINKSTO LinIf.EB.UnInit.LinErrorIndication,1,
                  LinIf.ASR20-11.SWS_LinIf_00376_1.LINIF_E_UNINIT,1
       */
      (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
                            LINIF_API_LINERRORIND, LINIF_E_UNINIT);
  }
  else if ((Channel < LinIf_ConfigPtr->NumChannelsMaster)
       ||  (Channel >= (LinIf_ConfigPtr->NumChannelsMaster + LinIf_ConfigPtr->NumSlaveChannels)))
  {
      /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00855,1 */
      (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
                LINIF_API_LINERRORIND, LINIF_E_NONEXISTENT_CHANNEL);
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

      Lin_StatusType status = LIN_NOT_OK;
      uint8 errSig = 0U;
      boolean skipProcessing = FALSE;

      /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00754.LinErrorIndication,1 */
      LinIf_StartTimerBI(pIfChConfSlave, pIfChannelSlave);

      switch(pIfChannel->LastHeaderType)
      {
        case LINIF_FRM_TX:
        {
          status = LIN_TX_ERROR;
          break;
        }
        case LINIF_FRM_SRF:
        {
#if (LINIF_TP_SUPPORTED == STD_ON)
          if(LINIF_RSID_FWD_TP == pIfChannelSlave->RSID)
          {
            /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00796,1,
                        LinIf.ASR20-11.SWS_LinIf_00837_2_1,1,
                        LinIf.ASR20-11.SWS_LinIf_00837_2_2,1
             */
            LinTp_Err_Tx(Channel, NULL_PTR); /* param not used */
          }
#endif /* LINIF_TP_SUPPORTED == STD_ON */
          pIfChannelSlave->RSID = 0U;
          status = LIN_TX_ERROR;
          break;
        }
        case LINIF_FRM_RX:
        {
          status = LIN_RX_ERROR;
          break;
        }
        case LINIF_FRM_MRF:
        {
          /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00837_1_1,1,
                      LinIf.ASR20-11.SWS_LinIf_00837_1_2,1
           */
#if (LINIF_TP_SUPPORTED == STD_ON)
          /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00837_1_3,1 */
          LinTp_Err_Rx(Channel);
#endif /* LINIF_TP_SUPPORTED == STD_ON */
          status = LIN_RX_ERROR;
          break;
        }
        case LINIF_FRM_EMPTY:
        {
          skipProcessing = TRUE;
          break;
        }
        /* CHECK: NOPARSE */
        default:
        {
          /*should not happen*/
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
          LINIF_UNREACHABLE_CODE_ASSERT(LINIF_API_LINERRORIND);
#endif /* LINIF_DEV_ERROR_DETECT == STD_ON */
          break;
        }
        /* CHECK: PARSE */
      }

      if(FALSE == skipProcessing)
      {

        LinIf_HandleErrorStatus(Channel, ErrorStatus, &errSig, &status);

        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00764,1 */
        if(0U != errSig)
        {
          TS_AtomicAssign8(pIfChannelSlave->Flags, LINIF_FLG_RESP_ERR_SET);
          (void)Com_SendSignal(pIfChConfSlave->ErrRespSigId, &errSig);
          /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00766,1 */
#if (LINIF_ERR_RES_CHG_CO_SUPPORTED == STD_ON)
          /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00765,1 */
          LINIF_ERR_RES_CHG_CO_FUNC(Channel, TRUE);
#endif /* LINIF_ERR_RES_CHG_CO_SUPPORTED == STD_ON */
        }

        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00735.LIN_ERR_HEADER,1,
                    LinIf.ASR20-11.SWS_LinIf_00869.LIN_ERR_HEADER,1,
                    LinIf.ASR20-11.SWS_LinIf_00743.LIN_ERR_HEADER,1
         */
        if(LIN_TX_HEADER_ERROR != status)
        {
          /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00869.ERR_RESP_STOPBIT.RX,1,
                      LinIf.ASR20-11.SWS_LinIf_00869.ERR_RESP_STOPBIT.TX,1,
                      LinIf.ASR20-11.SWS_LinIf_00869.ERR_RESP_CHKSUM,1,
                      LinIf.ASR20-11.SWS_LinIf_00869.ERR_INCOMP_RESP,1,
                      LinIf.ASR20-11.SWS_LinIf_00869.ERR_NO_RESP,1,
                      LinIf.ASR20-11.SWS_LinIf_00869.ERR_RESP_DATABIT,1
           */
#if(LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON)
          uint16 chEnabled;
          TS_AtomicAssign16(chEnabled, LinIf_BusMiroringChannelEnabled);

          if (LINIF_CHANNEL_ENABLED == LINIF_MIRROR_CHANNEL_ENABLED(chEnabled, Channel))
          {
            LINIF_UL_REPORTING(LINIF_GET_COMM_CHANNEL(Channel), pIfChannel->FrameInfo.Pid, NULL_PTR, status);
          }
#endif /* LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON */

          /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00735.ERR_RESP_STOPBIT,1,
                      LinIf.ASR20-11.SWS_LinIf_00743.ERR_RESP_STOPBIT,1,
                      LinIf.ASR20-11.SWS_LinIf_00735.ERR_RESP_CHKSUM,1,
                      LinIf.ASR20-11.SWS_LinIf_00743.ERR_RESP_CHKSUM,1,
                      LinIf.ASR20-11.SWS_LinIf_00735.ERR_INCOMP_RESP,1,
                      LinIf.ASR20-11.SWS_LinIf_00743.ERR_INCOMP_RESP,1,
                      LinIf.ASR20-11.SWS_LinIf_00735.ERR_NO_RESP,1,
                      LinIf.ASR20-11.SWS_LinIf_00743.ERR_NO_RESP,1,
                      LinIf.ASR20-11.SWS_LinIf_00735.ERR_RESP_DATABIT,1,
                      LinIf.ASR20-11.SWS_LinIf_00743.ERR_RESP_DATABIT,1
           */
#if (LINIF_RUNTIME_ERROR_REPORTING == STD_ON)
          (void)Det_ReportRuntimeError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
                                       LINIF_API_LINERRORIND, LINIF_E_RESPONSE);
#endif
        }

        LINIF_LIN_ERROR_NOTIFY(Channel, status);

        pIfChannel->LastHeaderType = LINIF_FRM_EMPTY;
      }
    }
  }

  DBG_LINIF_LINERRORINDICATION_EXIT(Channel, ErrorStatus);
}

STATIC FUNC(void, LINIF_CODE) LinIf_HandleErrorStatus
(
  NetworkHandleType Channel,
  Lin_SlaveErrorType ErrorStatus,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) ErrSig,
  P2VAR(Lin_StatusType, AUTOMATIC, AUTOMATIC) Status
)
{

#if(LINIF_DEM_EVENTS_CONFIGURED)
  CONSTP2VAR(LinIf_ChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannel =
    &LinIf_ChannelInfo[Channel];
#else
  TS_PARAM_UNUSED(Channel);
#endif /* LINIF_DEM_EVENTS_CONFIGURED */

  switch(ErrorStatus)
  {
    case LIN_ERR_HEADER:
    {
      *Status = LIN_TX_HEADER_ERROR;
      break;
    }
    case LIN_ERR_RESP_STOPBIT:
    {
      /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00736.ERR_RESP_STOPBIT,1,
                  LinIf.ASR20-11.SWS_LinIf_00744.ERR_RESP_STOPBIT,1
       */
      *ErrSig = 1U;

      /* status already set */
      if(LIN_TX_ERROR == *Status)
      {
#if(LINIF_DEM_EVENTS_CONFIGURED)
#if(LINIF_PROD_ERR_HANDLING_TX_BIT == TS_PROD_ERR_REP_TO_DEM)
        if ((pIfChannel->LastDemEventsIndex != LINIF_FRAME_DEMEVENTS_NOT_AVAILABLE)
           && (LINIF_GET_TXBIT_FRAME_DEMEVENT_ID(pIfChannel->LastDemEventsIndex) != LINIF_INVALID_DEM_EVENT_ID))
#endif /* LINIF_PROD_ERR_HANDLING_TX_BIT == TS_PROD_ERR_REP_TO_DEM */
#if((LINIF_PROD_ERR_HANDLING_TX_BIT == TS_PROD_ERR_REP_TO_DEM) || (LINIF_PROD_ERR_HANDLING_TX_BIT == TS_PROD_ERR_REP_TO_DET))
        {
          LinIf_Status_HandleTxBitError(Channel, LIN_TX_BIT_ERROR, NULL_PTR); /* safe, not used */
        }
#endif /* TS_PROD_ERR_REP_TO_DEM || TS_PROD_ERR_REP_TO_DET */
#endif /* LINIF_DEM_EVENTS_CONFIGURED */
      }

      break;
    }
    case LIN_ERR_RESP_CHKSUM:
    {
      /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00736.ERR_RESP_CHKSUM,1,
                  LinIf.ASR20-11.SWS_LinIf_00744.ERR_RESP_CHKSUM,1
       */
      *ErrSig = 1U;

      *Status = LIN_RX_ERROR;
#if(LINIF_DEM_EVENTS_CONFIGURED)
#if(LINIF_PROD_ERR_HANDLING_RX_CHECKSUM == TS_PROD_ERR_REP_TO_DEM)
        if ((pIfChannel->LastDemEventsIndex != LINIF_FRAME_DEMEVENTS_NOT_AVAILABLE)
           && (LINIF_GET_CHECKSUMERR_FRAME_DEMEVENT_ID(pIfChannel->LastDemEventsIndex) != LINIF_INVALID_DEM_EVENT_ID))
#endif /* LINIF_PROD_ERR_HANDLING_TX_BIT == TS_PROD_ERR_REP_TO_DEM */
#if((LINIF_PROD_ERR_HANDLING_RX_CHECKSUM == TS_PROD_ERR_REP_TO_DEM) || (LINIF_PROD_ERR_HANDLING_RX_CHECKSUM == TS_PROD_ERR_REP_TO_DET))
        {
          LinIf_Status_HandleRxChecksumError(Channel, LIN_RX_CHECKSUM_ERROR);
        }
#endif /* TS_PROD_ERR_REP_TO_DEM || TS_PROD_ERR_REP_TO_DET */
#endif /* LINIF_DEM_EVENTS_CONFIGURED */
      break;
    }
    case LIN_ERR_RESP_DATABIT:
    {
      /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00736.ERR_RESP_DATABIT,1,
                  LinIf.ASR20-11.SWS_LinIf_00744.ERR_RESP_DATABIT,1
       */
      *ErrSig = 1U;

      *Status = LIN_TX_ERROR;

#if(LINIF_DEM_EVENTS_CONFIGURED)
#if(LINIF_PROD_ERR_HANDLING_TX_BIT == TS_PROD_ERR_REP_TO_DEM)
      if ((pIfChannel->LastDemEventsIndex != LINIF_FRAME_DEMEVENTS_NOT_AVAILABLE)
         && (LINIF_GET_TXBIT_FRAME_DEMEVENT_ID(pIfChannel->LastDemEventsIndex) != LINIF_INVALID_DEM_EVENT_ID))
#endif /* LINIF_PROD_ERR_HANDLING_TX_BIT == TS_PROD_ERR_REP_TO_DEM */
#if((LINIF_PROD_ERR_HANDLING_TX_BIT == TS_PROD_ERR_REP_TO_DEM) || (LINIF_PROD_ERR_HANDLING_TX_BIT == TS_PROD_ERR_REP_TO_DET))
      {
        LinIf_Status_HandleTxBitError(Channel, LIN_TX_BIT_ERROR, NULL_PTR); /* safe, not used */
      }
#endif /* TS_PROD_ERR_REP_TO_DEM || TS_PROD_ERR_REP_TO_DET */
#endif /* LINIF_DEM_EVENTS_CONFIGURED */

      break;
    }
    case LIN_ERR_NO_RESP:
    {
      *Status = LIN_RX_NO_RESPONSE;

#if(LINIF_DEM_EVENTS_CONFIGURED)
#if(LINIF_PROD_ERR_HANDLING_RX_NO_RESPONSE == TS_PROD_ERR_REP_TO_DEM)
      if ((pIfChannel->LastDemEventsIndex != LINIF_FRAME_DEMEVENTS_NOT_AVAILABLE)
         && (LINIF_GET_NORESP_FRAME_DEMEVENT_ID(pIfChannel->LastDemEventsIndex) != LINIF_INVALID_DEM_EVENT_ID))
#endif /* LINIF_PROD_ERR_HANDLING_RX_NO_RESPONSE == TS_PROD_ERR_REP_TO_DEM */
#if((LINIF_PROD_ERR_HANDLING_RX_NO_RESPONSE == TS_PROD_ERR_REP_TO_DEM) || (LINIF_PROD_ERR_HANDLING_RX_NO_RESPONSE == TS_PROD_ERR_REP_TO_DET))
      {
        LinIf_Status_HandleRxNoResponseError(Channel, LIN_RX_NO_RESPONSE_ERROR);
      }
#endif /* TS_PROD_ERR_REP_TO_DEM || TS_PROD_ERR_REP_TO_DET */
#endif /* LINIF_DEM_EVENTS_CONFIGURED */
      break;
    }
    case LIN_ERR_INC_RESP:
    {
      if(LIN_RX_ERROR == *Status)
      {
        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00736.ERR_INCOMP_RESP,1 */
        *ErrSig = 1U;
      }
      else
      {
        *Status = LIN_RX_ERROR;
      }

#if(LINIF_DEM_EVENTS_CONFIGURED)
#if(LINIF_PROD_ERR_HANDLING_RX_NO_RESPONSE == TS_PROD_ERR_REP_TO_DEM)
        if ((pIfChannel->LastDemEventsIndex != LINIF_FRAME_DEMEVENTS_NOT_AVAILABLE)
           && (LINIF_GET_NORESP_FRAME_DEMEVENT_ID(pIfChannel->LastDemEventsIndex) != LINIF_INVALID_DEM_EVENT_ID))
#endif /* LINIF_PROD_ERR_HANDLING_RX_NO_RESPONSE == TS_PROD_ERR_REP_TO_DEM */
#if((LINIF_PROD_ERR_HANDLING_RX_NO_RESPONSE == TS_PROD_ERR_REP_TO_DEM) || (LINIF_PROD_ERR_HANDLING_RX_NO_RESPONSE == TS_PROD_ERR_REP_TO_DET))
        {
          LinIf_Status_HandleRxNoResponseError(Channel, LIN_RX_NO_RESPONSE_ERROR);
        }
#endif /* TS_PROD_ERR_REP_TO_DEM || TS_PROD_ERR_REP_TO_DET */
#endif /* LINIF_DEM_EVENTS_CONFIGURED */

      break;
    }
    /* CHECK: NOPARSE */
    default:
    {
      /*should not happen*/
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
      LINIF_UNREACHABLE_CODE_ASSERT(LINIF_API_LINERRORIND);
#endif /* LINIF_DEV_ERROR_DETECT == STD_ON */
      break;
    }
    /* CHECK: PARSE */
  }
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
/* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00866.Disabled,1 */
#endif /* LINIF_SLAVE_SUPPORTED == STD_ON */
/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
