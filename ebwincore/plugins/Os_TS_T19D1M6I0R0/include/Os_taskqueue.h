/* Os_taskqueue.h
 *
 * This file defines the new priority queue mechanism for tasks.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_taskqueue.h 27282 2017-08-24 13:57:43Z olme8414 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 8.11 (advisory)
 * When an array with external linkage is declared, its size should be
 * explicitly specified.
 *
 * Reason:
 * Array size and initialization are depending on configuration whereas the
 * array declaration is used by configuration-independent library code.
 */

#ifndef OS_TASKQUEUE_H
#define OS_TASKQUEUE_H

#include <Os_types.h>
#include <Os_tool.h>

#ifndef OS_ASM

/* Select CC1 optimisations a way to avoid stupid compiler warnings about
 * evaluating undefined macros - even though a correct C evaluation of the expression
 * should not evaluate the undefined macro!
*/
#if OS_USE_OPTIMIZATION_OPTIONS

#if (OS_SCHEDULING_ALGORITHM==OS_CLZ_QUEUE)

/* If the old queueing algorithm is in use we don't check the library configuration but just
 * supply a default.
*/
#ifndef OS_LCFG_TASKQUEUE_CC1
#error "OS_LCFG_TASKQUEUE_CC1 is not defined. Check the lib-config file!"
#endif

#ifndef OS_LCFG_TASKQUEUE_SMALL
#error "OS_LCFG_TASKQUEUE_SMALL is not defined. Check the lib-config file!"
#endif

#endif

#ifndef OS_LCFG_TASKQUEUE_CC1
#define OS_LCFG_TASKQUEUE_CC1	0
#endif

#ifndef OS_LCFG_TASKQUEUE_SMALL
#define OS_LCFG_TASKQUEUE_SMALL	0
#endif

#else

#define OS_LCFG_TASKQUEUE_CC1	0
#define OS_LCFG_TASKQUEUE_SMALL	0

#endif

/* What we store in the queues.
 * FOR FUTURE USE: this could be defined as const os_task_t *
*/
typedef os_taskid_t os_priorityqueueentry_t;

/* os_priorityqueue_t and os_priorityqueuedynamic_t : the queue structure types
 *
 * The generator generates an array of static queue descriptors, one for each priority in the system.
 * Queues with more than 1 entry need a dynamic set of state variables too.
 *
 * The static structure (os_priorityqueue_t):
 *	dynamic		- The state variables for this queue. OS_NULL if the queue has only one entry.
 *	queue		- The first element of the fifo buffer: points somewhere in OS_prioritySlot
 *	slave		- The address of the slave CLZ-word in which this queue is represented
 *	slaveBit	- The bit in the slave CLZ-word that represents this queue
 *	masterBit	- The bit in the master CLZ-word that represents this queue
 *	nEntries	- The number of entries in the fifo
 *
 * In the dynamic structure:
 *	head		- The index of the next task in the queue
 *	next		- The index of the next free slot in the queue
*/
typedef struct os_priorityqueue_s os_priorityqueue_t;
typedef struct os_priorityqueuedynamic_s os_priorityqueuedynamic_t;

struct os_priorityqueue_s
{
	os_priorityqueuedynamic_t *dynamic;		/* The state variables for this queue */
	os_priorityqueueentry_t *queue;			/* The fifo buffer: points somewhere in OS_prioritySlot */
	os_clzword_t *slave;					/* The slave clzword in which this queue is present */
	os_clzword_t slaveBit;					/* The bit to set in the slave clzword */
	os_clzword_t masterBit;					/* The bit to set in the master clzword */
	os_int_t nEntries;						/* The number of entries in the fifo */
};

struct os_priorityqueuedynamic_s
{
	os_int_t head;		/* The position of the first task in the queue */
	os_int_t next;		/* The position of the next free slot in the queue */
};

extern const os_int_t OS_nPriorities[OS_N_CORES_MAX];
extern const os_int_t OS_nPrioritySlots[OS_N_CORES_MAX];

/*!
 * OS_GetPriorityQueueOfCore
 *
 * Returns a pointer to the priority queue for priority 'prio' of the given core 'coreId'.
 */
#if (OS_N_CORES == 1)
/* Deviation MISRAC2012-1 */
extern os_priorityqueue_t const OS_priorityQueue[];
#define OS_GetPriorityQueueOfCore(coreId, prio)	(&OS_priorityQueue[(prio)])
#else
/* Deviation MISRAC2012-1 */
extern os_priorityqueue_t const * const OS_priorityQueue_ptr[];
#define OS_GetPriorityQueueOfCore(coreId, prio)	(&(OS_priorityQueue_ptr[(coreId)])[(prio)])
#endif

/*!
 * OS_GetPrioritySlotOfCore
 *
 * Returns a pointer to the priority slot with the given index 'idx' on core 'coreId'.
 */
#if (OS_N_CORES == 1)
/* Deviation MISRAC2012-1 */
extern os_priorityqueueentry_t
		OS_SECTION_ATTRIB_PRE_DECL(OS_SECTIONNAME(.bss, OS_INITCORE, prioritySlot))
		OS_prioritySlot[]
		OS_SECTION_ATTRIB_POST_DECL(OS_SECTIONNAME(.bss, OS_INITCORE, prioritySlot));
#define OS_GetPrioritySlotOfCore(coreId, idx)	(&OS_prioritySlot[(idx)])
#else
/* Deviation MISRAC2012-1 */
extern os_priorityqueueentry_t * const OS_prioritySlot_ptr[];
#define OS_GetPrioritySlotOfCore(coreId, idx)	(&((OS_prioritySlot_ptr[(coreId)])[(idx)]))
#endif

/*!
 * OS_GetSlavePrioWord
 *
 * Returns a pointer to the slave prio word with the given index 'idx' on the core 'coreId'.
 */
#if (OS_N_CORES == 1)
/* Deviation MISRAC2012-1 */
extern os_clzword_t
		OS_SECTION_ATTRIB_PRE_DECL(OS_SECTIONNAME(.bss, OS_INITCORE, slavePrioWord))
		OS_slavePrioWord[]
		OS_SECTION_ATTRIB_POST_DECL(OS_SECTIONNAME(.bss, OS_INITCORE, slavePrioWord));
#define OS_GetSlavePrioWord(coreId, idx)	(&OS_slavePrioWord[(idx)])
#else
extern os_clzword_t * const OS_slavePrioWord_ptr[OS_N_CORES_MAX];
#define OS_GetSlavePrioWord(coreId, idx)	(&(OS_slavePrioWord_ptr[(coreId)])[(idx)])
#endif

/* OS_PriorityQueueIsFull() returns true if the priority queue is (fully) occupied.
 *
 * Optimisations:
 *  if all queues have a single entry, return false if the slot doesn't contain OS_NULLTASK
*/
#if OS_LCFG_TASKQUEUE_CC1
#define OS_PriorityQueueIsFull(index, q)	(*OS_GetPrioritySlotOfCore(OS_GetMyCoreId(), (index)) != OS_NULLTASK)
#else
#define	OS_PriorityQueueIsFull(index, q)	(((q)->dynamic == OS_NULL)					\
												? ((q)->queue[0] != OS_NULLTASK)	\
												: ((q)->queue[(q)->dynamic->next] != OS_NULLTASK))
#endif

/* OS_Get_PriorityQueueDynamic() returns the dynamic queue variables for a multiple-task queue
 *
 * Optimisations:
 *	if all queues have a single entry, return OS_NULL
*/
#if OS_LCFG_TASKQUEUE_CC1
#define OS_Get_PriorityQueueDynamic(q)				OS_NULL
#else
#define OS_Get_PriorityQueueDynamic(q)				((q)->dynamic)
#endif

/* OS_Get_PriorityQueueHead() returns the task at the head of the queue
 * OS_Get_PriorityQueueHeadAfterRemoval() returns the task at the head of the queue after a task has been removed
 *
 * Optimisations:
 *  if all queues have a single entry, return the content of the priority slot at [index], or OS_NULLTASK in the
 *  "after removal" case
 *  In the "after removal" case for the a CC2 system, return OS_NULLTASK if there's only one task at the given
 *  priority.
*/
#if OS_LCFG_TASKQUEUE_CC1
#define OS_Get_PriorityQueueHead(index, q, qd)		(*OS_GetPrioritySlotOfCore(OS_GetMyCoreId(), (index)))
#define OS_Get_PriorityQueueHeadAfterRemoval(q, qd)	OS_NULLTASK
#else
#define OS_Get_PriorityQueueHead(index, q, qd)		(((qd)==OS_NULL) ? (q)->queue[0] : (q)->queue[(qd)->head])
#define OS_Get_PriorityQueueHeadAfterRemoval(q, qd)	(((qd)==OS_NULL) ? OS_NULLTASK : (q)->queue[(qd)->head])
#endif

/* OS_Append_PriorityQueue() adds the task to the back of the queue
 *
 * Optimisations:
 *  if all queues have a single entry, the priority slot at [index] is set to the task
*/
#if OS_LCFG_TASKQUEUE_CC1

#define OS_Append_PriorityQueue(index, q, qd, t)	*OS_GetPrioritySlotOfCore(OS_GetMyCoreId(), (index)) = (t)
#define OS_Prepend_PriorityQueue(index, q, qd, t)	*OS_GetPrioritySlotOfCore(OS_GetMyCoreId(), (index)) = (t)

#else /* !OS_LCFG_TASKQUEUE_CC1 */

#define OS_Append_PriorityQueue(index, q, qd, t) 	\
	do {											\
		if ( (qd) == OS_NULL )						\
		{											\
			(q)->queue[0] = (t);					\
		}											\
		else										\
		{											\
			(q)->queue[(qd)->next] = (t);			\
			(qd)->next++;							\
			if ( (qd)->next >= (q)->nEntries )		\
			{										\
				(qd)->next = 0;						\
			}										\
		}											\
	} while (0)
#define OS_Prepend_PriorityQueue(index, q, qd, t) 	\
	do {											\
		if ( (qd) == OS_NULL )						\
		{											\
			(q)->queue[0] = (t);					\
		}											\
		else										\
		{											\
			if ( (qd)->head <= 0 )					\
			{										\
				(qd)->head = (q)->nEntries - 1;		\
			}										\
			else									\
			{										\
				(qd)->head--;						\
			}										\
			(q)->queue[(qd)->head] = (t);			\
		}											\
	} while (0)

#endif /* OS_LCFG_TASKQUEUE_CC1 */

/* OS_Remove_PriorityQueueHead() removes the task from the head of the queue
 *
 * Optimisations:
 *  if all queues have a single entry, the priority slot at [index] is set to OS_NULLTASK
*/
#if OS_LCFG_TASKQUEUE_CC1

#define OS_Remove_PriorityQueueHead(index, q, qd)	*OS_GetPrioritySlotOfCore(OS_GetMyCoreId(), (index)) = OS_NULLTASK

#else /* !OS_LCFG_TASKQUEUE_CC1 */

#define OS_Remove_PriorityQueueHead(index, q, qd)	\
	do {											\
		if ( (qd) == OS_NULL )						\
		{											\
			(q)->queue[0] = OS_NULLTASK;			\
		}											\
		else										\
		{											\
			(q)->queue[(qd)->head] = OS_NULLTASK;	\
			(qd)->head++;							\
			if ( (qd)->head >= (q)->nEntries )		\
			{										\
				(qd)->head = 0;						\
			}										\
		}											\
	} while (0)

#endif /* OS_LCFG_TASKQUEUE_CC1 */

/* OS_Set_PriorityBits() sets the appropriate bits in the priority words for the given queue
 * OS_Clr_PriorityBits() clears the appropriate bits in the priority words for the given queue
 * OS_FindSlaveIndex() finds the index n the slave array by counting the leading zeros in the master
 *
 * Optimisations:
 *  if there are fewer task priorities than bits in an os_clzword_t there is only one slave word and the
 *	master word is never used.
*/
#if OS_LCFG_TASKQUEUE_SMALL

#define OS_Set_PriorityBits(q)										\
	do {															\
		*OS_GetSlavePrioWord(OS_GetMyCoreId(), 0) |= q->slaveBit;	\
	} while (0)

#define OS_Clr_PriorityBits(q)										\
	do {															\
		*OS_GetSlavePrioWord(OS_GetMyCoreId(), 0) &= ~q->slaveBit;	\
	} while (0)

#define OS_FindSlaveIndex()			0
#define OS_GetTopLevelPrioWord()	*OS_GetSlavePrioWord(OS_GetMyCoreId(), 0)
#define OS_SLAVEWORDNOTOCCUPIED(i)	0

#else /* !OS_LCFG_TASKQUEUE_SMALL */

#define OS_Set_PriorityBits(q)									\
	do {														\
		*(q)->slave |= (q)->slaveBit;							\
		OS_GetKernelData()->masterPrioWord |= (q)->masterBit;	\
	} while (0)

#define OS_Clr_PriorityBits(q)										\
	do {															\
		*(q)->slave &= ~(q)->slaveBit;								\
		if ( *(q)->slave == 0 )										\
		{															\
			OS_GetKernelData()->masterPrioWord &= ~(q)->masterBit;	\
		}															\
	} while (0)

#define OS_FindSlaveIndex()			OS_CLZ(OS_GetKernelData()->masterPrioWord);
#define OS_GetTopLevelPrioWord()	OS_GetKernelData()->masterPrioWord
#define OS_SLAVEWORDNOTOCCUPIED(i)	OS_DEVSANITYCHECK(*OS_GetSlavePrioWord(OS_GetMyCoreId(), (i)) == 0)

#endif /* OS_LCFG_TASKQUEUE_SMALL */

extern os_priorityqueueentry_t OS_FindHighestTask(void);
extern void OS_EnqueueTask(const os_task_t *, os_prio_t);
extern void OS_InsertTask(const os_task_t *, os_prio_t);
extern void OS_DequeueTask(const os_task_t *, os_prio_t);

#endif /* OS_ASM */

#endif /* OS_TASKQUEUE_H */

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
