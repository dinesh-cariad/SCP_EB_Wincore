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
 * AUTOSAR defines a generic type which dependent on a member of the generic type
 * implements an concrete type. Thus for usage of this concrete type, a cast is necessary.
 *
 */

/*==================[inclusions]============================================*/

#include <EthIf.h>                /* EthIf API declarations */
#include <SchM_TcpIp.h>           /* SchM API declarations */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_IpV6_Priv.h>      /* used SW-unit interface file */
#include <TcpIp_IpAddrM_Int.h>    /* used SW-unit interface file */

/*==================[macros]================================================*/

/* static IPv6 assignment state machine states */
/** \brief IP address is assigned */
#define TCPIP_IPV6_STATICIP_PREFERRED     1U
/** \brief DAD ongoing - address isn't assigned */
#define TCPIP_IPV6_STATICIP_TENTATIVE     3U
/** \brief DAD ongoing - address is assigned */
#define TCPIP_IPV6_STATICIP_OPTIMISTIC    4U
/** \brief DAD ongoing - address is conflicted */
#define TCPIP_IPV6_STATICIP_CONFLICTED    5U
/** \brief no IP address is assigned */
#define TCPIP_IPV6_STATICIP_UNASSIGNED  255U

/*==================[type definitions]======================================*/

/** \brief Structure contains state information for each local address */
typedef struct
{
  uint8 state; /**< current local address state */
}TcpIp_IpV6_StaticIp_StateType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#if (TCPIP_IPV6_ENABLED == STD_ON)

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_getNewIpAddr
(
    uint8 assignIdx,
    P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) ipAddrPtr
);

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_assignSolicitMulticast
(
    uint8 assignIdx
);

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_releaseSolicitMulticast
(
  uint8 assignIdx
);

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_updatePhysAddrFilter
(
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_CONST) ip,
  Eth_FilterActionType Action
);

#if (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)

#if (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_tr_startAssign_UNASSIGNED_OPTIMISTIC
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 assignIdx,
  uint8 method,
  TcpIp_Ndp_dadResult_FctPtrType resultFctPtr
);

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_tr_startAssign_OPTIMISTIC_OPTIMISTIC
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 assignIdx,
  uint8 method,
  TcpIp_Ndp_dadResult_FctPtrType resultFctPtr
);

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_tr_stopAssign_OPTIMISTIC_UNASSIGNED
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 assignIdx,
  uint8 method
);

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_tr_dadResult_OPTIMISTIC_CONFLICTED
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 assignIdx,
  uint8 method
);

#else /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_tr_startAssign_UNASSIGNED_TENTATIVE
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 assignIdx,
  TcpIp_Ndp_dadResult_FctPtrType resultFctPtr
);

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_tr_dadResult_TENTATIVE_PREFERRED
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 assignIdx,
  uint8 method
);

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_tr_startAssign_TENTATIVE_TENTATIVE
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 assignIdx,
  uint8 method,
  TcpIp_Ndp_dadResult_FctPtrType resultFctPtr
);

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_tr_stopAssign_TENTATIVE_UNASSIGNED
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 assignIdx
);

#endif /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */

#else /* (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_OFF) */

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_tr_startAssign_UNASSIGNED_PREFERRED
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 assignIdx,
  uint8 method
);

#endif /* (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_OFF) */

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_tr_stopAssign_PREFERRED_UNASSIGNED
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 assignIdx,
  uint8 method
);

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_tr_startAssign_UNASSIGNED
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 assignIdx,
  uint8 method,
  TcpIp_Ndp_dadResult_FctPtrType resultFctPtr
);

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_tr_startAssign_PREFERRED
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 assignIdx,
  uint8 method,
  TcpIp_Ndp_dadResult_FctPtrType resultFctPtr
);


STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_entry_ASSIGNED
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 method
);

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_entry_INACTIVE
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 assignIdx
);

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_exit_ASSIGNED
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 assignIdx,
  uint8 method
);



STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_Static_dadResult
(
  TcpIp_LocalAddrIdType localAddrId,
  boolean unique
);

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

#define TCPIP_START_SEC_CONST_32
#include <TcpIp_MemMap.h>

STATIC CONST(uint32, TCPIP_CONST) TcpIp_IpAddr_unspecified[TCPIP_IPV6_ADDR_SIZE] = {0U};

#define TCPIP_STOP_SEC_CONST_32
#include <TcpIp_MemMap.h>


/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

#define TCPIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

STATIC TcpIp_IpV6_StaticIp_StateType TcpIp_IpV6_StaticIp_State[TCPIP_NUM_IPV6_LOCALADDRENTRIES];

#define TCPIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

#define TCPIP_START_SEC_VAR_CLEARED_32
#include <TcpIp_MemMap.h>

STATIC VAR(uint32, TCPIP_VAR_CLEARED)
  TcpIp_IpV6_StaticIp_IpAddress[TCPIP_NUM_IPV6_LOCALADDRENTRIES][TCPIP_IPV6_ADDR_SIZE];

STATIC VAR(uint32, TCPIP_VAR_CLEARED)
  TcpIp_IpV6_StaticIp_NewIpAddress[TCPIP_NUM_IPV6_LOCALADDRENTRIES][TCPIP_IPV6_ADDR_SIZE];

#define TCPIP_STOP_SEC_VAR_CLEARED_32
#include <TcpIp_MemMap.h>

/*==================[external function definitions]=========================*/

/*==================[SW-Unit internal functions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_init
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  DBG_TCPIP_IPV6_STATICIP_INIT_ENTRY(localAddrId);

  TcpIp_IpV6_Static_init(localAddrId, TCPIP_IPADDRM_ASSIGN_STATICV6);

  DBG_TCPIP_IPV6_STATICIP_INIT_EXIT(localAddrId);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_startAssign
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  DBG_TCPIP_IPV6_STATICIP_STARTASSIGN_ENTRY(localAddrId);

  TcpIp_IpV6_Static_startAssign(localAddrId, TCPIP_IPADDRM_ASSIGN_STATICV6, &TcpIp_IpV6_Static_dadResult);

  DBG_TCPIP_IPV6_STATICIP_STARTASSIGN_EXIT(localAddrId);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_stopAssign
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  DBG_TCPIP_IPV6_STATICIP_STOPASSIGN_ENTRY(localAddrId);

  TcpIp_IpV6_Static_stopAssign(localAddrId, TCPIP_IPADDRM_ASSIGN_STATICV6);

  DBG_TCPIP_IPV6_STATICIP_STOPASSIGN_EXIT(localAddrId);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_getIpConfig
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) gatewayIpPtrPtr,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) netmaskIpPtrPtr
)
{
  DBG_TCPIP_IPV6_STATICIP_GETIPCONFIG_ENTRY
    (localAddrId,addressIpPtrPtr,gatewayIpPtrPtr,netmaskIpPtrPtr);

  TcpIp_IpV6_Static_getIpConfig
    (localAddrId, addressIpPtrPtr, gatewayIpPtrPtr, netmaskIpPtrPtr, TCPIP_IPADDRM_ASSIGN_STATICV6);

  DBG_TCPIP_IPV6_STATICIP_GETIPCONFIG_EXIT
    (localAddrId,addressIpPtrPtr,gatewayIpPtrPtr,netmaskIpPtrPtr);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_getIpAddress
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr
)
{
  DBG_TCPIP_IPV6_STATICIP_GETIPADDRESS_ENTRY(localAddrId,addressIpPtrPtr);

  TcpIp_IpV6_Static_getIpAddress
    (localAddrId, addressIpPtrPtr, TCPIP_IPADDRM_ASSIGN_STATICV6);

  DBG_TCPIP_IPV6_STATICIP_GETIPADDRESS_EXIT(localAddrId,addressIpPtrPtr);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_assignmentActive
(
  TcpIp_LocalAddrIdType localAddrId,
  boolean state
)
{
  DBG_TCPIP_IPV6_STATICIP_ASSIGNMENTACTIVE_ENTRY(localAddrId,state);
  {
    TS_PARAM_UNUSED(localAddrId);
    TS_PARAM_UNUSED(state);
  }
  DBG_TCPIP_IPV6_STATICIP_ASSIGNMENTACTIVE_EXIT(localAddrId,state);
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpV6_StaticIp_requestIpAddress
(
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) LocalIpAddrPtr
)
{
  Std_ReturnType result;

  DBG_TCPIP_IPV6_STATICIP_REQUESTIPADDRESS_ENTRY(localAddrId,LocalIpAddrPtr);

  result = TcpIp_IpV6_Static_requestIpAddress
             (localAddrId, LocalIpAddrPtr, TCPIP_IPADDRM_ASSIGN_STATICV6);

  DBG_TCPIP_IPV6_STATICIP_REQUESTIPADDRESS_EXIT(result, localAddrId,LocalIpAddrPtr);

  return result;
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpV6_StaticIp_getIpAddrState
(
    TcpIp_LocalAddrIdType localAddrId
)
{
  uint8 state = TCPIP_IPADDRM_ADDRSTATE_INVALID;

  DBG_TCPIP_IPV6_STATICIP_GETIPADDRSTATE_ENTRY(localAddrId);

  state = TcpIp_IpV6_Static_getIpAddrState(localAddrId, TCPIP_IPADDRM_ASSIGN_STATICV6);

  DBG_TCPIP_IPV6_STATICIP_GETIPADDRSTATE_EXIT(state,localAddrId);

  return state;
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV6_Static_init
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 method
)
{
  const uint8 assignIdx = TcpIp_IpAddrM_getAssignmentIdx(localAddrId, method);

  CONSTP2CONST(TcpIp_localAddrIpV6ConfigType,AUTOMATIC,TCPIP_APPL_CONST) intLocalAddrIndexPtr =
    &(TCPIP_CFG_TYPE(TcpIp_localAddrIpV6ConfigType,intIpV6LocalAddr,assignIdx));

  if (intLocalAddrIndexPtr->staticAddrId != TCPIP_LOCALADDRID_INVALID )
  {
    CONSTP2CONST(TcpIp_localAddrIpV6StaticConfigType,AUTOMATIC,TCPIP_APPL_CONST) intLocalAddrPtr =
        &(TCPIP_CFG_TYPE(TcpIp_localAddrIpV6StaticConfigType,intIPv6StaticAddr,intLocalAddrIndexPtr->staticAddrId));

    TCPIP_IPV6_ADDR_CPY(TcpIp_IpV6_StaticIp_IpAddress[assignIdx], intLocalAddrPtr->srcIpStatic);
    TCPIP_IPV6_ADDR_CPY(TcpIp_IpV6_StaticIp_NewIpAddress[assignIdx], intLocalAddrPtr->srcIpStatic);
  }
  else
  {
    TCPIP_IPV6_SET_UNSPECIFIED_IP(TcpIp_IpV6_StaticIp_IpAddress[assignIdx]);
    TCPIP_IPV6_SET_UNSPECIFIED_IP(TcpIp_IpV6_StaticIp_NewIpAddress[assignIdx]);
  }
  TcpIp_IpV6_StaticIp_State[assignIdx].state = TCPIP_IPV6_STATICIP_UNASSIGNED;
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV6_Static_startAssign
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 method,
  TcpIp_Ndp_dadResult_FctPtrType resultFctPtr
)
{
  const uint8 assignIdx = TcpIp_IpAddrM_getAssignmentIdx(localAddrId, method);

  const uint8 state = TcpIp_IpV6_StaticIp_State[assignIdx].state;

  /* evaluate possible transitions for trigger 'startAssign', based on the current state */
  switch(state)
  {
    case TCPIP_IPV6_STATICIP_UNASSIGNED:
      TcpIp_IpV6_StaticIp_tr_startAssign_UNASSIGNED(localAddrId,assignIdx,method,resultFctPtr);
      break;

    case TCPIP_IPV6_STATICIP_PREFERRED:
      TcpIp_IpV6_StaticIp_tr_startAssign_PREFERRED(localAddrId,assignIdx,method,resultFctPtr);
      break;

#if (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)
#if (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)
    case TCPIP_IPV6_STATICIP_OPTIMISTIC:
      TcpIp_IpV6_StaticIp_tr_startAssign_OPTIMISTIC_OPTIMISTIC(localAddrId,assignIdx,method,resultFctPtr);
      break;
    case TCPIP_IPV6_STATICIP_CONFLICTED:
      /* State can be exited only through initialization */
      break;
#else /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
    case TCPIP_IPV6_STATICIP_TENTATIVE:
      TcpIp_IpV6_StaticIp_tr_startAssign_TENTATIVE_TENTATIVE(localAddrId,assignIdx,method,resultFctPtr);
      break;
#endif /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
#endif /*(TCPIP_NDP_DAD_CONFIG_ENABLED == STD_OFF) */


    /* CHECK: NOPARSE */
    default:
      TcpIp_IpV6_StaticIp_State[assignIdx].state = TCPIP_IPV6_STATICIP_UNASSIGNED;
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break; /* should not happen */
    /* CHECK: PARSE */
  }
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV6_Static_stopAssign
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 method
)
{
  const uint8 assignIdx = TcpIp_IpAddrM_getAssignmentIdx(localAddrId, method);
  const uint8 state = TcpIp_IpV6_StaticIp_State[assignIdx].state;

  /* evaluate possible transitions for trigger 'stopAssign', based on the current state */
  switch(state)
  {
    case TCPIP_IPV6_STATICIP_UNASSIGNED:
      break;

    case TCPIP_IPV6_STATICIP_PREFERRED:
      TcpIp_IpV6_StaticIp_tr_stopAssign_PREFERRED_UNASSIGNED(localAddrId, assignIdx, method);
      break;

#if (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)
#if (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)
    case TCPIP_IPV6_STATICIP_OPTIMISTIC:
      TcpIp_IpV6_StaticIp_tr_stopAssign_OPTIMISTIC_UNASSIGNED(localAddrId, assignIdx, method);
      break;
    case TCPIP_IPV6_STATICIP_CONFLICTED:
      break;
#else /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
    case TCPIP_IPV6_STATICIP_TENTATIVE:
      TcpIp_IpV6_StaticIp_tr_stopAssign_TENTATIVE_UNASSIGNED(localAddrId,assignIdx);
      break;
#endif /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
#endif /*(TCPIP_NDP_DAD_CONFIG_ENABLED == STD_OFF) */

    /* CHECK: NOPARSE */
    default:
      TcpIp_IpV6_StaticIp_State[assignIdx].state = TCPIP_IPV6_STATICIP_UNASSIGNED;
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break; /* should not happen */
    /* CHECK: PARSE */
  }
}

#if (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_dadResult
(
  TcpIp_LocalAddrIdType localAddrId,
  boolean unique,
  uint8 method
)
{
  const uint8 assignIdx = TcpIp_IpAddrM_getAssignmentIdx(localAddrId, method);
  const uint8 state = TcpIp_IpV6_StaticIp_State[assignIdx].state;

  switch(state)
  {

#if (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)
#if (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)
    case TCPIP_IPV6_STATICIP_OPTIMISTIC:
      if(unique)
      {
        TcpIp_IpV6_StaticIp_State[assignIdx].state = TCPIP_IPV6_STATICIP_PREFERRED;
      }
      else
      {
        TcpIp_IpV6_StaticIp_tr_dadResult_OPTIMISTIC_CONFLICTED(localAddrId,assignIdx, method);
      }
      break;
#else /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
    case TCPIP_IPV6_STATICIP_TENTATIVE:
      if(unique)
      {
        TcpIp_IpV6_StaticIp_tr_dadResult_TENTATIVE_PREFERRED(localAddrId, assignIdx, method);
      }
      else
      {
        TcpIp_IpV6_StaticIp_tr_stopAssign_TENTATIVE_UNASSIGNED(localAddrId,assignIdx);
      }
      break;
#endif /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */
#endif /*(TCPIP_NDP_DAD_CONFIG_ENABLED == STD_OFF) */

    /* CHECK: NOPARSE */
    case TCPIP_IPV6_STATICIP_UNASSIGNED:       /* intended fall through */
    case TCPIP_IPV6_STATICIP_PREFERRED:   /* intended fall through */
    case TCPIP_IPV6_STATICIP_CONFLICTED:   /* intended fall through */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }
}

#endif /* (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON) */

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV6_Static_getIpConfig
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) gatewayIpPtrPtr,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) netmaskIpPtrPtr,
  uint8 method
)
{
  const uint8 assignIdx = TcpIp_IpAddrM_getAssignmentIdx(localAddrId, method);
  CONSTP2CONST(TcpIp_localAddrIpV6ConfigType,AUTOMATIC,TCPIP_APPL_CONST) intLocalAddrIndexPtr =
          &(TCPIP_CFG_TYPE(TcpIp_localAddrIpV6ConfigType,intIpV6LocalAddr,assignIdx));

  if (intLocalAddrIndexPtr->staticAddrId != TCPIP_LOCALADDRID_INVALID)
  {
    CONSTP2CONST(TcpIp_localAddrIpV6StaticConfigType,AUTOMATIC,TCPIP_APPL_CONST) intLocalAddrPtr =
      &(TCPIP_CFG_TYPE(TcpIp_localAddrIpV6StaticConfigType,intIPv6StaticAddr,intLocalAddrIndexPtr->staticAddrId));

    *netmaskIpPtrPtr = intLocalAddrPtr->netMaskStatic;
    *gatewayIpPtrPtr = intLocalAddrPtr->routerIpStatic;
  }
  else
  {
    *netmaskIpPtrPtr = TcpIp_IpAddr_unspecified ;
    *gatewayIpPtrPtr = TcpIp_IpAddr_unspecified;
  }

  *addressIpPtrPtr = TcpIp_IpV6_StaticIp_IpAddress[assignIdx];
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV6_Static_getIpAddress
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr,
  uint8 method
)
{
  const uint8 assignIdx = TcpIp_IpAddrM_getAssignmentIdx(localAddrId, method);

  *addressIpPtrPtr = TcpIp_IpV6_StaticIp_IpAddress[assignIdx];
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpV6_Static_requestIpAddress
(
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) LocalIpAddrPtr,
  uint8 method
)
{
  Std_ReturnType result;
  const uint8 assignIdx = TcpIp_IpAddrM_getAssignmentIdx(localAddrId, method);
  CONSTP2CONST(TcpIp_localAddrIpV6ConfigType,AUTOMATIC,TCPIP_APPL_CONST) intLocalAddrIndexPtr =
    &(TCPIP_CFG_TYPE(TcpIp_localAddrIpV6ConfigType,intIpV6LocalAddr,assignIdx));

  if( (intLocalAddrIndexPtr->staticAddrId == TCPIP_LOCALADDRID_INVALID) && (LocalIpAddrPtr == NULL_PTR))
  {
    result = E_NOT_OK;
  }
  else
  {
    P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA) ipAddrPtr =
      /* Deviation MISRAC2012-2 */
      (P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA)) LocalIpAddrPtr;

    if((LocalIpAddrPtr != NULL_PTR) && (!TCPIP_IPV6_ADDR_IS_UNSPECIFIED(ipAddrPtr->addr)))
    {
      TCPIP_IPV6_ADDR_CPY(TcpIp_IpV6_StaticIp_NewIpAddress[assignIdx], ipAddrPtr->addr);
    }

    result = E_OK;
  }

  return result;
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpV6_Static_getIpAddrState
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 method
)
{
  const uint8 assignIdx = TcpIp_IpAddrM_getAssignmentIdx(localAddrId, method);

  return TcpIp_IpV6_StaticIp_State[assignIdx].state;
}

/*==================[internal function definitions]=========================*/

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_Static_dadResult
(
  TcpIp_LocalAddrIdType localAddrId,
  boolean unique
)
{
#if (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)
  TcpIp_IpV6_StaticIp_dadResult(localAddrId, unique, TCPIP_IPADDRM_ASSIGN_STATICV6);
#else
  TS_PARAM_UNUSED(localAddrId);
  TS_PARAM_UNUSED(unique);
#endif /* (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON) */
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_getNewIpAddr
(
  uint8 assignIdx,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) ipAddrPtr
)
{
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_2();

  TCPIP_IPV6_ADDR_CPY(ipAddrPtr, TcpIp_IpV6_StaticIp_NewIpAddress[assignIdx]);

  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_2();
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_assignSolicitMulticast
(
  uint8 assignIdx
)
{
  /* transitional code */
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_CONST) ip = TcpIp_IpV6_StaticIp_IpAddress[assignIdx];

  /* assign solicit multicast ip address */
  CONSTP2CONST(TcpIp_localAddrIpV6ConfigType,AUTOMATIC,TCPIP_APPL_CONST) intLocalAddrPtr =
    &(TCPIP_CFG_TYPE(TcpIp_localAddrIpV6ConfigType,intIpV6LocalAddr,assignIdx));

  TcpIp_SockAddrInet6Type solicitMulticast;
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) solicitMulticastPtr =
    /* Deviation MISRAC2012-2 */
    (P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST)) &solicitMulticast;
  const TcpIp_LocalAddrIdType multicastlocalAddrId = intLocalAddrPtr->solMultilocalAddrId;

  TcpIp_IpV6_getSolicitMulti(solicitMulticast.addr, ip);
  solicitMulticast.domain = TCPIP_AF_INET6;

  /* assign solicited multicast ip address */
  (void) TcpIp_IpAddrM_requestIpAddrAssignment
           (multicastlocalAddrId, TCPIP_IPADDR_ASSIGNMENT_STATIC, solicitMulticastPtr);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_releaseSolicitMulticast
(
  uint8 assignIdx
)
{
  CONSTP2CONST(TcpIp_localAddrIpV6ConfigType,AUTOMATIC,TCPIP_APPL_CONST) intLocalAddrPtr =
    &(TCPIP_CFG_TYPE(TcpIp_localAddrIpV6ConfigType,intIpV6LocalAddr,assignIdx));

  /* unassign solicited-node multicast ip address */
  (void) TcpIp_IpAddrM_releaseIpAddrAssignment(intLocalAddrPtr->solMultilocalAddrId);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_updatePhysAddrFilter
(
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_CONST) ip,
  Eth_FilterActionType Action
)
{
  CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
      &(TCPIP_CFG(localAddr,localAddrId));
  uint8 physMulticastAddr[TCPIP_ETH_ADDR_LENGTH];

  /* exit code */

  /* transitional code */
  TcpIp_IpV6_GetPhysMulticastAddr(physMulticastAddr,ip);
  (void)EthIf_UpdatePhysAddrFilter((TCPIP_CFG(ctrl,localAddrPtr->ctrlIdx)).ethIfCtrlIdx,physMulticastAddr,Action);
}

STATIC FUNC(boolean, TCPIP_CODE) TcpIp_IpV6_StaticIp_ipAddrHasChanged
(
  uint8 assignIdx
)
{
  uint32 ip[TCPIP_IPV6_ADDR_SIZE];

  TcpIp_IpV6_StaticIp_getNewIpAddr(assignIdx, ip);

  return !TCPIP_IPV6_ADDR_CMP(TcpIp_IpV6_StaticIp_IpAddress[assignIdx], ip);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_tr_startAssign_UNASSIGNED
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 assignIdx,
  uint8 method,
  TcpIp_Ndp_dadResult_FctPtrType resultFctPtr
)
{

  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_CONST) ip = TcpIp_IpV6_StaticIp_IpAddress[assignIdx];

  TcpIp_IpV6_StaticIp_getNewIpAddr(assignIdx, TcpIp_IpV6_StaticIp_IpAddress[assignIdx]);

  if(TCPIP_IPV6_ADDR_IS_MULTICAST(ip))
  {
    /* exit code */

    /* transitional code */
    TcpIp_IpV6_StaticIp_updatePhysAddrFilter(localAddrId, ip, ETH_ADD_TO_FILTER);

    /* entry code */
    TcpIp_IpV6_StaticIp_entry_ASSIGNED(localAddrId, method);
    TcpIp_IpV6_StaticIp_State[assignIdx].state = TCPIP_IPV6_STATICIP_PREFERRED;
  }
  else
  {
#if (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_OFF)
    TcpIp_IpV6_StaticIp_tr_startAssign_UNASSIGNED_PREFERRED(localAddrId,assignIdx,method);
    TS_PARAM_UNUSED(resultFctPtr);
#elif (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)
    TcpIp_IpV6_StaticIp_tr_startAssign_UNASSIGNED_OPTIMISTIC(localAddrId,assignIdx,method,resultFctPtr);
#else /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_OFF) */
    TcpIp_IpV6_StaticIp_tr_startAssign_UNASSIGNED_TENTATIVE(localAddrId,assignIdx,resultFctPtr);
#endif /* (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_OFF) */
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_tr_startAssign_PREFERRED
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 assignIdx,
  uint8 method,
  TcpIp_Ndp_dadResult_FctPtrType resultFctPtr
)
{
  if(TcpIp_IpV6_StaticIp_ipAddrHasChanged(assignIdx))
  {
    /* exit code */
    TcpIp_IpV6_StaticIp_exit_ASSIGNED(localAddrId, assignIdx, method);

    /* transitional code */
    TcpIp_IpV6_StaticIp_tr_startAssign_UNASSIGNED(localAddrId, assignIdx, method,resultFctPtr);

    /* entry code */
  }
}


#if (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)

#if (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON)

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_tr_startAssign_UNASSIGNED_OPTIMISTIC
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 assignIdx,
  uint8 method,
  TcpIp_Ndp_dadResult_FctPtrType resultFctPtr
)
{
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_CONST) ip = TcpIp_IpV6_StaticIp_IpAddress[assignIdx];

  /* exit code */

  /* transitional code */
  TcpIp_IpV6_StaticIp_assignSolicitMulticast(assignIdx);

  /* entry code */
  TcpIp_IpV6_StaticIp_entry_ASSIGNED(localAddrId, method);
  TcpIp_Ndp_Dad_start(resultFctPtr, ip, localAddrId);
  TcpIp_IpV6_StaticIp_State[assignIdx].state  = TCPIP_IPV6_STATICIP_OPTIMISTIC;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_tr_startAssign_OPTIMISTIC_OPTIMISTIC
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 assignIdx,
  uint8 method,
  TcpIp_Ndp_dadResult_FctPtrType resultFctPtr
)
{
  if(TcpIp_IpV6_StaticIp_ipAddrHasChanged(assignIdx))
  {
     /* exit code */
     TcpIp_Ndp_Dad_abort(localAddrId);
     TcpIp_IpV6_StaticIp_exit_ASSIGNED(localAddrId, assignIdx, method);

     /* transitional code */
     TcpIp_IpV6_StaticIp_tr_startAssign_UNASSIGNED(localAddrId,assignIdx,method,resultFctPtr);

     /* entry code */
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_tr_stopAssign_OPTIMISTIC_UNASSIGNED
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 assignIdx,
  uint8 method
)
{
  /* exit code */
  TcpIp_Ndp_Dad_abort(localAddrId);
  TcpIp_IpV6_StaticIp_exit_ASSIGNED(localAddrId, assignIdx, method);

    /* transitional code */
  /* entry code */
  TcpIp_IpV6_StaticIp_State[assignIdx].state = TCPIP_IPV6_STATICIP_UNASSIGNED;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_tr_dadResult_OPTIMISTIC_CONFLICTED
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 assignIdx,
  uint8 method
)
{
  /* exit code */
  TcpIp_Ndp_Dad_abort(localAddrId);
  TcpIp_IpV6_StaticIp_exit_ASSIGNED(localAddrId, assignIdx, method);

  /* transitional code */

  /* entry code */
  TcpIp_IpV6_StaticIp_State[assignIdx].state = TCPIP_IPV6_STATICIP_CONFLICTED;
}

#else /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_tr_startAssign_UNASSIGNED_TENTATIVE
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 assignIdx,
  TcpIp_Ndp_dadResult_FctPtrType resultFctPtr
)
{
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_CONST) ip = TcpIp_IpV6_StaticIp_IpAddress[assignIdx];

  /* exit code */

  /* transitional code */
  TcpIp_IpV6_StaticIp_assignSolicitMulticast(assignIdx);

  /* entry code */
  TcpIp_Ndp_Dad_start(resultFctPtr, ip, localAddrId);
  TcpIp_IpV6_StaticIp_State[assignIdx].state  = TCPIP_IPV6_STATICIP_TENTATIVE;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_tr_dadResult_TENTATIVE_PREFERRED
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 assignIdx,
  uint8 method
)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  TcpIp_IpV6_StaticIp_entry_ASSIGNED(localAddrId, method);
  TcpIp_IpV6_StaticIp_State[assignIdx].state  = TCPIP_IPV6_STATICIP_PREFERRED;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_tr_startAssign_TENTATIVE_TENTATIVE
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 assignIdx,
  uint8 method,
  TcpIp_Ndp_dadResult_FctPtrType resultFctPtr
)
{
  if(TcpIp_IpV6_StaticIp_ipAddrHasChanged(assignIdx))
  {
     /* exit code */
     TcpIp_Ndp_Dad_abort(localAddrId);

     /* transitional code */
     TcpIp_IpV6_StaticIp_releaseSolicitMulticast(assignIdx);
     TcpIp_IpV6_StaticIp_tr_startAssign_UNASSIGNED(localAddrId,assignIdx,method,resultFctPtr);

     /* entry code */
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_tr_stopAssign_TENTATIVE_UNASSIGNED
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 assignIdx
)
{
  /* exit code */
  TcpIp_Ndp_Dad_abort(localAddrId);

  /* transitional code */
  TcpIp_IpV6_StaticIp_entry_INACTIVE(localAddrId,assignIdx);

  /* entry code */
  TcpIp_IpV6_StaticIp_State[assignIdx].state = TCPIP_IPV6_STATICIP_UNASSIGNED;
}


#endif /* (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_ON) */

#else /* (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON) */


STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_tr_startAssign_UNASSIGNED_PREFERRED
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 assignIdx,
  uint8 method
)
{
  /* exit code */

  /* transitional code */
  TcpIp_IpV6_StaticIp_assignSolicitMulticast(assignIdx);

  /* entry code */
  TcpIp_IpV6_StaticIp_entry_ASSIGNED(localAddrId, method);
  TcpIp_IpV6_StaticIp_State[assignIdx].state = TCPIP_IPV6_STATICIP_PREFERRED;
}

#endif /* (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_OFF) */

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_tr_stopAssign_PREFERRED_UNASSIGNED
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 assignIdx,
  uint8 method
)
{
  /* exit code */
  TcpIp_IpV6_StaticIp_exit_ASSIGNED(localAddrId, assignIdx, method);

  /* transitional code */

  /* entry code */
  TcpIp_IpV6_StaticIp_State[assignIdx].state = TCPIP_IPV6_STATICIP_UNASSIGNED;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_entry_INACTIVE
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 assignIdx
)
{
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_CONST) ip = TcpIp_IpV6_StaticIp_IpAddress[assignIdx];

  /* reduce ethernet acceptance filter if we assigned a Multicast address */
  if(TCPIP_IPV6_ADDR_IS_MULTICAST(ip))
  {
    TcpIp_IpV6_StaticIp_updatePhysAddrFilter(localAddrId, ip, ETH_REMOVE_FROM_FILTER);
  }
  else
  {
    TcpIp_IpV6_StaticIp_releaseSolicitMulticast(assignIdx);
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_exit_ASSIGNED
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 assignIdx,
  uint8 method
)
{
  TcpIp_IpV6_StaticIp_entry_INACTIVE(localAddrId,assignIdx);

  /* inform the ip address manager that ip address shall be unassigned */
  TcpIp_IpAddrM_AssignSM_unassigned(localAddrId,method);
}


STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV6_StaticIp_entry_ASSIGNED
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 method
)
{
  TcpIp_IpAddrM_AssignSM_assigned(localAddrId, method);
}


#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#else /* (TCPIP_IPV6_ENABLED == STD_ON) */
  TS_PREVENTEMPTYTRANSLATIONUNIT
#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */

/*==================[end of file]===========================================*/
