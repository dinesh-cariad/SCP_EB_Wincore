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
 * MISRAC2012-1) Deviated Rule: 20.10 (advisory)
 * The # and ## preprocessor operators should not be used.
 *
 * Reason:
 * Usage of concatenation operator improves readability of code.
 *
 * MISRAC2012-3) Deviated Rule: 11.3 (required)
 * A cast shall not be performed between a pointer to object type and a pointer to a different
 * object type.
 *
 * Reason:
 * AUTOSAR defines a generic type which dependent on a member of the generic type
 * implements an concrete type. Thus for usage of this concrete type, a cast is necessary.
 *
 */

/*==================[inclusions]============================================*/

#include <EthIf.h>              /* EthIf API declarations */
#include <SchM_TcpIp.h>         /* SchM API declarations */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_IpAddrM_Int.h>  /* used SW-unit interface file */
#include <TcpIp_IpV4_Priv.h>    /* own SW-unit private header file; must be last */
#include <TcpIp_IpSecDb_Int.h>    /* external IpSec Api declarations*/

/*==================[macros]================================================*/

/** \brief TcpIp_GetPhysAddr() service ID */
#define TCPIP_SID_GETARPCACHEENTRIES               0x1dU

/** \brief Length of Ethernet address */
#define TCPIP_IPV4_ARP_HADDR_ETH_LENGTH TCPIP_ETH_ADDR_LENGTH

/** \brief Length of IPv4 Address */
#define TCPIP_IPV4_ARP_PADDR_IP_LENGTH 4U

/** \brief ARP HW-Type for Ethernet */
#define TCPIP_IPV4_ARP_HTYPE_ETH 1U

/** \brief ARP Protocol type for Ip */
#define TCPIP_IPV4_ARP_PROTO_IP 0x800U

/** \brief length of ARP control header control information withpout addresses */
#define TCPIP_IPV4_ARP_HEADER_CONTROL_LENGTH 8U

/** \brief  Macro checks ARP protocol header: full header contained in datagram */
#define TCPIP_IPV4_ARP_HEADER_LENGTH_CHECK(htype,proto,length)\
  ((length) >= TCPIP_IPV4_ARP_HEADER_LENGTH(htype,proto))

/** \brief ARP header length */
#define TCPIP_IPV4_ARP_HEADER_LENGTH(htype,proto)   ((TCPIP_IPV4_ARP_HADDR_LENGTH(htype)*2U) + \
                                                     (TCPIP_IPV4_ARP_PADDR_LENGTH(proto)*2U) + \
                                                     TCPIP_IPV4_ARP_HEADER_CONTROL_LENGTH)

/** \brief ARP HW address length */
/* Deviation MISRAC2012-1 */
#define TCPIP_IPV4_ARP_HADDR_LENGTH(htype) TCPIP_IPV4_ARP_HADDR_##htype##_LENGTH

/** \brief ARP protocol address length */
/* Deviation MISRAC2012-1 */
#define TCPIP_IPV4_ARP_PADDR_LENGTH(proto) TCPIP_IPV4_ARP_PADDR_##proto##_LENGTH

/** \brief Macro checks ARP protocol header HW-Type */
/* Deviation MISRAC2012-1 */
#define TCPIP_IPV4_ARP_HWTYPE_IS(htype,headerPtr)\
  ((((uint16)(((uint16)(headerPtr)[0])<<8U))|((uint16)(headerPtr)[1])) == TCPIP_IPV4_ARP_HTYPE_##htype)

/** \brief Macro checks ARP protocol header Protocol */
/* Deviation MISRAC2012-1 */
#define TCPIP_IPV4_ARP_PROTO_IS(proto,headerPtr)\
  ((((uint16)(((uint16)(headerPtr)[2])<<8U))|((uint16)(headerPtr)[3])) == TCPIP_IPV4_ARP_PROTO_##proto)

/** \brief Macro checks ARP protocol header: HW-Address-Length */
/* Deviation MISRAC2012-1 */
#define TCPIP_IPV4_ARP_CHECK_HADDR_LENGTH(htype,headerPtr)\
  (((headerPtr)[4]) == TCPIP_IPV4_ARP_HADDR_##htype##_LENGTH)

/** \brief Macro checks ARP protocol header: Protocol-Address-Length */
/* Deviation MISRAC2012-1 */
#define TCPIP_IPV4_ARP_CHECK_PADDR_LENGTH(proto,headerPtr)\
  (((headerPtr)[5]) == TCPIP_IPV4_ARP_PADDR_##proto##_LENGTH)

/** \brief Macro returns the ARP Opcode from ARP-header*/
#define TCPIP_IPV4_ARP_OPCODE_GET(headerPtr)\
  (((uint16)(((uint16)(headerPtr)[6])<<8U))|((uint16)(headerPtr)[7]))

/** \brief Macro contains the ARP Opcode: Request message */
#define TCPIP_IPV4_ARP_OPCODE_REQUEST 0x01U

/** \brief Macro contains the ARP Opcode: Reply message */
#define TCPIP_IPV4_ARP_OPCODE_REPLY 0x02U

/** \brief Macro returns the destination protocol (IP) address from ARP header */
#define TCPIP_IPV4_ARP_DEST_IPADDR_GET(headerPtr,ipAddr) \
  do { \
   ((uint8*)&(ipAddr))[0] = ((headerPtr)[24]); \
   ((uint8*)&(ipAddr))[1] = ((headerPtr)[25]); \
   ((uint8*)&(ipAddr))[2] = ((headerPtr)[26]); \
   ((uint8*)&(ipAddr))[3] = ((headerPtr)[27]); \
  }while(0)

/** \brief Macro returns the source protocol address (IP) from ARP header */
#define TCPIP_IPV4_ARP_SRC_IPADDR_GET(headerPtr,ipAddr) \
  do { \
   ((uint8*)&(ipAddr))[0] = ((headerPtr)[14]); \
   ((uint8*)&(ipAddr))[1] = ((headerPtr)[15]); \
   ((uint8*)&(ipAddr))[2] = ((headerPtr)[16]); \
   ((uint8*)&(ipAddr))[3] = ((headerPtr)[17]); \
  }while(0)


/** \brief Macro sets the ARP Opcode of ARP-header*/
#define TCPIP_IPV4_ARP_OPCODE_SET(opcode, headerPtr) \
    do { \
    (headerPtr)[6] = (uint8)(((opcode)>>8U)&0xFFU); \
    (headerPtr)[7] = (uint8)((opcode)&0xFFU);       \
    }while(0) \

/** \brief Macro sets the destination protocol address (IP) of ARP header */
#define TCPIP_IPV4_ARP_DEST_IPADDR_SET(ipAddr,headerPtr)  \
  do { \
    (headerPtr)[24] = ((const uint8*)&(ipAddr))[0]; \
    (headerPtr)[25] = ((const uint8*)&(ipAddr))[1]; \
    (headerPtr)[26] = ((const uint8*)&(ipAddr))[2]; \
    (headerPtr)[27] = ((const uint8*)&(ipAddr))[3]; \
    }while(0) \

/** \brief Macro sets the source protocol address (IP) of ARP header */
#define TCPIP_IPV4_ARP_SRC_IPADDR_SET(ipAddr,headerPtr)  \
  do { \
    (headerPtr)[14] = ((const uint8*)&(ipAddr))[0]; \
    (headerPtr)[15] = ((const uint8*)&(ipAddr))[1]; \
    (headerPtr)[16] = ((const uint8*)&(ipAddr))[2]; \
    (headerPtr)[17] = ((const uint8*)&(ipAddr))[3]; \
    }while(0) \

/** \brief Macro sets the HW-Address-Length of ARP header */
/* Deviation MISRAC2012-1 */
#define TCPIP_IPV4_ARP_HADDR_LENGTH_SET(htype,headerPtr)\
  (((headerPtr)[4]) = TCPIP_IPV4_ARP_HADDR_##htype##_LENGTH)

/** \brief Macro sets the Protocol-Address-Length of ARP header */
/* Deviation MISRAC2012-1 */
#define TCPIP_IPV4_ARP_PADDR_LENGTH_SET(proto,headerPtr)\
  (((headerPtr)[5]) = TCPIP_IPV4_ARP_PADDR_##proto##_LENGTH)

/** \brief Macro sets the HW-Type of ARP header */
#define TCPIP_IPV4_ARP_HTYPE_SET(htype,headerPtr) \
  TCPIP_SET16((headerPtr), 0, (htype))

/** \brief Macro sets the Protocol-Type of ARP header */
#define TCPIP_IPV4_ARP_PROTO_SET(proto,headerPtr) \
  TCPIP_SET16((headerPtr), 2, (proto))

/** \brief Macro returns pointer to start of the HW source address of ARP header*/
#define TCPIP_IPV4_ARP_LOCAL_HADDR_GET_PTR(htype,proto,headerPtr)\
  (&((headerPtr)[TCPIP_IPV4_ARP_HEADER_CONTROL_LENGTH]))

/** \brief Macro returns pointer to start of the HW destination address of ARP header*/
#define TCPIP_IPV4_ARP_REMOTE_HADDR_GET_PTR(htype,proto,headerPtr)\
  (&((headerPtr)[TCPIP_IPV4_ARP_HEADER_CONTROL_LENGTH + \
    TCPIP_IPV4_ARP_HADDR_LENGTH(htype) +  \
    TCPIP_IPV4_ARP_PADDR_LENGTH(proto)]))

/** \brief Factor to calculate 1s to 100ms steps */
#define TCPIP_IPV4_ARP_100MS_TO_1S_FACTOR  10U

/** \brief Mask to check if a remote physical address entry in the table is valid */
#define TCPIP_IPV4_ARP_VALID_PHYS_ADDR_MASK   0x05U
/** \brief Mask to check if state is valid */
#define TCPIP_IPV4_ARP_VALID_STATE_MASK       0x45U

/** \brief Macro to verify if an entry in the table is valid (REACHABLE or STATIC) */
#define TCPIP_IPV4_ARP_PHSY_ADDR_VALID(state)        \
  (((state) & TCPIP_IPV4_ARP_VALID_PHYS_ADDR_MASK) != 0U)

/** \brief Macro to verify if state is valid (REACHABLE, STATIC, FREE) */
#define TCPIP_IPV4_ARP_STATE_VALID(state)        \
  (((state) & TCPIP_IPV4_ARP_VALID_STATE_MASK) != 0U)

/** \brief Returns the Arp Dad cache index (0-based and dense) for the given local address Id and assignment method pair */
#define TcpIp_IpV4_ArpDad_getCacheIdx(localAddrId,method) \
  ((TCPIP_CFG(localAddr,localAddrId)).assignmByMethod[(method)].dadIpV4Idx)

/** \brief Number of ARP DAD entries */
#define TCPIP_IPV4_ARP_DAD_NUM  TcpIp_PBcfgPtr->IpV4ArpDadNum
/*==================[type definitions]======================================*/

#if (TCPIP_IPV4_ENABLED == STD_ON)

/** \brief This struct contains information for a single ARP table entry. */
typedef struct
{
  TcpIp_ArpCacheEntryType remoteAddr; /**< contains the ip address, physical address and state of the remote host */
  uint32  stateTime;    /**< remaining time the ARP entry is alive */
  uint8   lock;         /**< indicates whether ARP table entry is in use (> 0) or not in use (= 0) */
}TcpIp_IpV4_ArpEntryType;


/** \brief This struct contains information for ARP table entries per controller. */
typedef struct
{
  TcpIp_IpV4_ArpEntryType arpTable[TCPIP_ARP_TABLESIZE_MAX];  /**< arp table for this controller */
  uint32 ipAnnounce;                                          /**< Ip address used for ARP announcements */
  uint8 countAnnounce;                                        /**< count of remaining announcements */
  uint8 announceTime;                                         /**< announce timer */
  uint8 announceInterval;                                     /**< interval of announcement messages */
  uint8 announceType;                                         /**< type of announcement messages */

}TcpIp_IpV4_ArpStateType;

#if (TCPIP_ARP_DAD_ENABLED == STD_ON)
/** \brief This struct contains information for a single ARP DAD table entry. */
typedef struct
{
  uint32 ipProbe;                                             /**< Ip address used for ARP probing */
  TcpIp_IpV4_ArpProbeResult_FctPtrType probeResultFctPtr;     /**< Calback pointer to report probe result to */
  uint8 countProbe;                                           /**< count of remaining probes */
  uint16 probeTime;                                           /**< probe timer */
  uint8 lastProbeWait;                                        /**< wait time after last probe */
  uint8 randomProbeInterval;                                  /**< shall probing use random interval time ? */
}TcpIp_IpV4_ArpDadStateType;
#endif /* (TCPIP_ARP_DAD_ENABLED == STD_ON) */

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>


/** \brief Check ARP header for validity
 *
 * \param[in] dataPtr    Address of ARP message
 * \param[in] lenByte    length of message in units of bytes
 *
 * \return Validity of ARP header
 * \retval FALSE ARP header is invalid
 * \retval TRUE ARP header is valid
 *
 */
STATIC FUNC( boolean, TCPIP_CODE ) TcpIp_IpV4_ArpCheckHeader
(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 lenByte
);

/** \brief Inserts or updates ARP entries
 *
 * \param[in] ctrlIdx         Interface the ARP message maps to
 * \param[in] localAddrId     Unicast Local address Id of the interface
 * \param[in] remoteIpAddr    IP address of the remote host
 * \param[in] remotePhysAddr  Physical address of the remote host
 * \param[in] physAddrPtr     physical address of remote host on which arp was received
 *
 * \return transmission of arp reply
 * \retval FALSE  arp reply shall not be transmitted
 * \retval TRUE   arp reply shall be transmitted
 *
 */
STATIC FUNC( boolean, TCPIP_CODE ) TcpIp_IpV4_ArpCacheUpdate
(
  uint8 ctrlIdx,
  TcpIp_LocalAddrIdType localAddrId,
  uint32 remoteIpAddr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) remotePhysAddr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr
);

/** \brief gets ARP table entries which may be used for update/insertion/replacement
 *
 * \param[in] ipAddr                Remote IpV4 address
 * \param[in]  ctrlIdx              Interface the ARP message maps to
 * \param[out] replace              Indicates that the entry was replaced

 * \return - an existing entry for ipAddr if exists
 *         - a free ARP entry
 *         - the oldest ARP entry
 *         - TCPIP_ARP_TABLESIZE_MAX no entry found (all are locked)
 */
STATIC FUNC( uint16, TCPIP_CODE ) TcpIp_IpV4_ArpGetEntry
(
  uint32 ipAddr,
  uint8 ctrlIdx,
  P2VAR(boolean, AUTOMATIC, TCPIP_APPL_DATA) replace
);

/** \brief search ARP table for matching remote addresses
 *
 * \param[in] ipAddr                Remote IpV4 address
 * \param[in]  ctrlIdx              Interface the ARP message maps to

 * \return - an existing entry for ipAddr if exists
 *         - TCPIP_ARP_TABLESIZE_MAX no entry found
 */
STATIC FUNC( uint16, TCPIP_CODE ) TcpIp_IpV4_ArpGetExistingEntry
(
  uint32 ipAddr,
  uint8 ctrlIdx
);

/** \brief Sets ARP entry to reachable, updates physical address, ip address
 *         and alive timeout
 *
 * \param[in] arpEntryPtr           pointer to entry data
 * \param[in] physAddrPtr           physical address which shall be set
 * \param[in] ipAddr                ip address which shall be set
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_Arp_entry_REACHABLE
(
  P2VAR(TcpIp_IpV4_ArpEntryType, AUTOMATIC, TCPIP_APPL_DATA) arpEntryPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr,
  uint32 ipAddr
);

/** \brief Sets ARP entry to static, updates physical address, ip address
 *
 * \param[in] arpEntryPtr           pointer to entry data
 * \param[in] physAddrPtr           physical address which shall be set
 * \param[in] ipAddr                ip address which shall be set
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_Arp_entry_STATIC
(
  P2VAR(TcpIp_IpV4_ArpEntryType, AUTOMATIC, TCPIP_APPL_DATA) arpEntryPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr,
  uint32 ipAddr
);

/** \brief Transmits an ARP message
 *
 *  This function transmits an ARP message.
 *
 * \param[in] remoteIpAddr        destination IP address
 * \param[in] localIpAddr         local IP address
 * \param[in] remotePhysAddrPr    pointer to destination physical address
 *                                (if not known: TcpIp_IpV4_EthUnknownAddress)
 * \param[in] targetPhysAddrPtr   pointer to destination physical address used for address
 *                                resolution
 * \param[in]  opcode             ARP opcode
 * \param[in]  ctrlIdx            controller index
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_ArpTransmit
  (
    uint32 remoteIpAddr,
    uint32 localIpAddr,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) remotePhysAddrPtr,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) targetPhysAddrPtr,
    uint16 opcode,
    uint8 ctrlIdx
  );

/** \brief Set physical address of remote host in ARP table
 *
 * \param[in]  CtrlIdx      TcpIp controller index to identify the related ARP table.
 * \param[in]  IpAddrPtr    specifies the IP address for which the physical address shall be
 *                          set in the ARP table
 * \param[in]  PhysAddrPtr  specifies the physical address which shall be set in the ARP table
 * \param[in]  State        TCPIP_PHYS_ADDR_ENTRY_STATE_REACHABLE: add/update the specified
 *                          entry in the ARP table
 *                          TCPIP_PHYS_ADDR_ENTRY_STATE_STATIC: set the specified entry in
 *                          the ARP table to static
 *
 * \return Result of operation
 * \retval TCPIP_OK:           physical address successfully added/updated
 * \retval TCPIP_E_NOT_OK:     entry is currently locked
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_IpV4_ArpSetAddr
(
  uint8 CtrlIdx,
  uint32 IpAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr,
  uint8 State
);

/** \brief Remove remote host from ARP table
 *
 *  This function removes the physical address of the specified remote host from
 *  the ARP table
 *
 * \param[in] ipAddr    ip address of the remote host that shall be removed
 * \param[in] ctrlIdx   controller index
 *
 * \return Result of the operation
 * \retval TCPIP_OK                 Remote host successfully removed
 * \retval TCPIP_E_PHYS_ADDR_MISS   Remote host could not be found
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE ) TcpIp_IpV4_ArpRemove
(
  uint32 ipAddr,
  uint8 ctrlIdx
);

/** \brief ARP table main function
 *
 *  This function performs the ARP table related timing.
 */
STATIC FUNC( void, TCPIP_CODE ) TcpIp_IpV4_ArpTableMainFunction(void);

/** \brief Sets ARP entry to free, if timeout is reached
 *
 * \param[in] arpEntryPtr       pointer to entry data
 * \param[in] ctrlIdx           controller index
 */
STATIC FUNC( void, TCPIP_CODE ) TcpIp_IpV4_ArpTimeout
(
  P2VAR(TcpIp_IpV4_ArpEntryType, AUTOMATIC, TCPIP_APPL_DATA) arpEntryPtr,
  uint8 ctrlIdx
);

/** \brief ARP announce main function
 *
 *  This function performs the ARP announcement related timing.
 *
 * \param[in] timerTick        current time in ticks
 */
STATIC FUNC( void, TCPIP_CODE ) TcpIp_IpV4_ArpAnnounceMainFunction(uint16 timerTick);

#if (TCPIP_ARP_DAD_ENABLED == STD_ON)
STATIC FUNC( void, TCPIP_CODE ) TcpIp_IpV4_ArpProbeMainFunction(uint16 timerTick);

STATIC FUNC(boolean, TCPIP_CODE ) TcpIp_IpV4_arpProbeResponseDetection
(
  uint8 ctrlIdx,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) remotePhysAddr,
  uint8 opcode,
  uint32 srcIpAddr,
  uint32 destIpAddr
);
#endif /* (TCPIP_ARP_DAD_ENABLED == STD_ON) */

#if TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON
/** \brief Call ARP cache change callout
 *
 *  This function calls the arp table entry change callout.
 *
 * \param[in] ctrlIdx       controller index
 * \param[in] ipAddr        Ip address of modified entry
 * \param[in] physAddrPtr   pointer to physical address of modified entry
 * \param[in] valid         entry has been addred/updated (TRUE) or removed(FALSE)
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_ArpChangeFct
(
  uint8 ctrlIdx,
  uint32 ipAddr,
  P2VAR(uint8,AUTOMATIC,TCPIP_APPL_DATA) physAddrPtr,
  boolean valid
);
#endif /* TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON */

/** \brief Check if local address is a unicast
 *
 *  This function checks if a local address is a unicast or not specified
 *
 * \param[in] localAddrId   Address Id of the destination address
 */
STATIC FUNC(boolean, TCPIP_CODE) TcpIp_Arp_LocalAddrIsUnicast
(
  TcpIp_LocalAddrIdType localAddrId
);

/** \brief Check if remote host is unicast and within subnet
 *
 * \param[in] remoteIpAddr  remote host ip address
 * \param[in] addrId        Address Id of the destination address
 */
STATIC FUNC(boolean, TCPIP_CODE) TcpIp_Arp_isSubnetComm
(
  uint32 remoteIpAddr,
  TcpIp_LocalAddrIdType addrId
);

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

#define TCPIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

/* ARP table memory allocation */
STATIC VAR(TcpIp_IpV4_ArpStateType,TCPIP_VAR_CLEARED) TcpIp_IpV4_ArpState[TCPIP_NUM_ETHIFCTRL];

#if (TCPIP_ARP_DAD_ENABLED == STD_ON)
/* ARP DAD table memory allocation */
STATIC VAR(TcpIp_IpV4_ArpDadStateType,TCPIP_VAR_CLEARED) TcpIp_IpV4_ArpDadState[TCPIP_NUM_ARP_DAD_ENTRIES];
#endif /* (TCPIP_ARP_DAD_ENABLED == STD_ON) */

#define TCPIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

#define TCPIP_START_SEC_VAR_INIT_8
#include <TcpIp_MemMap.h>

/* value for unknown ethernet mac address */
STATIC VAR(uint8,TCPIP_VAR) TcpIp_IpV4_EthUnknownAddress[TCPIP_IPV4_ARP_HADDR_LENGTH(ETH)] =
  {0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U };

#define TCPIP_STOP_SEC_VAR_INIT_8
#include <TcpIp_MemMap.h>

#define TCPIP_START_SEC_VAR_INIT_16
#include <TcpIp_MemMap.h>

/* number of elapsed hundreds of a second */
STATIC VAR(uint32,TCPIP_VAR) TcpIp_IpV4_ArpCounter = 0U;

#define TCPIP_STOP_SEC_VAR_INIT_16
#include <TcpIp_MemMap.h>


#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)

#define TCPIP_START_SEC_VAR_INIT_32
#include <TcpIp_MemMap.h>

STATIC VAR(uint32, TCPIP_VAR) TcpIp_IpV4_Arp_ReplacedArpEntryCounter = 0U;
STATIC VAR(uint32, TCPIP_VAR) TcpIp_IpV4_Arp_DroppedLockedArpEntryCounter = 0U;

#define TCPIP_STOP_SEC_VAR_INIT_32
#include <TcpIp_MemMap.h>

#endif /* TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */

#endif /*  (TCPIP_IPV4_ENABLED == STD_ON) */

/*==================[external function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetArpCacheEntries
(
  uint8 ctrlIdx,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) numberOfElements,
  P2VAR(TcpIp_ArpCacheEntryType, AUTOMATIC, TCPIP_APPL_DATA) entryListPtr
)
{
  Std_ReturnType retVal = E_NOT_OK;

  DBG_TCPIP_GETARPCACHEENTRIES_ENTRY(ctrlIdx,numberOfElements,entryListPtr);

#if (TCPIP_IPV4_ENABLED == STD_ON)
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  if( TCPIP_UNINIT )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_GETARPCACHEENTRIES, TCPIP_E_NOTINIT);
  }
  else if( (ctrlIdx > TcpIp_PBcfgPtr->maxCtrlIdx) ||
            (TCPIP_CFG_TYPE(uint8,ctrlMapping,ctrlIdx) == TCPIP_CTRLIDX_INVALID)
          )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_GETARPCACHEENTRIES, TCPIP_E_INV_ARG);
  }
  else if(numberOfElements == NULL_PTR)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_GETARPCACHEENTRIES, TCPIP_EB_E_PARAM_POINTER);
  }
  else if((*numberOfElements > 0U) && (entryListPtr == NULL_PTR))
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_GETARPCACHEENTRIES, TCPIP_EB_E_PARAM_POINTER);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
    uint8 TcpIp_ctrlIdx = TCPIP_CFG_TYPE(uint8,ctrlMapping,ctrlIdx);
    /* get the unicast address of the controller */
    const TcpIp_LocalAddrIdType localAddrId =
      TcpIp_IpV4_getTxLocalAddrId_byCtrlIdx(TcpIp_ctrlIdx, NULL_PTR);

    if(TcpIp_IpAddrM_lockIfOnlineShutdown(localAddrId))
    {
      uint16_least i;
      uint16 numValidElements = 0U;

      /* !LINKSTO TcpIp.Design.IpV4.EA1.GetArpCacheEntries,1 */
      SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();

      /* loop all entries of ARP table for specific interface */
      for(i = 0U; i < TCPIP_ARP_TABLESIZE_MAX; i++)
      {
        CONSTP2VAR(TcpIp_IpV4_ArpEntryType,AUTOMATIC,TCPIP_VAR_CLEARED) arpEntryPtr =
          &TcpIp_IpV4_ArpState[TcpIp_ctrlIdx].arpTable[i];

        /* check if this entry is valid */
        if(TCPIP_IPV4_ARP_PHSY_ADDR_VALID(arpEntryPtr->remoteAddr.State))
        {
          if(*numberOfElements > 0U)
          {
            entryListPtr[numValidElements] = arpEntryPtr->remoteAddr;
            numValidElements++;

            if(numValidElements == *numberOfElements)
            {
              /* save up to numberOfElements valid entries */
              break;
            }
          }
          else
          {
            /* if numberOfElements is 0 just count the valid entries */
            numValidElements++;
          }
        }
      }

      /* !LINKSTO TcpIp.Design.IpV4.EA1.GetArpCacheEntries,1 */
      SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();

      *numberOfElements = numValidElements;

      TcpIp_IpAddrM_unlockIf(localAddrId);

      retVal = E_OK;
    }
  }
#else /* (TCPIP_IPV4_ENABLED == STD_ON) */
  TS_PARAM_UNUSED(ctrlIdx);
  TS_PARAM_UNUSED(numberOfElements);
  TS_PARAM_UNUSED(entryListPtr);

#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  TCPIP_REPORT_ERROR(TCPIP_SID_GETARPCACHEENTRIES, TCPIP_E_INV_ARG);
#endif /* (TCPIP_DEV_ERROR_DETECT == STD_ON) */
#endif /* (TCPIP_IPV4_ENABLED == STD_ON) */

  DBG_TCPIP_GETARPCACHEENTRIES_EXIT(retVal,ctrlIdx,numberOfElements,entryListPtr);

  return retVal;
}

#if(TCPIP_IPV4_ENABLED == STD_ON)

TS_MOD_PRIV_DEFN FUNC( void, TCPIP_CODE ) TcpIp_IpV4_ArpInit(void)
{
  uint8_least i;
  uint16_least j;

  /* loop over all interface ARP tables */
  for(i = 0U; i < TCPIP_NUM_ETHIFCTRL; i++)
  {

    CONSTP2VAR(TcpIp_IpV4_ArpStateType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_IpV4_ArpStatePtr
                                                                        = &TcpIp_IpV4_ArpState[i];
    TcpIp_IpV4_ArpStatePtr->ipAnnounce = TCPIP_IPV4_ADDR_INVALID_VALUE;
    TcpIp_IpV4_ArpStatePtr->announceTime = 0U;

    /* loop over all ARP table entries */
    for(j = 0U; j < TCPIP_ARP_TABLESIZE_MAX; j++)
    {
      /* invalidate ARP table entry */
      TcpIp_IpV4_ArpStatePtr->arpTable[j].stateTime = 0U;
      TcpIp_IpV4_ArpStatePtr->arpTable[j].remoteAddr.State = TCPIP_PHYS_ADDR_ENTRY_STATE_FREE;
      /* unlock ARP table entry */
      TcpIp_IpV4_ArpStatePtr->arpTable[j].lock = 0U;
    }
  }

#if (TCPIP_ARP_DAD_ENABLED == STD_ON)
  /* loop over all ARP DAD entries */
  for(j = 0U; j < TCPIP_IPV4_ARP_DAD_NUM; j++)
  {
    CONSTP2VAR(TcpIp_IpV4_ArpDadStateType, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_IpV4_ArpDadStatePtr
                                                                        = &TcpIp_IpV4_ArpDadState[j];
    TcpIp_IpV4_ArpDadStatePtr->ipProbe = TCPIP_IPV4_ADDR_INVALID_VALUE;
    TcpIp_IpV4_ArpDadStatePtr->probeTime = 0U;
  }
#endif /* (TCPIP_ARP_DAD_ENABLED == STD_ON) */

  /* initialize the seconds counter */
  TcpIp_IpV4_ArpCounter = 0U;
}



TS_MOD_PRIV_DEFN FUNC( void, TCPIP_CODE ) TcpIp_IpV4_ArpMainFunction(uint16 timerTick)
{
  TcpIp_IpV4_ArpTableMainFunction();
#if (TCPIP_ARP_DAD_ENABLED == STD_ON)
  TcpIp_IpV4_ArpProbeMainFunction(timerTick);
#endif /* (TCPIP_ARP_DAD_ENABLED == STD_ON) */
  TcpIp_IpV4_ArpAnnounceMainFunction(timerTick);
}



TS_MOD_PRIV_DEFN FUNC(TcpIp_Ip_RxReturnType, TCPIP_CODE ) TcpIp_IpV4_rxIndicationArp
(
  uint8 ctrlIdx,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 lenByte
)
{
  TcpIp_Ip_RxReturnType RxRetCode = TCPIP_IP_RX_NOT_OK;

  DBG_TCPIP_IPV4_RXINDICATIONARP_ENTRY(ctrlIdx,physAddrPtr,dataPtr,lenByte);

  /* check for a valid ARP header */
  if(TcpIp_IpV4_ArpCheckHeader(dataPtr,lenByte))
  {
    /* extract IP addresses from header  */
    uint32 srcIpAddr;
    uint8 srcPhysAddr[6U];

    TCPIP_IPV4_ARP_SRC_IPADDR_GET(dataPtr,srcIpAddr);

    TcpIp_EthAddrCpy(srcPhysAddr, TCPIP_IPV4_ARP_LOCAL_HADDR_GET_PTR(ETH,IP,dataPtr));

    if(!TcpIp_IpV4_AddrConflDetect_arpDetection(ctrlIdx,srcPhysAddr,srcIpAddr))
    {
      uint32 destIpAddr;
      TCPIP_IPV4_ARP_DEST_IPADDR_GET(dataPtr,destIpAddr);

#if (TCPIP_ARP_DAD_ENABLED == STD_ON)
      if(!TcpIp_IpV4_arpProbeResponseDetection(ctrlIdx,srcPhysAddr,(uint8)TCPIP_IPV4_ARP_OPCODE_GET(dataPtr),srcIpAddr,destIpAddr))
#endif /* (TCPIP_ARP_DAD_ENABLED == STD_ON) */
      {

        /* remote address shall not be a multicast or limited broadcast */
        if((!TCPIP_IPV4_IS_MULTICAST_ADDR(srcIpAddr)) &&
           (!TCPIP_IPV4_IS_LIMITED_BROADCAST_ADDR(srcIpAddr))
          )
        {
          /* check if destination IP of ARP is our address */
          const TcpIp_LocalAddrIdType localAddrId =
          TcpIp_IpV4_getLocalAddrId_byIpV4(destIpAddr,ctrlIdx);

          /* local address must be unicast or not addressed to us */
          if(TcpIp_Arp_LocalAddrIsUnicast(localAddrId))
          {
            boolean arp_reply_transmit_needed =
              TcpIp_IpV4_ArpCacheUpdate(ctrlIdx, localAddrId, srcIpAddr, srcPhysAddr, physAddrPtr);
            RxRetCode = TCPIP_IP_RX_OK;

            /* if we received an ARP request, transmit the reply */
            if( (arp_reply_transmit_needed == TRUE) &&
                (localAddrId != TCPIP_LOCALADDRID_INVALID) &&
                (TCPIP_IPV4_ARP_OPCODE_GET(dataPtr) == TCPIP_IPV4_ARP_OPCODE_REQUEST) )
            {
              /* this is an ARP request - we must send a reply */
              if(TcpIp_IpAddrM_lockIfOnlineShutdown(localAddrId))
              {
                TcpIp_IpV4_ArpTransmit
                (
                  srcIpAddr,
                  destIpAddr,
                  physAddrPtr,
                  srcPhysAddr,
                  TCPIP_IPV4_ARP_OPCODE_REPLY,
                  ctrlIdx
                );
                TcpIp_IpAddrM_unlockIf(localAddrId);
              }
            }
          }
        }
      }
    }
  }
  else
  {
    /* header check failed */
    RxRetCode = TCPIP_IP_RX_INVALID_HEADER;
  }

  DBG_TCPIP_IPV4_RXINDICATIONARP_EXIT(RxRetCode,ctrlIdx,physAddrPtr,dataPtr,lenByte);

  return RxRetCode;
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE ) TcpIp_IpV4_ArpLookupAndLock
(
  uint32 ipAddr,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrHandlePtr,
  uint8 ctrlIdx,
  boolean insert
)
{
  TcpIp_ReturnType result = TCPIP_E_PHYS_ADDR_MISS;
#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
  uint8 oldEthAddr[TCPIP_IPV4_ARP_HADDR_LENGTH(ETH)];
  uint32 oldIpAddr = 0U;
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
  uint16 entryIdx;
  boolean replaceEntry = FALSE;

  /* !LINKSTO TcpIp.Design.IpV4.EA1.provideTxBuffer,1 */
  /* !LINKSTO TcpIp.Design.IpV4.EA1.getRemotePhysAddr,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();

  TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

  /* find and update an entry if it exists */
  entryIdx = TcpIp_IpV4_ArpGetExistingEntry(ipAddr, ctrlIdx);

  if((entryIdx == TCPIP_ARP_TABLESIZE_MAX) && (insert == TRUE))
  {
    /* add a new entry if none exists */
    entryIdx = TcpIp_IpV4_ArpGetEntry(ipAddr, ctrlIdx, &replaceEntry);
  }

  if(entryIdx != TCPIP_ARP_TABLESIZE_MAX)
  {
    CONSTP2VAR(TcpIp_IpV4_ArpEntryType,AUTOMATIC,TCPIP_VAR_CLEARED) arpEntryPtr =
      &TcpIp_IpV4_ArpState[ctrlIdx].arpTable[entryIdx];

#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
    if(replaceEntry == TRUE)
    {
      /* copy old ARP entry data to temporary storage for callout */
      oldIpAddr = arpEntryPtr->remoteAddr.InetAddr[0U];
      TcpIp_EthAddrCpy(oldEthAddr,arpEntryPtr->remoteAddr.PhysAddr);
    }
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */

    switch(arpEntryPtr->remoteAddr.State)
    {
      case TCPIP_PHYS_ADDR_ENTRY_STATE_INCOMPLETE:
        /* address resolution is still ongoing */
        result = TCPIP_E_NOT_OK;
        break;
      case TCPIP_PHYS_ADDR_ENTRY_STATE_REACHABLE: /* intended fall through */
      case TCPIP_PHYS_ADDR_ENTRY_STATE_STATIC:
        /* save index as ARP table handle */
        *remoteAddrHandlePtr = entryIdx;
        /* lock this entry */
        arpEntryPtr->lock++;
        result = TCPIP_OK;
        break;
      case TCPIP_PHYS_ADDR_ENTRY_STATE_FREE:
        if(TcpIp_PBcfgPtr->ipV4Config.arpRequestTimeout != 0U)
        {
          arpEntryPtr->remoteAddr.InetAddr[0U] = ipAddr;
          arpEntryPtr->remoteAddr.State = TCPIP_PHYS_ADDR_ENTRY_STATE_INCOMPLETE;
          arpEntryPtr->stateTime =
              TcpIp_IpV4_ArpCounter +
              (TcpIp_PBcfgPtr->ipV4Config.arpRequestTimeout);
        }
        break;
      /* CHECK: NOPARSE */
      default:
        /* should not happen */
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break;
      /* CHECK: PARSE */
    }
  }

  /* !LINKSTO TcpIp.Design.IpV4.EA1.provideTxBuffer,1 */
  /* !LINKSTO TcpIp.Design.IpV4.EA1.getRemotePhysAddr,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();

#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
  if(replaceEntry == TRUE)
  {
    /* callout for invalidated old ARP entry */
    TcpIp_ArpChangeFct(ctrlIdx,oldIpAddr,oldEthAddr,FALSE);
  }
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */

  return result;
}



TS_MOD_PRIV_DEFN FUNC_P2VAR(uint8, TCPIP_APPL_DATA, TCPIP_CODE ) TcpIp_IpV4_Arp_getPhysAddrPtr
(
  uint16 remoteAddrHandle,
  uint8 ctrlIdx
)
{
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) addr =
    TcpIp_IpV4_ArpState[ctrlIdx].arpTable[remoteAddrHandle].remoteAddr.PhysAddr;
  DBG_TCPIP_IPV4_ARP_GETPHYSADDRPTR_ENTRY(remoteAddrHandle,ctrlIdx);
  DBG_TCPIP_IPV4_ARP_GETPHYSADDRPTR_EXIT(addr,remoteAddrHandle,ctrlIdx);
  return addr;
}

TS_MOD_PRIV_DEFN FUNC( void, TCPIP_CODE ) TcpIp_IpV4_ArpUnlock
(
  uint16 remoteAddrHandle,
  uint8 ctrlIdx
)
{
  TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);
  TCPIP_PRECONDITION_ASSERT(remoteAddrHandle < TCPIP_ARP_TABLESIZE_MAX, TCPIP_INTERNAL_API_ID);
  {
    CONSTP2VAR(TcpIp_IpV4_ArpEntryType,AUTOMATIC,TCPIP_VAR_CLEARED) arpEntryPtr =
      &TcpIp_IpV4_ArpState[ctrlIdx].arpTable[remoteAddrHandle];
    /* unlock ARP table entry */

    /* !LINKSTO TcpIp.Design.IpV4.EA1.transmit,1 */
    /* !LINKSTO TcpIp.Design.IpV4.EA1.getRemotePhysAddr,1 */
    SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();
    arpEntryPtr->lock--;
    /* !LINKSTO TcpIp.Design.IpV4.EA1.transmit,1 */
    /* !LINKSTO TcpIp.Design.IpV4.EA1.getRemotePhysAddr,1 */
    SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();
  }
}

TS_MOD_PRIV_DEFN FUNC( void, TCPIP_CODE ) TcpIp_IpV4_ArpUpdate
(
  uint32 ipAddr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr,
  uint8 ctrlIdx,
  boolean insert
)
{
  uint16 entryIdx;
  boolean replaceEntry = FALSE;
  boolean ethAddrChanged = FALSE;
#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
  uint8 oldEthAddr[TCPIP_IPV4_ARP_HADDR_LENGTH(ETH)];
  uint32 oldIpAddr = 0U;
  boolean valid = FALSE;
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */

  TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

  /* !LINKSTO TcpIp.Design.IpV4.EA1.rxIndicationIp,1 */
  /* !LINKSTO TcpIp.Design.IpV4.EA1.rxIndicationArp,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();

  /* find and update an entry if it exists */
  entryIdx = TcpIp_IpV4_ArpGetExistingEntry(ipAddr, ctrlIdx);

  if((entryIdx == TCPIP_ARP_TABLESIZE_MAX) && (insert == TRUE))
  {
    /* add a new entry if none exists */
    entryIdx = TcpIp_IpV4_ArpGetEntry(ipAddr, ctrlIdx, &replaceEntry);
  }

  TCPIP_PRECONDITION_ASSERT(entryIdx <= TCPIP_ARP_TABLESIZE_MAX, TCPIP_INTERNAL_API_ID);
  if(entryIdx != TCPIP_ARP_TABLESIZE_MAX)
  {
    CONSTP2VAR(TcpIp_IpV4_ArpEntryType,AUTOMATIC,TCPIP_VAR_CLEARED) arpEntryPtr =
      &TcpIp_IpV4_ArpState[ctrlIdx].arpTable[entryIdx];

#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
    if(replaceEntry == TRUE)
    {
      /* copy old ARP entry data to temporary storage for callout */
      oldIpAddr = arpEntryPtr->remoteAddr.InetAddr[0U];
      TcpIp_EthAddrCpy(oldEthAddr,arpEntryPtr->remoteAddr.PhysAddr);
    }
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */

    switch(arpEntryPtr->remoteAddr.State)
    {
      case TCPIP_PHYS_ADDR_ENTRY_STATE_STATIC:
        /* do not update */
        break;
      case TCPIP_PHYS_ADDR_ENTRY_STATE_REACHABLE:
      {
        /* check if physical address has changed */
        ethAddrChanged = TcpIp_EthAddrCmp(arpEntryPtr->remoteAddr.PhysAddr,physAddrPtr);

        if((ethAddrChanged == FALSE) || (arpEntryPtr->lock == 0))
        {
          /* update MAC and alive time */
          TcpIp_EthAddrCpy(arpEntryPtr->remoteAddr.PhysAddr,physAddrPtr);
          arpEntryPtr->stateTime =
            TcpIp_IpV4_ArpCounter +
            (TcpIp_PBcfgPtr->ipV4Config.arpTimeout);
#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
          valid = TRUE;
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
        }
        else
        {
          /* if the MAC has changed and the entry is locked - then this entry is
           * invalidated by clearing the alive time, but we do not replace the
           * entry as it is still in use */
          arpEntryPtr->remoteAddr.State = TCPIP_PHYS_ADDR_ENTRY_STATE_FREE;
#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
          valid = FALSE;
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
        }
        break;
      }
      case TCPIP_PHYS_ADDR_ENTRY_STATE_FREE:
      case TCPIP_PHYS_ADDR_ENTRY_STATE_INCOMPLETE:
        if(TcpIp_PBcfgPtr->ipV4Config.arpTimeout == 0U)
        {
          TcpIp_IpV4_Arp_entry_STATIC(arpEntryPtr, physAddrPtr, ipAddr);
        }
        else
        {
          TcpIp_IpV4_Arp_entry_REACHABLE(arpEntryPtr, physAddrPtr, ipAddr);
        }
#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
        ethAddrChanged = TRUE;
        valid = TRUE;
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
        break;
      /* CHECK: NOPARSE */
      default:
        /* should not happen */
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break;
      /* CHECK: PARSE */
    }
  }

  /* !LINKSTO TcpIp.Design.IpV4.EA1.rxIndicationIp,1 */
  /* !LINKSTO TcpIp.Design.IpV4.EA1.rxIndicationArp,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();

#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
  if(replaceEntry == TRUE)
  {
    /* callout for invalidated old ARP entry */
    TcpIp_ArpChangeFct(ctrlIdx,oldIpAddr,oldEthAddr,FALSE);
  }

  if(ethAddrChanged == TRUE)
  {
    CONSTP2VAR(TcpIp_IpV4_ArpEntryType,AUTOMATIC,TCPIP_VAR_CLEARED) arpEntryPtr =
      &TcpIp_IpV4_ArpState[ctrlIdx].arpTable[entryIdx];

    /* if physical address has been changed, indicate this to configurable callout */
    /* indicate ARP table change to upper layer */
    TcpIp_ArpChangeFct(ctrlIdx,ipAddr,arpEntryPtr->remoteAddr.PhysAddr, valid);
  }
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
}


TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE)  TcpIp_IpV4_ArpTransmitRequest
(
  uint32 remoteIpAddr,
  uint32 localIpAddr,
  uint8 ctrlIdx
)
{
  TcpIp_IpV4_ArpTransmit
    (
      remoteIpAddr,
      localIpAddr,
      TcpIp_IpV4_EthUnknownAddress,
      TcpIp_IpV4_EthUnknownAddress,
      TCPIP_IPV4_ARP_OPCODE_REQUEST,ctrlIdx
    );
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_ArpTransmitAnnounce
(
  uint32 LocalIpAddr,
  uint8 ctrlIdx
)
{
  TcpIp_IpV4_ArpTransmit
    (LocalIpAddr,LocalIpAddr,TcpIp_IpV4_EthUnknownAddress, TcpIp_IpV4_EthUnknownAddress,TCPIP_IPV4_ARP_OPCODE_REQUEST,ctrlIdx);
}

#if (TCPIP_ARP_DAD_ENABLED == STD_ON)
TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_ArpProbe
(
  TcpIp_IpV4_ArpProbeResult_FctPtrType arpProbeResultFctPtr,
  uint32 ipAddress,
  uint8 numProbes,
  uint8 lastProbeWait,
  boolean randomInterval,
  TcpIp_LocalAddrIdType localAddrId,
  uint8 method,
  uint16 probe_delay
)
{
  const uint16 arpDadIdx = TcpIp_IpV4_ArpDad_getCacheIdx(localAddrId,method);
  DBG_TCPIP_IPV4_ARPPROBE_ENTRY
    (arpProbeResultFctPtr,ipAddress,numProbes,lastProbeWait,randomInterval,localAddrId,method,probe_delay);
  TCPIP_PRECONDITION_ASSERT(arpDadIdx < TCPIP_IPV4_ARP_DAD_NUM, TCPIP_INTERNAL_API_ID);
  {
    CONSTP2VAR(TcpIp_IpV4_ArpDadStateType,AUTOMATIC,TCPIP_VAR_CLEARED) arpDadStatePtr =
      &TcpIp_IpV4_ArpDadState[arpDadIdx];

    /* either set or clear the address */
    arpDadStatePtr->ipProbe = ipAddress;

    if(ipAddress != TCPIP_IPV4_ADDR_INVALID_VALUE)
    {
      arpDadStatePtr->countProbe = numProbes;
      arpDadStatePtr->randomProbeInterval = randomInterval;
      arpDadStatePtr->probeResultFctPtr = arpProbeResultFctPtr;
      arpDadStatePtr->lastProbeWait = lastProbeWait;
      arpDadStatePtr->probeTime = probe_delay; /* start probing after a delay */
      if(arpDadStatePtr->probeTime == 0U)
      {
        arpDadStatePtr->probeTime = 1U; /* start probing in next tick */
      }
    }
    else
    {
      arpDadStatePtr->probeTime = 0U; /* stop probing */
    }
  }
  DBG_TCPIP_IPV4_ARPPROBE_EXIT
    (arpProbeResultFctPtr,ipAddress,numProbes,lastProbeWait,randomInterval,localAddrId,method,probe_delay);
}
#endif /* (TCPIP_ARP_DAD_ENABLED == STD_ON) */



TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_ArpAnnounce
(
  uint32 ipAddress,
  uint8 numAnnounce,
  uint8 delay,
  uint8 ctrlIdx,
  uint8 method
)
{
  DBG_TCPIP_IPV4_ARPANNOUNCE_ENTRY(ipAddress,numAnnounce,delay,ctrlIdx,method);
  TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);
  {
    CONSTP2VAR(TcpIp_IpV4_ArpStateType,AUTOMATIC,TCPIP_VAR_CLEARED) arpStatePtr =
      &TcpIp_IpV4_ArpState[ctrlIdx];


    arpStatePtr->ipAnnounce = ipAddress;
    arpStatePtr->countAnnounce = numAnnounce;
    arpStatePtr->announceInterval = delay;
    if(method == TCPIP_IPADDRM_ASSIGN_DHCPV4)
    {
      arpStatePtr->announceType = TCPIP_IPV4_ARP_OPCODE_REPLY;
    }
    else
    {
      arpStatePtr->announceType = TCPIP_IPV4_ARP_OPCODE_REQUEST;
    }


    if (numAnnounce > 0U)
    {
      arpStatePtr->announceTime = 1U; /* start announcing with the next tick */
    }
    else
    {
      arpStatePtr->announceTime = 0U; /* stop announcing immediately */
    }
  }
  DBG_TCPIP_IPV4_ARPANNOUNCE_EXIT(ipAddress,numAnnounce,delay,ctrlIdx,method);
}
TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_IpV4_Arp_getRemotePhysAddr
(
  uint8 CtrlIdx,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) IpRemoteAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr,
  boolean initRes
)
{
  TcpIp_ReturnType retVal = TCPIP_E_NOT_OK;
  /* Deviation MISRAC2012-3 */
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_CONST) IpAddrPtr =
      ((P2CONST(TcpIp_SockAddrInetType, AUTOMATIC, TCPIP_APPL_CONST)) IpRemoteAddrPtr)->addr;

  DBG_TCPIP_IPV4_ARP_GETREMOTEPHYSADDR_ENTRY(CtrlIdx,IpRemoteAddrPtr,PhysAddrPtr,initRes);
  {
    /* get the unicast address of the controller */
    const TcpIp_LocalAddrIdType localAddrId =
      TcpIp_IpV4_getTxLocalAddrId_byCtrlIdx(CtrlIdx, NULL_PTR);

    /* verify that unicast address is assigned */
    if(TcpIp_IpAddrM_lockIfOnline(localAddrId))
    {
      TcpIp_IpAddrConstPtr nextHopIpPtr;
      TcpIp_IpAddrConstPtr networkMaskPtr;
      TcpIp_IpAddrConstPtr localIpAddrPtr;

      /* get the netmask */
      TcpIp_IpAddrM_getIpConfig(localAddrId, &localIpAddrPtr, &nextHopIpPtr, &networkMaskPtr);

      {
        const uint32 localNetworkId = (*localIpAddrPtr)&(*networkMaskPtr);
        const uint32 destinationNetworkId = (*IpAddrPtr)&(*networkMaskPtr);

        /* check if destination is in the same network */
        if(localNetworkId == destinationNetworkId)
        {
          uint16 remoteAddrHandle = 0U;

          /* check for remote ip address in the ARP cache */
          retVal = TcpIp_IpV4_ArpLookupAndLock(*IpAddrPtr, &remoteAddrHandle, CtrlIdx, initRes);

          if(retVal == TCPIP_OK)
          {
            /* retrieve physical address from arp table and copy it to PhysAddrPtr */
            P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysRemoteAddrPtr =
              TcpIp_IpV4_Arp_getPhysAddrPtr(remoteAddrHandle, CtrlIdx);

            TcpIp_EthAddrCpy(PhysAddrPtr, PhysRemoteAddrPtr);

            TcpIp_IpV4_ArpUnlock(remoteAddrHandle,CtrlIdx);
          }
          else if (retVal == TCPIP_E_PHYS_ADDR_MISS)
          {
            /* remote address unknown, send ARP request if initRes is TRUE */
            if(initRes)
            {
              TcpIp_IpV4_ArpTransmitRequest(*IpAddrPtr, *localIpAddrPtr, CtrlIdx);
            }
          }
          else
          {
            retVal = TCPIP_E_PHYS_ADDR_MISS;
          }
        }
      }

      TcpIp_IpAddrM_unlockIf(localAddrId);
    }
  }
  DBG_TCPIP_IPV4_ARP_GETREMOTEPHYSADDR_EXIT(retVal,CtrlIdx,IpRemoteAddrPtr,PhysAddrPtr,initRes);

  return retVal;
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_IpV4_Arp_setRemotePhysAddr
(
  uint8 CtrlIdx,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CONST) IpRemoteAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr,
  uint8 State
)
{
  TcpIp_ReturnType retval = TCPIP_E_NOT_OK;
  /* Deviation MISRAC2012-3 */
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_CONST) IpAddrPtr =
      ((P2CONST(TcpIp_SockAddrInetType, AUTOMATIC, TCPIP_APPL_CONST)) IpRemoteAddrPtr)->addr;

  DBG_TCPIP_IPV4_ARP_SETREMOTEPHYSADDR_ENTRY(CtrlIdx,IpRemoteAddrPtr,PhysAddrPtr,State);

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
  if(!TCPIP_IPV4_ARP_STATE_VALID(State))
  {
    /* invalid request */
    retval = TCPIP_E_NOT_PERMITTED;
  }
  else if(TCPIP_IPV4_ADDR_IS_NOT_VALID_UNICAST(IpAddrPtr))
  {
    retval = TCPIP_E_NOT_PERMITTED;
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
    switch(State)
    {
      case TCPIP_PHYS_ADDR_ENTRY_STATE_STATIC:     /* intended fall through */
      case TCPIP_PHYS_ADDR_ENTRY_STATE_REACHABLE:
        retval = TcpIp_IpV4_ArpSetAddr(CtrlIdx, *IpAddrPtr, PhysAddrPtr, State);
        break;
      case TCPIP_PHYS_ADDR_ENTRY_STATE_FREE:
        retval = TcpIp_IpV4_ArpRemove(*IpAddrPtr, CtrlIdx);
        break;
      /* CHECK: NOPARSE */
      default:
        /* should not happen */
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        retval = TCPIP_E_NOT_OK;
        break;
      /* CHECK: PARSE */
    }
  }

  DBG_TCPIP_IPV4_ARP_SETREMOTEPHYSADDR_EXIT(retval,CtrlIdx,IpRemoteAddrPtr,PhysAddrPtr,State);

  return retval;
}

#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)
TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_Arp_GetAndResetReplacedArpEntryCount
(
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) MeasurementDataPtr
)
{
  if(MeasurementDataPtr != NULL_PTR)
  {
    /* !LINKSTO TcpIp.Design.IpV4.Atomic.ReplacedArpEntryCounter,1 */
    TS_AtomicAssign32(*MeasurementDataPtr, TcpIp_IpV4_Arp_ReplacedArpEntryCounter);
  }
  if(MeasurementResetNeeded)
  {
    /* !LINKSTO TcpIp.Design.IpV4.Atomic.ReplacedArpEntryCounter,1 */
    TS_AtomicAssign32(TcpIp_IpV4_Arp_ReplacedArpEntryCounter, 0U);
  }
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_Arp_GetAndResetDroppedLockedArpEntryCount
(
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) MeasurementDataPtr
)
{
  if(MeasurementDataPtr != NULL_PTR)
  {
    /* !LINKSTO TcpIp.Design.IpV4.Atomic.DroppedLockedArpEntryCounter,1 */
    TS_AtomicAssign32(*MeasurementDataPtr, TcpIp_IpV4_Arp_DroppedLockedArpEntryCounter);
  }
  if(MeasurementResetNeeded)
  {
    /* !LINKSTO TcpIp.Design.IpV4.Atomic.DroppedLockedArpEntryCounter,1 */
    TS_AtomicAssign32(TcpIp_IpV4_Arp_DroppedLockedArpEntryCounter, 0U);
  }
}
#endif /* TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */


/*==================[internal function definitions]=========================*/

STATIC FUNC( boolean, TCPIP_CODE ) TcpIp_IpV4_ArpCacheUpdate
(
  uint8 ctrlIdx,
  TcpIp_LocalAddrIdType localAddrId,
  uint32 remoteIpAddr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) remotePhysAddr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr
)
{
  boolean arp_reply_transmit_allowed = FALSE;

#if (TCPIP_SECURITY_MODE == STD_OFF)
  TS_PARAM_UNUSED(physAddrPtr);
#endif /* (TCPIP_SECURITY_MODE == STD_OFF) */

  /* if ARP is a probe do not add in cache */
  if(remoteIpAddr != TCPIP_IPV4_ADDR_INVALID_VALUE)
  {
    if(localAddrId != TCPIP_LOCALADDRID_INVALID)
    {
      /* only add addresses which are in the same subnet */
      if(TcpIp_Arp_isSubnetComm(remoteIpAddr, localAddrId))
      {
#if (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL)
        /* check if TcpIp is allowed to communicate with the remote host */
        if(TcpIp_IpSecDb_checkSecurityPolicyAddr(localAddrId, &remoteIpAddr, physAddrPtr) == E_OK)
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL) */
        {
          /* this ARP message is addressed to me - insert into ARP table */
          TcpIp_IpV4_ArpUpdate(remoteIpAddr,remotePhysAddr,ctrlIdx, TRUE);

          arp_reply_transmit_allowed = TRUE;
        }
      }
    }
#if (TCPIP_SECURITY_MODE == STD_OFF)
    else
    {
      /* this ARP message is not addressed to me - perform an ARP entry update (if existing) */
      TcpIp_IpV4_ArpUpdate(remoteIpAddr,remotePhysAddr,ctrlIdx, FALSE);
    }
#endif /* (TCPIP_SECURITY_MODE == STD_OFF) */
  }
  else
  {
    /* reply to arp probe */
    arp_reply_transmit_allowed = TRUE;
  }

  return arp_reply_transmit_allowed;
}

STATIC FUNC( boolean, TCPIP_CODE ) TcpIp_IpV4_ArpCheckHeader
(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 lenByte
)
{
  boolean result = FALSE;

  if(TCPIP_IPV4_ARP_HEADER_LENGTH_CHECK(ETH,IP,lenByte))
  {
    /* check for matching header attributes */
    if(
        ((TCPIP_IPV4_ARP_HWTYPE_IS(ETH,dataPtr) &&
          TCPIP_IPV4_ARP_PROTO_IS(IP,dataPtr)) &&
          TCPIP_IPV4_ARP_CHECK_HADDR_LENGTH(ETH,dataPtr)) &&
          TCPIP_IPV4_ARP_CHECK_PADDR_LENGTH(IP,dataPtr)
      )
    {
      result = TRUE;
    }
  }

  return result;
}

STATIC FUNC( uint16, TCPIP_CODE ) TcpIp_IpV4_ArpGetEntry
(
  uint32 ipAddr,
  uint8 ctrlIdx,
  P2VAR(boolean, AUTOMATIC, TCPIP_APPL_DATA) replace
)
{
  uint16_least i;
  uint16_least entry = TCPIP_ARP_TABLESIZE_MAX;
  uint16_least replaceEntry = TCPIP_ARP_TABLESIZE_MAX;
  uint16_least freeEntry = TCPIP_ARP_TABLESIZE_MAX;
  uint16_least existingEntry = TCPIP_ARP_TABLESIZE_MAX;
  uint32 replaceCandidateTime = 0xFFFFFFFFU;

  /* loop all entries of ARP table for specific interface */
  for(i = 0U; i < TCPIP_ARP_TABLESIZE_MAX; i++)
  {
    CONSTP2VAR(TcpIp_IpV4_ArpEntryType,AUTOMATIC,TCPIP_VAR_CLEARED) arpEntryPtr =
      &TcpIp_IpV4_ArpState[ctrlIdx].arpTable[i];

    /* only search in entries which are in use */
    if(arpEntryPtr->remoteAddr.State != TCPIP_PHYS_ADDR_ENTRY_STATE_FREE)
    {
      /* verify if ip address match with ip address of entry */
      if(arpEntryPtr->remoteAddr.InetAddr[0U] == ipAddr)
      {
        existingEntry = i;
        break;
      }
    }

    /* beside the existing entry, try to find a free entry */
    if(freeEntry == TCPIP_ARP_TABLESIZE_MAX)
    {
      /* only considering unlocked entries as free entries */
      if(arpEntryPtr->lock == 0U)
      {
        if(arpEntryPtr->remoteAddr.State == TCPIP_PHYS_ADDR_ENTRY_STATE_FREE)
        {
          /* yeah it is available - stop further searching */
          freeEntry = i;
        }
        else if(arpEntryPtr->remoteAddr.State == TCPIP_PHYS_ADDR_ENTRY_STATE_REACHABLE)
        {
          const uint32 age =
            (uint32)((TcpIp_IpV4_ArpCounter +
                     (TcpIp_PBcfgPtr->ipV4Config.arpTimeout)) -
                     arpEntryPtr->stateTime);

          /* until we found a free entry, we keep track of the oldest ARP entry */
          if((age > replaceCandidateTime) || (replaceEntry == TCPIP_ARP_TABLESIZE_MAX))
          {
            replaceEntry = i;
            replaceCandidateTime = age;
          }
        }
        else
        {
          ; /* no matching or older entry found */
        }
      }
    }
  }

  if(existingEntry != TCPIP_ARP_TABLESIZE_MAX)
  {
    /* there is an existing matching ARP entry */
    entry = existingEntry;
  }
  else if(freeEntry != TCPIP_ARP_TABLESIZE_MAX)
  {
    /* there is an empty ARP entry */
    entry = freeEntry;
  }
  else if(replaceEntry != TCPIP_ARP_TABLESIZE_MAX)
  {
    TcpIp_IpV4_ArpState[ctrlIdx].arpTable[replaceEntry].remoteAddr.State =
        TCPIP_PHYS_ADDR_ENTRY_STATE_FREE;
    entry = replaceEntry;
    *replace = TRUE;
#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)
    /* increment counter of the dropped frames caused by replacing oldest Arp entries */
    /* !LINKSTO TcpIp.Design.IpV4.EA6.rxIndication,1 */
    /* !LINKSTO TcpIp.Design.IpV4.EA6.provideTxBuffer,1 */
    SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_6();
    if(TcpIp_IpV4_Arp_ReplacedArpEntryCounter < 0xFFFFFFFFU)
    {
      TS_AtomicAssign32(TcpIp_IpV4_Arp_ReplacedArpEntryCounter, (TcpIp_IpV4_Arp_ReplacedArpEntryCounter+1U));
    }
    /* !LINKSTO TcpIp.Design.IpV4.EA6.rxIndication,1 */
    /* !LINKSTO TcpIp.Design.IpV4.EA6.provideTxBuffer,1 */
    SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_6();
#endif /* TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */
  }
  else
  {
    /* no (unlocked) existing entry, no free entry, all other entries are locked
     * or entries are either in state STATIC or INCOMPLETE
     * quite unrealistic but in this case we can't update the ARP-table */
#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)
    /* increment counter of the dropped frames caused by locked all existing Arp entries */
    /* !LINKSTO TcpIp.Design.IpV4.EA6.rxIndication,1 */
    /* !LINKSTO TcpIp.Design.IpV4.EA6.provideTxBuffer,1 */
    /* !LINKSTO TcpIp.Design.IpV4.EA6.setRemotePhysAddr,1 */
    SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_6();
    if(TcpIp_IpV4_Arp_DroppedLockedArpEntryCounter < 0xFFFFFFFFU)
    {
      TS_AtomicAssign32(TcpIp_IpV4_Arp_DroppedLockedArpEntryCounter, (TcpIp_IpV4_Arp_DroppedLockedArpEntryCounter+1U));
    }
    /* !LINKSTO TcpIp.Design.IpV4.EA6.rxIndication,1 */
    /* !LINKSTO TcpIp.Design.IpV4.EA6.provideTxBuffer,1 */
    /* !LINKSTO TcpIp.Design.IpV4.EA6.setRemotePhysAddr,1 */
    SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_6();
#endif /* TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */
  }

  return (uint16) entry;
}

STATIC FUNC( uint16, TCPIP_CODE ) TcpIp_IpV4_ArpGetExistingEntry
(
  uint32 ipAddr,
  uint8 ctrlIdx
)
{
  uint16_least i;
  uint16_least existingEntry = TCPIP_ARP_TABLESIZE_MAX;

  /* loop all entries of ARP table for specific interface */
  for(i = 0U; i < TCPIP_ARP_TABLESIZE_MAX; i++)
  {
    CONSTP2VAR(TcpIp_IpV4_ArpEntryType,AUTOMATIC,TCPIP_VAR_CLEARED) arpEntryPtr =
      &TcpIp_IpV4_ArpState[ctrlIdx].arpTable[i];

    /* only search in entries which are in use */
    if(arpEntryPtr->remoteAddr.State != TCPIP_PHYS_ADDR_ENTRY_STATE_FREE)
    {
      /* verify if ip address match with ip address of entry */
      if(arpEntryPtr->remoteAddr.InetAddr[0U] == ipAddr)
      {
        existingEntry = i;
        break;
      }
    }
  }

  return (uint16) existingEntry;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_Arp_entry_REACHABLE
(
  P2VAR(TcpIp_IpV4_ArpEntryType, AUTOMATIC, TCPIP_APPL_DATA) arpEntryPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr,
  uint32 ipAddr
)
{
  /* update MAC and alive time */
  TcpIp_EthAddrCpy(arpEntryPtr->remoteAddr.PhysAddr,physAddrPtr);
  arpEntryPtr->remoteAddr.InetAddr[0] = ipAddr;
  arpEntryPtr->stateTime =
    TcpIp_IpV4_ArpCounter + (TcpIp_PBcfgPtr->ipV4Config.arpTimeout);
  arpEntryPtr->remoteAddr.State = TCPIP_PHYS_ADDR_ENTRY_STATE_REACHABLE;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_Arp_entry_STATIC
(
  P2VAR(TcpIp_IpV4_ArpEntryType, AUTOMATIC, TCPIP_APPL_DATA) arpEntryPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr,
  uint32 ipAddr
)
{
  /* update MAC and alive time */
  TcpIp_EthAddrCpy(arpEntryPtr->remoteAddr.PhysAddr,physAddrPtr);
  arpEntryPtr->remoteAddr.InetAddr[0] = ipAddr;
  arpEntryPtr->remoteAddr.State = TCPIP_PHYS_ADDR_ENTRY_STATE_STATIC;
}


STATIC FUNC(void, TCPIP_CODE) TcpIp_IpV4_ArpTransmit
  (
    uint32 remoteIpAddr,
    uint32 localIpAddr,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) remotePhysAddrPtr,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) targetPhysAddrPtr,
    uint16 opcode,
    uint8 ctrlIdx
  )
{
  const uint8 vlanPriority =
    (TCPIP_CFG(ctrl,ctrlIdx)).defaultVlanPrio;
  uint16 LenByte = TCPIP_IPV4_ARP_HEADER_LENGTH(ETH,IP);
  P2VAR(uint8,AUTOMATIC,TCPIP_APPL_DATA) bufPtr;
  Eth_BufIdxType BufIdx;

  /* get an ethernet buffer for transmission */
  if(EthIf_ProvideTxBuffer((TCPIP_CFG(ctrl,ctrlIdx)).ethIfCtrlIdx,TCPIP_IP_FRAMETYPE_ARP,vlanPriority,&BufIdx,&bufPtr,&LenByte) == BUFREQ_OK)
  {
    P2VAR(uint8,AUTOMATIC,TCPIP_APPL_DATA) refinedRemotePhysAddrPtr;
    P2CONST(uint8,AUTOMATIC,TCPIP_APPL_DATA) srcAddrPtr = (P2CONST(uint8,AUTOMATIC,TCPIP_APPL_DATA))&localIpAddr;

    uint8 TcpIp_EthBroadcastAddress[TCPIP_ETH_ADDR_LENGTH] =
            {0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU };

    /* build ARP header */
    TCPIP_IPV4_ARP_HTYPE_SET(TCPIP_IPV4_ARP_HTYPE_ETH, bufPtr);
    TCPIP_IPV4_ARP_PROTO_SET(TCPIP_IPV4_ARP_PROTO_IP, bufPtr);
    TCPIP_IPV4_ARP_HADDR_LENGTH_SET(ETH,bufPtr);
    TCPIP_IPV4_ARP_PADDR_LENGTH_SET(IP,bufPtr);
    TCPIP_IPV4_ARP_OPCODE_SET(opcode,bufPtr);
    EthIf_GetPhysAddr((TCPIP_CFG(ctrl,ctrlIdx)).ethIfCtrlIdx,TCPIP_IPV4_ARP_LOCAL_HADDR_GET_PTR(ETH,IP,bufPtr));
    TCPIP_IPV4_ARP_SRC_IPADDR_SET(localIpAddr,bufPtr);
    TcpIp_EthAddrCpy(TCPIP_IPV4_ARP_REMOTE_HADDR_GET_PTR(ETH,IP,bufPtr),targetPhysAddrPtr);
    TCPIP_IPV4_ARP_DEST_IPADDR_SET(remoteIpAddr,bufPtr);

    if((remotePhysAddrPtr == TcpIp_IpV4_EthUnknownAddress) ||
       TCPIP_IPV4_ARP_ADDR_IS_LINKLOCAL(srcAddrPtr)
      )
    {
      /* if the remote physical address is unknown, we transmit to physical broadcast address */
      refinedRemotePhysAddrPtr = TcpIp_EthBroadcastAddress;
    }
    else
    {
      /* if the remote physical address is known, we transmit to the known physical address */
      refinedRemotePhysAddrPtr = remotePhysAddrPtr;
    }
    /* transmit ARP message - don't care about result - upper layers must retry in case ARP entry is missing */
    /* !LINKSTO TcpIp.ASR40.SWS_TCPIP_00131,1 */
    (void)EthIf_Transmit
      (
        (TCPIP_CFG(ctrl,ctrlIdx)).ethIfCtrlIdx,
        BufIdx,
        TCPIP_IP_FRAMETYPE_ARP,FALSE,TCPIP_IPV4_ARP_HEADER_LENGTH(ETH,IP),
        refinedRemotePhysAddrPtr
      );
  }
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_IpV4_ArpSetAddr
(
  uint8 CtrlIdx,
  uint32 IpAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr,
  uint8 State
)
{
  TcpIp_ReturnType result = TCPIP_E_NOT_OK;
  uint16 entryIdx;
  boolean replaceEntry = FALSE;
#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
  boolean ethAddrChanged = FALSE;
  uint8 oldEthAddr[TCPIP_IPV4_ARP_HADDR_LENGTH(ETH)];
  uint32 oldIpAddr = 0U;
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */

  TCPIP_PRECONDITION_ASSERT(CtrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

  /* !LINKSTO TcpIp.Design.IpV4.EA1.setRemotePhysAddr,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();

  /* get existing or free entry */
  entryIdx = TcpIp_IpV4_ArpGetEntry(IpAddrPtr, CtrlIdx, &replaceEntry);
  TCPIP_PRECONDITION_ASSERT(entryIdx <= TCPIP_ARP_TABLESIZE_MAX, TCPIP_INTERNAL_API_ID);
  if(entryIdx != TCPIP_ARP_TABLESIZE_MAX)
  {
    CONSTP2VAR(TcpIp_IpV4_ArpEntryType,AUTOMATIC,TCPIP_VAR_CLEARED) arpEntryPtr =
      &TcpIp_IpV4_ArpState[CtrlIdx].arpTable[entryIdx];

#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
    if(replaceEntry == TRUE)
    {
      /* copy old ARP entry data to temporary storage for callout */
      oldIpAddr = arpEntryPtr->remoteAddr.InetAddr[0U];
        TcpIp_EthAddrCpy(oldEthAddr,arpEntryPtr->remoteAddr.PhysAddr);
    }
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */

    switch(arpEntryPtr->remoteAddr.State)
    {
      case TCPIP_PHYS_ADDR_ENTRY_STATE_STATIC:
      case TCPIP_PHYS_ADDR_ENTRY_STATE_REACHABLE:
      case TCPIP_PHYS_ADDR_ENTRY_STATE_FREE:
      case TCPIP_PHYS_ADDR_ENTRY_STATE_INCOMPLETE:
        if(arpEntryPtr->lock == 0U)
        {
#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
          if((arpEntryPtr->remoteAddr.State == TCPIP_PHYS_ADDR_ENTRY_STATE_REACHABLE) ||
            (arpEntryPtr->remoteAddr.State == TCPIP_PHYS_ADDR_ENTRY_STATE_STATIC)
            )
          {
            /* check if physical address has changed */
            ethAddrChanged = TcpIp_EthAddrCmp(arpEntryPtr->remoteAddr.PhysAddr,PhysAddrPtr);
          }
          else
          {
            ethAddrChanged = TRUE;
          }
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */

          arpEntryPtr->remoteAddr.InetAddr[0U] = IpAddrPtr;
          TcpIp_EthAddrCpy(arpEntryPtr->remoteAddr.PhysAddr,PhysAddrPtr);
          arpEntryPtr->remoteAddr.State = State;
          result = TCPIP_OK;
        }
        break;
      /* CHECK: NOPARSE */
      default:
        /* should not happen */
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break;
      /* CHECK: PARSE */
    }
  }

  /* !LINKSTO TcpIp.Design.IpV4.EA1.setRemotePhysAddr,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();

#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
  if(replaceEntry == TRUE)
  {
    /* callout for invalidated old ARP entry */
    TcpIp_ArpChangeFct(CtrlIdx,oldIpAddr,oldEthAddr,FALSE);
  }

  if(ethAddrChanged == TRUE)
  {
    CONSTP2VAR(TcpIp_IpV4_ArpEntryType,AUTOMATIC,TCPIP_VAR_CLEARED) arpEntryPtr =
      &TcpIp_IpV4_ArpState[CtrlIdx].arpTable[entryIdx];

    /* if physical address has been changed, indicate this to configurable callout */
    /* indicate ARP table change to upper layer */
    TcpIp_ArpChangeFct
      (CtrlIdx,arpEntryPtr->remoteAddr.InetAddr[0],arpEntryPtr->remoteAddr.PhysAddr, TRUE);
  }
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */

  return result;
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE ) TcpIp_IpV4_ArpRemove
(
  uint32 ipAddr,
  uint8 ctrlIdx
)
{
  uint16 entryIdx;
  TcpIp_ReturnType result = TCPIP_E_PHYS_ADDR_MISS;
#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
  boolean ethAddrChanged = FALSE;
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */

  TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

  /* !LINKSTO TcpIp.Design.IpV4.EA1.setRemotePhysAddr,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();

  entryIdx = TcpIp_IpV4_ArpGetExistingEntry(ipAddr, ctrlIdx);

  if(entryIdx != TCPIP_ARP_TABLESIZE_MAX)
  {
    CONSTP2VAR(TcpIp_IpV4_ArpEntryType,AUTOMATIC,TCPIP_VAR_CLEARED) arpEntryPtr =
      &TcpIp_IpV4_ArpState[ctrlIdx].arpTable[entryIdx];

    switch(arpEntryPtr->remoteAddr.State)
    {
      case TCPIP_PHYS_ADDR_ENTRY_STATE_STATIC:     /* intended fall through */
      case TCPIP_PHYS_ADDR_ENTRY_STATE_REACHABLE:
#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
        ethAddrChanged = TRUE;
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
        arpEntryPtr->remoteAddr.State = TCPIP_PHYS_ADDR_ENTRY_STATE_FREE;
        result = TCPIP_OK;
        break;
      case TCPIP_PHYS_ADDR_ENTRY_STATE_INCOMPLETE:
        arpEntryPtr->remoteAddr.State = TCPIP_PHYS_ADDR_ENTRY_STATE_FREE;
        result = TCPIP_OK;
        break;
      /* CHECK: NOPARSE */
      case TCPIP_PHYS_ADDR_ENTRY_STATE_FREE:
      default:
        /* should not happen */
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break;
      /* CHECK: PARSE */
    }
  }

  /* !LINKSTO TcpIp.Design.IpV4.EA1.setRemotePhysAddr,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();

#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
  if(ethAddrChanged == TRUE)
  {
    CONSTP2VAR(TcpIp_IpV4_ArpEntryType,AUTOMATIC,TCPIP_VAR_CLEARED) arpEntryPtr =
      &TcpIp_IpV4_ArpState[ctrlIdx].arpTable[entryIdx];

    /* if physical address has been changed, indicate this to configurable callout */
    /* indicate ARP table change to upper layer */
    TcpIp_ArpChangeFct
      (ctrlIdx,arpEntryPtr->remoteAddr.InetAddr[0],arpEntryPtr->remoteAddr.PhysAddr, FALSE);
  }
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */

  return result;
}

STATIC FUNC( void, TCPIP_CODE ) TcpIp_IpV4_ArpTableMainFunction(void)
{
  uint8_least i;
  uint16_least j;

  /* basically the decrement of aliveTime must be protected only, but in order to avoid
   * continuous locking/unlocking the whole loop is protected  with a exclusive area */
  /* !LINKSTO TcpIp.Design.IpV4.EA1.mainFunction,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();

  /* increase the seconds counter */
  TcpIp_IpV4_ArpCounter++;

  /* loop over all interface ARP tables */
  for(i = 0U; i < TCPIP_NUM_ETHIFCTRL; i++)
  {
    /* loop over all ARP table entries */
    for(j = 0U; j < TCPIP_ARP_TABLESIZE_MAX; j++)
    {
      CONSTP2VAR(TcpIp_IpV4_ArpEntryType,AUTOMATIC,TCPIP_VAR_CLEARED) arpEntryPtr =
        &TcpIp_IpV4_ArpState[i].arpTable[j];

      TcpIp_IpV4_ArpTimeout(arpEntryPtr, (uint8)i);
    }
  }
  /* !LINKSTO TcpIp.Design.IpV4.EA1.mainFunction,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();
}

STATIC FUNC( void, TCPIP_CODE ) TcpIp_IpV4_ArpTimeout
(
  P2VAR(TcpIp_IpV4_ArpEntryType, AUTOMATIC, TCPIP_APPL_DATA) arpEntryPtr,
  uint8 ctrlIdx
)
{
  switch(arpEntryPtr->remoteAddr.State)
  {
    case TCPIP_PHYS_ADDR_ENTRY_STATE_STATIC:     /* intended fall through */
    case TCPIP_PHYS_ADDR_ENTRY_STATE_FREE:
      /* ignore entry */
      break;
    case TCPIP_PHYS_ADDR_ENTRY_STATE_REACHABLE:
      if(TcpIp_IpV4_ArpCounter == arpEntryPtr->stateTime)
      {
#if (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON)
        /* copy ARP callout info to local variables to ensure data consistency */
        const uint32 ipAddr = arpEntryPtr->remoteAddr.InetAddr[0U];
        uint8 ethAddr[TCPIP_IPV4_ARP_HADDR_LENGTH(ETH)];

        TcpIp_EthAddrCpy(ethAddr,arpEntryPtr->remoteAddr.PhysAddr);

        /* !LINKSTO TcpIp.Design.IpV4.EA1.mainFunction,1 */
        SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();

        TcpIp_ArpChangeFct(ctrlIdx,ipAddr,ethAddr,FALSE);

        /* !LINKSTO TcpIp.Design.IpV4.EA1.mainFunction,1 */
        SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_1();
#else /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
  TS_PARAM_UNUSED(ctrlIdx);
#endif /* (TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON) */
        arpEntryPtr->remoteAddr.State = TCPIP_PHYS_ADDR_ENTRY_STATE_FREE;
      }
      break;
    case TCPIP_PHYS_ADDR_ENTRY_STATE_INCOMPLETE:
      if(TcpIp_IpV4_ArpCounter == arpEntryPtr->stateTime)
      {
        arpEntryPtr->remoteAddr.State = TCPIP_PHYS_ADDR_ENTRY_STATE_FREE;
      }
      break;
    /* CHECK: NOPARSE */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }
}

STATIC FUNC( void, TCPIP_CODE ) TcpIp_IpV4_ArpAnnounceMainFunction(uint16 timerTick)
{
  uint8_least i;

  if((timerTick % TCPIP_MAINFUNCTION_PRESCALER_100MS) == TCPIP_TIMER_OFFSET_100MS_ARPANNOUNCE)
  {
    /* loop over all interface ARP tables */
    for(i = 0U; i < TCPIP_NUM_ETHIFCTRL; i++)
    {
      CONSTP2VAR(TcpIp_IpV4_ArpStateType,AUTOMATIC,TCPIP_VAR_CLEARED) arpStatePtr = &TcpIp_IpV4_ArpState[i];

      if(arpStatePtr->announceTime > 0U)
      {
        arpStatePtr->announceTime--;

        if(arpStatePtr->announceTime == 0U)
        {
          CONSTP2CONST(TcpIp_ctrlIpV4ConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlPtr =
              &((TCPIP_CFG(ctrl,i)).ctrlIpV4);

          const TcpIp_LocalAddrIdType txlocalAddrId = ctrlPtr->localAddrIdTx;

          if(TcpIp_IpAddrM_lockIfOnline(txlocalAddrId))
          {
            TcpIp_IpV4_ArpTransmit
            (
              arpStatePtr->ipAnnounce,
              arpStatePtr->ipAnnounce,
              TcpIp_IpV4_EthUnknownAddress,
              TcpIp_IpV4_EthUnknownAddress,
              arpStatePtr->announceType,
              (uint8)i
            );
            TcpIp_IpAddrM_unlockIf(txlocalAddrId);
          }

          arpStatePtr->countAnnounce--;
          if(arpStatePtr->countAnnounce > 0U)
          {
            arpStatePtr->announceTime = arpStatePtr->announceInterval*TCPIP_IPV4_ARP_100MS_TO_1S_FACTOR;
          }
        }
      }
    }
  }
}



#if (TCPIP_ARP_DAD_ENABLED == STD_ON)
STATIC FUNC( void, TCPIP_CODE ) TcpIp_IpV4_ArpProbeMainFunction(uint16 timerTick)
{
  uint16_least i;

  if((timerTick % TCPIP_MAINFUNCTION_PRESCALER_100MS) == TCPIP_TIMER_OFFSET_100MS_ARPPROBE)
  {
    /* loop over all interface ARP tables */
    for(i = 0U; i < TCPIP_IPV4_ARP_DAD_NUM; i++)
    {
      CONSTP2VAR(TcpIp_IpV4_ArpDadStateType,AUTOMATIC,TCPIP_VAR_CLEARED) arpDadStatePtr = &TcpIp_IpV4_ArpDadState[i];

      if(arpDadStatePtr->probeTime > 0U)
      {
        arpDadStatePtr->probeTime--;

        if(arpDadStatePtr->probeTime == 0U)
        {
          const uint8 intCtrlIdx = TCPIP_CFG_TYPE(uint8,arpDadCtrlMapping,i);
          if(arpDadStatePtr->countProbe > 0U)
          {
            CONSTP2CONST(TcpIp_ctrlIpV4ConfigType,AUTOMATIC,TCPIP_APPL_CONST) ctrlPtr =
                &((TCPIP_CFG(ctrl,intCtrlIdx)).ctrlIpV4);

            const TcpIp_LocalAddrIdType txlocalAddrId = ctrlPtr->localAddrIdTx;

            if(TcpIp_IpAddrM_lockIfStartupOnlineShutdown(txlocalAddrId))
            {
              TcpIp_IpV4_ArpTransmit
              (
                arpDadStatePtr->ipProbe,
                TCPIP_IPV4_ADDR_ANY_VALUE,
                TcpIp_IpV4_EthUnknownAddress,
                TcpIp_IpV4_EthUnknownAddress,
                TCPIP_IPV4_ARP_OPCODE_REQUEST,
                intCtrlIdx
              );
              TcpIp_IpAddrM_unlockIf(txlocalAddrId);
            }
            arpDadStatePtr->countProbe--;

            if(arpDadStatePtr->countProbe == 0U)
            {
              arpDadStatePtr->probeTime = (uint16)arpDadStatePtr->lastProbeWait * TCPIP_IPV4_ARP_100MS_TO_1S_FACTOR;
            }
            else
            {
              arpDadStatePtr->probeTime = (arpDadStatePtr->randomProbeInterval != FALSE) ?
                                       (uint16)((1U*TCPIP_IPV4_ARP_100MS_TO_1S_FACTOR) + TcpIp_GetRandomRange(1U*TCPIP_IPV4_ARP_100MS_TO_1S_FACTOR)) :
                                       (uint16)(1U*TCPIP_IPV4_ARP_100MS_TO_1S_FACTOR);
            }
          }
          else
          {
            arpDadStatePtr->ipProbe = TCPIP_IPV4_ADDR_INVALID_VALUE;

            /* Note: remote and local address only required when conflict is detected */
            (*arpDadStatePtr->probeResultFctPtr)((uint8)intCtrlIdx, TRUE, NULL_PTR, NULL_PTR);
          }
        }
      }
    }
  }
}


STATIC FUNC(boolean, TCPIP_CODE ) TcpIp_IpV4_arpProbeResponseDetection
(
  uint8 ctrlIdx,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) remotePhysAddr,
  uint8 opcode,
  uint32 srcIpAddr,
  uint32 destIpAddr
)
{
  boolean isLoopback = FALSE;
  uint16 arpDadIdx;
  TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);
  for(arpDadIdx = 0U; arpDadIdx < TCPIP_IPV4_ARP_DAD_NUM; arpDadIdx++)
  {
    CONSTP2VAR(TcpIp_IpV4_ArpDadStateType,AUTOMATIC,TCPIP_VAR_CLEARED) arpDadStatePtr =
            &TcpIp_IpV4_ArpDadState[arpDadIdx];
    const uint32 ipProbe = arpDadStatePtr->ipProbe;

    if(ctrlIdx == TCPIP_CFG_TYPE(uint8,arpDadCtrlMapping,arpDadIdx))
    {
      /* if filtering is enabled, check for a match */
      if(ipProbe != TCPIP_IPV4_ADDR_INVALID_VALUE)
      {
        if(((opcode == TCPIP_IPV4_ARP_OPCODE_REQUEST) &&
            (srcIpAddr == 0U) &&
            (ipProbe == destIpAddr)) ||
           (ipProbe == srcIpAddr) )
        {
          uint8 localEthAddr[TCPIP_IPV4_ARP_HADDR_LENGTH(ETH)];

          /* read interface hardware address */
          EthIf_GetPhysAddr((TCPIP_CFG(ctrl,ctrlIdx)).ethIfCtrlIdx,localEthAddr);

          if(TcpIp_EthAddrCmp(remotePhysAddr,localEthAddr))
          {

            /* stop probing immediately */
            arpDadStatePtr->ipProbe = TCPIP_IPV4_ADDR_INVALID_VALUE;
            arpDadStatePtr->probeTime = 0U;

            /* we detected another node with the probed IP indicate this fact*/
            (*arpDadStatePtr->probeResultFctPtr)(ctrlIdx,FALSE,remotePhysAddr, localEthAddr);
          }
          else
          {
            isLoopback = TRUE;
          }
        }
      }
    }
  }

  return isLoopback;
}

#endif /* (TCPIP_ARP_DAD_ENABLED == STD_ON) */


#if TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON

STATIC FUNC(void, TCPIP_CODE) TcpIp_ArpChangeFct
(
  uint8 ctrlIdx,
  uint32 ipAddr,
  P2VAR(uint8,AUTOMATIC,TCPIP_APPL_DATA) physAddrPtr,
  boolean valid
)
{
  /* indicate ARP table change to upper layer */
  TcpIp_SockAddrInetType ipSockAddr;
  ipSockAddr.addr[0] = ipAddr;
  ipSockAddr.domain = TCPIP_AF_INET;

  /* Deviation MISRAC2012-3 */
  TcpIp_PhysAddrTableChgAPI
    ((TCPIP_CFG(ctrl,ctrlIdx)).ethIfCtrlIdx,(P2VAR(TcpIp_SockAddrType,AUTOMATIC,TCPIP_APPL_DATA))&ipSockAddr,physAddrPtr,valid);
}

#endif /* TCPIP_PHYSADDR_CHANGE_FCT_ENABLE == STD_ON */

STATIC FUNC(boolean, TCPIP_CODE) TcpIp_Arp_LocalAddrIsUnicast
(
  TcpIp_LocalAddrIdType localAddrId
)
{
  boolean retVal = FALSE;

  if(localAddrId != TCPIP_LOCALADDRID_INVALID)
  {
    if(TCPIP_IPADDRM_IS_ADDRTYPE((&TCPIP_CFG(localAddr,localAddrId))->addrType,UNICAST))
    {
      retVal = TRUE;
    }
  }
  else
  {
    retVal = TRUE;
  }

  return retVal;
}

STATIC FUNC(boolean, TCPIP_CODE) TcpIp_Arp_isSubnetComm
(
  uint32 remoteIpAddr,
  TcpIp_LocalAddrIdType addrId
)
{
  boolean retVal = FALSE;
  TcpIp_IpAddrConstPtr nextHopIpPtr;
  TcpIp_IpAddrConstPtr networkMaskPtr;
  TcpIp_IpAddrConstPtr localIpAddrPtr;

  /* get ip address and netmask, gateway address can be ignored */
  TcpIp_IpAddrM_getIpConfig(addrId, &localIpAddrPtr, &nextHopIpPtr, &networkMaskPtr);

  /* remote address shall not be our ip */
  if(remoteIpAddr != *localIpAddrPtr)
  {
    const uint32 localNetworkId = (*localIpAddrPtr)&(*networkMaskPtr);
    const uint32 destinationNetworkId = remoteIpAddr&(*networkMaskPtr);

    /* destination and source are in the same subnet */
    if(localNetworkId == destinationNetworkId)
    {
      const uint32 hostMask = (uint32)~(*networkMaskPtr);

      /* check for subnet broadcast */
      if((remoteIpAddr&hostMask) == (hostMask&COMSTACK_HTON_UINT32(TCPIP_IPV4_ADDR_LIMITED_BROADCAST_VALUE)))
      {
        /* remote address is the subnet broadcast, it makes no sense to create
         * ARP entry for subnet broadcast because subnet broadcast is sent with
         * Broadcast link layer address  */
        retVal = FALSE;
      }
      else
      {
        /* within subnet */
        retVal = TRUE;
      }
    }
    else if(TcpIp_IpV4_IsLinkLocalAddr(*localIpAddrPtr))
    {
      /* source address is link local, allow remote host outside the network */
      retVal = TRUE;
    }
    else if(TcpIp_IpV4_IsLinkLocalAddr(remoteIpAddr))
    {
       /* destination address is link local, always send to link local address directly */
       retVal = TRUE;
    }
    else
    {
      /* remote address is not in subnet, it makes no sense to create
      * ARP entries for ip addresses which are not in the same subnet, because
      * messages outside of the subnet are sent to the gateway  */
      retVal = FALSE;
    }
  }

  return retVal;
}

#endif /* (TCPIP_IPV4_ENABLED == STD_ON) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>


/*==================[end of file]===========================================*/
