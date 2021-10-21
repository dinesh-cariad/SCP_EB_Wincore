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

FUNC(BufReq_ReturnType, PDUR_CODE) PduR_MultiTpCopyTxData
(
   PduIdType MTpTxPduId,
   P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) PduInfoPtr,
   P2VAR(RetryInfoType, AUTOMATIC, PDUR_APPL_DATA) RetryInfoPtr,
   P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) AvailableDataPtr
)
{
   /* return value */
   BufReq_ReturnType RetVal = BUFREQ_E_NOT_OK;

   /* determine multicast transmit session of already ongoing transmission */
   const uint8 SessionIndex =  PduR_SearchMultiTpTxSessionIndex((PduR_PduIdType)MTpTxPduId);

   DBG_PDUR_MULTITPCOPYTXDATA_ENTRY(MTpTxPduId, PduInfoPtr, RetryInfoPtr, AvailableDataPtr);

   /* CopyTxData called without preceding Transmit */
#if (PDUR_DEV_ERROR_DETECT == STD_ON)
   if(SessionIndex == PDUR_NO_MULTITP_SESSION_ASSIGNED)
   {
      PDUR_DET_REPORT_ERROR(PDUR_SID_LOTP_COPY_TX_DATA, PDUR_E_INVALID_REQUEST);
      /* init value BUFREQ_E_NOT_OK is returned */
   }
   else
#endif /* if (PDUR_DEV_ERROR_DETECT == STD_ON) */
   {
      /* pointer to multicast transmit sessions */
      /* Deviation MISRAC2012-1 */
      CONSTP2VAR(PduR_MultiTpTxSessionInfoType, AUTOMATIC, PDUR_VAR_CLEARED) pSessionInfo =
        PDUR_GET_RAM_ADDR(PduR_MultiTpTxSessionInfoType,
                          PduR_GConfigPtr->PduR_MultiTpTxSessionInfoRef);

      uint8 ResultGetBitTxComplete;
      uint8 ResultGetBitTxAbort;

      /* provide RetryInfo to upper layer independent of the lower layer's RetryInfoPtr */
      RetryInfoType RetryInfo;

      /* pointer to upper CopyTxData function */
      PduR_CopyTxDataFpType CopyTxDataFp = NULL_PTR;

      /* get target PDU ID to CopyTxData function from routing table */
      PduR_PduIdType TargetPduId = PDUR_INVALID_PDUID;

      /* ENTER CRITICAL SECTION */
      SchM_Enter_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

      /* abort Transmit */
      TS_GetBit(&pSessionInfo[SessionIndex].Status,
         PDUR_MULTITPTX_ABORT_POS, uint8, ResultGetBitTxAbort);

      /* all LoTp_Transmit calls are executed */
      TS_GetBit(&pSessionInfo[SessionIndex].Status,
         PDUR_MULTITPTX_TX_COMPLETE_POS, uint8, ResultGetBitTxComplete);

      /* check if transmission shall be aborted (e.g. LoTp_Transmit call failed) */
      if(ResultGetBitTxAbort > 0U)
      {
         /* init value BUFREQ_E_NOT_OK is returned */
      }
      /* wait until all LoTp_Transmit calls are executed for considered PduId */
      else if(ResultGetBitTxComplete == 0U)
      {
         RetVal = BUFREQ_E_BUSY;
      }
      else
      {
         /* multicast transmit is allowed only for single frames by SWS */
         const PduLengthType SizeSingleFrame = pSessionInfo[SessionIndex].SduLength;

         /* SduLength equal zero, i.e. request size of available data */
         if(PduInfoPtr->SduLength == PDUR_SDU_LENGTH_ZERO)
         {
            /* the available size of data to be transmitted holds the complete single frame */
            *AvailableDataPtr = SizeSingleFrame;
            RetVal = BUFREQ_OK;
         }
         /* abort if retry size is different to size of single frame, i.e. it can not be rewound */
         else if((RetryInfoPtr != NULL_PTR) &&
                 ((RetryInfoPtr->TpDataState == TP_DATARETRY) &&
                  (RetryInfoPtr->TxTpDataCnt != SizeSingleFrame)))
         {
            /* init value BUFREQ_E_NOT_OK is returned */
         }
         /* SduLength of the size of the single frame */
         else if(PduInfoPtr->SduLength == SizeSingleFrame)
         {
            uint8 ResultGetBitUpCopyTxDataProt;

            /* get bit from global Status variable close to usage, to ensure that it did not
             * change by call of CopyTxData from a different destination PDU */
            TS_GetBit(&pSessionInfo[SessionIndex].Status, PDUR_MULTITPTX_UP_COPYTX_PROT_POS, uint8,
               ResultGetBitUpCopyTxDataProt);

            if(0U == ResultGetBitUpCopyTxDataProt)
            {
               /* pointer to routing table */
               CONSTP2CONST(PduR_MTabUpTpTxType, AUTOMATIC, PDUR_APPL_CONST) pMTabUpTpTx =
                  PDUR_GET_CONFIG_ADDR(PduR_MTabUpTpTxType,
                                       PduR_GConfigPtr->PduR_MTabUpTpConfig.MTabTxRef);

               /* reference to set of API functions */
               const PduR_RefToModuleAPI TargetAPIRef = pMTabUpTpTx[MTpTxPduId].SourceModuleAPIRef;

               uint8 ResultGetBit1stCopyTxData;

               /* get target PDU ID to CopyTxData function from routing table */
               TargetPduId = pMTabUpTpTx[MTpTxPduId].UprLyrTxPduId;

               /* get pointer to upper CopyTxData function from routing table */
               CopyTxDataFp = PduR_TpModuleAPI[TargetAPIRef].TpCopyTxData;

               /* set bit from global Status variable to avoid concurrent calls of Up_CopyTxData */
               TS_AtomicSetBit_8(&pSessionInfo[SessionIndex].Status,
                  PDUR_MULTITPTX_UP_COPYTX_PROT_POS);

               /* get bit from global Status variable close to usage, to ensure that it did not
                * change by call of CopyTxData from a different destination PDU */
               TS_GetBit(&pSessionInfo[SessionIndex].Status, PDUR_MULTITPTX_1ST_COPYTX_POS, uint8,
                  ResultGetBit1stCopyTxData);

               /* adjust RetryInfo for multicast -
                * first call of this function for respective PduId (bit not set) */
               if(ResultGetBit1stCopyTxData == 0U)
               {
                  /* set bit for first call of CopyTxData */
                  TS_AtomicSetBit_8(&pSessionInfo[SessionIndex].Status,
                     PDUR_MULTITPTX_1ST_COPYTX_POS);

                  /* TpDataState is set to TP_CONFPENDING for the first call */
                  RetryInfo.TpDataState = TP_CONFPENDING;

                  /* TxTpDataCnt is not considered since not defined by SWS */
               }
               /* adjust RetryInfo for multicast for the calls following the first one (bit set) */
               else
               {
                  /* TpDataState is set to TP_DATARETRY for further calls */
                  RetryInfo.TpDataState = TP_DATARETRY;

                  /* TxTpDataCnt is not defined by SWS,
                     but set to the value of the complete single frame to be transmitted */
                  RetryInfo.TxTpDataCnt = SizeSingleFrame;
               }

               /* the available size of data to be transmitted is directly set by the
                * upper layer module */
            }
            else
            {
               /* calling Up_CopyTxData by different destination is already ongoing */
               RetVal = BUFREQ_E_BUSY;
            }
         }
         /* invalid size of SduLength */
         else
         {
            /* retry if SduLength is different to size of single frame or zero,
               allows new request for available data size with SduLength 0 */
            RetVal = BUFREQ_E_BUSY;
         }
      }

      /* EXIT CRITICAL SECTION */
      SchM_Exit_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

      /* for a concurrent call of this function, the local CopyTxDataFp from stack is considered */
      if(CopyTxDataFp != NULL_PTR)
      {
         /* call Up_CopyTxData function */
         RetVal = CopyTxDataFp(TargetPduId, PduInfoPtr, &RetryInfo, AvailableDataPtr);

         TS_AtomicClearBit_8(&pSessionInfo[SessionIndex].Status, PDUR_MULTITPTX_UP_COPYTX_PROT_POS);
      }
   }

   DBG_PDUR_MULTITPCOPYTXDATA_EXIT(RetVal, MTpTxPduId, PduInfoPtr, RetryInfoPtr, AvailableDataPtr);

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

/*==================[internal function definitions]=========================*/

#endif /* if (PDUR_ZERO_COST_OPERATION_TP == STD_OFF) && .. */
/*==================[end of file]===========================================*/
