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

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 20.7 (required)
 * Expressions resulting from the expansion of macro parameters shall be
 * enclosed in parentheses.
 *
 * Reason:
 * The AUTOSAR compiler abstraction requires these definitions in this way
 * and the arguments cannot be enclosed in parentheses due to C syntax.
 *
 */

/*==================[inclusions]============================================*/


#define LINIF_NO_PBCFG_REQUIRED
#include <LinIf.h>            /* Module library API */
/* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00863.Enabled,1 */
#if (LINIF_SLAVE_SUPPORTED == STD_ON)
#include <LinIf_Int.h>        /* LinIf internal data */
#include <LinTp_Int.h>        /* LinTp internal data */
#include <LinIf_Externals.h>  /* Callouts */
#include <LinSM_Cbk.h>        /* LinSM callback functions */
#include <PduR_LinIf.h>       /* PDU Router LIN Interface */
#if (LINIF_LIN_ERROR_CALLOUT == STD_ON)
#include LINIF_LIN_ERROR_CALLOUT_INCLUDE
#endif /* (LINIF_LIN_ERROR_CALLOUT == STD_ON) */
#include <LinIf_Trace.h>

#if ((LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON))
#include <LinIf_Version.h>
#include <Det.h>              /* Default Error Tracer */
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON) */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/
#if (TS_MERGED_COMPILE == STD_OFF)
/* AUTOSAR Memory Mapping - start section for code */
#define LINIF_START_SEC_CODE
#include <LinIf_MemMap.h>
#endif /* TS_MERGED_COMPILE */

STATIC FUNC(Std_ReturnType, LINIF_CODE) LinIf_ProvideSlaveResponse
(
  NetworkHandleType Channel,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) dataPtr
);

STATIC FUNC(void, LINIF_CODE) LinIf_BldFrame_PosRespGeneric
(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) dataPtr,
  uint8 Nad,
  uint8 RSID
);

STATIC FUNC(void, LINIF_CODE) LinIf_BldFrame_RI_NegativeResp
(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) dataPtr,
  uint8 Nad
);

STATIC FUNC(void, LINIF_CODE) LinIf_BldFrame_RI_ProdInd
(
  NetworkHandleType Channel,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) dataPtr
);

STATIC FUNC(void, LINIF_CODE) LinIf_BldFrame_RI_SerNum
(
  NetworkHandleType Channel,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) dataPtr
);

STATIC FUNC(boolean, LINIF_CODE) LinIf_FindTxEvtTriggered
(
  NetworkHandleType Channel,
  P2VAR(uint16, AUTOMATIC, AUTOMATIC) PidIdx,
  /* Deviation MISRAC2012-1 */
  P2VAR(P2CONST(LinIfFrameInfoType, AUTOMATIC, LINIF_APPL_DATA), AUTOMATIC, AUTOMATIC) ppFrameInfo
);

STATIC FUNC(Std_ReturnType, LINIF_CODE) LinIf_HandlePidHeader
(
  NetworkHandleType Channel,
  P2VAR(uint16, AUTOMATIC, AUTOMATIC) FrameIndex,
  P2VAR(Lin_FrameResponseType, AUTOMATIC, AUTOMATIC) Response,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) DLFlags,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) SduPtr
);

#if (TS_MERGED_COMPILE == STD_OFF)
/* AUTOSAR Memory Mapping - end section for code */
#define LINIF_STOP_SEC_CODE
#include <LinIf_MemMap.h>
#endif /* TS_MERGED_COMPILE */
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

FUNC(Std_ReturnType, LINIF_CODE) LinIf_HeaderIndication
(
  NetworkHandleType Channel,
  P2VAR(Lin_PduType, AUTOMATIC, AUTOMATIC) PduPtr
)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_LINIF_HEADERINDICATION_ENTRY(Channel, PduPtr);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
  /* check DET errors first */
  if(LINIF_UNINIT_ACCESS)
  {
    /* !LINKSTO LinIf.EB.UnInit.HeaderIndication,1,
                LinIf.ASR20-11.SWS_LinIf_00376_1.LINIF_E_UNINIT,1
     */
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
                          LINIF_API_HEADERIND, LINIF_E_UNINIT);
  }
  else if ((Channel < LinIf_ConfigPtr->NumChannelsMaster)
       ||  (Channel >= (LinIf_ConfigPtr->NumChannelsMaster + LinIf_ConfigPtr->NumSlaveChannels)))
  {
    /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00843,1 */
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
                          LINIF_API_HEADERIND, LINIF_E_NONEXISTENT_CHANNEL);
  }
  else if (NULL_PTR == PduPtr)
  {
    /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00844,1 */
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
                          LINIF_API_HEADERIND, LINIF_E_PARAM_POINTER);
  }
  else
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */
  {
    CONSTP2VAR(LinIf_ChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannel =
      &LinIf_ChannelInfo[Channel];

    /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00189,1 */
    if(pIfChannel->cFSM == LINIF_CHANNEL_OPERATIONAL)
    {
      CONSTP2CONST(LinIfSlaveChannelType, AUTOMATIC, LINIF_APPL_DATA) pIfChConfSlave =
        LINIF_CFG_GET_CHANNEL_SLAVE(Channel);

      CONSTP2VAR(LinIf_SlaveChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannelSlave =
        LINIF_GET_CHANNEL_SLAVE(Channel);

      Lin_FrameResponseType response = LINIF_LIN_RESP_S2S_IGN;
      uint8 DLFlags = 0U;

      /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00754.HeaderIndication,1 */
      LinIf_StartTimerBI(pIfChConfSlave, pIfChannelSlave);

      /* Confirm wakeup */
      {
        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00761,1 */
        if((pIfChannel->SleepHandling & LINIF_WAKEUP_CONFIRMED_FLAG) == LINIF_WAKEUP_CONFIRMED_FLAG)
        {
          LINIF_CFG_GET_UL_WUCONF_FPTR(Channel)(LINIF_GET_COMM_CHANNEL(Channel), TRUE);
          pIfChannel->SleepHandling &= ((uint8)~LINIF_WAKEUP_CONFIRMED_FLAG);
        }
      }

      if(pIfChannel->LastHeaderType != LINIF_FRM_EMPTY)
      {
        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00846,1,
                    LinIf.ASR20-11.SWS_LinIf_00847.ErrorReport,1
         */
#if (LINIF_RUNTIME_ERROR_REPORTING == STD_ON)
        (void)Det_ReportRuntimeError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
                                     LINIF_API_HEADERIND, LINIF_E_RESPONSE);
#endif /* LINIF_RUNTIME_ERROR_REPORTING == STD_ON */

        LINIF_LIN_ERROR_NOTIFY(Channel, LIN_NOT_OK);
      }

      /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00847.ProcessFrame,1 */
      if(LINIF_PID_SRF == PduPtr->Pid)
      {
        /* SRF */
        if(NULL_PTR != PduPtr->SduPtr)
        {
          if(E_OK == LinIf_ProvideSlaveResponse(Channel, PduPtr->SduPtr))
          {
            pIfChannel->FrameType = LINIF_FRM_SRF;
            pIfChannel->LastHeaderType = LINIF_FRM_SRF;
            /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00739.Cs,1 */
            PduPtr->Cs = LIN_CLASSIC_CS;
            PduPtr->Drc = LINIF_LIN_RESP_MR_TX;
            /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00739.Dl,1 */
            PduPtr->Dl = 8U;
            Result = E_OK;
          }
        }
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
        else
        {
          /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00845,1 */
          (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
                                LINIF_API_HEADERIND, LINIF_E_PARAM_POINTER);
        }
#endif /* LINIF_DEV_ERROR_DETECT == STD_ON */
      }
      else if(LINIF_PID_MRF == PduPtr->Pid)
      {
        /* MRF */
        pIfChannel->FrameType = LINIF_FRM_MRF;
        pIfChannel->LastHeaderType = LINIF_FRM_MRF;
        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00739.Cs,1 */
        PduPtr->Cs = LIN_CLASSIC_CS;
        PduPtr->Drc = LINIF_LIN_RESP_SL_RX;
        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00739.Dl,1 */
        PduPtr->Dl = 8U;
        Result = E_OK;
      }
      else
      {
        uint16 i;
        for(i = pIfChConfSlave->FrameStartInd; i < (pIfChConfSlave->FrameStartInd + pIfChConfSlave->FrameNumAssoc); i++)
        {
          if(LinIf_PidTable[i] == PduPtr->Pid)
          {
            break;
          }
        }

        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00748.E_NOT_OK,1 */
        if(i < (pIfChConfSlave->FrameStartInd + pIfChConfSlave->FrameNumAssoc))
        {
          Result = LinIf_HandlePidHeader(Channel, &i, &response, &DLFlags, PduPtr->SduPtr);

          if(E_OK == Result)
          {
            /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00733.Cs,1,
                        LinIf.ASR20-11.SWS_LinIf_00739.Cs,1
             */
            PduPtr->Cs = (((DLFlags & LINIF_CS_MASK) != 0U)
                         ? (Lin_FrameCsModelType)LIN_ENHANCED_CS
                         : (Lin_FrameCsModelType)LIN_CLASSIC_CS
                         );
            /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00733.Dl,1,
                        LinIf.ASR20-11.SWS_LinIf_00739.Dl,1
             */
            PduPtr->Dl = DLFlags & LINIF_DL_MASK;
            /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00733.Drc.PreAR44,1,
                        LinIf.ASR20-11.SWS_LinIf_00733.Drc.PostAR44,1,
                        LinIf.ASR20-11.SWS_LinIf_00739.Drc.PreAR44,1,
                        LinIf.ASR20-11.SWS_LinIf_00739.Drc.PostAR44,1
             */
            PduPtr->Drc = response;
            pIfChannel->LastDl = PduPtr->Dl;
            pIfChannel->LastId = LINIF_CFG_GET_SLAVEFRTYPE[i].LinIfFrameIdx;
            pIfChannel->ThisId = LINIF_CFG_GET_SLAVEFRTYPE[i].LinIfFrameIdx;
#if(LINIF_DEM_EVENTS_CONFIGURED)
            pIfChannel->LastDemEventsIndex = LINIF_CFG_GET_SLAVEFRTYPE[i].DemEventId;
#endif /* LINIF_DEM_EVENTS_CONFIGURED */

            /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00763,1 */
            if(pIfChConfSlave->IdxErrRespSig == i)
            {
              TS_AtomicSetBit_8(&pIfChannelSlave->Flags, LINIF_BIT_FLG_RESP_ERR_TX);
            }

#if (LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON)
            pIfChannel->FrameInfo.Pid = LinIf_PidTable[i];

            if(LINIF_LIN_RESP_MR_TX == response)
            {
              pIfChannel->FrameInfo.FrameLength = pIfChannel->LastDl;

              {
                uint8 idx;
                for(idx = 0U; idx < pIfChannel->LastDl; idx++)
                {
                  pIfChannel->FrameInfo.FrameData[idx] = PduPtr->SduPtr[idx];
                }
              }
            }
#endif /* LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON */
          }
        } /* pid found */
      }

      if(E_OK != Result)
      {
        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00748.PreAR44,1,
                    LinIf.ASR20-11.SWS_LinIf_00748.PostAR44,1,
                    LinIf.ASR20-11.SWS_LinIf_00740.PreAR44,1,
                    LinIf.ASR20-11.SWS_LinIf_00740.PostAR44,1
         */
        PduPtr->Drc = LINIF_LIN_RESP_S2S_IGN;
        PduPtr->Dl = 0U;
        pIfChannel->LastHeaderType = LINIF_FRM_EMPTY;
        pIfChannel->FrameType = LINIF_FRM_EMPTY;
      }
    }
  }

  DBG_LINIF_HEADERINDICATION_EXIT(Result, Channel, PduPtr);

  return Result;
}

/*==================[internal function definitions]=========================*/
STATIC FUNC(Std_ReturnType, LINIF_CODE) LinIf_ProvideSlaveResponse
(
  NetworkHandleType Channel,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) dataPtr
)
{
  CONSTP2VAR(LinIf_SlaveChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannelSlave =
    LINIF_GET_CHANNEL_SLAVE(Channel);

  Std_ReturnType Result = E_NOT_OK;
  uint8 flags;

  TS_AtomicAssign8(flags, pIfChannelSlave->Flags);

  /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00775,1,
              LinIf.ASR20-11.SWS_LinIf_00791,1
   */
  switch(pIfChannelSlave->RSID)
  {
    case LINIF_ASSIGN_NAD_RSID:
    {
      /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00779_2,1,
                  LinIf.ASR20-11.SWS_LinIf_00780,1
       */
      LinIf_BldFrame_PosRespGeneric(dataPtr, LINIF_CFG_GET_CHANNEL_SLAVE(Channel)->NCInitNad, LINIF_ASSIGN_NAD_RSID);
      Result = E_OK;
      break;
    }
    case LINIF_READ_ID_RSID:
    {
      if((flags & LINIF_FLG_READ_BY_ID_1) != LINIF_FLG_READ_BY_ID_1)
      {
        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00783,1 */
        LinIf_BldFrame_RI_ProdInd(Channel, dataPtr);
        Result = E_OK;
      }
      else
      {
        LinIf_BldFrame_RI_SerNum(Channel, dataPtr);
        Result = E_OK;
      }
      break;
    }
#if(LINIF_NC_SAVECONF_SUPPORTED == STD_ON)
    case LINIF_SAVECONF_RSID:
    {
      /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00782_2,1,
                  LinIf.ASR20-11.SWS_LinIf_00780,1
       */
      LinIf_BldFrame_PosRespGeneric(dataPtr, pIfChannelSlave->ConfNad, LINIF_SAVECONF_RSID);
      Result = E_OK;
      break;
    }
#endif /* LINIF_NC_SAVECONF_SUPPORTED == STD_ON */
    case LINIF_ASSIGN_FRMIDRNG_RSID:
    {
      /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00781_2,1,
                  LinIf.ASR20-11.SWS_LinIf_00780,1
       */
      LinIf_BldFrame_PosRespGeneric(dataPtr, pIfChannelSlave->ConfNad, LINIF_ASSIGN_FRMIDRNG_RSID);
      Result = E_OK;
      break;
    }
    case LINIF_STANDARD_RSID_NEGATIVE_RESPONSE:
    {
      /* Used only for bit timing */
      /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00841,1 */
      LinIf_BldFrame_RI_NegativeResp(dataPtr, pIfChannelSlave->ConfNad);
      Result = E_OK;
      break;
    }
    default:
    {
#if (LINIF_TP_SUPPORTED == STD_ON)
      /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00776,1 */
      PduInfoType pduInfo;

      pduInfo.SduLength = 8U;
      pduInfo.SduDataPtr = dataPtr;

      LinTp_SlavePhysTransmitHandling(Channel);

      /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00672.Slave,1 */
      Result = LinTp_TriggerTransmit(Channel,&pduInfo);
      if(E_OK == Result)
      {
        pIfChannelSlave->RSID = LINIF_RSID_FWD_TP;
      }
#endif /* LINIF_TP_SUPPORTED == STD_ON */
      break;
    }
  }

  return Result;
}

STATIC FUNC(void, LINIF_CODE) LinIf_BldFrame_PosRespGeneric
(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) dataPtr,
  uint8 Nad,
  uint8 RSID
)
{
  uint8 i;

  dataPtr[LINIF_PDU_BYTE_NAD] = Nad;
  dataPtr[LINIF_PDU_BYTE_PCI] = 0x01U;
  dataPtr[LINIF_PDU_BYTE_SID] = RSID;

  for(i = 3U; i < 8U; i++)
  {
    dataPtr[i] = LINIF_UNUSED_DATA;
  }
}

STATIC FUNC(void, LINIF_CODE) LinIf_BldFrame_RI_NegativeResp
(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) dataPtr,
  uint8 Nad
)
{
  dataPtr[LINIF_PDU_BYTE_NAD] = Nad;
  dataPtr[LINIF_PDU_BYTE_PCI] = 0x03U;
  dataPtr[LINIF_PDU_BYTE_SID] = LINIF_STANDARD_RSID_NEGATIVE_RESPONSE;
  dataPtr[LINIF_PDU_BYTE_SID_ECHO] = 0xB2;
  dataPtr[4U] = 0x12U;
  dataPtr[5U] = LINIF_UNUSED_DATA;
  dataPtr[6U] = LINIF_UNUSED_DATA;
  dataPtr[7U] = LINIF_UNUSED_DATA;
}

STATIC FUNC(void, LINIF_CODE) LinIf_BldFrame_RI_ProdInd
(
  NetworkHandleType Channel,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) dataPtr
)
{
  CONSTP2CONST(LinIfSlaveChannelType, AUTOMATIC, LINIF_VAR) pIfChConfSlave =
    LINIF_CFG_GET_CHANNEL_SLAVE(Channel);
  CONSTP2VAR(LinIf_SlaveChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannelSlave =
    LINIF_GET_CHANNEL_SLAVE(Channel);

  dataPtr[LINIF_PDU_BYTE_NAD] = pIfChannelSlave->ConfNad;
  dataPtr[LINIF_PDU_BYTE_PCI] = 0x06U;
  dataPtr[LINIF_PDU_BYTE_SID] = LINIF_READ_ID_RSID;
  /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00769.SupplierID,1 */
  dataPtr[3U] = pIfChConfSlave->NCSupIdB0;
  dataPtr[4U] = pIfChConfSlave->NCSupIdB1;
  /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00769.FunctionID,1 */
  dataPtr[5U] = pIfChConfSlave->NCFuncIdB0;
  dataPtr[6U] = pIfChConfSlave->NCFuncIdB1;
  /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00769.VariantID,1 */
  dataPtr[7U] = pIfChConfSlave->NCVarId;
}

STATIC FUNC(void, LINIF_CODE) LinIf_BldFrame_RI_SerNum
(
  NetworkHandleType Channel,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) dataPtr
)
{
  CONSTP2CONST(LinIfSlaveChannelType, AUTOMATIC, LINIF_VAR) pIfChConfSlave =
    LINIF_CFG_GET_CHANNEL_SLAVE(Channel);
  CONSTP2VAR(LinIf_SlaveChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannelSlave =
    LINIF_GET_CHANNEL_SLAVE(Channel);

  dataPtr[LINIF_PDU_BYTE_NAD] = pIfChannelSlave->ConfNad;
  dataPtr[LINIF_PDU_BYTE_PCI] = 0x05U;
  dataPtr[LINIF_PDU_BYTE_SID] = LINIF_READ_ID_RSID;
  dataPtr[3U] = pIfChConfSlave->NCSerialNumB0;
  dataPtr[4U] = pIfChConfSlave->NCSerialNumB1;
  dataPtr[5U] = pIfChConfSlave->NCSerialNumB2;
  dataPtr[6U] = pIfChConfSlave->NCSerialNumB3;
  dataPtr[7U] = LINIF_UNUSED_DATA;
}

STATIC FUNC(boolean, LINIF_CODE) LinIf_FindTxEvtTriggered
(
  NetworkHandleType Channel,
  P2VAR(uint16, AUTOMATIC, AUTOMATIC) PidIdx,
  /* Deviation MISRAC2012-1 */
  P2VAR(P2CONST(LinIfFrameInfoType, AUTOMATIC, LINIF_APPL_DATA), AUTOMATIC, AUTOMATIC) ppFrameInfo
)
{
  boolean isTriggered = FALSE;
  uint16 i;

  for(i = LINIF_CFG_GET_SLAVE_SUBSTSEP(Channel)->StartIdx; i < LINIF_CFG_GET_SLAVE_SUBSTSEP(Channel)->EndIdx; i++)
  {
    /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00730,1 */
    if(LINIF_BIT_IS_SET(LinIf_FrameEventFlags, LINIF_CFG_GET_SLAVE_SUBSTFRM[i].SduId))
    {
      isTriggered = TRUE;
      break;
    }
  }

  if(isTriggered)
  {
    CONSTP2CONST(LinIfSlaveChannelType, AUTOMATIC, LINIF_APPL_DATA) pIfChConfSlave =
        LINIF_CFG_GET_CHANNEL_SLAVE(Channel);
    P2CONST(LinIf_SlaveETFrameListType, AUTOMATIC, LINIF_APPL_DATA) substFrm = &LINIF_CFG_GET_SLAVE_SUBSTFRM[i];

    *ppFrameInfo = &LINIF_CFG_GET_TXFRAME[substFrm->SduId];

    /* Find associated PID
       The loop always terminates early, the associated frame is always found,
       worst case on the last position.
     */
    /* CHECK: NOPARSE */
    for(i = pIfChConfSlave->FrameStartInd; i < (pIfChConfSlave->FrameStartInd + pIfChConfSlave->FrameNumAssoc); i++)
    /* CHECK: PARSE */
    {
      if((LINIF_CFG_GET_SLAVEFRTYPE[i].LinIfFrameType == LINIF_FRM_TX)
        && (substFrm->FrameIdx == LINIF_CFG_GET_SLAVEFRTYPE[i].LinIfFrameIdx))
      {
        *PidIdx = i;
        break;
      }
    }
  }

  return isTriggered;
}

STATIC FUNC(Std_ReturnType, LINIF_CODE) LinIf_HandlePidHeader
(
  NetworkHandleType Channel,
  P2VAR(uint16, AUTOMATIC, AUTOMATIC) FrameIndex,
  P2VAR(Lin_FrameResponseType, AUTOMATIC, AUTOMATIC) Response,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) DLFlags,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) SduPtr
)
{
  CONSTP2VAR(LinIf_ChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannel =
    &LinIf_ChannelInfo[Channel];
  Std_ReturnType Result = E_NOT_OK;

  P2CONST(LinIfFrameInfoType, AUTOMATIC, LINIF_APPL_DATA) pFrameInfo;

  switch(LINIF_CFG_GET_SLAVEFRTYPE[*FrameIndex].LinIfFrameType)
  {
    case LINIF_FRM_RX:
    {
      pIfChannel->FrameType = LINIF_FRM_RX;
      pIfChannel->LastHeaderType = LINIF_FRM_RX;
      *Response = LINIF_LIN_RESP_SL_RX;
      *DLFlags = LINIF_CFG_GET_RXFRAME[LINIF_CFG_GET_SLAVEFRTYPE[*FrameIndex].LinIfFrameIdx].DLFlags;
      Result = E_OK;
      break;
    }
    case LINIF_FRM_TX:
    {
      if(NULL_PTR != SduPtr)
      {
        PduInfoType PduInfo;

        pIfChannel->FrameType = LINIF_FRM_TX;
        pIfChannel->LastHeaderType = LINIF_FRM_TX;
        *Response = LINIF_LIN_RESP_MR_TX;
        pFrameInfo = &LINIF_CFG_GET_TXFRAME[LINIF_CFG_GET_SLAVEFRTYPE[*FrameIndex].LinIfFrameIdx];
        *DLFlags = pFrameInfo->DLFlags;

        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00738,1 */
        PduInfo.SduLength = (Lin_FrameDlType)(*DLFlags & LINIF_DL_MASK);
        PduInfo.SduDataPtr = SduPtr;

        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00740.E_NOT_OK,1 */
        Result = LINIF_CFG_GET_UL_TRGTX_FPTR(LINIF_CFG_GET_SLAVEFRTYPE[i].LinIfFrameIdx)(pFrameInfo->ULPduHandleId, &PduInfo);
      }
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
      else
      {
        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00845,1 */
        (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
                              LINIF_API_HEADERIND, LINIF_E_PARAM_POINTER);
      }
#endif /* LINIF_DEV_ERROR_DETECT == STD_ON */
      break;
    }
    case LINIF_FRM_EVENT_TRIGGERED:
    {
      if(NULL_PTR != SduPtr)
      {
        pIfChannel->FrameType = LINIF_FRM_TX;
        pIfChannel->LastHeaderType = LINIF_FRM_TX;
        *Response = LINIF_LIN_RESP_MR_TX;

        if(LinIf_FindTxEvtTriggered(Channel, FrameIndex, &pFrameInfo))
        {
          PduInfoType PduInfo;

          *DLFlags = pFrameInfo->DLFlags;

          /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00738,1 */
          PduInfo.SduLength = (Lin_FrameDlType)(*DLFlags & LINIF_DL_MASK);
          PduInfo.SduDataPtr = SduPtr;

          /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00740.E_NOT_OK,1 */
          Result = LINIF_CFG_GET_UL_TRGTX_FPTR(LINIF_CFG_GET_SLAVEFRTYPE[i].LinIfFrameIdx)(pFrameInfo->ULPduHandleId, &PduInfo);
          /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00731,1 */
          PduInfo.SduDataPtr[0U] = LinIf_PidTable[*FrameIndex];
        }
      }
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
      else
      {
        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00845,1 */
        (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
                              LINIF_API_HEADERIND, LINIF_E_PARAM_POINTER);
      }
#endif /* LINIF_DEV_ERROR_DETECT == STD_ON */
      break;
    }
    /* CHECK: NOPARSE */
    default:
    {
    /*should not happen*/
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    LINIF_UNREACHABLE_CODE_ASSERT(LINIF_API_HEADERIND);
#endif /* LINIF_DEV_ERROR_DETECT == STD_ON */
    break;
    /* CHECK: PARSE */
    }
  }

  return Result;
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
/* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00863.Disabled,1 */
#endif /* LINIF_SLAVE_SUPPORTED == STD_ON */

/*==================[end of file]===========================================*/
