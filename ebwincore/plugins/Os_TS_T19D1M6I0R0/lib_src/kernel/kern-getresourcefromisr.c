/* kern-getresourcefromisr.c
 *
 * This file contains the OS_KernGetResource function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-getresourcefromisr.c 27926 2017-11-21 17:19:25Z mist9353 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: UnusedVariable
 *   Variable is set but never used.
 *
 * Reason: Not an issue, variable will be used if at least one application exists.
 *
 * TOOLDIAG-2) Possible diagnostic: UnusedVariable
 *   Variable is set but never used.
 *
 * Reason: Not an issue, variable will be used if timing protection is enabled.
 *
 * TOOLDIAG-3) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   This statement can be reached if ISRs are allowed to take resources.
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-2) Deviated Rule: 11.5 (advisory)
 * A conversion should not be performed from pointer to void into pointer to
 * object.
 *
 * Reason:
 * Some architectures need to query hardware to determine its current state.
 * These accesses involve type casts, because hardware naturally has no notion
 * of C language types.
 *
 *
 * MISRAC2012-3) Deviated Rule: 11.6 (required)
 * A cast shall not be performed between pointer to void and an arithmetic type.
 *
 * Reason:
 * Some architectures need to query hardware to determine its current state.
 * These accesses involve type casts, because hardware naturally has no notion
 * of C language types.
 */

#define OS_SID	OS_SID_GetResource
#define OS_SIF	OS_svc_GetResource

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/* OS_GetResourceFromIsr
 *
 * This function implements the GetResource system service in the kernel, when the caller is a Category 2 ISR
*/
os_result_t OS_GetResourceFromIsr(os_resourceid_t r, os_resourcedynamic_t *rd, os_paramtype_t *p)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	os_result_t result = OS_E_OK;
	const os_resource_t *rs = &OS_resourceTableBase[r];
	os_intstatus_t is;
	/* Possible diagnostic TOOLDIAG-2 <1> */
	const os_isr_t *isrp = &OS_isrTableBase[kernel_data->isrCurrent];
	/* Possible diagnostic TOOLDIAG-1 <1> */
	const os_appcontext_t *app = OS_GET_APP(isrp->app);


	OS_PARAM_UNUSED(p);

	/* !LINKSTO Kernel.API.ResourceManagement.GetResource.ISRC2, 1
	*/
	if ( !OS_HasPermission(app, rs->permissions) )
	{
		result = OS_ERROR(OS_ERROR_Permission, p);
	}
	else
	if ( OS_ResourceInvalidForIsr(isrp, rs) )
	{
		/* This happens if resource priority is a task priority as
		 * well as if the resource priority is an interrupt priority
		 * lower than the run-level of the current ISR
		*/
		result = OS_ERROR(OS_ERROR_ResourcePriorityError, p);
	}
	else
	{
		/* Now we must disable interrupts to prevent an ISR that
		 * takes the resource from overwriting what we write into
		 * the dynamic structure. This also prevents the execution-
		 * budget monitor from killing the ISR and leaving the
		 * resource in an undefined state.
		*/
		/* Possible diagnostic TOOLDIAG-3 <+1> */
		is = OS_IntDisable();

		/* Chain the resource into the ISR lock queue.
		 * !LINKSTO Kernel.API.ResourceManagement.GetResource.NestedResources, 1
		*/
		rd->next = kernel_data->isrLastLock;
		kernel_data->isrLastLock = OS_CreateObjectId(r, OS_OBJ_RESOURCE);

		/* Store the old ISR priority into the resource's
		 * save location.
		*/
		rd->lastPrio = OS_GetInterruptPriority(is);

		/* Set taker of the resource
		*/
		rd->takenBy = OS_IsrToTOI(isrp->isrId);

		/* Start resource-lock timing if necessary
		*/
		if ( OS_GET_RLOCK((isrp->resourceLockTime != OS_NULL) &&
				(OS_GET_RLOCK(isrp->resourceLockTime[r]) != 0)) )
		{
			OS_STARTRESLOCKTIMING(rd, isrp->resourceLockTime[r]);
		}

		/* If resource's priority is higher, set the new task
		 * priority.
		 * !LINKSTO Kernel.ResourceManagement.PriorityCeiling, 1
		*/
		if ( rs->prio > rd->lastPrio )
		{
			/* This branch sets the current level to the
			 * resource's level, so we must not
			 * restore the saved interrupt state.
			*/
			/* Deviation MISRAC2012-2, MISRAC2012-3 <1> */
			OS_SetIsrResourceLevel(rs->prio);
			OS_IntRestoreHardLock(is);
		}
		else
		{
			/* The ISR priority was already greater than the
			 * resource's priority, so we simply restore the
			 * original interrupt level
			*/
			OS_IntRestore(is);
		}
	}

	return result;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
