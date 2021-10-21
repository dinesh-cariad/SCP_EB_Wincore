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

FUNC(void,PDUR_CODE) PduR_GateIfRxIndicationTfDynPyld
(
    PduIdType GPduId,
    P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) PduInfoPtr
)
{
   DBG_PDUR_GATEIFRXINDICATIONTFDYNPYLD_ENTRY(GPduId, PduInfoPtr);

#if (PDUR_DEV_ERROR_DETECT == STD_ON)
   /* check if input parameter is NULL */
   if (NULL_PTR == PduInfoPtr->SduDataPtr)
   {
      PDUR_DET_REPORT_ERROR(PDUR_SID_IFGW_RXIND_TF, PDUR_E_NULL_POINTER);
   }
   else
#endif /* #if (PDUR_DEV_ERROR_DETECT == STD_ON) */
   {
      /* pointer to routing table */
      CONSTP2CONST(PduR_GTabIfRxType, AUTOMATIC, PDUR_APPL_CONST) pGTabIfRx =
         PDUR_GET_CONFIG_ADDR(PduR_GTabIfRxType, PduR_GConfigPtr->PduR_GTabIfConfig.GTabRxRef);

      /* local representation of the BufferId */
      const uint16 BufferId = pGTabIfRx[GPduId].BufferId;

      /* pointer to T-FIFO buffer information in RAM for dynamic payload */
      /* Deviation MISRAC2012-1 */
      CONSTP2VAR(PduR_BufTfInfoType, AUTOMATIC, PDUR_VAR_CLEARED) pBufTfInfo =
         PDUR_GET_RAM_ADDR(PduR_BufTfInfoType, PduR_GConfigPtr->PduR_BufTfDynPyldInfoRef);

      /* pointer to configuration of T-FIFO buffer for dynamic payload */
      CONSTP2CONST(PduR_BufTfDynPyldConfigType, AUTOMATIC, PDUR_APPL_CONST) pBufTfConfig =
         PDUR_GET_CONFIG_ADDR(PduR_BufTfDynPyldConfigType, PduR_GConfigPtr->PduR_BufTfDynPyldConfigRef);

      /* pointer to memory location of T-FIFO buffer in RAM */
      /* Deviation MISRAC2012-1 */
      CONSTP2VAR(uint8, AUTOMATIC, PDUR_VAR_CLEARED) pBufMem =
         PDUR_GET_RAM_ADDR(uint8, pBufTfConfig[BufferId].MemRef);

      /* pointer to UsedSduLength information in RAM for dynamic payload */
      /* Deviation MISRAC2012-1 */
      CONSTP2VAR(uint8, AUTOMATIC, PDUR_VAR_CLEARED) pUsedSduLength =
         PDUR_GET_RAM_ADDR(uint8, pBufTfConfig[BufferId].UsedSduLengthPtr);

      /* initialize flag showing if transmit operation shall be called */
      boolean Transmit = TRUE;

      /* length and depth of triggered FIFO buffer */
      const uint8 LenTfBuffer = pBufTfConfig[BufferId].Length;
      const uint8 Depth = pBufTfConfig[BufferId].Depth;

      /* Tail of FIFO buffer of PDU Router */
      uint8 TailPos;

#if (PDUR_DEV_ERROR_DETECT == STD_ON)
      boolean CallDet = FALSE;
#endif

      /* ENTER CRITICAL SECTION */
      SchM_Enter_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

      /* Get Tail of FIFO buffer of PDU Router */
      TailPos = (uint8) ((pBufTfInfo[BufferId].HeadPos + pBufTfInfo[BufferId].Count) % Depth);

      /* check if a PDU has been sent already */
      if (pBufTfInfo[BufferId].ConfP > 0U)
      {
         /* check if buffer is full */
         if (pBufTfInfo[BufferId].Count >= Depth)
         {
#if (PDUR_DEV_ERROR_DETECT == STD_ON)
            /* Report flush of FIFO to Det */
            CallDet = TRUE;
#endif /* (PDUR_DEV_ERROR_DETECT == STD_ON) */

            pBufTfInfo[BufferId].Count = 0U;
            ++(pBufTfInfo[BufferId].ConfP);
         }
         else
         {
            Transmit = FALSE;
         }
      }
      else
      {
         ++(pBufTfInfo[BufferId].ConfP);
      }

      /* allocate length value */
      if(PduInfoPtr->SduLength > LenTfBuffer)
      {
         /* use buffer length */
         pUsedSduLength[TailPos] = LenTfBuffer;
      }
      else
      {
         /* use SDU length value from PduInfoPtr: the SduLength data type (fix PduLengthTypeEnum
          * uint16) is casted to uint8 since the maximum value that can be reached by SduLength
          * does not exceed the maximum value that can be represented by uint8 */
         pUsedSduLength[TailPos] = (uint8)PduInfoPtr->SduLength;
      }

      /* Copy data to Tail of T-FIFO buffer of PDU Router */
      TS_MemCpy(&pBufMem[TailPos * LenTfBuffer], PduInfoPtr->SduDataPtr, pUsedSduLength[TailPos] );

      /* increment number of elements stored in buffer */
      ++(pBufTfInfo[BufferId].Count);

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

         /* check if received SDU length fits to configured Tx buffer size */
         if (PduInfoPtr->SduLength > LenTfBuffer)
         {
            /* set up data of PduInfo for Transmit call */
            PduInfoType PduR_PduInfo;
            PduR_PduInfo.SduDataPtr = PduInfoPtr->SduDataPtr;
            PduR_PduInfo.SduLength = LenTfBuffer;

            /* Call Transmit function with restricted data length */
            RetVal = TransmitFp(TargetPduId, &PduR_PduInfo);
         }
         else
         {
            /* Call Transmit function with PduInfoPtr passed through */
            RetVal = TransmitFp(TargetPduId, PduInfoPtr);
         }

         /* revert changes for rejected Transmit call */
         if(RetVal != E_OK)

         {
            /* ENTER CRITICAL SECTION */
            SchM_Enter_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

            /* revert changes for rejected Transmit call */
            --(pBufTfInfo[BufferId].ConfP);
            --(pBufTfInfo[BufferId].Count);

            /* LEAVE CRITICAL SECTION */
            SchM_Exit_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();
         }
      }

#if (PDUR_DEV_ERROR_DETECT == STD_ON)
      /* Tx buffer is full */
      if (CallDet == TRUE)
      {
         /* Report flush of FIFO to Det */
         PDUR_DET_REPORT_ERROR(PDUR_SID_IFGW_RXIND_TF, PDUR_E_PDU_INSTANCES_LOST);
      }
#endif /* (PDUR_DEV_ERROR_DETECT == STD_ON) */
   }

   DBG_PDUR_GATEIFRXINDICATIONTFDYNPYLD_EXIT(GPduId, PduInfoPtr);
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
