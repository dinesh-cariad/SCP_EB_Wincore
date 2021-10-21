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


/* ****************************************************************************
 **                      Include Section                                     **
 *****************************************************************************/

#include <FrArTp_Int.h>           /* get module internal generic interfaces */
#include <FrArTp_SM_Int.h>        /* get module internal SM unit interfaces */
#include <FrArTp_Encode_Int.h>    /* get module internal SM unit interfaces */
#include <FrArTp_CfgAccess_Int.h> /* get module internal CfgAccess unit interfaces */
#include <FrIf.h>                 /* get FrIf interface */

/* ****************************************************************************
 **                      Local Macros                                        **
 *****************************************************************************/

#define FRARTP_ENCODE_RECEIVE   0x00U
#define FRARTP_ENCODE_TRANSMIT  0x01U

/* ****************************************************************************
**                      Variable Definitions                                 **
******************************************************************************/

/** \brief Describes the state of a single N-PDU */
typedef struct
{
  FrArTp_ActConnIdxType actConnIdx; /**< active connection index of the connection using this PDU */
  uint8 direction; /**< direction (Tx or Rx) of this PDU */
} FrArTp_Encode_TxPduStateType;

/** \brief Describes the administration of transmissions */
typedef struct
{
  /** \brief Saves the active connection index of the last scheduled connection for each TX PDU pool. */
  FrArTp_ActConnIdxType currActConnIdx[FRARTP_ACTCONN_MAX];
  /** \brief describes the current state of all TX PDUs */
  FrArTp_Encode_TxPduStateType txPdu[FRARTP_TXPDUS_MAX];
} FrArTp_Encode_TxAdminDataType;

/** \brief Describes a transmission request of an active connection */
typedef struct
{
  /** \brief active connection index of the connection requesting the transmission */
  FrArTp_ActConnIdxType actConnIdx;
  uint8 DataCount; /**< number of data frames to be transmitted */
  uint8 FCCount; /**< number of flow control frames to be transmitted */
  uint8 payloadLength; /**< payload length supported by the channel the request is using */
} FrArTp_Encode_TxRequestType;

#define FRARTP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <FrArTp_MemMap.h>

/** \brief Construct to manage the transmission of data */
STATIC VAR(FrArTp_Encode_TxAdminDataType,FRARTP_VAR) FrArTp_Encode_TxAdminData;

#define FRARTP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <FrArTp_MemMap.h>


#if FRARTP_LOWLEVELROUTING_ENABLE == STD_ON

#define FRARTP_START_SEC_CONST_UNSPECIFIED
#include <FrArTp_MemMap.h>

typedef P2FUNC(Std_ReturnType, FRARTP_APPL_CODE, FrArTp_TriggerTransmitFctPtrType)
(
  PduIdType FrTxPduId,
  P2VAR(PduInfoType, AUTOMATIC, FRARTP_APPL_DATA) PduInfoPtr,
  P2VAR(Std_ReturnType, AUTOMATIC, FRARTP_APPL_DATA) FrIfTriggerTransmitStatus
);

typedef P2FUNC(Std_ReturnType, FRARTP_APPL_CODE, FrArTp_TxConfirmationFctPtrType)(PduIdType FrTxPduId);

typedef P2FUNC(Std_ReturnType, FRARTP_APPL_CODE, FrArTp_TransmitFctPtrType)
(
  PduIdType FrIf_TxPduId,
  P2CONST(PduInfoType, AUTOMATIC, FRARTP_APPL_CONST) PduInfoPtr
);

STATIC CONST(FrArTp_TriggerTransmitFctPtrType,AUTOMATIC) FrArTp_TriggerTransmitFctPtr =
    &FRARTP_LLR_FCTNAME(FRARTP_LOWLEVELROUTING_PRAEFIX,LLR_FrIf_FrTp_TriggerTransmit);
STATIC CONST(FrArTp_TxConfirmationFctPtrType,AUTOMATIC) FrArTp_TxConfirmationFctPtr =
    &FRARTP_LLR_FCTNAME(FRARTP_LOWLEVELROUTING_PRAEFIX,LLR_FrIf_FrTp_TxConfirmation);
STATIC CONST(FrArTp_TransmitFctPtrType,AUTOMATIC) FrArTp_TransmitFctPtr =
    &FRARTP_LLR_FCTNAME(FRARTP_LOWLEVELROUTING_PRAEFIX,LLR_FrTp_FrIf_Transmit);

#define FRARTP_STOP_SEC_CONST_UNSPECIFIED
#include <FrArTp_MemMap.h>

#endif /* FRARTP_LOWLEVELROUTING_ENABLE == STD_ON */

/*
 * Start code section declaration
 */
#define FRARTP_START_SEC_CODE
#include <FrArTp_MemMap.h>

/* ****************************************************************************
**                      Local Function Prototypes                            **
******************************************************************************/

STATIC FUNC(void, FRARTP_CODE) FrArTp_Encode_WriteAddress
(
    FrArTp_ConnIdxType connIdx,
    P2VAR(uint8,AUTOMATIC,FRARTP_APPL_DATA) pAddr
);

STATIC FUNC(Std_ReturnType,FRARTP_CODE) FrArTp_Encode_EncodeDataFrame
(
    FrArTp_ActConnIdxType actConnIdx,
    P2VAR(PduInfoType,AUTOMATIC,FRARTP_APPL_DATA) PduInfoPtr
);

STATIC FUNC(Std_ReturnType,FRARTP_CODE) FrArTp_Encode_EncodeFCFrame
(
    FrArTp_ActConnIdxType actConnIdx,
    P2VAR(PduInfoType,AUTOMATIC,FRARTP_APPL_DATA) PduInfoPtr
);

STATIC FUNC(FrArTp_TxPduIdxType, FRARTP_CODE) FrArTp_Encode_DetermineTxRequests
(
  FrArTp_TxPoolIdxType iTxPool,
  P2CONST(FrArTp_TxPoolCfgType, AUTOMATIC, FRARTP_APPL_CONST) txPoolCfgPtr,
  P2VAR(FrArTp_Encode_TxRequestType, AUTOMATIC, FRARTP_APPL_DATA) txRequestsPtr,
  P2VAR(FrArTp_ActConnIdxType, AUTOMATIC, FRARTP_APPL_DATA) numActConnPtr
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_Encode_SendTxRequests
(
  FrArTp_TxPduIdxType numTxRequests,
  P2CONST(FrArTp_TxPoolCfgType, AUTOMATIC, FRARTP_APPL_CONST) txPoolCfgPtr,
  P2VAR(FrArTp_Encode_TxRequestType, AUTOMATIC, FRARTP_APPL_DATA) txRequestsPtr,
  FrArTp_ActConnIdxType numActConn
);



/* ****************************************************************************
**                      Local Function Definitions                           **
******************************************************************************/

STATIC FUNC(void, FRARTP_CODE) FrArTp_Encode_WriteAddress
(
    FrArTp_ConnIdxType connIdx,
    P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pAddr
)
{
  CONSTP2CONST(FrArTp_ConnectionCfgType, AUTOMATIC, FRARTP_APPL_CONST)
  pConnectionCfg = FrArTp_CfgAccess_getConnectionCfgPtr(connIdx);

  CONSTP2CONST(FrArTp_ChannelCfgType, AUTOMATIC, FRARTP_APPL_CONST)
  pChannelCfg = FrArTp_CfgAccess_getChannelCfgPtr_byConnIdx(connIdx);

  if(pChannelCfg->addrSize == 1U)
  {
    pAddr[0] = (uint8)(pConnectionCfg->remoteAddress & 0xFFU);
    pAddr[1] = (uint8)(pConnectionCfg->localAddress & 0xFFU);
  }
  else
  {
    pAddr[0] = (uint8)((pConnectionCfg->remoteAddress >> 8U) & 0xFFU);
    pAddr[1] = (uint8)(pConnectionCfg->remoteAddress & 0xFFU);
    pAddr[2] = (uint8)((pConnectionCfg->localAddress >> 8U) & 0xFFU);
    pAddr[3] = (uint8)(pConnectionCfg->localAddress & 0xFFU);
  }
}

STATIC FUNC(Std_ReturnType,FRARTP_CODE) FrArTp_Encode_EncodeDataFrame
(
    FrArTp_ActConnIdxType actConnIdx,
    P2VAR(PduInfoType,AUTOMATIC,FRARTP_APPL_DATA) PduInfoPtr
)
{
  Std_ReturnType result = E_NOT_OK;

  const FrArTp_ConnIdxType connIdx = FrArTp_SM_getConnIdx(actConnIdx);

  if(connIdx != FRARTP_CONN_INVALID)
  {
    CONSTP2CONST(FrArTp_ChannelCfgType, AUTOMATIC, FRARTP_APPL_CONST)
    pChannelCfg = FrArTp_CfgAccess_getChannelCfgPtr_byConnIdx(connIdx);
    const uint8 pciOffset = pChannelCfg->addrSize * 2U;
    const uint8 frameType = FrArTp_SM_getTxFrameType(actConnIdx);
    uint8 payloadOffset = pciOffset;
    uint8 pci;

    switch(frameType)
    {
      case FRARTP_SM_FT_SINGLE_FRAME:
      {
        if(pChannelCfg->format == FRARTP_FRARTPLM_L4G)
        {
          /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00029,1 */
          payloadOffset += FRARTP_PCI_SF_E_LENGTH;
          /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00030.1,1,
                      FrArTp.ASR42.SWS_FrArTp_00030.2,1 */
          pci = FRARTP_FT_EXT_SINGLE_FRAME_ID << 4U;
        }
        else
        {
          /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00023,1 */
          payloadOffset += FRARTP_PCI_SF_LENGTH;
          /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00024,1 */
          pci = FRARTP_FT_SINGLE_FRAME_ID << 4U;
        }

        break;
      }
      case FRARTP_SM_FT_FIRST_FRAME:
      {
        if(pChannelCfg->format == FRARTP_FRARTPLM_L4G)
        {
          /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00238,1 */
          payloadOffset += FRARTP_PCI_FF_E_LENGTH;
          /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00055,1,
                      FrArTp.ASR42.SWS_FrArTp_00056,1 */
          pci = FRARTP_FT_EXT_FIRST_FRAME_ID << 4U;
        }
        else
        {
          /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00237,1 */
          payloadOffset += FRARTP_PCI_FF_LENGTH;
          /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00037,1 */
          pci = FRARTP_FT_FIRST_FRAME_ID << 4U;
        }

        break;
      }
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00058.1,1,
                  FrArTp.ASR42.SWS_FrArTp_00058.2,1 */
      case FRARTP_SM_FT_CONSECUTIVE_FRAME:
      default:
      {
        /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00239,1 */
        payloadOffset += FRARTP_PCI_CF_LENGTH;
        /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00060.1,1 */
        pci = FRARTP_FT_CONSECUTIVE_FRAME_ID << 4U;
        break;
      }
    }

    {
      uint32 dataLength = 0U;
      uint8 payloadLength = pChannelCfg->payloadLength - payloadOffset;

      boolean copyResult = FALSE;

      /* data must always be larger than zero */
      if(PduInfoPtr->SduLength > (PduLengthType)payloadOffset)
      {
        PduLengthType remainingSduLength = PduInfoPtr->SduLength - (PduLengthType)payloadOffset;
        copyResult = FrArTp_SM_copyData
          (
            actConnIdx, connIdx, &dataLength, &payloadLength, &PduInfoPtr->SduDataPtr[pciOffset],
            &PduInfoPtr->SduDataPtr[payloadOffset], remainingSduLength
          );
      }

      if(copyResult)
      {
        FrArTp_Encode_WriteAddress(connIdx, PduInfoPtr->SduDataPtr);
        PduInfoPtr->SduDataPtr[pciOffset] |= pci;
        PduInfoPtr->SduLength = (PduLengthType) payloadOffset + (PduLengthType) payloadLength;
        result = E_OK;

        switch(frameType)
        {
          case FRARTP_SM_FT_SINGLE_FRAME:
          {
            if(pChannelCfg->format == FRARTP_FRARTPLM_L4G)
            {
              /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00029,1 */
              const uint8 dl = (uint8)(0xFFU & dataLength);
              PduInfoPtr->SduDataPtr[pciOffset + 1U] = dl;
            }
            else
            {
              /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00023,1 */
              const uint8 dl = (uint8)(0xFU & dataLength);
              PduInfoPtr->SduDataPtr[pciOffset] |= dl;
            }
            break;
          }
          case FRARTP_SM_FT_FIRST_FRAME:
          {
            if(pChannelCfg->format == FRARTP_FRARTPLM_L4G)
            {
              /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00238,1 */
              PduInfoPtr->SduDataPtr[pciOffset + 1U] = (uint8)((dataLength >> 24U) & 0xFFU);
              PduInfoPtr->SduDataPtr[pciOffset + 2U] = (uint8)((dataLength >> 16U) & 0xFFU);
              PduInfoPtr->SduDataPtr[pciOffset + 3U] = (uint8)((dataLength >> 8U) & 0xFFU);
              PduInfoPtr->SduDataPtr[pciOffset + 4U] = (uint8)((dataLength >> 0U) & 0xFFU);
            }
            else
            {
              /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00237,1,
                          FrArTp.ASR42.SWS_FrArTp_00299,1 */
              const uint8 dl = (uint8)((0xF00U & dataLength) >> 8U);
              PduInfoPtr->SduDataPtr[pciOffset] |= dl;
              PduInfoPtr->SduDataPtr[pciOffset + 1U] = (uint8)(dataLength & 0xFFU);
            }
            break;
          }
          case FRARTP_SM_FT_CONSECUTIVE_FRAME:
          {
            /* In case of a CF the PCI is already complete => do nothing */
            break;
          }
          /* CHECK: NOPARSE */
          default:
            FRARTP_UNREACHABLE_CODE_ASSERT(FRARTP_TRIGGERTRANSMIT_SERVICE_ID);
            break;
          /* CHECK: PARSE */
        }
      }
    }
  }
  return result;
}

STATIC FUNC(Std_ReturnType,FRARTP_CODE) FrArTp_Encode_EncodeFCFrame
(
    FrArTp_ActConnIdxType actConnIdx,
    P2VAR(PduInfoType,AUTOMATIC,FRARTP_APPL_DATA) PduInfoPtr
)
{
  Std_ReturnType result = E_NOT_OK;

  const FrArTp_ConnIdxType connIdx = FrArTp_SM_getConnIdx(actConnIdx);

  if(connIdx != FRARTP_CONN_INVALID)
  {
    CONSTP2CONST(FrArTp_ChannelCfgType, AUTOMATIC, FRARTP_APPL_CONST)
    pChannelCfg = FrArTp_CfgAccess_getChannelCfgPtr_byConnIdx(connIdx);

    const uint8 pciOffset = pChannelCfg->addrSize * 2U;

    /* If the FC frame fits into the N-PDU */
    if(PduInfoPtr->SduLength >= (((PduLengthType) pciOffset) + FRARTP_PCI_FC_LENGTH))
    {
      FrArTp_Encode_WriteAddress(connIdx, PduInfoPtr->SduDataPtr);

      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00240,1 */
      if(FrArTp_SM_copyFCParameters
          (
            actConnIdx,
            &PduInfoPtr->SduDataPtr[pciOffset],
            &PduInfoPtr->SduDataPtr[pciOffset + FRARTP_PCI_FC_OFFSET_BS],
            &PduInfoPtr->SduDataPtr[pciOffset + FRARTP_PCI_FC_OFFSET_STMIN]
          )
        )
      {
        /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00066,1 */
        PduInfoPtr->SduDataPtr[pciOffset] |= FRARTP_FT_FLOW_CONTROL_ID << 4U;
        result = E_OK;
      }
    }
  }

  return result;
}

STATIC FUNC(FrArTp_TxPduIdxType, FRARTP_CODE) FrArTp_Encode_DetermineTxRequests
(
  FrArTp_TxPoolIdxType iTxPool,
  P2CONST(FrArTp_TxPoolCfgType, AUTOMATIC, FRARTP_APPL_CONST) txPoolCfgPtr,
  P2VAR(FrArTp_Encode_TxRequestType, AUTOMATIC, FRARTP_APPL_DATA) txRequestsPtr,
  P2VAR(FrArTp_ActConnIdxType, AUTOMATIC, FRARTP_APPL_DATA) numActConnPtr
)
{
  /* read last active connection processed from the previous main function cycle */
  /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00257.2,1 */
  FrArTp_ActConnIdxType currActConnIdx = FrArTp_Encode_TxAdminData.currActConnIdx[iTxPool];
  FrArTp_ActConnIdxType lastActProcessedConnIdx = currActConnIdx;
  FrArTp_TxPduIdxType numTxRequests = 0U;
  FrArTp_ActConnIdxType i;

  /* Iterate the active connection indices */
  for(i = 0U; i < FRARTP_ACTCONN_MAX; i++)
  {
    FrArTp_ConnIdxType connIdx;

    /* forward current active connection index */
    currActConnIdx++;
    if(currActConnIdx >= FRARTP_ACTCONN_MAX)
    {
      currActConnIdx = 0U;
    }

    connIdx = FrArTp_SM_getConnIdx(currActConnIdx);

    /* If connection is active */
    if(connIdx != FRARTP_CONN_INVALID)
    {
      CONSTP2CONST(FrArTp_ChannelCfgType,AUTOMATIC,FRARTP_APPL_CONST) pChannelCfg =
          FrArTp_CfgAccess_getChannelCfgPtr_byConnIdx(connIdx);

      /* If the PDU pool of the channel the current active connection corresponds to current PDU pool */
      if(pChannelCfg->txPoolIdx == iTxPool)
      {
        /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00064.1,1 */
        const uint8 FCCount = FrArTp_SM_isFCPending(currActConnIdx);
        uint8 DataCount = 0U;
        uint8 TxCount = 0U;

        /* If at least one Tx PDU is available for data transmission */
        if((txPoolCfgPtr->numTxPdus - FCCount) != 0U)
        {
          DataCount = FrArTp_SM_isDataPending(currActConnIdx);
        }
        TxCount = FCCount + DataCount;

        if(TxCount > 0U)
        {
          txRequestsPtr[*numActConnPtr].actConnIdx = currActConnIdx;
          txRequestsPtr[*numActConnPtr].FCCount = FCCount;
          txRequestsPtr[*numActConnPtr].DataCount = DataCount;
          txRequestsPtr[*numActConnPtr].payloadLength = pChannelCfg->payloadLength;

          /* store the total number of Tx requests */
          numTxRequests += TxCount;

          lastActProcessedConnIdx = currActConnIdx;
          (*numActConnPtr)++;
        }
      }
    }
  }
  FrArTp_Encode_TxAdminData.currActConnIdx[iTxPool] = lastActProcessedConnIdx;

  return numTxRequests;
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_Encode_SendTxRequests
(
  FrArTp_TxPduIdxType numTxRequests,
  P2CONST(FrArTp_TxPoolCfgType, AUTOMATIC, FRARTP_APPL_CONST) txPoolCfgPtr,
  P2VAR(FrArTp_Encode_TxRequestType, AUTOMATIC, FRARTP_APPL_DATA) txRequestsPtr,
  FrArTp_ActConnIdxType numActConn
)
{
  /* create transmission requests for each Pdu */
  /* !LINKSTO FrArTp.EB.FrArTp_Encode.NOTRANSMISSION,1,
              FrArTp.EB.FrArTp_Encode.NOACTIVECONNECTION,1 */
  /* If there are transmission requests for this transmit PDU pool */
  if(numTxRequests > 0U)
  {
    FrArTp_ActConnIdxType iActConn = 0U;
    FrArTp_TxPduIdxType numTx = (numTxRequests > txPoolCfgPtr->numTxPdus) ? txPoolCfgPtr->numTxPdus : numTxRequests;
    const FrArTp_TxPduIdxType txPduEnd = txPoolCfgPtr->startTxPduIdx + (txPoolCfgPtr->numTxPdus - 1U);
    const FrArTp_TxPduIdxType txPduStart = (txPduEnd + 1U) - numTx;
    FrArTp_TxPduIdxType iTxPdu;

    /* Iterate all Pdus of this pool */
    /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00259,1,
                FrArTp.EB.FrArTp_Encode.TRANSMISSION_MULTI_CONN_SINGLE_CHAN,1,
                FrArTp.EB.FrArTp_Encode.TRANSMISSION_MULTI_CONN_SINGLE_CHAN_OVFL,1 */
    for(iTxPdu = txPduStart; iTxPdu <= txPduEnd; iTxPdu++)
    {
      P2CONST(FrArTp_TxPduCfgType, AUTOMATIC, FRARTP_APPL_CONST) pTxPduCfg = FrArTp_CfgAccess_getTxPduCfgPtr(iTxPdu);
      PduInfoType pduInfo;
      Std_ReturnType retVal;

      FRARTP_PRECONDITION_ASSERT
      (
          (txRequestsPtr[iActConn].FCCount  + txRequestsPtr[iActConn].DataCount) > 0,
          FRARTP_MAINFUNCTION_SERVICE_ID
      );

      pduInfo.SduLength = txRequestsPtr[iActConn].payloadLength;
      pduInfo.SduDataPtr = NULL_PTR;

      /* !LINKSTO FrArTp.EB.FrArTp_Encode.FCDATATRANSMISSION,1 */
#if FRARTP_LOWLEVELROUTING_ENABLE == STD_ON
      retVal = (*FrArTp_TransmitFctPtr)(pTxPduCfg->frIfTxPduId,&pduInfo);
      if(retVal != E_NOT_OK)
#endif /* FRARTP_LOWLEVELROUTING_ENABLE == STD_ON */
      {
        /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00219.FrIf_Transmit,1,
                    FrArTp.EB.FrArTp_Encode.FCTRANSMISSION,1,
                    FrArTp.EB.FrArTp_Encode.DATATRANSMISSION,1 */
        retVal = FrIf_Transmit(pTxPduCfg->frIfTxPduId,&pduInfo);
      }

      if(retVal == E_OK)
      {
        FrArTp_Encode_TxAdminData.txPdu[iTxPdu].actConnIdx = txRequestsPtr[iActConn].actConnIdx;

        if(txRequestsPtr[iActConn].FCCount > 0U)
        {
          FrArTp_Encode_TxAdminData.txPdu[iTxPdu].direction = FRARTP_ENCODE_RECEIVE;
          txRequestsPtr[iActConn].FCCount--;
        }
        else
        {
          FrArTp_Encode_TxAdminData.txPdu[iTxPdu].direction = FRARTP_ENCODE_TRANSMIT;
          txRequestsPtr[iActConn].DataCount--;
        }
      }
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00302,1 */
      else
      {
        const FrArTp_ConnIdxType connIdx = FrArTp_SM_getConnIdx(txRequestsPtr[iActConn].actConnIdx);
        if(txRequestsPtr[iActConn].FCCount > 0U)
        {
          (void) FrArTp_SM_cancelRx(connIdx);
          txRequestsPtr[iActConn].FCCount--;
        }
        else
        {
          (void) FrArTp_SM_cancelTx(connIdx);
          txRequestsPtr[iActConn].DataCount--;
        }

        /* Transmission of the last Pdu failed! Cancel all Pdus, otherwise we won't get a confirmation! */
        if(iTxPdu == txPduEnd)
        {
          FrArTp_TxPduIdxType cancelTxPdu;
          for(cancelTxPdu = txPduStart; cancelTxPdu <= txPduEnd; cancelTxPdu++)
          {
            FrArTp_Encode_TxAdminData.txPdu[cancelTxPdu].actConnIdx = FRARTP_ACTCONN_INVALID;
          }
        }
      }

      iActConn++;
      if(iActConn >= numActConn)
      {
        iActConn = 0U;
      }
    }
  }
}

/* ****************************************************************************
**                      Function Definitions                                 **
******************************************************************************/

/* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00154,1 */
FUNC(Std_ReturnType,FRARTP_CODE) FrArTp_TriggerTransmit
(
    PduIdType TxPduId,
    P2VAR(PduInfoType,AUTOMATIC,FRARTP_APPL_DATA) PduInfoPtr
)
{
  Std_ReturnType result;

  DBG_FRARTP_TRIGGERTRANSMIT_ENTRY(TxPduId, PduInfoPtr);

#if FRARTP_LOWLEVELROUTING_ENABLE == STD_ON
  /* !LINKSTO FrArTp.VAG.AUDI_ASR_FrTP_044,1,
              FrArTp.VAG.AUDI_ASR_FrTP_045,1.1,
              FrArTp.VAG.AUDI_ASR_FrTP_046,1,
              FrArTp.VAG.AUDI_ASR_FrTP_047,1,
              FrArTp.VAG.AUDI_ASR_FrTP_048,1 */
  if(FrArTp_TriggerTransmitFctPtr(TxPduId, PduInfoPtr, &result) != E_NOT_OK)
#endif /* FRARTP_LOWLEVELROUTING_ENABLE == STD_ON */
  {
    result = E_NOT_OK;

/* check if development error detection is enabled */
#if(FRARTP_DEV_ERROR_DETECT == STD_ON)

    /* check for successfully initialized module */
    /* Report to DET and return Error in case module was not initialized before */
    /* !LINKSTO FrArTp.EB.ASR42.SWS_FrArTp_00154.E_UNINIT,1 */
    if(FrArTp_InitStatus == FALSE)
    {
      FRARTP_REPORT_ERROR(FRARTP_TRIGGERTRANSMIT_SERVICE_ID, FRARTP_E_UNINIT);
    }
    /* check whether that LPduIdx is supported */
    /* !LINKSTO FrArTp.EB.ASR42.SWS_FrArTp_00154.E_INVALID_PDU_SDU_ID,1 */
    else if(!FrArTp_CfgAccess_isValid_TxPduIdx(TxPduId))
    {
      /* passed an invalid FrArTpTxPduId, report to DET */
      FRARTP_REPORT_ERROR(FRARTP_TRIGGERTRANSMIT_SERVICE_ID, FRARTP_E_INVALID_PDU_SDU_ID);
    }
    /* check whether parameter is a NULL_PTR */
    /* !LINKSTO FrArTp.EB.ASR42.SWS_FrArTp_00154.E_PARAM_POINTER,1 */
    else if((PduInfoPtr == NULL_PTR) || (PduInfoPtr->SduDataPtr == NULL_PTR))
    {
      /* found a NULL_PTR, report to DET */
      FRARTP_REPORT_ERROR(FRARTP_TRIGGERTRANSMIT_SERVICE_ID, FRARTP_E_PARAM_POINTER);
    }
    else
#endif /* FRARTP_DEV_ERROR_DETECT */
    {
      const FrArTp_ActConnIdxType actConnIdx = FrArTp_Encode_TxAdminData.txPdu[TxPduId].actConnIdx;
      if(actConnIdx != FRARTP_ACTCONN_INVALID)
      {
        /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00064.1,1,
                    FrArTp.ASR42.SWS_FrArTp_00274.2,1 */
        if(FrArTp_Encode_TxAdminData.txPdu[TxPduId].direction == FRARTP_ENCODE_RECEIVE)
        {
          result = FrArTp_Encode_EncodeFCFrame(actConnIdx, PduInfoPtr);
        }
        else
        {
          result = FrArTp_Encode_EncodeDataFrame(actConnIdx, PduInfoPtr);
        }
      }
    }
  }

  DBG_FRARTP_TRIGGERTRANSMIT_EXIT(result, TxPduId, PduInfoPtr);
  return result;
}

/* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00153,1 */
FUNC(void,FRARTP_CODE) FrArTp_TxConfirmation
(
  PduIdType TxPduId
)
{
  DBG_FRARTP_TXCONFIRMATION_ENTRY(TxPduId);

#if FRARTP_LOWLEVELROUTING_ENABLE == STD_ON
  /* !LINKSTO FrArTp.VAG.AUDI_ASR_FrTP_049,1,
              FrArTp.VAG.AUDI_ASR_FrTP_050,1.1,
              FrArTp.VAG.AUDI_ASR_FrTP_051,1,
              FrArTp.VAG.AUDI_ASR_FrTP_052,1,
              FrArTp.VAG.AUDI_ASR_FrTP_053,1 */
  if(FrArTp_TxConfirmationFctPtr(TxPduId) != E_NOT_OK)
#endif /* FRARTP_LOWLEVELROUTING_ENABLE == STD_ON */
  {
/* check if development error detection is enabled */
#if(FRARTP_DEV_ERROR_DETECT == STD_ON)

    /* check for successfully initialized module */
    /* Report to DET and return Error in case module was not initialized before */
    /* !LINKSTO FrArTp.EB.ASR42.SWS_FrArTp_00153.E_UNINIT,1 */
    if(FrArTp_InitStatus == FALSE)
    {
      FRARTP_REPORT_ERROR(FRARTP_TXCONFIRMATION_SERVICE_ID, FRARTP_E_UNINIT);
    }
    /* check whether that LPduIdx is supported */
    /* !LINKSTO FrArTp.EB.ASR42.SWS_FrArTp_00153.E_INVALID_PDU_SDU_ID,1 */
    else if(!FrArTp_CfgAccess_isValid_TxPduIdx(TxPduId))
    {
      /* passed an invalid FrArTpTxPduId, report to DET */
      FRARTP_REPORT_ERROR(FRARTP_TXCONFIRMATION_SERVICE_ID, FRARTP_E_INVALID_PDU_SDU_ID);
    }
    else
#endif /* FRARTP_DEV_ERROR_DETECT */
    {
      const FrArTp_ActConnIdxType actConnIdx = FrArTp_Encode_TxAdminData.txPdu[TxPduId].actConnIdx;
      if(actConnIdx != FRARTP_ACTCONN_INVALID)
      {
        const FrArTp_ConnIdxType connIdx = FrArTp_SM_getConnIdx(actConnIdx);
        if(connIdx != FRARTP_CONN_INVALID)
        {
          CONSTP2CONST(FrArTp_ChannelCfgType, AUTOMATIC, FRARTP_APPL_CONST)
          pChannelCfg = FrArTp_CfgAccess_getChannelCfgPtr_byConnIdx(connIdx);

          CONSTP2CONST(FrArTp_TxPoolCfgType, AUTOMATIC, FRARTP_APPL_CONST)
          pTxPoolCfg = FrArTp_CfgAccess_getTxPoolCfgPtr(pChannelCfg->txPoolIdx);

          FrArTp_TxPduIdxType iTxPduIdx;
          FrArTp_TxPduIdxType endTxPduIdx = pTxPoolCfg->startTxPduIdx + (pTxPoolCfg->numTxPdus - 1U);

          /* Iterate all Tx PDUs of the pool the confirmed PDU belongs to */
          for(iTxPduIdx = pTxPoolCfg->startTxPduIdx; iTxPduIdx <= endTxPduIdx; iTxPduIdx++)
          {
            const FrArTp_ActConnIdxType pduActConnIdx =
              FrArTp_Encode_TxAdminData.txPdu[iTxPduIdx].actConnIdx;
            if(pduActConnIdx != FRARTP_ACTCONN_INVALID)
            {
              if(FrArTp_Encode_TxAdminData.txPdu[iTxPduIdx].direction == FRARTP_ENCODE_RECEIVE)
              {
                FrArTp_SM_txConfirmationFC(actConnIdx);
              }
              else /* FRARTP_ENCODE_TRANSMIT */
              {
                FrArTp_SM_txConfirmationData(actConnIdx, connIdx);
              }
              FrArTp_Encode_TxAdminData.txPdu[iTxPduIdx].actConnIdx = FRARTP_ACTCONN_INVALID;
            }
          }
        }
        /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00274.1,1 */
        else
        {
          FrArTp_Encode_TxAdminData.txPdu[TxPduId].actConnIdx = FRARTP_ACTCONN_INVALID;
        }
      }
    }
  }
  DBG_FRARTP_TXCONFIRMATION_EXIT(TxPduId);
}

/* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00257.1,1 */
TS_MOD_PRIV_DEFN FUNC(void, FRARTP_CODE) FrArTp_Encode_mainFunction
(
  void
)
{
  /* for each Tx-Pool manage separately the PDU assignment */
  FrArTp_TxPoolIdxType iTxPool;
  const FrArTp_TxPoolIdxType numTxPool = FrArTp_CfgAccess_getNumTxPools();

  /* Iterate the Tx PDU pools */
  /* !LINKSTO FrArTp.EB.FrArTp_Encode.TRANSMISSION_MULTI_CONN_MULTI_CHAN,1 */
  for(iTxPool = 0U; iTxPool < numTxPool; iTxPool++)
  {
    FrArTp_Encode_TxRequestType txRequests[FRARTP_ACTCONN_MAX];
    FrArTp_ActConnIdxType numActConn = 0U;
    FrArTp_TxPduIdxType numTxRequests = 0U;

    CONSTP2CONST(FrArTp_TxPoolCfgType,AUTOMATIC,FRARTP_APPL_CONST) pTxPoolCfg =
        FrArTp_CfgAccess_getTxPoolCfgPtr(iTxPool);

    const FrArTp_TxPduIdxType confTxPdu = pTxPoolCfg->startTxPduIdx + (pTxPoolCfg->numTxPdus - 1U);

    /* If the last PDU in the pool is marked as inactive, i.e. its last transmission has been confirmed. */
    if(FrArTp_Encode_TxAdminData.txPdu[confTxPdu].actConnIdx == FRARTP_ACTCONN_INVALID)
    {
      /* Determine the transmission requests */
      numTxRequests = FrArTp_Encode_DetermineTxRequests(iTxPool, pTxPoolCfg, txRequests, &numActConn);

      /* Send and handle the transmission requests */
      FrArTp_Encode_SendTxRequests(numTxRequests, pTxPoolCfg, txRequests, numActConn);
    }
  }
}


TS_MOD_PRIV_DEFN FUNC(void, FRARTP_CODE) FrArTp_Encode_init
(
  void
)
{
  /* Initialize current active connection indices */
  {
    FrArTp_TxPoolIdxType iActConn;
    for(iActConn = 0U; iActConn < FRARTP_ACTCONN_MAX; iActConn++)
    {
      FrArTp_Encode_TxAdminData.currActConnIdx[iActConn] = FRARTP_ACTCONN_MAX;
    }
  }

  /* Initialize tansmit PDUs */
  {
    FrArTp_TxPduIdxType iTxPdu;
    for(iTxPdu = 0U; iTxPdu < FRARTP_TXPDUS_MAX; iTxPdu++)
    {
      FrArTp_Encode_TxAdminData.txPdu[iTxPdu].actConnIdx = FRARTP_ACTCONN_INVALID;
    }
  }

}

TS_MOD_PRIV_DEFN FUNC(void, FRARTP_CODE) FrArTp_Encode_cancelTx
(
  FrArTp_ActConnIdxType actConnIdx
)
{
  /* Cancel the PDUs by setting their active connection index to invalid. FrIf does not support
  FrIf_CancelTx() so we just have to wait until FrIf calls FrArTp_TriggerTransmit(), which will
  fail and then FrIf will cancel the transmission itself. */

  const FrArTp_ConnIdxType connIdx = FrArTp_SM_getConnIdx(actConnIdx);
  CONSTP2CONST(FrArTp_ChannelCfgType, AUTOMATIC, FRARTP_APPL_CONST)
  pChannelCfg = FrArTp_CfgAccess_getChannelCfgPtr_byConnIdx(connIdx);
  CONSTP2CONST(FrArTp_TxPoolCfgType, AUTOMATIC, FRARTP_APPL_CONST)
  pTxPoolCfg = FrArTp_CfgAccess_getTxPoolCfgPtr(pChannelCfg->txPoolIdx);

  FrArTp_TxPduIdxType iTxPduIdx;
  FrArTp_TxPduIdxType endTxPduIdx = pTxPoolCfg->startTxPduIdx + (pTxPoolCfg->numTxPdus - 1U);

  /* If the last PDU in the pool has been scheduled for this connection, all PDUs need to be
  canceled, because the last one is the only one with a TxConfirmation configured. */
  if(FrArTp_Encode_TxAdminData.txPdu[endTxPduIdx].actConnIdx == actConnIdx)
  {
    /* Iterate all Tx PDUs of the pool the to be canceled PDU(s) belong(s) to */
    for(iTxPduIdx = pTxPoolCfg->startTxPduIdx; iTxPduIdx <= endTxPduIdx; iTxPduIdx++)
    {
      /* Cancel the PDU */
      FrArTp_Encode_TxAdminData.txPdu[iTxPduIdx].actConnIdx = FRARTP_ACTCONN_INVALID;
    }
  }
  else
  {
    /* Iterate all Tx PDUs of the pool the to be canceled PDU(s) belong(s) to */
    for(iTxPduIdx = pTxPoolCfg->startTxPduIdx; iTxPduIdx <= endTxPduIdx; iTxPduIdx++)
    {
      /* If the PDU has been scheduled for the connection to be canceled. */
      if(FrArTp_Encode_TxAdminData.txPdu[iTxPduIdx].actConnIdx == actConnIdx)
      {
        /* Cancel the PDU */
        FrArTp_Encode_TxAdminData.txPdu[iTxPduIdx].actConnIdx = FRARTP_ACTCONN_INVALID;
      }
    }
  }
}

/*
 * Stop code section declaration
 */
#define FRARTP_STOP_SEC_CODE
#include <FrArTp_MemMap.h>
