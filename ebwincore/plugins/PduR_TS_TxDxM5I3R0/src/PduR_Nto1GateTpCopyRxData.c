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
#include <TSAtomic_Assign.h>      /* Atomic assignments from Base PlugIn */
#include <TSMem.h>                /* TS_MemCpy */
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

FUNC(BufReq_ReturnType, PDUR_CODE) PduR_Nto1GateTpCopyRxData
(
  PduIdType Nto1TabTpId,
  P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) BufferSizePtr
)
{
  /* initialize return value */
  BufReq_ReturnType RetVal = BUFREQ_E_NOT_OK;

  /* pointer to Nto1 table */
  CONSTP2CONST(PduR_Nto1TabTpType, AUTOMATIC, PDUR_APPL_CONST) pNto1TabTp =
      PDUR_GET_CONFIG_ADDR(PduR_Nto1TabTpType, PduR_GConfigPtr->PduR_Nto1TabTpConfig.Nto1TabTpRef);

  DBG_PDUR_NTO1GATETPCOPYRXDATA_ENTRY(Nto1TabTpId, PduInfoPtr, BufferSizePtr);

  RetVal = PduR_GateTpCopyRxData(pNto1TabTp[Nto1TabTpId].GMPduId, PduInfoPtr, BufferSizePtr);

  DBG_PDUR_NTO1GATETPCOPYRXDATA_EXIT(RetVal, Nto1TabTpId, PduInfoPtr, BufferSizePtr);

  return RetVal;
}

/*==================[internal function definitions]=========================*/

/* AUTOSAR Memory Mapping - end section for code */
#if (TS_MERGED_COMPILE == STD_OFF)
#define PDUR_STOP_SEC_CODE
#include <PduR_MemMap.h>
#endif

#else /* if (PDUR_ZERO_COST_OPERATION_TP == STD_OFF) && .. */

#include <TSCompiler.h>           /* usage of macro in TSCompiler_Default.h */




/* Avoid empty translation unit according to ISO C90 */
TS_PREVENTEMPTYTRANSLATIONUNIT

#endif /* if (PDUR_ZERO_COST_OPERATION_TP == STD_OFF) && .. */
/*==================[end of file]===========================================*/
