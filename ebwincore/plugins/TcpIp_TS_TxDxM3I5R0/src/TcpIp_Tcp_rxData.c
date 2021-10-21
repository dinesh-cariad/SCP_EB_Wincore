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
 * AUTOSAR defines a generic type which dependent on a member of the generic type
 * implements an concrete type. Thus for usage of this concrete type, a cast is necessary.
 *
 * MISRAC2012-2) Deviated Rule: 10.8 (required)
 * The value of a composite expression shall not be cast to a different essential
 * type category or a wider essential type.
 *
 * Reason:
 * In order to detect a wrap around of sequence and acknowledgement numbers it is required
 * to cast unsigned int to signed int.
 *
 */

/*==================[inclusions]============================================*/

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_IpAddrM_Int.h>  /* used SW-unit interface file */
#include <TcpIp_Memory_Int.h>   /* used SW-unit interface file */
#include <TcpIp_Tcp_Priv.h>     /* own SW-unit interface file */
#include <SchM_TcpIp.h>           /* SchM API declarations */

/*==================[macros]================================================*/

/** \brief Retrieves the 16-bit source port from the TCP header */
#define TCPIP_TCP_GET_SRC_PORT(tcphdr)                            \
  TCPIP_GET16((tcphdr), 0)

/** \brief Retrieves the 16-bit destination port from the TCP header */
#define TCPIP_TCP_GET_DST_PORT(tcphdr)                            \
  TCPIP_GET16((tcphdr), 2)

/** \brief Retrieves the 32-bit sequence number from the TCP header */
#define TCPIP_TCP_GET_SEQNR(tcphdr)                               \
  TCPIP_GET32((tcphdr), 4)

/** \brief Retrieves the 32-bit acknowledgement number from the TCP header */
#define TCPIP_TCP_GET_ACKNR(tcphdr)                               \
  TCPIP_GET32((tcphdr), 8)

/** \brief Retrieves the 4-bit data offset value from the TCP header */
#define TCPIP_TCP_GET_OFFSET(tcphdr)                              \
  ((uint8)(((tcphdr)[12U] & 0xF0U) >> 4U))

/** \brief Retrieves the flags from the TCP header */
#define TCPIP_TCP_GET_FLAGS(tcphdr)                               \
  ((uint8)((tcphdr)[13U] & 0x3FU))

/** \brief Retrieves the window from the TCP header */
#define TCPIP_TCP_GET_WINDOW(tcphdr)                              \
  TCPIP_GET16((tcphdr), 14)

/** \brief Retrieves the checksum from the TCP header */
#define TCPIP_TCP_GET_CHECKSUM(tcphdr)                            \
  TCPIP_GET16((tcphdr), 16)

/** \brief Get the mss field of the mss option */
#define TCPIP_TCP_MSS_OPT_GET_MSS(tcphdr)                         \
  TCPIP_GET16((tcphdr), 2)

/** \brief Checks if the segment only contains SYN and an optional ACK */
#define TCPIP_TCP_IS_SYN_SEG(flags)                                \
  (TCPIP_TCP_FLAG_SYN_IS_SET((flags)) &&                           \
   TCPIP_TCP_ALLOWED_FLAGS                                         \
      ((flags), TCPIP_TCP_FLAG_SYN + TCPIP_TCP_FLAG_ACK))

/** \brief Checks if the segment only contains ACK and an optional PSH or URG */
#define TCPIP_TCP_IS_ACK_SEG(flags)                                \
  (TCPIP_TCP_FLAG_ACK_IS_SET((flags)) &&                           \
   TCPIP_TCP_ALLOWED_FLAGS                                         \
     ((flags), TCPIP_TCP_FLAG_ACK + TCPIP_TCP_FLAG_PSH + TCPIP_TCP_FLAG_URG))

/** \brief Checks if the segment only contains FIN, ACK and an optional PSH or URG */
#define TCPIP_TCP_IS_FIN_SEG(flags)                                \
  (TCPIP_TCP_FLAG_FIN_IS_SET((flags)) &&                           \
   TCPIP_TCP_FLAG_ACK_IS_SET((flags)) &&                           \
   TCPIP_TCP_ALLOWED_FLAGS                                         \
     ((flags), TCPIP_TCP_FLAG_FIN + TCPIP_TCP_FLAG_ACK + TCPIP_TCP_FLAG_PSH + TCPIP_TCP_FLAG_URG))

/** \brief Checks if the segment only contains RST and an optional ACK or SYN */
#define TCPIP_TCP_IS_RST_SEG(flags)                                \
  (TCPIP_TCP_FLAG_RST_IS_SET((flags)) &&                           \
   TCPIP_TCP_ALLOWED_FLAGS                                         \
     ((flags), TCPIP_TCP_FLAG_RST + TCPIP_TCP_FLAG_ACK + TCPIP_TCP_FLAG_SYN))

/** \brief Checks which flags are allowed to be set in a segment */
#define TCPIP_TCP_ALLOWED_FLAGS(recievedflags, allowedflags)       \
  (((recievedflags) & (~((uint8)(0xC0U | (allowedflags))))) == 0U)

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/


/*==================[internal function declarations]========================*/

#if (TCPIP_TCP_ENABLED == STD_ON)

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief Parse Option of a segment
 *
 * This function processes the variable option field of a TCP segment and extracts
 * the mss option value.
 *
 * \param[in] SocketPtr  Pointer to the socket connection information
 * \param[in]  dataPtr   Points to the beginning of the option field
 * \param[in]  length    length of the option field
 * \param[out] mss       Maximum Segment Size Option Value
 *
 * \retval TCPIP_OK        parsing successful
 * \retval TCPIP_E_NOT_OK  option contains invalid length
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_rxData_retreiveOptions
(
  TcpIp_Socket_TCP_Generic_PtrType SocketPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint8 length,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) mss
);

#if (TCPIP_TCP_OPTION_FILTER_ENABLE == STD_ON)
 /** \brief Check Option filter
  *
  * This function checks if the options is in the options white list
  *
  * \param[in] SocketPtr  Pointer to the socket connection information
  * \param[in] optionId   Option id
  *
  * \retval TRUE          option is in the white list (allowed)
  * \retval FALSE         option is not in the white list (not allowed)
  */
STATIC FUNC(boolean, TCPIP_CODE) TcpIp_Tcp_rxData_CheckOptionsFilter
(
  TcpIp_Socket_TCP_Generic_PtrType SocketPtr,
  uint8 optionId
);
#endif /* (TCPIP_TCP_OPTION_FILTER_ENABLE == STD_ON) */

/** \brief Confirm acknowledged data
 *
 * This function frees data from the transmission queue which is acknowledged by the
 * remote host, updates the retransmission timer and confirms the transmission to Soad
 *
 * \param[in]  socketId      id of the socket connection
 * \param[in]  socketPtr     Pointer to the socket connection information
 * \param[in]  ackNumber     acknowledgement number of received segment
 * \param[in]  localAddrId   Local address the packet is received on
 * \param[in]  remoteAddrPtr IP Address and port of the remote host
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_rxData_ackUpdate
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  uint32 ackNumber,
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr
);

/** \brief Update the Send Window
 *
 * This function evaluates the number of bytes the remote host is willing to accept.
 *
 * \param[in]  ctrlBlockPtr  Pointer to the transmission control block
 * \param[in]  seqNumber     sequence number of received segment
 * \param[in]  ackNumber     acknowledgement number of received segment
 * \param[in]  sendWindow    window of received segment
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_rxData_SendWindowUpdate
(
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr,
  uint32 seqNumber,
  uint32 ackNumber,
  uint16 sendWindow
);

/** \brief Verify TCP segment fields
 *
 * This function verifies if the segment is valid. E.g. correct checksum, destination
 * address equals unicast, correct header length
 *
 * \param[in]  dataPtr          Points to the beginning of the TCP header
 * \param[in]  remoteAddrPtr    IP address and port the remote host
 * \param[in]  localIpAddrPtr   Local IP address
 * \param[in]  localAddrId      Local address the packet is received on
 * \param[in]  lenByte          Length of received data.
 * \param[in]  isFragmented     If true, packet is fragmented
 *
 * \retval TCPIP_OK        segment ok
 * \retval TCPIP_E_NOT_OK  segment contains invalid field
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_rxData_checkTcpSeg
(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) localIpAddrPtr,
  TcpIp_LocalAddrIdType localAddrId,
  uint16 lenByte,
  boolean isFragmented
);

/** \brief Evaluate TCP header fields and data of segments
 *
 * This function process the control flags and the data of segments, confirms
 * acknowledged data and updates the send window
 *
 * \param[in]  socketId       id of the socket connection
 * \param[in]  tcpSocketPtr   Pointer to the socket connection information
 * \param[in]  remoteAddrPtr  IP Address and port of the remote host
 * \param[in]  localAddr      IP address and port on which the segment was received on
 * \param[in]  dataPtr        Points to the beginning of the TCP header
 * \param[in]  lenByte        Length of received data.
 * \param[in]  mss            Mss of received segment
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_rxData_evaluateSeg
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) tcpSocketPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  TcpIp_SockAddrLocalType localAddr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 lenByte,
  uint16 mss
);

/** \brief Check for erroneous segments
 *
 * This function checks if the segment contains unexpected control flags,
 * sequence numbers or acknowledgement numbers.
 *
 * \param[in]  socketId       id of the socket connection
 * \param[in]  tcpSocketPtr   Pointer to the socket connection information
 * \param[in]  remoteAddrPtr  IP Address and port of the remote host
 * \param[in]  localAddr      IP address and port on which the segment was received on
 * \param[in]  controlBits    Control bits of received segment
 * \param[in]  seqNum         Sequence Number of received segment
 * \param[in]  ackNum         Acknowledgement Number of received segment
 * \param[in]  datalen        data length of the received segment
 *
 * \retval TCPIP_OK           no error
 * \retval TCPIP_E_DROP       discard segment
 * \retval TCPIP_E_NOT_OK     discard segment and close connection
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_rxData_checkTcpConn
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) tcpSocketPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  TcpIp_SockAddrLocalType localAddr,
  uint8 controlBits,
  uint32 seqNum,
  uint32 ackNum,
  uint16 datalen
);

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)

#define TCPIP_START_SEC_VAR_INIT_32
#include <TcpIp_MemMap.h>

STATIC VAR(uint32, TCPIP_VAR) TcpIp_Tcp_InvalidSocketCounter = 0U;

#define TCPIP_STOP_SEC_VAR_INIT_32
#include <TcpIp_MemMap.h>

#endif /* TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Tcp_rxIndication
(
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) localIpAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  TcpIp_LocalAddrIdType localAddrId,
  uint16 lenByte,
  boolean isFragmented
)
{
  DBG_TCPIP_TCP_RXINDICATION_ENTRY(remoteAddrPtr,localIpAddrPtr,dataPtr,localAddrId,lenByte,isFragmented);
  {

    const TcpIp_StateType state = TcpIp_IpAddrM_getIfState(localAddrId);

    /* only accept segments if unicast ip is assigned and link is up */
    if((state == TCPIP_STATE_ONLINE) || (state == TCPIP_STATE_SHUTDOWN))
    {
      /* check for malformed segment */
      if
      (
        TcpIp_Tcp_rxData_checkTcpSeg
        (
          dataPtr,
          remoteAddrPtr,
          localIpAddrPtr,
          localAddrId,
          lenByte,
          isFragmented
        ) == TCPIP_OK
      )
      {
        TcpIp_ReturnType result;
        P2VAR(TcpIp_SockAddrInetType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrInetPtr =
            /* Deviation MISRAC2012-1 */
            (P2VAR(TcpIp_SockAddrInetType, AUTOMATIC, TCPIP_APPL_DATA)) remoteAddrPtr;
        TcpIp_SockAddrLocalType localAddr;
        const uint8 controlBits = TCPIP_TCP_GET_FLAGS(dataPtr);
        TcpIp_SocketIdType socketIdPtr = (TcpIp_SocketIdType) 0U;
        TcpIp_Socket_TCP_Generic_PtrType socketPtr = NULL_PTR;
        const uint8 dataOffset = TCPIP_TCP_GET_OFFSET(dataPtr) * 4U;

        remoteAddrInetPtr->port = TCPIP_TCP_GET_SRC_PORT(dataPtr);
        localAddr.port = TCPIP_TCP_GET_DST_PORT(dataPtr);
        localAddr.addrId = localAddrId;

        /* Retrieve matching socket connection */
        result =
          TcpIp_Socket_lookupTcp(&localAddr, remoteAddrPtr, &socketPtr, &socketIdPtr);

        if(result == TCPIP_OK)
        {
          const uint8 optionlength = dataOffset - TCPIP_TCP_HEADER_SIZE;
          uint16 mss = 0U;
          CONSTP2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) optPtr = &dataPtr[TCPIP_TCP_HEADER_SIZE];

          TCPIP_PRECONDITION_ASSERT( socketPtr != NULL_PTR, TCPIP_INTERNAL_API_ID );
          /* parse options and get mss if available */
          if(TcpIp_Tcp_rxData_retreiveOptions(socketPtr, optPtr, optionlength, &mss) == TCPIP_OK)
          {
            /* socket connection found, process segment */
            TcpIp_Tcp_rxData_evaluateSeg
                                      (
                                       socketIdPtr,
                                       socketPtr,
                                       remoteAddrPtr,
                                       localAddr,
                                       dataPtr,
                                       lenByte,
                                       mss
                                      );
          }
        }
        else
        {
#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)
          TcpIp_Tcp_IncrementInvalidSocketCount();
#endif /* TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */

          /* a reset is only sent in response to non resets */
          if(!TCPIP_TCP_FLAG_RST_IS_SET(controlBits))
          {
            TcpIp_Ip_IpParameterType ipParameter = TCPIP_IP_IPPARAMETER_INITIALIZER;

            result = TcpIp_Tcp_getTransmissionParam(localAddrId,
                                                    remoteAddrPtr,
                                                    TCPIP_IP_PROTOCOL_TCP,
                                                    localAddr.port,
                                                    &ipParameter
                                                   );

            if(result == E_OK)
            {
              const uint32 seqNumber = TCPIP_TCP_GET_SEQNR(dataPtr);
              const uint32 ackNumber = TCPIP_TCP_GET_ACKNR(dataPtr);
              const uint16 datalen = lenByte - dataOffset;
              const uint16 seglen = datalen + TcpIp_Tcp_evaluateSegLen(controlBits);

              ipParameter.protocol = TCPIP_IP_PROTOCOL_TCP;
              ipParameter.calculatePseudoChecksum = TRUE;
              ipParameter.ttl = TcpIp_PBcfgPtr->tcpConfig.ttl;
              ipParameter.vlanPriority = TCPIP_IP_VLANPRIO_UNUSED;

              /* socket connection does not exist, transmit reset */
              TcpIp_Tcp_txData_sendRst
                                   (
                                    &ipParameter,
                                    remoteAddrPtr,
                                    &localAddr,
                                    ackNumber,
                                    seqNumber,
                                    seglen,
                                    controlBits
                                   );
            }
          }
        }
      }
    }
  }
  DBG_TCPIP_TCP_RXINDICATION_EXIT(remoteAddrPtr,localIpAddrPtr,dataPtr,localAddrId,lenByte,isFragmented);
}


TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Tcp_freeSocketMemoryData
(
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr,
  uint16 dataToFree
)
{
  TCPIP_PRECONDITION_ASSERT(dataToFree <= ctrlBlockPtr->used_buff, TCPIP_INTERNAL_API_ID);
  TCPIP_PRECONDITION_ASSERT(dataToFree <= ctrlBlockPtr->send_buff, TCPIP_INTERNAL_API_ID);

  /* free acknowledged data */
  ctrlBlockPtr->avail_buff =
    TcpIp_Memory_decrease
      (
       ctrlBlockPtr->memory_idPtr,
       /* if no memory is needed any more, set available memory size to free the whole
        * socket memory */
       (ctrlBlockPtr->used_buff == dataToFree) ? ctrlBlockPtr->avail_buff : dataToFree
      );

  /* update used and sent buff */
  ctrlBlockPtr->used_buff -= dataToFree;
  ctrlBlockPtr->send_buff -= dataToFree;

  if(ctrlBlockPtr->curr_buff < dataToFree)
  {
   /* if data is retransmitted it can happen that more data is
      acknowledged than retransmitted (old segment gets ack) */
   ctrlBlockPtr->curr_buff = 0U;
  }
  else
  {
   ctrlBlockPtr->curr_buff -= dataToFree;
  }
}

#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)
TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Tcp_GetAndResetInvalidSocketCount
(
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) MeasurementDataPtr
)
{
  DBG_TCPIP_TCP_GETANDRESETINVALIDSOCKETCOUNT_ENTRY(MeasurementResetNeeded, MeasurementDataPtr);

  if(MeasurementDataPtr != NULL_PTR)
  {
    /* !LINKSTO TcpIp.Design.Tcp.Atomic.InvalidSocketCounter,1 */
    TS_AtomicAssign32(*MeasurementDataPtr, TcpIp_Tcp_InvalidSocketCounter);
  }
  if(MeasurementResetNeeded)
  {
    /* !LINKSTO TcpIp.Design.Tcp.Atomic.InvalidSocketCounter,1 */
    TS_AtomicAssign32(TcpIp_Tcp_InvalidSocketCounter, 0U);
  }

  DBG_TCPIP_TCP_GETANDRESETINVALIDSOCKETCOUNT_EXIT(MeasurementResetNeeded, MeasurementDataPtr);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Tcp_IncrementInvalidSocketCount(void)
{
  /* increment counter of dropped PDUs caused by invalid destination TCP-Port */
  /* !LINKSTO TcpIp.Design.Tcp.EA6.rxIndication,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_6();
  if(TcpIp_Tcp_InvalidSocketCounter < 0xFFFFFFFFU)
  {
    /* !LINKSTO TcpIp.Design.Tcp.Atomic.InvalidSocketCounter,1 */
    TS_AtomicAssign32(TcpIp_Tcp_InvalidSocketCounter, (TcpIp_Tcp_InvalidSocketCounter+1U));
  }
  /* !LINKSTO TcpIp.Design.Tcp.EA6.rxIndication,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_6();
}
#endif /* TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */

/*==================[internal function definitions]=========================*/

STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_rxData_evaluateSeg
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) tcpSocketPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  TcpIp_SockAddrLocalType localAddr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 lenByte,
  uint16 mss
)
{
  TcpIp_ReturnType result = TCPIP_E_DROP;
  const uint8 controlBits = TCPIP_TCP_GET_FLAGS(dataPtr);
  const uint32 seqNum = TCPIP_TCP_GET_SEQNR(dataPtr);
  const uint32 ackNum = TCPIP_TCP_GET_ACKNR(dataPtr);
  const uint8 dataOffset = TCPIP_TCP_GET_OFFSET(dataPtr) * 4U;
  const uint16 datalen = lenByte - dataOffset;

  /* check for segments */
  result =
    TcpIp_Tcp_rxData_checkTcpConn
      (socketId, tcpSocketPtr, remoteAddrPtr, localAddr, controlBits, seqNum, ackNum, datalen);

  if(result == TCPIP_OK)
  {
    const uint16 sendWindow = TCPIP_TCP_GET_WINDOW(dataPtr);

    if(TCPIP_TCP_FLAG_ACK_IS_SET(controlBits))
    {
      /* update receive next */
      TcpIp_Tcp_rxData_ackUpdate(socketId, tcpSocketPtr, ackNum, localAddr.addrId,remoteAddrPtr);

      /* handle ack segment */
      result =
        TcpIp_Tcp_SM_rcvAck
          (&socketId, &tcpSocketPtr, remoteAddrPtr, &localAddr, seqNum, ackNum, datalen, controlBits);

      if(result == TCPIP_OK)
      {
        /* update send window */
        TcpIp_Tcp_rxData_SendWindowUpdate(&tcpSocketPtr->tcp, seqNum, ackNum, sendWindow);

        /* process data, pass data to SoAd and delay transmission of Ack */
        if(datalen != 0U)
        {
          result =
            TcpIp_Tcp_SM_rcvData
              (socketId, tcpSocketPtr, &dataPtr[dataOffset], datalen, seqNum);
        }
      }
    }

    if(result == TCPIP_OK)
    {
      if(TCPIP_TCP_FLAG_FIN_IS_SET(controlBits))
      {
        /* handle fin segment, send Ack */
        (void) TcpIp_Tcp_SM_rcvFin(socketId, tcpSocketPtr, (seqNum + datalen));
      }
      else if(TCPIP_TCP_FLAG_SYN_IS_SET(controlBits))
      {
        const uint16 seglen = datalen + TcpIp_Tcp_evaluateSegLen(controlBits);

        /* handle Syn segment, send Syn/Ack */
        (void) TcpIp_Tcp_SM_rcvSyn
                   (socketId, tcpSocketPtr, remoteAddrPtr, &localAddr, seqNum, seglen, mss, controlBits);
      }
      else
      {
        /* ack received */
      }

      {
        P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr =
          &tcpSocketPtr->tcp;

        /* transmit queued data if there is any */
        if((ctrlBlockPtr->used_buff - ctrlBlockPtr->curr_buff) != 0U)
        {
          /* mark transmission, in case data cannot be transmitted */
          ctrlBlockPtr->flags |= TCPIP_TCP_TRANSMIT_FLAG;

          if(TcpIp_Tcp_txData_transmit(tcpSocketPtr) != TCPIP_OK)
          {
            /* segment could not be sent try in next mainfunction */
            TcpIp_Tcp_startRetry(ctrlBlockPtr);
          }
        }
      }
    }
  }
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_rxData_checkTcpConn
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) tcpSocketPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  TcpIp_SockAddrLocalType localAddr,
  uint8 controlBits,
  uint32 seqNum,
  uint32 ackNum,
  uint16 datalen
)
{
  TcpIp_ReturnType result;
  const uint16 seglen = datalen + TcpIp_Tcp_evaluateSegLen(controlBits);

  /* check if the sequence number of the segment is acceptable */
  result = TcpIp_Tcp_SM_checkSeqNum(tcpSocketPtr, seqNum, seglen, controlBits);

  if((result == TCPIP_OK) && TCPIP_TCP_FLAG_ACK_IS_SET(controlBits))
  {
    /* check if acknowledgement number of the segment is acceptable */
    result =
      TcpIp_Tcp_SM_checkAckNum
        (tcpSocketPtr, remoteAddrPtr, localAddr, ackNum, seqNum, seglen, controlBits);
  }

  if(result == TCPIP_OK)
  {
    if(TCPIP_TCP_IS_RST_SEG(controlBits))
    {
      /* handle reset segment */
      result = TcpIp_Tcp_SM_rcvRst(socketId, tcpSocketPtr, controlBits);
    }
    else if(TCPIP_TCP_IS_SYN_SEG(controlBits) && (datalen == 0U))
    {
      /* check if SYN is received in illegal state */
      result = TcpIp_Tcp_SM_rcvSynCheckErr
                 (socketId, tcpSocketPtr, seqNum, ackNum, seglen, controlBits);
    }
    else if(TCPIP_TCP_IS_FIN_SEG(controlBits))
    {
      /* check if SYN is received in illegal state */
      result = TcpIp_Tcp_SM_rcvFinCheckErr(tcpSocketPtr);
    }
    else if(TCPIP_TCP_IS_ACK_SEG(controlBits))
    {
      /* check if ACK is received in illegal state */
      result =  TcpIp_Tcp_SM_rcvAckCheckErr(tcpSocketPtr);
    }
    else
    {
      /* drop the segment, segment contains unexpected flags */
      result = TCPIP_E_DROP;
    }
  }

  return result;
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_rxData_checkTcpSeg
(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) localIpAddrPtr,
  TcpIp_LocalAddrIdType localAddrId,
  uint16 lenByte,
  boolean isFragmented
)
{
  TcpIp_ReturnType result;
  const TcpIp_IpAddrM_AddrInfoType addressInfo = TcpIp_IpAddrM_getLocalAddrInfo(localAddrId);
  const uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
  boolean calculateChecksum;

  TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

  calculateChecksum =
  (
    (TCPIP_GET_ENABLE_OFFLOAD_CHECKSUM_TCP(ctrl,ctrlIdx) == FALSE) ||
    (isFragmented == TRUE)
  );

  if(!TCPIP_IPADDRM_IS_ADDRTYPE(addressInfo, UNICAST))
  {
    /* Tcp allows unicast connections only */
    result = TCPIP_E_NOT_OK;
  }
  else if(lenByte < TCPIP_TCP_HEADER_SIZE)
  {
    /* Tcp segment must have a size of at least 20 bytes */
    result = TCPIP_E_NOT_OK;
  }
  else if(TCPIP_TCP_GET_OFFSET(dataPtr) < TCPIP_TCP_HEADER_SIZE_UINT32)
  {
    /* Tcp segment must have a size of at least 20 bytes */
    result = TCPIP_E_NOT_OK;
  }
  else if(((uint16)TCPIP_TCP_GET_OFFSET(dataPtr) * 4U) > lenByte)
  {
    /* offset must be within the received payload */
    result = TCPIP_E_NOT_OK;
  }
  else if(calculateChecksum == TRUE)
  {
    if(
        TcpIp_CalcChecksum16Bit_withInitValue
        (
          dataPtr,
          TcpIp_Ip_calcPartialChecksum
            (remoteAddrPtr,localIpAddrPtr,lenByte,  TCPIP_IP_PROTOCOL_TCP),
          lenByte
        ) != 0U
      )
    {
      result = TCPIP_E_NOT_OK;
    }
    else
    {
      result = TCPIP_OK;
    }
  }
  else
  {
    /* no error encountered */
    result = TCPIP_OK;
  }

  return result;
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_rxData_retreiveOptions
(
  TcpIp_Socket_TCP_Generic_PtrType SocketPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint8 length,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) mss
)
{
  TcpIp_ReturnType result = TCPIP_OK;
  uint16 index = 0U;

  /* loop over variable option field */
  while((index < length) &&
        (dataPtr[index] != TCPIP_TCP_OPT_KIND_ENDOPT) &&
        (result == TCPIP_OK)
       )
  {
#if (TCPIP_TCP_OPTION_FILTER_ENABLE == STD_ON)
    /* check if the option is in the options white list */
    if (!TcpIp_Tcp_rxData_CheckOptionsFilter(SocketPtr, dataPtr[index]))
    {
      result = TCPIP_E_NOT_OK;
    }
    else
#else /* (TCPIP_TCP_OPTION_FILTER_ENABLE == STD_ON) */
    TS_PARAM_UNUSED(SocketPtr);
#endif /* (TCPIP_TCP_OPTION_FILTER_ENABLE == STD_ON) */
    if(dataPtr[index] == TCPIP_TCP_OPT_KIND_NOP)
    {
      index++;
    }
    else
    {
      if((index + TCPIP_TCP_OPT_HDR_LEN) > length)
      {
        /* invalid option encountered, drop segment */
        result = TCPIP_E_NOT_OK;
      }
      else
      {
        uint8 optlen = dataPtr[index + 1U];

        if(dataPtr[index] == TCPIP_TCP_OPT_KIND_MSS)
        {
          if((index + TCPIP_TCP_MSS_OPT_LEN) > length)
          {
            /* invalid mss option length encountered, drop segment */
            result = TCPIP_E_NOT_OK;
          }
          else if(optlen != TCPIP_TCP_MSS_OPT_LEN)
          {
            /* invalid mss option length encountered, drop segment */
             result = TCPIP_E_NOT_OK;
          }
          else
          {
            *mss = TCPIP_TCP_MSS_OPT_GET_MSS(&dataPtr[index]);
          }
        }
        else /* unknown option */
        {
          if(optlen == 0U)
          {
            /* drop the segment if unknown option with 0 length is encountered */
            result = TCPIP_E_NOT_OK;
          }
        }

        index += optlen;
      }
    }
  }

  return result;
}


#if (TCPIP_TCP_OPTION_FILTER_ENABLE == STD_ON)
STATIC FUNC(boolean, TCPIP_CODE) TcpIp_Tcp_rxData_CheckOptionsFilter
(
  TcpIp_Socket_TCP_Generic_PtrType SocketPtr,
  uint8 optionId
)
{
  boolean optionIdIsAllowed = FALSE;
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr = &SocketPtr->tcp;

  /* 255 default value allow all options */
  if(ctrlBlockPtr->activeOptionFilter == TCPIP_OPTION_FILTER_UNSET)
  {
    optionIdIsAllowed = TRUE;
  }
  /* check if current option is in the option white list */
  else
  {
    uint8 i;
    CONSTP2CONST(TcpIp_TcpOptionFilterConfigType,AUTOMATIC,TCPIP_APPL_CONST) tcpOptionFilter =
      &(TCPIP_CFG_TYPE(TcpIp_TcpOptionFilterConfigType,tcpOptionFilterList,ctrlBlockPtr->activeOptionFilter));

    for(i=tcpOptionFilter->filterOffset; i<(tcpOptionFilter->optionFilterNum + tcpOptionFilter->filterOffset); ++i)
    {
      if(optionId == (TCPIP_CFG_TYPE(uint8, tcpOptionIdList, i)))
      {
        optionIdIsAllowed = TRUE;
        break;
      }
    }
  }

  return optionIdIsAllowed;
}
#endif /* (TCPIP_TCP_OPTION_FILTER_ENABLE == STD_ON) */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_rxData_ackUpdate
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  uint32 ackNumber,
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr
)
{
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr = &socketPtr->tcp;

  if(ctrlBlockPtr->state != TCPIP_TCP_STATE_LISTEN)
  {
    if(TCPIP_TCP_IS_LOWER_OR_EQUAL(ackNumber, ctrlBlockPtr->send_una))
    {
      /* duplicated ACK */

      /* in case of zero window probing, restart the retransmission counter */
      if((ackNumber == ctrlBlockPtr->send_una) && (ctrlBlockPtr->send_wnd == 0U))
      {
        TcpIp_Tcp_startRtxTimer(ctrlBlockPtr);
      }
    }
    else
    {
      uint16 dataToFree = 0U;
      boolean confirm = FALSE;
      uint8 CtrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddrId);
      TCPIP_PRECONDITION_ASSERT(CtrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

      /* !LINKSTO TcpIp.Design.Socket.EA0.RxIndication,1 */
      SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();
      {
        const uint32 usedBuff = ctrlBlockPtr->used_buff;

        /* first check if data was sent, maybe only SYN or FIN segment without
           data was transmitted, Note: A SYN is always sent without data */
        if(usedBuff != 0U)
        {
          /* Deviation MISRAC2012-2 */
          dataToFree = (uint16)(sint32)(ackNumber - ctrlBlockPtr->send_una);

          /* it can happen that acknowledgement number is bigger than
             sent data, due to a FIN at the end of a segment */
          if(dataToFree > usedBuff)
          {
            /* therefore, free all data left, Note: after FIN no data
               exists */
            dataToFree = (uint16)usedBuff;
          }


          /* check if a copying is in progress */
          if(!ctrlBlockPtr->copyOngoing)
          {
            /* if copying is not done, free memory immediately */
            TcpIp_Tcp_freeSocketMemoryData(ctrlBlockPtr, dataToFree);
            ctrlBlockPtr->dataToFree = 0U;
          }
          else
          {
            /* if copying is ongoing save amount of data which shall be freed,
               data is freed after copying is finished */
            ctrlBlockPtr->dataToFree += dataToFree;
          }

          confirm = TRUE;
        }
      }
      /* !LINKSTO TcpIp.Design.Socket.EA0.RxIndication,1 */
      SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

      if(confirm)
      {
        const uint8 localsockOwnerId = socketPtr->generic.aligned.sockOwnerId;

        TCPIP_PRECONDITION_ASSERT
          ((localsockOwnerId < TcpIp_NumConfSockOwner),TCPIP_INTERNAL_API_ID);

        /* confirm acknowledged data */
        TcpIp_TxConfirmationAPI[localsockOwnerId](socketId, dataToFree);
      }

      /* Confirm the reception of an ack from the remote host to the neighbor cache */
      TcpIp_Ip_reachConfirmation(CtrlIdx, remoteAddrPtr);

      /* update unacknowledged data */
      ctrlBlockPtr->send_una = ackNumber;

      /* all outstanding data, control command has been acknowledged */
      if((ctrlBlockPtr->used_buff == 0U) && (ackNumber == ctrlBlockPtr->send_nxt))
      {
        /* turn off retransmission timer (RFC2988 section 5.2) */
        TcpIp_Tcp_stopRtxTimer(ctrlBlockPtr);

        /* clear all flgs and mss option from retransmission queue */
        ctrlBlockPtr->ctrl_flags = TCPIP_TCP_FLAG_CLEAR;
        ctrlBlockPtr->flags &= (~((uint8)TCPIP_TCP_MSS_OPT_FLAG));

        /* stop all retries */
        TcpIp_Tcp_stopRetry(ctrlBlockPtr);
      }
      else
      {
        /* RFC2988 section 5.3: When an ACK is received that acknowledges new data,
           restart the retransmission timer */
        TcpIp_Tcp_startRtxTimer(ctrlBlockPtr);

        /* stop all retries */
        TcpIp_Tcp_stopRetry(ctrlBlockPtr);
      }
    }
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_rxData_SendWindowUpdate
(
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr,
  uint32 seqNumber,
  uint32 ackNumber,
  uint16 sendWindow
)
{
  /* update send window (RFC793 page 72) */
  if (TCPIP_TCP_IS_LOWER_THAN(ctrlBlockPtr->send_wl1, seqNumber) ||
      ((ctrlBlockPtr->send_wl1  == seqNumber) &&
        TCPIP_TCP_IS_LOWER_OR_EQUAL(ctrlBlockPtr->send_wl2, ackNumber)
      )
     )
  {
    /* new window update was received */
    /* the check above prevents old segments from updating the window */
    if
    (
      (ctrlBlockPtr->send_wnd == 0U) &&
      (sendWindow != 0U)
    )
    {
      ctrlBlockPtr->zwpTransmitted = FALSE;
    }
    ctrlBlockPtr->send_wnd = sendWindow;
    ctrlBlockPtr->send_wl1 = seqNumber;
    ctrlBlockPtr->send_wl2 = ackNumber;
  }
}

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#else
  TS_PREVENTEMPTYTRANSLATIONUNIT
#endif /* (TCPIP_TCP_ENABLED == STD_ON) */

/*==================[end of file]===========================================*/
