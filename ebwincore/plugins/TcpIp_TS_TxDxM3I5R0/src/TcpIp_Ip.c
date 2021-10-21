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
 * MISRAC2012-2) Deviated Rule: 11.3 (required)
 * A cast shall not be performed between a pointer to object type and a pointer to a different
 * object type.
 *
 * Reason:
 * 1)AUTOSAR defines a generic type which dependent on a member of the generic type
 * implements an concrete type. Thus for usage of this concrete type, a cast is necessary.
 * 2)Aligning for better memory subsystem performance can only be done by casting
 * the pointer to different alignment after the alignment checks.
 *
 *
 * MISRAC2012-3) Deviated Rule: 20.7 (required)
 * Expressions resulting from the expansion of macro parameters shall be enclosed in
 * parentheses.
 *
 * Reason:
 * Parentheses defeat the purpose of a compile-time-if macro: e.g. it could not be used
 * within a parameter list, where the additional parenthesis are harmful.
 *
 */

/*==================[inclusions]============================================*/
#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_IpV4_Int.h>     /* used SW-unit interface file */
#include <TcpIp_IpV6_Int.h>     /* used SW-unit interface file */
#include <TcpIp_Ndp_Int.h>      /* used SW-unit interface file */
#include <TcpIp_IpAddrM_Int.h>  /* used SW-unit interface file */
#include <TcpIp_Ip_Priv.h>      /* own SW-unit interface file */
#include <TcpIp_IpSecDb_Int.h>  /* used SW-unit interface file */
#include <TcpIp_IpSec_Int.h>    /* used SW-unit interface file */
#include <TcpIp_Trace.h>        /* debug and trace macros; must be last */
/*==================[macros]================================================*/

/** \brief TcpIp_SetRemotePhysAddr() service ID */
#define TCPIP_SID_SETREMOTEPHYSADDR                               0xF0U
/** \brief TcpIp_GetRemotePhysAddr() service ID */
#define TCPIP_SID_GETREMOTEPHYSADDR                               0x16U

/** \brief Compile time if-conversion:
 *  evaluating to
 *  the first parameter \a then, if \a TCPIP_ICMPV4_ENABLED is \a STD_ON,
 *  the second parameter \a else, if \a TCPIP_ICMPV4_ENABLED is \a STD_OFF.
 */
#if (TCPIP_ICMPV4_ENABLED == STD_ON)
#  define TCPIP_IP_IF_ICMPV4(then, else) (then)
#else
#  define TCPIP_IP_IF_ICMPV4(then, else) (else)
#endif

/*==================[type definitions]======================================*/

/*==================[internal data]=========================================*/

#if ( TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON ) || ( TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF )
#define TCPIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

/* Coding violation produces false positive */
/** \brief The raw memory reserved for allocation of reassembly and transmit buffers. */
STATIC TS_DefMaxAlignedByteArray(TcpIp_Ip_Frag_MemReservedMaxAligned, TCPIP, APPL_DATA, TCPIP_IP_FRAGMENT_MEMRESERVED);

#define TCPIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>
#endif /*( TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON ) || ( TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF )*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

#if (TCPIP_IP_DUALSTACK == STD_ON)
#define TCPIP_START_SEC_CONST_UNSPECIFIED
#include <TcpIp_MemMap.h>

CONST(TcpIp_Ip_FctDispatchType,TCPIP_CONST) TcpIp_Ip_FctDispatchArray[TCPIP_IP_NUMDOMAINS] =
{
  {  /* BEGIN IPv4 table */
    &TcpIp_IpV4_provideTxBuffer,
    &TcpIp_IpV4_transmit,
    &TcpIp_IpV4_getMaximumPayload,
#if ( TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF )
    &TcpIp_IpV4_getFragmentSize,
#endif /* ( TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF ) */
#if (TCPIP_TCP_ENABLED == STD_ON)
    &TcpIp_IpV4_getMinimumPayload,
    &TcpIp_IpV4_reachConfirmation,
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
    &TcpIp_IpV4_isValidTcpRemoteAddress,
#endif/* (TCPIP_DEV_ERROR_DETECT == STD_ON) */
#endif /* (TCPIP_TCP_ENABLED == STD_ON) */
    &TcpIp_IpV4_getIfState,
    &TcpIp_IpV4_getIpConfig,
#if ((TCPIP_TCP_ENABLED == STD_ON) || (TCPIP_UDP_ENABLED == STD_ON))
    &TcpIp_IpV4_getTxLocalAddrId_byRemoteIp,
    &TcpIp_IpV4_getTxLocalAddrId_byCtrlIdx,
#endif /* ((TCPIP_TCP_ENABLED == STD_ON) || (TCPIP_UDP_ENABLED == STD_ON)) */
    TCPIP_IP_IF_ICMPV4(&TcpIp_IpV4_icmpTransmit, NULL_PTR),
    &TcpIp_IpV4_Arp_setRemotePhysAddr,
    &TcpIp_IpV4_Arp_getRemotePhysAddr,
    &TcpIp_IpV4_calcPartialChecksum,
    &TcpIp_IpV4_getRemoteAddrHandle,
    &TcpIp_IpV4_ArpUnlock,
    &TcpIp_IpV4_updateHeader,
#if (TCPIP_UDP_BROADCAST_RECEPTION_ENABLED == STD_ON)
    &TcpIp_IpV4_isAddrLimBroadcast,
    &TcpIp_IpV4_getCtrlAddrRange,
#endif /* (TCPIP_UDP_BROADCAST_RECEPTION_ENABLED == STD_ON) */
    &TcpIp_IpV4_Arp_getPhysAddrPtr
  }, /* END IPv4 table */
  {  /* BEGIN IPv6 table */
    &TcpIp_IpV6_provideTxBuffer,
    &TcpIp_IpV6_transmit,
    &TcpIp_IpV6_getMaximumPayload,
#if ( TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF )
    &TcpIp_IpV6_getFragmentSize,
#endif /* ( TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF ) */
#if (TCPIP_TCP_ENABLED == STD_ON)
    &TcpIp_IpV6_getMinimumPayload,
    &TcpIp_Ndp_NCache_reachConfirmation,
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
    &TcpIp_IpV6_isValidTcpRemoteAddress,
#endif/* (TCPIP_DEV_ERROR_DETECT == STD_ON) */
#endif /* (TCPIP_TCP_ENABLED == STD_ON) */
    &TcpIp_IpV6_getIfState,
    &TcpIp_IpV6_getIpConfig,
#if ((TCPIP_TCP_ENABLED == STD_ON) || (TCPIP_UDP_ENABLED == STD_ON))
    &TcpIp_IpV6_getTxLocalAddrId_byRemoteIp,
    &TcpIp_IpV6_getTxLocalAddrId_byCtrlIdx,
#endif /* ((TCPIP_TCP_ENABLED == STD_ON) || (TCPIP_UDP_ENABLED == STD_ON)) */
    &TcpIp_IpV6_Icmp_transmit,
    &TcpIp_Ndp_NCache_setRemotePhysAddr,
    &TcpIp_Ndp_NCache_getRemotePhysAddr,
    &TcpIp_IpV6_calcPartialChecksum,
    &TcpIp_IpV6_getRemoteAddrHandle,
    &TcpIp_IpV6_unlockEntry,
    &TcpIp_IpV6_updateHeader,
#if (TCPIP_UDP_BROADCAST_RECEPTION_ENABLED == STD_ON)
    &TcpIp_IpV6_isAddrAllNodesMulticast,
    &TcpIp_IpV6_getCtrlAddrRange,
#endif /* (TCPIP_UDP_BROADCAST_RECEPTION_ENABLED == STD_ON) */
    &TcpIp_Ndp_NCache_getPhysAddrPtr
  }  /* END IPv6 table */
};

#define TCPIP_STOP_SEC_CONST_UNSPECIFIED
#include <TcpIp_MemMap.h>
#endif /* TCPIP_IP_DUALSTACK==STD_ON */

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_SetRemotePhysAddr
(
  uint8 CtrlIdx,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) IpAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr,
  uint8 State
)
{
  TcpIp_ReturnType retval = TCPIP_E_NOT_OK;

  DBG_TCPIP_SETREMOTEPHYSADDR_ENTRY(CtrlIdx,IpAddrPtr,PhysAddrPtr,State);

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
  if( TCPIP_UNINIT )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_SETREMOTEPHYSADDR, TCPIP_E_NOTINIT);
  }
  else if( (CtrlIdx > TcpIp_PBcfgPtr->maxCtrlIdx) ||
            (TCPIP_CFG_TYPE(uint8,ctrlMapping,CtrlIdx) == TCPIP_CTRLIDX_INVALID)
         )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_SETREMOTEPHYSADDR, TCPIP_E_INV_ARG);
  }
  else if( IpAddrPtr == NULL_PTR)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_SETREMOTEPHYSADDR, TCPIP_EB_E_PARAM_POINTER);
  }
  else if( PhysAddrPtr == NULL_PTR)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_SETREMOTEPHYSADDR, TCPIP_EB_E_PARAM_POINTER);
  }
  else if ( TCPIP_IP_IF_V4_V6_BOTH
              (
               (IpAddrPtr->domain != TCPIP_AF_INET),
               (IpAddrPtr->domain != TCPIP_AF_INET6),
               ((IpAddrPtr->domain != TCPIP_AF_INET) && (IpAddrPtr->domain != TCPIP_AF_INET6))
              )
          )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_SETREMOTEPHYSADDR, TCPIP_E_AFNOSUPPORT);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
    retval =
     /* Deviation MISRAC2012-3 */
     TCPIP_IP_SETREMOTEPHYSADDR(TCPIP_IP_DOMAIN_GET(IpAddrPtr->domain))
       (
        (TCPIP_CFG_TYPE(uint8,ctrlMapping,CtrlIdx)),
        IpAddrPtr,
        PhysAddrPtr,
        State
        );

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
    if( retval == TCPIP_E_NOT_PERMITTED )
    {
      TCPIP_REPORT_ERROR(TCPIP_SID_SETREMOTEPHYSADDR, TCPIP_E_INV_ARG);
      retval = TCPIP_E_NOT_OK;
    }
#endif /* ( TCPIP_DEV_ERROR_DETECT == STD_ON ) */
  }

  DBG_TCPIP_SETREMOTEPHYSADDR_EXIT(retval,CtrlIdx,IpAddrPtr,PhysAddrPtr,State);

  return retval;
}

FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_GetRemotePhysAddr
(
  uint8 CtrlIdx,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) IpAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr,
  boolean initRes
)
{
  TcpIp_ReturnType retval = TCPIP_E_NOT_OK;

  DBG_TCPIP_GETREMOTEPHYSADDR_ENTRY(CtrlIdx,IpAddrPtr,PhysAddrPtr,initRes);

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
  if( TCPIP_UNINIT )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_GETREMOTEPHYSADDR, TCPIP_E_NOTINIT);
  }
  else if( (CtrlIdx > TcpIp_PBcfgPtr->maxCtrlIdx) ||
           (TCPIP_CFG_TYPE(uint8,ctrlMapping,CtrlIdx) == TCPIP_CTRLIDX_INVALID)
         )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_GETREMOTEPHYSADDR, TCPIP_E_INV_ARG);
  }
  else if( IpAddrPtr == NULL_PTR)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_GETREMOTEPHYSADDR, TCPIP_EB_E_PARAM_POINTER);
  }
  else if( PhysAddrPtr == NULL_PTR)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_GETREMOTEPHYSADDR, TCPIP_EB_E_PARAM_POINTER);
  }
  else if ( TCPIP_IP_IF_V4_V6_BOTH
              (
               (IpAddrPtr->domain != TCPIP_AF_INET),
               (IpAddrPtr->domain != TCPIP_AF_INET6),
               ((IpAddrPtr->domain != TCPIP_AF_INET) && (IpAddrPtr->domain != TCPIP_AF_INET6))
              )
          )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_GETREMOTEPHYSADDR, TCPIP_E_AFNOSUPPORT);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
    retval =
     /* Deviation MISRAC2012-3 */
     TCPIP_IP_GETREMOTEPHYSADDR(TCPIP_IP_DOMAIN_GET(IpAddrPtr->domain))
       (
        (TCPIP_CFG_TYPE(uint8,ctrlMapping,CtrlIdx)),
        IpAddrPtr,
        PhysAddrPtr,
        initRes
       );
  }

  DBG_TCPIP_GETREMOTEPHYSADDR_EXIT(retval,CtrlIdx,IpAddrPtr,PhysAddrPtr,initRes);

  return retval;
}

#if (TCPIP_ICMPV4_ENABLED == STD_ON) || (TCPIP_ICMPV6_ENABLED == STD_ON)
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IcmpTransmit
(
  TcpIp_LocalAddrIdType LocalIpAddrId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RemoteAddrPtr,
  uint8 Ttl,
  uint8 Type,
  uint8 Code,
  uint16 DataLength,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr
)
{
  Std_ReturnType retval = E_NOT_OK;

  DBG_TCPIP_ICMPTRANSMIT_ENTRY(LocalIpAddrId,RemoteAddrPtr,Ttl,Type,Code,DataLength,DataPtr);

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
  if( TCPIP_UNINIT )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_ICMPTRANSMIT, TCPIP_E_NOTINIT);
  }
  else if(TcpIp_IpAddrM_checkConcreteLocalAddr(LocalIpAddrId) != E_OK)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_ICMPTRANSMIT, TCPIP_E_INV_ARG);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
#if TCPIP_IP_DUALSTACK==STD_ON
    const TcpIp_IpAddrM_AddrInfoType addrInfo = TcpIp_IpAddrM_getLocalAddrInfo(LocalIpAddrId);
    const uint8 page = TCPIP_IPADDRM_ADDRDOMAIN_GET(addrInfo);
#endif /* TCPIP_IP_DUALSTACK==STD_ON */

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
    if( RemoteAddrPtr == NULL_PTR)
    {
      TCPIP_REPORT_ERROR(TCPIP_SID_ICMPTRANSMIT, TCPIP_EB_E_PARAM_POINTER);
    }
    else if ( TCPIP_IP_IF_V4_V6_BOTH( (RemoteAddrPtr->domain != TCPIP_AF_INET),
                                      (RemoteAddrPtr->domain != TCPIP_AF_INET6),
                                      (TcpIp_IpAddrM_checkIpAddr(RemoteAddrPtr, addrInfo) != E_OK )
                                    )
            )
    {
      TCPIP_REPORT_ERROR(TCPIP_SID_ICMPTRANSMIT, TCPIP_E_AFNOSUPPORT);
    }
    else if( DataPtr == NULL_PTR)
    {
      TCPIP_REPORT_ERROR(TCPIP_SID_ICMPTRANSMIT, TCPIP_EB_E_PARAM_POINTER);
    }
    else
#endif /* ( TCPIP_DEV_ERROR_DETECT == STD_ON ) */
    {
      const uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(LocalIpAddrId);
      TcpIp_LocalAddrIdType unicastLocalAddrId;
      TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);
      unicastLocalAddrId =
          /* Deviation MISRAC2012-3 */
          TCPIP_IP_GETTXLOCALADDRID_BYLOCALADDRID
              (page)
              (
               ctrlIdx,
               RemoteAddrPtr
              );

      if(unicastLocalAddrId != TCPIP_LOCALADDRID_INVALID)
      {
        TcpIp_Ip_Icmp_ParameterType icmpParameter;

        icmpParameter.code = Code;
        icmpParameter.hopLimit = Ttl;
        icmpParameter.type = Type;
        icmpParameter.specificValue = NULL_PTR;

        /* Deviation MISRAC2012-3 */
        retval = TCPIP_IP_ICMPTRANSMIT(page)
                     (
                      DataPtr,
                      DataLength,
                      RemoteAddrPtr,
                      unicastLocalAddrId,
                      &icmpParameter,
                      FALSE);

        if(retval == TCPIP_OK)
        {
          retval = E_OK;
        }
        else if(retval == TCPIP_E_MSGSIZE)
        {
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
          TCPIP_REPORT_ERROR(TCPIP_SID_ICMPTRANSMIT, TCPIP_E_MSGSIZE);
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
          retval= E_NOT_OK;
        }
        else
        {
          retval = E_NOT_OK;
        }
      }
    }
  }
  DBG_TCPIP_ICMPTRANSMIT_EXIT(retval,LocalIpAddrId,RemoteAddrPtr,Ttl,Type,Code,DataLength,DataPtr);

  return retval;
}

# if (TCPIP_ICMPTRANSMITERROR_API == STD_ON)
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IcmpTransmitError
(
  uint8 CtrlIdx,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RemoteAddrPtr,
  uint8 Ttl,
  uint8 Type,
  uint8 Code,
  uint16 DataLength,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) SpecificValue
)
{
  Std_ReturnType retval = E_NOT_OK;

  DBG_TCPIP_ICMPTRANSMITERROR_ENTRY(CtrlIdx,RemoteAddrPtr,Ttl,Type,Code,DataLength,DataPtr,SpecificValue);

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
  if( TCPIP_UNINIT )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_ICMPTRANSMITERROR, TCPIP_E_NOTINIT);
  }
  else if(RemoteAddrPtr == NULL_PTR)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_ICMPTRANSMITERROR, TCPIP_EB_E_PARAM_POINTER);
  }
  else if( TCPIP_IP_IF_V4_V6_BOTH( (RemoteAddrPtr->domain != TCPIP_AF_INET),
                                   (RemoteAddrPtr->domain != TCPIP_AF_INET6),
                                   ((RemoteAddrPtr->domain != TCPIP_AF_INET) || (RemoteAddrPtr->domain != TCPIP_AF_INET6))
                                 )
         )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_ICMPTRANSMITERROR, TCPIP_E_AFNOSUPPORT);
  }
  else if(DataPtr == NULL_PTR)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_ICMPTRANSMITERROR, TCPIP_EB_E_PARAM_POINTER);
  }
  else if((CtrlIdx > TcpIp_PBcfgPtr->maxCtrlIdx) ||
         (TCPIP_CFG_TYPE(uint8,ctrlMapping,CtrlIdx) == TCPIP_CTRLIDX_INVALID))
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_ICMPTRANSMITERROR, TCPIP_E_INV_ARG);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
#if ( TCPIP_IP_DUALSTACK == STD_ON )
    const uint8 page = TCPIP_IP_DOMAIN_GET(RemoteAddrPtr->domain);
#endif /* TCPIP_IP_DUALSTACK==STD_ON */

    TcpIp_LocalAddrIdType unicastLocalAddrId;

    unicastLocalAddrId =
        /* Deviation MISRAC2012-3 */
        TCPIP_IP_GETTXLOCALADDRID_BYLOCALADDRID
            (page)
            (
             TCPIP_CFG_TYPE(uint8,ctrlMapping,CtrlIdx),
             RemoteAddrPtr
            );

    if(unicastLocalAddrId != TCPIP_LOCALADDRID_INVALID)
    {
      TcpIp_Ip_Icmp_ParameterType icmpParameter;

      icmpParameter.code = Code;
      icmpParameter.hopLimit = Ttl;
      icmpParameter.type = Type;
      icmpParameter.specificValue = SpecificValue;

      /* Deviation MISRAC2012-3 */
      retval = TCPIP_IP_ICMPTRANSMIT(page)
                   (
                    DataPtr,
                    DataLength,
                    RemoteAddrPtr,
                    unicastLocalAddrId,
                    &icmpParameter,
                    FALSE);

      if(retval == TCPIP_OK)
      {
        retval = E_OK;
      }
      else if(retval == TCPIP_E_MSGSIZE)
      {
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
        TCPIP_REPORT_ERROR(TCPIP_SID_ICMPTRANSMITERROR, TCPIP_E_MSGSIZE);
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
        retval= E_NOT_OK;
      }
      else
      {
        retval = E_NOT_OK;
      }
    }
  }

  DBG_TCPIP_ICMPTRANSMITERROR_EXIT(retval,CtrlIdx,RemoteAddrPtr,Ttl,Type,Code,DataLength,DataPtr,SpecificValue);

  return retval;
}
# endif /* (TCPIP_ICMPTRANSMITERROR_API == STD_ON) */
#endif /* (TCPIP_ICMPV4_ENABLED == STD_ON) || (TCPIP_ICMPV6_ENABLED == STD_ON) */


FUNC(void, TCPIP_CODE) TcpIp_RxIndication
(
  uint8                                    CtrlIdx,
  Eth_FrameType                            FrameType,
  boolean                                  IsBroadcast,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16                                   LenByte
)
{

#if (TCPIP_RX_PACKET_DROP_FCT_ENABLED == STD_ON)
  TcpIp_Ip_RxReturnType result = TCPIP_IP_RX_NOT_OK;
#endif /* TCPIP_RX_PACKET_DROP_FCT_ENABLED == STD_ON */

  DBG_TCPIP_RXINDICATION_ENTRY(CtrlIdx,FrameType,IsBroadcast,PhysAddrPtr,DataPtr,LenByte);

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
  if( TCPIP_UNINIT )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_RXINDICATION, TCPIP_E_NOTINIT);
  }
  else if( DataPtr == NULL_PTR )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_RXINDICATION, TCPIP_EB_E_PARAM_POINTER);
  }
  else if( PhysAddrPtr == NULL_PTR )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_RXINDICATION, TCPIP_EB_E_PARAM_POINTER);
  }
  else
#endif
  {
    if ( (CtrlIdx <= TcpIp_PBcfgPtr->maxCtrlIdx) &&
         (TCPIP_CFG_TYPE(uint8,ctrlMapping,CtrlIdx) != TCPIP_CTRLIDX_INVALID)
       )
    {
#if (TCPIP_IPV4_ENABLED == STD_OFF)
      TS_PARAM_UNUSED(PhysAddrPtr);
#endif /* (TCPIP_IPV4_ENABLED == STD_OFF) */

      if(LenByte > (TCPIP_CFG(ctrl,(TCPIP_CFG_TYPE(uint8,ctrlMapping,CtrlIdx)))).mtu)
      {
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
        TCPIP_REPORT_ERROR(TCPIP_SID_RXINDICATION, TCPIP_E_MSGSIZE);
#endif
      }
      else
      {
        /* evaluate ethernet frame type and forward call to sub-units */
        switch(FrameType)
        {
#if (TCPIP_IPV6_ENABLED == STD_ON)
          case TCPIP_IP_FRAMETYPE_IPV6:
          {
# if (TCPIP_RX_PACKET_DROP_FCT_ENABLED == STD_ON)
          result =
# else
            (void)
# endif /* TCPIP_RX_PACKET_DROP_FCT_ENABLED == STD_ON */
              TcpIp_IpV6_rxIndicationIp((TCPIP_CFG_TYPE(uint8,ctrlMapping,CtrlIdx)),DataPtr,LenByte,IsBroadcast,PhysAddrPtr);
            break;
          }
#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */
#if (TCPIP_IPV4_ENABLED == STD_ON)
          case TCPIP_IP_FRAMETYPE_IPV4:
          {
# if (TCPIP_RX_PACKET_DROP_FCT_ENABLED == STD_ON)
            result =
# else
            (void)
# endif /* TCPIP_RX_PACKET_DROP_FCT_ENABLED == STD_ON */
              TcpIp_IpV4_rxIndicationIp((TCPIP_CFG_TYPE(uint8,ctrlMapping,CtrlIdx)),IsBroadcast,PhysAddrPtr,DataPtr,LenByte);
            break;
          }
          case TCPIP_IP_FRAMETYPE_ARP:
          {
# if (TCPIP_RX_PACKET_DROP_FCT_ENABLED == STD_ON)
            result =
# else
            (void)
# endif /* TCPIP_RX_PACKET_DROP_FCT_ENABLED == STD_ON */
              TcpIp_IpV4_rxIndicationArp((TCPIP_CFG_TYPE(uint8,ctrlMapping,CtrlIdx)),PhysAddrPtr,DataPtr,LenByte);
            break;
          }
#endif /* (TCPIP_IPV4_ENABLED == STD_ON) */
          default:
          {
            /* ignore unknown frame type */
            break;
          }
        }
      }
    }
  }

#if (TCPIP_RX_PACKET_DROP_FCT_ENABLED == STD_ON)
  if(result != TCPIP_IP_RX_OK)
  {
    TcpIp_RxPacketDropHandlerAPI(result,CtrlIdx,FrameType,IsBroadcast,PhysAddrPtr,DataPtr,LenByte);
  }
#endif /* TCPIP_RX_PACKET_DROP_FCT_ENABLED == STD_ON */

  DBG_TCPIP_RXINDICATION_EXIT(CtrlIdx,FrameType,IsBroadcast,PhysAddrPtr,DataPtr,LenByte);
} /* TcpIp_RxIndication */

/*==================[internal function definitions]=========================*/


TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ip_init(void)
{
  DBG_TCPIP_IP_INIT_ENTRY();

#if (TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON) || (TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF)
  {
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_Ip_Frag_MemReserved=
      (P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA))TcpIp_Ip_Frag_MemReservedMaxAligned;
    uint32 n= 0u;

#if (TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON)
    n= TcpIp_Ip_Reass_init(TcpIp_Ip_Frag_MemReserved, n);
#endif /* (TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON) */
#if (TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON) && (TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF)
    n= TCPIP_ROUNDUP(n, sizeof(uint32));
#endif /* (TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON) && (TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF) */
#if (TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF)
#if (TCPIP_POSTCONDITION_ASSERT_ENABLED == STD_ON)
    n=
#else /* (TCPIP_POSTCONDITION_ASSERT_ENABLED == STD_ON) */
    (void)
#endif/* (TCPIP_POSTCONDITION_ASSERT_ENABLED == STD_ON) */
     TcpIp_Ip_Frag_init(TcpIp_Ip_Frag_MemReserved, n);
#endif /* (TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF) */

    TCPIP_POSTCONDITION_ASSERT(n<=TCPIP_IP_FRAGMENT_MEMRESERVED, TCPIP_SID_INIT);  /* TCPIP_E_NOBUFS */
  }
#endif /* (TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON) || (TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF) */

  DBG_TCPIP_IP_INIT_EXIT();
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ip_mainFunction(void)
{
  DBG_TCPIP_IP_MAINFUNCTION_ENTRY();

#if (TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON)
  TcpIp_Ip_Frag_mainFunction();
#endif /* (TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON) */

  DBG_TCPIP_IP_MAINFUNCTION_EXIT();
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Ip_transmit
(
  P2VAR(TcpIp_Ip_CopyDataFunctorType, AUTOMATIC, TCPIP_APPL_DATA) copyDataFunctorPtr,
  P2VAR(TcpIp_Ip_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA) updateHeaderFunctorPtr,
  P2VAR(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameterPtr,
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 headerLength,
  uint16 totalLength
)
{
  uint16 remoteAddrHandle;
  Std_ReturnType retVal = TCPIP_E_NOT_OK;

  DBG_TCPIP_IP_TRANSMIT_ENTRY
    (copyDataFunctorPtr,updateHeaderFunctorPtr,ipParameterPtr,
     localAddrId,remoteAddrPtr,dataPtr,headerLength,totalLength);

  /* first get the remote link layer address */
  /* Deviation MISRAC2012-3 */
  retVal = TCPIP_IP_GETREMOTEADDRHANDLE(TCPIP_IP_DOMAIN_GET(remoteAddrPtr->domain))
                (&remoteAddrHandle, remoteAddrPtr, localAddrId);

  if(retVal == TCPIP_OK)
  {
#if (TCPIP_REMOTE_MAC_FILTERING_ENABLED == STD_ON)
    uint8 const ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr;
    P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtrInternal;
    if(remoteAddrPtr->domain == TCPIP_AF_INET)
    {
      /* Deviation MISRAC2012-2 */
      remoteAddrPtrInternal = ((P2CONST(TcpIp_SockAddrInetType, AUTOMATIC, TCPIP_APPL_DATA))remoteAddrPtr)->addr;
    }
    else
    {
      /* Deviation MISRAC2012-2 */
      remoteAddrPtrInternal = ((P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA))remoteAddrPtr)->addr;
    }

    if(remoteAddrHandle < TCPIP_IP_REMOTE_HANDLE_MULTICAST)
    {
      /* dedicated physical address from ARP/NCACHE table */
      /* Deviation MISRAC2012-3 */
      physAddrPtr = TCPIP_IP_GETPHYSADDRPTR(TCPIP_IP_DOMAIN_GET(remoteAddrPtr->domain))
                    (remoteAddrHandle,ctrlIdx);
      retVal = TcpIp_IpSecDb_checkSecurityPolicyAddr(localAddrId, remoteAddrPtrInternal,physAddrPtr);
    }
    if(retVal != E_OK)
    {
      TCPIP_PRECONDITION_ASSERT(remoteAddrHandle < TCPIP_IP_REMOTE_HANDLE_MULTICAST, TCPIP_INTERNAL_API_ID);
      /* Deviation MISRAC2012-3 */
      TCPIP_IP_UNLOCKADDR(TCPIP_IP_DOMAIN_GET(remoteAddrPtr->domain))
                  (remoteAddrHandle,ctrlIdx);
    }
    else
#endif /* (TCPIP_REMOTE_MAC_FILTERING_ENABLED == STD_ON) */
    {
      TcpIp_Ip_AddrType ipAddress;

      ipAddress.localAddrId = localAddrId;
      ipAddress.remoteAddrPtr = remoteAddrPtr;

      /* get buffer from hardware and transmit ip frame */
      retVal = TcpIp_Ip_transmitIp(copyDataFunctorPtr,
                                  updateHeaderFunctorPtr,
                                  ipParameterPtr,
                                  &ipAddress,
                                  remoteAddrHandle,
                                  dataPtr,
                                  headerLength,
                                  totalLength
                                 );
      /* if there was an lookup unlock the entry */
      if(remoteAddrHandle < TCPIP_IP_REMOTE_HANDLE_MULTICAST)
      {
        /* Deviation MISRAC2012-3 */
        TCPIP_IP_UNLOCKADDR(TCPIP_IP_DOMAIN_GET(remoteAddrPtr->domain))
                    (remoteAddrHandle, TcpIp_IpAddrM_getCtrlIdx(ipAddress.localAddrId));
      }
    }
  }

  DBG_TCPIP_IP_TRANSMIT_EXIT
    (retVal, copyDataFunctorPtr,updateHeaderFunctorPtr,ipParameterPtr,
     localAddrId,remoteAddrPtr,dataPtr,headerLength,totalLength);

  return retVal;
}

#if ((TCPIP_TCP_ENABLED == STD_ON) || (TCPIP_UDP_ENABLED == STD_ON))

TS_MOD_PRIV_DEFN FUNC(TcpIp_LocalAddrIdType, TCPIP_CODE) TcpIp_Ip_getTxLocalAddrId_byRemoteIp
(
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr
)
{
  TcpIp_LocalAddrIdType result;

  DBG_TCPIP_IP_GETTXLOCALADDRID_BYREMOTEIP_ENTRY(remoteAddrPtr);

  /* Deviation MISRAC2012-3 */
  result = TCPIP_IP_GETTXLOCALADDRID_BYREMOTEIP
                (TCPIP_IP_DOMAIN_GET(remoteAddrPtr->domain))
                (remoteAddrPtr);

  DBG_TCPIP_IP_GETTXLOCALADDRID_BYREMOTEIP_EXIT(result,remoteAddrPtr);

  return result;
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_LocalAddrIdType, TCPIP_CODE) TcpIp_Ip_getTxLocalAddrId_byLocalAddrId
(
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr
)
{
  TcpIp_LocalAddrIdType result;

  DBG_TCPIP_IP_GETTXLOCALADDRID_BYLOCALADDRID_ENTRY(localAddrId, remoteAddrPtr);
  TCPIP_PBCFG_PRECONDITION_ASSERT(localAddrId, localAddrCfg, TCPIP_INTERNAL_API_ID);
  {
    const uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
    TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

    /* Deviation MISRAC2012-3 */
    result = TCPIP_IP_GETTXLOCALADDRID_BYLOCALADDRID
                  (TCPIP_IP_DOMAIN_GET(remoteAddrPtr->domain))
                  (
                  ctrlIdx,
                  remoteAddrPtr
                  );
  }
  DBG_TCPIP_IP_GETTXLOCALADDRID_BYLOCALADDRID_EXIT(result, localAddrId, remoteAddrPtr);

  return result;
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Ip_getMaximumPayload
(
  uint8 ipSecSaCommonIdx,
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) maxPayloadPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) additionalHdrSizePtr
)
{
  Std_ReturnType result;

  /* Deviation MISRAC2012-3 */
  DBG_TCPIP_IP_GETMAXIMUMPAYLOAD_ENTRY
    (ipSecSaCommonIdx, localAddrId, maxPayloadPtr, additionalHdrSizePtr);
  result = TCPIP_IP_GETMAXIMUMPAYLOAD
                (TCPIP_IPADDRM_ADDRDOMAIN_GET(TcpIp_IpAddrM_getLocalAddrInfo(localAddrId)))
                (ipSecSaCommonIdx, localAddrId, maxPayloadPtr, additionalHdrSizePtr);
  DBG_TCPIP_IP_GETMAXIMUMPAYLOAD_EXIT
    (result, ipSecSaCommonIdx, localAddrId, maxPayloadPtr, additionalHdrSizePtr);

  return result;
}

#endif /* ((TCPIP_TCP_ENABLED == STD_ON) || (TCPIP_UDP_ENABLED == STD_ON)) */

#if (TCPIP_TCP_ENABLED == STD_ON)

TS_MOD_PRIV_DEFN FUNC(uint16, TCPIP_CODE) TcpIp_Ip_getMinimumPayload
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  uint16 result;
#if TCPIP_IP_DUALSTACK==STD_OFF
  TS_PARAM_UNUSED(localAddrId);
#endif

  /* Deviation MISRAC2012-3 */
  DBG_TCPIP_IP_GETMINIMUMPAYLOAD_ENTRY(localAddrId);
    result = TCPIP_IP_GETMINIMUMPAYLOAD
                  (TCPIP_IPADDRM_ADDRDOMAIN_GET(TcpIp_IpAddrM_getLocalAddrInfo(localAddrId)))
                  ();
  DBG_TCPIP_IP_GETMINIMUMPAYLOAD_EXIT(result, localAddrId);

  return result;
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ip_reachConfirmation
(
  uint8 CtrlIdx,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr
)
{
  DBG_TCPIP_IP_REACHCONFIRMATION_ENTRY(CtrlIdx,remoteAddrPtr);
  {
     /* Deviation MISRAC2012-3 */
     TCPIP_IP_REACHCONFIRMATION(TCPIP_IP_DOMAIN_GET(remoteAddrPtr->domain))
       (
        CtrlIdx,
        remoteAddrPtr
       );
  }

  DBG_TCPIP_IP_REACHCONFIRMATION_EXIT(CtrlIdx,remoteAddrPtr);

}
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
TS_MOD_PRIV_DEFN FUNC(boolean, TCPIP_CODE) TcpIp_Ip_isValidTcpRemoteAddress
(
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RemoteAddrPtr
)
{
  boolean retVal;
  DBG_TCPIP_IP_ISVALIDTCPREMOTEADDRESS_ENTRY(RemoteAddrPtr);
  {
     /* Deviation MISRAC2012-3 */
     retVal = TCPIP_IP_ISVALIDTCPREMOTEADDRESS(TCPIP_IP_DOMAIN_GET(RemoteAddrPtr->domain))
       (
        RemoteAddrPtr
       );
  }
  DBG_TCPIP_IP_ISVALIDTCPREMOTEADDRESS_EXIT(retVal,RemoteAddrPtr);
  return retVal;
}
#endif/* (TCPIP_DEV_ERROR_DETECT == STD_ON) */

#endif /* (TCPIP_TCP_ENABLED == STD_ON) */

TS_MOD_PRIV_DEFN FUNC(TcpIp_StateType, TCPIP_CODE) TcpIp_Ip_getIfState
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  TcpIp_StateType result;

  DBG_TCPIP_IP_GETIFSTATE_ENTRY(localAddrId);
  {
    if(localAddrId == TCPIP_LOCALADDRID_ANY)
    {
      /* ANY is no physical device thus we assume this is available always */
      result = TCPIP_STATE_ONLINE;
    }
    else
    {
      /* Deviation MISRAC2012-3 */
      result = TCPIP_IP_GETIFSTATE
                    (TCPIP_IPADDRM_ADDRDOMAIN_GET(TcpIp_IpAddrM_getLocalAddrInfo(localAddrId)))
                    (localAddrId);
    }
  }
  DBG_TCPIP_IP_GETIFSTATE_EXIT(result, localAddrId);

  return result;
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ip_getIpConfig
(
  TcpIp_LocalAddrIdType LocalAddrId,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) IpAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) NetmaskPtr,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) DefaultRouterPtr
)
{
  DBG_TCPIP_IP_GETIPCONFIG_ENTRY(LocalAddrId,IpAddrPtr,NetmaskPtr,DefaultRouterPtr);

  /* Deviation MISRAC2012-3 */
  TCPIP_IP_GETIPCONFIG
      (TCPIP_IPADDRM_ADDRDOMAIN_GET(TcpIp_IpAddrM_getLocalAddrInfo(LocalAddrId)))
      (LocalAddrId, IpAddrPtr, NetmaskPtr, DefaultRouterPtr);

  DBG_TCPIP_IP_GETIPCONFIG_EXIT(LocalAddrId,IpAddrPtr,NetmaskPtr,DefaultRouterPtr);
}

TS_MOD_PRIV_DEFN FUNC(uint16, TCPIP_CODE) TcpIp_Ip_calcPartialChecksum
(
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) localAddrPtr,
  uint16 lenByte,
  uint8 protocol
)
{
  uint16 retVal;

  DBG_TCPIP_IP_CALCPARTIALCHECKSUM_ENTRY(remoteAddrPtr,localAddrPtr,lenByte,protocol);

  retVal =
    /* Deviation MISRAC2012-3 */
    TCPIP_IP_CALCPARTIALCHECKSUM(TCPIP_IP_DOMAIN_GET(remoteAddrPtr->domain))
    (
      remoteAddrPtr,
      localAddrPtr,
      lenByte,
      protocol
    );

  DBG_TCPIP_IP_CALCPARTIALCHECKSUM_EXIT(retVal,remoteAddrPtr,localAddrPtr,lenByte,protocol);

  return retVal;
}

#if (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL)
TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Ip_compareIpAddr
(
  uint8 domainType,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) AddrPtr1,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) AddrPtr2
)
{
  Std_ReturnType retVal = E_OK;
  uint8 i;

  DBG_TCPIP_IP_COMPAREIPADDR_ENTRY(domainType,AddrPtr1,AddrPtr2);

  for(i = 0U; i < (TCPIP_IP_GET_IP_ADDR_LEN(domainType)); i++)
  {
    if(AddrPtr1[i] != AddrPtr2[i])
    {
      retVal = E_NOT_OK;
      break;
    }
  }

  DBG_TCPIP_IP_COMPAREIPADDR_EXIT(retVal,domainType,AddrPtr1,AddrPtr2);

  return retVal;
}
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL) */

#if (TCPIP_UDP_BROADCAST_RECEPTION_ENABLED == STD_ON)
TS_MOD_PRIV_DEFN FUNC(boolean, TCPIP_CODE) TcpIp_Ip_isAddrLimBroadcastAllNodesMulticast
(
  uint8 ctrlIdx,
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) localIpAddrPtr,
  TcpIp_DomainType domainType
)
{
  boolean retVal = FALSE;
  DBG_TCPIP_IP_ISADDRLIMBROADCASTALLNODESMULTICAST_ENTRY(ctrlIdx,localAddrId,localIpAddrPtr,domainType);

  /* Deviation MISRAC2012-3 */
  retVal = TCPIP_IP_ISADDRLIMBROADCASTALLNODESMULTICAST
                (TCPIP_IP_DOMAIN_GET(domainType))
                (ctrlIdx,localAddrId,localIpAddrPtr);

#if (TCPIP_IP_DUALSTACK == STD_OFF)
  TS_PARAM_UNUSED(domainType);
#endif /* (TCPIP_IP_DUALSTACK == STD_OFF) */
  DBG_TCPIP_IP_ISADDRLIMBROADCASTALLNODESMULTICAST_EXIT(retVal,ctrlIdx,localAddrId,localIpAddrPtr,domainType);

  return retVal;
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ip_getCtrlAddrRange
(
  uint8 ctrlIdx,
  P2VAR(TcpIp_LocalAddrIdType, AUTOMATIC, TCPIP_APPL_DATA) startIndexPtr,
  P2VAR(TcpIp_LocalAddrIdType, AUTOMATIC, TCPIP_APPL_DATA) endIndexPtr,
  TcpIp_DomainType domainType
)
{
  DBG_TCPIP_IP_GETCTRLADDRRANGE_ENTRY(ctrlIdx,startIndexPtr,endIndexPtr,domainType);
  /* Deviation MISRAC2012-3 */
  TCPIP_IP_GETCTRLADDRRANGE
                  (TCPIP_IP_DOMAIN_GET(domainType))
                  (ctrlIdx,startIndexPtr,endIndexPtr);

#if (TCPIP_IP_DUALSTACK == STD_OFF)
  TS_PARAM_UNUSED(domainType);
#endif /* (TCPIP_IP_DUALSTACK == STD_OFF) */

  DBG_TCPIP_IP_GETCTRLADDRRANGE_EXIT(ctrlIdx,startIndexPtr,endIndexPtr,domainType);
}
#endif /* (TCPIP_UDP_BROADCAST_RECEPTION_ENABLED == STD_ON) */

/*==================[internal function definitions]=========================*/

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[end of file]===========================================*/
