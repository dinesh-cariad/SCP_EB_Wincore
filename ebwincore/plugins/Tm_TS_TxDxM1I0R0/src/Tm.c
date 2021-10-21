/**
 * \file
 *
 * \brief AUTOSAR Tm
 *
 * This file contains the implementation of the AUTOSAR
 * module Tm.
 *
 * \version 1.0.4
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==[Includes]================================================================*/

#include <Tm_Trace.h>

#include <TSAutosar.h>          /* EB specific standard types */

#include <Tm.h>                 /* Module public API */

/* !LINKSTO Tm.swdd.Gpt_PredefTimerType,1, Tm.swdd.Gpt_GetPredefTimerValue,1 */
#include <Gpt.h>

/* !LINKSTO Tm.swdd.Det_ReportRuntimeError,1 */
#include <Det.h>

/*==[Macros]==================================================================*/

/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef TM_VENDOR_ID /* configuration check */
#error TM_VENDOR_ID must be defined
#endif

#if (TM_VENDOR_ID != 1U) /* vendor check */
#error TM_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef TM_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error TM_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (TM_AR_RELEASE_MAJOR_VERSION != 4U)
#error TM_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef TM_AR_RELEASE_MINOR_VERSION /* configuration check */
#error TM_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (TM_AR_RELEASE_MINOR_VERSION != 4U )
#error TM_AR_RELEASE_MINOR_VERSION wrong (!= 4U)
#endif

#ifndef TM_AR_RELEASE_REVISION_VERSION /* configuration check */
#error TM_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (TM_AR_RELEASE_REVISION_VERSION != 0U )
#error TM_AR_RELEASE_REVISION_VERSION wrong (!= 0U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef TM_SW_MAJOR_VERSION /* configuration check */
#error TM_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (TM_SW_MAJOR_VERSION != 1U)
#error TM_SW_MAJOR_VERSION wrong (!= 1U)
#endif

#ifndef TM_SW_MINOR_VERSION /* configuration check */
#error TM_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (TM_SW_MINOR_VERSION < 0U)
#error TM_SW_MINOR_VERSION wrong (< 0U)
#endif

#ifndef TM_SW_PATCH_VERSION /* configuration check */
#error TM_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (TM_SW_PATCH_VERSION < 4U)
#error TM_SW_PATCH_VERSION wrong (< 4U)
#endif

/** \brief Max value of 16bit Timestamp */
#define TM_MAX_TIMESTAMP_16_BIT   0x0000FFFFUL
/** \brief Max value of 24bit Timestamp */
#define TM_MAX_TIMESTAMP_24_BIT   0x00FFFFFFUL

#if (TM_DEV_ERROR_DETECT == STD_ON)
/* !LINKSTO Tm.swdd.Det_ReportError,1 */
#if (defined TM_DET_REPORT_ERROR)
#error TM_DET_REPORT_ERROR already defined
#endif /* #if (defined TM_DET_REPORT_ERROR) */
/** \brief Macro for reporting an error to Det
 **
 ** \param[in] ApiId Service ID of the API function
 ** \param[in] ErrorCode Error code reported to Det module
 **/
#define TM_DET_REPORT_ERROR(ApiId, ErrorCode) \
  (void)Det_ReportError(TM_MODULE_ID, TM_INSTANCE_ID, (ApiId), (ErrorCode))
#endif /* #if (TM_DEV_ERROR_DETECT == STD_ON) */

#if (defined TM_DET_REPORT_RUNTIME_ERROR)
#error TM_DET_REPORT_RUNTIME_ERROR already defined
#endif /* #if (defined TM_DET_REPORT_RUNTIME_ERROR) */
/** \brief Macro for reporting an runtime errors to Det
 **
 ** \param[in] ApiId Service ID of the API function
 ** \param[in] ErrorCode Error code reported to Det module
 **/
#define TM_DET_REPORT_RUNTIME_ERROR(ApiId, ErrorCode) \
  (void)Det_ReportRuntimeError(TM_MODULE_ID, TM_INSTANCE_ID, (ApiId), (ErrorCode))


/*==[Types]===================================================================*/

/*==[Declaration of functions with internal linkage]==========================*/

/*==[Constants with internal linkage]=========================================*/

/*==[Variables with internal linkage]=========================================*/

/*==[Constants with external linkage]=========================================*/

/*==[Variables with external linkage]=========================================*/

/*==[Definition of functions with external linkage]===========================*/

/* !LINKSTO Tm.swdd.CodeSection,1 */
#define TM_START_SEC_CODE
#include <Tm_MemMap.h>

#if (TM_VERSION_INFO_API == STD_ON)
/* !LINKSTO Tm.swdd.Interface.GetVersionInfo,1, Tm.swdd.PointerClass,1 */
FUNC(void, TM_CODE) Tm_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, TM_APPL_DATA)             VersionInfoPtr
)
{
  DBG_TM_GETVERSIONINFO_ENTRY(VersionInfoPtr);
  /* !LINKSTO SWS_Tm_00063_0,1 */
#if (TM_DEV_ERROR_DETECT == STD_ON)
  if (NULL_PTR == VersionInfoPtr)
  {
    /* !LINKSTO SWS_Tm_00037,1, Tm.swdd.Err_TM_E_PARAM_POINTER,1 */
    TM_DET_REPORT_ERROR( TM_SID_GET_VERSION_INFO, TM_E_PARAM_POINTER );
  }
  else
#endif /* #if (TM_DEV_ERROR_DETECT == STD_ON) */
  {
    VersionInfoPtr->vendorID         = TM_VENDOR_ID;
    VersionInfoPtr->moduleID         = TM_MODULE_ID;
    VersionInfoPtr->sw_major_version = TM_SW_MAJOR_VERSION;
    VersionInfoPtr->sw_minor_version = TM_SW_MINOR_VERSION;
    VersionInfoPtr->sw_patch_version = TM_SW_PATCH_VERSION;
  }

  DBG_TM_GETVERSIONINFO_EXIT(VersionInfoPtr);
}

#endif /* #if (TM_VERSION_INFO_API == STD_ON) */

#if (TM_ENABLE_PREDEF_TIMER_1US_16BIT == STD_ON)
/* !LINKSTO Tm.swdd.1us16Bit.Interface.ResetTimer,1, Tm.swdd.PointerClass,1 */
FUNC(Std_ReturnType, TM_CODE) Tm_ResetTimer1us16bit
(
  P2VAR(Tm_PredefTimer1us16bitType, AUTOMATIC, TM_APPL_DATA)      TimerPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint32 CurrentTimeStamp = 0U;

  DBG_TM_RESETTIMER1US16BIT_ENTRY(TimerPtr);
  /* !LINKSTO SWS_Tm_00063_0,1 */
#if (TM_DEV_ERROR_DETECT == STD_ON)
  if (NULL_PTR == TimerPtr)
  {
    /* !LINKSTO SWS_Tm_00008,1, Tm.swdd.Err_TM_E_PARAM_POINTER,1 */
    TM_DET_REPORT_ERROR( TM_SID_RESET_TIMER_1US_16BIT, TM_E_PARAM_POINTER);
  }
  else
#endif /* #if (TM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* !LINKSTO SWS_Tm_00001,1, SWS_Tm_00002,1 */
    RetVal = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_16BIT, &CurrentTimeStamp);

    if (E_OK == RetVal)
    {
      /* !LINKSTO SWS_Tm_00006,1, Tm.swdd.ResetFunctionReEntry,1 */
      TS_AtomicAssign16(TimerPtr->TimeValue, (uint16) CurrentTimeStamp);
    }
    else
    {
      /* !LINKSTO SWS_Tm_00064_0,1, Tm.swdd.ReportRuntimeError,1 */
      TM_DET_REPORT_RUNTIME_ERROR( TM_SID_RESET_TIMER_1US_16BIT, TM_E_HARDWARE_TIMER);
    }
  }

  DBG_TM_RESETTIMER1US16BIT_EXIT(RetVal, TimerPtr);

  return RetVal;
}

/* !LINKSTO Tm.swdd.1us16Bit.Interface.GetTimeSpan,1, Tm.swdd.PointerClass,1 */
FUNC(Std_ReturnType, TM_CODE) Tm_GetTimeSpan1us16bit
(
  P2CONST(Tm_PredefTimer1us16bitType, AUTOMATIC, TM_APPL_DATA)    TimerPtr,
  P2VAR(uint16, AUTOMATIC, TM_APPL_DATA)                          TimeSpanPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint32 CurrentTimeStamp = 0U;

  DBG_TM_GETTIMESPAN1US16BIT_ENTRY(TimerPtr, TimeSpanPtr);
#if (TM_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO SWS_Tm_00012_0,1, SWS_Tm_00012_1,1 */
  if ((NULL_PTR == TimerPtr) || (NULL_PTR == TimeSpanPtr))
  {
    /* !LINKSTO Tm.swdd.Err_TM_E_PARAM_POINTER,1 */
    TM_DET_REPORT_ERROR( TM_SID_GET_TIMESPAN_1US_16BIT, TM_E_PARAM_POINTER);
  }
  else
#endif /* #if (TM_DEV_ERROR_DETECT == STD_ON) */
  {
    if (TimeSpanPtr != NULL_PTR)
    {
      /* !LINKSTO SWS_Tm_00001,1, SWS_Tm_00002,1 */
      RetVal = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_16BIT, &CurrentTimeStamp);

      if (E_OK == RetVal)
      {
        /* !LINKSTO SWS_Tm_00009,1, SWS_Tm_00010,1, Tm.swdd.GetTmeSpanFunctionReEntry,1 */
        TS_AtomicAssign16(*TimeSpanPtr, (uint16) ((CurrentTimeStamp - TimerPtr->TimeValue) & TM_MAX_TIMESTAMP_16_BIT));
      }
      else
      {
        /* !LINKSTO SWS_Tm_00065,1 */
        TS_AtomicAssign16(*TimeSpanPtr, 0U);

        /* !LINKSTO SWS_Tm_00064_0,1, Tm.swdd.ReportRuntimeError,1 */
        TM_DET_REPORT_RUNTIME_ERROR( TM_SID_GET_TIMESPAN_1US_16BIT, TM_E_HARDWARE_TIMER);
      }
    }
  }

  DBG_TM_GETTIMESPAN1US16BIT_EXIT(RetVal, TimerPtr, TimeSpanPtr);

  return RetVal;
}

/* !LINKSTO Tm.swdd.1us16Bit.Interface.ShiftTimer,1, Tm.swdd.PointerClass,1 */
FUNC(void, TM_CODE) Tm_ShiftTimer1us16bit
(
  P2VAR(Tm_PredefTimer1us16bitType, AUTOMATIC, TM_APPL_DATA)      TimerPtr,
  uint16 TimeValue
)
{
  DBG_TM_SHIFTTIMER1US16BIT_ENTRY(TimerPtr, TimeValue);
  /* !LINKSTO SWS_Tm_00063_0,1 */
#if (TM_DEV_ERROR_DETECT == STD_ON)
  if (NULL_PTR == TimerPtr)
  {
    /* !LINKSTO SWS_Tm_00018,1, Tm.swdd.Err_TM_E_PARAM_POINTER,1 */
    TM_DET_REPORT_ERROR( TM_SID_SHIFT_TIMER_1US_16BIT, TM_E_PARAM_POINTER);
  }
  else
#endif /* #if (TM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* !LINKSTO SWS_Tm_00013,1, SWS_Tm_00014,1, Tm.swdd.ShiftTimerFunctionReEntry,1 */
    TS_AtomicAssign16(TimerPtr->TimeValue, (uint16) (TimerPtr->TimeValue + TimeValue));  /* overflow is considered as a time reset to 0 */
  }

  DBG_TM_SHIFTTIMER1US16BIT_EXIT(TimerPtr, TimeValue);
}

/* !LINKSTO Tm.swdd.1us16Bit.Interface.SyncTimer,1, Tm.swdd.PointerClass,1 */
FUNC(void, TM_CODE) Tm_SyncTimer1us16bit
(
  P2VAR(Tm_PredefTimer1us16bitType, AUTOMATIC, TM_APPL_DATA)      TimerDstPtr,
  P2CONST(Tm_PredefTimer1us16bitType, AUTOMATIC, TM_APPL_DATA)    TimerSrcPtr
)
{
  DBG_TM_SYNCTIMER1US16BIT_ENTRY(TimerDstPtr, TimerSrcPtr);
  /* !LINKSTO SWS_Tm_00063_0,1 */
#if (TM_DEV_ERROR_DETECT == STD_ON)
  /* check parameters for execution of the function */
  /* if any eeror is detected return imeediately without any effect */
  if ((NULL_PTR == TimerDstPtr) || (NULL_PTR == TimerSrcPtr))
  {
    /* !LINKSTO SWS_Tm_00021,1, Tm.swdd.Err_TM_E_PARAM_POINTER,1 */
    TM_DET_REPORT_ERROR( TM_SID_SYNC_TIMER_1US_16BIT, TM_E_PARAM_POINTER);
  }
  else
#endif /* #if (TM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* !LINKSTO SWS_Tm_00019,1, Tm.swdd.SyncTimerFunctionReEntry,1 */
    TS_AtomicAssign16(TimerDstPtr->TimeValue, TimerSrcPtr->TimeValue);
  }

  DBG_TM_SYNCTIMER1US16BIT_EXIT(TimerDstPtr, TimerSrcPtr);
}

/* !LINKSTO Tm.swdd.1us16Bit.Interface.BusyWait,1, Tm.swdd.BusyWaitReEntry,1 */
/* !LINKSTO Tm.swdd.BusyWaitInterrupts,1 */
FUNC(Std_ReturnType, TM_CODE) Tm_BusyWait1us16bit
(
  uint8 WaitingTimeMin
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint32 StartTimeStamp = 0U;
  uint32 CurrentTimeStamp = 0U;

  DBG_TM_BUSYWAIT1US16BIT_ENTRY(WaitingTimeMin);

  /* !LINKSTO SWS_Tm_00001,1, SWS_Tm_00002,1 */
  RetVal = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_16BIT, &StartTimeStamp);

  CurrentTimeStamp = StartTimeStamp;

  /* !LINKSTO SWS_Tm_00022,1, SWS_Tm_00024,1, SWS_Tm_00066_0,1 */
  while ( (E_OK == RetVal) &&
          ( ((CurrentTimeStamp - StartTimeStamp) & TM_MAX_TIMESTAMP_16_BIT) <= (uint32) WaitingTimeMin)
        )
  {
    /* !LINKSTO SWS_Tm_00001,1, SWS_Tm_00002,1 */
    RetVal = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_16BIT, &CurrentTimeStamp);
  }

  if (E_OK != RetVal)
  {
    /* !LINKSTO SWS_Tm_00064_0,1, Tm.swdd.ReportRuntimeError,1 */
    TM_DET_REPORT_RUNTIME_ERROR( TM_SID_BUSY_WAIT_1US_16BIT, TM_E_HARDWARE_TIMER);
  }
  else
  {}

  DBG_TM_BUSYWAIT1US16BIT_EXIT(RetVal, WaitingTimeMin);

  return RetVal;
}

#endif /* #if (TM_ENABLE_PREDEF_TIMER_1US_16BIT == STD_ON) */

#if (TM_ENABLE_PREDEF_TIMER_1US_24BIT == STD_ON)
/* !LINKSTO Tm.swdd.1us24Bit.Interface.ResetTimer,1, Tm.swdd.PointerClass,1 */
FUNC(Std_ReturnType, TM_CODE) Tm_ResetTimer1us24bit
(
  P2VAR(Tm_PredefTimer1us24bitType, AUTOMATIC, TM_APPL_DATA)      TimerPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint32 CurrentTimeStamp = 0U;

  DBG_TM_RESETTIMER1US24BIT_ENTRY(TimerPtr);
  /* !LINKSTO SWS_Tm_00063_0,1 */
#if (TM_DEV_ERROR_DETECT == STD_ON)
  if (NULL_PTR == TimerPtr)
  {
    /* !LINKSTO SWS_Tm_00008,1, Tm.swdd.Err_TM_E_PARAM_POINTER,1 */
    TM_DET_REPORT_ERROR( TM_SID_RESET_TIMER_1US_24BIT, TM_E_PARAM_POINTER);
  }
  else
#endif /* #if (TM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* !LINKSTO SWS_Tm_00001,1, SWS_Tm_00003,1 */
    RetVal = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_24BIT, &CurrentTimeStamp);

    if (E_OK == RetVal)
    {
      /* !LINKSTO SWS_Tm_00006,1, Tm.swdd.ResetFunctionReEntry,1 */
      TS_AtomicAssign32(TimerPtr->TimeValue, CurrentTimeStamp);
    }
    else
    {
      /* !LINKSTO SWS_Tm_00064_1,1, Tm.swdd.ReportRuntimeError,1 */
      TM_DET_REPORT_RUNTIME_ERROR( TM_SID_RESET_TIMER_1US_24BIT, TM_E_HARDWARE_TIMER);
    }
  }

  DBG_TM_RESETTIMER1US24BIT_EXIT(RetVal, TimerPtr);

  return RetVal;
}

/* !LINKSTO Tm.swdd.1us24Bit.Interface.GetTimeSpan,1, Tm.swdd.PointerClass,1 */
FUNC(Std_ReturnType, TM_CODE) Tm_GetTimeSpan1us24bit
(
  P2CONST(Tm_PredefTimer1us24bitType, AUTOMATIC, TM_APPL_DATA)    TimerPtr,
  P2VAR(uint32, AUTOMATIC, TM_APPL_DATA)                          TimeSpanPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint32 CurrentTimeStamp = 0U;

  DBG_TM_GETTIMESPAN1US24BIT_ENTRY(TimerPtr, TimeSpanPtr);
#if (TM_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO SWS_Tm_00012_0,1, SWS_Tm_00012_1,1 */
  if ((NULL_PTR == TimerPtr) || (NULL_PTR == TimeSpanPtr))
  {
    /* !LINKSTO Tm.swdd.Err_TM_E_PARAM_POINTER,1 */
    TM_DET_REPORT_ERROR( TM_SID_GET_TIMESPAN_1US_24BIT, TM_E_PARAM_POINTER);
  }
  else
#endif /* #if (TM_DEV_ERROR_DETECT == STD_ON) */
  {
    if (TimeSpanPtr != NULL_PTR)
    {
      /* !LINKSTO SWS_Tm_00001,1, SWS_Tm_00003,1 */
      RetVal = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_24BIT, &CurrentTimeStamp);

      if (E_OK == RetVal)
      {
        /* !LINKSTO SWS_Tm_00009,1, SWS_Tm_00010,1, Tm.swdd.GetTmeSpanFunctionReEntry,1 */
        TS_AtomicAssign32(*TimeSpanPtr, (uint32) ((CurrentTimeStamp - TimerPtr->TimeValue) & TM_MAX_TIMESTAMP_24_BIT));
      }
      else
      {
        /* !LINKSTO SWS_Tm_00065,1 */
        TS_AtomicAssign32(*TimeSpanPtr, 0U);

        /* !LINKSTO SWS_Tm_00064_1,1, Tm.swdd.ReportRuntimeError,1 */
        TM_DET_REPORT_RUNTIME_ERROR( TM_SID_GET_TIMESPAN_1US_24BIT, TM_E_HARDWARE_TIMER);
      }
    }
  }

  DBG_TM_GETTIMESPAN1US24BIT_EXIT(RetVal, TimerPtr, TimeSpanPtr);

  return RetVal;
}

/* !LINKSTO Tm.swdd.1us24Bit.Interface.ShiftTimer,1, Tm.swdd.PointerClass,1 */
FUNC(void, TM_CODE) Tm_ShiftTimer1us24bit
(
  P2VAR(Tm_PredefTimer1us24bitType, AUTOMATIC, TM_APPL_DATA)      TimerPtr,
  uint32 TimeValue
)
{
  DBG_TM_SHIFTTIMER1US24BIT_ENTRY(TimerPtr, TimeValue);
  /* !LINKSTO SWS_Tm_00063_0,1 */
#if (TM_DEV_ERROR_DETECT == STD_ON)
  if (NULL_PTR == TimerPtr)
  {
    /* !LINKSTO SWS_Tm_00018,1, Tm.swdd.Err_TM_E_PARAM_POINTER,1 */
    TM_DET_REPORT_ERROR( TM_SID_SHIFT_TIMER_1US_24BIT, TM_E_PARAM_POINTER);
  }
  else if (TimeValue > TM_MAX_TIMESTAMP_24_BIT)
  {
    /* !LINKSTO SWS_Tm_00016,1, Tm.swdd.Err_TM_E_PARAM_VALUE,1 */
    TM_DET_REPORT_ERROR( TM_SID_SHIFT_TIMER_1US_24BIT, TM_E_PARAM_VALUE);
  }
  else
#endif /* #if (TM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* !LINKSTO SWS_Tm_00013,1, SWS_Tm_00014,1, SWS_Tm_00015,1, Tm.swdd.ShiftTimerFunctionReEntry,1 */
    TS_AtomicAssign32( TimerPtr->TimeValue, (uint32) ((TimerPtr->TimeValue + TimeValue) & TM_MAX_TIMESTAMP_24_BIT));
  }

  DBG_TM_SHIFTTIMER1US24BIT_EXIT(TimerPtr, TimeValue);
}

/* !LINKSTO Tm.swdd.1us24Bit.Interface.SyncTimer,1, Tm.swdd.PointerClass,1 */
FUNC(void, TM_CODE) Tm_SyncTimer1us24bit
(
  P2VAR(Tm_PredefTimer1us24bitType, AUTOMATIC, TM_APPL_DATA)      TimerDstPtr,
  P2CONST(Tm_PredefTimer1us24bitType, AUTOMATIC, TM_APPL_DATA)    TimerSrcPtr
)
{
  DBG_TM_SYNCTIMER1US24BIT_ENTRY(TimerDstPtr, TimerSrcPtr);
  /* !LINKSTO SWS_Tm_00063_0,1 */
#if (TM_DEV_ERROR_DETECT == STD_ON)
  if ((NULL_PTR == TimerDstPtr) || (NULL_PTR == TimerSrcPtr))
  {
    /* !LINKSTO SWS_Tm_00021,1, Tm.swdd.Err_TM_E_PARAM_POINTER,1 */
    TM_DET_REPORT_ERROR( TM_SID_SYNC_TIMER_1US_24BIT, TM_E_PARAM_POINTER);
  }
  else
#endif /* #if (TM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* !LINKSTO SWS_Tm_00019,1, Tm.swdd.SyncTimerFunctionReEntry,1 */
    TS_AtomicAssign32( TimerDstPtr->TimeValue, TimerSrcPtr->TimeValue);
  }

  DBG_TM_SYNCTIMER1US24BIT_EXIT(TimerDstPtr, TimerSrcPtr);
}

/* !LINKSTO Tm.swdd.1us24Bit.Interface.BusyWait,1, Tm.swdd.BusyWaitReEntry,1 */
/* !LINKSTO Tm.swdd.BusyWaitInterrupts,1 */
FUNC(Std_ReturnType, TM_CODE) Tm_BusyWait1us24bit
(
  uint8 WaitingTimeMin
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint32 StartTimeStamp = 0U;
  uint32 CurrentTimeStamp = 0U;

  DBG_TM_BUSYWAIT1US24BIT_ENTRY(WaitingTimeMin);

  /* !LINKSTO SWS_Tm_00001,1, SWS_Tm_00003,1 */
  RetVal = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_24BIT, &StartTimeStamp);

  CurrentTimeStamp = StartTimeStamp;

  /* !LINKSTO SWS_Tm_00022,1, SWS_Tm_00024,1, SWS_Tm_00066_1,1 */
  while ( (E_OK == RetVal) &&
          ( ((CurrentTimeStamp - StartTimeStamp) & TM_MAX_TIMESTAMP_24_BIT) <= (uint32) WaitingTimeMin)
        )
  {
    /* !LINKSTO SWS_Tm_00001,1, SWS_Tm_00003,1 */
    RetVal = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_24BIT, &CurrentTimeStamp);
  }

  if (E_OK != RetVal)
  {
    /* !LINKSTO SWS_Tm_00064_1,1, Tm.swdd.ReportRuntimeError,1 */
    TM_DET_REPORT_RUNTIME_ERROR( TM_SID_BUSY_WAIT_1US_24BIT, TM_E_HARDWARE_TIMER);
  }
  else
  {}

  DBG_TM_BUSYWAIT1US24BIT_EXIT(RetVal, WaitingTimeMin);

  return RetVal;
}

#endif /* #if (TM_ENABLE_PREDEF_TIMER_1US_24BIT == STD_ON) */

#if (TM_ENABLE_PREDEF_TIMER_1US_32BIT == STD_ON)
/* !LINKSTO Tm.swdd.1us32Bit.Interface.ResetTimer,1, Tm.swdd.PointerClass,1 */
FUNC(Std_ReturnType, TM_CODE) Tm_ResetTimer1us32bit
(
  P2VAR(Tm_PredefTimer1us32bitType, AUTOMATIC, TM_APPL_DATA)      TimerPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint32 CurrentTimeStamp = 0U;

  DBG_TM_RESETTIMER1US32BIT_ENTRY(TimerPtr);
  /* !LINKSTO SWS_Tm_00063_0,1 */
#if (TM_DEV_ERROR_DETECT == STD_ON)
  if (NULL_PTR == TimerPtr)
  {
    /* !LINKSTO SWS_Tm_00008,1, Tm.swdd.Err_TM_E_PARAM_POINTER,1 */
    TM_DET_REPORT_ERROR( TM_SID_RESET_TIMER_1US_32BIT, TM_E_PARAM_POINTER);
  }
  else
#endif /* #if (TM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* !LINKSTO SWS_Tm_00001,1, SWS_Tm_00004,1 */
    RetVal = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_32BIT, &CurrentTimeStamp);

    if (E_OK == RetVal)
    {
      /* !LINKSTO SWS_Tm_00006,1, Tm.swdd.ResetFunctionReEntry,1 */
      TS_AtomicAssign32( TimerPtr->TimeValue, CurrentTimeStamp);
    }
    else
    {
      /* !LINKSTO SWS_Tm_00064_2,1, Tm.swdd.ReportRuntimeError,1 */
      TM_DET_REPORT_RUNTIME_ERROR( TM_SID_RESET_TIMER_1US_32BIT, TM_E_HARDWARE_TIMER);
    }
  }

  DBG_TM_RESETTIMER1US32BIT_EXIT(RetVal, TimerPtr);

  return RetVal;
}

/* !LINKSTO Tm.swdd.1us32Bit.Interface.GetTimeSpan,1, Tm.swdd.PointerClass,1 */
FUNC(Std_ReturnType, TM_CODE) Tm_GetTimeSpan1us32bit
(
  P2CONST(Tm_PredefTimer1us32bitType, AUTOMATIC, TM_APPL_DATA)    TimerPtr,
  P2VAR(uint32, AUTOMATIC, TM_APPL_DATA)                          TimeSpanPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint32 CurrentTimeStamp = 0U;

  DBG_TM_GETTIMESPAN1US32BIT_ENTRY(TimerPtr, TimeSpanPtr);
#if (TM_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO SWS_Tm_00012_0,1, SWS_Tm_00012_1,1 */
  if ((NULL_PTR == TimerPtr) || (NULL_PTR == TimeSpanPtr))
  {
    /* !LINKSTO Tm.swdd.Err_TM_E_PARAM_POINTER,1 */
    TM_DET_REPORT_ERROR( TM_SID_GET_TIMESPAN_1US_32BIT, TM_E_PARAM_POINTER);
  }
  else
#endif /* #if (TM_DEV_ERROR_DETECT == STD_ON) */
  {
    if (TimeSpanPtr != NULL_PTR)
    {
      /* !LINKSTO SWS_Tm_00001,1, SWS_Tm_00004,1 */
      RetVal = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_32BIT, &CurrentTimeStamp);

      if (E_OK == RetVal)
      {
        /* !LINKSTO SWS_Tm_00009,1, SWS_Tm_00010,1, Tm.swdd.GetTmeSpanFunctionReEntry,1 */
        TS_AtomicAssign32( *TimeSpanPtr, (uint32) (CurrentTimeStamp - TimerPtr->TimeValue));
      }
      else
      {
        /* !LINKSTO SWS_Tm_00065,1 */
        TS_AtomicAssign32(*TimeSpanPtr, 0U);

        /* !LINKSTO SWS_Tm_00064_2,1, Tm.swdd.ReportRuntimeError,1 */
        TM_DET_REPORT_RUNTIME_ERROR( TM_SID_GET_TIMESPAN_1US_32BIT, TM_E_HARDWARE_TIMER);
      }
    }
  }

  DBG_TM_GETTIMESPAN1US32BIT_EXIT(RetVal, TimerPtr, TimeSpanPtr);

  return RetVal;
}

/* !LINKSTO Tm.swdd.1us32Bit.Interface.ShiftTimer,1, Tm.swdd.PointerClass,1 */
FUNC(void, TM_CODE) Tm_ShiftTimer1us32bit
(
  P2VAR(Tm_PredefTimer1us32bitType, AUTOMATIC, TM_APPL_DATA)      TimerPtr,
  uint32 TimeValue
)
{
  DBG_TM_SHIFTTIMER1US32BIT_ENTRY(TimerPtr, TimeValue);
  /* !LINKSTO SWS_Tm_00063_0,1 */
#if (TM_DEV_ERROR_DETECT == STD_ON)
  if (NULL_PTR == TimerPtr)
  {
    /* !LINKSTO SWS_Tm_00018,1, Tm.swdd.Err_TM_E_PARAM_POINTER,1 */
    TM_DET_REPORT_ERROR( TM_SID_SHIFT_TIMER_1US_32BIT, TM_E_PARAM_POINTER);
  }
  else
#endif /* #if (TM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* !LINKSTO SWS_Tm_00013,1, SWS_Tm_00014,1, Tm.swdd.ShiftTimerFunctionReEntry,1 */
    TS_AtomicAssign32( TimerPtr->TimeValue, (uint32) (TimerPtr->TimeValue + TimeValue));  /* overflow is considered as a time reset to 0 */
  }

  DBG_TM_SHIFTTIMER1US32BIT_EXIT(TimerPtr, TimeValue);
}

/* !LINKSTO Tm.swdd.1us32Bit.Interface.SyncTimer,1, Tm.swdd.PointerClass,1 */
FUNC(void, TM_CODE) Tm_SyncTimer1us32bit
(
  P2VAR(Tm_PredefTimer1us32bitType, AUTOMATIC, TM_APPL_DATA)      TimerDstPtr,
  P2CONST(Tm_PredefTimer1us32bitType, AUTOMATIC, TM_APPL_DATA)    TimerSrcPtr
)
{
  DBG_TM_SYNCTIMER1US32BIT_ENTRY(TimerDstPtr, TimerSrcPtr);
  /* !LINKSTO SWS_Tm_00063_0,1 */
#if (TM_DEV_ERROR_DETECT == STD_ON)
  if ((NULL_PTR == TimerDstPtr) || (NULL_PTR == TimerSrcPtr))
  {
    /* !LINKSTO SWS_Tm_00021,1, Tm.swdd.Err_TM_E_PARAM_POINTER,1 */
    TM_DET_REPORT_ERROR( TM_SID_SYNC_TIMER_1US_32BIT, TM_E_PARAM_POINTER);
  }
  else
#endif /* #if (TM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* !LINKSTO SWS_Tm_00019,1, Tm.swdd.SyncTimerFunctionReEntry,1 */
    TS_AtomicAssign32( TimerDstPtr->TimeValue, TimerSrcPtr->TimeValue);
  }

  DBG_TM_SYNCTIMER1US32BIT_EXIT(TimerDstPtr, TimerSrcPtr);
}

/* !LINKSTO Tm.swdd.1us32Bit.Interface.BusyWait,1, Tm.swdd.BusyWaitReEntry,1 */
/* !LINKSTO Tm.swdd.BusyWaitInterrupts,1 */
FUNC(Std_ReturnType, TM_CODE) Tm_BusyWait1us32bit
(
  uint8 WaitingTimeMin
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint32 StartTimeStamp = 0U;
  uint32 CurrentTimeStamp = 0U;

  DBG_TM_BUSYWAIT1US32BIT_ENTRY(WaitingTimeMin);

  /* !LINKSTO SWS_Tm_00001,1, SWS_Tm_00004,1 */
  RetVal = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_32BIT, &StartTimeStamp);

  CurrentTimeStamp = StartTimeStamp;

  /* !LINKSTO SWS_Tm_00022,1, SWS_Tm_00024,1, SWS_Tm_00066_2,1 */
  while ( (E_OK == RetVal) &&
          ( (CurrentTimeStamp - StartTimeStamp) <= (uint32) WaitingTimeMin)
        )
  {
    /* !LINKSTO SWS_Tm_00001,1, SWS_Tm_00004,1 */
    RetVal = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_1US_32BIT, &CurrentTimeStamp);
  }

  if (E_OK != RetVal)
  {
    /* !LINKSTO SWS_Tm_00064_2,1, Tm.swdd.ReportRuntimeError,1 */
    TM_DET_REPORT_RUNTIME_ERROR( TM_SID_BUSY_WAIT_1US_32BIT, TM_E_HARDWARE_TIMER);
  }
  else
  {}

  DBG_TM_BUSYWAIT1US32BIT_EXIT(RetVal, WaitingTimeMin);

  return RetVal;
}

#endif /* #if (TM_ENABLE_PREDEF_TIMER_1US_32BIT == STD_ON) */

#if (TM_ENABLE_PREDEF_TIMER_100US_32BIT == STD_ON)
/* !LINKSTO Tm.swdd.100us32Bit.Interface.ResetTimer,1, Tm.swdd.PointerClass,1 */
FUNC(Std_ReturnType, TM_CODE) Tm_ResetTimer100us32bit
(
  P2VAR(Tm_PredefTimer100us32bitType, AUTOMATIC, TM_APPL_DATA)    TimerPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint32 CurrentTimeStamp = 0U;

  DBG_TM_RESETTIMER100US32BIT_ENTRY(TimerPtr);
  /* !LINKSTO SWS_Tm_00063_0,1 */
#if (TM_DEV_ERROR_DETECT == STD_ON)
  if (NULL_PTR == TimerPtr)
  {
    /* !LINKSTO SWS_Tm_00008,1, Tm.swdd.Err_TM_E_PARAM_POINTER,1 */
    TM_DET_REPORT_ERROR( TM_SID_RESET_TIMER_100US_32BIT, TM_E_PARAM_POINTER);
  }
  else
#endif /* #if (TM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* !LINKSTO SWS_Tm_00001,1, SWS_Tm_00005,1 */
    RetVal = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_100US_32BIT, &CurrentTimeStamp);

    if (E_OK == RetVal)
    {
    /* !LINKSTO SWS_Tm_00006,1, Tm.swdd.ResetFunctionReEntry,1 */
    TS_AtomicAssign32( TimerPtr->TimeValue, CurrentTimeStamp);
    }
    else
    {
      /* !LINKSTO SWS_Tm_00064_3,1, Tm.swdd.ReportRuntimeError,1 */
      TM_DET_REPORT_RUNTIME_ERROR( TM_SID_RESET_TIMER_100US_32BIT, TM_E_HARDWARE_TIMER);
    }
  }

  DBG_TM_RESETTIMER100US32BIT_EXIT(RetVal, TimerPtr);

  return RetVal;
}

/* !LINKSTO Tm.swdd.100us32Bit.Interface.GetTimeSpan,1, Tm.swdd.PointerClass,1 */
FUNC(Std_ReturnType, TM_CODE) Tm_GetTimeSpan100us32bit
(
  P2CONST(Tm_PredefTimer100us32bitType, AUTOMATIC, TM_APPL_DATA)  TimerPtr,
  P2VAR(uint32, AUTOMATIC, TM_APPL_DATA)                          TimeSpanPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint32 CurrentTimeStamp = 0U;

  DBG_TM_GETTIMESPAN100US32BIT_ENTRY(TimerPtr, TimeSpanPtr);
#if (TM_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO SWS_Tm_00012_0,1, SWS_Tm_00012_1,1 */
  if ((NULL_PTR == TimerPtr) || (NULL_PTR == TimeSpanPtr))
  {
    /* !LINKSTO Tm.swdd.Err_TM_E_PARAM_POINTER,1 */
    TM_DET_REPORT_ERROR( TM_SID_GET_TIME_SPAN_100US_32BIT, TM_E_PARAM_POINTER);
  }
  else
#endif /* #if (TM_DEV_ERROR_DETECT == STD_ON) */
  {
    if (TimeSpanPtr != NULL_PTR)
    {
      /* !LINKSTO SWS_Tm_00001,1, SWS_Tm_00005,1 */
      RetVal = Gpt_GetPredefTimerValue(GPT_PREDEF_TIMER_100US_32BIT, &CurrentTimeStamp);

      if (E_OK == RetVal)
      {
        /* !LINKSTO SWS_Tm_00009,1, SWS_Tm_00010,1, Tm.swdd.GetTmeSpanFunctionReEntry,1 */
        TS_AtomicAssign32( *TimeSpanPtr, (uint32) (CurrentTimeStamp - TimerPtr->TimeValue));
      }
      else
      {
        /* !LINKSTO SWS_Tm_00065,1 */
        TS_AtomicAssign32(*TimeSpanPtr, 0U);

        /* !LINKSTO SWS_Tm_00064_3,1, Tm.swdd.ReportRuntimeError,1 */
        TM_DET_REPORT_RUNTIME_ERROR( TM_SID_GET_TIME_SPAN_100US_32BIT, TM_E_HARDWARE_TIMER);
      }
    }
  }

  DBG_TM_GETTIMESPAN100US32BIT_EXIT(RetVal, TimerPtr, TimeSpanPtr);

  return RetVal;
}

/* !LINKSTO Tm.swdd.100us32Bit.Interface.ShiftTimer,1, Tm.swdd.PointerClass,1 */
FUNC(void, TM_CODE) Tm_ShiftTimer100us32bit
(
  P2VAR(Tm_PredefTimer100us32bitType, AUTOMATIC, TM_APPL_DATA)    TimerPtr,
  uint32 TimeValue
)
{
  DBG_TM_SHIFTTIMER100US32BIT_ENTRY(TimerPtr, TimeValue);
  /* !LINKSTO SWS_Tm_00063_0,1 */
#if (TM_DEV_ERROR_DETECT == STD_ON)
  if (NULL_PTR == TimerPtr)
  {
    /* !LINKSTO SWS_Tm_00018,1, Tm.swdd.Err_TM_E_PARAM_POINTER,1 */
    TM_DET_REPORT_ERROR( TM_SID_SHIFT_TIMER_100US_32BIT, TM_E_PARAM_POINTER);
  }
  else
#endif /* #if (TM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* !LINKSTO SWS_Tm_00013,1, SWS_Tm_00014,1, Tm.swdd.ShiftTimerFunctionReEntry,1 */
    TS_AtomicAssign32( TimerPtr->TimeValue, (uint32) (TimerPtr->TimeValue + TimeValue));  /* overflow is considered as a time reset to 0 */
  }

  DBG_TM_SHIFTTIMER100US32BIT_EXIT(TimerPtr, TimeValue);
}

/* !LINKSTO Tm.swdd.100us32Bit.Interface.SyncTimer,1, Tm.swdd.PointerClass,1 */
FUNC(void, TM_CODE) Tm_SyncTimer100us32bit
(
  P2VAR(Tm_PredefTimer100us32bitType, AUTOMATIC, TM_APPL_DATA)    TimerDstPtr,
  P2CONST(Tm_PredefTimer100us32bitType, AUTOMATIC, TM_APPL_DATA)  TimerSrcPtr
)
{
  DBG_TM_SYNCTIMER100US32BIT_ENTRY(TimerDstPtr, TimerSrcPtr);
  /* !LINKSTO SWS_Tm_00063_0,1 */
#if (TM_DEV_ERROR_DETECT == STD_ON)
  if ((NULL_PTR == TimerDstPtr) || (NULL_PTR == TimerSrcPtr))
  {
    /* !LINKSTO SWS_Tm_00021,1, Tm.swdd.Err_TM_E_PARAM_POINTER,1 */
    TM_DET_REPORT_ERROR( TM_SID_SYNC_TIMER_100US_32BIT, TM_E_PARAM_POINTER);
  }
  else
#endif /* #if (TM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* !LINKSTO SWS_Tm_00019,1, Tm.swdd.SyncTimerFunctionReEntry,1 */
    TS_AtomicAssign32( TimerDstPtr->TimeValue, TimerSrcPtr->TimeValue);
  }

  DBG_TM_SYNCTIMER100US32BIT_EXIT(TimerDstPtr, TimerSrcPtr);
}

#endif /* #if (TM_ENABLE_PREDEF_TIMER_100US_32BIT == STD_ON) */

#define TM_STOP_SEC_CODE
#include <Tm_MemMap.h>

/*==[Definition of functions with internal linkage]===========================*/

