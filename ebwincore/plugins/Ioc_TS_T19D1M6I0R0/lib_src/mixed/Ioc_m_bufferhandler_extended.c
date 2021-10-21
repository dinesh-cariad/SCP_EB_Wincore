/* Ioc_m_bufferhandler_extended.c
 *
 * This file contains the functions implementing the extended unqueued operations of the IOC.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_m_bufferhandler_extended.c 2254 2017-09-01 13:32:15Z olme8414 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 11.5 (advisory)
 *  A conversion should not be performed from pointer to void into pointer to
 *  object.
 *
 * Reason:
 *  A reinterpretation of the buffer is required, because the element's lengths
 *  are stored alongside their data.
 *
 * MISRAC2012-2) Deviated Rule: 11.5 (advisory)
 *  A conversion should not be performed from pointer to void into pointer to
 *  object.
 *
 * Reason:
 *  An address shall be determined with byte granularity. The pointer, though,
 *  has type 'void *'. Hence, a type cast for this operation is required.
 */

#include <public/Ioc_public_api.h>
#include <private/Ioc_bufferinterface.h>
#include <private/Ioc_osinterface.h>
#include <private/Ioc_configinterface.h>
#include <private/Ioc_memcpy.h>
#include <private/Ioc_extentry_common.h>

/* IOC_BH_ReadExt() - reads data from extended channel
 *
 * Parameters:
 *	channel_config	- reference to the channel configuration
 *	lengths			- After a successful call this array contains the lengths of the data elements
 *					  that have been read.
 *					  The size of the array is derived from the channel configuration.
 *	data			- Array containing the const target pointers to write each data element to.
 *					  The size of the array is derived from the channel configuration.
 *
 * Return value:
 *	- IOC_E_OK:			operation completed successfully
 *	- IOC_E_TRYAGAIN:	buffer wasn't in a consistent state
 *	- IOC_E_NOK:		some unspecified error occurred
 *
 * !LINKSTO IOC.Function.IOC_BH_ReadExt, 1
 */
ioc_return_t IOC_BH_ReadExt(const ioc_config_t *channel_config,
							ioc_ilength_t *lengths,
							void * const *data)
{
	ioc_return_t ret = IOC_E_NOK;
	ioc_uint32_t curWriteBegin;
	const ioc_bufferstate_t * const bufState = &(channel_config->writerConfig.state->bufState);

	curWriteBegin = bufState->writeBegin;
	if ( curWriteBegin == bufState->writeEnd )
	{
		IOC_HwMemoryBarrier();
		ret = IOC_ReadFromExtEntry(channel_config, channel_config->buffer, lengths, data);
		IOC_HwMemoryBarrier();
		if (curWriteBegin != bufState->writeBegin)
		{
			/* Even if IOC_ReadFromExtEntry returned an error, we overwrite it here,
			 * because IOC_ReadFromExtEntry could have failed, because the buffer was written concurrently.
			*/
			ret = IOC_E_TRYAGAIN;
		}
	}
	else
	{
		ret = IOC_E_TRYAGAIN;
	}

	return ret;
}

/* IOC_BH_WriteExt() - writes data to extended channel
 *
 * Parameters:
 *	channel_config	- reference to the channel configuration
 *	elements		- Array containing the data elements.
 *					  The size of the array is derived from the channel configuration.
 *
 * Return value:
 *	- IOC_E_OK:			operation completed successfully
 *	- IOC_E_NOK:		some unspecified error occurred
 *
 * !LINKSTO IOC.Function.IOC_BH_WriteExt, 1
 */
ioc_return_t IOC_BH_WriteExt(const ioc_config_t *channel_config, const ioc_extinput_t *elements)
{
	ioc_return_t ret = IOC_E_NOK;
	ioc_return_t lockRet = IOC_E_NOK;
	ioc_bufferstate_t * const bufState = &(channel_config->writerConfig.state->bufState);

	if (IOC_CH_CheckLengths(channel_config, elements))
	{
		lockRet = IOC_LH_AcquireLocks(channel_config);
		if (lockRet == IOC_E_OK)
		{
			ioc_uint32_t curWriteCount;
			ioc_uint32_t curDataElement;
			/* Deviation MISRAC2012-1 <1> */
			ioc_ilength_t * const lastLengths = (ioc_ilength_t *) channel_config->buffer;

			curWriteCount = bufState->writeBegin + 1;
			bufState->writeBegin = curWriteCount;
			IOC_HwMemoryBarrier();
			/* Copy Data */
			for (curDataElement = 0u; curDataElement < channel_config->numDataElements; curDataElement++)
			{
				const ioc_extinput_t *pelem = &elements[curDataElement];
				const void * const curData = pelem->data;
				if (curData != IOC_NULL)
				{
					/* The offset of the current data element within the buffer */
					const ioc_uint32_t curDeOffset = channel_config->dataElemOffsets[curDataElement];
					/* Destination of current data element */
					/* Deviation MISRAC2012-2 <1> */
					void * const curDest = &(((ioc_uint8_t *)(channel_config->buffer))[curDeOffset]);
					/* Copy data area determined by length */
					const ioc_ilength_t length = pelem->length;
					IOC_MemCpy(curDest, curData, length);
					/* Store the written length in the channel's state */
					lastLengths[curDataElement] = length;
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
	}
	else
	{
		ret = IOC_E_LENGTH;
	}
	return ret;
}

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
