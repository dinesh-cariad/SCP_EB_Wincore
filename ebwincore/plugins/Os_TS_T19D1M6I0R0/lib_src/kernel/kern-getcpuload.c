/* kern-getcpuload.c
 *
 * This file contains the OS_KernGetCpuLoad() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-getcpuload.c 30633 2019-08-12 14:02:48Z ache271795 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 13.5 (required)
 * The right hand operand of a logical && or || operator shall not contain side effects.
 *
 * Reason:
 * False positive: OS_GetMyCoreId() does not have side effects but only reads information. In the worst case it
 * triggers an exception to read that information, yet for the calling function this happens seamless.
*/

#define OS_SID OS_SID_GetCpuLoad
#define OS_SIF OS_svc_GetCpuLoad

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

/*!
 * OS_KernGetCpuLoad
 *
 * This function returns either the current or the peak CPU load (selected by 'getPeak') on the logical core 'coreId'.
 *
 * !LINKSTO Kernel.Feature.CpuLoadMeasurement, 1
 * !LINKSTO Kernel.Feature.CpuLoadMeasurement.API.GetCpuLoad, 1
*/
os_uint8_t OS_KernGetCpuLoad(os_coreid_t coreId, os_boolean_t getPeak)
{
	os_coreid_t coreIdPhy;

	/* Set 'result' to a value which can't be attained during normal operation.
	 * The valid range is [0,100].
	 */
	os_uint8_t result = 255u;
	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0,(os_paramtype_t)coreId);
	OS_SAVE_PARAMETER_N(1,(os_paramtype_t)getPeak);

	coreIdPhy = OS_GetPhysicalCoreId(coreId);

	if (!OS_CoreIdIsValid(coreIdPhy))
	{
		(void)OS_ERROR(OS_ERROR_InvalidCoreId, OS_GET_PARAMETER_VAR());
	}
	else if ( (OS_configMode & OS_CPULOAD) != 0 )
	{
		os_cpuloadmeasurementtype_t const measurementtype = (getPeak ? OS_CPULOAD_GET_PEAK : OS_CPULOAD_GET_CURRENT);
#if (OS_N_CORES == 1)
		result = OS_MeasureCoreLoad(measurementtype);
#else
		/* Deviation MISRAC2012-1 <1> */
		if ((coreIdPhy == OS_CORE_ID_THIS_CORE) || (coreIdPhy == OS_GetMyCoreId()))
		{
			result = OS_MeasureCoreLoad(measurementtype);
		}
		else
		{
			os_errorresult_t const e = OS_SendMeasureCpuLoad(coreIdPhy, measurementtype, &result);
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

	return result;
}

#include <memmap/Os_mm_code_end.h>

#endif	/* OS_EXCLUDE_CPULOAD */

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_UserGetCpuLoad</name>
  <synopsis>Return the CPU load as an integer percentage.</synopsis>
  <syntax>
    os_uint8_t OS_UserGetCpuLoad
    (   os_coreid_t coreId,		/@ Selects the core on which the measurement shall be taken. @/
    	os_boolean_t getPeak    /@ Returns peak load if true, otherwise current load. @/
    )
  </syntax>
  <description>
    <para>
    <code>OS_UserGetCpuLoad()</code> returns either the current or the peak CPU load, depending on the
    value of the <code>getPeak</code> parameter. The return value is a percentage in the range 0 to
    100.
    </para>
    <para>
    The core on which the measurement is taken is selected by <code>coreId</code>. Use the special value
    <code>OS_CORE_ID_THIS_CORE</code> to select the core on which the function is called.
    </para>
    <para>
    If CPU load measurement is disabled, no action takes place and the function returns 255 (0xff).
    </para>
  </description>
  <availability>
    Include the <code>Os_salsa.h</code> header file. No restrictions.
  </availability>
  <returns>0..100=CPU load as percentage</returns>
  <returns>255=Measurement is not enabled</returns>
</api>

<api API="OS_USER">
  <name>GetCpuLoad</name>
  <synopsis>Return the current CPU load as an integer percentage.</synopsis>
  <syntax>
    os_uint8_t GetCpuLoad (void)
  </syntax>
  <description>
    <para>
    <code>GetCpuLoad()</code> returns the current CPU load. The return value is a percentage in the range 0 to
    100.
    </para>
    <para>
    The measurement is taken on the core on which the function is called.
    </para>
    <para>
    If CPU load measurement is disabled, no action takes place and the function returns 255 (0xff).
    </para>
  </description>
  <availability>
    Include the <code>Os_salsa.h</code> header file. This function must not be called from non-trusted
    applications; instead, <code>OS_UserGetCpuLoad()</code> can be used.
  </availability>
  <returns>0..100=CPU load as percentage</returns>
  <returns>255=Measurement is not enabled</returns>
</api>

<api API="OS_USER">
  <name>GetMaxCpuLoad</name>
  <synopsis>Return the peak CPU load as an integer percentage.</synopsis>
  <syntax>
    os_uint8_t GetMaxCpuLoad (void)
  </syntax>
  <description>
    <para>
    <code>GetMaxCpuLoad()</code> returns the peak CPU load. The return value is a percentage in the range 0 to
    100.
    </para>
    <para>
    The measurement is taken on the core on which the function is called.
    </para>
    <para>
    If CPU load measurement is disabled, no action takes place and the function returns 255 (0xff).
    </para>
  </description>
  <availability>
    Include the <code>Os_salsa.h</code> header file. This function must not be called from non-trusted
    applications; instead, <code>OS_UserGetCpuLoad()</code> can be used.
  </availability>
  <returns>0..100=CPU load as percentage</returns>
  <returns>255=Measurement is not enabled</returns>
</api>

<api API="OS_USER">
  <name>GetCpuLoadOnCore</name>
  <synopsis>Return the current CPU load as an integer percentage.</synopsis>
  <syntax>
    os_uint8_t GetCpuLoadOnCore
    (	os_coreid_t	coreId /@ Selects the core on which the measurement shall be taken. @/
    )
  </syntax>
  <description>
    <para>
    <code>GetCpuLoadOnCore()</code> returns the current CPU load. The return value is a percentage in the range 0 to
    100.
    </para>
    <para>
    The measurement is taken on the core <code>coreId</code>.
    </para>
    <para>
    If CPU load measurement is disabled, no action takes place and the function returns 255 (0xff).
    </para>
  </description>
  <availability>
    Include the <code>Os_salsa.h</code> header file. This function must not be called from non-trusted
    applications; instead, <code>OS_UserGetCpuLoad()</code> can be used.
  </availability>
  <returns>0..100=CPU load as percentage</returns>
  <returns>255=Measurement is not enabled</returns>
</api>

<api API="OS_USER">
  <name>GetMaxCpuLoadOnCore</name>
  <synopsis>Return the peak CPU load as an integer percentage.</synopsis>
  <syntax>
    os_uint8_t GetMaxCpuLoadOnCore
    (	os_coreid_t	coreId /@ Selects the core on which the measurement shall be taken. @/
    )
  </syntax>
  <description>
    <para>
    <code>GetMaxCpuLoadOnCore()</code> returns the peak CPU load. The return value is a percentage in the range 0 to
    100.
    </para>
    <para>
    The measurement is taken on the core <code>coreId</code>.
    </para>
    <para>
    If CPU load measurement is disabled, no action takes place and the function returns 255 (0xff).
    </para>
  </description>
  <availability>
    Include the <code>Os_salsa.h</code> header file. This function must not be called from non-trusted
    applications; instead, <code>OS_UserGetCpuLoad()</code> can be used.
  </availability>
  <returns>0..100=CPU load as percentage</returns>
  <returns>255=Measurement is not enabled</returns>
</api>

 * CHECK: PARSE
*/

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
