/* Os_cpuload_kernel.h
 *
 * Defines data types and function prototypes for measuring CPU load (kernel implementation)
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_cpuload_kernel.h 27525 2017-09-21 14:28:38Z olme8414 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 8.11 (advisory)
 * When an array with external linkage is declared, its size should be
 * explicitly specified.
 *
 * Reason:
 * Array size and initialization are depending on configuration whereas the
 * array declaration is used by configuration-independent library code.
*/

/*
 * !LINKSTO Kernel.Feature.CpuLoadMeasurement.Execution, 1
*/

#ifndef OS_CPULOAD_KERNEL_H
#define OS_CPULOAD_KERNEL_H

#include <Os_kernel.h>
#include <Os_timestamp.h>
#include <Os_cpuload.h>

#ifdef OS_EXCLUDE_CPULOAD
/* Define dummy versions of all the kernel macros.
 * Ensure that the architecture hasn't defined any special versions of these macros.
*/
#if (defined OS_LEAVEIDLESTATE) || (defined OS_MEASURECPULOAD) || \
	(defined OS_CPULOAD_GENERICCAT1_ENTRY) || (defined OS_CPULOAD_GENERICCAT1_EXIT) || \
	(defined OS_CPULOAD_GENERICCAT2_ENTRY) || (defined OS_INITMEASURECPULOAD)
#error "Architecture defines a special CPU-load-measurement macro when OS_EXCLUDE_CPULOAD is defined!"
#endif

#define OS_LEAVEIDLESTATE()				do { } while(0)
#define OS_MEASURECPULOAD()				do { } while(0)
#define OS_CPULOAD_GENERICCAT1_ENTRY()	do { } while(0)
#define OS_CPULOAD_GENERICCAT1_EXIT()	do { } while(0)
#define OS_CPULOAD_GENERICCAT2_ENTRY()	do { } while(0)
#define OS_INITMEASURECPULOAD()			do { } while(0)

#endif

/* Data structures for measurement of CPU load and configuration of this feature.
*/
#ifndef OS_ASM
typedef struct os_cpuload_s os_cpuload_t;
typedef struct os_cpuloadcfg_s os_cpuloadcfg_t;

struct os_cpuload_s
{
	/* ATTENTION!!!
	 * The following members are referenced in assembly language code. Hence, changing their
	 * type, order, etc. must be accounted for in the byte-offset macros OS_CPULOAD_* below.
	 */

	os_timestamp_t idleExitTime;
	os_int_t busyNestingCounter;

	/* The following members are not referenced in assembly language code. */

	os_timestamp_t intervalStartTime;
	os_tick_t busyTime;
	os_tick_t busyTimeSum;
	os_tick_t *busyBuffer;			/* Address of the ring buffer used for storing the interval busy-times */
	os_int_t busyIndex;
	os_uint8_t currentLoad;
	os_uint8_t peakLoad;
};
#endif

/* Offsets of variables in the os_cpuload_s structure. On some architectures these will be used
 * in assembly-language.
 * The values here should be OK for all architectures because the size of the timestamp is
 * fixed at 64 bits. However, we define them conditionally just in case some architecture has
 * different characteristics.
 * BEWARE: the size of the busyNestingCounter is architecture-dependent.
*/
#ifndef OS_CPULOAD_IDLEEXITTIME
#define OS_CPULOAD_IDLEEXITTIME			OS_U(0)
#endif
#ifndef OS_CPULOAD_BUSYNESTINGCOUNTER
#define OS_CPULOAD_BUSYNESTINGCOUNTER	OS_U(8)
#endif

#ifndef OS_ASM
struct os_cpuloadcfg_s
{
	os_tick_t intervalDuration;		/* The duration of the measurement interval, in timer ticks */
	os_int_t nIntervals;			/* The number of measurement intervals in a measurement window */
	os_tick_t windowDuration;		/* The duration of the measurement window (intervalDuration*nIntervals) */
	os_tick_t rounding;				/* The rounding factor: either 0, windowDuration/2 or windowDuration-1 */
	os_tick_t busyOverflowLimit;	/* Value at which average computation overflows ==> use alternative calculation */
	os_tick_t windowDuration100;	/* windowDuration/100, used in alternative calculation */
	os_tick_t rounding100;			/* rounding/100, used in alternative calculation */
};
#endif

#ifndef OS_ASM
/*!
 * os_cpuloadmeasurementtype_t
 *
 * Enumerates the services offered by the cpu load measurement feature.
 */
enum os_cpuloadmeasurementtype_e
{
	OS_CPULOAD_GET_CURRENT = 0,
	OS_CPULOAD_GET_PEAK,
	OS_CPULOAD_RESET_PEAK
};
typedef enum os_cpuloadmeasurementtype_e os_cpuloadmeasurementtype_t;
#endif

#ifndef OS_ASM
extern const os_cpuloadcfg_t OS_cpuLoadCfg;
/* Deviation MISRAC2012-1 */
extern os_tick_t * const OS_cpuLoadBusyBuffer_ptr[];

/* OS_LeaveIdleState()
 *
 * For most architectures, this macro will be implemented in assembler code in the interrupt
 * handler(s), as close to the start as possible. The C implementation is provided here for
 * reference and test purposes, but can be used if the interrupt handler is written in C.
*/
#define OS_LeaveIdleState()		OS_GetTimeStamp(&OS_GetKernelData()->cpuLoad->idleExitTime)

/* Conditional definitions for the following macros. This permits them to be defined differently
 * in the optimised build.
 * LeaveIdleState is small, so it isn't worth testing a config flag. If CPU load is disabled
 * it will only get a timestamp on the first call anyway.
 * The configMode flag is tested for the full load measurement because the measurement is
 * quite a large chunk of code to execute inside an all-interrupts-locked section.
*/

#ifndef OS_LEAVEIDLESTATE
#define OS_LEAVEIDLESTATE() 										\
	do {															\
		os_cpuload_t * const cpuLoad = OS_GetKernelData()->cpuLoad;	\
		os_int_t cLbNCtmp = cpuLoad->busyNestingCounter;			\
		cpuLoad->busyNestingCounter++;								\
		if ( cLbNCtmp == 0 )										\
		{															\
			OS_LeaveIdleState();									\
		}															\
	} while(0)
#endif

#ifndef OS_MEASURECPULOAD
#define OS_MEASURECPULOAD()	\
	do {														\
		if ( (OS_configMode & OS_CPULOAD) != 0 )				\
		{														\
			OS_GetKernelData()->cpuLoad->busyNestingCounter = 0;\
			OS_MeasureCpuLoad();								\
		}														\
	} while(0)
#endif

/* The following macros define a generic implementation of the measurement functions
 * that can be used in OS_Cat1Entry, OS_Cat2Entry and OS_CatKEntry, with the known measurement
 * errors due to the architecture-defined latency.
 * If the architecture can perform this functionality in the outermost handlers it can define
 * these macros as empty. The Cat1 macros can also be defined as empty in the optimised kernel
 * when load measurement is not configured. The Cat2 version will be optimised away automatically.
 *
 * Notes:
 *  1. OS_CPULOAD_GENERICCAT2_ENTRY works for CatK as well.
 *  2. There is no EXIT macro for Cat2/CatK because they always leave via the Dispatcher
 *     when the idle loop has been interrupted.
*/
#ifndef OS_CPULOAD_GENERICCAT1_ENTRY
#define OS_CPULOAD_GENERICCAT1_ENTRY()	OS_LEAVEIDLESTATE()
#endif

#ifndef OS_CPULOAD_GENERICCAT1_EXIT
#define OS_CPULOAD_GENERICCAT1_EXIT() 									\
	do {																\
		os_cpuload_t * const cpuLoad = OS_GetKernelData()->cpuLoad; 	\
		cpuLoad->busyNestingCounter--;									\
		if ( cpuLoad->busyNestingCounter == 0 )							\
		{																\
			OS_MEASURECPULOAD();										\
		}																\
	} while (0)
#endif

#ifndef OS_CPULOAD_GENERICCAT2_ENTRY
#define OS_CPULOAD_GENERICCAT2_ENTRY()	OS_LEAVEIDLESTATE()
#endif

#ifndef OS_INITMEASURECPULOAD
#define OS_INITMEASURECPULOAD()			OS_InitMeasureCpuLoad()
#endif

/* Kernel functions
*/
extern void OS_InitMeasureCpuLoad(void);
extern void OS_MeasureCpuLoad(void);
extern os_uint8_t OS_MeasureCoreLoad(os_cpuloadmeasurementtype_t);

#endif /* OS_ASM */

#endif /* OS_CPULOAD_KERNEL_H */

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
