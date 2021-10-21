/* lib-getisrmaxruntime.c
 *
 * This file contains the OS_GetIsrMaxRuntime function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-getisrmaxruntime.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#include <Os_kernel.h>
#include <Os_acct_api.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_GetIsrMaxRuntime
 *
 * This library function returns the longest observed run-time for an ISR.
 * It can be called by any trusted application. On some architectures
 * it might be callable from non-trusted applications too.
 *
 * If the ISR ID is out of range, or if the ISR does not have runtime
 * measurement enabled, E_OS_ID is returned. Otherwise, the ISR's
 * longest observed runtime is placed in the out location and E_OK is returned.
 * The error handler ia not called, so no error hook is called.
 *
 * !LINKSTO Kernel.Feature.OsAccounting, 1,
 * !        Kernel.Feature.OsAccounting.API.GetIsrMaxRuntime, 1
 * !doctype SRC
*/

os_result_t OS_GetIsrMaxRuntime(os_isrid_t i, os_tick_t *out)
{
	os_result_t r = OS_E_ID;
	const os_isr_t *isr;
	os_israccounting_t *ia;

	if ( i < OS_nInterrupts )
	{
		isr = &OS_isrTableBase[i];
		ia = OS_GET_ACCT(isr->accounting);

		if ( (isr->flags & OS_ISRF_MEASUREEXEC) != 0 )
		{
			*out = ia->etMax;
			r = OS_E_OK;
		}
	}

	return r;
}

#include <memmap/Os_mm_code_end.h>

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_GetIsrMaxRuntime</name>
  <synopsis>Get longest observed runtime of an ISR</synopsis>
  <syntax>
    os_result_t OS_GetIsrMaxRuntime
    (   os_isrid_t t    /@ ID of ISR @/
        os_tick_t *out  /@ Where to put the answer @/
    )
  </syntax>
  <description>
    <code>OS_GetIsrMaxRuntime()</code> places the longest observed execution time of the
    specified ISR into the variable referenced by 'out'. If the ISR ID is invalid or the
    ISR does not have execution-time measurement enabled (attribute MEASURE_MAX_RUNTIME),
    <code>OS_GetIsrMaxRuntime()</code> returns <code>OS_E_ID</code>.
  </description>
  <availability>
    Available from all trusted tasks, ISRs and hook functions. One some architectures, it
    might be possible to call this function from non-trusted contexts as well.
  </availability>
  <returns>OS_E_OK=Success.</returns>
  <returns>OS_E_ID=Invalid ISR ID specified.</returns>
  <returns>OS_E_ID=ISR does not have the feature enabled.</returns>
</api>

 * CHECK: PARSE
*/


/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/

