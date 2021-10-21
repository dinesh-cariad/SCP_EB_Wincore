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
#include <Com_Filter.h>

#if (COM_FILTER_AVAILABLE == STD_ON)
#include <Com_Priv.h>           /* Module private API */
#include <Com_Core_Read_hlp.h>

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_START_SEC_CODE
#include <Com_MemMap.h>
#endif

#if (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON)

_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_FilterOneEveryN
(
      ComFilterRefType ComFilterRef
);

_COM_STATIC_ FUNC(boolean, COM_CODE) Com_EB_FilterOneEveryN
(
      ComFilterRefType ComFilterRef
)
{
   boolean FilterResult;
   /* Do OneEveryN-filtering */
   /* get reference to the structure where the data of the filter are
    * stored */
   CONSTP2CONST(ComFilterOneEveryNType, AUTOMATIC,
                COM_APPL_CONST) ComFilterOneEveryNPtr =
      COM_GET_CONFIG_ELEMENT(ComFilterOneEveryNType,
                             Com_gConfigPtr->ComFilterOneEveryNRef, ComFilterRef);
   ComFilterOneEveryNSizeType period;
   ComFilterOneEveryNSizeType offset;
   ComFilterOneEveryNSizeType occurrence;
   /* pointer to the start of array where the occurrence values  are stored */
   P2VAR(ComFilterOneEveryNSizeType, AUTOMATIC, COM_VAR_CLEARED) occurrencebase;


   /* get the configuration of the filter */
   offset = ComFilterOneEveryNPtr->ComFilterOffset;
   period = ComFilterOneEveryNPtr->ComFilterPeriodFactor;

   /* get the start of the occurrence values */
   /* Deviation MISRAC2012-1 <+2> */
   occurrencebase =
       COM_GET_MEMPTR(ComFilterOneEveryNSizeType, Com_gConfigPtr->ComFilterOneEveryNBase);

   /* Enter critical section */
   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   /* get the value how often the signal occurred */
   occurrence = occurrencebase[ComFilterOneEveryNPtr->ComFilterOccurrence];

   /* Evaluate if the filter allows the value to pass */
   FilterResult = (occurrence == offset);
   /* increase occurrence and store its new value */
   occurrence++;
   occurrencebase[ComFilterOneEveryNPtr->ComFilterOccurrence] = (occurrence % period);

   /* Leave critical section */
   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   return FilterResult;
}

#endif /* (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON) */


TS_MOD_PRIV_DEFN FUNC(boolean, COM_CODE) Com_EB_FilterBoolean
(
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr,
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
)
{
   boolean FilterResult = FALSE;
   uint8 NewVal = (uint8) * (P2CONST(boolean, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;


   switch (ComFilterPropPtr->ComFilterType)
   {
   case COM_F_MASK_NEW_DIFFERS_MASK_OLD:
   {
      /* pointer to mask */
      CONSTP2CONST(ComFilter8BitMaskType, AUTOMATIC, COM_APPL_CONST) maskPtr =
         COM_GET_CONFIG_ELEMENT(ComFilter8BitMaskType,
                                Com_gConfigPtr->ComFilter8BitMaskRef,
                                ComFilterPropPtr->ComFilterRef);
      uint8 mask = *maskPtr;
      /* pointer the old value */
         CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) bufferBaseBool =
            (P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED))
            & Com_gDataMemPtr[Com_gConfigPtr->ComBufferBaseBool];
         uint8 oldVal;
         TS_GetBit(&bufferBaseBool[(ComFilterPropPtr->ComFilterSignalValueRef / 8U)],
            ComFilterPropPtr->ComFilterSignalValueRef % 8U, uint8, oldVal);

      /* evaluate filter */
      FilterResult =
         ((( NewVal ^ ((uint8) oldVal)) &
           ((uint8) mask)) != 0);
   }                      /* case COM_F_MASK_NEW_DIFFERS_MASK_OLD: */
   break;
#if (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON)
   case COM_F_ONE_EVERY_N:
   {
      FilterResult = Com_EB_FilterOneEveryN(ComFilterPropPtr->ComFilterRef);
   }                            /* case COM_F_ONE_EVERY_N: */
   break;
#endif /* (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON) */
      default:
         {
            CONSTP2CONST(ComFilter8BitMaskXMinMaxType, AUTOMATIC,
                         COM_APPL_CONST) FilterPtr =
               COM_GET_CONFIG_ELEMENT(ComFilter8BitMaskXMinMaxType,
                                      Com_gConfigPtr->ComFilter8BitMaskXMinMaxRef,
                                      ComFilterPropPtr->ComFilterRef);
            uint8 maskOrMin = FilterPtr->ComFilterMaskOrMin;
            uint8 XOrMax = FilterPtr->ComFilterXOrMax;

            switch (ComFilterPropPtr->ComFilterType)
            {
            case COM_F_MASKED_NEW_EQUALS_X:
            {
               FilterResult =
                  (NewVal & (uint8) maskOrMin) ==
                  (uint8) XOrMax;
            }
            break;
            case COM_F_MASKED_NEW_DIFFERS_X:
            {
               FilterResult =
                  (NewVal & (uint8) maskOrMin) !=
                  (uint8) XOrMax;
            }
            break;
            /* CHECK: NOPARSE */
            default:
            {
               /* Should never be reached, FilterType is unknown */
                COM_UNREACHABLE_CODE_ASSERT(COM_INTERNAL_API_ID);
            }
            break;
            /* CHECK: PARSE */
            }
         }
         break;
   }                            /* end switch FilterType */

   /* Check if the value should be stored to the signal buffer */
   if ((FilterResult == TRUE) &&
       (ComFilterPropPtr->ComFilterSignalValueRef != COM_SIGNAL_BUFFER_REF_INVALID)
      )
   {
      CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) bufferBaseBool =
         (P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED))
         & Com_gDataMemPtr[Com_gConfigPtr->ComBufferBaseBool];
      if (NewVal == 1U)
      {
         COM_AtomicSetBit_8(&bufferBaseBool[(ComFilterPropPtr->ComFilterSignalValueRef / 8U)],
               ComFilterPropPtr->ComFilterSignalValueRef % 8U);
      }
      else
      {
         COM_AtomicClearBit_8(&bufferBaseBool[(ComFilterPropPtr->ComFilterSignalValueRef / 8U)],
               ComFilterPropPtr->ComFilterSignalValueRef % 8U);
      }
   }             /* if ((SaveResult == TRUE) && (ComFilterSignalValueRef != COM_SIGNAL_BUFFER_REF_INVALID)) */


   return FilterResult;

}                               /* Com_EB_FilterBoolean */

TS_MOD_PRIV_DEFN FUNC(boolean, COM_CODE) Com_EB_FilterUint8
(
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr,
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
)
{
   boolean FilterResult = FALSE;
   uint8 NewVal = (uint8) * (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;


   switch (ComFilterPropPtr->ComFilterType)
   {
   case COM_F_MASK_NEW_DIFFERS_MASK_OLD:
   {
      /* pointer to mask */
      CONSTP2CONST(ComFilter8BitMaskType, AUTOMATIC, COM_APPL_CONST) maskPtr =
         COM_GET_CONFIG_ELEMENT(ComFilter8BitMaskType,
                                Com_gConfigPtr->ComFilter8BitMaskRef,
                                ComFilterPropPtr->ComFilterRef);
      uint8 mask = *maskPtr;
      /* pointer the old value */
     CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) bufferBaseUint8 =
        (P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED))
        & Com_gDataMemPtr[Com_gConfigPtr->ComBufferBaseUInt8];
         uint8 oldVal;
         TS_AtomicAssign8(oldVal, (uint8) bufferBaseUint8[ComFilterPropPtr->ComFilterSignalValueRef]);
      /* evaluate filter */
      FilterResult =
         ((((uint8) NewVal ^ ((uint8) oldVal)) &
           ((uint8) mask)) != 0);
   }                      /* case COM_F_MASK_NEW_DIFFERS_MASK_OLD: */
   break;
#if (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON)
   case COM_F_ONE_EVERY_N:
   {
      FilterResult = Com_EB_FilterOneEveryN(ComFilterPropPtr->ComFilterRef);
   }                            /* case COM_F_ONE_EVERY_N: */
   break;
#endif /* (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON) */
      default:
         {
            CONSTP2CONST(ComFilter8BitMaskXMinMaxType, AUTOMATIC,
                         COM_APPL_CONST) FilterPtr =
               COM_GET_CONFIG_ELEMENT(ComFilter8BitMaskXMinMaxType,
                                      Com_gConfigPtr->ComFilter8BitMaskXMinMaxRef,
                                      ComFilterPropPtr->ComFilterRef);
            uint8 maskOrMin = FilterPtr->ComFilterMaskOrMin;
            uint8 XOrMax = FilterPtr->ComFilterXOrMax;

            switch (ComFilterPropPtr->ComFilterType)
            {
            case COM_F_MASKED_NEW_EQUALS_X:
            {
               FilterResult =
                  ((uint8) NewVal & (uint8) maskOrMin) ==
                  (uint8) XOrMax;
            }
            break;
            case COM_F_MASKED_NEW_DIFFERS_X:
            {
               FilterResult =
                  ((uint8) NewVal & (uint8) maskOrMin) !=
                  (uint8) XOrMax;
            }
            break;
            case COM_F_NEW_IS_WITHIN:
            {
              FilterResult = ((uint8) maskOrMin <= (uint8) NewVal)
                 && ((uint8) NewVal <= (uint8) XOrMax);
            }
            break;
            case COM_F_NEW_IS_OUTSIDE:
            {
               FilterResult = ((uint8) maskOrMin > (uint8) NewVal)
                  || ((uint8) NewVal > (uint8) XOrMax);
            }
            break;
            /* CHECK: NOPARSE */
            default:
            {
               /* Should never be reached, FilterType is unknown */
               COM_UNREACHABLE_CODE_ASSERT(COM_INTERNAL_API_ID);
            }
            break;
            /* CHECK: PARSE */
            }
         }
         break;
   }                            /* end switch FilterType */

   /* Check if the value should be stored to the signal buffer */
   if ((FilterResult == TRUE) &&
       (ComFilterPropPtr->ComFilterSignalValueRef != COM_SIGNAL_BUFFER_REF_INVALID)
      )
   {
      CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) bufferBaseUint8 =
         (P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED))
         & Com_gDataMemPtr[Com_gConfigPtr->ComBufferBaseUInt8];
      TS_AtomicAssign8(bufferBaseUint8[ComFilterPropPtr->ComFilterSignalValueRef], NewVal);

   }             /* if ((FilterResult == TRUE) && (ComFilterSignalValueRef != COM_SIGNAL_BUFFER_REF_INVALID)) */


   return FilterResult;

}                               /* Com_EB_FilterUint8 */

TS_MOD_PRIV_DEFN FUNC(boolean, COM_CODE) Com_EB_FilterSint8
(
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr,
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
)
{
   boolean FilterResult = FALSE;
   sint8 NewVal = (sint8) * (P2CONST(sint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;


   switch (ComFilterPropPtr->ComFilterType)
   {
   case COM_F_MASK_NEW_DIFFERS_MASK_OLD:
   {
      CONSTP2CONST(ComFilter8BitMaskType, AUTOMATIC, COM_APPL_CONST) maskPtr =
         COM_GET_CONFIG_ELEMENT(ComFilter8BitMaskType,
                                Com_gConfigPtr->ComFilter8BitMaskRef,
                                ComFilterPropPtr->ComFilterRef);
      uint8 mask = *maskPtr;
      /* pointer the old value */
     CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) bufferBaseSint8 =
        (P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED))
        & Com_gDataMemPtr[Com_gConfigPtr->ComBufferBaseSInt8];
         sint8 oldVal;
         TS_AtomicAssign8(oldVal, (sint8) bufferBaseSint8[ComFilterPropPtr->ComFilterSignalValueRef]);
      /* evaluate filter */
      FilterResult =
         ((((uint8) NewVal ^ ((uint8) oldVal)) &
           ((uint8) mask)) != 0);
   }                      /* case COM_F_MASK_NEW_DIFFERS_MASK_OLD: */
   break;
#if (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON)
   case COM_F_ONE_EVERY_N:
   {
      FilterResult = Com_EB_FilterOneEveryN(ComFilterPropPtr->ComFilterRef);
   }                            /* case COM_F_ONE_EVERY_N: */
   break;
#endif /* (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON) */
      default:
         {
            CONSTP2CONST(ComFilter8BitMaskXMinMaxType, AUTOMATIC,
                         COM_APPL_CONST) FilterPtr =
               COM_GET_CONFIG_ELEMENT(ComFilter8BitMaskXMinMaxType,
                                      Com_gConfigPtr->ComFilter8BitMaskXMinMaxRef,
                                      ComFilterPropPtr->ComFilterRef);
            uint8 maskOrMin = FilterPtr->ComFilterMaskOrMin;
            uint8 XOrMax = FilterPtr->ComFilterXOrMax;

            switch (ComFilterPropPtr->ComFilterType)
            {
            case COM_F_MASKED_NEW_EQUALS_X:
            {
               FilterResult =
                  ((uint8) NewVal & (uint8) maskOrMin) ==
                  (uint8) XOrMax;
            }
            break;
            case COM_F_MASKED_NEW_DIFFERS_X:
            {
               FilterResult =
                  ((uint8) NewVal & (uint8) maskOrMin) !=
                  (uint8) XOrMax;
            }
            break;
            case COM_F_NEW_IS_WITHIN:
            {
              FilterResult = ((sint8) maskOrMin <= (sint8) NewVal)
                 && ((sint8) NewVal <= (sint8) XOrMax);
            }
            break;
            case COM_F_NEW_IS_OUTSIDE:
            {
               FilterResult = ((sint8) maskOrMin > (sint8) NewVal)
                  || ((sint8) NewVal > (sint8) XOrMax);
            }
            break;
            /* CHECK: NOPARSE */
            default:
            {
               /* Should never be reached, FilterType is unknown */
                COM_UNREACHABLE_CODE_ASSERT(COM_INTERNAL_API_ID);
            }
            break;
            /* CHECK: PARSE */
            }
         }
         break;
   }                            /* end switch FilterType */

   /* Check if the value should be stored to the signal buffer */
   if ((FilterResult == TRUE) &&
       (ComFilterPropPtr->ComFilterSignalValueRef != COM_SIGNAL_BUFFER_REF_INVALID)
      )
   {
      CONSTP2VAR(sint8, AUTOMATIC, COM_VAR_CLEARED) bufferBaseSint8 =
         (P2VAR(sint8, AUTOMATIC, COM_VAR_CLEARED))
         & Com_gDataMemPtr[Com_gConfigPtr->ComBufferBaseSInt8];
      TS_AtomicAssign8(bufferBaseSint8[ComFilterPropPtr->ComFilterSignalValueRef], NewVal);

   }             /* if ((FilterResult == TRUE) && (ComFilterSignalValueRef != COM_SIGNAL_BUFFER_REF_INVALID)) */


   return FilterResult;

}                               /* Com_EB_FilterSint8 */

TS_MOD_PRIV_DEFN FUNC(boolean, COM_CODE) Com_EB_FilterUint16
(
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr,
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
)
{
   boolean FilterResult = FALSE;
   uint16 NewVal = (uint16) * (P2CONST(uint16, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;


   switch (ComFilterPropPtr->ComFilterType)
   {
   case COM_F_MASK_NEW_DIFFERS_MASK_OLD:
   {
      /* pointer to mask */
      CONSTP2CONST(ComFilter16BitMaskType, AUTOMATIC, COM_APPL_CONST) maskPtr =
         COM_GET_CONFIG_ELEMENT(ComFilter16BitMaskType,
                                Com_gConfigPtr->ComFilter16BitMaskRef,
                                ComFilterPropPtr->ComFilterRef);
      uint16 mask = *maskPtr;
      /* pointer the old value */
      /* Deviation MISRAC2012-1 <+2> */
      CONSTP2VAR(uint16, AUTOMATIC, COM_VAR_CLEARED) bufferBaseUint16 =
          COM_GET_MEMPTR(uint16, Com_gConfigPtr->ComBufferBaseUInt16);
      uint16 oldVal;
      TS_AtomicAssign16(oldVal, (uint16) bufferBaseUint16[ComFilterPropPtr->ComFilterSignalValueRef]);
      /* evaluate filter */
      FilterResult =
         ((((uint16) NewVal ^ ((uint16) oldVal)) &
           ((uint16) mask)) != 0);
   }                      /* case COM_F_MASK_NEW_DIFFERS_MASK_OLD: */
   break;
#if (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON)
   case COM_F_ONE_EVERY_N:
   {
      FilterResult = Com_EB_FilterOneEveryN(ComFilterPropPtr->ComFilterRef);
   }                            /* case COM_F_ONE_EVERY_N: */
   break;
#endif /* (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON) */
      default:
         {
            CONSTP2CONST(ComFilter16BitMaskXMinMaxType, AUTOMATIC,
                         COM_APPL_CONST) FilterPtr =
               COM_GET_CONFIG_ELEMENT(ComFilter16BitMaskXMinMaxType,
                                      Com_gConfigPtr->ComFilter16BitMaskXMinMaxRef,
                                      ComFilterPropPtr->ComFilterRef);
            uint16 maskOrMin = FilterPtr->ComFilterMaskOrMin;
            uint16 XOrMax = FilterPtr->ComFilterXOrMax;

            switch (ComFilterPropPtr->ComFilterType)
            {
            case COM_F_MASKED_NEW_EQUALS_X:
            {
               FilterResult =
                  ((uint16) NewVal & (uint16) maskOrMin) ==
                  (uint16) XOrMax;
            }
            break;
            case COM_F_MASKED_NEW_DIFFERS_X:
            {
               FilterResult =
                  ((uint16) NewVal & (uint16) maskOrMin) !=
                  (uint16) XOrMax;
            }
            break;
            case COM_F_NEW_IS_WITHIN:
            {
              FilterResult = ((uint16) maskOrMin <= (uint16) NewVal)
                 && ((uint16) NewVal <= (uint16) XOrMax);
            }
            break;
            case COM_F_NEW_IS_OUTSIDE:
            {
               FilterResult = ((uint16) maskOrMin > (uint16) NewVal)
                  || ((uint16) NewVal > (uint16) XOrMax);
            }
            break;
            /* CHECK: NOPARSE */
            default:
            {
               /* Should never be reached, FilterType is unknown */
               COM_UNREACHABLE_CODE_ASSERT(COM_INTERNAL_API_ID);
            }
            break;
            /* CHECK: PARSE */
            }
         }
         break;
   }                            /* end switch FilterType */

   /* Check if the value should be stored to the signal buffer */
   if ((FilterResult == TRUE) &&
       (ComFilterPropPtr->ComFilterSignalValueRef != COM_SIGNAL_BUFFER_REF_INVALID)
      )
   {
      /* Deviation MISRAC2012-1 <+2> */
      CONSTP2VAR(uint16, AUTOMATIC, COM_VAR_CLEARED) bufferBaseUint16 =
          COM_GET_MEMPTR(uint16, Com_gConfigPtr->ComBufferBaseUInt16);
      TS_AtomicAssign16(bufferBaseUint16[ComFilterPropPtr->ComFilterSignalValueRef], NewVal);

   }             /* if ((FilterResult == TRUE) && (ComFilterSignalValueRef != COM_SIGNAL_BUFFER_REF_INVALID)) */


   return FilterResult;

}                               /* Com_EB_FilterUint16 */

TS_MOD_PRIV_DEFN FUNC(boolean, COM_CODE) Com_EB_FilterSint16
(
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr,
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
)
{
   boolean FilterResult = FALSE;
   sint16 NewVal = (sint16) * (P2CONST(sint16, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;


   switch (ComFilterPropPtr->ComFilterType)
   {
   case COM_F_MASK_NEW_DIFFERS_MASK_OLD:
   {
      /* pointer to mask */
      CONSTP2CONST(ComFilter16BitMaskType, AUTOMATIC, COM_APPL_CONST) maskPtr =
         COM_GET_CONFIG_ELEMENT(ComFilter16BitMaskType,
                                Com_gConfigPtr->ComFilter16BitMaskRef,
                                ComFilterPropPtr->ComFilterRef);
      uint16 mask = *maskPtr;
      /* pointer the old value */
      /* Deviation MISRAC2012-1 <+2> */
      CONSTP2VAR(uint16, AUTOMATIC, COM_VAR_CLEARED) bufferBaseSint16 =
          COM_GET_MEMPTR(uint16, Com_gConfigPtr->ComBufferBaseSInt16);
      sint16 oldVal;
      TS_AtomicAssign16(oldVal, (sint16) bufferBaseSint16[ComFilterPropPtr->ComFilterSignalValueRef]);
      /* evaluate filter */
      FilterResult =
         ((((uint16) NewVal ^ ((uint16) oldVal)) &
           ((uint16) mask)) != 0);
}                      /* case COM_F_MASK_NEW_DIFFERS_MASK_OLD: */
   break;
#if (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON)
   case COM_F_ONE_EVERY_N:
   {
      FilterResult = Com_EB_FilterOneEveryN(ComFilterPropPtr->ComFilterRef);
   }                            /* case COM_F_ONE_EVERY_N: */
   break;
#endif /* (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON) */
      default:
         {
            CONSTP2CONST(ComFilter16BitMaskXMinMaxType, AUTOMATIC,
                         COM_APPL_CONST) FilterPtr =
               COM_GET_CONFIG_ELEMENT(ComFilter16BitMaskXMinMaxType,
                                      Com_gConfigPtr->ComFilter16BitMaskXMinMaxRef,
                                      ComFilterPropPtr->ComFilterRef);
            uint16 maskOrMin = FilterPtr->ComFilterMaskOrMin;
            uint16 XOrMax = FilterPtr->ComFilterXOrMax;

            switch (ComFilterPropPtr->ComFilterType)
            {
            case COM_F_MASKED_NEW_EQUALS_X:
            {
               FilterResult =
                  ((uint16) NewVal & (uint16) maskOrMin) ==
                  (uint16) XOrMax;
            }
            break;
            case COM_F_MASKED_NEW_DIFFERS_X:
            {
               FilterResult =
                  ((uint16) NewVal & (uint16) maskOrMin) !=
                  (uint16) XOrMax;
            }
            break;
            case COM_F_NEW_IS_WITHIN:
            {
              FilterResult = ((sint16) maskOrMin <= (sint16) NewVal)
                 && ((sint16) NewVal <= (sint16) XOrMax);
            }
            break;
            case COM_F_NEW_IS_OUTSIDE:
            {
               FilterResult = ((sint16) maskOrMin > (sint16) NewVal)
                  || ((sint16) NewVal > (sint16) XOrMax);
            }
            break;
            /* CHECK: NOPARSE */
            default:
            {
               /* Should never be reached, FilterType is unknown */
               COM_UNREACHABLE_CODE_ASSERT(COM_INTERNAL_API_ID);
            }
            break;
            /* CHECK: PARSE */
            }
         }
         break;
   }                            /* end switch FilterType */

   /* Check if the value should be stored to the signal buffer */
   if ((FilterResult == TRUE) &&
       (ComFilterPropPtr->ComFilterSignalValueRef != COM_SIGNAL_BUFFER_REF_INVALID)
      )
   {
      /* Deviation MISRAC2012-1 <+2> */
      CONSTP2VAR(sint16, AUTOMATIC, COM_VAR_CLEARED) bufferBaseSint16 =
          COM_GET_MEMPTR(sint16, Com_gConfigPtr->ComBufferBaseSInt16);
      TS_AtomicAssign16(bufferBaseSint16[ComFilterPropPtr->ComFilterSignalValueRef], NewVal);

   }             /* if ((FilterResult == TRUE) && (ComFilterSignalValueRef != COM_SIGNAL_BUFFER_REF_INVALID)) */


   return FilterResult;

}                               /* Com_EB_FilterSint16 */

TS_MOD_PRIV_DEFN FUNC(boolean, COM_CODE) Com_EB_FilterUint32
(
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr,
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
)
{
   boolean FilterResult = FALSE;
   uint32 NewVal = (uint32) * (P2CONST(uint32, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;


   switch (ComFilterPropPtr->ComFilterType)
   {
   case COM_F_MASK_NEW_DIFFERS_MASK_OLD:
   {
      /* pointer to mask */
      CONSTP2CONST(ComFilter32BitMaskType, AUTOMATIC, COM_APPL_CONST) maskPtr =
         COM_GET_CONFIG_ELEMENT(ComFilter32BitMaskType,
                                Com_gConfigPtr->ComFilter32BitMaskRef,
                                ComFilterPropPtr->ComFilterRef);
      uint32 mask = *maskPtr;
      /* pointer the old value */
      /* Deviation MISRAC2012-1 <+2> */
      CONSTP2VAR(uint32, AUTOMATIC, COM_VAR_CLEARED) bufferBaseUint32 =
          COM_GET_MEMPTR(uint32, Com_gConfigPtr->ComBufferBaseUInt32);
      uint32 oldVal;
      TS_AtomicAssign32(oldVal, (uint32) bufferBaseUint32[ComFilterPropPtr->ComFilterSignalValueRef]);
      /* evaluate filter */
      FilterResult =
         ((((uint32) NewVal ^ ((uint32) oldVal)) &
           ((uint32) mask)) != 0);
   }                      /* case COM_F_MASK_NEW_DIFFERS_MASK_OLD: */
   break;
#if (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON)
   case COM_F_ONE_EVERY_N:
   {
      FilterResult = Com_EB_FilterOneEveryN(ComFilterPropPtr->ComFilterRef);
   }                            /* case COM_F_ONE_EVERY_N: */
   break;
#endif /* (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON) */
      default:
         {
            CONSTP2CONST(ComFilter32BitMaskXMinMaxType, AUTOMATIC,
                         COM_APPL_CONST) FilterPtr =
               COM_GET_CONFIG_ELEMENT(ComFilter32BitMaskXMinMaxType,
                                      Com_gConfigPtr->ComFilter32BitMaskXMinMaxRef,
                                      ComFilterPropPtr->ComFilterRef);
            uint32 maskOrMin = FilterPtr->ComFilterMaskOrMin;
            uint32 XOrMax = FilterPtr->ComFilterXOrMax;

            switch (ComFilterPropPtr->ComFilterType)
            {
            case COM_F_MASKED_NEW_EQUALS_X:
            {
               FilterResult =
                  ((uint32) NewVal & (uint32) maskOrMin) ==
                  (uint32) XOrMax;
            }
            break;
            case COM_F_MASKED_NEW_DIFFERS_X:
            {
               FilterResult =
                  ((uint32) NewVal & (uint32) maskOrMin) !=
                  (uint32) XOrMax;
            }
            break;
            case COM_F_NEW_IS_WITHIN:
            {
              FilterResult = ((uint32) maskOrMin <= (uint32) NewVal)
                 && ((uint32) NewVal <= (uint32) XOrMax);
            }
            break;
            case COM_F_NEW_IS_OUTSIDE:
            {
               FilterResult = ((uint32) maskOrMin > (uint32) NewVal)
                  || ((uint32) NewVal > (uint32) XOrMax);
            }
            break;
            /* CHECK: NOPARSE */
            default:
            {
               /* Should never be reached, FilterType is unknown */
               COM_UNREACHABLE_CODE_ASSERT(COM_INTERNAL_API_ID);
            }
            break;
            /* CHECK: PARSE */
            }
         }
         break;
   }                            /* end switch FilterType */

   /* Check if the value should be stored to the signal buffer */
   if ((FilterResult == TRUE) &&
       (ComFilterPropPtr->ComFilterSignalValueRef != COM_SIGNAL_BUFFER_REF_INVALID)
      )
   {
      /* Deviation MISRAC2012-1 <+2> */
      CONSTP2VAR(uint32, AUTOMATIC, COM_VAR_CLEARED) bufferBaseUint32 =
          COM_GET_MEMPTR(uint32, Com_gConfigPtr->ComBufferBaseUInt32);
      TS_AtomicAssign32(bufferBaseUint32[ComFilterPropPtr->ComFilterSignalValueRef], NewVal);

   }             /* if ((FilterResult == TRUE) && (ComFilterSignalValueRef != COM_SIGNAL_BUFFER_REF_INVALID)) */


   return FilterResult;

}                               /* Com_EB_FilterUint32 */

TS_MOD_PRIV_DEFN FUNC(boolean, COM_CODE) Com_EB_FilterSint32
(
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr,
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
)
{
   boolean FilterResult = FALSE;
   sint32 NewVal = (sint32) * (P2CONST(sint32, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;


   switch (ComFilterPropPtr->ComFilterType)
   {
   case COM_F_MASK_NEW_DIFFERS_MASK_OLD:
   {
      /* pointer to mask */
      CONSTP2CONST(ComFilter32BitMaskType, AUTOMATIC, COM_APPL_CONST) maskPtr =
         COM_GET_CONFIG_ELEMENT(ComFilter32BitMaskType,
                                Com_gConfigPtr->ComFilter32BitMaskRef,
                                ComFilterPropPtr->ComFilterRef);
      uint32 mask = *maskPtr;
      /* pointer the old value */
      /* Deviation MISRAC2012-1 <+2> */
      CONSTP2VAR(uint32, AUTOMATIC, COM_VAR_CLEARED) bufferBaseSint32 =
          COM_GET_MEMPTR(uint32, Com_gConfigPtr->ComBufferBaseSInt32);
      sint32 oldVal;
      TS_AtomicAssign32(oldVal, (sint32) bufferBaseSint32[ComFilterPropPtr->ComFilterSignalValueRef]);
      /* evaluate filter */
      FilterResult =
         ((((uint32) NewVal ^ ((uint32) oldVal)) &
           ((uint32) mask)) != 0);
   }                      /* case COM_F_MASK_NEW_DIFFERS_MASK_OLD: */
   break;
#if (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON)
   case COM_F_ONE_EVERY_N:
   {
      FilterResult = Com_EB_FilterOneEveryN(ComFilterPropPtr->ComFilterRef);
   }                            /* case COM_F_ONE_EVERY_N: */
   break;
#endif /* (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON) */
      default:
         {
            CONSTP2CONST(ComFilter32BitMaskXMinMaxType, AUTOMATIC,
                         COM_APPL_CONST) FilterPtr =
               COM_GET_CONFIG_ELEMENT(ComFilter32BitMaskXMinMaxType,
                                      Com_gConfigPtr->ComFilter32BitMaskXMinMaxRef,
                                      ComFilterPropPtr->ComFilterRef);
            uint32 maskOrMin = FilterPtr->ComFilterMaskOrMin;
            uint32 XOrMax = FilterPtr->ComFilterXOrMax;

            switch (ComFilterPropPtr->ComFilterType)
            {
            case COM_F_MASKED_NEW_EQUALS_X:
            {
               FilterResult =
                  ((uint32) NewVal & (uint32) maskOrMin) ==
                  (uint32) XOrMax;
            }
            break;
            case COM_F_MASKED_NEW_DIFFERS_X:
            {
               FilterResult =
                  ((uint32) NewVal & (uint32) maskOrMin) !=
                  (uint32) XOrMax;
            }
            break;
            case COM_F_NEW_IS_WITHIN:
            {
              FilterResult = ((sint32) maskOrMin <= (sint32) NewVal)
                 && ((sint32) NewVal <= (sint32) XOrMax);
            }
            break;
            case COM_F_NEW_IS_OUTSIDE:
            {
               FilterResult = ((sint32) maskOrMin > (sint32) NewVal)
                  || ((sint32) NewVal > (sint32) XOrMax);
            }
            break;
            /* CHECK: NOPARSE */
            default:
            {
               /* Should never be reached, FilterType is unknown */
               COM_UNREACHABLE_CODE_ASSERT(COM_INTERNAL_API_ID);
            }
            break;
            /* CHECK: PARSE */
            }
         }
         break;
   }                            /* end switch FilterType */

   /* Check if the value should be stored to the signal buffer */
   if ((FilterResult == TRUE) &&
       (ComFilterPropPtr->ComFilterSignalValueRef != COM_SIGNAL_BUFFER_REF_INVALID)
      )
   {
      /* Deviation MISRAC2012-1 <+2> */
      CONSTP2VAR(sint32, AUTOMATIC, COM_VAR_CLEARED) bufferBaseSint32 =
          COM_GET_MEMPTR(sint32, Com_gConfigPtr->ComBufferBaseSInt32);
      TS_AtomicAssign32(bufferBaseSint32[ComFilterPropPtr->ComFilterSignalValueRef], NewVal);

   }             /* if ((FilterResult == TRUE) && (ComFilterSignalValueRef != COM_SIGNAL_BUFFER_REF_INVALID)) */


   return FilterResult;

}                               /* Com_EB_FilterSint32 */

TS_MOD_PRIV_DEFN FUNC(boolean, COM_CODE) Com_EB_FilterUint64
(
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr,
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
)
{
   uint8 FilterResult = FALSE;
   ComFilterConfType ComFilterProp_loc = *ComFilterPropPtr;
   P2CONST(ComEmu64Type, AUTOMATIC, AUTOMATIC) SignalDataPtr_loc =
       (P2CONST(ComEmu64Type, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

#if (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON)
   if(COM_F_ONE_EVERY_N == ComFilterPropPtr->ComFilterType)
   {
     FilterResult = (uint8) Com_EB_FilterOneEveryN(ComFilterPropPtr->ComFilterRef);
   }
   else
#endif /* (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON) */
   {
     FilterResult = (uint8) Com_EB_FilterUint32(ComFilterPropPtr,&SignalDataPtr_loc->LoWord);

     /* set ComFilterRef for next ComFilter32BitMaskXMinMax entry */
     ComFilterProp_loc.ComFilterRef++;

     switch (ComFilterPropPtr->ComFilterType)
     {
     case COM_F_MASKED_NEW_DIFFERS_X:
     {
       uint8 FilterResult_new_differs_x =
           (uint8) Com_EB_FilterUint32(&ComFilterProp_loc,&SignalDataPtr_loc->HiWord);
       FilterResult = FilterResult | FilterResult_new_differs_x;
     }
     break;
     case COM_F_MASKED_NEW_EQUALS_X:
     {
       uint8 FilterResult_new_equals_x =
           (uint8) Com_EB_FilterUint32(&ComFilterProp_loc,&SignalDataPtr_loc->HiWord);
       FilterResult = FilterResult & FilterResult_new_equals_x;
     }                            /* case COM_F_MASKED_NEW_EQUALS_X: */
     break;
     /* CHECK: NOPARSE */
     default:
     {
       /* Should never be reached, FilterType is unknown */
       COM_UNREACHABLE_CODE_ASSERT(COM_INTERNAL_API_ID);
     }
     break;
     /* CHECK: PARSE */
     }                            /* end switch FilterType */
   }

   return (boolean) FilterResult;

}                               /* Com_EB_FilterUint64 */

TS_MOD_PRIV_DEFN FUNC(boolean, COM_CODE) Com_EB_FilterSint64
(
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr,
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
)
{
  uint8 FilterResult = FALSE;
  ComFilterConfType ComFilterProp_loc = *ComFilterPropPtr;
  P2CONST(ComEmu64Type, AUTOMATIC, AUTOMATIC) SignalDataPtr_loc =
      (P2CONST(ComEmu64Type, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

#if (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON)
  if(COM_F_ONE_EVERY_N == ComFilterPropPtr->ComFilterType)
  {
    FilterResult = (uint8) Com_EB_FilterOneEveryN(ComFilterPropPtr->ComFilterRef);
  }
  else
#endif /* (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON) */
  {
    FilterResult = (uint8) Com_EB_FilterUint32(ComFilterPropPtr,&SignalDataPtr_loc->LoWord);

    /* set ComFilterRef for next ComFilter32BitMaskXMinMax entry */
    ComFilterProp_loc.ComFilterRef++;

    switch (ComFilterPropPtr->ComFilterType)
    {
    case COM_F_MASKED_NEW_DIFFERS_X:
    {
      uint8 FilterResult_new_differs_x =
          (uint8) Com_EB_FilterUint32(&ComFilterProp_loc,&SignalDataPtr_loc->HiWord);
      FilterResult = FilterResult | FilterResult_new_differs_x;
    }
    break;
    case COM_F_MASKED_NEW_EQUALS_X:
    {
      uint8 FilterResult_new_equals_x =
          (uint8) Com_EB_FilterUint32(&ComFilterProp_loc,&SignalDataPtr_loc->HiWord);
      FilterResult = FilterResult & FilterResult_new_equals_x;
    }                            /* case COM_F_MASKED_NEW_EQUALS_X: */
    break;
    /* CHECK: NOPARSE */
    default:
    {
      /* Should never be reached, FilterType is unknown */
      COM_UNREACHABLE_CODE_ASSERT(COM_INTERNAL_API_ID);
    }
    break;
    /* CHECK: PARSE */
    }                            /* end switch FilterType */
  }

  return (boolean) FilterResult;

}                               /* Com_EB_FilterSint64 */


#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
#endif

#endif /* (COM_FILTER_AVAILABLE == STD_ON) */

/*==================[end of file]============================================*/

