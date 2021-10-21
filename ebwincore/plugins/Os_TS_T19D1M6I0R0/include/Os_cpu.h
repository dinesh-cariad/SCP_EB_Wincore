/* Os_cpu.h
 *
 * This file provides characteristics of the current derivative's cpu.
 * It includes the target specific Os_*_cpu.h file.
 *
 * This file is used by the IOC to determine the number of cores provided by the hardware.
 *
 * $Id: Os_cpu.h 30593 2019-07-30 14:10:36Z beoe851 $
*/
#ifndef OS_CPU_H
#define OS_CPU_H

#include <Os_defs.h>

#if (OS_ARCH==OS_PA)
#include <PA/Os_PA_cpu.h>
#elif (OS_ARCH==OS_TRICORE)
#include <TRICORE/Os_TRICORE_cpu.h>
#elif (OS_ARCH==OS_RH850)
#include <RH850/Os_RH850_cpu.h>
#elif (OS_ARCH==OS_NEWARCH)
#include <NEWARCH/Os_NEWARCH_cpu.h>
#elif (OS_ARCH==OS_PIKEOS)
#include <PIKEOS/Os_PIKEOS_cpu.h>
#elif (OS_ARCH==OS_WINDOWS)
#include <WINDOWS/Os_WINDOWS_cpu.h>
#elif (OS_ARCH==OS_ARM)
#include <ARM/Os_ARM_cpu.h>
#elif (OS_ARCH==OS_ARM64)
#include <ARM64/Os_ARM64_cpu.h>
#elif (OS_ARCH==OS_LINUX)
#include <LINUX/Os_LINUX_cpu.h>
#elif (OS_ARCH==OS_CORTEXM)
#include <CORTEXM/Os_CORTEXM_cpu.h>
#elif (OS_ARCH==OS_ARC)
#include <ARC/Os_ARC_cpu.h>
#else
#error "Unsupported OS_ARCH defined. Check your Makefiles!"
#endif

/* OS_N_CORES_MAX - The number of cores provided by the hardware.
*/
#ifndef OS_N_CORES_MAX
#error "OS_N_CORES_MAX is undefined."
#endif


#endif
/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
