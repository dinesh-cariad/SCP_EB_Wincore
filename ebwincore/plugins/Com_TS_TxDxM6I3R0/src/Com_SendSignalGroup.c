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

#include <Com_Core_TxSignalGroup.h>  /* (group)signal(group) functions */

#include <Com_ComXf.h>          /* APIs for EB ComXf */

#if (COM_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>                /* Det API */
#endif

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_START_SEC_CODE
#include <Com_MemMap.h>
#endif

/** \brief Com_GetTxSignalGroupCfg - returns the pointer to the configuration of a
 * Tx signal groups and provides DET checks
 * Preconditions:
 * \param[in] SignalGroupId - ID of signal group to be accessed.
 * \param[in] ServiceId - Id which shall be used for DET calls
 * \return Function execution success status
 * \retval pointer to configuration in case of success
 * \retval NULL_PTR in case of failure
 */
_COM_STATIC_ FUNC_P2CONST(ComTxSignalGroupType, COM_APPL_CONST, AUTOMATIC) Com_GetTxSignalGroupCfg
(
   Com_SignalGroupIdType SignalGroupId,
   uint8 ServiceId
);

/** \brief Com_SetupTxSignalGroup_DET - retrieves configuration data for in order to copy
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
 * \param[in] SignalGroupId - Id of the signal group which shall be accessed
 * \param[in] ServiceId - Id which shall be used for DET calls
 * \return Function execution success status
 * \retval E_OK - success
 * \retval COM_SERVICE_NOT_AVAILABLE - failure (Com not initialized,
 * corresponding I-PDU group was stopped or
 * service failed due to development error)
 */
_COM_STATIC_ FUNC(uint8, COM_CODE) Com_SetupTxSignalGroup_DET
(
   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) SGArrayPtr,
   uint16 SGArrayLength,
   P2VAR(Com_TxSignalGroupFPtrType, AUTOMATIC, COM_CODE) Com_TxSignalGroupFPtr,
   Com_SignalGroupIdType SignalGroupId,
   uint8 ServiceId
);

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/


/** \brief Com_GetTxSignalGroupCfg - returns the pointer to the configuration of a
 * Tx signal groups and provides DET checks
 * Preconditions:
 * \param[in] SignalGroupId - ID of signal group to be accessed.
 * \param[in] ServiceId - Id which shall be used for DET calls
 * \return Function execution success status
 * \retval pointer to configuration in case of success
 * \retval NULL_PTR in case of failure
 */
_COM_STATIC_ FUNC_P2CONST(ComTxSignalGroupType, COM_APPL_CONST, AUTOMATIC) Com_GetTxSignalGroupCfg
(
   Com_SignalGroupIdType SignalGroupId,
   uint8 ServiceId
)
{

   P2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr = NULL_PTR;

   TS_PARAM_UNUSED(ServiceId);

#if (COM_DEV_ERROR_DETECT == STD_ON)

   /* check whether Com is initialized */
   if (Com_InitStatus == COM_EB_UNINIT)
   {
      COM_DET_REPORTERROR(COM_E_UNINIT, ServiceId);
   }
   /* Check if the SignalGroupId is within the range of the ComTxSignalGroupArraySize */
   else if (Com_gConfigPtr->ComTxSignalGroupArraySize <= SignalGroupId)
   {
      COM_DET_REPORTERROR(COM_E_PARAM, ServiceId);
   }
   else

#endif /* COM_DEV_ERROR_DETECT */

   {
      /* get the configuration of the signal group identified by the SignalGroupId and check the
       * configuration */
      SignalGPtr =
            COM_GET_CONFIG_ELEMENT(ComTxSignalGroupType, Com_gConfigPtr->ComTxSignalGroupRef,
                  SignalGroupId);

#if (COM_PRECONDITION_ASSERT_ENABLED == STD_ON)
      /* CHECK: NOPARSE */
      if (NULL_PTR == SignalGPtr)
      {
         COM_PRECONDITION_ASSERT(
               (NULL_PTR == SignalGPtr),
               ServiceId);
      }
      else if (SignalGPtr->ComConfigCheckData != COM_CONFIG_CHECK_TX_SIG_GROUP)
      {
         COM_PRECONDITION_ASSERT(
               (SignalGPtr->ComConfigCheckData == COM_CONFIG_CHECK_TX_SIG_GROUP),
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

      /* check if dummy signal group */
      if (TRUE == COM_GET_TX_SIGNAL_GROUP_DUMMY(SignalGPtr))
      {
         COM_DET_REPORTERROR(COM_E_PARAM, ServiceId);
         SignalGPtr = NULL_PTR;
      }

#endif /* ( (COM_DEV_ERROR_DETECT == STD_ON) && (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) ) */

   }

   return SignalGPtr;
}

/** \brief Com_SetupTxSignalGroup_DET - retrieves configuration data for in order to copy
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
 * \param[in] SignalGroupId - Id of the signal group which shall be accessed
 * \param[in] ServiceId - Id which shall be used for DET calls
 * \return Function execution success status
 * \retval E_OK - success
 * \retval COM_SERVICE_NOT_AVAILABLE - failure (Com not initialized,
 * corresponding I-PDU group was stopped or
 * service failed due to development error)
 */
_COM_STATIC_ FUNC(uint8, COM_CODE) Com_SetupTxSignalGroup_DET
(
   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) SGArrayPtr,
   uint16 SGArrayLength,
   P2VAR(Com_TxSignalGroupFPtrType, AUTOMATIC, COM_CODE) Com_TxSignalGroupFPtr,
   Com_SignalGroupIdType SignalGroupId,
   uint8 ServiceId
)
{
   uint8 retval = COM_SERVICE_NOT_AVAILABLE;

   /* pointer to the configuration of the signal group */
   CONSTP2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr =
            Com_GetTxSignalGroupCfg(SignalGroupId, ServiceId);


#if (COM_DEV_ERROR_DETECT)
   /* check if a valid signal config could be determined */
   if (SignalGPtr != NULL_PTR)
#endif /* COM_DEV_ERROR_DETECT */
   {

      Com_SetupTxSignalGroup(
               SGArrayPtr,
               SGArrayLength,
               Com_TxSignalGroupFPtr,
               SignalGPtr,
               &retval
#if ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) )
               ,FALSE
#endif /* ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) ) */
      );

   }

   return retval;
}                               /* Com_SetupTxSignalGroup_DET */


FUNC(uint8, COM_CODE) Com_SendSignalGroup
(
   Com_SignalGroupIdType SignalGroupId
)
{
   uint8 retval = COM_SERVICE_NOT_AVAILABLE;

   DBG_COM_SENDSIGNALGROUP_ENTRY(SignalGroupId);

   retval = Com_SetupTxSignalGroup_DET(
         NULL_PTR,
         0U,
         &Com_EB_SendSignalGroup,
         SignalGroupId,
         COMServiceId_SendSignalGroup
   );

   DBG_COM_SENDSIGNALGROUP_EXIT(retval,SignalGroupId);
   return retval;
}                               /* Com_SendSignalGroup */


FUNC(uint8, COM_CODE) Com_SendSignalGroupArray
(
   Com_SignalGroupIdType SignalGroupId,
   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) SignalGroupArrayPtr,
   uint16 SignalGroupArrayLength
)
{
   uint8 retval = COM_SERVICE_NOT_AVAILABLE;

   DBG_COM_SENDSIGNALGROUPARRAY_ENTRY(SignalGroupId,SignalGroupArrayPtr,SignalGroupArrayLength);

#if (COM_BASED_TRANSFORMER_SUPPORT_TX == STD_ON)

   retval = Com_SetupTxSignalGroup_DET(
         SignalGroupArrayPtr,
         SignalGroupArrayLength,
         &Com_EB_SendSignalGroupArray_DETCheck,
         SignalGroupId,
         COMServiceId_SendSignalGroupArray
   );

#else

   TS_PARAM_UNUSED(SignalGroupId);
   TS_PARAM_UNUSED(SignalGroupArrayPtr);
   TS_PARAM_UNUSED(SignalGroupArrayLength);

#if (COM_DEV_ERROR_DETECT == STD_ON)
   COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_SendSignalGroupArray);
#endif /* (COM_DEV_ERROR_DETECT == STD_ON) */

#endif /* (COM_BASED_TRANSFORMER_SUPPORT_TX == STD_ON) */


   DBG_COM_SENDSIGNALGROUPARRAY_EXIT(retval,SignalGroupId,SignalGroupArrayPtr,SignalGroupArrayLength);
   return retval;
}                               /* Com_SendSignalGroupArray */

FUNC(uint16, COM_CODE) Com_ComXf_TxPrepareBuffer
(
   P2VAR(uint8, AUTOMATIC, COMXF_APPL_DATA) buffer,
   Com_SignalGroupIdType SignalGroupId,
   boolean Initialize
)
{

   uint16 length = 0U;

   /* pointer to the configuration of the signal group */
   CONSTP2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr =
            Com_GetTxSignalGroupCfg(SignalGroupId, COMServiceId_InternalAPI);

#if (COM_DEV_ERROR_DETECT)
   /* check if a valid signal config could be determined */
   if (SignalGPtr != NULL_PTR)
#endif /* COM_DEV_ERROR_DETECT */
   {

      if (Initialize == TRUE)
      {

         /* buffer has to be initialized */

         /* get the IPdu Id to which the signal group belongs */
         ComIPduRefType const PduId = SignalGPtr->ComIPduRef;

         /* get IPdu to which the signal belongs and check if the IPdu is valid */
         CONSTP2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr =
                  COM_GET_CONFIG_ELEMENT(ComTxIPduType, Com_gConfigPtr->ComTxIPduRef, PduId);

         /* copy the value of the I-Pdu to the signal group array */
         length = Com_EB_InitSGBuffer(buffer,
                  SignalGPtr,
                  IPduPtr);

      }

   }

   return length;
}

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
#endif

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/
