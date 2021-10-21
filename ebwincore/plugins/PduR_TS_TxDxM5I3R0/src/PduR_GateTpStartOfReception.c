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
 ** \brief Auxiliary TP gateway handler function
 **
 ** Auxiliary function that allocates a free TP gateway session.
 **
 ** \param[in]  GMPduId             Identification of the received gateway PDU.
 ** \param[in]  TpSduLength         Total length of the PDU to be received.
 ** \param[out] SessionIndexUsed    SessionIndex of the selected TP gateway session.
 **                                 PDUR_NO_GATETP_SESSION_ASSIGNED if no one is free or
 **                                 already one is pending for GMPduId.
 **
 ** \return Result of TP gateway session request
 ** \retval BUFREQ_E_OVFL: No Buffer of the required length can be provided.
 **                        Either it is not configured of that size or temporary not available.
 ** \retval BUFREQ_OK: Otherwise.
 **/
STATIC FUNC(BufReq_ReturnType, PDUR_CODE) PduR_AssignGateTpSession
(
   PduR_PduIdType GMPduId,
   PduR_PduLengthType TpSduLength,
   P2VAR(PduR_GateTpSessionIndexType, AUTOMATIC, PDUR_APPL_DATA) SessionIndexUsed
);


/**
 ** \brief Auxiliary TP gateway handler function
 **
 ** Auxiliary function that allocates the largest available buffer, but of at least BufferSizeSmallest size.
 **
 ** \param[in]  BufferSizeMin  minimum buffer size of the allowed range.
 **
 ** \return SessionIndex of the selected TP gateway session. PDUR_NO_GATETP_SESSION_ASSIGNED if no one is free.
 **/
STATIC FUNC(PduR_GateTpSessionIndexType, PDUR_CODE) PduR_LargestTpBufferAbove
(
   PduR_PduLengthType BufferSizeMin
);


/**
 ** \brief Auxiliary TP gateway handler function
 **
 ** Auxiliary function that allocates the available buffer of at least BufferSizeSmallest size.
 **
 ** \param[in]  BufferSizeSmallest  Buffer Size as base for the incrementing search algorithm of the ordered TP buffers.
 **
 ** \return SessionIndex of the selected TP gateway session. PDUR_NO_GATETP_SESSION_ASSIGNED if no one is free.
 **/
STATIC FUNC(PduR_GateTpSessionIndexType, PDUR_CODE) PduR_AssignNextBiggerTpBuffer
(
   PduR_PduLengthType BufferSizeSmallest
);


/**
 ** \brief Auxiliary TP gateway handler function
 **
 ** Auxiliary function that allocates a free TP gateway session for routing on-the-fly.
 **
 ** \param[in]  TpThreshold  TpThreshold of the considered routing path.
 ** \param[in]  TpSduLength  TpSduLength received by StartOfReception
 **
 ** \return SessionIndex of the selected TP gateway session. PDUR_NO_GATETP_SESSION_ASSIGNED if no one is free.
 **/
STATIC FUNC(PduR_GateTpSessionIndexType, PDUR_CODE) PduR_AssignTpBufferRotf
(
   PduR_PduLengthType TpThreshold,
   PduR_PduLengthType TpSduLength
);

#if (PDUR_ROTF_BUFFER_ASSIGNMENT_STRATEGY == PDUR_BUFFER_NEXT_TO_SDULENGTH)
/**
 ** \brief Auxiliary TP gateway handler function
 **
 ** Auxiliary function that allocates the largest available TP buffer in the range [BufferSizeMin,BufferSizeMax].
 **
 ** \param[in]  BufferSizeMin  minimum buffer size of the allowed range.
 ** \param[in]  BufferSizeMax  maximum buffer size of the allowed range.
 **
 ** \return SessionIndex of the selected TP gateway session. PDUR_NO_GATETP_SESSION_ASSIGNED if no one is free.
 **/
STATIC FUNC(PduR_GateTpSessionIndexType, PDUR_CODE) PduR_AssignBiggestTpBufferInRange
(
   PduR_PduLengthType BufferSizeMin,
   PduR_PduLengthType BufferSizeMax
);
#endif /* #if (PDUR_ROTF_BUFFER_ASSIGNMENT_STRATEGY == PDUR_BUFFER_NEXT_TO_SDULENGTH) */

/**
 ** \brief Auxiliary TP gateway handler function
 **
 ** Auxiliary function that determines the index of the first free position in the array
 ** of queued TP gateway sessions.
 **
 ** \param[in]  GMPduId             Identification of the received gateway PDU.
 **
 ** \return Index of the first free position in the array to queue the TP gateway session.
 **         PDUR_NO_GATETP_QUEUEING_POSSIBLE if TP gateway session for GMPduId can not be queued.
 **
 **/
STATIC FUNC(PduR_GateTpQueueIndexType, PDUR_CODE) PduR_GetGateTpQueueIndexHead
(
   PduR_PduIdType GMPduId
);

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

FUNC(BufReq_ReturnType, PDUR_CODE) PduR_GateTpStartOfReception
(
   PduIdType GMPduId,
   PduLengthType TpSduLength,
   P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) BufferSizePtr
)
{
   BufReq_ReturnType RetVal;

   DBG_PDUR_GATETPSTARTOFRECEPTION_ENTRY(GMPduId, TpSduLength, BufferSizePtr);

   /* ENTER CRITICAL SECTION */
   SchM_Enter_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

#if (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON)
   /* at least a single destination PDU is active (reachable) */
   if(PduR_RPathGroupNumTpGwDestEnabled(GMPduId) > PDUR_RPATHGROUP_NUM_NO_DEST_ENABLED)
#endif
   {
      PduR_GateTpSessionIndexType SessionIndex;

      /* assign TP gateway session */
      RetVal = PduR_AssignGateTpSession(GMPduId, TpSduLength, &SessionIndex);

      /* free TP gateway session (TP buffer) assigned */
      if(SessionIndex != PDUR_NO_GATETP_SESSION_ASSIGNED)
      {
         /* pointer to TP buffer configurations */
         CONSTP2CONST(PduR_BufTpConfigType, AUTOMATIC, PDUR_APPL_CONST) pBufTpConfig =
            PDUR_GET_CONFIG_ADDR(PduR_BufTpConfigType, PduR_GConfigPtr->PduR_BufTpConfigRef);

         *BufferSizePtr = pBufTpConfig[SessionIndex].Length;
      }
      /* else: no TP buffer of that size configured, temporary not available or
         already ongoing TP gateway session.
      RetVal = BUFREQ_E_OVFL, BufferSize shall remain unchanged */
   }
#if (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON)
   else
   {
      /* no single destination PDU is active (reachable) */
      RetVal = BUFREQ_E_NOT_OK;
   }
#endif

   /* EXIT CRITICAL SECTION */
   SchM_Exit_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

   DBG_PDUR_GATETPSTARTOFRECEPTION_EXIT(RetVal, GMPduId, TpSduLength, BufferSizePtr);

   return RetVal;
}


/*==================[internal function definitions]=========================*/

STATIC FUNC(BufReq_ReturnType, PDUR_CODE) PduR_AssignGateTpSession
(
   PduR_PduIdType GMPduId,
   PduR_PduLengthType TpSduLength,
   P2VAR(PduR_GateTpSessionIndexType, AUTOMATIC, PDUR_APPL_DATA) SessionIndexUsed
)
{
   PduR_BufTpIndexType  BufTpIndex = 0U;
   BufReq_ReturnType RetVal;

   /* pointer to routing table */
   CONSTP2CONST(PduR_GTabTpRxType, AUTOMATIC, PDUR_APPL_CONST) pGTabTpRx =
      PDUR_GET_CONFIG_ADDR(PduR_GTabTpRxType, PduR_GConfigPtr->PduR_GTabTpConfig.GTabRxRef);

   /* initial session index, empty element in queue */
   PduR_GateTpSessionIndexType SessionIndex = PDUR_NO_GATETP_SESSION_ASSIGNED;

   /* provide next queue index to be used for reception */
   PduR_GateTpQueueIndexType QueueIndex = PduR_GetGateTpQueueIndexHead(GMPduId);

   DBG_PDUR_ASSIGNGATETPSESSION_ENTRY(GMPduId, TpSduLength, SessionIndexUsed);

   /* reception request for GMPduId can be handled */
   if(QueueIndex != PDUR_NO_GATETP_QUEUEING_POSSIBLE)
   {
      /* TP buffer threshold for direct routing (PduRTpThreshold disabled) */
      if(pGTabTpRx[GMPduId].TpThreshold == PDUR_GATETPTHRESHOLD_DIRECT)
      {
         /* search for free TP buffer */
         /* determine TP buffer of maximum size */
         if(TpSduLength == PDUR_SDU_LENGTH_ZERO)
         {
            SessionIndex = PduR_LargestTpBufferAbove((PDUR_SDU_LENGTH_ZERO));
         }
         else
         {
            SessionIndex = PduR_AssignNextBiggerTpBuffer(TpSduLength);
         }
      }
      /* TP buffer threshold for routing on the fly (TpThreshold configured) */
      else
      {
         /* search for free TP buffer */
         /* determine TP buffer of maximum size */
         if(TpSduLength == PDUR_SDU_LENGTH_ZERO)
         {
           SessionIndex = PduR_LargestTpBufferAbove(pGTabTpRx[GMPduId].TpThreshold);
         }
#if (PDUR_TPGWQUEUE_ENABLE == STD_ON)
         /* for to be queued PDUs, the complete PDU needs to be stored, since only the first
            TP buffer can be considered as ring buffer */
         else if (PduR_GateTpNoSessionQueued(GMPduId) == FALSE)
         {
            SessionIndex = PduR_AssignNextBiggerTpBuffer(TpSduLength);
         }
#endif /* (PDUR_TPGWQUEUE_ENABLE == STD_ON)  */
         else
         {
            SessionIndex = PduR_AssignTpBufferRotf(pGTabTpRx[GMPduId].TpThreshold, TpSduLength);
         }
      }
   }

   /* assign TP gateway session and queue at proper position */
   if(SessionIndex != PDUR_NO_GATETP_SESSION_ASSIGNED)
   {
      /* pointer to TP buffer configurations */
      CONSTP2CONST(PduR_BufTpConfigType, AUTOMATIC, PDUR_APPL_CONST) pBufTpConfig =
         PDUR_GET_CONFIG_ADDR(PduR_BufTpConfigType, PduR_GConfigPtr->PduR_BufTpConfigRef);

      /* pointer to TP gateway sessions */
      /* Deviation MISRAC2012-1 */
      CONSTP2VAR(PduR_GateTpSessionInfoType, AUTOMATIC, PDUR_VAR_CLEARED) pSessionInfos =
         PDUR_GET_RAM_ADDR(PduR_GateTpSessionInfoType, PduR_GConfigPtr->PduR_GateTpSessionInfoRef);

      /* pointer to queue of TP gateway sessions */
      /* Deviation MISRAC2012-1 */
      CONSTP2VAR(PduR_GateTpSessionIndexType, AUTOMATIC, PDUR_VAR_CLEARED) pGateTpSessionIndex =
         PDUR_GET_RAM_ADDR(PduR_GateTpSessionIndexType, pGTabTpRx[GMPduId].SessionQueueMemRef);

      /* both are identical if valid TP gateway session detected */
      BufTpIndex = (PduR_BufTpIndexType)SessionIndex;

      /* provide entry to queue of TP gateway sessions */
      pGateTpSessionIndex[QueueIndex] = SessionIndex;

      /* occupy TP gateway session and initialize */
      TS_AtomicAssign8(pSessionInfos[SessionIndex].Status, (uint8)PDUR_GATETP_RST_STATUS_MSK);
      TS_AtomicSetBit_8(&pSessionInfos[SessionIndex].Status, PDUR_GATETP_OCCUPIED_SES_POS);
      TS_AtomicAssign8(pSessionInfos[SessionIndex].PendTxConfCount,
                       (uint8)PDUR_GATETP_NO_PEND_TXCONF_MSK);
      pSessionInfos[SessionIndex].TpSduLength = TpSduLength;
      pSessionInfos[SessionIndex].RxWriteIndex = 0U;
      pSessionInfos[SessionIndex].AvailableBufferSize = pBufTpConfig[BufTpIndex].Length;

      /* if a TP gateway session is assigned also a TP buffer is assigned and vice versa */
      RetVal = BUFREQ_OK;
   }
   else
   {
     /* no TP buffer of that size configured, temporary not available or
        already ongoing TP gateway session.
        AvailableBufferSize shall remain unchanged */
     RetVal = BUFREQ_E_OVFL;
   }

   *SessionIndexUsed = SessionIndex;

   DBG_PDUR_ASSIGNGATETPSESSION_EXIT(RetVal, GMPduId, TpSduLength, SessionIndexUsed);

   return RetVal;
}


STATIC FUNC(PduR_GateTpSessionIndexType, PDUR_CODE) PduR_AssignTpBufferRotf
(
   PduR_PduLengthType TpThreshold,
   PduR_PduLengthType TpSduLength
)
{
   /* initial session index, empty element in queue */
   PduR_GateTpSessionIndexType SessionIndex = PDUR_NO_GATETP_SESSION_ASSIGNED;

   DBG_PDUR_ASSIGNTPBUFFERROTF_ENTRY(TpThreshold, TpSduLength);

   /* TpSduLength 0 is handled one level above */
   if(TpSduLength < TpThreshold)
   {
     SessionIndex = PduR_AssignNextBiggerTpBuffer(TpSduLength);
   }
   else
#if (PDUR_ROTF_BUFFER_ASSIGNMENT_STRATEGY == PDUR_BUFFER_NEXT_TO_TPTHRESHOLD)
   /* next to TpThreshold */
   {
     SessionIndex = PduR_AssignNextBiggerTpBuffer(TpThreshold);
   }
#elif (PDUR_ROTF_BUFFER_ASSIGNMENT_STRATEGY == PDUR_BUFFER_NEXT_TO_SDULENGTH)
   /* next to SduLength */
   {
      SessionIndex = PduR_AssignBiggestTpBufferInRange(TpThreshold, TpSduLength);

      if(SessionIndex == PDUR_NO_GATETP_SESSION_ASSIGNED)
      {
        SessionIndex = PduR_AssignNextBiggerTpBuffer(TpSduLength);
      }
   }
#else
/* case should  not occur - defensive programming with error message (exclude from coverage) */
#endif

   DBG_PDUR_ASSIGNTPBUFFERROTF_EXIT(SessionIndex, TpThreshold, TpSduLength);

   return SessionIndex;
}

#if (PDUR_ROTF_BUFFER_ASSIGNMENT_STRATEGY == PDUR_BUFFER_NEXT_TO_SDULENGTH)
STATIC FUNC(PduR_GateTpSessionIndexType, PDUR_CODE) PduR_AssignBiggestTpBufferInRange
(
   PduR_PduLengthType BufferSizeMin,
   PduR_PduLengthType BufferSizeMax
)
{
   PduR_BufTpIndexType  BufTpIndex = PduR_GConfigPtr->PduR_BufTpCount;

   /* pointer to TP buffer configurations */
   CONSTP2CONST(PduR_BufTpConfigType, AUTOMATIC, PDUR_APPL_CONST) pBufTpConfig =
      PDUR_GET_CONFIG_ADDR(PduR_BufTpConfigType, PduR_GConfigPtr->PduR_BufTpConfigRef);

   /* initial session index, empty element in queue */
   PduR_GateTpSessionIndexType SessionIndex = PDUR_NO_GATETP_SESSION_ASSIGNED;

   /* pointer to TP gateway sessions */
   /* Deviation MISRAC2012-1 */
   CONSTP2VAR(PduR_GateTpSessionInfoType, AUTOMATIC, PDUR_VAR_CLEARED) pSessionInfos =
      PDUR_GET_RAM_ADDR(PduR_GateTpSessionInfoType, PduR_GConfigPtr->PduR_GateTpSessionInfoRef);

   uint8 ResultGetBitOccupiedSession;

   DBG_PDUR_ASSIGNBIGGESTTPBUFFERINRANGE_ENTRY(BufferSizeMin, BufferSizeMax);

   /* search for first suitable TP buffer in descending order with respect to BufferSizeMax */
   do{
      /* loop is left if first valid TP buffer size is detected. */
      BufTpIndex--;
   }
   while((BufTpIndex > 0U) &&
         (BufferSizeMax < pBufTpConfig[BufTpIndex].Length));

   /* check if starting point is inbetween the range with respect to BufferSizeMin */
   if(BufferSizeMin <= pBufTpConfig[BufTpIndex].Length)
   {
      /* increment session index for do-while loop handling */
      SessionIndex = BufTpIndex + 1U;

      /* search for free TP gateway session */
      do{
         /* loop is left if free TP gateway session is detected.
            If its not detected, the final SessionIndex
            holds the value of PduR_GConfigPtr->PduR_BufTpCount. */
         SessionIndex--;

      /* check if session is occupied */
         TS_GetBit(&pSessionInfos[SessionIndex].Status, PDUR_GATETP_OCCUPIED_SES_POS,
                   uint8, ResultGetBitOccupiedSession);

      }while((ResultGetBitOccupiedSession > 0U) &&
             (BufferSizeMin <= pBufTpConfig[SessionIndex].Length) &&
             (SessionIndex > 0U));

      /* no proper TP gateway session detected */
      if((ResultGetBitOccupiedSession > 0U) ||
         (BufferSizeMin > pBufTpConfig[SessionIndex].Length))
      {
         SessionIndex = PDUR_NO_GATETP_SESSION_ASSIGNED;
      }
   }
   else
   {
      SessionIndex = PDUR_NO_GATETP_SESSION_ASSIGNED;
   }

   DBG_PDUR_ASSIGNBIGGESTTPBUFFERINRANGE_EXIT(SessionIndex, BufferSizeMin, BufferSizeMax);

   return SessionIndex;
}
#endif /* #if (PDUR_ROTF_BUFFER_ASSIGNMENT_STRATEGY == PDUR_BUFFER_NEXT_TO_SDULENGTH) */

STATIC FUNC(PduR_GateTpSessionIndexType, PDUR_CODE) PduR_AssignNextBiggerTpBuffer
(
   PduR_PduLengthType BufferSizeSmallest
)
{
   /* pointer to TP buffer configurations */
   CONSTP2CONST(PduR_BufTpConfigType, AUTOMATIC, PDUR_APPL_CONST) pBufTpConfig =
      PDUR_GET_CONFIG_ADDR(PduR_BufTpConfigType, PduR_GConfigPtr->PduR_BufTpConfigRef);

   /* initial session index, empty element in queue */
   PduR_GateTpSessionIndexType SessionIndex = PDUR_NO_GATETP_SESSION_ASSIGNED;

   DBG_PDUR_ASSIGNNEXTBIGGERTPBUFFER_ENTRY(BufferSizeSmallest);

   /*  No TP buffer of the required length can be provided */
   if(BufferSizeSmallest > pBufTpConfig[PduR_GConfigPtr->PduR_BufTpCount - 1U].Length)
   {
     /* Initial SessionIndex with PDUR_NO_GATETP_SESSION_ASSIGNED is returned since no session can be assigned */
   }
   /* determine available TP buffer */
   else
   {
      PduR_BufTpIndexType  BufTpIndex = 0U;

      /* pointer to TP gateway sessions */
      /* Deviation MISRAC2012-1 */
      CONSTP2VAR(PduR_GateTpSessionInfoType, AUTOMATIC, PDUR_VAR_CLEARED) pSessionInfos =
         PDUR_GET_RAM_ADDR(PduR_GateTpSessionInfoType, PduR_GConfigPtr->PduR_GateTpSessionInfoRef);

      uint8 ResultGetBitOccupiedSession;

      /* search for first suitable TP buffer in ascending order */
      while((BufTpIndex < PduR_GConfigPtr->PduR_BufTpCount) &&
            (BufferSizeSmallest > pBufTpConfig[BufTpIndex].Length))
      {
         /* loop is left if first valid TP buffer size is detected. */
         BufTpIndex++;
      }

      SessionIndex = BufTpIndex;

      /* search for free TP gateway session */
      do{
         /* check if session is occupied */
         TS_GetBit(&pSessionInfos[SessionIndex].Status, PDUR_GATETP_OCCUPIED_SES_POS,
                   uint8, ResultGetBitOccupiedSession);

         /* loop is left if free TP gateway session is detected.
            If its not detected, the final SessionIndex
            holds the value of PduR_GConfigPtr->PduR_BufTpCount. */
         SessionIndex++;

      }while((SessionIndex < PduR_GConfigPtr->PduR_BufTpCount) &&
             (ResultGetBitOccupiedSession > 0U));

      /* no proper TP gateway session detected */
      if(ResultGetBitOccupiedSession > 0U)
      {
         SessionIndex = PDUR_NO_GATETP_SESSION_ASSIGNED;
      }
      else
      {
         /* correction from do-while loop */
         SessionIndex--;
      }
   }

   DBG_PDUR_ASSIGNNEXTBIGGERTPBUFFER_EXIT(SessionIndex, BufferSizeSmallest);

   return SessionIndex;
}

STATIC FUNC(PduR_GateTpSessionIndexType, PDUR_CODE) PduR_LargestTpBufferAbove
(
   PduR_PduLengthType BufferSizeMin
)
{
   /* pointer to TP buffer configurations */
   CONSTP2CONST(PduR_BufTpConfigType, AUTOMATIC, PDUR_APPL_CONST) pBufTpConfig =
      PDUR_GET_CONFIG_ADDR(PduR_BufTpConfigType, PduR_GConfigPtr->PduR_BufTpConfigRef);

   /* pointer to TP gateway sessions */
   /* Deviation MISRAC2012-1 */
   CONSTP2VAR(PduR_GateTpSessionInfoType, AUTOMATIC, PDUR_VAR_CLEARED) pSessionInfos =
      PDUR_GET_RAM_ADDR(PduR_GateTpSessionInfoType, PduR_GConfigPtr->PduR_GateTpSessionInfoRef);

   /* initially set to allow first while loop */
   uint8 ResultGetBitOccupiedSession;

   /* condition to break the loop */
   boolean LowerSizeReached = FALSE;

   /* search for TP buffer in descending order */
   PduR_GateTpSessionIndexType SessionIndex = (PduR_GateTpSessionIndexType)PduR_GConfigPtr->PduR_BufTpCount;

   DBG_PDUR_LARGESTTPBUFFERABOVE_ENTRY(BufferSizeMin);

   /* search for free TP gateway session */
   do{
      /* loop is left if free TP gateway session is detected. */
      SessionIndex--;

      /* check if TP gateway session is occupied */
      TS_GetBit(&pSessionInfos[SessionIndex].Status, PDUR_GATETP_OCCUPIED_SES_POS,
                uint8, ResultGetBitOccupiedSession);

      /* no buffer can be assigned of at least BufferSizeMin */
      if(BufferSizeMin > pBufTpConfig[SessionIndex].Length)
      {
         LowerSizeReached = TRUE;
      }

   }while((SessionIndex > 0U) &&
          (ResultGetBitOccupiedSession > 0U) &&
          (LowerSizeReached == FALSE));


   /* no free TP gateway session detected in range of available buffers */
   if((ResultGetBitOccupiedSession > 0U) ||
      (LowerSizeReached == TRUE))
   {
      SessionIndex = PDUR_NO_GATETP_SESSION_ASSIGNED;
   }

   DBG_PDUR_LARGESTTPBUFFERABOVE_EXIT(BufferSizeMin, SessionIndex);

   return SessionIndex;
}


STATIC FUNC(PduR_GateTpQueueIndexType, PDUR_CODE) PduR_GetGateTpQueueIndexHead
(
   PduR_PduIdType GMPduId
)
{
   PduR_GateTpQueueIndexType QueueIndex;

   /* pointer to routing table */
   CONSTP2CONST(PduR_GTabTpRxType, AUTOMATIC, PDUR_APPL_CONST) pGTabTpRx =
      PDUR_GET_CONFIG_ADDR(PduR_GTabTpRxType, PduR_GConfigPtr->PduR_GTabTpConfig.GTabRxRef);

   DBG_PDUR_GETGATETPQUEUEINDEXHEAD_ENTRY(GMPduId);

#if (PDUR_TPGWQUEUE_ENABLE == STD_ON)
   if (pGTabTpRx[GMPduId].SessionQueueDepth > PDUR_GATETPQUEUE_DEPTH_ONE)
   {
      /* pointer to TP gateway session queue info */
      /* Deviation MISRAC2012-1 */
      CONSTP2VAR(PduR_GateTpQueueInfoType, AUTOMATIC, PDUR_VAR_CLEARED) pGateTpQueueInfo =
         PDUR_GET_RAM_ADDR(PduR_GateTpQueueInfoType, PduR_GConfigPtr->PduR_GateTpQueueInfoRef);

      QueueIndex = pGateTpQueueInfo[GMPduId].HeadIdx;
   }
   else
#endif /* (PDUR_TPGWQUEUE_ENABLE == STD_ON)  */
   {
      /* pointer to queue of TP gateway sessions */
      /* Deviation MISRAC2012-1 */
      CONSTP2VAR(PduR_GateTpSessionIndexType, AUTOMATIC, PDUR_VAR_CLEARED) pGateTpSessionIndex =
         PDUR_GET_RAM_ADDR(PduR_GateTpSessionIndexType, pGTabTpRx[GMPduId].SessionQueueMemRef);

      QueueIndex = PDUR_GATETPQUEUE_FIRSTINDEX;

      /* free session can not be queued */
      if(pGateTpSessionIndex[QueueIndex] != PDUR_NO_GATETP_SESSION_ASSIGNED)
      {
         QueueIndex = PDUR_NO_GATETP_QUEUEING_POSSIBLE;
      }
   }

   DBG_PDUR_GETGATETPQUEUEINDEXHEAD_EXIT(QueueIndex, GMPduId);

   return QueueIndex;
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
