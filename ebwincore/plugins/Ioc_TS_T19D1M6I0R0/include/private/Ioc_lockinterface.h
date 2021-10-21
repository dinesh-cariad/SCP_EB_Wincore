/* Ioc_lockinterface.h
 *
 * This file defines the LockInterface of the IOC.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_lockinterface.h 1825 2017-02-16 12:14:32Z mist9353 $
*/
#ifndef IOC_LOCKINTERFACE_H
#define IOC_LOCKINTERFACE_H

#include <public/Ioc_basic_types.h>
#include <private/Ioc_types_forward.h>
#include <private/Ioc_core.h>


#ifndef IOC_ASM

typedef ioc_return_t (*ioc_lockfn_t) (volatile void *lock_ref);
typedef ioc_return_t (*ioc_unlockfn_t) (volatile void *lock_ref);

typedef struct ioc_locktype_s ioc_locktype_t;

struct ioc_locktype_s
{
	ioc_lockfn_t	lockfn;			/* Pointer to the lock function */
	ioc_unlockfn_t	unlockfn;		/* Pointer to the unlock function */
};

/* Generic definition for "no lock"
 */
#define IOC_LOCKINIT_NO_LOCK \
	{	\
		IOC_NULL, \
		IOC_NULL \
	}

/* Generic implementation of the local-core interrupt lock from user mode.
 */
ioc_return_t IOC_OsAllInterrupts_Lock(volatile void *);
ioc_return_t IOC_OsAllInterrupts_Unlock(volatile void *);

#define IOC_LOCKINIT_INTRA_INT_USER \
	{	\
		&IOC_OsAllInterrupts_Lock, \
		&IOC_OsAllInterrupts_Unlock \
	}

/* Generic implementation of the local-core interrupt lock from kernel mode.
 */
#if (IOC_LCFG_KERNEL_TYPE == IOC_MICROKERNEL)

/* Interrupts are already disabled during microkernel system calls.
*/
#define IOC_LOCKINIT_INTRA_INT_KERNEL IOC_LOCKINIT_NO_LOCK

#else

ioc_return_t IOC_OsInterruptsKernel_Lock(volatile void *);
ioc_return_t IOC_OsInterruptsKernel_Unlock(volatile void *);

#define IOC_LOCKINIT_INTRA_INT_KERNEL \
	{	\
		&IOC_OsInterruptsKernel_Lock, \
		&IOC_OsInterruptsKernel_Unlock \
	}

#endif /* IOC_LCFG_KERNEL_TYPE */

/* Declaration of user-mode cross-core lock functions.
 * If IOC_HW_HAS_LOCK_INTER_USER the platform offers optimized locks.
 * Otherwise we use the locks provided by the OS.
 */
#if IOC_HW_HAS_LOCK_INTER_USER

ioc_return_t IOC_Spinlock_Lock(volatile void *);
ioc_return_t IOC_Spinlock_Unlock(volatile void *);

#define IOC_LOCKINIT_INTER_USER \
	{	\
		&IOC_Spinlock_Lock, \
		&IOC_Spinlock_Unlock \
	}

#else

ioc_return_t IOC_OsSpinlock_Lock(volatile void *);
ioc_return_t IOC_OsSpinlock_Unlock(volatile void *);

#define IOC_LOCKINIT_INTER_USER \
	{	\
		&IOC_OsSpinlock_Lock, \
		&IOC_OsSpinlock_Unlock \
	}

#endif /* IOC_HW_HAS_LOCK_INTER_USER */


/* Generic implementation of the cross-core lock from kernel mode.
 */
ioc_return_t IOC_OsSpinlockKernel_Lock(volatile void *);
ioc_return_t IOC_OsSpinlockKernel_Unlock(volatile void *);

#define IOC_LOCKINIT_INTER_KERNEL \
	{	\
		&IOC_OsSpinlockKernel_Lock, \
		&IOC_OsSpinlockKernel_Unlock \
	}


ioc_return_t IOC_LH_AcquireLocks(const ioc_config_t *);
void IOC_LH_ReleaseLocks(const ioc_config_t *);

#endif /* IOC_ASM */


#endif /* IOC_LOCKINTERFACE_H */

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
