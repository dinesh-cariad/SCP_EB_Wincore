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

#include <TcpIp_IpV4_Priv.h>    /* own SW-unit private header file */
#include <EthIf.h>              /* EthIf API declarations */
#if (TCPIP_NVM_STORING_ENABLED == STD_ON)
#include <TcpIp_Memory_Int.h>   /* used SW-unit interface file */
#endif /* (TCPIP_NVM_STORING_ENABLED == STD_ON) */

/*==================[macros]================================================*/

/** \brief use default auto ip timings */
#define TCPIP_IPV4_LINKLOCAL_DEFAULT_TIMINGS                     0U
/** \brief use special doip timings */
#define TCPIP_IPV4_LINKLOCAL_DOIP_TIMINGS                        1U
/** \brief number of different timing methods */
#define TCPIP_IPV4_LINKLOCAL_NUM_METHODS                         2U

/** \brief maximum probe wait delay */
#define TCPIP_IPV4_LINKLOCAL_DEFAULT_PROBE_WAIT                  1U
/** \brief number of probe messages */
#define TCPIP_IPV4_LINKLOCAL_DEFAULT_PROBE_NUM                   3U
/** \brief random delay between 1 and 2 seconds until probe is repeated */
#define TCPIP_IPV4_LINKLOCAL_DEFAULT_PROBE_RANDOM              TRUE
/** \brief maximum delay till repeated probe*/
#define TCPIP_IPV4_LINKLOCAL_DEFAULT_PROBE_MAX                   2U
/** \brief delay before announcing (must not be zero) */
#define TCPIP_IPV4_LINKLOCAL_DEFAULT_ANNOUNCE_WAIT               2U
/** \brief number of announcement packets */
#define TCPIP_IPV4_LINKLOCAL_DEFAULT_ANNOUNCE_NUM                2U
/** \brief time between announcement packets (must not be zero) */
#define TCPIP_IPV4_LINKLOCAL_DEFAULT_ANNOUNCE_INTERVAL           2U

/** \brief number of probe messages */
#define TCPIP_IPV4_LINKLOCAL_DOIP_PROBE_NUM                      1U
/** \brief delay of 1 seconds until probe is repeated */
#define TCPIP_IPV4_LINKLOCAL_DOIP_PROBE_RANDOM                FALSE
/** \brief delay before announcing (must not be zero) */
#define TCPIP_IPV4_LINKLOCAL_DOIP_ANNOUNCE_WAIT                  1U
/** \brief number of announcement packets */
#define TCPIP_IPV4_LINKLOCAL_DOIP_ANNOUNCE_NUM                   1U
/** \brief time between announcement packets */
#define TCPIP_IPV4_LINKLOCAL_DOIP_ANNOUNCE_INTERVAL              1U

/** \brief max conflicts before rate limiting*/
#define TCPIP_IPV4_LINKLOCAL_MAX_CONFLICTS                      10U
/** \brief delay between successive attempts */
#define TCPIP_IPV4_LINKLOCAL_RATE_LIMIT_INTERVAL                61U

/** \brief system was just booted */
#define TCPIP_IPV4_LINKLOCAL_STATE_INACTIVE                      0U
/** \brief waiting some time before start link local assignment */
#define TCPIP_IPV4_LINKLOCAL_STATE_START_DELAY                   1U
/** \brief probing the chosen IP address */
#define TCPIP_IPV4_LINKLOCAL_STATE_PROBING                       2U
/** \brief we have configured a link-local address */
#define TCPIP_IPV4_LINKLOCAL_STATE_CONFIGURED                    3U

/** \brief Factor to calculate 1s to 100ms steps */
#define TCPIP_IPV4_LINKLOCAL_100MS_TO_1S_FACTOR                 10U

/*==================[type definitions]======================================*/

#if (TCPIP_IPV4_LINK_LOCAL_ENABLED == STD_ON)

/** \brief This structure defines the runtime data of a link local assignment. */
typedef struct
{
  uint32 ip_addr;
  /**< link local ip address */
  uint16 timer;
  /**< transmission timeout */
  uint8 conflicts;
  /**< counts number of conflicting ARPs */
  uint8 state;
  /**< assignment state */
  uint8 method;
  /**< timings used for probing, announcements  */
}TcpIp_IpV4_LinkLocal_DataType;

/** \brief */
typedef struct
{
  uint8 probe_num;
  /**< Number of probe messages. */
  boolean probe_random;
  /**< FALSE: delay of 1 second, TRUE: random delay between 1 and 2 seconds */
  uint8 announce_wait;
  /**< Delay before announcing locally configured IP address. */
  uint8 announce_num;
  /**< Number of announcement messages. */
  uint8 announce_interval;
  /**< Time between announcement messages. */

}TcpIp_IpV4_LinkLocal_TimingType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief Initalize Link local (generic AutoIp, DoIp) address assignment
 *
 * This function initialize the link local data structures.
 *
 * \param[in] localAddrId  Id of local IP address
 * \param[in] method     use TCPIP_IPADDRM_ASSIGN_LINKLOCALV4 or
 *                           TCPIP_IPADDRM_ASSIGN_LINKLOCAL_DOIP parameter set
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_init
(
    TcpIp_LocalAddrIdType localAddrId,
    uint8 method
);

/** \brief Request Link local (generic AutoIp, DoIp) address assignment.
 *
 * This function decides what to do if the IP address manager wants to assign the
 * link local address in a specific state of the link local state machine. E.g.
 * generate IP address and start probing
 *
 * \param[in] localAddrId  Id of local IP address
 * \param[in] method     use TCPIP_IPADDRM_ASSIGN_LINKLOCALV4 or
 *                           TCPIP_IPADDRM_ASSIGN_LINKLOCAL_DOIP parameter set
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_startAssign
(
    TcpIp_LocalAddrIdType localAddrId,
    uint8 method
);

/** \brief Release Link local (generic AutoIp, DoIp) address assignment.
 *
 * This function decides what to do if the IP address manager wants to unassign the
 * link local address in a specific state of the link local state machine. E.g.
 * stop probing, announcing, defending.
 *
 * \param[in] localAddrId  Id of local IP address
 * \param[in] method     use TCPIP_IPADDRM_ASSIGN_LINKLOCALV4 or
 *                           TCPIP_IPADDRM_ASSIGN_LINKLOCAL_DOIP parameter set
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_stopAssign
(
    TcpIp_LocalAddrIdType localAddrId,
    uint8 method
);

/** \brief trigger announcement
 *
 * This API function triggers the ARP to announce the Link local (generic AutoIp, DoIp) ip address
 *
 * \param[in] localAddrId     Id of local IP address
 * \param[in] state           TRUE: assignment is active, FALSE: assignment is inactive
 * \param[in] method     use TCPIP_IPADDRM_ASSIGN_LINKLOCALV4 or
 *                           TCPIP_IPADDRM_ASSIGN_LINKLOCAL_DOIP parameter set
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_assignmentActive
(
    TcpIp_LocalAddrIdType localAddrId,
    boolean state,
    uint8 method
);

/** \brief get assignment status of the Link local (generic AutoIp, DoIp) assignment
 *
 * This API function returns the assignment status
 *
 * \param[in] localAddrId  Id of local IP address
 * \param[in] method     use TCPIP_IPADDRM_ASSIGN_LINKLOCALV4 or
 *                           TCPIP_IPADDRM_ASSIGN_LINKLOCAL_DOIP parameter set
 */
STATIC FUNC(boolean, TCPIP_CODE) TcpIp_IpV4_LinkLocal_isAssigned
(
    TcpIp_LocalAddrIdType localAddrId,
    uint8 method
);

/** \brief get Ip configuration of the Link local (generic AutoIp, DoIp) assignment
 *
 * \param[in] localAddrId         Id of local IP address
 * \param[out] assignedIpPtrPtr   Address the assigned Ip address shall be stored to
 * \param[out] gatewayIpPtrPtr    Address the gateway Ip address shall be stored to
 * \param[out] netmaskIpPtrPtr    Address the netmask shall be stored to
 * \param[in] method     use TCPIP_IPADDRM_ASSIGN_LINKLOCALV4 or
 *                           TCPIP_IPADDRM_ASSIGN_LINKLOCAL_DOIP parameter set
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_getIpConfig
(
    TcpIp_LocalAddrIdType localAddrId,
    P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) assignedIpPtrPtr,
    P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) gatewayIpPtrPtr,
    P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) netmaskIpPtrPtr,
    uint8 method
);

/** \brief get Ip address of the Link local (generic AutoIp, DoIp) assignment
 *
 * \param[in] localAddrId         Id of local IP address
 * \param[out] assignedIpPtrPtr   Address the assigned Ip address shall be stored to
 * \param[in] method     use TCPIP_IPADDRM_ASSIGN_LINKLOCALV4 or
 *                           TCPIP_IPADDRM_ASSIGN_LINKLOCAL_DOIP parameter set
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_getIpAddress
(
    TcpIp_LocalAddrIdType localAddrId,
    P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) assignedIpPtrPtr,
    uint8 method
);

/** \brief Handle probe results for AutoIp
 *
 * This function handles the result of ARP probing. In case the IP address is unique
 * it announces the IP otherwise it generates a new IP and retries probing.
 *
 * \param[in] ctrlIdx         controller on which probing was processed
 * \param[in] unique          TRUE: IP address unique in network
 *                            FALSE: IP address used by other node
 * \param[in] remotePhysAddr  physical address of the remote host
 * \param[in] localPhysAddr   physical address of the local host
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_probeResult_AutoIp
(
  uint8 ctrlIdx,
  boolean unique,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) remotePhysAddr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) localPhysAddr
);

/** \brief Handle probe results for DoIp
 *
 * This function handles the result of ARP probing. In case the IP address is unique
 * it announces the IP otherwise it generates a new IP and retries probing.
 *
 * \param[in] ctrlIdx         controller on which probing was processed
 * \param[in] unique          TRUE: IP address unique in network
 *                            FALSE: IP address used by other node
 * \param[in] remotePhysAddr  physical address of the remote host
 * \param[in] localPhysAddr   physical address of the local host
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_probeResult_AutoIp_DoIp
(
  uint8 ctrlIdx,
  boolean unique,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) remotePhysAddr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) localPhysAddr
);

/** \brief Handle probe results for Link local (generic AutoIp, DoIp)
 *
 * This function handles the result of ARP probing. In case the IP address is unique
 * it announces the IP otherwise it generates a new IP and retries probing.
 *
 * \param[in] ctrlIdx controller on which probing was processed
 * \param[in] unique  TRUE: IP address unique in network
 *                    FALSE: IP address used by other node
 * \param[in] method     use TCPIP_IPADDRM_ASSIGN_LINKLOCALV4 or
 *                           TCPIP_IPADDRM_ASSIGN_LINKLOCAL_DOIP parameter set
 * \param[in] remotePhysAddr  physical address of the remote host
 * \param[in] localPhysAddr   physical address of the local host
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_probeResult
(
  uint8 ctrlIdx,
  boolean unique,
  uint8 method,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) remotePhysAddr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) localPhysAddr
);


/** \brief Report address conflicts for AutoIp
 *
 * This function is called if 2 address conflicts are detected within the
 * defend interval. It releases the link local ip address and triggers the
 * generation of a new one.
 *
 * \param[in] localAddr  Local address id of link local assignment
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_conflict_AutoIp
(
  TcpIp_LocalAddrIdType localAddr
);

/** \brief Report address conflicts for DoIp
 *
 * This function is called if 2 address conflicts are detected within the
 * defend interval. It releases the link local ip address and triggers the
 * generation of a new one.
 *
 * \param[in] localAddr  Local address id of link local assignment

 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_conflict_AutoIp_DoIp
(
  TcpIp_LocalAddrIdType localAddr
);

/** \brief Report address conflicts.
 *
 * This function is called if 2 address conflicts are detected within the
 * defend interval. It releases the link local ip address and triggers the
 * generation of a new one.
 *
 * \param[in] localAddr  Local address id of link local assignment

 * \param[in] method     use TCPIP_IPADDRM_ASSIGN_LINKLOCALV4 or
 *                           TCPIP_IPADDRM_ASSIGN_LINKLOCAL_DOIP parameter set
 *
 * \retval TRUE: Send announcement to defend IP, FALSE: do nothing
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_conflict
(
  TcpIp_LocalAddrIdType localAddr,
  uint8 method
);


/** \brief Generate link local address
 *
 * This function generates a random link local IP address.
 *
 * \param[in] linkLocalPtr  Pointer to link local data structure
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_generateIp
(
    P2VAR(TcpIp_IpV4_LinkLocal_DataType, AUTOMATIC, TCPIP_APPL_DATA) linkLocalPtr
);

/** \brief starts the timer for probing delay
 *
 * This function generates a random link local probing delay.
 *
 * \param[in] linkLocalPtr  Pointer to link local data structure
 *
 * \return probe_delay time
 */
STATIC FUNC(uint16, TCPIP_CODE) TcpIp_IpV4_LinkLocal_startProbeTimer
(
    P2VAR(TcpIp_IpV4_LinkLocal_DataType, AUTOMATIC, TCPIP_APPL_DATA) linkLocalPtr
);

/** \brief Handle Timeouts
 *
 * This function decides what to do in case a timeout occurs in a specific state
 * in the link local address assignment state machine. e.g. start probing.
 *
 * \param[in] localAddrId     Local address id of link local assignment
 * \param[in] linkLocalIdx  Index of assignment
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_timeout
(
    TcpIp_LocalAddrIdType localAddrId,
    uint8 linkLocalIdx
);

/* Transition functions */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_tr_INACTIVE_START_DELAY
(
    uint8 linkLocalIdx
);

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_tr_START_DELAY_PROBING
(
    TcpIp_LocalAddrIdType localAddrId,
    uint8 linkLocalIdx
);

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_probeResult_tr_PROBING_PROBING
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 linkLocalIdx,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) remotePhysAddr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) localPhysAddr
);

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_tr_PROBING_CONFIGURED
(
    TcpIp_LocalAddrIdType localAddrId,
    uint8 linkLocalIdx
);

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_tr_CONFIGURED_INACTIVE
(
    TcpIp_LocalAddrIdType localAddrId,
    uint8 linkLocalIdx
);

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_tr_START_DELAY_INACTIVE
(
  uint8 linkLocalIdx
);

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_tr_PROBING_INACTIVE
(
    TcpIp_LocalAddrIdType localAddrId,
    uint8 linkLocalIdx
);


STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_tr_CONFIGURED_PROBING
(
    TcpIp_LocalAddrIdType localAddrId,
    uint8 linkLocalIdx
);

/* entry functions */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_entry_INACTIVE
(
    uint8 linkLocalIdx
);

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_entry_START_DELAY
(
    uint8 linkLocalIdx
);

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_entry_PROBING
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 linkLocalIdx,
  boolean nvmGet
);

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_entry_CONFIGURED
(
    TcpIp_LocalAddrIdType localAddrId,
    uint8 linkLocalIdx
);

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_exit_CONFIGURED
(
    TcpIp_LocalAddrIdType localAddrId,
    uint8 linkLocalIdx
);

/* exit functions */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_exit_PROBING
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 linkLocalIdx
);

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[internal constants]====================================*/

#define TCPIP_START_SEC_CONST_32
#include <TcpIp_MemMap.h>

/** \brief netmask of a link local ip address */
STATIC CONST(uint32, TCPIP_CONST)
TcpIp_IpV4_LinkLocal_netmask = TCPIP_IPV4_LINK_LOCAL_NETMASK;

/** \brief IP address has not been assigned yet */
STATIC CONST(uint32, TCPIP_CONST) TcpIp_IpV4_LinkLocal_unspecified = 0x0U;

#define TCPIP_STOP_SEC_CONST_32
#include <TcpIp_MemMap.h>

#define TCPIP_START_SEC_CONST_UNSPECIFIED
#include <TcpIp_MemMap.h>

/** \brief Default and doip timing */
STATIC CONST(TcpIp_IpV4_LinkLocal_TimingType, TCPIP_CONST)
TcpIp_IpV4_LinkLocal_Timings[TCPIP_IPV4_LINKLOCAL_NUM_METHODS] =
{
  {
    TCPIP_IPV4_LINKLOCAL_DEFAULT_PROBE_NUM,
    TCPIP_IPV4_LINKLOCAL_DEFAULT_PROBE_RANDOM,
    TCPIP_IPV4_LINKLOCAL_DEFAULT_ANNOUNCE_WAIT,
    TCPIP_IPV4_LINKLOCAL_DEFAULT_ANNOUNCE_NUM,
    TCPIP_IPV4_LINKLOCAL_DEFAULT_ANNOUNCE_INTERVAL
  },
  {
    TCPIP_IPV4_LINKLOCAL_DOIP_PROBE_NUM,
    TCPIP_IPV4_LINKLOCAL_DOIP_PROBE_RANDOM,
    TCPIP_IPV4_LINKLOCAL_DOIP_ANNOUNCE_WAIT,
    TCPIP_IPV4_LINKLOCAL_DOIP_ANNOUNCE_NUM,
    TCPIP_IPV4_LINKLOCAL_DOIP_ANNOUNCE_INTERVAL
  }
};

#define TCPIP_STOP_SEC_CONST_UNSPECIFIED
#include <TcpIp_MemMap.h>

/*==================[internal data]=========================================*/

#define TCPIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

/** \brief state information variable definition */
STATIC VAR(TcpIp_IpV4_LinkLocal_DataType, TCPIP_VAR_CLEARED)
TcpIp_IpV4_LinkLocal_Data[TCPIP_NUM_IPV4_LINK_LOCAL_ASSIGN];

#define TCPIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_AutoIp_init
(
    TcpIp_LocalAddrIdType localAddrId
)
{
  DBG_TCPIP_IPV4_AUTOIP_INIT_ENTRY(localAddrId);

  TcpIp_IpV4_LinkLocal_init(localAddrId, TCPIP_IPADDRM_ASSIGN_LINKLOCALV4);

  DBG_TCPIP_IPV4_AUTOIP_INIT_EXIT(localAddrId);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_AutoIp_DoIp_init
(
    TcpIp_LocalAddrIdType localAddrId
)
{
  DBG_TCPIP_IPV4_AUTOIP_DOIP_INIT_ENTRY(localAddrId);

  TcpIp_IpV4_LinkLocal_init(localAddrId, TCPIP_IPADDRM_ASSIGN_LINKLOCAL_DOIP);

  DBG_TCPIP_IPV4_AUTOIP_DOIP_INIT_EXIT(localAddrId);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_AutoIp_startAssign
(
    TcpIp_LocalAddrIdType localAddrId
)
{
  DBG_TCPIP_IPV4_AUTOIP_STARTASSIGN_ENTRY(localAddrId);

  TcpIp_IpV4_LinkLocal_startAssign(localAddrId, TCPIP_IPADDRM_ASSIGN_LINKLOCALV4);

  DBG_TCPIP_IPV4_AUTOIP_STARTASSIGN_EXIT(localAddrId);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_AutoIp_DoIp_startAssign
(
    TcpIp_LocalAddrIdType localAddrId
)
{
  DBG_TCPIP_IPV4_AUTOIP_DOIP_STARTASSIGN_ENTRY(localAddrId);

  TcpIp_IpV4_LinkLocal_startAssign(localAddrId, TCPIP_IPADDRM_ASSIGN_LINKLOCAL_DOIP);

  DBG_TCPIP_IPV4_AUTOIP_DOIP_STARTASSIGN_EXIT(localAddrId);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_AutoIp_stopAssign
(
    TcpIp_LocalAddrIdType localAddrId
)
{
  DBG_TCPIP_IPV4_AUTOIP_STOPASSIGN_ENTRY(localAddrId);

  TcpIp_IpV4_LinkLocal_stopAssign(localAddrId, TCPIP_IPADDRM_ASSIGN_LINKLOCALV4);

  DBG_TCPIP_IPV4_AUTOIP_STOPASSIGN_EXIT(localAddrId);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_AutoIp_DoIp_stopAssign
(
    TcpIp_LocalAddrIdType localAddrId
)
{
  DBG_TCPIP_IPV4_AUTOIP_DOIP_STOPASSIGN_ENTRY(localAddrId);

  TcpIp_IpV4_LinkLocal_stopAssign(localAddrId, TCPIP_IPADDRM_ASSIGN_LINKLOCAL_DOIP);

  DBG_TCPIP_IPV4_AUTOIP_DOIP_STOPASSIGN_EXIT(localAddrId);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_AutoIp_assignmentActive
(
    TcpIp_LocalAddrIdType localAddrId,
    boolean state
)
{
  DBG_TCPIP_IPV4_AUTOIP_ASSIGNMENTACTIVE_ENTRY(localAddrId, state);

  TcpIp_IpV4_LinkLocal_assignmentActive(localAddrId,state,TCPIP_IPADDRM_ASSIGN_LINKLOCALV4);

  DBG_TCPIP_IPV4_AUTOIP_ASSIGNMENTACTIVE_EXIT(localAddrId, state);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_AutoIp_DoIp_assignmentActive
(
    TcpIp_LocalAddrIdType localAddrId,
    boolean state
)
{
  DBG_TCPIP_IPV4_AUTOIP_DOIP_ASSIGNMENTACTIVE_ENTRY(localAddrId, state);

  TcpIp_IpV4_LinkLocal_assignmentActive(localAddrId,state,TCPIP_IPADDRM_ASSIGN_LINKLOCAL_DOIP);

  DBG_TCPIP_IPV4_AUTOIP_DOIP_ASSIGNMENTACTIVE_EXIT(localAddrId, state);
}

TS_MOD_PRIV_DEFN FUNC(boolean, TCPIP_CODE) TcpIp_IpV4_AutoIp_isAssigned
(
    TcpIp_LocalAddrIdType localAddrId
)
{
  boolean result;

  DBG_TCPIP_IPV4_AUTOIP_ISASSIGNED_ENTRY(localAddrId);

  result = TcpIp_IpV4_LinkLocal_isAssigned(localAddrId, TCPIP_IPADDRM_ASSIGN_LINKLOCALV4);

  DBG_TCPIP_IPV4_AUTOIP_ISASSIGNED_EXIT(result,localAddrId);

  return result;
}

TS_MOD_PRIV_DEFN FUNC(boolean, TCPIP_CODE) TcpIp_IpV4_AutoIp_DoIp_isAssigned
(
    TcpIp_LocalAddrIdType localAddrId
)
{
  boolean result;

  DBG_TCPIP_IPV4_AUTOIP_DOIP_ISASSIGNED_ENTRY(localAddrId);

  result = TcpIp_IpV4_LinkLocal_isAssigned(localAddrId, TCPIP_IPADDRM_ASSIGN_LINKLOCAL_DOIP);

  DBG_TCPIP_IPV4_AUTOIP_DOIP_ISASSIGNED_EXIT(result,localAddrId);

  return result;
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_AutoIp_getIpConfig
(
    TcpIp_LocalAddrIdType localAddrId,
    P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) assignedIpPtrPtr,
    P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) gatewayIpPtrPtr,
    P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) netmaskIpPtrPtr
)
{
  DBG_TCPIP_IPV4_AUTOIP_GETIPCONFIG_ENTRY(localAddrId,assignedIpPtrPtr,gatewayIpPtrPtr,netmaskIpPtrPtr);

  TcpIp_IpV4_LinkLocal_getIpConfig(localAddrId,assignedIpPtrPtr,gatewayIpPtrPtr,netmaskIpPtrPtr,TCPIP_IPADDRM_ASSIGN_LINKLOCALV4);

  DBG_TCPIP_IPV4_AUTOIP_GETIPCONFIG_EXIT(localAddrId,assignedIpPtrPtr,gatewayIpPtrPtr,netmaskIpPtrPtr);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_AutoIp_DoIp_getIpConfig
(
    TcpIp_LocalAddrIdType localAddrId,
    P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) assignedIpPtrPtr,
    P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) gatewayIpPtrPtr,
    P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) netmaskIpPtrPtr
)
{
  DBG_TCPIP_IPV4_AUTOIP_DOIP_GETIPCONFIG_ENTRY(localAddrId,assignedIpPtrPtr,gatewayIpPtrPtr,netmaskIpPtrPtr);

  TcpIp_IpV4_LinkLocal_getIpConfig(localAddrId,assignedIpPtrPtr,gatewayIpPtrPtr,netmaskIpPtrPtr,TCPIP_IPADDRM_ASSIGN_LINKLOCAL_DOIP);

  DBG_TCPIP_IPV4_AUTOIP_DOIP_GETIPCONFIG_EXIT(localAddrId,assignedIpPtrPtr,gatewayIpPtrPtr,netmaskIpPtrPtr);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_AutoIp_getIpAddress
(
    TcpIp_LocalAddrIdType localAddrId,
    P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) assignedIpPtrPtr
)
{
  DBG_TCPIP_IPV4_AUTOIP_GETIPADDRESS_ENTRY(localAddrId,assignedIpPtrPtr);

  TcpIp_IpV4_LinkLocal_getIpAddress(localAddrId,assignedIpPtrPtr,TCPIP_IPADDRM_ASSIGN_LINKLOCALV4);

  DBG_TCPIP_IPV4_AUTOIP_GETIPADDRESS_EXIT(localAddrId,assignedIpPtrPtr);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_AutoIp_DoIp_getIpAddress
(
    TcpIp_LocalAddrIdType localAddrId,
    P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) assignedIpPtrPtr
)
{
  DBG_TCPIP_IPV4_AUTOIP_DOIP_GETIPADDRESS_ENTRY(localAddrId,assignedIpPtrPtr);

  TcpIp_IpV4_LinkLocal_getIpAddress(localAddrId,assignedIpPtrPtr,TCPIP_IPADDRM_ASSIGN_LINKLOCAL_DOIP);

  DBG_TCPIP_IPV4_AUTOIP_DOIP_GETIPADDRESS_EXIT(localAddrId,assignedIpPtrPtr);
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpV4_AutoIp_requestIpAddress
(
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) LocalIpAddrPtr
)
{
  TS_PARAM_UNUSED(localAddrId);
  TS_PARAM_UNUSED(LocalIpAddrPtr);

  return E_OK;
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpV4_AutoIp_DoIp_requestIpAddress
(
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) LocalIpAddrPtr
)
{
  TS_PARAM_UNUSED(localAddrId);
  TS_PARAM_UNUSED(LocalIpAddrPtr);

  return E_OK;
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_mainFunction
(
    uint16 timerTick
)
{
  uint8_least i;
  TCPIP_PRECONDITION_ASSERT(TcpIp_PBcfgPtr->IpV4LinkLocalNum <= TCPIP_NUM_IPV4_LINK_LOCAL_ASSIGN, TCPIP_INTERNAL_API_ID);

  DBG_TCPIP_IPV4_LINKLOCAL_MAINFUNCTION_ENTRY(timerTick);

  for (i = 0U; i < TcpIp_PBcfgPtr->IpV4LinkLocalNum; ++i)
  {
    if((timerTick % TCPIP_MAINFUNCTION_PRESCALER_100MS) == TCPIP_TIMER_OFFSET_100MS_AUTOIP_TIMEOUT)
    {
      const TcpIp_LocalAddrIdType localAddrId =
      TcpIp_IpAddrM_getLocalAddrId(i , TCPIP_IPADDRM_ASSIGN_LINKLOCALV4);

      P2VAR(TcpIp_IpV4_LinkLocal_DataType, AUTOMATIC, TCPIP_APPL_DATA) linkLocalPtr =
      &TcpIp_IpV4_LinkLocal_Data[i];

      if(linkLocalPtr->timer > 0U)
      {
        linkLocalPtr->timer--;

        if(linkLocalPtr->timer == 0U)
        {
          TcpIp_IpV4_LinkLocal_timeout(localAddrId, (uint8)i);
        }
      }
    }
  }

  DBG_TCPIP_IPV4_LINKLOCAL_MAINFUNCTION_EXIT(timerTick);
}

/*==================[internal function definitions]=========================*/

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_getIpAddress
(
    TcpIp_LocalAddrIdType localAddrId,
    P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) assignedIpPtrPtr,
    uint8 method
)
{
  const uint8 linkLocalIdx = TcpIp_IpAddrM_getAssignmentIdx(localAddrId, method);
  uint8 state;

  TCPIP_PRECONDITION_ASSERT(linkLocalIdx < TCPIP_NUM_IPV4_LINK_LOCAL_ASSIGN, TCPIP_INTERNAL_API_ID);
  /* !LINKSTO TcpIp.Design.IpV4.LinkLocal.Atomic.State,1 */
  TS_AtomicAssign8(state, TcpIp_IpV4_LinkLocal_Data[linkLocalIdx].state);

  switch(state)
  {
    case TCPIP_IPV4_LINKLOCAL_STATE_INACTIVE: /* intended fall through */
    case TCPIP_IPV4_LINKLOCAL_STATE_START_DELAY: /* intended fall through */
    case TCPIP_IPV4_LINKLOCAL_STATE_PROBING: /* intended fall through */
      /* link local address not assigned return 0 */
      *assignedIpPtrPtr = &TcpIp_IpV4_LinkLocal_unspecified;
    break;
    case TCPIP_IPV4_LINKLOCAL_STATE_CONFIGURED:
      *assignedIpPtrPtr = &TcpIp_IpV4_LinkLocal_Data[linkLocalIdx].ip_addr;
    break;
    /* CHECK: NOPARSE */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
    break;
    /* CHECK: PARSE */
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_getIpConfig
(
    TcpIp_LocalAddrIdType localAddrId,
    P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) assignedIpPtrPtr,
    P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) gatewayIpPtrPtr,
    P2VAR(TcpIp_IpAddrConstPtr, AUTOMATIC, TCPIP_APPL_DATA) netmaskIpPtrPtr,
    uint8 method
)
{
  const uint8 linkLocalIdx = TcpIp_IpAddrM_getAssignmentIdx(localAddrId, method);
  uint8 state;

  TCPIP_PRECONDITION_ASSERT(linkLocalIdx < TCPIP_NUM_IPV4_LINK_LOCAL_ASSIGN, TCPIP_INTERNAL_API_ID);

  /* !LINKSTO TcpIp.Design.IpV4.LinkLocal.Atomic.State,1 */
  TS_AtomicAssign8(state, TcpIp_IpV4_LinkLocal_Data[linkLocalIdx].state);

  switch(state)
  {
    case TCPIP_IPV4_LINKLOCAL_STATE_INACTIVE: /* intended fall through */
    case TCPIP_IPV4_LINKLOCAL_STATE_START_DELAY: /* intended fall through */
    case TCPIP_IPV4_LINKLOCAL_STATE_PROBING: /* intended fall through */
      /* link local address not assigned return 0 */
      *assignedIpPtrPtr = &TcpIp_IpV4_LinkLocal_unspecified;
      *gatewayIpPtrPtr = &TcpIp_IpV4_LinkLocal_unspecified;
      *netmaskIpPtrPtr = &TcpIp_IpV4_LinkLocal_unspecified;
    break;
    case TCPIP_IPV4_LINKLOCAL_STATE_CONFIGURED: /* intended fall through */
      *assignedIpPtrPtr = &TcpIp_IpV4_LinkLocal_Data[linkLocalIdx].ip_addr;
      /* link local address are not routable */
      *gatewayIpPtrPtr = &TcpIp_IpV4_LinkLocal_unspecified;
      *netmaskIpPtrPtr = &TcpIp_IpV4_LinkLocal_netmask;
    break;
    /* CHECK: NOPARSE */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
    break;
    /* CHECK: PARSE */
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_assignmentActive
(
    TcpIp_LocalAddrIdType localAddrId,
    boolean state,
    uint8 method
)
{
  const uint8 linkLocalIdx =
  TcpIp_IpAddrM_getAssignmentIdx(localAddrId, method);
  uint8 linklocalState;

  TCPIP_PRECONDITION_ASSERT(linkLocalIdx < TCPIP_NUM_IPV4_LINK_LOCAL_ASSIGN, TCPIP_INTERNAL_API_ID);
  {
    P2VAR(TcpIp_IpV4_LinkLocal_DataType, AUTOMATIC, TCPIP_APPL_DATA) linkLocalPtr =
      &TcpIp_IpV4_LinkLocal_Data[linkLocalIdx];

    TCPIP_PRECONDITION_ASSERT(linkLocalPtr->method == method, TCPIP_INTERNAL_API_ID);

    /* !LINKSTO TcpIp.Design.IpV4.LinkLocal.Atomic.State,1 */
    TS_AtomicAssign8(linklocalState, linkLocalPtr->state);

    switch (linklocalState)
    {
      case TCPIP_IPV4_LINKLOCAL_STATE_CONFIGURED:
      case TCPIP_IPV4_LINKLOCAL_STATE_PROBING:
      {
        const uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
        TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

        if(state)
        {
          const TcpIp_IpV4_AddrConflDetect_FctPtrType conflictFctPtr =
            (linkLocalPtr->method == TCPIP_IPADDRM_ASSIGN_LINKLOCALV4 ?
              &TcpIp_IpV4_LinkLocal_conflict_AutoIp :
              &TcpIp_IpV4_LinkLocal_conflict_AutoIp_DoIp);
          const uint8 addrDefend = TcpIp_PBcfgPtr->ipV4linkLocalConfig.addrDefendMechanism;
          const uint8 timing =
            (method == TCPIP_IPADDRM_ASSIGN_LINKLOCALV4 ?
              TCPIP_IPV4_LINKLOCAL_DEFAULT_TIMINGS :
              TCPIP_IPV4_LINKLOCAL_DOIP_TIMINGS);

          const uint8 numAnnouncements = TcpIp_IpV4_LinkLocal_Timings[timing].announce_num;
          const uint8 interval = TcpIp_IpV4_LinkLocal_Timings[timing].announce_interval;

          /* announce the IP address */
          TcpIp_IpV4_ArpAnnounce(linkLocalPtr->ip_addr, numAnnouncements, interval, ctrlIdx, linkLocalPtr->method);

          /* listen for ARP conflicts */
          TcpIp_IpV4_AddrConflDetect_activate(localAddrId, conflictFctPtr, addrDefend);
        }
        else
        {
          /* stop announcing, even if it has already finished */
          TcpIp_IpV4_ArpAnnounce(TCPIP_IPV4_ADDR_INVALID_VALUE, 0U, 0U, ctrlIdx, linkLocalPtr->method);

          /* stop listen for ARP conflicts */
          TcpIp_IpV4_AddrConflDetect_activate
            (localAddrId, NULL_PTR, TCPIP_IPV4_ADDR_CONFL_DETECT_MECHANISM_OFF);
        }

        break;
      }
      /* CHECK: NOPARSE */
      case TCPIP_IPV4_LINKLOCAL_STATE_INACTIVE: /* intended fall through */
      case TCPIP_IPV4_LINKLOCAL_STATE_START_DELAY: /* intended fall through */
      default:
        /* should not happen */
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break;
        /* CHECK: PARSE */
    }
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_probeResult_AutoIp
(
  uint8 ctrlIdx,
  boolean unique,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) remotePhysAddr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) localPhysAddr
)
{
  TcpIp_IpV4_LinkLocal_probeResult
    (ctrlIdx,unique,TCPIP_IPADDRM_ASSIGN_LINKLOCALV4, remotePhysAddr, localPhysAddr);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_probeResult_AutoIp_DoIp
(
  uint8 ctrlIdx,
  boolean unique,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) remotePhysAddr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) localPhysAddr
)
{
  TcpIp_IpV4_LinkLocal_probeResult
    (ctrlIdx,unique,TCPIP_IPADDRM_ASSIGN_LINKLOCAL_DOIP, remotePhysAddr, localPhysAddr);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_probeResult
(
  uint8 ctrlIdx,
  boolean unique,
  uint8 method,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) remotePhysAddr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) localPhysAddr
)
{
  const TcpIp_LocalAddrIdType localAddrId =
    (TCPIP_CFG(ctrl,ctrlIdx)).ctrlIpV4.localAddrIdTx;

  const uint8 linkLocalIdx = TcpIp_IpAddrM_getAssignmentIdx(localAddrId, method);

  TCPIP_PRECONDITION_ASSERT(linkLocalIdx < TCPIP_NUM_IPV4_LINK_LOCAL_ASSIGN, TCPIP_INTERNAL_API_ID);
  {
    P2VAR(TcpIp_IpV4_LinkLocal_DataType, AUTOMATIC, TCPIP_APPL_DATA) linkLocalPtr =
        &TcpIp_IpV4_LinkLocal_Data[linkLocalIdx];
    uint8 state;

    /* !LINKSTO TcpIp.Design.IpV4.LinkLocal.Atomic.State,1 */
    TS_AtomicAssign8(state, linkLocalPtr->state);

    switch(state)
    {
      case TCPIP_IPV4_LINKLOCAL_STATE_PROBING:
      {
        if(unique)
        {
          TcpIp_IpV4_LinkLocal_tr_PROBING_CONFIGURED(localAddrId,linkLocalIdx);
        }
        else
        {
          TCPIP_PRECONDITION_ASSERT(localPhysAddr != NULL_PTR, TCPIP_INTERNAL_API_ID);
          TCPIP_PRECONDITION_ASSERT(remotePhysAddr != NULL_PTR, TCPIP_INTERNAL_API_ID);

          TcpIp_IpV4_LinkLocal_probeResult_tr_PROBING_PROBING
            (localAddrId,linkLocalIdx, remotePhysAddr, localPhysAddr);
        }
        break;
      }
      /* CHECK: NOPARSE */
      case TCPIP_IPV4_LINKLOCAL_STATE_INACTIVE: /* intended fall through */
      case TCPIP_IPV4_LINKLOCAL_STATE_START_DELAY: /* intended fall through */
      case TCPIP_IPV4_LINKLOCAL_STATE_CONFIGURED: /* intended fall through */
      default:
        /* should not happen */
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break;
      /* CHECK: PARSE */
    }
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_conflict_AutoIp
(
    TcpIp_LocalAddrIdType localAddr
)
{
  TcpIp_IpV4_LinkLocal_conflict(localAddr,TCPIP_IPADDRM_ASSIGN_LINKLOCALV4);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_conflict_AutoIp_DoIp
(
    TcpIp_LocalAddrIdType localAddr
)
{
  TcpIp_IpV4_LinkLocal_conflict(localAddr,TCPIP_IPADDRM_ASSIGN_LINKLOCAL_DOIP);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_conflict
(
    TcpIp_LocalAddrIdType localAddr,
    uint8 method
)
{
  const uint8 linkLocalIdx = TcpIp_IpAddrM_getAssignmentIdx(localAddr, method);
  TCPIP_PRECONDITION_ASSERT(linkLocalIdx < TCPIP_NUM_IPV4_LINK_LOCAL_ASSIGN, TCPIP_INTERNAL_API_ID);
  {
    P2VAR(TcpIp_IpV4_LinkLocal_DataType, AUTOMATIC, TCPIP_APPL_DATA) linkLocalPtr =
    &TcpIp_IpV4_LinkLocal_Data[linkLocalIdx];
    uint8 state;

    /* !LINKSTO TcpIp.Design.IpV4.LinkLocal.Atomic.State,1 */
    TS_AtomicAssign8(state, linkLocalPtr->state);

    switch(state)
    {
      case TCPIP_IPV4_LINKLOCAL_STATE_CONFIGURED:
      {
        /* abandon the address, to many conflicts detected */
        TcpIp_IpV4_LinkLocal_tr_CONFIGURED_PROBING(localAddr, linkLocalIdx);
        break;
      }
      /* CHECK: NOPARSE */
      case TCPIP_IPV4_LINKLOCAL_STATE_INACTIVE: /* intended fall through */
      case TCPIP_IPV4_LINKLOCAL_STATE_START_DELAY: /* intended fall through */
      case TCPIP_IPV4_LINKLOCAL_STATE_PROBING: /* intended fall through */
      default:
        /* should not happen */
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break;
      /* CHECK: PARSE */
    }
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_init
(
    TcpIp_LocalAddrIdType localAddrId,
    uint8 method
)
{
  const uint8 linkLocalIdx =
  TcpIp_IpAddrM_getAssignmentIdx(localAddrId, method);

  TcpIp_IpV4_LinkLocal_entry_INACTIVE(linkLocalIdx);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_startAssign
(
    TcpIp_LocalAddrIdType localAddrId,
    uint8 method
)
{
  const uint8 linkLocalIdx =
    TcpIp_IpAddrM_getAssignmentIdx(localAddrId, method);
  TCPIP_PRECONDITION_ASSERT(linkLocalIdx < TCPIP_NUM_IPV4_LINK_LOCAL_ASSIGN, TCPIP_INTERNAL_API_ID);
  {
    P2VAR(TcpIp_IpV4_LinkLocal_DataType, AUTOMATIC, TCPIP_APPL_DATA) linkLocalPtr =
      &TcpIp_IpV4_LinkLocal_Data[linkLocalIdx];
    uint8 state;

    /* !LINKSTO TcpIp.Design.IpV4.LinkLocal.Atomic.State,1 */
    TS_AtomicAssign8(state, linkLocalPtr->state);

    /* store timing information that are used for probing and announcing */
    linkLocalPtr->method = method;

    switch (state)
    {
      case TCPIP_IPV4_LINKLOCAL_STATE_INACTIVE:
      {
        TcpIp_IpV4_LinkLocal_tr_INACTIVE_START_DELAY(linkLocalIdx);
        break;
      }
      case TCPIP_IPV4_LINKLOCAL_STATE_START_DELAY: /* intended fall through */
      case TCPIP_IPV4_LINKLOCAL_STATE_PROBING: /* intended fall through */
      case TCPIP_IPV4_LINKLOCAL_STATE_CONFIGURED: /* intended fall through */
        /* ignore */
        break;
        /* CHECK: NOPARSE */
      default:
        /* should not happen */
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break;
      /* CHECK: PARSE */
    }
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_stopAssign
(
    TcpIp_LocalAddrIdType localAddrId,
    uint8 method
)
{
  const uint8 linkLocalIdx =
      TcpIp_IpAddrM_getAssignmentIdx(localAddrId, method);
  TCPIP_PRECONDITION_ASSERT(linkLocalIdx < TCPIP_NUM_IPV4_LINK_LOCAL_ASSIGN, TCPIP_INTERNAL_API_ID);
  {
    P2VAR(TcpIp_IpV4_LinkLocal_DataType, AUTOMATIC, TCPIP_APPL_DATA) linkLocalPtr =
      &TcpIp_IpV4_LinkLocal_Data[linkLocalIdx];
    uint8 state;

    /* !LINKSTO TcpIp.Design.IpV4.LinkLocal.Atomic.State,1 */
    TS_AtomicAssign8(state, linkLocalPtr->state);

    if (linkLocalPtr->method == method)
    {
      switch (state)
      {
        case TCPIP_IPV4_LINKLOCAL_STATE_START_DELAY:
          /* stop timeout */
          TcpIp_IpV4_LinkLocal_tr_START_DELAY_INACTIVE(linkLocalIdx);
          break;
        case TCPIP_IPV4_LINKLOCAL_STATE_PROBING:
          /* stop timeout */
          TcpIp_IpV4_LinkLocal_tr_PROBING_INACTIVE(localAddrId,linkLocalIdx);
          break;
        case TCPIP_IPV4_LINKLOCAL_STATE_CONFIGURED:
          /* stop timeout */
          TcpIp_IpV4_LinkLocal_tr_CONFIGURED_INACTIVE(localAddrId,linkLocalIdx);
          break;
        case TCPIP_IPV4_LINKLOCAL_STATE_INACTIVE:
          /* ignore */
          break;
        /* CHECK: NOPARSE */
        default:
          /* should not happen */
          TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
          break;
        /* CHECK: PARSE */
      }
    }
  }
}

STATIC FUNC(boolean, TCPIP_CODE) TcpIp_IpV4_LinkLocal_isAssigned
(
    TcpIp_LocalAddrIdType localAddrId,
    uint8 method
)
{
  boolean assigned = FALSE;
  const uint8 linkLocalIdx =
    TcpIp_IpAddrM_getAssignmentIdx(localAddrId, method);
  TCPIP_PRECONDITION_ASSERT(linkLocalIdx < TCPIP_NUM_IPV4_LINK_LOCAL_ASSIGN, TCPIP_INTERNAL_API_ID);
  {
    P2VAR(TcpIp_IpV4_LinkLocal_DataType, AUTOMATIC, TCPIP_APPL_DATA) linkLocalPtr =
      &TcpIp_IpV4_LinkLocal_Data[linkLocalIdx];
    uint8 state;

    /* !LINKSTO TcpIp.Design.IpV4.LinkLocal.Atomic.State,1 */
    TS_AtomicAssign8(state, linkLocalPtr->state);

    if (method == linkLocalPtr->method)
    {
      if (state == TCPIP_IPV4_LINKLOCAL_STATE_CONFIGURED)
      {
        assigned = TRUE;
      }
    }
  }
  return assigned;
}

STATIC FUNC(uint16, TCPIP_CODE) TcpIp_IpV4_LinkLocal_startProbeTimer
(
    P2VAR(TcpIp_IpV4_LinkLocal_DataType, AUTOMATIC, TCPIP_APPL_DATA) linkLocalPtr
)
{
  uint16 probe_delay_time;

  if(linkLocalPtr->conflicts < TCPIP_IPV4_LINKLOCAL_MAX_CONFLICTS)
  {
    probe_delay_time =
      (uint16)TcpIp_GetRandomRange
        (
          TCPIP_IPV4_LINKLOCAL_DEFAULT_PROBE_WAIT*TCPIP_IPV4_LINKLOCAL_100MS_TO_1S_FACTOR
        );
  }
  else
  {
    probe_delay_time =
      (TCPIP_IPV4_LINKLOCAL_RATE_LIMIT_INTERVAL*TCPIP_IPV4_LINKLOCAL_100MS_TO_1S_FACTOR) + 1U;
  }

  return probe_delay_time;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_generateIp
(
    P2VAR(TcpIp_IpV4_LinkLocal_DataType, AUTOMATIC, TCPIP_APPL_DATA) linkLocalPtr
)
{
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) ipAddrPtr =
  (P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA)) &linkLocalPtr->ip_addr;

  ipAddrPtr[0U] = 169U;
  ipAddrPtr[1U] = 254U;
  ipAddrPtr[2U] = 1U + (uint8)TcpIp_GetRandomRange(253);
  ipAddrPtr[3U] = (uint8)TcpIp_GetRandomRange(254);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_timeout
(
    TcpIp_LocalAddrIdType localAddrId,
    uint8 linkLocalIdx
)
{
  TCPIP_PRECONDITION_ASSERT(linkLocalIdx < TCPIP_NUM_IPV4_LINK_LOCAL_ASSIGN, TCPIP_INTERNAL_API_ID);
  {
    P2VAR(TcpIp_IpV4_LinkLocal_DataType, AUTOMATIC, TCPIP_APPL_DATA) linkLocalPtr =
      &TcpIp_IpV4_LinkLocal_Data[linkLocalIdx];
    uint8 state;

    /* !LINKSTO TcpIp.Design.IpV4.LinkLocal.Atomic.State,1 */
    TS_AtomicAssign8(state, linkLocalPtr->state);

    switch (state)
    {
      case TCPIP_IPV4_LINKLOCAL_STATE_START_DELAY:
        TcpIp_IpV4_LinkLocal_tr_START_DELAY_PROBING(localAddrId,linkLocalIdx);
        break;
      /* CHECK: NOPARSE */
      case TCPIP_IPV4_LINKLOCAL_STATE_INACTIVE: /* intended fall through */
      case TCPIP_IPV4_LINKLOCAL_STATE_PROBING: /* intended fall through */
      case TCPIP_IPV4_LINKLOCAL_STATE_CONFIGURED: /* intended fall through */
      default:
        /* should not happen */
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break;
      /* CHECK: PARSE */
    }
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_tr_INACTIVE_START_DELAY
(
    uint8 linkLocalIdx
)
{
  /* exit */

  /* transition */

  /* entry */
  TcpIp_IpV4_LinkLocal_entry_START_DELAY(linkLocalIdx);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_tr_START_DELAY_PROBING
(
    TcpIp_LocalAddrIdType localAddrId,
    uint8 linkLocalIdx
)
{
  /* exit */

  /* transition */

  /* entry */
  TcpIp_IpV4_LinkLocal_entry_PROBING(localAddrId,linkLocalIdx,TRUE);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_probeResult_tr_PROBING_PROBING
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 linkLocalIdx,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) remotePhysAddr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) localPhysAddr
)
{
  TCPIP_PRECONDITION_ASSERT(linkLocalIdx < TCPIP_NUM_IPV4_LINK_LOCAL_ASSIGN, TCPIP_INTERNAL_API_ID);
  {
    P2VAR(TcpIp_IpV4_LinkLocal_DataType, AUTOMATIC, TCPIP_APPL_DATA) linkLocalPtr =
      &TcpIp_IpV4_LinkLocal_Data[linkLocalIdx];

    /* exit */
  TcpIp_IpV4_LinkLocal_exit_PROBING(localAddrId,linkLocalIdx);

    /* transition */
    if (linkLocalPtr->conflicts <= TCPIP_IPV4_LINKLOCAL_MAX_CONFLICTS)
    {
      linkLocalPtr->conflicts++;
    }
#if(TCPIP_DAD_FCT_ENABLE == STD_ON)
    {
      TcpIp_IpV4_AddrConflDetect_conflictCallout
      (
        localAddrId,
        &linkLocalPtr->ip_addr,
        localPhysAddr,
        remotePhysAddr
      );
    }
#else
    TS_PARAM_UNUSED(localPhysAddr);
    TS_PARAM_UNUSED(remotePhysAddr);
#endif /* (TCPIP_DAD_FCT_ENABLE == STD_ON) */

    /* entry */
    TcpIp_IpV4_LinkLocal_entry_PROBING(localAddrId,linkLocalIdx,FALSE);
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_tr_PROBING_CONFIGURED
(
    TcpIp_LocalAddrIdType localAddrId,
    uint8 linkLocalIdx
)
{
  TCPIP_PRECONDITION_ASSERT(linkLocalIdx < TCPIP_NUM_IPV4_LINK_LOCAL_ASSIGN, TCPIP_INTERNAL_API_ID);
  {
    P2VAR(TcpIp_IpV4_LinkLocal_DataType, AUTOMATIC, TCPIP_APPL_DATA) linkLocalPtr =
      &TcpIp_IpV4_LinkLocal_Data[linkLocalIdx];

  /* exit */
    TcpIp_IpV4_LinkLocal_exit_PROBING(localAddrId,linkLocalIdx);

    /* transition */
    linkLocalPtr->conflicts = 0U;

    /* entry */
    TcpIp_IpV4_LinkLocal_entry_CONFIGURED(localAddrId, linkLocalIdx);
  }
}
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_tr_CONFIGURED_INACTIVE
(
    TcpIp_LocalAddrIdType localAddrId,
    uint8 linkLocalIdx
)
{
#if (TCPIP_NVM_STORING_ENABLED == STD_ON)
  P2VAR(TcpIp_IpV4_LinkLocal_DataType, AUTOMATIC, TCPIP_APPL_DATA) linkLocalPtr =
  &TcpIp_IpV4_LinkLocal_Data[linkLocalIdx];
  TcpIp_Memory_NvM_setIpAddress(localAddrId,linkLocalPtr->ip_addr,linkLocalPtr->method);
#endif /* (TCPIP_NVM_STORING_ENABLED == STD_ON) */

  /* exit */
  TcpIp_IpV4_LinkLocal_exit_CONFIGURED(localAddrId,linkLocalIdx);

  /* transition */

  /* entry */
  TcpIp_IpV4_LinkLocal_entry_INACTIVE(linkLocalIdx);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_tr_START_DELAY_INACTIVE
(
  uint8 linkLocalIdx
)
{
  /* exit */

  /* transition */

  /* entry */
  TcpIp_IpV4_LinkLocal_entry_INACTIVE(linkLocalIdx);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_tr_PROBING_INACTIVE
(
    TcpIp_LocalAddrIdType localAddrId,
    uint8 linkLocalIdx
)
{
#if (TCPIP_NVM_STORING_ENABLED == STD_ON)
  P2VAR(TcpIp_IpV4_LinkLocal_DataType, AUTOMATIC, TCPIP_APPL_DATA) linkLocalPtr =
  &TcpIp_IpV4_LinkLocal_Data[linkLocalIdx];
  TcpIp_Memory_NvM_setIpAddress(localAddrId,linkLocalPtr->ip_addr,linkLocalPtr->method);
#endif /* (TCPIP_NVM_STORING_ENABLED == STD_ON) */

  /* exit */
  TcpIp_IpV4_LinkLocal_exit_PROBING(localAddrId,linkLocalIdx);

  /* transition */

  /* entry */
  TcpIp_IpV4_LinkLocal_entry_INACTIVE(linkLocalIdx);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_tr_CONFIGURED_PROBING
(
    TcpIp_LocalAddrIdType localAddrId,
    uint8 linkLocalIdx
)
{
  /* exit */
  TcpIp_IpV4_LinkLocal_exit_CONFIGURED(localAddrId, linkLocalIdx);

  /* transition */

  /* entry */
  TcpIp_IpV4_LinkLocal_entry_PROBING(localAddrId, linkLocalIdx,FALSE);
}


STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_entry_INACTIVE
(
    uint8 linkLocalIdx
)
{
  TCPIP_PRECONDITION_ASSERT(linkLocalIdx < TCPIP_NUM_IPV4_LINK_LOCAL_ASSIGN, TCPIP_INTERNAL_API_ID);
  {
    P2VAR(TcpIp_IpV4_LinkLocal_DataType, AUTOMATIC, TCPIP_APPL_DATA) linkLocalPtr =
      &TcpIp_IpV4_LinkLocal_Data[linkLocalIdx];

    linkLocalPtr->ip_addr = TCPIP_IPV4_ADDR_INVALID_VALUE;
    linkLocalPtr->timer = 0U;
    linkLocalPtr->method = TCPIP_IPADDRM_ASSIGN_LINKLOCALV4;

    linkLocalPtr->conflicts = 0U;
    /* !LINKSTO TcpIp.Design.IpV4.LinkLocal.Atomic.State,1 */
    TS_AtomicAssign8(linkLocalPtr->state, TCPIP_IPV4_LINKLOCAL_STATE_INACTIVE);
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_entry_START_DELAY
(
    uint8 linkLocalIdx
)
{
  TCPIP_PRECONDITION_ASSERT(linkLocalIdx < TCPIP_NUM_IPV4_LINK_LOCAL_ASSIGN, TCPIP_INTERNAL_API_ID);
  {
    P2VAR(TcpIp_IpV4_LinkLocal_DataType, AUTOMATIC, TCPIP_APPL_DATA) linkLocalPtr =
      &TcpIp_IpV4_LinkLocal_Data[linkLocalIdx];

    linkLocalPtr->timer =
      (uint16)TcpIp_PBcfgPtr->ipV4linkLocalConfig.autoIpInitTimeout*TCPIP_IPV4_LINKLOCAL_100MS_TO_1S_FACTOR;
    /* !LINKSTO TcpIp.Design.IpV4.LinkLocal.Atomic.State,1 */
    TS_AtomicAssign8(linkLocalPtr->state, TCPIP_IPV4_LINKLOCAL_STATE_START_DELAY);

    /* If timer is 0, transition to PROBE_DELAY immediately */
    if (linkLocalPtr->timer == 0U)
    {
      const TcpIp_LocalAddrIdType localAddrId =
        TcpIp_IpAddrM_getLocalAddrId(linkLocalIdx, TCPIP_IPADDRM_ASSIGN_LINKLOCALV4);
      TcpIp_IpV4_LinkLocal_timeout(localAddrId, linkLocalIdx);
    }
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_entry_PROBING
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 linkLocalIdx,
  boolean nvmGet
)
{
  TCPIP_PRECONDITION_ASSERT(linkLocalIdx < TCPIP_NUM_IPV4_LINK_LOCAL_ASSIGN, TCPIP_INTERNAL_API_ID);
  {
    P2VAR(TcpIp_IpV4_LinkLocal_DataType, AUTOMATIC, TCPIP_APPL_DATA) linkLocalPtr =
    &TcpIp_IpV4_LinkLocal_Data[linkLocalIdx];

    const uint8 method = linkLocalPtr->method;
    const uint8 timing =
      ((method == TCPIP_IPADDRM_ASSIGN_LINKLOCALV4) ?
        TCPIP_IPV4_LINKLOCAL_DEFAULT_TIMINGS :
        TCPIP_IPV4_LINKLOCAL_DOIP_TIMINGS);
    const boolean random = TcpIp_IpV4_LinkLocal_Timings[timing].probe_random;
    const uint8 numProbes = TcpIp_IpV4_LinkLocal_Timings[timing].probe_num;
    const uint8 lastProbeWait = TcpIp_IpV4_LinkLocal_Timings[timing].announce_wait;
    const TcpIp_IpV4_ArpProbeResult_FctPtrType probeResultFctPtr =
      ((method == TCPIP_IPADDRM_ASSIGN_LINKLOCALV4) ?
        &TcpIp_IpV4_LinkLocal_probeResult_AutoIp :
        &TcpIp_IpV4_LinkLocal_probeResult_AutoIp_DoIp);
    const uint16 probe_delay = TcpIp_IpV4_LinkLocal_startProbeTimer(linkLocalPtr);
#if (TCPIP_NVM_STORING_ENABLED == STD_ON)
    uint32 storedAddress = 0U;
    boolean nvmObtainedAddress = TcpIp_Memory_NvM_getIpAddress(localAddrId,&(storedAddress),linkLocalPtr->method);
    /* if address should be obtained from Memory unit */
    if((nvmGet) && (nvmObtainedAddress))
    {
      /* Ip address needs to be linkLocal */
      nvmObtainedAddress = TCPIP_IPV4_ARP_ADDR_IS_LINKLOCAL
                 ((P2CONST(uint8,AUTOMATIC,TCPIP_APPL_DATA))&storedAddress);
      linkLocalPtr->ip_addr = storedAddress;
    }

    /* if address should be generated */
    if(!((nvmGet) && (nvmObtainedAddress)))
#else /* (TCPIP_NVM_STORING_ENABLED == STD_ON) */
    TS_PARAM_UNUSED(nvmGet);
#endif /* (TCPIP_NVM_STORING_ENABLED == STD_ON) */
    {
      TcpIp_IpV4_LinkLocal_generateIp(linkLocalPtr);
#if (TCPIP_NVM_STORING_ENABLED == STD_ON)
      {
        if((linkLocalPtr->ip_addr) == storedAddress)
        {
          /* If the generated address is equal to the stored address, regenerate it */
          TcpIp_IpV4_LinkLocal_generateIp(linkLocalPtr);
          TCPIP_PRECONDITION_ASSERT((linkLocalPtr->ip_addr) != storedAddress, TCPIP_INTERNAL_API_ID);
        }
      }
#endif /* (TCPIP_NVM_STORING_ENABLED == STD_ON) */
    }

    TcpIp_IpV4_ArpProbe
    (
      probeResultFctPtr,
      linkLocalPtr->ip_addr,
      numProbes,
      lastProbeWait,
      random,
      localAddrId,
      linkLocalPtr->method,
      probe_delay
    );

  /* !LINKSTO TcpIp.Design.IpV4.LinkLocal.Atomic.State,1 */
    TS_AtomicAssign8(linkLocalPtr->state, TCPIP_IPV4_LINKLOCAL_STATE_PROBING);
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_exit_PROBING
(
  TcpIp_LocalAddrIdType localAddrId,
  uint8 linkLocalIdx
)
{
  P2VAR(TcpIp_IpV4_LinkLocal_DataType, AUTOMATIC, TCPIP_APPL_DATA) linkLocalPtr =
        &TcpIp_IpV4_LinkLocal_Data[linkLocalIdx];

  TcpIp_IpV4_ArpProbe(NULL_PTR, TCPIP_IPV4_ADDR_INVALID_VALUE, 0U, 0U, 0U, localAddrId, linkLocalPtr->method, 0U);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_entry_CONFIGURED
(
    TcpIp_LocalAddrIdType localAddrId,
    uint8 linkLocalIdx
)
{
  TCPIP_PRECONDITION_ASSERT(linkLocalIdx < TCPIP_NUM_IPV4_LINK_LOCAL_ASSIGN, TCPIP_INTERNAL_API_ID);
  {
    P2VAR(TcpIp_IpV4_LinkLocal_DataType, AUTOMATIC, TCPIP_APPL_DATA) linkLocalPtr =
      &TcpIp_IpV4_LinkLocal_Data[linkLocalIdx];

    /* !LINKSTO TcpIp.Design.IpV4.LinkLocal.Atomic.State,1 */
    TS_AtomicAssign8(linkLocalPtr->state, TCPIP_IPV4_LINKLOCAL_STATE_CONFIGURED);

    /* Inform IpAddrM that link local IP address can be used */
    TcpIp_IpAddrM_AssignSM_assigned(localAddrId, linkLocalPtr->method);
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_LinkLocal_exit_CONFIGURED
(
    TcpIp_LocalAddrIdType localAddrId,
    uint8 linkLocalIdx
)
{
  TCPIP_PRECONDITION_ASSERT(linkLocalIdx < TCPIP_NUM_IPV4_LINK_LOCAL_ASSIGN, TCPIP_INTERNAL_API_ID);
  {
    P2VAR(TcpIp_IpV4_LinkLocal_DataType, AUTOMATIC, TCPIP_APPL_DATA) linkLocalPtr =
      &TcpIp_IpV4_LinkLocal_Data[linkLocalIdx];

    /* Inform IpAddrM that link local IP address can not be used */
    TcpIp_IpAddrM_AssignSM_unassigned(localAddrId, linkLocalPtr->method);
  }
}

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#else
  TS_PREVENTEMPTYTRANSLATIONUNIT
#endif /* (TCPIP_IPV4_LINK_LOCAL_ENABLED == STD_ON) */

/*==================[end of file]===========================================*/
