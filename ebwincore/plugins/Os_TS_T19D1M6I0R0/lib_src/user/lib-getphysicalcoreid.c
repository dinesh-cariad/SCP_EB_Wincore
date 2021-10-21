/* lib-getphysicalcoreid.c
 *
 * This file contains the OS_GetPhysicalCoreId function.
 * It returns the physical ID of the given logical ID
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: lib-getphysicalcoreid.c 30633 2019-08-12 14:02:48Z ache271795 $
*/

#include <Os_alci.h>

#if ((OS_KERNEL_TYPE != OS_MICROKERNEL) && (OS_USE_LOGICAL_CORE_IDS == 1))

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_GetPhysicalCoreId returns the physical ID of the given logical ID
 *
 * !LINKSTO Kernel.Misc.OS_GetPhysicalCoreId, 1
*/
os_coreid_t OS_GetPhysicalCoreId(os_coreid_t core)
{
	if (core >= OS_N_CORES_MAX)
	{
		return core;
	}
	else
	{
		return OS_alciLogToPhy[core];
	}
}

#include <memmap/Os_mm_code_end.h>

#else /* (!((OS_KERNEL_TYPE != OS_MICROKERNEL) && (OS_USE_LOGICAL_CORE_IDS == 1))) */
os_uint8_t _os_lib_getphysicalcoreid_c_dummy_;
#endif /* ((OS_KERNEL_TYPE != OS_MICROKERNEL) && (OS_USE_LOGICAL_CORE_IDS == 1)) */

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/

