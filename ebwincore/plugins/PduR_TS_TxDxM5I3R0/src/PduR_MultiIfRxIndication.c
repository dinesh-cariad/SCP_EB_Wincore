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

#if ((PDUR_ZERO_COST_OPERATION_IF == STD_OFF) && \
    (PDUR_MULTICAST_FROMIF_SUPPORT == STD_ON))
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

FUNC(void, PDUR_CODE) PduR_MultiIfRxIndication
(
   PduIdType MPduId,
   P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) PduInfoPtr
)
{
   /* pointer to routing table */
   CONSTP2CONST(PduR_MTabLoIfRxType, AUTOMATIC, PDUR_APPL_CONST) pMTabIfRx =
      PDUR_GET_CONFIG_ADDR(PduR_MTabLoIfRxType, PduR_GConfigPtr->PduR_MTabLoIfConfig.MTabRxRef);

   /* iterator for all entries providing the same MPduId */
   PduR_PduIdType MTabPos = (PduR_PduIdType)MPduId;

   /* provided local variable to ease readability of while condition */
   uint8 MoreEntries;

   DBG_PDUR_MULTIIFRXINDICATION_ENTRY(MPduId, PduInfoPtr);

   /* loop through all routing paths matching the current MPduId */
   do{

#if (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON)
      /* get destination PDU routing path group ID from routing table */
      const PduR_RoutingPathGroupIdType GroupId = pMTabIfRx[MTabPos].RPathGroupId;

      /* destination is either enabled or not referenced by a routing path group,
         Note: Order of OR condition on purpose for faster evaluation and preventing call
         of PduR_IsRoutingPathGroupEnabled with PDUR_ROUTINGPATHGROUP_GO_THROUGH_ID */
      /* Deviation MISRAC2012-1 */
      if((GroupId == PDUR_ROUTINGPATHGROUP_GO_THROUGH_ID) ||
         (PduR_IsRoutingPathGroupEnabled(GroupId) != FALSE))
#endif /* (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON) */
      {
         /* reference to set of target If-module API-functions */
         const PduR_RefToModuleAPI TargetAPIRef = pMTabIfRx[MTabPos].TargetModuleAPIRef;

         /* get pointer to RxIndication function from routing table */
         const PduR_IfRxIndFpType RxIndFp = PduR_IfModuleAPI[TargetAPIRef].IfRxIndication;

         /* get target PDU ID to RxIndication function from routing table */
         const PduR_PduIdType TargetPduId = pMTabIfRx[MTabPos].TargetPduId;

         /* call RxIndication function */
         RxIndFp(TargetPduId, PduInfoPtr);
      }

      /* initialize here to check while condition */
      MoreEntries = pMTabIfRx[MTabPos].MoreEntries;

      /* select next entry in routing table */
      MTabPos++;

   }while(MoreEntries != PDUR_LAST_MULTICAST_ENTRY_OF_PDUID);

   DBG_PDUR_MULTIIFRXINDICATION_EXIT(MPduId, PduInfoPtr);
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
