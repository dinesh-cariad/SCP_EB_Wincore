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
 * MISRAC2012-1) Deviated Rule: 5.8 (required)
 * Identifiers that define objects or functions with external
 * linkage shall be unique.
 *
 * Reason:
 * This violation is a false positive.
 * The tool does not recognize the following situation:
 * The function identifier depends if the merge compile use
 * case is enabled or not. The behavior is realized through a
 * instrumented macro which enables the use case dependent
 * function identifier.
 *
 */

/*==================[inclusions]=============================================*/

#include <Com_Trace.h>
#include <TSAutosar.h>          /* EB specific standard types */
#include <ComStack_Types.h>     /* AUTOSAR standard types */
#include <Com_Api_Static.h>     /* module public static API */
#include <Com_Priv.h>           /* module internal stuff */
#include <TSMem.h>              /* TS_MemCpy */

#include <Com_ComXf.h>          /* APIs for EB ComXf */

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


#if (COM_RX_DATA_TOUT_ACTION != COM_RX_DATA_TOUT_ACTION_NONE)

/** \brief Com_ReceiveSignalGroup_Replace - checks if the value of the signal
 * group shall be replaced with the init value
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * \param[in] SignalPtr - pointer to the description of the signal
 * \param[in] PduId - Id of the Pdu to which the signal belongs to
 * \return Function execution success status
 * \retval TRUE - when the value shall be replaced
 * \retval FALSE - when the value shall be no be replaced
 */
_COM_STATIC_ FUNC(boolean, COM_CODE) Com_ReceiveSignalGroup_Replace
(
   P2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   ComIPduRefType PduId
);
#endif /* (COM_RX_DATA_TOUT_ACTION != COM_RX_DATA_TOUT_ACTION_NONE) */

/** \brief Com_GetRxSignalGroupCfg - returns the pointer to the configuration of a
 * Rx signal groups and provides DET checks
 * Preconditions:
 * \param[in] SignalGroupId - ID of signal group to be accessed.
 * \param[in] ServiceId - Id which shall be used for DET calls
 * \return Function execution success status
 * \retval pointer to configuration in case of success
 * \retval NULL_PTR in case of failure
 */
_COM_STATIC_ FUNC_P2CONST(ComRxSignalGroupType, COM_APPL_CONST, AUTOMATIC) Com_GetRxSignalGroupCfg
(
   Com_SignalGroupIdType SignalGroupId,
   uint8 ServiceId
);

/** \brief Com_SetupRxSignalGroup_DET - prepares reading of a signal group array and invokes
 * a function to copy the signal group array from the I-Pdu buffer
 * either to a signal group array or to a shadow buffer
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * \param[in] SignalGroupId - ID of signal group to be accessed.
 * \param[out] RxSGArrayInfoPtr - info pointer of the signal group array;
 *             if function referred with Com_RxSignalGroupFPtr does not need info,
 *             it may be set to NULL_PTR
 * \param[in] Com_RxSignalGroupFPtr - function which shall be used to
 * copy the value
 * \param[in] ServiceId - Id which shall be used for DET calls
 * \return Function execution success status
 * \retval E_OK - service has been accepted
 * \retval COM_SERVICE_NOT_AVAILABLE - corresponding I-PDU group was stopped
 * (or service failed due to development error)
 */
_COM_STATIC_ FUNC(uint8, COM_CODE) Com_SetupRxSignalGroup_DET
(
   Com_SignalGroupIdType SignalGroupId,
   P2VAR(Com_RxSGArrayInfoType, AUTOMATIC, COM_APPL_DATA) RxSGArrayInfoPtr,
   P2VAR(Com_RxSignalGroupFPtrType, AUTOMATIC, COM_CODE) Com_RxSignalGroupFPtr,
   uint8 ServiceId
);

#if (COM_BASED_TRANSFORMER_SUPPORT_RX == STD_ON)
/** \brief Com_CopyRxSignalGroupArray_Array - copies the signal group array to
 * the I-Pdu buffer from a signal group array
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * \param[out] retvalPtr - reference where the return vale shall be stored
 *    E_OK - service has been accepted
 *    COM_SERVICE_NOT_AVAILABLE - corresponding I-PDU group was stopped
 *    (or service failed due to development error)
 * \param[out] RxSGArrayInfoPtr - info pointer of the signal group array;
 *             if function referred with Com_RxSignalGroupFPtr does not need info,
 *             it may be set to NULL_PTR
 * \param[in] SignalGPtr - reference to the configuration of the signal group
 * \param[in] IPduPtr - reference to the configuration of the I-Pdu
 * to which the signal group belongs to
 * \param[in] PduId - Id of the I-Pdu
 * to which the signal group belongs to
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_CopyRxSignalGroupArray_Array
(
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr,
   P2VAR(Com_RxSGArrayInfoType, AUTOMATIC, COM_APPL_DATA) RxSGArrayInfoPtr,
   P2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   ComIPduRefType PduId
);
#endif /* (COM_BASED_TRANSFORMER_SUPPORT_RX == STD_ON) */

/** \brief Com_CopyRxSignalGroupArray - copies the signal group array to
 * the I-Pdu buffer from a buffer
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * \param[out] SGArrayPtr - reference to the location where the received
 *  signal group array shall be stored
 * \param[in] SignalGPtr - reference to the configuration of the signal group
 * \param[in] IPduPtr - reference to the configuration of the I-Pdu
 * to which the signal group belongs to
 * \param[in] PduId - Id of the I-Pdu
 * to which the signal group belongs to
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_CopyRxSignalGroupArray
(
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) SGArrayPtr,
   P2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   ComIPduRefType PduId
);


#if (COM_RX_DATA_TOUT_ACTION != COM_RX_DATA_TOUT_ACTION_NONE)
/** \brief Com_ReceiveSignalGroup_Replace - checks if the value of the signal
 * group shall be replaced with the init value
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * \param[in] SignalPtr - pointer to the description of the signal
 * \param[in] PduId - Id of the Pdu to which the signal belongs to
 * \return Function execution success status
 * \retval TRUE - when the value shall be replaced
 * \retval FALSE - when the value shall be no be replaced
 */
_COM_STATIC_ FUNC(boolean, COM_CODE) Com_ReceiveSignalGroup_Replace
(
   P2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   ComIPduRefType PduId
)
{
   boolean RxDMReplace = FALSE;

   CONSTP2CONST(ComRxSignalExtType, AUTOMATIC, COM_APPL_CONST) SignalExtPtr =
            COM_GET_CONFIG_ELEMENT(ComRxSignalExtType,
                     Com_gConfigPtr->ComRxSignalExtRef, SignalGPtr->ComSignalExtRef);


#if ((COM_RX_DATA_TOUT_ACTION == COM_RX_DATA_TOUT_ACTION_CONFIG))
   /* check if timeout replace is configured for the signal group */
   if (COM_GET_RX_SIGNAL_GROUP_TIMEOUT_ACTION(SignalExtPtr) == COM_RX_DATA_TOUT_ACTION_REPLACE)
#endif /* (COM_RX_DATA_TOUT_ACTION == COM_RX_DATA_TOUT_ACTION_CONFIG) */
   {

      uint8 RxDMTout;

#if (COM_RX_SIGNAL_BASED_DM == STD_ON)
      /* check if signal based DM is configured */
      if (COM_GET_RX_SIGNAL_GROUP_SIGDM(SignalExtPtr) == COM_RX_SIG_DM_ON)
      {

         /* check signal based DM flags */
         TS_GetBit(COM_RX_SIGNAL_GROUP_DM_FLAGS_BYTE_POS(SignalExtPtr),
                  COM_RX_SIGNAL_GROUP_DM_TIMEOUT_BIT_POS(SignalExtPtr), uint8, RxDMTout);

      }                      /* if (COM_GET_RX_SIGNAL_GROUP_DM(SignalGPtr) == COM_RX_DM_ON) */
      else
#endif /* (COM_RX_SIGNAL_BASED_DM == STD_ON) */
      {                      /* COM_RX_DM_OFF */
         /* evaluate I-Pdu based DM */
         TS_GetBit(&Com_gDataMemPtr[(Com_gConfigPtr->ComRxIPduStatusBase + PduId)],
                  COM_RXPDU_RXDMTIMEOUT, uint8, RxDMTout);
      }                      /* COM_RX_DM_OFF */

      if (1U == RxDMTout)
      {
         RxDMReplace = TRUE;
      }
   }

   return RxDMReplace;
}

#endif /* (COM_RX_DATA_TOUT_ACTION != COM_RX_DATA_TOUT_ACTION_NONE) */

_COM_STATIC_ FUNC_P2CONST(ComRxSignalGroupType, COM_APPL_CONST, AUTOMATIC) Com_GetRxSignalGroupCfg
(
   Com_SignalGroupIdType SignalGroupId,
   uint8 ServiceId
)
{
   P2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr = NULL_PTR;

   TS_PARAM_UNUSED(ServiceId);

#if (COM_DEV_ERROR_DETECT == STD_ON)

   /* check whether Com is initialized */
   if (Com_InitStatus == COM_EB_UNINIT)
   {
      COM_DET_REPORTERROR(COM_E_UNINIT, ServiceId);
   }
   /* Check if the SignalGroupId is within the range of the ComRxSignalGroupArraySize */
   else if (Com_gConfigPtr->ComRxSignalGroupArraySize <= SignalGroupId)
   {
      COM_DET_REPORTERROR(COM_E_PARAM, ServiceId);
   }
   else
      /* The check whether the signal is a Rx signal or not is done below with the check of the
       * configuration of the signal */

#endif /* COM_DEV_ERROR_DETECT */

   {

      /* get the configuration of the signal identified by the signal Id and check the
       * configuration */
      SignalGPtr =
            COM_GET_CONFIG_ELEMENT(ComRxSignalGroupType, Com_gConfigPtr->ComRxSignalGroupRef,
                  SignalGroupId);

#if (COM_PRECONDITION_ASSERT_ENABLED == STD_ON)
      /* CHECK: NOPARSE */
      if (NULL_PTR == SignalGPtr)
      {
         COM_PRECONDITION_ASSERT(
               (NULL_PTR == SignalGPtr),
               ServiceId);
      }
      else if (SignalGPtr->ComConfigCheckData != COM_CONFIG_CHECK_RX_SIG_GROUP)
      {
         COM_PRECONDITION_ASSERT(
               (SignalGPtr->ComConfigCheckData == COM_CONFIG_CHECK_RX_SIG_GROUP),
               ServiceId);
         SignalGPtr = NULL_PTR;
      }
      else
      {
         /* empty else */
      }
      /* CHECK: PARSE */
#endif /* (COM_PRECONDITION_ASSERT_ENABLED == STD_ON) */

#if ( (COM_DEV_ERROR_DETECT == STD_ON) && (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) )
      {

        CONSTP2CONST(ComRxSignalExtType, AUTOMATIC, COM_APPL_CONST) SignalExtPtr =
            COM_GET_CONFIG_ELEMENT(ComRxSignalExtType,
                Com_gConfigPtr->ComRxSignalExtRef, SignalGPtr->ComSignalExtRef);

        /* check if dummy signal group */
        if (TRUE == COM_GET_RX_SIGNAL_GROUP_SIGDUMMY(SignalExtPtr))
        {
          COM_DET_REPORTERROR(COM_E_PARAM, ServiceId);
          SignalGPtr = NULL_PTR;
        }

      }
#endif /* ( (COM_DEV_ERROR_DETECT == STD_ON) && (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) ) */

   }

   return SignalGPtr;
}                               /* Com_GetRxSignalGroupCfg */

_COM_STATIC_ FUNC(uint8, COM_CODE) Com_SetupRxSignalGroup_DET
(
   Com_SignalGroupIdType SignalGroupId,
   P2VAR(Com_RxSGArrayInfoType, AUTOMATIC, COM_APPL_DATA) RxSGArrayInfoPtr,
   P2VAR(Com_RxSignalGroupFPtrType, AUTOMATIC, COM_CODE) Com_RxSignalGroupFPtr,
   uint8 ServiceId
)
{

   uint8 retval = COM_SERVICE_NOT_AVAILABLE;

   /* pointer to the configuration of the signal group */
   CONSTP2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr =
            Com_GetRxSignalGroupCfg(SignalGroupId, ServiceId);


#if (COM_DEV_ERROR_DETECT == STD_ON)
   /* check if a valid signal config could be determined */
   if (SignalGPtr != NULL_PTR)
#endif /* (COM_DEV_ERROR_DETECT == STD_ON) */
   {

       Com_SetupRxSignalGroup(
               &retval,
               RxSGArrayInfoPtr,
               SignalGPtr,
               Com_RxSignalGroupFPtr
      );

   }


   return retval;
}                               /* Com_SetupRxSignalGroup_DET */

/* Deviation MISRAC2012-1 */
TS_MOD_PRIV_DEFN FUNC(void, COM_CODE) Com_SetupRxSignalGroup
(
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr,
   P2VAR(Com_RxSGArrayInfoType, AUTOMATIC, COM_APPL_DATA) RxSGArrayInfoPtr,
   P2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   P2VAR(Com_RxSignalGroupFPtrType, AUTOMATIC, COM_CODE) Com_RxSignalGroupFPtr
)
{
   /* get the IPdu Id to which the signal belongs */
   ComIPduRefType const PduId = SignalGPtr->ComIPduRef;

   /* get IPdu to which the signal belongs and check if the IPdu is valid */
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr =
            COM_GET_CONFIG_ELEMENT(ComRxIPduType, Com_gConfigPtr->ComRxIPduRef, PduId);

   {

      if ( Com_IsBitSet_8(&Com_gDataMemPtr[(Com_gConfigPtr->ComRxIPduStatusBase + PduId)],
          COM_RXPDU_STARTED) )
      {
         *retvalPtr = E_OK;
      }


      Com_RxSignalGroupFPtr(
               retvalPtr,
               RxSGArrayInfoPtr,
               SignalGPtr,
               IPduPtr,
               PduId
      );

   }
}

#if (COM_BASED_TRANSFORMER_SUPPORT_RX == STD_ON)
_COM_STATIC_ FUNC(void, COM_CODE) Com_CopyRxSignalGroupArray_Array
(
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr,
   P2VAR(Com_RxSGArrayInfoType, AUTOMATIC, COM_APPL_DATA) RxSGArrayInfoPtr,
   P2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   ComIPduRefType PduId
)
{

   TS_PARAM_UNUSED(retvalPtr);

#if (COM_DEV_ERROR_DETECT == STD_ON)
   if (RxSGArrayInfoPtr->inSGArrayLength < SignalGPtr->ComByteSize)
   {
      *retvalPtr = COM_SERVICE_NOT_AVAILABLE;
      COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_ReceiveSignalGroupArray);
   }
   else
#endif /* (COM_DEV_ERROR_DETECT == STD_ON) */
   {

      Com_CopyRxSignalGroupArray(
            RxSGArrayInfoPtr->SGArrayPtr,
            SignalGPtr,
            IPduPtr,
            PduId
      );

      *RxSGArrayInfoPtr->outSGArrayLengthPtr = SignalGPtr->ComByteSize;
   }

}
#endif /* (COM_BASED_TRANSFORMER_SUPPORT_RX == STD_ON) */


/* Deviation MISRAC2012-1 */
TS_MOD_PRIV_DEFN FUNC(void, COM_CODE) Com_CopyRxSignalGroupArray_Shadow
(
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr,
   P2VAR(Com_RxSGArrayInfoType, AUTOMATIC, COM_APPL_DATA) RxSGArrayInfoType,
   P2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   ComIPduRefType PduId
)
{

   const ComRAMIdxType shadowBufferRef = SignalGPtr->ComSignalGroupValueRef;


   TS_PARAM_UNUSED(RxSGArrayInfoType);


#if ((COM_DEV_ERROR_DETECT == STD_ON) && \
      (COM_BASED_TRANSFORMER_SUPPORT_RX == STD_ON))
   if (shadowBufferRef == COM_RAM_SIZE_MAX)
   {
      /* the signal group has no shadow buffer assigned an shall
       * only be used with the Com_ReceiveSignalGroupArray() */
      *retvalPtr = COM_SERVICE_NOT_AVAILABLE;
      COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_ReceiveSignalGroup);
   }
   else
#else
      TS_PARAM_UNUSED(retvalPtr);
#endif /* ((COM_DEV_ERROR_DETECT == STD_ON) && \
     (COM_BASED_TRANSFORMER_SUPPORT_RX == STD_ON)) */
   {

      CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) shadowBufferPtr =
            &Com_gDataMemPtr[shadowBufferRef];

      Com_CopyRxSignalGroupArray(
         shadowBufferPtr,
         SignalGPtr,
         IPduPtr,
         PduId
      );
   }


}                               /* Com_CopyRxSignalGroupArray_Shadow */



_COM_STATIC_ FUNC(void, COM_CODE) Com_CopyRxSignalGroupArray
(
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) SGArrayPtr,
   P2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   ComIPduRefType PduId
)
{
   {

#if (COM_RX_DATA_TOUT_ACTION != COM_RX_DATA_TOUT_ACTION_NONE)
      boolean const RxDMReplace = Com_ReceiveSignalGroup_Replace(SignalGPtr, PduId);

      /* check if value should be replaced */
      if (RxDMReplace == TRUE)
      {
         /* value has to be replaced with init value */

         /* pointer to first byte of init value of the signal */
         CONSTP2CONST(uint8, AUTOMATIC, COM_APPL_CONST) valPtr =
               COM_GET_CONFIG_ELEMENT(uint8, IPduPtr->ComInitValueRef,
                     SignalGPtr->ComSignalPositionInByte);

         /* copy value from PDU buffer to shadow buffer */
         TS_MemCpy(SGArrayPtr,
            valPtr,
            SignalGPtr->ComByteSize);

      }
      else
#endif /* (COM_RX_DATA_TOUT_ACTION != COM_RX_DATA_TOUT_ACTION_NONE) */
      {
         /* pointer to first byte of value */
         CONSTP2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) valPtr =
               &Com_gDataMemPtr[(IPduPtr->ComIPduValueRef +
                     SignalGPtr->ComSignalPositionInByte)];

         TS_PARAM_UNUSED(PduId);

         /* Enter critical section */
         Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

         /* copy value from PDU buffer to shadow buffer */
         TS_MemCpy(SGArrayPtr,
            valPtr,
            SignalGPtr->ComByteSize);

         /* Leave critical section */
         Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();
      }

   }


}

FUNC(uint8, COM_CODE) Com_ReceiveSignalGroup
(
   Com_SignalGroupIdType SignalGroupId
)
{
   uint8 retval = COM_SERVICE_NOT_AVAILABLE;

   DBG_COM_RECEIVESIGNALGROUP_ENTRY(SignalGroupId);

   retval = Com_SetupRxSignalGroup_DET(
         SignalGroupId,
         NULL_PTR,
         &Com_CopyRxSignalGroupArray_Shadow,
         COMServiceId_ReceiveSignalGroup
   );


   DBG_COM_RECEIVESIGNALGROUP_EXIT(retval,SignalGroupId);
   return retval;
}                               /* Com_ReceiveSignalGroup */

FUNC(uint8, COM_CODE) Com_ReceiveSignalGroupArray
(
   Com_SignalGroupIdType SignalGroupId,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) SignalGroupArrayPtr,
   P2VAR(uint16, AUTOMATIC, COM_APPL_DATA) SignalGroupArrayLengthPtr
)
{
   uint8 retval = COM_SERVICE_NOT_AVAILABLE;

   DBG_COM_RECEIVESIGNALGROUPARRAY_ENTRY(SignalGroupId,SignalGroupArrayPtr,SignalGroupArrayLengthPtr);

#if (COM_BASED_TRANSFORMER_SUPPORT_RX == STD_ON)

#if (COM_DEV_ERROR_DETECT == STD_ON)
   if (SignalGroupArrayLengthPtr == NULL_PTR)
   {
      COM_DET_REPORTERROR(COM_E_PARAM_POINTER, COMServiceId_ReceiveSignalGroupArray);
   }
   else if (SignalGroupArrayPtr == NULL_PTR)
   {
      *SignalGroupArrayLengthPtr = 0U;
      COM_DET_REPORTERROR(COM_E_PARAM_POINTER, COMServiceId_ReceiveSignalGroupArray);
   }
   else
#endif /* (COM_DEV_ERROR_DETECT == STD_ON) */
   {

      Com_RxSGArrayInfoType SGArrayInfo;
      SGArrayInfo.SGArrayPtr = SignalGroupArrayPtr;
      SGArrayInfo.outSGArrayLengthPtr = SignalGroupArrayLengthPtr;
      SGArrayInfo.inSGArrayLength = *SignalGroupArrayLengthPtr;

      *SignalGroupArrayLengthPtr = 0U;

      retval = Com_SetupRxSignalGroup_DET(
            SignalGroupId,
            &SGArrayInfo,
            &Com_CopyRxSignalGroupArray_Array,
            COMServiceId_ReceiveSignalGroupArray
      );
   }

#else

   TS_PARAM_UNUSED(SignalGroupId);
   TS_PARAM_UNUSED(SignalGroupArrayPtr);
   TS_PARAM_UNUSED(SignalGroupArrayLengthPtr);

#if (COM_DEV_ERROR_DETECT == STD_ON)
   COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_ReceiveSignalGroupArray);
#endif /* (COM_DEV_ERROR_DETECT == STD_ON) */


#endif /* (COM_BASED_TRANSFORMER_SUPPORT_RX == STD_ON) */

   DBG_COM_RECEIVESIGNALGROUPARRAY_EXIT(retval,SignalGroupId,SignalGroupArrayPtr,SignalGroupArrayLengthPtr);
   return retval;
}                               /* Com_ReceiveSignalGroupArray */

#if (COM_BASED_TRANSFORMER_SUPPORT_RX == STD_ON)
FUNC(uint16, COM_CODE) Com_ComXf_Get_RxSignalGroupSize
(
   Com_SignalGroupIdType SignalGroupId
)
{

   uint16 length = 0U;

   /* pointer to the configuration of the signal group */
   CONSTP2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr =
            Com_GetRxSignalGroupCfg(SignalGroupId, COMServiceId_InternalAPI);

#if (COM_DEV_ERROR_DETECT == STD_ON)
   /* check if a valid signal config could be determined */
   if (SignalGPtr != NULL_PTR)
#endif /* (COM_DEV_ERROR_DETECT == STD_ON) */
   {
      length = SignalGPtr->ComByteSize;
   }

   return length;
}
#endif /* (COM_BASED_TRANSFORMER_SUPPORT_RX == STD_ON) */

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
#endif

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/
