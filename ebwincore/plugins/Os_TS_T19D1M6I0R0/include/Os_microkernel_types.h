/* Os_microkernel_types.h
 *
 * This file maps OS data types onto microkernel types
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_microkernel_types.h 27527 2017-09-21 14:32:06Z thob2262 $
*/
#ifndef OS_MICROKERNEL_TYPES_H
#define OS_MICROKERNEL_TYPES_H

#include <Os_defs.h>
#include <public/Mk_defs.h>

/* This file only gets included when the OS is being compiled to run under the microkernel
 * This is a sanity check.
*/
#if OS_KERNEL_TYPE != OS_MICROKERNEL
#error "Os_microkernel_types.h should only be included when using the microkernel."
#endif

#include <public/Mk_public_types.h>
#include <public/Mk_autosar.h>

/* map to microkernel definition */
#define OS_ARCH_HAS_64BIT MK_HAS_INT64

/* this is only used with Windows to avoid a compiler warning */
#define OS_ARCH_HAS_CHAR_T 0

/* endianess */
#if MK_HWENDIAN==MK_LITTLEENDIAN
#define OS_ENDIAN OS_LITTLEENDIAN
#elif MK_HWENDIAN==MK_BIGENDIAN
#define OS_ENDIAN OS_BIGENDIAN
#else
#error "MK_HWENDIAN value out of range."
#endif

/* ARM: instruction set */
#if (OS_ARCH == OS_ARM)
/* Currently, only ARMv7 CPUs are supported by the microkernel. */
#define OS_ARM_INSTR_SET OS_ARM_INSTR_SET_ARMV7
#endif

/* Define all the types that the OS expects to see in terms of microkernel data types
*/
#ifndef OS_ASM
typedef mk_int8_t os_int8_t;
typedef mk_int16_t os_int16_t;
typedef mk_int32_t os_int32_t;
typedef mk_uint8_t os_uint8_t;
typedef mk_uint16_t os_uint16_t;
typedef mk_uint32_t os_uint32_t;
#if MK_HAS_INT64
typedef mk_uint64_t os_uint64_t;
#endif
typedef mk_address_t os_address_t;
typedef mk_size_t os_size_t;
typedef mk_parametertype_t os_intstatus_t;
typedef mk_parametertype_t os_paramtype_t;
typedef mk_stackelement_t os_stackelement_t;
typedef mk_uint8_t os_errorresult_t;

typedef struct os_appcontext_s os_appcontext_t;
typedef os_uint32_t os_clzword_t;
typedef unsigned int os_msr_t;
#endif

#ifndef OS_NULL
#define OS_NULL	MK_NULL
#endif

/*!
 * os_tick_t
 *
 * Tick counter - only if not already defined
*/
#ifndef OS_ASM
typedef TickType os_tick_t;
#endif
#define OS_MAXTICK		0xffffffff
#define OS_SIZEOF_TICK	4

/*!
 * os_physicaltime_t
 *
 * Use the same type for the physical time type, that was used for os_tick_t.
*/
#define OS_ARCH_PHYSICALTIME_T	1
#ifndef OS_ASM
typedef TickType os_physicaltime_t;
#endif

/*!
 * os_alarmbase_t
 *
 * Alarmbase structure for GetAlarmBase
*/
#ifndef OS_ASM
typedef AlarmBaseType os_alarmbase_t;
#endif

/*
 * os_timestamp_t
 *
 * Representation of the timestamp (tick) value.
*/
#ifndef OS_ASM
typedef mk_time_t os_timestamp_t;
#endif

/*
 * os_service_t
 *
 * Service function identifier
*/
#ifndef OS_ASM
typedef mk_serviceid_t os_serviceid_t;
#endif

#endif
/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
