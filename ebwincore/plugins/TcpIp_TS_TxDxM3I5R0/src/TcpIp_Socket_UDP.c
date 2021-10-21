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
#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Udp_Int.h>
#include <TcpIp_Socket_Priv.h>   /* own SW-unit internal header file; must be last */

#if (TCPIP_UDP_ENABLED == STD_ON)
/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief searches a socket socketIdx in list with entry nextEntryPtr, removes it
   from linked list and returns the pointer to the socket */
STATIC FUNC_P2VAR(TcpIp_Socket_UDP_Generic_Type, TCPIP_APPL_DATA, TCPIP_CODE) TcpIp_Socket_Udp_searchAndRemoveSocket
(
#if (TCPIP_IP_DUALSTACK == STD_ON)
  uint8 SocketPage,
#endif /* (TCPIP_IP_DUALSTACK == STD_ON) */
  P2VAR(TcpIp_SocketIdType,AUTOMATIC,TCPIP_APPL_DATA) nextEntryPtr,
  TcpIp_SocketIdType socketIdx
);

/** \brief looks-up sockets with local_port in a list nextEntryPtr, writes
   the results (maximum of maxSocketInstances) to socketIdsPtr and returns
   the number of found sockets */
STATIC FUNC(uint8, TCPIP_CODE) TcpIp_Socket_Udp_lookupSingleList
(
  uint8 SocketPage,
  P2VAR(TcpIp_SocketIdType,AUTOMATIC,TCPIP_APPL_DATA) socketIdsPtr,
  P2CONST(TcpIp_SocketIdType,AUTOMATIC,TCPIP_APPL_DATA) nextEntryPtr,
  uint16 local_port,
  uint8 maxSocketInstances
);

/** \brief returns pointer to a active socket list (localAddrId or ANY) entry based on the localAddrId */
STATIC FUNC_P2VAR(TcpIp_SocketIdType, TCPIP_APPL_DATA, TCPIP_CODE)
 TcpIp_Socket_Udp_autoGetActiveSocketListEntryPtr
(
#if (TCPIP_IP_DUALSTACK == STD_ON)
  uint8 SocketPage,
#endif /* (TCPIP_IP_DUALSTACK == STD_ON) */
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief initializes the UDP sockets to default vales of the specified domain */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Socket_Udp_initDomain
(
#if (TCPIP_IP_DUALSTACK == STD_ON)
  uint8 SocketPage
#else /* TCPIP_IP_DUALSTACK == STD_OFF */
  void
#endif /* TCPIP_IP_DUALSTACK */
);

/** \brief Immediately closes UDP sockets bound to a particular local address identifier of the
           specified domain */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Socket_Udp_terminateSocketsDomain
(
  uint8 SocketPage,
  TcpIp_LocalAddrIdType localAddrId
);

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>


/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/
#define TCPIP_START_SEC_VAR_CLEARED_16
#include <TcpIp_MemMap.h>

#if (TCPIP_SOCKET_NUM_UDP_IPV4 > 0U)
/** \brief entry point into lists of Udp sockets, one list per local address */
VAR(TcpIp_SocketIdType,TCPIP_VAR_CLEARED)
  TcpIp_Socket_localAddrActiveSockets_UDP_IPV4[TCPIP_NUM_LOCALADDRENTRIES];

#endif /* (TCPIP_SOCKET_NUM_UDP_IPV4 > 0U) */

#if (TCPIP_SOCKET_NUM_UDP_IPV6 > 0U)

/** \brief entry point into lists of Udp sockets, one list per local address */
VAR(TcpIp_SocketIdType,TCPIP_VAR_CLEARED)
  TcpIp_Socket_localAddrActiveSockets_UDP_IPV6[TCPIP_NUM_LOCALADDRENTRIES];

#endif /* (TCPIP_SOCKET_NUM_UDP_IPV6 > 0U) */

#define TCPIP_STOP_SEC_VAR_CLEARED_16
#include <TcpIp_MemMap.h>

#define TCPIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

#if (TCPIP_SOCKET_NUM_UDP_IPV4 > 0U)

/** \brief UDP IPV4 socket memory space */
VAR(TcpIp_Socket_UDP_IPV4_Type,TCPIP_VAR_CLEARED)
  TcpIp_Socket_SocketMemory_UDP_IPV4[TCPIP_SOCKET_NUM_UDP_IPV4];

#endif /* (TCPIP_SOCKET_NUM_UDP_IPV4 > 0U) */


#if  (TCPIP_SOCKET_NUM_UDP_IPV6 > 0U)

/** \brief UDP IPV6 socket memory space */
VAR(TcpIp_Socket_UDP_IPV6_Type,TCPIP_VAR_CLEARED)
  TcpIp_Socket_SocketMemory_UDP_IPV6[TCPIP_SOCKET_NUM_UDP_IPV6];

#endif /*  (TCPIP_SOCKET_NUM_UDP_IPV6 > 0U) */

#define TCPIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[module internal function definitions]=======================*/

/*==================[unit internal function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Socket_Udp_init(void)
{
#if (TCPIP_SOCKET_NUM_UDP_IPV4 > 0U)
  TcpIp_Socket_Udp_initDomain
    (TCPIP_IP_IF_DUALSTACK(TcpIp_Socket_SocketPage_UDP_IPV4));
#endif /* (TCPIP_SOCKET_NUM_UDP_IPV4 > 0U) */
#if (TCPIP_SOCKET_NUM_UDP_IPV6 > 0U)
  TcpIp_Socket_Udp_initDomain
    (TCPIP_IP_IF_DUALSTACK(TcpIp_Socket_SocketPage_UDP_IPV6));
#endif /* (TCPIP_SOCKET_NUM_UDP_IPV6 > 0U) */
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Socket_Udp_GetSocket
(
  uint8 SocketPage,
  P2VAR(TcpIp_SocketIdType, AUTOMATIC, TCPIP_APPL_DATA) SocketIdPtr,
  uint8 sockOwnerId
)
{
  Std_ReturnType result = E_NOT_OK;
  TcpIp_Socket_ProtocolDomainDescriptorConstPtrType protocolDomainDescriptorPtr=
    TcpIp_Socket_getProtocolDomainDescriptorPtr(UDP, SocketPage);
  TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
      TcpIp_Socket_getProtocolDomainConstDescriptorPtr(UDP, SocketPage);

  /* !LINKSTO TcpIp.Design.Socket.EA0.GetSocket,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();
  {
    P2VAR(TcpIp_SocketIdType, AUTOMATIC, TCPIP_APPL_DATA) const freeSocketListEntryPtr =
      &protocolDomainDescriptorPtr->freeSockets;
    /* get socket from free socket list */
    TcpIp_SocketIdType const socketIdx = *freeSocketListEntryPtr;

    /* check if there is a socket available in the free-socket-list */
    if(socketIdx != TCPIP_SOCKETID_INVALID)
    {
      /* get pointer to socket */
      CONSTP2VAR(TcpIp_Socket_UDP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr =
        TcpIp_Socket_getIntSocketPtr(UDP, protocolDomainConstDescPtr, socketIdx);
      /* remove the socket */
      TcpIp_Socket_removeNextSocket(UDP, protocolDomainConstDescPtr, freeSocketListEntryPtr );

      /* socket must be in state UNUSED */
      TCPIP_PRECONDITION_ASSERT
        (socketPtr->generic.aligned.state == TCPIP_SOCKET_STATE_UNUSED,TCPIP_INTERNAL_API_ID);

      /* set state to used */
      socketPtr->generic.aligned.state = TCPIP_SOCKET_STATE_USED;
      socketPtr->generic.aligned.sockOwnerId = sockOwnerId;

      /* initialize communication parameters for Udp */
      TcpIp_Udp_changeSocketToDefault(socketPtr);

      /* write output parameter */
      *SocketIdPtr = TCPIP_SOCKET_CREATEID(SocketPage, socketIdx);

      result = E_OK;
    }
  }
  /* !LINKSTO TcpIp.Design.Socket.EA0.GetSocket,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  return result;
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Socket_Udp_Bind
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
      TcpIp_Socket_getProtocolDomainConstDescriptorPtr(UDP, SocketPage);
  CONSTP2VAR(TcpIp_Socket_UDP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr =
    TcpIp_Socket_getIntSocketPtr(UDP, protocolDomainConstDescPtr, socketIdx);

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

      /* set local address and port */
      socketPtr->generic.aligned.local.port = port;
      socketPtr->generic.aligned.local.addrId = LocalAddrId;

      /* insert into localAddrId bound-socket-list at first position */
      /* Deviation MISRAC2012-8 <+5> */
      TcpIp_Socket_insertSocket(UDP, protocolDomainConstDescPtr,
          TcpIp_Socket_Udp_autoGetActiveSocketListEntryPtr
            ( TCPIP_IP_DUALSTACK_PARAM(SocketPage)
              LocalAddrId ),
          socketIdx  );

      socketPtr->generic.aligned.state = TCPIP_SOCKET_STATE_BOUND;

      retval = E_OK;
      break;
    }
    case TCPIP_SOCKET_STATE_BOUND:
    {
      break;
    }
    /* CHECK: NOPARSE */
    case TCPIP_SOCKET_STATE_UNUSED: /* intended fall through */
    case TCPIP_SOCKET_STATE_ACTIVE: /* intended fall through */
    case TCPIP_SOCKET_STATE_LISTEN: /* intended fall through */
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

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Socket_Udp_Close
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
  TcpIp_Socket_ProtocolDomainDescriptorConstPtrType protocolDomainDescriptorPtr=
    TcpIp_Socket_getProtocolDomainDescriptorPtr(UDP, SocketPage);
  TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
      TcpIp_Socket_getProtocolDomainConstDescriptorPtr(UDP, SocketPage);
  CONSTP2VAR(TcpIp_Socket_UDP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr =
    TcpIp_Socket_getIntSocketPtr(UDP, protocolDomainConstDescPtr, socketIdx);

  TS_PARAM_UNUSED(Abort);

  switch(socketPtr->generic.aligned.state)
  {
    case TCPIP_SOCKET_STATE_BOUND:
    {
      const TcpIp_LocalAddrIdType localAddrId =socketPtr->generic.aligned.local.addrId;

      /* !LINKSTO TcpIp.Design.Socket.EA0.Close,1 */
      SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

      /* remove from active socket list */
      (void)TcpIp_Socket_Udp_searchAndRemoveSocket
              ( TCPIP_IP_DUALSTACK_PARAM(SocketPage)
                TcpIp_Socket_Udp_autoGetActiveSocketListEntryPtr
                  (TCPIP_IP_DUALSTACK_PARAM(SocketPage) localAddrId),
                socketIdx
              );

      /* update state */
      socketPtr->generic.aligned.state = TCPIP_SOCKET_STATE_UNUSED;

      /* insert into free socket list */
      /* Deviation MISRAC2012-8 <+4> */
      TcpIp_Socket_insertSocket(UDP, protocolDomainConstDescPtr,
          &protocolDomainDescriptorPtr->freeSockets,
          socketIdx  );

      /* !LINKSTO TcpIp.Design.Socket.EA0.Close,1 */
      SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

      TcpIp_Udp_close(SocketId, socketPtr->generic.aligned.sockOwnerId);

      retVal = E_OK;

      break;
    }
    /* intended fall trough */
    case TCPIP_SOCKET_STATE_USED:
    {
      /* !LINKSTO TcpIp.Design.Socket.EA0.Close,1 */
      SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

      /* update state */
     socketPtr->generic.aligned.state = TCPIP_SOCKET_STATE_UNUSED;

      /* insert into free socket list */
      /* Deviation MISRAC2012-8 <+3> */
      TcpIp_Socket_insertSocket(UDP, protocolDomainConstDescPtr,
        &protocolDomainDescriptorPtr->freeSockets,
        socketIdx  );

      /* !LINKSTO TcpIp.Design.Socket.EA0.Close,1 */
      SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

      TcpIp_Udp_close(SocketId, socketPtr->generic.aligned.sockOwnerId);

      retVal = E_OK;

      break;
    }
    /* CHECK: NOPARSE */
    case TCPIP_SOCKET_STATE_UNUSED: /* intended fall through */
    case TCPIP_SOCKET_STATE_ACTIVE: /* intended fall through */
    case TCPIP_SOCKET_STATE_LISTEN: /* intended fall through */
    default:
    {
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    }
    /* CHECK: PARSE */
  }

  return retVal;
}



TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Socket_autoBindUdp
(
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2VAR(TcpIp_LocalAddrIdType,AUTOMATIC,TCPIP_APPL_DATA) unicastLocalAddrIdPtr,
  P2VAR(uint16,AUTOMATIC,TCPIP_APPL_DATA) portPtr,
  TcpIp_SocketIdType socketId
)
{
  TcpIp_ReturnType retVal = TCPIP_E_NOT_OK;

  /* extract socket index from socket ID */
  const TcpIp_SocketIdType socketIdx = TCPIP_SOCKET_GETINDEX(socketId);
#if (TCPIP_SOCKET_DYNAMIC_DISPATCH == STD_ON)
  const uint8 SocketPage = TCPIP_SOCKET_GETPAGE(socketId);
#endif /* (TCPIP_SOCKET_DYNAMIC_DISPATCH == STD_ON) */
  TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
      TcpIp_Socket_getProtocolDomainConstDescriptorPtr(UDP, SocketPage);
  /* get pointer to socket data structure */
  CONSTP2VAR(TcpIp_Socket_UDP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr =
    TcpIp_Socket_getIntSocketPtr(UDP, protocolDomainConstDescPtr, socketIdx);

  DBG_TCPIP_SOCKET_AUTOBINDUDP_ENTRY(remoteAddrPtr,unicastLocalAddrIdPtr,portPtr,socketId);

  switch(socketPtr->generic.aligned.state)
  {
    case TCPIP_SOCKET_STATE_BOUND:
    {
      TcpIp_LocalAddrIdType originalLocalAddrId = socketPtr->generic.aligned.local.addrId;

      /* if socket is already bound, we get the tx localAddrId */
      if(originalLocalAddrId == TCPIP_LOCALADDRID_ANY)
      {
        *unicastLocalAddrIdPtr = TcpIp_Ip_getTxLocalAddrId_byRemoteIp(remoteAddrPtr);
      }
      else
      {
        const TcpIp_IpAddrM_AddrInfoType addrInfo = TcpIp_IpAddrM_getLocalAddrInfo(originalLocalAddrId);
        const TcpIp_LocalAddrIdType AnylocalAddrId = TcpIp_IpAddrM_getAnyIpLocalAddrId(originalLocalAddrId);

        if((AnylocalAddrId == originalLocalAddrId) || TCPIP_IPADDRM_IS_ADDRTYPE(addrInfo,MULTICAST))
        {
          /* if socket is bound to a multicast or any, select an appropriate unicast address */
          *unicastLocalAddrIdPtr =
              TcpIp_Ip_getTxLocalAddrId_byLocalAddrId
                (socketPtr->generic.aligned.local.addrId, remoteAddrPtr);
        }
        else
        {
          /* if socket is bound to a unicast, use the bound address */
          *unicastLocalAddrIdPtr = originalLocalAddrId;
        }
      }

      if(*unicastLocalAddrIdPtr != TCPIP_LOCALADDRID_INVALID)
      {
        /* get the port from the socket */
        *portPtr = socketPtr->generic.aligned.local.port;

        retVal = E_OK;
      }
      break;
    }
    case TCPIP_SOCKET_STATE_USED:
    {
      /* if socket is not bound, we bind it automatically */

      /* get the correct localAddrId */
      const TcpIp_LocalAddrIdType localAddrId = TcpIp_Ip_getTxLocalAddrId_byRemoteIp(remoteAddrPtr);
      uint16 port;

      /* !LINKSTO TcpIp.Design.Socket.EA0.autoBindUdp,1 */
      SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

      if(localAddrId != TCPIP_LOCALADDRID_INVALID)
      {
        if(TcpIp_IpAddrM_getIfState(localAddrId) == TCPIP_STATE_ONLINE)
        {
          /* get an automatic port */
          port =
            TcpIp_Socket_getPort(TCPIP_SOCKET_DYNAMIC_DISPATCH_IF(SocketPage));

          /* save new state (binding) to socket */
          socketPtr->generic.aligned.local.port = port;
          socketPtr->generic.aligned.local.addrId = localAddrId;
          socketPtr->generic.aligned.state = TCPIP_SOCKET_STATE_BOUND;

          /* insert into localAddrId active socket list at first position */
          /* Deviation MISRAC2012-8 <+4> */
          TcpIp_Socket_insertSocket(UDP, protocolDomainConstDescPtr,
              &protocolDomainConstDescPtr->localAddrActiveSockets[localAddrId],
              socketIdx  );

          *portPtr = port;
          *unicastLocalAddrIdPtr = localAddrId;

          retVal = E_OK;
        }
      }

      /* !LINKSTO TcpIp.Design.Socket.EA0.autoBindUdp,1 */
      SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

      break;
    }

    /* CHECK: NOPARSE */
    case TCPIP_SOCKET_STATE_UNUSED: /* intended fall through */
    case TCPIP_SOCKET_STATE_ACTIVE: /* intended fall through */
    case TCPIP_SOCKET_STATE_LISTEN: /* intended fall through */
    default:
    {
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    }
    /* CHECK: PARSE */
  }

  DBG_TCPIP_SOCKET_AUTOBINDUDP_EXIT(retVal,remoteAddrPtr,unicastLocalAddrIdPtr,portPtr,socketId);

  return retVal;
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Socket_Udp_closeSockets
(
  TcpIp_LocalAddrIdType localAddrId
)
{

#if (TCPIP_SOCKET_NUM_UDP_IPV4 > 0U)
    TcpIp_Socket_Udp_terminateSocketsDomain(TcpIp_Socket_SocketPage_UDP_IPV4, localAddrId);
#endif /* (TCPIP_SOCKET_NUM_UDP_IPV4 > 0U) */
#if (TCPIP_SOCKET_NUM_UDP_IPV6 > 0U)
    TcpIp_Socket_Udp_terminateSocketsDomain(TcpIp_Socket_SocketPage_UDP_IPV6, localAddrId);
#endif /* (TCPIP_SOCKET_NUM_UDP_IPV6 > 0U) */

}

TS_MOD_PRIV_DEFN FUNC(uint8, TCPIP_CODE) TcpIp_Socket_lookupUdp
(
  uint8 maxSocketInstances,
  uint16 local_port,
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) localIpAddrPtr,
  TcpIp_DomainType domain,
  P2VAR(TcpIp_SocketIdType,AUTOMATIC,TCPIP_APPL_DATA) socketIdsPtr
)
{
  uint8_least SocketsFound = 0U;

  DBG_TCPIP_SOCKET_LOOKUPUDP_ENTRY(maxSocketInstances,local_port,localAddrId,localIpAddrPtr,domain,socketIdsPtr);
  {
    const uint8 addrInfo = TcpIp_IpAddrM_getLocalAddrInfo(localAddrId);
    const uint8 SocketPage = TCPIP_SOCKET_CREATEPAGE(UDP, addrInfo);

#if ( TCPIP_UDP_ENABLED == STD_ON ) && ( TCPIP_TCP_ENABLED == STD_ON )
    if(TCPIP_SOCKET_ISVALIDSOCKETPAGE(SocketPage))
#endif /* ( TCPIP_UDP_ENABLED == STD_ON ) && ( TCPIP_TCP_ENABLED == STD_ON ) */
    {
      const uint8 MaxResults =
                    TCPIP_IPADDRM_IS_ADDRTYPE(addrInfo,UNICAST) ? 1U : maxSocketInstances;
      TcpIp_Socket_ProtocolDomainDescriptorConstPtrType protocolDomainDescriptorPtr=
        TcpIp_Socket_getProtocolDomainDescriptorPtr(UDP, SocketPage);
      TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
          TcpIp_Socket_getProtocolDomainConstDescriptorPtr(UDP, SocketPage);
      TcpIp_LocalAddrIdType AnylocalAddrId = TCPIP_LOCALADDRID_INVALID;
#if (TCPIP_UDP_BROADCAST_RECEPTION_ENABLED == STD_ON)
      boolean boundAddress = FALSE;
#endif /* (TCPIP_UDP_BROADCAST_RECEPTION_ENABLED == STD_ON) */

      /* !LINKSTO TcpIp.Design.Socket.EA0.lookupUdp,1 */
      SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

      /* search through LocalAddr specific socket list */
      SocketsFound +=
          TcpIp_Socket_Udp_lookupSingleList
          (
            SocketPage,
            &socketIdsPtr[SocketsFound],
            &protocolDomainConstDescPtr->localAddrActiveSockets[localAddrId],
            local_port,
            MaxResults
          );
#if (TCPIP_UDP_BROADCAST_RECEPTION_ENABLED == STD_ON)
      if(SocketsFound != 0U)
      {
        boundAddress = TRUE;
      }
#endif /* (TCPIP_UDP_BROADCAST_RECEPTION_ENABLED == STD_ON) */

      /* search through interface specific ANY socket list */
      if(SocketsFound < MaxResults)
      {
        AnylocalAddrId = TcpIp_IpAddrM_getAnyIpLocalAddrId(localAddrId);
        if(AnylocalAddrId != TCPIP_LOCALADDRID_INVALID)
        {
          SocketsFound +=
              TcpIp_Socket_Udp_lookupSingleList
              (
                SocketPage,
                &socketIdsPtr[SocketsFound],
                &protocolDomainConstDescPtr->localAddrActiveSockets[AnylocalAddrId],
                local_port,
                (uint8)(MaxResults - SocketsFound)
              );
        }
      }

      /* search through ANY specific socket list */
      if(SocketsFound < MaxResults)
      {
        SocketsFound +=
            TcpIp_Socket_Udp_lookupSingleList
            (
              SocketPage,
              &socketIdsPtr[SocketsFound],
              &protocolDomainDescriptorPtr->anySockets,
              local_port,
              (uint8)(MaxResults - SocketsFound)
            );
      }

#if (TCPIP_UDP_BROADCAST_RECEPTION_ENABLED == STD_ON)
      if((SocketsFound < MaxResults) && (!boundAddress))
      {
        const uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
        TcpIp_LocalAddrIdType startIndex;
        TcpIp_LocalAddrIdType endIndex;
        /* search through ANY specific socket list */
        if(TcpIp_Ip_isAddrLimBroadcastAllNodesMulticast(ctrlIdx,localAddrId,localIpAddrPtr,domain) == TRUE)
        {
          uint8 i;
          TcpIp_Ip_getCtrlAddrRange(ctrlIdx,&startIndex,&endIndex,domain);

          /* iterate over all local addresses assigned to this controller */
          for(i = startIndex; i < endIndex; i++)
          {
            TCPIP_PBCFG_PRECONDITION_ASSERT(i, intIpLocalAddrCfg, TCPIP_INTERNAL_API_ID);
            {
              const uint8 ctrlLocalAddrId =
                (TCPIP_CFG_TYPE(uint8,intIpLocalAddr,i));
              CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) lAddrPtr =
                      &(TCPIP_CFG(localAddr,ctrlLocalAddrId));

              if
              (
                (TCPIP_IPADDRM_IS_ADDRTYPE(lAddrPtr->addrType,UNICAST)) &&
                (localAddrId != ctrlLocalAddrId) &&
                (localAddrId != AnylocalAddrId)
              )
              {
                /* search through LocalAddr specific socket list */
                SocketsFound +=
                    TcpIp_Socket_Udp_lookupSingleList
                    (
                      SocketPage,
                      &socketIdsPtr[SocketsFound],
                      &protocolDomainConstDescPtr->localAddrActiveSockets[ctrlLocalAddrId],
                      local_port,
                      (uint8)(MaxResults - SocketsFound)
                    );
              }
              TCPIP_POSTCONDITION_ASSERT(SocketsFound <= MaxResults,TCPIP_INTERNAL_API_ID);
            }
          }
        }
      }
#else /* (TCPIP_UDP_BROADCAST_RECEPTION_ENABLED == STD_ON) */
  TS_PARAM_UNUSED(localIpAddrPtr);
  TS_PARAM_UNUSED(domain);
#endif /* (TCPIP_UDP_BROADCAST_RECEPTION_ENABLED == STD_ON) */

      /* !LINKSTO TcpIp.Design.Socket.EA0.lookupUdp,1 */
      SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();
    }
  }

  DBG_TCPIP_SOCKET_LOOKUPUDP_EXIT((uint8)SocketsFound,maxSocketInstances,local_port,localAddrId,localIpAddrPtr,domain,socketIdsPtr);

  return (uint8)SocketsFound;
}

TS_MOD_PRIV_DEFN FUNC(boolean, TCPIP_CODE) TcpIp_Socket_Udp_isPortUsed
(
#if (TCPIP_IP_DUALSTACK == STD_ON)
  uint8 SocketPage,
#endif /* (TCPIP_IP_DUALSTACK == STD_ON) */
  uint16 port
)
{
  TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
      TcpIp_Socket_getProtocolDomainConstDescriptorPtr(UDP, SocketPage);
  boolean portAlreadyUsed = FALSE;
  uint16_least i = 0U;

#if((TCPIP_SOCKET_NUM_UDP_IPV4 > 1U) || (TCPIP_SOCKET_NUM_UDP_IPV6 > 1U))
  for(i = 0U; i < protocolDomainConstDescPtr->numSockets; i++)
#endif /* ((TCPIP_SOCKET_NUM_UDP_IPV4 > 1U) || (TCPIP_SOCKET_NUM_UDP_IPV6 > 1U)) */
  {
    CONSTP2VAR(TcpIp_Socket_UDP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr =
      TcpIp_Socket_getIntSocketPtr(UDP, protocolDomainConstDescPtr, (TcpIp_SocketIdType) i);

    if((socketPtr->generic.aligned.state == TCPIP_SOCKET_STATE_BOUND) &&
       (socketPtr->generic.aligned.local.port == port))
    {
      portAlreadyUsed = TRUE;
#if((TCPIP_SOCKET_NUM_UDP_IPV4 > 1U) || (TCPIP_SOCKET_NUM_UDP_IPV6 > 1U))
      break;
#endif /* ((TCPIP_SOCKET_NUM_UDP_IPV4 > 1U) || (TCPIP_SOCKET_NUM_UDP_IPV6 > 1U)) */
    };
  }

  return portAlreadyUsed;
}

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Socket_Udp_isAlreadyBound
(
#if (TCPIP_IP_DUALSTACK == STD_ON)
  uint8 SocketPage,
#endif /* (TCPIP_IP_DUALSTACK == STD_ON) */
  uint16 Port,
  TcpIp_LocalAddrIdType LocalAddrId,
  TcpIp_SocketIdType socketIdx
)
{
  TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
      TcpIp_Socket_getProtocolDomainConstDescriptorPtr(UDP, SocketPage);
  Std_ReturnType result = E_NOT_OK;
  TcpIp_SocketIdType nextSocketIndex;
  uint8 isUnicast = FALSE;

  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  if(LocalAddrId != TCPIP_LOCALADDRID_ANY)
  {
    /* unicasts can only be bound to a single socket, unless it is ANY, multicast can be bound on multiple ones */
    if(TcpIp_IpAddrM_getAnyIpLocalAddrId(LocalAddrId) != LocalAddrId)
    {
      if(TCPIP_IPADDRM_IS_ADDRTYPE(TcpIp_IpAddrM_getLocalAddrInfo(LocalAddrId),UNICAST) == TRUE)
      {
        isUnicast = TRUE;
      }
    }
  }

  if(isUnicast == TRUE)
  {
    /* search Socket list for matching active sockets */
    nextSocketIndex = protocolDomainConstDescPtr->localAddrActiveSockets[LocalAddrId];
    if (nextSocketIndex == TCPIP_SOCKETID_INVALID)
    {
      result = E_OK;
    }
    else
    {
      /* search the active socket list and check if socket is already in state bound */
      while(nextSocketIndex != TCPIP_SOCKETID_INVALID)
      {
        CONSTP2VAR(TcpIp_Socket_UDP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr =
        TcpIp_Socket_getIntSocketPtr(UDP, protocolDomainConstDescPtr, nextSocketIndex);

        if
        (
          (socketIdx != nextSocketIndex) &&
          (TCPIP_SOCKET_STATE_BOUND == socketPtr->generic.aligned.state) &&
          (Port == socketPtr->generic.aligned.local.port) &&
          (LocalAddrId == socketPtr->generic.aligned.local.addrId)
        )
        {
          result = E_NOT_OK;
          break;
        }
        else
        {
          result = E_OK;
        }

        nextSocketIndex = socketPtr->generic.aligned.next;
      }
    }
  }
  else
  {
    result = E_OK;
  }

  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  return result;

}
#endif /* ( TCPIP_DEV_ERROR_DETECT == STD_ON ) */

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Socket_Udp_getConnectionProperties
(
  TcpIp_SocketIdType SocketId,
  P2VAR(TcpIp_Socket_IsConnectionReadyConnectionPropertiesType, AUTOMATIC, TCPIP_APPL_DATA) conPropertiesPtr
)
{
  TcpIp_ReturnType retVal = TCPIP_E_NOT_OK;
  TcpIp_Socket_UDP_Generic_Type const *socketPtr;

  /* !LINKSTO TcpIp.Design.Socket.EA0.IsConnectionReady.Udp,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  /* Deviation MISRAC2012-8 */
  socketPtr = (TcpIp_Socket_UDP_Generic_Type const *) TcpIp_Socket_getSocketPtr(SocketId);

  conPropertiesPtr->localAddrId = socketPtr->generic.aligned.local.addrId;
  conPropertiesPtr->localPort = socketPtr->generic.aligned.local.port;
  conPropertiesPtr->protocol = socketPtr->generic.aligned.ipParameters.protocol;

  /* Socket has to be bound and local address has to be known */
  if
  (
    (socketPtr->generic.aligned.state == TCPIP_SOCKET_STATE_BOUND) &&
    (conPropertiesPtr->localAddrId != TCPIP_LOCALADDRID_ANY)
  )
  {
    /* Local address has to be unicast */
    if
    (
      TCPIP_IPADDRM_IS_ADDRTYPE
      (
        TcpIp_IpAddrM_getLocalAddrInfo(conPropertiesPtr->localAddrId), UNICAST
      )
    )
    {
      retVal = TCPIP_OK;
    }
  }

  /* !LINKSTO TcpIp.Design.Socket.EA0.IsConnectionReady.Udp,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  if(retVal != TCPIP_OK)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_ISCONNECTIONREADY, TCPIP_E_NOTCONN);
  }
#endif /* (TCPIP_DEV_ERROR_DETECT == STD_ON) */

  return retVal;
}

/*==================[ internal function definitions]=========================*/

STATIC FUNC_P2VAR(TcpIp_Socket_UDP_Generic_Type, TCPIP_APPL_DATA, TCPIP_CODE) TcpIp_Socket_Udp_searchAndRemoveSocket
(
#if (TCPIP_IP_DUALSTACK == STD_ON)
  uint8 SocketPage,
#endif /* (TCPIP_IP_DUALSTACK == STD_ON) */
  P2VAR(TcpIp_SocketIdType,AUTOMATIC,TCPIP_APPL_DATA) nextEntryPtr,
  TcpIp_SocketIdType socketIdx
)
{
  TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
      TcpIp_Socket_getProtocolDomainConstDescriptorPtr(UDP, SocketPage);
  const TcpIp_SocketIdType listEntryIndex = *nextEntryPtr;
  /* remove from local address socket list */
  P2VAR(TcpIp_Socket_UDP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr =
    TcpIp_Socket_getIntSocketPtr(UDP, protocolDomainConstDescPtr, listEntryIndex);

  if(listEntryIndex == socketIdx)
  {
    *nextEntryPtr = socketPtr->generic.aligned.next;
  }
  else
  {
    P2VAR(TcpIp_Socket_UDP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) prevSocketPtr = socketPtr;

    /* iterate through linked list to find the socket in the bound-socket-list */
    while(socketPtr->generic.aligned.next != socketIdx)
    {
      /* Deviation MISRAC2012-8  <+2> */
      socketPtr = TcpIp_Socket_getIntSocketPtr
                    (UDP, protocolDomainConstDescPtr, socketPtr->generic.aligned.next);
      prevSocketPtr = socketPtr;
    }
    /* remove from active-socket-list */
    /* Deviation MISRAC2012-8  <+2> */
    TcpIp_Socket_removeNextSocket(UDP, protocolDomainConstDescPtr,
                                  &prevSocketPtr->generic.aligned.next );
  }

  return socketPtr;
}

STATIC FUNC(uint8, TCPIP_CODE) TcpIp_Socket_Udp_lookupSingleList
(
  uint8 SocketPage,
  P2VAR(TcpIp_SocketIdType,AUTOMATIC,TCPIP_APPL_DATA) socketIdsPtr,
  P2CONST(TcpIp_SocketIdType,AUTOMATIC,TCPIP_APPL_DATA) nextEntryPtr,
  uint16 local_port,
  uint8 maxSocketInstances
)
{
  TcpIp_SocketIdType nextSocketIndex = *nextEntryPtr;
  uint8_least SocketsFound = 0U;

  while((nextSocketIndex != TCPIP_SOCKETID_INVALID) && (SocketsFound < maxSocketInstances))
  {
    TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
        TcpIp_Socket_getProtocolDomainConstDescriptorPtr(UDP, SocketPage);
    const TcpIp_SocketIdType currentSocket = nextSocketIndex;

    CONSTP2VAR(TcpIp_Socket_UDP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr =
      TcpIp_Socket_getIntSocketPtr(UDP, protocolDomainConstDescPtr, currentSocket);

    /* socket must be in state BOUND */
    TCPIP_PRECONDITION_ASSERT
      (socketPtr->generic.aligned.state == TCPIP_SOCKET_STATE_BOUND,TCPIP_INTERNAL_API_ID);

    if(socketPtr->generic.aligned.local.port == local_port)
    {
      /* found matching socket */
      socketIdsPtr[SocketsFound] =
        TCPIP_SOCKET_CREATEID(SocketPage, nextSocketIndex);
      SocketsFound++;
    }
    nextSocketIndex = socketPtr->generic.aligned.next;
  }

  return (uint8)SocketsFound;
}

STATIC FUNC_P2VAR(TcpIp_SocketIdType, TCPIP_APPL_DATA, TCPIP_CODE)
 TcpIp_Socket_Udp_autoGetActiveSocketListEntryPtr
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
    socketPtr = &TcpIp_Socket_getProtocolDomainDescriptorPtr(UDP, SocketPage)->anySockets;
  }
  else
  {
    socketPtr = &TcpIp_Socket_getProtocolDomainConstDescriptorPtr(UDP, SocketPage)->localAddrActiveSockets
                                      [localAddrId];
  }

  return socketPtr;
}


STATIC FUNC(void, TCPIP_CODE) TcpIp_Socket_Udp_initDomain
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
    TcpIp_Socket_getProtocolDomainConstDescriptorPtr(UDP, SocketPage)->localAddrActiveSockets
                         [(TcpIp_LocalAddrIdType) i]=  TCPIP_SOCKETID_INVALID;
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Socket_Udp_terminateSocketsDomain
(
  uint8 SocketPage,
  TcpIp_LocalAddrIdType localAddrId
)
{
  TcpIp_SocketIdType firstSocket;
  TcpIp_SocketIdType nextSocket;
  TcpIp_SocketIdType validSocket = TCPIP_SOCKETID_INVALID;
  P2VAR(TcpIp_SocketIdType,AUTOMATIC,TCPIP_APPL_DATA) socketIdPtr;
  TcpIp_Socket_ProtocolDomainDescriptorConstPtrType protocolDomainDescriptorPtr=
      TcpIp_Socket_getProtocolDomainDescriptorPtr(UDP, SocketPage);
  TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
      TcpIp_Socket_getProtocolDomainConstDescriptorPtr(UDP, SocketPage);

  /* !LINKSTO TcpIp.Design.Socket.EA0.closeSockets,1 */
  /* !LINKSTO TcpIp.Design.Socket.EA0.terminateSockets,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  /* clear socket list completely */
  socketIdPtr = TcpIp_Socket_Udp_autoGetActiveSocketListEntryPtr
                  (TCPIP_IP_DUALSTACK_PARAM(SocketPage) localAddrId);
  firstSocket = *socketIdPtr;
  *socketIdPtr = TCPIP_SOCKETID_INVALID;

  /* !LINKSTO TcpIp.Design.Socket.EA0.closeSockets,1 */
  /* !LINKSTO TcpIp.Design.Socket.EA0.terminateSockets,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  nextSocket = firstSocket;

  if(firstSocket != TCPIP_SOCKETID_INVALID)
  {
    /* iterate over all sockets of the addressed list */
    while(nextSocket != TCPIP_SOCKETID_INVALID)
    {
      CONSTP2VAR(TcpIp_Socket_UDP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr =
        TcpIp_Socket_getIntSocketPtr(UDP, protocolDomainConstDescPtr, nextSocket);

      socketPtr->generic.aligned.state = TCPIP_SOCKET_STATE_UNUSED;

      /* tell Udp that socket is to be closed */
      TcpIp_Udp_close
        (
          TCPIP_SOCKET_CREATEID(SocketPage, nextSocket),
          socketPtr->generic.aligned.sockOwnerId
        );

      validSocket = nextSocket;
      nextSocket = socketPtr->generic.aligned.next;
    }

    /* !LINKSTO TcpIp.Design.Socket.EA0.closeSockets,1 */
    /* !LINKSTO TcpIp.Design.Socket.EA0.terminateSockets,1 */
    SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

    /* insert complete list into into free socket list */
    /* Deviation MISRAC2012-8 <+4> */
    TcpIp_Socket_insertSocketList(UDP, protocolDomainConstDescPtr,
        &protocolDomainDescriptorPtr->freeSockets,
        firstSocket,
        validSocket );

    /* !LINKSTO TcpIp.Design.Socket.EA0.closeSockets,1 */
    /* !LINKSTO TcpIp.Design.Socket.EA0.terminateSockets,1 */
    SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();
  }


}


#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#else
  TS_PREVENTEMPTYTRANSLATIONUNIT
#endif /* (TCPIP_UDP_ENABLED == STD_ON) */
/*==================[end of file]===========================================*/
