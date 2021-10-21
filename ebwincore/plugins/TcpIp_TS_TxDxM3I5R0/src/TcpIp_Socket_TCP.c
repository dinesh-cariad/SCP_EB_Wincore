/**
 * \file
 *
 * \brief AUTOSAR TcpIp
 *
 * This file contains the implementation of the AUTOSAR
 * module TcpIp.
 *
 * \version 3.5.13
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
 * MISRAC2012-8) Deviated Rule: 11.3 (required)
 * A cast shall not be performed between a pointer to object type and a pointer to a different
 * object type.
 *
 * Reason:
 * Sockets come in variants for UDP and TCP. To enable common handling, they must be cast
 * to the generic type from which they are derived as C does not have derived types.
 *
 */

/*==================[inclusions]============================================*/

#include <TSMem.h>                /* memory functions */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Tcp_Int.h>
#include <TcpIp_Socket_Priv.h>   /* own SW-unit internal header file; must be last */

#if (TCPIP_TCP_ENABLED == STD_ON)

/*==================[macros]================================================*/

/** \brief Helper macro to copy the address of a \a domain specific socket from \a src to \a dest,
 *  looking up the correct size. */
#define TcpIp_Socket_setRemoteAddr_TCP(protocolDomainConstDescPtr, dest, src) \
  TS_MemCpy32_NoCheck((dest), \
                      (src),  \
                      (protocolDomainConstDescPtr)->remoteSize)

/** \brief Helper macro to compare the address of a \a domain specific socket at \a socketAddr1 to
 *  \a socketAddr2, looking up the correct size. */
#define TcpIp_Socket_socketMatch_TCP(protocolDomainConstDescPtr, socketAddr1, socketAddr2) \
  (E_OK == TS_MemCmp32_NoCheck((socketAddr1), \
                               (socketAddr2), \
                               (protocolDomainConstDescPtr)->remoteSize))

/*==================[type definitions]======================================*/

/** \brief function pointer used to indicate which tcp close function shall be called */
typedef P2FUNC(void,TCPIP_CODE,TcpIp_Socket_abort_FctPtrType)
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>


/** \brief initializes the TCP sockets to default vales of the specified domain */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Socket_Tcp_initDomain
(
#if (TCPIP_IP_DUALSTACK == STD_ON)
  uint8 SocketPage
#else /* TCPIP_IP_DUALSTACK == STD_OFF */
  void
#endif /* TCPIP_IP_DUALSTACK */
);

/** \brief Immediately closes TCP sockets bound to a particular local address identifier of the
           specified domain */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Socket_Tcp_terminateSocketsDomain
(
  uint8 SocketPage,
  TcpIp_LocalAddrIdType localAddrId,
  TcpIp_Socket_abort_FctPtrType abortFctPtr
);

/** \brief Requests closing of TCP sockets bound to a particular local address identifier of the
           specified domain */
STATIC FUNC(boolean, TCPIP_CODE) TcpIp_Socket_Tcp_closeSocketsDomain
(
  uint8 SocketPage,
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief performs periodic tasks of a TCP socket domain, e.g. timeouts, transmissions */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Socket_Tcp_processTcpSocketsDomain
(
  uint8 SocketPage
);

/** \brief performs tasks of a TCP socket transmissions */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Socket_Tcp_processTcpSocketsDomainTx
(
  uint8 SocketPage
);

/** \brief returns socket from the free socket list (and removed it form there) */
STATIC FUNC_P2VAR(TcpIp_Socket_TCP_Generic_Type, TCPIP_APPL_DATA, TCPIP_CODE)
 TcpIp_Socket_Tcp_intGetSocket
(
#if (TCPIP_IP_DUALSTACK == STD_ON)
  uint8 SocketPage,
#endif /* (TCPIP_IP_DUALSTACK == STD_ON) */
  P2VAR(TcpIp_SocketIdType, AUTOMATIC, TCPIP_APPL_DATA) socketIdxPtr
);

/** \brief searches a socket socketIdx in list with entry nextEntryPtr and
 * returns pointer to the index pointing to the found socket */
STATIC FUNC_P2VAR(TcpIp_SocketIdType, TCPIP_APPL_DATA, TCPIP_CODE) TcpIp_Socket_Tcp_searchSocket
(
  TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr,
  P2VAR(TcpIp_SocketIdType,AUTOMATIC,TCPIP_APPL_DATA) nextEntryPtr,
  TcpIp_SocketIdType socketIdx
);

/** \brief searches a socket socketIdx in list with entry nextEntryPtr, removes
 * it from linked list and returns the pointer to the socket */
STATIC FUNC_P2VAR(TcpIp_Socket_TCP_Generic_Type, TCPIP_APPL_DATA, TCPIP_CODE)
 TcpIp_Socket_Tcp_searchAndRemoveSocket
(
  TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr,
  P2VAR(TcpIp_SocketIdType,AUTOMATIC,TCPIP_APPL_DATA) nextEntryPtr,
  TcpIp_SocketIdType socketIdx
);

/* loops through all sockets and eliminates the listenSocket reference if
 * pointing to a particular socketId */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Socket_Tcp_removeListenSocketReference
(
  TcpIp_SocketIdType socketId
);

/** \brief  looks-up sockets with local_port in a list nextEntryPtr, writes the
 * results (maximum of maxSocketInstances) to socketIdPtr and returns the number
 * of found sockets */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Socket_Tcp_lookupSingleList
(
  uint8 SocketPage,
  P2VAR(TcpIp_SocketIdType,AUTOMATIC,TCPIP_APPL_DATA) socketIdPtr,
  P2CONST(TcpIp_SocketIdType,AUTOMATIC,TCPIP_APPL_DATA) nextEntryPtr,
  P2VAR(TcpIp_Socket_TCP_Generic_PtrType, AUTOMATIC, TCPIP_APPL_DATA) socketPtrPtr,
  P2CONST(TcpIp_SockAddrType,AUTOMATIC,TCPIP_APPL_DATA) remoteAddrPtr,
  uint16 local_port
);

/** \brief returns pointer to a listen socket list (localAddrId or ANY) entry based on
           the localAddrId */
STATIC FUNC_P2VAR(TcpIp_SocketIdType, TCPIP_APPL_DATA, TCPIP_CODE)
 TcpIp_Socket_Tcp_autoGetListenSocketListEntryPtr
(
#if (TCPIP_IP_DUALSTACK == STD_ON)
  uint8 SocketPage,
#endif /* (TCPIP_IP_DUALSTACK == STD_ON) */
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief Creates cloned socket from socket in state LISTEN
 *
 * This function copies values from the listen socket to the new empty socket.
 *
 * \param[out] newSocketPtrPtr     Pointer to pointer of the new socket (clone)
 * \param[in]  newSocketIdx        Socket Index of the new socket (clone)
 * \param[in]  localAddrPtr        Local address information
 * \param[in]  remoteAddrPtr       Remote address information
 * \param[in]  listenSocketPtr     Pointer to socket in state LISTEN
 * \param[in]  listenSocketId      Socket ID of the socket in state LISTEN
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Socket_Tcp_cloneSocket
(
  P2VAR(TcpIp_Socket_TCP_Generic_PtrType, AUTOMATIC, TCPIP_APPL_DATA) newSocketPtrPtr,
  TcpIp_SocketIdType newSocketIdx,
  P2CONST(TcpIp_SockAddrLocalType,AUTOMATIC,TCPIP_APPL_DATA) localAddrPtr,
  P2CONST(TcpIp_SockAddrType,AUTOMATIC,TCPIP_APPL_DATA) remoteAddrPtr,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) listenSocketPtr,
  TcpIp_SocketIdType listenSocketId
);

#if (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON)

/** \brief Get socket which is not established yet
 *
 * This function loops over all sockets which are in use but not established yet.
 * If such a socket can be found it will be reused for a connection which has
 * been established with a SYN cookie.
 *
 * \param[in]     SocketPage       Indicates page index (UDP, TCP, IPv4, IPV6)
 * \param[in,out] socketPtrPtr     in: pointer to the listen socket
 *                                 out: pointer to the reused socket
 * \param[in,out] socketIdPtr      in: socket id of the listen socket
 *                                 out: socket id of the reused socket
 * \param[in]     remoteAddrPtr    Remote address information
 * \param[in]     localAddrPtr     Local address information
 *
 * \retval TCPIP_OK         Socket it state not established found and can be reused
 * \retval TCPIP_E_NOT_OK   No socket found
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Socket_Tcp_getSocketNotEstablished
(
  uint8 SocketPage,
  P2VAR(TcpIp_Socket_TCP_Generic_PtrType, AUTOMATIC, TCPIP_APPL_DATA) socketPtrPtr,
  P2VAR(TcpIp_SocketIdType,AUTOMATIC,TCPIP_APPL_DATA) socketIdPtr,
  P2CONST(TcpIp_SockAddrType,AUTOMATIC,TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(TcpIp_SockAddrLocalType,AUTOMATIC,TCPIP_APPL_DATA) localAddrPtr
);

/** \brief Get socket which is not established yet
 *
 * This function loops over all sockets attached to the active socket listen
 * for the given localAddr which are in use but not established yet.
 *
 * \param[in] listenSocketId             socket id of the listen socket
 * \param[in] localAddr                  Local address which indicates the active
 *                                       socket list shall be used
 * \param[in] protocolDomainConstDescPtr pointer to constant socket information
 *
 * \retval TCPIP_SOCKETID_INVALID  no socket in state not established found in the list
 * \retval else                    return matching socket
 */
STATIC FUNC(TcpIp_SocketIdType, TCPIP_CODE) TcpIp_Socket_Tcp_getSocketNotEstablishedList
(
  TcpIp_SocketIdType listenSocketId,
  TcpIp_LocalAddrIdType localAddr,
  TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr
);

#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON) */

/** \brief close TCP socket
 *
 * This function either aborts or closes the TCP connection through 3 way
 * handshake depending on the socket state and variable Abort.
 *
 * \param[in] SocketId    Identifier of the socket
 * \param[in] socketPtr   Pointer to the socket data strucutre
 * \param[in] Abort       TRUE: Abort connection with RST
 *                        FALSE: Close connection with FIN
 *
 * \retval E_OK       Socket closed successfully
 * \retval E_NOT_OK   Error occurred during closing
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Socket_Tcp_CloseTcp
(
  TcpIp_SocketIdType SocketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr,
  boolean Abort
);

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/
#define TCPIP_START_SEC_VAR_CLEARED_16
#include <TcpIp_MemMap.h>

#if (TCPIP_SOCKET_NUM_TCP_IPV4 > 0U)
/** \brief entry point into lists of active Tcp sockets, one list per local address */
VAR(TcpIp_SocketIdType,TCPIP_VAR_CLEARED)
  TcpIp_Socket_localAddrActiveSockets_TCP_IPV4[TCPIP_NUM_LOCALADDRENTRIES];

/** \brief entry point into lists of listen Tcp sockets, one list per local address */
VAR(TcpIp_SocketIdType,TCPIP_VAR_CLEARED)
  TcpIp_Socket_localAddrListenSockets_TCP_IPV4[TCPIP_NUM_LOCALADDRENTRIES];
#endif /* (TCPIP_SOCKET_NUM_TCP_IPV4 > 0U) */

#if (TCPIP_SOCKET_NUM_TCP_IPV6 > 0U)
/** \brief entry point into lists of active Tcp sockets, one list per local address */
VAR(TcpIp_SocketIdType,TCPIP_VAR_CLEARED)
  TcpIp_Socket_localAddrActiveSockets_TCP_IPV6[TCPIP_NUM_LOCALADDRENTRIES];

/** \brief entry point into lists of listen Tcp sockets, one list per local address */
VAR(TcpIp_SocketIdType,TCPIP_VAR_CLEARED)
  TcpIp_Socket_localAddrListenSockets_TCP_IPV6[TCPIP_NUM_LOCALADDRENTRIES];

#endif /* (TCPIP_SOCKET_NUM_TCP_IPV6 > 0U) */

#define TCPIP_STOP_SEC_VAR_CLEARED_16
#include <TcpIp_MemMap.h>

#define TCPIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

#if (TCPIP_SOCKET_NUM_TCP_IPV4 > 0U)
/** \brief TCP IPV4 socket memory space */
VAR(TcpIp_Socket_TCP_IPV4_Type,TCPIP_VAR_CLEARED)
  TcpIp_Socket_SocketMemory_TCP_IPV4[TcpIp_Socket_numSockets_TCP_IPV4];
#endif /* (TCPIP_SOCKET_NUM_TCP_IPV4 > 0U) */

#if (TCPIP_SOCKET_NUM_TCP_IPV6 > 0U)
/** \brief TCP IPV6 socket memory space */
VAR(TcpIp_Socket_TCP_IPV6_Type,TCPIP_VAR_CLEARED)
  TcpIp_Socket_SocketMemory_TCP_IPV6[TcpIp_Socket_numSockets_TCP_IPV6];
#endif /* (TCPIP_SOCKET_NUM_TCP_IPV6 > 0U) */

#define TCPIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[module internal function definitions]=======================*/

/*==================[unit internal function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Socket_Tcp_init(void)
{
#if (TCPIP_SOCKET_NUM_TCP_IPV4 > 0U)
  TcpIp_Socket_Tcp_initDomain(TCPIP_IP_IF_DUALSTACK(TcpIp_Socket_SocketPage_TCP_IPV4));
#endif /* (TCPIP_SOCKET_NUM_TCP_IPV4 > 0U) */
#if (TCPIP_SOCKET_NUM_TCP_IPV6 > 0U)
  TcpIp_Socket_Tcp_initDomain(TCPIP_IP_IF_DUALSTACK(TcpIp_Socket_SocketPage_TCP_IPV6));
#endif /* (TCPIP_SOCKET_NUM_TCP_IPV6 > 0U) */
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Socket_Tcp_GetSocket
(
  uint8 SocketPage,
  P2VAR(TcpIp_SocketIdType, AUTOMATIC, TCPIP_APPL_DATA) SocketIdPtr,
  uint8 sockOwnerId
)
{
  Std_ReturnType result = E_NOT_OK;
  TcpIp_SocketIdType socketIdx= 0u;
  P2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr;

  /* !LINKSTO TcpIp.Design.Socket.EA0.GetSocket,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  socketPtr =
    TcpIp_Socket_Tcp_intGetSocket(TCPIP_IP_DUALSTACK_PARAM(SocketPage) &socketIdx);

  /* !LINKSTO TcpIp.Design.Socket.EA0.GetSocket,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  if(socketPtr != NULL_PTR)
  {
    /* initialize communication parameters for Tcp */
    TcpIp_Tcp_changeSocketToDefault(socketPtr);

    /* mark the upper layer of this socket */
    socketPtr->generic.aligned.sockOwnerId = sockOwnerId;

    /* save socket Id to output parameter */
    *SocketIdPtr = TCPIP_SOCKET_CREATEID(SocketPage, socketIdx);

    /* set result to OK */
    result = E_OK;
  }

  return result;
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Socket_Tcp_Bind
(
  TcpIp_SocketIdType SocketId,
  TcpIp_LocalAddrIdType LocalAddrId,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) PortPtr
)
{
  Std_ReturnType retval = E_NOT_OK;


  /* extract socket index from socket ID */
  const TcpIp_SocketIdType socketIdx = TCPIP_SOCKET_GETINDEX(SocketId);
#if (TCPIP_SOCKET_DYNAMIC_DISPATCH == STD_ON)
  const uint8 SocketPage = TCPIP_SOCKET_GETPAGE(SocketId);
#endif /* (TCPIP_SOCKET_DYNAMIC_DISPATCH == STD_ON) */
  TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
      TcpIp_Socket_getProtocolDomainConstDescriptorPtr(TCP, SocketPage);

  CONSTP2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr =
    TcpIp_Socket_getIntSocketPtr(TCP, protocolDomainConstDescPtr, socketIdx);

  /* !LINKSTO TcpIp.Design.Socket.EA0.Bind,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();


  switch(socketPtr->generic.aligned.state)
  {
    case TCPIP_SOCKET_STATE_USED:
    {
      uint16 port;

      /* if the given port is wildcard, calculate an automatic Port */
      if(*PortPtr == TCPIP_PORT_ANY)
      {
        port =
          TcpIp_Socket_getPort(TCPIP_SOCKET_DYNAMIC_DISPATCH_IF(SocketPage));
        *PortPtr = port;
      }
      else
      {
        port = *PortPtr;
      }

      /* initialize local address and port */
      socketPtr->generic.aligned.local.port = port;
      socketPtr->generic.aligned.local.addrId = LocalAddrId;
      socketPtr->generic.aligned.state = TCPIP_SOCKET_STATE_BOUND;
      retval = E_OK;

      break;
    }

    case TCPIP_SOCKET_STATE_LISTEN: /* intended fall through */
    case TCPIP_SOCKET_STATE_BOUND: /* intended fall through */
    case TCPIP_SOCKET_STATE_ACTIVE:
      break;

    /* CHECK: NOPARSE */
    case TCPIP_SOCKET_STATE_UNUSED: /* intended fall through */
    default:
    {
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    }
    /* CHECK: PARSE */
  }

  /* !LINKSTO TcpIp.Design.Socket.EA0.Bind,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  return retval;
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Socket_Tcp_Close
(
  TcpIp_SocketIdType SocketId,
  boolean Abort
)
{
  Std_ReturnType retVal = E_NOT_OK;

  /* extract socket index from socket ID */
  const TcpIp_SocketIdType socketIdx = TCPIP_SOCKET_GETINDEX(SocketId);
#if (TCPIP_IP_DUALSTACK == STD_ON)
  const uint8 SocketPage = TCPIP_SOCKET_GETPAGE(SocketId);
#endif /* (TCPIP_IP_DUALSTACK == STD_ON) */
  TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
      TcpIp_Socket_getProtocolDomainConstDescriptorPtr(TCP, SocketPage);

  CONSTP2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr =
      TcpIp_Socket_getIntSocketPtr(TCP, protocolDomainConstDescPtr, socketIdx);

  uint8 localState;

  /* TcpIp.Design.Socket.EA0.Close,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  localState = socketPtr->generic.aligned.state;

  retVal = TcpIp_Socket_Tcp_CloseTcp(SocketId, socketPtr, Abort);

  if(retVal == E_OK)
  {
    TcpIp_Socket_ProtocolDomainDescriptorConstPtrType protocolDomainDescPtr =
        TcpIp_Socket_getProtocolDomainDescriptorPtr(TCP, SocketPage);

    switch(localState)
    {
      case TCPIP_SOCKET_STATE_LISTEN:
      {
#if (TCPIP_PRECONDITION_ASSERT_ENABLED == STD_ON)
        CONSTP2CONST(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) resultPtr =
#else
        (void)
#endif /* (TCPIP_PRECONDITION_ASSERT_ENABLED == STD_ON) */
          TcpIp_Socket_Tcp_searchAndRemoveSocket
            ( protocolDomainConstDescPtr,
              TcpIp_Socket_Tcp_autoGetListenSocketListEntryPtr
                ( TCPIP_IP_DUALSTACK_PARAM(SocketPage)
                  socketPtr->generic.aligned.local.addrId ),
              socketIdx
            );

        TCPIP_PRECONDITION_ASSERT(resultPtr != NULL_PTR, TCPIP_INTERNAL_API_ID);

        TcpIp_Socket_Tcp_removeListenSocketReference(SocketId);

        /* insert into free socket list */
        /* Deviation MISRAC2012-8 */
        TcpIp_Socket_insertSocket(TCP, protocolDomainConstDescPtr,
                                  &protocolDomainDescPtr->freeSockets,
                                  socketIdx);

        retVal = E_OK;
        break;
      }
      case TCPIP_SOCKET_STATE_ACTIVE:
      {
        if(Abort)
        {
          const TcpIp_SocketIdType listenSocketId = socketPtr->listenSocketId;
#if (TCPIP_PRECONDITION_ASSERT_ENABLED == STD_ON)
          CONSTP2CONST(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) resultPtr =
#else
          (void)
#endif /* (TCPIP_PRECONDITION_ASSERT_ENABLED == STD_ON) */
            TcpIp_Socket_Tcp_searchAndRemoveSocket
              ( protocolDomainConstDescPtr,
                &protocolDomainConstDescPtr->localAddrActiveSockets
                                      [socketPtr->generic.aligned.local.addrId],
                socketIdx
              );

          TCPIP_PRECONDITION_ASSERT(resultPtr != NULL_PTR, TCPIP_INTERNAL_API_ID);

          if(listenSocketId != TCPIP_SOCKETID_INVALID)
          {
            CONSTP2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) listenSocketPtr =
              /* Deviation MISRAC2012-8 */
              TcpIp_Socket_getIntSocketPtr
                (TCP, protocolDomainConstDescPtr, TCPIP_SOCKET_GETINDEX(socketPtr->listenSocketId));

            TCPIP_PRECONDITION_ASSERT
              ((listenSocketPtr->numConnections >= 1U), TCPIP_INTERNAL_API_ID);

            listenSocketPtr->numConnections--;

            if(socketPtr->numEstablishedConnections != 0U)
            {
              TCPIP_PRECONDITION_ASSERT
                ((listenSocketPtr->numEstablishedConnections >= 1U), TCPIP_INTERNAL_API_ID);

              listenSocketPtr->numEstablishedConnections--;
            }
          }

          /* insert into free socket list */
          /* Deviation MISRAC2012-8 */
          TcpIp_Socket_insertSocket
            (TCP, protocolDomainConstDescPtr, &protocolDomainDescPtr->freeSockets, socketIdx);

          retVal = E_OK;
        }
        else
        {
          retVal = E_OK;
        }
        break;
      }
      case TCPIP_SOCKET_STATE_USED: /* intended fall through */
      case TCPIP_SOCKET_STATE_BOUND:
      {
        /* insert into free socket list */
        /* Deviation MISRAC2012-8 <+3> */
        TcpIp_Socket_insertSocket
          (TCP, protocolDomainConstDescPtr, &protocolDomainDescPtr->freeSockets, socketIdx);

        retVal = E_OK;

        break;
      }
      /* CHECK: NOPARSE */
      case TCPIP_SOCKET_STATE_UNUSED: /* intended fall through */
      default:
      {
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break;
      }
      /* CHECK: PARSE */
    }
  }

  /* !LINKSTO TcpIp.Design.Socket.EA0.Close,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  return retVal;
}


TS_MOD_PRIV_DEFN FUNC(boolean, TCPIP_CODE) TcpIp_Socket_Tcp_closeSockets
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  boolean pendingSocket = FALSE;

#if (TCPIP_SOCKET_NUM_TCP_IPV4 > 0U)
    if(TcpIp_Socket_Tcp_closeSocketsDomain(TcpIp_Socket_SocketPage_TCP_IPV4, localAddrId) == TRUE)
    {
      pendingSocket = TRUE;
    }
#endif /* (TCPIP_SOCKET_NUM_TCP_IPV4 > 0U) */
#if (TCPIP_SOCKET_NUM_TCP_IPV6 > 0U)
    if(TcpIp_Socket_Tcp_closeSocketsDomain(TcpIp_Socket_SocketPage_TCP_IPV6, localAddrId) == TRUE)
    {
      pendingSocket = TRUE;
    }
#endif /* (TCPIP_SOCKET_NUM_TCP_IPV6 > 0U) */
    ;
  return pendingSocket;
}


TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Socket_Tcp_terminateSockets
(
  TcpIp_LocalAddrIdType localAddrId,
  boolean informRemoteHost
)
{
  TcpIp_Socket_abort_FctPtrType closeFctPtr;

  if(informRemoteHost)
  {
    /* abort connection and send reset */
    closeFctPtr = &TcpIp_Tcp_abort;
  }
  else
  {
    /* just abort connection without transmitting reset */
    closeFctPtr = &TcpIp_Tcp_terminate;
  }

#if (TCPIP_SOCKET_NUM_TCP_IPV4 > 0U)
    TcpIp_Socket_Tcp_terminateSocketsDomain(TcpIp_Socket_SocketPage_TCP_IPV4, localAddrId,  closeFctPtr);
#endif /* (TCPIP_SOCKET_NUM_TCP_IPV4 > 0U) */
#if (TCPIP_SOCKET_NUM_TCP_IPV6 > 0U)
    TcpIp_Socket_Tcp_terminateSocketsDomain(TcpIp_Socket_SocketPage_TCP_IPV6, localAddrId,  closeFctPtr);
#endif /* (TCPIP_SOCKET_NUM_TCP_IPV6 > 0U) */
    ;

}


TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Socket_listenTcp
(
  TcpIp_SocketIdType socketId,
  uint16 maxChannels
)
{
  TcpIp_ReturnType retVal = TCPIP_E_NOT_OK;

  const TcpIp_SocketIdType socketIdx = TCPIP_SOCKET_GETINDEX(socketId);
#if (TCPIP_IP_DUALSTACK == STD_ON)
  const uint8 SocketPage = TCPIP_SOCKET_GETPAGE(socketId);
#endif /* (TCPIP_IP_DUALSTACK == STD_ON) */
  TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
      TcpIp_Socket_getProtocolDomainConstDescriptorPtr(TCP, SocketPage);
  CONSTP2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr =
      TcpIp_Socket_getIntSocketPtr(TCP, protocolDomainConstDescPtr, socketIdx);

  DBG_TCPIP_SOCKET_LISTENTCP_ENTRY(socketId,maxChannels);

  /* !LINKSTO TcpIp.Design.Socket.EA0.listenTcp,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  if(socketPtr->generic.aligned.state == TCPIP_SOCKET_STATE_BOUND)
  {
    if(TcpIp_Ip_getIfState(socketPtr->generic.aligned.local.addrId) == TCPIP_STATE_ONLINE)
    {
      socketPtr->numConnections = 0U;
      socketPtr->numEstablishedConnections = 0U;
      socketPtr->maxConnections = maxChannels;
      socketPtr->generic.aligned.state = TCPIP_SOCKET_STATE_LISTEN;

      /* Deviation MISRAC2012-8 <+5> */
      TcpIp_Socket_insertSocket(TCP, protocolDomainConstDescPtr,
          TcpIp_Socket_Tcp_autoGetListenSocketListEntryPtr
            ( TCPIP_IP_DUALSTACK_PARAM(SocketPage)
              socketPtr->generic.aligned.local.addrId ),
          socketIdx  );

      retVal = TCPIP_OK;
    }
    else
    {
      retVal = TCPIP_E_ADDRNOTASSIGNED;
    }
  }

  /* !LINKSTO TcpIp.Design.Socket.EA0.listenTcp,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  DBG_TCPIP_SOCKET_LISTENTCP_EXIT(retVal,socketId,maxChannels);

  return retVal;
}


TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Socket_autoBindTcp
(
  TcpIp_SocketIdType socketId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2VAR(TcpIp_SockAddrLocalType,AUTOMATIC,TCPIP_APPL_DATA) localAddrPtr
)
{
  TcpIp_ReturnType retVal = TCPIP_E_NOT_OK;

  const TcpIp_SocketIdType socketIdx = TCPIP_SOCKET_GETINDEX(socketId);
  const uint8 SocketPage = TCPIP_SOCKET_GETPAGE(socketId);
  TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
      TcpIp_Socket_getProtocolDomainConstDescriptorPtr(TCP, SocketPage);
  CONSTP2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr =
      TcpIp_Socket_getIntSocketPtr(TCP, protocolDomainConstDescPtr, socketIdx);

  DBG_TCPIP_SOCKET_AUTOBINDTCP_ENTRY(socketId,remoteAddrPtr,localAddrPtr);

  /* !LINKSTO TcpIp.Design.Socket.EA0.autoBindActivateTcp,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  {
    const uint8 state = socketPtr->generic.aligned.state;

    P2VAR(TcpIp_LocalAddrIdType,AUTOMATIC,TCPIP_APPL_DATA) localAddrIdPtr = &localAddrPtr->addrId;
    P2VAR(uint16,AUTOMATIC,TCPIP_APPL_DATA) localPortPtr = &localAddrPtr->port;
    TcpIp_LocalAddrIdType originalLocalAddrId = 0U;

    if((state != TCPIP_SOCKET_STATE_UNUSED) && (state != TCPIP_SOCKET_STATE_ACTIVE))
    {
      if(state == TCPIP_SOCKET_STATE_USED)
      {
        *localAddrIdPtr = TcpIp_Ip_getTxLocalAddrId_byRemoteIp(remoteAddrPtr);
        *localPortPtr =
          TcpIp_Socket_getPort(TCPIP_SOCKET_DYNAMIC_DISPATCH_IF(SocketPage));
      }
      else
      {
        originalLocalAddrId = socketPtr->generic.aligned.local.addrId;

        /* get a localAddrId we can use for transmission */
        if(originalLocalAddrId == TCPIP_LOCALADDRID_ANY)
        {
          *localAddrIdPtr = TcpIp_Ip_getTxLocalAddrId_byRemoteIp(remoteAddrPtr);
        }
        else
        {
          const TcpIp_LocalAddrIdType AnylocalAddrId = TcpIp_IpAddrM_getAnyIpLocalAddrId(originalLocalAddrId);

           if(AnylocalAddrId == originalLocalAddrId)
           {
             /* if socket is bound to ANY, select an appropriate unicast address */
             *localAddrIdPtr = TcpIp_Ip_getTxLocalAddrId_byLocalAddrId(originalLocalAddrId, remoteAddrPtr);
           }
           else
           {
             /* otherwise, use the bound address */
             *localAddrIdPtr = originalLocalAddrId;
           }
        }

        *localPortPtr = socketPtr->generic.aligned.local.port;
      }

      if(*localAddrIdPtr != TCPIP_LOCALADDRID_INVALID)
      {
        if(TcpIp_IpAddrM_getIfState(*localAddrIdPtr) == TCPIP_STATE_ONLINE)
        {
          TcpIp_SocketIdType searchId;
          P2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) searchPtr;
          if(TcpIp_Socket_Tcp_lookupSingleList(
              SocketPage,
              &searchId,
              &protocolDomainConstDescPtr->localAddrActiveSockets[*localAddrIdPtr],
              &searchPtr,
              remoteAddrPtr,
              *localPortPtr) != TCPIP_OK)
          {
            retVal = TCPIP_OK;
          }
          else
          {
            /* connection already exists */
            retVal = TCPIP_E_NOT_PERMITTED;
          }
        }
        else
        {
          /* address is not ONLINE */
          retVal = TCPIP_E_ADDRNOTASSIGNED;
        }
      }
      else
      {
        /* no unicast address available for transmission */
        retVal = TCPIP_E_ADDRNOTASSIGNED;
      }
    }
  }

  /* !LINKSTO TcpIp.Design.Socket.EA0.autoBindActivateTcp,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  DBG_TCPIP_SOCKET_AUTOBINDTCP_EXIT(retVal,socketId,remoteAddrPtr,localAddrPtr);

  return retVal;
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Socket_activateTcp
(
  TcpIp_SocketIdType socketId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(TcpIp_SockAddrLocalType,AUTOMATIC,TCPIP_APPL_DATA) localAddrPtr
)
{
  const TcpIp_SocketIdType socketIdx = TCPIP_SOCKET_GETINDEX(socketId);
#if (TCPIP_IP_DUALSTACK == STD_ON)
  const uint8 SocketPage = TCPIP_SOCKET_GETPAGE(socketId);
#endif /* (TCPIP_IP_DUALSTACK == STD_ON) */
  TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
      TcpIp_Socket_getProtocolDomainConstDescriptorPtr(TCP, SocketPage);
  CONSTP2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr =
      TcpIp_Socket_getIntSocketPtr(TCP, protocolDomainConstDescPtr, socketIdx);
  const TcpIp_LocalAddrIdType localAddrId = localAddrPtr->addrId;
  const uint16 localPort = localAddrPtr->port;

  DBG_TCPIP_SOCKET_ACTIVATETCP_ENTRY(socketId,remoteAddrPtr,localAddrPtr);

  /* !LINKSTO TcpIp.Design.Socket.EA0.autoBindActivateTcp,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  /* if we are currently listening, kick it out of the listen list */
  if(socketPtr->generic.aligned.state == TCPIP_SOCKET_STATE_LISTEN)
  {
    TcpIp_LocalAddrIdType originalLocalAddrId = socketPtr->generic.aligned.local.addrId;

    TcpIp_Socket_Tcp_removeListenSocketReference(socketId);
    (void)TcpIp_Socket_Tcp_searchAndRemoveSocket
            ( protocolDomainConstDescPtr,
              TcpIp_Socket_Tcp_autoGetListenSocketListEntryPtr
                (TCPIP_IP_DUALSTACK_PARAM(SocketPage) originalLocalAddrId),
              socketIdx
            );
  }

  /* save local address information into socket */
  socketPtr->listenSocketId = TCPIP_SOCKETID_INVALID;
  socketPtr->generic.aligned.local.addrId = localAddrId;
  socketPtr->generic.aligned.local.port = localPort;
  TcpIp_Socket_setRemoteAddr_TCP(protocolDomainConstDescPtr, &socketPtr->remote, remoteAddrPtr);
  socketPtr->generic.aligned.state = TCPIP_SOCKET_STATE_ACTIVE;
  socketPtr->numEstablishedConnections = 1U;

  /* Deviation MISRAC2012-8 <+4> */
  TcpIp_Socket_insertSocket(TCP, protocolDomainConstDescPtr,
      &protocolDomainConstDescPtr->localAddrActiveSockets[localAddrId],
      socketIdx  );

  /* !LINKSTO TcpIp.Design.Socket.EA0.autoBindActivateTcp,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  DBG_TCPIP_SOCKET_ACTIVATETCP_EXIT(socketId,remoteAddrPtr,localAddrPtr);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Socket_closeTcp
(
  TcpIp_SocketIdType socketId
)
{
  const TcpIp_SocketIdType socketIdx = TCPIP_SOCKET_GETINDEX(socketId);
#if (TCPIP_IP_DUALSTACK == STD_ON)
  const uint8 SocketPage = TCPIP_SOCKET_GETPAGE(socketId);
#endif /* (TCPIP_IP_DUALSTACK == STD_ON) */
  TcpIp_Socket_ProtocolDomainDescriptorConstPtrType protocolDomainDescPtr=
    TcpIp_Socket_getProtocolDomainDescriptorPtr(TCP, SocketPage);
  TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
      TcpIp_Socket_getProtocolDomainConstDescriptorPtr(TCP, SocketPage);

  CONSTP2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr =
      TcpIp_Socket_getIntSocketPtr(TCP, protocolDomainConstDescPtr, socketIdx);

  TcpIp_LocalAddrIdType emptyLocalAddrId = TCPIP_LOCALADDRID_INVALID;

  DBG_TCPIP_SOCKET_CLOSETCP_ENTRY(socketId);

  /* !LINKSTO TcpIp.Design.Socket.EA0.closeTcp,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  if(socketPtr->generic.aligned.state == TCPIP_SOCKET_STATE_ACTIVE)
  {
    const TcpIp_LocalAddrIdType localAddrId = socketPtr->generic.aligned.local.addrId;

    (void)TcpIp_Socket_Tcp_searchAndRemoveSocket
      ( protocolDomainConstDescPtr,
        &protocolDomainConstDescPtr->localAddrActiveSockets[localAddrId],
        socketIdx
      );

    if(socketPtr->listenSocketId != TCPIP_SOCKETID_INVALID)
    {
      CONSTP2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) listenSocketPtr =
      /* Deviation MISRAC2012-8 */
          TcpIp_Socket_getIntSocketPtr
            (TCP, protocolDomainConstDescPtr, TCPIP_SOCKET_GETINDEX(socketPtr->listenSocketId));

      TCPIP_PRECONDITION_ASSERT
        ((listenSocketPtr->numConnections >= 1U), TCPIP_INTERNAL_API_ID);

      listenSocketPtr->numConnections--;

      if(socketPtr->numEstablishedConnections != 0U)
      {
        TCPIP_PRECONDITION_ASSERT
          ((listenSocketPtr->numEstablishedConnections >= 1U), TCPIP_INTERNAL_API_ID);

        listenSocketPtr->numEstablishedConnections--;
      }
    }

    socketPtr->generic.aligned.state = TCPIP_SOCKET_STATE_UNUSED;

    /* Deviation MISRAC2012-8 <+3> */
    TcpIp_Socket_insertSocket(TCP, protocolDomainConstDescPtr,
        &protocolDomainDescPtr->freeSockets,
        socketIdx  );

    if(protocolDomainConstDescPtr->localAddrActiveSockets[localAddrId]
       == TCPIP_SOCKETID_INVALID)
    {
      emptyLocalAddrId = localAddrId;
    }
  }

  /* !LINKSTO TcpIp.Design.Socket.EA0.closeTcp,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  if(emptyLocalAddrId != TCPIP_LOCALADDRID_INVALID)
  {
    /* indicate closed sockets to IpAddrM state machine */
    TcpIp_IpAddrM_socketsClosed(emptyLocalAddrId);
  }

  DBG_TCPIP_SOCKET_CLOSETCP_EXIT(socketId);
}


TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Socket_processTcpSockets
(
  void
)
{
#if (TCPIP_SOCKET_NUM_TCP_IPV4 > 0U)
  TcpIp_Socket_Tcp_processTcpSocketsDomain(TcpIp_Socket_SocketPage_TCP_IPV4);
#endif /* (TCPIP_SOCKET_NUM_TCP_IPV4 > 0U) */
#if (TCPIP_SOCKET_NUM_TCP_IPV6 > 0U)
  TcpIp_Socket_Tcp_processTcpSocketsDomain(TcpIp_Socket_SocketPage_TCP_IPV6);
#endif /* (TCPIP_SOCKET_NUM_TCP_IPV6 > 0U) */
}


TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Socket_processTcpSocketsTx
(
  void
)
{
#if (TCPIP_SOCKET_NUM_TCP_IPV4 > 0U)
  TcpIp_Socket_Tcp_processTcpSocketsDomainTx(TcpIp_Socket_SocketPage_TCP_IPV4);
#endif /* (TCPIP_SOCKET_NUM_TCP_IPV4 > 0U) */
#if (TCPIP_SOCKET_NUM_TCP_IPV6 > 0U)
  TcpIp_Socket_Tcp_processTcpSocketsDomainTx(TcpIp_Socket_SocketPage_TCP_IPV6);
#endif /* (TCPIP_SOCKET_NUM_TCP_IPV6 > 0U) */
}


TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Socket_lookupTcp
(
  P2CONST(TcpIp_SockAddrLocalType,AUTOMATIC,TCPIP_APPL_DATA) localAddrPtr,
  P2CONST(TcpIp_SockAddrType,AUTOMATIC,TCPIP_APPL_DATA) remoteAddrPtr,
  P2VAR(TcpIp_Socket_TCP_Generic_PtrType, AUTOMATIC, TCPIP_APPL_DATA) socketPtrPtr,
  P2VAR(TcpIp_SocketIdType,AUTOMATIC,TCPIP_APPL_DATA) socketIdPtr
)
{
  Std_ReturnType retVal = TCPIP_E_NOT_OK;
  const TcpIp_LocalAddrIdType localAddrId = localAddrPtr->addrId;
  const uint16 port = localAddrPtr->port;

  DBG_TCPIP_SOCKET_LOOKUPTCP_ENTRY(localAddrPtr,remoteAddrPtr,socketPtrPtr,socketIdPtr);
  {
#if (TCPIP_IP_DUALSTACK == STD_ON)
    const uint8 addrInfo = TcpIp_IpAddrM_getLocalAddrInfo(localAddrId);
#endif /* (TCPIP_IP_DUALSTACK == STD_ON) */
    const uint8 SocketPage = TCPIP_SOCKET_CREATEPAGE(TCP, addrInfo);
#if ( TCPIP_UDP_ENABLED == STD_ON ) && ( TCPIP_TCP_ENABLED == STD_ON )
    if(TCPIP_SOCKET_ISVALIDSOCKETPAGE(SocketPage))
#endif /* ( TCPIP_UDP_ENABLED == STD_ON ) && ( TCPIP_TCP_ENABLED == STD_ON ) */
    {
      TcpIp_Socket_ProtocolDomainDescriptorConstPtrType protocolDomainDescPtr=
        TcpIp_Socket_getProtocolDomainDescriptorPtr(TCP, SocketPage);
      TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
          TcpIp_Socket_getProtocolDomainConstDescriptorPtr(TCP, SocketPage);

      /* !LINKSTO TcpIp.Design.Socket.EA0.lookupTcp,1 */
      SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

      /* search active sockets */
      retVal = TcpIp_Socket_Tcp_lookupSingleList(
                  SocketPage,
                  socketIdPtr,
                  &protocolDomainConstDescPtr->localAddrActiveSockets[localAddrId],
                  socketPtrPtr,
                  remoteAddrPtr,
                  port);

      /* we search the listen socket list second */
      if((retVal != TCPIP_OK))
      {
        TcpIp_SocketIdType socketId;
        TcpIp_Socket_TCP_Generic_PtrType socketPtr;

        /* search listen sockets bound to a concrete localAddrId */
        retVal = TcpIp_Socket_Tcp_lookupSingleList(
                    SocketPage,
                    &socketId,
                    &protocolDomainConstDescPtr->localAddrListenSockets[localAddrId],
                    &socketPtr,
                    NULL_PTR,
                    port);

        if(retVal != TCPIP_OK)
        {
          const TcpIp_LocalAddrIdType AnylocalAddrId = TcpIp_IpAddrM_getAnyIpLocalAddrId(localAddrId);
          if(AnylocalAddrId != TCPIP_LOCALADDRID_INVALID)
          {
            /* search listen sockets bound to a controller wildcard */
            retVal = TcpIp_Socket_Tcp_lookupSingleList
              (
                SocketPage,
                &socketId,
                &protocolDomainConstDescPtr->localAddrListenSockets[AnylocalAddrId],
                &socketPtr,
                NULL_PTR,
                port
              );

          }
        }

        if(retVal != TCPIP_OK)
        {
          /* search listen sockets bound to a global wildcard */
          retVal = TcpIp_Socket_Tcp_lookupSingleList(
                      SocketPage,
                      &socketId,
                      &protocolDomainDescPtr->anySockets,
                      &socketPtr,
                      NULL_PTR,
                      localAddrPtr->port);
        }

        if(retVal == TCPIP_OK)
        {
          *socketIdPtr = socketId;
          *socketPtrPtr = socketPtr;
        }
      }

      /* !LINKSTO TcpIp.Design.Socket.EA0.lookupTcp,1 */
      SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();
    }
  }

  DBG_TCPIP_SOCKET_LOOKUPTCP_EXIT(retVal,localAddrPtr,remoteAddrPtr,socketPtrPtr,socketIdPtr);

  return retVal;
}


TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Socket_Tcp_cloneListenSocket
(
  P2CONST(TcpIp_SockAddrLocalType,AUTOMATIC,TCPIP_APPL_DATA) localAddrPtr,
  P2CONST(TcpIp_SockAddrType,AUTOMATIC,TCPIP_APPL_DATA) remoteAddrPtr,
  P2VAR(TcpIp_Socket_TCP_Generic_PtrType, AUTOMATIC, TCPIP_APPL_DATA) socketPtrPtr,
  P2VAR(TcpIp_SocketIdType,AUTOMATIC,TCPIP_APPL_DATA) socketIdPtr,
  boolean alsoCheckNotEstablished
)
{
  Std_ReturnType retVal = TCPIP_E_NOT_OK;
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) listenSocketPtr = *socketPtrPtr;
  TcpIp_SocketIdType listenSocketId = *socketIdPtr;
  const uint8 SocketPage = TCPIP_SOCKET_GETPAGE(listenSocketId);
  TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
      TcpIp_Socket_getProtocolDomainConstDescriptorPtr(TCP, SocketPage);

  DBG_TCPIP_SOCKET_TCP_CLONELISTENSOCKET_ENTRY
    (localAddrPtr,remoteAddrPtr,socketPtrPtr,socketIdPtr,alsoCheckNotEstablished);

#if(TCPIP_TCP_SYN_COOKIES_ENABLED == STD_OFF)
  TS_PARAM_UNUSED(alsoCheckNotEstablished);
#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_OFF) */

  /* !LINKSTO TcpIp.Design.Socket.EA0.cloneListenSocket,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  /* are there to many established connections for this listen socket ? */
  if
  (
    (TCPIP_TCP_SOCKETS_INFINITE_PASSIVE_CONNECTIONS_AVAIL(listenSocketPtr)) ||
    (listenSocketPtr->maxConnections > listenSocketPtr->numEstablishedConnections)
  )
  {
    P2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) newSocketPtr = NULL_PTR;
    /* Setting invalid value */
    TcpIp_SocketIdType socketIdx  = protocolDomainConstDescPtr->numSockets;

    /* are there to many active connections for this listen socket ? */
    if
    (
      (TCPIP_TCP_SOCKETS_INFINITE_PASSIVE_CONNECTIONS_AVAIL(listenSocketPtr)) ||
      (listenSocketPtr->maxConnections > listenSocketPtr->numConnections)
    )
    {
      /* get a new socket */
      newSocketPtr = TcpIp_Socket_Tcp_intGetSocket(
         TCPIP_IP_DUALSTACK_PARAM(SocketPage)
         &socketIdx );

      /* success ? */
      if(newSocketPtr != NULL_PTR)
      {

        TcpIp_Socket_Tcp_cloneSocket
               (
                 &newSocketPtr,
                 socketIdx,
                 localAddrPtr,
                 remoteAddrPtr,
                 listenSocketPtr,
                 listenSocketId
               );

        /* set output parameters */
        *socketIdPtr = TCPIP_SOCKET_CREATEID(SocketPage, socketIdx);
        *socketPtrPtr = newSocketPtr;
        retVal = TCPIP_OK;
      }
#if (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON)
      else if(listenSocketPtr->numEstablishedConnections == listenSocketPtr->numConnections)
      {
        /* number of used and established sockets is equal and no free socket left,
         * connection cannot be established */
        retVal = TCPIP_E_NOT_OK;
      }
      else
      {
        /* there are some connections which are not fully established yet,
         * maybe due to SYN flooding? */
        retVal = TCPIP_E_PENDING;
      }
#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_OFF) */
    }
    else
    {
#if (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON)
      /* there are some connections which are not fully established yet,
       * maybe due to SYN flooding? */
      retVal = TCPIP_E_PENDING;
#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_OFF) */
    }

#if (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON)
    if((retVal == TCPIP_E_PENDING) && alsoCheckNotEstablished)
    {
      retVal =
        TcpIp_Socket_Tcp_getSocketNotEstablished
          (SocketPage, socketPtrPtr, socketIdPtr, remoteAddrPtr, localAddrPtr);
    }
#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_OFF) */
  }
  /* else: maximal allowed sockets are established for this listen socket, do
   * not allow any further connection */

  /* !LINKSTO TcpIp.Design.Socket.EA0.cloneListenSocket,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  DBG_TCPIP_SOCKET_TCP_CLONELISTENSOCKET_EXIT
    (retVal,localAddrPtr,remoteAddrPtr,socketPtrPtr,socketIdPtr,alsoCheckNotEstablished);

  return retVal;
}

#if (TCPIP_TCP_CRYPTO_ENABLED == STD_ON)
TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Socket_Tcp_setHashData
(
  P2CONST(TcpIp_SockAddrLocalType,AUTOMATIC,TCPIP_APPL_DATA) localAddrPtr,
  P2CONST(TcpIp_SockAddrType,AUTOMATIC,TCPIP_APPL_DATA) remoteAddrPtr,
  P2VAR(uint8,AUTOMATIC,TCPIP_APPL_DATA) hashDataPtr,
  P2VAR(uint16,AUTOMATIC,TCPIP_APPL_DATA) lengthDataPtr,
  TcpIp_SocketIdType socketId
)
{

  DBG_TCPIP_SOCKET_TCP_SETHASHDATA_ENTRY(localAddrPtr,remoteAddrPtr,hashDataPtr,lengthDataPtr,socketId);
  {
#if (TCPIP_IP_DUALSTACK == STD_ON)
    const uint8 SocketPage = TCPIP_SOCKET_GETPAGE(socketId);
#endif /* (TCPIP_IP_DUALSTACK == STD_ON) */
    TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr =
            TcpIp_Socket_getProtocolDomainConstDescriptorPtr(TCP, SocketPage);

#if (TCPIP_IP_DUALSTACK == STD_OFF)
    TS_PARAM_UNUSED(socketId);
#endif /* (TCPIP_IP_DUALSTACK == STD_ON) */

    TcpIp_Socket_setRemoteAddr_TCP
    (
      protocolDomainConstDescPtr,
      &hashDataPtr[0U],
      remoteAddrPtr
    );
    /* add domain, source address and port */
    *lengthDataPtr = (protocolDomainConstDescPtr)->remoteSize;

    /* add destination address */
    hashDataPtr[*lengthDataPtr] = localAddrPtr->addrId;
    (*lengthDataPtr) ++;

    /* add destination port */
    TS_MemCpy(&hashDataPtr[*lengthDataPtr],&localAddrPtr->port,sizeof(localAddrPtr->port));
    *lengthDataPtr += sizeof(localAddrPtr->port);
  }

  DBG_TCPIP_SOCKET_TCP_SETHASHDATA_EXIT(localAddrPtr,remoteAddrPtr,hashDataPtr,lengthDataPtr,socketId);
}

#endif /* (TCPIP_TCP_CRYPTO_ENABLED == STD_ON) */

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Socket_Tcp_establishConnection
(
  TcpIp_SocketIdType socketId
)
{
  DBG_TCPIP_SOCKET_TCP_ESTABLISHCONNECTION_ENTRY(socketId);

  /* !LINKSTO TcpIp.Design.Socket.EA0.establishConnection,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();
  {
    const TcpIp_SocketIdType socketIdx = TCPIP_SOCKET_GETINDEX(socketId);
    TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
        TcpIp_Socket_getProtocolDomainConstDescriptorPtr(TCP, TCPIP_SOCKET_GETPAGE(socketId));

    CONSTP2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr =
      /* Deviation MISRAC2012-8 */
      TcpIp_Socket_getIntSocketPtr(
          TCP,
          protocolDomainConstDescPtr,
          socketIdx);

    TCPIP_PRECONDITION_ASSERT((socketPtr->numEstablishedConnections == 0U), TCPIP_INTERNAL_API_ID);

    /* indicate that socket is established */
    socketPtr->numEstablishedConnections++;

    TCPIP_PRECONDITION_ASSERT((socketPtr->listenSocketId != TCPIP_SOCKETID_INVALID), TCPIP_INTERNAL_API_ID);
    {
      const TcpIp_SocketIdType listenSocketIdx = TCPIP_SOCKET_GETINDEX(socketPtr->listenSocketId);
      CONSTP2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) listenSocketPtr =
            /* Deviation MISRAC2012-8 */
            TcpIp_Socket_getIntSocketPtr(
                TCP,
                protocolDomainConstDescPtr,
                listenSocketIdx);

      listenSocketPtr->numEstablishedConnections++;
    }
  }
  /* !LINKSTO TcpIp.Design.Socket.EA0.establishConnection,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  DBG_TCPIP_SOCKET_TCP_ESTABLISHCONNECTION_EXIT(socketId);
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_SocketIdType, TCPIP_CODE) TcpIp_Socket_getListenSocket
(
  TcpIp_SocketIdType socketId
)
{
  TcpIp_SocketIdType listenSocketId = TCPIP_SOCKETID_INVALID;

  DBG_TCPIP_SOCKET_GETLISTENSOCKET_ENTRY(socketId);
  {
    const TcpIp_SocketIdType socketIdx = TCPIP_SOCKET_GETINDEX(socketId);
    TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
        TcpIp_Socket_getProtocolDomainConstDescriptorPtr(TCP, TCPIP_SOCKET_GETPAGE(socketId));

    CONSTP2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr =
      /* Deviation MISRAC2012-8 */
      TcpIp_Socket_getIntSocketPtr(
          TCP,
          protocolDomainConstDescPtr,
          socketIdx);

    if(socketPtr->generic.aligned.state == TCPIP_SOCKET_STATE_ACTIVE)
    {
      listenSocketId = socketPtr->listenSocketId;
    }
  }
  DBG_TCPIP_SOCKET_GETLISTENSOCKET_EXIT(listenSocketId,socketId);

  return listenSocketId;
}

TS_MOD_PRIV_DEFN FUNC(boolean, TCPIP_CODE) TcpIp_Socket_Tcp_isPortUsed
(
#if (TCPIP_IP_DUALSTACK == STD_ON)
  uint8 SocketPage,
#endif /* (TCPIP_IP_DUALSTACK == STD_ON) */
  uint16 port
)
{
  TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
      TcpIp_Socket_getProtocolDomainConstDescriptorPtr(TCP, SocketPage);

  boolean portAlreadyUsed = FALSE;
  uint16_least i;

  for(i = 0U; (i < protocolDomainConstDescPtr->numSockets) && (portAlreadyUsed == FALSE); i++)
  {
    CONSTP2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr =
      TcpIp_Socket_getIntSocketPtr(TCP, protocolDomainConstDescPtr, (TcpIp_SocketIdType) i);

    switch(socketPtr->generic.aligned.state)
    {
      case TCPIP_SOCKET_STATE_BOUND: /* intended fall through */
      case TCPIP_SOCKET_STATE_LISTEN: /* intended fall through */
      case TCPIP_SOCKET_STATE_ACTIVE:
        if(socketPtr->generic.aligned.local.port == port)
        {
          portAlreadyUsed = TRUE;
        }
        break;
      case TCPIP_SOCKET_STATE_UNUSED: /* intended fall through */
      case TCPIP_SOCKET_STATE_USED: /* intended fall through */
        break;
      /* CHECK: NOPARSE */
      default:
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break;
      /* CHECK: PARSE */
    }
  }
  return portAlreadyUsed;
}

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Socket_Tcp_CloseTcp
(
  TcpIp_SocketIdType SocketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr,
  boolean Abort
)
{
  Std_ReturnType retVal = E_NOT_OK;
  uint8 localState = socketPtr->generic.aligned.state;
  boolean localAbort = Abort;

  switch(localState)
  {
    case TCPIP_SOCKET_STATE_USED: /* intended fall through */
    case TCPIP_SOCKET_STATE_BOUND: /* intended fall through */
    case TCPIP_SOCKET_STATE_LISTEN: /* intended fall through */
    case TCPIP_SOCKET_STATE_ACTIVE:
    {
      if(localState != TCPIP_SOCKET_STATE_ACTIVE)
      {
        localAbort = TRUE;
      }

      if(localAbort)
      {
        socketPtr->generic.aligned.state = TCPIP_SOCKET_STATE_UNUSED;

        /* TcpIp.Design.Socket.EA0.Close,1 */
        SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

        /* abort connection by sending reset */
        TcpIp_Tcp_abort(SocketId,socketPtr);
        retVal = E_OK;

        /* !LINKSTO TcpIp.Design.Socket.EA0.Close,1 */
        SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();
      }
      else
      {
        /* close connection through 3 way handsake */
        retVal = TcpIp_Tcp_close(socketPtr,TRUE);
      }

      break;
    }
    /* CHECK: NOPARSE */
    case TCPIP_SOCKET_STATE_UNUSED: /* intended fall through */
    default:
    {
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    }
    /* CHECK: PARSE */
  }

  return retVal;
}

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Socket_Tcp_isAlreadyListen
(
  TcpIp_SocketIdType SocketId
)
{
  Std_ReturnType result = E_NOT_OK;
  TcpIp_SocketIdType nextSocketIndex;
  const TcpIp_SocketIdType socketIdx = TCPIP_SOCKET_GETINDEX(SocketId);
#if (TCPIP_IP_DUALSTACK == STD_ON)
  const uint8 SocketPage = TCPIP_SOCKET_GETPAGE(SocketId);
#endif /* (TCPIP_IP_DUALSTACK == STD_ON) */
  TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
      TcpIp_Socket_getProtocolDomainConstDescriptorPtr(TCP, SocketPage);
  P2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr;

  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  socketPtr = TcpIp_Socket_getIntSocketPtr(TCP, protocolDomainConstDescPtr, socketIdx);

  /* if socket is bound to TCPIP_LOCALADDRID_ANY search anySocket list for matching listen sockets */
  if(socketPtr->generic.aligned.local.addrId == TCPIP_LOCALADDRID_ANY)
  {
    nextSocketIndex =
        TcpIp_Socket_getProtocolDomainDescriptorPtr(TCP, SocketPage)->anySockets;
  }
  else
  {
    /* search Socket list for matching listen sockets */
    nextSocketIndex =
        protocolDomainConstDescPtr->localAddrListenSockets
          [socketPtr->generic.aligned.local.addrId];
  }

  if(nextSocketIndex == TCPIP_SOCKETID_INVALID)
  {
    result = E_OK;
  }
  else
  {
    /* search the active socket list and check if socket is already in state listen */
    while(nextSocketIndex != TCPIP_SOCKETID_INVALID)
    {
      CONSTP2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) currentSocketsocketPtr =
      TcpIp_Socket_getIntSocketPtr(TCP, protocolDomainConstDescPtr, nextSocketIndex);

      if
      (
        (TCPIP_SOCKET_STATE_LISTEN == currentSocketsocketPtr->generic.aligned.state) &&
        (socketPtr->generic.aligned.local.port == currentSocketsocketPtr->generic.aligned.local.port) &&
        (socketPtr->generic.aligned.local.addrId == currentSocketsocketPtr->generic.aligned.local.addrId)
      )
      {
        result = E_NOT_OK;
        break;
      }
      else
      {
        result = E_OK;
      }

      nextSocketIndex = currentSocketsocketPtr->generic.aligned.next;
    }
  }

  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  return result;
}

#endif /* (TCPIP_DEV_ERROR_DETECT == STD_ON) */

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Socket_Tcp_getConnectionProperties
(
  TcpIp_SocketIdType SocketId,
  P2VAR(TcpIp_Socket_IsConnectionReadyConnectionPropertiesType, AUTOMATIC, TCPIP_APPL_DATA) conPropertiesPtr
)
{
  TcpIp_ReturnType retVal = TCPIP_E_NOT_OK;
  TcpIp_Socket_TCP_Generic_Type const *socketPtr;

#if (TCPIP_POSTCONDITION_ASSERT_ENABLED == STD_ON)
    TcpIp_IpAddrM_AddrInfoType addrInfo = TCPIP_IPADDRM_ADDRTYPE_UNICAST;
#endif /* (TCPIP_POSTCONDITION_ASSERT_ENABLED == STD_ON) */

  /* !LINKSTO TcpIp.Design.Socket.EA0.IsConnectionReady.Tcp,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  /* Deviation MISRAC2012-8 */
  socketPtr = (TcpIp_Socket_TCP_Generic_Type const *) TcpIp_Socket_getSocketPtr(SocketId);

  conPropertiesPtr->localAddrId = socketPtr->generic.aligned.local.addrId;
  conPropertiesPtr->localPort = socketPtr->generic.aligned.local.port;
  conPropertiesPtr->protocol = socketPtr->generic.aligned.ipParameters.protocol;

  /* Socket has to be active and local address has to be known */
  if(socketPtr->generic.aligned.state == TCPIP_SOCKET_STATE_ACTIVE)
  {
#if (TCPIP_IP_DUALSTACK == STD_ON)
    const uint8 SocketPage = TCPIP_SOCKET_GETPAGE(SocketId);
#endif /* (TCPIP_IP_DUALSTACK == STD_ON) */
    TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr =
            TcpIp_Socket_getProtocolDomainConstDescriptorPtr(TCP, SocketPage);

    TcpIp_Socket_setRemoteAddr_TCP
    (
      protocolDomainConstDescPtr,
      conPropertiesPtr->remoteAddrStoragePtr,
      &(socketPtr->remote)
    );
    conPropertiesPtr->remoteAddrPtr = conPropertiesPtr->remoteAddrStoragePtr;

    retVal = TCPIP_OK;

#if (TCPIP_POSTCONDITION_ASSERT_ENABLED == STD_ON)
    addrInfo = TcpIp_IpAddrM_getLocalAddrInfo(conPropertiesPtr->localAddrId);
#endif /* (TCPIP_POSTCONDITION_ASSERT_ENABLED == STD_ON) */
  }

  /* !LINKSTO TcpIp.Design.Socket.EA0.IsConnectionReady.Tcp,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

#if (TCPIP_POSTCONDITION_ASSERT_ENABLED == STD_ON)
  /* Socket has to be unicast */
  TCPIP_POSTCONDITION_ASSERT
  (
    TCPIP_IPADDRM_IS_ADDRTYPE(addrInfo, UNICAST),
    TCPIP_SID_ISCONNECTIONREADY
  );
#endif /* (TCPIP_POSTCONDITION_ASSERT_ENABLED == STD_ON) */

#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  if(retVal != TCPIP_OK)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_ISCONNECTIONREADY, TCPIP_E_NOTCONN);
  }
#endif /* (TCPIP_DEV_ERROR_DETECT == STD_ON) */

  return retVal;
}

/*==================[ internal function definitions]=========================*/


STATIC FUNC_P2VAR(TcpIp_Socket_TCP_Generic_Type, TCPIP_APPL_DATA, TCPIP_CODE)
 TcpIp_Socket_Tcp_intGetSocket
(
#if (TCPIP_IP_DUALSTACK == STD_ON)
  uint8 SocketPage,
#endif /* (TCPIP_IP_DUALSTACK == STD_ON) */
  P2VAR(TcpIp_SocketIdType, AUTOMATIC, TCPIP_APPL_DATA) socketIdxPtr
)
{
  P2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr = NULL_PTR;
  TcpIp_Socket_ProtocolDomainDescriptorConstPtrType protocolDomainDescPtr=
    TcpIp_Socket_getProtocolDomainDescriptorPtr(TCP, SocketPage);
  TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
      TcpIp_Socket_getProtocolDomainConstDescriptorPtr(TCP, SocketPage);

  /* check if there is a socket available in the free-socket-list */
  if(protocolDomainDescPtr->freeSockets != TCPIP_SOCKETID_INVALID)
  {
    /* get socket from free socket list */
    const TcpIp_SocketIdType socketIdx = protocolDomainDescPtr->freeSockets;
    /* Deviation MISRAC2012-8 <+2> */
    TcpIp_Socket_removeNextSocket(TCP, protocolDomainConstDescPtr,
                                  &protocolDomainDescPtr->freeSockets );

    /* get pointer to socket */
    socketPtr = TcpIp_Socket_getIntSocketPtr(TCP, protocolDomainConstDescPtr, socketIdx);

    /* socket must be in state UNUSED */
    TCPIP_PRECONDITION_ASSERT
      (socketPtr->generic.aligned.state == TCPIP_SOCKET_STATE_UNUSED,TCPIP_INTERNAL_API_ID);

    /* set state to used */
    socketPtr->generic.aligned.state = TCPIP_SOCKET_STATE_USED;

    /* write output parameter */
    *socketIdxPtr = socketIdx;
  }

  return socketPtr;
}

STATIC FUNC_P2VAR(TcpIp_SocketIdType, TCPIP_APPL_DATA, TCPIP_CODE) TcpIp_Socket_Tcp_searchSocket
(
  TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr,
  P2VAR(TcpIp_SocketIdType,AUTOMATIC,TCPIP_APPL_DATA) nextEntryPtr,
  TcpIp_SocketIdType socketIdx
)
{
  P2VAR(TcpIp_SocketIdType,AUTOMATIC,TCPIP_APPL_DATA) resultEntryPtr = NULL_PTR;

  if(*nextEntryPtr == socketIdx)
  {
    resultEntryPtr = nextEntryPtr;
  }
  else
  {
    P2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) prevSocketPtr =
        TcpIp_Socket_getIntSocketPtr(TCP, protocolDomainConstDescPtr, *nextEntryPtr);

    /* iterate through linked list to find the socket in the bound-socket-list */
    while((prevSocketPtr->generic.aligned.next != socketIdx) &&
          (prevSocketPtr->generic.aligned.next != TCPIP_SOCKETID_INVALID)
         )
    {
      prevSocketPtr =
        /* Deviation MISRAC2012-8 */
        TcpIp_Socket_getIntSocketPtr
          (TCP, protocolDomainConstDescPtr, prevSocketPtr->generic.aligned.next);
    }

    /* currently it is not possible that a socket is not found in the list */
    TCPIP_PRECONDITION_ASSERT
      ((prevSocketPtr->generic.aligned.next != TCPIP_SOCKETID_INVALID),TCPIP_INTERNAL_API_ID);

    resultEntryPtr = &prevSocketPtr->generic.aligned.next;
  }

  return resultEntryPtr;
}

STATIC FUNC_P2VAR(TcpIp_Socket_TCP_Generic_Type, TCPIP_APPL_DATA, TCPIP_CODE)
 TcpIp_Socket_Tcp_searchAndRemoveSocket
(
  TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr,
  P2VAR(TcpIp_SocketIdType,AUTOMATIC,TCPIP_APPL_DATA) nextEntryPtr,
  TcpIp_SocketIdType socketIdx
)
{
  /* remove from local address socket list */
  CONSTP2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr =
      TcpIp_Socket_getIntSocketPtr(TCP, protocolDomainConstDescPtr, socketIdx);

  /* Deviation MISRAC2012-8 <+5> */
  TcpIp_Socket_removeNextSocket(TCP, protocolDomainConstDescPtr,
                                TcpIp_Socket_Tcp_searchSocket(
                                    protocolDomainConstDescPtr,
                                    nextEntryPtr,
                                    socketIdx )
                               );

  return socketPtr;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Socket_Tcp_removeListenSocketReference
(
  TcpIp_SocketIdType socketId
)
{
  uint16_least socketIdx = 0U;
#if (TCPIP_IP_DUALSTACK == STD_ON)
  const uint8 SocketPage = TCPIP_SOCKET_GETPAGE(socketId);
#endif /* (TCPIP_IP_DUALSTACK == STD_ON) */
  TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
      TcpIp_Socket_getProtocolDomainConstDescriptorPtr(TCP, SocketPage);

#if((TCPIP_SOCKET_NUM_TCP_IPV4 > 1U) || (TCPIP_SOCKET_NUM_TCP_IPV6 > 1U))
  for(socketIdx = 0U; socketIdx < protocolDomainConstDescPtr->numSockets; socketIdx++)
#endif /* ((TCPIP_SOCKET_NUM_TCP_IPV4 > 1U) || (TCPIP_SOCKET_NUM_TCP_IPV6 > 1U)) */
  {
    CONSTP2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr =
      TcpIp_Socket_getIntSocketPtr(TCP, protocolDomainConstDescPtr, socketIdx);

    if(socketPtr->generic.aligned.state == TCPIP_SOCKET_STATE_ACTIVE)
    {
      if(socketPtr->listenSocketId == socketId)
      {
        socketPtr->listenSocketId = TCPIP_SOCKETID_INVALID;
      }
    }
  }
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Socket_Tcp_lookupSingleList
(
  uint8 SocketPage,
  P2VAR(TcpIp_SocketIdType,AUTOMATIC,TCPIP_APPL_DATA) socketIdPtr,
  P2CONST(TcpIp_SocketIdType,AUTOMATIC,TCPIP_APPL_DATA) nextEntryPtr,
  P2VAR(TcpIp_Socket_TCP_Generic_PtrType, AUTOMATIC, TCPIP_APPL_DATA) socketPtrPtr,
  P2CONST(TcpIp_SockAddrType,AUTOMATIC,TCPIP_APPL_DATA) remoteAddrPtr,
  uint16 local_port
)
{
  Std_ReturnType result = TCPIP_E_NOT_OK;
  TcpIp_SocketIdType nextSocketIndex = *nextEntryPtr;

  while((nextSocketIndex != TCPIP_SOCKETID_INVALID) && (result != TCPIP_OK))
  {
    const TcpIp_SocketIdType currentSocketIdx = nextSocketIndex;
    TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
        TcpIp_Socket_getProtocolDomainConstDescriptorPtr(TCP, SocketPage);

    CONSTP2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr =
      TcpIp_Socket_getIntSocketPtr(TCP, protocolDomainConstDescPtr, currentSocketIdx);

    if(socketPtr->generic.aligned.local.port == local_port)
    {
      if(remoteAddrPtr == NULL_PTR)
      {
        /* found matching socket (without remote address) */
        *socketIdPtr = TCPIP_SOCKET_CREATEID(SocketPage, currentSocketIdx);
        *socketPtrPtr = socketPtr;
        result = TCPIP_OK;
      }
      else if(TcpIp_Socket_socketMatch_TCP(protocolDomainConstDescPtr, &socketPtr->remote, remoteAddrPtr))
      {
        /* found matching socket - including remote address */
        *socketIdPtr = TCPIP_SOCKET_CREATEID(SocketPage, currentSocketIdx);
        *socketPtrPtr = socketPtr;
        result = TCPIP_OK;
      }
      else
      {
        /* no match */
      }
    }
    nextSocketIndex = socketPtr->generic.aligned.next;
  }

  return result;
}


STATIC FUNC_P2VAR(TcpIp_SocketIdType, TCPIP_APPL_DATA, TCPIP_CODE)
 TcpIp_Socket_Tcp_autoGetListenSocketListEntryPtr
(
#if (TCPIP_IP_DUALSTACK == STD_ON)
  uint8 SocketPage,
#endif /* (TCPIP_IP_DUALSTACK == STD_ON) */
  TcpIp_LocalAddrIdType localAddrId
)
{
  P2VAR(TcpIp_SocketIdType,AUTOMATIC,TCPIP_APPL_DATA) socketPtr;

  if(localAddrId == TCPIP_LOCALADDRID_ANY)
  {
    socketPtr = &TcpIp_Socket_getProtocolDomainDescriptorPtr(TCP, SocketPage)->anySockets;
  }
  else
  {
    socketPtr =
      &TcpIp_Socket_getProtocolDomainConstDescriptorPtr(TCP, SocketPage)->localAddrListenSockets[localAddrId];
  }

  return socketPtr;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Socket_Tcp_initDomain
(
#if (TCPIP_IP_DUALSTACK == STD_ON)
  uint8 SocketPage
#else /* TCPIP_IP_DUALSTACK == STD_OFF */
  void
#endif /* TCPIP_IP_DUALSTACK */
)
{
  uint8_least i;

  for(i = 0U; i < TCPIP_NUM_LOCALADDRENTRIES; i++)
  {
    TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
        TcpIp_Socket_getProtocolDomainConstDescriptorPtr(TCP, SocketPage);

    protocolDomainConstDescPtr->localAddrActiveSockets
                         [(TcpIp_LocalAddrIdType) i]= TCPIP_SOCKETID_INVALID;
    protocolDomainConstDescPtr->localAddrListenSockets
                         [(TcpIp_LocalAddrIdType) i]= TCPIP_SOCKETID_INVALID;
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Socket_Tcp_processTcpSocketsDomain
(
  uint8 SocketPage
)
{
  TcpIp_SocketIdType socketIdx = 0U;
  TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
      TcpIp_Socket_getProtocolDomainConstDescriptorPtr(TCP, SocketPage);

#if((TCPIP_SOCKET_NUM_TCP_IPV4 > 1U) || (TCPIP_SOCKET_NUM_TCP_IPV6 > 1U))
  for(socketIdx = 0U; socketIdx < protocolDomainConstDescPtr->numSockets; socketIdx++)
#endif /* ((TCPIP_SOCKET_NUM_TCP_IPV4 > 1U) || (TCPIP_SOCKET_NUM_TCP_IPV6 > 1U)) */
  {
    CONSTP2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr =
      TcpIp_Socket_getIntSocketPtr(TCP, protocolDomainConstDescPtr, socketIdx);

    if(socketPtr->generic.aligned.state == TCPIP_SOCKET_STATE_ACTIVE)
    {
      TcpIp_Tcp_processTcpSocket
        (TCPIP_SOCKET_CREATEID(SocketPage, socketIdx), socketPtr);
    }
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Socket_Tcp_processTcpSocketsDomainTx
(
  uint8 SocketPage
)
{
  TcpIp_SocketIdType socketIdx = 0U;
  TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
      TcpIp_Socket_getProtocolDomainConstDescriptorPtr(TCP, SocketPage);

#if((TCPIP_SOCKET_NUM_TCP_IPV4 > 1U) || (TCPIP_SOCKET_NUM_TCP_IPV6 > 1U))
  for(socketIdx = 0U; socketIdx < protocolDomainConstDescPtr->numSockets; socketIdx++)
#endif /* ((TCPIP_SOCKET_NUM_TCP_IPV4 > 1U) || (TCPIP_SOCKET_NUM_TCP_IPV6 > 1U)) */
  {
    CONSTP2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr =
      /* Deviation MISRAC2012-8 */
      TcpIp_Socket_getIntSocketPtr(TCP, protocolDomainConstDescPtr, socketIdx);

    if(socketPtr->generic.aligned.state == TCPIP_SOCKET_STATE_ACTIVE)
    {
      TcpIp_Tcp_processTcpSocketTx
        (TCPIP_SOCKET_CREATEID(SocketPage, socketIdx), socketPtr);
    }
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Socket_Tcp_terminateSocketsDomain
(
  uint8 SocketPage,
  TcpIp_LocalAddrIdType localAddrId,
  TcpIp_Socket_abort_FctPtrType abortFctPtr
)
{
  TcpIp_SocketIdType validListenSocket = TCPIP_SOCKETID_INVALID;
  TcpIp_SocketIdType validActiveSocket = TCPIP_SOCKETID_INVALID;
  TcpIp_SocketIdType firstListenSocket;
  TcpIp_SocketIdType firstActiveSocket;
  TcpIp_SocketIdType nextListenSocket;
  TcpIp_SocketIdType nextActiveSocket;
  TcpIp_Socket_ProtocolDomainDescriptorConstPtrType protocolDomainDescPtr=
      TcpIp_Socket_getProtocolDomainDescriptorPtr(TCP, SocketPage);
  TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
      TcpIp_Socket_getProtocolDomainConstDescriptorPtr(TCP, SocketPage);

  /* !LINKSTO TcpIp.Design.Socket.EA0.terminateSockets,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  if(localAddrId == TCPIP_LOCALADDRID_ANY)
  {
    firstListenSocket = protocolDomainDescPtr->anySockets;
    protocolDomainDescPtr->anySockets = TCPIP_SOCKETID_INVALID;

    firstActiveSocket = TCPIP_SOCKETID_INVALID;
  }
  else
  {
    firstActiveSocket =
      protocolDomainConstDescPtr->localAddrActiveSockets[localAddrId];
    protocolDomainConstDescPtr->localAddrActiveSockets[localAddrId] =
      TCPIP_SOCKETID_INVALID;

    firstListenSocket =
        protocolDomainConstDescPtr->localAddrListenSockets[localAddrId];
    protocolDomainConstDescPtr->localAddrListenSockets[localAddrId] =
      TCPIP_SOCKETID_INVALID;
  }

  /* !LINKSTO TcpIp.Design.Socket.EA0.terminateSockets,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  nextActiveSocket = firstActiveSocket;
  nextListenSocket = firstListenSocket;

  while(nextActiveSocket != TCPIP_SOCKETID_INVALID)
  {
    P2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr =
      TcpIp_Socket_getIntSocketPtr(TCP, protocolDomainConstDescPtr, nextActiveSocket);

    /* socket must be in state ACTIVE */
    TCPIP_PRECONDITION_ASSERT
      (socketPtr->generic.aligned.state == TCPIP_SOCKET_STATE_ACTIVE,TCPIP_INTERNAL_API_ID);

    /* update state */
    socketPtr->generic.aligned.state = TCPIP_SOCKET_STATE_UNUSED;

    /* tell Tcp that socket is enforced to be closed */
    (void)abortFctPtr
          (
              TCPIP_SOCKET_CREATEID(SocketPage, nextActiveSocket),
              socketPtr
          );

    /* !LINKSTO TcpIp.Design.Socket.EA0.terminateSockets,1 */
    SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

    if(socketPtr->listenSocketId != TCPIP_SOCKETID_INVALID)
    {
      CONSTP2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) listenSocketPtr =
        /* Deviation MISRAC2012-8 */
        TcpIp_Socket_getIntSocketPtr
          (TCP, protocolDomainConstDescPtr, TCPIP_SOCKET_GETINDEX(socketPtr->listenSocketId));

      TCPIP_PRECONDITION_ASSERT
        ((listenSocketPtr->numConnections >= 1U), TCPIP_INTERNAL_API_ID);

      listenSocketPtr->numConnections--;
    }

    /* !LINKSTO TcpIp.Design.Socket.EA0.terminateSockets,1 */
    SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

    validActiveSocket = nextActiveSocket;
    nextActiveSocket =
      /* Deviation MISRAC2012-8 */
      TcpIp_Socket_getIntSocketPtr
        (TCP, protocolDomainConstDescPtr, nextActiveSocket)->generic.aligned.next;
  }

  while(nextListenSocket != TCPIP_SOCKETID_INVALID)
  {
    P2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr =
        TcpIp_Socket_getIntSocketPtr(TCP, protocolDomainConstDescPtr, nextListenSocket);

    /* socket must be in state LISTEN */
    TCPIP_PRECONDITION_ASSERT
      (socketPtr->generic.aligned.state == TCPIP_SOCKET_STATE_LISTEN,TCPIP_INTERNAL_API_ID);

    /* update state */
    socketPtr->generic.aligned.state = TCPIP_SOCKET_STATE_UNUSED;

    /* tell Tcp that socket is enforced to be closed */
    (void)abortFctPtr
          (
              TCPIP_SOCKET_CREATEID(SocketPage, nextListenSocket),
              socketPtr
          );

    validListenSocket = nextListenSocket;
    nextListenSocket =
      /* Deviation MISRAC2012-8 */
      TcpIp_Socket_getIntSocketPtr
        (TCP, protocolDomainConstDescPtr, nextListenSocket)->generic.aligned.next;
  }

  /* !LINKSTO TcpIp.Design.Socket.EA0.terminateSockets,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  if(firstActiveSocket != TCPIP_SOCKETID_INVALID)
  {
    /* insert into free socket list */
    /* Deviation MISRAC2012-8 <+4> */
    TcpIp_Socket_insertSocketList(TCP, protocolDomainConstDescPtr,
        &protocolDomainDescPtr->freeSockets,
        firstActiveSocket,
        validActiveSocket );
  }

  if(firstListenSocket != TCPIP_SOCKETID_INVALID)
  {
    /* insert into free socket list */
    /* Deviation MISRAC2012-8 <+4> */
    TcpIp_Socket_insertSocketList(TCP, protocolDomainConstDescPtr,
        &protocolDomainDescPtr->freeSockets,
        firstListenSocket,
        validListenSocket );
  }

  /* !LINKSTO TcpIp.Design.Socket.EA0.terminateSockets,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();
}

STATIC FUNC(boolean, TCPIP_CODE) TcpIp_Socket_Tcp_closeSocketsDomain
(
  uint8 SocketPage,
  TcpIp_LocalAddrIdType localAddrId
)
{
  boolean retVal = FALSE;
  TcpIp_SocketIdType validListenSocket = TCPIP_SOCKETID_INVALID;
  TcpIp_SocketIdType firstActiveSocket;
  TcpIp_SocketIdType firstListenSocket;
  TcpIp_SocketIdType nextActiveSocket;
  TcpIp_SocketIdType nextListenSocket;
  TcpIp_Socket_ProtocolDomainDescriptorConstPtrType protocolDomainDescPtr=
      TcpIp_Socket_getProtocolDomainDescriptorPtr(TCP, SocketPage);
  TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
      TcpIp_Socket_getProtocolDomainConstDescriptorPtr(TCP, SocketPage);

  /* !LINKSTO TcpIp.Design.Socket.EA0.closeSockets,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  firstActiveSocket =
      protocolDomainConstDescPtr->localAddrActiveSockets[localAddrId];

  firstListenSocket =
      protocolDomainConstDescPtr->localAddrListenSockets[localAddrId];
  protocolDomainConstDescPtr->localAddrListenSockets[localAddrId] =
      TCPIP_SOCKETID_INVALID;

  nextActiveSocket = firstActiveSocket;
  nextListenSocket = firstListenSocket;

  while(nextActiveSocket != TCPIP_SOCKETID_INVALID)
  {
    CONSTP2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr =
      TcpIp_Socket_getIntSocketPtr(TCP, protocolDomainConstDescPtr, nextActiveSocket);

    /* socket must be in state ACTIVE */
    TCPIP_PRECONDITION_ASSERT
      (socketPtr->generic.aligned.state == TCPIP_SOCKET_STATE_ACTIVE,TCPIP_INTERNAL_API_ID);

    /* tell Tcp that connection shall be closed */
    (void)TcpIp_Tcp_close(socketPtr, FALSE);

    nextActiveSocket =
      /* Deviation MISRAC2012-8 */
      TcpIp_Socket_getIntSocketPtr
        (TCP, protocolDomainConstDescPtr, nextActiveSocket)->generic.aligned.next;
  }

  /* !LINKSTO TcpIp.Design.Socket.EA0.closeSockets,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  while(nextListenSocket != TCPIP_SOCKETID_INVALID)
  {
    P2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr =
        TcpIp_Socket_getIntSocketPtr(TCP, protocolDomainConstDescPtr, nextListenSocket);

    /* socket must be in state LISTEN */
    TCPIP_PRECONDITION_ASSERT
      (socketPtr->generic.aligned.state == TCPIP_SOCKET_STATE_LISTEN,TCPIP_INTERNAL_API_ID);

    socketPtr->generic.aligned.state = TCPIP_SOCKET_STATE_UNUSED;

    /* tell Tcp that socket shall be closed */
    (void)TcpIp_Tcp_terminate
          (
              TCPIP_SOCKET_CREATEID(SocketPage, nextListenSocket),
              socketPtr
          );


    validListenSocket = nextListenSocket;
    nextListenSocket =
      /* Deviation MISRAC2012-8 */
      TcpIp_Socket_getIntSocketPtr
        (TCP, protocolDomainConstDescPtr, nextListenSocket)->generic.aligned.next;
  }


  if(firstListenSocket != TCPIP_SOCKETID_INVALID)
  {
    /* !LINKSTO TcpIp.Design.Socket.EA0.closeSockets,1 */
    SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

    /* insert into free socket list */
    /* Deviation MISRAC2012-8 <+4> */
    TcpIp_Socket_insertSocketList(TCP, protocolDomainConstDescPtr,
        &protocolDomainDescPtr->freeSockets,
        firstListenSocket,
        validListenSocket );

    /* !LINKSTO TcpIp.Design.Socket.EA0.closeSockets,1 */
    SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();
  }


  if(firstActiveSocket != TCPIP_SOCKETID_INVALID)
  {
    retVal = TRUE;
  }

  return retVal;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Socket_Tcp_cloneSocket
(
  P2VAR(TcpIp_Socket_TCP_Generic_PtrType, AUTOMATIC, TCPIP_APPL_DATA) newSocketPtrPtr,
  TcpIp_SocketIdType newSocketIdx,
  P2CONST(TcpIp_SockAddrLocalType,AUTOMATIC,TCPIP_APPL_DATA) localAddrPtr,
  P2CONST(TcpIp_SockAddrType,AUTOMATIC,TCPIP_APPL_DATA) remoteAddrPtr,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) listenSocketPtr,
  TcpIp_SocketIdType listenSocketId
)
{
  const TcpIp_LocalAddrIdType localAddrId = localAddrPtr->addrId;
  const uint16 port = localAddrPtr->port;
#if (TCPIP_IP_DUALSTACK == STD_ON)
  const uint8 SocketPage = TCPIP_SOCKET_GETPAGE(listenSocketId);
#endif /* (TCPIP_IP_DUALSTACK == STD_ON) */
  TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
      TcpIp_Socket_getProtocolDomainConstDescriptorPtr(TCP, SocketPage);

  /* link new socket to listen socket and increment reference counter */
  listenSocketPtr->numConnections++;

  /* save local address the segment has been received */
  (*newSocketPtrPtr)->generic.aligned.local.addrId = localAddrId;
  (*newSocketPtrPtr)->generic.aligned.local.port = port;
  (*newSocketPtrPtr)->listenSocketId = listenSocketId;
  (*newSocketPtrPtr)->numEstablishedConnections = 0U;

  /* clone IP parameters */
  (*newSocketPtrPtr)->generic.aligned.ipParameters = listenSocketPtr->generic.aligned.ipParameters;

  /* clone tcp data structure */
  (*newSocketPtrPtr)->tcp = listenSocketPtr->tcp;

  /* clone socket owner */
  (*newSocketPtrPtr)->generic.aligned.sockOwnerId = listenSocketPtr->generic.aligned.sockOwnerId;;

  /* save remote address the segment has been transmitted from */
  TcpIp_Socket_setRemoteAddr_TCP
    (protocolDomainConstDescPtr, &(*newSocketPtrPtr)->remote, remoteAddrPtr);

  /* set new state */
  (*newSocketPtrPtr)->generic.aligned.state = TCPIP_SOCKET_STATE_ACTIVE;

  /* insert new socket into active list and set state properly */
  /* Deviation MISRAC2012-8 <+4> */
  TcpIp_Socket_insertSocket(TCP, protocolDomainConstDescPtr,
      &protocolDomainConstDescPtr->localAddrActiveSockets[localAddrId],
      newSocketIdx  );
}

#if (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON)

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Socket_Tcp_getSocketNotEstablished
(
  uint8 SocketPage,
  P2VAR(TcpIp_Socket_TCP_Generic_PtrType, AUTOMATIC, TCPIP_APPL_DATA) socketPtrPtr,
  P2VAR(TcpIp_SocketIdType,AUTOMATIC,TCPIP_APPL_DATA) socketIdPtr,
  P2CONST(TcpIp_SockAddrType,AUTOMATIC,TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(TcpIp_SockAddrLocalType,AUTOMATIC,TCPIP_APPL_DATA) localAddrPtr
)
{
  TcpIp_ReturnType result = TCPIP_E_NOT_OK;
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) listenSocketPtr = *socketPtrPtr;
  TcpIp_SocketIdType listenSocketId = *socketIdPtr;
  TcpIp_SocketIdType oldestSocketId = TCPIP_SOCKETID_INVALID;
  TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
     TcpIp_Socket_getProtocolDomainConstDescriptorPtr(TCP, SocketPage);
  TcpIp_LocalAddrIdType rceiveLocalAddrId = localAddrPtr->addrId;
  TcpIp_LocalAddrIdType listenSocketLocalAddrId = listenSocketPtr->generic.aligned.local.addrId;
  TcpIp_LocalAddrIdType anyLocalAddrId = TcpIp_IpAddrM_getAnyIpLocalAddrId(rceiveLocalAddrId);

  if((listenSocketLocalAddrId == TCPIP_LOCALADDRID_ANY) || (listenSocketLocalAddrId == anyLocalAddrId))
  {
    uint8 i;

    /* local address is any, loop over all unicast local addresses */
    for(i = 0U; (i < TCPIP_NUM_LOCALADDRENTRIES) && (oldestSocketId == TCPIP_SOCKETID_INVALID); i++)
    {
      const TcpIp_IpAddrM_AddrInfoType addrInfo = TcpIp_IpAddrM_getLocalAddrInfo(i);

      /* only check for ids which match domain and are unicast */
      if((TCPIP_IPADDRM_IS_ADDRTYPE(addrInfo,UNICAST)) &&
         (TCPIP_SOCKET_PAGE_DOMAIN_GET(SocketPage) == TCPIP_IPADDRM_ADDRDOMAIN_GET(addrInfo))
        )
      {
        oldestSocketId =
          TcpIp_Socket_Tcp_getSocketNotEstablishedList(listenSocketId, i, protocolDomainConstDescPtr);
      }
    }
  }
  else
  {
    /* search in the active socket list if some socket is not established yet */
    oldestSocketId =
      TcpIp_Socket_Tcp_getSocketNotEstablishedList(listenSocketId, rceiveLocalAddrId, protocolDomainConstDescPtr);
  }

  /* this function is only called when there is at least one socket which is not established */
  TCPIP_PRECONDITION_ASSERT((oldestSocketId != TCPIP_SOCKETID_INVALID), TCPIP_INTERNAL_API_ID);

  {
    CONSTP2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr =
          TcpIp_Socket_getIntSocketPtr(TCP, protocolDomainConstDescPtr, oldestSocketId);

    /* save local address the segment has been received */
    socketPtr->generic.aligned.local.addrId = rceiveLocalAddrId;
    socketPtr->generic.aligned.local.port = localAddrPtr->port;

    /* copy IP parameters */
    socketPtr->generic.aligned.ipParameters = listenSocketPtr->generic.aligned.ipParameters;

    /* copy tcp data structure */
    socketPtr->tcp = listenSocketPtr->tcp;

    /* save remote address the segment has been transmitted from */
    TcpIp_Socket_setRemoteAddr_TCP
      (protocolDomainConstDescPtr, &socketPtr->remote, remoteAddrPtr);

    *socketPtrPtr = socketPtr;
    *socketIdPtr = TCPIP_SOCKET_CREATEID(SocketPage, oldestSocketId);
    result = TCPIP_OK;
  }

  return result;
}

STATIC FUNC(TcpIp_SocketIdType, TCPIP_CODE) TcpIp_Socket_Tcp_getSocketNotEstablishedList
(
  TcpIp_SocketIdType listenSocketId,
  TcpIp_LocalAddrIdType localAddr,
  TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr
)
{
  TcpIp_SocketIdType oldestSocketId = TCPIP_SOCKETID_INVALID;

  /* search in the active socket list if some socket is not established yet */
  TcpIp_SocketIdType nextSocketIndex =
    protocolDomainConstDescPtr->localAddrActiveSockets[localAddr];

  /* loop over all active sockets and retrieve the oldest socket which is not established */
  while(nextSocketIndex != TCPIP_SOCKETID_INVALID)
  {
    const TcpIp_SocketIdType currentSocketIdx = nextSocketIndex;

    CONSTP2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr =
       TcpIp_Socket_getIntSocketPtr(TCP, protocolDomainConstDescPtr, currentSocketIdx);

    if((socketPtr->listenSocketId == listenSocketId) &&
       (socketPtr->numEstablishedConnections == 0U)
      )
    {
      oldestSocketId = nextSocketIndex;
    }

    nextSocketIndex = socketPtr->generic.aligned.next;
  }

  return oldestSocketId;
}

#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#else
  TS_PREVENTEMPTYTRANSLATIONUNIT
#endif /* (TCPIP_TCP_ENABLED == STD_ON) */
/*==================[end of file]===========================================*/
