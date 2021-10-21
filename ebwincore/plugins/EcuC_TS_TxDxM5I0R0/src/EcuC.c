/**
 * \file
 *
 * \brief AUTOSAR EcuC
 *
 * This file contains the implementation of the AUTOSAR
 * module EcuC.
 *
 * \version 5.0.21
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]=============================================*/

#include <TSMem.h>                /* TS_MemCpy(), TS_MemSet() */
#include <EcuC.h>

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

#define ECUC_START_SEC_CODE
#include <EcuC_MemMap.h>

/** \brief Enter IDLE state
 *
 * This function restarts the buffer data structures.
 *
 * \param[in] RxControlInfoPtr control information of the passed TP-receive buffer
 */
STATIC FUNC(void, ECUC_CODE) EcuC_TpReception_entry_IDLE
(
  P2VAR(EcuC_RxControlInfoType, AUTOMATIC, ECUC_APPL_DATA) RxControlInfoPtr
);

/** \brief Enter IDLE state TxConfirmation
 *
 * This function restarts the buffer data structures for transmission.
 *
 * \param[in] TxControlInfoPtr control information of the passed TPtransmit buffer
 */
STATIC FUNC(void, ECUC_CODE) EcuC_TpTransmission_entry_IDLE
(
  P2VAR(EcuC_TxControlInfoType, AUTOMATIC, ECUC_APPL_DATA) TxControlInfoPtr
);

#define ECUC_STOP_SEC_CODE
#include <EcuC_MemMap.h>

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

#define ECUC_START_SEC_CODE
#include <EcuC_MemMap.h>

/* !LINKSTO EcuC.ArchDesign.API.TpLib.EcuC_InitRx,1 */
FUNC(void, ECUC_CODE) EcuC_InitRx
(
 P2VAR(EcuC_RxControlInfoType, AUTOMATIC, ECUC_APPL_DATA) RxControlInfoPtr
)
{
  EcuC_TpReception_entry_IDLE (RxControlInfoPtr);
}

/* !LINKSTO EcuC.ArchDesign.API.TpLib.EcuC_StartOfReception,1 */
FUNC(BufReq_ReturnType, ECUC_CODE) EcuC_StartOfReception
(
  P2VAR(EcuC_RxControlInfoType, AUTOMATIC, ECUC_APPL_DATA) RxControlInfoPtr,
  P2VAR(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) RxBufferInfoPtr,
  PduLengthType TpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, ECUC_APPL_DATA) RxBufferSizePtr
)
{
  BufReq_ReturnType Result = BUFREQ_E_NOT_OK;

  switch(RxControlInfoPtr->state)
  {
    case ECUC_STATE_IDLE:
    {
      PduLengthType msgSize = 0U;

      if (TpSduLength > RxBufferInfoPtr->SduLength)
      {
        /* buffer not big enough to hold all data */
        Result = BUFREQ_E_OVFL;
      }
      else
      {
        if (TpSduLength == 0U)
        {
          /* if message length is unknown, take whole buffer size as length */
          msgSize = RxBufferInfoPtr->SduLength;
        }
        else
        {
          msgSize = TpSduLength;
        }

        RxControlInfoPtr->remainMsgSize = msgSize;
        RxControlInfoPtr->totalMsgSize = msgSize;
        RxControlInfoPtr->state = ECUC_STATE_RXONGOING;

        *RxBufferSizePtr = msgSize;

        Result = BUFREQ_OK;
      }
      break;
    }
    case ECUC_STATE_RXONGOING:
    {
      /* unexpected state, reception already ongoing */
      Result = BUFREQ_E_NOT_OK;
    }
      break;
    /* CHECK: NOPARSE */
    case ECUC_STATE_TXONGOING:  /* intended fall through */
    default: /* default case intended to be empty */
      break;
    /* CHECK: PARSE */
  }

  return Result;
}

/* !LINKSTO EcuC.ArchDesign.API.TpLib.EcuC_CopyRxData,1 */
FUNC(BufReq_ReturnType, ECUC_CODE) EcuC_CopyRxData
(
  P2VAR(EcuC_RxControlInfoType, AUTOMATIC, ECUC_APPL_DATA) RxControlInfoPtr,
  P2VAR(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) RxBufferInfoPtr,
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, ECUC_APPL_DATA) RxBufferSizePtr
)
{
  BufReq_ReturnType Result = BUFREQ_E_NOT_OK;

  switch(RxControlInfoPtr->state)
  {
    case ECUC_STATE_IDLE:
    {
      Result = BUFREQ_E_NOT_OK;
      break;
    }
    case ECUC_STATE_RXONGOING:
    {
      if (PduInfoPtr->SduLength == 0U)
      {
        /* if number of bytes to copy is 0 return BUFREQ_OK and remaining message size */
        *RxBufferSizePtr = RxControlInfoPtr->remainMsgSize;
        Result = BUFREQ_OK;
      }
      else if(PduInfoPtr->SduLength > RxControlInfoPtr->remainMsgSize )
      {
        /* if data to copy from lower to upper layer is greater
         than the expected remaining length, abort reception */
        EcuC_TpReception_entry_IDLE (RxControlInfoPtr);
        Result = BUFREQ_E_NOT_OK;
      }
      else
      {
        TS_MemCpy( &RxBufferInfoPtr->SduDataPtr[RxControlInfoPtr->totalMsgSize - RxControlInfoPtr->remainMsgSize],
                   PduInfoPtr->SduDataPtr,
                   PduInfoPtr->SduLength
                  );

        RxControlInfoPtr->remainMsgSize -= PduInfoPtr->SduLength;
        *RxBufferSizePtr = RxControlInfoPtr->remainMsgSize;
        Result = BUFREQ_OK;
      }
      break;
    }
    /* CHECK: NOPARSE */
    case ECUC_STATE_TXONGOING:  /* intended fall through */
    default: /* default case intended to be empty */
      break;
    /* CHECK: PARSE */
  }

  return Result;
}

/* !LINKSTO EcuC.ArchDesign.API.TpLib.EcuC_RxBufferIsLocked,1 */
FUNC(boolean, ECUC_CODE) EcuC_RxBufferIsLocked
(
  P2VAR(EcuC_RxControlInfoType, AUTOMATIC, ECUC_APPL_DATA) RxControlInfoPtr
)
{
  boolean Result = TRUE;

  switch(RxControlInfoPtr->state)
  {
    case ECUC_STATE_IDLE:
    {
      Result = FALSE;
      break;
    }
    case ECUC_STATE_RXONGOING:
    {
      Result = TRUE;
      break;
    }
    /* CHECK: NOPARSE */
    case ECUC_STATE_TXONGOING:  /* intended fall through */
    default: /* default case intended to be empty */
      break;
    /* CHECK: PARSE */
  }
  return Result;
}

/* !LINKSTO EcuC.ArchDesign.API.TpLib.EcuC_CopyRxDataDet,1 */
FUNC(Std_ReturnType, ECUC_CODE) EcuC_CopyRxDataDet
(
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, ECUC_APPL_DATA) RxBufferSizePtr
)
{
  Std_ReturnType Result = E_NOT_OK;

  if(PduInfoPtr == NULL_PTR)
  {
    Result = E_NOT_OK;
  }
  else if (RxBufferSizePtr == NULL_PTR)
  {
    Result = E_NOT_OK;
  }
  else if ((PduInfoPtr->SduLength > 0U) && (PduInfoPtr->SduDataPtr == NULL_PTR))
  {
    Result = E_NOT_OK;
  }
  else
  {
    Result = E_OK;
  }
  return Result;
}

/* !LINKSTO EcuC.ArchDesign.API.TpLib.EcuC_StartOfReceptionDet,1 */
FUNC(Std_ReturnType, ECUC_CODE) EcuC_StartOfReceptionDet
(
  PduLengthType TpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, ECUC_APPL_DATA) RxBufferSizePtr
)
{
  Std_ReturnType Result;
  /* all vallues allowed, value 0 stands for unknown message length */
  TS_PARAM_UNUSED(TpSduLength);

  if(RxBufferSizePtr == NULL_PTR)
  {
    Result = E_NOT_OK;
  }
  else
  {
    Result = E_OK;
  }

  return Result;
}

/* !LINKSTO EcuC.ArchDesign.API.TpLib.EcuC_TpRxIndication,1 */
FUNC(Std_ReturnType, ECUC_CODE) EcuC_TpRxIndication
(
  P2VAR(EcuC_RxControlInfoType, AUTOMATIC, ECUC_APPL_DATA) RxControlInfoPtr,
  NotifResultType Result,
  P2VAR(PduLengthType, AUTOMATIC, ECUC_APPL_DATA) CopiedDataSize
)
{
  Std_ReturnType retVal = E_NOT_OK;

  *CopiedDataSize = RxControlInfoPtr->totalMsgSize - RxControlInfoPtr->remainMsgSize;

  switch(RxControlInfoPtr->state)
  {
    case ECUC_STATE_IDLE:
    {
      retVal = E_NOT_OK;
      break;
    }
    case ECUC_STATE_RXONGOING:
    {
      if (Result == NTFRSLT_OK)
      {
        retVal = E_OK;
      }
      else
      {
        retVal = E_NOT_OK;
      }
      EcuC_TpReception_entry_IDLE (RxControlInfoPtr);
      break;
    }
    /* CHECK: NOPARSE */
    case ECUC_STATE_TXONGOING:  /* intended fall through */
    default: /* default case intended to be empty */
      break;
    /* CHECK: PARSE */
  }
  return retVal;
}

/* !LINKSTO EcuC.ArchDesign.API.TpLib.EcuC_InitTx,1 */
FUNC(void, ECUC_CODE) EcuC_InitTx
(
 P2VAR(EcuC_TxControlInfoType, AUTOMATIC, ECUC_APPL_DATA) TxControlInfoPtr
)
{
  EcuC_TpTransmission_entry_IDLE(TxControlInfoPtr);
}

/* !LINKSTO EcuC.ArchDesign.API.TpLib.EcuC_CopyTxData,1 */
FUNC(BufReq_ReturnType, ECUC_CODE) EcuC_CopyTxData
(
  P2VAR(EcuC_TxControlInfoType, AUTOMATIC, ECUC_APPL_DATA) TxControlInfoPtr,
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_CONST) TxBufferInfoPtr,
  P2VAR(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2VAR(RetryInfoType, AUTOMATIC, ECUC_APPL_DATA) RetryInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, ECUC_APPL_DATA) TxDataCntPtr
)
{
  BufReq_ReturnType retVal = BUFREQ_OK;

  if(TxControlInfoPtr->state == ECUC_STATE_TXONGOING)
  {
    PduLengthType copiedOffset = TxBufferInfoPtr->SduLength - TxControlInfoPtr->remainMsgSize;
    PduLengthType retry = 0U;

    if (RetryInfoPtr != NULL_PTR)
    {
      if(RetryInfoPtr->TpDataState == TP_DATARETRY)
      {
        /* TxTpDataCnt is not allowed to be greater than already copied data */
        if (copiedOffset < RetryInfoPtr->TxTpDataCnt)
        {
          retVal = BUFREQ_E_NOT_OK;
        }
        else
        {
          retry = RetryInfoPtr->TxTpDataCnt;
        }
      }
    }

    if(retVal == BUFREQ_OK)
    {
      /* if number of bytes to copy is greater than remaining length return buffer busy */
      if (PduInfoPtr->SduLength > (TxControlInfoPtr->remainMsgSize + retry))
      {
        retVal = BUFREQ_E_BUSY;
      }
      else
      {
        if(PduInfoPtr->SduLength > 0U)
        {
          TS_MemCpy( PduInfoPtr->SduDataPtr,
                     &TxBufferInfoPtr->SduDataPtr[copiedOffset - retry],
                     PduInfoPtr->SduLength
                   );

          /* if bytes to copy exceeds already copied data update remaining length */
          if (PduInfoPtr->SduLength > retry)
          {
            TxControlInfoPtr->remainMsgSize -= (PduInfoPtr->SduLength - retry);
          }
        }

        *TxDataCntPtr = TxControlInfoPtr->remainMsgSize;
        retVal = BUFREQ_OK;
      }
    }
  }
  else
  {
    retVal = BUFREQ_E_NOT_OK;
  }
  return retVal;
}

/* !LINKSTO EcuC.ArchDesign.API.TpLib.EcuC_TxBufferIsLocked,1 */
FUNC(boolean, ECUC_CODE) EcuC_TxBufferIsLocked
(
  P2VAR(EcuC_TxControlInfoType, AUTOMATIC, ECUC_APPL_DATA) TxControlInfoPtr
)
{
  boolean retVal = FALSE;
  switch(TxControlInfoPtr->state)
  {
    case ECUC_STATE_IDLE:
        retVal = FALSE;
        break;
    case ECUC_STATE_TXONGOING:
        retVal = TRUE;
        break;
    /* CHECK: NOPARSE */
    case ECUC_STATE_RXONGOING:
    default: /* default case intended to be empty */
        break;
    /* CHECK: PARSE */
  }
  return retVal;
}

/* !LINKSTO EcuC.ArchDesign.API.TpLib.EcuC_TpTransmit,1 */
FUNC(Std_ReturnType, ECUC_CODE) EcuC_TpTransmit
(
  P2VAR(EcuC_TxControlInfoType, AUTOMATIC, ECUC_APPL_DATA) TxControlInfoPtr,
  PduIdType PduId,
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_CONST) TxBufferInfoPtr,
  EcuC_LoTpTransmitFunctPtrType LoTpTransmitFunctPtr
)
{
  Std_ReturnType retVal = E_NOT_OK;

  if(TxControlInfoPtr->state == ECUC_STATE_TXONGOING)
  {
    retVal = E_NOT_OK;
  }
  else
  {
    if(LoTpTransmitFunctPtr != NULL_PTR)
    {
      /* Update ControlInfo for successful TpTransmit call in advance */
      TxControlInfoPtr->remainMsgSize = TxBufferInfoPtr->SduLength;
      TxControlInfoPtr->state = ECUC_STATE_TXONGOING;

      /* Call TpTransmit */
      retVal = (LoTpTransmitFunctPtr)(PduId, TxBufferInfoPtr);

      /* roll back in case of erroneous TpTransmit call */
      if(retVal != E_OK)
      {
        EcuC_TpTransmission_entry_IDLE(TxControlInfoPtr);
      }
    }
  }

  return retVal;
}

/* !LINKSTO EcuC.ArchDesign.API.TpLib.EcuC_TpTxConfirmation,1 */
FUNC(Std_ReturnType, ECUC_CODE) EcuC_TpTxConfirmation
(
  P2VAR(EcuC_TxControlInfoType, AUTOMATIC, ECUC_APPL_DATA) TxControlInfoPtr,
  NotifResultType Result
)
{
  Std_ReturnType retVal;
  if(Result == NTFRSLT_OK)
  {
    if(TxControlInfoPtr ->state == ECUC_STATE_IDLE)
    {
      retVal = E_NOT_OK;
    }
    else
    {
      retVal = E_OK;
    }
  }
  else
  {
    retVal = E_NOT_OK;
  }
  EcuC_TpTransmission_entry_IDLE(TxControlInfoPtr);
  return retVal;
}

/*==================[internal function definitions]==========================*/

STATIC FUNC(void, ECUC_CODE) EcuC_TpReception_entry_IDLE
(
  P2VAR(EcuC_RxControlInfoType, AUTOMATIC, ECUC_APPL_DATA) RxControlInfoPtr
)
{
  TS_MemBZero(RxControlInfoPtr, sizeof(EcuC_RxControlInfoType));
}

STATIC FUNC(void, ECUC_CODE) EcuC_TpTransmission_entry_IDLE
(
  P2VAR(EcuC_TxControlInfoType, AUTOMATIC, ECUC_APPL_DATA) TxControlInfoPtr
)
{
  TS_MemBZero(TxControlInfoPtr, sizeof(EcuC_TxControlInfoType));
}

#define ECUC_STOP_SEC_CODE
#include <EcuC_MemMap.h>

/*==================[end of file]============================================*/

