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
 *  MISRA-C:2012 Deviation List
 *
 *  MISRAC2012-1) Deviated Rule: 11.5 (advisory)
 *  A conversion should not be performed from pointer to void into pointer to
 *  object.
 *
 *  Reason:
 *  The memory routines are optimized for dealing with aligned memory sections.
 *
 */

/*==================[inclusions]============================================*/

#include <PduR_Trace.h>
#include <TSAutosar.h>            /* EB specific standard types */
#include <TSAtomic_Assign.h>      /* Atomic assignments from Base PlugIn */
#include <TSMem.h>                /* TS_MemCpy */
#include <ComStack_Types.h>       /* AUTOSAR Communication Stack types */
#include <PduR_Internal_Static.h> /* Internal API (static part) */
#include <PduR_Api_Depend.h>      /* Public API (depend part) */

#if ((PDUR_ZERO_COST_OPERATION_TP == STD_OFF) && (PDUR_TPGATEWAY_SUPPORT == STD_ON))
#include <SchM_PduR.h>            /* Schedule Manager for module PduR */
/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/* AUTOSAR Memory Mapping - start section for code */
#if (TS_MERGED_COMPILE == STD_OFF)
#define PDUR_START_SEC_CODE
#include <PduR_MemMap.h>
#endif

/**
 ** \brief Auxiliary Tp gateway handler function
 **
 ** Auxiliary function that copies data from the allocated TP buffer
 ** to the provided buffer of the lower layer TP module for routing on-the-fly.
 ** The allocated TP buffer is handled as a circular buffer.
 ** The TP gateway session member AvailableBufferSize is updated accordingly.
 **
 ** \param[in] SessionIndex        Index of considered TP gateway session.
 ** \param[in] PduInfoPtr          Pointer to the buffer (SduDataPtr) and its length (SduLength)
 **                                of the lower TP module where the data shall be copied to.
 **
 ** \return Result of request to copy available data to lower layer TP module's buffer.
 ** \retval BUFREQ_E_NOT_OK: Pointer to buffer of lower TP module is set to null for enabled DET.
 ** \retval BUFREQ_E_BUSY:   Size of buffer of the lower TP module exceeds available size of
 **                          data provided by the allocated TP buffer.
 ** \retval BUFREQ_OK:       Data to be transmitted are copied successfully to the lower TP
 **                          module's buffer.
 **/
STATIC FUNC(BufReq_ReturnType, PDUR_CODE) PduR_CopyDataFromCircularBuffer
(
   PduR_GateTpSessionIndexType SessionIndex,
   P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) PduInfoPtr
);

/**
 ** \brief Auxiliary Tp gateway handler function
 **
 ** Auxiliary function that copies data from the allocated TP buffer
 ** to the provided buffer of the lower layer TP module for direct gateway
 ** with possible retry mechanism.
 ** The allocated TP buffer is handled as a linear buffer.
 ** The TP gateway session member AvailableBufferSize is updated accordingly.
 **
 ** \param[in] SessionIndex        Index of considered TP gateway session.
 ** \param[in] PduInfoPtr          Pointer to the buffer (SduDataPtr) and its length (SduLength) of
 **                                the lower TP module where the available data shall be copied to.
 ** \param[in] TxTpDataCnt         Data to be copied again for retry.
 ** \param[in] IsMulticastPdu      I-PDU is transmitted to multiple destinations.
 **
 ** \return Result of request to copy available data to lower layer TP module's buffer.
 ** \retval BUFREQ_E_NOT_OK: Pointer to buffer of lower TP module is set to null for enabled DET or
 **                          an incorrect retry size TxTpDataCnt is considered.
 ** \retval BUFREQ_E_BUSY:   Size of buffer of the lower TP module exceeds available size of
 **                          data provided by the allocated TP buffer including the data
 **                          for retransmission.
 **                          Multicast I-PDU is requested not at once (limited to single frames).
 ** \retval BUFREQ_OK:       Data to be transmitted are copied successfully to the lower TP
 **                          module's buffer.
 **/
STATIC FUNC(BufReq_ReturnType, PDUR_CODE) PduR_CopyDataFromLinearBuffer
(
   PduR_GateTpSessionIndexType SessionIndex,
   P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) PduInfoPtr,
   PduR_PduLengthType TxTpDataCnt,
   boolean IsMulticastPdu
);

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

FUNC(BufReq_ReturnType, PDUR_CODE) PduR_GateTpCopyTxData
(
   PduIdType GMPduId,
   P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) PduInfoPtr,
   P2VAR(RetryInfoType, AUTOMATIC, PDUR_APPL_DATA) RetryInfoPtr,
   P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) AvailableDataPtr
)
{
   /* initialize return value */
   BufReq_ReturnType RetVal = BUFREQ_E_NOT_OK;

   /* get TP gateway session in use */
   PduR_GateTpSessionIndexType SessionIndex;

#if (PDUR_DEV_ERROR_DETECT == STD_ON)
   boolean CallDetReportError = FALSE;
   uint8 DetErrorCode = PDUR_E_INVALID_REQUEST;
#endif /* if (PDUR_DEV_ERROR_DETECT == STD_ON) */

   DBG_PDUR_GATETPCOPYTXDATA_ENTRY(GMPduId, PduInfoPtr, RetryInfoPtr, AvailableDataPtr);

   /* ENTER CRITICAL SECTION */
   SchM_Enter_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

   /* get TP gateway session in use */
   SessionIndex = PduR_GetGateTpSessionInUse(GMPduId, PDUR_GATETP_QUEUE_ELEMENT_TAIL);

   /* CopyTxData called without preceding StartOfReception */
#if (PDUR_DEV_ERROR_DETECT == STD_ON)
   if(SessionIndex == PDUR_NO_GATETP_SESSION_ASSIGNED)
   {
      CallDetReportError = TRUE;
      DetErrorCode = PDUR_E_INVALID_REQUEST;
   }
   else
#endif /* if (PDUR_DEV_ERROR_DETECT == STD_ON) */
   {
      /* pointer to TP gateway sessions */
      /* Deviation MISRAC2012-1 */
      CONSTP2VAR(PduR_GateTpSessionInfoType, AUTOMATIC, PDUR_VAR_CLEARED) pSessionInfos =
         PDUR_GET_RAM_ADDR(PduR_GateTpSessionInfoType, PduR_GConfigPtr->PduR_GateTpSessionInfoRef);

      uint8 ResultGetBitAbort;
      uint8 ResultGetBitTxComplete;

      /* pointer to routing table */
      CONSTP2CONST(PduR_GTabTpRxType, AUTOMATIC, PDUR_APPL_CONST) pGTabTpRx =
         PDUR_GET_CONFIG_ADDR(PduR_GTabTpRxType, PduR_GConfigPtr->PduR_GTabTpConfig.GTabRxRef);

      /* set available DataSize to initial value,
         that will be overwritten later on if necessary */
      *AvailableDataPtr = (PduLengthType)PduR_CalcAvailableDataSize(SessionIndex);

      /* Tp gateway shall be aborted */
      TS_GetBit(&pSessionInfos[SessionIndex].Status, PDUR_GATETP_ABORT_POS, uint8,
                ResultGetBitAbort);

      /* check if all LoTp_Transmit are called */
      TS_GetBit(&pSessionInfos[SessionIndex].Status, PDUR_GATETP_TX_COMPLETE_POS, uint8,
                ResultGetBitTxComplete);

      if(ResultGetBitAbort > 0U)
      {
         /* Reception might have failed, but at least one TxConfirmation is pending,
            BUFREQ_E_NOT_OK from initialization is returned */
      }
      /* data is not available until all LoTp_Transmit are called */
      else if(ResultGetBitTxComplete == 0U)
      {
         /* The LoTp shall wait until all other calls to LoTp_Transmit are completed.
          * This eliminates any race conditions caused by PduR_GateTpCopyTxData preempting
          * PduR_GateTpRxIndication. It also holds for the request of available data with
          * SduLength 0, since basically NOT available when subsequent CopyTxData will fetch
          * data in the same cycle. */

         RetVal = BUFREQ_E_BUSY;
      }
      /* for the below conditions, all Transmit calls are completed (either successfully or not) */
      /* return available length of data in TP buffer */
      else if(PduInfoPtr->SduLength == (PduLengthType)PDUR_SDU_LENGTH_ZERO)
      {
         RetVal = BUFREQ_OK;
      }
      /* direct gateway */
      else if(pGTabTpRx[GMPduId].TpThreshold == PDUR_GATETPTHRESHOLD_DIRECT)
      {
         PduR_PduLengthType RetrySize;

         boolean IsMulticastPdu;

         /*  multicast gateway transmission configured */
         if(pGTabTpRx[GMPduId].MoreEntries != PDUR_LAST_MULTICAST_ENTRY_OF_PDUID)
         {
#if (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON)
           /* only one destination remains enabled for a multicast gateway path, others are
              disabled by routing path groups (considers also disabling after the LoTp_Transmit
              calls are completed, i.e. for already ongoing transmission at Tx side with the
              enabled destination PDU handled first) */
           if( (PduR_RPathGroupNumTpGwDestEnabled(GMPduId) == PDUR_RPATHGROUP_NUM_ONE_DEST_ENABLED) &&
               (pSessionInfos[SessionIndex].PendTxConfCount == 1U) )
           {
             /* configured multicast PDU for single frames can be considered as multiframe PDU
                at single cast */
             IsMulticastPdu = FALSE;
           }
           else
#endif
           {
             IsMulticastPdu = TRUE;
           }
         }
         else
         {
            IsMulticastPdu = FALSE;
         }

         /* retry is supported */
         if((RetryInfoPtr != NULL_PTR) &&
            (RetryInfoPtr->TpDataState == TP_DATARETRY))
         {
            RetrySize = RetryInfoPtr->TxTpDataCnt;
         }
         else
         {
            RetrySize = PDUR_SDU_LENGTH_ZERO;
         }

#if (PDUR_DEV_ERROR_DETECT == STD_ON)
         /* check if input parameter member is NULL */
         if(PduInfoPtr->SduDataPtr == NULL_PTR)
         {
            CallDetReportError = TRUE;
            DetErrorCode = PDUR_E_NULL_POINTER;
            /* RetVal from initialization is returned */
         }
         else
#endif /* #if (PDUR_DEV_ERROR_DETECT == STD_ON) */
         {
            /* copy available data from TP buffer of PduR to buffer of LoTp */
            RetVal = PduR_CopyDataFromLinearBuffer(SessionIndex, PduInfoPtr, RetrySize,
                                                   IsMulticastPdu);
         }

         /* return available size of data */
         if((IsMulticastPdu) &&
            (RetVal == BUFREQ_OK))
         {
            /* for multiple activated lower layer TP destinations only single frames are handled */
            *AvailableDataPtr = PDUR_SDU_LENGTH_ZERO;
         }
         else
         {
            *AvailableDataPtr = (PduLengthType)PduR_CalcAvailableDataSize(SessionIndex);
         }
      }
      /* routing on the fly */
      else
      {
         /* retry is not supported */
         if((RetryInfoPtr != NULL_PTR) &&
            (RetryInfoPtr->TpDataState != TP_DATACONF))
         {
            TS_AtomicSetBit_8(&pSessionInfos[SessionIndex].Status, PDUR_GATETP_ABORT_POS);

            /* BUFREQ_E_NOT_OK from initialization is returned */

            *AvailableDataPtr = (PduLengthType)PDUR_SDU_LENGTH_ZERO;
         }
         else
         {
#if (PDUR_DEV_ERROR_DETECT == STD_ON)
            /* check if input parameter member is NULL */
            if(PduInfoPtr->SduDataPtr == NULL_PTR)
            {
               CallDetReportError = TRUE;
               DetErrorCode = PDUR_E_NULL_POINTER;
               /* RetVal from initialization is returned */
            }
            else
#endif /* #if (PDUR_DEV_ERROR_DETECT == STD_ON) */
            {
               /* copy data from circular buffer of PduR to buffer of LoTp */
               RetVal = PduR_CopyDataFromCircularBuffer(SessionIndex, PduInfoPtr);

               /* return available size of data */
               *AvailableDataPtr = (PduLengthType)PduR_CalcAvailableDataSize(SessionIndex);
            }
         }
      }
   } /* if(SessionIndex != PDUR_NO_GATETP_SESSION_ASSIGNED) */

   /* EXIT CRITICAL SECTION */
   SchM_Exit_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

   /* Conglomerate DET handling */
#if (PDUR_DEV_ERROR_DETECT == STD_ON)
   if(CallDetReportError)
   {
      PDUR_DET_REPORT_ERROR(PDUR_SID_LOTP_COPY_TX_DATA, DetErrorCode);
   }
#endif /* if (PDUR_DEV_ERROR_DETECT == STD_ON) */

   DBG_PDUR_GATETPCOPYTXDATA_EXIT(RetVal, GMPduId, PduInfoPtr, RetryInfoPtr, AvailableDataPtr);

   return RetVal;
}


/*==================[internal function definitions]=========================*/

STATIC FUNC(BufReq_ReturnType, PDUR_CODE) PduR_CopyDataFromCircularBuffer
(
   PduR_GateTpSessionIndexType SessionIndex,
   P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) PduInfoPtr
)
{
   /* initialize return value */
   BufReq_ReturnType RetVal = BUFREQ_E_NOT_OK;

   const PduR_BufTpIndexType BufTpIndex = (PduR_BufTpIndexType)SessionIndex;

   /* pointer to TP gateway sessions */
   /* Deviation MISRAC2012-1 */
   CONSTP2VAR(PduR_GateTpSessionInfoType, AUTOMATIC, PDUR_VAR_CLEARED) pGateTpSessions =
      PDUR_GET_RAM_ADDR(PduR_GateTpSessionInfoType, PduR_GConfigPtr->PduR_GateTpSessionInfoRef);

   /* pointer to TP buffer configurations */
   CONSTP2CONST(PduR_BufTpConfigType, AUTOMATIC, PDUR_APPL_CONST) pBufTpConfig =
      PDUR_GET_CONFIG_ADDR(PduR_BufTpConfigType, PduR_GConfigPtr->PduR_BufTpConfigRef);

   /* index to the first element of the TP buffer where to write the data by reception */
   const PduR_PduLengthType RxWriteIndex = pGateTpSessions[SessionIndex].RxWriteIndex;

   /* length of the associated TP buffer (used also to determine TxReadIndex) */
   const PduR_PduLengthType BufTpLength = pBufTpConfig[BufTpIndex].Length;

   /* pointer to the first element of the TP buffer */
   /* Deviation MISRAC2012-1 */
   CONSTP2VAR(uint8, AUTOMATIC, PDUR_VAR_CLEARED) pStartTpBuf =
      PDUR_GET_RAM_ADDR(uint8, pBufTpConfig[BufTpIndex].MemRef);

   PduR_PduLengthType AvailableBufferSize = pGateTpSessions[SessionIndex].AvailableBufferSize;

   PduR_PduLengthType AvailableDataSize = pBufTpConfig[BufTpIndex].Length - AvailableBufferSize;

   /* index to the first element of the TP buffer where to read the data by transmit */
   PduR_PduLengthType TxReadIndex =
      (RxWriteIndex + AvailableBufferSize) % pBufTpConfig[BufTpIndex].Length;

   DBG_PDUR_COPYDATAFROMCIRCULARBUFFER_ENTRY(SessionIndex, PduInfoPtr);

   /* provided buffer of LoTp exceeds the provided data of PduR
    * includes handling of AvailableDataSize equal to 0 */
   if(AvailableDataSize <  PduInfoPtr->SduLength)
   {
      RetVal = BUFREQ_E_BUSY;
   }
   /* routing on the fly (no retry) */
   else
   {
      /* copy data without wrapping of the TP buffer at the end */
      if(TxReadIndex < RxWriteIndex)
      {
         TS_MemCpy(PduInfoPtr->SduDataPtr, &pStartTpBuf[TxReadIndex], PduInfoPtr->SduLength);
      }
      /* TxReadIndex >= RxWriteIndex */
      else
      {
         /* Copy data without wrapping at end of TP buffer */
         if(PduInfoPtr->SduLength <= (BufTpLength - TxReadIndex))
         {
            TS_MemCpy(PduInfoPtr->SduDataPtr, &pStartTpBuf[TxReadIndex], PduInfoPtr->SduLength);
         }
         /* Copy data with wrapping at end of TP buffer */
         else
         {
            /* constants used to avoid MISRA 10.1 violation in TS_MemCpy due to complex
               expressions of the form u16a - u16b for third parameter */
            const PduR_PduLengthType SizeToEndOfBuffer = BufTpLength - TxReadIndex;
            const PduR_PduLengthType RemainingSduSize = PduInfoPtr->SduLength - SizeToEndOfBuffer;

            TS_MemCpy(PduInfoPtr->SduDataPtr, &pStartTpBuf[TxReadIndex], SizeToEndOfBuffer);
            TS_MemCpy(&PduInfoPtr->SduDataPtr[BufTpLength - TxReadIndex], &pStartTpBuf[0],
                      RemainingSduSize);
         }
      }

      RetVal = BUFREQ_OK;

      /* update available TP buffer size */
      AvailableBufferSize += PduInfoPtr->SduLength;

      /* reassign available TP buffer size */
      pGateTpSessions[SessionIndex].AvailableBufferSize = AvailableBufferSize;
   }

   DBG_PDUR_COPYDATAFROMCIRCULARBUFFER_EXIT(RetVal, SessionIndex, PduInfoPtr);

   return RetVal;
}


STATIC FUNC(BufReq_ReturnType, PDUR_CODE) PduR_CopyDataFromLinearBuffer
(
   PduR_GateTpSessionIndexType SessionIndex,
   P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) PduInfoPtr,
   PduR_PduLengthType TxTpDataCnt,
   boolean IsMulticastPdu
)
{
   /* initialize return value */
   BufReq_ReturnType RetVal = BUFREQ_E_NOT_OK;

   const PduR_BufTpIndexType BufTpIndex = (PduR_BufTpIndexType)SessionIndex;

   /* pointer to TP gateway sessions */
   /* Deviation MISRAC2012-1 */
   CONSTP2VAR(PduR_GateTpSessionInfoType, AUTOMATIC, PDUR_VAR_CLEARED) pGateTpSessions =
      PDUR_GET_RAM_ADDR(PduR_GateTpSessionInfoType, PduR_GConfigPtr->PduR_GateTpSessionInfoRef);

   /* pointer to TP buffer configurations */
   CONSTP2CONST(PduR_BufTpConfigType, AUTOMATIC, PDUR_APPL_CONST) pBufTpConfig =
      PDUR_GET_CONFIG_ADDR(PduR_BufTpConfigType, PduR_GConfigPtr->PduR_BufTpConfigRef);

   /* index to the first element of the TP buffer where to write the data by reception */
   const PduR_PduLengthType RxWriteIndex = pGateTpSessions[SessionIndex].RxWriteIndex;

   /* pointer to the first element of the TP buffer */
   /* Deviation MISRAC2012-1 */
   CONSTP2VAR(uint8, AUTOMATIC, PDUR_VAR_CLEARED) pStartTpBuf =
      PDUR_GET_RAM_ADDR(uint8, pBufTpConfig[BufTpIndex].MemRef);

   PduR_PduLengthType AvailableBufferSize = pGateTpSessions[SessionIndex].AvailableBufferSize;

   const PduR_PduLengthType AvailableDataSize =
      pBufTpConfig[BufTpIndex].Length - AvailableBufferSize;

   DBG_PDUR_COPYDATAFROMLINEARBUFFER_ENTRY(SessionIndex, PduInfoPtr, TxTpDataCnt, IsMulticastPdu);

   /* provided buffer of LoTp exceeds the provided data of PduR, i.e. AvailableDataSize excludes
    * TxTpDataCnt */
   if((AvailableDataSize + TxTpDataCnt) <  PduInfoPtr->SduLength)
   {
      RetVal = BUFREQ_E_BUSY;
   }
   /* direct gateway */
   else
   {
      /* index to the first element of the TP buffer where to read the data by transmit */
      PduR_PduLengthType TxReadIndex;

      /* TxReadIndex is not wrapped at the end of the linear TP buffer,
         while RxWriteIndex is wrapped in writing to the TP buffer considered as cyclic. */
      if((RxWriteIndex == 0U) && (AvailableBufferSize == pBufTpConfig[BufTpIndex].Length))
      {
         /* This case can only occur at the end of the transmission if retry is necessary */
         TxReadIndex = pBufTpConfig[BufTpIndex].Length;
      }
      else
      {
         TxReadIndex = (RxWriteIndex + AvailableBufferSize) % pBufTpConfig[BufTpIndex].Length;
      }

      /* TP gateway to multiple LoTp */
      /* Data are copied from the beginning of the TP buffer for each valid CopyTxData request */
      if(IsMulticastPdu)
      {
         /* A single request of CopyTxData with exactly the same size of the received I-PDU
          * (single frame) shall deliver the data */
         if( (PduInfoPtr->SduLength != AvailableDataSize) ||
             ((TxTpDataCnt > PDUR_SDU_LENGTH_ZERO) && (TxTpDataCnt != AvailableDataSize)) )
         {
            /* BUFREQ_E_NOT_OK from initialization is returned */
         }
         else
         {
            /* Copy data from buffer */
            TS_MemCpy(PduInfoPtr->SduDataPtr, &pStartTpBuf[0], PduInfoPtr->SduLength);

            /* no need to update TxReadIndex since it always starts from the
             * beginning of the TP buffer */

            RetVal = BUFREQ_OK;
         }
      }
      /* TP gateway to single LoTp */
      /* multiple requests of CopyTxData might occur to fetch all the data,
       * all data is present until the TxConfirmation was received */
      else
      {
         /* illegal TxTpDataCnt (linear TP buffer usage) */
         if(TxReadIndex < TxTpDataCnt)
         {
            /* BUFREQ_E_NOT_OK from initialization is returned */
         }
         else
         {
            /* Copy data from buffer to the LoTP buffer */
            TS_MemCpy(PduInfoPtr->SduDataPtr, &pStartTpBuf[TxReadIndex - TxTpDataCnt],
                      PduInfoPtr->SduLength);

            /* update available buffer size by released data */
            AvailableBufferSize -=  TxTpDataCnt;
            AvailableBufferSize +=  PduInfoPtr->SduLength;

            RetVal = BUFREQ_OK;
         }
      }

      /* reassign Available buffer size */
      pGateTpSessions[SessionIndex].AvailableBufferSize = AvailableBufferSize;
   }

   DBG_PDUR_COPYDATAFROMLINEARBUFFER_EXIT(RetVal, SessionIndex, PduInfoPtr, TxTpDataCnt,
                                          IsMulticastPdu);

   return RetVal;
}


/* AUTOSAR Memory Mapping - end section for code */
#if (TS_MERGED_COMPILE == STD_OFF)
#define PDUR_STOP_SEC_CODE
#include <PduR_MemMap.h>
#endif

#else /* if (PDUR_ZERO_COST_OPERATION_TP == STD_OFF) && .. */

#include <TSCompiler.h>           /* usage of macro in TSCompiler_Default.h */

/* Avoid empty translation unit according to ISO C90 */
TS_PREVENTEMPTYTRANSLATIONUNIT

#endif /* if (PDUR_ZERO_COST_OPERATION_TP == STD_OFF) && .. */
/*==================[end of file]===========================================*/
