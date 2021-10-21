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
 * MISRAC2012-4) Deviated Rule: 11.3 (required)
 * A cast shall not be performed between a pointer to object type and a pointer to a different
 * object type.
 *
 * Reason:
 * AUTOSAR defines a generic type which dependent on a member of the generic type
 * implements an concrete type. Thus for usage of this concrete type, a cast is necessary.
 *
 * MISRAC2012-5) Deviated Rule: 16.3 (required)
 * An unconditional break statement shall terminate every switch-clause.
 *
 * Reason:
 * Intentional fall-through
 *
 * MISRAC2012-6) Deviated Rule: 11.5 (advisory)
 * A conversion should not be performed from pointer to void into pointer to
 * object.
 *
 * Reason:
 * The memory routines are optimized for dealing with aligned memory sections.
 *
 * MISRAC2012-7) Deviated Rule: 16.1 (required)
 * All switch statements shall be well-formed.
 *
 * Reason:
 * Intentional fall-through
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


/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/** \brief Transmission mode type
 *  The transition mode shall be used to indicate how to transmit an IF UDP frame.
 **/
typedef uint8 SoAd_TxModeType;

#if(defined SOAD_TX_MODE_NONE)
#error SOAD_TX_MODE_NONE is already defined
#endif
#define SOAD_TX_MODE_NONE 0U

#if(defined SOAD_TX_MODE_DIRECT)
#error SOAD_TX_MODE_DIRECT is already defined
#endif
#define SOAD_TX_MODE_DIRECT 1U

#if(defined SOAD_TX_MODE_INDIRECT)
#error SOAD_TX_MODE_INDIRECT is already defined
#endif
#define SOAD_TX_MODE_INDIRECT 2U

#if(defined SOAD_TX_MODE_BUFFERING)
#error SOAD_TX_MODE_BUFFERING is already defined
#endif
#define SOAD_TX_MODE_BUFFERING 3U

/*==================[external function declarations]========================*/


/*==================[internal function declarations]========================*/
#if(TS_MERGED_COMPILE == STD_OFF)
#define SOAD_START_SEC_CODE
#include <SoAd_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

/** \brief Part of the SoAd_MainFunction that deals buffered If Udp frames as required by
           nPduUdpTx and Udp retry feature
 **
 ** \param[in] SoConPtr - Pointer to socket connection configuration.
 ** \param[in] SoConStatePtr - Pointer to socket connection state.
 */
STATIC FUNC(void, SOAD_CODE) SoAd_MainFunctionIfTxUdp
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

STATIC FUNC(void, SOAD_CODE) SoAd_MainFunctionUdpTxConf
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

/** \brief Helper function to restore PDUs from shadow buffer.
 *
 *  This helper function copies If PDUs stored in the shadow buffer back to the Tx buffer after
 *  transmission was performed.
 **/
STATIC FUNC(void, SOAD_CODE) SoAd_RestoreUdpIfShadowBuffer
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr
);

/** \brief Checks all PduRoute related to the given socket connection for routes in state
 **        SOAD_PDUROUTEDESTVIRTUALSTATE_BUFFERED and sets them to _WAITCONF or _IDLE depending on the success
 **        of the transmission. Also resets the nPduUdpTx timer and buffer counter.
 ** \attention Should be called from within a critical section due to the access to SoConStatePtr
 **            and SoConTxData.
 ** \param[in] SoConPtr - to find related PduRoutes.
 ** \param[in] SoConStatePtr - to set SOAD_SOCONSTATE_FLAG_TXCONF_PENDING.
 ** \param[in] TransmissionOk to decided if TxConfirmation shall be issued
 **/
STATIC FUNC(void, SOAD_CODE) SoAd_CleanUpUdpTxBuffer
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  boolean TransmissionOk
);

STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_IfTransmitPduRouteDestUdp
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) SoAdSrcPduInfoPtr,
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr
);

STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_IfTransmitPduRouteDestTcp
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) SoAdSrcPduInfoPtr,
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr
);

STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_IfTransmitPduRouteDestUdp_SendBuffer
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, AUTOMATIC) RemoteAddrPtr,
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr
);

STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_IfTransmit_UdpTransmitRetValHandling
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) SoAdSrcPduInfoPtr,
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr,
  TcpIp_ReturnType RetVal_TcpIp,
  uint16 RemainingBytes
);

/** \brief Helper function to store in buffer.
 *
 *  This helper function copies UDP If PDUs into the buffer. In case that transmission of the
 *  buffer is in progress the remaining buffer space is used as shadow buffer for PDUs to be
 *  restored after transmission into Tx buffer.
 **/
STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_IfTransmit_UdpStoreToBuffer
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) SoAdSrcPduInfoPtr,
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr
);

/** \brief Helper function to store in shadow buffer.
 *
 *  This helper function copies UDP If PDUs into remaining Tx buffer in case that transmission of the
 *  buffer is in progress.
 **/
STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_IfTransmit_UdpStoreToShadowBuffer
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) SoAdSrcPduInfoPtr,
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr
);

/** \brief Helper function to provide common part for both store to Tx and shadow buffer.
 *
 **/
STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_IfTransmit_UdpStoreToBufferCommon
(
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) TxBufferPtr,
  CONSTP2VAR(SoAd_BufLengthType, AUTOMATIC, SOAD_APPL_DATA) BufferLenPtr,
  CONSTP2VAR(SoAd_TimerType, AUTOMATIC, SOAD_APPL_DATA) TimerPtr,
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) SoAdSrcPduInfoPtr,
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr
);


STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_PrepareUdpIfTransmit
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(SoAd_SoConTxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConTxDataPtr,
  CONSTP2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) SoAdSrcPduInfoPtr,
  CONSTP2VAR(SoAd_TxModeType, AUTOMATIC, SOAD_APPL_DATA) TxModePtr,
  CONSTP2VAR(boolean, AUTOMATIC, SOAD_APPL_DATA) SendBufferPtr,
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr,
  CONSTP2VAR(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) RemoteAddrPtr
);


STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_PrepareUdpIfTransmit_TxModeSelection
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(SoAd_SoConTxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConTxDataPtr,
  CONSTP2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) SoAdSrcPduInfoPtr,
  CONSTP2VAR(SoAd_TxModeType, AUTOMATIC, SOAD_APPL_DATA) TxModePtr,
  CONSTP2VAR(boolean, AUTOMATIC, SOAD_APPL_DATA) SendBufferPtr,
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr,
  CONSTP2VAR(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) RemoteAddrPtr
);


STATIC FUNC(void, SOAD_CODE) SoAd_IfTransmitSkipConf
(
  PduIdType SoAdSrcPduId
);

STATIC FUNC(void, SOAD_CODE) SoAd_TxConfirmation_ProvideConf
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  uint16 Length
);

/** \brief SoAd_CheckTcpIpSocketInUse checks if the TcpIp socket connection is in use.
 **
 ** \param[in] SoConPtr - Pointer to socket connection configuration.
 **
 ** \returns   boolean
 ** \retval    TRUE   The TcpIp socket is in use.
 ** \retval    FALSE  The TcpIp socket is not in use.
 */
STATIC FUNC(boolean, SOAD_CODE) SoAd_CheckTcpIpSocketInUse
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr
);

STATIC FUNC(SoAd_SoConIdType, SOAD_CODE) SoAd_CopyTxData_GetUsedSoConId
(
  SoAd_SoConIdType SoConFirstId
);

STATIC FUNC(void, SOAD_CODE) SoAd_ULIfTxConfirmation
(
  SoAd_RouteIdType PduRouteId
);

STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_ULIfTriggerTransmit
(
  SoAd_RouteIdType PduRouteId,
  CONSTP2VAR(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) PduInfoPtr
);

STATIC FUNC(void, SOAD_CODE) SoAd_WritePduHeader
(
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  uint32 PduHeaderId,
  uint32 PduHeaderLengthField,
  uint8 Offset,
  uint8 Length
);

STATIC FUNC(void, SOAD_CODE) SoAd_WritePduHeaderLastIsBest
(
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  CONSTP2VAR(SoAd_BufLengthType, AUTOMATIC, SOAD_APPL_DATA) BufferLenPtr,
  PduLengthType PduHeaderLengthField
);


/** \brief Checks if a destination of a given PduRoute is waiting for a transmit confirmation or
 **        UDP retransmission.
 **
 ** \param[in]  PduRouteId - Index of the PduRoute, that shall be checked
 ** \returns boolean
 ** \retval TRUE  A destination of the fan-out is waiting for a confirmation or UDP retransmission.
 ** \retval FALSE No destination is waiting, a TxConfirmation can be passed to the upper layer.
 **/
STATIC FUNC(boolean, SOAD_CODE) SoAd_PduFanOutPduTxConfPending
(
  SoAd_RouteIdType PduRouteId
);

STATIC FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_CopyTxData_BufferedData
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr,
  uint16 BufLength
);

STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_CopyTxData_FromInternalBuffer_LastIsBest
(
  CONSTP2VAR(SoAd_Uint8PtrType, AUTOMATIC, SOAD_APPL_DATA) BufPtrPtr,
  CONSTP2VAR(SoAd_SoConTxBufferStateType, AUTOMATIC, SOAD_APPL_DATA) TxBufferStatePtr,
  CONSTP2CONST(SoAd_SoConTxBufferCfgType, AUTOMATIC, SOAD_APPL_DATA) TxBufferCfgPtr,
  CONSTP2VAR(SoAd_BufLengthType, AUTOMATIC, SOAD_APPL_DATA) RemainingBufLengthPtr
);

/** \brief Service to copy data from internal buffer
 **
 ** The service copies data from internal Tx buffer. In case of last is best it requests the
 ** current data from UL.
 ** \param[in] SoConPtr - Pointer to socket connection configuration.
 ** \param[in/out] BufPtrPtr - Pointer to transmit buffer. It shall return the address of the next
 **                            free space.
 ** \param[in/out] RemainingBufLengthPtr - Pointer the remaining free length in the transmit buffer.
 **                                        It shall return the remaining free length in the transmit
 **                                        buffer after service has copied data from internal buffer.
 ** \return Std_ReturnType
 ** \retval E_OK - Success
 ** \retval E_NOT_OK - Failed
 **/
STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_CopyTxData_FromInternalBuffer
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_Uint8PtrType, AUTOMATIC, SOAD_APPL_DATA) BufPtrPtr,
  CONSTP2VAR(SoAd_BufLengthType, AUTOMATIC, SOAD_APPL_DATA) RemainingBufLengthPtr
);

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

FUNC(Std_ReturnType, SOAD_CODE) SoAd_IfTransmit
(
  PduIdType SoAdSrcPduId,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) SoAdSrcPduInfoPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_SOAD_IFTRANSMIT_ENTRY(SoAdSrcPduId, SoAdSrcPduInfoPtr);

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  if(SoAd_Initialized == SOAD_UNINITED)
  {
    SOAD_DET_REPORT_ERROR(SOAD_IFTRANSMIT_SVCID, SOAD_E_NOTINIT);
  }
  else if(SoAdSrcPduId >= SoAd_PBcfg->PduRouteCnt)
  {
    SOAD_DET_REPORT_ERROR(SOAD_IFTRANSMIT_SVCID, SOAD_E_INV_PDUID);
  }
  else if(SOAD_FLAG(&SOAD_CFG(PduRoute,SoAdSrcPduId), SOAD_PDUROUTE_FLAG_IS_UL_TP))
  {
    SOAD_DET_REPORT_ERROR(SOAD_IFTRANSMIT_SVCID, SOAD_E_INV_PDUID);
  }
  else if(SoAdSrcPduInfoPtr == NULL_PTR)
  {
    SOAD_DET_REPORT_ERROR(SOAD_IFTRANSMIT_SVCID, SOAD_E_PARAM_POINTER);
  }
  else if(SoAdSrcPduInfoPtr->SduLength == 0)
  {
    /* !LINKSTO SoAd.dsn.Udp.Tx.If.SduLengthNull.NotOK, 1 */
    SOAD_DET_REPORT_ERROR(SOAD_IFTRANSMIT_SVCID, SOAD_E_INV_ARG);
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    const SoAd_RouteIdType PduRouteId = SOAD_TX_PDUID_TO_PDUROUTE_ID(SoAdSrcPduId);
    CONSTP2CONST(SoAd_PduRouteType, AUTOMATIC, SOAD_APPL_DATA) PduRoutePtr =
        &SOAD_CFG(PduRoute, PduRouteId);
    const SoAd_PduRouteDestVirtualIdType PduRouteDestCnt = PduRoutePtr->PduRouteDestVirtualCnt;
    const SoAd_PduRouteDestVirtualIdType PduRouteDestLastId = PduRoutePtr->PduRouteDestVirtualFirstId + PduRouteDestCnt;
#if(SOAD_META_DATA_HANDLING == STD_ON)
    SoAd_SoConIdType SoConId_EcuC = SOAD_INVALID_SOCON_ID;
#endif

    /* Pdu fan-out: assuming all transmissions are going to succeed the result is now set to E_OK,
     * if a single transmission fails or all related routing groups are disabled the result will be
     * set to E_NOT_OK. */
    RetVal = E_OK;

#if(SOAD_META_DATA_HANDLING == STD_ON)
    if(SOAD_FLAG(PduRoutePtr, SOAD_PDUROUTE_FLAG_METADATA_SOCON_ID))
    {
      RetVal = EcuC_GetMetaDataSoConId(PduRoutePtr->EcuCPduId, SoAdSrcPduInfoPtr, &SoConId_EcuC);
    }

    if(E_OK == RetVal)
#endif
    {
      uint32 NextPduRouteDestVirtualId = 0U;
      uint32 StartBit = (uint32)PduRoutePtr->PduRouteDestVirtualFirstId - 1U;
      boolean AtLeastOneDestinationCalled = FALSE;

      /*Start Lookup at first Virtual Destination assosiated with this pduRoute*/
      CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestTxBitMapPtr =
        &SOAD_PBRAM_REF_ID(uint32, SoAd_PBcfg->PduRouteDestVirtualTxEnabledBitMapBase, 0U);
      /* loop over all pduRouteDests related to this PduRoutePtr */
      /* Deviation MISRAC2012-3 */
      for(SOAD_COMSTACK_BF_ITERATE(NextPduRouteDestVirtualId, &PduRouteDestTxBitMapPtr[0U], StartBit, PduRouteDestLastId))
      {
        CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr =
                    &SOAD_CFG(PduRouteDestVirtual, NextPduRouteDestVirtualId);

        CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr =
                    &SOAD_CFG(SoCon, PduRouteDestVirtualPtr->SoConId);
        CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr =
                    &SOAD_PBRAM(SoConState, PduRouteDestVirtualPtr->SoConId);
        Std_ReturnType RetVal_PduRouteDestTarget = E_NOT_OK;

#if(SOAD_META_DATA_HANDLING == STD_ON)
        boolean SendTx = FALSE;

        if(SOAD_FLAG(PduRoutePtr, SOAD_PDUROUTE_FLAG_METADATA_SOCON_ID))
        {
          /* Initially set RetVal to false in case that no matching dest was found. */
          RetVal = E_NOT_OK;
          if(SoConId_EcuC == PduRouteDestVirtualPtr->SoConId)
          {
            SendTx = TRUE;
          }
        }
        else
        {
          SendTx = TRUE;
        }

        if(SendTx)
#endif
        {
          const boolean IsTcp = SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_TCP) ? TRUE : FALSE;
          AtLeastOneDestinationCalled = TRUE;

          if(IsTcp)
          {
            RetVal_PduRouteDestTarget = SoAd_IfTransmitPduRouteDestTcp
            (
              SoConPtr,
              SoConStatePtr,
              SoAdSrcPduInfoPtr,
              PduRouteDestVirtualPtr
            );
          }
          else
          {
            RetVal_PduRouteDestTarget = SoAd_IfTransmitPduRouteDestUdp
            (
              SoConPtr,
              SoConStatePtr,
              SoAdSrcPduInfoPtr,
              PduRouteDestVirtualPtr
            );
          }

#if(SOAD_META_DATA_HANDLING == STD_ON)
          if(SOAD_FLAG(PduRoutePtr, SOAD_PDUROUTE_FLAG_METADATA_SOCON_ID))
          {
            /* Leave loop after transmission is done for dedicated SoConId */
            RetVal = RetVal_PduRouteDestTarget;
            break;
          }
          else
#endif
          {
            /* if a single transmission fails, E_NOT_OK shall be returned,
             * but processing shall still continue, other transmissions might succeed */
            if(RetVal_PduRouteDestTarget == E_NOT_OK)
            {
              RetVal = E_NOT_OK;
            }
          }
        }
      }


      /* In case something went wrong, SoAd_IfTransmit shall return E_NOT_OK and there shall be no
       * transmit confirmation! Since some of the transmissions might have been successful, their
       * state shall be set to IDLE so no confirmation is passed to the upper layer */
      if(RetVal == E_NOT_OK)
      {
        SoAd_IfTransmitSkipConf(SoAdSrcPduId);
      }

      /* if the RoutingGroups of all PduRouteDests are deactivated, E_NOT_OK shall be returned */
      if(!AtLeastOneDestinationCalled)
      {
        RetVal = E_NOT_OK;
      }
    }
  }

  DBG_SOAD_IFTRANSMIT_EXIT(RetVal, SoAdSrcPduId, SoAdSrcPduInfoPtr);
  return RetVal;
}

FUNC(void, SOAD_CODE) SoAd_TxConfirmation
(
  TcpIp_SocketIdType SocketId,
  uint16 Length
)
{
  DBG_SOAD_TXCONFIRMATION_ENTRY(SocketId, Length);

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  if(SoAd_Initialized == SOAD_UNINITED)
  {
    SOAD_DET_REPORT_ERROR(SOAD_TXCONFIRMATION_SVCID, SOAD_E_NOTINIT);
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    SoAd_SoConIdType SoConId;

    SOAD_ENTER_CRITSEC();
    {
      SoConId = SoAd_SocketToSoConId(SocketId);

      if(SoConId != SOAD_INVALID_SOCON_ID)
      {
        /* lock the socket */
        SOAD_PBRAM(SoConState, SoConId).BusyCnt++;
      }
    }
    SOAD_EXIT_CRITSEC();

    if(SoConId == SOAD_INVALID_SOCON_ID)
    {
      /* TcpIp socket was invalid or already closed. */
#if(SOAD_DEV_ERROR_DETECT == STD_ON)
      SOAD_DET_REPORT_ERROR(SOAD_TXCONFIRMATION_SVCID, SOAD_E_INV_SOCKETID);
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
    }
#if (SOAD_DEV_ERROR_DETECT == STD_ON)
    else if(SOAD_CFG(SoCon, SoConId).TxDataId == SOAD_INVALID_SOCON_ID)
    {
      /* Tx Confirmation for read-only socket */
      SOAD_DET_REPORT_ERROR(SOAD_TXCONFIRMATION_SVCID, SOAD_E_INV_SOCKETID);
    }
    else if(!SOAD_FLAG(&SOAD_CFG(SoCon, SoConId), SOAD_SOCON_FLAG_IS_TCP))
    {
      /* Tx Confirmation for UDP */
      SOAD_DET_REPORT_ERROR(SOAD_TXCONFIRMATION_SVCID, SOAD_E_INV_SOCKETID);
    }
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
    else if
    (
      /* Deviation MISRAC2012-6  <+2> */
      SOAD_FLAG_UP(&SOAD_PBRAM(SoConState, SoConId), SOAD_SOCONSTATE_FLAG_TX_TP_CANCELED)
      || SOAD_FLAG_UP(&SOAD_PBRAM(SoConState, SoConId), SOAD_SOCONSTATE_FLAG_CLOSE_SOCON)
    )
    {
      /* socket connection is currently in shutdown process - do not accept tx confirmation */
    }
    else
    {
      CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr =
          &SOAD_CFG(SoCon, SoConId);
      CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr =
          &SOAD_PBRAM(SoConState, SoConId);

      SoAd_TxConfirmation_ProvideConf(SoConPtr, SoConStatePtr, Length);
    }

    if(SoConId != SOAD_INVALID_SOCON_ID)
    {
      CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr =
        &SOAD_PBRAM(SoConState, SoConId);
      /* Unlock the socket. */
      SOAD_ENTER_CRITSEC();
      SoConStatePtr->BusyCnt--;
      SOAD_EXIT_CRITSEC();
    }
  }

  DBG_SOAD_TXCONFIRMATION_EXIT(SocketId, Length);
}

FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_CopyTxData
(
  TcpIp_SocketIdType SocketId,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  uint16 BufLength
)
{
#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  uint8 DelayDetErrId = SOAD_ERRID_NONE;
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */

  BufReq_ReturnType RetVal_BufReq = BUFREQ_E_NOT_OK;
  P2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr
          = NULL_PTR;

  DBG_SOAD_COPYTXDATA_ENTRY(SocketId, BufPtr, BufLength);

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  if(SoAd_Initialized == SOAD_UNINITED)
  {
    SOAD_DET_REPORT_ERROR(SOAD_COPYTXDATA_SVCID, SOAD_E_NOTINIT);
  }
  else if(BufPtr == NULL_PTR)
  {
    SOAD_DET_REPORT_ERROR(SOAD_COPYTXDATA_SVCID, SOAD_E_PARAM_POINTER);
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* get the index of the socket connection (it might be the first in a group) */
    SoAd_SoConIdType SoConFirstId = SoAd_SocketToSoConId(SocketId);

    SoAd_SoConIdType SoConId = SoAd_CopyTxData_GetUsedSoConId(SoConFirstId);

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
    /* report to Det if no related socket connection or active PDU route can be found
     * (this always applies for read-only socket connections) */
    if(SoConId == SOAD_INVALID_SOCON_ID)
    {
      SOAD_DELAY_DET_REPORT_ERROR(SOAD_E_INV_SOCKETID, &DelayDetErrId);
    }
    else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
    {
      CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr =
        &SOAD_CFG(SoCon, SoConId);
      CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr =
        &SOAD_PBRAM(SoConState, SoConId);

      /* The SoCon was already found which expects the SoAd_CopyTxData() call. The
         PduRouteDestVirtualId is always valid if this call is expected. */
      const SoAd_PduRouteDestVirtualIdType PduRouteDestVirtualId
        = SoAd_GetActivePduRouteDestVirtualId(SoConPtr);

      if(PduRouteDestVirtualId != SOAD_INVALID_DEST_VIRTUAL_ID)
      {
        PduRouteDestVirtualPtr = &SOAD_CFG(PduRouteDestVirtual, PduRouteDestVirtualId);
      }
      if(
          SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_TP_PENDING)
          && SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_TCP)
        )
      {
        SOAD_PRECONDITION_ASSERT
        (
          PduRouteDestVirtualId != SOAD_INVALID_DEST_VIRTUAL_ID, SOAD_COPYTXDATA_SVCID
        );
        RetVal_BufReq = SoAd_CopyTxData_TpTcp(SoConPtr, BufPtr, PduRouteDestVirtualPtr, BufLength);
      }
      /* IfTransmit TCP/UDP or UDP TP. Since for UDP TP all data need to be already available in
         buffer UDP TP can be processed in the same way as UDP IF. */
      else
      {
        RetVal_BufReq = SoAd_CopyTxData_BufferedData
        (
          SoConPtr, BufPtr, PduRouteDestVirtualPtr, BufLength
        );
      }
    }
  }
#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  SOAD_DELAY_DET_EXECUTE(SOAD_COPYTXDATA_SVCID, &DelayDetErrId);
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */

  DBG_SOAD_COPYTXDATA_EXIT(RetVal_BufReq, SocketId, BufPtr, BufLength);
  return RetVal_BufReq;
}


TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, SOAD_CODE) SoAd_PrepareTransmit
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(SoAd_SoConTxDataType, AUTOMATIC, SOAD_APPL_DATA) TxDataPtr,
  CONSTP2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) SoAdSrcPduInfoPtr,
  CONSTP2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_DATA) TcpIpTxLen,
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr
)
{

  Std_ReturnType RetVal = E_NOT_OK;
  const boolean RemoteIsWildcard
    = SOAD_IS_REMOTE_WILDCARD(SoConStatePtr->RemoteAddress, SoConStatePtr->RemotePort);

  DBG_SOAD_PREPARETRANSMIT_ENTRY
  (
    SoConPtr,
    SoConStatePtr,
    TxDataPtr,
    SoAdSrcPduInfoPtr,
    TcpIpTxLen,
    PduRouteDestVirtualPtr
  );

  /* Initially reset NPduUdpTxBufferIdx for NPDU feature and UDP retry feature since they use the
   same buffer. */
  if(
    SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_NPDUUDPTXBUFFER)
    || SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_UDP_RETRY)
  )
  {
    P2VAR(SoAd_SoConTxBufferStateType, AUTOMATIC, SOAD_APPL_DATA) TxBufferStatePtr = NULL_PTR;
    SoAd_GetSoConTxBufferState(SoConPtr, TxBufferStatePtr);
    /* Reset transmitted buffer position. */
    TxBufferStatePtr->NPduUdpTxBufferIdx = 0U;
  }

  /* calculate actual PDU length */
  *TcpIpTxLen = SoAdSrcPduInfoPtr->SduLength
    + (uint8)(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_PDU_HEADER_MODE) ? SOAD_PDU_HEADER_LEN : 0U);

  if(SoAd_CheckTcpIpSocketInUse(SoConPtr))
  {
    /* Tx in progress: deny request */
  }
  else if(SoAdSrcPduInfoPtr->SduLength == 0U)
  {
    /* Do not accept a transmission with no payload. */
  }
  else if(SoConStatePtr->SocketState != SOAD_SOCKETSTATE_ONLINE)
  {
    /* socket connection not connected: deny request */
  }
  else if(
           SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_TP_CANCELED)
           || SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RX_TP_CANCELED)
           || SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_CLOSE_SOCON)
           || SOAD_FLAG_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TCPIP_SOCKET_CLOSED)
           || SOAD_FLAG_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RECONNECT_SOCKET)
           || SOAD_FLAG_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TCP_FIN_RECEIVED)
         )
  {
    /* socket connection is currently shutting down */
  }
  else if(
       (!SOAD_FLAG_UP(SoConStatePtr,SOAD_SOCONSTATE_FLAG_REMOTE_SET))
         || RemoteIsWildcard
       )
  {
    /* remote address must be known for transmission */
  }
  else
  {
    if(SoConPtr->SoConTxTcpDataId != SOAD_INVALID_SOCON_ID)
    {
      P2VAR(SoAd_SoConTxTcpDataType, AUTOMATIC, SOAD_APPL_DATA) SoConTxTcpDataPtr =
          &SOAD_PBRAM(SoConTxTcpData, SoConPtr->SoConTxTcpDataId);
      if(
        (SoConTxTcpDataPtr->TxConfCtr + (*TcpIpTxLen)) >
        (SOAD_CFG(SoConGroup, SoConPtr->SoConGroupId).TxQuota)
      )
      {
        /* TX quota for this TCP socket connection would be exceeded */
      }
      else
      {
        /* PduTxData have same order as PduRouteDestVirtual. */
        P2VAR(SoAd_PduTxTcpDataType, AUTOMATIC, SOAD_APPL_DATA) PduTxTcpDataPtr =
          &SOAD_PBRAM(PduTxTcpData, PduRouteDestVirtualPtr->PduTxTcpDataId);
        SoConTxTcpDataPtr->TxConfCtr += *TcpIpTxLen;
        PduTxTcpDataPtr->PduTxConfCtr = SoConTxTcpDataPtr->TxConfCtr;

        /* TX quota not exceeded */
        RetVal = E_OK;
      }
    }
    else
    {
      RetVal = E_OK;
    }
    if(RetVal == E_OK)
    {
      /* transmission is possible */
      RetVal = E_OK;

      /* update members for transmission */
      TxDataPtr->PduPayloadRemainingLen = SoAdSrcPduInfoPtr->SduLength;
      TxDataPtr->PduHeaderWrittenLen = 0U;
      TxDataPtr->ULIfDataPtr = SoAdSrcPduInfoPtr->SduDataPtr;
      TxDataPtr->TcpIpTxRemainingLen = *TcpIpTxLen;
      TxDataPtr->ActivePduRouteDestVirtualId = PduRouteDestVirtualPtr->PduRouteDestVirtualId;

      SoAd_SetPduRouteDestVirtualState(PduRouteDestVirtualPtr, SOAD_PDUROUTEDESTVIRTUALSTATE_TXING);
    }
  }

  DBG_SOAD_PREPARETRANSMIT_EXIT
  (
  RetVal,
  SoConPtr,
  SoConStatePtr,
  TxDataPtr,
  SoAdSrcPduInfoPtr,
  TcpIpTxLen,
  PduRouteDestVirtualPtr
  );

  return RetVal;
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_MainFunctionTxIf
(
  void
)
{
  uint8 BusyCnt;
  P2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr;
  P2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr;
  uint32 NextSoConId;
  const SoAd_SoConIdType SoConMax = SoAd_PBcfg->SoConCnt;

  /* Tx IF Condition Change Map*/
  CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) TxIFCondChange
    = &SOAD_PBRAM_REF_ID(uint32, SoAd_PBcfg->SoConTxIFNotIdleBitMapBase, 0U);
  TS_AtomicAssign8(SoAd_TxIfCondChange, FALSE);

  /* Deviation MISRAC2012-3 */
  for(SOAD_COMSTACK_BF_ITERATE(NextSoConId, &TxIFCondChange[0U], COMSTACK_BF_START, SoConMax))
  {
    uint8 SoConStateLowerFlags;
    SoConPtr = &SOAD_CFG(SoCon, NextSoConId);
    SoConStatePtr = &SOAD_PBRAM(SoConState, NextSoConId);
    /* Else branch is not testable. Nevertheless, it is necessary to avoid TcpIp calls with an
     * invalid SocketId if SoAd_IfTransmit gets interrupted by SoAd_TcpIpEvent and SoAd_Mainfunction
     */
    /* CHECK: NOPARSE */
    if(!(SOAD_FLAG_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TCPIP_SOCKET_CLOSED)))
    {
      /* CHECK: PARSE */
      /* Before anything else, clear the Status Bit in the map, because preemption could already
      set it during the Rx Action */
      SoAd_ClearBitInBitMap(SoConPtr,&SoAd_PBcfg->SoConTxIFNotIdleBitMapBase);


      /* If data is available in the receive buffer it shall be processed. */
      TS_AtomicAssign8(SoConStateLowerFlags, SoConStatePtr->LowerFlags);

      /* BusyCnt ensures that SoAd_MainFunction() does not interrupt an Rx/Tx triggering API
       request. */
      TS_AtomicAssign8(BusyCnt, SoConStatePtr->BusyCnt);

      if(BusyCnt == 0U)
      {
        /* Only UDP IF socket connections shall be handled here. If IF TCP shall be handled as well
         during main function then the precondition assert needs to be converted to a proper check.
         */
        SOAD_PRECONDITION_ASSERT
        (
            !SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_TCP), SOAD_MAINFUNCTION_SVCID
        );

        {
          /* UDP */
          const uint8 RetryUdpPDUFlag
          = SoConStateLowerFlags & SOAD_SOCONSTATE_FLAG_TX_UDP_RETRY_MASK;
          uint8 TxConfFlag;

          /* If a previous TcpIp_UdpTransmit call with a IF-PDU returned TCPIP_E_PHYS_ADDR_MISS,
           retry the transmission now. */
          if((SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_NPDUUDPTXBUFFER)) || (RetryUdpPDUFlag > 0U))
          {
            SoAd_MainFunctionIfTxUdp(SoConPtr, SoConStatePtr);
          }

          /* Check for outstanding UDP transmit confirmations.
           * Note: It is important to check SOAD_SOCONSTATE_FLAG_TXCONF_PENDING at the end and
           * refresh the flag, because a previous UDP retransmission might have effected the flag.
           */
          TS_AtomicAssign8(SoConStateLowerFlags, SoConStatePtr->LowerFlags);
          TxConfFlag = SoConStateLowerFlags & SOAD_SOCONSTATE_FLAG_TXCONF_PENDING_MASK;
          if(TxConfFlag != 0U)
          {
            SoAd_MainFunctionUdpTxConf(SoConPtr, SoConStatePtr);
          }
        }
      }
      else
      {
        /* Prepare the Map to open this SoCon with the next Main Function */
        SoAd_SetBitInBitMap(SoConPtr,&SoAd_PBcfg->SoConTxIFNotIdleBitMapBase,&SoAd_TxIfCondChange);
      }
    }
    else
    {
      /* in case that SoCon is closed by TcpIp the buffer should already be cleaned up in
       * state machine of the mainfunction.
       */
      SOAD_UNREACHABLE_CODE_ASSERT(SOAD_MAINFUNCTION_SVCID);
    }
  }
}

/** \brief Checks if RoutingGroups were triggered and handles the related transmissions accordingly.
 */
TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_HandleTriggeredRoutingGroups(void)
{
  uint32 NextBit;
  const SoAd_RoutingGroupIdType RoutingGroupTriggerableCnt =
        SoAd_PBcfg->RoutingGroupTriggerableCnt;
  CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) RoutingGroupTriggeredMapPtr =
                    &SOAD_PBRAM_REF_ID(uint32, SoAd_PBcfg->RoutingGroupTriggeredBitMapBase, 0U);
  uint32 StartBit = COMSTACK_BF_START;
  DBG_SOAD_HANDLETRIGGEREDROUTINGGROUPS_ENTRY();

  /* Deviation MISRAC2012-3 */
  for(SOAD_COMSTACK_BF_ITERATE(NextBit,&RoutingGroupTriggeredMapPtr[0U],StartBit,RoutingGroupTriggerableCnt)
     )
  {
    SoAd_PduRouteDestVirtualIdType MapId;
    SoAd_RoutingGroupIdType InternalRoutingGroupId = (SoAd_RoutingGroupIdType)NextBit;
    P2CONST(SoAd_RoutingGroupPduRouteDestMapType, AUTOMATIC, SOAD_APPL_DATA) RoutingGroupTriggerPtr = &SOAD_CFG_TYPE
        (
          SoAd_RoutingGroupPduRouteDestMapType,
          RoutingGroupPduRouteDestMap,
          InternalRoutingGroupId
        );
    const SoAd_PduRouteDestVirtualIdType MaxPduRouteDestMap =
        RoutingGroupTriggerPtr->PduRouteDestVirtualMapFirstId
        + RoutingGroupTriggerPtr->PduRouteDestVirtualMapCnt;

    /* loop over all PduRouteDests related to this routing group */
    for(MapId = RoutingGroupTriggerPtr->PduRouteDestVirtualMapFirstId;
        MapId < MaxPduRouteDestMap;
        MapId++
       )
    {

      SoAd_PduRouteDestVirtualIdType  PduRouteDestVirtualId =
        SOAD_CFG_TYPE(SoAd_PduRouteDestVirtualIdType, RoutingGroupPduRouteDestVirtualMap, MapId);
      SoAd_RouteIdType PduRouteId = SOAD_CFG(PduRouteDestVirtual, PduRouteDestVirtualId).PduRouteId;
      CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr =
          &SOAD_CFG(PduRouteDestVirtual, PduRouteDestVirtualId);
      CONSTP2CONST(SoAd_PduRouteType, AUTOMATIC, SOAD_APPL_DATA) PduRoutePtr = &SOAD_CFG(PduRoute,PduRouteId);

      if(!SOAD_FLAG(PduRoutePtr, SOAD_PDUROUTE_FLAG_IS_UL_TP))
      {

        Std_ReturnType RetVal;
        CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) TriggerTransmitBufPtr =
          &SOAD_PBRAM_REF_ID(uint8, SoAd_PBcfg->TriggerTransmitBufferBufId, 0U);
        PduInfoType PduInfo;
        PduInfo.SduLength = SoAd_PBcfg->TriggerTransmitBufferCapacity;
        PduInfo.SduDataPtr = TriggerTransmitBufPtr;

        /* RoutingGroupTriggerPtr only contains If routing groups (MCG feature). */

        /* request data from the upper layer */
        RetVal = SoAd_ULIfTriggerTransmit(PduRouteId, &PduInfo);

        #if (SOAD_DEV_ERROR_DETECT == STD_ON)
                /* Note: Unfortunately SoAd has no knowledge of PDU length beforehand (SoAd doesn't store
                 * PDU length info and length might be dynamic!), so it can't be ensured that the buffer
                 * is large enough. If a misbehaving upper layer exceeds the configured max PDU size,
                 * i.e. the available buffer a development error shall be reported. */
                if(PduInfo.SduLength > SoAd_PBcfg->TriggerTransmitBufferCapacity)
                {
                  SOAD_DET_REPORT_ERROR(SOAD_MAINFUNCTION_SVCID, SOAD_E_TRIGGERTXBUF);
                }
        #endif /* SOAD_DEV_ERROR_DETECT == STD_ON */

        /* if the upper layer copied data */
        if(RetVal == E_OK)
        {

          /* For global routing the PduRouteDestVirtualPtr->SoConId is the SoConId of the routing group.
             For specific routing the offset between first internal routing group and
             triggered internal routing group of the same external routing group equals the
             offset from PduRouteDestVirtualPtr->SoConId (first Id of SoConGroup) and required
             SoConId. */
          CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr =
              &SOAD_CFG(SoCon, PduRouteDestVirtualPtr->SoConId);
          CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr =
              &SOAD_PBRAM(SoConState, PduRouteDestVirtualPtr->SoConId);

          /* Start transmission on the related PduRouteDest.
           * Ignore the return value, don't care if the transmission was successful. */
          const boolean IsTcp = SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_TCP) ? TRUE : FALSE;

          if(IsTcp)
          {
            (void)SoAd_IfTransmitPduRouteDestTcp
            (
              SoConPtr,
              SoConStatePtr,
              &PduInfo,
              PduRouteDestVirtualPtr
            );
          }
          else
          {
            (void) SoAd_IfTransmitPduRouteDestUdp
            (
              SoConPtr,
              SoConStatePtr,
              &PduInfo,
              PduRouteDestVirtualPtr
            );
          }

          /* Clear RX/TX flag to unlock state machine transitions to offline. */
          SOAD_FLAG_CLR_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RXTX_PROGRESS);
        }
      }
    }

    SOAD_ENTER_CRITSEC();

    /* clear the trigger for this routing group */
    SOAD_CLEAR_ROUTINGGROUP_TRIGGER(InternalRoutingGroupId);

    SOAD_EXIT_CRITSEC();
  }

  DBG_SOAD_HANDLETRIGGEREDROUTINGGROUPS_EXIT();
}

TS_MOD_PRIV_DEFN FUNC(uint8, SOAD_CODE) SoAd_CopyTxData_CopyPduHeader
(
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr,
  CONSTP2VAR(SoAd_SoConTxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConTxDataPtr,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  CONSTP2VAR(SoAd_BufLengthType, AUTOMATIC, SOAD_APPL_DATA) RemainingBufLengthPtr
)
{
  uint8 CopyNow = 0U;

  /* Check if a part of the header is still not copied. */
  if(SoConTxDataPtr->PduHeaderWrittenLen < SOAD_PDU_HEADER_LEN)
  {
    /* Write (part of) header */
    /* Index of first byte of PDU header to be written now */
    const uint8 HeaderWrittenBefore = SoConTxDataPtr->PduHeaderWrittenLen;
    const SoAd_BufLengthType TransmittedPduLength = HeaderWrittenBefore + (*RemainingBufLengthPtr);

    /* 1 + index of last byte of PDU header to be written now */
    const uint8 HeaderWrittenTotal = (uint8)
      SOAD_MIN
      (
        TransmittedPduLength,
        SOAD_PDU_HEADER_LEN
      );

    /* TcpIpTxRemainingLen already contains payload length + header. To get the payload length
       only (to fill in the header) the not processed header bytes needs to be subtracted. */
    const SoAd_BufLengthType RemainingHeaderLen =
      (SoAd_BufLengthType)SOAD_PDU_HEADER_LEN - (SoAd_BufLengthType)HeaderWrittenBefore;
    /* This cast is protected by SoAd_CopyTxData(), can't copy more than uint16
     * (parameter BufLength is uint16) */
    const SoAd_BufLengthType PduHeaderLengthField = (SoAd_BufLengthType)(
      SoConTxDataPtr->TcpIpTxRemainingLen - RemainingHeaderLen);

    CopyNow = HeaderWrittenTotal - HeaderWrittenBefore;

    if(CopyNow > 0U)
    {
      SoAd_WritePduHeader
      (
        BufPtr,
        SOAD_CFG(PduRouteDestCommon, PduRouteDestVirtualPtr->PduRouteDestCommonId).PduHeaderId,
        (uint32)PduHeaderLengthField,
        HeaderWrittenBefore,
        CopyNow
      );
    }

    SoConTxDataPtr->PduHeaderWrittenLen = HeaderWrittenTotal;
    *RemainingBufLengthPtr -= CopyNow;
    /* Remove copied header from remaining total length. This is required for TCP data
       confirmation. */
    SoConTxDataPtr->TcpIpTxRemainingLen -= CopyNow;
  }
  return CopyNow;
}


/*==================[internal function definitions]=========================*/

STATIC FUNC(void, SOAD_CODE) SoAd_MainFunctionIfTxUdp
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
#if (SOAD_IPV6_SUPPORT == STD_ON)
  TcpIp_SockAddrInet6Type RemoteAddr;
#else
  TcpIp_SockAddrInetType RemoteAddr;
#endif /* (SOAD_IPV6_SUPPORT == STD_ON) */
  /* Deviation MISRAC2012-4 */
  CONSTP2VAR(TcpIp_SockAddrType, AUTOMATIC, AUTOMATIC) RemoteAddrPtr =
      (P2VAR(TcpIp_SockAddrType, TYPEDEF, AUTOMATIC))
      &RemoteAddr;
  CONSTP2VAR(SoAd_SoConGroupStateType, AUTOMATIC, SOAD_APPL_DATA) SoConGroupStatePtr =
    &SOAD_PBRAM(SoConGroupState, SoConPtr->SoConGroupId);

  P2VAR(SoAd_SoConTxBufferStateType, AUTOMATIC, SOAD_APPL_DATA) TxBufferStatePtr = NULL_PTR;

  boolean TransmitTxBuffer = FALSE;

  DBG_SOAD_MAINFUNCTIONIFTXUDP_ENTRY(SoConPtr, SoConStatePtr);

  SoAd_GetSoConTxBufferState(SoConPtr, TxBufferStatePtr);

  SOAD_ENTER_CRITSEC();

  /* the timer is deactivated if nPduUdpTxTimer is set to 0 */
  if(TxBufferStatePtr->TxTimer != 0U)
  {
    /* decrease the running timer each MainFunction */
    TxBufferStatePtr->TxTimer--;

    /* if the timeout expires */
    if(TxBufferStatePtr->TxTimer == 0U)
    {
      /* Set flag to call TcpIp_UdpTransmit outside of the exclusive area.
         BusyCnt ensures that no SoAd_IfTransmit() is interrupted by this main function
         for this SoCon. */
      TransmitTxBuffer = TRUE;

      /* Shadow buffer start index shall beginn after TxLen of stored PDUs. This will
         reduce the shadow buffer size to  buffer capacity - TxLen. */
      TxBufferStatePtr->ShadowBufferStartIdx = TxBufferStatePtr->TxLen;

      /* Indicate that buffer is locked for this socket connection. */
      SOAD_FLAG_SET_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_BUFFER_LOCK);

      /* While Tx Buffer is locked also set SoConGroup occupied */
      SOAD_FLAG_SET(SoConGroupStatePtr, SOAD_SOCONGROUP_FLAG_UDP_SOCKET_IN_USE);


      /* set remote address within the critical section */
      SoAd_AssignRemoteAddressSoCon2SockAddrType(SoConPtr, SoConStatePtr, RemoteAddrPtr);
    }
    else
    {
      SoAd_SetBitInBitMap(SoConPtr,&SoAd_PBcfg->SoConTxIFNotIdleBitMapBase,&SoAd_TxIfCondChange);
    }
  }
  SOAD_EXIT_CRITSEC();

  if(TransmitTxBuffer)
  {
    TcpIp_ReturnType RetVal_TcpIp;

    /* call TcpIp API*/
    /* Udp frame cannot be bigger than 64k, cast of TxLen is protected by check in
     * SoAd_PrepareUdpIfTransmit
     */
    SOAD_FLAG_SET_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_COPYTXDATA);
    RetVal_TcpIp = TcpIp_UdpTransmit
                (
                  SoConStatePtr->SocketId,
                  NULL_PTR,
                  RemoteAddrPtr,
                  (uint16)TxBufferStatePtr->TxLen
                );
    SOAD_FLAG_CLR_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_COPYTXDATA);

    SOAD_ENTER_CRITSEC();
    switch(RetVal_TcpIp)
    {
      case TCPIP_OK:
      {
        SoAd_CleanUpUdpTxBuffer(SoConPtr, SoConStatePtr, TRUE);
        /* set a flag for UDP socket connection, that the transmit confirmation is
         * called in the context of the next SoAd_MainFunctionUdpTxConf() */
        SOAD_FLAG_SET_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TXCONF_PENDING);
        SoAd_SetBitInBitMap(SoConPtr,&SoAd_PBcfg->SoConTxIFNotIdleBitMapBase,&SoAd_TxIfCondChange);
        break;
      }
      case TCPIP_E_PHYS_ADDR_MISS:
      {
        if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_UDP_RETRY))
        {
          /* Note: If the UDP Retry feature is enabled for this socket connection and the physical
           * remote address is still unknown (i.e. TcpIp_UdpTransmit returns
           * TCPIP_E_PHYS_ADDR_MISS) PDU data is stored in the Tx buffer, the transmission will
           * be retried in the next MainFunction */
          /* To retry the transmission in the next MainFunction simply recharge the nPduUdpTxTimer. */
          TxBufferStatePtr->TxTimer = 1U;
          /* Retry the transmission in the next MainFunction */
          SOAD_FLAG_SET_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_UDP_RETRY);
          SoAd_SetBitInBitMap(SoConPtr,&SoAd_PBcfg->SoConTxIFNotIdleBitMapBase,&SoAd_TxIfCondChange);
        }
        else
        {
          SoAd_CleanUpUdpTxBuffer(SoConPtr, SoConStatePtr, FALSE);
        }
        break;
      }
      default:
      {
        SoAd_CleanUpUdpTxBuffer(SoConPtr, SoConStatePtr, FALSE);
        break;
      }
    }
    /* Unlock Tx buffer */
    SOAD_FLAG_CLR_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_BUFFER_LOCK);

    /* When unlocking Buffer also release Group */
    SOAD_FLAG_CLR(SoConGroupStatePtr, SOAD_SOCONGROUP_FLAG_UDP_SOCKET_IN_USE);

    SoAd_RestoreUdpIfShadowBuffer(SoConPtr);

    SOAD_EXIT_CRITSEC();
  }

  DBG_SOAD_MAINFUNCTIONIFTXUDP_EXIT(SoConPtr, SoConStatePtr);
}


STATIC FUNC(void, SOAD_CODE) SoAd_MainFunctionUdpTxConf
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
)
{
  /* loop over all PduRoutes of this socket connections, there might be more than one
   * pending transmit confirmation */
  const SoAd_PduTxDataIdType MaxPduTxData = SoConPtr->PduTxDataFirstId + SoConPtr->PduTxDataCnt;

  P2VAR(SoAd_PduRouteDestVirtualDataType, AUTOMATIC, SOAD_APPL_DATA) PduTxDataPtr;
  SoAd_PduRouteDestVirtualStateType PduTxState;
  /* Initialize with previous. Underflow is covered by ComStack_FindNextOne() */
  uint32 StartBit = SoConPtr->PduTxDataFirstId - (uint32)1U;
  uint32 NextBit;
  CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) PduTxDataNotIdleBitMap =
              &SOAD_PBRAM_REF_ID(uint32, SoAd_PBcfg->PduTxDataNotIdleBitMapBase, 0U);

  DBG_SOAD_MAINFUNCTIONUDPTXCONF_ENTRY(SoConPtr, SoConStatePtr);

  /* assuming all pending PDUs can be confirmed the TXCONF_PENDING flag can be cleared */
  SOAD_FLAG_CLR_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TXCONF_PENDING);

  /* loop over all PduRoutes related to this socket connection */
  /* !LINKSTO SoAd.dsn.IndicationBitMaps, 1 */
  /* Deviation MISRAC2012-3 */
  for(SOAD_COMSTACK_BF_ITERATE(NextBit, &PduTxDataNotIdleBitMap[0U], StartBit, MaxPduTxData))
  {
    P2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr;
    SoAd_GetPduRouteDestVirtualFromPduTxDataId(NextBit, &PduRouteDestVirtualPtr);
    PduTxDataPtr = &SOAD_PBRAM(PduRouteDestVirtualData, PduRouteDestVirtualPtr->PduRouteDestVirtualId);

    TS_AtomicAssign8(PduTxState, PduTxDataPtr->PduRouteDestVirtualState);

    /* Check if a transmit confirmation is pending. */
    if(PduTxState == SOAD_PDUROUTEDESTVIRTUALSTATE_WAITCONF)
    {
      /* Reset the state of this destination to IDLE */
      SoAd_SetPduRouteDestVirtualState(PduRouteDestVirtualPtr, SOAD_PDUROUTEDESTVIRTUALSTATE_IDLE);

      /* Pdu fan-out: if no destination is waiting for a TCP acknowledgment or
         UDP retransmission, the upper layer shall be informed */
      if(SoAd_PduFanOutPduTxConfPending(PduRouteDestVirtualPtr->PduRouteId) == FALSE)
      {
        SoAd_ULIfTxConfirmation(PduRouteDestVirtualPtr->PduRouteId);
      }
    }
  }

  DBG_SOAD_MAINFUNCTIONUDPTXCONF_EXIT(SoConPtr, SoConStatePtr);
}

STATIC FUNC(SoAd_SoConIdType, SOAD_CODE) SoAd_CopyTxData_GetUsedSoConId
(
  SoAd_SoConIdType SoConFirstId
)
{
  SoAd_SoConIdType SoConId_Found = SOAD_INVALID_SOCON_ID;

  if(SoConFirstId != SOAD_INVALID_SOCON_ID)
  {
    SoAd_SoConIdType Idx;
    CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) FirstSoConPtr =
      &SOAD_CFG(SoCon, SoConFirstId);
    CONSTP2CONST(SoAd_SoConGroupType, AUTOMATIC, SOAD_APPL_DATA) SoConGroupPtr =
      &SOAD_CFG(SoConGroup, FirstSoConPtr->SoConGroupId);
    const SoAd_SoConIdType SoConCnt = SoConGroupPtr->SoConCnt;

    /* loop over all socket connection of this group */
    for(Idx = 0U; Idx < SoConCnt; Idx++)
    {
      P2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr =
        &SOAD_PBRAM(SoConState, SoConGroupPtr->SoConFirstId + Idx);

      if(SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_COPYTXDATA))
      {
        SoConId_Found = SoConGroupPtr->SoConFirstId + Idx;
        break;
      }
    }
  }

  return SoConId_Found;
}


STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_IfTransmitPduRouteDestTcp
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) SoAdSrcPduInfoPtr,
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  P2VAR(SoAd_SoConTxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConTxDataPtr = NULL_PTR;

  PduLengthType TcpIpTxLen = 0U;

  SoAd_GetSoConTxData(SoConPtr, SoConTxDataPtr);

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO SoAd.Dsn.Func0101.Tx.Tcp.If.Det.SduDataPtr.NullPtr,1 */
  if(SoAdSrcPduInfoPtr->SduDataPtr == NULL_PTR)
  {
    SOAD_DET_REPORT_ERROR(SOAD_IFTRANSMIT_SVCID, SOAD_E_INV_ARG);
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    SOAD_ENTER_CRITSEC(); /* CS1 */

    RetVal = SoAd_PrepareTransmit(
                                   SoConPtr,
                                   SoConStatePtr,
                                   SoConTxDataPtr,
                                   SoAdSrcPduInfoPtr,
                                   &TcpIpTxLen,
                                   PduRouteDestVirtualPtr
                                 );

    if(RetVal == E_OK)
    {
      SoConStatePtr->BusyCnt++;
    }
    SOAD_EXIT_CRITSEC();

    if(RetVal == E_OK)
    {
      /* Try to send. */
      SOAD_FLAG_SET_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_COPYTXDATA);
      RetVal = TcpIp_TcpTransmit(
                                  SoConStatePtr->SocketId,
                                  NULL_PTR,
                                  TcpIpTxLen,
                                  TRUE
                                );
      SOAD_FLAG_CLR_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_COPYTXDATA);

      SOAD_ENTER_CRITSEC(); /* CS2 */

      SoConStatePtr->BusyCnt--;

      if(RetVal == E_OK)
      {
        SOAD_PRECONDITION_ASSERT(SoConTxDataPtr->TcpIpTxRemainingLen == 0U, SOAD_IFTRANSMIT_SVCID);

        SoAd_SetPduRouteDestVirtualState
        (
          PduRouteDestVirtualPtr, SOAD_PDUROUTEDESTVIRTUALSTATE_WAITCONF
        );
      }
      else
      {

        P2VAR(SoAd_PduTxTcpDataType, AUTOMATIC, SOAD_APPL_DATA) PduTxTcpDataPtr = NULL_PTR;
        P2VAR(SoAd_SoConTxTcpDataType, AUTOMATIC, SOAD_APPL_DATA) SoConTxTcpDataPtr = NULL_PTR;

        /* Since TcpIp_TcpTransmit() is called ForceRetrieve = TRUE no data is sent if E_NOT_OK
           is returned. */
        SOAD_PRECONDITION_ASSERT
        (
          SoConTxDataPtr->TcpIpTxRemainingLen == TcpIpTxLen, SOAD_IFTRANSMIT_SVCID
        );

        /* Because of conditions in generator this PduRouteDestVirtualPtr must always have a
           PduTxTcpDataId defined here. */
        SoAd_GetPduTxTcpData(PduRouteDestVirtualPtr,PduTxTcpDataPtr);
        PduTxTcpDataPtr->PduTxConfCtr = 0U;
        /* Because of conditions in generator this SoConPtr must always have a SoConTxTcpDataId
           defined here. */
        SoAd_GetSoConTxTcpData(SoConPtr, SoConTxTcpDataPtr);
        /* Revert the TxConfCtr which was already set in SoAd_PrepareTransmit(). */
        SoConTxTcpDataPtr->TxConfCtr -= TcpIpTxLen;

        /* Reset the transmission state. */
        SoAd_SetPduRouteDestVirtualState(PduRouteDestVirtualPtr, SOAD_PDUROUTEDESTVIRTUALSTATE_IDLE);

        /* and the remote address setting shall be reset too */
        if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_AUTO_SETUP))
        {
          SoAd_ResetWildCardSoCon(SoConPtr, SoConStatePtr);
        }
      }

      /* Cleanup */
      SoConTxDataPtr->PduPayloadRemainingLen = 0U;
      SoConTxDataPtr->ULIfDataPtr = NULL_PTR;
      SoConTxDataPtr->ActivePduRouteDestVirtualId = SOAD_INVALID_DEST_VIRTUAL_ID;
      SoConTxDataPtr->TcpIpTxRemainingLen = 0U;

      SOAD_EXIT_CRITSEC();
    }
  }
  return RetVal;
}

STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_IfTransmitPduRouteDestUdp
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) SoAdSrcPduInfoPtr,
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  P2VAR(SoAd_SoConTxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConTxDataPtr = NULL_PTR;
  /* variables set in critical section */
#if(SOAD_IPV6_SUPPORT == STD_ON)
  TcpIp_SockAddrInet6Type RemoteAddr;
#else
  TcpIp_SockAddrInetType RemoteAddr;
#endif /* (SOAD_IPV6_SUPPORT == STD_ON) */
  /* Deviation MISRAC2012-4 */
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, AUTOMATIC) RemoteAddrPtr =
      (P2VAR(TcpIp_SockAddrType, AUTOMATIC, AUTOMATIC))
      &RemoteAddr;
  CONSTP2VAR(SoAd_SoConGroupStateType, AUTOMATIC, SOAD_APPL_DATA) SoConGroupStatePtr =
      &SOAD_PBRAM(SoConGroupState, SoConPtr->SoConGroupId);
  SoAd_TxModeType TxMode = SOAD_TX_MODE_NONE;
  boolean BufferSendingRequired = FALSE;
  TcpIp_ReturnType RetVal_TcpIp;

  SoAd_GetSoConTxData(SoConPtr, SoConTxDataPtr);
  SOAD_ENTER_CRITSEC(); /* CS1 */

  /* Check if Tx is locked due to release of SoCon */
  if(!SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_LOCK))
  {
    /* check preconditions and prepare tx data structures */
    RetVal = SoAd_PrepareUdpIfTransmit
    (
      SoConPtr,
      SoConStatePtr,
      SoConTxDataPtr,
      SoAdSrcPduInfoPtr,
      &TxMode, /* TxModePtr */
      &BufferSendingRequired, /* SendBufferPtr */
      PduRouteDestVirtualPtr,
      RemoteAddrPtr
    );
  }

  SOAD_EXIT_CRITSEC();

  if(BufferSendingRequired)
  {
    RetVal = SoAd_IfTransmitPduRouteDestUdp_SendBuffer
    (
      SoConPtr,
      SoConStatePtr,
      RemoteAddrPtr,
      PduRouteDestVirtualPtr
    );

    if(E_OK != RetVal)
    {
      /* Only continue sending if previous transmission was successful. */
      TxMode = SOAD_TX_MODE_NONE;
    }
  }

  switch(TxMode)
  {
    case SOAD_TX_MODE_DIRECT:
    {
      RetVal_TcpIp = TcpIp_UdpTransmit
      (
        SoConStatePtr->SocketId,
        SoAdSrcPduInfoPtr->SduDataPtr,
        RemoteAddrPtr,
        /* cast protected by check from SoAd_TpTransmit(), UDP protocol can only send up to 64k */
        (uint16)SoAdSrcPduInfoPtr->SduLength
      );
      RetVal = SoAd_IfTransmit_UdpTransmitRetValHandling
      (
        SoConPtr,
        SoConStatePtr,
        SoAdSrcPduInfoPtr,
        PduRouteDestVirtualPtr,
        RetVal_TcpIp,
        0U /* No remaining bytes for direct transmission. */
      );
      break;
    }
    case SOAD_TX_MODE_INDIRECT:
    {
      SoAd_BufLengthType BufferLength = 0U;
      P2VAR(SoAd_BufLengthType, AUTOMATIC, SOAD_APPL_DATA) BufferLengthPtr = &BufferLength;
      if(SOAD_INVALID_SOCON_ID != SoConPtr->TxBufferId)
      {
        P2VAR(SoAd_SoConTxBufferStateType, AUTOMATIC, SOAD_APPL_DATA) TxBufferStatePtr = NULL_PTR;
        SoAd_GetSoConTxBufferState(SoConPtr, TxBufferStatePtr);
        BufferLengthPtr = &TxBufferStatePtr->TxLen;
      }
      else
      {
        BufferLengthPtr = &BufferLength;
      }
      SOAD_FLAG_SET_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_COPYTXDATA);
      /* Udp frame cannot be bigger than 64k, cast of TxLen is protected by check in
       * SoAd_PrepareUdpIfTransmit
       */
      RetVal_TcpIp = TcpIp_UdpTransmit
      (
        SoConStatePtr->SocketId,
        NULL_PTR,
        RemoteAddrPtr,
        /* cast protected by TpCapacity buffer which is uint16 */
       (uint16)*BufferLengthPtr + (uint16)SoConTxDataPtr->TcpIpTxRemainingLen
      );
      SOAD_FLAG_CLR_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_COPYTXDATA);
      /* Udp frame cannot be bigger than 64k, cast of TxLen is protected by check in
       * SoAd_PrepareUdpIfTransmit
       */
      RetVal = SoAd_IfTransmit_UdpTransmitRetValHandling
      (
        SoConPtr,
        SoConStatePtr,
        SoAdSrcPduInfoPtr,
        PduRouteDestVirtualPtr,
        RetVal_TcpIp,
        /* cast protected by TpCapacity buffer which is uint16 */
        /* Content of BufferLengthPtr and TcpIpTxRemainingLen got modified in context of
           TcpIp_UdpTransmit(). */
       (uint16)*BufferLengthPtr + (uint16)SoConTxDataPtr->TcpIpTxRemainingLen
      );
      break;
    }
    case SOAD_TX_MODE_BUFFERING:
    {
      SOAD_ENTER_CRITSEC();
      /* Try to store the Udp transmit request. */
      RetVal = SoAd_IfTransmit_UdpStoreToBuffer
      (
        SoConPtr, /* SoConPtr */
        SoConStatePtr,/* SoConStatePtr */
        SoAdSrcPduInfoPtr, /* SoAdSrcPduInfoPtr */
        PduRouteDestVirtualPtr /* PduRouteDestVirtualPtr */
      );
      SOAD_EXIT_CRITSEC();
      break;
    }
    case SOAD_TX_MODE_NONE:
    {
      break;
    }
    /* CHECK: NOPARSE */
    default:
    {
      SOAD_UNREACHABLE_CODE_ASSERT(SOAD_IFTRANSMIT_SVCID);
      break; /* Caution: this 'break' is required by MISRA-C:2012 */
    }
    /* CHECK: PARSE */
  }

  if((SOAD_TX_MODE_NONE != TxMode) || BufferSendingRequired)
  {
    SOAD_ENTER_CRITSEC();
    SoConStatePtr->BusyCnt--;

    /* Unlock SoConGroup */
    SOAD_FLAG_CLR(SoConGroupStatePtr, SOAD_SOCONGROUP_FLAG_UDP_SOCKET_IN_USE);

    /* Cleanup */
    SoConTxDataPtr->PduPayloadRemainingLen = 0U;
    SoConTxDataPtr->ULIfDataPtr = NULL_PTR;
    SoConTxDataPtr->ActivePduRouteDestVirtualId = SOAD_INVALID_DEST_VIRTUAL_ID;
    SoConTxDataPtr->TcpIpTxRemainingLen = 0U;

    if(SOAD_INVALID_SOCON_ID != SoConPtr->TxBufferId)
    {
      SoAd_RestoreUdpIfShadowBuffer(SoConPtr);
    }
    SOAD_EXIT_CRITSEC();
  }

  return RetVal;
}


STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_IfTransmitPduRouteDestUdp_SendBuffer
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, AUTOMATIC) RemoteAddrPtr,
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr
)
{
  TcpIp_ReturnType RetVal_TcpIp;
  Std_ReturnType RetVal;
  /* initialized to avoid warning */
  P2VAR(SoAd_SoConTxBufferStateType, AUTOMATIC, SOAD_APPL_DATA) TxBufferStatePtr = NULL_PTR;
  SoAd_GetSoConTxBufferState(SoConPtr, TxBufferStatePtr);

  /* call TcpIp API*/
  SOAD_FLAG_SET_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_COPYTXDATA);
  /* Udp frame cannot be bigger than 64k, cast of TxLen is protected by check in
   * SoAd_PrepareUdpIfTransmit
   */
  RetVal_TcpIp = TcpIp_UdpTransmit
              (
                SoConStatePtr->SocketId,
                NULL_PTR,
                RemoteAddrPtr,
                (uint16)TxBufferStatePtr->TxLen
              );
  SOAD_FLAG_CLR_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_COPYTXDATA);
  /* Udp frame cannot be bigger than 64k, cast of TxLen is protected by check in
   * SoAd_PrepareUdpIfTransmit
   */
  RetVal = SoAd_IfTransmit_UdpTransmitRetValHandling
  (
    SoConPtr,
    SoConStatePtr,
    NULL_PTR,
    PduRouteDestVirtualPtr,
    RetVal_TcpIp,
    (uint16)TxBufferStatePtr->TxLen
  );

  return RetVal;
}

STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_IfTransmit_UdpTransmitRetValHandling
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) SoAdSrcPduInfoPtr,
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr,
  TcpIp_ReturnType RetVal_TcpIp,
  uint16 RemainingBytes
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  SOAD_ENTER_CRITSEC();
  switch(RetVal_TcpIp)
  {
    case TCPIP_OK:
    {
      CONSTP2CONST(SoAd_PduRouteType, AUTOMATIC, SOAD_APPL_DATA) PduRoutePtr =
        &SOAD_CFG(PduRoute, PduRouteDestVirtualPtr->PduRouteId);
      /* change state to wait for transmit confirmation */
      SoAd_SetPduRouteDestVirtualState(PduRouteDestVirtualPtr, SOAD_PDUROUTEDESTVIRTUALSTATE_WAITCONF);
      if(SoConPtr->TxBufferId != SOAD_INVALID_SOCON_ID)
      {
        P2VAR(SoAd_SoConTxBufferStateType, AUTOMATIC, SOAD_APPL_DATA) TxBufferStatePtr
          = &SOAD_PBRAM(SoConTxBufferState, SoConPtr->TxBufferId);
        SoAd_CleanUpUdpTxBuffer(SoConPtr, SoConStatePtr, TRUE);

        /* Reset NPduUdpTxBufferIdx to be ready for the next transmission. This is necessary in case
        that another frame triggers sending of the buffer but gets stored afterwards
        (nPDU/retry feature. */
        TxBufferStatePtr->NPduUdpTxBufferIdx = 0U;
      }
      SOAD_PRECONDITION_ASSERT(PduRoutePtr->ULFuncId != SOAD_INVALID_ULFUNCID, DET_INTERNAL_API_ID);
      /* !LINKSTO SoAd.dsn.Func0120.Udp.SkipIfTxConfirmation, 1 */
      /* !LINKSTO SoAd.dsn.Func0120.Udp.SkipIfTxConfirmationPerPdu, 1 */
      if(
          (SoAd_IfModuleAPI[PduRoutePtr->ULFuncId].IfTxConfirmation == NULL_PTR)
          || SOAD_FLAG(PduRoutePtr, SOAD_PDUROUTE_FLAG_SKIP_IF_TXCONF)
        )
      {
        /* No Tx confirmation required for this PDU */
        SoAd_SetPduRouteDestVirtualState(PduRouteDestVirtualPtr, SOAD_PDUROUTEDESTVIRTUALSTATE_IDLE);
        if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_AUTO_SETUP))
        {
          SoAd_ResetWildCardSoCon(SoConPtr, SoConStatePtr);
        }
      }
      else
      {
        /* set a flag for UDP socket connection, that the transmit confirmation is
         * called in the context of the next SoAd_MainFunctionUdpTxConf() */
        SOAD_FLAG_SET_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TXCONF_PENDING);
        SoAd_SetBitInBitMap(SoConPtr,&SoAd_PBcfg->SoConTxIFNotIdleBitMapBase,&SoAd_TxIfCondChange);
      }

      /* Ensure that copy process worked. */
      if(RemainingBytes == 0U)
      {
        RetVal = E_OK;
      }

      break;
    }
    case TCPIP_E_PHYS_ADDR_MISS:
    {
      if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_UDP_RETRY))
      {
        P2VAR(SoAd_SoConTxBufferStateType, AUTOMATIC, SOAD_APPL_DATA) TxBufferStatePtr = NULL_PTR;
        SoAd_GetSoConTxBufferState(SoConPtr, TxBufferStatePtr);
        TxBufferStatePtr->TxTimer = 1U; /* Retry in next main function */
        /* Set flag again. SoAd_CleanUpUdpTxBuffer() might have reset it already during buffer
           cleanup process. */
        SOAD_FLAG_SET_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_UDP_RETRY);
        SoAd_SetBitInBitMap(SoConPtr,&SoAd_PBcfg->SoConTxIFNotIdleBitMapBase,&SoAd_TxIfCondChange);

        if(NULL_PTR != SoAdSrcPduInfoPtr)
        {
          P2CONST(SoAd_SoConTxBufferCfgType, AUTOMATIC, SOAD_APPL_DATA) TxBufferCfgPtr = NULL_PTR;
          SoAd_GetSoConTxBufferCfg(SoConPtr, TxBufferCfgPtr);
          /* PDU needs to be stored. Disable shadow buffer by setting ShadowBufferStartIdx
             to BufferCapacity. */
          TxBufferStatePtr->ShadowBufferStartIdx = TxBufferCfgPtr->Capacity;
          TxBufferStatePtr->ShadowBufferLen = 0U;
          TxBufferStatePtr->ShadowBufferTxTimer = 0U;

          RetVal = SoAd_IfTransmit_UdpStoreToBuffer
          (
            SoConPtr,
            SoConStatePtr,
            SoAdSrcPduInfoPtr,
            PduRouteDestVirtualPtr
          );
        }
        else
        {
          /* Buffer transmission was postpone. Switch from state TXING to BUFFERING. */
          SoAd_SetPduRouteDestVirtualState(PduRouteDestVirtualPtr, SOAD_PDUROUTEDESTVIRTUALSTATE_BUFFERED);
          RetVal = E_OK;
        }
      }
      else
      {
        /* Note: the TcpIp module takes care that nothing is transmitted if an error occurred,
         *       nothing to do for the SoAd. No UL TxConfirmation in case of error for If API. */
        SoAd_SetPduRouteDestVirtualState(PduRouteDestVirtualPtr, SOAD_PDUROUTEDESTVIRTUALSTATE_IDLE);
      }
      break;
    }
    default:
    {
      /* Note: the TcpIp module takes care that nothing is transmitted if an error occurred,
       *       nothing to do for the SoAd. No UL TxConfirmation in case of error for If API. */
      SoAd_SetPduRouteDestVirtualState(PduRouteDestVirtualPtr, SOAD_PDUROUTEDESTVIRTUALSTATE_IDLE);
      break;
    }
  }

  if((NULL_PTR == SoAdSrcPduInfoPtr) && (E_NOT_OK == RetVal))
  {
    /* Buffer is used for transmission but transmission failed and no retry planned. */
    SoAd_CleanUpUdpTxBuffer(SoConPtr, SoConStatePtr, FALSE);
  }
  SOAD_EXIT_CRITSEC();

  return RetVal;
}

STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_PrepareUdpIfTransmit
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(SoAd_SoConTxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConTxDataPtr,
  CONSTP2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) SoAdSrcPduInfoPtr,
  CONSTP2VAR(SoAd_TxModeType, AUTOMATIC, SOAD_APPL_DATA) TxModePtr,
  CONSTP2VAR(boolean, AUTOMATIC, SOAD_APPL_DATA) SendBufferPtr,
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr,
  CONSTP2VAR(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) RemoteAddrPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  const SoAd_PduRouteDestVirtualIdType ActivePduRouteDestVirtualId
    = SoAd_GetActivePduRouteDestVirtualId(SoConPtr);
  CONSTP2CONST(SoAd_SoConGroupStateType, AUTOMATIC, SOAD_APPL_DATA) SoConGroupStatePtr =
       &SOAD_PBRAM(SoConGroupState, SoConPtr->SoConGroupId);

  const uint32 HeaderSize =
          (SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_PDU_HEADER_MODE) ? SOAD_PDU_HEADER_LEN : 0U);
  const uint32 TotalPduLength = (uint32)SoAdSrcPduInfoPtr->SduLength + HeaderSize;

  *TxModePtr = SOAD_TX_MODE_NONE;
  *SendBufferPtr = FALSE;

  /* UDP protocol can only send up to 64k */
  if(SOAD_FLAG_UP(SoConStatePtr,SOAD_SOCONSTATE_TXING_OK) && (TotalPduLength <= 0xFFFFU))
  {
    /* Socket connection is ready for transmission */
    if(
        (SOAD_INVALID_DEST_VIRTUAL_ID == ActivePduRouteDestVirtualId)
        && !(SOAD_FLAG(SoConGroupStatePtr,SOAD_SOCONGROUP_FLAG_UDP_SOCKET_IN_USE))
      )
    {
      RetVal = SoAd_PrepareUdpIfTransmit_TxModeSelection
      (
        SoConPtr, /* SoConPtr */
        SoConStatePtr, /* SoConStatePtr */
        SoConTxDataPtr, /* SoConTxDataPtr */
        SoAdSrcPduInfoPtr, /* SoAdSrcPduInfoPtr */
        TxModePtr, /* TxModePtr */
        SendBufferPtr, /* SendBufferPtr */
        PduRouteDestVirtualPtr,
        RemoteAddrPtr
      );
    }
    else if(SOAD_INVALID_DEST_VIRTUAL_ID != ActivePduRouteDestVirtualId)
    {
      CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA)
        ActivePduRouteDestVirtualPtr = &SOAD_CFG(PduRouteDestVirtual, ActivePduRouteDestVirtualId);

      if(ActivePduRouteDestVirtualPtr->PduRouteId == PduRouteDestVirtualPtr->PduRouteId)
      {
        /* !LINKSTO SoAd.dsn.Func0102.Udp.Tx.If.SamePdu.NotOK, 1 */
        /* Reject request if PDU was interrupted by same PDU */
      }
      else
      {
        if(SOAD_INVALID_SOCON_ID != SoConPtr->TxBufferId)
        {
          if(ActivePduRouteDestVirtualPtr->SoConId == PduRouteDestVirtualPtr->SoConId)
          {
            /* !LINKSTO SoAd.dsn.Func0102.Tx.If.Interrupt.StoreToBuffer.SameSoCon, 1 */
            /* This socket connection is in use. */
            RetVal = SoAd_IfTransmit_UdpStoreToShadowBuffer
            (
              SoConPtr, /* SoConPtr */
              SoAdSrcPduInfoPtr, /* SoAdSrcPduInfoPtr */
              PduRouteDestVirtualPtr /* PduRouteDestVirtualPtr */
            );
          }
          else
          {
            /* !LINKSTO SoAd.dsn.Func0102.Tx.If.Interrupt.StoreToBuffer.OtherSoCon, 1 */
            /* Another socket connection of the same UDP socket is in use. */
            RetVal = SoAd_IfTransmit_UdpStoreToBuffer
            (
              SoConPtr, /* SoConPtr */
              SoConStatePtr,/* SoConStatePtr */
              SoAdSrcPduInfoPtr, /* SoAdSrcPduInfoPtr */
              PduRouteDestVirtualPtr /* PduRouteDestVirtualPtr */
            );
          }
        }
      }
    }
    /* !LINKSTO SoAd.dsn.Func0102.Tx.If.Interrupt.StoreToBuffer.MainFuncContext, 1 */
    /* only possible case is SOAD_FLAG(SoConGroupStatePtr,SOAD_SOCONGROUP_FLAG_UDP_SOCKET_IN_USE) == True */
    else
    {
      /* CHECK: NOPARSE */
      if(SOAD_INVALID_SOCON_ID != SoConPtr->TxBufferId)
      {
        /* CHECK: PARSE */
        if(SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_BUFFER_LOCK))
        {
          /* Tx buffer is transmitted in main function for this socket connection. */
          RetVal = SoAd_IfTransmit_UdpStoreToShadowBuffer
          (
            SoConPtr, /* SoConPtr */
            SoAdSrcPduInfoPtr, /* SoAdSrcPduInfoPtr */
            PduRouteDestVirtualPtr /* PduRouteDestVirtualPtr */
          );
        }
        else
        {
          RetVal = SoAd_IfTransmit_UdpStoreToBuffer
          (
            SoConPtr, /* SoConPtr */
            SoConStatePtr,/* SoConStatePtr */
            SoAdSrcPduInfoPtr, /* SoAdSrcPduInfoPtr */
            PduRouteDestVirtualPtr /* PduRouteDestVirtualPtr */
          );
        }
      }
      /* CHECK: NOPARSE */
      else
      {
        SOAD_UNREACHABLE_CODE_ASSERT(SOAD_IFTRANSMIT_SVCID);
      }
      /* CHECK: PARSE */
    }
  }
  else /* handle all error cases */
  {
    if(TotalPduLength > 0xFFFFU)
    {
      RetVal = E_NOT_OK;
#if(SOAD_DEV_ERROR_DETECT == STD_ON)
      SOAD_DET_REPORT_ERROR(SOAD_IFTRANSMIT_SVCID, SOAD_E_INV_ARG);
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
    }
  }

  return RetVal;
}

STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_PrepareUdpIfTransmit_TxModeSelection
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(SoAd_SoConTxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConTxDataPtr,
  CONSTP2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) SoAdSrcPduInfoPtr,
  CONSTP2VAR(SoAd_TxModeType, AUTOMATIC, SOAD_APPL_DATA) TxModePtr,
  CONSTP2VAR(boolean, AUTOMATIC, SOAD_APPL_DATA) SendBufferPtr,
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr,
  CONSTP2VAR(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) RemoteAddrPtr
)
{
  /* Max length is limited to 0xFFFF */
  uint16 TcpIpTxLen = (uint16)SoAdSrcPduInfoPtr->SduLength
    + (uint8)(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_PDU_HEADER_MODE) ? SOAD_PDU_HEADER_LEN : 0U);
  Std_ReturnType RetVal = E_NOT_OK;

  /* if header mode is deactivated and data are available */
  if(
     (!SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_PDU_HEADER_MODE))
      && (!(SoAdSrcPduInfoPtr->SduDataPtr == NULL_PTR))
    )
  {
    *TxModePtr = SOAD_TX_MODE_DIRECT;
    RetVal = E_OK;
  }
  else
  {
    if(SOAD_INVALID_SOCON_ID != SoConPtr->TxBufferId)
    {
      CONSTP2CONST(SoAd_PduRouteDestCommonType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestCommonPtr =
          &SOAD_CFG(PduRouteDestCommon, PduRouteDestVirtualPtr->PduRouteDestCommonId);
      P2CONST(SoAd_SoConTxBufferCfgType, AUTOMATIC, SOAD_APPL_DATA) TxBufferCfgPtr = NULL_PTR;
      P2VAR(SoAd_SoConTxBufferStateType, AUTOMATIC, SOAD_APPL_DATA) TxBufferStatePtr = NULL_PTR;
      SoAd_GetSoConTxBufferCfg(SoConPtr, TxBufferCfgPtr);
      SoAd_GetSoConTxBufferState(SoConPtr, TxBufferStatePtr);
      /* Since buffer is involved it is possible to collect the PDUs for later transmission. */

      /* Reset transmitted buffer position. */
      TxBufferStatePtr->NPduUdpTxBufferIdx = 0U;

      /* Shadow buffer start index shall beginn after TxLen of stored PDUs. This will
         reduce the shadow buffer size to  buffer capacity - TxLen. */
      TxBufferStatePtr->ShadowBufferStartIdx = TxBufferStatePtr->TxLen;

      if(!SOAD_FLAG(PduRouteDestCommonPtr, SOAD_PDUROUTEDEST_FLAG_TRIGGER_ALWAYS))
      {
        if((TxBufferStatePtr->TxLen + TcpIpTxLen) <= TxBufferCfgPtr->Capacity)
        {
          /* Store in buffer */
          RetVal = SoAd_IfTransmit_UdpStoreToBuffer
          (
            SoConPtr, /* SoConPtr */
            SoConStatePtr,/* SoConStatePtr */
            SoAdSrcPduInfoPtr, /* SoAdSrcPduInfoPtr */
            PduRouteDestVirtualPtr /* PduRouteDestVirtualPtr */
          );
        }
        else
        {
          if(TxBufferStatePtr->TxLen > 0U)
          {
            /* The nPduUdpTxBuffer must be transmitted before the new PDU is stored/transmitted */
            *SendBufferPtr = TRUE;
          }
          if(TcpIpTxLen > TxBufferCfgPtr->Capacity)
          {
            /* !LINKSTO SoAd.dsn.Func0102.Udp.Tx.If.OversizedPdu, 1 */
            /* !LINKSTO SoAd.dsn.Func0102.Tx.If.Trigger_Never.OversizedPDU, 1 */
            /* Try to send PDU even if it does not fit into the buffer. */
            *TxModePtr = SOAD_TX_MODE_INDIRECT;
          }
          else
          {
            *TxModePtr = SOAD_TX_MODE_BUFFERING;
            /* Buffering is done after transmission of buffer. Shadow buffer may need to be reduced
               if new PDU, which shall be stored, is bigger than currently stored PDUs. */
            TxBufferStatePtr->ShadowBufferStartIdx = SOAD_MAX(TcpIpTxLen, TxBufferStatePtr->TxLen);
          }
        }
      }
      else
      {
        if((TxBufferStatePtr->TxLen + TcpIpTxLen) > TxBufferCfgPtr->Capacity)
        {
          if(TxBufferStatePtr->TxLen > 0U)
          {
            /* the nPduUdpTxBuffer must be transmitted before the new PDU is stored/transmitted */
            *SendBufferPtr = TRUE;
          }
        }
        /* !LINKSTO SoAd.dsn.Func0102.Tx.If.Trigger_Always.OversizedPDU, 1 */
        /* Send new frame if trigger always PDU does not fit into current buffer */
        /* Try to send PDU even if it does not fit into the buffer. */
        *TxModePtr = SOAD_TX_MODE_INDIRECT;
      }
    }
    else
    {
      /* Header mode is enabled but no buffer available. */
      *TxModePtr = SOAD_TX_MODE_INDIRECT;
    }
  }

  if((SOAD_TX_MODE_NONE != *TxModePtr) || *SendBufferPtr)
  {
    CONSTP2VAR(SoAd_SoConGroupStateType, AUTOMATIC, SOAD_APPL_DATA) SoConGroupStatePtr =
      &SOAD_PBRAM(SoConGroupState, SoConPtr->SoConGroupId);
    CONSTP2CONST(SoAd_PduRouteType, AUTOMATIC, SOAD_APPL_DATA) PduRoutePtr
      = &SOAD_CFG(PduRoute, PduRouteDestVirtualPtr->PduRouteId);

    if(SOAD_FLAG_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_UDP_RETRY))
    {
      *SendBufferPtr = TRUE;
    }
    /* Update members for transmission */
    SoConTxDataPtr->PduPayloadRemainingLen = SoAdSrcPduInfoPtr->SduLength;
    SoConTxDataPtr->PduHeaderWrittenLen = 0U;
    if(SOAD_FLAG(PduRoutePtr , SOAD_PDUROUTE_FLAG_LAST_IS_BEST))
    {
      SoConTxDataPtr->ULIfDataPtr = NULL_PTR;
    }
    else
    {
      SoConTxDataPtr->ULIfDataPtr = SoAdSrcPduInfoPtr->SduDataPtr;
    }
    /* calculate actual PDU length */
    SoConTxDataPtr->TcpIpTxRemainingLen = TcpIpTxLen;
    SoConTxDataPtr->ActivePduRouteDestVirtualId = PduRouteDestVirtualPtr->PduRouteDestVirtualId;

    SoAd_SetPduRouteDestVirtualState(PduRouteDestVirtualPtr, SOAD_PDUROUTEDESTVIRTUALSTATE_TXING);

    /* Preparation was successful. Cleanup is required at the end of this function. */
    SoConStatePtr->BusyCnt++;
    /* Lock SoconGroup if either a PDU must be sent or a Buffer is in use */
    SOAD_FLAG_SET(SoConGroupStatePtr, SOAD_SOCONGROUP_FLAG_UDP_SOCKET_IN_USE);

    /* store the remote address within the exclusive area
     * (only relevant for UDP socket connections) */
    SoAd_AssignRemoteAddressSoCon2SockAddrType(SoConPtr, SoConStatePtr, RemoteAddrPtr);

    RetVal = E_OK;
  }
  return RetVal;
}

STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_IfTransmit_UdpStoreToBuffer
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) SoAdSrcPduInfoPtr,
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  P2CONST(SoAd_SoConTxBufferCfgType, AUTOMATIC, SOAD_APPL_DATA) TxBufferCfgPtr = NULL_PTR;
  P2VAR(SoAd_SoConTxBufferStateType, AUTOMATIC, SOAD_APPL_DATA) TxBufferStatePtr = NULL_PTR;
  const uint8 HeaderLength
    = (uint8)(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_PDU_HEADER_MODE) ? SOAD_PDU_HEADER_LEN : 0U);
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) TxBufferPtr = NULL_PTR;
  const boolean IsUdpRetry
    = SOAD_FLAG_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_UDP_RETRY) ? TRUE : FALSE;

  SoAd_GetSoConTxBufferCfg(SoConPtr,TxBufferCfgPtr);
  TxBufferPtr = &SOAD_PBRAM(SoAdBuffer, TxBufferCfgPtr->BufId);
  SoAd_GetSoConTxBufferState(SoConPtr, TxBufferStatePtr);

  /* Reset transmitted buffer position. */
  TxBufferStatePtr->NPduUdpTxBufferIdx = 0U;

  /* If PDUs in buffer + new PDU does not fit in same buffer. */
  if(
      (TxBufferStatePtr->TxLen + SoAdSrcPduInfoPtr->SduLength + HeaderLength)
      > TxBufferCfgPtr->Capacity
    )
  {
    SoAd_CleanUpUdpTxBuffer(SoConPtr, SoConStatePtr, FALSE);
    /* !LINKSTO SoAd.dsn.Func0102.Tx.If.OversizedPDU.NotOK, 1 */
    if((SoAdSrcPduInfoPtr->SduLength + HeaderLength) <= TxBufferCfgPtr->Capacity)
    {
      /* At least new PDU can be stored in buffer. */
      RetVal = E_OK;
    }
  }
  else
  {
    RetVal = E_OK;
  }
  if(RetVal == E_OK)
  {
    RetVal = SoAd_IfTransmit_UdpStoreToBufferCommon
    (
      TxBufferPtr, /* TxBufferPtr */
      &TxBufferStatePtr->TxLen, /* BufferLenPtr */
      &TxBufferStatePtr->TxTimer, /* TimerPtr */
      SoConPtr,
      SoAdSrcPduInfoPtr,
      PduRouteDestVirtualPtr
    );

    if(RetVal == E_OK)
    {
      SoAd_SetPduRouteDestVirtualState(PduRouteDestVirtualPtr, SOAD_PDUROUTEDESTVIRTUALSTATE_BUFFERED);
      SoAd_SetBitInBitMap(SoConPtr,&SoAd_PBcfg->SoConTxIFNotIdleBitMapBase,&SoAd_TxIfCondChange);
    }

    if(IsUdpRetry)
    {
      TxBufferStatePtr->TxTimer = 1U; /* Retry in next main function */
      /* Set flag again. SoAd_CleanUpUdpTxBuffer() might have reset it already during buffer
         cleanup process. */
      SOAD_FLAG_SET_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_UDP_RETRY);
      SoAd_SetBitInBitMap(SoConPtr,&SoAd_PBcfg->SoConTxIFNotIdleBitMapBase,&SoAd_TxIfCondChange);
    }
  }

  return RetVal;
}


STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_IfTransmit_UdpStoreToShadowBuffer
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) SoAdSrcPduInfoPtr,
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  P2CONST(SoAd_SoConTxBufferCfgType, AUTOMATIC, SOAD_APPL_DATA) TxBufferCfgPtr = NULL_PTR;
  P2VAR(SoAd_SoConTxBufferStateType, AUTOMATIC, SOAD_APPL_DATA) TxBufferStatePtr = NULL_PTR;
  const uint8 HeaderLength
    = (uint8)(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_PDU_HEADER_MODE) ? SOAD_PDU_HEADER_LEN : 0U);

  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) TxBufferPtr = NULL_PTR;

  CONSTP2CONST(SoAd_PduRouteType, AUTOMATIC, SOAD_APPL_DATA) PduRoutePtr =
      &SOAD_CFG(PduRoute, PduRouteDestVirtualPtr->PduRouteId);
  SoAd_BufLengthType BufferSize;

  SoAd_GetSoConTxBufferCfg(SoConPtr, TxBufferCfgPtr);
  TxBufferPtr = &SOAD_PBRAM(SoAdBuffer, TxBufferCfgPtr->BufId);
  SoAd_GetSoConTxBufferState(SoConPtr, TxBufferStatePtr);

  BufferSize = TxBufferCfgPtr->Capacity
    - (TxBufferStatePtr->ShadowBufferStartIdx + TxBufferStatePtr->ShadowBufferLen);

  /* !LINKSTO SoAd.dsn.Func0102.Udp.Tx.If.Interrupt_OversizedPdu, 1 */
  /* !LINKSTO SoAd.dsn.Func0102.Tx.If.OversizedPDU.NotOK, 1 */
  if(BufferSize >= (HeaderLength + (SoAd_BufLengthType)SoAdSrcPduInfoPtr->SduLength))
  {
    if(SOAD_FLAG(PduRoutePtr , SOAD_PDUROUTE_FLAG_LAST_IS_BEST))
    {
      /* Shadow buffering is not supported for last is best. */
    }
    else
    {
      RetVal = SoAd_IfTransmit_UdpStoreToBufferCommon
      (
        &TxBufferPtr[TxBufferStatePtr->ShadowBufferStartIdx], /* TxBufferPtr */
        &TxBufferStatePtr->ShadowBufferLen, /* BufferLenPtr */
        &TxBufferStatePtr->ShadowBufferTxTimer, /* TimerPtr */
        SoConPtr,
        SoAdSrcPduInfoPtr,
        PduRouteDestVirtualPtr
      );

      if(RetVal == E_OK)
      {
        SoAd_SetPduRouteDestVirtualState(PduRouteDestVirtualPtr, SOAD_PDUROUTEDESTVIRTUALSTATE_SHADOW_BUFFERED);
      }
    }
  }

  return RetVal;
}

STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_IfTransmit_UdpStoreToBufferCommon
(
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) TxBufferPtr,
  CONSTP2VAR(SoAd_BufLengthType, AUTOMATIC, SOAD_APPL_DATA) BufferLenPtr,
  CONSTP2VAR(SoAd_TimerType, AUTOMATIC, SOAD_APPL_DATA) TimerPtr,
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) SoAdSrcPduInfoPtr,
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  CONSTP2CONST(SoAd_PduRouteDestCommonType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestCommonPtr =
      &SOAD_CFG(PduRouteDestCommon, PduRouteDestVirtualPtr->PduRouteDestCommonId);
  CONSTP2CONST(SoAd_PduRouteType, AUTOMATIC, SOAD_APPL_DATA) PduRoutePtr =
      &SOAD_CFG(PduRoute, PduRouteDestVirtualPtr->PduRouteId);
  const uint8 HeaderLength
    = (uint8)(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_PDU_HEADER_MODE) ? SOAD_PDU_HEADER_LEN : 0U);

  /*if buffered add also data */
  if(SOAD_FLAG(PduRoutePtr , SOAD_PDUROUTE_FLAG_LAST_IS_BEST))
  {
    /* last is best */
    SoAd_WritePduHeaderLastIsBest
    (
      PduRouteDestVirtualPtr, /* PduRouteDestVirtualPtr*/
      &TxBufferPtr[0U], /* BufPtr */
      BufferLenPtr, /* BufferLenPtr */
      SoAdSrcPduInfoPtr->SduLength /* PduHeaderLengthField */
    );
    RetVal = E_OK;
  }
  else
  {
    /* !LINKSTO SoAd.dsn.Func0102.Udp.Tx.If.Interrupt_NullPtr.NotOK, 1 */
    if(SoAdSrcPduInfoPtr->SduDataPtr == NULL_PTR)
    {
      /* Data Pointer is required if stored to buffer and last is best functionality is
      not enabled. */
      RetVal = E_NOT_OK;
    }
    else
    {
      if(HeaderLength == SOAD_PDU_HEADER_LEN)
      {
        /* Write PDU header into the TxBuffer. */
        COMSTACK_SET32(&TxBufferPtr[*BufferLenPtr], 0U, PduRouteDestCommonPtr->PduHeaderId);
        /* Value for PduHeaderLengthField */
        COMSTACK_SET32(&TxBufferPtr[*BufferLenPtr], 4U, SoAdSrcPduInfoPtr->SduLength);
      }

      TS_MemCpy
      (
        &TxBufferPtr[*BufferLenPtr + HeaderLength],
        SoAdSrcPduInfoPtr->SduDataPtr,
        SoAdSrcPduInfoPtr->SduLength
      );

      /* Is protected by sum < TxBufferCfgPtr->Capacity, TxBufferCfgPtr->Capacity is uint16 */
      *BufferLenPtr += (HeaderLength + (SoAd_BufLengthType)SoAdSrcPduInfoPtr->SduLength);
      RetVal = E_OK;
    }
  }

  if(RetVal == E_OK)
  {
    SoAd_TimerType NewTxTimeout;
    CONSTP2CONST(SoAd_SoConGroupType, AUTOMATIC, SOAD_APPL_DATA) SoConGroupPtr =
      &SOAD_CFG(SoConGroup, SoConPtr->SoConGroupId);

    /* Setup the timer */
    if(PduRouteDestCommonPtr->NPduUdpTxPduTimeout != SOAD_INVALID_TIMERVAL)
    {
      NewTxTimeout = PduRouteDestCommonPtr->NPduUdpTxPduTimeout;
    }
    else if(SoConGroupPtr->NPduUdpTxRefTimeout != SOAD_INVALID_TIMERVAL)
    {
      NewTxTimeout = SoConGroupPtr->NPduUdpTxRefTimeout;
    }
    else
    {
      NewTxTimeout = *TimerPtr;
    }

    /* Set to new value if new timer is smaller than current timer or if current timer is
       disabled. */
    if((NewTxTimeout < *TimerPtr) || (*TimerPtr == 0U))
    {
      *TimerPtr = NewTxTimeout;
    }
    if(SOAD_FLAG(PduRouteDestCommonPtr, SOAD_PDUROUTEDEST_FLAG_TRIGGER_ALWAYS))
    {
      /* Send TRIGGER always PDUs in next main function latest. */
      *TimerPtr = 1U;
    }
  }
  return RetVal;
}


STATIC FUNC(void, SOAD_CODE) SoAd_RestoreUdpIfShadowBuffer
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr
)
{
  P2CONST(SoAd_SoConTxBufferCfgType, AUTOMATIC, SOAD_APPL_DATA) TxBufferCfgPtr = NULL_PTR;
  P2VAR(SoAd_SoConTxBufferStateType, AUTOMATIC, SOAD_APPL_DATA) TxBufferStatePtr = NULL_PTR;
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) TxBufferPtr = NULL_PTR;

  SoAd_GetSoConTxBufferCfg(SoConPtr, TxBufferCfgPtr);
  TxBufferPtr = &SOAD_PBRAM(SoAdBuffer, TxBufferCfgPtr->BufId);
  SoAd_GetSoConTxBufferState(SoConPtr, TxBufferStatePtr);

  if(0U != TxBufferStatePtr->ShadowBufferLen)
  {
    const SoAd_PduTxDataIdType MaxPduTxDataId = SoConPtr->PduTxDataFirstId + SoConPtr->PduTxDataCnt;
    P2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr;
    P2VAR(SoAd_PduRouteDestVirtualDataType, AUTOMATIC, SOAD_APPL_DATA) PduTxDataPtr;
    /* Initialize with previous. Underflow is covered by ComStack_FindNextOne() */
    uint32 StartBit = SoConPtr->PduTxDataFirstId - (uint32)1U;
    uint32 NextBit;
    CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) PduTxDataNotIdleBitMap =
                  &SOAD_PBRAM_REF_ID(uint32, SoAd_PBcfg->PduTxDataNotIdleBitMapBase, 0U);

    SOAD_PRECONDITION_ASSERT
    (
      (TxBufferStatePtr->TxLen + TxBufferStatePtr->ShadowBufferLen) <= TxBufferCfgPtr->Capacity,
      SOAD_IFTRANSMIT_SVCID
    );

    TS_MemCpy(
               &TxBufferPtr[TxBufferStatePtr->TxLen],
               &TxBufferPtr[TxBufferStatePtr->ShadowBufferStartIdx],
               TxBufferStatePtr->ShadowBufferLen
             );
    TxBufferStatePtr->TxLen += TxBufferStatePtr->ShadowBufferLen;
    /* Reset transmitted buffer position. */
    TxBufferStatePtr->NPduUdpTxBufferIdx = 0U;
    TxBufferStatePtr->ShadowBufferLen = 0U;
    TxBufferStatePtr->TxTimer = TxBufferStatePtr->ShadowBufferTxTimer;
    TxBufferStatePtr->ShadowBufferTxTimer = 0U;
    /* Shadow buffer start begins after buffered PDUs. */
    TxBufferStatePtr->ShadowBufferStartIdx = TxBufferStatePtr->TxLen;

    /* loop over all PduRoutes of this socket connections, to find PDUs which are stored in
       shadow buffer */
    /* !LINKSTO SoAd.dsn.IndicationBitMaps, 1 */
    /* Deviation MISRAC2012-3 */
    for(SOAD_COMSTACK_BF_ITERATE(NextBit, &PduTxDataNotIdleBitMap[0U], StartBit, MaxPduTxDataId))
    {
      SoAd_GetPduRouteDestVirtualFromPduTxDataId(NextBit, &PduRouteDestVirtualPtr);
      PduTxDataPtr = &SOAD_PBRAM(PduRouteDestVirtualData, PduRouteDestVirtualPtr->PduRouteDestVirtualId);
      if(PduTxDataPtr->PduRouteDestVirtualState == SOAD_PDUROUTEDESTVIRTUALSTATE_SHADOW_BUFFERED)
      {
        /* Reset state to buffered after interrupted transmission was performed. */
        SoAd_SetPduRouteDestVirtualState(PduRouteDestVirtualPtr, SOAD_PDUROUTEDESTVIRTUALSTATE_BUFFERED);
      }
    }

    SoAd_SetBitInBitMap(SoConPtr,&SoAd_PBcfg->SoConTxIFNotIdleBitMapBase,&SoAd_TxIfCondChange);
  }
}

STATIC FUNC(void, SOAD_CODE) SoAd_IfTransmitSkipConf
(
  PduIdType SoAdSrcPduId
)
{

  const SoAd_RouteIdType PduRouteId = SOAD_TX_PDUID_TO_PDUROUTE_ID(SoAdSrcPduId);
  CONSTP2CONST(SoAd_PduRouteType, AUTOMATIC, SOAD_APPL_DATA) PduRoutePtr =
      &SOAD_CFG(PduRoute, PduRouteId);
  SoAd_PduRouteDestVirtualIdType PduRouteDest_loop; /* SoConId iterator for Tx Pdu fan-out */
  const SoAd_PduRouteDestVirtualIdType PduRouteDestCnt = PduRoutePtr->PduRouteDestVirtualCnt;

  SOAD_ENTER_CRITSEC();
  for(
       PduRouteDest_loop = 0U;
       PduRouteDest_loop < PduRouteDestCnt;
       PduRouteDest_loop++
     )
  {
    CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr =
       &SOAD_CFG(PduRouteDestVirtual, PduRoutePtr->PduRouteDestVirtualFirstId + PduRouteDest_loop);

    CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr =
      &SOAD_CFG(SoCon, PduRouteDestVirtualPtr->SoConId);

    CONSTP2VAR(SoAd_PduRouteDestVirtualDataType, AUTOMATIC, SOAD_APPL_DATA) PduTxDataPtr =
      &SOAD_PBRAM(PduRouteDestVirtualData, PduRouteDestVirtualPtr->PduRouteDestVirtualId);
    SoAd_RouteIdType PduTxState;
    TS_AtomicAssign8(PduTxState, PduTxDataPtr->PduRouteDestVirtualState);

    /* this transmission is waiting for acknowledgment */
    if(PduTxState == SOAD_PDUROUTEDESTVIRTUALSTATE_WAITCONF)
    {
      CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr =
        &SOAD_PBRAM(SoConState, PduRouteDestVirtualPtr->SoConId);
      /* the state can be reset to IDLE */
      SoAd_SetPduRouteDestVirtualState(PduRouteDestVirtualPtr, SOAD_PDUROUTEDESTVIRTUALSTATE_IDLE);

      /* and the remote address setting shall be reset too */
      /* For TCP connections the socket connection will be restarted before
         remote address can be reset.
         In context of next main function the TCP connection gets closed.
         All data which are already sent will be confirmed and confirmation
         will be discarded because destination is IDLE.
         TcpIp will close the TCP connection afterwards which will trigger the
         reset of the remote address. No need to wait for TxConfirmation before
         closing the TCP connection.
      */

      if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_TCP))
      {
        P2VAR(SoAd_PduTxTcpDataType, AUTOMATIC, SOAD_APPL_DATA) PduTxTcpDataPtr = NULL_PTR;
        /* Because of conditions in generator this PduRouteDestVirtualPtr must always have a
           PduTxTcpDataId defined here. */
        SoAd_GetPduTxTcpData(PduRouteDestVirtualPtr,PduTxTcpDataPtr);
        PduTxTcpDataPtr->PduTxConfCtr = 0U;
      }
      if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_AUTO_SETUP))
      {
        SoAd_ResetWildCardSoCon(SoConPtr, SoConStatePtr);
      }
    }
  }
  SOAD_EXIT_CRITSEC();
}

STATIC FUNC(void, SOAD_CODE) SoAd_TxConfirmation_ProvideConf
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  uint16 Length
)
{
  SoAd_PduRouteDestVirtualIdType PduRouteDestVirtualId = SOAD_INVALID_DEST_VIRTUAL_ID;
  P2VAR(SoAd_SoConTxTcpDataType, AUTOMATIC, SOAD_APPL_DATA) SoConTxTcpDataPtr = NULL_PTR;
  P2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr = NULL_PTR;
  P2VAR(SoAd_PduRouteDestVirtualDataType, AUTOMATIC, SOAD_APPL_DATA) PduTxDataPtr;
  boolean ContinueForIf = FALSE;
  if(SoConPtr -> SoConTxTcpDataId != SOAD_INVALID_SOCON_ID)
  {
    SoAd_GetSoConTxTcpData(SoConPtr, SoConTxTcpDataPtr);

    if(SoConTxTcpDataPtr->TxConfCtr >= Length)
    {
      SoConTxTcpDataPtr->TxConfCtr -= Length;
    }
    else
    {
      /* It is only possible to detect that in total more data are confirmed than sent. It is not
         possible to detect which confirmation was wrong and therefore it shall be processed
         if it is valid. A Det shall indicate the length problem. */
      SoConTxTcpDataPtr->TxConfCtr = 0U;
  #if(SOAD_DEV_ERROR_DETECT == STD_ON)
      SOAD_DET_REPORT_ERROR(SOAD_TXCONFIRMATION_SVCID, SOAD_E_INV_ARG);
  #endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
    }
  }
  {
    boolean FinishTp = FALSE;
    SOAD_ENTER_CRITSEC();

    /* If a TP transmission is in progress. */

    if(SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_TP_PENDING))
    {
      const boolean TpSkipTxConf = SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_TCP_IMMEDIATE_TP_TXCONF);
      PduRouteDestVirtualId = SoAd_GetActivePduRouteDestVirtualId(SoConPtr);
      if(
          (SOAD_INVALID_DEST_VIRTUAL_ID != PduRouteDestVirtualId)
          && (!TpSkipTxConf)
          && (SoConTxTcpDataPtr != NULL_PTR)
        )
      {

        PduRouteDestVirtualPtr = &SOAD_CFG(PduRouteDestVirtual, PduRouteDestVirtualId);

        /* PduTxData have same order as PduRouteDestVirtual. */
        PduTxDataPtr = &SOAD_PBRAM(PduRouteDestVirtualData, PduRouteDestVirtualPtr->PduRouteDestVirtualId);

        if((SOAD_PDUROUTEDESTVIRTUALSTATE_WAITCONF == PduTxDataPtr->PduRouteDestVirtualState) && (0U == SoConTxTcpDataPtr->TxConfCtr))
        {
          /* PduTxTcpDataId is always valid here Generator checks for TCP and TCP_IMMEDIATE_TP_TXCONF not set */
          CONSTP2VAR(SoAd_PduTxTcpDataType, AUTOMATIC, SOAD_APPL_DATA) PduTxTcpDataPtr
              = &SOAD_PBRAM(PduTxTcpData, PduRouteDestVirtualPtr->PduTxTcpDataId);
          FinishTp = TRUE;
          SoAd_SetPduRouteDestVirtualState(PduRouteDestVirtualPtr, SOAD_PDUROUTEDESTVIRTUALSTATE_CONF_OK);
          /* For TP SoConTxData->TxConfCtr = PduTxDataPtr->PduTxConfCtr */
          PduTxTcpDataPtr->PduTxConfCtr = 0U;
        }
      }
      else
      {
        /* Tp transmission is not waiting for a TxConfirmation or the complete PDU
           was not confirmed yet. Stop processing */
      }
    }
    else
    {
      /* No Tp transmission is occupying this SoCon. Check for If. */
      ContinueForIf = TRUE;
    }

    SOAD_EXIT_CRITSEC();

    if(FinishTp)
    {
      SoAd_Int_05_TpFinishTransmission
      (
        SoConPtr, SoConStatePtr, PduRouteDestVirtualPtr
      );
    }
  }

  if(ContinueForIf)
  {
    const SoAd_PduTxDataIdType MaxPduTxDataId = SoConPtr->PduTxDataFirstId + SoConPtr->PduTxDataCnt;
    /* Initialize with previous. Underflow is covered by ComStack_FindNextOne() */
    uint32 StartBit = SoConPtr->PduTxDataFirstId - (uint32)1U;
    uint32 NextBit;
    CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) PduTxDataNotIdleBitMap =
                  &SOAD_PBRAM_REF_ID(uint32, SoAd_PBcfg->PduTxDataNotIdleBitMapBase, 0U);

    /* !LINKSTO SoAd.dsn.IndicationBitMaps, 1 */
    /* loop over all PduRoutes of this socket connections, to find the recently transmitted PDUs */
    /* Deviation MISRAC2012-3 */
    for(SOAD_COMSTACK_BF_ITERATE(NextBit, &PduTxDataNotIdleBitMap[0U], StartBit, MaxPduTxDataId))
    {
      P2VAR(SoAd_PduTxTcpDataType, AUTOMATIC, SOAD_APPL_DATA) PduTxTcpDataPtr = NULL_PTR;
      SoAd_GetPduRouteDestVirtualFromPduTxDataId
              (
                NextBit,
                &PduRouteDestVirtualPtr
              );
      /* PduTxTcpDataId is always valid here Generator checks for TCP and IF */
      PduTxTcpDataPtr = &SOAD_PBRAM(PduTxTcpData, PduRouteDestVirtualPtr->PduTxTcpDataId);

      /* PduTxData have same order as PduRouteDestVirtual. */
      PduTxDataPtr = &SOAD_PBRAM(PduRouteDestVirtualData, PduRouteDestVirtualPtr->PduRouteDestVirtualId);
      if(PduTxTcpDataPtr->PduTxConfCtr <= Length)
      {
        /* Transmitted PDU confirmed. */
        PduTxTcpDataPtr->PduTxConfCtr = 0U;
      }
      else
      {
        /* Only part of PDU confirmed. */
        PduTxTcpDataPtr->PduTxConfCtr -= Length;
      }
      if(PduTxTcpDataPtr->PduTxConfCtr == 0U)
      {
        switch(PduTxDataPtr->PduRouteDestVirtualState)
        {
          case SOAD_PDUROUTEDESTVIRTUALSTATE_TXING:
          {
            /* PDU route already in use. UL did not wait for TxConfirmation or an unexpected Tx
               confirmation occurred. Discard it. */
            break;
          }
          case SOAD_PDUROUTEDESTVIRTUALSTATE_WAITCONF:
          {
            /* PDU confirmed in full */
            SoAd_SetPduRouteDestVirtualState(PduRouteDestVirtualPtr, SOAD_PDUROUTEDESTVIRTUALSTATE_IDLE);

            /* Pdu fan-out: if no more destination is waiting for a TCP acknowledgment or
            UDP retransmission, the upper layer shall be informed */
            if(SoAd_PduFanOutPduTxConfPending(PduRouteDestVirtualPtr->PduRouteId) == FALSE)
            {
              SoAd_ULIfTxConfirmation(PduRouteDestVirtualPtr->PduRouteId);
            }
            break;
          }
          /* CHECK: NOPARSE */
          default:
          {
            /* No other state shall be possible. */
            SOAD_UNREACHABLE_CODE_ASSERT(SOAD_TCPIPEVENT_SVCID);
            break;
          }
          /* CHECK: PARSE */
        }
      }
    }
  }
}


STATIC FUNC(void, SOAD_CODE) SoAd_ULIfTxConfirmation
(
  SoAd_RouteIdType PduRouteId
)
{
  CONSTP2CONST(SoAd_PduRouteType, AUTOMATIC, SOAD_APPL_DATA) PduRoutePtr =
      &SOAD_CFG(PduRoute, PduRouteId);

  DBG_SOAD_ULIFTXCONFIRMATION_ENTRY(PduRouteId);

  /* reset all socket connections of this PduRoute if they use a wild card and automatic setup
   * before the <UL>TxConfirmation to prevent further transmissions until the
   * connection was reopened */
  SoAd_ResetWildCardSoConInPduRoute(PduRouteId);

  SOAD_PRECONDITION_ASSERT(PduRoutePtr->ULFuncId != SOAD_INVALID_ULFUNCID, DET_INTERNAL_API_ID);

  if
  (
    (SoAd_IfModuleAPI[PduRoutePtr->ULFuncId].IfTxConfirmation != NULL_PTR)
    && (SOAD_FLAG(PduRoutePtr, SOAD_PDUROUTE_FLAG_SKIP_IF_TXCONF) == FALSE)
  )
  {
    SoAd_IfModuleAPI[PduRoutePtr->ULFuncId].IfTxConfirmation(PduRoutePtr->TxConfPduId);
  }

  DBG_SOAD_ULIFTXCONFIRMATION_EXIT(PduRouteId);
}


/* !LINKSTO SoAd.Dsn.Func0101.Tx.Udp.If.NoHeader,1 */
STATIC FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_CopyTxData_BufferedData
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr,
  uint16 BufLength
)
{
  SoAd_BufLengthType RemainingBufLength = BufLength;
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) PayloadDestPtr = BufPtr;
  P2VAR(SoAd_SoConTxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConTxDataPtr = NULL_PTR;
  BufReq_ReturnType RetVal_BufReq = BUFREQ_E_NOT_OK;
  Std_ReturnType RetVal = E_NOT_OK;
  SoAd_BufLengthType CopyNow;

  SoAd_GetSoConTxData(SoConPtr, SoConTxDataPtr);

  if(SoConPtr->TxBufferId != SOAD_INVALID_SOCON_ID)
  {
    P2VAR(SoAd_SoConTxBufferStateType, AUTOMATIC, SOAD_APPL_DATA) TxBufferStatePtr = NULL_PTR;

    SoAd_GetSoConTxBufferState(SoConPtr, TxBufferStatePtr);
    if(TxBufferStatePtr->TxLen > 0U)
    {
      RetVal = SoAd_CopyTxData_FromInternalBuffer(SoConPtr, &PayloadDestPtr, &RemainingBufLength);
    }
    else
    {
      RetVal = E_OK;
    }
  }
  else
  {
    RetVal = E_OK;
  }

  if((RetVal == E_OK) && (PduRouteDestVirtualPtr != NULL_PTR))
  {
    /* Write PDU header (if applicable) for UDP/TCP */
    if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_PDU_HEADER_MODE))
    {
      CopyNow = SoAd_CopyTxData_CopyPduHeader
      (
        PduRouteDestVirtualPtr,
        SoConTxDataPtr,
        PayloadDestPtr,
        &RemainingBufLength
      );
      /* Set buffer to next free space. */
      PayloadDestPtr = &PayloadDestPtr[CopyNow];
    }

    /* This cast is protected by SoAd_CopyTxData(), can't copy more than uint16
     * (parameter BufLength is uint16) */
    /* Copy (part of) UL IfTransmit data for UDP/TCP */
    CopyNow = (SoAd_BufLengthType)SOAD_MIN(SoConTxDataPtr->TcpIpTxRemainingLen, RemainingBufLength);
    RetVal = E_NOT_OK;

    if(CopyNow > 0U)
    {
      /* !LINKSTO SoAd.ASR43.SWS_SoAd_00731,1 */
      /* !LINKSTO SoAd.Dsn.Func0101.Tx.Udp.If.Header.Fanout,1 */
      if(SoConTxDataPtr->ULIfDataPtr != NULL_PTR)
      {
        TS_MemCpy(PayloadDestPtr, SoConTxDataPtr->ULIfDataPtr, CopyNow);
        /* !LINKSTO SoAd.ASR42.SWS_SoAd_00543_UDP_IF_Tx_IPFrag, 1 */
        /* Set pointer to remaining data in case only a part was copied. */
        SoConTxDataPtr->ULIfDataPtr = &SoConTxDataPtr->ULIfDataPtr[CopyNow];
        RetVal = E_OK;
      }
      else
      {
        if(CopyNow == SoConTxDataPtr->TcpIpTxRemainingLen)
        {
          PduInfoType PduInfo;
          PduInfo.SduLength = CopyNow;
          PduInfo.SduDataPtr = PayloadDestPtr;
          RetVal = SoAd_ULIfTriggerTransmit(PduRouteDestVirtualPtr->PduRouteId, &PduInfo);
        }
#if(SOAD_DEV_ERROR_DETECT == STD_ON)
        /* !LINKSTO SoAd.Dsn.Func0101.Tx.Udp.If.Det.IPFrag,1 */
        else
        {
          SOAD_DET_REPORT_ERROR(SOAD_COPYTXDATA_SVCID, SOAD_E_TRIGGERTXBUF);
        }
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
      }
      /* Remove copied data from TcpIp data to transmit count (payload + header). */
      SoConTxDataPtr->TcpIpTxRemainingLen -= CopyNow;
      RemainingBufLength -= CopyNow;
    }
    else
    {
      RetVal = E_OK;
    }
  }

  /* BUFREQ_OK will only be returned if the TCP buffer was completely filled */
  if((RemainingBufLength == 0U) && (RetVal == E_OK))
  {
    RetVal_BufReq = BUFREQ_OK;
  }
  return RetVal_BufReq;
}

STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_ULIfTriggerTransmit
(
  SoAd_RouteIdType PduRouteId,
  CONSTP2VAR(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) PduInfoPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  CONSTP2CONST(SoAd_PduRouteType, AUTOMATIC, SOAD_APPL_DATA) PduRoutePtr =
      &SOAD_CFG(PduRoute, PduRouteId);

  DBG_SOAD_ULIFTRIGGERTRANSMIT_ENTRY(PduRouteId, PduInfoPtr);

  SOAD_PRECONDITION_ASSERT(PduRoutePtr->ULFuncId != SOAD_INVALID_ULFUNCID, DET_INTERNAL_API_ID);

  /* !LINKSTO SoAd.ASR43.SWS_SoAd_00732,1 */
  if(SoAd_IfModuleAPI[PduRoutePtr->ULFuncId].IfTriggerTransmit != NULL_PTR)
  {
    RetVal = SoAd_IfModuleAPI[PduRoutePtr->ULFuncId].IfTriggerTransmit
    (
      PduRoutePtr->TxConfPduId,
      PduInfoPtr
    );
  }

  DBG_SOAD_ULIFTRIGGERTRANSMIT_EXIT(RetVal, PduRouteId, PduInfoPtr);
  return RetVal;
}

STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_CopyTxData_FromInternalBuffer
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_Uint8PtrType, AUTOMATIC, SOAD_APPL_DATA) BufPtrPtr,
  CONSTP2VAR(SoAd_BufLengthType, AUTOMATIC, SOAD_APPL_DATA) RemainingBufLengthPtr
)
{
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) PayloadDestPtr = *BufPtrPtr;
  Std_ReturnType RetVal = E_NOT_OK;

  P2VAR(SoAd_SoConTxBufferStateType, AUTOMATIC, SOAD_APPL_DATA) TxBufferStatePtr = NULL_PTR;
  P2CONST(SoAd_PduRouteType, AUTOMATIC, SOAD_APPL_DATA) PduRoutePtr = NULL_PTR;
  P2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr;
  P2CONST(SoAd_SoConTxBufferCfgType, AUTOMATIC, SOAD_APPL_DATA) TxBufferCfgPtr = NULL_PTR;

  SoAd_GetSoConTxBufferState(SoConPtr, TxBufferStatePtr);
  /* Process buffered nPDU UDP data. Is skipped for TCP or UDP TP */

  SoAd_GetPduRouteDestVirtualFromPduTxDataId(SoConPtr->PduTxDataFirstId, &PduRouteDestVirtualPtr);
  PduRoutePtr = &SOAD_CFG(PduRoute, PduRouteDestVirtualPtr->PduRouteId);
  SoAd_GetSoConTxBufferCfg(SoConPtr, TxBufferCfgPtr);

  if(!SOAD_FLAG(PduRoutePtr , SOAD_PDUROUTE_FLAG_LAST_IS_BEST))
  {
    const SoAd_BufLengthType CopyNow
      = SOAD_MIN(TxBufferStatePtr->TxLen, *RemainingBufLengthPtr);
    CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) TxBufPtr
      = &SOAD_PBRAM(SoAdBuffer, TxBufferCfgPtr->BufId);
    TS_MemCpy(PayloadDestPtr, &TxBufPtr[TxBufferStatePtr->NPduUdpTxBufferIdx], CopyNow);
    /* !LINKSTO SoAd.ASR42.SWS_SoAd_00543_UDP_IF_Tx_nPDU_IPFrag, 1 */
    TxBufferStatePtr->NPduUdpTxBufferIdx += CopyNow;
    PayloadDestPtr = &PayloadDestPtr[CopyNow];
    /* Udp frame cannot be bigger than 64k, cast of TxLen is protected by check in
     * SoAd_PrepareUdpIfTransmit
     */
    *RemainingBufLengthPtr -= (uint16)CopyNow;
    TxBufferStatePtr->TxLen -= CopyNow;
    RetVal = E_OK;
  }
  else
  {
    RetVal = SoAd_CopyTxData_FromInternalBuffer_LastIsBest
    (
      &PayloadDestPtr,
      TxBufferStatePtr,
      TxBufferCfgPtr,
      RemainingBufLengthPtr
    );
  }

  *BufPtrPtr = PayloadDestPtr;
  return RetVal;
}

STATIC FUNC(Std_ReturnType, SOAD_CODE) SoAd_CopyTxData_FromInternalBuffer_LastIsBest
(
  CONSTP2VAR(SoAd_Uint8PtrType, AUTOMATIC, SOAD_APPL_DATA) BufPtrPtr,
  CONSTP2VAR(SoAd_SoConTxBufferStateType, AUTOMATIC, SOAD_APPL_DATA) TxBufferStatePtr,
  CONSTP2CONST(SoAd_SoConTxBufferCfgType, AUTOMATIC, SOAD_APPL_DATA) TxBufferCfgPtr,
  CONSTP2VAR(SoAd_BufLengthType, AUTOMATIC, SOAD_APPL_DATA) RemainingBufLengthPtr
)
{
  Std_ReturnType RetVal;
  SoAd_BufLengthType HeaderPos = 0U;
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) PayloadDestPtr = *BufPtrPtr;

  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) TxBufPtr
    = &SOAD_PBRAM(SoAdBuffer, TxBufferCfgPtr->BufId);
  SoAd_BufLengthType BufferCapacity;
  BufferCapacity = TxBufferCfgPtr->Capacity;

  /* Set RetVal to E_OK and use it to leave the function in error case. */
  RetVal = E_OK;

  for
  (
    HeaderPos = 0U;
    (HeaderPos < BufferCapacity) &&
    (0U < TxBufferStatePtr->TxLen) &&
    (RetVal != E_NOT_OK);
    HeaderPos += SOAD_PDU_HEADER_LEN
  )
  {
    /* Last is best feature is supported only by UDP protocol which can only send
       up to 64k bytes */
    const uint16 LengthInHeader = (uint16) COMSTACK_GET32(&TxBufPtr[HeaderPos], 4U);
    const uint16 HeaderAndPayloadLength = SOAD_PDU_HEADER_LEN + LengthInHeader;

    if(*RemainingBufLengthPtr >= HeaderAndPayloadLength)
    {
      PduInfoType PduInfo;
      const SoAd_PduRouteDestVirtualIdType PduRouteDestVirtIdLastIsBest
        = (SoAd_PduRouteDestVirtualIdType) COMSTACK_GET32(&TxBufPtr[HeaderPos], 0U);
      CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA)
        TmpPduRouteDestVirtualPtr =
        &SOAD_CFG(PduRouteDestVirtual, PduRouteDestVirtIdLastIsBest);
      CONSTP2CONST(SoAd_PduRouteDestCommonType, AUTOMATIC, SOAD_APPL_DATA)
        PduRouteDestCommonPtr =
        &SOAD_CFG(PduRouteDestCommon, TmpPduRouteDestVirtualPtr->PduRouteDestCommonId);


      /* Write PDU Header ID */
      COMSTACK_SET32(&TxBufPtr[HeaderPos], 0U, PduRouteDestCommonPtr->PduHeaderId);

      TS_MemCpy(PayloadDestPtr, &TxBufPtr[HeaderPos], SOAD_PDU_HEADER_LEN);
      PayloadDestPtr = &PayloadDestPtr[SOAD_PDU_HEADER_LEN];
      /* Length in header for UDP can be maximum 64k */
      PduInfo.SduLength = (PduLengthType) LengthInHeader;
      PduInfo.SduDataPtr = PayloadDestPtr;
      RetVal =
        SoAd_ULIfTriggerTransmit(TmpPduRouteDestVirtualPtr->PduRouteId, &PduInfo);
      PayloadDestPtr = &PayloadDestPtr[LengthInHeader];
      /* Last is best feature is supported only by UDP protocol which can only send
         up to 64k bytes */
      *RemainingBufLengthPtr -= HeaderAndPayloadLength;
      /* This is filled with PDU length. The TxBufferStatePtr->TxLen cannot become negative. */
      SOAD_PRECONDITION_ASSERT
      (
        TxBufferStatePtr->TxLen >= HeaderAndPayloadLength,
        SOAD_IFTRANSMIT_SVCID
      );
      /* Last is best feature is supported only by UDP protocol which can only send
         up to 64k bytes */
      TxBufferStatePtr->TxLen -= HeaderAndPayloadLength;
    }
    else
    {
      RetVal = E_NOT_OK;
      /* IP fragmentation is not supported for last is best. See dev.SoAd.IPFrag.LastIsBest */
#if(SOAD_DEV_ERROR_DETECT == STD_ON)
    /* !LINKSTO SoAd.Dsn.Func0101.Tx.Udp.If.Det.IPFrag,1 */
      SOAD_DET_REPORT_ERROR(SOAD_COPYTXDATA_SVCID, SOAD_E_TRIGGERTXBUF);
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
    }
  }

  *BufPtrPtr = PayloadDestPtr;
  return RetVal;
}



STATIC FUNC(void, SOAD_CODE) SoAd_WritePduHeader
(
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  uint32 PduHeaderId,
  uint32 PduHeaderLengthField,
  uint8 Offset,
  uint8 Length
)
{

  DBG_SOAD_WRITEPDUHEADER_ENTRY(BufPtr, PduHeaderId, PduHeaderLengthField, Offset, Length);

  if(Length == SOAD_PDU_HEADER_LEN)
  {
    /* Write PDU header into the TxBuffer. */
    COMSTACK_SET32(&BufPtr[0U], 0U,  PduHeaderId);
    COMSTACK_SET32(&BufPtr[4U], 0U,  PduHeaderLengthField);
  }
  else
  {
    /* Performance Method: Scalar Replacement */
    uint32 SoAdHeader0 = PduHeaderId;
    uint32 SoAdHeader1 = PduHeaderLengthField;
    const uint8 BitOffset = Offset << 3U; /* convert Offset from byte to bits */
    /* Performance Method: Shift the 64 bit header by the number of Offset bits. */
    SOAD_TS_SHL_UINT64(SoAdHeader0, SoAdHeader1, BitOffset);

    /* Deviation MISRAC2012-7       */
    /* Deviation MISRAC2012-5  <+9> */
    switch(Length) /* write Length bytes to BufPtr[0] (reverse order) to enable fall-through */
    {
      case 7U: BufPtr[6U] = (uint8)(SoAdHeader1 >> 8U); /* Intentional fall-through */
      case 6U: BufPtr[5U] = (uint8)(SoAdHeader1 >> 16U); /* Intentional fall-through */
      case 5U: BufPtr[4U] = (uint8)(SoAdHeader1 >> 24U); /* Intentional fall-through */
      case 4U: BufPtr[3U] = (uint8) SoAdHeader0; /* Intentional fall-through */
      case 3U: BufPtr[2U] = (uint8)(SoAdHeader0 >> 8U); /* Intentional fall-through */
      case 2U: BufPtr[1U] = (uint8)(SoAdHeader0 >> 16U); /* Intentional fall-through */
      case 1U: BufPtr[0U] = (uint8)(SoAdHeader0 >> 24U); /* finished - wrote Length bytes */
      {
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
  DBG_SOAD_WRITEPDUHEADER_EXIT(BufPtr, PduHeaderId, PduHeaderLengthField, Offset, Length);
}

STATIC FUNC(void, SOAD_CODE) SoAd_WritePduHeaderLastIsBest
(
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  CONSTP2VAR(SoAd_BufLengthType, AUTOMATIC, SOAD_APPL_DATA) BufferLenPtr,
  PduLengthType PduHeaderLengthField
)
{
  boolean PduIdFound = FALSE;
  SoAd_BufLengthType HeaderPos = 0U;
  SoAd_BufLengthType NrOfByteBuffer = 0U;
  uint32 PduRouteDestVirtualIdInBuffer = 0U;
  uint32 PduLengthInBuffer = 0U;
  const SoAd_BufLengthType DataBufferLen = *BufferLenPtr;

  /* NrOfByteBuffer grows more than HeaderPos. HeaderPos is smaller than BufferLenPtr */

  for(
       HeaderPos = 0U;
       (HeaderPos < DataBufferLen) && (NrOfByteBuffer < DataBufferLen);
       HeaderPos += SOAD_PDU_HEADER_LEN
     )
  {
    PduRouteDestVirtualIdInBuffer = COMSTACK_GET32(&BufPtr[HeaderPos], 0U);

    if(PduRouteDestVirtualPtr->PduRouteDestVirtualId == PduRouteDestVirtualIdInBuffer)
    {
      PduIdFound = TRUE;
      break;
    }
    else
    {
      PduLengthInBuffer = COMSTACK_GET32(&BufPtr[HeaderPos], 4U);
      /* Cast protected  because Last is best feature is supported only by UDP protocol which can
         only send up to 64k bytes */
      NrOfByteBuffer += (PduLengthType)PduLengthInBuffer + SOAD_PDU_HEADER_LEN;
    }
  }

  if(PduIdFound)
  {
    PduLengthInBuffer = COMSTACK_GET32(&BufPtr[HeaderPos], 4U);
    /* Last is best feature is supported only by UDP protocol which can only
       send up to 64k bytes */
    /* Subtract the last stored PDU length of this header. */
    *BufferLenPtr -= (uint16)(SOAD_PDU_HEADER_LEN + PduLengthInBuffer);
  }
  else
  {
    COMSTACK_SET32(&BufPtr[HeaderPos], 0U, PduRouteDestVirtualPtr->PduRouteDestVirtualId);
  }
  COMSTACK_SET32(&BufPtr[HeaderPos], 4U, (uint32)PduHeaderLengthField);
  /* Add the new PDU length + header for this header. */
  /* Last is best feature is supported only by UDP protocol which can only
       send up to 64k bytes */
  *BufferLenPtr += (uint16)(SOAD_PDU_HEADER_LEN + PduHeaderLengthField);
}



STATIC FUNC(boolean, SOAD_CODE) SoAd_PduFanOutPduTxConfPending
(
  SoAd_RouteIdType PduRouteId
)
{
  CONSTP2CONST(SoAd_PduRouteType, AUTOMATIC, SOAD_APPL_DATA) PduRoutePtr =
    &SOAD_CFG(PduRoute, PduRouteId);
  const SoAd_PduRouteDestVirtualIdType PduRouteDestMax
    = PduRoutePtr->PduRouteDestVirtualFirstId + PduRoutePtr->PduRouteDestVirtualCnt;
  boolean RetVal = FALSE;
  /* Initialize with previous. Underflow is covered by ComStack_FindNextOne() */
  uint32 StartBit = PduRoutePtr->PduRouteDestVirtualFirstId - (uint32)1U;
  uint32 NextBit;
  CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) DestVirtualNotIdleBitMap =
                &SOAD_PBRAM_REF_ID(uint32, SoAd_PBcfg->DestVirtualNotIdleBitMapBase, 0U);

  DBG_SOAD_PDUFANOUTPDUTXCONFPENDING_ENTRY(PduRouteId);

  /* loop over all PduRoutes of this socket connections, to find the recently transmitted PDUs */
  /* !LINKSTO SoAd.dsn.IndicationBitMaps, 1 */
  /* Deviation MISRAC2012-3 */
  for(SOAD_COMSTACK_BF_ITERATE(NextBit, &DestVirtualNotIdleBitMap[0U], StartBit, PduRouteDestMax))
  {
    /* Next bit is in  range */
    RetVal = TRUE;
    break;
  }

  DBG_SOAD_PDUFANOUTPDUTXCONFPENDING_EXIT(RetVal, PduRouteId);
  return RetVal;
}



STATIC FUNC(void, SOAD_CODE) SoAd_CleanUpUdpTxBuffer
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  boolean TransmissionOk
)
{
  const SoAd_PduTxDataIdType MaxPduTxData = SoConPtr->PduTxDataFirstId + SoConPtr->PduTxDataCnt;
  P2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr;
  P2VAR(SoAd_PduRouteDestVirtualDataType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualDataPtr;
  /* Initialize with previous. Underflow is covered by ComStack_FindNextOne() */
  const uint32 StartBit = SoConPtr->PduTxDataFirstId - (uint32)1U;
  uint32 NextBit;
  boolean WaitForTxConfirmation = FALSE;
  boolean ResetRemoteAddr = FALSE;
  CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) PduTxDataNotIdleBitMap =
                &SOAD_PBRAM_REF_ID(uint32, SoAd_PBcfg->PduTxDataNotIdleBitMapBase, 0U);


  DBG_SOAD_CLEANUPUDPTXBUFFER_ENTRY(SoConPtr, SoConStatePtr, TransmissionOk);

  /* loop over all PduRoutes of this socket connections, to find the recently transmitted PDUs */
  /* !LINKSTO SoAd.dsn.IndicationBitMaps, 1 */
  /* Deviation MISRAC2012-3 */
  for(SOAD_COMSTACK_BF_ITERATE(NextBit, &PduTxDataNotIdleBitMap[0U], StartBit, MaxPduTxData))
  {
    SoAd_GetPduRouteDestVirtualFromPduTxDataId(NextBit, &PduRouteDestVirtualPtr);
    PduRouteDestVirtualDataPtr
      = &SOAD_PBRAM(PduRouteDestVirtualData, PduRouteDestVirtualPtr->PduRouteDestVirtualId);
    /* check if this PDU was part of the nPduUdpTx buffer that was sent */
    if
    (
      PduRouteDestVirtualDataPtr->PduRouteDestVirtualState == SOAD_PDUROUTEDESTVIRTUALSTATE_BUFFERED
    )
    {
      if(TransmissionOk)
      {
        CONSTP2CONST(SoAd_PduRouteType, AUTOMATIC, SOAD_APPL_DATA) PduRoutePtr =
          &SOAD_CFG(PduRoute, PduRouteDestVirtualPtr->PduRouteId);
        SOAD_PRECONDITION_ASSERT
        (
          PduRoutePtr->ULFuncId != SOAD_INVALID_ULFUNCID, DET_INTERNAL_API_ID
        );

        /* !LINKSTO SoAd.dsn.Func0120.Udp.SkipIfTxConfirmation, 1 */
        /* !LINKSTO SoAd.dsn.Func0120.Udp.SkipIfTxConfirmationPerPdu, 1 */
        if(
            (SoAd_IfModuleAPI[PduRoutePtr->ULFuncId].IfTxConfirmation == NULL_PTR)
            || SOAD_FLAG(PduRoutePtr, SOAD_PDUROUTE_FLAG_SKIP_IF_TXCONF)
          )
        {
          /* TxConfirmation is not required for this PDU. Skip further processing and set PDU
             immediately to IDLE.*/
          PduRouteDestVirtualDataPtr->PduRouteDestVirtualState = SOAD_PDUROUTEDESTVIRTUALSTATE_IDLE;
          ResetRemoteAddr = TRUE;
        }
        else
        {
          /* transmission was successful -> set the state to WAITCONF, TxConfirmation will be
           * provided in the context of SoAd_MainFunctionUdpTxConf() */
          SoAd_SetPduRouteDestVirtualState
          (
            PduRouteDestVirtualPtr,
            SOAD_PDUROUTEDESTVIRTUALSTATE_WAITCONF
          );
          WaitForTxConfirmation = TRUE;
        }
      }
      else
      {
        /* transmission was not successful -> no TxConfirmation;
         * the PduTxState is reset to IDLE */
        SoAd_SetPduRouteDestVirtualState(PduRouteDestVirtualPtr, SOAD_PDUROUTEDESTVIRTUALSTATE_IDLE);
        ResetRemoteAddr = TRUE;
      }
    }
  }

  {
    P2VAR(SoAd_SoConTxBufferStateType, AUTOMATIC, SOAD_APPL_DATA) TxBufferStatePtr = NULL_PTR;

    SoAd_GetSoConTxBufferState(SoConPtr, TxBufferStatePtr);
    /* reset the length field to flush the nPduUdpTxBuffer */
    TxBufferStatePtr->TxLen = 0U;

    /* stop the nPduUdpTxTimer */
    TxBufferStatePtr->TxTimer = 0U;
  }

  /* Clear buffer retry flag for socket connection. */
  SOAD_FLAG_CLR_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_UDP_RETRY);

  if(WaitForTxConfirmation)
  {
    /* set a flag for UDP socket connection, that the transmit confirmation is
     * called in the context of the next SoAd_MainFunctionUdpTxConf() */
    SOAD_FLAG_SET_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TXCONF_PENDING);
    SoAd_SetBitInBitMap(SoConPtr, &SoAd_PBcfg->SoConTxIFNotIdleBitMapBase, &SoAd_TxIfCondChange);
  }

  if(ResetRemoteAddr && SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_AUTO_SETUP))
  {
    SoAd_ResetWildCardSoCon(SoConPtr, SoConStatePtr);
  }

  DBG_SOAD_CLEANUPUDPTXBUFFER_EXIT(SoConPtr, SoConStatePtr, TransmissionOk);
}

STATIC FUNC(boolean, SOAD_CODE) SoAd_CheckTcpIpSocketInUse
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr
)
{
  boolean TcpIpSocketInUse = FALSE;
  SoAd_PduRouteDestVirtualIdType PduRouteDestVirtualId = SOAD_INVALID_DEST_VIRTUAL_ID;

  const boolean IsTcp = SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_TCP) ? TRUE : FALSE;

  if(IsTcp)
  {
   /* Each TCP SoCon has its own TcpIp socket */
    PduRouteDestVirtualId = SoAd_GetActivePduRouteDestVirtualId(SoConPtr);
    if(SOAD_INVALID_DEST_VIRTUAL_ID != PduRouteDestVirtualId)
    {
      TcpIpSocketInUse = TRUE;
    }
  }
  else
  {
    SoAd_SoConIdType Idx;
    CONSTP2CONST(SoAd_SoConGroupType, AUTOMATIC, SOAD_APPL_DATA) SoConGroupPtr =
      &SOAD_CFG(SoConGroup, SoConPtr->SoConGroupId);
    const SoAd_SoConIdType SoConCnt = SoConGroupPtr->SoConCnt;

    /* Since UDP uses one TcpIp socket per SoCon group, loop over all socket connection of this
       group to verify that the TcpIp socket is not used. */
    for(Idx = 0U; Idx < SoConCnt; Idx++)
    {
      CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) TmpSoConPtr =
        &SOAD_CFG(SoCon, SoConGroupPtr->SoConFirstId + Idx);

      PduRouteDestVirtualId = SoAd_GetActivePduRouteDestVirtualId(TmpSoConPtr);

      if(SOAD_INVALID_DEST_VIRTUAL_ID != PduRouteDestVirtualId)
      {
        TcpIpSocketInUse = TRUE;
        break;
      }
    }
  }

  return TcpIpSocketInUse;
}

#if(TS_MERGED_COMPILE == STD_OFF)
#define SOAD_STOP_SEC_CODE
#include <SoAd_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */
/*==================[end of file]===========================================*/
