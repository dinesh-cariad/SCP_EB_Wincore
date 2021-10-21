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

/*==================[inclusions]============================================*/

#include <PduR_Trace.h>
#include <TSAutosar.h>            /* EB specific standard types */
#include <ComStack_Types.h>       /* AUTOSAR Communication Stack types */
#include <PduR_Internal_Static.h> /* Internal API (static part) */
#include <PduR_Api_Depend.h>      /* Public API (depend part) */

#if ((PDUR_LOIF_TXCONF_ENABLED == STD_ON) && (PDUR_ZERO_COST_OPERATION_IF == STD_OFF))
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

FUNC(void, PDUR_CODE) PduR_GenericIfTxConfirmation
(
   PduIdType    TxPduId,
   uint8 RTabLoIfConfigIdx
)
{
   /* pointer to configuration */
   CONSTP2CONST(PduR_RTabLoIfConfigType, AUTOMATIC, PDUR_APPL_CONST) pRTabLoIfConfig =
      &PduR_GConfigPtr->PduR_RTabLoIfConfig[RTabLoIfConfigIdx];

   DBG_PDUR_GENERICIFTXCONFIRMATION_ENTRY(TxPduId, RTabLoIfConfigIdx);

   /* check if PDU Router is initialized */
   if(PduR_State == PDUR_UNINIT)
   {
      /* according to PDUR330, no routing shall be performed in the uninitialized state */

#if (PDUR_DEV_ERROR_DETECT == STD_ON)
      /* reported to Det in development mode */
      PDUR_DET_REPORT_ERROR(PDUR_SID_LO_TXCONF, PDUR_E_INVALID_REQUEST);
   }
   /* range check of PDU ID */
   else if (TxPduId >= pRTabLoIfConfig->RTabTxCount)
   {
       PDUR_DET_REPORT_ERROR(PDUR_SID_LO_TXCONF, PDUR_E_PDU_ID_INVALID);
#endif /* #if (PDUR_DEV_ERROR_DETECT == STD_ON) */
   }
   else
   {
      /* pointer to routing table */
      CONSTP2CONST(PduR_RTabLoIfType, AUTOMATIC, PDUR_APPL_CONST) pIfRTabTx =
         PDUR_GET_CONFIG_ADDR(PduR_RTabLoIfType, pRTabLoIfConfig->RTabTxRef);

      /* get target PDU ID */
      const PduR_PduIdType TargetPduId = pIfRTabTx[TxPduId].TargetPduId;

      /* Note: The target PDU-ID of an adjacent layer must be enabled since
               PduRDestPduHandleId of the related routing path is enabled too. This
               is ensured by a generator check.
               So, checking here for a disabled target PDU-ID is not required. */

      /* reference to set of target module API-functions */
      const PduR_RefToModuleAPI TargetAPIRef = pIfRTabTx[TxPduId].TargetModuleAPIRef;

      /* pointer to TxConfirmation function */
      const PduR_IfTxConfFpType TxConfirmationFp = PduR_IfModuleAPI[TargetAPIRef].IfTxConfirmation;

      /* Sb or Nb gateway I-PDU */
      if(NULL_PTR != TxConfirmationFp)
      {
         /* call TxConfirmation function */
         TxConfirmationFp(TargetPduId);
      }
   }

   DBG_PDUR_GENERICIFTXCONFIRMATION_EXIT(TxPduId, RTabLoIfConfigIdx);
}

/* AUTOSAR Memory Mapping - end section for code */
#if (TS_MERGED_COMPILE == STD_OFF)
#define PDUR_STOP_SEC_CODE
#include <PduR_MemMap.h>
#endif

#else /* if (PDUR_LOIF_TXCONF_ENABLED == STD_ON) && .. */

#include <TSCompiler.h>           /* usage of macro in TSCompiler_Default.h */

/* Avoid empty translation unit according to ISO C90 */
TS_PREVENTEMPTYTRANSLATIONUNIT


/*==================[internal function definitions]=========================*/

#endif /* if (PDUR_LOIF_TXCONF_ENABLED == STD_ON) && .. */
/*==================[end of file]===========================================*/
