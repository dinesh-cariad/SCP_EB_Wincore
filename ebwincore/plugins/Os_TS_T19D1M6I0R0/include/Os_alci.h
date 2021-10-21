/* Os_alci.h - Header for ALCI feature
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Os_alci.h 30651 2019-08-13 10:23:16Z ache271795 $
*/

#ifndef OS_ALCI_H
#define OS_ALCI_H

#include <Os_types.h>
#include <Os_cpu.h>

#ifndef OS_USE_LOGICAL_CORE_IDS
#define OS_USE_LOGICAL_CORE_IDS 0
#endif

#if (OS_USE_LOGICAL_CORE_IDS == 1)

#ifndef OS_ASM
/* Advanced Logical Core Interface Look-Up Tables. */
extern const os_coreid_t OS_alciPhyToLog[OS_N_CORES_MAX];
extern const os_coreid_t OS_alciLogToPhy[OS_N_CORES_MAX];
os_coreid_t OS_GetPhysicalCoreId(os_coreid_t core);
os_coreid_t OS_GetLogicalCoreId(os_coreid_t core);
#endif  /* OS_ASM */

#else /* OS_USE_LOGICAL_CORE_IDS == 0 */

#define OS_GetPhysicalCoreId(p) 	(p)
#define OS_GetLogicalCoreId(l) 		(l)

#endif /* OS_USE_LOGICAL_CORE_IDS == 1 */

#endif /* OS_ALCI_H */
/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/

