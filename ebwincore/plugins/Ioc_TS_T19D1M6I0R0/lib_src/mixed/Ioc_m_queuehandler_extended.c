/* Ioc_m_queuehandler_extended.c
 *
 * This file is part if the component QueueHandler and contains the
 * functions implementing the extended queued interface of the IOC, handling
 * queued communication.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_m_queuehandler_extended.c 2256 2017-09-01 14:11:21Z olme8414 $
*/
/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 11.5 (advisory)
 *  A conversion should not be performed from pointer to void into pointer to
 *  object.
 *
 * Reason:
 *  The buffer of a channel shall be accessed with byte granularity. The pointer
 *  to this buffer has the type 'void *'. To enable the access, a type cast is
 *  required.
 *
 * MISRAC2012-2) Deviated Rule: 11.5 (advisory)
 *  A conversion should not be performed from pointer to void into pointer to
 *  object.
 *
 * Reason:
 *  A reinterpretation of the buffer is required, because the element's lengths
 *  are stored alongside their data.
 *
 * MISRAC2012-3) Deviated Rule: 11.5 (advisory)
 *  A conversion should not be performed from pointer to void into pointer to
 *  object.
 *
 * Reason:
 *  An address shall be determined with byte granularity, which is located
 *  inside the buffer. The pointer used in this calculation has the type
 *  'void *', though. To enable the calculation, a type cast to a non-void
 *  type is required.
*/

#include <public/Ioc_basic_types.h>
#include <private/Ioc_memcpy.h>
#include <private/Ioc_configinterface.h>
#include <private/Ioc_lockinterface.h>
#include <private/Ioc_tool.h>
#include <private/Ioc_queueinterface.h>
#include <private/Ioc_osinterface.h>
#include <private/Ioc_extentry_common.h>

/*
 * IOC_QH_ReceiveExt() - receives data from extended channels
 *
 * Parameters:
 *	channel_config	- reference to the channel configuration
 *	lengths			- After a successful call this array contains the lengths of the data elements
 *					  that have been received.
 *					  The size of the array is derived from the channel configuration.
 *	data			- Array containing the const target pointers to write each data element to.
 *					  The size of the array is derived from the channel configuration.
 *
 * Return value:
 *	- IOC_E_OK:			operation completed successfully
 *	- IOC_E_LIMIT:		queue is full
 *	- IOC_E_NOK:		internal error
 *
 * !LINKSTO IOC.Function.IOC_QH_ReceiveExt, 1
 */
ioc_return_t IOC_QH_ReceiveExt(const ioc_config_t * channel_config, ioc_ilength_t *lengths, void * const * data)
{
	ioc_return_t ret = IOC_E_NOK;
	const ioc_queueendstate_t * const senderState = &(channel_config->writerConfig.state->queueState);
	ioc_queueendstate_t * const receiverState = &(channel_config->readerConfig.state->queueState);
	const ioc_uint32_t queueLength = channel_config->numQueueEntries;
	const ioc_uint32_t receiverIdx = receiverState->idx;

	if (senderState->idx == receiverIdx)
	{
		/* Queue Empty */
		ret = IOC_E_NO_DATA;
	}
	else
	{
		/* Deviation MISRAC2012-1 <1> */
		ioc_uint8_t * const totalBuffer = channel_config->buffer;
		const ioc_uint32_t queueEntrySize = channel_config->bufferSize / channel_config->numQueueEntries;
		const ioc_uint32_t recAccessIndex = receiverIdx % queueLength;
		void * const entry = &totalBuffer[recAccessIndex * queueEntrySize];

		IOC_HwMemoryBarrier();
		ret = IOC_ReadFromExtEntry(channel_config, entry, lengths, data);
		IOC_HwMemoryBarrier();

		/* If reading the entry failed, it is corrupted,
		 * so we discard it and return the error code returned by IOC_ReadFromExtEntry.
		*/
		receiverState->idx = (receiverIdx + 1u) % (2u * queueLength);
		if ((ret == IOC_E_OK) && (receiverState->lostDataCounter != senderState->lostDataCounter))
		{
			receiverState->lostDataCounter = senderState->lostDataCounter;
			ret = IOC_E_LOST_DATA;
		}
	}
	return ret;
}

/*
 * IOC_QH_SendExt() SendExt appends the provided data elements to the queue of the communication channel with the
 *					provided configuration. Supports variable length transmission and multiple data elements.
 *
 * If necessary, the function also tries to get the configured
 * local-core and/or cross-core locks.
 *
 * Parameters:
 *	channel_config	- reference to the channel configuration
 *	elements		- Array containing the data elements.
 *					  The size of the array is derived from the channel configuration.
 *
 * Return value:
 *	- IOC_E_OK:			operation completed successfully
 *	- IOC_E_LIMIT:		queue is full
 *	- IOC_E_NOK:		internal error
 *
 * !LINKSTO IOC.Function.IOC_QH_SendExt, 1
 */
ioc_return_t IOC_QH_SendExt(const ioc_config_t * channel_config, const ioc_extinput_t *elements)
{
	ioc_return_t ret = IOC_E_NOK;
	ioc_return_t lockRet = IOC_E_NOK;
	ioc_queueendstate_t * const senderState = &(channel_config->writerConfig.state->queueState);
	const ioc_queueendstate_t * const receiverState = &(channel_config->readerConfig.state->queueState);
	const ioc_uint32_t queueLength = channel_config->numQueueEntries;

	if (IOC_CH_CheckLengths(channel_config, elements))
	{

		lockRet = IOC_LH_AcquireLocks(channel_config);
		if (lockRet == IOC_E_OK)
		{
			const ioc_uint32_t senderIdx = senderState->idx;
			const ioc_uint32_t recCheckIndex = (receiverState->idx + queueLength) % (2u * queueLength);

			if (senderIdx == recCheckIndex)
			{
				/* Queue Full */
				senderState->lostDataCounter++;
				ret = IOC_E_LIMIT;
			}
			else
			{
				/* Deviation MISRAC2012-1 <1> */
				ioc_uint8_t * const totalBuffer = channel_config->buffer;
				const ioc_uint32_t queueEntrySize = channel_config->bufferSize / channel_config->numQueueEntries;
				const ioc_uint32_t sendAccessIndex = senderIdx % queueLength;
				void * const entry = &totalBuffer[sendAccessIndex * queueEntrySize];
				/* Deviation MISRAC2012-2 <1> */
				ioc_ilength_t * const entryLengths = (ioc_ilength_t *)entry;
				ioc_uint32_t curDataElement;

				IOC_HwMemoryBarrier();
				/* Copy Data */
				for (curDataElement = 0u; curDataElement < channel_config->numDataElements; curDataElement++)
				{
					const ioc_extinput_t *pelem = &elements[curDataElement];
					/* The offset of the current data element within the queue entry */
					const ioc_uint32_t curDeOffset = channel_config->dataElemOffsets[curDataElement];
					/* Destination of current data element */
					/* Deviation MISRAC2012-3 <1> */
					void *curDest  = &((ioc_uint8_t *)entry)[curDeOffset];
					/* Store the current data element */
					IOC_MemCpy(curDest, pelem->data, pelem->length);
					/* Store the length of the current data element */
					entryLengths[curDataElement] = pelem->length;
				}
				IOC_HwMemoryBarrier();
				senderState->idx = (senderIdx + 1u) % (2u * queueLength);
				ret = IOC_E_OK;
			}
			IOC_LH_ReleaseLocks(channel_config);
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
