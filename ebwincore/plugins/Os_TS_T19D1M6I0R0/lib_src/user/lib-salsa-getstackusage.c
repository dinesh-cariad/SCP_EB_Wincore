/* lib-salsa-getstackusage.c
 *
 * This file contains the GetStackUsage function of the SALSA API.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: lib-salsa-getstackusage.c 27282 2017-08-24 13:57:43Z olme8414 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 12.3 (advisory)
 * The comma operator should not be used.
 *
 * Reason:
 * For function-call kernels, OS API is wrapped in macros. The macro wrappers
 * need to use the comma operator to form an expression evaluating to the
 * correct result.
 *
 *
 * MISRAC2012-2) Deviated Rule: 13.3 (advisory)
 * A full expression containing an increment (++) or decrement (--) operator
 * should have no other potential side effects other than that caused by the
 * increment or decrement operator.
 *
 * Reason:
 * For function-call kernels, OS API is wrapped in macros. The macro wrappers
 * need to handle an "in kernel" flag so that the increment operator has to be
 * used in combination with the comma operator.
*/

#include <Os_salsa.h>
#include <Os_api.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * GetStackUsage implements the API GetStackUsage
 *
 * The system call OS_UserGetStackInfo is called, first setting the
 * info structure's current SP to 0. The answer is then computed.
 *
 * The return value is the percentage of task stack used.
 * If anything goes wrong, 255 is returned.
 *
 * Note this function will fail when a stack bigger than 42 MegaBytes is configured
 *
 * !LINKSTO Kernel.Feature.StackCheck.API.getUsedTaskStack, 1
 * !LINKSTO Kernel.Feature.StackCheck.API.GetStackUsage, 1
*/

os_uint8_t GetStackUsage(os_taskid_t t)
{
	os_uint8_t answer = 255U;
	os_stackinfo_t info;

	info.stackPointer = 0;

	/* Deviation MISRAC2012-1, MISRAC2012-2 <1> */
	if ( OS_UserGetStackInfo( OS_TaskToTOI((os_taskorisr_t)t), &info) == OS_E_OK )
	{
		answer = (os_uint8_t)
				( ( ((info.stackLen - info.stackClean) * 100) + (info.stackLen / 2) ) / info.stackLen );
	}

	return answer;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
