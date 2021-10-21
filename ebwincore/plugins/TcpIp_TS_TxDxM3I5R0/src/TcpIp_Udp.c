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
 */

/*==================[inclusions]============================================*/

#include <TSMem.h>              /* TS_MemCpy(), TS_MemSet() */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Socket_Int.h>   /* used SW-unit interface file */
#include <TcpIp_Ip_Int.h>       /* used SW-unit interface file */
#include <TcpIp_IpAddrM_Int.h>  /* used SW-unit interface file */
#include <TcpIp_Udp_Int.h>      /* own SW-unit interface file */
#include <TcpIp_Trace.h>        /* debug and trace macros; must be last */
#include <SchM_TcpIp.h>         /* SchM API declarations */
#include <TcpIp_IpSecDb_Int.h>  /* external IpSec Api declarations*/

/*==================[macros]================================================*/

/** \brief TcpIp_UdpTransmit() service ID */
#define TCPIP_SID_UDPTRANSMIT                   0x12U

/** \brief Udp header length in units of bytes */
#define TCPIP_UDP_HEADER_LENGTH                 8U

#define TCPIP_UDP_PAYLOAD_PTR_GET(headerPtr)   &((headerPtr)[TCPIP_UDP_HEADER_LENGTH])

#define TCPIP_UDP_SRC_PORT_GET(headerPtr)\
    TCPIP_GET16(headerPtr, 0)

#define TCPIP_UDP_DEST_PORT_GET(headerPtr)\
    TCPIP_GET16(headerPtr, 2)

#define TCPIP_UDP_LENGTH_GET(headerPtr)\
    TCPIP_GET16(headerPtr, 4)

#define TCPIP_UDP_CHECKSUM_GET(headerPtr)\
    TCPIP_GET16(headerPtr, 6)

#define TCPIP_UDP_SRC_PORT_SET(headerPtr,port) \
  TCPIP_SET16(headerPtr, 0, port)

#define TCPIP_UDP_DEST_PORT_SET(headerPtr,port) \
  TCPIP_SET16(headerPtr, 2, port)

#define TCPIP_UDP_LENGTH_SET(headerPtr,length) \
  TCPIP_SET16(headerPtr, 4, length)

#define TCPIP_UDP_CHECKSUM_SET(headerPtr,checksum) \
  TCPIP_SET16(headerPtr, 6, checksum)

#define TCPIP_UDP_MAYFRAGMENT            TcpIp_PBcfgPtr->udpConfig.mayFragment
/*==================[type definitions]======================================*/

#define TCPIP_UDP_UPDATEHEADER_INITIALIZER { &TcpIp_Udp_updateHeader, 0u, 0u, 0U }

typedef struct
{
  /*  TcpIp_Ip_UpdateHeaderFunctorType polymorphic base class */
  TCPIP_IP_UPDATEHEADER_FIELDS

  /* derived data */
  uint16  length;        /**< Length of UDP header plus UDP payload */
  uint16  udpSrcPort;    /**< updateHeaderFctPtr needs the src port to build its header */
  uint16  udpDstPort;    /**< updateHeaderFctPtr needs the dst port to build its header */
} TcpIp_Udp_UpdateHeaderFunctorType;

typedef struct
{
  /*  TcpIp_Ip_CopyDataFunctorType polymorphic base class */
  TCPIP_IP_COPYDATA_FIELDS

  /* derived data */
  TcpIp_CopyTxDataFpType copyTxDataFctPtr;

} TcpIp_Udp_CopyDataFunctorType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#if ( TCPIP_UDP_ENABLED == STD_ON )

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Udp_transmit
(
  TcpIp_SocketIdType socketId,
  uint8 ipSecSaCommonIdx,
  TcpIp_Socket_Generic_PtrType socketPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  TcpIp_LocalAddrIdType localAddrId,
  uint16 udpSrcPort,
  uint16 totalLength
);

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Udp_checkIfOnline
(
  TcpIp_Socket_Generic_PtrType socketPtr,
  TcpIp_LocalAddrIdType localAddrId
);

#endif /* ( TCPIP_UDP_ENABLED == STD_ON ) */

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/
#if ( TCPIP_UDP_ENABLED == STD_ON )
#define TCPIP_START_SEC_VAR_INIT_32
#include <TcpIp_MemMap.h>

/** \brief  Fragment Identification: always enabled & always supplied to a packet,
            unless IP's dontFragment is set. */
TCPIP_IP_DEFINEFRAGIDCOUNTER(TcpIp_Udp_fragmentIdCounter)

#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)
STATIC VAR(uint32, TCPIP_VAR) TcpIp_Udp_InvalidSocketCounter = 0U;
#endif /* TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */

#define TCPIP_STOP_SEC_VAR_INIT_32
#include <TcpIp_MemMap.h>
#endif /* ( TCPIP_UDP_ENABLED == STD_ON ) */

/*==================[external function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>


FUNC( TcpIp_ReturnType, TCPIP_CODE ) TcpIp_UdpTransmit
(
  TcpIp_SocketIdType  socketId,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  uint16 totalLength
)
{
  TcpIp_ReturnType retVal = TCPIP_E_NOT_OK;

  DBG_TCPIP_UDPTRANSMIT_ENTRY(socketId,dataPtr,remoteAddrPtr,totalLength);
#if ( TCPIP_UDP_ENABLED == STD_ON )
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
  if( TCPIP_UNINIT )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_UDPTRANSMIT, TCPIP_E_NOTINIT);
  }
  else if( remoteAddrPtr == NULL_PTR)
  {
      /* In case of UDP (connection-less) we need a remote address */
    TCPIP_REPORT_ERROR(TCPIP_SID_UDPTRANSMIT, TCPIP_EB_E_PARAM_POINTER);
  }
  else if( TcpIp_Socket_checkSocketProtocol(socketId,UDP) != E_OK)
  {
    /* No UDP socket */
    TCPIP_REPORT_ERROR(TCPIP_SID_UDPTRANSMIT, TCPIP_E_PROTOTYPE);
  }
  else if( TCPIP_IP_IF_V4_V6_BOTH((remoteAddrPtr->domain != TCPIP_AF_INET),
                                  (remoteAddrPtr->domain != TCPIP_AF_INET6),
                                  (TcpIp_Socket_checkDomain(socketId, remoteAddrPtr->domain) != E_OK)
                                 )
         )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_UDPTRANSMIT, TCPIP_E_AFNOSUPPORT);
  }
  else if( TcpIp_Socket_checkSocketId(socketId) != E_OK)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_UDPTRANSMIT, TCPIP_E_INV_ARG);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
    TcpIp_LocalAddrIdType localAddrId = (TcpIp_LocalAddrIdType)0U;
    uint16 udpSrcPort= 0U;

    retVal= TcpIp_Socket_autoBindUdp(remoteAddrPtr, &localAddrId, &udpSrcPort, socketId);

    /* verify that local address is in range */
    TCPIP_PRECONDITION_ASSERT(localAddrId < TcpIp_PBcfgPtr->totalLocalAddrNum, TCPIP_INTERNAL_API_ID);

    if(retVal == TCPIP_OK)
    {
      uint8 ipSecSaCommonIdx = TCPIP_IPSECDB_INVALID_SA_INDEX;
#if (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL)
      uint8 IpsecMechanism;
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL) */

      TcpIp_Socket_Generic_PtrType socketPtr = TcpIp_Socket_getSocketPtr(socketId);

#if (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL)
      IpsecMechanism = TcpIp_IpSecDb_checkSecurityTxPolicy(
                                                           localAddrId,
                                                           remoteAddrPtr,
                                                           TCPIP_IP_PROTOCOL_UDP,
                                                           udpSrcPort,
                                                           &ipSecSaCommonIdx
                                                         );

      retVal = TCPIP_E_NOT_OK;

      if(IpsecMechanism != TCPIP_IPSECDB_MECHANISM_DISCARDED)
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL) */
      {
        if(TcpIp_Udp_checkIfOnline(socketPtr, localAddrId) == TCPIP_OK)
        {
          retVal = TcpIp_Udp_transmit
                        (
                         socketId,
                         ipSecSaCommonIdx,
                         socketPtr,
                         dataPtr,
                         remoteAddrPtr,
                         localAddrId,
                         udpSrcPort,
                         totalLength
                        );

          TcpIp_IpAddrM_unlockIf(localAddrId);
        }
        else
        {
          /* interface is not online */
          retVal= TCPIP_E_NOT_OK;
        }
      } /*if(TcpIp_Socket_autoBindUdp(... */
    }
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
    else
    {
      TCPIP_REPORT_ERROR(TCPIP_SID_UDPTRANSMIT, TCPIP_E_ADDRNOTAVAIL);
    }
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  }
#else /* ( TCPIP_UDP_ENABLED == STD_ON ) */
  TS_PARAM_UNUSED(socketId);
  TS_PARAM_UNUSED(dataPtr);
  TS_PARAM_UNUSED(remoteAddrPtr);
  TS_PARAM_UNUSED(totalLength);
#endif /* ( TCPIP_UDP_ENABLED == STD_ON ) */
  DBG_TCPIP_UDPTRANSMIT_EXIT(retVal,socketId,dataPtr,remoteAddrPtr,totalLength);
  return retVal;
} /* eof - TcpIp_UdpTransmit */


/*==================[internal function definitions]=========================*/
#if ( TCPIP_UDP_ENABLED == STD_ON )

TS_MOD_PRIV_DEFN FUNC(BufReq_ReturnType, TCPIP_CODE) TcpIp_Udp_copyData
(
    P2VAR(TcpIp_Ip_CopyDataFunctorType, AUTOMATIC, TCPIP_APPL_DATA) that,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) bufferPtr,
    uint16 copyLength
)
{
  /* Deviation MISRAC2012-1 */
  P2VAR(TcpIp_Udp_CopyDataFunctorType, AUTOMATIC, TCPIP_APPL_DATA) this =
    (P2VAR(TcpIp_Udp_CopyDataFunctorType, AUTOMATIC, TCPIP_APPL_DATA)) that;

  return this->copyTxDataFctPtr(this->handleId, bufferPtr, copyLength);
}


TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Udp_updateHeader
(
  P2VAR(TcpIp_Ip_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA) that,
  uint16 payloadSum,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) headerPtr,
  boolean calculateChecksum
)
{
  /* Deviation MISRAC2012-1 */
  P2VAR(TcpIp_Udp_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA) this=
    (P2VAR(TcpIp_Udp_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA)) that;

  uint16 checksum = 0U;

  DBG_TCPIP_UDP_UPDATEHEADER_ENTRY(that, payloadSum, headerPtr, calculateChecksum);

  /* for UDP, we can construct the whole header here and avoid separate callbacks from the IP
   * fragmentation code to first construct everything but the checksum and then add the checksum */
  TCPIP_UDP_SRC_PORT_SET(headerPtr, this->udpSrcPort);
  TCPIP_UDP_DEST_PORT_SET(headerPtr, this->udpDstPort);
  TCPIP_UDP_LENGTH_SET(headerPtr, this->length);
  TCPIP_UDP_CHECKSUM_SET(headerPtr, 0u);

  if(calculateChecksum)
  {
    checksum = TcpIp_CalcChecksum16Bit_withInitValue(headerPtr,payloadSum,TCPIP_UDP_HEADER_LENGTH);

    if(checksum == 0x0U)  /* as per rfc 768 pg 2 */
    {
      checksum = 0xFFFFU;
    }
  }

  TCPIP_UDP_CHECKSUM_SET(headerPtr,checksum);

  DBG_TCPIP_UDP_UPDATEHEADER_EXIT(that, payloadSum, headerPtr, calculateChecksum);
}



TS_MOD_PRIV_DEFN FUNC(TcpIp_Ip_RxReturnType, TCPIP_CODE ) TcpIp_Udp_rxIndication
(
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) localIpAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  TcpIp_LocalAddrIdType localAddrId,
  uint16 lenByte,
  boolean isFragmented
)
{
  TcpIp_Ip_RxReturnType retVal= TCPIP_IP_RX_NOT_OK;
  /* Deviation MISRAC2012-1 */
  P2VAR(TcpIp_SockAddrInetType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrInetPtr =
    (P2VAR(TcpIp_SockAddrInetType, AUTOMATIC, TCPIP_APPL_DATA))remoteAddrPtr;

  DBG_TCPIP_UDP_RXINDICATION_ENTRY(remoteAddrPtr,localIpAddrPtr,dataPtr,localAddrId,lenByte,isFragmented);
  if(TcpIp_IpAddrM_getIfState(localAddrId) == TCPIP_STATE_ONLINE)
  {
    if(lenByte >= TCPIP_UDP_HEADER_LENGTH)
    {
      uint16 const length = TCPIP_UDP_LENGTH_GET(dataPtr);

      if((length >= TCPIP_UDP_HEADER_LENGTH) && (lenByte >= length))
      {
        uint16 const rxChecksum = TCPIP_UDP_CHECKSUM_GET(dataPtr);
        uint16 checksum = 0xFFFFU;
        const uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
        boolean calculateChecksumOffload;
        TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);
        calculateChecksumOffload =
        (
          (TCPIP_GET_ENABLE_OFFLOAD_CHECKSUM_UDP(ctrl,ctrlIdx) == FALSE) ||
          (isFragmented == TRUE)
        );
        if(calculateChecksumOffload == TRUE)
        {
          if(rxChecksum != 0x0U)
          {
            checksum =
              TcpIp_CalcChecksum16Bit_withInitValue
              (
                dataPtr,
                TcpIp_Ip_calcPartialChecksum
                  (remoteAddrPtr, localIpAddrPtr, lenByte, TCPIP_IP_PROTOCOL_UDP),
                length
              );
          }
        }
        else
        {
          /* Checksum is calculated and verified in hardware */
          checksum = 0U;
        }

        /* either checksum shall not be used or the calculated checksum is correct */
        if((rxChecksum == 0x0U) || (checksum == 0U))
        {
          TcpIp_SocketIdType socketIds[TCPIP_UDP_MAXIMUM_SOCKET_INSTANCES] = {0U};
          uint16 const destPort = TCPIP_UDP_DEST_PORT_GET(dataPtr);
          uint8 const numSocketsFound =
              TcpIp_Socket_lookupUdp
                (
                    TCPIP_UDP_MAXIMUM_SOCKET_INSTANCES,
                    destPort,
                    localAddrId,
                    localIpAddrPtr,
                    remoteAddrInetPtr->domain,
                    socketIds
                );

          if(numSocketsFound > 0U)
          {
            uint8_least i;
            remoteAddrInetPtr->port = TCPIP_UDP_SRC_PORT_GET(dataPtr);

            /* perform Rx-Indications to upper layers */
            for(i = 0U; i < (uint8_least)numSocketsFound; i++)
            {
              TcpIp_Socket_Generic_PtrType const socketPtr= TcpIp_Socket_getSocketPtr(socketIds[i]);
              if(!calculateChecksumOffload ||
                 /* Deviation MISRAC2012-1 */
                 ((!((socketPtr->aligned.ipParameters.calculateChecksum == TRUE) &&
                   (rxChecksum == 0x0U))))
                )
              {

                TCPIP_PRECONDITION_ASSERT
                  ((socketPtr->aligned.sockOwnerId < TcpIp_NumSockOwner),TCPIP_INTERNAL_API_ID);
                /* either checksum shall not be used or the calculated checksum is correct */

                TcpIp_RxIndicationAPI[socketPtr->aligned.sockOwnerId]
                  (
                    socketIds[i],
                    remoteAddrPtr,
                    TCPIP_UDP_PAYLOAD_PTR_GET(dataPtr),
                    length-TCPIP_UDP_HEADER_LENGTH
                  );
              }
              retVal = TCPIP_IP_RX_OK;
            }
           }
           else
           {
#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)
             /* !LINKSTO TcpIp.Design.Udp.EA6.rxIndication,1 */
             SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_6();
             if(TcpIp_Udp_InvalidSocketCounter < 0xFFFFFFFFU)
             {
               /* !LINKSTO TcpIp.Design.Udp.Atomic.InvalidSocketCounter,1 */
               TS_AtomicAssign32(TcpIp_Udp_InvalidSocketCounter, (TcpIp_Udp_InvalidSocketCounter+1U));
             }
             /* !LINKSTO TcpIp.Design.Udp.EA6.rxIndication,1 */
             SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_6();
#endif /* TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */
             retVal = TCPIP_IP_RX_PORT_UNREACHABLE;
         }
        }
      }
    }
  }

  DBG_TCPIP_UDP_RXINDICATION_EXIT(retVal,remoteAddrPtr,localIpAddrPtr,dataPtr,localAddrId,lenByte,isFragmented);

  return retVal;
}


TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Udp_close
(
    TcpIp_SocketIdType socketId,
    uint8 socketOwnerId
)
{
  DBG_TCPIP_UDP_CLOSE_ENTRY(socketId, socketOwnerId);

  TCPIP_PRECONDITION_ASSERT
    ((socketOwnerId < TcpIp_NumSockOwner),TCPIP_INTERNAL_API_ID);

  TcpIp_TcpIpEventAPI[socketOwnerId](socketId,TCPIP_UDP_CLOSED);

  DBG_TCPIP_UDP_CLOSE_EXIT(socketId, socketOwnerId);
}


TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Udp_changeSocketToDefault
(
    P2VAR(TcpIp_Socket_UDP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  DBG_TCPIP_UDP_CHANGESOCKETTODEFAULT_ENTRY(socketPtr);

  socketPtr->generic.aligned.ipParameters.ttl = TcpIp_PBcfgPtr->udpConfig.ttl;
  socketPtr->generic.aligned.ipParameters.vlanPriority = TCPIP_IP_VLANPRIO_UNUSED;
  socketPtr->generic.aligned.ipParameters.mayFragment = TCPIP_UDP_MAYFRAGMENT;
  socketPtr->generic.aligned.ipParameters.useUnspecifiedSrc = FALSE;
  socketPtr->generic.aligned.ipParameters.flowLabel = 0U;
  socketPtr->generic.aligned.ipParameters.dscp = 0U;
  socketPtr->generic.aligned.ipParameters.calculateChecksum = TRUE;
  socketPtr->generic.aligned.ipParameters.protocol = TCPIP_IP_PROTOCOL_UDP;
  socketPtr->generic.aligned.ipParameters.calculatePseudoChecksum = TRUE;
  socketPtr->generic.aligned.ipParameters.identifier = 0U;

  DBG_TCPIP_UDP_CHANGESOCKETTODEFAULT_EXIT(socketPtr);
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE)  TcpIp_Udp_changeParameter
(
  P2VAR(TcpIp_Socket_UDP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) paramValuePtr,
  TcpIp_ParamIdType paramId
)
{
  Std_ReturnType retVal = E_NOT_OK;

  DBG_TCPIP_UDP_CHANGEPARAMETER_ENTRY(socketPtr, paramValuePtr, paramId);

  switch(paramId)
  {
    case TCPIP_PARAMID_UDP_CHECKSUM:
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
    if(*paramValuePtr > 1U)
    {
      TCPIP_REPORT_ERROR(TCPIP_SID_CHANGEPARAMETER, TCPIP_E_INV_ARG);
    }
    else
#endif /* ( TCPIP_DEV_ERROR_DETECT == STD_ON ) */
    {
      retVal = E_OK;
      if(*paramValuePtr == 0U)
      {
        socketPtr->generic.aligned.ipParameters.calculateChecksum = FALSE;
      }
      else
      {
        socketPtr->generic.aligned.ipParameters.calculateChecksum = TRUE;
      }
    }
    break;
    /* CHECK: NOPARSE */
    default:
    {
     /* unknown parameter id */
      TCPIP_UNREACHABLE_CODE_ASSERT(TCPIP_SID_CHANGEPARAMETER);
      break;
    }
    /* CHECK: PARSE */
  }

  DBG_TCPIP_UDP_CHANGEPARAMETER_EXIT(retVal, socketPtr, paramValuePtr, paramId);

  return retVal;
}

#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)
TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Udp_GetAndResetInvalidSocketCount
(
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) MeasurementDataPtr
)
{
  DBG_TCPIP_UDP_GETANDRESETINVALIDSOCKETCOUNT_ENTRY(MeasurementResetNeeded, MeasurementDataPtr);

  if(MeasurementDataPtr != NULL_PTR)
  {
    /* !LINKSTO TcpIp.Design.Udp.Atomic.InvalidSocketCounter,1 */
    TS_AtomicAssign32(*MeasurementDataPtr, TcpIp_Udp_InvalidSocketCounter);
  }
  if(MeasurementResetNeeded)
  {
    /* !LINKSTO TcpIp.Design.Udp.Atomic.InvalidSocketCounter,1 */
    TS_AtomicAssign32(TcpIp_Udp_InvalidSocketCounter, 0U);
  }

  DBG_TCPIP_UDP_GETANDRESETINVALIDSOCKETCOUNT_EXIT(MeasurementResetNeeded, MeasurementDataPtr);
}
#endif /* TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */


STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Udp_transmit
(
  TcpIp_SocketIdType socketId,
  uint8 ipSecSaCommonIdx,
  TcpIp_Socket_Generic_PtrType socketPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  TcpIp_LocalAddrIdType localAddrId,
  uint16 udpSrcPort,
  uint16 totalLength
)
{
  TcpIp_ReturnType retVal;
  uint8 addHeaderLength = 0U;
  uint16 maxPacketPayload;
  /* Deviation MISRAC2012-1 */
  P2CONST(TcpIp_SockAddrInetType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrInetPtr =
    (P2CONST(TcpIp_SockAddrInetType, AUTOMATIC, TCPIP_APPL_DATA))remoteAddrPtr;

  retVal =
    TcpIp_Ip_getMaximumPayload
      (ipSecSaCommonIdx, localAddrId, &maxPacketPayload, &addHeaderLength);

  if(retVal == E_OK)
  {
    uint16 maxPayloadLength;
#if(TCPIP_IP_FRAGMENT_TX_ENABLED == TCPIP_IP_FRAGMENT_TX_OUTOFORDER)
    maxPayloadLength = 0xFFFFU;
#elif(TCPIP_IP_FRAGMENT_TX_ENABLED == TCPIP_IP_FRAGMENT_TX_INORDER)
    maxPayloadLength = TcpIp_PBcfgPtr->ipFragConfig.txFragmentBufferSize - addHeaderLength;
#else /* (TCPIP_IP_FRAGMENT_TX_ENABLED == STD_OFF) */
    maxPayloadLength = maxPacketPayload;
#endif /* (TCPIP_IP_FRAGMENT_TX_ENABLED == ...) */

    if(totalLength > (maxPayloadLength - TCPIP_UDP_HEADER_LENGTH))
    {
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
      TCPIP_REPORT_ERROR(TCPIP_SID_UDPTRANSMIT, TCPIP_E_MSGSIZE);
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
      retVal= TCPIP_E_NOT_OK;
    }
    else
    {
      TcpIp_Udp_UpdateHeaderFunctorType updateHeaderFunctor= TCPIP_UDP_UPDATEHEADER_INITIALIZER;
      TcpIp_Udp_CopyDataFunctorType copyDataFunctor= { &TcpIp_Udp_copyData, 0u, NULL_PTR };
      TcpIp_Ip_IpParameterType ipParametersTransmit = socketPtr->aligned.ipParameters;
      uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
      boolean increaseId = ipParametersTransmit.mayFragment;
      boolean checksumOffload = TCPIP_GET_ENABLE_OFFLOAD_CHECKSUM_UDP(ctrl,ctrlIdx);
      /* Deviation MISRAC2012-1 <+4> */
      P2VAR(TcpIp_Ip_CopyDataFunctorType, AUTOMATIC, TCPIP_APPL_DATA) copyIpDataFunctor =
        (P2VAR(TcpIp_Ip_CopyDataFunctorType, AUTOMATIC, TCPIP_APPL_DATA)) &copyDataFunctor;
      P2VAR(TcpIp_Ip_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA) updateIpHeaderFunctor =
        (P2VAR(TcpIp_Ip_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA)) &updateHeaderFunctor;

      TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

      TCPIP_PRECONDITION_ASSERT
        ((socketPtr->aligned.sockOwnerId < TcpIp_NumSockOwner),TCPIP_INTERNAL_API_ID);


      copyDataFunctor.copyTxDataFctPtr= TcpIp_CopyTxDataAPI[socketPtr->aligned.sockOwnerId];
      copyDataFunctor.handleId= socketId;

      /* These are used to forward required data to the UDP header-construction. */
      updateHeaderFunctor.udpSrcPort= udpSrcPort;
      updateHeaderFunctor.udpDstPort= remoteAddrInetPtr->port;
      updateHeaderFunctor.length = totalLength + TCPIP_UDP_HEADER_LENGTH;

      ipParametersTransmit.maxIpPayloadLength = maxPacketPayload;

#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)
      ipParametersTransmit.ipSecSaCommonIdx =  ipSecSaCommonIdx;
      ipParametersTransmit.addHeaderLength =  addHeaderLength;
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */

#if (TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF)
      if((totalLength + TCPIP_UDP_HEADER_LENGTH) > maxPacketPayload)
      {
        /* Override mayFragment to FALSE for fragmented packets */
        ipParametersTransmit.mayFragment = FALSE;
        /* if transmission is fragmented, checksum offload cannot be used */
        checksumOffload = FALSE;
        /* for fragmented frames the id always needs to be incremented */
        increaseId = TRUE;
      }
#endif /* (TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF) */

      if(increaseId)
      {
        /* const for all fragments (& full packet) */
        TCPIP_IP_GETNEXTID(TCPIP_IP_DOMAIN_GET(remoteAddrPtr->domain),
            ipParametersTransmit.identifier, TcpIp_Udp_fragmentIdCounter);
      }

      ipParametersTransmit.calculateChecksum =
        TCPIP_CALCULATE_CHECKSUM(socketPtr->aligned.ipParameters.calculateChecksum, checksumOffload);

      retVal= TcpIp_Ip_transmit
              (
                copyIpDataFunctor,
                updateIpHeaderFunctor,
                &ipParametersTransmit,
                localAddrId,
                remoteAddrPtr,
                dataPtr,
                TCPIP_UDP_HEADER_LENGTH,
                totalLength
              );
    }
  }

  return retVal;
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Udp_checkIfOnline
(
  TcpIp_Socket_Generic_PtrType socketPtr,
  TcpIp_LocalAddrIdType localAddrId
)
{
  TcpIp_ReturnType retVal = E_NOT_OK;

#if ((TCPIP_DHCPV4_CLIENT_ENABLED == STD_ON) || (TCPIP_DHCPV6_CLIENT_ENABLED == STD_ON))
  /* check if data is allowed to be sent with unspecified address, only for internal
     socket owner */
  if(socketPtr->aligned.ipParameters.useUnspecifiedSrc)
  {
    if(TcpIp_IpAddrM_lockIfStartupOnlineShutdown(localAddrId))
    {
      retVal = TCPIP_OK;
    }
  }
  else
#else /* ((TCPIP_DHCPV4_CLIENT_ENABLED == STD_ON) || (TCPIP_DHCPV6_CLIENT_ENABLED == STD_ON)) */
  TS_PARAM_UNUSED(socketPtr);
#endif /* ((TCPIP_DHCPV4_CLIENT_ENABLED == STD_ON) || (TCPIP_DHCPV6_CLIENT_ENABLED == STD_ON)) */
  {
    /* interface needs to be online */
    if(TcpIp_IpAddrM_lockIfOnline(localAddrId))
    {
      retVal = TCPIP_OK;
    }
  }

  return retVal;
}

#endif /* (TCPIP_UDP_ENABLED == STD_ON) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[end of file]===========================================*/
