/**
 * \file
 *
 * \brief AUTOSAR ApplTemplates
 *
 * This file contains the implementation of the AUTOSAR
 * module ApplTemplates.
 *
 * \version 6.8.0
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==[Includes]================================================================*/

#include <Std_Types.h>
#include <Mcu.h>
#include <EBStubs_Cfg.h>

/*==[Macros]==================================================================*/

/*==[Types]===================================================================*/

#ifndef EBSTUBS_FUNCTION_TRACE
#define EBSTUBS_FUNCTION_TRACE STD_OFF
#endif

#if (EBSTUBS_FUNCTION_TRACE == STD_ON)
#include <EBStubs_Trace.h>
#endif

/*==[Declaration of functions with internal linkage]==========================*/

/*==[Constants with internal linkage]=========================================*/

/*==[Variables with internal linkage]=========================================*/

/*==[Constants with external linkage]=========================================*/

/*==[Variables with external linkage]=========================================*/

#define EBSTUBS_START_SEC_VAR_UNSPECIFIED
#include <MemMap.h>

#if (EBSTUBS_FUNCENABLED_MCU_SETMODE == STD_ON)
VAR(Mcu_SetMode_StubType, EBSTUBS_VAR)
   Mcu_SetMode_Stub = MCU_SETMODE_STUB_INIT;
#endif

#if (EBSTUBS_FUNCENABLED_MCU_GETRESETRAWVALUE == STD_ON)
VAR(Mcu_GetResetRawValue_StubType, EBSTUBS_VAR)
   Mcu_GetResetRawValue_Stub = MCU_GETRESETRAWVALUE_STUB_INIT;
#endif

#if (EBSTUBS_FUNCENABLED_MCU_GETPLLSTATUS == STD_ON)
VAR(Mcu_GetPllStatus_StubType, EBSTUBS_VAR)
   Mcu_GetPllStatus_Stub = MCU_GETPLLSTATUS_STUB_INIT;
#endif

#if (EBSTUBS_FUNCENABLED_MCU_INITCLOCK == STD_ON)
VAR(Mcu_InitClock_StubType, EBSTUBS_VAR)
   Mcu_InitClock_Stub = MCU_INITCLOCK_STUB_INIT;
#endif

#if (EBSTUBS_FUNCENABLED_MCU_INITRAMSECTION == STD_ON)
VAR(Mcu_InitRamSection_StubType, EBSTUBS_VAR)
   Mcu_InitRamSection_Stub = MCU_INITRAMSECTION_STUB_INIT;
#endif

#if (EBSTUBS_FUNCENABLED_MCU_PERFORMRESET == STD_ON)
VAR(Mcu_PerformReset_StubType, EBSTUBS_VAR)
   Mcu_PerformReset_Stub = MCU_PERFORMRESET_STUB_INIT;
#endif

#if (EBSTUBS_FUNCENABLED_MCU_DISTRIBUTEPLLCLOCK == STD_ON)
VAR(Mcu_DistributePllClock_StubType, EBSTUBS_VAR)
   Mcu_DistributePllClock_Stub = MCU_DISTRIBUTEPLLCLOCK_STUB_INIT;
#endif

#if (EBSTUBS_FUNCENABLED_MCU_GETRESETREASON == STD_ON)
VAR(Mcu_GetResetReason_StubType, EBSTUBS_VAR)
   Mcu_GetResetReason_Stub = MCU_GETRESETREASON_STUB_INIT;
#endif

#if (EBSTUBS_FUNCENABLED_MCU_INIT == STD_ON)
VAR(Mcu_Init_StubType, EBSTUBS_VAR)
   Mcu_Init_Stub = MCU_INIT_STUB_INIT;
#endif

#if (EBSTUBS_FUNCENABLED_MCU_GETVERSIONINFO == STD_ON)
VAR(Mcu_GetVersionInfo_StubType, EBSTUBS_VAR)
   Mcu_GetVersionInfo_Stub = MCU_GETVERSIONINFO_STUB_INIT;
#endif

#if (EBSTUBS_FUNCENABLED_MCU_GETRAMSTATE == STD_ON)
VAR(Mcu_GetRamState_StubType, EBSTUBS_VAR)
   Mcu_GetRamState_Stub = MCU_GETRAMSTATE_STUB_INIT;
#endif

#define EBSTUBS_STOP_SEC_VAR_UNSPECIFIED
#include <MemMap.h>

/*==[Definition of functions with external linkage]===========================*/

#define EBSTUBS_START_SEC_CODE
#include <MemMap.h>

#if (EBSTUBS_FUNCENABLED_MCU_GETRESETREASON == STD_ON)
FUNC(Mcu_ResetType, EBSTUBS_CODE) Mcu_GetResetReason
(
  void
)
{
  Mcu_ResetType result;

#if (EBSTUBS_MAX_CALLS_MCU_GETRESETREASON > 1)
  uint32 count = Mcu_GetResetReason_Stub.Count;
#endif

  result = Mcu_GetResetReason_Stub.RetVal;

#if (EBSTUBS_MAX_CALLS_MCU_GETRESETREASON > 1)
  if( count < EBSTUBS_MAX_CALLS_MCU_GETRESETREASON )
   {
    result = Mcu_GetResetReason_Stub.RetVal_Ary[count];
  }
#endif

  if ( Mcu_GetResetReason_Stub.Callout != NULL_PTR )
  {
    result = Mcu_GetResetReason_Stub.Callout();
  }

  Mcu_GetResetReason_Stub.Count++;

#if (EBSTUBS_FUNCTION_TRACE == STD_ON)
  EBSTUBS_TRACE_PUSH(Mcu_GetResetReason);
#endif

  return result;
}
#endif

#if (EBSTUBS_FUNCENABLED_MCU_INIT == STD_ON)
FUNC(void, EBSTUBS_CODE) Mcu_Init
(
  P2CONST(Mcu_ConfigType, AUTOMATIC, EBSTUBS_APPL_CONST) ConfigPtr
)
{

#if (EBSTUBS_MAX_CALLS_MCU_INIT > 1)
  uint32 count = Mcu_Init_Stub.Count;
#endif

  Mcu_Init_Stub.ConfigPtr = ConfigPtr;

#if (EBSTUBS_MAX_CALLS_MCU_INIT > 1)
  if( count < EBSTUBS_MAX_CALLS_MCU_INIT )
   {
    Mcu_Init_Stub.ConfigPtr_Ary[count] = ConfigPtr;
  }
#endif

  if ( Mcu_Init_Stub.Callout != NULL_PTR )
  {
    Mcu_Init_Stub.Callout(ConfigPtr);
  }

  Mcu_Init_Stub.Count++;

#if (EBSTUBS_FUNCTION_TRACE == STD_ON)
  EBSTUBS_TRACE_PUSH(Mcu_Init);
#endif
}
#endif

#if (EBSTUBS_FUNCENABLED_MCU_PERFORMRESET == STD_ON)
FUNC(void, EBSTUBS_CODE) Mcu_PerformReset
(
  void
)
{

  if ( Mcu_PerformReset_Stub.Callout != NULL_PTR )
  {
    Mcu_PerformReset_Stub.Callout();
  }

  Mcu_PerformReset_Stub.Count++;

#if (EBSTUBS_FUNCTION_TRACE == STD_ON)
  EBSTUBS_TRACE_PUSH(Mcu_PerformReset);
#endif
}
#endif

#if (EBSTUBS_FUNCENABLED_MCU_SETMODE == STD_ON)
FUNC(void, EBSTUBS_CODE) Mcu_SetMode
(
  Mcu_ModeType McuMode
)
{

#if (EBSTUBS_MAX_CALLS_MCU_SETMODE > 1)
  uint32 count = Mcu_SetMode_Stub.Count;
#endif

  Mcu_SetMode_Stub.McuMode = McuMode;

#if (EBSTUBS_MAX_CALLS_MCU_SETMODE > 1)
  if( count < EBSTUBS_MAX_CALLS_MCU_SETMODE )
   {
    Mcu_SetMode_Stub.McuMode_Ary[count] = McuMode;
  }
#endif

  if ( Mcu_SetMode_Stub.Callout != NULL_PTR )
  {
    Mcu_SetMode_Stub.Callout(McuMode);
  }

  Mcu_SetMode_Stub.Count++;

#if (EBSTUBS_FUNCTION_TRACE == STD_ON)
  EBSTUBS_TRACE_PUSH(Mcu_SetMode);
#endif
}
#endif

#if (EBSTUBS_FUNCENABLED_MCU_DISTRIBUTEPLLCLOCK == STD_ON)
FUNC(void, EBSTUBS_CODE) Mcu_DistributePllClock
(
  void
)
{

  if ( Mcu_DistributePllClock_Stub.Callout != NULL_PTR )
  {
    Mcu_DistributePllClock_Stub.Callout();
  }

  Mcu_DistributePllClock_Stub.Count++;

#if (EBSTUBS_FUNCTION_TRACE == STD_ON)
  EBSTUBS_TRACE_PUSH(Mcu_DistributePllClock);
#endif
}
#endif

#if (EBSTUBS_FUNCENABLED_MCU_GETPLLSTATUS == STD_ON)
FUNC(Mcu_PllStatusType, EBSTUBS_CODE) Mcu_GetPllStatus
(
  void
)
{
  Mcu_PllStatusType result;

#if (EBSTUBS_MAX_CALLS_MCU_GETPLLSTATUS > 1)
  uint32 count = Mcu_GetPllStatus_Stub.Count;
#endif

  result = Mcu_GetPllStatus_Stub.RetVal;

#if (EBSTUBS_MAX_CALLS_MCU_GETPLLSTATUS > 1)
  if( count < EBSTUBS_MAX_CALLS_MCU_GETPLLSTATUS )
   {
    result = Mcu_GetPllStatus_Stub.RetVal_Ary[count];
  }
#endif

  if ( Mcu_GetPllStatus_Stub.Callout != NULL_PTR )
  {
    result = Mcu_GetPllStatus_Stub.Callout();
  }

  Mcu_GetPllStatus_Stub.Count++;

#if (EBSTUBS_FUNCTION_TRACE == STD_ON)
  EBSTUBS_TRACE_PUSH(Mcu_GetPllStatus);
#endif

  return result;
}
#endif

#if (EBSTUBS_FUNCENABLED_MCU_GETRAMSTATE == STD_ON)
FUNC(Mcu_RamStateType, EBSTUBS_CODE) Mcu_GetRamState
(
  void
)
{
  Mcu_RamStateType result;

#if (EBSTUBS_MAX_CALLS_MCU_GETRAMSTATE > 1)
  uint32 count = Mcu_GetRamState_Stub.Count;
#endif

  result = Mcu_GetRamState_Stub.RetVal;

#if (EBSTUBS_MAX_CALLS_MCU_GETRAMSTATE > 1)
  if( count < EBSTUBS_MAX_CALLS_MCU_GETRAMSTATE )
   {
    result = Mcu_GetRamState_Stub.RetVal_Ary[count];
  }
#endif

  if ( Mcu_GetRamState_Stub.Callout != NULL_PTR )
  {
    result = Mcu_GetRamState_Stub.Callout();
  }

  Mcu_GetRamState_Stub.Count++;

#if (EBSTUBS_FUNCTION_TRACE == STD_ON)
  EBSTUBS_TRACE_PUSH(Mcu_GetRamState);
#endif

  return result;
}
#endif

#if (EBSTUBS_FUNCENABLED_MCU_GETRESETRAWVALUE == STD_ON)
FUNC(Mcu_RawResetType, EBSTUBS_CODE) Mcu_GetResetRawValue
(
  void
)
{
  Mcu_RawResetType result;

#if (EBSTUBS_MAX_CALLS_MCU_GETRESETRAWVALUE > 1)
  uint32 count = Mcu_GetResetRawValue_Stub.Count;
#endif

  result = Mcu_GetResetRawValue_Stub.RetVal;

#if (EBSTUBS_MAX_CALLS_MCU_GETRESETRAWVALUE > 1)
  if( count < EBSTUBS_MAX_CALLS_MCU_GETRESETRAWVALUE )
   {
    result = Mcu_GetResetRawValue_Stub.RetVal_Ary[count];
  }
#endif

  if ( Mcu_GetResetRawValue_Stub.Callout != NULL_PTR )
  {
    result = Mcu_GetResetRawValue_Stub.Callout();
  }

  Mcu_GetResetRawValue_Stub.Count++;

#if (EBSTUBS_FUNCTION_TRACE == STD_ON)
  EBSTUBS_TRACE_PUSH(Mcu_GetResetRawValue);
#endif

  return result;
}
#endif

#if (EBSTUBS_FUNCENABLED_MCU_GETVERSIONINFO == STD_ON)
FUNC(void, EBSTUBS_CODE) Mcu_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, EBSTUBS_APPL_DATA) versioninfo
)
{

#if (EBSTUBS_MAX_CALLS_MCU_GETVERSIONINFO > 1)
  uint32 count = Mcu_GetVersionInfo_Stub.Count;
#endif

  Mcu_GetVersionInfo_Stub.versioninfo = versioninfo;

#if (EBSTUBS_MAX_CALLS_MCU_GETVERSIONINFO > 1)
  if( count < EBSTUBS_MAX_CALLS_MCU_GETVERSIONINFO )
   {
    Mcu_GetVersionInfo_Stub.versioninfo_Ary[count] = versioninfo;
  }
#endif

  if ( Mcu_GetVersionInfo_Stub.Callout != NULL_PTR )
  {
    Mcu_GetVersionInfo_Stub.Callout(versioninfo);
  }

  Mcu_GetVersionInfo_Stub.Count++;

#if (EBSTUBS_FUNCTION_TRACE == STD_ON)
  EBSTUBS_TRACE_PUSH(Mcu_GetVersionInfo);
#endif
}
#endif

#if (EBSTUBS_FUNCENABLED_MCU_INITCLOCK == STD_ON)
FUNC(Std_ReturnType, EBSTUBS_CODE) Mcu_InitClock
(
  Mcu_ClockType ClockSetting
)
{
  Std_ReturnType result;

#if (EBSTUBS_MAX_CALLS_MCU_INITCLOCK > 1)
  uint32 count = Mcu_InitClock_Stub.Count;
#endif

  result = Mcu_InitClock_Stub.RetVal;
  Mcu_InitClock_Stub.ClockSetting = ClockSetting;

#if (EBSTUBS_MAX_CALLS_MCU_INITCLOCK > 1)
  if( count < EBSTUBS_MAX_CALLS_MCU_INITCLOCK )
   {
    Mcu_InitClock_Stub.ClockSetting_Ary[count] = ClockSetting;
    result = Mcu_InitClock_Stub.RetVal_Ary[count];
  }
#endif

  if ( Mcu_InitClock_Stub.Callout != NULL_PTR )
  {
    result = Mcu_InitClock_Stub.Callout(ClockSetting);
  }

  Mcu_InitClock_Stub.Count++;

#if (EBSTUBS_FUNCTION_TRACE == STD_ON)
  EBSTUBS_TRACE_PUSH(Mcu_InitClock);
#endif

  return result;
}
#endif

#if (EBSTUBS_FUNCENABLED_MCU_INITRAMSECTION == STD_ON)
FUNC(Std_ReturnType, EBSTUBS_CODE) Mcu_InitRamSection
(
  Mcu_RamSectionType RamSection
)
{
  Std_ReturnType result;

#if (EBSTUBS_MAX_CALLS_MCU_INITRAMSECTION > 1)
  uint32 count = Mcu_InitRamSection_Stub.Count;
#endif

  result = Mcu_InitRamSection_Stub.RetVal;
  Mcu_InitRamSection_Stub.RamSection = RamSection;

#if (EBSTUBS_MAX_CALLS_MCU_INITRAMSECTION > 1)
  if( count < EBSTUBS_MAX_CALLS_MCU_INITRAMSECTION )
   {
    Mcu_InitRamSection_Stub.RamSection_Ary[count] = RamSection;
    result = Mcu_InitRamSection_Stub.RetVal_Ary[count];
  }
#endif

  if ( Mcu_InitRamSection_Stub.Callout != NULL_PTR )
  {
    result = Mcu_InitRamSection_Stub.Callout(RamSection);
  }

  Mcu_InitRamSection_Stub.Count++;

#if (EBSTUBS_FUNCTION_TRACE == STD_ON)
  EBSTUBS_TRACE_PUSH(Mcu_InitRamSection);
#endif

  return result;
}
#endif

#define EBSTUBS_STOP_SEC_CODE
#include <MemMap.h>

/*==[Definition of functions with internal linkage]===========================*/

