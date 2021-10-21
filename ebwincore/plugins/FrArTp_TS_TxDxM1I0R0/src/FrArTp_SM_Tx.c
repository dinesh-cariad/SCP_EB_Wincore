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

#include <FrArTp.h>
#include <FrArTp_SM_Int.h>        /* get module internal SM unit interfaces */
#include <FrArTp_SduMng_Int.h>    /* get module internal SduMng unit interfaces */
#include <FrArTp_CfgAccess_Int.h> /* get module internal CfgAccess unit interfaces */

/* ****************************************************************************
 **                      Local Macros                                        **
 *****************************************************************************/

#define FRARTP_SM_TX_SINGLE_FRAME                                         0x0U
#define FRARTP_SM_TX_FIRST_FRAME                                          0x1U
#define FRARTP_SM_TX_CONSECUTIVE_FRAME                                    0x2U
#define FRARTP_SM_TX_MAX_VALUE_MILLISECONDS                              0x80U

#define FrArTp_SM_Tx_tr_WaitFC_Idle(actConnIdx, connIdx) FrArTp_SM_Tx_tr_Active_Idle(actConnIdx, connIdx)

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

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_tr_Idle_WaitPdu
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx,
  PduLengthType sduLength,
  uint8 frameType
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_self_tr_WaitPdu
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_tr_WaitTx_WaitPdu
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_self_tr_WaitFC
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_entry_WaitPdu
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_tr_WaitPdu_TxPending
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_entry_TxPending
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_tr_Active_Idle
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx
);

STATIC FUNC(boolean, FRARTP_CODE) FrArTp_SM_Tx_self_tr_TxPending
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx,
  P2VAR(uint32, AUTOMATIC, FRARTP_APPL_DATA) pDataLength,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pPayloadLength,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pSequenceNumber,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pPayload,
  PduLengthType remainingSduLength
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_tr_TxPending_WaitTx
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_entry_WaitTx
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_tr_TxPending_WaitFC
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_entry_WaitFC
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_tr_TxPending_Idle
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_entry_Idle
(
  FrArTp_ActConnIdxType actConnIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_tr_WaitFC_WaitPdu
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx,
  uint8 blockSize,
  uint8 STMin
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_timeout
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_exit_TxPending
(
  FrArTp_ActConnIdxType actConnIdx
);
/* ****************************************************************************
**                      Function Definitions                                 **
******************************************************************************/

TS_MOD_PRIV_DEFN FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_mainFunction
(
  FrArTp_ActConnIdxType actConnIdx
)
{
  boolean timerIsExpired = FALSE;
  FrArTp_ConnIdxType connIdx;

  /* Enter exclusive area */
  SchM_Enter_FrArTp_SCHM_FRARTP_EXCLUSIVE_AREA_0();

  connIdx = FrArTp_SM_actConn[actConnIdx].connIdx;

  if(connIdx != FRARTP_CONN_INVALID)
  {
    /* If timer is currently running */
    if(FrArTp_SM_actConn[actConnIdx].tx.timer > 0U)
    {
      FrArTp_SM_actConn[actConnIdx].tx.timer--;

      /* If timer expired */
      if(FrArTp_SM_actConn[actConnIdx].tx.timer == 0U)
      {
        timerIsExpired = TRUE;
      }
    }
  }

  /* Exit exclusive area */
  SchM_Exit_FrArTp_SCHM_FRARTP_EXCLUSIVE_AREA_0();

  /* Handle potential timeout */
  if(timerIsExpired == TRUE)
  {
    FrArTp_SM_Tx_timeout(actConnIdx, connIdx);
  }


}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_timeout
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx
)
{
  uint8 txState;
  TS_AtomicAssign8(txState, FrArTp_SM_actConn[actConnIdx].tx.state);

  switch(txState)
  {
    case FRARTP_SM_TXSTATE_WAITTX :
    {
      /* StMin expires */
      FrArTp_SM_Tx_tr_WaitTx_WaitPdu(actConnIdx, connIdx);
      break;
    }
    /* FrArTpTimeoutAs expired */
    /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00282.1,1 */
    case FRARTP_SM_TXSTATE_TXPENDING :
    {
      FrArTp_Encode_cancelTx(actConnIdx);
      FrArTp_SM_Tx_tr_Active_Idle(actConnIdx, connIdx);
      break;
    }
    case FRARTP_SM_TXSTATE_WAITPDU : /*  Intended fall-through */
    /* FrArTpTimeoutBs expired */
    /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00264,1 */
    case FRARTP_SM_TXSTATE_WAITFC :
    {
      FrArTp_SM_Tx_tr_Active_Idle(actConnIdx, connIdx);
      break;
    }
    /* CHECK: NOPARSE */
    case FRARTP_SM_TXSTATE_IDLE: /* fall through */
    default:
    {
      FRARTP_UNREACHABLE_CODE_ASSERT(FRARTP_MAINFUNCTION_SERVICE_ID);
      break;
    }
    /* CHECK: PARSE */
  }
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, FRARTP_CODE) FrArTp_SM_cancelTx
(
  FrArTp_ConnIdxType connIdx
)
{
  Std_ReturnType retVal = E_NOT_OK;
  const FrArTp_ActConnIdxType actConnIdx = FrArTp_SM_FindActConnIdx(connIdx);

  /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00236.2,1 */
  if(actConnIdx != FRARTP_ACTCONN_INVALID)
  {
    uint8 txState;
    TS_AtomicAssign8(txState, FrArTp_SM_actConn[actConnIdx].tx.state);

    switch(txState)
    {
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00279,1,
                  FrArTp.ASR42.SWS_FrArTp_00236.3,1 */
      case FRARTP_SM_TXSTATE_IDLE : break;
      case FRARTP_SM_TXSTATE_WAITPDU :
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00273,1 */
      case FRARTP_SM_TXSTATE_TXPENDING :
      case FRARTP_SM_TXSTATE_WAITTX :
      case FRARTP_SM_TXSTATE_WAITFC :
      {
        /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00103,1 */
        FrArTp_SM_Tx_tr_Active_Idle(actConnIdx, connIdx);
        /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00236.1,1 */
        retVal = E_OK;
        break;
      }
      /* CHECK: NOPARSE */
      default:
      {
        FRARTP_UNREACHABLE_CODE_ASSERT(FRARTP_INTERNAL_API_ID);
        break;
      }
      /* CHECK: PARSE */
    }
  }

  return retVal;
}

TS_MOD_PRIV_DEFN FUNC(void, FRARTP_CODE) FrArTp_SM_receivedCTS
(
  FrArTp_ConnIdxType connIdx,
  uint8 BS,
  uint8 STMin
)
{
  const FrArTp_ActConnIdxType actConnIdx = FrArTp_SM_FindActConnIdx(connIdx);

  if(actConnIdx != FRARTP_ACTCONN_INVALID)
  {
    uint8 txState;
    TS_AtomicAssign8(txState, FrArTp_SM_actConn[actConnIdx].tx.state);

    switch (txState)
    {
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00083.14,1,
                  FrArTp.ASR42.SWS_FrArTp_00283.14,1 */
      case FRARTP_SM_TXSTATE_IDLE : break;
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00083.12,1,
                  FrArTp.ASR42.SWS_FrArTp_00283.12,1 */
      case FRARTP_SM_TXSTATE_WAITPDU : break;
      case FRARTP_SM_TXSTATE_TXPENDING : break;
      case FRARTP_SM_TXSTATE_WAITTX : break;
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00064.2,1,
                  FrArTp.ASR42.SWS_FrArTp_00083.11,1,
                  FrArTp.ASR42.SWS_FrArTp_00083.13,1,
                  FrArTp.ASR42.SWS_FrArTp_00283.11,1,
                  FrArTp.ASR42.SWS_FrArTp_00283.13,1 */
      case FRARTP_SM_TXSTATE_WAITFC :
      {
        /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00067.1,1 */
        FrArTp_SM_Tx_tr_WaitFC_WaitPdu(actConnIdx, connIdx, BS, STMin);
        break;
      }
      /* CHECK: NOPARSE */
      default:
      {
        FRARTP_UNREACHABLE_CODE_ASSERT(FRARTP_RXINDICATION_SERVICE_ID);
        break;
      }
      /* CHECK: PARSE */
    }
  }
}

TS_MOD_PRIV_DEFN FUNC(void, FRARTP_CODE) FrArTp_SM_receivedWT
(
  FrArTp_ConnIdxType connIdx
)
{
  const FrArTp_ActConnIdxType actConnIdx = FrArTp_SM_FindActConnIdx(connIdx);

  if(actConnIdx != FRARTP_ACTCONN_INVALID)
  {
    uint8 txState;
    TS_AtomicAssign8(txState, FrArTp_SM_actConn[actConnIdx].tx.state);

    switch(txState)
    {
      case FRARTP_SM_TXSTATE_IDLE : break;
      case FRARTP_SM_TXSTATE_WAITPDU : break;
      case FRARTP_SM_TXSTATE_TXPENDING : break;
      case FRARTP_SM_TXSTATE_WAITTX : break;
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00064.2,1 */
      case FRARTP_SM_TXSTATE_WAITFC :
      {
        /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00067.2,1 */
        FrArTp_SM_Tx_self_tr_WaitFC(actConnIdx, connIdx);
        break;
      }
      /* CHECK: NOPARSE */
      default:
      {
        FRARTP_UNREACHABLE_CODE_ASSERT(FRARTP_RXINDICATION_SERVICE_ID);
        break;
      }
      /* CHECK: PARSE */
    }
  }
}

TS_MOD_PRIV_DEFN FUNC(void, FRARTP_CODE) FrArTp_SM_receivedOVFLW
(
  FrArTp_ConnIdxType connIdx
)
{
  const FrArTp_ActConnIdxType actConnIdx = FrArTp_SM_FindActConnIdx(connIdx);

  if(actConnIdx != FRARTP_ACTCONN_INVALID)
  {
    uint8 txState;
    TS_AtomicAssign8(txState, FrArTp_SM_actConn[actConnIdx].tx.state);

    switch(txState)
    {
      case FRARTP_SM_TXSTATE_IDLE : break;
      case FRARTP_SM_TXSTATE_WAITPDU : break;
      case FRARTP_SM_TXSTATE_TXPENDING : break;
      case FRARTP_SM_TXSTATE_WAITTX : break;
      case FRARTP_SM_TXSTATE_WAITFC :
      {
        /* The sender shall abort the transmission, because the receiver has not
           enough buffer space for the whole message available.*/
        /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00067.4,1 */
        FrArTp_SM_Tx_tr_WaitFC_Idle(actConnIdx, connIdx);
        break;
      }
      /* CHECK: NOPARSE */
      default:
      {
        FRARTP_UNREACHABLE_CODE_ASSERT(FRARTP_RXINDICATION_SERVICE_ID);
        break;
      }
      /* CHECK: PARSE */
    }
  }
}

TS_MOD_PRIV_DEFN FUNC(void, FRARTP_CODE) FrArTp_SM_receivedInvalidFS
(
  FrArTp_ConnIdxType connIdx
)
{
  const FrArTp_ActConnIdxType actConnIdx = FrArTp_SM_FindActConnIdx(connIdx);

  if(actConnIdx != FRARTP_ACTCONN_INVALID)
  {
    uint8 txState;
    TS_AtomicAssign8(txState, FrArTp_SM_actConn[actConnIdx].tx.state);

    switch(txState)
    {
      case FRARTP_SM_TXSTATE_IDLE : break;
      case FRARTP_SM_TXSTATE_WAITPDU : break;
      case FRARTP_SM_TXSTATE_TXPENDING : break;
      case FRARTP_SM_TXSTATE_WAITTX : break;
      case FRARTP_SM_TXSTATE_WAITFC :
      {
        FrArTp_SM_Tx_tr_WaitFC_Idle(actConnIdx, connIdx);
        break;
      }
      /* CHECK: NOPARSE */
      default:
      {
        FRARTP_UNREACHABLE_CODE_ASSERT(FRARTP_RXINDICATION_SERVICE_ID);
        break;
      }
      /* CHECK: PARSE */
    }
  }
}

TS_MOD_PRIV_DEFN FUNC(uint8, FRARTP_CODE) FrArTp_SM_isDataPending
(
  FrArTp_ActConnIdxType actConnIdx
)
{
  const FrArTp_ConnIdxType connIdx = FrArTp_SM_getConnIdx(actConnIdx);

  CONSTP2CONST(FrArTp_ChannelCfgType,AUTOMATIC,FRARTP_APPL_CONST) pChannelCfg
      = FrArTp_CfgAccess_getChannelCfgPtr_byConnIdx(connIdx);
  uint32 framesToTransmit = 0U;
  uint8 txState;
  TS_AtomicAssign8(txState, FrArTp_SM_actConn[actConnIdx].tx.state);

  switch(txState)
  {
    case FRARTP_SM_TXSTATE_IDLE : break;
    case FRARTP_SM_TXSTATE_WAITPDU :
    {
      uint8 pduPayloadLength;

      /* If there is data available to transmit */
      if(FrArTp_SM_actConn[actConnIdx].tx.available > 0U)
      {
        switch(FrArTp_SM_actConn[actConnIdx].tx.frameType)
        {
          case FRARTP_SM_FT_SINGLE_FRAME:
          {
            if (pChannelCfg->format != FRARTP_FRARTPLM_L4G)
            {
              pduPayloadLength = pChannelCfg->payloadLength - ((2U*pChannelCfg->addrSize) + FRARTP_PCI_SF_LENGTH);
            }
            else
            {
              pduPayloadLength = pChannelCfg->payloadLength - ((2U*pChannelCfg->addrSize) + FRARTP_PCI_SF_E_LENGTH);
            }
            break;
          }
          case FRARTP_SM_FT_FIRST_FRAME:
          {
            if (pChannelCfg->format != FRARTP_FRARTPLM_L4G)
            {
              pduPayloadLength = pChannelCfg->payloadLength - ((2U*pChannelCfg->addrSize) + FRARTP_PCI_FF_LENGTH);
            }
            else
            {
              pduPayloadLength = pChannelCfg->payloadLength - ((2U*pChannelCfg->addrSize) + FRARTP_PCI_FF_E_LENGTH);
            }
            break;
          }
          case FRARTP_SM_FT_CONSECUTIVE_FRAME: /* fall through */
          default:
          {
            pduPayloadLength = pChannelCfg->payloadLength - ((2U*pChannelCfg->addrSize) + FRARTP_PCI_CF_LENGTH);
            break;
          }
        }

        /* If there is more data available to transmit then expected from the remaining message length */
        if(FrArTp_SM_actConn[actConnIdx].tx.available > FrArTp_SM_actConn[actConnIdx].tx.remaining)
        {
          FrArTp_SM_actConn[actConnIdx].tx.available = FrArTp_SM_actConn[actConnIdx].tx.remaining;
        }

        /* If there is less data available then could fit into the N-PDU */
        if(FrArTp_SM_actConn[actConnIdx].tx.available < pduPayloadLength)
        {
          pduPayloadLength = (uint8)FrArTp_SM_actConn[actConnIdx].tx.available;
        }

        /* Calculate the number of still to be transmitted frames */
        if ((FrArTp_SM_actConn[actConnIdx].tx.available % (uint32)pduPayloadLength) == 0U )
        {
          framesToTransmit = (FrArTp_SM_actConn[actConnIdx].tx.available/(uint32)pduPayloadLength);
        }
        else
        {
          framesToTransmit = (FrArTp_SM_actConn[actConnIdx].tx.available/(uint32)pduPayloadLength) + 1U;
        }
      }

      if(framesToTransmit == 0U)
      {
        /* Not enough data available, no frame shall be transmitted, available data checked again */
        FrArTp_SM_Tx_self_tr_WaitPdu(actConnIdx, connIdx);
      }
      else
      {
        /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00068.1,1 */
        if (FrArTp_SM_actConn[actConnIdx].tx.ctsExpected == FALSE)
        {
          framesToTransmit = (framesToTransmit > 255) ? 255U : framesToTransmit;
        }
        else
        {
          /* calculate number of frames to transmit and limit by blocksize */
          framesToTransmit = (framesToTransmit > FrArTp_SM_actConn[actConnIdx].tx.frameCount ? FrArTp_SM_actConn[actConnIdx].tx.frameCount : framesToTransmit);
        }
        FrArTp_SM_Tx_tr_WaitPdu_TxPending(actConnIdx, connIdx);
      }
      break;
    }
    case FRARTP_SM_TXSTATE_TXPENDING : break;
    case FRARTP_SM_TXSTATE_WAITTX : break;
    case FRARTP_SM_TXSTATE_WAITFC : break;
    /* CHECK: NOPARSE */
    default:
    {
      FRARTP_UNREACHABLE_CODE_ASSERT(FRARTP_MAINFUNCTION_SERVICE_ID);
      break;
    }
    /* CHECK: PARSE */
  }

  return (uint8) framesToTransmit;
}

TS_MOD_PRIV_DEFN FUNC(boolean, FRARTP_CODE) FrArTp_SM_copyData
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx,
  P2VAR(uint32, AUTOMATIC, FRARTP_APPL_DATA) pDataLength,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pPayloadLength,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pSequenceNumber,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pPayload,
  PduLengthType remainingSduLength
)
{
  boolean result = FALSE;
  uint8 txState;
  TS_AtomicAssign8(txState, FrArTp_SM_actConn[actConnIdx].tx.state);

  switch(txState)
  {
    case FRARTP_SM_TXSTATE_IDLE : break;
    case FRARTP_SM_TXSTATE_WAITPDU : break;
    case FRARTP_SM_TXSTATE_TXPENDING :
    {
      result = FrArTp_SM_Tx_self_tr_TxPending(actConnIdx, connIdx, pDataLength, pPayloadLength,
        pSequenceNumber, pPayload, remainingSduLength);
      break;
    }
    case FRARTP_SM_TXSTATE_WAITTX : break;
    case FRARTP_SM_TXSTATE_WAITFC : break;
    /* CHECK: NOPARSE */
    default:
    {
      FRARTP_UNREACHABLE_CODE_ASSERT(FRARTP_TRIGGERTRANSMIT_SERVICE_ID);
      break;
    }
    /* CHECK: PARSE */
  }

  return result;
}

TS_MOD_PRIV_DEFN FUNC(void, FRARTP_CODE) FrArTp_SM_txConfirmationData
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx
)
{
  uint8 txState;
  TS_AtomicAssign8(txState, FrArTp_SM_actConn[actConnIdx].tx.state);

  switch(txState)
  {
    case FRARTP_SM_TXSTATE_IDLE : break;
    case FRARTP_SM_TXSTATE_WAITPDU : break;
    case FRARTP_SM_TXSTATE_TXPENDING :
    {
      /* If there is still data to transmit */
      if (FrArTp_SM_actConn[actConnIdx].tx.remaining > 0U)
      {
        /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00064.2,1 */
        if
        (
          /* If there are still frames to transmit within the current block*/
          (FrArTp_SM_actConn[actConnIdx].tx.frameCount > 0U) ||
          /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00068.1,1 */
          /* If no FC CTS is expected */
          (FrArTp_SM_actConn[actConnIdx].tx.ctsExpected == FALSE)
        )
        {
          FrArTp_SM_Tx_tr_TxPending_WaitTx(actConnIdx, connIdx);
        }
        else
        {
          FrArTp_SM_Tx_tr_TxPending_WaitFC(actConnIdx, connIdx);
        }
      }
      else /* All data transmitted */
      {
        FrArTp_SM_Tx_tr_TxPending_Idle(actConnIdx, connIdx);
      }
      break;
    }
    case FRARTP_SM_TXSTATE_WAITTX : break;
    case FRARTP_SM_TXSTATE_WAITFC : break;
    /* CHECK: NOPARSE */
    default:
    {
      FRARTP_UNREACHABLE_CODE_ASSERT(FRARTP_TXCONFIRMATION_SERVICE_ID);
      break;
    }
    /* CHECK: PARSE */
  }
}

TS_MOD_PRIV_DEFN FUNC(uint8, FRARTP_CODE) FrArTp_SM_getTxFrameType
(
  FrArTp_ActConnIdxType actConnIdx
)
{
  return FrArTp_SM_actConn[actConnIdx].tx.frameType;
}


TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, FRARTP_CODE) FrArTp_SM_transmit
(
  FrArTp_ConnIdxType connIdx,
  PduLengthType sduLength,
  uint8 frameType
)
{
  Std_ReturnType retVal = E_NOT_OK;
  /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00230,1 */
  const FrArTp_ActConnIdxType actConnIdx = FrArTp_SM_FindOrAllocateActConnIdx(connIdx);

  if (actConnIdx != FRARTP_ACTCONN_INVALID)
  {
    uint8 txState;
    TS_AtomicAssign8(txState, FrArTp_SM_actConn[actConnIdx].tx.state);

    switch(txState)
    {
      case FRARTP_SM_TXSTATE_IDLE :
      {
        FrArTp_SM_Tx_tr_Idle_WaitPdu(actConnIdx, connIdx, sduLength, frameType);
        retVal = E_OK;
        break;
      }
      case FRARTP_SM_TXSTATE_WAITPDU : break;
      case FRARTP_SM_TXSTATE_TXPENDING : break;
      case FRARTP_SM_TXSTATE_WAITTX : break;
      case FRARTP_SM_TXSTATE_WAITFC : break;
      /* CHECK: NOPARSE */
      default:
      {
        FRARTP_UNREACHABLE_CODE_ASSERT(FRARTP_TRANSMIT_SERVICE_ID);
        break;
      }
      /* CHECK: PARSE */
    }
  }

  return retVal;
}

/* ****************************************************************************
**                      Local Function Definitions                           **
******************************************************************************/

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_tr_Idle_WaitPdu
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx,
  PduLengthType sduLength,
  uint8 frameType
)
{
  /* transition code */
  FrArTp_SM_actConn[actConnIdx].tx.frameType = frameType;
  /* The first block consists of a single FF/SF */
  FrArTp_SM_actConn[actConnIdx].tx.frameCount = 1U;
  FrArTp_SM_actConn[actConnIdx].tx.available = 0U;
  /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00061.1,1 */
  FrArTp_SM_actConn[actConnIdx].tx.seq = 0U;
  FrArTp_SM_actConn[actConnIdx].tx.remaining = sduLength;
  FrArTp_SM_actConn[actConnIdx].tx.stMinTicks = 0U;
  /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00064.2,1 */
  FrArTp_SM_actConn[actConnIdx].tx.ctsExpected = TRUE;

  /* entry code */
  FrArTp_SM_Tx_entry_WaitPdu(actConnIdx, connIdx);
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_entry_WaitPdu
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx
)
{
  CONSTP2CONST(FrArTp_ChannelCfgType,AUTOMATIC,FRARTP_APPL_CONST) pChannelCfg =
      FrArTp_CfgAccess_getChannelCfgPtr_byConnIdx(connIdx);

  /* Enter exclusive area */
  SchM_Enter_FrArTp_SCHM_FRARTP_EXCLUSIVE_AREA_0();

  FrArTp_SM_Tx_StartTimer(actConnIdx,pChannelCfg->timeCs);

  /* Set state variable */
  TS_AtomicAssign8(FrArTp_SM_actConn[actConnIdx].tx.state, FRARTP_SM_TXSTATE_WAITPDU);

  /* Exit exclusive area */
  SchM_Exit_FrArTp_SCHM_FRARTP_EXCLUSIVE_AREA_0();
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_self_tr_WaitPdu
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx
)
{
  PduInfoType PduInfo;
  PduLengthType available;
  BufReq_ReturnType retBuffer;

  PduInfo.SduDataPtr = NULL_PTR;
  PduInfo.SduLength = 0U;

  /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00232.1,1 */
  retBuffer = FrArTp_SduMng_copyTxData(connIdx,&PduInfo,NULL_PTR,&available);

  if(retBuffer == BUFREQ_OK)
  {
    FrArTp_SM_actConn[actConnIdx].tx.available = available;
  }
  else if(retBuffer == BUFREQ_E_BUSY)
  {
    FrArTp_SM_actConn[actConnIdx].tx.available = 0U;
  }
  /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00107.3,1,
              FrArTp.ASR42.SWS_FrArTp_00293.2,1 */
  else
  {
    FrArTp_SM_Tx_tr_Active_Idle(actConnIdx, connIdx);
  }
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_self_tr_WaitFC
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx
)
{
  CONSTP2CONST(FrArTp_ChannelCfgType,AUTOMATIC,FRARTP_APPL_CONST) pChannelCfg =
      FrArTp_CfgAccess_getChannelCfgPtr_byConnIdx(connIdx);

  /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00067.2,1 */
  FrArTp_SM_Tx_StartTimer(actConnIdx,pChannelCfg->timeoutBs);
}


STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_tr_WaitPdu_TxPending
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx
)
{
  /* exit code */

  /* transition code */

  /* entry code */
  FrArTp_SM_Tx_entry_TxPending(actConnIdx, connIdx);
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_entry_TxPending
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx
)
{
  CONSTP2CONST(FrArTp_ChannelCfgType,AUTOMATIC,FRARTP_APPL_CONST) pChannelCfg =
      FrArTp_CfgAccess_getChannelCfgPtr_byConnIdx(connIdx);

  /* Enter exclusive area */
  SchM_Enter_FrArTp_SCHM_FRARTP_EXCLUSIVE_AREA_0();

  /* Start the timer */
  FrArTp_SM_Tx_StartTimer(actConnIdx,pChannelCfg->timeoutAs);

  /* Set state variable */
  TS_AtomicAssign8(FrArTp_SM_actConn[actConnIdx].tx.state, FRARTP_SM_TXSTATE_TXPENDING);

  /* Exit exclusive area */
  SchM_Exit_FrArTp_SCHM_FRARTP_EXCLUSIVE_AREA_0();
}

STATIC FUNC(boolean, FRARTP_CODE) FrArTp_SM_Tx_self_tr_TxPending
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx,
  P2VAR(uint32, AUTOMATIC, FRARTP_APPL_DATA) pDataLength,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pPayloadLength,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pSequenceNumber,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pPayload,
  PduLengthType remainingSduLength
)
{
  boolean result = FALSE;

  /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00064.2,1 */
  if ((FrArTp_SM_actConn[actConnIdx].tx.frameCount > 0U)
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00068.1,1 */
      || (FrArTp_SM_actConn[actConnIdx].tx.ctsExpected == FALSE))
  {
    if(FrArTp_SM_actConn[actConnIdx].tx.frameType != FRARTP_SM_FT_CONSECUTIVE_FRAME)
    {
      *pDataLength = FrArTp_SM_actConn[actConnIdx].tx.remaining;
    }

    /* update current payload length */
    if (FrArTp_SM_actConn[actConnIdx].tx.remaining < *pPayloadLength)
    {
      *pPayloadLength = (uint8)FrArTp_SM_actConn[actConnIdx].tx.remaining;
    }

    /* check if updated payload can fit in tx pdu */
    if((PduLengthType)(*pPayloadLength) <= remainingSduLength)
    {
      PduInfoType pduInfo;
      BufReq_ReturnType retBuffer;
      PduLengthType available;

      pduInfo.SduDataPtr = pPayload;
      pduInfo.SduLength = *pPayloadLength;

      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00106.1,1,
                  FrArTp.ASR42.SWS_FrArTp_00110,1,
                  FrArTp.ASR42.SWS_FrArTp_00232.1,1,
                  FrArTp.ASR42.SWS_FrArTp_00230,1 */
      retBuffer = FrArTp_SduMng_copyTxData(connIdx, &pduInfo, NULL_PTR, &available);

      if(retBuffer == BUFREQ_OK)
      {
        FrArTp_SM_actConn[actConnIdx].tx.frameType = FRARTP_SM_FT_CONSECUTIVE_FRAME;
        FrArTp_SM_actConn[actConnIdx].tx.available = available;
        FrArTp_SM_actConn[actConnIdx].tx.remaining = FrArTp_SM_actConn[actConnIdx].tx.remaining - *pPayloadLength;

        /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00068.1,1 */
        if (FrArTp_SM_actConn[actConnIdx].tx.ctsExpected != FALSE)
        {
          FrArTp_SM_actConn[actConnIdx].tx.frameCount--;
        }

        *pSequenceNumber = FrArTp_SM_actConn[actConnIdx].tx.seq;

        /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00061.1,1 */
        FrArTp_SM_actConn[actConnIdx].tx.seq++;
        /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00061.2,1 */
        /* FRARTP_MAX_SEQUENCE_VALUE reached then reset sequence number */
        if (FrArTp_SM_actConn[actConnIdx].tx.seq > FRARTP_MAX_SEQUENCE_VALUE)
        {
          FrArTp_SM_actConn[actConnIdx].tx.seq = 0U;
        }

        result = TRUE;
      }
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00107.1,1,
                  FrArTp.ASR42.SWS_FrArTp_00296,1 */
      else if(retBuffer == BUFREQ_E_BUSY)
      {
        /* tx.state shall not change, causing retry of copy tx data */
        FrArTp_SM_actConn[actConnIdx].tx.available = 0U;
      }
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00107.3,1,
                  FrArTp.ASR42.SWS_FrArTp_00293.2,1 */
      else
      {
        FrArTp_SM_Tx_tr_Active_Idle(actConnIdx, connIdx);
      }
    }
  }
  return result;
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_tr_TxPending_WaitTx
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx
)
{
  /* exit code */
  FrArTp_SM_Tx_exit_TxPending(actConnIdx);

  /* transition code */

  /* entry code */
  FrArTp_SM_Tx_entry_WaitTx(actConnIdx, connIdx);
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_entry_WaitTx
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx
)
{
  /* If there is a non-zero minimum separation time between CFs */
  if (FrArTp_SM_actConn[actConnIdx].tx.stMinTicks > 0U)
  {
    /* Start the timer */
    FrArTp_SM_Tx_StartTimer(actConnIdx,FrArTp_SM_actConn[actConnIdx].tx.stMinTicks);

    /* Set state variable */
    TS_AtomicAssign8(FrArTp_SM_actConn[actConnIdx].tx.state, FRARTP_SM_TXSTATE_WAITTX);
  }
  else /* there is no minimum time between CFs */
  {
    FrArTp_SM_Tx_tr_WaitTx_WaitPdu(actConnIdx, connIdx);
  }
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_tr_TxPending_WaitFC
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx
)
{
  /* exit code */
  FrArTp_SM_Tx_exit_TxPending(actConnIdx);

  /* transition code */

  /* entry code */
  FrArTp_SM_Tx_entry_WaitFC(actConnIdx, connIdx);
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_entry_WaitFC
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx
)
{
  CONSTP2CONST(FrArTp_ChannelCfgType,AUTOMATIC,FRARTP_APPL_CONST) pChannelCfg =
      FrArTp_CfgAccess_getChannelCfgPtr_byConnIdx(connIdx);

  FrArTp_SM_Tx_StartTimer(actConnIdx,pChannelCfg->timeoutBs);

  /* Set state variable */
  /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00064.2,1 */
  TS_AtomicAssign8(FrArTp_SM_actConn[actConnIdx].tx.state, FRARTP_SM_TXSTATE_WAITFC);
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_tr_TxPending_Idle
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx
)
{
  /* exit code */
  FrArTp_SM_Tx_exit_TxPending(actConnIdx);

  /* entry code */
  FrArTp_SM_Tx_entry_Idle(actConnIdx);

  /* transition code */
  FrArTp_SduMng_txConfirmation(connIdx,NTFRSLT_OK);
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_tr_Active_Idle
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx
)
{
  /* entry code */
  FrArTp_SM_Tx_entry_Idle(actConnIdx);

  /* transition code */
  FrArTp_SduMng_txConfirmation(connIdx,NTFRSLT_E_NOT_OK);
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_entry_Idle
(
  FrArTp_ActConnIdxType actConnIdx
)
{
  /* Enter exclusive area */
  SchM_Enter_FrArTp_SCHM_FRARTP_EXCLUSIVE_AREA_0();

  /* Stop the timer */
  FrArTp_SM_Tx_StopTimer(actConnIdx);

  /* Set state variable */
  TS_AtomicAssign8(FrArTp_SM_actConn[actConnIdx].tx.state, FRARTP_SM_TXSTATE_IDLE);

  /* release active connection */
  FrArTp_SM_FreeActConnIdx(actConnIdx);

  /* Exit exclusive area */
  SchM_Exit_FrArTp_SCHM_FRARTP_EXCLUSIVE_AREA_0();
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_tr_WaitFC_WaitPdu
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx,
  uint8 blockSize,
  uint8 STMin
)
{
  const uint16 mainFunctionPeriodMs = FrArTp_CfgAccess_getMainFunctionPeriodMS();

  /* transition code */
  FrArTp_SM_Tx_StopTimer(actConnIdx);

  /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00068.1,1 */
  if (blockSize == 0U)
  {
    FrArTp_SM_actConn[actConnIdx].tx.ctsExpected = FALSE;
  }

  FrArTp_SM_actConn[actConnIdx].tx.frameCount = blockSize;

  if ((STMin < FRARTP_SM_TX_MAX_VALUE_MILLISECONDS) && (STMin > mainFunctionPeriodMs))
  {
    /* milliseconds */
    if ((STMin % mainFunctionPeriodMs) == 0U)
    {
      FrArTp_SM_actConn[actConnIdx].tx.stMinTicks = STMin/(uint8)mainFunctionPeriodMs;
    }
    else
    {
      /* round up */
      FrArTp_SM_actConn[actConnIdx].tx.stMinTicks = (STMin/(uint8)mainFunctionPeriodMs) + 1U;
    }
  }
  else
  {
    /* microseconds */
    FrArTp_SM_actConn[actConnIdx].tx.stMinTicks = 0U;
  }

  /* entry code */
  /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00067.1,1 */
  FrArTp_SM_Tx_entry_WaitPdu(actConnIdx, connIdx);
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_tr_WaitTx_WaitPdu
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx
)
{
  /* entry code */
  FrArTp_SM_Tx_entry_WaitPdu(actConnIdx, connIdx);
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_exit_TxPending
(
  FrArTp_ActConnIdxType actConnIdx
)
{
  /* entry code */
  FrArTp_SM_Tx_StopTimer(actConnIdx);
}
/*
 * Stop code section declaration
 */
#define FRARTP_STOP_SEC_CODE
#include <FrArTp_MemMap.h>
