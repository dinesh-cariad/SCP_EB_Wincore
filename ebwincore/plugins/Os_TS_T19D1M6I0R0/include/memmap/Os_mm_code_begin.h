/* Os_mm_code_begin.h
 *
 * Implements the Autosar MemMap specification, but only if
 * support is enabled by defining the macro OS_MEMMAP with the value 1
 *
 * This file starts the code section
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 * CHECK: RULE 303 OFF (These files won't work if anti-recursion is implemented)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_mm_code_begin.h 27282 2017-08-24 13:57:43Z olme8414 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: D4.10 (required)
 * Precautions shall be taken in order to prevent the contents of a header
 * file being included more than once.
 *
 * Reason:
 * The concept of AUTOSAR memory mapping requires the content of memory
 * mapping headers to be included multiple times in a file.
 *
 *
 * MISRAC2012-2) Deviated Rule: 20.5 (advisory)
 * #undef shall not be used.
 *
 * Reason:
 * The concept of AUTOSAR memory mapping requires the use of #undef.
*/
/* Deviation MISRAC2012-1 <*> */
#ifdef OS_MM_CHECK
#ifndef OS_MM_CHECK_H
#include <memmap/Os_mm_check.h>
#endif
#ifdef OS_MM_NESTING
#error "Incorrect nesting of MemMap header files"
#endif
#define OS_MM_NESTING	OS_MM_CODE
#endif

#if defined(OS_ASM)
#define OS_START_SEC_CODE_ASM
#else
#define OS_START_SEC_CODE
#endif

#if defined(OS_MEMMAP) && (OS_MEMMAP==1)
#include <MemMap.h>
#else
#if defined(OS_ASM)
#include <memmap/Os_MemMap.i>
#else
/* Deviation MISRAC2012-2 */
#undef OS_START_SEC_CODE
#endif
#endif
/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
