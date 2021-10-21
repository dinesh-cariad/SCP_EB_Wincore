/* Os_WINLIN_timer.h - WINLIN timer driver
 *
 * The WINLIN timer driver interface.
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_WINLIN_timer.h 27282 2017-08-24 13:57:43Z olme8414 $
*/

#ifndef OS_WINLIN_TIMER_H
#define OS_WINLIN_TIMER_H

#include <Os_kernel.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef OS_ASM
extern const os_hwtdrv_t OS_frcDriver;

extern void OS_FrcInit(const os_hwt_t *);
extern os_timervalue_t OS_FrcRead(const os_hwt_t *);
extern os_boolean_t OS_FrcStart(const os_hwt_t *, os_timervalue_t, os_tick_t);
extern void OS_FrcStop(const os_hwt_t *);
extern void OS_FrcGlobalInit(void);
extern os_boolean_t OS_FrcIsThisAFrcThread(void);
#endif /* !OS_ASM */

#ifdef __cplusplus
}
#endif

#endif /* OS_WINLIN_TIMER_H */

/* Editor settings: DO NOT DELETE
 * vi:set ts=4:
*/
