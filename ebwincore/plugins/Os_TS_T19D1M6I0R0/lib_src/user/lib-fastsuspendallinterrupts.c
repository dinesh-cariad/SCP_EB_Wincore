/* lib-fastsuspendallinterrupts.c
 *
 * This file contains the OS_FastSuspendAllInterrupts() function.
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: lib-fastsuspendallinterrupts.c 23953 2016-04-07 15:47:11Z mist9353 $
*/

#include <Os_kernel.h>
#include <Os_api.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_FastSuspendAllInterrupts()
 *
 * This function disables interrupts, implementing the SuspendAllInterrupts()
 * and DisableAllInterrupts() system services.
 *
 * CAVEAT:
 *  - using the same function for both SuspendAllInterrupts() and DisableAllInterrupts()
 *    means that, unlike the OSEK standard, these functions are freely nestable and
 *    interchangable without any problems occurring.
 *  - the functions must NEVER be called from a non-trusted application
 *  - OS system services must NEVER be called inside a Suspend/Resume pair
 *  - error checking is not implemented. The functions must never be nested more
 *    than 255 deep.
 *  - interrupt lock timing is not implemented, and any other execution timing might be
 *    disabled
 *
 * Re-entrancy considerations:
 * Assuming a well-behaved application, the nesting count is invariant
 * through interrupts. The old level could get changed by an interrupt routine,
 * but only when the nesting count is zero. In this function, that happens when
 * the nesting count is zero on entry. In that branch, we do not write to the
 * old level variable until after setting the nesting count to 1. However,
 * disabling interrupts must be done *before* setting the nesting count,
 * otherwise this function would fail if called from an ISR that occurs after
 * setting the nesting count but before disabling interrupts.
*/
void OS_FastSuspendAllInterrupts(void)
{
	os_intstatus_t is;
	os_kerneldata_t * const kernel_data = OS_GetKernelData();

	if (kernel_data->fastSuspendResumeStatus.allNestCount == 0)
	{
		is = OS_IntDisableAll();
		kernel_data->fastSuspendResumeStatus.allNestCount = 1;
		kernel_data->fastSuspendResumeStatus.allOldLevel = is;
	}
	else
	{
		kernel_data->fastSuspendResumeStatus.allNestCount++;
	}
}

#include <memmap/Os_mm_code_end.h>

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_FastSuspendAllInterrupts</name>
  <synopsis>Suspend interrupts up to the "all" lock level</synopsis>
  <syntax>
    void OS_FastSuspendAllInterrupts(void)
  </syntax>
  <description>
	<para>
    <code>OS_FastSuspendAllInterrupts()</code> raises the interrupt level of the processor
    or interrupt controller to a level that locks out all Cat1 and Cat2 interrupts.
    It is used to implement the <code>SuspendAllInterrupts()</code> and <code>DisableAllInterrupts()</code>
    system services.
    </para>
    <para>
    <code>SuspendAllInterrupts()</code> and <code>DisableAllInterrupts()</code> are
    interchangable and mutually nestable;
    this is implemented by a counter. The interrupt level is only truly manipulated on the outermost
    of the nested calls. The equivalence of the two services is a deviation from a strict
    interpretation of the OSEK and Autosar standards.
    </para>
    <para>
    WARNING: no error checking is implemented, therefore incorrect nesting, or calling
    system services between Suspend and Resume pairs, could cause unexpected
    behaviour. No interrupt lock timing is implemented, and a running execution or
    resource lock timer might be disabled for the duration of the interrupt lock.
    </para>
  </description>
  <availability>
  </availability>
</api>

 * CHECK: PARSE
*/

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
