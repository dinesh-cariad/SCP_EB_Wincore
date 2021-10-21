/**
 * \file
 *
 * \brief AUTOSAR EthTSyn
 *
 * This file contains the implementation of the AUTOSAR
 * module EthTSyn.
 *
 * \version 2.2.4
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*
 *  Misra-C:2012 Deviations:
 *
 *  MISRAC2012-2) Deviated Rule: 8.9 (Advisory)
 *    An object should be defined at block scope if its identifier only appears in a single
 *    function.
 *
 *    Reason:
 *    - The AUTOSAR memory mapping requires that functions are mapped in SEC_CODE memory sections.
 *    - Objects at block scope require a different memory mapping, e.g. to a SEC_VAR section,
 *    which leads to nested memory sections, which is not supported by some compilers.
 *
 */
/*==================[inclusions]============================================*/
#include <EthTSyn_Int01_Lib.h> /* Unit internal header file. */
#include <EthTSyn.h>
#include <EthTSyn_Trace.h> /* Debug and trace */
#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
#include <Det.h> /* DET support */
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/
#define ETHTSYN_START_SEC_CONST_UNSPECIFIED
#include <EthTSyn_MemMap.h>

#if((ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) || (ETHTSYN_SWITCH_USED == STD_ON))
/** \brief Conversion table of uint32 second bit in uint32 nanosecondsLo and uint32 nanosecondsHi
 *
 *  This table contains the converted value in nanosecondsLo and nanosecondsHi:
 *  nanosecondsLo = (2^Index*10000000000)%(2^32)
 *  nanosecondsHi = (2^Index*10000000000)/(2^32)
 *
 *  Type sorted by nanosecondsLo, nanosecondsHi.
 */
 /* Deviation MISRAC2012-2 */
STATIC CONST(StbM_VirtualLocalTimeType, ETHTSYN_CONST)
  EthTSyn_Int01_EthTimeToNsConvertionTable[32U] =
{
  {1000000000U, 0U}, /* Index 0 */
  {2000000000U, 0U}, /* Index 1 */
  {4000000000U, 0U}, /* Index 2 */
  {3705032704U, 1U}, /* Index 3 */
  {3115098112U, 3U}, /* Index 4 */
  {1935228928U, 7U}, /* Index 5 */
  {3870457856U, 14U}, /* Index 6 */
  {3445948416U, 29U}, /* Index 7 */
  {2596929536U, 59U}, /* Index 8 */
  {898891776U, 119U}, /* Index 9 */
  {1797783552U, 238U}, /* Index 10 */
  {3595567104U, 476U}, /* Index 11 */
  {2896166912U, 953U}, /* Index 12 */
  {1497366528U, 1907U}, /* Index 13 */
  {2994733056U, 3814U}, /* Index 14 */
  {1694498816U, 7629U}, /* Index 15 */
  {3388997632U, 15258U}, /* Index 16 */
  {2483027968U, 30517U}, /* Index 17 */
  {671088640U, 61035U}, /* Index 18 */
  {1342177280U, 122070U}, /* Index 19 */
  {2684354560U, 244140U}, /* Index 20 */
  {1073741824U, 488281U}, /* Index 21 */
  {2147483648U, 976562U}, /* Index 22 */
  {0U, 1953125U}, /* Index 23 */
  {0U, 3906250U}, /* Index 24 */
  {0U, 7812500U}, /* Index 25 */
  {0U, 15625000U}, /* Index 26 */
  {0U, 31250000U}, /* Index 27 */
  {0U, 62500000U}, /* Index 28 */
  {0U, 125000000U}, /* Index 29 */
  {0U, 250000000U}, /* Index 30 */
  {0U, 500000000U}, /* Index 31 */
};
#endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) || (ETHTSYN_SWITCH_USED == STD_ON) */

/** \brief Virtual Local Time to time stamp Array conversion table
 *
 *  This table contains the converted value in nanoseconds, seconds, and secondsHi of:
 *  Index 0: 2^0s
 *  Index 1: 2^1s
 *  ...
 *  Index 29: 2^29s
 *
 *  Type sorted by nanoseconds, seconds, secondsHi.
 */
 /* Deviation MISRAC2012-2 */
STATIC CONST(Eth_TimeStampType, ETHTSYN_CONST) EthTSyn_Int01_VRTtoTimeStampConversionTable[30U] =
{
  {294967296U, 4U, 0U}, /* Index 0 */
  {589934592U, 8U, 0U}, /* Index 1 */
  {179869184U, 17U, 0U}, /* Index 2 */
  {359738368U, 34U, 0U}, /* Index 3 */
  {719476736U, 68U, 0U}, /* Index 4 */
  {438953472U, 137U, 0U}, /* Index 5 */
  {877906944U, 274U, 0U}, /* Index 6 */
  {755813888U, 549U, 0U}, /* Index 7 */
  {511627776U, 1099U, 0U}, /* Index 8 */
  {23255552U, 2199U, 0U}, /* Index 9 */
  {46511104U, 4398U, 0U}, /* Index 10 */
  {93022208U, 8796U, 0U}, /* Index 11 */
  {186044416U, 17592U, 0U}, /* Index 12 */
  {372088832U, 35184U, 0U}, /* Index 13 */
  {744177664U, 70368U, 0U}, /* Index 14 */
  {488355328U, 140737U, 0U}, /* Index 15 */
  {976710656U, 281474U, 0U}, /* Index 16 */
  {953421312U, 562949U, 0U}, /* Index 17 */
  {906842624U, 1125899U, 0U}, /* Index 18 */
  {813685248U, 2251799U, 0U}, /* Index 19 */
  {627370496U, 4503599U, 0U}, /* Index 20 */
  {254740992U, 9007199U, 0U}, /* Index 21 */
  {509481984U, 18014398U, 0U}, /* Index 22 */
  {18963968U, 36028797U, 0U}, /* Index 23 */
  {37927936U, 72057594U, 0U}, /* Index 24 */
  {75855872U, 144115188U, 0U}, /* Index 25 */
  {151711744U, 288230376U, 0U}, /* Index 26 */
  {303423488U, 576460752U, 0U}, /* Index 27 */
  {606846976U, 1152921504U, 0U}, /* Index 28 */
  {213693952U, 2305843009U, 0U} /* Index 29 */
};

#define ETHTSYN_STOP_SEC_CONST_UNSPECIFIED
#include <EthTSyn_MemMap.h>
/*==================[external function definitions]=========================*/
#define ETHTSYN_START_SEC_CODE
#include <EthTSyn_MemMap.h>

#if((ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) || (ETHTSYN_SWITCH_USED == STD_ON))
TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Int01_AddVirtualLocalTimes
(
  P2VAR(StbM_VirtualLocalTimeType, AUTOMATIC, ETHTSYN_APPL_DATA) VirtualLocalTime1Ptr,
  P2CONST(StbM_VirtualLocalTimeType, AUTOMATIC, ETHTSYN_APPL_DATA) VirtualLocalTime2Ptr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  DBG_ETHTSYN_INT01_ADDVIRTUALLOCALTIMES_ENTRY(VirtualLocalTime1Ptr, VirtualLocalTime2Ptr);
  /* ANSI C90: A computation involving unsigned operands can
             never overflow, because a result that cannot be represented by the
             resulting unsigned integer type is reduced modulo the number that is
             one greater than the largest value that can be represented by the
             resulting unsigned integer type. */
  VirtualLocalTime1Ptr->nanosecondsLo += VirtualLocalTime2Ptr->nanosecondsLo;
  VirtualLocalTime1Ptr->nanosecondsHi += VirtualLocalTime2Ptr->nanosecondsHi;

  if(VirtualLocalTime1Ptr->nanosecondsLo < VirtualLocalTime2Ptr->nanosecondsLo)
  {
    /* Overflow occurs in nanosecondsLo */
    VirtualLocalTime1Ptr->nanosecondsHi++;
  }

  /* In the current implementation EthTSyn_Int01_AddVirtualLocalTimes() is called only in
   * the context of EthTSyn_Int01_ConvertEthTimeToVirtualTime() where the overflow is
   * protected by EthTSyn_Int01_VRTtoTimeStampConversionTable
   * The false condition can be reached if this function is used in another context,
   * in conclusion NOPARSE is needed here.
   * */
  /* CHECK: NOPARSE */
  if(VirtualLocalTime1Ptr->nanosecondsHi >= VirtualLocalTime2Ptr->nanosecondsHi)
  {
    RetVal = E_OK;
  }
  /* CHECK: PARSE */

  DBG_ETHTSYN_INT01_ADDVIRTUALLOCALTIMES_EXIT(RetVal, VirtualLocalTime1Ptr, VirtualLocalTime2Ptr);

  return RetVal;
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Int01_ConvertEthTimeToVirtualTime
(
  P2CONST(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) EthTimePtr,
  P2VAR(StbM_VirtualLocalTimeType, AUTOMATIC, ETHTSYN_APPL_DATA) VirtualLocalTimePtr
)
{
  uint32 RemainingSeconds = EthTimePtr->seconds;
  uint8 Idx;
  Std_ReturnType RetVal = E_NOT_OK;
  boolean ContinueForLoop = TRUE;

  DBG_ETHTSYN_INT01_CONVERTETHTIMETOVIRTUALTIME_ENTRY(EthTimePtr, VirtualLocalTimePtr);

  VirtualLocalTimePtr->nanosecondsLo = EthTimePtr->nanoseconds;
  VirtualLocalTimePtr->nanosecondsHi = 0U;

  /* An overflow of nanosecondsHi shall not happen within car operation time. */
  if(EthTimePtr->secondsHi > 0U)
  {
    RetVal = E_NOT_OK;
    ContinueForLoop = FALSE;
  }

  /* Convert the remaining seconds in ns. */
  for(Idx = 0U; (Idx < 32U) && ContinueForLoop; Idx++)
  {
    if(RemainingSeconds == 0U)
    {
      /* Calculation finished*/
      ContinueForLoop = FALSE;
      RetVal = E_OK;
    }
    else
    {
      if((RemainingSeconds & 0x00000001U) == 1U)
      {
        RetVal = EthTSyn_Int01_AddVirtualLocalTimes
        (
          VirtualLocalTimePtr, &EthTSyn_Int01_EthTimeToNsConvertionTable[Idx]
        );
        /* CHECK: NOPARSE */
        /* Check rationale of adding NOPARSE in function EthTSyn_Int01_AddVirtualLocalTimes() */
        if(E_OK != RetVal)
        {
          ContinueForLoop = FALSE;
        }
        /* CHECK: PARSE */
      }
      RemainingSeconds >>= 1U;
    }
  }
  DBG_ETHTSYN_INT01_CONVERTETHTIMETOVIRTUALTIME_EXIT(RetVal, EthTimePtr, VirtualLocalTimePtr);

  return RetVal;
}
#endif /* ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON || ETHTSYN_SWITCH_USED == STD_ON*/

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Int01_SubVirtualLocalTimes
(
  P2CONST(StbM_VirtualLocalTimeType, AUTOMATIC, ETHTSYN_APPL_DATA) VirtualLocalTime_MinuendPtr,
  P2CONST(StbM_VirtualLocalTimeType, AUTOMATIC, ETHTSYN_APPL_DATA) VirtualLocalTime_SubtrahendPtr,
  P2VAR(StbM_VirtualLocalTimeType, AUTOMATIC, ETHTSYN_APPL_DATA) VirtualLocalTime_ResultPtr
)
{
  boolean Sign;
  Std_ReturnType RetVal = E_NOT_OK;

  StbM_VirtualLocalTimeType Virtual_Minuend = *VirtualLocalTime_MinuendPtr;
  StbM_VirtualLocalTimeType Virtual_Subtrahend = *VirtualLocalTime_SubtrahendPtr;

  DBG_ETHTSYN_INT01_SUBVIRTUALLOCALTIMES_ENTRY
  (
    VirtualLocalTime_MinuendPtr,
    VirtualLocalTime_SubtrahendPtr,
    VirtualLocalTime_ResultPtr
  );

  Sign = EthTSyn_Int01_VirtualLocalTime_IsGreaterOrEqual(&Virtual_Minuend, &Virtual_Subtrahend);

  if(Sign != FALSE)
  {
    boolean Borrow;

    if(Virtual_Minuend.nanosecondsLo >= Virtual_Subtrahend.nanosecondsLo)
    {
      Virtual_Minuend.nanosecondsLo -= Virtual_Subtrahend.nanosecondsLo;
      Borrow = FALSE;
    }
    else
    {
      uint32 DiffNanoSeconds = Virtual_Subtrahend.nanosecondsLo - Virtual_Minuend.nanosecondsLo;
      Virtual_Minuend.nanosecondsLo = (ETHTSYN_32BIT_MAX - DiffNanoSeconds);
      Virtual_Minuend.nanosecondsLo += 1U;
      Borrow = TRUE;
    }

    if(Borrow)
    {
      /* CHECK: NOPARSE */
      if(Virtual_Minuend.nanosecondsHi > 0U)
      {
        Virtual_Minuend.nanosecondsHi--;
      }
      else
      {
#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
        ETHTSYN_UNREACHABLE_CODE_ASSERT(ETHTSYN_INTERNAL_SVCID);
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */
      }
      /* CHECK: PARSE */
    }

    /* Virtual_Minuend.nanosecondsHi is always >= Virtual_Subtrahend.nanosecondsHi */
    Virtual_Minuend.nanosecondsHi -= Virtual_Subtrahend.nanosecondsHi;

    /* Write result. */
    VirtualLocalTime_ResultPtr->nanosecondsHi = Virtual_Minuend.nanosecondsHi;
    VirtualLocalTime_ResultPtr->nanosecondsLo = Virtual_Minuend.nanosecondsLo;

    RetVal = E_OK;
  }

  DBG_ETHTSYN_INT01_SUBVIRTUALLOCALTIMES_EXIT
  (
    RetVal,
    VirtualLocalTime_MinuendPtr,
    VirtualLocalTime_SubtrahendPtr,
    VirtualLocalTime_ResultPtr
  );
  return RetVal;
}

TS_MOD_PRIV_DEFN FUNC(boolean, ETHTSYN_CODE) EthTSyn_Int01_VirtualLocalTime_IsGreaterOrEqual
(
  P2CONST(StbM_VirtualLocalTimeType, AUTOMATIC, ETHTSYN_APPL_DATA) VirtualLocalTime1Ptr,
  P2CONST(StbM_VirtualLocalTimeType, AUTOMATIC, ETHTSYN_APPL_DATA) VirtualLocalTime2Ptr
)
{
  boolean Sign;

  DBG_ETHTSYN_INT01_VIRTUALLOCALTIME_ISGREATEROREQUAL_ENTRY
    (VirtualLocalTime1Ptr, VirtualLocalTime2Ptr);

  if(VirtualLocalTime1Ptr->nanosecondsHi > VirtualLocalTime2Ptr->nanosecondsHi)
  {
    Sign = TRUE;
  }
  else if(VirtualLocalTime1Ptr->nanosecondsHi < VirtualLocalTime2Ptr->nanosecondsHi)
  {
    Sign = FALSE;
  }
  /* VirtualLocalTime1Ptr->nanosecondsHi == VirtualLocalTime2Ptr->nanosecondsHi */
  else if(VirtualLocalTime1Ptr->nanosecondsLo >= VirtualLocalTime2Ptr->nanosecondsLo)
  {
    Sign = TRUE; /* VirtualLocalTime1Ptr >= VirtualLocalTime2Ptr */
  }
  else
  {
    Sign = FALSE; /* VirtualLocalTime1Ptr < VirtualLocalTime2Ptr */
  }

  DBG_ETHTSYN_INT01_VIRTUALLOCALTIME_ISGREATEROREQUAL_EXIT
    (Sign, VirtualLocalTime1Ptr, VirtualLocalTime2Ptr);

  return Sign;
}

TS_MOD_PRIV_DEFN FUNC(void, ETHTSYN_CODE) EthTSyn_Int01_ConvertVirtualTimeToEthTime
(
  P2CONST(StbM_VirtualLocalTimeType, AUTOMATIC, ETHTSYN_APPL_DATA) VirtualLocalTimePtr,
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) EthTimePtr
)
{
  uint32 RemainingNsHi = VirtualLocalTimePtr->nanosecondsHi;
  uint8 Idx;

  DBG_ETHTSYN_INT01_CONVERTVIRTUALTIMETOETHTIME_ENTRY(VirtualLocalTimePtr, EthTimePtr);

  EthTimePtr->nanoseconds = VirtualLocalTimePtr->nanosecondsLo;
  EthTimePtr->seconds = 0U;
  EthTimePtr->secondsHi = 0U;

  /* Convert the remaining nsHi in sec. */
  for(Idx = 0U; Idx < 30U; Idx++)
  {
    if(RemainingNsHi == 0U)
    {
      /* Calculation finished*/
      break;
    }
    if((RemainingNsHi & 0x00000001U) == 1U)
    {
      EthTSyn_AddTimeStamps(EthTimePtr,&EthTSyn_Int01_VRTtoTimeStampConversionTable[Idx]);
    }
    RemainingNsHi >>= 1U;
  }
  DBG_ETHTSYN_INT01_CONVERTVIRTUALTIMETOETHTIME_EXIT(VirtualLocalTimePtr, EthTimePtr);
}
#define ETHTSYN_STOP_SEC_CODE
#include <EthTSyn_MemMap.h>
/*==================[end of file]===========================================*/
