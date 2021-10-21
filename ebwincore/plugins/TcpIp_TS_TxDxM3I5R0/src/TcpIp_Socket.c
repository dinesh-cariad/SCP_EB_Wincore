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
 * MISRAC2012-1) Deviated Rule: 20.10 (advisory)
 * The # and ## preprocessor operators should not be used.
 *
 * Reason:
 * Usage of concatenation operator improves readability of code.
 *
 *
 * MISRAC2012-2) Deviated Rule: 5.5 (required)
 * Identifiers shall be distinct from macro names.
 *
 * Reason:
 * For easier associating design transitions and transitions in the source code.
 *
 *
 * MISRAC2012-8) Deviated Rule: 11.3 (required)
 * A cast shall not be performed between a pointer to object type and a pointer to a different
 * object type.
 *
 * Reason:
 * 1) Socket is a generic type which is dependent on the actual instance.
 * Thus for usage of this concrete type, a cast is necessary.
 * 2) The cast is necessary in C to mimic a polymorphic assignment of a derived type to the base type pointer.
 *
 */

/*==================[inclusions]============================================*/
#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */
#include <TcpIp_Udp_Int.h>      /* used SW-unit interface file */
#include <TcpIp_Tcp_Int.h>      /* used SW-unit interface file */
#include <TcpIp_IpAddrM_Int.h>  /* used SW-unit interface file */

#if (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL)
#include <TcpIp_IpSecDb_Int.h>  /* used SW-unit interface file */
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL) */

#include <TcpIp_Socket_Priv.h>  /* own SW-unit internal header file; must be last */

/*==================[macros]================================================*/

/** \brief TcpIp_GetSocket() service ID */
#define TCPIP_SID_GETSOCKET                3u

/** \brief Dispatch a generic socket service call.
 * The Socket Page selects the implementation based on the protocol \a page */
/* Deviation MISRAC2012-1 <START> */
#define TcpIp_Socket_Protocol_Dispatch(protocol, functionCall) \
  TCPIP_IF_UDP_TCP_BOTH_NEITHER(  \
    TcpIp_Socket_Udp_##functionCall, \
    TcpIp_Socket_Tcp_##functionCall, \
    TcpIp_Socket_Protocol_DispatchArray[(protocol)].ptr##functionCall, \
    E_NOT_OK /* NEITHER_UDP_NOR_TCP_CONFIGURED */ )
/* Deviation MISRAC2012-1 <STOP> */

/*==================[type definitions]======================================*/

/** \brief function type of type specific TcpIp_GetSocket() implementation */
typedef P2FUNC(Std_ReturnType,TCPIP_CODE,TcpIp_Socket_GetSocket_FctPtrType)
(
  uint8 SocketPage,
  P2VAR(TcpIp_SocketIdType, AUTOMATIC, TCPIP_APPL_DATA),
  uint8 sockOwnerId
);

/** \brief function type of type specific TcpIp_Bind() implementation */
typedef P2FUNC(Std_ReturnType, TCPIP_CODE, TcpIp_Socket_Bind_FctPtrType)
(
  TcpIp_SocketIdType SocketId,
  TcpIp_LocalAddrIdType LocalAddrId,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) PortPtr
);

/** \brief function type of type specific TcpIp_Close() implementation */
typedef P2FUNC(Std_ReturnType,TCPIP_CODE,TcpIp_Socket_Close_FctPtrType)
(
  TcpIp_SocketIdType SocketId,
  boolean Abort
);

/** \brief function type of type specific TcpIp_Socket_closeSockets()/
 * TcpIp_Socket_terminateSockets() implementation */
typedef P2FUNC(boolean, TCPIP_CODE, TcpIp_Socket_closeSockets_FctPtrType)
(
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief function type of type specific TcpIp_Socket_isPortUsed()
 * implementation */
typedef P2FUNC(boolean, TCPIP_CODE, TcpIp_Socket_isPortUsed_FctPtrType)
(
#if (TCPIP_IP_DUALSTACK == STD_ON)
  uint8 SocketPage,
#endif /* (TCPIP_IP_DUALSTACK == STD_ON) */
  uint16 port
);

typedef P2FUNC(TcpIp_ReturnType, TCPIP_CODE, TcpIp_Socket_getConnectionProperties_FctPtrType)
(
  TcpIp_SocketIdType SocketId,
  P2VAR(TcpIp_Socket_IsConnectionReadyConnectionPropertiesType, AUTOMATIC, TCPIP_APPL_DATA) conPropertiesPtr
);

/** \brief Set of function pointers for generic socket operations */
typedef struct
{
  TcpIp_Socket_GetSocket_FctPtrType ptrGetSocket;
  TcpIp_Socket_Bind_FctPtrType ptrBind;
  TcpIp_Socket_Close_FctPtrType ptrClose;
  TcpIp_Socket_isPortUsed_FctPtrType ptrisPortUsed;
  TcpIp_Socket_getConnectionProperties_FctPtrType ptrgetConnectionProperties;
} TcpIp_Socket_Protocol_DispatchType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

#if ( TCPIP_UDP_ENABLED == STD_ON ) || ( TCPIP_TCP_ENABLED == STD_ON )

/* initializes generic parts of a socket page */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Socket_pageInit
(
#if (TCPIP_SOCKET_DYNAMIC_DISPATCH == STD_ON)
  uint8 SocketPage
#else /* (TCPIP_SOCKET_DYNAMIC_DISPATCH == STD_OFF) */
  void
#endif /* TCPIP_SOCKET_DYNAMIC_DISPATCH */
);

#endif /* ( TCPIP_UDP_ENABLED == STD_ON ) || ( TCPIP_TCP_ENABLED == STD_ON ) */


#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )

/* function to evaluate if local address is valid */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Socket_checkLocalAddr
(
  TcpIp_LocalAddrIdType localAddrId,
  TcpIp_SocketIdType SocketId
);

#if ( TCPIP_UDP_ENABLED == STD_ON )

/* function to evaluate if another socket is already bound to the same port, protocol
   and local address */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Socket_isAlreadyBound
(
  TcpIp_SocketIdType socketId,
  uint16 Port,
  TcpIp_LocalAddrIdType LocalAddrId
);

#endif /* ( TCPIP_UDP_ENABLED == STD_ON ) */

#endif /* ( TCPIP_DEV_ERROR_DETECT == STD_ON ) */



#if ( TCPIP_UDP_ENABLED == STD_ON ) || ( TCPIP_TCP_ENABLED == STD_ON )
/* function to change priority through TcpIp_ChangeParameter */
STATIC FUNC( Std_ReturnType, TCPIP_CODE ) TcpIp_ChangeParameter_FramePrio
(
  const uint8 vlanPriority,
  CONSTP2VAR(TcpIp_Ip_IpParameterType,AUTOMATIC,TCPIP_APPL_DATA) ipParameterPtr
);

/* function to change TTL through TcpIp_ChangeParameter */
STATIC FUNC( Std_ReturnType, TCPIP_CODE ) TcpIp_ChangeParameter_TTL
(
  const uint8 ttl,
  CONSTP2VAR(TcpIp_Ip_IpParameterType,AUTOMATIC,TCPIP_APPL_DATA) ipParameterPtr
);

/* function to set through TcpIp_ChangeParameter if TcpIp_UdpTransmit shall use unspecified address */
STATIC FUNC( Std_ReturnType, TCPIP_CODE ) TcpIp_ChangeParameter_Unspecified_IP
(
  const uint8 useUnspecifiedSrc,
  CONSTP2VAR(TcpIp_Ip_IpParameterType,AUTOMATIC,TCPIP_APPL_DATA) ipParameterPtr,
  uint8 socketOwnerId
);

/* function to change flow label through TcpIp_ChangeParameter */
STATIC FUNC( Std_ReturnType, TCPIP_CODE ) TcpIp_ChangeParameter_FlowLabel
(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) ParameterValue,
  CONSTP2VAR(TcpIp_Ip_IpParameterType,AUTOMATIC,TCPIP_APPL_DATA) ipParameterPtr
);

/* function to change dscp through TcpIp_ChangeParameter */
STATIC FUNC( Std_ReturnType, TCPIP_CODE ) TcpIp_ChangeParameter_Dscp
(
  const uint8 dspc,
  CONSTP2VAR(TcpIp_Ip_IpParameterType,AUTOMATIC,TCPIP_APPL_DATA) ipParameterPtr
);

#endif /* ( TCPIP_UDP_ENABLED == STD_ON ) || ( TCPIP_TCP_ENABLED == STD_ON ) */

#if ( TCPIP_UDP_ENABLED == STD_ON )

/* function to set TcpIp_ChangeParameter if udp checksum will be calculated through */
STATIC FUNC( Std_ReturnType, TCPIP_CODE ) TcpIp_ChangeParameter_UDP_Checksum
(
  TcpIp_SocketIdType SocketId,
  TcpIp_ParamIdType  ParameterId,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) ParameterValue,
  P2VAR(TcpIp_Socket_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr
);

#endif /* ( TCPIP_UDP_ENABLED == STD_ON ) */

#if ( TCPIP_TCP_ENABLED == STD_ON )
/* function to change tcp parameters through TcpIp_ChangeParameter */
STATIC FUNC( Std_ReturnType, TCPIP_CODE ) TcpIp_ChangeParameter_TCP_Parameters
(
  TcpIp_SocketIdType SocketId,
  TcpIp_ParamIdType  ParameterId,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) ParameterValue,
  P2VAR(TcpIp_Socket_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr
);
#endif /* ( TCPIP_TCP_ENABLED == STD_ON ) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[external constants]====================================*/

#define TCPIP_START_SEC_CONST_UNSPECIFIED
#include <TcpIp_MemMap.h>

#if ( TCPIP_UDP_ENABLED == STD_ON ) || ( TCPIP_TCP_ENABLED == STD_ON )

/** \brief Constant socket information */
CONST(TcpIp_Socket_ProtocolDomainConst_DescriptorType,TCPIP_VAR)
  TcpIp_Socket_ProtocolDomainConst_DescriptorArray[TCPIP_SOCKETPAGE_NUM_VALID_CONFIGS] =
{
# if (TCPIP_UDP_ENABLED == STD_ON)
#   if (TCPIP_SOCKET_NUM_UDP_IPV4 > 0U)
  {
    TcpIp_Socket_localAddrActiveSockets_UDP_IPV4,
    NULL_PTR,
    /* Deviation MISRAC2012-8 <+2> */
    (P2VAR(TcpIp_Socket_Generic_Type, TCPIP_VAR, TCPIP_VAR_CLEARED))
      TcpIp_Socket_SocketMemory_UDP_IPV4,
    TcpIp_Socket_numSockets_UDP_IPV4,
    TcpIp_Socket_socketSize_UDP_IPV4,
    TcpIp_Socket_remoteSize_UDP_IPV4
  },
#   endif /* (TCPIP_SOCKET_NUM_UDP_IPV4 > 0U) */
#   if (TCPIP_SOCKET_NUM_UDP_IPV6 > 0U)
  {
    TcpIp_Socket_localAddrActiveSockets_UDP_IPV6,
    NULL_PTR,
    /* Deviation MISRAC2012-8 <+2> */
    (P2VAR(TcpIp_Socket_Generic_Type, TCPIP_VAR, TCPIP_VAR_CLEARED))
      TcpIp_Socket_SocketMemory_UDP_IPV6,
    TcpIp_Socket_numSockets_UDP_IPV6,
    TcpIp_Socket_socketSize_UDP_IPV6,
    TcpIp_Socket_remoteSize_UDP_IPV6
  },
#   endif /* (TCPIP_SOCKET_NUM_UDP_IPV6 > 0U) */
# endif /* (TCPIP_UDP_ENABLED == STD_ON) */
# if (TCPIP_TCP_ENABLED == STD_ON)
#   if (TCPIP_SOCKET_NUM_TCP_IPV4 > 0U)
  {
    TcpIp_Socket_localAddrActiveSockets_TCP_IPV4,
    TcpIp_Socket_localAddrListenSockets_TCP_IPV4,
    /* Deviation MISRAC2012-8 <+2> */
    (P2VAR(TcpIp_Socket_Generic_Type, TCPIP_VAR, TCPIP_VAR_CLEARED))
      TcpIp_Socket_SocketMemory_TCP_IPV4,
    TcpIp_Socket_numSockets_TCP_IPV4,
    TcpIp_Socket_socketSize_TCP_IPV4,
    TcpIp_Socket_remoteSize_TCP_IPV4
  },
#   endif /* (TCPIP_SOCKET_NUM_TCP_IPV4 > 0U) */
#   if (TCPIP_SOCKET_NUM_TCP_IPV6 > 0U)
  {
    TcpIp_Socket_localAddrActiveSockets_TCP_IPV6,
    TcpIp_Socket_localAddrListenSockets_TCP_IPV6,
    /* Deviation MISRAC2012-8 <+2> */
    (P2VAR(TcpIp_Socket_Generic_Type, TCPIP_VAR, TCPIP_VAR_CLEARED))
      TcpIp_Socket_SocketMemory_TCP_IPV6,
    TcpIp_Socket_numSockets_TCP_IPV6,
    TcpIp_Socket_socketSize_TCP_IPV6,
    TcpIp_Socket_remoteSize_TCP_IPV6
  }
#   endif /* (TCPIP_SOCKET_NUM_TCP_IPV6 > 0U) */
# endif /* (TCPIP_TCP_ENABLED == STD_ON) */
};

#endif /* ( TCPIP_UDP_ENABLED == STD_ON ) || ( TCPIP_TCP_ENABLED == STD_ON ) */

#define TCPIP_STOP_SEC_CONST_UNSPECIFIED
#include <TcpIp_MemMap.h>

/*==================[internal constants]====================================*/


#define TCPIP_START_SEC_CONST_UNSPECIFIED
#include <TcpIp_MemMap.h>

#if (TCPIP_UDP_ENABLED == STD_ON) && (TCPIP_TCP_ENABLED == STD_ON)

/** \brief protocol service Socket page specific function pointer lists */
STATIC CONST(TcpIp_Socket_Protocol_DispatchType,TCPIP_CONST)
  TcpIp_Socket_Protocol_DispatchArray[TCPIP_SOCKET_NUMPROT] =
{
  {
    &TcpIp_Socket_Udp_GetSocket,
    &TcpIp_Socket_Udp_Bind,
    &TcpIp_Socket_Udp_Close,
    &TcpIp_Socket_Udp_isPortUsed,
    &TcpIp_Socket_Udp_getConnectionProperties,
  },
  {
    &TcpIp_Socket_Tcp_GetSocket,
    &TcpIp_Socket_Tcp_Bind,
    &TcpIp_Socket_Tcp_Close,
    &TcpIp_Socket_Tcp_isPortUsed,
    &TcpIp_Socket_Tcp_getConnectionProperties
  }
};

#endif /* (TCPIP_UDP_ENABLED == STD_ON) && (TCPIP_TCP_ENABLED == STD_ON) */

#define TCPIP_STOP_SEC_CONST_UNSPECIFIED
#include <TcpIp_MemMap.h>

/*==================[external data]=========================================*/


#if (TCPIP_SOCKETPAGE_NUM_VALID_CONFIGS > 0u)
#define TCPIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

/** \brief generic service Socket page specific function pointer lists */
VAR(TcpIp_Socket_ProtocolDomain_DescriptorType,TCPIP_VAR)
  TcpIp_Socket_ProtocolDomain_DescriptorArray[TCPIP_SOCKETPAGE_NUM_VALID_CONFIGS];

#define TCPIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>
#endif /* TCPIP_SOCKETPAGE_NUM_VALID_CONFIGS > 0u */

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
#if ( TCPIP_UDP_ENABLED == STD_ON ) ||  ( TCPIP_TCP_ENABLED == STD_ON )
/* Deviation MISRAC2012-2 */
TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Socket_checkSocketProtocolFct
(
  TcpIp_SocketIdType socketId,
  uint8 protocol
)
{
  /* get socket page */
  Std_ReturnType result;
  const uint8 SocketPage = TCPIP_SOCKET_GETPAGE(socketId);

  DBG_TCPIP_SOCKET_CHECKSOCKETPROTOCOLFCT_ENTRY(socketId,protocol);

  /* compare socket page against expected protocol */
  if(TCPIP_SOCKET_PAGE_PROTOCOL_GET(SocketPage) != protocol)
  {
    result = E_NOT_OK;
  }
  else
  {
    result = E_OK;
  }

  DBG_TCPIP_SOCKET_CHECKSOCKETPROTOCOLFCT_EXIT(result,socketId,protocol);

  return result;
}

#endif /* ( TCPIP_UDP_ENABLED == STD_ON ) ||  ( TCPIP_TCP_ENABLED == STD_ON ) */

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Socket_checkSocketId
(
  TcpIp_SocketIdType socketId
)
{
  Std_ReturnType result= E_NOT_OK;
  DBG_TCPIP_SOCKET_CHECKSOCKETID_ENTRY(socketId);
  {
#if ( TCPIP_UDP_ENABLED == STD_OFF ) && ( TCPIP_TCP_ENABLED == STD_OFF )
    TS_PARAM_UNUSED(socketId);
#else /* ( TCPIP_UDP_ENABLED == STD_OFF ) && ( TCPIP_TCP_ENABLED == STD_OFF ) */
    const TcpIp_SocketIdType socketIdx = TCPIP_SOCKET_GETINDEX(socketId);
    /* extract socket page and socket index from socketID */
    const uint8 SocketPage = TCPIP_SOCKET_GETPAGE(socketId);
    TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
          TcpIp_Socket_getProtocolDomainConstDescriptorPtr(ANY, SocketPage);

    /* check SocketPage / descriptor availability (configured) */
    if (!TCPIP_SOCKET_ISVALIDSOCKETPAGE(SocketPage))
    {
      result = E_NOT_OK;
    }
    /* check socket index range */
    else if(socketIdx >= protocolDomainConstDescPtr->numSockets)
    {
      result = E_NOT_OK;
    }
    /* check if socket is in use */
    else if(TcpIp_Socket_getIntSocketPtr(ANY, protocolDomainConstDescPtr, socketIdx)
            ->aligned.state != TCPIP_SOCKET_STATE_UNUSED)
    {
      result = E_OK;
    }
    else
#endif /* ( TCPIP_UDP_ENABLED == STD_OFF ) && ( TCPIP_TCP_ENABLED == STD_OFF ) */
    {
      result = E_NOT_OK;
    }
  }
  DBG_TCPIP_SOCKET_CHECKSOCKETID_EXIT(result,socketId);

  return result;
}

#endif /* ( TCPIP_DEV_ERROR_DETECT == STD_ON ) */



#if ( TCPIP_UDP_ENABLED == STD_ON ) || ( TCPIP_TCP_ENABLED == STD_ON )

TS_MOD_PRIV_DEFN FUNC_P2VAR(TcpIp_Socket_Generic_Type, TCPIP_APPL_DATA, TCPIP_CODE)
  TcpIp_Socket_getSocketPtr
(
  TcpIp_SocketIdType socketId
)
{
  TcpIp_Socket_Generic_PtrType result;
  TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
     TcpIp_Socket_getProtocolDomainConstDescriptorPtr(ANY, TCPIP_SOCKET_GETPAGE(socketId));
  /* note: must verify that uint8~char because sizeof() is defined in multiples of char */
  TS_STATIC_ASSERT(sizeof(uint8)==sizeof(char),
  Address_computations_incorrect_in_TcpIp_Socket_getIntSocketPtr_when_uint8_not_equivalent_to_char);

  DBG_TCPIP_SOCKET_GETSOCKETPTR_ENTRY(socketId);

  result=
      /* Deviation MISRAC2012-8 */
      TcpIp_Socket_getIntSocketPtr
        (ANY, protocolDomainConstDescPtr, TCPIP_SOCKET_GETINDEX(socketId));

  DBG_TCPIP_SOCKET_GETSOCKETPTR_EXIT(result, socketId);
  return result;
}

#if TCPIP_IP_DUALSTACK == STD_ON
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Socket_checkDomain
(
  TcpIp_SocketIdType socketId,
  TcpIp_DomainType domain
)
{
  Std_ReturnType result = E_NOT_OK;

  DBG_TCPIP_SOCKET_CHECKDOMAIN_ENTRY(socketId,domain);

  {
    /* extract socket page and socket index from socketID */
    const uint8 SocketPage = TCPIP_SOCKET_GETPAGE(socketId);
    const uint8 SocketDomain =
      TCPIP_SOCKET_IS_PAGE_DOMAIN(SocketPage, IPV4) ? TCPIP_AF_INET : TCPIP_AF_INET6;

    if(SocketDomain == domain)
    {
      result = E_OK;
    }
  }

  DBG_TCPIP_SOCKET_CHECKDOMAIN_EXIT(result,socketId,domain);

  return result;
}

#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
#endif /* TCPIP_IP_DUALSTACK == STD_ON */

#endif /* ( TCPIP_UDP_ENABLED == STD_ON ) || ( TCPIP_TCP_ENABLED == STD_ON ) */



FUNC( Std_ReturnType, TCPIP_CODE ) TcpIp_Socket_getSocket
(
  TcpIp_DomainType Domain,
  TcpIp_ProtocolType Protocol,
  P2VAR(TcpIp_SocketIdType, AUTOMATIC, TCPIP_APPL_DATA) SocketIdPtr,
  uint8 sockOwnerId
)
{
  Std_ReturnType retval = E_NOT_OK;
  DBG_TCPIP_SOCKET_GETSOCKET_ENTRY(Domain,Protocol,SocketIdPtr,sockOwnerId);

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )

  if( TCPIP_UNINIT )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_GETSOCKET, TCPIP_E_NOTINIT);
  }
  else if((Protocol != TCPIP_IPPROTO_UDP) && (Protocol != TCPIP_IPPROTO_TCP))
  {
    /* no valid protocol */
    TCPIP_REPORT_ERROR(TCPIP_SID_GETSOCKET, TCPIP_E_INV_ARG);
  }
#if ( TCPIP_UDP_ENABLED == STD_OFF )
  else if( Protocol == TCPIP_IPPROTO_UDP )
  {
    /* UDP socket requested but none configured */
    TCPIP_REPORT_ERROR(TCPIP_SID_GETSOCKET, TCPIP_E_NOPROTOOPT);
  }
#endif /* ( TCPIP_UDP_ENABLED == STD_OFF ) */
#if ( TCPIP_TCP_ENABLED == STD_OFF )
  else if( Protocol == TCPIP_IPPROTO_TCP )
  {
    /* TCP socket requested but none configured */
    TCPIP_REPORT_ERROR(TCPIP_SID_GETSOCKET, TCPIP_E_NOPROTOOPT);
  }
#endif /* ( TCPIP_TCP_ENABLED == STD_OFF ) */
  else if( TCPIP_IP_IF_V4_V6_BOTH
           (
            (Domain != TCPIP_AF_INET),
            (Domain != TCPIP_AF_INET6),
            ((Domain != TCPIP_AF_INET) && (Domain != TCPIP_AF_INET6))
           )
         )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_GETSOCKET, TCPIP_E_AFNOSUPPORT);
  }
  else if( SocketIdPtr == NULL_PTR )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_GETSOCKET, TCPIP_EB_E_PARAM_POINTER);
  }
  else if(sockOwnerId >= TcpIp_NumSockOwner)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_GETSOCKET, TCPIP_E_INV_ARG);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
#if ( ( TCPIP_UDP_ENABLED == STD_ON ) ||  ( TCPIP_TCP_ENABLED == STD_ON ))
    const uint8 domain =
      TCPIP_IP_IF_V4_V6_BOTH
                    (
                      (TCPIP_IP_DOMAIN_IPV4),
                      (TCPIP_IP_DOMAIN_IPV6),
                      ((Domain == TCPIP_AF_INET) ? TCPIP_IP_DOMAIN_IPV4 : TCPIP_IP_DOMAIN_IPV6)
                    );

    const uint8 SocketPage =
      domain |
      (
        (
          TCPIP_IP_IF_UDP_TCP_BOTH
          (
            (TCPIP_SOCKET_PAGE_PROTOCOL_UDP),
            (TCPIP_SOCKET_PAGE_PROTOCOL_TCP),
            ((Protocol == TCPIP_IPPROTO_UDP) ?
              TCPIP_SOCKET_PAGE_PROTOCOL_UDP : TCPIP_SOCKET_PAGE_PROTOCOL_TCP)
          )
        ) << 1U
      );

#if (TCPIP_IP_DUALSTACK == STD_OFF)
    /* if dual stack is not active, domain parameter is not used */
    TS_PARAM_UNUSED(Domain);
#endif /* (TCPIP_IP_DUALSTACK == STD_OFF) */
#if (TCPIP_SOCKETPAGE_NUM_VALID_CONFIGS > 1U)
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
    if (!TCPIP_SOCKET_ISVALIDSOCKETPAGE(SocketPage))
    {
      /* Socket of protocol requested that is not configured */
      TCPIP_REPORT_ERROR(TCPIP_SID_GETSOCKET, TCPIP_E_NOPROTOOPT);
    }
    else
#endif /* (TCPIP_DEV_ERROR_DETECT == STD_ON)*/
#endif /* (TCPIP_SOCKETPAGE_NUM_VALID_CONFIGS > 1U) */
    {
      /* call SocketPage specific GetSocket() service */
      retval = TcpIp_Socket_Protocol_Dispatch(TCPIP_SOCKET_PAGE_PROTOCOL_GET(SocketPage),
               GetSocket(SocketPage, SocketIdPtr, sockOwnerId) );
    }
#endif /* ( ( TCPIP_UDP_ENABLED == STD_ON ) ||  ( TCPIP_TCP_ENABLED == STD_ON )) */
  }
#if ( !(( TCPIP_UDP_ENABLED == STD_ON ) &&  ( TCPIP_TCP_ENABLED == STD_ON)) && (TCPIP_DEV_ERROR_DETECT == STD_OFF) )
  TS_PARAM_UNUSED(Protocol);
#endif /* ( !(( TCPIP_UDP_ENABLED == STD_ON ) &&  ( TCPIP_TCP_ENABLED == STD_ON)) && (TCPIP_DEV_ERROR_DETECT == STD_OFF) ) */

  DBG_TCPIP_SOCKET_GETSOCKET_EXIT(retval,Domain,Protocol,SocketIdPtr,sockOwnerId);

  return retval;
} /* eof - TcpIp_GetSocket */

FUNC( Std_ReturnType, TCPIP_CODE ) TcpIp_Bind
(
  TcpIp_SocketIdType SocketId,
  TcpIp_LocalAddrIdType LocalAddrId,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) PortPtr
)
{
  Std_ReturnType retval = E_NOT_OK;

  retval = TcpIp_Socket_bind(SocketId, LocalAddrId, PortPtr, TRUE);

  return retval;
} /* eof - TcpIp_Bind */



FUNC( Std_ReturnType, TCPIP_CODE ) TcpIp_Close
(
  TcpIp_SocketIdType SocketId,
  boolean Abort
)
{
  Std_ReturnType retval = E_NOT_OK;

  DBG_TCPIP_CLOSE_ENTRY(SocketId,Abort);

  TS_PARAM_UNUSED(Abort);

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )

  if( TCPIP_UNINIT )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_CLOSE, TCPIP_E_NOTINIT);
  }
  else if(TcpIp_Socket_checkSocketId(SocketId) != E_OK)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_CLOSE, TCPIP_E_INV_ARG);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* call SocketPage specific Close() service */
    retval =  TcpIp_Socket_Protocol_Dispatch(TCPIP_SOCKET_PAGE_PROTOCOL_GET(TCPIP_SOCKET_GETPAGE(SocketId)),
                Close(SocketId,Abort) );
  }

  DBG_TCPIP_CLOSE_EXIT(retval,SocketId,Abort);

  return retval;
} /* eof - TcpIp_Close */



FUNC( Std_ReturnType, TCPIP_CODE ) TcpIp_ChangeParameter
(
  TcpIp_SocketIdType SocketId,
  TcpIp_ParamIdType  ParameterId,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) ParameterValue
)
{
  Std_ReturnType retval = E_NOT_OK;

  DBG_TCPIP_CHANGEPARAMETER_ENTRY(SocketId,ParameterId,ParameterValue);

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
  if( TCPIP_UNINIT )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_CHANGEPARAMETER, TCPIP_E_NOTINIT);
  }
  else if( ParameterValue == NULL_PTR )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_CHANGEPARAMETER, TCPIP_EB_E_PARAM_POINTER);
  }
  else if(TcpIp_Socket_checkSocketId(SocketId) != E_OK)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_CHANGEPARAMETER, TCPIP_E_INV_ARG);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
#if ( TCPIP_UDP_ENABLED == STD_ON ) || ( TCPIP_TCP_ENABLED == STD_ON )

    TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
       TcpIp_Socket_getProtocolDomainConstDescriptorPtr(ANY, TCPIP_SOCKET_GETPAGE(SocketId));

    /* call SocketPage specific getSocketPtr() service to get the IpParameters */
    CONSTP2VAR(TcpIp_Socket_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr =
      /* Deviation MISRAC2012-8 */
      TcpIp_Socket_getIntSocketPtr
        ( ANY,
          protocolDomainConstDescPtr,
          TCPIP_SOCKET_GETINDEX(SocketId)
        );

    CONSTP2VAR(TcpIp_Ip_IpParameterType,AUTOMATIC,TCPIP_APPL_DATA) ipParameterPtr =
      &socketPtr->aligned.ipParameters;

    switch(ParameterId)
    {
      case TCPIP_PARAMID_FRAMEPRIO:
      {
        retval = TcpIp_ChangeParameter_FramePrio(*ParameterValue,ipParameterPtr);
        break;
      }
      case TCPIP_PARAMID_TTL:
      {
        retval = TcpIp_ChangeParameter_TTL(*ParameterValue,ipParameterPtr);
        break;
      }
      case TCPIP_PARAMID_UDP_UNSPECIFIED_IP:
      {
        retval = TcpIp_ChangeParameter_Unspecified_IP
                 (
                   *ParameterValue,
                   ipParameterPtr,
                   socketPtr->aligned.sockOwnerId
                 );
        break;
      }
      case TCPIP_PARAMID_FLOWLABEL:
      {
        retval = TcpIp_ChangeParameter_FlowLabel(ParameterValue,ipParameterPtr);
        break;
      }
      case TCPIP_PARAMID_DSCP:
      {
        retval = TcpIp_ChangeParameter_Dscp(*ParameterValue,ipParameterPtr);
        break;
      }
#if ( TCPIP_UDP_ENABLED == STD_ON )
      case TCPIP_PARAMID_UDP_CHECKSUM:
      {
        retval = TcpIp_ChangeParameter_UDP_Checksum(SocketId,ParameterId,ParameterValue,socketPtr);
        break;
      }
#endif /* ( TCPIP_UDP_ENABLED == STD_ON ) */

#if ( TCPIP_TCP_ENABLED == STD_ON )
      case TCPIP_PARAMID_TCP_NAGLE:                   /* fall through intended */
      case TCPIP_PARAMID_TCP_RXWND_MAX:               /* fall through intended */
#if (TCPIP_TCP_OPTION_FILTER_ENABLE == STD_ON)
      case TCPIP_PARAMID_TCP_OPTIONFILTER:            /* fall through intended */
#endif /* (TCPIP_TCP_OPTION_FILTER_ENABLE == STD_ON) */
#if (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON)
      case TCPIP_PARAMID_TCP_KEEPALIVE:               /* fall through intended */
      case TCPIP_PARAMID_TCP_KEEPALIVE_TIME:          /* fall through intended */
      case TCPIP_PARAMID_TCP_KEEPALIVE_PROBES_MAX:    /* fall through intended */
      case TCPIP_PARAMID_TCP_KEEPALIVE_INTERVAL:
#endif /* ( TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON ) */
      {
        retval = TcpIp_ChangeParameter_TCP_Parameters(SocketId,ParameterId,ParameterValue,socketPtr);
        break;
      }
#endif /* ( TCPIP_TCP_ENABLED == STD_ON ) */
      default:
      {
        TCPIP_REPORT_ERROR(TCPIP_SID_CHANGEPARAMETER, TCPIP_E_INV_ARG);
        break;
      }
    }
#else /* ( TCPIP_UDP_ENABLED == STD_ON ) || ( TCPIP_TCP_ENABLED == STD_ON ) */
    TS_PARAM_UNUSED(ParameterId);
#if ( TCPIP_DEV_ERROR_DETECT != STD_ON )
    TS_PARAM_UNUSED(SocketId);
    TS_PARAM_UNUSED(ParameterValue);
#endif /* ( TCPIP_DEV_ERROR_DETECT != STD_ON ) */

#endif /* ( TCPIP_UDP_ENABLED == STD_ON ) || ( TCPIP_TCP_ENABLED == STD_ON ) */

  }
  DBG_TCPIP_CHANGEPARAMETER_EXIT(retval,SocketId,ParameterId,ParameterValue);
  return retval;
} /* eof - TcpIp_ChangeParameterRequest */



FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_IsConnectionReady
(
  TcpIp_SocketIdType SocketId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RemoteAddrPtr
)
{
  TcpIp_ReturnType retVal = TCPIP_E_NOT_OK;

  DBG_TCPIP_ISCONNECTIONREADY_ENTRY(SocketId, RemoteAddrPtr);

#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  if(TCPIP_UNINIT)
  {
    /* TcpIp not initialized */
    TCPIP_REPORT_ERROR(TCPIP_SID_ISCONNECTIONREADY, TCPIP_E_NOTINIT);
  }
#if (TCPIP_UDP_ENABLED == STD_ON) || (TCPIP_TCP_ENABLED == STD_ON)
  else if((TcpIp_Socket_checkSocketProtocol(SocketId, UDP) == E_OK) && (RemoteAddrPtr == NULL_PTR))
  {
    /* UDP socket and RemoteAddrPtr is null */
    TCPIP_REPORT_ERROR(TCPIP_SID_ISCONNECTIONREADY, TCPIP_E_PARAM_POINTER);
  }
  else if(TcpIp_Socket_checkSocketId(SocketId) != E_OK)
  {
    /* Invalid socket ID */
    TCPIP_REPORT_ERROR(TCPIP_SID_ISCONNECTIONREADY, TCPIP_E_INV_ARG);
  }
  else if(RemoteAddrPtr != NULL_PTR)
  {
    if
    (
      TCPIP_IP_IF_V4_V6_BOTH
      (
        (RemoteAddrPtr->domain != TCPIP_AF_INET),
        (RemoteAddrPtr->domain != TCPIP_AF_INET6),
        (TcpIp_Socket_checkDomain(SocketId, RemoteAddrPtr->domain) != E_OK)
      )
    )
    {
      /* Domain not supported */
      TCPIP_REPORT_ERROR(TCPIP_SID_ISCONNECTIONREADY, TCPIP_E_AFNOSUPPORT);
    }
    else
    {
      retVal = TCPIP_OK;
    }
  }
  else
  {
    retVal = TCPIP_OK;
  }
  if(retVal == TCPIP_OK)
#endif /* (TCPIP_UDP_ENABLED == STD_ON) || (TCPIP_TCP_ENABLED == STD_ON) */
#endif /* (TCPIP_DEV_ERROR_DETECT == STD_ON) */

  {
#if (TCPIP_UDP_ENABLED == STD_ON) || (TCPIP_TCP_ENABLED == STD_ON)
    TcpIp_Socket_IsConnectionReadyConnectionPropertiesType conProperties;

#if (TCPIP_TCP_ENABLED == STD_ON)
    TCPIP_IP_IF_V4_V6_BOTH(TcpIp_SockAddrInetType, TcpIp_SockAddrInet6Type, TcpIp_SockAddrInet6Type)
      remoteAddrStorage;
    /* Deviation MISRAC2012-8 */
    conProperties.remoteAddrStoragePtr = (TcpIp_SockAddrPtrType) &remoteAddrStorage;
#endif /* (TCPIP_TCP_ENABLED == STD_ON) */

    conProperties.remoteAddrPtr = RemoteAddrPtr;

    /* call SocketPage specific get port service */
    retVal = TcpIp_Socket_Protocol_Dispatch(
      TCPIP_SOCKET_PAGE_PROTOCOL_GET(TCPIP_SOCKET_GETPAGE(SocketId)),
      getConnectionProperties(SocketId, &conProperties));

#if (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL)
    {
      uint8 ipSecConIdx;
      uint8 ipSecMechanism;

      if(retVal == TCPIP_OK)
      {
        ipSecMechanism = TcpIp_IpSecDb_checkSecurityTxPolicy
        (
          conProperties.localAddrId,
          conProperties.remoteAddrPtr,
          conProperties.protocol,
          conProperties.localPort,
          &ipSecConIdx
        );

        if(ipSecMechanism == TCPIP_IPSECDB_MECHANISM_DISCARDED)
        {
          retVal = TCPIP_E_NOT_OK;
        }
#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)
        else if(ipSecMechanism == TCPIP_IPSECDB_MECHANISM_SECURED)
        {
          TcpIp_IpSecSaDataType ipSecSaData;
          Std_ReturnType status =
            TcpIp_IpSecDb_getSecurityTxAssociation(ipSecConIdx, &ipSecSaData, FALSE);

          /* Check if SA is established. */
          if(status != E_OK)
          {
            retVal = TCPIP_E_PENDING;
          }
          /* Else: SA is established => retVal = TCPIP_OK already set. */
        }
#endif /* ((TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */
        else
        {
          /* Mechanism is bypassed or secured but not active */
          /* retVal = TCPIP_OK already set, but else branch needed for MISRA */
        }
      }
    }
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL) */
    if(retVal == TCPIP_OK)
    {
      uint8 physAddr[TCPIP_ETH_ADDR_LENGTH];
      uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(conProperties.localAddrId);

      /* Check if physical address of remote is known, otherwise trigger address resolution */
      retVal = TcpIp_GetRemotePhysAddr(ctrlIdx, conProperties.remoteAddrPtr, physAddr, TRUE);

      if(retVal == TCPIP_E_PHYS_ADDR_MISS)
      {
        retVal = TCPIP_E_PENDING;
      }
    }
#else
    TS_PARAM_UNUSED(SocketId);
    TS_PARAM_UNUSED(RemoteAddrPtr);
#endif /* (TCPIP_UDP_ENABLED == STD_ON) || (TCPIP_TCP_ENABLED == STD_ON) */
  }

  DBG_TCPIP_ISCONNECTIONREADY_EXIT(retVal, SocketId, RemoteAddrPtr);
  return retVal;
}


/*==================[module internal function definitions]=======================*/


TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Socket_init(void)
{
  DBG_TCPIP_SOCKET_INIT_ENTRY();

#if (TCPIP_SOCKET_NUM_UDP_IPV4 > 0U)
  TcpIp_Socket_pageInit(TCPIP_SOCKET_DYNAMIC_DISPATCH_IF(TcpIp_Socket_SocketPage_UDP_IPV4));
#endif /* (TCPIP_SOCKET_NUM_UDP_IPV4 > 0U) */
#if (TCPIP_SOCKET_NUM_UDP_IPV6 > 0U)
  TcpIp_Socket_pageInit(TCPIP_SOCKET_DYNAMIC_DISPATCH_IF(TcpIp_Socket_SocketPage_UDP_IPV6));
#endif /* (TCPIP_SOCKET_NUM_UDP_IPV6 > 0U) */

#if (TCPIP_SOCKET_NUM_TCP_IPV4 > 0U)
  TcpIp_Socket_pageInit(TCPIP_SOCKET_DYNAMIC_DISPATCH_IF(TcpIp_Socket_SocketPage_TCP_IPV4));
#endif /* (TCPIP_SOCKET_NUM_TCP_IPV4 > 0U) */
#if (TCPIP_SOCKET_NUM_TCP_IPV6 > 0U)
  TcpIp_Socket_pageInit(TCPIP_SOCKET_DYNAMIC_DISPATCH_IF(TcpIp_Socket_SocketPage_TCP_IPV6));
#endif /* (TCPIP_SOCKET_NUM_TCP_IPV6 > 0U) */

#if (TCPIP_UDP_ENABLED == STD_ON)
  TcpIp_Socket_Udp_init();
#endif /* (TCPIP_UDP_ENABLED == STD_ON) */
#if (TCPIP_TCP_ENABLED == STD_ON)
  TcpIp_Socket_Tcp_init();
#endif /* (TCPIP_TCP_ENABLED == STD_ON) */

  DBG_TCPIP_SOCKET_INIT_EXIT();
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Socket_mainFunction
(
  void
)
{
  DBG_TCPIP_SOCKET_MAINFUNCTION_ENTRY();

#if (TCPIP_TCP_ENABLED == STD_ON)
  TcpIp_Socket_processTcpSockets();
#endif /* (TCPIP_TCP_ENABLED == STD_ON) */

  DBG_TCPIP_SOCKET_MAINFUNCTION_EXIT();
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Socket_mainFunctionTx
(
  void
)
{
  DBG_TCPIP_SOCKET_MAINFUNCTIONTX_ENTRY();

#if( TCPIP_TCP_ENABLED == STD_ON )
  TcpIp_Socket_processTcpSocketsTx();
#endif /* ( TCPIP_TCP_ENABLED == STD_ON ) */

  DBG_TCPIP_SOCKET_MAINFUNCTIONTX_EXIT();
}

TS_MOD_PRIV_DEFN FUNC(void,TCPIP_CODE) TcpIp_Socket_closeSockets(TcpIp_LocalAddrIdType localAddrId)
{
  DBG_TCPIP_SOCKET_CLOSESOCKETS_ENTRY(localAddrId);

  TCPIP_PRECONDITION_ASSERT(localAddrId != TCPIP_LOCALADDRID_ANY,TCPIP_INTERNAL_API_ID);

#if ((TCPIP_UDP_ENABLED == STD_ON) || (TCPIP_TCP_ENABLED == STD_ON))
  {
#if TCPIP_UDP_ENABLED == STD_ON
    TcpIp_Socket_Udp_closeSockets(localAddrId);
#endif /* TCPIP_UDP_ENABLED == STD_ON */

# if TCPIP_TCP_ENABLED == STD_ON
    if(TcpIp_Socket_Tcp_closeSockets(localAddrId) == FALSE)
#endif /* TCPIP_TCP_ENABLED == STD_ON */
    {
      /* indicate closed sockets to IpAddrM state machine */
      TcpIp_IpAddrM_socketsClosed(localAddrId);
    }
  }
#else  /* ((TCPIP_UDP_ENABLED == STD_ON) || (TCPIP_TCP_ENABLED == STD_ON)) */
  TS_PARAM_UNUSED(localAddrId);
#endif /* ((TCPIP_UDP_ENABLED == STD_ON) || (TCPIP_TCP_ENABLED == STD_ON)) */

  DBG_TCPIP_SOCKET_CLOSESOCKETS_EXIT(localAddrId);
}



TS_MOD_PRIV_DEFN FUNC(void,TCPIP_CODE) TcpIp_Socket_terminateSockets
(
    TcpIp_LocalAddrIdType localAddrId,
    boolean informRemoteHost
)
{
  DBG_TCPIP_SOCKET_TERMINATESOCKETS_ENTRY(localAddrId,informRemoteHost);

# if TCPIP_UDP_ENABLED == STD_ON
    (void) TcpIp_Socket_Udp_closeSockets(localAddrId); /* for UDP, close and terminate is the same */
# endif /* TCPIP_UDP_ENABLED == STD_ON */
# if TCPIP_TCP_ENABLED == STD_ON
    (void) TcpIp_Socket_Tcp_terminateSockets(localAddrId, informRemoteHost);
# else /* TCPIP_TCP_ENABLED == STD_ON */
    TS_PARAM_UNUSED(informRemoteHost);
# endif /* TCPIP_TCP_ENABLED == STD_ON */

#if ((TCPIP_UDP_ENABLED != STD_ON) && (TCPIP_TCP_ENABLED != STD_ON))
  TS_PARAM_UNUSED(localAddrId);
#endif /* ((TCPIP_UDP_ENABLED != STD_ON) && (TCPIP_TCP_ENABLED != STD_ON)) */

  DBG_TCPIP_SOCKET_TERMINATESOCKETS_EXIT(localAddrId,informRemoteHost);
}

#if ( TCPIP_UDP_ENABLED == STD_ON ) ||  ( TCPIP_TCP_ENABLED == STD_ON )

TS_MOD_PRIV_DEFN FUNC(uint16, TCPIP_CODE) TcpIp_Socket_getPort
(
#if (TCPIP_SOCKET_DYNAMIC_DISPATCH == STD_ON)
  uint8 SocketPage
#else /* (TCPIP_SOCKET_DYNAMIC_DISPATCH == STD_OFF) */
  void
#endif /* TCPIP_SOCKET_DYNAMIC_DISPATCH */
)
{
  boolean portAlreadyUsed;
  uint16 autoPort;
  uint16 TryCountMax = TCPIP_SOCKET_LAST_RANDOM_PORT - TCPIP_SOCKET_FIRST_RANDOM_PORT;
  TcpIp_Socket_ProtocolDomainDescriptorConstPtrType protocolDomainDescriptorPtr=
      TcpIp_Socket_getProtocolDomainDescriptorPtr(ANY, SocketPage);
  do
  {
    autoPort = protocolDomainDescriptorPtr->nextPort;
    protocolDomainDescriptorPtr->nextPort++;
    TryCountMax--;

    if(protocolDomainDescriptorPtr->nextPort < TCPIP_SOCKET_FIRST_RANDOM_PORT)
    {
      protocolDomainDescriptorPtr->nextPort = TCPIP_SOCKET_FIRST_RANDOM_PORT;
    }

    /* check if protocol's port is already used for IP version */
    portAlreadyUsed = TcpIp_Socket_Protocol_Dispatch(
                        TCPIP_SOCKET_PAGE_PROTOCOL_GET(SocketPage),
                        isPortUsed(TCPIP_IP_DUALSTACK_PARAM(SocketPage) autoPort)
                      );

  } while((portAlreadyUsed != FALSE) && (TryCountMax > 0U));

  return autoPort;
}
#endif /* ( TCPIP_UDP_ENABLED == STD_ON ) ||  ( TCPIP_TCP_ENABLED == STD_ON ) */

TS_MOD_PRIV_DEFN FUNC( Std_ReturnType, TCPIP_CODE ) TcpIp_Socket_bind
(
  TcpIp_SocketIdType SocketId,
  TcpIp_LocalAddrIdType LocalAddrId,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) PortPtr,
  boolean checkDet
)
{
  Std_ReturnType retval = E_NOT_OK;
  DBG_TCPIP_BIND_ENTRY(SocketId,LocalAddrId,PortPtr);

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
  if(checkDet)
  {
    if( TCPIP_UNINIT )
    {
      TCPIP_REPORT_ERROR(TCPIP_SID_BIND, TCPIP_E_NOTINIT);
    }
    else if( PortPtr == NULL_PTR )
    {
      TCPIP_REPORT_ERROR(TCPIP_SID_BIND, TCPIP_EB_E_PARAM_POINTER);
    }
    else if(TcpIp_Socket_checkSocketId(SocketId) != E_OK)
    {
      TCPIP_REPORT_ERROR(TCPIP_SID_BIND, TCPIP_E_INV_ARG);
    }
    else if (TcpIp_Socket_checkLocalAddr(LocalAddrId, SocketId) != E_OK)
    {
      TCPIP_REPORT_ERROR(TCPIP_SID_BIND, TCPIP_E_ADDRNOTAVAIL);
    }
#if ( TCPIP_UDP_ENABLED == STD_ON )
    else if(TcpIp_Socket_isAlreadyBound(SocketId, *PortPtr, LocalAddrId ) != E_OK)
    {
      TCPIP_REPORT_ERROR(TCPIP_SID_BIND, TCPIP_E_ADDRINUSE);
    }
#endif /* ( TCPIP_UDP_ENABLED == STD_ON ) */
    else
    {
      retval = E_OK;
    }
  }
  else
  {
    retval = E_OK;
  }

  if(retval == E_OK)
#else /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  TS_PARAM_UNUSED(checkDet);
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
    retval = E_NOT_OK;
    if(TcpIp_Ip_getIfState(LocalAddrId) == TCPIP_STATE_ONLINE)
    {
      /* call SocketPage specific Bind() service */
      retval = TcpIp_Socket_Protocol_Dispatch(TCPIP_SOCKET_PAGE_PROTOCOL_GET(TCPIP_SOCKET_GETPAGE(SocketId)),
                  Bind(SocketId,LocalAddrId,PortPtr) );
    }
  }
  DBG_TCPIP_BIND_EXIT(retval,SocketId,LocalAddrId,PortPtr);

  return retval;
} /* eof - TcpIp_Socket_bind */


/*==================[internal function definitions]=========================*/

#if ( TCPIP_UDP_ENABLED == STD_ON ) || ( TCPIP_TCP_ENABLED == STD_ON )

STATIC FUNC(void, TCPIP_CODE) TcpIp_Socket_pageInit
(
#if (TCPIP_SOCKET_DYNAMIC_DISPATCH == STD_ON)
  uint8 SocketPage
#else /* (TCPIP_SOCKET_DYNAMIC_DISPATCH == STD_OFF) */
  void
#endif /* TCPIP_SOCKET_DYNAMIC_DISPATCH */
)
{
  TcpIp_SocketIdType j;
  TcpIp_Socket_ProtocolDomainDescriptorConstPtrType protocolDomainDescriptorPtr=
      TcpIp_Socket_getProtocolDomainDescriptorPtr(ANY, SocketPage);
  TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType protocolDomainConstDescPtr=
     TcpIp_Socket_getProtocolDomainConstDescriptorPtr(ANY, SocketPage);

  /* clear free socket list */
  protocolDomainDescriptorPtr->freeSockets = TCPIP_SOCKETID_INVALID;

  /* clear ANY socket list */
  protocolDomainDescriptorPtr->anySockets = TCPIP_SOCKETID_INVALID;

  protocolDomainDescriptorPtr->nextPort = TCPIP_SOCKET_FIRST_RANDOM_PORT;

  /* put all sockets into free socket list (reverse order causes socketIdx
     0 to be the first one returned at GetSocket()) */
  for(j = protocolDomainConstDescPtr->numSockets; j > 0U; j--)
  {
    const TcpIp_SocketIdType socketIdx = j - 1U; /* socket index */

    /* initialize socket state */
    TcpIp_Socket_getIntSocketPtr(ANY, protocolDomainConstDescPtr, socketIdx)->aligned.state =
      TCPIP_SOCKET_STATE_UNUSED;

    /* put socket into free socket list */
    /* Deviation MISRAC2012-8 */
    TcpIp_Socket_insertSocket
      (ANY, protocolDomainConstDescPtr, &protocolDomainDescriptorPtr->freeSockets, socketIdx);
  }
}

#endif /* ( TCPIP_UDP_ENABLED == STD_ON ) || ( TCPIP_TCP_ENABLED == STD_ON ) */

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Socket_checkLocalAddr
(
  TcpIp_LocalAddrIdType localAddrId,
  TcpIp_SocketIdType SocketId
)
{
  Std_ReturnType retval;

  if((localAddrId != TCPIP_LOCALADDRID_ANY))
  {
    if((localAddrId >= TcpIp_PBcfgPtr->configLocalAddrNum))
    {
      /* local address id is greater than number of configured addresses */
      retval = E_NOT_OK;
    }
    else
    {
      CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
                    &(TCPIP_CFG(localAddr,localAddrId));
      CONSTP2CONST(TcpIp_ctrlConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlPtr =
                    &((TCPIP_CFG(ctrl,localAddrPtr->ctrlIdx)));

#if (TCPIP_IP_DUALSTACK == STD_ON)  || (TCPIP_TCP_ENABLED == STD_ON) /* don't ask, if it's clear */
      if (ctrlPtr->localAddrIdAny == localAddrId)
      {
        retval = E_OK;
      }
      else
      {
        const TcpIp_IpAddrM_AddrInfoType addrInfo = TcpIp_IpAddrM_getLocalAddrInfo(localAddrId);

#if ( TCPIP_TCP_ENABLED == STD_ON )
        if((TcpIp_Socket_checkSocketProtocol(SocketId,TCP) == E_OK) &&
           (TCPIP_IPADDRM_IS_ADDRTYPE(addrInfo,MULTICAST))
          )
        {
          retval = E_NOT_OK;
        }
        else
#endif /* ( TCPIP_TCP_ENABLED == STD_ON ) */
        {
#if ( TCPIP_IP_DUALSTACK == STD_ON )
          const uint8 SocketPage = TCPIP_SOCKET_GETPAGE(SocketId);
          const uint8 SocketDomain = TCPIP_SOCKET_PAGE_DOMAIN_GET(SocketPage);
          const uint8 localAddrDomain =
            TCPIP_IPADDRM_IS_ADDRDOMAIN(addrInfo,IPV4) ?
            TCPIP_IP_DOMAIN_IPV4 :
            TCPIP_IP_DOMAIN_IPV6;

          if(localAddrDomain != SocketDomain)
          {
            /* bind ipv6 address on ipv4 socket or vice versa */
            retval = E_NOT_OK;
          }
          else
          {
            retval = E_OK;
          }
#else /* ( TCPIP_IP_DUALSTACK == STD_ON ) */
          retval = E_OK;
#endif /* TCPIP_IP_DUALSTACK == STD_ON */
        }
      }
#else /* (TCPIP_IP_DUALSTACK == STD_ON)  || (TCPIP_TCP_ENABLED == STD_ON) */
      TS_PARAM_UNUSED(SocketId);
      TS_PARAM_UNUSED(ctrlPtr);
      retval = E_OK;
#endif /* (TCPIP_IP_DUALSTACK == STD_ON)  || (TCPIP_TCP_ENABLED == STD_ON) */
    }
  }
  else
  {
    retval = E_OK;
  }

  return retval;
}

#if ( TCPIP_UDP_ENABLED == STD_ON )

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Socket_isAlreadyBound
(
  TcpIp_SocketIdType socketId,
  uint16 Port,
  TcpIp_LocalAddrIdType LocalAddrId
)
{
  Std_ReturnType result = E_NOT_OK;
  TcpIp_SocketIdType socketIdx = TCPIP_SOCKET_GETINDEX(socketId);
  if(TcpIp_Socket_checkSocketProtocol(socketId,UDP) == E_OK)
  {
    result= TcpIp_Socket_Udp_isAlreadyBound
            (
              TCPIP_IP_DUALSTACK_PARAM(TCPIP_SOCKET_GETPAGE(socketId))
              Port,
              LocalAddrId,
              socketIdx
            );
  }
  else
  {
    result = E_OK;
  }
  return result;
}
#endif /* ( TCPIP_UDP_ENABLED == STD_ON ) */

#endif /* ( TCPIP_DEV_ERROR_DETECT == STD_ON ) */

#if ( TCPIP_UDP_ENABLED == STD_ON ) || ( TCPIP_TCP_ENABLED == STD_ON )
STATIC FUNC( Std_ReturnType, TCPIP_CODE ) TcpIp_ChangeParameter_FramePrio
(
  const uint8 vlanPriority,
  CONSTP2VAR(TcpIp_Ip_IpParameterType,AUTOMATIC,TCPIP_APPL_DATA) ipParameterPtr
)
{
  Std_ReturnType retVal = E_NOT_OK;
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
  if(vlanPriority > 7U)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_CHANGEPARAMETER, TCPIP_E_INV_ARG);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* !LINKSTO TcpIp.Design.Socket.Atomic.ChangeParameter,1 */
    TS_AtomicAssign8(ipParameterPtr->vlanPriority,vlanPriority);
    retVal = E_OK;
  }
  return retVal;
}

STATIC FUNC( Std_ReturnType, TCPIP_CODE ) TcpIp_ChangeParameter_TTL
(
  const uint8 ttl,
  CONSTP2VAR(TcpIp_Ip_IpParameterType,AUTOMATIC,TCPIP_APPL_DATA) ipParameterPtr
)
{
  Std_ReturnType retVal = E_NOT_OK;
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
  if(ttl == 0U)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_CHANGEPARAMETER, TCPIP_E_INV_ARG);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* !LINKSTO TcpIp.Design.Socket.Atomic.ChangeParameter,1 */
    TS_AtomicAssign8(ipParameterPtr->ttl,ttl);
    retVal = E_OK;
  }
  return retVal;
}

STATIC FUNC( Std_ReturnType, TCPIP_CODE ) TcpIp_ChangeParameter_Unspecified_IP
(
  const uint8 useUnspecifiedSrc,
  CONSTP2VAR(TcpIp_Ip_IpParameterType,AUTOMATIC,TCPIP_APPL_DATA) ipParameterPtr,
  uint8 socketOwnerId
)
{
  Std_ReturnType retVal = E_NOT_OK;
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
  if(useUnspecifiedSrc > 1U)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_CHANGEPARAMETER, TCPIP_E_INV_ARG);
  }
  else if(socketOwnerId < TcpIp_NumConfSockOwner)
  {
    /* only internal socket owner are allowed to modify this parameter */
    TCPIP_REPORT_ERROR(TCPIP_SID_CHANGEPARAMETER, TCPIP_E_INV_ARG);
  }
  else
#else /* ( TCPIP_DEV_ERROR_DETECT == STD_ON ) */
  TS_PARAM_UNUSED(socketOwnerId);
#endif /* ( TCPIP_DEV_ERROR_DETECT == STD_ON ) */
  {
    retVal = E_OK;
    if(useUnspecifiedSrc == 0U)
    {
      ipParameterPtr->useUnspecifiedSrc = FALSE;
    }
    else
    {
      ipParameterPtr->useUnspecifiedSrc = TRUE;
    }
  }
  return retVal;
}

STATIC FUNC( Std_ReturnType, TCPIP_CODE ) TcpIp_ChangeParameter_FlowLabel
(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) ParameterValue,
  CONSTP2VAR(TcpIp_Ip_IpParameterType,AUTOMATIC,TCPIP_APPL_DATA) ipParameterPtr
)
{
  Std_ReturnType retVal = E_NOT_OK;
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
  if(!TCPIP_ISALIGNED(ParameterValue, sizeof(uint32)))
  {
    /* invalid parameter size */
    TCPIP_REPORT_ERROR(TCPIP_SID_CHANGEPARAMETER, TCPIP_E_INV_ARG);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* Deviation MISRAC2012-8 */
    P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) flowLabel =
        (P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA))ParameterValue;

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
    if(*flowLabel > 0xFFFFFU)
    {
      /* invalid parameter value */
      TCPIP_REPORT_ERROR(TCPIP_SID_CHANGEPARAMETER, TCPIP_E_INV_ARG);
    }
    else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
    {
      TS_AtomicAssign32(ipParameterPtr->flowLabel, *flowLabel);
      retVal = E_OK;
    }
  }
  return retVal;
}

STATIC FUNC( Std_ReturnType, TCPIP_CODE ) TcpIp_ChangeParameter_Dscp
(
  const uint8 dspc,
  CONSTP2VAR(TcpIp_Ip_IpParameterType,AUTOMATIC,TCPIP_APPL_DATA) ipParameterPtr
)
{
  Std_ReturnType retVal = E_NOT_OK;
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
  if(dspc > 0x3FU)
  {
    /* invalid parameter value */
    TCPIP_REPORT_ERROR(TCPIP_SID_CHANGEPARAMETER, TCPIP_E_INV_ARG);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
    TS_AtomicAssign8(ipParameterPtr->dscp, dspc);
    retVal = E_OK;
  }
  return retVal;
}

#endif /* ( TCPIP_UDP_ENABLED == STD_ON ) || ( TCPIP_TCP_ENABLED == STD_ON ) */

#if ( TCPIP_UDP_ENABLED == STD_ON )

STATIC FUNC( Std_ReturnType, TCPIP_CODE ) TcpIp_ChangeParameter_UDP_Checksum
(
  TcpIp_SocketIdType SocketId,
  TcpIp_ParamIdType  ParameterId,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) ParameterValue,
  P2VAR(TcpIp_Socket_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr
)
{
  Std_ReturnType retVal = E_NOT_OK;
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
        if(TcpIp_Socket_checkSocketProtocol(SocketId,UDP) != E_OK)
        {
          TCPIP_REPORT_ERROR(TCPIP_SID_CHANGEPARAMETER, TCPIP_E_PROTOTYPE);
        }
        else
#else /* ( TCPIP_DEV_ERROR_DETECT == STD_ON ) */
  TS_PARAM_UNUSED(SocketId);
#endif  /* ( TCPIP_DEV_ERROR_DETECT == STD_ON ) */
        {
          /* Deviation MISRAC2012-8 */
          CONSTP2VAR(TcpIp_Socket_UDP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) udpSocketPtr =
              (P2VAR(TcpIp_Socket_UDP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA)) socketPtr;
          retVal = TcpIp_Udp_changeParameter(udpSocketPtr,ParameterValue,ParameterId);
        }
  return retVal;
}

#endif /* ( TCPIP_UDP_ENABLED == STD_ON ) */

#if ( TCPIP_TCP_ENABLED == STD_ON )

STATIC FUNC( Std_ReturnType, TCPIP_CODE ) TcpIp_ChangeParameter_TCP_Parameters
(
  TcpIp_SocketIdType SocketId,
  TcpIp_ParamIdType  ParameterId,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) ParameterValue,
  P2VAR(TcpIp_Socket_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socketPtr
)
{
  Std_ReturnType retVal = E_NOT_OK;
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
  if( TcpIp_Socket_checkSocketProtocol(SocketId,TCP) != E_OK)
  {
    /* No TCP socket */
    TCPIP_REPORT_ERROR(TCPIP_SID_CHANGEPARAMETER, TCPIP_E_PROTOTYPE);
  }
  else
#else /* ( TCPIP_DEV_ERROR_DETECT == STD_ON ) */
  TS_PARAM_UNUSED(SocketId);
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* Deviation MISRAC2012-8 */
    CONSTP2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) tcpSocketPtr =
        (P2VAR(TcpIp_Socket_TCP_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA)) socketPtr;

    retVal = TcpIp_Tcp_changeParameter(tcpSocketPtr,ParameterValue,ParameterId);
  }
  return retVal;
}
#endif /* ( TCPIP_TCP_ENABLED == STD_ON ) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[end of file]===========================================*/
