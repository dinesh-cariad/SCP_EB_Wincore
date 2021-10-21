/**
 * \file
 *
 * \brief AUTOSAR SoAd
 *
 * This file contains the implementation of the AUTOSAR
 * module SoAd.
 *
 * \version 1.8.16
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*
 * Misra-C:2012 Deviations:
 *
 * MISRAC2012-1) Deviated Rule: 11.3 (advisory)
 * A cast shall not be performed between a pointer to object type and a pointer to a different
 * object type.
 *
 * Reason:
 * In order to retrieve the maximal receive window via API function TcpIp_ChangeParamter
 * it is required to convert a unit8 pointer to uint16.
 */
/*==================[inclusions]============================================*/

#include <SoAd_02_SocketCon_Int.h> /* Unit public header file. */
#include <SoAd.h> /* Module public header file. */
#include <SoAd_Int.h> /* Module internal header file. */
#include <SchM_SoAd.h>
#if(SOAD_TLS_ENABLED == STD_ON)
#include <Tls.h>
#endif /* SOAD_TLS_ENABLED == STD_ON */

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
#include <Det.h> /* Det API */
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/
#if(TS_MERGED_COMPILE == STD_OFF)
#define SOAD_START_SEC_CODE
#include <SoAd_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_Int_02_ULSoConModeChg
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  SoAd_SoConModeType Mode
)
{
  /* each bit in ULNotifBitmap represents an upper layer that shall be notified if the bit is set */
  uint32 ULNotifBits;
  uint8_least Idx;

  DBG_SOAD_INT_02_ULSOCONMODECHG_ENTRY(SoConPtr, Mode);

  ULNotifBits = SOAD_CFG(SoCon, SoConPtr->SoConId).ULNotifBitmap;

  /* loop over all available upper layer modules as long as notifications are outstanding */
  for(Idx = 0U; (Idx < SoAd_NotificationAPISize) && (ULNotifBits != 0U); Idx++)
  {
    /* if upper layer shall be notified and
     * a valid function pointer exists ... */
    if(
        ((ULNotifBits & 1U) == 1U)
        && (SoAd_NotificationAPI[Idx].SoConModeChg != NULL_PTR)
      )
    {
      /* ... call upper layer function */
      SoAd_NotificationAPI[Idx].SoConModeChg(SoConPtr->SoConId, Mode);
    }

    /* shift uLNotifBits */
    ULNotifBits >>= 1U;
  }

  DBG_SOAD_INT_02_ULSOCONMODECHG_EXIT(SoConPtr, Mode);
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_Int_02_CloseSocket
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  boolean Abort
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  if(SoConStatePtr->SocketId != SOAD_SOCKETID_INVALID)
  {
    /* request TcpIp module to close the socket */
    RetVal = TcpIp_Close(SoConStatePtr->SocketId, Abort);

    if(RetVal == E_NOT_OK)
    {
      /* Set flag to close in the context of the next MainFunction. */
      SOAD_FLAG_SET_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TCPIP_SOCKET_CLOSED);
    }
  }
  else
  {
    /* Set flag to close in the context of the next MainFunction. */
    SOAD_FLAG_SET_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TCPIP_SOCKET_CLOSED);
  }
  /* Set global flag to know that for a certain SoCon a transition condition was met. */
  SoAd_SetBitInTransCondChange(SoConPtr);
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_Int_02_CleanUpSoCon
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  boolean RxIndication = FALSE;
  NotifResultType RetVal_Notif = NTFRSLT_E_NOT_OK;
  P2CONST(SoAd_SocketRouteType, AUTOMATIC, SOAD_APPL_DATA) SocketRoutePtr = NULL_PTR;
  P2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr = NULL_PTR;
  DBG_SOAD_INT_02_CLEANUPSOCON_ENTRY(SoConPtr, SoConStatePtr);

  SOAD_ENTER_CRITSEC();
  {
    /* check for active transmissions */
    const SoAd_PduRouteDestVirtualIdType PduRouteDestVirtualId
      = SoAd_GetActivePduRouteDestVirtualId(SoConPtr);

    /* if there is a current transmission the UL has to be notified about the cancellation or
       closure of the socket connection. This is only relevant for Tp API since for If no
       Tx confirmation shall be performed in error case. */
    if(
        (PduRouteDestVirtualId != SOAD_INVALID_DEST_VIRTUAL_ID) &&
        SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_TP_PENDING)
      )
    {
      /* PduTxData have same order as PduRouteDestVirtual. */
      P2VAR(SoAd_SoConTxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConTxDataPtr = NULL_PTR;
      PduRouteDestVirtualPtr = &SOAD_CFG(PduRouteDestVirtual, PduRouteDestVirtualId);

      SoAd_GetSoConTxData(SoConPtr, SoConTxDataPtr);

      /* reset transmission state */
      SoAd_SetPduRouteDestVirtualState(PduRouteDestVirtualPtr, SOAD_PDUROUTEDESTVIRTUALSTATE_IDLE);

      SOAD_FLAG_CLR_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_TP_PENDING);
      SoConTxDataPtr->ActivePduRouteDestVirtualId = SOAD_INVALID_DEST_VIRTUAL_ID;
    }

    /* reset all other transmission data structures */
    SoAd_InitSoConTxDataStructures(SoConPtr);

    if(SOAD_FLAG_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TCPIP_SOCKET_CLOSED))
    {
      /* Set SocketId to invalid */
      SoConStatePtr->SocketId = SOAD_SOCKETID_INVALID;
    }
    /* Assigned Local Port is not cleaned and should not be */
  }
  SOAD_EXIT_CRITSEC();

  if(NULL_PTR != PduRouteDestVirtualPtr)
  {
    SoAd_ULTpTxConfirmation(PduRouteDestVirtualPtr->PduRouteId, NTFRSLT_E_NOT_OK);
  }

  SOAD_ENTER_CRITSEC();
  if(SoConPtr->SocketRouteFirstId != SOAD_INVALID_ROUTE_ID)
  {
    SocketRoutePtr = &SOAD_CFG(SocketRoute, SoConPtr->SocketRouteFirstId);

    /* if this socket connection uses the TP-API */
    if(SOAD_FLAG(SocketRoutePtr, SOAD_SOCKETROUTE_FLAG_IS_UL_TP))
    {
      /* a final TpRxIndication shall be passed to the upper layer (in most cases)
       * it will be called outside of the exclusive area */
      RxIndication = TRUE;

      /* check for valid receive data structure */
      if(SoConPtr->RxDataId != SOAD_INVALID_SOCON_ID)
      {
        P2VAR(SoAd_SoConRxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConRxDataPtr = NULL_PTR;
        SoAd_GetSoConRxData(SoConPtr, SoConRxDataPtr);

        if(!SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_PDU_HEADER_MODE))
        {
          /* if nothing is buffered, and no PDU cancellation occurred */
          if(
              SOAD_FLAG_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RX_DATA_BUFFERED)
              || SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RX_TP_CANCELED)
              || SOAD_FLAG_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RECONNECT_SOCKET)
              || SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_TP_CANCELED)
            )
          {
            /* TCP stream was interrupted */
            RetVal_Notif = NTFRSLT_E_NOT_OK;
          }
          else /* SOAD_SOCONSTATE_FLAG_TCP_FIN_RECEIVED */
          {
            /* Streaming connection was successful */
            RetVal_Notif = NTFRSLT_OK;
          }
        }
        else
        {
          /* Note: if there is currently no active reception
           * TpRxIndication was already called */
          if(SoConRxDataPtr->CurrSocketRouteId == SOAD_INVALID_ROUTE_ID)
          {
            RxIndication = FALSE;
          }
          else
          {
            /* only the upper layer of the active reception needs to be informed */
            SocketRoutePtr = &SOAD_CFG(SocketRoute, SoConRxDataPtr->CurrSocketRouteId);
          }
        }

        /* reset all other reception data structures */
        SoAd_InitSoConRxDataStructures(SoConPtr);
        /* Clean up flag */
        SOAD_FLAG_CLR_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RX_DATA_BUFFERED);
      }
      else
      {
        /* this is a Udp socket connection without a rx buffer
         * call the upper layer and finalize reception */
        RetVal_Notif = NTFRSLT_OK;
      }
    }
  }
  SOAD_EXIT_CRITSEC();

  if(RxIndication == TRUE)
  {
    /* TP only is allowed for one Destination */
    P2CONST(SoAd_SocketRouteDestType, AUTOMATIC, SOAD_APPL_DATA) SocketRouteDestPtr =
        &SOAD_CFG(SocketRouteDest, SocketRoutePtr->SocketRouteDestFirstId);
    SoAd_ULTpRxIndication(SocketRouteDestPtr, RetVal_Notif);
  }

  DBG_SOAD_INT_02_CLEANUPSOCON_EXIT(SoConPtr, SoConStatePtr);
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, SOAD_CODE) SoAd_RequestTcpIpSocket
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(TcpIp_SocketIdType, AUTOMATIC, SOAD_APPL_DATA) SocketIdPtr
)
{
  Std_ReturnType RetVal;
  const TcpIp_ProtocolType ProtocolType = SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_TCP)
    ? (TcpIp_ProtocolType) TCPIP_IPPROTO_TCP : (TcpIp_ProtocolType) TCPIP_IPPROTO_UDP;
  const TcpIp_DomainType DomainType = SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_IPV6)
    ? (TcpIp_DomainType) TCPIP_AF_INET6 : (TcpIp_DomainType) TCPIP_AF_INET;
  TcpIp_SocketIdType SocketId;

  DBG_SOAD_REQUESTTCPIPSOCKET_ENTRY(SoConPtr, SoConStatePtr, SocketIdPtr);

  /* request a new socket from the TcpIp module */
  RetVal = TcpIp_GetSocket(
                             DomainType,
                             ProtocolType,
                             &SocketId
                           );

  if(RetVal == E_OK)
  {
    CONSTP2CONST(SoAd_SoConGroupType, AUTOMATIC, SOAD_APPL_DATA) SoConGroupPtr =
            &SOAD_CFG(SoConGroup, SoConPtr->SoConGroupId);

    /* inout parameter - if TcpIp_Bind is called with Port = 0 the TcpIp module chooses
     * the port number automatically and assigns the value to localPort */
    uint16 LocalPort = SoConGroupPtr->LocalPort;

    RetVal = TcpIp_Bind(SocketId, SoConGroupPtr->LocalIpAddrId, &LocalPort);

    if(RetVal == E_OK)
    {
      /* remember the actually assigned local port (TcpIp might have changed it)*/
      SoConStatePtr->AssignedLocalPort = LocalPort;

      /* TcpIp_GetSocket and TcpIp_Bind succeeded, update socketIdPtr */
      *SocketIdPtr = SocketId;

      /* configure Frame Priority for VLAN, if used */
      if(SoConGroupPtr->FramePriority != SOAD_INVALID_FRAME_PRIO)
      {
        uint8 FramePrio = SoConGroupPtr->FramePriority;

        /* Note: return value is ignored on purpose, there's nothing that can be done if the
                 request fails (most likely because of DET checks) */
        (void)TcpIp_ChangeParameter(SocketId, TCPIP_PARAMID_FRAMEPRIO, &FramePrio);
      }

      /* configure the TCPIP TCP socket */
      if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_TCP))
      {
        /* !LINKSTO SoAd.dsn.Func0120.Udp.AssignedLocalPort.NotOverWritten, 1 */
        /* For Listen Server set AssignedPort for the first SoConState .
         * It represents the SoConGroup and all other will inherit its local port*/
        if(!SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_TCP_INITIATE))
        {
          CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConFirstStatePtr =
            &SOAD_PBRAM(SoConState, SoConGroupPtr->SoConFirstId);
          SoConFirstStatePtr->AssignedLocalPort = LocalPort;
        }
        /* check if this socket connection uses RxBuffer(s) */
        if(SoConPtr->RxDataId != SOAD_INVALID_SOCON_ID)
        {
          /* get the size of the TpRxBuffer */
          /* Cast protected as TpCapacity can be max 65535 */
          uint16 RxWnd = (uint16)SOAD_CFG(SoConRxBufferCfg, SoConPtr->RxDataId).TpCapacity;

          if(RxWnd != 0U)
          {
            /* TcpIp expects uint16* aligned pointer within uint8* parameter pointer for
               TCPIP_PARAMID_TCP_RXWND_MAX. */
            P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) RxWindowPtr =
              (P2VAR(uint8, TYPEDEF, SOAD_APPL_DATA))
              &RxWnd;
            /* If a TpRxBuffer is used, set the TcpIp receive window to the size of the buffer. */
            /* Note: return value is ignored on purpose, there's nothing that can be done if the
                     request fails (most likely because of DET checks) */
            (void)TcpIp_ChangeParameter(
                                         SocketId,
                                         TCPIP_PARAMID_TCP_RXWND_MAX,
                                         RxWindowPtr
                                       );
          }
        }
        if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_SOCKET_TCP_KEEP_ALIVE))
        {
          boolean KeepAlive = TRUE;
          uint32 KeepAliveTime = SoConGroupPtr->SocketTcpKeepAliveTime;
          uint32 KeepAliveInterval = SoConGroupPtr->SocketTcpKeepAliveInterval;
          uint8 KeepAliveProbesMax = SoConGroupPtr->SocketTcpKeepAliveProbesMax;

          (void)TcpIp_ChangeParameter(
                                       SocketId,
                                       TCPIP_PARAMID_TCP_KEEPALIVE,
                                       (uint8 *)&KeepAlive
                                     );
          /* If SocketTcpKeepAliveTime is not enabled, it will hold the value 0U */
          if(KeepAliveTime != 0U)
          {
            (void)TcpIp_ChangeParameter(
                                         SocketId,
                                         TCPIP_PARAMID_TCP_KEEPALIVE_TIME,
                                         /* Deviation MISRAC2012-1 */
                                         (uint8 *)&KeepAliveTime
                                       );
          }
          /* If SocketTcpKeepAliveProbesMax is not enabled, it will hold the value 0U */
          if(KeepAliveProbesMax != 0U)
          {
            (void)TcpIp_ChangeParameter(
                                         SocketId,
                                         TCPIP_PARAMID_TCP_KEEPALIVE_PROBES_MAX,
                                         (uint8 *)&KeepAliveProbesMax
                                       );
          }
          /* If SocketTcpKeepAliveInterval is not enabled, it will hold the value 0U */
          if(KeepAliveInterval != 0U)
          {
            (void)TcpIp_ChangeParameter(
                                         SocketId,
                                         TCPIP_PARAMID_TCP_KEEPALIVE_INTERVAL,
                                         /* Deviation MISRAC2012-1 */
                                         (uint8 *)&KeepAliveInterval
                                       );
          }
        }
#if (SOAD_TLS_ENABLED == STD_ON)
        if(SoConPtr->TlsConnectionId != SOAD_TLS_INVALID_CONNECTION_ID)
        {
          if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_TCP_INITIATE))
          {
            Tls_ConnectionIdType TlsId = (Tls_ConnectionIdType) SoConPtr->TlsConnectionId;

            /* Note: return value is ignored on purpose, there's nothing that can be done if the
               request fails (most likely because of DET checks) */
            (void)TcpIp_ChangeParameter(
                                         SocketId,
                                         TCPIP_PARAMID_TLS_CONNECTION_ID,
                                         &TlsId
                                       );
          }
        }
#endif /* SOAD_TLS_ENABLED == STD_ON */
      }
      /* UDP */
      else
      {
        /* !LINKSTO SoAd.dsn.Func0120.Udp.AssignedLocalPort.NotOverWritten, 1 */
        /* For UDP mirror AssignedPort for the first SoConState .
         * It represents the SoConGroup and all other will inherit its local port*/
        if(SoConPtr->SoConId != SoConGroupPtr->SoConFirstId)
        {
          CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConFirstStatePtr =
            &SOAD_PBRAM(SoConState, SoConGroupPtr->SoConFirstId);
          SoConFirstStatePtr->AssignedLocalPort = LocalPort;
        }
#if (SOAD_TLS_ENABLED == STD_ON)

        if(SoConPtr->TlsConnectionId != SOAD_TLS_INVALID_CONNECTION_ID)
        {
          Tls_ConnectionIdType TlsId = (Tls_ConnectionIdType) SoConPtr->TlsConnectionId;

          /* Note: return value is ignored on purpose, there's nothing that can be done if the
             request fails (most likely because of DET checks) */
          (void)TcpIp_ChangeParameter(
                                       SocketId,
                                       TCPIP_PARAMID_TLS_CONNECTION_ID,
                                       &TlsId
                                     );
        }

#endif /* SOAD_TLS_ENABLED == STD_ON */
      }
    }
    else
    {
      /* TcpIp_Bind failed, maybe the local ip address and port are already in use
       * the socket connection can't be opened, the TcpIp socket must be released again */
      (void)TcpIp_Close(SocketId, TRUE);

      /* Precondition to enter state RESTART to repeat the connecting process */
      SOAD_FLAG_SET_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TCPIP_SOCKET_CLOSED);

      /* Set global flag to know that for a certain SoCon a transition condition was met. */
      SoAd_SetBitInTransCondChange(SoConPtr);
    }
  }
  else
  {
    /* Precondition to enter state RESTART to repeat the connecting process */
    SOAD_FLAG_SET_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TCPIP_SOCKET_CLOSED);

    /* Update TXING_OK_State */
    SOAD_FLAG_CLR_UP(SoConStatePtr,SOAD_SOCONSTATE_TXING_OK);

    /* Set global flag to know that for a certain SoCon a transition condition was met. */
    SoAd_SetBitInTransCondChange(SoConPtr);
  }

  DBG_SOAD_REQUESTTCPIPSOCKET_EXIT(RetVal, SoConPtr, SoConStatePtr, SocketIdPtr);
  return RetVal;
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_InitSoConTxDataStructures
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr
)
{

  DBG_SOAD_INITSOCONTXDATASTRUCTURES_ENTRY(SoConPtr);

  if(SoConPtr->TxDataId != SOAD_INVALID_SOCON_ID)
  {
    P2VAR(SoAd_SoConTxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConTxDataPtr = NULL_PTR;
    P2VAR(SoAd_PduTxTcpDataType, AUTOMATIC, SOAD_APPL_DATA) PduTxTcpDataPtr = NULL_PTR;
    P2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr;
    SoAd_PduTxDataIdType PduTxDataId;
    const SoAd_PduTxDataIdType PduTxDataFirstId = SoConPtr->PduTxDataFirstId;
    const SoAd_PduTxDataIdType MaxPduTxDataId = PduTxDataFirstId + SoConPtr->PduTxDataCnt;

    SoAd_GetSoConTxData(SoConPtr, SoConTxDataPtr);

    /* initialize socket connection specific transmission variables */
    SoConTxDataPtr->ULIfDataPtr = NULL_PTR;
    SoConTxDataPtr->TcpIpTxRemainingLen = 0U;
    SoConTxDataPtr->PduPayloadRemainingLen = 0U;
    SoConTxDataPtr->PduHeaderWrittenLen = 0U;
    SoConTxDataPtr->ActivePduRouteDestVirtualId = SOAD_INVALID_DEST_VIRTUAL_ID;
    if(SoConPtr->TxBufferId != SOAD_INVALID_SOCON_ID)
    {
      P2VAR(SoAd_SoConTxBufferStateType, AUTOMATIC, SOAD_APPL_DATA) TxBufferStatePtr = NULL_PTR;

      SoAd_GetSoConTxBufferState(SoConPtr,TxBufferStatePtr);
      TxBufferStatePtr->TxLen = 0U;

      TxBufferStatePtr->TxTimer = 0U;
      TxBufferStatePtr->NPduUdpTxBufferIdx = 0U;
      /* Init shadow buffer variables. */
      TxBufferStatePtr->ShadowBufferLen = 0U;
      TxBufferStatePtr->ShadowBufferStartIdx = 0U;
      TxBufferStatePtr->ShadowBufferTxTimer = 0U;
    }

    if(SoConPtr->SoConTxTcpDataId != SOAD_INVALID_SOCON_ID)
    {
      P2VAR(SoAd_SoConTxTcpDataType, AUTOMATIC, SOAD_APPL_DATA) SoConTxTcpDataPtr = NULL_PTR;
      SoAd_GetSoConTxTcpData(SoConPtr, SoConTxTcpDataPtr);
      SoConTxTcpDataPtr->TxConfCtr = 0U;
    }


    /* initialize PDU specific transmission variables */
    for(
         PduTxDataId = PduTxDataFirstId;
         PduTxDataId < MaxPduTxDataId;
         PduTxDataId++
       )
    {
      /* load related SoAd_PduTxData structure */
      SoAd_GetPduRouteDestVirtualFromPduTxDataId(PduTxDataId, &PduRouteDestVirtualPtr);
      /* reset transmission state and confirmation counter */
      SoAd_SetPduRouteDestVirtualState(PduRouteDestVirtualPtr, SOAD_PDUROUTEDESTVIRTUALSTATE_IDLE);
      if(PduRouteDestVirtualPtr->PduTxTcpDataId != SOAD_INVALID_PDUTXTCPDATA_ID)
      {
        PduTxTcpDataPtr = &SOAD_PBRAM(PduTxTcpData, PduRouteDestVirtualPtr->PduTxTcpDataId);
        PduTxTcpDataPtr->PduTxConfCtr = 0U;
      }
    }
  }

  DBG_SOAD_INITSOCONTXDATASTRUCTURES_EXIT(SoConPtr);
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_InitSoConRxDataStructures
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr
)
{
  /* check if this socket connection uses RxBuffer(s) */
  if(SoConPtr->RxDataId != SOAD_INVALID_SOCON_ID)
  {
    P2VAR(SoAd_SoConRxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConRxDataPtr = NULL_PTR;
    SoAd_GetSoConRxData(SoConPtr, SoConRxDataPtr);

    SoConRxDataPtr->CurrPduHeaderRemainingLen = SOAD_PDU_HEADER_LEN;
    SoConRxDataPtr->CurrPduForwardedLen = 0U;
    SoConRxDataPtr->NextPduOutstandingLen = 0U;
    SoConRxDataPtr->CurrPduLen = 0U;

    SoConRxDataPtr->TpIn = 0U;
    SoConRxDataPtr->TpOut = 0U;
    SoConRxDataPtr->IfIn = 0U;

    SoConRxDataPtr->CurrSocketRouteId = SOAD_INVALID_ROUTE_ID;
    /* SoConRxDataPtr->CurrPduHeaderId;  Requires invalid header ID */
  }
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_SetBitInTransCondChange
(
  P2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr
)
{
  SoAd_SetBitInBitMap(SoConPtr, &SoAd_PBcfg->SoConTransCondChangeBitMapBase, &SoAd_StateTransCondChange);
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_SetBitInBitMap
(
  P2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  P2CONST(SoAd_RefToRAM, AUTOMATIC, SOAD_APPL_DATA) BitMapPtr,
  P2VAR(boolean,AUTOMATIC,SOAD_APPL_DATA) GlobalVarChangeIndicatorPtr
)
{
  const SoAd_SoConIdType BitMapIdx = SoConPtr->SoConId / (SoAd_SoConIdType)32U;
  const uint8 SoConOffset = SoConPtr->SoConId % 32U;
  CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) BitMap =
      &SOAD_PBRAM_REF_ID(uint32, *BitMapPtr, 0U);

  /* Set global flag to know that for any SoCon a transition condition was met. */
  TS_AtomicAssign8(*GlobalVarChangeIndicatorPtr, TRUE);

  /* Set global flag to know that for a certain SoCon a transition condition was met. */
  TS_AtomicSetBit_32(&BitMap[BitMapIdx], SoConOffset);
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_SetBitIn32BitMap
(
    uint32 DestId,
    P2CONST(SoAd_RefToRAM, AUTOMATIC, SOAD_APPL_DATA) BitMapPtr
)
{
  const uint32 BitMapIdx = DestId / 32U;
  const uint8 OffSet = DestId % 32U;
  CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) BitMap =
    &SOAD_PBRAM_REF_ID(uint32, *BitMapPtr, 0U);

  TS_AtomicSetBit_32(&BitMap[BitMapIdx], OffSet);
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_ClearBitInBitMap
(
  P2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  P2CONST(SoAd_RefToRAM, AUTOMATIC, SOAD_APPL_DATA) BitMapPtr
)
{
  const SoAd_SoConIdType BitMapIdx = SoConPtr->SoConId / (SoAd_SoConIdType)32U;
  const uint8 SoConOffset = SoConPtr->SoConId % 32U;
  CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) BitMap =
      &SOAD_PBRAM_REF_ID(uint32, *BitMapPtr, 0U);

  /* Set global flag to know that for a certain SoCon a transition condition was met. */
  TS_AtomicClearBit_32(&BitMap[BitMapIdx], SoConOffset);
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_ClearBitIn32BitMap
(
    uint32 DestId,
    P2CONST(SoAd_RefToRAM, AUTOMATIC, SOAD_APPL_DATA) BitMapPtr
)
{
  const uint32 BitMapIdx = DestId / 32U;
  const uint8 OffSet = DestId % 32U;
  CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) BitMap =
      &SOAD_PBRAM_REF_ID(uint32, *BitMapPtr, 0U);

  TS_AtomicClearBit_32(&BitMap[BitMapIdx], OffSet);
}

/*==================[internal function definitions]=========================*/

#if(TS_MERGED_COMPILE == STD_OFF)
#define SOAD_STOP_SEC_CODE
#include <SoAd_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */
/*==================[end of file]===========================================*/
