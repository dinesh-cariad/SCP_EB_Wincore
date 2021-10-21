/* Os_types_arch.h
 *
 * Architecture-dependent types are included from the appropriate ARCH-types.h
 * include file depending on the chosen architecture.
 *
 * The Makefiles must ensure that the OS_ARCH and OS_CPU macros are
 * defined appropriately on the command line.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_types_arch.h 30593 2019-07-30 14:10:36Z beoe851 $
*/
#ifndef OS_TYPES_ARCH_H
#define OS_TYPES_ARCH_H

#include <Os_defs.h>

#if (OS_ARCH==OS_PA)
#include <PA/Os_types_PA.h>
#elif (OS_ARCH==OS_TRICORE)
#include <TRICORE/Os_types_TRICORE.h>
#elif (OS_ARCH==OS_RH850)
#include <RH850/Os_types_RH850.h>
#elif (OS_ARCH==OS_NEWARCH)
#include <NEWARCH/Os_types_NEWARCH.h>
#elif (OS_ARCH==OS_PIKEOS)
#include <PIKEOS/Os_types_PIKEOS.h>
#elif (OS_ARCH==OS_WINDOWS)
#include <WINDOWS/Os_types_WINDOWS.h>
#elif (OS_ARCH==OS_ARM)
#include <ARM/Os_types_ARM.h>
#elif (OS_ARCH==OS_ARM64)
#include <ARM64/Os_types_ARM64.h>
#elif (OS_ARCH==OS_LINUX)
#include <LINUX/Os_types_LINUX.h>
#elif (OS_ARCH==OS_CORTEXM)
#include <CORTEXM/Os_types_CORTEXM.h>
#elif (OS_ARCH==OS_ARC)
#include <ARC/Os_types_ARC.h>
#else
#error "Unsupported OS_ARCH defined. Check your Makefiles!"
#endif

/* Os types (non-microkernel variant) */

/*!
 * os_tick_t
 *
 * Tick counter - only if not already defined
*/
#ifndef OS_ARCH_TICK_T
#ifndef OS_ASM
typedef os_uint32_t os_tick_t;
#endif
#define OS_MAXTICK		OS_U(0xffffffff)
#define OS_SIZEOF_TICK	4
#endif

/*!
 * os_alarmbase_t
 *
 * Alarmbase structure for GetAlarmBase
*/
#ifndef OS_ASM
typedef struct os_alarmbase_s os_alarmbase_t;

struct os_alarmbase_s
{
	os_tick_t	maxallowedvalue;
	os_tick_t	ticksperbase;
	os_tick_t	mincycle;
};
#endif

#endif
/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
