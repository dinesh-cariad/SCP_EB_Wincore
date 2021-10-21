/* Os_messagequeue.h
 *
 * Definitions for an cross-core message queues.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_messagequeue.h 27282 2017-08-24 13:57:43Z olme8414 $
*/

#ifndef OS_MESSAGE_H
#define OS_MESSAGE_H

#ifndef OS_ASM

#include <Os_kernel.h>
#include <Os_cpuload_kernel.h>

#if (OS_KERNEL_TYPE == OS_MICROKERNEL)
#error "message queues are implemented by the microkernel"
#else

/* Cross-core message queue.
 *
 * Each core has an incoming message queue. The queues are designed such that if there is only
 * a single writer (2-core case) the queue is lock free.
 *
 * Each message in the queue is an opcode, up to OS_MAXPARAM parameters and a result location,
 * so it is possible for a core to request almost any defined API service on another core.
 *
 * The message queue itself is an array of n+1 message structures, where n is the largest number of
 * messages that can be queued at any time. This array is written by the sender and read by the
 * receiver.
 *
 * The message queue descriptor is a constant structure and so is only ever read by
 * both sender or receiver.
 *
 * Queue control is achieved through two indices; the os_messagequeue_t::fill and the os_messagequeue_t::empty index.
 * When the fill index is the same as the empty index the queue is empty. When incrementing (modulo queue size) the
 * fill index would make it the same as the empty index the queue is full (and the operation is not performed).
 * Thus there is always a wasted message slot in the ring buffer. Hence, the fill index marks the current head
 * of a message queue and the empty index its tail.
 *
 * The os_messagequeue_t::fill index is written by the sender and is read-only for the receiver. Likewise, the
 * os_messagequeue_t::empty index is read-only for the sender and is written by the receiver.
 * This design allows lock-free operation, when two cores are involved.
 * For more cores locking is required, because two or more cores might want to send
 * a message to the same receiving core and compete for the same entry in the receiving core's message queue.
 *
 * In case of more than two cores the function OS_GetIntercoreMsgQueueLockIdForCore() must be used to achieve
 * proper locking. It returns an ID of a spinlock which must have been successfully acquired before accessing
 * the respective message queue.
*/

/*!
 * OS_CROSSCORE_MESSAGE_QUEUE_SIZE
 *
 * The maximum number of messages which fit into a cross-core message queue.
 * Please note, that the number of usable messages is one less, because of the way the "queue empty" condition is
 * determined.
 */
#define OS_CROSSCORE_MESSAGE_QUEUE_SIZE	16

/*!
 * os_message_s
 *
 * A single message. A message queue consists of a set of instances of this type.
 * Such an instance is read-only by the receiving core and write-only by the sending core.
 * A message buffer is an array of these.
*/
typedef struct os_message_s os_message_t;
struct os_message_s
{
	os_coreid_t to;							/* The core ID of the sender core. */
	os_coreid_t from;						/* The core ID of the receiver core. */
	os_uint32_t	opcode;						/* The operation to be performed by the receiving core. */
	os_paramtype_t volatile *result;		/* Points to where the result shall be put by the receiving core. */
	os_paramtype_t parameter[OS_MAXPARAM];	/* Parameters passed to the receiving core as input to the operation. */
};

/*!
 * OS_WriteMessageResult
 *
 * Writes a message's result value and flushes the cache.
 */
#define OS_WriteMessageResult(msg, res) do {					\
		*((msg)->result) = (os_paramtype_t)(res);				\
		OS_CacheFlush((msg)->result, sizeof(os_paramtype_t));	\
	} while (0)

/*!
 * OS_AwaitXCoreReply
 *
 * This macro checks if the remote core has replied to a message.
 */
#ifndef OS_HAS_AWAIT_XCORE_REPLY
#define OS_AwaitXCoreReply(pMsgResult) do {						\
		OS_CacheInvalidate((pMsgResult), sizeof *(pMsgResult));	\
	} while (0)
#endif

/*!
 * OS_SEPARATE_INDICES_IN_MESSAGE_QUEUE
 *
 * This switch may be used to enforce the alignment of the indices in a message queue on different cache lines.
 * This might be beneficial in the case when there are only two cores, because then there is no access synchronization
 * based on locks. See also description above.
 */
#if ((OS_N_CORES <= 2) && (OS_CACHE_LINE_LEN != 0))
#define OS_SEPARATE_INDICES_IN_MESSAGE_QUEUE	1
#else
#define OS_SEPARATE_INDICES_IN_MESSAGE_QUEUE	0
#endif

/*!
 * os_messagequeue_s
 *
 * Descriptor for a message queue, which is constant and hence stored in ROM.
 * An instance contains a buffer, which holds the messages of that queue.
 *
 * Also contained are the fill and empty indices of a message queue. There may be a spacer in between these to
 * align them on different cache lines. This helps to avoid cache-contention problems.
*/
typedef struct os_messagequeue_s os_messagequeue_t;
struct os_messagequeue_s
{
	os_message_t messages[OS_CROSSCORE_MESSAGE_QUEUE_SIZE];	/* Message buffer of the queue.*/
	volatile os_size_t fill;			/* Index of the message queue's head. */
#if OS_SEPARATE_INDICES_IN_MESSAGE_QUEUE
	os_char_t cacheLineAlignSeparator[OS_CACHE_LINE_LEN];
#endif
	volatile os_size_t empty;		/* Index of the message queue's tail. */
};

/*!
 * OS_crosscoreMessageQueue_ptr
 */
extern os_messagequeue_t* const OS_crosscoreMessageQueue_ptr[OS_N_CORES_MAX];

/*!
 * OS_XC_ERROR_WaitingForReply
 *
 * "Special" error code for cross-core wait.
*/
#define OS_XC_ERROR_WaitingForReply	OS_U(0xffffffff)

/*!
 * OS_XC_OP_*
 *
 * Opcodes to identify message handlers on the receiving core.
*/
#define OS_XC_OP_UnknownCall					OS_U(0)
#define OS_XC_OP_ActivateTask					OS_U(1)
#define OS_XC_OP_GetTaskState					OS_U(2)
#define OS_XC_OP_SetEvent						OS_U(3)
#define OS_XC_OP_GetEvent						OS_U(4)
#define OS_XC_OP_GetAlarm						OS_U(5)
#define OS_XC_OP_SetRelAlarm					OS_U(6)
#define OS_XC_OP_SetAbsAlarm					OS_U(7)
#define OS_XC_OP_CancelAlarm					OS_U(8)
#define OS_XC_OP_StartScheduleTable				OS_U(9)
#define OS_XC_OP_ChainScheduleTable				OS_U(10)
#define OS_XC_OP_StopScheduleTable				OS_U(11)
#define OS_XC_OP_SyncScheduleTable				OS_U(12)
#define OS_XC_OP_SetScheduleTableAsync			OS_U(13)
#define OS_XC_OP_GetScheduleTableStatus			OS_U(14)
#define OS_XC_OP_DisableInterruptSource			OS_U(15)
#define OS_XC_OP_EnableInterruptSource			OS_U(16)
#define OS_XC_OP_GetCount						OS_U(17)
#define OS_XC_OP_AdvanceCounter					OS_U(18)
#define OS_XC_OP_StartScheduleTableSynchron		OS_U(19)
#define OS_XC_OP_TerminateApplication			OS_U(20)
#define OS_XC_OP_GetApplicationState			OS_U(21)
#define OS_XC_OP_ShutdownCore					OS_U(22)
#define OS_XC_OP_MeasureCpuLoad					OS_U(23)
#define OS_XC_OP_ReturnResult					OS_U(24)
#define OS_XC_OP_ReturnTaskState				(OS_U(OS_XC_OP_ReturnResult + 1))
#define OS_XC_OP_ReturnTick						(OS_U(OS_XC_OP_ReturnResult + 2))
#define OS_XC_OP_ReturnEvent					(OS_U(OS_XC_OP_ReturnResult + 3))
#define OS_XC_OP_ReturnScheduleTableStatus		(OS_U(OS_XC_OP_ReturnResult + 4))
#define OS_XC_OP_ReturnApplicationState			(OS_U(OS_XC_OP_ReturnResult + 5))
#define OS_XC_OP_ReturnCpuLoad					(OS_U(OS_XC_OP_ReturnResult + 6))
#define OS_N_CROSSCORE_HANDLERS					(OS_U(OS_XC_OP_ReturnResult + 7))

typedef void (*os_messagehandler_t)(os_message_t *);
extern const os_messagehandler_t OS_messageHandler[OS_N_CROSSCORE_HANDLERS];

extern void OS_SendMessage(os_coreid_t, os_uint32_t, os_paramtype_t volatile *, os_paramtype_t *);
extern os_errorresult_t OS_SendMessageWait(os_coreid_t, os_uint32_t, os_paramtype_t *);
extern void OS_ReceiveMessage(os_coreid_t);
extern void OS_InitMessageQueue(void);
extern os_uint8_t OS_CrossCoreNotifyIsr(os_isrid_t iid);

extern os_errorresult_t OS_SendActivateTask(const os_task_t *);
extern os_errorresult_t OS_SendCancelAlarm(os_alarmid_t, const os_alarm_t *);
extern os_errorresult_t OS_SendGetApplicationState(os_applicationid_t, const os_appcontext_t *, os_appstate_t *);
extern os_errorresult_t OS_SendGetAlarm(os_alarmid_t, const os_alarm_t *, os_tick_t *);
extern os_errorresult_t OS_SendGetCount(os_counterid_t, const os_counter_t *, os_tick_t *);
extern os_errorresult_t OS_SendGetEvent(os_taskid_t, const os_task_t *, os_eventmask_t *);
extern os_errorresult_t OS_SendGetTaskState(os_taskid_t, const os_task_t *, os_taskstate_t *);
extern os_errorresult_t OS_SendGetScheduleTableStatus(os_scheduleid_t, const os_schedule_t *, os_uint8_t *);
extern os_errorresult_t OS_SendSetAbsAlarm(os_alarmid_t, const os_alarm_t *, os_tick_t, os_tick_t);
extern os_errorresult_t OS_SendSetEvent(os_taskid_t, const os_task_t *, os_eventmask_t);
extern os_errorresult_t OS_SendSetRelAlarm(os_alarmid_t, const os_alarm_t *, os_tick_t, os_tick_t);
extern os_errorresult_t OS_SendStartScheduleTable(os_scheduleid_t, const os_schedule_t *, os_tick_t, os_boolean_t);
extern os_errorresult_t OS_SendStartScheduleTableSynchron(os_scheduleid_t, const os_schedule_t *);
extern os_errorresult_t OS_SendStopScheduleTable(os_scheduleid_t, const os_schedule_t *);
extern os_errorresult_t OS_SendSyncScheduleTable(os_scheduleid_t, const os_schedule_t *, os_tick_t);
extern os_errorresult_t OS_SendSetScheduleTableAsync(os_scheduleid_t, const os_schedule_t *);
extern os_errorresult_t OS_SendChainScheduleTable(os_scheduleid_t, os_scheduleid_t, const os_schedule_t *);
extern os_errorresult_t OS_SendEnableInterruptSource(os_isrid_t, const os_isr_t *);
extern os_errorresult_t OS_SendDisableInterruptSource(os_isrid_t, const os_isr_t *);
extern os_errorresult_t OS_SendTerminateApplication(os_applicationid_t, const os_appcontext_t *, os_restart_t);
extern os_errorresult_t OS_SendAdvanceCounter(os_counterid_t, const os_counter_t *, os_tick_t);
extern os_errorresult_t OS_SendMeasureCpuLoad(os_coreid_t, os_cpuloadmeasurementtype_t, os_uint8_t *);

extern void OS_RecvUnknownCall(os_message_t *);
extern void OS_RecvActivateTask(os_message_t *);
extern void OS_RecvGetTaskState(os_message_t *);
extern void OS_RecvSetEvent(os_message_t *);
extern void OS_RecvGetEvent(os_message_t *);
extern void OS_RecvGetAlarm(os_message_t *);
extern void OS_RecvSetRelAlarm(os_message_t *);
extern void OS_RecvSetAbsAlarm(os_message_t *);
extern void OS_RecvCancelAlarm(os_message_t *);
extern void OS_RecvStartScheduleTable(os_message_t *);
extern void OS_RecvChainScheduleTable(os_message_t *);
extern void OS_RecvStopScheduleTable(os_message_t *);
extern void OS_RecvSyncScheduleTable(os_message_t *);
extern void OS_RecvSetScheduleTableAsync(os_message_t *);
extern void OS_RecvGetScheduleTableStatus(os_message_t *);
extern void OS_RecvDisableInterruptSource(os_message_t *);
extern void OS_RecvEnableInterruptSource(os_message_t *);
extern void OS_RecvGetCount(os_message_t *);
extern void OS_RecvStartScheduleTableSynchron(os_message_t *);
extern void OS_RecvTerminateApplication(os_message_t *);
extern void OS_RecvAdvanceCounter(os_message_t *);
extern void OS_RecvGetApplicationState(os_message_t *);
extern void OS_RecvShutdownCore(os_message_t *);
extern void OS_RecvMeasureCpuLoad(os_message_t *);

extern void OS_RetnResult(os_message_t *);
extern void OS_RetnTaskState(os_message_t *);
extern void OS_RetnTick(os_message_t *);
extern void OS_RetnEvent(os_message_t *);
extern void OS_RetnScheduleTableStatus(os_message_t *);
extern void OS_RetnApplicationState(os_message_t *);
extern void OS_RetnCpuLoad(os_message_t *);

#endif /* OS_KERNEL_TYPE != OS_MICROKERNEL */
#endif /* OS_ASM */

#endif /* OS_MESSAGE_H */

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
