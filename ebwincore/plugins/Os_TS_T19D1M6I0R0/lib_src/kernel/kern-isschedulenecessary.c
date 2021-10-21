/* kern-isschedulenecessary.c
 *
 * This file contains the OS_IsScheduleNecessary function
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-isschedulenecessary.c 29483 2018-11-19 10:12:44Z dh $
*/
/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: UnusedVariable
 *   Variable is set but never used.
 *
 * Reason: Not an issue, variable will be used if multiple task activations are enabled.
 */
#include <Os_kernel.h>
#include <Os_api.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_IsScheduleNecessary implements the API Schedule
 *
 * Returns TRUE is there is no current task or there is a task of higher priority enqueued behind the current task.
 * Otherwise returns FALSE.
 *
 * !LINKSTO Kernel.Feature.FastSchedule, 1
 * !LINKSTO Kernel.Feature.FastSchedule.ErrorChecking, 1
 * !LINKSTO Kernel.Feature.FastSchedule.IsScheduleNecessary, 1
*/
os_boolean_t OS_IsScheduleNecessary(void)
{
	os_boolean_t answer = OS_TRUE;

#if (OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE)
	/* Improve: for the moment, the answer is always TRUE
	*/
#else
	os_tasklink_t * const taskActivations = OS_GetTaskActivations();
	os_task_t const * const current_task = OS_GetKernelData()->taskCurrent;
	
	if ( current_task != OS_NULL )
	{
		/* Possible diagnostic TOOLDIAG-1 <1> */
		os_taskdynamic_t * const td = current_task->dynamic;
		const os_tasklink_t link = OS_CurrentLink(current_task, td);
		const os_tasklink_t next = taskActivations[link];

		answer = ( (next != 0) && (current_task->queuePrio < OS_FindPrio(next)) );
	} /* else not needed */
#endif

	return answer;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_IsScheduleNecessary</name>
  <synopsis>Determine whether a call to Schedule() is necessary</synopsis>
  <syntax>
    os_boolean_t OS_IsScheduleNecessary(void)
  </syntax>
  <description>
    <code>OS_IsScheduleNecessary()</code> returns TRUE (non-zero) if there is
    no current task or another task in the task queue with a higher configured
    priority than the current task. Otherwise it returns FALSE.
  </description>
  <availability>
    <code>OS_IsScheduleNecessary()</code> should only be called from a task. If it is called from
    another context and there is a current task, it will return information about that task. If
    there is no current task it will return true, Schedule() gets called and reports context error.
    <code>OS_IsScheduleNecessary()</code> can only be called from tasks that have read access to kernel
    variables. On most systems this will be true, but in SC3 and SC4 memory protection might prevent
    access if so configured and will detect a memory protection error in the calling task.
  </availability>
  <returns>Zero=A call to schedule is not necessary</returns>
  <returns>Non-zero=A call to schedule is necessary</returns>
</api>

<api API="OS_USER">
  <name>OS_ScheduleIfNecessary</name>
  <synopsis>Call Schedule() if necessary</synopsis>
  <syntax>
    os_result_t OS_ScheduleIfNecessary(void)
  </syntax>
  <description>
    <code>OS_ScheduleIfNecessary()</code> calls <code>OS_IsScheduleNecessary()</code> and
    if it returns TRUE, calls <code>Schedule()</code> and returns the result. Otherwise
    E_OS_OK is returned.
  </description>
  <availability>
    <code>OS_ScheduleIfNecessary()</code> should only be called from a task. The conditions and restrictions
    for <code>OS_IsScheduleNecessary()</code> apply here as well.
  </availability>
  <returns>E_OS_OK=Success, or Schedule() was not called.</returns>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
