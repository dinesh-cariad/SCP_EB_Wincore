/* Ioc_k_confighandler_queued.c
 *
 * This file contains the functions implementing system call handlers of the IOC,
 * handling queued communication.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_k_confighandler_queued.c 1112 2016-09-30 13:03:31Z stpo8218 $
*/

#include <public/Ioc_basic_types.h>
#include <private/Ioc_lockinterface.h>
#include <private/Ioc_tool.h>
#include <private/Ioc_configinterface.h>
#include <private/Ioc_queueinterface.h>
#include <private/Ioc_osinterface.h>


/* IOC_SysSend() - Checks the syscall arguments and calls the config handler.
 *
 * !LINKSTO IOC.Function.IOC_SysSend, 1
*/
ioc_return_t IOC_SysSend(ioc_channelid_t channelID, const void *data)
{
	ioc_return_t ret = IOC_E_NOK;
	const ioc_config_t * channel_config = IOC_CH_GetChannelConfigById(channelID, IOC_QUEUED);

	if (channel_config != IOC_NULL)
	{
		ret = IOC_CheckSyscallSource(channel_config, data);
		if (ret == IOC_E_OK)
		{
			ret = IOC_CH_Send(channel_config, data);
		}
	}
	return ret;
}

/* IOC_SysSendExt() - Checks the syscall arguments and calls the config handler.
 *
 * !LINKSTO IOC.Function.IOC_SysSendExt, 1
*/
ioc_return_t IOC_SysSendExt(ioc_channelid_t channelID, const ioc_extinput_t *elements)
{
	ioc_return_t ret = IOC_E_NOK;
	const ioc_config_t * channel_config = IOC_CH_GetChannelConfigById(channelID, IOC_QUEUED_EXT);

	if (channel_config != IOC_NULL)
	{
		ret = IOC_CheckSyscallSourceExt(channel_config, elements);
		if (ret == IOC_E_OK)
		{
			ret = IOC_CH_SendExt(channel_config, elements);
		}
	}
	return ret;
}

/* IOC_SysEmptyQueue() - Checks the syscall arguments and calls the config handler.
 *
 * If the calling application is both, sender and receiver of the given channel, access is denied.
 * Sender side calls to IOC_QH_EmptyQueue must not overlap with receive operations.
 * The user-space handler will, therefore, always favor the receiver side.
 * This means, if the given application is a receiver, the syscall wasn't triggered by IOC_EmptyQueue
 * and, hence, may be rejected.
 *
 * !LINKSTO IOC.Function.IOC_SysEmptyQueue, 1
*/
ioc_return_t IOC_SysEmptyQueue(ioc_channelid_t channelID)
{
	ioc_return_t ret = IOC_E_NOK;
	const ioc_config_t *channel_config = IOC_NULL;

	if (channelID < IOC_nChannels)
	{
		const ioc_config_t *channel_config_unchecked = &IOC_channelConfigurations[channelID];

		if ((channel_config_unchecked->commSemantics == IOC_QUEUED)
			|| (channel_config_unchecked->commSemantics == IOC_QUEUED_EXT))
		{
			channel_config = channel_config_unchecked;
		}
	}

	if (channel_config != IOC_NULL)
	{
		const ioc_os_application_t app = IOC_GetApplicationId_Kernel();

		if (IOC_CH_GetAccessorTypeOfApp(channel_config, app) == IOC_ACCESSOR_TRAPPING_SENDER)
		{
			ret = IOC_CH_EmptyQueue(channel_config, IOC_TRUE);
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
