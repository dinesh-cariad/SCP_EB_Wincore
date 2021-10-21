/* kern-quarantineapplication.c
 *
 * This file contains the OS_QuarantineApplication function
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-quarantineapplication.c 27282 2017-08-24 13:57:43Z olme8414 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: SetButNeverUsed
 *   Dead assignment.
 *
 * Reason: Not an issue, assignment is effective depending on OS error handling level.
 */

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_QuarantineApplication terminates the specified application.
 *
 * Terminating an application means:
 *  - disabling all ISRs
 *  - cancelling all alarms
 *  - stopping all schedule tables
 *  - killing all tasks (including the caller, if it is a task)
 *
 * Objects that are terminated are set to QUARANTINED to prevent reactivation.
 *
 * NOTE: this function does nothing about physically killing any ISRs that may be active.
 * The ISRs are marked as killed, but the error handler and the interrupt entry functions
 * are responsible for the actual killing because there may be unaffected ISRs also on the
 * stack.
 *
 * !LINKSTO Kernel.Autosar.OsApplication.Termination, 1
 * !LINKSTO Kernel.Autosar.Protection.ProtectionHook.KillApplication, 2
*/
void OS_QuarantineApplication(const os_appcontext_t *app)
{
	os_taskid_t taskid;
	os_isrid_t isrid;
	os_alarmid_t alarmid;
	os_scheduleid_t scheduleid;
	const os_isr_t *isr;
	const os_task_t *task;
	const os_alarm_t *alarm;
	const os_schedule_t *schedule;

	/* Disable all interrupts belonging to this application
	*/
	for ( isrid = 0; isrid < OS_nInterrupts; isrid++ )
	{
		isr = &OS_isrTableBase[isrid];
		if ( OS_GET_APP(isr->app) == app )
		{
			OS_DisableIsr(isr);
			OS_MarkIsrKilled(isr->dynamic);
			isr->dynamic->statusflags |= OS_ISRF_BLOCKED;
		}
	}

	/* Cancel all alarms belonging to this application.
	 * Only true OSEK alarms are considered. Others (such as
	 * those belonging to schedule tables) will get cancelled
	 * when the object to which they belong gets cancelled.
	*/
	/* Possible diagnostic TOOLDIAG-1 <1> */
	alarm = OS_alarmTableBase;
	for ( alarmid = 0; alarmid < OS_nAlarms; alarmid++ )
	{
		if ( OS_GET_APP(alarm->app) == app )
		{
			(*OS_killAlarmFunc)(alarmid, OS_ALARM_QUARANTINED);
		}
		/* Possible diagnostic TOOLDIAG-1 <2> */
		alarm++;
	}

	/* Stop all schedule tables belonging to this application.
	*/
	/* Possible diagnostic TOOLDIAG-1 <1> */
	schedule = OS_scheduleTableBase;
	for ( scheduleid = 0; scheduleid < OS_nSchedules; scheduleid++ )
	{
		if ( OS_GET_APP(schedule->app) == app )
		{
			(*OS_killScheduleFunc)(scheduleid, OS_ST_QUARANTINED);
		}
		/* Possible diagnostic TOOLDIAG-1 <2> */
		schedule++;
	}

	/* Kill all tasks that belong to this application. We
	 * set the state to QUARANTINED to prevent other applications
	 * from activating the tasks later. OS_RestartApplication()
	 * will set the states back to SUSPENDED if it is called.
	*/
	task = OS_taskTableBase;
	for ( taskid = 0; taskid < OS_nTasks; taskid++ )
	{
		if ( OS_GET_APP(task->app) == app )
		{
			(*OS_killTaskFunc)(task, OS_TS_QUARANTINED);
		}
		task++;
	}

	/* Set the application state to QUARANTINED
	*/
	app->dynamic->appstate = OS_APP_QUARANTINED;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
