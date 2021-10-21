/* kern-dotrytogetspinlock.c
 *
 * This file contains the function OS_DoTryToGetSpinlock().
 * It provides the implementation for OS_KernTryToGetSpinlock().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-dotrytogetspinlock.c 27926 2017-11-21 17:19:25Z mist9353 $
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
#define OS_SID OS_SID_TryToGetSpinlock
#define OS_SIF OS_svc_TryToGetSpinlock

#include <Os_kernel.h>
#include <Os_spinlock.h>

/* Include definitions for tracing */
#include <Os_trace.h>

static os_prio_t OS_GetSpinlockPrio(os_spinlocklockmethod_t);
static void OS_GetSpinlockResource(os_taskdynamic_t *, os_prio_t, os_prio_t *, os_intstatus_t);


static os_prio_t OS_GetSpinlockPrio(os_spinlocklockmethod_t sllm)
{
	os_prio_t prio;

	switch (sllm)
	{
	case OS_SPINLOCKLM_LOCK_NOTHING:
		prio = 0;
		break;
	case OS_SPINLOCKLM_LOCK_WITH_RES_SCHEDULER:
		{
			/* The configuration has to make sure, that any task can be raised to the
			 * maximum priority, see Generator.Processing.ImplicitResScheduler.
			*/
			os_coreid_t const myCoreId = OS_GetMyCoreId();
			prio = OS_maxPrio[myCoreId];
		}
		break;
	case OS_SPINLOCKLM_LOCK_CAT2_INTERRUPTS:
		prio = OS_GetInterruptPriority(OS_intDisableLevel);
		break;
	default: /*OS_SPINLOCKLM_LOCK_ALL_INTERRUPTS*/
		prio = OS_GetInterruptPriority(OS_intDisableLevelAll);
		break;
	}
	return prio;
}

static void OS_GetSpinlockResource(os_taskdynamic_t *td, os_prio_t prio, os_prio_t *pLastPrio, os_intstatus_t is)
{
	os_prio_t curPrio;
	if (td != OS_NULL)
	{
		curPrio = td->prio;
		if (curPrio < prio)
		{
			OS_RaiseTaskPriority(td, prio);
		}
	}
	else
	{
		curPrio = OS_GetInterruptPriority(is);
	}
	*pLastPrio = curPrio;

	if ((curPrio < prio) && OS_IsIsrPrio(prio))
	{
		/* This sets the current interrupt level, so we must
		 * not restore the saved interrupt status afterwards.
		*/
		/* Possible diagnostic TOOLDIAG-1 <+2> */
		/* Deviation MISRAC2012-2, MISRAC2012-3 <1> */
		OS_SetIsrResourceLevel(prio);
		OS_IntRestoreHardLock(is);
	}
	else
	{
		/* The interrupt priority was already greater than the
		 * resource's priority, so we simply restore the
		 * original interrupt level
		*/
		OS_IntRestore(is);
	}
}


/* OS_DoTryToGetSpinlock()
 *
 * This is a helper function for OS_KernTryToGetSpinlock().
 * As GetSpinlock() is mapped to TryToGetSpinlock() in user space, this function
 * fulfills requirements for both.
 *
 * !LINKSTO Kernel.Autosar.API.SystemServices.GetSpinlock, 1
 * !LINKSTO Kernel.Autosar.API.SystemServices.TryToGetSpinlock, 1
 */
os_result_t OS_DoTryToGetSpinlock(os_autosarspinlockid_t lockId, os_tryspinlock_t *out)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	const os_autosarspinlock_t *lockStatic;
	const os_appcontext_t *app;
	os_objectid_t *lastLockPtr = OS_NULL;
	os_objectid_t lastLock;
	os_objectid_t lastSpinlock;
	os_autosarspinlockid_t findSpinlock;
	os_result_t result = OS_E_OK;
	os_objecttype_t myType = OS_OBJ_NONE;
	os_objectid_t myId = OS_NULLOBJECT;
	os_coreid_t currCore;
	os_objecttype_t currType;
	os_objectid_t currId;
	os_intstatus_t is;
	os_taskdynamic_t *td = OS_NULL;

	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0, (os_paramtype_t)lockId);
	OS_SAVE_PARAMETER_N(1, (os_paramtype_t)out);

	OS_TRACE_TRYTOGETSPINLOCK_ENTRY(lockId, out);

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
		/* !LINKSTO Kernel.Autosar.API.SystemServices.GetSpinlock.CallingContext, 1
		 * !LINKSTO Kernel.Autosar.API.SystemServices.TryToGetSpinlock.CallingContext, 1
		*/
		result = OS_ERROR(OS_ERROR_WrongContext, OS_GET_PARAMETER_VAR());
	}

	if ( lastLockPtr == OS_NULL )
	{
		/* Drop out: failed the calling context check */
	}
	else
	if ( out == OS_NULL )
	{
		result = OS_ERROR(OS_ERROR_WriteProtect, OS_GET_PARAMETER_VAR());
	}
	else
	if ( !OS_IsValidAutosarSpinlock(lockId) )
	{
		/* !LINKSTO Kernel.Autosar.API.SystemServices.GetSpinlock.InvalidId, 1
		 * !LINKSTO Kernel.Autosar.API.SystemServices.TryToGetSpinlock.InvalidId, 1
		*/
		result = OS_ERROR(OS_ERROR_InvalidSpinlockId, OS_GET_PARAMETER_VAR());
	}
	else
	{
		lockStatic = &OS_autosarSpinlockTableBase[lockId];

		app = OS_CurrentApp();

		if ( !OS_HasPermission(app, lockStatic->accessPermissions) )
		{
			/* !LINKSTO Kernel.Autosar.API.SystemServices.GetSpinlock.Access, 1
			 * !LINKSTO Kernel.Autosar.API.SystemServices.TryToGetSpinlock.Access, 1
			*/
			result = OS_ERROR(OS_ERROR_Permission, OS_GET_PARAMETER_VAR());
		}
		else
		{
			findSpinlock = lockId;
			lastLock = *lastLockPtr;
			lastSpinlock = lastLock;

			/* The last lock taken might be a resource, but they don't take part in the nesting checks,
			 * so we need to look back down the predecessor list to see if there's a spinlock already
			 * taken. This loop stops at the first non-resource.
			*/
			while (OS_GetTypeFromObjectId(lastSpinlock) == OS_OBJ_RESOURCE)
			{
				lastSpinlock = OS_ResourceGetDynamic(OS_GetIdFromObjectId(lastSpinlock))->next;
			}

			/* The above loop exits with lastSpinlock as the last spinlock taken (or OS_NULLOBJECT). */

			if (lastSpinlock != OS_NULLOBJECT)
			{
				/* Current object already owns a lock. Since nesting is allowed, the lock
				 * that's now wanted must be a (direct or indirect) successor of the lock that's held
				*/
				findSpinlock = (os_autosarspinlockid_t)OS_GetIdFromObjectId(lastSpinlock);

				do
				{
					findSpinlock = OS_autosarSpinlockTableBase[findSpinlock].successor;
				}
				while ( (findSpinlock != lockId) && (findSpinlock != OS_NULLAUTOSARSPINLOCK) );
			}

			if ( findSpinlock == OS_NULLAUTOSARSPINLOCK )
			{
				/* !LINKSTO Kernel.Autosar.API.SystemServices.GetSpinlock.Nesting, 1
				 * !LINKSTO Kernel.Autosar.API.SystemServices.TryToGetSpinlock.Nesting, 1
				*/
				result = OS_ERROR(OS_ERROR_InvalidSpinlockNesting, OS_GET_PARAMETER_VAR());
			}
			else
			{
				os_autosarspinlockdyn_t *lockDynamic = lockStatic->dynamic;
				os_prio_t prio = OS_GetSpinlockPrio(lockStatic->lockMethod);
				os_coreid_t myCore = OS_GetMyCoreId();

				OS_TRACE_TRYTOGETSPINLOCK_BEFORE_KERNELLOCK();
				is = OS_TakeInternalLock(OS_SOFTWARELOCK_LOCKID);
				OS_TRACE_TRYTOGETSPINLOCK_IN_KERNELLOCK();

				if ((td != OS_NULL) && (td->state != OS_TS_RUNNING))
				{
					/* Task has been killed, e.g. by overrunning execution budget.
					 * In this case we just return without doing anything.
					 * If the return value of this function becomes visible
					 * to some task, this is an internal error.
					*/
					OS_DropInternalLock(OS_SOFTWARELOCK_LOCKID, is);
					result = OS_E_INTERNAL;
				}
				else
				{
					OS_CacheInvalidate(lockDynamic, sizeof(os_autosarspinlockdyn_t));

					currCore = lockDynamic->ownerCore;
					currType = lockDynamic->ownerType;
					currId = lockDynamic->ownerId;

					/* Spinlock isn't occupied by another core, so occupy it. */
					if (currCore == OS_CORE_ID_INVALID)
					{
						lockDynamic->ownerCore = myCore;
						lockDynamic->ownerType = myType;
						lockDynamic->ownerId = myId;
						lockDynamic->predecessor = lastLock;
						OS_CacheFlush(lockStatic->dynamic, sizeof(os_autosarspinlockdyn_t));

						*lastLockPtr = OS_CreateObjectId(findSpinlock, OS_OBJ_SPINLOCK);
					}

					/* Release the software lock spinlock, but don't unlock interrupts yet.
					*/
					OS_DropInternalSpinlockOnly(OS_SOFTWARELOCK_LOCKID);

					if (currCore == OS_CORE_ID_INVALID)
					{
						/* Spinlock wasn't occupied by another core, so we got it. */
						*out = OS_TRUE;

						/* Take this spinlock's resource. */
						/* !LINKSTO Kernel.Autosar.API.SystemServices.GetSpinlock.LockMethod, 1
						 * !LINKSTO Kernel.Autosar.API.SystemServices.TryToGetSpinlock.LockMethod, 1
						*/
						OS_GetSpinlockResource(td, prio, &(lockDynamic->lastPrio), is);
					}
					else if (currCore == myCore)
					{
						/* Spinlock is already held by something on the same core.
						 * Restore the interrupt state and report the respective error.
						*/
						OS_IntRestore(is);

						/* !LINKSTO Kernel.Autosar.API.SystemServices.GetSpinlock.Occupied, 1
						 * !LINKSTO Kernel.Autosar.API.SystemServices.TryToGetSpinlock.Occupied, 1
						*/
						if ((currType == myType) && (currId == myId))
						{
							/* Can this happen? A spinlock cannot be a successor of itself, and even if it
							 * could be, we'd get stuck in an endless loop up above!
							*/
							result = OS_ERROR(OS_ERROR_SpinlockAlreadyHeld, OS_GET_PARAMETER_VAR());
						}
						else
						{
							result = OS_ERROR(OS_ERROR_SpinlockInterferenceDeadlock, OS_GET_PARAMETER_VAR());
						}
					}
					else
					{
						/* Spinlock was already occupied by another core, so we didn't get it. */
						OS_IntRestore(is);
						*out = OS_FALSE;
					}
				}
			}
		}
	}

	OS_TRACE_TRYTOGETSPINLOCK_EXIT_P(lockId, out);

	return result;
}

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
