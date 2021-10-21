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
 * MISRAC2012-1) Deviated Rule: 11.3 (required)
 * A cast shall not be performed between a pointer to object type and a pointer to a different
 * object type.
 *
 * Reason:
 * 1) AUTOSAR defines a generic type which dependent on a member of the generic type
 * implements an concrete type. Thus for usage of this concrete type, a cast is necessary.
 * 2) Polymorphic (derived) data structures need an explicit pointer-to-base-class cast and
 * then another explicit pointer-to-derived-class cast in C.
 *
 * MISRAC2012-2) Deviated Rule: 8.9 (Advisory)
 * An object should be defined at block scope if its identifier only appears in a single function.
 *
 * Reason:
 * The AUTOSAR memory mapping requires that functions are mapped in SEC_CODE memory sections.
 * Objects at block scope require a different memory mapping, e.g. to a SEC_VAR section,
 * which leads to nested memory sections, which is not supported by some compilers.
 *
 */

/*==================[inclusions]============================================*/

#include <TSMem.h>              /* TS_MemCpy(), TS_MemSet() */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_IpAddrM_Int.h>  /* used SW-unit interface file */
#include <TcpIp_IpSec_Int.h>    /* used SW-unit interface file */
#include <TcpIp_IpSecDb_Int.h>  /* used SW-unit interface file */
#include <TcpIp_IpV4_Priv.h>    /* own SW-unit private header file; must be last */

/*==================[macros]================================================*/

/** \brief Macro gets the ICMP Type from ICMP-header*/
#define TCPIP_IPV4_ICMP_TYPE_GET(headerPtr) ((headerPtr)[0])

/** \brief Macro sets the ICMP Type of ICMP-header*/
#define TCPIP_IPV4_ICMP_TYPE_SET(type, headerPtr) \
    do { \
    (headerPtr)[0] = (type); \
    }while(0) \

/** \brief Macro gets the ICMP Code from ICMP-header*/
#define TCPIP_IPV4_ICMP_CODE_GET(headerPtr) ((headerPtr)[1])

/** \brief Macro sets the ICMP code of ICMP-header*/
#define TCPIP_IPV4_ICMP_CODE_SET(code, headerPtr) \
    do { \
    (headerPtr)[1] = (code); \
    }while(0) \

/** \brief Macro sets the Checksum of ICMP header */
#define TCPIP_IPV4_ICMP_CHECKSUM_SET(checksum,headerPtr) \
    do { \
    (headerPtr)[2] = (uint8)(((checksum)>>8U)&0xFFU); \
    (headerPtr)[3] = (uint8)((checksum)&0xFFU);       \
    }while(0) \


/** \brief Macro returns pointer to start of the HW source address of ARP header*/
#define TCPIP_IPV4_ICMP_PAYLOAD_GET_PTR(headerPtr) (&((headerPtr)[TCPIP_IPV4_ICMP_HEADER_LENGTH]))

#define TCPIP_IPV4_ICMP_SPECIFIC_VALUE_LENGTH   4U

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
} TcpIp_Icmp_UpdateHeaderFunctorType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#if (TCPIP_ICMPV4_ENABLED == STD_ON)

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief Check if Icmp Echo Reply shall be sent
 *
 * \param[in] type          Icmp Type of the received message
 * \param[in] localAddrId   Local address the frame has been received on.
 * \param[in] isBroadcast   Indicates if the target MAC address is a broadcast address
 *
 * \retval TRUE   Icmp Echo Reply shall be transmitted
 * \retval FALSE  Call IcmpMsgHandler
 */
STATIC FUNC(boolean, TCPIP_CODE) TcpIp_IpV4_icmptransitEchoReply
(
  uint8 type,
  TcpIp_LocalAddrIdType localAddrId,
  boolean isBroadcast
);

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/
#if (TCPIP_ICMPV4_ENABLED == STD_ON)
#if ( TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF )
#define TCPIP_START_SEC_VAR_INIT_32
#include <TcpIp_MemMap.h>

/** \brief  Fragment Identification: always enabled & always supplied to a packet, unless IPdontFragment is set. */
/* Deviation MISRAC2012-2 */
STATIC VAR(uint32, TCPIP_VAR) TcpIp_IpV4_icmpFragmentIdCounter = 0;

#define TCPIP_STOP_SEC_VAR_INIT_32
#include <TcpIp_MemMap.h>
#endif /* ( TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF ) */
#endif /* (TCPIP_ICMPV4_ENABLED == STD_ON) */

/*==================[external function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE ) TcpIp_IpV4_rxIndicationIcmp
(
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  TcpIp_LocalAddrIdType localAddrId,
  uint16 lenByte,
  uint8 ttl,
  boolean isBroadcast,
  boolean isFragmented
)
{
#if (TCPIP_ICMPV4_RX_FCT_ENABLED == STD_OFF)
  TS_PARAM_UNUSED(ttl);
#endif /* (TCPIP_ICMPV4_RX_FCT_ENABLED == STD_ON) */

  /* ICMP messages are forwarded only in ONLINE state */
  if(TcpIp_IpAddrM_getIfState(localAddrId) == TCPIP_STATE_ONLINE)
  {
    const uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
    uint8 ICMP_hardware_checksum;
    TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);
    ICMP_hardware_checksum = TCPIP_GET_ENABLE_OFFLOAD_CHECKSUM_ICMP(ctrl,ctrlIdx);

    /* perform ICMP header length check */
    if(lenByte >= TCPIP_IPV4_ICMP_HEADER_LENGTH)
    {
      uint16 checksum = 0U;
      if((ICMP_hardware_checksum == FALSE) || (isFragmented == TRUE))
      {
        checksum = TcpIp_CalcChecksum16Bit(dataPtr,lenByte);
      }
      /* perform ICMP header check: checksum */
      if(checksum == 0x0U)
      {
        /* extract data form ICMP header */
        const uint8 type = TCPIP_IPV4_ICMP_TYPE_GET(dataPtr);
        const uint16 payloadLength = (lenByte - TCPIP_IPV4_ICMP_HEADER_LENGTH);
        CONSTP2CONST(uint8,AUTOMATIC,TCPIP_APPL_DATA) payloadPtr =
          &dataPtr[TCPIP_IPV4_ICMP_HEADER_LENGTH];

        if(TcpIp_IpV4_icmptransitEchoReply(type, localAddrId, isBroadcast))
        {
          TcpIp_Ip_Icmp_ParameterType icmpParameter;

          icmpParameter.code = 0u;
          icmpParameter.hopLimit = TcpIp_PBcfgPtr->ipV4Config.icmpTtl;
          icmpParameter.type = TCPIP_IPV4_ICMP_TYPE_ECHO_REPLY;
          icmpParameter.specificValue = NULL_PTR;

          /* if type is Icmp Echo Request is received, Icmp Echo Reply is enabled and
           * localAddrId is a unicast sent Icmp Echo Reply */
          (void)TcpIp_IpV4_icmpTransmit(
              payloadPtr,
              payloadLength,
              remoteAddrPtr,
              localAddrId,
              &icmpParameter,
              TRUE);
        }
#if (TCPIP_ICMPV4_RX_FCT_ENABLED == STD_ON)
        else
        {
          /* call user configured Rx indication function and pass icmp data */
          TcpIp_IcmpMsgHandlerAPI
            (
              localAddrId,
              remoteAddrPtr,
              ttl,
              type,
              TCPIP_IPV4_ICMP_CODE_GET(dataPtr),
              payloadLength,
              payloadPtr
            );
        }
#endif /* (TCPIP_ICMPV4_RX_FCT_ENABLED == STD_ON) */
      }
    }
  }
}


TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_IpV4_icmpTransmit
(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 lenByte,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) destIpPtr,
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(TcpIp_Ip_Icmp_ParameterType, AUTOMATIC, TCPIP_APPL_DATA) icmpParameterPtr,
  boolean truncate
)
{
  TcpIp_ReturnType retVal = E_NOT_OK;
  uint8 ipSecSaCommonIdx = TCPIP_IPSECDB_INVALID_SA_INDEX;
#if (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL)
  uint8 IpsecMechanism;
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL) */

  DBG_TCPIP_IPV4_ICMPTRANSMIT_ENTRY(dataPtr,lenByte,destIpPtr,localAddrId,icmpParameterPtr,truncate);

#if (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL)
  /* check policy table if traffic shall be secured, bypassed or discarded */
  IpsecMechanism =
     TcpIp_IpSecDb_checkSecurityTxPolicy
       (localAddrId, destIpPtr, TCPIP_IP_PROTOCOL_ICMP, 0U, &ipSecSaCommonIdx);

  if(IpsecMechanism != TCPIP_IPSECDB_MECHANISM_DISCARDED)
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL) */
  {
    /* transmit only in ONLINE state */
    if(TcpIp_IpAddrM_lockIfOnline(localAddrId))
    {
      uint16 messageLength = TCPIP_IPV4_ICMP_HEADER_LENGTH + lenByte;
      uint16 headerLength = TCPIP_IPV4_ICMP_HEADER_LENGTH;
      uint8 addHeaderLength = 0U;
      uint16 maxLen;
      TcpIp_Ip_IpParameterType localIpParameters = TCPIP_IP_IPPARAMETER_INITIALIZER;

      /* retrieve maximal ip payload length and additional headers to consider */
      retVal =
         TcpIp_IpV4_getMaximumPayload
           (ipSecSaCommonIdx, localAddrId, &maxLen, &addHeaderLength);

      if(retVal == E_OK)
      {
#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)
        localIpParameters.ipSecSaCommonIdx = ipSecSaCommonIdx;
        localIpParameters.addHeaderLength = addHeaderLength;
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */

        localIpParameters.maxIpPayloadLength = maxLen;
        localIpParameters.ttl = icmpParameterPtr->hopLimit;
        localIpParameters.vlanPriority = TCPIP_IP_VLANPRIO_UNUSED;
        localIpParameters.protocol = TCPIP_IP_PROTOCOL_ICMP;
        localIpParameters.calculatePseudoChecksum = FALSE;

        if(icmpParameterPtr->specificValue != NULL_PTR)
        {
          messageLength += TCPIP_IPV4_ICMP_SPECIFIC_VALUE_LENGTH;
          headerLength += TCPIP_IPV4_ICMP_SPECIFIC_VALUE_LENGTH;
        }

        /* check if message size can be truncated, and if not message does not exceed maximal size */
        if(truncate || (messageLength <= maxLen))
        {
          if(messageLength > maxLen)
          {
            /* limit the size of a icmp message to the maximal mtu */
            messageLength = maxLen;
          }

          {
            TcpIp_Icmp_UpdateHeaderFunctorType updateHeaderFunctor;
            uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
            P2VAR(TcpIp_Ip_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA) updateIpHeaderFunctor =
            /* Deviation MISRAC2012-1 */
              (P2VAR(TcpIp_Ip_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA)) &updateHeaderFunctor;
            boolean checksumOffload = TCPIP_GET_ENABLE_OFFLOAD_CHECKSUM_ICMP(ctrl,ctrlIdx);

            TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

            /* These are used to forward required data to the UDP header-construction. */
            updateHeaderFunctor.Code = icmpParameterPtr->code;
            updateHeaderFunctor.Type = icmpParameterPtr->type;
            updateHeaderFunctor.specificValue = icmpParameterPtr->specificValue;
            updateHeaderFunctor.updateHeaderFctPtr = &TcpIp_IpV4_Icmp_updateHeader;

            localIpParameters.calculateChecksum = TCPIP_CALCULATE_CHECKSUM(TRUE, checksumOffload);
#if (TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF )
            TcpIp_IpV4_icmpFragmentIdCounter++;
            localIpParameters.identifier = TcpIp_IpV4_icmpFragmentIdCounter;
#endif /* (TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF ) */

            retVal= TcpIp_Ip_transmit
                    (
                      NULL_PTR,
                      updateIpHeaderFunctor,
                      &localIpParameters,
                      localAddrId,
                      destIpPtr,
                      dataPtr,
                      headerLength,
                      messageLength - headerLength
                    );
          }
        }
        else
        {
          retVal = TCPIP_E_MSGSIZE;
        }
      }

      /* unlock interface */
      TcpIp_IpAddrM_unlockIf(localAddrId);
    }
  }

  DBG_TCPIP_IPV4_ICMPTRANSMIT_EXIT(retVal,dataPtr,lenByte,destIpPtr,localAddrId,icmpParameterPtr,truncate);

  return retVal;
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpV4_Icmp_updateHeader
(
  P2VAR(TcpIp_Ip_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA) that,
  uint16 payloadSum,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) headerPtr,
  boolean calculateChecksum
)
{
  P2VAR(TcpIp_Icmp_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA) this=
  /* Deviation MISRAC2012-1 */
    (P2VAR(TcpIp_Icmp_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA)) that;

  uint16 checksum = 0U;
  uint8 headerLength = TCPIP_IPV4_ICMP_HEADER_LENGTH;

  DBG_TCPIP_IPV4_ICMP_UPDATEHEADER_ENTRY(that, payloadSum, headerPtr, calculateChecksum);

  TCPIP_IPV4_ICMP_TYPE_SET(this->Type, headerPtr);
  TCPIP_IPV4_ICMP_CODE_SET(this->Code, headerPtr);
  TCPIP_IPV4_ICMP_CHECKSUM_SET(checksum, headerPtr);

  if(this->specificValue != NULL_PTR)
  {
    /* set specific value */
    TCPIP_SET32(headerPtr, 4, *(this->specificValue));
    headerLength += TCPIP_IPV4_ICMP_SPECIFIC_VALUE_LENGTH;
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

  TCPIP_IPV4_ICMP_CHECKSUM_SET(checksum, headerPtr);

  DBG_TCPIP_IPV4_ICMP_UPDATEHEADER_EXIT(that, payloadSum, headerPtr, calculateChecksum);
}

#if(TCPIP_DEV_ERROR_DETECT == STD_ON)

#endif /*(TCPIP_DEV_ERROR_DETECT == STD_ON)*/
/*==================[internal function definitions]=========================*/

STATIC FUNC(boolean, TCPIP_CODE) TcpIp_IpV4_icmptransitEchoReply
(
  uint8 type,
  TcpIp_LocalAddrIdType localAddrId,
  boolean isBroadcast
)
{
  boolean transmit = FALSE;

  if(type == TCPIP_IPV4_ICMP_TYPE_ECHO)
  {
    /* if we received an ECHO message, send a ECHO reply, if enabled */
    if(TcpIp_PBcfgPtr->ipV4Config.icmpEchoReplyEnable != FALSE)
    {
      const TcpIp_IpAddrM_AddrInfoType addrInfo = TcpIp_IpAddrM_getLocalAddrInfo(localAddrId);

      /* send the response only, if the destination address of the ECHO was no
         broadcast/multicast address (either IP or physical) */
      if(TCPIP_IPADDRM_IS_ADDRTYPE(addrInfo,UNICAST) && (!isBroadcast))
      {
        transmit = TRUE;
      }
    }
  }

  return transmit;
}

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#else
  TS_PREVENTEMPTYTRANSLATIONUNIT
#endif /* (TCPIP_ICMPV4_ENABLED == STD_ON) */

/*==================[end of file]===========================================*/
