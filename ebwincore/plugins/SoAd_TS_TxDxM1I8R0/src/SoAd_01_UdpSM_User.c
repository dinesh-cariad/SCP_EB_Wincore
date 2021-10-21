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
 * MISRAC2012-9) Deviated Rule: 11.1 (required)
 * Conversion shall not be performed between a pointer to a function and any type
 * other than an integral type.
 *
 * Reason:
 * This violation only occurs in a specific testing scenario only where these macros are used for
 * function trace recoding. Thus this is not contained in production code.
 *
 */

/*==================[inclusions]============================================*/
#include <SoAd_01_UdpSM_User.h> /* Unit public header file. */
#include <SchM_SoAd.h>
#include <SoAd_Trace.h>

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[internal function declarations]========================*/

#if(TS_MERGED_COMPILE == STD_OFF)
#define SOAD_START_SEC_CODE
#include <SoAd_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

STATIC FUNC(void, SOAD_CODE) SoAd_Int_01_SetupGroupSoCon_Udp
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

STATIC FUNC(void, SOAD_CODE) SoAd_Int_01_Udp_CloseSocket
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_Int_01_UdpSM_State_CLOSED_Entry_Cleanup
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  boolean UsedSetRemoteAddr;
  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_01_UDPSM_STATE_CLOSED_ENTRY_CLEANUP_ENTRY(SoConPtr, SoConStatePtr);

  UsedSetRemoteAddr = SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_USED_SETREMOTEADDR);

  SOAD_ENTER_CRITSEC();

  /* Caution: socket connection is closed, ALL flags shall be cleared
   * new socket connections must not be affected by remaining feature flags */
  SoConStatePtr->UpperFlags = 0U;
  SoConStatePtr->LowerFlags = 0U;
  SoConStatePtr->UdpAliveTimeout = 0U;
  SoConStatePtr->SocketId = SOAD_SOCKETID_INVALID;

  SOAD_EXIT_CRITSEC();

  /* Remote address shall only be reset when socket connection is closed,
   * if it was not set with SoAd_SetUniqueRemoteAddr() */
  /* !LINKSTO SoAd.dsn.RemoteAddressHandling.ReleaseRemoteAddr.Udp.CloseSoCon, 1 */
  if(SoConStatePtr->SetUniqueRemoteAddrCnt == 0)
  {
    /* Note: clear flags before SoAd_ResetRemoteAddress, if the remote address is configured
     * SOAD_SOCONSTATE_FLAG_REMOTE_SET shall be set again */
    if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_REMOTE_SET))
    {
      SOAD_FLAG_SET_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_REMOTE_SET);

      /* Return value can be ignored */
      (void)SoAd_ResetRemoteAddress(SoConPtr, SoConStatePtr);
    }
  }
  else
  {
    /* Remote address is set --> restore flag */
    SOAD_FLAG_SET_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_REMOTE_SET);

    /* Restore flag to indicate if the remote address was initial set with
     * SoAd_SetRemoteAddr() */
    if(UsedSetRemoteAddr == TRUE)
    {
      SOAD_FLAG_SET_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_USED_SETREMOTEADDR);
    }

  }
  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_01_UDPSM_STATE_CLOSED_ENTRY_CLEANUP_EXIT(SoConPtr, SoConStatePtr);
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_Int_01_UdpSM_State_CLOSING_Entry_Closing
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_01_UDPSM_STATE_CLOSING_ENTRY_CLOSING_ENTRY(SoConPtr, SoConStatePtr);

  SoAd_Int_01_Udp_CloseSocket(SoConPtr, SoConStatePtr);

  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_01_UDPSM_STATE_CLOSING_ENTRY_CLOSING_EXIT(SoConPtr, SoConStatePtr);
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_Int_01_UdpSM_State_OFFLINE_Entry_SoConModeChg
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2CONST(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  TS_PARAM_UNUSED(SoConStatePtr);

  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_01_UDPSM_STATE_OFFLINE_ENTRY_SOCONMODECHG_ENTRY(SoConPtr, SoConStatePtr);

  SoAd_Int_02_ULSoConModeChg(SoConPtr, SOAD_SOCON_OFFLINE);

  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_01_UDPSM_STATE_OFFLINE_ENTRY_SOCONMODECHG_EXIT(SoConPtr, SoConStatePtr);
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_Int_01_UdpSM_State_ONLINE_Entry_SoConModeChg
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2CONST(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  TS_PARAM_UNUSED(SoConStatePtr);

  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_01_UDPSM_STATE_ONLINE_ENTRY_SOCONMODECHG_ENTRY(SoConPtr, SoConStatePtr);

  SoAd_Int_02_ULSoConModeChg(SoConPtr, SOAD_SOCON_ONLINE);

  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_01_UDPSM_STATE_ONLINE_ENTRY_SOCONMODECHG_EXIT(SoConPtr, SoConStatePtr);
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_Int_01_UdpSM_State_ONLINE_Entry_Online
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2CONST(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  TS_PARAM_UNUSED(SoConStatePtr);

  /* Deviation MISRAC2012-9 */
  DBG_SOAD_01_UDPSM_STATE_ONLINE_ENTRY_ONLINE_ENTRY(SoConPtr, SoConStatePtr);

  SoAd_StartUpSoCon(SoConPtr);

  /* Deviation MISRAC2012-9 */
  DBG_SOAD_01_UDPSM_STATE_ONLINE_ENTRY_ONLINE_EXIT(SoConPtr, SoConStatePtr);
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_Int_01_UdpSM_State_CONNECTING_Entry_Connecting
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  boolean Disconnect = FALSE;
  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_01_UDPSM_STATE_CONNECTING_ENTRY_CONNECTING_ENTRY(SoConPtr, SoConStatePtr);

  SOAD_ENTER_CRITSEC();

  if(SoConStatePtr->SocketState != SOAD_SOCKETSTATE_ONLINE)
  {
    Disconnect = TRUE;
  }

  SOAD_EXIT_CRITSEC();

  if(Disconnect == TRUE)
  {
    (void)SoAd_Int_01_SetupGroupSoCon_Udp(SoConPtr, SoConStatePtr);
  }

  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_01_UDPSM_STATE_CONNECTING_ENTRY_CONNECTING_EXIT(SoConPtr, SoConStatePtr);
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_Int_01_UdpSM_State_RESTART_Entry_Restart
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  boolean UsedSetRemoteAddr;
  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_01_UDPSM_STATE_RESTART_ENTRY_RESTART_ENTRY(SoConPtr, SoConStatePtr);

  UsedSetRemoteAddr = SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_USED_SETREMOTEADDR);

  SOAD_ENTER_CRITSEC();

  /* Caution: socket connection is closed, ALL flags shall be cleared
   * new socket connections must not be affected by remaining feature flags */
  SoConStatePtr->UpperFlags = 0U;
  SoConStatePtr->LowerFlags = 0U;
  SoConStatePtr->UdpAliveTimeout = 0U;
  SoConStatePtr->SocketId = SOAD_SOCKETID_INVALID;

  SOAD_EXIT_CRITSEC();

  /* Remote address shall only be reset when socket connection is closed,
   * if it was not set with SoAd_SetUniqueRemoteAddr() */
  /* !LINKSTO SoAd.dsn.RemoteAddressHandling.ReleaseRemoteAddr.Udp.CloseSoCon, 1 */
  if(SoConStatePtr->SetUniqueRemoteAddrCnt == 0)
  {
    /* Note: clear flags before SoAd_ResetRemoteAddress, if the remote address is configured
     * SOAD_SOCONSTATE_FLAG_REMOTE_SET shall be set again */
    if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_REMOTE_SET))
    {
      SOAD_FLAG_SET_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_REMOTE_SET);

      /* Return value can be ignored */
      (void)SoAd_ResetRemoteAddress(SoConPtr, SoConStatePtr);
    }
  }
  else
  {
    /* Remote address is set --> restore flag */
    SOAD_FLAG_SET_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_REMOTE_SET);

    /* Restore flag to indicate if the remote address was initial set with
     * SoAd_SetRemoteAddr() */
    if(UsedSetRemoteAddr == TRUE)
    {
      SOAD_FLAG_SET_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_USED_SETREMOTEADDR);
    }

  }
  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_01_UDPSM_STATE_RESTART_ENTRY_RESTART_EXIT(SoConPtr, SoConStatePtr);
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_Int_01_UdpSM_State_SHUTDOWN_Entry_ShutDown
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_01_UDPSM_STATE_SHUTDOWN_ENTRY_SHUTDOWN_ENTRY(SoConPtr, SoConStatePtr);

  SoAd_Int_01_Udp_CloseSocket(SoConPtr, SoConStatePtr);

  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_01_UDPSM_STATE_SHUTDOWN_ENTRY_SHUTDOWN_EXIT(SoConPtr, SoConStatePtr);
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_Int_01_UdpSM_State_RECONNECT_Entry_SoConModeChg
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2CONST(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  TS_PARAM_UNUSED(SoConStatePtr);

  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_01_UDPSM_STATE_RECONNECT_ENTRY_SOCONMODECHG_ENTRY(SoConPtr, SoConStatePtr);

  SoAd_Int_02_ULSoConModeChg(SoConPtr, SOAD_SOCON_RECONNECT);

  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_01_UDPSM_STATE_RECONNECT_ENTRY_SOCONMODECHG_EXIT(SoConPtr, SoConStatePtr);
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_Int_01_UdpSM_State_ONLINE_Exit_CloseRxTx
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_01_UDPSM_STATE_ONLINE_EXIT_CLOSERXTX_ENTRY(SoConPtr, SoConStatePtr);

  /* Update TXING_OK_State */
  SOAD_FLAG_CLR_UP(SoConStatePtr,SOAD_SOCONSTATE_TXING_OK);

  /* TS_PARAM_UNUSED(SoConStatePtr); */
  SoAd_Int_02_CleanUpSoCon(SoConPtr, SoConStatePtr);

  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_01_UDPSM_STATE_ONLINE_EXIT_CLOSERXTX_EXIT(SoConPtr, SoConStatePtr);
}

/*==================[internal function definitions]=========================*/

STATIC FUNC(void, SOAD_CODE) SoAd_Int_01_SetupGroupSoCon_Udp
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  CONSTP2CONST(SoAd_SoConGroupType, AUTOMATIC, SOAD_APPL_DATA) SoConGroupPtr =
    &SOAD_CFG(SoConGroup, SoConPtr->SoConGroupId);
  /* Identify the first socket connection in this group */
  const SoAd_SoConIdType SoConFirstId = SoConGroupPtr->SoConFirstId;
  const SoAd_SoConIdType SoConCnt = SoAd_PBcfg->SoConCnt;
  /* also count the other related socket connections */
  Std_ReturnType RetVal_SocketOk = E_NOT_OK;

  SoAd_SoConIdType Idx;

  if(SoConGroupPtr->SoConCnt > 1U)
  {
    /* scan the SoAd_SocketSoConMap lookup table if the related UDP socket already exists for
       this socket connection. Maybe another SoCon of the same group has already an active
       socket. This is not required in case that the SoCon group has only a single SoCon. */
    SOAD_ENTER_CRITSEC();
    for(Idx = 0U; Idx < SoConCnt; Idx++)
    {
      CONSTP2VAR(SoAd_SocketSoConMapType, AUTOMATIC, SOAD_APPL_DATA) SocketMapEntryPtr =
        &SOAD_PBRAM(SocketSoConMap, Idx);

      if(SocketMapEntryPtr->SoConId == SoConFirstId)
      {
        CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStateFirstPtr =
            &SOAD_PBRAM(SoConState, SoConFirstId);

        SoConStatePtr->SocketId = SocketMapEntryPtr->SocketId;
        SoConStatePtr->AssignedLocalPort = SoConStateFirstPtr->AssignedLocalPort;
        RetVal_SocketOk = E_OK;
        break;
      }
    }
    SOAD_EXIT_CRITSEC();
  }

  if(RetVal_SocketOk != E_OK)
  {
    TcpIp_SocketIdType SocketId = SOAD_SOCKETID_INVALID;

    /* request a new socket */
    RetVal_SocketOk = SoAd_RequestTcpIpSocket(SoConPtr, SoConStatePtr, &SocketId);

    if(RetVal_SocketOk == E_OK)
    {
      boolean Success = FALSE;
      /* socket request was successful, store socketId */
      SoConStatePtr->SocketId = SocketId;

      /* create mapping from socketId to SoConId */
      SOAD_ENTER_CRITSEC();
      for(Idx = 0U; (Idx < SoConCnt) && (Success == FALSE); Idx++)
      {
        CONSTP2VAR(SoAd_SocketSoConMapType, AUTOMATIC, SOAD_APPL_DATA) SocketMapEntryPtr =
          &SOAD_PBRAM(SocketSoConMap, Idx);

        if(SocketMapEntryPtr->SoConId == SOAD_INVALID_SOCON_ID)
        {
          SocketMapEntryPtr->SoConId = SoConFirstId;
          SocketMapEntryPtr->SocketId = SocketId;
          Success = TRUE;
        }
      }
      SOAD_EXIT_CRITSEC();

      /* assert that a valid entry in SocketSoConMap was created for this socket */
      SOAD_INVARIANT_ASSERT(Success == TRUE, SOAD_INTERNAL_SVCID);
    }

  }
}

STATIC FUNC(void, SOAD_CODE) SoAd_Int_01_Udp_CloseSocket
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  boolean SkipCloseSocket = FALSE;

  SOAD_ENTER_CRITSEC();

  /* In case of SOAD_SOCKETSTATE_SHUTDOWN or when socket is already closed TcpIp_Close()
     is already called and shall be skipped. */
  if(
      (SoConStatePtr->SocketState != SOAD_SOCKETSTATE_SHUTDOWN)
      && (SOAD_FLAG_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TCPIP_SOCKET_CLOSED) == FALSE)
    )
  {
    CONSTP2CONST(SoAd_SoConGroupType, AUTOMATIC, SOAD_APPL_DATA) SoConGroupPtr =
      &SOAD_CFG(SoConGroup, SoConPtr->SoConGroupId);
    /* Identify the first socket connection in this group */
    const SoAd_SoConIdType SoConFirstId = SoConGroupPtr->SoConFirstId;
    const SoAd_SoConIdType SoConGroup_SoConMax
      = SoConGroupPtr->SoConFirstId + SoConGroupPtr->SoConCnt;
    SoAd_SoConIdType Idx;

    for(Idx = SoConFirstId; Idx < SoConGroup_SoConMax; Idx++)
    {
      /* Skip current SoCon. Check the others if one of them is not OFFLINE. */
      if(Idx != SoConPtr->SoConId)
      {
        CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStateTmpPtr =
          &SOAD_PBRAM(SoConState, Idx);

        if(SoConStateTmpPtr->SocketState != SOAD_SOCKETSTATE_CLOSED)
        {
          /* TcpIp UDP socket is still in use by other SoCon of the same SoConGroup. No need to
           *  close the socket. Set flag to bring the socket connection to state CLOSED. */
          SkipCloseSocket = TRUE;
          SOAD_FLAG_SET_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TCPIP_SOCKET_CLOSED);
          break;
        }
      }
    }
  }
  else
  {
    SkipCloseSocket = TRUE;
  }
  SOAD_EXIT_CRITSEC();

  if(!SkipCloseSocket)
  {
    /* Abort set to FALSE due to deviation dev.SoAd.Udp.TcpIpCloseAbort */
    SoAd_Int_02_CloseSocket(SoConPtr, SoConStatePtr, FALSE);
  }

  /* Set global flag to know that for a certain SoCon a transition condition was met. */
  SoAd_SetBitInTransCondChange(SoConPtr);
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_Int_01_Setup_UDP_TXING_SoConState
(
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  const boolean RemoteAddressKnown = SOAD_FLAG_UP(SoConStatePtr,SOAD_SOCONSTATE_FLAG_REMOTE_SET)
      && !SOAD_IS_REMOTE_WILDCARD(SoConStatePtr->RemoteAddress, SoConStatePtr->RemotePort);
  const boolean TxFlagCheck = (SoConStatePtr->SocketState == SOAD_SOCKETSTATE_ONLINE)
      && !SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_CLOSE_SOCON)
      && !SOAD_FLAG_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TCPIP_SOCKET_CLOSED)
      && !SOAD_FLAG_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RECONNECT_SOCKET);

  const boolean TxingOk = RemoteAddressKnown && TxFlagCheck;

  if(TxingOk)
  {
    SOAD_FLAG_SET_UP(SoConStatePtr,SOAD_SOCONSTATE_TXING_OK);
  }

}

#if(TS_MERGED_COMPILE == STD_OFF)
#define SOAD_STOP_SEC_CODE
#include <SoAd_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

/*==================[end of file]===========================================*/
