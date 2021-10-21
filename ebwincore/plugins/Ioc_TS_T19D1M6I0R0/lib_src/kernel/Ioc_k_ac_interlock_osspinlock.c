/* Ioc_k_ac_interlock_osspinlock.c
 *
 * This file contains an cross-core lock implementation for use inside the EB tresos AutoCore OS kernel.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_k_ac_interlock_osspinlock.c 2524 2018-02-19 12:17:30Z kosc261728 $
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
#include <public/Ioc_libconfig.h>
#include <private/Ioc_lockinterface.h>
#include <private/Ioc_osinterface.h>
#include <Os_kernel.h>

#if (IOC_LCFG_KERNEL_TYPE != IOC_AUTOCOREOS)
#error "This file is only needed for the EB tresos AutoCore OS. Check your makefiles."
#endif

/* !LINKSTO IOC.Function.IOC_OsSpinlockKernel_Lock, 1
*/
ioc_return_t IOC_OsSpinlockKernel_Lock(volatile void *lock)
{
	/* Possible diagnostic TOOLDIAG-1 <1> */
	const ioc_address_t lockIdBits = (ioc_address_t)lock;
	/* If the configuration (IOC_channelConfigurations) is correct, lockIdBits contains a valid spinlock id. */
	const os_autosarspinlockid_t autosarLockId = (os_autosarspinlockid_t)lockIdBits;
	os_tryspinlock_t flag = OS_TRYTOGETSPINLOCK_NOSUCCESS;
	os_result_t result;
	ioc_return_t ret = IOC_E_NOK;

	result = OS_DoTryToGetSpinlock(autosarLockId, &flag);

	if (result == OS_E_OK)
	{
		if (flag == OS_TRYTOGETSPINLOCK_SUCCESS)
		{
			ret = IOC_E_OK;
		}
		else
		{
			ret = IOC_E_TRYAGAIN;
		}
	}
	return ret;
}

/* !LINKSTO IOC.Function.IOC_OsSpinlockKernel_Unlock, 1
*/
ioc_return_t IOC_OsSpinlockKernel_Unlock(volatile void *lock)
{
	/* Possible diagnostic TOOLDIAG-1 <1> */
	const ioc_address_t lockIdBits = (ioc_address_t)lock;
	/* If the configuration (IOC_channelConfigurations) is correct, lockIdBits contains a valid spinlock id. */
	const os_autosarspinlockid_t autosarLockId = (os_autosarspinlockid_t)lockIdBits;
	ioc_return_t ret = IOC_E_NOK;
	os_result_t result = OS_KernReleaseSpinlock(autosarLockId);

	if (result == OS_E_OK)
	{
		ret = IOC_E_OK;
	}
	return ret;
}

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
