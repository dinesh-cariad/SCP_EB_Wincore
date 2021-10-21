/* Os_acct_api.h
 *
 * This file defines the OS Accounting API.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_acct_api.h 27282 2017-08-24 13:57:43Z olme8414 $
*/
#ifndef OS_ACCT_API_H
#define OS_ACCT_API_H

#include <Os_types.h>

#ifndef OS_ASM
/* User library routines
*/
os_result_t OS_GetIsrMaxRuntime(os_isrid_t, os_tick_t *);
os_result_t OS_GetTaskMaxRuntime(os_taskid_t, os_tick_t *);

#endif   /* OS_ASM */

#endif /* OS_ACCT_API_H */

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
