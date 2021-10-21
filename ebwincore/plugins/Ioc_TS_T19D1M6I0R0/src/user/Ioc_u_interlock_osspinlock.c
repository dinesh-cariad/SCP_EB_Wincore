/* Ioc_u_interlock_osspinlock.c
 *
 * This file contains an cross-core lock implementation using Autosar OS spinlocks,
 * usable from user space.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_u_interlock_osspinlock.c 2964 2018-11-14 14:05:00Z olme8414 $
*/
/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: UnusedVariable
 *   Variable is set but never used.
 *
 * Reason: Not an issue, variable will be used if at least one spinlock exists.
 *
 * TOOLDIAG-2) Possible diagnostic: CastDiscardsQualPtrTgtType
 *   Volatile is cast away from pointer target type.
 *
 * Reason: Not an issue, as pointer will not be used to access memory but
 * instead its numerical value is used as an id. Hence, the volatility of the
 * target type is of no meaning here.
 */
/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 12.3 (advisory)
 * The comma operator should not be used.
 *
 * Reason:
 * A function-call kernel maps API calls to the function OS_ArchLeaveKernel().
 * This function takes one argument, which is the return value of the requested
 * kernel service. The necessity to call OS_EnterKernel() before this service
 * requires the use of the comma operator. Otherwise, the API macros which are
 * used for this mapping do no longer expand into plain C-language expressions,
 * which is required here.
 *
 * MISRAC2012-2) Deviated Rule: 13.3 (advisory)
 * A full expression containing an increment (++) or decrement (--) operator
 * should have no other potential side effects other than that caused by the
 * increment or decrement operator.
 *
 * Reason:
 * The ++ operator is used in the context of OS_EnterKernel() for function-call
 * kernels. As function-call kernels use the comma operator, which is a
 * sequence point, to separate the increment operator from other expressions,
 * there is no undefined behavior.
*/

#include <public/Ioc_basic_types.h>
#include <private/Ioc_lockinterface.h>
#include <private/Ioc_osinterface.h>
#include <Os.h>

/*
 * !LINKSTO IOC.Function.IOC_OsSpinlock_Lock, 1
*/
ioc_return_t IOC_OsSpinlock_Lock(volatile void *lock)
{
	/* Possible diagnostic TOOLDIAG-2 <1> */
	const ioc_address_t lockIdBits = (ioc_address_t)lock;
	/* If the configuration (IOC_channelConfigurations) is correct, lockIdBits contains a valid spinlock id. */
	/* Possible diagnostic TOOLDIAG-1 <1> */
	const SpinlockIdType spinlockId = (SpinlockIdType)lockIdBits;
	StatusType status;
	TryToGetSpinlockType flag = TRYTOGETSPINLOCK_NOSUCCESS;
	ioc_return_t ret = IOC_E_NOK;

	/* Deviation MISRAC2012-1, MISRAC2012-2 */
	status = TryToGetSpinlock(spinlockId, &flag);
	if (status == E_OK)
	{
		if (flag == TRYTOGETSPINLOCK_SUCCESS)
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

/*
 * !LINKSTO IOC.Function.IOC_OsSpinlock_Unlock, 1
*/
ioc_return_t IOC_OsSpinlock_Unlock(volatile void *lock)
{
	/* Possible diagnostic TOOLDIAG-2 <1> */
	const ioc_address_t lockIdBits = (ioc_address_t)lock;
	/* If the configuration (IOC_channelConfigurations) is correct, lockIdBits contains a valid spinlock id. */
	/* Possible diagnostic TOOLDIAG-1 <1> */
	const SpinlockIdType spinlockId = (SpinlockIdType)lockIdBits;
	StatusType status;
	ioc_return_t ret = IOC_E_NOK;

	/* Deviation MISRAC2012-1, MISRAC2012-2 */
	status = ReleaseSpinlock(spinlockId);
	if (status == E_OK)
	{
		ret = IOC_E_OK;
	}
	return ret;
}

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
