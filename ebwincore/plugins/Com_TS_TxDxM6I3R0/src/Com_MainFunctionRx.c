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

#include <Com_Core_RxIPdu.h>    /* Header TxIPdu functions */


#if (COM_RX_SIGNAL_BASED_DM == STD_ON)
#include <Com_Core_RxUpdateBit.h> /* UpdateBit Functions */
#endif /* (COM_RX_SIGNAL_BASED_DM == STD_ON) */


/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_START_SEC_CODE
#include <Com_MemMap.h>
#endif

#if (COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_CheckRxDMTable
(
#if (COM_TIMEBASE == STD_ON)
    void
#else
    Com_MainFunctionRx_IdxType ComMainFunctionsRx_Idx
#endif /* (COM_TIMEBASE == STD_ON) */
);
#endif /* (COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */

#if (COM_RX_SIGNAL_BASED_DM == STD_ON)

_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_CheckRxSignalDMTable
(
#if (COM_TIMEBASE == STD_ON)
    void
#else
    Com_MainFunctionRx_IdxType ComMainFunctionsRx_Idx
#endif /* (COM_TIMEBASE == STD_ON) */
);
#endif /* (COM_RX_SIGNAL_BASED_DM == STD_ON) */

#if (COM_RX_SIGNAL_BASED_DM == STD_ON)

_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_CheckRxSignalGroupDMTable
(
#if (COM_TIMEBASE == STD_ON)
    void
#else
    Com_MainFunctionRx_IdxType ComMainFunctionsRx_Idx
#endif /* (COM_TIMEBASE == STD_ON) */
);

#endif /* (COM_RX_SIGNAL_BASED_DM == STD_ON) */

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

#if (COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)

_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_CheckRxDMTable
(
#if (COM_TIMEBASE == STD_ON)
    void
#else
    Com_MainFunctionRx_IdxType ComMainFunctionsRx_Idx
#endif /* (COM_TIMEBASE == STD_ON) */
)
{
   /* variable to hold global time in data type of Com_RxDMNextDueTimeType */
   Com_RxDMNextDueTimeType globalTime;
   Com_RxDMNextDueTimeType CompTime;
   ComIPduRefType i;       /* iterator variable */


   /* no critical section needed here, the only Com_GlobalTime is update only in
    * context of Com_MainfunctionRx() and this function is also only invoked in context of
    * Com_MainfunctionRx() */
   globalTime = (Com_RxDMNextDueTimeType) COM_GET_Com_GlobalRxTime(ComMainFunctionsRx_Idx);

   for (i = 0; i < Com_gConfigPtr->ComRxTimeoutQueueMax; i++)
   {
      boolean invokeCbks = FALSE;

      P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr;
      /* Deviation MISRAC2012-1 <+2> */
      CONSTP2VAR(Com_RxDMNextDueTimeType, AUTOMATIC, COM_VAR_CLEARED) RxTimeoutQueueBase =
            COM_GET_MEMPTR(Com_RxDMNextDueTimeType, Com_gConfigPtr->ComRxTimeoutQueueBase);

      ComIPduRefType AktPduId;    /* temporary PDU-Id variable */
      /* fetch the PDU id of the actual entry */
      AktPduId =
            (ComIPduRefType) *
            (COM_GET_CONFIG_ELEMENT(ComIPduRefType, (Com_gConfigPtr->ComRxTimeoutQueueListRef), i));

      IPduPtr = COM_GET_CONFIG_ELEMENT(ComRxIPduType, Com_gConfigPtr->ComRxIPduRef, AktPduId);

#if (COM_TIMEBASE == STD_OFF)
      /* PDU relates to the corresponding MainFunction id
       * in order to be consistent with the globalTime used */
      if(IPduPtr->ComMainFunctionsRx_Idx == ComMainFunctionsRx_Idx)
      {
#endif /* (COM_TIMEBASE == STD_OFF) */

      /* Critical section; Com_TS_RxDMQueue may interrupt the
       * function at the wrong time; clearing of RXDMTIMEOUT may be lost */
      Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

      /* This entry may have timed out, if it had RxDM enabled */
      if ( Com_IsBitSet_8(&Com_gDataMemPtr[(Com_gConfigPtr->ComRxIPduStatusBase + AktPduId)],
          COM_RXPDU_RXDMRUNNING) )
      {
         /* get the time out */
         CompTime = RxTimeoutQueueBase[i];
         if (globalTime == CompTime)
         {

            /* Entry times out right now */
            Com_SetBitUInt8(&Com_gDataMemPtr[(Com_gConfigPtr->ComRxIPduStatusBase + AktPduId)],
                  COM_RXPDU_RXDMTIMEOUT);

            /* Restart RxDM at once */
            Com_EB_RxDMQueue(IPduPtr, COM_GET_RX_TIMEOUT_FACTOR(IPduPtr));

            /* call callbacks outside critical section */
            invokeCbks = TRUE;

         }                   /* if (globalTime == CompTime) */
      }                      /* if ( Com_IsBitSet_8(&Com_gDataMemPtr[(Com_gConfigPtr->ComRxIPduStatusBase +
                                     AktPduId)], COM_RXPDU_RXDMRUNNING) ) */

      /* LEAVE CRITICAL SECTION */
      Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

      if (TRUE == invokeCbks)
      {
         Com_CbkRxTOutIdxType j;  /* iterator variable */

         /* send timeout notifications */
         for (j = IPduPtr->ComRxTimeoutNotificationFirst;
               j < (IPduPtr->ComRxTimeoutNotificationFirst
                     + IPduPtr->ComRxTimeoutNotificationNum); j++)
         {
            /* call timeout notification */
            Com_CbkRxTOut_Array[COM_GET_CBK_RX_T_OUT_IDX(j)] ();
         }

      }                      /* if (TRUE == invokeCbks) */

#if (COM_TIMEBASE == STD_OFF)
      }                      /* if(IPduPtr->ComMainFunctionsRx_Idx == ComMainFunctionsRx_Idx) */
#endif /* (COM_TIMEBASE == STD_OFF) */

   }                         /* for (i = 0; i < Com_gConfigPtr->ComRxTimeoutQueueMax; i++) */

   return;
}                               /* Com_EB_CheckRxDMTable */
#endif /* (COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */


#if (COM_RX_SIGNAL_BASED_DM == STD_ON)

_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_CheckRxSignalDMTable
(
#if (COM_TIMEBASE == STD_ON)
    void
#else
    Com_MainFunctionRx_IdxType ComMainFunctionsRx_Idx
#endif /* (COM_TIMEBASE == STD_ON) */
)
{
   /* variable to hold global time in data type of Com_RxDMNextDueTimeType */
   Com_RxDMNextDueTimeType globalTime;
   Com_RxDMNextDueTimeType CompTime;
   ComSignalRefType i;       /* iterator variable */


   /* no critical section needed here, the only Com_GlobalTime is update only in
    * context of Com_MainfunctionRx() and this function is also only invoked in context of
    * Com_MainfunctionRx() */
    globalTime = (Com_RxDMNextDueTimeType) COM_GET_Com_GlobalRxTime(ComMainFunctionsRx_Idx);

      for (i = 0; i < Com_gConfigPtr->ComRxTimeoutSignalQueueMax; i++)
      {
         boolean invokeCbks = FALSE;

         /* Deviation MISRAC2012-1 <+2> */
         CONSTP2VAR(Com_RxDMNextDueTimeType, AUTOMATIC, COM_VAR_CLEARED) RxTimeoutSignalQueueBase =
             COM_GET_MEMPTR(Com_RxDMNextDueTimeType, Com_gConfigPtr->ComRxTimeoutSignalQueueBase);

         /* fetch the PDU id of the actual entry */
         ComSignalRefType const AktSignalId =
            (ComSignalRefType) *(COM_GET_CONFIG_ELEMENT(ComSignalRefType,
                  (Com_gConfigPtr->ComRxTimeoutSignalQueueListRef), i));

         /* reference to configuration of the signal */
         CONSTP2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr = COM_GET_CONFIG_ELEMENT(
                  ComRxSignalType, Com_gConfigPtr->ComRxSignalRef, AktSignalId);

         CONSTP2CONST(ComRxSignalExtType, AUTOMATIC, COM_APPL_CONST) SignalExtPtr =
                  COM_GET_CONFIG_ELEMENT(ComRxSignalExtType,
                           Com_gConfigPtr->ComRxSignalExtRef, SignalPtr->ComSignalExtRef);

#if (COM_TIMEBASE == STD_OFF)
         /* get referenced PDU ptr */
         ComIPduRefType const PduId = SignalPtr->ComIPduRef;
         CONSTP2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr =
                  COM_GET_CONFIG_ELEMENT(ComRxIPduType, Com_gConfigPtr->ComRxIPduRef, PduId);

      /* PDU relates to the corresponding MainFunction id
       * in order to be consistent with the globalTime used */
      if(IPduPtr->ComMainFunctionsRx_Idx == ComMainFunctionsRx_Idx)
      {
#endif /* (COM_TIMEBASE == STD_OFF) */

         /* Critical section; Com_TS_RxDMQueue may interrupt the
          * function at the wrong time; clearing of RXDMTIMEOUT may be lost */
         Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

         /* This entry may have timed out, if it had RxDM enabled */
         if ( Com_IsBitSet_8(COM_RX_SIGNAL_DM_FLAGS_BYTE_POS(SignalExtPtr),
             COM_RX_SIGNAL_DM_RUNNING_BIT_POS(SignalExtPtr)) )
         {
            /* get the time out */
            CompTime = RxTimeoutSignalQueueBase[i];
            if (globalTime == CompTime)
            {

               /* Entry times out right now */
               COM_AtomicSetBit_8(COM_RX_SIGNAL_DM_FLAGS_BYTE_POS(SignalExtPtr),
                     COM_RX_SIGNAL_DM_TIMEOUT_BIT_POS(SignalExtPtr));

               /* Restart RxDM at once */
               Com_EB_RxDMSignalQueue(SignalExtPtr, COM_GET_RX_SIGNAL_TIMEOUT_FACTOR(SignalExtPtr)
#if (COM_TIMEBASE == STD_OFF)
               , ComMainFunctionsRx_Idx
#endif /* (COM_TIMEBASE == STD_ON) */
                 );
               /* call callbacks outside critical section */
               invokeCbks = TRUE;

            }                   /* if (globalTime == CompTime) */
         }                      /* if ( Com_IsBitSet_8(COM_RX_SIGNAL_DM_FLAGS_BYTE_POS(SignalExtPtr),
                                   COM_RX_SIGNAL_DM_RUNNING_BIT_POS(SignalExtPtr)) ) */

         /* LEAVE CRITICAL SECTION */
         Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

         if (TRUE == invokeCbks)
         {
            /* check if a callback is configured */
            if (SignalExtPtr->ComRxTimeoutNotification != COM_CBK_RX_T_OUT_IDX_FUNC_PTR_NULL)
            {
               /* call timeout notification */
               Com_CbkRxTOut_Array[SignalExtPtr->ComRxTimeoutNotification] ();
            }
         }                      /* if (TRUE == invokeCbks) */

#if (COM_TIMEBASE == STD_OFF)
      }                      /* if(IPduPtr->ComMainFunctionsRx_Idx == ComMainFunctionsRx_Idx) */
#endif /* (COM_TIMEBASE == STD_OFF) */

      }                         /* for (i = 0; i < Com_gConfigPtr->ComRxTimeoutSignalQueueMax; i++) */

   return;
}                               /* Com_EB_CheckRxSignalDMTable */
#endif /* (COM_RX_SIGNAL_BASED_DM == STD_ON) */

#if (COM_RX_SIGNAL_BASED_DM == STD_ON)
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_CheckRxSignalGroupDMTable
(
#if (COM_TIMEBASE == STD_ON)
      void
#else
      Com_MainFunctionRx_IdxType ComMainFunctionsRx_Idx
#endif /* (COM_TIMEBASE == STD_ON) */
)
{
   /* variable to hold global time in data type of Com_RxDMNextDueTimeType */
   Com_RxDMNextDueTimeType globalTime;
   Com_RxDMNextDueTimeType CompTime;
   ComSignalGroupRefType i;       /* iterator variable */


   /* no critical section needed here, the only Com_GlobalTime is update only in
    * context of Com_MainfunctionRx() and this function is also only invoked in context of
    * Com_MainfunctionRx() */
   globalTime = (Com_RxDMNextDueTimeType) COM_GET_Com_GlobalRxTime(ComMainFunctionsRx_Idx);

   for (i = 0; i < Com_gConfigPtr->ComRxTimeoutSignalGroupQueueMax; i++)
   {
      boolean invokeCbks = FALSE;

      /* Deviation MISRAC2012-1 <+4> */
      CONSTP2VAR(Com_RxDMNextDueTimeType, AUTOMATIC, COM_VAR_CLEARED)
      RxTimeoutSignalGroupQueueBase =
            COM_GET_MEMPTR(Com_RxDMNextDueTimeType,
                  Com_gConfigPtr->ComRxTimeoutSignalGroupQueueBase);

      /* fetch the PDU id of the actual entry */
      ComSignalGroupRefType const AktSignalGId =
            (ComSignalGroupRefType) *(COM_GET_CONFIG_ELEMENT(ComSignalGroupRefType,
                  (Com_gConfigPtr->ComRxTimeoutSignalGroupQueueListRef), i));

      /* reference to configuration of the signal */
      P2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr =
               COM_GET_CONFIG_ELEMENT(ComRxSignalGroupType,
            Com_gConfigPtr->ComRxSignalGroupRef, AktSignalGId);

      CONSTP2CONST(ComRxSignalExtType, AUTOMATIC, COM_APPL_CONST) SignalExtPtr =
               COM_GET_CONFIG_ELEMENT(ComRxSignalExtType,
                        Com_gConfigPtr->ComRxSignalExtRef, SignalGPtr->ComSignalExtRef);

#if (COM_TIMEBASE == STD_OFF)
         /* get referenced PDU ptr */
         ComIPduRefType const PduId = SignalGPtr->ComIPduRef;
         CONSTP2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr =
                  COM_GET_CONFIG_ELEMENT(ComRxIPduType, Com_gConfigPtr->ComRxIPduRef, PduId);

      /* PDU relates to the corresponding MainFunction id
       * in order to be consistent with the globalTime used */
      if(IPduPtr->ComMainFunctionsRx_Idx == ComMainFunctionsRx_Idx)
      {
#endif /* (COM_TIMEBASE == STD_OFF) */

      /* Critical section; Com_TS_RxDMQueue may interrupt the
       * function at the wrong time; clearing of RXDMTIMEOUT may be lost */
      Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

      /* This entry may have timed out, if it had RxDM enabled */
      if ( Com_IsBitSet_8(COM_RX_SIGNAL_GROUP_DM_FLAGS_BYTE_POS(SignalExtPtr),
          COM_RX_SIGNAL_GROUP_DM_RUNNING_BIT_POS(SignalExtPtr)) )
      {
         /* get the time out */
         CompTime = RxTimeoutSignalGroupQueueBase[i];
         if (globalTime == CompTime)
         {

            /* Entry times out right now */
            COM_AtomicSetBit_8(COM_RX_SIGNAL_GROUP_DM_FLAGS_BYTE_POS(SignalExtPtr),
                  COM_RX_SIGNAL_GROUP_DM_TIMEOUT_BIT_POS(SignalExtPtr));

            /* Restart RxDM at once */
            Com_EB_RxDMSignalGroupQueue(
                  SignalGPtr,
                  COM_GET_RX_SIGNAL_GROUP_TIMEOUT_FACTOR(SignalExtPtr)
#if (COM_TIMEBASE == STD_OFF)
                  , ComMainFunctionsRx_Idx
#endif /* (COM_TIMEBASE == STD_OFF) */
             );

            /* call callbacks outside critical section */
            invokeCbks = TRUE;

         }                   /* if (globalTime == CompTime) */

      }                      /* if ( Com_IsBitSet_8(COM_RX_SIGNAL_GROUP_DM_FLAGS_BYTE_POS(SignalExtPtr),
                                COM_RX_SIGNAL_GROUP_DM_RUNNING_BIT_POS(SignalExtPtr)) ) */

      /* LEAVE CRITICAL SECTION */
      Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

      if (TRUE == invokeCbks)
      {

         /* check if a callback is configured */
         if (SignalExtPtr->ComRxTimeoutNotification != COM_CBK_RX_T_OUT_IDX_FUNC_PTR_NULL)
         {
            /* call timeout notification */
            Com_CbkRxTOut_Array[SignalExtPtr->ComRxTimeoutNotification] ();
         }
      }                      /* if (TRUE == invokeCbks) */

#if (COM_TIMEBASE == STD_OFF)
      }                      /* if(IPduPtr->ComMainFunctionsRx_Idx == ComMainFunctionsRx_Idx) */
#endif /* (COM_TIMEBASE == STD_OFF) */

   }                         /* for (i = 0; i < Com_gConfigPtr->ComRxTimeoutSignalGroupQueueMax; i++) */
   return;
}                               /* Com_EB_CheckRxSignalGroupDMTable */

#endif /* (COM_RX_SIGNAL_BASED_DM == STD_ON) */

#if (COM_TIMEBASE == STD_ON)
/**
 * \brief Com_MainFunctionRx - handle cyclic receiving-related tasks
 *
 * This function handles cyclic receiving-related tasks like reception deadline
 * monitoring.
 * Preconditions:
 * - COM must be initialized
 *
 *
 * \ServiceID{0x18}
 * \Reentrancy{Non re-entrant}
 * \Synchronicity{Synchronous}
 */
FUNC(void, COM_CODE) Com_MainFunctionRx
(
   void
)
{

DBG_COM_MAINFUNCTIONRX_ENTRY();

   if (Com_InitStatus == COM_EB_INIT)
   {

#if (COM_GLOBAL_TIME != COM_SIZE_0_BIT)
      /* Make the time tick */
      Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();
      Com_GlobalTime += 1;
      Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();
#endif /* (COM_GLOBAL_TIME != COM_SIZE_0_BIT) */

#if (COM_RX_SIG_CONF_DEFERRED_ENABLE == STD_ON)
#if (COM_NOTIFY_RECEIVERS_ENABLE == STD_ON)
      {
         uint16 i;
         uint16 RxPduCount = Com_gConfigPtr->ComRxIPduArraySize;

         for (i = 0; i < RxPduCount; i++)
         {

           P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr =
               COM_GET_CONFIG_ELEMENT(ComRxIPduType, Com_gConfigPtr->ComRxIPduRef, i);

#if (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON)
               if( FALSE == COM_GET_COM_RX_IPDU_DUMMY(IPduPtr) )
#endif /* (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) */
               {

                 if ( Com_IsBitSet_8(&Com_gDataMemPtr[(Com_gConfigPtr->ComRxIPduStatusBase + i)],
                     COM_RXPDU_RXDEFNOTIFY) )
                 {
                   COM_AtomicClearBit_8(&Com_gDataMemPtr[(Com_gConfigPtr->ComRxIPduStatusBase + i)],
                       COM_RXPDU_RXDEFNOTIFY);

                   /* Notify receivers (deferred) */
                   Com_EB_NotifyReceivers(IPduPtr, i);
                 }

               }
         }
      }
#endif /* (COM_NOTIFY_RECEIVERS_ENABLE == STD_ON) */
#endif /* (COM_RX_SIG_CONF_DEFERRED_ENABLE == STD_ON) */

#if (COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)
      /* See if Rx DM timed out */
      Com_EB_CheckRxDMTable();
#endif /* (COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */


#if (COM_RX_SIGNAL_BASED_DM == STD_ON)
      /* check if signal based Rx DM timed out */
      Com_EB_CheckRxSignalDMTable();

      /* check if signal group based DM timed out */
      Com_EB_CheckRxSignalGroupDMTable();

#endif /* (COM_RX_SIGNAL_BASED_DM == STD_ON) */

   }                            /* if (Com_Status == COM_EB_INIT) */

   DBG_COM_MAINFUNCTIONRX_EXIT();
   return;
}
#else
/* AUTOSAR declares Com_MainFunctionRx_<MainFunctionName_Src>() also in SchM_Com.h which leads to
   "MISRA-C:2012 Rule 08.5" violation. */

/** \brief Com_MainFunctionRx - handle cyclic sending-related tasks
 * This function handles cyclic sending-related tasks such as minimum delay
 * time and cyclic sending.
 * Preconditions:
 * - COM must be initialized
 * \param[in] ComMainFunctionsRx_Idx - Id of the main function
 */
FUNC(void, COM_CODE) Com_MainFunctionRx_Generic
(
    Com_MainFunctionRx_IdxType ComMainFunctionsRx_Idx
)
{

DBG_COM_MAINFUNCTIONRX_ENTRY();

   if (Com_InitStatus == COM_EB_INIT)
   {

#if (COM_DEV_ERROR_DETECT == STD_ON)
     if(ComMainFunctionsRx_Idx >= COM_MAIN_FUNCS_RX)
     {
       COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_MainFunctionRx);
     }
     else
#endif /* (COM_DEV_ERROR_DETECT == STD_ON) */
     {

#if (COM_GLOBAL_TIME != COM_SIZE_0_BIT)
      /* Make the time tick */
      Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

      Com_GlobalTimes_Rx[ComMainFunctionsRx_Idx] += 1;

      Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();
#endif /* (COM_GLOBAL_TIME != COM_SIZE_0_BIT) */

#if (COM_RX_SIG_CONF_DEFERRED_ENABLE == STD_ON)
#if (COM_NOTIFY_RECEIVERS_ENABLE == STD_ON)
      {
         ComIPduRefType PduId;
         ComIPduRefType PduIdCount = 0U;

         P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr;

         /* get main context information pointer for pdu processing */
         CONSTP2CONST(ComMainfunctionRxType, AUTOMATIC, COM_APPL_CONST) MainRxInfoPtr =
         COM_GET_CONFIG_ELEMENT(ComMainfunctionRxType,
                                Com_gConfigPtr->ComMainfunctionRxRef,
                                ComMainFunctionsRx_Idx);

         for (PduIdCount = 0U; PduIdCount < MainRxInfoPtr->ComMainPduIDListSize; PduIdCount++)
         {
           /* get starting element of pduId list from the corresponding Rx main function sector */
           PduId = * COM_GET_CONFIG_ELEMENT(ComIPduRefType, MainRxInfoPtr->ComMainPduIDListRef, PduIdCount);

           /* Notify receivers (deferred) */
           IPduPtr = COM_GET_CONFIG_ELEMENT(ComRxIPduType, Com_gConfigPtr->ComRxIPduRef, PduId);

            if ( Com_IsBitSet_8(&Com_gDataMemPtr[(Com_gConfigPtr->ComRxIPduStatusBase + PduId)],
                COM_RXPDU_RXDEFNOTIFY) )
            {
               COM_AtomicClearBit_8(&Com_gDataMemPtr[(Com_gConfigPtr->ComRxIPduStatusBase + PduId)],
                                 COM_RXPDU_RXDEFNOTIFY);

               Com_EB_NotifyReceivers(IPduPtr, PduId);
            }
         }
      }
#endif /* (COM_NOTIFY_RECEIVERS_ENABLE == STD_ON) */
#endif /* (COM_RX_SIG_CONF_DEFERRED_ENABLE == STD_ON) */

#if (COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)
      /* See if Rx DM timed out */
      Com_EB_CheckRxDMTable(ComMainFunctionsRx_Idx);
#endif /* (COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */


#if (COM_RX_SIGNAL_BASED_DM == STD_ON)
      /* check if signal based Rx DM timed out */
      Com_EB_CheckRxSignalDMTable(ComMainFunctionsRx_Idx);

      /* check if signal group based DM timed out */
      Com_EB_CheckRxSignalGroupDMTable(ComMainFunctionsRx_Idx);
#endif /* (COM_RX_SIGNAL_BASED_DM == STD_ON) */

     }                          /* if(ComMainFunctionsRx_Idx >= COM_MAIN_FUNCS_RX) */

   }                            /* if (Com_Status == COM_EB_INIT) */

   DBG_COM_MAINFUNCTIONRX_EXIT();
   return;
}
#endif /* (COM_TIMEBASE == STD_ON) */

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
#endif

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/
