/* kern-waitevent.c
 *
 * This file contains the OS_KernWaitEvent function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-waitevent.c 27926 2017-11-21 17:19:25Z mist9353 $
*/

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 13.5 (required)
 * The right-hand operand of a logical && or || operator shall not contain side effects.
 *
 * Reason:
 * The rate monitoring function has to make modifications to the global system state.
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: PointlessComparison
 *   Condition is always true.
 *
 * Reason: This warning depends on OS configuration and library optimization.
 *   If OS_ERROR is OS_Error it could call the protection hook, which could change the task's state.
 *   Also the result of OS_ERROR could be E_OK, if the hook decides, that the task may continue.
 *
 * TOOLDIAG-2) Possible diagnostic: PointlessComparison
 *   Condition is always true.
 *
 * Reason: This warning depends on OS configuration and library optimization.
 *   If timing protection is included (no OS_EXCLUDE_TIMINGPROTECTION), OS_STOPTASKEXECTIMING could
 *   change the task's state. Therefore this check is necessary.
 *
 * TOOLDIAG-3) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   The statements can be reached, if interrupt enable checks are enabled.
 *
 * TOOLDIAG-4) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   The statements can be reached, if rate monitoring is enabled.
*/

#define OS_SID	OS_SID_WaitEvent
#define OS_SIF	OS_svc_WaitEvent

#include <Os_kernel.h>
#include <Os_taskqueue.h>

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_KernWaitEvent
 *
 * If any of the specified events is already set for the current task, this
 * function simply returns. Otherwise the task is dequeued and goes
 * to the WAITING state.
 *
 * !LINKSTO Kernel.API.EventControl.WaitEvent.API, 1
 *
 * !LINKSTO Kernel.API.EventControl.WaitEvent.Scheduling, 1
 *		Happens automatically in the system-call handler.
 *
 * !LINKSTO Kernel.API.EventControl.WaitEvent.InternalResource, 1
 *		Happens automatically because of the way internal resources are
 *		implemented. SetEvent sets the priority to queuePrio when the task
 *		leaves the waiting state.
*/
os_result_t OS_KernWaitEvent(os_eventmask_t e)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	os_intstatus_t is;
	const os_task_t *ts;
	os_taskdynamic_t *td;
	os_result_t r = OS_E_OK;
	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0,(os_paramtype_t)e);

	OS_TRACE_WAITEVENT_ENTRY(e);

	if ( kernel_data->inFunction == OS_INTASK )
	{
		ts = kernel_data->taskCurrent;

		if ( (ts->flags & OS_TF_EXTENDED) != 0 )
		{
			td = ts->dynamic;

			if ( !OS_InterruptEnableCheckFromTask(OS_IEC_OSEKEXTRA, td) )
			{
				/* Possible diagnostic TOOLDIAG-3 <+1> */
				r = OS_ERROR(OS_ERROR_InterruptDisabled, OS_NULL);
			}
			else
			if ( td->lastLock != OS_NULLOBJECT )
			{
				/* !LINKSTO Kernel.API.EventControl.WaitEvent.ExternalResource, 1
				*/
				if (OS_GetTypeFromObjectId(td->lastLock) == OS_OBJ_SPINLOCK)
				{
					/* !LINKSTO Kernel.Autosar.Multicore.WaitEvent.Spinlock, 1
					*/
					r = OS_ERROR(OS_ERROR_HoldsLock, OS_GET_PARAMETER_VAR());
				}
				else
				{
					r = OS_ERROR(OS_ERROR_HoldsResource, OS_GET_PARAMETER_VAR());
				}
			}
			else
			{
				is = OS_IntDisable();

				/* Ensure task hasn't been killed by a timeout
				*/
				if ( td->state == OS_TS_RUNNING )
				{
					if ( e == 0 )
					{
						/* Waiting for no events. Without a special case, this
						 * would end up waiting forever, which is probably a
						 * programming error in the application. However, the
						 * specifications are silent about what should happen.
						 * Options:
						 * - wait forever
						 * - simply return with no error
						 * - return an error code
						 * - treat as application error (STD: kill app, EXT: error code)
						 * - others ???
						 * At the moment, we call the error handler. If it
						 * returns OK we wait. If not, we return the error code
						 * to the caller.
						*/
						r = OS_ERROR(OS_ERROR_NoEvents, OS_GET_PARAMETER_VAR());
					}

					if ( r != OS_E_OK )
					{
						/* e == 0 and the error handler says "return error".
						*/
					}
					else
					{
						/* !LINKSTO Kernel.API.EventControl.WaitEvent.StatusChange, 1
						 *
						 * Stop the execution timer. This could kill the task!
						*/
						OS_STOPTASKEXECTIMING(ts);

						/* Ensure task hasn't been killed by a timeout
						*/
						/* Possible diagnostic TOOLDIAG-2 <1> */
						if ( td->state == OS_TS_RUNNING )
						{
							if ( (td->pEvents & e) == 0 )
							{
								/* Call post-task hook before task goes to WAITING state.
								 * Interrupts are already disabled here.
								 *
								 * !LINKSTO Kernel.API.Hooks.PostTaskHook.API, 1
								 * !LINKSTO Kernel.HookRoutines.PrioISRC2, 1
								 *		Interrupts are disabled here
								*/
								OS_CALLPOSTTASKHOOK();

#if (OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE)
								/* If the run priority of the task is different from the base priority
								 * we need to dequeue the task from the runPrio queue too.
								*/
								if ( ts->runPrio != ts->queuePrio)
								{
									OS_DequeueTask(ts, ts->runPrio);
								}
								OS_DequeueTask(ts, ts->queuePrio);
#else
								/* Dequeue the task. We always use the first link slot
								 * because extended tasks cannot have multiple
								 * activations
								*/
								OS_Dequeue(ts->linkBase);
#endif

								td->wEvents = e;

								OS_TRACE_STATE_TASK( ts->taskId, td->state, OS_TS_WAITING );
								td->state = OS_TS_WAITING;
							}
							else
							{
								/* One or more of the events is already pending.
								 * Check rate monitor before returning to the task.
								 *
								 * !LINKSTO Kernel.Autosar.Protection.TimingProtection.RateLimit.Tasks, 2
								*/
								/* Deviation MISRAC2012-1 */
								if ( OS_RATEMONITORCHECK(ts->rateMonitor) )
								{
									/* Possible diagnostic TOOLDIAG-4 <+1> */
									r = OS_ERROR(OS_ERROR_RateLimitExceeded, OS_GET_PARAMETER_VAR());

									/* Do we need to kill the task? This happens if the ProtectionHook
									 * returns PRO_IGNORE.
									 *
									 * note: semantics of PRO_IGNORE are not well-defined!
									*/
									/* Possible diagnostic TOOLDIAG-1 <1> */
									if ( (r != OS_E_OK) && (td->state == OS_TS_RUNNING) )
									{
										OS_CALLPOSTTASKHOOK();
#if (OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE)
										/* If the run priority of the task is different from the base priority
										 * we need to dequeue the task from the runPrio queue too.
										*/
										if ( ts->runPrio != ts->queuePrio)
										{
											OS_DequeueTask(ts, ts->runPrio);
										}
										OS_DequeueTask(ts, ts->queuePrio);
#else
										OS_Dequeue(ts->linkBase);
#endif

										OS_TRACE_STATE_TASK( ts->taskId, td->state, OS_TS_QUARANTINED );
										td->state = OS_TS_QUARANTINED;
									}
								}
								else
								{
									/*
									 * We need to restart the execution timing here because we'll most likely
									 * just return to the task and not enter the dispatcher.
									 *
									 * !LINKSTO Kernel.Autosar.Protection.TimingProtection.ExecutionTime.Measurement.Task, 1
									*/
									OS_InitTaskEtb(ts);
									OS_STARTTASKEXECTIMING(ts);
								}
							}
						}
					}
				}

				OS_IntRestore(is);
			}
		}
		else
		{
			/* !LINKSTO Kernel.API.EventControl.WaitEvent.BasicTask, 1
			*/
			r = OS_ERROR(OS_ERROR_TaskNotExtended, OS_GET_PARAMETER_VAR());
		}
	}
	else
	{
		/* !LINKSTO Kernel.API.EventControl.WaitEvent.ISRC2, 1
		*/
		r = OS_ERROR(OS_ERROR_WrongContext, OS_GET_PARAMETER_VAR());
	}

	OS_TRACE_WAITEVENT_EXIT_P(r,e);
	return r;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserWaitEvent</name>
  <synopsis>Wait for one of a set of events</synopsis>
  <syntax>
    os_result_t OS_UserWaitEvent
    (   os_eventmask_t e
    )
  </syntax>
  <description>
    <code>OS_UserWaitEvent()</code> causes the calling task to wait until
    one or more of the events specified in the <code>e</code> parameter
    occurs. If an event is already pending, the function returns
    immediately. Otherwise, the task enters the <emphasis>waiting</emphasis> state
    until one of the events occurs.
    <para>
    Calling <code>OS_UserWaitEvent()</code> with an empty set of events
    is considered to be an error and handled accordingly.
    </para>
  </description>
  <availability>
    <code>OS_UserWaitEvent()</code> may only be called from an extended
    task.
  </availability>
  <returns>OS_E_OK=Success</returns>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
