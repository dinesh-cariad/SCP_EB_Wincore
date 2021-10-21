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
 *  MISRAC2012-2) Deviated Rule: 13.5 (required)
 *  The right hand operand of a logical && or || operator shall not contain persistent side effects.
 *
 *  Reason:
 *  This violation is a false positive.
 *  The right hand operation does not contain persistent side effects. The usage of a macro is not
 *  interpreted correctly by the tool.
 *
 */
/*==================[inclusions]============================================*/

#include <PduR_Trace.h>
#include <TSAutosar.h>            /* EB specific standard types */
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

FUNC(void, PDUR_CODE) PduR_GateTpRxIndication
(
   PduIdType GMPduId,
   NotifResultType Result
)
{
   /* TP gateway session in use */
   PduR_GateTpSessionIndexType SessionIndex;

   boolean CallLoTpTransmit = FALSE;

   /* PduInfo for Transmit */
   PduInfoType PduR_PduInfo;

   /* pointer to TP gateway sessions */
   /* Deviation MISRAC2012-1 */
   CONSTP2VAR(PduR_GateTpSessionInfoType, AUTOMATIC, PDUR_VAR_CLEARED) pSessionInfos =
      PDUR_GET_RAM_ADDR(PduR_GateTpSessionInfoType, PduR_GConfigPtr->PduR_GateTpSessionInfoRef);

   /* pointer to routing table */
   CONSTP2CONST(PduR_GTabTpRxType, AUTOMATIC, PDUR_APPL_CONST) pGTabTpRx =
      PDUR_GET_CONFIG_ADDR(PduR_GTabTpRxType, PduR_GConfigPtr->PduR_GTabTpConfig.GTabRxRef);

#if (PDUR_DEV_ERROR_DETECT == STD_ON)
   boolean CallDetReportError = FALSE;
   uint8 DetErrorCode = PDUR_E_INVALID_REQUEST;
#endif /* if (PDUR_DEV_ERROR_DETECT == STD_ON) */

   DBG_PDUR_GATETPRXINDICATION_ENTRY(GMPduId, Result);

   /* ENTER CRITICAL SECTION */
   SchM_Enter_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

   /* get TP gateway session in use */
   SessionIndex = PduR_GetGateTpSessionInUse(GMPduId, PDUR_GATETP_QUEUE_ELEMENT_HEAD);

   /* TpRxIndication called without preceding StartOfReception */
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

      /* reception completed */
      TS_AtomicSetBit_8(&pSessionInfos[SessionIndex].Status, PDUR_GATETP_RECEPT_FIN_POS);

      /* reception failed */
      if(Result != NTFRSLT_OK)
      {
         /* abort transmission */
         TS_AtomicSetBit_8(&pSessionInfos[SessionIndex].Status, PDUR_GATETP_ABORT_POS);
      }

      /* check if TP gateway session shall be released */
      TS_GetBit(&pSessionInfos[SessionIndex].Status, PDUR_GATETP_ABORT_POS, uint8,
                ResultGetBitAbort);
      if((ResultGetBitAbort > 0U) && (pSessionInfos[SessionIndex].PendTxConfCount == 0U))
      {
         /* release assigned TP gateway session */
         PduR_ReleaseGateTpSession(&pSessionInfos[SessionIndex]);

         /* release head of Tp gateway queue */
         PduR_ResetGateTpQueueHead(GMPduId);
      }
      /* direct gateway */
      else if(pGTabTpRx[GMPduId].TpThreshold == PDUR_GATETPTHRESHOLD_DIRECT)
      {
         /* no TP gateway sessions are queued */
         if(PduR_GateTpNoSessionQueued(GMPduId))
         {
            const PduR_BufTpIndexType BufTpIndex = (PduR_BufTpIndexType)SessionIndex;

            /* pointer to TP buffer configurations */
            CONSTP2CONST(PduR_BufTpConfigType, AUTOMATIC, PDUR_APPL_CONST) pBufTpConfig =
               PDUR_GET_CONFIG_ADDR(PduR_BufTpConfigType, PduR_GConfigPtr->PduR_BufTpConfigRef);

            /* pointer to the first element of the TP buffer */
            /* Deviation MISRAC2012-1 */
            CONSTP2VAR(uint8, AUTOMATIC, PDUR_VAR_CLEARED) pStartTpBuf =
               PDUR_GET_RAM_ADDR(uint8, pBufTpConfig[BufTpIndex].MemRef);

            /* No need for a check of TpSduLength equal to AvailableDataSize,
             * the LoTP module is responsible for correctly provided data */

            /* setup PduInfo for Transmit */
            /* SduLength: No explicit cast is necessary since PduR_PduLengthType and PduLengthType
             * are both fixed with uint16 by the Java generator. */
            /* SduDataPtr: For the upper layer called via Transmit, a dedicated SduDataPtr has to be
             * provided */
            PduR_PduInfo.SduLength = pSessionInfos[SessionIndex].TpSduLength;
            PduR_PduInfo.SduDataPtr = pStartTpBuf;

            /* call all LoTp_Transmit with PduR_PduInfo after critical section is closed */
            CallLoTpTransmit = TRUE;
         }
#if (PDUR_TPGWQUEUE_ENABLE == STD_ON)
         else
         {
            /* release head of Tp gateway queue and update head index */
            PduR_ReleaseGateTpQueueElement(GMPduId, PDUR_GATETP_QUEUE_ELEMENT_HEAD);
         }
#endif /* (PDUR_TPGWQUEUE_ENABLE == STD_ON) */

      }
      /* routing on the fly */
      else
      {
         boolean ReleaseQueueElement = TRUE;

         if(PduR_GateTpNoSessionQueued(GMPduId))
         {
            uint8 ResultGetBitTxComplete;
            TS_GetBit(&pSessionInfos[SessionIndex].Status, PDUR_GATETP_TX_COMPLETE_POS, uint8,
                      ResultGetBitTxComplete);

            /* case: no TpTransmit called before */
            if(ResultGetBitTxComplete == 0U)
            {
               /* setup PduInfo for Transmit */
               /* No explicit cast is necessary since PduR_PduLengthType and PduLengthType
                * are both fixed with uint16 by the Java generator */
               PduR_PduInfo.SduLength = pSessionInfos[SessionIndex].TpSduLength;
               PduR_PduInfo.SduDataPtr = NULL_PTR;

               /* call LoTp_Transmit with PduR_PduInfo after critical section is closed */
               CallLoTpTransmit = TRUE;

               /* release current HeadIdx and update to next free one after CallLoTpTransmit */
               ReleaseQueueElement = FALSE;
            }

            /* case: TpTxConfirmation received before TpRxIndication */
            if((ResultGetBitTxComplete > 0U) && (pSessionInfos[SessionIndex].PendTxConfCount == 0U))
            {
               /* release assigned TP gateway session */
               PduR_ReleaseGateTpSession(&pSessionInfos[SessionIndex]);

               /* release session index at head of Tp gateway queue */
               PduR_ResetGateTpQueueHead(GMPduId);

               /* HeadIdx and TailIdx remain equal and unchanged */
               ReleaseQueueElement = FALSE;
            }

            /* case: TpTransmit called by CopyRxData, no TpTxConfirmation received */
#if (PDUR_TPGWQUEUE_ENABLE == STD_ON)
            if (pGTabTpRx[GMPduId].SessionQueueDepth == PDUR_GATETPQUEUE_DEPTH_ONE)
            {
              /* HeadIdx and TailIdx remain 0 */
              ReleaseQueueElement = FALSE;
            }
#else
            /* no QueueInfo is available */
            if( pSessionInfos[SessionIndex].PendTxConfCount > 0U )
            {
              ReleaseQueueElement = FALSE;
            }
#endif /* (PDUR_TPGWQUEUE_ENABLE == STD_ON) */
         }

         /* update queuing information for enabled as well as disabled queueing functionality */
         if(ReleaseQueueElement)
         {
            /* queue available: HeadIdx updated, otherwise session entry reset */
            PduR_ReleaseGateTpQueueElement(GMPduId, PDUR_GATETP_QUEUE_ELEMENT_HEAD);
         }
      }
   }

   /* EXIT CRITICAL SECTION */
   SchM_Exit_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

   if(CallLoTpTransmit)
   {
     /* no correct Transmit call initially */
      boolean SingleTransmitOk = FALSE;

      /* initialize iterator for all entries providing the same GMPduId */
      PduR_PduIdType GTabPos = GMPduId;

      /* provided local variable to ease readability of while condition */
      uint8 MoreEntries;

      /* store old head index since we might need to restore this one in case of
       * failed calls to TransmitFp() */
      const PduR_GateTpQueueIndexType OldHeadIndex = PduR_GateTpGetSessionQueueHeadIndex(GMPduId);

      /* update state information assuming that the call to TransmitFp() will succeed (part 1)
       * will need a roll-back in case TransmitFp() fails */
#if (PDUR_TPGWQUEUE_ENABLE == STD_ON)
      /* update queue index of head of Tp gateway queue to allow reception of next PDU */
      if (pGTabTpRx[GMPduId].SessionQueueDepth > PDUR_GATETPQUEUE_DEPTH_ONE)
      {
        PduR_ReleaseGateTpQueueElement(GMPduId, PDUR_GATETP_QUEUE_ELEMENT_HEAD);
      }
#endif /* (PDUR_TPGWQUEUE_ENABLE == STD_ON) */

      /* loop through all routing table entries matching the current GMPduId */
      do{

#if (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON)
         /* get associated group ID from routing table */
         const PduR_RoutingPathGroupIdType GroupId = pGTabTpRx[GTabPos].RPathGroupId;

         /* destination is either enabled or not referenced by a routing path group,
            Note: Order of OR condition on purpose for faster evaluation and preventing call
            of PduR_IsRoutingPathGroupEnabled with PDUR_ROUTINGPATHGROUP_GO_THROUGH_ID */
         /* Deviation MISRAC2012-2 */
         if((GroupId == PDUR_ROUTINGPATHGROUP_GO_THROUGH_ID) ||
            (PduR_IsRoutingPathGroupEnabled(GroupId) != FALSE))
#endif /* (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON) */
         {
            /* reference to set of target Tp-module API-functions */
            const PduR_RefToModuleAPI TargetAPIRef = pGTabTpRx[GTabPos].TargetModuleAPIRef;

            /* get pointer to Transmit function */
            const PduR_TransmitFpType TransmitFp = PduR_TpModuleAPI[TargetAPIRef].TpTransmit;

            /* get target PDU ID to Transmit function from routing table */
            const PduR_PduIdType TargetPduId = pGTabTpRx[GTabPos].TargetPduId;

            Std_ReturnType RetVal;

            /* initialize to check while condition */
            MoreEntries = pGTabTpRx[GTabPos].MoreEntries;
#if(PDUR_MULTICAST_LOTPTOUP_SUPPORT == STD_ON)
            /* do as long as the last entry is not an upper layer (only one Up allowed) */
            if(!((MoreEntries == PDUR_LAST_MULTICAST_ENTRY_OF_PDUID) &&
                 (PDUR_GATETP_CALL_UPPER_LAYER(TargetAPIRef))))
            {
#endif

               /* ENTER CRITICAL SECTION */
               SchM_Enter_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

               /* update state information assuming that the call to TransmitFp() will succeed (part 2)
                * will need a roll-back in case TransmitFp() fails */
               /* increment counter for pending TxConfirmation */
               pSessionInfos[SessionIndex].PendTxConfCount++;

               /* EXIT CRITICAL SECTION */
               SchM_Exit_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

#if(PDUR_MULTICAST_LOTPTOUP_SUPPORT == STD_ON)
            /* the reception by the upper layer shall not influence the
               TP gateway (no else statement) */
            }
#endif

            /* call Transmit function */
            RetVal = TransmitFp(TargetPduId, &PduR_PduInfo);

#if(PDUR_MULTICAST_LOTPTOUP_SUPPORT == STD_ON)
            /* do as long as the last entry is not an upper layer (only one Up allowed) */
            if(!((MoreEntries == PDUR_LAST_MULTICAST_ENTRY_OF_PDUID) &&
                 (PDUR_GATETP_CALL_UPPER_LAYER(TargetAPIRef))))
            {
#endif
               /* handle Transmit call */
               if(RetVal == E_OK)
               {
                  /* at least one Transmit call was successfully */
                  SingleTransmitOk = TRUE;
               }
               /* handle rejected Transmit call */
               else
               {
                  /* ENTER CRITICAL SECTION */
                  SchM_Enter_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

                  /* transmission failed => roll back the changes done prior to the call to TransmitFp() (part 2) */
                  pSessionInfos[SessionIndex].PendTxConfCount--;

#if (PDUR_MULTICAST_TP_HIGHEST_WINS_STRATEGY == STD_OFF)
                  /* indicate abort of transmission */
                  TS_AtomicSetBit_8(&pSessionInfos[SessionIndex].Status, PDUR_GATETP_ABORT_POS);
#endif /* if (PDUR_MULTICAST_TP_HIGHEST_WINS_STRATEGY == STD_OFF) */

                  /* EXIT CRITICAL SECTION */
                  SchM_Exit_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

                  /* development error detection handling for rejected transmit call */
#if (PDUR_DEV_ERROR_DETECT == STD_ON)
                  CallDetReportError = TRUE;
                  DetErrorCode = PDUR_E_TP_TX_REQ_REJECTED;
#endif /* if (PDUR_DEV_ERROR_DETECT == STD_ON) */

               }

#if(PDUR_MULTICAST_LOTPTOUP_SUPPORT == STD_ON)
            /* the reception by the upper layer shall not influence the
               TP gateway (no else statement) */
            }
#endif
         }
#if (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON)
         else
         {
             /* initialize to check while condition */
            MoreEntries = pGTabTpRx[GTabPos].MoreEntries;
         }
#endif /* (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON) */

         GTabPos++;

      }while(MoreEntries != PDUR_LAST_MULTICAST_ENTRY_OF_PDUID);

#if (PDUR_TPGWQUEUE_ENABLE == STD_ON)
      /* ENTER CRITICAL SECTION */
      SchM_Enter_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();
#endif /* (PDUR_TPGWQUEUE_ENABLE == STD_ON) */

      /* release TP gateway session when all Transmit calls are rejected,
       * no need to protect since no Transmit is called */
      if(SingleTransmitOk == FALSE)
      {
         /* release assigned TP gateway session */
         PduR_ReleaseGateTpSession(&pSessionInfos[SessionIndex]);

         /* transmission failed => roll back the changes done prior to the call to TransmitFp() (part 2) */
         PduR_GateTpSetSessionQueueHeadIndex(GMPduId, OldHeadIndex);

         /* release queued session at head of Tp gateway queue,
          * without update of the head index of the queue info */
         PduR_ResetGateTpQueueHead(GMPduId);
      }
      /* no need to protect since PduR_GateTpCopyTxData can only be called successfully if all
       * LoTp_Transmit calls have finished */
      else
      {
         /* DecouplingImprovement: verify if this really needs to be within a critical section for multicore */
         /* provide information on all finished LoTp_Transmit calls to PduR_GateTpCopyTxData */
         TS_AtomicSetBit_8(&pSessionInfos[SessionIndex].Status, PDUR_GATETP_TX_COMPLETE_POS);
      }

#if (PDUR_TPGWQUEUE_ENABLE == STD_ON)
      /* EXIT CRITICAL SECTION */
      SchM_Exit_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();
#endif /* (PDUR_TPGWQUEUE_ENABLE == STD_ON) */
   }

   /* Conglomerate DET handling */
#if (PDUR_DEV_ERROR_DETECT == STD_ON)
   if(CallDetReportError)
   {
      PDUR_DET_REPORT_ERROR(PDUR_SID_LOTP_RXIND, DetErrorCode);
   }
#endif /* if (PDUR_DEV_ERROR_DETECT == STD_ON) */

   DBG_PDUR_GATETPRXINDICATION_EXIT(GMPduId, Result);
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


/*==================[internal function definitions]=========================*/

#endif /* if (PDUR_ZERO_COST_OPERATION_TP == STD_OFF) && .. */
/*==================[end of file]===========================================*/
