/**
 * \file
 *
 * \brief AUTOSAR Com
 *
 * This file contains the implementation of the AUTOSAR
 * module Com.
 *
 * \version 6.3.50
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */


/* MISRA-C:2012 Deviation List
 *
 *
 * MISRAC2012-1) Deviated Rule: 11.5 (advisory)
 * A conversion should not be performed from pointer to
 * void into pointer to object.
 *
 * Reason:
 * The memory routines are optimized for dealing with aligned
 * memory sections.
 *
 */


/*==================[inclusions]=============================================*/

#include <Com_Trace.h>
#include <TSAutosar.h>          /* EB specific standard types */
#include <ComStack_Types.h>     /* AUTOSAR standard types */
#include <Com_Api_Static.h>     /* module public static API */
#include <Com_Priv.h>           /* module internal stuff */
#include <Com_Cbk_Static.h>     /* Callback declaration header */
#include <TSMem.h>              /* TS_MemCpy */

#include <Com_Core_TxIPdu.h>    /* Header TxIPdu functions */

#if (COM_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>                /* Det API */
#endif

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/



#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_START_SEC_CODE
#include <Com_MemMap.h>
#endif

/**
 * \brief Com_TriggerTransmit - copy data to PDU-router memory
 *
 * a function to be used to fetch data from the COM immediately
 * Preconditions:
 * - COM should be initialized
 *
 * \param[in] ComTxPduId - ID of the PDU which's data shall be copied
 * \param[in] PduInfoPtr - Contains a pointer to a buffer (SduDataPtr) to where the SDU
 * shall be copied to. On return, the service will indicate the length of
 * the copied SDU data in SduLength.
 * \return Function execution success status
 * \retval E_OK: SDU has been copied and SduLength indicates the
 * number of copied bytes.
 * \retval E_NOT_OK: No SDU has been copied. PduInfoPtr must not be
 * used since it may contain a NULL pointer or point to invalid data.
 *
 * \ServiceID{0x41}
 * \Reentrancy{Re-entrant for different values of ComTxPduId only.}
 * \Synchronicity{Synchronous}
 */
FUNC(Std_ReturnType, COM_CODE) Com_TriggerTransmit
(
   PduIdType ComTxPduId,
   P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr
)
{
   Std_ReturnType retval = E_NOT_OK;

   /* pointer to the configuration of the IPdu to which the signal belongs */
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr;
   PduInfoType PduData; /* the data of the PDU */

   DBG_COM_TRIGGERTRANSMIT_ENTRY(ComTxPduId,PduInfoPtr);


#if (COM_DEV_ERROR_DETECT == STD_ON)

   /* check SignalDataPtr */
   if (PduInfoPtr == NULL_PTR)
   {
      COM_DET_REPORTERROR(COM_E_PARAM_POINTER, COMServiceId_TriggerTransmit);
   }
   else
   {
      /* set return value in case function fails */
      PduInfoPtr->SduLength = 0U;

      /* check whether Com is initialized */
      if (Com_InitStatus == COM_EB_UNINIT)
      {
         COM_DET_REPORTERROR(COM_E_UNINIT, COMServiceId_TriggerTransmit);
      }
      /* check if SduPtr is NULL_PTR */
      else if (PduInfoPtr->SduDataPtr == NULL_PTR)
      {
         COM_DET_REPORTERROR(COM_E_PARAM_POINTER, COMServiceId_TriggerTransmit);
      }
      /* check if ComTxPduId is valid */
      else if (Com_gConfigPtr->ComTxIPduArraySize <= ComTxPduId)
      {
         COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_TriggerTransmit);
      }
      else

#endif /* COM_DEV_ERROR_DETECT */

      {
        /* get IPdu to which ComTxPduId identifies and check if the IPdu is valid */
        IPduPtr = COM_GET_CONFIG_ELEMENT(ComTxIPduType, Com_gConfigPtr->ComTxIPduRef, ComTxPduId);



#if ( (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) && (COM_DEV_ERROR_DETECT == STD_ON) )
        if( TRUE == COM_GET_COM_TX_IPDU_DUMMY(IPduPtr) )
        {
          COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_TriggerTransmit);
        }
        else
#endif /* ( (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) && (COM_DEV_ERROR_DETECT == STD_ON) ) */
        {

          /* Prepare IPdu */
          PduData.SduDataPtr = &Com_gDataMemPtr[(IPduPtr->ComIPduValueRef)];
          /* Deviation MISRAC2012-1 */
          COM_AtomicAssign_PduLengthType(PduData.SduLength, COM_GET_TX_IPDU_SIZE(ComTxPduId, IPduPtr));

#if (COM_CALL_OUT_FUNC_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE)
#if (COM_TRIGGERTX_CALLOUT_ENABLE == STD_ON)
          /* Get the function pointer of the callout function */
          if (IPduPtr->ComIPduTrigTxCallOutFuncPtrRef != COM_CALL_OUT_FUNC_PTR_NULL)
          {
            CONSTP2VAR(Com_TxCalloutType, AUTOMATIC, COM_APPL_CODE) fptr =
                Com_TxCallouts[(IPduPtr->ComIPduTrigTxCallOutFuncPtrRef)];
            /* call callout, ignore return value (see COM395) */
            (void)fptr(ComTxPduId, &PduData);
          }

#endif /* (COM_TRIGGERTX_CALLOUT_ENABLE == STD_ON) */
#endif /* (COM_CALL_OUT_FUNC_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) */

          if ( Com_IsBitSet_8(&Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + ComTxPduId)],
              COM_TXPDU_STARTED) )
          {
            /* Enter critical section;
             * the section is critical as an interrupting SendSignal call may
             * cause data garbage to be copied */
            Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

            /* set size of copied bytes */
            PduInfoPtr->SduLength = PduData.SduLength;

            /* copy PDU content */
            TS_MemCpy(PduInfoPtr->SduDataPtr, PduData.SduDataPtr, PduData.SduLength);

            /* Leave critical section */
            Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

#if (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL)
#if (COM_CL_UB_TX_TRIGGERTRANSMIT_ENABLE == STD_ON)

#if (!((COM_UPDATE_BIT_TX_CONFIG == COM_UPDATE_BIT_PRESENT_FOR_ALL) &&  \
    (COM_CL_UB_TX_TRANSMIT_ENABLE == STD_OFF) && \
    (COM_CL_UB_TX_TXCONFIRMATION_ENABLE == STD_OFF)))
            if (COM_GET_COM_TX_IPDU_CLEAR_UPDATE_BIT(IPduPtr) == COM_CL_UB_TRIGGERTRANSMIT)
#endif

            {
              Com_EB_ClearTxUpdateBits(IPduPtr);
            }

#endif /* (COM_CL_UB_TX_TRIGGERTRANSMIT_ENABLE == STD_ON) */
#endif /* (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) */

            retval = E_OK;
          } /* if ( Com_IsBitSet_8(&Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase +
                 ComTxPduId)], COM_TXPDU_STARTED) ) */
        }

      }

#if (COM_DEV_ERROR_DETECT == STD_ON)
   }
#endif /* COM_DEV_ERROR_DETECT == STD_ON */


   DBG_COM_TRIGGERTRANSMIT_EXIT(retval,ComTxPduId,PduInfoPtr);
   return retval;
}                               /* Com_TriggerTransmit */

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
#endif

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/

