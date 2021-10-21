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

#ifndef MCU_H
#define MCU_H

/*==[Includes]================================================================*/

#include <Std_Types.h>
#include <Mcu_StubCfg.h>

/*==[Macros]==================================================================*/

/* Configuration macros */
#ifndef EBSTUBS_MAX_CALLS_MCU
#define EBSTUBS_MAX_CALLS_MCU 1U
#endif

#ifndef EBSTUBS_MAX_CALLS_MCU_GETRESETREASON
#define EBSTUBS_MAX_CALLS_MCU_GETRESETREASON EBSTUBS_MAX_CALLS_MCU
#endif

#ifndef EBSTUBS_MAX_CALLS_MCU_INIT
#define EBSTUBS_MAX_CALLS_MCU_INIT EBSTUBS_MAX_CALLS_MCU
#endif

#ifndef EBSTUBS_MAX_CALLS_MCU_PERFORMRESET
#define EBSTUBS_MAX_CALLS_MCU_PERFORMRESET EBSTUBS_MAX_CALLS_MCU
#endif

#ifndef EBSTUBS_MAX_CALLS_MCU_SETMODE
#define EBSTUBS_MAX_CALLS_MCU_SETMODE EBSTUBS_MAX_CALLS_MCU
#endif

#ifndef EBSTUBS_MAX_CALLS_MCU_DISTRIBUTEPLLCLOCK
#define EBSTUBS_MAX_CALLS_MCU_DISTRIBUTEPLLCLOCK EBSTUBS_MAX_CALLS_MCU
#endif

#ifndef EBSTUBS_MAX_CALLS_MCU_GETPLLSTATUS
#define EBSTUBS_MAX_CALLS_MCU_GETPLLSTATUS EBSTUBS_MAX_CALLS_MCU
#endif

#ifndef EBSTUBS_MAX_CALLS_MCU_GETRAMSTATE
#define EBSTUBS_MAX_CALLS_MCU_GETRAMSTATE EBSTUBS_MAX_CALLS_MCU
#endif

#ifndef EBSTUBS_MAX_CALLS_MCU_GETRESETRAWVALUE
#define EBSTUBS_MAX_CALLS_MCU_GETRESETRAWVALUE EBSTUBS_MAX_CALLS_MCU
#endif

#ifndef EBSTUBS_MAX_CALLS_MCU_GETVERSIONINFO
#define EBSTUBS_MAX_CALLS_MCU_GETVERSIONINFO EBSTUBS_MAX_CALLS_MCU
#endif

#ifndef EBSTUBS_MAX_CALLS_MCU_INITCLOCK
#define EBSTUBS_MAX_CALLS_MCU_INITCLOCK EBSTUBS_MAX_CALLS_MCU
#endif

#ifndef EBSTUBS_MAX_CALLS_MCU_INITRAMSECTION
#define EBSTUBS_MAX_CALLS_MCU_INITRAMSECTION EBSTUBS_MAX_CALLS_MCU
#endif

#ifndef EBSTUBS_FUNCENABLED_MCU_GETRESETREASON
#define EBSTUBS_FUNCENABLED_MCU_GETRESETREASON STD_ON
#endif

#ifndef EBSTUBS_FUNCENABLED_MCU_INIT
#define EBSTUBS_FUNCENABLED_MCU_INIT STD_ON
#endif

#ifndef EBSTUBS_FUNCENABLED_MCU_PERFORMRESET
#define EBSTUBS_FUNCENABLED_MCU_PERFORMRESET STD_ON
#endif

#ifndef EBSTUBS_FUNCENABLED_MCU_SETMODE
#define EBSTUBS_FUNCENABLED_MCU_SETMODE STD_ON
#endif

#ifndef EBSTUBS_FUNCENABLED_MCU_DISTRIBUTEPLLCLOCK
#define EBSTUBS_FUNCENABLED_MCU_DISTRIBUTEPLLCLOCK STD_ON
#endif

#ifndef EBSTUBS_FUNCENABLED_MCU_GETPLLSTATUS
#define EBSTUBS_FUNCENABLED_MCU_GETPLLSTATUS STD_ON
#endif

#ifndef EBSTUBS_FUNCENABLED_MCU_GETRAMSTATE
#define EBSTUBS_FUNCENABLED_MCU_GETRAMSTATE STD_ON
#endif

#ifndef EBSTUBS_FUNCENABLED_MCU_GETRESETRAWVALUE
#define EBSTUBS_FUNCENABLED_MCU_GETRESETRAWVALUE STD_ON
#endif

#ifndef EBSTUBS_FUNCENABLED_MCU_GETVERSIONINFO
#define EBSTUBS_FUNCENABLED_MCU_GETVERSIONINFO STD_ON
#endif

#ifndef EBSTUBS_FUNCENABLED_MCU_INITCLOCK
#define EBSTUBS_FUNCENABLED_MCU_INITCLOCK STD_ON
#endif

#ifndef EBSTUBS_FUNCENABLED_MCU_INITRAMSECTION
#define EBSTUBS_FUNCENABLED_MCU_INITRAMSECTION STD_ON
#endif

#if (EBSTUBS_MAX_CALLS_MCU_INIT > 1)
#define MCU_INIT_STUB_INIT { 0, NULL_PTR, { NULL_PTR }, NULL_PTR }
#else
#define MCU_INIT_STUB_INIT { 0, NULL_PTR, NULL_PTR }
#endif

#if (EBSTUBS_MAX_CALLS_MCU_PERFORMRESET > 1)
#define MCU_PERFORMRESET_STUB_INIT { 0, NULL_PTR }
#else
#define MCU_PERFORMRESET_STUB_INIT { 0, NULL_PTR }
#endif

#if (EBSTUBS_MAX_CALLS_MCU_SETMODE > 1)
#define MCU_SETMODE_STUB_INIT { 0, NULL_PTR, { 0 }, 0 }
#else
#define MCU_SETMODE_STUB_INIT { 0, NULL_PTR, 0 }
#endif

#if (EBSTUBS_MAX_CALLS_MCU_DISTRIBUTEPLLCLOCK > 1)
#define MCU_DISTRIBUTEPLLCLOCK_STUB_INIT { 0, NULL_PTR }
#else
#define MCU_DISTRIBUTEPLLCLOCK_STUB_INIT { 0, NULL_PTR }
#endif

#if (EBSTUBS_MAX_CALLS_MCU_GETPLLSTATUS > 1)
#define MCU_GETPLLSTATUS_STUB_INIT { 0, NULL_PTR, { 0 }, 0 }
#else
#define MCU_GETPLLSTATUS_STUB_INIT { 0, NULL_PTR, 0 }
#endif

#if (EBSTUBS_MAX_CALLS_MCU_GETRAMSTATE > 1)
#define MCU_GETRAMSTATE_STUB_INIT { 0, NULL_PTR, { 0 }, 0 }
#else
#define MCU_GETRAMSTATE_STUB_INIT { 0, NULL_PTR, 0 }
#endif

#if (EBSTUBS_MAX_CALLS_MCU_GETRESETRAWVALUE > 1)
#define MCU_GETRESETRAWVALUE_STUB_INIT { 0, NULL_PTR, { 0 }, 0 }
#else
#define MCU_GETRESETRAWVALUE_STUB_INIT { 0, NULL_PTR, 0 }
#endif

#if (EBSTUBS_MAX_CALLS_MCU_GETVERSIONINFO > 1)
#define MCU_GETVERSIONINFO_STUB_INIT { 0, NULL_PTR, { NULL_PTR }, NULL_PTR }
#else
#define MCU_GETVERSIONINFO_STUB_INIT { 0, NULL_PTR, NULL_PTR }
#endif

#if (EBSTUBS_MAX_CALLS_MCU_INITCLOCK > 1)
#define MCU_INITCLOCK_STUB_INIT { 0, NULL_PTR, { 0 }, { 0 }, 0, 0 }
#else
#define MCU_INITCLOCK_STUB_INIT { 0, NULL_PTR, 0, 0 }
#endif

#if (EBSTUBS_MAX_CALLS_MCU_INITRAMSECTION > 1)
#define MCU_INITRAMSECTION_STUB_INIT { 0, NULL_PTR, { 0 }, { 0 }, 0, 0 }
#else
#define MCU_INITRAMSECTION_STUB_INIT { 0, NULL_PTR, 0, 0 }
#endif


/*==[Types]===================================================================*/

typedef enum
{
   MCU_RAMSTATE_INVALID,
   MCU_RAMSTATE_VALID
} Mcu_RamStateType;

typedef struct
{
   uint8 dummy;
} Mcu_ConfigType;

typedef enum
{
   MCU_PLL_LOCKED,
   MCU_PLL_UNLOCKED,
   MCU_PLL_STATUS_UNDEFINED
} Mcu_PllStatusType;

typedef uint8 Mcu_ClockType;

typedef enum
{
   MCU_POWER_ON_RESET,
   MCU_WATCHDOG_RESET,
   MCU_SW_RESET,
   MCU_RESET_UNDEFINED
} Mcu_ResetType;

typedef uint8 Mcu_RawResetType;

typedef uint8 Mcu_ModeType;

typedef uint8 Mcu_RamSectionType;

#if (EBSTUBS_FUNCENABLED_MCU_GETRESETREASON == STD_ON)
typedef Mcu_ResetType(*Mcu_GetResetReason_CalloutType)
(
   void
);
typedef struct
{
   uint32 Count;
   Mcu_GetResetReason_CalloutType Callout;

#if (EBSTUBS_MAX_CALLS_MCU_GETRESETREASON > 1)
   Mcu_ResetType RetVal_Ary[EBSTUBS_MAX_CALLS_MCU_GETRESETREASON];
#endif

   Mcu_ResetType RetVal;
} Mcu_GetResetReason_StubType;
#endif

#if (EBSTUBS_FUNCENABLED_MCU_INIT == STD_ON)
typedef void(*Mcu_Init_CalloutType)
(
   P2CONST(Mcu_ConfigType, TYPEDEF, EBSTUBS_APPL_CONST)
);
typedef struct
{
   uint32 Count;
   Mcu_Init_CalloutType Callout;

#if (EBSTUBS_MAX_CALLS_MCU_INIT > 1)
   P2CONST(Mcu_ConfigType, TYPEDEF, EBSTUBS_APPL_CONST) ConfigPtr_Ary[EBSTUBS_MAX_CALLS_MCU_INIT];
#endif

   P2CONST(Mcu_ConfigType, TYPEDEF, EBSTUBS_APPL_CONST) ConfigPtr;
} Mcu_Init_StubType;
#endif

#if (EBSTUBS_FUNCENABLED_MCU_PERFORMRESET == STD_ON)
typedef void(*Mcu_PerformReset_CalloutType)
(
   void
);
typedef struct
{
   uint32 Count;
   Mcu_PerformReset_CalloutType Callout;

#if (EBSTUBS_MAX_CALLS_MCU_PERFORMRESET > 1)
#endif

} Mcu_PerformReset_StubType;
#endif

#if (EBSTUBS_FUNCENABLED_MCU_SETMODE == STD_ON)
typedef void(*Mcu_SetMode_CalloutType)
(
   Mcu_ModeType
);
typedef struct
{
   uint32 Count;
   Mcu_SetMode_CalloutType Callout;

#if (EBSTUBS_MAX_CALLS_MCU_SETMODE > 1)
   Mcu_ModeType McuMode_Ary[EBSTUBS_MAX_CALLS_MCU_SETMODE];
#endif

   Mcu_ModeType McuMode;
} Mcu_SetMode_StubType;
#endif

#if (EBSTUBS_FUNCENABLED_MCU_DISTRIBUTEPLLCLOCK == STD_ON)
typedef void(*Mcu_DistributePllClock_CalloutType)
(
   void
);
typedef struct
{
   uint32 Count;
   Mcu_DistributePllClock_CalloutType Callout;

#if (EBSTUBS_MAX_CALLS_MCU_DISTRIBUTEPLLCLOCK > 1)
#endif

} Mcu_DistributePllClock_StubType;
#endif

#if (EBSTUBS_FUNCENABLED_MCU_GETPLLSTATUS == STD_ON)
typedef Mcu_PllStatusType(*Mcu_GetPllStatus_CalloutType)
(
   void
);
typedef struct
{
   uint32 Count;
   Mcu_GetPllStatus_CalloutType Callout;

#if (EBSTUBS_MAX_CALLS_MCU_GETPLLSTATUS > 1)
   Mcu_PllStatusType RetVal_Ary[EBSTUBS_MAX_CALLS_MCU_GETPLLSTATUS];
#endif

   Mcu_PllStatusType RetVal;
} Mcu_GetPllStatus_StubType;
#endif

#if (EBSTUBS_FUNCENABLED_MCU_GETRAMSTATE == STD_ON)
typedef Mcu_RamStateType(*Mcu_GetRamState_CalloutType)
(
   void
);
typedef struct
{
   uint32 Count;
   Mcu_GetRamState_CalloutType Callout;

#if (EBSTUBS_MAX_CALLS_MCU_GETRAMSTATE > 1)
   Mcu_RamStateType RetVal_Ary[EBSTUBS_MAX_CALLS_MCU_GETRAMSTATE];
#endif

   Mcu_RamStateType RetVal;
} Mcu_GetRamState_StubType;
#endif

#if (EBSTUBS_FUNCENABLED_MCU_GETRESETRAWVALUE == STD_ON)
typedef Mcu_RawResetType(*Mcu_GetResetRawValue_CalloutType)
(
   void
);
typedef struct
{
   uint32 Count;
   Mcu_GetResetRawValue_CalloutType Callout;

#if (EBSTUBS_MAX_CALLS_MCU_GETRESETRAWVALUE > 1)
   Mcu_RawResetType RetVal_Ary[EBSTUBS_MAX_CALLS_MCU_GETRESETRAWVALUE];
#endif

   Mcu_RawResetType RetVal;
} Mcu_GetResetRawValue_StubType;
#endif

#if (EBSTUBS_FUNCENABLED_MCU_GETVERSIONINFO == STD_ON)
typedef void(*Mcu_GetVersionInfo_CalloutType)
(
   P2VAR(Std_VersionInfoType, TYPEDEF, EBSTUBS_APPL_DATA)
);
typedef struct
{
   uint32 Count;
   Mcu_GetVersionInfo_CalloutType Callout;

#if (EBSTUBS_MAX_CALLS_MCU_GETVERSIONINFO > 1)
   P2VAR(Std_VersionInfoType, TYPEDEF, EBSTUBS_APPL_DATA) versioninfo_Ary[EBSTUBS_MAX_CALLS_MCU_GETVERSIONINFO];
#endif

   P2VAR(Std_VersionInfoType, TYPEDEF, EBSTUBS_APPL_DATA) versioninfo;
} Mcu_GetVersionInfo_StubType;
#endif

#if (EBSTUBS_FUNCENABLED_MCU_INITCLOCK == STD_ON)
typedef Std_ReturnType(*Mcu_InitClock_CalloutType)
(
   Mcu_ClockType
);
typedef struct
{
   uint32 Count;
   Mcu_InitClock_CalloutType Callout;

#if (EBSTUBS_MAX_CALLS_MCU_INITCLOCK > 1)
   Mcu_ClockType ClockSetting_Ary[EBSTUBS_MAX_CALLS_MCU_INITCLOCK];
   Std_ReturnType RetVal_Ary[EBSTUBS_MAX_CALLS_MCU_INITCLOCK];
#endif

   Mcu_ClockType ClockSetting;
   Std_ReturnType RetVal;
} Mcu_InitClock_StubType;
#endif

#if (EBSTUBS_FUNCENABLED_MCU_INITRAMSECTION == STD_ON)
typedef Std_ReturnType(*Mcu_InitRamSection_CalloutType)
(
   Mcu_RamSectionType
);
typedef struct
{
   uint32 Count;
   Mcu_InitRamSection_CalloutType Callout;

#if (EBSTUBS_MAX_CALLS_MCU_INITRAMSECTION > 1)
   Mcu_RamSectionType RamSection_Ary[EBSTUBS_MAX_CALLS_MCU_INITRAMSECTION];
   Std_ReturnType RetVal_Ary[EBSTUBS_MAX_CALLS_MCU_INITRAMSECTION];
#endif

   Mcu_RamSectionType RamSection;
   Std_ReturnType RetVal;
} Mcu_InitRamSection_StubType;
#endif

/*==[Declaration of functions with external linkage]==========================*/

#define EBSTUBS_START_SEC_CODE
#include <MemMap.h>

#if (EBSTUBS_FUNCENABLED_MCU_GETRESETREASON == STD_ON)
extern FUNC(Mcu_ResetType, EBSTUBS_CODE) Mcu_GetResetReason
(
  void
);
#endif

#if (EBSTUBS_FUNCENABLED_MCU_INIT == STD_ON)
extern FUNC(void, EBSTUBS_CODE) Mcu_Init
(
  P2CONST(Mcu_ConfigType, AUTOMATIC, EBSTUBS_APPL_CONST) ConfigPtr
);
#endif

#if (EBSTUBS_FUNCENABLED_MCU_PERFORMRESET == STD_ON)
extern FUNC(void, EBSTUBS_CODE) Mcu_PerformReset
(
  void
);
#endif

#if (EBSTUBS_FUNCENABLED_MCU_SETMODE == STD_ON)
extern FUNC(void, EBSTUBS_CODE) Mcu_SetMode
(
  Mcu_ModeType McuMode
);
#endif

#if (EBSTUBS_FUNCENABLED_MCU_DISTRIBUTEPLLCLOCK == STD_ON)
extern FUNC(void, EBSTUBS_CODE) Mcu_DistributePllClock
(
  void
);
#endif

#if (EBSTUBS_FUNCENABLED_MCU_GETPLLSTATUS == STD_ON)
extern FUNC(Mcu_PllStatusType, EBSTUBS_CODE) Mcu_GetPllStatus
(
  void
);
#endif

#if (EBSTUBS_FUNCENABLED_MCU_GETRAMSTATE == STD_ON)
extern FUNC(Mcu_RamStateType, EBSTUBS_CODE) Mcu_GetRamState
(
  void
);
#endif

#if (EBSTUBS_FUNCENABLED_MCU_GETRESETRAWVALUE == STD_ON)
extern FUNC(Mcu_RawResetType, EBSTUBS_CODE) Mcu_GetResetRawValue
(
  void
);
#endif

#if (EBSTUBS_FUNCENABLED_MCU_GETVERSIONINFO == STD_ON)
extern FUNC(void, EBSTUBS_CODE) Mcu_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, EBSTUBS_APPL_DATA) versioninfo
);
#endif

#if (EBSTUBS_FUNCENABLED_MCU_INITCLOCK == STD_ON)
extern FUNC(Std_ReturnType, EBSTUBS_CODE) Mcu_InitClock
(
  Mcu_ClockType ClockSetting
);
#endif

#if (EBSTUBS_FUNCENABLED_MCU_INITRAMSECTION == STD_ON)
extern FUNC(Std_ReturnType, EBSTUBS_CODE) Mcu_InitRamSection
(
  Mcu_RamSectionType RamSection
);
#endif

/* Initialization macros */
/* Initialize with MCU_SW_RESET to prevent a immediate shutdown */
#if (EBSTUBS_MAX_CALLS_MCU_GETRESETREASON > 1)
#define MCU_GETRESETREASON_STUB_INIT { MCU_SW_RESET, NULL_PTR, { 0 }, 0 }
#else
#define MCU_GETRESETREASON_STUB_INIT { MCU_SW_RESET, NULL_PTR, 0 }
#endif


#define EBSTUBS_STOP_SEC_CODE
#include <MemMap.h>

/*==[Constants with external linkage]=========================================*/

/*==[Variables with external linkage]=========================================*/

#define EBSTUBS_START_SEC_VAR_UNSPECIFIED
#include <MemMap.h>

extern VAR(Mcu_GetResetReason_StubType, EBSTUBS_VAR)
   Mcu_GetResetReason_Stub;
extern VAR(Mcu_Init_StubType, EBSTUBS_VAR)
   Mcu_Init_Stub;
extern VAR(Mcu_PerformReset_StubType, EBSTUBS_VAR)
   Mcu_PerformReset_Stub;
extern VAR(Mcu_SetMode_StubType, EBSTUBS_VAR)
   Mcu_SetMode_Stub;
extern VAR(Mcu_DistributePllClock_StubType, EBSTUBS_VAR)
   Mcu_DistributePllClock_Stub;
extern VAR(Mcu_GetPllStatus_StubType, EBSTUBS_VAR)
   Mcu_GetPllStatus_Stub;
extern VAR(Mcu_GetRamState_StubType, EBSTUBS_VAR)
   Mcu_GetRamState_Stub;
extern VAR(Mcu_GetResetRawValue_StubType, EBSTUBS_VAR)
   Mcu_GetResetRawValue_Stub;
extern VAR(Mcu_GetVersionInfo_StubType, EBSTUBS_VAR)
   Mcu_GetVersionInfo_Stub;
extern VAR(Mcu_InitClock_StubType, EBSTUBS_VAR)
   Mcu_InitClock_Stub;
extern VAR(Mcu_InitRamSection_StubType, EBSTUBS_VAR)
   Mcu_InitRamSection_Stub;

#define EBSTUBS_STOP_SEC_VAR_UNSPECIFIED
#include <MemMap.h>

/* stub extension header file */
#include <Mcu_StubExt.h>

#endif /* ifndef MCU_H */
