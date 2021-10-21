/* Os_mm_check.h
 *
 * Some defines to help check that the MemMap support in the kernel
 * is properly nested (viz not at all)
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_mm_check.h 23286 2015-12-22 12:05:22Z tojo2507 $
*/
#ifndef OS_MM_CHECK_H

#define OS_MM_CHECK_H	0
#define OS_MM_CODE		1
#define OS_MM_CONST16	2
#define OS_MM_CONST32	3
#define OS_MM_CONST8	4
#define OS_MM_CONST		5
#define OS_MM_VAR32		6
#define OS_MM_VAR8		7
#define OS_MM_VAR		8

#endif
/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
