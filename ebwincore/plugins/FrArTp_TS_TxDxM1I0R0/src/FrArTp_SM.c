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
 * W507: variable "rxState" is possibly uninitialized violated
 *
 * Reason:
 * False positive. Due to short-circuit evaluation in C, rxState is only evaluated if actConnIdx
 * != FRARTP_ACTCONN_INVALID in which case rxState is also set before.
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

/* ****************************************************************************
**                      Variable Definitions                                 **
******************************************************************************/

#define FRARTP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <FrArTp_MemMap.h>

VAR(FrArTp_ActConnType,FRARTP_VAR) FrArTp_SM_actConn[FRARTP_ACTCONN_MAX];

VAR(FrArTp_ConnType,FRARTP_VAR) FrArTp_SM_Conn[FRARTP_CONN_MAX];

#define FRARTP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <FrArTp_MemMap.h>

/* ****************************************************************************
**                      Local Function Prototypes                            **
******************************************************************************/

/* ****************************************************************************
**                      Function Definitions                                 **
******************************************************************************/

/*
 * Start code section declaration
 */
#define FRARTP_START_SEC_CODE
#include <FrArTp_MemMap.h>

TS_MOD_PRIV_DEFN FUNC(FrArTp_ConnIdxType, FRARTP_CODE) FrArTp_SM_getConnIdx
(
  FrArTp_ActConnIdxType actConnIdx
)
{
  FrArTp_ConnIdxType connIdx;

  /* Enter exclusive area */
  SchM_Enter_FrArTp_SCHM_FRARTP_EXCLUSIVE_AREA_0();

  connIdx =  FrArTp_SM_actConn[actConnIdx].connIdx;

  /* Exit exclusive area */
  SchM_Exit_FrArTp_SCHM_FRARTP_EXCLUSIVE_AREA_0();

  return connIdx;
}


TS_MOD_PRIV_DEFN FUNC(void, FRARTP_CODE) FrArTp_SM_init
(
  void
)
{
  /* Initialize state machine array for active connections */
  {
    FrArTp_ActConnIdxType i;

    for(i = 0U; i < FRARTP_ACTCONN_MAX; i++)
    {
      FrArTp_SM_actConn[i].connIdx = FRARTP_CONN_INVALID;
    }

  }

  /* Initialize state machine array for connections */
  {
    FrArTp_ConnIdxType i;
    const FrArTp_ConnIdxType numConn = FrArTp_CfgAccess_getNumConn();

    for(i = 0U; i < numConn; i++)
    {
      P2CONST(FrArTp_ChannelCfgType, AUTOMATIC, FRARTP_APPL_CONST) channelCfgPtr =
          FrArTp_CfgAccess_getChannelCfgPtr_byConnIdx(i);

      FrArTp_SM_Conn[i].BS = channelCfgPtr->maxBs;
      FrArTp_SM_Conn[i].stMin = channelCfgPtr->stMin;
    }
  }
}

TS_MOD_PRIV_DEFN FUNC(void, FRARTP_CODE) FrArTp_SM_mainFunction
(
  void
)
{
  FrArTp_ActConnIdxType iActConn;
  for(iActConn = 0U; iActConn < FRARTP_ACTCONN_MAX; iActConn++)
  {
    /* Rx state machine */
    {
      uint8 rxState;
      TS_AtomicAssign8(rxState, FrArTp_SM_actConn[iActConn].rx.state);

      if(rxState != FRARTP_SM_RXSTATE_IDLE)
      {
        FrArTp_SM_Rx_mainFunction(iActConn);
      }
    }

    /* Tx state machine */
    {
      uint8 txState;
      TS_AtomicAssign8(txState, FrArTp_SM_actConn[iActConn].tx.state);

      if(txState != FRARTP_SM_TXSTATE_IDLE)
      {
        FrArTp_SM_Tx_mainFunction(iActConn);
      }
    }
  }
}

TS_MOD_PRIV_DEFN FUNC(FrArTp_ActConnIdxType, FRARTP_CODE) FrArTp_SM_FindActConnIdx
(
  FrArTp_ConnIdxType connIdx
)
{
  CONSTP2CONST(FrArTp_ChannelCfgType,AUTOMATIC,FRARTP_APPL_CONST) pChannelCfg =
      FrArTp_CfgAccess_getChannelCfgPtr_byConnIdx(connIdx);

  FrArTp_ConnIdxType iIdx;
  FrArTp_ConnIdxType const endIdx = pChannelCfg->actConnIdx + pChannelCfg->numMaxActConn;

  /* Enter exclusive area */
  SchM_Enter_FrArTp_SCHM_FRARTP_EXCLUSIVE_AREA_0();

  for(iIdx = pChannelCfg->actConnIdx; iIdx < endIdx; iIdx++)
  {
    if(FrArTp_SM_actConn[iIdx].connIdx == connIdx)
    {
      break;
    }
  }

  /* Exit exclusive area */
  SchM_Exit_FrArTp_SCHM_FRARTP_EXCLUSIVE_AREA_0();

  return (iIdx != endIdx) ? iIdx : FRARTP_ACTCONN_INVALID;
}


TS_MOD_PRIV_DEFN FUNC(void, FRARTP_CODE) FrArTp_SM_FreeActConnIdx
(
  FrArTp_ActConnIdxType actConnIdx
)
{
  /* Note: This function has to be called from within an exclusive area
     (or from a function that is never preempted) */

  uint8 rxState;
  uint8 txState;

  TS_AtomicAssign8(rxState, FrArTp_SM_actConn[actConnIdx].rx.state);
  TS_AtomicAssign8(txState, FrArTp_SM_actConn[actConnIdx].tx.state);

  if((rxState == FRARTP_SM_RXSTATE_IDLE) && (txState == FRARTP_SM_TXSTATE_IDLE))
  {
    FrArTp_SM_actConn[actConnIdx].connIdx = FRARTP_CONN_INVALID;
  }
}


TS_MOD_PRIV_DEFN FUNC(FrArTp_ActConnIdxType, FRARTP_CODE) FrArTp_SM_FindOrAllocateActConnIdx
(
  FrArTp_ConnIdxType connIdx
)
{
  CONSTP2CONST(FrArTp_ChannelCfgType,AUTOMATIC,FRARTP_APPL_CONST) pChannelCfg =
      FrArTp_CfgAccess_getChannelCfgPtr_byConnIdx(connIdx);

  FrArTp_ConnIdxType start = pChannelCfg->actConnIdx;
  FrArTp_ConnIdxType end = start + pChannelCfg->numMaxActConn;
  FrArTp_ConnIdxType freeActConn = end;
  FrArTp_ConnIdxType result = FRARTP_ACTCONN_INVALID;

  /* Enter exclusive area */
  SchM_Enter_FrArTp_SCHM_FRARTP_EXCLUSIVE_AREA_0();

  for(;start < end; start++)
  {
    if((FrArTp_SM_actConn[start].connIdx == FRARTP_CONN_INVALID) && (freeActConn == end))
    {
      freeActConn = start;
    }
    else
    {
      if(FrArTp_SM_actConn[start].connIdx == connIdx)
      {
        break;
      }
    }
  }

  if(start != end)
  {
    result = start;
  }
  else
  {
    if(freeActConn != end)
    {
      result = freeActConn;
      FrArTp_SM_actConn[freeActConn].connIdx = connIdx;
      TS_AtomicAssign8(FrArTp_SM_actConn[freeActConn].rx.state, FRARTP_SM_RXSTATE_IDLE);
      TS_AtomicAssign8(FrArTp_SM_actConn[freeActConn].tx.state, FRARTP_SM_TXSTATE_IDLE);
    }
  }

  /* Exit exclusive area */
  SchM_Exit_FrArTp_SCHM_FRARTP_EXCLUSIVE_AREA_0();

  return result;
}


TS_MOD_PRIV_DEFN FUNC(Std_ReturnType,FRARTP_CODE) FrArTp_SM_changeParameter
(
  FrArTp_ConnIdxType connIdx,
  TPParameterType parameter,
  uint8 value
)
{
  Std_ReturnType RetCode = E_NOT_OK;
  FrArTp_ActConnIdxType actConnIdx = FrArTp_SM_FindActConnIdx(connIdx);
  uint8 rxState;

  if(actConnIdx != FRARTP_ACTCONN_INVALID)
  {
    TS_AtomicAssign8(rxState, FrArTp_SM_actConn[actConnIdx].rx.state);
  }

  /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00104.4,1 */
  /* Deviation TASKING-1 */
  if((actConnIdx == FRARTP_ACTCONN_INVALID) || (rxState == FRARTP_SM_RXSTATE_IDLE))
  {
    switch(parameter)
    {
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00104.2,1 */
      case TP_STMIN:
      {
        FrArTp_SM_Conn[connIdx].stMin = value;
        RetCode = E_OK;
        break;
      }
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00104.1,1 */
      case TP_BS:
      {
        FrArTp_SM_Conn[connIdx].BS = value;
        RetCode = E_OK;
        break;
      }
      /* CHECK: NOPARSE */
      default:
        FRARTP_UNREACHABLE_CODE_ASSERT(FRARTP_CHANGEPARAMETER_SERVICE_ID);
        break;
      /* CHECK: PARSE */
    }
  }

  return RetCode;
}


/*
 * Stop code section declaration
 */
#define FRARTP_STOP_SEC_CODE
#include <FrArTp_MemMap.h>
