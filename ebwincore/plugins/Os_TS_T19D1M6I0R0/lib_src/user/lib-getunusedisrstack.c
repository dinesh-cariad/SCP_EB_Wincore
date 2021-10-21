/* lib-getunusedisrstack.c
 *
 * This file contains the OS_GetUnusedIsrStack function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: lib-getunusedisrstack.c 27282 2017-08-24 13:57:43Z olme8414 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 12.3 (required)
 * The comma operator shall not be used.
 *
 * Reason:
 * For function-call kernels, OS API is wrapped in macros. The macro wrappers
 * need to use the comma operator to form an expression evaluating to the
 * correct result.
 *
 *
 * MISRAC2012-2) Deviated Rule: 13.3 (advisory)
 * The increment (++) and decrement (--) operators should not be mixed with
 * other operators in an expression.
 *
 * Reason:
 * For function-call kernels, OS API is wrapped in macros. The macro wrappers
 * need to handle an "in kernel" flag so that the increment operator has to be
 * used in combination with the comma operator.
*/

#include <Os_api.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_GetUnusedIsrStack implements the API getUnusedIsrStack
 *
 * The system call OS_UserGetStackInfo is called, first setting the
 * info structure's current SP to 0. The answer is then computed.
 * If anything goes wrong, 0 is returned.
 *
 * !LINKSTO Kernel.Feature.StackCheck.API.getUnusedIsrStack, 1
*/

os_size_t OS_GetUnusedIsrStack(void)
{
	os_size_t answer = 0;
	os_stackinfo_t info;

	info.stackPointer = 0;

	/* Deviation MISRAC2012-1, MISRAC2012-2 <1> */
	if ( OS_UserGetStackInfo(OS_IsrToTOI(OS_NULLISR), &info) == OS_E_OK )
	{
		answer = info.stackClean;
	}

	return answer;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_GetUnusedIsrStack</name>
  <synopsis>Get the amount of interrupt stack that remains unused</synopsis>
  <syntax>
    os_size_t OS_GetUnusedIsrStack(void)
  </syntax>
  <description>
    <code>OS_GetUnusedIsrStack</code> returns the amount of interrupt stack
    that has not been overwritten. At startup, all stacks are filled with a
    fill pattern. The amount of interrupt stack that still contains the fill
    pattern is counted.
  </description>
  <availability>
    <code>OS_GetUnusedIsrStack</code> can be used from Tasks and ISRs.
  </availability>
  <returns>=The number of bytes of stack that remain unused.</returns>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/

