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

FUNC(void, PDUR_CODE) PduR_MultiTpTxConfirmation
(
   PduIdType MTpTxPduId,
   NotifResultType Result
)
{
   /* determine multicast transmit session of already ongoing transmission */
   const uint8 SessionIndex =  PduR_SearchMultiTpTxSessionIndex((PduR_PduIdType)MTpTxPduId);

   DBG_PDUR_MULTITPTXCONFIRMATION_ENTRY(MTpTxPduId, Result);

   /* CopyTxData called without preceding Transmit */
#if (PDUR_DEV_ERROR_DETECT == STD_ON)
   if(SessionIndex == PDUR_NO_MULTITP_SESSION_ASSIGNED)
   {
      PDUR_DET_REPORT_ERROR(PDUR_SID_LOTP_TX_CONF, PDUR_E_INVALID_REQUEST);
   }
   else
#endif /* if (PDUR_DEV_ERROR_DETECT == STD_ON) */
   {
       /* pending Transmit calls */
       uint8 PendingTxCount;

      /* pointer to routing table */
      CONSTP2CONST(PduR_MTabUpTpTxType, AUTOMATIC, PDUR_APPL_CONST) pMTabUpTpTx =
         PDUR_GET_CONFIG_ADDR(PduR_MTabUpTpTxType, PduR_GConfigPtr->PduR_MTabUpTpConfig.MTabTxRef);

      /* pointer to multicast transmit sessions */
      /* Deviation MISRAC2012-1 */
      CONSTP2VAR(PduR_MultiTpTxSessionInfoType, AUTOMATIC, PDUR_VAR_CLEARED) pMultiTpTxSessionInfo =
         PDUR_GET_RAM_ADDR(PduR_MultiTpTxSessionInfoType, PduR_GConfigPtr->PduR_MultiTpTxSessionInfoRef);

      /* ENTER CRITICAL SECTION */
      SchM_Enter_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

#if (PDUR_MULTICAST_TP_HIGHEST_WINS_STRATEGY == STD_OFF)
      /* lowest wins:
       * if at least one Result value differs from NTFRSLT_OK, this is returned
       * with parameter Result by Up_TpTxConfirmation */
      if((pMultiTpTxSessionInfo[SessionIndex].ResultTxConf == NTFRSLT_OK) &&
         (Result != NTFRSLT_OK))
      {
         pMultiTpTxSessionInfo[SessionIndex].ResultTxConf = Result;

         /* indicate abort of transmission to CopyTxData */
         TS_AtomicSetBit_8(&pMultiTpTxSessionInfo[SessionIndex].Status, PDUR_MULTITPTX_ABORT_POS);
      }
#else /* (PDUR_MULTICAST_TP_HIGHEST_WINS_STRATEGY == STD_OFF) */
      /* highest wins:
         if at least one Result value is NTFRSLT_OK, this is returned with parameter Result by
         Up_TpTxConfirmation.
         NTFRSLT_E_NOT_OK is returned if no single Result is okay (conform to meanwhile changed
         type Std_ReturnType that should use normally E_OK and E_NOT_OK only by SWS_Std_00011) */
      if((pMultiTpTxSessionInfo[SessionIndex].ResultTxConf != NTFRSLT_OK) &&
         (Result == NTFRSLT_OK))
      {
         pMultiTpTxSessionInfo[SessionIndex].ResultTxConf = Result;
      }
#endif /* if (PDUR_MULTICAST_TP_HIGHEST_WINS_STRATEGY == STD_OFF) */

      /* update counter of multicast transmit session member Status */
      pMultiTpTxSessionInfo[SessionIndex].Status--;

      /* actual pending Transmit calls */
      PendingTxCount = pMultiTpTxSessionInfo[SessionIndex].Status & PDUR_MULTITPTX_PENDTXCOUNT_MSK;

      /* EXIT CRITICAL SECTION */
      SchM_Exit_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

      /* release multicast transmit session after final call of this function */
      if(PDUR_MULTITPTX_NO_PENDING_TX_MSK == PendingTxCount)
      {
         /* reference to set of target Tp-module API-functions */
         const PduR_RefToModuleAPI TargetAPIRef = pMTabUpTpTx[MTpTxPduId].SourceModuleAPIRef;

         /* pointer to TpTxConfirmation function */
         const PduR_TpTxConfFpType TxConfirmationFp = PduR_TpModuleAPI[TargetAPIRef].TpTxConfirmation;

         /* get target PDU ID to TpTxConfirmation from routing table */
         const PduR_PduIdType TargetPduId = pMTabUpTpTx[MTpTxPduId].UprLyrTxPduId;

         /* call Up by corresponding TpTxConfirmation function */
         TxConfirmationFp(TargetPduId, pMultiTpTxSessionInfo[SessionIndex].ResultTxConf);

         /* Release session information (no critical section necessary at the end of the session) */
         PduR_ReleaseMultiTpTxSession(SessionIndex);
      }
   }

   DBG_PDUR_MULTITPTXCONFIRMATION_EXIT(MTpTxPduId, Result);
}

/* AUTOSAR Memory Mapping - end section for code */
#if (TS_MERGED_COMPILE == STD_OFF)
#define PDUR_STOP_SEC_CODE
#include <PduR_MemMap.h>
#endif

#else /* if (PDUR_ZERO_COST_OPERATION_TP == STD_OFF) && .. */

/* Avoid empty translation unit according to ISO C90 */
typedef void PduR_PreventEmptyTranslationUnit_MultiTpTxConfirmationType;

/*==================[internal function definitions]=========================*/

#endif /* if (PDUR_ZERO_COST_OPERATION_TP == STD_OFF) && .. */
/*==================[end of file]===========================================*/
