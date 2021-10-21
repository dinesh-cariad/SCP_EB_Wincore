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
 * MISRAC2012-4) Deviated Rule: 11.3 (required)
 * A cast shall not be performed between a pointer to object type and a pointer to a different
 * object type.
 *
 * Reason:
 * Polymorphic (derived) data structures need an explicit pointer-to-base-class cast and
 * then another explicit pointer-to-derived-class cast in C.
 */

/*==================[inclusions]============================================*/
#include <EthIf.h>                /* EthIf API declarations */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Ndp_Priv.h>       /* own SW-unit private file */
#include <TcpIp_IpV6_Int.h>       /* used SW-unit interface file */
#include <TcpIp_Trace.h>          /* debug and trace macros; must be last */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

#if (TCPIP_IPV6_ENABLED == STD_ON)

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ndp_init (void)
{
  DBG_TCPIP_NDP_INIT_ENTRY();

  TcpIp_Ndp_NCache_init();
#if (TCPIP_NDP_RPD_ENABLED == STD_ON)
  TcpIp_Ndp_Rpd_init();
#endif /* (TCPIP_NDP_RPD_ENABLED == STD_ON) */
#if (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)
  TcpIp_Ndp_Dad_init();
#endif /* (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON) */
#if(TCPIP_NDP_DESTINATION_CACHE_ENABLED == STD_ON)
  TcpIp_Ndp_DestCache_init();
#endif /* (TCPIP_NDP_DESTINATION_CACHE_ENABLED == STD_ON) */

  DBG_TCPIP_NDP_INIT_EXIT();
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE ) TcpIp_Ndp_rxIndication
(
  uint8 type,
  uint8 code,
  P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 lenByte,
  uint8 HopLimit
)
{
  DBG_TCPIP_NDP_RXINDICATION_ENTRY(type,code,remoteAddrPtr,localAddrId,dataPtr,lenByte,HopLimit);

  TCPIP_PRECONDITION_ASSERT
    (TCPIP_IPADDRM_IS_ADDRDOMAIN((TCPIP_CFG(localAddr,localAddrId)).addrType,IPV6),
     TCPIP_INTERNAL_API_ID);

  /* ICMP code must be zero */
  if(code == TCPIP_NDP_ICMP_CODE_NDP)
  {
    if(HopLimit == TCPIP_NDP_HOP_LIMIT)
    {
      switch(type)
      {
        case TCPIP_NDP_ICMP_TYPE_NEIGHBOR_SOLICIT:
          TcpIp_Ndp_NCache_rxNeighborSolicit(remoteAddrPtr, localAddrId, dataPtr, lenByte);
          break;
        case TCPIP_NDP_ICMP_TYPE_NEIGHBOR_ADVERT:
          TcpIp_Ndp_NCache_rxNeighborAdvert(localAddrId, dataPtr, lenByte);
          break;
#if (TCPIP_NDP_RPD_ENABLED == STD_ON)
        case TCPIP_NDP_ICMP_TYPE_ROUTER_ADVERT:
          TcpIp_Ndp_Rpd_rxRouterAdvert
          (
            remoteAddrPtr->addr,
            localAddrId,
            dataPtr,
            lenByte
          );
          break;
#endif /* (TCPIP_NDP_RPD_ENABLED == STD_ON) */
        default:
          /* unknown NDP message */
          break;
      }
    }
  }

  DBG_TCPIP_NDP_RXINDICATION_EXIT(type,code,remoteAddrPtr,localAddrId,dataPtr,lenByte,HopLimit);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ndp_mainFunction
(
  uint16 timerTick
)
{
  DBG_TCPIP_NDP_MAINFUNCTION_ENTRY(timerTick);
#if (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)
  TcpIp_Ndp_Dad_mainFunction(timerTick);
#endif /* (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON) */
  TcpIp_Ndp_NCache_mainFunction(timerTick);
#if (TCPIP_NDP_RPD_ENABLED == STD_ON)
  TcpIp_Ndp_Rpd_mainFunction(timerTick);
#endif /* (TCPIP_NDP_RPD_ENABLED == STD_ON) */

  DBG_TCPIP_NDP_MAINFUNCTION_EXIT(timerTick);
}

TS_MOD_PRIV_DEFN FUNC(BufReq_ReturnType, TCPIP_CODE) TcpIp_Ndp_copyData
(
  P2VAR(TcpIp_Ip_CopyDataFunctorType, AUTOMATIC, TCPIP_APPL_DATA) that,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) bufferPtr,
  uint16 copyLength
)
{
  uint8 linkLayerId;
  uint8 linkLayerAddrOffset;
  /* Deviation MISRAC2012-4 <+2> */
  P2VAR(TcpIp_Ndp_CopyDataFunctorType, AUTOMATIC, TCPIP_APPL_DATA) this =
    (P2VAR(TcpIp_Ndp_CopyDataFunctorType, AUTOMATIC, TCPIP_APPL_DATA)) that;

  TS_PARAM_UNUSED(copyLength);

  DBG_TCPIP_NDP_COPYDATA_ENTRY(that, bufferPtr, copyLength);

  if(this->type == TCPIP_NDP_ICMP_TYPE_NEIGHBOR_ADVERT)
  {
    /* build the icmpv6 header for a neighbor advertisement */
    TCPIP_NDP_SET_FLAGS(bufferPtr, this->flags);
    TCPIP_NDP_NEIGH_ADVERT_SET_RESERVED(bufferPtr);
    linkLayerId = TCPIP_NDP_OPT_TARGET_LINK_ADDR_ID;
    linkLayerAddrOffset = TCPIP_NDP_NEIGH_ADVERT_SIZE_MIN;
  }
  else if(this->type == TCPIP_NDP_ICMP_TYPE_NEIGHBOR_SOLICIT)
  {
    TCPIP_NDP_NEIGH_SOLIC_SET_RESERVED(bufferPtr);
    linkLayerId = TCPIP_NDP_OPT_SRC_LINK_ADDR_ID;
    linkLayerAddrOffset = TCPIP_NDP_NEIGH_SOLIC_SIZE_MIN;
  }
  else /* TCPIP_NDP_ICMP_TYPE_ROUTER_SOLICIT */
  {
    TCPIP_NDP_RPD_ROUTER_SOLIC_SET_RESERVED(bufferPtr);
    linkLayerAddrOffset = TCPIP_NDP_RPD_ROUTER_SOLIC_SIZE_MIN;
    linkLayerId = TCPIP_NDP_OPT_SRC_LINK_ADDR_ID;
  }

  if(this->targetIpPtr != NULL_PTR)
  {
    TCPIP_IPV6_ADDR_COPY_TO_PAYLOAD
      (TCPIP_NDP_TARGET_ADDR_GET_PTR(bufferPtr), this->targetIpPtr);
  }

  /* add target link layer address */
  if(this->linkLayerAddressPtr != NULL_PTR)
  {
    P2VAR(uint8, AUTOMATIC, TCPIP_VAR) optionPtr =
      &bufferPtr[linkLayerAddrOffset];

    TCPIP_NDP_SET_OPTION_TYPE(optionPtr, linkLayerId);
    TCPIP_NDP_SET_OPTION_LENGTH(optionPtr, TCPIP_NDP_OPT_TARGET_LINK_ADDR_LEN);
    TcpIp_EthAddrCpy(TCPIP_NDP_OPT_LL_ADDR_GET_PTR(optionPtr), this->linkLayerAddressPtr);
  }

  DBG_TCPIP_NDP_COPYDATA_EXIT(BUFREQ_OK, that, bufferPtr, copyLength);

  return BUFREQ_OK;
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Ndp_IntTransmit
(
  P2VAR(TcpIp_Ip_AddrType, AUTOMATIC, TCPIP_APPL_DATA) ipAddrPtr,
  uint16 remoteAddrHandle,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) targetIp,
  uint16 msgLength,
  uint8 type,
  uint8 flags,
  boolean addLocalLinkLayerAddress ,
  boolean useUnspecifiedSrc
)
{
  TcpIp_ReturnType result;
  TcpIp_Ndp_CopyDataFunctorType copyDataFunctor;
  /* Deviation MISRAC2012-4 <+2> */
  P2VAR(TcpIp_Ip_CopyDataFunctorType, AUTOMATIC, TCPIP_APPL_DATA) copyIpDataFunctor =
    (P2VAR(TcpIp_Ip_CopyDataFunctorType, AUTOMATIC, TCPIP_APPL_DATA)) &copyDataFunctor;
  TcpIp_Ip_Icmp_ParameterType icmpParamter;
  uint8 ethAddr[TCPIP_ETH_ADDR_LENGTH];

  copyDataFunctor.copyDataFctPtr = &TcpIp_Ndp_copyData;
  copyDataFunctor.targetIpPtr = targetIp;
  copyDataFunctor.flags = flags;
  copyDataFunctor.type = type;
  if(addLocalLinkLayerAddress )
  {
    const uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(ipAddrPtr->localAddrId);

    TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

    EthIf_GetPhysAddr((TCPIP_CFG(ctrl,ctrlIdx)).ethIfCtrlIdx, ethAddr);

    copyDataFunctor.linkLayerAddressPtr = ethAddr;
  }
  else
  {
    copyDataFunctor.linkLayerAddressPtr = NULL_PTR;
  }

  icmpParamter.code = TCPIP_NDP_ICMP_CODE_NDP;
  icmpParamter.hopLimit = TCPIP_NDP_HOP_LIMIT;
  icmpParamter.specificValue = NULL_PTR;
  icmpParamter.type = type;

  result =
    TcpIp_IpV6_Icmp_intTransmit
      (copyIpDataFunctor, NULL_PTR, msgLength, ipAddrPtr, remoteAddrHandle, &icmpParamter, useUnspecifiedSrc, FALSE);

  return result;
}
#endif /* (TCPIP_NDP_RPD_ENABLED == STD_ON) */

/*==================[internal function definitions]=========================*/

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[end of file]===========================================*/
