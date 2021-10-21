/* Os_starttickers.c
 *
 * This file contains the function OS_StartTickers
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_starttickers.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#include <Os_configuration.h>
#include <Os_kernel.h>
#include <board.h>

#ifndef OS_INHIBIT_GPT_STARTUP
#define OS_INHIBIT_GPT_STARTUP	0
#endif

#if OS_NGPTTICKERS != 0
#include <Gpt.h>
#endif

/*!
 * OS_StartTickers
 *
 * This function starts all configured software-counter tickers.
 *
 * For GPT tickers it is assumed that Gpt_Init has been called before StartOS. If this is not the case,
 * define the macro OS_INHIBIT_GPT_STARTUP in board.h and call the function OS_StartGptTickers() in the
 * Init-Task, after Gpt_Init has been called.
 *
 * !LINKSTO Kernel.Autosar.Counter.Types.SOFTWARE.GPTDriven, 1
*/
#include <memmap/Os_mm_code_begin.h>

void OS_StartTickers(void)
{
#if OS_NINTERNALTICKERS != 0
	OS_STARTINTERNALTICKERS();
#endif

#if (!OS_INHIBIT_GPT_STARTUP) && (OS_NGPTTICKERS != 0)
	OS_STARTGPTTICKERS();
#endif
}

#if (OS_INHIBIT_GPT_STARTUP) && (OS_NGPTTICKERS != 0)
void OS_StartGptTickers(void)
{
	OS_STARTGPTTICKERS();
}
#endif

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
