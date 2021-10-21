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
 * MISRAC2012-1) Deviated Rule: 11.3 (required)
 * A cast shall not be performed between a pointer to object type and a pointer to a different
 * object type.
 *
 * Reason:
 * AUTOSAR defines a generic type which dependent on a member of the generic type
 * implements an concrete type. Thus for usage of this concrete type, a cast is necessary.
 */

/*==================[inclusions]============================================*/

#include <SoAd.h> /* Module public header file. */
#include <SoAd_Int.h> /* Module internal header file. */
#include <SchM_SoAd.h>

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
#include <Det.h> /* Det API */
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */

#include <SoAd_01_UdpSM_Int.h> /* UDP state machine unit. */
#include <SoAd_02_SocketCon_Int.h> /* Common socket handling relevant APIs. */

#if(SOAD_META_DATA_HANDLING == STD_ON)
#include <EcuC.h> /* Module public API */
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
#define SOAD_START_SEC_CODE
#include <SoAd_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_ULTpRxIndication
(
  P2CONST(SoAd_SocketRouteDestType, AUTOMATIC, SOAD_APPL_DATA) SocketRouteDestPtr,
  NotifResultType Result
)
{
  DBG_SOAD_ULTPRXINDICATION_ENTRY(SocketRoutePtr, Result);


  SOAD_PRECONDITION_ASSERT(SocketRouteDestPtr->ULFuncId != SOAD_INVALID_ULFUNCID, DET_INTERNAL_API_ID);
  SoAd_TpModuleAPI[SocketRouteDestPtr->ULFuncId].TpRxIndication
  (
    SocketRouteDestPtr->DestPduId,
    Result
  );

  DBG_SOAD_ULTPRXINDICATION_EXIT(SocketRoutePtr, Result);
}

TS_MOD_PRIV_DEFN FUNC(boolean, SOAD_CODE) SoAd_ResetRemoteAddress
(
  P2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  P2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  boolean RemoteAddrHasWildcard = FALSE;

  DBG_SOAD_RESETREMOTEADDRESS_ENTRY(SoConPtr, SoConStatePtr);

  /* store allowed remote address for this socket connection, 0 = wildcard */
  TS_MemCpy(SoConStatePtr->RemoteAddress, SoConPtr->AllowedRemoteIpAddr, SOAD_IPADDRMEMSIZE);

  /* store allowed remote port for this socket connection, 0 = wildcard */
  SoConStatePtr->RemotePort = SoConPtr->AllowedRemotePort;

  if(SOAD_IS_REMOTE_WILDCARD(SoConStatePtr->RemoteAddress, SoConStatePtr->RemotePort))
  {
    RemoteAddrHasWildcard = TRUE;

    /* Update TXING_OK_State */
    SOAD_FLAG_CLR_UP(SoConStatePtr,SOAD_SOCONSTATE_TXING_OK);
  }

  /* Set global flag to know that for a certain SoCon a transition condition was met. */
  SoAd_SetBitInTransCondChange(SoConPtr);

  DBG_SOAD_RESETREMOTEADDRESS_EXIT(SoConPtr, SoConStatePtr);

  return RemoteAddrHasWildcard;
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_ULTpTxConfirmation
(
  SoAd_RouteIdType PduRouteId,
  NotifResultType Result
)
{
  CONSTP2CONST(SoAd_PduRouteType, AUTOMATIC, SOAD_APPL_DATA) PduRoutePtr =
      &SOAD_CFG(PduRoute, PduRouteId);

  DBG_SOAD_ULTPTXCONFIRMATION_ENTRY(PduRouteId, Result);

  /* reset all socket connections of this PduRoute if they use a wild card and automatic setup
   * before the <UL>TxConfirmation to prevent further transmissions until the
   * connection was reopened */
  SoAd_ResetWildCardSoConInPduRoute(PduRouteId);

  SOAD_PRECONDITION_ASSERT(PduRoutePtr->ULFuncId != SOAD_INVALID_ULFUNCID, DET_INTERNAL_API_ID);

  SoAd_TpModuleAPI[PduRoutePtr->ULFuncId].TpTxConfirmation(PduRoutePtr->TxConfPduId, Result);

  DBG_SOAD_ULTPTXCONFIRMATION_EXIT(PduRouteId, Result);
}

TS_MOD_PRIV_DEFN FUNC(SoAd_PduRouteDestVirtualIdType, SOAD_CODE) SoAd_GetActivePduRouteDestVirtualId
(
  P2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr
)
{
  SoAd_PduRouteDestVirtualIdType ActivePduRouteDestVirtualId = SOAD_INVALID_DEST_VIRTUAL_ID;

  DBG_SOAD_GETACTIVEPDUROUTEVIRTUALDESTID_ENTRY(SoConPtr);

  if(SoConPtr->TxDataId != SOAD_INVALID_SOCON_ID)
  {
    P2VAR(SoAd_SoConTxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConTxDataPtr = NULL_PTR;
    SoAd_GetSoConTxData(SoConPtr, SoConTxDataPtr);

    ActivePduRouteDestVirtualId = SoConTxDataPtr->ActivePduRouteDestVirtualId;
  }

  DBG_SOAD_GETACTIVEPDUROUTEVIRTUALDESTID_EXIT(ActivePduRouteDestVirtualId, SoConPtr);
  return ActivePduRouteDestVirtualId;
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_StartUpSoCon
(
  P2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr
)
{
  DBG_SOAD_STARTUPSOCON_ENTRY(SoConPtr);

  /* check if pdu header mode is disabled and that a socket route exists */
  if(
      (!SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_PDU_HEADER_MODE))
      && (SoConPtr->SocketRouteFirstId != SOAD_INVALID_ROUTE_ID)
    )
  {
    CONSTP2CONST(SoAd_SocketRouteType, AUTOMATIC, SOAD_APPL_DATA) SocketRoutePtr =
        &SOAD_CFG(SocketRoute, SoConPtr->SocketRouteFirstId);

    /* if the upper layer uses the TP-API */
    if(SOAD_FLAG(SocketRoutePtr, SOAD_SOCKETROUTE_FLAG_IS_UL_TP))
    {
      PduLengthType BufferSize;

      if(SoConPtr->RxDataId != SOAD_INVALID_SOCON_ID)
      {
        P2VAR(SoAd_SoConRxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConRxDataPtr = NULL_PTR;
        SoAd_GetSoConRxData(SoConPtr, SoConRxDataPtr);

        /* set RxData variables */
        SoConRxDataPtr->CurrPduForwardedLen = 0U;
        SoConRxDataPtr->CurrPduLen = 65535U;
        SoConRxDataPtr->CurrSocketRouteId = SoConPtr->SocketRouteFirstId;
      }

      /* call the upper layer and start reception with unknown length */
      (void)SoAd_ULTpStartofReception(SoConPtr, SocketRoutePtr, 0U, &BufferSize);
    }
  }

  DBG_SOAD_STARTUPSOCON_EXIT(SoConPtr);
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_AssignRemoteAddressSoCon2SockAddrType
(
  P2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  P2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) AddressDstPtr
)
{

#if(SOAD_IPV6_SUPPORT == STD_ON)
  if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_IPV6))
  {
   /* Deviation MISRAC2012-1 */
    P2VAR(TcpIp_SockAddrInet6Type, AUTOMATIC, SOAD_APPL_DATA) AddressDstInet6Ptr =
        (P2VAR(TcpIp_SockAddrInet6Type, TYPEDEF, SOAD_APPL_DATA))
        AddressDstPtr;

    SOAD_ENTER_CRITSEC();
    TS_MemCpy(AddressDstInet6Ptr->addr, SoConStatePtr->RemoteAddress, SOAD_IPADDRMEMSIZE);
    AddressDstInet6Ptr->port = SoConStatePtr->RemotePort;
    AddressDstInet6Ptr->domain = TCPIP_AF_INET6;
    SOAD_EXIT_CRITSEC();
  }
  else
#else
  TS_PARAM_UNUSED(SoConPtr);
#endif /* (SOAD_IPV6_SUPPORT == STD_ON) */
  {
   /* Deviation MISRAC2012-1 */
    P2VAR(TcpIp_SockAddrInetType, AUTOMATIC, SOAD_APPL_DATA) AddressDstInetPtr =
        (P2VAR(TcpIp_SockAddrInetType, TYPEDEF, SOAD_APPL_DATA))
        AddressDstPtr;

    TS_AtomicAssign32(AddressDstInetPtr->addr[0], SoConStatePtr->RemoteAddress[0]);
    TS_AtomicAssign16(AddressDstInetPtr->port, SoConStatePtr->RemotePort);
    TS_AtomicAssign16(AddressDstInetPtr->domain, TCPIP_AF_INET);
  }
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_AssignRemoteAddressSockAddrType2SoCon
(
  P2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  P2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) AddressSrcPtr
)
{

#if(SOAD_IPV6_SUPPORT == STD_ON)
  if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_IPV6))
  {
    P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, SOAD_APPL_DATA) AddressSrcInet6Ptr =
      (P2CONST(TcpIp_SockAddrInet6Type, TYPEDEF, SOAD_APPL_DATA))
      (P2CONST(void, TYPEDEF, SOAD_APPL_DATA))
      AddressSrcPtr;

    SOAD_ENTER_CRITSEC();
    TS_MemCpy(SoConStatePtr->RemoteAddress, AddressSrcInet6Ptr->addr, SOAD_IPADDRMEMSIZE);
    SoConStatePtr->RemotePort = AddressSrcInet6Ptr->port;
    SOAD_EXIT_CRITSEC();
  }
  else
#else
  TS_PARAM_UNUSED(SoConPtr);
#endif /* (SOAD_IPV6_SUPPORT == STD_ON) */
  {
    P2CONST(TcpIp_SockAddrInetType, AUTOMATIC, SOAD_APPL_DATA) AddressSrcInetPtr =
      (P2CONST(TcpIp_SockAddrInetType, TYPEDEF, SOAD_APPL_DATA))
      (P2CONST(void, TYPEDEF, SOAD_APPL_DATA))
      AddressSrcPtr;

    TS_AtomicAssign32(SoConStatePtr->RemoteAddress[0U], AddressSrcInetPtr->addr[0U]);
    SoConStatePtr->RemotePort = AddressSrcInetPtr->port;
  }
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_StartAliveSupervisionTimer
(
  P2CONST(SoAd_SoConGroupType, AUTOMATIC, SOAD_APPL_DATA) SoConGroupPtr,
  P2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  SOAD_ENTER_CRITSEC();
  if(SoConGroupPtr->UdpAliveRefTimeout > (SOAD_TIMERTYPE_MAX - SoAd_CurrentTimerValue))
  {
    const SoAd_TimerType UdpAliveTime = SOAD_TIMERTYPE_MAX - SoAd_CurrentTimerValue;

    SoConStatePtr->UdpAliveTimeout = SoConGroupPtr->UdpAliveRefTimeout - UdpAliveTime;
  }
  else
  {
    SoConStatePtr->UdpAliveTimeout = SoConGroupPtr->UdpAliveRefTimeout + SoAd_CurrentTimerValue;
  }
  SOAD_EXIT_CRITSEC();

  /* Check if SoAd_NextUdpAliveTimeout needs to be updated */
  /* !LINKSTO SoAd.dsn.UdpTimeoutHandling, 1 */
  if((SoAd_NextUdpAliveTimeout) == 0U)
  {
    (SoAd_NextUdpAliveTimeout) = SoConStatePtr->UdpAliveTimeout;
  }
  else
  {
    if((SoAd_NextUdpAliveTimeout) < SoAd_CurrentTimerValue)
    {
      if((SoConStatePtr->UdpAliveTimeout > SoAd_CurrentTimerValue) ||
          (SoConStatePtr->UdpAliveTimeout < (SoAd_NextUdpAliveTimeout))
      )
      {
        (SoAd_NextUdpAliveTimeout) = SoConStatePtr->UdpAliveTimeout;
      }
    }
    /* (SoAd_NextUdpAliveTimeout > SoAd_CurrentTimerValue) */
    else
    {
      if((SoConStatePtr->UdpAliveTimeout < (SoAd_NextUdpAliveTimeout)) &&
          (SoConStatePtr->UdpAliveTimeout > SoAd_CurrentTimerValue)
      )
      {
        (SoAd_NextUdpAliveTimeout) = SoConStatePtr->UdpAliveTimeout;
      }
    }
  }
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_HandleUdpAliveTimeout(void)
{
  SoAd_SoConIdType SoConId;
  const SoAd_SoConIdType SoConCnt = SoAd_PBcfg->SoConCnt;

  SoAd_NextUdpAliveTimeout = 0U;

  for(SoConId = 0U; SoConId < SoConCnt; SoConId++)
  {
    CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr =
        &SOAD_PBRAM(SoConState, SoConId);

    if(SoConStatePtr->UdpAliveTimeout != 0U)
    {
      /* timeout occurred for UdpAlive */
      if(SoAd_CurrentTimerValue == SoConStatePtr->UdpAliveTimeout)
      {
        CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr =
            &SOAD_CFG(SoCon, SoConId);
        boolean Release = FALSE;

        SOAD_ENTER_CRITSEC();
        /* deactivate alive supervision timer */
        SoConStatePtr->UdpAliveTimeout = 0U;

        if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_REMOTE_SET) &&
            (!SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_USED_SETREMOTEADDR)) &&
            (SoConStatePtr->SetUniqueRemoteAddrCnt == 0U)
        )
        {
          /* there is an active TP session if
           * - either received TP data is buffered or
           * - a TP transmission is pending
           * or an active If transmission if BusyCnt != 0
           */
          if(
              SOAD_FLAG_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RX_DATA_BUFFERED)
              || SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_TP_PENDING)
              || (SoConStatePtr->BusyCnt != 0)
            )
          {
            /* RxTx in Progress --> lock Tx */
            SOAD_FLAG_SET_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_LOCK);
          }
          else
          {
            Release = TRUE;
          }
        }
        SOAD_EXIT_CRITSEC();

        if(Release == TRUE)
        {
          /* UdpAliveTimeout is only active if configured remote address conatins wildcards.
           * Hence return value of SoAd_ResetRemoteAddress is always true and socket connection
           * has to be changed to RECONNECT*/
          (void)SoAd_ResetRemoteAddress(SoConPtr, SoConStatePtr);
          (void)SoAd_Int_01_UdpSM_Disconnected(SoConPtr, SoConStatePtr);
        }
      }
      else
      {
        /* Update SoAd_NextUdpAliveTimeout */
        /* !LINKSTO SoAd.dsn.UdpTimeoutHandling, 1 */
        if(SoAd_NextUdpAliveTimeout == 0U)
        {
          SoAd_NextUdpAliveTimeout = SoConStatePtr->UdpAliveTimeout;
        }
        else
        {
          if(SoAd_NextUdpAliveTimeout < SoAd_CurrentTimerValue)
          {
            if((SoConStatePtr->UdpAliveTimeout > SoAd_CurrentTimerValue) ||
               (SoConStatePtr->UdpAliveTimeout < SoAd_NextUdpAliveTimeout)
              )
            {
              SoAd_NextUdpAliveTimeout = SoConStatePtr->UdpAliveTimeout;
            }
          }
          /* (SoAd_NextUdpAliveTimeout > SoAd_CurrentTimerValue) */
          else
          {
            if((SoConStatePtr->UdpAliveTimeout < SoAd_NextUdpAliveTimeout) &&
                (SoConStatePtr->UdpAliveTimeout > SoAd_CurrentTimerValue)
            )
            {
              SoAd_NextUdpAliveTimeout = SoConStatePtr->UdpAliveTimeout;
            }
          }
        }
      }
    }
  }
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_ResetWildCardSoConInPduRoute
(
  SoAd_RouteIdType PduRouteId
)
{
  CONSTP2CONST(SoAd_PduRouteType, AUTOMATIC, SOAD_APPL_DATA) PduRoutePtr =
      &SOAD_CFG(PduRoute, PduRouteId);
  SoAd_SoConIdType Idx = 0U; /* SoConId iterator for Tx Pdu fan-out */
  const SoAd_PduRouteDestVirtualIdType PduRouteDestCnt = PduRoutePtr->PduRouteDestVirtualCnt;
  DBG_SOAD_RESETWILDCARDSOCONINPDUROUTE_ENTRY(PduRouteId);

  SOAD_ENTER_CRITSEC();

  for(Idx = 0U ; Idx < PduRouteDestCnt ; Idx++)
  {
    CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr =
        &SOAD_CFG(PduRouteDestVirtual, PduRoutePtr->PduRouteDestVirtualFirstId + Idx);
    CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr =
        &SOAD_CFG(SoCon, PduRouteDestVirtualPtr->SoConId);

    if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_AUTO_SETUP))
    {
      CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr =
        &SOAD_PBRAM(SoConState, PduRouteDestVirtualPtr->SoConId);
      SoAd_ResetWildCardSoCon(SoConPtr, SoConStatePtr);
    }
  }
  SOAD_EXIT_CRITSEC();

  DBG_SOAD_RESETWILDCARDSOCONINPDUROUTE_EXIT(PduRouteId);
}

TS_MOD_PRIV_DEFN FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_ULTpStartofReception
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  P2CONST(SoAd_SocketRouteType, AUTOMATIC, SOAD_APPL_DATA) SocketRoutePtr,
  PduLengthType TpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_DATA) BufferSizePtr
)
{
  /* TP only is allowed for one Destination */
  P2CONST(SoAd_SocketRouteDestType, AUTOMATIC, SOAD_APPL_DATA) SocketRouteDestPtr =
    &SOAD_CFG(SocketRouteDest, SocketRoutePtr->SocketRouteDestFirstId);
  BufReq_ReturnType RetVal_BufReq = BUFREQ_E_NOT_OK;

  DBG_SOAD_ULTPSTARTOFRECEPTION_ENTRY(SoConPtr, SocketRoutePtr, TpSduLength, BufferSizePtr);

#if(SOAD_META_DATA_HANDLING == STD_ON)
  if(SOAD_FLAG(SocketRoutePtr, SOAD_SOCKETROUTE_FLAG_METADATA_SOCON_ID))
  {
    PduInfoType PduInfo;
    PduInfo.SduDataPtr = NULL_PTR;
    PduInfo.SduLength = TpSduLength;
    /* No error handling expected. Data will be provided to UL even if EcuC does not accept setting
    of Meta data. */
    (void) EcuC_SetMetaDataSoConId(SocketRouteDestPtr->EcuCPduId, &PduInfo, SoConPtr->SoConId);
  }
#else
  TS_PARAM_UNUSED(SoConPtr);
#endif

  SOAD_PRECONDITION_ASSERT(SocketRouteDestPtr->ULFuncId != SOAD_INVALID_ULFUNCID, DET_INTERNAL_API_ID);

  RetVal_BufReq = SoAd_TpModuleAPI[SocketRouteDestPtr->ULFuncId].TpStartOfReception
  (
    SocketRouteDestPtr->DestPduId,
    TpSduLength,
    BufferSizePtr
  );

  /* BUFREQ_E_BUSY can be treated as BUFREQ_OK */
  if(RetVal_BufReq == BUFREQ_E_BUSY)
  {
    RetVal_BufReq = BUFREQ_OK;
    *BufferSizePtr = 0U;
  }

  DBG_SOAD_ULTPSTARTOFRECEPTION_EXIT
  (
    RetVal_BufReq,
    SoConPtr,
    SocketRoutePtr,
    TpSduLength,
    BufferSizePtr
  );
  return RetVal_BufReq;
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_ResetWildCardSoCon
(
  P2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  P2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  const boolean RemoteIsWildcard
    = (SOAD_IS_REMOTE_WILDCARD(SoConPtr->AllowedRemoteIpAddr, SoConPtr->AllowedRemotePort));

  DBG_SOAD_RESETWILDCARDSOCON_ENTRY(SoConPtr, SoConStatePtr);

  /* If this socket connection is configured for automatic setup and
   * uses a wild card for the remote address, the connection shall be closed to be reopened
   * in the next MainFunction by changing the socket state */
  if(RemoteIsWildcard)
  {
    if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_TCP))
    {
      /* TCP connection have to be reconnected to reset the wild card address */
      SOAD_FLAG_SET_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RECONNECT_SOCKET);
    }
    else
    {
      /* for UDP connections it's sufficient to reset the address in SoConStatePtr to the configured
       * wild card address */
      /* Return value can be ignored. It is not relevant if remote addr is wildcard at this point. */
      (void)SoAd_ResetRemoteAddress(SoConPtr, SoConStatePtr);

    }
    /* Set global flag to know that for a certain SoCon a transition condition was met. */
    SoAd_SetBitInTransCondChange(SoConPtr);
  }

  DBG_SOAD_RESETWILDCARDSOCON_EXIT(SoConPtr, SoConStatePtr);
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_SetPduRouteDestVirtualState
(
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr,
  SoAd_PduRouteDestVirtualStateType RequestedTxState
)
{
  /* PduTxData have same order as PduRouteDestVirtual. */
  CONSTP2VAR(SoAd_PduRouteDestVirtualDataType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualStatePtr =
    &SOAD_PBRAM(PduRouteDestVirtualData, PduRouteDestVirtualPtr->PduRouteDestVirtualId);
  CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) PduTxDataNotIdleBitMap =
    &SOAD_PBRAM_REF_ID(uint32, SoAd_PBcfg->PduTxDataNotIdleBitMapBase, 0U);
  CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) DestVirtualNotIdleBitMap =
    &SOAD_PBRAM_REF_ID(uint32, SoAd_PBcfg->DestVirtualNotIdleBitMapBase, 0U);
  const SoAd_PduTxDataIdType PduTxData_BitMapOffset
    = (SoAd_PduTxDataIdType)PduRouteDestVirtualPtr->PduTxDataId / 32U;
  const uint8 PduTxData_BitMapIdx = PduRouteDestVirtualPtr->PduTxDataId % 32U;
  const SoAd_PduRouteDestVirtualIdType PduRouteDestVirtual_BitMapOffset
    = PduRouteDestVirtualPtr->PduRouteDestVirtualId / 32U;
  const uint8 PduRouteDestVirtual_BitMapIdx = PduRouteDestVirtualPtr->PduRouteDestVirtualId % 32U;

  TS_AtomicAssign8(PduRouteDestVirtualStatePtr->PduRouteDestVirtualState, RequestedTxState);
  if(RequestedTxState == SOAD_PDUROUTEDESTVIRTUALSTATE_IDLE)
  {
    TS_AtomicClearBit_32(&PduTxDataNotIdleBitMap[PduTxData_BitMapOffset], PduTxData_BitMapIdx);
    TS_AtomicClearBit_32
    (
      &DestVirtualNotIdleBitMap[PduRouteDestVirtual_BitMapOffset], PduRouteDestVirtual_BitMapIdx
    );
  }
  else
  {
    TS_AtomicSetBit_32(&PduTxDataNotIdleBitMap[PduTxData_BitMapOffset], PduTxData_BitMapIdx);
    TS_AtomicSetBit_32
    (
      &DestVirtualNotIdleBitMap[PduRouteDestVirtual_BitMapOffset], PduRouteDestVirtual_BitMapIdx
    );
  }
}

#if(TS_MERGED_COMPILE == STD_OFF)
#define SOAD_STOP_SEC_CODE
#include <SoAd_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */
/*==================[end of file]===========================================*/
