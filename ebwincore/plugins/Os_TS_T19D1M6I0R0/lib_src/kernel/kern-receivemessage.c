/* kern-receivemessage.c
 *
 * This file contains the implementation of the function OS_ReceiveMessage().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-receivemessage.c 24025 2016-04-21 12:37:21Z mist9353 $
*/

#include <Os_kernel.h>
#include <Os_messagequeue.h>
#include <Os_panic.h>

static void OS_RecvDuringShutdown(const os_message_t *);

/*!
 * OS_RecvDuringShutdown
 *
 * Handles the case, when cross-core calls arrive at cores, which have been shut down.
 */
static void OS_RecvDuringShutdown(const os_message_t * msg)
{
	if (msg->result != OS_NULL)
	{
		os_paramtype_t replyParams[OS_MAXPARAM];

		replyParams[0] = (os_paramtype_t)OS_ERROR_CoreIsDown;

#if OS_XC_REPLY_DIRECTLY
		OS_WriteMessageResult(msg, replyParams[0]);
#else
		OS_SendMessage(msg->from, OS_XC_OP_ReturnResult, msg->result, replyParams);
#endif
	}
	else
	{
		(void)OS_PANIC(OS_PANIC_CrosscoreCommunicationError);
	}
}

/*!
 * OS_ReceiveMessage
 *
 * Receives and processes messages on the given core 'coreId'. This function does not return until the message
 * queue of the core 'coreId' is empty, i.e. it processes all currently pending messages.
 */
void OS_ReceiveMessage(os_coreid_t coreId)
{
	os_kerneldata_t * const kernel_data = OS_GetKernelData();
	os_messagequeue_t *mq;
	os_size_t empty;
	os_message_t *currentMsg;
	os_uint32_t opcode;

	/* Get the message queue descriptor for the receiving core.
	*/
	mq = OS_crosscoreMessageQueue_ptr[coreId];

	/* Process all messages in the queue.
	*/
	empty = mq->empty;
	OS_CacheInvalidate(&mq->fill, sizeof(mq->fill));
	while (empty != mq->fill)
	{
		/* Ensure, that this core sees the message as sent by the other core through the memory hierarchy.
		*/
		currentMsg = &mq->messages[empty];
		OS_CacheInvalidate(currentMsg, sizeof(*currentMsg));

		/* Check for valid opcode.
		*/
		opcode = currentMsg->opcode;
		if ( opcode >= OS_N_CROSSCORE_HANDLERS )
		{
			opcode = OS_XC_OP_UnknownCall;
		}

		if ( kernel_data->inFunction == OS_INSHUTDOWN )
		{
			OS_RecvDuringShutdown(currentMsg);
		}
		else
		{
			(*OS_messageHandler[opcode])(currentMsg);
		}

		/* Move to next message and make sure, that empty index update is observable by other cores.
		*/
		empty = ((empty + 1) >= OS_CROSSCORE_MESSAGE_QUEUE_SIZE) ? 0 : (empty + 1);
		mq->empty = empty;
		OS_CacheFlush(&mq->empty, sizeof(mq->empty));

		/* Check for updated fill index and repeat until queue is empty.
		*/
		OS_CacheInvalidate(&mq->fill, sizeof(mq->fill));
	}
}

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
