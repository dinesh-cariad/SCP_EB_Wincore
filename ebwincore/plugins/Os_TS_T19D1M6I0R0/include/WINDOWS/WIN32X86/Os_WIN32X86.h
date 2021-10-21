/* Os_WIN32X86.h
 *
 * Description of WINDOWS variant WIN32X86.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_WIN32X86.h 27282 2017-08-24 13:57:43Z olme8414 $
*/

/*	This file describes the WIN32X86 variant of WINDOWS.
*/

#ifndef OS_WIN32X86_H
#define OS_WIN32X86_H

#include <Os_types.h>

/** \def OS_N_CORES_MAX
 * \brief The maximum number of cores supported.
 *
 * Note, the way AUTOCORE OS was ported to Windows does not limit the maximum number of cores.
 * It's a limitation of the AUTOCORE OS itself. Future versions might support more cores.
 */
#define OS_N_CORES_MAX				4

/** \name Core ID Determination.
 * \brief Functions to determine the ID of the core on which the caller runs.
 *
 * To determine the ID of the current core, there is a macro and a helper function.
 * The macro must be defined, because it's checked in #if preprocessor statements.
 * Hence, a function alone wouldn't work.
 * The helper function does the actual work. The separation between macro and function
 * is necessary, to suppress compiler warnings. This way, the #pragmas to suppress the
 * warnings have to be in one spot only and must not be spread across the whole kernel,
 * everywhere OS_GetMyCoreId() is used.
 */
/** @{ */
#ifndef EB_STATIC_CHECK
# if (OS_N_CORES > 1)
#  define OS_GetMyCoreId()	OS_MyCoreHelper()
# endif
#else
# if (OS_N_CORES > 1)
#  define OS_GetMyCoreId()	0u
# endif
#endif
#define OS_ArchGetCoreId() 	OS_GetMyCoreId()

#if (OS_N_CORES > 1)
extern os_coreid_t OS_MyCoreHelper(void);
#endif
/** @} */

#if (OS_N_CORES > 1)
/** \def OS_XC_REPLY_DIRECTLY
 * \brief Reply directly via memory to cross-core calls.
 *
 * This macro is relevant for cross-core communication. It allows a core to reply directly
 * via memory instead of sending a reply message to the original initiator, after having executed
 * the cross-core request. This only works, when the interacting cores have mutual memory, i.e. a
 * core can access *everything* another core can also access. When this is precluded by hardware
 * a reply message must be sent to the initiator and this macro must be set to 0. Of course, this
 * message exchange also needs mutual memory, but the requirement that *everything* must be accessible
 * ceases to apply. Furthermore, cache coherency is taken care of. Hence, even when the caches are
 * incoherent this macro may be set to 1 as long as *everything* in memory is accessible by every core.
 */
#define OS_XC_REPLY_DIRECTLY		1

/** \def OS_NHARDWARELOCKS
 * \brief Number of hardware locks supported.
 *
 * There are no hardware locks.
 */
#define OS_NHARDWARELOCKS			0U

/** \name Cross-Core Notifications
 *
 * Macros to enable cross-core collaboration. They are used to notify foreign
 * core about the availability of new messages in their queues.
 *
 * The parameters \c c represents the target core to be notified.
 * Acknowledgment of a cross-core notification is done locally, e.g. by
 * resetting the respective IRQ bit. The latter is not needed on WINDOWS/WIN32X86.
 */
/** @{ */
#define OS_NotifyCore(c)	OS_WINLINTriggerInterrupt(OS_crossCoreIsrIds[(c)])

#define OS_AckNotifyCore()  \
	do {					\
	} while (0)
/** @} */

/** \brief Initializes the multi-core environment.
 *
 * \remarks This is not needed on WINDOWS/WIN32X86.
 */
#define OS_MulticoreInit()  \
    do {                    \
    } while (0)

/** \brief Enables only cross-core interrupts. */
#define OS_IntEnableXcore()									\
	OS_WINLINSetInterruptDisableLevel(						\
			OS_kernOnlyXcoreLevel,							\
			OS_WINLIN_SET_INTERRUPT_DISABLE_LEVEL_EPLICIT)

extern void OS_ArchStartCore(os_coreid_t);

#endif /* (OS_N_CORES > 1) */

#endif /* OS_WIN32X86_H */

/* Editor settings: DO NOT DELETE
 * vi: set ts=4:
*/
