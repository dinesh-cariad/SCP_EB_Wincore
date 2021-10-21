/* Os_mklib.h
 *
 * This file provides prototypes of the library functions that are available
 * in a microkernel environment.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_mklib.h 25795 2017-01-30 16:03:59Z mist9353 $
*/
#ifndef OS_MKLIB_H
#define OS_MKLIB_H

#include <Os_defs.h>

#if (OS_KERNEL_TYPE!=OS_MICROKERNEL)
#error "Os_mklib.h should only be used in a microkernel environment"
#endif

#include <Os_types.h>

/* !LINKSTO Kernel.Autosar.API.SystemServices.GetNumberOfActivatedCores, 1
*/
#define GetNumberOfActivatedCores() OS_N_CORES

/* AUTOSAR 4.0 renamed GetElapsedCounterValue - we provide both names
*/
#define GetElapsedValue GetElapsedCounterValue


#ifndef OS_ASM
os_result_t OS_GetAlarmBase(os_alarmid_t, os_alarmbase_t*);
#endif


#endif
/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
