/* Os_callouts.h - callouts (user-defined functions) for OSEK and Autosar
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_callouts.h 27282 2017-08-24 13:57:43Z olme8414 $
*/
#ifndef OS_CALLOUTS_H
#define OS_CALLOUTS_H

#include <Os_types.h>
#include <Os_error.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * !LINKSTO Kernel.Autosar.API.HookFunctions.ProtectionHook, 1
*/

#ifndef OS_ASM
void ErrorHook(os_result_t);
void PreTaskHook(void);
void PostTaskHook(void);
void StartupHook(void);
void ShutdownHook(os_result_t);
void PreIsrHook(os_isrid_t);
void PostIsrHook(os_isrid_t);
os_erroraction_t ProtectionHook(os_result_t);
#endif

#ifdef __cplusplus
}
#endif

#endif

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
