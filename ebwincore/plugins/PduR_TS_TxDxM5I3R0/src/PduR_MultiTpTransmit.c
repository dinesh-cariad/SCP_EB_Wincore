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
#include <TSAtomic_Assign.h>      /* Atomic assignments from Base PlugIn */
#include <ComStack_Types.h>       /* AUTOSAR Communication Stack types */
#include <PduR_Internal_Static.h> /* Internal API (static part) */
#include <PduR_Api_Depend.h>      /* Public API (depend part) */


#if((PDUR_ZERO_COST_OPERATION_TP == STD_OFF) && \
    (PDUR_MULTICAST_UPTOLOTP_SUPPORT == STD_ON))
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
 ** \brief Auxiliary TP multicast handler function
 **
 ** Assigns free multicast transmit session.
 ** If no multicast transmit session can be assigned PDUR_NO_MULTITP_SESSION_ASSIGNED is returned.
 **
 ** \param[in]  MTpTxPduId  PduId group of multicast Transmit to LoTP(s)
 **
 ** \return     Assigned multicast transmit session index.
 ** \retval     uint8       Multicast transmit session index assigned
 **                         or PDUR_NO_MULTITP_SESSION_ASSIGNED if no session can be assigned.
 **/
STATIC FUNC(uint8, PDUR_CODE) PduR_AssignMultiTpTxSession
(
   PduR_PduIdType MTpTxPduId
);

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

FUNC(Std_ReturnType, PDUR_CODE) PduR_MultiTpTransmit
(
   PduIdType MTpTxPduId,
   P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) PduInfoPtr
)
{
   /* return value */
   Std_ReturnType RetVal;

   /* determine pending multicast transmit session */
   const uint8 PendingSessionIndex = PduR_SearchMultiTpTxSessionIndex((PduR_PduIdType)MTpTxPduId);

   DBG_PDUR_MULTITPTRANSMIT_ENTRY(MTpTxPduId, PduInfoPtr);

   /* check for pending multicast transmit session */
   if(PendingSessionIndex != PDUR_NO_MULTITP_SESSION_ASSIGNED)
   {
      /* E_NOT_OK is returned for already pending multicast transmit session */
      RetVal = E_NOT_OK;
   }
   else
   {
      /* determine multicast transmit session */
      const uint8 SessionIndex = PduR_AssignMultiTpTxSession((PduR_PduIdType)MTpTxPduId);

      /* free multicast transmit session assigned */
      if(SessionIndex != PDUR_NO_MULTITP_SESSION_ASSIGNED)
      {

         /* no correct Transmit call initially */
         boolean SingleTransmitOk = FALSE;

         /* pointer to routing table */
         CONSTP2CONST(PduR_MTabUpTpTxType, AUTOMATIC, PDUR_APPL_CONST) pMTabUpTpTx =
            PDUR_GET_CONFIG_ADDR(PduR_MTabUpTpTxType,
                                 PduR_GConfigPtr->PduR_MTabUpTpConfig.MTabTxRef);

         /* pointer to multicast transmit sessions */
         /* Deviation MISRAC2012-1 */
         CONSTP2VAR(PduR_MultiTpTxSessionInfoType, AUTOMATIC, PDUR_VAR_CLEARED) pMultiTpTxSessionInfo =
            PDUR_GET_RAM_ADDR(PduR_MultiTpTxSessionInfoType,
                              PduR_GConfigPtr->PduR_MultiTpTxSessionInfoRef);

         /* initialize iterator for all entries providing the same MTpTxPduId */
         PduR_PduIdType MTabPos = (PduR_PduIdType)MTpTxPduId;

         /* provided local variable to ease readability of while condition */
         uint8 TotalNumDests;

#if ((PDUR_MULTICAST_TP_HIGHEST_WINS_STRATEGY == STD_ON) && (PDUR_DEV_ERROR_DETECT == STD_ON))
         /* no failing Transmit call initially */
         boolean SingleTransmitNotOk = FALSE;
#endif

         RetVal = E_OK;

         /* loop through all routing table entries matching the current MTpTxPduId */
         do{
#if (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON)
            /* get destination PDU routing path group ID from routing table */
            const PduR_RoutingPathGroupIdType GroupId = pMTabUpTpTx[MTabPos].RPathGroupId;

            /* destination is either enabled or not referenced by a routing path group,
               Note: Order of OR condition on purpose for faster evaluation and preventing call
               of PduR_IsRoutingPathGroupEnabled with PDUR_ROUTINGPATHGROUP_GO_THROUGH_ID */
            /* Deviation MISRAC2012-2 */
            if((GroupId == PDUR_ROUTINGPATHGROUP_GO_THROUGH_ID) ||
               (PduR_IsRoutingPathGroupEnabled(GroupId) != FALSE))
#endif /* (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON) */
            {
               /* reference set of TP API-functions */
               const PduR_RefToModuleAPI TargetAPIRef = pMTabUpTpTx[MTabPos].TargetModuleAPIRef;

               /* pointer to Transmit function */
               const PduR_TransmitFpType TransmitFp = PduR_TpModuleAPI[TargetAPIRef].TpTransmit;

               /* get target PDU ID */
               const PduR_PduIdType TargetPduId = pMTabUpTpTx[MTabPos].TargetPduId;

               /* handle rejected Transmit call,
                  comparison to E_OK, since SchM_Call returns other values than E_OK/E_NOT_OK */
               if(TransmitFp(TargetPduId, PduInfoPtr) != E_OK)
               {
#if (PDUR_MULTICAST_TP_HIGHEST_WINS_STRATEGY == STD_OFF)
                  /* lowest wins:
                     E_NOT_OK is returned if at least one of the Transmit functions returns not E_OK */
                  RetVal = E_NOT_OK;

                  /* indicate abort of transmission to CopyTxData */
                  TS_AtomicSetBit_8(&pMultiTpTxSessionInfo[SessionIndex].Status,
                     PDUR_MULTITPTX_ABORT_POS);
#else
                  /* highest wins:
                   * E_NOT_OK is returned if all Transmit calls fail,
                   * RetVal value is assigned with checking SingleTransmitOk == FALSE later on */
#if (PDUR_DEV_ERROR_DETECT == STD_ON)
                  /* at least one Transmit call was not successfully */
                  SingleTransmitNotOk = TRUE;
#endif  /* if (PDUR_DEV_ERROR_DETECT == STD_ON) */
#endif /* if (PDUR_MULTICAST_TP_HIGHEST_WINS_STRATEGY == STD_OFF) */
               }
               else
               {
                 /* ENTER CRITICAL SECTION */
                  SchM_Enter_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

                  /* increment part of Status holding the number of successful Transmit calls */
                  pMultiTpTxSessionInfo[SessionIndex].Status++;

                  /* LEAVE CRITICAL SECTION  */
                  SchM_Exit_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

                  /* at least one Transmit call was successfully */
                  SingleTransmitOk = TRUE;
               }
            }

            /* initialize to check while condition */
            TotalNumDests = pMTabUpTpTx[MTabPos].TotalNumDests;

            MTabPos++;

         }while(TotalNumDests < PDUR_TOTAL_NUM_DEST_THRESHOLD);

         /* release multicast transmit session when no single destination can be reached,
            i.e. either the destinations PDU is disabled or the Transmit call is rejected */
         if(SingleTransmitOk == FALSE)
         {
            /* release assigned session info */
            PduR_ReleaseMultiTpTxSession(SessionIndex);

#if (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON) || (PDUR_MULTICAST_TP_HIGHEST_WINS_STRATEGY == STD_ON)
            /* no single destination was reached, i.e either the destination PDU is disabled or the
             * Transmit call failed, then the SingleTransmitOK flag remains at initial state FALSE.
             * In that case E_NOT_OK is returned. */
            RetVal = E_NOT_OK;
#endif /* (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON) */
         }
         else
         {
            /* store SduLength for retry parameter TxTpDataCnt applied within CopyTxData */
            pMultiTpTxSessionInfo[SessionIndex].SduLength = PduInfoPtr->SduLength;

            /* provide information of finished Transmit calls to CopyTxData */
            TS_AtomicSetBit_8(&pMultiTpTxSessionInfo[SessionIndex].Status,
               PDUR_MULTITPTX_TX_COMPLETE_POS);
         }

#if ((PDUR_MULTICAST_TP_HIGHEST_WINS_STRATEGY == STD_ON) && (PDUR_DEV_ERROR_DETECT == STD_ON))
         /* handle rejected Transmit call when highest win strategy applies and not all are
            rejected (other case is handled by PduR_GenericTpTransmit) */
         if((RetVal == E_OK) && (SingleTransmitNotOk == TRUE))
         {
            PDUR_DET_REPORT_ERROR(PDUR_SID_UP_TX, PDUR_E_TP_TX_REQ_REJECTED);
         }
#endif /* if (PDUR_DEV_ERROR_DETECT == STD_ON) */

      }
      else
      {
         /* return E_NOT_OK when all sessions are blocked */
         RetVal = E_NOT_OK;
      } /* end if SessionIndex != PDUR_NO_MULTITP_SESSION_ASSIGNED */
   }

   DBG_PDUR_MULTITPTRANSMIT_EXIT(RetVal, MTpTxPduId, PduInfoPtr);

   return RetVal;
}


/*==================[internal function definitions]=========================*/

STATIC FUNC(uint8, PDUR_CODE) PduR_AssignMultiTpTxSession
(
  PduR_PduIdType MTpTxPduId
)
{
   /* loop index */
   uint8_least LoopIdx = 0U;

   /* initialize return value */
   uint8 RetValSessionIndex = PDUR_NO_MULTITP_SESSION_ASSIGNED;

   /* pointer to multicast transmit sessions */
   /* Deviation MISRAC2012-1 */
   CONSTP2VAR(PduR_MultiTpTxSessionInfoType, AUTOMATIC, PDUR_VAR_CLEARED) pMultiTpTxSessionInfo =
      PDUR_GET_RAM_ADDR(PduR_MultiTpTxSessionInfoType,
      PduR_GConfigPtr->PduR_MultiTpTxSessionInfoRef);

   DBG_PDUR_ASSIGNMULTITPTXSESSION_ENTRY(MTpTxPduId);

   /* search for multicast transmit session in use */
   while((LoopIdx < PduR_GConfigPtr->PduR_MultiTpTxRPathsMax) &&
         (pMultiTpTxSessionInfo[LoopIdx].MTpTxPduId != PDUR_INVALID_PDUID))
   {
      /* loop is left if free multicast transmit session is detected. If no free session is
         detected, LoopIdx holds the value of PduR_GConfigPtr->PduR_MultiTpTxRPathsMax */
      LoopIdx++;
   }

   /* assign free multicast transmit session */
   if(LoopIdx < PduR_GConfigPtr->PduR_MultiTpTxRPathsMax)
   {
      /* assign multicast transmit session member */
      pMultiTpTxSessionInfo[LoopIdx].MTpTxPduId = MTpTxPduId;

      /* update return value to assigned multicast transmit session index */
      RetValSessionIndex = (uint8)LoopIdx;
   }

   DBG_PDUR_ASSIGNMULTITPTXSESSION_EXIT(RetValSessionIndex, MTpTxPduId);

   return RetValSessionIndex;
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
