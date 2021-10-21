/* kern-startupchecksqueue.c
 *
 * This file contains the OS_StartupChecksQueue function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-startupchecksqueue.c 27926 2017-11-21 17:19:25Z mist9353 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: PointlessComparison
 *   Condition is always true.
 *
 * Reason: These warnings depend on the number of configured cores.
 *   Only in case of a single core configuration,
 *   these comparisons can be evaluated at compile time.
 *
 * TOOLDIAG-2) Possible diagnostic: StatementIsUnreachable
 *   statement not reached.
 *
 * Reason:
 *   These warnings depend on the OS configuration and library optimization.
 *   This statement can be reached if ISRs are allowed to take resources.
*/

#include <Os_kernel.h>
#include <Os_taskqueue.h>
#include <Os_panic.h>

#include <memmap/Os_mm_code_begin.h>

/* Calculate the Most Significant Bit of a priority word */
#define OS_CLZ_MSBIT	(os_clzword_t)((os_clzword_t) 1U << (OS_CLZWORD_NBITS-1))

/* MISRA-C checkers want prototypes for static helpers */
static os_result_t OS_CheckQueueNum(void);
static os_result_t OS_CheckEntryNum(void);
static os_result_t OS_CheckMSBits(void);
static os_result_t OS_CheckSlotNumberForPrio(os_prio_t p);

/* Checks that we have the correct number of priority queues
*/
static os_result_t OS_CheckQueueNum(void)
{
	os_coreid_t const myCoreId = OS_GetMyCoreId();
	os_unsigned_t n;
	os_prio_t resprio;
	os_prio_t highprio = 0;
	const os_task_t *ts;
	const os_resource_t *rs;
	os_taskid_t nCorelocalTasks = 0;
	os_result_t result = OS_E_OK;

	for ( n = 0; n < OS_nTasks; n++ )
	{
		ts = &OS_taskTableBase[n];

		/* Possible diagnostic TOOLDIAG-1 <1> */
		if ( OS_GetTaskCoreId(ts) == myCoreId )
		{
			/* runPrio >= queuePrio, so we don't need to look at queuePrio
			*/
			if ( highprio < ts->runPrio )
			{
				highprio = ts->runPrio;
			}
			++nCorelocalTasks;
		}
	}

	if ( nCorelocalTasks == 0 )
	{
		if ( OS_nPriorities[myCoreId] != 0 )
		{
			result = OS_PANIC(OS_PANIC_SCHK_IncorrectPriorityQueueCount);
		}
	}
	else
	{
		for ( n = 0; n < OS_nResources; n++ )
		{
			rs = &OS_resourceTableBase[n];

			/* if this resource is present on our core */
			/* Possible diagnostic TOOLDIAG-1 <1> */
			if (OS_GetResourceCoreId(rs) == myCoreId)
			{
				resprio = rs->prio;

				if ( OS_IsIsrPrio(resprio) )
				{
					/* Possible diagnostic TOOLDIAG-2 <+1> */
					resprio = OS_maxPrio[myCoreId];
				}

				if ( highprio < resprio )
				{
					highprio = resprio;
				}
			}
		}

		if ( OS_nPriorities[myCoreId] != (highprio+1) )
		{
			result = OS_PANIC(OS_PANIC_SCHK_IncorrectPriorityQueueCount);
		}
	}

	return result;
}

static os_result_t OS_CheckSlotNumberForPrio(os_prio_t p)
{
	os_unsigned_t n;
	os_prio_t resprio;
	const os_resource_t *rs;
	const os_task_t *ts;
	os_int_t nslots = 0;
	os_int_t nextra = 0;
	const os_priorityqueue_t *pq;
	os_result_t result = OS_E_OK;
	os_coreid_t const myCoreId = OS_GetMyCoreId();
	pq = OS_GetPriorityQueueOfCore(myCoreId, p);

	for ( n = 0; n < OS_nTasks; n++ )
	{
		ts = &OS_taskTableBase[n];

		/* Possible diagnostic TOOLDIAG-1 <1> */
		if ( OS_GetTaskCoreId(ts) == myCoreId )
		{
			if ( ts->queuePrio == p )
			{
				/* For each task, we need as many slots as its total number of activations.
				 * maxAct+1 because maxAct is the number of "pending" activations.
				*/
				nslots += ts->maxAct + 1;
			}
			else
			if ( ts->runPrio == p )
			{
				/* We need an extra slot if there's a task whose priority can be raised to this one
				*/
				nextra = 1;
			}
			else
			{
				/* MISRA-C */
			}
		}
	}

	/* To find out if we need an extra slot we might need to look at the resources.
	 * But only until we know the answer.
	 * Loop doesn't execute at all if we already know the answer.
	 * Don't even bother for p==0 because a task can never be raised to priority 0.
	*/
	if ( p > 0 )
	{
		for ( n = 0; (nextra == 0) && (n < OS_nResources); n++ )
		{
			rs = &OS_resourceTableBase[n];

			/* Possible diagnostic TOOLDIAG-1 <1> */
			if (OS_GetResourceCoreId(rs) == myCoreId)
			{
				resprio = rs->prio;

				if ( OS_IsIsrPrio(resprio) )
				{
					/* Possible diagnostic TOOLDIAG-2 <+1> */
					resprio = OS_maxPrio[OS_GetMyCoreId()];
				}

				if ( resprio == p )
				{
					nextra = 1;
				}
			}
		}
	}

	if ( pq->nEntries != (nslots + nextra) )
	{
		result = OS_PANIC(OS_PANIC_SCHK_IncorrectNumberOfEntriesInPriorityQueue);
	}

	return result;
}

/* Check that every priority queue has the correct number of entries
*/
static os_result_t OS_CheckEntryNum(void)
{
	os_coreid_t const myCoreId = OS_GetMyCoreId();
	os_int_t p;
	os_int_t p2;
	os_int_t totalslots = 0;
	os_boolean_t firstfound = OS_FALSE;
	os_boolean_t nextfound;
	os_boolean_t lastfound = OS_FALSE;
	const os_priorityqueue_t *pq;
	os_priorityqueueentry_t *nextslot;
	os_priorityqueueentry_t *lastslot = OS_GetPrioritySlotOfCore(myCoreId, OS_nPrioritySlots[myCoreId]);
	os_result_t result = OS_E_OK;
	os_result_t tempres;

	for ( p = 0; p < OS_nPriorities[myCoreId]; p++ )
	{
		pq = OS_GetPriorityQueueOfCore(myCoreId, p);

		totalslots += pq->nEntries;

		if ( pq->nEntries != 0 )
		{
			if ( pq->nEntries == 1 )
			{
				if ( pq->dynamic != OS_NULL )
				{
					result = OS_PANIC(OS_PANIC_SCHK_SingleEntryQueueWithDynamicVariables);
				}
			}
			else
			{
				if ( (OS_configMode & OS_CC2) == 0 )
				{
					result = OS_PANIC(OS_PANIC_SCHK_ConformanceClass1ButMultipleEntryQueue);
				}

				if ( pq->dynamic == OS_NULL )
				{
					result = OS_PANIC(OS_PANIC_SCHK_NoDynamicVariablesInMultipleEntryQueue);
				}
			}

			tempres = OS_CheckSlotNumberForPrio((os_prio_t) p);
			if (tempres != OS_E_OK)
			{
				result = tempres;
			}

			if ( pq->queue == OS_GetPrioritySlotOfCore(myCoreId, 0))
			{
				if ( firstfound )
				{
					result = OS_PANIC(OS_PANIC_SCHK_OverlappingPriorityQueuesFound);
				}
				else
				{
					firstfound = OS_TRUE;
				}
			}

			nextslot = &pq->queue[pq->nEntries];

			if ( nextslot >= lastslot )
			{
				if ( nextslot > lastslot )
				{
					result = OS_PANIC(OS_PANIC_SCHK_PriorityQueueEntriesOutOfBounds);
				}
				else
				if ( lastfound )
				{
					result = OS_PANIC(OS_PANIC_SCHK_OverlappingPriorityQueuesFound);
				}
				else
				{
					lastfound = 1;
				}
			}
			else
			{
				/* Each queue that's not the last one should have a successor
				*/
				nextfound = OS_FALSE;

				for ( p2 = 0; (p2 < OS_nPriorities[myCoreId]) && (nextfound == OS_FALSE); p2++ )
				{
					if ( (p2 != p) && (OS_GetPriorityQueueOfCore(myCoreId, p2)->queue == nextslot) )
					{
						nextfound = OS_TRUE;
					}
				}

				if ( nextfound == OS_FALSE )
				{
					result = OS_PANIC(OS_PANIC_SCHK_GapInPriorityQueuesFound);
				}
			}
		}
	}

	if ( (OS_nPriorities[myCoreId] != 0) && (totalslots != OS_nPrioritySlots[myCoreId]) )
	{
		result = OS_PANIC(OS_PANIC_SCHK_PrioritySlotsArrayHasWrongSize);
	}

	return result;
}

/* Checks master/slave bits.
*/
static os_result_t OS_CheckMSBits(void)
{
	os_coreid_t const myCoreId = OS_GetMyCoreId();
	os_int_t p;
	const os_priorityqueue_t *pq;
	os_clzword_t expectedMasterBit = OS_CLZ_MSBIT;
	os_clzword_t expectedSlaveBit = OS_CLZ_MSBIT;
	os_clzword_t *expectedSlave = OS_GetSlavePrioWord(myCoreId, 0);
	os_result_t result = OS_E_OK;

	/* To check the master/slave bits we look at the priorities backwards */
	for ( p = OS_nPriorities[myCoreId]; p > 0; p-- )
	{
		pq = OS_GetPriorityQueueOfCore(myCoreId, p - 1);

		if ( (pq->masterBit != expectedMasterBit) )
		{
			result = OS_PANIC(OS_PANIC_SCHK_IncorrectMasterPriorityBitAssignment);
		}

		if ( (pq->slaveBit != expectedSlaveBit) ||
			 (pq->slave != expectedSlave) )
		{
			result = OS_PANIC(OS_PANIC_SCHK_IncorrectSlavePriorityBitAssignment);
		}

		/* Shift the expected slave bit right. The AND ensures we don't get any sign-bit
		 * propagation
		*/
		expectedSlaveBit = (expectedSlaveBit >> 1) & ~OS_CLZ_MSBIT;

		/* If the bit disappears, start again in the next slave slot
		*/
		if ( expectedSlaveBit == 0 )
		{
			expectedMasterBit = (expectedMasterBit >> 1) & ~OS_CLZ_MSBIT;
			expectedSlave++;
			expectedSlaveBit = OS_CLZ_MSBIT;
		}
	}

	return result;
}

/* OS_StartupChecksQueue
 *
 * This function performs a variety of consistency and sanity checks on the priority queue configuration
 * data to try to ensure that the generator is behaving itself as far as we can tell.
*/
os_result_t OS_StartupChecksQueue(void)
{
	os_result_t result = OS_E_OK;
	os_result_t tempres;

	/* Check that we have the correct number of priority queues
	*/
	tempres = OS_CheckQueueNum();
	if (tempres != OS_E_OK)
	{
		result = tempres;
	}
	/* Check that every priority queue has the correct number of entries
	*/
	tempres = OS_CheckEntryNum();
	if (tempres != OS_E_OK)
	{
		result = tempres;
	}
	/* Check the master/slave bits
	*/
	tempres = OS_CheckMSBits();
	if (tempres != OS_E_OK)
	{
		result = tempres;
	}

	return result;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
