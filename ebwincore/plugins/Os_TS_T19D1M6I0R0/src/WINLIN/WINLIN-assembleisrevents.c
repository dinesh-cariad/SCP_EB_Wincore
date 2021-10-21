/* WINLIN-assembleisrevents.c
 *
 * This file contains the implementation of OS_WINLINAssembleIsrEvents().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-assembleisrevents.c 23286 2015-12-22 12:05:22Z tojo2507 $
 */

#include <Os_configuration.h>
#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/** \brief Assembles the ISR events for a core thread.
 *
 * \see OS_WINLINAssembleCoreThreadEvents()
 *
 * \param[out] eventArray		The handles of the ISRs to be observed are put
 *								into this array based on their priority. The ISR with
 *								the highest priority comes first followed by less
 *								important ISRs.
 * \param[out] associatedIsrs	The ISRs itself are put into this array. It parallels
 *								the array \a eventArray, i.e. entry \c i in \a eventArray
 *								is the Windows event which is raised to trigger the ISR
 *								in \a associatedIsrs[i].
 * \param eventArrayLength		The number of entries in \a eventArray and \a associatedIsrs.
 *								It must be at least OS_NINTERRUPTS.
 * \param considerAllIsrs		A switch to control the interrupt disable level assumed during
 *								the assembly. If set to OS_FALSE the current interrupt disable
 *								level is used. Otherwise that level is assumed to be
 *								OS_WINLIN_INTERRUPT_ENABLE_LEVEL.
 *
 * \return The number of events put into \a eventArray. The same amount of ISRs were also
 * put into \a associatedIsrs. This is the set of ISRs to observe regarding the current
 * interrupt disable level of the core on which the this function was called.
 */
os_size_t OS_WINLINAssembleIsrEvents(
		os_winlinHandle_t eventArray[],
		os_isr_t const* associatedIsrs[],
		os_size_t eventArrayLength,
		os_boolean_t considerAllIsrs)
{
	os_size_t numEvents = 0;

#if (OS_NINTERRUPTS != 0)
	/* Collect the events of those interrupts, which might be requested with the current
	 * interrupt disable level. Only those interrupts with higher priority may be served.
	 */
	{
		os_coreid_t const myCore = OS_GetMyCoreId();
		os_intstatus_t const myIntDisableLevel =
				(considerAllIsrs
				? (os_intstatus_t)OS_WINLIN_INTERRUPT_ENABLE_LEVEL
				: OS_WINLINGetCoreInterruptDisableLevel());
		os_isrid_t isrIdIter = OS_NINTERRUPTS;
		do
		{
			--isrIdIter;
			{
				os_isr_t const* const pIsr		= &OS_isrTableBase[isrIdIter];
				os_isrdynamic_t* const pIsrDyn	= pIsr->dynamic;
				if (OS_GetIsrCoreId(pIsr) == myCore)
				{
					if (pIsr->arch.prio > myIntDisableLevel)
					{
						OS_WINLINAssert(numEvents < eventArrayLength);
						eventArray[numEvents]		= OS_WINLINGetRequestedEventOfIsr(pIsrDyn);
						associatedIsrs[numEvents]	= pIsr;
						++numEvents;
					}
					else
					{
						/* We can stop here, because OS_isrTable and OS_isrDynamic are
						 * sorted in ascending order of their elements' priorities.
						 * Hence, we won't see any higher-priority interrupts further down the road.
						 */
						break;
					}
				}
			}
		}
		while (isrIdIter != 0);
	}
#else
	OS_PARAM_UNUSED(eventArray);
	OS_PARAM_UNUSED(associatedIsrs);
	OS_PARAM_UNUSED(eventArrayLength);
	OS_PARAM_UNUSED(considerAllIsrs);
#endif

	return numEvents;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
