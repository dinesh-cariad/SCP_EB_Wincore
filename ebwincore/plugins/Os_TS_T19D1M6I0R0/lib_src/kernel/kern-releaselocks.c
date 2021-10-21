/* kern-releaselocks.c
 *
 * This file contains the function OS_ReleaseLocks().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-releaselocks.c 31463 2020-02-25 10:20:28Z waab271441 $
*/

#include <Os_kernel.h>

#if (OS_N_CORES > 1u)
#include <Os_spinlock.h>
#endif

/* OS_ReleaseLocks() releases all locks headed by the lock whose ID is given as parameter
 *
 * On single-core systems, the only locks are resources, so a lock ID is simply a resource ID.
 * On multicore systems there are resources and spinlocks. The lock ID identifies the type as well as
 * the ID of the spinlock or resource.
*/
void OS_ReleaseLocks(os_objectid_t head)
{
	os_objectid_t lock = head;

	while (lock != OS_NULLOBJECT)
	{
#if (OS_N_CORES > 1)
		if (OS_GetTypeFromObjectId(lock) == OS_OBJ_SPINLOCK)
		{
			os_autosarspinlockdyn_t * sld = OS_autosarSpinlockTableBase[OS_GetIdFromObjectId(lock)].dynamic;

			os_intstatus_t is = OS_TakeInternalLock(OS_SOFTWARELOCK_LOCKID);

			lock = sld->predecessor;
			sld->predecessor = OS_NULLOBJECT;
			sld->ownerCore = OS_CORE_ID_INVALID;
			sld->ownerType = OS_OBJ_NONE;
			sld->ownerId = OS_NULLOBJECT;
			OS_CacheFlush(sld, sizeof(os_autosarspinlockdyn_t));

			OS_DropInternalLock(OS_SOFTWARELOCK_LOCKID, is);
		}
		else
#endif
		{
			os_resourcedynamic_t * rd = OS_ResourceGetDynamic(OS_GetIdFromObjectId(lock));

			lock = rd->next;

			rd->next = OS_NULLOBJECT;
			rd->takenBy = OS_NULLTASK;
			rd->lastPrio = 0;
		}
	}
}

