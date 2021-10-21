/* Os_configuration_WINLIN.c
 *
 * This file provides architecture-specific kernel configuration data
 * for WINLIN.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_configuration_WINLIN.c 27660 2017-10-18 14:07:36Z olme8414 $
 */

#if OS_USE_POSIX_SIGNALS_FOR_THREAD_CONTROL
#include <LINUX/Os_LINUX_threadsyncspot.h>
#endif

#include <Os_configuration.h>
#include <Os_kernel.h>
#include <board.h>

#include <memmap/Os_mm_const_begin.h>

/** \brief The interrupt disable level for disabling OS (category 2) interrupts. */
const os_intstatus_t OS_intDisableLevel = OS_CAT2LOCK;

/** \brief The interrupt disable level for disabling all (category 1 and 2) interrupts. */
const os_intstatus_t OS_intDisableLevelAll = OS_CAT1LOCK;

/** \brief The interrupt disable level for disabling OS (category 2) and
 * kernel interrupts (for multi-core case).
 */
const os_intstatus_t OS_kernDisableLevel = OS_KERNLOCK;

/** \var OS_kernOnlyXcoreLevel
 * \brief The interrupt disable level for allowing cross-core interrupts only.
 */
#if (OS_N_CORES > 1)
const os_intstatus_t OS_kernOnlyXcoreLevel = OS_XCOREUNLOCK;
#else
const os_intstatus_t OS_kernOnlyXcoreLevel = OS_KERNLOCK;
#endif

/** \var OS_intDisableLevelMax
 * \brief The interrupt disable level for disabling ALL interruptions.
 *
 * The interrupt disable level to disable ALL possible interrupts. This includes
 * CAT2, CAT1 and OS interrupts, i.e. the maximum of all lock levels.
 */
#if (OS_KERNLOCK < OS_CAT1LOCK)
const os_intstatus_t OS_intDisableLevelMax = OS_CAT1LOCK;
#else
const os_intstatus_t OS_intDisableLevelMax = OS_KERNLOCK;
#endif

#if (OS_N_CORES > 1)
/** \brief The ISR ID of the cross-core interrupts.
 *
 * This array contains for each core the ID of its cross-core ISR.
 * These ISRs must be triggered to notify the respective cores of new messages,
 * i.e. trigger <tt>OS_crossCoreIsrIds[i]</tt> to notify core \c i.
 *
 * Note, there are cross-core interrupts, if there are more than one core.
 */
os_isrid_t const OS_crossCoreIsrIds[OS_N_CORES_MAX] = OS_XCORE_ISR_IDS_INIT;
#endif

/* \brief The conversion factor from ticks to nanoseconds.
 *
 * Instead of OS_BoardTicksToNs(), this constant is used by code which is
 * configuration independent.
 */
os_winlinTimerValue_t const OS_winlinNsPerTick = OS_BoardTicksToNs(1);

#include <memmap/Os_mm_const_end.h>

#include <memmap/Os_mm_var_begin.h>

/** \brief The time warp factor.
 *
 * The time warp factor is used to adjust the passage of time to the given facts
 * of the host machine running Windows or Linux. Normally, timing constraints as seen in
 * embedded system are not feasible on Windows/Linux. Hence, either the
 * timing constraints must be relaxed or the perception of time by an AUTOSAR
 * application. With this factor the perception of time by an AUTOSAR application
 * can be slowed down or \e warped. A value of 1 effectively switches off time
 * warping, i.e. AUTOSAR and Windows/Linux clocks run with the same speed. A larger
 * value proportionally slows down AUTOSAR clocks by the given amount, i.e.
 * time as perceived by AUTOSAR applications is warped. Let \c OS_winlinTimeWarpFactor
 * be \c n, then \c x nanoseconds in the AUTOSAR world correspond to <tt>x * n</tt>
 * nanoseconds in the Windows/Linux world or reality, because the Windows/Linux clock used
 * by the FRC implementation shows the wall-clock time.
 */
os_winlinTimerValue_t OS_winlinTimeWarpFactor = OS_TIME_WARP_FACTOR;

#include <memmap/Os_mm_var_end.h>

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
 */
