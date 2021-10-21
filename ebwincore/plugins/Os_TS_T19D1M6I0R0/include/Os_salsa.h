/* Os_salsa.h
 *
 * This file maps the required SALSA service names onto existing OS services.
 *
 * The content of this file is not integrated into the normal OS files because
 * the names do not conform to the AUTOSAR nameing scheme. These names were
 * specified in the requirements, so they must be used.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_salsa.h 27282 2017-08-24 13:57:43Z olme8414 $
*/
#ifndef OS_SALSA_H
#define OS_SALSA_H

#ifndef OS_ASM

#include <Os_types.h>
#include <Os_cpuload.h>

#define GetCpuLoad()				OS_GetCpuLoad(OS_CORE_ID_THIS_CORE, 0)
#define GetMaxCpuLoad()				OS_GetCpuLoad(OS_CORE_ID_THIS_CORE, 1)
#define GetCpuLoadOnCore(coreId)	OS_GetCpuLoad((coreId), 0)
#define GetMaxCpuLoadOnCore(coreId)	OS_GetCpuLoad((coreId), 1)
#define InitCpuLoad()				OS_ResetPeakCpuLoad(OS_CORE_ID_THIS_CORE)
#define InitCpuLoadOnCore(coreId)	OS_ResetPeakCpuLoad((coreId))

extern os_uint8_t GetStackUsage(os_taskid_t);
extern os_uint8_t GetSystemStackUsage(void);

#endif  /* OS_ASM */

#endif  /* OS_SALSA_H */

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
