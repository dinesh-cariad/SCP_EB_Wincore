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

/*==================[inclusions]============================================*/

#include <Com_Trace.h>
#include <TSAutosar.h>          /* EB specific standard types */
#include <ComStack_Types.h>     /* AUTOSAR Standard types */
#include <Com_Api.h>            /* Module public API */
#include <Com_Priv.h>           /* Module private API */
#include <TSMem.h>              /* TS_MemCpy */
#include <Com_Core_TxSignalGroup.h>  /* (group)signal(group) functions */


#if ((COM_TM_AVAILABLE == STD_ON) || (COM_TMS_ENABLE == STD_ON))
#include <Com_Core_TM.h>        /* Transmission Mode functions */
#endif /* ((COM_TM_AVAILABLE == STD_ON) || (COM_TMS_ENABLE == STD_ON)) */

#if (COM_TMS_ENABLE == STD_ON)
#include <Com_Filter.h>          /* Filter functions */
#endif /* (COM_TMS_ENABLE == STD_ON) */

#if (COM_TX_TPAPI_ENABLE == STD_ON)
#include <EcuC.h>
#endif /* (COM_TX_TPAPI_ENABLE == STD_ON) */


#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_START_SEC_CODE
#include <Com_MemMap.h>
#endif


#if (COM_TMS_ENABLE == STD_ON)
/** \brief Com_EB_SetGroupSignalTMSFlagOfSignalGroup - calculates the TMS of a
 * signal group
 * This function Com_EB_SetGroupSignalTMSFlagOfSignalGroup loops over all group
 * signals of the signal group and calculates the TMS of the signal group
 * Preconditions:
 * - COM must be initialized
 * \param[in] SignalGPtr - reference to the configuration of signal group
 * \param[in] SGArrayPtr - reference to the signal group array
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_SetGroupSignalTMSFlagOfSignalGroup
(
   P2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) SGArrayPtr
);
#endif /* (COM_TMS_ENABLE == STD_ON) */



/** \brief Com_EB_CalculateSignalGroupTM - It evaluates the TMS flags of the
 * group signals and calculates the TMS for the signal group and
 * initiates sending of the I-Pdu if necessary
 * Preconditions:
 * - COM must be initialized.
 * \param[in] SignalGPtr - reference to configuration of signal group
 * \param[in] PduId - id of I-Pdu to which the signal group belongs
 * \param[in] IPduPtr - reference to the configuration of I-Pdu
 * \param[in] SendInMainFunctionTx - if TRUE, the Pdu is sent not now but
 * during the next Com_MainfunctionTx(), otherwise immediately
 * \param[in] forceTrigger - if TRUE the transmission is triggered
 * \return Function execution success status
 * \retval E_OK if signal group could be copied to the I-Pdu
 * \retval COM_SERVICE_NOT_AVAILABLE if corresponding I-PDU group was stopped
 * or development error occurred
 */
_COM_STATIC_ FUNC(uint8, COM_CODE) Com_EB_CalculateSignalGroupTM
(
    boolean forceTrigger
#if ((COM_TMS_ENABLE == STD_ON) || (COM_TRANSF_PROP_WITHOUT_REP_ENABLE == STD_ON))
   ,P2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr
   ,P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
#endif
   ,ComIPduRefType PduId
#if ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) )
   ,boolean SendInMainFunctionTx
#endif /* ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) ) */
);

/** \brief Com_EB_WriteShadowBuffer_Fragment - writes fragmented shadow buffer
 * to the I-Pdu
 * The service Com_EB_WriteShadowBuffer_Fragment copies the value of fragmented signal group
 * into the I-Pdu
 * Preconditions:
 * COM must be initialized.
 * *forceTriggerPtr shall be set to TRUE if the configuration or previous TOC calculations
 * indicate that the Pdu shall be triggered; else value shall be FALSE
 * \param[in] PduValPtr - reference to the value of the Pdu
 * \param[in] SGArrayValPtr - reference to the value in a signal group array
 * \param[in] WriteMaskPtr - reference the mask which defines the relevant bits of the signal group
 * \param[in] TOCMaskPtr - reference the mask which defines the relevant bits
 * for the TRIGGERED ON CHANGE calculation
 * \param[in,out] forceTriggerPtr - if the value changed and it is relevant for the
 * TRIGGERED ON CHANGE calculation, then the value is set to TRUE, else the value remains.
 * \param[in] offset offset for SGArrayValPtr and TOCMaskPtr
 * \param[in] ByteSize number of bytes of the signal group
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_WriteShadowBuffer_Fragment
(
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) PduValPtr,
   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) SGArrayValPtr,
   P2CONST(uint8, AUTOMATIC, COM_APPL_CONST) WriteMaskPtr,
#if (COM_EVALUATE_TRIGGERED_OC == STD_ON)
   P2CONST(uint8, AUTOMATIC, COM_APPL_CONST) TOCMaskPtr,
   P2VAR(boolean, AUTOMATIC, AUTOMATIC) forceTriggerPtr,
#endif
   ComSignalDescByteType ByteSize
);

/** \brief Com_EB_WriteShadowBuffer - writes the shadow buffer to the I-Pdu
 * The service Com_EB_WriteShadowBuffer copies the value of the signal group
 * into the I-Pdu
 * Preconditions:
 * COM must be initialized.
 * \param[in] SignalGPtr - reference to configuration of the signal group
 * \param[in] IPduPtr - reference to configuration of the I-Pdu to which the
 * \param[in] SGArrayPtr - reference to the signal group array
 * Signal Group is stored
 * signal group belongs
 * \param[out] TpPduBuffer_IsLocked - returns Tp buffer lock state
 * \return Function execution success status
 * \retval TRUE if the value has changed, else FALSE
 */
_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_WriteShadowBuffer
(
   P2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) SGArrayPtr
#if (COM_TX_TPAPI_ENABLE == STD_ON)
   , P2VAR(boolean, AUTOMATIC, COM_APPL_DATA) TpPduBuffer_IsLocked
#endif /* (COM_TX_TPAPI_ENABLE == STD_ON) */
);


/** \brief Com_EB_SendSignalGroup - internal function for sending a signal group
 * The service Com_EB_SendSignalGroup sends a signal group. It evaluates the
 * TMS flags of the group signals and calculates the TMS for the signal group
 * Preconditions:
 * - COM must be initialized.
 * \param[in] SGArrayPtr - not used
 * \param[in] SGArrayPtrLength - not used
 * \param[in] SignalGPtr - reference to configuration of signal group
 * \param[in] PduId - id of I-Pdu to which the signal group belongs
 * \param[in] IPduPtr - reference to the configuration of I-Pdu
 * \param[in] SendInMainFunctionTx - if TRUE, the Pdu is sent not now but
 * during the next Com_MainfunctionTx(), otherwise immediately
 * \return Function execution success status
 * \retval E_OK if signal group could be copied to the I-Pdu
 * \retval COM_SERVICE_NOT_AVAILABLE if corresponding I-PDU group was stopped
 * \retval COM_BUSY - The receive Tp buffer is actually not available
 * (or service failed due to development error)
 */
TS_MOD_PRIV_DEFN FUNC(uint8, COM_CODE) Com_EB_SendSignalGroup
(
   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) SGArrayPtr,
   uint16 SGArrayPtrLength,
   P2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   ComIPduRefType PduId,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
#if ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) )
   ,boolean SendInMainFunctionTx
#endif /* ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) ) */
)
{

   const ComRAMIdxType ComSignalGroupValueRef = SignalGPtr->ComSignalGroupValueRef;

   uint8 retval = COM_SERVICE_NOT_AVAILABLE;


   TS_PARAM_UNUSED(SGArrayPtr);
   TS_PARAM_UNUSED(SGArrayPtrLength);

#if (COM_DEV_ERROR_DETECT == STD_ON)
#if (COM_BASED_TRANSFORMER_SUPPORT_TX == STD_ON)
   if (ComSignalGroupValueRef == COM_RAM_SIZE_MAX)
   {
      /* the signal group has no shadow buffer assigned an shall
       * only be used with the Com_SendSignalGroupArray() */
      COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_SendSignalGroup);
   }
   else
#endif /* (COM_BASED_TRANSFORMER_SUPPORT_TX == STD_ON) */
#endif /* (COM_DEV_ERROR_DETECT == STD_ON) */
   {

#if (COM_TX_TPAPI_ENABLE == STD_ON)
     boolean TpPduBuffer_IsLocked = FALSE;
#endif /* (COM_TX_TPAPI_ENABLE == STD_ON) */

     boolean forceTrigger =
         Com_EB_WriteShadowBuffer(SignalGPtr, IPduPtr, &Com_gDataMemPtr[(ComSignalGroupValueRef)]
#if (COM_TX_TPAPI_ENABLE == STD_ON)
            , &TpPduBuffer_IsLocked
#endif /* (COM_TX_TPAPI_ENABLE == STD_ON) */
         );

#if (COM_TX_TPAPI_ENABLE == STD_ON)
     if(TRUE == TpPduBuffer_IsLocked)
     {
       retval = COM_BUSY;
     }
     else
#endif /* (COM_TX_TPAPI_ENABLE == STD_ON) */
     {
      retval = Com_EB_CalculateSignalGroupTM(
          forceTrigger
#if ((COM_TMS_ENABLE == STD_ON) || (COM_TRANSF_PROP_WITHOUT_REP_ENABLE == STD_ON))
          ,SignalGPtr
          ,IPduPtr
#endif
          ,PduId
#if ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) )
          ,SendInMainFunctionTx
#endif /* ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) ) */
      );

     }

   }

   return retval;
}                               /* Com_EB_SendSignalGroup */

#if (COM_BASED_TRANSFORMER_SUPPORT_TX == STD_ON)

TS_MOD_PRIV_DEFN FUNC(uint8, COM_CODE) Com_EB_SendSignalGroupArray_DETCheck
(
   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) SGArrayPtr,
   uint16 SGArrayLength,
   P2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   ComIPduRefType PduId,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
#if ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) )
   ,boolean SendInMainFunctionTx
#endif /* ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) ) */
)
{

   uint8 retval = COM_SERVICE_NOT_AVAILABLE;

#if (COM_DEV_ERROR_DETECT == STD_ON)

   if (SGArrayPtr == NULL_PTR)
   {
      retval = COM_SERVICE_NOT_AVAILABLE;
      COM_DET_REPORTERROR(COM_E_PARAM_POINTER, COMServiceId_SendSignalGroupArray);
   }
   else if (SGArrayLength != (uint16)(SignalGPtr->ComByteSize))
   {
      retval = COM_SERVICE_NOT_AVAILABLE;
      COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_SendSignalGroupArray);
   }
   else
#endif /* COM_DEV_ERROR_DETECT */
   {
      TS_PARAM_UNUSED(SGArrayLength);

      retval = Com_EB_SendSignalGroupArray(
            SGArrayPtr,
            SignalGPtr,
            PduId,
            IPduPtr
#if ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) )
            ,SendInMainFunctionTx
#endif /* ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) ) */
      );
   }


   return retval;

}                               /* Com_EB_SendSignalGroupArray_DETCheck */

#endif /* (COM_BASED_TRANSFORMER_SUPPORT_TX == STD_ON) */


TS_MOD_PRIV_DEFN FUNC(uint8, COM_CODE) Com_EB_SendSignalGroupArray
(
   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) SGArrayPtr,
   P2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   ComIPduRefType PduId,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
#if ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) )
   ,boolean SendInMainFunctionTx
#endif /* ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) ) */
)
{
   uint8 retval = COM_SERVICE_NOT_AVAILABLE;


#if (COM_TMS_ENABLE == STD_ON)

   Com_EB_SetGroupSignalTMSFlagOfSignalGroup(
            SignalGPtr,
            SGArrayPtr);

#endif /* (COM_TMS_ENABLE == STD_ON) */
   {

#if (COM_TX_TPAPI_ENABLE == STD_ON)
     boolean TpPduBuffer_IsLocked = FALSE;
#endif /* (COM_TX_TPAPI_ENABLE == STD_ON) */

     /* copy the value to the I-Pdu buffer */
     boolean forceTrigger =
         Com_EB_WriteShadowBuffer(SignalGPtr, IPduPtr, SGArrayPtr
#if (COM_TX_TPAPI_ENABLE == STD_ON)
            , &TpPduBuffer_IsLocked
#endif /* (COM_TX_TPAPI_ENABLE == STD_ON) */
         );


#if (COM_TX_TPAPI_ENABLE == STD_ON)
     if(TRUE == TpPduBuffer_IsLocked)
     {
       retval = COM_BUSY;
     }
     else
#endif /* (COM_TX_TPAPI_ENABLE == STD_ON) */
     {
      /* calculate the TM of the signal group
       * and send if required */
      retval = Com_EB_CalculateSignalGroupTM(
          forceTrigger
#if ((COM_TMS_ENABLE == STD_ON) || (COM_TRANSF_PROP_WITHOUT_REP_ENABLE == STD_ON))
          ,SignalGPtr
          ,IPduPtr
#endif
          ,PduId
#if ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) )
          ,SendInMainFunctionTx
#endif /* ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) ) */
      );

     }

   }

   return retval;
}


/** \brief Com_EB_CalculateSignalGroupTM - It evaluates the TMS flags of the
 * group signals and calculates the TMS for the signal group and
 * initiates sending of the I-Pdu if necessary
 * Preconditions:
 * - COM must be initialized.
 * \param[in] SignalGPtr - reference to configuration of signal group
 * \param[in] PduId - id of I-Pdu to which the signal group belongs
 * \param[in] IPduPtr - reference to the configuration of I-Pdu
 * \param[in] SendInMainFunctionTx - if TRUE, the Pdu is sent not now but
 * during the next Com_MainfunctionTx(), otherwise immediately
 * \param[in] forceTrigger - if TRUE the transmission is triggered
 * \return Function execution success status
 * \retval E_OK if signal group could be copied to the I-Pdu
 * \retval COM_SERVICE_NOT_AVAILABLE if corresponding I-PDU group was stopped
 * or development error occurred
 */
_COM_STATIC_ FUNC(uint8, COM_CODE) Com_EB_CalculateSignalGroupTM
(
    boolean forceTrigger
#if ((COM_TMS_ENABLE == STD_ON) || (COM_TRANSF_PROP_WITHOUT_REP_ENABLE == STD_ON))
   ,P2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr
   ,P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
#endif
   ,ComIPduRefType PduId
#if ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) )
   ,boolean SendInMainFunctionTx
#endif /* ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) ) */
)
{
   uint8 retval;

   /* Flag byte Ptr for status byte of I-Pdu */
   CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) PduStatusFlagsPtr =
            &Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + PduId)];

#if ((COM_TMS_ENABLE == STD_ON) || (COM_SENDUPONAPICALL == STD_ON))
   boolean transmit = forceTrigger;
#else
   TS_PARAM_UNUSED(forceTrigger);
#endif

#if ((COM_TRANSF_PROP_WITHOUT_REP_ENABLE == STD_ON) && (COM_TMS_ENABLE == STD_ON))
   boolean TMSChanged = FALSE;
#endif


#if (COM_TMS_ENABLE == STD_ON)
   {

      ComSignalGroupRefType SignalGId;

      /* get Id of the signal group */
      COM_GET_TX_SG_ID(SignalGId, SignalGPtr);

      {
         /* Flag byte Ptr for status byte of group signal */
         CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) SGFlagPtr =
                  COM_TXSG_TMS_MIGHT_CHANGE_FLAG_ADDRESS(SignalGId);

         /* Critical section */
         Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();
         {


            /* get the TM might change flag of Signal Group */
            if ( Com_IsBitSet_8(SGFlagPtr, COM_TXSG_TMSMIGHTCHANGE) )
            {
               /* Although the might change flag is set, it does not mean that the
                * state really has changed. We have to check if at least on signal
                * has a positive result */
               CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) SignalGTMFlagsBase =
                        (&Com_gDataMemPtr[(SignalGPtr->ComSignalGTMSFlagsRef)]);
               uint16 j = 0U;
               boolean newResult = FALSE;
               boolean CurrentTMSG;
               uint16 numberOfFlagBytes;

               /* clear the COM_TXSG_TMSMIGHTCHANGE flag */
               Com_ClearBitUInt8(SGFlagPtr, COM_TXSG_TMSMIGHTCHANGE);

               /* get the number of Bytes which hold the TM flags of the Signal group */
               numberOfFlagBytes = SignalGPtr->ComGroupSignalWithFilterRefNum / 8U;
               if ((SignalGPtr->ComGroupSignalWithFilterRefNum % 8U) > 0U)
               {
                  numberOfFlagBytes++;
               }

               /* check if at least one bit is set */
               for(j = 0U; j < numberOfFlagBytes; j++)
               {
                  if (SignalGTMFlagsBase[j] > 0U)
                  {
                     /* at least one bit is set -> TM is TRUE */
                     newResult = TRUE;
                  }
               }                            /* for(j = 0U; j < numberOfFlagBytes; j++) */

               /* get the current TMS status of Signal Group */
               TS_GetBit(SGFlagPtr, COM_TXSG_TMSSTATE, uint8, CurrentTMSG);

               if (newResult != CurrentTMSG)
               {

                  /* store result of signal group */
                  if (newResult == TRUE)
                  {
                     Com_SetBitUInt8((&Com_gDataMemPtr[(IPduPtr->ComIPduTMSFlagsRef) +
                                                       ((ComRAMIdxType)SignalGPtr->ComPositionOfTMSFlag / 8U)]),
                              ((uint8) (SignalGPtr->ComPositionOfTMSFlag % 8U)));

                     Com_SetBitUInt8(SGFlagPtr, COM_TXSG_TMSSTATE);
                  }
                  else
                  {
                     Com_ClearBitUInt8((&Com_gDataMemPtr[(IPduPtr->ComIPduTMSFlagsRef) +
                                                         ((ComRAMIdxType)SignalGPtr->ComPositionOfTMSFlag / 8U)]),
                              ((uint8) (SignalGPtr->ComPositionOfTMSFlag % 8U)));

                     Com_ClearBitUInt8(SGFlagPtr, COM_TXSG_TMSSTATE);
                  }
               }                         /* if (newResult != CurrentTM) */


               /* always recalculate the TM of I-PDU since it might have changed due to a call to
                * Com_SwitchIpduTxMode() */
               {

                  boolean newTM;
                  boolean CurrentTMIPDU;

                  /* get the current TMS status of I-PDU */
                  TS_GetBit(PduStatusFlagsPtr, COM_TXPDU_TMSSTATE, uint8, CurrentTMIPDU);

                  /* calculate if the TM of the Pdu changed */
                  /* we are already in a critical section -> no need to protect the function */
                  newTM = Com_EB_CalculateTM(IPduPtr);

                  if (newTM != CurrentTMIPDU)
                  {

#if (COM_TRANSF_PROP_WITHOUT_REP_ENABLE == STD_ON)
                     TMSChanged = TRUE;
#endif


                     /* change the TM of the I-Pdu */
                     if (newTM == TRUE)
                     {
                        /* set the flag in the IPdu */
                        Com_SetBitUInt8(PduStatusFlagsPtr, COM_TXPDU_TMSSTATE);
                     }
                     else
                     {
                        /* clear the flag in the IPdu */
                        Com_ClearBitUInt8(PduStatusFlagsPtr, COM_TXPDU_TMSSTATE);
                     }

                     /* Set the changed flag of the I-Pdu */
                     Com_SetBitUInt8(PduStatusFlagsPtr, COM_TXPDU_TMSCHANGED);

                     /* COM582: If a change of the TMS causes a change to the
                      * transmission mode DIRECT, an immediate (respecting the MDT)
                      * direct/ n-times transmission to the underlying layer shall
                      * be initiated.
                      * -> shall also initiate transmission if signal has
                      * transfer property Pending */
                     {
                        uint8 const AktTM =
                                 COM_GET_COM_TX_MODE(newTM, IPduPtr->ComTxModeModes);

                        if ((COM_TX_MODE_MODE_DIRECT == AktTM) || (COM_TX_MODE_MODE_N_TIMES == AktTM))
                        {
                           transmit = TRUE;
                        }

                     }

                  }                      /* if (newTM != CurrentTM) */

               }

            }                            /* if ( Com_IsBitSet_8(SGFlagPtr, COM_TXSG_TMSMIGHTCHANGE) ) */

         }
         /* Leave critical section */
         Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();
      }
   }

#endif /* (COM_TMS_ENABLE == STD_ON) */
   /* There is a minor race here; see Com_SendSignal */

   /* Only do further processing if the IPdu is started */
   if ( Com_IsBitSet_8(PduStatusFlagsPtr, COM_TXPDU_STARTED) )
   {
     retval = E_OK;

#if (COM_SENDUPONAPICALL == STD_ON)

      if (TRUE == transmit)
      {

#if (COM_TRANSF_PROP_WITHOUT_REP_ENABLE == STD_ON)

        boolean trig_wo_rep = (COM_TRANSFER_PPROPERTY_TRIG_WO_REP ==
            COM_GET_COM_TX_SIGNAL_GROUP_TRANSFER_PROPERTY_WO_REP(SignalGPtr));

        boolean tms_changed =
#if (COM_TMS_ENABLE == STD_ON)
            (TMSChanged == FALSE);
#else
            TRUE;
#endif

         if ( (trig_wo_rep) && (tms_changed) )
         {

            /* send out Pdu once, independent from  transmission mode */
            Com_EB_Request_Send_Pdu(
                     PduId,
                     IPduPtr
#if ((COM_MDT_QUEUE_AVAILABLE == STD_ON) || (COM_SIGNAL_GW_ENABLE == STD_ON))
                     ,PduStatusFlagsPtr
#endif
#if ( (COM_SIGNAL_GW_ENABLE == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) )
                     ,SendInMainFunctionTx
#endif /* ( (COM_SIGNAL_GW_ENABLE == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) ) */
            );
         }
         else
#endif /* (COM_TRANSF_PROP_WITHOUT_REP_ENABLE == STD_ON) */
         {
            /* make a call to eventually have it sent according to transmission mode */
            Com_EB_SendUponSendApiCall(
                     PduId
#if ( (COM_SIGNAL_GW_ENABLE == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) )
                     ,SendInMainFunctionTx
#endif /* ( (COM_SIGNAL_GW_ENABLE == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) ) */
            );
         }
      }

#endif /* (COM_SENDUPONAPICALL == STD_ON) */
   }
   else                         /* <-- IPdu started / stopped --> */
   {
      /* If the IPdu is not started, do return error code */
      retval = COM_SERVICE_NOT_AVAILABLE;
   }
   return retval;
}                               /* Com_EB_CalculateSignalGroupTM */


/** \brief Com_EB_WriteShadowBuffer_Fragment - writes fragmented shadow buffer
 * to the I-Pdu
 * The service Com_EB_WriteShadowBuffer_Fragment copies the value of fragmented signal group
 * into the I-Pdu
 * Preconditions:
 * COM must be initialized.
 * *forceTriggerPtr shall be set to TRUE if the configuration or previous TOC calculations
 * indicate that the Pdu shall be triggered; else value shall be FALSE
 * \param[in] PduValPtr - reference to the value of the Pdu
 * \param[in] SGArrayValPtr - reference to the value in a signal group array
 * \param[in] WriteMaskPtr - reference the mask which defines the relevant bits of the signal group
 * \param[in] TOCMaskPtr - reference the mask which defines the relevant bits
 * for the TRIGGERED ON CHANGE calculation
 * \param[in,out] forceTriggerPtr - if the value changed and it is relevant for the
 * TRIGGERED ON CHANGE calculation, then the value is set to TRUE, else the value remains.
 * \param[in] offset offset for SGArrayValPtr and TOCMaskPtr
 * \param[in] ByteSize number of bytes of the signal group
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_WriteShadowBuffer_Fragment
(
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) PduValPtr,
   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) SGArrayValPtr,
   P2CONST(uint8, AUTOMATIC, COM_APPL_CONST) WriteMaskPtr,
#if (COM_EVALUATE_TRIGGERED_OC == STD_ON)
   P2CONST(uint8, AUTOMATIC, COM_APPL_CONST) TOCMaskPtr,
   P2VAR(boolean, AUTOMATIC, AUTOMATIC) forceTriggerPtr,
#endif
   ComSignalDescByteType ByteSize
)
{
   uint32 offset;

   for (offset = 0U; offset < (ByteSize); offset++)
   {

      uint8 valuePdu = PduValPtr[offset];
      uint8 valueShadowBuffer = SGArrayValPtr[offset];
      uint8 writeMask = WriteMaskPtr[offset];

      /* clear the unused bits */
      valuePdu = (valuePdu & (uint8)(~(writeMask)));
      valueShadowBuffer = (valueShadowBuffer & writeMask);

   #if (COM_EVALUATE_TRIGGERED_OC == STD_ON)
      {
         if(TOCMaskPtr != NULL_PTR)
         {
            /* calculate value for comparison */
            uint8 valuePduCmp = PduValPtr[offset];
            valuePduCmp = (valuePduCmp & writeMask);

            /* check if the relevant parts have changed */
            COM_TRIGGERED_OC_BYTE_VALUE_CHANGED_TOCMASK(
                     valuePduCmp, valueShadowBuffer,  TOCMaskPtr[offset], *forceTriggerPtr);
         }

      }
   #endif /* (COM_EVALUATE_TRIGGERED_OC == STD_ON) */

      /* Copy to Pdu */
      PduValPtr[offset] = (uint8)(valuePdu | valueShadowBuffer);

   }
}


/** \brief Com_EB_WriteShadowBuffer - writes the shadow buffer to the I-Pdu
 * The service Com_EB_WriteShadowBuffer copies the value of the signal group
 * into the I-Pdu
 * Preconditions:
 * COM must be initialized.
 * \param[in] SignalGPtr - reference to configuration of the signal group
 * \param[in] IPduPtr - reference to configuration of the I-Pdu to which the
 * \param[in] SGArrayPtr - reference to the signal group array
 * Signal Group is stored
 * signal group belongs
 * \param[out] TpPduBuffer_IsLocked - returns Tp buffer lock state
 * \return
 * \retval TRUE transmission shall be triggered, else FALSE
 */
_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_WriteShadowBuffer
(
   P2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) SGArrayPtr
#if (COM_TX_TPAPI_ENABLE == STD_ON)
   , P2VAR(boolean, AUTOMATIC, COM_APPL_DATA) TpPduBuffer_IsLocked
#endif /* (COM_TX_TPAPI_ENABLE == STD_ON) */
)
{
   boolean forceTrigger = FALSE;

   /* get the size of the signal group within the IPdu */
   ComSignalDescByteType const ByteSize = SignalGPtr->ComByteSize;
   /* reference to RAM where the value of the IPdu is stored */
   ComRAMIdxType const ComIPduValueRef = IPduPtr->ComIPduValueRef;

   ComSignalDescByteType byteOffset = 0U;

#if (COM_TX_TPAPI_ENABLE == STD_ON)
   boolean isLocked;
#endif /* (COM_TX_TPAPI_ENABLE == STD_ON) */

   ComSignalDescByteType StartingByte;

   P2CONST(uint8, AUTOMATIC, COM_APPL_CONST) writeMaskPtr =
            COM_GET_CONFIG_ELEMENT_CHECK(uint8, SignalGPtr->ComWriteMaskRef, 0U);

#if (COM_EVALUATE_TRIGGERED_OC == STD_ON)
   P2CONST(uint8, AUTOMATIC, COM_APPL_CONST) TOCMaskPtr =
            COM_GET_CONFIG_ELEMENT_CHECK(uint8, SignalGPtr->ComTOCMaskRef, 0U);
#endif /* (COM_EVALUATE_TRIGGERED_OC == STD_ON) */


#if (COM_TRANSF_PROP_TRIGGERED_ENABLE == STD_ON)
   /* transmit Pdu if TransferProperty == TRIGGERED */
   if (COM_TRANSFER_PPROPERTY_TRIG == COM_GET_COM_TX_SIGNAL_GROUP_TRANSFER_PROPERTY(SignalGPtr))
   {
      forceTrigger = TRUE;
   }
#endif /* (COM_TRANSF_PROP_TRIGGERED_ENABLE == STD_ON) */


   /* get the position of the signal group within the IPdu */
   StartingByte = SignalGPtr->ComSignalPositionInByte;


   /* enter critical section */
   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();
   {
#if (COM_TX_TPAPI_ENABLE == STD_ON)
     /* ref to TP Control struct exists for this IPdu */
     if( (IPduPtr->ComTxIpduTPControlInfoRef != COM_IPDU_ARRAY_SIZE_MAX) )
     {
       /* get TP Control array of struct base */
       CONSTP2VAR(EcuC_TxControlInfoType, AUTOMATIC, COM_VAR_CLEARED) TpTxContPtrBase =
           COM_GET_MEMPTR(EcuC_TxControlInfoType, Com_gConfigPtr->ComTxIpduTPControlInfoBase);

       /* get TP Control struct value pointer */
       CONSTP2VAR(EcuC_TxControlInfoType, AUTOMATIC, COM_VAR_CLEARED) TpTxContPtrValPtr =
           &TpTxContPtrBase[IPduPtr->ComTxIpduTPControlInfoRef];

       /* is Tx buffer for TP pdu locked */
        isLocked = EcuC_TxBufferIsLocked(TpTxContPtrValPtr);
     }
     else
     {
       /* no TP pdu -> write buffer */
        isLocked = FALSE;
     }

     *TpPduBuffer_IsLocked = isLocked;

     /* Pdu is not TP || (pdu is TP && Tp buffer is not locked) */
     if(FALSE == isLocked)
#endif /* (COM_TX_TPAPI_ENABLE == STD_ON) */
     {

      if (ByteSize != 0U)
      {

         if(writeMaskPtr != NULL_PTR)
         {
            /* copy with mask */
            Com_EB_WriteShadowBuffer_Fragment(
                     &Com_gDataMemPtr[(ComIPduValueRef + StartingByte)],
                     SGArrayPtr,
                     writeMaskPtr,
#if (COM_EVALUATE_TRIGGERED_OC == STD_ON)
                     TOCMaskPtr,
                     &forceTrigger,
#endif
                     ByteSize
            );
         }
         else
         {
            /* copy without mask */

#if (COM_EVALUATE_TRIGGERED_OC == STD_ON)
            if(TOCMaskPtr != NULL_PTR)
            {
               COM_TRIGGERED_OC_WRITE_BYTEARRAY_TO_PDU_TOCMASK(
                        &Com_gDataMemPtr[ComIPduValueRef + StartingByte],
                        &SGArrayPtr[byteOffset],
                        &TOCMaskPtr[byteOffset],
                        ByteSize,
                        forceTrigger);
            }
            else
#endif /* (COM_EVALUATE_TRIGGERED_OC == STD_ON) */
            {
               TS_MemCpy(&Com_gDataMemPtr[ComIPduValueRef + StartingByte],
                        &SGArrayPtr[byteOffset],
                        ByteSize);
            }

         }                            /* if(writeMaskPtr != NULL_PTR) */

      }                               /* if (ByteSize != 0U) */


#if (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL)

#if (COM_UPDATE_BIT_TX_CONFIG == COM_UPDATE_BIT_INDIVIDUAL)
      if (COM_GET_TX_SIGNAL_GROUP_UPDATE_BIT(SignalGPtr) == TRUE)
#endif /* (COM_UPDATE_BIT_TX_CONFIG == COM_UPDATE_BIT_INDIVIDUAL) */
      {

#if (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL)
         COM_AtomicSetBit_8(&Com_gDataMemPtr[(IPduPtr->ComIPduValueRef +
                  ((ComRAMIdxType)SignalGPtr->ComUpdateBitPositionInBit / 8U))],
                  (SignalGPtr->ComUpdateBitPositionInBit % 8U));
#endif /* (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) */
      }

#endif /* (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) */

     }
   }
   /* leave critical section */
   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   return forceTrigger;
}                               /* Com_EB_WriteShadowBuffer */

/** \brief Com_EB_InitSGBuffer - internal function initialization of a signal group array
 * The service Com_EB_InitSGBuffer copies the value of the IPdu buffer to the signal group array.
 * Preconditions:
 * - COM must be initialized.
 * \param[in] SGArrayPtr - Reference to the signal group array
 * \param[in] SignalGPtr - reference to configuration of signal group
 * \param[in] IPduPtr - reference to the configuration of I-Pdu
 * \return
 * \retval number of bytes which have been initialized
 */
TS_MOD_PRIV_DEFN FUNC(uint16, COM_CODE) Com_EB_InitSGBuffer
(
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) SGArrayPtr,
   P2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
)
{
   /* the signal group has a shadow buffer assigned which has to be initialized */
   ComSignalDescByteType const byteSize = SignalGPtr->ComByteSize;

   {
      /* get the first byte of the signal group in the Pdu */
      CONSTP2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) pduBuffer =
               &Com_gDataMemPtr[(IPduPtr->ComIPduValueRef + SignalGPtr->ComSignalPositionInByte)];

      /* copy the value of the Pdu to the signal group buffer */
      TS_MemCpy(SGArrayPtr,
               pduBuffer,
               byteSize);
   }

   return byteSize;
}


#if (COM_TMS_ENABLE == STD_ON)
/** \brief Com_EB_CalculateGroupSignalTM - calculates the transmission mode of
 * a group signal
 * The service Com_EB_CalculateGroupSignalTM evaluates the filter of the group
 * signal and sets the TMS flags
 * Preconditions:
 * - COM must be initialized.
 * - signal type COM_UINT8_N not allowed
 * \param[in] GSignalPtr - reference to configuration of group signal
 * \param[in] SignalGPtr - reference to configuration of signal group to
 * which the group signal belongs
 * \param[in] TMSResult - result of filter
 * \param[in] SignalType - type of the group signal
 */
TS_MOD_PRIV_DEFN FUNC(void, COM_CODE) Com_EB_CalculateGroupSignalTM
(
   P2CONST(ComTxSignalType, AUTOMATIC, COM_APPL_CONST) GSignalPtr,
   P2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   boolean TMSResult
)
{


   /* get the old value of TMS of signal */
   CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) FlagPtr =
      (&Com_gDataMemPtr
       [(SignalGPtr->ComSignalGTMSFlagsRef) +
        ((ComRAMIdxType)GSignalPtr->ComPositionOfTMSFlag / 8U)]);

   boolean OldFilterResult;
   boolean CurrentTM;

   ComSignalGroupRefType const SignalGId = GSignalPtr->ComSignalBase.ComSignalGroupRef;


   TS_GetBit(FlagPtr, ((uint8) (GSignalPtr->ComPositionOfTMSFlag % 8U)),
             uint8, OldFilterResult);


   /* get the current TMS status of Signal Group */
   TS_GetBit(COM_TXSG_TMS_MIGHT_CHANGE_FLAG_ADDRESS(SignalGId),
             COM_TXSG_TMSSTATE, uint8, CurrentTM);


   /* set the COM_TXSG_TMSMIGHTCHANGE flag if TMSResult is FALSE and
    * OldFilterResult is TRUE and CurrentTM is TRUE */
   if (((TMSResult == FALSE) && (OldFilterResult)) && (CurrentTM))
   {
      COM_AtomicSetBit_8(COM_TXSG_TMS_MIGHT_CHANGE_FLAG_ADDRESS(SignalGId),
                      COM_TXSG_TMSMIGHTCHANGE);
   }

   /* set COM_TXSG_TMSMIGHTCHANGE to TRUE if CurrentTM was FALSE and the
    * TMSResult is TRUE (OldFilterResult has to be FALSE because CurrentTM
    * is FALSE)*/
   if ((TMSResult) && (CurrentTM == FALSE))
   {
      COM_AtomicSetBit_8(COM_TXSG_TMS_MIGHT_CHANGE_FLAG_ADDRESS(SignalGId),
                      COM_TXSG_TMSMIGHTCHANGE);
   }

   /* store result of filter */
   if (TMSResult == TRUE)
   {
      COM_AtomicSetBit_8(FlagPtr, GSignalPtr->ComPositionOfTMSFlag % 8U);
   }
   else
   {
      COM_AtomicClearBit_8(FlagPtr, GSignalPtr->ComPositionOfTMSFlag % 8U);
   }
   return;
}                               /* Com_EB_CalculateGroupSignalTM */


/** \brief Com_EB_SetGroupSignalTMSFlagOfSignalGroup - calculates the TMS of a
 * signal group
 * This function Com_EB_SetGroupSignalTMSFlagOfSignalGroup loops over all group
 * signals of the signal group and calculates the TMS of the signal group
 * Preconditions:
 * - COM must be initialized
 * \param[in] SignalGPtr - reference to the configuration of signal group
 * \param[in] SGArrayPtr - reference to the signal group array
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_SetGroupSignalTMSFlagOfSignalGroup
(
   P2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) SGArrayPtr
)
{
   Com_SignalIdType i; /* iteration variable */

   boolean TMSResult;

   /* loop over all group signals with filter */
   for (i = SignalGPtr->ComGroupSignalRefFirst;
            i < (SignalGPtr->ComGroupSignalWithFilterRefNum + SignalGPtr->ComGroupSignalRefFirst);
            i++)
   {

      /* Get signal id and pointer to config of the signal in the signal list of the IPdu */
      ComSignalRefType const GSignalId =
               (ComSignalRefType) * (COM_GET_CONFIG_ELEMENT (ComSignalRefType,
                        (Com_gConfigPtr->ComTxSignalGroupGroupSignalListRef), i));

      CONSTP2CONST(ComTxSignalType, AUTOMATIC, COM_APPL_CONST) GSignalPtr =
               COM_GET_CONFIG_ELEMENT(ComTxSignalType,
                        Com_gConfigPtr->ComTxSignalRef, GSignalId);

      TMSResult = Com_EB_CalculateFilter(
               &GSignalPtr->ComSignalBase,
               SGArrayPtr,
               &GSignalPtr->ComSignalBase.ComFilterConf);

      /* calculate the value of the TMS Flag of the group signal */
      Com_EB_CalculateGroupSignalTM(
               GSignalPtr,
               SignalGPtr,
               TMSResult);

   } /* (i = SignalGPtr->ComGroupSignalRefFirst; ... */

   return;
}                               /* Com_EB_SetGroupSignalTMSFlagOfSignalGroup */

#endif /* (COM_TMS_ENABLE == STD_ON) */

/** \brief Com_SetupTxSignalGroup - retrieves configuration data for in order to copy
 * a signal group array into the appropriate I-PDU buffer and calls function to copy it into
 * the I-PDU buffer
 * Preconditions:
 * \param[out] SGArrayPtr - Reference to the signal group array to be transmitted;
 *             if function referred with Com_TxSignalGroupFPtr does not need info,
 *             it may be set to NULL_PTR
 * \param[in] SGArrayLength - Length of the signal group array
 *             if function referred with Com_TxSignalGroupFPtr does not need info,
 *             it may be set to 0
 * \param[in] Com_TxSignalGroupFPtr - function which shall be used to
 * copy the value (either copy shadow buffer or signal group array)
 * \param[in] SignalGPtr - reference to configuration of signal group
 * \param[in] retvalPtr - pointer where return value of Com_TxSignalGroupFPtr shall be stored
 * \param[in] SendInMainFunctionTx - if TRUE, the Pdu is sent not now but
 * during the next Com_MainfunctionTx(), otherwise immediately
 */
TS_MOD_PRIV_DEFN FUNC(void, COM_CODE) Com_SetupTxSignalGroup
(
   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) SGArrayPtr,
   uint16 SGArrayLength,
   P2VAR(Com_TxSignalGroupFPtrType, AUTOMATIC, COM_CODE) Com_TxSignalGroupFPtr,
   P2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
#if ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) )
   ,boolean SendInMainFunctionTx
#endif /* ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) ) */
)
{
   /* get the IPdu Id to which the signal group belongs */
   ComIPduRefType const PduId = SignalGPtr->ComIPduRef;

   /* get IPdu to which the signal belongs and check if the IPdu is valid */
   CONSTP2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr =
            COM_GET_CONFIG_ELEMENT(ComTxIPduType, Com_gConfigPtr->ComTxIPduRef, PduId);

   /* call internal signal group sending function */
   *retvalPtr = Com_TxSignalGroupFPtr(
            SGArrayPtr,
            SGArrayLength,
            SignalGPtr,
            PduId,
            IPduPtr
#if ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) )
            ,SendInMainFunctionTx
#endif /* ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) ) */
   );

}                               /* Com_SetupTxSignalGroup */

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
#endif


