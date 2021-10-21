/* Ioc_m_confighandler_common.c
 *
 * This file contains the functions implementing common functions of the
 * ConfigHandler component to validate calls at runtime.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_m_confighandler_common.c 2524 2018-02-19 12:17:30Z kosc261728 $
*/

#include <public/Ioc_basic_types.h>
#include <private/Ioc_lockinterface.h>
#include <private/Ioc_tool.h>
#include <private/Ioc_configinterface.h>
#include <private/Ioc_osinterface.h>
#include <private/Ioc_core.h>

/* IOC_CH_GetChannelConfigById() - Returns a pointer to the configuration for a given channel ID.
 *
 * Parameters:
 *	channelId		- the channel ID for which the pointer is needed.
 *	comm_semantic	- the expected communication semantics of the channel
 *
 * Return value:
 *	- reference to the channel configuration if the request was valid, otherwise IOC_NULL
 *
 * !LINKSTO IOC.Function.IOC_CH_GetChannelConfigById, 1
 */
const ioc_config_t * IOC_CH_GetChannelConfigById(ioc_channelid_t channelId, ioc_comm_semantics_t comm_semantic)
{
	const ioc_config_t * channel_config_ret = IOC_NULL;

	if (channelId < IOC_nChannels)
	{
		const ioc_config_t * channel_config = &IOC_channelConfigurations[channelId];

		if (comm_semantic == channel_config->commSemantics)
		{
			channel_config_ret = channel_config;
		}
	}
	return channel_config_ret;
}

/* IOC_CH_GetAccessorTypeOfApp() - Tells, how the given application accesses the given channel.
 *
 * Parameters:
 *  app            - the ID of the calling application
 *  channel_config - the configuration of the called channel
 *
 * Return value:
 *  The way app accesses channel_config. In case the application is both, sender and receiver,
 *  receiver is favoured over sender.
 *
 * !LINKSTO IOC.Function.IOC_CH_GetAccessorTypeOfApp, 1
*/
ioc_accessor_t IOC_CH_GetAccessorTypeOfApp(const ioc_config_t *channel_config, ioc_os_application_t app)
{
	ioc_accessor_t ret = IOC_ACCESSOR_NONE;

	if (app == IOC_INVALID_OSAPPLICATION)
	{
		/* ret is already IOC_ACCESSOR_NONE. */
	}
	else
	{
		ioc_acl_t appbit = IOC_GetAppAccessBit(app);

		if ((appbit & channel_config->readerConfig.accessors) == appbit)
		{
			ret = IOC_ACCESSOR_RECEIVER;
		}
		else if ((appbit & channel_config->writerConfig.accessors) == appbit)
		{
			if (channel_config->writerConfig.trapping)
			{
				ret = IOC_ACCESSOR_TRAPPING_SENDER;
			}
			else
			{
				ret = IOC_ACCESSOR_SENDER;
			}
		}
		else
		{
			/* ret is already IOC_ACCESSOR_NONE. */
		}
	}
	return ret;
}

/* IOC_CH_CheckLengths - Checks whether the given length values are valid for VLT.
 *
 * Return value:
 *  FALSE:  Length values are invalid.
 *  TRUE :  Length values are valid.
 *
 * !LINKSTO IOC.Function.IOC_CH_CheckLengths, 1
*/
ioc_boolean_t IOC_CH_CheckLengths(const ioc_config_t *channel_config, const ioc_extinput_t *elements)
{
	ioc_uint32_t i;
	ioc_boolean_t validLengths;

	validLengths = IOC_TRUE;
	for (i = 0u; i < channel_config->numDataElements; i++)
	{
		ioc_ilength_t length = elements[i].length;

		if (length > channel_config->dataElemLengths[i])
		{
			validLengths = IOC_FALSE;
		}
		/* Sending empty data elements is not allowed. */
		if (length == 0u)
		{
			validLengths = IOC_FALSE;
		}
	}
	return validLengths;
}

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
 */
