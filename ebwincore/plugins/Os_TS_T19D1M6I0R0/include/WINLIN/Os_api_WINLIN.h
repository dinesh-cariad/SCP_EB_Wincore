/* Os_api_WINLIN.h
 *
 * This file is the architecture-dependent public API prototypes file for WINLIN.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_api_WINLIN.h 30052 2019-03-27 14:30:23Z olme8414 $
 */

#ifndef OS_API_WINLIN_H
#define OS_API_WINLIN_H

#include <WINLIN/Os_api_WINLIN_shared.h>

/** \def OS_HAS_TRANSACTION_SUPPORT
 *
 * WINLIN does not support atomic monitor operations.
 */
#define OS_HAS_TRANSACTION_SUPPORT	0

#ifndef OS_HAS_TB_INTERNAL
#error "OS_HAS_TB_INTERNAL undefined."
#endif

#if OS_HAS_TB_INTERNAL
#define OS_TB_FUNC OS_WINLINGetTimeStamp
#ifndef OS_ASM
extern void OS_WINLINGetTimeStamp(os_timestamp_t*);
#endif	/* !OS_ASM */
#endif	/* OS_HAS_TB_INTERNAL */

#ifndef OS_ASM
extern os_intstatus_t OS_WINLINGoingToUseHostOsService(void);
extern void OS_WINLINFinishedUsingHostOsService(os_intstatus_t);
extern void OS_WINLINPreStartOs(os_int_t, os_char_t* argv[]);
extern void OS_WINLINClearInterrupt(os_isrid_t);
#endif /* !OS_ASM */

/** \name Architecture-specific StartOS()
 *
 * WINLIN uses a non-standard way of starting the AutoCore OS.
 */
#define OS_ARCH_USERSTARTOS 1
#ifndef OS_ASM
extern void OS_ArchUserStartOs(os_uint8_t);
#endif	/* !OS_ASM */

#endif	/* OS_API_WINLIN_H */

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
 */
