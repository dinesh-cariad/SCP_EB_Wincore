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
 * MISRAC2012-2) Deviated Rule: 11.5 (advisory)
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
#include <Com_Lcfg_Static.h>    /* declaration of the callback arrays */
#include <Com_Priv.h>           /* Module private API */

#include <Com_Core_TxIPdu.h>    /* Header TxIPdu functions */
#include <Com_Core_RxIPdu.h>    /* Header RxIPdu functions */

#include <Com_Core_Read.h>      /* de-serialization library */


#if ((COM_TM_AVAILABLE == STD_ON) || (COM_TMS_ENABLE == STD_ON))
#include <Com_Core_TM.h> /* Transmission Mode functions */
#endif /* ((COM_TM_AVAILABLE == STD_ON) || (COM_TMS_ENABLE == STD_ON)) */

#if (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL)
#include <Com_Core_RxUpdateBit.h> /* UpdateBit Functions */
#endif /* (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) */

#if ((COM_FILTER_RECEIVER_ENABLE == STD_ON) || \
      ((COM_TMS_ENABLE == STD_ON) && (COM_SIGNAL_GW_ENABLE == STD_ON)))
#include <Com_Filter.h>
#endif /* ((COM_FILTER_RECEIVER_ENABLE == STD_ON) || \
      ((COM_TMS_ENABLE == STD_ON) && (COM_SIGNAL_GW_ENABLE == STD_ON))) */

/*==================[version check]=========================================*/

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*------------------[version constants definition]--------------------------*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/


#define COM_START_SEC_VAR_INIT_8BIT
#include <Com_MemMap.h>

/** \brief Com_InitStatus
 * global variable for the actual initialization status of the Com */
VAR(Com_EB_StatusType, COM_VAR) Com_InitStatus = COM_EB_UNINIT;

#define COM_STOP_SEC_VAR_INIT_8BIT
#include <Com_MemMap.h>

#if (COM_GLOBAL_TIME == COM_SIZE_8_BIT)

#define COM_START_SEC_VAR_CLEARED_8BIT
#include <Com_MemMap.h>

/** \brief Com_GlobalTime
 * time in COM specific ticks */
#if (COM_TIMEBASE == STD_ON)
VAR(ComGlobalTimeType, COM_VAR_CLEARED) Com_GlobalTime;
#else
VAR(ComGlobalTimeType_Tx, COM_VAR_CLEARED) Com_GlobalTimes_Tx;
VAR(ComGlobalTimeType_Rx, COM_VAR_CLEARED) Com_GlobalTimes_Rx;
#endif /* (COM_TIMEBASE == STD_ON) */

#define COM_STOP_SEC_VAR_CLEARED_8BIT
#include <Com_MemMap.h>

#endif /* (COM_GLOBAL_TIME == COM_SIZE_8_BIT) */


#if (COM_GLOBAL_TIME == COM_SIZE_16_BIT)

#define COM_START_SEC_VAR_CLEARED_16BIT
#include <Com_MemMap.h>

/** \brief Com_GlobalTime
 * time in COM specific ticks */
#if (COM_TIMEBASE == STD_ON)
VAR(ComGlobalTimeType, COM_VAR_CLEARED) Com_GlobalTime;
#else
VAR(ComGlobalTimeType_Tx, COM_VAR_CLEARED) Com_GlobalTimes_Tx;
VAR(ComGlobalTimeType_Rx, COM_VAR_CLEARED) Com_GlobalTimes_Rx;
#endif /* (COM_TIMEBASE == STD_ON) */

#define COM_STOP_SEC_VAR_CLEARED_16BIT
#include <Com_MemMap.h>

#endif /* (COM_GLOBAL_TIME == COM_SIZE_16_BIT) */


#if (COM_GLOBAL_TIME == COM_SIZE_32_BIT)

#define COM_START_SEC_VAR_CLEARED_32BIT
#include <Com_MemMap.h>

/** \brief Com_GlobalTime
 * time in COM specific ticks */
#if (COM_TIMEBASE == STD_ON)
VAR(ComGlobalTimeType, COM_VAR_CLEARED) Com_GlobalTime;
#else
VAR(ComGlobalTimeType_Tx, COM_VAR_CLEARED) Com_GlobalTimes_Tx;
VAR(ComGlobalTimeType_Rx, COM_VAR_CLEARED) Com_GlobalTimes_Rx;
#endif /* (COM_TIMEBASE == STD_ON) */

#define COM_STOP_SEC_VAR_CLEARED_32BIT
#include <Com_MemMap.h>

#endif /* (COM_GLOBAL_TIME == COM_SIZE_32_BIT) */


/*
 * if not a constant configuration address is used - define pointer variable
 */
#if (COM_CONST_CFG_ADDRESS_ENABLE == STD_OFF)


#define COM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Com_MemMap.h>

/* CHECK: RULE 506 OFF */
/* Check_C disabled. Com_gConfigPtr is not renamed because of readability and severity of impact to benefit ratio. */
/** \brief
 * global variable for the pointer to the config of Com
 */
P2CONST(Com_ConfigType, COM_VAR_CLEARED, COM_APPL_CONST) Com_gConfigPtr;
/* CHECK: RULE 506 ON */

#define COM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Com_MemMap.h>

#endif /* COM_CONST_CFG_ADDRESS_ENABLE == STD_OFF */


/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#define COM_START_SEC_CODE
#include <Com_MemMap.h>



#if ((COM_FILTER_RECEIVER_ENABLE == STD_ON) || \
      ((COM_TMS_ENABLE == STD_ON) && (COM_SIGNAL_GW_ENABLE == STD_ON)))


_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_CalculateFilter_Bool
(
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr
);

_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_CalculateFilter_Uint8
(
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr
);

_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_CalculateFilter_Uint16
(
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr
);

_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_CalculateFilter_Uint32
(
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr
);

_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_CalculateFilter_Sint8
(
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr
);

_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_CalculateFilter_Sint16
(
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr
);

_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_CalculateFilter_Sint32
(
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr
);

_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_CalculateFilter_Uint64
(
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr
);

_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_CalculateFilter_Sint64
(
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr
);

#endif  /* ((COM_FILTER_RECEIVER_ENABLE == STD_ON) || \
      ((COM_TMS_ENABLE == STD_ON) && (COM_SIGNAL_GW_ENABLE == STD_ON))) */


#if ((COM_FILTER_RECEIVER_ENABLE == STD_ON) || \
      ((COM_TMS_ENABLE == STD_ON) && (COM_SIGNAL_GW_ENABLE == STD_ON)))


_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_CalculateFilter_Bool
(
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr
)
{
   ComSignalDescBitType  const ComSignalPosition = Com_EB_ReadPropPtr->ComSignalPositionInBit;

   uint8 MSB;
   uint8 tmp_val = dataPtr[ComSignalPosition / 8U];
   TS_GetBit(&tmp_val, ComSignalPosition % 8U, uint8, MSB);

   return Com_EB_FilterBoolean(ComFilterPropPtr, &MSB);
}

_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_CalculateFilter_Uint8
(
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr
)
{
   uint8 value;
   uint8 retval = COM_SERVICE_NOT_AVAILABLE;

   Com_EB_Read_Core(&value,dataPtr,Com_EB_ReadPropPtr, &retval);

   return Com_EB_FilterUint8(ComFilterPropPtr, &value);
}

_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_CalculateFilter_Uint16
(
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr
)
{
   uint16 value;
   uint8 retval = COM_SERVICE_NOT_AVAILABLE;

   Com_EB_Read_Core(&value,dataPtr,Com_EB_ReadPropPtr, &retval);

   return Com_EB_FilterUint16(ComFilterPropPtr, &value);
}

_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_CalculateFilter_Uint32
(
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr
)
{
   uint32 value;
   uint8 retval = COM_SERVICE_NOT_AVAILABLE;

   Com_EB_Read_Core(&value,dataPtr,Com_EB_ReadPropPtr, &retval);

   return Com_EB_FilterUint32(ComFilterPropPtr, &value);
}

_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_CalculateFilter_Sint8
(
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr
)
{
   sint8 value;
   uint8 retval = COM_SERVICE_NOT_AVAILABLE;

   Com_EB_Read_Core(&value,dataPtr,Com_EB_ReadPropPtr, &retval);

   return Com_EB_FilterSint8(ComFilterPropPtr, &value);
}

_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_CalculateFilter_Sint16
(
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr
)
{
   sint16 value;
   uint8 retval = COM_SERVICE_NOT_AVAILABLE;

   Com_EB_Read_Core(&value,dataPtr,Com_EB_ReadPropPtr, &retval);

   return Com_EB_FilterSint16(ComFilterPropPtr, &value);
}

_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_CalculateFilter_Sint32
(
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr
)
{
   sint32 value;
   uint8 retval = COM_SERVICE_NOT_AVAILABLE;

   Com_EB_Read_Core(&value,dataPtr,Com_EB_ReadPropPtr, &retval);

   return Com_EB_FilterSint32(ComFilterPropPtr, &value);
}

_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_CalculateFilter_Uint64
(
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr
)
{
   ComEmu64Type value;
   uint8 retval = COM_SERVICE_NOT_AVAILABLE;

   Com_EB_Read_Core(&value,dataPtr,Com_EB_ReadPropPtr, &retval);

   return Com_EB_FilterUint64(ComFilterPropPtr, &value);
}

_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_CalculateFilter_Sint64
(
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr
)
{
   ComEmu64Type value;
   uint8 retval = COM_SERVICE_NOT_AVAILABLE;

   Com_EB_Read_Core(&value,dataPtr,Com_EB_ReadPropPtr, &retval);

   return Com_EB_FilterSint64(ComFilterPropPtr, &value);
}

/* Deviation MISRAC2012-1 */
TS_MOD_PRIV_DEFN FUNC(boolean, COM_CODE) Com_EB_CalculateFilter
(
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr
)
{
   boolean FilterResult = FALSE;
   Com_TS_SignalTypeType const SignalType = COM_GET_RX_SIGNAL_SIGNAL_TYPE(Com_EB_ReadPropPtr);


   switch (SignalType)
   {

   case COM_BOOLEAN:
   {
      FilterResult = Com_EB_CalculateFilter_Bool(Com_EB_ReadPropPtr, dataPtr, ComFilterPropPtr);
      break;
   }                            /* case COM_BOOLEAN: */

   case COM_UINT8:
   {
      FilterResult = Com_EB_CalculateFilter_Uint8(Com_EB_ReadPropPtr, dataPtr, ComFilterPropPtr);
      break;
   }                            /* case COM_UINT8: */

   case COM_UINT16:
   {
      FilterResult = Com_EB_CalculateFilter_Uint16(Com_EB_ReadPropPtr, dataPtr, ComFilterPropPtr);
      break;
   }                            /* case COM_UINT16: */

   case COM_UINT32:
   {
      FilterResult = Com_EB_CalculateFilter_Uint32(Com_EB_ReadPropPtr, dataPtr, ComFilterPropPtr);
      break;
   }                            /* case COM_UINT32: */

   case COM_SINT8:
   {
      FilterResult = Com_EB_CalculateFilter_Sint8(Com_EB_ReadPropPtr, dataPtr, ComFilterPropPtr);
      break;
   }                            /* case COM_SINT8: */

   case COM_SINT16:
   {
      FilterResult = Com_EB_CalculateFilter_Sint16(Com_EB_ReadPropPtr, dataPtr, ComFilterPropPtr);
      break;
   }                            /* case COM_SINT16: */

   case COM_SINT32:
   {
      FilterResult = Com_EB_CalculateFilter_Sint32(Com_EB_ReadPropPtr, dataPtr, ComFilterPropPtr);
      break;
   }                            /* case COM_SINT32: */

   case COM_SINT64:
   {

     FilterResult = Com_EB_CalculateFilter_Sint64(Com_EB_ReadPropPtr, dataPtr, ComFilterPropPtr);
     break;
   }                            /* case COM_SINT64: */

   case COM_UINT64:
   {

     FilterResult = Com_EB_CalculateFilter_Uint64(Com_EB_ReadPropPtr, dataPtr, ComFilterPropPtr);
     break;
   }                            /* case COM_UINT64: */

   /* CHECK: NOPARSE */
   default:
   {
       COM_UNREACHABLE_CODE_ASSERT(COM_INTERNAL_API_ID);
   }
   break;
   /* CHECK: PARSE */
   }


   return FilterResult;
}                               /* Com_EB_CalculateFilter */

#endif /* ((COM_FILTER_RECEIVER_ENABLE == STD_ON) || \
      ((COM_TMS_ENABLE == STD_ON) && (COM_SIGNAL_GW_ENABLE == STD_ON))) */



#if ((COM_RX_SIGNAL_BUFFER_AVAILABLE == STD_ON) || \
      (COM_TX_SIGNAL_BUFFER_AVAILABLE == STD_ON))

/** \brief Com_EB_SetBuffer - sets the signal buffer with the value of the singal
 * in the I-Pdu
 * Preconditions:
 * - prior to this call, the COM module shall be initialized
 * \param[in] Com_EB_ReadPropPtr - properties of the signal
 */
/* Deviation MISRAC2012-1 */
TS_MOD_PRIV_DEFN FUNC(void, COM_CODE) Com_EB_SetBuffer
(
   P2CONST(ComSignalBaseType, AUTOMATIC, COM_APPL_CONST) Com_EB_ReadPropPtr
)
{

   CONSTP2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr =
            &Com_gDataMemPtr[Com_EB_ReadPropPtr->ComBufferRef];

   Com_TS_SignalTypeType const SignalType = COM_GET_RX_SIGNAL_SIGNAL_TYPE(Com_EB_ReadPropPtr);

   ComSignalBufferRefType const ValueRef = Com_EB_ReadPropPtr->ComFilterConf.ComFilterSignalValueRef;


   switch (SignalType)
   {

      case COM_BOOLEAN:
         {
            CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) bufferBaseBool =
               (P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED))
               & Com_gDataMemPtr[Com_gConfigPtr->ComBufferBaseBool];

            boolean boolVal;
            uint8 retval = COM_SERVICE_NOT_AVAILABLE;

            /* get the value using the library */
            Com_EB_Read_Core(&boolVal,dataPtr,Com_EB_ReadPropPtr, &retval);

            if ((boolean) boolVal == 1U)
            {
               COM_AtomicSetBit_8(&bufferBaseBool[(ValueRef / 8U)], (ValueRef % 8U));
            }
            else
            {
               COM_AtomicClearBit_8(&bufferBaseBool[(ValueRef / 8U)], (ValueRef % 8U));
            }
         }
         break;

      case COM_UINT8:
         {
            CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) bufferBaseUint8 =
               (P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED))
               & Com_gDataMemPtr[Com_gConfigPtr->ComBufferBaseUInt8];

            uint8 uint8Val;
            uint8 retval = COM_SERVICE_NOT_AVAILABLE;

            /* get the value using the library */
            Com_EB_Read_Core(&uint8Val,dataPtr,Com_EB_ReadPropPtr, &retval);

            TS_AtomicAssign8(bufferBaseUint8[ValueRef], uint8Val);
         }
         break;

      case COM_UINT16:
         {
            /* Deviation MISRAC2012-2 <+2> */
            CONSTP2VAR(uint16, AUTOMATIC, COM_VAR_CLEARED) bufferBaseUint16 =
                COM_GET_MEMPTR(uint16, Com_gConfigPtr->ComBufferBaseUInt16);

            uint16 uint16Val;
            uint8 retval = COM_SERVICE_NOT_AVAILABLE;

            /* get the value using the library */
            Com_EB_Read_Core(&uint16Val,dataPtr,Com_EB_ReadPropPtr, &retval);

            TS_AtomicAssign16(bufferBaseUint16[ValueRef], uint16Val);
         }
         break;

      case COM_UINT32:
         {
            /* Deviation MISRAC2012-2 <+2> */
            CONSTP2VAR(uint32, AUTOMATIC, COM_VAR_CLEARED) bufferBaseUint32 =
                COM_GET_MEMPTR(uint32, Com_gConfigPtr->ComBufferBaseUInt32);

            uint32 uint32Val;
            uint8 retval = COM_SERVICE_NOT_AVAILABLE;

            /* get the value using the library */
            Com_EB_Read_Core(&uint32Val,dataPtr,Com_EB_ReadPropPtr, &retval);

            TS_AtomicAssign32(bufferBaseUint32[ValueRef], uint32Val);
         }
         break;

      case COM_SINT8:
         {
            CONSTP2VAR(sint8, AUTOMATIC, COM_VAR_CLEARED) bufferBaseSint8 =
               (P2VAR(sint8, AUTOMATIC, COM_VAR_CLEARED))
               & Com_gDataMemPtr[Com_gConfigPtr->ComBufferBaseSInt8];

            sint8 sint8Val;
            uint8 retval = COM_SERVICE_NOT_AVAILABLE;

            /* get the value using the library */
            Com_EB_Read_Core(&sint8Val,dataPtr,Com_EB_ReadPropPtr, &retval);

            TS_AtomicAssign8(bufferBaseSint8[ValueRef], sint8Val);
         }
         break;

      case COM_SINT16:
         {
            /* Deviation MISRAC2012-2 <+2> */
            CONSTP2VAR(sint16, AUTOMATIC, COM_VAR_CLEARED) bufferBaseSint16 =
                COM_GET_MEMPTR(sint16, Com_gConfigPtr->ComBufferBaseSInt16);

            sint16 sint16Val;
            uint8 retval = COM_SERVICE_NOT_AVAILABLE;

            /* get the value using the library */
            Com_EB_Read_Core(&sint16Val,dataPtr,Com_EB_ReadPropPtr, &retval);

            TS_AtomicAssign16(bufferBaseSint16[ValueRef], sint16Val);
         }
         break;

      case COM_SINT32:
         {
            /* Deviation MISRAC2012-2 <+2> */
            CONSTP2VAR(sint32, AUTOMATIC, COM_VAR_CLEARED) bufferBaseSint32 =
                COM_GET_MEMPTR(sint32, Com_gConfigPtr->ComBufferBaseSInt32);

            sint32 sint32Val;
            uint8 retval = COM_SERVICE_NOT_AVAILABLE;

            /* get the value using the library */
            Com_EB_Read_Core(&sint32Val,dataPtr,Com_EB_ReadPropPtr, &retval);

            TS_AtomicAssign32(bufferBaseSint32[ValueRef], sint32Val);
         }
         break;

      case COM_SINT64:
        /* fall through */
      case COM_UINT64:
         {
           /* for COM_SINT64 signal and COM_UINT64 types no signal buffers are allocated */
         }
         break;

         /* CHECK: NOPARSE */
      default:
          COM_UNREACHABLE_CODE_ASSERT(COM_INTERNAL_API_ID);
          break;
         /* CHECK: PARSE */
   }                            /* end switch */

   return;
}                               /* Com_EB_SetBufferUInt32 */

#endif /* ((COM_RX_SIGNAL_BUFFER_AVAILABLE == STD_ON) || \
      (COM_TX_SIGNAL_BUFFER_AVAILABLE == STD_ON)) */


#if (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON)
/** \brief Com_ResetFilerOneEveryN - resets the occurance value of a
 * OneEveryN filter
 * Preconditions:
 * - prior to this call, the COM module shall be initialized
 * - ComFilterRef has to address a ComOneEveryN configuration
 * \param[in] ComFilterRef - Filter which shall be reseted
 */
/* Deviation MISRAC2012-1 */
TS_MOD_PRIV_DEFN FUNC(void, COM_CODE) Com_ResetFilerOneEveryN
(
   ComFilterRefType ComFilterRef
)
{
   P2CONST(ComFilterOneEveryNType, AUTOMATIC, COM_APPL_CONST) ComFilterOneEveryNPtr;
   /* pointer to the start of array where the occurrence values  are stored */
   P2VAR(ComFilterOneEveryNSizeType, AUTOMATIC, COM_VAR_CLEARED) occurrencebase;


   ComFilterOneEveryNPtr =
         COM_GET_CONFIG_ELEMENT(ComFilterOneEveryNType,
                                Com_gConfigPtr->ComFilterOneEveryNRef,
                                ComFilterRef);

   /* get the start of the occurrence values */
   /* Deviation MISRAC2012-2 <+2> */
   occurrencebase =
       COM_GET_MEMPTR(ComFilterOneEveryNSizeType, Com_gConfigPtr->ComFilterOneEveryNBase);

   /* reset counter */
#if (COM_FILTER_ONE_EVERY_N_SIZE_TYPE_MAX == COM_SIZE_8_BIT)
   TS_AtomicAssign8(occurrencebase[ComFilterOneEveryNPtr->ComFilterOccurrence],
         0U);
#elif (COM_FILTER_ONE_EVERY_N_SIZE_TYPE_MAX == COM_SIZE_16_BIT)
   TS_AtomicAssign16(occurrencebase[ComFilterOneEveryNPtr->ComFilterOccurrence],
         0U);
#elif (COM_FILTER_ONE_EVERY_N_SIZE_TYPE_MAX == COM_SIZE_32_BIT)
   TS_AtomicAssign32(occurrencebase[ComFilterOneEveryNPtr->ComFilterOccurrence],
         0U);
#endif /* (COM_FILTER_ONE_EVERY_N_SIZE_TYPE_MAX == COM_SIZE_8_BIT) */

   return;
}                               /* Com_ResetFilerOneEveryN */

#endif /* (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON) */


#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>

#define COM_START_SEC_APPL_CODE
#include <Com_MemMap.h>

FUNC(void, COM_APPL_CODE) Com_EB_TxDMDummyCbk(void)
{
   return;
}

#define COM_STOP_SEC_APPL_CODE
#include <Com_MemMap.h>


/*==================[end of file]===========================================*/
