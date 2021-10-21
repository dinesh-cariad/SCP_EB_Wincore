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

/*==================[inclusions]============================================*/

#include <Com_Trace.h>
#include <TSAutosar.h>          /* EB specific standard types */
#include <ComStack_Types.h>     /* AUTOSAR Standard types */
#include <Com_Api.h>                /* Module public API */
#include <Com_Priv.h>           /* Module private API */

#include <Com_Core_TxIPdu.h>    /* Header TxIPdu functions */

#if ((COM_TM_AVAILABLE == STD_ON) || (COM_TMS_ENABLE == STD_ON))
#include <Com_Core_TM.h> /* Transmission Mode functions */
#endif /* ((COM_TM_AVAILABLE == STD_ON) || (COM_TMS_ENABLE == STD_ON)) */

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_START_SEC_CODE
#include <Com_MemMap.h>
#endif


#if (COM_SENDUPONAPICALL == STD_ON)

/** \brief Com_EB_SendUponSendApiCall
 * Function which reacts on a send API call (SendSignal / SendSignalGroup)
 * \param[in] PduId - Id of the IPdu the API call was called for
 * \param[in] SendInMainFunctionTx - if TRUE, the Pdu is sent not now
 *            but deferred to the next main function
 * during the next Com_MainfunctionTx(), otherwise immediately
 */
TS_MOD_PRIV_DEFN FUNC(void, COM_CODE) Com_EB_SendUponSendApiCall
(
   ComIPduRefType PduId
#if ( (COM_SIGNAL_GW_ENABLE == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) )
   ,boolean SendInMainFunctionTx
#endif /* ( (COM_SIGNAL_GW_ENABLE == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) ) */
)
{
   uint8 AktTM;
   uint8 CurrentTMS;

   CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) PduStatusFlagsPtr =
      &Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + PduId)];

   CONSTP2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr =
            COM_GET_CONFIG_ELEMENT(ComTxIPduType, Com_gConfigPtr->ComTxIPduRef, PduId);


   /* Get TM State */
   TS_GetBit(PduStatusFlagsPtr, COM_TXPDU_TMSSTATE, uint8, CurrentTMS);

   /* Get transmission mode */
   AktTM = COM_GET_COM_TX_MODE(CurrentTMS, IPduPtr->ComTxModeModes);

   switch (AktTM)
   {
         /* See bugzilla 16833, 14178 */
         /* Specification interpretation:
          * 1. In periodic transmission mode, the TxDM will only be
          * restarted if it is not running if a new transmission request
          * comes in.
          * 2. In Direct/NTimes/Mixed TM, a call to Com_SendSignal /
          * Com_SendSignalGroup / Com_SendSignalGroupArray , will restart the TxDM. */
#if (COM_TX_MODE_N_TIMES_ENABLE == STD_ON)
      case COM_TX_MODE_MODE_N_TIMES:
#endif /* (COM_TX_MODE_N_TIMES_ENABLE == STD_ON) */
         /* fall through */

#if (COM_TX_MODE_MIXED_N_TIMES_ENABLE == STD_ON)
      case COM_TX_MODE_MODE_MIXED_N_TIMES:
#endif /* (COM_TX_MODE_MIXED_N_TIMES_ENABLE == STD_ON) */
         /* fall through */

#if (COM_TX_MODE_DIRECT_ENABLE == STD_ON)
      case COM_TX_MODE_MODE_DIRECT:
#endif /* (COM_TX_MODE_DIRECT_ENABLE == STD_ON) */
         /* fall through */

#if (COM_TX_MODE_MIXED_DIRECT_ENABLE == STD_ON)
      case COM_TX_MODE_MODE_MIXED_DIRECT:
#endif /* COM_TX_MODE_MIXED_DIRECT_ENABLE */


#if (COM_TX_MODE_NOT_NONE_OR_PERIODIC_AVAILABLE == STD_ON)
         {

            /* try to send Pdu if not possible defer transmission to next main function */
            Com_EB_Request_Send_Pdu(
                     PduId,
                     IPduPtr
#if ((COM_MDT_QUEUE_AVAILABLE == STD_ON) || (COM_SIGNAL_GW_ENABLE == STD_ON))
                     ,PduStatusFlagsPtr
#endif
#if ( (COM_SIGNAL_GW_ENABLE == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) )
                     , SendInMainFunctionTx
#endif /* ( (COM_SIGNAL_GW_ENABLE == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) ) */
            );

            switch (AktTM)
            {
#if (COM_TX_MODE_MIXED_N_TIMES_ENABLE == STD_ON)
               case COM_TX_MODE_MODE_MIXED_N_TIMES:
                  {
                     /* Re-Initialize N sending (MixedN) */
                     ComTxModeIdxType TMRef;    /* reference to the TM configuration */
                     P2CONST(ComTxModeMixedNTimesType, AUTOMATIC,
                             COM_APPL_CONST) TMMixedNTimesPtr;

                     ComTxModeRepetitionPeriodFactorType NPeriod;
                     uint8 N;
                     CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) NTimesQueueBaseRemaining =
                        (P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED))
                        & Com_gDataMemPtr[Com_gConfigPtr->ComNTimesQueueBaseRemaining];

                     TMRef = COM_GET_COM_TX_MODE_REF(CurrentTMS, IPduPtr);

                     TMMixedNTimesPtr = COM_GET_CONFIG_ELEMENT(ComTxModeMixedNTimesType,
                                                               Com_gConfigPtr->
                                                               ComTxModeMixedNTimesRef, TMRef);
                     NPeriod = TMMixedNTimesPtr->ComTxModeRepetitionPeriodFactor;
                     N = TMMixedNTimesPtr->ComTxModeNumberOfRepetitions;
                     TS_AtomicAssign8(NTimesQueueBaseRemaining[IPduPtr->NTimesQueueRef], N);
                     Com_EB_NTimesSendQueue(PduId, NPeriod, IPduPtr->NTimesQueueRef);
                     break;
                  }
#endif /* (COM_TX_MODE_MIXED_N_TIMES_ENABLE == STD_ON) */
#if (COM_TX_MODE_N_TIMES_ENABLE == STD_ON)
               case COM_TX_MODE_MODE_N_TIMES:
                  {
                     /* Re-Initialize N sendings (NTimes) */
                     ComTxModeIdxType TMRef;    /* reference to the TM configuration */
                     P2CONST(ComTxModeNTimesType, AUTOMATIC, COM_APPL_CONST) TMNTimesPtr;

                     uint8 N;
                     ComTxModeRepetitionPeriodFactorType NPeriod;
                     CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) NTimesQueueBaseRemaining =
                        (P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED))
                        & Com_gDataMemPtr[Com_gConfigPtr->ComNTimesQueueBaseRemaining];

                     TMRef = COM_GET_COM_TX_MODE_REF(CurrentTMS, IPduPtr);

                     TMNTimesPtr = COM_GET_CONFIG_ELEMENT(ComTxModeNTimesType,
                                                          Com_gConfigPtr->ComTxModeNTimesRef,
                                                          TMRef);
                     NPeriod = TMNTimesPtr->ComTxModeRepetitionPeriodFactor;
                     N = TMNTimesPtr->ComTxModeNumberOfRepetitions;

                     TS_AtomicAssign8(NTimesQueueBaseRemaining[IPduPtr->NTimesQueueRef], N);
                     Com_EB_NTimesSendQueue(PduId, NPeriod, IPduPtr->NTimesQueueRef);
                  }
                  break;
#endif /* (COM_TX_MODE_N_TIMES_ENABLE == STD_ON) */
#if (COM_TX_MODE_DIRECT_ENABLE == STD_ON)
               case COM_TX_MODE_MODE_DIRECT:
                   /* nothing to do */
                   break;
#endif /* (COM_TX_MODE_DIRECT_ENABLE == STD_ON) */
#if (COM_TX_MODE_MIXED_DIRECT_ENABLE == STD_ON)
               case COM_TX_MODE_MODE_MIXED_DIRECT:
                   /* nothing to do */
                   break;
#endif /* COM_TX_MODE_MIXED_DIRECT_ENABLE */
               /* CHECK: NOPARSE */
               default:
               {
                   COM_UNREACHABLE_CODE_ASSERT(COM_INTERNAL_API_ID);
               }
               break;
               /* CHECK: PARSE */
            }                   /* end inner switch(AktTM) */

            break;
         }

#endif /* (COM_TX_MODE_NOT_NONE_OR_PERIODIC_AVAILABLE == STD_ON) */

#if (COM_TX_MODE_PERIODIC_ENABLE == STD_ON)
      case COM_TX_MODE_MODE_PERIODIC:
         /* Do nothing upon send API call */
         /* fall through, otherwise compiler detects unreachable code */

#endif /* (COM_TX_MODE_PERIODIC_ENABLE == STD_ON) */
      case COM_TX_MODE_MODE_NONE:
         /* Do nothing */
         break;

         /* CHECK: NOPARSE */
      default:
         COM_UNREACHABLE_CODE_ASSERT(COM_INTERNAL_API_ID);
         break;
         /* CHECK: PARSE */
   }                            /* end switch */


   return;
}                               /* Com_EB_SendUponSendApiCall */


/** \brief Com_EB_Request_Send_Pdu
 * Function which tries to send out an I-PDU, otherwise defers transmission to next main function
 * \param[in] PduId - Id of the IPdu the API call was called for
 * \param[in] IPduPtr - reference to the configuration of the I-Pdu
 * \param[in] SendInMainFunctionTx - if TRUE, the Pdu is sent not now but
 * during the next Com_MainfunctionTx(), otherwise immediately
 */
TS_MOD_PRIV_DEFN FUNC(void, COM_CODE) Com_EB_Request_Send_Pdu
(
   ComIPduRefType PduId,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
#if ((COM_MDT_QUEUE_AVAILABLE == STD_ON) || (COM_SIGNAL_GW_ENABLE == STD_ON))
   ,P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) PduStatusFlagsPtr
#endif
#if ( (COM_SIGNAL_GW_ENABLE == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) )
   ,boolean SendInMainFunctionTx
#endif /* ( (COM_SIGNAL_GW_ENABLE == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) ) */
)
{
#if (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)
   /* Restart the TxDM, no matter if running or what */
   if (COM_GET_COM_TX_IPDU_DM_AVAILABLE(IPduPtr) == COM_TX_DM_ON)
   {
      Com_EB_TxDMQueue(PduId, COM_GET_TX_TIMEOUT_FACTOR(IPduPtr), IPduPtr->TxDMQueueRef);
   }
#endif /* COM_TX_TIMEOUT_FACTOR_AVAILABLE */

#if (COM_DEFERTX2MAINFUNC == STD_OFF)

#if ((COM_MDT_QUEUE_AVAILABLE == STD_ON) && (COM_SIGNAL_GW_ENABLE == STD_ON))
   if ((Com_IsBitSet_8(PduStatusFlagsPtr, COM_TXPDU_MDTQUEUED))||(SendInMainFunctionTx == TRUE))
#elif ((COM_MDT_QUEUE_AVAILABLE == STD_ON) && (COM_SIGNAL_GW_ENABLE == STD_OFF))
     if (Com_IsBitSet_8(PduStatusFlagsPtr, COM_TXPDU_MDTQUEUED))
#elif ((COM_MDT_QUEUE_AVAILABLE == STD_OFF) && (COM_SIGNAL_GW_ENABLE == STD_ON))
       if (SendInMainFunctionTx == TRUE)
#endif /* ((COM_MDT_QUEUE_AVAILABLE == STD_ON) && (COM_SIGNAL_GW_ENABLE == STD_ON)) */

#if ((COM_MDT_QUEUE_AVAILABLE == STD_ON) || (COM_SIGNAL_GW_ENABLE == STD_ON))
       {
         /* If Mdt-queued, schedule for sending asap. or Pdu shall be
          * sent because of a routed signal */
         COM_AtomicSetBit_8(PduStatusFlagsPtr, COM_TXPDU_SENDIMMEDIATELY);
       }
       else
#endif /* ((COM_MDT_QUEUE_AVAILABLE == STD_ON) || (COM_SIGNAL_GW_ENABLE == STD_ON)) */
       {
         /* send the Pdu out */
         Com_EB_Send_Pdu(PduId, IPduPtr);
       }

#else

   /* defer send request to Tx main function */
   COM_AtomicSetBit_8(PduStatusFlagsPtr, COM_TXPDU_SENDIMMEDIATELY);

#endif /* (COM_DEFERTX2MAINFUNC == STD_ON) */

}                               /* Com_EB_Request_Send_Pdu */


#endif /* (COM_SENDUPONAPICALL == STD_ON) */

#if (COM_N_TIMES_QUEUE_AVAILABLE == STD_ON)
/** \brief Com_EB_NTimesSendQueue - "queue" an PDU for sending in an
 * amount of time
 * this function schedules the sending of a PDU in some amount of time
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * - Must only be called for IPdus which have a transmission mode with
 * ntimes sending part
 * \param[in] PduId - ID of the PDU to be sent
 * \param[in] Offset - Time (in COM ticks) to pass by before sending
 * \param[in] EntryInNTimesQueue - position in of the PDU in N-Times Queue
 */
TS_MOD_PRIV_DEFN FUNC(void, COM_CODE) Com_EB_NTimesSendQueue
(
   ComIPduRefType PduId,
   ComTxModeRepetitionPeriodFactorType Offset,
   ComIPduRefType EntryInNTimesQueue
)
{

   ComTxModeRepetitionPeriodFactorType TimeOut; /* calculated point in time for sending */
   /* variable to hold global time in data type of ComTxModeRepetitionPeriodFactorType */
   ComTxModeRepetitionPeriodFactorType globalTime;

   /* Deviation MISRAC2012-1 <+3> */
   CONSTP2VAR(ComTxModeRepetitionPeriodFactorType, AUTOMATIC, COM_VAR_CLEARED)
   NTimesQueueBaseNextTimeBase =
     COM_GET_MEMPTR(ComTxModeRepetitionPeriodFactorType,Com_gConfigPtr->ComNTimesQueueBaseNextTime);

   /* address of storage of Next Time to send */
   CONSTP2VAR(ComTxModeRepetitionPeriodFactorType, AUTOMATIC, COM_VAR_CLEARED)
   NTimesQueueValNextTimePtr =
         &NTimesQueueBaseNextTimeBase[EntryInNTimesQueue];

#if (COM_TIMEBASE == STD_OFF)
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr =
         COM_GET_CONFIG_ELEMENT(ComTxIPduType, Com_gConfigPtr->ComTxIPduRef, PduId);
#endif /* (COM_TIMEBASE == STD_OFF) */

   /* Enter critical section */
   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   globalTime = (ComTxModeRepetitionPeriodFactorType) COM_GET_Com_GlobalTxTime(COM_GET_Com_MainFunctionTxIdx(IPduPtr));

   TimeOut = (ComTxModeRepetitionPeriodFactorType) (globalTime + Offset);
   *NTimesQueueValNextTimePtr = TimeOut;

   Com_SetBitUInt8(&Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + PduId)],
         COM_TXPDU_NTIMESSENDQUEUED);

   /* Leave critical section */
   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();


   return;
}                               /* Com_EB_NTimesSendQueue */
#endif /* (COM_N_TIMES_QUEUE_AVAILABLE == STD_ON) */



#if (COM_TMS_ENABLE == STD_ON)

/** \brief Com_EB_CalculateTM - calculates the TM of a Pdu
 * Preconditions:
 * - COM must be initialized
 * - should only be called in a critical section
 * \param[in] IPduPtr - reference to the configuration of the I-Pdu
 * \param[out] None
 * \return Function execution success status
 * \retval TRUE, TM is true; FALSE TM is false
 */
TS_MOD_PRIV_DEFN FUNC(boolean, COM_CODE) Com_EB_CalculateTM
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
)
{
   /* Although the might change flag is set, it does not mean that the
    * state really has changed. We have to check if at least one signal
    * has a positive result */
   /* get the first byte of the TMFlags */
   CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) IpduTMFlagsBase =
      (&Com_gDataMemPtr[(IPduPtr->ComIPduTMSFlagsRef)]);
   uint16 j;
   boolean newTM = FALSE;
   uint16 numberOfFlagBytes;

   /* number of TM Bits */
   uint16 NumberOfFlags =
      IPduPtr->ComIPduSignalWithFilterRefNum
      + IPduPtr->ComIPduSignalGroupWithFilterRefNum;


   /* get the number of Bytes which hold the TM flags of the I-Pdu */
   numberOfFlagBytes = (NumberOfFlags + 7U) / 8U;

   /* check if at least one bit is set */
   for(j = 0; j < numberOfFlagBytes; j++)
   {
      if (IpduTMFlagsBase[j] != 0U)
      {
          newTM = TRUE;
      }
   }                            /* for(j = 0; j < numberOfFlagBytes; j++) */


   return newTM;
}                               /* Com_EB_CalculateTM */


#endif /* (COM_TMS_ENABLE == STD_ON) */



#if (COM_PERIODIC_QUEUE_AVAILABLE == STD_ON)

/** \brief Com_EB_PeriodicSendQueue - "queue" an PDU for sending in an
 * amount of time
 * this function schedules the sending of a PDU in some amount of time
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * - Must only be called for IPdus which have a transmission mode with
 * periodic sending part
 * \param[in] Offset Time (in ticks) to pass by before sending
  * \param[in] EntryInPeriodicQueue position in of the PDU in Periodic Queue
 * or stopped (false)
 */
TS_MOD_PRIV_DEFN FUNC(void, COM_CODE) Com_EB_PeriodicSendQueue
(
   Com_PeriodicSendNextDueTimeType Offset,
   ComIPduRefType EntryInPeriodicQueue
#if (COM_TIMEBASE == STD_OFF)
   , Com_MainFunctionTx_IdxType Com_MainFunctionTx_Idx
#endif /* (COM_TIMEBASE == STD_OFF) */
)
{
   Com_PeriodicSendNextDueTimeType TimeOut;     /* calculated point in time for sending */
   /* variable to hold global time in data type of Com_PeriodicSendNextDueTime */
   Com_PeriodicSendNextDueTimeType globalTime;

   /* Deviation MISRAC2012-1 <+2> */
   CONSTP2VAR(Com_PeriodicSendNextDueTimeType, AUTOMATIC, COM_VAR_CLEARED) PeriodicQueueBase =
       COM_GET_MEMPTR(Com_PeriodicSendNextDueTimeType, Com_gConfigPtr->ComPeriodicQueueBase);

   CONSTP2VAR(Com_PeriodicSendNextDueTimeType, AUTOMATIC, COM_VAR_CLEARED) PeriodicQueueValPtr =
         &PeriodicQueueBase[EntryInPeriodicQueue];

/* Enter critical section */
   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   globalTime = (Com_PeriodicSendNextDueTimeType) COM_GET_Com_GlobalTxTime(Com_MainFunctionTx_Idx);

   TimeOut = (Com_PeriodicSendNextDueTimeType) (globalTime + Offset);

   *PeriodicQueueValPtr = TimeOut;

   /* Leave critical section */
   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();


   return;
}                               /* Com_EB_PeriodicSendQueue */
#endif /* (COM_PERIODIC_QUEUE_AVAILABLE == STD_ON) */


#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
#endif

