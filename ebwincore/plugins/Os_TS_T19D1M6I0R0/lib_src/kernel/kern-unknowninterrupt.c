/* kern-unknowninterrupt.c
 *
 * This file contains the OS_UnknownInterrupt function
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-unknowninterrupt.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#define OS_SID	OS_SID_TrapHandler
#define OS_SIF	OS_svc_TrapHandler

#include <Os_kernel.h>
#include <Os_tool.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_UnknownInterrupt
 *
 * This function is called when an unconfigured interrupt occurs
*/
os_uint8_t OS_UnknownInterrupt(os_isrid_t iid)
{
	os_uint8_t inKernel;
	OS_PARAM_UNUSED(iid);

	inKernel = OS_GetKernelData()->inKernel;
	/* can't propagate the return value of OS_ERROR -> ignore it */
	(void) OS_ERROR(OS_ERROR_UnknownInterrupt, OS_NULL);

	return inKernel;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
