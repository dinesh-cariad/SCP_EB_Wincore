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

/*==================[inclusions]============================================*/
#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Socket_Int.h>     /* used SW-unit interface file */
#include <TcpIp_Ip_Int.h>         /* used SW-unit interface file */
#include <TcpIp_IpAddrM_Priv.h>   /* own SW-unit private header file */


/*==================[macros]================================================*/

/* local address state machine states */
#define TCPIP_IPADDRM_LOCALADDRSM_OFFLINE   0U  /** \brief local address is not used */
#define TCPIP_IPADDRM_LOCALADDRSM_ASSIGN    1U  /** \brief assignment for local addres is ongoing */
#define TCPIP_IPADDRM_LOCALADDRSM_ONLINE    2U  /** \brief local address is assigned, and can be used */
#define TCPIP_IPADDRM_LOCALADDRSM_ONHOLD    3U  /** \brief local address is assigned but physical link is lost (keep assignment) */
#define TCPIP_IPADDRM_LOCALADDRSM_CLOSE     4U  /** \brief local address shutdown: waiting for sockets getting closed */
#define TCPIP_IPADDRM_LOCALADDRSM_UNASSIGN  5U  /** \brief local address shutdown: waiting for unassignment */

/*==================[type definitions]======================================*/

/** \brief Structure contains state information for each local address */
typedef struct
{
  uint8 state; /**< current local address state */
  uint8 method; /**< current active assignment method */
  uint8 assignedMethods; /**< indicates whcih assignment methods are currently assigned */
}TcpIp_IpAddrM_LocalAddrSM_StateType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/
#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/* transitions */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_tr_OFFLINE_ASSIGN(TcpIp_LocalAddrIdType localAddrId);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_tr_ASSIGN_OFFLINE(TcpIp_LocalAddrIdType localAddrId);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_tr_ONLINE_CLOSE(TcpIp_LocalAddrIdType localAddrId);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_tr_ONHOLD_UNASSIGN(TcpIp_LocalAddrIdType localAddrId);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_tr_ONLINE_ONHOLD(TcpIp_LocalAddrIdType localAddrId);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_tr_ONHOLD_ONLINE(TcpIp_LocalAddrIdType localAddrId);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_tr_ASSIGN_ONHOLD(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_tr_ASSIGN_ONLINE(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_tr_ASSIGNED_ASSIGN(TcpIp_LocalAddrIdType localAddrId);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_tr_UNASSIGN_OFFLINE(TcpIp_LocalAddrIdType localAddrId);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_tr_CLOSE_OFFLINE(TcpIp_LocalAddrIdType localAddrId);
#if ((TCPIP_UDP_ENABLED == STD_ON) || (TCPIP_TCP_ENABLED == STD_ON))
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_tr_CLOSE_UNASSIGN(TcpIp_LocalAddrIdType localAddrId);
#endif /* ((TCPIP_UDP_ENABLED == STD_ON) || (TCPIP_TCP_ENABLED == STD_ON)) */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_tr_assigned_ASSIGNED_ASSIGNED
(
    TcpIp_LocalAddrIdType localAddrId,
    TcpIp_IpAddrAssignmentType method,
    uint8 currentState
);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_tr_unassigned_ASSIGNED_ASSIGNED
(
    TcpIp_LocalAddrIdType localAddrId,
    TcpIp_IpAddrAssignmentType method,
    uint8 currentState
);


/* state entry functions */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_entry_OFFLINE(TcpIp_LocalAddrIdType localAddrId);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_entry_ASSIGN(TcpIp_LocalAddrIdType localAddrId);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_entry_ONLINE(TcpIp_LocalAddrIdType localAddrId);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_entry_ONHOLD(TcpIp_LocalAddrIdType localAddrId);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_entry_CLOSE(TcpIp_LocalAddrIdType localAddrId);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_entry_UNASSIGN(TcpIp_LocalAddrIdType localAddrId);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_entry_ASSIGNED(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method);

/* state exit functions */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_exit_ASSIGNED(TcpIp_LocalAddrIdType localAddrId);

/* helper functions */
/* Returns true if and only if IpAddrM for localAddrId is in a state where a valid IP address is assigned */
STATIC FUNC(boolean, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_is_valid_addr_assignment
(
  TcpIp_LocalAddrIdType localAddrId
);

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/
#define TCPIP_START_SEC_CONST_32
#include <TcpIp_MemMap.h>

STATIC CONST(uint32, TCPIP_CONST) TcpIp_IpAddrM_DefaultValue[4] = {0U, 0U, 0U, 0U};

#define TCPIP_STOP_SEC_CONST_32
#include <TcpIp_MemMap.h>
/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

#define TCPIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

/** \brief state information variable definition */
STATIC VAR(TcpIp_IpAddrM_LocalAddrSM_StateType, TCPIP_VAR_CLEARED)
  TcpIp_IpAddrM_LocalAddrSM_State[TCPIP_NUM_LOCALADDRENTRIES];


#define TCPIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

/*==================[external function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

TS_MOD_PRIV_DEFN FUNC(TcpIp_StateType, TCPIP_CODE) TcpIp_IpAddrM_getIfState
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  TcpIp_StateType returnState;

  DBG_TCPIP_IPADDRM_GETIFSTATE_ENTRY(localAddrId);

  returnState = TcpIp_IpAddrM_getIntIfState(localAddrId);

  DBG_TCPIP_IPADDRM_GETIFSTATE_EXIT(returnState,localAddrId);

  return returnState;
}

#if (TCPIP_IPV6_ENABLED == STD_ON)

TS_MOD_PRIV_DEFN FUNC(uint8, TCPIP_CODE) TcpIp_IpAddrM_getIpAddrState
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  uint8 returnState;
  const uint8 method = TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].method;

  DBG_TCPIP_IPADDRM_GETIPADDRSTATE_ENTRY(localAddrId);

  returnState = TcpIp_IpAddrM_AssignSM_getAddrState(localAddrId,method);

  DBG_TCPIP_IPADDRM_GETIPADDRSTATE_EXIT(returnState,localAddrId);

  return returnState;
}

#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */

#if ((TCPIP_UDP_ENABLED == STD_ON) || (TCPIP_TCP_ENABLED == STD_ON))
TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_socketsClosed
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  const uint8 state = TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].state;

  DBG_TCPIP_IPADDRM_SOCKETSCLOSED_ENTRY(localAddrId);

  /* evaluate possible transitions for trigger 'socketsClosed', based on the current state */
  switch(state)
  {
    case TCPIP_IPADDRM_LOCALADDRSM_CLOSE:
      TcpIp_IpAddrM_LocalAddrSM_tr_CLOSE_UNASSIGN(localAddrId);
      break;

    case TCPIP_IPADDRM_LOCALADDRSM_OFFLINE: /* intended fall through */
    case TCPIP_IPADDRM_LOCALADDRSM_UNASSIGN: /* intended fall through */
    case TCPIP_IPADDRM_LOCALADDRSM_ONLINE: /* intended fall through */
    case TCPIP_IPADDRM_LOCALADDRSM_ONHOLD: /* intended fall through */
    case TCPIP_IPADDRM_LOCALADDRSM_ASSIGN:
      break; /* ignore */

    /* CHECK: NOPARSE */
    default:
      TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].state = TCPIP_IPADDRM_LOCALADDRSM_OFFLINE;
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break; /* should not happen */

    /* CHECK: PARSE */
  }

  DBG_TCPIP_IPADDRM_SOCKETSCLOSED_EXIT(localAddrId);
}
#endif /* ((TCPIP_UDP_ENABLED == STD_ON) || (TCPIP_TCP_ENABLED == STD_ON)) */

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_getIpConfig
(
    TcpIp_LocalAddrIdType localAddrId,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) gatewayIpPtrPtr,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) netmaskIpPtrPtr
)
{
  DBG_TCPIP_IPADDRM_GETIPCONFIG_ENTRY(localAddrId,addressIpPtrPtr,gatewayIpPtrPtr,netmaskIpPtrPtr);

  *addressIpPtrPtr = &TcpIp_IpAddrM_DefaultValue[0U];
  *gatewayIpPtrPtr = &TcpIp_IpAddrM_DefaultValue[0U];
  *netmaskIpPtrPtr = &TcpIp_IpAddrM_DefaultValue[0U];

  if(TcpIp_IpAddrM_LocalAddrSM_is_valid_addr_assignment(localAddrId))
  {
    TcpIp_IpAddrM_getIntIpConfig(localAddrId,addressIpPtrPtr,gatewayIpPtrPtr,netmaskIpPtrPtr);
  }

  DBG_TCPIP_IPADDRM_GETIPCONFIG_EXIT(localAddrId,addressIpPtrPtr,gatewayIpPtrPtr,netmaskIpPtrPtr);
}



TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_getIpAddress
(
    TcpIp_LocalAddrIdType localAddrId,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr
)
{
  DBG_TCPIP_IPADDRM_GETIPADDRESS_ENTRY(localAddrId,addressIpPtrPtr);

  if(TcpIp_IpAddrM_LocalAddrSM_is_valid_addr_assignment(localAddrId))
  {
    TcpIp_IpAddrM_getIntIpAddress(localAddrId,addressIpPtrPtr);
  } else
  {
    *addressIpPtrPtr = &TcpIp_IpAddrM_DefaultValue[0U];
  }

  DBG_TCPIP_IPADDRM_GETIPADDRESS_EXIT(localAddrId,addressIpPtrPtr);
}


/*==================[SW-Unit internal functions]=========================*/

TS_MOD_PRIV_DEFN FUNC(TcpIp_StateType, TCPIP_CODE) TcpIp_IpAddrM_getIntIfState
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  TcpIp_StateType returnState = TCPIP_STATE_OFFLINE;
  const uint8 state = TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].state;

  switch(state)
  {
    case TCPIP_IPADDRM_LOCALADDRSM_OFFLINE:
      returnState = TCPIP_STATE_OFFLINE;
      break;

    case TCPIP_IPADDRM_LOCALADDRSM_ASSIGN:
      returnState = TCPIP_STATE_STARTUP;
      break;

    case TCPIP_IPADDRM_LOCALADDRSM_ONLINE:
      returnState = TCPIP_STATE_ONLINE;
      break;

    case TCPIP_IPADDRM_LOCALADDRSM_ONHOLD:
      returnState = TCPIP_STATE_ONHOLD;
      break;

    case TCPIP_IPADDRM_LOCALADDRSM_CLOSE: /* intended fall through */
    case TCPIP_IPADDRM_LOCALADDRSM_UNASSIGN:
      returnState = TCPIP_STATE_SHUTDOWN;
      break;

    /* CHECK: NOPARSE */
    default:
      TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].state = TCPIP_IPADDRM_LOCALADDRSM_OFFLINE;
      returnState = TCPIP_STATE_OFFLINE;
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break; /* should not happen */
    /* CHECK: PARSE */
  }

  return returnState;
}



TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_getIntIpConfig
(
    TcpIp_LocalAddrIdType localAddrId,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) gatewayIpPtrPtr,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) netmaskIpPtrPtr
)
{
  const uint8 method = TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].method;

#if (TCPIP_PRECONDITION_ASSERT_ENABLED == STD_ON)
  {
    const uint8 state = TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].state;

    TCPIP_PRECONDITION_ASSERT
      (
       (state != TCPIP_IPADDRM_LOCALADDRSM_ASSIGN) && (state != TCPIP_IPADDRM_LOCALADDRSM_OFFLINE),
       TCPIP_INTERNAL_API_ID
      );
  }
#endif /* (TCPIP_PRECONDITION_ASSERT_ENABLED == STD_ON) */

  TcpIp_IpAddrM_AssignSM_getIpConfig(localAddrId,method,addressIpPtrPtr,gatewayIpPtrPtr,netmaskIpPtrPtr);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_getIntIpAddress
(
    TcpIp_LocalAddrIdType localAddrId,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr
)
{
  const uint8 method = TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].method;

  TcpIp_IpAddrM_AssignSM_getIpAddress(localAddrId,method,addressIpPtrPtr);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_init
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].state = TCPIP_IPADDRM_LOCALADDRSM_OFFLINE;
  TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].method = TCPIP_IPADDRM_ASSIGN_INVALID;
  TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].assignedMethods = 0U;

  {
    CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
        &(TCPIP_CFG(localAddr,localAddrId));

    uint8_least i;

    /* initialize all configured assignment state machines */
    for(i = 0U; i < localAddrPtr->numAssignm; i++)
    {
      TcpIp_IpAddrM_AssignSM_init(localAddrId,localAddrPtr->assignmByPriority[i]);
    }
  }
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_startAssign
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  TCPIP_PRECONDITION_ASSERT(localAddrId < TCPIP_NUM_LOCALADDRENTRIES, TCPIP_INTERNAL_API_ID);
  {
    const uint8 state = TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].state;

    /* evaluate possible transitions for trigger 'startAssign', based on the current state */
    switch(state)
    {
      case TCPIP_IPADDRM_LOCALADDRSM_OFFLINE:
        TcpIp_IpAddrM_LocalAddrSM_tr_OFFLINE_ASSIGN(localAddrId);
        break;

      /* CHECK: NOPARSE */
      case TCPIP_IPADDRM_LOCALADDRSM_ASSIGN: /* intended fall through */
      case TCPIP_IPADDRM_LOCALADDRSM_ONLINE: /* intended fall through */
      case TCPIP_IPADDRM_LOCALADDRSM_ONHOLD: /* intended fall through */
      case TCPIP_IPADDRM_LOCALADDRSM_CLOSE: /* intended fall through */
      case TCPIP_IPADDRM_LOCALADDRSM_UNASSIGN: /* intended fall through */
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break; /* should not happen */

      default:
        TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].state = TCPIP_IPADDRM_LOCALADDRSM_OFFLINE;
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break; /* should not happen */
        /* CHECK: PARSE */
    }
  }
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_stopAssign
(
  TcpIp_LocalAddrIdType localAddrId
)
{

  TCPIP_PRECONDITION_ASSERT(localAddrId < TCPIP_NUM_LOCALADDRENTRIES, TCPIP_INTERNAL_API_ID);
  {
    const uint8 state = TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].state;

    /* evaluate possible transitions for trigger 'stopAssign', based on the current state */
    switch (state)
    {
    case TCPIP_IPADDRM_LOCALADDRSM_ASSIGN:
      TcpIp_IpAddrM_LocalAddrSM_tr_ASSIGN_OFFLINE(localAddrId);
      break;

    case TCPIP_IPADDRM_LOCALADDRSM_ONLINE:
      TcpIp_IpAddrM_LocalAddrSM_tr_ONLINE_CLOSE(localAddrId);
      break;

    case TCPIP_IPADDRM_LOCALADDRSM_ONHOLD:
      TcpIp_IpAddrM_LocalAddrSM_tr_ONHOLD_UNASSIGN(localAddrId);
      break;

    case TCPIP_IPADDRM_LOCALADDRSM_OFFLINE:
      break; /* ignore */

    /* CHECK: NOPARSE */
    case TCPIP_IPADDRM_LOCALADDRSM_CLOSE: /* intended fall through */
    case TCPIP_IPADDRM_LOCALADDRSM_UNASSIGN:
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break; /* should not happen */

    default:
      TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].state = TCPIP_IPADDRM_LOCALADDRSM_OFFLINE;
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break; /* should not happen */
    /* CHECK: PARSE */
    }
  }
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_holdAssign
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  TCPIP_PRECONDITION_ASSERT(localAddrId < TCPIP_NUM_LOCALADDRENTRIES, TCPIP_INTERNAL_API_ID);
  {
    const uint8 state = TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].state;

    /* evaluate possible transitions for trigger 'holdAssign', based on the current state */
    switch (state)
    {
      case TCPIP_IPADDRM_LOCALADDRSM_ONLINE:
        TcpIp_IpAddrM_LocalAddrSM_tr_ONLINE_ONHOLD(localAddrId);
        break;

      case TCPIP_IPADDRM_LOCALADDRSM_ASSIGN:
        break; /* ignore */

      /* CHECK: NOPARSE */
      case TCPIP_IPADDRM_LOCALADDRSM_ONHOLD: /* intended fall through */
      case TCPIP_IPADDRM_LOCALADDRSM_OFFLINE: /* intended fall through */
      case TCPIP_IPADDRM_LOCALADDRSM_CLOSE: /* intended fall through */
      case TCPIP_IPADDRM_LOCALADDRSM_UNASSIGN:
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break; /* should not happen */

      default:
        TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].state = TCPIP_IPADDRM_LOCALADDRSM_OFFLINE;
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break; /* should not happen */
      /* CHECK: PARSE */
    }
  }
}
TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_continueAssign
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  TCPIP_PRECONDITION_ASSERT(localAddrId < TCPIP_NUM_LOCALADDRENTRIES, TCPIP_INTERNAL_API_ID);
  {
    const uint8 state = TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].state;

    /* evaluate possible transitions for trigger 'continueAssign', based on the current state */
    switch (state)
    {
      case TCPIP_IPADDRM_LOCALADDRSM_ONHOLD:
        TcpIp_IpAddrM_LocalAddrSM_tr_ONHOLD_ONLINE(localAddrId);
        break;

      case TCPIP_IPADDRM_LOCALADDRSM_ASSIGN:
        break; /* ignore */

      /* CHECK: NOPARSE */
      case TCPIP_IPADDRM_LOCALADDRSM_ONLINE: /* intended fall through */
      case TCPIP_IPADDRM_LOCALADDRSM_OFFLINE: /* intended fall through */
      case TCPIP_IPADDRM_LOCALADDRSM_CLOSE: /* intended fall through */
      case TCPIP_IPADDRM_LOCALADDRSM_UNASSIGN:
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break; /* should not happen */

      default:
        TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].state = TCPIP_IPADDRM_LOCALADDRSM_OFFLINE;
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break; /* should not happen */
      /* CHECK: PARSE */
    }
  }
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_assigned
(
  TcpIp_LocalAddrIdType localAddrId,
  TcpIp_IpAddrAssignmentType method
)
{
  const uint8 state = TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].state;

  CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
      &(TCPIP_CFG(localAddr,localAddrId));

  /* indicate that method is assigned */
  TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].assignedMethods |= (uint8)(1U << method);

  /* evaluate possible transitions for trigger 'assigned', based on the current state */
  switch(state)
  {
    case TCPIP_IPADDRM_LOCALADDRSM_ASSIGN:
    {

      if(TcpIp_IpAddrM_CtrlSM_getCurrentState(localAddrPtr->ctrlIdx) == TCPIP_STATE_ONHOLD)
      {
        TcpIp_IpAddrM_LocalAddrSM_tr_ASSIGN_ONHOLD(localAddrId, method);
      }
      else
      {
        TcpIp_IpAddrM_LocalAddrSM_tr_ASSIGN_ONLINE(localAddrId, method);
      }
      break;
    }
    case TCPIP_IPADDRM_LOCALADDRSM_ONLINE: /* intended fall through */
    case TCPIP_IPADDRM_LOCALADDRSM_ONHOLD:
    {

      const uint8 activeMethod = TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].method;
      /* check if new assignment has higher priority than current assignment */
      if(localAddrPtr->assignmByMethod[method].priority_autoAssign <= localAddrPtr->assignmByMethod[activeMethod].priority_autoAssign)
      {
        /* trigger transition if this is the case */
        TcpIp_IpAddrM_LocalAddrSM_tr_assigned_ASSIGNED_ASSIGNED(localAddrId, method, state);
      }
      break;
    }

    /* CHECK: NOPARSE */
    case TCPIP_IPADDRM_LOCALADDRSM_OFFLINE: /* intended fall through */
    case TCPIP_IPADDRM_LOCALADDRSM_CLOSE: /* intended fall through */
    case TCPIP_IPADDRM_LOCALADDRSM_UNASSIGN:
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break; /* should not happen */

    default:
      TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].state = TCPIP_IPADDRM_LOCALADDRSM_OFFLINE;
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break; /* should not happen */
    /* CHECK: PARSE */
  }
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_unassigned
(
  TcpIp_LocalAddrIdType localAddrId,
  TcpIp_IpAddrAssignmentType method
)
{
  const uint8 state = TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].state;

  /* indicate that method is not assigned any longer */
  TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].assignedMethods &= ~((uint8)(1U << method));

  /* evaluate possible transitions for trigger 'unassigned', based on the current state */
  switch(state)
  {
    case TCPIP_IPADDRM_LOCALADDRSM_ONLINE: /* intended fall through */
    case TCPIP_IPADDRM_LOCALADDRSM_ONHOLD:
    {
      const uint8 activeMethod = TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].method;

      /* if the active method assignment has been lost, check if there is any other
         assignment method active */
      if(method == activeMethod)
      {
        CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
            &(TCPIP_CFG(localAddr,localAddrId));

        uint8 i; /* iterator */
        uint8 newMethod = 0U; /* found assignment method */
        uint8 newMethodPriority = 0xFFU; /* current found assignment method priority
                                            (0xFF = lowest priority = no assignment) */

        /* loop over all configured assignments */
        for(i = 0U; i < localAddrPtr->numAssignm; i++)
        {
          /* get method */
          const uint8 currentMethod = localAddrPtr->assignmByPriority[i];

          /* ignore active method in evaluation (because this one is getting unassigned */
          if(currentMethod != activeMethod)
          {
            /* check if already a higher priority assignment method has been found */
            const uint8 currentPriority =
              localAddrPtr->assignmByMethod[currentMethod].priority_autoAssign;

            if(TcpIp_IpAddrM_AssignSM_isAssigned(localAddrId,currentMethod))
            {
              if(currentPriority < newMethodPriority)
              {
                /* new highest assignment priority found - save as result */
                newMethodPriority = currentPriority;
                newMethod = currentMethod;
              }
            }
          }
        }

        /* if still have another active assignment, trigger self-transition */
        if(newMethodPriority != 0xFFU)
        {
          TcpIp_IpAddrM_LocalAddrSM_tr_unassigned_ASSIGNED_ASSIGNED(localAddrId,newMethod,state);
        }
        /* otherwise trigger transition to state ASSIGN */
        else
        {
          TcpIp_IpAddrM_LocalAddrSM_tr_ASSIGNED_ASSIGN(localAddrId);
        }
      }
      break;
    }

    case TCPIP_IPADDRM_LOCALADDRSM_UNASSIGN:
    {
      /* if no assignment is assigned anymore go to offline */
      if(TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].assignedMethods == 0U)
      {
        TcpIp_IpAddrM_LocalAddrSM_tr_UNASSIGN_OFFLINE(localAddrId);
      }
      break;
    }

    case TCPIP_IPADDRM_LOCALADDRSM_CLOSE:
      TcpIp_IpAddrM_LocalAddrSM_tr_CLOSE_OFFLINE(localAddrId);
      break;

    /* CHECK: NOPARSE */
    case TCPIP_IPADDRM_LOCALADDRSM_ASSIGN: /* intended fall through */
    case TCPIP_IPADDRM_LOCALADDRSM_OFFLINE: /* intended fall through */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break; /* should not happen */

    default:
      TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].state = TCPIP_IPADDRM_LOCALADDRSM_OFFLINE;
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break; /* should not happen */
    /* CHECK: PARSE */
  }
}

/*==================[internal function definitions]=========================*/

/* transitions */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_tr_OFFLINE_ASSIGN
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  TcpIp_IpAddrM_LocalAddrSM_entry_ASSIGN(localAddrId);

  /* transitional code - must be executed after entry code to break recursion problem */
  {
    CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
        &(TCPIP_CFG(localAddr,localAddrId));

    uint8 i; /* iterator */

    /* start all configured assignments */
    for(i = 0U; i < localAddrPtr->numAssignm; i++)
    {
      TcpIp_IpAddrM_AssignSM_startAssign(localAddrId,localAddrPtr->assignmByPriority[i]);
    }
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_tr_ASSIGN_OFFLINE
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  /* exit code */

  /* transitional code */
  {
    CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
        &(TCPIP_CFG(localAddr,localAddrId));

    uint8 i; /* iterator */

    /* stop all configured assignments */
    for(i = localAddrPtr->numAssignm; i > 0U; i--)
    {
      TcpIp_IpAddrM_AssignSM_stopAssign(localAddrId,localAddrPtr->assignmByPriority[i-1U]);
    }
  }

  /* entry code */
  TcpIp_IpAddrM_LocalAddrSM_entry_OFFLINE(localAddrId);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_tr_ONLINE_CLOSE
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  /* exit code */
  TcpIp_IpAddrM_LocalAddrSM_exit_ASSIGNED(localAddrId);

  /* transitional code */

  /* entry code */
  TcpIp_IpAddrM_LocalAddrSM_entry_CLOSE(localAddrId);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_tr_ONHOLD_UNASSIGN
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  /* exit code */
  TcpIp_IpAddrM_LocalAddrSM_exit_ASSIGNED(localAddrId);

  /* transitional code */
  TcpIp_Socket_terminateSockets(localAddrId, FALSE);

  /* entry code */
  TcpIp_IpAddrM_LocalAddrSM_entry_UNASSIGN(localAddrId);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_tr_ONLINE_ONHOLD
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  TcpIp_IpAddrM_LocalAddrSM_entry_ONHOLD(localAddrId);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_tr_ONHOLD_ONLINE
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  TcpIp_IpAddrM_LocalAddrSM_entry_ONLINE(localAddrId);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_tr_ASSIGN_ONHOLD
(
  TcpIp_LocalAddrIdType localAddrId,
  TcpIp_IpAddrAssignmentType method
)
{
  /* exit code */

  /* transitional code */
  TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].method = method;

  /* entry code */
  TcpIp_IpAddrM_LocalAddrSM_entry_ONHOLD(localAddrId);
  TcpIp_IpAddrM_LocalAddrSM_entry_ASSIGNED(localAddrId, method);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_tr_ASSIGN_ONLINE
(
  TcpIp_LocalAddrIdType localAddrId,
  TcpIp_IpAddrAssignmentType method
)
{
  /* exit code */

  /* transitional code */
  TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].method = method;

  /* entry code */
  TcpIp_IpAddrM_LocalAddrSM_entry_ONLINE(localAddrId);
  TcpIp_IpAddrM_LocalAddrSM_entry_ASSIGNED(localAddrId, method);
}



STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_tr_ASSIGNED_ASSIGN
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  /* exit code */
  TcpIp_IpAddrM_LocalAddrSM_exit_ASSIGNED(localAddrId);

  /* transitional code */
  TcpIp_Socket_terminateSockets(localAddrId, TRUE);
  {
    CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
        &(TCPIP_CFG(localAddr,localAddrId));

    if(TCPIP_IPADDRM_IS_ADDRTYPE(localAddrPtr->addrType,UNICAST))
    {
      TcpIp_IpAddrM_CtrlSM_unassigned(localAddrPtr->ctrlIdx);
    }
  }

  /* entry code */
  TcpIp_IpAddrM_LocalAddrSM_entry_ASSIGN(localAddrId);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_tr_UNASSIGN_OFFLINE
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
      &(TCPIP_CFG(localAddr,localAddrId));

  /* exit code */

  /* transitional code */
  if(TCPIP_IPADDRM_IS_ADDRTYPE(localAddrPtr->addrType,UNICAST))
  {
    TcpIp_IpAddrM_CtrlSM_unassigned(localAddrPtr->ctrlIdx);
  }

  /* entry code */
  TcpIp_IpAddrM_LocalAddrSM_entry_OFFLINE(localAddrId);
}

#if ((TCPIP_UDP_ENABLED == STD_ON) || (TCPIP_TCP_ENABLED == STD_ON))
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_tr_CLOSE_UNASSIGN
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  TcpIp_IpAddrM_LocalAddrSM_entry_UNASSIGN(localAddrId);
}
#endif /* ((TCPIP_UDP_ENABLED == STD_ON) || (TCPIP_TCP_ENABLED == STD_ON)) */

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_tr_CLOSE_OFFLINE
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
      &(TCPIP_CFG(localAddr,localAddrId));

  /* exit code */

  /* transitional code */
  TcpIp_Socket_terminateSockets(localAddrId, TRUE);
  if(TCPIP_IPADDRM_IS_ADDRTYPE(localAddrPtr->addrType,UNICAST))
  {
    TcpIp_IpAddrM_CtrlSM_unassigned(localAddrPtr->ctrlIdx);
  }

  /* entry code */
  TcpIp_IpAddrM_LocalAddrSM_entry_OFFLINE(localAddrId);
}



STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_tr_assigned_ASSIGNED_ASSIGNED
(
    TcpIp_LocalAddrIdType localAddrId,
    TcpIp_IpAddrAssignmentType method,
    uint8 currentState
)
{
  TCPIP_PRECONDITION_ASSERT
    ( localAddrId <  TcpIp_PBcfgPtr->configLocalAddrNum, TCPIP_INTERNAL_API_ID);

  /* transitional code */
  TcpIp_IpAddrM_AssignSM_assignmentActive
    (localAddrId,TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].method,FALSE);

  {
    uint8 i;

    for(i = 0U; i < TCPIP_IPADDRM_NUMCONFSOCKOWNER; i++)
    {
      /* inform all configured upper layers that local address is unassigned */
      TcpIp_LocalIpAddrAssignChgAPI[i](localAddrId,TCPIP_IPADDR_STATE_UNASSIGNED);
    }
  }

  TcpIp_Socket_terminateSockets(localAddrId, TRUE);
  TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].method = method;

  {
    uint8 i;
    for(i = 0U; i < TCPIP_IPADDRM_NUMCONFSOCKOWNER; i++)
    {
      /* inform all configured upper layers that local address is unassigned */
      TcpIp_LocalIpAddrAssignChgAPI[i]
        (localAddrId,
        (currentState == TCPIP_IPADDRM_LOCALADDRSM_ONHOLD) ?
          TCPIP_IPADDR_STATE_ONHOLD :
          TCPIP_IPADDR_STATE_ASSIGNED);
    }
  }

  TcpIp_IpAddrM_AssignSM_assignmentActive(localAddrId,method,TRUE);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_tr_unassigned_ASSIGNED_ASSIGNED
(
    TcpIp_LocalAddrIdType localAddrId,
    TcpIp_IpAddrAssignmentType method,
    uint8 currentState
)
{
  TCPIP_PRECONDITION_ASSERT
    ( localAddrId <  TcpIp_PBcfgPtr->configLocalAddrNum, TCPIP_INTERNAL_API_ID);

  TcpIp_IpAddrM_AssignSM_assignmentActive
    (localAddrId,TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].method,FALSE);

  /* transitional code */
  {
    uint8 i;
    for(i = 0U; i < TCPIP_IPADDRM_NUMCONFSOCKOWNER; i++)
    {
      /* inform all configured upper layers that local address is unassigned */
      TcpIp_LocalIpAddrAssignChgAPI[i](localAddrId,TCPIP_IPADDR_STATE_UNASSIGNED);
    }
  }

  TcpIp_Socket_terminateSockets(localAddrId, TRUE);
  TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].method = method;

  {
    uint8 i;
    for(i = 0U; i < TCPIP_IPADDRM_NUMCONFSOCKOWNER; i++)
    {
      /* inform all configured upper layers that local address is assigned/onhold */
      TcpIp_LocalIpAddrAssignChgAPI[i]
        (localAddrId,
        (currentState == TCPIP_IPADDRM_LOCALADDRSM_ONHOLD) ?
          TCPIP_IPADDR_STATE_ONHOLD :
          TCPIP_IPADDR_STATE_ASSIGNED);
    }
  }

  TcpIp_IpAddrM_AssignSM_assignmentActive(localAddrId,method,TRUE);
}

/* state entry functions */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_entry_OFFLINE
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].state = TCPIP_IPADDRM_LOCALADDRSM_OFFLINE;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_entry_ASSIGN
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].state = TCPIP_IPADDRM_LOCALADDRSM_ASSIGN;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_entry_ONLINE
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  uint8 i;
  uint8 offset = 0U;
  TCPIP_PRECONDITION_ASSERT( TcpIp_PBcfgPtr != NULL_PTR, TCPIP_INTERNAL_API_ID );

  /* internal local addresses shall only report assignment change to internal
     socket owner, configured local addresses shall inform all socket owner */
  if(localAddrId >= TcpIp_PBcfgPtr->configLocalAddrNum)
  {
    offset = TcpIp_NumConfSockOwner;
  }

  TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].state = TCPIP_IPADDRM_LOCALADDRSM_ONLINE;

  for(i = offset; i < TCPIP_IPADDRM_NUMCONFSOCKOWNER; i++)
  {
    /* inform all configured upper layers that local address is assigned */
    TcpIp_LocalIpAddrAssignChgAPI[i](localAddrId,TCPIP_IPADDR_STATE_ASSIGNED);
  }
}



STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_entry_ONHOLD
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  uint8 i;
  uint8 offset = 0U;
  TCPIP_PRECONDITION_ASSERT( TcpIp_PBcfgPtr != NULL_PTR, TCPIP_INTERNAL_API_ID );

  /* internal local addresses shall only report assignment change to internal
     socket owner, configured local addresses shall inform all socket owner */
  if(localAddrId >= TcpIp_PBcfgPtr->configLocalAddrNum)
  {
    offset = TcpIp_NumConfSockOwner;
  }

  TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].state = TCPIP_IPADDRM_LOCALADDRSM_ONHOLD;

  for(i = offset; i < TCPIP_IPADDRM_NUMCONFSOCKOWNER; i++)
  {
    /* inform all configured upper layers that link is down */
    TcpIp_LocalIpAddrAssignChgAPI[i](localAddrId,TCPIP_IPADDR_STATE_ONHOLD);
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_entry_CLOSE
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].state = TCPIP_IPADDRM_LOCALADDRSM_CLOSE;
  TcpIp_Socket_closeSockets(localAddrId);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_entry_UNASSIGN
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].state = TCPIP_IPADDRM_LOCALADDRSM_UNASSIGN;

  {
    CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
        &(TCPIP_CFG(localAddr,localAddrId));

    uint8 i; /* iterator */

    /* stop all configured assignments */
    for(i = localAddrPtr->numAssignm; i > 0U; i--)
    {
      TcpIp_IpAddrM_AssignSM_stopAssign(localAddrId,localAddrPtr->assignmByPriority[i-1U]);
    }
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_entry_ASSIGNED
(
  TcpIp_LocalAddrIdType localAddrId,
  TcpIp_IpAddrAssignmentType method
)
{
  CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
      &(TCPIP_CFG(localAddr,localAddrId));

  TcpIp_IpAddrM_AssignSM_assignmentActive(localAddrId,method,TRUE);

  if(TCPIP_IPADDRM_IS_ADDRTYPE(localAddrPtr->addrType,UNICAST))
  {
    TcpIp_IpAddrM_CtrlSM_assigned(localAddrPtr->ctrlIdx);
  }
}

/* state exit functions */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_exit_ASSIGNED
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  uint8 i;
  uint8 offset = 0U;
  TCPIP_PRECONDITION_ASSERT( TcpIp_PBcfgPtr != NULL_PTR, TCPIP_INTERNAL_API_ID );

  /* internal local addresses shall only report assignment change to internal
     socket owner, configured local addresses shall inform all socket owner */
  if(localAddrId >= TcpIp_PBcfgPtr->configLocalAddrNum)
  {
    offset = TcpIp_NumConfSockOwner;
  }

  for(i = offset; i < TCPIP_IPADDRM_NUMCONFSOCKOWNER; i++)
  {
    /* inform all configured upper layers that local address is unassigned */
    TcpIp_LocalIpAddrAssignChgAPI[i](localAddrId,TCPIP_IPADDR_STATE_UNASSIGNED);
  }
  TcpIp_IpAddrM_AssignSM_assignmentActive
    (localAddrId,TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].method,FALSE);

}

/* helper functions */
STATIC FUNC(boolean, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_is_valid_addr_assignment
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  const uint8 state = TcpIp_IpAddrM_LocalAddrSM_State[localAddrId].state;
  boolean result = FALSE;
  if(    (state == TCPIP_IPADDRM_LOCALADDRSM_ONLINE)
      || (state == TCPIP_IPADDRM_LOCALADDRSM_ONHOLD)
      || (state == TCPIP_IPADDRM_LOCALADDRSM_CLOSE)
      || (state == TCPIP_IPADDRM_LOCALADDRSM_UNASSIGN)
    )
  {
    result = TRUE;
  }
  return result;
}

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[end of file]===========================================*/
