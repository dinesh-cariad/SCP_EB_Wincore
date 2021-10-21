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
 *  MISRAC2012-1) Deviated Rule: 13.5 (required)
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

FUNC(BufReq_ReturnType, PDUR_CODE) PduR_GenericTpStartOfReception
(
   PduIdType RxPduId,
   PduLengthType TpSduLength,
   P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) BufferSizePtr,
   uint8 RTabLoTpConfigIdx
)
{
   /* initialize return value */
   BufReq_ReturnType RetVal = BUFREQ_E_NOT_OK;

   /* pointer to configuration */
   CONSTP2CONST(PduR_RTabLoTpConfigType, AUTOMATIC, PDUR_APPL_CONST) pRTabLoTpConfig =
      &PduR_GConfigPtr->PduR_RTabLoTpConfig[RTabLoTpConfigIdx];

   DBG_PDUR_GENERICTPSTARTOFRECEPTION_ENTRY(RxPduId, TpSduLength, BufferSizePtr,
                                            RTabLoTpConfigIdx);

   /* check if PDU Router is initialized */
   if(PduR_State == PDUR_UNINIT)
   {
      /* according to PDUR330, no routing shall be performed in the uninitialized state */

#if (PDUR_DEV_ERROR_DETECT == STD_ON)
      /* reported to Det in development mode */
      PDUR_DET_REPORT_ERROR(PDUR_SID_LOTP_STRT_OF_RCPTN, PDUR_E_INVALID_REQUEST);
   }
   /* range check of PDU ID */
   else if (RxPduId >= pRTabLoTpConfig->RTabRxCount)
   {
      PDUR_DET_REPORT_ERROR(PDUR_SID_LOTP_STRT_OF_RCPTN, PDUR_E_PDU_ID_INVALID);
   }
   /* check if input parameter is not NULL */
   else if (BufferSizePtr == NULL_PTR)
   {
      PDUR_DET_REPORT_ERROR(PDUR_SID_LOTP_STRT_OF_RCPTN, PDUR_E_NULL_POINTER);
#endif /* #if (PDUR_DEV_ERROR_DETECT == STD_ON) */
   }
   else
   {
      /* pointer to routing table */
      CONSTP2CONST(PduR_RTabLoTpType, AUTOMATIC, PDUR_APPL_CONST) pTpRTabRx =
         PDUR_GET_CONFIG_ADDR(PduR_RTabLoTpType, pRTabLoTpConfig->RTabRxRef);

      /* reference to set of target Tp-module API-functions */
      const PduR_RefToModuleAPI TargetAPIRef = pTpRTabRx[RxPduId].TargetModuleAPIRef;

#if (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON)
      /* get associated group ID from routing table */
      const PduR_RoutingPathGroupIdType GroupId = pTpRTabRx[RxPduId].RPathGroupId;

      /* destination is either enabled or not referenced by a routing path group,
         Note: Order of OR condition on purpose for faster evaluation and preventing call
         of PduR_IsRoutingPathGroupEnabled with PDUR_ROUTINGPATHGROUP_GO_THROUGH_ID */
      /* Deviation MISRAC2012-1 */
      if((GroupId == PDUR_ROUTINGPATHGROUP_GO_THROUGH_ID) ||
         (PduR_IsRoutingPathGroupEnabled(GroupId) != FALSE))
#endif /* (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON) */
      {
         /* pointer to StartOfReception function */
         const PduR_StrtOfRcptnFpType StrtOfRcptnFp =
            PduR_TpModuleAPI[TargetAPIRef].TpStartOfReception;

#if (PDUR_DEV_ERROR_DETECT == STD_ON)
         /* check if target function pointer is equal to null pointer */
         /* CHECK: NOPARSE */
         if(NULL_PTR == StrtOfRcptnFp)
         {
            /* in the sense of defensive programming, call assertion if
             * target function pointer is equal to null pointer */
            PDUR_UNREACHABLE_CODE_ASSERT(PDUR_SID_LOTP_STRT_OF_RCPTN);
            /* RetVal is still BUFREQ_E_NOT_OK */
         }
         else
         /* CHECK: PARSE */
#endif /* #if (PDUR_DEV_ERROR_DETECT == STD_ON) */
         {
            /* get target PDU ID to StartOfReception function from routing table */
            const PduR_PduIdType TargetPduId = pTpRTabRx[RxPduId].TargetPduId;

            /* call StartOfReception function */
            RetVal = StrtOfRcptnFp(TargetPduId, TpSduLength, BufferSizePtr);
         }
      }
   }

   DBG_PDUR_GENERICTPSTARTOFRECEPTION_EXIT(RetVal, RxPduId, TpSduLength, BufferSizePtr,
                                           RTabLoTpConfigIdx);
   return RetVal;
}

/* AUTOSAR Memory Mapping - end section for code */
#if (TS_MERGED_COMPILE == STD_OFF)
#define PDUR_STOP_SEC_CODE
#include <PduR_MemMap.h>
#endif

#else /* if (PDUR_LOTP_ENABLED == STD_ON) && .. */

#include <TSCompiler.h>           /* usage of macro in TSCompiler_Default.h */



/* Avoid empty translation unit according to ISO C90 */
TS_PREVENTEMPTYTRANSLATIONUNIT

/*==================[internal function definitions]=========================*/

#endif /* if (PDUR_LOTP_ENABLED == STD_ON) &&  .. */
/*==================[end of file]===========================================*/
