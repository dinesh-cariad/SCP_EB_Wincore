/* WINLIN-mycorehelper.c
 *
 * This file contains a helpr function to determine the core ID.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-mycorehelper.c 25843 2017-02-06 10:12:47Z vaka852 $
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/** \brief Helps the OS_GetMyCoreId() macro.
 *
 * \return The ID of the core on which this function was called.
 */
 
#if (OS_TOOL == OS_gnu)
#if OS_GccVersionIsAtLeast(4,5,0)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wbad-function-cast"
#endif
#endif

os_coreid_t OS_MyCoreHelper(void)
{
	return (os_coreid_t)OS_WINLIN_OSAL_TlsGetValue(OS_winlinTlsCoreId);
}

#if (OS_TOOL == OS_gnu)
#if OS_GccVersionIsAtLeast(4,5,0)
#pragma GCC diagnostic pop
#endif
#endif

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
