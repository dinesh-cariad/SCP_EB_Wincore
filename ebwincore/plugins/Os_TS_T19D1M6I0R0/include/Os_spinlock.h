/* Os_spinlock.h
 *
 * Definitions for spinlock mechanisms. This incorporates both AUTOSAR and internal spinlocks.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_spinlock.h 27881 2017-11-15 11:42:38Z miwi261726 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 8.11 (advisory)
 * When an array with external linkage is declared, its size should be
 * explicitly specified.
 *
 * Reason:
 * This would require to make the OS library dependent on the user configuration
 * and this is not desired.
 */

#ifndef OS_SPINLOCK_H
#define OS_SPINLOCK_H

#include <Os_kernel.h>
#include <Os_timestamp.h>

#if (OS_KERNEL_TYPE == OS_MICROKERNEL)
#error "spinlocks are implemented by the microkernel"
#endif

/* Autosar-Spinlock data structures.
 *
 * Each spinlock has two data structures; one in ROM, one in RAM.
 * The address of the ROM structure is the lock ID. The table of spinlocks
 * is used for confirmation that any individual reference really refers to
 * a configured spinlock. If that level of checking is omitted the
 * table can be dropped.
 */
#ifndef OS_ASM
typedef struct os_autosarspinlock_s os_autosarspinlock_t;
typedef struct os_autosarspinlockdyn_s os_autosarspinlockdyn_t;

struct os_autosarspinlockdyn_s
{
	os_coreid_t ownerCore;				/* The current owner's core id; OS_CORE_ID_INVALID if unoccupied */
	os_objecttype_t ownerType;			/* The current owner's type; OS_OBJ_NONE, OS_OBJ_TASK or OS_OBJ_ISR */
	os_objectid_t ownerId;				/* The task- or ISR-ID of the owner */

	/* List of already-occupied resources/spinlocks (retrospective). Concrete values for this member are created with
	 * OS_CreateObjectId(id, OS_OBJ_SPINLOCK) or OS_CreateObjectId(id, OS_OBJ_RESOURCE) for a predecessor
	 * resource/spinlock with identifier 'id'. The identifier 'id' has either the type os_lockid_t or os_resourceid_t.
	 */
	os_objectid_t predecessor;
	os_prio_t lastPrio;
};

struct os_autosarspinlock_s
{
	os_autosarspinlockdyn_t *dynamic;
	const os_autosarspinlockid_t successor;
	os_permission_t accessPermissions;
	os_autosarspinlockid_t id;
	os_spinlocklockmethod_t lockMethod;
};
#endif /* OS_ASM */

#ifndef OS_ASM
extern const os_autosarspinlock_t * const OS_autosarSpinlockTableBase;
extern const os_autosarspinlockid_t OS_nAutosarSpinlocks;
#endif /* OS_ASM */

/*!
 * OS_SOFTWARELOCK_LOCKID
 * OS_NSOFTWARELOCKLOCKS
 *
 * The identifier of the lock, which protects the OS_softwareLock[] array. This lock must have hardware
 * support, because it's used to protect all other locks. Hence, there must be at least one hardware-assisted
 * locking mechanism provided, which backs all other locks.
 * Note, the macro OS_NSOFTWARELOCKLOCKS is always 1 and exists because of symmetry reasons.
 */
#define OS_NSOFTWARELOCKLOCKS	OS_U(1)
#define OS_SOFTWARELOCK_LOCKID	OS_U(0)

/*!
 * OS_NMESSAGEQLOCKS
 * OS_MESSAGEQLOCKBASE
 * OS_GetIntercoreMsgQueueLockIdForCore
 *
 * The number of message queue locks depends on the number of cores. When there is just one core or when
 * there are just two cores, locks aren't necessary. For more cores there is one such lock per core, whose
 * lock ID start at OS_MESSAGEQ_LOCKIDBASE. This means, lock 'OS_MESSAGEQ_LOCKIDBASE + x' is for the input
 * queue of core x. Use the macro OS_GetIntercoreMsgQueueLockIdForCore() to determine the lock ID of the message
 * queue of core x.
 */
#if (OS_N_CORES >= 3)
#define OS_NMESSAGEQLOCKS	OS_N_CORES_MAX
#else
#define OS_NMESSAGEQLOCKS	OS_U(0)
#endif /* OS_N_CORES >= 3 */

#define OS_MESSAGEQ_LOCKIDBASE	(OS_SOFTWARELOCK_LOCKID + OS_NSOFTWARELOCKLOCKS)
#define OS_GetIntercoreMsgQueueLockIdForCore(coreid)	((os_lockid_t)((os_lockid_t)OS_MESSAGEQ_LOCKIDBASE + (coreid)))

/*!
 * OS_APPMODE_LOCKID
 * OS_NAPPMODELOCKS
 *
 * The identifier of the lock protecting the global variable OS_appMode.
 */
#if (OS_N_CORES == 1)
#define OS_NAPPMODELOCKS	OS_U(0)
#else
#define OS_NAPPMODELOCKS	OS_U(1)
#endif
#define OS_APPMODE_LOCKID	(OS_MESSAGEQ_LOCKIDBASE + OS_NMESSAGEQLOCKS)

/*!
 * OS_CORESTARTED_LOCKID
 * OS_NCORESTARTEDLOCKS
 *
 * This lock is used by StartCore() to ensure proper locking for recording which cores have been started yet.
 * This is especially important on cores, which do not provide cache-coherency, because the array OS_coreStarted[]
 * might straddle cache lines.
 */
#if (OS_N_CORES == 1)
#define OS_NCORESTARTEDLOCKS	OS_U(0)
#else
#define OS_NCORESTARTEDLOCKS	OS_U(1)
#endif
#define OS_CORESTARTED_LOCKID	(OS_APPMODE_LOCKID + OS_NAPPMODELOCKS)

/*!
 * OS_SYNCHERE_LOCKID
 * OS_NSYNCHERELOCKS
 *
 * This lock is used in OS_SyncHere() to ensure proper locking of the OS_syncArray.
 * This is especially important on cores, which do not provide cache-coherency, because several entries of
 * array OS_syncArray[] are likely within one cache line.
 */
#if (OS_N_CORES == 1)
#define OS_NSYNCHERELOCKS	OS_U(0)
#else
#define OS_NSYNCHERELOCKS	OS_U(1)
#endif
#define OS_SYNCHERE_LOCKID	(OS_CORESTARTED_LOCKID + OS_NCORESTARTEDLOCKS)

/*!
 * OS_TIMESTAMP_LOCKID
 * OS_NTIMESTAMPLOCKS
 *
 * In case the global variant of the generic time-stamp mechanism is used, there needs to be a lock to
 * prevent concurrent access from different cores.
 */
#if (OS_USE_GLOBAL_GENERICTIMESTAMP && (OS_N_CORES > 1))
#define OS_NTIMESTAMPLOCKS	OS_U(1)
#else
#define OS_NTIMESTAMPLOCKS	OS_U(0)
#endif
#define OS_TIMESTAMP_LOCKID	(OS_SYNCHERE_LOCKID + OS_NSYNCHERELOCKS)

/*!
 * OS_STARTUPRES_LOCKID
 * OS_NSTARTUPRESLOCKS
 *
 * In case core shared resources are used during the startup, there needs to be a lock to
 * prevent concurrent access from different cores.
 */
#if ((defined(OS_USE_SHARED_STARTUP_RESOURCES)) && (OS_N_CORES > 1))
#define OS_NSTARTUPRESLOCKS	OS_U(1)
#else
#define OS_NSTARTUPRESLOCKS	OS_U(0)
#endif
#define OS_STARTUPRES_LOCKID	(OS_TIMESTAMP_LOCKID + OS_NTIMESTAMPLOCKS)

/*!
 * OS_NKERNELLOCKS
 *
 * The total number of locks required by the kernel for internal purposes.
 */
#define OS_NKERNELLOCKS			\
	(OS_NSOFTWARELOCKLOCKS		\
		+ OS_NMESSAGEQLOCKS		\
		+ OS_NAPPMODELOCKS		\
		+ OS_NCORESTARTEDLOCKS	\
		+ OS_NSYNCHERELOCKS		\
		+ OS_NTIMESTAMPLOCKS	\
		+ OS_NSTARTUPRESLOCKS)

/*!
 * OS_IsAutosarSpinlockNestingPermitted
 * OS_IsValidAutosarSpinlock
 *
 * Macros to check properties of AUTOSAR spinlocks: nesting and validity.
 */
#define OS_IsAutosarSpinlockNestingPermitted()	OS_TRUE
#define OS_IsValidAutosarSpinlock(x)			((x) < OS_nAutosarSpinlocks)

#ifndef OS_ASM
/* CHECK: NOPARSE */
/* Deviation MISRAC2012-1 */
extern os_syncspot_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.shared.os.OS_softwareLock)
		OS_ALIGNED_ATTRIB_PRE_DECL(OS_SYNCSPOT_ALIGNMENT_GRANULE)
		OS_softwareLock[]
		OS_SECTION_ATTRIB_POST_DECL(.bss.shared.os.OS_softwareLock);
/* CHECK: PARSE */

extern const os_uint_t OS_nSoftwareLocks;

void OS_InitAutosarSpinlocks(void);
os_intstatus_t OS_TakeInternalLock(os_lockid_t);
void OS_DropInternalLock(os_lockid_t, os_intstatus_t);
void OS_DropInternalSpinlockOnly(os_lockid_t);
#endif /* OS_ASM */

#endif /* OS_SPINLOCK_H */

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
