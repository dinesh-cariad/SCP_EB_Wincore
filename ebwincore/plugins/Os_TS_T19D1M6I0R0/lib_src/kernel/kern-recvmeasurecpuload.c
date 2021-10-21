/* kern-recvmeasurecpuload.c
 *
 * This file contains the function OS_RecvMeasureCpuLoad().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-recvmeasurecpuload.c 27282 2017-08-24 13:57:43Z olme8414 $
*/

#include <Os_kernel.h>
#include <Os_cpuload_kernel.h>
#include <Os_messagequeue.h>
#include <Os_panic.h>

/*!
 * OS_RecvMeasureCpuLoad
 *
 * Handles cross-core GetCpuLoad() calls.
 */
void OS_RecvMeasureCpuLoad(os_message_t * msg)
{
	os_paramtype_t replyParams[OS_MAXPARAM];

	if (msg->result != OS_NULL)
	{
		os_cpuloadmeasurementtype_t const measurementType = (os_cpuloadmeasurementtype_t)msg->parameter[0];

		replyParams[0] = (os_paramtype_t)OS_ERROR_NoError;
		replyParams[1] = msg->parameter[1];
		replyParams[2] = (os_paramtype_t)OS_MeasureCoreLoad(measurementType);

#if OS_XC_REPLY_DIRECTLY
		msg->parameter[0] = replyParams[0];
		msg->parameter[1] = replyParams[1];
		msg->parameter[2] = replyParams[2];
		OS_RetnCpuLoad(msg);
#else
		OS_SendMessage(msg->from, OS_XC_OP_ReturnCpuLoad, msg->result, replyParams);
#endif
	}
	else
	{
		(void)OS_PANIC(OS_PANIC_CrosscoreCommunicationError);
	}
}

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
