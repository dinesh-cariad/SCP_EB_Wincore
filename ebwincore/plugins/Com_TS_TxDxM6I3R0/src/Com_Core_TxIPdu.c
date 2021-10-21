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
 * NOCOV-UNSUPPORTEDOPTIMIZATION:
 *
 * Reason:
 * A feature is not supported for this release and is inactivated with
 * pre-compile time switches (and therefore not included into the binary).
 *
 *
 */

/*==================[inclusions]============================================*/

#include <Com_Trace.h>
#include <TSAutosar.h>          /* EB specific standard types */
#include <ComStack_Types.h>     /* AUTOSAR Standard types */
#include <Com_Api.h>                /* Module public API */

#include <Com_Priv.h>           /* Module private API */

/* Com module does not require configuration of PduR, no need to re-build
 * Com module in case the post-build configuration of PduR changes */
#define PDUR_NO_PBCFG_REQUIRED
#include <PduR_Com.h>           /* For the PduR_ComTransmit-call */
#include <TSMem.h>              /* TS_MemCpy */

#include <Com_Core_TxIPdu.h>    /* Header TxIPdu functions */
#include <Com_Core_TxSignalGroup.h>    /* Header Tx signal group functions */

#if ((COM_TM_AVAILABLE == STD_ON) || (COM_TMS_ENABLE == STD_ON))
#include <Com_Core_TM.h> /* Transmission Mode functions */
#endif /* ((COM_TM_AVAILABLE == STD_ON) || (COM_TMS_ENABLE == STD_ON)) */

#if (COM_TX_TPAPI_ENABLE == STD_ON)
#include <EcuC.h>
#endif /* (COM_TX_TPAPI_ENABLE == STD_ON) */

/*==================[macros]=================================================*/

#if (defined COM_GET_PDUR_RETVAL)        /* To prevent double declaration */
#error COM_GET_PDUR_RETVAL is already defined
#endif /* (defined COM_GET_PDUR_RETVAL) */

#if (COM_EVALUATE_TRANSMIT_RET_VAL == STD_ON)
#define COM_GET_PDUR_RETVAL PduRRet =
#else
#define COM_GET_PDUR_RETVAL /* ignore return value */ (void)
#endif /* (COM_EVALUATE_TRANSMIT_RET_VAL == STD_ON) */

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_START_SEC_CODE
#include <Com_MemMap.h>
#endif

#if (COM_MDT_QUEUE_AVAILABLE == STD_ON)
/** \brief Com_EB_HandleMDT
 * starts MDT if configured
 * \param[in] PduId - Id of the IPdu the API call was called for
 * \param[in] IPduPtr - reference to configuration of Pdu
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_HandleMDT
(
   ComIPduRefType PduId,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
);
#endif /* (COM_MDT_QUEUE_AVAILABLE == STD_ON) */

/** \brief Com_EB_HandleTxIPdu_Start - starts a Tx IPdu
 * Preconditions:
 * - prior to this call, the COM module shall be initialized
 * \param[in] IPduPtr - configuration of the I-Pdu
 * \param[in] Initialize - defines if the Pdu group should be initialized
 * \param[in] IPduId - Id of the I-Pdu
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_HandleTxIPdu_Start
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   boolean Initialize,
   ComIPduRefType IPduId
);


/** \brief Com_EB_HandleTxIPdu_Stop - stops a Tx IPdu
 * Preconditions:
 * - prior to this call, the COM module shall be initialized
 * \param[in] IPduPtr - configuration of the I-Pdu
 * \param[in] IPduId - Id of the I-Pdu
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_HandleTxIPdu_Stop
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   ComIPduRefType IPduId
);

/** \brief Com_EB_InitTxIPdu_PduBuffer
 * function which initializes the IPdu-buffer of a Tx-IPdu
 * \param[in] IPduPtr pointer to the configuration of the Tx-IPdu
 * \param[in] TxPduId -IPdu which should be initialized
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_InitTxIPdu_PduBuffer
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   ComIPduRefType TxPduId
);

/** \brief Com_EB_InitTxIPdu_SignalGroups
 * function which initializes the signal groups of a Tx-IPdu
 * \param[in] IPduPtr pointer to the configuration of the Tx-IPdu
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_InitTxIPdu_SignalGroups
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
);


/** \brief Com_EB_InitTxIPdu_HandleFlags
 * function which initializes the flags of a Tx-IPdu
 * \param[in] IPduPtr pointer to the configuration of the Tx-IPdu
 * \param[in] TxPduId PduId to start
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_InitTxIPdu_HandleFlags
(
      P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
      ComIPduRefType TxPduId
);

#if (COM_TMS_ENABLE == STD_ON)

#if ((COM_TXF_MASKNEWDIFFERSMASKOLD_EN == STD_ON) || \
      (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON))

_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_InitTx_Signal
(
    ComSignalRefType signalId
);

/** \brief Com_EB_InitTxIPdu_Signals
 * function which initializes the signals of a Tx-IPdu
 * \param[in] IPduPtr pointer to the configuration of the Tx-IPdu
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_InitTxIPdu_Signals
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
);


#endif /* ((COM_TXF_MASKNEWDIFFERSMASKOLD_EN == STD_ON) || \
      (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON)) */

#endif /* (COM_TMS_ENABLE == STD_ON) */


/** \brief Com_EB_StartIPdu
 * function which manages starting a single IPdu
 * Preconditions:
 * - prior to this call, the COM module shall be initialized
 * - the Pdu must not be started before
 * \param[in] IPduPtr pointer to the configuration of the I-Pdu
 * \param[in] PduId PduId to start
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_StartIPdu
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   ComIPduRefType PduId
);


/** \brief Com_EB_Send_Pdu
 * Function which sends out the PDU (calls the I-Pdu callout if configured)
  * Preconditions:
 * - function does not check if MDT is running -> invoke only if MDT is not
 *   running or MDT shall be ignored.
 * \param[in] PduId - Id of the IPdu the API call was called for
 * \param[in] IPduPtr - reference to configuration of Pdu
 */
TS_MOD_PRIV_DEFN FUNC(void, COM_CODE) Com_EB_Send_Pdu
(
   ComIPduRefType PduId,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
)
{
   PduInfoType pduinfo;

   CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) pdudataptr =
      (P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED))
      & Com_gDataMemPtr[(IPduPtr->ComIPduValueRef)];


#if (COM_CALL_OUT_FUNC_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE)
   boolean coretval = TRUE;
   P2VAR(Com_TxCalloutType, AUTOMATIC, COM_APPL_CODE) fptr;
#else
   /* if no callout and MDT is possible, the PduId is not required */
#if (COM_MDT_QUEUE_AVAILABLE == STD_OFF)
   TS_PARAM_UNUSED(PduId);
#endif /* (COM_MDT_QUEUE_AVAILABLE == STD_ON) */
#endif /* (COM_CALL_OUT_FUNC_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) */


   pduinfo.SduDataPtr = pdudataptr;
   /* Deviation MISRAC2012-1 */
   COM_AtomicAssign_PduLengthType(pduinfo.SduLength, COM_GET_TX_IPDU_SIZE(PduId,IPduPtr));

#if (COM_CALL_OUT_FUNC_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE)
   if (IPduPtr->ComIPduCallOutFuncPtrRef != COM_CALL_OUT_FUNC_PTR_NULL)
   {
      /* get the function pointer */
      fptr = Com_TxCallouts[(IPduPtr->ComIPduCallOutFuncPtrRef)];
      {
         /* the function pointer seems to be OK, call it */
         coretval = fptr(PduId, &pduinfo);
      }
   }
   if (coretval == TRUE)
#endif /* (COM_CALL_OUT_FUNC_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) */
   {

#if (COM_EVALUATE_TRANSMIT_RET_VAL == STD_ON)
      Std_ReturnType PduRRet;
#endif /* (COM_EVALUATE_TRANSMIT_RET_VAL == STD_ON) */

      /* Tx request shall be issued now. In order to reduce possibility of race condition
       * we start MDT now and disable again when transmit shall be repeated again because of RFTR.
       * Note: it is still possible that MDT is violated if there are several trigger sources of
       * an I-Pdu (e.g Com_SendSignal(), Com_SendSignalGroup(), Com_SendSignalGroupArray
       * Com_TriggerIPDUSend() and
       * Com_MainFunctionTx(). During the time of checking that MDT is running
       * (outside this function) and starting of MDT below, several send requests may be issued
       * which violate MDT. However, changes are high that several rapidly Tx requests are
       * accumulated to one frame on the bus. */
#if (COM_MDT_QUEUE_AVAILABLE == STD_ON)
      Com_EB_HandleMDT(PduId, IPduPtr);
#endif /* (COM_MDT_QUEUE_AVAILABLE == STD_ON) */

      /* COM428: If not stated otherwise AUTOSAR COM will ignore all errors
       * from the underlying communication layer. Note: AUTOSAR COM supervises
       * the communication with deadline monitoring if configured. The specific
       * error codes from the underlying layer therefore can be ignored. In
       * case of update-bits this error codes are handled see COM062. */

#if (COM_TX_TPAPI_ENABLE == STD_ON)
      /* ref to TP Control struct exists for this IPdu */
      if(IPduPtr->ComTxIpduTPControlInfoRef != COM_IPDU_ARRAY_SIZE_MAX)
      {

        /* get TP Control array of struct base */
        CONSTP2VAR(EcuC_TxControlInfoType, AUTOMATIC, COM_VAR_CLEARED) TpTxContPtrBase =
              COM_GET_MEMPTR(EcuC_TxControlInfoType, Com_gConfigPtr->ComTxIpduTPControlInfoBase);

        /* get TP Control struct value pointer */
        CONSTP2VAR(EcuC_TxControlInfoType, AUTOMATIC, COM_VAR_CLEARED) TpTxContPtrValPtr =
              &TpTxContPtrBase[IPduPtr->ComTxIpduTPControlInfoRef];

        COM_GET_PDUR_RETVAL EcuC_TpTransmit(TpTxContPtrValPtr, (PduIdType)IPduPtr->PduRTxPduId,
            &pduinfo, (EcuC_LoTpTransmitFunctPtrType) &PduR_ComTpTransmit);

      }
      else
#endif /* (COM_TX_TPAPI_ENABLE == STD_ON) */
      {
         COM_GET_PDUR_RETVAL PduR_ComTransmit((PduIdType)IPduPtr->PduRTxPduId,&pduinfo);
      }

#if (COM_EVALUATE_TRANSMIT_RET_VAL == STD_ON)

      if (PduRRet == E_OK)
      {

         /* COM062: If the parameter ComTxIPduClearUpdateBit of an I-PDU is
          * configured to Transmit, the AUTOSAR COM module shall clear all
          * update-bits of all contained signals and signal groups after this
          * I-PDU was sent out via PduR_ComTransmit and PduR_ComTransmit returned
          * E_OK. */
         /* clear update bits of all signals within the I-Pdu */
#if (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL)
#if (!((COM_UPDATE_BIT_TX_CONFIG == COM_UPDATE_BIT_PRESENT_FOR_ALL) &&  \
   (COM_CL_UB_TX_TRIGGERTRANSMIT_ENABLE == STD_OFF) && \
   (COM_CL_UB_TX_TXCONFIRMATION_ENABLE == STD_OFF)))

         if (COM_GET_COM_TX_IPDU_CLEAR_UPDATE_BIT(IPduPtr) == COM_CL_UB_TRANSMIT)
#endif
         {
            Com_EB_ClearTxUpdateBits(IPduPtr);
         }
#endif /* (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) */

      }                         /* if (PduRRet == E_OK) */
#if (COM_RETRY_FAILED_TRANSMIT_REQ == STD_ON)
      else
      {                         /* (PduRRet != E_OK) */
         CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) PduStatusFlagsPtr =
            &Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + PduId)];

         COM_AtomicSetBit_8(PduStatusFlagsPtr, COM_TXPDU_SENDIMMEDIATELY);

#if (COM_MDT_QUEUE_AVAILABLE == STD_ON)
         /* Clear MDT since Tx request failed and shall be repeated within next
          * Com_MainFunctionTx() */
         COM_AtomicClearBit_8(PduStatusFlagsPtr, COM_TXPDU_MDTQUEUED);
#endif /* (COM_MDT_QUEUE_AVAILABLE == STD_ON) */

      }                         /* (PduRRet != E_OK) */
#endif /* (COM_RETRY_FAILED_TRANSMIT_REQ == STD_ON) */

#endif /* (COM_EVALUATE_TRANSMIT_RET_VAL == STD_ON) */

   }                            /* if (coretval == TRUE) */


   return;
}                               /* Com_EB_Send_Pdu */


#if (COM_MDT_QUEUE_AVAILABLE == STD_ON)
/** \brief Com_EB_HandleMDT
 * starts MDT if configured
 * \param[in] PduId - Id of the IPdu the API call was called for
 * \param[in] IPduPtr - reference to configuration of Pdu
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_HandleMDT
(
   ComIPduRefType PduId,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
)
{

   /* Get MDT */
   ComTxIPduMinimumDelayTimeFactorType const MDT = COM_GET_MDT(IPduPtr);

   if (MDT > 0)
   {
      Com_EB_MdtQueue(PduId, IPduPtr, MDT);
   }


   return;
}                               /* Com_EB_HandleMDT */
#endif /* (COM_MDT_QUEUE_AVAILABLE == STD_ON) */


/** \brief Com_EB_HandleTxIPdu_Start - starts a Tx IPdu
 * Preconditions:
 * - prior to this call, the COM module shall be initialized
 * \param[in] IPduPtr - configuration of the I-Pdu
 * \param[in] Initialize - defines if the Pdu group should be initialized
 * \param[in] IPduId - Id of the I-Pdu
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_HandleTxIPdu_Start
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   boolean Initialize,
   ComIPduRefType IPduId
)
{
   /* check if Pdu is started */
   if (!(Com_IsBitSet_8(&Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + IPduId)],
       COM_TXPDU_STARTED)))
   {

      if (Initialize == TRUE)
      {
         /* Re-initialize the IPdu (including shadow signal buffers of
          * signal groups)*/
         Com_EB_InitTxIPdu(IPduId);
      }                /* if (Initialize == TRUE) */

      Com_EB_StartIPdu(IPduPtr, IPduId);
   }                   /* if (BitRes == FALSE) */

}                               /* Com_EB_HandleTxIPdu_Start */


/** \brief Com_EB_HandleTxIPdu_Stop - stops a Tx IPdu
 * Preconditions:
 * - prior to this call, the COM module shall be initialized
 * \param[in] IPduPtr - configuration of the I-Pdu
 * \param[in] IPduId - Id of the I-Pdu
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_HandleTxIPdu_Stop
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   ComIPduRefType IPduId
)
{
   uint8 BitRes;

#if (COM_TM_AVAILABLE == STD_ON)
   uint8 AktTM;
#endif /* (COM_TM_AVAILABLE == STD_ON) */

   /* Stop group: Clear the started-flag for the IPDU */
   COM_AtomicClearBit_8(&Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + IPduId)],
                     COM_TXPDU_STARTED);

   /* Cancel sending of pending Pdus */
   COM_AtomicClearBit_8(&Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + IPduId)],
         COM_TXPDU_SENDIMMEDIATELY);

#if (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)
   /* cancel Tx deadline monitoring, if running */
   if ( Com_IsBitSet_8(&Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + IPduId)],
       COM_TXPDU_TXDMQUEUED) )
   {

      /* clear TxDM-queued-flag */
      COM_AtomicClearBit_8(&Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + IPduId)],
                        COM_TXPDU_TXDMQUEUED);

#if (COM_CBK_TX_ERR_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE)
      {

         Com_CbkTxTOutIdxType j;  /* iterator variable */
         /* Notify configured signals */
         for (j = IPduPtr->ComTxTimeoutNotificationFirst;
               j < (IPduPtr->ComTxTimeoutNotificationFirst
                     + IPduPtr->ComTxTimeoutNotificationNum); j++)
         {
            /* call callback */
            Com_CbkTxErr_Array[COM_GET_CBK_TX_ERROR_IDX(j)] ();
         }
      }
#endif /* #if (COM_CBK_TX_ERR_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) */

   }                            /* if ( Com_IsBitSet_8(&Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase 
                                        + IPduId)], COM_TXPDU_TXDMQUEUED)) */

#endif /* (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */


#if (COM_TM_AVAILABLE == STD_ON)
   /* If the IPdu is sent periodically, stop it */

   /* Get the actual transmission mode */
   /* Have a look if the reason for sending is a periodic event */
   TS_GetBit(&Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + IPduId)],
             COM_TXPDU_TMSSTATE, uint8, BitRes);

   /* get current TM */
   AktTM = COM_GET_COM_TX_MODE(BitRes, IPduPtr->ComTxModeModes);

   switch (AktTM)
   {
#if (COM_TX_MODE_DIRECT_ENABLE == STD_ON)
      case COM_TX_MODE_MODE_DIRECT:
#endif /* (COM_TX_MODE_DIRECT_ENABLE == STD_ON) */
         /* fall through */
      case COM_TX_MODE_MODE_NONE:
         break;

#if (COM_TX_MODE_N_TIMES_ENABLE == STD_ON)
      case COM_TX_MODE_MODE_N_TIMES:
#endif /* (COM_TX_MODE_N_TIMES_ENABLE == STD_ON) */
         /* fall through */
#if (COM_TX_MODE_MIXED_N_TIMES_ENABLE == STD_ON)
      case COM_TX_MODE_MODE_MIXED_N_TIMES:
#endif /* (COM_TX_MODE_MIXED_N_TIMES_ENABLE == STD_ON) */
         /* fall through */
#if (COM_TX_MODE_MIXED_DIRECT_ENABLE == STD_ON)
      case COM_TX_MODE_MODE_MIXED_DIRECT:
#endif /* COM_TX_MODE_MIXED_DIRECT_ENABLE */

#if (COM_TX_MODE_PERIODIC_ENABLE == STD_ON)
      case COM_TX_MODE_MODE_PERIODIC:
         /* fall through */
#endif /* (COM_TX_MODE_PERIODIC_ENABLE == STD_ON) */

#if (COM_PERIODIC_QUEUE_AVAILABLE == STD_ON)
         /* unqueue the IPdu by clearing
          * its Tx-queued flag for Periodic. */
         COM_AtomicClearBit_8(&Com_gDataMemPtr
                           [(Com_gConfigPtr->ComTxIPduStatusBase + IPduId)],
                           COM_TXPDU_PERIODICSENDQUEUED);
#endif /* (COM_PERIODIC_QUEUE_AVAILABLE == STD_ON) */

#if (COM_N_TIMES_QUEUE_AVAILABLE == STD_ON)
         /* unqueue the IPdu by clearing
          * its Tx-queued flag for NTImes. */
         COM_AtomicClearBit_8(&Com_gDataMemPtr
                           [(Com_gConfigPtr->ComTxIPduStatusBase + IPduId)],
                           COM_TXPDU_NTIMESSENDQUEUED);
#endif /* (COM_N_TIMES_QUEUE_AVAILABLE == STD_ON) */

#if ((COM_PERIODIC_QUEUE_AVAILABLE == STD_ON) || (COM_N_TIMES_QUEUE_AVAILABLE == STD_ON))
         break;
#endif
         /* CHECK: NOPARSE */
      default:
         COM_UNREACHABLE_CODE_ASSERT(COMServiceId_IpduGroupControl);
         break;
         /* CHECK: PARSE */

   }                   /* end switch (AktTM) */
#endif /* (COM_TM_AVAILABLE == STD_ON) */

   return;
}                               /* Com_EB_HandleTxIPdu_Stop */



/** \brief Com_EB_InitTxIPdu_PduBuffer
 * function which initializes the IPdu-buffer of a Tx-IPdu
 * \param[in] IPduPtr pointer to the configuration of the Tx-IPdu
 * \param[in] TxPduId -IPdu which should be initialized
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_InitTxIPdu_PduBuffer
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   ComIPduRefType TxPduId
)
{

#if (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON)
   if( TRUE == COM_GET_COM_TX_IPDU_DUMMY(IPduPtr) )
   {
     TS_AtomicAssign16(COM_TX_IPDU_SIZE_VAR(TxPduId), (ComIPduSizeType) 0U);
   }
   else
#endif /* (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) */
   {

     {

      /* a init value for the I-Pdu is available */
      /* pointer to first byte of init value */
      CONSTP2CONST(uint8, AUTOMATIC, COM_APPL_CONST) valPtr =
         COM_GET_CONFIG_ELEMENT(uint8, IPduPtr->ComInitValueRef, 0U);
      /* copy PDU Init data to PDU buffer */
      TS_MemCpy(&Com_gDataMemPtr[IPduPtr->ComIPduValueRef], valPtr, IPduPtr->ComIPduSizeMax);

     }

#if (COM_TX_DYN_LENGTH_IPDU_ENABLE == STD_ON)
   /* Deviation MISRAC2012-1 */
   TS_AtomicAssign16(COM_TX_IPDU_SIZE_VAR(TxPduId), IPduPtr->ComIPduInitSize);
#else
   TS_PARAM_UNUSED(TxPduId);
#endif /* (COM_TX_DYN_LENGTH_IPDU_ENABLE == STD_ON) */

   }

   return;
}                               /* Com_EB_InitTxIPdu_PduBuffer */

#if (COM_TMS_ENABLE == STD_ON)
/** \brief Com_EB_InitTxIPdu - clear/initializes TxSGTMSState flag byte
 * \param[in] SignalGId -SignalGroupID where the TxSGTMSState flag byte
 * should be initialized
 */
TS_MOD_PRIV_DEFN FUNC(void, COM_CODE) Com_EB_ClearTxSGTMSState
(
    ComIPduRefType SignalGId
)
{

  CONSTP2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr =
           COM_GET_CONFIG_ELEMENT(ComTxSignalGroupType, Com_gConfigPtr->ComTxSignalGroupRef,
                    SignalGId);

  /* get initial TMS flags of the groups signals */
  CONSTP2CONST(uint8, AUTOMATIC, COM_APPL_CONST) initFlagPtr
           = COM_GET_CONFIG_ELEMENT_CHECK(uint8, SignalGPtr->ComSignalGTMSFlagsInitValueRef, 0U);

  /* Flag byte Ptr for status byte of group signal */
  P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) SGFlagPtr =
                    COM_TXSG_TMS_MIGHT_CHANGE_FLAG_ADDRESS(SignalGId);

  /* clear hole TxSGTMSState flag byte instead of bit flags only */
  TS_AtomicAssign8(*SGFlagPtr, 0U);

  /* Hint: for each Tx signal group a TxSGTMSState byte is reserved and initialized
   * but only two bits are used */

  /* if there are flags available proceed with initialization */
  if (initFlagPtr != NULL_PTR)
  {
     ComSignalRefType length = ((SignalGPtr->ComGroupSignalWithFilterRefNum + 7U) / 8U);
     /* copy init values for TMS flags */
     TS_MemCpy(&Com_gDataMemPtr[(SignalGPtr->ComSignalGTMSFlagsRef)],
              initFlagPtr,
              length);


     {
        uint16 j;
        /* Set the COM_TXSG_TMSSTATE if TM of signal group is TRUE */
        for (j = 0U; j < length; j++)
        {
           if (Com_gDataMemPtr[(SignalGPtr->ComSignalGTMSFlagsRef + j)] > 0U)
           {
              COM_AtomicSetBit_8(COM_TXSG_TMS_MIGHT_CHANGE_FLAG_ADDRESS(SignalGId),
                       COM_TXSG_TMSSTATE);
           }
        }
     }

#if ((COM_TXF_MASKNEWDIFFERSMASKOLD_EN == STD_ON) || \
  (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON))

     {

        ComTxGroupSignalRefType GSigRef;

        /* loop over all group signals with filter */
        for(GSigRef = SignalGPtr->ComGroupSignalRefFirst;
                 GSigRef < (SignalGPtr->ComGroupSignalWithFilterRefNum + SignalGPtr->ComGroupSignalRefFirst);
                 GSigRef++)
        {

           /* Get signal id and pointer to config of the signal in the signal list of the IPdu */
           ComSignalRefType const GSignalId =  (ComSignalRefType) *
                    (COM_GET_CONFIG_ELEMENT (ComSignalRefType,
                             (Com_gConfigPtr->ComTxSignalGroupGroupSignalListRef), GSigRef));

           Com_EB_InitTx_Signal(GSignalId);

        } /* for(i = DestSignalGPtr->ComGroupSignalRefFirst;
  i < (DestSignalGPtr->ComGroupSignalWithFilterRefNum + DestSignalGPtr->ComGroupSignalRefFirst);
  i++) */
     }

#endif /* ((COM_TXF_MASKNEWDIFFERSMASKOLD_EN == STD_ON) || \
(COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON)) */

  }

}
#endif /* (COM_TMS_ENABLE == STD_ON) */

/** \brief Com_EB_InitTxIPdu_SignalGroups
 * function which initializes the signal groups of a Tx-IPdu
 * \param[in] IPduPtr pointer to the configuration of the Tx-IPdu
 * \return Function execution success status
 * \retval E_OK - success
 * \retval E_NOT_OK - in case of an config error
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_InitTxIPdu_SignalGroups
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
)
{
   ComSignalGroupRefType i;


   for (i = 0; i < IPduPtr->ComIPduSignalGroupRefNum; i++)
   {
      /* Get signal group id and pointer to config of the signal group in the
       * signal group list of the IPdu */

      /* Id of signal group which belongs to Pdu */
      ComSignalGroupRefType const SignalGId =
               (ComSignalGroupRefType) *
               (COM_GET_CONFIG_ELEMENT
                        (ComSignalGroupRefType,
                                 (Com_gConfigPtr->ComTxIPduSignalGroupListRef),
                                 (i + IPduPtr->ComIPduSignalGroupRefFirst)));

      CONSTP2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr =
               COM_GET_CONFIG_ELEMENT(ComTxSignalGroupType, Com_gConfigPtr->ComTxSignalGroupRef,
                        SignalGId);

#if (COM_BASED_TRANSFORMER_SUPPORT_TX == STD_ON)
      if (SignalGPtr->ComSignalGroupValueRef != COM_RAM_SIZE_MAX)
#endif /* (COM_BASED_TRANSFORMER_SUPPORT_TX == STD_ON) */
      {
         /* the signal group has a shadow buffer assigned which has to be initialized */

         CONSTP2VAR(uint8, AUTOMATIC, COMXF_APPL_DATA) buffer =
                  &Com_gDataMemPtr[(SignalGPtr->ComSignalGroupValueRef)];

         /* number of bytes which have been initialized is not required here */
         (void)Com_EB_InitSGBuffer(buffer,
                  SignalGPtr,
                  IPduPtr);
      }

#if (COM_TMS_ENABLE == STD_ON)
      {

        Com_EB_ClearTxSGTMSState(SignalGId);

      }
#endif /* (COM_TMS_ENABLE == STD_ON) */

   }                            /* for (i = 0; i < IPduPtr->ComIPduSignalGroupRefNum; i++) */
}



/** \brief Com_EB_InitTxIPdu_HandleFlags
 * function which initializes the flags of a Tx-IPdu
 * \param[in] IPduPtr pointer to the configuration of the Tx-IPdu
 * \param[in] PduId PduId to start
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_InitTxIPdu_HandleFlags
(
      P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
      ComIPduRefType TxPduId
)
{

   /* clear all flags of the IPdu */
   TS_AtomicAssign8(Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + TxPduId)], 0U);

   /* After this reset of all flags the Transmission Mode false is selected.
    * If there is a transmission mode true select it, otherwise leave it as is.
    * TM false is not invalid, so it MAY eventually be the transmission
    * mode which is selected after startup for the respective IPdu.
    * So we select this transmission mode here; if the startup TM is
    * going to be the TM true, the initial evaluation of the filters
    * will lead to the correct selection of TM true later on */

#if (COM_TM_AVAILABLE == STD_ON)
   /* If there is a TM true, TM true must always be selected - do it! */
   if (COM_GET_COM_TX_IPDU_INIT_TM(IPduPtr) == TRUE)
   {
      COM_AtomicSetBit_8(&Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + TxPduId)],
               COM_TXPDU_TMSSTATE);
   }
#endif /* (COM_TM_AVAILABLE == STD_ON) */

#if (COM_TMS_ENABLE == STD_ON)
   /* Set the ComIPduTMSFlags */
   {

      CONSTP2CONST(uint8, AUTOMATIC, COM_APPL_CONST) initFlagPtr
               = COM_GET_CONFIG_ELEMENT_CHECK(uint8, IPduPtr->ComIPduTMSFlagsInitValueRef, 0U);

      if (initFlagPtr != NULL_PTR)
      {

         /* calculate number of bytes which hold the TMS flags of the signals and
          * the signal groups */
         /* calculate number of bytes which hold the TMS flags of the signals and
          * the signal groups */
         uint16 const NumberOfFlagsBits =
                  IPduPtr->ComIPduSignalWithFilterRefNum
                  + IPduPtr->ComIPduSignalGroupWithFilterRefNum;

         uint16 const NumberOfFlagsBytes = (uint16)(NumberOfFlagsBits + 7U) / 8U;

         /* copy init values for TMS flags */
         TS_MemCpy(&Com_gDataMemPtr[(IPduPtr->ComIPduTMSFlagsRef)],
                  initFlagPtr,
                  NumberOfFlagsBytes);
      }
   }

#endif /* (COM_TMS_ENABLE == STD_ON) */

}                               /* Com_EB_InitTxIPdu_HandleFlags */

#if (COM_TMS_ENABLE == STD_ON)

#if ((COM_TXF_MASKNEWDIFFERSMASKOLD_EN == STD_ON) || \
      (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON))

_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_InitTx_Signal
(
    ComSignalRefType signalId
)
{
   CONSTP2CONST(ComTxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr =
            COM_GET_CONFIG_ELEMENT(ComTxSignalType, Com_gConfigPtr->ComTxSignalRef, signalId);


   {
      {

         /* check if the signal has the filter COM_F_MASK_NEW_DIFFERS_MASK_OLD */
         if (SignalPtr->ComSignalBase.ComFilterConf.ComFilterType == COM_F_MASK_NEW_DIFFERS_MASK_OLD)
         {

            /* ComFilterSignalValueRef has to be configured  */
            Com_EB_SetBuffer(&SignalPtr->ComSignalBase);
         }                /* if (SignalPtr->ComSignalBase.ComFilterConf.ComFilterType == COM_F_MASK_NEW_DIFFERS_MASK_OLD) */
         else
         {
            {

#if (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON)
               /* check if the signal has the filter COM_F_ONE_EVERY_N */
               if (SignalPtr->ComSignalBase.ComFilterConf.ComFilterType == COM_F_ONE_EVERY_N)
               {
                  /* reset the filter */
                  Com_ResetFilerOneEveryN(SignalPtr->ComSignalBase.ComFilterConf.ComFilterRef);
               }

#endif /* (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON) */
            }
         }
      }
   }
}


/** \brief Com_EB_InitTxIPdu_Signals
 * function which initializes the signals of a Tx-IPdu (filter)
 * \param[in] IPduPtr pointer to the configuration of the Tx-IPdu
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_InitTxIPdu_Signals
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
)
{

   ComSignalRefType i;

   for (i = 0; i < IPduPtr->ComIPduSignalWithFilterRefNum; i++)
   {
      /* Get signal id and pointer to config of the signal in the signal list of the IPdu */
      ComSignalRefType const signalId =
         (ComSignalRefType) *
         (COM_GET_CONFIG_ELEMENT
          (ComSignalRefType,
           (Com_gConfigPtr->ComTxIPduSignalListRef), (i + IPduPtr->ComIPduSignalRefFirst)));

      Com_EB_InitTx_Signal(signalId);

   }                           /* for (i = 0; i < IPduPtr->ComIPduSignalWithFilterRefNum; i++) */


}                               /* Com_EB_InitTxIPdu_Signals */


#endif /* ((COM_TXF_MASKNEWDIFFERSMASKOLD_EN == STD_ON) || \
      (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON)) */

#endif /* (COM_TMS_ENABLE == STD_ON) */



/** \brief Com_EB_StartIPdu
 * function which manages starting a single IPdu
 * Preconditions:
 * - prior to this call, the COM module shall be initialized
 * - the Pdu must not be started before
 * \param[in] IPduPtr pointer to the configuration of the I-Pdu
 * \param[in] PduId PduId to start
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_StartIPdu
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   ComIPduRefType PduId
)
{

#if (COM_TM_AVAILABLE == STD_ON)
   uint8 AktTM;                 /*  current Transmission Mode */
   uint8 BitRes;
#if ((COM_TX_MODE_MIXED_N_TIMES_ENABLE == STD_ON) || \
      (COM_TX_MODE_MIXED_DIRECT_ENABLE == STD_ON) || \
      (COM_TX_MODE_PERIODIC_ENABLE == STD_ON))
   ComTxModeIdxType TMRef;      /* reference to the TM configuration */
#endif
#endif /* (COM_TM_AVAILABLE == STD_ON) */

#if (COM_PERIODIC_QUEUE_AVAILABLE == STD_ON)
   Com_PeriodicSendNextDueTimeType TimeToSendPeriodic = 0;      /* for cyclic sends */
   Com_PeriodicSendNextDueTimeType TimeToSendFirst = 0; /* for first sends */
#endif /* (COM_PERIODIC_QUEUE_AVAILABLE == STD_ON) */

   CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) IPduFlagBytePtr =
         &Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + PduId)];



   /* enter the critical */
   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   /* start the IPdu via setting the started flag */
   Com_SetBitUInt8(IPduFlagBytePtr, COM_TXPDU_STARTED);

   /* clear the TMS change flag, because the flag might have been set while
    * I-Pdu group was stopped. If would not have been cleared in the next
    * Com_MainFunctionTx call the switch would be done again */
   Com_ClearBitUInt8(IPduFlagBytePtr, COM_TXPDU_TMSCHANGED);

#if (COM_TM_AVAILABLE == STD_ON)
   /* get current TM state */
   TS_GetBit(IPduFlagBytePtr, COM_TXPDU_TMSSTATE, uint8, BitRes);
#endif /* (COM_TM_AVAILABLE == STD_ON) */

   /* exit the critical section */
   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

#if (COM_TM_AVAILABLE == STD_ON)

   /* get current TM */
   AktTM = COM_GET_COM_TX_MODE(BitRes, IPduPtr->ComTxModeModes);

#if ((COM_TX_MODE_MIXED_N_TIMES_ENABLE == STD_ON) || \
      (COM_TX_MODE_MIXED_DIRECT_ENABLE == STD_ON) || \
      (COM_TX_MODE_PERIODIC_ENABLE == STD_ON))
   TMRef = COM_GET_COM_TX_MODE_REF(BitRes, IPduPtr);
#endif

#if (COM_PERIODIC_QUEUE_AVAILABLE == STD_ON)
   /* if the transmission mode contains periodic sending,
    * start periodic transmissions; see if it is required
    * to send immediately */

   switch (AktTM)
   {
#if (COM_TX_MODE_MIXED_N_TIMES_ENABLE == STD_ON)
      case COM_TX_MODE_MODE_MIXED_N_TIMES:
         {
            CONSTP2CONST(ComTxModeMixedNTimesType, AUTOMATIC, COM_APPL_CONST) TMMixedNTimesPtr =
                  COM_GET_CONFIG_ELEMENT(
                        ComTxModeMixedNTimesType,
                        Com_gConfigPtr->ComTxModeMixedNTimesRef,
                        TMRef
                        );
            TimeToSendFirst = COM_GET_TM_TIME_OFFSET_FACTOR(TMMixedNTimesPtr);
            TimeToSendPeriodic = TMMixedNTimesPtr->ComTxModeTimePeriodFactor;
         }
         break;
#endif /* (COM_TX_MODE_MIXED_N_TIMES_ENABLE == STD_ON) */

#if (COM_TX_MODE_PERIODIC_ENABLE == STD_ON)
      case COM_TX_MODE_MODE_PERIODIC:
#endif /* (COM_TX_MODE_PERIODIC_ENABLE == STD_ON) */

         /* fall through */
#if (COM_TX_MODE_MIXED_DIRECT_ENABLE == STD_ON)
      case COM_TX_MODE_MODE_MIXED_DIRECT:
#endif /* COM_TX_MODE_MIXED_DIRECT_ENABLE */

#if ((COM_TX_MODE_MIXED_DIRECT_ENABLE == STD_ON) || (COM_TX_MODE_PERIODIC_ENABLE == STD_ON))
         {
            CONSTP2CONST(ComTxModePeriodicType, AUTOMATIC, COM_APPL_CONST) TMPeriodicPtr =
                  COM_GET_CONFIG_ELEMENT(
                        ComTxModePeriodicType,
                        Com_gConfigPtr->ComTxModePeriodicRef,
                        TMRef
                        );
            TimeToSendFirst = COM_GET_TM_TIME_OFFSET_FACTOR(TMPeriodicPtr);
            TimeToSendPeriodic = TMPeriodicPtr->ComTxModeTimePeriodFactor;
         }
#endif /* ((COM_TX_MODE_MIXED_DIRECT_ENABLE == STD_ON) || (COM_TX_MODE_PERIODIC_ENABLE == STD_ON)) */
         break;
      default: /* default case intended to be empty */
         break;
   }                            /* switch (AktTM) */

#endif /* (COM_PERIODIC_QUEUE_AVAILABLE == STD_ON) */

   /* send immediately and schedule sending or only
    * schedule sending */
   switch (AktTM)
   {
#if (COM_TX_MODE_PERIODIC_ENABLE == STD_ON)
      case COM_TX_MODE_MODE_PERIODIC:
#endif /* (COM_TX_MODE_PERIODIC_ENABLE == STD_ON) */

         /* fall through */
#if (COM_TX_MODE_MIXED_DIRECT_ENABLE == STD_ON)
      case COM_TX_MODE_MODE_MIXED_DIRECT:
#endif /* COM_TX_MODE_MIXED_DIRECT_ENABLE */

         /* fall through */
#if (COM_TX_MODE_MIXED_N_TIMES_ENABLE == STD_ON)
      case COM_TX_MODE_MODE_MIXED_N_TIMES:
#endif /* (COM_TX_MODE_MIXED_N_TIMES_ENABLE == STD_ON) */

#if (COM_PERIODIC_QUEUE_AVAILABLE == STD_ON)
         {
            if (TimeToSendFirst == 0)
            {
               /* This means, the PDU must first be sent at
                * once and then in TimeToSendPeriodic cycle;
                * try sending at once */

               /* Since the Pdu cannot be started twice, MDT is not running */

               /* send the Pdu out */
               Com_EB_Send_Pdu(PduId, IPduPtr);


#if (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)

               /* Check if PDU has TxDM */
               if (COM_GET_COM_TX_IPDU_DM_AVAILABLE(IPduPtr) == COM_TX_DM_ON)
               {

                  /* Since the Pdu cannot be started twice, TxDM is not running */

                  /* Queue the Pdu */
                  Com_EB_TxDMQueue(PduId, COM_GET_TX_TIMEOUT_FACTOR(IPduPtr),
                                   IPduPtr->TxDMQueueRef);

               }                /* if (COM_GET_COM_TX_IPDU_DM_AVAILABLE(IPduPtr) == COM_TX_DM_ON) */
#endif /* (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */

            }
            else                /* if (TimeToSendFirst == 0) */
            {
               /* Sending the first time needs to be done
                * to some point in time later; let the "period"
                * being set for "next" sending be the time
                * to send first */
               TimeToSendPeriodic = TimeToSendFirst;
            }                   /* else from if (TimeToSendFirst == 0) */

            /* Schedule periodic sending */
            Com_EB_PeriodicSendQueue(TimeToSendPeriodic, IPduPtr->PeriodicQueueRef
#if (COM_TIMEBASE == STD_OFF)
                      , IPduPtr->ComMainFunctionsTx_Idx
#endif /* (COM_TIMEBASE == STD_OFF) */
                    );
            /* Mark the PDU as send queued */
            COM_AtomicSetBit_8(IPduFlagBytePtr, COM_TXPDU_PERIODICSENDQUEUED);

         }
         break;
#endif /* (COM_PERIODIC_QUEUE_AVAILABLE == STD_ON) */

      case COM_TX_MODE_MODE_NONE:
      {
#if (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)
         /* rewind TxDM if TxDM is configured and only TM None is configured */
         Com_EB_TxDM_TMNone(IPduPtr, PduId);
#endif /* (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */
      }
      break;
      default: /* default case intended to be empty */
         break;
   }                            /* switch (AktTM) */
#endif /* (COM_TM_AVAILABLE == STD_ON) */

   return;
}                               /* Com_EB_StartIPdu */



/** \brief Com_EB_HandleTxIPdu - handles a Tx IPdu
 * function which manages starting/stopping a Tx I-Pdu
 * Preconditions:
 * - prior to this call, the COM module shall be initialized
 *
 * \param[in] toBeActivated - state of start/stop
 * \param[in] IPduPtr - configuration of the I-Pdu
 * \param[in] Initialize - defines if the Pdu group should be initialized
 * in case of COM_START_GROUP
 * \param[in] IPduId - Id of the I-Pdu
 */
TS_MOD_PRIV_DEFN FUNC(void, COM_CODE) Com_EB_HandleTxIPdu
(
   boolean toBeActivated,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   boolean Initialize,
   ComIPduRefType IPduId
)
{




   if (TRUE == toBeActivated)
   {
      Com_EB_HandleTxIPdu_Start(IPduPtr, Initialize, IPduId);
   }
   else
   {
      Com_EB_HandleTxIPdu_Stop(IPduPtr, IPduId);
   }

}                               /* Com_EB_HandleTxIPdu */


/** \brief Com_EB_InitTxIPdu - initialize the specified Tx IPdu and the
 * shadow buffer if the signal groups belonging to the I-Pdu group.
 * NOTE does not clear the RAM buffers of ONE_EVERY_N filters
 * \param[in] TxPduId -IPdu which should be initialized
 */
TS_MOD_PRIV_DEFN FUNC(void, COM_CODE) Com_EB_InitTxIPdu
(
   ComIPduRefType TxPduId
)
{
   /* pointer to config structure of I-Pdu */
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr;


   /* get IPdu to which TxPduId identifies and check if the IPdu is valid */
   IPduPtr = COM_GET_CONFIG_ELEMENT(ComTxIPduType, Com_gConfigPtr->ComTxIPduRef, TxPduId);

   /* initialize the pdu buffer */
   Com_EB_InitTxIPdu_PduBuffer(IPduPtr, TxPduId);

   /* initialize signal groups of Pdu*/
   Com_EB_InitTxIPdu_SignalGroups(IPduPtr);

   /* Handle the flags */
   Com_EB_InitTxIPdu_HandleFlags(IPduPtr, TxPduId);

#if ((COM_TMS_ENABLE == STD_ON) &&                     \
    ((COM_TXF_MASKNEWDIFFERSMASKOLD_EN == STD_ON) ||  \
        (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON)))
   /* initialize the signals (filter) */
   Com_EB_InitTxIPdu_Signals(IPduPtr);
#endif /* (COM_TMS_ENABLE == STD_ON) && .... */

#if (COM_TX_TPAPI_ENABLE == STD_ON)

   /* ref to TP Control struct exists for this IPdu */
   if(IPduPtr->ComTxIpduTPControlInfoRef != COM_IPDU_ARRAY_SIZE_MAX)
   {

     /* get TP Control array of struct base */
     CONSTP2VAR(EcuC_TxControlInfoType, AUTOMATIC, COM_VAR_CLEARED) TpTxContPtrBase =
         COM_GET_MEMPTR(EcuC_TxControlInfoType, Com_gConfigPtr->ComTxIpduTPControlInfoBase);

     /* get TP Control struct value pointer */
     CONSTP2VAR(EcuC_TxControlInfoType, AUTOMATIC, COM_VAR_CLEARED) TpTxContPtrValPtr =
         &TpTxContPtrBase[IPduPtr->ComTxIpduTPControlInfoRef];

     /* init TP Control struct value via EcuC lib */
     EcuC_InitTx(TpTxContPtrValPtr);

   }

#endif /* (COM_TX_TPAPI_ENABLE == STD_ON) */

}                               /* Com_EB_InitTxIPdu */

#if (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)

/** \brief Com_EB_TxDMQueue - "rewind" the TxDM timeout for an IPDU
 * This function starts a timeout for the Tx deadline monitoring of an IPDU
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * \param[in] PduId ID of the Tx PDU to start deadline monitoring for
 * \param[in] Offset Time (in COM ticks) to pass by before sending
 * \param[in] EntryInTxDMcQueue position in of the PDU in TxDM Queue
 */
TS_MOD_PRIV_DEFN FUNC(void, COM_CODE) Com_EB_TxDMQueue
(
   ComIPduRefType PduId,
   ComTxTimeoutFactorType Offset,
   ComIPduRefType EntryInTxDMcQueue
)
{
   ComTxTimeoutFactorType TimeOut;
   ComTxTimeoutFactorType globalTime;

   /* Deviation MISRAC2012-1 <+2> */
   CONSTP2VAR(ComTxTimeoutFactorType, AUTOMATIC, COM_VAR_CLEARED) TxTimeoutQueueBase =
       COM_GET_MEMPTR(ComTxTimeoutFactorType, Com_gConfigPtr->ComTxTimeoutQueueBase);

   CONSTP2VAR(ComTxTimeoutFactorType, AUTOMATIC, COM_VAR_CLEARED) TxTimeoutQueueValPtr =
         &TxTimeoutQueueBase[EntryInTxDMcQueue];

#if (COM_TIMEBASE == STD_OFF)
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr =
         COM_GET_CONFIG_ELEMENT(ComTxIPduType, Com_gConfigPtr->ComTxIPduRef, PduId);
#endif /* (COM_TIMEBASE == STD_OFF) */

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   globalTime = (ComTxTimeoutFactorType) COM_GET_Com_GlobalTxTime(COM_GET_Com_MainFunctionTxIdx(IPduPtr));

   /* see other queue-functions above */
   TimeOut = (ComTxTimeoutFactorType) (globalTime + Offset);
   *TxTimeoutQueueValPtr = TimeOut;

   Com_SetBitUInt8(&Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + PduId)],
                   COM_TXPDU_TXDMQUEUED);

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

}                               /* Com_EB_TxDMQueue */

/** \brief Com_EB_TxDM_TMNone - "rewind" the TxDM timeout for an IPDU with
 * TM only one TM configured.
 * This function starts a timeout for the Tx deadline monitoring of an IPDU
 * for an I-Pdu which has TxDM configured and only one Transmission Mode.
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * - shall only be called in the current TM is TM_NONE
 * \param[in] IPduPtr pointer to the configuration of the PDU
 * \param[in] PduId ID of the Tx PDU to start deadline monitoring for
 */
TS_MOD_PRIV_DEFN FUNC(void, COM_CODE) Com_EB_TxDM_TMNone
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   ComIPduRefType PduId
)
{
   boolean TxDM = (COM_GET_COM_TX_IPDU_DM_AVAILABLE(IPduPtr) == COM_TX_DM_ON) ? TRUE: FALSE;


   /* rewind only if only TM None is configured (TM False is configured as TM_UNDEF) */
   if (TRUE == TxDM)
   {

#if (COM_TM_AVAILABLE == STD_ON)
      uint8 BitRes;
      uint8 TMOther;
      /* get current TM state */
      TS_GetBit( &Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + PduId)],
            COM_TXPDU_TMSSTATE, uint8, BitRes);

      TMOther = COM_GET_COM_TX_MODE_INV(BitRes, IPduPtr->ComTxModeModes);

      /* rewind only if only TM None is configured */
      if (COM_TX_MODE_MODE_UNDEF == TMOther)
#endif /* (COM_TM_AVAILABLE == STD_ON) */
      {
          Com_EB_TxDMQueue(PduId,
               COM_GET_TX_TIMEOUT_FACTOR(IPduPtr),
               IPduPtr->TxDMQueueRef);
      }
   }


   return;
}                               /* Com_EB_TxDM_TMNone */


#endif /* (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */


#if (COM_CBK_TX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE)
/** \brief Com_EB_TxConfirmations - calls all Tx notifications of the signals
 * which are within a PDU
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * \param[in] IPduPtr pointer to the configuration of the PDU
 */
TS_MOD_PRIV_DEFN FUNC(void, COM_CODE) Com_EB_TxConfirmations
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
)
{
   Com_CbkTxAckIdxType i;
   P2VAR(Com_CbkTxAck_Type, AUTOMATIC, COM_APPL_CODE) fptr;

   /* Call Tx confirmations */

   for (i = IPduPtr->ComNotificationFirst;
        i < (IPduPtr->ComNotificationFirst + IPduPtr->ComNotificationNum); i++)
   {

      /* get the function pointer */
      fptr = Com_CbkTxAck_Array[COM_GET_CBK_TX_ACK_PTR_IDX(i)];
      fptr();
   }
}                               /*Com_EB_TxConfirmations  */

#endif /* (COM_CBK_TX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) */



#if (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL)

/** \brief Com_EB_ClearTxUpdateBits - clears the update bit within the I-Pdu.
 * function which clears the update bit of the signals and signal groups
 * within the specified I-Pdu
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * - must not be called from any function but Com_TxConfirmation
 * - must only be called for IPdus which have MDT configured
 * \param[in] IPduPtr reference to the I-Pdu
 * or stopped (false)
 */
TS_MOD_PRIV_DEFN FUNC(void, COM_APPL_CODE) Com_EB_ClearTxUpdateBits
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
)
{

   ComSignalRefType i; /* iteration variable for signal list*/
   ComSignalGroupRefType j; /* iteration variable for signal group list*/


   for (i = IPduPtr->ComIPduSignalRefFirst;
   i < (IPduPtr->ComIPduSignalRefNum + IPduPtr->ComIPduSignalRefFirst); i++)
   {
      /* check if the signal has an update bit and clear it */
      /* possible optimization: */
      /* Use list with signals which have an update bit */
      /* generate bit masks which which can be used to clear bit(s) */

      /* get signal */
      ComSignalRefType const signalId =
         (ComSignalRefType) *
         (COM_GET_CONFIG_ELEMENT(
               ComSignalRefType, Com_gConfigPtr->ComTxIPduSignalListRef, i));

      CONSTP2CONST(ComTxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr =
         COM_GET_CONFIG_ELEMENT(ComTxSignalType, Com_gConfigPtr->ComTxSignalRef, signalId);

#if (COM_UPDATE_BIT_TX_CONFIG == COM_UPDATE_BIT_INDIVIDUAL)
      if (COM_GET_TX_SIGNAL_UPDATE_BIT(SignalPtr) == TRUE)
#endif /* (COM_UPDATE_BIT_TX_CONFIG == COM_UPDATE_BIT_INDIVIDUAL) */
      {

         COM_AtomicClearBit_8(
              &Com_gDataMemPtr[(IPduPtr->ComIPduValueRef +
                    ((ComRAMIdxType)SignalPtr->ComUpdateBitPositionInBit / 8U))],
               (SignalPtr->ComUpdateBitPositionInBit % 8U));

      }                         /* if (COM_GET_TX_SIGNAL_UPDATE_BIT(SignalPtr) == TRUE) */

   }                            /* for (i = IPduPtr->ComIPduSignalRefFirst;
   i < (IPduPtr->ComIPduSignalRefNum + IPduPtr->ComIPduSignalRefFirst); i++) */

   for (j = IPduPtr->ComIPduSignalGroupRefFirst;
   j < (IPduPtr->ComIPduSignalGroupRefNum + IPduPtr->ComIPduSignalGroupRefFirst); j++)
   {
      /* check if the signal group has an update bit and clear it */
      /* possible optimization: */
      /* Use list with signals which have an update bit */
      /* generate bit masks which which can be used to clear bit(s) */

      /* get signal */
      ComSignalGroupRefType const signalGId =
         (ComSignalGroupRefType) *
         (COM_GET_CONFIG_ELEMENT(ComSignalGroupRefType,
            Com_gConfigPtr->ComTxIPduSignalGroupListRef, j));

      CONSTP2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr =
         COM_GET_CONFIG_ELEMENT(ComTxSignalGroupType,
            Com_gConfigPtr->ComTxSignalGroupRef, signalGId);

#if (COM_UPDATE_BIT_TX_CONFIG == COM_UPDATE_BIT_INDIVIDUAL)
      if (COM_GET_TX_SIGNAL_GROUP_UPDATE_BIT(SignalGPtr) == TRUE)
#endif /* (COM_UPDATE_BIT_TX_CONFIG == COM_UPDATE_BIT_INDIVIDUAL) */
      {

         COM_AtomicClearBit_8(
              &Com_gDataMemPtr[(IPduPtr->ComIPduValueRef +
                    ((ComRAMIdxType)SignalGPtr->ComUpdateBitPositionInBit / 8U))],
               (SignalGPtr->ComUpdateBitPositionInBit % 8U));

      }                         /* if (COM_GET_TX_SIGNAL_GROUP_UPDATE_BIT(SignalGPtr) == TRUE) */

   }                            /* for (j = IPduPtr->ComIPduSignalGroupRefFirst;
   j < (IPduPtr->ComIPduSignalGroupRefNum + IPduPtr->ComIPduSignalGroupRefFirst); j++) */


   return;
} /* Com_EB_ClearTxUpdateBits */


#endif /* (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) */

#if (COM_MDT_QUEUE_AVAILABLE == STD_ON)

/** \brief Com_EB_MdtQueue - set a minimum delay time for a PDU.
 * function which blocks sending of a PDU for an amount of time to implement
 * the minimum delay time mechanism of the COM.
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * - must not be called from any function but Com_TxConfirmation
 * - must only be called for IPdus which have MDT configured
 * \param[in] PduId ID of the PDU to be blocked for its' minimum delay time
 * or stopped (false)
 */
TS_MOD_PRIV_DEFN FUNC(void, COM_CODE) Com_EB_MdtQueue
(
   ComIPduRefType Pdu,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   ComTxIPduMinimumDelayTimeFactorType MDT
)
{
   /* variable to hold global time in data type of ComTxIPduMinimumDelayTimeFactorType */
   ComTxIPduMinimumDelayTimeFactorType globalTime;
   ComTxIPduMinimumDelayTimeFactorType TimeOut; /* calculated point in time for unblocking */

   /* Deviation MISRAC2012-1 <+2> */
   CONSTP2VAR(ComTxIPduMinimumDelayTimeFactorType, AUTOMATIC, COM_VAR_CLEARED) MDTQueueBase =
       COM_GET_MEMPTR(ComTxIPduMinimumDelayTimeFactorType, Com_gConfigPtr->ComMDTQueueBase);

   CONSTP2VAR(ComTxIPduMinimumDelayTimeFactorType, AUTOMATIC, COM_VAR_CLEARED) MDTQueueValPtr =
         &MDTQueueBase[IPduPtr->MDTQueueRef];


   /* Enter critical section;
    * it must be made sure that the MainFunction is not called before
    * the due time has been added to the MDT "queue", as this could
    * block the PDU from being sent for a long time */
   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   globalTime = (ComTxIPduMinimumDelayTimeFactorType) COM_GET_Com_GlobalTxTime(COM_GET_Com_MainFunctionTxIdx(IPduPtr));

   /* calculate new timeout */
   TimeOut = (ComTxIPduMinimumDelayTimeFactorType) globalTime + MDT;
   /* set timeout */
   *MDTQueueValPtr = TimeOut;

   Com_SetBitUInt8(&Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + Pdu)],
                   COM_TXPDU_MDTQUEUED);

   /* Leave critical section */
   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();


   return;
}

#endif /* (COM_MDT_QUEUE_AVAILABLE == STD_ON) */

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
#endif


