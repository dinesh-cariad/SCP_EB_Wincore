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
#include <TSAtomic_Assign.h>      /* Atomic assignments from Base PlugIn */
#include <ComStack_Types.h>       /* AUTOSAR Communication Stack types */
#include <PduR_Internal_Static.h> /* Internal API (static part) */
#include <PduR_Api_Depend.h>      /* Public API (depend part) */

#if ((PDUR_ZERO_COST_OPERATION_TP == STD_OFF) && \
     (PDUR_TPGATEWAY_SUPPORT == STD_ON) && \
     (PDUR_TPGWQUEUE_AND_NTO1ROUTING_ENABLED == STD_ON))
#include <PduR_Lcfg.h>            /* Link-time configurable data */
/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/* AUTOSAR Memory Mapping - start section for code */
#if (TS_MERGED_COMPILE == STD_OFF)
#define PDUR_START_SEC_CODE
#include <PduR_MemMap.h>
#endif

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

FUNC(BufReq_ReturnType, PDUR_CODE) PduR_Nto1GateTpStartOfReception
(
  PduIdType Nto1TabTpId,
  PduLengthType TpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) BufferSizePtr
)
{
  /* initialize return value */
  BufReq_ReturnType RetVal = BUFREQ_E_OVFL;

  /* pointer to Nto1 table */
  CONSTP2CONST(PduR_Nto1TabTpType, AUTOMATIC, PDUR_APPL_CONST) pNto1TabTp =
      PDUR_GET_CONFIG_ADDR(PduR_Nto1TabTpType, PduR_GConfigPtr->PduR_Nto1TabTpConfig.Nto1TabTpRef);

  const PduR_PduIdType GMPduId = pNto1TabTp[Nto1TabTpId].GMPduId;

  DBG_PDUR_NTO1GATETPSTARTOFRECEPTION_ENTRY(Nto1TabTpId, TpSduLength, BufferSizePtr);

  RetVal = PduR_GateTpStartOfReception(GMPduId, TpSduLength, BufferSizePtr);

  /* A session was assigned */
  if (RetVal == BUFREQ_OK)
  {
    /* pointer to memory location of active GMPduId */
    /* Deviation MISRAC2012-1 */
    CONSTP2VAR(PduR_PduIdType, AUTOMATIC, PDUR_VAR_CLEARED) pGMPduIdMem =
        PDUR_GET_RAM_ADDR(PduR_PduIdType, pNto1TabTp[Nto1TabTpId].GMPduIdMemRef);

    *pGMPduIdMem = GMPduId;
  }

  DBG_PDUR_NTO1GATETPSTARTOFRECEPTION_EXIT(RetVal, Nto1TabTpId, TpSduLength, BufferSizePtr);

  return RetVal;
}

/* AUTOSAR Memory Mapping - end section for code */
#if (TS_MERGED_COMPILE == STD_OFF)
#define PDUR_STOP_SEC_CODE
#include <PduR_MemMap.h>
#endif

#else /* if (PDUR_ZERO_COST_OPERATION_TP == STD_OFF) && .. */

/* Avoid empty translation unit according to ISO C90 */
typedef void PduR_PreventEmptyTranslationUnit_Nto1GateTpStartOfReceptionType;

#endif /* if (PDUR_ZERO_COST_OPERATION_TP == STD_OFF) && .. */
/*==================[end of file]===========================================*/
