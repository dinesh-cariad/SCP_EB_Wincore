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
 * MISRAC2012-1) Deviated Rule: 10.8 (required)
 * The value of a composite expression shall not be cast to a different essential type category
 *  or a wider essential type.
 *
 * Reason:
 * In order to detect a wrap around of sequence and acknowledgement numbers it is required
 * to cast unsigned int to signed int.
 *
 */

#ifndef TCPIP_TCP_PRIV_H
#define TCPIP_TCP_PRIV_H

/*==================[inclusions]============================================*/

#include <SchM_TcpIp.h>         /* SchM API declarations */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Tcp_Int.h>      /* own SW-unit interface file */
#include <TcpIp_IpSecDb_Int.h>  /* external IpSec Api declarations*/
#include <TcpIp_Trace.h>        /* debug and trace macros; must be last */

/*==================[macros]================================================*/

/** \brief maximal size of data that can be transmitted, Note: send window
 *         max 65535 bytes */
#define TCPIP_TCP_MAX_WINDOW_SIZE                             0xFFFFU
/** \brief The value used to increment the initial sequence number
 *  for each new connection */
#define TCPIP_TCP_INITIAL_SEQUENCE_NUMBER_INCREMENT                64000U

/** \brief No connection state at all */
#define TCPIP_TCP_STATE_CLOSED                                     0U
/** \brief Waiting for connection request */
#define TCPIP_TCP_STATE_LISTEN                                     1U
/** \brief Waiting for matching connection request after having sent connection
    request */
#define TCPIP_TCP_STATE_SYN_SENT                                   2U
/** \brief Waiting for connection request acknowledgement after having received
    and sent connection request */
#define TCPIP_TCP_STATE_SYN_RECEIVED                               3U
/** \brief Connection open */
#define TCPIP_TCP_STATE_ESTABLISHED                                4U
/** \brief Waiting for remote termination request or termination request
    acknowledgement */
#define TCPIP_TCP_STATE_FIN_WAIT_1                                 5U
/** \brief Waiting for remote termination request */
#define TCPIP_TCP_STATE_FIN_WAIT_2                                 6U
/** \brief Waiting for termination request from local user */
#define TCPIP_TCP_STATE_CLOSE_WAIT                                 7U
/** \brief Waiting for remote termination acknowledgement */
#define TCPIP_TCP_STATE_CLOSING                                    8U
/** \brief Waiting for termination request acknowledgement */
#define TCPIP_TCP_STATE_LAST_ACK                                   9U
/** \brief Waiting for enough time to pass to be sure remote TCP received ACK of
    its termination request */
#define TCPIP_TCP_STATE_TIME_WAIT                                 10U

/** \brief Clears all control bits */
#define TCPIP_TCP_FLAG_CLEAR                                    0x00U
/** \brief No more data from sender control bit */
#define TCPIP_TCP_FLAG_FIN                                      0x01U
/** \brief Synchronize sequence numbers control bit */
#define TCPIP_TCP_FLAG_SYN                                      0x02U
/** \brief  Reset the connection control bit */
#define TCPIP_TCP_FLAG_RST                                      0x04U
/** \brief  Push Function control bit */
#define TCPIP_TCP_FLAG_PSH                                      0x08U
/** \brief  Acknowledgement field significant control bit */
#define TCPIP_TCP_FLAG_ACK                                      0x10U
/** \brief  Urgent Pointer field significant control bit */
#define TCPIP_TCP_FLAG_URG                                      0x20U

/** \brief Checks if the FIN flag is set */
#define TCPIP_TCP_FLAG_FIN_IS_SET(flags)                          \
  ((((flags) & TCPIP_TCP_FLAG_FIN) == TCPIP_TCP_FLAG_FIN))

/** \brief Checks if the SYN flag is set */
#define TCPIP_TCP_FLAG_SYN_IS_SET(flags)                          \
  ((((flags) & TCPIP_TCP_FLAG_SYN) == TCPIP_TCP_FLAG_SYN))

/** \brief Checks if the RST flag is set */
#define TCPIP_TCP_FLAG_RST_IS_SET(flags)                          \
  ((((flags) & TCPIP_TCP_FLAG_RST) == TCPIP_TCP_FLAG_RST))

/** \brief Checks if the ACK flag is set */
#define TCPIP_TCP_FLAG_ACK_IS_SET(flags)                          \
  ((((flags) & TCPIP_TCP_FLAG_ACK) == TCPIP_TCP_FLAG_ACK))

/** \brief Default data offset in TCP header */
#define TCPIP_TCP_HEADER_SIZE                                     20U
#define TCPIP_TCP_HEADER_SIZE_UINT32                               5U

/** \brief Id of the End of option list */
#define TCPIP_TCP_OPT_KIND_ENDOPT                                  0U
/** \brief Id of the No operation option */
#define TCPIP_TCP_OPT_KIND_NOP                                     1U
/** \brief Id of the MSS option */
#define TCPIP_TCP_OPT_KIND_MSS                                     2U

/** \brief Length of the option header */
#define TCPIP_TCP_OPT_HDR_LEN                                      2U
/** \brief Length of the MSS option */
#define TCPIP_TCP_MSS_OPT_LEN                                      4U
#define TCPIP_TCP_MSS_OPT_LEN_UINT32                               1U

/** \brief Flag to indicate transmission of MSS */
#define TCPIP_TCP_MSS_OPT_FLAG                                  0x01U
/** \brief Flag to indicate pending confirmation */
#define TCPIP_TCP_WAIT_CONF_FLAG                                0x02U
/** \brief Flag to indicate pending close call */
#define TCPIP_TCP_CLOSE_FLAG                                    0x04U
/** \brief Flag to indicate use of nagle algorithm */
#define TCPIP_TCP_NAGLE_FLAG                                    0x08U
/** \brief Flag to indicate active open call */
#define TCPIP_TCP_ACTIVE_FLAG                                   0x10U
/** \brief Flag to indicate pending transmit call */
#define TCPIP_TCP_TRANSMIT_FLAG                                 0x20U
/** \brief Flag to indicate use of keep alive */
#define TCPIP_TCP_KEEP_ALIVE_FLAG                               0x40U
/** \brief Flag to indicate retransmission in next mainfunction */
#define TCPIP_TCP_RETRY_FLAG                                    0x80U

#define TCPIP_OPTION_FILTER_UNSET                                255U

/* \brief Value indicating an invalid hole index which shall not be used as array index */
#define TCPIP_TCP_HOLE_LIST_INDEX_INVALID 0xFFU

/* \brief Value indicating an ringbuffer index which shall not be used as array index */
#define TCPIP_TCP_RINGBUFFER_INFO_INDEX_INVALID 0xFFFFU

/** \brief Check if the MSS option flag is set */
#define TCPIP_TCP_FLAG_MSS_OPT_IS_SET(flags)                      \
  ((((flags) & TCPIP_TCP_MSS_OPT_FLAG) == TCPIP_TCP_MSS_OPT_FLAG))

/** \brief Check if TCP is waiting for a confirmation */
#define TCPIP_TCP_FLAG_WAIT_CONF_IS_SET(flags)                    \
  ((((flags) & TCPIP_TCP_WAIT_CONF_FLAG) == TCPIP_TCP_WAIT_CONF_FLAG))

/** \brief Check if close call is requested */
#define TCPIP_TCP_FLAG_CLOSE_IS_SET(flags)                        \
  ((((flags) & TCPIP_TCP_CLOSE_FLAG) == TCPIP_TCP_CLOSE_FLAG))

/** \brief Check if nagle is activated */
#define TCPIP_TCP_FLAG_NAGLE_IS_SET(flags)                        \
  ((((flags) & TCPIP_TCP_NAGLE_FLAG) == TCPIP_TCP_NAGLE_FLAG))

/** \brief Check if keep alive is activated */
#define TCPIP_TCP_FLAG_KEEP_ALIVE_IS_SET(flags)                   \
  ((((flags) & TCPIP_TCP_KEEP_ALIVE_FLAG) == TCPIP_TCP_KEEP_ALIVE_FLAG))

/** \brief Check for active open call */
#define TCPIP_TCP_FLAG_ACTIVE_IS_SET(flags)                       \
  ((((flags) & TCPIP_TCP_ACTIVE_FLAG) == TCPIP_TCP_ACTIVE_FLAG))

/** \brief Check if data shall be transmitted */
#define TCPIP_TCP_FLAG_TRANSMIT_IS_SET(flags)                     \
  ((((flags) & TCPIP_TCP_TRANSMIT_FLAG) == TCPIP_TCP_TRANSMIT_FLAG))

/** \brief Check if mss shall be transmitted */
#define TCPIP_TCP_FLAG_MSS_IS_SET(flags)                     \
  ((((flags) & TCPIP_TCP_MSS_OPT_FLAG) == TCPIP_TCP_MSS_OPT_FLAG))

/** \brief Check if segment shall be retransmitted */
#define TCPIP_TCP_FLAG_RETRY_IS_SET(flags)                     \
  ((((flags) & TCPIP_TCP_RETRY_FLAG) == TCPIP_TCP_RETRY_FLAG))

/** \brief Value to indicate that acknowledgement timer is not running */
#define TCPIP_TCP_TIMER_UINT16_NOT_RUN                        0xFFFFU

/** \brief Value to indicate that uint32 timer is not running */
#define TCPIP_TCP_TIMER_UINT32_NOT_RUN                    0xFFFFFFFFU

/** \brief Starts the retransmission timer */
#define TcpIp_Tcp_startRtxTimer(tcb)                              \
  do                                                              \
  {                                                               \
    (tcb)->rtx_timer = 0U;                                        \
    (tcb)->num_rtx = 0U;                                          \
  }while(0)

/** \brief  Checks if the retransmission timer is running */
#define TcpIp_Tcp_rtxTimerRunning(tcb)                            \
  ((tcb)->rtx_timer != TCPIP_TCP_TIMER_UINT32_NOT_RUN)

/** \brief Stops the retransmission timer */
#define TcpIp_Tcp_stopRtxTimer(tcb)                               \
  do                                                              \
  {                                                               \
    (tcb)->rtx_timer = TCPIP_TCP_TIMER_UINT32_NOT_RUN;            \
    (tcb)->num_rtx = 0U;                                          \
  }while(0)

/** \brief Retry transmission in next mainfunction */
#define TcpIp_Tcp_startRetry(tcb)                                 \
   (tcb)->flags |= TCPIP_TCP_RETRY_FLAG

/** \brief Stops transmission in next mainfunction */
#define TcpIp_Tcp_stopRetry(tcb)                                  \
  do                                                              \
  {                                                               \
    (tcb)->flags &= ~((uint8) TCPIP_TCP_RETRY_FLAG);              \
    (tcb)->num_retry = 0U;                                        \
  }while(0)

/** \brief Starts the transmission delay of an acknowledgement */
#define TcpIp_Tcp_startAckTimer(tcb)                              \
  (tcb)->ack_timer = 0U

/** \brief Starts the modified transmission delay of an acknowledgement */
#define TcpIp_Tcp_startModAckTimer(tcb, ticks)                    \
  TS_AtomicAssign16                                               \
  (                                                               \
    (tcb)->ack_timer,                                             \
    (TcpIp_PBcfgPtr->tcpConfig.delayedAckTimeout - ticks)         \
  )

/** \brief Stops the transmission delay of an acknowledgement */
#define TcpIp_Tcp_stopAckTimer(tcb)                               \
  (tcb)->ack_timer = TCPIP_TCP_TIMER_UINT16_NOT_RUN

/** \brief Checks if the ack timer is running */
#define TcpIp_Tcp_ackTimerRunning(tcb)                            \
  ((tcb)->ack_timer != TCPIP_TCP_TIMER_UINT16_NOT_RUN)

/** \brief Try retransmission of ACK in next mainfunction */
#define TcpIp_Tcp_startAckRetranTimer(tcb)                        \
  do                                                              \
  {                                                               \
    (tcb)->flags |= TCPIP_TCP_WAIT_CONF_FLAG;                     \
    TcpIp_Tcp_startAckTimer((tcb));                               \
  }while(0)

/** \brief Stop retransmission of ACK in next mainfunction */
#define TcpIp_Tcp_stopAckRetranTimer(tcb)                         \
  do                                                              \
  {                                                               \
    (tcb)->flags &= ~((uint8) (TCPIP_TCP_WAIT_CONF_FLAG));        \
    TcpIp_Tcp_stopAckTimer((tcb));                                \
  }while(0)

/** \brief Starts the 2 MSL count down */
#define TcpIp_Tcp_start2MslTimer(tcb)                             \
  (tcb)->state_timer = 2U * TcpIp_PBcfgPtr->tcpConfig.msl

/** \brief Starts the FIN_WAIT_2 count down */
#define TcpIp_Tcp_startFinWait2Timer(tcb)                         \
  (tcb)->state_timer = TcpIp_PBcfgPtr->tcpConfig.finWait2Timeout

/** \brief Starts Keep Alive transmission count down */
#define TcpIp_Tcp_startKeepAliveTimer(tcb)                        \
  (tcb)->state_timer = (tcb)->keepAlive_interval

/** \brief Starts Keep Alive transmission count down */
#define TcpIp_Tcp_startFirstKeepAliveTimer(tcb)                   \
  do                                                              \
  {                                                               \
    (tcb)->state_timer = (tcb)->keepAlive_time;                   \
    (tcb)->num_rtx = 0U;                                          \
  }while(0)

/** \brief Starts Confirming connection request acknowledgment count down */
#define TcpIp_Tcp_startSynReceivedTimer(tcb)                        \
  (tcb)->state_timer = TcpIp_PBcfgPtr->tcpConfig.synReceivedTimeout

/** \brief Stops a state count down */
#define TcpIp_Tcp_stopStateTimer(tcb)                             \
  (tcb)->state_timer = 0U

/** \brief Checks if the state timer is running */
#define TcpIp_Tcp_stateTimerRunning(tcb)                          \
  ((tcb)->state_timer != 0U)

/** \brief Starts timer for Rate-limiting the duplicate Acknowledgement
 * for out-of-window sequence number */
#define TcpIp_Tcp_startDuplicateAcknowledgementTimerSeqNum(tcb)         \
  (tcb)->dupackSeqNum_timer = TcpIp_PBcfgPtr->tcpConfig.dupAckTxTimeoutSeqNum;

/** \brief Stops timer for Rate-limiting the duplicate Acknowledgement
 * for out-of-window sequence number */
#define TcpIp_Tcp_stopDuplicateAcknowledgementTimerSeqNum(tcb)          \
  (tcb)->dupackSeqNum_timer = 0U

/** \brief Checks if the timer for Rate-limiting the duplicate Acknowledgement
 * for out-of-window sequence number is running */
#define TcpIp_Tcp_duplicateAcknowledgementTimerRunningSeqNum(tcb)       \
  ((tcb)->dupackSeqNum_timer != 0U)

/** \brief Starts timer for Rate-limiting the duplicate Acknowledgement
 * for out-of-window acknowledgement number */
#define TcpIp_Tcp_startDuplicateAcknowledgementTimerAckNum(tcb)         \
  (tcb)->dupackAckNum_timer = TcpIp_PBcfgPtr->tcpConfig.dupAckTxTimeoutSeqNum;

/** \brief Stops timer for Rate-limiting the duplicate Acknowledgement
 * for out-of-window acknowledgement number */
#define TcpIp_Tcp_stopDuplicateAcknowledgementTimerAckNum(tcb)          \
  (tcb)->dupackAckNum_timer = 0U

/** \brief Checks if the timer for Rate-limiting the duplicate Acknowledgement
 * for out-of-window acknowledgement number is running */
#define TcpIp_Tcp_duplicateAcknowledgementTimerRunningAckNum(tcb)       \
  ((tcb)->dupackAckNum_timer != 0U)

/* macros for comparing modulo-32-numbers, i.e. sequence numbers */
/* a > b */
/* Deviation MISRAC2012-1 */
#define TCPIP_TCP_IS_GREATER_THAN(a, b)     ((sint32)((a) - (b)) > 0)
/* a < b */
/* Deviation MISRAC2012-1 */
#define TCPIP_TCP_IS_LOWER_THAN(a, b)       ((sint32)((b) - (a)) > 0)
/* a >= b */
#define TCPIP_TCP_IS_GREATER_OR_EQUAL(a, b) ((sint32)((a) - (b)) >= 0)
/* a <= b */
/* Deviation MISRAC2012-1 */
#define TCPIP_TCP_IS_LOWER_OR_EQUAL(a, b)   ((sint32)((b) - (a)) >= 0)

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

#if (TCPIP_TCP_ENABLED == STD_ON)

/** \brief Retrieve segment length
 *
 * This function evaluates the length of segments containing a SYN and FIN flag.
 *
 * \param[in]  controlBits    Control bits of received segment
 *
 * \return segment length
 */
TS_MOD_PRIV_DECL FUNC(uint8, TCPIP_CODE) TcpIp_Tcp_evaluateSegLen
(
  uint8 controlBits
);

/** \brief Free data from socket memory block
 *
 * This function frees the amount of data specified by dataToFree from the allocated
 * socket memory block, if all data has been acknowledged the whole memory block is freed.
 *
 * \param[in]  CtrlBlock  Pointer to the transmission control block
 * \param[in]  dataToFree    indicates how much data shall be removed from socket memory
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Tcp_freeSocketMemoryData
(
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr,
  uint16 dataToFree
);

/** \brief Active open
 *
 * This function starts to establish a connection to a remote host by transmitting
 * a SYN segment.
 *
 * \param[in]  SocketId       Id of the socket connection
 * \param[in]  RemoteAddrPtr  IP address and port of remote host
 *
 * \retval TCPIP_OK               SYN successfully transmitted
 * \retval TCPIP_E_NOT_OK         Transmission error occurred
 * \retval TCPIP_E_DROP           Out of memory
 * \retval TCPIP_E_NOT_PERMITTED  Request not permitted in this socket state
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_connect
(
  TcpIp_SocketIdType SocketId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RemoteAddrPtr
);

/** \brief Passive Open
 *
 * This function appends a socket to the listen socket list and sets the socket state
 * to listen.
 *
 * \param[in]  SocketId     Id of the socket connection
 * \param[in]  MaxChannels  Maximum number of new parallel connections established on
 *                          this listen connection.
 *
 * \retval TCPIP_OK               Socket set to LISTEN state
 * \retval TCPIP_E_NOT_PERMITTED  Request not permitted in this socket state
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_listen
(
  TcpIp_SocketIdType SocketId,
  uint16 MaxChannels
);

/** \brief Buffer data for retransmission
 *
 * This function buffers data into the dynamic memory and marks the data for transmission
 * in the next mainfunction.
 *
 * \param[in]  SocketId         Id of the socket connection
 * \param[in]  DataPtr          Pointer to the data that shall be transmitted
 * \param[in]  AvailableLength  Available data for transmission in bytes.
 * \param[in]  ForceRetrieve    Indicates how the TCP/IP stack retrieves data from SoAd
 *                              if DataPtr is a NULL_PTR.
 *
 * \retval TCPIP_OK               Data successfully transmitted
 * \retval TCPIP_E_NOT_OK         Transmission error occurred
 * \retval TCPIP_E_DROP           Out of memory
 * \retval TCPIP_E_NOT_PERMITTED  Connection not established
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_bufferData
(
  TcpIp_SocketIdType socketId,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint32 AvailableLength,
  boolean ForceRetrieve
);

/** \brief Mark close of socket connection
 *
 * This function sets a flag to mark the close of a socket connection in the next
 * mainfunction.
 *
 * \param[in]  socketPtr  Pointer to Tcp socket structure
 *
 * \retval TCPIP_OK               Request accepted
 * \retval TCPIP_E_NOT_PERMITTED  Request not permitted in this socket state
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_requestClose
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Regular close of a TCP connection
 *
 * This function closes a TCP connection by transmitting a FIN.
 *
 * \param[in]  socketId   Socket identifier
 * \param[in]  socketPtr  Pointer to Tcp socket structure
 *
 * \retval TCPIP_OK               FIN successfully transmitted
 * \retval TCPIP_E_NOT_OK         Transmission error occurred
 * \retval TCPIP_E_DROP           Out of memory, stop further processing
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_regularClose
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Update receive window
 *
 * This function updates the receive window of the socket.
 *
 * \param[in]  socketId  Socket identifier
 * \param[in]  Length    Number of bytes finally consumed by the upper layer
 *
 * \retval TCPIP_OK               Update successful
 * \retval TCPIP_E_NOT_PERMITTED  Request not permitted in this socket state
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_received
(
  TcpIp_SocketIdType SocketId,
  uint16 Length
);

/** \brief Handle SYN segment
 *
 * This function handles the reception of a SYN segment. E.g. Response with SYN,ACK.
 *
 * \param[in]  socketId       Socket identifier
 * \param[in]  socketPtr      Pointer to connection information
 * \param[in]  remoteAddrPtr  IP address and port of the remote host
 * \param[in]  localAddrPtr   IP address and port of the local host
 * \param[in]  seqNum         Sequence Number of received segment
 * \param[in]  seglen         segment length
 * \param[in]  mss            Maximum segment size of segment
 * \param[in]  flags          control flags set in the received segment
 *
 * \retval TCPIP_OK        SYN,ACK successfully transmitted
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Out of memory, stop further processing
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_rcvSyn
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(TcpIp_SockAddrLocalType, AUTOMATIC, TCPIP_APPL_DATA) localAddrPtr,
  uint32 seqNum,
  uint16 seglen,
  uint16 mss,
  uint8 flags
);

/** \brief Handle SYN segment
 *
 * This function handles the reception of a SYN segment in illegal states and transmits
 * a reset if unexpected SYN is received.
 *
 * \param[in]  socketId       Socket identifier
 * \param[in]  socketPtr      Pointer to connection information
 * \param[in]  seqNum         Sequence Number of received segment
 * \param[in]  ackNum         Acknowledgement Number of received segment
 * \param[in]  seglen         Length of the segment
 * \param[in]  flags          control flags set in the received segment
 *
 * \retval TCPIP_OK        No error occurred
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Out of memory, stop further processing
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_rcvSynCheckErr
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  uint32 seqNum,
  uint32 ackNum,
  uint16 seglen,
  uint8 flags
);

/** \brief Handle FIN segment
 *
 * This function handles the reception of a FIN segment in illegal states and drops
 * the segment if unexpected FIN is received.
 *
 * \param[in]  socketPtr      Pointer to connection information
 *
 * \retval TCPIP_OK        No error occurred
 * \retval TCPIP_E_NOT_OK  Unreachable state
 * \retval TCPIP_E_DROP    Do not process segment any further
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_rcvFinCheckErr
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Handle FIN segment
 *
 * This function handles the reception of a FIN segment. E.g. Response with ACK
 * or reset time wait timeout
 *
 * \param[in]  socketId       Socket identifier
 * \param[in]  socketPtr      Pointer to connection information
 * \param[in]  seqNum         Sequence Number of the FIN
 *
 * \retval TCPIP_OK        ACK successfully transmitted
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Out of memory, stop further processing
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_rcvFin
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  uint32 seqNum
);

/** \brief Handle RST segment
 *
 * This function handles the reception of a RST segment. E.g. free socket
 *
 * \param[in]  socketId       Socket identifier
 * \param[in]  socketPtr      Pointer to connection information
 * \param[in]  flags          Control flags set in the received segment
 *
 * \retval TCPIP_OK        connection successfully reseted
 * \retval TCPIP_E_DROP    stop further processing
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_rcvRst
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  uint8 flags
);

/** \brief Handle ACK segment
 *
 * This function handles the reception of a ACK segment in illegal states and drops
 * the segment if unexpected ACK is received.
 *
 * \param[in]  socketPtr      Pointer to connection information
 *
 * \retval TCPIP_OK        No error occurred
 * \retval TCPIP_E_NOT_OK  Unreachable state
 * \retval TCPIP_E_DROP    Do not process segment any further
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_rcvAckCheckErr
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Handle ACK segment
 *
 * This function handles the reception of a ACK segment. E.g. Set socket state
 *
 * \param[in,out]  socketIdPtr    in: Listen Socket identifier
 *                                out: Listen Socket identifier or
 *                                     resued Socket identifier (when ACK with valid SYN
 *                                     cookie was received in state LISTEN)
 * \param[in,out]  socketPtrPtr   in: Pointer to Listen socket connection information
 *                                out: Pointer to Listen socket connection information or
 *                                     resued Socket connection information (when ACK with
 *                                     valid SYN cookie was received in state LISTEN)
 * \param[in]      remoteAddrPtr  IP address and port of the remote host
 * \param[in]      localAddrPtr   IP address and port of the local host
 * \param[in]      seqNum         Sequence Number of received segment
 * \param[in]      ackNum         Acknowledgement Number of received segment
 * \param[in]      seglen         segment length
 * \param[in]      flags          control flags set in the received segment

 *
 * \retval TCPIP_OK        continue processing
 * \retval TCPIP_E_DROP    stop further processing
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_rcvAck
(
  P2VAR(TcpIp_SocketIdType, AUTOMATIC, TCPIP_APPL_DATA) socketIdPtr,
  P2VAR(TcpIp_Socket_TCP_Generic_PtrType, AUTOMATIC, TCPIP_APPL_DATA) socketPtrPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(TcpIp_SockAddrLocalType, AUTOMATIC, TCPIP_APPL_DATA) localAddrPtr,
  uint32 seqNum,
  uint32 ackNum,
  uint16 seglen,
  uint8 flags
);

/** \brief Check sequence number
 *
 * This function verifies if the sequence number of the received segment lies inside the
 * receive window. If sequence number is outside the window, TCP transmits an ACK.
 *
 * \param[in]  socketPtr  Pointer to connection information
 * \param[in]  seqNum     Sequence Number of received segment
 * \param[in]  seglen     Segment length
 * \param[in]  flags      Control flags set in the received segment
 *
 * \retval TCPIP_OK        Sequence number acceptable
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Out of memory, stop further processing
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_checkSeqNum
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  uint32 seqNum,
  uint16 seglen,
  uint8 flags
);

/** \brief Check acknowledgement number
 *
 * This function verifies if the acknowledgement number of the received segment
 * acknowledge previously transmitted data. If acknowledgement number acknowledge
 * something not yet sent, TCP transmits an ACK.
 *
 * \param[in]  socketPtr      Pointer to connection information
 * \param[in]  remoteAddrPtr  IP Address and port of the remote host
 * \param[in]  localAddr      IP address and port on which the segment was received on
 * \param[in]  ackNumber      Acknowledgement Number of received segment
 * \param[in]  seqNumber      Sequence Number of received segment
 * \param[in]  seglen         Segment length
 * \param[in]  flags          Control flags set in the received segment
 *
 * \retval TCPIP_OK        Acknowledgement number acceptable
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Out of memory, stop further processing
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_checkAckNum
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  TcpIp_SockAddrLocalType localAddr,
  uint32 ackNumber,
  uint32 seqNumber,
  uint16 seglen,
  uint8 flags
);

/** \brief Handle data segment
 *
 * This function handles the payload of a segment. It pass the data to to Soad, triggers
 * the transmission of a delayed acknowledgement and transmits an acknowledgement if an
 * out of order segment is detected.
 *
 * \param[in]  socketId   Socket identifier
 * \param[in]  socketPtr  Pointer to connection information
 * \param[in]  dataPtr    Points to the payload
 * \param[in]  dataLen    data length
 * \param[in]  seqNum     Sequence Number of received segment
 *
 * \retval TCPIP_OK        ACK successfully transmitted
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    stop further processing
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_rcvData
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 dataLen,
  uint32 seqNum
);

/** \brief Handle a state timeout
 *
 * This function frees a socket in case a state timeout (e.g. 2* MSL) is reached.
 *
 * \param[in]  socketId   Socket identifier
 * \param[in]  socketPtr  Pointer to the socket structure
 *
 * \retval TCPIP_OK        retransmission successful
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Out of memory, stop further processing
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_stateTimeout
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Handle a retransmission timeout
 *
 * This function retransmits sent data and control messages. If maximal number of
 * retransmission is reached, socket is freed.
 *
 * \param[in]  socketId  Socket identifier
 * \param[in]  socketPtr Pointer to socket connection information
 *
 * \retval TCPIP_OK        retransmission successful
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Out of memory, stop further processing
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_rtxTimeout
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Buffer data
 *
 * This function evaluates the number of bytes that shall be buffered in the dynamic
 * memory and copies them.
 *
 * \param[in]  socketId         Socket identifier
 * \param[in]  socketPtr        Pointer to socket connection information
 * \param[in]  DataPtr          Pointer to the data that shall be transmitted
 * \param[in]  AvailableLength  Available data for transmission in bytes.
 * \param[in]  ForceRetrieve    Indicates how the TCP/IP stack retrieves data from SoAd
 *                              if DataPtr is a NULL_PTR.
 *
 * \retval TCPIP_OK        buffering successful
 * \retval TCPIP_E_NOT_OK  SoAd_CopyTxData returned error
 * \retval TCPIP_E_DROP    no data copied
 *         TCPIP_E_MEMORY  out of memory
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_txData_buff
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint32 AvailableLength,
  boolean ForceRetrieve
);

/** \brief Transmit data
 *
 * This function segments the data into packets and transmits them to the remote node.
 *
 * \param[in]  socketPtr   Pointer to socket connection information
 *
 * \retval TCPIP_OK        transmission successful
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, copy error
 * \retval TCPIP_E_DROP    no data copied
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_txData_transmit
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);


#if (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON)

/** \brief Transmit SYN segment
 *
 * This function transmits a SYN/ACK segment. Used for SYN cookies when there is
 * no free socket left.
 *
 * \param[in]  ipParameter    Points to a structure containing Ip communication parameters
 * \param[in]  remoteAddrPtr  IP Address and port of the remote host
 * \param[in]  localAddrPtr   IP Address and port of the local host
 * \param[in]  sendNext       next sequence number to send
 * \param[in]  receiveNext    next sequence number to receive
 * \param[in]  window         window to send
 * \param[in]  mss            maximum segment size to send
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Tcp_txData_sendTempSyn
(
  P2CONST(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameter,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(TcpIp_SockAddrLocalType, AUTOMATIC, TCPIP_APPL_DATA) localAddrPtr,
  uint32 sendNext,
  uint32 receiveNext,
  uint16 window,
  uint16 mss
);

#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON) */

/** \brief Transmit SYN segment
 *
 * This function transmits a SYN segment.
 *
 * \param[in]  socketPtr  Pointer to socket connection information
 * \param[in]  ack        TRUE:  transmit SYN,ACK
 *                        FALSE: transmit SYN
 *
 * \retval TCPIP_OK        transmission successful
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Out of memory, stop further processing
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_txData_sendSyn
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  boolean ack
);

/** \brief Transmit FIN segment
 *
 * This function transmits a FIN segment.
 *
 * \param[in]  socketPtr  Pointer to socket connection information
 *
 * \retval TCPIP_OK        transmission successful
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Out of memory, stop further processing
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_txData_sendFin
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Transmit RST segment
 *
 * This function transmits a RST segment.
 *
 * \param[in]  ipParameter    Points to a structure containing Ip communication parameters
 * \param[in]  remoteAddrPtr  IP Address and port of the remote host
 * \param[in]  localAddrPtr   IP Address and port of the local host
 * \param[in]  ackNum         acknowledgement number of the received segment
 * \param[in]  seqNum         sequence number of the received segment
 * \param[in]  seglen         segment length
 * \param[in]  flags          control flags
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Tcp_txData_sendRst
(
  P2CONST(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameter,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(TcpIp_SockAddrLocalType, AUTOMATIC, TCPIP_APPL_DATA) localAddrPtr,
  uint32 ackNum,
  uint32 seqNum,
  uint16 seglen,
  uint8 flags
);

#if (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON)

/** \brief Transmit keep alive segment
 *
 * This function transmits a Keep alive segment.
 *
 * \param[in]  socketPtr  Pointer to socket connection information
 *
 * \retval TCPIP_OK        transmission successful
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Out of memory, stop further processing
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_txData_sendKeepAlive
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

#endif /* (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON) */

/** \brief Transmit RST segment in response to ACK
 *
 * This function builds and transmits a RST segment in response to a received ACK
 * segment.
 *
 * \param[in]  ipParameter    Points to a structure containing Ip communication parameters
 * \param[in]  remoteAddrPtr  IP Address and port of the remote host
 * \param[in]  localAddrPtr   IP Address and port of the local host
 * \param[in]  ackNum         acknowledgement number of the received segment
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Tcp_txData_sendRstAck
(
  P2CONST(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameter,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(TcpIp_SockAddrLocalType, AUTOMATIC, TCPIP_APPL_DATA) localAddrPtr,
  uint32 ackNum
);

/** \brief Transmit ACK segment
 *
 * This function transmits an ACK segment.
 *
 * \param[in]  socketPtr  Pointer to socket connection information
 *
 * \retval TCPIP_OK        transmission successful
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Out of memory, stop further processing
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_txData_sendAck
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Transmit unsent data
 *
 * This function retries to transmit unsent data and control messages.
 *
 * \param[in]  socketPtr  Pointer to socket connection information
 *
 * \retval TCPIP_OK        transmission successful
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Out of memory, stop further processing
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_txData_retransmitUnsent
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Retransmit sent data
 *
 * This function retransmit sent data and control messages.
 *
 * \param[in]  socketPtr  Pointer to socket connection information
 *
 * \retval TCPIP_OK        transmission successful
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Out of memory, stop further processing
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_txData_retransmit
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Close socket
 *
 * This function closes the TCP socket if error occurs during TCP transmission.
 *
 * \param[in]  socketId   id of the socket connection
 * \param[in]  socketPtr  Pointer to the connection information
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Tcp_closeProcessedTcpSocket
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Retrieve security information and payload lengths
 *
 * This function consults the policy table and returns maximal payload lengths and
 * security information
 *
 * \param[in]  localAddrId     local address over which data is sent
 * \param[in]  remoteAddrPtr   remote address and port to which data is sent
 * \param[in]  nextHeader      used protocol (UDP; TCP, ICMP)
 * \param[in]  localPort       local port over which data is sent
 * \param[out] ipParameterPtr  returns ip payload, additional headers (AH) and
 *                             security association index
 *
 * \retval E_OK      Transmission parameter like maximal ip payload, additional
 *                   headers (AH) and security association index could be retrieved
 *                   successfully
 * \retval E_NOT_OK  Security association is not established or not matching policy found
 *                   (only when IPSec is enabled)
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_getTransmissionParam
(
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  uint8 nextHeader,
  uint16 localPort,
  P2VAR(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameterPtr
);

#if (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON)
/** \brief Mark SYN cookie keys for regeneration
 *
 * This function sets the reset timer for two SYN cookie keys
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Tcp_SYN_Cookies_setKeyRegenerateTimer(void);
#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON) */


#if(TCPIP_TCP_OUTOFORDER_RECEPTION_ENABLED == STD_ON)

/** \brief Initialize out-of-order data structures
 *
 * Function to initialize data structures used to support out-of-order reception of TCP segments.
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Tcp_rxOutOfOrder_Init
(
  void
);

/** \brief Process the reception of an out-of-order-segment
 *
 * This function solves 2 tasks:
 * 1. It buffers parts of a received out-of-order segments that are not yet buffered and fit into
 *    the buffer.
 * 2. It passes data to the upper layer which is in order again.
 * The received segment has a length of dataLen bytes. Its first byte corresponds to the TCP
 * sequence number sequenceNum and is located at dataPtr. The segment belongs to the control block
 * pointed to by ctrlBlockPtr and to the socket identified by socketId and socketPtr.
 *
 * \param[in,out] ctrlBlockPtr Pointer to the transmission control block this segment belongs to
 * \param[in] dataPtr Memory location of the first data byte of the segment
 * \param[in] dataLen Length in bytes of the segment
 * \param[in] sequenceNum TCP sequence number correpsonding to the first byte of the segment
 * \param[in] socketId Identifying the local socket resource
 * \param[in] socketPtr Pointer to the TCP socket structure
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Tcp_rxOutOfOrder_Receive
(
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 dataLen,
  uint32 sequenceNum,
  TcpIp_SocketIdType socketId,
  P2CONST(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Reset the buffer and hole list of a single socket
 *
 * Function to reset the data structures used by out-of-order reception, i.e. buffer and hole list
 * data storage and management structures.
 *
 * \param[in] bufferIndex Index of the buffer to be reset
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Tcp_rxOutOfOrder_reset
(
  uint16 bufferIndex
);

#endif /* (TCPIP_TCP_OUTOFORDER_RECEPTION_ENABLED == STD_ON) */

#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)
/** \brief Increment invalid socket counter
 *
 * Function increments invalid socket counter.
 *
 * \param[in] bufferIndex Index of the buffer to be reset
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Tcp_IncrementInvalidSocketCount(void);
#endif /* (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON) */

#if (TCPIP_TCP_UNPREDICTABLE_INIT_SEQ_NUM_ENABLED == STD_ON)
/** \brief Trigger unpredictable sequence number timer
 *
 * If unpredictable sequence number timer is:
 * - stopped, then start the timer
 * - running, do nothing
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Tcp_unprSeqNumSetKeyRegenerateTimer(void);
#endif /* (TCPIP_TCP_UNPREDICTABLE_INIT_SEQ_NUM_ENABLED == STD_ON) */

/** \brief Get initial sequence number
 *
 * This function calculates the initial sequence number of a TCP connection and returns
 * it to the caller.
 *
 * \return Initial sequence number
 */
TS_MOD_PRIV_DECL FUNC(uint32, TCPIP_CODE) TcpIp_Tcp_getInitialSeqNum(void);

#endif /* (TCPIP_TCP_ENABLED == STD_ON) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /* TCPIP_TCP_PRIV_H */
