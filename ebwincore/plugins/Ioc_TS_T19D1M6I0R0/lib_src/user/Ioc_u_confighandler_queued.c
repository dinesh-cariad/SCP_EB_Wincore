/* Ioc_u_confighandler_queued.c
 *
 * This file contains the functions implementing the queued interface of the IOC.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_u_confighandler_queued.c 1860 2017-02-20 12:26:31Z mist9353 $
*/

#include <public/Ioc_public_api.h>
#include <public/Ioc_basic_types.h>
#include <private/Ioc_lockinterface.h>
#include <private/Ioc_tool.h>
#include <private/Ioc_queueinterface.h>
#include <private/Ioc_osinterface.h>
#include <private/Ioc_configinterface.h>

/*
 * !LINKSTO IOC.API.IOC_Receive, 1
*/
ioc_return_t IOC_Receive(ioc_channelid_t channelID, void *data)
{
	ioc_return_t ret = IOC_E_NOK;
	const ioc_config_t * channel_config = IOC_CH_GetChannelConfigById(channelID, IOC_QUEUED);

	if (channel_config != IOC_NULL)
	{
		ret = IOC_CH_Receive(channel_config, data);
	}
	return ret;
}

/*
 * !LINKSTO IOC.API.IOC_Send, 1
*/
ioc_return_t IOC_Send(ioc_channelid_t channelID, const void *data)
{
	ioc_return_t ret = IOC_E_NOK;
	const ioc_config_t * channel_config = IOC_CH_GetChannelConfigById(channelID, IOC_QUEUED);

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
				ioc_statusandvalue_t syscallRet;

				syscallRet = IOC_SyscallSend(channelID, data);
				ret = (ioc_return_t) syscallRet.requestedValue;
			}
			else
			{
				ret = IOC_CH_Send(channel_config, data);
			}
		} while (ret == IOC_E_TRYAGAIN);
	}
	return ret;
}

/*
 * !LINKSTO IOC.API.IOC_ReceiveExt, 1
*/
ioc_return_t IOC_ReceiveExt(ioc_channelid_t channelID, ioc_ilength_t lengths[], void * const *data)
{
	ioc_return_t ret = IOC_E_NOK;
	const ioc_config_t * channel_config = IOC_CH_GetChannelConfigById(channelID, IOC_QUEUED_EXT);

	if (channel_config != IOC_NULL)
	{
		ret = IOC_CH_ReceiveExt(channel_config, lengths, data);
	}
	return ret;
}

/*
 * !LINKSTO IOC.API.IOC_SendExt, 1
*/
ioc_return_t IOC_SendExt(ioc_channelid_t channelID, const ioc_extinput_t *elements)
{
	ioc_return_t ret = IOC_E_NOK;
	const ioc_config_t * channel_config = IOC_CH_GetChannelConfigById(channelID, IOC_QUEUED_EXT);

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
				ioc_statusandvalue_t syscallRet = IOC_SyscallSendExt(channelID, elements);

				ret = (ioc_return_t) syscallRet.requestedValue;
			}
			else
			{
				ret = IOC_CH_SendExt(channel_config, elements);
			}
		} while (ret == IOC_E_TRYAGAIN);
	}
	return ret;
}

/*
 * !LINKSTO IOC.API.IOC_EmptyQueue, 1
*/
ioc_return_t IOC_EmptyQueue(ioc_channelid_t channelID)
{
	ioc_return_t ret = IOC_E_NOK;

	if (channelID < IOC_nChannels)
	{
		const ioc_config_t * channel_config = &IOC_channelConfigurations[channelID];

		if ((channel_config->commSemantics == IOC_QUEUED)
			|| (channel_config->commSemantics == IOC_QUEUED_EXT))
		{
			const ioc_os_application_t app = IOC_GetApplicationId_User();
			const ioc_accessor_t acct = IOC_CH_GetAccessorTypeOfApp(channel_config, app);

			switch (acct)
			{
			case IOC_ACCESSOR_RECEIVER:
				ret = IOC_QH_EmptyQueue(channel_config, IOC_FALSE);
				break;
			case IOC_ACCESSOR_SENDER:
				ret = IOC_QH_EmptyQueue(channel_config, IOC_TRUE);
				break;
			case IOC_ACCESSOR_TRAPPING_SENDER:
				{
					ioc_statusandvalue_t syscallRet;
					syscallRet = IOC_SyscallEmptyQueue(channelID);
					ret = (ioc_return_t) syscallRet.requestedValue;
				}
				break;
			default:
				/* Call from wrong application. */
				ret = IOC_E_ILLEGAL_CALL;
				break;
			}
		}
	}

	return ret;
}
/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
