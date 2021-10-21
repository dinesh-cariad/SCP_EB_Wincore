/* Ioc_m_lockhandler_lock.c
 *
 * This file contains the functions implementing the generic lock interface
 * for the IOC.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_m_lockhandler_lock.c 1817 2017-02-15 16:52:07Z mist9353 $
 */

#include <public/Ioc_basic_types.h>
#include <private/Ioc_lockinterface.h>
#include <private/Ioc_tool.h>
#include <private/Ioc_configinterface.h>
#include <private/Ioc_osinterface.h>


static ioc_return_t IOC_TakeLock(const ioc_locktype_t *, volatile void *);
static ioc_return_t IOC_DropLock(const ioc_locktype_t *, volatile void *);


static ioc_return_t IOC_TakeLock(const ioc_locktype_t *rtti, volatile void *plock)
{
	ioc_return_t ret = IOC_E_NOK;
	if (rtti->lockfn == IOC_NULL)
	{
		/* Nothing to do. */
		ret = IOC_E_OK;
	}
	else
	{
		ret = rtti->lockfn(plock);
	}
	return ret;
}
static ioc_return_t IOC_DropLock(const ioc_locktype_t *rtti, volatile void *plock)
{
	ioc_return_t ret = IOC_E_NOK;
	if (rtti->unlockfn == IOC_NULL)
	{
		/* Nothing to do. */
		ret = IOC_E_OK;
	}
	else
	{
		ret = rtti->unlockfn(plock);
	}
	return ret;
}


/*
 * !LINKSTO IOC.Function.IOC_LH_AcquireLocks, 1
*/
ioc_return_t IOC_LH_AcquireLocks(const ioc_config_t * channel_config)
{
	ioc_return_t ret = IOC_E_NOK;
	ioc_return_t fnRet;

	/* core-local lock */
	fnRet = IOC_TakeLock(&(channel_config->writerLockTypeLocalCore), channel_config->writerLockLocalCore);
	if (fnRet == IOC_E_OK)
	{
		/* Cross-core lock */
		ret = IOC_TakeLock(&(channel_config->writerLockTypeCrossCore), channel_config->writerLockCrossCore);
		if (ret != IOC_E_OK)
		{
			(void)IOC_DropLock(&(channel_config->writerLockTypeLocalCore), channel_config->writerLockLocalCore);
		}
	}
	else
	{
		ret = IOC_E_INTRALOCK_FAILED;
	}
	return ret;
}

/*
 * !LINKSTO IOC.Function.IOC_LH_ReleaseLocks, 1
*/
void IOC_LH_ReleaseLocks(const ioc_config_t * channel_config)
{
	/* Cross-core unlock */
	(void) IOC_DropLock(&(channel_config->writerLockTypeCrossCore), channel_config->writerLockCrossCore);
	/* Core-local unlock */
	(void) IOC_DropLock(&(channel_config->writerLockTypeLocalCore), channel_config->writerLockLocalCore);
}

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
