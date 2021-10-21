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

#if ((PDUR_UPTP_ENABLED == STD_ON) && (PDUR_ZERO_COST_OPERATION_TP == STD_OFF))
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

FUNC (Std_ReturnType, PDUR_CODE) PduR_GenericTpTransmit
(
   PduIdType TxPduId,
   P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) PduInfoPtr,
   uint8 RTabUpTpConfigIdx
)
{
   /* initialize return value */
   Std_ReturnType RetVal = E_NOT_OK;

   /* pointer to configuration */
   CONSTP2CONST(PduR_RTabUpTpConfigType, AUTOMATIC, PDUR_APPL_CONST) pRTabUpTpConfig =
      &PduR_GConfigPtr->PduR_RTabUpTpConfig[RTabUpTpConfigIdx];

   DBG_PDUR_GENERICTPTRANSMIT_ENTRY(TxPduId, PduInfoPtr, RTabUpTpConfigIdx);

   /* check if PDU Router is initialized */
   if(PduR_State == PDUR_UNINIT)
   {
      /* according to PDUR330, no routing shall be performed in the uninitialized state */

#if (PDUR_DEV_ERROR_DETECT == STD_ON)
      /* reported to Det in development mode */
      PDUR_DET_REPORT_ERROR(PDUR_SID_UP_TX, PDUR_E_INVALID_REQUEST);
   }
   /* range check of PDU ID */
   else if (TxPduId >= pRTabUpTpConfig->RTabTxCount)
   {
      PDUR_DET_REPORT_ERROR(PDUR_SID_UP_TX, PDUR_E_PDU_ID_INVALID);
   }
   /* check if input parameter is not NULL */
   else if (PduInfoPtr == NULL_PTR)
   {
      PDUR_DET_REPORT_ERROR(PDUR_SID_UP_TX, PDUR_E_NULL_POINTER);
#endif /* #if (PDUR_DEV_ERROR_DETECT == STD_ON) */
   }
   else
   {
      /* pointer to routing table */
      CONSTP2CONST(PduR_RTabUpTpType, AUTOMATIC, PDUR_APPL_CONST) pRTabUpTpTx =
         PDUR_GET_CONFIG_ADDR(PduR_RTabUpTpType, pRTabUpTpConfig->RTabTxRef);

#if (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON)
      /* get destination PDU routing path group ID from routing table */
      const PduR_RoutingPathGroupIdType GroupId = pRTabUpTpTx[TxPduId].RPathGroupId;

      /* destination is either enabled or not referenced by a routing path group,
         Note: Order of OR condition on purpose for faster evaluation and preventing call
         of PduR_IsRoutingPathGroupEnabled with PDUR_ROUTINGPATHGROUP_GO_THROUGH_ID */
      /* Deviation MISRAC2012-1 */
      if((GroupId == PDUR_ROUTINGPATHGROUP_GO_THROUGH_ID) ||
         (PduR_IsRoutingPathGroupEnabled(GroupId) != FALSE))
#endif /* (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON) */
      {
         /* reference to set of target Tp-module API-functions */
         const PduR_RefToModuleAPI TargetAPIRef = pRTabUpTpTx[TxPduId].TargetModuleAPIRef;

         /* pointer to Transmit function */
         const PduR_TransmitFpType TransmitFp = PduR_TpModuleAPI[TargetAPIRef].TpTransmit;

#if (PDUR_DEV_ERROR_DETECT == STD_ON)
         /* check if target function pointer is equal to null pointer */
         /* CHECK: NOPARSE */
         if(NULL_PTR == TransmitFp)
         {
            /* in the sense of defensive programming, call assertion if
             * target function pointer is equal to null pointer */
            PDUR_UNREACHABLE_CODE_ASSERT(PDUR_SID_UP_TX);
         }
         else
         /* CHECK: PARSE */
#endif /* #if (PDUR_DEV_ERROR_DETECT == STD_ON) */
         {
            /* get target PDU ID */
            const PduR_PduIdType TargetPduId = pRTabUpTpTx[TxPduId].TargetPduId;

            /* call Transmit function */
            RetVal = TransmitFp(TargetPduId, PduInfoPtr);

            /* development error detection handling for rejected transmit call */
#if (PDUR_DEV_ERROR_DETECT == STD_ON)
            /* handle rejected Transmit call */
            if(RetVal != E_OK)
            {
               PDUR_DET_REPORT_ERROR(PDUR_SID_UP_TX, PDUR_E_TP_TX_REQ_REJECTED);
            }
#endif /* if (PDUR_DEV_ERROR_DETECT == STD_ON) */
         }
      }
   }


   DBG_PDUR_GENERICTPTRANSMIT_EXIT(RetVal, TxPduId, PduInfoPtr, RTabUpTpConfigIdx);
   return RetVal;
}

/* AUTOSAR Memory Mapping - end section for code */
#if (TS_MERGED_COMPILE == STD_OFF)
#define PDUR_STOP_SEC_CODE
#include <PduR_MemMap.h>
#endif

#else /* if (PDUR_UPTP_ENABLED == STD_ON) && .. */

#include <TSCompiler.h>           /* usage of macro in TSCompiler_Default.h */

/* shift the TS_PREVENTEMPTYTRANSLATIONUNIT to a line different to all others (400 - 450)
 * in order to avoid MISRA-C:2012 Rule 05.6 violation */









































































































































































































































































































/* Avoid empty translation unit according to ISO C90 */
TS_PREVENTEMPTYTRANSLATIONUNIT

/*==================[internal function definitions]=========================*/

#endif /* if (PDUR_UPTP_ENABLED == STD_ON) && .. */
/*==================[end of file]===========================================*/
