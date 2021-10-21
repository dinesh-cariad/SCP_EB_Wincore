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
#include <PduR_Lcfg.h>            /* Link-time configurable data */

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
 ** Auxiliary function that copies received data to the allocated TP buffer.
 ** The TP gateway session members AvailableBufferSize and RxWriteIndex
 ** are updated appropriately.
 **
 ** \param[in] SessionIndex        Index of considered TP gateway session.
 ** \param[in] PduInfoPtr          Pointer to the buffer (SduDataPtr) and its length (SduLength)
 **                                containing the data to be copied by PDU Router module in
 **                                case of TP gateway.
 **
 ** \return Result of request to copy received data to allocated TP buffer.
 ** \retval BUFREQ_E_NOT_OK: SduDataPtr set to null for enabled DET.
 **                          Received data exceeds available size of TP buffer or
 **                          available size of TP buffer equals zero.
 ** \retval BUFREQ_OK:       Received data is copied successfully to TP buffer.
 **/
STATIC FUNC(BufReq_ReturnType, PDUR_CODE) PduR_CopyDataToTpBuffer
(
   PduR_GateTpSessionIndexType SessionIndex,
   P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) PduInfoPtr
);

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

FUNC(BufReq_ReturnType, PDUR_CODE) PduR_GateTpCopyRxData
(
   PduIdType GMPduId,
   P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) PduInfoPtr,
   P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) BufferSizePtr
)
{
   /* initialize return value */
   BufReq_ReturnType RetVal = BUFREQ_E_NOT_OK;

   DBG_PDUR_GATETPCOPYRXDATA_ENTRY(GMPduId, PduInfoPtr, BufferSizePtr);

#if (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON)
   if(PduR_RPathGroupNumTpGwDestEnabled(GMPduId) > PDUR_RPATHGROUP_NUM_NO_DEST_ENABLED)
#endif
   {
      /* get TP gateway session in use */
      PduR_GateTpSessionIndexType SessionIndex;

      boolean CallLoTpTransmit = FALSE;

      /* pointer to routing table */
      CONSTP2CONST(PduR_GTabTpRxType, AUTOMATIC, PDUR_APPL_CONST) pGTabTpRx =
         PDUR_GET_CONFIG_ADDR(PduR_GTabTpRxType, PduR_GConfigPtr->PduR_GTabTpConfig.GTabRxRef);

      /* pointer to TP gateway sessions */
      /* Deviation MISRAC2012-1 */
      CONSTP2VAR(PduR_GateTpSessionInfoType, AUTOMATIC, PDUR_VAR_CLEARED) pSessionInfos =
         PDUR_GET_RAM_ADDR(PduR_GateTpSessionInfoType, PduR_GConfigPtr->PduR_GateTpSessionInfoRef);

#if (PDUR_DEV_ERROR_DETECT == STD_ON)
   boolean CallDetReportError = FALSE;
   uint8 DetErrorCode = PDUR_E_INVALID_REQUEST;
#endif /* if (PDUR_DEV_ERROR_DETECT == STD_ON) */

      /* ENTER CRITICAL SECTION */
      SchM_Enter_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

      /* get TP gateway session in use */
      SessionIndex = PduR_GetGateTpSessionInUse(GMPduId, PDUR_GATETP_QUEUE_ELEMENT_HEAD);

      /* CopyRxData called without preceding StartOfReception */
#if (PDUR_DEV_ERROR_DETECT == STD_ON)
      if(SessionIndex == PDUR_NO_GATETP_SESSION_ASSIGNED)
      {
         CallDetReportError = TRUE;
         DetErrorCode = PDUR_E_INVALID_REQUEST;
      }
      else
#endif /* if (PDUR_DEV_ERROR_DETECT == STD_ON) */
      {
         uint8 ResultGetBitAbort;

         PduR_PduLengthType AvailableBufferSize = pSessionInfos[SessionIndex].AvailableBufferSize;

         /* set BufferSize to initial value */
         *BufferSizePtr = AvailableBufferSize;

         /* check if transmission shall be aborted (e.g. Tx side failed) */
         TS_GetBit(&pSessionInfos[SessionIndex].Status, PDUR_GATETP_ABORT_POS, uint8,
                   ResultGetBitAbort);
         if(ResultGetBitAbort > 0U)
         {
            /* BUFREQ_E_NOT_OK from initialization is returned */
         }
         /* return available length of reserved TP buffer */
         else if(PduInfoPtr->SduLength == (PduLengthType)PDUR_SDU_LENGTH_ZERO)
         {
            RetVal = BUFREQ_OK;
         }
         /* available TP buffer size is to small */
         else if(AvailableBufferSize < PduInfoPtr->SduLength)
         {
            TS_AtomicSetBit_8(&pSessionInfos[SessionIndex].Status, PDUR_GATETP_ABORT_POS);
            /* RetVal from initialization is returned */
         }
         /* copy data on reception */
         else
         {

#if (PDUR_DEV_ERROR_DETECT == STD_ON)
            /* check if input parameter member is NULL */
            if(PduInfoPtr->SduDataPtr == NULL_PTR)
            {
               CallDetReportError = TRUE;
               DetErrorCode = PDUR_E_NULL_POINTER;

               /* set BufferSize to updated value */
               *BufferSizePtr = pSessionInfos[SessionIndex].AvailableBufferSize;

               TS_AtomicSetBit_8(&pSessionInfos[SessionIndex].Status, PDUR_GATETP_ABORT_POS);
               /* RetVal from initialization is returned */
            }
            else
#endif /* #if (PDUR_DEV_ERROR_DETECT == STD_ON) */
            {
               uint8 ResultGetBitTxComplete;
               PduR_PduLengthType AvailableDataSize;
               /* copy data from buffer of LoTp to TP buffer of PduR */
               RetVal = PduR_CopyDataToTpBuffer(SessionIndex, PduInfoPtr);

               /* PduR_CopyDataToTpBuffer returns always successfully (RetVal = BUFREQ_OK) */
               AvailableDataSize = PduR_CalcAvailableDataSize(SessionIndex);

               /* set BufferSize to updated value */
               *BufferSizePtr = pSessionInfos[SessionIndex].AvailableBufferSize;

               if(PduR_GateTpNoSessionQueued(GMPduId))
               {
                  /* call Transmit if TpThreshold is reached for the first time,
                     this is only possible for routing on the fly (single destination) */
                  TS_GetBit(&pSessionInfos[SessionIndex].Status, PDUR_GATETP_TX_COMPLETE_POS, uint8,
                            ResultGetBitTxComplete);
                  if((pGTabTpRx[GMPduId].TpThreshold != PDUR_GATETPTHRESHOLD_DIRECT) &&
                     (AvailableDataSize >= pGTabTpRx[GMPduId].TpThreshold) &&
                     (ResultGetBitTxComplete == 0U))
                  {
                     /* call LoTp_Transmit after critical section is closed */
                     CallLoTpTransmit = TRUE;
                  }
               }
            }
         }
      }

      /* EXIT CRITICAL SECTION */
      SchM_Exit_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

      if(CallLoTpTransmit)
      {
         /* return value of Transmit */
         Std_ReturnType RetValTransmit;

         /* PduInfo to Transmit */
         PduInfoType PduR_PduInfo;

         /* reference to set of target module API functions */
         const PduR_RefToModuleAPI TargetAPIRef = pGTabTpRx[GMPduId].TargetModuleAPIRef;

         /* get pointer to Transmit function */
         const PduR_TransmitFpType TransmitFp = PduR_TpModuleAPI[TargetAPIRef].TpTransmit;

         /* get target PDU ID to Transmit function from routing table */
         const PduR_PduIdType TargetPduId = pGTabTpRx[GMPduId].TargetPduId;

         /* setup PduInfo for Transmit */
         /* No explicit cast is necessary since PduR_PduLengthType and PduLengthType are both fixed
          * with uint16 by the Java generator.
          * Since session member TpSduLength is only updated (written) by StartOfReception, no
          * protection is necessary. */
         PduR_PduInfo.SduLength = pSessionInfos[SessionIndex].TpSduLength;
         PduR_PduInfo.SduDataPtr = NULL_PTR;

         /* ENTER CRITICAL SECTION */
         SchM_Enter_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

         /* update part of the state information (PendTxConfCount - since this is checked by
          * PduR_GetGateTpSessionInUse() in PduR_GateTpCopyTxData() and would cause a DET in
          * case PendTxConfCount == 0) assuming that the call to TransmitFp() will succeed -
          * will need a roll-back in case TransmitFp() fails */
         /* increment counter for pending TxConfirmation */
         pSessionInfos[SessionIndex].PendTxConfCount++;

         /* EXIT CRITICAL SECTION */
         SchM_Exit_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

         /* call Transmit for routing on the fly - only single destination possible */
         RetValTransmit = TransmitFp(TargetPduId, &PduR_PduInfo);

         if(RetValTransmit == E_OK)
         {
            RetVal = BUFREQ_OK;

            TS_AtomicSetBit_8(&pSessionInfos[SessionIndex].Status, PDUR_GATETP_TX_COMPLETE_POS);
         }
         else
         {
            /* ENTER CRITICAL SECTION */
            SchM_Enter_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

            /* transmission failed => roll back the changes done prior to the call to TransmitFp() */
            pSessionInfos[SessionIndex].PendTxConfCount--;

            /* indicate abort of transmission */
            TS_AtomicSetBit_8(&pSessionInfos[SessionIndex].Status, PDUR_GATETP_ABORT_POS);

            /* EXIT CRITICAL SECTION */
            SchM_Exit_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

            RetVal = BUFREQ_E_NOT_OK;

            /* development error detection handling for rejected transmit call */
#if (PDUR_DEV_ERROR_DETECT == STD_ON)
            CallDetReportError = TRUE;
            DetErrorCode = PDUR_E_TP_TX_REQ_REJECTED;
#endif /* if (PDUR_DEV_ERROR_DETECT == STD_ON) */
         }
      }

      /* Conglomerate DET handling */
#if (PDUR_DEV_ERROR_DETECT == STD_ON)
      if(CallDetReportError)
      {
         PDUR_DET_REPORT_ERROR(PDUR_SID_LOTP_COPY_RX_DATA, DetErrorCode);
      }
#endif /* if (PDUR_DEV_ERROR_DETECT == STD_ON) */
   }

   DBG_PDUR_GATETPCOPYRXDATA_EXIT(RetVal, GMPduId, PduInfoPtr, BufferSizePtr);

   return RetVal;
}

/*==================[internal function definitions]=========================*/

STATIC FUNC(BufReq_ReturnType, PDUR_CODE) PduR_CopyDataToTpBuffer
(
   PduR_GateTpSessionIndexType SessionIndex,
   P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) PduInfoPtr
)
{
   const PduR_BufTpIndexType BufTpIndex = (PduR_BufTpIndexType)SessionIndex;

   /* pointer to TP gateway sessions */
   /* Deviation MISRAC2012-1 */
   CONSTP2VAR(PduR_GateTpSessionInfoType, AUTOMATIC, PDUR_VAR_CLEARED) pGateTpSessions =
      PDUR_GET_RAM_ADDR(PduR_GateTpSessionInfoType, PduR_GConfigPtr->PduR_GateTpSessionInfoRef);

   /* pointer to TP buffer configurations */
   CONSTP2CONST(PduR_BufTpConfigType, AUTOMATIC, PDUR_APPL_CONST) pBufTpConfig =
      PDUR_GET_CONFIG_ADDR(PduR_BufTpConfigType, PduR_GConfigPtr->PduR_BufTpConfigRef);

   /* index to the first element of the TP buffer where to write the data by reception */
   PduR_PduLengthType RxWriteIndex = pGateTpSessions[SessionIndex].RxWriteIndex;

   /* length of the associated TP buffer (used also to determine TxReadIndex) */
   const PduR_PduLengthType BufTpLength = pBufTpConfig[BufTpIndex].Length;

   /* pointer to the first element of the TP buffer */
   /* Deviation MISRAC2012-1 */
   CONSTP2VAR(uint8, AUTOMATIC, PDUR_VAR_CLEARED) pStartTpBuf =
      PDUR_GET_RAM_ADDR(uint8, pBufTpConfig[BufTpIndex].MemRef);

   PduR_PduLengthType AvailableBufferSize = pGateTpSessions[SessionIndex].AvailableBufferSize;

   /* index to the first element of the TP buffer where to read the data by transmit */
   const PduR_PduLengthType TxReadIndex =
      (RxWriteIndex + AvailableBufferSize) % pBufTpConfig[BufTpIndex].Length;

   DBG_PDUR_COPYDATATOTPBUFFER_ENTRY(SessionIndex, PduInfoPtr);

   /* copy data to TP buffer with already wrapped RxWriteIndex */
   if(TxReadIndex > RxWriteIndex)
   {
      TS_MemCpy(&pStartTpBuf[RxWriteIndex], PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);

      RxWriteIndex += PduInfoPtr->SduLength;
   }
   /* copy data to TP buffer without wrapped RxWriteIndex, RxWriteIndex >= TxReadIndex */
   else
   {
      /* received data fits completely at the end of the TP buffer */
      if(PduInfoPtr->SduLength <= (BufTpLength - RxWriteIndex))
      {
         TS_MemCpy(&pStartTpBuf[RxWriteIndex], PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);

         RxWriteIndex += PduInfoPtr->SduLength;

         /* reset RxWriteIndex */
         if(RxWriteIndex == BufTpLength)
         {
            RxWriteIndex = 0U;
         }
      }
      /* received data exceeds free size at the end of the TP buffer */
      else
      {
         /* constants used to avoid MISRA 10.1 violation in TS_MemCpy due to complex
            expressions of the form u16a - u16b for third parameter */
         const PduR_PduLengthType SizeToEndOfBuffer = BufTpLength - RxWriteIndex;
         const PduR_PduLengthType RemainingSduSize = PduInfoPtr->SduLength - SizeToEndOfBuffer;

         TS_MemCpy(&pStartTpBuf[RxWriteIndex], PduInfoPtr->SduDataPtr, SizeToEndOfBuffer);
         TS_MemCpy(&pStartTpBuf[0], &PduInfoPtr->SduDataPtr[SizeToEndOfBuffer],
                   RemainingSduSize);

         RxWriteIndex = RemainingSduSize;
      }
   }

   /* update available TP buffer size */
   AvailableBufferSize -= PduInfoPtr->SduLength;

   /* reassign available TP buffer size and current write index */
   pGateTpSessions[SessionIndex].AvailableBufferSize = AvailableBufferSize;
   pGateTpSessions[SessionIndex].RxWriteIndex = RxWriteIndex;

   DBG_PDUR_COPYDATATOTPBUFFER_EXIT(BUFREQ_OK, SessionIndex, PduInfoPtr);

   /* always returns BUFREQ_OK */
   return BUFREQ_OK;
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
