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
 * MISRAC2012-2) Deviated Rule: 11.3 (required)
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

#include <TcpIp_IpAddrM_Int.h>   /* used SW-unit interface file */
#include <TcpIp_Memory_Int.h>    /* used SW-unit interface file */
#include <TcpIp_Tcp_Priv.h>      /* own SW-unit interface file */

#include <TcpIp_IpSecDb_Int.h>  /* external IpSec Api declarations*/

/*==================[macros]================================================*/

/** \brief Set the source port in TCP header */
#define TCPIP_TCP_SET_SRC_PORT(tcphdr, port)                      \
  TCPIP_SET16((tcphdr), 0, (port))

/** \brief Set the destination port in the TCP header */
#define TCPIP_TCP_SET_DST_PORT(tcphdr, port)                      \
  TCPIP_SET16((tcphdr), 2, (port))

/** \brief Set the sequence number in the TCP header */
#define TCPIP_TCP_SET_SEQNR(tcphdr, seqnr)                        \
  TCPIP_SET32((tcphdr), 4, (seqnr))

/** \brief Set the acknowledgement number in the TCP header */
#define TCPIP_TCP_SET_ACKNR(tcphdr, acknr)                        \
  TCPIP_SET32((tcphdr), 8, (acknr))

/** \brief Set the data offset and reserved field in the TCP header */
#define TCPIP_TCP_SET_OFFSET(tcphdr, offset)                      \
  do                                                              \
  {                                                               \
    (tcphdr)[12U] = ((offset) & 0x0FU) << 4U;                     \
  } while (0)

/** \brief Set the TCP control bits and reserved field in the TCP header */
#define TCPIP_TCP_SET_FLAGS(tcphdr, flags)                        \
  do                                                              \
  {                                                               \
    (tcphdr)[13U] = ((flags) & 0x3FU);                            \
  } while (0)

/** \brief  Set the TCP Window in the TCP header */
#define TCPIP_TCP_SET_WINDOW(tcphdr, window)                      \
  TCPIP_SET16((tcphdr), 14, (window))

/** \brief Set the TCP Checksum in the TCP header */
#define TCPIP_TCP_SET_CHECKSUM(tcphdr, chksum)                    \
  TCPIP_SET16((tcphdr), 16, (chksum))

/** \brief Set the TCP Urgent Pointer in the TCP header */
#define TCPIP_TCP_SET_URGENT_PTR(tcphdr, pointer)                 \
  TCPIP_SET16((tcphdr), 18, (pointer))

/** \brief Set the kind field of an option */
#define TCPIP_TCP_OPT_SET_KIND(tcphdr, kind)                      \
  (tcphdr)[0U] = (kind)

/** \brief Set the length field of an option */
#define TCPIP_TCP_OPT_SET_LENGTH(tcphdr, length)                  \
  (tcphdr)[1U] = (length)

/** \brief Set the mss field of the mss option */
#define TCPIP_TCP_MSS_OPT_SET_MSS(tcphdr, mss)                    \
  TCPIP_SET16((tcphdr), 2, (mss))

/*==================[type definitions]======================================*/

typedef struct
{
  /*  TcpIp_Ip_UpdateHeaderFunctorType polymorphic base class */
  TCPIP_IP_UPDATEHEADER_FIELDS

  /* derived data */
  /**< updateHeaderFctPtr needs the Index into dynamic memory to build its header */
  uint32 memoryOffset;
  /**< updateHeaderFctPtr needs the Tcp ctrlblock to build its header */
  P2CONST(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr;
  /**< updateHeaderFctPtr needs the Tcp local address to build its header */
  P2CONST(TcpIp_SockAddrLocalType, AUTOMATIC, TCPIP_APPL_DATA) localAddr;
  /**< updateHeaderFctPtr needs the Tcp remote address to build its header */
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddr;
  /**< updateHeaderFctPtr needs the control flags to build its header */
  uint8 flags;
} TcpIp_Tcp_UpdateHeaderFunctorType;


typedef struct
{
  /*  TcpIp_Ip_CopyDataFunctorType polymorphic base class */
  TCPIP_IP_COPYDATA_FIELDS

  /**< copyFctPtr needs the Tcp ctrlblock to build its header */
  P2CONST(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr;
  /**< copyFctPtr needs the Index into dynamic memory to build its header */
  uint32 memoryOffset;
} TcpIp_Tcp_CopyDataFunctorType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#if (TCPIP_TCP_ENABLED == STD_ON)

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief Allocate memory from Memory unit
 *
 * This function either retrieves a new buffer if none was previously allocated or
 * increase the existing by size.
 *
 * \param[in]  CtrlBlock  Pointer to the transmission control block
 * \param[in]  size       number of bytes that shall be allocated
 *
 * \return total number of allocated memory
 */
STATIC FUNC(uint32, TCPIP_CODE) TcpIp_Tcp_txData_allocateMemory
(
  TcpIp_TcpCtrlBlockPtrType CtrlBlock,
  uint32 size
);

/** \brief Appends the mss option
 *
 * This function appends the mss option to a segment
 *
 * \param[in]  buffPtr  Points to the beginning of the option field
 * \param[in]  mss      receive MSS value
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_appendMssOption
(
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) buffPtr,
  uint16 mss
);

/** \brief Copy data from Soad
 *
 * This function copies data from Soad.
 *
 * \param[in]     SocketId       id of the socket connection
 * \param[in]     socketOwnerId  upper layer of the socket
 * \param[in]     memoryPtr      Points to the location data shall be copied to
 * \param[in,out] dataLength     in:  Number of bytes to copy
 *                               out: Number of actual copied bytes
 *
 * \retval TCPIP_OK        copying successful
 * \retval TCPIP_E_NOT_OK  <Up>_CopyTxData returned error
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_txData_copyDataFromSoAd
(
  TcpIp_SocketIdType SocketId,
  uint8 socketOwnerId,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) memoryPtr,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) dataLength
);

/** \brief Copy data to dynamic memory
 *
 * This function retrieves a linear buffer from the memory module and copies data from
 * Soad or data from DataPtr to dynamic memory.
 *
 * \param[in]  SocketId       id of the socket connection
 * \param[in]  socketOwnerId  upper layer of the socket
 * \param[in]  idPtr          Pointer to dynamic memory identifier.
 * \param[in]  DataPtr        Points to the location data shall be copied from
 *                            NULL_PTR: copy data from Soad
 *                            else: copy data from linear buffer
 * \param[in]  index          Index into dynamic memory to write the data to.
 * \param[in]  length         Number of bytes to copy
 *
 * \return Number of copied bytes
 */
STATIC FUNC(uint32, TCPIP_CODE) TcpIp_Tcp_txData_bufferData
(
  TcpIp_SocketIdType SocketId,
  uint8 socketOwnerId,
  P2VAR(void, AUTOMATIC, TCPIP_APPL_DATA) idPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint32 index,
  uint32 length
);

/** \brief transmit data and control
 *
 * This function transmits data segments and TCP control commands.
 *
 * \param[in]  socketPtr     Pointer to connection information
 * \param[in]  dataLen       payload length to transmit
 * \param[in]  index         Index into dynamic memory to read the data from.
 *
 * \retval TCPIP_OK        transmission successful
 * \retval TCPIP_E_NOT_OK  error occurred during transmission, reset connection
 * \retval TCPIP_E_DROP    not enough memory available, retry in next mainfunction
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_txData_send
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  uint16 dataLen,
  uint16 index
);

/** \brief evaluate next sequence number
 *
 * This function evaluates the sequence number which is used in the next segment.
 *
 * \param[in]  sequence  current next sequence number
 * \param[in]  dataLen   length of the data that was transmitted
 * \param[in]  index     index into dynamic memory to read the data from.
 * \param[in]  flags         control flags that shall be transmitted in the segment
 *
 * \return next sequence number
 */
STATIC FUNC(uint32, TCPIP_CODE) TcpIp_Tcp_txData_evaluateSendNext
(
  uint32 sentSeq,
  uint32 dataLen,
  uint32 index,
  uint8 flags
);

/** \brief Transmit single TCP segment
 *
 * This function retrieves a buffer from Ethernet driver, builds the TCP header, copies
 * data from the dynamic memory to the buffer and transmits the segment.
 *
 * \param[in]      ctrlBlockPtr  Pointer to transmission control block
 * \param[in]      ipParameters  Points to a structure containing Ip communication
 *                               parameters
 * \param[in]      localAddr     Pointer to local address and local port
 * \param[in]      remoteAddr    Pointer to remote ip address and remote port
 * \param[in,out]  dataLen       payload length to transmit
 * \param[in]      index         Index into dynamic memory to read the data from.
 * \param[in]      flags         control flags that shall be transmitted in the segment
 *
 * \retval TCPIP_OK        transmission successful
 * \retval TCPIP_E_NOT_OK  error occurred during transmission, reset connection
 * \retval TCPIP_E_DROP    not enough memory available, retry in next mainfunction
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_txData_sendSeg
(
  P2CONST(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr,
  P2CONST(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameters,
  P2CONST(TcpIp_SockAddrLocalType, AUTOMATIC, TCPIP_APPL_DATA) localAddr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddr,
  uint16 dataLen,
  uint32 index,
  uint8 flags
);

/** \brief Evaluates offset of the TCP payload
 *
 * This function evaluates the beginning of the payload in a segment.
 *
 * \param[in]  options  options that shall be transmitted in the segment
 *
 * \return offset of the TCP payload
 */
STATIC FUNC(uint16, TCPIP_CODE) TcpIp_Tcp_txData_getDataOffset
(
  uint8 options
);

/** \brief Evaluate segment size
 *
 * This function evaluates the number of data a segment shall contain. It considers the
 * maximal segment size and the current send window during calculation.
 *
 * \param[in]  totalDataLength  data length that shall be transmitted
 * \param[in]  ctrlBlockPtr  Pointer to transmission control block
 *
 * \return data length of the segment
 */
STATIC FUNC(uint16, TCPIP_CODE) TcpIp_Tcp_txData_evaluateDataLength
(
  uint16 totalDataLength,
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr
);


/** \brief Transmit reset
 *
 * This function builds and transmits a reset segment.
 *
 * \param[in]  ipParameter    Points to a structure containing Ip communication parameters
 * \param[in]  remoteAddrPtr  IP Address and port of the remote host
 * \param[in]  localAddrPtr   IP Address and port of the local host
 * \param[in]  sendNext       sequence number of the segment
 * \param[in]  receiveNext    acknowledgement number of the segment
 * \param[in]  flags          control flags
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_txData_buildRst
(
  P2CONST(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameter,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(TcpIp_SockAddrLocalType, AUTOMATIC, TCPIP_APPL_DATA) localAddrPtr,
  uint32 sendNext,
  uint32 receiveNext,
  uint8 flags
);

/** \brief Evaluates when to send data
 *
 * This function evaluates the number of bytes to be sent according to the
 * nagle algorithm.
 *
 * \param[in]  ctrlBlockPtr  Pointer to transmission control block
 * \param[in]  availWindow   available window
 * \param[in]  dataLen       data length to transmit
 *
 * \return number of bytes to transmit
 *         0        - only buffer data
 *         dataLen  - data to transmit
 */
TS_MOD_PRIV_DEFN FUNC(uint16, TCPIP_CODE) TcpIp_Tcp_performNagleAlgorithm
(
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr,
  uint16 availWindow,
  uint16 dataLen
);

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

#define TCPIP_START_SEC_VAR_INIT_32
#include <TcpIp_MemMap.h>

/** \brief The ID, that IpV4 needs to use for the packet. */
TCPIP_IP_DEFINEFRAGIDCOUNTER(TcpIp_Tcp_fragmentIdCounter)

#define TCPIP_STOP_SEC_VAR_INIT_32
#include <TcpIp_MemMap.h>

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_txData_transmit
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  TcpIp_ReturnType retVal;
  const TcpIp_LocalAddrIdType localAddrId = socketPtr->generic.aligned.local.addrId;
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr =
    &socketPtr->tcp;
  const uint16 unsentIndex = ctrlBlockPtr->curr_buff;
  uint16 unsentLength;

  if(TcpIp_IpAddrM_lockIfOnline(localAddrId))
  {
#if (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON)
    if(TCPIP_TCP_FLAG_KEEP_ALIVE_IS_SET(ctrlBlockPtr->flags))
    {
      /* transmission ongoing, do not check for idle connection */
      TcpIp_Tcp_stopStateTimer(ctrlBlockPtr);
    }
#endif /* (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON) */

    /* unmark transmission request in case controller is online */
    ctrlBlockPtr->flags &= ~((uint8) TCPIP_TCP_TRANSMIT_FLAG);

    if((ctrlBlockPtr->used_buff - unsentIndex) > TCPIP_TCP_MAX_WINDOW_SIZE)
    {
      /* transmit maximal send window */
      unsentLength = TCPIP_TCP_MAX_WINDOW_SIZE;
    }
    else
    {
      unsentLength = (uint16) ctrlBlockPtr->used_buff - unsentIndex;
    }

    if(unsentLength != 0U)
    {

      SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

      /* stop the request of a delayed acknowledgement, data will be acknowledged
         with data transmission */
      TcpIp_Tcp_stopAckRetranTimer(ctrlBlockPtr);

      SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

      /* start the retransmission timer */
      if(!TcpIp_Tcp_rtxTimerRunning(ctrlBlockPtr))
      {
        TcpIp_Tcp_startRtxTimer(ctrlBlockPtr);
      }

      retVal = TcpIp_Tcp_txData_send(socketPtr, unsentLength, unsentIndex);
    }
    else
    {
      retVal = TCPIP_OK;
    }

    TcpIp_IpAddrM_unlockIf(localAddrId);
  }
  else
  {
    /* ignore transmission of data */
    retVal = TCPIP_OK;
  }

  return retVal;
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_txData_buff
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint32 AvailableLength,
  boolean ForceRetrieve
)
{
  /* out of memory, data cannot be buffered */
  TcpIp_ReturnType retVal;
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr = &socketPtr->tcp;

  /* !LINKSTO TcpIp.Design.Socket.EA0.TcpTransmit,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  /* indicate that copying is ongoing */
  ctrlBlockPtr->copyOngoing = TRUE;

  /* !LINKSTO TcpIp.Design.Socket.EA0.TcpTransmit,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();
  {
    uint32 copyLength = AvailableLength;

    /* TCP can decide how much data to retrieve from Soad */
#if (TCPIP_TCP_COPY_WINDOW_CHECK_ENABLED == STD_ON)
    if((DataPtr == NULL_PTR) && (!ForceRetrieve))
    {
      uint16 availWindow = 0U;

      if(ctrlBlockPtr->send_wnd >= ctrlBlockPtr->send_buff)
      {
        /* first check how much data is accepted by the remote host */
        availWindow = ctrlBlockPtr->send_wnd - ctrlBlockPtr->send_buff;
      }

      if(availWindow != 0U)
      {
        if(copyLength > availWindow)
        {
          /* only buffer as much data as remote host is willing to accept */
          copyLength = availWindow;
        }
      }
      else
      {
        if(ctrlBlockPtr->send_buff == 0U)
        {
          /* no data is currently transmitted, to get window update, perform zero window
             probe */
          copyLength = 1U;
        }
        else
        {
          /* do not copy any data */
          copyLength = 0U;
        }
      }
    }
#endif /* (TCPIP_TCP_COPY_WINDOW_CHECK_ENABLED == STD_ON) */

    if(copyLength != 0U)
    {
      uint32 availBuff = ctrlBlockPtr->avail_buff;
      uint32 FreeBuffSize = availBuff - ctrlBlockPtr->used_buff;

      /* check if enough data is reserved for transmission */
      if(FreeBuffSize < copyLength)
      {
        /* retrieve additional space for buffering */
        availBuff =
          TcpIp_Tcp_txData_allocateMemory(ctrlBlockPtr, copyLength - FreeBuffSize);

        TCPIP_PRECONDITION_ASSERT(availBuff >= ctrlBlockPtr->avail_buff, DET_INTERNAL_API_ID);

        ctrlBlockPtr->avail_buff = availBuff;

        /* recalculate the free buffer size */
        FreeBuffSize = availBuff - ctrlBlockPtr->used_buff;
      }

      if(availBuff != 0U)
      {
        /* check if complete data fits into buffer, or TcpIp can decide
           how much to retrieve */
        if((FreeBuffSize >= copyLength) || ((DataPtr == NULL_PTR) && (!ForceRetrieve)))
        {
          if((DataPtr == NULL_PTR) && (!ForceRetrieve))
          {
            if(FreeBuffSize < copyLength)
            {
              /* if Tcp can decide how much data to copy and complete data
                 does not fit, copy as much data as free buffer is available */
              copyLength = FreeBuffSize;
            }
          }

          if(copyLength != 0U)
          {
            /* copy data into dynamic memory */
            ctrlBlockPtr->used_buff +=
               TcpIp_Tcp_txData_bufferData
                       (
                        socketId,
                        socketPtr->generic.aligned.sockOwnerId,
                        ctrlBlockPtr->memory_idPtr,
                        DataPtr,
                        ctrlBlockPtr->used_buff,
                        copyLength
                       );
          }
          retVal = TCPIP_OK;
        }
        else
        {
          /* requested number of bytes cannot be buffered */
          retVal = TCPIP_E_MEMORY;
        }
      }
      else
      {
        /* no buffer id left */
        retVal = TCPIP_E_MEMORY;
      }
    }
    else
    {
      /* no data to copy */
      retVal = TCPIP_OK;
    }
  }

  /* !LINKSTO TcpIp.Design.Socket.EA0.TcpTransmit,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  /* indicate that copying finished */
  ctrlBlockPtr->copyOngoing = FALSE;

  if(ctrlBlockPtr->dataToFree != 0U)
  {
    /* if there is any unused data remaining, free it after copying is finished */
    TcpIp_Tcp_freeSocketMemoryData(ctrlBlockPtr, ctrlBlockPtr->dataToFree);
    ctrlBlockPtr->dataToFree = 0U;
  }
  /* !LINKSTO TcpIp.Design.Socket.EA0.TcpTransmit,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  return retVal;
}

#if (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON)

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Tcp_txData_sendTempSyn
(
  P2CONST(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameter,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(TcpIp_SockAddrLocalType, AUTOMATIC, TCPIP_APPL_DATA) localAddrPtr,
  uint32 sendNext,
  uint32 receiveNext,
  uint16 window,
  uint16 mss
)
{
  TcpIp_TcpCtrlBlockType ctrlBlock;
  uint8 Flags;

  /* a socket needs the following information to transmit a SYN/ACK */
  if(TcpIp_IpAddrM_lockIfOnline(localAddrPtr->addrId))
  {
    /* sequence number */
    ctrlBlock.send_una = sendNext;
    ctrlBlock.send_nxt = sendNext;
    /* acknowledgement number */
    ctrlBlock.recv_nxt = receiveNext;
    /* window set to 0 */
    ctrlBlock.recv_wnd = window;
    /* mss options */
    ctrlBlock.flags = TCPIP_TCP_MSS_OPT_FLAG;
    ctrlBlock.recv_mss = mss;
    /* reset flag set */
    Flags = TCPIP_TCP_FLAG_SYN | TCPIP_TCP_FLAG_ACK;

    (void)
      TcpIp_Tcp_txData_sendSeg
        (&ctrlBlock, ipParameter, localAddrPtr, remoteAddrPtr, 0U, 0U, Flags);

    TcpIp_IpAddrM_unlockIf(localAddrPtr->addrId);
  }
}

#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON) */

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_txData_sendSyn
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  boolean ack
)
{
  const TcpIp_LocalAddrIdType localAddrId = socketPtr->generic.aligned.local.addrId;
  TcpIp_ReturnType result = TCPIP_E_NOT_OK;

  if(TcpIp_IpAddrM_lockIfOnline(localAddrId))
  {
    P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr =
        &socketPtr->tcp;
    uint8 flags = TCPIP_TCP_FLAG_SYN;

    if(ack)
    {
      /* transmit a SYN, ACK */
      flags |= TCPIP_TCP_FLAG_ACK;
    }

    /* SYN segment shall contain the MSS option */
    ctrlBlockPtr->flags |= TCPIP_TCP_MSS_OPT_FLAG;

    /* mark flags for retransmission */
    ctrlBlockPtr->ctrl_flags |= flags;

    /* start the retransmission timer */
    TcpIp_Tcp_startRtxTimer(ctrlBlockPtr);

    result = TcpIp_Tcp_txData_send(socketPtr, 0U, 0U);

    TcpIp_IpAddrM_unlockIf(localAddrId);
  }

  return result;
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_txData_sendFin
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr =
      &socketPtr->tcp;
  const uint16 unsentIndex = ctrlBlockPtr->curr_buff;
  uint16 unsentLenght;

  if((ctrlBlockPtr->used_buff - unsentIndex) > TCPIP_TCP_MAX_WINDOW_SIZE)
  {
    /* transmit maximal send window */
    unsentLenght = TCPIP_TCP_MAX_WINDOW_SIZE;
  }
  else
  {
    unsentLenght = (uint16) ctrlBlockPtr->used_buff - unsentIndex;
  }

  /* mark flags for retransmission */
  ctrlBlockPtr->ctrl_flags = (TCPIP_TCP_FLAG_FIN | TCPIP_TCP_FLAG_ACK);

  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  /* stop the request of a delayed acknowledgement, data will be acknowledged
     with FIN segment */
  TcpIp_Tcp_stopAckRetranTimer(ctrlBlockPtr);

  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  /* start the retransmission timer */
  TcpIp_Tcp_startRtxTimer(ctrlBlockPtr);

  return TcpIp_Tcp_txData_send(socketPtr, unsentLenght, unsentIndex);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Tcp_txData_sendRstAck
(
  P2CONST(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameter,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(TcpIp_SockAddrLocalType, AUTOMATIC, TCPIP_APPL_DATA) localAddrPtr,
  uint32 ackNum
)
{
  TcpIp_Tcp_txData_sendRst
    (ipParameter, remoteAddrPtr, localAddrPtr, ackNum, 0U, 0U, TCPIP_TCP_FLAG_ACK);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Tcp_txData_sendRst
(
  P2CONST(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameter,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(TcpIp_SockAddrLocalType, AUTOMATIC, TCPIP_APPL_DATA) localAddrPtr,
  uint32 ackNum,
  uint32 seqNum,
  uint16 seglen,
  uint8 flags
)
{
  if(TcpIp_IpAddrM_lockIfOnlineShutdown(localAddrPtr->addrId))
  {
    uint32 sendNext, receiveNext;
    uint8 controlFlags = TCPIP_TCP_FLAG_CLEAR;

    if(TCPIP_TCP_FLAG_ACK_IS_SET(flags))
    {
      /* received segment contains an ACK */
      sendNext = ackNum;
      receiveNext = 0U;
    }
    else
    {
      /* received segment does not have an ACK */
      sendNext = 0U;
      receiveNext = seqNum + seglen;
      controlFlags = TCPIP_TCP_FLAG_ACK;
    }

    TcpIp_Tcp_txData_buildRst
      (ipParameter, remoteAddrPtr, localAddrPtr, sendNext, receiveNext, controlFlags);

    TcpIp_IpAddrM_unlockIf(localAddrPtr->addrId);
  }
}
#if (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON)

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_txData_sendKeepAlive
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  const TcpIp_LocalAddrIdType localAddrId = socketPtr->generic.aligned.local.addrId;
  TcpIp_ReturnType result;

  if(TcpIp_IpAddrM_lockIfOnline(localAddrId))
  {
    P2CONST(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr = &socketPtr->tcp;
    TcpIp_TcpCtrlBlockType keepAliveCtrlBlock;
    const uint8 flags = TCPIP_TCP_FLAG_ACK;

    /* transit segment with sequence number that was already used */
    keepAliveCtrlBlock.send_una = ctrlBlockPtr->send_una - 1U;

    keepAliveCtrlBlock.recv_nxt = ctrlBlockPtr->recv_nxt;
    keepAliveCtrlBlock.recv_wnd = ctrlBlockPtr->recv_wnd;
    keepAliveCtrlBlock.flags = TCPIP_TCP_FLAG_CLEAR;

    result = TcpIp_Tcp_txData_sendSeg
                               (
                                &keepAliveCtrlBlock,
                                &socketPtr->generic.aligned.ipParameters,
                                &socketPtr->generic.aligned.local,
                                &socketPtr->remote,
                                0U,
                                0U,
                                flags
                               );

    TcpIp_IpAddrM_unlockIf(localAddrId);
  }
  else
  {
    /* ignore transmission of keep alive */
    result = TCPIP_OK;
  }

  return result;
}

#endif /* (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON) */

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_txData_sendAck
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  const TcpIp_LocalAddrIdType localAddrId = socketPtr->generic.aligned.local.addrId;
  TcpIp_ReturnType result;

  /* if state is ONHOLD we handle it like ACK has been sent, remote host will
     retransmit segments if ACK is not received */
  if(TcpIp_IpAddrM_lockIfOnlineShutdown(localAddrId))
  {
    P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr =
      &socketPtr->tcp;
    const uint8 flags = TCPIP_TCP_FLAG_ACK;
    /* use next sequence number in stream */
    const uint32 currentSeq = ctrlBlockPtr->send_nxt - ctrlBlockPtr->send_una;

    result = TcpIp_Tcp_txData_sendSeg
                                  (
                                   &socketPtr->tcp,
                                   &socketPtr->generic.aligned.ipParameters,
                                   &socketPtr->generic.aligned.local,
                                   &socketPtr->remote,
                                   0U,
                                   currentSeq,
                                   flags
                                  );

    if(result == TCPIP_OK)
    {
      SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

      /* stop delayed ack or ack retry */
      TcpIp_Tcp_stopAckRetranTimer(ctrlBlockPtr);

      SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

      ctrlBlockPtr->num_unack = 0U;
    }

    TcpIp_IpAddrM_unlockIf(localAddrId);
  }
  else
  {
    /* ignore transmission of ack */
    result = TCPIP_OK;
  }

  return result;
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_txData_retransmitUnsent
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  TcpIp_ReturnType result;
  const TcpIp_LocalAddrIdType localAddrId = socketPtr->generic.aligned.local.addrId;

  /* if state is ONHOLD we handle it like segment has not been sent */
  if(TcpIp_IpAddrM_lockIfOnlineShutdown(localAddrId))
  {
    P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr =
      &socketPtr->tcp;
    uint16 unsentIndex = ctrlBlockPtr->curr_buff;
    uint16 unsentLenght;

    if(TCPIP_TCP_FLAG_NAGLE_IS_SET(ctrlBlockPtr->flags) &&
       (ctrlBlockPtr->curr_buff != ctrlBlockPtr->send_buff)
      )
    {
      /* in case of nagle algorithm, retransmission ongoing, only retransmit
         already transmitted data */
      unsentLenght = ctrlBlockPtr->send_buff - unsentIndex;
    }
    else
    {
      if((ctrlBlockPtr->used_buff - unsentIndex) > TCPIP_TCP_MAX_WINDOW_SIZE)
      {
        /* transmit maximal send window */
        unsentLenght = TCPIP_TCP_MAX_WINDOW_SIZE;
      }
      else
      {
        /* no retransmission ongoing, send all buffered data */
        unsentLenght = (uint16) ctrlBlockPtr->used_buff - unsentIndex;
      }
    }

    result = TcpIp_Tcp_txData_send(socketPtr, unsentLenght, unsentIndex);

    TcpIp_IpAddrM_unlockIf(localAddrId);
  }
  else
  {
    /* ignore transmission of unsent data */
    result = TCPIP_E_DROP;
  }

  return result;
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_txData_retransmit
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  TcpIp_ReturnType result;
  const TcpIp_LocalAddrIdType localAddrId = socketPtr->generic.aligned.local.addrId;
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr =
    &socketPtr->tcp;

  /* restart retransmission timeout */
  ctrlBlockPtr->rtx_timer = 0U;

  /* increment number of retransmissions */
  ctrlBlockPtr->num_rtx++;

  if(TcpIp_IpAddrM_lockIfOnlineShutdown(localAddrId))
  {
    uint16 dataLen;

    if(TCPIP_TCP_FLAG_NAGLE_IS_SET(ctrlBlockPtr->flags))
    {
      /* only retransmit already transmitted data */
      dataLen = ctrlBlockPtr->send_buff;
    }
    else
    {
      if(ctrlBlockPtr->used_buff > TCPIP_TCP_MAX_WINDOW_SIZE)
      {
        /* transmit maximal send window */
        dataLen = TCPIP_TCP_MAX_WINDOW_SIZE;
      }
      else
      {
        /* transmit all buffered data */
        dataLen = (uint16) ctrlBlockPtr->used_buff;
      }
    }

    /* retransmit all buffered data */
    ctrlBlockPtr->curr_buff = 0U;
    /* mention that zero-window probe wasn't transmitted (retransmission needed) */
    ctrlBlockPtr->zwpTransmitted = FALSE;

    result = TcpIp_Tcp_txData_send(socketPtr, dataLen, 0U);

    TcpIp_IpAddrM_unlockIf(localAddrId);
  }
  else
  {
    /* ignore retransmission of data */
    result = TCPIP_OK;
  }

  return result;
}

TS_MOD_PRIV_DEFN FUNC(BufReq_ReturnType, TCPIP_CODE) TcpIp_Tcp_copyData
(
    P2VAR(TcpIp_Ip_CopyDataFunctorType, AUTOMATIC, TCPIP_APPL_DATA) that,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) bufferPtr,
    uint16 copyLength
)
{
  /* Deviation MISRAC2012-2 <+2> */
  P2VAR(TcpIp_Tcp_CopyDataFunctorType, AUTOMATIC, TCPIP_APPL_DATA) this =
    (P2VAR(TcpIp_Tcp_CopyDataFunctorType, AUTOMATIC, TCPIP_APPL_DATA)) that;

  DBG_TCPIP_TCP_COPYDATA_ENTRY(that, bufferPtr, copyLength);

  /* copy data */
  if(copyLength > 0U)
  {
    TcpIp_Memory_get
              ( this->ctrlBlockPtr->memory_idPtr,
                bufferPtr,
                this->memoryOffset,
                (copyLength)
              );
  }

  DBG_TCPIP_TCP_COPYDATA_EXIT(BUFREQ_OK, that, bufferPtr, copyLength);

  return BUFREQ_OK;
}


TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Tcp_updateHeader
(
  P2VAR(TcpIp_Ip_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA) that,
  uint16 payloadSum,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) headerPtr,
  boolean calculateChecksum
)
{
  /* Deviation MISRAC2012-2 <+2> */
  P2VAR(TcpIp_Tcp_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA) this=
    (P2VAR(TcpIp_Tcp_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA)) that;

  const uint16 offset = TcpIp_Tcp_txData_getDataOffset(this->ctrlBlockPtr->flags);

  DBG_TCPIP_TCP_UPDATEHEADER_ENTRY(that, payloadSum, headerPtr, calculateChecksum);

  if(TCPIP_TCP_FLAG_MSS_OPT_IS_SET(this->ctrlBlockPtr->flags))
  {
    /* evaluate the maximal segment size TCP is willing to accept */
    const uint16 mss = this->ctrlBlockPtr->recv_mss;

    /* append mss to the segment */
    TcpIp_Tcp_appendMssOption(&headerPtr[TCPIP_TCP_HEADER_SIZE], mss);
  }

  {
    /* Deviation MISRAC2012-2 <+2> */
    P2CONST(TcpIp_SockAddrInetType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrInetPtr =
      (P2CONST(TcpIp_SockAddrInetType, AUTOMATIC, TCPIP_APPL_DATA)) this->remoteAddr;

    /* set fields of the TCP header */
    TCPIP_TCP_SET_SRC_PORT(headerPtr, this->localAddr->port);
    TCPIP_TCP_SET_DST_PORT(headerPtr, remoteAddrInetPtr->port);
    TCPIP_TCP_SET_SEQNR(headerPtr, this->ctrlBlockPtr->send_una + this->memoryOffset);
    TCPIP_TCP_SET_ACKNR(headerPtr, this->ctrlBlockPtr->recv_nxt);
    TCPIP_TCP_SET_OFFSET(headerPtr, (uint8)(offset / 4U));
    TCPIP_TCP_SET_FLAGS(headerPtr, this->flags);
    TCPIP_TCP_SET_WINDOW(headerPtr, this->ctrlBlockPtr->recv_wnd);
    TCPIP_TCP_SET_CHECKSUM(headerPtr, (uint16) 0U);
    TCPIP_TCP_SET_URGENT_PTR(headerPtr, (uint16) 0U);
  }

  /* calculate checksum of TCP header (initial value = pseudo + payload) */
  if(calculateChecksum)
  {
    const uint16 checksum =
      TcpIp_CalcChecksum16Bit_withInitValue(headerPtr, payloadSum, offset);

    /* set checksum in TCP header */
    TCPIP_TCP_SET_CHECKSUM(headerPtr, checksum);
  }

  DBG_TCPIP_TCP_UPDATEHEADER_EXIT(that, payloadSum, headerPtr, calculateChecksum);
}

/*==================[internal function definitions]=========================*/


STATIC FUNC(uint32, TCPIP_CODE) TcpIp_Tcp_txData_allocateMemory
(
  TcpIp_TcpCtrlBlockPtrType CtrlBlock,
  uint32 size
)
{
  uint32 AllocatedSize;

  /* check if there is some dynamic memory in use */
  if(CtrlBlock->avail_buff == 0U)
  {
    AllocatedSize = size;

    /* no buffer in use, try to allocate AvailableLength bytes */
    CtrlBlock->memory_idPtr = TcpIp_Memory_new(&AllocatedSize);

    if(CtrlBlock->memory_idPtr == TCPIP_MEMORY_IDPTR_INVALID)
    {
      AllocatedSize = 0U;
    }
  }
  else
  {
    /* previously allocated buffer cannot hold all data, try to allocate more */
    AllocatedSize =
        TcpIp_Memory_increase(CtrlBlock->memory_idPtr, size);
  }

  return AllocatedSize;
}


STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_appendMssOption
(
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) buffPtr,
  uint16 mss
)
{
  TCPIP_TCP_OPT_SET_KIND(buffPtr, TCPIP_TCP_OPT_KIND_MSS);
  TCPIP_TCP_OPT_SET_LENGTH(buffPtr, TCPIP_TCP_MSS_OPT_LEN);
  TCPIP_TCP_MSS_OPT_SET_MSS(buffPtr, mss);
}

STATIC FUNC(uint16, TCPIP_CODE) TcpIp_Tcp_txData_evaluateDataLength
(
  uint16 totalDataLength,
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr
)
{
  uint16 availWindow = 0U;
  uint16 segLength = 0U;

  /* check for negative windows */
  TCPIP_PRECONDITION_ASSERT
    (ctrlBlockPtr->send_wnd >=  ctrlBlockPtr->curr_buff, TCPIP_INTERNAL_API_ID);
  availWindow = (ctrlBlockPtr->send_wnd - ctrlBlockPtr->curr_buff);

  if(availWindow > 0U)
  {
    segLength = totalDataLength;

    if(TCPIP_TCP_FLAG_NAGLE_IS_SET(ctrlBlockPtr->flags))
    {
      segLength = TcpIp_Tcp_performNagleAlgorithm(ctrlBlockPtr, availWindow, segLength);
    }
    else /* send as much data as possible */
    {
      if(segLength > ctrlBlockPtr->send_mss)
      {
        /* data to transmit is bigger than mss,
           allocate buffer equal to mss */
        segLength = ctrlBlockPtr->send_mss;
      }

      if(segLength > availWindow)
      {
        /* data to transmit is bigger than send window,
           allocate buffer equal to send window */
        segLength = availWindow;
      }
    }
  }
  else /* send window is negative or 0 */
  {
    if((ctrlBlockPtr->curr_buff == 0U) && (ctrlBlockPtr->zwpTransmitted == FALSE))
    {
      /* no data queued, perform zero window probe */
      segLength = 1U;
    }
  }

  return segLength;
}

STATIC FUNC(uint16, TCPIP_CODE) TcpIp_Tcp_txData_getDataOffset
(
  uint8 options
)
{
  uint16 offset = TCPIP_TCP_HEADER_SIZE;

  if(TCPIP_TCP_FLAG_MSS_IS_SET(options))
  {
    /* consider option length for buffer allocation */
    offset += TCPIP_TCP_MSS_OPT_LEN;
  }

  return offset;
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_txData_sendSeg
(
  P2CONST(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr,
  P2CONST(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameters,
  P2CONST(TcpIp_SockAddrLocalType, AUTOMATIC, TCPIP_APPL_DATA) localAddr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddr,
  uint16 dataLen,
  uint32 index,
  uint8 flags
)
{
  TcpIp_ReturnType result;
  const uint16 offset = TcpIp_Tcp_txData_getDataOffset(ctrlBlockPtr->flags);
  TcpIp_Tcp_UpdateHeaderFunctorType updateHeaderFunctor;
  TcpIp_Tcp_CopyDataFunctorType copyDataFunctor;
  TcpIp_Ip_IpParameterType ipParametersTransmit = *ipParameters;
  const uint8 ctrlIdx = TcpIp_IpAddrM_getCtrlIdx(localAddr->addrId);
  /* Deviation MISRAC2012-2 <+4> */
  P2VAR(TcpIp_Ip_CopyDataFunctorType, AUTOMATIC, TCPIP_APPL_DATA) copyIpDataFunctor =
    (P2VAR(TcpIp_Ip_CopyDataFunctorType, AUTOMATIC, TCPIP_APPL_DATA)) &copyDataFunctor;
  P2VAR(TcpIp_Ip_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA) updateIpHeaderFunctor =
    (P2VAR(TcpIp_Ip_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA)) &updateHeaderFunctor;
  boolean checksumOffload = TCPIP_GET_ENABLE_OFFLOAD_CHECKSUM_TCP(ctrl,ctrlIdx);

  TCPIP_PRECONDITION_ASSERT(ctrlIdx < TCPIP_NUM_ETHIFCTRL, TCPIP_INTERNAL_API_ID);

  copyDataFunctor.ctrlBlockPtr = ctrlBlockPtr;
  copyDataFunctor.memoryOffset = index;
  copyDataFunctor.copyDataFctPtr = &TcpIp_Tcp_copyData;

  /* These are used to forward required data to the TCP header-construction. */
  updateHeaderFunctor.ctrlBlockPtr = ctrlBlockPtr;
  updateHeaderFunctor.flags = flags;
  updateHeaderFunctor.memoryOffset = index;
  updateHeaderFunctor.localAddr = localAddr;
  updateHeaderFunctor.remoteAddr = remoteAddr;
  updateHeaderFunctor.updateHeaderFctPtr = &TcpIp_Tcp_updateHeader;

  /* TCP does need the ID as sequence nr - no bandwidth limitation */
  ipParametersTransmit.mayFragment= TRUE;
  ipParametersTransmit.calculateChecksum = TCPIP_CALCULATE_CHECKSUM(TRUE, checksumOffload);

  TCPIP_IP_GETNEXTID(TCPIP_IP_DOMAIN_GET(remoteAddr->domain),
      ipParametersTransmit.identifier, TcpIp_Tcp_fragmentIdCounter);

  result = TcpIp_Ip_transmit
              (
                copyIpDataFunctor,
                updateIpHeaderFunctor,
                &ipParametersTransmit,
                localAddr->addrId,
                remoteAddr,
                NULL_PTR,
                offset,
                dataLen
              );

  return result;
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_txData_send
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  uint16 dataLen,
  uint16 index
)
{
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr = &socketPtr->tcp;
  TcpIp_ReturnType result;
  uint16 currDataLen = dataLen;
  uint16 currIndex = index;
  uint8 flags;

  do
  {
    uint16 segDataLen = 0U;

    if(currDataLen != 0U)
    {
      segDataLen = TcpIp_Tcp_txData_evaluateDataLength(currDataLen, ctrlBlockPtr);

      if(segDataLen == 0U)
      {
        /* remote host does not accept any data, abort transmission */
        result = TCPIP_OK;
        break;
      }

      /* data segment always contains ack flag */
      flags = TCPIP_TCP_FLAG_ACK;

      /* check for last segment */
      if((currDataLen - segDataLen) == 0U)
      {
        /* set the Push flag at the last segment in a stream */
        flags |= TCPIP_TCP_FLAG_PSH;

        if(TCPIP_TCP_FLAG_FIN_IS_SET(ctrlBlockPtr->ctrl_flags))
        {
          /* set the Fin flag at the last segment in a stream if application requested
             close */
          flags |= TCPIP_TCP_FLAG_FIN;
        }
      }
    }
    else
    {
      /* Note: SYN does not contain data, this function is not used to transmit RST */
      flags = ctrlBlockPtr->ctrl_flags;
    }

    result = TcpIp_Tcp_txData_sendSeg
                                  (
                                   ctrlBlockPtr,
                                   &socketPtr->generic.aligned.ipParameters,
                                   &socketPtr->generic.aligned.local,
                                   &socketPtr->remote,
                                   segDataLen,
                                   currIndex,
                                   flags
                                  );

    if(result == TCPIP_OK)
    {
      const uint32 sentSeq = ctrlBlockPtr->send_nxt - ctrlBlockPtr->send_una;

      /* evaluate next sequence number to be used */
      ctrlBlockPtr->send_nxt +=
        TcpIp_Tcp_txData_evaluateSendNext(sentSeq, segDataLen, currIndex, flags);

      /* mark how much data was sent */
      ctrlBlockPtr->curr_buff += segDataLen;

      if(ctrlBlockPtr->curr_buff > ctrlBlockPtr->send_buff)
      {
        /* if more data is sent than retransmitted */
        ctrlBlockPtr->send_buff = ctrlBlockPtr->curr_buff;
      }

      /* update remaining data */
      currDataLen -= segDataLen;
      /* update currIndex in dynamic memory */
      currIndex += segDataLen;

      if(((segDataLen == 1U) && (ctrlBlockPtr->send_wnd == 0U)))
      {
        /* Zero probe, sent only once, remove from send buffer in
         * order to transmit everything when window is obtained */
        ctrlBlockPtr->curr_buff -= segDataLen;
        ctrlBlockPtr->zwpTransmitted = TRUE;
      }
    }

  } while((currDataLen > 0U) && (result == TCPIP_OK) && (ctrlBlockPtr->zwpTransmitted == FALSE));

  return result;
}

STATIC FUNC(uint32, TCPIP_CODE) TcpIp_Tcp_txData_evaluateSendNext
(
  uint32 sentSeq,
  uint32 dataLen,
  uint32 index,
  uint8 flags
)
{
  uint32 newSent = dataLen;
  uint32 sequence = sentSeq;

  /* verify if some of the data was already sent */
  if(index < sequence)
  {
    /* get the length of already sent data */
    sequence -= index;
    newSent = 0U;

    /* verify if part of the data has not been sent yet */
    if(dataLen > sequence)
    {
      newSent = dataLen - sequence;
    }
  }

  /* check if SYN or FIN has already been sent */
  if((index + dataLen) == (sequence + newSent))
  {
    newSent += TcpIp_Tcp_evaluateSegLen(flags);
  }

  return newSent;
}

STATIC FUNC(uint32, TCPIP_CODE) TcpIp_Tcp_txData_bufferData
(
  TcpIp_SocketIdType SocketId,
  uint8 socketOwnerId,
  P2VAR(void, AUTOMATIC, TCPIP_APPL_DATA) idPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint32 index,
  uint32 length
)
{
  TcpIp_ReturnType retVal = TCPIP_E_NOT_OK;
  uint32 currMemIndex = index;
  uint32 currlength = length;
  uint32 currDataIndex = 0U;

  /* copy all data into memory, request buffers until all data is stored in dynamic
     memory Note: enough buffer to hold all data is guaranteed through previous
     allocation */
  do
  {
    uint32 size;
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) memoryPtr;

    /* Retrieve linear buffer from memory unit */
    memoryPtr = TcpIp_Memory_getLinearMemoryAddress(idPtr, currMemIndex, &size);

    TCPIP_PRECONDITION_ASSERT(size > 0U, DET_INTERNAL_API_ID);

    /* evaluate length to copy, buffer returned by memory module may not be big enough
       to hold all data */
    if(size > currlength)
    {
      size = currlength;
    }

    if(DataPtr == NULL_PTR)
    {
      /* copy data from Soad (DataPtr == NULL_PTR) or linear buffer */
      retVal = TcpIp_Tcp_txData_copyDataFromSoAd(SocketId, socketOwnerId, memoryPtr, &size);
    }
    else
    {
      /* otherwise copy data located at DataPtr */
      TS_MemCpy(memoryPtr, &DataPtr[currDataIndex], size);
      retVal = TCPIP_OK;
    }

    /* mark remaining data to be copied to decide if another linear buffer needs
       to be requested */
    currlength -= size;
    currMemIndex += size;
    currDataIndex += size;

  } while((currlength > 0U) && (retVal != TCPIP_E_NOT_OK));

  return length - currlength;
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_txData_copyDataFromSoAd
(
  TcpIp_SocketIdType SocketId,
  uint8 socketOwnerId,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) memoryPtr,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) dataLength
)
{
  TcpIp_ReturnType retVal;
  uint32 currDatLength = *dataLength;

  do
  {
    uint16 Length = (uint16) currDatLength;

    /* check if data to copy is greater than CopyTxData API can handle */
    if(currDatLength > 0xFFFFU)
    {
      Length = 0xFFFFU;
    }

    TCPIP_PRECONDITION_ASSERT
      ((socketOwnerId < TcpIp_NumConfSockOwner),TCPIP_INTERNAL_API_ID);

    /* if buffer to copy from is not specified, copy data from Soad */
    if(TcpIp_CopyTxDataAPI[socketOwnerId](SocketId, memoryPtr, Length) == BUFREQ_OK)
    {
      currDatLength -= Length;
      retVal = TCPIP_OK;
    }
    else
    {
      /* stop copying */
      retVal = TCPIP_E_NOT_OK;
    }

  } while((currDatLength > 0U) && (retVal == TCPIP_OK));

  /* return actual number of copied bytes */
  *dataLength -= currDatLength;

  return retVal;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_txData_buildRst
(
  P2CONST(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameter,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(TcpIp_SockAddrLocalType, AUTOMATIC, TCPIP_APPL_DATA) localAddrPtr,
  uint32 sendNext,
  uint32 receiveNext,
  uint8 flags
)
{
  TcpIp_TcpCtrlBlockType ctrlBlock;
  uint8 rstFlags = flags;

  /* a socket needs the following information to transmit a reset */

  /* sequence number */
  ctrlBlock.send_una = sendNext;
  ctrlBlock.send_nxt = sendNext;
  /* acknowledgement number */
  ctrlBlock.recv_nxt = receiveNext;
  /* window set to 0 */
  ctrlBlock.recv_wnd = 0U;
  /* no options */
  ctrlBlock.flags = TCPIP_TCP_FLAG_CLEAR;
  /* reset flag set */
  rstFlags |= TCPIP_TCP_FLAG_RST;

  (void)
    TcpIp_Tcp_txData_sendSeg
      (&ctrlBlock, ipParameter, localAddrPtr, remoteAddrPtr, 0U, 0U, rstFlags);
}

TS_MOD_PRIV_DEFN FUNC(uint16, TCPIP_CODE) TcpIp_Tcp_performNagleAlgorithm
(
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr,
  uint16 availWindow,
  uint16 dataLen
)
{
  uint16 transmitLength = dataLen;

  if((availWindow >= ctrlBlockPtr->send_mss) &&
     (transmitLength >= ctrlBlockPtr->send_mss)
    )
  {
    /* window and data size is big enough to transmit a full segment */
    transmitLength = ctrlBlockPtr->send_mss;
  }
  else
  {
    if(ctrlBlockPtr->curr_buff != 0U)
    {
      /* transmission already ongoing , buffer all data */
      transmitLength = 0U;
    }
    else
    {
      /* all data acknowledged, transmit requested data */
      if(transmitLength > availWindow)
      {
        /* data to transmit is bigger than send window,
           allocate buffer equal to send window */
        transmitLength = availWindow;
      }
    }
  }

  return transmitLength;
}

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#else
  TS_PREVENTEMPTYTRANSLATIONUNIT
#endif /* (TCPIP_TCP_ENABLED == STD_ON) */

/*==================[end of file]===========================================*/
