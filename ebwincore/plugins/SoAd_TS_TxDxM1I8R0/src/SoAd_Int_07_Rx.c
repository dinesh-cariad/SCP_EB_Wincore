/**
 * \file
 *
 * \brief AUTOSAR SoAd
 *
 * This file contains the implementation of the AUTOSAR
 * module SoAd.
 *
 * \version 1.8.16
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
 *
 * MISRAC2012-3) Deviated Rule: 14.2
 * A for loop shall be well-formed.
 *
 * Reason:
 * 1)The comparison is done with the incremented loop counter to protect the last for loop entry.
 * 2)Macro COMSTACK_BF_ITERATE is an optimized pattern to iterate over bit fields.
 *   It allows the compiler simplifying control code (fewer conditional jumps) and reducing the
 *   number of cache misses.
 *
 */

/*==================[inclusions]============================================*/
#include <TSAutosar.h> /* EB specific standard types */
/* !LINKSTO SoAd.ASR42.SWS_SoAd_00073, 1 */
#include <ComStack_Types.h>
/* !LINKSTO SoAd.ASR42.SWS_SoAd_00072, 1 */
#include <SoAd.h> /* Module public API */
#if(SOAD_META_DATA_HANDLING == STD_ON)
#include <EcuC.h> /* Module public API */
#endif
#include <SoAd_Int.h> /* Module internal header file */
/* !LINKSTO SoAd.ASR42.SWS_SoAd_00073, 1 */
#include <SoAd_Cbk.h> /* Module callbacks */
#include <SoAd_Version.h> /* Module Version Info */
#include <SchM_SoAd.h>
#if(SOAD_DEV_ERROR_DETECT == STD_ON)
#include <Det.h> /* Det API */
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */

#include <SoAd_01_UdpSM_Int.h> /* UDP state machine unit. */
#include <SoAd_04_TcpSM_Int.h> /* UDP state machine unit. */

#include <SoAd_02_SocketCon_Int.h> /* Common socket handling relevant APIs. */
#include <ComStack_Helpers.h>


#if(SOAD_TLS_ENABLED == STD_ON)
#include <Tls.h>
#endif /* SOAD_TLS_ENABLED == STD_ON */
/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/
#if(TS_MERGED_COMPILE == STD_OFF)
#define SOAD_START_SEC_CODE
#include <SoAd_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

STATIC FUNC(void, SOAD_CODE) SoAd_MainFunctionTpRx
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

/** \brief Helper function to process TP PDUs during main function in header mode
 **
 **
 ** \param[in] SoConPtr - Pointer to socket connection configuration.
 ** \param[in] SoConStatePtr - Pointer to socket connection state.
 ** \param[in] SoConRxDataPtr - Pointer to Rx data belonging to socket connection.
 ** \param[in] RingBufferAccessPtr - Pointer to ring TP buffer structure.
 ** \param[out] TcpMoveWindowLenPtr - Pointer returning the processed bytes to open the TCP
 **                                   Rx window.
 **/
STATIC FUNC(void, SOAD_CODE) SoAd_ProcessTpPdu_HeaderMode
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(SoAd_SoConRxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConRxDataPtr,
  CONSTP2VAR(SoAd_RingBufferAccessType, AUTOMATIC, SOAD_APPL_DATA) RingBufferAccessPtr,
  CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) TcpMoveWindowLenPtr
);

/** \brief Helper function to process TP PDUs during main function in non header mode
 **
 **
 ** \param[in] SoConPtr - Pointer to socket connection configuration.
 ** \param[in] SoConStatePtr - Pointer to socket connection state.
 ** \param[in] RingBufferAccessPtr - Pointer to ring TP buffer structure.
 ** \param[out] TcpMoveWindowLenPtr - Pointer returning the processed bytes to open the TCP
 **                                   Rx window.
 **/
STATIC FUNC(void, SOAD_CODE) SoAd_ProcessTpPdu_NonHeaderMode
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(SoAd_RingBufferAccessType, AUTOMATIC, SOAD_APPL_DATA) RingBufferAccessPtr,
  CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) TcpMoveWindowLenPtr
);

STATIC FUNC(void, SOAD_CODE) SoAd_RxUdp
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  uint16 Length,
  SoAd_SoConIdType SoConGroupFirstIdOffset
);

STATIC FUNC(void, SOAD_CODE) SoAd_RxIndication_Tcp
(
  SoAd_SoConIdType SoConId,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  uint16 Length,
  CONSTP2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) RemoteAddrPtr
);

STATIC FUNC(void, SOAD_CODE) SoAd_RxIndication_Udp
(
  SoAd_SoConIdType SoConFirstId,
  CONSTP2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) RemoteAddrPtr,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  uint16 Length
);

/** \brief Helper function to pre check UDP frames before they get processed.
 **
 ** This function checks if UDP frames contain at least a valid PDU. If
 ** SoAdSocketUdpStrictHeaderLenCheckEnabled is enabled, the frame gets ignored if it
 ** contains unknown bytes at the end of the frame.
 **
 ** \param[in] SoConPtr - Pointer to socket connection configuration.
 ** \param[in] BufPtr - Pointer to the received data.
 ** \param[in] Length - Received data length.
 ** \param[in] SocketRouteSourceId - Id of the corresponding socket route.
 **/
STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_RxIndication_CheckUdpFrame
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  uint16 Length,
  SoAd_SoConIdType SoConGroupFirstIdOffset
);

STATIC FUNC(void, SOAD_CODE) SoAd_RxProcessTcpPduHeaders
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  uint16 Length,
  SoAd_SoConIdType SoConGroupFirstIdOffset
);

STATIC FUNC(void, SOAD_CODE) SoAd_RxProcessTcpPduHeaders_IfApi
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) CurrBufPtr,
  CONSTP2CONST(SoAd_SocketRouteType, AUTOMATIC, SOAD_APPL_DATA) SocketRoutePtr,
  PduLengthType LengthInHeader,
  CONSTP2VAR(uint16, AUTOMATIC, SOAD_APPL_DATA) RemainingLenPtr
);

STATIC FUNC(void, SOAD_CODE) SoAd_RxProcessUdpPduHeaders
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  uint16 Length,
  SoAd_SoConIdType SoConGroupFirstIdOffset
);

STATIC FUNC(void, SOAD_CODE) SoAd_RxProcessTcpPduSegment
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  CONSTP2VAR(uint16, AUTOMATIC, SOAD_APPL_DATA) Length,
  SoAd_SoConIdType SoConGroupFirstIdOffset
);

/** \brief Helper function to perform a If reception for an upper layer.
 **
 ** \param[in] SoConPtr - Pointer to the socket connection configuration.
 ** \param[in] SocketRoutePtr - Pointer to the socket route the PDU belongs to.
 ** \param[in] BufPtr - Pointer to the received data.
 ** \param[in] Length - Length of received data.
 **/
STATIC FUNC(void, SOAD_CODE) SoAd_ULIfRxIndication
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2CONST(SoAd_SocketRouteDestType, AUTOMATIC, SOAD_APPL_DATA) SocketRouteDestPtr,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  PduLengthType Length
);

STATIC FUNC(boolean, SOAD_CODE) SoAd_IsSocketRouteDestEnabled
(
  CONSTP2CONST(SoAd_SocketRouteType, AUTOMATIC, SOAD_APPL_DATA) SocketRoutePtr,
  SoAd_SoConIdType SoConGroupFirstIdOffset
);


/** \brief Checks if a given remote address passes the message acceptance filter of a
 **        socket connection. If the configured remote address contains wild card(s)
 **        the socket connection state remote address information is updated and
 **        the upper layer is notified in case it is a UDP socket connection.
 **
 ** \param[in] SoConPtr - Pointer to socket connection configuration.
 ** \param[in] SoConStatePtr - Pointer to socket connection state.
 ** \param[in] RemoteAddrPtr  Pointer to the address information to compare.
 ** \param[in] AddressMatch - Contains the result of the best match algorithm.
 **
 ** \returns   boolean
 ** \retval    TRUE   The remote address passed the filter.
 ** \retval    FALSE  The remote address did not pass the filter.
 **/
STATIC FUNC(boolean, SOAD_CODE) SoAd_MessageAcceptanceFilter
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) RemoteAddrPtr,
  SoAd_AddressMatchType AddressMatch
);

STATIC FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_ULTpCopyRxData
(
  CONSTP2CONST(SoAd_SocketRouteType, AUTOMATIC, SOAD_APPL_DATA) SocketRoutePtr,
  CONSTP2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_CONST) PduInfoPtr,
  CONSTP2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_DATA) BufferSizePtr
);

STATIC FUNC(void, SOAD_CODE) SoAd_ReceiveTpPdu
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  SoAd_RouteIdType SocketRouteId,
  CONSTP2VAR(SoAd_RingBufferAccessType, AUTOMATIC, SOAD_APPL_DATA) RingBufferAccessPtr,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) PduHeaderPtr,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  uint16 Length
);



STATIC FUNC(void, SOAD_CODE) SoAd_ReceiveTpPduUdpNoBuf
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  SoAd_RouteIdType SocketRouteId,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  uint16 Length
);

STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_RingBufferStore
(
  CONSTP2VAR(SoAd_RingBufferAccessType, AUTOMATIC, SOAD_APPL_DATA) RingBufPtr,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) SrcHeaderBufPtr,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) SrcPayloadBufPtr,
  SoAd_RefToRAM Length
);

/** \brief Helper function to forward Rx TP PDU from ring buffer to upper layer
 **
 ** \param[in] RingBufPtr - Pointer to ring buffer structure.
 ** \param[in] SocketRoutePtr - Pointer to the socket route the PDU belongs to.
 ** \param[in/out] BufferSizePtr - Pointer to provide data to be copied and return available
                                   buffer size.
 **
 ** \return BufReq_ReturnType
 ** \retval BUFREQ_OK - Delivery was okay.
 ** \retval !BUFREQ_OK - Problem occurred.
 **/
STATIC FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_RingBufferDeliverTp
(
  CONSTP2VAR(SoAd_RingBufferAccessType, AUTOMATIC, SOAD_APPL_DATA) RingBufPtr,
  CONSTP2CONST(SoAd_SocketRouteType, AUTOMATIC, SOAD_APPL_DATA) SocketRoutePtr,
  CONSTP2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_DATA) BufferSizePtr
);

STATIC FUNC(PduLengthType, SOAD_CODE) SoAd_RingBufferDiscard
(
  CONSTP2VAR(SoAd_RingBufferAccessType, AUTOMATIC, SOAD_APPL_DATA) RingBuf,
  PduLengthType Length
);

/** \brief Helper function to load the ring buffer access structure.
 **
 ** \param[in] SoConPtr - Pointer to socket connection configuration.
 ** \param[in] SoConStatePtr - Pointer to socket connection state.
 ** \param[out] RingBufferAccessPtr - Pointer ring buffer entry to be stored.
 **
 ** \return Std_ReturnType
 ** \retval E_OK - Ring buffer loaded.
 ** \retval E_NOT_OK - No ring buffer available.
 */
STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_RingBufferLoadAccess
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(SoAd_RingBufferAccessType, AUTOMATIC, SOAD_APPL_DATA) RingBufferAccessPtr
);

/** \brief Helper function to store the ring buffer access structure.
 **
 ** \param[in] SoConPtr - Pointer to socket connection configuration.
 ** \param[in] SoConStatePtr - Pointer to socket connection state.
 ** \param[in] RingBufferAccessPtr - Pointer to ring buffer access structure to be stored.
 */
STATIC FUNC(void, SOAD_CODE) SoAd_RingBufferSaveAccess
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(SoAd_RingBufferAccessType, AUTOMATIC, SOAD_APPL_DATA) RingBufferAccessPtr
);

STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_PduHeaderSearch
(
  P2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  VAR(uint32, SOAD_VAR) PduHeaderId,
  P2VAR(SoAd_RouteIdType, AUTOMATIC, SOAD_APPL_DATA) SocketRouteIdPtr
);

STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_ExtractPduHeader
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2CONST(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  CONSTP2VAR(SoAd_RouteIdType, AUTOMATIC, SOAD_APPL_DATA) SocketRouteIdPtr,
  CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) PduHeaderIdPtr,
  CONSTP2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_DATA) PduLengthPtr
);

STATIC FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_ProcessRxData
(
  uint16 Length,
  P2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_DATA) BufferSize,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_DATA) RemainingLength,
  P2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) TcpMoveWindowLen,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  CONSTP2CONST(SoAd_SocketRouteType, AUTOMATIC, SOAD_APPL_DATA) SocketRoutePtr,
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr
);

STATIC FUNC(SoAd_AddressMatchType, SOAD_CODE) SoAd_RemoteAddressMatch
(
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) RemoteAddrPtr
);



/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

FUNC(void, SOAD_CODE) SoAd_RxIndication
(
  TcpIp_SocketIdType SocketId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) RemoteAddrPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  uint16 Length
)
{
  DBG_SOAD_RXINDICATION_ENTRY(SocketId, RemoteAddrPtr, BufPtr, Length);

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  if(SoAd_Initialized == SOAD_UNINITED)
  {
    SOAD_DET_REPORT_ERROR(SOAD_RXINDICATION_SVCID, SOAD_E_NOTINIT);
  }
  else if(BufPtr == NULL_PTR)
  {
    SOAD_DET_REPORT_ERROR(SOAD_RXINDICATION_SVCID, SOAD_E_PARAM_POINTER);
  }
  else if(RemoteAddrPtr == NULL_PTR)
  {
    SOAD_DET_REPORT_ERROR(SOAD_RXINDICATION_SVCID, SOAD_E_INV_ARG);
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    SoAd_SoConIdType SoConFirstId;

    SOAD_ENTER_CRITSEC();
    SoConFirstId = SoAd_SocketToSoConId(SocketId);
    SOAD_EXIT_CRITSEC();
    if(SoConFirstId != SOAD_INVALID_SOCON_ID)
    {
      P2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr =
        &SOAD_CFG(SoCon, SoConFirstId);

      if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_TCP))
      {
        SoAd_RxIndication_Tcp(SoConFirstId, BufPtr, Length, RemoteAddrPtr);
      }
      else
      {
        SoAd_RxIndication_Udp(SoConFirstId, RemoteAddrPtr, BufPtr, Length);
      }
    }
#if(SOAD_DEV_ERROR_DETECT == STD_ON)
    else
    {
      SOAD_DET_REPORT_ERROR(SOAD_RXINDICATION_SVCID, SOAD_E_INV_SOCKETID);
    }
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  }
  DBG_SOAD_RXINDICATION_EXIT(SocketId, RemoteAddrPtr, BufPtr, Length);
}

FUNC(Std_ReturnType, SOAD_CODE) SoAd_TpCancelReceive
(
  PduIdType PduId
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_SOAD_TPCANCELRECEIVE_ENTRY(PduId);

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  if(SoAd_Initialized == SOAD_UNINITED)
  {
    SOAD_DET_REPORT_ERROR(SOAD_TPCANCELRECEIVE_SVCID, SOAD_E_NOTINIT);
  }
  else if(PduId >= SoAd_PBcfg->CancelReceiveSoConMapCnt)
  {
    SOAD_DET_REPORT_ERROR(SOAD_TPCANCELRECEIVE_SVCID, SOAD_E_INV_PDUID);
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    CONSTP2CONST(SoAd_CancelReceiveSoConMapType, AUTOMATIC, SOAD_APPL_DATA)
      CancelReceiveSoConMapEntry = &SOAD_CFG(CancelReceiveSoConMap, PduId);
    CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr =
        &SOAD_CFG(SoCon, CancelReceiveSoConMapEntry->SoConId);
    CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr =
        &SOAD_PBRAM(SoConState, CancelReceiveSoConMapEntry->SoConId);
    P2VAR(SoAd_SoConRxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConRxDataPtr = NULL_PTR;

    SoAd_GetSoConRxData(SoConPtr, SoConRxDataPtr);

    SOAD_ENTER_CRITSEC();
    if(    (SoConRxDataPtr->CurrPduLen != SoConRxDataPtr->CurrPduForwardedLen)
        && (CancelReceiveSoConMapEntry->SocketRouteId == SoConRxDataPtr->CurrSocketRouteId)
      )
    {
      SOAD_FLAG_SET_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RX_TP_CANCELED);

      /* Set global flag to know that for a certain SoCon a transition condition was met. */
      SoAd_SetBitInTransCondChange(SoConPtr);

      RetVal = E_OK;
    }
    SOAD_EXIT_CRITSEC();
  }

  DBG_SOAD_TPCANCELRECEIVE_EXIT(RetVal, PduId);
  return RetVal;
}



TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_MainFunctionRx(void)
{
  uint32 NextSoConId;
  const SoAd_SoConIdType SoConMax = SoAd_PBcfg->SoConCnt;
  uint8 RxPendingFlag;
  uint8 SoConStateLowerFlags;
  P2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr;
  P2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr;

  CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) SoConRxNotIdleBitMapPtr =
                    &SOAD_PBRAM_REF_ID(uint32, SoAd_PBcfg->SoConRxNotIdleBitMapBase, 0U);

  TS_AtomicAssign8(SoAd_RxCondChange, FALSE);
  /* Deviation MISRAC2012-3 */
  for(SOAD_COMSTACK_BF_ITERATE(NextSoConId, SoConRxNotIdleBitMapPtr, COMSTACK_BF_START, SoConMax))
  {
    SoConPtr = &SOAD_CFG(SoCon, NextSoConId);
    SoConStatePtr = &SOAD_PBRAM(SoConState, NextSoConId);
    /* Before anything else, clear the Status Bit in the map, because preemption could already
    set it during the Rx Action */
    SoAd_ClearBitInBitMap(SoConPtr, &SoAd_PBcfg->SoConRxNotIdleBitMapBase);

    /* If data is available in the receive buffer it shall be processed. */
    TS_AtomicAssign8(SoConStateLowerFlags, SoConStatePtr->LowerFlags);
    RxPendingFlag = SoConStateLowerFlags & SOAD_SOCONSTATE_FLAG_RX_DATA_BUFFERED_MASK;

    /* Flag is integrated into SoConRxNotIdleBitMapBase. IF could be unreachable here */
    if(RxPendingFlag != 0U)
    {
      SoAd_MainFunctionTpRx(SoConPtr, SoConStatePtr);
    }
  }
}

FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpAccepted
(
  TcpIp_SocketIdType SocketId,
  TcpIp_SocketIdType SocketIdConnected,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) RemoteAddrPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_SOAD_TCPACCEPTED_ENTRY(SocketId, SocketIdConnected, RemoteAddrPtr);

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  if(SoAd_Initialized == SOAD_UNINITED)
  {
    SOAD_DET_REPORT_ERROR(SOAD_TCPACCEPTED_SVCID, SOAD_E_NOTINIT);
  }
  else if(RemoteAddrPtr == NULL_PTR)
  {
    SOAD_DET_REPORT_ERROR(SOAD_TCPACCEPTED_SVCID, SOAD_E_PARAM_POINTER);
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    const SoAd_SoConIdType SoConFirstId = SoAd_ListenSocketToSoConId(SocketId);

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
    if(SoConFirstId == SOAD_INVALID_SOCON_ID)
    {
      SOAD_DET_REPORT_ERROR(SOAD_TCPACCEPTED_SVCID, SOAD_E_INV_SOCKETID);
    }
    else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
    {
      P2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr = &SOAD_CFG(SoCon, SoConFirstId);
      P2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr;
      SoAd_SoConIdType SoConId = SoConFirstId;
      boolean Accepted = FALSE;
      SoAd_AddressMatchType AddressMatch = SOAD_ADDRESSMATCH_NONE;

      SOAD_ENTER_CRITSEC();
      SoAd_BestMatchAlgorithm
      (
        SoConPtr,
        RemoteAddrPtr,
        &SoConId,
        &AddressMatch,
        FALSE, /* SocketRouteRequired */
        FALSE /* IncludeClosedSoCon */
      );

      SoConPtr = &SOAD_CFG(SoCon, SoConId);
      SoConStatePtr = &SOAD_PBRAM(SoConState, SoConId);

      if(SoConStatePtr->SocketState == SOAD_SOCKETSTATE_CONNECTING)
      {
        Accepted = SoAd_MessageAcceptanceFilter
        (
          SoConPtr,
          SoConStatePtr,
          RemoteAddrPtr,
          AddressMatch
        );
      }
      SOAD_EXIT_CRITSEC();

      if(Accepted)
      {
        SoAd_SoConIdType Idx;
        const SoAd_SoConIdType SoConCnt = SoAd_PBcfg->SoConCnt;
        boolean Success = FALSE;

        SoConStatePtr->SocketId = SocketIdConnected;

        for(Idx = 0U; (Idx < SoConCnt) && (Success == FALSE); Idx++)
        {
          CONSTP2VAR(SoAd_SocketSoConMapType, AUTOMATIC, SOAD_APPL_DATA) SocketSoConMapEntry =
              &SOAD_PBRAM(SocketSoConMap, Idx);

          if(SocketSoConMapEntry->SoConId == SOAD_INVALID_SOCON_ID)
          {
            SocketSoConMapEntry->SoConId = SoConId;
            SocketSoConMapEntry->SocketId = SocketIdConnected;
            Success = TRUE;
          }
        }

        SOAD_INVARIANT_ASSERT(Success == TRUE, SOAD_INTERNAL_SVCID);
#if(SOAD_TLS_ENABLED == STD_ON)
        if(SoConPtr->TlsConnectionId != SOAD_TLS_INVALID_CONNECTION_ID)
        {
          Tls_ConnectionIdType TlsId = (Tls_ConnectionIdType) SoConPtr->TlsConnectionId;

          (void)TcpIp_ChangeParameter
          (
             SocketIdConnected,
             TCPIP_PARAMID_TLS_CONNECTION_ID,
             &TlsId
           );
        }
        else
#endif /* SOAD_TLS_ENABLED == STD_ON */
        {
          /* Return value not checked. The return value is only relevant for main function
           *  repetition. */
        (void)SoAd_Int_04_TcpSM_Connected(SoConPtr, SoConStatePtr);
        }

        RetVal = E_OK;
      }
    }
  }

  DBG_SOAD_TCPACCEPTED_EXIT(RetVal, SocketId, SocketIdConnected, RemoteAddrPtr);
  return RetVal;
}

#if(SOAD_TLS_ENABLED == STD_ON)
FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpTlsAccepted
(
  TcpIp_SocketIdType SocketIdConnected
)
{
  SoAd_SoConIdType Idx;
  Std_ReturnType RetVal = E_NOT_OK;
  const SoAd_SoConIdType SoConCnt = SoAd_PBcfg->SoConCnt;
  boolean Success = FALSE;

  DBG_SOAD_TCPTLSACCEPTED_ENTRY(SocketIdConnected);

  for(Idx = 0U; (Idx < SoConCnt) && (Success == FALSE); Idx++)
  {
    CONSTP2VAR(SoAd_SocketSoConMapType, AUTOMATIC, SOAD_APPL_DATA) SocketSoConMapEntry =
        &SOAD_PBRAM(SocketSoConMap, Idx);

    if(SocketSoConMapEntry->SocketId == SocketIdConnected)
    {
      P2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr;
      P2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr;

      SoConPtr = &SOAD_CFG(SoCon, SocketSoConMapEntry->SoConId);
      SoConStatePtr = &SOAD_PBRAM(SoConState, SocketSoConMapEntry->SoConId);

      Success = TRUE;

      /* Return value not checked. The return value is only relevant for main function
       *  repetition. */
      (void)SoAd_Int_04_TcpSM_Connected(SoConPtr, SoConStatePtr);

      RetVal = E_OK;
    }
  }

  DBG_SOAD_TCPTLSACCEPTED_EXIT(RetVal, SocketIdConnected);

  return RetVal;
}
#endif /* SOAD_TLS_ENABLED == STD_ON */

FUNC(void, SOAD_CODE) SoAd_TcpConnected
(
  TcpIp_SocketIdType SocketId
)
{
  DBG_SOAD_TCPCONNECTED_ENTRY(SocketId);

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  if(SoAd_Initialized == SOAD_UNINITED)
  {
    SOAD_DET_REPORT_ERROR(SOAD_TCPCONNECTED_SVCID, SOAD_E_NOTINIT);
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    const SoAd_SoConIdType soConId = SoAd_SocketToSoConId(SocketId);

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
    if(soConId == SOAD_INVALID_SOCON_ID)
    {
      SOAD_DET_REPORT_ERROR(SOAD_TCPCONNECTED_SVCID, SOAD_E_INV_SOCKETID);
    }
    else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
    {
      CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr =
          &SOAD_CFG(SoCon, soConId);
      CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr =
          &SOAD_PBRAM(SoConState, soConId);

      /* Return value not checked. The return value is only relevant for main function
       *  repetition. */
      (void)SoAd_Int_04_TcpSM_Connected(SoConPtr, SoConStatePtr);
    }
  }

  DBG_SOAD_TCPCONNECTED_EXIT(SocketId);
}

TS_MOD_PRIV_DEFN FUNC(SoAd_SoConIdType, SOAD_CODE) SoAd_SocketToSoConId
(
  TcpIp_SocketIdType SocketId
)
{
  SoAd_SoConIdType RetVal = SOAD_INVALID_SOCON_ID;
  SoAd_SoConIdType Idx;
  const SoAd_SoConIdType SoConCnt = SoAd_PBcfg->SoConCnt;
  P2VAR(SoAd_SocketSoConMapType, AUTOMATIC, SOAD_APPL_DATA) SocketSoConMapEntryPtr;

  DBG_SOAD_SOCKETTOSOCONID_ENTRY(socketId);

  for(Idx = 0U; Idx < SoConCnt; Idx++)
  {
    SocketSoConMapEntryPtr = &SOAD_PBRAM(SocketSoConMap, Idx);
    if(
        (SocketSoConMapEntryPtr->SoConId != SOAD_INVALID_SOCON_ID)
        && (SocketSoConMapEntryPtr->SocketId == SocketId)
      )
    {
      RetVal = SocketSoConMapEntryPtr->SoConId;
      break;
    }
  }

  /* Required for the dynamic Polyspace analysis
    Accessing the PBRAM returns full range values due to the type casts; the
    assertion assures that we continue with a valid range */
  SOAD_POSTCONDITION_ASSERT(   (RetVal < SoAd_PBcfg->SoConCnt)
                            || (RetVal == SOAD_INVALID_SOCON_ID),
                            SOAD_INTERNAL_SVCID
                           );
  DBG_SOAD_SOCKETTOSOCONID_EXIT(RetVal, SocketId);
  return RetVal;
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_BestMatchAlgorithm
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) RemoteAddrPtr,
  CONSTP2VAR(SoAd_SoConIdType, AUTOMATIC, SOAD_APPL_DATA) MatchingSoConIdPtr,
  CONSTP2VAR(SoAd_AddressMatchType, AUTOMATIC, SOAD_APPL_DATA) AddressMatchPtr,
  boolean SocketRouteRequired,
  boolean IncludeClosedSoCon
)
{
  SoAd_SoConIdType SoConId;
  boolean ChooseAnySoCon = FALSE;
  SoAd_SoConIdType SelectedSoConId = SoConPtr->SoConId;
  SoAd_AddressMatchType CurrAddressMatch = SOAD_ADDRESSMATCH_NONE;
  SoAd_AddressMatchType BestAddressMatch = SOAD_ADDRESSMATCH_NONE;

  P2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr;
  CONSTP2CONST(SoAd_SoConGroupType, AUTOMATIC, SOAD_APPL_DATA) SoConGroupPtr =
    &SOAD_CFG(SoConGroup, SoConPtr->SoConGroupId);
  P2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConTmpPtr;

  const SoAd_SoConIdType SoConFirstId = SoConGroupPtr->SoConFirstId;
  const SoAd_SoConIdType MaxSoCon = SoConFirstId + SoConGroupPtr->SoConCnt;

  if(TRUE == SocketRouteRequired)
  {
    /* Check if a group socket route exists */
    CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConFirstPtr =
      &SOAD_CFG(SoCon, SoConFirstId);
    SoAd_RouteIdType SocketRouteId;
    P2CONST(SoAd_SocketRouteType, AUTOMATIC, SOAD_APPL_DATA) SocketRoutePtr = NULL_PTR;
    const SoAd_RouteIdType SocketRouteFirstId = SoConFirstPtr->SocketRouteFirstId;
    const SoAd_RouteIdType MaxSocketRoute = SocketRouteFirstId + SoConFirstPtr->SocketRouteCnt;

    if(SOAD_INVALID_ROUTE_ID != SoConFirstPtr->SocketRouteFirstId)
    {
      /* search for a related group socket route */
      for
      (
         SocketRouteId = SocketRouteFirstId;
         SocketRouteId < MaxSocketRoute;
         SocketRouteId++
      )
      {
        SocketRoutePtr = &SOAD_CFG(SocketRoute, SocketRouteId);

        if(SOAD_FLAG(SocketRoutePtr, SOAD_SOCKETROUTE_FLAG_IS_GROUPROUTE))
        {
          ChooseAnySoCon = TRUE;
          break;
        }
      }
    }
  }
  else
  {
    ChooseAnySoCon = TRUE;
  }

  /* ... identify the correct socket connection  */
  for(SoConId = SoConFirstId; SoConId < MaxSoCon; SoConId++)
  {
    SoConTmpPtr = &SOAD_CFG(SoCon, SoConId);
    SoConStatePtr = &SOAD_PBRAM(SoConState, SoConId);

    /* skip closed socket connections */
    if(
        (!(SoConStatePtr->SocketState == SOAD_SOCKETSTATE_CLOSED))
        || (IncludeClosedSoCon && SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_REMOTE_SET))
      )
    {
      /* if required, only select a socket connection with socketRoute(s) */
      if((TRUE == ChooseAnySoCon) || (SoConTmpPtr->SocketRouteFirstId != SOAD_INVALID_ROUTE_ID))
      {
        CurrAddressMatch = SoAd_RemoteAddressMatch(SoConStatePtr, RemoteAddrPtr);

        if(CurrAddressMatch > BestAddressMatch)
        {
          BestAddressMatch = CurrAddressMatch;
          SelectedSoConId = SoConId;
          if(BestAddressMatch == SOAD_ADDRESSMATCH_EXACT)
          {
            /* Exact match found, no further searching necessary */
            break;
          }
        }
      }
    }
  }

  *MatchingSoConIdPtr = SelectedSoConId;
  *AddressMatchPtr = BestAddressMatch;
}






/*==================[internal function definitions]=========================*/

STATIC FUNC(void, SOAD_CODE) SoAd_MainFunctionTpRx
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  SoAd_RingBufferAccessType RingBufferAccess;
  uint32 TcpMoveWindowLen = 0U;
  const boolean IsTcp = SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_TCP) ? TRUE : FALSE;

  DBG_SOAD_MAINFUNCTIONTPRX_ENTRY(SoConPtr, SoConStatePtr);

  /* SoAd_MainFunctionTpRx() is only called if data is stored in the ring buffer.
     SoAd_RingBufferLoadAccess() always returns OK since these checks were already performed
     during SoAd_RxIndication(). */
  (void)SoAd_RingBufferLoadAccess(SoConPtr, SoConStatePtr, &RingBufferAccess);

  if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_PDU_HEADER_MODE))
  {
    P2VAR(SoAd_SoConRxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConRxDataPtr = NULL_PTR;
    SoAd_GetSoConRxData(SoConPtr, SoConRxDataPtr);

    SoAd_ProcessTpPdu_HeaderMode
    (
      SoConPtr,
      SoConStatePtr,
      SoConRxDataPtr,
      &RingBufferAccess,
      &TcpMoveWindowLen
    );
  }
  else
  {
    SoAd_ProcessTpPdu_NonHeaderMode
    (
      SoConPtr,
      SoConStatePtr,
      &RingBufferAccess,
      &TcpMoveWindowLen
    );
  }
  SoAd_RingBufferSaveAccess(SoConPtr, SoConStatePtr, &RingBufferAccess);

  if(IsTcp && (TcpMoveWindowLen > 0U))
  {
    (void)TcpIp_TcpReceived(SoConStatePtr->SocketId, TcpMoveWindowLen);
  }

  DBG_SOAD_MAINFUNCTIONTPRX_EXIT(SoConPtr, SoConStatePtr);
}

STATIC FUNC(void, SOAD_CODE) SoAd_ProcessTpPdu_HeaderMode
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(SoAd_SoConRxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConRxDataPtr,
  CONSTP2VAR(SoAd_RingBufferAccessType, AUTOMATIC, SOAD_APPL_DATA) RingBufferAccessPtr,
  CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) TcpMoveWindowLenPtr
)
{
  P2CONST(SoAd_SocketRouteType, AUTOMATIC, SOAD_APPL_DATA) SocketRoutePtr = NULL_PTR;
  PduInfoType PduInfo = {NULL_PTR, 0U};
  boolean Proceed = TRUE;
  PduLengthType BufferSize = 0U; /* set to zero to query the upper layer */
  PduLengthType Length;
  BufReq_ReturnType RetVal_BufReq;
  CONSTP2CONST(SoAd_SoConGroupType, AUTOMATIC, SOAD_APPL_DATA) SoConGroupPtr =
    &SOAD_CFG(SoConGroup, SoConPtr->SoConGroupId);
  const SoAd_SoConIdType SoConGroupFirstIdOffset = SoConPtr->SoConId - SoConGroupPtr->SoConFirstId;

  DBG_SOAD_PROCESSTPPDU_HEADERMODE_ENTRY
  (
    SoConPtr,
    SoConStatePtr,
    SoConRxDataPtr,
    RingBufferAccessPtr,
    TcpMoveWindowLenPtr
  );

  TS_PARAM_UNUSED(SoConStatePtr);

  /* work with remaining PDU length */
  Length = (PduLengthType)(SoConRxDataPtr->CurrPduLen - SoConRxDataPtr->CurrPduForwardedLen);

  /* NOTE: The following loop is repeated until all buffered PDUs are processed, UL returns busy
   * or an error occurs. Several calls to <UL>_CopyRxData and <UL>_StartOfReception are possible,
   * to simplify the handling of their return values it is done mutually as part of the loop. */
  while((SOAD_RINGBUFFER_USED(RingBufferAccessPtr) > 0U) && (TRUE == Proceed))
  {
    boolean StartOfReceptionFailed = FALSE;
    Proceed = FALSE;

    /* if there is no active/current SocketRoute,
     * extract the next PduHeader and call StartOfReception */
    if(SoConRxDataPtr->CurrSocketRouteId == SOAD_INVALID_ROUTE_ID)
    {
      SoAd_RouteIdType NextSocketRouteId = SOAD_INVALID_ROUTE_ID;
      uint32 PduHeaderId;
      uint8 TempBuffer[SOAD_PDU_HEADER_LEN];
      P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) TempBufferPtr;
      const SoAd_RefToRAM RingBufferContLength = SOAD_RINGBUFFER_CONTUSED(RingBufferAccessPtr);

      /* Extract the next PduHeader from the ringBuffer. */
      if(RingBufferContLength < SOAD_PDU_HEADER_LEN)
      {
        const SoAd_RefToRAM RemainingHeaderLength =
          SOAD_PDU_HEADER_LEN - RingBufferContLength;

        /* Get the first part of the PduHeader stored in the ringbuffer */
        TS_MemCpy(
                   &TempBuffer[0U],
                   SOAD_RINGBUFFER_OUTPTR(RingBufferAccessPtr),
                   RingBufferContLength
                 );
        /* End of ringbuffer reached the remaining Header parts are at the begining */
        TS_MemCpy(
                   &TempBuffer[RingBufferContLength],
                   &((RingBufferAccessPtr)->BufPtr[0U]),
                   RemainingHeaderLength
                 );
        /* Set pointer to pass the header to SoAd_ExtractPduHeader() */
        TempBufferPtr = &TempBuffer[0];
        /* Out parameter is not increased because the Header in the ringbuffer is discarded anyway
         * after SoAd_ExtractPduHeader() */
      }
      else
      {
        /* Header was stored continuously inside the ringbuffer just pass the OutPtr to
         * SoAd_ExtractPduHeader() */
        TempBufferPtr = SOAD_RINGBUFFER_OUTPTR(RingBufferAccessPtr);
      }
      /* SoAd_ExtractPduHeader() always returns E_OK. The function was already executed in
         SoAd_RxIndication() and only frames with valid header are stored in
         the ring buffer. */
      (void) SoAd_ExtractPduHeader
      (
        SoConPtr,
        TempBufferPtr,
        &NextSocketRouteId,
        &PduHeaderId,
        &Length
      );

      /* skip over the PduHeader information stored in the ring buffer */
      (void)SoAd_RingBufferDiscard(RingBufferAccessPtr, SOAD_PDU_HEADER_LEN);

      /* remember to acknowledge the PduHeader to the TcpIp module */
      *TcpMoveWindowLenPtr += SOAD_PDU_HEADER_LEN;


      /* Since SoAd_ExtractPduHeader() always returns E_OK NextSocketRouteId is always valid. */
      SOAD_PRECONDITION_ASSERT(NextSocketRouteId != SOAD_INVALID_ROUTE_ID, SOAD_RXINDICATION_SVCID);

      /* check if the SocketRoute (i.e. the related RoutingGroups) are enabled */
      if(SoAd_IsSocketRouteDestEnabled(&SOAD_CFG(SocketRoute, NextSocketRouteId),
                                       SoConGroupFirstIdOffset
                                      )
        )
      {
        /* set up rxData information */
        SoConRxDataPtr->CurrSocketRouteId = NextSocketRouteId;
        SoConRxDataPtr->CurrPduLen = Length;
        SoConRxDataPtr->CurrPduForwardedLen = 0U;

        /* load the next socketRoute */
        SocketRoutePtr = &SOAD_CFG(SocketRoute, NextSocketRouteId);

        /* indicate start of reception to upper layer */
        RetVal_BufReq = SoAd_ULTpStartofReception(SoConPtr, SocketRoutePtr, Length, &BufferSize);

        if(BUFREQ_OK != RetVal_BufReq)
        {
          StartOfReceptionFailed = TRUE;
        }
        /* this PDU is was the last not fully received PDU,
         * hence reset the special counter */
        if(RingBufferAccessPtr->Used < Length)
        {
          /* reset the next PDU counter because there is no "next" PDU yet,
           * for which SoAd would have to keep track of the PDU Header offsets */
          SoConRxDataPtr->NextPduOutstandingLen = 0U;
        }
      }
      else
      {
        /* SocketRoute disabled, pretend UL canceled the reception -> PDU will be discarded */
        RetVal_BufReq = BUFREQ_E_NOT_OK;
        StartOfReceptionFailed = TRUE;
      }
    }
    else
    {
      /* there is an active/current SocketRoute,
       * use CopyRxData to retrieve available buffer size */
      /* load SocketRoute */
      SocketRoutePtr = &SOAD_CFG(SocketRoute, SoConRxDataPtr->CurrSocketRouteId);
      RetVal_BufReq = SoAd_ULTpCopyRxData(SocketRoutePtr, &PduInfo, &BufferSize);
    }

    if(
        (RetVal_BufReq == BUFREQ_OK)
        && (BufferSize != 0U)
      )
    {
      PduLengthType TmpBufferSize;
      /* avoid to copy more data than the UL can process */
      if(BufferSize > Length)
      {
        BufferSize = Length;
      }

      /* avoid to copy more data then available in the ring buffer */
      if(SOAD_RINGBUFFER_USED(RingBufferAccessPtr) < BufferSize)
      {
        BufferSize = (PduLengthType)SOAD_RINGBUFFER_USED(RingBufferAccessPtr);
      }

      /* BufferSize shall not be overwritten. TmpBufferSize returns the next buffer size, which gets
         ignored. */
      TmpBufferSize = BufferSize;

      /* deliver the PDU from the ring buffer to the UL */
      /* cast protected by if(SOAD_RINGBUFFER_USED(RingBufferAccessPtr) < BufferSize) */
      RetVal_BufReq = SoAd_RingBufferDeliverTp
      (
        RingBufferAccessPtr,
        SocketRoutePtr,
        &TmpBufferSize
       );

    }

    if(RetVal_BufReq == BUFREQ_OK)
    {
      SoConRxDataPtr->CurrPduForwardedLen += BufferSize;

      /* remember to acknowledge TcpIp module */
      *TcpMoveWindowLenPtr += BufferSize;

      /* PDU Header mode is enabled and everything was delivered */
      if(SoConRxDataPtr->CurrPduForwardedLen == SoConRxDataPtr->CurrPduLen)
      {
        /* TP only: For TP only 1 Destination is allowed to be configured.
         * Therefore we can use the SocketRouteDestFirstId and do not need to iterate.*/
        P2CONST(SoAd_SocketRouteDestType, AUTOMATIC, SOAD_APPL_DATA) SocketRouteDestPtr =
          &SOAD_CFG(SocketRouteDest, SocketRoutePtr->SocketRouteDestFirstId);
        /* reset rxData information */
        SoConRxDataPtr->CurrSocketRouteId = SOAD_INVALID_ROUTE_ID;
        SoConRxDataPtr->CurrPduLen = 0U;
        SoConRxDataPtr->CurrPduForwardedLen = 0U;

        /* notify upper layer */
        SoAd_ULTpRxIndication(SocketRouteDestPtr, NTFRSLT_OK);

        Proceed = TRUE;
      }
    }
    else
    {
      /* invalid header, disabled route or upper layer refused the reception -> discard PDU */
      const PduLengthType DiscardedLen = SoAd_RingBufferDiscard(RingBufferAccessPtr, Length);

      SoConRxDataPtr->CurrPduForwardedLen += DiscardedLen;
      /* PDU rejected by UL, reset SocketRouteId to discard following PDU fragments */
      SoConRxDataPtr->CurrSocketRouteId = SOAD_INVALID_ROUTE_ID;

      /* UL shall only be notified if CopyRxData failed */
      if(!StartOfReceptionFailed)
      {
        /* TP only: For TP only 1 Destination is allowed to be configured.
         * Therefore we can use the SocketRouteDestFirstId and do not need to iterate.*/
        P2CONST(SoAd_SocketRouteDestType, AUTOMATIC, SOAD_APPL_DATA) SocketRouteDestPtr =
          &SOAD_CFG(SocketRouteDest, SocketRoutePtr->SocketRouteDestFirstId);
        /* notify upper layer, as remaining PDU fragments are just discarded in
          RxIndication without notifying */
        SoAd_ULTpRxIndication(SocketRouteDestPtr, NTFRSLT_E_NOT_OK);
      }

      if(SoConRxDataPtr->CurrPduForwardedLen == SoConRxDataPtr->CurrPduLen)
      {
        /* Complete PDU was discarded. Reset rxData information */
        SoConRxDataPtr->CurrPduLen = 0U;
        SoConRxDataPtr->CurrPduForwardedLen = 0U;

      }

      *TcpMoveWindowLenPtr += DiscardedLen;
    }
  }

  DBG_SOAD_PROCESSTPPDU_HEADERMODE_EXIT
  (
    SoConPtr,
    SoConStatePtr,
    SoConRxDataPtr,
    RingBufferAccessPtr,
    TcpMoveWindowLenPtr
  );
}

STATIC FUNC(void, SOAD_CODE) SoAd_ProcessTpPdu_NonHeaderMode
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(SoAd_RingBufferAccessType, AUTOMATIC, SOAD_APPL_DATA) RingBufferAccessPtr,
  CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) TcpMoveWindowLenPtr
)
{
  CONSTP2CONST(SoAd_SocketRouteType, AUTOMATIC, SOAD_APPL_DATA) SocketRoutePtr
    = &SOAD_CFG(SocketRoute, SoConPtr->SocketRouteFirstId);
  PduInfoType PduInfo = {NULL_PTR, 0U};
  PduLengthType BufferSize = 0U;
  BufReq_ReturnType RetVal_BufReq;

  DBG_SOAD_PROCESSTPPDU_NONHEADERMODE_ENTRY
  (
    SoConPtr,
    SoConStatePtr,
    RingBufferAccessPtr,
    TcpMoveWindowLenPtr
  );

  /* Retrieve available buffer size. */
  RetVal_BufReq = SoAd_ULTpCopyRxData(SocketRoutePtr, &PduInfo, &BufferSize);

  if(RetVal_BufReq != BUFREQ_OK)
  {
    BufferSize = 0U;
  }

  /* NOTE: The following loop is repeated until all buffered PDUs are processed, UL returns busy
   * or an error occurs. Several calls to <UL>_CopyRxData are possible,
   * to simplify the handling of their return values it is done mutually as part of the loop. */
  while(BufferSize > 0U)
  {
    /* avoid to copy more data then available in the ring buffer */
    if(RingBufferAccessPtr->Used < BufferSize)
    {
      BufferSize = RingBufferAccessPtr->Used;
    }

    /* deliver the PDU from the ring buffer to the UL */
    /* cast protected by if(RingBufferAccessPtr->Used < BufferSize)
       RingBufferAccessPtr->Used will be decremented if OK. */
    RetVal_BufReq = SoAd_RingBufferDeliverTp
    (
      RingBufferAccessPtr,
      SocketRoutePtr,
      &BufferSize
    );

    if(RetVal_BufReq == BUFREQ_OK)
    {
      *TcpMoveWindowLenPtr += BufferSize;
    }
    else
    {
      BufferSize = 0U;
    }
  }

  if(RetVal_BufReq != BUFREQ_OK)
  {
    /* Upper layer refused the reception -> discard complete buffer. */
    const PduLengthType DiscardedLen
      = SoAd_RingBufferDiscard(RingBufferAccessPtr, RingBufferAccessPtr->Used);

    /* in that case the socket connection must be reconnected */
    SOAD_FLAG_SET_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RECONNECT_SOCKET);

    /* Update TXING_OK_State */
    SOAD_FLAG_CLR_UP(SoConStatePtr, SOAD_SOCONSTATE_TXING_OK);

    /* Set global flag to know that for a certain SoCon a transition condition was met. */
    SoAd_SetBitInTransCondChange(SoConPtr);

    *TcpMoveWindowLenPtr += DiscardedLen;
  }

  DBG_SOAD_PROCESSTPPDU_NONHEADERMODE_EXIT
  (
    SoConPtr,
    SoConStatePtr,
    RingBufferAccessPtr,
    TcpMoveWindowLenPtr
  );
}

STATIC FUNC(boolean, SOAD_CODE) SoAd_MessageAcceptanceFilter
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) RemoteAddrPtr,
  SoAd_AddressMatchType AddressMatch
)
{
  boolean RetVal = FALSE;

  DBG_SOAD_MESSAGEACCEPTANCEFILTER_ENTRY(SoConPtr, SoConStatePtr, RemoteAddrPtr, AddressMatch);

  if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_NO_MSG_FILTER))
  {
    RetVal = TRUE;
  }
  else
  {
    CONSTP2CONST(SoAd_SoConGroupType, AUTOMATIC, SOAD_APPL_DATA) SoConGroupPtr =
            &SOAD_CFG(SoConGroup, SoConPtr->SoConGroupId);

    switch(AddressMatch)
    {
      case SOAD_ADDRESSMATCH_WILDCARD: /* fall through */
      case SOAD_ADDRESSMATCH_PORT: /* fall through */
      case SOAD_ADDRESSMATCH_IPADDR:
      {
        /* Store address and port in socket connection state */
        SoAd_AssignRemoteAddressSockAddrType2SoCon(SoConPtr, SoConStatePtr, RemoteAddrPtr);

        /* If UdpAlive timer is enabled start timer. */
        /* !LINKSTO SoAd.ASR43.SWS_SoAd_00694, 1 */
        if(SoConGroupPtr->UdpAliveRefTimeout > 0U)
        {
          SoAd_StartAliveSupervisionTimer(SoConGroupPtr, SoConStatePtr);
        }

        RetVal = TRUE;
        break;
      }
      case SOAD_ADDRESSMATCH_EXACT:
      {
        /* If UdpAlive timer is enabled restart timer. */
        /* !LINKSTO SoAd.ASR43.SWS_SoAd_00742, 1 */
        if(SoConGroupPtr->UdpAliveRefTimeout > 0U)
        {
          SoAd_StartAliveSupervisionTimer(SoConGroupPtr, SoConStatePtr);
        }

        RetVal = TRUE;
        break;
      }
      case SOAD_ADDRESSMATCH_NONE:
      {
        /* No match. Return init value. */
        break;
      }
      /* CHECK: NOPARSE */
      default:
      {
        SOAD_UNREACHABLE_CODE_ASSERT(SOAD_MAINFUNCTION_SVCID);
        break; /* Caution: this 'break' is required by MISRA-C:2012 */
      }
      /* CHECK: PARSE */
    }
  }

  DBG_SOAD_MESSAGEACCEPTANCEFILTER_EXIT
  (
    RetVal,
    SoConPtr,
    SoConStatePtr,
    RemoteAddrPtr,
    AddressMatch
  );
  return RetVal;
}


STATIC FUNC(void, SOAD_CODE) SoAd_ReceiveTpPdu
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  SoAd_RouteIdType SocketRouteId,
  CONSTP2VAR(SoAd_RingBufferAccessType, AUTOMATIC, SOAD_APPL_DATA) RingBufferAccessPtr,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) PduHeaderPtr,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  uint16 Length
)
{
  P2VAR(SoAd_SoConRxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConRxDataPtr = NULL_PTR;
  CONSTP2CONST(SoAd_SocketRouteType, AUTOMATIC, SOAD_APPL_DATA) SocketRoutePtr =
    &SOAD_CFG(SocketRoute, SocketRouteId);
  SoAd_RefToRAM StoreInRingBufferLen;
  P2VAR(uint8,AUTOMATIC, SOAD_APPL_DATA) StoreInRingBufferSrcPtr = NULL_PTR;
  P2VAR(uint8,AUTOMATIC, SOAD_APPL_DATA) StoreInRingBufferHeaderPtr = PduHeaderPtr;
  uint32 TcpMoveWindowLen = 0U;
  boolean StoreRemainingPduFragment = FALSE;
  const boolean IsTcp = SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_TCP) ? TRUE : FALSE;

  SoAd_GetSoConRxData(SoConPtr, SoConRxDataPtr);
  DBG_SOAD_RECEIVETPPDU_ENTRY
  (
    SoConPtr,
    SoConStatePtr,
    SocketRouteId,
    RingBufferAccessPtr,
    PduHeaderPtr,
    BufPtr,
    Length
  );

  /* do not process this PDU if there is already data buffered
  * Note: It's important to use the ring buffer macro here instead of the
  * SOAD_SOCONSTATE_FLAG_RX_DATA_BUFFERED flag, because the flag is not set/reset
  * until the next call of SoAd_RingBufferSaveAccess. */
  if(SOAD_RINGBUFFER_USED(RingBufferAccessPtr) == 0U)
  {
    PduLengthType BufferSize = 0U;
    uint16 RemainingLength = Length;
    BufReq_ReturnType RetVal_BufReq;
    boolean StartOfReceptionFailed = FALSE;

    /* valid header pointer this means this is the start of a new PDU */
    if(PduHeaderPtr != NULL_PTR)
    {
      /* indicate start of reception to upper layer */
      RetVal_BufReq = SoAd_ULTpStartofReception
      (
        SoConPtr,
        SocketRoutePtr,
        (PduLengthType)SoConRxDataPtr->CurrPduLen,
        &BufferSize
      );

      if(BUFREQ_OK != RetVal_BufReq)
      {
        StartOfReceptionFailed = TRUE;
      }

      TcpMoveWindowLen += SOAD_PDU_HEADER_LEN;

    }
    else
    {
      PduInfoType PduInfo = {NULL_PTR, 0U};

      /* query available buffer from upper layer */
      RetVal_BufReq = SoAd_ULTpCopyRxData(SocketRoutePtr, &PduInfo, &BufferSize);
    }
    if(RetVal_BufReq == BUFREQ_OK)
    {
      RetVal_BufReq = SoAd_ProcessRxData
      (
          Length, &BufferSize, &RemainingLength, &TcpMoveWindowLen,
          BufPtr, SocketRoutePtr, SoConPtr
      );
    }

    if(RetVal_BufReq == BUFREQ_OK)
    {
      /* in case the PduHeaderMode is enabled check if the PDU was completely
       * delivered to the upper layer */
      if(
          (SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_PDU_HEADER_MODE))
          && (SoConRxDataPtr->CurrPduLen == SoConRxDataPtr->CurrPduForwardedLen)
        )
      {
        /* TP only: For TP only 1 Destination is allowed to be configured.
         * Therefore we can use the SocketRouteDestFirstId and do not need to iterate.*/
        P2CONST(SoAd_SocketRouteDestType, AUTOMATIC, SOAD_APPL_DATA) SocketRouteDestPtr =
          &SOAD_CFG(SocketRouteDest, SocketRoutePtr->SocketRouteDestFirstId);
        /* reset rxData information */
        SoConRxDataPtr->CurrSocketRouteId = SOAD_INVALID_ROUTE_ID;

        /* indicate complete reception to upper layer */
        SoAd_ULTpRxIndication(SocketRouteDestPtr, NTFRSLT_OK);

        /* nothing must be copied into the ring buffer */
        StoreInRingBufferLen = 0U;
      }
      else
      {
        /* only the rest of the PDU, that couldn't be copied to UL shall be stored
         * in the ring buffer */
        StoreInRingBufferHeaderPtr = NULL_PTR;
        StoreInRingBufferLen = RemainingLength;
        StoreInRingBufferSrcPtr = &BufPtr[Length - RemainingLength];
        StoreRemainingPduFragment = TRUE;
      }
    }
    else
    {
      /* <UL>_StartofReception or <UL>_CopyRxData returned BUFREQ_E_NOT_OK,
       * if the header mode is disabled the socket connection has to be reopened */
      if(!SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_PDU_HEADER_MODE))
      {
        SOAD_FLAG_SET_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RECONNECT_SOCKET);

        /* Update TXING_OK_State */
        SOAD_FLAG_CLR_UP(SoConStatePtr,SOAD_SOCONSTATE_TXING_OK);

        /* Set global flag to know that for a certain SoCon a transition condition was met. */
        SoAd_SetBitInTransCondChange(SoConPtr);
      }
      else
      {
        if(!StartOfReceptionFailed)
        {
          /* TP only: For TP only 1 Destination is allowed to be configured.
           * Therefore we can use the SocketRouteDestFirstId and do not need to iterate.*/
          P2CONST(SoAd_SocketRouteDestType, AUTOMATIC, SOAD_APPL_DATA) SocketRouteDestPtr =
            &SOAD_CFG(SocketRouteDest, SocketRoutePtr->SocketRouteDestFirstId);
          SoAd_ULTpRxIndication(SocketRouteDestPtr, NTFRSLT_E_NOT_OK);
        }
      }
      StoreInRingBufferLen = 0U;
      SoConRxDataPtr->CurrPduForwardedLen += RemainingLength;
      TcpMoveWindowLen += Length;

      /* reset rxData information */
      SoConRxDataPtr->CurrSocketRouteId = SOAD_INVALID_ROUTE_ID;
    }
  }
  else
  {
    /* Store the PDU data into the ringbuffer, if this is the start of a new PDU, then
     * PduHeaderPtr != NULL_PTR and it will be passed to the SoAd_RingBufferStore as well */
    if(StoreInRingBufferHeaderPtr != NULL_PTR)
    {
      /* Typecast is due to Misra warning if SoAd_RefToRAM is uint32 */
      StoreInRingBufferLen = (SoAd_RefToRAM)Length + (SoAd_RefToRAM)SOAD_PDU_HEADER_LEN;
    }
    else
    {
      StoreInRingBufferLen = Length;
    }
    StoreInRingBufferSrcPtr = &BufPtr[0];

    /* Note: this might be a segment that does not belong to the current but one
     * of the next PDUs. SoAd still must keep track of the PDU Header offsets */
    if(SoConRxDataPtr->NextPduOutstandingLen >= Length)
    {
      SoConRxDataPtr->NextPduOutstandingLen -= Length;
    }
    else
    {
      SoConRxDataPtr->NextPduOutstandingLen = 0U;
    }
  }

  if(StoreInRingBufferLen != 0U)
  {
    const Std_ReturnType RetVal = SoAd_RingBufferStore(
                                                        RingBufferAccessPtr,
                                                        StoreInRingBufferHeaderPtr,
                                                        StoreInRingBufferSrcPtr,
                                                        StoreInRingBufferLen
                                                      );

    if(RetVal == E_NOT_OK)
    {
      /* not enough space in ring buffer available -> discard PDU, continue processing */

      /* in case of a TCP connection this requires a reconnection */
      if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_TCP))
      {
        SOAD_FLAG_SET_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RECONNECT_SOCKET);

        /* Set global flag to know that for a certain SoCon a transition condition was met. */
        SoAd_SetBitInTransCondChange(SoConPtr);
      }
      else if(StoreRemainingPduFragment == TRUE)
      {
        if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_PDU_HEADER_MODE))
        {
          /* TP only: For TP only 1 Destination is allowed to be configured.
           * Therefore we can use the SocketRouteDestFirstId and do not need to iterate.*/
          P2CONST(SoAd_SocketRouteDestType, AUTOMATIC, SOAD_APPL_DATA) SocketRouteDestPtr =
            &SOAD_CFG(SocketRouteDest, SocketRoutePtr->SocketRouteDestFirstId);
          /* reset rxData information */
          SoConRxDataPtr->CurrSocketRouteId = SOAD_INVALID_ROUTE_ID;

          SoAd_ULTpRxIndication(SocketRouteDestPtr, NTFRSLT_E_NOT_OK);
        }
      }
      else
      {
        /* Drop PDU */
      }
#if(SOAD_DEV_ERROR_DETECT == STD_ON)
      SOAD_DET_REPORT_ERROR(SOAD_RXINDICATION_SVCID, SOAD_E_NOBUFS);
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
    }
  }

  if(IsTcp && (TcpMoveWindowLen > 0U))
  {
    (void)TcpIp_TcpReceived(SoConStatePtr->SocketId, TcpMoveWindowLen);
  }

  DBG_SOAD_RECEIVETPPDU_EXIT
  (
    SoConPtr,
    SoConStatePtr,
    SocketRouteId,
    RingBufferAccessPtr,
    PduHeaderPtr,
    BufPtr,
    Length
  );
}

STATIC FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_ProcessRxData
(
  uint16 Length,
  P2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_DATA) BufferSize,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_DATA) RemainingLength,
  P2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) TcpMoveWindowLen,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  CONSTP2CONST(SoAd_SocketRouteType, AUTOMATIC, SOAD_APPL_DATA) SocketRoutePtr,
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr
)
{

  BufReq_ReturnType RetValBufReq = BUFREQ_OK;
  P2VAR(SoAd_SoConRxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConRxDataPtr = NULL_PTR;
  SoAd_GetSoConRxData(SoConPtr, SoConRxDataPtr);
  while ((RetValBufReq == BUFREQ_OK) && (*BufferSize > 0U) && (*RemainingLength > 0))
  {
    PduInfoType PduInfo;
    PduLengthType ProcessedSduLength;

    if(*BufferSize > *RemainingLength)
    {
      *BufferSize = *RemainingLength;
    }
    ProcessedSduLength = *BufferSize;
    PduInfo.SduLength = *BufferSize;
    PduInfo.SduDataPtr = &BufPtr[Length - *RemainingLength];

    RetValBufReq = SoAd_ULTpCopyRxData(SocketRoutePtr, &PduInfo, BufferSize);

    if(RetValBufReq == BUFREQ_OK)
    {
      if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_TCP))
      {
        *TcpMoveWindowLen += ProcessedSduLength;
      }
      SoConRxDataPtr->CurrPduForwardedLen += ProcessedSduLength;
      /* cast protected by if(*BufferSize > *RemainingLength) */
      *RemainingLength -= (uint16)ProcessedSduLength;
    }
  }
  return RetValBufReq;
}

STATIC FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_ULTpCopyRxData
(
  CONSTP2CONST(SoAd_SocketRouteType, AUTOMATIC, SOAD_APPL_DATA) SocketRoutePtr,
  CONSTP2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_CONST) PduInfoPtr,
  CONSTP2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_DATA) BufferSizePtr
)
{
  /* TP only: For TP only 1 Destination is allowed to be configured.
   * Therefore we can use the SocketRouteDestFirstId and do not need to iterate.*/
  P2CONST(SoAd_SocketRouteDestType, AUTOMATIC, SOAD_APPL_DATA) SocketRouteDestPtr =
    &SOAD_CFG(SocketRouteDest, SocketRoutePtr->SocketRouteDestFirstId);
  BufReq_ReturnType RetVal_BufReq = BUFREQ_E_NOT_OK;

  DBG_SOAD_ULTPCOPYRXDATA_ENTRY(SocketRoutePtr, PduInfoPtr, BufferSizePtr);

  SOAD_PRECONDITION_ASSERT(SocketRouteDestPtr->ULFuncId != SOAD_INVALID_ULFUNCID, DET_INTERNAL_API_ID);
  RetVal_BufReq = SoAd_TpModuleAPI[SocketRouteDestPtr->ULFuncId].TpCopyRxData
  (
    SocketRouteDestPtr->DestPduId,
    PduInfoPtr,
    BufferSizePtr
  );

  /* Feature not fully supported: BUFREQ_E_BUSY */
  if(RetVal_BufReq == BUFREQ_E_BUSY)
  {
    /* if TpCopyRxData is called to query the available buffer,
     * BUFREQ_E_BUSY can be treated as BUFREQ_OK with no available buffer */
    if(PduInfoPtr->SduLength == 0U)
    {
      RetVal_BufReq = BUFREQ_OK;
      *BufferSizePtr = 0U;
    }
    else
    {
      /* otherwise BUFREQ_E_BUSY is mapped to BUFREQ_E_NOT_OK
       * Feature not yet supported */
      RetVal_BufReq = BUFREQ_E_NOT_OK;
    }
  }

  DBG_SOAD_ULTPCOPYRXDATA_EXIT(RetVal_BufReq, SocketRoutePtr, PduInfoPtr, BufferSizePtr);
  return RetVal_BufReq;
}

STATIC FUNC(void, SOAD_CODE) SoAd_ReceiveTpPduUdpNoBuf
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  SoAd_RouteIdType SocketRouteId,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  uint16 Length
)
{
  CONSTP2CONST(SoAd_SocketRouteType, AUTOMATIC, SOAD_APPL_DATA) SocketRoutePtr =
      &SOAD_CFG(SocketRoute, SocketRouteId);
  BufReq_ReturnType RetVal_BufReq = BUFREQ_OK;
  NotifResultType RetVal_Notif = NTFRSLT_E_NOT_OK;
  PduLengthType BufferSize = 0U;
  boolean RxIndication = FALSE;

  DBG_SOAD_RECEIVETPPDUUDPNOBUF_ENTRY(SoConPtr, SoConStatePtr, SocketRouteId, BufPtr, Length);

  /* if the PduHeaderMode is disabled, StartOfReception was already called during SoCon setup */
  if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_PDU_HEADER_MODE))
  {
    RetVal_BufReq =
      SoAd_ULTpStartofReception(SoConPtr, SocketRoutePtr, Length, &BufferSize);

    /* unless the upper layer refused the reception, a final RxIndication must be called */
    if(RetVal_BufReq != BUFREQ_E_NOT_OK)
    {
      RxIndication = TRUE;
    }

    /* if the currently available buffer is not sufficient, the PDU must be discarded */
    if(BufferSize < ((uint16)(Length - SOAD_PDU_HEADER_LEN)))
    {
      RetVal_BufReq = BUFREQ_E_NOT_OK;
    }
  }
  else
  {
    PduInfoType PduInfo = {NULL_PTR, 0U};

    /* query available buffer from upper layer */
    RetVal_BufReq = SoAd_ULTpCopyRxData(SocketRoutePtr, &PduInfo, &BufferSize);

    /* if BUFREQ_E_NOT_OK is return, socket connection shall be closed (SWS_SoAd_00570) */
    if(RetVal_BufReq == BUFREQ_E_NOT_OK)
    {
      SOAD_FLAG_SET_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RECONNECT_SOCKET);

      /* Clear TXING_OK_State */
      SOAD_FLAG_CLR_UP(SoConStatePtr,SOAD_SOCONSTATE_TXING_OK);

      /* Set global flag to know that for a certain SoCon a transition condition was met. */
      SoAd_SetBitInTransCondChange(SoConPtr);
    }

    /* if the currently available buffer is not sufficient, the PDU must be discarded */
    if(BufferSize < Length)
    {
      RetVal_BufReq = BUFREQ_E_BUSY;
    }
  }

  /* do not call CopyRxData if the UL refused the reception or
   * not enough the available buffer is not sufficient */
  if(RetVal_BufReq == BUFREQ_OK)
  {
    PduInfoType pduInfo;

    pduInfo.SduDataPtr = BufPtr;
    pduInfo.SduLength = Length;

    RetVal_BufReq = SoAd_ULTpCopyRxData(SocketRoutePtr, &pduInfo, &BufferSize);
  }

  /* the reception only succeeded if CopyRxData returned BUFREQ_OK */
  if(RetVal_BufReq == BUFREQ_OK)
  {
    RetVal_Notif = NTFRSLT_OK;
  }

  /* if StartOfReception was called successfully, RxIndication must be called too */
  if(RxIndication == TRUE)
  {
    /* TP only: For TP only 1 Destination is allowed to be configured.
     * Therefore we can use the SocketRouteDestFirstId and do not need to iterate.*/
    P2CONST(SoAd_SocketRouteDestType, AUTOMATIC, SOAD_APPL_DATA) SocketRouteDestPtr =
      &SOAD_CFG(SocketRouteDest, SocketRoutePtr->SocketRouteDestFirstId);
    SoAd_ULTpRxIndication(SocketRouteDestPtr, RetVal_Notif);
  }

  DBG_SOAD_RECEIVETPPDUUDPNOBUF_EXIT(SoConPtr, SoConStatePtr, SocketRouteId, BufPtr, Length);
}

STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_RingBufferStore
(
  CONSTP2VAR(SoAd_RingBufferAccessType, AUTOMATIC, SOAD_APPL_DATA) RingBufPtr,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) SrcHeaderBufPtr,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) SrcPayloadBufPtr,
  SoAd_RefToRAM Length
)
{
  Std_ReturnType RetVal;

  DBG_SOAD_RINGBUFFERSTORE_ENTRY(RingBufPtr, SrcHeaderBufPtr, SrcPayloadBufPtr, Length);

  /* check remaining buffer Size */
  if(SOAD_RINGBUFFER_FREE(RingBufPtr) >= Length)
  {
    /* get remaining continuous buffer Size */
    SoAd_RefToRAM FreeContinuousLength;
    SoAd_RefToRAM SrcLength = Length;
    SoAd_RefToRAM TmpLen;

    /* Check if a header needs to be copied before payload */
    if(SrcHeaderBufPtr != NULL_PTR)
    {
      FreeContinuousLength = SOAD_RINGBUFFER_CONTFREE(RingBufPtr);
      TmpLen = SOAD_PDU_HEADER_LEN;

      /* if there is not enough continuous free space */
      if(FreeContinuousLength < TmpLen)
      {
        /* copy a first part into the ring buffer */
        TS_MemCpy(SOAD_RINGBUFFER_INPTR(RingBufPtr), SrcHeaderBufPtr, FreeContinuousLength);

        /* In pointer wrap around */
        RingBufPtr->Used += FreeContinuousLength;
        RingBufPtr->In = 0U;
        TmpLen -= FreeContinuousLength;
      }

      /* copy data into the ring buffer */
      TS_MemCpy(
                 SOAD_RINGBUFFER_INPTR(RingBufPtr),
                 &(SrcHeaderBufPtr[SOAD_PDU_HEADER_LEN-TmpLen]),
                 TmpLen
               );
      RingBufPtr->Used += TmpLen;
      RingBufPtr->In += TmpLen;
      /* Reduce Length as Header is already copied into ringbuffer */
      SrcLength -= SOAD_PDU_HEADER_LEN;
    }

    /* Copy the payload into the ringbuffer */
    FreeContinuousLength = SOAD_RINGBUFFER_CONTFREE(RingBufPtr);
    TmpLen = SrcLength;

    /* if there is not enough continuous free space */
    if(FreeContinuousLength < TmpLen)
    {
      /* copy a first part into the ring buffer */
      TS_MemCpy(SOAD_RINGBUFFER_INPTR(RingBufPtr), SrcPayloadBufPtr, FreeContinuousLength);

      /* In pointer wrap around */
      RingBufPtr->Used += FreeContinuousLength;
      RingBufPtr->In = 0U;
      TmpLen -= FreeContinuousLength;
    }

    /* copy data into the ring buffer */
    TS_MemCpy(SOAD_RINGBUFFER_INPTR(RingBufPtr), &(SrcPayloadBufPtr[SrcLength-TmpLen]), TmpLen);
    RingBufPtr->Used += TmpLen;
    RingBufPtr->In += TmpLen;

    RetVal = E_OK;
  }
  else
  {
    /* not enough space In the ring buffer */
    RetVal = E_NOT_OK;
  }

  DBG_SOAD_RINGBUFFERSTORE_EXIT(RetVal, RingBufPtr, SrcHeaderBufPtr, SrcPayloadBufPtr, Length);
  return RetVal;
}

STATIC FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_RingBufferDeliverTp
(
  CONSTP2VAR(SoAd_RingBufferAccessType, AUTOMATIC, SOAD_APPL_DATA) RingBufPtr,
  CONSTP2CONST(SoAd_SocketRouteType, AUTOMATIC, SOAD_APPL_DATA) SocketRoutePtr,
  CONSTP2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_DATA) BufferSizePtr
)
{
  BufReq_ReturnType RetVal_BufReq;

  /* get available continuous buffer Size */
  SoAd_RefToRAM UsedContinuousLength = SOAD_RINGBUFFER_CONTUSED(RingBufPtr);
  /* Cast is protected since Length is max used buffer. */
  SoAd_RefToRAM RemainingLength = (SoAd_RefToRAM)*BufferSizePtr;
  PduInfoType PduInfo;
  PduLengthType AvailableBufferSize = 0U;


  DBG_SOAD_RINGBUFFERDELIVERTP_ENTRY(RingBufPtr, SocketRoutePtr, Length);

  /* prepare PduInfo, all data shall be copied at once */
  PduInfo.SduLength = *BufferSizePtr;
  PduInfo.SduDataPtr = SOAD_RINGBUFFER_OUTPTR(RingBufPtr);


  /* if the data is not fully continuous, two TpCopyRxData calls are necessary */
  if(UsedContinuousLength < RemainingLength)
  {
    /* update PduInfo */
    PduInfo.SduLength = (PduLengthType)UsedContinuousLength;
    PduInfo.SduDataPtr = SOAD_RINGBUFFER_OUTPTR(RingBufPtr);

    /* deliver the first part */
    RetVal_BufReq = SoAd_ULTpCopyRxData(SocketRoutePtr, &PduInfo, &AvailableBufferSize);

    if(RetVal_BufReq == BUFREQ_OK)
    {
      /* Out pointer wrap around */
      RingBufPtr->Out = 0U;
      RingBufPtr->Used -= UsedContinuousLength;

      /* update PduInfo, only remaining data shall be copied */
      RemainingLength = RemainingLength - UsedContinuousLength;
    }
  }
  else
  {
    RetVal_BufReq = BUFREQ_OK;
  }


  /* if the first call to SoAd_ULTpCopyRxData failed, don't call a second time */
  if(RetVal_BufReq == BUFREQ_OK)
  {
    /* prepare PduInfo, all data shall be copied at once */
    PduInfo.SduLength = (PduLengthType)RemainingLength;
    PduInfo.SduDataPtr = SOAD_RINGBUFFER_OUTPTR(RingBufPtr);

    /* deliver to the upper layer */
    RetVal_BufReq = SoAd_ULTpCopyRxData(SocketRoutePtr, &PduInfo, &AvailableBufferSize);

    if(RetVal_BufReq == BUFREQ_OK)
    {
      /* either data has been copied to the UL successfully or the PDU shall be discarded.
       * in any case it is removed from the ring buffer. */
      /* cast is protected by PduInfo.SduLength <= Length */
      RingBufPtr->Out += (SoAd_RefToRAM)PduInfo.SduLength;
      RingBufPtr->Used -= (SoAd_RefToRAM)PduInfo.SduLength;
      *BufferSizePtr = AvailableBufferSize;
    }
  }

  /* wrap around if the Out pointer has reached the end of the ring buffer */
  if(RingBufPtr->Out == RingBufPtr->Size)
  {
    RingBufPtr->Out = 0U;
  }

  DBG_SOAD_RINGBUFFERDELIVERTP_EXIT(RetVal_BufReq, RingBufPtr, SocketRoutePtr, Length);
  return RetVal_BufReq;
}

STATIC FUNC(PduLengthType, SOAD_CODE) SoAd_RingBufferDiscard
(
  CONSTP2VAR(SoAd_RingBufferAccessType, AUTOMATIC, SOAD_APPL_DATA) RingBuf,
  PduLengthType Length
)
{
  PduLengthType DiscardedLen;

  DBG_SOAD_RINGBUFFERDISCARD_ENTRY(RingBuf, Length);

  /* check available buffer Size */
  if(SOAD_RINGBUFFER_USED(RingBuf) >= Length)
  {
    /* skip over the requested length */
    /* cast protected by if(SOAD_RINGBUFFER_USED(RingBuf) >= Length) */
    RingBuf->Out = (RingBuf->Out +  (SoAd_RefToRAM)Length) % (RingBuf->Size);
    RingBuf->Used -= (SoAd_RefToRAM)Length;

    /* set return value */
    DiscardedLen = Length;
  }
  else /* not enough data in ring buffer */
  {
    /* set return value */
    /* Cast is protected because SOAD_RINGBUFFER_USED(RingBuf) < Length */
    DiscardedLen = (PduLengthType)RingBuf->Used;

    /* flush the whole buffer */
    RingBuf->In = 0U;
    RingBuf->Out = 0U;
    RingBuf->Used = 0U;
  }

  DBG_SOAD_RINGBUFFERDISCARD_EXIT(DiscardedLen, RingBuf, Length);
  return DiscardedLen;
}

STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_RingBufferLoadAccess
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(SoAd_RingBufferAccessType, AUTOMATIC, SOAD_APPL_DATA) RingBufferAccessPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_SOAD_RINGBUFFERLOADACCESS_ENTRY(SoConPtr, SoConStatePtr, RingBufferAccessPtr);

  RingBufferAccessPtr->Size = 0U;
  RingBufferAccessPtr->In = 0U;
  RingBufferAccessPtr->Out = 0U;
  RingBufferAccessPtr->BufPtr = NULL_PTR;
  RingBufferAccessPtr->Used = 0U;

  if(SoConPtr->RxDataId != SOAD_INVALID_SOCON_ID)
  {
    P2VAR(SoAd_SoConRxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConRxDataPtr = NULL_PTR;
    CONSTP2CONST(SoAd_SoConRxBufferCfgType, AUTOMATIC, SOAD_APPL_DATA) RxBufferCfgPtr =
      &SOAD_CFG(SoConRxBufferCfg, SoConPtr->RxDataId);

    SoAd_GetSoConRxData(SoConPtr, SoConRxDataPtr);
    if(RxBufferCfgPtr->TpCapacity > 0U)
    {
      /* Cast OK as generator ensures that Capacity is small enough to fit into SoAd_RefToRAM */
      RingBufferAccessPtr->Size = (SoAd_RefToRAM)RxBufferCfgPtr->TpCapacity;
      /* they are assigned to each other , but should be of the same type */
      RingBufferAccessPtr->In = (SoAd_RefToRAM)SoConRxDataPtr->TpIn;
      RingBufferAccessPtr->Out = (SoAd_RefToRAM)SoConRxDataPtr->TpOut;
      RingBufferAccessPtr->BufPtr = &SOAD_PBRAM(SoAdBuffer, RxBufferCfgPtr->BufId);
      RingBufferAccessPtr->Used =
        ((RingBufferAccessPtr->In + RingBufferAccessPtr->Size) - RingBufferAccessPtr->Out)
        % RingBufferAccessPtr->Size;

      if(
          SOAD_FLAG_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RX_DATA_BUFFERED)
          && (RingBufferAccessPtr->Used == 0U)
        )
      {
        RingBufferAccessPtr->Used = RingBufferAccessPtr->Size;
      }

      /* access successfully loaded */
      RetVal = E_OK;
    }
  }

  DBG_SOAD_RINGBUFFERLOADACCESS_EXIT(RetVal, SoConPtr, SoConStatePtr, RingBufferAccessPtr);
  return RetVal;
}

STATIC FUNC(void, SOAD_CODE) SoAd_RingBufferSaveAccess
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(SoAd_RingBufferAccessType, AUTOMATIC, SOAD_APPL_DATA) RingBufferAccessPtr
)
{
  DBG_SOAD_RINGBUFFERSAVEACCESS_ENTRY(SoConPtr, SoConStatePtr, RingBufferAccessPtr);

  /* Do nothing if no Rx buffer exist */
  if(SoConPtr->RxDataId != SOAD_INVALID_SOCON_ID)
  {
    P2VAR(SoAd_SoConRxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConRxDataPtr = NULL_PTR;
    SoAd_GetSoConRxData(SoConPtr, SoConRxDataPtr);

    SoConRxDataPtr->TpIn = (SoAd_PduLengthType) RingBufferAccessPtr->In;
    SoConRxDataPtr->TpOut = (SoAd_PduLengthType) RingBufferAccessPtr->Out;

    if(RingBufferAccessPtr->Used == 0U)
    {
      SOAD_FLAG_CLR_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RX_DATA_BUFFERED);
    }
    else
    {
      SOAD_FLAG_SET_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RX_DATA_BUFFERED);
      SoAd_SetBitInBitMap(SoConPtr, &SoAd_PBcfg->SoConRxNotIdleBitMapBase, &SoAd_RxCondChange);
    }
  }

  DBG_SOAD_RINGBUFFERSAVEACCESS_EXIT(SoConPtr, SoConStatePtr, RingBufferAccessPtr);
}

STATIC FUNC(void, SOAD_CODE) SoAd_ULIfRxIndication
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2CONST(SoAd_SocketRouteDestType, AUTOMATIC, SOAD_APPL_DATA) SocketRouteDestPtr,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  PduLengthType Length
)
{
  PduInfoType PduInfo;
  DBG_SOAD_ULIFRXINDICATION_ENTRY(SoConPtr, SocketRoutePtr, BufPtr, Length);

  PduInfo.SduDataPtr = BufPtr;
  PduInfo.SduLength = Length;

#if(SOAD_META_DATA_HANDLING == STD_ON)
  {
    CONSTP2CONST(SoAd_SocketRouteType, AUTOMATIC, SOAD_APPL_DATA) SocketRoutePtr =
      &SOAD_CFG(SocketRoute, SocketRouteDestPtr->SocketRouteId);
    if(SOAD_FLAG(SocketRoutePtr, SOAD_SOCKETROUTE_FLAG_METADATA_SOCON_ID))
    {
      /* No error handling expected. Data will be provided to UL even if EcuC does not accept setting
      of Meta data. */
      (void) EcuC_SetMetaDataSoConId(SocketRouteDestPtr->EcuCPduId, &PduInfo, SoConPtr->SoConId);
    }
  }
#else
  TS_PARAM_UNUSED(SoConPtr);
#endif

  SOAD_PRECONDITION_ASSERT(SocketRouteDestPtr->ULFuncId != SOAD_INVALID_ULFUNCID, DET_INTERNAL_API_ID);
  SoAd_IfModuleAPI[SocketRouteDestPtr->ULFuncId].IfRxIndication
  (
    SocketRouteDestPtr->DestPduId,
    &PduInfo
  );

  DBG_SOAD_ULIFRXINDICATION_EXIT(SoConPtr, SocketRoutePtr, BufPtr, Length);
}

STATIC FUNC(void, SOAD_CODE) SoAd_RxProcessTcpPduSegment
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  CONSTP2VAR(uint16, AUTOMATIC, SOAD_APPL_DATA) Length,
  SoAd_SoConIdType SoConGroupFirstIdOffset
)
{
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) CurrBufPtr = BufPtr;

  P2VAR(SoAd_SoConRxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConRxDataPtr = NULL_PTR;

  CONSTP2CONST(SoAd_SoConRxBufferCfgType, AUTOMATIC, SOAD_APPL_DATA) RxBufferCfg =
    &SOAD_CFG(SoConRxBufferCfg, SoConPtr->RxDataId);
  /* The If buffer is placed after the Tp buffer. No explicit pointer to If buffer available. */
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) RxIfBufferPtr =
    &SOAD_PBRAM(SoAdBuffer, (RxBufferCfg->BufId + RxBufferCfg->TpCapacity));

  SoAd_GetSoConRxData(SoConPtr, SoConRxDataPtr);

  DBG_SOAD_RXPROCESSTCPPDUSEGMENT_ENTRY(SoConPtr, SoConStatePtr, BufPtr, Length);

  SOAD_INVARIANT_ASSERT(RxBufferCfg->IfCapacity > 0U, SOAD_INTERNAL_SVCID);

  if(SoConRxDataPtr->IfIn < SOAD_PDU_HEADER_LEN)
  {
    uint8 PduHeaderRemainingLen = (uint8)(SOAD_PDU_HEADER_LEN - SoConRxDataPtr->IfIn);

    if(PduHeaderRemainingLen > *Length)
    {
      PduHeaderRemainingLen = (uint8) *Length;
    }

    /* store part of the If-Pdu in the linear buffer */
    TS_MemCpy(&(RxIfBufferPtr[SoConRxDataPtr->IfIn]), CurrBufPtr, PduHeaderRemainingLen);
    SoConRxDataPtr->IfIn += PduHeaderRemainingLen;
    *Length -= PduHeaderRemainingLen;
    CurrBufPtr = &CurrBufPtr[PduHeaderRemainingLen];
  }

  if(SoConRxDataPtr->IfIn >= SOAD_PDU_HEADER_LEN)
  {
    Std_ReturnType RetVal;
    SoAd_RouteIdType SocketRouteId = 0U;
    uint32 PduHeaderId;
    PduLengthType LengthInHeader;

    /* header fully received, clear segmentation flag */
    SOAD_FLAG_CLR_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RX_TCP_SEGMENTED);

    RetVal = SoAd_ExtractPduHeader(
                                     SoConPtr,
                                     RxIfBufferPtr,
                                     &SocketRouteId,
                                     &PduHeaderId,
                                     &LengthInHeader
                                   );
    /* check if the PduHeader is valid, and the routing group of the related socket route is
       enabled */
    if(RetVal == E_OK)
    {
      if(SoAd_IsSocketRouteDestEnabled(&SOAD_CFG(SocketRoute, SocketRouteId), SoConGroupFirstIdOffset))
      {
        RetVal = E_OK;
      }
      else
      {
        RetVal = E_NOT_OK;
      }
    }

    if(RetVal == E_OK)
    {
      CONSTP2CONST(SoAd_SocketRouteType, AUTOMATIC, SOAD_APPL_DATA) SocketRoutePtr =
          &SOAD_CFG(SocketRoute, SocketRouteId);

      /* if this is actually a TP Pdu */
      if(SOAD_FLAG(SocketRoutePtr, SOAD_SOCKETROUTE_FLAG_IS_UL_TP))
      {
        PduLengthType RemainingLen = SOAD_MIN(LengthInHeader, *Length);

        /* assemble RingBufferAccess */
        SoAd_RingBufferAccessType RingBufferAccess;
        /* SoAd_RingBufferLoadAccess() always returns OK since TCP TP always has a ring buffer. */
        (void)SoAd_RingBufferLoadAccess(SoConPtr, SoConStatePtr, &RingBufferAccess);

        /* do not override the current PDU settings if there is already data buffered
         * Note: It's important to use the ring buffer macro here instead of the
         * SOAD_SOCONSTATE_FLAG_RX_DATA_BUFFERED flag, because the flag is not set/reset
         * until the end of the this function in the context of SoAd_RingBufferSaveAccess. */
        if(SOAD_RINGBUFFER_USED(&RingBufferAccess) == 0U)
        {
          SoConRxDataPtr->CurrPduHeaderId = PduHeaderId;
          SoConRxDataPtr->CurrPduLen = LengthInHeader;
          SoConRxDataPtr->CurrPduForwardedLen = 0U;
          SoConRxDataPtr->CurrSocketRouteId = SocketRouteId;
        }
        else
        {
          /* remember how much of the PDU was already received, this is important to keep track of
           * the ending of this PDU and the start of the next PDU-Header */
          if(LengthInHeader > RemainingLen)
          {
            /* set NextPduOutstandingLen to the length of the whole PDU,
             * received bytes will be subtracted within SoAd_ReceiveTpPdu */
            SoConRxDataPtr->NextPduOutstandingLen = LengthInHeader;
          }
        }

        SoAd_ReceiveTpPdu(
                           SoConPtr,
                           SoConStatePtr,
                           SocketRouteId,
                           &RingBufferAccess,
                           RxIfBufferPtr,
                           CurrBufPtr,
                           /* cast protected by SOAD_MIN(LengthInHeader, *Length);
                              where Length is uint16*/
                           (uint16)RemainingLen
                         );

        SoAd_RingBufferSaveAccess(SoConPtr, SoConStatePtr, &RingBufferAccess);

        /* remember data has already been processed */
        /* cast protected by SOAD_MIN(LengthInHeader, *Length);
           where Length is uint16*/
        *Length -= (uint16)RemainingLen;

        /* empty linear buffer */
        SoConRxDataPtr->IfIn = 0U;
      }
      else /* If-Pdu */
      {
        /* still more data missing for complete reception */
        if((SOAD_PDU_HEADER_LEN + LengthInHeader) > (SoConRxDataPtr->IfIn + *Length))
        {
          /* only copy data if the PDU is not larger then the buffer capacity */
          if(RxBufferCfg->IfCapacity >= (LengthInHeader + SOAD_PDU_HEADER_LEN))
          {
            /* store part of the If-Pdu in the linear buffer */
            TS_MemCpy(&(RxIfBufferPtr[SoConRxDataPtr->IfIn]), CurrBufPtr, *Length);
          }

          /* count the data anyway, even if nothing was copied,
           * to keep track of the PDU Header offsets */
          SoConRxDataPtr->IfIn += *Length;

          /* set segmentation flag, PDU not fully received */
          SOAD_FLAG_SET_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RX_TCP_SEGMENTED);

          /* remember data has already been processed */
          *Length = 0U;
        }
        else /* the If-Pdu is complete */
        {
          /* only process the PDU if it was not larger then the buffer capacity */
          if(RxBufferCfg->IfCapacity >= (LengthInHeader + SOAD_PDU_HEADER_LEN))
          {
            /* copy the remaining payload into the linear buffer */
            TS_MemCpy(
                       &(RxIfBufferPtr[SoConRxDataPtr->IfIn]),
                       CurrBufPtr,
                       (uint16)(LengthInHeader - (SoConRxDataPtr->IfIn - SOAD_PDU_HEADER_LEN))
                     );

            /* indicate reception to upper layer */
            SoAd_ULIfRxIndication(
                                   SoConPtr,
                                   &SOAD_CFG(SocketRouteDest,
                                             SocketRoutePtr->SocketRouteDestFirstId
                                            ),
                                   &(RxIfBufferPtr[SOAD_PDU_HEADER_LEN]),
                                   LengthInHeader
                                 );
          }

          (void)TcpIp_TcpReceived(
                                   SoConStatePtr->SocketId,
                                   (uint32)((uint32)LengthInHeader + (uint32)SOAD_PDU_HEADER_LEN)
                                 );

          /* clear segmentation flag */
          SOAD_FLAG_CLR_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RX_TCP_SEGMENTED);
          /* remember data has already been processed */
          /* cast protected by if((SOAD_PDU_HEADER_LEN + LengthInHeader) > (SoConRxDataPtr->IfIn
             + *Length))*/
          *Length -= (uint16)(LengthInHeader-(SoConRxDataPtr->IfIn-SOAD_PDU_HEADER_LEN));

          /* empty linear buffer */
          SoConRxDataPtr->IfIn = 0U;

        }
      }
    }
    else
    {
      /* The received PduHeaderId is either unknown or the related routing group is disabled,
       * it shall be discarded. */


      /* check if the whole Pdu was received */
      if(*Length >= LengthInHeader)
      {
        /* processing shall be continued, another Pdu might have been received */
        /* cast protected by if(*Length >= LengthInHeader) */
        *Length -= (uint16)LengthInHeader;

        /* the received but discarded amount of data shall still be acknowledged */
        (void)TcpIp_TcpReceived(
                                 SoConStatePtr->SocketId,
                                 (uint32)((uint32)SOAD_PDU_HEADER_LEN + (uint32)LengthInHeader)
                               );

        /* empty linear buffer */
        SoConRxDataPtr->IfIn = 0U;
      }
      else
      {
        /* If only a part of the Pdu was received, the remote node shall be informed
           by resetting the Tcp connection. It does not make sense to continue the reception
           of data that is irrelevant for the upper layer. */

        SOAD_FLAG_SET_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RECONNECT_SOCKET);

        /* set the remaining Length to 0 to stop further processing */
        *Length = 0U;

        /* Set global flag to know that for a certain SoCon a transition condition was met. */
        SoAd_SetBitInTransCondChange(SoConPtr);
      }

    }
  }

  DBG_SOAD_RXPROCESSTCPPDUSEGMENT_EXIT(SoConPtr, SoConStatePtr, BufPtr, Length);
}


STATIC FUNC(void, SOAD_CODE) SoAd_RxProcessTcpPduHeaders
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  uint16 Length,
  SoAd_SoConIdType SoConGroupFirstIdOffset
)
{
  if(SoConPtr->RxDataId != SOAD_INVALID_SOCON_ID)
  {
    uint16 RemainingLen = Length;
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) CurrBufPtr = BufPtr;
    P2VAR(SoAd_SoConRxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConRxDataPtr = NULL_PTR;

    /* assemble RingBufferAccess */
    SoAd_RingBufferAccessType RingBufferAccess;

    SoAd_GetSoConRxData(SoConPtr, SoConRxDataPtr);

    (void)SoAd_RingBufferLoadAccess(SoConPtr, SoConStatePtr, &RingBufferAccess);

    /* Note: before processing this frame, it must be checked where the next SoAdHeader begins,
     * on TCP connections this might not be at the beginning of this frame because the current
     * or one of the following PDUs is not yet fully received and stored in the ringBuffer.
     * TCP Segmentation of PduHeaders or If-PDUs is already handled in SoAd_RxProcessTcpPduSegment */

    /* check where the next PduHeader begins */
    /* if there is a piece of this PDU missing */
    /* or if the next PDU is only partly buffered */
    if(
        ((SoConRxDataPtr->CurrPduLen - SoConRxDataPtr->CurrPduForwardedLen) > RingBufferAccess.Used)
        || (SoConRxDataPtr->NextPduOutstandingLen != 0U)
      )
    {
      SoAd_PduLengthType SegmentLength;

      /* check if there is a segment of the current PDU missing */
      if((SoConRxDataPtr->CurrPduLen - SoConRxDataPtr->CurrPduForwardedLen) > RingBufferAccess.Used)
      {
        SegmentLength =
          (SoAd_PduLengthType)(SoConRxDataPtr->CurrPduLen - SoConRxDataPtr->CurrPduForwardedLen);
        SegmentLength -= (SoAd_PduLengthType)RingBufferAccess.Used;
      }
      else /* or one of the next PDUs is segmented */
      {
        SegmentLength = SoConRxDataPtr->NextPduOutstandingLen;
      }

      /* don't process more bytes than received */
      if(SegmentLength > RemainingLen)
      {
        SegmentLength = RemainingLen;
      }

      if(SoConRxDataPtr->CurrSocketRouteId != SOAD_INVALID_ROUTE_ID)
      {
        /* deliver the segment to the upper layer or store it in the ring buffer */
        SoAd_ReceiveTpPdu
        (
          SoConPtr,
          SoConStatePtr,
          SoConRxDataPtr->CurrSocketRouteId,
          &RingBufferAccess,
          NULL_PTR,
          CurrBufPtr,
          (uint16)SegmentLength
        );
      }
      else
      {
        /* PDU was rejected by upper layer -> discard data. */
        SoConRxDataPtr->CurrPduForwardedLen += SegmentLength;
        /* the received but discarded amount of data shall still be acknowledged */
        (void)TcpIp_TcpReceived(SoConStatePtr->SocketId, SegmentLength);
      }

      /* move buffer pointer and RemainingLen in case there more data is available in this frame */
      CurrBufPtr = &CurrBufPtr[SegmentLength];
      RemainingLen -= (uint16)SegmentLength;
    }

    /* Note: the next bytes in the frame (CurrBufPtr) are a PduHeader */

    /* process until the end of the frame or an error occurs */
    while(RemainingLen > 0U)
    {
      if(RemainingLen < SOAD_PDU_HEADER_LEN)
      {
        CONSTP2CONST(SoAd_SoConRxBufferCfgType, AUTOMATIC, SOAD_APPL_DATA) RxBufferCfgPtr =
          &SOAD_CFG(SoConRxBufferCfg, SoConPtr->RxDataId);
        CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) RxIfBufferPtr =
          &SOAD_PBRAM(SoAdBuffer, (RxBufferCfgPtr->BufId + RxBufferCfgPtr->TpCapacity));

        SOAD_INVARIANT_ASSERT(RxBufferCfgPtr->IfCapacity > 0U, SOAD_INTERNAL_SVCID);

        /* store part of the If-Pdu in the linear buffer */
        TS_MemCpy(&(RxIfBufferPtr[SoConRxDataPtr->IfIn]), CurrBufPtr, RemainingLen);
        SoConRxDataPtr->IfIn += RemainingLen;
        RemainingLen = 0U;

        /* set the segmentation flag */
        SOAD_FLAG_SET_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RX_TCP_SEGMENTED);
      }
      else
      {
        Std_ReturnType RetVal;
        SoAd_RouteIdType SocketRouteId = SOAD_INVALID_ROUTE_ID;
        uint32 PduHeaderId;
        PduLengthType LengthInHeader;

        RetVal = SoAd_ExtractPduHeader(
                                         SoConPtr,
                                         CurrBufPtr,
                                         &SocketRouteId,
                                         &PduHeaderId,
                                         &LengthInHeader
                                       );
        /* check if the PduHeader is valid, and the routing group of the related socket route is
           enabled */
        if(RetVal == E_OK)
        {
          if(!SoAd_IsSocketRouteDestEnabled(&SOAD_CFG(SocketRoute, SocketRouteId), SoConGroupFirstIdOffset))
          {
            RetVal = E_NOT_OK;
          }
        }

        if(RetVal == E_OK)
        {
          CONSTP2CONST(SoAd_SocketRouteType, AUTOMATIC, SOAD_APPL_DATA) SocketRoutePtr =
              &SOAD_CFG(SocketRoute, SocketRouteId);

          if(SOAD_FLAG(SocketRoutePtr, SOAD_SOCKETROUTE_FLAG_IS_UL_TP))
          {
            uint16 ReceivedPduLength;
            /* Do not override the current PDU settings if there is already data buffered.
             * Note: It's important to use the ring buffer macro here instead of the
             * SOAD_SoConStatePtr_FLAG_RX_DATA_BUFFERED flag, because the flag is not set/reset
             * until the end of the this function in the context of SoAd_RingBufferSaveAccess.*/
            if(SOAD_RINGBUFFER_USED(&RingBufferAccess) == 0U)
            {
              SoConRxDataPtr->CurrPduHeaderId = PduHeaderId;
              SoConRxDataPtr->CurrPduLen = LengthInHeader;
              SoConRxDataPtr->CurrPduForwardedLen = 0U;
              SoConRxDataPtr->CurrSocketRouteId = SocketRouteId;
            }

            if(RemainingLen < (LengthInHeader + SOAD_PDU_HEADER_LEN))
            {
              /* Segment of a PDU received. */
              if(SOAD_RINGBUFFER_USED(&RingBufferAccess) != 0U)
              {
                /* There is already another PDU stored in the buffer. Set NextPduOutstandingLen
                   for this PDU. SoAd_ReceiveTpPdu() stores the PDU segment and subtracts the stored
                   segment. The next incoming frame will contain the other segment of the PDU.
                   So the remaining bytes in NextPduOutstandingLen describe the offset to the next
                   header in the next frame. NextPduOutstandingLen is only needed for segmented TCP
                   TP PDUs. */
                SoConRxDataPtr->NextPduOutstandingLen = LengthInHeader;
              }
              ReceivedPduLength = RemainingLen - SOAD_PDU_HEADER_LEN;
            }
            else
            {
              ReceivedPduLength = (uint16)LengthInHeader;
            }

            /* Try to deliver (part of) PDU or store it in the ring buffer. */
            SoAd_ReceiveTpPdu(
                               SoConPtr,
                               SoConStatePtr,
                               SocketRouteId,
                               &RingBufferAccess,
                               CurrBufPtr,
                               &CurrBufPtr[SOAD_PDU_HEADER_LEN],
                               ReceivedPduLength
                             );
            RemainingLen = RemainingLen - (ReceivedPduLength + SOAD_PDU_HEADER_LEN);
            CurrBufPtr = &CurrBufPtr[ReceivedPduLength + SOAD_PDU_HEADER_LEN];
          }
          else
          {
            SoAd_RxProcessTcpPduHeaders_IfApi
            (
              SoConPtr,
              SoConStatePtr,
              CurrBufPtr,
              SocketRoutePtr,
              LengthInHeader,
              &RemainingLen
            );

            /* Set buffer to the next PDU. */
            CurrBufPtr = &CurrBufPtr[SOAD_PDU_HEADER_LEN + LengthInHeader];
          }
        }
        else
        {
          /* The received PduHeaderId is either unknown or the related routing group is disabled,
           * it shall be discarded. */

          /* check if the whole Pdu was received */
          if(RemainingLen >= (LengthInHeader + SOAD_PDU_HEADER_LEN))
          {
            uint16 PduLen = SOAD_PDU_HEADER_LEN + (uint16)LengthInHeader;

            /* processing shall be continued, another PDU might have been received */
            RemainingLen -= PduLen;
            CurrBufPtr = &CurrBufPtr[PduLen];

            /* the received but discarded amount of data shall still be acknowledged */
            (void)TcpIp_TcpReceived(SoConStatePtr->SocketId, PduLen);
          }
          else
          {
            /* If only a part of the PDU was received (which is only possible on a Tcp socket
             * connection), the remote node shall be informed by resetting the Tcp connection.
             * It does not make sense to continue the reception of data that is irrelevant for the
             * upper layer. */
            SOAD_FLAG_SET_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RECONNECT_SOCKET);

            RemainingLen = 0U; /* Leave the loop. */

            /* Set global flag to know that for a certain SoCon a transition condition was met. */
            SoAd_SetBitInTransCondChange(SoConPtr);
          }
        }
      }
    }

    /* store ring buffer information */
    SoAd_RingBufferSaveAccess(SoConPtr, SoConStatePtr, &RingBufferAccess);
  }
  else
  {
    /* the received but discarded amount of data shall still be acknowledged */
    (void)TcpIp_TcpReceived(SoConStatePtr->SocketId, Length);
#if((SOAD_DEV_ERROR_DETECT == STD_ON) && (SOAD_REPORT_SOAD_E_INV_PDUHEADER_ID == STD_ON))
    /* For write only socket the PduHeaderId is unknown */
    SOAD_DET_REPORT_ERROR(SOAD_RXINDICATION_SVCID, SOAD_E_INV_PDUHEADER_ID);
#endif /* ((SOAD_DEV_ERROR_DETECT == STD_ON) && (SOAD_REPORT_SOAD_E_INV_PDUHEADER_ID == STD_ON)) */
  }
}

STATIC FUNC(void, SOAD_CODE) SoAd_RxProcessTcpPduHeaders_IfApi
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) CurrBufPtr,
  CONSTP2CONST(SoAd_SocketRouteType, AUTOMATIC, SOAD_APPL_DATA) SocketRoutePtr,
  PduLengthType LengthInHeader,
  CONSTP2VAR(uint16, AUTOMATIC, SOAD_APPL_DATA) RemainingLenPtr
)
{
  if(LengthInHeader > ((PduLengthType)*RemainingLenPtr - (PduLengthType)SOAD_PDU_HEADER_LEN))
  {
    CONSTP2CONST(SoAd_SoConRxBufferCfgType, AUTOMATIC, SOAD_APPL_DATA) RxBufferCfgPtr =
        &SOAD_CFG(SoConRxBufferCfg, SoConPtr->RxDataId);
    CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) RxIfBufferPtr =
        &SOAD_PBRAM(SoAdBuffer, (RxBufferCfgPtr->BufId + RxBufferCfgPtr->TpCapacity));
    SoAd_BufLengthType StoreInIfBufferLen;
    P2VAR(SoAd_SoConRxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConRxDataPtr = NULL_PTR;

    SoAd_GetSoConRxData(SoConPtr, SoConRxDataPtr);

    /* check if this PDU is going to fit entirely into the IfBuffer */
    if((LengthInHeader + SOAD_PDU_HEADER_LEN) <= RxBufferCfgPtr->IfCapacity)
    {
      /* copy PduHeader + First segment of the PDU */
      StoreInIfBufferLen = *RemainingLenPtr;
    }
    else
    {
      /* copy only PduHeader */
      StoreInIfBufferLen = SOAD_PDU_HEADER_LEN;
    }

    /* store PduHeader and if possible the first part of the If-PDU in the linear buffer */
    TS_MemCpy(RxIfBufferPtr, CurrBufPtr, StoreInIfBufferLen);

    /* count all bytes anyway, even if only the PduHeader was copied,
     * to keep track of the PDU Header offsets */
    SoConRxDataPtr->IfIn += *RemainingLenPtr;

    /* set the segmentation flag */
    SOAD_FLAG_SET_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RX_TCP_SEGMENTED);

    /* no more data to process */
    *RemainingLenPtr = 0U;
  }
  else
  {
    SoAd_ULIfRxIndication(
                           SoConPtr,
                           &SOAD_CFG(SocketRouteDest,
                                     SocketRoutePtr->SocketRouteDestFirstId
                                    ),
                           &CurrBufPtr[SOAD_PDU_HEADER_LEN],
                           LengthInHeader
                         );

    (void)TcpIp_TcpReceived
    (
      SoConStatePtr->SocketId,
      (uint32)((uint32)LengthInHeader + (uint32)SOAD_PDU_HEADER_LEN)
    );
    /* the cast for uint16 is save because of the enclosing if/else statement */
    *RemainingLenPtr -= (SOAD_PDU_HEADER_LEN + (uint16)LengthInHeader);
  }
}

STATIC FUNC(void, SOAD_CODE) SoAd_RxProcessUdpPduHeaders
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  uint16 Length,
  SoAd_SoConIdType SoConGroupFirstIdOffset
)
{
  uint16 RemainingLen = Length;
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) CurrBufPtr = BufPtr;

  /* assemble RingBufferAccess */
  SoAd_RingBufferAccessType RingBufferAccess;

  (void)SoAd_RingBufferLoadAccess(SoConPtr, SoConStatePtr, &RingBufferAccess);

  /* Note: the next bytes in the frame (CurrBufPtr) are a PduHeader */

  /* process until the end of the frame or an error occurs */
  while(RemainingLen > 0U)
  {
    if(RemainingLen < SOAD_PDU_HEADER_LEN)
    {
      /* Skip the padding bytes */
      RemainingLen = 0U; /* leave loop */
    }
    else
    {
      Std_ReturnType RetVal;
      SoAd_RouteIdType SocketRouteId = SOAD_INVALID_ROUTE_ID;
      uint32 PduHeaderId;
      PduLengthType LengthInHeader;

      RetVal = SoAd_ExtractPduHeader(
                                      SoConPtr,
                                      CurrBufPtr,
                                      &SocketRouteId,
                                      &PduHeaderId,
                                      &LengthInHeader
                                    );

      if(RemainingLen < (LengthInHeader + SOAD_PDU_HEADER_LEN))
      {
        /* Skip the padding bytes */
        RemainingLen = 0U; /* Leave loop */
      }
      else
      {
        /* check if the PduHeader is valid, and the routing group of the related socket route is
           enabled */
        if(RetVal == E_OK)
        {
          if(!SoAd_IsSocketRouteDestEnabled(&SOAD_CFG(SocketRoute, SocketRouteId), SoConGroupFirstIdOffset))
          {
            RetVal = E_NOT_OK;
          }
        }

        if(RetVal == E_OK)
        {
          CONSTP2CONST(SoAd_SocketRouteType, AUTOMATIC, SOAD_APPL_DATA) SocketRoutePtr =
              &SOAD_CFG(SocketRoute, SocketRouteId);

          if(SOAD_FLAG(SocketRoutePtr, SOAD_SOCKETROUTE_FLAG_IS_UL_TP))
          {
            if(SoConPtr->RxDataId != SOAD_INVALID_SOCON_ID)
            {
              P2VAR(SoAd_SoConRxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConRxDataPtr = NULL_PTR;
              SoAd_GetSoConRxData(SoConPtr, SoConRxDataPtr);

              /* do not override the current PDU settings if there is already data buffered
               * Note: It's important to use the ring buffer macro here instead of the
               * SOAD_SoConStatePtr_FLAG_RX_DATA_BUFFERED flag, because the flag is not set/reset
               * until the end of the this function in the context of SoAd_RingBufferSaveAccess.*/
              if(SOAD_RINGBUFFER_USED(&RingBufferAccess) == 0U)
              {
                SoConRxDataPtr->CurrPduHeaderId = PduHeaderId;
                SoConRxDataPtr->CurrPduLen = LengthInHeader;
                SoConRxDataPtr->CurrPduForwardedLen = 0U;
                SoConRxDataPtr->CurrSocketRouteId = SocketRouteId;
              }

              SoAd_ReceiveTpPdu(
                                 SoConPtr,
                                 SoConStatePtr,
                                 SocketRouteId,
                                 &RingBufferAccess,
                                 CurrBufPtr,
                                 &CurrBufPtr[SOAD_PDU_HEADER_LEN],
                                 /* cast protected by check from SoAd_TpTransmit(),
                                    UDP protocol can only send up to 64k */
                                 (uint16)LengthInHeader
                               );
            }
            else
            {
              SoAd_ReceiveTpPduUdpNoBuf(
                                         SoConPtr,
                                         SoConStatePtr,
                                         SocketRouteId,
                                         &CurrBufPtr[SOAD_PDU_HEADER_LEN],
                                         /* cast protected by check from SoAd_TpTransmit(),
                                            UDP protocol can only send up to 64k */
                                         (uint16)LengthInHeader
                                       );
            }
          }
          else
          {
            SoAd_SocketRouteDestIdType SocketRouteDestId;
            P2CONST(SoAd_SocketRouteDestType, AUTOMATIC, SOAD_APPL_DATA) SocketRouteDestPtr;
            if(SocketRoutePtr->SocketRouteDestCnt == 1U)
            {
              SocketRouteDestId = SocketRoutePtr->SocketRouteDestFirstId;
            }
            else
            {
              const SoAd_SocketRouteDestIdType Id =
                   SocketRoutePtr->SoConToSocketRouteDestMapFirstId + SoConGroupFirstIdOffset;
              SocketRouteDestId = SOAD_PBRAM_REF_ID
                                  (
                                    SoAd_SocketRouteDestIdType,
                                    SoAd_PBcfg->SoConToSocketRouteDestMapBase,
                                    Id
                                  );
            }
            SocketRouteDestPtr = &SOAD_CFG(SocketRouteDest, SocketRouteDestId);
            SoAd_ULIfRxIndication(
                                   SoConPtr,
                                   SocketRouteDestPtr,
                                   &CurrBufPtr[SOAD_PDU_HEADER_LEN],
                                   LengthInHeader
                                 );
          }
        }

        /* Remove current PDU. If RemainingLen > 0 processing shall be continued. */
        /* cast protected by check from SoAd_ExtractPduHeader(),
         * UDP protocol can only send up to 64k.
         */
        RemainingLen = RemainingLen - (uint16)(LengthInHeader + SOAD_PDU_HEADER_LEN);
        CurrBufPtr = &CurrBufPtr[LengthInHeader + SOAD_PDU_HEADER_LEN];
      }
    }
  }

  /* store ring buffer information */
  SoAd_RingBufferSaveAccess(SoConPtr, SoConStatePtr, &RingBufferAccess);
}


STATIC FUNC(void, SOAD_CODE) SoAd_RxUdp
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  uint16 Length,
  SoAd_SoConIdType SoConGroupFirstIdOffset
)
{

  DBG_SOAD_RXUDP_ENTRY(SoConPtr, SoConStatePtr, BufPtr, Length, SoConGroupFirstIdOffset);

  if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_PDU_HEADER_MODE))
  {
    SoAd_RxProcessUdpPduHeaders(SoConPtr, SoConStatePtr, BufPtr, Length, SoConGroupFirstIdOffset);
  }
  else
  {
    if(SoConPtr->SocketRouteFirstId != SOAD_INVALID_ROUTE_ID)
    {
      /* Note: since PDU Header mode is disabled use the only SocketRoute, i.e. SocketRouteFirstId */
      CONSTP2CONST(SoAd_SocketRouteType, AUTOMATIC, SOAD_APPL_DATA) SocketRoutePtr =
          &SOAD_CFG(SocketRoute, SoConPtr->SocketRouteFirstId);

      if(SoAd_IsSocketRouteDestEnabled(SocketRoutePtr, SoConGroupFirstIdOffset))
      {
        /* Note: ignore setting of related routing group, active receptions shall not
         * be interrupted */
        if(SOAD_FLAG(SocketRoutePtr, SOAD_SOCKETROUTE_FLAG_IS_UL_TP))
        {
          SoAd_RingBufferAccessType RingBufferAccess;
          const Std_ReturnType RetVal_RingBuf =
            SoAd_RingBufferLoadAccess(SoConPtr, SoConStatePtr, &RingBufferAccess);
          if(E_OK == RetVal_RingBuf)
          {
            SoAd_ReceiveTpPdu(
                               SoConPtr,
                               SoConStatePtr,
                               SoConPtr->SocketRouteFirstId,
                               &RingBufferAccess,
                               NULL_PTR, /* no pduHeader used here */
                               BufPtr,
                               Length
                             );
            SoAd_RingBufferSaveAccess(SoConPtr, SoConStatePtr, &RingBufferAccess);
          }
          else
          {
            SoAd_ReceiveTpPduUdpNoBuf(
                                       SoConPtr,
                                       SoConStatePtr,
                                       SoConPtr->SocketRouteFirstId,
                                       BufPtr,
                                       /* cast protected by check from SoAd_TpTransmit(),
                                       UDP protocol can only send up to 64k */
                                       (uint16)Length
                                     );
          }
        }
        else /* upper layer is 'If' */
        {
          /* Without Header mode only one SocketRouteDest is allowed. */
          SoAd_ULIfRxIndication(SoConPtr,
                                &SOAD_CFG(SocketRouteDest, SocketRoutePtr->SocketRouteDestFirstId),
                                BufPtr,
                                Length
                               );
        }
      }/* else Socket route disabled. Ignore frame. */
    } /* else Received data on write-only socket. Ignore frame. */

  }

  DBG_SOAD_RXUDP_EXIT(SoConPtr, SoConStatePtr, BufPtr, Length, SoConGroupFirstIdOffset);
}

STATIC FUNC(void, SOAD_CODE) SoAd_RxIndication_Tcp
(
  SoAd_SoConIdType SoConId,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  uint16 Length,
  CONSTP2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) RemoteAddrPtr
)
{
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr =
    &SOAD_CFG(SoCon, SoConId);
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr =
    &SOAD_PBRAM(SoConState, SoConId);
  SoAd_SocketStateType SocketState;

  TS_AtomicAssign8(SocketState, SoConStatePtr->SocketState);

  if
  (
    SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RX_TP_CANCELED)
    || SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_TP_CANCELED)
    || SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_CLOSE_SOCON)
    || SOAD_FLAG_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TCPIP_SOCKET_CLOSED)
    || SOAD_FLAG_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RECONNECT_SOCKET)
    || (SocketState != SOAD_SOCKETSTATE_ONLINE)
  )
  {
    /* socket connection is currently in shutdown process - do not accept data */
    /* In case a FIN was received, TcpIp has already sent the data to SoAd and is not accepting
     * data anymore for this Tcp connection. Therefore, it is not necessary to check here for a
     * received FIN. */

    /* Note: SoAd must still acknowledge the received TCP data so that the socket can be closed.
     * See IEEE RFC 793 section-3.5:
     * ... All segments preceding and including FIN will be retransmitted until acknowledged. */
    (void)TcpIp_TcpReceived(SoConStatePtr->SocketId, Length);
  }
  else
  {
    CONSTP2CONST(SoAd_SoConGroupType, AUTOMATIC, SOAD_APPL_DATA) SoConGroupPtr =
      &SOAD_CFG(SoConGroup, SoConPtr->SoConGroupId);
    const SoAd_SoConIdType SoConGroupFirstIdOffset = SoConPtr->SoConId - SoConGroupPtr->SoConFirstId;
    SoAd_AddressMatchType AddressMatch;

    SOAD_ENTER_CRITSEC();
    /* lock the socket connection */
    SoConStatePtr->BusyCnt++;
    SOAD_EXIT_CRITSEC();

    AddressMatch = SoAd_RemoteAddressMatch(SoConStatePtr, RemoteAddrPtr);
    if(AddressMatch != SOAD_ADDRESSMATCH_EXACT)
    {
#if(SOAD_GETANDRESETMEASUREMENTDATA_API == STD_ON)
      /* increase TCP MeasurementData counter and drop frame */
      /* !LINKSTO SoAd.ASR43.SWS_SoAd_00754, 1 */
      SchM_Enter_SoAd_SCHM_SOAD_EXCLUSIVE_AREA_0();
      if(SoAd_MeasurementDropData_Tcp < 0xFFFFFFFFU)
      {
        SoAd_MeasurementDropData_Tcp++;
      }
      SchM_Exit_SoAd_SCHM_SOAD_EXCLUSIVE_AREA_0();
#endif /* SOAD_GETANDRESETMEASUREMENTDATA_API == STD_ON */
    }
    else
    {
      if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_PDU_HEADER_MODE))
      {
        uint16 Len = Length;

        if(SOAD_FLAG_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RX_TCP_SEGMENTED))
        {
          SoAd_RxProcessTcpPduSegment(SoConPtr, SoConStatePtr, BufPtr, &Len, SoConGroupFirstIdOffset);
        }

        SoAd_RxProcessTcpPduHeaders(
                                     SoConPtr,
                                     SoConStatePtr,
                                     &(BufPtr[Length-Len]),
                                     Len,
                                     SoConGroupFirstIdOffset
                                   );
      }
      else
      {
        /* Note: since PDU Header mode is disabled use the only SocketRoute, i.e.
           SocketRouteFirstId */
        if(SoConPtr->SocketRouteFirstId != SOAD_INVALID_ROUTE_ID)
        {
          CONSTP2CONST(SoAd_SocketRouteType, AUTOMATIC, SOAD_APPL_DATA) SocketRoutePtr =
              &SOAD_CFG(SocketRoute, SoConPtr->SocketRouteFirstId);

          if(SoAd_IsSocketRouteDestEnabled(SocketRoutePtr, SoConGroupFirstIdOffset))
          {
            if(SOAD_FLAG(SocketRoutePtr, SOAD_SOCKETROUTE_FLAG_IS_UL_TP))
            {
              /* Note: ignore setting of related routing group, active receptions shall not
               * be interrupted */
              SoAd_RingBufferAccessType RingBufferAccess;
              /* SoAd_RingBufferLoadAccess() always returns OK since TCP TP always has a ring buffer. */
              (void)SoAd_RingBufferLoadAccess(SoConPtr, SoConStatePtr, &RingBufferAccess);
              SoAd_ReceiveTpPdu
              (
                SoConPtr,
                SoConStatePtr,
                SoConPtr->SocketRouteFirstId,
                &RingBufferAccess,
                NULL_PTR, /* no pduHeader used here */
                BufPtr,
                Length
              );
              SoAd_RingBufferSaveAccess(SoConPtr, SoConStatePtr, &RingBufferAccess);
            }
            else /* upper layer is 'If' */
            {
              SoAd_ULIfRxIndication
              (
                SoConPtr,
                &SOAD_CFG(SocketRouteDest, SocketRoutePtr->SocketRouteDestFirstId),
                BufPtr,
                Length
              );

              (void)TcpIp_TcpReceived(SoConStatePtr->SocketId, Length);
            }
          }
          else
          {
            /* Free TCP window if frame discarded. */
            (void)TcpIp_TcpReceived(SoConStatePtr->SocketId, Length);
          }
        }
        else
        {
          (void)TcpIp_TcpReceived(SoConStatePtr->SocketId, Length);
        }
      }
    }
    SOAD_ENTER_CRITSEC();
    /* unlock the (first) SoCon */
    SoConStatePtr->BusyCnt--;
    SOAD_EXIT_CRITSEC();
  }
}

STATIC FUNC(void, SOAD_CODE) SoAd_RxIndication_Udp
(
  SoAd_SoConIdType SoConFirstId,
  CONSTP2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) RemoteAddrPtr,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  uint16 Length
)
{
  SoAd_SoConIdType SoConId = SoConFirstId;
  boolean Accepted = FALSE;
  P2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr = &SOAD_CFG(SoCon, SoConFirstId);
  P2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr;
  Std_ReturnType RetVal_CheckUdpFrame;

  SoAd_AddressMatchType AddressMatch = SOAD_ADDRESSMATCH_NONE;

  SOAD_ENTER_CRITSEC();

  /* ... of a socket connection group ... */
  /* The function will return for SoConId either best match or first SoConId. */
  SoAd_BestMatchAlgorithm(SoConPtr, RemoteAddrPtr, &SoConId, &AddressMatch, TRUE, FALSE);

  /* Update SoConStatePtr and SoConPtr based on best match SoConId */
  SoConStatePtr = &SOAD_PBRAM(SoConState, SoConId);
  SoConPtr = &SOAD_CFG(SoCon, SoConId);

  RetVal_CheckUdpFrame = SoAd_RxIndication_CheckUdpFrame
  (
    SoConPtr,
    BufPtr,
    Length,
    (SoConId - SoConFirstId)
  );

  if(E_OK == RetVal_CheckUdpFrame)
  {
    if(
        (SoConStatePtr->SocketState == SOAD_SOCKETSTATE_CONNECTING)
        || (SoConStatePtr->SocketState == SOAD_SOCKETSTATE_ONLINE)
      )
    {
      /* Check if the reception passed the message acceptance filter */
      Accepted = SoAd_MessageAcceptanceFilter
      (
        SoConPtr,
        SoConStatePtr,
        RemoteAddrPtr,
        AddressMatch
      );
#if (SOAD_GETANDRESETMEASUREMENTDATA_API == STD_ON)
      if(Accepted == FALSE)
      {
        /* increase UDP MeasurementData counter */
        /* !LINKSTO SoAd.ASR43.SWS_SoAd_00754, 1 */
        SchM_Enter_SoAd_SCHM_SOAD_EXCLUSIVE_AREA_0();
        if(SoAd_MeasurementDropData_Udp < 0xFFFFFFFFU)
        {
          SoAd_MeasurementDropData_Udp++;
        }
        SchM_Exit_SoAd_SCHM_SOAD_EXCLUSIVE_AREA_0();
      }
#endif /* SOAD_GETANDRESETMEASUREMENTDATA_API == STD_ON */
    }
  }

  SOAD_EXIT_CRITSEC();

  if(Accepted == FALSE)
  {
    /* Discard reception */
  }
  else if
  (
    SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RX_TP_CANCELED)
    || SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_TP_CANCELED)
    || SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_CLOSE_SOCON)
    || SOAD_FLAG_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TCPIP_SOCKET_CLOSED)
    || SOAD_FLAG_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RECONNECT_SOCKET)
  )
  {
    /* Discard reception */
  }
  else
  {
    /* If the socket connection is in state CONNECTING try to put it to state ONLINE.
     * Return value is ignored. If transition is not executed it is already in the state or it
     * will be repeated in the next main function. */
    (void)SoAd_Int_01_UdpSM_Connected(SoConPtr, SoConStatePtr);

    /* continue with further processing of the UDP message */
    SoAd_RxUdp(SoConPtr, SoConStatePtr, BufPtr, Length, (SoConId - SoConFirstId));
  }

}

STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_RxIndication_CheckUdpFrame
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  uint16 Length,
  SoAd_SoConIdType SoConGroupFirstIdOffset
)
{
  uint16 RemainingLen = Length;
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) CurrBufPtr = BufPtr;
  Std_ReturnType RetVal_ValidPduFound = E_NOT_OK;

  if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_PDU_HEADER_MODE))
  {
    /* Note: the next bytes in the frame (CurrBufPtr) are a PduHeader */
    /* Process until the end of the frame or an error occurs. */
    while(RemainingLen > 0U)
    {
      PduLengthType LengthInHeader = 0U;

      if(RemainingLen < SOAD_PDU_HEADER_LEN)
      {
        /* Stop processing. Error handling is performed at the end. */
      }
      else
      {
        Std_ReturnType RetVal;
        SoAd_RouteIdType SocketRouteId = SOAD_INVALID_ROUTE_ID;
        uint32 PduHeaderId;

        RetVal = SoAd_ExtractPduHeader(
                                        SoConPtr,
                                        CurrBufPtr,
                                        &SocketRouteId,
                                        &PduHeaderId,
                                        &LengthInHeader
                                      );
        if(RetVal != E_OK)
        {
          /* SoAd_ExtractPduHeader returned E_NOT_OK due to an invalid PduHeader --> skip PDU */
        }
        else if(RemainingLen < (LengthInHeader + SOAD_PDU_HEADER_LEN))
        {

        }
        else if(!SoAd_IsSocketRouteDestEnabled(
                                              &SOAD_CFG(SocketRoute, SocketRouteId),
                                              SoConGroupFirstIdOffset
                                              )
               )
        {
          /* Skip PDU if socket route is disabled. */
        }
        else
        {
          /* Valid PDU found. */
          RetVal_ValidPduFound = E_OK;
          if(!SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_UDP_STRICT_HEADER_LEN_CHECK))
          {
            RemainingLen = 0U; /* Leave loop if strict header check is disabled. */
          }
        }
      }

      if(0U == RemainingLen)
      {
        /* RemainingLen was set to 0 by purpose. Leave the loop. */
      }
      else if(RemainingLen >= (LengthInHeader + SOAD_PDU_HEADER_LEN))
      {
        /* Remove current PDU. If RemainingLen > 0 processing shall be continued. */
        /* cast protected by if(RemainingLen >= (LengthInHeader + SOAD_PDU_HEADER_LEN)),
           RemainingLen is uint16 */
        RemainingLen = RemainingLen - (uint16)(LengthInHeader + SOAD_PDU_HEADER_LEN);
        CurrBufPtr = &CurrBufPtr[LengthInHeader + SOAD_PDU_HEADER_LEN];
      }
      else
      {
        /* Padding bytes detected. */
        if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_UDP_STRICT_HEADER_LEN_CHECK))
        {
          /* Reject whole frame. */
          RetVal_ValidPduFound = E_NOT_OK;
        }
        else
        {
          /* Just ignore the padding bytes. */
        }
        RemainingLen = 0U; /* Leave loop. */
      }
    }
  }
  else
  {
    /* No checks required if header mode is disabled. */
    RetVal_ValidPduFound = E_OK;
  }

  return RetVal_ValidPduFound;
}


STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_ExtractPduHeader
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2CONST(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  CONSTP2VAR(SoAd_RouteIdType, AUTOMATIC, SOAD_APPL_DATA) SocketRouteIdPtr,
  CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) PduHeaderIdPtr,
  CONSTP2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_DATA) PduLengthPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint32 Length;
  CONSTP2CONST(SoAd_SoConGroupType, AUTOMATIC, SOAD_APPL_DATA) SoConGroupPtr =
    &SOAD_CFG(SoConGroup, SoConPtr->SoConGroupId);

  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConFirstPtr =
      &SOAD_CFG(SoCon, SoConGroupPtr->SoConFirstId);

  const SoAd_RouteIdType SocketRouteFirstId = SoConPtr->SocketRouteFirstId;

  const SoAd_RouteIdType SoConFirstId = SoConFirstPtr->SocketRouteFirstId;

  DBG_SOAD_EXTRACTPDUHEADER_ENTRY(SoConPtr, BufPtr, SocketRouteIdPtr, PduHeaderIdPtr, PduLengthPtr);

  /* extract the header id of the PDU */
  *PduHeaderIdPtr = COMSTACK_GET32(BufPtr, 0U);

  /* Note: If no socketRoute was found there might be a groupSocketRoute */
  if(SocketRouteFirstId != SOAD_INVALID_ROUTE_ID)
  {
    /* search for the related socket route */
    RetVal = SoAd_PduHeaderSearch(SoConPtr, *PduHeaderIdPtr, SocketRouteIdPtr);
  }

  if((SoConFirstId != SOAD_INVALID_ROUTE_ID) && (RetVal == E_NOT_OK))
  {
    /* search for a related group socket route */
    RetVal = SoAd_PduHeaderSearch(SoConFirstPtr, *PduHeaderIdPtr, SocketRouteIdPtr);
  }

#if((SOAD_DEV_ERROR_DETECT == STD_ON) && (SOAD_REPORT_SOAD_E_INV_PDUHEADER_ID == STD_ON))
  if(RetVal == E_NOT_OK)
  {
    /* SoAd_ExtractPduHeader returns E_NOT_OK if the PduHeaderId is unknown */
    SOAD_DET_REPORT_ERROR(SOAD_RXINDICATION_SVCID, SOAD_E_INV_PDUHEADER_ID);
  }
#endif /* ((SOAD_DEV_ERROR_DETECT == STD_ON) && (SOAD_REPORT_SOAD_E_INV_PDUHEADER_ID == STD_ON)) */

  /* extract the length of the PDU */
  Length = COMSTACK_GET32(BufPtr, 4U);

  /* check if length in Pdu header is too large, or Udp Pdu exceeds 64k Pdu length */
  /* !LINKSTO SoAd.dsn.PduHeader.LengthInHeader.Udp.Invalid, 1 */
  if(
      (!SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_TCP)) &&
      (Length > (SOAD_UDP_PDULENGTH_MAX - SOAD_PDU_HEADER_LEN))
    )
  {
    *PduLengthPtr = SOAD_UDP_PDULENGTH_MAX - SOAD_PDU_HEADER_LEN;
    RetVal = E_NOT_OK;
#if(SOAD_DEV_ERROR_DETECT == STD_ON)
    SOAD_DET_REPORT_ERROR(SOAD_RXINDICATION_SVCID, SOAD_E_INV_LENGTH_IN_PDUHEADER);
#endif /* (SOAD_DEV_ERROR_DETECT == STD_ON) */
  }
  /* !LINKSTO SoAd.dsn.PduHeader.LengthInHeader.Max.Invalid, 1 */
  else if(Length > (SOAD_PDULENGTHTYPE_MAX - SOAD_PDU_HEADER_LEN))
  {
    *PduLengthPtr = SOAD_PDULENGTHTYPE_MAX - SOAD_PDU_HEADER_LEN;
    RetVal = E_NOT_OK;
#if(SOAD_DEV_ERROR_DETECT == STD_ON)
    SOAD_DET_REPORT_ERROR(SOAD_RXINDICATION_SVCID, SOAD_E_INV_LENGTH_IN_PDUHEADER);
#endif /* (SOAD_DEV_ERROR_DETECT == STD_ON) */
  }
  else
  {
    *PduLengthPtr = (PduLengthType)Length;
  }

  DBG_SOAD_EXTRACTPDUHEADER_EXIT
  (
    RetVal,
    SoConPtr,
    BufPtr,
    SocketRouteIdPtr,
    PduHeaderIdPtr,
    PduLengthPtr
  );
  return RetVal;
}

/* !LINKSTO SoAd.Dsn.ExtractPduHeader.BinarySearch, 1 */
STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_PduHeaderSearch
(
  P2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  VAR(uint32, SOAD_VAR) PduHeaderId,
  P2VAR(SoAd_RouteIdType, AUTOMATIC, SOAD_APPL_DATA) SocketRouteIdPtr
)
{
    SoAd_RouteIdType MedianIdx;
    Std_ReturnType RetVal = E_NOT_OK;
    SoAd_RouteIdType SocketRouteLeftIdx = SoConPtr->SocketRouteFirstId;
    SoAd_RouteIdType SocketRouteRightIdx = SocketRouteLeftIdx + SoConPtr->SocketRouteCnt - 1U;

    /* Check if needed PduHeaderId is within range */
    /* !LINKSTO SoAd.Dsn.ExtractPduHeader.BinarySearch.IndexOutOfBoundsProtection, 1 */
    if(
        (&SOAD_CFG(SocketRoute, SocketRouteLeftIdx))->PduHeaderId <= PduHeaderId &&
        (&SOAD_CFG(SocketRoute, SocketRouteRightIdx))->PduHeaderId >= PduHeaderId
      )
    {
      P2CONST(SoAd_SocketRouteType, AUTOMATIC, SOAD_APPL_DATA) SocketRoutePtr = NULL_PTR;

      while((RetVal != E_OK) && (SocketRouteLeftIdx <= SocketRouteRightIdx))
      {
        /* Divide by 2U with shift operation */
        MedianIdx = (SocketRouteLeftIdx + SocketRouteRightIdx) >> 1U;
        SocketRoutePtr = &SOAD_CFG(SocketRoute, MedianIdx);

        if(SocketRoutePtr->PduHeaderId < PduHeaderId)
        {
          SocketRouteLeftIdx = MedianIdx + 1U;
        }
        else if (SocketRoutePtr->PduHeaderId > PduHeaderId)
        {
          SocketRouteRightIdx = MedianIdx - 1U;
        }
        else
        {
          *SocketRouteIdPtr = MedianIdx;
          RetVal = E_OK;
        }
      }
    }

    return RetVal;
}


STATIC FUNC(boolean, SOAD_CODE) SoAd_IsSocketRouteDestEnabled
(
  CONSTP2CONST(SoAd_SocketRouteType, AUTOMATIC, SOAD_APPL_DATA) SocketRoutePtr,
  SoAd_SoConIdType SoConGroupFirstIdOffset
)
{
  boolean IsEnabled = FALSE;
  DBG_SOAD_ISSOCKETROUTEENABLED_ENTRY(SocketRoutePtr, SoConGroupFirstIdOffset);

  if(SocketRoutePtr->SocketRouteDestCnt == 1U)
  {
    /* Cnt is 1U, use First Id */
    P2CONST(SoAd_SocketRouteDestType, AUTOMATIC, SOAD_APPL_DATA) SocketRouteDestPtr =
              &SOAD_CFG(SocketRouteDest, SocketRoutePtr->SocketRouteDestFirstId);
    CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) SocketRouteDestEnabledBitMapPtr =
              &SOAD_PBRAM_REF_ID(uint32, SoAd_PBcfg->SocketRouteDestEnabledBitMapBase, 0U);
    IsEnabled = (0U != SOAD_GET_BIT_FROM_32BIT_BITARRAY(SocketRouteDestEnabledBitMapPtr,SocketRouteDestPtr->SocketRouteDestId));
  }
  else
  {
    const SoAd_SocketRouteDestIdType Id =
      SocketRoutePtr->SoConToSocketRouteDestMapFirstId + SoConGroupFirstIdOffset;
    CONSTP2VAR(SoAd_SocketRouteDestIdType, AUTOMATIC, SOAD_APPL_DATA) SocketRouteDestIdPtr =
                  &SOAD_PBRAM_REF_ID(SoAd_SocketRouteDestIdType, SoAd_PBcfg->SoConToSocketRouteDestMapBase, Id);
    IsEnabled = (*SocketRouteDestIdPtr != SOAD_INVALID_SOCKET_ROUTE_DEST_ID);
  }

  DBG_SOAD_ISSOCKETROUTEENABLED_EXIT(IsEnabled, SoConGroupFirstIdOffset, SocketRoutePtr);
  return IsEnabled;

}

/** \brief Matches the configured remote address of a socket connection against a given address.
 ** \param[in] SoConState     Pointer to the state information of the socket connection
 ** \param[in] RemoteAddrPtr  Pointer to the address information to compare.
 ** \returns   SoAd_AddressMatchType
 ** \retval    SOAD_ADDRESSMATCH_NONE      No match.
 ** \retval    SOAD_ADDRESSMATCH_WILDCARD  IpAddr and Port are wild cards.
 ** \retval    SOAD_ADDRESSMATCH_PORT      Port matches, IpAddr is a wild card
 ** \retval    SOAD_ADDRESSMATCH_IPADDR    IpAddr matches, Port is a wild card
 ** \retval    SOAD_ADDRESSMATCH_EXACT     IpAddr and Port match.
 **/
STATIC FUNC(SoAd_AddressMatchType, SOAD_CODE) SoAd_RemoteAddressMatch
(
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) RemoteAddrPtr
)
{
  SoAd_AddressMatchType RetVal = SOAD_ADDRESSMATCH_NONE;
#if(SOAD_IPV6_SUPPORT == STD_ON)
  CONSTP2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, SOAD_APPL_DATA) RemoteInetAddrPtr =
      (P2CONST(void, TYPEDEF, SOAD_APPL_DATA))
      RemoteAddrPtr;
#else /* (SOAD_IPV6_SUPPORT == STD_ON) */
  CONSTP2CONST(TcpIp_SockAddrInetType, AUTOMATIC, SOAD_APPL_DATA) RemoteInetAddrPtr =
      (P2CONST(void, TYPEDEF, SOAD_APPL_DATA))
      RemoteAddrPtr;
#endif /* (SOAD_IPV6_SUPPORT == STD_ON) */

  DBG_SOAD_REMOTEADDRESSMATCH_ENTRY(SoConStatePtr, RemoteAddrPtr);

  if(SOAD_IS_ADDRESS_WILDCARD(SoConStatePtr->RemoteAddress))
  {
    if(SoConStatePtr->RemotePort == 0U)
    {
      RetVal = SOAD_ADDRESSMATCH_WILDCARD;
    }
    else if(SoConStatePtr->RemotePort == RemoteInetAddrPtr->port)
    {
      RetVal = SOAD_ADDRESSMATCH_PORT;
    }
    else
    {
      /* Address is wildcard and port doesn't match */
      RetVal = SOAD_ADDRESSMATCH_NONE;
    }
  }
  else
  {
    boolean AddressIsMatch = FALSE;
#if(SOAD_IPV6_SUPPORT == STD_ON)
    if(RemoteAddrPtr->domain == TCPIP_AF_INET6)
    {
      AddressIsMatch =
          ((( SoConStatePtr->RemoteAddress[3U] == RemoteInetAddrPtr->addr[3U] )  &&
              ( SoConStatePtr->RemoteAddress[2U] == RemoteInetAddrPtr->addr[2U] )) &&
              ( SoConStatePtr->RemoteAddress[1U] == RemoteInetAddrPtr->addr[1U] )) &&
              ( SoConStatePtr->RemoteAddress[0U] == RemoteInetAddrPtr->addr[0U]
              );
    }
    else
#endif /* (SOAD_IPV6_SUPPORT == STD_ON) */
    {
      AddressIsMatch = (SoConStatePtr->RemoteAddress[0U] == RemoteInetAddrPtr->addr[0U]);
    }

    if(AddressIsMatch)
    {
      if(SoConStatePtr->RemotePort == 0U)
      {
        RetVal = SOAD_ADDRESSMATCH_IPADDR;
      }
      else if(SoConStatePtr->RemotePort == RemoteInetAddrPtr->port)
      {
        RetVal = SOAD_ADDRESSMATCH_EXACT;
      }
      else
      {
        /* Address matches and port doesn't match */
        RetVal = SOAD_ADDRESSMATCH_NONE;
      }
    }
  }

  DBG_SOAD_REMOTEADDRESSMATCH_EXIT(RetVal, SoConState, RemoteAddrPtr);
  return RetVal;
}







#if(TS_MERGED_COMPILE == STD_OFF)
#define SOAD_STOP_SEC_CODE
#include <SoAd_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */
/*==================[end of file]===========================================*/
