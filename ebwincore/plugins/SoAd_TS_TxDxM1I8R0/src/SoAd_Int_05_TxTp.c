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

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/
#if(TS_MERGED_COMPILE == STD_OFF)
#define SOAD_START_SEC_CODE
#include <SoAd_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

STATIC FUNC(void, SOAD_CODE) SoAd_MainFunctionTpTxUdp
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(SoAd_SoConTxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConTxDataPtr,
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr
);

STATIC FUNC(void, SOAD_CODE) SoAd_HandleNotOkAfterTrySend
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(SoAd_SoConTxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConTxDataPtr,
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr
);

STATIC FUNC(void, SOAD_CODE) SoAd_MainFunctionTpTxTcp
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(SoAd_SoConTxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConTxDataPtr,
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr
);


#if(SOAD_META_DATA_HANDLING == STD_ON)
  /** \brief Service get PduRouteDestVirtualId from PduRoutePtr
 **
 ** The service gets PduRouteDestVirtualId of a given PduRoute based on stored meta data SoConId
 ** stored for this PDU.
 ** \param[in] PduRoutePtr - Pointer to the PduRoute the meta data is stored.
 ** \param[out] PduRouteDestVirtualIdPtr - Pointer return the corresponding virtual PDU route
 **                                        destination ID matching to the PDU meta data. It is
                                           invalid if nothing is found.
 **                            free space.
 ** \param[in] SoAdSrcPduInfoPtr - PduInfo pointer to be passed to ECUC.
 **/
STATIC FUNC(void, SOAD_CODE) SoAd_TpTransmit_MetaData_GetPduRouteDestVirtualId
(
  CONSTP2CONST(SoAd_PduRouteType, AUTOMATIC, SOAD_APPL_DATA) PduRoutePtr,
  CONSTP2VAR(SoAd_PduRouteDestVirtualIdType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualIdPtr,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) SoAdSrcPduInfoPtr
);
#endif /* (SOAD_META_DATA_HANDLING == STD_ON) */

STATIC FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_ULTpCopyTxData
(
  SoAd_RouteIdType PduRouteId,
  CONSTP2VAR(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) PduInfoPtr,
  CONSTP2VAR(RetryInfoType, AUTOMATIC, SOAD_APPL_DATA) RetryPtr,
  CONSTP2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_DATA) AvailableDataPtr
);


/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/


FUNC(Std_ReturnType, SOAD_CODE) SoAd_TpTransmit
(
  PduIdType SoAdSrcPduId,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) SoAdSrcPduInfoPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_SOAD_TPTRANSMIT_ENTRY(SoAdSrcPduId, SoAdSrcPduInfoPtr);

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  if(SoAd_Initialized == SOAD_UNINITED)
  {
    SOAD_DET_REPORT_ERROR(SOAD_TPTRANSMIT_SVCID, SOAD_E_NOTINIT);
  }
  else if(SoAdSrcPduInfoPtr == NULL_PTR)
  {
    SOAD_DET_REPORT_ERROR(SOAD_TPTRANSMIT_SVCID, SOAD_E_PARAM_POINTER);
  }
  else if(SoAdSrcPduId >= SoAd_PBcfg->PduRouteCnt)
  {
    SOAD_DET_REPORT_ERROR(SOAD_TPTRANSMIT_SVCID, SOAD_E_INV_PDUID);
  }
  else if(!SOAD_FLAG(&SOAD_CFG(PduRoute, SoAdSrcPduId), SOAD_PDUROUTE_FLAG_IS_UL_TP))
  {
    SOAD_DET_REPORT_ERROR(SOAD_TPTRANSMIT_SVCID, SOAD_E_INV_PDUID);
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    SoAd_PduRouteDestVirtualIdType PduRouteDestVirtualId = SOAD_INVALID_DEST_VIRTUAL_ID;
    const SoAd_RouteIdType PduRouteId = SOAD_TX_PDUID_TO_PDUROUTE_ID(SoAdSrcPduId);
    CONSTP2CONST(SoAd_PduRouteType, AUTOMATIC, SOAD_APPL_DATA) PduRoutePtr =
      &SOAD_CFG(PduRoute, PduRouteId);

#if(SOAD_META_DATA_HANDLING == STD_ON)
    SoAd_TpTransmit_MetaData_GetPduRouteDestVirtualId
    (
      PduRoutePtr, &PduRouteDestVirtualId, SoAdSrcPduInfoPtr
    );
#else
    /* It is ensured by xdm check that for TP only 1 destination exists. */
    PduRouteDestVirtualId = PduRoutePtr->PduRouteDestVirtualFirstId;
#endif /* (SOAD_META_DATA_HANDLING == STD_ON) */

    if(SOAD_INVALID_DEST_VIRTUAL_ID != PduRouteDestVirtualId)
    {
      P2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr
        = &SOAD_CFG(PduRouteDestVirtual, PduRouteDestVirtualId);
      CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr =
        &SOAD_CFG(SoCon, PduRouteDestVirtualPtr->SoConId);
      CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr =
        &SOAD_PBRAM(SoConState, PduRouteDestVirtualPtr->SoConId);
      CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) TxEnabledRamPtr =
        &SOAD_PBRAM_REF_ID(uint32, SoAd_PBcfg->PduRouteDestVirtualTxEnabledBitMapBase, 0U);

      /* variables set in critical section */
      PduLengthType TcpIpTxLen = 0U;
      P2VAR(SoAd_SoConTxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConTxDataPtr = NULL_PTR;

      const boolean IsTcp = SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_TCP) ? TRUE : FALSE;
      const uint32 HeaderSize =
        (SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_PDU_HEADER_MODE) ? SOAD_PDU_HEADER_LEN : 0U);
      const uint32 TotalPduLength = (uint32)SoAdSrcPduInfoPtr->SduLength + HeaderSize;

      /* UDP protocol can only send up to 64k */
      if((!IsTcp) && (TotalPduLength > 0xFFFFU))
      {
        RetVal = E_NOT_OK;
#if(SOAD_DEV_ERROR_DETECT == STD_ON)
        SOAD_DET_REPORT_ERROR(SOAD_TPTRANSMIT_SVCID, SOAD_E_INV_ARG);
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
      }
      else
      {
        RetVal = E_OK;
      }

      SoAd_GetSoConTxData(SoConPtr, SoConTxDataPtr);

      SOAD_ENTER_CRITSEC(); /* CS1 */
      if(E_OK == RetVal)
      {
        RetVal = E_NOT_OK;
        if(SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_TP_PENDING))
        {
          /* Tp transmission is in progress. Return E_NOT_OK. */
        }
        /*Check if Tx is disabled for PduRouteDestVirtualId*/
        else if(SOAD_GET_BIT_FROM_32BIT_BITARRAY(TxEnabledRamPtr, PduRouteDestVirtualId) == 0U)
        {
          /* Pdu route is disabled. */
        }
        else
        {
          /* Checks passed */
          RetVal = E_OK;
        }

        if(E_OK == RetVal)
        {
          if(!IsTcp)
          {
            /* Udp requires a buffer */
            P2CONST(SoAd_SoConTxBufferCfgType, AUTOMATIC, SOAD_APPL_DATA) TxBufferCfgPtr = NULL_PTR;
            SoAd_GetSoConTxBufferCfg(SoConPtr, TxBufferCfgPtr);

            /* Check buffersize and if Tx path is not locked due to ongoing remote address release and autosetup is disabled*/
            if(TxBufferCfgPtr->Capacity < SoAdSrcPduInfoPtr->SduLength ||
               SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_LOCK)
              )
            {
              RetVal = E_NOT_OK;
            }
          }
        }
      }

      if(E_OK == RetVal)
      {
        RetVal = SoAd_PrepareTransmit
        (
          SoConPtr,
          SoConStatePtr,
          SoConTxDataPtr,
          SoAdSrcPduInfoPtr,
          &TcpIpTxLen,
          PduRouteDestVirtualPtr
        );

        if(E_OK == RetVal)
        {
          /* the request has been accepted */
          SOAD_FLAG_SET_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_TP_PENDING);
          SOAD_FLAG_SET_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_TP_NO_DATA_COPIED);
          SoAd_SetBitInBitMap(SoConPtr,&SoAd_PBcfg->SoConTxTPNotIdleBitMapBase,&SoAd_TxTpCondChange);
        }
      }

      SOAD_EXIT_CRITSEC();

    }
    else
    {
      RetVal = E_NOT_OK;
    }
  }

  DBG_SOAD_TPTRANSMIT_EXIT(RetVal, SoAdSrcPduId, SoAdSrcPduInfoPtr);
  return RetVal;
}

FUNC(Std_ReturnType, SOAD_CODE) SoAd_TpCancelTransmit
(
  PduIdType PduId
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_SOAD_TPCANCELTRANSMIT_ENTRY(PduId);

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  if(SoAd_Initialized == SOAD_UNINITED)
  {
    SOAD_DET_REPORT_ERROR(SOAD_TPCANCELTRANSMIT_SVCID, SOAD_E_NOTINIT);
  }
  else if(PduId >= SoAd_PBcfg->PduRouteCnt)
  {
    SOAD_DET_REPORT_ERROR(SOAD_TPCANCELTRANSMIT_SVCID, SOAD_E_INV_PDUID);
  }
  else if(!SOAD_FLAG(&SOAD_CFG(PduRoute,PduId), SOAD_PDUROUTE_FLAG_IS_UL_TP))
  {
    SOAD_DET_REPORT_ERROR(SOAD_TPCANCELTRANSMIT_SVCID, SOAD_E_INV_PDUID);
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    const SoAd_RouteIdType CancelPduRouteId = SOAD_TX_PDUID_TO_PDUROUTE_ID(PduId);
    const SoAd_SoConIdType SoConId =
      SOAD_CFG(PduRouteDestVirtual, SOAD_CFG(PduRoute, CancelPduRouteId).PduRouteDestVirtualFirstId).SoConId;
    CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr =
      &SOAD_CFG(SoCon, SoConId);
    CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr =
      &SOAD_PBRAM(SoConState, SoConId);

    SOAD_ENTER_CRITSEC();
    {
      const SoAd_PduRouteDestVirtualIdType PduRouteDestVirtualId
        = SoAd_GetActivePduRouteDestVirtualId(SoConPtr);

      if(PduRouteDestVirtualId != SOAD_INVALID_DEST_VIRTUAL_ID)
      {
        CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA)
          PduRouteDestVirtualPtr = &SOAD_CFG(PduRouteDestVirtual, PduRouteDestVirtualId);

        /* check that the right PDU is canceled */
        if(CancelPduRouteId == PduRouteDestVirtualPtr->PduRouteId)
        {
          SOAD_FLAG_SET_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_TP_CANCELED);

          /* Set global flag to know that for a certain SoCon a transition condition was met. */
          SoAd_SetBitInTransCondChange(SoConPtr);

          RetVal = E_OK;
        }
      }
    }
    SOAD_EXIT_CRITSEC();
  }

  DBG_SOAD_TPCANCELTRANSMIT_EXIT(RetVal, PduId);
  return RetVal;
}


#if(SOAD_ENABLE_MAIN_FUNCTION_TX == STD_ON)
FUNC(void, SOAD_CODE) SoAd_MainFunctionTx
#else
TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_MainFunctionTx
#endif
(
  void
)
{
  DBG_SOAD_MAINFUNCTIONTX_ENTRY();

  if(SoAd_Initialized == SOAD_INITED)
  {
    boolean StateTxTpCondChange;

    TS_AtomicAssign8(StateTxTpCondChange, SoAd_TxTpCondChange);
    if(TRUE == StateTxTpCondChange)
    {
      uint16 FlagTxPending;
      uint8 BusyCnt;
      P2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr;
      P2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr;
      uint32 NextSoConId;
      const SoAd_SoConIdType SoConMax = SoAd_PBcfg->SoConCnt;
      /* Tx TP*/
      CONSTP2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) TxTpCondChange =
                          &SOAD_PBRAM_REF_ID(uint32, SoAd_PBcfg->SoConTxTPNotIdleBitMapBase, 0U);
      TS_AtomicAssign8(SoAd_TxTpCondChange, FALSE);

      /* Deviation MISRAC2012-3 */
      for(SOAD_COMSTACK_BF_ITERATE(NextSoConId, &TxTpCondChange[0U], COMSTACK_BF_START, SoConMax))
      {
        SoConPtr = &SOAD_CFG(SoCon, NextSoConId);
        SoConStatePtr = &SOAD_PBRAM(SoConState, NextSoConId);
        /* Before anything else, clear the Status Bit in the map, because preemption could already
        set it during the Rx Action */
        SoAd_ClearBitInBitMap(SoConPtr,&SoAd_PBcfg->SoConTxTPNotIdleBitMapBase);

        /* BusyCnt ensures that SoAd_MainFunction() does not interrupt an Rx/Tx triggering API
           request. */
        TS_AtomicAssign8(BusyCnt, SoConStatePtr->BusyCnt);

        if(BusyCnt == 0U)
        {
          /* If a TP transmission is outstanding it shall be processed. */
          TS_AtomicAssign16(FlagTxPending, SoConStatePtr->UpperFlags);
          FlagTxPending &= SOAD_SOCONSTATE_FLAG_TX_TP_PENDING_MASK;

          /* Flag is integrated into SoConTxTPNotIdleBitMapBase. IF could be unreachable here. */
          if(FlagTxPending != 0U)
          {
            const SoAd_PduRouteDestVirtualIdType PduRouteDestVirtualId
              = SoAd_GetActivePduRouteDestVirtualId(SoConPtr);
            P2VAR(SoAd_SoConTxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConTxDataPtr = NULL_PTR;

            SoAd_GetSoConTxData(SoConPtr, SoConTxDataPtr);

            if
            (
              (PduRouteDestVirtualId != SOAD_INVALID_DEST_VIRTUAL_ID)
              && (SoConTxDataPtr->TcpIpTxRemainingLen != 0U)
            )
            {
              CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA)
                PduRouteDestVirtualPtr = &SOAD_CFG(PduRouteDestVirtual, PduRouteDestVirtualId);

              if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_TCP))
              {
                SoAd_MainFunctionTpTxTcp
                (
                  SoConPtr, SoConStatePtr, SoConTxDataPtr, PduRouteDestVirtualPtr
                );
              }
              else
              {
                SoAd_MainFunctionTpTxUdp
                (
                  SoConPtr, SoConStatePtr, SoConTxDataPtr, PduRouteDestVirtualPtr
                );
              }
            }
          }
        }
        else
        {
          /* Prepare the Map to open this SoCon with the next Main Function */
          SoAd_SetBitInBitMap(SoConPtr,&SoAd_PBcfg->SoConTxTPNotIdleBitMapBase,&SoAd_TxTpCondChange);
        }
      }
    }
  }

  DBG_SOAD_MAINFUNCTIONTX_EXIT();
}

TS_MOD_PRIV_DEFN FUNC(void, SOAD_CODE) SoAd_Int_05_TpFinishTransmission
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr
)
{
  NotifResultType NotifResult = NTFRSLT_E_NOT_OK;
  P2VAR(SoAd_SoConTxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConTxDataPtr = NULL_PTR;
  CONSTP2VAR(SoAd_PduRouteDestVirtualDataType, AUTOMATIC, SOAD_APPL_DATA) PduTxDataPtr =
    &SOAD_PBRAM(PduRouteDestVirtualData, PduRouteDestVirtualPtr->PduRouteDestVirtualId);

  SoAd_GetSoConTxData(SoConPtr, SoConTxDataPtr);

  /* Perform cleanup before calling TxConfirmation() to ensure that a transmission of the same PDU
     in the context of TxConfirmation() is possible. */
  SOAD_ENTER_CRITSEC();
  /* Cleanup of SoCon */
  /* Clear Tx Tp pending flag */
  SOAD_FLAG_CLR_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_TP_PENDING);
  SoConTxDataPtr->ActivePduRouteDestVirtualId = SOAD_INVALID_DEST_VIRTUAL_ID;

  /* Cleanup of PduRoute */
  switch(PduTxDataPtr->PduRouteDestVirtualState)
  {
    case SOAD_PDUROUTEDESTVIRTUALSTATE_CONF_OK:
    {
      NotifResult = NTFRSLT_OK;
      break;
    }
    case SOAD_PDUROUTEDESTVIRTUALSTATE_CONF_NOT_OK:
    {
      NotifResult = NTFRSLT_E_NOT_OK;
      break;
    }
    /* CHECK: NOPARSE */
    default:
    {
      SOAD_UNREACHABLE_CODE_ASSERT(SOAD_MAINFUNCTION_SVCID);
      break; /* Caution: this 'break' is required by MISRA-C:2004 */
    }
    /* CHECK: PARSE */
  }
  SoAd_SetPduRouteDestVirtualState(PduRouteDestVirtualPtr, SOAD_PDUROUTEDESTVIRTUALSTATE_IDLE);

  if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_AUTO_SETUP))
  {
    SoAd_ResetWildCardSoCon(SoConPtr, SoConStatePtr);
  }

  SOAD_EXIT_CRITSEC();

  SoAd_ULTpTxConfirmation(PduRouteDestVirtualPtr->PduRouteId, NotifResult);

}

TS_MOD_PRIV_DEFN FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_CopyTxData_TpTcp
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr,
  uint16 BufLength
)
{
  BufReq_ReturnType RetVal_BufReq = BUFREQ_E_NOT_OK;
  SoAd_BufLengthType RemainingBufLength = BufLength;
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) PayloadDestPtr = BufPtr;
  P2VAR(SoAd_SoConTxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConTxDataPtr = NULL_PTR;
  SoAd_BufLengthType CopyNow = 0U;
  PduInfoType PduInfo;
  PduLengthType AvailableData = 0U;

  SoAd_GetSoConTxData(SoConPtr, SoConTxDataPtr);

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
  CopyNow = (SoAd_BufLengthType)SOAD_MIN(SoConTxDataPtr->TcpIpTxRemainingLen, RemainingBufLength);
  PduInfo.SduDataPtr = PayloadDestPtr;
  PduInfo.SduLength = CopyNow;
  RetVal_BufReq = SoAd_ULTpCopyTxData
  (
    PduRouteDestVirtualPtr->PduRouteId, &PduInfo, NULL_PTR, &AvailableData
  );

  if(RetVal_BufReq == BUFREQ_OK)
  {
    CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr =
      &SOAD_PBRAM(SoConState, SoConPtr->SoConId);
    SoConTxDataPtr->TcpIpTxRemainingLen -= CopyNow;
    SOAD_FLAG_CLR_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_TP_NO_DATA_COPIED);
  }

  return RetVal_BufReq;
}



/*==================[internal function definitions]=========================*/

STATIC FUNC(void, SOAD_CODE) SoAd_MainFunctionTpTxUdp
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(SoAd_SoConTxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConTxDataPtr,
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr
)
{
  SoAd_RouteIdType PduRouteId;

  P2CONST(SoAd_SoConTxBufferCfgType, AUTOMATIC, SOAD_APPL_DATA) TxBufferCfgPtr = NULL_PTR;
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) TxBufferPtr = NULL_PTR;
  uint8 PduHeaderOffset =
    (SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_PDU_HEADER_MODE) ? SOAD_PDU_HEADER_LEN : 0U);
  boolean TransmitUdpDatagram = FALSE;
  boolean TxFinish = FALSE;

  SoAd_GetSoConTxBufferCfg(SoConPtr, TxBufferCfgPtr);
  TxBufferPtr = &SOAD_PBRAM(SoAdBuffer, TxBufferCfgPtr->BufId);

  DBG_SOAD_MAINFUNCTIONTPTXUDP_ENTRY
  (
    SoConPtr, SoConStatePtr, SoConTxDataPtr, PduRouteDestVirtualPtr
  );
  /* data needs to be copied from UL into local buffer */
  if(SoConTxDataPtr->PduPayloadRemainingLen != 0U)
  {
    BufReq_ReturnType RetVal_BufReq;
    PduInfoType PduInfo = {NULL_PTR, 0U};
    PduLengthType AvailableData = 0U;

    PduRouteId = PduRouteDestVirtualPtr->PduRouteId;

    RetVal_BufReq = SoAd_ULTpCopyTxData(PduRouteId, &PduInfo, NULL_PTR, &AvailableData);

    if((RetVal_BufReq == BUFREQ_OK) && (AvailableData > 0U))
    {
      PduInfo.SduDataPtr =
        &TxBufferPtr[
                      ((uint16)(SoConTxDataPtr->TcpIpTxRemainingLen - PduHeaderOffset))
                      - SoConTxDataPtr->PduPayloadRemainingLen
                    ];
      PduInfo.SduLength = SOAD_MIN(SoConTxDataPtr->PduPayloadRemainingLen, AvailableData);

      RetVal_BufReq = SoAd_ULTpCopyTxData(PduRouteId, &PduInfo, NULL_PTR, &AvailableData);
    }

    switch(RetVal_BufReq)
    {
      case BUFREQ_OK:
      {
        /* If available = 0 has no impact since also SduLength is 0. */
        SoConTxDataPtr->PduPayloadRemainingLen -= PduInfo.SduLength;

        /* If all data was copied from the UL */
        if(SoConTxDataPtr->PduPayloadRemainingLen == 0U)
        {
          /* Set flag to call TcpIp_UdpTransmit */
          TransmitUdpDatagram = TRUE;
        }
        break;
      }
      case BUFREQ_E_BUSY:
      {
        /* BUFREQ_E_BUSY was returned - retry later */
        break;
      }
      case BUFREQ_E_NOT_OK: /* fall through */
      default:
      {
        /* Upper layer refused to provide data, abort transmission (no reconnection required) */
        TxFinish = TRUE;
        SoAd_SetPduRouteDestVirtualState(PduRouteDestVirtualPtr, SOAD_PDUROUTEDESTVIRTUALSTATE_CONF_NOT_OK);
        break;
      }
    }
  }
  else
  {
    TransmitUdpDatagram = TRUE; /* All UL data available. Perform retry. */
  }

  /* if all data has been copied from UL into local buffer */
  if(TransmitUdpDatagram == TRUE)
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

    /* provide remote address */
    SoAd_AssignRemoteAddressSoCon2SockAddrType(SoConPtr, SoConStatePtr, RemoteAddrPtr);

    SoConTxDataPtr->ULIfDataPtr = TxBufferPtr;

    {
      TcpIp_ReturnType RetVal_TcpIp;
      SOAD_FLAG_SET_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_COPYTXDATA);
      RetVal_TcpIp = TcpIp_UdpTransmit
      (
        SoConStatePtr->SocketId,
        NULL_PTR,
        RemoteAddrPtr,
        (uint16)SoConTxDataPtr->TcpIpTxRemainingLen
      );
      SOAD_FLAG_CLR_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_COPYTXDATA);

      /* Note: If the UDP Retry feature is enabled for this socket connection and the physical
       * remote address is still unknown (i.e. TcpIp_UdpTransmit returns TCPIP_E_PHYS_ADDR_MISS)
       * the transmission control information remains unchanged, the transmission will be
       * retried in the next MainFunction */

      switch(RetVal_TcpIp)
      {
        case TCPIP_OK:
        {
          TxFinish = TRUE;
          SoAd_SetPduRouteDestVirtualState(PduRouteDestVirtualPtr, SOAD_PDUROUTEDESTVIRTUALSTATE_CONF_OK);
          /* Indirect Tx. Precondition verifies that SoAd_CopyTxData() was successfully called. */
          SOAD_PRECONDITION_ASSERT(SoConTxDataPtr->TcpIpTxRemainingLen == 0U, SOAD_INTERNAL_SVCID);
          break;
        }
        case TCPIP_E_PHYS_ADDR_MISS:
        {
          if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_UDP_RETRY))
          {
            /* Note: If the UDP Retry feature is enabled for this socket connection and the physical
               remote address is still unknown (i.e. TcpIp_UdpTransmit returns
               TCPIP_E_PHYS_ADDR_MISS)
               the transmission control information remains unchanged, the transmission will be
               retried in the next MainFunction */
          }
          else
          {
            TxFinish = TRUE;
            SoAd_SetPduRouteDestVirtualState(PduRouteDestVirtualPtr, SOAD_PDUROUTEDESTVIRTUALSTATE_CONF_NOT_OK);
          }
          break;
        }
        default:
        {
          TxFinish = TRUE;
          SoAd_SetPduRouteDestVirtualState(PduRouteDestVirtualPtr, SOAD_PDUROUTEDESTVIRTUALSTATE_CONF_NOT_OK);
          break;
        }
      }
    }
  }

  if(TxFinish)
  {
    SoAd_Int_05_TpFinishTransmission
    (
      SoConPtr, SoConStatePtr, PduRouteDestVirtualPtr
    );
  }
  else
  {
    /* Prepare the Map to open this SoCon with the next Main Function */
    SoAd_SetBitInBitMap(SoConPtr,&SoAd_PBcfg->SoConTxTPNotIdleBitMapBase,&SoAd_TxTpCondChange);
  }

  DBG_SOAD_MAINFUNCTIONTPTXUDP_EXIT
  (
    SoConPtr, SoConStatePtr, SoConTxDataPtr, PduRouteDestVirtualPtr
  );
}

STATIC FUNC(void, SOAD_CODE) SoAd_HandleNotOkAfterTrySend
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(SoAd_SoConTxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConTxDataPtr,
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr
)
{
  /* !LINKSTO SoAd.dsn.Func0140.Tx.Tcp.Tp.NoDataSent, 1 */
  if (SOAD_FLAG_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_TP_NO_DATA_COPIED))
  {
    /* Something went wrong. */
    if(PduRouteDestVirtualPtr->PduTxTcpDataId != SOAD_INVALID_PDUTXTCPDATA_ID)
    {
      P2VAR(SoAd_PduTxTcpDataType, AUTOMATIC, SOAD_APPL_DATA) PduTxTcpDataPtr = NULL_PTR;
      SoAd_GetPduTxTcpData(PduRouteDestVirtualPtr, PduTxTcpDataPtr);
      /* Complete PDU was canceled. */
      PduTxTcpDataPtr->PduTxConfCtr = 0U;
    }

    if(SoConPtr->SoConTxTcpDataId != SOAD_INVALID_SOCON_ID)
    {
      P2VAR(SoAd_SoConTxTcpDataType, AUTOMATIC, SOAD_APPL_DATA) SoConTxTcpDataPtr = NULL_PTR;
      SoAd_GetSoConTxTcpData(SoConPtr, SoConTxTcpDataPtr);
      /* !LINKSTO SoAd.Func0111.dsn.Tcp.Tp.Tx.HeaderMode, 1 */
      /* Revert/Decrement the TxConfCtr since nothing was transmitted.
      TxConf could still be pending for other PDUs which are already
      transmitted.*/
      SoConTxTcpDataPtr->TxConfCtr -= SOAD_MIN(SoConTxDataPtr->TcpIpTxRemainingLen, SoConTxTcpDataPtr->TxConfCtr);
    }
  }
  else
  {
    /* Upper layer refused to provide data, abort transmission and perform reconnect */
    /* !LINKSTO SoAd.dsn.Func0140.Tx.Tcp.Tp.DataSent.CloseSocketConnection, 1 */
    SOAD_FLAG_SET_LO(SoConStatePtr, SOAD_SOCONSTATE_FLAG_RECONNECT_SOCKET);
    /* Update TXING_OK_State */
    /* !LINKSTO SoAd.dsn.Func0140.Tx.Tcp.Tp.DataSent.DisableFurtherCom, 1 */
    SOAD_FLAG_CLR_UP(SoConStatePtr,SOAD_SOCONSTATE_TXING_OK);
    /* Set global flag to know that for a certain SoCon a transition condition was met. */
    SoAd_SetBitInTransCondChange(SoConPtr);
  }
  /* Inform upper layer. */
  SoAd_SetPduRouteDestVirtualState(PduRouteDestVirtualPtr, SOAD_PDUROUTEDESTVIRTUALSTATE_CONF_NOT_OK);
}

/* ==== TCP ==== */

STATIC FUNC(void, SOAD_CODE) SoAd_MainFunctionTpTxTcp
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(SoAd_SoConTxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConTxDataPtr,
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr
)
{
  BufReq_ReturnType RetVal_BufReq;
  PduInfoType PduInfo = {NULL_PTR, 0U};
  PduLengthType AvailableData = 0U;
  const uint8 RemainingPduHeaderLen =
      (SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_PDU_HEADER_MODE)) ?
      (SOAD_PDU_HEADER_LEN - SoConTxDataPtr->PduHeaderWrittenLen) : (0U);
  boolean TxFinished = FALSE;
  Std_ReturnType RetVal_TcpIp = E_NOT_OK;

  DBG_SOAD_MAINFUNCTIONTPTXTCP_ENTRY
  (
    SoConPtr, SoConStatePtr, SoConTxDataPtr, PduRouteDestVirtualPtr
  );

  /* Query available data from upper layer. */
  RetVal_BufReq =
    SoAd_ULTpCopyTxData(PduRouteDestVirtualPtr->PduRouteId, &PduInfo, NULL_PTR, &AvailableData);

  switch(RetVal_BufReq)
  {
    case BUFREQ_OK:
    {
      /* If data is available. */
      if(AvailableData > 0U)
      {
        SOAD_FLAG_SET_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_COPYTXDATA);
        /* call transmit */
        RetVal_TcpIp = TcpIp_TcpTransmit
        (
          SoConStatePtr->SocketId,
          NULL_PTR,
          (uint32)((uint32)AvailableData + (uint32)RemainingPduHeaderLen),
          FALSE
        );
        SOAD_FLAG_CLR_UP(SoConStatePtr, SOAD_SOCONSTATE_FLAG_TX_COPYTXDATA);

        SOAD_ENTER_CRITSEC();
        if(RetVal_TcpIp == E_OK)
        {
          /* If all data was retrieved by TcpIp module. */
          if(SoConTxDataPtr->TcpIpTxRemainingLen == 0U)
          {
            /* If the socket connection is configured for immediate TP transmission confirmation. */
            if(SOAD_FLAG(SoConPtr, SOAD_SOCON_FLAG_IS_TCP_IMMEDIATE_TP_TXCONF))
            {
              /* TcpIp is still going to confirm this transmission as soon as TCP acknowledgment
                 arrives. This needs to be handled in SoAd_TxConfirmation()*/
              SoAd_SetPduRouteDestVirtualState(PduRouteDestVirtualPtr, SOAD_PDUROUTEDESTVIRTUALSTATE_CONF_OK);

              /* upper layer transmit confirmation */
              TxFinished = TRUE;
            }
            else
            {
              /* Set transmission state to WAITCONF
                 to give the transmit confirmation after the TCP acknowledgment arrives. */
              SoAd_SetPduRouteDestVirtualState(PduRouteDestVirtualPtr, SOAD_PDUROUTEDESTVIRTUALSTATE_WAITCONF);
            }
          }
          /* else: Not all data are copied yet. Try again in next main function */
        }
        else
        {
          SoAd_HandleNotOkAfterTrySend
            (SoConPtr, SoConStatePtr, SoConTxDataPtr, PduRouteDestVirtualPtr);
          TxFinished = TRUE;
        }
        SOAD_EXIT_CRITSEC();
      }
      break;
    }
    case BUFREQ_E_BUSY:
    {
      /* BUFREQ_E_BUSY was returned - retry later */
      break;
    }
    case BUFREQ_E_NOT_OK: /* fall through */
    default:
    {
      SoAd_HandleNotOkAfterTrySend
        (SoConPtr, SoConStatePtr, SoConTxDataPtr, PduRouteDestVirtualPtr);
      TxFinished = TRUE;
      break;
    }
  }

  if(TxFinished)
  {
    SoAd_Int_05_TpFinishTransmission
    (
      SoConPtr, SoConStatePtr, PduRouteDestVirtualPtr
    );
  }
  else
  {
    /* Prepare the Map to open this SoCon with the next Main Function */
    SoAd_SetBitInBitMap(SoConPtr,&SoAd_PBcfg->SoConTxTPNotIdleBitMapBase,&SoAd_TxTpCondChange);
  }

  DBG_SOAD_MAINFUNCTIONTPTXTCP_EXIT
  (
    SoConPtr, SoConStatePtr, SoConTxDataPtr, PduRouteDestVirtualPtr
  );
}


#if(SOAD_META_DATA_HANDLING == STD_ON)
STATIC FUNC(void, SOAD_CODE) SoAd_TpTransmit_MetaData_GetPduRouteDestVirtualId
(
  CONSTP2CONST(SoAd_PduRouteType, AUTOMATIC, SOAD_APPL_DATA) PduRoutePtr,
  CONSTP2VAR(SoAd_PduRouteDestVirtualIdType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualIdPtr,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) SoAdSrcPduInfoPtr
)
{
  SoAd_PduRouteDestVirtualIdType PduRouteDest_loop; /* SoConId iterator for Tx Pdu fan-out */
  const SoAd_PduRouteDestVirtualIdType PduRouteDestCnt = PduRoutePtr->PduRouteDestVirtualCnt;
  SoAd_SoConIdType SoConId_EcuC = SOAD_INVALID_SOCON_ID;
  P2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr;
  Std_ReturnType RetVal = E_NOT_OK;

  /* Set out parameter to invalid by default. */
  *PduRouteDestVirtualIdPtr = SOAD_INVALID_DEST_VIRTUAL_ID;

  if(SOAD_FLAG(PduRoutePtr, SOAD_PDUROUTE_FLAG_METADATA_SOCON_ID))
  {
    RetVal = EcuC_GetMetaDataSoConId(PduRoutePtr->EcuCPduId, SoAdSrcPduInfoPtr, &SoConId_EcuC);
  }

  for(
       PduRouteDest_loop = 0U;
       PduRouteDest_loop < PduRouteDestCnt;
       PduRouteDest_loop++
     )
  {
    PduRouteDestVirtualPtr
      = &SOAD_CFG(PduRouteDestVirtual, PduRoutePtr->PduRouteDestVirtualFirstId + PduRouteDest_loop);

    if(SOAD_FLAG(PduRoutePtr, SOAD_PDUROUTE_FLAG_METADATA_SOCON_ID))
    {
      if((SoConId_EcuC == PduRouteDestVirtualPtr->SoConId) && (E_OK == RetVal))
      {
        *PduRouteDestVirtualIdPtr = PduRouteDestVirtualPtr->PduRouteDestVirtualId;
        break;
      }
    }
    else
    {
      /* It is ensured by xdm check that for TP only 1 destination exists. */
      *PduRouteDestVirtualIdPtr = PduRoutePtr->PduRouteDestVirtualFirstId;
    }
  }
#if(SOAD_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO SoAd.dsn.Metadata.Tx.SoConId.Invalid,1 */
  if(*PduRouteDestVirtualIdPtr == SOAD_INVALID_DEST_VIRTUAL_ID)
  {
    SOAD_DET_REPORT_ERROR(SOAD_TPTRANSMIT_SVCID, SOAD_E_INV_METADATA);
  }
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
}
#endif /* (SOAD_META_DATA_HANDLING == STD_ON)  */

STATIC FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_ULTpCopyTxData
(
  SoAd_RouteIdType PduRouteId,
  CONSTP2VAR(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) PduInfoPtr,
  CONSTP2VAR(RetryInfoType, AUTOMATIC, SOAD_APPL_DATA) RetryPtr,
  CONSTP2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_DATA) AvailableDataPtr
)
{
  BufReq_ReturnType RetVal_BufReq = BUFREQ_E_NOT_OK;
  CONSTP2CONST(SoAd_PduRouteType, AUTOMATIC, SOAD_APPL_DATA) PduRoutePtr =
      &SOAD_CFG(PduRoute, PduRouteId);

  DBG_SOAD_ULTPCOPYTXDATA_ENTRY(PduRouteId, PduInfoPtr, RetryPtr, AvailableDataPtr);

  SOAD_PRECONDITION_ASSERT(PduRoutePtr->ULFuncId != SOAD_INVALID_ULFUNCID, DET_INTERNAL_API_ID);
  RetVal_BufReq = SoAd_TpModuleAPI[PduRoutePtr->ULFuncId].TpCopyTxData
  (
    PduRoutePtr->TxConfPduId,
    PduInfoPtr,
    RetryPtr,
    AvailableDataPtr
  );

  DBG_SOAD_ULTPCOPYTXDATA_EXIT(RetVal_BufReq, PduRouteId, PduInfoPtr, RetryPtr, AvailableDataPtr);
  return RetVal_BufReq;
}



#if(TS_MERGED_COMPILE == STD_OFF)
#define SOAD_STOP_SEC_CODE
#include <SoAd_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */
/*==================[end of file]===========================================*/
