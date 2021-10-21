/**
 * \file
 *
 * \brief AUTOSAR FrArTp
 *
 * This file contains the implementation of the AUTOSAR
 * module FrArTp.
 *
 * \version 1.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W507
 * W507: variable "frameType" is possibly uninitialized violated
 *
 * Reason:
 * False positive. frameType is only evaluated if RetCode == E_OK, which is also the condition for
 * frameType being set.
 */

/* ****************************************************************************
 **                      Include Section                                     **
 *****************************************************************************/

#include <FrArTp_Int.h>           /* get module internal generic interfaces */
#include <FrArTp_SM_Int.h>        /* get module internal Encode unit interfaces */
#include <FrArTp_CfgAccess_Int.h> /* get module internal CfgAccess unit interfaces */
#include <FrArTp_SduMng_Int.h>    /* get module internal SduMng unit interfaces */
#include <PduR_FrArTp.h>


/* ****************************************************************************
 **                      Local Macros                                        **
 *****************************************************************************/



/* ****************************************************************************
**                      Variable Definitions                                 **
******************************************************************************/

/* ****************************************************************************
**                      Local Function Prototypes                            **
******************************************************************************/

/*
 * Start code section declaration
 */
#define FRARTP_START_SEC_CODE
#include <FrArTp_MemMap.h>

/* ****************************************************************************
**                      Local Function Definitions                           **
******************************************************************************/

/* ****************************************************************************
**                      Function Definitions                                 **
******************************************************************************/

/* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00149,1 */
FUNC(Std_ReturnType,FRARTP_CODE) FrArTp_Transmit
(
  VAR(PduIdType,AUTOMATIC) FrArTpTxSduId,
  P2CONST(PduInfoType,AUTOMATIC,FRARTP_APPL_DATA) FrArTpTxSduInfoPtr
)
{
  /* variable holding function status code */
  Std_ReturnType RetCode = E_NOT_OK;

  DBG_FRARTP_TRANSMIT_ENTRY(FrArTpTxSduId, FrArTpTxSduInfoPtr);

/* check if development error detection is enabled */
#if(FRARTP_DEV_ERROR_DETECT == STD_ON)
  /* check for successfully initialized module */
  /* Report to DET and return Error in case module was not initialized before */
  /* !LINKSTO FrArTp.EB.ASR42.SWS_FrArTp_00149.E_UNINIT,1 */
  if(FrArTp_InitStatus == FALSE)
  {
    FRARTP_REPORT_ERROR(FRARTP_TRANSMIT_SERVICE_ID, FRARTP_E_UNINIT);
  }
  /* check whether that LPduIdx is supported */
  /* !LINKSTO FrArTp.EB.ASR42.SWS_FrArTp_00149.E_INVALID_PDU_SDU_ID,1 */
  else if(FrArTpTxSduId >= FrArTp_CfgAccess_getNumTxSdus())
  {
    /* passed an invalid FrArTpTxPduId, report to DET */
    FRARTP_REPORT_ERROR(FRARTP_TRANSMIT_SERVICE_ID, FRARTP_E_INVALID_PDU_SDU_ID);
  }
  /* check whether parameter is a NULL_PTR */
  /* !LINKSTO FrArTp.EB.ASR42.SWS_FrArTp_00149.E_PARAM_POINTER,1 */
  else if(FrArTpTxSduInfoPtr == NULL_PTR)
  {
    /* found a NULL_PTR, report to DET */
    FRARTP_REPORT_ERROR(FRARTP_TRANSMIT_SERVICE_ID, FRARTP_E_PARAM_POINTER);
  }
  else
#endif /* FRARTP_DEV_ERROR_DETECT */
  {
    const FrArTp_ConnIdxType connIdx = FrArTp_CfgAccess_getConnectionIdx_byTxSduId(FrArTpTxSduId);
    CONSTP2CONST(FrArTp_ChannelCfgType,AUTOMATIC,FRARTP_APPL_CONST)
      pChannelCfg = FrArTp_CfgAccess_getChannelCfgPtr_byConnIdx(connIdx);
    uint8 maxSfPayloadLength;
    uint8 frameType;

    if(pChannelCfg->format != FRARTP_FRARTPLM_L4G)
    {
      maxSfPayloadLength =
        pChannelCfg->payloadLength - ((2U*pChannelCfg->addrSize) + FRARTP_PCI_SF_LENGTH);
    }
    else
    {
      /* SF-E if L4G is configured */
      maxSfPayloadLength =
        pChannelCfg->payloadLength - ((2U*pChannelCfg->addrSize) + FRARTP_PCI_SF_E_LENGTH);
    }

    /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00091.1,1,
                FrArTp.ASR42.SWS_FrArTp_00091.3,1,
                FrArTp.ASR42.SWS_FrArTp_00034.1,1,
                FrArTp.ASR42.SWS_FrArTp_00034.3,1,
                FrArTp.ASR42.SWS_FrArTp_00025.3,1 */
    if(FrArTpTxSduInfoPtr->SduLength > maxSfPayloadLength)
    {
      CONSTP2CONST(FrArTp_ConnectionCfgType,AUTOMATIC,FRARTP_APPL_CONST)
        pConnectionCfg = FrArTp_CfgAccess_getConnectionCfgPtr(connIdx);

      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00091.6,1 */
      if(!pConnectionCfg->isMultRec)
      {
        frameType = FRARTP_SM_FT_FIRST_FRAME;
        RetCode = E_OK;
      }
    }
    /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00091.2,1,
                FrArTp.ASR42.SWS_FrArTp_00091.4,1 */
    else
    {
      frameType = FRARTP_SM_FT_SINGLE_FRAME;
      RetCode = E_OK;
    }

    if(RetCode == E_OK)
    {
      /* Deviation TASKING-1 */
      RetCode = FrArTp_SM_transmit(connIdx, FrArTpTxSduInfoPtr->SduLength, frameType);
    }
  }

  /* return status value */
  DBG_FRARTP_TRANSMIT_EXIT(RetCode, FrArTpTxSduId, FrArTpTxSduInfoPtr);
  return RetCode;
}

#if FRARTP_CANCEL_API == STD_ON

/* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00150,1 */
FUNC(Std_ReturnType,FRARTP_CODE) FrArTp_CancelTransmit
(
    VAR(PduIdType,AUTOMATIC) FrArTpTxSduId
)
{
  /* variable holding function status code */
  Std_ReturnType RetCode = E_NOT_OK;

  DBG_FRARTP_CANCELTRANSMIT_ENTRY(FrArTpTxSduId);

/* check if development error detection is enabled */
#if(FRARTP_DEV_ERROR_DETECT == STD_ON)

  /* check for successfully initialized module */
  /* Report to DET and return Error in case module was not initialized before */
  /* !LINKSTO FrArTp.EB.ASR42.SWS_FrArTp_00150.E_UNINIT,1 */
  if(FrArTp_InitStatus == FALSE)
  {
    FRARTP_REPORT_ERROR(FRARTP_CANCELTRANSMIT_SERVICE_ID, FRARTP_E_UNINIT);
  }
  /* check whether that LPduIdx is supported */
  /* !LINKSTO FrArTp.EB.ASR42.SWS_FrArTp_00150.E_INVALID_PDU_SDU_ID,1 */
  else if(FrArTpTxSduId >= FrArTp_CfgAccess_getNumTxSdus())
  {
    /* passed an invalid FrArTpTxPduId, report to DET */
    FRARTP_REPORT_ERROR(FRARTP_CANCELTRANSMIT_SERVICE_ID, FRARTP_E_INVALID_PDU_SDU_ID);
  }
  else
#endif /* FRARTP_DEV_ERROR_DETECT */
  {
    const FrArTp_ConnIdxType connIdx = FrArTp_CfgAccess_getConnectionIdx_byTxSduId(FrArTpTxSduId);
    RetCode = FrArTp_SM_cancelTx(connIdx);
  }

  DBG_FRARTP_CANCELTRANSMIT_EXIT(RetCode, FrArTpTxSduId);
  return RetCode;
}

/* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00229,1,
            FrArTp.ASR42.SWS_FrArTp_00226,1 */
FUNC(Std_ReturnType,FRARTP_CODE) FrArTp_CancelReceive
(
    VAR(PduIdType,AUTOMATIC) FrArTpRxSduId
)
{
  /* variable holding function status code */
  Std_ReturnType RetCode = E_NOT_OK;

  DBG_FRARTP_CANCELRECEIVE_ENTRY(FrArTpRxSduId);

/* check if development error detection is enabled */
#if (FRARTP_DEV_ERROR_DETECT == STD_ON)

  /* check for successfully initialized module */
  /* Report to DET and return Error in case module was not initialized before */
  /* !LINKSTO FrArTp.EB.ASR42.SWS_FrArTp_00229.E_UNINIT,1 */
  if (FrArTp_InitStatus == FALSE)
  {
    FRARTP_REPORT_ERROR(FRARTP_CANCELRECEIVE_SERVICE_ID, FRARTP_E_UNINIT);
  }
  /* check validity of FrArTpTxPduId */
  /* !LINKSTO FrArTp.EB.ASR42.SWS_FrArTp_00229.E_INVALID_PDU_SDU_ID,1 */
  else if(FrArTpRxSduId >= FrArTp_CfgAccess_getNumRxSdus())
  {
    FRARTP_REPORT_ERROR(FRARTP_CANCELRECEIVE_SERVICE_ID, FRARTP_E_INVALID_PDU_SDU_ID);
  }
  else
#endif /* FRARTP_DEV_ERROR_DETECT */
  {
    const FrArTp_ConnIdxType connIdx = FrArTp_CfgAccess_getConnectionIdx_byRxSduId(FrArTpRxSduId);
    RetCode = FrArTp_SM_cancelRx(connIdx);
  }

  DBG_FRARTP_CANCELRECEIVE_EXIT(RetCode,FrArTpRxSduId);
  return RetCode;
}

#endif /* FRARTP_CANCEL_API == STD_ON */


TS_MOD_PRIV_DEFN FUNC(BufReq_ReturnType, FRARTP_CODE) FrArTp_SduMng_startOfReception
(
  FrArTp_ConnIdxType connIdx,
  PduLengthType TpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, FRARTP_APPL_DATA) BufferSizePtr
)
{
  CONSTP2CONST(FrArTp_ConnectionCfgType, AUTOMATIC, FRARTP_APPL_CONST)
    pConnectionCfg = FrArTp_CfgAccess_getConnectionCfgPtr(connIdx);

  /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00219.PduR_FrArTpStartOfReception,1 */
  return PduR_FrArTpStartOfReception(pConnectionCfg->pduRRxPduId, TpSduLength, BufferSizePtr);
}

TS_MOD_PRIV_DEFN FUNC(BufReq_ReturnType, FRARTP_CODE) FrArTp_SduMng_copyRxData
(
  FrArTp_ConnIdxType connIdx,
  P2CONST(PduInfoType, AUTOMATIC, FRARTP_APPL_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, FRARTP_APPL_DATA) BufferSizePtr
)
{
  CONSTP2CONST(FrArTp_ConnectionCfgType,AUTOMATIC,FRARTP_APPL_CONST) pConnectionCfg
    = FrArTp_CfgAccess_getConnectionCfgPtr(connIdx);

  /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00108,1,
              FrArTp.ASR42.SWS_FrArTp_00114,1,
              FrArTp.ASR42.SWS_FrArTp_00219.PduR_FrArTpCopyRxData,1 */
  return PduR_FrArTpCopyRxData(pConnectionCfg->pduRRxPduId, PduInfoPtr, BufferSizePtr);
}


TS_MOD_PRIV_DEFN FUNC(void, FRARTP_CODE) FrArTp_SduMng_rxIndication
(
  FrArTp_ConnIdxType connIdx,
  NotifResultType Result
)
{
  CONSTP2CONST(FrArTp_ConnectionCfgType,AUTOMATIC,FRARTP_APPL_CONST) pConnectionCfg
    = FrArTp_CfgAccess_getConnectionCfgPtr(connIdx);

  /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00219.PduR_FrArTpRxIndication,1 */
  PduR_FrArTpRxIndication(pConnectionCfg->pduRRxPduId, Result);
}


TS_MOD_PRIV_DEFN FUNC(BufReq_ReturnType, FRARTP_CODE) FrArTp_SduMng_copyTxData
(
  FrArTp_ConnIdxType connIdx,
  P2VAR(PduInfoType, AUTOMATIC, FRARTP_APPL_DATA) PduInfoPtr,
  P2VAR(RetryInfoType, AUTOMATIC, FRARTP_APPL_DATA) RetryInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, FRARTP_APPL_DATA) AvailableDataPtr
)
{
  CONSTP2CONST(FrArTp_ConnectionCfgType, AUTOMATIC, FRARTP_APPL_CONST)
    pConnectionCfg = FrArTp_CfgAccess_getConnectionCfgPtr(connIdx);

  /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00219.PduR_FrArTpCopyTxData,1 */
  return PduR_FrArTpCopyTxData(pConnectionCfg->pduRTxPduId, PduInfoPtr, RetryInfoPtr,
                               AvailableDataPtr);
}

TS_MOD_PRIV_DEFN FUNC(void, FRARTP_CODE) FrArTp_SduMng_txConfirmation
(
  FrArTp_ConnIdxType connIdx,
  NotifResultType Result
)
{
  CONSTP2CONST(FrArTp_ConnectionCfgType,AUTOMATIC,FRARTP_APPL_CONST) pConnectionCfg
    = FrArTp_CfgAccess_getConnectionCfgPtr(connIdx);

  /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00219.PduR_FrArTpTxConfirmation,1 */
  PduR_FrArTpTxConfirmation(pConnectionCfg->pduRTxPduId, Result);
}

/* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00151,1 */
FUNC(Std_ReturnType,FRARTP_CODE) FrArTp_ChangeParameter
(
  VAR(PduIdType,AUTOMATIC) id,
  VAR(TPParameterType,AUTOMATIC) parameter,
  VAR(uint16,AUTOMATIC) value
)
{
  /* variable holding function status code */
  Std_ReturnType RetCode = E_NOT_OK;

  DBG_FRARTP_CHANGEPARAMETER_ENTRY(id, parameter, value);

/* check if development error detection is enabled */
#if(FRARTP_DEV_ERROR_DETECT == STD_ON)

  /* check for successfully initialized module */
  /* Report to DET and return Error in case module was not initialized before */
  /* !LINKSTO FrArTp.EB.ASR42.SWS_FrArTp_00151.E_UNINIT,1 */
  if(FrArTp_InitStatus == FALSE)
  {
    FRARTP_REPORT_ERROR(FRARTP_CHANGEPARAMETER_SERVICE_ID, FRARTP_E_UNINIT);
  }
  /* !LINKSTO FrArTp.EB.ASR42.SWS_FrArTp_00151.E_INVALID_PDU_SDU_ID,1 */
  else if(id >= FrArTp_CfgAccess_getNumRxSdus())
  {
    /* passed an invalid id, report to DET */
    FRARTP_REPORT_ERROR(FRARTP_CHANGEPARAMETER_SERVICE_ID, FRARTP_E_INVALID_PDU_SDU_ID);
  }
  /* !LINKSTO FrArTp.EB.FrArTp_ChangeParameter.E_INVALID_PARAMETER.3,1 */
  else if((parameter != TP_STMIN) && (parameter != TP_BS))
  {
    FRARTP_REPORT_ERROR(FRARTP_CHANGEPARAMETER_SERVICE_ID, FRARTP_E_INVALID_PARAMETER);
  }
  /* !LINKSTO FrArTp.EB.FrArTp_ChangeParameter.E_INVALID_PARAMETER.2,1 */
  else if(value > FRARTP_GENERAL_PARAMETER_MAX_VALUE)
  {
    FRARTP_REPORT_ERROR(FRARTP_CHANGEPARAMETER_SERVICE_ID, FRARTP_E_INVALID_PARAMETER);
  }
  /* !LINKSTO FrArTp.EB.FrArTp_ChangeParameter.E_INVALID_PARAMETER.1,1 */
  else if
  (
    (parameter == TP_STMIN) &&
    (
      (value >= FRARTP_INVALID_STMIN_VALUES_0XFA) ||
      ((value >= FRARTP_INVALID_STMIN_VALUES_0X80) && (value <= FRARTP_INVALID_STMIN_VALUES_0XF0))
    )
  )
  {
    FRARTP_REPORT_ERROR(FRARTP_CHANGEPARAMETER_SERVICE_ID, FRARTP_E_INVALID_PARAMETER);
  }

  else
#endif /* FRARTP_DEV_ERROR_DETECT */
  {
    const FrArTp_ConnIdxType connIdx = FrArTp_CfgAccess_getConnectionIdx_byRxSduId(id);
    RetCode = FrArTp_SM_changeParameter(connIdx, parameter, (uint8)(value & 0xFFU));
  }
  DBG_FRARTP_CHANGEPARAMETER_EXIT(RetCode, id, parameter, value);
  return RetCode;
}

/*
 * Stop code section declaration
 */
#define FRARTP_STOP_SEC_CODE
#include <FrArTp_MemMap.h>
