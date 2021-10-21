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
 * MISRAC2012-3) Deviated Rule: 11.3 (required)
 * A cast shall not be performed between a pointer to object type and a pointer to a different
 * object type.
 *
 * Reason:
 * 1)AUTOSAR defines a generic type which dependent on a member of the generic type
 * implements an concrete type. Thus for usage of this concrete type, a cast is necessary.
 * 2)Aligning for better memory subsystem performance can only be done by casting
 * the pointer to different alignment after the alignment checks.
 *
 */
/*==================[inclusions]============================================*/
#include <TSMem.h>                /* memory functions */
#include <SchM_TcpIp.h>           /* SchM API declarations */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_IpSecDb_Int.h>    /* own SW-unit interface file */

#include <TcpIp_Int.h>            /* used SW-unit interface file */
#include <TcpIp_IpV4_Int.h>       /* used SW-unit interface file */
#include <TcpIp_IpV6_Int.h>       /* used SW-unit interface file */
#include <TcpIp_Trace.h>          /* debug and trace macros; must be last */

/*==================[macros]================================================*/

#if (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL)

/** \brief IPsec is activated for this connection */
#define TCPIP_IPSECDB_ACTIVE                                                0U
/** \brief IPsec is deactivated for this connection */
#define TCPIP_IPSECDB_INACTIVE                                              1U
/** \brief Security association is not in use */
#define TCPIP_IPSECDB_SA_STATE_STOPPED                                      0U
/** \brief Security association is a manual configured SA and is activated */
#define TCPIP_IPSECDB_SA_STATE_STATIC                                       1U

#define TCPIP_IPSECDB_INVALID_POLICY_INDEX                                255U
#define TCPIP_IPSECDB_INVALID_CONN_INDEX                                  255U

/** \brief IpSec upper layer protocol ANY */
#define TCPIP_IPSECDB_UPPER_LAYER_ANY                                       0U

/** \brief Check if IpSec upper layer protocol ICMP or ICMPV6 */
#define TCPIP_IPSECDB_UPPER_LAYER_ICMP_ICMPV6(protocol) \
  ((protocol == TCPIP_IP_PROTOCOL_ICMP) || (protocol == TCPIP_IP_PROTOCOL_ICMPV6))

/** \brief Retrieves IPv6 remote address from ipsec connection config */
#define TCPIP_IPSECDB_PBCFG_GET_REMOTE_V6_ADDR(idx)                         \
  (TCPIP_CFG_TYPE(TcpIp_AddrInet6Type,                                      \
                  ipSecConnectionIpV6RemoteAddrList,                        \
                  idx)).addr

/** \brief Retrieves IPv4 remote address from ipsec connection config */
#define TCPIP_IPSECDB_PBCFG_GET_REMOTE_V4_ADDR(idx)                         \
  (TCPIP_CFG_TYPE(TcpIp_AddrInetType,                                       \
                  ipSecConnectionIpV4RemoteAddrList,                        \
                  idx)).addr

/** \brief Retrieves Physical/MAC remote address from ipsec connection config */
#define TCPIP_IPSECDB_PBCFG_GET_REMOTE_MAC_ADDR(idx)                       \
  (TCPIP_CFG_TYPE(TcpIp_MACAddrType,                                       \
                  ipSecConnectionMACRemoteAddrList,                        \
                  idx)).physAddr

/** \brief Retrieves the 16-bit source port from the TCP/UDP header */
#define TCPIP_IPSECDB_SRC_PORT_GET(headerPtr)\
    TCPIP_GET16(headerPtr, 0)

/** \brief Retrieves the 16-bit destination port from the TCP/UDP header */
#define TCPIP_IPSECDB_DEST_PORT_GET(headerPtr)\
    TCPIP_GET16(headerPtr, 2)

/** \brief Upper layer must have at least 4 bytes to hold the ports */
#define TCPIP_IPSECDB_MINIMAL_UPPER_HDR_LENGTH                              4U

#define TCPIP_IPSCEDB_INIT_SOCK_ADDR(sockAddr, ipAddr, portIn, dom, addrLen)  \
  TS_MemCpy((sockAddr).addr, (ipAddr), (addrLen));                            \
  ((sockAddr).port) = (portIn);                                               \
  ((sockAddr).domain) = (dom)

#define TCPIP_IPSECDB_GET_ADDR_DOMAIN(localAddrId)               \
    TCPIP_IP_IF_V4_V6_BOTH                                       \
    (                                                            \
      TCPIP_AF_INET,                                             \
      TCPIP_AF_INET6,                                            \
      (TCPIP_IPADDRM_IS_ADDRDOMAIN                               \
        (TcpIp_IpAddrM_getLocalAddrInfo(localAddrId), IPV4) ?    \
                TCPIP_AF_INET : TCPIP_AF_INET6)                  \
    )

#define TCPIP_IPSECDB_GET_ADDR_LEN(domain)                       \
    TCPIP_IP_IF_V4_V6_BOTH                                       \
    (                                                            \
      TCPIP_IPV4_ADDR_BYTE_SIZE,                                 \
      TCPIP_IPV6_ADDR_BYTE_SIZE,                                 \
      ((domain == TCPIP_AF_INET) ?                               \
        TCPIP_IPV4_ADDR_BYTE_SIZE : TCPIP_IPV6_ADDR_BYTE_SIZE)   \
    )

#if (TCPIP_REMOTE_MAC_FILTERING_ENABLED == STD_ON)
/** \brief Check if the connection should be protected by cheking remote MAC address */
#define TCPIP_IPSECDB_REMOTE_PHYS_PROTECTED(remotePhysAddressIdx)      \
   ((remotePhysAddressIdx) < ((uint8_least)TcpIp_PBcfgPtr->numOfIpSecMACFilteringEntries))
#endif /* (TCPIP_REMOTE_MAC_FILTERING_ENABLED == STD_ON) */

/*==================[type definitions]======================================*/

/** \brief Structure describing a Security Association (SA) entry in the IpSecDb database. */
typedef struct
{
  /** \brief Security Parameter Index (SPI) for inbound communication. */
  uint32 inboundSpi;
  /** \brief SPI for outbound communication. */
  uint32 outboundSpi;
  /** \brief The 4 least significant bytes of the sequence number used for outbound communication. */
  uint32 outSequenceNumberLow;
  /** \brief The 4 most significant bytes of the sequence number used for outbound communication.
    Only used for extended sequence number (ESN) usage. */
  uint32 outSequenceNumberHigh;
#if (TCPIP_SECURITY_MODE == TCPIP_IKEV2)
  /** \brief The 4 least significant bytes of the sequence number used for inbound communication. */
  uint32 inSequenceNumberLow;
  /** \brief The 4 most significant bytes of the sequence number used for inbound communication.
    Only used for ESN usage. */
  uint32 inSequenceNumberHigh;
#endif /* (TCPIP_SECURITY_MODE == TCPIP_IKEV2) */
  /** \brief State of the database entry. */
  uint8 state;
  /** \brief Index of the next database entry in the linked list of free/used entries. */
  uint8 nextSaIndex;
  /** \brief IpSec connection identifier this database entry belongs to. */
  uint8 connectionId;
}TcpIp_IpSecDb_SaDataType;

/** \brief Structure describing a list of security associations (SAs). */
typedef struct
{
  /** \brief Index to list of SAs established for this connection. */
  uint8 saIndex;
  /** \brief State of the current connection. */
  uint8 state;
}TcpIp_IpSecDb_SaConnDataType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)

/** \brief Read and reset valid secured/bypassed packets
 *
 * This function reads and resets valid secured/bypassed packets
 *
 * \param[in] MeasurementResetNeeded   Flag to trigger a reset of the measurement
 * \param[out] MeasurementDataPtr      Reference to data buffer
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpSecDb_GetAndResetValidPolicyCount
(
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) MeasurementDataPtr
);

/** \brief Read and reset dropped datagrams caused by invalid policy
 *
 * This function reads and resets dropped datagrams caused by invalid policy.
 *
 * \param[in] MeasurementResetNeeded   Flag to trigger a reset of the measurement
 * \param[out] MeasurementDataPtr      Reference to data buffer
 */

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpSecDb_GetAndResetInvalidPolicyCount
(
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) MeasurementDataPtr
);
#endif /* (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON) */



/** \brief Check in the security policy table if a frame shall be
 *         discarded, secured or bypassed
 *
 * \param[in]  localAddrId            ip address of the local host
 * \param[in]  remoteAddrPtr          ip address of the remote host
 * \param[in]  nextHeader             Upper layer Protocol UDP/TCP/ICMP
 * \param[in]  localPort              UDP/TCP port of the local host
 * \param[in]  remotePort             UDP/TCP port of the remote host
 * \param[out] ipSecConnectionIdxPtr  returns matching connection idx if
 *                                    secured, otherwise TCPIP_IPSECDB_INVALID_SA_INDEX
 * \param[in]  direction              INBOUND: look for inbound policies
                                      OUTBOUND: look for outbound policies
 * \param[in]  physAddrPtr            physical address of the remote host
 *
 * \return Indicates what shall happen with the frame
 * \retval TCPIP_IPSECDB_MECHANISM_DISCARDED  discard the frame
 * \retval TCPIP_IPSECDB_MECHANISM_SECURED    secure the frame
 * \retval TCPIP_IPSECDB_MECHANISM_BYPASSED   bypass the frame
 */
STATIC FUNC(uint8, TCPIP_CODE) TcpIp_IpSecDb_checkSecurityPolicy
(
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  uint8 nextHeader,
  uint16 localPort,
  uint16 remotePort,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) ipSecConnectionIdxPtr,
  boolean direction,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr
);

/** \brief Loops over all policies configured for the remote host and
 *  checks if matching ports and protocol can be found
 *
 * \param[in]  numOfPolicies  number of policies configured for the connection
 * \param[in]  policiesOffset offset of the first policy in the policy list
 * \param[in]  nextHeader     upper layer protocol to check
 * \param[in]  localPort      Local UDP/TCP port to check
 * \param[in]  remotePort     Remote UDP/TCP port to check
 *
 * \return Policy index
 */
STATIC FUNC(uint8, TCPIP_CODE) TcpIp_IpSecDb_checkMechanism
(
  uint8 numOfPolicies,
  uint8 policiesOffset,
  uint8 nextHeader,
  uint16 localPort,
  uint16 remotePort
);

/** \brief Checks if port and upper layer protocol matches with a local or remote rule
 *
 * \param[in]  numOfRules  number of rules configured for this policy
 * \param[in]  rulesOffset offset of the first rule in the rule list
 * \param[in]  nextHeader  upper layer protocol to check
 * \param[in]  port        UDP/TCP port to check
 *
 * \return Indicates if matching rule was found
 * \retval TRUE           match found
 * \retval FALSE          no match found
 */
STATIC FUNC(boolean, TCPIP_CODE) TcpIp_IpSecDb_checkRule
(
  uint8 numOfRules,
  uint8 rulesOffset,
  uint8 nextHeader,
  uint16 port
);

/** \brief Verifies if remoteAddr matches ip address range configured in policy table
 *
 * \param[in]  connectionConfigPtr  Reference to connection configuration which contains
 *                                  allowed remote ip address range
 * \param[in]  remoteAddrPtr        IP address which should be checked
 *
 * \return Indicates if remote address is in range
 * \retval E_OK           Address in range
 * \retval E_NOT_OK       Address out of range
 *
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSecDb_checkRemoteIpAddrRange
(
  P2CONST(TcpIp_IpSecConnectionConfigType, AUTOMATIC, TCPIP_APPL_DATA) connectionConfigPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr
);

#if (TCPIP_REMOTE_MAC_FILTERING_ENABLED == STD_ON)
/** \brief Verifies if remoteAddr matches ip address range configured in policy table
 *
 * \param[in]  connectionConfigPtr  Reference to connection configuration which contains
 *                                  allowed remote physical address range
 * \param[in]  physAddrPtr          Points to MAC address of remote host (source MAC)
 *
 * \return Indicates if remote address is in range
 * \retval E_OK           Address in range
 * \retval E_NOT_OK       Address out of range
 *
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSecDb_checkRemotePhysAddrRange
(
  P2CONST(TcpIp_IpSecConnectionConfigType, AUTOMATIC, TCPIP_APPL_DATA) connectionConfigPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr
);
#endif /* (TCPIP_REMOTE_MAC_FILTERING_ENABLED == STD_ON) */

/** \brief Verifies if remotePhysAddrPtr is between startAddr and endAddr
 *
 * \param[in]  startAddrPtr      Begin of the address range
 * \param[in]  endAddrPtr        End of the address range
 * \param[in]  remoteAddrPtr     Address which should be checked
 * \param[in]  length            Address length which should be checked
 *
 * \return Indicates if remote address is in range
 * \retval E_OK           Address in range
 * \retval E_NOT_OK       Address out of range
 *
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSecDb_checkAddrRange
(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) startAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) endAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  uint8 length
);

/** \brief Gets source and destination ports from UDP/TCP header
 *
 * \param[in]  nextHeader    nextHeader
 * \param[in]  dataPtr       Pointer to the UDP/TCP header
 * \param[in]  length        Length of the IP payload
 * \param[out] localPortPtr  Returns the local port from the UPD/TCP header
 * \param[out] remotePortPtr Returns the remote port from the UPD/TCP header
 *
 * \return Indicates if ports could be successfully extracted
 * \retval TCPIP_OK                Ports could be extracted
 * \retval TCPIP_E_NOT_OK          Invalid header encountered
 *
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSecDb_getPorts
(
  uint8 nextHeader,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 length,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) localPortPtr,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) remotePortPtr
);


#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)

/** \brief Calculates the next outbound sequence number
 *
 * \param[in] ipsecSaIdx   index of the security association
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpSecDb_updateOutboundSeqNum
(
  uint8 ipsecSaIdx
);
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */

/** \brief Gets a remote IP address
 *
 * This service retrieves an IP address of an IpSec connection specified by a configuration
 * reference from the post-build configuration.
 *
 * \param[in] connectionConfigPtr   Reference to the post-build configuration element of the IpSec
 *                                  connection
 *
 * \return A reference to the remote IP address
 *
 */
STATIC FUNC_P2CONST(uint32, TCPIP_APPL_DATA, TCPIP_CODE) TcpIp_IpSecDb_getRemoteIpAddr
(
  P2CONST(TcpIp_IpSecConnectionConfigType, AUTOMATIC, TCPIP_APPL_DATA) connectionConfigPtr
);

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/
#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)

#define TCPIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

/** \brief Array of structures describing the Security Associations (SAs). */
VAR(TcpIp_IpSecDb_SaDataType, TCPIP_VAR_CLEARED) TcpIp_IpSecDb_Data[TCPIP_IPSEC_NUM_SEC_ASSOCIATIONS];

/** \brief Array of structures describing the SAs of the IpSec connections. */
VAR(TcpIp_IpSecDb_SaConnDataType, TCPIP_VAR_CLEARED) TcpIp_IpSecDb_SaConnData[TCPIP_IPSEC_MAX_NUM_CONNECTIONS];

#if (TCPIP_SECURITY_MODE == TCPIP_IKEV2)
/** \brief Array of indices to the first free SA runtime data elements of an SA configuration. */
STATIC VAR(uint8, TCPIP_VAR_CLEARED) TcpIp_IpSecDb_firstFreeIpSecSa[TCPIP_IPSEC_NUM_SA_CONFIGS];
#endif /* (TCPIP_SECURITY_MODE == TCPIP_IKEV2) */

#define TCPIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */
#define TCPIP_START_SEC_VAR_INIT_32
#include <TcpIp_MemMap.h>

#if ((TCPIP_IPSECDB_GETANDRESETMEASUREMENTDATA_ENABLE == STD_ON))
STATIC VAR(uint32, TCPIP_VAR) TcpIp_IpSecDb_ValidCounter = 0U;
STATIC VAR(uint32, TCPIP_VAR) TcpIp_IpSecDb_InvalidCounter = 0U;
#endif /* ((TCPIP_IPSECDB_GETANDRESETMEASUREMENTDATA_ENABLE == STD_ON)) */

#define TCPIP_STOP_SEC_VAR_INIT_32
#include <TcpIp_MemMap.h>

/*==================[external function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpSecDb_init (void)
{
  uint8_least i;

  DBG_TCPIP_IPSECDB_INIT_ENTRY();

  /* loop over all security association configs */
  for(i = 0U; i < (uint8_least)TcpIp_PBcfgPtr->numOfIpSecSecurityAssociationCfg; i++)
  {
    CONSTP2CONST(TcpIp_IpSecSecurityAssociationCommonConfigType, AUTOMATIC, TCPIP_APPL_DATA) ipSecSaCommConfigPtr =
      &TCPIP_CFG_TYPE(TcpIp_IpSecSecurityAssociationCommonConfigType, ipSecSecurityAssociationCommonList, i);
    const uint8 startSA = ipSecSaCommConfigPtr->ipSecSaOffset;
    const uint8 endSA = startSA + ipSecSaCommConfigPtr->ipSecSaNum;
    uint8_least j;

    /* loop over all ipsec sas which are configured for this config */
    for(j = startSA; j < endSA; j++)
    {
      P2VAR(TcpIp_IpSecDb_SaDataType, AUTOMATIC, TCPIP_APPL_DATA) ipSecSaDataPtr =
        &TcpIp_IpSecDb_Data[j];

      ipSecSaDataPtr->state = TCPIP_IPSECDB_SA_STATE_STOPPED;
      ipSecSaDataPtr->outboundSpi =  0xffffffffU;
      ipSecSaDataPtr->inboundSpi =  0xffffffffU;
      ipSecSaDataPtr->outSequenceNumberHigh = 0U;
      ipSecSaDataPtr->outSequenceNumberLow = 0U;
      ipSecSaDataPtr->nextSaIndex = (uint8)(j + 1U);
      ipSecSaDataPtr->connectionId = TCPIP_IPSECDB_INVALID_CONN_INDEX;
#if (TCPIP_SECURITY_MODE == TCPIP_IKEV2)
      ipSecSaDataPtr->inSequenceNumberHigh = 0U;
      ipSecSaDataPtr->inSequenceNumberLow = 0U;
#endif /* (TCPIP_SECURITY_MODE == TCPIP_IKEV2) */
    }

    /* set first and last free ike SA */
    TcpIp_IpSecDb_Data[j - 1U].nextSaIndex = TCPIP_IPSECDB_INVALID_SA_INDEX;
#if (TCPIP_SECURITY_MODE == TCPIP_IKEV2)
    TcpIp_IpSecDb_firstFreeIpSecSa[i] = startSA;
#endif /* (TCPIP_SECURITY_MODE == TCPIP_IKEV2) */
  }

  for(i=0U; i < TCPIP_IPSEC_NUM_CONNECTIONS; i++)
  {
    P2VAR(TcpIp_IpSecDb_SaConnDataType, AUTOMATIC, TCPIP_APPL_DATA) ipSecSaConnDataPtr =
      &TcpIp_IpSecDb_SaConnData[i];

    ipSecSaConnDataPtr->state = TCPIP_IPSECDB_INACTIVE;
    ipSecSaConnDataPtr->saIndex = TCPIP_IPSECDB_INVALID_SA_INDEX;
  }

  DBG_TCPIP_IPSECDB_INIT_EXIT();

}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE ) TcpIp_IpSecDb_getSecurityRxAssociation
(
  uint32 spi,
  uint32 seqNum,
  TcpIp_LocalAddrIdType localAddr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) ipSecSaIdxPtr,
  P2VAR(TcpIp_IpSecSaDataType, AUTOMATIC, TCPIP_APPL_DATA) saDataPtr
)
{
  Std_ReturnType retval = E_NOT_OK;

  DBG_TCPIP_IPSECDB_GETSECURITYRXASSOCIATION_ENTRY
    (spi,seqNum,localAddr,remoteAddrPtr,ipSecSaIdxPtr,saDataPtr);

#if (TCPIP_SECURITY_MODE != TCPIP_IKEV2)
  TS_PARAM_UNUSED(seqNum);
#endif /* (TCPIP_SECURITY_MODE != TCPIP_IKEV2) */

  {
    P2CONST(TcpIp_IpSecLocalAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrConfigPtr =
       &(TCPIP_CFG_TYPE(TcpIp_IpSecLocalAddrConfigType, ipSecLocalAddrList, localAddr));
    const uint8 startOffset = localAddrConfigPtr->connectionOffset;
    const uint8 endOffset = startOffset + localAddrConfigPtr->numOfConnections;

    uint8 i;

    /* loop over all configured connections for this local address */
    for(i = startOffset; (i < endOffset) && (retval != E_OK); i++)
    {
      CONSTP2CONST(TcpIp_IpSecConnectionConfigType,AUTOMATIC,TCPIP_APPL_CONST) IpsecConnPtr =
        &(TCPIP_CFG_TYPE(TcpIp_IpSecConnectionConfigType,ipSecConnectionsList,i));

        P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) configuredRemoteIpAddrPtr =
          TcpIp_IpSecDb_getRemoteIpAddr(IpsecConnPtr);

      if(IpsecConnPtr->remoteAddrIsSingle)
      {
        /* verify if remote addresses match for this configuration */
        retval =
          TcpIp_Ip_compareIpAddr(IpsecConnPtr->addrType, remoteAddrPtr, configuredRemoteIpAddrPtr);
      }

      if(retval == E_OK)
      {
        uint8 ipsecSaIdx = TcpIp_IpSecDb_SaConnData[i].saIndex;

        retval = E_NOT_OK;

        /* loop over all security associations established for this connection */
        while((ipsecSaIdx != TCPIP_IPSECDB_INVALID_SA_INDEX) && (retval != E_OK))
        {
          P2VAR(TcpIp_IpSecDb_SaDataType, AUTOMATIC, TCPIP_APPL_DATA) ipSecSaPtr =
            &TcpIp_IpSecDb_Data[ipsecSaIdx];

          /* find matching spi */
          if(spi == ipSecSaPtr->inboundSpi)
          {

            {
              P2CONST(TcpIp_IpSecSecurityAssociationCommonConfigType, AUTOMATIC, TCPIP_APPL_DATA) saCommonConfigPtr =
                 &TCPIP_CFG_TYPE(TcpIp_IpSecSecurityAssociationCommonConfigType,
                                 ipSecSecurityAssociationCommonList,
                                 ipsecSaIdx
                                );
              P2CONST(TcpIp_IpSecSecurityAssociationConfigType, AUTOMATIC, TCPIP_APPL_DATA) saConfigPtr =
                  &TCPIP_CFG_TYPE(TcpIp_IpSecSecurityAssociationConfigType,
                                  ipSecSecurityAssociationList,
                                  ipsecSaIdx
                                 );
              P2CONST(TcpIp_IpSec_ProposalType, AUTOMATIC, TCPIP_APPL_DATA) proposalPtr =
                  &(TS_UNCHECKEDGETCFG
                      (TcpIp_PBcfgPtr, TcpIp_IpSec_ProposalType,
                       TCPIP,
                       saCommonConfigPtr->proposalPtr))[0U];

              *ipSecSaIdxPtr = ipsecSaIdx;
              saDataPtr->spi = ipSecSaPtr->inboundSpi;
              saDataPtr->keyId = saConfigPtr->inboundIntegrityKey;
              saDataPtr->jobId = saConfigPtr->inboundIntegrityjobId;

              saDataPtr->MacLen = proposalPtr->truncatedLength;
              saDataPtr->useGmac = proposalPtr->useGMAC;
              retval = E_OK;
            }

            /* matching SA found stop further processing */
            break;
          }

          /* get the next security association in the list */
          ipsecSaIdx = ipSecSaPtr->nextSaIndex;
        }
      }
    }
  }

  DBG_TCPIP_IPSECDB_GETSECURITYRXASSOCIATION_EXIT
    (retval,spi,seqNum,localAddr,remoteAddrPtr,ipSecSaIdxPtr,saDataPtr);

  return retval;
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpSecDb_stop
(
  uint8 conId
)
{
  P2CONST(TcpIp_IpSecConnectionConfigType, AUTOMATIC, TCPIP_APPL_DATA) connectionConfigPtr =
      &TCPIP_CFG_TYPE(TcpIp_IpSecConnectionConfigType, ipSecConnectionsList, conId);

  uint8 inSecurityPolicyIdx = connectionConfigPtr->inboundPolicies.SecuredPoliciesOffset;

  DBG_TCPIP_IPSECDB_STOP_ENTRY(conId);

  {
    CONSTP2CONST(TcpIp_IpSecPolicyConfigType, AUTOMATIC, TCPIP_APPL_DATA) policyConfigPtr =
      &TCPIP_CFG_TYPE(TcpIp_IpSecPolicyConfigType, ipSecPolicyList, inSecurityPolicyIdx);
    const uint8 saConfigIdx = policyConfigPtr->securityAssociationIdx;

    /* check if a security association is configured for the policy */
    if(saConfigIdx != TCPIP_IPSECDB_INVALID_SA_INDEX)
    {
      P2CONST(TcpIp_IpSecSecurityAssociationCommonConfigType,AUTOMATIC,TCPIP_APPL_CONST) saConfig =
        &TCPIP_CFG_TYPE(TcpIp_IpSecSecurityAssociationCommonConfigType,
                        ipSecSecurityAssociationCommonList,
                        saConfigIdx
                       );


      {
        /* Note: manual sas have only one ipsec sa configured */
        uint8 ipsecSaIdx = saConfig->ipSecSaOffset;

        P2VAR(TcpIp_IpSecDb_SaDataType, AUTOMATIC, TCPIP_APPL_DATA) ipSecSaPtr =
          &TcpIp_IpSecDb_Data[ipsecSaIdx];

        ipSecSaPtr->state = TCPIP_IPSECDB_SA_STATE_STOPPED;

        /* remove security association from the active list */
        TcpIp_IpSecDb_SaConnData[conId].saIndex = TCPIP_IPSECDB_INVALID_SA_INDEX;

        /* !LINKSTO TcpIp.Design.IpSecDb.Atomic.state,1 */
        TS_AtomicAssign8(TcpIp_IpSecDb_SaConnData[conId].state, TCPIP_IPSECDB_INACTIVE);
      }
    }
  }

  DBG_TCPIP_IPSECDB_STOP_EXIT(conId);

}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpSecDb_start
(
  uint8 conId
)
{
  P2CONST(TcpIp_IpSecConnectionConfigType, AUTOMATIC, TCPIP_APPL_DATA) connectionConfigPtr =
      &TCPIP_CFG_TYPE(TcpIp_IpSecConnectionConfigType, ipSecConnectionsList, conId);

  uint8 inSecurityPolicyIdx = connectionConfigPtr->inboundPolicies.SecuredPoliciesOffset;

  DBG_TCPIP_IPSECDB_START_ENTRY(conId);

  {
    CONSTP2CONST(TcpIp_IpSecPolicyConfigType, AUTOMATIC, TCPIP_APPL_DATA) policyConfigPtr =
      &TCPIP_CFG_TYPE(TcpIp_IpSecPolicyConfigType, ipSecPolicyList, inSecurityPolicyIdx);
    const uint8 saConfigIdx = policyConfigPtr->securityAssociationIdx;

    /* check if a security association is configured for the policy */
    if(saConfigIdx != TCPIP_IPSECDB_INVALID_SA_INDEX)
    {
      P2CONST(TcpIp_IpSecSecurityAssociationCommonConfigType,AUTOMATIC,TCPIP_APPL_CONST) saConfig =
        &TCPIP_CFG_TYPE(TcpIp_IpSecSecurityAssociationCommonConfigType,
                        ipSecSecurityAssociationCommonList,
                        saConfigIdx
                       );

      /* Note: manual sas have only one ipsec sa configured */
      uint8 ipsecSaIdx = saConfig->ipSecSaOffset;
      P2VAR(TcpIp_IpSecDb_SaDataType, AUTOMATIC, TCPIP_APPL_DATA) ipSecSaDataPtr =
        &TcpIp_IpSecDb_Data[ipsecSaIdx];

      /* if sa is manual set configured for this policy */
      TcpIp_IpSecDb_SaConnData[conId].saIndex = ipsecSaIdx;
      ipSecSaDataPtr->outSequenceNumberHigh = 0U;
      ipSecSaDataPtr->outSequenceNumberLow = 0U;
      ipSecSaDataPtr->state = TCPIP_IPSECDB_SA_STATE_STATIC;
      ipSecSaDataPtr->outboundSpi = saConfig->outboundSpi;
      ipSecSaDataPtr->inboundSpi = saConfig->inboundSpi;
      ipSecSaDataPtr->connectionId = conId;

      /* !LINKSTO TcpIp.Design.IpSecDb.Atomic.state,1 */
      TS_AtomicAssign8(TcpIp_IpSecDb_SaConnData[conId].state, TCPIP_IPSECDB_ACTIVE);
    }
  }

  DBG_TCPIP_IPSECDB_START_EXIT(conId);

}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSecDb_getSecurityTxAssociation
(
  uint8 connectionIdx,
  P2VAR(TcpIp_IpSecSaDataType, AUTOMATIC, TCPIP_APPL_DATA) saDataPtr,
  boolean getSeqNum
)
{
  Std_ReturnType retval = E_NOT_OK;

  DBG_TCPIP_IPSECDB_GETSECURITYTXASSOCIATION_ENTRY(connectionIdx,saDataPtr,getSeqNum);

  {
    P2CONST(TcpIp_IpSecConnectionConfigType, AUTOMATIC, TCPIP_APPL_DATA) connectionConfigPtr =
      &TCPIP_CFG_TYPE(TcpIp_IpSecConnectionConfigType, ipSecConnectionsList, connectionIdx);

    uint8 inSecurityPolicyIdx = connectionConfigPtr->outboundPolicies.SecuredPoliciesOffset;
    CONSTP2CONST(TcpIp_IpSecPolicyConfigType, AUTOMATIC, TCPIP_APPL_DATA) policyConfigPtr =
      &TCPIP_CFG_TYPE(TcpIp_IpSecPolicyConfigType, ipSecPolicyList, inSecurityPolicyIdx);
    const uint8 saConfigIdx = policyConfigPtr->securityAssociationIdx;

    uint8 ipsecSaIdx = TcpIp_IpSecDb_SaConnData[connectionIdx].saIndex;

    /* loop over all security associations established for this connection */
    while((ipsecSaIdx != TCPIP_IPSECDB_INVALID_SA_INDEX) && (retval != E_OK))
    {
      P2CONST(TcpIp_IpSecSecurityAssociationCommonConfigType, AUTOMATIC, TCPIP_APPL_DATA) saCommonConfigPtr =
          &TCPIP_CFG_TYPE(TcpIp_IpSecSecurityAssociationCommonConfigType,
                          ipSecSecurityAssociationCommonList,
                          saConfigIdx
                         );
      P2CONST(TcpIp_IpSecSecurityAssociationConfigType, AUTOMATIC, TCPIP_APPL_DATA) saConfigPtr =
          &TCPIP_CFG_TYPE(TcpIp_IpSecSecurityAssociationConfigType,
                          ipSecSecurityAssociationList,
                          ipsecSaIdx
                         );

      P2CONST(TcpIp_IpSec_ProposalType, AUTOMATIC, TCPIP_APPL_DATA) proposalPtr =
          &(TS_UNCHECKEDGETCFG
              (TcpIp_PBcfgPtr, TcpIp_IpSec_ProposalType,
               TCPIP,
               saCommonConfigPtr->proposalPtr))[0U];

      /* update the sequence number */
      if(getSeqNum)
      {
        TcpIp_IpSecDb_updateOutboundSeqNum(ipsecSaIdx);
        saDataPtr->SeqNumLow = TcpIp_IpSecDb_Data[ipsecSaIdx].outSequenceNumberLow;
        saDataPtr->SeqNumHigh = TcpIp_IpSecDb_Data[ipsecSaIdx].outSequenceNumberHigh;
      }

      /* return information for this security association */
      saDataPtr->spi = TcpIp_IpSecDb_Data[ipsecSaIdx].outboundSpi;
      saDataPtr->keyId = saConfigPtr->outboundIntegrityKey;
      saDataPtr->jobId = saConfigPtr->outboundIntegrityjobId;

      saDataPtr->MacLen = proposalPtr->truncatedLength;
      saDataPtr->useGmac = proposalPtr->useGMAC;
      saDataPtr->securityHeadersize =
         TCPIP_IP_IF_V4_V6_BOTH((proposalPtr->ipv4SecurityHeadersize),
                                (proposalPtr->ipv6SecurityHeadersize),
                                ((connectionConfigPtr->addrType == TCPIP_AF_INET) ?
                                (proposalPtr->ipv4SecurityHeadersize):
                                (proposalPtr->ipv6SecurityHeadersize))
                               );

      retval = E_OK;
    }
  }

  DBG_TCPIP_IPSECDB_GETSECURITYTXASSOCIATION_EXIT(retval,connectionIdx,saDataPtr,getSeqNum);

  return retval;
}

#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */

#if (TCPIP_IPSECDB_GETANDRESETMEASUREMENTDATA_ENABLE == STD_ON)

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpSecDb_LogError
(
  uint8 localAddrId,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteIpPtr,
  uint8 errorType
)
{
#if (TCPIP_IPSEC_ERROR_REPORT_FCT_ENABLED == STD_ON)
  uint32 validCounter = 0xFFFFFFFFU;
  uint32 invalidCounter = 0xFFFFFFFFU;
#endif /* (TCPIP_IPSEC_ERROR_REPORT_FCT_ENABLED == STD_ON) */

  DBG_TCPIP_IPSECDB_LOGERROR_ENTRY(localAddrId, remoteIpPtr, errorType);

  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_6();

  if((TcpIp_IpSecDb_InvalidCounter<0xFFFFFFFFU) && (errorType == TCPIP_MEAS_DROP_INVALID_POLICY))
  {
    TS_AtomicAssign32(TcpIp_IpSecDb_InvalidCounter, TcpIp_IpSecDb_InvalidCounter+1U);
  }

#if (TCPIP_IPSEC_ERROR_REPORT_FCT_ENABLED == STD_ON)
  TS_AtomicAssign32(validCounter, TcpIp_IpSecDb_ValidCounter);
  TS_AtomicAssign32(invalidCounter, TcpIp_IpSecDb_InvalidCounter);
#endif /* (TCPIP_IPSEC_ERROR_REPORT_FCT_ENABLED == STD_ON) */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_6();

#if (TCPIP_IPSEC_ERROR_REPORT_FCT_ENABLED == STD_ON)
  {
    const uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
    TcpIp_IpAddrConstPtr ipAddrPtr;
    const uint8 domain = TCPIP_IPSECDB_GET_ADDR_DOMAIN(localAddrId);
    const uint8 addrLen = TCPIP_IPSECDB_GET_ADDR_LEN(domain);
    TCPIP_IP_IF_V4_V6_BOTH
      (TcpIp_SockAddrInetType, TcpIp_SockAddrInet6Type, TcpIp_SockAddrInet6Type)
        localAddr;
    TCPIP_IP_IF_V4_V6_BOTH
      (TcpIp_SockAddrInetType, TcpIp_SockAddrInet6Type, TcpIp_SockAddrInet6Type)
        remoteAddr;

    TcpIp_IpAddrM_getIpAddress(localAddrId, &ipAddrPtr);

    TCPIP_IPSCEDB_INIT_SOCK_ADDR(localAddr, ipAddrPtr, 0U, domain, addrLen);
    TCPIP_IPSCEDB_INIT_SOCK_ADDR(remoteAddr, remoteIpPtr, 0U, domain, addrLen);

    TcpIp_IpSecReportErrorAPI
    (
      ctrlIdx,
      /* Deviation MISRAC2012-3 */
      (P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CODE)) &localAddr,
      (P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_CODE)) &remoteAddr,
      validCounter,
      invalidCounter,
      errorType
    );
  }
#else /* (TCPIP_IPSEC_ERROR_REPORT_FCT_ENABLED == STD_ON) */
    TS_PARAM_UNUSED(localAddrId);
    TS_PARAM_UNUSED(remoteIpPtr);
    TS_PARAM_UNUSED(errorType);
#endif /* (TCPIP_IPSEC_ERROR_REPORT_FCT_ENABLED == STD_ON) */

  DBG_TCPIP_IPSECDB_LOGERROR_EXIT(localAddrId, remoteIpPtr, errorType);
}

#endif /* (TCPIP_IPSECDB_GETANDRESETMEASUREMENTDATA_ENABLE == STD_ON) */

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSecDb_checkSecurityPolicyAddr
(
  TcpIp_LocalAddrIdType localAddr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr
)
{
  Std_ReturnType retval = E_NOT_OK;

  DBG_TCPIP_IPSECDB_CHECKSECURITYPOLICYADDR_ENTRY(localAddr,remoteAddrPtr,physAddrPtr);

  /* generated address shall be handled as bypassed */
  if(localAddr < TcpIp_PBcfgPtr->configLocalAddrNum )
  {
    P2CONST(TcpIp_IpSecLocalAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrConfigPtr =
       &(TCPIP_CFG_TYPE(TcpIp_IpSecLocalAddrConfigType, ipSecLocalAddrList, localAddr));
    const uint8 startOffset = localAddrConfigPtr->connectionOffset;
    const uint8 endOffset = startOffset + localAddrConfigPtr->numOfConnections;

    uint8 i;

    /* loop over all configured connections for this local address */
    for(i = startOffset; (i < endOffset) && (retval != E_OK); i++)
    {
      CONSTP2CONST(TcpIp_IpSecConnectionConfigType,AUTOMATIC,TCPIP_APPL_CONST) IpsecConnPtr =
        &(TCPIP_CFG_TYPE(TcpIp_IpSecConnectionConfigType,ipSecConnectionsList,i));

      if(IpsecConnPtr->remoteAddrIsSingle)
      {
        P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) configuredRemoteIpAddrPtr =
          TcpIp_IpSecDb_getRemoteIpAddr(IpsecConnPtr);
        /* verify remote addresses matches configuration */
        retval =
          TcpIp_Ip_compareIpAddr(IpsecConnPtr->addrType, remoteAddrPtr, configuredRemoteIpAddrPtr);
      }
      else
      {
        retval = TcpIp_IpSecDb_checkRemoteIpAddrRange(IpsecConnPtr, remoteAddrPtr);
      }
#if (TCPIP_REMOTE_MAC_FILTERING_ENABLED == STD_ON)
      if(retval == E_OK)
      {
        retval = TcpIp_IpSecDb_checkRemotePhysAddrRange(IpsecConnPtr, physAddrPtr);
      }
#else /* (TCPIP_REMOTE_MAC_FILTERING_ENABLED == STD_ON) */
      TS_PARAM_UNUSED(physAddrPtr);
#endif /* (TCPIP_REMOTE_MAC_FILTERING_ENABLED == STD_ON) */
    }
  }
  else
  {
    /* generated address must be multicast */
    TCPIP_PRECONDITION_ASSERT
      (TCPIP_IPADDRM_IS_ADDRTYPE(TcpIp_IpAddrM_getLocalAddrInfo(localAddr), MULTICAST),
       TCPIP_INTERNAL_API_ID);
    retval = E_OK;
  }

#if (TCPIP_IPSECDB_GETANDRESETMEASUREMENTDATA_ENABLE == STD_ON)
  if(retval != E_OK)
  {
    TcpIp_IpSecDb_LogError(localAddr, remoteAddrPtr, TCPIP_MEAS_DROP_INVALID_POLICY);
  }
#endif/* ((TCPIP_IPSECDB_GETANDRESETMEASUREMENTDATA_ENABLE == STD_ON))*/

  DBG_TCPIP_IPSECDB_CHECKSECURITYPOLICYADDR_EXIT(retval,localAddr,remoteAddrPtr,physAddrPtr);

  return retval;
}
TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSecDb_checkSecurityRxPolicy
(
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 dataLength,
  uint8 nextHeader,
  uint8 ipSecSaIdx,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr
)
{
  Std_ReturnType retval = E_NOT_OK;
  uint16 localPort = 0U, remotePort = 0U;

  DBG_TCPIP_IPSECDB_CHECKSECURITYRXPOLICY_ENTRY(localAddrId,remoteAddrPtr,dataPtr,dataLength,nextHeader,ipSecSaIdx,physAddrPtr);

  /* retrieve local and remote port from the message */
  if(TcpIp_IpSecDb_getPorts(nextHeader, dataPtr, dataLength, &localPort, &remotePort) == E_OK)
  {
    uint8 policyConnectionIdx;

    uint8 mechanism =
      TcpIp_IpSecDb_checkSecurityPolicy(localAddrId,
                                        remoteAddrPtr,
                                        nextHeader,
                                        localPort,
                                        remotePort,
                                        &policyConnectionIdx,
                                        TCPIP_IPSECDB_DIRECTION_INBOUND,
                                        physAddrPtr
                                       );

    if(mechanism != TCPIP_IPSECDB_MECHANISM_DISCARDED)
    {
#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)
      /* if message shall be secured the mac must be valid, mac is valid when
       * connection index of found security association and connection index of
       * found policy match */
      if(mechanism == TCPIP_IPSECDB_MECHANISM_SECURED)
      {
        if((ipSecSaIdx != TCPIP_IPSECDB_INVALID_SA_INDEX) &&
           (policyConnectionIdx == TcpIp_IpSecDb_Data[ipSecSaIdx].connectionId)
          )
        {
          retval = E_OK;
        }
        else
        {
          retval = E_NOT_OK;
        }
      }
      else
#else
      TS_PARAM_UNUSED(ipSecSaIdx);
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */
      {
        /* message can be passed to next layer */
        retval = E_OK;
      }
    }
  }

#if (TCPIP_IPSECDB_GETANDRESETMEASUREMENTDATA_ENABLE == STD_ON)
  if(retval == E_OK)
  {
    SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_6();

    if(TcpIp_IpSecDb_ValidCounter<0xFFFFFFFFU)
    {
      TS_AtomicAssign32(TcpIp_IpSecDb_ValidCounter, TcpIp_IpSecDb_ValidCounter+1U);
    }

    SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_6();
  }
  else
  {
    TcpIp_IpSecDb_LogError(localAddrId, remoteAddrPtr, TCPIP_MEAS_DROP_INVALID_POLICY);
  }
#endif /* (TCPIP_IPSECDB_GETANDRESETMEASUREMENTDATA_ENABLE == STD_ON)) */

  DBG_TCPIP_IPSECDB_CHECKSECURITYRXPOLICY_EXIT(retval,localAddrId,remoteAddrPtr,dataPtr,dataLength,nextHeader,ipSecSaIdx,physAddrPtr);

  return retval;
}

TS_MOD_PRIV_DEFN FUNC(uint8, TCPIP_CODE) TcpIp_IpSecDb_checkSecurityTxPolicy
(
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  uint8 nextHeader,
  uint16 localPort,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) ipSecConnectionIdxPtr
)
{
  uint8 mechanism = TCPIP_IPSECDB_MECHANISM_DISCARDED;

  DBG_TCPIP_IPSECDB_CHECKSECURITYTXPOLICY_ENTRY
    (localAddrId,remoteAddrPtr,nextHeader,localPort,ipSecConnectionIdxPtr);

  {
    /* Deviation MISRAC2012-3 <+4> */
    P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteIpAddrPtr =
      ((P2CONST(TcpIp_SockAddrInetType, AUTOMATIC, TCPIP_APPL_DATA)) remoteAddrPtr)->addr;
    uint16 const remotePort =
      ((P2CONST(TcpIp_SockAddrInetType, AUTOMATIC, TCPIP_APPL_DATA)) remoteAddrPtr)->port;

    mechanism =
      TcpIp_IpSecDb_checkSecurityPolicy(localAddrId,
                                        remoteIpAddrPtr,
                                        nextHeader,
                                        localPort,
                                        remotePort,
                                        ipSecConnectionIdxPtr,
                                        TCPIP_IPSECDB_DIRECTION_OUTBOUND,
                                        NULL_PTR
                                       );
  }

  DBG_TCPIP_IPSECDB_CHECKSECURITYTXPOLICY_EXIT
    (mechanism,localAddrId,remoteAddrPtr,nextHeader,localPort,ipSecConnectionIdxPtr);

  return mechanism;
}

#if ((TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON))
TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpSecDb_GetAndResetPolicyCount
(
  TcpIp_MeasurementIdxType MeasurementIdx,
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) MeasurementDataPtr
)
{
  DBG_TCPIP_IPSECDB_GETANDRESETPOLICYCOUNT_ENTRY(MeasurementIdx, MeasurementResetNeeded, MeasurementDataPtr);

  switch(MeasurementIdx)
  {
    case TCPIP_MEAS_PASS_VALID_POLICY:
      TcpIp_IpSecDb_GetAndResetValidPolicyCount(MeasurementResetNeeded, MeasurementDataPtr);
      break;
    case TCPIP_MEAS_DROP_INVALID_POLICY:
      TcpIp_IpSecDb_GetAndResetInvalidPolicyCount(MeasurementResetNeeded, MeasurementDataPtr);
      break;
    case TCPIP_MEAS_ALL:
      TcpIp_IpSecDb_GetAndResetValidPolicyCount(TRUE, NULL_PTR);
      TcpIp_IpSecDb_GetAndResetInvalidPolicyCount(TRUE, NULL_PTR);
      break;
    /* CHECK: NOPARSE */
    default:
      TCPIP_UNREACHABLE_CODE_ASSERT(TCPIP_INTERNAL_API_ID);
      /* measurement index not supported */
      break;
    /* CHECK: PARSE */
  }

  DBG_TCPIP_IPSECDB_GETANDRESETPOLICYCOUNT_EXIT(MeasurementIdx, MeasurementResetNeeded, MeasurementDataPtr);
}

#endif /* ((TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)) */
/*==================[internal function definitions]=========================*/

#if(TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)
STATIC FUNC(void, TCPIP_CODE) TcpIp_IpSecDb_GetAndResetValidPolicyCount
(
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) MeasurementDataPtr
)
{
  if(MeasurementDataPtr != NULL_PTR)
  {
    TS_AtomicAssign32(*MeasurementDataPtr, TcpIp_IpSecDb_ValidCounter);
  }
  if(MeasurementResetNeeded)
  {
    TS_AtomicAssign32(TcpIp_IpSecDb_ValidCounter, 0U);
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpSecDb_GetAndResetInvalidPolicyCount
(
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) MeasurementDataPtr
)
{
  if(MeasurementDataPtr != NULL_PTR)
  {
    TS_AtomicAssign32(*MeasurementDataPtr, TcpIp_IpSecDb_InvalidCounter);
  }
  if(MeasurementResetNeeded)
  {
    TS_AtomicAssign32(TcpIp_IpSecDb_InvalidCounter, 0U);
  }
}
#endif /* TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */

STATIC FUNC(uint8, TCPIP_CODE) TcpIp_IpSecDb_checkSecurityPolicy
(
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  uint8 nextHeader,
  uint16 localPort,
  uint16 remotePort,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) ipSecConnectionIdxPtr,
  boolean direction,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr
)
{
  uint8 mechanism = TCPIP_IPSECDB_MECHANISM_DISCARDED;

  /* generated address shall be handled as bypassed */
  if(localAddrId < TcpIp_PBcfgPtr->configLocalAddrNum )
  {
    Std_ReturnType retval = E_NOT_OK;
    uint8 i;

    P2CONST(TcpIp_IpSecLocalAddrConfigType,AUTOMATIC,TCPIP_APPL_CONST) localAddrConfigPtr =
       &(TCPIP_CFG_TYPE(TcpIp_IpSecLocalAddrConfigType, ipSecLocalAddrList, localAddrId));
    const uint8 startOffset = localAddrConfigPtr->connectionOffset;
    const uint8 endOffset = startOffset + localAddrConfigPtr->numOfConnections;

    /* loop over all configured connections for this local address */
    for(i = startOffset; (i < endOffset) && (retval != E_OK); i++)
    {
      CONSTP2CONST(TcpIp_IpSecConnectionConfigType,AUTOMATIC,TCPIP_APPL_CONST) IpsecConnPtr =
        &(TCPIP_CFG_TYPE(TcpIp_IpSecConnectionConfigType,ipSecConnectionsList,i));
      const TcpIp_IpSecPolicyDirConfigType policyCfg =
        (direction == TCPIP_IPSECDB_DIRECTION_INBOUND) ?
          IpsecConnPtr->inboundPolicies : IpsecConnPtr->outboundPolicies;

      if(IpsecConnPtr->remoteAddrIsSingle)
      {
        P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) configuredRemoteIpAddrPtr =
          TcpIp_IpSecDb_getRemoteIpAddr(IpsecConnPtr);

        /* verify if remote addresses match for this configuration */
        retval =
          TcpIp_Ip_compareIpAddr
            (IpsecConnPtr->addrType, remoteAddrPtr, configuredRemoteIpAddrPtr);
      }
      else
      {
        retval = TcpIp_IpSecDb_checkRemoteIpAddrRange(IpsecConnPtr, remoteAddrPtr);
      }

#if (TCPIP_REMOTE_MAC_FILTERING_ENABLED == STD_ON)
      if((retval == E_OK) && (direction == TCPIP_IPSECDB_DIRECTION_INBOUND))
      {
        TCPIP_PRECONDITION_ASSERT(physAddrPtr != NULL_PTR, TCPIP_INTERNAL_API_ID);
        retval = TcpIp_IpSecDb_checkRemotePhysAddrRange(IpsecConnPtr, physAddrPtr);
      }
#else /* (TCPIP_REMOTE_MAC_FILTERING_ENABLED == STD_ON) */
      TS_PARAM_UNUSED(physAddrPtr);
#endif /* (TCPIP_REMOTE_MAC_FILTERING_ENABLED == STD_ON) */

      if(retval == E_OK)
      {
        uint8 policyIdx;

        retval = E_NOT_OK;

        /* check if message shall be bypassed */
        policyIdx =
          TcpIp_IpSecDb_checkMechanism(policyCfg.numOfBypassedPolicies,
                                       policyCfg.BypassedPoliciesOffset,
                                       nextHeader,
                                       localPort,
                                       remotePort
                                      );

        if(policyIdx != TCPIP_IPSECDB_INVALID_POLICY_INDEX)
        {
          /* message can be bypassed */
          *ipSecConnectionIdxPtr = TCPIP_IPSECDB_INVALID_SA_INDEX;
          mechanism = TCPIP_IPSECDB_MECHANISM_BYPASSED;
          break;
        }
#if  (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)
        else /* verify if message needs to be secured */
        {

          policyIdx =
            TcpIp_IpSecDb_checkMechanism(policyCfg.numOfSecuredPolicies,
                                         policyCfg.SecuredPoliciesOffset,
                                         nextHeader,
                                         localPort,
                                         remotePort
                                        );

          if(policyIdx != TCPIP_IPSECDB_INVALID_POLICY_INDEX)
          {
            /* message needs to be secured */
            uint8 ipSecConnectionState;

            /* save state in local variable before accessing */
            /* !LINKSTO TcpIp.Design.IpSecDb.Atomic.state,1 */
            TS_AtomicAssign8(ipSecConnectionState, TcpIp_IpSecDb_SaConnData[i].state);

            if(ipSecConnectionState == TCPIP_IPSECDB_ACTIVE)
            {
              /* if ipsec is enabled message needs to be secured */
              *ipSecConnectionIdxPtr = i;
              mechanism = TCPIP_IPSECDB_MECHANISM_SECURED;
            }
            else
            {
              /* if ipsec is disabled message can be bypassed */
              *ipSecConnectionIdxPtr = TCPIP_IPSECDB_INVALID_SA_INDEX;
              mechanism = TCPIP_IPSECDB_MECHANISM_BYPASSED;
            }
            break;
          }
          /* else discard message */
        }
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */
      }
    }
  }
  else
  {
    /* generated address must be multicast */
    TCPIP_PRECONDITION_ASSERT
      (TCPIP_IPADDRM_IS_ADDRTYPE(TcpIp_IpAddrM_getLocalAddrInfo(localAddrId), MULTICAST),
       TCPIP_INTERNAL_API_ID);
    mechanism = TCPIP_IPSECDB_MECHANISM_BYPASSED;
  }

  return mechanism;
}

STATIC FUNC(uint8, TCPIP_CODE) TcpIp_IpSecDb_checkMechanism
(
  uint8 numOfPolicies,
  uint8 policiesOffset,
  uint8 nextHeader,
  uint16 localPort,
  uint16 remotePort
)
{
  boolean match = FALSE;
  uint8 policyIdx = TCPIP_IPSECDB_INVALID_POLICY_INDEX;
  uint8 j;

  for(j = 0U; (j < numOfPolicies) && !match; j++)
  {
    CONSTP2CONST(TcpIp_IpSecPolicyConfigType,AUTOMATIC,TCPIP_APPL_CONST) SecPolPtr =
      &(TCPIP_CFG_TYPE(TcpIp_IpSecPolicyConfigType,ipSecPolicyList,(policiesOffset + j)));

    match =
      TcpIp_IpSecDb_checkRule
        (SecPolPtr->numOfLocalPolicies, SecPolPtr->LocalPoliciesOffset, nextHeader, localPort);

    if(match)
    {
      match =
        TcpIp_IpSecDb_checkRule
          (SecPolPtr->numOfRemotePolicies, SecPolPtr->remotePoliciesOffset, nextHeader, remotePort);

      if(match)
      {
        policyIdx = policiesOffset + j;
      }
    }
  }

  return policyIdx;
}

STATIC FUNC(boolean, TCPIP_CODE) TcpIp_IpSecDb_checkRule
(
  uint8 numOfRules,
  uint8 rulesOffset,
  uint8 nextHeader,
  uint16 port
)
{
  boolean match = FALSE;
  uint8 i;

  for(i = 0U; i < numOfRules; i++)
  {
    CONSTP2CONST(TcpIp_IpSecRuleConfigType,AUTOMATIC,TCPIP_APPL_CONST) RulePtr =
      &(TCPIP_CFG_TYPE(TcpIp_IpSecRuleConfigType,ipSecRuleList,(rulesOffset + i)));

    if(
       (
        (RulePtr->upperLayerProtocol == nextHeader) ||
        (RulePtr->upperLayerProtocol == TCPIP_IPSECDB_UPPER_LAYER_ANY)
       ) &&
       (
        ((port >= RulePtr->startPort) && (port <= RulePtr->endPort)) ||
        (TCPIP_IPSECDB_UPPER_LAYER_ICMP_ICMPV6(nextHeader))
       )
      )
    {
      match = TRUE;
      break;
    }
  }

  return match;
}

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSecDb_checkRemoteIpAddrRange
(
  P2CONST(TcpIp_IpSecConnectionConfigType, AUTOMATIC, TCPIP_APPL_DATA) connectionConfigPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr
)
{
  Std_ReturnType retval = E_NOT_OK;
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) startAddrPtr;
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) endAddrPtr;

  if(connectionConfigPtr->addrType == TCPIP_AF_INET)
  {
    startAddrPtr = TCPIP_IPSECDB_PBCFG_GET_REMOTE_V4_ADDR(connectionConfigPtr->remoteIpAddressIdx);
    endAddrPtr = TCPIP_IPSECDB_PBCFG_GET_REMOTE_V4_ADDR(connectionConfigPtr->remoteIpAddressIdx + 1U);
  }
  else
  {
    startAddrPtr = TCPIP_IPSECDB_PBCFG_GET_REMOTE_V6_ADDR(connectionConfigPtr->remoteIpAddressIdx);
    endAddrPtr = TCPIP_IPSECDB_PBCFG_GET_REMOTE_V6_ADDR(connectionConfigPtr->remoteIpAddressIdx + 1U);
  }

  retval = TcpIp_IpSecDb_checkAddrRange
           (
             (P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA))startAddrPtr,
             (P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA))endAddrPtr,
             (P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA))remoteAddrPtr,
             TCPIP_IP_GET_IP_ADDR_BYTE_LEN(connectionConfigPtr->addrType)
           );

  return retval;
}

#if (TCPIP_REMOTE_MAC_FILTERING_ENABLED == STD_ON)
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSecDb_checkRemotePhysAddrRange
(
  P2CONST(TcpIp_IpSecConnectionConfigType, AUTOMATIC, TCPIP_APPL_DATA) connectionConfigPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) physAddrPtr
)
{
  Std_ReturnType retval = E_NOT_OK;
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) startAddrPtr = NULL_PTR;
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) endAddrPtr = NULL_PTR;
  if(TCPIP_IPSECDB_REMOTE_PHYS_PROTECTED(connectionConfigPtr->remotePhysAddressIdx))
  {
    /* Only unicast physical addresses are allowed */
    if
    (
      !(
        ((connectionConfigPtr->addrType == TCPIP_AF_INET) && (TCPIP_MAC_IS_IPV4_MULTICAST(physAddrPtr))) ||
        ((connectionConfigPtr->addrType == TCPIP_AF_INET6) && (TCPIP_MAC_IS_IPV6_MULTICAST(physAddrPtr)))
      )
    )
    {
      startAddrPtr = TCPIP_IPSECDB_PBCFG_GET_REMOTE_MAC_ADDR(connectionConfigPtr->remotePhysAddressIdx);
      endAddrPtr = TCPIP_IPSECDB_PBCFG_GET_REMOTE_MAC_ADDR(connectionConfigPtr->remotePhysAddressIdx + 1U);

      retval = TcpIp_IpSecDb_checkAddrRange(startAddrPtr, endAddrPtr, physAddrPtr, TCPIP_ETH_ADDR_LENGTH);
    }
  }
  else
  {
    retval = E_OK;
  }

  return retval;
}
#endif /* (TCPIP_REMOTE_MAC_FILTERING_ENABLED == STD_ON) */

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSecDb_checkAddrRange
(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) startAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) endAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  uint8 length
)
{
  Std_ReturnType retval = E_NOT_OK;
  uint8 i = 0U;
  uint8 startAddr;
  uint8 endAddr;
  uint8 remoteAddr;

  for(i = 0U; i < length; i++)
  {
    remoteAddr = remoteAddrPtr[i];
    startAddr = startAddrPtr[i];
    endAddr = endAddrPtr[i];

    /* check if remote address is between start and end address segment */
    if((remoteAddr >= startAddr) && (endAddr >= remoteAddr))
    {
      retval = E_OK;

      if(startAddr == endAddr)
      {
        /* start, end and remote match continue */
        continue;
      }
      else if(remoteAddr == startAddr)
      {
        /* start and remote match, set remote as end */
        endAddrPtr = remoteAddrPtr;
      }
      else if(remoteAddr == endAddr)
      {
        /* end and remote match, set remote as start */
        startAddrPtr = remoteAddrPtr;
      }
      else
      {
        /* address is in range stop processing */
        break;
      }
    }
    else
    {
      /*ip address is not in range, stop further processing */
      retval = E_NOT_OK;
      break;
    }
  }

  return retval;
}

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSecDb_getPorts
(
  uint8 nextHeader,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 length,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) localPortPtr,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) remotePortPtr
)
{
  Std_ReturnType result = E_NOT_OK;

  *localPortPtr = 0U;
  *remotePortPtr = 0U;

  if((nextHeader == TCPIP_IP_PROTOCOL_UDP) || (nextHeader == TCPIP_IP_PROTOCOL_TCP))
  {
    if(length > TCPIP_IPSECDB_MINIMAL_UPPER_HDR_LENGTH)
    {
      *remotePortPtr = TCPIP_IPSECDB_SRC_PORT_GET(dataPtr);
      *localPortPtr = TCPIP_IPSECDB_DEST_PORT_GET(dataPtr);
      result = E_OK;
    }
  }
  else /* other header do not have ports */
  {
    result = E_OK;
  }

  return result;
}


#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)

STATIC FUNC(void, TCPIP_CODE) TcpIp_IpSecDb_updateOutboundSeqNum
(
  uint8 ipsecSaIdx
)
{
  if(TcpIp_IpSecDb_Data[ipsecSaIdx].outSequenceNumberLow == 0xffffffffU)
  {
    TcpIp_IpSecDb_Data[ipsecSaIdx].outSequenceNumberHigh++;
    TcpIp_IpSecDb_Data[ipsecSaIdx].outSequenceNumberLow = 0U;
  }
  else
  {
    TcpIp_IpSecDb_Data[ipsecSaIdx].outSequenceNumberLow++;
  }
}
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */

STATIC FUNC_P2CONST(uint32, TCPIP_APPL_DATA, TCPIP_CODE) TcpIp_IpSecDb_getRemoteIpAddr
(
  P2CONST(TcpIp_IpSecConnectionConfigType, AUTOMATIC, TCPIP_APPL_DATA) connectionConfigPtr
)
{
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteIpAddrPtr;

  if(connectionConfigPtr->addrType == TCPIP_AF_INET)
  {
    remoteIpAddrPtr =
      (TCPIP_CFG_TYPE(TcpIp_AddrInetType,
                      ipSecConnectionIpV4RemoteAddrList,
                      connectionConfigPtr->remoteIpAddressIdx)).addr;
  }
  else /* currently we only support IpV4 and IpV6 */
  {
    remoteIpAddrPtr =
      (TCPIP_CFG_TYPE(TcpIp_AddrInet6Type,
                      ipSecConnectionIpV6RemoteAddrList,
                      connectionConfigPtr->remoteIpAddressIdx)).addr;
  }
  return remoteIpAddrPtr;
}

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#endif /* (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL) */
/*==================[end of file]===========================================*/
