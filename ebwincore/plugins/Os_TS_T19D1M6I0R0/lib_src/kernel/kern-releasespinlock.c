/* kern-releasespinlock.c
 *
 * This file contains the function OS_KernReleaseSpinlock().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-releasespinlock.c 28130 2018-01-12 14:54:58Z olme8414 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-2) Deviated Rule: 11.5 (advisory)
 * A conversion should not be performed from pointer to void into pointer to
 * object.
 *
 * Reason:
 * Some architectures require access to hardware, and this involves this kind
 * of cast, because the hardware has no notion of C language types.
 *
 * MISRAC2012-3) Deviated Rule: 11.6 (required)
 * A cast shall not be performed between pointer to void and an arithmetic type.
 *
 * Reason:
 * Some architectures require access to hardware, and this involves this kind
 * of cast, because the hardware has no notion of C language types.
 */
/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   The statements can be reached, if ISRs are allowed to take resources.
*/

/* The SID and service info must be defined before including any headers because the
 * optimised error handler file needs the definition to translate internal error codes
 * into OSEK/Autosar error status codes
*/
#define OS_SID OS_SID_ReleaseSpinlock
#define OS_SIF OS_svc_ReleaseSpinlock

#include <Os_kernel.h>
#include <Os_spinlock.h>

/* Include definitions for tracing */
#include <Os_trace.h>

static void OS_ReleaseSpinlockResource(os_taskdynamic_t *, os_prio_t, os_intstatus_t);

static void OS_ReleaseSpinlockResource(os_taskdynamic_t *td, os_prio_t lastPrio, os_intstatus_t is)
{
	if (td != OS_NULL)
	{
		os_prio_t currprio = td->prio;

		if (currprio == lastPrio)
		{
			/* No priority change.
			*/
			OS_IntRestore(is);
		}
		else
		{
			/* Lower the current task's priority.
			*/
			OS_LowerCurrentTasksPriority(lastPrio);

			/* If the priority of the resource being released is an
			 * interrupt priority, we must set the interrupt level
			 * even if the priority being restored is a task priority.
			 * In the latter case we must set the interrupt level to 0.
			*/
			if ( OS_IsIsrPrio(currprio) )
			{
				/* This sets the current interrupt level, so we must
				 * not restore the saved interrupt status afterwards.
				*/
				/* Possible diagnostic TOOLDIAG-1 <+1> */
				if ( !OS_IsIsrPrio(lastPrio) )
				{
					/* Possible diagnostic TOOLDIAG-1 <+1> */
					lastPrio = OS_NULLISRPRIO;
				}
				/* Deviation MISRAC2012-2, MISRAC2012-3 <1> */
				OS_SetIsrResourceLevel(lastPrio);
				OS_IntRestoreHardLock(is);
			}
			else
			{
				/* Interrupt status is restored in this branch.
				*/
				OS_IntRestore(is);
			}
		}
	}
	else
	{
		/* Deviation MISRAC2012-2, MISRAC2012-3 <1> */
		OS_SetIsrResourceLevel(lastPrio);
		OS_IntRestoreHardLock(is);
	}
}


/*!
 * OS_KernReleaseSpinlock()
 *
 * Releases the AUTOSAR spinlock 'lockId'.
 * See [SWS_Os_00695] in "AUTOSAR V5.3.0, R4.1 Rev 3" for further details.
 *
 * !LINKSTO Kernel.Autosar.API.SystemServices.ReleaseSpinlock, 2
 */
os_result_t OS_KernReleaseSpinlock(os_autosarspinlockid_t lockId)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	const os_autosarspinlock_t *lockStatic;
	const os_appcontext_t *app;
	os_objectid_t *lastLockPtr = OS_NULL;
	os_objecttype_t myType = OS_OBJ_NONE;
	os_objectid_t myId = OS_NULLOBJECT;
	os_result_t result = OS_E_OK;
	os_intstatus_t is;
	os_taskdynamic_t *td = OS_NULL;
	os_prio_t lastPrio;

	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0, (os_paramtype_t)lockId);

	OS_TRACE_RELEASESPINLOCK_ENTRY(lockId);

	if ( kernel_data->inFunction == OS_INTASK )
	{
		myType = OS_OBJ_TASK;
		myId = kernel_data->taskCurrent->taskId;
		td = kernel_data->taskCurrent->dynamic;
		lastLockPtr = &td->lastLock;
	}
	else
	if ( kernel_data->inFunction == OS_INCAT2 )
	{
		myType = OS_OBJ_ISR;
		myId = kernel_data->isrCurrent;
		lastLockPtr = &kernel_data->isrLastLock;
	}
	else
	{
		/* !LINKSTO Kernel.Autosar.API.SystemServices.ReleaseSpinlock.CallingContext, 1
		*/
		result = OS_ERROR(OS_ERROR_WrongContext, OS_GET_PARAMETER_VAR());
	}

	if ( lastLockPtr == OS_NULL )
	{
		/* Drop out: failed the calling context check. */
	}
	else
	if ( !OS_IsValidAutosarSpinlock(lockId) )
	{
		/* !LINKSTO Kernel.Autosar.API.SystemServices.ReleaseSpinlock.InvalidId, 1
		*/
		result = OS_ERROR(OS_ERROR_InvalidSpinlockId, OS_GET_PARAMETER_VAR());
	}
	else
	{
		lockStatic = &OS_autosarSpinlockTableBase[lockId];

		if ( *lastLockPtr != OS_CreateObjectId(lockId, OS_OBJ_SPINLOCK) )
		{
			/* Error handling: this wasn't the lock we were looking for...
			 * ...so let's try to find out which error case we've encountered. */
			app = OS_CurrentApp();

			if ( !OS_HasPermission(app, lockStatic->accessPermissions) )
			{
				/* !LINKSTO Kernel.Autosar.API.SystemServices.ReleaseSpinlock.Access, 1
				*/
				result = OS_ERROR(OS_ERROR_Permission, OS_GET_PARAMETER_VAR());
			}
			else
			if ( (lockStatic->dynamic->ownerType != myType) || (lockStatic->dynamic->ownerId != myId) )
			{
				/* !LINKSTO Kernel.Autosar.API.SystemServices.ReleaseSpinlock.NotOccupied, 1
				*/
				result = OS_ERROR(OS_ERROR_SpinlockNotOccupied, OS_GET_PARAMETER_VAR());
			}
			else
			if ( OS_GetTypeFromObjectId(*lastLockPtr) == OS_OBJ_RESOURCE )
			{
				/* !LINKSTO Kernel.Autosar.API.SystemServices.ReleaseSpinlock.Resources, 1
				*/
				result = OS_ERROR(OS_ERROR_HoldsResource, OS_GET_PARAMETER_VAR());
			}
			else
			{
				/* !LINKSTO Kernel.Autosar.API.SystemServices.ReleaseSpinlock.Nesting, 1
				*/
				result = OS_ERROR(OS_ERROR_InvalidSpinlockNesting, OS_GET_PARAMETER_VAR());
			}
		}
		else
		{
			OS_TRACE_RELEASESPINLOCK_BEFORE_KERNELLOCK();
			is = OS_TakeInternalLock(OS_SOFTWARELOCK_LOCKID);
			OS_TRACE_RELEASESPINLOCK_IN_KERNELLOCK();

			if ((td != OS_NULL) && (td->state != OS_TS_RUNNING))
			{
				/* Task has been killed, e.g. by overrunning execution budget.
				 * In this case there's nothing to do, because the locks have already been released.
				 * If the return value of this function becomes visible
				 * to some task, this is an internal error.
				*/
				OS_DropInternalLock(OS_SOFTWARELOCK_LOCKID, is);
				result = OS_E_INTERNAL;
			}
			else
			{
				*lastLockPtr = lockStatic->dynamic->predecessor;
				lockStatic->dynamic->ownerCore = OS_CORE_ID_INVALID;
				lockStatic->dynamic->ownerType = OS_OBJ_NONE;
				lockStatic->dynamic->ownerId = OS_NULLOBJECT;
				lockStatic->dynamic->predecessor = OS_NULLOBJECT;
				lastPrio = lockStatic->dynamic->lastPrio;
				lockStatic->dynamic->lastPrio = 0;
				OS_CacheFlush(lockStatic->dynamic, sizeof(os_autosarspinlockdyn_t));

				OS_DropInternalSpinlockOnly(OS_SOFTWARELOCK_LOCKID);
				/* !LINKSTO Kernel.Autosar.API.SystemServices.ReleaseSpinlock.LockMethod, 1
				*/
				OS_ReleaseSpinlockResource(td, lastPrio, is);
			}

		}
	}

	OS_TRACE_RELEASESPINLOCK_EXIT_P(lockId);

	return result;
}

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
