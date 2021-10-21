/* WINLIN-longjmp.c
 *
 * This file contains the implementation of function wrappers for longjmp(),
 * which is part of the standard C library. The motivation to encapsulate
 * this function in dedicated wrappers is to add EB_mir markup more easily.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-longjmp.c 27660 2017-10-18 14:07:36Z olme8414 $
 */

#include <Os_kernel.h>
#include <WINLIN/Os_WINLIN_jump.h>

#include <memmap/Os_mm_code_begin.h>

void OS_WINLINTaskLongjmp(void)
{
	longjmp(OS_WINLINTaskJmpBufs[OS_GetKernelData()->taskCurrent->taskId], 1);
}

void OS_WINLINCoreLongjmp(void)
{
	longjmp(OS_WINLINCoreJmpBufs[OS_GetMyCoreId()], 1);
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
