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
 * MISRAC2012-2) Deviated Rule: 11.3 (required)
 * A cast shall not be performed between a pointer to object
 * type and a pointer to a different object type.
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
#include <Com_Types.h>          /* types of configuration elements */
#include <Com_Filter.h>         /* Filter functions */
#include <Com_Core_RxIPdu.h>    /* RxIPdu functions */

#include <Com_Rules.h>

#include <Com_Core_TxSignalGroup.h>  /* (group)signal(group) functions */

#include <Com_Core_Read.h>
#include <Com_Core_Write.h>

#if (COM_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>                /* Det API */
#endif /* (COM_DEV_ERROR_DETECT == STD_ON) */


#if ((COM_TM_AVAILABLE == STD_ON) || (COM_TMS_ENABLE == STD_ON))
#include <Com_Core_TM.h> /* Transmission Mode functions */
#endif /* ((COM_TM_AVAILABLE == STD_ON) || (COM_TMS_ENABLE == STD_ON)) */

#include <Com_Core_TxSignalGroup.h>  /* (group)signal(group) functions */
#include <Com_Core_TxSignal.h>  /* signal functions */

#if (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON)
#include <TSMem.h>  /* TS_MemCpy */
#endif /* (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON) */

/*==================[macros]=================================================*/

#if (defined COM_EB_GET_SRCVALUE_REF)        /* To prevent double declaration */
#error COM_EB_GET_SRCVALUE_REF already defined
#endif /* #if (defined COM_EB_GET_SRCVALUE_REF) */

/** \brief COM_EB_GET_SRCVALUE_REF
 * returns the reference to the I-Pdu buffer of the Rx I-Pdu
 * \param[in] IPduPtr - reference to the configuration of the source I-Pdu
 * \return
 * \retval reference to the I-Pdu buffer
 */
#define COM_EB_GET_SRCVALUE_REF(IPduPtr) (IPduPtr)->ComIPduValueRef


#if (defined COM_EB_MULTIGW_LENGTHTYPE_SIZE)        /* To prevent double declaration */
#error COM_EB_MULTIGW_LENGTHTYPE_SIZE already defined
#endif /* #if (defined COM_EB_MULTIGW_LENGTHTYPE_SIZE) */

/** \brief COM_EB_MULTIGW_LENGTHTYPE_SIZE
 * returns size of the byets reserved for the length of the Rx-IPdu
 * \return
 * \retval size in bytes of reserved bytes for Rx-IPdu length
 */
#if (COM_RX_DYN_LENGTH_IPDU_ENABLE == STD_ON) && (COM_TX_DYN_LENGTH_IPDU_ENABLE == STD_ON)
#define COM_EB_MULTIGW_LENGTHTYPE_SIZE() sizeof(ComIPduSizeType)
#else
#define COM_EB_MULTIGW_LENGTHTYPE_SIZE() 0U
#endif

#if (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON)


#if (defined COM_EB_MULTIGW_INCREASE_SRC_COUNT)        /* To prevent double declaration */
#error COM_EB_MULTIGW_INCREASE_SRC_COUNT already defined
#endif /* #if (defined COM_EB_MULTIGW_INCREASE_SRC_COUNT) */

/** \brief COM_EB_MULTIGW_INCREASE_SRC_COUNT
 * signals that a new value is available which shall be routed. It increments the source status
 * for all destination main functions
 * \param[in] IPduPtr - reference to the configuration of the source I-Pdu
 */
#define COM_EB_MULTIGW_INCREASE_SRC_COUNT(IPduPtr)                                    \
   do{                                                                                \
      Com_MainFunctionRx_IdxType mainFuncIdx = 0U;                                    \
     P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) MultiGWSrcStatusptr =                    \
         &Com_gDataMemPtr[COM_EB_MULTIGW_GET_SRC_STATUS_REF(IPduPtr)];                \
   for (mainFuncIdx = 0U; mainFuncIdx < COM_MAIN_FUNCS_TX; mainFuncIdx++)             \
   {                                                                                  \
      (MultiGWSrcStatusptr[mainFuncIdx]) =                                            \
          (uint8)(MultiGWSrcStatusptr[mainFuncIdx] + 1U);                             \
   }                                                                                  \
   }while(0)


#if (defined COM_EB_MULTIGW_GET_SRC_PDULENGTH_REF)        /* To prevent double declaration */
#error COM_EB_MULTIGW_GET_SRC_PDULENGTH_REF already defined
#endif /* #if (defined COM_EB_MULTIGW_GET_SRC_PDULENGTH_REF) */

/** \brief COM_EB_MULTIGW_GET_SRC_PDULENGTH_REF
 * returns the reference to the memory where the size of the source
 * \param[in] IPduPtr - reference to the configuration of the source I-Pdu
 * \return
 * \retval reference source I-Pdu length
 */
#define COM_EB_MULTIGW_GET_SRC_PDULENGTH_REF(IPduPtr) \
    ((IPduPtr)->ComIPduMultiGWValueRef)


#if (defined COM_EB_MULTIGW_GET_SRC_STATUS_REF)        /* To prevent double declaration */
#error COM_EB_MULTIGW_GET_SRC_STATUS_REF already defined
#endif /* #if (defined COM_EB_MULTIGW_GET_SRC_STATUS_REF) */

/** \brief COM_EB_MULTIGW_GET_SRC_STATUS_REF
 * returns the reference of source status for all destination main functions
 * \param[in] IPduPtr - reference to the configuration of the source I-Pdu
 * \return
 * \retval reference to source status
 */
#define COM_EB_MULTIGW_GET_SRC_STATUS_REF(IPduPtr) \
    (COM_EB_MULTIGW_GET_SRC_PDULENGTH_REF((IPduPtr)) + (ComRAMIdxType) COM_EB_MULTIGW_LENGTHTYPE_SIZE())


#if (defined COM_EB_MULTIGW_GET_DEST_STATUS_REF)        /* To prevent double declaration */
#error COM_EB_MULTIGW_GET_DEST_STATUS_REF already defined
#endif /* #if (defined COM_EB_MULTIGW_GET_DEST_STATUS_REF) */

/** \brief COM_EB_MULTIGW_GET_DEST_STATUS_REF
 * returns the reference of destination status for all destination main functions
 * \param[in] IPduPtr - reference to the configuration of the source I-Pdu
 * \return
 * \retval reference to destination status
 */
#define COM_EB_MULTIGW_GET_DEST_STATUS_REF(IPduPtr) \
    (COM_EB_MULTIGW_GET_SRC_STATUS_REF((IPduPtr)) + (ComRAMIdxType) COM_MAIN_FUNCS_TX)


#if (defined COM_EB_MULTIGW_GET_SRC_VALUE_SHADOW_REF)        /* To prevent double declaration */
#error COM_EB_MULTIGW_GET_SRC_VALUE_SHADOW_REF already defined
#endif /* #if (defined COM_EB_MULTIGW_GET_SRC_VALUE_SHADOW_REF) */
/** \brief COM_EB_MULTIGW_GET_SRC_VALUE_SHADOW_REF
 *
 * returns the reference to the I-Pdu gateway shadow buffer of the Rx I-Pdu
 * \param[in] IPduPtr - reference to the configuration of the source I-Pdu
 * \return
 * \retval reference to the I-Pdu gateway shadow buffer
 */
#define COM_EB_MULTIGW_GET_SRC_VALUE_SHADOW_REF(IPduPtr) \
    (COM_EB_MULTIGW_GET_DEST_STATUS_REF((IPduPtr)) + (ComRAMIdxType) COM_MAIN_FUNCS_TX)

#endif /* (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON) */


#if (defined COM_EB_MULTIGW_GET_SRCVALUE_REF)        /* To prevent double declaration */
#error COM_EB_MULTIGW_GET_SRCVALUE_REF already defined
#endif /* #if (defined COM_EB_MULTIGW_GET_SRCVALUE_REF) */
/** \brief COM_EB_MULTIGW_GET_SRCVALUE_REF
 *
 * returns the reference to the buffer from which the data shall be copied to the destination
 * If multi-main function support is switched off, that is the I-Pdu buffer,
 * else it the the I-Pdu gateway shadow buffer
 * \param[in] IPduPtr - reference to the configuration of the source I-Pdu
 * \return
 * \retval reference to the I-Pdu source buffer
 */
#if (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON)
#define COM_EB_MULTIGW_GET_SRCVALUE_REF(IPduPtr) COM_EB_MULTIGW_GET_SRC_VALUE_SHADOW_REF((IPduPtr))
#else
#define COM_EB_MULTIGW_GET_SRCVALUE_REF(IPduPtr) COM_EB_GET_SRCVALUE_REF((IPduPtr))
#endif /* (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON) */


#if (defined COM_EB_MULTIGW_Read)        /* To prevent double declaration */
#error COM_EB_MULTIGW_Read already defined
#endif /* #if (defined COM_EB_MULTIGW_Read) */
/** \brief COM_EB_MULTIGW_Read
 * Invokes a library function (core part, independent from if extensions is configured)
 * If multi-main function support is switched off, values are read with locks,
 * else without locks
 * \param[in] SignalDataPtr - pointer to signal value
 * \param[in] dataPtr - buffer to which the data shall be read
 * \param[in] Com_EB_ReadPropPtr - configuration of a signal
 */
#if (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON)
#define COM_EB_MULTIGW_Read(SignalDataPtr, dataPtr, Com_EB_ReadPropPtr, retval) \
   Com_EB_Read_Core_NoLock((SignalDataPtr), (dataPtr), (Com_EB_ReadPropPtr), (retval))
#else
#define COM_EB_MULTIGW_Read(SignalDataPtr, dataPtr, Com_EB_ReadPropPtr, retval) \
   Com_EB_Read_Core((SignalDataPtr), (dataPtr), (Com_EB_ReadPropPtr), (retval))
#endif /* (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON) */


#if (defined COM_EB_MULTIGW_PDULENGTH)        /* To prevent double declaration */
#error COM_EB_MULTIGW_PDULENGTH already defined
#endif /* #if (defined COM_EB_MULTIGW_PDULENGTH) */
/** \brief COM_EB_MULTIGW_PDULENGTH
 * returns pointer of the length of the source I-Pdu
 * \param[in] PduId - handle ID of the source I-Pdu
 * \param[in] IPduPtr - reference to the configuration of the source I-Pdu
 * \return size of the source I-Pdu
 * \retval size of the source I-Pdu */
#if (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON)
#define COM_EB_MULTIGW_PDULENGTH(PduId, IPduPtr) \
   *(COM_GET_MEMPTR(ComIPduSizeType, COM_EB_MULTIGW_GET_SRC_PDULENGTH_REF((IPduPtr))))
#else
#define COM_EB_MULTIGW_PDULENGTH(PduId, IPduPtr) COM_GET_RX_IPDU_SIZE(PduId, IPduPtr)
#endif

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_START_SEC_CODE
#include <Com_MemMap.h>
#endif

#if (COM_SIGNAL_GW_ENABLE == STD_ON)
/** \brief Com_EB_SetSendSignalGroup - set the signal group list
 * This function set the signal group with the \pSignalGId
 * in the signal group list. If the signal group is already in
 * the list, nothing will be done.
 * \param[in] SignalGId - signal group Id
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_SetSendSignalGroup
(
   ComSignalGroupRefType SignalGId
);

/** \brief Com_EB_CheckSendSignalGroup - checks if signal groups are in the signal group list.
 * If yes, the signal groups is sent and the entry is cleared.
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_CheckSendSignalGroup(void);

/** \brief Com_EB_RouteSignalHelper - routes a signal
 * This function copies the value from source signal / group signal and
 * update the value of the destination signals / group signal
 * if it is assigned to the main function referenced by ComMainFunctionsTx_Idx
 * Preconditions:
 * - COM must be initialized
 * - SoureType of the GWSource has to be a signal / group signal
 * - the signal must be ready to be gated (has to be updated)
 * \param[in] SrcSignalPtr - reference to the configuration of the source object
 * \param[in] GWSourcePtr - reference to routing source configuration
 * \param[in] SrcDataPtr - reference to the data buffer where the data shall be extracted
 * \param[in] ComMainFunctionsTx_Idx - Id of the main function
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_RouteSignalHelper
(
   P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) SrcSignalPtr,
   P2CONST(ComGWSourceType, AUTOMATIC, COM_APPL_CONST) GWSourcePtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) SrcDataPtr
#if (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON)
   ,Com_MainFunctionTx_IdxType ComMainFunctionsTx_Idx
#endif
);


/** \brief Com_EB_RouteSignalHelper_Write - update the value of a
 * destination signal / group signal with the value of the source signal / group signal
 * Preconditions:
 * - COM must be initialized
 * \param[in] SrcSignalPtr - reference to the configuration of the source object
 * \param[in] DestSignalPtr - reference to the configuration of the destination object
 * \param[in] SignalDataPtr - reference to value of the source signal
 * \param[in] IntDataPtr - pointer to internal data structures
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_RouteSignalHelper_Write
(
   P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) SrcSignalPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, COM_APPL_CONST) DestSignalPtr,
   P2CONST(void, AUTOMATIC, COM_VAR_CLEARED) SignalDataPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);


/** \brief Com_EB_RouteSG_2_SG - routes a source signal group to a destination signal group
 * if it is assigned to the main function referenced by ComMainFunctionsTx_Idx
 * Preconditions:
 * - COM must be initialized
 * - SoureType of the GWSource has to be a signal
 * \param[in] GWDestinationPtr - reference to the configuration of the destination
 * \param[in] PosInSrcIPdu - index in RAM where buffer of source SG starts
 * \param[in] ComMainFunctionsTx_Idx - Id of the main function
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_RouteSG_2_SG
(
   P2CONST(ComGWDestinationType, AUTOMATIC, COM_APPL_CONST) GWDestinationPtr,
   ComRAMIdxType PosInSrcIPdu
#if (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON)
   ,Com_MainFunctionTx_IdxType ComMainFunctionsTx_Idx
#endif
);

_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_RouteReceiveSignalGroup
(
   P2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr
);

/** \brief Com_EB_RouteSignalGroup - routes a signal group
 * This function checks if the signal group is updated. If no update bit is configured
 * this is always the case, otherwise the update bit is checked. If signal groups
 * shall routed, the destination is updated,
 * if it is assigned to the main function referenced by ComMainFunctionsTx_Idx
 * Preconditions:
 * - COM must be initialized
 * - SoureType of the GWSource has to be a signal
 * \param[in] SrcIPduPtr - reference to the configuration of the source I-Pdu
 * \param[in] GWSourcePtr - reference to routing source configuration
 * \param[in] ComMainFunctionsTx_Idx - Id of the main function
 * \return Function execution success status
 * \retval TRUE if the signal group was updated and group signals are routed individually,
 * else FALSE
 */
_COM_STATIC_  FUNC(boolean, COM_CODE) Com_EB_RouteSignalGroup
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) SrcIPduPtr,
   P2CONST(ComGWSourceType, AUTOMATIC, COM_APPL_CONST) GWSourcePtr
#if (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON)
   ,Com_MainFunctionTx_IdxType ComMainFunctionsTx_Idx
#endif
);

/** \brief Com_EB_RouteGroupSignal - routes a group signal
 * This function copies the value from source group signal and
 * update the value of the destination signals / group signal,
 * if it is assigned to the main function referenced by ComMainFunctionsTx_Idx
 * Preconditions:
 * - COM must be initialized
 * - SoureType of the GWSource has to be a group signal
 * - the signal must be ready to be gated (has to be updated)
 * \param[in] GWSourcePtr - reference to routing source configuration
 * \param[in] routeGroupSignals - if TRUE, the group signal shall be routed
 * \param[in] SrcIPduPtr - reference to the configuration of the source I-Pdu
 * \param[in] ComMainFunctionsTx_Idx - Id of the main function
 * (i.e. the update bit of signal group was set), else the group signal is not routed.
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_RouteGroupSignal
(
   P2CONST(ComGWSourceType, AUTOMATIC, COM_APPL_CONST) GWSourcePtr,
   boolean routeGroupSignals
#if (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON)
   ,P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) SrcIPduPtr
   ,Com_MainFunctionTx_IdxType ComMainFunctionsTx_Idx
#endif
);

/** \brief Com_EB_RouteSignal - routes a source signal to a destination,
 * if it is assigned to the main function referenced by ComMainFunctionsTx_Idx
 * Preconditions:
 * - COM must be initialized
 * - SoureType of the GWSource has to be a signal
 * - the signal must be ready to be gated (has to be updated)
 * \param[in] GWSourcePtr - reference to routing source configuration
 * \param[in] SrcIPduPtr - reference to the configuration of the source I-Pdu
 * \param[in] ComMainFunctionsTx_Idx - Id of the main function
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_RouteSignal
(
   P2CONST(ComGWSourceType, AUTOMATIC, COM_APPL_CONST) GWSourcePtr,
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) SrcIPduPtr
#if (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON)
   ,Com_MainFunctionTx_IdxType ComMainFunctionsTx_Idx
#endif
);

/** \brief Com_MainFunctionRouteSignals_Src_Generic_hlp - routes source to destination,
 * if it is assigned to the main function referenced by ComMainFunctionsTx_Idx
 * Preconditions:
 * - COM must be initialized
 * - SoureType of the GWSource has to be a signal
 * - the signal must be ready to be gated (has to be updated)
 * \param[in] SrcIPduPtr - reference to routing source configuration
 * \param[in] ComMainFunctionsTx_Idx - Id of the main function
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_MainFunctionRouteSignals_Src_Generic_hlp
(
   CONSTP2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) SrcIPduPtr
#if (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON)
   ,Com_MainFunctionTx_IdxType ComMainFunctionsTx_Idx
#endif
);

#endif /* (COM_SIGNAL_GW_ENABLE == STD_ON) */

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

#if (COM_SIGNAL_GW_ENABLE == STD_ON)


/** \brief Com_EB_SetSendSignalGroup - set the signal group list
 * This function set the signal group with the \pSignalGId
 * in the signal group list. If the signal group is already in
 * the list, nothing will be done.
 * \param[in] SignalGId - signal group Id
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_SetSendSignalGroup
(
   ComSignalGroupRefType SignalGId
)
{
   ComSignalGroupRefType ii;
   uint8 exit_flag = 1U;

   for (ii = 0U; (ii < Com_gConfigPtr->ComSigGWTxSignalGroupQueueMax) && (exit_flag == 1U); ii++)
   {
      /* Deviation MISRAC2012-1 <+2> */
      CONSTP2VAR(ComSignalGroupRefType, AUTOMATIC, COM_VAR_CLEARED) TxSignalGroupQueueBase =
               COM_GET_MEMPTR(ComSignalGroupRefType, Com_gConfigPtr->ComSigGWTxSignalGroupQueueBase);

      if (TxSignalGroupQueueBase[ii] == SignalGId)
      {
         /* signal group is already in list, exit loop */
         exit_flag = 0U;
      } else if (TxSignalGroupQueueBase[ii] == COM_SIGNAL_GROUP_INVALID)
      {
         /* entry in list is free, add signal group */
         TxSignalGroupQueueBase[ii] = SignalGId;

         /* exit loop */
         exit_flag = 0U;
      } else
      {
         /* empty else */
      }
   }

}

/** \brief Com_EB_CheckSendSignalGroup - checks if signal groups are in the signal group list.
 * If yes, the signal groups is sent and the entry is cleared.
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_CheckSendSignalGroup(void)
{

   ComSignalGroupRefType ii;
   uint8 exit_flag = 1U;
   uint8 retval = E_OK;

   for (ii = 0U; (ii < Com_gConfigPtr->ComSigGWTxSignalGroupQueueMax) && (exit_flag == 1U); ii++)
   {
      /* Deviation MISRAC2012-1 <+2> */
      CONSTP2VAR(ComSignalGroupRefType, AUTOMATIC, COM_VAR_CLEARED) TxSignalGroupQueueBase =
               COM_GET_MEMPTR(ComSignalGroupRefType, Com_gConfigPtr->ComSigGWTxSignalGroupQueueBase);

      ComSignalGroupRefType const SignalGId  = TxSignalGroupQueueBase[ii];

      if (SignalGId != COM_SIGNAL_GROUP_INVALID)
      {

         CONSTP2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr =
                  COM_GET_CONFIG_ELEMENT(ComTxSignalGroupType,
                           Com_gConfigPtr->ComTxSignalGroupRef, SignalGId);

         TxSignalGroupQueueBase[ii] = COM_SIGNAL_GROUP_INVALID;

         Com_SetupTxSignalGroup(
                  NULL_PTR,
                  0U,
                  &Com_EB_SendSignalGroup,
                  SignalGPtr,
                  &retval
#if ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) )
                  ,TRUE
#endif /* ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) ) */
         );
      }
      else
      {
         /* exit loop */
         exit_flag = 0U;
      }

   }

   TS_PARAM_UNUSED(retval);
}

/** \brief Com_EB_RouteSignalHelper - routes a signal
 * This function copies the value from source signal / group signal and
 * update the value of the destination signals / group signal
 * for signals / group signal which are referenced by ComMainFunctionsTx_Idx
 * Preconditions:
 * - COM must be initialized
 * - SoureType of the GWSource has to be a signal / group signal
 * - the signal must be ready to be gated (has to be updated)
 * \param[in] SrcSignalPtr - reference to the configuration of the source object
 * \param[in] GWSourcePtr - reference to routing source configuration
 * \param[in] SrcDataPtr - reference to the data buffer where the data shall be extracted
 * \param[in] ComMainFunctionsTx_Idx - Id of the main function
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_RouteSignalHelper
(
   P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) SrcSignalPtr,
   P2CONST(ComGWSourceType, AUTOMATIC, COM_APPL_CONST) GWSourcePtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) SrcDataPtr
#if (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON)
   ,Com_MainFunctionTx_IdxType ComMainFunctionsTx_Idx
#endif
)
{
#if (COM_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
   boolean proceedProcessing = TRUE;
#endif

   uint32 valuint32;
   uint16 valuint16;
   uint8 valuint8;
   boolean valboolean;
   ComEmu64Type valuint64;

   /* Pointer to the source data */
   P2VAR(void, AUTOMATIC, COM_VAR_CLEARED) SignalDataPtr = &valuint8;

   ComGWDestinationRefType i; /* iteration variable for destination object */

   CONSTP2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr =
            &SrcSignalPtr->ComSignalBase;

   /* Get the signal type of the source signal */
   Com_TS_SignalTypeType SignalType = COM_GET_RX_SIGNAL_SIGNAL_TYPE(Com_EB_ReadPropPtr);


   if ((SignalType != COM_UINT8_N) && (SignalType != COM_UINT8_DYN))
   {
     uint8 retval = COM_SERVICE_NOT_AVAILABLE;

      /* get the address of the value */
      switch (SignalType)
      {
      case COM_BOOLEAN:
         COM_EB_MULTIGW_Read(&valboolean,SrcDataPtr,Com_EB_ReadPropPtr, &retval);
         SignalDataPtr = &valboolean;
         break;

      case COM_UINT8:
      case COM_SINT8:
         COM_EB_MULTIGW_Read(&valuint8,SrcDataPtr,Com_EB_ReadPropPtr, &retval);
         SignalDataPtr = &valuint8;
         break;

      case COM_UINT16:
      case COM_SINT16:
         COM_EB_MULTIGW_Read(&valuint16,SrcDataPtr,Com_EB_ReadPropPtr, &retval);
         SignalDataPtr = &valuint16;
         break;

      case COM_UINT32:
      case COM_SINT32:
         COM_EB_MULTIGW_Read(&valuint32,SrcDataPtr,Com_EB_ReadPropPtr, &retval);
         SignalDataPtr = &valuint32;
         break;

      case COM_UINT64:
      case COM_SINT64:
         COM_EB_MULTIGW_Read(&valuint64,SrcDataPtr,Com_EB_ReadPropPtr, &retval);
         SignalDataPtr = &valuint64;
         break;

         /* CHECK: NOPARSE */
      default:
      {
#if (COM_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
         COM_UNREACHABLE_CODE_ASSERT(COMServiceId_MainFunctionRouteSignals);
         proceedProcessing = FALSE;
#endif /* (COM_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON) */
         break;
      }
      /* CHECK: PARSE */
      }                /* switch (SignalType) */


   }                         /* if (SignalType != COM_UINT8_N) */
   else
   {
      /* signal is a byte array */
      ComSignalDescBitType StartByte =  SrcSignalPtr->ComSignalBase.ComSignalPositionInBit;
      /* no need to convert endianness for byte arrays */
      SignalDataPtr = &SrcDataPtr[StartByte];

   }                         /* signal is byte array */



#if (COM_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
   /* CHECK: NOPARSE */
   if (TRUE == proceedProcessing)
      /* CHECK: PARSE */
#endif
   {
      /* loop over all destination  */
      for (i = GWSourcePtr->ComGWDestinationFirstRef;
               (i < (GWSourcePtr->ComGWDestinationFirstRef + GWSourcePtr->ComGWDestinationNumRef));
               i++)
      {
         /* get the configuration of the destination object */
         /* reference to the configuration of the destination object */
         CONSTP2CONST(ComGWDestinationType, AUTOMATIC, COM_APPL_CONST) GWDestinationPtr =
                  COM_GET_CONFIG_ELEMENT(ComGWDestinationType,
                           (Com_gConfigPtr->ComGWDestinationRef), i);

         /* set the configuration of the destination signal */
         CONSTP2CONST(ComTxSignalType, AUTOMATIC, COM_APPL_CONST) DestSignalPtr =
                  COM_GET_CONFIG_ELEMENT_GWTYPE(ComTxSignalType,
                           GWDestinationPtr->ComGWDestinationRef, 0U);

#if (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON)

         CONSTP2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr =
                  COM_GET_CONFIG_ELEMENT(ComTxIPduType, Com_gConfigPtr->ComTxIPduRef,
                           DestSignalPtr->ComIPduRef);

         if(IPduPtr->ComMainFunctionsTx_Idx == ComMainFunctionsTx_Idx)
#endif
         {

            Com_EB_WriteIntDataType IntData;

            COM_INIT_Com_EB_WriteIntDataType(IntData, 0U, FALSE,
                COM_SERVICE_NOT_AVAILABLE, TRUE, FALSE, E_NOT_OK);

            /* GWDestinationPtr is valid */
            switch (GWDestinationPtr->ComGWDestinationSelect)
            {
            case COM_GW_SIGNAL:
            {

               IntData.SendSigRetval = Com_GetSendSignalRetVal(DestSignalPtr);

               Com_EB_RouteSignalHelper_Write(
                        SrcSignalPtr,
                        DestSignalPtr,
                        SignalDataPtr,
                        &IntData);

#if (COM_SENDUPONAPICALL == STD_ON)

               /* transmit Pdu if required; ignore the error; error could be
                * COM_SERVICE_NOT_AVAILABLE in case the I-Pdu is stopped */
               Com_EB_SendSignal_Transmit(DestSignalPtr,&IntData);

#endif /* (COM_SENDUPONAPICALL == STD_ON) */

               break;
            }                   /* case COM_GW_SIGNAL */


            case COM_GW_GROUP_SIGNAL:
            {

               ComSignalGroupRefType const SignalGId =
                        DestSignalPtr->ComSignalBase.ComSignalGroupRef;

               IntData.SendSigRetval = COM_SERVICE_NOT_AVAILABLE;

               Com_EB_RouteSignalHelper_Write(
                        SrcSignalPtr,
                        DestSignalPtr,
                        SignalDataPtr,
                        &IntData);

               Com_EB_SetSendSignalGroup(SignalGId);

               break;
            }                   /* case COM_GW_SIGNAL */

            /* CHECK: NOPARSE */
            default:
               COM_UNREACHABLE_CODE_ASSERT(COMServiceId_MainFunctionRouteSignals);
               break;
               /* CHECK: PARSE */
            }                   /* switch (GWDestinationPtr->ComGWDestinationType) */
         }
      } /* for (i = GWSourcePtr->ComGWDestinationFirstRef;
              (i < GWSourcePtr->ComGWDestinationFirstRef + GWSourcePtr->ComGWDestinationNumRef);
              i++) */
   }                            /* (TRUE == proceedProcessing) */


}                               /* Com_EB_RouteSignalHelper */


/** \brief Com_EB_RouteSignalHelper_Write - update the value of a
 * destination signal / group signal with the value of the source signal / group signal
 * Preconditions:
 * - COM must be initialized
 * \param[in] SrcSignalPtr - reference to the configuration of the source object
 * \param[in] DestSignalPtr - reference to the configuration of the destination object
 * \param[in] SignalDataPtr - reference to value of the source signal
 * \param[in] IntDataPtr - pointer to internal data structures
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_RouteSignalHelper_Write
(
   P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) SrcSignalPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, COM_APPL_CONST) DestSignalPtr,
   P2CONST(void, AUTOMATIC, COM_VAR_CLEARED) SignalDataPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{

#if (COM_RX_DYN_LENGTH_IPDU_ENABLE == STD_ON) && (COM_TX_DYN_LENGTH_IPDU_ENABLE == STD_ON)

   if( COM_UINT8_DYN == COM_GET_RX_SIGNAL_SIGNAL_TYPE(&SrcSignalPtr->ComSignalBase) )
   {

      CONSTP2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr =
               COM_GET_CONFIG_ELEMENT(ComRxIPduType, (Com_gConfigPtr->ComRxIPduRef),
                        SrcSignalPtr->ComIPduRef);

      /* Deviation MISRAC2012-1 <+3> */
      /* Deviation MISRAC2012-2 <+2> */
      CONSTP2VAR(ComIPduSizeType, AUTOMATIC, COM_APPL_DATA) SrcSigDatLenPtr =
          (ComIPduSizeType *   ) &(COM_EB_MULTIGW_PDULENGTH(SrcSignalPtr->ComIPduRef,IPduPtr));

      ComSignalDescBitType const SrcSigStartByte =
               SrcSignalPtr->ComSignalBase.ComSignalPositionInBit;

#if (COM_MULTI_ROUTE_MAIN_ENABLE == STD_OFF)
      TS_PARAM_UNUSED(IPduPtr);
#endif

      {

        /* enter critical section */
        Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();
        {

          ComSignalBitSizeType DestSigLength;
          ComIPduSizeType const SrcPduSize = *SrcSigDatLenPtr;


          /* Calculation is required since byte arrays are directly copied from
           * source I-Pdu; since the size may change at Rx side the length of the
           * Tx signal has to be calculated immediately before copying the data */
          if (SrcPduSize <= SrcSigStartByte)
          {
            /* the signal is not available any more;
             * that is the case when the source I-PDU is shorter
             * than the starting position of the dynamic signal.
             * In this case send a signal with size 0. */
            DestSigLength = 0U;
          }
          else
          {
            DestSigLength = SrcPduSize - SrcSigStartByte;
          }

          IntDataPtr->arraySize = DestSigLength;

        }

        /* Write the value to the buffer and exit critical section */
        Com_EB_WriteDynSignal_ExitArea_0(SignalDataPtr,DestSignalPtr,IntDataPtr);

      }

   }
   else
#endif /* (COM_RX_DYN_LENGTH_IPDU_ENABLE == STD_ON) && (COM_TX_DYN_LENGTH_IPDU_ENABLE == STD_ON) */
   {

      /* signal is not dynamic */

      CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) DestDataPtr =
               &Com_gDataMemPtr[DestSignalPtr->ComSignalBase.ComBufferRef];

      TS_PARAM_UNUSED(SrcSignalPtr);

      /* in case it is a byte array this size is used for writing data to buffer */
      IntDataPtr->arraySize = DestSignalPtr->ComSignalBase.ComBitSize;

      /* set the value of the using the library */
      Com_EB_Write_Ext(SignalDataPtr,DestDataPtr,DestSignalPtr,IntDataPtr);
   }

}


/** \brief Com_EB_RouteSG_2_SG - routes a source signal group to a destination signal group
 * if it is assigned to the main function referenced by ComMainFunctionsTx_Idx
 * Preconditions:
 * - COM must be initialized
 * - SoureType of the GWSource has to be a signal
 * \param[in] GWDestinationPtr - reference to the configuration of the destination
 * \param[in] PosInSrcIPdu - index in RAM where buffer of source SG starts
 * \param[in] ComMainFunctionsTx_Idx - Id of the main function
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_RouteSG_2_SG
(
   P2CONST(ComGWDestinationType, AUTOMATIC, COM_APPL_CONST) GWDestinationPtr,
   ComRAMIdxType PosInSrcIPdu
#if (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON)
   ,Com_MainFunctionTx_IdxType ComMainFunctionsTx_Idx
#endif
)
{
   /* set configuration destination signal */
   CONSTP2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) DestSignalGPtr =
            COM_GET_CONFIG_ELEMENT_GWTYPE(ComTxSignalGroupType,
                     GWDestinationPtr->ComGWDestinationRef, 0U);


   /* get the Id and the reference of the I-Pdu to which the destination signal belongs */
   ComIPduRefType const DestPduId = DestSignalGPtr->ComIPduRef;

   /* get the configuration of the destination I-Pdu */
   CONSTP2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) DestIPduPtr =
            COM_GET_CONFIG_ELEMENT(ComTxIPduType, Com_gConfigPtr->ComTxIPduRef, DestPduId);


#if (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON)
   if(DestIPduPtr->ComMainFunctionsTx_Idx == ComMainFunctionsTx_Idx)
#endif
   {

      (void)Com_EB_SendSignalGroupArray(
               &Com_gDataMemPtr[(PosInSrcIPdu)],
               DestSignalGPtr,
               DestPduId,
               DestIPduPtr
#if ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) )
               ,TRUE
#endif /* ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) ) */
      );
   }

}                               /* Com_EB_RouteSG_2_SG */


_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_RouteReceiveSignalGroup
(
   P2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr
)
{

   uint8 retval = E_OK;

   Com_SetupRxSignalGroup(
            &retval,
            NULL_PTR,
            SignalGPtr,
            &Com_CopyRxSignalGroupArray_Shadow
   );

   TS_PARAM_UNUSED(retval);

}

/** \brief Com_EB_RouteSignalGroup - routes a signal group
 * This function checks if the signal group is is updated. If no update bit is configured
 * this is always the case, otherwise the update bit is checked. If signal groups
 * shall routed, the destination is updated,
 * if it is assigned to the main function referenced by ComMainFunctionsTx_Idx
 * Preconditions:
 * - COM must be initialized
 * - SoureType of the GWSource has to be a signal
 * \param[in] SrcIPduPtr - reference to the configuration of the source I-Pdu
 * \param[in] GWSourcePtr - reference to routing source configuration
 * \param[in] ComMainFunctionsTx_Idx - Id of the main function
 * \return Function execution success status
 * \retval TRUE if the signal group was updated and group signals are routed individually,
 * else FALSE
 */
_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_RouteSignalGroup
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) SrcIPduPtr,
   P2CONST(ComGWSourceType, AUTOMATIC, COM_APPL_CONST) GWSourcePtr
#if (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON)
   ,Com_MainFunctionTx_IdxType ComMainFunctionsTx_Idx
#endif
)
{
   /* reference to the source (Rx) signal group */
   CONSTP2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SrcSignalGPtr =
            COM_GET_CONFIG_ELEMENT_GWTYPE(ComRxSignalGroupType, GWSourcePtr->ComGWSourceRef, 0U);


   /* The starting position of the value of the signal group is the position
    * of source signal group within the source I-Pdu
    * plus the starting position of the source I-Pdu in the RAM */
   const ComRAMIdxType PosInSrcIPdu =
            (ComRAMIdxType)(SrcSignalGPtr->ComSignalPositionInByte +
                     COM_EB_MULTIGW_GET_SRCVALUE_REF(SrcIPduPtr));

   boolean routeGroupSignals = FALSE;

#if (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL)
   uint8 SignalUpdated = 1U; /* value of update bit (if no update bit is
   configured, the signal shall always be gated) */
#endif /* (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) */


#if (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL)

   CONSTP2CONST(ComRxSignalExtType, AUTOMATIC, COM_APPL_CONST) SignalExtPtr =
            COM_GET_CONFIG_ELEMENT(ComRxSignalExtType,
                     Com_gConfigPtr->ComRxSignalExtRef, SrcSignalGPtr->ComSignalExtRef);

#if(COM_UPDATE_BIT_RX_CONFIG == COM_UPDATE_BIT_INDIVIDUAL)
   /* check if the update bit is configured */
   if (COM_GET_RX_SIGNAL_GROUP_UPDATE_BIT(SignalExtPtr) == COM_UPDATE_BIT_PRESENT)
#endif /* (COM_UPDATE_BIT_RX_CONFIG == COM_UPDATE_BIT_INDIVIDUAL) */
   {
      /* The signal group has the update bit configured -> read it */
      TS_GetBit(
               &Com_gDataMemPtr[((ComRAMIdxType)COM_EB_MULTIGW_GET_SRCVALUE_REF(SrcIPduPtr) +
                        ((ComRAMIdxType)SignalExtPtr->ComUpdateBitPositionInBit / 8U))],
                        (uint8)(SignalExtPtr->ComUpdateBitPositionInBit % (ComSignalDescBitType)8U),
                        uint8,
                        SignalUpdated);
   }

   if (1U == SignalUpdated)
#endif /* (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) */
   {                            /* signal group is gated */
      /* either no update bit is configured or the update bit is set */

      if (GWSourcePtr->ComGWDestinationNumRef > 0)
      {

         ComGWDestinationRefType GWDestinationRef; /* iteration variable for destination object */

         /* loop over all destination  */
         for (GWDestinationRef = GWSourcePtr->ComGWDestinationFirstRef;
                  (GWDestinationRef < (GWSourcePtr->ComGWDestinationFirstRef + GWSourcePtr->ComGWDestinationNumRef));
                  GWDestinationRef++)
         {
            /* get the configuration of the destination object */
            CONSTP2CONST(ComGWDestinationType, AUTOMATIC, COM_APPL_CONST) GWDestinationPtr =
                     COM_GET_CONFIG_ELEMENT(ComGWDestinationType,
                              (Com_gConfigPtr->ComGWDestinationRef), GWDestinationRef);

            {
               /* GWDestinationPtr is valid */

               switch (GWDestinationPtr->ComGWDestinationSelect)
               {
               case COM_GW_SIGNAL_GROUP:
               {
                  /* the singal group shall be copied directly */
                  Com_EB_RouteSG_2_SG(
                           GWDestinationPtr,
                           PosInSrcIPdu
#if (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON)
                           ,ComMainFunctionsTx_Idx
#endif
                           );
                  break;
               }                /* case COM_GW_SIGNAL_GROUP: */

               /* CHECK: NOPARSE */
               default:
                  COM_UNREACHABLE_CODE_ASSERT(COM_INTERNAL_API_ID);
                  break;
                  /* CHECK: PARSE */
               }                /* switch (GWDestinationPtr->ComGWDestinationType) */

            }                   /* GWDestinationPtr is valid */

         } /* for (i = GWSourcePtr->ComGWDestinationFirstRef;
                  (i < GWSourcePtr->ComGWDestinationFirstRef + GWSourcePtr->ComGWDestinationNumRef);
                  i++) */
      }
      else
      {
         /* there is no destination for the signal group */
         routeGroupSignals = TRUE;
         /* copy to the shadow buffer, group signals are extracted later */
         Com_EB_RouteReceiveSignalGroup(SrcSignalGPtr);
      }


   }                         /* signal group is gated */

   return routeGroupSignals;

}                               /* Com_EB_RouteSignalGroup */

/** \brief Com_EB_RouteGroupSignal - routes a group signal
 * This function copies the value from source group signal and
 * update the value of the destination signals / group signal,
 * if it is assigned to the main function referenced by ComMainFunctionsTx_Idx
 * Preconditions:
 * - COM must be initialized
 * - SoureType of the GWSource has to be a group signal
 * - the signal must be ready to be gated (has to be updated)
 * \param[in] GWSourcePtr - reference to routing source configuration
 * \param[in] routeGroupSignals - if TRUE, the group signal shall be routed
 * \param[in] SrcIPduPtr - reference to the configuration of the source I-Pdu
 * \param[in] ComMainFunctionsTx_Idx - Id of the main function
 * (i.e. the update bit of signal group was set), else the group signal is not routed.
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_RouteGroupSignal
(
   P2CONST(ComGWSourceType, AUTOMATIC, COM_APPL_CONST) GWSourcePtr,
   boolean routeGroupSignals
#if (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON)
   ,P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) SrcIPduPtr
   ,Com_MainFunctionTx_IdxType ComMainFunctionsTx_Idx
#endif
)
{

   if (routeGroupSignals == TRUE)
   {
      /* reference to the source (Rx) group signal */
      CONSTP2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) SrcSignalPtr =
               COM_GET_CONFIG_ELEMENT_GWTYPE(ComRxSignalType, GWSourcePtr->ComGWSourceRef, 0U);

#if (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON)

      ComSignalGroupRefType const SignalGId= SrcSignalPtr->ComSignalBase.ComSignalGroupRef;

      CONSTP2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr =
               COM_GET_CONFIG_ELEMENT(ComRxSignalGroupType,
                        Com_gConfigPtr->ComRxSignalGroupRef, SignalGId);

      ComSignalDescByteType const StartingByte = SignalGPtr->ComSignalPositionInByte;

      P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) SrcDataPtr =
               &Com_gDataMemPtr[COM_EB_MULTIGW_GET_SRCVALUE_REF(SrcIPduPtr) + StartingByte];

#else

      CONSTP2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr =
               &SrcSignalPtr->ComSignalBase;

      P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) SrcDataPtr =
               &Com_gDataMemPtr[Com_EB_ReadPropPtr->ComBufferRef];

#endif

      Com_EB_RouteSignalHelper(
               SrcSignalPtr,
               GWSourcePtr,
               SrcDataPtr
#if (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON)
               ,ComMainFunctionsTx_Idx
#endif
               );
   }

}                               /* Com_EB_RouteGroupSignal */

/** \brief Com_EB_RouteSignal - routes a source signal to a destination,
 * if it is assigned to the main function referenced by ComMainFunctionsTx_Idx
 * Preconditions:
 * - COM must be initialized
 * - SoureType of the GWSource has to be a signal
 * - the signal must be ready to be gated (has to be updated)
 * \param[in] GWSourcePtr - reference to routing source configuration
 * \param[in] SrcIPduPtr - reference to the configuration of the source I-Pdu
 * \param[in] ComMainFunctionsTx_Idx - Id of the main function
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_RouteSignal
(
   P2CONST(ComGWSourceType, AUTOMATIC, COM_APPL_CONST) GWSourcePtr,
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) SrcIPduPtr
#if (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON)
   ,Com_MainFunctionTx_IdxType ComMainFunctionsTx_Idx
#endif
)
{
   /* reference to the source (Rx) signal */
   CONSTP2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) SrcSignalPtr =
            COM_GET_CONFIG_ELEMENT_GWTYPE(ComRxSignalType, GWSourcePtr->ComGWSourceRef, 0);


   {
#if ((COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) || \
         (COM_SIGGW_RX_FILTER_ENABLE == STD_ON))
      /* Signal shall be gated if:
       * - the update bit is set (when update bits are enabled and configured)
       * - the filter evaluates to TRUE (when SigGWFilter is enabled)
       * - if neither an update bit nor a GW Filter is configured */
      uint8 GateSignal = 1U;

      CONSTP2CONST(ComRxSignalExtType, AUTOMATIC, COM_APPL_CONST) SignalExtPtr =
               COM_GET_CONFIG_ELEMENT(ComRxSignalExtType,
                        Com_gConfigPtr->ComRxSignalExtRef, SrcSignalPtr->ComSignalExtRef);


#if (COM_SIGGW_RX_FILTER_ENABLE == STD_ON)

      if (SignalExtPtr->ComSignalSigGWFlagBit != COM_SIGNAL_INVALID)
      {
         /* check if the signal shall be routed (filter evaluates to TRUE) */
         TS_GetBit(COM_RX_SIGNAL_SIG_GW_FLAG_ADDRESS(SignalExtPtr),
                  COM_RX_SIGNAL_SIG_GW_FLAG_BIT(SignalExtPtr), uint8, GateSignal);

         /* clear the flag */
         COM_AtomicClearBit_8(COM_RX_SIGNAL_SIG_GW_FLAG_ADDRESS(SignalExtPtr),
                  COM_RX_SIGNAL_SIG_GW_FLAG_BIT(SignalExtPtr));

      }
      else
#endif /* (COM_SIGGW_RX_FILTER_ENABLE == STD_ON) */

      {
         /* the signal has no filter configured/enabled -> check the update bit */
#if (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL)


#if (COM_UPDATE_BIT_RX_CONFIG == COM_UPDATE_BIT_INDIVIDUAL)

         /* check if signal has update bit */
         if (COM_GET_RX_SIGNAL_EXT_UPDATE_BIT(SignalExtPtr) == COM_UPDATE_BIT_PRESENT)
#endif /* (COM_UPDATE_BIT_RX_CONFIG == COM_UPDATE_BIT_INDIVIDUAL) */
         {

            /* get the value of the update bit */
            TS_GetBit(&Com_gDataMemPtr[((ComRAMIdxType)COM_EB_MULTIGW_GET_SRCVALUE_REF(SrcIPduPtr) +
                     ((ComRAMIdxType)SignalExtPtr->ComUpdateBitPositionInBit / 8U))],
                     (uint8)(SignalExtPtr->ComUpdateBitPositionInBit % (ComSignalDescBitType)8U),
                     uint8,
                     GateSignal);

         }
#endif /* (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) */

      }


      /* check if the signal shall be gated */
      if (GateSignal == 1U)
#endif /* ((COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) || \
      (COM_SIGGW_RX_FILTER_ENABLE == STD_ON)) */
      {

         P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) SrcDataPtr =
                  &Com_gDataMemPtr[COM_EB_MULTIGW_GET_SRCVALUE_REF(SrcIPduPtr)];

         /* the signal has to be gated */
         /* call the signal routing function */
         Com_EB_RouteSignalHelper(
                  SrcSignalPtr,
                  GWSourcePtr,
                  SrcDataPtr
#if (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON)
                  ,ComMainFunctionsTx_Idx
#endif
                  );
      }
   }

}                               /* Com_EB_RouteSignal */


/** \brief Com_MainFunctionRouteSignals_Src_Generic_hlp - routes source to destination,
 * if it is assigned to the main function referenced by ComMainFunctionsTx_Idx
 * Preconditions:
 * - COM must be initialized
 * - SoureType of the GWSource has to be a signal
 * - the signal must be ready to be gated (has to be updated)
 * \param[in] SrcIPduPtr - reference to routing source configuration
 * \param[in] ComMainFunctionsTx_Idx - Id of the main function
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_MainFunctionRouteSignals_Src_Generic_hlp
(
   CONSTP2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) SrcIPduPtr
#if (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON)
   ,Com_MainFunctionTx_IdxType ComMainFunctionsTx_Idx
#endif
)
{
   /* IPduPtr is valid */


   {

      boolean routeGroupSignals = FALSE;
      ComGWSourceRefType sigGWSourceRef; /* iteration variable */


      /* the I-Pdu has (group)signals(group) which have to be gated now */
      /* get the Routing Table */

      for (sigGWSourceRef = SrcIPduPtr->ComIPduGWSourceRefFirst;
               sigGWSourceRef <
               (SrcIPduPtr->ComIPduGWSourceRefFirst + SrcIPduPtr->ComIPduGWSourceRefNum);
               sigGWSourceRef++)
      {
         /* get routing information */
         /* reference to a routing table entry */
         CONSTP2CONST(ComGWSourceType, AUTOMATIC, COM_APPL_CONST) GWSourcePtr =
                  COM_GET_CONFIG_ELEMENT(ComGWSourceType,
                           (Com_gConfigPtr->ComGWSourceRef), sigGWSourceRef);

         {
            /* GWSourcePtr is valid */

            switch (GWSourcePtr->ComGWSourceSelect)
            {
            case COM_GW_SIGNAL:
            {
               /* the gated object is a signal, handle it */
               Com_EB_RouteSignal(
                        GWSourcePtr,
                        SrcIPduPtr
#if (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON)
                        ,ComMainFunctionsTx_Idx
#endif
                        );
               break;
            }                   /* case COM_GW_SIGNAL */

            case COM_GW_SIGNAL_GROUP:
            {
               /* the gated object is a signal group, handle it */
               routeGroupSignals = Com_EB_RouteSignalGroup(
                        SrcIPduPtr,
                        GWSourcePtr
#if (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON)
                        ,ComMainFunctionsTx_Idx
#endif
                        );
               break;
            }                   /* case COM_GW_SIGNAL_GROUP */

            case COM_GW_GROUP_SIGNAL:
            {
               Com_EB_RouteGroupSignal(
                        GWSourcePtr,
                        routeGroupSignals
#if (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON)
                        ,SrcIPduPtr
                        ,ComMainFunctionsTx_Idx
#endif
                        );
               break;
            }                /* case COM_GW_SIGNAL_GROUP: */

            /* CHECK: NOPARSE */
            default:
               COM_UNREACHABLE_CODE_ASSERT(COMServiceId_MainFunctionRouteSignals);
               break;
               /* CHECK: PARSE */
            }          /* switch (GWSourcePtr->ComGWSourceSelect) */

         }             /* GWSourcePtr is valid */

      } /* for (j = IPduPtr->ComIPduGwRoutingRefFirst;
        j < (IPduPtr->ComIPduGwRoutingRefFirst + IPduPtr->ComIPduGwRoutingRefNum);
        j++) */

      /* check if there is a signal group which has to be sent */
      Com_EB_CheckSendSignalGroup();

   }                   /* if( Com_IsBitSet_8(COM_RX_IPDU_SIG_GW_FLAG_ADDRESS(IPduPtr),
                              COM_RX_IPDU_SIG_GW_FLAG_BIT(IPduPtr)) ) */

}                      /* IPduPtr is valid */




#if (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON)
/* AUTOSAR declares Com_MainFunctionRouteSignals_Src_<MainFunctionName_Src>_Dest_<MainFunctionName_Dest>()
   also in SchM_Com.h which leads to "MISRA-C:2012 Rule 08.5" violation. */

/**
 * \brief Com_MainFunctionRouteSignals_Src_Dest_Generic - handles destination related part for
 * signal gateway for source I-Pdus which are assigned to Rx main function with the Id
 * ComMainFunctionsRx_Idx and destination I-Pdus which are assigned to Tx main function with the Id
 * ComMainFunctionsTx_Idx
 *
 * This function copies values from the source signal gateway shadow buffer to the destination
 * I-Pdus which are assigned to Tx main function with the Id ComMainFunctionsTx_Idx
 * Preconditions:
 * - COM must be initialized
 * - must not be interrupted by or interrupt Com_MainFunctionRouteSignals_Src_Generic
 *   with the same Rx main function Id ComMainFunctionsRx_Idx
 *
 * \param[in] ComMainFunctionsRx_Idx - Id of the main function
 * \param[in] ComMainFunctionsTx_Idx - Id of the main function
 *
 *
 * \ServiceID{0x18}
 * \Reentrancy{Non re-entrant}
 * \Synchronicity{Synchronous}
 */
FUNC(void, COM_CODE) Com_MainFunctionRouteSignals_Src_Dest_Generic
(

   Com_MainFunctionRx_IdxType ComMainFunctionsRx_Idx,
   Com_MainFunctionTx_IdxType ComMainFunctionsTx_Idx

)
{

   if (Com_InitStatus == COM_EB_INIT)
   {

      uint16 sigGWRxPduRef; /* iteration variable */

      for (sigGWRxPduRef = 0U;
               sigGWRxPduRef < Com_gConfigPtr->ComSigGWRxIPduArraySize;
               sigGWRxPduRef++)
      {
         /* get Id of IPdu */
         const ComIPduRefType IPduId = (ComIPduRefType) * (COM_GET_CONFIG_ELEMENT
                  (ComIPduRefType,(Com_gConfigPtr->ComSigGWRxIPduListRef), sigGWRxPduRef));

         /* get RxPduId configuration reference */
         CONSTP2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr =
                  COM_GET_CONFIG_ELEMENT(ComRxIPduType, (Com_gConfigPtr->ComRxIPduRef), IPduId);

         /* Check IPdu only if it belongs to  */
         if ( IPduPtr->ComMainFunctionsRx_Idx == ComMainFunctionsRx_Idx )
         {

            CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) MultiGWSrcStatusptr =
                     &Com_gDataMemPtr[COM_EB_MULTIGW_GET_SRC_STATUS_REF(IPduPtr)
                                      + ComMainFunctionsTx_Idx];

            CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) MultiGWDestStatusptr =
                     &Com_gDataMemPtr[COM_EB_MULTIGW_GET_DEST_STATUS_REF(IPduPtr)
                                      + ComMainFunctionsTx_Idx];

            /* New Value is available */
            if (* MultiGWDestStatusptr != * MultiGWSrcStatusptr)
            {
               /* New Value is available */
               * MultiGWDestStatusptr = * MultiGWSrcStatusptr;

               /* signals shall be gated for destination */
               Com_MainFunctionRouteSignals_Src_Generic_hlp(IPduPtr, ComMainFunctionsTx_Idx);

            }
         }

      }                         /* (      for (sigGWRxPduRef = 0;
           sigGWRxPduRef < Com_gConfigPtr->ComSigGWRxIPduArraySize;
           sigGWRxPduRef++) */
   }                            /* if (Com_Status == COM_EB_INIT) */

}                               /* Com_MainFunctionRouteSignals_Src_Dest_Generic */

/* AUTOSAR declares Com_MainFunctionRouteSignals_Src_<MainFunctionName_Src>() also in SchM_Com.h which leads to
   "MISRA-C:2012 Rule 08.5" violation. */
FUNC(void, COM_CODE) Com_MainFunctionRouteSignals_Src_Generic
(

   Com_MainFunctionRx_IdxType ComMainFunctionsRx_Idx

)
{

   if (Com_InitStatus == COM_EB_INIT)
   {

      uint16 sigGWRxPduRef; /* iteration variable */

      for (sigGWRxPduRef = 0U;
               sigGWRxPduRef < Com_gConfigPtr->ComSigGWRxIPduArraySize;
               sigGWRxPduRef++)
      {
         /* get Id of IPdu */
         const ComIPduRefType IPduId = (ComIPduRefType) * (COM_GET_CONFIG_ELEMENT
                  (ComIPduRefType,(Com_gConfigPtr->ComSigGWRxIPduListRef), sigGWRxPduRef));

         /* get RxPduId configuration reference */
         CONSTP2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr =
                  COM_GET_CONFIG_ELEMENT(ComRxIPduType, (Com_gConfigPtr->ComRxIPduRef), IPduId);


         if ( IPduPtr->ComMainFunctionsRx_Idx == ComMainFunctionsRx_Idx )
         {

            /* check if SigGWFlag is set */
            if( Com_IsBitSet_8(COM_RX_IPDU_SIG_GW_FLAG_ADDRESS(IPduPtr),
                     COM_RX_IPDU_SIG_GW_FLAG_BIT(IPduPtr)) )
            {

               /* clear the SigGWFlag*/
               COM_AtomicClearBit_8(COM_RX_IPDU_SIG_GW_FLAG_ADDRESS(IPduPtr),
                        COM_RX_IPDU_SIG_GW_FLAG_BIT(IPduPtr));
               {
                  /* get data pointer of I-PDU */
                  P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) pdudataptr =
                           &Com_gDataMemPtr[COM_EB_GET_SRCVALUE_REF(IPduPtr)];

                  P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) MultiGWpdudataptr =
                           &Com_gDataMemPtr[COM_EB_MULTIGW_GET_SRC_VALUE_SHADOW_REF(IPduPtr)];

                  ComIPduSizeType pdulength = IPduPtr->ComIPduSizeMax;

                  Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();
                  {
                     /* copy the Pdu */
                     TS_MemCpy(MultiGWpdudataptr, pdudataptr, pdulength);

                     /* set the length of the I-Pdu */
                     /* Deviation MISRAC2012-1 <+2> */
                     COM_EB_MULTIGW_PDULENGTH(IPduId, IPduPtr) =
                              COM_GET_RX_IPDU_SIZE(IPduId, IPduPtr);
                  }
                  Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

                  COM_EB_MULTIGW_INCREASE_SRC_COUNT(IPduPtr);


               }

            }
         }


      }                         /* (      for (sigGWRxPduRef = 0;
           sigGWRxPduRef < Com_gConfigPtr->ComSigGWRxIPduArraySize;
           sigGWRxPduRef++) */
   }                            /* if (Com_Status == COM_EB_INIT) */

}                               /* Com_MainFunctionRouteSignals_Src_Generic */
#endif /* (COM_MULTI_ROUTE_MAIN_ENABLE == STD_ON) */

#endif /* (COM_SIGNAL_GW_ENABLE == STD_ON) */


#if (COM_MULTI_ROUTE_MAIN_ENABLE == STD_OFF)

/**
 * \brief Com_MainFunctionRouteSignals - handle cyclic Signal Gateway tasks
 *
 * This function handles cyclic receiving and sending (group)signals(group)
 * for the Signal Gateway functionality (SigGW).
 * Preconditions:
 * - COM must be initialized
 *
 *
 * \ServiceID{0x1A}
 * \Reentrancy{Non re-entrant}
 * \Synchronicity{Synchronous}
 */
FUNC(void, COM_CODE) Com_MainFunctionRouteSignals
(
   void
)
{

   DBG_COM_MAINFUNCTIONROUTESIGNALS_ENTRY();
#if (COM_SIGNAL_GW_ENABLE == STD_ON)
   if (Com_InitStatus == COM_EB_INIT)
   {

      uint16 sigGWRxPduRef; /* iteration variable */

      for (sigGWRxPduRef = 0U;
               sigGWRxPduRef < Com_gConfigPtr->ComSigGWRxIPduArraySize;
               sigGWRxPduRef++)
      {
         /* get Id of IPdu */
         const ComIPduRefType IPduId = (ComIPduRefType) * (COM_GET_CONFIG_ELEMENT
                  (ComIPduRefType,(Com_gConfigPtr->ComSigGWRxIPduListRef), sigGWRxPduRef));

         /* get RxPduId configuration reference */
         CONSTP2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr =
                  COM_GET_CONFIG_ELEMENT(ComRxIPduType, (Com_gConfigPtr->ComRxIPduRef), IPduId);

         /* check if SigGWFlag is set */
         if(Com_IsBitSet_8(COM_RX_IPDU_SIG_GW_FLAG_ADDRESS(IPduPtr),
                  COM_RX_IPDU_SIG_GW_FLAG_BIT(IPduPtr)) )
         {

            /* clear the SigGWFlag*/
            COM_AtomicClearBit_8(COM_RX_IPDU_SIG_GW_FLAG_ADDRESS(IPduPtr),
                     COM_RX_IPDU_SIG_GW_FLAG_BIT(IPduPtr));

            Com_MainFunctionRouteSignals_Src_Generic_hlp(IPduPtr);

         }


      }                         /* (      for (sigGWRxPduRef = 0;
           sigGWRxPduRef < Com_gConfigPtr->ComSigGWRxIPduArraySize;
           sigGWRxPduRef++) */
   }                            /* if (Com_Status == COM_EB_INIT) */
#endif /* (COM_SIGNAL_GW_ENABLE == STD_ON) */

   DBG_COM_MAINFUNCTIONROUTESIGNALS_EXIT();
}                               /* Com_MainFunctionRouteSignals */

#endif /* (COM_MULTI_ROUTE_MAIN_ENABLE == STD_OFF) */

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
#endif

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/

