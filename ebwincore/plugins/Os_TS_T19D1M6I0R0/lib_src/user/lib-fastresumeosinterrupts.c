/* lib-fastresumeosinterrupts.c
 *
 * This file contains the OS_FastResumeOsInterrupts() function
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: lib-fastresumeosinterrupts.c 23953 2016-04-07 15:47:11Z mist9353 $
*/

#include <Os_kernel.h>
#include <Os_api.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_FastResumeOsInterrupts()
 *
 * Implements the ResumeOSInterrupts() services.
 *
 * This function restores the interrupt state previously saved by
 * OS_FastSuspendOsInterrupts(). The "OS" nesting counter
 * is decremented, and if it reaches zero the "OS" old level
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
void OS_FastResumeOsInterrupts(void)
{
	os_intstatus_t is;
	os_kerneldata_t * const kernel_data = OS_GetKernelData();

	if (kernel_data->fastSuspendResumeStatus.osNestCount == 1)
	{
		is = kernel_data->fastSuspendResumeStatus.osOldLevel;
		kernel_data->fastSuspendResumeStatus.osNestCount = 0;
		OS_IntRestore(is);
	}
	else
	{
		kernel_data->fastSuspendResumeStatus.osNestCount--;
	}
}

#include <memmap/Os_mm_code_end.h>

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_FastResumeOsInterrupts</name>
  <synopsis>Resume interrupts to a previously-saved level</synopsis>
  <syntax>
    void OS_FastResumeOsInterrupts(void)
  </syntax>
  <description>
    <para>
      <code>OS_FastResumeOsInterrupts()</code> restores the interrupt level of
      the processor or interrupt controller to the level that it was before
      the corresponding call to <code>OS_FastSuspendOsInterrupts()</code>.
      It is used to implement the <code>ResumeOsInterrupts()</code>
      system service.
    </para>
    <para>
      <code>ResumeOsInterrupts()</code> is nestable;
      this is implemented by a counter. The interrupt level is only truly
      manipulated on the outermost of the nested calls.
    </para>
    <para>
      CAVEAT: There are no tests or checks. It is up to the user to ensure
      that no OS services are called while interrupts are disabled and that
      the nesting count is not exceeded.
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
