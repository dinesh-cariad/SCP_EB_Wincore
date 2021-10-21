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

#include <EthIf.h>               /* EthIf API declarations */
#include <SchM_TcpIp.h>          /* SchM API declarations */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_IpAddrM_Int.h>   /* used SW-unit interface file */
#include <TcpIp_IpV4_Priv.h>     /* own SW-unit private header file; must be last */

/*==================[macros]================================================*/

/* static IPv4 assignment state machine states */
#define TCPIP_IPV4_STATICIP_UNASSIGNED 0U /** \brief no IP address is assigned */
#define TCPIP_IPV4_STATICIP_ASSIGNED   1U /** \brief IP address is assigned */

/*==================[type definitions]======================================*/

/** \brief Structure contains state information for each local address */
typedef struct
{
  uint8 state; /**< current local address state */
}TcpIp_IpV4_StaticIp_StateType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#if (TCPIP_IPV4_ENABLED == STD_ON)

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_StaticIp_conflict
(
    TcpIp_LocalAddrIdType localAddrId
);

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_StaticIp_getNewIpAddr
(
    uint8 assignIdx,
    P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) ipAddrPtr
);

/* transitions */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_StaticIp_tr_UNASSIGNED_ASSIGNED(TcpIp_LocalAddrIdType localAddrId, uint8 assignIdx);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_StaticIp_tr_ASSIGNED_UNASSIGNED(TcpIp_LocalAddrIdType localAddrId, uint8 assignIdx);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_StaticIp_tr_self_ASSIGNED(TcpIp_LocalAddrIdType localAddrId, uint8 assignIdx);

/* state entry functions */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_StaticIp_entry_ASSIGNED(TcpIp_LocalAddrIdType localAddrId, uint8 assignIdx);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_StaticIp_entry_UNASSIGNED(TcpIp_LocalAddrIdType localAddrId, uint8 assignIdx);

/* state exit functions */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_StaticIp_exit_ASSIGNED(TcpIp_LocalAddrIdType localAddrId, uint8 assignIdx);

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

#define TCPIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

STATIC TcpIp_IpV4_StaticIp_StateType TcpIp_IpV4_StaticIp_State[TCPIP_NUM_IPV4_LOCALADDRENTRIES];

#define TCPIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

#define TCPIP_START_SEC_VAR_CLEARED_32
#include <TcpIp_MemMap.h>

STATIC uint32 TcpIp_IpV4_StaticIp_IpAddress[TCPIP_NUM_IPV4_LOCALADDRENTRIES];
STATIC uint32 TcpIp_IpV4_StaticIp_NewIpAddress[TCPIP_NUM_IPV4_LOCALADDRENTRIES];

#define TCPIP_STOP_SEC_VAR_CLEARED_32
#include <TcpIp_MemMap.h>

/*==================[external function definitions]=========================*/

/*==================[SW-Unit internal functions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_StaticIp_init(TcpIp_LocalAddrIdType localAddrId)
{
  DBG_TCPIP_IPV4_STATICIP_INIT_ENTRY(localAddrId);

  {
    const uint8 assignIdx = TcpIp_IpAddrM_getAssignmentIdx(localAddrId, TCPIP_IPADDRM_ASSIGN_STATICV4);
    TCPIP_PRECONDITION_ASSERT(assignIdx < TCPIP_NUM_IPV4_LOCALADDRENTRIES, TCPIP_INTERNAL_API_ID);
    {

      const uint32 ip = (TCPIP_CFG_TYPE(uint32, intIpV4LocalAddr, assignIdx));

      TcpIp_IpV4_StaticIp_IpAddress[assignIdx] = ip;
      TcpIp_IpV4_StaticIp_NewIpAddress[assignIdx] = ip;
      TcpIp_IpV4_StaticIp_State[assignIdx].state = TCPIP_IPV4_STATICIP_UNASSIGNED;

    }
  }
  DBG_TCPIP_IPV4_STATICIP_INIT_EXIT(localAddrId);
}
TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_StaticIp_startAssign(TcpIp_LocalAddrIdType localAddrId)
{
  DBG_TCPIP_IPV4_STATICIP_STARTASSIGN_ENTRY(localAddrId);

  {
    const uint8 assignIdx = TcpIp_IpAddrM_getAssignmentIdx(localAddrId, TCPIP_IPADDRM_ASSIGN_STATICV4);
    TCPIP_PRECONDITION_ASSERT(assignIdx < TCPIP_NUM_IPV4_LOCALADDRENTRIES, TCPIP_INTERNAL_API_ID);
    {
      const uint8 state = TcpIp_IpV4_StaticIp_State[assignIdx].state;

      /* evaluate possible transitions for trigger 'startAssign', based on the current state */
      switch (state)
      {
      case TCPIP_IPV4_STATICIP_UNASSIGNED:
        TcpIp_IpV4_StaticIp_tr_UNASSIGNED_ASSIGNED(localAddrId, assignIdx);
        break;

      case TCPIP_IPV4_STATICIP_ASSIGNED:
        TcpIp_IpV4_StaticIp_tr_self_ASSIGNED(localAddrId, assignIdx);
        break;

        /* CHECK: NOPARSE */
      default:
        TcpIp_IpV4_StaticIp_State[assignIdx].state = TCPIP_IPV4_STATICIP_UNASSIGNED;
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break; /* should not happen */
      /* CHECK: PARSE */
      }
    }
  }
  DBG_TCPIP_IPV4_STATICIP_STARTASSIGN_EXIT(localAddrId);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_StaticIp_stopAssign(TcpIp_LocalAddrIdType localAddrId)
{
  DBG_TCPIP_IPV4_STATICIP_STOPASSIGN_ENTRY(localAddrId);

  {
    const uint8 assignIdx = TcpIp_IpAddrM_getAssignmentIdx(localAddrId, TCPIP_IPADDRM_ASSIGN_STATICV4);
    TCPIP_PRECONDITION_ASSERT(assignIdx < TCPIP_NUM_IPV4_LOCALADDRENTRIES, TCPIP_INTERNAL_API_ID);
    {
      const uint8 state = TcpIp_IpV4_StaticIp_State[assignIdx].state;

      /* evaluate possible transitions for trigger 'stopAssign', based on the current state */
      switch (state)
      {
      case TCPIP_IPV4_STATICIP_ASSIGNED:
        TcpIp_IpV4_StaticIp_tr_ASSIGNED_UNASSIGNED(localAddrId, assignIdx);
        break;

      case TCPIP_IPV4_STATICIP_UNASSIGNED:
        break;

        /* CHECK: NOPARSE */
      default:
        TcpIp_IpV4_StaticIp_State[assignIdx].state = TCPIP_IPV4_STATICIP_UNASSIGNED;
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break; /* should not happen */
      /* CHECK: PARSE */
      }
    }
  }
  DBG_TCPIP_IPV4_STATICIP_STOPASSIGN_EXIT(localAddrId);
}
TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_StaticIp_getIpConfig
(
    TcpIp_LocalAddrIdType localAddrId,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) gatewayIpPtrPtr,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) netmaskIpPtrPtr
)
{
  DBG_TCPIP_IPV4_STATICIP_GETIPCONFIG_ENTRY(localAddrId,addressIpPtrPtr,gatewayIpPtrPtr,netmaskIpPtrPtr);

  {
    CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
        &(TCPIP_CFG(localAddr,localAddrId));

    CONSTP2CONST(TcpIp_ctrlIpV4ConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlPtr =
        &((TCPIP_CFG(ctrl,localAddrPtr->ctrlIdx)).ctrlIpV4);

    const uint8 assignIdx = TcpIp_IpAddrM_getAssignmentIdx(localAddrId,TCPIP_IPADDRM_ASSIGN_STATICV4);
    TCPIP_PRECONDITION_ASSERT(assignIdx < TCPIP_NUM_IPV4_LOCALADDRENTRIES, TCPIP_INTERNAL_API_ID);

    *addressIpPtrPtr = &TcpIp_IpV4_StaticIp_IpAddress[assignIdx];
    *gatewayIpPtrPtr = &ctrlPtr->defaultGWIpStatic;
    *netmaskIpPtrPtr = &ctrlPtr->netMaskStatic;
  }

  DBG_TCPIP_IPV4_STATICIP_GETIPCONFIG_EXIT(localAddrId,addressIpPtrPtr,gatewayIpPtrPtr,netmaskIpPtrPtr);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_StaticIp_getIpAddress
(
    TcpIp_LocalAddrIdType localAddrId,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr
)
{
  DBG_TCPIP_IPV4_STATICIP_GETIPADDRESS_ENTRY(localAddrId,addressIpPtrPtr);

  {
    const uint8 assignIdx = TcpIp_IpAddrM_getAssignmentIdx(localAddrId,TCPIP_IPADDRM_ASSIGN_STATICV4);
    TCPIP_PRECONDITION_ASSERT(assignIdx < TCPIP_NUM_IPV4_LOCALADDRENTRIES, TCPIP_INTERNAL_API_ID);

    *addressIpPtrPtr = &TcpIp_IpV4_StaticIp_IpAddress[assignIdx];
  }

  DBG_TCPIP_IPV4_STATICIP_GETIPADDRESS_EXIT(localAddrId,addressIpPtrPtr);
}

TS_MOD_PRIV_DEFN FUNC(boolean, TCPIP_CODE) TcpIp_IpV4_StaticIp_isAssigned
(
    TcpIp_LocalAddrIdType localAddrId
)
{
  boolean result;

  DBG_TCPIP_IPV4_STATICIP_ISASSIGNED_ENTRY(localAddrId);

  {
    const uint8 assignIdx = TcpIp_IpAddrM_getAssignmentIdx(localAddrId,TCPIP_IPADDRM_ASSIGN_STATICV4);
    const uint8 state = TcpIp_IpV4_StaticIp_State[assignIdx].state;

    result = (state == TCPIP_IPV4_STATICIP_ASSIGNED) ? TRUE : FALSE;
  }

  DBG_TCPIP_IPV4_STATICIP_ISASSIGNED_EXIT(result, localAddrId);

  return result;
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_StaticIp_assignmentActive
(
    TcpIp_LocalAddrIdType localAddrId,
    boolean state
)
{
  DBG_TCPIP_IPV4_STATICIP_ASSIGNMENTACTIVE_ENTRY(localAddrId,state);

  {
    CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
        &(TCPIP_CFG(localAddr,localAddrId));

    TCPIP_PRECONDITION_ASSERT( TcpIp_PBcfgPtr != NULL_PTR, TCPIP_INTERNAL_API_ID );
    if(TCPIP_IPADDRM_IS_ADDRTYPE(localAddrPtr->addrType,UNICAST))
    {
      const uint8 numAnnounce = TcpIp_PBcfgPtr->ipV4Config.numGratuitousARP;

      if(numAnnounce > 0U)
      {
        if(state)
        {
          const uint8 assignIdx =
            TcpIp_IpAddrM_getAssignmentIdx(localAddrId,TCPIP_IPADDRM_ASSIGN_STATICV4);

          /* inform others about assigned ip address by transmitting arp announces */
          TcpIp_IpV4_ArpAnnounce
            (TcpIp_IpV4_StaticIp_IpAddress[assignIdx], numAnnounce, 1U, localAddrPtr->ctrlIdx, TCPIP_IPADDRM_ASSIGN_STATICV4);
        }
        else
        {
          /* stop announcing ip address */
          TcpIp_IpV4_ArpAnnounce(TCPIP_IPV4_ADDR_INVALID_VALUE, 0U, 0U, localAddrPtr->ctrlIdx, TCPIP_IPADDRM_ASSIGN_STATICV4);
        }
      }

      {
        if(state)
        {
          const uint8 addrDefend = TcpIp_PBcfgPtr->ipV4Config.addrDefendMechanism;

          /* listen for address conflicts */
          TcpIp_IpV4_AddrConflDetect_activate
            (localAddrId, &TcpIp_IpV4_StaticIp_conflict, addrDefend);
        }
        else
        {
          /* stop listen for ARP conflicts */
          TcpIp_IpV4_AddrConflDetect_activate
            (localAddrId, NULL_PTR, TCPIP_IPV4_ADDR_CONFL_DETECT_MECHANISM_OFF);
        }
      }
    }
  }
  DBG_TCPIP_IPV4_STATICIP_ASSIGNMENTACTIVE_EXIT(localAddrId,state);
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpV4_StaticIp_requestIpAddress
(
    TcpIp_LocalAddrIdType localAddrId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) LocalIpAddrPtr
)
{
  Std_ReturnType result;
  const uint8 assignIdx = TcpIp_IpAddrM_getAssignmentIdx(localAddrId, TCPIP_IPADDRM_ASSIGN_STATICV4);
  DBG_TCPIP_IPV4_STATICIP_REQUESTIPADDRESS_ENTRY(localAddrId, LocalIpAddrPtr);
  TCPIP_PRECONDITION_ASSERT(assignIdx < TCPIP_NUM_IPV4_LOCALADDRENTRIES, TCPIP_INTERNAL_API_ID);
  {
    const uint32 ip = (TCPIP_CFG_TYPE(uint32, intIpV4LocalAddr, assignIdx));



    if ((ip == TCPIP_IPV4_ADDR_INVALID_VALUE) && (LocalIpAddrPtr == NULL_PTR))
    {
      result = E_NOT_OK;
    }
    else
    {
      /* Deviation MISRAC2012-2 */
      P2CONST(TcpIp_SockAddrInetType, AUTOMATIC, TCPIP_APPL_DATA) ipAddrPtr =
        (P2CONST(TcpIp_SockAddrInetType, AUTOMATIC, TCPIP_APPL_DATA)) LocalIpAddrPtr;

      if ((LocalIpAddrPtr != NULL_PTR) && (ipAddrPtr->addr[0] != TCPIP_IPV4_ADDR_INVALID_VALUE))
      {
        TcpIp_IpV4_StaticIp_NewIpAddress[assignIdx] = ipAddrPtr->addr[0];
      }

      result = E_OK;
    }
  }
  DBG_TCPIP_IPV4_STATICIP_REQUESTIPADDRESS_EXIT(result, localAddrId, LocalIpAddrPtr);
  return result;
}
/*==================[internal function definitions]=========================*/

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_StaticIp_conflict
(
    TcpIp_LocalAddrIdType localAddrId
)
{
  {
    TCPIP_PBCFG_PRECONDITION_ASSERT(localAddrId, localAddrCfg, TCPIP_INTERNAL_API_ID);
    {
      const uint8 assignIdx = TcpIp_IpAddrM_getAssignmentIdx(localAddrId, TCPIP_IPADDRM_ASSIGN_STATICV4);
      TCPIP_PRECONDITION_ASSERT(assignIdx < TCPIP_NUM_IPV4_LOCALADDRENTRIES, TCPIP_INTERNAL_API_ID);
      {
        const uint8 state = TcpIp_IpV4_StaticIp_State[assignIdx].state;

        /* evaluate possible transitions for trigger 'conflict', based on the current state */
        switch (state)
        {
        case TCPIP_IPV4_STATICIP_ASSIGNED:
          TcpIp_IpV4_StaticIp_tr_ASSIGNED_UNASSIGNED(localAddrId, assignIdx);
          break;

          /* CHECK: NOPARSE */
        case TCPIP_IPV4_STATICIP_UNASSIGNED:
        default:
          TcpIp_IpV4_StaticIp_State[assignIdx].state = TCPIP_IPV4_STATICIP_UNASSIGNED;
          TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
          break; /* should not happen */
          /* CHECK: PARSE */
        }
      }
    }
  }
}
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_StaticIp_getNewIpAddr
(
    uint8 assignIdx,
    P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) ipAddrPtr
)
{
  /* !LINKSTO TcpIp.Design.IpV4.EA2.mainFunction,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_2();

  *ipAddrPtr = TcpIp_IpV4_StaticIp_NewIpAddress[assignIdx];

  /* !LINKSTO TcpIp.Design.IpV4.EA2.mainFunction,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_2();
}

/* transitions */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_StaticIp_tr_UNASSIGNED_ASSIGNED(TcpIp_LocalAddrIdType localAddrId, uint8 assignIdx)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  TcpIp_IpV4_StaticIp_entry_ASSIGNED(localAddrId,assignIdx);
}



STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_StaticIp_tr_ASSIGNED_UNASSIGNED(TcpIp_LocalAddrIdType localAddrId, uint8 assignIdx)
{
  /* exit code */
  TcpIp_IpV4_StaticIp_exit_ASSIGNED(localAddrId,assignIdx);

  /* transitional code */

  /* entry code */
  TcpIp_IpV4_StaticIp_entry_UNASSIGNED(localAddrId,assignIdx);
}


STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_StaticIp_tr_self_ASSIGNED(TcpIp_LocalAddrIdType localAddrId, uint8 assignIdx)
{
  uint32 ip;

  TcpIp_IpV4_StaticIp_getNewIpAddr(assignIdx, &ip);

  /* transitional code */
  if(TcpIp_IpV4_StaticIp_IpAddress[assignIdx] != ip)
  {
    TcpIp_IpV4_StaticIp_exit_ASSIGNED(localAddrId, assignIdx);
    TcpIp_IpV4_StaticIp_entry_ASSIGNED(localAddrId, assignIdx);
  }
}


/* state entry functions */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_StaticIp_entry_ASSIGNED(TcpIp_LocalAddrIdType localAddrId, uint8 assignIdx)
{
  TCPIP_PRECONDITION_ASSERT(assignIdx < TCPIP_NUM_IPV4_LOCALADDRENTRIES, TCPIP_INTERNAL_API_ID);
  TcpIp_IpV4_StaticIp_State[assignIdx].state = TCPIP_IPV4_STATICIP_ASSIGNED;

  TcpIp_IpV4_StaticIp_getNewIpAddr(assignIdx, &TcpIp_IpV4_StaticIp_IpAddress[assignIdx]);

  TcpIp_IpAddrM_AssignSM_assigned(localAddrId,TCPIP_IPADDRM_ASSIGN_STATICV4);

  {
    CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
        &(TCPIP_CFG(localAddr,localAddrId));

    const uint32 ip = TcpIp_IpV4_StaticIp_IpAddress[assignIdx];

    /* extend ethernet acceptance filter if we assigned a Multicast address */
    if((ip&COMSTACK_CT_HTON_UINT32(TCPIP_IPV4_ADDR_MULTICAST_MASK)) ==
           COMSTACK_CT_HTON_UINT32(TCPIP_IPV4_ADDR_MULTICAST_VALUE))
    {
      uint8 physMulticastAddr[TCPIP_ETH_ADDR_LENGTH];
      TcpIp_IpV4_GetPhysMulticastAddr(physMulticastAddr, (P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA)) &ip);
      (void) EthIf_UpdatePhysAddrFilter((TCPIP_CFG(ctrl,localAddrPtr->ctrlIdx)).ethIfCtrlIdx,physMulticastAddr,ETH_ADD_TO_FILTER);
    }
  }
}



STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_StaticIp_entry_UNASSIGNED(TcpIp_LocalAddrIdType localAddrId, uint8 assignIdx)
{
  TCPIP_PRECONDITION_ASSERT(assignIdx < TCPIP_NUM_IPV4_LOCALADDRENTRIES, TCPIP_INTERNAL_API_ID);
  TcpIp_IpV4_StaticIp_State[assignIdx].state = TCPIP_IPV4_STATICIP_UNASSIGNED;
  TcpIp_IpAddrM_AssignSM_unassigned(localAddrId,TCPIP_IPADDRM_ASSIGN_STATICV4);
}



/* state exit functions */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_StaticIp_exit_ASSIGNED(TcpIp_LocalAddrIdType localAddrId, uint8 assignIdx)
{
  const uint32 ip = TcpIp_IpV4_StaticIp_IpAddress[assignIdx];

  /* reduce ethernet acceptance filter if we assigned a Multicast address */
  if((ip&COMSTACK_CT_HTON_UINT32(TCPIP_IPV4_ADDR_MULTICAST_MASK)) ==
         COMSTACK_CT_HTON_UINT32(TCPIP_IPV4_ADDR_MULTICAST_VALUE))
  {
    CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
        &(TCPIP_CFG(localAddr,localAddrId));

    uint8 physMulticastAddr[TCPIP_ETH_ADDR_LENGTH];
    TcpIp_IpV4_GetPhysMulticastAddr(physMulticastAddr, (P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA)) &ip);
    (void) EthIf_UpdatePhysAddrFilter((TCPIP_CFG(ctrl,localAddrPtr->ctrlIdx)).ethIfCtrlIdx,physMulticastAddr,ETH_REMOVE_FROM_FILTER);
  }
}


#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#else
  TS_PREVENTEMPTYTRANSLATIONUNIT
#endif /* (TCPIP_IPV4_ENABLED == STD_ON) */

/*==================[end of file]===========================================*/
