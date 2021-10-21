/* Ioc_u_confighandler_unqueued.c
 *
 * This file contains the functions implementing the unqueued interface of the IOC.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_u_confighandler_unqueued.c 1851 2017-02-17 17:17:25Z mist9353 $
*/

#include <public/Ioc_basic_types.h>
#include <public/Ioc_public_api.h>
#include <private/Ioc_lockinterface.h>
#include <private/Ioc_tool.h>
#include <private/Ioc_bufferinterface.h>
#include <private/Ioc_osinterface.h>
#include <private/Ioc_configinterface.h>

static ioc_return_t IOC_DoWrite(ioc_channelid_t, const ioc_config_t *, const void *);
static ioc_return_t IOC_DoReInitZero(ioc_channelid_t, const ioc_config_t *);


static ioc_return_t IOC_DoWrite(ioc_channelid_t channelID, const ioc_config_t *channel_config, const void *data)
{
	ioc_return_t ret = IOC_E_NOK;
	ioc_boolean_t const trapping = channel_config->writerConfig.trapping;
	/* We don't want to spin around in kernel mode or with locked local-core locks,
	 * thus if a lock can't be acquired, we have to go all the way back to the user mode and try again.
	*/
	do
	{
		if (trapping)
		{
			ioc_statusandvalue_t syscallRet;
			syscallRet = IOC_SyscallWrite(channelID, data);
			ret = (ioc_return_t) syscallRet.requestedValue;
		}
		else
		{
			ret = IOC_CH_Write(channel_config, data);
		}
	} while (ret == IOC_E_TRYAGAIN);
	return ret;
}

static ioc_return_t IOC_DoReInitZero(ioc_channelid_t channelID, const ioc_config_t *channel_config)
{
	ioc_return_t ret = IOC_E_NOK;
	ioc_boolean_t const trapping = channel_config->writerConfig.trapping;
	/* We don't want to spin around in kernel mode or with locked local-core locks,
	 * thus if a lock can't be acquired, we have to go all the way back to the user mode and try again.
	*/
	do
	{
		if (trapping)
		{
			ioc_statusandvalue_t syscallRet;
			syscallRet = IOC_SyscallReInitZero(channelID);
			ret = (ioc_return_t) syscallRet.requestedValue;
		}
		else
		{
			ret = IOC_CH_ReInitZero(channel_config);
		}
	} while (ret == IOC_E_TRYAGAIN);
	return ret;
}


/*
 * !LINKSTO IOC.API.IOC_Read, 1
*/
ioc_return_t IOC_Read(ioc_channelid_t channelID, void *data)
{
	ioc_return_t ret = IOC_E_NOK;
	const ioc_config_t * channel_config = IOC_CH_GetChannelConfigById(channelID, IOC_UNQUEUED);
	if (channel_config != IOC_NULL)
	{
		do
		{
			ret = IOC_CH_Read(channel_config, data);
		} while (ret == IOC_E_TRYAGAIN);
	}
	return ret;
}

/*
 * !LINKSTO IOC.API.IOC_Write, 1
*/
ioc_return_t IOC_Write(ioc_channelid_t channelID, const void *data)
{
	ioc_return_t ret = IOC_E_NOK;
	const ioc_config_t * channel_config = IOC_CH_GetChannelConfigById(channelID, IOC_UNQUEUED);
	if (channel_config != IOC_NULL)
	{
		ret = IOC_DoWrite(channelID, channel_config, data);
	}
	return ret;
}

/*
 * !LINKSTO IOC.API.IOC_ReadExt, 1
*/
ioc_return_t IOC_ReadExt(	ioc_channelid_t channelID,
							ioc_ilength_t *lengths,
							void * const *data)
{
	ioc_return_t ret = IOC_E_NOK;
	const ioc_config_t * channel_config = IOC_CH_GetChannelConfigById(channelID, IOC_UNQUEUED_EXT);
	if (channel_config != IOC_NULL)
	{
		do
		{
			ret = IOC_CH_ReadExt(channel_config, lengths, data);
		} while (ret == IOC_E_TRYAGAIN);
	}
	return ret;
}

/*
 * !LINKSTO IOC.API.IOC_WriteExt, 1
*/
ioc_return_t IOC_WriteExt(ioc_channelid_t channelID, const ioc_extinput_t *elements)
{
	ioc_return_t ret = IOC_E_NOK;
	const ioc_config_t * channel_config = IOC_CH_GetChannelConfigById(channelID, IOC_UNQUEUED_EXT);
	if (channel_config != IOC_NULL)
	{
		ioc_boolean_t const trapping = channel_config->writerConfig.trapping;
		/* We don't want to spin around in kernel mode or with locked local-core locks,
		 * thus if a lock can't be acquired, we have to go all the way back to the user mode and try again.
		*/
		do
		{
			if (trapping)
			{
				ioc_statusandvalue_t syscallRet = IOC_SyscallWriteExt(channelID, elements);
				ret = (ioc_return_t) syscallRet.requestedValue;
			}
			else
			{
				ret = IOC_CH_WriteExt(channel_config, elements);
			}
		} while (ret == IOC_E_TRYAGAIN);
	}
	return ret;
}

/*
 * !LINKSTO IOC.API.IOC_ReInit, 1
*/
ioc_return_t IOC_ReInit(ioc_channelid_t channelID)
{
	ioc_return_t ret = IOC_E_NOK;
	if (channelID < IOC_nChannels)
	{
		const ioc_config_t *channel_config = &IOC_channelConfigurations[channelID];
		const void *initData = channel_config->initData;
		ioc_comm_semantics_t comsem = channel_config->commSemantics;

		if (comsem == IOC_UNQUEUED)
		{
			if (initData == IOC_NULL)
			{
				ret = IOC_DoReInitZero(channelID, channel_config);
			}
			else
			{
				ret = IOC_DoWrite(channelID, channel_config, initData);
			}
		}
		else if (comsem == IOC_UNQUEUED_EXT)
		{
			ret = IOC_DoReInitZero(channelID, channel_config);
		}
		else
		{
			ret = IOC_E_ILLEGAL_CALL;
		}
	}
	return ret;
}

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
