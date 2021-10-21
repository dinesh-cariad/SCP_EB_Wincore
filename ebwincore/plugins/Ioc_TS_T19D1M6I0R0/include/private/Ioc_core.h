/* Ioc_core.h - core definitions and data structures
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_core.h 3259 2019-07-31 11:59:00Z beoe851 $
*/

#ifndef IOC_CORE_H
#define IOC_CORE_H

#include <public/Ioc_libconfig.h>
#include <public/Ioc_defs.h>


#if IOC_CPUFAMILY==IOC_ARC
#include <private/ARC/Ioc_ARC_core.h>
#elif IOC_CPUFAMILY==IOC_ARM
#include <private/ARM/Ioc_ARM_core.h>
#elif IOC_CPUFAMILY==IOC_ARM64
#include <private/ARM64/Ioc_ARM64_core.h>
#elif IOC_CPUFAMILY==IOC_CORTEXM
#include <private/CORTEXM/Ioc_CORTEXM_core.h>
#elif IOC_CPUFAMILY==IOC_PA
#include <private/PA/Ioc_PA_core.h>
#elif IOC_CPUFAMILY==IOC_RH850
#include <private/RH850/Ioc_RH850_core.h>
#elif IOC_CPUFAMILY==IOC_TRICORE
#include <private/TRICORE/Ioc_TRICORE_core.h>
#elif IOC_CPUFAMILY==IOC_WINDOWS
#include <private/WINDOWS/Ioc_WINDOWS_core.h>
#else
#error "Unknown or unsupported value for IOC_CPUFAMILY. Check your Makefiles!"
#endif

/* IOC_HwWellAlignedExtInput - tells whether the address of a ioc_extinput_t is well-aligned.
*/
#ifndef IOC_HwWellAlignedExtInput
#error "IOC_HwWellAlignedExtInput not defined for this hardware."
#endif

/* IOC_WellAlignedILength - tells whether the address of an ioc_ilength_t is well-aligned.
 *
 * We assume, that ioc_ilength_t needs natural alignment.
*/
#define IOC_WellAlignedILength(address) ((((ioc_address_t)(address)) & (sizeof(ioc_ilength_t) - 1u)) == 0u)

/* IOC_HW_HAS_LOCK_INTER_USER - tells, whether the IOC defines hardware-specific user-mode spinlocks.
*/
#ifndef IOC_HW_HAS_LOCK_INTER_USER
#define IOC_HW_HAS_LOCK_INTER_USER 0
#endif

#ifndef IOC_HW_HAS_WEAK_MEM_ORDERING
/* IOC_HW_HAS_WEAK_MEM_ORDERING - tells, whether the IOC has to use additional memory barriers to
 *                                handle channel information in a consistent way.
 */
#define IOC_HW_HAS_WEAK_MEM_ORDERING   0
#endif

#if !IOC_HW_HAS_WEAK_MEM_ORDERING
/* IOC_HwMemoryBarrier() - default (i.e. empty) memory barrier
 *
 * !LINKSTO IOC.Macro.IOC_HwMemoryBarrier, 1
 */
#define IOC_HwMemoryBarrier()       do {} while(0)
#endif

#endif /* IOC_CORE_H */

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
