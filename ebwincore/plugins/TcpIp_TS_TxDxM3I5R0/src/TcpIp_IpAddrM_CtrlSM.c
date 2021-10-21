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
#include <EthSM.h>                /* EthSM API declarations */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Socket_Int.h>    /* used SW-unit interface file */
#include <TcpIp_IpAddrM_Priv.h>  /* own SW-unit private header file */
#if (TCPIP_NVM_STORING_ENABLED == STD_ON)
#include <TcpIp_Memory_Int.h>    /* used SW-unit interface file */
#endif /* (TCPIP_NVM_STORING_ENABLED == STD_ON) */


/*==================[macros]================================================*/

/* controller state machine states are equivalent to SWS states */
#define TCPIP_IPADDRM_CTRLSM_OFFLINE  TCPIP_STATE_OFFLINE   /** \brief controller is offline */
#define TCPIP_IPADDRM_CTRLSM_STARTUP  TCPIP_STATE_STARTUP   /** \brief controller waits for assigned IP address */
#define TCPIP_IPADDRM_CTRLSM_ONLINE   TCPIP_STATE_ONLINE    /** \brief controller is online */
#define TCPIP_IPADDRM_CTRLSM_ONHOLD   TCPIP_STATE_ONHOLD    /** \brief controller has assigned address but lost it's physical link */
#define TCPIP_IPADDRM_CTRLSM_SHUTDOWN TCPIP_STATE_SHUTDOWN  /** \brief closing connection resources */

/*==================[type definitions]======================================*/

/** \brief Structure contains state and state related information for each controller */
typedef struct
{
  TcpIp_StateType state; /**<  controller state */
  uint8 cntAssigned;     /**< number of addresses currently assigned to this controller */
}TcpIp_IpAddrM_CtrlSM_StateType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/
#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/* transitions */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_tr_update_OFFLINE_OFFLINE(uint8 ctrlIdx);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_tr_request_OFFLINE_STARTUP(uint8 ctrlIdx);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_tr_request_STARTUP_OFFLINE(uint8 ctrlIdx);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_tr_request_ONLINE_SHUTDOWN(uint8 ctrlIdx);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_tr_request_ONLINE_ONHOLD(uint8 ctrlIdx);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_tr_request_ONHOLD_SHUTDOWN(uint8 ctrlIdx);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_tr_request_ONHOLD_ONLINE(uint8 ctrlIdx);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_tr_assigned_STARTUP_ONLINE(uint8 ctrlIdx);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_tr_assigned_ACTIVE_ACTIVE(uint8 ctrlIdx);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_tr_unassigned_ACTIVE_OFFLINE(uint8 ctrlIdx);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_tr_unassigned_ACTIVE_ACTIVE(uint8 ctrlIdx);

/* state entry functions */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_entry_STARTUP(uint8 ctrlIdx);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_entry_ONLINE(uint8 ctrlIdx);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_entry_ONHOLD(uint8 ctrlIdx);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_entry_SHUTDOWN(uint8 ctrlIdx);
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_entry_OFFLINE(uint8 ctrlIdx);

/* state exit functions */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_exit_OFFLINE(void);

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

#define TCPIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

/** \brief state information variable definition */
VAR(TcpIp_IpAddrM_CtrlSM_StateType, TCPIP_VAR_CLEARED) TcpIp_IpAddrM_CtrlSM_State[TCPIP_NUM_ETHIFCTRL];

#define TCPIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>


#define TCPIP_START_SEC_VAR_CLEARED_8
#include <TcpIp_MemMap.h>

/** \brief variable contains number of currently active controllers (state != TCPIP_IPADDRM_CTRLSM_OFFLINE)
 *
 *  Tracking the number of active controllers is required for triggering closing all sockets assigned to ANY local address.
 */
STATIC VAR(uint8, TCPIP_VAR_CLEARED) TcpIp_IpAddrM_CtrlSM_cntActiveCtrl;

#define TCPIP_STOP_SEC_VAR_CLEARED_8
#include <TcpIp_MemMap.h>

/*==================[external function definitions]=========================*/

/*==================[SW-Unit internal functions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_init(void)
{
  TcpIp_IpAddrM_CtrlSM_cntActiveCtrl = 0U;
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_initCtrl(uint8 ctrlIdx)
{
  TcpIp_IpAddrM_CtrlSM_State[ctrlIdx].state = TCPIP_IPADDRM_CTRLSM_OFFLINE;
  TcpIp_IpAddrM_CtrlSM_State[ctrlIdx].cntAssigned = 0U;
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_request
(
  uint8 ctrlIdx,
  TcpIp_StateType requestedState
)
{
  const uint8 state = TcpIp_IpAddrM_CtrlSM_State[ctrlIdx].state;

  /* evaluate possible transitions for trigger 'request', based on the current state */
  switch(state)
  {
    case TCPIP_IPADDRM_CTRLSM_OFFLINE:
      if(requestedState == TCPIP_IPADDRM_CTRLSM_ONLINE)
      {
        TcpIp_IpAddrM_CtrlSM_tr_request_OFFLINE_STARTUP(ctrlIdx);
      }
      else
      {
        ; /* ignore */
      }
      break;

    case TCPIP_IPADDRM_CTRLSM_STARTUP:
      if(requestedState == TCPIP_IPADDRM_CTRLSM_OFFLINE)
      {
        TcpIp_IpAddrM_CtrlSM_tr_request_STARTUP_OFFLINE(ctrlIdx);
      }
      else
      {
        ; /* ignore */
      }
      break;

    case TCPIP_IPADDRM_CTRLSM_ONLINE:
      if(requestedState == TCPIP_IPADDRM_CTRLSM_OFFLINE)
      {
        TcpIp_IpAddrM_CtrlSM_tr_request_ONLINE_SHUTDOWN(ctrlIdx);
      }
      else if(requestedState == TCPIP_IPADDRM_CTRLSM_ONHOLD)
      {
        TcpIp_IpAddrM_CtrlSM_tr_request_ONLINE_ONHOLD(ctrlIdx);
      }
      else
      {
        ; /* ignore */
      }
      break;

    case TCPIP_IPADDRM_CTRLSM_ONHOLD:
      if(requestedState == TCPIP_IPADDRM_CTRLSM_OFFLINE)
      {
        TcpIp_IpAddrM_CtrlSM_tr_request_ONHOLD_SHUTDOWN(ctrlIdx);
      }
      else if(requestedState == TCPIP_IPADDRM_CTRLSM_ONLINE)
      {
        TcpIp_IpAddrM_CtrlSM_tr_request_ONHOLD_ONLINE(ctrlIdx);
      }
      else
      {
        ; /* ignore */
      }
      break;

    case TCPIP_IPADDRM_CTRLSM_SHUTDOWN:
      break;

    /* CHECK: NOPARSE */
    default:
      TcpIp_IpAddrM_CtrlSM_State[ctrlIdx].state = TCPIP_IPADDRM_CTRLSM_OFFLINE;
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break; /* should not happen */
    /* CHECK: PARSE */

  }
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_update(uint8 ctrlIdx)
{
  TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);
  {
    const uint8 state = TcpIp_IpAddrM_CtrlSM_State[ctrlIdx].state;

    /* evaluate possible transitions for trigger 'update', based on the current state */
    switch (state)
    {
      case TCPIP_IPADDRM_CTRLSM_OFFLINE:
        TcpIp_IpAddrM_CtrlSM_tr_update_OFFLINE_OFFLINE(ctrlIdx);
        break;

      case TCPIP_IPADDRM_CTRLSM_STARTUP: /* intended fall through */
      case TCPIP_IPADDRM_CTRLSM_ONLINE: /* intended fall through */
      case TCPIP_IPADDRM_CTRLSM_ONHOLD: /* intended fall through */
      case TCPIP_IPADDRM_CTRLSM_SHUTDOWN: /* intended fall through */
        break; /* ignore */

      /* CHECK: NOPARSE */
      default:
        TcpIp_IpAddrM_CtrlSM_State[ctrlIdx].state = TCPIP_IPADDRM_CTRLSM_OFFLINE;
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break; /* should not happen */
      /* CHECK: PARSE */
    }
  }
}



TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_assigned(uint8 ctrlIdx)
{
  TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);
  {
    const uint8 state = TcpIp_IpAddrM_CtrlSM_State[ctrlIdx].state;

    /* evaluate possible transitions for trigger 'assigned', based on the current state */
    switch (state)
    {
    case TCPIP_IPADDRM_CTRLSM_STARTUP:
      TcpIp_IpAddrM_CtrlSM_tr_assigned_STARTUP_ONLINE(ctrlIdx);
      break;

    case TCPIP_IPADDRM_CTRLSM_ONLINE: /* intended fall through */
    case TCPIP_IPADDRM_CTRLSM_ONHOLD: /* intended fall through */
    case TCPIP_IPADDRM_CTRLSM_SHUTDOWN:
      TcpIp_IpAddrM_CtrlSM_tr_assigned_ACTIVE_ACTIVE(ctrlIdx);
      break;

      /* CHECK: NOPARSE */
    case TCPIP_IPADDRM_CTRLSM_OFFLINE: /* intended fall through */
    default:
      TcpIp_IpAddrM_CtrlSM_State[ctrlIdx].state = TCPIP_IPADDRM_CTRLSM_OFFLINE;
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break; /* should not happen */
    /* CHECK: PARSE */
    }
  }
}
TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_unassigned(uint8 ctrlIdx)
{
  TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);
  {
    CONSTP2VAR(TcpIp_IpAddrM_CtrlSM_StateType, AUTOMATIC, TCPIP_APPL_DATA)
        TcpIp_IpAddrM_CtrlSM_StatePtr = &TcpIp_IpAddrM_CtrlSM_State[ctrlIdx];
    const uint8 state = TcpIp_IpAddrM_CtrlSM_StatePtr->state;
    const uint8 cntAssigned = TcpIp_IpAddrM_CtrlSM_StatePtr->cntAssigned;

    /* evaluate possible transitions for trigger 'unassigned', based on the current state */
    switch(state)
    {
      case TCPIP_IPADDRM_CTRLSM_ONLINE: /* intended fall through */
      case TCPIP_IPADDRM_CTRLSM_ONHOLD: /* intended fall through */
      case TCPIP_IPADDRM_CTRLSM_SHUTDOWN:
        /* is this the last assigned address getting unassigned ? */
        if(cntAssigned > 1U)
        {
          /* no - stay in current state */
          TcpIp_IpAddrM_CtrlSM_tr_unassigned_ACTIVE_ACTIVE(ctrlIdx);
        }
        else
        {
          /* yes - go to OFFLINE */
          TcpIp_IpAddrM_CtrlSM_tr_unassigned_ACTIVE_OFFLINE(ctrlIdx);
        }
        break;

        /* CHECK: NOPARSE */
      case TCPIP_IPADDRM_CTRLSM_STARTUP:
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break; /* should not happen */

      case TCPIP_IPADDRM_CTRLSM_OFFLINE: /* intended fall through */
      default:
        TcpIp_IpAddrM_CtrlSM_StatePtr->state = TCPIP_IPADDRM_CTRLSM_OFFLINE;
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
          break; /* should not happen */
      /* CHECK: PARSE */
    }
  }
}


TS_MOD_PRIV_DEFN FUNC(TcpIp_StateType, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_getCurrentState(uint8 ctrlIdx)
{
  return (TcpIp_StateType)TcpIp_IpAddrM_CtrlSM_State[ctrlIdx].state;
}

/*==================[internal function definitions]=========================*/

/* transitions */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_tr_update_OFFLINE_OFFLINE(uint8 ctrlIdx)
{
  /* exit code */

  /* transitional code */
  {
    CONSTP2CONST(TcpIp_ctrlConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlPtr =
        &((TCPIP_CFG(ctrl,ctrlIdx)));

    const uint8_least startIndex = ctrlPtr->intLocalAddrIdFirst;
    const uint8_least endIndex = startIndex + ctrlPtr->intLocalAddrNum;
    uint8_least i;

    /* stop assignment for all local addresses configured for this controller */
    for(i = startIndex; i < endIndex; i++)
    {
      const uint8 intLocalAddr =
        (TCPIP_CFG_TYPE(uint8,intIpLocalAddr,i));

      TcpIp_IpAddrM_LocalAddrSM_stopAssign(intLocalAddr);
      TcpIp_IpAddrM_mainFunction_localAddr(intLocalAddr);
    }
  }

  /* entry code */
}



STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_tr_request_OFFLINE_STARTUP(uint8 ctrlIdx)
{
  /* exit code */
  TcpIp_IpAddrM_CtrlSM_exit_OFFLINE();

  /* transitional code */
  {

#if (TCPIP_GET_RANDOM_ENABLED == STD_ON)
    /* initialize the random number generator */
    TcpIp_InitRandomGenerator(ctrlIdx);
#endif /* (TCPIP_GET_RANDOM_ENABLED == STD_ON)) */

#if (TCPIP_NVM_STORING_ENABLED == STD_ON)
    if(TcpIp_IpAddrM_CtrlSM_cntActiveCtrl == 1U)
    {
      TcpIp_Memory_NvM_start();
    }
#endif /* (TCPIP_NVM_STORING_ENABLED == STD_ON) */
  }

  /* entry code */
  TcpIp_IpAddrM_CtrlSM_entry_STARTUP(ctrlIdx);
}



STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_tr_request_STARTUP_OFFLINE(uint8 ctrlIdx)
{
  /* exit code */

  /* transitional code */
  {
    CONSTP2CONST(TcpIp_ctrlConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlPtr =
        &((TCPIP_CFG(ctrl,ctrlIdx)));

    const uint8_least startIndex = ctrlPtr->intLocalAddrIdFirst;
    const uint8_least endIndex = startIndex + ctrlPtr->intLocalAddrNum;
    uint8_least i;

    /* stop assignment for all local addresses configured for this controller */
    for(i = startIndex; i < endIndex; i++)
    {
      const uint8 intLocalAddr =
        (TCPIP_CFG_TYPE(uint8,intIpLocalAddr,i));

      TcpIp_IpAddrM_LocalAddrSM_stopAssign(intLocalAddr);
    }

#if (TCPIP_NVM_STORING_ENABLED == STD_ON)
    if(TcpIp_IpAddrM_CtrlSM_cntActiveCtrl == 1U)
    {
      TcpIp_Memory_NvM_stop();
    }
#endif /* (TCPIP_NVM_STORING_ENABLED == STD_ON) */

  }

  /* entry code */
  TcpIp_IpAddrM_CtrlSM_entry_OFFLINE(ctrlIdx);
}



STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_tr_request_ONLINE_SHUTDOWN(uint8 ctrlIdx)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  TcpIp_IpAddrM_CtrlSM_entry_SHUTDOWN(ctrlIdx);
}



STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_tr_request_ONLINE_ONHOLD(uint8 ctrlIdx)
{
  /* exit code */

  /* transitional code */
  {
    CONSTP2CONST(TcpIp_ctrlConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlPtr =
        &((TCPIP_CFG(ctrl,ctrlIdx)));

    const uint8_least startIndex = ctrlPtr->intLocalAddrIdFirst;
    const uint8_least endIndex = startIndex + ctrlPtr->intLocalAddrNum;
    uint8_least i;

    /* hold assignment for all local addresses configured for this controller */
    for(i = startIndex; i < endIndex; i++)
    {
      const uint8 intLocalAddr =
        (TCPIP_CFG_TYPE(uint8,intIpLocalAddr,i));

      TcpIp_IpAddrM_LocalAddrSM_holdAssign(intLocalAddr);
    }
  }

  /* entry code */
  TcpIp_IpAddrM_CtrlSM_entry_ONHOLD(ctrlIdx);
}



STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_tr_request_ONHOLD_SHUTDOWN(uint8 ctrlIdx)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  TcpIp_IpAddrM_CtrlSM_entry_SHUTDOWN(ctrlIdx);
}



STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_tr_request_ONHOLD_ONLINE(uint8 ctrlIdx)
{
  /* exit code */

  /* transitional code */
  {
    CONSTP2CONST(TcpIp_ctrlConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlPtr =
        &((TCPIP_CFG(ctrl,ctrlIdx)));

    const uint8_least startIndex = ctrlPtr->intLocalAddrIdFirst;
    const uint8_least endIndex = startIndex + ctrlPtr->intLocalAddrNum;
    uint8_least i;

    /* continue assignment for all local addresses configured for this controller */
    for(i = startIndex; i < endIndex; i++)
    {
      const uint8 intLocalAddr =
        (TCPIP_CFG_TYPE(uint8,intIpLocalAddr,i));

      TcpIp_IpAddrM_LocalAddrSM_continueAssign(intLocalAddr);
    }
  }

  /* entry code */
  TcpIp_IpAddrM_CtrlSM_entry_ONLINE(ctrlIdx);
}



STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_tr_assigned_STARTUP_ONLINE(uint8 ctrlIdx)
{
  /* exit code */

  /* transitional code */
  TcpIp_IpAddrM_CtrlSM_State[ctrlIdx].cntAssigned++;

  /* entry code */
  TcpIp_IpAddrM_CtrlSM_entry_ONLINE(ctrlIdx);
}



STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_tr_assigned_ACTIVE_ACTIVE(uint8 ctrlIdx)
{
  /* transitional code */
  TcpIp_IpAddrM_CtrlSM_State[ctrlIdx].cntAssigned++;

}



STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_tr_unassigned_ACTIVE_OFFLINE(uint8 ctrlIdx)
{
  /* exit code */

  /* transitional code */
  TcpIp_IpAddrM_CtrlSM_State[ctrlIdx].cntAssigned = 0U;
#if (TCPIP_NVM_STORING_ENABLED == STD_ON)
  if(TcpIp_IpAddrM_CtrlSM_cntActiveCtrl == 1U)
  {
    TcpIp_Memory_NvM_stop();
  }
#endif /* (TCPIP_NVM_STORING_ENABLED == STD_ON) */

  /* entry code */
  TcpIp_IpAddrM_CtrlSM_entry_OFFLINE(ctrlIdx);
}



STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_tr_unassigned_ACTIVE_ACTIVE(uint8 ctrlIdx)
{
  /* transitional code */
  TcpIp_IpAddrM_CtrlSM_State[ctrlIdx].cntAssigned--;

}




/* state entry functions */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_entry_STARTUP(uint8 ctrlIdx)
{
  TcpIp_IpAddrM_CtrlSM_State[ctrlIdx].state = TCPIP_IPADDRM_CTRLSM_STARTUP;
  EthSM_TcpIpModeIndication((TCPIP_CFG(ctrl,ctrlIdx)).ethIfCtrlIdx,TCPIP_STATE_STARTUP);

  {
    CONSTP2CONST(TcpIp_ctrlConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlPtr =
        &((TCPIP_CFG(ctrl,ctrlIdx)));

    const uint8_least startIndex = ctrlPtr->intLocalAddrIdFirst;
    const uint8_least endIndex = startIndex + ctrlPtr->intLocalAddrNum;
    uint8_least i;

    /* start assignment for all local addresses configured for this controller */
    for(i = startIndex; i < endIndex; i++)
    {
      const uint8 intLocalAddr =
        (TCPIP_CFG_TYPE(uint8,intIpLocalAddr,i));

      TcpIp_IpAddrM_LocalAddrSM_startAssign(intLocalAddr);
    }
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_entry_ONLINE(uint8 ctrlIdx)
{
  CONSTP2CONST(TcpIp_ctrlConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlPtr =
      &(TCPIP_CFG(ctrl,ctrlIdx));

  TcpIp_IpAddrM_CtrlSM_State[ctrlIdx].state = TCPIP_IPADDRM_CTRLSM_ONLINE;

  if(ctrlPtr->localAddrIdAny != TCPIP_LOCALADDRID_INVALID)
  {
    uint8 i;
    for(i = 0U; i < TCPIP_IPADDRM_NUMCONFSOCKOWNER; i++)
    {
      /* inform all configured upper layers that local address is assigned */
      TcpIp_LocalIpAddrAssignChgAPI[i](ctrlPtr->localAddrIdAny,TCPIP_IPADDR_STATE_ASSIGNED);
    }
  }

  EthSM_TcpIpModeIndication((TCPIP_CFG(ctrl,ctrlIdx)).ethIfCtrlIdx,TCPIP_STATE_ONLINE);

}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_entry_ONHOLD(uint8 ctrlIdx)
{
  CONSTP2CONST(TcpIp_ctrlConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlPtr =
      &(TCPIP_CFG(ctrl,ctrlIdx));

  TcpIp_IpAddrM_CtrlSM_State[ctrlIdx].state = TCPIP_IPADDRM_CTRLSM_ONHOLD;

  if(ctrlPtr->localAddrIdAny != TCPIP_LOCALADDRID_INVALID)
  {
    uint8 i;
    for(i = 0U; i < TCPIP_IPADDRM_NUMCONFSOCKOWNER; i++)
    {
      /* inform all configured upper layers that local address is onhold */
      TcpIp_LocalIpAddrAssignChgAPI[i](ctrlPtr->localAddrIdAny,TCPIP_IPADDR_STATE_ONHOLD);
    }
  }

  EthSM_TcpIpModeIndication((TCPIP_CFG(ctrl,ctrlIdx)).ethIfCtrlIdx,TCPIP_STATE_ONHOLD);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_entry_SHUTDOWN(uint8 ctrlIdx)
{
  TcpIp_IpAddrM_CtrlSM_State[ctrlIdx].state = TCPIP_IPADDRM_CTRLSM_SHUTDOWN;
  EthSM_TcpIpModeIndication((TCPIP_CFG(ctrl,ctrlIdx)).ethIfCtrlIdx,TCPIP_STATE_SHUTDOWN);

  {
    CONSTP2CONST(TcpIp_ctrlConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlPtr =
        &((TCPIP_CFG(ctrl,ctrlIdx)));

    const uint8_least startIndex = ctrlPtr->intLocalAddrIdFirst;
    const uint8_least endIndex = startIndex + ctrlPtr->intLocalAddrNum;
    uint8_least i;

    /* stop assignment for all local addresses configured for this controller */
    for(i = startIndex; i < endIndex; i++)
    {
      const uint8 intLocalAddr =
        (TCPIP_CFG_TYPE(uint8,intIpLocalAddr,i));

      TcpIp_IpAddrM_LocalAddrSM_stopAssign(intLocalAddr);
    }
  }

}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_entry_OFFLINE(uint8 ctrlIdx)
{
  CONSTP2CONST(TcpIp_ctrlConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlPtr =
      &((TCPIP_CFG(ctrl,ctrlIdx)));

  TcpIp_IpAddrM_CtrlSM_State[ctrlIdx].state = TCPIP_IPADDRM_CTRLSM_OFFLINE;
  TcpIp_IpAddrM_CtrlSM_cntActiveCtrl--;

  /* if there is an controller specific ANY local address, immediately close all sockets assigned to it. */
  if(ctrlPtr->localAddrIdAny != TCPIP_LOCALADDRID_INVALID)
  {
    uint8 i;
    for(i = 0U; i < TCPIP_IPADDRM_NUMCONFSOCKOWNER; i++)
    {
      /* inform all configured upper layers that local address is unassigned */
      TcpIp_LocalIpAddrAssignChgAPI[i](ctrlPtr->localAddrIdAny,TCPIP_IPADDR_STATE_UNASSIGNED);
    }
  }

  /* if there is an controller specific ANY local address, immediately close all sockets assigned to it. */
  if(ctrlPtr->localAddrIdAny != TCPIP_LOCALADDRID_INVALID)
  {
    TcpIp_Socket_terminateSockets(ctrlPtr->localAddrIdAny, FALSE);
  }

  /* if this is the last controller switching to offline, immediately close all sockets assigned to ANY local address. */
  if(TcpIp_IpAddrM_CtrlSM_cntActiveCtrl == 0U)
  {
    TcpIp_Socket_terminateSockets(TCPIP_LOCALADDRID_ANY, FALSE);
  }

  EthSM_TcpIpModeIndication((TCPIP_CFG(ctrl,ctrlIdx)).ethIfCtrlIdx,TCPIP_STATE_OFFLINE);
}



/* state exit functions */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_exit_OFFLINE(void)
{
  TcpIp_IpAddrM_CtrlSM_cntActiveCtrl++;
}
/* state exit functions */
/* none implemented */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[end of file]===========================================*/
