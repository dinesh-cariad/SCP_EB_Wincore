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
 * MISRAC2012-4) Deviated Rule: 11.3 (required)
 * A cast shall not be performed between a pointer to object type and a pointer to a different
 * object type.
 *
 * Reason:
 * AUTOSAR defines a generic type which dependent on a member of the generic type
 * implements an concrete type. Thus for usage of this concrete type, a cast is necessary.

 */

/*==================[inclusions]============================================*/
#include <SoAd_04_TcpSM_User.h> /* Unit public header file. */
#include <SoAd_04_TcpSM_Int.h> /* Unit public header file. */
#include <SchM_SoAd.h>
#include <SoAd_Trace.h>
/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[internal function declarations]========================*/

#if(TS_MERGED_COMPILE == STD_OFF)
#define SOAD_START_SEC_CODE
#include <SoAd_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

STATIC FUNC(void, SOAD_CODE) SoAd_Int_04_ShutdownSocket
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

STATIC FUNC(void, SOAD_CODE) SoAd_Int_04_SetupGroupSoCon
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);


/** \brief Sets up a socket connection
 **
 ** \param[in] SoConFirstId - Index of the first socket connection of the group (set to soConId
 **                           for single socket connections).
 ** \param[in] SoConCnt - Number of the socket connection in the group.
 ** \param[in] SoConPtr - Pointer to the configuration of the socket connection.
 ** \param[inout] SoConStatePtr - Pointer to the state information of the socket connection.
 **
 ** \returns Std_ReturnType
 ** \retval E_OK - The socket connection setup was successful.
 ** \retval E_NOT_OK - The socket connection setup was not successful.
 **/
STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_Int_04_SetupSoCon
(
  SoAd_SoConIdType SoConFirstId,
  SoAd_SoConIdType SoConCnt,
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_State_CLOSED_Entry_Cleanup
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_04_TCPSM_STATE_CLOSED_ENTRY_CLEANUP_ENTRY(SoConPtr, SoConStatePtr);

  SoConStatePtr->SocketId = SOAD_SOCKETID_INVALID;

  /* Caution: socket connection is closed, ALL flags shall be cleared
   * new socket connections must not be affected by remaining feature flags */
  SoConStatePtr->UpperFlags = 0U;
  SoConStatePtr->LowerFlags = 0U;

  /* Note: clear flags before SoAd_ResetRemoteAddress, if the remote address is configured
   * SOAD_SOCONSTATE_FLAG_REMOTE_SET shall be set again */
  if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_REMOTE_SET))
  {
    SOAD_FLAG_SET_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_REMOTE_SET);
    /* Return value can be ignored */
    (void)SoAd_ResetRemoteAddress(SoConPtr, SoConStatePtr);
  }

  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_04_TCPSM_STATE_CLOSED_ENTRY_CLEANUP_EXIT(SoConPtr, SoConStatePtr);
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_State_CLOSING_Entry_Closing
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_04_TCPSM_STATE_CLOSING_ENTRY_CLOSING_ENTRY(SoConPtr, SoConStatePtr);

  if(SoConStatePtr->SocketState != SOAD_SOCKETSTATE_SHUTDOWN)
  {
    SoAd_Int_04_ShutdownSocket(SoConPtr, SoConStatePtr);
  }

  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_04_TCPSM_STATE_CLOSING_ENTRY_CLOSING_EXIT(SoConPtr, SoConStatePtr);
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_State_OFFLINE_Entry_SoConModeChg
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2CONST(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  TS_PARAM_UNUSED(SoConStatePtr);
  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_04_TCPSM_STATE_OFFLINE_ENTRY_SOCONMODECHG_ENTRY(SoConPtr, SoConStatePtr);

  SoAd_Int_02_ULSoConModeChg(SoConPtr, SOAD_SOCON_OFFLINE);

  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_04_TCPSM_STATE_OFFLINE_ENTRY_SOCONMODECHG_EXIT(SoConPtr, SoConStatePtr);
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_State_ONLINE_Entry_SoConModeChg
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2CONST(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  TS_PARAM_UNUSED(SoConStatePtr);
  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_04_TCPSM_STATE_ONLINE_ENTRY_SOCONMODECHG_ENTRY(SoConPtr, SoConStatePtr);

  SoAd_Int_02_ULSoConModeChg(SoConPtr, SOAD_SOCON_ONLINE);

  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_04_TCPSM_STATE_ONLINE_ENTRY_SOCONMODECHG_EXIT(SoConPtr, SoConStatePtr);
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_State_ONLINEentry_Online
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2CONST(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  TS_PARAM_UNUSED(SoConStatePtr);
  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_04_TCPSM_STATE_ONLINEENTRY_ONLINE_ENTRY(SoConPtr, SoConStatePtr);

  SoAd_StartUpSoCon(SoConPtr);

  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_04_TCPSM_STATE_ONLINEENTRY_ONLINE_EXIT(SoConPtr, SoConStatePtr);
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_State_CONNECTING_Entry_Connecting
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_04_TCPSM_STATE_CONNECTING_ENTRY_CONNECTING_ENTRY(SoConPtr, SoConStatePtr);

  SoAd_Int_04_SetupGroupSoCon(SoConPtr, SoConStatePtr);

  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_04_TCPSM_STATE_CONNECTING_ENTRY_CONNECTING_EXIT(SoConPtr, SoConStatePtr);
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_State_RESTART_Entry_Reset
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_04_TCPSM_STATE_RESTART_ENTRY_RESET_ENTRY(SoConPtr, SoConStatePtr);

  SoConStatePtr->SocketId = SOAD_SOCKETID_INVALID;

  /* Caution: socket connection is closed, ALL flags shall be cleared
   * new socket connections must not be affected by remaining feature flags */
  SoConStatePtr->UpperFlags = 0U;
  SoConStatePtr->LowerFlags = 0U;

  /* Note: clear flags before SoAd_ResetRemoteAddress, if the remote address is configured
   * SOAD_SOCONSTATE_FLAG_REMOTE_SET shall be set again */
  if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_REMOTE_SET))
  {
    SOAD_FLAG_SET_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_REMOTE_SET);
    /* Return value can be ignored */
    (void)SoAd_ResetRemoteAddress(SoConPtr, SoConStatePtr);
  }

  /* Set global flag to know that for a certain SoCon a transition condition was met. */
  SoAd_SetBitInTransCondChange(SoConPtr);

  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_04_TCPSM_STATE_RESTART_ENTRY_RESET_EXIT(SoConPtr, SoConStatePtr);
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_State_SHUTDOWN_Entry_ShutDown
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  TS_PARAM_UNUSED(SoConPtr);

  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_04_TCPSM_STATE_SHUTDOWN_ENTRY_SHUTDOWN_ENTRY(SoConPtr, SoConStatePtr);

  /* request TcpIp module to close the socket */
  SoAd_Int_02_CloseSocket(SoConPtr, SoConStatePtr, FALSE);

  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_04_TCPSM_STATE_SHUTDOWN_ENTRY_SHUTDOWN_EXIT(SoConPtr, SoConStatePtr);
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_State_RECONNECT_Entry_SoConModeChg
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2CONST(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  TS_PARAM_UNUSED(SoConStatePtr);
  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_04_TCPSM_STATE_RECONNECT_ENTRY_SOCONMODECHG_ENTRY(SoConPtr, SoConStatePtr);

  SoAd_Int_02_ULSoConModeChg(SoConPtr, SOAD_SOCON_RECONNECT);

  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_04_TCPSM_STATE_RECONNECT_ENTRY_SOCONMODECHG_EXIT(SoConPtr, SoConStatePtr);
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_Int_04_TcpSM_State_ONLINE_Exit_CloseRxTx
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_04_TCPSM_STATE_ONLINE_EXIT_CLOSERXTX_ENTRY(SoConPtr, SoConStatePtr);

  SoAd_Int_02_CleanUpSoCon(SoConPtr, SoConStatePtr);

  /* Deviation MISRAC2012-9 */
  DBG_SOAD_INT_04_TCPSM_STATE_ONLINE_EXIT_CLOSERXTX_EXIT(SoConPtr, SoConStatePtr);
}

/*==================[internal function definitions]=========================*/

STATIC FUNC(void, SOAD_CODE) SoAd_Int_04_ShutdownSocket
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  CONSTP2CONST(SoAd_SoConGroupType, AUTOMATIC, SOAD_APPL_DATA) SoConGroupPtr =
    &SOAD_CFG(SoConGroup, SoConPtr->SoConGroupId);
  const boolean IsAcceptSocket =
        (!SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_TCP_INITIATE)) ? TRUE : FALSE;

  TcpIp_SocketIdType ListenSocketId = SOAD_SOCKETID_INVALID;
  boolean Abort;

  DBG_SOAD_INT_04_SHUTDOWNSOCKET_ENTRY(SoConPtr, SoConStatePtr);

  SOAD_ENTER_CRITSEC();

  /* check if the listenSocket shall be closed too */
  if(IsAcceptSocket)
  {
    boolean ShutdownListenSocket = TRUE;
    SoAd_SoConIdType Idx;
    const SoAd_SoConIdType SoConCnt = SoConGroupPtr->SoConCnt;
    const SoAd_SoConIdType SoConFirstId = SoConGroupPtr->SoConFirstId;

    for(Idx = 0U; Idx < SoConCnt; Idx++)
    {
      if(SOAD_PBRAM(SoConState, SoConFirstId + Idx).OpenRefCnt > 0U)
      {
        /* this socket connection still uses the listen socket, don't close it */
        ShutdownListenSocket = FALSE;
      }
    }

    if(ShutdownListenSocket)
    {
      const SoAd_SoConIdType ListenSocketCapacity = SoAd_PBcfg->ListenSocketCapacity;

      /* search for the listenSocket associated with this socket connection */
      for(Idx = 0U; Idx < ListenSocketCapacity; Idx++)
      {
        CONSTP2VAR(SoAd_ListenSocketSoConMapType, AUTOMATIC, SOAD_APPL_DATA) ListenMapEntry =
            &SOAD_PBRAM(ListenSocketSoConMap, Idx);

        if(ListenMapEntry->SoConListFirstId == SoConFirstId)
        {
          ListenSocketId = ListenMapEntry->ListenSocketId;
          break;
        }
      }
      /* If no listen socket entry exists, the socket was already closed */
    }
  }

  /* if SoAd_CloseSoCon was called with abort = TRUE, TcpIp_Close shall also be called with
   * abort = TRUE.
   * I.e the socket shall be close immediately without regarding a clean shutdown sequence */
  Abort = SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_CLOSE_SOCON_ABORT);

  SOAD_EXIT_CRITSEC();

  /* if a valid listenSocketId was assigned the related listenSocket shall be closed */
  if(ListenSocketId != SOAD_SOCKETID_INVALID)
  {
    /* request TcpIp module to close the listenSocket */
    (void)TcpIp_Close(ListenSocketId, Abort);
  }

  /* request TcpIp module to close the socket */
  SoAd_Int_02_CloseSocket(SoConPtr, SoConStatePtr, Abort);

  DBG_SOAD_INT_04_SHUTDOWNSOCKET_EXIT(SoConPtr, SoConStatePtr);
}

STATIC FUNC(void, SOAD_CODE) SoAd_Int_04_SetupGroupSoCon
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  SoAd_SoConIdType Idx;
  CONSTP2CONST(SoAd_SoConGroupType, AUTOMATIC, SOAD_APPL_DATA) SoConGroupPtr =
    &SOAD_CFG(SoConGroup, SoConPtr->SoConGroupId);

  /* Identify the first socket connection in this group */
  const SoAd_SoConIdType SoConFirstId = SoConGroupPtr->SoConFirstId;
  Std_ReturnType RetVal_SocketOk = E_NOT_OK;

  DBG_SOAD_INT_04_SETUPGROUPSOCON_ENTRY(SoConPtr, SoConStatePtr);

  if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_TCP_INITIATE))
  {
    /* This is a TCP client. Only one SoCon is allowed since local port can not be reused. */
    /* ignore return value - if the socket connection is not opened,
       nothing is done at the moment. */
    RetVal_SocketOk = SoAd_Int_04_SetupSoCon(SoConFirstId, 1U, SoConPtr, SoConStatePtr);
  }
  else
  {
    const SoAd_SoConIdType ListenSocketCapacity = SoAd_PBcfg->ListenSocketCapacity;
    /* scan the ListenSocketSoConMap lookup table if a related listen socket already exists for
     * this socket connection */
    SOAD_ENTER_CRITSEC();
    for(Idx = 0U; Idx < ListenSocketCapacity; Idx++)
    {
      if(SOAD_PBRAM(ListenSocketSoConMap, Idx).SoConListFirstId == SoConFirstId)
      {
        CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) soConStateFirst =
            &SOAD_PBRAM(SoConState, SoConFirstId);

        /* the local port is already defined for the listenSocket */
        SoConStatePtr->AssignedLocalPort = soConStateFirst->AssignedLocalPort;

        RetVal_SocketOk = E_OK;
        break;
      }
    }
    SOAD_EXIT_CRITSEC();

    /* if no ListenSocketSoConMap entry exists a new listenSocket must be requested */
    if(RetVal_SocketOk != E_OK)
    {
      /* open a new listenSocket and this socket connection */
      RetVal_SocketOk = SoAd_Int_04_SetupSoCon
      (
        SoConFirstId,
        SoConGroupPtr->SoConCnt,
        SoConPtr,
        SoConStatePtr
      );
    }
  }

  if(RetVal_SocketOk != E_OK)
  {
    SOAD_FLAG_SET_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TCPIP_SOCKET_CLOSED);

    /* Set global flag to know that for a certain SoCon a transition condition was met. */
    SoAd_SetBitInTransCondChange(SoConPtr);
  }

  DBG_SOAD_INT_04_SETUPGROUPSOCON_EXIT(SoConPtr, SoConStatePtr);
}

STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_Int_04_SetupSoCon
(
  SoAd_SoConIdType SoConFirstId,
  SoAd_SoConIdType SoConCnt,
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  const boolean IsListenSocket = (
                                   (!SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_TCP_INITIATE))
                                 ) ? TRUE : FALSE;
  TcpIp_SocketIdType SocketId = SOAD_SOCKETID_INVALID;
  Std_ReturnType RetVal_SocketOk;
  boolean AddedToList = FALSE;

  DBG_SOAD_INT_04_SETUPSOCON_ENTRY(SoConFirstId, SoConCnt, SoConPtr, SoConStatePtr);

  /* request a new socket */
  RetVal_SocketOk = SoAd_RequestTcpIpSocket(SoConPtr, SoConStatePtr, &SocketId);

  if(RetVal_SocketOk == E_OK)
  {
    /* proceed with protocol specific initialization steps */
    if(IsListenSocket)
    {
      /* TCP Server: start to listen */
      RetVal_SocketOk = TcpIp_TcpListen(SocketId, SoConCnt);
    }
    else
    {
#if (SOAD_IPV6_SUPPORT == STD_ON)
      TcpIp_SockAddrInet6Type RemoteAddr;
#else
      TcpIp_SockAddrInetType RemoteAddr;
#endif /* (SOAD_IPV6_SUPPORT == STD_ON) */
      /* Deviation MISRAC2012-4 */
      CONSTP2VAR(TcpIp_SockAddrType, AUTOMATIC, AUTOMATIC) RemoteAddrPtr =
              (P2VAR(TcpIp_SockAddrType, AUTOMATIC, AUTOMATIC))
              &RemoteAddr;

      SoAd_AssignRemoteAddressSoCon2SockAddrType(SoConPtr, SoConStatePtr, RemoteAddrPtr);

      /* TCP Client: connect to server */
      RetVal_SocketOk = TcpIp_TcpConnect(SocketId, RemoteAddrPtr);
    }

    if(RetVal_SocketOk != E_OK)
    {
      /* TCP connection setup failed - release the socket again */
      (void)TcpIp_Close(SocketId, TRUE);

      /* Precondition to enter state RESTART to repeat the connecting process */
      SOAD_FLAG_SET_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TCPIP_SOCKET_CLOSED);

      /* Set global flag to know that for a certain SoCon a transition condition was met. */
      SoAd_SetBitInTransCondChange(SoConPtr);
    }
  }

  if(RetVal_SocketOk == E_OK)
  {
    SoAd_SoConIdType Idx;

    /* create mapping from socketId to soConId, respectively listenSocketId to soConFirstId */
    if(IsListenSocket)
    {
      const SoAd_SoConIdType ListenSocketCapacity = SoAd_PBcfg->ListenSocketCapacity;

      SOAD_ENTER_CRITSEC();
      for(Idx = 0U; (Idx < ListenSocketCapacity) && (AddedToList == FALSE); Idx++)
      {
        CONSTP2VAR(SoAd_ListenSocketSoConMapType, AUTOMATIC, SOAD_APPL_DATA) ListenMapEntry =
          &SOAD_PBRAM(ListenSocketSoConMap, Idx);

        if(ListenMapEntry->SoConListFirstId == SOAD_INVALID_SOCON_ID)
        {
          ListenMapEntry->SoConListFirstId = SoConFirstId;
          ListenMapEntry->ListenSocketId = SocketId;
          AddedToList = TRUE;
        }
      }
      SOAD_EXIT_CRITSEC();

    }
    else
    {
      SOAD_ENTER_CRITSEC();
      for(Idx = 0U; (Idx < SoAd_PBcfg->SoConCnt) && (AddedToList == FALSE); Idx++)
      {
        CONSTP2VAR(SoAd_SocketSoConMapType, AUTOMATIC, SOAD_APPL_DATA) SocketMapEntry =
          &SOAD_PBRAM(SocketSoConMap, Idx);

        if(SocketMapEntry->SoConId == SOAD_INVALID_SOCON_ID)
        {
          SocketMapEntry->SoConId = SoConFirstId;
          SocketMapEntry->SocketId = SocketId;
          AddedToList = TRUE;
        }
      }

      /* socket request was successful, store SocketId */
      SoConStatePtr->SocketId = SocketId;

      SOAD_EXIT_CRITSEC();

    }

    /* Assert if free space in the map found. */
    SOAD_POSTCONDITION_ASSERT(AddedToList == TRUE, SOAD_INTERNAL_SVCID);
  }

  DBG_SOAD_INT_04_SETUPSOCON_EXIT(SocketOk, SoConFirstId, SoConCnt, SoConPtr, SoConStatePtr);
  return RetVal_SocketOk;
}

#if(TS_MERGED_COMPILE == STD_OFF)
#define SOAD_STOP_SEC_CODE
#include <SoAd_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

/*==================[end of file]===========================================*/
