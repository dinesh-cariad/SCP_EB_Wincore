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
 * MISRAC2012-4) Deviated Rule: 11.3 (required)
 * A cast shall not be performed between a pointer to object type and a pointer to a different
 * object type.
 *
 * Reason:
 * 1) AUTOSAR defines a generic type which dependent on a member of the generic type
 * implements an concrete type. Thus for usage of this concrete type, a cast is necessary.
 * 2) Polymorphic (derived) data structures need an explicit pointer-to-base-class cast and
 * then another explicit pointer-to-derived-class cast in C.
 */

/*==================[inclusions]============================================*/

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Ndp_Int.h>       /* used SW-unit interface file */
#include <TcpIp_IpV6_Priv.h>     /* own SW-unit interface file */
#include <SchM_TcpIp.h>          /* SchM API declarations */
#include <TcpIp_IpSecDb_Int.h>  /* used SW-unit interface file */
/*==================[macros]================================================*/

/** \brief Macro gets the ICMP Type from ICMP-header*/
#define TCPIP_IPV6_ICMP_TYPE_GET(headerPtr)                             \
  ((headerPtr)[0U])

/** \brief Macro gets the ICMP Code from ICMP-header*/
#define TCPIP_IPV6_ICMP_CODE_GET(headerPtr)                             \
  ((headerPtr)[1U])

/** \brief Macro returns pointer to start of payload in ICMP header */
#define TCPIP_IPV6_ICMP_PAYLOAD_GET_PTR(headerPtr)                      \
  (&((headerPtr)[TCPIP_IPV6_ICMP_HEADER_LENGTH]))

/** \brief Macro sets the ICMP Type of ICMP-header*/
#define TCPIP_IPV6_ICMP_TYPE_SET(type, headerPtr)                       \
  (headerPtr)[0U] = (type)

/** \brief Macro sets the ICMP code of ICMP-header*/
#define TCPIP_IPV6_ICMP_CODE_SET(code, headerPtr)                       \
  (headerPtr)[1U] = (code)

/** \brief Macro sets the Checksum of ICMP header */
#define TCPIP_IPV6_ICMP_CHECKSUM_SET(checksum, headerPtr)               \
  TCPIP_SET16((headerPtr), 2, (checksum))

/*==================[type definitions]======================================*/

typedef struct
{
  /*  TcpIp_Ip_UpdateHeaderFunctorType polymorphic base class */
  TCPIP_IP_UPDATEHEADER_FIELDS

  /* derived data */
  /**< updateHeaderFctPtr needs the Icmp specificValue to build its header */
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) specificValue;
  /**< updateHeaderFctPtr needs the Icmp Type to build its header */
  uint8 Type;
  /**< updateHeaderFctPtr needs the Icmp Code to build its header */
  uint8 Code;
} TcpIp_IpV6_Icmp_UpdateHeaderFunctorType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#if (TCPIP_IPV6_ENABLED == STD_ON)

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief Determines whether to send Echo Reply
 *
 *  This function checks if the received ICMP message is an Echo request. And if yes, should TcpIp
 *  respond to it. Also checks if it should responds to requests destined to multicast. In case it
 *  doesn't it also checks if the address is MULTICAST.
 *
 * \param[in] localIpAddrPtr   destination address of the echo request.
 * \param[in] type             type of ICMP message
 *
 * \return Result of operation
 * \retval FALSE                   TcpIp shall not respond
 * \retval TRUE                    TcpIp shall respond
 */
STATIC FUNC(boolean, TCPIP_CODE) TcpIp_IpV6_Icmp_checkForEchoRequest
(
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) localIpAddrPtr,
  uint8 type
);

#if (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL)

/** \brief Determine if traffic shall be bypassed, secured or discarded
 *
 * If Neighbor Soliciation is sent to a Multicast address message is alawys baypassed
 * otherwise policy table is consulted.
 *
 * \param[in] ipAddrPtr               local and remote ip address
 * \param[in] icmpParameterPtr        icmp parameter e.g. code, type
 * \param[out] ipSecConnectionIdxPtr  Index of the connection
 *
 * \return Result of operation
 * \retval TCPIP_IPSECDB_MECHANISM_DISCARDED   Packet is discarded
 * \retval TCPIP_IPSECDB_MECHANISM_BYPASSED    Packet is bypassed
 * \retval TCPIP_IPSECDB_MECHANISM_SECURED     Packet is secured
 */
STATIC FUNC(uint8, TCPIP_CODE) TcpIp_IpV6_Icmp_checkSecurityTxPolicy
(
  P2VAR(TcpIp_Ip_AddrType, AUTOMATIC, TCPIP_APPL_DATA) ipAddrPtr,
  P2CONST(TcpIp_Ip_Icmp_ParameterType, AUTOMATIC, TCPIP_APPL_DATA) icmpParameterPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) ipSecConnectionIdxPtr
);

#endif /* (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)

#define TCPIP_START_SEC_VAR_INIT_32
#include <TcpIp_MemMap.h>

STATIC VAR(uint32, TCPIP_VAR) TcpIp_IpV6_Icmp_InvalidTypeCounter = 0U;

#define TCPIP_STOP_SEC_VAR_INIT_32
#include <TcpIp_MemMap.h>

#endif /* TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE ) TcpIp_IpV6_Icmp_rxIndication
(
  P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) localIpAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  TcpIp_LocalAddrIdType localAddrId,
  uint16 lenByte,
  uint8 hopLimit,
  boolean isFragmented
)
{
  /* ICMP messages are forwarded only in ONLINE state */
  if(TcpIp_IpAddrM_getIfState(localAddrId) == TCPIP_STATE_ONLINE)
  {
    /* perform ICMP header check: length */
    if(lenByte >= TCPIP_IPV6_ICMP_HEADER_LENGTH)
    {
      uint16 checksum = 0U;
      const uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
      P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteSockAddrPtr =
      /* Deviation MISRAC2012-4 */
        (P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA)) remoteAddrPtr;
      boolean calculateChecksum;
      TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);
      calculateChecksum =
      (
        (TCPIP_GET_ENABLE_OFFLOAD_CHECKSUM_ICMP(ctrl,ctrlIdx) == FALSE) ||
        (isFragmented == TRUE)
      );
      if(calculateChecksum == TRUE)

      {
        uint16  partialChecksum =
          TcpIp_IpV6_calcPartialChecksum
          (
            remoteSockAddrPtr,
            localIpAddrPtr,
            lenByte,
            TCPIP_IP_PROTOCOL_ICMPV6
          );
        checksum = TcpIp_CalcChecksum16Bit_withInitValue(dataPtr, partialChecksum, lenByte);
      }
      /* perform ICMP header check: checksum */
      if(checksum == 0U)
      {
        /* extract data form ICMP header */
        const uint8 type = TCPIP_IPV6_ICMP_TYPE_GET(dataPtr);
        const uint8 code = TCPIP_IPV6_ICMP_CODE_GET(dataPtr);
        const uint16 payloadLength = (lenByte - TCPIP_IPV6_ICMP_HEADER_LENGTH);
        CONSTP2CONST(uint8,AUTOMATIC,TCPIP_APPL_DATA) payloadPtr =
          &dataPtr[TCPIP_IPV6_ICMP_HEADER_LENGTH];

        if((type == TCPIP_IPV6_ICMP_TYPE_NEIGHBOR_SOLICIT) ||
           (type == TCPIP_IPV6_ICMP_TYPE_NEIGHBOR_ADVERT) ||
           (type == TCPIP_IPV6_ICMP_TYPE_ROUTER_ADVERT)
          )
        {
          /* handle Ndp message */
          TcpIp_Ndp_rxIndication
                (
                 type,
                 code,
                 remoteAddrPtr,
                 localAddrId,
                 payloadPtr,
                 payloadLength,
                 hopLimit
                );
        }
        else if(TcpIp_IpV6_Icmp_checkForEchoRequest(localIpAddrPtr, type))
        {
          if((payloadLength >= TCPIP_IPV6_MIN_ICMP_ECHO_REQ_LENGTH))
          {
            TcpIp_Ip_Icmp_ParameterType icmpParameter;

            icmpParameter.code = 0U;
            icmpParameter.hopLimit = TCPIP_IPV6_ICMP_DEFAULT_HOP_LIMIT;
            icmpParameter.type = TCPIP_IPV6_ICMP_TYPE_ECHO_REPLY;
            icmpParameter.specificValue = NULL_PTR;

            (void) TcpIp_IpV6_Icmp_transmit
                    (
                     payloadPtr,
                     payloadLength,
                     remoteSockAddrPtr,
                     localAddrId,
                     &icmpParameter,
                     TRUE
                    );
          }
        }
        else
        {
#if (TCPIP_ICMPV6_RX_FCT_ENABLED == STD_ON)
          /* call user configured Rx indication function and pass icmp data */
          TcpIp_IcmpV6MsgHandlerAPI
            (
              localAddrId,
              remoteSockAddrPtr,
              hopLimit,
              type,
              code,
              payloadLength,
              payloadPtr
            );
#else
#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)
            /* increment counter of dropped frames caused by invalid ICMv6 */
            /* !LINKSTO TcpIp.Design.IpV6.EA6.rxIndication,1 */
            SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_6();
            if(TcpIp_IpV6_Icmp_InvalidTypeCounter < 0xFFFFFFFFU)
            {
              /* !LINKSTO TcpIp.Design.IpV6.Atomic.GetAndResetInvalidIcmpCount,1 */
              TS_AtomicAssign32(TcpIp_IpV6_Icmp_InvalidTypeCounter,
                               (TcpIp_IpV6_Icmp_InvalidTypeCounter+1U));
            }
            /* !LINKSTO TcpIp.Design.IpV6.EA6.rxIndication,1 */
            SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_6();
#endif /* TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */
#endif /* (TCPIP_ICMPV6_RX_FCT_ENABLED == STD_ON) */
        }
      }
    }
  }
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_IpV6_Icmp_intTransmit
(
  P2VAR(TcpIp_Ip_CopyDataFunctorType, AUTOMATIC, TCPIP_APPL_DATA) copyDataFunctorPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 messageLengthIn,
  P2VAR(TcpIp_Ip_AddrType, AUTOMATIC, TCPIP_APPL_DATA) ipAddrPtr,
  uint16 remoteAddrHandle,
  P2CONST(TcpIp_Ip_Icmp_ParameterType, AUTOMATIC, TCPIP_APPL_DATA) icmpParameterPtr,
  boolean useUnspecifiedSrc,
  boolean truncate
)
{
  TcpIp_ReturnType retVal = TCPIP_E_NOT_OK;
  TcpIp_LocalAddrIdType localAddrId = ipAddrPtr->localAddrId;
  uint16 messageLength = messageLengthIn;
  uint8 ipSecSaCommonIdx = 0U;
#if (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL)
  uint8 IpsecMechanism;
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL)*/

  DBG_TCPIP_IPV6_ICMP_INTTRANSMIT_ENTRY
    (copyDataFunctorPtr,dataPtr,messageLengthIn,ipAddrPtr,remoteAddrHandle,icmpParameterPtr,useUnspecifiedSrc,truncate);

#if (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL)
  /* check policy table if traffic shall be secured, bypassed or discarded */
  IpsecMechanism = TcpIp_IpV6_Icmp_checkSecurityTxPolicy(
                                                         ipAddrPtr,
                                                         icmpParameterPtr,
                                                         &ipSecSaCommonIdx
                                                        );

  if(IpsecMechanism != TCPIP_IPSECDB_MECHANISM_DISCARDED)
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL) */
  {
    TcpIp_Ip_IpParameterType localIpParameters = TCPIP_IP_IPPARAMETER_INITIALIZER;
    uint8 headerLength = TCPIP_IPV6_ICMP_HEADER_LENGTH;
    uint8 addHeaderLength = 0U;
    uint16 maxLen;

    /* retreive maximal ip payload length and additional headers to consider */
    retVal =
       TcpIp_IpV6_getMaximumPayload
         (ipSecSaCommonIdx, localAddrId, &maxLen, &addHeaderLength);

#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)
    localIpParameters.addHeaderLength =  addHeaderLength;
    localIpParameters.ipSecSaCommonIdx = ipSecSaCommonIdx;
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */
    localIpParameters.maxIpPayloadLength = maxLen;

    if(icmpParameterPtr->specificValue != NULL_PTR)
    {
      if((0xFFFFU - messageLength) > TCPIP_IPV6_ICMP_ERR_MSG_FIXED_LENGTH)
      {
        headerLength += TCPIP_IPV6_ICMP_ERR_MSG_FIXED_LENGTH;
      }
      else
      {
        retVal = E_NOT_OK;
      }
    }

    if(retVal == E_OK)
    {
      maxLen -= TCPIP_IPV6_ICMP_HEADER_LENGTH;

      /* check if message size can be truncated, and if not message does not exceed maximal size */
      if(truncate || (messageLength <= maxLen))
      {
        if(messageLength > maxLen)
        {
          /* limit the size of a icmp message to the maximal mtu */
          messageLength = maxLen;
        }

        localIpParameters.ttl = icmpParameterPtr->hopLimit;
        localIpParameters.vlanPriority = TCPIP_IP_VLANPRIO_UNUSED;
        localIpParameters.protocol = TCPIP_IP_PROTOCOL_ICMPV6;

        {
          TcpIp_IpV6_Icmp_UpdateHeaderFunctorType updateHeaderFunctor;
          uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
          P2VAR(TcpIp_Ip_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA) updateIpHeaderFunctor =
          /* Deviation MISRAC2012-4 */
           (P2VAR(TcpIp_Ip_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA)) &updateHeaderFunctor;
          boolean checksumOffload = TCPIP_GET_ENABLE_OFFLOAD_CHECKSUM_ICMP(ctrl,ctrlIdx);

          TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

          /* These are used to forward required data to the UDP header-construction. */
          updateHeaderFunctor.Code = icmpParameterPtr->code;
          updateHeaderFunctor.Type = icmpParameterPtr->type;
          updateHeaderFunctor.specificValue = icmpParameterPtr->specificValue;
          updateHeaderFunctor.updateHeaderFctPtr = &TcpIp_IpV6_Icmp_updateHeader;

          localIpParameters.useUnspecifiedSrc = useUnspecifiedSrc;
          localIpParameters.identifier = 0U;
          localIpParameters.calculateChecksum = TCPIP_CALCULATE_CHECKSUM(TRUE, checksumOffload);

          retVal= TcpIp_Ip_transmitIp
                  (
                    copyDataFunctorPtr,
                    updateIpHeaderFunctor,
                    &localIpParameters,
                    ipAddrPtr,
                    remoteAddrHandle,
                    dataPtr,
                    headerLength,
                    messageLength
                  );
        }
      }
      else
      {
        retVal = TCPIP_E_MSGSIZE;
      }
    }
  }

  DBG_TCPIP_IPV6_ICMP_INTTRANSMIT_EXIT
    (retVal,copyDataFunctorPtr,dataPtr,messageLengthIn,ipAddrPtr,remoteAddrHandle,icmpParameterPtr,useUnspecifiedSrc,truncate);

  return retVal;
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpV6_Icmp_transmit
(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 messageLengthIn,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) destIpPtr,
  TcpIp_LocalAddrIdType localAddrIdIn,
  P2CONST(TcpIp_Ip_Icmp_ParameterType, AUTOMATIC, TCPIP_APPL_DATA) icmpParameterPtr,
  boolean truncate
)
{
  TcpIp_ReturnType retVal = TCPIP_E_NOT_OK;
  uint16 remoteAddrHandle;

  DBG_TCPIP_IPV6_ICMP_TRANSMIT_ENTRY
    (dataPtr,messageLengthIn,destIpPtr,localAddrIdIn,icmpParameterPtr,truncate);

  {
    TcpIp_Ip_AddrType ipAddress;
    TcpIp_IpAddrM_AddrInfoType addrInfo;
    uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrIdIn);
    addrInfo = TcpIp_IpAddrM_getLocalAddrInfo(localAddrIdIn);
    retVal = TCPIP_E_NOT_OK;

    ipAddress.localAddrId = localAddrIdIn;

    if(TCPIP_IPADDRM_IS_ADDRTYPE(addrInfo,MULTICAST))
    {
      TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

      /* get unicast address if message was received on a multicast, otherwise use the destination
         address as source address */
      ipAddress.localAddrId = TcpIp_IpV6_getTxLocalAddrId_byCtrlIdx(ctrlIdx, destIpPtr);
    }

    if(ipAddress.localAddrId != TCPIP_LOCALADDRID_INVALID)
    {
      if(TcpIp_IpAddrM_lockIfOnline(ipAddress.localAddrId))
      {
        retVal = TcpIp_IpV6_getRemoteAddrHandle(&remoteAddrHandle, destIpPtr, ipAddress.localAddrId);

        if(retVal == TCPIP_OK)
        {
          ipAddress.remoteAddrPtr = destIpPtr;

          retVal =
            TcpIp_IpV6_Icmp_intTransmit
              (NULL_PTR, dataPtr, messageLengthIn, &ipAddress, remoteAddrHandle, icmpParameterPtr, FALSE, truncate);

          /* unlock unicast entry if solicitation wasn't transmitted because transmit wasn't called */
          if(remoteAddrHandle != TCPIP_IPV6_NDP_REMOTE_HANDLE_MULTICAST)
          {
            TcpIp_Ndp_NCache_unlock(remoteAddrHandle,ctrlIdx);
          }
        }

        /* unlock interface */
        TcpIp_IpAddrM_unlockIf(ipAddress.localAddrId);
      }
    }
  }

  DBG_TCPIP_IPV6_ICMP_TRANSMIT_EXIT
    (retVal,dataPtr,messageLengthIn,destIpPtr,localAddrIdIn,icmpParameterPtr,truncate);

  return retVal;
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV6_Icmp_updateHeader
(
  P2VAR(TcpIp_Ip_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA) that,
  uint16 payloadSum,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) headerPtr,
  boolean calculateChecksum
)
{
  P2VAR(TcpIp_IpV6_Icmp_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA) this=
  /* Deviation MISRAC2012-4 */
    (P2VAR(TcpIp_IpV6_Icmp_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA)) that;

  uint16 checksum = 0U;
  uint8 headerLength = TCPIP_IPV6_ICMP_HEADER_LENGTH;

  DBG_TCPIP_IPV6_ICMP_UPDATEHEADER_ENTRY(that, payloadSum, headerPtr, calculateChecksum);

  TCPIP_IPV6_ICMP_TYPE_SET(this->Type, headerPtr);
  TCPIP_IPV6_ICMP_CODE_SET(this->Code, headerPtr);
  TCPIP_IPV6_ICMP_CHECKSUM_SET((uint16)0x0U, headerPtr);

  if(this->specificValue != NULL_PTR)
  {
    /* set specific value */
    TCPIP_SET32(headerPtr, 4, *(this->specificValue));
    headerLength += TCPIP_IPV6_ICMP_ERR_MSG_FIXED_LENGTH;
  }

  /* calculate header checksum and add payload checksum */
  if(calculateChecksum)
  {
    checksum = TcpIp_CalcChecksum16Bit_withInitValue(headerPtr,payloadSum,headerLength);

    if(checksum == 0x0U)  /* as per rfc 768 pg 2 */
    {
      checksum = 0xFFFFU;
    }
  }

  TCPIP_IPV6_ICMP_CHECKSUM_SET(checksum, headerPtr);

  DBG_TCPIP_IPV6_ICMP_UPDATEHEADER_EXIT(that, payloadSum, headerPtr, calculateChecksum);
}

#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)
TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV6_GetAndResetInvalidIcmpCount
(
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) MeasurementDataPtr
)
{
  if(MeasurementDataPtr != NULL_PTR)
  {
    /* !LINKSTO TcpIp.Design.IpV6.Atomic.GetAndResetInvalidIcmpCount,1 */
    TS_AtomicAssign32(*MeasurementDataPtr, TcpIp_IpV6_Icmp_InvalidTypeCounter);
  }
  if(MeasurementResetNeeded)
  {
    /* !LINKSTO TcpIp.Design.IpV6.Atomic.GetAndResetInvalidIcmpCount,1 */
    TS_AtomicAssign32(TcpIp_IpV6_Icmp_InvalidTypeCounter, 0U);
  }

}
#endif /* TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */

/*==================[internal function definitions]=========================*/

#if (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL)

STATIC FUNC(uint8, TCPIP_CODE) TcpIp_IpV6_Icmp_checkSecurityTxPolicy
(
  P2VAR(TcpIp_Ip_AddrType, AUTOMATIC, TCPIP_APPL_DATA) ipAddrPtr,
  P2CONST(TcpIp_Ip_Icmp_ParameterType, AUTOMATIC, TCPIP_APPL_DATA) icmpParameterPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) ipSecConnectionIdxPtr
)
{
  /* Deviation MISRAC2012-4 */
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteIpAddrPtr =
    ((P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TCPIP_APPL_DATA)) ipAddrPtr->remoteAddrPtr)->addr;
  uint8 mechanism = TCPIP_IPSECDB_MECHANISM_DISCARDED;

  if((icmpParameterPtr->code == 0U) &&
     (icmpParameterPtr->type == TCPIP_IPV6_ICMP_TYPE_NEIGHBOR_SOLICIT) &&
     (TCPIP_IPV6_ADDR_IS_MULTICAST(remoteIpAddrPtr))
    )
  {
    /* bypass all neighbor solicitations which are sent to a multicast address,
     * if messages are allowed to be sent is checked in upper layer protocol
     * e.g. UDP, TCP
     */
    *ipSecConnectionIdxPtr = TCPIP_IPSECDB_INVALID_SA_INDEX;
    mechanism = TCPIP_IPSECDB_MECHANISM_BYPASSED;
  }
  else
  {
    /* check policy table if traffic shall be secured, bypassed or discarded */
    mechanism = TcpIp_IpSecDb_checkSecurityTxPolicy(
                                                    ipAddrPtr->localAddrId,
                                                    ipAddrPtr->remoteAddrPtr,
                                                    TCPIP_IP_PROTOCOL_ICMPV6,
                                                    0U,
                                                    ipSecConnectionIdxPtr
                                                   );
  }

  return mechanism;
}

#endif /* (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL) */

STATIC FUNC(boolean, TCPIP_CODE) TcpIp_IpV6_Icmp_checkForEchoRequest
(
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) localIpAddrPtr,
  uint8 type
)
{
  boolean retVal;

  retVal = FALSE;

  if((type == TCPIP_IPV6_ICMP_TYPE_ECHO_REQUEST) && TCPIP_IPV6_ICMP_ECHO_REPLY_IS_ENABLED)
  {
    if(TCPIP_IPV6_ICMP_ECHO_REPLY_TO_MULTICAST_IS_ENABLED)
    {
      retVal = TRUE;
    }
    else
    {
      if(!TCPIP_IPV6_ADDR_IS_MULTICAST(localIpAddrPtr))
      {
        retVal = TRUE;
      }
    }
  }

  return retVal;
}

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */

/*==================[end of file]===========================================*/
