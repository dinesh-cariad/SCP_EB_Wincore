/* WINLIN-setinterruptdisablelevel.c
 *
 * This file contains the implementation of OS_WINLINSetInterruptDisableLevel().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-setinterruptdisablelevel.c 23286 2015-12-22 12:05:22Z tojo2507 $
 */

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/** \brief Sets the interrupt disable level of the current core.
 *
 * \param newTargetStatus	The new interrupt disable level.
 * \param mode				The way how to change the interrupt disable level.
 *
 * \return The interrupt disable level before it was changed as indicated
 * by \a mode.
 */
os_intstatus_t OS_WINLINSetInterruptDisableLevel(
		os_intstatus_t targetStatus,
		os_setInterruptDisableLevelMode_t mode)
{
	os_intstatus_t const retVal = OS_WINLINGetCoreInterruptDisableLevel();

	switch (mode)
	{
	case OS_WINLIN_SET_INTERRUPT_DISABLE_LEVEL_RESTORE:
	case OS_WINLIN_SET_INTERRUPT_DISABLE_LEVEL_EPLICIT:
		OS_WINLINSetCoreInterruptDisableLevel(targetStatus);
		break;

	case OS_WINLIN_SET_INTERRUPT_DISABLE_LEVEL_CONDITIONAL:
		if (retVal < targetStatus)
		{
			OS_WINLINSetCoreInterruptDisableLevel(targetStatus);
		}
		break;

	default:
		OS_WINLINPanic();
		break;
	}

	return retVal;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
