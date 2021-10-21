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
#include <ComStack_Types.h>       /* AUTOSAR Communication Stack types */
#include <PduR_Internal_Static.h> /* Internal API (static part) */
#include <PduR_Api_Depend.h>      /* Public API (depend part) */

#if((PDUR_ZERO_COST_OPERATION_IF == STD_OFF) && \
    (PDUR_MULTICAST_TOIF_SUPPORT == STD_ON) && \
    (PDUR_LOIF_TXCONF_ENABLED == STD_ON))
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

FUNC(void, PDUR_CODE) PduR_MultiIfTxConfirmation
(
   PduIdType     MPduId
)
{
   DBG_PDUR_MULTIIFTXCONFIRMATION_ENTRY(MPduId);

#if  (PDUR_MULTICAST_TX_CONFIRMATION == STD_ON)
   /* call Up_TxConfirmation when the last enabled PduR_LoTxConfirmation was received and all
      Lo_Transmit are sent */
   {
      /* initialized for do-while loop handling with bit-wise  */
      uint8 SendUpTxConf = 1U;
      /* byte index of received TxConfirmation */
      PduR_PduIdType ByteIdx;
      /* bit index for Lo_Transmit */
      uint8 TransmitBitIdx;
      /* bit index of received TxConfirmation */
      uint8 TxConfBitIdx;
      uint8 tempTransmit;
      uint8 tempTxConf;
      /* initialized for do-while loop handling */
      uint8 TotalNumDests = 0U;
      /* initialized for do-while loop handling */
      uint8 IdxNumDests = 0U;
      boolean LastEntryDetected = FALSE;

      /* pointer to routing table */
      CONSTP2CONST(PduR_MTabUpIfTxType, AUTOMATIC, PDUR_APPL_CONST) pMTabUpIfTx =
         PDUR_GET_CONFIG_ADDR(PduR_MTabUpIfTxType, PduR_GConfigPtr->PduR_MTabUpIfConfig.MTabTxRef);

      /* pointer to multicast transmission flags */
      /* Deviation MISRAC2012-1 <+2> */
      CONSTP2VAR(uint8, AUTOMATIC, PDUR_VAR_CLEARED) pMultiIfTxInfo =
         PDUR_GET_RAM_ADDR(uint8, PduR_GConfigPtr->PduR_MultiIfTxInfoRef);

      /* PDU-ID iterator for all entries of the same multicast routing path */
      PduR_PduIdType MTabPos = (PduR_PduIdType)MPduId;

      /* first MPduId of multicast routing path, initialized to avoid compiler warning
         even though the case can not happen */
      PduR_PduIdType FirstMTabPos = MTabPos;

      /* get indexes for PduR_MultiIfTxInfo[] */
      ByteIdx = PduR_MultiIfTxGetByteIndex(MTabPos, &TransmitBitIdx, &TxConfBitIdx);

      /* avoid concurrent access to flags by last PduR_LoTxConfirmation */
      /* ENTER CRITICAL SECTION */
      SchM_Enter_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

      /* set flag for received TxConfirmation */
      TS_AtomicSetBit_8(&pMultiIfTxInfo[ByteIdx], TxConfBitIdx);

      /* loop through all destination PDUs of considered routing paths */
      do{
         /* get indexes for PduR_MultiIfTxInfo[] */
         ByteIdx = PduR_MultiIfTxGetByteIndex(MTabPos, &TransmitBitIdx, &TxConfBitIdx);

         TS_GetBit(&pMultiIfTxInfo[ByteIdx], TransmitBitIdx, uint8, tempTransmit);
         SendUpTxConf = SendUpTxConf & tempTransmit;

         TS_GetBit(&pMultiIfTxInfo[ByteIdx], TxConfBitIdx, uint8, tempTxConf);
         SendUpTxConf = SendUpTxConf & tempTxConf;

         /* set to check do-while loop condition (only the last entry is different to 0) */
         TotalNumDests |= pMTabUpIfTx[MTabPos].TotalNumDests;

         /* Advance to the next entry in multicast routing path */
         MTabPos++;
         if((TotalNumDests > PDUR_TOTAL_NUM_DEST_THRESHOLD) && (LastEntryDetected == FALSE))
         {
            LastEntryDetected = TRUE;
            /* reset from last to first entry of considered multicast routing path */
            MTabPos = MTabPos - TotalNumDests;
            /* get first entry for later reset in for-loop */
            FirstMTabPos = MTabPos;
         }

         IdxNumDests++;

      /* loop through all entries of the multicast routing path, independent on entry point */
      }while(IdxNumDests != TotalNumDests);

      /* EXIT CRITICAL SECTION */
      SchM_Exit_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

      /* outstanding Up_TxConfirmation caused by final PduR_LoTxConfirmation when all Lo_Transmit
         are called */
      if(SendUpTxConf == 1U)
      {
         /* get upper layer PDU-ID */
         const PduR_PduIdType UprLyrTxPduId = pMTabUpIfTx[MPduId].UprLyrTxPduId;

         /* Note: The target PDU-ID of an adjacent layer must be enabled since
                  PduRDestPduHandleId of the related routing path is enabled too. This
                  is ensured by a generator check.
                  So, checking here for a disabled target PDU-ID is not required. */

         /* reference to set of upper layer module API-functions */
         const PduR_RefToModuleAPI SourceAPIRef = pMTabUpIfTx[MPduId].SourceModuleAPIRef;

         /* function pointer to Up_TxConfirmation */
         const PduR_IfTxConfFpType TxConfirmationFp = PduR_IfModuleAPI[SourceAPIRef].IfTxConfirmation;

         /* call Up_TxConfirmation function */
         TxConfirmationFp(UprLyrTxPduId);

         /* Reset all flags of routing path, basically no protection required since no
            PduR_LoTxConfirmation expected and all Lo_Transmit sent */
         for(MTabPos = FirstMTabPos; MTabPos < (FirstMTabPos + TotalNumDests); MTabPos++)
         {
            /* get indexes for PduR_MultiIfTxInfo[] */
            ByteIdx = PduR_MultiIfTxGetByteIndex(MTabPos, &TransmitBitIdx, &TxConfBitIdx);

            TS_AtomicClearBit_8(&pMultiIfTxInfo[ByteIdx], TransmitBitIdx);
            TS_AtomicClearBit_8(&pMultiIfTxInfo[ByteIdx], TxConfBitIdx);
         }
      }
   }
#else
   {
      /* No Up_TxConfirmation for disabled PduRMulticastTxConfirmation */
      TS_PARAM_UNUSED(MPduId);
   }
#endif

   DBG_PDUR_MULTIIFTXCONFIRMATION_EXIT(MPduId);
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
