/**
 * \file
 *
 * \brief AUTOSAR PduR
 *
 * This file contains the implementation of the AUTOSAR
 * module PduR.
 *
 * \version 5.3.46
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*
 * Sequence of API calls for the wrapper, e.g. for CopyRxData:
 *
 *   PduR_UpCopyRxData          (generated wrapper function, invoked when calling upper layer of
 *           |                   AUTOSAR 3.2)
 *           V
 *   PduR_WrapASR32CopyRxData   (generic wrapper function called within PduR_UpCopyRxData)
 *           |
 *           V
 *   Up_ProvideRxBuffer         (called within generic wrapper function,
 *                               provided by the upper layer's property file)
 */

/*==================[inclusions]============================================*/
#include <PduR_Trace.h>
#include <TSMem.h>                  /* TS_MemCpy */

#include <PduR_Cfg.h>
#if (PDUR_AUTOSAR32_SUPPORT == STD_ON)
#include <PduR_WrapASR32.h>         /* Generic wrapper to AUTOSAR 3.2 upper layer */
#include <PduR_Internal_Static.h>   /* Internal API (static part) */

/*==================[version check]=========================================*/

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/
/* AUTOSAR Memory Mapping - start section for code */
#if (TS_MERGED_COMPILE == STD_OFF)
#define PDUR_START_SEC_CODE
#include <PduR_MemMap.h>
#endif

FUNC(BufReq_ReturnType, PDUR_CODE) PduR_WrapASR32StartOfReception
(
   PduIdType RxPduId,
   PduLengthType TpSduLength,
   P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) AvailableBufferSizePtr,
   PduR_ProvRxBufFpType ProvideRxBufferFp,
   PduR_TpRxIndFpType RxIndicationFp,
   P2VAR(PduR_WrapASR32RxSessionInfoType, AUTOMATIC, PDUR_APPL_DATA) RxSessionInfoPtr
)
{
   BufReq_ReturnType RetVal = BUFREQ_E_NOT_OK;
   P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) UpTpPduInfoPtr;

   DBG_PDUR_WRAPASR32STARTOFRECEPTION_ENTRY(RxPduId, TpSduLength, AvailableBufferSizePtr, ProvideRxBufferFp, RxIndicationFp, RxSessionInfoPtr);

   /* handle unknown message length */
   if(TpSduLength == PDUR_TP_SDU_LENGTH_UNKNOWN)
   {
      /* unknown message length is not supported by AUTOSAR 3.2 */
      RetVal = BUFREQ_E_OVFL;
   }
   else
   {
      /* first call of ProvideRxBuffer */
      RetVal = ProvideRxBufferFp(RxPduId, TpSduLength, &UpTpPduInfoPtr);

      /* update available buffer size of upper layer */
      if(RetVal != BUFREQ_OK)
      {
#if (PDUR_AUTOSAR32_REVISION_COMPATIBILITY == STD_ON)
         if(RetVal != BUFREQ_E_BUSY)
         {
            /* no RxIndication is provided by ASR 4.0 for failing StartOfReception */
            RxIndicationFp(RxPduId, NTFRSLT_E_NOT_OK);
         }
         else
         {
            /* store necessary session data for BUFREQ_E_BUSY, because
               Up_ProvideRxBufferFp has to be called with correct TpSduLength each time */
            RxSessionInfoPtr->TpSduLength = TpSduLength;
         }
#else
         /* did not get a new buffer */
         RetVal = BUFREQ_E_OVFL;
         /* no RxIndication is provided by ASR 4.0 for failing StartOfReception */
         RxIndicationFp(RxPduId, NTFRSLT_E_NOT_OK);
#endif
      }
      else
      {
         /* return BufferSize */
         *AvailableBufferSizePtr = UpTpPduInfoPtr->SduLength;

         /* store session data */
         RxSessionInfoPtr->BufferPtr = UpTpPduInfoPtr->SduDataPtr;
         RxSessionInfoPtr->TpSduLength = TpSduLength;
         RxSessionInfoPtr->AvailableBufferSize = UpTpPduInfoPtr->SduLength;
      }
   }

   DBG_PDUR_WRAPASR32STARTOFRECEPTION_EXIT(RetVal, RxPduId, TpSduLength, AvailableBufferSizePtr, ProvideRxBufferFp, RxIndicationFp, RxSessionInfoPtr);

   return RetVal;
}


FUNC(BufReq_ReturnType, PDUR_CODE) PduR_WrapASR32CopyRxData
(
   PduIdType RxPduId,
   P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) PduInfoPtr,
   P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) AvailableBufferSizePtr,
   PduR_ProvRxBufFpType ProvideRxBufferFp,
   P2VAR(PduR_WrapASR32RxSessionInfoType, AUTOMATIC, PDUR_APPL_DATA) RxSessionInfoPtr
)
{
   BufReq_ReturnType RetVal = BUFREQ_E_NOT_OK;
   P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) UpTpPduInfoPtr;
   PduLengthType AvailableBufferSize = RxSessionInfoPtr->AvailableBufferSize;

   DBG_PDUR_WRAPASR32COPYRXDATA_ENTRY(RxPduId, PduInfoPtr, AvailableBufferSizePtr, ProvideRxBufferFp, RxSessionInfoPtr);

   /* handle request for currently available buffer size */
   if(PduInfoPtr->SduLength == PDUR_SDU_LENGTH_ZERO)
   {
      /* Already filled up buffer */
      if(AvailableBufferSize == 0U)
      {
         /* call Up_ProvideRxBuffer */
         RetVal = ProvideRxBufferFp(RxPduId, RxSessionInfoPtr->TpSduLength, &UpTpPduInfoPtr);

         /* update session information and available buffer size when Up_ProvideRxBuffer is
          * called successfully */
         if(RetVal == BUFREQ_OK)
         {
            /* store session information */
            RxSessionInfoPtr->BufferPtr = UpTpPduInfoPtr->SduDataPtr;
            RxSessionInfoPtr->AvailableBufferSize = UpTpPduInfoPtr->SduLength;
            AvailableBufferSize = UpTpPduInfoPtr->SduLength;

            /* update available buffer size */
            *AvailableBufferSizePtr = AvailableBufferSize;
         }
         else
         {
#if (PDUR_AUTOSAR32_REVISION_COMPATIBILITY == STD_ON)
            /* did not get a proper buffer, passing RetVal unchanged */
#else
            /* did not get a proper buffer,
               BUFREQ_E_BUSY is not handled on Rx side by lower layer.
               ASR 4.x provides only BUFREQ_E_NOT_OK aside BUFREQ_OK. */
            RetVal = BUFREQ_E_NOT_OK;
#endif
         }

      }
      else
      {
         RetVal = BUFREQ_OK;

         /* update available buffer size */
         *AvailableBufferSizePtr = AvailableBufferSize;
      }
   }
#if (PDUR_DEV_ERROR_DETECT == STD_ON)
   /* check input pointer */
   else if (PduInfoPtr->SduDataPtr == NULL_PTR)
   {
      PDUR_DET_REPORT_ERROR(PDUR_SID_LOTP_COPY_RX_DATA, PDUR_E_NULL_POINTER);
      RetVal = BUFREQ_E_NOT_OK;
   }
#endif /* #if (PDUR_DEV_ERROR_DETECT == STD_ON) */
   /* Up_ProvideRxBuffer shall be called to copy data */
   else if (AvailableBufferSize == 0U)
   {
      /* call Up_ProvideRxBuffer */
      RetVal = ProvideRxBufferFp(RxPduId, RxSessionInfoPtr->TpSduLength, &UpTpPduInfoPtr);

      /* proper buffer provided from upper layer */
      if((PduInfoPtr->SduLength <= UpTpPduInfoPtr->SduLength) &&
         (RetVal == BUFREQ_OK))
      {
         /* copy data */
         TS_MemCpy(RxSessionInfoPtr->BufferPtr, PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);

         /* update available buffer size */
         AvailableBufferSize = UpTpPduInfoPtr->SduLength - PduInfoPtr->SduLength;
         *AvailableBufferSizePtr = AvailableBufferSize;

         /* update pointer to new position of upper layer buffer */
         RxSessionInfoPtr->BufferPtr = &RxSessionInfoPtr->BufferPtr[PduInfoPtr->SduLength];
      }
      else
      {
#if (PDUR_AUTOSAR32_REVISION_COMPATIBILITY == STD_ON)
         /* PduInfoPtr->SduLength > UpTpPduInfoPtr->SduLength */
         if (RetVal != BUFREQ_E_BUSY)
         {
            RetVal = BUFREQ_E_NOT_OK;
         }
#else
         /* did not get a proper buffer,
           BUFREQ_E_BUSY is not handled on Rx side by lower layer.
           ASR 4.x provides only BUFREQ_E_NOT_OK aside BUFREQ_OK. */
         RetVal = BUFREQ_E_NOT_OK;
#endif
       }
   }
   /* some data remain for copying (without calling ProvideRxBuffer) */
   else
   {
      /* provided data exceeds size of the upper layer's buffer */
      if((PduInfoPtr->SduLength > AvailableBufferSize))
      {
         /* did not get a proper buffer */
         RetVal = BUFREQ_E_NOT_OK;
      }
      /* provided data fits into upper layer's buffer */
      else
      {
         /* copy data */
         TS_MemCpy(RxSessionInfoPtr->BufferPtr, PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);

         /* update available buffer size */
         AvailableBufferSize -= PduInfoPtr->SduLength;
         *AvailableBufferSizePtr = AvailableBufferSize;

         /* update pointer to buffer */
         RxSessionInfoPtr->BufferPtr = &RxSessionInfoPtr->BufferPtr[PduInfoPtr->SduLength];

         RetVal = BUFREQ_OK;
      }
   }

   /* set session value */
   RxSessionInfoPtr->AvailableBufferSize = AvailableBufferSize;

   DBG_PDUR_WRAPASR32COPYRXDATA_EXIT(RetVal, RxPduId,PduInfoPtr, AvailableBufferSizePtr, ProvideRxBufferFp, RxSessionInfoPtr);

   return RetVal;
}



FUNC(void, PDUR_CODE) PduR_WrapASR32RxIndication
(
   PduIdType RxPduId,
   NotifResultType Result,
   PduR_TpRxIndFpType RxIndicationFp,
   P2VAR(PduR_WrapASR32RxSessionInfoType, AUTOMATIC, PDUR_APPL_DATA) RxSessionInfoPtr
)
{
   DBG_PDUR_WRAPASR32RXINDICATION_ENTRY(RxPduId, Result, RxIndicationFp, RxSessionInfoPtr);

   RxIndicationFp(RxPduId, Result);

   /* reset session data */
   RxSessionInfoPtr->BufferPtr = NULL_PTR;
   RxSessionInfoPtr->TpSduLength = 0U;
   RxSessionInfoPtr->AvailableBufferSize = 0U;

   DBG_PDUR_WRAPASR32RXINDICATION_EXIT(RxPduId, Result, RxIndicationFp, RxSessionInfoPtr);
}



FUNC(BufReq_ReturnType, PDUR_CODE) PduR_WrapASR32CopyTxData
(
   PduIdType TxPduId,
   P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) PduInfoPtr,
   P2VAR(RetryInfoType, AUTOMATIC, PDUR_APPL_DATA) RetryInfoPtr,
   P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) AvailableDataSizePtr,
   PduR_ProvTxBufFpType ProvideTxBufferFp,
   P2VAR(PduR_WrapASR32TxSessionInfoType, AUTOMATIC, PDUR_APPL_DATA) TxSessionInfoPtr
)
{
   BufReq_ReturnType RetVal = BUFREQ_E_NOT_OK;
   P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) UpTpPduInfoPtr;
   PduLengthType AvailableDataSize = TxSessionInfoPtr->AvailableDataSize;

   DBG_PDUR_WRAPASR32COPYTXDATA_ENTRY(TxPduId, PduInfoPtr, RetryInfoPtr, AvailableDataSizePtr, ProvideTxBufferFp, TxSessionInfoPtr);

   /* handle request for available data size */
   if(PduInfoPtr->SduLength == PDUR_SDU_LENGTH_ZERO)
   {
      /* first call of provide TxBuffer or already emptied data */
      if(AvailableDataSize == 0U)
      {
         /* call ProvideTxBuffer (Length: called with SduLength since this special value is
          * provided by the lower module. 0 might be used as well possibly) */
         RetVal = ProvideTxBufferFp(TxPduId, &UpTpPduInfoPtr, PduInfoPtr->SduLength);

         /* update session information and available buffer size when Up_ProvideTxBuffer is
          * called successfully */
         if(RetVal == BUFREQ_OK)
         {
            /* store session data */
            TxSessionInfoPtr->BufferBeginPtr = UpTpPduInfoPtr->SduDataPtr;
            TxSessionInfoPtr->BufferSize = UpTpPduInfoPtr->SduLength;
            AvailableDataSize = UpTpPduInfoPtr->SduLength;

            /* update available data size */
            *AvailableDataSizePtr = AvailableDataSize;
         }
         /* otherwise the RetVal from Up_ProvideTxBuffer is returned */

      }
      else
      {
         RetVal = BUFREQ_OK;

         /* update available data size */
         *AvailableDataSizePtr = AvailableDataSize;
      }
   }
#if (PDUR_DEV_ERROR_DETECT == STD_ON)
   /* check input pointer */
   else if (PduInfoPtr->SduDataPtr == NULL_PTR)
   {
      PDUR_DET_REPORT_ERROR(PDUR_SID_LOTP_COPY_TX_DATA, PDUR_E_NULL_POINTER);
      RetVal = BUFREQ_E_NOT_OK;
   }
#endif /* #if (PDUR_DEV_ERROR_DETECT == STD_ON) */
   /* handle Retry */
   else if((RetryInfoPtr != NULL_PTR) &&
           (RetryInfoPtr->TpDataState == TP_DATARETRY))
   {
      /* rewind by TxTpDataCnt is possible */
      if((RetryInfoPtr->TxTpDataCnt + AvailableDataSize) <=
          TxSessionInfoPtr->BufferSize
        )
      {
         /* enough data provided for the buffer */
         if((AvailableDataSize + RetryInfoPtr->TxTpDataCnt) >=
            PduInfoPtr->SduLength
           )
         {
            PduLengthType CurDataIdx = TxSessionInfoPtr->BufferSize
                                       - AvailableDataSize
                                       - RetryInfoPtr->TxTpDataCnt;

            /* copy all data from rewinded position */
            TS_MemCpy(PduInfoPtr->SduDataPtr,
                      &TxSessionInfoPtr->BufferBeginPtr[CurDataIdx],
                      PduInfoPtr->SduLength);

            /* update available data size */
            AvailableDataSize -= (PduInfoPtr->SduLength - RetryInfoPtr->TxTpDataCnt);

            RetVal = BUFREQ_OK;
         }
         /* provided buffer exceeds size of data to be copied */
         else
         {
            RetVal = BUFREQ_E_BUSY;
         }

         /* update available buffer size */
         *AvailableDataSizePtr = AvailableDataSize;
      }
      /* rewind by TxTpDataCnt exceeds buffer border */
      else
      {
         RetVal = BUFREQ_E_NOT_OK;
      }
   }
   /* Up_ProvideTxBuffer shall be called to copy data */
   else if (AvailableDataSize == 0U)
   {
      /* call ProvideTxBuffer (Length: called with SduLength since this special value is
       * provided by the lower module. 0 might be used as well possibly) */
      RetVal = ProvideTxBufferFp(TxPduId, &UpTpPduInfoPtr, PduInfoPtr->SduLength);

      /* ProvideTxBuffer is called successfully */
      if(RetVal == BUFREQ_OK)
      {
         /* data is passed to lower layer buffer */
         if(UpTpPduInfoPtr->SduLength >= PduInfoPtr->SduLength)
         {
            /* copy data */
            TS_MemCpy(PduInfoPtr->SduDataPtr, UpTpPduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);

            /* store session data */
            TxSessionInfoPtr->BufferBeginPtr = UpTpPduInfoPtr->SduDataPtr;
            TxSessionInfoPtr->BufferSize = UpTpPduInfoPtr->SduLength;
            AvailableDataSize = UpTpPduInfoPtr->SduLength - PduInfoPtr->SduLength;

            RetVal = BUFREQ_OK;
         }
         /* data provided is less than lower layer's buffer size */
         else
         {
            RetVal = BUFREQ_E_BUSY;

            /* store session data */
            TxSessionInfoPtr->BufferBeginPtr = UpTpPduInfoPtr->SduDataPtr;
            TxSessionInfoPtr->BufferSize = UpTpPduInfoPtr->SduLength;
            AvailableDataSize = UpTpPduInfoPtr->SduLength;
         }

         /* update available data size */
         *AvailableDataSizePtr = AvailableDataSize;
      }
      /* otherwise the RetVal from Up_ProvideTxBuffer is returned */

   }
   /* some data remain for copying (without calling Up_ProvideTxBuffer) */
   else
   {
      /* data is passed to lower layer buffer */
      if(AvailableDataSize >= PduInfoPtr->SduLength)
      {
         PduLengthType CurDataIdx = TxSessionInfoPtr->BufferSize - AvailableDataSize;

         /* copy data */
         TS_MemCpy(PduInfoPtr->SduDataPtr,
                   &TxSessionInfoPtr->BufferBeginPtr[CurDataIdx],
                   PduInfoPtr->SduLength);

         /* update session data */
         AvailableDataSize -= PduInfoPtr->SduLength;

         RetVal = BUFREQ_OK;
      }
      /* data provided is less than lower layer's buffer size */
      else
      {
         RetVal = BUFREQ_E_BUSY;
      }

      /* update available buffer size */
      *AvailableDataSizePtr = AvailableDataSize;
   }

   /* set session value */
   TxSessionInfoPtr->AvailableDataSize = AvailableDataSize;

   DBG_PDUR_WRAPASR32COPYTXDATA_EXIT(RetVal, TxPduId, PduInfoPtr, RetryInfoPtr, AvailableDataSizePtr, ProvideTxBufferFp, TxSessionInfoPtr);

   return RetVal;
}



FUNC(void, PDUR_CODE) PduR_WrapASR32TxConfirmation
(
   PduIdType TxPduId,
   NotifResultType Result,
   PduR_TpTxConfFpType TxConfirmationFp,
   P2VAR(PduR_WrapASR32TxSessionInfoType, AUTOMATIC, PDUR_APPL_DATA) TxSessionInfoPtr
)
{
   DBG_PDUR_WRAPASR32TXCONFIRMATION_ENTRY(TxPduId, Result, TxConfirmationFp, TxSessionInfoPtr);

   TxConfirmationFp(TxPduId, Result);

   /* reset session data */
   TxSessionInfoPtr->BufferBeginPtr = NULL_PTR;
   TxSessionInfoPtr->BufferSize = 0U;
   TxSessionInfoPtr->AvailableDataSize = 0U;

   DBG_PDUR_WRAPASR32TXCONFIRMATION_EXIT(TxPduId, Result, TxConfirmationFp, TxSessionInfoPtr);
}

/*==================[internal function definitions]=========================*/

/* AUTOSAR Memory Mapping - end section for code */
#if (TS_MERGED_COMPILE == STD_OFF)
#define PDUR_STOP_SEC_CODE
#include <PduR_MemMap.h>
#endif

#else /* if (PDUR_AUTOSAR32_SUPPORT == STD_ON) */

#include <TSCompiler.h>           /* usage of macro in TSCompiler_Default.h */

/* Avoid empty translation unit according to ISO C90 */
TS_PREVENTEMPTYTRANSLATIONUNIT

#endif /* if (PDUR_AUTOSAR32_SUPPORT == STD_ON)  */
/*==================[end of file]===========================================*/
