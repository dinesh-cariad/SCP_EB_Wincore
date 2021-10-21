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
 * 1)In order to retrieve the maximal receive window via API function TcpIp_ChangeParamter
 * it is required to convert a unit8 pointer to uint16.
 * 2)Sockets come in variants for UDP and TCP. To enable common handling, they must be cast
 * to the generic type from which they are derived as C does not have derived types.
 *
 */

/*==================[inclusions]============================================*/

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Memory_Int.h>   /* used SW-unit interface file */
#include <TcpIp_Tcp_Priv.h>     /* own SW-unit interface file */
#include <TcpIp_Socket_Int.h>   /* used SW-unit interface file */
#include <SchM_TcpIp.h>         /* SchM API declarations */
#include <TcpIp_Trace.h>        /* debug and trace macros; must be last */

#if (TCPIP_TCP_CRYPTO_ENABLED == STD_ON)
#include <TcpIp_Crypto.h>   /* used SW-unit interface file */
#endif /* (TCPIP_TCP_CRYPTO_ENABLED == STD_ON) */

/*==================[macros]================================================*/

/** \brief TcpIp_TcpConnect() service ID */
#define TCPIP_SID_TCPCONNECT                                    0x06U
/** \brief TcpIp_TcpListen() service ID */
#define TCPIP_SID_TCPLISTEN                                     0x07U
/** \brief TcpIp_TcpReceived() service ID */
#define TCPIP_SID_TCPRECEIVED                                   0x08U
/** \brief TcpIp_TcpTransmit() service ID */
#define TCPIP_SID_TCPTRANSMIT                                   0x13U

#if (TCPIP_TCP_OPTION_FILTER_ENABLE == STD_ON)
/** \brief Number of tcp option filters */
#define TCPIP_TCP_OPTION_FILTER_NUM  TcpIp_PBcfgPtr->tcpConfig.NumOfptionFilters
#endif /* (TCPIP_TCP_OPTION_FILTER_ENABLE == STD_ON) */

#if (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON)
/** \brief Parameters for SYN cookies */
#define TCPIP_TCP_SYN_COOKIES_KEY_1          TcpIp_PBcfgPtr->tcpConfig.synCookieskeyId1
#define TCPIP_TCP_SYN_COOKIES_KEY_2          TcpIp_PBcfgPtr->tcpConfig.synCookieskeyId2
#define TCPIP_TCP_SYN_COOKIES_RESET_KEYS     TcpIp_PBcfgPtr->tcpConfig.synCookiesTimeResetKeys
#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON) */

#if (TCPIP_TCP_UNPREDICTABLE_INIT_SEQ_NUM_ENABLED == STD_ON)
/** \brief Unpredictable initial sequence number key id */
#define TCPIP_TCP_UNPR_SEQ_NUM_KEY                TcpIp_PBcfgPtr->tcpConfig.unprSeqNumKeyId
/** \brief Initial time for unpredictable sequence number timer used to generate the key */
#define TCPIP_TCP_UNPR_SEQ_NUM_RESET_KEYS         TcpIp_PBcfgPtr->tcpConfig.unprSeqNumKeyResetTime
/** \brief Check if unpredictable sequence timer is running */
#define TCPIP_TCP_UNPR_SEQ_NUM_TIMER_RUNNING()    (TcpIp_Tcp_UnpSeqNumTimer > 0U)
/** \brief Check if unpredictable sequence timer is stopped */
#define TCPIP_TCP_UNPR_SEQ_NUM_TIMER_STOPPED()    (TcpIp_Tcp_UnpSeqNumTimer == 0U)
/** \brief Stop the unpredictable sequence timer  */
#define TCPIP_TCP_UNPR_SEQ_NUM_TIMER_STOP()       TcpIp_Tcp_UnpSeqNumTimer = 0U
/** \brief Start the unpredictable sequence timer  */
#define TCPIP_TCP_UNPR_SEQ_NUM_TIMER_START()      TcpIp_Tcp_UnpSeqNumTimer = TCPIP_TCP_UNPR_SEQ_NUM_RESET_KEYS
#endif /* (TCPIP_TCP_UNPREDICTABLE_INIT_SEQ_NUM_ENABLED == STD_ON) */

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#if (TCPIP_TCP_ENABLED == STD_ON)

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief Evaluate retransmission
 *
 * This function checks if a retransmission timeout occurred and triggers the
 * retransmission of a segment.
 *
 * \param[in]  socketId   id of the socket connection
 * \param[in]  socketPtr  Pointer to the connection information
 *
 * \retval TCPIP_OK        no timeout occurred, retransmission successful
 * \retval TCPIP_E_NOT_OK  error occurred during retransmission
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_retransmission
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Evaluate transmission of acknowledgement
 *
 * This function checks if a delayed acknowledgement is due to transmit or an
 * acknowledgement shall be retransmitted due to a previous memory lack.
 *
 * \param[in]  socketId   id of the socket connection
 * \param[in]  socketPtr  Pointer to the connection information
 *
 * \retval TCPIP_OK        no timeout occurred, transmission successful
 * \retval TCPIP_E_NOT_OK  error occurred during transmission
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_ackTimer
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

/** \brief Evaluate state timeout
 *
 * This function checks if socket state reached maximal dwell time. If dwell time is
 * reached the socket connection is closed.
 *
 * \param[in]  socketId   id of the socket connection
 * \param[in]  socketPtr  Pointer to the connection information
 *
 * \retval TCPIP_OK        no timeout occurred
 * \retval TCPIP_E_DROP    connection was closed
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_stateTimer
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
);

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )

/** \brief Evaluate new socket connection
 *
 * function to evaluate if another socket with the same port, protocol and local address
 * is in the state LISTEN
 *
 * \param[in]  socketId   id of the socket connection
 *
 * \retval E_OK            socket doesn't exist
 * \retval E_NOT_OK        socket already exists
 */

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_isAlreadyListen
(
  TcpIp_SocketIdType SocketId
);

#endif /* (TCPIP_DEV_ERROR_DETECT == STD_ON) */

#if (TCPIP_TCP_UNPREDICTABLE_INIT_SEQ_NUM_ENABLED == STD_ON)
/** \brief Generete new key for unpredictale sequence number
 *
 * This function generates a new cryptographic key used to
 * generate unpredictable initial sequence numbers when
 * opening an active connection.
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_generateIssKey(void);
#endif /* (TCPIP_TCP_UNPREDICTABLE_INIT_SEQ_NUM_ENABLED == STD_ON) */

/** \brief De/active feature
 *
 * This function de/activates a feature for a TCP socket by re/setting a flag.
 * e.g. Nagle, Keep Alive
 *
 * \param[in]  ctrlBlockPtr   Pointer to transmission control block
 * \param[in]  paramValue     0: turn off, 1: trun on
 * \param[in]  flag           Feature that shall be de/activated
 *
 * \retval E_OK      request successfully performed
 * \retval E_NOT_OK  invalid parameter value
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_changeParameterSetFlag
(
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr,
  uint8 paramValue,
  uint8 flag
);

/** \brief Sets maximal receive window
 *
 * This function sets the maximal receive window for the specified socket only
 * in state closed.
 *
 * \param[in]  ctrlBlockPtr   Pointer to transmission control block
 * \param[in]  paramValuePtr  New maximal receive window
 *
 * \retval E_OK      request successfully performed
 * \retval E_NOT_OK  socket is not closed, value is not 16bit aligned
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_changeParameterRcvWinMax
(
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) paramValuePtr
);

#if (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON)

/** \brief Extract uint32 value
 *
 * This function extracts a uint32 value from a uint8 array.
 *
 * \param[out] paramPtr       parameter that shall be set
 * \param[in]  paramValuePtr  new value
 *
 * \retval E_OK      request successfully performed
 * \retval E_NOT_OK  invalid parameter value
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_changeParameterSetUint32
(
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) paramPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) paramValuePtr
);

#endif /* (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#endif /* (TCPIP_TCP_ENABLED == STD_ON) */

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

#if (TCPIP_TCP_ENABLED == STD_ON)

#define TCPIP_START_SEC_VAR_INIT_32
#include <TcpIp_MemMap.h>


/** \brief Initial sequence number generator */
STATIC VAR(uint32, TCPIP_VAR) TcpIp_Tcp_InitalSeqNum = 0U;

#if (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON)

/** \brief Timeout for reseting the keys */
STATIC VAR(uint32, TCPIP_VAR) TcpIp_Tcp_SYN_Cookies_Reset_Timer = 0U;

#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON) */

#if (TCPIP_TCP_UNPREDICTABLE_INIT_SEQ_NUM_ENABLED == STD_ON)
/** \brief Timeout for reseting the key for unpredictable sequence numbers */
STATIC VAR(uint32, TCPIP_VAR) TcpIp_Tcp_UnpSeqNumTimer = 0U;
#endif /* (TCPIP_TCP_UNPREDICTABLE_INIT_SEQ_NUM_ENABLED == STD_ON) */

#define TCPIP_STOP_SEC_VAR_INIT_32
#include <TcpIp_MemMap.h>

#endif /* (TCPIP_TCP_ENABLED == STD_ON) */

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_TcpConnect
(
  TcpIp_SocketIdType SocketId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RemoteAddrPtr
)
{
  Std_ReturnType retVal = E_NOT_OK;

  DBG_TCPIP_TCPCONNECT_ENTRY(SocketId,RemoteAddrPtr);

#if (TCPIP_TCP_ENABLED == STD_ON)
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  if(TCPIP_UNINIT)
  {
    /* TCP not initialized */
    TCPIP_REPORT_ERROR(TCPIP_SID_TCPCONNECT, TCPIP_E_NOTINIT);
  }
  else if(RemoteAddrPtr == NULL_PTR)
  {
    /* no remote address specified */
    TCPIP_REPORT_ERROR(TCPIP_SID_TCPCONNECT, TCPIP_EB_E_PARAM_POINTER);
  }
  else if(TcpIp_Socket_checkSocketProtocol(SocketId,TCP) != E_OK)
  {
    /* No TCP socket */
    TCPIP_REPORT_ERROR(TCPIP_SID_TCPCONNECT, TCPIP_E_PROTOTYPE);
  }
  else if(TcpIp_Socket_checkSocketId(SocketId) != E_OK)
  {
    /* illegal socket state */
    TCPIP_REPORT_ERROR(TCPIP_SID_TCPCONNECT, TCPIP_E_INV_ARG);
  }
  else if(TCPIP_IP_IF_V4_V6_BOTH((RemoteAddrPtr->domain != TCPIP_AF_INET),
                                 (RemoteAddrPtr->domain != TCPIP_AF_INET6),
                                 (TcpIp_Socket_checkDomain(SocketId, RemoteAddrPtr->domain) != E_OK)
                                )
         )
  {
    /* domain not supported by socket */
    TCPIP_REPORT_ERROR(TCPIP_SID_TCPCONNECT, TCPIP_E_AFNOSUPPORT);
  }
  else if(TcpIp_Ip_isValidTcpRemoteAddress(RemoteAddrPtr) != TRUE)
  {
    /* invalid remote address */
    TCPIP_REPORT_ERROR(TCPIP_SID_TCPCONNECT, TCPIP_E_INV_SOCKADDR);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
    TcpIp_ReturnType result = TcpIp_Tcp_SM_connect(SocketId, RemoteAddrPtr);

#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
    if(result == TCPIP_E_NOT_PERMITTED)
    {
      /* connection already exists */
      TCPIP_REPORT_ERROR(TCPIP_SID_TCPCONNECT, TCPIP_E_ISCONN);
    }
    else if(result == TCPIP_E_ADDRNOTASSIGNED)
    {
      /* local address not available */
      TCPIP_REPORT_ERROR(TCPIP_SID_TCPCONNECT, TCPIP_E_ADDRNOTAVAIL);
    }
    else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
    {

      if(result == TCPIP_E_DROP || result == TCPIP_OK)
      {
        /* SYN transmitted or transmission delayed in next mainfunction */
        retVal = E_OK;
      }
      /* else Security policy check or ISN generation failed */

    }
  }
#else /* (TCPIP_TCP_ENABLED == STD_ON) */
  TS_PARAM_UNUSED(SocketId);
  TS_PARAM_UNUSED(RemoteAddrPtr);
#endif /* (TCPIP_TCP_ENABLED == STD_ON) */

  DBG_TCPIP_TCPCONNECT_EXIT(retVal,SocketId,RemoteAddrPtr);

  return retVal;
}


FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_TcpListen
(
  TcpIp_SocketIdType SocketId,
  uint16 MaxChannels
)
{
  Std_ReturnType retVal = E_NOT_OK;

  DBG_TCPIP_TCPLISTEN_ENTRY(SocketId,MaxChannels);

#if (TCPIP_TCP_ENABLED == STD_ON)
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  if(TCPIP_UNINIT)
  {
    /* TCP not initialized */
    TCPIP_REPORT_ERROR(TCPIP_SID_TCPLISTEN, TCPIP_E_NOTINIT);
  }
  else if(TcpIp_Socket_checkSocketProtocol(SocketId,TCP) != E_OK)
  {
    /* No TCP socket */
    TCPIP_REPORT_ERROR(TCPIP_SID_TCPLISTEN, TCPIP_E_PROTOTYPE);
  }
  else if(TcpIp_Socket_checkSocketId(SocketId) != E_OK)
  {
    /* illegal socket state */
    TCPIP_REPORT_ERROR(TCPIP_SID_TCPLISTEN, TCPIP_E_INV_ARG);
  }
  else if(TcpIp_Tcp_isAlreadyListen(SocketId) != E_OK)
  {
    /* socket already exists */
    TCPIP_REPORT_ERROR(TCPIP_SID_TCPLISTEN, TCPIP_E_ADDRINUSE);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
    TcpIp_ReturnType result;

    result = TcpIp_Tcp_SM_listen(SocketId, MaxChannels);

#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
    if(result == TCPIP_E_NOT_PERMITTED)
    {
      /* connection already exists */
      TCPIP_REPORT_ERROR(TCPIP_SID_TCPLISTEN, TCPIP_E_ISCONN);
    }
    else if(result == TCPIP_E_ADDRNOTASSIGNED)
    {
      /* connection already exists */
      TCPIP_REPORT_ERROR(TCPIP_SID_TCPLISTEN, TCPIP_E_ADDRNOTAVAIL);
    }
    else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
    {
      if(result == TCPIP_OK)
      {
        retVal = E_OK;
      }
    }
  }
#else /* (TCPIP_TCP_ENABLED == STD_ON) */
  TS_PARAM_UNUSED(SocketId);
  TS_PARAM_UNUSED(MaxChannels);
#endif /* (TCPIP_TCP_ENABLED == STD_ON) */

  DBG_TCPIP_TCPLISTEN_EXIT(retVal,SocketId,MaxChannels);

  return retVal;
}


FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_TcpReceived
(
  TcpIp_SocketIdType SocketId,
  uint32 Length
)
{
  Std_ReturnType retVal = E_NOT_OK;

  DBG_TCPIP_TCPRECEIVED_ENTRY(SocketId,Length);

#if (TCPIP_TCP_ENABLED == STD_ON)
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  if(TCPIP_UNINIT)
  {
    /* TCP not initialized */
    TCPIP_REPORT_ERROR(TCPIP_SID_TCPRECEIVED, TCPIP_E_NOTINIT);
  }
  else if(TcpIp_Socket_checkSocketProtocol(SocketId,TCP) != E_OK)
  {
    /* No TCP socket */
    TCPIP_REPORT_ERROR(TCPIP_SID_TCPRECEIVED, TCPIP_E_PROTOTYPE);
  }
  else if(TcpIp_Socket_checkSocketId(SocketId) != E_OK)
  {
    /* illegal socket state */
    TCPIP_REPORT_ERROR(TCPIP_SID_TCPRECEIVED, TCPIP_E_INV_ARG);
  }
  else if(Length > TCPIP_TCP_MAX_WINDOW_SIZE)
  {
    /* invalid length */
    TCPIP_REPORT_ERROR(TCPIP_SID_TCPRECEIVED, TCPIP_E_INV_ARG);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
    const TcpIp_ReturnType result =
#else /* TCPIP_DEV_ERROR_DETECT == STD_ON */
    (void)
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
    TcpIp_Tcp_SM_received(SocketId, (uint16)Length);

#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
    if(result == TCPIP_E_NOT_PERMITTED)
    {
      /* connection does not exist */
      TCPIP_REPORT_ERROR(TCPIP_SID_TCPRECEIVED, TCPIP_E_NOTCONN);
    }
    else if(result == TCPIP_E_MEMORY)
    {
      /* more memory shall be freed then allocated */
      TCPIP_REPORT_ERROR(TCPIP_SID_TCPRECEIVED, TCPIP_E_INV_ARG);
    }
    else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
    {
      retVal = E_OK;
    }
  }
#else /* (TCPIP_TCP_ENABLED == STD_ON) */
  TS_PARAM_UNUSED(SocketId);
  TS_PARAM_UNUSED(Length);
#endif /* (TCPIP_TCP_ENABLED == STD_ON) */

  DBG_TCPIP_TCPRECEIVED_EXIT(retVal,SocketId,Length);

  return retVal;
}


FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_TcpTransmit
(
  TcpIp_SocketIdType SocketId,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint32 AvailableLength,
  boolean ForceRetrieve
)
{
  Std_ReturnType retVal = E_NOT_OK;

  DBG_TCPIP_TCPTRANSMIT_ENTRY(SocketId,DataPtr,AvailableLength,ForceRetrieve);

#if (TCPIP_TCP_ENABLED == STD_ON)
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  if(TCPIP_UNINIT)
  {
    /* TCP not initialized */
    TCPIP_REPORT_ERROR(TCPIP_SID_TCPTRANSMIT, TCPIP_E_NOTINIT);
  }
  else if(TcpIp_Socket_checkSocketProtocol(SocketId,TCP) != E_OK)
  {
    /* No TCP socket */
    TCPIP_REPORT_ERROR(TCPIP_SID_TCPTRANSMIT, TCPIP_E_PROTOTYPE);
  }
  else if(TcpIp_Socket_checkSocketId(SocketId) != E_OK)
  {
    /* illegal socket state */
    TCPIP_REPORT_ERROR(TCPIP_SID_TCPTRANSMIT, TCPIP_E_INV_ARG);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
    if(AvailableLength == 0U)
    {
      /* no data was specified */
      retVal = E_OK;
    }
    else
    {
      TcpIp_ReturnType result =
        TcpIp_Tcp_SM_bufferData(SocketId, DataPtr, AvailableLength, ForceRetrieve);

      if(result == TCPIP_OK)
      {
        /* data successfully buffered */
        retVal = E_OK;
      }
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
      else if(result == TCPIP_E_NOT_PERMITTED)
      {
        /* connection does not exist */
        TCPIP_REPORT_ERROR(TCPIP_SID_TCPTRANSMIT, TCPIP_E_NOTCONN);
      }
      /* CHECK: NOPARSE */
      else if(result == TCPIP_E_MEMORY)
      {
        /* out of memory, data cannot be buffered */
        TCPIP_REPORT_ERROR(TCPIP_SID_TCPTRANSMIT, TCPIP_E_NOBUFS);
      }
      else
      {
        TCPIP_UNREACHABLE_CODE_ASSERT(TCPIP_SID_TCPTRANSMIT);
      }
      /* CHECK: PARSE */
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
    }
  }
#else /* (TCPIP_TCP_ENABLED == STD_ON) */
  TS_PARAM_UNUSED(SocketId);
  TS_PARAM_UNUSED(DataPtr);
  TS_PARAM_UNUSED(AvailableLength);
  TS_PARAM_UNUSED(ForceRetrieve);
#endif /* (TCPIP_TCP_ENABLED == STD_ON) */

  DBG_TCPIP_TCPTRANSMIT_EXIT(retVal,SocketId,DataPtr,AvailableLength,ForceRetrieve);

  return retVal;
}

#if (TCPIP_TCP_ENABLED == STD_ON)

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Tcp_changeSocketToDefault
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr = &socketPtr->tcp;
  const uint32 iss = TcpIp_Tcp_getInitialSeqNum();

  DBG_TCPIP_TCP_CHANGESOCKETTODEFAULT_ENTRY(socketPtr);

  TCPIP_PRECONDITION_ASSERT( TcpIp_PBcfgPtr != NULL_PTR, TCPIP_INTERNAL_API_ID );
  socketPtr->generic.aligned.ipParameters.ttl = TcpIp_PBcfgPtr->tcpConfig.ttl;
  socketPtr->generic.aligned.ipParameters.vlanPriority = TCPIP_IP_VLANPRIO_UNUSED;
  socketPtr->generic.aligned.ipParameters.useUnspecifiedSrc = FALSE;
  socketPtr->generic.aligned.ipParameters.mayFragment = FALSE;
  socketPtr->generic.aligned.ipParameters.flowLabel = 0U;
  socketPtr->generic.aligned.ipParameters.dscp = 0U;
  socketPtr->generic.aligned.ipParameters.calculateChecksum = TRUE;
  socketPtr->generic.aligned.ipParameters.protocol = TCPIP_IP_PROTOCOL_TCP;
  socketPtr->generic.aligned.ipParameters.calculatePseudoChecksum = TRUE;
  socketPtr->generic.aligned.ipParameters.identifier = 0U;

  ctrlBlockPtr->avail_buff = 0U;
  ctrlBlockPtr->used_buff = 0U;
  ctrlBlockPtr->dataToFree = 0U;
  ctrlBlockPtr->send_buff = 0U;
  ctrlBlockPtr->curr_buff = 0U;
  ctrlBlockPtr->copyOngoing = FALSE;
  ctrlBlockPtr->zwpTransmitted = FALSE;
  ctrlBlockPtr->flags = TCPIP_TCP_ACTIVE_FLAG;
  ctrlBlockPtr->ctrl_flags = TCPIP_TCP_FLAG_CLEAR;
  ctrlBlockPtr->memory_idPtr = TCPIP_MEMORY_IDPTR_INVALID;
  ctrlBlockPtr->recv_nxt = 0U;
  ctrlBlockPtr->num_retry = 0U;
  ctrlBlockPtr->recv_wnd = TcpIp_PBcfgPtr->tcpConfig.maxRxWindow;
  ctrlBlockPtr->recv_wndmax = TcpIp_PBcfgPtr->tcpConfig.maxRxWindow;
  ctrlBlockPtr->num_unack = 0U;
  ctrlBlockPtr->send_mss = 0U;
  ctrlBlockPtr->recv_mss = 0U;
  ctrlBlockPtr->send_nxt = iss;
  ctrlBlockPtr->send_una = iss;
  ctrlBlockPtr->send_wl1 = iss;
  ctrlBlockPtr->send_wl2 = iss;
  ctrlBlockPtr->send_wnd = TcpIp_PBcfgPtr->tcpConfig.maxRxWindow;
#if(TCPIP_TCP_OUTOFORDER_RECEPTION_ENABLED == STD_ON)
  ctrlBlockPtr->ringbuffer_info_index = TCPIP_TCP_RINGBUFFER_INFO_INDEX_INVALID;
#endif /* (TCPIP_TCP_OUTOFORDER_RECEPTION_ENABLED == STD_ON) */
#if (TCPIP_TCP_OPTION_FILTER_ENABLE == STD_ON)
  ctrlBlockPtr->activeOptionFilter = TCPIP_OPTION_FILTER_UNSET;
#endif /* (TCPIP_TCP_OPTION_FILTER_ENABLE == STD_ON) */
  ctrlBlockPtr->state = TCPIP_TCP_STATE_CLOSED;

  TcpIp_Tcp_stopRtxTimer(ctrlBlockPtr);
  TcpIp_Tcp_stopStateTimer(ctrlBlockPtr);
  TcpIp_Tcp_stopDuplicateAcknowledgementTimerSeqNum(ctrlBlockPtr);
  TcpIp_Tcp_stopDuplicateAcknowledgementTimerAckNum(ctrlBlockPtr);

  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  TcpIp_Tcp_stopAckTimer(ctrlBlockPtr);

  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  if(TcpIp_PBcfgPtr->tcpConfig.nagleDefault == 1U)
  {
    ctrlBlockPtr->flags |= TCPIP_TCP_NAGLE_FLAG;
  }

#if (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON)

  ctrlBlockPtr->keepAlive_interval = TcpIp_PBcfgPtr->tcpConfig.keepAliveInterval;
  ctrlBlockPtr->keepAlive_probemax = TcpIp_PBcfgPtr->tcpConfig.maxAliveProbes;
  ctrlBlockPtr->keepAlive_time = TcpIp_PBcfgPtr->tcpConfig.keepAliveTime;

  if(TcpIp_PBcfgPtr->tcpConfig.keepAliveDefault == 1U)
  {
    ctrlBlockPtr->flags |= TCPIP_TCP_KEEP_ALIVE_FLAG;
  }

#endif /* (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON) */

  DBG_TCPIP_TCP_CHANGESOCKETTODEFAULT_EXIT(socketPtr);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Tcp_processTcpSocket
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  TcpIp_ReturnType result;

  DBG_TCPIP_TCP_PROCESSTCPSOCKET_ENTRY(socketId, socketPtr);

  /* check for the transmission of delayed acks */
  result = TcpIp_Tcp_ackTimer(socketId, socketPtr);

  if(result == TCPIP_OK)
  {
    /* check for retransmission timeouts and transmission retries */
    result = TcpIp_Tcp_retransmission(socketId, socketPtr);

    if(result ==  TCPIP_OK)
    {
      /* check for state timeouts */
      (void) TcpIp_Tcp_stateTimer(socketId, socketPtr);
    }
  }

  DBG_TCPIP_TCP_PROCESSTCPSOCKET_EXIT(socketId, socketPtr);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Tcp_processTcpSocketTx
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr =
    &socketPtr->tcp;

  DBG_TCPIP_TCP_PROCESSTCPSOCKETTX_ENTRY(socketId,socketPtr);

  /* handle pending close requests */
  if(TCPIP_TCP_FLAG_CLOSE_IS_SET(ctrlBlockPtr->flags))
  {
    (void)TcpIp_Tcp_SM_regularClose(socketId, socketPtr);
  }
  /* handle pending transmission requests */
  else if(TCPIP_TCP_FLAG_TRANSMIT_IS_SET(ctrlBlockPtr->flags))
  {
    if(TcpIp_Tcp_txData_transmit(socketPtr) != TCPIP_OK)
    {
      /* segment could not be sent try in next mainfunction */
      TcpIp_Tcp_startRetry(ctrlBlockPtr);
    }
  }
  else
  {
    /* do nothing */
  }

  DBG_TCPIP_TCP_PROCESSTCPSOCKETTX_EXIT(socketId,socketPtr);

}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_close
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  boolean detReporting
)
{
  Std_ReturnType retVal = E_NOT_OK;
  TcpIp_ReturnType result;

  DBG_TCPIP_TCP_CLOSE_ENTRY(socketPtr, detReporting);
  /* transmit FIN */
  result = TcpIp_Tcp_SM_requestClose(socketPtr);

#if (TCPIP_DEV_ERROR_DETECT == STD_OFF)
  TS_PARAM_UNUSED(detReporting);
#else
  if((detReporting == TRUE) && (result == TCPIP_E_NOT_PERMITTED))
  {
    /* connection does not exist */
    TCPIP_REPORT_ERROR(TCPIP_SID_CLOSE, TCPIP_E_NOTCONN);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
    if(result == TCPIP_OK)
    {
      /* FIN transmitted or transmission delayed in next mainfunction */
      retVal = E_OK;
    }
  }

  DBG_TCPIP_TCP_CLOSE_EXIT(retVal, socketPtr, detReporting);

  return retVal;
}


TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Tcp_mainFunction
(
  void
)
{
  DBG_TCPIP_TCP_MAINFUNCTION_ENTRY();

  TCPIP_PRECONDITION_ASSERT( TcpIp_PBcfgPtr != NULL_PTR, TCPIP_INTERNAL_API_ID );

  /* periodically increase the initial sequence number generator */
  TcpIp_Tcp_InitalSeqNum += TcpIp_PBcfgPtr->tcpConfig.issPeriod;
#if (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON)
  /* keys need to be correctly generated even if there are no sockets in state LISTEN */
  if(TcpIp_Tcp_SYN_Cookies_Reset_Timer > 0U)
  {
    TcpIp_Tcp_SYN_Cookies_Reset_Timer--;
    if(TcpIp_Tcp_SYN_Cookies_Reset_Timer == 0U)
    {
#if (TCPIP_PRECONDITION_ASSERT_ENABLED == STD_ON)
      Std_ReturnType retVal = E_NOT_OK;
      /* keys should be correctly generated */
      retVal =
#else /* (TCPIP_PRECONDITION_ASSERT_ENABLED == STD_ON) */
      (void)
#endif /* (TCPIP_PRECONDITION_ASSERT_ENABLED == STD_ON) */
        TcpIp_Crypto_KeyGenerate(TCPIP_TCP_SYN_COOKIES_KEY_1);
      TCPIP_PRECONDITION_ASSERT((retVal == E_OK),TCPIP_INTERNAL_API_ID);
#if (TCPIP_PRECONDITION_ASSERT_ENABLED == STD_ON)
      /* keys should be correctly generated */
      retVal =
#else /* (TCPIP_PRECONDITION_ASSERT_ENABLED == STD_ON) */
      (void)
#endif /* (TCPIP_PRECONDITION_ASSERT_ENABLED == STD_ON) */
        TcpIp_Crypto_KeyGenerate(TCPIP_TCP_SYN_COOKIES_KEY_2);
      TCPIP_PRECONDITION_ASSERT((retVal == E_OK),TCPIP_INTERNAL_API_ID);
    }
  }
#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON) */
#if (TCPIP_TCP_UNPREDICTABLE_INIT_SEQ_NUM_ENABLED == STD_ON)
  /* If true, key was used and needs to be generated after timeout. Otherwise wait to be used */
  if(TCPIP_TCP_UNPR_SEQ_NUM_TIMER_RUNNING())
  {
    TcpIp_Tcp_UnpSeqNumTimer--;
    if(TCPIP_TCP_UNPR_SEQ_NUM_TIMER_STOPPED())
    {
      /* periodic key generate */
      TcpIp_Tcp_generateIssKey();
    }
  }
#endif /* (TCPIP_TCP_UNPREDICTABLE_INIT_SEQ_NUM_ENABLED == STD_ON) */

  DBG_TCPIP_TCP_MAINFUNCTION_EXIT();
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_changeParameter
(
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) paramValuePtr,
  TcpIp_ParamIdType paramId
)
{
  Std_ReturnType retVal = E_NOT_OK;
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr = &socketPtr->tcp;

  DBG_TCPIP_TCP_CHANGEPARAMETER_ENTRY(socketPtr, paramValuePtr, paramId);

#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  if((paramId != TCPIP_PARAMID_TCP_NAGLE) &&
     (ctrlBlockPtr->state != TCPIP_TCP_STATE_CLOSED)
    )
  {
    /* connection exists */
    TCPIP_REPORT_ERROR(TCPIP_SID_CHANGEPARAMETER, TCPIP_E_ISCONN);
  }
  else
#endif /* (TCPIP_DEV_ERROR_DETECT == STD_ON) */
  {
    switch(paramId)
    {
      case TCPIP_PARAMID_TCP_RXWND_MAX:
      {
        retVal = TcpIp_Tcp_changeParameterRcvWinMax(ctrlBlockPtr, paramValuePtr);
        break;
      }
      case TCPIP_PARAMID_TCP_NAGLE:
      {
        retVal =  TcpIp_Tcp_changeParameterSetFlag
                    (ctrlBlockPtr, *paramValuePtr, TCPIP_TCP_NAGLE_FLAG);
        break;
      }
#if (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON)
      case TCPIP_PARAMID_TCP_KEEPALIVE_INTERVAL:
      {
        retVal = TcpIp_Tcp_changeParameterSetUint32
                   (&ctrlBlockPtr->keepAlive_interval, paramValuePtr);
        break;
      }
      case TCPIP_PARAMID_TCP_KEEPALIVE_PROBES_MAX:
      {
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
        if(*paramValuePtr == 0U)
        {
          /* invalid parameter value */
          TCPIP_REPORT_ERROR(TCPIP_SID_CHANGEPARAMETER, TCPIP_E_INV_ARG);
        }
        else
#endif /* (TCPIP_DEV_ERROR_DETECT == STD_ON) */
        {
          ctrlBlockPtr->keepAlive_probemax = *paramValuePtr;
          retVal = E_OK;
        }
        break;
      }
      case TCPIP_PARAMID_TCP_KEEPALIVE_TIME:
      {
        retVal = TcpIp_Tcp_changeParameterSetUint32
                   (&ctrlBlockPtr->keepAlive_time, paramValuePtr);
        break;
      }
      case TCPIP_PARAMID_TCP_KEEPALIVE:
      {
        retVal =  TcpIp_Tcp_changeParameterSetFlag
                    (ctrlBlockPtr, *paramValuePtr, TCPIP_TCP_KEEP_ALIVE_FLAG);
        break;
      }
#endif /* (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON) */
#if (TCPIP_TCP_OPTION_FILTER_ENABLE == STD_ON)
      case TCPIP_PARAMID_TCP_OPTIONFILTER:
      {
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
        if (TCPIP_TCP_OPTION_FILTER_NUM <= *paramValuePtr)
        {
          TCPIP_REPORT_ERROR(TCPIP_SID_CHANGEPARAMETER, TCPIP_E_INV_ARG);
        }
        else
#endif /* (TCPIP_DEV_ERROR_DETECT == STD_ON) */
        {
          ctrlBlockPtr->activeOptionFilter = *paramValuePtr;
          retVal = E_OK;
        }
        break;
      }
#endif /* (TCPIP_TCP_OPTION_FILTER_ENABLE == STD_ON) */
      /* CHECK: NOPARSE */
      default:
      {
        /* unknown parameter id */
        TCPIP_UNREACHABLE_CODE_ASSERT(TCPIP_SID_CHANGEPARAMETER);
        break;
      }
      /* CHECK: PARSE */
    }
  }

  DBG_TCPIP_TCP_CHANGEPARAMETER_EXIT(retVal, socketPtr, paramValuePtr, paramId);

  return retVal;
}

TS_MOD_PRIV_DEFN FUNC(uint8, TCPIP_CODE) TcpIp_Tcp_evaluateSegLen
(
  uint8 controlBits
)
{
  uint8 len = 0U;

  /* segment contains a FIN flag, increase segment length by one */
  if(TCPIP_TCP_FLAG_FIN_IS_SET(controlBits))
  {
     len++;
  }

  /* segment contains a SYN flag, increase segment length by one */
  if(TCPIP_TCP_FLAG_SYN_IS_SET(controlBits))
  {
     len++;
  }

  /* segment contains a RST flag, increase segment length by one */
  if(TCPIP_TCP_FLAG_RST_IS_SET(controlBits))
  {
     len++;
  }

  return len;
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Tcp_init
(
  void
)
{
  DBG_TCPIP_TCP_INIT_ENTRY();

#if (TCPIP_TCP_OUTOFORDER_RECEPTION_ENABLED == STD_ON)
  TcpIp_Tcp_rxOutOfOrder_Init();
#endif /* (TCPIP_TCP_OUTOFORDER_RECEPTION_ENABLED == STD_ON) */

#if (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON)
  {
#if (TCPIP_PRECONDITION_ASSERT_ENABLED == STD_ON)
    Std_ReturnType retVal = E_NOT_OK;
    /* keys should be correctly generated */
    retVal =
#else /* (TCPIP_PRECONDITION_ASSERT_ENABLED == STD_ON) */
    (void)
#endif /* (TCPIP_PRECONDITION_ASSERT_ENABLED == STD_ON) */
      TcpIp_Crypto_KeyGenerate(TCPIP_TCP_SYN_COOKIES_KEY_1);
    TCPIP_PRECONDITION_ASSERT((retVal == E_OK),TCPIP_INTERNAL_API_ID);
#if (TCPIP_PRECONDITION_ASSERT_ENABLED == STD_ON)
    /* keys should be correctly generated */
    retVal =
#else /* (TCPIP_PRECONDITION_ASSERT_ENABLED == STD_ON) */
    (void)
#endif /* (TCPIP_PRECONDITION_ASSERT_ENABLED == STD_ON) */
      TcpIp_Crypto_KeyGenerate(TCPIP_TCP_SYN_COOKIES_KEY_2);
    TCPIP_PRECONDITION_ASSERT((retVal == E_OK),TCPIP_INTERNAL_API_ID);
    TcpIp_Tcp_SYN_Cookies_Reset_Timer = 0U;
  }
#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON) */

#if (TCPIP_TCP_UNPREDICTABLE_INIT_SEQ_NUM_ENABLED == STD_ON)
  /* periodic key generate */
  TcpIp_Tcp_generateIssKey();
  TCPIP_TCP_UNPR_SEQ_NUM_TIMER_STOP();
#endif /* (TCPIP_TCP_UNPREDICTABLE_INIT_SEQ_NUM_ENABLED == STD_ON) */
  DBG_TCPIP_TCP_INIT_EXIT();
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Tcp_closeProcessedTcpSocket
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr =
    &socketPtr->tcp;

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

  if(ctrlBlockPtr->avail_buff != 0U)
  {
    /* if a buffer was previously allocated free it */
    (void)TcpIp_Memory_decrease(ctrlBlockPtr->memory_idPtr, ctrlBlockPtr->avail_buff);
  }

  if(TCPIP_TCP_FLAG_ACTIVE_IS_SET(ctrlBlockPtr->flags))
  {
    const uint8 localsockOwnerId = socketPtr->generic.aligned.sockOwnerId;

    TCPIP_PRECONDITION_ASSERT
      ((localsockOwnerId < TcpIp_NumConfSockOwner),TCPIP_INTERNAL_API_ID);

    /* error occurred, inform Soad */
    TcpIp_TcpIpEventAPI[localsockOwnerId](socketId, TCPIP_TCP_RESET);
  }

  TcpIp_Socket_closeTcp(socketId);
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_getTransmissionParam
(
  TcpIp_LocalAddrIdType localAddrId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  uint8 nextHeader,
  uint16 localPort,
  P2VAR(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameterPtr
)
{
  Std_ReturnType result = TCPIP_E_NOT_OK;
  uint8 ipSecSaCommonIdx = 0U;
  uint8 addHeaderLength = 0U;
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) addHeaderLengthPtr = &addHeaderLength;
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) ipSecSaCommonIdxPtr = &ipSecSaCommonIdx;

#if (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL)
  uint8 IpsecMechanism;

#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)
  ipSecSaCommonIdxPtr = &ipParameterPtr->ipSecSaCommonIdx;
  addHeaderLengthPtr = &ipParameterPtr->addHeaderLength;
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */

  /* check policy table if traffic shall be secured, bypassed or discarded */
  IpsecMechanism =
    TcpIp_IpSecDb_checkSecurityTxPolicy
      (localAddrId, remoteAddrPtr, nextHeader, localPort, ipSecSaCommonIdxPtr);

  if(IpsecMechanism != TCPIP_IPSECDB_MECHANISM_DISCARDED)
#else
  TS_PARAM_UNUSED(remoteAddrPtr);
  TS_PARAM_UNUSED(nextHeader);
  TS_PARAM_UNUSED(localPort);
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL) */
  {
    /* retrieve maximal ip payload length and additional headers to consider */
    result =
      TcpIp_Ip_getMaximumPayload
        (
         *ipSecSaCommonIdxPtr,
         localAddrId,
         &ipParameterPtr->maxIpPayloadLength,
         addHeaderLengthPtr
        );
  }

  return result;
}

#if (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON)
TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Tcp_SYN_Cookies_setKeyRegenerateTimer(void)
{
  if(TcpIp_Tcp_SYN_Cookies_Reset_Timer == 0U)
  {
    TcpIp_Tcp_SYN_Cookies_Reset_Timer = TCPIP_TCP_SYN_COOKIES_RESET_KEYS;
  }
}
#endif /* (TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON) */

#if (TCPIP_TCP_UNPREDICTABLE_INIT_SEQ_NUM_ENABLED == STD_ON)

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Tcp_unprSeqNumSetKeyRegenerateTimer(void)
{
  if(TCPIP_TCP_UNPR_SEQ_NUM_TIMER_STOPPED())
  {
    TCPIP_TCP_UNPR_SEQ_NUM_TIMER_START();
  }
}

#endif /* (TCPIP_TCP_UNPREDICTABLE_INIT_SEQ_NUM_ENABLED == STD_ON) */

TS_MOD_PRIV_DEFN FUNC(uint32, TCPIP_CODE) TcpIp_Tcp_getInitialSeqNum(void)
{

  /* increase initial Sequence Number for every connection */
  TcpIp_Tcp_InitalSeqNum += TCPIP_TCP_INITIAL_SEQUENCE_NUMBER_INCREMENT;

  return TcpIp_Tcp_InitalSeqNum;
}

/*==================[internal function definitions]=========================*/

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_changeParameterSetFlag
(
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr,
  uint8 paramValue,
  uint8 flag
)
{
  Std_ReturnType retVal = E_NOT_OK;

#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  if(paramValue > 1U)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_CHANGEPARAMETER, TCPIP_E_INV_ARG);
  }
  else
#endif  /* (TCPIP_DEV_ERROR_DETECT == STD_ON) */
  {
    if(paramValue == 1U)
    {
      /* activate nagle algorithm */
      ctrlBlockPtr->flags |= flag;
    }
    else
    {
      /* deactivate nagle algorithm */
      ctrlBlockPtr->flags &= (uint8)(~flag);
    }

    retVal = E_OK;
  }

  return retVal;
}

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_changeParameterRcvWinMax
(
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) paramValuePtr
)
{
  Std_ReturnType retVal = E_NOT_OK;

#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  if(!TCPIP_ISALIGNED(paramValuePtr, sizeof(uint16)))
  {
    /* invalid parameter size */
    TCPIP_REPORT_ERROR(TCPIP_SID_CHANGEPARAMETER, TCPIP_E_INV_ARG);
  }
  else
#endif  /* (TCPIP_DEV_ERROR_DETECT == STD_ON) */
  {
    const uint16 receiveWin =
                   /* Deviation MISRAC2012-3 */
                   *((P2CONST(uint16, AUTOMATIC, TCPIP_APPL_DATA)) paramValuePtr);

    /* Change receive window advertised */
    ctrlBlockPtr->recv_wnd = receiveWin;
    /* Change default receive window */
    ctrlBlockPtr->recv_wndmax = receiveWin;

    /* setting socket option successful. */
    retVal = E_OK;
  }

  return retVal;
}

#if (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON)

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_changeParameterSetUint32
(
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) paramPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) paramValuePtr
)
{
  Std_ReturnType retVal = E_NOT_OK;

#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  if(!TCPIP_ISALIGNED(paramValuePtr, sizeof(uint32)))
  {
    /* invalid parameter size */
    TCPIP_REPORT_ERROR(TCPIP_SID_CHANGEPARAMETER, TCPIP_E_INV_ARG);
  }
  else
#endif  /* (TCPIP_DEV_ERROR_DETECT == STD_ON) */
  {
    const uint32 value =
                   /* Deviation MISRAC2012-3 */
                   *((P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA)) paramValuePtr);

#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
    if(value == 0U)
    {
      /* invalid parameter value */
      TCPIP_REPORT_ERROR(TCPIP_SID_CHANGEPARAMETER, TCPIP_E_INV_ARG);
    }
    else
#endif /* (TCPIP_DEV_ERROR_DETECT == STD_ON) */
    {
      *paramPtr = value;

      /* setting socket option successful. */
      retVal = E_OK;
    }
  }

  return retVal;
}

#endif /* (TCPIP_TCP_KEEP_ALIVE_ENABLED == STD_ON) */

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_retransmission
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  TcpIp_ReturnType result;
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr = &socketPtr->tcp;

  TCPIP_PRECONDITION_ASSERT( TcpIp_PBcfgPtr != NULL_PTR, TCPIP_INTERNAL_API_ID );
  if(TCPIP_TCP_FLAG_RETRY_IS_SET(ctrlBlockPtr->flags))
  {
    ctrlBlockPtr->num_retry++;

    /* last segment could not be transmitted because no hardware buffer was available,
       retry transmission */
    result = TcpIp_Tcp_txData_retransmitUnsent(socketPtr);

    if(result == TCPIP_OK)
    {
      /* segment could be sent */
      TcpIp_Tcp_stopRetry(ctrlBlockPtr);
    }
    else
    {
      if(ctrlBlockPtr->num_retry >= TcpIp_PBcfgPtr->transRetryNum)
      {
        /* maximal number of retries reached, close the connection */
        TcpIp_Tcp_closeProcessedTcpSocket(socketId, socketPtr);
      }
    }
  }
  /* retransmission timer running */
  else if(TcpIp_Tcp_rtxTimerRunning(ctrlBlockPtr))
  {
    ctrlBlockPtr->rtx_timer++;

    if(ctrlBlockPtr->rtx_timer >= TcpIp_PBcfgPtr->tcpConfig.reTxTimeout)
    {
      /* retransmission timeout occurred, retransmit segment */
      result = TcpIp_Tcp_SM_rtxTimeout(socketId, socketPtr);
    }
    else
    {
      result = TCPIP_OK;
    }
  }
  else
  {
    /* no data buffered */
    result = TCPIP_OK;
  }

  return result;
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_ackTimer
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  TcpIp_ReturnType result = TCPIP_E_NOT_OK;
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr =
    &socketPtr->tcp;
  boolean transmitAck = FALSE;
  boolean transmitMaxReached = FALSE;

  TCPIP_PRECONDITION_ASSERT( TcpIp_PBcfgPtr != NULL_PTR, TCPIP_INTERNAL_API_ID );

  if(TcpIp_Tcp_duplicateAcknowledgementTimerRunningSeqNum(ctrlBlockPtr))
  {
    ctrlBlockPtr->dupackSeqNum_timer--;
  }
  if(TcpIp_Tcp_duplicateAcknowledgementTimerRunningAckNum(ctrlBlockPtr))
  {
    ctrlBlockPtr->dupackAckNum_timer--;
  }

  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  /* ack timer running */
  if(ctrlBlockPtr->ack_timer != TCPIP_TCP_TIMER_UINT16_NOT_RUN)
  {
    ctrlBlockPtr->ack_timer++;

    /* transmission of an ack ongoing */
    if(TCPIP_TCP_FLAG_WAIT_CONF_IS_SET(ctrlBlockPtr->flags))
    {
      if(ctrlBlockPtr->ack_timer >= TcpIp_PBcfgPtr->transRetryNum)
      {
        /* max number of retries reached delete connection */
        transmitMaxReached = TRUE;
      }

      /* retransmit ack */
      transmitAck = TRUE;
    }
    else
    {
      if(ctrlBlockPtr->ack_timer >= TcpIp_PBcfgPtr->tcpConfig.delayedAckTimeout)
      {
        /* stop delayed ack timer */
        TcpIp_Tcp_stopAckTimer(ctrlBlockPtr);

        /* delay of ack transmission reached, trigger transmission */
        transmitAck = TRUE;
      }
      else
      {
        result = TCPIP_OK;
      }
    }
  }
  else
  {
    result = TCPIP_OK;
  }

  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

  /* check if ack shall be transmitted */
  if(transmitAck)
  {
    result = TcpIp_Tcp_txData_sendAck(socketPtr);

    /* if there is no buffer or physical address */
    if(result != TCPIP_OK)
    {
      if(transmitMaxReached)
      {
        /* max number of retries reached delete connection */
        TcpIp_Tcp_closeProcessedTcpSocket(socketId, socketPtr);
      }
      else
      {
        SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();

        /* ack transmission failed, if timer is not running start it */
        if(!TcpIp_Tcp_ackTimerRunning(ctrlBlockPtr))
        {
          /* mark retransmission of ACK in case no hardware buffer is available */
          TcpIp_Tcp_startAckRetranTimer(ctrlBlockPtr);
        }

        SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_0();
      }
    }
  }

  return result;
}

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_stateTimer
(
  TcpIp_SocketIdType socketId,
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  TcpIp_ReturnType result;
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr =
    &socketPtr->tcp;

  /* state timer running */
  if(TcpIp_Tcp_stateTimerRunning(ctrlBlockPtr))
  {
    ctrlBlockPtr->state_timer--;

    if(ctrlBlockPtr->state_timer == 0U)
    {
      /* state timeout reached, delete connection */
      result = TcpIp_Tcp_SM_stateTimeout(socketId, socketPtr);
    }
    else
    {
      result = TCPIP_OK;
    }
  }
  else
  {
    result = TCPIP_OK;
  }

  return result;
}

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Tcp_isAlreadyListen
(
  TcpIp_SocketIdType SocketId
)
{
  TcpIp_ReturnType result;
  P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) tcpSocketPtr;

  TcpIp_Socket_Generic_PtrType socketPtr = TcpIp_Socket_getSocketPtr(SocketId);

  tcpSocketPtr =
       /* Deviation MISRAC2012-3 */
       (P2VAR(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA)) socketPtr;

  if(TCPIP_TCP_STATE_CLOSED == tcpSocketPtr->tcp.state)
  {
    result = TcpIp_Socket_Tcp_isAlreadyListen(SocketId);
  }
  else
  {
    result = E_OK;
  }
  return result;
}

#endif /* (TCPIP_DEV_ERROR_DETECT == STD_ON) */

#if (TCPIP_TCP_UNPREDICTABLE_INIT_SEQ_NUM_ENABLED == STD_ON)
STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_generateIssKey
(
  void
)
{
#if (TCPIP_PRECONDITION_ASSERT_ENABLED == STD_ON)
  Std_ReturnType retVal = E_NOT_OK;
  /* keys should be correctly generated */
  retVal =
#else /* (TCPIP_PRECONDITION_ASSERT_ENABLED == STD_ON) */
  (void)
#endif /* (TCPIP_PRECONDITION_ASSERT_ENABLED == STD_ON) */
    TcpIp_Crypto_KeyGenerate(TCPIP_TCP_UNPR_SEQ_NUM_KEY);
  TCPIP_PRECONDITION_ASSERT((retVal == E_OK),TCPIP_INTERNAL_API_ID);
}
#endif /* (TCPIP_TCP_UNPREDICTABLE_INIT_SEQ_NUM_ENABLED == STD_ON) */

#endif /* (TCPIP_TCP_ENABLED == STD_ON) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[end of file]===========================================*/
