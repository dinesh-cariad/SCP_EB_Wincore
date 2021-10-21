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

#include <FrArTp_SM_Int.h>        /* get module internal SM unit interfaces */
#include <FrArTp_SduMng_Int.h>    /* get module internal SduMng unit interfaces */
#include <FrArTp_CfgAccess_Int.h> /* get module internal CfgAccess unit interfaces */

/* ****************************************************************************
 **                      Local Macros                                        **
 *****************************************************************************/

#define FrArTp_SM_Rx_timeout_tr_CheckBuffer_Idle(actConn) FrArTp_SM_Rx_tr_Active_Idle(actConn)

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

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_timeout
(
    FrArTp_ActConnIdxType actConnIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_tr_Timeout_Idle
(
    FrArTp_ActConnIdxType actConnIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_tr_Timeout_OVFLW_Idle
(
    FrArTp_ActConnIdxType actConnIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_self_tr_Idle_NOTOK
(
    FrArTp_ActConnIdxType actConnIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_tr_Idle_CheckBuffer
(
  FrArTp_ActConnIdxType actConnIdx,
  uint32 dataLength,
  uint8 payloadLength,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pPayload
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_tr_CheckBuffer_CTSWaitPdu
(
  FrArTp_ActConnIdxType actConnIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_tr_CTSWaitPdu_CTSTxPending
(
  FrArTp_ActConnIdxType actConnIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_tr_CTSTxPending_WaitData
(
  FrArTp_ActConnIdxType actConnIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_entry_CheckBuffer
(
  FrArTp_ActConnIdxType actConnIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_entry_CTSWaitPdu
(
  FrArTp_ActConnIdxType actConnIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_entry_CTSTxPending
(
  FrArTp_ActConnIdxType actConnIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_entry_WaitData
(
  FrArTp_ActConnIdxType actConnIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_self_tr_CTSTxPending
(
  FrArTp_ActConnIdxType actConnIdx,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pFrameStatus,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pBlockSize,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pStMin
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_tr_WaitData_event_receivedData
(
  FrArTp_ActConnIdxType actConnIdx,
  uint8 sequenceNumber,
  uint8 payloadLength,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pPayload
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_entry_Idle
(
  FrArTp_ActConnIdxType actConnIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_tr_Idle_OVFLWWaitPdu
(
  FrArTp_ActConnIdxType actConnIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_tr_OVFLWWaitPdu_OVFLWTxPending
(
  FrArTp_ActConnIdxType actConnIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_tr_WAITWaitPdu_WAITTxPending
(
  FrArTp_ActConnIdxType actConnIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_self_tr_OVFLWTxPending
(
  FrArTp_ActConnIdxType actConnIdx,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pFrameStatus,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pBlockSize,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pStMin
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_self_tr_WAITTxPending
(
  FrArTp_ActConnIdxType actConnIdx,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pFrameStatus,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pBlockSize,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pStMin
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_tr_OVFLWTxPending_Idle
(
  FrArTp_ActConnIdxType actConnIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_tr_WAITTxPending_CheckBuffer
(
  FrArTp_ActConnIdxType actConnIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_entry_WAITWaitPdu
(
  FrArTp_ActConnIdxType actConnIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_tr_CheckBuffer_WAITWaitPdu
(
  FrArTp_ActConnIdxType actConnIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_entry_WAITTxPending
(
  FrArTp_ActConnIdxType actConnIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_entry_OVFLWTxPending
(
  FrArTp_ActConnIdxType actConnIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_entry_OVFLWWaitPdu
(
  FrArTp_ActConnIdxType actConnIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_tr_Active_Idle
(
  FrArTp_ActConnIdxType actConnIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_tr_OVFLW_Idle
(
  FrArTp_ActConnIdxType actConnIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_self_tr_CheckBuffer
(
  FrArTp_ActConnIdxType actConnIdx
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_SF_Received
(
  FrArTp_ConnIdxType connIdx,
  uint32 dataLength,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pPayload
);

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_FF_Received
(
  FrArTp_ConnIdxType connIdx,
  FrArTp_ActConnIdxType actConnIdx,
  uint32 dataLength,
  uint8 payloadLength,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pPayload
);

/** \brief Check if connection with connIdx is active and terminate it, otherwise allocate a new one. */
STATIC FUNC(FrArTp_ActConnIdxType, FRARTP_CODE) FrArTp_SM_Rx_GetActConnIdx
(
  FrArTp_ConnIdxType connIdx
);

/* ****************************************************************************
**                      Function Definitions                                 **
******************************************************************************/


TS_MOD_PRIV_DEFN FUNC(void, FRARTP_CODE) FrArTp_SM_receivedData
(
  FrArTp_ConnIdxType connIdx,
  uint32 dataLength,
  uint8 payloadLength,
  uint8 sequenceNumber,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pPayload
)
{
  FrArTp_ActConnIdxType actConnIdx = FRARTP_ACTCONN_INVALID;

  /* If SF-X received */
  if((dataLength <= (uint32)payloadLength) && (dataLength > 0U))
  {
    actConnIdx = FrArTp_SM_FindActConnIdx(connIdx);

    /* If connection is active */
  /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00262,1,
              FrArTp.ASR42.SWS_FrArTp_00083.2,1,
              FrArTp.ASR42.SWS_FrArTp_00283.2,1 */
    if(actConnIdx != FRARTP_ACTCONN_INVALID)
    {
      uint8 rxState;
      TS_AtomicAssign8(rxState, FrArTp_SM_actConn[actConnIdx].rx.state);

      /* If connection is currently receiving */
      if(rxState != FRARTP_SM_RXSTATE_IDLE)
      {
        /* Terminate the current reception */
        FrArTp_SM_Rx_tr_Active_Idle(actConnIdx);
      }
    }

    /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00083.1,1,
                FrArTp.ASR42.SWS_FrArTp_00083.3,1,
                FrArTp.ASR42.SWS_FrArTp_00283.1,1,
                FrArTp.ASR42.SWS_FrArTp_00283.3,1 */
    /* Process received single frame */
    FrArTp_SM_Rx_SF_Received(connIdx, dataLength, pPayload);
  }
  /* FF-X or CF received */
  else
  {
    /* dataLength represents embedded length of an FF */
    if(dataLength > 0U)
    {
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00083.5,1,
                  FrArTp.ASR42.SWS_FrArTp_00283.5,1 */
      /* Search for potentially active connection and terminate it, otherwise allocate a new one. */
      actConnIdx = FrArTp_SM_Rx_GetActConnIdx(connIdx);
    }
    else /* CF received */
    {
      actConnIdx = FrArTp_SM_FindActConnIdx(connIdx);
    }

    if(actConnIdx != FRARTP_ACTCONN_INVALID)
    {
      uint8 rxState;
      TS_AtomicAssign8(rxState, FrArTp_SM_actConn[actConnIdx].rx.state);

      switch(rxState)
      {
        case FRARTP_SM_RXSTATE_IDLE :
        {
          /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00114,1 */
          FrArTp_SM_Rx_FF_Received(connIdx,actConnIdx,dataLength,payloadLength,pPayload);
          break;
        }
        case FRARTP_SM_RXSTATE_OVFLW_WAITPDU :  /* intended fall through */
        case FRARTP_SM_RXSTATE_CHECKBUFFER :    /* intended fall through */
        case FRARTP_SM_RXSTATE_CTS_WAITPDU :    /* intended fall through */
        case FRARTP_SM_RXSTATE_CTS_TXPENDING :  /* intended fall through */
        case FRARTP_SM_RXSTATE_WAIT_WAITPDU :   /* intended fall through */
        case FRARTP_SM_RXSTATE_WAIT_TXPENDING : /* intended fall through */
        case FRARTP_SM_RXSTATE_OVFLW_TXPENDING : break;
        case FRARTP_SM_RXSTATE_WAITDATA :
        {
          /* CF is received */
          /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00083.8,1,
                      FrArTp.ASR42.SWS_FrArTp_00083.9,1,
                      FrArTp.ASR42.SWS_FrArTp_00283.8,1,
                      FrArTp.ASR42.SWS_FrArTp_00283.9,1,
                      FrArTp.ASR42.SWS_FrArTp_00063.1,1,
                      FrArTp.ASR42.SWS_FrArTp_00114,1 */
          FrArTp_SM_Rx_tr_WaitData_event_receivedData(actConnIdx, sequenceNumber, payloadLength, pPayload);
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
}

TS_MOD_PRIV_DEFN FUNC(uint8, FRARTP_CODE) FrArTp_SM_isFCPending
(
  FrArTp_ActConnIdxType actConnIdx
)
{
  uint8 fcPendingCount = 0U;
  uint8 rxState;
  TS_AtomicAssign8(rxState, FrArTp_SM_actConn[actConnIdx].rx.state);

  switch(rxState)
  {
    case FRARTP_SM_RXSTATE_IDLE : break;
    case FRARTP_SM_RXSTATE_CHECKBUFFER : break;
    case FRARTP_SM_RXSTATE_CTS_WAITPDU :
    {
      fcPendingCount = 1U;
      FrArTp_SM_Rx_tr_CTSWaitPdu_CTSTxPending(actConnIdx);
      break;
    }
    case FRARTP_SM_RXSTATE_CTS_TXPENDING : break;
    case FRARTP_SM_RXSTATE_OVFLW_WAITPDU :
    {
      fcPendingCount = 1U;
      FrArTp_SM_Rx_tr_OVFLWWaitPdu_OVFLWTxPending(actConnIdx);
      break;
    }
    case FRARTP_SM_RXSTATE_OVFLW_TXPENDING : break;
    case FRARTP_SM_RXSTATE_WAIT_WAITPDU :
    {
      fcPendingCount = 1U;
      FrArTp_SM_Rx_tr_WAITWaitPdu_WAITTxPending(actConnIdx);
      break;
    }
    case FRARTP_SM_RXSTATE_WAIT_TXPENDING : break;
    case FRARTP_SM_RXSTATE_WAITDATA : break;
    /* CHECK: NOPARSE */
    default:
    {
      FRARTP_UNREACHABLE_CODE_ASSERT(FRARTP_MAINFUNCTION_SERVICE_ID);
      break;
    }
    /* CHECK: PARSE */
  }

  return fcPendingCount;
}

TS_MOD_PRIV_DEFN FUNC(boolean, FRARTP_CODE) FrArTp_SM_copyFCParameters
(
  FrArTp_ActConnIdxType actConnIdx,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pFrameStatus,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pBlockSize,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pStMin
)
{
  boolean result = FALSE;
  uint8 rxState;
  TS_AtomicAssign8(rxState, FrArTp_SM_actConn[actConnIdx].rx.state);

  switch(rxState)
  {
    case FRARTP_SM_RXSTATE_IDLE : break;
    case FRARTP_SM_RXSTATE_CHECKBUFFER : break;
    case FRARTP_SM_RXSTATE_CTS_WAITPDU :  break;
    case FRARTP_SM_RXSTATE_CTS_TXPENDING :
    {
      result = TRUE;
      FrArTp_SM_Rx_self_tr_CTSTxPending(actConnIdx,pFrameStatus,pBlockSize,pStMin);
      break;
    }
    case FRARTP_SM_RXSTATE_OVFLW_WAITPDU : break;
    case FRARTP_SM_RXSTATE_OVFLW_TXPENDING :
    {
      result = TRUE;
      FrArTp_SM_Rx_self_tr_OVFLWTxPending(actConnIdx,pFrameStatus,pBlockSize,pStMin);
      break;
    }
    case FRARTP_SM_RXSTATE_WAIT_WAITPDU : break;
    case FRARTP_SM_RXSTATE_WAIT_TXPENDING :
    {
      result = TRUE;
      FrArTp_SM_Rx_self_tr_WAITTxPending(actConnIdx,pFrameStatus,pBlockSize,pStMin);
      break;
    }
    case FRARTP_SM_RXSTATE_WAITDATA : break;
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

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, FRARTP_CODE) FrArTp_SM_cancelRx
(
  FrArTp_ConnIdxType connIdx
)
{
  Std_ReturnType retVal = E_NOT_OK;

  const FrArTp_ActConnIdxType actConnIdx = FrArTp_SM_FindActConnIdx(connIdx);

  /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00227.1,1,
              FrArTp.ASR42.SWS_FrArTp_00227.2,1 */
  if(actConnIdx != FRARTP_ACTCONN_INVALID)
  {
    uint8 rxState;
    TS_AtomicAssign8(rxState, FrArTp_SM_actConn[actConnIdx].rx.state);

    switch(rxState)
    {
      case FRARTP_SM_RXSTATE_OVFLW_WAITPDU :
      case FRARTP_SM_RXSTATE_OVFLW_TXPENDING :
      {
        FrArTp_SM_Rx_tr_OVFLW_Idle(actConnIdx);
        retVal = E_OK;
        break;
      }
      case FRARTP_SM_RXSTATE_CHECKBUFFER :
      case FRARTP_SM_RXSTATE_CTS_WAITPDU :
      case FRARTP_SM_RXSTATE_CTS_TXPENDING :
      case FRARTP_SM_RXSTATE_WAIT_WAITPDU :
      case FRARTP_SM_RXSTATE_WAIT_TXPENDING :
      case FRARTP_SM_RXSTATE_WAITDATA :
      {
        /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00228,1 */
        FrArTp_SM_Rx_tr_Active_Idle(actConnIdx);
        retVal = E_OK;
        break;
      }
      case FRARTP_SM_RXSTATE_IDLE :
      {
        /* Do nothing if reception is not active */
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

TS_MOD_PRIV_DEFN FUNC(void, FRARTP_CODE) FrArTp_SM_txConfirmationFC
(
  FrArTp_ActConnIdxType actConnIdx
)
{

  uint8 rxState;
  TS_AtomicAssign8(rxState, FrArTp_SM_actConn[actConnIdx].rx.state);

  switch(rxState)
  {
    case FRARTP_SM_RXSTATE_IDLE : break;
    case FRARTP_SM_RXSTATE_CHECKBUFFER : break;
    case FRARTP_SM_RXSTATE_CTS_WAITPDU : break;
    case FRARTP_SM_RXSTATE_CTS_TXPENDING :
    {
      FrArTp_SM_Rx_tr_CTSTxPending_WaitData(actConnIdx);
      break;
    }
    case FRARTP_SM_RXSTATE_OVFLW_WAITPDU : break;
    case FRARTP_SM_RXSTATE_OVFLW_TXPENDING :
    {
      FrArTp_SM_Rx_tr_OVFLWTxPending_Idle(actConnIdx);
      break;
    }
    case FRARTP_SM_RXSTATE_WAIT_WAITPDU : break;
    case FRARTP_SM_RXSTATE_WAIT_TXPENDING :
    {
      FrArTp_SM_Rx_tr_WAITTxPending_CheckBuffer(actConnIdx);
      break;
    }
    case FRARTP_SM_RXSTATE_WAITDATA : break;
    /* CHECK: NOPARSE */
    default:
    {
      FRARTP_UNREACHABLE_CODE_ASSERT(FRARTP_TXCONFIRMATION_SERVICE_ID);
      break;
    }
    /* CHECK: PARSE */
  }

}

/* ****************************************************************************
**                      Local Function Definitions                           **
******************************************************************************/

TS_MOD_PRIV_DEFN FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_mainFunction
(
    FrArTp_ActConnIdxType actConnIdx
)
{
  const FrArTp_ConnIdxType connIdx = FrArTp_SM_actConn[actConnIdx].connIdx;

  if(connIdx != FRARTP_CONN_INVALID)
  {
    CONSTP2CONST(FrArTp_ChannelCfgType,AUTOMATIC,FRARTP_APPL_CONST) pChannelCfg
        = FrArTp_CfgAccess_getChannelCfgPtr_byConnIdx(connIdx);

    if(FrArTp_SM_actConn[actConnIdx].rx.timer > 0U)
    {
      FrArTp_SM_actConn[actConnIdx].rx.timer--;

      if(FrArTp_SM_actConn[actConnIdx].rx.timer == 0U)
      {
        FrArTp_SM_Rx_timeout(actConnIdx);
      }
    }

    {
      uint8 rxState;
      TS_AtomicAssign8(rxState, FrArTp_SM_actConn[actConnIdx].rx.state);

      switch(rxState)
      {
        case FRARTP_SM_RXSTATE_CHECKBUFFER :
        {
          /* CheckBuffer self transition */
          /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00297.1,1,
                      FrArTp.ASR42.SWS_FrArTp_00297.2,1 */
          FrArTp_SM_Rx_self_tr_CheckBuffer(actConnIdx);

          /* checkBuffer to CTS.WaitPdu transition */
          /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00064.1,1 */
          TS_AtomicAssign8(rxState, FrArTp_SM_actConn[actConnIdx].rx.state);
          if(rxState == FRARTP_SM_RXSTATE_CHECKBUFFER)
          {
            const uint8 CFpayloadlength = pChannelCfg->payloadLength - ((2U*pChannelCfg->addrSize) + (uint8)FRARTP_PCI_CF_LENGTH);
            if((FrArTp_SM_actConn[actConnIdx].rx.available >= ((uint32)CFpayloadlength*(uint32)FrArTp_SM_Conn[connIdx].BS)) ||
               (FrArTp_SM_actConn[actConnIdx].rx.available >= FrArTp_SM_actConn[actConnIdx].rx.remaining)
              )
            {
              FrArTp_SM_Rx_tr_CheckBuffer_CTSWaitPdu(actConnIdx);
            }
          }

          break;
        }
        case FRARTP_SM_RXSTATE_IDLE: break;
        case FRARTP_SM_RXSTATE_CTS_WAITPDU : break;
        case FRARTP_SM_RXSTATE_CTS_TXPENDING : break;
        case FRARTP_SM_RXSTATE_OVFLW_WAITPDU : break;
        case FRARTP_SM_RXSTATE_OVFLW_TXPENDING : break;
        case FRARTP_SM_RXSTATE_WAIT_WAITPDU : break;
        case FRARTP_SM_RXSTATE_WAIT_TXPENDING : break;
        case FRARTP_SM_RXSTATE_WAITDATA : break;
        /* CHECK: NOPARSE */
        default:
        {
          FRARTP_UNREACHABLE_CODE_ASSERT(FRARTP_MAINFUNCTION_SERVICE_ID);
          break;
        }
        /* CHECK: PARSE */
      }
    }
  }
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_timeout
(
    FrArTp_ActConnIdxType actConnIdx
)
{
  const FrArTp_ConnIdxType connIdx = FrArTp_SM_actConn[actConnIdx].connIdx;
  CONSTP2CONST(FrArTp_ChannelCfgType,AUTOMATIC,FRARTP_APPL_CONST) pChannelCfg
      = FrArTp_CfgAccess_getChannelCfgPtr_byConnIdx(connIdx);
  uint8 rxState;
  TS_AtomicAssign8(rxState, FrArTp_SM_actConn[actConnIdx].rx.state);

  switch(rxState)
  {
    case FRARTP_SM_RXSTATE_CHECKBUFFER :
    /* if timeout(BR) occurs */
    {
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00301,1 */
      if(FrArTp_SM_actConn[actConnIdx].rx.count < pChannelCfg->maxWft)
      {
        FrArTp_SM_Rx_tr_CheckBuffer_WAITWaitPdu(actConnIdx);
      }
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00067.3,1,
                  FrArTp.ASR42.SWS_FrArTp_00295.2,1 */
      else
      {
        FrArTp_SM_Rx_timeout_tr_CheckBuffer_Idle(actConnIdx);
      }
      break;
    }
    case FRARTP_SM_RXSTATE_OVFLW_WAITPDU : /* intended fall through */
    case FRARTP_SM_RXSTATE_OVFLW_TXPENDING :
    {
      FrArTp_SM_Rx_tr_Timeout_OVFLW_Idle(actConnIdx);
      break;
    }
    /* FrArTpTimeoutAr expires */
    /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00263.1,1 */
    case FRARTP_SM_RXSTATE_CTS_TXPENDING : /* intended fall through */
    case FRARTP_SM_RXSTATE_WAIT_TXPENDING :
    {
      FrArTp_Encode_cancelTx(actConnIdx);
      FrArTp_SM_Rx_tr_Timeout_Idle(actConnIdx);
      break;
    }
    /* FrArTpTimeoutCr expires */
    /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00265.1,1 */
    case FRARTP_SM_RXSTATE_WAITDATA :      /* intended fall through */
    case FRARTP_SM_RXSTATE_CTS_WAITPDU :   /* intended fall through */
    case FRARTP_SM_RXSTATE_WAIT_WAITPDU :
    {
      FrArTp_SM_Rx_tr_Timeout_Idle(actConnIdx);
      break;
    }
    /* CHECK: NOPARSE */
    case FRARTP_SM_RXSTATE_IDLE: /* intended fall through */
    default:
    {
      FRARTP_UNREACHABLE_CODE_ASSERT(FRARTP_MAINFUNCTION_SERVICE_ID);
      break;
    }
    /* CHECK: PARSE */
  }
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_tr_CTSTxPending_WaitData
(
  FrArTp_ActConnIdxType actConnIdx
)
{
  /* exit code */

  /* transition code */
  const FrArTp_ConnIdxType connIdx = FrArTp_SM_actConn[actConnIdx].connIdx;

  CONSTP2CONST(FrArTp_ChannelCfgType,AUTOMATIC,FRARTP_APPL_CONST) pChannelCfg =
      FrArTp_CfgAccess_getChannelCfgPtr_byConnIdx(connIdx);

  FrArTp_SM_Rx_StopTimer(actConnIdx);
  FrArTp_SM_Rx_StartTimer(actConnIdx,pChannelCfg->timeoutCr);

  /* entry code */
  FrArTp_SM_Rx_entry_WaitData(actConnIdx);
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_tr_WaitData_event_receivedData
(
  FrArTp_ActConnIdxType actConnIdx,
  uint8 sequenceNumber,
  uint8 payloadLength,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pPayload
)
{
  BufReq_ReturnType retBuffer;
  PduLengthType available;

  const FrArTp_ConnIdxType connIdx = FrArTp_SM_actConn[actConnIdx].connIdx;
  CONSTP2CONST(FrArTp_ChannelCfgType,AUTOMATIC,FRARTP_APPL_CONST) pChannelCfg =
      FrArTp_CfgAccess_getChannelCfgPtr_byConnIdx(connIdx);

  PduInfoType PduInfo;
  /* transition code */
  /* check SN from CF */
  if ( sequenceNumber == FrArTp_SM_actConn[actConnIdx].rx.seq )
  {
    FrArTp_SM_actConn[actConnIdx].rx.seq++;

    /* If max SN is reached, reset it to  zero */
    /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00061.2,1 */
    if (FrArTp_SM_actConn[actConnIdx].rx.seq > (uint8)FRARTP_MAX_SEQUENCE_VALUE)
    {
      FrArTp_SM_actConn[actConnIdx].rx.seq = 0U;
    }
    FrArTp_SM_Rx_StopTimer(actConnIdx);

    if ((uint32)payloadLength > FrArTp_SM_actConn[actConnIdx].rx.remaining )
    {
      payloadLength  = (uint8)FrArTp_SM_actConn[actConnIdx].rx.remaining;
    }

    PduInfo.SduLength = payloadLength;
    PduInfo.SduDataPtr = pPayload;

    /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00114,1 */
    retBuffer = FrArTp_SduMng_copyRxData(connIdx,&PduInfo,&available);

    if(retBuffer == BUFREQ_OK)
    {
      FrArTp_SM_actConn[actConnIdx].rx.available = available;
      FrArTp_SM_actConn[actConnIdx].rx.remaining -= payloadLength;

      if (FrArTp_SM_actConn[actConnIdx].rx.remaining > 0U)
      {
        /* If at least one other CF is expected */
        if
        (
          /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00064.1,1 */
          (((uint16)FrArTp_SM_actConn[actConnIdx].rx.frameCount + 1U) <= (uint16)FrArTp_SM_Conn[connIdx].BS)
          /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00068.1,1 */
          || (FrArTp_SM_Conn[connIdx].BS == 0U)
        )
        {
          /* self transition WaitData*/
          /* transition code */
          FrArTp_SM_Rx_StartTimer(actConnIdx,pChannelCfg->timeoutCr);
        }
        /* Otherwise an FC should be sent */
        else
        {
          /* transition code */
          FrArTp_SM_actConn[actConnIdx].rx.count = 0U;
          /* entry code */
          FrArTp_SM_Rx_entry_CheckBuffer(actConnIdx);
        }
        FrArTp_SM_actConn[actConnIdx].rx.frameCount++;
      }
      else
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00063.1,1 */
      {
        /* transition code */
        FrArTp_SduMng_rxIndication(connIdx,NTFRSLT_OK);

        /* entry code */
        FrArTp_SM_Rx_entry_Idle(actConnIdx);
      }
    }
    /* If CopyRxData failed */
    /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00295.1,1 */
    else
    {
      FrArTp_SM_Rx_tr_Active_Idle(actConnIdx);
    }
  }
  /* If the received sequence number was not the expected one */
  else
  {
    FrArTp_SM_Rx_tr_Active_Idle(actConnIdx);
  }
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_self_tr_Idle_NOTOK
(
  FrArTp_ActConnIdxType actConnIdx
)
{
  /* Notify upper layer */
  const FrArTp_ConnIdxType connIdx = FrArTp_SM_actConn[actConnIdx].connIdx;
  FrArTp_SduMng_rxIndication(connIdx,NTFRSLT_E_NOT_OK);

  /* Release active connection */
  FrArTp_SM_FreeActConnIdx(actConnIdx);
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_tr_CheckBuffer_CTSWaitPdu
(
  FrArTp_ActConnIdxType actConnIdx
)
{
  /* exit code */

  /* transition code */
  FrArTp_SM_Rx_StopTimer(actConnIdx);

  /* entry code */
  FrArTp_SM_Rx_entry_CTSWaitPdu(actConnIdx);
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_tr_CTSWaitPdu_CTSTxPending
(
  FrArTp_ActConnIdxType actConnIdx
)
{
  /* exit code */

  /* transition code */

  /* entry code */
  FrArTp_SM_Rx_entry_CTSTxPending(actConnIdx);
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_self_tr_CTSTxPending
(
    FrArTp_ActConnIdxType actConnIdx,
    P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pFrameStatus,
    P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pBlockSize,
    P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pStMin
)
{
  const FrArTp_ConnIdxType connIdx = FrArTp_SM_actConn[actConnIdx].connIdx;

  *pFrameStatus = FRARTP_SM_FC_FS_CTS;
  *pBlockSize = FrArTp_SM_Conn[connIdx].BS;
  *pStMin = FrArTp_SM_Conn[connIdx].stMin;
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_tr_Idle_OVFLWWaitPdu
(
    FrArTp_ActConnIdxType actConnIdx
)
{
  /* exit code */

  /* transition code */

  /* entry code */
  FrArTp_SM_Rx_entry_OVFLWWaitPdu(actConnIdx);
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_tr_Idle_CheckBuffer
(
    FrArTp_ActConnIdxType actConnIdx,
    uint32 dataLength,
    uint8 payloadLength,
    P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pPayload
)
{
  PduInfoType PduInfo;
  PduLengthType available;
  BufReq_ReturnType retBuffer;
  PduInfo.SduLength = payloadLength;
  PduInfo.SduDataPtr = pPayload;

  /* exit code */

  /* transition code */
  /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00114,1,
              FrArTp.ASR42.SWS_FrArTp_00221.3,1 */
  retBuffer = FrArTp_SduMng_copyRxData(FrArTp_SM_actConn[actConnIdx].connIdx,&PduInfo, &available);
  if (retBuffer == BUFREQ_OK)
  {
    FrArTp_SM_actConn[actConnIdx].rx.available = available;
    FrArTp_SM_actConn[actConnIdx].rx.count = 0U;
    /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00061.1,1 */
    FrArTp_SM_actConn[actConnIdx].rx.seq = 1U;
    FrArTp_SM_actConn[actConnIdx].rx.remaining = dataLength - (uint32)payloadLength;
    /* entry code */
    FrArTp_SM_Rx_entry_CheckBuffer(actConnIdx);
  }
  else
  {
    FrArTp_SM_Rx_self_tr_Idle_NOTOK(actConnIdx);
  }

}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_entry_CheckBuffer
(
  FrArTp_ActConnIdxType actConnIdx
)
{
  const FrArTp_ConnIdxType connIdx = FrArTp_SM_actConn[actConnIdx].connIdx;

  CONSTP2CONST(FrArTp_ChannelCfgType,AUTOMATIC,FRARTP_APPL_CONST) pChannelCfg =
      FrArTp_CfgAccess_getChannelCfgPtr_byConnIdx(connIdx);

  FrArTp_SM_Rx_StartTimer(actConnIdx,pChannelCfg->timeBr);

  /* Set state variable */
  TS_AtomicAssign8(FrArTp_SM_actConn[actConnIdx].rx.state, FRARTP_SM_RXSTATE_CHECKBUFFER);
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_entry_CTSWaitPdu
(
  FrArTp_ActConnIdxType actConnIdx
)
{
  const FrArTp_ConnIdxType connIdx = FrArTp_SM_actConn[actConnIdx].connIdx;

  CONSTP2CONST(FrArTp_ChannelCfgType,AUTOMATIC,FRARTP_APPL_CONST) pChannelCfg =
      FrArTp_CfgAccess_getChannelCfgPtr_byConnIdx(connIdx);

  FrArTp_SM_Rx_StartTimer(actConnIdx,pChannelCfg->timeoutAr);

  /* Set state variable */
  TS_AtomicAssign8(FrArTp_SM_actConn[actConnIdx].rx.state, FRARTP_SM_RXSTATE_CTS_WAITPDU);
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_entry_CTSTxPending
(
  FrArTp_ActConnIdxType actConnIdx
)
{
  /* Set state variable */
  TS_AtomicAssign8(FrArTp_SM_actConn[actConnIdx].rx.state, FRARTP_SM_RXSTATE_CTS_TXPENDING);
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_entry_OVFLWWaitPdu
(
  FrArTp_ActConnIdxType actConnIdx
)
{
  const FrArTp_ConnIdxType connIdx = FrArTp_SM_actConn[actConnIdx].connIdx;

  CONSTP2CONST(FrArTp_ChannelCfgType,AUTOMATIC,FRARTP_APPL_CONST) pChannelCfg =
      FrArTp_CfgAccess_getChannelCfgPtr_byConnIdx(connIdx);

  FrArTp_SM_Rx_StartTimer(actConnIdx,pChannelCfg->timeoutAr);

  /* Set state variable */
  TS_AtomicAssign8(FrArTp_SM_actConn[actConnIdx].rx.state, FRARTP_SM_RXSTATE_OVFLW_WAITPDU);
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_tr_OVFLWWaitPdu_OVFLWTxPending
(
  FrArTp_ActConnIdxType actConnIdx
)
{
  /* exit code */

  /* transition code */

  /* entry code */
  FrArTp_SM_Rx_entry_OVFLWTxPending(actConnIdx);
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_tr_WAITWaitPdu_WAITTxPending
(
  FrArTp_ActConnIdxType actConnIdx
)
{
  /* exit code */

  /* transition code */

  /* entry code */
  FrArTp_SM_Rx_entry_WAITTxPending(actConnIdx);
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_tr_CheckBuffer_WAITWaitPdu
(
  FrArTp_ActConnIdxType actConnIdx
)
{
  /* exit code */

  /* transition code */
  /* Increase wait frame counter */
  FrArTp_SM_actConn[actConnIdx].rx.count++;

  /* entry code */
  FrArTp_SM_Rx_entry_WAITWaitPdu(actConnIdx);
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_entry_WAITWaitPdu
(
  FrArTp_ActConnIdxType actConnIdx
)
{
  const FrArTp_ConnIdxType connIdx = FrArTp_SM_actConn[actConnIdx].connIdx;
  CONSTP2CONST(FrArTp_ChannelCfgType,AUTOMATIC,FRARTP_APPL_CONST) pChannelCfg
      = FrArTp_CfgAccess_getChannelCfgPtr_byConnIdx(connIdx);

  /* Time for transmission of the FC frame on the receiver side */
  FrArTp_SM_Rx_StartTimer(actConnIdx,pChannelCfg->timeoutAr);

  /* Set state variable */
  TS_AtomicAssign8(FrArTp_SM_actConn[actConnIdx].rx.state, FRARTP_SM_RXSTATE_WAIT_WAITPDU);
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_entry_OVFLWTxPending
(
  FrArTp_ActConnIdxType actConnIdx
)
{
  /* Set state variable */
  TS_AtomicAssign8(FrArTp_SM_actConn[actConnIdx].rx.state, FRARTP_SM_RXSTATE_OVFLW_TXPENDING);
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_entry_WAITTxPending
(
  FrArTp_ActConnIdxType actConnIdx
)
{
  /* Set state variable */
  TS_AtomicAssign8(FrArTp_SM_actConn[actConnIdx].rx.state, FRARTP_SM_RXSTATE_WAIT_TXPENDING);
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_self_tr_OVFLWTxPending
(
  FrArTp_ActConnIdxType actConnIdx,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pFrameStatus,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pBlockSize,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pStMin
)
{
  const FrArTp_ConnIdxType connIdx = FrArTp_SM_actConn[actConnIdx].connIdx;

  *pFrameStatus = FRARTP_SM_FC_FS_OVFLW;
  *pBlockSize = FrArTp_SM_Conn[connIdx].BS;
  *pStMin = FrArTp_SM_Conn[connIdx].stMin;
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_self_tr_WAITTxPending
(
  FrArTp_ActConnIdxType actConnIdx,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pFrameStatus,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pBlockSize,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pStMin
)
{
  const FrArTp_ConnIdxType connIdx = FrArTp_SM_actConn[actConnIdx].connIdx;

  *pFrameStatus = FRARTP_SM_FC_FS_WT;
  *pBlockSize = FrArTp_SM_Conn[connIdx].BS;
  *pStMin = FrArTp_SM_Conn[connIdx].stMin;
}
STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_tr_OVFLWTxPending_Idle
(
  FrArTp_ActConnIdxType actConnIdx
)
{
  /* exit code */

  /* transition code */
  FrArTp_SM_Rx_StopTimer(actConnIdx);

  /* entry code */
  FrArTp_SM_Rx_entry_Idle(actConnIdx);
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_tr_WAITTxPending_CheckBuffer
(
  FrArTp_ActConnIdxType actConnIdx
)
{
  /* exit code */

  /* transition code */
  FrArTp_SM_Rx_StopTimer(actConnIdx);

  /* entry code */
  FrArTp_SM_Rx_entry_CheckBuffer(actConnIdx);
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_entry_WaitData
(
  FrArTp_ActConnIdxType actConnIdx
)
{

  /* Reset frame counter for the next block */
  FrArTp_SM_actConn[actConnIdx].rx.frameCount = 1U;

  /* Set state variable */
  TS_AtomicAssign8(FrArTp_SM_actConn[actConnIdx].rx.state, FRARTP_SM_RXSTATE_WAITDATA);
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_tr_Timeout_Idle
(
  FrArTp_ActConnIdxType actConnIdx
)
{
  /* exit code */

  /* transition code */
  const FrArTp_ConnIdxType connIdx = FrArTp_SM_actConn[actConnIdx].connIdx;
  FrArTp_SM_Rx_StopTimer(actConnIdx);
  FrArTp_SduMng_rxIndication(connIdx,NTFRSLT_E_NOT_OK);

  /* entry code */
  FrArTp_SM_Rx_entry_Idle(actConnIdx);
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_tr_Timeout_OVFLW_Idle
(
  FrArTp_ActConnIdxType actConnIdx
)
{
  /* exit code */

  /* transition code */
  FrArTp_SM_Rx_StopTimer(actConnIdx);

  /* entry code */
  FrArTp_SM_Rx_entry_Idle(actConnIdx);
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_entry_Idle
(
  FrArTp_ActConnIdxType actConnIdx
)
{
  /* Set state variable */
  TS_AtomicAssign8(FrArTp_SM_actConn[actConnIdx].rx.state, FRARTP_SM_RXSTATE_IDLE);

  /* Enter exclusive area */
  SchM_Enter_FrArTp_SCHM_FRARTP_EXCLUSIVE_AREA_0();

  /* Release active connection */
  FrArTp_SM_FreeActConnIdx(actConnIdx);

  /* Exit exclusive area */
  SchM_Exit_FrArTp_SCHM_FRARTP_EXCLUSIVE_AREA_0();
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_self_tr_CheckBuffer
(
  FrArTp_ActConnIdxType actConnIdx
)
{
  const FrArTp_ConnIdxType connIdx = FrArTp_SM_actConn[actConnIdx].connIdx;
  PduInfoType PduInfo;
  PduLengthType available;
  BufReq_ReturnType retBuffer;

  PduInfo.SduDataPtr = NULL_PTR;
  PduInfo.SduLength = 0U;

  retBuffer = FrArTp_SduMng_copyRxData(connIdx,&PduInfo,&available);

  if(retBuffer == BUFREQ_OK)
  {
    FrArTp_SM_actConn[actConnIdx].rx.available = available;
  }
  else
  {
    FrArTp_SM_Rx_tr_Active_Idle(actConnIdx);
  }
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_tr_Active_Idle
(
  FrArTp_ActConnIdxType actConnIdx
)
{
  const FrArTp_ConnIdxType connIdx = FrArTp_SM_actConn[actConnIdx].connIdx;
  /* transition code */
  FrArTp_SM_Rx_StopTimer(actConnIdx);
  FrArTp_SduMng_rxIndication(connIdx,NTFRSLT_E_NOT_OK);

  /* entry code */
  FrArTp_SM_Rx_entry_Idle(actConnIdx);
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_tr_OVFLW_Idle
(
  FrArTp_ActConnIdxType actConnIdx
)
{
  /* transition code */
  FrArTp_SM_Rx_StopTimer(actConnIdx);

  /* entry code */
  FrArTp_SM_Rx_entry_Idle(actConnIdx);
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_SF_Received
(
  FrArTp_ConnIdxType connIdx,
  uint32 dataLength,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pPayload
)
{
  PduLengthType available;
  /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00108,1,
              FrArTp.ASR42.SWS_FrArTp_00221.2,1 */
  BufReq_ReturnType retBuffer = FrArTp_SduMng_startOfReception(connIdx,(PduLengthType)dataLength,&available);

  if (retBuffer == BUFREQ_OK)
  {
    if (available >= dataLength)
    {
      PduInfoType pduInfo;
      pduInfo.SduDataPtr = pPayload;
      pduInfo.SduLength = (PduLengthType)dataLength;
      /* forward data to upper layer */
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00108,1,
                  FrArTp.ASR42.SWS_FrArTp_00221.3,1 */
      retBuffer = FrArTp_SduMng_copyRxData(connIdx,&pduInfo,&available);
      if (retBuffer == BUFREQ_OK)
      {
        FrArTp_SduMng_rxIndication(connIdx, NTFRSLT_OK);
      }
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00289.2,1,
                  FrArTp.ASR42.SWS_FrArTp_00289.4,1 */
      else
      {
        FrArTp_SduMng_rxIndication(connIdx, NTFRSLT_E_NOT_OK);
      }
    }
    /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00289.1,1,
                FrArTp.ASR42.SWS_FrArTp_00289.3,1 */
    else
    {
      FrArTp_SduMng_rxIndication(connIdx, NTFRSLT_E_NOT_OK);
    }
  }
  /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00298,1 */
  else
  {
    /* nothing to do, but branch is necessary for MISRA */
  }
}

STATIC FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_FF_Received
(
  FrArTp_ConnIdxType connIdx,
  FrArTp_ActConnIdxType actConnIdx,
  uint32 dataLength,
  uint8 payloadLength,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pPayload
)
{
  /* If FF received */
  if (dataLength > 0U)
  {
    PduLengthType available;
    /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00114,1,
                FrArTp.ASR42.SWS_FrArTp_00221.1,1,
                FrArTp.ASR42.SWS_FrArTp_00083.4,1,
                FrArTp.ASR42.SWS_FrArTp_00083.6,1,
                FrArTp.ASR42.SWS_FrArTp_00283.4,1,
                FrArTp.ASR42.SWS_FrArTp_00283.6,1 */
    const BufReq_ReturnType retBuffer = FrArTp_SduMng_startOfReception(connIdx,(PduLengthType)dataLength,&available);

    if(retBuffer == BUFREQ_OK)
    {
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00294,1 */
      if(available < payloadLength)
      {
        /* self transition - NOT_OK */
        FrArTp_SM_Rx_self_tr_Idle_NOTOK(actConnIdx);
      }
      else
      {
        /* FF */
        /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00114,1 */
        FrArTp_SM_Rx_tr_Idle_CheckBuffer(actConnIdx,dataLength,payloadLength,pPayload);
      }
    }
    /* !LINKSTO FrArTp.EB.AUDI_ASR_FrTP_014.1,1,
                FrArTp.ASR42.SWS_FrArTp_00117.3,1 */
    else if(retBuffer == BUFREQ_E_OVFL)
    {
      /* transmit FC:OVFLW */
      FrArTp_SM_Rx_tr_Idle_OVFLWWaitPdu(actConnIdx);
    }
    else /* BUFREQ_E_NOT_OK or BUFREQ_E_BUSY */
    {
      FrArTp_SM_FreeActConnIdx(actConnIdx);
    }
  }
  /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00083.7,1,
              FrArTp.ASR42.SWS_FrArTp_00083.10,1,
              FrArTp.ASR42.SWS_FrArTp_00283.7,1,
              FrArTp.ASR42.SWS_FrArTp_00283.10,1 */
  else /* CF received */
  {
    /* Ignore */
  }
}

STATIC FUNC(FrArTp_ActConnIdxType, FRARTP_CODE) FrArTp_SM_Rx_GetActConnIdx
(
  FrArTp_ConnIdxType connIdx
)
{
  FrArTp_ActConnIdxType activeConnIdx = FrArTp_SM_FindOrAllocateActConnIdx(connIdx);

  if (activeConnIdx != FRARTP_ACTCONN_INVALID)
  {
    uint8 rxState;
    TS_AtomicAssign8(rxState, FrArTp_SM_actConn[activeConnIdx].rx.state);

    if(
        (rxState == FRARTP_SM_RXSTATE_OVFLW_WAITPDU) ||
        (rxState == FRARTP_SM_RXSTATE_OVFLW_TXPENDING)
      )
    {
      /* FF, the existing connection is terminated  */
      /* the received FF is processed as a new communication request on the same connection */
      FrArTp_SM_Rx_tr_OVFLW_Idle(activeConnIdx);
      activeConnIdx = FrArTp_SM_FindOrAllocateActConnIdx(connIdx);
    }
    else if(rxState != FRARTP_SM_RXSTATE_IDLE)
    {
      /* FF, the existing connection is terminated  */
      /* the received FF is processed as a new communication request on the same connection */
      FrArTp_SM_Rx_tr_Active_Idle(activeConnIdx);
      activeConnIdx = FrArTp_SM_FindOrAllocateActConnIdx(connIdx);
    }
    /* CHECK: NOPARSE */
    else /* If state is idle */
    {
      /* Do nothing, branch necessary for MISRA */
    }
    /* CHECK: PARSE */
  }
  return activeConnIdx;
}
/*
 * Stop code section declaration
 */
#define FRARTP_STOP_SEC_CODE
#include <FrArTp_MemMap.h>
