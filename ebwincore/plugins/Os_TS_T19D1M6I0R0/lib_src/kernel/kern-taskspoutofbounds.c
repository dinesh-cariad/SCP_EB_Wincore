/* kern-taskspoutofbounds.c
 *
 * This file contains the OS_TaskSpOutOfBounds function. It returns TRUE if the
 * task's SP lies outside the permitted boundary.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-taskspoutofbounds.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/* OS_TaskSpOutOfBounds
 *
 * This function returns TRUE if the task's stack pointer lies outside the
 * permitted boundaries.
 *
 * The code here assumes that (in the case of STACKGROWSDOWN at least) the
 * stack model is "decrement before store", so that a stack pointer that is
 * pointing just outside the stack at the top is perfectly OK.
 *
 * If the stack pointer provided is OS_NULL we return FALSE (==OK) because
 * on some architectures a stack-pointer might not be available sometimes.
 * (see TRICORE)
*/
os_boolean_t OS_TaskSpOutOfBounds(os_address_t sp, const os_task_t *tp)
{
	os_address_t base;

	base = (os_address_t)tp->stackBase;

	return ( sp != 0 ) && ( (sp < base) || (sp > (base + tp->stackLen)) );
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
