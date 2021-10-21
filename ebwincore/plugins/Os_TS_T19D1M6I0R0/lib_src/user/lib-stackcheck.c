/* lib-stackcheck.c
 *
 * This file contains the OS_StackCheck function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: lib-stackcheck.c 27282 2017-08-24 13:57:43Z olme8414 $
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
#include <Os_kernel.h>		/* For OS_GetCurrentSp() */

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_StackCheck implements the API stackCheck
 *
 * The system call OS_UserGetStackInfo is called, first putting the
 * current SP into the info structure. This will be overwritten if we are
 * in an ISR context, because in that case calling the function uses some
 * stack.
 *
 * !LINKSTO Kernel.Feature.StackCheck.API.stackCheck, 1
*/
os_int_t OS_StackCheck(void)
{
	os_int_t answer = 0;
	os_stackinfo_t info;

	info.stackPointer = (os_stackinfoptr_t)OS_GetCurrentSp();

	/* Deviation MISRAC2012-1, MISRAC2012-2 <1> */
	if ( OS_UserGetStackInfo(OS_TOI_CURRENTCONTEXT, &info) == OS_E_OK )
	{
		answer = info.stackStatus;
	}

	return answer;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_StackCheck</name>
  <synopsis>Check current stack use</synopsis>
  <syntax>
    os_size_t OS_StackCheck(void)
  </syntax>

  <description>
    <code>OS_StackCheck</code> checks the stack use in the current context. If
    If there is or has been a stack overflow, <code>OS_StackCheck</code>
    returns +1.
    If there is a stack underflow, <code>OS_StackCheck</code> returns -1.
    Otherwise <code>OS_StackCheck</code> returns 0.
  </description>
  <availability>
    Can be used from all tasks and ISRs.
  </availability>
  <returns>0=OK</returns>
  <returns>+1=Stack overflow</returns>
  <returns>-1=Stack underflow</returns>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
