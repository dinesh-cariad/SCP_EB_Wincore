/* kern-releasespinlockscurcore.c
 *
 * This file contains OS_ReleaseSpinlocksCurCore - a helper for the OS_Shutdown function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-releasespinlockscurcore.c 27818 2017-11-08 16:44:06Z mist9353 $
*/

#include <Os_kernel.h>
#include <Os_spinlock.h>

#if (OS_N_CORES < 2)
#error "This file is only needed on multi-core systems - please check your makefiles."
#endif

#include <memmap/Os_mm_code_begin.h>

/* Helper function: Releases all spinlocks held by the current core.
 * Preconditions: the caller has already locked interrupts.
 *
 * !LINKSTO Kernel.Autosar.Multicore.ShutdownOS.ReleaseSpinlock, 1
 */

void OS_ReleaseSpinlocksCurCore(void)
{
	os_autosarspinlockid_t i;
	os_coreid_t const myCoreId = OS_GetMyCoreId();

	for (i = 0; i < OS_nAutosarSpinlocks; i++)
	{
		os_autosarspinlockdyn_t * const dyn = OS_autosarSpinlockTableBase[i].dynamic;

		if (dyn->ownerCore == myCoreId)
		{
			dyn->ownerCore = OS_CORE_ID_INVALID;
			dyn->ownerType = OS_OBJ_NONE;
			dyn->ownerId = OS_NULLOBJECT;
			dyn->predecessor = OS_NULLOBJECT;

			OS_CacheFlush(dyn, sizeof(*dyn));
		}
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
