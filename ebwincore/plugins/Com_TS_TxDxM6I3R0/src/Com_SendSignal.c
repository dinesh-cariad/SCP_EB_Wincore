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


/*==================[inclusions]=============================================*/

#include <Com_Trace.h>
#include <TSAutosar.h>          /* EB specific standard types */
#include <ComStack_Types.h>     /* AUTOSAR standard types */
#include <Com_Api_Static.h>     /* module public static API */
#include <Com_Priv.h>           /* module internal stuff */
#include <Com_Defs.h>           /* COM_SERVICE_NOT_AVAILABLE */

#include <Com_Core_TxSignal.h>  /* signal functions */

#include <Com_ComXf.h>          /* APIs for EB ComXf */

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

_COM_STATIC_ FUNC_P2CONST(ComTxSignalType, COM_APPL_CONST, AUTOMATIC) Com_GetTxSignalCfg
(
   Com_SignalIdType SignalId,
   uint8 ServiceId
);

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/** \brief Com_SendSignal - send a signal see COM197
 * The service Com_SendSignal updates the signal object identified by SignalId
 * with the signal referenced by the SignalDataPtr parameter.
 * Preconditions:
 * COM must be initialized.
 * \param[in] SignalId - ID of the signal to be sent
 * \param[in] SignalDataPtr - place in memory to copy the data from
 * \return Function execution success status
 * \retval E_OK - success
 * \retval COM_SERVICE_NOT_AVAILABLE - failure (Com not initialized,
 * corresponding I-PDU group was stopped or
 * service failed due to development error)
 * \retval COM_BUSY - The receive Tp buffer is actually not available
 */
FUNC(uint8, COM_CODE) Com_SendSignal
(
   Com_SignalIdType SignalId,
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
)
{
   uint8 RetVal = COM_SERVICE_NOT_AVAILABLE;

   CONSTP2CONST(ComTxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr =
            Com_GetTxSignalCfg(SignalId, COMServiceId_SendSignal);

   DBG_COM_SENDSIGNAL_ENTRY(SignalId,SignalDataPtr);

#if (COM_DEV_ERROR_DETECT == STD_ON)

   /* check if a valid signal config could be determined */
   if (SignalPtr == NULL_PTR)
   {
      /* DET was already reported, nothing to do here */
   }
   /* check SignalDataPtr */
   else if (SignalDataPtr == NULL_PTR)
   {
      COM_DET_REPORTERROR(COM_E_PARAM_POINTER, COMServiceId_SendSignal);
   }
   /* check that signal type is not a dynamic array */
   else if( COM_UINT8_DYN == COM_GET_TX_SIGNAL_SIGNAL_TYPE(SignalPtr) )
   {
      COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_SendSignal);
   }
#if (COM_BASED_TRANSFORMER_SUPPORT_TX == STD_ON)
   else if (SignalPtr->ComSignalBase.ComBufferRef == COM_RAM_SIZE_MAX)
   {
     /* the signal group has no shadow buffer assigned an shall
      * only be used with the Com_SendSignalGroupArray() */
     COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_SendSignal);
   }
#endif /* (COM_BASED_TRANSFORMER_SUPPORT_TX == STD_ON) */

   else

#endif /* COM_DEV_ERROR_DETECT */
   {

     CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr =
         &Com_gDataMemPtr[SignalPtr->ComSignalBase.ComBufferRef];

     RetVal = Com_GetSendSignalRetVal(SignalPtr);

     {
       Com_EB_WriteIntDataType IntData;

       /* in case it is a byte array this size is used for writing data to buffer */
       /* in case signal is a zero size signal, serialization lib is not called */
       COM_INIT_Com_EB_WriteIntDataType(IntData, SignalPtr->ComSignalBase.ComBitSize, FALSE,
                                        RetVal, FALSE, FALSE, E_NOT_OK);

       /* set the value of the using the library */
       Com_EB_Write_Ext(SignalDataPtr,dataPtr,SignalPtr,&IntData);

#if (COM_TX_TPAPI_ENABLE == STD_ON)
       RetVal = IntData.SendSigRetval;
#endif /* #if (COM_TX_TPAPI_ENABLE == STD_ON) */

     }

   }

   DBG_COM_SENDSIGNAL_EXIT(RetVal,SignalId,SignalDataPtr);
   return RetVal;
}


/**
 * \brief Com_SendDynSignal - send a dynamic length signal
 *
 * The service Com_SendSignal updates the signal object identified by SignalId and
 * from signal type UINT8_DYN with the value referenced by the SignalDataPtr parameter.
 * Preconditions:
 * COM must be initialized.
 *
 * \param[in] SignalId - ID of the signal to be sent
 * \param[in] SignalDataPtr - place in memory to copy the data from
 * \param[in] Length - Length of the dynamic length signal to be send
 * \return Result of operation
 * \retval E_OK - success
 * \retval E_NOT_OK - in case the Length is greater than the configured ComSignalLength
 * of this sent signal
 * \retval COM_SERVICE_NOT_AVAILABLE - failure (Com not initialized,
 * corresponding I-PDU group was stopped or
 * service failed due to development error)
 * \retval COM_BUSY - The receive Tp buffer is actually not available
 *
 * \ServiceID{0x21}
 * \Reentrancy{Re-entrant for different values of SignalId only.}
 * \Synchronicity{Asynchronous}
 */
FUNC(uint8, COM_CODE) Com_SendDynSignal
(
   Com_SignalIdType SignalId,
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   uint16 Length
)
{
  uint8 RetVal = COM_SERVICE_NOT_AVAILABLE;

  CONSTP2CONST(ComTxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr =
      Com_GetTxSignalCfg(SignalId, COMServiceId_SendDynSignal);

  DBG_COM_SENDDYNSIGNAL_ENTRY(SignalId,SignalDataPtr,Length);

#if (COM_TX_DYN_LENGTH_IPDU_ENABLE == STD_ON)

#if (COM_DEV_ERROR_DETECT == STD_ON)

  /* check if a valid signal config could be determined */
  if (SignalPtr == NULL_PTR)
  {
    /* DET was already reported, nothing to do here */
  }
  /* check SignalDataPtr */
  else if (SignalDataPtr == NULL_PTR)
  {
    COM_DET_REPORTERROR(COM_E_PARAM_POINTER, COMServiceId_SendDynSignal);
  }
  /* check that signal type is a dynamic array */
  else if( COM_UINT8_DYN != COM_GET_TX_SIGNAL_SIGNAL_TYPE(SignalPtr) )
  {
    COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_SendDynSignal);
  }
  /* check that signal value fits into signal */
  else
#endif /* COM_DEV_ERROR_DETECT */
  {
    if( Length > SignalPtr->ComSignalBase.ComBitSize )
    {
#if (COM_DEV_ERROR_DETECT == STD_ON)
      COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_SendDynSignal);
#endif /* COM_DEV_ERROR_DETECT */
      RetVal = E_NOT_OK;
    }
    else
    {

      RetVal = Com_GetSendSignalRetVal(SignalPtr);

#if ( (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) && (COM_DEV_ERROR_DETECT == STD_ON) )
      if(TRUE == COM_IS_TX_DUMMY_SIGNAL(SignalPtr))
      {
        /* dyn signal is a dummy dyn signal */
        RetVal = COM_SERVICE_NOT_AVAILABLE;

        COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_SendDynSignal);
      }
      else
#endif /* ( (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) && (COM_DEV_ERROR_DETECT == STD_ON) ) */
      {
        /* dyn signal is not a dummy dyn signal */

        Com_EB_WriteIntDataType IntData;

        COM_INIT_Com_EB_WriteIntDataType(IntData, Length, FALSE,
                                         RetVal, FALSE, FALSE, E_NOT_OK);

        {
#if (COM_TX_TPAPI_ENABLE == STD_ON)

          /* Id of the IPdu to which the signal belongs */
          ComIPduRefType const PduId = SignalPtr->ComIPduRef;

          /* pointer to the configuration of the IPdu to which the signal belongs */
          CONSTP2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr =
              COM_GET_CONFIG_ELEMENT(ComTxIPduType, Com_gConfigPtr->ComTxIPduRef, PduId);

          /* ref to TP Control struct exists for this IPdu */
          if( (IPduPtr->ComTxIpduTPControlInfoRef != COM_IPDU_ARRAY_SIZE_MAX) )
          {

            boolean IsLocked;

            /* get TP Control array of struct base */
            CONSTP2VAR(EcuC_TxControlInfoType, AUTOMATIC, COM_VAR_CLEARED) TpTxContPtrBase =
                COM_GET_MEMPTR(EcuC_TxControlInfoType, Com_gConfigPtr->ComTxIpduTPControlInfoBase);

            /* get TP Control struct value pointer */
            CONSTP2VAR(EcuC_TxControlInfoType, AUTOMATIC, COM_VAR_CLEARED) TpTxContPtrValPtr =
                &TpTxContPtrBase[IPduPtr->ComTxIpduTPControlInfoRef];

            /* enter critical section */
            Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

            /* if no lock -> write */
            IsLocked = (boolean) EcuC_TxBufferIsLocked(TpTxContPtrValPtr);

            if(FALSE == IsLocked)
            {
              /* Write the value to the buffer and exit critical section */
              Com_EB_WriteDynSignal_ExitArea_0(SignalDataPtr,SignalPtr,&IntData);
            }
            else
            {
              /* exit critical section */
              Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

              RetVal = COM_BUSY;
            }

          }
          else
#endif /* (COM_TX_TPAPI_ENABLE == STD_ON) */
          {

            /* enter critical section */
            Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

            /* Write the value to the buffer and exit critical section */
            Com_EB_WriteDynSignal_ExitArea_0(SignalDataPtr,SignalPtr,&IntData);

          }
        }
      }
    }
  }

#else
  TS_PARAM_UNUSED(SignalId);
  TS_PARAM_UNUSED(SignalDataPtr);
  TS_PARAM_UNUSED(Length);
  TS_PARAM_UNUSED(SignalPtr);

#if (COM_DEV_ERROR_DETECT == STD_ON)
  COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_SendDynSignal);
#endif /* (COM_DEV_ERROR_DETECT == STD_ON) */

#endif /* (COM_TX_DYN_LENGTH_IPDU_ENABLE == STD_ON) */

  DBG_COM_SENDDYNSIGNAL_EXIT(RetVal,SignalId,SignalDataPtr,Length);
  return RetVal;
}

FUNC(Com_SignalGroupIdType, COM_CODE) Com_ComXf_GetTxSGId
(
   Com_SignalIdType GroupSignalId
)
{
   Com_SignalGroupIdType SGId = 0xFFFFU;

   /* get configuration of signal */
   CONSTP2CONST(ComTxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr =
            Com_GetTxSignalCfg(GroupSignalId, COMServiceId_InternalAPI);

#if (COM_DEV_ERROR_DETECT == STD_ON)
   /* check if a valid signal config could be determined */
   if (SignalPtr != NULL_PTR)
#endif /* (COM_DEV_ERROR_DETECT == STD_ON) */
   {
      SGId = SignalPtr->ComSignalBase.ComSignalGroupRef;
   }

   return SGId;

}                               /* Com_ComXf_GetTxSGId */

/*==================[internal function definitions]==========================*/

_COM_STATIC_ FUNC_P2CONST(ComTxSignalType, COM_APPL_CONST, AUTOMATIC) Com_GetTxSignalCfg
(
   Com_SignalIdType SignalId,
   uint8 ServiceId
)
{

   P2CONST(ComTxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr = NULL_PTR;

   TS_PARAM_UNUSED(ServiceId);

#if (COM_DEV_ERROR_DETECT == STD_ON)

   /* check whether Com is initialized */
   if (Com_InitStatus == COM_EB_UNINIT)
   {
      COM_DET_REPORTERROR(COM_E_UNINIT, ServiceId);
   }
   /* Check if the SignalId is within the range of the ComTxSignalArraySize */
   else if (Com_gConfigPtr->ComTxSignalArraySize <= SignalId)
   {
      COM_DET_REPORTERROR(COM_E_PARAM, ServiceId);
   }
   else
#endif /* (COM_DEV_ERROR_DETECT == STD_ON) */
   {

      /* get the configuration of the signal identified by the signal Id and check the
       * configuration */
      SignalPtr =
               COM_GET_CONFIG_ELEMENT(ComTxSignalType, Com_gConfigPtr->ComTxSignalRef, SignalId);
   }

   return SignalPtr;
}                               /* Com_GetTxSignalCfg */


#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
#endif

/*==================[end of file]============================================*/
