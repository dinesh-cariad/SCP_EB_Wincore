/* lib-gettaskmaxruntime.c
 *
 * This file contains the OS_GetTaskMaxRuntime function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-gettaskmaxruntime.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#include <Os_kernel.h>
#include <Os_acct_api.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_GetTaskMaxRuntime
 *
 * This library function returns the longest observed run-time for a task.
 * It can be called by any trusted application. On some architectures
 * it might be callable from non-trusted applications too.
 *
 * If the task ID is out of range, or if the task does not have runtime
 * measurement enabled, E_OS_ID is returned. Otherwise, the task's
 * longest observed runtime is placed in the out location and E_OK is returned.
 * The error handler ia not called, so no error hook is called.
 *
 * !LINKSTO Kernel.Feature.OsAccounting, 1,
 * !        Kernel.Feature.OsAccounting.API.GetTaskMaxRuntime, 1
 * !doctype SRC
*/

os_result_t OS_GetTaskMaxRuntime(os_taskid_t t, os_tick_t *out)
{
	os_result_t r = OS_E_ID;
	const os_task_t *ts;
	os_taskaccounting_t *ta;

	if ( t < OS_nTasks )
	{
		ts = &OS_taskTableBase[t];
		ta = OS_GET_ACCT(ts->accounting);

		if ( (ts->flags & OS_TF_MEASUREEXEC) != 0 )
		{
			*out = ta->etMax;
			r = OS_E_OK;
		}
	}

	return r;
}

#include <memmap/Os_mm_code_end.h>

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_GetTaskMaxRuntime</name>
  <synopsis>Get longest observed runtime of a task</synopsis>
  <syntax>
    os_result_t OS_GetTaskMaxRuntime
    (   os_taskid_t t   /@ ID of task @/
        os_tick_t *out  /@ Where to put the answer @/
    )
  </syntax>
  <description>
    <code>OS_GetTaskMaxRuntime()</code> places the longest observed execution time of the
    specified task into the variable referenced by 'out'. If the task ID is invalid or the
    task does not have execution-time measurement enabled (attribute MEASURE_MAX_RUNTIME),
    <code>OS_GetTaskMaxRuntime()</code> returns <code>OS_E_ID</code>.
  </description>
  <availability>
    Available from all trusted tasks, ISRs and hook functions. One some architectures, it
    might be possible to call this function from non-trusted contexts as well.
  </availability>
  <returns>OS_E_OK=Success.</returns>
  <returns>OS_E_ID=Invalid Task ID specified.</returns>
  <returns>OS_E_ID=Task does not have the feature enabled.</returns>
</api>

 * CHECK: PARSE
*/

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/

