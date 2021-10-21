/* Ioc_m_bufferhandler_basic.c
 *
 * This file contains the functions implementing the simple unqueued operations of the IOC.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_m_bufferhandler_basic.c 2254 2017-09-01 13:32:15Z olme8414 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 11.5 (advisory)
 *  A conversion should not be performed from pointer to void into pointer to
 *  object.
 *
 * Reason:
 *  The pointed to memory is initialized. To enable this, C language type
 *  other than 'void' is required. Hence, a type cast is required.
 *
 * MISRAC2012-2) Deviated Rule: 11.5 (advisory)
 *  A conversion should not be performed from pointer to void into pointer to
 *  object.
 *
 * Reason:
 *  A reinterpretation of the buffer is required, because the element's lengths
 *  are stored alongside their data.
 */

#include <public/Ioc_public_api.h>
#include <private/Ioc_bufferinterface.h>
#include <private/Ioc_osinterface.h>
#include <private/Ioc_configinterface.h>
#include <private/Ioc_memcpy.h>

/* IOC_BH_Read() - Read writes the most recently written data element of the communication channel with the provided
 *					configuration to the provided target pointer.
 *
 * Parameters:
 *	channel_config	- reference to the channel configuration
 *	data			- Pointer to memory location to write the data element to.
 *
 * Return value:
 *	- IOC_E_OK:			operation completed successfully
 *	- IOC_E_TRYAGAIN:	buffer wasn't in a consistent state
 *
 * !LINKSTO IOC.Function.IOC_BH_Read, 1
 */
ioc_return_t IOC_BH_Read(const ioc_config_t * channel_config, void *data)
{
	ioc_return_t ret = IOC_E_NOK;
	ioc_uint32_t curWriteBegin;
	const ioc_bufferstate_t * const bufState = &(channel_config->writerConfig.state->bufState);

	curWriteBegin = bufState->writeBegin;
	if (curWriteBegin == bufState->writeEnd)
	{
		void * const curBuffer = channel_config->buffer;
		IOC_HwMemoryBarrier();
		IOC_MemCpy(data, curBuffer, channel_config->dataElemLengths[0u]);
		IOC_HwMemoryBarrier();
		if (curWriteBegin == bufState->writeBegin)
		{
			ret = IOC_E_OK;
		}
		else
		{
			ret = IOC_E_TRYAGAIN;
		}
	}
	else
	{
		ret = IOC_E_TRYAGAIN;
	}

	return ret;

}

/* IOC_BH_Write() - Write writes the provided data element to the buffer of the communication channel with the
 *					provided configuration.
 *
 * Parameters:
 *	channel_config	- reference to the channel configuration
 *	data			- Pointer to memory location to read the data element from
 *
 * Return value:
 *	- IOC_E_OK:			operation completed successfully
 *	- IOC_E_NOK:		some unspecified error occured
 *
 * !LINKSTO IOC.Function.IOC_BH_Write, 1
 */
ioc_return_t IOC_BH_Write(const ioc_config_t * channel_config, const void *data)
{
	ioc_return_t ret = IOC_E_NOK;
	ioc_return_t lockRet = IOC_E_NOK;

	lockRet = IOC_LH_AcquireLocks(channel_config);
	if (lockRet == IOC_E_OK)
	{
		ioc_bufferstate_t * const bufState = &(channel_config->writerConfig.state->bufState);
		void * const curBuffer = channel_config->buffer;
		ioc_uint32_t curWriteCount;

		curWriteCount = bufState->writeBegin + 1;
		bufState->writeBegin = curWriteCount;
		IOC_HwMemoryBarrier();
		IOC_MemCpy(curBuffer, data, channel_config->dataElemLengths[0u]);
		IOC_HwMemoryBarrier();
		bufState->writeEnd = curWriteCount;
		IOC_LH_ReleaseLocks(channel_config);
		ret = IOC_E_OK;
	}
	else
	{
		ret = lockRet;
	}
	return ret;
}

/* IOC_BH_ReInitZero() - Fills the buffer of the communication channel with zeros.
 *
 * Parameters:
 *	channel_config	- reference to the channel configuration
 *
 * Return value:
 *	- IOC_E_OK:			operation completed successfully
 *	- IOC_E_NOK:		some unspecified error occured
 *
 * !LINKSTO IOC.Function.IOC_BH_ReInitZero, 1
 */
ioc_return_t IOC_BH_ReInitZero(const ioc_config_t * channel_config)
{
	ioc_return_t ret = IOC_E_NOK;
	ioc_return_t lockRet = IOC_E_NOK;

	lockRet = IOC_LH_AcquireLocks(channel_config);
	if (lockRet == IOC_E_OK)
	{
		ioc_bufferstate_t * const bufState = &(channel_config->writerConfig.state->bufState);
		void * const curBuffer = channel_config->buffer;
		ioc_uint8_t *buf;
		ioc_uint32_t size, i;
		ioc_uint32_t curWriteCount;

		curWriteCount = bufState->writeBegin;
		curWriteCount++;
		bufState->writeBegin = curWriteCount;

		IOC_HwMemoryBarrier();

		/* Fill the buffer with zeros.
		 * This could be optimized like MemCpy. However, it isn't considered crucial for performance.
		*/
		/* Deviation MISRAC2012-1 <1> */
		buf = (ioc_uint8_t *)curBuffer;
		size = channel_config->bufferSize;
		for (i = 0u; i < size; i++)
		{
			buf[i] = 0u;
		}

		/* Reset the size of each data element to its maximum size.
		*/
		if (channel_config->commSemantics == IOC_UNQUEUED_EXT)
		{
			const ioc_uint32_t nElems = channel_config->numDataElements;
			/* Deviation MISRAC2012-2 <1> */
			ioc_ilength_t * const lastLengths = (ioc_ilength_t *) channel_config->buffer;
			for (i = 0; i < nElems; i++)
			{
				lastLengths[i] = channel_config->dataElemLengths[i];
			}
		}

		IOC_HwMemoryBarrier();

		bufState->writeEnd = curWriteCount;
		IOC_LH_ReleaseLocks(channel_config);
		ret = IOC_E_OK;
	}
	else
	{
		ret = lockRet;
	}
	return ret;
}

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
