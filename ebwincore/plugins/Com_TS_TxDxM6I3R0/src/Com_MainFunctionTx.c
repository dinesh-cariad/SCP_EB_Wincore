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
 *
 * MISRAC2012-2) Deviated Rule: 16.1 (required)
 * All switch statements shall be well-formed.
 *
 * Reason:
 * violation of Rule 16.1 caused by Rule 16.3.
 *
 *
 * MISRAC2012-3) Deviated Rule: 16.3 (required)
 * An unconditional break statement shall terminate
 * every switch-clause.
 *
 *
 * Reason:
 * fall through allows shorter and non-duplicated code
 *
 */


/*==================[inclusions]=============================================*/

#include <Com_Trace.h>
#include <TSAutosar.h>          /* EB specific standard types */
#include <ComStack_Types.h>     /* AUTOSAR standard types */
#include <Com_Api_Static.h>     /* module public static API */
#include <Com_Priv.h>           /* module internal stuff */

#include <Com_Core_TxIPdu.h>    /* Header TxIPdu functions */

#if (COM_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>                /* Det API */
#endif

#if ((COM_TM_AVAILABLE == STD_ON) || (COM_TMS_ENABLE == STD_ON))
#include <Com_Core_TM.h> /* Transmission Mode functions */
#endif /* ((COM_TM_AVAILABLE == STD_ON) || (COM_TMS_ENABLE == STD_ON)) */

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_START_SEC_CODE
#include <Com_MemMap.h>
#endif


#if (COM_TM_AVAILABLE == STD_ON)

#if (COM_TM_SWITCH_AVAILABLE == STD_ON)

/** \brief Com_EB_CheckTMSforPdu - check if TM-switching must be done
 * Preconditions:
 * - COM must be initialized
 * \param[in] PduId - Id of the Pdu which should be checked
 * \param[in] IPduPtr - configuration of the Pdu
 * \param[out] None
 * \return Function execution success status
 * \retval TRUE if I-Pdu shall be sent, else FALSE
 */
_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_CheckTMSforPdu
(
   ComIPduRefType PduId,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
);


/** \brief Com_EB_TMSwitch - switch to new TM
 * Preconditions:
 * - COM must be initialized
 * \param[in] PduId - Id of the Pdu
 * \param[in] IPduPtr - pointer of the Pdu configuration
 * \param[in] CurrentTM - new TM
 * \param[out] None
 * \return Function execution success status
 * \retval TRUE if I-Pdu shall be sent, else FALSE
 */
_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_TMSwitch
(
   ComIPduRefType PduId,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   boolean CurrentTM
);

#endif /* (COM_TM_SWITCH_AVAILABLE == STD_ON) */

#if (COM_PERIODIC_QUEUE_AVAILABLE == STD_ON)

_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_CheckPeriodicSendforPdu
(
   ComIPduRefType PduId,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
);

#endif /* (COM_PERIODIC_QUEUE_AVAILABLE == STD_ON) */

#if (COM_N_TIMES_QUEUE_AVAILABLE == STD_ON)

_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_CheckNTimesSendforPdu
(
   ComIPduRefType PduId,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
);

#endif /* (COM_N_TIMES_QUEUE_AVAILABLE == STD_ON) */


#endif /* (COM_TM_AVAILABLE == STD_ON) */


#if (COM_MDT_QUEUE_AVAILABLE == STD_ON)


/** \brief Com_EB_CheckMdtforPdu - checks MDT for the given Pdu
 * This function checks the Mdt-table and un-blocks PDUs from sending
 * if a minimum delay time timed out (clears MDTQUEUED flag).
 * NOTE: function does not trigger sending of the Pdu.
 * Preconditions:
 * - COM must be initialized
 * - MDT has to be configured for the Pdu and MDT has to run
 * \param[in] PduId - Id of the Pdu which should be checked
 * \param[in] IPduPtr - configuration of the Pdu
 * \param[out] None
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_CheckMdtforPdu
(
   ComIPduRefType PduId,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
);

#endif /* (COM_MDT_QUEUE_AVAILABLE == STD_ON) */

#if (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)

_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_CheckTxDMforPdu
(
   ComIPduRefType PduId,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
#if (COM_MDT_QUEUE_AVAILABLE == STD_ON)
#if (COM_RETRY_FAILED_TRANSMIT_REQ == STD_ON)
   ,boolean TxDM_clearRTFR
#endif /* (COM_RETRY_FAILED_TRANSMIT_REQ == STD_ON) */
#endif /* (COM_MDT_QUEUE_AVAILABLE == STD_ON) */
);

#endif /* (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */


#if (COM_TX_CONFIRMATIONS_DEFERED_AVAILABLE == STD_ON)

_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_HandleDeferredTxConfirm
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
);

#endif /* (COM_TX_CONFIRMATIONS_DEFERED_AVAILABLE == STD_ON) */

/** \brief Com_EB_Handle_Send_Pdu - handles sending of I-Pdu
 * This function checks if an I-Pdu shall be transmitted with
 * respect to MDT and the ComEnableMDTForCyclicTransmission.
 * - Clears the SENDIMMEDIATELY flag if a pending transmission
 *  request could be issued successfully.
 * Preconditions:
 * - COM must be initialized
 * - API shall only be used in context of Com_MainfunctionTx
 * \param[in] PduId - Id of the Pdu which should be checked
 * \param[in] IPduPtr - configuration of the Pdu
 * \param[in] transmit - if TRUE an (additional) transmit request due
 *            to calculations in Com_MainFunctionTx is pending,
 *            else only the SENDIMMEDIATELY flag is used for calculation.
 * \param[out] None
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_Handle_Send_Pdu
(
   ComIPduRefType PduId,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) IPduFlagBytePtr,
   boolean transmit
);

/** \brief Com_EB_MainFunctionTx_hlp - Tx mainfunction helper
 * This function executes the Com_MainFunctionTx context for the
 * Com_MainFunctionTx_Generic and Com_MainFunctionTx.
 * Preconditions:
 * - COM must be initialized
 * - API shall only be used in context of Com_MainfunctionTx
 * or Com_MainFunctionTx_Generic
 * \param[in] PduId - Id of the Pdu which should handled
 * \param[out] None
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_MainFunctionTx_hlp
(
    ComIPduRefType PduId
);

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



#if (COM_TM_AVAILABLE == STD_ON)

#if (COM_TM_SWITCH_AVAILABLE == STD_ON)

/** \brief Com_EB_CheckTMSforPdu - check if TM-switching must be done
 * Preconditions:
 * - COM must be initialized
 * \param[in] PduId - Id of the Pdu which should be checked
 * \param[in] IPduPtr - configuration of the Pdu
 * \param[out] None
 * \return
 * \retval TRUE if I-Pdu shall be sent, else FALSE
 */

_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_CheckTMSforPdu
(
   ComIPduRefType PduId,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
)
{

   boolean newTM;
   boolean transmit = FALSE;

   CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) IPduFlagBytePtr =
         &Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + PduId)];


   /* get the TMChanged flag of Pdu */
   if ( Com_IsBitSet_8(IPduFlagBytePtr, COM_TXPDU_TMSCHANGED) )
   {
      COM_AtomicClearBit_8(IPduFlagBytePtr, COM_TXPDU_TMSCHANGED);

      TS_GetBit(IPduFlagBytePtr, COM_TXPDU_TMSSTATE, uint8, newTM);

      /* if TM has changed, switch to the new mode */
      transmit = Com_EB_TMSwitch(PduId, IPduPtr, newTM);

   }                         /* if (TMChanged) */

   return transmit;
}                               /* Com_EB_CheckTMSforPdu */


/** \brief Com_EB_TMSwitch - switch to new TM
 * Preconditions:
 * - COM must be initialized
 * \param[in] PduId - Id of the Pdu
 * \param[in] IPduPtr - pointer of the Pdu configuration
 * \param[in] CurrentTM - new TM
 * \param[out] None
 * \return
 * \retval TRUE if I-Pdu shall be sent, else FALSE
 */


_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_TMSwitch
(
   ComIPduRefType PduId,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   boolean CurrentTM
)
{
   boolean transmit = FALSE;
   uint8 mode;                  /* current TX mode */
#if (COM_PERIODIC_QUEUE_AVAILABLE == STD_ON)
   Com_PeriodicSendNextDueTimeType TimeToSendPeriodic = 0;      /* for cyclic sends */
#endif /* (COM_PERIODIC_QUEUE_AVAILABLE == STD_ON) */

   CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) IPduFlagBytePtr =
         &Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + PduId)];



   /* get the new TM */
   mode = COM_GET_COM_TX_MODE(CurrentTM, IPduPtr->ComTxModeModes);

   /* Prepare for setting up new TM and getting configuration for periodic
    * sending */
   /* Deviation MISRAC2012-2 */
   switch (mode)
   {
      /* Deviation MISRAC2012-3 */
      case COM_TX_MODE_MODE_NONE:
#if (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)
          /* disable TxDM for TM None of two TMs are configured */
          COM_AtomicClearBit_8(IPduFlagBytePtr, COM_TXPDU_TXDMQUEUED);
#endif /* (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */
          /* Cancel any periodic/n-times sending */
          /* fall through */
#if (COM_TX_MODE_DIRECT_ENABLE == STD_ON)
      case COM_TX_MODE_MODE_DIRECT:
#endif /* (COM_TX_MODE_DIRECT_ENABLE == STD_ON) */

#if (COM_PERIODIC_QUEUE_AVAILABLE == STD_ON)
         COM_AtomicClearBit_8(IPduFlagBytePtr, COM_TXPDU_PERIODICSENDQUEUED);
#endif /* (COM_PERIODIC_QUEUE_AVAILABLE == STD_ON) */

#if (COM_N_TIMES_QUEUE_AVAILABLE == STD_ON)
         COM_AtomicClearBit_8(IPduFlagBytePtr, COM_TXPDU_NTIMESSENDQUEUED);
#endif /* (COM_N_TIMES_QUEUE_AVAILABLE == STD_ON) */

         break;
#if (COM_TX_MODE_N_TIMES_ENABLE == STD_ON)
      case COM_TX_MODE_MODE_N_TIMES:
         /* NOTE: N-times queuing has to be done in context of SendSignal,
          * otherwise a TxConfirmation may be lost */
         COM_AtomicClearBit_8(IPduFlagBytePtr, COM_TXPDU_PERIODICSENDQUEUED);
         break;
#endif /* (COM_TX_MODE_N_TIMES_ENABLE == STD_ON) */

#if (COM_TX_MODE_PERIODIC_ENABLE == STD_ON)
      case COM_TX_MODE_MODE_PERIODIC:
#endif /* (COM_TX_MODE_PERIODIC_ENABLE == STD_ON) */
         /* fall through to cancellation of NTimes and get period */
#if (COM_TX_MODE_MIXED_DIRECT_ENABLE == STD_ON)
      case COM_TX_MODE_MODE_MIXED_DIRECT:
#endif /* COM_TX_MODE_MIXED_DIRECT_ENABLE */

#if ((COM_TX_MODE_MIXED_DIRECT_ENABLE == STD_ON) || (COM_TX_MODE_PERIODIC_ENABLE == STD_ON))
      {
         /* get configuration for new TM */
         ComTxModeIdxType TMRef;     /* reference to the TM configuration */
         P2CONST(ComTxModePeriodicType, AUTOMATIC, COM_APPL_CONST) TMPeriodicPtr;
         TMRef = COM_GET_COM_TX_MODE_REF(CurrentTM, IPduPtr);
         TMPeriodicPtr = COM_GET_CONFIG_ELEMENT(ComTxModePeriodicType,
                                                Com_gConfigPtr->ComTxModePeriodicRef, TMRef);
         TimeToSendPeriodic = TMPeriodicPtr->ComTxModeTimePeriodFactor;

         /* Cancel N-Times */
         COM_AtomicClearBit_8(IPduFlagBytePtr,  COM_TXPDU_NTIMESSENDQUEUED);
      }
      break;
#endif /* ((COM_TX_MODE_MIXED_DIRECT_ENABLE == STD_ON) || (COM_TX_MODE_PERIODIC_ENABLE == STD_ON)) */

#if (COM_TX_MODE_MIXED_N_TIMES_ENABLE == STD_ON)
      case COM_TX_MODE_MODE_MIXED_N_TIMES:
      {
         /* NOTE: N-times queuing has to be done in context of SendSignal,
          * otherwise a TxConfirmation may be lost */
         ComTxModeIdxType TMRef;     /* reference to the TM configuration */
         P2CONST(ComTxModeMixedNTimesType, AUTOMATIC, COM_APPL_CONST) TMMixedNTimesPtr;
         TMRef = COM_GET_COM_TX_MODE_REF(CurrentTM, IPduPtr);
         TMMixedNTimesPtr = COM_GET_CONFIG_ELEMENT(ComTxModeMixedNTimesType,
                                                   Com_gConfigPtr->ComTxModeMixedNTimesRef,
                                                   TMRef);
         TimeToSendPeriodic = TMMixedNTimesPtr->ComTxModeTimePeriodFactor;
      }

         break;
#endif /* (COM_TX_MODE_MIXED_N_TIMES_ENABLE == STD_ON) */
         /* CHECK: NOPARSE */
      default:
          COM_UNREACHABLE_CODE_ASSERT(COMServiceId_MainFunctionTx);
          break;
         /* CHECK: PARSE */
   }                            /* end switch(mode) */


   /* If TimeToSendPeriodic is legally configured, it can never be 0,
    * so if it is not 0, periodic sending is part of the new TM;
    * enable periodic sending, then */
#if (COM_PERIODIC_QUEUE_AVAILABLE == STD_ON)
   if (TimeToSendPeriodic != 0)
   {
      /* Schedule periodic sending */

      Com_EB_PeriodicSendQueue(TimeToSendPeriodic, IPduPtr->PeriodicQueueRef
#if (COM_TIMEBASE == STD_OFF)
          , IPduPtr->ComMainFunctionsTx_Idx
#endif /* (COM_TIMEBASE == STD_OFF) */
      );

      /* Mark the PDU as send queued */
      COM_AtomicSetBit_8(IPduFlagBytePtr, COM_TXPDU_PERIODICSENDQUEUED);

      /* Finally, if TM now is a mode with cyclic part, and the IPdu
       * was not sent because of an eventual triggered property, send
       * the IPdu out once at once. */

#if (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)

      /* Check if PDU has TxDM */
      if (COM_GET_COM_TX_IPDU_DM_AVAILABLE(IPduPtr) == COM_TX_DM_ON)
      {
         /* If it is not yet running, start Tx DM */
         if (!(Com_IsBitSet_8(IPduFlagBytePtr, COM_TXPDU_TXDMQUEUED)))
         {
            Com_EB_TxDMQueue(PduId, COM_GET_TX_TIMEOUT_FACTOR(IPduPtr), IPduPtr->TxDMQueueRef);
         }
      }
#endif /* (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */

      /* Set flag to send Pdu */
      transmit = TRUE;

   }                            /* if (TimeToSendPeriodic != 0) */
#endif /* (COM_PERIODIC_QUEUE_AVAILABLE == STD_ON) */

   return transmit;
}                               /* Com_EB_TMSwitch */

#endif /* (COM_TM_SWITCH_AVAILABLE == STD_ON) */

#if (COM_PERIODIC_QUEUE_AVAILABLE == STD_ON)

/** \brief Com_EB_CheckPeriodicSendforPdu
 * This function checks if a scheduled sending must be executed.
 * Preconditions:
 * - COM must be initialized
 * - Pdu has to be periodic queued
 * \param[in] PduId - Id of the Pdu which should be checked
 * \param[in] IPduPtr - configuration of the Pdu
 * \param[out] None
 * \return
 * \retval TRUE if I-Pdu shall be sent, else FALSE
 */

_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_CheckPeriodicSendforPdu
(
   ComIPduRefType PduId,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
)
{
   /* variable to hold global time in data type of TimePeriodFactor */
   Com_PeriodicSendNextDueTimeType globalTime;

   Com_PeriodicSendNextDueTimeType CompTime;    /* temporary variable for expiration time
                                                   calculation */
   Com_PeriodicSendNextDueTimeType Period = 0;      /* period for periodic PDU sending */

   uint8 BitRes;

   uint8 AktTM;     /* current TM mode */

   ComTxModeIdxType TMRef;  /* reference to the TM configuration */

   boolean transmit = FALSE;

#if (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)
   /* if QueuedTxDM == 0 the TxDM has to be started
    * if QueuedTxDM == 1 the TxDM is started or not configured (nothing to do) */
   uint8 QueuedTxDM = 1U;
#endif /* (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */

   /* Deviation MISRAC2012-1 <+2> */
   CONSTP2VAR(Com_PeriodicSendNextDueTimeType, AUTOMATIC, COM_VAR_CLEARED) PeriodicQueueBase =
         COM_GET_MEMPTR(Com_PeriodicSendNextDueTimeType, Com_gConfigPtr->ComPeriodicQueueBase);

   CONSTP2VAR(Com_PeriodicSendNextDueTimeType, AUTOMATIC, COM_VAR_CLEARED) PeriodicQueueValPtr =
         &PeriodicQueueBase[IPduPtr->PeriodicQueueRef];

   CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) IPduFlagBytePtr =
         &Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + PduId)];

   /* Enter critical section */
   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   globalTime = (Com_PeriodicSendNextDueTimeType) COM_GET_Com_GlobalTxTime(COM_GET_Com_MainFunctionTxIdx(IPduPtr));

   CompTime = *PeriodicQueueValPtr;

   if (globalTime == CompTime)
   {

#if (COM_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
      boolean proceedProcessing = TRUE;
#endif /* (COM_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON) */

      /* Have a look if the reason for sending is a periodic event */
      TS_GetBit(IPduFlagBytePtr, COM_TXPDU_TMSSTATE, uint8, BitRes);

      /* get current TM */
      AktTM = COM_GET_COM_TX_MODE(BitRes, IPduPtr->ComTxModeModes);
      TMRef = COM_GET_COM_TX_MODE_REF(BitRes, IPduPtr);

      switch (AktTM)
      {
#if (COM_TX_MODE_MIXED_N_TIMES_ENABLE == STD_ON)
      case COM_TX_MODE_MODE_MIXED_N_TIMES:
      {
         P2CONST(ComTxModeMixedNTimesType, AUTOMATIC, COM_APPL_CONST) TMMixedNTimesPtr;
         TMMixedNTimesPtr = COM_GET_CONFIG_ELEMENT(ComTxModeMixedNTimesType,
               Com_gConfigPtr->
               ComTxModeMixedNTimesRef, TMRef);
         Period = TMMixedNTimesPtr->ComTxModeTimePeriodFactor;
         break;
      }
#endif /* (COM_TX_MODE_MIXED_N_TIMES_ENABLE == STD_ON) */

#if ((COM_TX_MODE_MIXED_DIRECT_ENABLE == STD_ON) || (COM_TX_MODE_PERIODIC_ENABLE == STD_ON))
#if (COM_TX_MODE_MIXED_DIRECT_ENABLE == STD_ON)
      case COM_TX_MODE_MODE_MIXED_DIRECT:
         /* fall trough */
#endif /* (COM_TX_MODE_MIXED_DIRECT_ENABLE == STD_ON) */

#if (COM_TX_MODE_PERIODIC_ENABLE == STD_ON)
      case COM_TX_MODE_MODE_PERIODIC:
#endif /* (COM_TX_MODE_PERIODIC_ENABLE == STD_ON) */
      {
         P2CONST(ComTxModePeriodicType, AUTOMATIC, COM_APPL_CONST) TMPeriodicPtr;

         TMPeriodicPtr = COM_GET_CONFIG_ELEMENT(ComTxModePeriodicType,
               Com_gConfigPtr->ComTxModePeriodicRef,
               TMRef);
         Period = TMPeriodicPtr->ComTxModeTimePeriodFactor;
         break;
      }                /* COM_TX_MODE_MODE_MIXED_DIRECT or COM_TX_MODE_MODE_PERIODIC */
#endif /* ((COM_TX_MODE_MIXED_DIRECT_ENABLE == STD_ON) || (COM_TX_MODE_PERIODIC_ENABLE == STD_ON)) */

      /* CHECK: NOPARSE */
      default:
      {
#if (COM_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
         /* if the mode is neither periodic, mixed direct or
          * mixed N-times an error occurred */
         COM_UNREACHABLE_CODE_ASSERT(COMServiceId_MainFunctionTx);
         proceedProcessing = FALSE;
#endif /* (COM_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON) */
         break;
      }                         /* default */
      /* CHECK: PARSE */
      }                         /* switch (AktTM) */

#if (COM_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
      /* CHECK: NOPARSE */
      if (TRUE == proceedProcessing)
         /* CHECK: PARSE */
#endif /* (COM_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON) */
      {


#if (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)
         /* Check if PDU has TxDM */
         if (COM_GET_COM_TX_IPDU_DM_AVAILABLE(IPduPtr) == COM_TX_DM_ON)
         {
            TS_GetBit(IPduFlagBytePtr, COM_TXPDU_TXDMQUEUED, uint8, QueuedTxDM);

         }

#endif /* (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */

         transmit = TRUE;

      }                         /* if (TRUE == proceedProcessing) */

   }                            /* if (globalTime == CompTime) */

   /* Leave critical section */
   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   if (TRUE == transmit)
   {
      /* Schedule next sending */
      Com_EB_PeriodicSendQueue(Period, IPduPtr->PeriodicQueueRef
#if (COM_TIMEBASE == STD_OFF)
          , IPduPtr->ComMainFunctionsTx_Idx
#endif /* (COM_TIMEBASE == STD_OFF) */
      );
   }


#if (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)
   /* Check if PDU has TxDM */

   /* If it is not yet running, start Tx DM */
   if (QueuedTxDM == 0U)
   {
      Com_EB_TxDMQueue(PduId, COM_GET_TX_TIMEOUT_FACTOR(IPduPtr), IPduPtr->TxDMQueueRef);
   }
#endif /* (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */


   return transmit;
}                               /* Com_EB_CheckPeriodicSendTable */

#endif /* (COM_PERIODIC_QUEUE_AVAILABLE == STD_ON) */

#if (COM_N_TIMES_QUEUE_AVAILABLE == STD_ON)


/** \brief Com_EB_CheckNTimesSendforPdu
 * This function checks if a scheduled sending must be executed.
 * Preconditions:
 * - COM must be initialized
 * - Pdu has to be N-Times send queued
 * \param[in] PduId - Id of the Pdu which should be checked
 * \param[in] IPduPtr - configuration of the Pdu
 * \param[out] None
 * \return
 * \retval TRUE if I-Pdu shall be sent, else FALSE
 */

_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_CheckNTimesSendforPdu
(
   ComIPduRefType PduId,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
)
{
   /* variable to hold global time in data type of ComTxModeRepetitionPeriodFactorType */
   ComTxModeRepetitionPeriodFactorType globalTime;

   ComTxModeRepetitionPeriodFactorType CompTime;        /* temporary variable for expiration time
                                                           calculation */

   ComTxModeRepetitionPeriodFactorType NPeriod = 0U;    /* period of N-Times PDU sending */
   uint8 N = 0U;                     /* number of repetitions left */

   uint8 BitRes;
   uint8 AktTM;     /* current TM mode */
   ComTxModeIdxType TMRef;  /* reference to the TM configuration */

   boolean transmit = FALSE;

   CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) NTimesQueueBaseRemaining =
         (P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED))
         & Com_gDataMemPtr[Com_gConfigPtr->ComNTimesQueueBaseRemaining];

   /* address of the storage of N */
   CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) NTimesQueueValRemainingPtr =
         &NTimesQueueBaseRemaining[IPduPtr->NTimesQueueRef];

   /* Deviation MISRAC2012-1 <+3> */
   CONSTP2VAR(ComTxModeRepetitionPeriodFactorType, AUTOMATIC, COM_VAR_CLEARED)
   NTimesQueueBaseNextTimeBase = COM_GET_MEMPTR(ComTxModeRepetitionPeriodFactorType,
         Com_gConfigPtr->ComNTimesQueueBaseNextTime);

   /* address of storage of Next Time to send */
   CONSTP2VAR(ComTxModeRepetitionPeriodFactorType, AUTOMATIC, COM_VAR_CLEARED)
   NTimesQueueValNextTimePtr =
         &NTimesQueueBaseNextTimeBase[IPduPtr->NTimesQueueRef];

   /* Enter critical section */
   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   globalTime = (ComTxModeRepetitionPeriodFactorType) COM_GET_Com_GlobalTxTime(COM_GET_Com_MainFunctionTxIdx(IPduPtr));

   CompTime = *NTimesQueueValNextTimePtr;

   if (globalTime == CompTime)
   {

#if (COM_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
      boolean proceedProcessing = TRUE;
#endif /* (COM_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON) */

      /* Have a look if the reason for sending is a periodic event */
      TS_GetBit(&Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + PduId)],
         COM_TXPDU_TMSSTATE, uint8, BitRes);

      /* get current TM */
      AktTM = COM_GET_COM_TX_MODE(BitRes, IPduPtr->ComTxModeModes);
      TMRef = COM_GET_COM_TX_MODE_REF(BitRes, IPduPtr);
      switch (AktTM)
      {
#if (COM_TX_MODE_N_TIMES_ENABLE == STD_ON)
      case COM_TX_MODE_MODE_N_TIMES:
      {
         P2CONST(ComTxModeNTimesType, AUTOMATIC, COM_APPL_CONST) TMNTimesPtr;
         TMNTimesPtr = COM_GET_CONFIG_ELEMENT(ComTxModeNTimesType,
               Com_gConfigPtr->ComTxModeNTimesRef,
               TMRef);
         NPeriod = TMNTimesPtr->ComTxModeRepetitionPeriodFactor;
         break;
      }          /* case COM_TX_MODE_MODE_N_TIMES */
#endif /* (COM_TX_MODE_N_TIMES_ENABLE == STD_ON) */
#if (COM_TX_MODE_MIXED_N_TIMES_ENABLE == STD_ON)
      case COM_TX_MODE_MODE_MIXED_N_TIMES:
      {
         P2CONST(ComTxModeMixedNTimesType, AUTOMATIC,
               COM_APPL_CONST) TMMixedNTimesPtr;

         TMMixedNTimesPtr = COM_GET_CONFIG_ELEMENT(ComTxModeMixedNTimesType,
               Com_gConfigPtr->
               ComTxModeMixedNTimesRef, TMRef);
         NPeriod = TMMixedNTimesPtr->ComTxModeRepetitionPeriodFactor;
         break;
      }          /* case COM_TX_MODE_MODE_MIXED_N_TIMES */
#endif /* (COM_TX_MODE_MIXED_N_TIMES_ENABLE == STD_ON) */
      /* CHECK: NOPARSE */
      default:
      {
#if (COM_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
         COM_UNREACHABLE_CODE_ASSERT(COMServiceId_MainFunctionTx);
         proceedProcessing = FALSE;
#endif /* (COM_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON) */
         break;
      }          /* default */
      /* CHECK: PARSE */
      }                /* switch (AktTM) */

#if (COM_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
      /* CHECK: NOPARSE */
      if (TRUE == proceedProcessing)
         /* CHECK: PARSE */
#endif /* (COM_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON) */
      {
         N = *NTimesQueueValRemainingPtr;
      }
   }                   /* if (globalTime == CompTime) */

   /* Leave critical section */
   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   if (N > 0)
   {
      /* DO NOT decrease N; N is decreased in Com_TxConfirmation;
       * As long as N is not zero, send and schedule next sending */

      transmit = TRUE;

      /* Schedule next sending */
      Com_EB_NTimesSendQueue(PduId,
         NPeriod,
         IPduPtr->NTimesQueueRef);

   }                /* end N > 0 */

   return transmit;
}                               /* Com_EB_CheckNTimesSendforPdu */



#endif /* (COM_N_TIMES_QUEUE_AVAILABLE == STD_ON) */

#endif /* (COM_TM_AVAILABLE == STD_ON) */


#if (COM_MDT_QUEUE_AVAILABLE == STD_ON)

/** \brief Com_EB_CheckMdtforPdu - checks MDT for the given Pdu
 * This function checks the Mdt-table and un-blocks PDUs from sending
 * if a minimum delay time timed out (clears MDTQUEUED flag).
 * NOTE: function does not trigger sending of the Pdu.
 * Preconditions:
 * - COM must be initialized
 * - MDT has to be configured for the Pdu and MDT has to run
 * \param[in] PduId - Id of the Pdu which should be checked
 * \param[in] IPduPtr - configuration of the Pdu
 * \param[out] None
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_CheckMdtforPdu
(
   ComIPduRefType PduId,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
)
{

   /* variable to hold global time in data type of ComTxIPduMinimumDelayTimeFactorType */
   ComTxIPduMinimumDelayTimeFactorType globalTime;

   /* temporary variable for expiration time calculation */
   ComTxIPduMinimumDelayTimeFactorType CompTime;

   CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) PduFlagPtr =
         &Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + PduId)];

   /* Deviation MISRAC2012-1 <+2> */
   CONSTP2VAR(ComTxIPduMinimumDelayTimeFactorType, AUTOMATIC, COM_VAR_CLEARED) MDTQueueBase =
       COM_GET_MEMPTR(ComTxIPduMinimumDelayTimeFactorType, Com_gConfigPtr->ComMDTQueueBase);

   CONSTP2VAR(ComTxIPduMinimumDelayTimeFactorType, AUTOMATIC, COM_VAR_CLEARED) MDTQueueValPtr =
         &MDTQueueBase[IPduPtr->MDTQueueRef];

   /* Enter critical section */
   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   globalTime = (ComTxIPduMinimumDelayTimeFactorType) COM_GET_Com_GlobalTxTime(COM_GET_Com_MainFunctionTxIdx(IPduPtr));

   /* get the time out */
   CompTime = *MDTQueueValPtr;

   /* Check if this IPdu has timed out */
   if (globalTime == CompTime)
   {
      /* This PDU's MDT timed out; clear queued flag */
      Com_ClearBitUInt8(PduFlagPtr, COM_TXPDU_MDTQUEUED);
   }

   /* Leave critical section */
   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();


}                               /* Com_EB_CheckMdtforPdu */

#endif /* (COM_MDT_QUEUE_AVAILABLE == STD_ON) */


#if (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)

/** \brief Com_EB_CheckTxDMforPdu - checks if TxDM expired
 * Preconditions:
 * - COM must be initialized
 * - TxDM has to be running
 * \param[in] PduId - Id of the Pdu which should be checked
 * \param[in] IPduPtr - configuration of the Pdu#
 * \param[in] TxDM_clearRTFR - indicates if pending RTFR shall be cleared in case TxDM expires
 * \param[out] None
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_CheckTxDMforPdu
(
   ComIPduRefType PduId,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
#if (COM_MDT_QUEUE_AVAILABLE == STD_ON)
#if (COM_RETRY_FAILED_TRANSMIT_REQ == STD_ON)
   ,boolean TxDM_clearRTFR
#endif /* (COM_RETRY_FAILED_TRANSMIT_REQ == STD_ON) */
#endif /* (COM_MDT_QUEUE_AVAILABLE == STD_ON) */
)
{
   /* variable to hold global time in data type of ComTxTimeoutFactorType */
   ComTxTimeoutFactorType globalTime;

   ComTxTimeoutFactorType CompTime;

   boolean timeout = FALSE;

   CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) IPduFlagBytePtr =
         &Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + PduId)];

   /* Deviation MISRAC2012-1 <+2> */
   CONSTP2VAR(ComTxTimeoutFactorType, AUTOMATIC, COM_VAR_CLEARED) TxTimeoutQueueBase =
       COM_GET_MEMPTR(ComTxTimeoutFactorType, Com_gConfigPtr->ComTxTimeoutQueueBase);

   CONSTP2VAR(ComTxTimeoutFactorType, AUTOMATIC, COM_VAR_CLEARED) TxTimeoutQueueValPtr =
         &TxTimeoutQueueBase[IPduPtr->TxDMQueueRef];

   /* Enter critical section */
   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   globalTime = (ComTxTimeoutFactorType) COM_GET_Com_GlobalTxTime(COM_GET_Com_MainFunctionTxIdx(IPduPtr));

   CompTime = *TxTimeoutQueueValPtr;

   if (globalTime == CompTime)
   {

      timeout = TRUE;

      /* Entry times out right now */
      Com_ClearBitUInt8(IPduFlagBytePtr, COM_TXPDU_TXDMQUEUED);

#if (COM_N_TIMES_QUEUE_AVAILABLE == STD_ON)

      /* Un-Schedule NTimes-Sending */
      Com_ClearBitUInt8(IPduFlagBytePtr, COM_TXPDU_NTIMESSENDQUEUED);

#endif /* (COM_N_TIMES_QUEUE_AVAILABLE == STD_ON) */

#if (COM_RETRY_FAILED_TRANSMIT_REQ == STD_ON)
      {
#if (COM_MDT_QUEUE_AVAILABLE == STD_ON)

         /* clear pending ComRetryFailedTransmitRequests (RTF Requests) */

         if (TxDM_clearRTFR)
#endif /* (COM_MDT_QUEUE_AVAILABLE == STD_ON) */
         {
            /* cancel pending retry transmission requests */
            Com_ClearBitUInt8(IPduFlagBytePtr, COM_TXPDU_SENDIMMEDIATELY);
         }
      }

#endif /* (COM_RETRY_FAILED_TRANSMIT_REQ == STD_ON) */

#if (COM_MDT_QUEUE_AVAILABLE == STD_ON)

      /* stop MDT monitoring */
      Com_ClearBitUInt8(IPduFlagBytePtr, COM_TXPDU_MDTQUEUED);

#endif /* (COM_MDT_QUEUE_AVAILABLE == STD_ON) */

   }                   /* if (globalTime == CompTime) */


   /* LEAVE CRITICAL SECTION */
   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();


   if (timeout)
   {

      Com_CbkTxTOutIdxType j;  /* iterator variable */
      /* Notify configured signals */
      for (j = IPduPtr->ComTxTimeoutNotificationFirst;
            j < (IPduPtr->ComTxTimeoutNotificationFirst
                  + IPduPtr->ComTxTimeoutNotificationNum); j++)
      {
         /* call callback */
         Com_CbkTxTOut_Array[COM_GET_CBK_TX_T_OUT_IDX(j)] ();
      }

      {
#if (COM_TM_AVAILABLE == STD_ON)
         uint8 CurrentTMS;
         uint8 AktTM;

         /* get TM state of Pdu */
         TS_GetBit(IPduFlagBytePtr, COM_TXPDU_TMSSTATE, uint8, CurrentTMS);

         /* Get transmission mode */
         AktTM = COM_GET_COM_TX_MODE(CurrentTMS, IPduPtr->ComTxModeModes);

         if (COM_TX_MODE_MODE_NONE == AktTM)
#endif /* (COM_TM_AVAILABLE == STD_ON) */
         {
            /* rewind TxDM if only TM None is configured */
            Com_EB_TxDM_TMNone(IPduPtr, PduId);
         }
      }
   }                            /* if (timeout) */

}                               /* Com_EB_CheckTxDMTable */
#endif /* (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */


#if (COM_TX_CONFIRMATIONS_DEFERED_AVAILABLE == STD_ON)

_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_HandleDeferredTxConfirm
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
)
{

#if ((COM_TX_CONFIRMATIONS_IMMEDIATE_AVAILABLE == STD_ON) && (COM_TX_CONFIRMATIONS_DEFERED_AVAILABLE == STD_ON))
   if (COM_GET_COM_TX_IPDU_SIGNAL_PROCESSING(IPduPtr) == COM_DEFERED)
#endif /* ((COM_TX_CONFIRMATIONS_IMMEDIATE_AVAILABLE == STD_ON) && (COM_TX_CONFIRMATIONS_DEFERED_AVAILABLE == STD_ON)) */
   {

      if ( Com_IsBitSet_8(&Com_gDataMemPtr[(Com_gConfigPtr->ComCbkTxAckDeferredStatusBase +
          ((ComRAMIdxType)IPduPtr->ComNotificationDeferredQueueRef / 8U))],
          (IPduPtr->ComNotificationDeferredQueueRef % 8U)) )
      {
         /* Clear the flag */
         COM_AtomicClearBit_8(&Com_gDataMemPtr[(ComRAMIdxType)(Com_gConfigPtr->ComCbkTxAckDeferredStatusBase +
              ((ComRAMIdxType)IPduPtr->ComNotificationDeferredQueueRef / 8U))],
                              IPduPtr->ComNotificationDeferredQueueRef % 8U);

         /* handle confirmations */
         Com_EB_TxConfirmations(IPduPtr);
      }
   }

}

#endif /* (COM_TX_CONFIRMATIONS_DEFERED_AVAILABLE == STD_ON) */


/** \brief Com_EB_Handle_Send_Pdu - handles sending of I-Pdu
 * This function checks if an I-Pdu shall be transmitted with
 * respect to MDT and the ComEnableMDTForCyclicTransmission.
 * - Clears the SENDIMMEDIATELY flag if a pending transmission
 *  request could be issued successfully.
 * Preconditions:
 * - COM must be initialized
 * - API shall only be used in context of Com_MainfunctionTx
 * \param[in] PduId - Id of the Pdu which should be checked
 * \param[in] IPduPtr - configuration of the Pdu
 * \param[in] transmit - if TRUE an (additional) transmit request due
 *            to calculations in Com_MainFunctionTx is pending,
 *            else only the SENDIMMEDIATELY flag is used for calculation.
 * \param[out] None
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_Handle_Send_Pdu
(
   ComIPduRefType PduId,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) IPduFlagBytePtr,
   boolean transmit
)
{
   boolean to_transmit = transmit;


#if (COM_ENABLE_MDT_FOR_CYCLIC_TX == STD_ON)
   /* Postpone Tx request from MainFunction only if configured
    * (ComEnableMDTForCyclicTransmission == true) */
#if (COM_MDT_QUEUE_AVAILABLE == STD_ON)
   if ( Com_IsBitSet_8(IPduFlagBytePtr, COM_TXPDU_MDTQUEUED) )
   {
      if (TRUE == to_transmit)
      {
         COM_AtomicSetBit_8(IPduFlagBytePtr, COM_TXPDU_SENDIMMEDIATELY);
      }
   }
   else
#endif /* (COM_MDT_QUEUE_AVAILABLE == STD_ON) */
#endif /* (COM_ENABLE_MDT_FOR_CYCLIC_TX == STD_ON) */
   {

      /* check if there are pending event based Tx- Requests
       *  ( = Tx Request not from MainFunction) */
      if ( Com_IsBitSet_8(IPduFlagBytePtr, COM_TXPDU_SENDIMMEDIATELY) )
      {
#if (COM_ENABLE_MDT_FOR_CYCLIC_TX == STD_OFF)
         /* Postpone pending event based Tx Request
          * further if
          * (ComEnableMDTForCyclicTransmission == false) */
         if ( !(Com_IsBitSet_8(IPduFlagBytePtr, COM_TXPDU_MDTQUEUED)) )
#endif /* (COM_ENABLE_MDT_FOR_CYCLIC_TX == STD_OFF) */
         {
            to_transmit = TRUE;
         }
      }

      if(TRUE == to_transmit)
      {
         /* Clear send-immediately flag */
         COM_AtomicClearBit_8(IPduFlagBytePtr, COM_TXPDU_SENDIMMEDIATELY);

         /* send the Pdu out */
         Com_EB_Send_Pdu(PduId, IPduPtr);

      }
   }

}                               /* Com_EB_Handle_Send_Pdu */

/** \brief Com_EB_MainFunctionTx_hlp - Tx mainfunction helper
 * This function executes the Com_MainFunctionTx context for the
 * Com_MainFunctionTx_Generic and Com_MainFunctionTx.
 * Preconditions:
 * - COM must be initialized
 * - API shall only be used in context of Com_MainfunctionTx
 * or Com_MainFunctionTx_Generic
 * \param[in] PduId - Id of the Pdu which should be processed
 * \param[out] None
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_MainFunctionTx_hlp
(
    ComIPduRefType PduId
)
{
  boolean transmit = FALSE;

  P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr =
      COM_GET_CONFIG_ELEMENT(ComTxIPduType, Com_gConfigPtr->ComTxIPduRef, PduId);

#if (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON)
  if( FALSE == COM_GET_COM_TX_IPDU_DUMMY(IPduPtr) )
#endif /* (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) */
  {


  CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) IPduFlagBytePtr =
      &Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + PduId)];

#if ( (COM_TX_CONFIRMATIONS_DEFERED_AVAILABLE == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_ON) )

  Com_EB_HandleDeferredTxConfirm(IPduPtr);

#endif /* ( (COM_TX_CONFIRMATIONS_DEFERED_AVAILABLE == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_ON) ) */

  {

#if (COM_MDT_QUEUE_AVAILABLE == STD_ON)

#if (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)
#if (COM_RETRY_FAILED_TRANSMIT_REQ == STD_ON)
  /* clear pending ComRetryFailedTransmitRequests (RTF Requests) in case TxDM expires */
  /* MDT is only started when transmit was successful -> no RTF Request pending,
   * therefore eventually pending transmission request shall be cleared when
   * no MDT is running */
    boolean TxDM_clearRTFR = FALSE;
#endif /* (COM_RETRY_FAILED_TRANSMIT_REQ == STD_ON) */
#endif /* (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */

    /* Check if the MDT timed out, if so clear flag */
    /* has to be done even if Pdu is not started! */
    if ( Com_IsBitSet_8(IPduFlagBytePtr, COM_TXPDU_MDTQUEUED) )
    {
      Com_EB_CheckMdtforPdu(PduId, IPduPtr);
    }
#if (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)
#if (COM_RETRY_FAILED_TRANSMIT_REQ == STD_ON)
    else
    {
      TxDM_clearRTFR = TRUE;
    }
#endif /* (COM_RETRY_FAILED_TRANSMIT_REQ == STD_ON) */
#endif /* (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */

#endif /* (COM_MDT_QUEUE_AVAILABLE == STD_ON) */

    if ( Com_IsBitSet_8(IPduFlagBytePtr, COM_TXPDU_STARTED) )
    {
      /* Handle the I-Pdu only if started */

#if (COM_TM_SWITCH_AVAILABLE == STD_ON)

      /* Check if TMs must be switched for periodic parts */
      transmit = Com_EB_CheckTMSforPdu(PduId, IPduPtr);

#endif /* (COM_TM_SWITCH_AVAILABLE == STD_ON) */

#if (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)

      if ( Com_IsBitSet_8(IPduFlagBytePtr, COM_TXPDU_TXDMQUEUED) )
      {
        /* Check Tx DM */
        Com_EB_CheckTxDMforPdu(PduId, IPduPtr
#if (COM_MDT_QUEUE_AVAILABLE == STD_ON)
#if (COM_RETRY_FAILED_TRANSMIT_REQ == STD_ON)
            ,TxDM_clearRTFR
#endif /* (COM_RETRY_FAILED_TRANSMIT_REQ == STD_ON) */
#endif /* (COM_MDT_QUEUE_AVAILABLE == STD_ON) */
        );
      }

#endif /* (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */


#if (COM_TM_AVAILABLE == STD_ON)
#if (COM_N_TIMES_QUEUE_AVAILABLE == STD_ON)

      if ( Com_IsBitSet_8(IPduFlagBytePtr, COM_TXPDU_NTIMESSENDQUEUED) )
      {
        /* Check if Pdu has to be sent N-times */
        boolean const transmit_ntimes =
            Com_EB_CheckNTimesSendforPdu(PduId, IPduPtr);

        transmit =  transmit || transmit_ntimes;
      }

#endif /* (COM_N_TIMES_QUEUE_AVAILABLE == STD_ON) */

#if (COM_PERIODIC_QUEUE_AVAILABLE == STD_ON)

      if ( Com_IsBitSet_8(IPduFlagBytePtr, COM_TXPDU_PERIODICSENDQUEUED) )
      {
        /* Check if Pdu has to be sent cyclic */
        boolean const transmit_cyclic =
            Com_EB_CheckPeriodicSendforPdu(PduId, IPduPtr);

        transmit = transmit || transmit_cyclic;
      }

#endif /* (COM_PERIODIC_QUEUE_AVAILABLE == STD_ON) */

#endif /* (COM_TM_AVAILABLE == STD_ON) */


      /* Now check if the Pdu shall be sent and check if shall be sent
       * immediately */
      Com_EB_Handle_Send_Pdu(PduId, IPduPtr, IPduFlagBytePtr, transmit);

    }                      /* COM_TXPDU_STARTED */

  }

#if ( (COM_TX_CONFIRMATIONS_DEFERED_AVAILABLE == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) )

  Com_EB_HandleDeferredTxConfirm(IPduPtr);

#endif /* ( (COM_TX_CONFIRMATIONS_DEFERED_AVAILABLE == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) ) */

  } /* if( FALSE == COM_GET_COM_TX_IPDU_DUMMY(IPduPtr) ) */

}                               /* Com_EB_MainFunctionTx_hlp */

#if (COM_TIMEBASE == STD_ON)
/**
 * \brief Com_MainFunctionTx - handle cyclic sending-related tasks
 *
 * This function handles cyclic sending-related tasks such as minimum delay
 * time and cyclic sending.
 * Preconditions:
 * - COM must be initialized
 *
 *
 * \ServiceID{0x19}
 * \Reentrancy{Non re-entrant}
 * \Synchronicity{Synchronous}
 */
FUNC(void, COM_CODE) Com_MainFunctionTx
(
   void
)
{

   DBG_COM_MAINFUNCTIONTX_ENTRY();

   if (Com_InitStatus == COM_EB_INIT)
   {

      ComIPduRefType PduId;

      for (PduId = 0U; PduId < Com_gConfigPtr->ComTxIPduArraySize; PduId++)
      {

        Com_EB_MainFunctionTx_hlp(PduId);

      }                /* for (PduId = 0U; PduId < Com_gConfigPtr->ComTxIPduArraySize; PduId++) */

   }                            /* if (Com_Status == COM_EB_INIT) */

   DBG_COM_MAINFUNCTIONTX_EXIT();
}                               /* Com_MainFunctionTx */
#else
/* AUTOSAR declares Com_MainFunctionTx_<MainFunctionName_Dest>() also in SchM_Com.h which leads to
   "MISRA-C:2012 Rule 08.5" violation. */

/**
 * \brief Com_MainFunctionTx_Generic - Com_MainFunctionTx for I-Pdus which are assigned to
 * Tx main function with the Id ComMainFunctionsTx_Idx
 *
 * This function handles cyclic sending-related tasks such as minimum delay
 * time and cyclic sending for I-Pdus which are assigned to Tx main function with the Id
 * ComMainFunctionsTx_Idx.
 * Preconditions:
 * - COM must be initialized
 *
 * \param[in] ComMainFunctionsTx_Idx - Id of the main function
 *
 * \ServiceID{0x19}
 * \Reentrancy{Non re-entrant}
 * \Synchronicity{Synchronous}
 */
FUNC(void, COM_CODE) Com_MainFunctionTx_Generic
(
    Com_MainFunctionTx_IdxType ComMainFunctionsTx_Idx
)
{
   DBG_COM_MAINFUNCTIONTX_ENTRY();

   if (Com_InitStatus == COM_EB_INIT)
   {

#if (COM_DEV_ERROR_DETECT == STD_ON)
      if(ComMainFunctionsTx_Idx >= COM_MAIN_FUNCS_TX)
      {
        COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_MainFunctionTx);
      }
      else
#endif /* (COM_DEV_ERROR_DETECT == STD_ON) */
      {

#if (COM_GLOBAL_TIME != COM_SIZE_0_BIT)
      /* Make the time tick */
      Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

      Com_GlobalTimes_Tx[ComMainFunctionsTx_Idx] += 1;

      Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();
#endif /* (COM_GLOBAL_TIME != COM_SIZE_0_BIT) */
      {
        ComIPduRefType PduIdCount = 0U;

        /* get main context information pointer for pdu processing */
        CONSTP2CONST(ComMainfunctionTxType, AUTOMATIC, COM_APPL_CONST) MainTxInfoPtr =
        COM_GET_CONFIG_ELEMENT(ComMainfunctionTxType,
                               Com_gConfigPtr->ComMainfunctionTxRef,
                               ComMainFunctionsTx_Idx);

        for (PduIdCount = 0U; PduIdCount < MainTxInfoPtr->ComMainPduIDListSize; PduIdCount++)
        {
          /* get starting element of pduId list from the corresponding Tx main function sector */
         ComIPduRefType PduId = * COM_GET_CONFIG_ELEMENT(ComIPduRefType, MainTxInfoPtr->ComMainPduIDListRef, PduIdCount);

         Com_EB_MainFunctionTx_hlp(PduId);

        }                /* for (PduIdCount = 0U; PduIdCount < MainTxInfoPtr->ArraySize_Main; PduIdCount++) */

      }

      }                /* if(ComMainFunctionsTx_Idx >= COM_MAIN_FUNCS_TX) */

   }                            /* if (Com_Status == COM_EB_INIT) */

   DBG_COM_MAINFUNCTIONTX_EXIT();
}                               /* Com_MainFunctionTx */
#endif /* (COM_TIMEBASE == STD_ON) */

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
#endif

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/
