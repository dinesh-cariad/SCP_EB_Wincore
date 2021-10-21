/* kern-startupchecks.c
 *
 * This file contains the OS_StartupChecks function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-startupchecks.c 27282 2017-08-24 13:57:43Z olme8414 $
*/

#include <Os_kernel.h>
#include <Os_panic.h>

#include <memmap/Os_mm_code_begin.h>

/* OS_StartupChecks
 *
 * This function performs a variety of consistency and sanity checks on the configuration
 * data and other tables to try to ensure that the generator is behaving itself as far as we can
 * tell.
*/
void OS_StartupChecks(void)
{
	os_result_t result = OS_E_OK;
	const os_schkfunc_t * fct;

	fct = &OS_startupCheckFunc[0];
	while ( *fct != OS_NULL )
	{
		if ( (*fct)() != OS_E_OK )
		{
			result = OS_E_PANIC;
		}
		fct++;
	}

	if ( result != OS_E_OK )
	{
		OS_ShutdownNoHooks();
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
