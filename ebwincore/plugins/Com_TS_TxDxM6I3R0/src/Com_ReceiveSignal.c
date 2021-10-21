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
#include <Com_Defs.h>           /* COM_SERVICE_NOT_AVAILABLE */
#include <TSMem.h>              /* TS_MemCpy */
#include <Com_Filter.h>         /* Filter functions */
#include <Com_Core_RxIPdu.h>    /* RxIPdu functions */
#include <Com_Core_Read.h>      /* de-serialization library */
#include <stddef.h>             /* offsetof(), size_t */

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

#if (COM_FILTER_RECEIVER_ENABLE == STD_ON)
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_Read_SignalExt_SigBuffer
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr
);
#endif /* (COM_FILTER_RECEIVER_ENABLE == STD_ON) */


#if (COM_VALUE_INIT_REPLACE == STD_ON)
_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_Read_SignalReplace
(
   P2CONST(ComRxSignalType, AUTOMATIC, AUTOMATIC) SignalPtr
);
#endif /* (COM_VALUE_INIT_REPLACE == STD_ON) */

/** \brief Com_GetRxSignalCfg - returns the pointer to the configuration of a
 * Rx signal and provides DET checks
 * Preconditions:
 * \param[in] SignalGroupId - ID of signal to be accessed.
 * \param[in] ServiceId - Id which shall be used for DET calls
 * \return Function execution success status
 * \retval pointer to configuration in case of success
 * \retval NULL_PTR in case of failure
 */
_COM_STATIC_ FUNC_P2CONST(ComRxSignalType, COM_APPL_CONST, AUTOMATIC) Com_GetRxSignalCfg
(
   Com_SignalIdType SignalId,
   uint8 ServiceId
);


#if (COM_RX_DYN_LENGTH_IPDU_ENABLE == STD_ON)
/** \brief Com_EB_ReadValue_Uint8Dyn - reads the value of an Uint8Dyn signal
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * \param[in] SignalDataPtr - pointer where value shall be copied
 * \param[in,out] LengthPtr - in - length of the provided signal data buffer
 *                           out- length of the received signal data
 * \param[in] SignalPtr - properties of the signal which shall be read
 * \param[in] IPduPtr - pointer to the Rx-I-Pdu configuration data
 * \param[in] PduId - Rx-I-Pdu id
 * \return Function execution success status
 * \retval E_OK - success
 * \retval E_NOT_OK - the Length (as in-parameter) is smaller than the received
 *            length of the dynamic length signal
 */
_COM_STATIC_ FUNC(uint8, COM_CODE) Com_EB_ReadValue_Uint8Dyn
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint16, AUTOMATIC, COM_APPL_DATA) LengthPtr,
   P2CONST(ComRxSignalType, AUTOMATIC, AUTOMATIC) SignalPtr,
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   ComIPduRefType PduId
);


#endif /* (COM_RX_DYN_LENGTH_IPDU_ENABLE == STD_ON) */



#if (COM_RX_DATA_TOUT_ACTION != COM_RX_DATA_TOUT_ACTION_NONE)

/** \brief Com_EB_CheckRxSignalDM - Checks if the value of the signal shall be
 * replaced with the init value
 * this function checks if the DM (I-Pdu based or signal based) of the signal
 * timed out and the value of the signal shall be replaced.
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * \param[in] SignalExtPtr - pointer to the extended description of the signal
 * \param[in] PduId - Id of the I-Pdu to which the signal belongs to
 * \return Function execution success status
 * \retval TRUE if the value has to be replace, otherwise FALSE
 */
_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_CheckRxSignalDM
(
#if ((COM_RX_SIGNAL_BASED_DM == STD_ON) || \
      (COM_RX_DATA_TOUT_ACTION == COM_RX_DATA_TOUT_ACTION_CONFIG))
   P2CONST(ComRxSignalExtType, AUTOMATIC, COM_APPL_CONST) SignalExtPtr
#if (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_PRESENT_FOR_ALL)
   ,
#endif /* (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_PRESENT_FOR_ALL) */
#endif /* ((COM_RX_SIGNAL_BASED_DM == STD_ON) || \
      (COM_RX_DATA_TOUT_ACTION == COM_RX_DATA_TOUT_ACTION_CONFIG)) */
#if (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_PRESENT_FOR_ALL)
   ComIPduRefType PduId
#endif /* (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_PRESENT_FOR_ALL) */
);

#endif /* (COM_RX_DATA_TOUT_ACTION != COM_RX_DATA_TOUT_ACTION_NONE) */



#if (COM_RX_DYN_LENGTH_IPDU_ENABLE == STD_ON)
/** \brief Com_EB_ReadValue_Uint8Dyn - reads the value of an Uint8Dyn signal
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * \param[in] SignalDataPtr - pointer where value shall be copied
 * \param[in,out] LengthPtr - in - length of the provided signal data buffer
 *                           out- length of the received signal data
 * \param[in] ComReadPropPtr - properties of the signal which shall be read
 * \param[in] IPduPtr - pointer to the Rx-I-Pdu configuration data
 * \param[in] PduId - Rx-I-Pdu id
 * \return Function execution success status
 * \retval E_OK - success
 * \retval E_NOT_OK - the Length (as in-parameter) is smaller than the received
 *            length of the dynamic length signal
 */
_COM_STATIC_ FUNC(uint8, COM_CODE) Com_EB_ReadValue_Uint8Dyn
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint16, AUTOMATIC, COM_APPL_DATA) LengthPtr,
   P2CONST(ComRxSignalType, AUTOMATIC, AUTOMATIC) SignalPtr,
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   ComIPduRefType PduId
)
{

   uint8 retval = E_NOT_OK;

   TS_PARAM_UNUSED(IPduPtr);
   {
      ComSignalBitSizeType SigLen;

      /* Starting position of the signals in bytes NOTE: this
       * assumes that signals of type UINT8_N are always byte aligned */
      ComSignalDescBitType const startbyte = SignalPtr->ComSignalBase.ComSignalPositionInBit;


#if (COM_VALUE_INIT_REPLACE == STD_ON)
      boolean const takeInitVal =
               Com_EB_Read_SignalReplace(SignalPtr);


      if (takeInitVal == TRUE)
      {
         /* read initial value */

         SigLen = IPduPtr->ComIPduInitSize - startbyte;
         /* check if the provided buffer is big enough for the init value of signal */
         if( (*LengthPtr) >= SigLen )
         {
            /* value from Pdu shall be replaced by the init value */

            CONSTP2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) initDataPduPtr =
                     COM_GET_CONFIG_ELEMENT(uint8, IPduPtr->ComInitValueRef, 0U);

            CONSTP2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) initDataDynSigPtr =
                     &initDataPduPtr[startbyte];

            /* copy init value */
            TS_MemCpy(SignalDataPtr,
                     initDataDynSigPtr,
                     SigLen);
            retval = E_OK;

         }

      }
      else
#endif /* (COM_VALUE_INIT_REPLACE == STD_ON) */
      {
         /* read the value from the Pdu */
         /* pointer to first byte of value */
         CONSTP2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr =
                  &Com_gDataMemPtr[SignalPtr->ComSignalBase.ComBufferRef + startbyte];

         /* Enter critical section */
         Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();
         {
            /* Deviation MISRAC2012-1 */
            const uint16 pduLength = COM_GET_RX_IPDU_SIZE(PduId, IPduPtr);

            /* check if the signal is present in Pdu */
            if (pduLength > startbyte)
            {
               /* calculate the length of the signal */
               SigLen = pduLength - startbyte;
            }
            else
            {
               /* Pdu does not contain signal, set size to 0 */
               SigLen = 0U;
            }

            /* check if the provided buffer is big enough for the value of signal */
            if( (*LengthPtr) >= SigLen )
            {
               /* copy the data from the Pdu */
               TS_MemCpy(SignalDataPtr,
                        dataPtr,
                        SigLen);

               retval = E_OK;
            }
         }
         /* Leave critical section */
         Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();
      }

      /* store the length of received signal in any case */
      (*LengthPtr) = SigLen;

   }
   return retval;
}                               /* Com_EB_ReadValue_Uint8N */


#endif /* (COM_RX_DYN_LENGTH_IPDU_ENABLE == STD_ON) */



#if (COM_RX_DATA_TOUT_ACTION != COM_RX_DATA_TOUT_ACTION_NONE)

/** \brief Com_EB_CheckRxSignalDM - Checks if the value of the signal shall be
 * replaced with the init value
 * this function checks if the DM (I-Pdu based or signal based) of the signal
 * timed out and the value of the signal shall be replaced.
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * \param[in] SignalExtPtr - pointer to the extended description of the signal
 * \param[in] PduId - Id of the I-Pdu to which the signal belongs to
 * \return Function execution success status
 * \retval TRUE if the value has to be replace, otherwise FALSE
 */
_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_CheckRxSignalDM
(
#if ((COM_RX_SIGNAL_BASED_DM == STD_ON) || \
      (COM_RX_DATA_TOUT_ACTION == COM_RX_DATA_TOUT_ACTION_CONFIG))
   P2CONST(ComRxSignalExtType, AUTOMATIC, COM_APPL_CONST) SignalExtPtr
#if (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_PRESENT_FOR_ALL)
   ,
#endif /* (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_PRESENT_FOR_ALL) */
#endif /* ((COM_RX_SIGNAL_BASED_DM == STD_ON) || \
      (COM_RX_DATA_TOUT_ACTION == COM_RX_DATA_TOUT_ACTION_CONFIG)) */
#if (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_PRESENT_FOR_ALL)
   ComIPduRefType PduId
#endif /* (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_PRESENT_FOR_ALL) */
)
{

   boolean RxDMReplace = FALSE;

#if ((COM_RX_DATA_TOUT_ACTION == COM_RX_DATA_TOUT_ACTION_CONFIG))
   /* check if timeout replace is configured for the signal */
   if (COM_GET_RX_SIGNAL_EXT_TIMEOUT_ACTION(SignalExtPtr) == COM_RX_DATA_TOUT_ACTION_REPLACE)
#endif /* (COM_RX_DATA_TOUT_ACTION == COM_RX_DATA_TOUT_ACTION_CONFIG) */
   {

      uint8 RxDMTout;

#if (COM_RX_SIGNAL_BASED_DM == STD_ON)

      if ((COM_GET_RX_SIGNAL_EXT_SIGDM(SignalExtPtr) == COM_RX_SIG_DM_ON))
      {
         /* Signal based DM is configured for the signal */

         /* Check if signal timed out */

         TS_GetBit(COM_RX_SIGNAL_DM_FLAGS_BYTE_POS(SignalExtPtr),
                  COM_RX_SIGNAL_DM_TIMEOUT_BIT_POS(SignalExtPtr), uint8, RxDMTout);

      }
      else
#endif /* (COM_RX_SIGNAL_BASED_DM == STD_ON) */
      {
#if (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_PRESENT_FOR_ALL)
         /* If all signals have an update bit configured, there is no I-Pdu based DM */
         /* signal has no signal base DM */

         /* Check if I-Pdu timed out */
         TS_GetBit(&Com_gDataMemPtr[(Com_gConfigPtr->ComRxIPduStatusBase + PduId)],
                  COM_RXPDU_RXDMTIMEOUT, uint8, RxDMTout);

#endif /* (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_PRESENT_FOR_ALL) */
      }

      if (1U == RxDMTout)
      {
         RxDMReplace = TRUE;
      }

   }

   return RxDMReplace;
}                               /* Com_EB_CheckRxSignalDM */

#endif /* (COM_RX_DATA_TOUT_ACTION != COM_RX_DATA_TOUT_ACTION_NONE) */



#if (COM_FILTER_RECEIVER_ENABLE == STD_ON)

_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_Read_SignalExt_SigBuffer
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr
)
{

   Com_TS_SignalTypeType const SignalType = COM_GET_RX_SIGNAL_SIGNAL_TYPE(Com_EB_ReadPropPtr);

   switch (SignalType)
   {
   case COM_BOOLEAN:
   {
      /* Deviation MISRAC2012-1 <+2> */
      P2VAR (boolean, AUTOMATIC, COM_APPL_DATA) BoolDataPtr =
            (P2VAR (boolean, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

      CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) bufferBaseBool =
            (P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED))
            & Com_gDataMemPtr[Com_gConfigPtr->ComBufferBaseBool];
      TS_GetBit(&bufferBaseBool[(Com_EB_ReadPropPtr->ComFilterConf.ComFilterSignalValueRef / 8U)],
               Com_EB_ReadPropPtr->ComFilterConf.ComFilterSignalValueRef % 8U, uint8, *BoolDataPtr);
      break;
   }
   case COM_UINT8:
   {
      /* Deviation MISRAC2012-1 <+2> */
      P2VAR (uint8, AUTOMATIC, COM_APPL_DATA) U8DataPtr =
            (P2VAR (uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

      CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) bufferBaseUint8 =
            (P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED))
            & Com_gDataMemPtr[Com_gConfigPtr->ComBufferBaseUInt8];
      TS_AtomicAssign8(*U8DataPtr, bufferBaseUint8[Com_EB_ReadPropPtr->ComFilterConf.ComFilterSignalValueRef]);
      break;
   }
   case COM_UINT16:
   {
      /* Deviation MISRAC2012-1 <+2> */
      P2VAR (uint16, AUTOMATIC, COM_APPL_DATA) U16DataPtr =
            (P2VAR (uint16, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

      /* Deviation MISRAC2012-1 <+2> */
      CONSTP2VAR(uint16, AUTOMATIC, COM_VAR_CLEARED) bufferBaseUint16 =
            COM_GET_MEMPTR(uint16, Com_gConfigPtr->ComBufferBaseUInt16);
      TS_AtomicAssign16(*U16DataPtr, bufferBaseUint16[Com_EB_ReadPropPtr->ComFilterConf.ComFilterSignalValueRef]);
      break;
   }
   case COM_UINT32:
   {
      /* Deviation MISRAC2012-1 <+2> */
      P2VAR (uint32, AUTOMATIC, COM_APPL_DATA) U32DataPtr =
            (P2VAR (uint32, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

      /* Deviation MISRAC2012-1 <+2> */
      CONSTP2VAR(uint32, AUTOMATIC, COM_VAR_CLEARED) bufferBaseUint32 =
            COM_GET_MEMPTR(uint32, Com_gConfigPtr->ComBufferBaseUInt32);
      TS_AtomicAssign32(*U32DataPtr, bufferBaseUint32[Com_EB_ReadPropPtr->ComFilterConf.ComFilterSignalValueRef]);
      break;
   }
   case COM_SINT8:
   {
      /* Deviation MISRAC2012-1 <+2> */
      P2VAR (sint8, AUTOMATIC, COM_APPL_DATA) S8DataPtr =
            (P2VAR (sint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

      CONSTP2VAR(sint8, AUTOMATIC, COM_VAR_CLEARED) bufferBaseSint8 =
            (P2VAR(sint8, AUTOMATIC, COM_VAR_CLEARED))
            & Com_gDataMemPtr[Com_gConfigPtr->ComBufferBaseSInt8];
      TS_AtomicAssign8(*S8DataPtr, bufferBaseSint8[Com_EB_ReadPropPtr->ComFilterConf.ComFilterSignalValueRef]);
      break;
   }
   case COM_SINT16:
   {
      /* Deviation MISRAC2012-1 <+2> */
      P2VAR (sint16, AUTOMATIC, COM_APPL_DATA) S16DataPtr =
            (P2VAR (sint16, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

      /* Deviation MISRAC2012-1 <+2> */
      CONSTP2VAR(sint16, AUTOMATIC, COM_VAR_CLEARED) bufferBaseSint16 =
            COM_GET_MEMPTR(sint16, Com_gConfigPtr->ComBufferBaseSInt16);
      TS_AtomicAssign16(*S16DataPtr, bufferBaseSint16[Com_EB_ReadPropPtr->ComFilterConf.ComFilterSignalValueRef]);
      break;
   }
   case COM_SINT32:
   {
      /* Deviation MISRAC2012-1 <+2> */
      P2VAR (sint32, AUTOMATIC, COM_APPL_DATA) S32DataPtr =
            (P2VAR (sint32, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

      /* Deviation MISRAC2012-1 <+2> */
      CONSTP2VAR(sint32, AUTOMATIC, COM_VAR_CLEARED) bufferBaseSint32 =
            COM_GET_MEMPTR(sint32, Com_gConfigPtr->ComBufferBaseSInt32);
      TS_AtomicAssign32(*S32DataPtr, bufferBaseSint32[Com_EB_ReadPropPtr->ComFilterConf.ComFilterSignalValueRef]);
      break;
   }
   /* CHECK: NOPARSE */
   default:
      COM_UNREACHABLE_CODE_ASSERT(COMServiceId_ReceiveSignal);
      break;
      /* CHECK: PARSE */
   }                         /* switch (SignalType) */
}

#endif /* (COM_FILTER_RECEIVER_ENABLE == STD_ON) */


#if (COM_VALUE_INIT_REPLACE == STD_ON)

_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_Read_SignalReplace
(
   P2CONST(ComRxSignalType, AUTOMATIC, AUTOMATIC) SignalPtr
)
{

   boolean takeInitVal = FALSE;

#if ((COM_RX_SIGNAL_BASED_DM == STD_ON) || \
         (COM_RX_DATA_TOUT_ACTION == COM_RX_DATA_TOUT_ACTION_CONFIG))
   CONSTP2CONST(ComRxSignalExtType, AUTOMATIC, COM_APPL_CONST) SignalExtPtr =
            COM_GET_CONFIG_ELEMENT(ComRxSignalExtType,
                     Com_gConfigPtr->ComRxSignalExtRef, SignalPtr->ComSignalExtRef);
#endif /* ((COM_RX_SIGNAL_BASED_DM == STD_ON) || \
         (COM_RX_DATA_TOUT_ACTION == COM_RX_DATA_TOUT_ACTION_CONFIG)) */

#if (COM_FILTER_RECEIVER_ENABLE == STD_ON)
   uint8 filterType = SignalPtr->ComSignalBase.ComFilterConf.ComFilterType;
   takeInitVal = (filterType == COM_F_NEVER);
#endif /* (COM_FILTER_RECEIVER_ENABLE == STD_ON) */

#if (COM_RX_DATA_TOUT_ACTION != COM_RX_DATA_TOUT_ACTION_NONE)
   {
      boolean rxDMReplace;

      rxDMReplace = Com_EB_CheckRxSignalDM(
#if ((COM_RX_SIGNAL_BASED_DM == STD_ON) || \
         (COM_RX_DATA_TOUT_ACTION == COM_RX_DATA_TOUT_ACTION_CONFIG))
               SignalExtPtr
#if (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_PRESENT_FOR_ALL)
               ,
#endif /* (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_PRESENT_FOR_ALL) */
#endif /* ((COM_RX_SIGNAL_BASED_DM == STD_ON) || \
         (COM_RX_DATA_TOUT_ACTION == COM_RX_DATA_TOUT_ACTION_CONFIG)) */

#if (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_PRESENT_FOR_ALL)
               SignalPtr->ComIPduRef
#endif /* (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_PRESENT_FOR_ALL) */
      );

      takeInitVal = takeInitVal || rxDMReplace;
   }
#endif /* (COM_RX_DATA_TOUT_ACTION != COM_RX_DATA_TOUT_ACTION_NONE) */

   return takeInitVal;
}

#endif /* (COM_VALUE_INIT_REPLACE == STD_ON) */

FUNC(void, COM_CODE) Com_EB_Read_SignalExt
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
)
{

#if ((COM_VALUE_INIT_REPLACE == STD_ON) || (COM_FILTER_RECEIVER_ENABLE == STD_ON))

   CONSTP2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr =
            (P2CONST(ComRxSignalType, AUTOMATIC, AUTOMATIC))
            ((P2CONST(void, AUTOMATIC, AUTOMATIC)) Com_EB_ReadPropPtr);


#if (COM_VALUE_INIT_REPLACE == STD_ON)
   boolean const takeInitVal =
            Com_EB_Read_SignalReplace(SignalPtr);


   if (takeInitVal == TRUE)
   {
      /* read initial value */

      ComIPduRefType const PduId = SignalPtr->ComIPduRef;

      CONSTP2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr =
               COM_GET_CONFIG_ELEMENT(ComRxIPduType, Com_gConfigPtr->ComRxIPduRef, PduId);

      CONSTP2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) initDataPtr =
               COM_GET_CONFIG_ELEMENT(uint8, IPduPtr->ComInitValueRef, 0U);

      Com_EB_Read_Core_NoLock(SignalDataPtr,initDataPtr,Com_EB_ReadPropPtr, retvalPtr);

   }
   else
#endif /* (COM_VALUE_INIT_REPLACE == STD_ON) */
   {
      /* read last received value */
#if (COM_FILTER_RECEIVER_ENABLE == STD_ON)
      if(Com_EB_ReadPropPtr->ComFilterConf.ComFilterSignalValueRef != COM_SIGNAL_BUFFER_REF_INVALID)
      {
         /* read value from signal buffer */
         Com_EB_Read_SignalExt_SigBuffer(SignalDataPtr,Com_EB_ReadPropPtr);
      }
      else
#endif /* (COM_FILTER_RECEIVER_ENABLE == STD_ON) */
      {
         /* read value from buffer */
         Com_EB_Read_Core(SignalDataPtr,dataPtr,Com_EB_ReadPropPtr, retvalPtr);
      }
   }

#else

   TS_PARAM_UNUSED(retvalPtr);
   TS_PARAM_UNUSED(SignalDataPtr);
   TS_PARAM_UNUSED(dataPtr);
   TS_PARAM_UNUSED(Com_EB_ReadPropPtr);

#endif /* ((COM_VALUE_INIT_REPLACE == STD_ON) || (COM_FILTER_RECEIVER_ENABLE == STD_ON)) */

}                               /* Com_EB_Read_SignalExt */

_COM_STATIC_ FUNC_P2CONST(ComRxSignalType, COM_APPL_CONST, AUTOMATIC) Com_GetRxSignalCfg
(
   Com_SignalIdType SignalId,
   uint8 ServiceId
)
{

   P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr = NULL_PTR;

   TS_PARAM_UNUSED(ServiceId);

#if (COM_DEV_ERROR_DETECT == STD_ON)

   /* check whether Com is initialized */
   if (Com_InitStatus == COM_EB_UNINIT)
   {
      COM_DET_REPORTERROR(COM_E_UNINIT, ServiceId);
   }
   /* Check if the SignalId is within the range of the ComRxSignalArraySize */
   else if (Com_gConfigPtr->ComRxSignalArraySize <= SignalId)
   {
      COM_DET_REPORTERROR(COM_E_PARAM, ServiceId);
   }
   else
#endif /* COM_DEV_ERROR_DETECT */

   {
      /* get the configuration of the signal identified by the signal Id and check the
       * configuration */
      /* pointer to the configuration of the signal */
      SignalPtr =
               COM_GET_CONFIG_ELEMENT(ComRxSignalType, Com_gConfigPtr->ComRxSignalRef, SignalId);
   }

   return SignalPtr;

}                               /* Com_GetRxSignalCfg */


#if ((COM_GENERATED_RCV_SIG_ENABLE == STD_ON) && \
(COM_MAP_RECEIVE_SIGNAL == COM_RECEIVE_SIGNAL_GENERATED))
/** \brief Com_ReceiveSignalGeneric - get a signal's actual value from COM
 * This function returns the actual value of a signal.
 * This function is only available if the configuration parameter
 * ComGeneratedRcvSigEnable is enabled and
 * ComMapReceiveSignal == Com_ReceiveSignalGenerated.
 * If available, this function maps to the generic implementation of the function.
 * Preconditions:
 * - COM must be initialized
 * \param[in] SignalId - ID of signal to receive
 * \param[in] SignalDataPtr - piece of memory to copy data to
 * \return Function execution success status
 * \retval E_OK - success
 * \retval COM_SERVICE_NOT_AVAILABLE - Com is disabled, no valid value passed over
 */
FUNC(uint8, COM_CODE) Com_ReceiveSignalGeneric
(
   Com_SignalIdType SignalId,
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
)
#else
/** \brief Com_ReceiveSignal - get a signal's actual value from COM
 * This function returns the actual value of a signal.
 * This function is always available.
 * If the configuration parameter ComGeneratedRcvSigEnable is disabled,
 * this function maps to the generic implementation of the function.
 * If the configuration parameter ComGeneratedRcvSigEnable is enabled
 * and ComMapReceiveSignal == Com_ReceiveSignalGeneric,
 * this function maps to the generic implementation of the function.
 * If the configuration parameter ComGeneratedRcvSigEnable is enabled
 * and ComMapReceiveSignal == Com_ReceiveSignalGenerated,
 * this function maps to the generated implementation of the function.
 * Preconditions:
 * - COM must be initialized
 * \param[in] SignalId - ID of signal to receive
 * \param[out] SignalDataPtr - piece of memory to copy data to
 * \return Function execution success status
 * \retval E_OK - success
 * \retval COM_SERVICE_NOT_AVAILABLE - Com is disabled, no valid value passed over
 */
FUNC(uint8, COM_CODE) Com_ReceiveSignal
(
   Com_SignalIdType SignalId,
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
)
#endif /* #if ((COM_GENERATED_RCV_SIG_ENABLE == STD_ON) && \
(COM_MAP_RECEIVE_SIGNAL == COM_RECEIVE_SIGNAL_GENERATED)) */
{
   uint8 retval = COM_SERVICE_NOT_AVAILABLE;

   /* get configuration of signal */
   CONSTP2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr =
            Com_GetRxSignalCfg(SignalId, COMServiceId_ReceiveSignal);

   DBG_COM_RECEIVESIGNALGENERIC_ENTRY(SignalId,SignalDataPtr);

#if (COM_DEV_ERROR_DETECT == STD_ON)
   /* check if a valid signal config could be determined */
   if (SignalPtr == NULL_PTR)
   {
      /* DET was already reported, nothing to do here */
   }
   /* check SignalDataPtr */
   else if (SignalDataPtr == NULL_PTR)
   {
      COM_DET_REPORTERROR(COM_E_PARAM_POINTER, COMServiceId_ReceiveSignal);
   }
   else if( COM_UINT8_DYN == COM_GET_RX_SIGNAL_SIGNAL_TYPE(&SignalPtr->ComSignalBase) )
   {
      COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_ReceiveSignal);
   }
#if (COM_BASED_TRANSFORMER_SUPPORT_RX == STD_ON)
   else if (SignalPtr->ComSignalBase.ComBufferRef == COM_RAM_SIZE_MAX)
   {
      /* the signal group has no shadow buffer assigned an shall
       * only be used with the Com_ReceiveSignalGroupArray() */
      COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_ReceiveSignal);
   }
#endif /* (COM_BASED_TRANSFORMER_SUPPORT_RX == STD_ON) */

   else

#endif /* COM_DEV_ERROR_DETECT */
   {
      /* get the IPdu Id to which the signal belongs */
      ComIPduRefType const PduId = SignalPtr->ComIPduRef;

      CONSTP2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr =
               &Com_gDataMemPtr[SignalPtr->ComSignalBase.ComBufferRef];

      if ( Com_IsBitSet_8(&Com_gDataMemPtr[(Com_gConfigPtr->ComRxIPduStatusBase + PduId)],
          COM_RXPDU_STARTED) )
      {
         retval = E_OK;
      }

      /* get the value of the using the library */
      Com_EB_Read_Ext(SignalDataPtr,dataPtr,&SignalPtr->ComSignalBase, &retval);

   }

   DBG_COM_RECEIVESIGNALGENERIC_EXIT(retval,SignalId,SignalDataPtr);
   return retval;
}

/**
 * \brief Com_ReceiveDynSignal - get a dynamic length signal's actual value from COM
 *
 * Com_ReceiveDynSignal copies the data of the signal identified by SignalId to
 * the location specified by SignalDataPtr and stores the length of the
 * dynamic length signal at the position given by the Length parameter.
 *
 * Preconditions:
 * - COM must be initialized
 *
 * \param[in] SignalId - ID of signal to receive
 * \param[in] SignalDataPtr - piece of memory to copy data to
 * \param[in,out] LengthPtr - in: maximum length that could be received
 *                            out: length of the dynamic length signal
 * \return Result of operation
 * \retval E_OK - success
 * \retval E_NOT_OK - the Length (as in-parameter) is smaller than the received
 *            length of the dynamic length signal
 * \retval COM_SERVICE_NOT_AVAILABLE - corresponding I-PDU group was stopped
 * (or service failed due to development error)
 */
FUNC(uint8, COM_CODE) Com_ReceiveDynSignal
(
   Com_SignalIdType SignalId,
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint16, AUTOMATIC, COM_APPL_DATA) LengthPtr
)
{
   uint8 retval = COM_SERVICE_NOT_AVAILABLE;

   /* get configuration of signal */
   CONSTP2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr =
            Com_GetRxSignalCfg(SignalId, COMServiceId_ReceiveDynSignal);

   DBG_COM_RECEIVEDYNSIGNAL_ENTRY(SignalId,SignalDataPtr,LengthPtr);

#if (COM_RX_DYN_LENGTH_IPDU_ENABLE == STD_ON)

#if (COM_DEV_ERROR_DETECT == STD_ON)

   /* check if a valid signal config could be determined */
   if (SignalPtr == NULL_PTR)
   {
      /* DET was already reported, nothing to do here */
   }
   /* check SignalDataPtr */
   else if (SignalDataPtr == NULL_PTR)
   {
      COM_DET_REPORTERROR(COM_E_PARAM_POINTER, COMServiceId_ReceiveDynSignal);
   }
   /* check SignalDataPtr */
   else if (LengthPtr == NULL_PTR)
   {
      COM_DET_REPORTERROR(COM_E_PARAM_POINTER, COMServiceId_ReceiveDynSignal);
   }
   else if ( COM_UINT8_DYN != COM_GET_RX_SIGNAL_SIGNAL_TYPE(&SignalPtr->ComSignalBase) )
   {
      COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_ReceiveDynSignal);
   }
   else
#endif /* COM_DEV_ERROR_DETECT */
   {
      /* get the IPdu Id to which the signal belongs */
      ComIPduRefType const PduId = SignalPtr->ComIPduRef;

      /* get IPdu to which the signal belongs and check if the IPdu is valid */
      CONSTP2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr =
               COM_GET_CONFIG_ELEMENT(ComRxIPduType, Com_gConfigPtr->ComRxIPduRef, PduId);

#if ( (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) && (COM_DEV_ERROR_DETECT == STD_ON) )
      if(TRUE == COM_IS_RX_DUMMY_SIGNAL(SignalPtr->ComSignalBase))
      {
        /* signal is a dummy signal */
        retval = COM_SERVICE_NOT_AVAILABLE;
        COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_ReceiveDynSignal);
      }
      else
#endif /* ( (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) && (COM_DEV_ERROR_DETECT == STD_ON) ) */
      {
        retval = Com_EB_ReadValue_Uint8Dyn( SignalDataPtr, LengthPtr, SignalPtr, IPduPtr, PduId);
        if (retval == E_OK)
        {
           if ( !(Com_IsBitSet_8(&Com_gDataMemPtr[(Com_gConfigPtr->ComRxIPduStatusBase + PduId)],
               COM_RXPDU_STARTED)) )
           {
              retval = COM_SERVICE_NOT_AVAILABLE;
           }

        }
      }

   }
#else
   TS_PARAM_UNUSED(SignalId);
   TS_PARAM_UNUSED(SignalDataPtr);
   TS_PARAM_UNUSED(LengthPtr);
   TS_PARAM_UNUSED(SignalPtr);

#if (COM_DEV_ERROR_DETECT == STD_ON)
  COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_ReceiveDynSignal);
#endif /* (COM_DEV_ERROR_DETECT == STD_ON) */

#endif /* (COM_RX_DYN_LENGTH_IPDU_ENABLE == STD_ON) */

   DBG_COM_RECEIVEDYNSIGNAL_EXIT(retval,SignalId,SignalDataPtr,LengthPtr);
   return retval;
}

FUNC(Com_SignalGroupIdType, COM_CODE) Com_ComXf_GetRxSGId
(
   Com_SignalIdType GroupSignalId
)
{
   Com_SignalGroupIdType SGId = 0xFFFFU;

   /* get configuration of signal */
   CONSTP2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr =
            Com_GetRxSignalCfg(GroupSignalId, COMServiceId_InternalAPI);

#if (COM_DEV_ERROR_DETECT == STD_ON)

   /* check if a valid signal config could be determined */
   if (SignalPtr == NULL_PTR)
   {
      /* DET was already reported, nothing to do here */
   }
   else
#endif /* (COM_DEV_ERROR_DETECT == STD_ON) */
   {
      SGId = SignalPtr->ComSignalBase.ComSignalGroupRef;
   }

   return SGId;

}                               /* Com_ComXf_GetRxSGId */

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
#endif

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/
