/* lib-fastresumeallinterrupts.c
 *
 * This file contains the OS_FastResumeAllInterrupts() function
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: lib-fastresumeallinterrupts.c 23953 2016-04-07 15:47:11Z mist9353 $
*/

#include <Os_kernel.h>
#include <Os_api.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_FastResumeAllInterrupts()
 *
 * Implements the ResumeAllInterrupts() and EnableAllInterrupts() services.
 *
 * This function restores the interrupt state previously saved by
 * OS_FastSuspendAllInterrupts(). The "all" nesting counter
 * is decremented, and if it reaches zero the "all" old level
 * is restored.
 *
 * Re-entrancy considerations:
 * Assuming a well-behaved application, the nesting count is invariant
 * through interrupts. The old level could get changed by an interrupt routine,
 * but only when the nesting count is zero. In this function, that happens when
 * the nesting count goes to zero. In that branch, the old level is already
 * copied to a local variable, so we do not use the (possibly modified)
 * old level after setting the nesting count to zero.
*/
void OS_FastResumeAllInterrupts(void)
{
	os_intstatus_t is;
	os_kerneldata_t * const kernel_data = OS_GetKernelData();

	if (kernel_data->fastSuspendResumeStatus.allNestCount == 1)
	{
		is = kernel_data->fastSuspendResumeStatus.allOldLevel;
		kernel_data->fastSuspendResumeStatus.allNestCount = 0;
		OS_IntRestore(is);
	}
	else
	{
		kernel_data->fastSuspendResumeStatus.allNestCount--;
	}
}

#include <memmap/Os_mm_code_end.h>

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_FastResumeAllInterrupts</name>
  <synopsis>Resume interrupts to a previously-saved level</synopsis>
  <syntax>
    void OS_FastResumeAllInterrupts(void)
  </syntax>
  <description>
	<para>
    <code>OS_FastResumeAllInterrupts()</code> restores the interrupt level of the processor
    or interrupt controller to the level that it was before the corresponding call to
    <code>OS_FastSuspendAllInterrupts()</code>.
    It is used to implement the <code>ResumeAllInterrupts()</code>
    and <code>DisableAllInterrupts()</code> system services.
    </para>
    <para>
    <code>ResumeAllInterrupts()</code> is nestable;
    this is implemented by a counter. The interrupt level is only truly manipulated on the outermost
    of the nested calls. According to the OSEK standard, <code>EnableAllInterrupts()</code> is
    not nestable, but due to the implementation here it is freely nestable with itself and with
    <code>ResumeAllInterrupts()</code>.
    </para>
    <para>
    CAVEAT: There are no tests or checks. It is up to the user to ensure that no OS services
    are called while interrupts are disabled and that the nesting count is not exceeded.
    Furthermore there is no interrupt lock timing.
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
