/**
 * \file
 *
 * \brief AUTOSAR DoIP
 *
 * This file contains the implementation of the AUTOSAR
 * module DoIP.
 *
 * \version 1.1.21
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[includes]==============================================*/
#include <DoIP.h> /* Module public header file */
#include <DoIP_Int.h> /* Module internal header file */
#include <DoIP_Trace.h> /* Debug and trace */
#include <DoIP_CustomPayloadType.h> /* Unit header file */
#include <TSMem.h>

#include <PduR_DoIP.h>
#include <SoAd.h>
#include <SchM_DoIP.h>

#if (DOIP_CUSTOM_PAYLOAD_TYPE_ENABLED == STD_ON)
#include <EcuC.h>
#endif /* DOIP_CUSTOM_PAYLOAD_TYPE_ENABLED == STD_ON */

#if(DOIP_DEV_ERROR_DETECT == STD_ON)
#include <Det.h> /* DET support */
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#define DOIP_START_SEC_CODE
#include <DoIP_MemMap.h>
#if (DOIP_CUSTOM_PAYLOAD_TYPE_ENABLED == STD_ON)
/* !LINKSTO DoIP.dsn.Func0117.RxCustomMsg.EcuC_SetMetaData, 1 */
/* !LINKSTO DoIP.dsn.Func0117.RxCustomMsg.PduR_StartOfReception, 1 */
FUNC(void, DOIP_CODE) DoIP_HandleCustomMsg
(
  DoIP_TcpConIdxType TcpConIdx,
  uint16 PayloadType,
  uint32 PayloadLength
)
{
  CONSTP2VAR(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr = &DoIP_TcpCon[TcpConIdx];
  SoAd_SoConIdType SoConId = SOAD_INVALID_SOCON_ID;
  Std_ReturnType RetVal;
  BufReq_ReturnType BufReq_Ret = BUFREQ_E_NOT_OK;
  const PduIdType DoIPSoAdPduId = TcpConPtr->DoIPSoAdPduId;

  DBG_DOIP_HANDLECUSTOMMSG_ENTRY(TcpConIdx, PayloadType, PayloadLength);

  TcpConPtr->RxDiagMsgLength = PayloadLength;

  RetVal = SoAd_GetSoConId(DOIP_PBCFG_TCPCON(DoIPSoAdPduId).SoAdTxPduId, &SoConId);

  if(RetVal == E_OK)
  {
    PduIdType Idx;
    boolean Found = FALSE;
    for(Idx = 0U; (Idx < DoIP_RootPtr->CustomChannelMax) && (Found == FALSE); Idx++)
    {
      if(DoIPSoAdPduId == DOIP_PBCFG_CUSTOMCHANNEL(Idx).DoIPSoAdPduId)
      {
        PduIdType DoIPPduRPduId = DOIP_PBCFG_CUSTOMCHANNEL(Idx).DoIPPduRRxPduId;
        PduLengthType BufferSize;
        PduInfoType PduInfo;

        Found = TRUE;

        RetVal |= EcuC_SetMetaDataPayloadType(
                                               DOIP_PBCFG_CUSTOMCHANNEL(Idx).ECUC_RxPduId,
                                               &PduInfo,
                                               PayloadType
                                             );
        RetVal |= EcuC_SetMetaDataSoConId(
                                           DOIP_PBCFG_CUSTOMCHANNEL(Idx).ECUC_RxPduId,
                                           &PduInfo,
                                           SoConId
                                         );

        if(RetVal == E_OK)
        {
          BufReq_Ret = PduR_DoIPStartOfReception(
                                                  DOIP_PBCFG_PDURRXPDUID(DoIPPduRPduId),
                                                  (PduLengthType) TcpConPtr->RxDiagMsgLength,
                                                  &BufferSize
                                                );

          if(BufReq_Ret == BUFREQ_OK)
          {
            TcpConPtr->RxBufferSize = DOIP_MIN(TcpConPtr->RxDiagMsgLength, BufferSize);
            /* No diagnostic header is analyzed for custom payload type messages ->
               go to payload forwarding. */
            TS_AtomicAssign8(TcpConPtr->RxState, DOIP_TCPCON_RX_CUSTOM_MSG_PAYLOAD);
            TcpConPtr->DoIPPduRRxPduId = DoIPPduRPduId;
          }
        }
      }
    }
  }

  if((RetVal != E_OK) || (BufReq_Ret != BUFREQ_OK))
  {
    /* Meta data handling failed, or PduR is not able to receive the message -> consume data
       from SoAd. */
    TS_AtomicAssign8(DoIP_TcpCon[TcpConIdx].RxState, DOIP_TCPCON_RX_DISCARD_DIAG_MSG);

    TcpConPtr->RxBufferSize = TcpConPtr->RxDiagMsgLength;

    /* No NACK is sent for custom payload type messages. */
    TS_AtomicAssign8(DoIP_TcpCon[TcpConIdx].TxRespCode, DOIP_DO_NOT_SEND_RESP);
    BufReq_Ret = BUFREQ_OK;
  }

  DBG_DOIP_HANDLECUSTOMMSG_EXIT(TcpConIdx, PayloadType, PayloadLength);
}

/* !LINKSTO DoIP.dsn.Func0117.TxCustomMsg.SoAdCopyTxData, 2 */
FUNC(void, DOIP_CODE) DoIP_FillCustomMsgHdr
(
  DoIP_TcpConIdxType TcpConIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) SduDataPtr
)
{
  const PduIdType DoIPPduRPduId = DoIP_TcpCon[TcpConIdx].DoIPPduRTxPduId;

  DBG_DOIP_FILLCUSTOMMSGHDR_ENTRY(TcpConIdx, SduDataPtr);

  DoIP_Fill_GenHeader(
                       SduDataPtr,
                       DoIP_Channel[DoIPPduRPduId].TxDiagMsgLength,
                       DoIP_Channel[DoIPPduRPduId].PayloadType
                     );

  DBG_DOIP_FILLCUSTOMMSGHDR_EXIT(TcpConIdx, SduDataPtr);

}

FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_FillCustomMsgPld
(
  DoIP_TcpConIdxType TcpConIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_DATA) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_DATA) AvailableDataPtr
)
{
  BufReq_ReturnType BufReq_Ret;
  CONSTP2VAR(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr = &DoIP_TcpCon[TcpConIdx];
  PduIdType DoIPPduRPduId = TcpConPtr->DoIPPduRTxPduId;

  DBG_DOIP_FILLCUSTOMMSGPLD_ENTRY(TcpConIdx, PduInfoPtr, AvailableDataPtr);

  BufReq_Ret = PduR_DoIPCopyTxData(
                                    DOIP_PBCFG_PDURTXPDUID(DoIPPduRPduId),
                                    PduInfoPtr,
                                    NULL_PTR,
                                    AvailableDataPtr
                                  );
  if(BUFREQ_OK == BufReq_Ret)
  {
    /* All requested data copied -> reduce the remaining message size. */
    DoIP_Channel[DoIPPduRPduId].TxDiagMsgLength -= PduInfoPtr->SduLength;

    /* Reset timeout */
    TS_AtomicAssign8(TcpConPtr->ResetGeneralInactivityTimer, TRUE);

    /* Function is called to copy data (and not only to provide data size) ->
      * headers are already copied. */
    if(PduInfoPtr->SduDataPtr != NULL_PTR)
    {
      TS_AtomicAssign8(TcpConPtr->TxState, DOIP_TCPCON_TX_CUSTOM_MSG);
    }
  }

  DBG_DOIP_FILLCUSTOMMSGPLD_EXIT(BufReq_Ret, TcpConIdx, PduInfoPtr, AvailableDataPtr);

  return BufReq_Ret;
}

/* !LINKSTO DoIP.dsn.Func0117.RxCustomMsg.PduR_CopyRxData, 1 */
/* !LINKSTO DoIP.dsn.Func0117.RxCustomMsg.PduR_TpRxIndication_OK, 2 */
/* !LINKSTO DoIP.dsn.Func0117.RxCustomMsg.PduR_TpRxIndication_NOT_OK, 2 */
FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_CopyCustomMsgPayload
(
  DoIP_TcpConIdxType TcpConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_DATA) PduInfoPtr
)
{
  CONSTP2VAR(DoIP_TcpConType, AUTOMATIC, DOIP_APPL_DATA) TcpConPtr = &DoIP_TcpCon[TcpConIdx];
  BufReq_ReturnType BufReq_Ret = BUFREQ_E_NOT_OK;

  DBG_DOIP_COPYCUSTOMMSGPAYLOAD_ENTRY(TcpConIdx, PduInfoPtr);

  if(PduInfoPtr->SduLength <= TcpConPtr->RxDiagMsgLength)
  {
    const PduIdType DoIPPduRPduId = TcpConPtr->DoIPPduRRxPduId;
    PduLengthType BufferSize;

    BufReq_Ret = PduR_DoIPCopyRxData(
                                      DOIP_PBCFG_PDURRXPDUID(DoIPPduRPduId),
                                      PduInfoPtr,
                                      &BufferSize
                                    );

    TcpConPtr->RxDiagMsgLength -= PduInfoPtr->SduLength;

    if(BufReq_Ret == BUFREQ_OK)
    {
      if(0U == TcpConPtr->RxDiagMsgLength)
      {
        TcpConPtr->RxBufferSize = DOIP_GEN_HEADER_SIZE;

        /* General inactivity timer is reset upon reception of a valid custom message. */
        TS_AtomicAssign8(TcpConPtr->ResetGeneralInactivityTimer, TRUE);

        /* No positive acknowledge is sent for custom payload type messages. */
        TS_AtomicAssign8(TcpConPtr->RxState, DOIP_TCPCON_RX_READY);

        PduR_DoIPTpRxIndication(DOIP_PBCFG_PDURRXPDUID(DoIPPduRPduId), NTFRSLT_OK);
      }
      else
      {
        TcpConPtr->RxBufferSize = DOIP_MIN(TcpConPtr->RxDiagMsgLength, BufferSize);
      }
    }
    else
    {
      if(0U == TcpConPtr->RxDiagMsgLength)
      {
        /* The complete message has been consumed from SoAd. */
        TcpConPtr->RxBufferSize = DOIP_GEN_HEADER_SIZE;
        TcpConPtr->RxState = DOIP_TCPCON_RX_READY;
      }
      else
      {
        /* Part of the message is still in SoAd, consume it to release SoAd buffers. */
        TS_AtomicAssign8(TcpConPtr->RxState, DOIP_TCPCON_RX_DISCARD_DIAG_MSG);

        /* No NACK is sent for custom payload type messages. */
        TS_AtomicAssign8(TcpConPtr->TxRespCode, DOIP_DO_NOT_SEND_RESP);

        TcpConPtr->RxBufferSize = TcpConPtr->RxDiagMsgLength;
      }

      PduR_DoIPTpRxIndication(DOIP_PBCFG_PDURRXPDUID(DoIPPduRPduId), NTFRSLT_E_NOT_OK);

      BufReq_Ret = BUFREQ_OK;
    }
  }
  /* else - return BUFREQ_E_NOT_OK */

  DBG_DOIP_COPYCUSTOMMSGPAYLOAD_EXIT(BufReq_Ret, TcpConIdx, PduInfoPtr);

  return BufReq_Ret;
}

/* !LINKSTO DoIP.dsn.Func0117.CustomChannel.ChannelActivation, 1 */
FUNC(void, DOIP_CODE) DoIP_PrepareTcpConCustom
(
  DoIP_TcpConIdxType TcpConIdx,
  PduIdType DoIPSoAdPduId
)
{
  /* Each Tcp connection has assigned custom payload type channel (when feature is enabled).
   * Find respective custom channel it and assign Tcp connection to it. */
  PduIdType Idx;
  boolean Found = FALSE;

  DBG_DOIP_PREPARETCPCONCUSTOM_ENTRY(TcpConIdx, DoIPSoAdPduId);

  for(Idx = 0U; (Idx < DoIP_RootPtr->CustomChannelMax) && (Found == FALSE); Idx++)
  {
    if(DoIPSoAdPduId == DOIP_PBCFG_CUSTOMCHANNEL(Idx).DoIPSoAdPduId)
    {
      const PduIdType DoIPPduRPduId = Idx + DoIP_RootPtr->StdChannelMax;
      DoIP_Channel[DoIPPduRPduId].TcpConIdx = TcpConIdx;

      DoIP_Channel[DoIPPduRPduId].RoutingActivationState = DOIP_ROUTING_ACTIVE;
      Found = TRUE;
    }
  }

  /* There is 1:1 relationship between Tcp connections and custom payload type channels. */
  DOIP_POSTCONDITION_ASSERT(Found == TRUE, DOIP_SID_SOADTPSTARTOFRECEPTION);

  DBG_DOIP_PREPARETCPCONCUSTOM_EXIT(TcpConIdx, DoIPSoAdPduId);
}

/* !LINKSTO DoIP.dsn.Func0117.CustomChannel.TcpConReset, 1 */
FUNC(void, DOIP_CODE) DoIP_ResetTcpConCustom
(
  DoIP_TcpConIdxType TcpConIdx
)
{
  PduIdType DoIPPduRPduId;
  const PduIdType CustomChannelIdxEnd = DoIP_RootPtr->StdChannelMax + DoIP_RootPtr->CustomChannelMax;
  boolean Found = FALSE;

  DBG_DOIP_RESETTCPCONCUSTOM_ENTRY(TcpConIdx);

  /* Find custom channel assigned to this Tcp connection. */
  for(
       DoIPPduRPduId = DoIP_RootPtr->StdChannelMax;
       (DoIPPduRPduId < CustomChannelIdxEnd) && (Found == FALSE);
       DoIPPduRPduId++
     )
  {
    if(DoIP_Channel[DoIPPduRPduId].TcpConIdx == TcpConIdx)
    {
      if(DoIP_Channel[DoIPPduRPduId].PendingRequest)
      {
        TS_AtomicAssign8(DoIP_Channel[DoIPPduRPduId].PendingRequest, FALSE);

        /* Report unsuccessful message transmission. */
        PduR_DoIPTpTxConfirmation(
                                   DOIP_PBCFG_PDURTXPDUID(DoIPPduRPduId),
                                   NTFRSLT_E_NOT_OK
                                 );
      }

      TS_AtomicAssign8(DoIP_Channel[DoIPPduRPduId].RoutingActivationState, DOIP_ROUTING_INACTIVE);

      if(DoIP_TcpCon[TcpConIdx].RxState == DOIP_TCPCON_RX_CUSTOM_MSG_PAYLOAD)
      {
        /* Report unsuccessful message reception. */
        PduR_DoIPTpRxIndication(DOIP_PBCFG_PDURRXPDUID(DoIPPduRPduId), NTFRSLT_E_NOT_OK);
      }
      Found = TRUE;
    }
  }

  /* Each Tcp connection needs to have custom channel assigned. */
  DOIP_POSTCONDITION_ASSERT(Found == TRUE, DOIP_SID_SOADTPCOPYRXDATA);

  DBG_DOIP_RESETTCPCONCUSTOM_EXIT(TcpConIdx);
}
#endif /* DOIP_CUSTOM_PAYLOAD_TYPE_ENABLED == STD_ON */

FUNC(void, DOIP_CODE) DoIP_ClearBuffer
(
  DoIP_TcpConIdxType TcpConIdx
)
{
  TS_AtomicAssign16(DoIP_TcpCon[TcpConIdx].ReadPos, 0U);
  TS_AtomicAssign16(DoIP_TcpCon[TcpConIdx].WritePos, 0U);
}

FUNC(void, DOIP_CODE) DoIP_ReleaseBuffer
(
  DoIP_TcpConIdxType TcpConIdx
)
{
  TS_AtomicAssign16(DoIP_TcpCon[TcpConIdx].ReadPos, 0U);
  TS_AtomicAssign16(DoIP_TcpCon[TcpConIdx].WritePos, 0U);
}

FUNC(void, DOIP_CODE) DoIP_WriteBuffer
(
  DoIP_TcpConIdxType TcpConIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) DataPtr,
  PduLengthType Size
)
{
  const PduLengthType WritePos = DoIP_TcpCon[TcpConIdx].WritePos;
  const PduLengthType AvailableSize = DOIP_TCP_CONN_BUFF_SIZE - WritePos;
  const PduLengthType CopySize = DOIP_MIN(Size, AvailableSize);

  TS_MemCpy(
             &DoIP_TcpCon[TcpConIdx].Buffer[WritePos],
             DataPtr,
             CopySize
           );

  DoIP_TcpCon[TcpConIdx].WritePos += CopySize;

}

FUNC(PduLengthType, DOIP_CODE) DoIP_ReadBuffer
(
  DoIP_TcpConIdxType TcpConIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) DataPtr,
  PduLengthType Size
)
{
  PduLengthType RemainingSize = 0U;

  const PduLengthType CopySize = DOIP_MIN(Size, DoIP_GetBufferDataSize(TcpConIdx));

  if((CopySize > 0U) && (DataPtr != NULL_PTR))
  {
    const PduLengthType ReadPos = DoIP_TcpCon[TcpConIdx].ReadPos;

    TS_MemCpy(
               DataPtr,
               &DoIP_TcpCon[TcpConIdx].Buffer[ReadPos],
               CopySize
             );

    DoIP_TcpCon[TcpConIdx].ReadPos += CopySize;
  }

  RemainingSize = DoIP_TcpCon[TcpConIdx].WritePos - DoIP_TcpCon[TcpConIdx].ReadPos;

  return RemainingSize;
}

FUNC(PduLengthType, DOIP_CODE) DoIP_GetBufferDataSize
(
  DoIP_TcpConIdxType TcpConIdx
)
{
  return DoIP_TcpCon[TcpConIdx].WritePos - DoIP_TcpCon[TcpConIdx].ReadPos;
}

FUNC_P2VAR(uint8, DOIP_APPL_DATA, DOIP_CODE) DoIP_GetBufferDataPtr
(
  DoIP_TcpConIdxType TcpConIdx
)
{
  const PduLengthType ReadPos = DoIP_TcpCon[TcpConIdx].ReadPos;

  return &DoIP_TcpCon[TcpConIdx].Buffer[ReadPos];
}

#define DOIP_STOP_SEC_CODE
#include <DoIP_MemMap.h>

/*==================[end of file]===========================================*/

