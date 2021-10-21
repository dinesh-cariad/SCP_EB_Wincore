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

/* Misra-C:2012 Deviations:
 *
 * MISRAC2012-1) Deviated Rule: 11.3 (required)
 * A cast shall not be performed between a pointer to object type and a pointer to a different
 * object type.
 *
 * Reason:
 * The most efficient way to copy an array of 4 bytes to an uint32 while preserving the
 * byte order without any alignment constraints requires to cast the uint32* to an uint8*.
 *
 * MISRAC2012-2) Deviated Rule: 8.9 (Advisory)
 * An object should be defined at block scope if its identifier only appears in a single function.
 *
 * Reason:
 * The AUTOSAR memory mapping requires that functions are mapped in SEC_CODE memory sections.
 * Objects at block scope require a different memory mapping, e.g. to a SEC_VAR section,
 * which leads to nested memory sections, which is not supported by some compilers.
 *
 */

/*==================[inclusions]============================================*/

#include <EthIf.h>               /* EthIf API declarations */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_IpV6_Priv.h>     /* own SW-unit interface file */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#if (TCPIP_IPV6_LINK_LOCAL_ENABLED == STD_ON)

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_LinkLocal_dadResult
(
  TcpIp_LocalAddrIdType localAddrId,
  boolean unique
);

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[internal constants]====================================*/

#define TCPIP_START_SEC_CONST_32
#include <TcpIp_MemMap.h>

  /* Deviation MISRAC2012-2 */
  STATIC CONST(uint32, TCPIP_CONST) TcpIp_LinkLocal_Prefix[4] =
      {COMSTACK_CT_HTON_UINT32(0xFE800000U),COMSTACK_CT_HTON_UINT32(0x00000000U),0U,0U};

#define TCPIP_STOP_SEC_CONST_32
#include <TcpIp_MemMap.h>

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV6_LinkLocal_init
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  DBG_TCPIP_IPV6_LINKLOCAL_INIT_ENTRY(localAddrId);

  TcpIp_IpV6_Static_init(localAddrId, TCPIP_IPADDRM_ASSIGN_LINKLOCALV6);

  DBG_TCPIP_IPV6_LINKLOCAL_INIT_EXIT(localAddrId);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV6_LinkLocal_startAssign
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  const uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
  uint8 ethAddr[TCPIP_ETH_ADDR_LENGTH];
  TcpIp_SockAddrInet6Type localIpAddr;
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) localIpAddrPtr =
      /* Deviation MISRAC2012-1 */
      (P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA)) &localIpAddr;
  TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

  DBG_TCPIP_IPV6_LINKLOCAL_STARTASSIGN_ENTRY(localAddrId);

  /* retrieve link layer of controller */
  EthIf_GetPhysAddr((TCPIP_CFG(ctrl,ctrlIdx)).ethIfCtrlIdx, ethAddr);

  /* generates link-local address */
  TcpIp_IpV6_generateIp(localIpAddr.addr, ethAddr, TcpIp_LinkLocal_Prefix);
  localIpAddr.domain = TCPIP_AF_INET6;

  /* save generated link-local address */
  (void) TcpIp_IpV6_Static_requestIpAddress
      (localAddrId, localIpAddrPtr, TCPIP_IPADDRM_ASSIGN_LINKLOCALV6);

  /* start assignment of generated link-local address */
  TcpIp_IpV6_Static_startAssign
    (localAddrId, TCPIP_IPADDRM_ASSIGN_LINKLOCALV6, &TcpIp_IpV6_LinkLocal_dadResult);

  DBG_TCPIP_IPV6_LINKLOCAL_STARTASSIGN_EXIT(localAddrId);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV6_LinkLocal_stopAssign
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  DBG_TCPIP_IPV6_LINKLOCAL_STOPASSIGN_ENTRY(localAddrId);

  TcpIp_IpV6_Static_stopAssign(localAddrId, TCPIP_IPADDRM_ASSIGN_LINKLOCALV6);

  DBG_TCPIP_IPV6_LINKLOCAL_STOPASSIGN_EXIT(localAddrId);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV6_LinkLocal_getIpConfig
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) gatewayIpPtrPtr,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) netmaskIpPtrPtr
)
{
  TcpIp_IpAddrConstPtr netMaskPtr = TcpIp_IPV6_Prefix_Mask;
  DBG_TCPIP_IPV6_LINKLOCAL_GETIPCONFIG_ENTRY
    (localAddrId,addressIpPtrPtr,gatewayIpPtrPtr,netmaskIpPtrPtr);

  TcpIp_IpV6_Static_getIpConfig
    (localAddrId, addressIpPtrPtr, gatewayIpPtrPtr, netmaskIpPtrPtr, TCPIP_IPADDRM_ASSIGN_LINKLOCALV6);
  *netmaskIpPtrPtr = netMaskPtr;

  DBG_TCPIP_IPV6_LINKLOCAL_GETIPCONFIG_EXIT
    (localAddrId,addressIpPtrPtr,gatewayIpPtrPtr,netmaskIpPtrPtr);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV6_LinkLocal_getIpAddress
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr
)
{
  DBG_TCPIP_IPV6_LINKLOCAL_GETIPADDRESS_ENTRY(localAddrId,addressIpPtrPtr);

  TcpIp_IpV6_Static_getIpAddress
    (localAddrId, addressIpPtrPtr, TCPIP_IPADDRM_ASSIGN_LINKLOCALV6);

  DBG_TCPIP_IPV6_LINKLOCAL_GETIPADDRESS_EXIT(localAddrId,addressIpPtrPtr);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV6_LinkLocal_assignmentActive
(
  TcpIp_LocalAddrIdType localAddrId,
  boolean state
)
{
  DBG_TCPIP_IPV6_LINKLOCAL_ASSIGNMENTACTIVE_ENTRY(localAddrId,state);
  {
    TS_PARAM_UNUSED(localAddrId);
    TS_PARAM_UNUSED(state);
  }
  DBG_TCPIP_IPV6_LINKLOCAL_ASSIGNMENTACTIVE_EXIT(localAddrId,state);
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpV6_LinkLocal_requestIpAddress
(
    TcpIp_LocalAddrIdType localAddrId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) LocalIpAddrPtr
)
{
  Std_ReturnType result = E_OK;

  DBG_TCPIP_IPV6_LINKLOCAL_REQUESTIPADDRESS_ENTRY(localAddrId,LocalIpAddrPtr);
  {
    TS_PARAM_UNUSED(localAddrId);
    TS_PARAM_UNUSED(LocalIpAddrPtr);
  }
  DBG_TCPIP_IPV6_LINKLOCAL_REQUESTIPADDRESS_EXIT(result, localAddrId,LocalIpAddrPtr);

  return result;
}

TS_MOD_PRIV_DEFN FUNC(uint8, TCPIP_CODE) TcpIp_IpV6_LinkLocal_getIpAddrState
(
    TcpIp_LocalAddrIdType localAddrId
)
{
  uint8 state = TCPIP_IPADDRM_ADDRSTATE_INVALID;

  DBG_TCPIP_IPV6_LINKLOCAL_GETIPADDRSTATE_ENTRY(localAddrId);

  state = TcpIp_IpV6_Static_getIpAddrState(localAddrId, TCPIP_IPADDRM_ASSIGN_LINKLOCALV6);

  DBG_TCPIP_IPV6_LINKLOCAL_GETIPADDRSTATE_EXIT(state,localAddrId);

  return state;
}

/*==================[internal function definitions]=========================*/

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_LinkLocal_dadResult
(
  TcpIp_LocalAddrIdType localAddrId,
  boolean unique
)
{
#if (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)
  TcpIp_IpV6_StaticIp_dadResult(localAddrId, unique, TCPIP_IPADDRM_ASSIGN_LINKLOCALV6);
#else
  TS_PARAM_UNUSED(localAddrId);
  TS_PARAM_UNUSED(unique);
#endif /* (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON) */
}

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#else /* (TCPIP_IPV6_LINK_LOCAL_ENABLED == STD_ON) */
  TS_PREVENTEMPTYTRANSLATIONUNIT
#endif /* (TCPIP_IPV6_LINK_LOCAL_ENABLED == STD_ON) */

/*==================[end of file]===========================================*/
