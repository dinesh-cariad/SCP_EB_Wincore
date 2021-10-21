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
#include <PduR_Api_Depend.h>      /* Public API (dependent part) */

#if ((PDUR_ZERO_COST_OPERATION_FULL == STD_OFF) && \
     (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON))
#include <SchM_PduR.h>            /* Schedule Manager for module PduR */

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

FUNC(void, PDUR_CODE) PduR_EnableRouting
(
   PduR_RoutingPathGroupIdType GroupId
)
{
  DBG_PDUR_ENABLEROUTING_ENTRY(GroupId);

  /* check if PDU Router is initialized */
  if(PduR_State == PDUR_UNINIT)
  {
#if (PDUR_DEV_ERROR_DETECT == STD_ON)
    /* reported to Det in development mode */
    PDUR_DET_REPORT_ERROR(PDUR_SID_ENABLE_ROUTING, PDUR_E_INVALID_REQUEST);
#endif /* #if (PDUR_DEV_ERROR_DETECT == STD_ON) */
  }

  /* range check of parameter GroupId, if this GroupId doesn't exist
    function returns with no action carried out */
  else if (GroupId >= PduR_GConfigPtr->PduR_RoutingPathGroupsConfig.RPathGroupsCount)
  {
#if (PDUR_DEV_ERROR_DETECT == STD_ON)
    PDUR_DET_REPORT_ERROR(PDUR_SID_ENABLE_ROUTING, PDUR_E_ROUTING_PATH_GROUP_ID_INVALID);
#endif /* #if (PDUR_DEV_ERROR_DETECT == STD_ON) */
  }
  else
  {
    /* pointer to single buffer information in RAM for routing path groups bitmap */
    /* Deviation MISRAC2012-1 */
    CONSTP2VAR(uint8, AUTOMATIC, PDUR_VAR_CLEARED) RoutingPathsGroupBitMapPtr =
       PDUR_GET_RAM_ADDR(uint8, PduR_GConfigPtr->PduR_RoutingPathGroupsBitmapRef);

    /* get this routing path group's byte and bit position on the bitmap */
    const PduR_RoutingPathGroupIdType BytePosition = GroupId/8U;
    const uint8 BitPosition = (uint8)(GroupId%8U);

    /* ENTER CRITICAL SECTION */
    SchM_Enter_PduR_SCHM_PDUR_EXCLUSIVE_AREA_3();

    TS_AtomicSetBit_8(&RoutingPathsGroupBitMapPtr[BytePosition], BitPosition);

    /* EXIT CRITICAL SECTION */
    SchM_Exit_PduR_SCHM_PDUR_EXCLUSIVE_AREA_3();
  }
  /* else, this routing path group GroupId does not exist, PDU Router module returns with no action */

  DBG_PDUR_ENABLEROUTING_EXIT(GroupId);
}

/* AUTOSAR Memory Mapping - end section for code */
#if (TS_MERGED_COMPILE == STD_OFF)
#define PDUR_STOP_SEC_CODE
#include <PduR_MemMap.h>
#endif

#else /* if ((PDUR_ZERO_COST_OPERATION_FULL == STD_OFF) &&
             (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON)) */

#include <TSCompiler.h>           /* usage of macro in TSCompiler_Default.h */

/* Avoid empty translation unit according to ISO C90 */
TS_PREVENTEMPTYTRANSLATIONUNIT


/*==================[internal function definitions]=========================*/

#endif /* if (PDUR_ZERO_COST_OPERATION_FULL == STD_OFF) && */
/*==================[end of file]===========================================*/
