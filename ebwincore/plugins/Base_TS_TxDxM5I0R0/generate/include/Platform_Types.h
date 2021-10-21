#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H
[!AUTOSPACING!][!//
/**
 * \file
 *
 * \brief AUTOSAR Base
 *
 * This file contains the implementation of the AUTOSAR
 * module Base.
 *
 * \version 5.0.31
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 20.10 (advisory)
 *   The # and ## preprocessor operators should not be used.
 *
 *   Reason:
 *   The operators are used to add the 'U' or 'UL' suffix to constants
 *   via macros which can only be performed using the '##' operator.
 */
/* !LINKSTO Base.ASR403.PLATFORM001,1, Base.ASR403.PLATFORM002,1, Base.ASR403.PLATFORM003,1 */
[!IF "ecu:has('Custom.Platform.Include') and ecu:get('Custom.Platform.Include') != '' "!]

/*==================[inclusions]=============================================*/

#include <[!"ecu:get('Custom.Platform.Include')"!]>
[!ENDIF!]

/*==================[macros]=================================================*/

/*------------------[AUTOSAR vendor identification]-------------------------*/

#if (defined PLATFORM_VENDOR_ID)
#error PLATFORM_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define PLATFORM_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]--------------------------*/

#if (defined PLATFORM_MODULE_ID)
#error PLATFORM_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define PLATFORM_MODULE_ID     199U

/*------------------[AUTOSAR release version identification]-----------------*/

#if (defined PLATFORM_AR_RELEASE_MAJOR_VERSION)
#error PLATFORM_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define PLATFORM_AR_RELEASE_MAJOR_VERSION     4U

#if (defined PLATFORM_AR_RELEASE_MINOR_VERSION)
#error PLATFORM_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define PLATFORM_AR_RELEASE_MINOR_VERSION     0U

#if (defined PLATFORM_AR_RELEASE_REVISION_VERSION)
#error PLATFORM_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define PLATFORM_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]-------------------*/

#if (defined PLATFORM_SW_MAJOR_VERSION)
#error PLATFORM_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define PLATFORM_SW_MAJOR_VERSION  2U

#if (defined PLATFORM_SW_MINOR_VERSION)
#error PLATFORM_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define PLATFORM_SW_MINOR_VERSION  5U

#if (defined PLATFORM_SW_PATCH_VERSION)
#error PLATFORM_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define PLATFORM_SW_PATCH_VERSION  0U

/*------------------[AUTOSAR module vendor API infix]------------------------*/

#if (defined PLATFORM_VENDOR_API_INFIX)
#error PLATFORM_VENDOR_API_INFIX already defined
#endif
/** \brief Vendor API infix
 *
 * Left empty as this header does not belong to any module */
#define PLATFORM_VENDOR_API_INFIX

/*------------------[global identifiers]-------------------------------------*/

/* !LINKSTO Base.ASR403.PLATFORM054,1 */
#ifndef TRUE
/** \brief true value for boolean type */
/* !LINKSTO Base.ASR403.PLATFORM056,1 */
#define TRUE 1U
#endif

/* !LINKSTO Base.ASR403.PLATFORM054,1 */
#ifndef FALSE
/** \brief false value for boolean type */
/* !LINKSTO Base.ASR403.PLATFORM056,1 */
#define FALSE 0U
#endif

/* redefinition check may lead to an incorrect condition that
 * TRUE and FALSE are equal */
/* !LINKSTO Base.ASR403.PLATFORM054,1 */
#if (TRUE == FALSE)
#error TRUE == FALSE is incorrect
#endif

/*------------------[CPU properties]-----------------------------------------*/

#if (defined CPU_TYPE_8)
#error CPU_TYPE_8 already defined
#endif
/** \brief cpu identifier for 8-bit CPUs */
/* !LINKSTO Base.ASR403.PLATFORM057,1, Base.ASR413.SWS_Platform_00064,1 */
#define CPU_TYPE_8 8U

#if (defined CPU_TYPE_16)
#error CPU_TYPE_16 already defined
#endif /* if (defined CPU_TYPE_16) */
/** \brief cpu identifier for 8-bit CPUs */
/* !LINKSTO Base.ASR403.PLATFORM057,1, Base.ASR413.SWS_Platform_00064,1 */
#define CPU_TYPE_16 16U

#if (defined CPU_TYPE_32)
#error CPU_TYPE_32 already defined
#endif
/** \brief cpu identifier for 32-bit CPUs */
/* !LINKSTO Base.ASR403.PLATFORM057,1, Base.ASR413.SWS_Platform_00064,1 */
#define CPU_TYPE_32 32U

/* !LINKSTO Base.ASR413.SWS_Platform_00064,1 */
#if (defined CPU_TYPE_64)
#error CPU_TYPE_64 already defined
#endif
/** \brief cpu identifier for 64-bit CPUs */
#define CPU_TYPE_64 64U

#if (defined MSB_FIRST)
#error MSB_FIRST already defined
#endif
/** \brief identifier for 'high byte first' */
#define MSB_FIRST 0U

#if (defined LSB_FIRST)
#error LSB_FIRST already defined
#endif /* if (defined LSB_FIRST) */
/** \brief identifier for 'little endian' */
#define LSB_FIRST 1U

#if (defined HIGH_BYTE_FIRST)
#error HIGH_BYTE_FIRST already defined
#endif
/** \brief identifier for 'high byte first' */
#define HIGH_BYTE_FIRST 0U

#if (defined LOW_BYTE_FIRST)
#error LOW_BYTE_FIRST already defined
#endif
/** \brief cpu identifier for  8-bit CPUs */
#define LOW_BYTE_FIRST 1U

#if (defined CPU_TYPE)
#error CPU_TYPE already defined
#endif

[!IF "node:exists(as:modconf('Base')[1]/BaseCpuConfig/BaseCpuConfig_type)"!]
  [!VAR "CpuTypeProperty" = "as:modconf('Base')[1]/BaseCpuConfig/BaseCpuConfig_type"!]
  [!IF "$CpuTypeProperty = '8' or $CpuTypeProperty = '16' or  $CpuTypeProperty = '32' or  $CpuTypeProperty = '64'"!]
    [!VAR "CpuType"="concat('CPU_TYPE_',$CpuTypeProperty)"!]
  [!ELSE!][!//
    [!ERROR!]
      Base: Invalid value for configuration parameter 'BaseCpuConfig_type': '[!"$CpuTypeProperty"!]' (must be 8, 16, 32 or 64)
    [!ENDERROR!]
  [!ENDIF!]
[!ELSE!][!//
  [!ERROR!]
      Base: Configuration parameter 'BaseCpuConfig_type' specifying the type of the CPU not found
  [!ENDERROR!]
[!ENDIF!]
/** \brief Word size of this CPU (generated)
 *
 * Platforms Setting: 
 * The word size specifies the natural data unit size of a CPU. 
 * It is set in the Base configuration by
 * setting the parameter 'BaseCpuConfig_type'.
 *
 * Possible values:
 *  - CPU_TYPE_64 (if parameter 'BaseCpuConfig_type:64')
 *  - CPU_TYPE_32 (if parameter 'BaseCpuConfig_type:32')
 *  - CPU_TYPE_16 (if parameter 'BaseCpuConfig_type:16')
 *  - CPU_TYPE_8  (if parameter 'BaseCpuConfig_type:8')
 */
/* !LINKSTO Base.ASR403.PLATFORM044,1, Base.ASR403.PLATFORM045,1, Base.ASR413.SWS_Platform_00064,1, Base.ASR430.SWS_Platform_00045,1 */
#define CPU_TYPE [!WS!][!"concat('CPU_TYPE_',as:modconf('Base')[1]/BaseCpuConfig/BaseCpuConfig_type)"!]

/* !LINKSTO Base.ASR403.PLATFORM043,1, Base.ASR403.PLATFORM048,1, Base.ASR403.PLATFORM049,1, Base.ASR403.PLATFORM038,1 */
#if (defined CPU_BIT_ORDER)
#error CPU_BIT_ORDER already defined
#endif

[!IF "node:exists(as:modconf('Base')[1]/BaseCpuConfig/BaseCpuConfig_bitorder)"!]
  [!VAR "CpuBitorderProperty" = "as:modconf('Base')[1]/BaseCpuConfig/BaseCpuConfig_bitorder"!]
  [!IF "$CpuBitorderProperty = 'LE'"!]
    [!VAR "CpuBitorder" = "'LSB_FIRST'"!]
  [!ELSEIF "$CpuBitorderProperty = 'BE'"!][!//
    [!VAR "CpuBitorder" = "'MSB_FIRST'"!]
  [!ELSE!][!//
    [!ERROR!]
      Base: Invalid value for configuration parameter 'BaseCpuConfig_bitorder': '[!"$CpuBitorderProperty"!]' (must be 'LE' or 'BE')
    [!ENDERROR!]
  [!ENDIF!]
[!ELSE!][!//
  [!ERROR!]
      Base: Configuration parameter 'BaseCpuConfig_bitorder' specifying the bit order of the CPU not found
  [!ENDERROR!]
[!ENDIF!]
/** \brief Bit order of this CPU (generated)
 *
 * Platforms Setting:
 * The bit order of the CPU is set in the Base configuration 
 * by setting the parameter 'BaseCpuConfig_bitorder'. 
 *
 * Range:
 *  - LE (LSB_FIRST if parameter 'BaseCpuConfig_bitorder:LE')
 *  - BE (MSB_FIRST if parameter 'BaseCpuConfig_bitorder:BE')
 */
#define CPU_BIT_ORDER [!WS!][!"$CpuBitorder"!]

/* !LINKSTO Base.ASR403.PLATFORM039,1, Base.ASR403.PLATFORM046,1, Base.ASR403.PLATFORM050,1, Base.ASR403.PLATFORM051,1 */
#if (defined CPU_BYTE_ORDER)
#error CPU_BYTE_ORDER already defined
#endif

[!IF "node:exists(as:modconf('Base')[1]/BaseCpuConfig/BaseCpuConfig_byteorder)"!]
  [!VAR "CpuByteorderProperty" = "as:modconf('Base')[1]/BaseCpuConfig/BaseCpuConfig_byteorder"!]
  [!IF "$CpuByteorderProperty = 'LE'"!]
    [!VAR "CpuByteorder" = "'LOW_BYTE_FIRST'"!]
  [!ELSEIF "$CpuByteorderProperty = 'BE'"!][!//
    [!VAR "CpuByteorder" = "'HIGH_BYTE_FIRST'"!]
  [!ELSE!][!//
    [!ERROR!]
      Base: Invalid value for configuration parameter 'BaseCpuConfig_byteorder': '[!"$CpuByteorderProperty"!]' (must be 'LE' or 'BE')
    [!ENDERROR!]
  [!ENDIF!]
[!ELSE!][!//
  [!ERROR!]
      Base: Configuration parameter 'BaseCpuConfig_byteorder' specifying the byte order of the CPU not found.
  [!ENDERROR!]
[!ENDIF!]
/** \brief Byte order of this CPU (generated)
 *
 * Platforms Setting:
 * The byte order of the CPU is set in the Base configuration 
 * by setting the parameter 'BaseCpuConfig_byteorder'. 
 *
 * Range:
 *  - BE (HIGH_BYTE_FIRST if parameter 'BaseCpuConfig_byteorder:BE')
 *  - LE (LOW_BYTE_FIRST  if parameter 'BaseCpuConfig_byteorder:LE')
 */
#define CPU_BYTE_ORDER [!WS!][!"$CpuByteorder"!]

/* !LINKSTO Base.Dbg.ConfigMacro,1 */
#if (defined BASE_DBG_ENABLE)
#error BASE_DBG_ENABLE already defined
#endif

/** \brief Macro for enabling debug header file (generated)
 *
 * Range:
 *  - STD_ON (if configuration parameter 'Base/BaseDbg/BaseDbgHeaderFile' enabled)
 *  - STD_OFF (if configuration parameter 'Base/BaseDbg/BaseDbgHeaderFile' disabled)
 */
#define BASE_DBG_ENABLE [!WS!][!IF "node:exists(as:modconf('Base')[1]/BaseDbg/BaseDbgHeaderFile)='true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* !LINKSTO Base.Properties.OverrideMemCpy,1 */
#if (defined TS_MEMCPY_CUSTOM_OVERRIDE)
#error TS_MEMCPY_CUSTOM_OVERRIDE already defined
#endif

/** \brief Macro for enabling overriding of the memory copy function by a custom implementation
 *
 * Range:
 *  - STD_ON (if configuration parameter 'Base/CustomOverrides/CustomOverride_MemCpy' enabled)
 *  - STD_OFF (if configuration parameter 'Base/CustomOverrides/CustomOverride_MemCpy' disabled)
 */
#define TS_MEMCPY_CUSTOM_OVERRIDE [!WS!][!IF "as:modconf('Base')[1]/CustomOverrides/CustomOverride_MemCpy='true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* !LINKSTO Base.Properties.OverrideMemSet,1 */
#if (defined TS_MEMSET_CUSTOM_OVERRIDE)
#error TS_MEMSET_CUSTOM_OVERRIDE already defined
#endif

/** \brief Macro for enabling overriding of the memory set function by a custom implementation
 *
 * Range:
 *  - STD_ON (if configuration parameter 'Base/CustomOverrides/CustomOverride_MemSet' enabled)
 *  - STD_OFF (if configuration parameter 'Base/CustomOverrides/CustomOverride_MemSet' disabled)
 */
#define TS_MEMSET_CUSTOM_OVERRIDE [!WS!][!IF "as:modconf('Base')[1]/CustomOverrides/CustomOverride_MemSet='true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* !LINKSTO Base.Properties.OverrideMemCmp,1 */
#if (defined TS_MEMCMP_CUSTOM_OVERRIDE)
#error TS_MEMCMP_CUSTOM_OVERRIDE already defined
#endif

/** \brief Macro for enabling overriding of the memory compare function by a custom implementation
 *
 * Range:
 *  - STD_ON (if configuration parameter 'Base/CustomOverrides/CustomOverride_MemCmp' enabled)
 *  - STD_OFF (if configuration parameter 'Base/CustomOverrides/CustomOverride_MemCmp' disabled)
 */
#define TS_MEMCMP_CUSTOM_OVERRIDE [!WS!][!IF "as:modconf('Base')[1]/CustomOverrides/CustomOverride_MemCmp='true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* !LINKSTO Base.Properties.OverrideMemBZero,1 */
#if (defined TS_MEMBZERO_CUSTOM_OVERRIDE)
#error TS_MEMBZERO_CUSTOM_OVERRIDE already defined
#endif

/** \brief Macro for enabling overriding of the memory zeroing function by a custom implementation
 *
 * Range:
 *  - STD_ON (if configuration parameter 'Base/CustomOverrides/CustomOverride_MemBZero' enabled)
 *  - STD_OFF (if configuration parameter 'Base/CustomOverrides/CustomOverride_MemBZero' disabled)
 */
#define TS_MEMBZERO_CUSTOM_OVERRIDE [!WS!][!IF "as:modconf('Base')[1]/CustomOverrides/CustomOverride_MemBZero='true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/*==================[type definitions]=======================================*/
[!INCLUDE "TypeLists.m"!]
[!IF "text:contains(text:split($BitWidthList,','), '64')"!]

#ifndef HAS_64BIT_TYPES
#define HAS_64BIT_TYPES
#endif
[!ENDIF!]

/* !LINKSTO Base.ASR403.PLATFORM005,1, Base.ASR403.PLATFORM013,1, Base.ASR403.PLATFORM014,1, Base.ASR403.PLATFORM015,1, Base.ASR403.PLATFORM016,1, Base.ASR403.PLATFORM017,1, Base.ASR403.PLATFORM018,1, Base.ASR403.PLATFORM020,1, Base.ASR403.PLATFORM021,1, Base.ASR403.PLATFORM022,1, Base.ASR403.PLATFORM023,1, Base.ASR403.PLATFORM024,1, Base.ASR403.PLATFORM025,1, Base.ASR403.PLATFORM027,1, Base.ASR403.PLATFORM041,1, Base.ASR403.PLATFORM042,1, Base.ASR403.PLATFORM060,1, Base.ASR413.SWS_Platform_00066,1, Base.ASR413.SWS_Platform_00067,1 */
[!VAR "ListLength"="count(text:split($AutosarTypeList,','))"!]
[!FOR "i"="1" TO  "num:i($ListLength)"!]
  [!VAR "AutosarType"="text:split($AutosarTypeList,',')[num:i($i)]"!]
  [!VAR "property"="concat('BaseType_',$AutosarType)"!]
  [!IF "node:exists(as:modconf('Base')[1]/BaseTypes/*[node:name(.)=$property])"!]
/** \brief Type definition of AUTOSAR standard type [!"$AutosarType"!]
 *
 * Platforms Setting: 
 * Definition of platform specific AUTOSAR standard types
 * that are set in the Platforms plugin by parameter 'Basetypes.[!"$AutosarType"!].Mapping' 
 */
typedef[!WS!][!"as:modconf('Base')[1]/BaseTypes/*[node:name(.)=$property]"!][!WS!][!"$AutosarType"!];

  [!ENDIF!]
[!ENDFOR!]
[!IF "as:modconf('Base')[1]/BaseCpuConfig/BaseCpuConfig_type = '64' and text:contains(text:split($BitWidthList,','), '64')"!]
[!VAR "SizeType"="'uint64'"!]
[!VAR "SizeSuffix"="'ULL'"!]
[!ELSE!][!//
[!VAR "SizeType"="'uint32'"!]
[!VAR "SizeSuffix"="'UL'"!]
[!ENDIF!]
/* !LINKSTO Base.Types.SizeType,1 */
/** \brief Type definition of platform specific size type (generated, depending on parameter 'Cpu.Type') */
typedef[!WS!][!"$SizeType"!][!WS!]usize;

#ifndef USIZE_C
/** \brief Macro to define a constant of platform specific type usize (generated, depending on parameter 'Cpu.Type') */
/* Deviation MISRAC2012-1 <+2> */
#define USIZE_C(x) (x ## [!"$SizeSuffix"!])
#endif

/* !LINKSTO Base.Types.VoidPtr,1 */
/** \brief type definition for pointer to void */
typedef void * VoidPtr;
/** \brief type definition for pointer to const void */
typedef const void * ConstVoidPtr;

/*==================[inclusions]=============================================*/

/* !LINKSTO Base.ConfigScheme.PlatformTypesGeneration,1 */
/** \brief Inclusions of custom standard files (generated)
 *
 * For each configuration parameter 'Base/CustomStdIncludeFiles/[file]',
 * an include statement is generated
 *
 * Platforms Setting: 
 * Custom standard file can be used for definition of
 * additional standard types that are customer specific.
 */
[!LOOP "as:modconf('Base')[1]/CustomStdIncludeFiles/*"!]
#include <[!"."!]>
[!ENDLOOP!]

#endif /* ifndef PLATFORM_TYPES_H */
