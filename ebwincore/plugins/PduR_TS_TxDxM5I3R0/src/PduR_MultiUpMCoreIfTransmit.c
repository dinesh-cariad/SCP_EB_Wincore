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
 *   A conversion should not be performed from pointer to void into pointer to object.
 *
 *   Reason:
 *   The memory routines are optimized for dealing with aligned memory sections.
 */

/*==================[inclusions]============================================*/

#include <PduR_Trace.h>
#include <TSAutosar.h>            /* EB specific standard types */
#include <TSAtomic_Assign.h>      /* Atomic assignments from Base PlugIn */
#include <ComStack_Types.h>       /* AUTOSAR Communication Stack types */
#include <PduR_Internal_Static.h> /* Internal API (static part) */
#include <PduR_Api_Depend.h>      /* Public API (depend part) */
#include <TSMem.h>                /* TS_MemCpy */

#if((PDUR_ZERO_COST_OPERATION_IF == STD_OFF) && \
    (PDUR_MULTICAST_TOIF_SUPPORT == STD_ON) && \
    (PDUR_MULTICORE_SUPPORT == STD_ON))
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

FUNC(Std_ReturnType, PDUR_CODE) PduR_MultiUpMCoreIfTransmit
(
   PduIdType   MPduId,
   P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) PduInfoPtr
)
{
   Std_ReturnType RetVal;

   /* pointer to routing table */
   CONSTP2CONST(PduR_MTabUpIfTxType, AUTOMATIC, PDUR_APPL_CONST) pMTabUpIfTx =
      PDUR_GET_CONFIG_ADDR(PduR_MTabUpIfTxType, PduR_GConfigPtr->PduR_MTabUpIfConfig.MTabTxRef);

   /* iterator for all entries providing the same MPduId */
   PduR_PduIdType MTabPos = (PduR_PduIdType)MPduId;

   /* BufferId */
   uint16 BufferId = PDUR_INVALID_BUFFERID;

   /* provided local variable to ease readability of while condition */
   uint8 TotalNumDests;

   /* pointer to configuration of single buffer */
   CONSTP2CONST(PduR_BufSbConfigType, AUTOMATIC, PDUR_APPL_CONST) pBufSbConfig =
      PDUR_GET_CONFIG_ADDR(PduR_BufSbConfigType, PduR_GConfigPtr->PduR_BufSbConfigRef);

   PduInfoType PduR_PduInfo;

   DBG_PDUR_MULTIUPMCOREIFTRANSMIT_ENTRY(MPduId, PduInfoPtr);

   /* initialize return value, DecouplingImprovement: name the method allowing this, i.e set if one not okay */
   RetVal = E_OK;

   /* determine some referenced single buffer for direct transmission */
   do{
      /* initialize to check while condition */
      TotalNumDests = pMTabUpIfTx[MTabPos].TotalNumDests;

      /* get BufferId for direct transmission */
      if(pMTabUpIfTx[MTabPos].BufferId != PDUR_INVALID_BUFFERID)
      {
         BufferId = pMTabUpIfTx[MTabPos].BufferId;
      }

      MTabPos++;

   /* last entry of the multicast routing table holds the number of destination PDUs */
   }while(TotalNumDests < PDUR_TOTAL_NUM_DEST_THRESHOLD);

   /* reset iterator for all entries providing the same MPduId for later usage */
   MTabPos = (PduR_PduIdType)MPduId;

   /* copy data to buffer, buffer will always be available, if no buffer is needed,
      corresponding single core API MultiUpIfTransmit is called */
   {
      /* pointer to memory location of single buffer in RAM */
      /* Deviation MISRAC2012-1 */
      CONSTP2VAR(uint8, AUTOMATIC, PDUR_VAR_CLEARED) pBufMem =
         PDUR_GET_RAM_ADDR(uint8, pBufSbConfig[BufferId].MemRef);

      /* length of single buffer */
      /* DecouplingImprovement: think about dynamic length check as length comparison */
      const PduLengthType lenSbBuffer = PduInfoPtr->SduLength;

      PduR_PduInfo.SduDataPtr = &pBufMem[0];
      PduR_PduInfo.SduLength = lenSbBuffer;

      /* ENTER CRITICAL SECTION */
      SchM_Enter_PduR_SCHM_PDUR_EXCLUSIVE_AREA_2();

      /* Copy data to single buffer of PDU Router */
      TS_MemCpy(pBufMem, PduInfoPtr->SduDataPtr, lenSbBuffer);

      /* LEAVE CRITICAL SECTION */
      SchM_Exit_PduR_SCHM_PDUR_EXCLUSIVE_AREA_2();
   }

#if  (PDUR_MULTICAST_TX_CONFIRMATION == STD_OFF)
   {
      /* loop through all routing paths matching the current MPduId */
      do{
         /* reference to set of target If-module API-functions */
         const PduR_RefToModuleAPI TargetAPIRef = pMTabUpIfTx[MTabPos].TargetModuleAPIRef;

         /* pointer to Transmit function */
         const PduR_TransmitFpType TransmitFp = PduR_IfModuleAPI[TargetAPIRef].IfTransmit;

         /* get target PDU ID */
         const PduR_PduIdType TargetPduId = pMTabUpIfTx[MTabPos].TargetPduId;

         /* initialize to check while condition */
         TotalNumDests = pMTabUpIfTx[MTabPos].TotalNumDests;

         /* E_NOT_OK is returned by this function if one of the Transmit functions fails */
         if(TransmitFp(TargetPduId, &PduR_PduInfo) != E_OK)
         {
            RetVal = E_NOT_OK;
         }

         MTabPos++;

      /* last entry of the multicast routing table holds the number of destination PDUs */
      }while(TotalNumDests < PDUR_TOTAL_NUM_DEST_THRESHOLD);
   }
#else
   {
      /* pointer to multicast transmission flags */
      /* Deviation MISRAC2012-1 <+2> */
      CONSTP2VAR(uint8, AUTOMATIC, PDUR_VAR_CLEARED) pMultiIfTxInfo =
         PDUR_GET_RAM_ADDR(uint8, PduR_GConfigPtr->PduR_MultiIfTxInfoRef);

      PduR_PduIdType ByteIdx;
      uint8 TransmitBitIdx;
      uint8 TxConfBitIdx;

      uint8 AtLeastOneTxConfExpected = 0U;

      /* loop through all routing paths matching the current MPduId */
      do{
         /* reference to set of target If-module API-functions */
         const PduR_RefToModuleAPI TargetAPIRef = pMTabUpIfTx[MTabPos].TargetModuleAPIRef;

         /* pointer to Transmit function */
         const PduR_TransmitFpType TransmitFp = PduR_IfModuleAPI[TargetAPIRef].IfTransmit;

         /* get target PDU ID */
         const PduR_PduIdType TargetPduId = pMTabUpIfTx[MTabPos].TargetPduId;

         boolean IsThisRetValNOK = FALSE;
         uint8 IsTxConfEnabled = pMTabUpIfTx[MTabPos].TxConfEnabled;

         /* get indexes for PduR_MultiIfTxInfo[] */
         ByteIdx = PduR_MultiIfTxGetByteIndex(MTabPos, &TransmitBitIdx, &TxConfBitIdx);

         /* initialize to check while condition */
         TotalNumDests = pMTabUpIfTx[MTabPos].TotalNumDests;

         /* reset flags since PduR_MultiUpMCoreIfTransmit might be called after Timeout of upper layer,
            i.e. TxConfirmation pending */
         TS_AtomicClearBit_8(&pMultiIfTxInfo[ByteIdx], TransmitBitIdx);
         TS_AtomicClearBit_8(&pMultiIfTxInfo[ByteIdx], TxConfBitIdx);

         /* E_NOT_OK is returned by this function if one of the Transmit functions fails */
         if(TransmitFp(TargetPduId, &PduR_PduInfo) != E_OK)
         {
            RetVal = E_NOT_OK;
            IsThisRetValNOK = TRUE;
         }

         /* Handle Transmit flag - all set, all Transmits called, init with 0 */
         TS_AtomicSetBit_8(&pMultiIfTxInfo[ByteIdx], TransmitBitIdx);

         /* Handle TxConfirmation flag - all set, all TxConf received, init with 0 */
         if(IsThisRetValNOK || (IsTxConfEnabled == 0U))
         {
            /* set for the cases no TxConfirmation is expected */
            TS_AtomicSetBit_8(&pMultiIfTxInfo[ByteIdx], TxConfBitIdx);
         }
         else
         {
            AtLeastOneTxConfExpected |= 1U;
         }

         MTabPos++;

      /* last entry of the multicast routing table holds the number of destination PDUs */
      }while(TotalNumDests < PDUR_TOTAL_NUM_DEST_THRESHOLD);

      /* call Up_TxConfirmation when the last enabled PduR_LoTxConfirmation was received before the
         last Lo_Transmit call is sent, e.g. last TxConfirmation disabled */
      {
         uint8 SendUpTxConf = 1U;
         uint8 tempTransmit;
         uint8 tempTxConf;

         /* avoid concurrent access to flags by last PduR_LoTxConfirmation */
         /* ENTER CRITICAL SECTION */
         SchM_Enter_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

         /* loop through all destination PDUs with TotalNumDests obtained in do-while loop above */
         for(MTabPos = (PduR_PduIdType)MPduId; MTabPos < (MPduId + TotalNumDests); MTabPos++)
         {
            /* get indexes for PduR_MultiIfTxInfo[] */
            ByteIdx = PduR_MultiIfTxGetByteIndex(MTabPos, &TransmitBitIdx, &TxConfBitIdx);

            TS_GetBit(&pMultiIfTxInfo[ByteIdx], TransmitBitIdx, uint8, tempTransmit);
            SendUpTxConf = SendUpTxConf & tempTransmit;

            TS_GetBit(&pMultiIfTxInfo[ByteIdx], TxConfBitIdx, uint8, tempTxConf);
            SendUpTxConf = SendUpTxConf & tempTxConf;
         }

         /* EXIT CRITICAL SECTION */
         SchM_Exit_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

         /* outstanding Up_TxConfirmation, all expected PduR_LoTxConfirmation are called but not
            all Lo_Transmit, e.g. last TxConfirmation disabled or last Transmit failed */
         if(SendUpTxConf == 1U)
         {
            if(AtLeastOneTxConfExpected == 1U)
            {
               /* get upper layer PDU-ID */
               const PduR_PduIdType UprLyrTxPduId = pMTabUpIfTx[MPduId].UprLyrTxPduId;

               /* Note: The target PDU-ID of an adjacent layer must be enabled since
                        PduRDestPduHandleId of the related routing path is enabled too. This
                        is ensured by a generator check.
                        So, checking here for a disabled target PDU-ID is not required. */

               /* reference to set of upper layer module API-functions */
               const PduR_RefToModuleAPI TargetAPIRef = pMTabUpIfTx[MPduId].SourceModuleAPIRef;

               /* pointer to TxConfirmation function */
               const PduR_IfTxConfFpType TxConfirmationFp = PduR_IfModuleAPI[TargetAPIRef].IfTxConfirmation;

               /* call Up_TxConfirmation function */
               TxConfirmationFp(UprLyrTxPduId);
            }

            /* Reset all flags of routing path, basically no protection required since no
               PduR_LoTxConfirmation awaited */
            for(MTabPos = (PduR_PduIdType)MPduId; MTabPos < (MPduId + TotalNumDests); MTabPos++)
            {
               /* get indexes for PduR_MultiIfTxInfo[] */
               ByteIdx = PduR_MultiIfTxGetByteIndex(MTabPos, &TransmitBitIdx, &TxConfBitIdx);

               TS_AtomicClearBit_8(&pMultiIfTxInfo[ByteIdx], TransmitBitIdx);
               TS_AtomicClearBit_8(&pMultiIfTxInfo[ByteIdx], TxConfBitIdx);
            }
         }
      }
   }
#endif /* (PDUR_MULTICAST_TX_CONFIRMATION == STD_OFF) */

   DBG_PDUR_MULTIUPMCOREIFTRANSMIT_EXIT(RetVal, MPduId, PduInfoPtr);

   return RetVal;
}

/* AUTOSAR Memory Mapping - end section for code */
#if (TS_MERGED_COMPILE == STD_OFF)
#define PDUR_STOP_SEC_CODE
#include <PduR_MemMap.h>
#endif

#else /* if (PDUR_ZERO_COST_OPERATION_IF == STD_OFF) && .. */

#include <TSCompiler.h>           /* usage of macro in TSCompiler_Default.h */

/* Avoid empty translation unit according to ISO C90 */
TS_PREVENTEMPTYTRANSLATIONUNIT


/*==================[internal function definitions]=========================*/

#endif /* if (PDUR_ZERO_COST_OPERATION_IF == STD_OFF) && .. */
/*==================[end of file]===========================================*/
