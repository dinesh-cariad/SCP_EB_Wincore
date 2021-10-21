/* Ioc_k_confighandler_unqueued.c
 *
 * This file contains the functions implementing system call handlers of unqueued interfaces of the IOC.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_k_confighandler_unqueued.c 2524 2018-02-19 12:17:30Z kosc261728 $
*/

#include <public/Ioc_basic_types.h>
#include <private/Ioc_lockinterface.h>
#include <private/Ioc_tool.h>
#include <private/Ioc_configinterface.h>
#include <private/Ioc_bufferinterface.h>
#include <private/Ioc_osinterface.h>
#include <private/Ioc_core.h>

/* IOC_SysWrite() - Checks the syscall arguments and calls the config handler.
 *
 * Parameters:
 *  - channelID:        Index of the channel configuration.
 *  - data:             Address of the data element, that shall be written to the channel.
 *
 * Return value:
 *  - IOC_E_OK:             Operation completed successfully.
 *  - IOC_E_NOK:            Some unspecified error occured.
 *  - IOC_E_SEG_FAULT:      The calling application is not allowed to read the given data.
 *  - IOC_E_ILLEGAL_CALL:   System call doesn't match the channel's configuration.
 *
 * !LINKSTO IOC.Function.IOC_SysWrite, 1
*/
ioc_return_t IOC_SysWrite(ioc_channelid_t channelID, const void *data)
{
	ioc_return_t ret = IOC_E_NOK;
	const ioc_config_t * channel_config = IOC_CH_GetChannelConfigById(channelID, IOC_UNQUEUED);

	if (channel_config != IOC_NULL)
	{
		ret = IOC_CheckSyscallSource(channel_config, data);
		if (ret == IOC_E_OK)
		{
			ret = IOC_CH_Write(channel_config, data);
		}
	}
	return ret;
}

/* IOC_SysWriteExt() - Checks the syscall arguments and calls the config handler.
 *
 * Parameters:
 *  - channelID:        Index of the channel configuration.
 *  - elements:         Array containing the data elements, which shall be written to the channel.
 *
 * Return value:
 *  - IOC_E_OK:             Operation completed successfully.
 *  - IOC_E_NOK:            Some unspecified error occured.
 *  - IOC_E_SEG_FAULT:      The calling application is not allowed to read the given data.
 *  - IOC_E_ILLEGAL_CALL:   System call doesn't match the channel's configuration.
 *
 * !LINKSTO IOC.Function.IOC_SysWriteExt, 1
*/
ioc_return_t IOC_SysWriteExt(ioc_channelid_t channelID, const ioc_extinput_t *elements)
{
	ioc_return_t ret = IOC_E_NOK;
	const ioc_config_t * channel_config = IOC_CH_GetChannelConfigById(channelID, IOC_UNQUEUED_EXT);

	if (channel_config != IOC_NULL)
	{
		ret = IOC_CheckSyscallSourceExt(channel_config, elements);
		if (ret == IOC_E_OK)
		{
			ret = IOC_CH_WriteExt(channel_config, elements);
		}
	}
	return ret;
}

/* IOC_SysReInitZero() - Checks the syscall arguments and calls the config handler.
 *
 * Parameters:
 *  - channelID:            Index of the channel configuration.
 *
 * Return value:
 *  - IOC_E_OK:             Operation completed successfully.
 *  - IOC_E_NOK:            Some unspecified error occured.
 *  - IOC_E_ILLEGAL_CALL:   System call doesn't match the channel's configuration.
 *
 * !LINKSTO IOC.Function.IOC_SysReInitZero, 1
*/
ioc_return_t IOC_SysReInitZero(ioc_channelid_t channelID)
{
	ioc_return_t ret = IOC_E_NOK;

	if (channelID < IOC_nChannels)
	{
		const ioc_config_t * const channel_config = &IOC_channelConfigurations[channelID];
		ioc_comm_semantics_t const comsem = channel_config->commSemantics;
		ioc_os_application_t const app = IOC_GetApplicationId_Kernel();

		if ((app != IOC_INVALID_OSAPPLICATION)
			&& ((IOC_GetAppAccessBit(app) & channel_config->writerConfig.accessors) != 0u)
			&& channel_config->writerConfig.trapping
			&& ((comsem == IOC_UNQUEUED) || (comsem == IOC_UNQUEUED_EXT)))
		{
			ret = IOC_CH_ReInitZero(channel_config);
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
