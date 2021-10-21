/* Ioc_k_mk_interlock_osspinlock.c
 *
 * This file contains a cross-core lock implementation using Microkernel OS
 * spinlocks, usable from kernel space.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_k_mk_interlock_osspinlock.c 2497 2018-02-06 12:11:28Z olme8414 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 11.5 (advisory)
 *  A conversion should not be performed from pointer to void into pointer to
 *  object.
 *
 * Reason:
 *  On some architectures the macro MK_GetCoreVars() may use data returned by
 *  hardware to gain access to the core variables. Because hardware does not
 *  know about C language types, a type conversion is required.
 */

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: CastDiscardsQualPtrTgtType
 *   Volatile is cast away from pointer target type.
 *
 * Reason: Not an issue, as pointer will not be used to access memory but
 * instead its numerical value is used as an id. Hence, the volatility of the
 * target type is of no meaning here.
 */

#include <public/Ioc_basic_types.h>
#include <private/Ioc_lockinterface.h>
#include <private/Mk_core.h>
#include <private/Mk_lock.h>

#if (IOC_LCFG_KERNEL_TYPE != IOC_MICROKERNEL)
#error "This file is meant for the microkernel variant of the IOC. Check your makefiles."
#endif

/*
 * !LINKSTO IOC.Function.IOC_OsSpinlockKernel_Lock, 1
*/
/* Deviation MISRA-1 */
ioc_return_t IOC_OsSpinlockKernel_Lock(volatile void * spinlock_id)
{
	ioc_return_t ret = IOC_E_NOK;
	/* Possible diagnostic TOOLDIAG-1 <1> */
	mk_lockid_t lockIndex = MK_LockIdToIndex((mk_lockid_t)spinlock_id);
	/* Deviation MISRAC2012-1 <1> */
	mk_kernelcontrol_t *coreVars = MK_GetCoreVars();
	mk_lock_t *lock = &MK_lockTables[coreVars->coreIndex][lockIndex];

	/* We need to acquire the spinlock.
	 * if that fails the lock does not get taken, and the service returns "try again".
	*/
	if ( MK_HwTrySpinlock(lock->spinlock) )
	{
		/* Got the spinlock; now occupy the lock.
		*/
		ret = IOC_E_OK;
	}
	else
	{
		/* Spinlock is occupied. We need to try again.
		*/
		ret = IOC_E_TRYAGAIN;
	}

	return ret;
}

/*
 * !LINKSTO IOC.Function.IOC_OsSpinlockKernel_Unlock, 1
*/
/* Deviation MISRA-1 */
ioc_return_t IOC_OsSpinlockKernel_Unlock(volatile void * spinlock_id)
{
	/* Possible diagnostic TOOLDIAG-1 <1> */
	mk_lockid_t lockIndex = MK_LockIdToIndex((mk_lockid_t)spinlock_id);
	/* Deviation MISRAC2012-1 <1> */
	mk_kernelcontrol_t *coreVars = MK_GetCoreVars();
	mk_lock_t *lock = &MK_lockTables[coreVars->coreIndex][lockIndex];

	MK_HwDropSpinlock(lock->spinlock);

	return IOC_E_OK;
}
