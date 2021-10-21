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
 *  MISRAC2012-2) Deviated Rule: 5.8 (required)
 *  Identifiers that define objects or functions with external linkage shall be unique.
 *
 *  Reason:
 *  This violation is a false positive.
 *  The tool does not recognize the following situation:
 *  The function identifier depends if the merge compile use
 *  case is enabled or not. The behavior is realized through a
 *  instrumented macro which enables the use case dependent
 *  function identifier.
 *
 */

/*==================[inclusions]============================================*/

#include <PduR_Trace.h>
#include <TSAutosar.h>            /* EB specific standard types */
#include <TSAtomic_Assign.h>      /* Atomic assignments from Base PlugIn */
#include <TSCompiler.h>           /* usage of macro in TSCompiler_Default.h */
#include <ComStack_Types.h>       /* AUTOSAR Communication Stack types */
#include <PduR_Internal_Static.h> /* Internal API (static part) */
#include <PduR_Api_Depend.h>      /* Public API (depend part) */

#if (PDUR_ZERO_COST_OPERATION_TP == STD_OFF)
/* Attention: avoid double inclusion of SchM_PduR.h in PduR_Merged.c by proper
 * selection of the pre-compile switches */
#if (PDUR_MULTICAST_UPTOLOTP_SUPPORT == STD_ON)
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

/* Deviation MISRAC2012-2 */
TS_MOD_PRIV_DEFN FUNC(uint8, PDUR_CODE) PduR_SearchMultiTpTxSessionIndex
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
      PDUR_GET_RAM_ADDR(PduR_MultiTpTxSessionInfoType, PduR_GConfigPtr->PduR_MultiTpTxSessionInfoRef);

#if (PDUR_MULTICORE_SUPPORT == STD_ON)
   /* get the first MTpTxPduId of multicast routing table (to distinguish Proxy and core local for multicore) */
   const PduR_PduIdType FirstMTpTxPduId = PduR_GetFirstMTabUpTpTxEntry(MTpTxPduId);
#else
   /* get the first MTpTxPduId of multicast routing table (all entries the same for single core) */
   const PduR_PduIdType FirstMTpTxPduId = MTpTxPduId;
#endif /* (PDUR_MULTICORE_SUPPORT == STD_ON) */

   DBG_PDUR_SEARCHMULTITPTXSESSIONINDEX_ENTRY(MTpTxPduId);

   /* search for multicast transmit session in use */
   while((LoopIdx < PduR_GConfigPtr->PduR_MultiTpTxRPathsMax) &&
         (pMultiTpTxSessionInfo[LoopIdx].MTpTxPduId != FirstMTpTxPduId))
   {
      /* loop is left if assigned multicast transmit session of considered PduId is detected.
         If it is not detected, LoopIdx holds the value of PduR_GConfigPtr->PduR_MultiTpTxRPathsMax */
      LoopIdx++;
   }

   /* multicast transmit session detected */
   if(LoopIdx < PduR_GConfigPtr->PduR_MultiTpTxRPathsMax)
   {
      /* update return value to detected multicast transmit session index */
      RetValSessionIndex = (uint8)LoopIdx;
   }

   DBG_PDUR_SEARCHMULTITPTXSESSIONINDEX_EXIT(RetValSessionIndex, MTpTxPduId);

   return RetValSessionIndex;
}

/* Deviation MISRAC2012-2 */
TS_MOD_PRIV_DEFN FUNC(void, PDUR_CODE) PduR_ReleaseMultiTpTxSession
(
   uint8 SessionIndex
)
{
   /* pointer to multicast transmit sessions */
   /* Deviation MISRAC2012-1 */
   CONSTP2VAR(PduR_MultiTpTxSessionInfoType, AUTOMATIC, PDUR_VAR_CLEARED) pMultiTpTxSessionInfo =
     PDUR_GET_RAM_ADDR(PduR_MultiTpTxSessionInfoType, PduR_GConfigPtr->PduR_MultiTpTxSessionInfoRef);

   DBG_PDUR_RELEASEMULTITPTXSESSION_ENTRY(SessionIndex);

   /* reset Status */
   TS_AtomicAssign8(pMultiTpTxSessionInfo[SessionIndex].Status,
      (uint8)PDUR_MULTITPTX_RESET_STATUS_VAL);

   /* reset multicast PduId */
   pMultiTpTxSessionInfo[SessionIndex].MTpTxPduId = (PduR_PduIdType)PDUR_INVALID_PDUID;

   /* reset SduLength */
   pMultiTpTxSessionInfo[SessionIndex].SduLength = PDUR_SDU_LENGTH_ZERO;

   /* reset parameter Result of TpTxConfirmation */
#if (PDUR_MULTICAST_TP_HIGHEST_WINS_STRATEGY == STD_OFF)
   pMultiTpTxSessionInfo[SessionIndex].ResultTxConf = NTFRSLT_OK;
#else /* (PDUR_MULTICAST_TP_HIGHEST_WINS_STRATEGY == STD_OFF)   */
   pMultiTpTxSessionInfo[SessionIndex].ResultTxConf = NTFRSLT_E_NOT_OK;
#endif /* if (PDUR_MULTICAST_TP_HIGHEST_WINS_STRATEGY == STD_OFF) */

   DBG_PDUR_RELEASEMULTITPTXSESSION_EXIT(SessionIndex);
}

#if (PDUR_MULTICORE_SUPPORT == STD_ON)
/* Deviation MISRAC2012-2 */
TS_MOD_PRIV_DEFN FUNC(PduR_PduIdType, PDUR_CODE) PduR_GetFirstMTabUpTpTxEntry
(
   PduR_PduIdType MTpTxPduId
)
{
   /* start value to search through the multicast transmit routing path entries */
   PduR_PduIdType MTabPos = MTpTxPduId;

   /* total number of destinations of the multicast transmit routing path */
   uint8 TotalNumDests;

   /* pointer to routing table */
   CONSTP2CONST(PduR_MTabUpTpTxType, AUTOMATIC, PDUR_APPL_CONST) pMTabUpTpTx =
      PDUR_GET_CONFIG_ADDR(PduR_MTabUpTpTxType, PduR_GConfigPtr->PduR_MTabUpTpConfig.MTabTxRef);

   DBG_PDUR_GETFIRSTMTABUPTPTXENTRY_ENTRY(MTpTxPduId);

   do{
      /* set to check do-while loop condition (only the last entry is different to 0) */
      TotalNumDests = pMTabUpTpTx[MTabPos].TotalNumDests;

      /* to get the next entry in multicast routing path */
      MTabPos++;

   }while(TotalNumDests < PDUR_TOTAL_NUM_DEST_THRESHOLD);

   DBG_PDUR_GETFIRSTMTABUPTPTXENTRY_EXIT((MTabPos - TotalNumDests), MTpTxPduId);

   return (MTabPos - TotalNumDests);
}
#endif /* (PDUR_MULTICORE_SUPPORT == STD_ON) */

/* AUTOSAR Memory Mapping - end section for code */
#if (TS_MERGED_COMPILE == STD_OFF)
#define PDUR_STOP_SEC_CODE
#include <PduR_MemMap.h>
#endif

/*==================[internal function definitions]=========================*/
#endif /* (PDUR_MULTICAST_UPTOLOTP_SUPPORT == STD_ON) */
#endif /* if (PDUR_ZERO_COST_OPERATION_TP == STD_OFF) */

/* Avoid empty translation unit according to ISO C90 */
TS_PREVENTEMPTYTRANSLATIONUNIT

/*==================[end of file]===========================================*/
