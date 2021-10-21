/* kern-resetpeakcpuload.c
 *
 * This file contains the OS_KernResetPeakCpuLoad() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-resetpeakcpuload.c 30633 2019-08-12 14:02:48Z ache271795 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 13.5 (required)
 * The right hand operand of a logical && or || operator shall not contain side effects.
 *
 * Reason:
 * False positive: OS_GetMyCoreId() does not have side effects but only reads information.
 */

#define OS_SID OS_SID_ResetPeakCpuLoad
#define OS_SIF OS_svc_ResetPeakCpuLoad

#include <Os_kernel.h>
#ifndef OS_EXCLUDE_CPULOAD

/* Note: this comment is relevant for asc_Os/tools/errdb-to-c.pl
 *
 * OS_ERRORCODE_CHECK(OS_ERROR_CoreIsDown)
 */

#include <Os_cpuload.h>
#include <Os_cpuload_kernel.h>
#if (OS_N_CORES > 1)
#include <Os_messagequeue.h>
#endif

#include <memmap/Os_mm_code_begin.h>

/* OS_KernResetPeakCpuLoad
 *
 * This function resets the peak load to the current load value.
 *
 * Before resetting the value, we update to include the current busy interval. To perform
 * the computation we "enter" the idle state and then "leave" it again.
*/
void OS_KernResetPeakCpuLoad(os_coreid_t coreId)
{
	os_coreid_t coreIdPhy;

	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0,(os_paramtype_t)coreId);

	/* coreId is a logical core id, so we need to transform to physical core id */	
	coreIdPhy = OS_GetPhysicalCoreId(coreId);

	if (!OS_CoreIdIsValid(coreIdPhy))
	{
		(void)OS_ERROR(OS_ERROR_InvalidCoreId, OS_GET_PARAMETER_VAR());
	}
	else if ( (OS_configMode & OS_CPULOAD) != 0 )
	{
#if (OS_N_CORES == 1)
		(void)OS_MeasureCoreLoad(OS_CPULOAD_RESET_PEAK);
#else
		/* Deviation MISRAC2012-1 <1> */
		if ((coreIdPhy == OS_CORE_ID_THIS_CORE) || (coreIdPhy == OS_GetMyCoreId()))
		{
			(void)OS_MeasureCoreLoad(OS_CPULOAD_RESET_PEAK);
		}
		else
		{
			os_uint8_t result;
			os_errorresult_t const e = OS_SendMeasureCpuLoad(coreIdPhy, OS_CPULOAD_RESET_PEAK, &result);
			if (e != OS_ERRORCODE_NOCHECK(OS_ERROR_NoError))
			{
				(void)OS_ERROR_NOCHECK(e, OS_GET_PARAMETER_VAR());
			}
		}
#endif
	}
	else
	{
		/* MISRA-C */
	}
}

#include <memmap/Os_mm_code_end.h>

#endif	/* OS_EXCLUDE_CPULOAD */

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserResetPeakCpuLoad</name>
  <synopsis>Reset the CPU load monitor's peak load detector.</synopsis>
  <syntax>
    void OS_UserResetPeakCpuLoad
    (	os_coreid_t	coreId /@ Selects the core of which the peak load shall be reset. @/
    )
  </syntax>
  <description>
    <para>
    <code>OS_UserResetPeakCpuLoad()</code> resets the peak CPU load detector of the load monitoring
    system. The peak load latch is set to the current load, after first ensuring that the current load
    is up-to-date. This is done for the specified core.
    </para>
    <para>
    If CPU load measurement is disabled, no action takes place.
    </para>
  </description>
  <availability>
    Include the <code>Os_salsa.h</code> header file. No restrictions.
  </availability>
</api>

<api API="OS_USER">
  <name>InitCpuLoad</name>
  <synopsis>Reset the CPU load monitor's peak load detector.</synopsis>
  <syntax>
    void InitCpuLoad(void)
  </syntax>
  <description>
    <para>
    <code>InitCpuLoad()</code> resets the peak CPU load detector of the load monitoring
    system. The peak load latch is set to the current load, after first ensuring that the current load
    is up-to-date.
    </para>
    <para>
    If CPU load measurement is disabled, no action takes place.
    </para>
  </description>
  <availability>
    Include the <code>Os_salsa.h</code> header file. This function must not be called from non-trusted
    applications; instead, <code>OS_UserResetPeakCpuLoad()</code> can be used.
  </availability>
</api>

<api API="OS_USER">
  <name>InitCpuLoadOnCore</name>
  <synopsis>Reset the CPU load monitor's peak load detector.</synopsis>
  <syntax>
    void InitCpuLoadOncore
    (	os_coreid_t	coreId /@ Selects the core of which the peak load shall be initialized. @/
    )
  </syntax>
  <description>
    <para>
    <code>InitCpuLoad()</code> resets the peak CPU load detector of the load monitoring
    system. The peak load latch is set to the current load, after first ensuring that the current load
    is up-to-date. This is done for the specified core.
    </para>
    <para>
    If CPU load measurement is disabled, no action takes place.
    </para>
  </description>
  <availability>
    Include the <code>Os_salsa.h</code> header file. This function must not be called from non-trusted
    applications; instead, <code>OS_UserResetPeakCpuLoad()</code> can be used.
  </availability>
</api>

 * CHECK: PARSE
*/

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
