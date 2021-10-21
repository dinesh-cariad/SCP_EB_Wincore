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
 * MISRAC2012-1) Deviated Rule: 16.1 (required)
 * All switch statements shall be well-formed.
 *
 * Reason:
 * violation of Rule 16.1 caused by Rule 16.3.
 *
 * MISRAC2012-2) Deviated Rule: 16.3 (required)
 * An unconditional break statement shall terminate every switch-clause.
 *
 * Reason:
 * Shorter code size
 *
 */


/*==================[inclusions]=============================================*/

#include <Com_Trace.h>
#include <TSAutosar.h>          /* EB specific standard types */
#include <ComStack_Types.h>     /* AUTOSAR standard types */
#include <Com_Api_Static.h>     /* module public static API */
#include <Com_Cbk_Static.h>     /* Static "callbacks" of the COM */
#include <Com_Priv.h>           /* module internal stuff */

#include <Com_Core_TxIPdu.h>    /* Header TxIPdu functions */

#if (COM_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>                /* Det API */
#endif

#if (COM_TX_TPAPI_ENABLE == STD_ON)
#include <EcuC.h>
#endif /* (COM_TX_TPAPI_ENABLE == STD_ON) */

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_START_SEC_CODE
#include <Com_MemMap.h>
#endif


#if (COM_CBK_TX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE)

_COM_STATIC_ FUNC(void, COM_APPL_CODE) Com_EB_HandleTxConfirmations
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
);

#endif /* (COM_CBK_TX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) */


#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
#endif

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/



#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_START_SEC_CODE
#include <Com_MemMap.h>
#endif


#if (COM_CBK_TX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE)

_COM_STATIC_ FUNC(void, COM_APPL_CODE) Com_EB_HandleTxConfirmations
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
)
{

   /* Handle TxConfirmations */
#if ((COM_TX_CONFIRMATIONS_IMMEDIATE_AVAILABLE == STD_ON) && (COM_TX_CONFIRMATIONS_DEFERED_AVAILABLE == STD_ON))
   if (COM_GET_COM_TX_IPDU_SIGNAL_PROCESSING(IPduPtr) == COM_IMMEDIATE)
#endif /* ((COM_TX_CONFIRMATIONS_IMMEDIATE_AVAILABLE == STD_ON) && (COM_TX_CONFIRMATIONS_DEFERED_AVAILABLE == STD_ON)) */

#if (COM_TX_CONFIRMATIONS_IMMEDIATE_AVAILABLE == STD_ON)
   {
      /* Call at once */
      Com_EB_TxConfirmations(IPduPtr);
   }
#endif /* (COM_TX_CONFIRMATIONS_IMMEDIATE_AVAILABLE == STD_ON) */

#if ((COM_TX_CONFIRMATIONS_IMMEDIATE_AVAILABLE == STD_ON) && (COM_TX_CONFIRMATIONS_DEFERED_AVAILABLE == STD_ON))
   else
#endif /* ((COM_TX_CONFIRMATIONS_IMMEDIATE_AVAILABLE == STD_ON) && (COM_TX_CONFIRMATIONS_DEFERED_AVAILABLE == STD_ON)) */

#if (COM_TX_CONFIRMATIONS_DEFERED_AVAILABLE == STD_ON)
   {
      /* Defer confirmations */
      COM_AtomicSetBit_8(&Com_gDataMemPtr
                      [(Com_gConfigPtr->ComCbkTxAckDeferredStatusBase +
                        ((ComRAMIdxType)IPduPtr->ComNotificationDeferredQueueRef / 8U))],
                      (IPduPtr->ComNotificationDeferredQueueRef % 8U));
   }
#endif /* (COM_TX_CONFIRMATIONS_DEFERED_AVAILABLE == STD_ON) */

}

#endif /* (COM_CBK_TX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) */


_COM_STATIC_ FUNC(void, COM_CODE ) Com_EB_HlpTxConfirmation
(
    PduIdType ComTxPduId,
    P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
)
{
#if (COM_TM_AVAILABLE == STD_ON)
   uint8 AktTM;                 /* current TM mode */
   uint8 BitRes;
#endif /* (COM_TM_AVAILABLE == STD_ON) */

#if (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL)
#if (COM_CL_UB_TX_TXCONFIRMATION_ENABLE == STD_ON)

#if (!((COM_UPDATE_BIT_TX_CONFIG == COM_UPDATE_BIT_PRESENT_FOR_ALL) &&  \
      (COM_CL_UB_TX_TRANSMIT_ENABLE == STD_OFF) && \
      (COM_CL_UB_TX_TRIGGERTRANSMIT_ENABLE == STD_OFF)))

         if (COM_GET_COM_TX_IPDU_CLEAR_UPDATE_BIT(IPduPtr) == COM_CL_UB_CONFIRMATION)
#endif

         {
            Com_EB_ClearTxUpdateBits(IPduPtr);
         }

#endif /* (COM_CL_UB_TX_TXCONFIRMATION_ENABLE == STD_ON) */
#endif /* (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) */

#if (COM_MDT_QUEUE_AVAILABLE == STD_ON)
         {
            if ( Com_IsBitSet_8(&Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + ComTxPduId)],
                COM_TXPDU_MDTQUEUED) )
            {
               /* Get MDT */
               ComTxIPduMinimumDelayTimeFactorType const MDT = COM_GET_MDT(IPduPtr);

               /* Start MDT */
               Com_EB_MdtQueue((ComIPduRefType)ComTxPduId, IPduPtr, MDT);

            }
         }
#endif /* (COM_MDT_QUEUE_AVAILABLE == STD_ON) */


         /* Maybe we should check if the IPdu actually is started now.
          * But that'd take extra time and sporadic confirmations are unlikely to
          * appear */

#if (COM_TM_AVAILABLE == STD_ON)
         /* For Direct / MixedD / Periodic transmission mode, simply stop TxDM and
          * call Tx confirmation callbacks;
          * for NTimes and MixedN TM, stop TxDM if N becomes zero and call Tx
          * confirmation callbacks only then. */

         /* Have a look if the reason for sending is a periodic event */
         TS_GetBit(&Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + ComTxPduId)],
            COM_TXPDU_TMSSTATE, uint8, BitRes);

         /* get current TM */
         AktTM = COM_GET_COM_TX_MODE(BitRes, IPduPtr->ComTxModeModes);

         /* Deviation MISRAC2012-1 */
         switch (AktTM)
         {
#if (COM_TX_MODE_N_TIMES_ENABLE == STD_ON)
         /* Deviation MISRAC2012-2 */
         case COM_TX_MODE_MODE_N_TIMES:
            TS_GetBit(&Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + ComTxPduId)],
                  COM_TXPDU_NTIMESSENDQUEUED, uint8, BitRes);
            if (BitRes == FALSE)
            {
               /* In NTimes mode, ignore superfluous TxConfirmations
                * according to COM305 */
               break;
            }
            /* fall through if NTimes sending is in progress */
#endif /* (COM_TX_MODE_N_TIMES_ENABLE == STD_ON) */
#if (COM_TX_MODE_MIXED_N_TIMES_ENABLE == STD_ON)
         /* Deviation MISRAC2012-2 */
         case COM_TX_MODE_MODE_MIXED_N_TIMES:
#endif /* (COM_TX_MODE_MIXED_N_TIMES_ENABLE == STD_ON) */

#if (COM_N_TIMES_QUEUE_AVAILABLE== STD_ON)
         {
            uint8 N;
            if ( Com_IsBitSet_8(&Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + ComTxPduId)],
                COM_TXPDU_NTIMESSENDQUEUED) )
            {
               ComIPduRefType NRamBufIdx;
               CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) NTimesQueueBaseRemaining =
                     (P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED))
                     & Com_gDataMemPtr[Com_gConfigPtr->ComNTimesQueueBaseRemaining];

               /* We know that the N currently stored must be > 0,
                * as otherwise the PDU would not be NTIMESSENDQUEUED */
               NRamBufIdx = IPduPtr->NTimesQueueRef;

               /* Enter critical section */
               /* The section is critical as a concurrent access to N may
                * take place if Com_SendSignal or Com_StopIPduGroup interrupts
                * us. */
               Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_1();

               /* calculate new N = N - 1 */
               N = NTimesQueueBaseRemaining[NRamBufIdx] - 1;

               /* Decrease N-counter (times to send left) */
               NTimesQueueBaseRemaining[NRamBufIdx] = N;

               if (N == 0U)
               {
                  /* N sends successful, stop sending */
                  Com_ClearBitUInt8(&Com_gDataMemPtr
                        [(Com_gConfigPtr->ComTxIPduStatusBase + ComTxPduId)],
                        COM_TXPDU_NTIMESSENDQUEUED);
               }

               /* Leave critical section */
               Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_1();

               if (N > 0U)
               {
                  /* Do not send Tx confirmations, do not cancel TxDM */
                  break;
               }
            }
         }
#endif /* (COM_N_TIMES_QUEUE_AVAILABLE== STD_ON) */

         /* fall through to tx confirmations / TxDM cancellation */
#if (COM_TX_MODE_MIXED_DIRECT_ENABLE == STD_ON)
         case COM_TX_MODE_MODE_MIXED_DIRECT:
#endif /* COM_TX_MODE_MIXED_DIRECT_ENABLE */
            /* fall through */
#if (COM_TX_MODE_DIRECT_ENABLE == STD_ON)
         case COM_TX_MODE_MODE_DIRECT:
#endif /* (COM_TX_MODE_DIRECT_ENABLE == STD_ON) */
            /* fall through */
#if (COM_TX_MODE_PERIODIC_ENABLE == STD_ON)
         case COM_TX_MODE_MODE_PERIODIC:
#endif /* (COM_TX_MODE_PERIODIC_ENABLE == STD_ON) */
            /* The stopping of the TxDM and the confirmations could also be done
             * with locks enabled, but confirmations with locks enabled would
             * probably take too long.
             * In principle, there is a race here and the module will react
             * "wrong" if the TxConfirmation is interrupted by a function
             * restarting the TxDM at this location.
             * Nevertheless, the race will happen rather seldom and as the
             * upper layer can't know that the TxConfirmation came in before the
             * upper layer's call, the upper layer can't detect the "wrong"
             * behavior. Therefore the race is being tolerated here, as fixing
             * it doesn't justify the effort required to. */

#if (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)

            COM_AtomicClearBit_8(&Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + ComTxPduId)],
                  COM_TXPDU_TXDMQUEUED);

#endif /* (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */

#if (COM_CBK_TX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE)

            /* Handle TxConfirmations */
            Com_EB_HandleTxConfirmations(IPduPtr);

#endif /* (COM_CBK_TX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) */

            break;
         case COM_TX_MODE_MODE_NONE:


#if (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)
            /* rewind TxDM if TxDM is configured and only TM None is configured */
            Com_EB_TxDM_TMNone(IPduPtr, (ComIPduRefType)ComTxPduId);
#endif /* (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */


#endif /* (COM_TM_AVAILABLE == STD_ON) */

#if (COM_CBK_TX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE)

            /* Handle TxConfirmations */
            Com_EB_HandleTxConfirmations(IPduPtr);

#endif /* (COM_CBK_TX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) */
#if (COM_TM_AVAILABLE == STD_ON)
            break;
            /* CHECK: NOPARSE */
         default:
            COM_UNREACHABLE_CODE_ASSERT(COMServiceId_TxConfirmation);
            break;
            /* CHECK: PARSE */
         }
#endif /* (COM_TM_AVAILABLE == STD_ON) */

}


/** \brief Com_TxConfirmation
 * Function to signal the COM that an IPDU has been transmitted
 * Preconditions:
 * - COM should be initialized
 * \param[in] ComTxPduId - ID of the PDU which was transmitted successfully
 */
FUNC(void, COM_CODE ) Com_TxConfirmation
(
   PduIdType ComTxPduId
)
{
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr;

   DBG_COM_TXCONFIRMATION_ENTRY(ComTxPduId);

#if (COM_DEV_ERROR_DETECT == STD_ON)
   /* check whether Com is initialized */
   if (Com_InitStatus == COM_EB_UNINIT)
   {
      COM_DET_REPORTERROR(COM_E_UNINIT, COMServiceId_TxConfirmation);
   }
   /* check if ComTxPduId is valid */
   else if (Com_gConfigPtr->ComTxIPduArraySize <= ComTxPduId)
   {
      COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_TxConfirmation);
   }
   else

#endif /* (COM_DEV_ERROR_DETECT == STD_ON) */

   {
      /* get IPdu to which ComTxPduId identifies and check if the IPdu is valid */
      IPduPtr = COM_GET_CONFIG_ELEMENT(ComTxIPduType, Com_gConfigPtr->ComTxIPduRef, ComTxPduId);

#if ( (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) && (COM_DEV_ERROR_DETECT == STD_ON) )
      if( TRUE == COM_GET_COM_TX_IPDU_DUMMY(IPduPtr) )
      {
        COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_TxConfirmation);
      }
      else
#endif /* ( (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) && (COM_DEV_ERROR_DETECT == STD_ON) ) */
      {
        Com_EB_HlpTxConfirmation(ComTxPduId, IPduPtr);
      }

   }

   DBG_COM_TXCONFIRMATION_EXIT(ComTxPduId);
}                               /* Com_TxConfirmation */


/** \brief TpTxConfirmation
 * Function to signal the COM that an large IPDU has been transmitted
 * Preconditions:
 * - COM should be initialized
 * \param[in] PduId - ID of the large PDU which was transmitted successfully
 * \param[in] Result - Result of the transmission of the I-PDU
 */
FUNC(void, COM_CODE ) Com_TpTxConfirmation
(
    PduIdType PduId,
    NotifResultType Result
)
{

  DBG_COM_TPTXCONFIRMATION_ENTRY(PduId, Result);

#if (COM_TX_TPAPI_ENABLE == STD_ON)

#if (COM_DEV_ERROR_DETECT == STD_ON)
  /* check whether Com is initialized */
  if (Com_InitStatus == COM_EB_UNINIT)
  {
     COM_DET_REPORTERROR(COM_E_UNINIT, COMServiceId_TpTxConfirmation);
  }
  /* check if ComTxPduId is valid */
  else if (Com_gConfigPtr->ComTxIPduArraySize <= PduId)
  {
     COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_TpTxConfirmation);
  }
  else

#endif /* (COM_DEV_ERROR_DETECT == STD_ON) */

  {

    Std_ReturnType RetVal = E_NOT_OK;
    CONSTP2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr =
        COM_GET_CONFIG_ELEMENT(ComTxIPduType, Com_gConfigPtr->ComTxIPduRef, PduId);

#if (COM_DEV_ERROR_DETECT == STD_ON)
      if (IPduPtr->ComTxIpduTPControlInfoRef == COM_IPDU_ARRAY_SIZE_MAX)
      {
        /* enters also for dummy pdu, isvalidconfig check ensure array size max value */
        COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_TpTxConfirmation);
      }
      else
#endif /* (COM_DEV_ERROR_DETECT == STD_ON) */
      {
        /* get TP Control array of struct base */
        CONSTP2VAR(EcuC_TxControlInfoType, AUTOMATIC, COM_VAR_CLEARED) TpTxContPtrBase =
            COM_GET_MEMPTR(EcuC_TxControlInfoType, Com_gConfigPtr->ComTxIpduTPControlInfoBase);

        /* get TP Control struct value pointer */
        CONSTP2VAR(EcuC_TxControlInfoType, AUTOMATIC, COM_VAR_CLEARED) TpTxContPtrValPtr =
            &TpTxContPtrBase[IPduPtr->ComTxIpduTPControlInfoRef];

        RetVal = EcuC_TpTxConfirmation(TpTxContPtrValPtr, Result);

        if(RetVal == E_OK)
        {
          Com_EB_HlpTxConfirmation(PduId, IPduPtr);
        }
        /* else: Result == NTFRSLT_OK or ECUC_STATE_IDLE
         * -> no Com_Notifications */
      }

  }
#else
  TS_PARAM_UNUSED(PduId);
  TS_PARAM_UNUSED(Result);
#endif /* (COM_TX_TPAPI_ENABLE == STD_ON) */

  DBG_COM_TPTXCONFIRMATION_EXIT(PduId, Result);

}                               /* TpTxConfirmation */

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
#endif

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/

