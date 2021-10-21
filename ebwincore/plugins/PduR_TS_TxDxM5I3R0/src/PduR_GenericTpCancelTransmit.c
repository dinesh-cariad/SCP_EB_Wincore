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

#if ((PDUR_UPTP_CANCELTX_ENABLED == STD_ON) && (PDUR_ZERO_COST_OPERATION_TP == STD_OFF))
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

FUNC(Std_ReturnType, PDUR_CODE) PduR_GenericTpCancelTransmit
(
   PduIdType   TxPduId,
   uint8 RTabUpTpConfigIdx
)
{
   /* initialize return value */
   Std_ReturnType RetVal = E_NOT_OK;

   /* pointer to configuration */
   CONSTP2CONST(PduR_RTabUpTpConfigType, AUTOMATIC, PDUR_APPL_CONST) pRTabUpTpConfig =
       &PduR_GConfigPtr->PduR_RTabUpTpConfig[RTabUpTpConfigIdx];

   DBG_PDUR_GENERICTPCANCELTRANSMIT_ENTRY(TxPduId, RTabUpTpConfigIdx);

   /* check if PDU Router is initialized */
   if(PduR_State == PDUR_UNINIT)
   {
      /* according to PDUR330, no routing shall be performed in the uninitialized state */

#if (PDUR_DEV_ERROR_DETECT == STD_ON)
      /* reported to Det in development mode */
      PDUR_DET_REPORT_ERROR(PDUR_SID_UP_CANCELTXREQ, PDUR_E_INVALID_REQUEST);
#endif /* #if (PDUR_DEV_ERROR_DETECT == STD_ON) */
   }

#if (PDUR_DEV_ERROR_DETECT == STD_ON)
   /* range check of PDU ID */
   else if (TxPduId >= pRTabUpTpConfig->RTabTxCount)
   {
      PDUR_DET_REPORT_ERROR(PDUR_SID_UP_CANCELTXREQ, PDUR_E_PDU_ID_INVALID);
   }
#endif /* #if (PDUR_DEV_ERROR_DETECT == STD_ON) */
   else
   {
      /* pointer to routing table */
      CONSTP2CONST(PduR_RTabUpTpType, AUTOMATIC, PDUR_APPL_CONST) pRTabUpTpTx =
         PDUR_GET_CONFIG_ADDR(PduR_RTabUpTpType, pRTabUpTpConfig->RTabTxRef);

#if (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON)
      /* get destination PDU routing path group ID from routing table */
      const PduR_RoutingPathGroupIdType GroupId = pRTabUpTpTx[TxPduId].RPathGroupId;

      /* check if the routing path group of this destination PDU is enabled or
         if this destination is not contained in a routing path group
         or if this is a PDU with multiple destinations (Multicast), in that case
         transmit cancellation is not supported,
         Note: Order of OR condition on purpose for faster evaluation and preventing call
         of PduR_IsRoutingPathGroupEnabled with PDUR_ROUTINGPATHGROUP_GO_THROUGH_ID */
      /* Deviation MISRAC2012-1 */
      if((GroupId == PDUR_ROUTINGPATHGROUP_GO_THROUGH_ID) ||
         (PduR_IsRoutingPathGroupEnabled(GroupId) != FALSE))
#endif /* (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON) */
      {
         /* reference to set of target Tp-module API-functions */
         const PduR_RefToModuleAPI TargetAPIRef = pRTabUpTpTx[TxPduId].TargetModuleAPIRef;

         /* pointer to CancelTransmit function */
         const PduR_CancelTransmitFpType CancelTransmitFp =
            PduR_TpModuleAPI[TargetAPIRef].TpCancelTransmit;

         /* check if target function pointer is equal to null pointer */
         /* this might occur since multicast routing paths for CancelTransmit
            are not supported yet but specified */
         if(NULL_PTR == CancelTransmitFp)
         {
#if (PDUR_DEV_ERROR_DETECT == STD_ON)
            PDUR_DET_REPORT_ERROR(PDUR_SID_UP_CANCELTXREQ, PDUR_E_PDU_ID_INVALID);
#endif /* #if (PDUR_DEV_ERROR_DETECT == STD_ON) */
         /* RetVal is still E_NOT_OK */
         }
         else
         {
            /* get target PDU ID */
            const PduR_PduIdType TargetPduId = pRTabUpTpTx[TxPduId].TargetPduId;

            /* call CancelTransmit function */
            RetVal = CancelTransmitFp(TargetPduId);
         }
      }
   }

   DBG_PDUR_GENERICTPCANCELTRANSMIT_EXIT(RetVal, TxPduId, RTabUpTpConfigIdx);
   return RetVal;
}

/* AUTOSAR Memory Mapping - end section for code */
#if (TS_MERGED_COMPILE == STD_OFF)
#define PDUR_STOP_SEC_CODE
#include <PduR_MemMap.h>
#endif

#else /* if (PDUR_UPTP_CANCELTX_ENABLED == STD_ON) && .. */

/* Avoid empty translation unit according to ISO C90 */
typedef void PduR_PreventEmptyTranslationUnit_GenericTpCancelTransmitType;


/*==================[internal function definitions]=========================*/

#endif /* if (PDUR_UPTP_CANCELTX_ENABLED == STD_ON) && .. */
/*==================[end of file]===========================================*/
