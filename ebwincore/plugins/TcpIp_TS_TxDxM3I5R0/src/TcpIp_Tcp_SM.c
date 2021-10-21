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
 * Socket is a generic type which dependent on the actual instance
 * Thus for usage of this concrete type, a cast is necessary.
 *
 * MISRAC2012-2) Deviated Rule: 5.5 (required)
 * Identifiers shall be distinct from macro names.
 *
 * Reason:
 * For easier associating design transitions and transitions in the source code.
 *
 * MISRAC2012-3) Deviated Rule: 10.8 (required)
 * The value of a composite expression shall not be cast to a different essential type category
 * or a wider essential type.
 *
 * Reason:
 * In order to detect a wrap around of sequence and acknowledgement numbers it is required
 * to cast unsigned int to signed int.
 */

/*==================[inclusions]============================================*/

#include <TcpIp_Socket_Api.h>   /* Type definitions of Socket SW-unit*/

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Socket_Int.h>   /* used SW-unit interface file */
#include <TcpIp_Memory_Int.h>   /* used SW-unit interface file */
#include <TcpIp_Tcp_Priv.h>     /* own SW-unit interface file */

#if (TCPIP_TCP_CRYPTO_ENABLED)
#include <TcpIp_Crypto.h>   /* used SW-unit interface file */
#include <TSMem.h>                  /* TS_MemCpy, TS_MemCmp */
#if (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON)
#include <StbM.h>
#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON) */
#endif /* (TCPIP_TCP_CRYPTO_ENABLED) */

/*==================[macros]================================================*/
#if (TCPIP_TCP_UNPREDICTABLE_INIT_SEQ_NUM_ENABLED == STD_ON)
/** \brief the size of sequence number value, meaning 32 bit value */
#define TCPIP_TCP_UNPR_SEQ_NUM_RESULT_SIZE           4U

/** \brief size of Ip addresses, domain octet, address id octet and 2 ports */
#define TCPIP_TCP_UNPR_SEQ_NUM_MAX_SIZE    TCPIP_TCP_INTERNAL_HASH_ADDR_PORT_SIZE

#endif /* (TCPIP_TCP_UNPREDICTABLE_INIT_SEQ_NUM_ENABLED == STD_ON) */

/** \brief Active open
 *
 * In state LISTEN. This function fills in the connection information and starts to
 * establish a connection to a remote host by transmitting a SYN segment. It sets the
 * socket state to SYN_SENT_WAIT.
 *
 * \param[in]  SocketId       id of the socket connection
 * \param[in]  socketPtr      Pointer to the socket connection information
 * \param[in]  RemoteAddrPtr  IP Address and port of the remote host
 *
 * \retval TCPIP_OK        SYN successfully transmitted
 * \retval TCPIP_E_NOT_OK  Error during transmission occurred, connection already exists
 */
#define TcpIp_Tcp_SM_tr_connect_LISTEN_SYN_SENT(SocketId, socketPtr, RemoteAddrPtr) \
  TcpIp_Tcp_SM_tr_connect_CLOSED_SYN_SENT((SocketId), (socketPtr), (RemoteAddrPtr))


/** \brief Immediate close of a TCP connection
 *
 * In State SYN_SENT. This function informs the Soad about the successful close.
 * It sets the socket state to CLOSED.
 *
 * \param[in]  Id    Socket identifier
 * \param[in]  sock  Pointer to the socket connection information
 */
#define TcpIp_Tcp_SM_tr_immediateClose_SYN_SENT_CLOSED(Id, sock)              \
  TcpIp_Tcp_SM_tr_immediateClose_LISTEN_CLOSED((Id), (sock))

/** \brief Immediate close of a TCP connection
 *
 * In State CLOSE_WAIT. This function transmits a RST, frees all allocated buffers and
 * informs the Soad about the successful close. It sets the socket state to CLOSED.
 *
 * \param[in]  socketId   Socket identifier
 * \param[in]  socketPtr  Pointer to the socket connection information
 */
#define TcpIp_Tcp_SM_tr_immediateClose_CLOSE_WAIT_CLOSED(SocketId, socketPtr)     \
  TcpIp_Tcp_SM_tr_immediateClose_SYN_RECEIVED_CLOSED((SocketId), (socketPtr))

/** \brief Immediate close of a TCP connection
 *
 * In State CLOSING. This function informs the Soad about the successful close.
 * It sets the socket state to CLOSED.
 *
 * \param[in]  Id    Socket identifier
 * \param[in]  sock  Pointer to the socket connection information
 */
#define TcpIp_Tcp_SM_tr_immediateClose_CLOSING_CLOSED(Id, sock)                    \
  TcpIp_Tcp_SM_tr_immediateClose_LISTEN_CLOSED((Id), (sock))

/** \brief Immediate close of a TCP connection
 *
 * In State LAST_ACK. This function informs the Soad about the successful close.
 * It sets the socket state to CLOSED.
 *
 * \param[in]  Id   Socket identifier
 * \param[in]  sock  Pointer to the socket connection information
 */
#define TcpIp_Tcp_SM_tr_immediateClose_LAST_ACK_CLOSED(Id, sock)                   \
  TcpIp_Tcp_SM_tr_immediateClose_LISTEN_CLOSED((Id), (sock))

/** \brief Immediate close of a TCP connection
 *
 * In State TIME_WAIT. This function informs the Soad about the successful close.
 * It sets the socket state to CLOSED.
 *
 * \param[in]  Id    Socket identifier
 * \param[in]  sock  Pointer to the socket connection information
 */
#define TcpIp_Tcp_SM_tr_immediateClose_TIME_WAIT_CLOSED(Id, sock)                  \
  TcpIp_Tcp_SM_tr_immediateClose_LISTEN_CLOSED((Id), (sock))

/** \brief Immediate close of a TCP connection
 *
 * In State ESTABLISHED. This function transmits a RST, frees all allocated buffers and
 * informs the Soad about the successful close. It sets the socket state to CLOSED.
 *
 * \param[in]  socketId   Socket identifier
 * \param[in]  socketPtr  Pointer to the socket connection information
 */
#define TcpIp_Tcp_SM_tr_immediateClose_ESTABLISHED_CLOSED(SocketId, socketPtr)     \
  TcpIp_Tcp_SM_tr_immediateClose_SYN_RECEIVED_CLOSED((SocketId), (socketPtr))

/** \brief Immediate close of a TCP connection
 *
 * In State FIN_WAIT_1. This function transmits a RST and informs the Soad about the
 * successful close. It sets the socket state to CLOSED.
 *
 * \param[in]  socketId   Socket identifier
 * \param[in]  socketPtr  Pointer to the socket connection information
 */
#define TcpIp_Tcp_SM_tr_immediateClose_FIN_WAIT_1_CLOSED(SocketId, socketPtr)      \
  TcpIp_Tcp_SM_tr_immediateClose_SYN_RECEIVED_CLOSED((SocketId), (socketPtr))

/** \brief Immediate close of a TCP connection
 *
 * In State FIN_WAIT_2. This function transmits a RST and informs the Soad about the
 * successful close. It sets the socket state to CLOSED.
 *
 * \param[in]  socketId   Socket identifier
 * \param[in]  socketPtr  Pointer to the socket connection information
 */
#define TcpIp_Tcp_SM_tr_immediateClose_FIN_WAIT_2_CLOSED(SocketId, socketPtr)      \
  TcpIp_Tcp_SM_tr_immediateClose_SYN_RECEIVED_CLOSED((SocketId), (socketPtr))

/** \brief Immediate close of a TCP connection
 *
 * In State TIME_WAIT_WAIT. This function transmits a RST and informs the Soad about the
 * successful close. It sets the socket state to CLOSED.
 *
 * \param[in]  socketId   Socket identifier
 * \param[in]  socketPtr  Pointer to the socket connection information
 */
#define TcpIp_Tcp_SM_tr_immediateClose_TIME_WAIT_WAIT_CLOSED(SocketId, socketPtr)  \
  TcpIp_Tcp_SM_tr_immediateClose_SYN_RECEIVED_CLOSED((SocketId), (socketPtr))

/** \brief Update receive window
 *
 * In State FIN_WAIT_1. This function updates the receive window of the socket.
 *
 * \param[in]  socketPtr  Pointer to the socket connection information
 * \param[in]  Length     Number of bytes finally consumed by the upper layer
 */
#define TcpIp_Tcp_SM_tr_received_FIN_WAIT_1_FIN_WAIT_1(SocketPtr, Length)          \
  TcpIp_Tcp_SM_tr_received_ESTABLISHED_ESTABLISHED((SocketPtr), (Length))

/** \brief Update receive window
 *
 * In State FIN_WAIT_2. This function updates the receive window of the socket.
 *
 * \param[in]  socketPtr  Pointer to the socket connection information
 * \param[in]  Length     Number of bytes finally consumed by the upper layer
 */
#define TcpIp_Tcp_SM_tr_received_FIN_WAIT_2_FIN_WAIT_2(SocketPtr, Length)          \
  TcpIp_Tcp_SM_tr_received_ESTABLISHED_ESTABLISHED((SocketPtr), (Length))

/** \brief Handle SYN segment
 *
 * In State LAST_ACK. This function transmit a RST segment in response to an
 * unexpected SYN segment. It sets the socket state to CLOSED.
 *
 * \param[in]  Id     Id of the socket connection
 * \param[in]  sock   Pointer to connection information
 * \param[in]  ack    Acknowledgment Number of received segment
 * \param[in]  seq    Sequence Number of received segment
 * \param[in]  len    Segment length
 * \param[in]  flags  Control flags set in the received segment
 *
 * \retval TCPIP_E_DROP  Stop further processing
 */
#define TcpIp_Tcp_SM_tr_rcvSynCheckErr_LAST_ACK_CLOSED(Id, sock, ack, seq, len, flags) \
  TcpIp_Tcp_SM_tr_rcvSynCheckErr_ESTABLISHED_CLOSED((Id), (sock), (ack), (seq), (len), (flags))

/** \brief Handle SYN segment
 *
 * In State CLOSE_WAIT. This function transmit a RST segment in response to an
 * unexpected SYN segment. It sets the socket state to CLOSED.
 *
 * \param[in]  Id     Id of the socket connection
 * \param[in]  sock   Pointer to connection information
 * \param[in]  ack    Acknowledgment Number of received segment
 * \param[in]  seq    Sequence Number of received segment
 * \param[in]  len    Segment length
 * \param[in]  flags  Control flags set in the received segment
 *
 * \retval TCPIP_E_DROP  Stop further processing
 */
#define TcpIp_Tcp_SM_tr_rcvSynCheckErr_CLOSE_WAIT_CLOSED(Id, sock, ack, seq, len, flags) \
  TcpIp_Tcp_SM_tr_rcvSynCheckErr_ESTABLISHED_CLOSED((Id), (sock), (ack), (seq), (len), (flags))

/** \brief Handle SYN segment
 *
 * In State SYN_RECEIVED. This function transmit a RST segment in response to an
 * unexpected SYN segment. It sets the socket state to CLOSED.
 *
 * \param[in]  Id     Id of the socket connection
 * \param[in]  sock   Pointer to connection information
 * \param[in]  ack    Acknowledgment Number of received segment
 * \param[in]  seq    Sequence Number of received segment
 * \param[in]  len    Segment length
 * \param[in]  flags  Control flags set in the received segment
 *
 * \retval TCPIP_E_DROP  Stop further processing
 */
#define TcpIp_Tcp_SM_tr_rcvSynCheckErr_SYN_RECEIVED_CLOSED(Id, sock, ack, seq, len, flags) \
  TcpIp_Tcp_SM_tr_rcvSynCheckErr_ESTABLISHED_CLOSED((Id), (sock), (ack), (seq), (len), (flags))

/** \brief Handle SYN segment
 *
 * In State FIN_WAIT_1. This function transmit a RST segment in response to an
 * unexpected SYN segment. It sets the socket state to CLOSED.
 *
 * \param[in]  Id     Id of the socket connection
 * \param[in]  sock   Pointer to connection information
 * \param[in]  ack    Acknowledgment Number of received segment
 * \param[in]  seq    Sequence Number of received segment
 * \param[in]  len    Segment length
 * \param[in]  flags  Control flags set in the received segment
 *
 * \retval TCPIP_E_DROP  Stop further processing
 */
#define TcpIp_Tcp_SM_tr_rcvSynCheckErr_FIN_WAIT_1_CLOSED(Id, sock, ack, seq, len, flags) \
  TcpIp_Tcp_SM_tr_rcvSynCheckErr_ESTABLISHED_CLOSED((Id), (sock), (ack), (seq), (len), (flags))

/** \brief Handle SYN segment
 *
 * In State FIN_WAIT_2. This function transmit a RST segment in response to an
 * unexpected SYN segment. It sets the socket state to CLOSED.
 *
 * \param[in]  Id     Id of the socket connection
 * \param[in]  sock   Pointer to connection information
 * \param[in]  ack    Acknowledgment Number of received segment
 * \param[in]  seq    Sequence Number of received segment
 * \param[in]  len    Segment length
 * \param[in]  flags  Control flags set in the received segment
 *
 * \retval TCPIP_E_DROP  Stop further processing
 */
#define TcpIp_Tcp_SM_tr_rcvSynCheckErr_FIN_WAIT_2_CLOSED(Id, sock, ack, seq, len, flags) \
  TcpIp_Tcp_SM_tr_rcvSynCheckErr_ESTABLISHED_CLOSED((Id), (sock), (ack), (seq), (len), (flags))

/** \brief Handle SYN segment
 *
 * In State TIME_WAIT_WAIT. This function transmit a RST segment in response to an
 * unexpected SYN segment. It sets the socket state to CLOSED.
 *
 * \param[in]  Id     Id of the socket connection
 * \param[in]  sock   Pointer to connection information
 * \param[in]  ack    Acknowledgment Number of received segment
 * \param[in]  seq    Sequence Number of received segment
 * \param[in]  len    Segment length
 * \param[in]  flags  Control flags set in the received segment
 *
 * \retval TCPIP_E_DROP  Stop further processing
 */
#define TcpIp_Tcp_SM_tr_rcvSynCheckErr_TIME_WAIT_WAIT_CLOSED(Id, sock, ack, seq, len, flags) \
  TcpIp_Tcp_SM_tr_rcvSynCheckErr_ESTABLISHED_CLOSED((Id), (sock), (ack), (seq), (len), (flags))

/** \brief Handle SYN segment
 *
 * In State TIME_WAIT. This function transmit a RST segment in response to an
 * unexpected SYN segment. It sets the socket state to CLOSED.
 *
 * \param[in]  Id     Id of the socket connection
 * \param[in]  sock   Pointer to connection information
 * \param[in]  ack    Acknowledgment Number of received segment
 * \param[in]  seq    Sequence Number of received segment
 * \param[in]  len    Segment length
 * \param[in]  flags  Control flags set in the received segment
 *
 * \retval TCPIP_E_DROP  Stop further processing
 */
#define TcpIp_Tcp_SM_tr_rcvSynCheckErr_TIME_WAIT_CLOSED(Id, sock, ack, seq, len, flags) \
  TcpIp_Tcp_SM_tr_rcvSynCheckErr_ESTABLISHED_CLOSED((Id), (sock), (ack), (seq), (len), (flags))

/** \brief Handle SYN segment
 *
 * In State CLOSING. This function transmit a RST segment in response to an
 * unexpected SYN segment. It sets the socket state to CLOSED.
 *
 * \param[in]  Id     Id of the socket connection
 * \param[in]  sock   Pointer to connection information
 * \param[in]  ack    Acknowledgment Number of received segment
 * \param[in]  seq    Sequence Number of received segment
 * \param[in]  len    Segment length
 * \param[in]  flags  Control flags set in the received segment
 *
 * \retval TCPIP_E_DROP  Stop further processing
 */
#define TcpIp_Tcp_SM_tr_rcvSynCheckErr_CLOSING_CLOSED(Id, sock, ack, seq, len, flags) \
  TcpIp_Tcp_SM_tr_rcvSynCheckErr_ESTABLISHED_CLOSED((Id), (sock), (ack), (seq), (len), (flags))

/** \brief Handle ACK segment
 *
 * In state LAST_ACK. This function frees a socket and all remaining buffers and informs
 * Soad about the successful close. It sets the socket state to CLOSED.
 *
 * \param[in]  Id    Id of the socket connection
 * \param[in]  sock  Pointer to the socket information
 */
#define TcpIp_Tcp_SM_tr_rcvAck_LAST_ACK_CLOSED(Id, sock)                           \
  TcpIp_Tcp_SM_tr_regularClose_SYN_SENT_CLOSED((Id), (sock))

/** \brief Handle data segment
 *
 * In State FIN_WAIT_1. This function handles the payload of a segment. It pass
 * the data to Soad, triggers the transmission of a delayed acknowledgement and
 * transmits an acknowledgement if an out of order segment is detected.
 *
 * \param[in]  Id    Socket identifier
 * \param[in]  sock  Pointer to connection information
 * \param[in]  data  Points to the payload
 * \param[in]  Len   data length
 * \param[in]  seq   Sequence Number of received segment
 *
 * \retval TCPIP_OK        Data successfully processed
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    stop further processing
 */
#define TcpIp_Tcp_SM_tr_rcvData_FIN_WAIT_1_FIN_WAIT_1(Id, sock, data, Len, seq)    \
  TcpIp_Tcp_SM_tr_rcvData_ESTABLISHED_ESTABLISHED((Id), (sock), (data), (Len), (seq))

/** \brief Handle data segment
 *
 * In State FIN_WAIT_2. This function handles the payload of a segment. It pass
 * the data to Soad, triggers the transmission of a delayed acknowledgement and
 * transmits an acknowledgement if an out of order segment is detected.
 *
 * \param[in]  Id    Socket identifier
 * \param[in]  sock  Pointer to connection information
 * \param[in]  data  Points to the payload
 * \param[in]  Len   data length
 * \param[in]  seq   Sequence Number of received segment
 *
 * \retval TCPIP_OK        Data successfully processed
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    stop further processing
 */
#define TcpIp_Tcp_SM_tr_rcvData_FIN_WAIT_2_FIN_WAIT_2(Id, sock, data, Len, seq)    \
  TcpIp_Tcp_SM_tr_rcvData_ESTABLISHED_ESTABLISHED((Id), (sock), (data), (Len), (seq))

/** \brief Check sequence number
 *
 * In State FIN_WAIT_1. This function verifies if the sequence number of the received
 * segment lies inside the receive window. If sequence number is outside the window and
 * segment does not contain a RST, TCP transmits an ACK.
 *
 * \param[in]  sock   Pointer to connection information
 * \param[in]  len    Segment length
 * \param[in]  seq    Sequence Number of received segment
 * \param[in]  flags  Control flags set in the received segment
 *
 * \retval TCPIP_OK        Sequence number acceptable
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Out of memory, stop further processing
 */
#define TcpIp_Tcp_SM_tr_checkSeqNum_FIN_WAIT_1_FIN_WAIT_1(sock, len, seq, flags)   \
  TcpIp_Tcp_SM_tr_checkSeqNum_ESTABLISHED_ESTABLISHED((sock), (len), (seq), (flags))

/** \brief Check sequence number
 *
 * In State FIN_WAIT_2. This function verifies if the sequence number of the received
 * segment lies inside the receive window. If sequence number is outside the window and
 * segment does not contain a RST, TCP transmits an ACK.
 *
 * \param[in]  sock   Pointer to connection information
 * \param[in]  len    Segment length
 * \param[in]  seq    Sequence Number of received segment
 * \param[in]  flags  Control flags set in the received segment
 *
 * \retval TCPIP_OK        Sequence number acceptable
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Out of memory, stop further processing
 */
#define TcpIp_Tcp_SM_tr_checkSeqNum_FIN_WAIT_2_FIN_WAIT_2(sock, len, seq, flags)   \
  TcpIp_Tcp_SM_tr_checkSeqNum_ESTABLISHED_ESTABLISHED((sock), (len), (seq), (flags))

/** \brief Check sequence number
 *
 * In State SYN_RECEIVED. This function verifies if the sequence number of the received
 * segment lies inside the receive window. If sequence number is outside the window and
 * segment does not contain a RST, TCP transmits an ACK.
 *
 * \param[in]  sock   Pointer to connection information
 * \param[in]  len    Segment length
 * \param[in]  seq    Sequence Number of received segment
 * \param[in]  flags  Control flags set in the received segment
 *
 * \retval TCPIP_OK        Sequence number acceptable
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Out of memory, stop further processing
 */
#define TcpIp_Tcp_SM_tr_checkSeqNum_SYN_RECEIVED_SYN_RECEIVED(sock, len, seq, flags)  \
  TcpIp_Tcp_SM_tr_checkSeqNum_ESTABLISHED_ESTABLISHED((sock), (len), (seq), (flags))

/** \brief Check sequence number
 *
 * In State LAST_ACK. This function verifies if the sequence number of the received
 * segment lies inside the receive window. If sequence number is outside the window and
 * segment does not contain a RST, TCP transmits an ACK.
 *
 * \param[in]  sock   Pointer to connection information
 * \param[in]  len    Segment length
 * \param[in]  seq    Sequence Number of received segment
 * \param[in]  flags  Control flags set in the received segment
 *
 * \retval TCPIP_OK        Sequence number acceptable
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Out of memory, stop further processing
 */
#define TcpIp_Tcp_SM_tr_checkSeqNum_LAST_ACK_LAST_ACK(sock, len, seq, flags)       \
  TcpIp_Tcp_SM_tr_checkSeqNum_ESTABLISHED_ESTABLISHED((sock), (len), (seq), (flags))

/** \brief Check sequence number
 *
 * In State CLOSING. This function verifies if the sequence number of the received
 * segment lies inside the receive window. If sequence number is outside the window and
 * segment does not contain a RST, TCP transmits an ACK.
 *
 * \param[in]  sock   Pointer to connection information
 * \param[in]  len    Segment length
 * \param[in]  seq    Sequence Number of received segment
 * \param[in]  flags  Control flags set in the received segment
 *
 * \retval TCPIP_OK        Sequence number acceptable
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Out of memory, stop further processing
 */
#define TcpIp_Tcp_SM_tr_checkSeqNum_CLOSING_CLOSING(sock, len, seq, flags)         \
  TcpIp_Tcp_SM_tr_checkSeqNum_ESTABLISHED_ESTABLISHED((sock), (len), (seq), (flags))

/** \brief Check sequence number
 *
 * In State CLOSE_WAIT. This function verifies if the sequence number of the received
 * segment lies inside the receive window. If sequence number is outside the window and
 * segment does not contain a RST, TCP transmits an ACK.
 *
 * \param[in]  sock   Pointer to connection information
 * \param[in]  len    Segment length
 * \param[in]  seq    Sequence Number of received segment
 * \param[in]  flags  Control flags set in the received segment
 *
 * \retval TCPIP_OK        Sequence number acceptable
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Out of memory, stop further processing
 */
#define TcpIp_Tcp_SM_tr_checkSeqNum_CLOSE_WAIT_CLOSE_WAIT(sock, len, seq, flags)   \
  TcpIp_Tcp_SM_tr_checkSeqNum_ESTABLISHED_ESTABLISHED((sock), (len), (seq), (flags))

/** \brief Check sequence number
 *
 * In State TIME_WAIT. This function verifies if the sequence number of the received
 * segment lies inside the receive window. If sequence number is outside the window and
 * segment does not contain a RST, TCP transmits an ACK.
 *
 * \param[in]  sock   Pointer to connection information
 * \param[in]  len    Segment length
 * \param[in]  seq    Sequence Number of received segment
 * \param[in]  flags  Control flags set in the received segment
 *
 * \retval TCPIP_OK        Sequence number acceptable
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Out of memory, stop further processing
 */
#define TcpIp_Tcp_SM_tr_checkSeqNum_TIME_WAIT_TIME_WAIT(sock, len, seq, flags)     \
  TcpIp_Tcp_SM_tr_checkSeqNum_ESTABLISHED_ESTABLISHED((sock), (len), (seq), (flags))

/** \brief Check acknowledgement number
 *
 * In State SYN_RECEIVED. This function verifies if the acknowledgement number of the
 * received segment acknowledge previously transmitted data. If acknowledgement number
 * acknowledge something not yet sent, TCP transmits a RST.
 *
 * \param[in]  sock   Pointer to connection information
 * \param[in]  ack    Acknowledgement Number of received segment
 * \param[in]  flags  Control flags set in the received segment
 *
 * \retval TCPIP_OK        Acknowledgement number acceptable
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Stop further processing
 */
#define TcpIp_Tcp_SM_tr_checkAckNum_SYN_RECEIVED_SYN_RECEIVED(sock, ack, flags) \
  TcpIp_Tcp_SM_tr_checkAckNum_SYN_SENT_SYN_SENT((sock), (ack), (flags))

/** \brief Check acknowledgement number
 *
 * In State TIME_WAIT. This function verifies if the acknowledgement number of the
 * received segment acknowledge previously transmitted data. If acknowledgement number
 * acknowledge something not yet sent, TCP transmits an ACK.
 *
 * \param[in]  sock   Pointer to connection information
 * \param[in]  ack    Acknowledgement Number of received segment
 *
 * \retval TCPIP_OK        Acknowledgement number acceptable
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Stop further processing
 */
#define TcpIp_Tcp_SM_tr_checkAckNum_TIME_WAIT_TIME_WAIT(sock, ack)                 \
  TcpIp_Tcp_SM_tr_checkAckNum_ESTABLISHED_ESTABLISHED((sock), (ack))

/** \brief Check acknowledgement number
 *
 * In State LAST_ACK. This function verifies if the acknowledgement number of the
 * received segment acknowledge previously transmitted data. If acknowledgement number
 * acknowledge something not yet sent, TCP transmits an ACK.
 *
 * \param[in]  sock   Pointer to connection information
 * \param[in]  ack    Acknowledgement Number of received segment
 *
 * \retval TCPIP_OK        Acknowledgement number acceptable
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Stop further processing
 */
#define TcpIp_Tcp_SM_tr_checkAckNum_LAST_ACK_LAST_ACK(sock, ack)                   \
  TcpIp_Tcp_SM_tr_checkAckNum_ESTABLISHED_ESTABLISHED((sock), (ack))

/** \brief Check acknowledgement number
 *
 * In State CLOSING. This function verifies if the acknowledgement number of the
 * received segment acknowledge previously transmitted data. If acknowledgement number
 * acknowledge something not yet sent, TCP transmits an ACK.
 *
 * \param[in]  sock   Pointer to connection information
 * \param[in]  ack    Acknowledgement Number of received segment
 *
 * \retval TCPIP_OK        Acknowledgement number acceptable
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Stop further processing
 */
#define TcpIp_Tcp_SM_tr_checkAckNum_CLOSING_CLOSING(sock, ack)                     \
  TcpIp_Tcp_SM_tr_checkAckNum_ESTABLISHED_ESTABLISHED((sock), (ack))

/** \brief Check acknowledgement number
 *
 * In State CLOSE_WAIT. This function verifies if the acknowledgement number of the
 * received segment acknowledge previously transmitted data. If acknowledgement number
 * acknowledge something not yet sent, TCP transmits an ACK.
 *
 * \param[in]  sock   Pointer to connection information
 * \param[in]  ack    Acknowledgement Number of received segment
 *
 * \retval TCPIP_OK        Acknowledgement number acceptable
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Stop further processing
 */
#define TcpIp_Tcp_SM_tr_checkAckNum_CLOSE_WAIT_CLOSE_WAIT(sock, ack)               \
  TcpIp_Tcp_SM_tr_checkAckNum_ESTABLISHED_ESTABLISHED((sock), (ack))

/** \brief Check acknowledgement number
 *
 * In State FIN_WAIT_2. This function verifies if the acknowledgement number of the
 * received segment acknowledge previously transmitted data. If acknowledgement number
 * acknowledge something not yet sent, TCP transmits an ACK.
 *
 * \param[in]  sock   Pointer to connection information
 * \param[in]  ack    Acknowledgement Number of received segment
 *
 * \retval TCPIP_OK        Acknowledgement number acceptable
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Stop further processing
 */
#define TcpIp_Tcp_SM_tr_checkAckNum_FIN_WAIT_2_FIN_WAIT_2(sock, ack)               \
  TcpIp_Tcp_SM_tr_checkAckNum_ESTABLISHED_ESTABLISHED((sock), (ack))

/** \brief Check acknowledgement number
 *
 * In State FIN_WAIT_1. This function verifies if the acknowledgement number of the
 * received segment acknowledge previously transmitted data. If acknowledgement number
 * acknowledge something not yet sent, TCP transmits an ACK.
 *
 * \param[in]  sock   Pointer to connection information
 * \param[in]  ack    Acknowledgement Number of received segment
 *
 * \retval TCPIP_OK        Acknowledgement number acceptable
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Stop further processing
 */
#define TcpIp_Tcp_SM_tr_checkAckNum_FIN_WAIT_1_FIN_WAIT_1(sock, ack)               \
  TcpIp_Tcp_SM_tr_checkAckNum_ESTABLISHED_ESTABLISHED((sock), (ack))

/** \brief Handle a state timeout
 *
 * In state TIME_WAIT. This function frees a socket and all remaining buffers and
 * informs Soad about the successful close. It sets the socket state to CLOSED.
 *
 * \param[in]  Id   Id of the socket connection
 * \param[in]  sock  Pointer to connection information
 */
#define TcpIp_Tcp_SM_tr_stateTimeout_TIME_WAIT_CLOSED(Id, sock)                    \
  TcpIp_Tcp_SM_tr_regularClose_SYN_SENT_CLOSED((Id), (sock))

/** \brief Handle a state timeout
 *
 * In state FIN_WAIT_2. This function frees a socket and all remaining buffers and
 * informs Soad about the successful close. It sets the socket state to CLOSED.
 *
 * \param[in]  Id    Id of the socket connection
 * \param[in]  sock  Pointer to connection information
 */
#define TcpIp_Tcp_SM_tr_stateTimeout_FIN_WAIT_2_CLOSED(Id, sock)                   \
  TcpIp_Tcp_SM_tr_rcvRst_ANY_CLOSED((Id), (sock))

/** \brief Handle a state timeout
 *
 * In state ESTABLISHED. This function frees a socket and all remaining buffers and
 * informs Soad about the successful close. It sets the socket state to CLOSED.
 *
 * \param[in]  Id    Id of the socket connection
 * \param[in]  sock  Pointer to connection information
 */
#define TcpIp_Tcp_SM_tr_stateTimeout_ESTABLISHED_CLOSED(Id, sock)                  \
  TcpIp_Tcp_SM_tr_rcvRst_ANY_CLOSED((Id), (sock))

/** \brief Handle a state timeout
 *
 * In state SYN_RECEIVED. This function frees a socket and all remaining buffers and
 * informs Soad about the successful close. It sets the socket state to CLOSED.
 *
 * \param[in]  Id    Socket identifier
 * \param[in]  sock  Pointer to connection information
 *
 * \retval TCPIP_E_DROP    stop further processing
 */
#define TcpIp_Tcp_SM_tr_stateTimeout_SYN_RECEIVED_CLOSED(Id, sock)                 \
  TcpIp_Tcp_SM_tr_rcvRst_ANY_CLOSED((Id), (sock))

/** \brief Handle a retransmission timeout
 *
 * In state SYN_SENT. This function frees a socket and all remaining buffers and informs
 * Soad about the reset. It sets the socket state to CLOSED.
 *
 * \param[in]  Id    Socket identifier
 * \param[in]  sock  Pointer to connection information
 *
 * \retval TCPIP_E_DROP    stop further processing
 */
#define TcpIp_Tcp_SM_tr_rtxTimeout_SYN_SENT_CLOSED(Id, sock)                        \
  TcpIp_Tcp_SM_tr_rcvRst_ANY_CLOSED((Id), (sock))

/** \brief Handle a retransmission timeout
 *
 * In state ESTABLISHED. This function frees a socket and all remaining buffers and informs
 * Soad about the reset. It sets the socket state to CLOSED.
 *
 * \param[in]  Id    Socket identifier
 * \param[in]  sock  Pointer to connection information
 *
 * \retval TCPIP_E_DROP    stop further processing
 */
#define TcpIp_Tcp_SM_tr_rtxTimeout_ESTABLISHED_CLOSED(Id, sock)                      \
  TcpIp_Tcp_SM_tr_rcvRst_ANY_CLOSED((Id), (sock))

/** \brief Handle a retransmission timeout
 *
 * In state SYN_RECEIVED. This function frees a socket and all remaining buffers and
 * informs Soad about the reset. It sets the socket state to CLOSED.
 *
 * \param[in]  Id    Socket identifier
 * \param[in]  sock  Pointer to connection information
 *
 * \retval TCPIP_E_DROP    stop further processing
 */
#define TcpIp_Tcp_SM_tr_rtxTimeout_SYN_RECEIVED_CLOSED(Id, sock)                    \
  TcpIp_Tcp_SM_tr_rcvRst_ANY_CLOSED((Id), (sock))

/** \brief Handle a retransmission timeout
 *
 * In state FIN_WAIT_1. This function frees a socket and all remaining buffers and
 * informs Soad about the reset. It sets the socket state to CLOSED.
 *
 * \param[in]  Id    Socket identifier
 * \param[in]  sock  Pointer to connection information
 *
 * \retval TCPIP_E_DROP    stop further processing
 */
#define TcpIp_Tcp_SM_tr_rtxTimeout_FIN_WAIT_1_CLOSED(Id, sock)                     \
  TcpIp_Tcp_SM_tr_rcvRst_ANY_CLOSED((Id), (sock))

/** \brief Handle a retransmission timeout
 *
 * In state CLOSE_WAIT. This function frees a socket and all remaining buffers and
 * informs Soad about the reset. It sets the socket state to CLOSED.
 *
 * \param[in]  Id    Socket identifier
 * \param[in]  sock  Pointer to connection information
 *
 * \retval TCPIP_E_DROP    stop further processing
 */
#define TcpIp_Tcp_SM_tr_rtxTimeout_CLOSE_WAIT_CLOSED(Id, sock)                      \
  TcpIp_Tcp_SM_tr_rcvRst_ANY_CLOSED((Id), (sock))

/** \brief Handle a retransmission timeout
 *
 * In state LAST_ACK. This function frees a socket and all remaining buffers and
 * informs Soad about the reset. It sets the socket state to CLOSED.
 *
 * \param[in]  Id   Socket identifier
 * \param[in]  sock  Pointer to connection information
 *
 * \retval TCPIP_E_DROP    stop further processing
 */
#define TcpIp_Tcp_SM_tr_rtxTimeout_LAST_ACK_CLOSED(Id, sock)                        \
  TcpIp_Tcp_SM_tr_rcvRst_ANY_CLOSED((Id), (sock))

/** \brief Handle a retransmission timeout
 *
 * In state CLOSING. This function frees a socket and all remaining buffers and
 * informs Soad about the reset. It sets the socket state to CLOSED.
 *
 * \param[in]  Id   Socket identifier
 * \param[in]  sock  Pointer to connection information
 *
 * \retval TCPIP_E_DROP    stop further processing
 */
#define TcpIp_Tcp_SM_tr_rtxTimeout_CLOSING_CLOSED(Id, sock)                         \
  TcpIp_Tcp_SM_tr_rcvRst_ANY_CLOSED((Id), (sock))

/** \brief Handle a retransmission timeout
 *
 * This function retransmits sent data and control messages.
 *
 * \param[in]  socketPtr   Pointer to socket connection information
 *
 * \retval TCPIP_OK        retransmission successful
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Out of memory, stop further processing
 */
#define TcpIp_Tcp_SM_tr_rtxTimeout_SYN_SENT_SYN_SENT(socketPtr)                    \
  TcpIp_Tcp_SM_tr_rtxTimeout_ESTABLISHED_ESTABLISHED((socketPtr))

/** \brief Handle a retransmission timeout
 *
 * This function retransmits sent data and control messages.
 *
 * \param[in]  socketPtr   Pointer to socket connection information
 *
 * \retval TCPIP_OK        retransmission successful
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Out of memory, stop further processing
 */
#define TcpIp_Tcp_SM_tr_rtxTimeout_SYN_RECEIVED_SYN_RECEIVED(socketPtr)             \
  TcpIp_Tcp_SM_tr_rtxTimeout_ESTABLISHED_ESTABLISHED((socketPtr))

/** \brief Handle a retransmission timeout
 *
 * This function retransmits sent data and control messages.
 *
 * \param[in]  socketPtr   Pointer to socket connection information
 *
 * \retval TCPIP_OK        retransmission successful
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Out of memory, stop further processing
 */
#define TcpIp_Tcp_SM_tr_rtxTimeout_FIN_WAIT_1_FIN_WAIT_1(socketPtr)                \
  TcpIp_Tcp_SM_tr_rtxTimeout_ESTABLISHED_ESTABLISHED((socketPtr))

/** \brief Handle a retransmission timeout
 *
 * This function retransmits sent data and control messages.
 *
 * \param[in]  socketPtr   Pointer to socket connection information
 *
 * \retval TCPIP_OK        retransmission successful
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Out of memory, stop further processing
 */
#define TcpIp_Tcp_SM_tr_rtxTimeout_CLOSE_WAIT_CLOSE_WAIT(socketPtr)                \
  TcpIp_Tcp_SM_tr_rtxTimeout_ESTABLISHED_ESTABLISHED((socketPtr))

/** \brief Handle a retransmission timeout
 *
 * This function retransmits sent data and control messages.
 *
 * \param[in]  socketPtr   Pointer to socket connection information
 *
 * \retval TCPIP_OK        retransmission successful
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Out of memory, stop further processing
 */
#define TcpIp_Tcp_SM_tr_rtxTimeout_LAST_ACK_LAST_ACK(socketPtr)                   \
  TcpIp_Tcp_SM_tr_rtxTimeout_ESTABLISHED_ESTABLISHED((socketPtr))

/** \brief Handle a retransmission timeout
 *
 * This function retransmits sent data and control messages.
 *
 * \param[in]  socketPtr   Pointer to socket connection information
 *
 * \retval TCPIP_OK        retransmission successful
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Out of memory, stop further processing
 */
#define TcpIp_Tcp_SM_tr_rtxTimeout_CLOSING_CLOSING(socketPtr)                    \
  TcpIp_Tcp_SM_tr_rtxTimeout_ESTABLISHED_ESTABLISHED((socketPtr))

#if (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON)
/* Parameters for SYN cookies */
/** \brief JobId for SYN cookies key 1 */
#define TCPIP_TCP_SYN_COOKIES_JOB_1          TcpIp_PBcfgPtr->tcpConfig.synCookiesjobId1

/** \brief JobId for SYN cookies key 2 */
#define TCPIP_TCP_SYN_COOKIES_JOB_2          TcpIp_PBcfgPtr->tcpConfig.synCookiesjobId2

/** \brief Time after overflow when ack is accepted */
#define TCPIP_TCP_SYN_COOKIES_ACCEPT_ACK     TcpIp_PBcfgPtr->tcpConfig.synCookiesAcceptAckOverflowTime

/** \brief Hash array size for Syn cookies */
#define TCPIP_TCP_SYN_COOKIES_HASH_ARRAYS_SIZE TCPIP_TCP_INTERNAL_HASH_ADDR_PORT_SIZE + sizeof(uint32)

/** \brief Counter is incrementing for each 64 seconds passed */
#define TCPIP_TCP_SYN_COOKIES_COUNTER_INCREMENT                                   64U

/** \brief Sequence number should be always checked against current counter and the
           last one because it could have been incremented a second ago */
#define TCPIP_TCP_SYN_COOKIES_COUNTER_NUM                                          2U

/** \brief Mss table size */
#define TCPIP_TCP_SYN_COOKIES_MSS_TABLE_SIZE                                       8U

/** \brief Obtain the number of times 64 second mark occurred */
#define TCPIP_TCP_SYN_COOKIES_GET_COUNTER_FROM_TIME(currentTimeStamp)     \
  (((uint32)(currentTimeStamp.secondsHi) & 0x3fU) << 26U) | (currentTimeStamp.seconds >> 6U)
#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON) */

#if (TCPIP_TCP_UNPREDICTABLE_INIT_SEQ_NUM_ENABLED == STD_ON)
/** \brief Unpredictable initial sequence number job id */
#define TCPIP_TCP_UNPR_SEQ_NUM_JOBID        TcpIp_PBcfgPtr->tcpConfig.unprSeqNumJobId
#endif /* (TCPIP_TCP_UNPREDICTABLE_INIT_SEQ_NUM_ENABLED == STD_ON) */
/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/


/*==================[internal function declarations]========================*/

#if (TCPIP_TCP_ENABLED == STD_ON)

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief Evaluate size of a segment
 *
 * This function evaluates based on the send mss and the mtu, the maximal size of
 * a segment
 *
 * \param[in]  localAddrId    Local address the packet is received on
 * \param[in]  maxPayload     maximal payload length
 * \param[in]  sendMss        Mss of received segment
 *
 * \return maximal size of a segment
 */
STATIC FUNC(uint16, TCPIP_CODE) TcpIp_Tcp_SM_evaluteSendMSS
(
  TcpIp_LocalAddrIdType localAddrId,
  uint16 maxPayload,
  uint16 sendMss
);

/** \brief Active open
 *
 * In state CLOSED. This function fills in the connection information and starts to
 * establish a connection to a remote host by transmitting a SYN segment. It sets the
 * socket state to SYN_SENT_WAIT.
 *
 * \param[in]  SocketId       id of the socket connection
 * \param[in]  socketPtr      Pointer to the socket connection information
 * \param[in]  RemoteAddrPtr  IP Address and port of the remote host
 *
 * \retval TCPIP_OK        SYN successfully transmitted
 * \retval TCPIP_E_NOT_OK  Error during transmission occurred, connection already exists
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_connect_CLOSED_SYN_SENT
(
  TcpIp_SocketIdType SocketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RemoteAddrPtr
);

/** \brief Passive Open
 *
 * In state CLOSED. This function appends a socket to the listen socket list. It
 * sets the socket state to LISTEN.
 *
 * \param[in]  SocketId      Id of the socket connection
 * \param[in]  ctrlBlockPtr  Pointer to the transmission control block
 * \param[in]  MaxChannels   Maximum number of new parallel connections established on
 *                           this listen connection.
 *
 * \retval TCPIP_OK        Request successfully performed
 * \retval TCPIP_E_NOT_OK  Operation not allowed in this state
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_listen_CLOSED_LISTEN
(
  TcpIp_SocketIdType SocketId,
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr,
  uint16 MaxChannels
);

/** \brief Regular close of a TCP connection
 *
 * In state SYN_SENT. This function frees a socket and all remaining buffers and informs
 * Soad about the successful close. It sets the socket state to CLOSED.
 *
 * \param[in]  SocketId      Id of the socket connection
 * \param[in]  socketPtr   Pointer to the socket connection information
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_SM_tr_regularClose_SYN_SENT_CLOSED
(
  TcpIp_SocketIdType SocketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Regular close of a TCP connection
 *
 * In state ESTABLISHED. This function closes a TCP connection by transmitting a FIN.
 * It stops the state timer and sets the socket state to FIN_WAIT_1.
 *
 * \param[in]  socketPtr   Pointer to the socket connection information
 *
 * \retval TCPIP_OK        FIN successfully transmitted
 * \retval TCPIP_E_NOT_OK  Error during transmission occurred
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_regularClose_ESTABLISHED_FIN_WAIT_1
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Regular close of a TCP connection
 *
 * In state CLOSE_WAIT. This function closes a TCP connection by transmitting a FIN.
 *
 * \param[in]  socketPtr   Pointer to the socket connection information
 *
 * \retval TCPIP_OK        FIN successfully transmitted
 * \retval TCPIP_E_NOT_OK  Error during transmission occurred
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_regularClose_CLOSE_WAIT_LAST_ACK
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Regular close of a TCP connection
 *
 * This function closes a TCP connection by transmitting a FIN.
 *
 * \param[in]  socketPtr   Pointer to the socket connection information
 * \param[in]  state       Socket state after successful transmission
 *
 * \retval TCPIP_OK        FIN successfully transmitted
 * \retval TCPIP_E_NOT_OK  Error during transmission occurred
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_regularClose
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  TcpIp_TcpStateType state
);

/** \brief Update receive window
 *
 * In State ESTABLISHED. This function updates the receive window of the socket.
 *
 * \param[in]  socketPtr  Pointer to the socket connection information
 * \param[in]  Length     Number of bytes finally consumed by the upper layer
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_received_ESTABLISHED_ESTABLISHED
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  uint16 Length
);

/** \brief Immediate close of a TCP connection
 *
 * In State LISTEN. This function informs the Soad about the successful close.
 * It sets the socket state to CLOSED.
 *
 * \param[in]  socketId   Socket identifier
 * \param[in]  socketPtr  Pointer to the socket connection information
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_SM_tr_immediateClose_LISTEN_CLOSED
(
  TcpIp_SocketIdType SocketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Immediate close of a TCP connection
 *
 * In State SYN_RECEIVED. This function transmits a RST and informs the Soad about the
 * successful close. It sets the socket state to CLOSED.
 *
 * \param[in]  socketId   Socket identifier
 * \param[in]  socketPtr  Pointer to the socket connection information
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_SM_tr_immediateClose_SYN_RECEIVED_CLOSED
(
  TcpIp_SocketIdType SocketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Immediate close of a TCP connection
 *
 * In State CLOSED. This function informs the Soad about the successful close.
 *
 * \param[in]  socketId      Socket identifier
 * \param[in]  socketPtr  Pointer to the socket connection information
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_SM_tr_immediateClose_CLOSED_CLOSED
(
  TcpIp_SocketIdType SocketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Handle SYN segment
 *
 * In State LISTEN. This function transmit a SYN,ACK segment in response to a SYN segment
 * and stores the connection information of the remote host. It sets the socket state to
 * SYN_RECEIVED.
 *
 * \param[in]  listenSocketId   Socket id of the listen socket
 * \param[in]  listenSocketPtr  Pointer to connection information (listen socket)
 * \param[in]  remoteAddrPtr    IP address and port of the remote host
 * \param[in]  localAddrPtr     IP address and port of the local host
 * \param[in]  mss              Maximum segment size of segment
 * \param[in]  seqNum           Sequence Number of received segment
 * \param[in]  seglen           Segment length
 *
 * \retval TCPIP_OK        SYN,ACK successfully transmitted
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_rcvSyn_LISTEN_SYN_RECEIVED
(
  TcpIp_SocketIdType listenSocketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) listenSocketPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(TcpIp_SockAddrLocalType, AUTOMATIC, TCPIP_APPL_DATA) localAddrPtr,
  uint16 mss,
  uint32 seqNum,
  uint16 seglen
);

/** \brief Handle SYN segment
 *
 * In State SYN_SENT. This function transmit a SYN,ACK segment in response to a SYN segment
 * and stores the connection information of the remote host. (simultaneous open attempt)
 *
 * \param[in]  socketPtr   Pointer to connection information
 * \param[in]  mss         Maximum segment size of segment
 * \param[in]  seqNum      Sequence Number of received segment
 * \param[in]  seglen      Segment length
 *
 * \retval TCPIP_OK        SYN,ACK successfully transmitted
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, retry next mainfunction
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_rcvSyn_SYN_SENT_SYN_RECEIVED
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  uint16 mss,
  uint32 seqNum,
  uint16 seglen
);

/** \brief Handle SYN segment
 *
 * In State SYN_SENT. This function transmit a ACK segment in response to a SYN,ACK
 * segment, stores the connection information of the remote host and informs Soad
 * about successful connection establishment. It sets the socket state to
 * ESTABLISHED.
 *
 * \param[in]  SocketId    Id of the socket connection
 * \param[in]  socketPtr   Pointer to connection information
 * \param[in]  mss         Maximum segment size of segment
 * \param[in]  seqNum      Sequence Number of received segment
 * \param[in]  seglen      Segment length
 *
 * \retval TCPIP_OK        SYN,ACK successfully transmitted
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, retry next mainfunction
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_rcvSyn_SYN_SENT_ESTABLISHED
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  uint16 mss,
  uint32 seqNum,
  uint16 seglen
);

/** \brief Handle SYN segment
 *
 * In State ESTABLISHED. This function transmit a RST segment in response to an unexpected
 * SYN segment. It sets the socket state to CLOSED.
 *
 * \param[in]  SocketId    Id of the socket connection
 * \param[in]  socketPtr   Pointer to connection information
 * \param[in]  ackNum      Acknowledgment Number of received segment
 * \param[in]  seqNum      Sequence Number of received segment
 * \param[in]  seglen      Segment length
 * \param[in]  flags       control flags set in the received segment
 *
 * \retval TCPIP_E_DROP    Stop further processing
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_rcvSynCheckErr_ESTABLISHED_CLOSED
(
  TcpIp_SocketIdType SocketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  uint32 ackNum,
  uint32 seqNum,
  uint16 seglen,
  uint8 flags
);

/** \brief Handle FIN segment
 *
 * In State FIN_WAIT_2. This function transmits an ACK in response to a FIN segment.
 * It sets the socket state to TIME_WAIT.
 *
 * \param[in]  socketId    Socket identifier
 * \param[in]  seqNum      Sequence number of the FIN
 * \param[in]  socketPtr   Pointer to connection information
 *
 * \retval TCPIP_OK        ACK successfully transmitted
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, retry next mainfunction
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_rcvFin_FIN_WAIT_2_TIME_WAIT
(
  TcpIp_SocketIdType socketId,
  uint32 seqNum,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Handle FIN segment
 *
 * In State FIN_WAIT_1. This function transmits an ACK in response to a FIN and sets
 * the socket state to CLOSING_WAIT.
 *
 * \param[in]  socketId    Socket identifier
 * \param[in]  seqNum      Sequence number of the FIN
 * \param[in]  socketPtr   Pointer to connection information
 *
 * \retval TCPIP_OK        ACK successfully transmitted
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, retry next mainfunction
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_rcvFin_FIN_WAIT_1_CLOSING
(
  TcpIp_SocketIdType socketId,
  uint32 seqNum,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Handle FIN segment
 *
 * In State ESTABLISHED_WAIT. This function transmits an ACK in response to a FIN segment
 * and informs Soad about the reception of a FIN. It stops the state timer and sets the
 * socket state to CLOSE_WAIT.
 *
 * \param[in]  socketId    Socket identifier
 * \param[in]  seqNum      Sequence number of the FIN
 * \param[in]  socketPtr   Pointer to connection information
 *
 * \retval TCPIP_OK        ACK successfully transmitted
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, retry next mainfunction
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_rcvFin_ESTABLISHED_CLOSE_WAIT
(
  TcpIp_SocketIdType socketId,
  uint32 seqNum,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Handle RST segment
 *
 * This function frees a socket and all remaining buffers and informs
 * Soad about the reset. It sets the socket state to CLOSED.
 *
 * \param[in]  SocketId   Socket identifier
 * \param[in]  socketPtr  Pointer to connection information
 *
 * \retval TCPIP_E_DROP    stop further processing
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_rcvRst_ANY_CLOSED
(
  TcpIp_SocketIdType SocketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Handle ACK segment
 *
 * In State SYN_RECEIVED. If ACK is received this function sets the socket state to
 * ESTABLISHED.
 *
 * \param[in]  socketPtr   Pointer to connection information
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_rcvAck_SYN_RECEIVED_ESTABLISHED
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Handle ACK segment
 *
 * In State FIN_WAIT_1. If ACK is received this function sets the socket state to
 * FIN_WAIT_2.
 *
 * \param[in]  socketPtr   Pointer to connection information
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_SM_tr_rcvAck_FIN_WAIT_1_FIN_WAIT_2
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Handle ACK segment
 *
 * In State CLOSING. If ACK is received this function sets the socket state to
 * TIME_WAIT and starts 2*MSL timer.
 *
 * \param[in]  socketPtr   Pointer to connection information
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_SM_tr_rcvAck_CLOSING_TIME_WAIT
(
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr
);

#if (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON)

/** \brief Handle ACK segment
 *
 * In State ESTABLISHED. If not any data is outstanding to be acknowledged, the
 * keep alive timer is started.
 *
 * \param[in]  socketPtr   Pointer to connection information
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_SM_tr_rcvAck_ESTABLISHED_ESTABLISHED
(
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr
);

#endif /* (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON) */

/** \brief Handle data segment
 *
 * In State ESTABLISHED. This function handles the payload of a segment. It pass the data
 * to to Soad, triggers the transmission of a delayed acknowledgement and transmits an
 * acknowledgement if an out of order segment is detected.
 *
 * \param[in]  socketId    Socket identifier
 * \param[in]  socketPtr   Pointer to connection information
 * \param[in]  dataPtr     Points to the payload
 * \param[in]  dataLen     data length
 * \param[in]  sequenceNum  Sequence Number of received segment
 *
 * \retval TCPIP_OK        Data successfully processed
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, retry next mainfunction
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_rcvData_ESTABLISHED_ESTABLISHED
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 dataLen,
  uint32 sequenceNum
);

/** \brief Check sequence number
 *
 * In State ESTABLISHED. This function verifies if the sequence number of the received
 * segment lies inside the receive window. If sequence number is outside the window and
 * segment does not contain a RST, TCP transmits an ACK.
 *
 * \param[in]  socketPtr  Pointer to connection information
 * \param[in]  seglen     Segment length
 * \param[in]  seqNumber  Sequence Number of received segment
 * \param[in]  flags      Control flags set in the received segment
 *
 * \retval TCPIP_OK        Sequence number acceptable
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, retry next mainfunction
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_checkSeqNum_ESTABLISHED_ESTABLISHED
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  uint16 seglen,
  uint32 seqNumber,
  uint8 flags
);

/** \brief Check acknowledgement number
 *
 * In State SYN_SENT. This function verifies if the acknowledgement number of the
 * received segment acknowledge previously transmitted data. If acknowledgement number
 * acknowledge something not yet sent, TCP transmits a RST.
 *
 * \param[in]  socketPtr   Pointer to connection information
 * \param[in]  ackNumber   Acknowledgement Number of received segment
 * \param[in]  flags       Control flags set in the received segment
 *
 * \retval TCPIP_OK        Acknowledgement number acceptable
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 * \retval TCPIP_E_DROP    Stop further processing
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_checkAckNum_SYN_SENT_SYN_SENT
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  uint32 ackNumber,
  uint8 flags
);

/** \brief Check acknowledgement number
 *
 * In State ESTABLISHED. This function verifies if the acknowledgement number of the
 * received segment acknowledge previously transmitted data. If acknowledgement number
 * acknowledge something not yet sent, TCP transmits an ACK.
 *
 * \param[in]  socketPtr   Pointer to connection information
 * \param[in]  ackNumber   Acknowledgement Number of received segment
 *
 * \retval TCPIP_OK        Acknowledgement number acceptable
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, retry next mainfunction
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_checkAckNum_ESTABLISHED_ESTABLISHED
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  uint32 ackNumber
);

/** \brief Check acknowledgement number
 *
 * In State LISTEN. If a ACK is received in this state this function transmits a RST.
 *
 * \param[in]  socketPtr      Pointer to connection information
 * \param[in]  RemoteAddrPtr  IP Address and port of the remote host
 * \param[in]  localAddr      IP Address and port on which the segment was received on
 * \param[in]  ackNum         Acknowledgement Number of received segment
 * \param[in]  seqNum         Sequence Number of received segment
 * \param[in]  seglen         Segment length
 * \param[in]  flags          Control flags set in the received segment
 *
 * \retval TCPIP_E_DROP       Stop further processing
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_checkAckNum_LISTEN_LISTEN
(
  P2CONST(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  TcpIp_SockAddrLocalType localAddr,
  uint32 ackNum,
  uint32 seqNum,
  uint16 seglen,
  uint8 flags
);

#if (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON)

/** \brief Handle keep alive timeout
 *
 * This function transmits a keep alive probe.
 *
 * \param[in]  socketPtr   Pointer to socket connection information
 *
 * \retval TCPIP_OK        transmission successful
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, close connection
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_stateTimeout_ESTABLISHED_ESTABLISHED
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

#endif /* (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON) */

/** \brief Handle a retransmission timeout
 *
 * This function retransmits sent data and control messages.
 *
 * \param[in]  socketPtr   Pointer to socket connection information
 *
 * \retval TCPIP_OK        retransmission successful
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, retry next mainfunction
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_rtxTimeout_ESTABLISHED_ESTABLISHED
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Entry of LISTEN state
 *
 * This function appends a socket to the listen socket list. It sets the socket state to
 * LISTEN.
 *
 * \param[in]  SocketId      Id of the socket connection
 * \param[in]  ctrlBlockPtr  Pointer to the transmission control block
 * \param[in]  MaxChannels   Maximum number of new parallel connections established on
 *                           this listen connection.
 *
 * \retval TCPIP_OK        Request successfully performed
 * \retval TCPIP_E_NOT_OK  Operation not allowed in this state
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_entry_LISTEN
(
  TcpIp_SocketIdType SocketId,
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr,
  uint16 MaxChannels
);

/** \brief Entry of CLOSED state
 *
 * This function frees the remaining buffers and calls <Up>_TcpIpEvent() to report
 * the socket state.
 *
 * \param[in]  SocketId   Id of the socket connection
 * \param[in]  socketPtr  Pointer to socket connection information
 * \param[in]  event      Event that shall be reported to Soad
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_SM_entry_CLOSED
(
  TcpIp_SocketIdType SocketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  TcpIp_EventType event,
  boolean callSocket
);

/** \brief Entry of TIME_WAIT state
 *
 * This function starts the 2 * msl timer.
 *
 * \param[in]  ctrlBlockPtr  Pointer to the TCP control block
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_SM_entry_TIME_WAIT
(
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr
);

/** \brief Entry of CLOSING state
 *
 * This function transmits an ACK in response to a FIN.
 *
 * \param[in]  SocketId   Id of the socket connection
 * \param[in]  seqNum     Sequence number of the Fin
 * \param[in]  socketPtr  Pointer to socket connection information
 *
 * \retval TCPIP_OK        transmission successful
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, retry next mainfunction
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_entry_CLOSING
(
  TcpIp_SocketIdType socketId,
  uint32 seqNum,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Transmit ACK segment
 *
 * This function transmits an ACK in response to a FIN and reports FIN received to Soad.
 *
 * \param[in]  SocketId   Id of the socket connection
 * \param[in]  state      Socket state after transmission
 * \param[in]  seqNum     Sequence number of the Fin
 * \param[in]  socketPtr  Pointer to socket connection information
 * \param[in]  callSoad   TRUE: call Soad_TcpEvent(TCPIP_TCP_FIN_RECEIVED)
 *                        FALSE: do not inform Soad
 *
 * \retval TCPIP_OK        transmission successful
 * \retval TCPIP_E_NOT_OK  Transmission error occurred, retry next mainfunction
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_rcvFin
(
  TcpIp_SocketIdType socketId,
  TcpIp_TcpStateType state,
  uint32 seqNum,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  boolean callSoad
);

/** \brief Entry of FIN_WAIT_2 state
 *
 * This function starts the FIN_WAIT_2 timer.
 *
 * \param[in]  ctrlBlockPtr  Pointer to the transmission control block
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_SM_entry_FIN_WAIT_2
(
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr
);

#if (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON)

/** \brief Exit of ESTABLSIHED state
 *
 * This function stops the KEEP ALIVE timer.
 *
 * \param[in]  ctrlBlockPtr  Pointer to the transmission control block
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_SM_exit_ESTABLISHED
(
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr
);

#endif /* (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON) */

#if (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON)

/** \brief Handle ACK segment in state listen
 *
 * In State LISTEN. This function verifies the SYN cookie of an ACK and if it is valid
 * Retrieves a socket from the socket unit and sets this socket to ESTABLISHED.
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
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_rcvAck_LISTEN_ESTABLISHED
(
  P2VAR(TcpIp_SocketIdType, AUTOMATIC, TCPIP_APPL_DATA) socketIdPtr,
  P2VAR(TcpIp_Socket_TCP_Generic_PtrType, AUTOMATIC, TCPIP_APPL_DATA) socketPtrPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(TcpIp_SockAddrLocalType, AUTOMATIC, TCPIP_APPL_DATA) localAddrPtr,
  uint32 seqNum,
  uint32 ackNum,
  uint16 datalen,
  uint8 flags
);

/** \brief Creates initial sequence number - syn cookie
 *
 * This function creates initial sequence number - syn cookie
 *
 * \param[in]  localAddrPtr      Pointer to the structure of the local configuration
 * \param[in]  remoteAddrPtr     Pointer to the structure of the remote configuration
 * \param[out] seqNum            Pointer to the generated sequence number
 * \param[in]  isn               Initial sequence number
 * \param[in]  mss               Mss value
 * \param[out] timeSec           Time used to set the overflow time
 * \param[in]  socketId          Socket ID of the listen socket
 *
 * \retval E_OK        syn cookies creation was successful
 * \retval !E_OK       syn cookies creation wasn't successful
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_getCryptoInitialSeqNum
(
  P2CONST(TcpIp_SockAddrLocalType,AUTOMATIC,TCPIP_APPL_DATA) localAddrPtr,
  P2CONST(TcpIp_SockAddrType,AUTOMATIC,TCPIP_APPL_DATA) remoteAddrPtr,
  P2VAR(uint32,AUTOMATIC,TCPIP_APPL_DATA) seqNum,
  uint32 isn,
  uint16 mss,
  P2VAR(uint32,AUTOMATIC,TCPIP_APPL_DATA) timeSec,
  TcpIp_SocketIdType socketId
);

/** \brief Checks initial sequence number fortwo counter values - syn cookie
 *
 * This function checks initial sequence number - syn cookie
 *
 * \param[in]  localAddrPtr      Pointer to the structure of the local configuration
 * \param[in]  remoteAddrPtr     Pointer to the structure of the remote configuration
 * \param[in]  seqNum            Received sequence number
 * \param[in]  isn               Initial sequence number
 * \param[out] sendMssPtr        Mss value to be set in cloned socket
 * \param[in]  timeSec           Last overflow time
 * \param[in]  socketId          Socket ID of the listen socket
 *
 * \retval E_OK        syn cookies was successfully verified
 * \retval !E_OK       syn cookies wasn't successfully verified
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_checkCryptoInitialSeqNum
(
  P2CONST(TcpIp_SockAddrLocalType,AUTOMATIC,TCPIP_APPL_DATA) localAddrPtr,
  P2CONST(TcpIp_SockAddrType,AUTOMATIC,TCPIP_APPL_DATA) remoteAddrPtr,
  const uint32 seqNum,
  uint32 isn,
  P2VAR(uint16,AUTOMATIC,TCPIP_APPL_DATA) sendMssPtr,
  uint32 timeOverflow,
  TcpIp_SocketIdType socketId
);

/** \brief Generates initial sequence number - syn cookie
 *
 * This function generates initial sequence number - syn cookie
 *
 * \param[in]  localAddrPtr      Pointer to the structure of the local configuration
 * \param[in]  remoteAddrPtr     Pointer to the structure of the remote configuration
 * \param[in]  seqNum            Received sequence number
 * \param[in]  isn               Initial sequence number
 * \param[in]  counter           Value of the counter, number of 64s after 1970
 * \param[in,out] sendMssPtr     Mss value
 * \param[in]  timeSec           Current time
 * \param[in]  socketId          Socket ID of the listen socket
 *
 * \retval E_OK        syn cookies was successfully verified
 * \retval !E_OK       syn cookies wasn't successfully verified
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_InitialSeqNumCalculateHash
(
  P2CONST(TcpIp_SockAddrLocalType,AUTOMATIC,TCPIP_APPL_DATA) localAddrPtr,
  P2CONST(TcpIp_SockAddrType,AUTOMATIC,TCPIP_APPL_DATA) remoteAddrPtr,
  P2VAR(uint32,AUTOMATIC,TCPIP_APPL_DATA) seqNum,
  uint32 isn,
  const uint32 counter,
  P2VAR(uint16,AUTOMATIC,TCPIP_APPL_DATA) sendMssPtr,
  TcpIp_SocketIdType socketId
);

#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON) */

#if (TCPIP_TCP_UNPREDICTABLE_INIT_SEQ_NUM_ENABLED == STD_ON)

/** \brief Generates an unpredictable sequence number
 *
 * This function generates an unpredictable sequence number using cryptography
 *
 * \param[in]  socketPtr         Pointer to the socket
 * \param[in]  socketId          Socket ID
 * \param[in]  remoteAddrPtr     Pointer to the structure of the remote configuration
 *
 * \retval TCPIP_OK        sequence number was successfully generated
 * \retval !TCPIP_OK       sequence number failed to generate
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_generateUnpredictableIss
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  TcpIp_SocketIdType socketId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr
);

#endif /* (TCPIP_TCP_UNPREDICTABLE_INIT_SEQ_NUM_ENABLED == STD_ON) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[internal constants]====================================*/

#if(TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON)
#define TCPIP_START_SEC_CONST_16
#include <TcpIp_MemMap.h>

/** \brief Initial sequence number generator */
STATIC CONST(uint16, TCPIP_VAR) TcpIp_Tcp_mssTab[TCPIP_TCP_SYN_COOKIES_MSS_TABLE_SIZE] =
                                  {536U,600U,800U,1000U,1220U,1300U,1440U,1460U};

#define TCPIP_STOP_SEC_CONST_16
#include <TcpIp_MemMap.h>
#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON) */

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Tcp_abort
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  DBG_TCPIP_TCP_ABORT_ENTRY(socketId, socketPtr);

  switch(socketPtr->tcp.state)
  {
    case TCPIP_TCP_STATE_LISTEN:
      /* free socket */
      TcpIp_Tcp_SM_tr_immediateClose_LISTEN_CLOSED(socketId, socketPtr);
      break;
    case TCPIP_TCP_STATE_SYN_SENT:
      /* free socket */
      TcpIp_Tcp_SM_tr_immediateClose_SYN_SENT_CLOSED(socketId, socketPtr);
      break;
    case TCPIP_TCP_STATE_SYN_RECEIVED:
      /* sent RST segment, free socket */
      TcpIp_Tcp_SM_tr_immediateClose_SYN_RECEIVED_CLOSED(socketId, socketPtr);
      break;
    case TCPIP_TCP_STATE_ESTABLISHED:
      /* sent RST segment, free socket */
      TcpIp_Tcp_SM_tr_immediateClose_ESTABLISHED_CLOSED(socketId, socketPtr);
      break;
    case TCPIP_TCP_STATE_CLOSE_WAIT:
      /* sent RST segment, free socket */
      TcpIp_Tcp_SM_tr_immediateClose_CLOSE_WAIT_CLOSED(socketId, socketPtr);
      break;
    case TCPIP_TCP_STATE_FIN_WAIT_1:
      /* sent RST segment, free socket */
      TcpIp_Tcp_SM_tr_immediateClose_FIN_WAIT_1_CLOSED(socketId, socketPtr);
      break;
    case TCPIP_TCP_STATE_FIN_WAIT_2:
      /* sent RST segment, free socket */
      TcpIp_Tcp_SM_tr_immediateClose_FIN_WAIT_2_CLOSED(socketId, socketPtr);
      break;
    case TCPIP_TCP_STATE_TIME_WAIT:
      /* free socket */
      TcpIp_Tcp_SM_tr_immediateClose_TIME_WAIT_CLOSED(socketId, socketPtr);
      break;
    case TCPIP_TCP_STATE_LAST_ACK:
      /* free socket */
      TcpIp_Tcp_SM_tr_immediateClose_LAST_ACK_CLOSED(socketId, socketPtr);
      break;
    case TCPIP_TCP_STATE_CLOSING:
      /* free socket */
      TcpIp_Tcp_SM_tr_immediateClose_CLOSING_CLOSED(socketId, socketPtr);
      break;
    case TCPIP_TCP_STATE_CLOSED:
      /* report close to Soad (socket unused or bound) */
      TcpIp_Tcp_SM_tr_immediateClose_CLOSED_CLOSED(socketId, socketPtr);
      break;
    /* CHECK: NOPARSE */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  DBG_TCPIP_TCP_ABORT_EXIT(socketId, socketPtr);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Tcp_terminate
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  DBG_TCPIP_TCP_TERMINATE_ENTRY(socketId, socketPtr);

  switch(socketPtr->tcp.state)
  {
    case TCPIP_TCP_STATE_LISTEN:          /* intended fall through */
    case TCPIP_TCP_STATE_SYN_SENT:        /* intended fall through */
    case TCPIP_TCP_STATE_SYN_RECEIVED:    /* intended fall through */
    case TCPIP_TCP_STATE_ESTABLISHED:     /* intended fall through */
    case TCPIP_TCP_STATE_CLOSE_WAIT:      /* intended fall through */
    case TCPIP_TCP_STATE_FIN_WAIT_1:      /* intended fall through */
    case TCPIP_TCP_STATE_FIN_WAIT_2:      /* intended fall through */
    case TCPIP_TCP_STATE_TIME_WAIT:       /* intended fall through */
    case TCPIP_TCP_STATE_LAST_ACK:        /* intended fall through */
    case TCPIP_TCP_STATE_CLOSING:
      TcpIp_Tcp_SM_entry_CLOSED(socketId, socketPtr, TCPIP_TCP_CLOSED, FALSE);
      break;
    /* CHECK: NOPARSE */
    case TCPIP_TCP_STATE_CLOSED:          /* intended fall through */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  DBG_TCPIP_TCP_TERMINATE_EXIT(socketId, socketPtr);
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_connect
(
  TcpIp_SocketIdType SocketId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RemoteAddrPtr
)
{
  TcpIp_ReturnType retVal = TCPIP_E_NOT_OK;
  TcpIp_Socket_Generic_PtrType socketPtr;
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) tcpSocketPtr;

  socketPtr= TcpIp_Socket_getSocketPtr(SocketId);

  tcpSocketPtr =
    /* Deviation MISRAC2012-1 */
    (P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA)) socketPtr;

  switch(tcpSocketPtr->tcp.state)
  {
    case TCPIP_TCP_STATE_CLOSED:
      /* transmit SYN segment */
      retVal =
        TcpIp_Tcp_SM_tr_connect_CLOSED_SYN_SENT(SocketId, tcpSocketPtr, RemoteAddrPtr);
      break;
    case TCPIP_TCP_STATE_LISTEN:
      /* transmit SYN segment */
      retVal =
        TcpIp_Tcp_SM_tr_connect_LISTEN_SYN_SENT(SocketId, tcpSocketPtr, RemoteAddrPtr);
      break;
    case TCPIP_TCP_STATE_SYN_SENT:           /* intended fall through */
    case TCPIP_TCP_STATE_SYN_RECEIVED:       /* intended fall through */
    case TCPIP_TCP_STATE_ESTABLISHED:        /* intended fall through */
    case TCPIP_TCP_STATE_FIN_WAIT_1:         /* intended fall through */
    case TCPIP_TCP_STATE_FIN_WAIT_2:         /* intended fall through */
    case TCPIP_TCP_STATE_CLOSE_WAIT:         /* intended fall through */
    case TCPIP_TCP_STATE_CLOSING:            /* intended fall through */
    case TCPIP_TCP_STATE_LAST_ACK:           /* intended fall through */
    case TCPIP_TCP_STATE_TIME_WAIT:          /* intended fall through */
      /* connection establishment is not allowed in this state */
      retVal = TCPIP_E_NOT_PERMITTED;
      break;
    /* CHECK: NOPARSE */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      retVal = TCPIP_E_NOT_OK;
      break;
     /* CHECK: PARSE */
  }

  return retVal;
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_listen
(
  TcpIp_SocketIdType SocketId,
  uint16 MaxChannels
)
{
  TcpIp_ReturnType retVal;
  TcpIp_Socket_Generic_PtrType socketPtr;
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) tcpSocketPtr;

  socketPtr= TcpIp_Socket_getSocketPtr(SocketId);

  tcpSocketPtr =
     /* Deviation MISRAC2012-1 */
     (P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA)) socketPtr;

  switch(tcpSocketPtr->tcp.state)
  {
    case TCPIP_TCP_STATE_CLOSED:
      /* append socket to listen socket list */
      retVal =
        TcpIp_Tcp_SM_tr_listen_CLOSED_LISTEN(SocketId, &tcpSocketPtr->tcp, MaxChannels);
      break;
    case TCPIP_TCP_STATE_LISTEN:              /* intended fall through */
    case TCPIP_TCP_STATE_SYN_SENT:            /* intended fall through */
    case TCPIP_TCP_STATE_SYN_RECEIVED:        /* intended fall through */
    case TCPIP_TCP_STATE_ESTABLISHED:         /* intended fall through */
    case TCPIP_TCP_STATE_FIN_WAIT_1:          /* intended fall through */
    case TCPIP_TCP_STATE_FIN_WAIT_2:          /* intended fall through */
    case TCPIP_TCP_STATE_CLOSE_WAIT:          /* intended fall through */
    case TCPIP_TCP_STATE_CLOSING:             /* intended fall through */
    case TCPIP_TCP_STATE_LAST_ACK:            /* intended fall through */
    case TCPIP_TCP_STATE_TIME_WAIT:           /* intended fall through */
      /* connection is already establishment */
      retVal = TCPIP_E_NOT_PERMITTED;
      break;
    /* CHECK: NOPARSE */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      retVal = TCPIP_E_NOT_OK;
      break;
    /* CHECK: PARSE */
  }

  return retVal;
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_requestClose
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  TcpIp_ReturnType retVal;
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr =
    &socketPtr->tcp;

  switch(ctrlBlockPtr->state)
  {
    case TCPIP_TCP_STATE_SYN_SENT:           /* intended fall through */
    case TCPIP_TCP_STATE_SYN_RECEIVED:       /* intended fall through */
    case TCPIP_TCP_STATE_ESTABLISHED:        /* intended fall through */
    case TCPIP_TCP_STATE_CLOSE_WAIT:         /* intended fall through */
      ctrlBlockPtr->flags |= TCPIP_TCP_CLOSE_FLAG;
      ctrlBlockPtr->flags &= ~((uint8) TCPIP_TCP_TRANSMIT_FLAG);
      TcpIp_Tcp_stopRetry(ctrlBlockPtr);
      retVal = TCPIP_OK;
      break;
    case TCPIP_TCP_STATE_FIN_WAIT_1:       /* intended fall through */
    case TCPIP_TCP_STATE_FIN_WAIT_2:       /* intended fall through */
      retVal = TCPIP_OK;
      /* do nothing */
      break;
    case TCPIP_TCP_STATE_LAST_ACK:       /* intended fall through */
    case TCPIP_TCP_STATE_TIME_WAIT:      /* intended fall through */
    case TCPIP_TCP_STATE_CLOSING:        /* intended fall through */
      /* connection is already closing */
      retVal = TCPIP_E_NOT_PERMITTED;
      break;
    /* CHECK: NOPARSE */
    case TCPIP_TCP_STATE_CLOSED:  /* intended fall through */
    case TCPIP_TCP_STATE_LISTEN:  /* intended fall through */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      retVal = TCPIP_E_NOT_OK;
      break;
    /* CHECK: PARSE */
  }

  return retVal;
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_regularClose
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  TcpIp_ReturnType retVal;

  switch(socketPtr->tcp.state)
  {
    case TCPIP_TCP_STATE_SYN_SENT:
      /* free socket */
      TcpIp_Tcp_SM_tr_regularClose_SYN_SENT_CLOSED(socketId, socketPtr);
      retVal = TCPIP_OK;
      break;
    case TCPIP_TCP_STATE_ESTABLISHED:
      /* send FIN segment */
      retVal = TcpIp_Tcp_SM_tr_regularClose_ESTABLISHED_FIN_WAIT_1(socketPtr);
      break;
    case TCPIP_TCP_STATE_CLOSE_WAIT:
      /* send FIN segment */
      retVal = TcpIp_Tcp_SM_tr_regularClose_CLOSE_WAIT_LAST_ACK(socketPtr);
      break;
    case TCPIP_TCP_STATE_SYN_RECEIVED:
      /* do nothing, FIN will be transmitted when connection is ESTABLISHED */
      retVal = TCPIP_OK;
      break;
    /* CHECK: NOPARSE */
    case TCPIP_TCP_STATE_FIN_WAIT_1:       /* intended fall through */
    case TCPIP_TCP_STATE_FIN_WAIT_2:       /* intended fall through */
    case TCPIP_TCP_STATE_LAST_ACK:         /* intended fall through */
    case TCPIP_TCP_STATE_TIME_WAIT:        /* intended fall through */
    case TCPIP_TCP_STATE_CLOSING:          /* intended fall through */
    case TCPIP_TCP_STATE_CLOSED:           /* intended fall through */
    case TCPIP_TCP_STATE_LISTEN:           /* intended fall through */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      retVal = TCPIP_E_NOT_OK;
      break;
    /* CHECK: PARSE */
  }

  return retVal;
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_received
(
  TcpIp_SocketIdType SocketId,
  uint16 Length
)
{
  TcpIp_ReturnType retVal = TCPIP_E_NOT_OK;
  TcpIp_Socket_Generic_PtrType socketPtr;
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) tcpSocketPtr;

  socketPtr= TcpIp_Socket_getSocketPtr(SocketId);

  tcpSocketPtr =
    /* Deviation MISRAC2012-1 */
    (P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA)) socketPtr;

  switch(tcpSocketPtr->tcp.state)
    {
      case TCPIP_TCP_STATE_ESTABLISHED:
        /* update receive window */
        retVal =
          TcpIp_Tcp_SM_tr_received_ESTABLISHED_ESTABLISHED(tcpSocketPtr, Length);
        break;
      case TCPIP_TCP_STATE_FIN_WAIT_1:
        /* update receive window */
        retVal =
          TcpIp_Tcp_SM_tr_received_FIN_WAIT_1_FIN_WAIT_1(tcpSocketPtr, Length);
        break;
      case TCPIP_TCP_STATE_FIN_WAIT_2:
        /* update receive window */
        retVal =
          TcpIp_Tcp_SM_tr_received_FIN_WAIT_2_FIN_WAIT_2(tcpSocketPtr, Length);
        break;
      case TCPIP_TCP_STATE_LISTEN:             /* intended fall through */
      case TCPIP_TCP_STATE_SYN_SENT:           /* intended fall through */
      case TCPIP_TCP_STATE_SYN_RECEIVED:       /* intended fall through */
      case TCPIP_TCP_STATE_CLOSE_WAIT:         /* intended fall through */
      case TCPIP_TCP_STATE_CLOSED:             /* intended fall through */
      case TCPIP_TCP_STATE_LAST_ACK:           /* intended fall through */
      case TCPIP_TCP_STATE_TIME_WAIT:          /* intended fall through */
      case TCPIP_TCP_STATE_CLOSING:            /* intended fall through */
        /* not connected, data reception not possible */
        retVal = TCPIP_E_NOT_PERMITTED;
        break;
      /* CHECK: NOPARSE */
      default:
        /* should not happen */
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        retVal = TCPIP_E_NOT_OK;
        break;
      /* CHECK: PARSE */
    }

  return retVal;
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_bufferData
(
  TcpIp_SocketIdType socketId,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint32 AvailableLength,
  boolean ForceRetrieve
)
{
  TcpIp_ReturnType retVal;
  TcpIp_Socket_Generic_PtrType socketPtr;
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) tcpSocketPtr;
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr;

  socketPtr= TcpIp_Socket_getSocketPtr(socketId);

  tcpSocketPtr =
    /* Deviation MISRAC2012-1 */
    (P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA)) socketPtr;
  ctrlBlockPtr = &tcpSocketPtr->tcp;

  switch(ctrlBlockPtr->state)
  {
    case TCPIP_TCP_STATE_ESTABLISHED:         /* intended fall through */
    case TCPIP_TCP_STATE_CLOSE_WAIT:          /* intended fall through */
    {
      /* buffer data and mark transmission in next main function */
      retVal = TcpIp_Tcp_txData_buff
                 (socketId, tcpSocketPtr, DataPtr, AvailableLength, ForceRetrieve);

      /* delay transmission of data in mainfunction */
      if(!TCPIP_TCP_FLAG_CLOSE_IS_SET(ctrlBlockPtr->flags))
      {
        ctrlBlockPtr->flags |= TCPIP_TCP_TRANSMIT_FLAG;
        TcpIp_Tcp_stopRetry(ctrlBlockPtr);
      }
      break;
    }
    case TCPIP_TCP_STATE_LISTEN:               /* intended fall through */
    case TCPIP_TCP_STATE_SYN_SENT:             /* intended fall through */
    case TCPIP_TCP_STATE_SYN_RECEIVED:         /* intended fall through */
    case TCPIP_TCP_STATE_FIN_WAIT_1:           /* intended fall through */
    case TCPIP_TCP_STATE_FIN_WAIT_2:           /* intended fall through */
    case TCPIP_TCP_STATE_TIME_WAIT:            /* intended fall through */
    case TCPIP_TCP_STATE_CLOSING:              /* intended fall through */
    case TCPIP_TCP_STATE_LAST_ACK:             /* intended fall through */
    case TCPIP_TCP_STATE_CLOSED:
      /* no connection established */
      retVal = TCPIP_E_NOT_PERMITTED;
      break;
    /* CHECK: NOPARSE */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      retVal = TCPIP_E_NOT_OK;
      break;
    /* CHECK: PARSE */
  }

   return retVal;
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_rcvSynCheckErr
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  uint32 seqNum,
  uint32 ackNum,
  uint16 seglen,
  uint8 flags
)
{
  TcpIp_ReturnType retVal;
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr =
    &socketPtr->tcp;

  switch(ctrlBlockPtr->state)
  {
    case TCPIP_TCP_STATE_LISTEN:
    case TCPIP_TCP_STATE_SYN_SENT:
      /* cases will be handled after acknowledgement update */
      retVal = TCPIP_OK;
      break;
    case TCPIP_TCP_STATE_CLOSE_WAIT:
      /* send RST segment and free socket */
      retVal = TcpIp_Tcp_SM_tr_rcvSynCheckErr_CLOSE_WAIT_CLOSED
                 (socketId, socketPtr, ackNum, seqNum, seglen, flags);
      break;
    case TCPIP_TCP_STATE_SYN_RECEIVED:
    {
      /* check for SYN retransmissions */
      if((seqNum + seglen) != ctrlBlockPtr->recv_nxt)
      {
        /* send RST segment and free socket */
        retVal = TcpIp_Tcp_SM_tr_rcvSynCheckErr_SYN_RECEIVED_CLOSED
                   (socketId, socketPtr, ackNum, seqNum, seglen, flags);
      }
      else
      {
        retVal = TCPIP_OK;
      }
      break;
    }
    case TCPIP_TCP_STATE_FIN_WAIT_1:
      /* send RST segment and free socket */
      retVal = TcpIp_Tcp_SM_tr_rcvSynCheckErr_FIN_WAIT_1_CLOSED
                 (socketId, socketPtr, ackNum, seqNum, seglen, flags);
      break;
    case TCPIP_TCP_STATE_FIN_WAIT_2:
      /* send RST segment and free socket */
      retVal = TcpIp_Tcp_SM_tr_rcvSynCheckErr_FIN_WAIT_2_CLOSED
                 (socketId, socketPtr, ackNum, seqNum, seglen, flags);
      break;
    case TCPIP_TCP_STATE_TIME_WAIT:
      /* send RST segment and free socket */
      retVal = TcpIp_Tcp_SM_tr_rcvSynCheckErr_TIME_WAIT_CLOSED
                 (socketId, socketPtr, ackNum, seqNum, seglen, flags);
      break;
    case TCPIP_TCP_STATE_CLOSING:
      /* send RST segment and free socket */
      retVal = TcpIp_Tcp_SM_tr_rcvSynCheckErr_CLOSING_CLOSED
                 (socketId, socketPtr, ackNum, seqNum, seglen, flags);
      break;
    case TCPIP_TCP_STATE_LAST_ACK:
      /* send RST segment and free socket */
      retVal = TcpIp_Tcp_SM_tr_rcvSynCheckErr_LAST_ACK_CLOSED
                 (socketId, socketPtr, ackNum, seqNum, seglen, flags);
      break;
    case TCPIP_TCP_STATE_ESTABLISHED:
      /* send RST segment and free socket */
      retVal = TcpIp_Tcp_SM_tr_rcvSynCheckErr_ESTABLISHED_CLOSED
                 (socketId, socketPtr, ackNum, seqNum, seglen, flags);
      break;
    /* CHECK: NOPARSE */
    case TCPIP_TCP_STATE_CLOSED: /* intended fall through */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      retVal = TCPIP_E_NOT_OK;
      break;
    /* CHECK: PARSE */
  }

   return retVal;
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_rcvSyn
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(TcpIp_SockAddrLocalType, AUTOMATIC, TCPIP_APPL_DATA) localAddrPtr,
  uint32 seqNum,
  uint16 seglen,
  uint16 mss,
  uint8 flags
)
{
  TcpIp_ReturnType retVal;


  switch(socketPtr->tcp.state)
  {
    case TCPIP_TCP_STATE_LISTEN:
      /* send SYN,ACK segment */
      retVal = TcpIp_Tcp_SM_tr_rcvSyn_LISTEN_SYN_RECEIVED
                 (socketId, socketPtr, remoteAddrPtr, localAddrPtr, mss, seqNum, seglen);
      break;
    case TCPIP_TCP_STATE_SYN_SENT:
    {
      if(TCPIP_TCP_FLAG_ACK_IS_SET(flags))
      {
        /* send ACK segment */
        retVal = TcpIp_Tcp_SM_tr_rcvSyn_SYN_SENT_ESTABLISHED
                     (socketId, socketPtr, mss, seqNum, seglen);
      }
      else
      {
        /* send SYN,ACK segment */
        retVal =  TcpIp_Tcp_SM_tr_rcvSyn_SYN_SENT_SYN_RECEIVED
                    (socketPtr, mss, seqNum, seglen);
      }
      break;
    }
    case TCPIP_TCP_STATE_SYN_RECEIVED:
      /* duplicate received (retransmitted by remote host) */
      retVal = TCPIP_OK;
      break;
    /* CHECK: NOPARSE */
    /* these cases are already handled in TcpIp_Tcp_SM_rcvSynCheckErr */
    case TCPIP_TCP_STATE_CLOSE_WAIT:       /* intended fall through */
    case TCPIP_TCP_STATE_FIN_WAIT_1:       /* intended fall through */
    case TCPIP_TCP_STATE_FIN_WAIT_2:       /* intended fall through */
    case TCPIP_TCP_STATE_TIME_WAIT:        /* intended fall through */
    case TCPIP_TCP_STATE_CLOSING:          /* intended fall through */
    case TCPIP_TCP_STATE_LAST_ACK:         /* intended fall through */
    case TCPIP_TCP_STATE_ESTABLISHED:      /* intended fall through */
    case TCPIP_TCP_STATE_CLOSED:           /* intended fall through */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      retVal = TCPIP_E_NOT_OK;
      break;
    /* CHECK: PARSE */
  }

   return retVal;
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_rcvFinCheckErr
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  TcpIp_ReturnType retVal;

  switch(socketPtr->tcp.state)
  {
    case TCPIP_TCP_STATE_SYN_RECEIVED:      /* intended fall through */
    case TCPIP_TCP_STATE_ESTABLISHED:       /* intended fall through */
    case TCPIP_TCP_STATE_FIN_WAIT_1:        /* intended fall through */
    case TCPIP_TCP_STATE_FIN_WAIT_2:        /* intended fall through */
      /* cases will be handled after acknowledgement update */
      retVal = TCPIP_OK;
      break;
    case TCPIP_TCP_STATE_LISTEN:            /* intended fall through */
    case TCPIP_TCP_STATE_SYN_SENT:          /* intended fall through */
    case TCPIP_TCP_STATE_CLOSING:           /* intended fall through */
    case TCPIP_TCP_STATE_CLOSE_WAIT:        /* intended fall through */
    case TCPIP_TCP_STATE_LAST_ACK:          /* intended fall through */
    case TCPIP_TCP_STATE_TIME_WAIT:
      /* ignore */
      retVal = TCPIP_E_DROP;
      break;
    /* CHECK: NOPARSE */
    case TCPIP_TCP_STATE_CLOSED: /* intended fall through */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      retVal = TCPIP_E_NOT_OK;
      break;
    /* CHECK: PARSE */
  }

   return retVal;
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_rcvFin
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  uint32 seqNum
)
{
  TcpIp_ReturnType retVal;
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr =
    &socketPtr->tcp;

  switch(ctrlBlockPtr->state)
  {
    case TCPIP_TCP_STATE_ESTABLISHED:
      /* send ACK segment */
      retVal = TcpIp_Tcp_SM_tr_rcvFin_ESTABLISHED_CLOSE_WAIT(socketId, seqNum, socketPtr);
      break;
    case TCPIP_TCP_STATE_FIN_WAIT_1:
      /* send ACK segment */
      retVal = TcpIp_Tcp_SM_tr_rcvFin_FIN_WAIT_1_CLOSING(socketId, seqNum, socketPtr);
      break;
    case TCPIP_TCP_STATE_FIN_WAIT_2:
      /* send ACK segment */
      retVal = TcpIp_Tcp_SM_tr_rcvFin_FIN_WAIT_2_TIME_WAIT(socketId, seqNum, socketPtr);
      break;
    /* CHECK: NOPARSE */
    /* these cases are already handled in TcpIp_Tcp_SM_rcvFinCheckErr */
    case TCPIP_TCP_STATE_CLOSING:            /* intended fall through */
    case TCPIP_TCP_STATE_CLOSE_WAIT:         /* intended fall through */
    case TCPIP_TCP_STATE_LAST_ACK:           /* intended fall through */
    case TCPIP_TCP_STATE_TIME_WAIT:          /* intended fall through */
    case TCPIP_TCP_STATE_CLOSED:             /* intended fall through */
    case TCPIP_TCP_STATE_LISTEN:             /* intended fall through */
    case TCPIP_TCP_STATE_SYN_SENT:           /* intended fall through */
    case TCPIP_TCP_STATE_SYN_RECEIVED:       /* intended fall through */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      retVal = TCPIP_E_NOT_OK;
      break;
    /* CHECK: PARSE */
  }

   return retVal;
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_rcvRst
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  uint8 flags
)
{
  TcpIp_ReturnType retVal;

  switch(socketPtr->tcp.state)
  {
    case TCPIP_TCP_STATE_LISTEN:         /* intended fall through */
      /* ignore */
      retVal = TCPIP_E_DROP;
      break;
    case TCPIP_TCP_STATE_SYN_SENT:
      if(TCPIP_TCP_FLAG_ACK_IS_SET(flags))
      {
        /* free socket */
        retVal = TcpIp_Tcp_SM_tr_rcvRst_ANY_CLOSED(socketId, socketPtr);
        break;
      }
      else
      {
        /* ignore */
        retVal = TCPIP_E_DROP;
      }
      break;
    case TCPIP_TCP_STATE_TIME_WAIT:         /* intended fall through */
#if (TCPIP_TCP_TIME_WAIT_RX_RST_IGNORE_ENABLED == STD_OFF)
      /* free socket */
      retVal = TcpIp_Tcp_SM_tr_rcvRst_ANY_CLOSED(socketId, socketPtr);
#else /* (TCPIP_TCP_TIME_WAIT_RX_RST_IGNORE_ENABLED == STD_OFF) */
      /* ignore */
      retVal = TCPIP_E_DROP;
#endif /* (TCPIP_TCP_TIME_WAIT_RX_RST_IGNORE_ENABLED == STD_OFF) */
      break;
    case TCPIP_TCP_STATE_SYN_RECEIVED:       /* intended fall through */
    case TCPIP_TCP_STATE_ESTABLISHED:        /* intended fall through */
    case TCPIP_TCP_STATE_FIN_WAIT_1:         /* intended fall through */
    case TCPIP_TCP_STATE_FIN_WAIT_2:         /* intended fall through */
    case TCPIP_TCP_STATE_CLOSING:            /* intended fall through */
    case TCPIP_TCP_STATE_LAST_ACK:           /* intended fall through */
    case TCPIP_TCP_STATE_CLOSE_WAIT:         /* intended fall through */
      /* free socket */
      retVal = TcpIp_Tcp_SM_tr_rcvRst_ANY_CLOSED(socketId, socketPtr);
      break;
    /* CHECK: NOPARSE */
    case TCPIP_TCP_STATE_CLOSED: /* intended fall through */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      retVal = TCPIP_E_NOT_OK;
      break;
    /* CHECK: PARSE */
  }

   return retVal;
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_rcvAckCheckErr
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  TcpIp_ReturnType retVal;

  switch(socketPtr->tcp.state)
  {
    case TCPIP_TCP_STATE_SYN_RECEIVED:      /* intended fall through */
    case TCPIP_TCP_STATE_LAST_ACK:          /* intended fall through */
    case TCPIP_TCP_STATE_FIN_WAIT_1:        /* intended fall through */
    case TCPIP_TCP_STATE_CLOSING:           /* intended fall through */
    case TCPIP_TCP_STATE_ESTABLISHED:       /* intended fall through */
    case TCPIP_TCP_STATE_FIN_WAIT_2:        /* intended fall through */
    case TCPIP_TCP_STATE_CLOSE_WAIT:        /* intended fall through */
    case TCPIP_TCP_STATE_TIME_WAIT:
      /* nothing to be done here */
      retVal =  TCPIP_OK;
      break;
    case TCPIP_TCP_STATE_SYN_SENT:
      /* ack segment will not be accepted in this state */
      retVal = TCPIP_E_DROP;
      break;
#if(TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON)
    case TCPIP_TCP_STATE_LISTEN:
      /* nothing to be done here */
      retVal =  TCPIP_OK;
      break;
#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON) */
    /* CHECK: NOPARSE */
#if (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_OFF)
    case TCPIP_TCP_STATE_LISTEN:
#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_OFF) */
    case TCPIP_TCP_STATE_CLOSED:
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      retVal = TCPIP_E_NOT_OK;
    break;
    /* CHECK: PARSE */
  }

  return retVal;
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_rcvAck
(
  P2VAR(TcpIp_SocketIdType, AUTOMATIC, TCPIP_APPL_DATA) socketIdPtr,
  P2VAR(TcpIp_Socket_TCP_Generic_PtrType, AUTOMATIC, TCPIP_APPL_DATA) socketPtrPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(TcpIp_SockAddrLocalType, AUTOMATIC, TCPIP_APPL_DATA) localAddrPtr,
  uint32 seqNum,
  uint32 ackNum,
  uint16 seglen,
  uint8 flags
)
{
  TcpIp_ReturnType retVal;
  TcpIp_SocketIdType socketId = *socketIdPtr;
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr = *socketPtrPtr;
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr =
    &socketPtr->tcp;

#if (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_OFF)
  TS_PARAM_UNUSED(remoteAddrPtr);
  TS_PARAM_UNUSED(localAddrPtr);
  TS_PARAM_UNUSED(seqNum);
  TS_PARAM_UNUSED(seglen);
  TS_PARAM_UNUSED(flags);
#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_OFF) */

  switch(ctrlBlockPtr->state)
  {
    case TCPIP_TCP_STATE_SYN_RECEIVED:
    {
      /* inform SoAd */
      retVal = TcpIp_Tcp_SM_tr_rcvAck_SYN_RECEIVED_ESTABLISHED(socketId, socketPtr);
      break;
    }
    case TCPIP_TCP_STATE_LAST_ACK:
    {
      /* all data including the FIN need to be acknowledged by the remote host */
      if((ctrlBlockPtr->used_buff == 0U) && (ackNum == ctrlBlockPtr->send_nxt))
      {
        /* free socket */
        TcpIp_Tcp_SM_tr_rcvAck_LAST_ACK_CLOSED(socketId, socketPtr);
        retVal = TCPIP_E_DROP;
      }
      else
      {
        retVal = TCPIP_OK;
      }
      break;
    }
    case TCPIP_TCP_STATE_FIN_WAIT_1:
    {
      /* all data including the FIN need to be acknowledged by the remote host */
      if((ctrlBlockPtr->used_buff == 0U) && (ackNum == ctrlBlockPtr->send_nxt))
      {
        /* set state */
        TcpIp_Tcp_SM_tr_rcvAck_FIN_WAIT_1_FIN_WAIT_2(socketPtr);
      }
      retVal = TCPIP_OK;
      break;
    }
    case TCPIP_TCP_STATE_CLOSING:
    {
      /* all data including the FIN need to be acknowledged by the remote host */
      if((ctrlBlockPtr->used_buff == 0U) && (ackNum == ctrlBlockPtr->send_nxt))
      {
        /* set state */
        TcpIp_Tcp_SM_tr_rcvAck_CLOSING_TIME_WAIT(ctrlBlockPtr);
      }
      retVal = TCPIP_OK;
      break;
    }
    case TCPIP_TCP_STATE_ESTABLISHED:
#if (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON)
    {
      if(TCPIP_TCP_FLAG_KEEP_ALIVE_IS_SET(ctrlBlockPtr->flags))
      {
        /* if all data is acknowledged, start the keep alive timer */
        if((ctrlBlockPtr->used_buff == 0U) && (ackNum == ctrlBlockPtr->send_nxt))
        {
          TcpIp_Tcp_SM_tr_rcvAck_ESTABLISHED_ESTABLISHED(ctrlBlockPtr);
        }
      }
      retVal = TCPIP_OK;
      break;
    }
#endif /* (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON) */
    case TCPIP_TCP_STATE_FIN_WAIT_2:
    case TCPIP_TCP_STATE_CLOSE_WAIT:        /* intended fall through */
    case TCPIP_TCP_STATE_TIME_WAIT:         /* intended fall through */
    case TCPIP_TCP_STATE_SYN_SENT:
      /* nothing to be done here */
      retVal = TCPIP_OK;
      break;
#if (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON)
    case TCPIP_TCP_STATE_LISTEN:
      retVal =
        TcpIp_Tcp_SM_tr_rcvAck_LISTEN_ESTABLISHED
          (socketIdPtr, socketPtrPtr, remoteAddrPtr, localAddrPtr, seqNum, ackNum, seglen, flags);
      break;
#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON) */
    /* CHECK: NOPARSE */
#if (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_OFF)
    case TCPIP_TCP_STATE_LISTEN:
#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_OFF) */
    case TCPIP_TCP_STATE_CLOSED:
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      retVal = TCPIP_E_NOT_OK;
      break;
    /* CHECK: PARSE */
  }

   return retVal;
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_rcvData
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 dataLen,
  uint32 seqNum
)
{
  TcpIp_ReturnType retVal;


  switch(socketPtr->tcp.state)
  {
    case TCPIP_TCP_STATE_ESTABLISHED:
      /* pass data to Soad */
      retVal= TcpIp_Tcp_SM_tr_rcvData_ESTABLISHED_ESTABLISHED
                (socketId, socketPtr, dataPtr, dataLen, seqNum);
      break;
    case TCPIP_TCP_STATE_FIN_WAIT_1:
      /* pass data to Soad */
      retVal= TcpIp_Tcp_SM_tr_rcvData_FIN_WAIT_1_FIN_WAIT_1
                (socketId, socketPtr, dataPtr, dataLen, seqNum);
      break;
    case TCPIP_TCP_STATE_FIN_WAIT_2:
      /* pass data to Soad */
      retVal= TcpIp_Tcp_SM_tr_rcvData_FIN_WAIT_2_FIN_WAIT_2
                (socketId, socketPtr, dataPtr, dataLen, seqNum);
      break;
    case TCPIP_TCP_STATE_SYN_RECEIVED:        /* intended fall through */
    case TCPIP_TCP_STATE_LAST_ACK:            /* intended fall through */
    case TCPIP_TCP_STATE_CLOSING:             /* intended fall through */
    case TCPIP_TCP_STATE_CLOSE_WAIT:          /* intended fall through */
    case TCPIP_TCP_STATE_TIME_WAIT:           /* intended fall through */
    case TCPIP_TCP_STATE_SYN_SENT:            /* intended fall through */
      /* ignore */
      retVal = TCPIP_OK;
      break;
    /* CHECK: NOPARSE */
    case TCPIP_TCP_STATE_LISTEN:              /* intended fall through */
    case TCPIP_TCP_STATE_CLOSED:
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      retVal = TCPIP_E_NOT_OK;
      break;
    /* CHECK: PARSE */
  }

   return retVal;
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_checkSeqNum
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  uint32 seqNum,
  uint16 seglen,
  uint8 flags
)
{
  TcpIp_ReturnType retVal;
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr =
    &socketPtr->tcp;

  switch(ctrlBlockPtr->state)
  {
    case TCPIP_TCP_STATE_LISTEN:             /* intended fall through */
    case TCPIP_TCP_STATE_SYN_SENT:           /* intended fall through */
      /* no need to check sequence number in this state */
      retVal = TCPIP_OK;
      break;
    case TCPIP_TCP_STATE_ESTABLISHED:
      /* verify if sequence number is in the receive window */
      retVal = TcpIp_Tcp_SM_tr_checkSeqNum_ESTABLISHED_ESTABLISHED
                 (socketPtr, seglen, seqNum, flags);
      break;
    case TCPIP_TCP_STATE_FIN_WAIT_1:
      /* verify if sequence number is in the receive window */
      retVal = TcpIp_Tcp_SM_tr_checkSeqNum_FIN_WAIT_1_FIN_WAIT_1
                 (socketPtr, seglen, seqNum, flags);
      break;
    case TCPIP_TCP_STATE_FIN_WAIT_2:
      /* verify if sequence number is in the receive window */
      retVal = TcpIp_Tcp_SM_tr_checkSeqNum_FIN_WAIT_2_FIN_WAIT_2
                 (socketPtr, seglen, seqNum, flags);
      break;
    case TCPIP_TCP_STATE_SYN_RECEIVED:
    {
      /* check if SYN was already transmitted, if received sequence number is
         invalid we cannot send an ack because remote host does not know our sequence
         number, in this case do not check */
      if(TCPIP_TCP_IS_GREATER_THAN(ctrlBlockPtr->send_nxt, ctrlBlockPtr->send_una) ||
         TCPIP_TCP_FLAG_RST_IS_SET(flags)
        )
      {
        /* verify if sequence number is in the receive window */
        retVal = TcpIp_Tcp_SM_tr_checkSeqNum_SYN_RECEIVED_SYN_RECEIVED
                   (socketPtr, seglen, seqNum, flags);
      }
      else
      {
        retVal = TCPIP_OK;
      }
      break;
    }
    case TCPIP_TCP_STATE_LAST_ACK:
      /* verify if sequence number is in the receive window */
      retVal = TcpIp_Tcp_SM_tr_checkSeqNum_LAST_ACK_LAST_ACK
                 (socketPtr, seglen, seqNum, flags);
      break;
    case TCPIP_TCP_STATE_CLOSING:
      /* verify if sequence number is in the receive window */
      retVal = TcpIp_Tcp_SM_tr_checkSeqNum_CLOSING_CLOSING
                 (socketPtr, seglen, seqNum, flags);
      break;
    case TCPIP_TCP_STATE_CLOSE_WAIT:
      /* verify if sequence number is in the receive window */
      retVal = TcpIp_Tcp_SM_tr_checkSeqNum_CLOSE_WAIT_CLOSE_WAIT
                 (socketPtr, seglen, seqNum, flags);
      break;
    case TCPIP_TCP_STATE_TIME_WAIT:
      /* verify if sequence number is in the receive window */
      retVal = TcpIp_Tcp_SM_tr_checkSeqNum_TIME_WAIT_TIME_WAIT
                 (socketPtr, seglen, seqNum, flags);
      break;
    /* CHECK: NOPARSE */
    case TCPIP_TCP_STATE_CLOSED:  /* intended fall through */
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      retVal = TCPIP_E_NOT_OK;
      break;
    /* CHECK: PARSE */
  }

   return retVal;
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_checkAckNum
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  TcpIp_SockAddrLocalType localAddr,
  uint32 ackNumber,
  uint32 seqNumber,
  uint16 seglen,
  uint8 flags
)
{
  TcpIp_ReturnType retVal;

  switch(socketPtr->tcp.state)
  {
    case TCPIP_TCP_STATE_LISTEN:
      /* segment from old connection, transmit RST segment */
      retVal = TcpIp_Tcp_SM_tr_checkAckNum_LISTEN_LISTEN
                 (socketPtr, remoteAddrPtr, localAddr, ackNumber, seqNumber, seglen, flags);
      break;
    case TCPIP_TCP_STATE_SYN_RECEIVED:
      /* verify if segment acknowledge something sent */
      retVal = TcpIp_Tcp_SM_tr_checkAckNum_SYN_RECEIVED_SYN_RECEIVED
                 (socketPtr, ackNumber, flags);
      break;
    case TCPIP_TCP_STATE_SYN_SENT:
      /* verify if segment acknowledge something sent */
      retVal = TcpIp_Tcp_SM_tr_checkAckNum_SYN_SENT_SYN_SENT(socketPtr, ackNumber, flags);
      break;
    case TCPIP_TCP_STATE_FIN_WAIT_1:
      /* verify if segment acknowledge something sent */
      retVal = TcpIp_Tcp_SM_tr_checkAckNum_FIN_WAIT_1_FIN_WAIT_1(socketPtr, ackNumber);
      break;
    case TCPIP_TCP_STATE_FIN_WAIT_2:
      /* verify if segment acknowledge something sent */
      retVal = TcpIp_Tcp_SM_tr_checkAckNum_FIN_WAIT_2_FIN_WAIT_2(socketPtr, ackNumber);
      break;
    case TCPIP_TCP_STATE_CLOSE_WAIT:
      /* verify if segment acknowledge something sent */
      retVal = TcpIp_Tcp_SM_tr_checkAckNum_CLOSE_WAIT_CLOSE_WAIT(socketPtr, ackNumber);
      break;
    case TCPIP_TCP_STATE_CLOSING:
      /* verify if segment acknowledge something sent */
      retVal = TcpIp_Tcp_SM_tr_checkAckNum_CLOSING_CLOSING(socketPtr, ackNumber);
      break;
    case TCPIP_TCP_STATE_LAST_ACK:
      /* verify if segment acknowledge something sent */
      retVal = TcpIp_Tcp_SM_tr_checkAckNum_LAST_ACK_LAST_ACK(socketPtr, ackNumber);
      break;
    case TCPIP_TCP_STATE_TIME_WAIT:
      /* verify if segment acknowledge something sent */
      retVal = TcpIp_Tcp_SM_tr_checkAckNum_TIME_WAIT_TIME_WAIT(socketPtr, ackNumber);
      break;
    case TCPIP_TCP_STATE_ESTABLISHED:
      /* verify if segment acknowledge something sent */
      retVal = TcpIp_Tcp_SM_tr_checkAckNum_ESTABLISHED_ESTABLISHED(socketPtr, ackNumber);
      break;
    /* CHECK: NOPARSE */
    case TCPIP_TCP_STATE_CLOSED:
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      retVal = TCPIP_E_NOT_OK;
      break;
    /* CHECK: PARSE */
  }

   return retVal;
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_stateTimeout
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  TcpIp_ReturnType result;
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr =  &socketPtr->tcp;

  switch(ctrlBlockPtr->state)
  {
    case TCPIP_TCP_STATE_TIME_WAIT:
      /* state timeout reached, free socket */
      TcpIp_Tcp_SM_tr_stateTimeout_TIME_WAIT_CLOSED(socketId, socketPtr);
      result = TCPIP_E_DROP;
      break;
    case TCPIP_TCP_STATE_FIN_WAIT_2:
      /* state timeout reached, free socket */
      result = TcpIp_Tcp_SM_tr_stateTimeout_FIN_WAIT_2_CLOSED(socketId, socketPtr);
      break;
    case TCPIP_TCP_STATE_SYN_RECEIVED:
      /* state timeout reached, free socket */
      result = TcpIp_Tcp_SM_tr_stateTimeout_SYN_RECEIVED_CLOSED(socketId, socketPtr);
      break;
#if (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON)
    case TCPIP_TCP_STATE_ESTABLISHED:
    {
      TCPIP_PRECONDITION_ASSERT
        (TCPIP_TCP_FLAG_KEEP_ALIVE_IS_SET(ctrlBlockPtr->flags), DET_INTERNAL_API_ID);

      if(ctrlBlockPtr->num_rtx >= ctrlBlockPtr->keepAlive_probemax)
      {
        /* maximal number of alive probes reached, free socket */
        result = TcpIp_Tcp_SM_tr_stateTimeout_ESTABLISHED_CLOSED(socketId, socketPtr);
      }
      else
      {
        /* transmit keep alive probe */
        result = TcpIp_Tcp_SM_tr_stateTimeout_ESTABLISHED_ESTABLISHED(socketPtr);
      }
      break;
    }
#else /*(TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON) */
    /* CHECK: NOPARSE */
    case TCPIP_TCP_STATE_ESTABLISHED:         /* intended fall through */
#endif /* (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON) */
    case TCPIP_TCP_STATE_FIN_WAIT_1:          /* intended fall through */
    case TCPIP_TCP_STATE_LISTEN:              /* intended fall through */
    case TCPIP_TCP_STATE_SYN_SENT:            /* intended fall through */
    case TCPIP_TCP_STATE_LAST_ACK:            /* intended fall through */
    case TCPIP_TCP_STATE_CLOSING:             /* intended fall through */
    case TCPIP_TCP_STATE_CLOSE_WAIT:          /* intended fall through */
    case TCPIP_TCP_STATE_CLOSED:
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      result = TCPIP_E_NOT_OK;
      break;
    /* CHECK: PARSE */
  }

  return result;
}

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_rtxTimeout
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  TcpIp_ReturnType result;
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr =
      &socketPtr->tcp;

  TCPIP_PRECONDITION_ASSERT( TcpIp_PBcfgPtr != NULL_PTR, TCPIP_INTERNAL_API_ID );
  switch(ctrlBlockPtr->state)
  {
    case TCPIP_TCP_STATE_SYN_SENT:
    {
      if(ctrlBlockPtr->num_rtx >= TcpIp_PBcfgPtr->tcpConfig.maxSynReTx)
      {
        /* free socket */
        result = TcpIp_Tcp_SM_tr_rtxTimeout_SYN_SENT_CLOSED(socketId, socketPtr);
      }
      else
      {
        /* retransmit segment */
        result = TcpIp_Tcp_SM_tr_rtxTimeout_SYN_SENT_SYN_SENT(socketPtr);
      }
      break;
    }
    case TCPIP_TCP_STATE_ESTABLISHED:
    {
      if(ctrlBlockPtr->num_rtx >= TcpIp_PBcfgPtr->tcpConfig.maxReTx)
      {
        /* free socket */
        result = TcpIp_Tcp_SM_tr_rtxTimeout_ESTABLISHED_CLOSED(socketId, socketPtr);
      }
      else
      {
        /* retransmit segment */
        result = TcpIp_Tcp_SM_tr_rtxTimeout_ESTABLISHED_ESTABLISHED(socketPtr);
      }
      break;
    }
    case TCPIP_TCP_STATE_SYN_RECEIVED:
    {
      if(ctrlBlockPtr->num_rtx >= TcpIp_PBcfgPtr->tcpConfig.maxSynReTx)
      {
        /* free socket */
        result = TcpIp_Tcp_SM_tr_rtxTimeout_SYN_RECEIVED_CLOSED(socketId, socketPtr);
      }
      else
      {
        /* retransmit segment */
        result = TcpIp_Tcp_SM_tr_rtxTimeout_SYN_RECEIVED_SYN_RECEIVED(socketPtr);
      }
      break;
    }
    case TCPIP_TCP_STATE_FIN_WAIT_1:
    {
      if(ctrlBlockPtr->num_rtx >= TcpIp_PBcfgPtr->tcpConfig.maxReTx)
      {
        /* free socket */
        result = TcpIp_Tcp_SM_tr_rtxTimeout_FIN_WAIT_1_CLOSED(socketId, socketPtr);
      }
      else
      {
        /* retransmit segment */
        result = TcpIp_Tcp_SM_tr_rtxTimeout_FIN_WAIT_1_FIN_WAIT_1(socketPtr);
      }
      break;
    }
    case TCPIP_TCP_STATE_CLOSE_WAIT:
    {
      if(ctrlBlockPtr->num_rtx >= TcpIp_PBcfgPtr->tcpConfig.maxReTx)
      {
        /* free socket */
        result = TcpIp_Tcp_SM_tr_rtxTimeout_CLOSE_WAIT_CLOSED(socketId, socketPtr);
      }
      else
      {
        /* retransmit segment */
        result = TcpIp_Tcp_SM_tr_rtxTimeout_CLOSE_WAIT_CLOSE_WAIT(socketPtr);
      }
      break;
    }
    case TCPIP_TCP_STATE_LAST_ACK:
    {
      if(ctrlBlockPtr->num_rtx >= TcpIp_PBcfgPtr->tcpConfig.maxReTx)
      {
        /* free socket */
        result = TcpIp_Tcp_SM_tr_rtxTimeout_LAST_ACK_CLOSED(socketId, socketPtr);
      }
      else
      {
        /* retransmit segment */
        result = TcpIp_Tcp_SM_tr_rtxTimeout_LAST_ACK_LAST_ACK(socketPtr);
      }
      break;
    }
    case TCPIP_TCP_STATE_CLOSING:
    {
      if(ctrlBlockPtr->num_rtx >= TcpIp_PBcfgPtr->tcpConfig.maxReTx)
      {
        /* free socket */
        result = TcpIp_Tcp_SM_tr_rtxTimeout_CLOSING_CLOSED(socketId, socketPtr);
      }
      else
      {
        /* retransmit segment */
        result = TcpIp_Tcp_SM_tr_rtxTimeout_CLOSING_CLOSING(socketPtr);
      }
      break;
    }
    /* CHECK: NOPARSE */
    case TCPIP_TCP_STATE_TIME_WAIT:
    case TCPIP_TCP_STATE_LISTEN:              /* intended fall through */
    case TCPIP_TCP_STATE_FIN_WAIT_2:          /* intended fall through */
    case TCPIP_TCP_STATE_CLOSED:
    default:
      /* should not happen */
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      result = TCPIP_E_NOT_OK;
      break;
    /* CHECK: PARSE */
  }

  return result;
}

/*==================[internal function definitions]=========================*/

STATIC FUNC(uint16, TCPIP_CODE) TcpIp_Tcp_SM_evaluteSendMSS
(
  TcpIp_LocalAddrIdType localAddrId,
  uint16 maxPayload,
  uint16 sendMss
)
{
  uint16 minPayload = TcpIp_Ip_getMinimumPayload(localAddrId) - TCPIP_TCP_HEADER_SIZE;
  uint16 mss;

  if(sendMss < minPayload)
  {
    /* minimal supported segment size */
    mss = minPayload;
  }
  else if(sendMss > maxPayload)
  {
    /* maximal allowed segment size */
    mss = maxPayload;
  }
  else
  {
    mss = sendMss;
  }

  return mss;
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_connect_CLOSED_SYN_SENT
(
  TcpIp_SocketIdType SocketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RemoteAddrPtr
)
{
  TcpIp_ReturnType retVal = TCPIP_E_NOT_OK;

  /* exit code */

  /* transitional code */
  {
    P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr =
      &socketPtr->tcp;
    P2VAR(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameterPtr =
      &socketPtr->generic.aligned.ipParameters;
    TcpIp_SockAddrLocalType localAddr;

    /* specify remote address of socket and retrieve local address and port */
    retVal = TcpIp_Socket_autoBindTcp(SocketId, RemoteAddrPtr, &localAddr);

    /* connection may already exist */
    if(retVal == TCPIP_OK)
    {
      /* get maximal segment length and check security policy */
      Std_ReturnType result = TcpIp_Tcp_getTransmissionParam(localAddr.addrId,
                                                             RemoteAddrPtr,
                                                             TCPIP_IP_PROTOCOL_TCP,
                                                             localAddr.port,
                                                             ipParameterPtr
                                                            );

      retVal = TCPIP_E_NOT_OK;
      if(result == E_OK)
      {
        socketPtr->tcp.recv_mss =
          socketPtr->generic.aligned.ipParameters.maxIpPayloadLength - TCPIP_TCP_HEADER_SIZE;
#if (TCPIP_TCP_UNPREDICTABLE_INIT_SEQ_NUM_ENABLED == STD_ON)
        retVal = TcpIp_Tcp_generateUnpredictableIss(socketPtr,SocketId,RemoteAddrPtr);
        if(retVal == TCPIP_OK)
#endif /* (TCPIP_TCP_UNPREDICTABLE_INIT_SEQ_NUM_ENABLED == STD_ON) */
        {
          /* activate the socket */
          TcpIp_Socket_activateTcp(SocketId, RemoteAddrPtr, &localAddr);

          /* send SYN (without ACK) */
          retVal = TcpIp_Tcp_txData_sendSyn(socketPtr, FALSE);
          ctrlBlockPtr->state = TCPIP_TCP_STATE_SYN_SENT;

          if(retVal != TCPIP_OK)
          {
            /* segment could not be sent try in next mainfunction */
            TcpIp_Tcp_startRetry(ctrlBlockPtr);
            retVal = TCPIP_E_DROP;
          }
        }
      }
    }
  }

  /* entry code */

  return retVal;
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_listen_CLOSED_LISTEN
(
  TcpIp_SocketIdType SocketId,
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr,
  uint16 MaxChannels
)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  return TcpIp_Tcp_SM_entry_LISTEN(SocketId, ctrlBlockPtr, MaxChannels);
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_SM_tr_regularClose_SYN_SENT_CLOSED
(
  TcpIp_SocketIdType SocketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  /* exit code */

  /* transitional code */
  {
    /* remove close request */
    socketPtr->tcp.flags &= ~((uint8) TCPIP_TCP_CLOSE_FLAG);
  }

  /* entry code */
  TcpIp_Tcp_SM_entry_CLOSED(SocketId, socketPtr, TCPIP_TCP_CLOSED, TRUE);
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_regularClose_ESTABLISHED_FIN_WAIT_1
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  TcpIp_ReturnType result;

  /* exit code */
#if (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON)
  TcpIp_Tcp_SM_exit_ESTABLISHED(&socketPtr->tcp);
#endif /* (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON) */

  /* transitional code */
  result = TcpIp_Tcp_SM_tr_regularClose(socketPtr, TCPIP_TCP_STATE_FIN_WAIT_1);

  /* entry code */

  return result;
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_regularClose_CLOSE_WAIT_LAST_ACK
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  TcpIp_ReturnType result;

  /* exit code */

  /* transitional code */
  result = TcpIp_Tcp_SM_tr_regularClose(socketPtr, TCPIP_TCP_STATE_LAST_ACK);

  /* entry code */

  return result;
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_regularClose
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  TcpIp_TcpStateType state
)
{
  TcpIp_ReturnType result;
  const TcpIp_LocalAddrIdType localAddrId = socketPtr->generic.aligned.local.addrId;

  if(TcpIp_IpAddrM_lockIfOnlineShutdown(localAddrId))
  {
    P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr =
          &socketPtr->tcp;

    ctrlBlockPtr->state = state;
    /* remove close request */
    ctrlBlockPtr->flags &= ~((uint8) TCPIP_TCP_CLOSE_FLAG);

    result = TcpIp_Tcp_txData_sendFin(socketPtr);

    if(result != TCPIP_OK)
    {
      /* segment could not be sent try in next mainfunction */
      TcpIp_Tcp_startRetry(ctrlBlockPtr);
    }

    TcpIp_IpAddrM_unlockIf(localAddrId);
  }
  else
  {
    result = TCPIP_E_DROP;
  }

  return result;
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_received_ESTABLISHED_ESTABLISHED
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  uint16 Length
)
{
  TcpIp_ReturnType result;

  /* exit code */

  /* transitional code */
  /* increase the receive window. Note: receive window max 0xFFFF */
  if((socketPtr->tcp.recv_wnd + Length) > socketPtr->tcp.recv_wndmax)
  {
    result = TCPIP_E_MEMORY;
  }
  else
  {
    uint16 recv_wndold = socketPtr->tcp.recv_wnd;
    socketPtr->tcp.recv_wnd += (uint16)Length;


    if ((recv_wndold < socketPtr->tcp.send_mss) && (socketPtr->tcp.recv_wnd >= socketPtr->tcp.send_mss))
    {
      /* Send TCP WIN update in the next main function call */
      SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

      TcpIp_Tcp_startModAckTimer(&(socketPtr->tcp), 1U);

      SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();
    }

    result = TCPIP_OK;
  }

  /* entry code */

  return result;
}

/* Deviation MISRAC2012-2 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_SM_tr_immediateClose_LISTEN_CLOSED
(
  TcpIp_SocketIdType SocketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  TcpIp_Tcp_SM_entry_CLOSED(SocketId, socketPtr, TCPIP_TCP_CLOSED, FALSE);
}

/* Deviation MISRAC2012-2 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_SM_tr_immediateClose_SYN_RECEIVED_CLOSED
(
  TcpIp_SocketIdType SocketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  /* exit code */

  /* transitional code */
  TcpIp_Tcp_txData_sendRstAck
                        (
                         &socketPtr->generic.aligned.ipParameters,
                         &socketPtr->remote,
                         &socketPtr->generic.aligned.local,
                         socketPtr->tcp.send_nxt
                        );

  /* entry code */
  TcpIp_Tcp_SM_entry_CLOSED(SocketId, socketPtr, TCPIP_TCP_CLOSED, FALSE);
}

/* Deviation MISRAC2012-2 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_SM_tr_immediateClose_CLOSED_CLOSED
(
  TcpIp_SocketIdType SocketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  /* exit code */

  /* transitional code */
  {
    const uint8 localsockOwnerId = socketPtr->generic.aligned.sockOwnerId;

    TCPIP_PRECONDITION_ASSERT
        ((localsockOwnerId < TcpIp_NumConfSockOwner),TCPIP_INTERNAL_API_ID);

    /* inform Soad about close */
    TcpIp_TcpIpEventAPI[localsockOwnerId](SocketId, TCPIP_TCP_CLOSED);
  }

  /* entry code */
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_rcvSyn_SYN_SENT_SYN_RECEIVED
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  uint16 mss,
  uint32 seqNum,
  uint16 seglen
)
{
  TcpIp_ReturnType result;

  /* exit code */

  /* transitional code */
  {
    P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr =
        &socketPtr->tcp;

    ctrlBlockPtr->state = TCPIP_TCP_STATE_SYN_RECEIVED;
    /* update the maximum send segment size */
    ctrlBlockPtr->send_mss = mss;
    /* update next expected sequence number to receive */
    ctrlBlockPtr->recv_nxt = seqNum + seglen;
    ctrlBlockPtr->send_wl1 = seqNum;

    result = TcpIp_Tcp_txData_sendSyn(socketPtr, TRUE);

    if(result != TCPIP_OK)
    {
      /* segment could not be sent try in next mainfunction */
      TcpIp_Tcp_startRetry(ctrlBlockPtr);
    }
  }

  /* entry code */

  return result;
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_rcvSyn_LISTEN_SYN_RECEIVED
(
  TcpIp_SocketIdType listenSocketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) listenSocketPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(TcpIp_SockAddrLocalType, AUTOMATIC, TCPIP_APPL_DATA) localAddrPtr,
  uint16 mss,
  uint32 seqNum,
  uint16 seglen
)
{
  TcpIp_ReturnType result = TCPIP_E_NOT_OK;

  /* exit code */

  /* transitional code */
  P2VAR(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameterPtr =
     &listenSocketPtr->generic.aligned.ipParameters;

  /* get maximal segment length and check security policy */
  result = TcpIp_Tcp_getTransmissionParam(localAddrPtr->addrId,
                                          remoteAddrPtr,
                                          TCPIP_IP_PROTOCOL_TCP,
                                          localAddrPtr->port,
                                          ipParameterPtr
                                         );

  if(result == E_OK)
  {
    TcpIp_SocketIdType newSocketId = listenSocketId;
    P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) newSocketPtr = listenSocketPtr;

    /* try to make a clone of the listen socket */
    result =
      TcpIp_Socket_Tcp_cloneListenSocket(localAddrPtr, remoteAddrPtr, &newSocketPtr, &newSocketId, FALSE);

    if(result == TCPIP_E_NOT_OK)
    {
#if (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON)
      /* all sockets are established, reset the overflow timeout, it doesn't make sense
       * to listen for any more ACKs because socket cannot be cloned */
      listenSocketPtr->tcp.lastOverflowTime = 0U;
#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON) */

      /* all sockets are in use, send reset */
      TcpIp_Tcp_txData_sendRst
                           (
                            ipParameterPtr,
                            remoteAddrPtr,
                            localAddrPtr,
                            0U,
                            seqNum,
                            seglen,
                            0U
                           );
    }
    else
    {
      TcpIp_LocalAddrIdType localAddrId = localAddrPtr->addrId;
      /* update the maximum send segment size */
      uint16 recv_mss = ipParameterPtr->maxIpPayloadLength - TCPIP_TCP_HEADER_SIZE;
      uint16 send_mss = TcpIp_Tcp_SM_evaluteSendMSS(localAddrId, recv_mss, mss);
      uint32 iss;

#if (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_OFF)
      iss = TcpIp_Tcp_getInitialSeqNum();
#else /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON) */
      Std_ReturnType retVal = E_NOT_OK;
      uint32 time_syn_cookie;

      /* calculate the sequence number for SYN cookies */
      retVal = TcpIp_Tcp_getCryptoInitialSeqNum
                     (
                       localAddrPtr,
                       remoteAddrPtr,
                       &iss,
                       seqNum,
                       send_mss,
                       &time_syn_cookie,
                       listenSocketId
                     );

      if(retVal != E_OK)
      {
        /* mark connection as passive open */
        newSocketPtr->tcp.flags &= ~((uint8) TCPIP_TCP_ACTIVE_FLAG);

        /* if SYN cookie calculation failed, close the cloned socket again */
        TcpIp_Tcp_closeProcessedTcpSocket(newSocketId, newSocketPtr);
      }
      else
#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON) */
      {
#if (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON)
        if(result == TCPIP_OK)
#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON) */
        {
          /* mark connection as passive open */
          newSocketPtr->tcp.flags &= ~((uint8) TCPIP_TCP_ACTIVE_FLAG);
          newSocketPtr->tcp.state = TCPIP_TCP_STATE_SYN_RECEIVED;
          newSocketPtr->tcp.send_wl1 = seqNum;
          newSocketPtr->tcp.send_mss = send_mss;
          newSocketPtr->tcp.recv_nxt = seqNum + seglen;
          newSocketPtr->tcp.recv_mss = recv_mss;

          /* if unused socket is available reset overflow time */
#if (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON)
          listenSocketPtr->tcp.lastOverflowTime = 0U;
#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON) */
          newSocketPtr->tcp.send_nxt = iss;
          newSocketPtr->tcp.send_wl2 = iss;
          newSocketPtr->tcp.send_una = iss;

          result = TcpIp_Tcp_txData_sendSyn(newSocketPtr, TRUE);

          /* Start the timer when SYN ACK segment was sent for the first time and
           * the connection is passive */
          TcpIp_Tcp_startSynReceivedTimer(&newSocketPtr->tcp);

          if(result != TCPIP_OK)
          {
            /* segment could not be sent try in next mainfunction */
            TcpIp_Tcp_startRetry(&newSocketPtr->tcp);
          }
        }
#if (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON)
        else /* BUSY, all sockets are in use, but sockets can still be established */
        {
          /* When first overflow was encountered, set the time */
          if(listenSocketPtr->tcp.lastOverflowTime == 0U)
          {
            listenSocketPtr->tcp.lastOverflowTime = time_syn_cookie;
          }

          (void) TcpIp_Tcp_txData_sendTempSyn
                    (
                     ipParameterPtr,
                     remoteAddrPtr,
                     localAddrPtr,
                     iss,
                     seqNum + seglen,
                     listenSocketPtr->tcp.recv_wnd,
                     recv_mss
                    );
        }
#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON) */
      }
    }
  }

  /* entry code */

  return result;
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_rcvSyn_SYN_SENT_ESTABLISHED
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  uint16 mss,
  uint32 seqNum,
  uint16 seglen
)
{
  TcpIp_ReturnType result;


  /* exit code */

  /* transitional code */
  {
    const uint8 localsockOwnerId = socketPtr->generic.aligned.sockOwnerId;
    P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr =
      &socketPtr->tcp;
    TcpIp_LocalAddrIdType localAddrId = socketPtr->generic.aligned.local.addrId;

    ctrlBlockPtr->state = TCPIP_TCP_STATE_ESTABLISHED;
    /* update the maximum send segment size */
    ctrlBlockPtr->send_mss = TcpIp_Tcp_SM_evaluteSendMSS(localAddrId, ctrlBlockPtr->recv_mss, mss);
    /* update next expected sequence number to receive */
    ctrlBlockPtr->recv_nxt = seqNum + seglen;
    ctrlBlockPtr->send_wl1 = seqNum;

#if (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON)
    if(TCPIP_TCP_FLAG_KEEP_ALIVE_IS_SET(ctrlBlockPtr->flags))
    {
        TcpIp_Tcp_startFirstKeepAliveTimer(ctrlBlockPtr);
    }
#endif /* (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON) */


    TCPIP_PRECONDITION_ASSERT
        ((localsockOwnerId < TcpIp_NumConfSockOwner),TCPIP_INTERNAL_API_ID);

    /* inform Soad that connection was successfully established */
    TcpIp_TcpConnectedAPI[localsockOwnerId](socketId);

    result = TcpIp_Tcp_txData_sendAck(socketPtr);

    if(result != TCPIP_OK)
    {
      /* mark retransmission of ACK in case no hardware buffer is available */
      TcpIp_Tcp_startAckRetranTimer(ctrlBlockPtr);
    }
  }

  /* entry code */

  return result;
}

/* Deviation MISRAC2012-2 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_rcvSynCheckErr_ESTABLISHED_CLOSED
(
  TcpIp_SocketIdType SocketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  uint32 ackNum,
  uint32 seqNum,
  uint16 seglen,
  uint8 flags
)
{
  /* exit code */

  /* transitional code */
  {
    TcpIp_Tcp_txData_sendRst
                          (
                           &socketPtr->generic.aligned.ipParameters,
                           &socketPtr->remote,
                           &socketPtr->generic.aligned.local,
                           ackNum,
                           seqNum,
                           seglen,
                           flags
                          );
  }

  /* entry code */
  TcpIp_Tcp_SM_entry_CLOSED(SocketId, socketPtr, TCPIP_TCP_RESET, TRUE);

  return TCPIP_E_DROP;
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_rcvFin_ESTABLISHED_CLOSE_WAIT
(
  TcpIp_SocketIdType socketId,
  uint32 seqNum,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  /* exit code */
#if (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON)
  {
    P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr =
      &socketPtr->tcp;

    TcpIp_Tcp_SM_exit_ESTABLISHED(ctrlBlockPtr);
  }
#endif /* (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON) */

  /* transitional code */
  {
    return TcpIp_Tcp_SM_tr_rcvFin
             (socketId, TCPIP_TCP_STATE_CLOSE_WAIT, seqNum, socketPtr, TRUE);
  }
  /* entry code */
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_rcvFin_FIN_WAIT_2_TIME_WAIT
(
  TcpIp_SocketIdType socketId,
  uint32 seqNum,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  TcpIp_ReturnType result;
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr =
    &socketPtr->tcp;

  /* exit code */

  /* transitional code */
  {
    /* stop FIN-WAIT-2 timer */
    TcpIp_Tcp_stopStateTimer(ctrlBlockPtr);

    result = TcpIp_Tcp_SM_tr_rcvFin
               (socketId, TCPIP_TCP_STATE_TIME_WAIT, seqNum, socketPtr, FALSE);
  }

  /* entry code */
  TcpIp_Tcp_SM_entry_TIME_WAIT(ctrlBlockPtr);

  return result;
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_rcvFin_FIN_WAIT_1_CLOSING
(
  TcpIp_SocketIdType socketId,
  uint32 seqNum,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  TcpIp_ReturnType result;

  /* exit code */

  /* transitional code */

  /* entry code */
  result = TcpIp_Tcp_SM_entry_CLOSING(socketId, seqNum, socketPtr);

  return result;
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_rcvRst_ANY_CLOSED
(
  TcpIp_SocketIdType SocketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  TcpIp_Tcp_SM_entry_CLOSED(SocketId, socketPtr, TCPIP_TCP_RESET, TRUE);

  return TCPIP_E_DROP;
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_rcvAck_SYN_RECEIVED_ESTABLISHED
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  TcpIp_ReturnType retVal;

  /* exit code */

  /* transitional code */
  {
    P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr =
        &socketPtr->tcp;

    const uint8 ownerId = socketPtr->generic.aligned.sockOwnerId;

    ctrlBlockPtr->state = TCPIP_TCP_STATE_ESTABLISHED;

    if(TCPIP_TCP_FLAG_ACTIVE_IS_SET(ctrlBlockPtr->flags))
    {
      TCPIP_PRECONDITION_ASSERT
        ((ownerId < TcpIp_NumConfSockOwner),TCPIP_INTERNAL_API_ID);

      /* active open call, inform Soad about successful connection establishment */
      TcpIp_TcpConnectedAPI[ownerId](socketId);
      retVal = TCPIP_OK;

#if (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON)
      /* we entered ESTABLISHED, if there is no pending data to acknowledge (idle)
         start keep alive timer */
      if(TCPIP_TCP_FLAG_KEEP_ALIVE_IS_SET(ctrlBlockPtr->flags))
      {
        TcpIp_Tcp_startFirstKeepAliveTimer(ctrlBlockPtr);
      }
      else
#endif /* (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON) */
      {
        /* stop syn-received timeout */
        TcpIp_Tcp_stopStateTimer(ctrlBlockPtr);
      }
    }
    else
    {
      const uint16 listenSocket = TcpIp_Socket_getListenSocket(socketId);
      P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddr =
        (P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA)) &socketPtr->remote;

      TCPIP_PRECONDITION_ASSERT
        ((ownerId < TcpIp_NumConfSockOwner),TCPIP_INTERNAL_API_ID);

      if(listenSocket == TCPIP_SOCKETID_INVALID)
      {
        /* listen socket was closed, close clone */
        (void) TcpIp_Close(socketId, TRUE);
        retVal = TCPIP_E_DROP;
      }
      else if(TcpIp_TcpAcceptedAPI[ownerId](listenSocket, socketId, remoteAddr) != E_OK)
      {
        /* SoAd does not accept connection, close it */
        (void) TcpIp_Close(socketId, TRUE);
        retVal = TCPIP_E_DROP;
      }
      else
      {
        TcpIp_Socket_Tcp_establishConnection(socketId);

        /* upper layer knows connection, mark as active to report state */
        ctrlBlockPtr->flags |= TCPIP_TCP_ACTIVE_FLAG;
        retVal = TCPIP_OK;

#if (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON)
        /* we entered ESTABLISHED, if there is no pending data to acknowledge (idle)
           start keep alive timer */
        if(TCPIP_TCP_FLAG_KEEP_ALIVE_IS_SET(ctrlBlockPtr->flags))
        {
          TcpIp_Tcp_startFirstKeepAliveTimer(ctrlBlockPtr);
        }
        else
#endif /* (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON) */
        {
          /* stop syn-received timeout */
          TcpIp_Tcp_stopStateTimer(ctrlBlockPtr);
        }
      }
    }
  }

  /* entry code */

  return retVal;
}

#if (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON)

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_rcvAck_LISTEN_ESTABLISHED
(
  P2VAR(TcpIp_SocketIdType, AUTOMATIC, TCPIP_APPL_DATA) socketIdPtr,
  P2VAR(TcpIp_Socket_TCP_Generic_PtrType, AUTOMATIC, TCPIP_APPL_DATA) socketPtrPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(TcpIp_SockAddrLocalType, AUTOMATIC, TCPIP_APPL_DATA) localAddrPtr,
  uint32 seqNum,
  uint32 ackNum,
  uint16 datalen,
  uint8 flags
)
{
  TcpIp_ReturnType retVal = TCPIP_E_NOT_OK;
  TcpIp_SocketIdType listenSocketId = *socketIdPtr;
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) listenSocketPtr =
     *socketPtrPtr;
  P2VAR(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameterPtr =
     &listenSocketPtr->generic.aligned.ipParameters;

  /* exit code */
  TCPIP_PRECONDITION_ASSERT((flags == TCPIP_TCP_FLAG_ACK), TCPIP_INTERNAL_API_ID);

  /* transitional code */
  {
    /* verify if we are in syn cookies mode */
    TCPIP_PRECONDITION_ASSERT
      ((listenSocketPtr->tcp.lastOverflowTime != 0U), TCPIP_INTERNAL_API_ID);

    {
      /* get maximal segment length and check security policy */
      Std_ReturnType result = TcpIp_Tcp_getTransmissionParam(localAddrPtr->addrId,
                                                             remoteAddrPtr,
                                                             TCPIP_IP_PROTOCOL_TCP,
                                                             localAddrPtr->port,
                                                             ipParameterPtr
                                                            );

      if(result == E_OK)
      {
        boolean sendReset = FALSE;
        uint16 mssCalculated;

        /* check if ACK matches with a previously sent SYN/ACK */
        Std_ReturnType seqNumResult =
           TcpIp_Tcp_checkCryptoInitialSeqNum
                    (localAddrPtr,
                     remoteAddrPtr,
                     ackNum - 1U,
                     seqNum - 1U,
                     &mssCalculated,
                     listenSocketPtr->tcp.lastOverflowTime,
                     listenSocketId
                    );

        if(seqNumResult == E_OK)
        {
          /* try to get a free socket or a socket which is currently establishing a connection */
          retVal =
            TcpIp_Socket_Tcp_cloneListenSocket(localAddrPtr, remoteAddrPtr, socketPtrPtr, socketIdPtr, TRUE);

          if(retVal == TCPIP_OK)
          {
            TcpIp_SocketIdType newSocketId = *socketIdPtr;
            P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) newSocketPtr = *socketPtrPtr;
            uint8 ownerId = newSocketPtr->generic.aligned.sockOwnerId;

            /* set the socket to established */
            newSocketPtr->tcp.state = TCPIP_TCP_STATE_ESTABLISHED;
            newSocketPtr->tcp.send_mss = mssCalculated;
            newSocketPtr->tcp.recv_mss = ipParameterPtr->maxIpPayloadLength - TCPIP_TCP_HEADER_SIZE;
            newSocketPtr->tcp.recv_nxt = seqNum;
            newSocketPtr->tcp.send_nxt = ackNum;
            newSocketPtr->tcp.send_wl1 = seqNum;
            newSocketPtr->tcp.send_wl2 = ackNum;
            newSocketPtr->tcp.send_una = ackNum;

            /* inform Socket unit that connection is established */
            TcpIp_Socket_Tcp_establishConnection(newSocketId);

            if(TcpIp_TcpAcceptedAPI[ownerId](listenSocketId, newSocketId, remoteAddrPtr) != E_OK)
            {
              /* SoAd does not accept connection, close it */
              (void) TcpIp_Close(newSocketId, TRUE);
              retVal = TCPIP_E_DROP;
            }
            else
            {
              /* upper layer knows connection, mark as active to report state */
              newSocketPtr->tcp.flags |= TCPIP_TCP_ACTIVE_FLAG;

#if (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON)
              /* we entered ESTABLISHED, if there is no pending data to acknowledge (idle)
                 start keep alive timer */
              if(TCPIP_TCP_FLAG_KEEP_ALIVE_IS_SET(newSocketPtr->tcp.flags))
              {
                TcpIp_Tcp_startFirstKeepAliveTimer(&newSocketPtr->tcp);
              }
#endif /* (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON) */
            }
          }
          else
          {
            /* there is no additional connection allowed, either no free socket left
               or maximal number of established connections is already reached */
            /* reset overflow timeout it doesn't make sense to listen for any
               more ACKs because socket cannot be cloned */
            listenSocketPtr->tcp.lastOverflowTime = 0U;
            sendReset = TRUE;
          }
        }
        else
        {
          /* unexpected ACK receieved in state LISTEN, send reset */
          sendReset = TRUE;
        }

        if(sendReset)
        {
          /* send reset to indicate that connection cannot be established */
          TcpIp_Tcp_txData_sendRst
                               (
                                ipParameterPtr,
                                remoteAddrPtr,
                                localAddrPtr,
                                ackNum,
                                seqNum,
                                datalen,
                                flags
                               );
        }
      }
    }
  }

  /* entry code */

  return retVal;
}

#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON) */

STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_SM_tr_rcvAck_FIN_WAIT_1_FIN_WAIT_2
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  {
    P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr =
        &socketPtr->tcp;

    TcpIp_Tcp_SM_entry_FIN_WAIT_2(ctrlBlockPtr);
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_SM_tr_rcvAck_CLOSING_TIME_WAIT
(
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr
)
{
  /* exit code */

  /* transitional code */

  /* entry code */
  TcpIp_Tcp_SM_entry_TIME_WAIT(ctrlBlockPtr);
}

#if (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON)

STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_SM_tr_rcvAck_ESTABLISHED_ESTABLISHED
(
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr
)
{
  /* exit code */

  /* transitional code */
  TcpIp_Tcp_startFirstKeepAliveTimer(ctrlBlockPtr);

  /* entry code */
}

#endif /* (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON) */

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_rcvData_ESTABLISHED_ESTABLISHED
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 dataLen,
  uint32 sequenceNum
)
{
  TcpIp_ReturnType result = TCPIP_E_DROP;

  /* exit code */

  /* transitional code */
  {
    P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr =
      &socketPtr->tcp;
#if(TCPIP_TCP_OUTOFORDER_RECEPTION_ENABLED == STD_ON)
    if((TCPIP_TCP_IS_LOWER_OR_EQUAL(sequenceNum, ctrlBlockPtr->recv_nxt)) &&
       (ctrlBlockPtr->ringbuffer_info_index == TCPIP_TCP_RINGBUFFER_INFO_INDEX_INVALID))
#else
    if(TCPIP_TCP_IS_LOWER_OR_EQUAL(sequenceNum, ctrlBlockPtr->recv_nxt))
#endif /* (TCPIP_TCP_OUTOFORDER_RECEPTION_ENABLED == STD_ON) */
    {
      P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddr =
        (P2VAR(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA)) &socketPtr->remote;

      /* Deviation MISRAC2012-3 */
      uint16 dataOffset = (uint16)(sint32)(ctrlBlockPtr->recv_nxt - sequenceNum);

      /* data must contain at least one byte of new data */
      if(dataLen > dataOffset)
      {
        uint16 receivedDataLen = dataLen - dataOffset;

        if(receivedDataLen > ctrlBlockPtr->recv_wnd)
        {
          /* received data may be greater than available buffer, only accept
            as much data as we can handle */
          receivedDataLen = ctrlBlockPtr->recv_wnd;
        }

        /* update receive window */
        ctrlBlockPtr->recv_wnd -= receivedDataLen;
        /* update next expected sequence number from remote host */
        ctrlBlockPtr->recv_nxt += receivedDataLen;
        /* update receive data since last ACK */
        ctrlBlockPtr->num_unack += receivedDataLen;

        TCPIP_PRECONDITION_ASSERT
          ((socketPtr->generic.aligned.sockOwnerId < TcpIp_NumConfSockOwner),TCPIP_INTERNAL_API_ID);

        /* pass data along to Soad */
        TcpIp_RxIndicationAPI[socketPtr->generic.aligned.sockOwnerId]
          (socketId, remoteAddr, &dataPtr[dataOffset], receivedDataLen);

        if(ctrlBlockPtr->recv_mss  <= (ctrlBlockPtr->num_unack >> 1U))
        {

          /* if half of the data received since last ACK is greater than maximal segment size transmit ACK.
            In a stream of full-sized segments the TCP shall send an ACK for every second segment */
          result = TcpIp_Tcp_txData_sendAck(socketPtr);

          if(result != TCPIP_OK)
          {
            TCPIP_PRECONDITION_ASSERT( TcpIp_PBcfgPtr != NULL_PTR, TCPIP_INTERNAL_API_ID );
            SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

            /* mark retransmission of ACK in case no hardware buffer is available */
            TcpIp_Tcp_startAckRetranTimer(ctrlBlockPtr);

            SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();
          }
        }
        else
        {
          SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

          if(!TcpIp_Tcp_ackTimerRunning(ctrlBlockPtr))
          {
            /* if delayed ack is not pending, start timer for delay */
            TcpIp_Tcp_startAckTimer(ctrlBlockPtr);
          }

          SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();
        }
      }
      result = TCPIP_OK;
    }
    /* out-of-order */
#if(TCPIP_TCP_OUTOFORDER_RECEPTION_ENABLED == STD_ON)
    else
    {
      /* Process out-of-order reception */
      TcpIp_Tcp_rxOutOfOrder_Receive(
        ctrlBlockPtr, dataPtr, dataLen, sequenceNum, socketId, socketPtr);

      result = TcpIp_Tcp_txData_sendAck(socketPtr);

      if(result != TCPIP_OK)
      {
        SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

        /* mark retransmission of ACK in case no hardware buffer is available */
        TcpIp_Tcp_startAckRetranTimer(ctrlBlockPtr);

        SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();
      }

      if(result == TCPIP_OK)
      {
        /* if segment contains FIN, segment will not be further processed */
        result = TCPIP_E_DROP;
      }
    }
#endif /* (TCPIP_TCP_OUTOFORDER_RECEPTION_ENABLED == STD_ON) */
  }

  /* entry code */

  return result;
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_checkSeqNum_ESTABLISHED_ESTABLISHED
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  uint16 seglen,
  uint32 seqNumber,
  uint8 flags
)
{
  TcpIp_ReturnType result;

  /* exit code */

  /* transitional code */
  {
    boolean acceptable = FALSE;

    P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr =
            &socketPtr->tcp;

    if(ctrlBlockPtr->recv_wnd == 0U)
    {
      if(seglen == 0U)
      {
        if(seqNumber == ctrlBlockPtr->recv_nxt)
        {
          /* if segment length and receive window is 0, sequence number of received
             segment must be equal to next expected sequence number from remote host */
          acceptable = TRUE;
        }
      }
    }
    else
    {
      const uint32 lowerBound = ctrlBlockPtr->recv_nxt;
      const uint32 upperBound =  ctrlBlockPtr->recv_nxt + ctrlBlockPtr->recv_wnd;

      if(TCPIP_TCP_IS_LOWER_OR_EQUAL(lowerBound, seqNumber) &&
         TCPIP_TCP_IS_LOWER_THAN(seqNumber, upperBound)
        )
      {
        /* sequence number must be inside the receive window */
        acceptable = TRUE;
      }
      else if(seglen != 0U)
      {
        const uint32 segNumberEnd = ((seqNumber + seglen) - 1U);

        if(TCPIP_TCP_IS_LOWER_OR_EQUAL(lowerBound, segNumberEnd) &&
           TCPIP_TCP_IS_LOWER_THAN(segNumberEnd, upperBound)
          )
        {
          /* sequence number must be inside the receive window */
          acceptable = TRUE;
        }
        else
        {
          /* not acceptable */
        }
      }
      else
      {
        /* not acceptable */
      }
    }

    if(!acceptable)
    {
      boolean isFinRetransmission =
      (
        (
          (ctrlBlockPtr->state == TCPIP_TCP_STATE_CLOSE_WAIT) ||
          (ctrlBlockPtr->state == TCPIP_TCP_STATE_CLOSING) ||
          (ctrlBlockPtr->state == TCPIP_TCP_STATE_LAST_ACK) ||
          (ctrlBlockPtr->state == TCPIP_TCP_STATE_TIME_WAIT)
        ) &&
        (TCPIP_TCP_FLAG_FIN_IS_SET(flags)) &&
        ((seqNumber + seglen) == ctrlBlockPtr->recv_nxt)
      ) ? TRUE : FALSE;

      if
      (
        (
          (!TcpIp_Tcp_duplicateAcknowledgementTimerRunningSeqNum(ctrlBlockPtr)) ||
          (isFinRetransmission == TRUE)
        ) &&
        (!TCPIP_TCP_FLAG_RST_IS_SET(flags))
      )
      {
        /* only transmit ACK if received segment does not contain a RST */
        result = TcpIp_Tcp_txData_sendAck(socketPtr);

        /* Prepare duplicate Acknowledgement timer for out-of-window sequence number */
        TcpIp_Tcp_startDuplicateAcknowledgementTimerSeqNum(ctrlBlockPtr);

        /* Restart 2*MSL timer */
        if
        (
          (ctrlBlockPtr->state == TCPIP_TCP_STATE_TIME_WAIT) &&
          (isFinRetransmission == TRUE)
        )
        {
          TcpIp_Tcp_SM_entry_TIME_WAIT(ctrlBlockPtr);
        }

        if(result != TCPIP_OK)
        {
          /* mark retransmission of ACK in case no hardware buffer is available */
          TcpIp_Tcp_startAckRetranTimer(ctrlBlockPtr);
        }

        if(result == TCPIP_OK)
        {
          /* drop the segment */
          result = TCPIP_E_DROP;
        }
      }
      else
      {
        result = TCPIP_E_DROP;
      }
    }
    else
    {
      result = TCPIP_OK;
    }
  }

  /* entry code */

  return result;
}

/* Deviation MISRAC2012-2 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_checkAckNum_SYN_SENT_SYN_SENT
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  uint32 ackNumber,
  uint8 flags
)
{
  TcpIp_ReturnType result;

  /* exit code */

  /* transitional code */
  {
    P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr =
            &socketPtr->tcp;

    if(TCPIP_TCP_IS_LOWER_OR_EQUAL(ackNumber, ctrlBlockPtr->send_una) ||
       TCPIP_TCP_IS_GREATER_THAN(ackNumber, ctrlBlockPtr->send_nxt)
      )
    {
      /* ACK acknowledges something not yet sent -> discard */
      if(!TCPIP_TCP_FLAG_RST_IS_SET(flags))
      {
        /* only transmit RST if received segment does not contain a RST */
        TcpIp_Tcp_txData_sendRstAck
                                (
                                 &socketPtr->generic.aligned.ipParameters,
                                 &socketPtr->remote,
                                 &socketPtr->generic.aligned.local,
                                 ackNumber
                                );
      }

      /* drop the segment */
      result = TCPIP_E_DROP;
    }
    else
    {
      /* ack is acceptable */
      result = TCPIP_OK;
    }
  }

  /* entry code */

  return result;
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_checkAckNum_ESTABLISHED_ESTABLISHED
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  uint32 ackNumber
)
{
  TcpIp_ReturnType result;

  /* exit code */

  /* transitional code */
  {
    P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr =
            &socketPtr->tcp;

    if
    (
      (!TcpIp_Tcp_duplicateAcknowledgementTimerRunningAckNum(ctrlBlockPtr)) &&
      (TCPIP_TCP_IS_GREATER_THAN(ackNumber, ctrlBlockPtr->send_nxt))
    )
    {
      /* unexpected ack number, transmit ack */
      result = TcpIp_Tcp_txData_sendAck(socketPtr);

      /* Prepare duplicate Acknowledgement timer for out-of-window acknowledgement number */
      TcpIp_Tcp_startDuplicateAcknowledgementTimerAckNum(ctrlBlockPtr);

      if(result != TCPIP_OK)
      {
        /* mark retransmission of ACK in case no hardware buffer is available */
        TcpIp_Tcp_startAckRetranTimer(ctrlBlockPtr);
      }

      if(result == TCPIP_OK)
      {
        /* drop the segment */
        result = TCPIP_E_DROP;
      }
    }
    else
    {
      result = TCPIP_OK;
    }
  }

  /* entry code */

  return result;
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_checkAckNum_LISTEN_LISTEN
(
  P2CONST(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  TcpIp_SockAddrLocalType localAddr,
  uint32 ackNum,
  uint32 seqNum,
  uint16 seglen,
  uint8 flags
)
{
  /* exit code */
  TcpIp_ReturnType result = TCPIP_E_NOT_OK;

  /* transitional code */
  {
    /* do not perform ACK check when in SYN cookies mode */
#if(TCPIP_TCP_SYN_COOKIES_ENABLED == TRUE)
    if((socketPtr->tcp.lastOverflowTime != 0U) && (flags == TCPIP_TCP_FLAG_ACK))
    {
      /* when SYN cookies are active only acks are allowed, they are checked later */
      result = TCPIP_OK;
    }
    else
#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == TRUE) */
    {
      if(!TCPIP_TCP_FLAG_RST_IS_SET(flags))
      {
        TcpIp_Ip_IpParameterType ipParameter = socketPtr->generic.aligned.ipParameters;

        /* get maximal segment length and check security policy */
        result = TcpIp_Tcp_getTransmissionParam(localAddr.addrId,
                                                remoteAddrPtr,
                                                TCPIP_IP_PROTOCOL_TCP,
                                                localAddr.port,
                                                &ipParameter
                                               );

        if(result == TCPIP_OK)
        {
          /* ack are not expected in LISTEN state, transmit RST */
          TcpIp_Tcp_txData_sendRst
                               (
                                &ipParameter,
                                remoteAddrPtr,
                                &localAddr,
                                ackNum,
                                seqNum,
                                seglen,
                                flags
                               );
        }
      }

      result = TCPIP_E_DROP;
    }
  }
  /* entry code */

  return result;
}

#if (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON)

/* Deviation MISRAC2012-2 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_stateTimeout_ESTABLISHED_ESTABLISHED
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  TcpIp_ReturnType result;

  /* exit code */

  /* transitional code */
  {
    P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr =
      &socketPtr->tcp;

    ctrlBlockPtr->num_rtx++;
    TcpIp_Tcp_startKeepAliveTimer(ctrlBlockPtr);

    result = TcpIp_Tcp_txData_sendKeepAlive(socketPtr);
  }

  /* entry code */

  return result;
}

#endif /* (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON) */

/* Deviation MISRAC2012-2 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_rtxTimeout_ESTABLISHED_ESTABLISHED
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  TcpIp_ReturnType result;

  /* exit code */

  /* transitional code */
  {
    P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr =
      &socketPtr->tcp;

    result = TcpIp_Tcp_txData_retransmit(socketPtr);

    if(result != TCPIP_OK)
    {
      /* segment could not be sent try in next mainfunction */
      TcpIp_Tcp_startRetry(ctrlBlockPtr);
    }
  }

  /* entry code */

  return result;
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_entry_LISTEN
(
  TcpIp_SocketIdType SocketId,
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr,
  uint16 MaxChannels
)
{
  TcpIp_ReturnType result;

  /* attach socket to listen socket list */
  result = TcpIp_Socket_listenTcp(SocketId, MaxChannels);

  /* Note: A socket may not be bound, return TCPIP_E_NOT_OK */
  if(result == TCPIP_OK)
  {
    ctrlBlockPtr->state = TCPIP_TCP_STATE_LISTEN;
#if (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON)
    ctrlBlockPtr->lastOverflowTime = 0U;
#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON) */
  }

  return result;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_SM_entry_CLOSED
(
  TcpIp_SocketIdType SocketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  TcpIp_EventType event,
  boolean callSocket
)
{
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr = &socketPtr->tcp;

#if(TCPIP_TCP_OUTOFORDER_RECEPTION_ENABLED == STD_ON)
  {
    uint16 bufferIndex = ctrlBlockPtr->ringbuffer_info_index;
    if(bufferIndex != TCPIP_TCP_RINGBUFFER_INFO_INDEX_INVALID)
    {
      TcpIp_Tcp_rxOutOfOrder_reset(bufferIndex);
      ctrlBlockPtr->ringbuffer_info_index = TCPIP_TCP_RINGBUFFER_INFO_INDEX_INVALID;
    }
  }
#endif /* (TCPIP_TCP_OUTOFORDER_RECEPTION_ENABLED == STD_ON) */

  ctrlBlockPtr->state = TCPIP_TCP_STATE_CLOSED;

  if(ctrlBlockPtr->avail_buff != 0U)
  {
    /* if a buffer was previously allocated free it, e.g. receive rst during transmission */
    (void)TcpIp_Memory_decrease(ctrlBlockPtr->memory_idPtr, ctrlBlockPtr->avail_buff);
  }

  if(TCPIP_TCP_FLAG_ACTIVE_IS_SET(ctrlBlockPtr->flags))
  {
    const uint8 localsockOwnerId = socketPtr->generic.aligned.sockOwnerId;

    TCPIP_PRECONDITION_ASSERT
        ((localsockOwnerId < TcpIp_NumConfSockOwner),TCPIP_INTERNAL_API_ID);

    /* inform Soad about close */
    TcpIp_TcpIpEventAPI[localsockOwnerId](SocketId, event);
  }

  if(callSocket)
  {
    /* deallocate socket */
    TcpIp_Socket_closeTcp(SocketId);
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_SM_entry_TIME_WAIT
(
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr
)
{
  ctrlBlockPtr->state = TCPIP_TCP_STATE_TIME_WAIT;

  TcpIp_Tcp_start2MslTimer(ctrlBlockPtr);
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_entry_CLOSING
(
  TcpIp_SocketIdType socketId,
  uint32 seqNum,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  return TcpIp_Tcp_SM_tr_rcvFin(socketId, TCPIP_TCP_STATE_CLOSING, seqNum, socketPtr, FALSE);
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_SM_tr_rcvFin
(
  TcpIp_SocketIdType socketId,
  TcpIp_TcpStateType state,
  uint32 seqNum,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  boolean callSoad
)
{
  TcpIp_ReturnType result;
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr = &socketPtr->tcp;

  /* check if FIN is out of order, send an ACK with next expected sequence */
  if(seqNum == ctrlBlockPtr->recv_nxt)
  {
    ctrlBlockPtr->state = state;

    if((callSoad == TRUE) && (TCPIP_TCP_FLAG_ACTIVE_IS_SET(ctrlBlockPtr->flags)))
    {
      const uint8 localsockOwnerId = socketPtr->generic.aligned.sockOwnerId;

      TCPIP_PRECONDITION_ASSERT
        ((localsockOwnerId < TcpIp_NumConfSockOwner),TCPIP_INTERNAL_API_ID);

      /* inform Soad about reception of FIN */
      TcpIp_TcpIpEventAPI[localsockOwnerId](socketId, TCPIP_TCP_FIN_RECEIVED);
    }

    /* FIN has a length of one, update next expected sequence number */
    ctrlBlockPtr->recv_nxt++;
  }

  result =  TcpIp_Tcp_txData_sendAck(socketPtr);

  if(result != TCPIP_OK)
  {
    SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

    /* mark retransmission of ACK in case no hardware buffer is available */
    TcpIp_Tcp_startAckRetranTimer(ctrlBlockPtr);

    SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();
  }

  return result;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_SM_entry_FIN_WAIT_2
(
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr
)
{
  ctrlBlockPtr->state = TCPIP_TCP_STATE_FIN_WAIT_2;

  /* we entered FIN_WAIT_2, limit delay for reception of ACK */
  TcpIp_Tcp_startFinWait2Timer(ctrlBlockPtr);
}

#if (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON)

STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_SM_exit_ESTABLISHED
(
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr
)
{
  if(TCPIP_TCP_FLAG_KEEP_ALIVE_IS_SET(ctrlBlockPtr->flags))
  {
    TcpIp_Tcp_stopStateTimer(ctrlBlockPtr);
  }
}

#endif /* (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON) */

#if (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON)

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_getCryptoInitialSeqNum
(
  P2CONST(TcpIp_SockAddrLocalType,AUTOMATIC,TCPIP_APPL_DATA) localAddrPtr,
  P2CONST(TcpIp_SockAddrType,AUTOMATIC,TCPIP_APPL_DATA) remoteAddrPtr,
  P2VAR(uint32,AUTOMATIC,TCPIP_APPL_DATA) seqNum,
  uint32 isn,
  uint16 mss,
  P2VAR(uint32,AUTOMATIC,TCPIP_APPL_DATA) timeSec,
  TcpIp_SocketIdType socketId
)
{
  Std_ReturnType result = E_NOT_OK;
  StbM_TimeStampType timeStamp;
  StbM_UserDataType timeUserData;
  uint16 calcMss = mss;

  result = StbM_GetCurrentTime(TCPIP_TCP_SYN_COOKIES_TIMEBASE_ID, &timeStamp, &timeUserData);
  /* time should always be available */
  TCPIP_PRECONDITION_ASSERT((result == E_OK),TCPIP_INTERNAL_API_ID);
  {
    const uint32 counter = TCPIP_TCP_SYN_COOKIES_GET_COUNTER_FROM_TIME(timeStamp);
    *timeSec = timeStamp.seconds;

    /* set that keys for SYN cookies were used and to start a timer */
    TcpIp_Tcp_SYN_Cookies_setKeyRegenerateTimer();

    result = TcpIp_Tcp_InitialSeqNumCalculateHash
             (
               localAddrPtr,
               remoteAddrPtr,
               seqNum,
               isn,
               counter,
               &calcMss,
               socketId
             );
  }
  return result;
}

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_checkCryptoInitialSeqNum
(
  P2CONST(TcpIp_SockAddrLocalType,AUTOMATIC,TCPIP_APPL_DATA) localAddrPtr,
  P2CONST(TcpIp_SockAddrType,AUTOMATIC,TCPIP_APPL_DATA) remoteAddrPtr,
  const uint32 seqNum,
  uint32 isn,
  P2VAR(uint16,AUTOMATIC,TCPIP_APPL_DATA) sendMssPtr,
  uint32 timeOverflow,
  TcpIp_SocketIdType socketId

)
{
  Std_ReturnType result = E_NOT_OK;
  StbM_TimeStampType timeStamp;
  StbM_UserDataType timeUserData;

  result = StbM_GetCurrentTime(TCPIP_TCP_SYN_COOKIES_TIMEBASE_ID, &timeStamp, &timeUserData);
  /* time should always be available */
  TCPIP_PRECONDITION_ASSERT((result == E_OK),TCPIP_INTERNAL_API_ID);
  if((timeOverflow == 0U) || (timeStamp.seconds >= (timeOverflow + TCPIP_TCP_SYN_COOKIES_ACCEPT_ACK)))
  {
    uint32 counter = TCPIP_TCP_SYN_COOKIES_GET_COUNTER_FROM_TIME(timeStamp);
    uint8 i = 0U;
    /* Get mss value from the first 3 bits of the received sequence number */
    uint16 sendMssCalculated = TcpIp_Tcp_mssTab[seqNum >> 29U];
    result = E_NOT_OK;

    /* set that keys for SYN cookies were used and to start a timer */
    TcpIp_Tcp_SYN_Cookies_setKeyRegenerateTimer();

    for(i = 0U; (i < TCPIP_TCP_SYN_COOKIES_COUNTER_NUM) && (result != E_OK); i++)
    {
      uint32 seqNumCalculated;
      result = TcpIp_Tcp_InitialSeqNumCalculateHash
               (
                 localAddrPtr,
                 remoteAddrPtr,
                 &seqNumCalculated,
                 isn,
                 counter - i,
                 &sendMssCalculated,
                 socketId
               );
      if
      (!(
        (result == E_OK) &&
        (seqNumCalculated == seqNum)
      ))
      {
        result = E_NOT_OK;
      }
      else
      {
        *sendMssPtr = sendMssCalculated;
      }
    }
  }
  else
  {
    result = E_NOT_OK;
  }
  return result;
}

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_InitialSeqNumCalculateHash
(
  P2CONST(TcpIp_SockAddrLocalType,AUTOMATIC,TCPIP_APPL_DATA) localAddrPtr,
  P2CONST(TcpIp_SockAddrType,AUTOMATIC,TCPIP_APPL_DATA) remoteAddrPtr,
  P2VAR(uint32,AUTOMATIC,TCPIP_APPL_DATA) seqNum,
  uint32 isn,
  const uint32 counter,
  P2VAR(uint16,AUTOMATIC,TCPIP_APPL_DATA) sendMssPtr,
  TcpIp_SocketIdType socketId
)
{
  Std_ReturnType result = E_NOT_OK;

  /* size of two addresses, two ports and a counter */
  uint8 hashData[TCPIP_TCP_SYN_COOKIES_HASH_ARRAYS_SIZE];
  /* size of sequence number */
  uint32 isnLength = 4U;
  uint8 hash1[4U];
  /* pointer to data without counter */
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) hash1DataPtr = &hashData[sizeof(counter)];
  TcpIp_Crypto_FragmentType dataFragmentPtr;

  /* copy data to a single buffer and calculate the length */
  TcpIp_Socket_Tcp_setHashData
  (
    localAddrPtr,
    remoteAddrPtr,
    &hashData[sizeof(counter)],
    &dataFragmentPtr.fragDataLength,
    socketId
  );
  /* add counter */
  TS_MemCpy(hashData,&counter,sizeof(counter));

  /* first hash without counter */
  dataFragmentPtr.fragDataPtr = hash1DataPtr;
  result =  TcpIp_Crypto_MacGenerate
            (
              TCPIP_TCP_SYN_COOKIES_JOB_1,
              &dataFragmentPtr,
              hash1,
              &isnLength,
              1U
            );
  /* second hash with counter */
  dataFragmentPtr.fragDataLength += sizeof(counter);
  if(result == E_OK)
  {
    uint8 hash2[4U];
    dataFragmentPtr.fragDataPtr = hashData;
    result =  TcpIp_Crypto_MacGenerate
              (
                TCPIP_TCP_SYN_COOKIES_JOB_2,
                &dataFragmentPtr,
                hash2,
                &isnLength,
                1U
              );
    if(result == E_OK)
    {
      uint32 z = TCPIP_GET32(hash1,0U) + isn + ((counter & 0xffffffU) << 24U) + (TCPIP_GET32(hash2,0U) % (1U << 24U));
      uint8 i = 0U;
      while((i < TCPIP_TCP_SYN_COOKIES_MSS_TABLE_SIZE) && (TcpIp_Tcp_mssTab[i] <= *sendMssPtr))
      {
        i++;
      }
      /* decrement the loop incrementation after finding the correct value */
      TCPIP_PRECONDITION_ASSERT(i > 0U, DET_INTERNAL_API_ID);
      i--;
      /* mss should exist in the table */
      TCPIP_PRECONDITION_ASSERT(i < TCPIP_TCP_SYN_COOKIES_MSS_TABLE_SIZE, DET_INTERNAL_API_ID);
      {
        *seqNum = (((uint32)i & 0x7U) << 29U) | (z % ((uint32)1U << 29U));
        *sendMssPtr = TcpIp_Tcp_mssTab[i];
      }
    }
  }
  return result;
}
#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON) */

#if (TCPIP_TCP_UNPREDICTABLE_INIT_SEQ_NUM_ENABLED == STD_ON)

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_generateUnpredictableIss
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  TcpIp_SocketIdType socketId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr
)
{
  Std_ReturnType result = E_NOT_OK;
  TcpIp_ReturnType tcpIpResult = TCPIP_E_NOT_OK;

  uint8 input[TCPIP_TCP_UNPR_SEQ_NUM_MAX_SIZE];
  TcpIp_Crypto_FragmentType dataFragmentPtr = {NULL_PTR, 0U, 0U};
  uint32 output;
  uint32 outputLength = TCPIP_TCP_UNPR_SEQ_NUM_RESULT_SIZE;

  dataFragmentPtr.fragDataPtr = input;

  /* copy data to a single buffer and calculate the length */
  TcpIp_Socket_Tcp_setHashData
  (
    &socketPtr->generic.aligned.local,
    remoteAddrPtr,
    dataFragmentPtr.fragDataPtr,
    &dataFragmentPtr.fragDataLength,
    socketId
  );

  result =  TcpIp_Crypto_MacGenerate
            (
              TCPIP_TCP_UNPR_SEQ_NUM_JOBID,
              &dataFragmentPtr,
              (P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA))&output,
              &outputLength,
              1U
            );

  if(result == E_OK)
  {
    const uint32 iss = (socketPtr->tcp.send_nxt) + output;

    TCPIP_PRECONDITION_ASSERT
      (outputLength == TCPIP_TCP_UNPR_SEQ_NUM_RESULT_SIZE, DET_INTERNAL_API_ID);

    socketPtr->tcp.send_nxt = iss;
    socketPtr->tcp.send_una = iss;
    socketPtr->tcp.send_wl1 = iss;
    socketPtr->tcp.send_wl2 = iss;

    TcpIp_Tcp_unprSeqNumSetKeyRegenerateTimer();
    tcpIpResult = TCPIP_OK;
  }

  return tcpIpResult;
}
#endif /* (TCPIP_TCP_UNPREDICTABLE_INIT_SEQ_NUM_ENABLED == STD_ON) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#else
  TS_PREVENTEMPTYTRANSLATIONUNIT
#endif /* (TCPIP_TCP_ENABLED == STD_ON) */

/*==================[end of file]===========================================*/
