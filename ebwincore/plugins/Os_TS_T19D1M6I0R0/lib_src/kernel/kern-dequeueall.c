/* kern-dequeueall.c
 *
 * This file contains the OS_DequeueAll function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-dequeueall.c 27926 2017-11-21 17:19:25Z mist9353 $
*/
/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: UnusedVariable
 *   Variable is set but never used.
 *
 * Reason: Not an issue, variable will be used if scalability class 2 is used.
 *
 *
 * TOOLDIAG-2) Possible diagnostic: PointlessComparison
 *   Condition is always true.
 *
 * Reason: This warning depends on the OS configuration and library optimization.
 *   Only if every queue has just one entry (OS_LCFG_TASKQUEUE_CC1),
 *   OS_Get_PriorityQueueHeadAfterRemoval will always yield OS_NULLTASK (at compile time).
 *
 * TOOLDIAG-3) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   If there aren't any tasks which have multiple activations,
 *   the respective if-condition can be evaluated at compile time.
 *   Otherwise this statement is reachable.
*/

#include <Os_kernel.h>
#if (OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE)
#include <Os_taskqueue.h>
#endif

#include <memmap/Os_mm_code_begin.h>

#if (OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE)
/* NextIndex() - local macro to calculate x+1 modulo (n+1)
*/
#define NextIndex(x,n)	( ((x)>=(n)) ? (os_int_t) 0 : ((x)+1) )

/* MISRA-C checkers want prototypes for static helpers */
static void OS_DequeueFromBasePrioQueue(os_taskprio_t, os_priorityqueueentry_t);

/* Helper to dequeue all entries for the task in the queue at its base priority
*/
static void OS_DequeueFromBasePrioQueue(os_taskprio_t prio, os_priorityqueueentry_t qent)
{
	const os_priorityqueue_t *pq = OS_GetPriorityQueueOfCore(OS_GetMyCoreId(), prio);
	os_priorityqueuedynamic_t * const pqd = OS_Get_PriorityQueueDynamic(pq);
	os_int_t dest;
	os_int_t src;
	os_int_t limit;
	os_int_t maxidx;
	os_priorityqueueentry_t newTask;

	/* No need to test pqd for being OS_NULL: unless the generator is playing games the
	 * multiple activations status of this task ensures that this queue has more than
	 * one entry and therefore must have dynamic variables.
	 *
	 * NOTE: if there's only one task per priority, it would be possible to do multiple
	 * activations just using the activation counter of the task, but that would mean
	 * a special case in DoTerminateTask (don't dequeue if multiple activations and
	 * one task per prio), so it probably isn't worth the effort.
	 * HOWEVER, if that were ever made the case, we'd need test for it here.
	*/
	if ( pqd->head != pqd->next )
	{
		/* Queue is not empty. Note: we know the queue isn't full because we just
		 * removed the task from the very beginning.
		 * 'dest' is the index we're copying *to*
		 * 'src' is the index we're copying *from*
		*/
		dest = pqd->head;
		limit = pqd->next;
		maxidx = pq->nEntries - 1;

		/* First, skip past all entries at the head that don't get deleted. These
		 * do not move.
		 * Loop terminates when we find an entry to delete, or there are no
		 * more entries.
		*/
		while ( (dest != limit) && (pq->queue[dest] != qent) )
		{
			dest = NextIndex(dest, maxidx);
		}

		/* If there are more entries, then there's at least one to delete.
		*/
		if ( dest != limit )
		{
			/* Find the first entry afterwards that we don't want to delete.
			 * As we go along, erase all "to delete" entries by writing NULLTASK
			 * over them. The loop here executes at least once because
			 * it erases the entry at dest.
			*/
			src = dest;
			while ( (src != limit) && (pq->queue[src] == qent) )
			{
				pq->queue[src] = OS_NULLTASK;
				src = NextIndex(src, maxidx);
			}

			/* Now copy all entries up towards the head of the queue and erase them as we go.
			 * Skip over any entries that are to be deleted.
			*/
			while ( src != limit )
			{
				if ( pq->queue[src] != qent )
				{
					/* This entry is to be kept. Move it to the dest and then advance the dest index.
					*/
					pq->queue[dest] = pq->queue[src];
					dest = NextIndex(dest, maxidx);
				}

				/* Erase the entry; it has been either moved or deleted.
				 * Then move the src index to the next candidate for moving or erasing.
				*/
				pq->queue[src] = OS_NULLTASK;
				src = NextIndex(src, maxidx);
			}
		}

		/* Update the 'next free' index in the queue dynamic struct.
		 * dest is the index of the first free slot.
		*/
		pqd->next = dest;

		/* If the queue becomes empty, clear its bit(s) in the priority-word(s) */
		/* Possible diagnostic TOOLDIAG-2 <+2> */
		newTask = OS_Get_PriorityQueueHeadAfterRemoval(pq, pqd);
		if ( newTask == OS_NULLTASK )
		{
			/* No more tasks at this priority, so we need to clear the appropriate bits in the
			 * the master/slave words and if the dequeued task was at the queue head, recalculate
			 * the new queue head from scratch.
			*/
			OS_Clr_PriorityBits(pq);
		}
	}
}

/*!
 * OS_DequeueAll
 *
 * All activations of the specified task are removed from all the task queues.
 * There's no point looking for which queues the task might be found in because
 * that would make the code a bit convoluted.
 * So we just look at:
 *  - the head of all queues from the task's current priority down to its base priority
 *  - all entries of the queue at the task's base priority.
 *
 * OS_DequeueAll() must be called with interrupts locked
*/

void OS_DequeueAll(const os_task_t *tp)
{
	os_coreid_t const myCoreId = OS_GetMyCoreId();
	os_taskprio_t prio;
	os_taskprio_t oldPrio;
	os_taskprio_t queuePrio = tp->queuePrio;
	const os_priorityqueue_t *pq;
	/* Possible diagnostic TOOLDIAG-1 <1> */
	os_priorityqueuedynamic_t *pqd;
	os_priorityqueueentry_t qent = tp->taskId;
	os_priorityqueueentry_t newTask;

	prio = OS_IsIsrPrio(tp->dynamic->prio) ? OS_maxPrio[myCoreId] : (os_taskprio_t) tp->dynamic->prio;
	oldPrio = prio;
	pq = OS_GetPriorityQueueOfCore(myCoreId, prio);


	/* Remove the task from the head of all the queues from its current priority down to its
	 * base priority. Only those queues where it is at the head, of course.
	*/
	while ( (prio >= queuePrio) && (oldPrio >= prio) )
	{
		/* Possible diagnostic TOOLDIAG-1 <1> */
		pqd = OS_Get_PriorityQueueDynamic(pq);

		if ( OS_Get_PriorityQueueHead(prio, pq, pqd) == qent )
		{
			OS_Remove_PriorityQueueHead(prio, pq, pqd);

			/* If the queue becomes empty, clear its bit(s) in the priority-word(s).
			*/
			newTask = OS_Get_PriorityQueueHeadAfterRemoval(pq, pqd);
			/* Possible diagnostic TOOLDIAG-2 <1> */
			if ( newTask == OS_NULLTASK )
			{
				/* No more tasks at this priority, so we need to clear the appropriate bits in the
				 * the master/slave words and if the dequeued task was at the queue head, recalculate
				 * the new queue head from scratch.
				*/
				OS_Clr_PriorityBits(pq);
			}
		}

		oldPrio = prio;
		pq--;
		prio--;
	}

	/* Now we examine the base priority queue for all further activations.
	 * There's no need to do this if the task's MAXACTIVATIONS is 1.
	*/
	if ( OS_TaskHasMultipleActivations(tp) )
	{
		/* Possible diagnostic TOOLDIAG-3 <+1> */
		OS_DequeueFromBasePrioQueue(queuePrio, qent);
	}

	/* Clear up the current task's pending activation count.
	*/
	tp->dynamic->nAct = 0;

	/* Recalculate the queue head from scratch. This is probably cheaper
	 * than recalculating as we go along.
	*/
	newTask = OS_FindHighestTask();
	OS_GetKernelData()->taskQueueHead = (newTask == OS_NULLTASK) ? OS_NULL : &OS_taskTableBase[newTask];
}

#else /* OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE */

/*!
 * OS_DequeueAll
 *
 * All activations of the specified task are removed from the task queue.
 * We don't need to go through the activations in order (from the task's
 * point of view) - we just walk the list and remove anything that's in
 * the task's range. We test for all activations removed to avoid walking
 * through all the lower priority tasks in the list. Our task can't possibly
 * be down there!
 *
 * OS_DequeueAll() must be called with interrupts locked
 *
 * CHECK: NOPARSE
*/
void OS_DequeueAll(const os_task_t *tp)
{
	os_tasklink_t * const taskActivations = OS_GetTaskActivations();
	os_taskdynamic_t *td = tp->dynamic;
	os_tasklink_t next = taskActivations[0];
	os_tasklink_t prev = 0;
	os_tasklink_t curr;
	os_tasklink_t min = tp->linkBase;
	os_tasklink_t max = min + (os_tasklink_t)tp->maxAct;
	os_unsigned_t nAct = (os_unsigned_t)td->nAct + 1;

	while ( (nAct > 0) && (next != 0) )
	{
		curr = next;
		next = taskActivations[next];

		if ( (curr >= min) && (curr <= max) )
		{
			taskActivations[prev] = next;
			taskActivations[curr] = 0;
			nAct--;
		}
		else
		{
			prev = curr;
		}
	}

	td->nAct = 0;
	td->cAct = 0;

	OS_GetKernelData()->taskQueueHead = OS_GetTaskPtrs(taskActivations[0]);
}

/* CHECK: PARSE */
#endif /* OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE */

#include <memmap/Os_mm_code_end.h>


/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
