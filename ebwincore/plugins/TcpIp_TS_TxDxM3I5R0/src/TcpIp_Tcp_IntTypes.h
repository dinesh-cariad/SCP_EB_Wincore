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

#ifndef TCPIP_TCP_INTTYPES_H
#define TCPIP_TCP_INTTYPES_H

/*==================[inclusions]============================================*/

#include <TSAutosar.h>      /* basic AUTOSAR types */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/** \brief Type which specifies the current connection state of a socket */
typedef uint8 TcpIp_TcpStateType;

/** \brief This structure stores all data about one TCP connection. */
typedef struct
{
  P2VAR(void, AUTOMATIC, TCPIP_APPL_DATA) memory_idPtr;
  /**< index of memory buffer */
#if (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON)
  uint32 keepAlive_interval;
  /**< interval between keep alive probes */
  uint32 keepAlive_time;
  /**< time until first transmission of keep alive probe */
#endif /* (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON) */
  uint32 avail_buff;
  /**< total size of the allocated memory */
  uint32 recv_nxt;
  /**< next sequence number to receive */
  uint32 rtx_timer;
  /**< retransmission timer */
  uint32 send_nxt;
  /**< next sequence number to send */
  uint32 send_una;
  /**< oldest unacknowledged number */
  uint32 send_wl1;
  /**< segment sequence number used for last window update */
  uint32 send_wl2;
  /**< segment acknowledgement number used for last window update */
  uint32 state_timer;
  /**< state timeout timer */
  uint32 used_buff;
  /**< number of bytes stored in the allocated memory */
  uint32 dupackSeqNum_timer;
  /**< duplicate acknowledgement timer for out-of-window sequence number */
  uint32 dupackAckNum_timer;
  /**< duplicate acknowledgement timer for out-of-window acknowledgement number */
#if (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON)
  uint32 lastOverflowTime;
  /**< time when the last overflow of SYN queue has occurred */
#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON) */
  uint16 ack_timer;
  /**< delayed acknowledgement timer */
  uint16 curr_buff;
  /**< current re/transmitted data */
  uint16 dataToFree;
  /**< data in socket memory which is not needed anymore (used in case copying is ongoing) */
  uint16 recv_wnd;
  /**< actual receive windows */
  uint16 recv_wndmax;
  /**< maximum receive window (default) */
  uint16 num_unack;
  /**< data received since last ACK */
  uint16 send_buff;
  /**< data already sent */
  uint16 recv_mss;
  /**< receive maximum segment size */
  uint16 send_mss;
  /**< send maximum segment size */
  uint16 send_wnd;
  /**< send window */
#if (TCPIP_TCP_OUTOFORDER_RECEPTION_ENABLED == STD_ON)
  uint16 ringbuffer_info_index;
  /**< index of the corresponding ringbuffer for out-of-order reception */
#endif /* (TCPIP_TCP_OUTOFORDER_RECEPTION_ENABLED == STD_ON) */
  uint8 ctrl_flags;
  /**< queue for control flags */
  uint8 num_rtx;
  /**< number of retransmissions */
  uint8 num_retry;
  /**< number of segment transmission retries */
  TcpIp_TcpStateType state;
  /**< State of the TCP connection */
#if (TCPIP_TCP_OPTION_FILTER_ENABLE == STD_ON)
  uint8 activeOptionFilter;
  /**< indicates which filter is currently active */
#endif /* (TCPIP_TCP_OPTION_FILTER_ENABLE == STD_ON) */
#if (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON)
  uint8 keepAlive_probemax;
  /**< number of keep alive probes until connection is closed */
#endif /* (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON) */
  uint8 flags;
  /**<  Bit0: 1 - Re/transmission of MSS option
              0 - No transmission
        Bit1: 1 - Wait for confirmation
              0 - No confirmation
        Bit2: 1 - Pending close request
              0 - No request
        Bit3: 1 - Nagle activated
              0 - Nagle deactivated
        Bit4: 1 - active open
              0 - passive open
        Bit5: 1 - Pending transmission request
              0 - No request
        Bit6: 1 - keep alive activated
              0 - keep alive deactivated
        Bit7: 1 - retry transmission in next mainfunction
              0 - no retry requested */
  boolean copyOngoing;
  /**< indicates if data is currently copied to socket memory */
  boolean zwpTransmitted;
  /**< indicates if zero-window probe was transmitted */
} TcpIp_TcpCtrlBlockType;

/* Pointer to structure of type TcpIp_TcpCtrlBlockType */
typedef P2VAR(TcpIp_TcpCtrlBlockType,TYPEDEF,TCPIP_APPL_DATA) TcpIp_TcpCtrlBlockPtrType;

#if (TCPIP_TCP_OUTOFORDER_RECEPTION_ENABLED == STD_ON)

/** \brief Structure to store the data required to manage the buffer for out-of-order segments */
typedef struct
{
  uint32 baseOffset;
  /**< Base offset of the buffer representing the sequence number of the data octet at index 0 */
  uint32 writeOffset;
  /**< Offset of the octet in the ring buffer that represents the start of the hole following
       the highest buffered segment */
} TcpIp_Tcp_RingBufferInfoType;

/** \brief Structure to store head indices for the used and unused holes lists */
typedef struct
{
  uint8 usedHoleListHeadIndex;
  /**< Index of the head element of the used elements in the hole list */
  uint8 freeHoleListHeadIndex;
  /**< Index of the head element of the unused elements in the hole list */
} TcpIp_Tcp_HoleListInfoType;

/** \brief Structure to store the data representing one entry of the linked list hole list */
typedef struct
{
  uint32 startOffset;
  /**< Offset of the ring buffer element corresponding to the first missing octet represented by this
       hole list entry */
  uint32 endOffset;
  /**< Offset of the ring buffer element corresponding to the last missing octet represented by this
       hole list entry */
  uint8 nextHoleIndex;
  /**< Index of the next element in the linked list of holes */
} TcpIp_Tcp_HoleListEntryType;

/** \brief Structure to store the data representine a hole list with multiple entries */
typedef struct
{
  TcpIp_Tcp_HoleListEntryType entries[TCPIP_TCP_HOLE_LIST_SIZE];
  /**< Array of list entries */
} TcpIp_Tcp_HoleListType;

/** \brief Structure to store the buffer data */
typedef struct
{
  uint8 data[TCPIP_TCP_RINGBUFFER_SIZE];
  /**< Array of buffer data */
} TcpIp_Tcp_RingBufferType;

#endif /* (TCPIP_TCP_OUTOFORDER_RECEPTION_ENABLED == STD_ON) */

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* TCPIP_TCP_INTTYPES_H */
