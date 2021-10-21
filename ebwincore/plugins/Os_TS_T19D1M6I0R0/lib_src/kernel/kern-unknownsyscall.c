/* kern-unknownsyscall.c
 *
 * This file contains the OS_UnknownSystemCall function
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-unknownsyscall.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#define OS_SID	OS_SID_UnknownSyscall
#define OS_SIF	OS_svc_UnknownSyscall

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_KernUnknownSyscall
 *
 * This function is called when an unknown or unimplemented system call
 * is made. Its address is in entry 0 and all unused entries in the
 * system-call table
*/
os_result_t OS_KernUnknownSyscall(void)
{
	return OS_ERROR(OS_ERROR_UnknownSystemCall, OS_NULL);
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
