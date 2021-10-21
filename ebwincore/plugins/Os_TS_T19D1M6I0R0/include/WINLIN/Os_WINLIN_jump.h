/* Os_WINLIN_jump.h
 *
 * Definitions for jump buffers.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_WINLIN_jump.h 27660 2017-10-18 14:07:36Z olme8414 $
 */

#ifndef OS_WINLIN_JUMP_H_
#define OS_WINLIN_JUMP_H_

#ifndef OS_ASM
#include <setjmp.h>
#endif

/** \name Setjump and Longjump macros.
 *
 * To use the setjmp() and longjmp() functions the following macros
 * provide convenience wrappers around them. They are intended to
 * be used by task and core threads.
 *
 * \remarks To wrap setjmp() in a function instead of a macro won't work, because of
 * the way the processor context is saved. Doing it this way would mean to "jump up
 * the call stack". This could cause troubles because the return address is naturally
 * located on the stack (for Intel X86 processors) and is thus may have an arbitrary
 * value when the corresponding longjmp() is executed at a later point. Hence, it is only
 * safe to use the setjmp() macro directly at the place where the longjmp() shall jump to
 * later. Furthermore, you should only "jump down the call stack" to avoid any stack
 * related issues. Note, that to jump up the call is towards the callee; to jump down is
 * towards the caller.
 * The function/macro longjmp() doesn't have this problem, because it "just" restores the
 * state in the buffer passed to it.
 *
 * \see https://en.wikipedia.org/wiki/Setjmp.h
 */
/** @{ */
#define OS_WINLINTaskSetjmpById(taskId) (setjmp(OS_WINLINTaskJmpBufs[(taskId)]))
#define OS_WINLINTaskSetjmp() (setjmp(OS_WINLINTaskJmpBufs[OS_GetKernelData()->taskCurrent->taskId]))
#define OS_WINLINCoreSetjmp() (setjmp(OS_WINLINCoreJmpBufs[OS_GetMyCoreId()]))

#ifndef OS_ASM
extern void OS_WINLINTaskLongjmp(void);
extern void OS_WINLINCoreLongjmp(void);
#endif
/** @} */

#ifndef OS_ASM
extern jmp_buf OS_WINLINTaskJmpBufs[];
extern jmp_buf OS_WINLINCoreJmpBufs[OS_N_CORES_MAX];
#endif

#endif /* OS_WINLIN_JUMP_H_ */

/* Editor settings: DO NOT DELETE
 * vi:set ts=4:
*/
