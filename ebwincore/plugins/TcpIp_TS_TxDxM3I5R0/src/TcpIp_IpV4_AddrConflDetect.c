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

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_IpV4_Priv.h>      /* own SW-unit private header file */

/*==================[macros]================================================*/

/** \brief Address Conflict Detection is inactive */
#define TCPIP_IPV4_ADDR_CONFL_DETECT_STATE_INACTIVE                       0U
/** \brief Check for address conflicts */
#define TCPIP_IPV4_ADDR_CONFL_DETECT_STATE_OBSERVING                      1U
/** \brief Defend the assigned local address */
#define TCPIP_IPV4_ADDR_CONFL_DETECT_STATE_DEFENDING                      2U

/** \brief minimum interval between defensive ARPs */
#define TCPIP_IPV4_ADDR_CONFL_DEFEND_INTERVAL                            10U

/** \brief Enter state Inactive
 *
 * This function reinitializes the runtime data to its default values.
 *
 * \param[in]  addrConflDetPtr  Pointer to the address conflict detection data structure
 */
#define TcpIp_IpV4_AddrConflDetect_entry_INACTIVE(addrConflDetPtr)  \
  TcpIp_IpV4_AddrConflDetect_initCtrl(addrConflDetPtr)

  /** \brief Enter state Inactive
 *
 * This function informs the address assignment about the address conflict and stops the
 * the address conflict detection.
 *
 * \param[in]  addrConflDetPtr  Pointer to the address conflict detection data structure
 */
#define TcpIp_IpV4_AddrConflDetect_tr_conflict_DEFENDING_INACTIVE(addrConflDetPtr) \
  TcpIp_IpV4_AddrConflDetect_tr_conflict_OBSERVING_INACTIVE(addrConflDetPtr)

/** \brief Restart Address Conflict Detection for the requested local address
 *
 * \param[in] confltPtr  Pointer to the address conflict detection data structure
 * \param[in] addrId     local address which should be observed for conflicts
 * \param[in] cFctPtr    Function pointer to conflict indication function
 * \param[in] mech       defend mechanism used to defend address
 */
#define TcpIp_IpV4_AddrConflDetect_tr_activate_ACTIVE_OBSERVING(confltPtr, addrId, cFctPtr, mech) \
    TcpIp_IpV4_AddrConflDetect_tr_activate_INACTIVE_OBSERVING(confltPtr, addrId, cFctPtr, mech)

  /** \brief Enter state Inactive
 *
 * This function reinitializes the runtime data to its default values.
 *
 * \param[in]  addrConflDetPtr  Pointer to the address conflict detection data structure
 */
#define TcpIp_IpV4_AddrConflDetect_tr_activate_ACTIVE_INACTIVE(addrConflDetPtr) \
  TcpIp_IpV4_AddrConflDetect_initCtrl(addrConflDetPtr)

/*==================[type definitions]======================================*/
#if (TCPIP_IPV4_ENABLED == STD_ON)

typedef struct
{
  TcpIp_IpV4_AddrConflDetect_FctPtrType conflictFctPtr;
  /**< Callback pointer to report address conflict to assignment */
  TcpIp_LocalAddrIdType localAddrId;
  /**< Local address which is probed for duplicates */
  uint8 state;
  /**< State of Address Conflict Detection state machine */
  uint8 timer;
  /**< timer for defend interval */
  uint8 mechanism;
  /**< selected defend mechanism */
}TcpIp_IpV4_AddrConflDetectDataType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/


#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief Address conflict detected
 *
 * This function is called when an address conflict was detected in an ARP message.
 *
 * \param[in]  addrConflDetPtr  Pointer to the address conflict detection data structure
 * \param[in]  ipFilter         IP address for which a conflict was detected
 * \param[in]  ctrlIdx          Controller on which the conflict was received
 * \param[in]  remotePhysAddr   Pointer to remote physical address
 * \param[in]  localPhysAddr    Pointer to local physical address
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_AddrConflDetect_conflict
(
  P2VAR(TcpIp_IpV4_AddrConflDetectDataType, AUTOMATIC, TCPIP_APPL_DATA) addrConflDetPtr,
  TcpIp_IpAddrConstPtr ipFilter,
  uint8 ctrlIdx,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) remotePhysAddr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) localPhysAddr
);

/** \brief Defend timeout reached
 *
 * This function is called when the defend timeout is reached.
 *
 * \param[in]  addrConflDetPtr  Pointer to the address conflict detection data structure
 */
STATIC FUNC(void, TCPIP_CODE)  TcpIp_IpV4_AddrConflDetect_timeout
(
  P2VAR(TcpIp_IpV4_AddrConflDetectDataType, AUTOMATIC, TCPIP_APPL_DATA) addrConflDetPtr
);

/** \brief Address conflict detected
 *
 * This function informs the address assignment about the address conflict and stops the
 * the address conflict detection.
 *
 * \param[in]  addrConflDetPtr  Pointer to the address conflict detection data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_AddrConflDetect_tr_conflict_OBSERVING_INACTIVE
(
  P2VAR(TcpIp_IpV4_AddrConflDetectDataType, AUTOMATIC, TCPIP_APPL_DATA) addrConflDetPtr
);

/** \brief Address conflict detected
 *
 * This function defends the IP address by transmitting an ARP announce.
 *
 * \param[in]  addrConflDetPtr  Pointer to the address conflict detection data structure
 * \param[in]  ipFilter         IP address for which a conflict was detected
 * \param[in]  ctrlIdx          Controller on which the conflict was received
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_AddrConflDetect_tr_conflict_OBSERVING_DEFENDING
(
  P2VAR(TcpIp_IpV4_AddrConflDetectDataType, AUTOMATIC, TCPIP_APPL_DATA) addrConflDetPtr,
  TcpIp_IpAddrConstPtr ipFilter,
  uint8 ctrlIdx
);

/** \brief Starts Address Conflict Detection for the requested local address
 *
 * \param[in] addrConflDetPtr Pointer to the address conflict detection data structure
 * \param[in] localAddrId     local address which should be observed for conflicts
 * \param[in] conflictFctPtr  Function pointer to conflict indication function
 * \param[in] mechanism       defend mechanism used to defend address
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_AddrConflDetect_tr_activate_INACTIVE_OBSERVING
(
  P2VAR(TcpIp_IpV4_AddrConflDetectDataType, AUTOMATIC, TCPIP_APPL_DATA) addrConflDetPtr,
  TcpIp_LocalAddrIdType localAddrId,
  TcpIp_IpV4_AddrConflDetect_FctPtrType conflictFctPtr,
  uint8 mechanism
);

/** \brief Defend timeout reached
 *
 * This function indicates that no address conflict was detected after defend interval.
 *
 * \param[in]  addrConflDetPtr  Pointer to the address conflict detection data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_AddrConflDetect_tr_timeout_DEFENDING_OBSERVING
(
  P2VAR(TcpIp_IpV4_AddrConflDetectDataType, AUTOMATIC, TCPIP_APPL_DATA) addrConflDetPtr
);


/** \brief Entry OBSERVING
 *
 * This function starts Address Conflict Detection
 *
 * \param[in]  addrConflDetPtr  Pointer to the address conflict detection data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_AddrConflDetect_entry_OBSERVING
(
  P2VAR(TcpIp_IpV4_AddrConflDetectDataType, AUTOMATIC, TCPIP_APPL_DATA) addrConflDetPtr
);

/** \brief Entry DEFENDING
 *
 * This function transmits an ARP announce and starts the defend timer
 *
 * \param[in]  addrConflDetPtr  Pointer to the address conflict detection data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_AddrConflDetect_entry_DEFENDING
(
  P2VAR(TcpIp_IpV4_AddrConflDetectDataType, AUTOMATIC, TCPIP_APPL_DATA) addrConflDetPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) ipFilter,
  uint8 ctrlIdx
);

/** \brief Initialize address conflict detection for controller
 *
 * This function reinitializes the runtime data to its default values.
 *
 * \param[in]  addrConflDetPtr  Pointer to the address conflict detection data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_AddrConflDetect_initCtrl
(
  P2VAR(TcpIp_IpV4_AddrConflDetectDataType, AUTOMATIC, TCPIP_APPL_DATA) addrConflDetPtr
);

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

#define TCPIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

/* Address Conflict Detection memory allocation */
STATIC VAR(TcpIp_IpV4_AddrConflDetectDataType,TCPIP_VAR_CLEARED)
  TcpIp_IpV4_AddrConflDetect_Data[TCPIP_NUM_ETHIFCTRL];

#define TCPIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_AddrConflDetect_activate
(
  TcpIp_LocalAddrIdType localAddrId,
  TcpIp_IpV4_AddrConflDetect_FctPtrType conflictFctPtr,
  uint8 mechanism
)
{
  uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
  P2VAR(TcpIp_IpV4_AddrConflDetectDataType, AUTOMATIC, TCPIP_APPL_DATA) addrConflDetPtr;

  TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

  DBG_TCPIP_IPV4_ADDRCONFLDETECT_ACTIVATE_ENTRY(localAddrId, conflictFctPtr, mechanism);

  addrConflDetPtr = &TcpIp_IpV4_AddrConflDetect_Data[ctrlIdx];

  TCPIP_PRECONDITION_ASSERT
    ((mechanism <= TCPIP_IPV4_ADDR_CONFL_DETECT_MECHANISM_DEFEND_INF), TCPIP_INTERNAL_API_ID);

  switch(addrConflDetPtr->state)
  {
    case TCPIP_IPV4_ADDR_CONFL_DETECT_STATE_INACTIVE:
    {
      if(mechanism != TCPIP_IPV4_ADDR_CONFL_DETECT_MECHANISM_OFF)
      {
        /* start address conflict detection */
        TcpIp_IpV4_AddrConflDetect_tr_activate_INACTIVE_OBSERVING
          (addrConflDetPtr, localAddrId, conflictFctPtr, mechanism);
      }
      break;
    }
    case TCPIP_IPV4_ADDR_CONFL_DETECT_STATE_OBSERVING:  /* intended fall through */
    case TCPIP_IPV4_ADDR_CONFL_DETECT_STATE_DEFENDING:  /* intended fall through */
    {
      if(mechanism == TCPIP_IPV4_ADDR_CONFL_DETECT_MECHANISM_OFF)
      {
        /* stop address conflict detection */
        TcpIp_IpV4_AddrConflDetect_tr_activate_ACTIVE_INACTIVE(addrConflDetPtr);
      }
      else
      {
        /* restart address conflict detection */
        TcpIp_IpV4_AddrConflDetect_tr_activate_ACTIVE_OBSERVING
          (addrConflDetPtr, localAddrId, conflictFctPtr, mechanism);
      }
      break;
    }
    /* CHECK: NOPARSE */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  DBG_TCPIP_IPV4_ADDRCONFLDETECT_ACTIVATE_EXIT(localAddrId, conflictFctPtr, mechanism);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_AddrConflDetect_init(void)
{
  uint8_least i;

  /* loop over all configured controllers and initialize data structures */
  for(i = 0U; i < TCPIP_NUM_ETHIFCTRL; i++)
  {
    CONSTP2VAR(TcpIp_IpV4_AddrConflDetectDataType, AUTOMATIC, TCPIP_APPL_DATA) addrConflDetPtr =
      &TcpIp_IpV4_AddrConflDetect_Data[i];

    TcpIp_IpV4_AddrConflDetect_initCtrl(addrConflDetPtr);
  }
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_AddrConflDetect_mainFunction
(
  uint16 timerTick
)
{
  uint8_least i;

  if((timerTick % TCPIP_MAINFUNCTION_PRESCALER_1S) == TCPIP_TIMER_OFFSET_100MS_AUTOIP_TIMEOUT)
  {
    /* loop over all configured controllers */
    for (i = 0U; i < TCPIP_NUM_ETHIFCTRL; ++i)
    {
      P2VAR(TcpIp_IpV4_AddrConflDetectDataType, AUTOMATIC, TCPIP_APPL_DATA) addrConflDetPtr =
          &TcpIp_IpV4_AddrConflDetect_Data[i];

      if(addrConflDetPtr->timer > 0U)
      {
        addrConflDetPtr->timer--;

        if(addrConflDetPtr->timer == 0U)
        {
          TcpIp_IpV4_AddrConflDetect_timeout(addrConflDetPtr);
        }
      }
    }
  }
}

TS_MOD_PRIV_DEFN FUNC(boolean, TCPIP_CODE) TcpIp_IpV4_AddrConflDetect_arpDetection
(
  uint8 ctrlIdx,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr,
  uint32 srcIpAddr
)
{
  boolean conflict = FALSE;
  TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);
  {
    CONSTP2VAR(TcpIp_IpV4_AddrConflDetectDataType, AUTOMATIC, TCPIP_APPL_DATA) addrConflDetPtr =
      &TcpIp_IpV4_AddrConflDetect_Data[ctrlIdx];


    switch (addrConflDetPtr->state)
    {
      case TCPIP_IPV4_ADDR_CONFL_DETECT_STATE_INACTIVE:
        /* don't check for ip address */
        break;
      case TCPIP_IPV4_ADDR_CONFL_DETECT_STATE_OBSERVING:  /* intended fall through */
      case TCPIP_IPV4_ADDR_CONFL_DETECT_STATE_DEFENDING:  /* intended fall through */
      {
        TcpIp_IpAddrConstPtr ipFilter;
        const TcpIp_LocalAddrIdType localAddrId = addrConflDetPtr->localAddrId;
        uint8 localEthAddr[TCPIP_ETH_ADDR_LENGTH];

        TCPIP_PRECONDITION_ASSERT
          (TCPIP_IPADDRM_IS_ADDRDOMAIN((TCPIP_CFG(localAddr, localAddrId)).addrType, IPV4),
          TCPIP_INTERNAL_API_ID);

        TcpIp_IpAddrM_getIpAddress(localAddrId, &ipFilter);

        /* read interface hardware address */
        EthIf_GetPhysAddr((TCPIP_CFG(ctrl, ctrlIdx)).ethIfCtrlIdx, localEthAddr);

        if (*ipFilter == srcIpAddr)
        {
          /* we have an address conflict - don't further process this ARP message
            later on - ignore it */
          conflict = TRUE;

          /* check if this is an local generated frame -TRUE means different addresses */
          if (TcpIp_EthAddrCmp(physAddrPtr, localEthAddr) == TRUE)
          {
            TcpIp_IpV4_AddrConflDetect_conflict
              (addrConflDetPtr, ipFilter, ctrlIdx, physAddrPtr, localEthAddr);
          }
        }
        break;
      }
      /* CHECK: NOPARSE */
      default:
        /* should not happen */
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break;
      /* CHECK: PARSE */
    }
  }
  return conflict;
}

#if(TCPIP_DAD_FCT_ENABLE == STD_ON)
TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_AddrConflDetect_conflictCallout
(
  TcpIp_LocalAddrIdType IpAddrId,
  TcpIp_IpAddrConstPtr IpAddr,
  P2CONST(uint8, AUTOMATIC,TCPIP_APPL_CONST) LocalPhysAddrPtr,
  P2CONST(uint8, AUTOMATIC,TCPIP_APPL_CONST) RemotePhysAddrPtr
)
{
  TcpIp_SockAddrInetType sourceInetAddr;
  DBG_TCPIP_IPV4_ADDRCONFLDETECT_CONFLICTCALLOUT_ENTRY(IpAddrId,IpAddr,LocalPhysAddrPtr,RemotePhysAddrPtr);

  sourceInetAddr.domain = TCPIP_AF_INET;
  sourceInetAddr.addr[0U] = *((P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA))IpAddr);

  TcpIp_DADAddressConflictAPI
  (
    IpAddrId,
    /* Deviation MISRAC2012-2 */
    (P2CONST(TcpIp_SockAddrType, AUTOMATIC,TCPIP_APPL_CONST))&sourceInetAddr,
    LocalPhysAddrPtr,
    RemotePhysAddrPtr
  );
  DBG_TCPIP_IPV4_ADDRCONFLDETECT_CONFLICTCALLOUT_EXIT(IpAddrId,IpAddr,LocalPhysAddrPtr,RemotePhysAddrPtr);
}
#endif /* (TCPIP_DAD_FCT_ENABLE == STD_ON) */

/*==================[internal function definitions]=========================*/

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_AddrConflDetect_conflict
(
  P2VAR(TcpIp_IpV4_AddrConflDetectDataType, AUTOMATIC, TCPIP_APPL_DATA) addrConflDetPtr,
  TcpIp_IpAddrConstPtr ipFilter,
  uint8 ctrlIdx,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) remotePhysAddr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) localPhysAddr
)
{
  switch(addrConflDetPtr->state)
  {
    case TCPIP_IPV4_ADDR_CONFL_DETECT_STATE_OBSERVING:
    {
      TCPIP_PRECONDITION_ASSERT
        ((addrConflDetPtr->mechanism == TCPIP_IPV4_ADDR_CONFL_DETECT_MECHANISM_ABANDON) ||
         (addrConflDetPtr->mechanism == TCPIP_IPV4_ADDR_CONFL_DETECT_MECHANISM_DEFEND) ||
         (addrConflDetPtr->mechanism == TCPIP_IPV4_ADDR_CONFL_DETECT_MECHANISM_DEFEND_INF),
         TCPIP_INTERNAL_API_ID
        );

#if(TCPIP_DAD_FCT_ENABLE == STD_ON)
      {
        TcpIp_IpV4_AddrConflDetect_conflictCallout
        (
          addrConflDetPtr->localAddrId,
          ipFilter,
          localPhysAddr,
          remotePhysAddr
        );
      }
#else
      TS_PARAM_UNUSED(remotePhysAddr);
      TS_PARAM_UNUSED(localPhysAddr);
#endif /* (TCPIP_DAD_FCT_ENABLE == STD_ON) */

      if(addrConflDetPtr->mechanism == TCPIP_IPV4_ADDR_CONFL_DETECT_MECHANISM_ABANDON)
      {
        /* mechanism = abandon: abandon the address */
        TcpIp_IpV4_AddrConflDetect_tr_conflict_OBSERVING_INACTIVE(addrConflDetPtr);
      }
      else
      {
        /* mechanism = defend or defend inf: send an ARP announce */
        TcpIp_IpV4_AddrConflDetect_tr_conflict_OBSERVING_DEFENDING
          (addrConflDetPtr, ipFilter, ctrlIdx);
      }
      break;
    }
    case TCPIP_IPV4_ADDR_CONFL_DETECT_STATE_DEFENDING:
    {
      TCPIP_PRECONDITION_ASSERT
        ((addrConflDetPtr->mechanism == TCPIP_IPV4_ADDR_CONFL_DETECT_MECHANISM_DEFEND) ||
         (addrConflDetPtr->mechanism == TCPIP_IPV4_ADDR_CONFL_DETECT_MECHANISM_DEFEND_INF),
         TCPIP_INTERNAL_API_ID
        );

#if(TCPIP_DAD_FCT_ENABLE == STD_ON)
      {
        TcpIp_IpV4_AddrConflDetect_conflictCallout
        (
          addrConflDetPtr->localAddrId,
          ipFilter,
          localPhysAddr,
          remotePhysAddr
        );
      }
#endif /* (TCPIP_DAD_FCT_ENABLE == STD_ON) */

      if(addrConflDetPtr->mechanism == TCPIP_IPV4_ADDR_CONFL_DETECT_MECHANISM_DEFEND)
      {
        /* mechanism = defend: abandon the address */
        TcpIp_IpV4_AddrConflDetect_tr_conflict_DEFENDING_INACTIVE(addrConflDetPtr);
      }
      else
      {
        /* mechanism = defend inf: ignore conflict during defend interval */
      }
      break;
    }
    /* CHECK: NOPARSE */
    case TCPIP_IPV4_ADDR_CONFL_DETECT_STATE_INACTIVE:   /* intended fall through */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }
}

STATIC FUNC(void, TCPIP_CODE)  TcpIp_IpV4_AddrConflDetect_timeout
(
  P2VAR(TcpIp_IpV4_AddrConflDetectDataType, AUTOMATIC, TCPIP_APPL_DATA) addrConflDetPtr
)
{
  switch(addrConflDetPtr->state)
  {
    case TCPIP_IPV4_ADDR_CONFL_DETECT_STATE_DEFENDING:
    {
      TcpIp_IpV4_AddrConflDetect_tr_timeout_DEFENDING_OBSERVING(addrConflDetPtr);
      break;
    }
    /* CHECK: NOPARSE */
    case TCPIP_IPV4_ADDR_CONFL_DETECT_STATE_OBSERVING:  /* intended fall through */
    case TCPIP_IPV4_ADDR_CONFL_DETECT_STATE_INACTIVE:   /* intended fall through */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }
}

/* Deviation MISRAC2012-1 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_AddrConflDetect_tr_conflict_OBSERVING_INACTIVE
(
  P2VAR(TcpIp_IpV4_AddrConflDetectDataType, AUTOMATIC, TCPIP_APPL_DATA) addrConflDetPtr
)
{
  /* exit code */

  /* transitional code */
  {
    /* Inform address assignment to abandon address */
    addrConflDetPtr->conflictFctPtr(addrConflDetPtr->localAddrId);
  }

  /* entry code */
  TcpIp_IpV4_AddrConflDetect_entry_INACTIVE(addrConflDetPtr);
}

/* Deviation MISRAC2012-1 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_AddrConflDetect_tr_conflict_OBSERVING_DEFENDING
(
  P2VAR(TcpIp_IpV4_AddrConflDetectDataType, AUTOMATIC, TCPIP_APPL_DATA) addrConflDetPtr,
  TcpIp_IpAddrConstPtr ipFilter,
  uint8 ctrlIdx
)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  TcpIp_IpV4_AddrConflDetect_entry_DEFENDING(addrConflDetPtr, ipFilter, ctrlIdx);
}

/* Deviation MISRAC2012-1 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_AddrConflDetect_tr_activate_INACTIVE_OBSERVING
(
  P2VAR(TcpIp_IpV4_AddrConflDetectDataType, AUTOMATIC, TCPIP_APPL_DATA) addrConflDetPtr,
  TcpIp_LocalAddrIdType localAddrId,
  TcpIp_IpV4_AddrConflDetect_FctPtrType conflictFctPtr,
  uint8 mechanism
)
{
  /* exit code */

  /* transitional code */
  {
    addrConflDetPtr->localAddrId = localAddrId;
    addrConflDetPtr->conflictFctPtr = conflictFctPtr;
    addrConflDetPtr->mechanism = mechanism;
  }

  /* entry code */
  TcpIp_IpV4_AddrConflDetect_entry_OBSERVING(addrConflDetPtr);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_AddrConflDetect_tr_timeout_DEFENDING_OBSERVING
(
  P2VAR(TcpIp_IpV4_AddrConflDetectDataType, AUTOMATIC, TCPIP_APPL_DATA) addrConflDetPtr
)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  TcpIp_IpV4_AddrConflDetect_entry_OBSERVING(addrConflDetPtr);
}

/* Deviation MISRAC2012-1 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_AddrConflDetect_entry_OBSERVING
(
  P2VAR(TcpIp_IpV4_AddrConflDetectDataType, AUTOMATIC, TCPIP_APPL_DATA) addrConflDetPtr
)
{
  addrConflDetPtr->state = TCPIP_IPV4_ADDR_CONFL_DETECT_STATE_OBSERVING;
}

/* Deviation MISRAC2012-1 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_AddrConflDetect_entry_DEFENDING
(
  P2VAR(TcpIp_IpV4_AddrConflDetectDataType, AUTOMATIC, TCPIP_APPL_DATA) addrConflDetPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) ipFilter,
  uint8 ctrlIdx
)
{
  /* address conflict detected - send an announcement */
  if(TcpIp_IpAddrM_lockIfOnlineShutdown(addrConflDetPtr->localAddrId))
  {
    TcpIp_IpV4_ArpTransmitAnnounce(*ipFilter, ctrlIdx);
    TcpIp_IpAddrM_unlockIf(addrConflDetPtr->localAddrId);
  }

  /* start defend interval */
  addrConflDetPtr->timer = TCPIP_IPV4_ADDR_CONFL_DEFEND_INTERVAL * 10U;
  addrConflDetPtr->state = TCPIP_IPV4_ADDR_CONFL_DETECT_STATE_DEFENDING;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_AddrConflDetect_initCtrl
(
  P2VAR(TcpIp_IpV4_AddrConflDetectDataType, AUTOMATIC, TCPIP_APPL_DATA) addrConflDetPtr
)
{
  addrConflDetPtr->localAddrId = 255U;
  addrConflDetPtr->conflictFctPtr = NULL_PTR;
  addrConflDetPtr->state = TCPIP_IPV4_ADDR_CONFL_DETECT_STATE_INACTIVE;
  addrConflDetPtr->mechanism = TCPIP_IPV4_ADDR_CONFL_DETECT_MECHANISM_OFF;
  addrConflDetPtr->timer = 0U;
}

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#else /* (TCPIP_IPV4_ENABLED == STD_ON) */
  TS_PREVENTEMPTYTRANSLATIONUNIT
#endif /* TCPIP_IPV4_ENABLED == STD_ON */

/*==================[end of file]===========================================*/
