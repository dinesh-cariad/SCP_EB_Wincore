/* kern-startos.c
 *
 * This file contains the OS_KernStartOs function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-startos.c 27921 2017-11-20 16:30:08Z mist9353 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: PointlessComparison
 *   Condition is always true.
 *
 * Reason: On single-core configurations the check, whether an application runs on the current core
 *   is always true, but on multi-core configurations it isn't.
 *
 * TOOLDIAG-2) Possible diagnostic: StatementIsUnreachable
 *   statement not reached
 *
 * Reason:
 *  These warnings depend on the OS configuration and library optimization.
 *  The statements can be reached, if "error handling for APIs returning void" is not excluded.
 *  See also OS_EXCLUDE_ERRORHANDLINGFORVOIDAPI.
*/

#define OS_SID	OS_SID_StartOs
#define OS_SIF	OS_svc_StartOs

#include <Os_kernel.h>
#include <Os_tool.h>

#if (OS_KERNEL_TYPE == OS_MICROKERNEL)
#error "This file is not meant for the SafetyOS, please check your makefiles."
#endif

/* QM-OS Spinlocks are only required if we have a multicore system.
*/
#if (OS_N_CORES > 1)
#include <Os_spinlock.h>
#include <Os_panic.h>
#endif

/* We define the macros here because they are only used here. There's no point constructing a
 * special header file for them
*/
#include <Os_cpuload_kernel.h>

#define OS_StartupInterruptsSuspended()	( (OS_iecMode >= OS_IEC_AUTOSAR) && \
                                            ( (kernel_data->nestSuspendAll != 0) || (kernel_data->nestSuspendOs != 0) ) )
#define OS_ErrorHandlingForStartOs()	OS_ErrorHandlingForVoidApi()

/*!
 * OS_IsBadStartMode
 *
 * Checks, if the provided start mode is valid.
 */
#if (OS_N_CORES == 1)
#define OS_IsBadStartMode(m)		((m) >= OS_nStartModes)
#else
#define OS_IsBadStartMode(m)		(((m) != OS_NULLAPPMODE) && ((m) >= OS_nStartModes))
#endif /* OS_N_CORES == 1 */

/* Include definitions for tracing */
#include <Os_trace.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_KernStartOs
 *
 * The OS is started.
 *
 *	- If the OS is already running --> ERROR
 *	- Initialise stacks.
 *	- Activate all AUTOSTART tasks and alarms for this mode.
 *	- Perform all other initialisation required before returning.
 *
 * !LINKSTO Kernel.API.OSControl.StartOS.API, 1
 *
 * If the kernel variables contain their correct initial values when StartOS is first called,
 * the first call to StartOS can never return. This is because on return from OS_KernStartOs(),
 * there is no task ready and the dispatcher ends in the idle loop. This happens even if an error
 * condition such as interrupts-disabled or parameter-out-of-range is detected.
 *
 * !LINKSTO Kernel.API.OSControl.StartOS.NoReturn, 1
 * !LINKSTO Kernel.Autosar.OSEK.Differences.StartOS, 1
 * Look! No cross-core calls!
 * !LINKSTO Kernel.Autosar.Multicore.StartOS.CoreLocal, 1
*/
void OS_KernStartOs(os_appmodeid_t mode)
{
	os_intstatus_t is;
	const os_initfunc_t * fct;
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0,(os_paramtype_t)mode);

	OS_TRACE_STARTOS_ENTRY(mode);

	is = OS_IntDisableAll();

	if ( kernel_data->inFunction != OS_INBOOT )
	{
		if ( OS_ErrorHandlingForStartOs() )
		{
			/* can't propagate the return value of OS_ERROR -> ignore it */
			/* Possible diagnostic TOOLDIAG-2 <+1> */
			(void) OS_ERROR(OS_ERROR_WrongContext, OS_GET_PARAMETER_VAR());
		}
		OS_IntRestoreAll(is);
	}
	else
	if ( OS_StartupInterruptsSuspended() )
	{
		if ( OS_ErrorHandlingForStartOs() )
		{
			/* can't propagate the return value of OS_ERROR -> ignore it */
			/* Possible diagnostic TOOLDIAG-2 <+1> */
			(void) OS_ERROR(OS_ERROR_InterruptDisabled, OS_NULL);
		}
		OS_IntRestoreAll(is);
	}
	else
	if ( OS_IsBadStartMode(mode) )
	{
		if ( OS_ErrorHandlingForStartOs() )
		{
			/* can't propagate the return value of OS_ERROR -> ignore it */
			/* Possible diagnostic TOOLDIAG-2 <+1> */
			(void) OS_ERROR(OS_ERROR_InvalidStartMode, OS_GET_PARAMETER_VAR());
		}
		OS_IntRestoreAll(is);
	}
	else
	{
		/* !LINKSTO Kernel.API.OSControl.StartOS.StartOS, 1
		*/
#if (OS_N_CORES == 1)
		OS_appMode = mode;
#else
		OS_MultiDetermineAppMode(mode);
#endif

		kernel_data->inFunction = OS_ININTERNAL;

		/* !LINKSTO Kernel.Autosar.Multicore.StartOS.IOCInit, 1
		*/
		fct = &OS_initFunc[0];
		while (*fct != OS_NULL)
		{
			(*fct)();
			fct++;
		}

		/* Set the "appsStarted" flag here. The applications haven't been started yet, but they're in
		 * a state where we assume that the error hooks can be called if necessary.
		*/
		kernel_data->appsStarted = 1;

		/* !LINKSTO Kernel.Autosar.Multicore.Startup.Synchronize, 1
		*/
		OS_SyncHere();

		/* !LINKSTO Kernel.API.Hooks.StartupHook.API, 1
		 * !LINKSTO Kernel.API.Hooks.StartupHook.Startup, 1
		 * !LINKSTO Kernel.Autosar.Multicore.Startup.StartupHook, 1
		 * !LINKSTO Kernel.HookRoutines.PrioISRC2, 1
		 *		Interrupts are disabled here
		*/
		OS_CALLSTARTUPHOOK();

		/* Call all the application-specific startup hooks. This is done
		 * AFTER the global startup hook, according to the Autosar spec.
		 * It is also done after auto-started objects have been started,
		 * so that if the application is terminated due to a protection
		 * violation its tasks get killed.
		 *
		 * !LINKSTO Kernel.Autosar.OsApplication.ApplicationHooks.StartupHook.Order, 1
		*/
		/* Possible diagnostic TOOLDIAG-1 <2> */
		OS_CALLAPPSTARTUPHOOKS();

		kernel_data->hookApp = OS_NULL;

		/* !LINKSTO Kernel.Autosar.Multicore.Startup.Synchronize, 1
		*/
		OS_SyncHere();

		/* On non-system-call kernels we set the inKernel flag
		 * to 1 to force a dispatcher call on exit.
		*/
#if (OS_KERNEL_TYPE==OS_FUNCTION_CALL)
		kernel_data->inKernel = 1;
#endif

		/* Initialize the CPU load measurement. This has to be done as close as possible to the
		 * end so that the startup load doesn't adversely affect the measurements.
		*/
		OS_INITMEASURECPULOAD();
	}

	OS_TRACE_STARTOS_EXIT_P(mode);
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserStartOs</name>
  <synopsis>Start the OS</synopsis>
  <syntax>
    void OS_KernStartOs
    (   os_uint8_t mode    /@ Startup mode @/
    )
  </syntax>
  <description>
    <code>OS_UserStartOs()</code> starts the OS. The <code>mode</code>
    parameter determines the set of tasks and alarms that should be
    started automatically.
    <para>
    After the kernel data structures have been initialized, the
    startup hook is called, if it has been configured.
    </para>
    <para>
    Normally <code>OS_UserStartOs()</code> does not return. If the OS
    has already been started or the <code>mode</code>parameter is not valid
    the function could return, depending on how the error handler is defined
    to handle the error.
    </para>
  </description>
  <availability>
    <code>OS_UserStartOs()</code> can only be called once, from outside
    the OS. It is typically called from the system's <code>main()</code>
    function.
  </availability>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
