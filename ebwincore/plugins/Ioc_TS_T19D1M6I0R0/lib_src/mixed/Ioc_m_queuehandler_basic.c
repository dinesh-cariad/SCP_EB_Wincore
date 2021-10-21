/* Ioc_m_queuehandler_basic.c
 *
 * This file is part of the component QueueHandler and contains the
 * functions implementing the basic queued interface of the IOC, handling
 * queued communication.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_m_queuehandler_basic.c 2540 2018-02-28 09:03:10Z mist9353 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 11.5 (advisory)
 *  A conversion should not be performed from pointer to void into pointer to
 *  object.
 *
 * Reason:
 *  The buffer of the channel is accessed with byte granularity. The buffer,
 *  though, has the type 'void *'. Hence, a type cast is required.
 */

#include <public/Ioc_basic_types.h>
#include <private/Ioc_memcpy.h>
#include <private/Ioc_configinterface.h>
#include <private/Ioc_lockinterface.h>
#include <private/Ioc_tool.h>
#include <private/Ioc_queueinterface.h>
#include <private/Ioc_osinterface.h>

/* IOC_QH_Receive() - Receive writes the least recently written data element of the communication channel with the
 *						provided configuration to the provided target pointer.
 *
 * Parameters:
 *	channel_config	- reference to the channel configuration
 *	data			- Pointer to memory location to write the data element to.
 *
 * Return value:
 *	- IOC_E_OK:			operation completed successfully
 *	- IOC_E_NO_DATA:	queue is empty
 *	- IOC_E_LOST_DATA:	at least one send operation after the previous receive operation returned IOC_E_LIMIT
 *	- IOC_E_NOK:		internal error
 *
 * !LINKSTO IOC.Function.IOC_QH_Receive, 1
 */
ioc_return_t IOC_QH_Receive(const ioc_config_t * channel_config, void *data)
{
	ioc_return_t ret = IOC_E_NOK;
	const ioc_queueendstate_t * const senderState = &(channel_config->writerConfig.state->queueState);
	ioc_queueendstate_t * const receiverState = &(channel_config->readerConfig.state->queueState);
	const ioc_uint32_t queueLength = channel_config->numQueueEntries;
	const ioc_uint32_t queueEntrySize = channel_config->bufferSize / channel_config->numQueueEntries;
	const ioc_uint32_t receiverIdx = receiverState->idx;

	if (senderState->idx == receiverIdx)
	{
		/* Queue Empty */
		ret = IOC_E_NO_DATA;
	}
	else
	{
		/* Deviation MISRAC2012-1 <1> */
		ioc_uint8_t * const curBuffer = channel_config->buffer;
		const void * bufferPtr;
		const ioc_uint32_t recAccessIndex = receiverIdx % queueLength;
		bufferPtr = &(curBuffer[(recAccessIndex * queueEntrySize)]);
		IOC_HwMemoryBarrier();
		IOC_MemCpy(data, bufferPtr, channel_config->dataElemLengths[0]);
		IOC_HwMemoryBarrier();
		receiverState->idx = (receiverIdx + 1u) % (2u * queueLength);
		if (receiverState->lostDataCounter == senderState->lostDataCounter)
		{
			ret = IOC_E_OK;
		}
		else
		{
			receiverState->lostDataCounter = senderState->lostDataCounter;
			ret = IOC_E_LOST_DATA;
		}
	}
	return ret;
}

/*
 * IOC_QH_Send() appends the provided data element to the queue of the channel with the provided configuration
 *
 * If necessary, this function also tries to get the configured
 * local-core and/or cross-core locks.
 *
 * Parameters:
 *	channel_config	- reference to the channel configuration
 *	data			- Pointer to memory location to write the data element to.
 *
 * Return value:
 *	- IOC_E_OK:			operation completed successfully
 *	- IOC_E_LIMIT:		queue is full
 *	- IOC_E_NOK:		internal error
 *
 * !LINKSTO IOC.Function.IOC_QH_Send, 1
*/
ioc_return_t IOC_QH_Send(const ioc_config_t * channel_config, const void *data)
{
	ioc_return_t ret = IOC_E_NOK;
	ioc_return_t lockRet = IOC_E_NOK;
	ioc_queueendstate_t * const senderState = &(channel_config->writerConfig.state->queueState);
	const ioc_queueendstate_t * const receiverState = &(channel_config->readerConfig.state->queueState);
	const ioc_uint32_t queueLength = channel_config->numQueueEntries;
	const ioc_uint32_t queueEntrySize = channel_config->bufferSize / channel_config->numQueueEntries;

	lockRet = IOC_LH_AcquireLocks(channel_config);
	if (lockRet == IOC_E_OK)
	{
		const ioc_uint32_t senderIdx = senderState->idx;
		const ioc_uint32_t recCheckIndex = (receiverState->idx + queueLength) % (2u * queueLength);
		const ioc_uint32_t sendAccessIndex = senderIdx % queueLength;

		if (senderIdx == recCheckIndex)
		{
			/* Queue Full */
			senderState->lostDataCounter++;
			ret = IOC_E_LIMIT;
		}
		else
		{
			/* Deviation MISRAC2012-1 <1> */
			ioc_uint8_t * const curBuffer = channel_config->buffer;
			void * bufferPtr = &(curBuffer[(sendAccessIndex * queueEntrySize)]);

			IOC_HwMemoryBarrier();
			IOC_MemCpy(bufferPtr, data, channel_config->dataElemLengths[0]);
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
	return ret;
}

/*
 * IOC_QH_EmptyQueue() Empties the queue
 *
 * Parameters:
 *	channel_config	- reference to the channel configuration
 *	isSender		- TRUE if the function is called from the sender side
 *
 * Return value:
 *	- IOC_E_OK:			operation completed successfully
 *
 * !LINKSTO IOC.Function.IOC_QH_EmptyQueue, 1
*/
ioc_return_t IOC_QH_EmptyQueue(const ioc_config_t * channel_config, ioc_boolean_t isSender)
{
	ioc_queueendstate_t * const senderState = &(channel_config->writerConfig.state->queueState);
	ioc_queueendstate_t * const receiverState = &(channel_config->readerConfig.state->queueState);
	if (isSender)
	{
		senderState->idx = receiverState->idx;
		senderState->lostDataCounter = receiverState->lostDataCounter;
	}
	else
	{
		receiverState->idx = senderState->idx;
		receiverState->lostDataCounter = senderState->lostDataCounter;
	}
	return IOC_E_OK;
}

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
