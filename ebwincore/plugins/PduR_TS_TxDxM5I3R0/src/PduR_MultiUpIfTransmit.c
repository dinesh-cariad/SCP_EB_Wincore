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
 *
 *  MISRAC2012-2) Deviated Rule: 13.5 (required)
 *   The right hand operand of a logical && or || operator shall not contain persistent side effects.
 *
 *   Reason:
 *   This violation is a false positive.
 *   The right hand operation does not contain persistent side effects. The usage of a macro is not
 *   interpreted correctly by the tool.
 *
 */

/*==================[inclusions]============================================*/

#include <PduR_Trace.h>
#include <TSAutosar.h>            /* EB specific standard types */
#include <TSAtomic_Assign.h>      /* Atomic assignments from Base PlugIn */
#include <ComStack_Types.h>       /* AUTOSAR Communication Stack types */
#include <PduR_Internal_Static.h> /* Internal API (static part) */
#include <PduR_Api_Depend.h>      /* Public API (depend part) */

#if((PDUR_ZERO_COST_OPERATION_IF == STD_OFF) && \
    (PDUR_MULTICAST_TOIF_SUPPORT == STD_ON))
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

FUNC(Std_ReturnType, PDUR_CODE) PduR_MultiUpIfTransmit
(
   PduIdType   MPduId,
   P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) PduInfoPtr
)
{
   Std_ReturnType RetVal;

   /* pointer to routing table */
   CONSTP2CONST(PduR_MTabUpIfTxType, AUTOMATIC, PDUR_APPL_CONST) pMTabUpIfTx =
      PDUR_GET_CONFIG_ADDR(PduR_MTabUpIfTxType, PduR_GConfigPtr->PduR_MTabUpIfConfig.MTabTxRef);

   DBG_PDUR_MULTIUPIFTRANSMIT_ENTRY(MPduId, PduInfoPtr);

#if  (PDUR_MULTICAST_TX_CONFIRMATION == STD_OFF)
   {
      /* iterator for all entries providing the same MPduId */
      PduR_PduIdType MTabPos = (PduR_PduIdType)MPduId;

      /* provided local variable to ease readability of while condition */
      uint8 TotalNumDests;

#if (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON)
      boolean bAtLeastOneDestEnabled = FALSE;
#endif /* (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON) */

      /* initialize return value with successful value to ease accumulation of return values for
         lowest wins strategy, i.e. return E_NOT_OK if one Transmit fails */
      RetVal = E_OK;

      /* loop through all routing paths matching the current MPduId */
      do{
#if (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON)
         /* get destination PDU routing path group ID from routing table */
         const PduR_RoutingPathGroupIdType GroupId = pMTabUpIfTx[MTabPos].RPathGroupId;

         /* destination is either enabled or not referenced by a routing path group,
            Note: Order of OR condition on purpose for faster evaluation and preventing call
            of PduR_IsRoutingPathGroupEnabled with PDUR_ROUTINGPATHGROUP_GO_THROUGH_ID */
         /* Deviation MISRAC2012-2 */
         if((GroupId == PDUR_ROUTINGPATHGROUP_GO_THROUGH_ID) ||
            (PduR_IsRoutingPathGroupEnabled(GroupId) != FALSE))
#endif /* (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON) */
         {
            /* reference to set of target If-module API-functions */
            const PduR_RefToModuleAPI TargetAPIRef = pMTabUpIfTx[MTabPos].TargetModuleAPIRef;

            /* pointer to Transmit function */
            const PduR_TransmitFpType TransmitFp = PduR_IfModuleAPI[TargetAPIRef].IfTransmit;

            /* get target PDU ID */
            const PduR_PduIdType TargetPduId = pMTabUpIfTx[MTabPos].TargetPduId;

            /* E_NOT_OK is returned by this function if one of the Transmit functions fails */
            if(TransmitFp(TargetPduId, PduInfoPtr) != E_OK)
            {
               RetVal = E_NOT_OK;
            }
#if (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON)
            /* set flag to indicate that at least one destination of this multicast destination is enabled */
            bAtLeastOneDestEnabled = TRUE;
#endif /* (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON) */
         }

         /* initialize to check while condition */
         TotalNumDests = pMTabUpIfTx[MTabPos].TotalNumDests;

         MTabPos++;

      /* last entry of the multicast routing table holds the number of destination PDUs */
      }while(TotalNumDests < PDUR_TOTAL_NUM_DEST_THRESHOLD);

#if (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON)
      /* E_NOT_OK is returned by this function  only if all destinations are disabled */
      if(bAtLeastOneDestEnabled == FALSE)
      {
         RetVal = E_NOT_OK;
      }
#endif /* (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON) */
   }
#else
   {
      /* iterator for all entries providing the same MPduId */
      PduR_PduIdType MTabPos = (PduR_PduIdType)MPduId;
      /* provided local variable to ease readability of while condition */
      uint8 TotalNumDests;
      /* pointer to multicast transmission flags */
      /* Deviation MISRAC2012-1 <+2> */
      CONSTP2VAR(uint8, AUTOMATIC, PDUR_VAR_CLEARED) pMultiIfTxInfo =
         PDUR_GET_RAM_ADDR(uint8, PduR_GConfigPtr->PduR_MultiIfTxInfoRef);

      PduR_PduIdType ByteIdx;
      uint8 TransmitBitIdx;
      uint8 TxConfBitIdx;

      uint8 AtLeastOneTxConfExpected = 0U;

#if (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON)
      boolean bAtLeastOneDestEnabled = FALSE;
#endif /* (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON) */

      /* initialize return value with successful value to ease accumulation of return values for
         lowest wins strategy, i.e. return E_NOT_OK if one Transmit fails */
      RetVal = E_OK;

      /* loop through all routing paths matching the current MPduId */
      do{
#if (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON)
         /* get destination PDU routing path group ID from routing table */
         const PduR_RoutingPathGroupIdType GroupId = pMTabUpIfTx[MTabPos].RPathGroupId;

         /* destination is either enabled or not referenced by a routing path group,
            Note: Order of OR condition on purpose for faster evaluation and preventing call
            of PduR_IsRoutingPathGroupEnabled with PDUR_ROUTINGPATHGROUP_GO_THROUGH_ID */
         /* Deviation MISRAC2012-2 */
         if((GroupId == PDUR_ROUTINGPATHGROUP_GO_THROUGH_ID) ||
            (PduR_IsRoutingPathGroupEnabled(GroupId) != FALSE))
#endif /* (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON) */
         {
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

            /* reset flags since PduR_MultiUpIfTransmit might be called after Timeout of upper layer,
               i.e. TxConfirmation pending */
            TS_AtomicClearBit_8(&pMultiIfTxInfo[ByteIdx], TransmitBitIdx);
            TS_AtomicClearBit_8(&pMultiIfTxInfo[ByteIdx], TxConfBitIdx);

            /* E_NOT_OK is returned by this function if one of the Transmit functions fails */
            if(TransmitFp(TargetPduId, PduInfoPtr) != E_OK)
            {
               RetVal = E_NOT_OK;
               IsThisRetValNOK = TRUE;
            }

#if (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON)
            /* set flag to indicate that at least one destination of this multicast destination is enabled */
            bAtLeastOneDestEnabled = TRUE;
#endif /* (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON) */

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
         }
#if (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON)
         else
         {
            /* get indexes for PduR_MultiIfTxInfo[] */
            ByteIdx = PduR_MultiIfTxGetByteIndex(MTabPos, &TransmitBitIdx, &TxConfBitIdx);

            /* This destination is not enabled set bit to indicate as if this Lo_Transmit had been called */
            TS_AtomicSetBit_8(&pMultiIfTxInfo[ByteIdx], TransmitBitIdx);
            /* set for the cases no TxConfirmation is expected */
            TS_AtomicSetBit_8(&pMultiIfTxInfo[ByteIdx], TxConfBitIdx);
         }
#endif /* (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON) */

         /* initialize to check while condition */
         TotalNumDests = pMTabUpIfTx[MTabPos].TotalNumDests;

         MTabPos++;

      /* last entry of the multicast routing table holds the number of destination PDUs */
      }while(TotalNumDests < PDUR_TOTAL_NUM_DEST_THRESHOLD);

#if (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON)
      /* if all multicast destinations of this PDU are disabled, no action at all will have to be carried out */
      if (bAtLeastOneDestEnabled != FALSE)
#endif /* (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON) */
      /* at least one of the multicast destinations is enabled */
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
            /* get indexes from PduR_MultiIfTxInfo[] */
            ByteIdx = PduR_MultiIfTxGetByteIndex(MTabPos, &TransmitBitIdx, &TxConfBitIdx);

            /* Up_TxConfirmation is triggered when:
             * 1) all Lo_Transmits are called
             * 2) all expected PduR_LoTxConfirmations are received (no further call to
             *    PduR_MultiIfTxConfirmation expected)
             * 3) PduRTxConfirmation for at least one destination is configured */

            /* Find the state of the 'Lo_Transmit called' bit */
            TS_GetBit(&pMultiIfTxInfo[ByteIdx], TransmitBitIdx, uint8, tempTransmit);
            SendUpTxConf = SendUpTxConf & tempTransmit;

            /* Find the state of the 'PduR_LoTxConfirmation received' bit */
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
#if (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON)
      else
      { /* E_NOT_OK is returned by this function if all multicast destinations are disabled */
        RetVal = E_NOT_OK;
      }
#endif /* (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON) */
   }
#endif /* (PDUR_MULTICAST_TX_CONFIRMATION == STD_OFF) */

   DBG_PDUR_MULTIUPIFTRANSMIT_EXIT(RetVal, MPduId, PduInfoPtr);

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

#endif /* if ((PDUR_ZERO_COST_OPERATION_IF == STD_OFF) &&
              (PDUR_MULTICAST_TOIF_SUPPORT == STD_ON)) */
/*==================[end of file]===========================================*/
