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
 *  MISRAC2012-1) Deviated Rule: 5.8 (required)
 *  Identifiers that define objects or functions with external linkage shall be unique.
 *
 *  Reason:
 *  This violation is a false positive.
 *  The tool does not recognize the following situation:
 *  The function identifier depends if the merge compile use
 *  case is enabled or not. The behavior is realized through a
 *  instrumented macro which enables the use case dependent
 *  function identifier.
 *
 *  MISRAC2012-2) Deviated Rule: 13.5 (required)
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
#include <PduR_Api_Depend.h>      /* Public API (dependent part) */

#if ((PDUR_ZERO_COST_OPERATION_FULL == STD_OFF) && \
     (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON))
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

/* Deviation MISRAC2012-1 */
TS_MOD_PRIV_DEFN FUNC(boolean, PDUR_CODE) PduR_IsRoutingPathGroupEnabled
(
   PduR_RoutingPathGroupIdType RoutPathGroupId
)
{ /* pointer to single buffer information in RAM for routing path groups bitmap */
  /* Deviation MISRAC2012-1 */
  P2VAR(uint8, AUTOMATIC, PDUR_VAR_CLEARED) RoutingPathsGroupBitMapPtr =
      PDUR_GET_RAM_ADDR(uint8, PduR_GConfigPtr->PduR_RoutingPathGroupsBitmapRef);

  boolean retStatus;

  uint8 resultBit;

  TS_GetBit(&RoutingPathsGroupBitMapPtr[RoutPathGroupId/8U],(RoutPathGroupId % 8U),uint8,resultBit);

  if(  resultBit > 0U  )
  {
    retStatus = TRUE;
  }
  else
  {
    retStatus = FALSE;
  }

  return retStatus;
}


#if ((PDUR_ZERO_COST_OPERATION_TP == STD_OFF) && (PDUR_TPGATEWAY_SUPPORT == STD_ON))
/* Deviation MISRAC2012-1 */
TS_MOD_PRIV_DEFN FUNC(uint8, PDUR_CODE) PduR_RPathGroupNumTpGwDestEnabled
(
   PduR_PduIdType GMPduId
)
{
   /* no destination of the TP (multicast) gateway routing path can be reached */
   uint8 NumDestEnabled = 0U;

   /* initialize iterator for all entries providing the same GMPduId */
   PduR_PduIdType GTabPos = GMPduId;

   /* provided local variable to ease readability of while condition */
   uint8 MoreEntries;

   /* pointer to routing table */
   CONSTP2CONST(PduR_GTabTpRxType, AUTOMATIC, PDUR_APPL_CONST) pGTabTpRx =
      PDUR_GET_CONFIG_ADDR(PduR_GTabTpRxType, PduR_GConfigPtr->PduR_GTabTpConfig.GTabRxRef);

   /* loop through all routing table entries matching the current GMPduId */
   do{
      /* get target PDU routing path group ID from routing table */
      const PduR_RoutingPathGroupIdType GroupId = pGTabTpRx[GTabPos].RPathGroupId;

       /* initialize to check while condition */
      MoreEntries = pGTabTpRx[GTabPos].MoreEntries;

      /* destination is either enabled or not referenced by a routing path group,
         Note: Order of OR condition on purpose for faster evaluation and preventing call
         of PduR_IsRoutingPathGroupEnabled with PDUR_ROUTINGPATHGROUP_GO_THROUGH_ID */
      /* Deviation MISRAC2012-2 */
      if((GroupId == PDUR_ROUTINGPATHGROUP_GO_THROUGH_ID) ||
         (PduR_IsRoutingPathGroupEnabled(GroupId) != FALSE))
      {
        /* count active (reachable) destinations */
        NumDestEnabled++;
      }

      GTabPos++;

   }while(MoreEntries != PDUR_LAST_MULTICAST_ENTRY_OF_PDUID);

   return NumDestEnabled;
}
#endif /* (PDUR_ZERO_COST_OPERATION_TP == STD_OFF) && (PDUR_TPGATEWAY_SUPPORT == STD_ON) */


/* AUTOSAR Memory Mapping - end section for code */
#if (TS_MERGED_COMPILE == STD_OFF)
#define PDUR_STOP_SEC_CODE
#include <PduR_MemMap.h>
#endif

#else /* if ((PDUR_ZERO_COST_OPERATION_FULL == STD_OFF) &&
             (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON)) */

/* Avoid empty translation unit according to ISO C90 */
typedef void PduR_PreventEmptyTranslationUnit_RPathGroupsType;


/*==================[internal function definitions]=========================*/

#endif /* if (PDUR_ZERO_COST_OPERATION_FULL == STD_OFF) */
/*==================[end of file]===========================================*/
