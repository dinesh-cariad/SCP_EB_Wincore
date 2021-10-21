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
 * MISRAC2012-1) Deviated Rule: 5.5 (required)
 * Identifiers shall be distinct from macro names.
 *
 * Reason:
 * For easier associating design transitions and transitions in the source code.
 *
 */

/*==================[inclusions]============================================*/
#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_IpV4_Int.h>       /* used SW-unit interface file */
#include <TcpIp_IpV6_Int.h>       /* used SW-unit interface file */
#include <TcpIp_DhcpV4_Int.h>     /* used SW-unit interface file */
#include <TcpIp_DhcpV6_Int.h>     /* used SW-unit interface file */
#include <TcpIp_Ndp_Int.h>        /* used SW-unit interface file */
#include <TcpIp_IpAddrM_Priv.h>   /* own SW-unit private header file */

/*==================[macros]================================================*/

/* static IPv4 assignment state machine states */
#define TCPIP_IPADDRM_ASSIGNSM_INACTIVE  0U /** \brief controller stopped, assignment not requested */
#define TCPIP_IPADDRM_ASSIGNSM_STARTED   1U /** \brief controller started, assignment not requested */
#define TCPIP_IPADDRM_ASSIGNSM_REQUESTED 2U /** \brief controller stopped, assignment requested */
#define TCPIP_IPADDRM_ASSIGNSM_ACTIVE    3U /** \brief controller started, assignment requested */

/*==================[type definitions]======================================*/

/** \brief Structure contains state information for each local address */
typedef struct
{
  uint8 state; /**< current local address state */
}TcpIp_IpAddrM_AssignSM_StateType;

typedef struct
{
  TcpIp_IpAddrM_AssignSM_init_FctPtrType initFctPtr;
  TcpIp_IpAddrM_AssignSM_startAssign_FctPtrType startAssignFctPtr;
  TcpIp_IpAddrM_AssignSM_stopAssign_FctPtrType stopAssignFctPtr;
  TcpIp_IpAddrM_AssignSM_getIpConfig_FctPtrType getIpConfigFctPtr;
  TcpIp_IpAddrM_AssignSM_getIpAddress_FctPtrType getIpAddressFctPtr;
  TcpIp_IpAddrM_AssignSM_isAssigned_FctPtrType isAssignedFctPtr;
  TcpIp_IpAddrM_AssignSM_assignmentActive_FctPtrType assignmentActiveFctPtr;
  TcpIp_IpAddrM_AssignSM_requestIpAddress_FctPtrType requestIpAddressFctPtr;
#if (TCPIP_IPV6_ENABLED == STD_ON)
  TcpIp_IpAddrM_AssignSM_getIpAddrState_FctPtrType getIpAddrState_FctPtr;
#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */
}TcpIp_IpAddrM_AssignSM_FctPtrType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/* transitions */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_tr_INACTIVE_ACTIVE(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_tr_INACTIVE_STARTED(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_tr_REQUESTED_ACTIVE(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_tr_STARTED_INACTIVE(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_tr_STARTED_STARTED(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_tr_ACTIVE_REQUESTED(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_tr_INACTIVE_REQUESTED(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_tr_STARTED_ACTIVE(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_tr_REQUESTED_INACTIVE(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_tr_ACTIVE_STARTED(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_tr_self_ACTIVE(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method);

#define TcpIp_IpAddrM_AssignSM_tr_unassigned_self_ACTIVE TcpIp_IpAddrM_AssignSM_tr_unassigned_self
#define TcpIp_IpAddrM_AssignSM_tr_unassigned_self_STARTED TcpIp_IpAddrM_AssignSM_tr_unassigned_self
#define TcpIp_IpAddrM_AssignSM_tr_unassigned_self_INACTIVE TcpIp_IpAddrM_AssignSM_tr_unassigned_self
#define TcpIp_IpAddrM_AssignSM_tr_unassigned_self_REQUESTED TcpIp_IpAddrM_AssignSM_tr_unassigned_self

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_tr_unassigned_self(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method);

/* state entry functions */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_entry_INACTIVE(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_entry_STARTED(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_entry_REQUESTED(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_entry_ACTIVE(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method);

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>
/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

#define TCPIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

STATIC VAR (TcpIp_IpAddrM_AssignSM_StateType, TCPIP_VAR_CLEARED) TcpIp_IpAddrM_AssignSM_State[TCPIP_NUM_LOCALADDRENTRIES][TCPIP_IPADDRM_ASSIGN_NUM];

#define TCPIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

#define TCPIP_START_SEC_CONST_UNSPECIFIED
#include <TcpIp_MemMap.h>

STATIC CONST (TcpIp_IpAddrM_AssignSM_FctPtrType, TCPIP_CONST) TcpIp_IpAddrM_AssignSM_FctPtrTable[TCPIP_IPADDRM_ASSIGN_NUM] =
{
  { /* TCPIP_IPADDR_ASSIGNMENT_STATIC */
#if (TCPIP_IPV4_ENABLED == STD_ON)
    &TcpIp_IpV4_StaticIp_init,
    &TcpIp_IpV4_StaticIp_startAssign,
    &TcpIp_IpV4_StaticIp_stopAssign,
    &TcpIp_IpV4_StaticIp_getIpConfig,
    &TcpIp_IpV4_StaticIp_getIpAddress,
    &TcpIp_IpV4_StaticIp_isAssigned,
    &TcpIp_IpV4_StaticIp_assignmentActive,
    &TcpIp_IpV4_StaticIp_requestIpAddress,
#if (TCPIP_IPV6_ENABLED == STD_ON)
    NULL_PTR
#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */
#else /* (TCPIP_IPV4_ENABLED == STD_ON) */
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
#if (TCPIP_IPV6_ENABLED == STD_ON)
    NULL_PTR
#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */
#endif /* (TCPIP_IPV4_ENABLED == STD_ON) */
  },
  { /* TCPIP_IPADDR_ASSIGNMENT_LINKLOCAL */
#if (TCPIP_IPV4_LINK_LOCAL_ENABLED == STD_ON)
    &TcpIp_IpV4_AutoIp_init,
    &TcpIp_IpV4_AutoIp_startAssign,
    &TcpIp_IpV4_AutoIp_stopAssign,
    &TcpIp_IpV4_AutoIp_getIpConfig,
    &TcpIp_IpV4_AutoIp_getIpAddress,
    &TcpIp_IpV4_AutoIp_isAssigned,
    &TcpIp_IpV4_AutoIp_assignmentActive,
    &TcpIp_IpV4_AutoIp_requestIpAddress,
#if (TCPIP_IPV6_ENABLED == STD_ON)
    NULL_PTR
#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */
#else /* (TCPIP_IPV4_LINK_LOCAL_ENABLED == STD_ON) */
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
#if (TCPIP_IPV6_ENABLED == STD_ON)
    NULL_PTR
#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */
#endif /* (TCPIP_IPV4_LINK_LOCAL_ENABLED == STD_ON) */
  },
  { /* TCPIP_IPADDR_ASSIGNMENT_LINKLOCAL_DOIP */
#if (TCPIP_IPV4_LINK_LOCAL_ENABLED == STD_ON)
    &TcpIp_IpV4_AutoIp_DoIp_init,
    &TcpIp_IpV4_AutoIp_DoIp_startAssign,
    &TcpIp_IpV4_AutoIp_DoIp_stopAssign,
    &TcpIp_IpV4_AutoIp_DoIp_getIpConfig,
    &TcpIp_IpV4_AutoIp_DoIp_getIpAddress,
    &TcpIp_IpV4_AutoIp_DoIp_isAssigned,
    &TcpIp_IpV4_AutoIp_DoIp_assignmentActive,
    &TcpIp_IpV4_AutoIp_DoIp_requestIpAddress,
#if (TCPIP_IPV6_ENABLED == STD_ON)
    NULL_PTR
#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */
#else /* (TCPIP_IPV4_LINK_LOCAL_ENABLED == STD_ON) */
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
#if (TCPIP_IPV6_ENABLED == STD_ON)
    NULL_PTR
#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */
#endif /* (TCPIP_IPV4_LINK_LOCAL_ENABLED == STD_ON) */
  },
  { /* TCPIP_IPADDR_ASSIGNMENT_DHCP */
#if (TCPIP_DHCPV4_CLIENT_ENABLED == STD_ON)
    &TcpIp_DhcpV4_init,
    &TcpIp_DhcpV4_startAssign,
    &TcpIp_DhcpV4_stopAssign,
    &TcpIp_DhcpV4_getIpConfig,
    &TcpIp_DhcpV4_getIpAddress,
    &TcpIp_DhcpV4_isAssigned,
    &TcpIp_DhcpV4_assignmentActive,
    &TcpIp_DhcpV4_requestIpAddress,
#if (TCPIP_IPV6_ENABLED == STD_ON)
    NULL_PTR
#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */
#else /* (TCPIP_DHCPV4_CLIENT_ENABLED == STD_ON) */
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
#if (TCPIP_IPV6_ENABLED == STD_ON)
    NULL_PTR
#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */
#endif /* (TCPIP_DHCPV4_CLIENT_ENABLED == STD_ON) */
  },
  { /* TCPIP_IPADDR_ASSIGNMENT_STATICV6 */
#if (TCPIP_IPV6_ENABLED == STD_ON)
    &TcpIp_IpV6_StaticIp_init,
    &TcpIp_IpV6_StaticIp_startAssign,
    &TcpIp_IpV6_StaticIp_stopAssign,
    &TcpIp_IpV6_StaticIp_getIpConfig,
    &TcpIp_IpV6_StaticIp_getIpAddress,
    NULL_PTR,
    &TcpIp_IpV6_StaticIp_assignmentActive,
    &TcpIp_IpV6_StaticIp_requestIpAddress,
    &TcpIp_IpV6_StaticIp_getIpAddrState
#else /* (TCPIP_IPV6_ENABLED == STD_ON) */
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR
#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */
  },
  { /* TCPIP_IPADDR_ASSIGNMENT_AUTOIPV6 */
#if (TCPIP_IPV6_LINK_LOCAL_ENABLED == STD_ON)
    &TcpIp_IpV6_LinkLocal_init,
    &TcpIp_IpV6_LinkLocal_startAssign,
    &TcpIp_IpV6_LinkLocal_stopAssign,
    &TcpIp_IpV6_LinkLocal_getIpConfig,
    &TcpIp_IpV6_LinkLocal_getIpAddress,
    NULL_PTR,
    &TcpIp_IpV6_LinkLocal_assignmentActive,
    &TcpIp_IpV6_LinkLocal_requestIpAddress,
    &TcpIp_IpV6_LinkLocal_getIpAddrState
#else /* (TCPIP_IPV6_LINK_LOCAL_ENABLED == STD_ON) */
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
#if (TCPIP_IPV6_ENABLED == STD_ON)
    NULL_PTR
#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */
#endif /* (TCPIP_IPV6_LINK_LOCAL_ENABLED == STD_ON) */
  },
  { /* TCPIP_IPADDRM_ASSIGN_DHCPV6 */
#if (TCPIP_DHCPV6_CLIENT_ENABLED == STD_ON)
    &TcpIp_DhcpV6_init,
    &TcpIp_DhcpV6_startAssign,
    &TcpIp_DhcpV6_stopAssign,
    &TcpIp_DhcpV6_getIpConfig,
    &TcpIp_DhcpV6_getIpAddress,
    NULL_PTR,
    &TcpIp_DhcpV6_assignmentActive,
    &TcpIp_DhcpV6_requestIpAddress,
    &TcpIp_DhcpV6_getIpAddrState
#else /* (TCPIP_DHCPV6_CLIENT_ENABLED == STD_ON) */
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
#if (TCPIP_IPV6_ENABLED == STD_ON)
    NULL_PTR
#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */
#endif /* (TCPIP_DHCPV6_CLIENT_ENABLED == STD_ON) */
  },
  { /* TCPIP_IPADDRM_ASSIGN_IPV6_ROUTER */
#if (TCPIP_NDP_RPD_ENABLED == STD_ON)
    &TcpIp_Ndp_Rpd_RouterAssign_init,
    &TcpIp_Ndp_Rpd_RouterAssign_startAssign,
    &TcpIp_Ndp_Rpd_RouterAssign_stopAssign,
    &TcpIp_Ndp_Rpd_RouterAssign_getIpConfig,
    &TcpIp_Ndp_Rpd_RouterAssign_getIpAddress,
    NULL_PTR,
    &TcpIp_Ndp_Rpd_RouterAssign_assignmentActive,
    &TcpIp_Ndp_Rpd_RouterAssign_requestIpAddress,
    &TcpIp_Ndp_Rpd_RouterAssign_getIpAddrState
#else /* (TCPIP_NDP_RPD_ENABLED == STD_ON) */
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
    NULL_PTR,
#if (TCPIP_IPV6_ENABLED == STD_ON)
    NULL_PTR
#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */
#endif /* (TCPIP_NDP_RPD_ENABLED == STD_ON) */
  }
};

#define TCPIP_STOP_SEC_CONST_UNSPECIFIED
#include <TcpIp_MemMap.h>
/*==================[external function definitions]=========================*/

/*==================[SW-Unit internal functions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_init(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method)
{
  TCPIP_PRECONDITION_ASSERT(method < TCPIP_IPADDRM_ASSIGN_NUM, TCPIP_INTERNAL_API_ID);
  TcpIp_IpAddrM_AssignSM_State[localAddrId][method].state = TCPIP_IPADDRM_ASSIGNSM_INACTIVE;
  (*TcpIp_IpAddrM_AssignSM_FctPtrTable[method].initFctPtr)(localAddrId);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_startAssign(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method)
{
  TCPIP_PRECONDITION_ASSERT(method < TCPIP_IPADDRM_ASSIGN_NUM, TCPIP_INTERNAL_API_ID);
  {
    const uint8 state = TcpIp_IpAddrM_AssignSM_State[localAddrId][method].state;

    /* evaluate possible transitions for trigger 'startAssign', based on the current state */
    switch (state)
    {
      case TCPIP_IPADDRM_ASSIGNSM_INACTIVE:
      {
        if (TcpIp_IpAddrM_autoAssignment(localAddrId, method))
        {
          TcpIp_IpAddrM_AssignSM_tr_INACTIVE_ACTIVE(localAddrId, method);
        }
        else
        {
          TcpIp_IpAddrM_AssignSM_tr_INACTIVE_STARTED(localAddrId, method);
        }
        break;
      }
      case TCPIP_IPADDRM_ASSIGNSM_REQUESTED:
        TcpIp_IpAddrM_AssignSM_tr_REQUESTED_ACTIVE(localAddrId, method);
        break;

        /* CHECK: NOPARSE */
      case TCPIP_IPADDRM_ASSIGNSM_STARTED: /* intended fall-through */
      case TCPIP_IPADDRM_ASSIGNSM_ACTIVE:
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break; /* should not happen */

      default:
        TcpIp_IpAddrM_AssignSM_State[localAddrId][method].state = TCPIP_IPADDRM_ASSIGNSM_INACTIVE;
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break; /* should not happen */
      /* CHECK: PARSE */
    }
  }
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_stopAssign(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method)
{
  TCPIP_PRECONDITION_ASSERT(method < TCPIP_IPADDRM_ASSIGN_NUM, TCPIP_INTERNAL_API_ID);
  {
    const uint8 state = TcpIp_IpAddrM_AssignSM_State[localAddrId][method].state;

    /* evaluate possible transitions for trigger 'stopAssign', based on the current state */
    switch (state)
    {
      case TCPIP_IPADDRM_ASSIGNSM_STARTED:
        TcpIp_IpAddrM_AssignSM_tr_STARTED_INACTIVE(localAddrId, method);
        break;

      case TCPIP_IPADDRM_ASSIGNSM_ACTIVE:
        TcpIp_IpAddrM_AssignSM_tr_ACTIVE_REQUESTED(localAddrId, method);
        break;

      /* CHECK: NOPARSE */
      case TCPIP_IPADDRM_ASSIGNSM_INACTIVE:
      case TCPIP_IPADDRM_ASSIGNSM_REQUESTED:
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break; /* should not happen */

      default:
        TcpIp_IpAddrM_AssignSM_State[localAddrId][method].state = TCPIP_IPADDRM_ASSIGNSM_INACTIVE;
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break; /* should not happen */
      /* CHECK: PARSE */
    }
  }
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_requestAssign(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method)
{
  const uint8 state = TcpIp_IpAddrM_AssignSM_State[localAddrId][method].state;

  /* evaluate possible transitions for trigger 'requestAssign', based on the current state */
  switch(state)
  {
    case TCPIP_IPADDRM_ASSIGNSM_INACTIVE:
      TcpIp_IpAddrM_AssignSM_tr_INACTIVE_REQUESTED(localAddrId,method);
      break;

    case TCPIP_IPADDRM_ASSIGNSM_STARTED:
      TcpIp_IpAddrM_AssignSM_tr_STARTED_ACTIVE(localAddrId,method);
      break;

    case TCPIP_IPADDRM_ASSIGNSM_ACTIVE:
      TcpIp_IpAddrM_AssignSM_tr_self_ACTIVE(localAddrId,method);
      break;

    case TCPIP_IPADDRM_ASSIGNSM_REQUESTED: /* intended fall through */
      break; /* ignore */

    /* CHECK: NOPARSE */
    default:
      TcpIp_IpAddrM_AssignSM_State[localAddrId][method].state = TCPIP_IPADDRM_ASSIGNSM_INACTIVE;
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break; /* should not happen */
    /* CHECK: PARSE */
  }
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_releaseAssign(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method)
{

  TCPIP_PRECONDITION_ASSERT(method < TCPIP_IPADDRM_ASSIGN_NUM, TCPIP_INTERNAL_API_ID);
  {
    const uint8 state = TcpIp_IpAddrM_AssignSM_State[localAddrId][method].state;

    /* evaluate possible transitions for trigger 'releaseAssign', based on the current state */
    switch(state)
    {
      case TCPIP_IPADDRM_ASSIGNSM_REQUESTED:
        TcpIp_IpAddrM_AssignSM_tr_REQUESTED_INACTIVE(localAddrId,method);
        break;

      case TCPIP_IPADDRM_ASSIGNSM_ACTIVE:
        TcpIp_IpAddrM_AssignSM_tr_ACTIVE_STARTED(localAddrId,method);
        break;

      case TCPIP_IPADDRM_ASSIGNSM_STARTED:
        TcpIp_IpAddrM_AssignSM_tr_STARTED_STARTED(localAddrId,method);
        break;

      case TCPIP_IPADDRM_ASSIGNSM_INACTIVE:
        break; /* ignore */

      /* CHECK: NOPARSE */
      default:
        TcpIp_IpAddrM_AssignSM_State[localAddrId][method].state = TCPIP_IPADDRM_ASSIGNSM_INACTIVE;
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break; /* should not happen */
      /* CHECK: PARSE */
    }
  }
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_unassigned(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method)
{
  const uint8 state = TcpIp_IpAddrM_AssignSM_State[localAddrId][method].state;

  DBG_TCPIP_IPADDRM_ASSIGNSM_UNASSIGNED_ENTRY(localAddrId,method);

  /* evaluate possible transitions for trigger 'releaseAssign', based on the current state */
  switch(state)
  {
    case TCPIP_IPADDRM_ASSIGNSM_ACTIVE:
      TcpIp_IpAddrM_AssignSM_tr_unassigned_self_ACTIVE(localAddrId,method);
      break;

    case TCPIP_IPADDRM_ASSIGNSM_INACTIVE:
      TcpIp_IpAddrM_AssignSM_tr_unassigned_self_INACTIVE(localAddrId,method);
      break;

    case TCPIP_IPADDRM_ASSIGNSM_REQUESTED:
      TcpIp_IpAddrM_AssignSM_tr_unassigned_self_REQUESTED(localAddrId,method);
      break;

    case TCPIP_IPADDRM_ASSIGNSM_STARTED:
      TcpIp_IpAddrM_AssignSM_tr_unassigned_self_STARTED(localAddrId,method);
      break;

    /* CHECK: NOPARSE */
    default:
      TcpIp_IpAddrM_AssignSM_State[localAddrId][method].state = TCPIP_IPADDRM_ASSIGNSM_INACTIVE;
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break; /* should not happen */
    /* CHECK: PARSE */
  }

  DBG_TCPIP_IPADDRM_ASSIGNSM_UNASSIGNED_EXIT(localAddrId,method);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_assigned(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method)
{
  DBG_TCPIP_IPADDRM_ASSIGNSM_ASSIGNED_ENTRY(localAddrId,method);

  TCPIP_PRECONDITION_ASSERT(TcpIp_IpAddrM_AssignSM_State[localAddrId][method].state == TCPIP_IPADDRM_ASSIGNSM_ACTIVE,TCPIP_INTERNAL_API_ID);

  TcpIp_IpAddrM_LocalAddrSM_assigned(localAddrId, method);

  DBG_TCPIP_IPADDRM_ASSIGNSM_ASSIGNED_EXIT(localAddrId,method);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_getIpConfig
(
    TcpIp_LocalAddrIdType localAddrId,
    TcpIp_IpAddrAssignmentType method,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) gatewayIpPtrPtr,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) netmaskIpPtrPtr
)
{
  TCPIP_PRECONDITION_ASSERT(method < TCPIP_IPADDRM_ASSIGN_NUM, TCPIP_INTERNAL_API_ID);
  (*TcpIp_IpAddrM_AssignSM_FctPtrTable[method].getIpConfigFctPtr)(localAddrId,addressIpPtrPtr,gatewayIpPtrPtr,netmaskIpPtrPtr);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_getIpAddress
(
    TcpIp_LocalAddrIdType localAddrId,
    TcpIp_IpAddrAssignmentType method,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr
)
{
  TCPIP_PRECONDITION_ASSERT(method < TCPIP_IPADDRM_ASSIGN_NUM, TCPIP_INTERNAL_API_ID);
  (*TcpIp_IpAddrM_AssignSM_FctPtrTable[method].getIpAddressFctPtr)(localAddrId,addressIpPtrPtr);
}

TS_MOD_PRIV_DEFN FUNC(boolean, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_isAssigned
(
    TcpIp_LocalAddrIdType localAddrId,
    TcpIp_IpAddrAssignmentType method
)
{
  return (*TcpIp_IpAddrM_AssignSM_FctPtrTable[method].isAssignedFctPtr)(localAddrId);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_assignmentActive
(
    TcpIp_LocalAddrIdType localAddrId,
    TcpIp_IpAddrAssignmentType method,
    boolean state
)
{
  TCPIP_PRECONDITION_ASSERT(method < TCPIP_IPADDRM_ASSIGN_NUM, TCPIP_INTERNAL_API_ID);
  (*TcpIp_IpAddrM_AssignSM_FctPtrTable[method].assignmentActiveFctPtr)(localAddrId,state);
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_requestIpAddress
(
    TcpIp_LocalAddrIdType localAddrId,
    TcpIp_IpAddrAssignmentType method,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) LocalIpAddrPtr
)
{
  return (*TcpIp_IpAddrM_AssignSM_FctPtrTable[method].requestIpAddressFctPtr)(localAddrId,LocalIpAddrPtr);
}

#if (TCPIP_IPV6_ENABLED == STD_ON)

TS_MOD_PRIV_DEFN FUNC(uint8, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_getAddrState
(
    TcpIp_LocalAddrIdType localAddrId,
    TcpIp_IpAddrAssignmentType method
)
{
  return (*TcpIp_IpAddrM_AssignSM_FctPtrTable[method].getIpAddrState_FctPtr)(localAddrId);
}

#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */

/*==================[internal function definitions]=========================*/

/* transitions */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_tr_INACTIVE_ACTIVE(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  TcpIp_IpAddrM_AssignSM_entry_ACTIVE(localAddrId,method);
}



STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_tr_INACTIVE_STARTED(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  TcpIp_IpAddrM_AssignSM_entry_STARTED(localAddrId,method);
}



STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_tr_REQUESTED_ACTIVE(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  TcpIp_IpAddrM_AssignSM_entry_ACTIVE(localAddrId,method);

}



STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_tr_STARTED_INACTIVE(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  TcpIp_IpAddrM_AssignSM_entry_INACTIVE(localAddrId,method);

}



STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_tr_STARTED_STARTED(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method)
{
  /* exit code */

  /* transitional code */
  (*TcpIp_IpAddrM_AssignSM_FctPtrTable[method].stopAssignFctPtr)(localAddrId);

  /* entry code */
}



STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_tr_ACTIVE_REQUESTED(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method)
{
  /* exit code */

  /* transitional code */
  (*TcpIp_IpAddrM_AssignSM_FctPtrTable[method].stopAssignFctPtr)(localAddrId);

  /* entry code */
  TcpIp_IpAddrM_AssignSM_entry_REQUESTED(localAddrId,method);

}



STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_tr_INACTIVE_REQUESTED(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  TcpIp_IpAddrM_AssignSM_entry_REQUESTED(localAddrId,method);

}



STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_tr_STARTED_ACTIVE(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  TcpIp_IpAddrM_AssignSM_entry_ACTIVE(localAddrId,method);

}



STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_tr_REQUESTED_INACTIVE(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  TcpIp_IpAddrM_AssignSM_entry_INACTIVE(localAddrId,method);

}



STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_tr_ACTIVE_STARTED(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method)
{
  /* exit code */

  /* transitional code */
  (*TcpIp_IpAddrM_AssignSM_FctPtrTable[method].stopAssignFctPtr)(localAddrId);

  /* entry code */
  TcpIp_IpAddrM_AssignSM_entry_STARTED(localAddrId,method);

}



STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_tr_self_ACTIVE(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method)
{
  /* transitional code */
  TcpIp_IpAddrM_AssignSM_entry_ACTIVE(localAddrId,method);
}


/* Deviation MISRAC2012-1 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_tr_unassigned_self(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method)
{
  /* transitional code */
  TcpIp_IpAddrM_LocalAddrSM_unassigned(localAddrId,method);
}


/* state entry functions */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_entry_INACTIVE(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method)
{
  TcpIp_IpAddrM_AssignSM_State[localAddrId][method].state = TCPIP_IPADDRM_ASSIGNSM_INACTIVE;
}



STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_entry_STARTED(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method)
{
  TcpIp_IpAddrM_AssignSM_State[localAddrId][method].state = TCPIP_IPADDRM_ASSIGNSM_STARTED;
}



STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_entry_REQUESTED(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method)
{
  TcpIp_IpAddrM_AssignSM_State[localAddrId][method].state = TCPIP_IPADDRM_ASSIGNSM_REQUESTED;
}



STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_entry_ACTIVE(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method)
{
  TcpIp_IpAddrM_AssignSM_State[localAddrId][method].state = TCPIP_IPADDRM_ASSIGNSM_ACTIVE;
  (*TcpIp_IpAddrM_AssignSM_FctPtrTable[method].startAssignFctPtr)(localAddrId);
}


#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>
/*==================[end of file]===========================================*/
