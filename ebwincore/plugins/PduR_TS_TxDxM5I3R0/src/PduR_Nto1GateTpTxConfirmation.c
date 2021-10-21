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

#if ((PDUR_ZERO_COST_OPERATION_TP == STD_OFF) && \
     (PDUR_TPGATEWAY_SUPPORT == STD_ON) && \
     (PDUR_TPGWQUEUE_AND_NTO1ROUTING_ENABLED == STD_ON))
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

FUNC(void, PDUR_CODE) PduR_Nto1GateTpTxConfirmation
(
   PduIdType Nto1TabTpId,
   NotifResultType Result
)
{
  /* pointer to Nto1 table */
  CONSTP2CONST(PduR_Nto1TabTpType, AUTOMATIC, PDUR_APPL_CONST) pNto1TabTp =
     PDUR_GET_CONFIG_ADDR(PduR_Nto1TabTpType, PduR_GConfigPtr->PduR_Nto1TabTpConfig.Nto1TabTpRef);

  /* pointer to memory location of active GMPduId */
  /* Deviation MISRAC2012-1 */
  CONSTP2VAR(PduR_PduIdType, AUTOMATIC, PDUR_VAR_CLEARED) pGMPduIdMem =
     PDUR_GET_RAM_ADDR(PduR_PduIdType, pNto1TabTp[Nto1TabTpId].GMPduIdMemRef);

  DBG_PDUR_NTO1GATETPTXCONFIRMATION_ENTRY(Nto1TabTpId, Result);

  if (*pGMPduIdMem != PDUR_INVALID_PDUID)
  {
    PduR_GateTpSessionIndexType SessionIndex;

    PduR_GateTpTxConfirmation(*pGMPduIdMem, Result);

    /* ENTER CRITICAL SECTION */
    SchM_Enter_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

    SessionIndex = PduR_GetGateTpSessionInUse(*pGMPduIdMem, PDUR_GATETP_QUEUE_ELEMENT_TAIL);

    /* Clear saved GMPduId after last session of queue was released... */
    if (PDUR_NO_GATETP_SESSION_ASSIGNED == SessionIndex)
    {
      /* ... and no reception is in progress */
      SessionIndex = PduR_GetGateTpSessionInUse(*pGMPduIdMem, PDUR_GATETP_QUEUE_ELEMENT_HEAD);
      if(PDUR_NO_GATETP_SESSION_ASSIGNED == SessionIndex)
      {
        *pGMPduIdMem = PDUR_INVALID_PDUID;
      }
    }

    /* EXIT CRITICAL SECTION */
    SchM_Exit_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();
  }

  DBG_PDUR_NTO1GATETPTXCONFIRMATION_EXIT(Nto1TabTpId, Result);
}

/* AUTOSAR Memory Mapping - end section for code */
#if (TS_MERGED_COMPILE == STD_OFF)
#define PDUR_STOP_SEC_CODE
#include <PduR_MemMap.h>
#endif

#else /* if (PDUR_ZERO_COST_OPERATION_TP == STD_OFF) && .. */

/* Avoid empty translation unit according to ISO C90 */
typedef void PduR_PreventEmptyTranslationUnit_Nto1GateTpTxConfirmationType;

/*==================[internal function definitions]=========================*/

#endif /* if (PDUR_ZERO_COST_OPERATION_TP == STD_OFF) && .. */
/*==================[end of file]===========================================*/
