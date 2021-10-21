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
 * MISRAC2012-8) Deviated Rule: 11.3 (required)
 * A cast shall not be performed between a pointer to object type and a pointer to a different
 * object type.
 *
 * Reason:
 * Sockets come in variants for UDP and TCP. To enable common handling, they must be cast
 * to the generic type from which they are derived as C does not have derived types.
 *
 */
#ifndef TCPIP_SOCKET_PRIV_H
#define TCPIP_SOCKET_PRIV_H

/*==================[inclusions]============================================*/

#include <SchM_TcpIp.h>         /* SchM API declarations */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Socket_Int.h>   /* own SW-unit interface file */
#include <TcpIp_IpAddrM_Int.h>  /* used SW-unit interface file */
#include <TcpIp_Trace.h>        /* debug and trace macros; must be last */

/*==================[macros]================================================*/

/** \brief TcpIp_Bind() service ID */
#define TCPIP_SID_BIND                     5u

/** \brief TcpIp_IsConnectionReady() service ID */
#define TCPIP_SID_ISCONNECTIONREADY        0xF5U

/** \brief socket page ID for UDP IPV4 sockets */
#define TcpIp_Socket_SocketPage_UDP_IPV4 0u
/** \brief socket page ID for UDP IPV6 sockets */
#define TcpIp_Socket_SocketPage_UDP_IPV6 1u
/** \brief socket page ID for TCP IPV4 sockets */
#define TcpIp_Socket_SocketPage_TCP_IPV4 2u
/** \brief socket page ID for TCP IPV6 sockets */
#define TcpIp_Socket_SocketPage_TCP_IPV6 3u

/** \brief Macro extracts the socket index from the socketId (lower 14bits) */
#define TCPIP_SOCKET_GETINDEX(socketId) ((TcpIp_SocketIdType)((socketId)&0x3FFFu))

/** \brief Macro extracts the socket page from the socketId (higher 2 bits) */
#define TCPIP_SOCKET_GETPAGE(socketId) ((uint8)(((socketId)>>14u) & 3u))

/** \brief Check if the \a SocketPage is valid (=is enabled; descriptor exists) */
#define TCPIP_SOCKET_ISVALIDSOCKETPAGE(SocketPage)        \
  ( 0u != (1u & (TCPIP_SOCKETPAGE_MASK >> (SocketPage)) ) )

/** \brief Macro constructs a socketId out of socket page and socket index */
#define TCPIP_SOCKET_CREATEID(SocketPage,socketIdx)      \
  (((uint16)((uint16)SocketPage << 14u))|(socketIdx))

/** \brief Macro constructs a socketId out of socket page and socket index.
 * \param addrInfo the address info that determins the socket page
 * \param PROTOCOL may only be symbolic: UDP or TCP */
/* Deviation MISRAC2012-1 <START> */
#define TCPIP_SOCKET_CREATEPAGE(PROTOCOL,addrInfo)   \
  TCPIP_IP_IF_V4_V6_BOTH( \
    TcpIp_Socket_SocketPage_##PROTOCOL##_IPV4, \
    TcpIp_Socket_SocketPage_##PROTOCOL##_IPV6, \
    (TCPIP_IPADDRM_IS_ADDRDOMAIN((addrInfo), IPV4) \
      ? (TcpIp_Socket_SocketPage_##PROTOCOL##_IPV4) \
      : (TcpIp_Socket_SocketPage_##PROTOCOL##_IPV6) ) )
/* Deviation MISRAC2012-1 <STOP> */

/** \brief Macro extracts the protocol of a socket page */
#define TCPIP_SOCKET_PAGE_DOMAIN_GET(SocketPage) ((SocketPage) & 1u)

/** \brief Macro tests the protocol of a socket page */
/* Deviation MISRAC2012-1 <+2> */
#define TCPIP_SOCKET_IS_PAGE_DOMAIN(SocketPage,domain) \
        (TCPIP_SOCKET_PAGE_DOMAIN_GET(SocketPage) == TCPIP_IP_DOMAIN_##domain)

/* each socket maintains a state */
/** \brief socket is currently unused */
#define TCPIP_SOCKET_STATE_UNUSED       0u
/** \brief socket is currently used but not bound */
#define TCPIP_SOCKET_STATE_USED         1u
/** \brief socket is currently bound (final UDP state) */
#define TCPIP_SOCKET_STATE_BOUND        2u
/** \brief socket is currently in a listening state (only TCP) */
#define TCPIP_SOCKET_STATE_LISTEN       3u
/** \brief socket is currently in an active (connected) state (only TCP) */
#define TCPIP_SOCKET_STATE_ACTIVE       4u

/** \brief Lowest port number which shall be randomly chosen for auto-port assignment */
#define TCPIP_SOCKET_FIRST_RANDOM_PORT 49152U
/** \brief Highest port number which shall be randomly chosen for auto-port assignment */
#define TCPIP_SOCKET_LAST_RANDOM_PORT  65535U

#if ((TCPIP_UDP_ENABLED == STD_ON) && (TCPIP_TCP_ENABLED == STD_ON)) || \
    ((TCPIP_IPV4_ENABLED == STD_ON) && (TCPIP_IPV6_ENABLED == STD_ON))
/** \brief conditional compilation switch for dynamic dispatches. */
#  define TCPIP_SOCKET_DYNAMIC_DISPATCH STD_ON
/** \brief compile-time if-conversion for dynamic dispatches. */
#  define TCPIP_SOCKET_IF_DYNAMIC_DISPATCH(then, else) then
/** \brief conditional compilation macro to avert unused parameter warnings
 *   due to avoided dynamic dispatches. */
#  define TCPIP_SOCKET_DYNAMIC_DISPATCH_IF(then) then
/** \brief conditional compilation macro to avert unused parameter warnings
 *   due to avoided dynamic dispatches. */
#  define TCPIP_SOCKET_DYNAMIC_DISPATCH_PARAM(param) param,
#else
/** \brief conditional compilation switch for dynamic dispatches. */
#  define TCPIP_SOCKET_DYNAMIC_DISPATCH STD_OFF
/** \brief compile-time if-conversion for dynamic dispatches. */
#  define TCPIP_SOCKET_IF_DYNAMIC_DISPATCH(then, else) else
/** \brief conditional compilation macro to avert unused parameter warnings
 *   due to avoided dynamic dispatches. */
#  define TCPIP_SOCKET_DYNAMIC_DISPATCH_IF(then)
/** \brief conditional compilation macro to clean up unused parameter warnings
 *   due to avoided dynamic dispatches. */
#  define TCPIP_SOCKET_DYNAMIC_DISPATCH_PARAM(param)
#endif /* ((TCPIP_UDP_ENABLED == STD_ON) && (TCPIP_TCP_ENABLED == STD_ON)) || \
          ((TCPIP_IPV4_ENABLED == STD_ON) && (TCPIP_IPV6_ENABLED == STD_ON)) */

/** \brief Compile time if-conversion:
 *  evaluating to
 *  the first parameter \a udp, if \a TCPIP_UDP_ENABLED is \a STD_ON,
 *  the second parameter \a tcp, if \a TCPIP_TCP_ENABLED is \a STD_ON,
 *  the third parameter \a both if \a TCPIP_UDP_ENABLED and \a TCPIP_TCP_ENABLED are both \a STD_ON
 *  else to the 4th parameter \a neither if neither protocol is enabled.
 */
#if (TCPIP_UDP_ENABLED == STD_ON) && (TCPIP_TCP_ENABLED == STD_ON)
#  define TCPIP_IF_UDP_TCP_BOTH_NEITHER(udp, tcp, both, neither) both
#elif (TCPIP_TCP_ENABLED == STD_ON)
#  define TCPIP_IF_UDP_TCP_BOTH_NEITHER(udp, tcp, both, neither) tcp
#elif (TCPIP_UDP_ENABLED == STD_ON)
#  define TCPIP_IF_UDP_TCP_BOTH_NEITHER(udp, tcp, both, neither) udp
#else
#  define TCPIP_IF_UDP_TCP_BOTH_NEITHER(udp, tcp, both, neither) neither
#endif

/** \brief returns the SocketPage index adjusted to the enabled configurations.
 * This macro does static dispatch for all SingleStack/SingleProtocol configurations and simplified
 * addressing for DualProtocol/SingleStack and SingleProtocol/DualStack configurations
 * i.e. no calling through function pointers, no dynamic computed data addresses.
 * As much of the address computation as possible is done at compile time,
 * substituting constants for the page parameter.
 * Hence the \a page parameter is ignored in these cases.
 * It falls back to dynamic dispatch for DualProtocol/DualStack configurations.
 */
#define TcpIp_Socket_getSocketPageVarConfigEntryPtr(SocketPage) \
  (&TcpIp_Socket_ProtocolDomain_DescriptorArray[ \
    TcpIp_Socket_getProtocolDomainDescriptorOffset(SocketPage)])

/** \brief returns pointer to the constant socket dispatcher array adjusted to the enabled
           configurations. */
#define TcpIp_Socket_getSocketPageConstConfigEntryPtr(SocketPage) \
  (&TcpIp_Socket_ProtocolDomainConst_DescriptorArray[ \
    TcpIp_Socket_getProtocolDomainDescriptorOffset(SocketPage)])

/** \brief returns the offset of the protocol and domain in the dispatcher function */
#define TcpIp_Socket_getProtocolDomainDescriptorOffset(SocketPage) \
  (((TCPIP_SOCKETPAGE_CONFIG_INDEX) >> ((uint8)((SocketPage) << 1u))) & 3u)


/** \brief Templated macro that returns the desired ProtocolDomainDescriptor address.
 * This macro employs static dispatch for all SingleStack configurations, and when the protocol is
 * known (TCP or UDP) in DualProtocol/SingleStack configurations within code that isn't shared
 * between protocols.
 * i.e. no dynamic computed data addresses are used in those cases.
 * As much of the address computation as possible is done at compile time,
 * substituting constants for the \a SocketPage parameter, which is then unused (not evaluated).
 */
/* Deviation MISRAC2012-1 <+2> */
#define TcpIp_Socket_getProtocolDomainDescriptorPtr(PROTOCOL, SocketPage) \
  TCPIP_SOCKET_GETPROTOCOLDOMAINDESCRIPTORPTR_##PROTOCOL(Var, SocketPage)

/** \brief Templated macro that returns the desired ProtocolDomainDescriptor address.
 * This macro employs static dispatch for all SingleStack configurations, and when the protocol is
 * known (TCP or UDP) in DualProtocol/SingleStack configurations within code that isn't shared
 * between protocols.
 * i.e. no dynamic computed data addresses are used in those cases.
 * As much of the address computation as possible is done at compile time,
 * substituting constants for the \a SocketPage parameter, which is then unused (not evaluated).
 */
/* Deviation MISRAC2012-1 <+2> */
#define TcpIp_Socket_getProtocolDomainConstDescriptorPtr(PROTOCOL, SocketPage) \
  TCPIP_SOCKET_GETPROTOCOLDOMAINDESCRIPTORPTR_##PROTOCOL(Const, SocketPage)

/** \brief Internal specialized macro for the generic
 * \a TcpIp_Socket_getProtocolDomainDescriptorPtr accessor macro,
 * if nothing is known about the protocol (either TCP or UDP).
 * There is no use trying to substitute a constant page here for SingleProtocol/SingleStack
 * configurations as that is already done in \a TcpIp_Socket_getSocketPageConfigEntryPtr,
 * which is better at that - and the substituted SocketPage would be ignored there, anyway.
 */
#define TCPIP_SOCKET_GETPROTOCOLDOMAINDESCRIPTORPTR_ANY(VARIANT, SocketPage)                 \
  TCPIP_IF_UDP_TCP_BOTH_NEITHER(                                                             \
    TCPIP_IP_IF_V4_V6_BOTH(                                                                  \
      TcpIp_Socket_getSocketPage##VARIANT##ConfigEntryPtr(TcpIp_Socket_SocketPage_UDP_IPV4), \
      TcpIp_Socket_getSocketPage##VARIANT##ConfigEntryPtr(TcpIp_Socket_SocketPage_UDP_IPV6), \
      TcpIp_Socket_getSocketPage##VARIANT##ConfigEntryPtr(SocketPage)),                      \
    TCPIP_IP_IF_V4_V6_BOTH(                                                                  \
      TcpIp_Socket_getSocketPage##VARIANT##ConfigEntryPtr(TcpIp_Socket_SocketPage_TCP_IPV4), \
      TcpIp_Socket_getSocketPage##VARIANT##ConfigEntryPtr(TcpIp_Socket_SocketPage_TCP_IPV6), \
      TcpIp_Socket_getSocketPage##VARIANT##ConfigEntryPtr(SocketPage)),                      \
    TcpIp_Socket_getSocketPage##VARIANT##ConfigEntryPtr(SocketPage),                         \
    (NULL_PTR)                                                                    )

/** \brief Diversion/de-specialization macro for UDP. */
#define TCPIP_SOCKET_GETPROTOCOLDOMAINDESCRIPTORPTR_UDP(VARIANT, SocketPage) \
        TCPIP_SOCKET_GETPROTOCOLDOMAINDESCRIPTORPTR_(UDP, VARIANT, (SocketPage))
/** \brief Diversion/de-specialization macro for TCP. */
#define TCPIP_SOCKET_GETPROTOCOLDOMAINDESCRIPTORPTR_TCP(VARIANT, SocketPage) \
        TCPIP_SOCKET_GETPROTOCOLDOMAINDESCRIPTORPTR_(TCP, VARIANT, (SocketPage))

/** \brief Internal de-specialized macro for the generic
 * \a TcpIp_Socket_getProtocolDomainDescriptorPtr accessor macro
 * when the protocol is known as TCP or UDP in DualProtocol configurations.
 * Uses static dispatch for all SingleStack configurations.
 * i.e. no calling through function pointers, no dynamic computed data addresses.
 * As much of the address computation as possible is done at compile time,
 * substituting constants for the page parameter, which is then unused (not evaluated).
 * It falls back to dynamic dispatch only for DualStack configurations.
 */
/* Deviation MISRAC2012-1 <START> */
#define TCPIP_SOCKET_GETPROTOCOLDOMAINDESCRIPTORPTR_(PROTOCOL, VARIANT, SocketPage)                \
  TCPIP_IP_IF_V4_V6_BOTH(                                                                          \
    TcpIp_Socket_getSocketPage##VARIANT##ConfigEntryPtr(TcpIp_Socket_SocketPage_##PROTOCOL##_IPV4),\
    TcpIp_Socket_getSocketPage##VARIANT##ConfigEntryPtr(TcpIp_Socket_SocketPage_##PROTOCOL##_IPV6),\
    TcpIp_Socket_getSocketPage##VARIANT##ConfigEntryPtr(SocketPage))                               \
/* Deviation MISRAC2012-1 <STOP> */


/** \brief Templated helper macro to compute the address of a socket for \a protocolDomainDescriptor
 * and indexed by \a socketIdx.
 *
 * \param PROTOCOL is the template parameter: UDP / TCP / ANY
 * \param protocolDomainDescriptorPtr This parameter is not evaluated for SingleStack
 *  configurations, when the template parameter \a PROTOCOL != ANY
 *  \return The return type is the correct type for the given template parameter \a PROTOCOL.
 */
  /* Deviation MISRAC2012-8 */
#define TcpIp_Socket_getIntSocketPtr(PROTOCOL, protocolDomainConstDescriptorPtr, socketIdx)    \
  /* Deviation MISRAC2012-1 */                                                                 \
  ((P2VAR(TcpIp_Socket_##PROTOCOL##_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA))                 \
    &(((uint8*)(protocolDomainConstDescriptorPtr)->SocketMemory)                               \
              [(protocolDomainConstDescriptorPtr)->socketSize * (socketIdx)] ) )

/** \brief Templated helper macro to compute the address of a socket for \a protocolDomainDescriptor
 * and indexed by \a socketIdx.
 *
 * \param PROTOCOL is the template parameter: UDP / TCP / ANY
 * \param protocolDomainDescriptorPtr This parameter is not evaluated for SingleStack
 *  configurations, when the template parameter \a PROTOCOL != ANY
 *  \return The return type is always \a TcpIp_Socket_Generic_Type.
 */
/* note: must use char*, because sizeof() is defined in multiples of char */
  /* Deviation MISRAC2012-8 */
#define TcpIp_Socket_getIntGenericSocketPtr(PROTOCOL, protocolDomainConstDescriptorPtr, socketIdx) \
  ((P2VAR(TcpIp_Socket_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA))                                  \
    &(((uint8*)(protocolDomainConstDescriptorPtr)->SocketMemory)                                   \
              [(protocolDomainConstDescriptorPtr)->socketSize * (socketIdx)] ) )

/** \brief type forwarding for macro dispatcher. Used whenever the type can be UDP or TCP. */
#define TcpIp_Socket_ANY_Generic_Type TcpIp_Socket_Generic_Type

/* some necessary name-forwarding macros to match the synthesized namespace */
/** \brief Helper macro for static dispatch. */
#define TcpIp_Socket_numSockets_UDP_IPV4 TCPIP_SOCKET_NUM_UDP_IPV4
/** \brief Helper macro for static dispatch. */
#define TcpIp_Socket_numSockets_UDP_IPV6 TCPIP_SOCKET_NUM_UDP_IPV6
/** \brief Helper macro for static dispatch. */
#define TcpIp_Socket_numSockets_TCP_IPV4 TCPIP_SOCKET_NUM_TCP_IPV4
/** \brief Helper macro for static dispatch. */
#define TcpIp_Socket_numSockets_TCP_IPV6 TCPIP_SOCKET_NUM_TCP_IPV6

/** \brief Helper macro for static dispatch. */
#define TcpIp_Socket_socketSize_UDP_IPV4 sizeof(TcpIp_Socket_UDP_IPV4_Type)
/** \brief Helper macro for static dispatch. */
#define TcpIp_Socket_socketSize_UDP_IPV6 sizeof(TcpIp_Socket_UDP_IPV6_Type)
/** \brief Helper macro for static dispatch. */
#define TcpIp_Socket_socketSize_TCP_IPV4 sizeof(TcpIp_Socket_TCP_IPV4_Type)
/** \brief Helper macro for static dispatch. */
#define TcpIp_Socket_socketSize_TCP_IPV6 sizeof(TcpIp_Socket_TCP_IPV6_Type)

/** \brief Helper macro for static dispatch. */
#define TcpIp_Socket_remoteSize_UDP_IPV4 sizeof(TcpIp_SockAddrInetType)
/** \brief Helper macro for static dispatch. */
#define TcpIp_Socket_remoteSize_UDP_IPV6 sizeof(TcpIp_SockAddrInet6Type)
/** \brief Helper macro for static dispatch. */
#define TcpIp_Socket_remoteSize_TCP_IPV4 sizeof(TcpIp_SockAddrInetType)
/** \brief Helper macro for static dispatch. */
#define TcpIp_Socket_remoteSize_TCP_IPV6 sizeof(TcpIp_SockAddrInet6Type)

/*
 *  These should be template inline functions, but in C90, we have to resort to one of:
 *  templated macros (returning nothing)
 *  explicitly specialized inline functions returning non-void
 *  generalized plain static inline (returning non-void) losing the template functionality.
 */

/** \brief inserts a socket \a socketIdx into a list with entry \a nextEntryPtr */
#define TcpIp_Socket_insertSocket(PROTOCOL, SocketPage, nextEntryPtr, socketIdx)       \
{                                                                                      \
  P2VAR(TcpIp_SocketIdType,AUTOMATIC,TCPIP_APPL_DATA) nextEntry_Ptr= (nextEntryPtr);   \
  TcpIp_SocketIdType socket_Idx= (socketIdx);                                          \
  /* remove from local address socket list */                                          \
  CONSTP2VAR(TcpIp_Socket_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socket_Ptr =         \
      TcpIp_Socket_getIntGenericSocketPtr(PROTOCOL, (SocketPage), socket_Idx);         \
  socket_Ptr->aligned.next = *nextEntry_Ptr;                                           \
  *nextEntry_Ptr = socket_Idx;                                                         \
}

/** \brief inserts a linked lists of sockets from \a firstSocketIdx to
          \a lastSocketIdx into a list with entry \a nextEntryPtr */
#define TcpIp_Socket_insertSocketList(PROTOCOL, SocketPage,                                       \
                                      nextEntryPtr, firstSocketIdx, lastSocketIdx)                \
{                                                                                                 \
  P2VAR(TcpIp_SocketIdType,AUTOMATIC,TCPIP_APPL_DATA) nextEntry_Ptr= (nextEntryPtr);              \
  TcpIp_SocketIdType firstSocket_Idx= (firstSocketIdx);                                           \
  TcpIp_SocketIdType lastSocket_Idx= (lastSocketIdx);                                             \
  /* remove from local address socket list */                                                     \
                                                                                                  \
  CONSTP2VAR(TcpIp_Socket_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) lastSocket_Ptr =                \
      TcpIp_Socket_getIntGenericSocketPtr(PROTOCOL, (SocketPage), lastSocket_Idx);                \
  lastSocket_Ptr->aligned.next = *nextEntry_Ptr;                                                  \
  *nextEntry_Ptr = firstSocket_Idx;                                                               \
}

/** \brief removes socket with index contained in nextEntryPtr from the linked list */
#define TcpIp_Socket_removeNextSocket(PROTOCOL, protocolDomaiConstnDescriptorPtr, nextEntryPtr)        \
{                                                                                                      \
  P2VAR(TcpIp_SocketIdType,AUTOMATIC,TCPIP_APPL_DATA) nextEntry_Ptr= (nextEntryPtr);                   \
  /* get pointer to socket */                                                                          \
  P2VAR(TcpIp_Socket_Generic_Type,AUTOMATIC,TCPIP_APPL_DATA) socket_Ptr =                              \
    TcpIp_Socket_getIntGenericSocketPtr(PROTOCOL, (protocolDomaiConstnDescriptorPtr), *nextEntry_Ptr); \
  /* set current next pointer to behind the removed element */                                         \
  *nextEntry_Ptr = socket_Ptr->aligned.next;                                                           \
}

/** \brief returns pointer to a listen socket list (localAddrId or ANY) entry based on
           the \a localAddrId */
#define TcpIp_Socket_autoGetListenSocketListEntryPtr(PROTOCOL, protocolDomainDescriptorPtr,       \
                                                     localAddrId)                                 \
 (((localAddrId) == TCPIP_LOCALADDRID_ANY)                                                        \
   ? &TcpIp_Socket_substituteDescriptorPtr(PROTOCOL, (protocolDomainDescriptorPtr))->anySockets;  \
   : &TcpIp_Socket_dispatch(PROTOCOL, (protocolDomainDescriptorPtr), localAddrListenSockets)[(localAddrId)])

/*==================[type definitions]======================================*/

/* socket list entry pointers */
typedef struct
{
  /** \brief entry point into lists of free sockets */
  TcpIp_SocketIdType freeSockets;

  /** \brief entry point into lists of sockets bound to TCPIP_LOCALADDRID_ANY
   *         for Tcp connections these are listening sockets -
   *         active Tcp sockets cannot be linked to this list
   */
  TcpIp_SocketIdType anySockets;

  /** \brief points to next random port number */
  uint16 nextPort;

} TcpIp_Socket_ProtocolDomain_DescriptorType;

/* socket list entry pointers */
typedef struct
{
  /** \brief Pointer to the array of active sockets' IDs. */
  CONSTP2VAR(TcpIp_SocketIdType, TCPIP_VAR, TCPIP_VAR_CLEARED) localAddrActiveSockets;

  /** \brief Pointer to the array of 'listening' sockets' IDs (TCP only). */
  CONSTP2VAR(TcpIp_SocketIdType, TCPIP_VAR, TCPIP_VAR_CLEARED) localAddrListenSockets;

  /** \brief Pointer to the socket data area's base address. */
  CONSTP2VAR(TcpIp_Socket_Generic_Type, TCPIP_VAR, TCPIP_VAR_CLEARED) SocketMemory;

  /** \brief Maximum number of sockets. */
  uint16 const numSockets;

  /** \brief Size of each socket. */
  uint8  const socketSize;

  /** \brief Size of a remote address (IpV4/IPv6). */
  uint8  const remoteSize;

} TcpIp_Socket_ProtocolDomainConst_DescriptorType;


typedef CONSTP2VAR(TcpIp_Socket_ProtocolDomain_DescriptorType, AUTOMATIC, TCPIP_APPL_DATA)
    TcpIp_Socket_ProtocolDomainDescriptorConstPtrType;

typedef CONSTP2CONST(TcpIp_Socket_ProtocolDomainConst_DescriptorType, AUTOMATIC, TCPIP_APPL_DATA)
    TcpIp_Socket_ProtocolDomainConstDescriptorConstPtrType;


/** \brief data type containing all information for handling Udp communication over IpV4
 *         Note: This type must match the type TcpIp_Socket_UDP_Generic_Type */
typedef TcpIp_Socket_UDP_Generic_Type TcpIp_Socket_UDP_IPV4_Type;

/** \brief data type containing all information for handling Udp communication over IPV6
 *         Note: This type must match the type TcpIp_Socket_UDP_Generic_Type */
typedef TcpIp_Socket_UDP_Generic_Type TcpIp_Socket_UDP_IPV6_Type;

/** \brief data type containing all information for handling Tcp communication over IpV4 */
TcpIp_Socket_TCP_Template_Type(TcpIp_SockAddrInetType,TcpIp_Socket_TCP_IPV4);

/** \brief pointer to data type containing TCP IPV4 sockets */
typedef P2VAR(TcpIp_Socket_TCP_IPV4_Type,TYPEDEF,TCPIP_APPL_DATA) TcpIp_Socket_TCP_IPV4_PtrType;

/** \brief data type containing all information for handling Tcp communication over IPV6 */
TcpIp_Socket_TCP_Template_Type(TcpIp_SockAddrInet6Type,TcpIp_Socket_TCP_IPV6);

/** \brief pointer to data type containing TCP IPV6 sockets */
typedef P2VAR(TcpIp_Socket_TCP_IPV6_Type,TYPEDEF,TCPIP_APPL_DATA) TcpIp_Socket_TCP_IPV6_PtrType;

/** \brief Contains the connection properties relevant for TcpIp_IsConnectionReady */
typedef struct
{
  /** \brief Local addressID */
  TcpIp_LocalAddrIdType localAddrId;
  /** \brief Local port*/
  uint16 localPort;
  /** \brief Pointer to remote address and port */
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr;
  /** \brief Pointer to the storage of remote address and port used in case of TCP */
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrStoragePtr;
  /** \brief Protocol (TCP/UDP) of the connection */
  uint8 protocol;
} TcpIp_Socket_IsConnectionReadyConnectionPropertiesType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#if ( TCPIP_UDP_ENABLED == STD_ON ) || ( TCPIP_TCP_ENABLED == STD_ON )

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief Returns the next free random port */
TS_MOD_PRIV_DECL FUNC(uint16, TCPIP_CODE) TcpIp_Socket_getPort
(
#if (TCPIP_SOCKET_DYNAMIC_DISPATCH == STD_ON)
  uint8 SocketPage
#else /* (TCPIP_SOCKET_DYNAMIC_DISPATCH == STD_OFF) */
  void
#endif /* TCPIP_SOCKET_DYNAMIC_DISPATCH */
);

#if (TCPIP_UDP_ENABLED == STD_ON)

/** \brief initializes the UDP sockets to default vales */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Socket_Udp_init(void);

/** \brief Allocates resources for a new UDP socket */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Socket_Udp_GetSocket
(
  uint8 SocketPage,
  P2VAR(TcpIp_SocketIdType, AUTOMATIC, TCPIP_APPL_DATA) SocketIdPtr,
  uint8 sockOwnerId
);

/** \brief Binds a UDP socket to a local address/port pair */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Socket_Udp_Bind
(
  TcpIp_SocketIdType SocketId,
  TcpIp_LocalAddrIdType LocalAddrId,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) PortPtr
);

/** \brief Closes a UDP socket */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Socket_Udp_Close
(
  TcpIp_SocketIdType SocketId,
  boolean Abort
);

/** \brief Immediately closes UDP sockets bound to a particular local address identifier */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Socket_Udp_closeSockets
(
  TcpIp_LocalAddrIdType localAddrId
);

/* For UDP, closeSockets is an alias for terminateSockets.
 * rather than using a wrapper, we use a macro alias, so closeSockets is called directly. */

/** \brief returns TRUE if a port is already used by a Udp socket */
TS_MOD_PRIV_DECL FUNC(boolean, TCPIP_CODE) TcpIp_Socket_Udp_isPortUsed
(
#if (TCPIP_IP_DUALSTACK == STD_ON)
  uint8 SocketPage,
#endif /* (TCPIP_IP_DUALSTACK == STD_ON) */
  uint16 port
);

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
/** \brief returns E_NOT_OK if a bound Udp socket with the same port and local address already exist */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Socket_Udp_isAlreadyBound
(
#if (TCPIP_IP_DUALSTACK == STD_ON)
  uint8 SocketPage,
#endif /* (TCPIP_IP_DUALSTACK == STD_ON) */
  uint16 Port,
  TcpIp_LocalAddrIdType LocalAddrId,
  TcpIp_SocketIdType socketIdx
);
#endif /* ( TCPIP_DEV_ERROR_DETECT == STD_ON ) */

/** \brief Retrieves the connection properties relevant for TcpIp_IsConnectionReady */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Socket_Udp_getConnectionProperties
(
  TcpIp_SocketIdType SocketId,
  P2VAR(TcpIp_Socket_IsConnectionReadyConnectionPropertiesType, AUTOMATIC, TCPIP_APPL_DATA) conPropertiesPtr
);

#endif /* (TCPIP_UDP_ENABLED == STD_ON) */

#if (TCPIP_TCP_ENABLED == STD_ON)

/** \brief initializes the TCP sockets to default vales */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Socket_Tcp_init(void);

/** \brief Allocates resources for a new TCP socket */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Socket_Tcp_GetSocket
(
  uint8 SocketPage,
  P2VAR(TcpIp_SocketIdType, AUTOMATIC, TCPIP_APPL_DATA) SocketIdPtr,
  uint8 sockOwnerId
);

/** \brief Binds a TCP socket to a local address/port pair */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Socket_Tcp_Bind
(
  TcpIp_SocketIdType SocketId,
  TcpIp_LocalAddrIdType LocalAddrId,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) PortPtr
);

/** \brief Closes a TCP socket */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Socket_Tcp_Close
(
  TcpIp_SocketIdType SocketId,
  boolean Abort
);

/** \brief Requests closing of TCP sockets bound to a particular local address identifier */
TS_MOD_PRIV_DECL FUNC(boolean, TCPIP_CODE) TcpIp_Socket_Tcp_closeSockets
(
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief Immediately closes TCP sockets bound to a particular local address identifier */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Socket_Tcp_terminateSockets
(
  TcpIp_LocalAddrIdType localAddrId,
  boolean informRemoteHost
);

/** \brief returns TRUE if a port is already used by a TCP socket */
TS_MOD_PRIV_DECL FUNC(boolean, TCPIP_CODE) TcpIp_Socket_Tcp_isPortUsed
(
#if (TCPIP_IP_DUALSTACK == STD_ON)
  uint8 SocketPage,
#endif /* (TCPIP_IP_DUALSTACK == STD_ON) */
  uint16 port
);

/** \brief performs periodic tasks of a TCP socket, e.g. timeouts */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Socket_processTcpSockets
(
  void
);

/** \brief performs periodic tasks of a TCP socket, e.g. transmissions */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Socket_processTcpSocketsTx
(
  void
);

/** \brief Retrieves the connection properties relevant for TcpIp_IsConnectionReady */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Socket_Tcp_getConnectionProperties
(
  TcpIp_SocketIdType SocketId,
  P2VAR(TcpIp_Socket_IsConnectionReadyConnectionPropertiesType, AUTOMATIC, TCPIP_APPL_DATA) conPropertiesPtr
);

#endif /* (TCPIP_TCP_ENABLED == STD_ON) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[external constants]====================================*/

#define TCPIP_START_SEC_CONST_UNSPECIFIED
#include <TcpIp_MemMap.h>

/** \brief Constant socket information */
extern CONST(TcpIp_Socket_ProtocolDomainConst_DescriptorType,TCPIP_VAR)
  TcpIp_Socket_ProtocolDomainConst_DescriptorArray[TCPIP_SOCKETPAGE_NUM_VALID_CONFIGS];

#define TCPIP_STOP_SEC_CONST_UNSPECIFIED
#include <TcpIp_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/
/* these data definitions must be public to allow static dispatch, whenever possible */

#define TCPIP_START_SEC_VAR_CLEARED_16
#include <TcpIp_MemMap.h>

#if (TCPIP_SOCKET_NUM_UDP_IPV4 > 0U)
/** \brief entry point into lists of Udp sockets, one list per local address */
extern VAR(TcpIp_SocketIdType,TCPIP_VAR_CLEARED) TcpIp_Socket_localAddrActiveSockets_UDP_IPV4[TCPIP_NUM_LOCALADDRENTRIES];
#endif /* (TCPIP_SOCKET_NUM_UDP_IPV4 > 0U) */

#if (TCPIP_SOCKET_NUM_UDP_IPV6 > 0U)
/** \brief entry point into lists of Udp sockets, one list per local address */
extern VAR(TcpIp_SocketIdType,TCPIP_VAR_CLEARED) TcpIp_Socket_localAddrActiveSockets_UDP_IPV6[TCPIP_NUM_LOCALADDRENTRIES];
#endif /* (TCPIP_SOCKET_NUM_UDP_IPV6 > 0U) */

#if (TCPIP_SOCKET_NUM_TCP_IPV4 > 0U)
/** \brief entry point into lists of active Tcp sockets, one list per local address */
extern VAR(TcpIp_SocketIdType,TCPIP_VAR_CLEARED) TcpIp_Socket_localAddrActiveSockets_TCP_IPV4[TCPIP_NUM_LOCALADDRENTRIES];

/** \brief entry point into lists of listen Tcp sockets, one list per local address */
extern VAR(TcpIp_SocketIdType,TCPIP_VAR_CLEARED) TcpIp_Socket_localAddrListenSockets_TCP_IPV4[TCPIP_NUM_LOCALADDRENTRIES];
#endif /* (TCPIP_SOCKET_NUM_TCP_IPV4 > 0U) */

#if (TCPIP_SOCKET_NUM_TCP_IPV6 > 0U)
/** \brief entry point into lists of active Tcp sockets, one list per local address */
extern VAR(TcpIp_SocketIdType,TCPIP_VAR_CLEARED) TcpIp_Socket_localAddrActiveSockets_TCP_IPV6[TCPIP_NUM_LOCALADDRENTRIES];

/** \brief entry point into lists of listen Tcp sockets, one list per local address */
extern VAR(TcpIp_SocketIdType,TCPIP_VAR_CLEARED) TcpIp_Socket_localAddrListenSockets_TCP_IPV6[TCPIP_NUM_LOCALADDRENTRIES];

#endif /* (TCPIP_SOCKET_NUM_TCP_IPV6 > 0U) */

#define TCPIP_STOP_SEC_VAR_CLEARED_16
#include <TcpIp_MemMap.h>

#define TCPIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

#if (TCPIP_SOCKET_NUM_UDP_IPV4 > 0U)
/** \brief UDP IPV4 socket memory space */
extern VAR(TcpIp_Socket_UDP_IPV4_Type,TCPIP_VAR_CLEARED) TcpIp_Socket_SocketMemory_UDP_IPV4[TcpIp_Socket_numSockets_UDP_IPV4];
#endif /* (TCPIP_SOCKET_NUM_UDP_IPV4 > 0U) */

#if (TCPIP_SOCKET_NUM_UDP_IPV6 > 0U)
/** \brief UDP IPV6 socket memory space */
extern VAR(TcpIp_Socket_UDP_IPV6_Type,TCPIP_VAR_CLEARED) TcpIp_Socket_SocketMemory_UDP_IPV6[TcpIp_Socket_numSockets_UDP_IPV6];
#endif /* (TCPIP_SOCKET_NUM_UDP_IPV6 > 0U) */

#if (TCPIP_SOCKET_NUM_TCP_IPV4 > 0U)
/** \brief TCP IPV4 socket memory space */
extern VAR(TcpIp_Socket_TCP_IPV4_Type,TCPIP_VAR_CLEARED) TcpIp_Socket_SocketMemory_TCP_IPV4[TCPIP_SOCKET_NUM_TCP_IPV4];
#endif /* (TCPIP_SOCKET_NUM_TCP_IPV4 > 0U) */

#if (TCPIP_SOCKET_NUM_TCP_IPV6 > 0U)
/** \brief TCP IPV6 socket memory space */
extern VAR(TcpIp_Socket_TCP_IPV6_Type,TCPIP_VAR_CLEARED) TcpIp_Socket_SocketMemory_TCP_IPV6[TCPIP_SOCKET_NUM_TCP_IPV6];
#endif /* (TCPIP_SOCKET_NUM_TCP_IPV6 > 0U) */

/** \brief Socket page specific constants and data values for dynamic lookup. */
extern VAR(TcpIp_Socket_ProtocolDomain_DescriptorType,TCPIP_VAR)
    TcpIp_Socket_ProtocolDomain_DescriptorArray[TCPIP_SOCKETPAGE_NUM_VALID_CONFIGS];

#define TCPIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

/*==================[internal data]=========================================*/

#endif /* ( TCPIP_UDP_ENABLED == STD_ON ) || ( TCPIP_TCP_ENABLED == STD_ON ) */

#endif /* TCPIP_SOCKET_PRIV_H */

/*==================[end of file]===========================================*/
