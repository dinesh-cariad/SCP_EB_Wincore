/* Os.h
 *
 * This file provides a defined interface from the kernel and from user
 * code to the generated include file.
 *
 * Note: when used on host systems with case-insensitive filenames, this
 * file will also get included when either os.h or Os.h is included. For host systems with
 * case-sensitive filenames, the directory containing os.h should be added to the
 * path when compiling legacy code that includes os.h. At the time of writing,
 * os.h can be found in the unix subdirectory of the OS-plugin's include directory.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os.h 27282 2017-08-24 13:57:43Z olme8414 $
 *
 * !LINKSTO Kernel.Autosar.HeaderFileStructure.Os_h, 1
*/

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 20.5 (advisory)
 * #undef shall not be used.
 *
 * Reason:
 * A macro is undefined to simplify matters for the build system and hence
 * for the end user.
 */

#ifndef OS_H
#define OS_H

#include <Os_defs.h>

#if OS_KERNEL_TYPE == OS_MICROKERNEL

/* The types CounterType and TickType are defined in microkernel headers
 * and must not be re-defined by the RTE. To avoid this, the RTE_TYPE_
 * macros are needed.
*/
#define RTE_TYPE_CounterType
#define RTE_TYPE_TickType

/* Include the microkernel's version of its subset of OSEK and Autosar.
*/
#include <MicroOs.h>

/* Include the board header to resolve the ns-to-ticks macros
*/
#include <Mk_qmboard.h>

/* Map the macros to the ones provided by the microkernel. We do this here,
 * because the macros are provided by the microkernel board files. As a result
 * the QM-OS library must not use the macros to stay configuration independent.
 */
#define OS_TimestampNsToTicks(ns)		MK_TimestampNsToTicks(ns)
#define OS_TimestampTicksToNs(ticks)	MK_TimestampTicksToNs(ticks)

/* Include the QM subset of the OSEK/VDX and Autosar API that is provided directly
 * by the OS and not by the microkernel.
*/
#include <Os_api_microkernel.h>

/* Include the OS configured object IDs
*/
#include <Os_user.h>

#else /* OS_KERNEL_TYPE == OS_MICROKERNEL */
/* Include the files specified by the SWS. This is only done if full
 * SWS conformance is required because the OS doesn't need them.
 * All the files included here are external to the OS; they are
 * not provided by the OS, and have therefore not been tested
 * by OS testing. Users of the OS should therefore be aware
 * that by defining OS_FULL_AUTOSAR_SWS_INCLUDE_CONFORMANCE
 * on the compiler command line might interfere with the operation
 * of the OS, depending on the contents of these files.
*/
#if defined(OS_FULL_AUTOSAR_SWS_INCLUDE_CONFORMANCE)
#include <Std_Types.h>
#include <Os_Cfg.h>
#include <MemMap.h>
#endif

/* Include the generated header
*/
#include <Os_user.h>

/* Include the board header to resolve the ns-to-ticks macros
*/
#include <board.h>

#endif /* OS_KERNEL_TYPE == OS_MICROKERNEL */

#include <Os_api.h>
#if (OS_KERNEL_TYPE != OS_MICROKERNEL)
#include <Os_api_cfg.h>
#endif

#include <Os_Version.h>

/* Include patch header file if indicated.
 * We set the macro OS_PATCH_USER to 1 to indicate to the included file that we
 * are patching the configuration.
 * Afterwards, we undefine the macro; this is a MISRA violation. The alternative
 * would be to have separate patch files and separate control macros for including
 * them, which would complicate matters for the build system and therefore for the user.
*/
#ifdef OS_INCLUDE_PATCHES

#define OS_PATCH_USER	1
#include <Os_config_patches.h>
/* Deviation MISRAC2012-1 */
#undef OS_PATCH_USER

#endif

#endif
/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
