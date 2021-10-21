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

#if ((PDUR_LOTP_ENABLED == STD_ON) && (PDUR_ZERO_COST_OPERATION_TP == STD_OFF))
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

FUNC(void, PDUR_CODE) PduR_GenericTpRxIndication
(
   PduIdType   RxPduId,
   NotifResultType  Result,
   uint8 RTabLoTpConfigIdx
)
{
   /* pointer to configuration */
   CONSTP2CONST(PduR_RTabLoTpConfigType, AUTOMATIC, PDUR_APPL_CONST) pRTabLoTpConfig =
      &PduR_GConfigPtr->PduR_RTabLoTpConfig[RTabLoTpConfigIdx];

   DBG_PDUR_GENERICTPRXINDICATION_ENTRY(RxPduId, Result, RTabLoTpConfigIdx);

   /* check if PDU Router is initialized */
   if(PduR_State == PDUR_UNINIT)
   {
      /* according to PDUR330, no routing shall be performed in the uninitialized state */

#if (PDUR_DEV_ERROR_DETECT == STD_ON)
      /* reported to Det in development mode */
      PDUR_DET_REPORT_ERROR(PDUR_SID_LOTP_RXIND, PDUR_E_INVALID_REQUEST);
   }
   /* range check of PDU ID */
   else if (RxPduId >= pRTabLoTpConfig->RTabRxCount)
   {
       PDUR_DET_REPORT_ERROR(PDUR_SID_LOTP_RXIND, PDUR_E_PDU_ID_INVALID);
#endif /* #if (PDUR_DEV_ERROR_DETECT == STD_ON) */
   }
   else
   {
      /* pointer to routing table */
      CONSTP2CONST(PduR_RTabLoTpType, AUTOMATIC, PDUR_APPL_CONST) pTpRTabRx =
         PDUR_GET_CONFIG_ADDR(PduR_RTabLoTpType, pRTabLoTpConfig->RTabRxRef);

      /* reference to set of target Tp-module API-functions */
      const PduR_RefToModuleAPI TargetAPIRef = pTpRTabRx[RxPduId].TargetModuleAPIRef;

      /* pointer to RxIndication function */
      const PduR_TpRxIndFpType RxIndFp = PduR_TpModuleAPI[TargetAPIRef].TpRxIndication;

#if (PDUR_DEV_ERROR_DETECT == STD_ON)
      /* check if target function pointer is equal to null pointer */
      /* CHECK: NOPARSE */
      if(NULL_PTR == RxIndFp)
      {
         /* in the sense of defensive programming, call assertion if
          * target function pointer is equal to null pointer */
         PDUR_UNREACHABLE_CODE_ASSERT(PDUR_SID_LOTP_RXIND);
      }
      else
      /* CHECK: PARSE */
#endif /* #if (PDUR_DEV_ERROR_DETECT == STD_ON) */
      {
         /* get target PDU ID to RxIndication function from routing table */
         const PduR_PduIdType TargetPduId = pTpRTabRx[RxPduId].TargetPduId;

         /* call RxIndication function */
         RxIndFp(TargetPduId, Result);
      }
   }

   DBG_PDUR_GENERICTPRXINDICATION_EXIT(RxPduId, Result, RTabLoTpConfigIdx);
}

/* AUTOSAR Memory Mapping - end section for code */
#if (TS_MERGED_COMPILE == STD_OFF)
#define PDUR_STOP_SEC_CODE
#include <PduR_MemMap.h>
#endif

#else /* if (PDUR_LOTP_ENABLED == STD_ON) &&  .. */

#include <TSCompiler.h>           /* usage of macro in TSCompiler_Default.h */



/* Avoid empty translation unit according to ISO C90 */
TS_PREVENTEMPTYTRANSLATIONUNIT

/*==================[internal function definitions]=========================*/

#endif /* if (PDUR_LOTP_ENABLED == STD_ON) &&  .. */
/*==================[end of file]===========================================*/
