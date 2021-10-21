/* Os_configuration_decls.h
 *
 * This file provides configuration dependent declarations
 * and macro definitions.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_configuration_decls.h 27282 2017-08-24 13:57:43Z olme8414 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 20.10 (advisory)
 * The # and ## preprocessor operators should not be used.
 *
 * Reason:
 * The ## operator is used here so we don't have to add eight initialization list elements
 * to each core look-up table. This avoids copy paste mistakes and makes the code more overseeable,
 * while still supporting up to eight cores.
 *
 * MISRAC2012-2) Deviated Rule: 20.7 (required)
 * Expressions resulting from the expansion of macro parameters shall be enclosed
 * in parentheses.
 *
 * Reason:
 * False positive. This macro parameter is used as operand of ##.
 *
 * MISRAC2012-3) Deviated Rule: 20.11 (required)
 * A macro parameter immediately following a # operator shall not immediately be
 * followed by a ## operator.
 *
 * Reason:
 * The order in which the ## operators are evaluated is not an issue here.
 * Moreover the macro doesn't use # operators, so operator precedence neither is an issue.
*/

#ifndef OS_CONFIGURATION_DECLS_H
#define OS_CONFIGURATION_DECLS_H

#include <Os_kernel.h>
#include <Os_configuration.h>


/* Preprocessor utility
*/
/*!
 * OS_COREARRAY is used to initialize core look-up tables.
*/
#if OS_N_CORES_MAX == 1u
#define OS_COREARRAY(c0, c1, c2, c3, c4, c5, c6, c7)	(c0)
#elif OS_N_CORES_MAX == 2u
#define OS_COREARRAY(c0, c1, c2, c3, c4, c5, c6, c7)	(c0), (c1)
#elif OS_N_CORES_MAX == 3u
#define OS_COREARRAY(c0, c1, c2, c3, c4, c5, c6, c7)	(c0), (c1), (c2)
#elif OS_N_CORES_MAX == 4u
#define OS_COREARRAY(c0, c1, c2, c3, c4, c5, c6, c7)	(c0), (c1), (c2), (c3)
#elif OS_N_CORES_MAX == 5u
#define OS_COREARRAY(c0, c1, c2, c3, c4, c5, c6, c7)	(c0), (c1), (c2), (c3), (c4)
#elif OS_N_CORES_MAX == 6u
#define OS_COREARRAY(c0, c1, c2, c3, c4, c5, c6, c7)	(c0), (c1), (c2), (c3), (c4), (c5)
#elif OS_N_CORES_MAX == 7u
#define OS_COREARRAY(c0, c1, c2, c3, c4, c5, c6, c7)	(c0), (c1), (c2), (c3), (c4), (c5), (c6)
#elif OS_N_CORES_MAX == 8u
#define OS_COREARRAY(c0, c1, c2, c3, c4, c5, c6, c7)	(c0), (c1), (c2), (c3), (c4), (c5), (c6), (c7)
#else
#error "Microcontrollers with more than 8 cores are currently not supported."
#endif

/*!
 * OS_MAKE_COREARRAY_FROM is used to initialize core look-up tables, by appending each core's index
 * to the given name argument.
*/
/* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-3 <+3> */
#define OS_MAKE_COREARRAY_FROM(name)\
	OS_COREARRAY(name##0, name##1, name##2, name##3, \
					name##4, name##5, name##6, name##7)


/* Deduced configuration constants.
*/

#if (OS_KERNEL_TYPE != OS_MICROKERNEL)

/*!
 * Configuration for CPU load measurement
 *
 * We define a default if the generator has not already defined one.
 * The default defines a dummy configuration that is internally consistent, in case any function
 * gets called (avoiding possible divide-by-zero problems, null-pointer accesses etc.)
 * Naturally, calling the APIs with the feature unconfigured will result in garbage answers.
 * The system-call version of the API is turned off.
*/
#if	(defined(OS_CPULOADCFG_NINTERVALS)) && (defined(OS_CPULOADCFG_INTERVAL)) \
	&& (defined(OS_CPULOADCFG_WINDOW)) && (defined(OS_CPULOADCFG_ROUNDING))

#define OS_CPULOADCFG_BUSYOVERFLOWLIMIT	(OS_MAXTICK - OS_CPULOADCFG_ROUNDING)/(OS_CPULOADCFG_WINDOW)
#define OS_CPULOADCFG_WINDOW100			((OS_CPULOADCFG_WINDOW + 50)/100)
#define OS_CPULOADCFG_ROUNDING100		((OS_CPULOADCFG_ROUNDING + 50)/100)

#else

#define OS_CPULOADCFG_INTERVAL			1
#define OS_CPULOADCFG_NINTERVALS		1
#define OS_CPULOADCFG_WINDOW			1
#define OS_CPULOADCFG_ROUNDING			0
#define OS_CPULOADCFG_BUSYOVERFLOWLIMIT	OS_MAXTICK
#define OS_CPULOADCFG_WINDOW100			1
#define OS_CPULOADCFG_ROUNDING100		0

#define OS_K_GETCPULOAD					&OS_KernUnknownSyscall
#define OS_K_RESETPEAKCPULOAD			&OS_KernUnknownSyscall

#endif /* defined(OS_CPULOADCFG_NINTERVALS)) && (defined(OS_CPULOADCFG_INTERVAL)) \
		*	&& (defined(OS_CPULOADCFG_WINDOW)) && (defined(OS_CPULOADCFG_ROUNDING) */

/* Declare symbols used by core local configurations.
*/
#if (OS_NTASKS != 0)
#ifndef OS_ASM
extern const os_task_t OS_taskTable[OS_NTASKS];
#endif /* ASM */
#endif /* OS_NTASKS != 0 */

#endif /* OS_KERNEL_TYPE != OS_MICROKERNEL */

/* Process the configurations of the different cores.
 * (Must be done at the end - include cycle)
*/
#include <Os_configuration_decls_c0.h>
#include <Os_configuration_decls_c1.h>
#include <Os_configuration_decls_c2.h>
#include <Os_configuration_decls_c3.h>
#include <Os_configuration_decls_c4.h>
#include <Os_configuration_decls_c5.h>
#include <Os_configuration_decls_c6.h>
#include <Os_configuration_decls_c7.h>
#if (OS_N_CORES_MAX > 8u)
#error "More than 8 cores are currently not supported."
#endif


#endif
/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
