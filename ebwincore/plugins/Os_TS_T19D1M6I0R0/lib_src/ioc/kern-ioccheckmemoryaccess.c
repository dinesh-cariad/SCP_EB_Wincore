/* kern-ioccheckmemoryaccess.c
 *
 * This file contains the IOC helper function OS_KernIocCheckMemoryAccess
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-ioccheckmemoryaccess.c 23967 2016-04-12 14:20:28Z mist9353 $
*/
#include <Os_types.h>
#include <Os_kernel.h>
#include <Os_userkernel.h>
#include <Ioc/Ioc_kern.h>

os_memoryaccess_t OS_KernIocCheckMemoryAccess( const void *data, os_size_t len )
{
	os_memoryaccess_t access;
	os_uint8_t const inFunction = OS_GetKernelData()->inFunction;

	/* check data pointer */
	if ( inFunction == OS_INTASK )
	{
		os_task_t const * const current_task = OS_GetKernelData()->taskCurrent;
		access = OS_KernCheckTaskMemoryAccess(current_task->taskId, data, len);
	}
	else if ( inFunction == OS_INCAT2 )
	{
		os_isrid_t const current_isr = OS_GetKernelData()->isrCurrent;
		access = OS_KernCheckIsrMemoryAccess(current_isr, data, len);
	}
	else
	{
		access = 0;
	}

	return access;
}


/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
