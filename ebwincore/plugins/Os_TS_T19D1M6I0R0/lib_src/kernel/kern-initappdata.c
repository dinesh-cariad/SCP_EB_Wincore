/* kern-initapplicationdata.c
 *
 * This file contains the OS_InitApplicationData() and
 * OS_InitAppData() functions. The two are together in one file
 * because they will never be used separately.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-initappdata.c 27282 2017-08-24 13:57:43Z olme8414 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: PointlessComparison
 *   Condition is always true.
 *
 * Reason: These warnings depend on the number of configured cores.
 *   Only in case of a single core configuration, these comparisons are always true.
 *
 * TOOLDIAG-2) Possible diagnostic: UnusedVariable
 *   Variable was declared but never referenced.
 *
 * Reason: If more than one core is in use, or if memory protection is
 *   used, then the variable is used.
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

#if (OS_KERNEL_TYPE==OS_SYSTEM_CALL) && (OS_HASMEMPROT == 1)
static void OS_InitDataArea(os_uint8_t *, os_uint8_t *, os_uint8_t const *, os_uint8_t const *);
#endif

/*!
 * OS_InitApplicationData()
 *
 * This function initializes the private data/bss areas for every application, task and ISR.
*/
void OS_InitApplicationData(void)
{
	os_applicationid_t a;
	os_taskid_t t;
	os_isrid_t i;
	const os_coreid_t myCoreId = OS_GetMyCoreId();

	for (a = 0; a < OS_nApps; a++)
	{
		/* Possible diagnostic TOOLDIAG-2 <1> */
		const os_appcontext_t *ap = &OS_appTableBase[a];
		/* Possible diagnostic TOOLDIAG-1 <1> */
		if (OS_GetAppCoreId(ap) == myCoreId)
		{
			OS_SET_MP(OS_InitDataArea(ap->dataStart, ap->dataEnd, ap->idataStart, ap->idataEnd));
		}
	}

	for (t = 0; t < OS_nTasks; t++)
	{
		/* Possible diagnostic TOOLDIAG-2 <1> */
		const os_task_t *tp = &OS_taskTableBase[t];
		/* Possible diagnostic TOOLDIAG-1 <1> */
		if (OS_GetTaskCoreId(tp) == myCoreId)
		{
			OS_SET_MP(OS_InitDataArea(tp->dataStart, tp->dataEnd, tp->idataStart, tp->idataEnd));
		}
	}

	for (i = 0; i < OS_nInterrupts; i++)
	{
		/* Possible diagnostic TOOLDIAG-2 <1> */
		const os_isr_t *ip = &OS_isrTableBase[i];
		/* Possible diagnostic TOOLDIAG-1 <1> */
		if (OS_GetIsrCoreId(ip) == myCoreId)
		{
			OS_SET_MP(OS_InitDataArea(ip->dataStart, ip->dataEnd, ip->idataStart, ip->idataEnd));
		}
	}
}

#if (OS_KERNEL_TYPE==OS_SYSTEM_CALL) && (OS_HASMEMPROT == 1)
/*!
 * OS_InitDataArea()
 *
 * This function initialises a single object's data/bss area.
*/
static void OS_InitDataArea
(	os_uint8_t *dest,
	os_uint8_t *dlimit,
	os_uint8_t const *src,
	os_uint8_t const *slimit
)
{
	/* Only initialise the data area if there is one!
	*/
	if ( dest != OS_NULL )
	{
		/* Initialisation is only done if the initialisation data exists (address is not OS_NULL),
		 * thus permitting use of uninitialised data, memory-mapped peripherals etc.
		*/
		if ( src != OS_NULL )
		{
			/* Copy all of the initialisation data to the start of the data area.
			 * This initialises the aggregated ".data" section.
			 * If the region is entirely bss, the src and slimit (__IDAT_* and __IEND_*) should be equal.
			*/
			while ( src < slimit )
			{
				*dest = *src;
				dest++;
				src++;
			}

			/* Zero the rest of the data area.
			 * This initialises the aggregated ".bss" section
			*/
			while ( dest < dlimit )
			{
				*dest = 0;
				dest++;
			}
		}
	}
}
#endif

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
