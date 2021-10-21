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

#include <TcpIp_Ndp_Priv.h>       /* own SW-unit private file */
#include <TcpIp_IpV6_Int.h>       /* used SW-unit interface file */
#include <TcpIp_Trace.h>          /* debug and trace macros; must be last */

/*==================[macros]================================================*/

#if ((TCPIP_IPV6_ENABLED == STD_ON) && (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON))

/* \brief States of the IpV6 Duplicate Address Detection state machine */
#define TCPIP_NDP_DAD_STATE_INACTIVE                             0U
#define TCPIP_NDP_DAD_STATE_START_DELAY                          1U
#define TCPIP_NDP_DAD_STATE_PROBING                              2U

/* \brief Invalid DAD values */
#define TCPIP_NDP_DAD_INVALID_INDEX                             255U

/** \brief Sends initial solicitation
 *
 * \param[in]  localAddrId  source address identifier
 * \param[in]  dadPtr       Entry reference containing information about the probed address
 */
#define TcpIp_Ndp_Dad_tr_dadTimeout_START_DELAY_PROBING(localAddrId, dadPtr)  \
  TcpIp_Ndp_Dad_entry_PROBING(localAddrId, dadPtr);

/** \brief Retrieves the address Id related to the Duplicate address process
 *
 * \param[in] dadIndex  Duplicate address process identifier
 */
#define TcpIp_Ndp_Dad_getLocalAddrId(dadIndex) \
  (TS_UNCHECKEDGETCFG(TcpIp_PBcfgPtr,TcpIp_LocalAddrIdType,TCPIP,TcpIp_PBcfgPtr->ndpConfig.IpV6NdpDadIndex2LocalAddrIdRef)[(dadIndex)])

 /** \brief Initiates delay until start of probing
 *
 * \param[in]  localAddrId  source address identifier
 * \param[in]  dadPtr       Entry reference containing information about the probed address
 */
#define TcpIp_Ndp_Dad_tr_duplAddrDetect_INACTIVE_START_DELAY(localAddrId, dadPtr, ipAddr, ndpDadResultFctPtr) \
  TcpIp_Ndp_Dad_entry_START_DELAY(localAddrId, dadPtr, ipAddr, ndpDadResultFctPtr)

/** \brief Initializes DAD entry to unused
 *
 * \param[in]  dadPtr  Entry reference containing to be reinitialized
 */
#define TcpIp_Ndp_Dad_tr_abort_PROBING_INACTIVE(dadPtr) \
  TcpIp_Ndp_Dad_entry_INACTIVE(dadPtr)

/** \brief Initializes DAD entry to unused
 *
 * \param[in]  dadPtr  Entry reference containing to be reinitialized
 */
#define TcpIp_Ndp_Dad_tr_abort_START_DELAY_INACTIVE(dadPtr)                   \
  TcpIp_Ndp_Dad_entry_INACTIVE(dadPtr)


/*==================[type definitions]======================================*/
typedef struct
{
  /**< Neighbour cache entry IP address */
  uint32 ipProbe[TCPIP_IPV6_ADDR_SIZE];
  /**< Callback pointer to report probe result to */
  TcpIp_Ndp_dadResult_FctPtrType dadResultFctPtr;
  /**< delay timer (in seconds) */
  uint8 timer;
  /**< count of remaining probes */
  uint8 rtrCount;
  /**< state */
  uint8 state;
}TcpIp_Ndp_Dad_DataType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/
#define TCPIP_START_SEC_CODE
#include <MemMap.h>

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Dad_timeout
(
  uint8 localAddrId,
  P2VAR(TcpIp_Ndp_Dad_DataType,AUTOMATIC,TCPIP_VAR_CLEARED) dadPtr
);

/** \brief Retransmits solicitation
 *
 * Remains in state PROBING and uses up one out of allowed number of DAD solicitation
 * retransmissions
 *
 * \param[in]  localAddrId  source address identifier
 * \param[in]  dadPtr       Entry reference containing information about the probed address
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Dad_tr_timeout_self_PROBING
(
  uint8 localAddrId,
  P2VAR(TcpIp_Ndp_Dad_DataType,AUTOMATIC,TCPIP_VAR_CLEARED) dadPtr
);

/** \brief Initializes DAD entry to unused
 *
 * \param[in]  dadPtr  Entry reference containing to be reinitialized
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Dad_entry_INACTIVE
(
  P2VAR(TcpIp_Ndp_Dad_DataType,AUTOMATIC,TCPIP_VAR_CLEARED) dadPtr
);

/** \brief Reports address conflict during START_DELAY
 *
 * Reports to the unit that initiated DAD that the probed address is not unique.
 *
 * \param[in]  localAddrId  source address identifier
 * \param[in]  dadPtr       Entry reference containing information about the probed address
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Dad_tr_checkDupplicate_START_DELAY_INACTIVE
(
  uint8 localAddrId,
  P2VAR(TcpIp_Ndp_Dad_DataType,AUTOMATIC,TCPIP_VAR_CLEARED) dadPtr
);

/** \brief Reports address uniqeneess
 *
 * Reports to the unit that initiated DAD that the probed address is unique.
 *
 * \param[in]  localAddrId  source address identifier
 * \param[in]  dadPtr       Entry reference containing information about the probed address
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Dad_tr_timeout_PROBING_INACTIVE
(
  uint8 localAddrId,
  P2VAR(TcpIp_Ndp_Dad_DataType,AUTOMATIC,TCPIP_VAR_CLEARED) dadPtr
);

/** \brief Sends initial solicitation
 *
 * Starts sending out DAD solicitations to determine if the address is being used by a neighbor.
 * This is considered as PROBING state
 *
 * \param[in]  localAddrId  source address identifier
 * \param[in]  dadPtr       Entry reference containing information about the probed address
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Dad_entry_PROBING
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_Ndp_Dad_DataType,AUTOMATIC,TCPIP_VAR_CLEARED) dadPtr
);

/** \brief Delays beggining of probing
 *
 * Delays transmission of first DAD solicitation. Delay is used whether it is enabled and whether
 * DAD is configured as non optimistic.
 *
 * \param[in]  localAddrId         source address identifier
 * \param[in]  dadPtr              Entry reference containing information about the probed address
 * \param[in]  ipAddress           Pointer to proved address
 * \param[in]  ndpDadResultFctPtr  Pointer to unit-specific function that initiated DAD
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Dad_entry_START_DELAY
(
  uint8 localAddrId,
  P2VAR(TcpIp_Ndp_Dad_DataType,AUTOMATIC,TCPIP_VAR_CLEARED) dadPtr,
  P2CONST(uint32,AUTOMATIC,TCPIP_VAR_CLEARED) ipAddress,
  TcpIp_Ndp_dadResult_FctPtrType ndpDadResultFctPtr
);

/** \brief transmit solicitation to probe address
 *
 * \param[in] localAddrId     Address Id
 * \param[in] ipProbe         Address to probe
 */
STATIC FUNC(void, TCPIP_CODE)TcpIp_Ndp_Dad_txNeighSol
(
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(uint32,AUTOMATIC,TCPIP_VAR_CLEARED) ipProbe
);

/** \brief Reports address conflict during PROBING
 *
 * Reports to the unit that initiated DAD that the probed address is not unique.
 *
 * \param[in]  localAddrId  source address identifier
 * \param[in]  dadPtr       Entry reference containing information about the probed address
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Dad_tr_checkDuplicate_PROBING_INACTIVE
(
  uint8 localAddrId,
  P2VAR(TcpIp_Ndp_Dad_DataType,AUTOMATIC,TCPIP_VAR_CLEARED) dadPtr
);

#define TCPIP_STOP_SEC_CODE
#include <MemMap.h>

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

#define TCPIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <MemMap.h>

  /* runtime data for duplicate address detection */
STATIC VAR(TcpIp_Ndp_Dad_DataType, TCPIP_VAR_CLEARED)
  TcpIp_Ndp_Dad_Cache[TCPIP_NDP_DAD_NUM_OF_ENTRIES];

#define TCPIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <MemMap.h>

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <MemMap.h>

/** \brief Initialize DAD entries */
TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ndp_Dad_init(void)
{
  uint8 i;
  for(i=0U;i<TCPIP_NDP_DAD_NUM_OF_ENTRIES;i++)
  {
    P2VAR(TcpIp_Ndp_Dad_DataType,AUTOMATIC,TCPIP_VAR_CLEARED) dadPtr =
      &TcpIp_Ndp_Dad_Cache[i];
    TcpIp_Ndp_Dad_entry_INACTIVE(dadPtr);
  }
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ndp_Dad_mainFunction
(
  uint16 timerTick
)
{
  uint8 i;

  if((timerTick % TCPIP_MAINFUNCTION_PRESCALER_100MS) == TCPIP_TIMER_OFFSET_100MS_NDP_DAD_TABLE)
  {
    /* go through all addreses */
    for (i = 0U; i < TCPIP_NDP_DAD_NUM_OF_ENTRIES; i++)
    {
      P2VAR(TcpIp_Ndp_Dad_DataType,AUTOMATIC,TCPIP_VAR_CLEARED) dadProcessPtr =
        &TcpIp_Ndp_Dad_Cache[i];

      if(dadProcessPtr->timer > 0U)
      {
        dadProcessPtr->timer--;

        if(dadProcessPtr->timer == 0U)
        {
          const TcpIp_LocalAddrIdType localAddrId =
            TcpIp_Ndp_Dad_getLocalAddrId(i);
          TcpIp_Ndp_Dad_timeout(localAddrId, dadProcessPtr);
        }
      }
    }
  }
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ndp_Dad_start
(
  TcpIp_Ndp_dadResult_FctPtrType ndpDadResultFctPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) ipAddress,
  TcpIp_LocalAddrIdType localAddrId
)
{
  uint8 dadIndex = (TCPIP_CFG(localAddr,localAddrId)).ipV6NdpDadIndex;
  P2VAR(TcpIp_Ndp_Dad_DataType,AUTOMATIC,TCPIP_VAR_CLEARED) dadPtr =
    &TcpIp_Ndp_Dad_Cache[dadIndex];

  if(dadPtr->state == TCPIP_NDP_DAD_STATE_INACTIVE)
  {
    TcpIp_Ndp_Dad_tr_duplAddrDetect_INACTIVE_START_DELAY(localAddrId, dadPtr, ipAddress, ndpDadResultFctPtr);
  }
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ndp_Dad_abort
(
  uint8 localAddrId
)
{
  uint8 dadIndex = (TCPIP_CFG(localAddr,localAddrId)).ipV6NdpDadIndex;
  P2VAR(TcpIp_Ndp_Dad_DataType,AUTOMATIC,TCPIP_VAR_CLEARED) dadPtr =
    &TcpIp_Ndp_Dad_Cache[dadIndex];

  switch(dadPtr->state)
  {
    case TCPIP_NDP_DAD_STATE_START_DELAY:
      TcpIp_Ndp_Dad_tr_abort_START_DELAY_INACTIVE(dadPtr);
      break;
    case TCPIP_NDP_DAD_STATE_PROBING:
      TcpIp_Ndp_Dad_tr_abort_PROBING_INACTIVE(dadPtr);
      break;
    /* CHECK: NOPARSE */
    case TCPIP_NDP_DAD_STATE_INACTIVE: /* intended fall through */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Ndp_Dad_checkDuplicate
(
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) targetIp
)
{
  TcpIp_ReturnType result = TCPIP_E_NOT_OK;
  VAR(TcpIp_LocalAddrIdType, TCPIP_APPL_DATA) targetId = TCPIP_LOCALADDRID_INVALID;
  uint8 dadIndex = TCPIP_NDP_DAD_INVALID_INDEX;
  P2VAR(TcpIp_Ndp_Dad_DataType,AUTOMATIC,TCPIP_VAR_CLEARED) dadPtr;
  uint8 i;

  for(i=0U;i<TCPIP_NDP_DAD_NUM_OF_ENTRIES;i++)
  {
    if(TcpIp_Ndp_Dad_Cache[i].state != TCPIP_NDP_DAD_STATE_INACTIVE)
    {
      Std_ReturnType retVal = TS_MemCmp(targetIp, TcpIp_Ndp_Dad_Cache[i].ipProbe, TCPIP_IPV6_ADDR_BYTE_SIZE);
      if(retVal == E_OK)
      {
        targetId = TcpIp_Ndp_Dad_getLocalAddrId(i);
        dadIndex = i;
        /* transition */
        break;
      }
    }
  }

  if(dadIndex != TCPIP_NDP_DAD_INVALID_INDEX)
  {
    TCPIP_PRECONDITION_ASSERT(targetId != TCPIP_LOCALADDRID_INVALID,TCPIP_INTERNAL_API_ID);
    dadPtr = &TcpIp_Ndp_Dad_Cache[dadIndex];
    switch(dadPtr->state)
    {
      case TCPIP_NDP_DAD_STATE_START_DELAY:
        TcpIp_Ndp_Dad_tr_checkDupplicate_START_DELAY_INACTIVE(targetId, dadPtr);
        break;
      case TCPIP_NDP_DAD_STATE_PROBING:
        TcpIp_Ndp_Dad_tr_checkDuplicate_PROBING_INACTIVE(targetId, dadPtr);
        break;
      /* CHECK: NOPARSE */
      case TCPIP_NDP_DAD_STATE_INACTIVE: /* intended fall through */
      default:
        /* should not happen */
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break;
     /* CHECK: PARSE */
    }
  }
  else
  {
    result = TCPIP_OK;
  }
  return result;
}

/*==================[internal function definitions]=========================*/

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Dad_timeout
(
  uint8 localAddrId,
  P2VAR(TcpIp_Ndp_Dad_DataType,AUTOMATIC,TCPIP_VAR_CLEARED) dadPtr
)
{
  switch(dadPtr->state)
  {
    case TCPIP_NDP_DAD_STATE_START_DELAY:
      TcpIp_Ndp_Dad_tr_dadTimeout_START_DELAY_PROBING(localAddrId, dadPtr);
      break;
    case TCPIP_NDP_DAD_STATE_PROBING:
      if(dadPtr->rtrCount < TCPIP_NDP_DAD_CONFIG_NUM_OF_SOL)
      {
        TcpIp_Ndp_Dad_tr_timeout_self_PROBING(localAddrId, dadPtr);
      }
      else
      {
        TcpIp_Ndp_Dad_tr_timeout_PROBING_INACTIVE(localAddrId, dadPtr);
      }
      break;
    /* CHECK: NOPARSE */
    case TCPIP_NDP_DAD_STATE_INACTIVE: /* intended fall through */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Dad_tr_timeout_self_PROBING
(
  uint8 localAddrId,
  P2VAR(TcpIp_Ndp_Dad_DataType,AUTOMATIC,TCPIP_VAR_CLEARED) dadPtr
)
{
  TcpIp_Ndp_Dad_txNeighSol(localAddrId, dadPtr->ipProbe);

  dadPtr->rtrCount++;
  dadPtr->timer = TCPIP_NDP_DAD_CONFIG_TIMEOUT;
}

/* entry functions */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Dad_entry_INACTIVE
(
  P2VAR(TcpIp_Ndp_Dad_DataType,AUTOMATIC,TCPIP_VAR_CLEARED) dadPtr
)
{
  dadPtr->rtrCount = 0U;
  dadPtr->timer = 0U;
  TCPIP_IPV6_SET_UNSPECIFIED_IP(dadPtr->ipProbe);
  dadPtr->state = TCPIP_NDP_DAD_STATE_INACTIVE;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Dad_tr_checkDupplicate_START_DELAY_INACTIVE
(
  uint8 localAddrId,
  P2VAR(TcpIp_Ndp_Dad_DataType,AUTOMATIC,TCPIP_VAR_CLEARED) dadPtr
)
{
  (*dadPtr->dadResultFctPtr)(localAddrId, FALSE);
  TcpIp_Ndp_Dad_entry_INACTIVE(dadPtr);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Dad_tr_timeout_PROBING_INACTIVE
(
  uint8 localAddrId,
  P2VAR(TcpIp_Ndp_Dad_DataType,AUTOMATIC,TCPIP_VAR_CLEARED) dadPtr
)
{
  dadPtr->dadResultFctPtr( (uint8) localAddrId, TRUE);
  TcpIp_Ndp_Dad_entry_INACTIVE(dadPtr);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Dad_entry_PROBING
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(TcpIp_Ndp_Dad_DataType,AUTOMATIC,TCPIP_VAR_CLEARED) dadPtr
)
{
  TcpIp_Ndp_Dad_txNeighSol(localAddrId, dadPtr->ipProbe);

  dadPtr->rtrCount = 1U;
  dadPtr->state = TCPIP_NDP_DAD_STATE_PROBING;
  dadPtr->timer= TCPIP_NDP_DAD_CONFIG_TIMEOUT;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Dad_entry_START_DELAY
(
  uint8 localAddrId,
  P2VAR(TcpIp_Ndp_Dad_DataType,AUTOMATIC,TCPIP_VAR_CLEARED) dadPtr,
  P2CONST(uint32,AUTOMATIC,TCPIP_VAR_CLEARED) ipAddress,
  TcpIp_Ndp_dadResult_FctPtrType ndpDadResultFctPtr
)
{
  dadPtr->dadResultFctPtr = ndpDadResultFctPtr;
  TCPIP_IPV6_ADDR_CPY(dadPtr->ipProbe, ipAddress);
  dadPtr->state = TCPIP_NDP_DAD_STATE_START_DELAY;
  dadPtr->rtrCount = 0U;
  /* if result is 0 TcpIp won't assign the solicit multicast address before
     transmitting neighbor solicitation */
#if(TCPIP_NDP_DAD_CONFIG_DELAY_ENABLED == STD_ON && TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_OFF)
    dadPtr->timer = (uint8)(TcpIp_GetRandomRange(TCPIP_NDP_DAD_CONFIG_TIMEOUT));
    /* if timer is 0, transition to PROBING immediately */
    if(dadPtr->timer == 0U)
    {
      TcpIp_Ndp_Dad_timeout(localAddrId, dadPtr);
    }
#else
    TS_PARAM_UNUSED(localAddrId);
    dadPtr->timer = 1U;
#endif /* TCPIP_NDP_DAD_CONFIG_DELAY_ENABLED == STD_ON && TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_OFF */
}

STATIC FUNC(void, TCPIP_CODE)TcpIp_Ndp_Dad_txNeighSol
(
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(uint32,AUTOMATIC,TCPIP_VAR_CLEARED) ipProbe
)
{
  TcpIp_SockAddrInet6Type remoteAddr;
  remoteAddr.domain = TCPIP_AF_INET6;

  TcpIp_IpV6_getSolicitMulti(remoteAddr.addr, ipProbe);

  (void) TcpIp_Ndp_NCache_transmitNeighborSolicit
  (
    localAddrId,
    &remoteAddr,
    (P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA))ipProbe,
    TCPIP_IPV6_NDP_REMOTE_HANDLE_MULTICAST,
    TRUE
  );
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ndp_Dad_tr_checkDuplicate_PROBING_INACTIVE
(
  uint8 localAddrId,
  P2VAR(TcpIp_Ndp_Dad_DataType,AUTOMATIC,TCPIP_VAR_CLEARED) dadPtr
)
{
  (*dadPtr->dadResultFctPtr)(localAddrId, FALSE);
  TcpIp_Ndp_Dad_entry_INACTIVE(dadPtr);
}

#define TCPIP_STOP_SEC_CODE
#include <MemMap.h>

#endif /* (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON) */

/*==================[end of file]===========================================*/
