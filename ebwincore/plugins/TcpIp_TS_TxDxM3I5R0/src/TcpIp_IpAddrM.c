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

#include <EthIf.h>              /* EthIf API declarations */
#include <SchM_TcpIp.h>         /* SchM API declarations */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Ip_Int.h>       /* used SW-unit interface file */
#include <TcpIp_IpAddrM_Priv.h> /* own SW-unit private header file */

/*==================[macros]================================================*/

/** \brief TcpIp_RequestComMode() service ID */
#define TCPIP_SID_REQUESTCOMMODE           0x09U

/** \brief TcpIp_RequestIpAddrAssignment() service ID */
#define TCPIP_SID_REQUESTIPADDRASSIGNMENT  0x0aU

/** \brief TcpIp_ReleaseIpAddrAssignment() service ID */
#define TCPIP_SID_RELEASEIPADDRASSIGNMENT  0x0bU

/** \brief TcpIp_GetIpAddr() service ID */
#define TCPIP_SID_GETIPADDR                0x10U

/** \brief TcpIp_GetPhysAddr() service ID */
#define TCPIP_SID_GETPHYSADDR              0x11U

/** \brief TcpIp_GetPhysAddr() service ID */
#define TCPIP_SID_GETCTRLIDX               0x17U

/** \brief macros for storing the address assignment requests TcpIp_RequestAddrAssignment()
 * and TcpIp_ReleaseAddrAssignment */
#define TCPIP_IPADDRM_ASSIGN_EVENT_NONE     0x0U

#define TCPIP_IPADDRM_EVENT_SET_REQUEST_FLAG(variable,method)\
  ((variable) |= ((uint16)(((uint16)0x0001U)<<(method))))

#define TCPIP_IPADDRM_EVENT_SET_RELEASE_FLAG(variable,method)\
  ((variable) |= ((uint16)(((uint16)0x0100U)<<(method))))

#define TCPIP_IPADDRM_EVENT_CLEAR_REQUEST_FLAG(variable,method)\
  ((variable) &= (uint16)~((uint16)(((uint16)0x0001U)<<(method))))

#define TCPIP_IPADDRM_EVENT_CLEAR_RELEASE_FLAG(variable,method)\
  ((variable) &= (uint16)~((uint16)(((uint16)0x0100U)<<(method))))

#define TCPIP_IPADDRM_EVENT_CHECK_REQUEST_FLAG(variable,method)\
  (((variable)&((uint16)(((uint16)0x0001U)<<(method)))) != 0x0U)

#define TCPIP_IPADDRM_EVENT_CHECK_RELEASE_FLAG(variable,method)\
  (((variable)&((uint16)(((uint16)0x0100U)<<(method)))) != 0x0U)

#define TCPIP_IPADDRM_EVENT_FLAG_EXISTS(variable)\
  ((variable) != TCPIP_IPADDRM_ASSIGN_EVENT_NONE)

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_mainFunction_ctrl(uint8 ctrlIdx, boolean checkLock);

/** \brief Maps AUTOSAR method type to internal method type
 */
STATIC FUNC(TcpIp_IpAddrAssignmentType, TCPIP_CODE) TcpIp_IpAddrM_getInternalType
(
  TcpIp_IpAddrM_AddrInfoType addrInfo,
  TcpIp_IpAddrAssignmentType Type
);


/** \brief request assignment  */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpAddrM_requestSingleAssignment
(
  TcpIp_LocalAddrIdType LocalAddrId,
  TcpIp_IpAddrAssignmentType intType,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) LocalIpAddrPtr
);

/** \brief Request all IP address assignments
 *
 * \param[in] LocalAddrId IP address index specifying the IP address for which the
 *                        assignments shall be requested.
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_requestAllAssignments
(
  TcpIp_LocalAddrIdType LocalAddrId
);

/** \brief Release IP address assignments
 *
 * \param[in] LocalAddrId IP address index specifying the IP address for which the
 *                        assignments shall be released.
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpAddrM_releaseAllAssignments
(
  TcpIp_LocalAddrIdType LocalAddrId
);


#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

#define TCPIP_START_SEC_VAR_CLEARED_8
#include <TcpIp_MemMap.h>

/** \brief variable holds the latest requested state as requested with API call TcpIp_RequestComMode() per controller */
STATIC VAR(TcpIp_StateType, TCPIP_VAR_CLEARED) TcpIp_IpAddrM_RequestComMode_requestedState[TCPIP_NUM_ETHIFCTRL];

/** \brief lock counters per controller which protect the related controller from state changes
 * Locking is used during Ip-transmission */
STATIC VAR(uint8, TCPIP_VAR_CLEARED) TcpIp_IpAddrM_ctrlLockCounter[TCPIP_NUM_ETHIFCTRL];

/** \brief lock counters per controller which indicate that a mainfunction call has been delayed
 *   caused by a locked interface
 */
STATIC VAR(uint8, TCPIP_VAR_CLEARED) TcpIp_IpAddrM_ctrlDelayFlag[TCPIP_NUM_ETHIFCTRL];

#define TCPIP_STOP_SEC_VAR_CLEARED_8
#include <TcpIp_MemMap.h>

#define TCPIP_START_SEC_VAR_CLEARED_16
#include <TcpIp_MemMap.h>

/** \brief remember address assignment events as triggered with TcpIp_RequestAddrAssignment() and TcpIp_ReleaseAddrAssignment()
 * Use macros TCPIP_IPADDRM_ASSIGN_EVENT_* for accessing this variable
 */
STATIC VAR(uint16,TCPIP_VAR_CLEARED) TcpIp_IpAddrM_IpAddrAssignment_Event[TCPIP_NUM_LOCALADDRENTRIES];

#define TCPIP_STOP_SEC_VAR_CLEARED_16
#include <TcpIp_MemMap.h>

/*==================[external function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_RequestComMode(uint8 CtrlIdx,TcpIp_StateType State)
{
  Std_ReturnType retval = E_NOT_OK;

  DBG_TCPIP_REQUESTCOMMODE_ENTRY(CtrlIdx,State);

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )

  if(TCPIP_UNINIT)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_REQUESTCOMMODE, TCPIP_E_NOTINIT);
  }
  else if(CtrlIdx > TcpIp_PBcfgPtr->maxCtrlIdx)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_REQUESTCOMMODE, TCPIP_E_INV_ARG);
  }
  else if((State != TCPIP_STATE_ONLINE) && (State != TCPIP_STATE_OFFLINE)
      && (State != TCPIP_STATE_ONHOLD))
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_REQUESTCOMMODE, TCPIP_E_INV_ARG);
  }
  else if((TCPIP_CFG_TYPE(uint8,ctrlMapping,CtrlIdx)) == TCPIP_CTRLIDX_INVALID)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_REQUESTCOMMODE, TCPIP_E_INV_ARG);
  }
  else

#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */

  {
    /* store new requested state */
    /* !LINKSTO TcpIp.Design.IpAddrM.Atomic.State.RequestComMode,1 */
    TS_AtomicAssign8
      ( TcpIp_IpAddrM_RequestComMode_requestedState[(TCPIP_CFG_TYPE(uint8,ctrlMapping,CtrlIdx))],
        State
      );
    retval = E_OK;
  }

  DBG_TCPIP_REQUESTCOMMODE_EXIT(retval,CtrlIdx,State);
  return retval;
} /* eof - TcpIp_RequestComMode */



FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_RequestIpAddrAssignment
(
  TcpIp_LocalAddrIdType LocalAddrId,
  TcpIp_IpAddrAssignmentType Type,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) LocalIpAddrPtr
)
{
  Std_ReturnType retval = E_NOT_OK;

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
  if( TCPIP_UNINIT )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_REQUESTIPADDRASSIGNMENT, TCPIP_E_NOTINIT);
  }
  else if(LocalAddrId >= TcpIp_PBcfgPtr->configLocalAddrNum)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_REQUESTIPADDRASSIGNMENT, TCPIP_E_INV_ARG);
  }
  else if((Type != TCPIP_IPADDR_ASSIGNMENT_STATIC)         &&
          (Type != TCPIP_IPADDR_ASSIGNMENT_DHCP)           &&
          (Type != TCPIP_IPADDR_ASSIGNMENT_LINKLOCAL)      &&
          (Type != TCPIP_IPADDR_ASSIGNMENT_LINKLOCAL_DOIP) &&
          (Type != TCPIP_IPADDR_ASSIGNMENT_IPV6_ROUTER)    &&
          (Type != TCPIP_IPADDR_ASSIGNMENT_ALL)
         )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_REQUESTIPADDRASSIGNMENT, TCPIP_E_INV_ARG);
  }
  else
#endif /* ( TCPIP_DEV_ERROR_DETECT == STD_ON ) */
  {
    retval = TcpIp_IpAddrM_requestIpAddrAssignment(LocalAddrId, Type, LocalIpAddrPtr);
  }

  return retval;
} /* eof - TcpIp_RequestIpAddrAssignment */



FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_ReleaseIpAddrAssignment
(
  TcpIp_LocalAddrIdType LocalAddrId
)
{
  Std_ReturnType retval = E_NOT_OK;

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
  if( TCPIP_UNINIT )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_RELEASEIPADDRASSIGNMENT, TCPIP_E_NOTINIT);
  }
  else if(LocalAddrId >= TcpIp_PBcfgPtr->configLocalAddrNum)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_RELEASEIPADDRASSIGNMENT, TCPIP_E_INV_ARG);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
    retval = TcpIp_IpAddrM_releaseIpAddrAssignment(LocalAddrId);
  }
  return retval;
} /* eof - TcpIp_ReleaseIpAddrAssignment */



FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetIpAddr
(
  TcpIp_LocalAddrIdType LocalAddrId,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) IpAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) NetmaskPtr,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) DefaultRouterPtr
)
{
  Std_ReturnType retval = E_NOT_OK;

  DBG_TCPIP_GETIPADDR_ENTRY(LocalAddrId,IpAddrPtr,NetmaskPtr,DefaultRouterPtr);

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
  if( TCPIP_UNINIT )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_GETIPADDR, TCPIP_E_NOTINIT);
  }
  else if(TcpIp_IpAddrM_checkConcreteLocalAddr(LocalAddrId) != E_OK)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_GETIPADDR, TCPIP_E_INV_ARG);
  }
  else if( IpAddrPtr == NULL_PTR )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_GETIPADDR, TCPIP_EB_E_PARAM_POINTER);
  }
  else if( DefaultRouterPtr == NULL_PTR )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_GETIPADDR, TCPIP_EB_E_PARAM_POINTER);
  }
  else if( NetmaskPtr == NULL_PTR )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_GETIPADDR, TCPIP_EB_E_PARAM_POINTER);
  }
  else if((IpAddrPtr->domain != TCPIP_AF_INET) && (IpAddrPtr->domain != TCPIP_AF_INET6))
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_GETIPADDR, TCPIP_E_AFNOSUPPORT);
  }
  else if((DefaultRouterPtr->domain != TCPIP_AF_INET) && (DefaultRouterPtr->domain != TCPIP_AF_INET6))
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_GETIPADDR, TCPIP_E_AFNOSUPPORT);
  }
  else if( IpAddrPtr->domain != DefaultRouterPtr->domain )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_GETIPADDR, TCPIP_E_INV_ARG);
  }
  else if(TCPIP_IPADDRM_ADDRDOMAIN_GET(TcpIp_IpAddrM_getLocalAddrInfo(LocalAddrId))
          != (TCPIP_IP_DOMAIN_GET(IpAddrPtr->domain))
         )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_GETIPADDR, TCPIP_E_AFNOSUPPORT);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
    switch(TcpIp_IpAddrM_getIntIfState(LocalAddrId))
    {
      case TCPIP_STATE_ONHOLD: /* intended fall through */
      case TCPIP_STATE_SHUTDOWN: /* intended fall through */
      case TCPIP_STATE_ONLINE:
      {
        TcpIp_Ip_getIpConfig(LocalAddrId, IpAddrPtr, NetmaskPtr, DefaultRouterPtr);

        retval = E_OK;
        break;
      }
      case TCPIP_STATE_OFFLINE: /* intended fall through */
      case TCPIP_STATE_STARTUP: /* intended fall through */
      default: break;
    }
  }
  DBG_TCPIP_GETIPADDR_EXIT(retval,LocalAddrId,IpAddrPtr,NetmaskPtr,DefaultRouterPtr);
  return retval;
} /* eof - TcpIp_GetIpAddr */



FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetPhysAddr
(
    TcpIp_LocalAddrIdType LocalAddrId,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr
)
{
  Std_ReturnType retval = E_NOT_OK;

  DBG_TCPIP_GETPHYSADDR_ENTRY(LocalAddrId,PhysAddrPtr);

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
  if( TCPIP_UNINIT )
  {
      TCPIP_REPORT_ERROR(TCPIP_SID_GETPHYSADDR, TCPIP_E_NOTINIT);
  }
  else if(TcpIp_IpAddrM_checkConcreteLocalAddr(LocalAddrId) != E_OK)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_GETPHYSADDR, TCPIP_E_INV_ARG);
  }
  else if( PhysAddrPtr == NULL_PTR )
  {
      TCPIP_REPORT_ERROR(TCPIP_SID_GETPHYSADDR, TCPIP_EB_E_PARAM_POINTER);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
    CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
        &(TCPIP_CFG(localAddr,LocalAddrId));

    EthIf_GetPhysAddr((TCPIP_CFG(ctrl,localAddrPtr->ctrlIdx)).ethIfCtrlIdx,PhysAddrPtr);

    retval = E_OK;
  }
  DBG_TCPIP_GETPHYSADDR_EXIT(retval,LocalAddrId,PhysAddrPtr);
  return retval;
} /* eof - TcpIp_GetPhysAddr */

FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetCtrlIdx
(
  TcpIp_LocalAddrIdType LocalAddrId,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) CtrlIdxPtr
)
{
  Std_ReturnType retval = E_NOT_OK;

  DBG_TCPIP_GETCTRLIDX_ENTRY(LocalAddrId,CtrlIdxPtr);

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
  if( TCPIP_UNINIT )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_GETCTRLIDX, TCPIP_E_NOTINIT);
  }
  else if( LocalAddrId >= TcpIp_PBcfgPtr->configLocalAddrNum)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_GETCTRLIDX, TCPIP_E_INV_ARG);
  }
  else if( CtrlIdxPtr == NULL_PTR )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_GETCTRLIDX, TCPIP_EB_E_PARAM_POINTER);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
    const uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(LocalAddrId);
    TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);
    *CtrlIdxPtr = (TCPIP_CFG(ctrl,ctrlIdx)).ethIfCtrlIdx;

    retval = E_OK;
  }

  DBG_TCPIP_GETCTRLIDX_EXIT(retval,LocalAddrId,CtrlIdxPtr);

  return retval;
}


/*==================[SW-Unit internal functions]=========================*/

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_mainFunction_localAddr
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  uint16 eventsFlags;
  TCPIP_PRECONDITION_ASSERT(localAddrId < TCPIP_NUM_LOCALADDRENTRIES, TCPIP_INTERNAL_API_ID);
  /* !LINKSTO TcpIp.Design.IpAddrM.EA2.Mainfunction,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_2();

  /* store events to local variables and clear events afterwards */
  eventsFlags = TcpIp_IpAddrM_IpAddrAssignment_Event[localAddrId];
  TcpIp_IpAddrM_IpAddrAssignment_Event[localAddrId] = TCPIP_IPADDRM_ASSIGN_EVENT_NONE;

  /* !LINKSTO TcpIp.Design.IpAddrM.EA2.Mainfunction,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_2();

  /* trigger release address assignment transition */
  {
    CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
        &(TCPIP_CFG(localAddr,localAddrId));

    uint8_least i;

    if(TCPIP_IPADDRM_EVENT_FLAG_EXISTS(eventsFlags))
    {
      /* check for assignment events in priority order */
      for(i = 0U; i < localAddrPtr->numAssignm; i++)
      {
        TcpIp_IpAddrAssignmentType method = localAddrPtr->assignmByPriority[i];

        if(TCPIP_IPADDRM_EVENT_CHECK_RELEASE_FLAG(eventsFlags,method))
        {
          /* call release assignment functions */
          TcpIp_IpAddrM_AssignSM_releaseAssign(localAddrId,method);
        }

        if(TCPIP_IPADDRM_EVENT_CHECK_REQUEST_FLAG(eventsFlags,method))
        {
          /* call request assignment functions */
          TcpIp_IpAddrM_AssignSM_requestAssign(localAddrId,method);
        }
      }
    }
  }
}

/*==================[external function definitions]=========================*/

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_mainFunction(void)
{
  uint8_least i; /* iterator variable */

  DBG_TCPIP_IPADDRM_MAINFUNCTION_ENTRY();

  /* perform controller state machine transitions first, this speeds up startup/shutdown*/
  for(i = 0U; i < (uint8_least)TcpIp_PBcfgPtr->ctrlNum; i++)
  {
    TcpIp_IpAddrM_mainFunction_ctrl((uint8)i,TRUE);
  }

  DBG_TCPIP_IPADDRM_MAINFUNCTION_EXIT();
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_IpAddrM_AddrInfoType, TCPIP_CODE) TcpIp_IpAddrM_getLocalAddrInfo
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
      &(TCPIP_CFG(localAddr,localAddrId));

  const TcpIp_IpAddrM_AddrInfoType retVal = (TCPIP_IPADDRM_ADDRDOMAIN_IPV4|localAddrPtr->addrType);

  DBG_TCPIP_IPADDRM_GETLOCALADDRINFO_ENTRY(localAddrId);

  DBG_TCPIP_IPADDRM_GETLOCALADDRINFO_EXIT(retVal,localAddrId);

  return retVal;
}


#if (TCPIP_UDP_ENABLED == STD_ON) || (TCPIP_TCP_ENABLED == STD_ON)

TS_MOD_PRIV_DEFN FUNC(TcpIp_LocalAddrIdType, TCPIP_CODE) TcpIp_IpAddrM_getAnyIpLocalAddrId
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  TcpIp_LocalAddrIdType result = localAddrId;

  CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
      &(TCPIP_CFG(localAddr,localAddrId));

  CONSTP2CONST(TcpIp_ctrlConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlPtr =
      &((TCPIP_CFG(ctrl,localAddrPtr->ctrlIdx)));

  DBG_TCPIP_IPADDRM_GETANYIPLOCALADDRID_ENTRY(localAddrId);

  result = ctrlPtr->localAddrIdAny;

  DBG_TCPIP_IPADDRM_GETANYIPLOCALADDRID_EXIT(result,localAddrId);

  return result;
}

#endif /* (TCPIP_UDP_ENABLED == STD_ON) || (TCPIP_TCP_ENABLED == STD_ON) */

TS_MOD_PRIV_DEFN FUNC(boolean, TCPIP_CODE) TcpIp_IpAddrM_lockIfOnline
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  boolean result = FALSE;

  DBG_TCPIP_IPADDRM_LOCKIFONLINE_ENTRY(localAddrId);

  /* !LINKSTO TcpIp.Design.IpAddrM.EA3.lockIfOnline,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_3();
  {
    TcpIp_StateType state = TcpIp_IpAddrM_getIntIfState(localAddrId);
    if(state == TCPIP_STATE_ONLINE)
    {
      CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
          &(TCPIP_CFG(localAddr,localAddrId));
      TCPIP_PRECONDITION_ASSERT(localAddrPtr->ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);
      TcpIp_IpAddrM_ctrlLockCounter[localAddrPtr->ctrlIdx]++;
      result = TRUE;
    }
  }
  /* !LINKSTO TcpIp.Design.IpAddrM.EA3.lockIfOnline,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_3();

  DBG_TCPIP_IPADDRM_LOCKIFONLINE_EXIT(result,localAddrId);

  return result;
}

TS_MOD_PRIV_DEFN FUNC(boolean, TCPIP_CODE) TcpIp_IpAddrM_lockIfOnlineShutdown
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  boolean result = FALSE;

  DBG_TCPIP_IPADDRM_LOCKIFONLINESHUTDOWN_ENTRY(localAddrId);

  /* !LINKSTO TcpIp.Design.IpAddrM.EA3.lockIfOnlineShutdown,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_3();
  {
    TcpIp_StateType state = TcpIp_IpAddrM_getIntIfState(localAddrId);
    if((state == TCPIP_STATE_ONLINE) || (state == TCPIP_STATE_SHUTDOWN))
    {
      CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
          &(TCPIP_CFG(localAddr,localAddrId));

      TcpIp_IpAddrM_ctrlLockCounter[localAddrPtr->ctrlIdx]++;
      result = TRUE;
    }
  }
  /* !LINKSTO TcpIp.Design.IpAddrM.EA3.lockIfOnlineShutdown,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_3();

  DBG_TCPIP_IPADDRM_LOCKIFONLINESHUTDOWN_EXIT(result,localAddrId);

  return result;
}

#if (TCPIP_IPV6_ENABLED == STD_ON)

TS_MOD_PRIV_DEFN FUNC(boolean, TCPIP_CODE) TcpIp_IpAddrM_lockIfStartupOnline
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  boolean result = FALSE;

  DBG_TCPIP_IPADDRM_LOCKIFSTARTUPONLINE_ENTRY(localAddrId);

  /* !LINKSTO TcpIp.Design.IpAddrM.EA3.lockIfOnlineShutdown,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_3();
  {
    TcpIp_StateType state = TcpIp_IpAddrM_getIntIfState(localAddrId);
    if((state == TCPIP_STATE_ONLINE) || (state == TCPIP_STATE_STARTUP))
    {
      CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
          &(TCPIP_CFG(localAddr,localAddrId));

      TcpIp_IpAddrM_ctrlLockCounter[localAddrPtr->ctrlIdx]++;
      result = TRUE;
    }
  }
  /* !LINKSTO TcpIp.Design.IpAddrM.EA3.lockIfOnlineShutdown,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_3();

  DBG_TCPIP_IPADDRM_LOCKIFSTARTUPONLINE_EXIT(result,localAddrId);

  return result;
}
#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */

#if (TCPIP_DHCPV4_CLIENT_ENABLED == STD_ON) || (TCPIP_IPV4_LINK_LOCAL_ENABLED == STD_ON) || \
    (TCPIP_DHCPV6_CLIENT_ENABLED == STD_ON) || (TCPIP_NDP_RPD_ENABLED == STD_ON)

TS_MOD_PRIV_DEFN FUNC(boolean, TCPIP_CODE) TcpIp_IpAddrM_lockIfStartupOnlineShutdown
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  boolean result = FALSE;

  DBG_TCPIP_IPADDRM_LOCKIFSTARTUPONLINESHUTDOWN_ENTRY(localAddrId);

  /* !LINKSTO TcpIp.Design.IpAddrM.EA3.lockIfStartupOnlineShutdown,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_3();
  {
    TcpIp_StateType state = TcpIp_IpAddrM_getIntIfState(localAddrId);
    if((state == TCPIP_STATE_STARTUP) ||
       (state == TCPIP_STATE_ONLINE) ||
       (state == TCPIP_STATE_SHUTDOWN)
      )
    {
      CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
          &(TCPIP_CFG(localAddr,localAddrId));
      TCPIP_PRECONDITION_ASSERT(localAddrPtr->ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);
      TcpIp_IpAddrM_ctrlLockCounter[localAddrPtr->ctrlIdx]++;
      result = TRUE;
    }
  }
  /* !LINKSTO TcpIp.Design.IpAddrM.EA3.lockIfStartupOnlineShutdown,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_3();

  DBG_TCPIP_IPADDRM_LOCKIFSTARTUPONLINESHUTDOWN_EXIT(result,localAddrId);

  return result;
}

#endif /* (TCPIP_DHCPV4_CLIENT_ENABLED == STD_ON) || (TCPIP_IPV4_LINK_LOCAL_ENABLED == STD_ON) ||
          (TCPIP_DHCPV6_CLIENT_ENABLED == STD_ON) || (TCPIP_NDP_RPD_ENABLED == STD_ON) */

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_init(void)
{
  uint8_least i; /* iterator */

  DBG_TCPIP_IPADDRM_INIT_ENTRY();

  /* initialize all global variables/state machines */
  TcpIp_IpAddrM_CtrlSM_init();

  /* initialize all controller specific variables /state machines */
  for(i = 0U; i < (uint8_least)TcpIp_PBcfgPtr->ctrlNum; i++)
  {
    TcpIp_IpAddrM_ctrlLockCounter[i] = 0U;
    TcpIp_IpAddrM_ctrlDelayFlag [i] = 0U;
    TcpIp_IpAddrM_RequestComMode_requestedState[i] = TCPIP_STATE_OFFLINE;
    TcpIp_IpAddrM_CtrlSM_initCtrl((uint8)i);
  }

  /* initialize all local address specific variables/state machines */
  for(i = 0U; i < (uint8_least)TcpIp_PBcfgPtr->totalLocalAddrNum; i++)
  {
    TcpIp_IpAddrM_IpAddrAssignment_Event[i] = TCPIP_IPADDRM_ASSIGN_EVENT_NONE;
    TcpIp_IpAddrM_LocalAddrSM_init((TcpIp_LocalAddrIdType)i);
  }

  DBG_TCPIP_IPADDRM_INIT_EXIT();
}

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpAddrM_checkConcreteLocalAddr
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  Std_ReturnType result;

  if( localAddrId >= TcpIp_PBcfgPtr->configLocalAddrNum)
  {
    result = E_NOT_OK;
  }
  else
  {
    CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
        &(TCPIP_CFG(localAddr,localAddrId));

    CONSTP2CONST(TcpIp_ctrlConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlPtr =
        &((TCPIP_CFG(ctrl,localAddrPtr->ctrlIdx)));

    if(ctrlPtr->localAddrIdAny == localAddrId)
    {
      result = E_NOT_OK;
    }
    else
    {
      result = E_OK;
    }
  }
  return result;
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpAddrM_checkIpAddr
(
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) ipAddrPtr,
  TcpIp_IpAddrM_AddrInfoType addrInfo
)
{
  Std_ReturnType result;

  if(ipAddrPtr != NULL_PTR)
  {
    const uint8 ipDomain = TCPIP_IP_DOMAIN_GET(ipAddrPtr->domain);
    const uint8 addrDomain = TCPIP_IPADDRM_ADDRDOMAIN_GET(addrInfo);

    if((ipAddrPtr->domain != TCPIP_AF_INET) &&
       (ipAddrPtr->domain != TCPIP_AF_INET6)
       )
    {
      /* is a not supported family */
      result = E_NOT_OK;
    }
    else if (ipDomain != addrDomain)
    {
      /* ip address not supported by local address */
      result = E_NOT_OK;
    }
    else
    {
      /* ip address is valid */
      result = E_OK;
    }
  }
  else
  {
    /* ip address is valid */
    result = E_OK;
  }

  return result;
}

#endif /* ( TCPIP_DEV_ERROR_DETECT == STD_ON ) */

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_unlockIf(TcpIp_LocalAddrIdType localAddrId)
{
  CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
      &(TCPIP_CFG(localAddr,localAddrId));

  boolean delayedCtrlMainFunction = FALSE;

  DBG_TCPIP_IPADDRM_UNLOCKIF_ENTRY(localAddrId);

  /* !LINKSTO TcpIp.Design.IpAddrM.EA3.unlockIf,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_3();

  /* check if a mainfunction could not drive the cltrl SM due to tx-locking and we unlock the If finally */
  if((TcpIp_IpAddrM_ctrlLockCounter[localAddrPtr->ctrlIdx] == 1U) &&
     (TcpIp_IpAddrM_ctrlDelayFlag[localAddrPtr->ctrlIdx] != 0U)
    )
  {
    /* of so, remember that we drive the ctrl SM now after unlocking */
    delayedCtrlMainFunction = TRUE;
  }

  TCPIP_PRECONDITION_ASSERT(localAddrPtr->ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

  /* defensive programming */
  TCPIP_PRECONDITION_ASSERT
    (TcpIp_IpAddrM_ctrlLockCounter[localAddrPtr->ctrlIdx] > 0U,DET_INTERNAL_API_ID);


  /* unlock the interface from usage */
  TcpIp_IpAddrM_ctrlLockCounter[localAddrPtr->ctrlIdx]--;

  /* !LINKSTO TcpIp.Design.IpAddrM.EA3.unlockIf,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_3();

  /* now perform missed ctrl SM mainfunction calls */
  if(delayedCtrlMainFunction)
  {
    /* no protection against Mainfunction concurrency, since there was recently a locked
       Mainfunction call resulting in this state handling - as locking is rather short compared against
       Mainfunction period, this is unexpected to happen.*/
    TcpIp_IpAddrM_mainFunction_ctrl(localAddrPtr->ctrlIdx,FALSE);
  }

  TS_AtomicAssign8(TcpIp_IpAddrM_ctrlDelayFlag[localAddrPtr->ctrlIdx],0U);

  DBG_TCPIP_IPADDRM_UNLOCKIF_EXIT(localAddrId);
}

TS_MOD_PRIV_DEFN FUNC(boolean, TCPIP_CODE) TcpIp_IpAddrM_autoAssignment
(
  TcpIp_LocalAddrIdType localAddrId,
  TcpIp_IpAddrAssignmentType type
)
{
  boolean result = FALSE;
  CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
    &(TCPIP_CFG(localAddr,localAddrId));

  if((localAddrPtr->assignmByMethod[type].priority_autoAssign&0x01U) != 0x0U)
  {
    result = TRUE;
  }

  return result;
}


TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpAddrM_requestIpAddrAssignment
(
  TcpIp_LocalAddrIdType LocalAddrId,
  TcpIp_IpAddrAssignmentType Type,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) LocalIpAddrPtr
)
{
  Std_ReturnType retval = E_NOT_OK;
  CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
      &(TCPIP_CFG(localAddr,LocalAddrId));
  CONSTP2CONST(TcpIp_ctrlConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlPtr =
          &((TCPIP_CFG(ctrl,localAddrPtr->ctrlIdx)));

  DBG_TCPIP_REQUESTIPADDRASSIGNMENT_ENTRY(LocalAddrId,Type,LocalIpAddrPtr);

  if(ctrlPtr->localAddrIdAny == LocalAddrId)
  {
    uint8_least i;
    const uint8_least startIndex = ctrlPtr->intLocalAddrIdFirst;
    const uint8_least endIndex = startIndex + ctrlPtr->intLocalAddrNum;

    /* loop over all local addresses configured for this controller */
    for(i = startIndex; i < endIndex; i++)
    {
      const uint8 intLocalAddr = (TCPIP_CFG_TYPE(uint8,intIpLocalAddr,i));

      /* only request assignment of configured local addresses */
      if(intLocalAddr < TcpIp_PBcfgPtr->configLocalAddrNum)
      {
        /* request assignment of all manual assignment methods */
        TcpIp_IpAddrM_requestAllAssignments(intLocalAddr);
      }
    }

    retval = E_OK;
  }
  else if(Type == TCPIP_IPADDR_ASSIGNMENT_ALL)
  {
    /* request assignment of all manual assignment methods */
    TcpIp_IpAddrM_requestAllAssignments(LocalAddrId);
    retval = E_OK;
  }
  else
  {
    const TcpIp_IpAddrM_AddrInfoType addrInfo = TcpIp_IpAddrM_getLocalAddrInfo(LocalAddrId);
    const TcpIp_IpAddrAssignmentType intType = TcpIp_IpAddrM_getInternalType(addrInfo, Type);

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
    if(intType == (uint8)TCPIP_IPADDRM_ASSIGN_INVALID)
    {
      TCPIP_REPORT_ERROR(TCPIP_SID_REQUESTIPADDRASSIGNMENT, TCPIP_E_INV_ARG);
    }
    else if(TcpIp_IpAddrM_isAssignmentInvalid(LocalAddrId,intType))
    {
      TCPIP_REPORT_ERROR(TCPIP_SID_REQUESTIPADDRASSIGNMENT, TCPIP_E_INV_ARG);
    }
    else if(TcpIp_IpAddrM_autoAssignment(LocalAddrId,intType))
    {
      TCPIP_REPORT_ERROR(TCPIP_SID_REQUESTIPADDRASSIGNMENT, TCPIP_E_INV_ARG);
    }
    else if(TcpIp_IpAddrM_checkIpAddr(LocalIpAddrPtr, addrInfo) != E_OK)
    {
      TCPIP_REPORT_ERROR(TCPIP_SID_REQUESTIPADDRASSIGNMENT, TCPIP_E_AFNOSUPPORT);
    }
    else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
    {
      retval = TcpIp_IpAddrM_requestSingleAssignment(LocalAddrId, intType, LocalIpAddrPtr);

#if (TCPIP_DEV_ERROR_DETECT == STD_ON )
      if(retval == E_NOT_OK)
      {
        TCPIP_REPORT_ERROR(TCPIP_SID_REQUESTIPADDRASSIGNMENT, TCPIP_EB_E_PARAM_POINTER);
      }
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
    }
  }

  DBG_TCPIP_REQUESTIPADDRASSIGNMENT_EXIT(retval,LocalAddrId,Type,LocalIpAddrPtr);

  return retval;
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpAddrM_releaseIpAddrAssignment
(
  TcpIp_LocalAddrIdType LocalAddrId
)
{
  Std_ReturnType retval = E_NOT_OK;

  DBG_TCPIP_RELEASEIPADDRASSIGNMENT_ENTRY(LocalAddrId);
  /* !LINKSTO TcpIp.Design.IpAddrM.EA2.ReleaseIpAddrAssignment,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_2();
  {
    CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
        &(TCPIP_CFG(localAddr,LocalAddrId));
    CONSTP2CONST(TcpIp_ctrlConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlPtr =
            &((TCPIP_CFG(ctrl,localAddrPtr->ctrlIdx)));

    if(ctrlPtr->localAddrIdAny == LocalAddrId)
    {
      uint8_least i;
      const uint8 startIndex = ctrlPtr->intLocalAddrIdFirst;
      const uint8 endIndex = startIndex + ctrlPtr->intLocalAddrNum;

      for(i = startIndex; i< endIndex; i++)
      {
        TCPIP_PBCFG_PRECONDITION_ASSERT(i, intIpLocalAddrCfg, TCPIP_INTERNAL_API_ID);
        {
          const uint8 intLocalAddr = (TCPIP_CFG_TYPE(uint8,intIpLocalAddr,i));
          retval = E_OK;
          if(intLocalAddr < TcpIp_PBcfgPtr->configLocalAddrNum)
          {
            /* ignore auto-assign methods */
            (void)TcpIp_IpAddrM_releaseAllAssignments(intLocalAddr);
          }
        }
      }
    }
    else
    {
      retval = TcpIp_IpAddrM_releaseAllAssignments(LocalAddrId);
    }
  }
  /* !LINKSTO TcpIp.Design.IpAddrM.EA2.ReleaseIpAddrAssignment,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_2();

  DBG_TCPIP_RELEASEIPADDRASSIGNMENT_EXIT(retval,LocalAddrId);
  return retval;
}

#if ((TCPIP_DHCPV6_FQDN_ENABLED == STD_ON) || (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON))
TS_MOD_PRIV_DEFN FUNC (TcpIp_LocalAddrIdType, TCPIP_CODE) TcpIp_IpAddrM_getLocalAddrOfAssignment
(
  TcpIp_LocalAddrIdType LocalAddrId,
  TcpIp_IpAddrAssignmentType method
)
{

  uint8_least i;
  CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
      &(TCPIP_CFG(localAddr,LocalAddrId));
  CONSTP2CONST(TcpIp_ctrlConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlPtr =
      &((TCPIP_CFG(ctrl,localAddrPtr->ctrlIdx)));
  const uint8_least startIndex = ctrlPtr->intLocalAddrIdFirst;
  const uint8_least endIndex = startIndex + ctrlPtr->intLocalAddrNum;


  /* loop over all local addresses configured for this controller */
  for(i = startIndex; i < endIndex; i++)
  {
    const uint8 intLocalAddr = (TCPIP_CFG_TYPE(uint8,intIpLocalAddr,i));

    if(!TcpIp_IpAddrM_isAssignmentInvalid(intLocalAddr, method))
    {
      LocalAddrId = intLocalAddr;
      break;
    }
  }

  return LocalAddrId;
}
#endif /* ((TCPIP_DHCPV6_FQDN_ENABLED == STD_ON) || (TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON)) */

/*==================[internal function definitions]=========================*/

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_mainFunction_ctrl(uint8 ctrlIdx, boolean checkLock)
{
  boolean unlocked;

  if(checkLock)
  {
    /* !LINKSTO TcpIp.Design.IpAddrM.EA3.MainFunction,1 */
    SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_3();

    if((TcpIp_IpAddrM_ctrlLockCounter[ctrlIdx] == 0U) && (TcpIp_IpAddrM_ctrlDelayFlag[ctrlIdx] == 0U))
    {
      unlocked = TRUE;
    }
    else
    {
      TcpIp_IpAddrM_ctrlDelayFlag[ctrlIdx] = 1U;
      unlocked = FALSE;
    }

    /* !LINKSTO TcpIp.Design.IpAddrM.EA3.MainFunction,1 */
    SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_3();
  }
  else
  {
    unlocked = TRUE;
  }

  /* only perform state transition if this controller state machine is not locked */
  if(unlocked)
  {
    TcpIp_StateType requestedState;

    TcpIp_IpAddrM_CtrlSM_update(ctrlIdx);
    /* !LINKSTO TcpIp.Design.IpAddrM.Atomic.State.MainFunction,1 */
    TS_AtomicAssign8(requestedState,TcpIp_IpAddrM_RequestComMode_requestedState[ctrlIdx]);
    TcpIp_IpAddrM_CtrlSM_request(ctrlIdx,requestedState);

    {
      CONSTP2CONST(TcpIp_ctrlConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlPtr =
          &((TCPIP_CFG(ctrl,ctrlIdx)));

      const uint8_least startIndex = ctrlPtr->intLocalAddrIdFirst;
      const uint8_least endIndex = startIndex + ctrlPtr->intLocalAddrNum;
      uint8_least i;

      /* request/release assignment for all local addresses configured for this controller */
      for(i = startIndex; i < endIndex; i++)
      {
        const uint8 intLocalAddr =
          (TCPIP_CFG_TYPE(uint8,intIpLocalAddr,i));

        TcpIp_IpAddrM_mainFunction_localAddr(intLocalAddr);
      }
    }
  }
}

STATIC FUNC(TcpIp_IpAddrAssignmentType, TCPIP_CODE) TcpIp_IpAddrM_getInternalType
(
  TcpIp_IpAddrM_AddrInfoType addrInfo,
  TcpIp_IpAddrAssignmentType Type
)
{
  TcpIp_IpAddrAssignmentType result = Type;

  if(TCPIP_IPADDRM_IS_ADDRDOMAIN(addrInfo,IPV6))
  {
    switch(Type)
    {
      case TCPIP_IPADDR_ASSIGNMENT_STATIC:
        result = TCPIP_IPADDRM_ASSIGN_STATICV6;
        break;
      case TCPIP_IPADDR_ASSIGNMENT_DHCP:
        result = TCPIP_IPADDRM_ASSIGN_DHCPV6;
        break;
      case TCPIP_IPADDR_ASSIGNMENT_IPV6_ROUTER:
        result = TCPIP_IPADDRM_ASSIGN_IPV6_ROUTER;
        break;
      case TCPIP_IPADDR_ASSIGNMENT_LINKLOCAL:
        result = TCPIP_IPADDRM_ASSIGN_LINKLOCALV6;
        break;
      case TCPIP_IPADDR_ASSIGNMENT_LINKLOCAL_DOIP:
        result = TCPIP_IPADDRM_ASSIGN_INVALID;
        break;
       /* CHECK: NOPARSE */
       default:
        /* should not happen */
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break;
       /* CHECK: PARSE */
    }
  }

  if(TCPIP_IPADDRM_IS_ADDRDOMAIN(addrInfo,IPV4))
  {
    switch(Type)
    {
      case TCPIP_IPADDR_ASSIGNMENT_IPV6_ROUTER:
        result = TCPIP_IPADDRM_ASSIGN_INVALID;
        break;
      default:
        /* default case intended to be empty */
        break;
    }
  }

  return result;
}

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpAddrM_requestSingleAssignment
(
  TcpIp_LocalAddrIdType LocalAddrId,
  TcpIp_IpAddrAssignmentType intType,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) LocalIpAddrPtr
)
{
  Std_ReturnType retval = E_NOT_OK;

  /* !LINKSTO TcpIp.Design.IpAddrM.EA2.RequestIpAddrAssignment,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_2();

  retval = TcpIp_IpAddrM_AssignSM_requestIpAddress(LocalAddrId, intType, LocalIpAddrPtr);

  if(retval == E_OK)
  {
    /* save the assignment request for the particular assignment type */
    TCPIP_IPADDRM_EVENT_SET_REQUEST_FLAG(TcpIp_IpAddrM_IpAddrAssignment_Event[LocalAddrId],intType);
  }

  /* !LINKSTO TcpIp.Design.IpAddrM.EA2.RequestIpAddrAssignment,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_2();

  return retval;
}


STATIC FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_requestAllAssignments
(
  TcpIp_LocalAddrIdType LocalAddrId
)
{
  CONSTP2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
      &(TCPIP_CFG(localAddr,LocalAddrId));
  uint8 j;

  TCPIP_PBCFG_PRECONDITION_ASSERT(localAddrPtr->numAssignm,localAddrCfg[LocalAddrId].assignmByPriority,TCPIP_INTERNAL_API_ID);

  /* loop over all assignment methods */
  for(j=0U; j < localAddrPtr->numAssignm; ++j)
  {
    const TcpIp_IpAddrAssignmentType intType = localAddrPtr->assignmByPriority[j];

    /* Check if TcpIpAssignmentTrigger is AUTOMATIC or MANUAL*/
    if (TcpIp_IpAddrM_autoAssignment(LocalAddrId, intType) == E_OK)
    {
      (void) TcpIp_IpAddrM_requestSingleAssignment(LocalAddrId, intType, NULL_PTR);
    }
  }
}


STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpAddrM_releaseAllAssignments
(
  TcpIp_LocalAddrIdType LocalAddrId
)
{
  P2CONST(TcpIp_localAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrPtr =
    &(TCPIP_CFG(localAddr, LocalAddrId));
  uint8_least j;
  Std_ReturnType retval = E_NOT_OK;

  /* loop all assignments and release the related assignment method */
  for(j = 0U; j < localAddrPtr->numAssignm; j++)
  {
    TcpIp_IpAddrAssignmentType method = localAddrPtr->assignmByPriority[j];

    /* Check if TcpIpAssignmentTrigger is AUTOMATIC or MANUAL*/
    if (TcpIp_IpAddrM_autoAssignment(LocalAddrId, method) == FALSE)
    {
      /* if this release assignment was preceded by a not yet performed request assignment,
         then just clear the assignment event flag */
      if(TCPIP_IPADDRM_EVENT_CHECK_REQUEST_FLAG
          (TcpIp_IpAddrM_IpAddrAssignment_Event[LocalAddrId],localAddrPtr->assignmByPriority[j]))
      {
        TCPIP_IPADDRM_EVENT_CLEAR_REQUEST_FLAG
          (TcpIp_IpAddrM_IpAddrAssignment_Event[LocalAddrId],localAddrPtr->assignmByPriority[j]);
      }
      /* otherwise, just store the release address event flag */
      else
      {
        TCPIP_IPADDRM_EVENT_SET_RELEASE_FLAG
          (TcpIp_IpAddrM_IpAddrAssignment_Event[LocalAddrId],localAddrPtr->assignmByPriority[j]);
      }
      retval = E_OK;
    }
  }
  return retval;
}


#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[end of file]===========================================*/
