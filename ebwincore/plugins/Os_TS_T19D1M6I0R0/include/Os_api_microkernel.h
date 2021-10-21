/* Os_api_microkernel.h
 *
 * This file defines the OS User API in terms of the underlying microkernel.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_api_microkernel.h 23286 2015-12-22 12:05:22Z tojo2507 $
*/
#ifndef OS_API_MICROKERNEL_H
#define OS_API_MICROKERNEL_H

/* This file will eventually define all the OSEK and Autosar APIs that must
 * never be called from applications with high safety integrity levels
*/

#include <Os_defs.h>

#if OS_KERNEL_TYPE != OS_MICROKERNEL
#error "Os_api_microkernel.h should only be used in a microkernel environment"
#endif

#include <Os_types.h>
#include <Os_mklib.h>

/* Map transaction support macro to microkernel's definition.
 *
 * The OS defines OS_HAS_TRANSACTION_SUPPORT in terms of MK_HAS_TRANSACTION_SUPPORT if available,
 * otherwise it uses MK_HAS_ATOMIC_MONITOR_SUPPORT for compatibility with earlier versions of the MK.
 *
 * !LINKSTO Kernel.Feature.TransactionSupport, 1
*/
#ifdef MK_HAS_TRANSACTION_SUPPORT
#define OS_HAS_TRANSACTION_SUPPORT		MK_HAS_TRANSACTION_SUPPORT
#else
#define OS_HAS_TRANSACTION_SUPPORT		MK_HAS_ATOMIC_MONITOR_SUPPORT
#endif

#endif
/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
