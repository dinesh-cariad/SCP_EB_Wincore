/* kern-sendmessagewait.c
 *
 * This file contains the implementation of the function OS_SendMessageWait().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-sendmessagewait.c 24025 2016-04-21 12:37:21Z mist9353 $
*/

#include <Os_kernel.h>
#include <Os_messagequeue.h>

/*!
 * OS_SendMessageWait()
 *
 * Sends a message to the given target core 'coreId' and waits for the result of the executed operation
 * identified by 'opcode'. The operation's parameters are passed via 'param'.
 */
os_errorresult_t OS_SendMessageWait(os_coreid_t coreId, os_uint32_t opcode, os_paramtype_t *param)
{
	os_intstatus_t is;
	volatile os_paramtype_t msgResult;

	/* Preset the result code and make it visible to receiving core.
	*/
	msgResult = OS_XC_ERROR_WaitingForReply;
	OS_CacheFlush(&msgResult, sizeof(msgResult));

	/* Send a message to receiving core.
	*/
	OS_SendMessage(coreId, opcode, &msgResult, param);

	/* Wait for the receiving core to process the message and return a result code.
	*/
	is = OS_IntEnableXcore();

	do
	{
		OS_AwaitXCoreReply(&msgResult);
	}
	while (msgResult == OS_XC_ERROR_WaitingForReply);

	OS_IntRestore(is);

	return (os_errorresult_t)msgResult;
}

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
