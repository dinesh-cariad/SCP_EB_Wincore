/* WINLIN-setlongjmp.c
 *
 * This file contains the setjmp/longjmp wrapper functions for task and core threads.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-setlongjmp.c 23341 2016-01-18 12:40:01Z olme8414 $
*/

#include <Os_configuration.h>
#include <setjmp.h>
#include <WINLIN/Os_WINLIN_cpu.h>

#include <memmap/Os_mm_var_begin.h>

/** \brief Set/longjmp buffers for tasks.
 *
 * These buffers are used by task threads to record their entry points.
 * It's necessary to know an entry point when a task terminates.
 * The entry point is where OS_WINLINTaskSetjmp() is called.
 *
 * To avoid an array of zero size in case there are no tasks configured
 * (i.e. OS_NTASKS == 0), one is added.
 */
jmp_buf OS_WINLINTaskJmpBufs[OS_NTASKS + 1];

/** \brief Set/longjmp buffer for core threads.
 *
 * These buffers are used by core threads to record their entry points.
 * The entry point is where OS_WINLINCoreSetjmp() is called.
 */
jmp_buf OS_WINLINCoreJmpBufs[OS_N_CORES_MAX];

#include <memmap/Os_mm_var_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
