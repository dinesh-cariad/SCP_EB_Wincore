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

#ifndef TCPIP_IP_PRIV_H
#define TCPIP_IP_PRIV_H

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
 * MISRAC2012-2) Deviated Rule: 20.7 (required)
 * Expressions resulting from the expansion of macro parameters shall be enclosed in
 * parentheses.
 *
 * Reason:
 * Parentheses defeat the purpose of a compile-time macro: e.g. it could not be used
 * with type definition, where the additional parenthesis are harmful.
 *
 */

/*==================[inclusions]============================================*/

#include <TcpIp_guards.h>   /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Ip_Int.h>       /* module internal headerfile */
#include <TcpIp_Trace.h>        /* debug and trace macros; must be last */

/*==================[macros]================================================*/

/** \brief IP service dispatch macro that supports Single (either IPv4 or IPv6 only) or Dual stack operation */
/* Deviation MISRAC2012-1 <+4> */
#define TCPIP_IP_SERVICEDISPATCH(SERVICE, page) \
  TCPIP_IP_IF_V4_V6_BOTH( TcpIp_IpV4_##SERVICE, \
                          TcpIp_IpV6_##SERVICE, \
                          TcpIp_Ip_FctDispatchArray[(page)].SERVICE##_FctPtr )

/* direct dispatch by macro - no need for function table */
/** \brief Dispatch macro for function \a getRemoteAddrHandle for IpV4, IpV6 and \a TCPIP_IP_DUALSTACK */
#define TCPIP_IP_GETREMOTEADDRHANDLE(page) TCPIP_IP_SERVICEDISPATCH(getRemoteAddrHandle, (page))

/** \brief Dispatch macro for function \a address unlock for IpV4, IpV6 and \a TCPIP_IP_DUALSTACK */
#define TCPIP_IP_UNLOCKADDR(page)                 \
  TCPIP_IP_IF_V4_V6_BOTH( TcpIp_IpV4_ArpUnlock,   \
                          TcpIp_IpV6_unlockEntry,\
                          TcpIp_Ip_FctDispatchArray[(page)].unlockAddr_FctPtr )

/* direct dispatch by macro - no need for function table */
/** \brief Dispatch macro for function \a provideTxBuffer for IpV4, IpV6 and \a TCPIP_IP_DUALSTACK */
#define TCPIP_IP_PROVIDETXBUFFER(page) TCPIP_IP_SERVICEDISPATCH(provideTxBuffer, (page))

/** \brief Dispatch macro for function \a transmit for IpV4, IpV6 and \a TCPIP_IP_DUALSTACK */
#define TCPIP_IP_TRANSMIT(page) TCPIP_IP_SERVICEDISPATCH(transmit, (page))

/** \brief Dispatch macro for function \a updateHeader for IpV4, IpV6 and \a TCPIP_IP_DUALSTACK */
#define TCPIP_IP_UPDATEHEADER(page) TCPIP_IP_SERVICEDISPATCH(updateHeader, (page))

/** \brief Dispatch macro for function \a setRemotePhysAddr for IpV4, IpV6 and \a TCPIP_IP_DUALSTACK */
#define TCPIP_IP_SETREMOTEPHYSADDR(page)         \
  TCPIP_IP_IF_V4_V6_BOTH( TcpIp_IpV4_Arp_setRemotePhysAddr, \
                          TcpIp_Ndp_NCache_setRemotePhysAddr, \
                          TcpIp_Ip_FctDispatchArray[(page)].setRemotePhysAddr_FctPtr )

/** \brief Dispatch macro for function \a getRemotePhysAddr for IpV4, IpV6 and \a TCPIP_IP_DUALSTACK */
#define TCPIP_IP_GETREMOTEPHYSADDR(page)         \
  TCPIP_IP_IF_V4_V6_BOTH( TcpIp_IpV4_Arp_getRemotePhysAddr, \
                          TcpIp_Ndp_NCache_getRemotePhysAddr, \
                          TcpIp_Ip_FctDispatchArray[(page)].getRemotePhysAddr_FctPtr )

/** \brief Dispatch macro for function \a getMaximumPayload for IpV4, IpV6 and \a TCPIP_IP_DUALSTACK */
#define TCPIP_IP_GETMAXIMUMPAYLOAD(page) TCPIP_IP_SERVICEDISPATCH(getMaximumPayload, (page))

/** \brief Dispatch macro for function \a getFragmentSize for IpV4, IpV6 and \a TCPIP_IP_DUALSTACK */
#define TCPIP_IP_GETFRAGMENTSIZE(page) TCPIP_IP_SERVICEDISPATCH(getFragmentSize, (page))

#if (TCPIP_TCP_ENABLED == STD_ON)
/** \brief Dispatch macro for function \a getMinimumPayload for IpV4, IpV6 and \a TCPIP_IP_DUALSTACK */
#  define TCPIP_IP_GETMINIMUMPAYLOAD(page) TCPIP_IP_SERVICEDISPATCH(getMinimumPayload, (page))

/** \brief Dispatch macro for function \a reachConfirmation for IpV4, IpV6 and \a TCPIP_IP_DUALSTACK */
#define TCPIP_IP_REACHCONFIRMATION(page)         \
  TCPIP_IP_IF_V4_V6_BOTH( TcpIp_IpV4_reachConfirmation, \
                          TcpIp_Ndp_NCache_reachConfirmation, \
                          TcpIp_Ip_FctDispatchArray[(page)].reachConfirmation_FctPtr )

#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
/** \brief Dispatch macro for function \a isValidTcpRemoteAddress for IpV4, IpV6 and \a TCPIP_IP_DUALSTACK */
#define TCPIP_IP_ISVALIDTCPREMOTEADDRESS(page) TCPIP_IP_SERVICEDISPATCH(isValidTcpRemoteAddress, (page))
#endif/* (TCPIP_DEV_ERROR_DETECT == STD_ON) */

#endif /* (TCPIP_TCP_ENABLED == STD_ON) */

/** \brief Dispatch macro for function \a getIfState for IpV4, IpV6 and \a TCPIP_IP_DUALSTACK */
#define TCPIP_IP_GETIFSTATE(page) TCPIP_IP_SERVICEDISPATCH(getIfState, (page))

/** \brief Dispatch macro for function \a getIpConfig for IpV4, IpV6 and \a TCPIP_IP_DUALSTACK */
#define TCPIP_IP_GETIPCONFIG(page) TCPIP_IP_SERVICEDISPATCH(getIpConfig, (page))

#if ((TCPIP_TCP_ENABLED == STD_ON) || (TCPIP_UDP_ENABLED == STD_ON) || \
  (TCPIP_ICMPV4_ENABLED == STD_ON) || (TCPIP_ICMPV6_ENABLED == STD_ON))
/** \brief Dispatch macro for function \a getTxLocalAddrId_byRemoteIp for IpV4, IpV6 and \a TCPIP_IP_DUALSTACK */
#  define TCPIP_IP_GETTXLOCALADDRID_BYREMOTEIP(page) TCPIP_IP_SERVICEDISPATCH(getTxLocalAddrId_byRemoteIp, (page))

/** \brief Dispatch macro for function \a getTxLocalAddrId_byLocalAddrId for IpV4, IpV6 and \a TCPIP_IP_DUALSTACK */
#  define TCPIP_IP_GETTXLOCALADDRID_BYLOCALADDRID(page) TCPIP_IP_SERVICEDISPATCH(getTxLocalAddrId_byCtrlIdx, (page))

#endif /* ((TCPIP_TCP_ENABLED == STD_ON) || (TCPIP_UDP_ENABLED == STD_ON)) ||
        (TCPIP_ICMPV4_ENABLED == STD_ON) || (TCPIP_ICMPV6_ENABLED == STD_ON) */
/** \brief Dispatch macro for function \a icmpTransmit for IpV4, IpV6 and \a TCPIP_IP_DUALSTACK */
#define TCPIP_IP_ICMPTRANSMIT(page)  TCPIP_IP_SERVICEDISPATCH(icmpTransmit, (page))
/** \brief redirect generated IF name to actual IF name */
#define TcpIp_IpV6_icmpTransmit(a,b,c,d,e,f) TcpIp_IpV6_Icmp_transmit(a,b,c,d,e,f)

/* direct dispatch by macro - no need for function table */
/** \brief Dispatch macro for function \a calcPartialChecksum for IpV4, IpV6 and \a TCPIP_IP_DUALSTACK */
#define TCPIP_IP_CALCPARTIALCHECKSUM(page) TCPIP_IP_SERVICEDISPATCH(calcPartialChecksum, (page))

/** \brief Dispatch macro for function \a getPhysAddrPtr for IpV4, IpV6 and \a TCPIP_IP_DUALSTACK */
#define TCPIP_IP_GETPHYSADDRPTR(page)         \
  TCPIP_IP_IF_V4_V6_BOTH( TcpIp_IpV4_Arp_getPhysAddrPtr, \
                          TcpIp_Ndp_NCache_getPhysAddrPtr, \
                          TcpIp_Ip_FctDispatchArray[(page)].getPhysAddrPtr_FctPtr )

/** \brief Dispatch macro for function \a isAddrLimBroadcastAllNodesMulticast for IpV4, IpV6 and \a TCPIP_IP_DUALSTACK */
#define TCPIP_IP_ISADDRLIMBROADCASTALLNODESMULTICAST(page)         \
  TCPIP_IP_IF_V4_V6_BOTH( TcpIp_IpV4_isAddrLimBroadcast, \
                          TcpIp_IpV6_isAddrAllNodesMulticast, \
                          TcpIp_Ip_FctDispatchArray[(page)].isAddrLimBroadcastAllNodesMulticast_FctPtr )

/** \brief Dispatch macro for function \a getCtrlAddrRange for IpV4, IpV6 and \a TCPIP_IP_DUALSTACK */
#define TCPIP_IP_GETCTRLADDRRANGE(page) TCPIP_IP_SERVICEDISPATCH(getCtrlAddrRange, (page))

/* direct dispatch by macro - no need for function table */
/** \brief Dispatch macro for function \a getIpAddrParam for IpV4, IpV6 and \a TCPIP_IP_DUALSTACK */
#define TCPIP_IP_GET_IP_ADDR_PARAM(page) TCPIP_IP_SERVICEDISPATCH(getIpAddrParam, (page))
/*==================[type definitions]======================================*/

/* each domain type must support the following functions and provide
   implementations therefore. */

/** \brief function type of version specific TcpIp_Ip_getRemoteAddrHandle() implementation */
typedef P2FUNC(TcpIp_ReturnType, TCPIP_CODE, TcpIp_Ip_getRemoteAddrHandle_FctPtrType)
(
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrHandlePtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) destIpAddrPtr,
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief function type of version specific unlockAddr() implementation */
typedef P2FUNC(void, TCPIP_CODE, TcpIp_Ip_unlockAddr_FctPtrType)
(
  uint16 remoteAddrHandle,
  uint8 ctrlIdx
);

/** \brief function type of version specific TcpIp_Ip_provideTxBuffer() implementation */
typedef P2FUNC(TcpIp_ReturnType, TCPIP_CODE, TcpIp_Ip_provideTxBuffer_FctPtrType)
(
  P2CONST(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameterPtr,
  P2VAR(TcpIp_Ip_EthTxHandleType, AUTOMATIC, TCPIP_APPL_DATA) ethTxHandlePtr,
  uint16 lenByte,
  TcpIp_LocalAddrIdType localAddrId,
  boolean ipFragment
);

/** \brief function type of version specific TcpIp_Ip_transmit() implementation */
typedef P2FUNC(TcpIp_ReturnType, TCPIP_CODE, TcpIp_Ip_transmit_FctPtrType)
(
  P2CONST(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameterPtr,
  P2VAR(TcpIp_Ip_Frag_TxHeaderType, AUTOMATIC, TCPIP_APPL_DATA) fragHdrPtr,
  P2VAR(TcpIp_Ip_EthTxHandleType, AUTOMATIC, TCPIP_APPL_DATA) ethTxHandlePtr,
  uint16 remoteAddrHandle,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) destIpPtr,
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief function type of version specific TcpIp_Ip_updateHeader() implementation */
typedef P2FUNC(TcpIp_ReturnType, TCPIP_CODE, TcpIp_Ip_updateHeader_FctPtrType)
(
  P2VAR(TcpIp_Ip_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA) updateHeaderFunctorPtr,
  P2VAR(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameterPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) destIpPtr,
  P2VAR(TcpIp_Ip_EthTxHandleType, AUTOMATIC, TCPIP_APPL_DATA) ethTxHandlePtr,
  TcpIp_LocalAddrIdType localAddrId,
  uint16 payloadSum,
  uint16 ipPayloadLength,
  uint16 frgCnt
);

/** \brief function type of version specific TcpIp_Ip_icmpTransmit() implementation */
typedef P2FUNC(TcpIp_ReturnType, TCPIP_CODE, TcpIp_Ip_icmpTransmit_FctPtrType)
(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 lenByte,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) destIpPtr,
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(TcpIp_Ip_Icmp_ParameterType, AUTOMATIC, TCPIP_APPL_DATA) icmpParameterPtr,
  boolean truncate
);

/** \brief function type of version specific TcpIp_Ip_getMaximumPayload() implementation */
typedef P2FUNC(Std_ReturnType, TCPIP_CODE, TcpIp_Ip_getMaximumPayload_FctPtrType)
(
  uint8 ipSecSaCommonIdx,
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) maxPayloadPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) additionalHdrSizePtr
);

/** \brief function type of version specific TcpIp_Ip_getFragmentSize() implementation */
typedef P2FUNC(uint16, TCPIP_CODE, TcpIp_Ip_getFragmentSize_FctPtrType)
(
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief function type of version specific TcpIp_Ip_getMinimumPayload() implementation */
typedef P2FUNC(uint16, TCPIP_CODE, TcpIp_Ip_getMinimumPayload_FctPtrType)
(
  void
);

/** \brief function type of version specific TcpIp_Ip_getIfState() implementation */
typedef P2FUNC(TcpIp_StateType, TCPIP_CODE, TcpIp_Ip_getIfState_FctPtrType)
(
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief function type of version specific TcpIp_Ip_getIpConfig() implementation */
typedef P2FUNC(void, TCPIP_CODE, TcpIp_Ip_getIpConfig_FctPtrType)
(
  TcpIp_LocalAddrIdType LocalAddrId,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) IpAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) NetmaskPtr,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) DefaultRouterPtr
);

/** \brief function type of version specific TcpIp_Ip_getTxLocalAddrId_byRemoteIp() implementation */
typedef P2FUNC(TcpIp_LocalAddrIdType, TCPIP_CODE, TcpIp_Ip_getTxLocalAddrId_byRemoteIp_FctPtrType)
(
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr
);

/** \brief function type of version specific TcpIp_Ip_getTxLocalAddrId_byCtrlIdxd() implementation */
typedef P2FUNC(TcpIp_LocalAddrIdType, TCPIP_CODE, TcpIp_Ip_getTxLocalAddrId_byCtrlIdx_FctPtrType)
(
  uint8 ctrlIdx,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteIpAddrPtr
);

/** \brief function type of version specific TcpIp_SetRemotePhysAddr() implementation */
typedef P2FUNC(TcpIp_ReturnType, TCPIP_CODE, TcpIp_Ip_setRemotePhysAddr_FctPtrType)
(
  uint8 CtrlIdx,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) IpAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr,
  uint8 State
);

/** \brief function type of version specific TcpIp_GetRemotePhysAddr() implementation */
typedef P2FUNC(TcpIp_ReturnType, TCPIP_CODE, TcpIp_Ip_getRemotePhysAddr_FctPtrType)
(
  uint8 CtrlIdx,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) IpAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr,
  boolean initRes
);

/** \brief function type of version specific TcpIp_Ip_reachConfirmation() implementation */
typedef P2FUNC(void, TCPIP_CODE, TcpIp_Ip_reachConfirmation_FctPtrType)
(
  uint8 CtrlIdx,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr
);

/** \brief function type of version specific TcpIp_Ip_isValidTcpRemoteAddress() implementation */
typedef P2FUNC(boolean, TCPIP_CODE, TcpIp_Ip_isValidTcpRemoteAddress_FctPtrType)
(
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RemoteAddrPtr
);

/** \brief function type of version specific TcpIp_Ip_calcPartialChecksum() implementation */
typedef P2FUNC(uint16, TCPIP_CODE, TcpIp_Ip_calcPartialChecksum_FctPtrType)
(
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) srcAddrPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) destAddrPtr,
  uint16 lenByte,
  uint8 protocol
);

/** \brief function type of version specific TcpIp_Ip_getPhysAddrPtr() implementation */
/* Deviation MISRAC2012-2 */
typedef P2FUNC(P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA), TCPIP_CODE, TcpIp_Ip_getPhysAddrPtr_FctPtrType)
(
  uint16 remoteAddrHandle,
  uint8 ctrlIdx
);

/** \brief function type of version specific TcpIp_Ip_isAddrLimBroadcastAllNodesMulticast() implementation */
/* Deviation MISRAC2012-2 */
typedef P2FUNC(boolean, TCPIP_CODE, TcpIp_Ip_isAddrLimBroadcastAllNodesMulticast_FctPtrType)
(
  uint8 ctrlIdx,
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) localIpAddrPtr
);

/** \brief function type of version specific TcpIp_Ip_getCtrlAddrRange() implementation */
/* Deviation MISRAC2012-2 */
typedef P2FUNC(void, TCPIP_CODE, TcpIp_Ip_getCtrlAddrRange_FctPtrType)
(
  uint8 ctrlIdx,
  P2VAR(TcpIp_LocalAddrIdType, AUTOMATIC, TCPIP_APPL_DATA) startIndexPtr,
  P2VAR(TcpIp_LocalAddrIdType, AUTOMATIC, TCPIP_APPL_DATA) endIndexPtr
);

#if (TCPIP_IP_DUALSTACK == STD_ON)
/** \brief Set of function pointers for dual stack ip operations */
typedef struct
{
  TcpIp_Ip_provideTxBuffer_FctPtrType provideTxBuffer_FctPtr;
  TcpIp_Ip_transmit_FctPtrType transmit_FctPtr;
  TcpIp_Ip_getMaximumPayload_FctPtrType getMaximumPayload_FctPtr;
#if ( TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF )
  TcpIp_Ip_getFragmentSize_FctPtrType getFragmentSize_FctPtr;
#endif /* ( TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF ) */
#if (TCPIP_TCP_ENABLED == STD_ON)
  TcpIp_Ip_getMinimumPayload_FctPtrType getMinimumPayload_FctPtr;
  TcpIp_Ip_reachConfirmation_FctPtrType reachConfirmation_FctPtr;
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  TcpIp_Ip_isValidTcpRemoteAddress_FctPtrType isValidTcpRemoteAddress_FctPtr;
#endif /* (TCPIP_DEV_ERROR_DETECT == STD_ON) */
#endif /* (TCPIP_TCP_ENABLED == STD_ON) */
  TcpIp_Ip_getIfState_FctPtrType getIfState_FctPtr;
  TcpIp_Ip_getIpConfig_FctPtrType getIpConfig_FctPtr;
#if ((TCPIP_TCP_ENABLED == STD_ON) || (TCPIP_UDP_ENABLED == STD_ON))
  TcpIp_Ip_getTxLocalAddrId_byRemoteIp_FctPtrType getTxLocalAddrId_byRemoteIp_FctPtr;
  TcpIp_Ip_getTxLocalAddrId_byCtrlIdx_FctPtrType getTxLocalAddrId_byCtrlIdx_FctPtr;
#endif /* ((TCPIP_TCP_ENABLED == STD_ON) || (TCPIP_UDP_ENABLED == STD_ON)) */
  TcpIp_Ip_icmpTransmit_FctPtrType icmpTransmit_FctPtr;
  TcpIp_Ip_setRemotePhysAddr_FctPtrType setRemotePhysAddr_FctPtr;
  TcpIp_Ip_getRemotePhysAddr_FctPtrType getRemotePhysAddr_FctPtr;
  TcpIp_Ip_calcPartialChecksum_FctPtrType calcPartialChecksum_FctPtr;
  TcpIp_Ip_getRemoteAddrHandle_FctPtrType getRemoteAddrHandle_FctPtr;
  TcpIp_Ip_unlockAddr_FctPtrType unlockAddr_FctPtr;
  TcpIp_Ip_updateHeader_FctPtrType updateHeader_FctPtr;
#if (TCPIP_UDP_BROADCAST_RECEPTION_ENABLED == STD_ON)
  TcpIp_Ip_isAddrLimBroadcastAllNodesMulticast_FctPtrType isAddrLimBroadcastAllNodesMulticast_FctPtr;
  TcpIp_Ip_getCtrlAddrRange_FctPtrType getCtrlAddrRange_FctPtr;
#endif /* (TCPIP_UDP_BROADCAST_RECEPTION_ENABLED == STD_ON) */
  TcpIp_Ip_getPhysAddrPtr_FctPtrType getPhysAddrPtr_FctPtr;
} TcpIp_Ip_FctDispatchType;
#endif /* (TCPIP_IP_DUALSTACK == STD_ON) */

/*==================[external function declarations]========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>


#if (TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON)
/** \brief Initalize IP reassembly
 *
 * This function initialize the reassembly runtime data and buffers
 *
 * \param[in] TcpIp_Ip_Frag_MemReserved  configured memory reserved for fragmentation
 * \param[in] startOffset                start of the reassembly runtime data and buffers
 *                                       in the reserved memory
 */
TS_MOD_PRIV_DECL FUNC(uint32, TCPIP_CODE) TcpIp_Ip_Reass_init
(
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_Ip_Frag_MemReserved,
  uint32 startOffset
);

/** \brief Triggers periodic IP functions
 *
 * This function handles reassembly buffer timeouts.
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Ip_Frag_mainFunction(void);

#endif /* (TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON) */

#if (TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF)

/** \brief Initalize IP fragmentation
 *
 * This function initialize the fragmentation runtime data and buffers
 *
 * \param[in] TcpIp_Ip_Frag_MemReserved  configured memory reserved for fragmentation
 * \param[in] startOffset                start of the fragmentation runtime data and buffers
 *                                       in the reserved memory
 */
TS_MOD_PRIV_DECL FUNC(uint32, TCPIP_CODE) TcpIp_Ip_Frag_init
(
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_Ip_Frag_MemReserved,
  uint32 startOffset
);


#endif /* (TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF)*/

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

#if (TCPIP_IP_DUALSTACK == STD_ON)
#define TCPIP_START_SEC_CONST_UNSPECIFIED
#include <TcpIp_MemMap.h>

/** \brief Displatch table for \a TCPIP_IP_DUALSTACK operation. */
extern CONST(TcpIp_Ip_FctDispatchType,TCPIP_CONST) TcpIp_Ip_FctDispatchArray[TCPIP_IP_NUMDOMAINS];

#define TCPIP_STOP_SEC_CONST_UNSPECIFIED
#include <TcpIp_MemMap.h>
#endif /* (TCPIP_IP_DUALSTACK == STD_ON) */

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /* TCPIP_IP_PRIV_H */
