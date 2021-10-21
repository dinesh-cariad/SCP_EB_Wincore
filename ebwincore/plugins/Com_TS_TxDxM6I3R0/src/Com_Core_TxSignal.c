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
#include <TSMem.h>              /* TS_MemCpy */

#include <Com_Core_TxSignal.h>  /* signal functions */

#include <Com_Core_Write.h>     /* serialization library */

#if ((COM_TM_AVAILABLE == STD_ON) || (COM_TMS_ENABLE == STD_ON))
#include <Com_Core_TM.h> /* Transmission Mode functions */
#endif /* ((COM_TM_AVAILABLE == STD_ON) || (COM_TMS_ENABLE == STD_ON)) */

#if (COM_TMS_ENABLE == STD_ON)
#include <Com_Filter.h>       /* Filter functions */
#include <Com_Core_TxSignalGroup.h>
#endif /* (COM_TMS_ENABLE == STD_ON) */

#if (COM_TX_TPAPI_ENABLE == STD_ON)
#include <EcuC.h>
#endif /* (COM_TX_TPAPI_ENABLE == STD_ON) */

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_START_SEC_CODE
#include <Com_MemMap.h>
#endif


#if (COM_TMS_ENABLE == STD_ON)

/** \brief Com_EB_SendSignal_Filter - performs filter for a Tx signal
 * Preconditions:
 * COM must be initialized.
 * \param[in] SignalPtr - reference to configuration of signal
 * \param[in] SignalDataPtr - place in memory to copy the data from (RAM)
 * \return Function execution success status
 * \retval TRUE - filter evaluated to TRUE
 * \retval FALSE - filter evaluated to FALSE
 */
_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_SendSignal_Filter
(
   P2CONST(ComTxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr,
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
);


/** \brief Com_EB_SendSignal_Filter - performs filter for a Tx signal
 * Preconditions:
 * COM must be initialized.
 * \param[in] SignalPtr - reference to configuration of signal
 * \param[in] SignalDataPtr - place in memory to copy the data from (RAM)
 * \return Function execution success status
 * \retval TRUE - filter evaluated to TRUE
 * \retval FALSE - filter evaluated to FALSE
 */
_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_SendSignal_Filter
(
   P2CONST(ComTxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr,
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
)
{
   boolean TMSResult = FALSE;

   /* signal type of the signal */
   Com_TS_SignalTypeType const SignalType = COM_GET_TX_SIGNAL_SIGNAL_TYPE(SignalPtr);

   CONSTP2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr =
            &SignalPtr->ComSignalBase;


   switch(SignalType)
   {

   case COM_BOOLEAN:
   {
      TMSResult = Com_EB_FilterBoolean(&Com_EB_ReadPropPtr->ComFilterConf,SignalDataPtr);
      break;
   }                            /* case COM_BOOLEAN: */

   case COM_UINT8:
   {
      TMSResult = Com_EB_FilterUint8(&Com_EB_ReadPropPtr->ComFilterConf,SignalDataPtr);
      break;
   }                            /* case COM_UINT8: */


   case COM_UINT16:
   {
      TMSResult = Com_EB_FilterUint16(&Com_EB_ReadPropPtr->ComFilterConf,SignalDataPtr);
      break;
   }                            /* case COM_UINT16: */

   case COM_UINT32:
   {
      TMSResult = Com_EB_FilterUint32(&Com_EB_ReadPropPtr->ComFilterConf,SignalDataPtr);
      break;
   }                            /* case COM_UINT32: */

   case COM_SINT8:
   {
      TMSResult = Com_EB_FilterSint8(&Com_EB_ReadPropPtr->ComFilterConf,SignalDataPtr);
      break;
   }                            /* case COM_SINT8: */

   case COM_SINT16:
   {
      TMSResult = Com_EB_FilterSint16(&Com_EB_ReadPropPtr->ComFilterConf,SignalDataPtr);
      break;
   }                            /* case COM_SINT16: */

   case COM_SINT32:
   {
      TMSResult = Com_EB_FilterSint32(&Com_EB_ReadPropPtr->ComFilterConf,SignalDataPtr);
      break;
   }                            /* case COM_SINT32: */

   case COM_SINT64:
   {
      TMSResult = Com_EB_FilterSint64(&Com_EB_ReadPropPtr->ComFilterConf, SignalDataPtr);
      break;
   }                            /* case COM_SINT64: */

   case COM_UINT64:
   {
      TMSResult = Com_EB_FilterUint64(&Com_EB_ReadPropPtr->ComFilterConf, SignalDataPtr);
      break;
   }                            /* case COM_UINT64: */

   /* CHECK: NOPARSE */
   default:
      COM_UNREACHABLE_CODE_ASSERT(COM_INTERNAL_API_ID);
      break;
      /* CHECK: PARSE */

   }                            /* switch(SignalType) */
   return TMSResult;

}                               /* Com_EB_SendSignal_Filter */

#endif /* (COM_TMS_ENABLE == STD_ON) */

/** \brief Com_EB_SendSignal - send a signal
 * The service Com_SendSignal updates the signal object identified by SignalId
 * with the signal referenced by the SignalDataPtr parameter.
 * Preconditions:
 * COM must be initialized.
 * \param[in] SignalPtr - reference to configuration of signal
 * \param[in] IPduPtr - reference to configuration of I-Pdu to which the
 * signal belongs
 * \param[in] PduId - ID of I-Pdu to which the signal belongs
 * \param[in] SignalDataPtr - place in memory to copy the data from (RAM)
 * \param[in] SignalInfoPtr - pointer to the length and length correction of the data
 * (NULL_PTR for fix length signals)
 * \param[in] SendInMainFunctionTx - if TRUE, the Pdu is sent not now but
 * during the next Com_MainfunctionTx(), otherwise immediately
 */

FUNC(void, COM_CODE) Com_EB_Write_SignalExt
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{

#if ( (COM_TMS_ENABLE == STD_ON) || (COM_TX_TPAPI_ENABLE == STD_ON) )
   /* Id of the IPdu to which the signal belongs */
   ComIPduRefType const PduId = Com_EB_WritePropPtr->ComIPduRef;

   /* pointer to the configuration of the IPdu to which the signal belongs */
   CONSTP2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr =
            COM_GET_CONFIG_ELEMENT(ComTxIPduType, Com_gConfigPtr->ComTxIPduRef, PduId);
#endif /* ( (COM_TMS_ENABLE == STD_ON) || (COM_TX_TPAPI_ENABLE == STD_ON) ) */

#if (COM_TX_TPAPI_ENABLE == STD_ON)
   boolean IsLocked;

   /* ref to TP Control struct exists for this IPdu */
   if( (IPduPtr->ComTxIpduTPControlInfoRef != COM_IPDU_ARRAY_SIZE_MAX) )
   {
      /* get TP Control array of struct base */
      CONSTP2VAR(EcuC_TxControlInfoType, AUTOMATIC, COM_VAR_CLEARED) TpTxContPtrBase =
               COM_GET_MEMPTR(EcuC_TxControlInfoType, Com_gConfigPtr->ComTxIpduTPControlInfoBase);

      /* get TP Control struct value pointer */
      CONSTP2VAR(EcuC_TxControlInfoType, AUTOMATIC, COM_VAR_CLEARED) TpTxContPtrValPtr =
               &TpTxContPtrBase[IPduPtr->ComTxIpduTPControlInfoRef];


      /* enter critical section */
      Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();
      {
         /* if no lock -> write */
         IsLocked = EcuC_TxBufferIsLocked(TpTxContPtrValPtr);
         if(IsLocked == FALSE)
         {
#if (COM_TX_ZERO_SIGNAL_ENABLE == STD_ON)
            if (IntDataPtr->arraySize > 0U)
#endif /* (COM_TX_ZERO_SIGNAL_ENABLE == STD_ON) */
               /* serialize data only if signal is not a zero signal */
            {
               /* Write the value to the buffer */
               Com_EB_Write_Core_NoLock(SignalDataPtr, dataPtr, Com_EB_WritePropPtr, IntDataPtr);
            }
         }
      }
      /* exit critical section */
      Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   }
   else
#endif /* (COM_TX_TPAPI_ENABLE == STD_ON) */
   {
#if (COM_TX_TPAPI_ENABLE == STD_ON)
      IsLocked = FALSE;
#endif /* (COM_TX_TPAPI_ENABLE == STD_ON) */
#if (COM_TX_ZERO_SIGNAL_ENABLE == STD_ON)
      if (IntDataPtr->arraySize > 0U)
#endif /* (COM_TX_ZERO_SIGNAL_ENABLE == STD_ON) */
         /* serialize data only if signal is not a zero signal */
      {
         /* serialize data only if signal is not a zero signal */
         /* Write the value to the buffer */
         Com_EB_Write_Core(SignalDataPtr, dataPtr, Com_EB_WritePropPtr, IntDataPtr);
      }
   }

#if (COM_TX_TPAPI_ENABLE == STD_ON)
   if(IsLocked == TRUE)
   {
      IntDataPtr->SendSigRetval = COM_BUSY;
   }
   else
#endif /* (COM_TX_TPAPI_ENABLE == STD_ON) */
   {

#if (COM_TMS_ENABLE == STD_ON)

      {
         CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) PduStatusFlagsPtr =
                  &Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + Com_EB_WritePropPtr->ComIPduRef)];

         if (TRUE == (COM_F_NON_TRIVIAL(Com_EB_WritePropPtr->ComSignalBase.ComFilterConf.ComFilterType)))
         {

            /* signal has non trivial filter */
            boolean const TMSResult = Com_EB_SendSignal_Filter(Com_EB_WritePropPtr, SignalDataPtr);

            if (Com_EB_WritePropPtr->ComSignalBase.ComSignalGroupRef != COM_SIGNAL_GROUP_INVALID)
            {
               /* signal is a group signal */

               ComSignalGroupRefType const SignalGId =
                        Com_EB_WritePropPtr->ComSignalBase.ComSignalGroupRef;

               CONSTP2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr =
                        COM_GET_CONFIG_ELEMENT(ComTxSignalGroupType,
                                 Com_gConfigPtr->ComTxSignalGroupRef, SignalGId);

               /* calculate the value of the TMS Flag of the group signal */
               Com_EB_CalculateGroupSignalTM(
                        Com_EB_WritePropPtr,
                        SignalGPtr,
                        TMSResult);
            }
            else
            {
               /* signal is a normal signal */

               /* get the old value of TMS of signal */
               CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) PduTMSFlagPtr =
                        (&Com_gDataMemPtr
                                 [(IPduPtr->ComIPduTMSFlagsRef) +
                                  ((ComRAMIdxType)Com_EB_WritePropPtr->ComPositionOfTMSFlag / 8U)]);

               boolean OldFilterResult;

               boolean CurrentTM;

               uint8 const SignalTMSFlag =
                        (uint8)((uint32)((uint32)(Com_EB_WritePropPtr->ComPositionOfTMSFlag) % 8U) );

               TS_GetBit(PduTMSFlagPtr, ((uint8) SignalTMSFlag), uint8,
                        OldFilterResult);

               if (OldFilterResult != TMSResult)
               {
                  /* the result of the filter changed */
                  boolean newTM;

                  uint8 PduStatusByte;

                  IntDataPtr->TMSChanged = TRUE;

                  /* store result of filter */
                  if (TMSResult == TRUE)
                  {
                     COM_AtomicSetBit_8(PduTMSFlagPtr, SignalTMSFlag);
                  }
                  else
                  {
                     COM_AtomicClearBit_8(PduTMSFlagPtr, SignalTMSFlag);
                  }


                  /* enter the critical section for calculation the new TM */
                  Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

                  /* calculate if the TM of the Pdu changed */
                  newTM = Com_EB_CalculateTM(IPduPtr);

                  /* get the status byte of the Pdu */
                  PduStatusByte = *PduStatusFlagsPtr;

                  /* get the current TMS status of Pdu */
                  TS_GetBit(&PduStatusByte, COM_TXPDU_TMSSTATE, uint8, CurrentTM);

                  if (newTM != CurrentTM)
                  {
                     /* change the TM of the I-Pdu */
                     if (newTM == TRUE)
                     {
                        Com_SetBitUInt8Var(PduStatusByte, COM_TXPDU_TMSSTATE);
                     }
                     else
                     {
                        Com_ClearBitUInt8Var(PduStatusByte, COM_TXPDU_TMSSTATE);
                     }

                     /* Set the changed flag of the I-Pdu */
                     Com_SetBitUInt8Var(PduStatusByte, COM_TXPDU_TMSCHANGED);

                     /* save the status byte again */
                     *PduStatusFlagsPtr = PduStatusByte;

                  }                /* if (newTM != CurrentTM) */

                  Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

                  /* COM582: If a change of the TMS causes a change to the
                   * transmission mode DIRECT, an immediate (respecting the MDT)
                   * direct/ n-times transmission to the underlying layer shall
                   * be initiated.
                   * -> shall also initiate transmission if signal has
                   * transfer property Pending */
                  if(newTM != CurrentTM)
                  {
                     uint8 const AktTM =
                              COM_GET_COM_TX_MODE(newTM, IPduPtr->ComTxModeModes);

                     if ((COM_TX_MODE_MODE_DIRECT == AktTM) || (COM_TX_MODE_MODE_N_TIMES == AktTM))
                     {
                        IntDataPtr->trigger = TRUE;
                     }

                  }

               }                   /* if (OldFilterResult != TMSResult) */
            }

         }                      /* if (TRUE == (COM_F_NON_TRIVIAL(Com_EB_WritePropPtr->ComFilterConf.ComFilterType))) */

      }
#endif /* (COM_TMS_ENABLE == STD_ON) */


#if (COM_SENDUPONAPICALL == STD_ON)

      if ( ! (Com_EB_WritePropPtr->ComSignalBase.ComSignalGroupRef != COM_SIGNAL_GROUP_INVALID))
      {

         /* transmit Pdu if required */
         Com_EB_SendSignal_Transmit(Com_EB_WritePropPtr,IntDataPtr);

      }

#endif /* (COM_SENDUPONAPICALL == STD_ON) */
   }

   return;
}                               /* Com_EB_Write_SignalExt */

#if (COM_SENDUPONAPICALL == STD_ON)

TS_MOD_PRIV_DEFN FUNC(void, COM_CODE) Com_EB_SendSignal_Transmit
(
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{


   /* Only do further processing if Com_SendSingal will return E_OK */
   if (IntDataPtr->SendSigRetval == E_OK)
   {


      boolean TmpTrigger = IntDataPtr->trigger;


#if (COM_TRANSF_PROP_TRIGGERED_ENABLE == STD_ON)
      if (COM_TRANSFER_PPROPERTY_TRIG == COM_GET_TX_SIGNAL_TRANSFER_PROPERTY(Com_EB_WritePropPtr))
      {
         if (COM_TRANSFER_PPROPERTY_SIG_TRIG_OC != COM_GET_TX_SIGNAL_TOC(Com_EB_WritePropPtr))
         {
            TmpTrigger = TRUE;
         }
      }
#endif /* (COM_TRANSF_PROP_TRIGGERED_ENABLE == STD_ON) */


      if (TRUE == TmpTrigger)
      {

#if (COM_TRANSF_PROP_WITHOUT_REP_ENABLE == STD_ON)

        boolean trig_wo_rep = (COM_TRANSFER_PPROPERTY_TRIG_WO_REP ==
            COM_GET_TX_SIGNAL_TRANSFER_PROPERTY_WO_REP(Com_EB_WritePropPtr));

        boolean tms_changed =
#if (COM_TMS_ENABLE == STD_ON)
            (IntDataPtr->TMSChanged == FALSE);
#else
            TRUE;
#endif

         if ( (trig_wo_rep) && (tms_changed) )
         {

            CONSTP2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr =
                     COM_GET_CONFIG_ELEMENT(ComTxIPduType, Com_gConfigPtr->ComTxIPduRef,
                              Com_EB_WritePropPtr->ComIPduRef);

#if ((COM_MDT_QUEUE_AVAILABLE == STD_ON) || (COM_SIGNAL_GW_ENABLE == STD_ON))
            CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) PduStatusFlagsPtr =
                     &Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase +
                              Com_EB_WritePropPtr->ComIPduRef)];
#endif

            Com_EB_Request_Send_Pdu(
                     Com_EB_WritePropPtr->ComIPduRef,
                     IPduPtr
#if ((COM_MDT_QUEUE_AVAILABLE == STD_ON) || (COM_SIGNAL_GW_ENABLE == STD_ON))
                     ,PduStatusFlagsPtr
#endif
#if ( (COM_SIGNAL_GW_ENABLE == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) )
                     ,IntDataPtr->SendInMainFunctionTx
#endif /* ( (COM_SIGNAL_GW_ENABLE == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) ) */
            );
         }
         else
#endif /* (COM_TRANSF_PROP_WITHOUT_REP_ENABLE == STD_ON) */
         {


            /* make a call to eventually have it sent according to transmission mode */
            Com_EB_SendUponSendApiCall(
                     Com_EB_WritePropPtr->ComIPduRef
#if ( (COM_SIGNAL_GW_ENABLE == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) )
                     ,IntDataPtr->SendInMainFunctionTx
#endif /* ( (COM_SIGNAL_GW_ENABLE == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) ) */
            );

         }

      }

   }


}                               /* Com_EB_SendSignal_Transmit */

#endif /* (COM_SENDUPONAPICALL == STD_ON) */

#if (COM_TX_DYN_LENGTH_IPDU_ENABLE == STD_ON)


/** \brief Com_EB_WriteDynSignal_ExitArea_0 - update the value and the
 *  length of a dynamic length ComIpdu and initiate a transmission.
 * Precondition:
 * - COM must be initialized
 * - Exclusive area 0 must be entered
 * \param[in] SignalDataPtr - reference to value of the source signal
 * \param[in] Com_EB_WritePropPtr - configuration of a signal
 * \param[in] IntDataPtr - pointer to internal data structures
 */
TS_MOD_PRIV_DEFN FUNC(void, COM_CODE) Com_EB_WriteDynSignal_ExitArea_0
(
   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{

   CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr =
            &Com_gDataMemPtr[Com_EB_WritePropPtr->ComSignalBase.ComBufferRef];

   ComSignalDescBitType const StartingByte = Com_EB_WritePropPtr->ComSignalPositionInBit;

   /* calculate length of Pdu */
   ComIPduSizeType const dynPduLength = (ComIPduSizeType) (StartingByte + IntDataPtr->arraySize);

#if (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON)
   if (COM_TRANSFER_PPROPERTY_SIG_TRIG_OC == COM_GET_TX_SIGNAL_TOC(Com_EB_WritePropPtr))
   {
      /* Deviation MISRAC2012-1 */
      if (COM_TX_IPDU_SIZE_VAR(Com_EB_WritePropPtr->ComIPduRef) != dynPduLength)
      {
         IntDataPtr->trigger = TRUE;
      }
   }
#endif /* (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) */

   /* set the new length of the Pdu */
   /* Deviation MISRAC2012-1 */
   COM_TX_IPDU_SIZE_VAR(Com_EB_WritePropPtr->ComIPduRef) = dynPduLength;

   /* set the value of the using the library, since we are in the critical section
    * additional lock is not required */
   Com_EB_Write_Core_NoLock(SignalDataPtr,dataPtr,Com_EB_WritePropPtr,IntDataPtr);

   /* exit critical section */
   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

#if (COM_SENDUPONAPICALL == STD_ON)

   /* transmit Pdu if required */
   Com_EB_SendSignal_Transmit(Com_EB_WritePropPtr,IntDataPtr);

#endif /* (COM_SENDUPONAPICALL == STD_ON) */

}

#endif /* (COM_TX_DYN_LENGTH_IPDU_ENABLE == STD_ON) */

TS_MOD_PRIV_DEFN FUNC(uint8, COM_CODE) Com_GetSendSignalRetVal
(
   P2CONST(ComTxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr
)
{

   uint8 RetVal = COM_SERVICE_NOT_AVAILABLE;

   CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) PduStatusFlagsPtr =
            &Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + SignalPtr->ComIPduRef)];

   /* Check if Pdu is started and only do further processing if the IPdu is started */
   if ( Com_IsBitSet_8(PduStatusFlagsPtr, COM_TXPDU_STARTED) )
   {
      RetVal = E_OK;
   }

   return RetVal;
}


#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
#endif

