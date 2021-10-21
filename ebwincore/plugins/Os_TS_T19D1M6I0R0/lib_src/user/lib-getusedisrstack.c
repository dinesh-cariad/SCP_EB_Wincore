/* lib-getusedisrstack.c
 *
 * This file contains the OS_GetUsedIsrStack function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: lib-getusedisrstack.c 27282 2017-08-24 13:57:43Z olme8414 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 12.3 (advisory)
 * The comma operator should not be used.
 *
 * Reason:
 * For function-call kernels, OS API is wrapped in macros. The macro wrappers
 * need to use the comma operator to form an expression evaluating to the
 * correct result.
 *
 *
 * MISRAC2012-2) Deviated Rule: 13.3 (advisory)
 * A full expression containing an increment (++) or decrement (--) operator
 * should have no other potential side effects other than that caused by the
 * increment or decrement operator.
 *
 * Reason:
 * For function-call kernels, OS API is wrapped in macros. The macro wrappers
 * need to handle an "in kernel" flag so that the increment operator has to be
 * used in combination with the comma operator.
*/

#include <Os_api.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_GetUsedIsrStack implements the API getUsedIsrStack
 *
 * The system call OS_UserGetStackInfo is called, first setting the
 * info structure's current SP to 0. The answer is then computed.
 * If anything goes wrong, 0 is returned.
 *
 * !LINKSTO Kernel.Feature.StackCheck.API.getUsedIsrStack, 1
*/
os_size_t OS_GetUsedIsrStack(void)
{
	os_size_t answer = 0;
	os_stackinfo_t info;

	info.stackPointer = 0;

	/* Deviation MISRAC2012-1, MISRAC2012-2 <1> */
	if ( OS_UserGetStackInfo(OS_IsrToTOI(OS_NULLISR), &info) == OS_E_OK )
	{
		answer = info.stackLen - info.stackClean;
	}

	return answer;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_GetUsedIsrStack</name>
  <synopsis>Get the amount of interrupt stack that has been used</synopsis>
  <syntax>
    os_size_t OS_GetUsedIsrStack(void)
  </syntax>
  <description>
    <code>OS_GetUsedIsrStack</code> returns the amount of interrupt stack
    that has been overwritten. At startup, all stacks are filled with a fill
    pattern. The amount of interrupt stack that still contains the fill
    pattern is counted and subtracted from the total amount.
  </description>
  <availability>
    <code>OS_GetUsedIsrStack</code> can be used from Tasks and ISRs.
  </availability>
  <returns>=The number of bytes of interrupt stack used.</returns>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
