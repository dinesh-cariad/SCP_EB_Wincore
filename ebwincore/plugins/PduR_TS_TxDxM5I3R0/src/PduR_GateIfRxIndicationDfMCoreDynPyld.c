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

#if ((PDUR_MULTICORE_SUPPORT == STD_ON) &&\
     (PDUR_ZERO_COST_OPERATION_IF == STD_OFF) &&\
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

FUNC(void,PDUR_CODE) PduR_GateIfRxIndicationDfMCoreDynPyld
(
    PduIdType GPduId,
    P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) PduInfoPtr
)
{
   DBG_PDUR_GATEIFRXINDICATIONDFMCOREDYNPYLD_ENTRY(GPduId, PduInfoPtr);

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

      /* pointer to D-FIFO buffer information in RAM for dynamic payload */
      /* Deviation MISRAC2012-1 */
      CONSTP2VAR(PduR_BufDfInfoType, AUTOMATIC, PDUR_VAR_CLEARED) pBufDfInfo =
         PDUR_GET_RAM_ADDR(PduR_BufDfInfoType, PduR_GConfigPtr->PduR_BufDfDynPyldInfoRef);

      /* pointer to configuration of D-FIFO buffer for dynamic payload */
      CONSTP2CONST(PduR_BufDfDynPyldConfigType, AUTOMATIC, PDUR_APPL_CONST) pBufDfConfig =
         PDUR_GET_CONFIG_ADDR(PduR_BufDfDynPyldConfigType, PduR_GConfigPtr->PduR_BufDfDynPyldConfigRef);

      /* pointer to memory location of D-FIFO buffer in RAM */
      /* Deviation MISRAC2012-1 */
      CONSTP2VAR(uint8, AUTOMATIC, PDUR_VAR_CLEARED) pBufMem =
         PDUR_GET_RAM_ADDR(uint8, pBufDfConfig[BufferId].MemRef);

      /* pointer to UsedSduLength information in RAM for dynamic payload */
      /* Deviation MISRAC2012-1 */
      CONSTP2VAR(uint8, AUTOMATIC, PDUR_VAR_CLEARED) pUsedSduLength =
         PDUR_GET_RAM_ADDR(uint8, pBufDfConfig[BufferId].UsedSduLengthPtr);

      /* initialize flag showing if transmit operation shall be called */
      boolean Transmit = TRUE;

      /* length and depth of triggered FIFO buffer */
      const uint8 LenDfBuffer = pBufDfConfig[BufferId].Length;
      const uint8 Depth = pBufDfConfig[BufferId].Depth;

      /* Tail of FIFO buffer of PDU Router */
      uint8 TailPos;

#if (PDUR_DEV_ERROR_DETECT == STD_ON)
      boolean CallDet = FALSE;
#endif

      /* ENTER CRITICAL SECTION */
      SchM_Enter_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

      /* Get Tail of FIFO buffer of PDU Router */
      TailPos = (uint8) ((pBufDfInfo[BufferId].HeadPos + pBufDfInfo[BufferId].Count) % Depth);

      /* check if a PDU has been sent already */
      if (pBufDfInfo[BufferId].ConfP > 0U)
      {
         /* check if Tx buffer is full, received PDU can not be added to FIFO */
         if (pBufDfInfo[BufferId].Count >= Depth)
         {
#if (PDUR_DEV_ERROR_DETECT == STD_ON)
            /* Report flush of FIFO to Det */
            CallDet = TRUE;
#endif /* (PDUR_DEV_ERROR_DETECT == STD_ON) */

            pBufDfInfo[BufferId].Count = 0U;

            /* for each Transmit sent, a TxConfirmation is expected */
            ++(pBufDfInfo[BufferId].ConfP);
         }
         else
         {
            Transmit = FALSE;
         }
      }
      else
      {
         /* set ConfP in advance, reset when Transmit fails */
         ++(pBufDfInfo[BufferId].ConfP);
      }

      /* allocate length value */
      if(PduInfoPtr->SduLength > LenDfBuffer)
      {
         /* use buffer length */
         pUsedSduLength[TailPos] = LenDfBuffer;
      }
      else
      {
         /* use SDU length value from PduInfoPtr: the SduLength data type (fix PduLengthTypeEnum
          * uint16) is casted to uint8 since the maximum value that can be reached by SduLength
          * does not exceed the maximum value that can be represented by uint8 */
         pUsedSduLength[TailPos] = (uint8)PduInfoPtr->SduLength;
      }

      /* Copy data to Tail of D-FIFO buffer of PDU Router */
      TS_MemCpy(&pBufMem[TailPos * LenDfBuffer], PduInfoPtr->SduDataPtr, pUsedSduLength[TailPos] );

      /* increment number of elements stored in buffer */
      ++(pBufDfInfo[BufferId].Count);

      /* LEAVE CRITICAL SECTION */
      SchM_Exit_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();


      /* Transmit function shall be called */
      if (Transmit == TRUE)
      {
         /* return value from Transmit call */
         Std_ReturnType RetVal;

         /* reference to set of target If-module API-functions */
         const PduR_RefToModuleAPI TargetAPIRef = pGTabIfRx[GPduId].TargetModuleAPIRef;

         /* pointer to Transmit function */
         const PduR_TransmitFpType TransmitFp = PduR_IfModuleAPI[TargetAPIRef].IfTransmit;

         /* get target PDU ID to Transmit function from routing table */
         const PduR_PduIdType TargetPduId = pGTabIfRx[GPduId].TargetPduId;

         /* set up data of PduInfo for Transmit call */
         PduInfoType PduR_PduInfo;
         PduR_PduInfo.SduDataPtr = &pBufMem[TailPos * LenDfBuffer];

         /* check if received SDU length fits to configured Tx buffer size */
         if (PduInfoPtr->SduLength > LenDfBuffer)
         {
            /* set up data of PduInfo with restricted data length */
            PduR_PduInfo.SduLength = LenDfBuffer;
         }
         else
         {
            /* Call Transmit function with SduLength passed through */
            PduR_PduInfo.SduLength = PduInfoPtr->SduLength;
         }

         /* Call Transmit function with incoming buffered PDU */
         RetVal = TransmitFp(TargetPduId, &PduR_PduInfo);

         /* revert changes for rejected Transmit call */
         if(RetVal != E_OK)

         {
            /* ENTER CRITICAL SECTION */
            SchM_Enter_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

            /* revert changes for rejected Transmit call */
            --(pBufDfInfo[BufferId].ConfP);
            --(pBufDfInfo[BufferId].Count);

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

   DBG_PDUR_GATEIFRXINDICATIONDFMCOREDYNPYLD_EXIT(GPduId, PduInfoPtr);
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
