/* kern-checktaskmemoryaccess.c
 *
 * This file contains the OS_CheckTaskMemoryAccess function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-checktaskmemoryaccess.c 27926 2017-11-21 17:19:25Z mist9353 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: UnusedVariable
 *   Variable is set but never used.
 *
 * Reason:
 * Not an issue, variable will be used if at least one application exists.
 *
 * TOOLDIAG-2) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   The statements can be reached, if "error handling for APIs returning void" is not excluded.
 *   See also OS_EXCLUDE_ERRORHANDLINGFORVOIDAPI.
*/

#define OS_SID	OS_SID_CheckTaskMemoryAccess
#define OS_SIF	OS_svc_CheckTaskMemoryAccess

#include <Os_kernel.h>

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*
 * Created helper function to reduce cyclomatic complexity of the
 * function OS_KernCheckTaskMemoryAccess
*/
static os_memoryaccess_t OS_DoCheckTaskMemoryAccess
	(os_taskid_t, const void *, os_size_t);

static os_memoryaccess_t OS_DoCheckTaskMemoryAccess
(	os_taskid_t t,
	const void *ptr,
	os_size_t len
)
{
	os_memoryaccess_t a = 0;
	const os_task_t *tp;
	os_address_t first;
	os_address_t last;
	/* Possible diagnostic TOOLDIAG-1 <1> */
	const os_appcontext_t *app;
	os_address_t base;
	os_address_t limit;
	os_taskstate_t ts;

	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0,(os_paramtype_t)t);
	OS_SAVE_PARAMETER_N(1,(os_paramtype_t)ptr);
	OS_SAVE_PARAMETER_N(2,(os_paramtype_t)len);

	first = (os_address_t)ptr;
	last = first + len;

	if ( ptr == OS_NULL )
	{
		/* No access to OS_NULL pointer */
	}
	else
	if ( last > first )
	{
		tp = &OS_taskTableBase[t];
		app = OS_GET_APP(tp->app);

		base = (os_address_t)tp->stackBase;
		limit = base + tp->stackLen;

		if ( (first >= base) && (last <= limit) )
		{
			/* Stack is only readable/writable if the task is currently
			 * active. Stack is never executable.
			 * This applies to all tasks; trusted and non-trusted.
			*/
			ts = tp->dynamic->state;
			if ( (ts == OS_TS_READY_SYNC) ||
				 (ts == OS_TS_READY_ASYNC) ||
				 (ts == OS_TS_RUNNING) ||
				 (ts == OS_TS_WAITING) )
			{
				a = OS_MA_STACK | OS_MA_READ | OS_MA_WRITE;
			}
			else
			{
				a = OS_MA_STACK;
			}
		}
		else
		if ( OS_AppIsTrusted(app) )
		{
			/* The task doesn't belong to an application, or the
			 * application is trusted. Either way, the task can write
			 * anywhere.
			 *
			 * NOTE: Just because the task has "permission" to
			 * access the memory doesn't mean that it will be able to
			 * do so with impunity. For example, non-existent addresses
			 * will still cause bus error traps.
			*/
			a = OS_MA_READ | OS_MA_WRITE | OS_MA_EXEC;
		}
		else
		if ( OS_IS_WITHIN_TASK_REGION(first, last) )
		{
			/* Region lies entirely within the task's private data section.
			 * This region is READABLE and WRITEABLE
			*/
			a = OS_MA_READ | OS_MA_WRITE;
		}
		else
		if ( OS_IS_WITHIN_APP_REGION(first, last) )
		{
			/* Region lies entirely within the app's private data section.
			 * This region is READABLE and WRITEABLE
			*/
			a = OS_MA_READ | OS_MA_WRITE;
		}
		else
		{
			a = (os_memoryaccess_t)OS_TaskMemoryAccess(tp, first, last);
		}
	}
	else
	if ( OS_ErrorHandlingForVoidApi() )
	{
		/* can't propagate the return value of OS_ERROR -> ignore it */
		/* Possible diagnostic TOOLDIAG-2 <+1> */
		(void) OS_ERROR(OS_ERROR_InvalidMemoryRegion, OS_GET_PARAMETER_VAR());
	}
	else
	{
		/* MISRA-C */
	}

	return a;

}

/*!
 * OS_KernCheckTaskMemoryAccess implements the API CheckTaskMemoryAccess
 *
 * !LINKSTO Kernel.Autosar.API.SystemServices.CheckTaskMemoryAccess, 2
 *
 * There is no permission check here ...
 * !LINKSTO Kernel.Autosar.OsApplication.Permissions.Exceptions, 1
*/
os_memoryaccess_t OS_KernCheckTaskMemoryAccess
(	os_taskid_t t,
	const void *ptr,
	os_size_t len
)
{
	os_memoryaccess_t a = 0;

	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0,(os_paramtype_t)t);
	OS_SAVE_PARAMETER_N(1,(os_paramtype_t)ptr);
	OS_SAVE_PARAMETER_N(2,(os_paramtype_t)len);

	OS_TRACE_CHECKTASKMEMORYACCESS_ENTRY(t);

	if ( !OS_CallingContextCheck() )
	{
		if ( OS_ErrorHandlingForVoidApi() )
		{
			/* can't propagate the return value of OS_ERROR -> ignore it */
			/* Possible diagnostic TOOLDIAG-2 <+1> */
			(void) OS_ERROR(OS_ERROR_WrongContext, OS_GET_PARAMETER_VAR());
		}
	}
	else
	if ( !OS_InterruptEnableCheck(OS_IEC_AUTOSAR) )
	{
		if ( OS_ErrorHandlingForVoidApi() )
		{
			/* can't propagate the return value of OS_ERROR -> ignore it */
			/* Possible diagnostic TOOLDIAG-2 <+1> */
			(void) OS_ERROR(OS_ERROR_InterruptDisabled, OS_GET_PARAMETER_VAR());
		}
	}
	else
	if ( OS_IsValidTaskId(t) )
	{
		a = OS_DoCheckTaskMemoryAccess(t, ptr, len);
	}
	else
	if ( OS_ErrorHandlingForVoidApi() )
	{
		/* can't propagate the return value of OS_ERROR -> ignore it */
		/* Possible diagnostic TOOLDIAG-2 <+1> */
		(void) OS_ERROR(OS_ERROR_InvalidTaskId, OS_GET_PARAMETER_VAR());
	}
	else
	{
		/* MISRA-C */
	}

	OS_TRACE_CHECKTASKMEMORYACCESS_EXIT_P(a,t);
	return a;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserCheckTaskMemoryAccess</name>
  <synopsis>Returns memory access permissions for a task</synopsis>
  <syntax>
    os_memoryaccess_t OS_KernCheckTaskMemoryAccess
    (   os_taskid_t t,    /@ Task ID @/
        void *ptr,        /@ Address of memory @/
        os_size_t len     /@ Length of memory @/
    )
  </syntax>
  <description>
    <para>
    <code>OS_UserCheckTaskMemoryAccess()</code> returns the access
    permissions (read/write/execute) for the referenced task for the
    specified memory region. In addition, the return value indicates
    whether the memory is in the task's stack.
    </para><para>
    The stack is only considered to be accessible when the task is active.
    </para><para>
    The return value is a logical OR of the bit fields given below.
    </para>
  </description>
  <availability>
    No restrictions.
  </availability>
  <returns>OS_MA_READ=The memory is readable</returns>
  <returns>OS_MA_WRITE=The memory is writeable</returns>
  <returns>OS_MA_EXEC=The memory is executable</returns>
  <returns>OS_MA_STACK=The memory is in the stack</returns>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
