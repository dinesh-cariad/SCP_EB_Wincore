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

/* NOCOV List
 *
 *
 * NOCOV-UNSUPPORTEDOPTIMIZATION:
 *
 * Reason:
 * A feature is not supported for this release and is inactivated with
 * pre-compile time switches (and therefore not included into the binary).
 */

/*==================[inclusions]============================================*/

#include <Com_Trace.h>
#include <TSAutosar.h>          /* EB specific standard types */
#include <ComStack_Types.h>     /* AUTOSAR Standard types */
#include <Com_Api.h>            /* Module public API */
#include <Com_Lcfg_Static.h>    /* declaration of the callback arrays */
#include <Com_Priv.h>           /* Module private API */
#include <TSMem.h>              /* TS_MemCpy */
#include <Com_Filter.h>         /* Filter functions */
#include <Com_Core_RxIPdu.h>    /* RxIPdu functions */

#if (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_ON)
#include <Com_Core_Read.h>
#endif

#if (COM_RX_TPAPI_ENABLE == STD_ON)
#include <EcuC.h>
#endif /* (COM_RX_TPAPI_ENABLE == STD_ON) */

#if (COM_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>                /* Det API */
#endif

#if (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL)
#include <Com_Core_RxUpdateBit.h> /* UpdateBit Functions */
#endif /*(COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) */
/*==================[macros]=================================================*/

#if (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON)

#define COM_MULTI_ROUTE_STATUS_LENGTH (2*COM_MAIN_FUNCS_TX)

#if (COM_RX_DYN_LENGTH_IPDU_ENABLE == STD_ON)
#define COM_MULTI_ROUTE_RX_DYN_LENGTH sizeof(ComIPduSizeType)
#else
#define COM_MULTI_ROUTE_RX_DYN_LENGTH 0U
#endif /* (COM_RX_DYN_LENGTH_IPDU_ENABLE == STD_ON) */

#endif /* (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON) */

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_START_SEC_CODE
#include <Com_MemMap.h>
#endif

#if (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_PRESENT_FOR_ALL)
#if (COM_RX_TIMEOUT_FACTOR_SIZE != COM_SIZE_0_BIT)

/** \brief Com_EB_HandlePduRxDM - sets the Pdu based Rx deadline monitoring
  ** Preconditions:
 ** - COM should be initialized
 ** \param[in] IPduPtr - pointer to the configuration of the Pdu
 ** \return Function execution success status
 ** \retval 0 Pdu base RxDM was not restarted
 ** \retval 1 Pdu base RxDM was restarted after a time out
 */
_COM_STATIC_ FUNC(uint8, COM_CODE) Com_EB_HandlePduRxDM
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
);

#endif /* (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_PRESENT_FOR_ALL) */
#endif /* (COM_RX_TIMEOUT_FACTOR_SIZE != COM_SIZE_0_BIT) */

#if (COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)

/** \brief Com_EB_HandleRxIPdu_EnableRxDM - starts RxDM of I-Pdu and signal based RxDM
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * - invoke only when Pdu is started.
 * \param[in] IPduPtr - pointer to the description of the Pdu
 * \param[in] IPduFlagBytePtr - Pointer to byte where the flags of the Pdu are
 * stored
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_HandleRxIPdu_EnableRxDM
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) IPduFlagBytePtr
);

/** \brief Com_EB_HandleRxIPdu_DisableRxDM - stops RxDM of I-Pdu and signal based RxDM
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * \param[in] IPduPtr - pointer to the description of the Pdu
 * \param[in] IPduFlagBytePtr - Pointer to byte where the flags of the Pdu are
 * stored
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_HandleRxIPdu_DisableRxDM
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) IPduFlagBytePtr
);

#endif /* (COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */


/** \brief Com_EB_HandleRxIPdu_Start - starts I-Pdu
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * \param[in] IPduId - Id of Pdu which shall be started
 * \param[in] Initialize - defines if the Pdu group should be initialized
 * \param[in] IPduFlagBytePtr - Pointer to byte where the flags of the Pdu are
 * stored
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_HandleRxIPdu_Start
(
   ComIPduRefType IPduId,
   boolean Initialize,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) IPduFlagBytePtr
);


#if (COM_SIGNALS_WITH_UB_RXDM_GW_ONLY == STD_ON)
/** \brief Com_EB_HandleRxSignalsDMUBOnly - handles all Rx signals within an
 * I-Pdu which have only a signal based DM and/or an update bit
 * this function handles all Rx signals Signals within an I-Pdu which have
 * filters with filter COM_F_ALWAYS or COM_F_NEVER, no notification, no signal
 * invalidation configured. Evaluates only the update bit and DM
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * - the filter has to be COM_F_ALWAYS or COM_F_NEVER (or no filter configured)
 * - a notification callback function has to be configured
 * - no invalid value has to be configured
 * \param[in] IPduPtr - pointer to the description of the signal
 * \param[in] PduId - ID of the I-PDU
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_HandleRxSignalsDMUBOnly
(
      P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
      ComIPduRefType PduId
);
#endif /* (COM_SIGNALS_WITH_UB_RXDM_GW_ONLY == STD_ON) */

#if (COM_CBK_RX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE)
/** \brief Com_EB_HandleRxSig_W_F_A_A_Noti - handles
 * Rx signals with filter COM_F_ALWAYS, Notification but no signal invalidation
 * this function handles the Rx signals which have filters COM_F_ALWAYS and a
 * Notification callback configured but no signal invalidation configured.
 * Evaluates the update bit, DM, and calls the notification callback
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * - the filter has to be COM_F_ALWAYS
 * - a notification callback function has to be configured
 * - no invalid value has to be configured
 * \param[in] IPduPtr - pointer to the description of the signal
 * \param[in] PduId - ID of the I-PDU
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_HandleRxSig_W_F_A_A_Noti
(
      P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
      ComIPduRefType PduId
);

#endif /* (COM_CBK_RX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) */

#if (COM_FILTER_RECEIVER_ENABLE == STD_ON)

/** \brief Com_EB_HandleRxSignalsWithBuffer - handles Rx signals with buffers
 * this function handles the Rx signals which have filters (but not COM_F_NEVER
 * or COM_F_ALWAYS) or an invalid value. Evaluates the update bit and DM,
 * invalidates the signal, filters the value (after invalidation as defined in
 * SWS 3.0 -> Deviation in SWS 2.1) updates signal buffers and call the
 * callback functions (receive and invalid)
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * \param[in] IPduPtr - pointer to the description of the I-PDU
 * \param[in] PduId - ID of the I-PDU
 * \param[in] pduRxDMRestat - indicates if the Pdu base RxDM has just been restarted after a
 * time out
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_HandleRxSignalsWithBuffer
(
        P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
        ComIPduRefType PduId,
        uint8 pduRxDMRestat
);

#endif /* ((COM_FILTER_RECEIVER_ENABLE == STD_ON) */

#if (COM_NOTIFY_RECEIVERS_ENABLE == STD_ON)
/** \brief Com_EB_HandleRxSignalGroups - handles Rx signals groups
 * this function evaluates the update bit and DM,
 * invalidates the signal group calls callback of group signals and the
 * signal group
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * \param[in] IPduPtr - pointer to the description of the signal
 * \param[in] PduId - ID of the I-PDU
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_HandleRxSignalGroups
(
      P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
      ComIPduRefType PduId
);
#endif /* #if (COM_NOTIFY_RECEIVERS_ENABLE == STD_ON) */

#if (COM_FILTER_RECEIVER_ENABLE == STD_ON)
/** \brief Com_EB_InitRxSignalsWithFilter - initializes the signal buffers and
 * the RxSigGWFlag of all signals of the Rx-Pdus
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * \param[in] IPduPtr - pointer to the description of the signal
 */

_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_InitRxSignalsWithFilter
(
      P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
);
#endif /* (COM_FILTER_RECEIVER_ENABLE == STD_ON) */


#if ((COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) || (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_ON))
/** \brief Com_EB_CheckRxSignal_Receive_UpdateBitAndDM - handles update bit and DM of
 * Rx signals this function evaluates
 * - if the signal was received (handling of short PDUs)
 * - checks if update bit is configured and handles it
 * - checks if signal based DM is configured and handles it
 * \param[in] SignalPtr - pointer to configuration of signal
 * \param[in] IPduPtr - pointer to configuration of I-PDU
 * \param[in,out] passFilter_MASKED_NEW_DIFFERS_MASKED_OLD -
 *         in: indicates if the Pdu base RxDM has been just restarted
 *         out: If signal based RxDM has just been restarted after a time out, the value is set to 1.
                If signal based RxDM did not timed out before, value is set to 0.
                If no update bit is configured, value is not modified.
 * \param[in] pduLength - length of the Pdu
 * \param[in] checkRequired - shall be TRUE if it is necessary to check if the signal has been received
 * \return Function execution success status
 * \retval TRUE if the signal was updated, else FALSE
 */
_COM_STATIC_ FUNC(uint8, COM_CODE) Com_EB_CheckRxSignal_Receive_UpdateBitAndDM
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(uint8, AUTOMATIC, AUTOMATIC) passFilter_MASKED_NEW_DIFFERS_MASKED_OLD,
    uint32 pduLength,
    uint8 checkRequired
);

_COM_STATIC_ FUNC(uint8, COM_CODE) Com_EB_CheckRxSignalGroup_Receive_UpdateBitAndDM
(
   P2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   uint32 pduLength
);
#endif /* ((COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) || (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_ON)) */

#if (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_PRESENT_FOR_ALL)
#if (COM_RX_TIMEOUT_FACTOR_SIZE != COM_SIZE_0_BIT)

/** \brief Com_EB_HandlePduRxDM - sets the Pdu based Rx deadline monitoring
  ** Preconditions:
 ** - COM should be initialized
 ** \param[in] IPduPtr - pointer to the configuration of the Pdu
 ** \return Function execution success status
 ** \retval 0 Pdu base RxDM was not restarted
 ** \retval 1 Pdu base RxDM was restarted after a time out
 */
_COM_STATIC_ FUNC(uint8, COM_CODE) Com_EB_HandlePduRxDM
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
)
{

   uint8 restart = 0U;

   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) IPduFlagBytePtr;

   ComIPduRefType ComRxPduId;


   COM_GET_RX_PDU_ID(ComRxPduId, IPduPtr);

   IPduFlagBytePtr =
         &Com_gDataMemPtr[(Com_gConfigPtr->ComRxIPduStatusBase + ComRxPduId)];

   if (COM_GET_COM_RX_IPDU_DM(IPduPtr) == COM_RX_DM_ON)
   {

      Com_RxDMNextDueTimeType const TimeOutOffset = COM_GET_RX_TIMEOUT_FACTOR(IPduPtr);

      /* enter critical section */
      Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

      /* check if RxDM is enabled */
      if ( Com_IsBitSet_8(IPduFlagBytePtr, COM_RXPDU_RXDMRUNNING) )
      {

         TS_GetBit(IPduFlagBytePtr, COM_RXPDU_RXDMTIMEOUT, uint8, restart);

         /* clear time out flag, new value is available */
         Com_ClearBitUInt8(IPduFlagBytePtr, COM_RXPDU_RXDMTIMEOUT);

         /* restart RxDM */
         Com_EB_RxDMQueue(IPduPtr, TimeOutOffset);
      }
#if (COM_RX_FIRST_TIMEOUT_FACTOR_SIZE != COM_SIZE_0_BIT)
      else
      {
         /* RxDM is not enabled, check if shall be enabled */
         /* check if RxDM is switched on */
         if ( Com_IsBitSet_8(IPduFlagBytePtr, COM_RXPDU_RXDMSWITCHON) )
         {

            TS_GetBit(IPduFlagBytePtr, COM_RXPDU_RXDMTIMEOUT, uint8, restart);

            /* clear time out flag, new value is available */
            Com_ClearBitUInt8(IPduFlagBytePtr, COM_RXPDU_RXDMTIMEOUT);

            /* enable RxDM */
            Com_SetBitUInt8(IPduFlagBytePtr, COM_RXPDU_RXDMRUNNING);

            /* start RxDM */
            Com_EB_RxDMQueue(IPduPtr, TimeOutOffset);
         }
      }
#endif /* (COM_RX_FIRST_TIMEOUT_FACTOR_SIZE != COM_SIZE_0_BIT) */

      /* leave critical section */
      Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   }                            /* if (COM_GET_COM_RX_IPDU_DM(IPduPtr) == COM_RX_DM_ON) */

   return restart;
}                               /* Com_EB_HandlePduRxDM */

#endif /* (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_PRESENT_FOR_ALL) */
#endif /* (COM_RX_TIMEOUT_FACTOR_SIZE != COM_SIZE_0_BIT) */



#if (COM_NOTIFY_RECEIVERS_ENABLE == STD_ON)

/** \brief Com_EB_NotifyReceivers
 * Notifies the receivers about the reception of an PDU. For signals within
 * the PDU Reception filtering is performed
 * \param[in] IPduPtr: pointer to the configuration of the PDU
 * \param[in] PduId - ID of the I-PDU
 */
TS_MOD_PRIV_DEFN FUNC(void, COM_CODE) Com_EB_NotifyReceivers
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   ComIPduRefType PduId
)
{
#if ((COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_PRESENT_FOR_ALL) && \
       (COM_RX_TIMEOUT_FACTOR_SIZE != COM_SIZE_0_BIT) && \
       (COM_FILTER_RECEIVER_ENABLE == STD_ON))
   uint8 pduRxDMRestat;
#elif (COM_FILTER_RECEIVER_ENABLE == STD_ON)
   uint8 pduRxDMRestat = 0U;
#endif /* ((COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_PRESENT_FOR_ALL) && \
       (COM_RX_TIMEOUT_FACTOR_SIZE != COM_SIZE_0_BIT) && \
       (COM_FILTER_RECEIVER_ENABLE == STD_ON)) */


#if ((COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_PRESENT_FOR_ALL) && \
       (COM_RX_TIMEOUT_FACTOR_SIZE != COM_SIZE_0_BIT))
# if (COM_FILTER_RECEIVER_ENABLE == STD_ON)
   pduRxDMRestat = Com_EB_HandlePduRxDM(IPduPtr);
# else
   /* information whether or not Pdu based RxDM was restarted after a time out
    * is not required if Rx Filtering is disabled */
   (void)Com_EB_HandlePduRxDM(IPduPtr);
# endif
#endif /* ((COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_PRESENT_FOR_ALL) && \
       (COM_RX_TIMEOUT_FACTOR_SIZE != COM_SIZE_0_BIT)) */

#if (COM_FILTER_RECEIVER_ENABLE == STD_ON)
   /* Handle all signals with buffers */
   Com_EB_HandleRxSignalsWithBuffer(IPduPtr, PduId, pduRxDMRestat);
#endif /* (COM_FILTER_RECEIVER_ENABLE == STD_ON) */

#if (COM_CBK_RX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE)
   /* Handle the Rx signals which have the filter COM_F_ALWAYS and a notification
    * (no invalid value) */
   Com_EB_HandleRxSig_W_F_A_A_Noti(IPduPtr, PduId);
#endif /* (COM_CBK_RX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) */

#if (COM_SIGNALS_WITH_UB_RXDM_GW_ONLY == STD_ON)
   /* Handle signals which have only signal base DM and update bits*/
   Com_EB_HandleRxSignalsDMUBOnly(IPduPtr, PduId);
#endif /* (COM_SIGNALS_WITH_UB_RXDM_GW_ONLY == STD_ON) */

   /* Handle signal groups */
   Com_EB_HandleRxSignalGroups(IPduPtr, PduId);


   return;
}                               /* Com_EB_NotifyReceivers */

#endif /* (COM_NOTIFY_RECEIVERS_ENABLE == STD_ON) */


/** \brief Com_EB_HandleRxIPdu_Start - starts I-Pdu
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * \param[in] IPduId - Id of Pdu which shall be started
 * \param[in] Initialize - defines if the Pdu group should be initialized
 * \param[in] IPduFlagBytePtr - Pointer to byte where the flags of the Pdu are
 * stored
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_HandleRxIPdu_Start
(
   ComIPduRefType IPduId,
   boolean Initialize,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) IPduFlagBytePtr
)
{

   if (Initialize == TRUE)
   {
       /* initialize the Pdu */
      Com_EB_InitRxIPdu(IPduId);
   }

   /* Start group */
   COM_AtomicSetBit_8(IPduFlagBytePtr, COM_RXPDU_STARTED);

}                               /* Com_EB_HandleRxIPdu_Start */

#if (COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)

/** \brief Com_EB_HandleRxIPdu_EnableRxDM - starts RxDM of I-Pdu and signal based RxDM
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * - invoke only when Pdu is started.
 * \param[in] IPduPtr - pointer to the description of the Pdu
 * \param[in] IPduFlagBytePtr - Pointer to byte where the flags of the Pdu are
 * stored
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_HandleRxIPdu_EnableRxDM
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) IPduFlagBytePtr
)
{
#if (COM_RX_FIRST_TIMEOUT_FACTOR_SIZE != COM_SIZE_0_BIT)
   /* RxDM is switch on */
   COM_AtomicSetBit_8(IPduFlagBytePtr, COM_RXPDU_RXDMSWITCHON);
#endif /* (COM_RX_FIRST_TIMEOUT_FACTOR_SIZE != COM_SIZE_0_BIT) */

   /* Pdu based RxDM shall be started if
    * 1) Pdu based RxDM is configured and
    * 2) FirstTimeout != 0 and
    * 3) RxDM not started yet */

   if (COM_GET_COM_RX_IPDU_DM(IPduPtr) == COM_RX_DM_ON)
   {

      Com_RxDMNextDueTimeType const TimeOutOffset =

#if (COM_RX_FIRST_TIMEOUT_FACTOR_SIZE == COM_SIZE_0_BIT)
               (ComRxTimeoutFactorType)(IPduPtr->ComRxTimeoutFactorValue);
#else
               (ComRxFirstTimeoutFactorType) (IPduPtr->ComRxFirstTimeoutFactorValue);

      /* if ComRxFirstTimeoutFactor is zero start RxDM with first reception */
      if (0U != TimeOutOffset)
#endif /* (COM_RX_FIRST_TIMEOUT_FACTOR_SIZE == COM_SIZE_0_BIT) */
      {

         /* enter the critical section */
         Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

         /* do not re-start if already enabled  */
         if (!(Com_IsBitSet_8(IPduFlagBytePtr, COM_RXPDU_RXDMRUNNING)))
         {

            /* Set RXDM enabled flag for IPDU */
            Com_SetBitUInt8(IPduFlagBytePtr, COM_RXPDU_RXDMRUNNING);

            /* Start DM timer */
            Com_EB_RxDMQueue(IPduPtr, TimeOutOffset);

         }         /* if (RxPduDMEnabled == FALSE) */


         /* exit the critical section */
         Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

      }                /* if (0U != TimeOutOffset) */

   }                            /*    if (COM_GET_COM_RX_IPDU_DM(IPduPtr) == COM_RX_DM_ON) */


#if (COM_RX_SIGNAL_BASED_DM == STD_ON)
   /* if I-Pdu based DM is set up do so also for signal(group) based DM */
   Com_EB_EnableSignalBasedDMFlag(IPduPtr);

   Com_EB_EnableSignalGroupBasedDMFlag(IPduPtr);

#endif /* (COM_RX_SIGNAL_BASED_DM == STD_ON) */


   return;
}                               /* Com_EB_HandleRxIPdu_EnableRxDM */



/** \brief Com_EB_HandleRxIPdu_DisableRxDM - stops RxDM of I-Pdu and signal based RxDM
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * \param[in] IPduPtr - pointer to the description of the Pdu
 * \param[in] IPduFlagBytePtr - Pointer to byte where the flags of the Pdu are
 * stored
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_HandleRxIPdu_DisableRxDM
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) IPduFlagBytePtr
)
{
#if (COM_RX_FIRST_TIMEOUT_FACTOR_SIZE != COM_SIZE_0_BIT)
   /* clear first RxDM switch on flag, otherwise a interrupting
    * Com_RxIndication may start RxDM */
   COM_AtomicClearBit_8(IPduFlagBytePtr, COM_RXPDU_RXDMSWITCHON);
#endif /* (COM_RX_FIRST_TIMEOUT_FACTOR_SIZE != COM_SIZE_0_BIT) */

   /* clear RxDM-enabled flag for the IPdu */
   COM_AtomicClearBit_8(IPduFlagBytePtr, COM_RXPDU_RXDMRUNNING);

#if (COM_RX_SIGNAL_BASED_DM == STD_ON)
   /* disable signal(group) based DM */
   Com_EB_DisableSignalAndSignalGroupBasedDMFlag(IPduPtr, FALSE);
#else
   TS_PARAM_UNUSED(IPduPtr);
#endif /* (COM_RX_SIGNAL_BASED_DM == STD_ON) */
}                               /* Com_EB_HandleRxIPdu_DisableRxDM */

#endif /* (COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */



#if (COM_CBK_RX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE)
/** \brief Com_EB_HandleRxSig_W_F_A_A_Noti - handles
 * Rx signals with filter COM_F_ALWAYS, Notification but no signal invalidation
 * this function handles the Rx signals which have filters COM_F_ALWAYS and a
 * Notification callback configured but no signal invalidation configured.
 * Evaluates the update bit, DM, and calls the notification callback
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * - the filter has to be COM_F_ALWAYS
 * - a notification callback function has to be configured
 * - no invalid value has to be configured
 * \param[in] IPduPtr - pointer to the description of the signal
 * \param[in] PduId - ID of the I-PDU
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_HandleRxSig_W_F_A_A_Noti
(
      P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
      ComIPduRefType PduId
)
{
  ComSignalRefType i;              /* loop variable */
  ComSignalRefType loopStart;      /* variable for start value of loop */
  ComSignalRefType loopStop;       /* variable for stop value of loop */

#if ((COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) || (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_ON))
  uint32 const pduLength = COM_GET_RX_IPDU_SIZE(PduId, IPduPtr);
  uint8 const checkRequired = (IPduPtr->ComIPduSizeMax != pduLength);
#else
  TS_PARAM_UNUSED(PduId);
#endif

#if (COM_FILTER_RECEIVER_ENABLE == STD_ON)
  loopStart = IPduPtr->ComIPduSignalWithBufferRefNum;
#else
  loopStart = 0U;
#endif /* (COM_FILTER_RECEIVER_ENABLE == STD_ON) */

  loopStop = IPduPtr->ComIPduSignalW_F_A_NotiRefNum;

  /* Handle all which have no invalidation and COM_F_ALWAYS */
  for (i = loopStart; i < loopStop; i++)
  {
    /* Get signal id and pointer to config of the signal in the signal list of the IPdu */
    ComSignalRefType const signalId =
        (ComSignalRefType) *
        (COM_GET_CONFIG_ELEMENT
            (ComSignalRefType,
             (Com_gConfigPtr->ComRxIPduSignalListRef), (i + IPduPtr->ComIPduSignalRefFirst)));

    CONSTP2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr =
        COM_GET_CONFIG_ELEMENT(ComRxSignalType, Com_gConfigPtr->ComRxSignalRef, signalId);

#if ((COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) || (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_ON))

    uint8 passFilter_MASKED_NEW_DIFFERS_MASKED_OLD = 0U;
    uint8 const SignalUpdated = Com_EB_CheckRxSignal_Receive_UpdateBitAndDM(
        SignalPtr,
        IPduPtr,
        &passFilter_MASKED_NEW_DIFFERS_MASKED_OLD,
        pduLength,
        checkRequired
    );

    if (SignalUpdated == TRUE)
#endif /* ((COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) || (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_ON)) */
    {
      /* HINT this point should only be reached if no update bit is
            configured or a configured update bit is 1 */

      CONSTP2CONST(ComRxSignalExtType, AUTOMATIC, COM_APPL_CONST) SignalExtPtr =
          COM_GET_CONFIG_ELEMENT(ComRxSignalExtType,
                                 Com_gConfigPtr->ComRxSignalExtRef, SignalPtr->ComSignalExtRef);

      /* function pointer for ComNotification callbacks */
      P2VAR(Com_CbkRxAck_Type, AUTOMATIC, COM_APPL_CODE) fptr;

      fptr = Com_CbkRxAck_Array[(SignalExtPtr->ComNotification)];

      /* call the Rx-notification function of the signal */
      fptr();

    }                          /* if (SignalUpdated) */

  }                            /* for (i = loopStart; i < loopStop; i++) */

}                              /* Com_EB_HandleRxSig_W_F_A_A_Noti */
#endif /* (COM_CBK_RX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) */


#if (COM_SIGNALS_WITH_UB_RXDM_GW_ONLY == STD_ON)
/** \brief Com_EB_HandleRxSignalsDMUBOnly - handles all Rx signals within an
 * I-Pdu which have only a signal based DM and/or an update bit
 * this function handles all Rx signals Signals within an I-Pdu which have
 * filters with filter COM_F_ALWAYS or COM_F_NEVER, no notification, no signal
 * invalidation configured. Evaluates only the update bit and DM
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * - the filter has to be COM_F_ALWAYS or COM_F_NEVER (or no filter configured)
 * - a notification callback function has to be configured
 * - no invalid value has to be configured
 * \param[in] IPduPtr - pointer to the description of the signal
 * \param[in] PduId - ID of the I-PDU
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_HandleRxSignalsDMUBOnly
(
      P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
      ComIPduRefType PduId
)
{
  ComSignalRefType i;              /* loop variable */
  /* variable for start value of loop */
  ComSignalRefType loopStart = IPduPtr->ComIPduSignalRefFirst;
  /* variable for start value of loop */
  ComSignalRefType loopStop = IPduPtr->ComIPduSignalRefFirst;

  uint32 const pduLength = COM_GET_RX_IPDU_SIZE(PduId, IPduPtr);

  uint8 const checkRequired = (IPduPtr->ComIPduSizeMax != pduLength);

  /* it is only necessary to handle the signals with no DM configured in
   * case the SigGW is enabled */
#if (COM_SIGNAL_GW_ENABLE == STD_ON)
  loopStop = loopStop + IPduPtr->ComIPduSignalRefNum;
#else
  loopStop = loopStop + IPduPtr->ComIPduSignalWithDMOnly;
#endif /* (COM_SIGNAL_GW_ENABLE == STD_ON) */


#if (COM_CBK_RX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE)
  loopStart = loopStart + IPduPtr->ComIPduSignalW_F_A_NotiRefNum;

#else
  /* no notifications can be configured, therefore the loop starts either
   * with ComIPduSignalWithBufferRefNum if available or with 0 */
#if (COM_FILTER_RECEIVER_ENABLE == STD_ON)
  loopStart = loopStart + IPduPtr->ComIPduSignalWithBufferRefNum;
#else
  /* do not change loopStart */
#endif /* (COM_FILTER_RECEIVER_ENABLE == STD_ON) */
#endif /* (COM_CBK_RX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) */

  /* the loop stops always with ComIPduSignalWithDMOnly, otherwise the this
   * function is not called */

  /* Handle all signals with update bit */
  for (i = loopStart; i < loopStop; i++)
  {
    /* Get signal id and pointer to config of the signal in the signal list of the IPdu */
    ComSignalRefType const signalId =
        (ComSignalRefType) *
        (COM_GET_CONFIG_ELEMENT
            (ComSignalRefType, Com_gConfigPtr->ComRxIPduSignalListRef, i));

    CONSTP2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr =
        COM_GET_CONFIG_ELEMENT(ComRxSignalType, Com_gConfigPtr->ComRxSignalRef, signalId);

    /* check the update bit */
    uint8 passFilter_MASKED_NEW_DIFFERS_MASKED_OLD = 0U;
    (void) Com_EB_CheckRxSignal_Receive_UpdateBitAndDM(
        SignalPtr,
        IPduPtr,
        &passFilter_MASKED_NEW_DIFFERS_MASKED_OLD,
        pduLength,
        checkRequired
    );

  }                            /* for (i = loopStart; i < loopStop; i++) */

}                               /* Com_EB_HandleRxSignalsDMUBOnly */
#endif /* (COM_SIGNALS_WITH_UB_RXDM_GW_ONLY == STD_ON) */


#if (COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)

/** \brief Com_EB_RxDMQueue - "rewind" the RxDM timeout for an IPDU
 * This function starts a timeout for the Rx deadline monitoring of an IPDU
 * Preconditions:
 * - must only be invoked within a critical section
 * - prior to this call, the COM must be initialized.
 * \param[in] IPduPtr - reference to the I-Pdu configuration
 * \param[in] TimeOutOffset - value which shall be used for "rewind"
 */
TS_MOD_PRIV_DEFN FUNC(void, COM_CODE) Com_EB_RxDMQueue
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   Com_RxDMNextDueTimeType TimeOutOffset
)
{
   Com_RxDMNextDueTimeType globalTime;

   Com_RxDMNextDueTimeType TimeOut;

   /* Deviation MISRAC2012-1 */
   CONSTP2VAR(Com_RxDMNextDueTimeType, AUTOMATIC, COM_VAR_CLEARED) RxTimeoutQueueBase =
           COM_GET_MEMPTR(Com_RxDMNextDueTimeType, Com_gConfigPtr->ComRxTimeoutQueueBase);

   /* get global time */
   globalTime = (Com_RxDMNextDueTimeType) COM_GET_Com_GlobalRxTime(COM_GET_Com_MainFunctionRxIdx(IPduPtr));

   /* see other queue-functions above */
   TimeOut = (Com_RxDMNextDueTimeType) (globalTime + TimeOutOffset);
   RxTimeoutQueueBase[IPduPtr->RxDMQueueRef] = TimeOut;


   return;
}                               /* Com_EB_RxDMQueue */

#endif /* (COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */


/** \brief Com_EB_HandleRxIPdu - handles a Rx IPdu
 * function which manages starting/stopping a Rx I-Pdu and enabling/disabling
 * RxDM of Rx I-Pdu.
 * Preconditions:
 * - prior to this call, the COM module shall be initialized
 *
 * \param[in] mode - type of action which should be performed
 * (COM_IPDUGROUP_CONTROL, COM_IPDUGROUP_RXDM_CONTROL)
 * \param[in] toBeActivated - state of start/stop or activate/de-activate RxDM
 * \param[in] IPduPtr - configuration of the I-Pdu
 * \param[in] Initialize - defines if the Pdu group should be initialized
 * in case of starting of Pdus
 * \param[in] IPduId - Id of the I-Pdu
 */
TS_MOD_PRIV_DEFN FUNC(void, COM_CODE) Com_EB_HandleRxIPdu
(
   Com_IpduGroupModeType mode,
   boolean toBeActivated,
#if (COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
#endif /* (COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */
   boolean Initialize,
   ComIPduRefType IPduId
)
{

   /* pointer to byte with status flags of the Pdu */
   CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) IPduFlagBytePtr =
            &Com_gDataMemPtr[(Com_gConfigPtr->ComRxIPduStatusBase + IPduId)];


   if (!(Com_IsBitSet_8(IPduFlagBytePtr, COM_RXPDU_STARTED)))
   {
      /* Pdu is stopped, check if state change is required */
      if (TRUE == toBeActivated)
      {
         switch (mode)
         {
         case COM_IPDUGROUP_CONTROL:
         {
            /* start the Pdu */
            Com_EB_HandleRxIPdu_Start(IPduId, Initialize, IPduFlagBytePtr);
#if (COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)
            /* enable the RxDM of Pdu and signal based RxDM */
            Com_EB_HandleRxIPdu_EnableRxDM(IPduPtr, IPduFlagBytePtr);
#endif /* (COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */
         }
         break;
#if (COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)
         case COM_IPDUGROUP_RXDM_CONTROL:
         {
            /* nothing to do here, the I-Pdu is stopped, state shall not be changed */
         }
         break;
#endif /* (COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */
         /* CHECK: NOPARSE */
         default:
         {
            /* Should never be reached */
            COM_UNREACHABLE_CODE_ASSERT(COM_INTERNAL_API_ID);
         }
         break;
         /* CHECK: PARSE */
         }                            /* switch (mode) */

      }
      else
      {
         /* nothing to do here, the I-Pdu is stopped, state shall not be changed */
      }
   }
   else
   {
      /* Pdu is started, check if state change is required */
      if (FALSE == toBeActivated)
      {
         switch (mode)
         {
         case COM_IPDUGROUP_CONTROL:
         {
            /* stop the Pdu */
            /* clear started-flag for the IPdu */
            COM_AtomicClearBit_8(IPduFlagBytePtr, COM_RXPDU_STARTED);
#if (COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)
            /* disable RxDM */
            Com_EB_HandleRxIPdu_DisableRxDM(IPduPtr, IPduFlagBytePtr);
#endif /* (COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */
         }
         break;
#if (COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)
         case COM_IPDUGROUP_RXDM_CONTROL:
         {
            /* disable RxDM */
            Com_EB_HandleRxIPdu_DisableRxDM(IPduPtr, IPduFlagBytePtr);
         }
         break;
#endif /* (COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */
         /* CHECK: NOPARSE */
         default:
         {
            /* Should never be reached */
            COM_UNREACHABLE_CODE_ASSERT(COM_INTERNAL_API_ID);
         }
         break;
         /* CHECK: PARSE */
         }                            /* switch (mode) */
      }
#if (COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)
      else
      {
         /* Pdu is started, maybe RxDM has the be re-enabled */
         switch (mode)
         {
         case COM_IPDUGROUP_RXDM_CONTROL:
         {
            /* enable the RxDM of Pdu and signal based RxDM */
            Com_EB_HandleRxIPdu_EnableRxDM(IPduPtr, IPduFlagBytePtr);
         }
         break;
         case COM_IPDUGROUP_CONTROL:
         {
            /* no action required, leave state unchanged */
         }
         break;
         /* CHECK: NOPARSE */
         default:
         {
            /* Should never be reached */
            COM_UNREACHABLE_CODE_ASSERT(COM_INTERNAL_API_ID);
         }
         break;
         /* CHECK: PARSE */
         }                            /* switch (mode) */
      }
#endif /* (COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */

   }


   return;
}                               /* Com_EB_HandleRxIPdu */

#if ((COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) || (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_ON))

/** \brief Com_EB_CheckRxSignal_Receive_UpdateBitAndDM - handles update bit and DM of
 * Rx signals this function evaluates
 * - if the signal was received (handling of short PDUs)
 * - checks if update bit is configured and handles it
 * - checks if signal based DM is configured and handles it
 * \param[in] SignalPtr - pointer to configuration of signal
 * \param[in] IPduPtr - pointer to configuration of I-PDU
 * \param[in,out] passFilter_MASKED_NEW_DIFFERS_MASKED_OLD -
 *         in: indicates if the Pdu base RxDM has been just restarted
 *         out: If signal based RxDM has just been restarted after a time out, the value is set to 1.
                If signal based RxDM did not timed out before, value is set to 0.
                If no update bit is configured, value is not modified.
 * \param[in] pduLength - length of the Pdu
 * \param[in] checkRequired - shall be TRUE if it is necessary to check if the signal has been received
 * \return Function execution success status
 * \retval TRUE if the signal was updated, else FALSE
 */
_COM_STATIC_ FUNC(uint8, COM_CODE) Com_EB_CheckRxSignal_Receive_UpdateBitAndDM
(
    P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr,
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(uint8, AUTOMATIC, AUTOMATIC) passFilter_MASKED_NEW_DIFFERS_MASKED_OLD,
    uint32 pduLength,
    uint8 checkRequired
)
{
  uint8 SignalUpdated = TRUE;

#if (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_OFF)
  TS_PARAM_UNUSED(checkRequired);
#else

#if (COM_HANDLESMALLERRXPDUS_NOTIFY_TEST == STD_ON)
  TS_PARAM_UNUSED(checkRequired);
#else
  if (TRUE == checkRequired)
#endif
  {
    SignalUpdated = Com_EB_ReadIsReceived(SignalPtr, pduLength);
  }
#endif /* (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_OFF) */

#if (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL)
#if (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_ON)
  if (TRUE == SignalUpdated)
#endif
  {
    SignalUpdated = Com_EB_CheckRxSignalUpdateBitAndDM(
        SignalPtr,
        IPduPtr,
        passFilter_MASKED_NEW_DIFFERS_MASKED_OLD,
        pduLength
    );
  }
#else
  TS_PARAM_UNUSED(SignalPtr);
  TS_PARAM_UNUSED(IPduPtr);
  TS_PARAM_UNUSED(passFilter_MASKED_NEW_DIFFERS_MASKED_OLD);
  TS_PARAM_UNUSED(pduLength);
#endif /* (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) */

  return SignalUpdated;
}


_COM_STATIC_ FUNC(uint8, COM_CODE) Com_EB_CheckRxSignalGroup_Receive_UpdateBitAndDM
(
   P2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   uint32 pduLength
)
{
  uint8 SignalUpdated = TRUE;

#if (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_OFF)
  TS_PARAM_UNUSED(pduLength);
#else

  SignalUpdated = ((uint32)SignalGPtr->ComByteSize + SignalGPtr->ComSignalPositionInByte) <= pduLength;

  if (TRUE == SignalUpdated)
#endif /* (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_ON) */
  {
#if (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL)

    SignalUpdated = Com_EB_CheckRxSignalGoupUpdateBitAndDM(SignalGPtr, IPduPtr, pduLength);

#else
    TS_PARAM_UNUSED(SignalGPtr);
    TS_PARAM_UNUSED(IPduPtr);
#endif /* (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) */
  }

  return SignalUpdated;
}

#endif /* ((COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) || (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_ON)) */



#if (COM_FILTER_RECEIVER_ENABLE == STD_ON)

/** \brief Com_EB_HandleRxSignalsWithBuffer - handles Rx signals with buffers
 * this function handles the Rx signals which have filters (but not COM_F_NEVER
 * or COM_F_ALWAYS) or an invalid value. Evaluates the update bit and DM,
 * invalidates the signal, filters the value (after invalidation as defined in
 * SWS 3.0 -> Deviation in SWS 2.1) updates signal buffers and call the
 * callback functions (receive and invalid)
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * \param[in] IPduPtr - pointer to the description of the I-PDU
 * \param[in] PduId - ID of the I-PDU
 * \param[in] pduRxDMRestat - indicates if the Pdu base RxDM has just been restarted after a
 * time out
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_HandleRxSignalsWithBuffer
(
      P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
      ComIPduRefType PduId,
      uint8 pduRxDMRestat
)
{
  ComSignalRefType i;              /* loop variable */

#if ((COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) || (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_ON))
  uint32 const pduLength = COM_GET_RX_IPDU_SIZE(PduId, IPduPtr);
  uint8 const checkRequired = (IPduPtr->ComIPduSizeMax != pduLength);
#else
  TS_PARAM_UNUSED(PduId);
#endif

  /* Handle signals which have buffer (with filter != COM_F_ALWAYS/COM_F_NEVER and/or
   * invalid value) */
  for (i = 0; i < IPduPtr->ComIPduSignalWithBufferRefNum; i++)
  {
    uint8 filter = TRUE;        /* flag for notification */

    uint8 passFilter_MASKED_NEW_DIFFERS_MASKED_OLD = pduRxDMRestat;

    /* Get signal id and pointer to config of the signal in the signal list of the IPdu */
    ComSignalRefType const signalId =
        (ComSignalRefType) *
        (COM_GET_CONFIG_ELEMENT
            (ComSignalRefType,
             (Com_gConfigPtr->ComRxIPduSignalListRef), (i + IPduPtr->ComIPduSignalRefFirst)));

    CONSTP2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr =
        COM_GET_CONFIG_ELEMENT(ComRxSignalType, Com_gConfigPtr->ComRxSignalRef, signalId);

#if ((COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) || (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_ON))

    uint8 const SignalUpdated = Com_EB_CheckRxSignal_Receive_UpdateBitAndDM(
        SignalPtr,
        IPduPtr,
        &passFilter_MASKED_NEW_DIFFERS_MASKED_OLD,
        pduLength,
        checkRequired
    );

    if (TRUE == SignalUpdated)
#endif /* ((COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) || (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_ON)) */
    {

      CONSTP2CONST(ComRxSignalExtType, AUTOMATIC, COM_APPL_CONST) SignalExtPtr =
          COM_GET_CONFIG_ELEMENT(ComRxSignalExtType,
                                 Com_gConfigPtr->ComRxSignalExtRef, SignalPtr->ComSignalExtRef);
      TS_PARAM_UNUSED(SignalExtPtr);

      /* HINT this point should only be reached if no update bit is
       * configured or a configured update bit is 1 */
      {
        CONSTP2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr =
            &Com_gDataMemPtr[SignalPtr->ComSignalBase.ComBufferRef];

        CONSTP2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr =
            &SignalPtr->ComSignalBase;

        filter = Com_EB_CalculateFilter(Com_EB_ReadPropPtr,
                                        dataPtr, &Com_EB_ReadPropPtr->ComFilterConf);

#if (COM_RX_TIMEOUT_FACTOR_SIZE != COM_SIZE_0_BIT)
        /* always pass first value of signal with filter MASKED_NEW_DIFFERS_MASKED_OLD after
         * a reception deadline monitoring timeout occurred (SWS_Com_00793) */
        if (passFilter_MASKED_NEW_DIFFERS_MASKED_OLD == 1U)
        {
          /* Check if signal has DM (signal based or PDU based) */
          if (COM_GET_RX_SIGNAL_EXT_PDUSIGDM(SignalExtPtr) == COM_RX_DM_ON)
          {
            /* check if signal has a filter MASK_NEW_DIFFERS_MASK_OLD */
            if (SignalPtr->ComSignalBase.ComFilterConf.ComFilterType == COM_F_MASK_NEW_DIFFERS_MASK_OLD)
            {
              filter = TRUE;
              /* set the signal buffer with the value of the signal in the I-PDU buffer */
              Com_EB_SetBuffer(&SignalPtr->ComSignalBase);
            }
          }
        }
#else
        TS_PARAM_UNUSED(passFilter_MASKED_NEW_DIFFERS_MASKED_OLD);
#endif /* (COM_RX_TIMEOUT_FACTOR_SIZE != COM_SIZE_0_BIT) */
      }

#if (COM_SIGGW_RX_FILTER_ENABLE == STD_ON)
#if (COM_SIGNAL_GW_ENABLE == STD_ON)

      /* in case the signal shall be gated if
       * - the signal has a gateway relation
       * - the filter evaluates to TRUE */
      if ((filter == TRUE) && (SignalExtPtr->ComSignalSigGWFlagBit != COM_SIGNAL_INVALID))
      {
        /* set the SigGWFlag */
        COM_AtomicSetBit_8(COM_RX_SIGNAL_SIG_GW_FLAG_ADDRESS(SignalExtPtr),
                           COM_RX_SIGNAL_SIG_GW_FLAG_BIT(SignalExtPtr));
      }

#endif /* (COM_SIGNAL_GW_ENABLE == STD_ON) */
#endif /* (COM_SIGGW_RX_FILTER_ENABLE == STD_ON) */

#if (COM_CBK_RX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE)
      {
        /* if no invalid notifications have been called check if the
         * normal notifications should be called */

        /* check if notification is configured and call it */
        if (filter == TRUE)
        {
          if (SignalExtPtr->ComNotification != COM_CBK_RX_ACK_FUNC_PTR_NULL)
          {
            /* function pointer for ComNotification callbacks */
            P2VAR(Com_CbkRxAck_Type, AUTOMATIC, COM_APPL_CODE) fptr;

            fptr = Com_CbkRxAck_Array[(SignalExtPtr->ComNotification)];

            /* call the Rx-notification function of the signal */
            fptr();
          }
        }                /* check if notification is configured and call it */
      }
#endif /* (COM_CBK_RX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) */

    }                    /* if (SignalUpdated) */

  }                      /* for (i = 0; i < IPduPtr->ComIPduSignalWithBufferRefNum; i++) */

}                               /* Com_EB_HandleRxSignalsWithBuffer */
#endif /* (COM_FILTER_RECEIVER_ENABLE == STD_ON) */


#if (COM_NOTIFY_RECEIVERS_ENABLE == STD_ON)
/** \brief Com_EB_HandleRxSignalGroups - handles Rx signals groups
 * this function evaluates the update bit and DM,
 * invalidates the signal group calls callback of group signals and the
 * signal group
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * \param[in] IPduPtr - pointer to the description of the signal
 * \param[in] PduId - ID of the I-PDU
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_HandleRxSignalGroups
(
      P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
      ComIPduRefType PduId
)
{
  ComSignalGroupRefType i;              /* loop variable */

#if ((COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) || (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_ON))
  uint32 const pduLength = COM_GET_RX_IPDU_SIZE(PduId, IPduPtr);
#else
  TS_PARAM_UNUSED(PduId);
#endif

  /* Handle signals all signal groups within the I-Pdu */
  for (i = 0; i < IPduPtr->ComIPduSignalGroupRefNum; i++)
  {

    {
#if ((COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) || \
    (COM_CBK_RX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE))
      /* Get signal id and pointer to config of the signal in the signal list of the IPdu */
      const ComSignalGroupRefType signalGId =
          (ComSignalGroupRefType) *
          (COM_GET_CONFIG_ELEMENT
              (ComSignalGroupRefType,
               (Com_gConfigPtr->ComRxIPduSignalGroupListRef),
               (i + IPduPtr->ComIPduSignalGroupRefFirst)));

      /* reference to the configuration of a signal group */
      CONSTP2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr =
          COM_GET_CONFIG_ELEMENT(ComRxSignalGroupType,
                                 Com_gConfigPtr->ComRxSignalGroupRef, signalGId);
#endif /* (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL || \
        COM_CBK_RX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) */

#if ((COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) || (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_ON))

      uint8 const SignalUpdated = Com_EB_CheckRxSignalGroup_Receive_UpdateBitAndDM(
          SignalGPtr,
          IPduPtr,
          pduLength
      );

      if (SignalUpdated == TRUE)
#endif /* ((COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) || (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_ON)) */
      {
        /* HINT this point should only be reached if no update bit is
         * configured or a configured update bit is 1 */


#if (COM_CBK_RX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE)

        {
          /* check if notification is configured and call it */

          CONSTP2CONST(ComRxSignalExtType, AUTOMATIC, COM_APPL_CONST) SignalExtPtr =
              COM_GET_CONFIG_ELEMENT(ComRxSignalExtType,
                                     Com_gConfigPtr->ComRxSignalExtRef, SignalGPtr->ComSignalExtRef);

          if (SignalExtPtr->ComNotification != COM_CBK_RX_ACK_FUNC_PTR_NULL)
          {
            P2VAR(Com_CbkRxAck_Type, AUTOMATIC, COM_APPL_CODE) fptr;

            fptr = Com_CbkRxAck_Array[(SignalExtPtr->ComNotification)];

            /* call the Rx-notification function of the signal group */
            fptr();

          }                         /* check if notification is configured and call it */

        }
#endif /* (COM_CBK_RX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) */

      }                         /* group signal is updated */
    }
  }                            /* for (i = 0; i < IPduPtr->ComIPduSignalGroupRefNum; i++) */
}                               /* Com_EB_HandleRxSignalGroups */
#endif /* (COM_NOTIFY_RECEIVERS_ENABLE == STD_ON) */

#if (COM_FILTER_RECEIVER_ENABLE == STD_ON)
/** \brief Com_EB_InitRxSignalsWithFilter - initializes the signal buffers and
 * the RxSigGWFlag of all signals of the Rx-Pdus
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * \param[in] IPduPtr - pointer to the description of the signal
 */

_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_InitRxSignalsWithFilter
(
      P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
)
{
   ComSignalRefType j;


   /* initialize signals with buffer */
   for (j = 0; j < IPduPtr->ComIPduSignalWithBufferRefNum; j++)
   {

      /* Get signal id and pointer to config of the signal in the signal list of the IPdu */
      /* Id of signal which belongs to Pdu */
      ComSignalRefType signalId =
               (ComSignalRefType) * (COM_GET_CONFIG_ELEMENT(ComSignalRefType,
                        (Com_gConfigPtr->ComRxIPduSignalListRef),
                        (j + IPduPtr->ComIPduSignalRefFirst)));
      /* pointer to the configuration of the signal */
      P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr =
               COM_GET_CONFIG_ELEMENT(ComRxSignalType, Com_gConfigPtr->ComRxSignalRef, signalId);


      /* set the signal buffer with the value of the signal in the I-PDU buffer */
      Com_EB_SetBuffer(&SignalPtr->ComSignalBase);

      /* COM_UINT8_N (UINT64,SINT64) have no signal buffer even if they have a filter */

#if (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON)
      if (SignalPtr->ComSignalBase.ComFilterConf.ComFilterType == COM_F_ONE_EVERY_N)
      {
         /* reset the OneEveryN filter */
         Com_ResetFilerOneEveryN(SignalPtr->ComSignalBase.ComFilterConf.ComFilterRef);
      }
#endif /* (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON) */

#if (COM_SIGNAL_GW_ENABLE == STD_ON)
#if (COM_SIGGW_RX_FILTER_ENABLE == STD_ON)
      {

         CONSTP2CONST(ComRxSignalExtType, AUTOMATIC, COM_APPL_CONST) SignalExtPtr =
                  COM_GET_CONFIG_ELEMENT(ComRxSignalExtType,
                           Com_gConfigPtr->ComRxSignalExtRef, SignalPtr->ComSignalExtRef);

         /* initialize the flags for the SigGwRxFilter */
         if (SignalExtPtr->ComSignalSigGWFlagBit != COM_SIGNAL_INVALID)
         {
            /* reset ComSignalSigGWFlagBit */
            COM_AtomicClearBit_8(COM_RX_SIGNAL_SIG_GW_FLAG_ADDRESS(SignalExtPtr),
                     COM_RX_SIGNAL_SIG_GW_FLAG_BIT(SignalExtPtr));
         }
      }
#endif /* (COM_SIGGW_RX_FILTER_ENABLE == STD_ON) */
#endif /* (COM_SIGNAL_GW_ENABLE == STD_ON) */

   }                          /* for (j = 0; j < IPduPtr->ComIPduSignalWithBufferRefNum; j++) */

}                               /* Com_EB_InitRxSignalsWithFilter */
#endif /* (COM_FILTER_RECEIVER_ENABLE == STD_ON) */



/** \brief Com_EB_InitRxIPdu - initializes the specified Rx IPdu and the
 * Rx-Signals associated with it
 * NOTE does not clear the RAM buffers of ONE_EVERY_N filters
 * \param[in] RxPduId -IPdu which should be initialized
 * \return Function execution success status
 * \retval E_OK if init was successful
 * \retval E_NOT_OK if init failed (wrong configuration)
 */
TS_MOD_PRIV_DEFN FUNC(void, COM_CODE) Com_EB_InitRxIPdu
(
   ComIPduRefType RxPduId
)
{
   /* pointer to config structure of I-Pdu */
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr;

   /* get IPdu to which RxPduId identifies and check if the IPdu is valid */
   IPduPtr = COM_GET_CONFIG_ELEMENT(ComRxIPduType, Com_gConfigPtr->ComRxIPduRef, RxPduId);

#if (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON)
   if( TRUE == COM_GET_COM_RX_IPDU_DUMMY(IPduPtr) )
   {
     TS_AtomicAssign16( COM_RX_IPDU_SIZE_VAR(RxPduId), (ComIPduSizeType) 0U );
   }
   else
#endif /* (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) */
   {

      {
         /* a init value for the I-Pdu is available */

         /* set pointer to the first byte of the init value */
         CONSTP2CONST(uint8, AUTOMATIC, COM_APPL_CONST) valPtr =
                  COM_GET_CONFIG_ELEMENT(uint8, IPduPtr->ComInitValueRef, 0);

         /* copy PDU Init data to PDU buffer */
         TS_MemCpy(&Com_gDataMemPtr[IPduPtr->ComIPduValueRef], valPtr, IPduPtr->ComIPduSizeMax);

#if (COM_RX_TPAPI_ENABLE == STD_ON)

         /* ref to TP Control struct exists for this IPdu */
         if(IPduPtr->ComRxIpduTPControlInfoRef != COM_IPDU_ARRAY_SIZE_MAX)
         {

           /* get TP Control array of struct base */
           CONSTP2VAR(EcuC_RxControlInfoType, AUTOMATIC, COM_VAR_CLEARED) TpRxContPtrBase =
                 COM_GET_MEMPTR(EcuC_RxControlInfoType, Com_gConfigPtr->ComRxIpduTPControlInfoBase);

           /* get TP Control struct value pointer */
           CONSTP2VAR(EcuC_RxControlInfoType, AUTOMATIC, COM_VAR_CLEARED) TpRxContPtrValPtr =
                 &TpRxContPtrBase[IPduPtr->ComRxIpduTPControlInfoRef];

           /* init TP Control struct value via EcuC lib */
           EcuC_InitRx(TpRxContPtrValPtr);

         }

#endif /* (COM_RX_TPAPI_ENABLE == STD_ON) */

      }

#if (COM_RX_DYN_LENGTH_IPDU_ENABLE == STD_ON)
      /* Deviation MISRAC2012-1 */
      TS_AtomicAssign16( COM_RX_IPDU_SIZE_VAR(RxPduId), IPduPtr->ComIPduInitSize );
#endif /* (COM_RX_DYN_LENGTH_IPDU_ENABLE == STD_ON) */

   }

      /* HINT: there is no need to initialize signal group buffer here see note
       * of COM484: Since it is not suspected that a well-formed SWC tries to read
       * a group signal before a call to Com_ReceiveSignalGroup COM484 applies to
       * the sender side only.*/

      /* clear all flags of the IPdu */
      TS_AtomicAssign8(Com_gDataMemPtr[(Com_gConfigPtr->ComRxIPduStatusBase + RxPduId)], 0U);

#if (COM_FILTER_RECEIVER_ENABLE == STD_ON)
      Com_EB_InitRxSignalsWithFilter(IPduPtr);
#endif

#if (COM_SIGNAL_GW_ENABLE == STD_ON)
      /* reset ComIPduSigGWFlagBit  */
      if (IPduPtr->ComIPduSigGWFlagBit != COM_IPDU_INVALID)
      {
         /* clear the I-Pdu SigGWFlag */
         COM_AtomicClearBit_8(COM_RX_IPDU_SIG_GW_FLAG_ADDRESS(IPduPtr),
                  COM_RX_IPDU_SIG_GW_FLAG_BIT(IPduPtr));

#if (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON)
         {
           uint32 i;
           /* init source and destination status */
           P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) MultiGWStatusptr =
                                &Com_gDataMemPtr[(IPduPtr)->ComIPduMultiGWValueRef];
           for(i = 0U;i<((COM_MULTI_ROUTE_RX_DYN_LENGTH)+COM_MULTI_ROUTE_STATUS_LENGTH);i++)
           {
             MultiGWStatusptr[i] = 0U;
           }
         }
#endif /* (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON) */
      }

#endif /* (COM_SIGNAL_GW_ENABLE == STD_ON) */

#if (COM_RX_SIGNAL_BASED_DM == STD_ON)
      /* clear the flags for signal based DM */
      Com_EB_DisableSignalAndSignalGroupBasedDMFlag(IPduPtr, TRUE);
#endif /* (COM_RX_SIGNAL_BASED_DM == STD_ON) */

}                               /* Com_EB_InitRxIPdu */


/** \brief Com_GetRxIPduBuffer - returns Rx IPdu buffer reference
 * The service Com_GetRxIPduBuffer returns the buffer object identified by ComRxPduId
 * with the buffer referenced by the PduInfoPtr parameter.
 * Preconditions:
 * COM must be initialized.
 *
 * \param[in] ComRxPduId - ID of the Rx ComIPdu
 * \param[out] PduInfoPtr - Rx IPdu buffer reference
 * \return Function execution success status
 * \retval E_OK - success
 * \retval E_NOT_OK - failure (Com not initialized or
 * service failed due to development error)
 */
FUNC(uint8, COM_CODE) Com_GetRxIPduBuffer
(
   PduIdType ComRxPduId,
   P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr
)
{
  uint8 RetVal = E_NOT_OK;

  DBG_COM_GETRXIPDUBUFFER_ENTRY(ComRxPduId,PduInfoPtr);

  /* Debug output */
#if (COM_DEV_ERROR_DETECT == STD_ON)
  /* check whether Com is initialized */
  if (Com_InitStatus == COM_EB_UNINIT)
  {
    COM_DET_REPORTERROR(COM_E_UNINIT, COMServiceId_GetRxIPduBuffer);
  }
  /* check SignalDataPtr */
  else if (PduInfoPtr == NULL_PTR)
  {
    COM_DET_REPORTERROR(COM_E_PARAM_POINTER, COMServiceId_GetRxIPduBuffer);
  }
  /* check if ComRxPduId is valid */
  else if (Com_gConfigPtr->ComRxIPduArraySize <= ComRxPduId)
  {
    COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_GetRxIPduBuffer);
  }
  else

#endif /* (COM_DEV_ERROR_DETECT == STD_ON) */

  {

    /* pointer to the configuration of the IPdu to which the signal belongs */
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr;

    /* get IPdu to which ComRxPduId identifies and check if the IPdu is valid */
    IPduPtr = COM_GET_CONFIG_ELEMENT(ComRxIPduType, Com_gConfigPtr->ComRxIPduRef, ComRxPduId);

#if ( (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) && (COM_DEV_ERROR_DETECT == STD_ON) )
    if( TRUE == COM_GET_COM_RX_IPDU_DUMMY(IPduPtr) )
    {

      RetVal = E_NOT_OK;
      COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_GetRxIPduBuffer);

    }
    else
#endif /* ( (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) && (COM_DEV_ERROR_DETECT == STD_ON) ) */
    {

      /* get data pointer of I-PDU and set out parameter PduInfoPtr */
      PduInfoPtr->SduDataPtr = &Com_gDataMemPtr[(IPduPtr->ComIPduValueRef)];

      /* get max size of I-PDU and set out parameter PduInfoPtr */
      COM_AtomicAssign_PduLengthType(PduInfoPtr->SduLength, COM_GET_RX_IPDU_SIZE(ComRxPduId,IPduPtr));

      RetVal = E_OK;

    }

  }

 DBG_COM_GETRXIPDUBUFFER_EXIT(RetVal,ComRxPduId,PduInfoPtr);

  return RetVal;
}


#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
#endif


