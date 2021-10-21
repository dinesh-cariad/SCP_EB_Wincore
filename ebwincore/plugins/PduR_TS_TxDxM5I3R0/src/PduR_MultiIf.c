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
 */

/*==================[inclusions]============================================*/

#include <PduR_Trace.h>
#include <TSAutosar.h>            /* EB specific standard types */
#include <ComStack_Types.h>       /* AUTOSAR Communication Stack types */
#include <PduR_Internal_Static.h> /* Internal API (static part) - PduR_MultiIfTxGetByteIndex() */

#if((PDUR_ZERO_COST_OPERATION_IF == STD_OFF) && \
    (PDUR_MULTICAST_TOIF_SUPPORT == STD_ON) && \
    (PDUR_MULTICAST_TX_CONFIRMATION == STD_ON))

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
TS_MOD_PRIV_DEFN FUNC(PduR_PduIdType, PDUR_CODE) PduR_MultiIfTxGetByteIndex
(
   PduR_PduIdType MTpTxPduId,
   P2VAR(uint8, AUTOMATIC, PDUR_APPL_DATA) TransmitBitIdxPtr,
   P2VAR(uint8, AUTOMATIC, PDUR_APPL_DATA) TxConfBitIdxPtr
)
{
   const uint8 TransmitBitIdx = (2U * MTpTxPduId) % 8U;
   const PduR_PduIdType ByteIdx = (MTpTxPduId / 4U); /* 2MTabPos/8 */

   DBG_PDUR_MULTIIFTXGETBYTEINDEX_ENTRY(MTpTxPduId, TransmitBitIdxPtr, TxConfBitIdxPtr);

   *TransmitBitIdxPtr = TransmitBitIdx;
   *TxConfBitIdxPtr = TransmitBitIdx + 1U;

   DBG_PDUR_MULTIIFTXGETBYTEINDEX_EXIT(ByteIdx, MTpTxPduId, TransmitBitIdxPtr, TxConfBitIdxPtr);

   return ByteIdx;
}

/* AUTOSAR Memory Mapping - end section for code */
#if (TS_MERGED_COMPILE == STD_OFF)
#define PDUR_STOP_SEC_CODE
#include <PduR_MemMap.h>
#endif

/*==================[internal function definitions]=========================*/
#else /* if (PDUR_ZERO_COST_OPERATION_IF == STD_OFF) && .. */

#include <TSCompiler.h>           /* usage of macro in TSCompiler_Default.h */

/* Avoid empty translation unit according to ISO C90 */
TS_PREVENTEMPTYTRANSLATIONUNIT


/*==================[internal function definitions]=========================*/

#endif /* if (PDUR_ZERO_COST_OPERATION_IF == STD_OFF) && .. */
/*==================[end of file]===========================================*/
