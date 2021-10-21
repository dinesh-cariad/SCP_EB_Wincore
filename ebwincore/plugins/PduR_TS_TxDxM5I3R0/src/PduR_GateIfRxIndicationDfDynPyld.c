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
#include <TSMem.h>                /* TS_MemCpy */

#if ((PDUR_ZERO_COST_OPERATION_IF == STD_OFF) &&\
     (PDUR_IFGATEWAY_SUPPORT == STD_ON) &&\
     (PDUR_FIFO_TX_BUFFER_SUPPORT == STD_ON))
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

FUNC(void, PDUR_CODE) PduR_GateIfRxIndicationDfDynPyld
(
    PduIdType GPduId,
    P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) PduInfoPtr
)
{
   DBG_PDUR_GATEIFRXINDICATIONDFDYNPYLD_ENTRY(GPduId, PduInfoPtr);

#if (PDUR_DEV_ERROR_DETECT == STD_ON)
   /* check if input parameter is NULL */
   if (NULL_PTR == PduInfoPtr->SduDataPtr)
   {
      PDUR_DET_REPORT_ERROR(PDUR_SID_IFGW_RXIND_DF, PDUR_E_NULL_POINTER);
   }
   else
#endif /* #if (PDUR_DEV_ERROR_DETECT == STD_ON) */
   {
      /* pointer to routing table */
      CONSTP2CONST(PduR_GTabIfRxType, AUTOMATIC, PDUR_APPL_CONST) pGTabIfRx =
         PDUR_GET_CONFIG_ADDR(PduR_GTabIfRxType, PduR_GConfigPtr->PduR_GTabIfConfig.GTabRxRef);

      /* local representation of the BufferId */
      const uint16 BufferId = pGTabIfRx[GPduId].BufferId;

      /* pointer to D-FIFO buffer information in RAM */
      /* Deviation MISRAC2012-1 */
      CONSTP2VAR(PduR_BufDfInfoType, AUTOMATIC, PDUR_VAR_CLEARED) pBufDfInfo =
         PDUR_GET_RAM_ADDR(PduR_BufDfInfoType, PduR_GConfigPtr->PduR_BufDfDynPyldInfoRef);

      /* initialize flag showing if transmit operation shall be called */
      boolean Transmit = TRUE;

#if (PDUR_DEV_ERROR_DETECT == STD_ON)
      boolean CallDet = FALSE;
#endif

      /* ENTER CRITICAL SECTION */
      SchM_Enter_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

      /* check if a PDU has been sent already */
      if (pBufDfInfo[BufferId].ConfP > 0U)
      {
         /* pointer to configuration of D-FIFO buffer for static payload */
         CONSTP2CONST(PduR_BufDfDynPyldConfigType, AUTOMATIC, PDUR_APPL_CONST) pBufDfConfig =
            PDUR_GET_CONFIG_ADDR(PduR_BufDfDynPyldConfigType,
            PduR_GConfigPtr->PduR_BufDfDynPyldConfigRef);

         /* length depth of FIFO buffer */
         const uint8 Depth = pBufDfConfig[BufferId].Depth;

         /* check if Tx buffer is full, received PDU can not be added to FIFO */
         if (pBufDfInfo[BufferId].Count >= Depth)
         {
#if (PDUR_DEV_ERROR_DETECT == STD_ON)
            /* Report flush of FIFO to Det */
            CallDet = TRUE;
#endif /* (PDUR_DEV_ERROR_DETECT == STD_ON) */

            /* for each Transmit sent, a TxConfirmation is expected */
            ++(pBufDfInfo[BufferId].ConfP);

            /* reset number of elements stored in FIFO influenced by possible preemption */
            pBufDfInfo[BufferId].Count = pBufDfInfo[BufferId].InTransmission;
         }
         else
         {
            /* Get Tail of FIFO buffer of PDU Router */
            const uint8 TailPos =
               (uint8) ((pBufDfInfo[BufferId].HeadPos + pBufDfInfo[BufferId].Count) % Depth);

            /* length of direct FIFO buffer */
            const uint8 LenDfBuffer = pBufDfConfig[BufferId].Length;

            /* pointer to memory location of D-FIFO buffer in RAM */
            /* Deviation MISRAC2012-1 */
            CONSTP2VAR(uint8, AUTOMATIC, PDUR_VAR_CLEARED) pBufMem =
               PDUR_GET_RAM_ADDR(uint8, pBufDfConfig[BufferId].MemRef);

            /* pointer to UsedSduLength information in RAM for dynamic payload */
            /* Deviation MISRAC2012-1 */
            CONSTP2VAR(uint8, AUTOMATIC, PDUR_VAR_CLEARED) pUsedSduLength =
               PDUR_GET_RAM_ADDR(uint8, pBufDfConfig[BufferId].UsedSduLengthPtr);

            Transmit = FALSE;

            /* allocate length value */
            if(PduInfoPtr->SduLength > LenDfBuffer)
            {
               /* use buffer length */
               pUsedSduLength[TailPos] = LenDfBuffer;
            }
            else
            {
               /* use SDU length value from PduInfoPtr: the SduLength data type (fix
                * PduLengthTypeEnum uint16) is casted to uint8 since the maximum value that
                * can be reached by SduLength does not exceed the maximum value that can be
                * represented by uint8 */
               pUsedSduLength[TailPos] = (uint8)PduInfoPtr->SduLength;
            }

            /* Copy data to Tail of D-FIFO buffer of PDU Router */
            /* overcome MISRA 10.1 in providing (TailPos * LenDfBuffer) directly to the array */
            TS_MemCpy(&pBufMem[TailPos * LenDfBuffer], PduInfoPtr->SduDataPtr,
               pUsedSduLength[TailPos]);

            /* increment number of elements stored in buffer */
            ++(pBufDfInfo[BufferId].Count);
         }
      }
      else
      {
         /* set ConfP in advance, reset when Transmit fails */
         ++(pBufDfInfo[BufferId].ConfP);
      }

      /* LEAVE CRITICAL SECTION */
      SchM_Exit_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

      /* Transmit function shall be called */
      if (Transmit == TRUE)
      {
         /* reference to set of target If-module API-functions */
         const PduR_RefToModuleAPI TargetAPIRef = pGTabIfRx[GPduId].TargetModuleAPIRef;

         /* pointer to Transmit function */
         const PduR_TransmitFpType TransmitFp = PduR_IfModuleAPI[TargetAPIRef].IfTransmit;

         /* get target PDU ID to Transmit function from routing table */
         const PduR_PduIdType TargetPduId = pGTabIfRx[GPduId].TargetPduId;

         /* Call Transmit function */
         if (TransmitFp(TargetPduId, PduInfoPtr) != E_OK)
         {
            /* ENTER CRITICAL SECTION */
            SchM_Enter_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

            /* revert changes for rejected Transmit call */
            --(pBufDfInfo[BufferId].ConfP);

            /* LEAVE CRITICAL SECTION */
            SchM_Exit_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();
         }
      }

#if (PDUR_DEV_ERROR_DETECT == STD_ON)
      /* Tx buffer is full */
      if (CallDet == TRUE)
      {
         /* Report flush of FIFO to Det */
         PDUR_DET_REPORT_ERROR(PDUR_SID_IFGW_RXIND_DF, PDUR_E_PDU_INSTANCES_LOST);
      }
#endif /* (PDUR_DEV_ERROR_DETECT == STD_ON) */
   }

   DBG_PDUR_GATEIFRXINDICATIONDFDYNPYLD_EXIT(GPduId, PduInfoPtr);
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
