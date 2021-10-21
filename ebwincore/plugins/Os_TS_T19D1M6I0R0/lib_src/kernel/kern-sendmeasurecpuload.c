/* kern-sendmeasurecpuload.c
 *
 * This file contains the OS_SendMeasureCpuLoad() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-sendmeasurecpuload.c 27282 2017-08-24 13:57:43Z olme8414 $
*/

#include <Os_kernel.h>
#include <Os_messagequeue.h>
#include <Os_cpuload_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_SendMeasureCpuLoad
 *
 * Sends the GetCpuLoad() request to the required core.
*/
os_errorresult_t OS_SendMeasureCpuLoad(os_coreid_t c, os_cpuloadmeasurementtype_t measurementType, os_uint8_t * out)
{
	os_paramtype_t msgParam[OS_MAXPARAM];

	msgParam[0] = (os_paramtype_t)measurementType;
	msgParam[1] = (os_paramtype_t)out;

#if OS_XC_REPLY_DIRECTLY
	OS_CacheInvalidate(out, sizeof(*out));
#endif

	return OS_SendMessageWait(c, OS_XC_OP_MeasureCpuLoad, msgParam);
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
