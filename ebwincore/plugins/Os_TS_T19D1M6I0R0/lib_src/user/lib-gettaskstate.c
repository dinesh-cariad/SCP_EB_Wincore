/* lib-gettaskstate.c
 *
 * This file contains the OS_GetTaskState function.
 * The OS task state is translated into the equivalent OSEK/VDX state
 * and stored where indicated.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: lib-gettaskstate.c 27282 2017-08-24 13:57:43Z olme8414 $
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
 * 
 * MISRAC2012-3) Deviated Rule: 8.9 (advisory)
 * An object should be defined at block scope if its identifier only appears
 * in a single function.
 *
 * Reason:
 * Readability. Defining and initialising a large constant array inside a small
 * function obscures the purpose of the function.
 *
 * MISRAC2012-4) Deviated Rule: 8.3 (required)
 * All declarations of an object or function shall use the same names and
 * type qualifiers.
 *
 * Reason:
 * In this file the type os_result_t must be used, because the AUTOSAR-defined
 * type StatusType is not declared at this point. The implementation ensures,
 * that they are compatible.
*/

#include <Os_api.h>

/* This array contains entries for each valid OS task state. The
 * entry contains the equivalent OSEK/VDX state.
*/
#include <memmap/Os_mm_const_begin.h>
/* Deviation MISRAC2012-3 <1> */
static const TaskStateType OS_taskStateToOsek[OS_TS_NSTATES] =
{	SUSPENDED,		/* OS_TS_SUSPENDED		*/
	SUSPENDED,		/* OS_TS_QUARANTINED	*/
	READY,			/* OS_TS_NEW			*/
	READY,			/* OS_TS_READY_SYNC		*/
	READY,			/* OS_TS_READY_ASYNC	*/
	RUNNING,		/* OS_TS_RUNNING		*/
	WAITING,		/* OS_TS_WAITING		*/
};
#include <memmap/Os_mm_const_end.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_GetTaskState implements the API GetTaskState
 *
 * The system call OS_UserGetTaskState is called. This places
 * the state of the specified task in the caller-supplied variable, or
 * returns an error code. If all is OK this routine translates the return
 * value into the more limited Autosar/OSEK form. In any case the
 * return value from OS_UserGetTaskState() is returned.
 *
 * !LINKSTO Kernel.API.TaskManagement.GetTaskState.API, 1
*/
/* Deviation MISRAC2012-4 <1> */
os_result_t OS_GetTaskState(os_taskid_t t, os_taskstate_t *sr)
{
	os_result_t r;

	/* Deviation MISRAC2012-1, MISRAC2012-2 <1> */
	r = OS_UserGetTaskState(t, sr);

	if ( r == OS_E_OK )
	{
		if ( *sr >= OS_TS_NSTATES )
		{
			/* UNREACHABLE? - Should never get here if the kernel is behaving itself
			*/
			r = OS_E_UNKNOWN;
		}
		else
		{
			/* Translate the OS task state to OSEK/VDX value and store.
			*/
			*sr = OS_taskStateToOsek[*sr];
		}
	}

	return r;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_GetTaskState</name>
  <synopsis>Get state of a task</synopsis>
  <syntax>
    os_result_t OS_GetTaskState
    (   os_taskid_t t       /@ ID of task @/
        os_taskstate_t *sr  /@ Where to put the answer @/
    )
  </syntax>
  <description>
    <code>OS_GetTaskState()</code> places the current state of the
    specified task into the 'sr' variable. If the task ID is invalid,
    <code>OS_GetTaskState()</code> returns <code>OS_E_ID</code>.
    Otherwise, the task's state is translated into one of the standard
    OSEK/VDX states and stored into the 'sr' variable.
  </description>
  <availability>
    No restrictions.
  </availability>
  <returns>OS_E_OK=Success.</returns>
  <returns>OS_E_ID=Invalid Task ID specified.</returns>
  <returns>OS_E_UNKNOWN=The task's state is invalid.</returns>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/

