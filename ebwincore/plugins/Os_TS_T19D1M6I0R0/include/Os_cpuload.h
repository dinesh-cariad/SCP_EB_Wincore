/* Os_cpuload.h
 *
 * Defines macros, data types and function prototypes for measuring CPU load (user-visible part)
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_cpuload.h 27282 2017-08-24 13:57:43Z olme8414 $
*/
#ifndef OS_CPULOAD_H
#define OS_CPULOAD_H

#ifndef OS_ASM

#include <Os_types.h>

#if (OS_KERNEL_TYPE==OS_FUNCTION_CALL)

#include <Os_userkernel.h>

#define OS_UserGetCpuLoad(coreId, getMax)	OS_CALL(os_uint8_t, OS_KernGetCpuLoad(coreId, getMax))
#define OS_UserResetPeakCpuLoad(coreId)		OS_VOIDCALL(OS_KernResetPeakCpuLoad((coreId)))

#else /* OS_KERNEL_TYPE */

/* Userland functions in case it's necessary to call the CPU load API from a non-trusted
 * application. The system-call overhead is counted as part of the load.
 *
 * !LINKSTO Kernel.Feature.CpuLoadMeasurement.API.GetMaxCpuLoad, 1
*/
extern os_uint8_t OS_UserGetCpuLoad(os_coreid_t, os_boolean_t);
extern void OS_UserResetPeakCpuLoad(os_coreid_t);

#endif /* OS_KERNEL_TYPE */

/* Kernel functions. These are mapped to the SALSA API via Os_salsa.h.
*/
extern os_uint8_t OS_KernGetCpuLoad(os_coreid_t, os_boolean_t);
extern void OS_KernResetPeakCpuLoad(os_coreid_t);

#define OS_GetCpuLoad(coreId, getMax)	OS_KernGetCpuLoad((coreId), (getMax))
#define OS_ResetPeakCpuLoad(coreId)		OS_KernResetPeakCpuLoad((coreId))

#endif /* OS_ASM */

#endif /* OS_CPULOAD_H */

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
