/* Os_mmu.h
 *
 * Header file for MMU support. Includes the architecture-specific MMU file first,
 * then defines some architecture-independent MMU structures etc.
 *
 * The Makefiles must ensure that the OS_ARCH and OS_CPU macros are
 * defined appropriately on the command line.
 *
 * If you get an error "file not found" it probably means that the architecture doesn't
 * provide MMU support.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_mmu.h 30593 2019-07-30 14:10:36Z beoe851 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 8.11 (advisory)
 * When an array with external linkage is declared, its size should be
 * explicitly specified.
 *
 * Reason:
 * Array size and initialization are depending on configuration whereas the
 * array declaration is used by configuration-independent library code.
 */

#ifndef OS_MMU_H
#define OS_MMU_H

#include <Os_defs.h>
#include <Os_types.h>

/* Include the architecture's MMU file.
 * Most architectures don't have an MMU, and of those that do only very few
 * are actually used. But the list of architectures here should be complete anyway
*/
#if (OS_ARCH==OS_PA)
#include <PA/Os_mmu_PA.h>
#elif (OS_ARCH==OS_TRICORE)
#include <TRICORE/Os_mmu_TRICORE.h>
#elif (OS_ARCH==OS_RH850)
#include <RH850/Os_mmu_RH850.h>
#elif (OS_ARCH==OS_NEWARCH)
#include <NEWARCH/Os_mmu_NEWARCH.h>
#elif (OS_ARCH==OS_PIKEOS)
#include <PIKEOS/Os_mmu_PIKEOS.h>
#elif (OS_ARCH==OS_WINDOWS)
#include <WINDOWS/Os_mmu_WINDOWS.h>
#elif (OS_ARCH==OS_ARM)
#include <ARM/Os_mmu_ARM.h>
#elif (OS_ARCH==OS_ARM64)
#include <ARM64/Os_mmu_ARM64.h>
#elif (OS_ARCH==OS_LINUX)
#include <LINUX/Os_mmu_LINUX.h>
#elif (OS_ARCH==OS_CORTEXM)
#include <ARM/Os_mmu_CORTEXM.h>
#elif (OS_ARCH==OS_ARC)
#include <ARC/Os_mmu_ARC.h>
#else
#error "Unsupported OS_ARCH defined. Check your Makefiles!"
#endif

/* Structure to hold a page mapping
*/
#ifndef OS_ASM
#ifndef OS_ARCH_PAGEMAP_T
/* the architecture files might already have the typedef */
typedef struct os_pagemap_s os_pagemap_t;
#endif

struct os_pagemap_s
{
	os_address_t base;		/* Base address of hunk */
	os_size_t size;			/* Size of hunk */
	os_uint32_t flags;		/* Accessibility etc. (see below) */
};

/* convenience define to find the end of a region(Address of last byte in the region)) */
#define OS_ENDADDR(x) ((x)->base + ((x)->size - 1))

/* convenience define to find the end of a region (address one after the region) */
#define OS_ENDADDR_P1(x) (((x)->base + (x)->size))

/* convenience define to get an align mask out of table sizes */
#define OS_ALIGN_MASK(x) ((x) - 1)

#endif

/* Generic bit definitions in the 'flags' field. Architecture-dependent flags
 * may be added by the architecture-specific header.
*/
#define OS_MEMPERM_SX		OS_U(0x00000001)	/* Can execute in supervisor mode */
#define OS_MEMPERM_SR		OS_U(0x00000002)	/* Can read in supervisor mode */
#define OS_MEMPERM_SW		OS_U(0x00000004)	/* Can write in supervisor mode */
#define OS_MEMPERM_UX		OS_U(0x00000010)	/* Can execute in user mode */
#define OS_MEMPERM_UR		OS_U(0x00000020)	/* Can read in user mode */
#define OS_MEMPERM_UW		OS_U(0x00000040)	/* Can write in user mode */

/* for padding read-only RAM regions, either the generic kernel function or a
 * architecture-specific implementation might be used.
*/
#ifndef OS_ArchMmuPadRamReadonly
#define OS_ArchMmuPadRamReadonly(a, b) OS_MmuPadRamReadonly(a, b)
#endif

/* for turning chunkmaps into pagemaps, either the generic kernel function or a
 * architecture-specific implementation might be used.
*/
#ifndef OS_ArchMmuMapPages
#define OS_ArchMmuMapPages(a, b, c, d) OS_MmuMapPages(a, b, c, d)
#endif

/* On machines with MMU support there's a concept of MMU ownership to reduce the amount of switching page
 * tables back and forth. Furthermore, it's also possible that trusted tasks and ISRs need to have their
 * page tables reprogrammed (see the TLB entry reduction code). So the decision whether to restore
 * the interrupted task/ISR's protection is left to the MMU functions.
*/
#define OS_QueryRestoreProtection(isr)	(1)

/* Global data and constants related to MMU support:
 *
 *	- Base, size and limit for .text, RAM and .rodata. Used in CheckXxxMemoryAccess()
 *	- Base and size for heap.
 *	- MMU management variables and constants
*/
#ifndef OS_ASM
extern const void * const OS_textBase;
extern const os_size_t OS_textSize;
extern const void * const OS_textLimit;

extern const void * const OS_ramBase;
extern const os_size_t OS_ramSize;
extern const void * const OS_ramLimit;

extern const void * const OS_constBase;
extern const os_size_t OS_constSize;
extern const void * const OS_constLimit;

extern const os_address_t OS_heapBase;
extern const os_size_t OS_heapSize;

extern os_int_t OS_nFreePageMaps;
extern os_int_t OS_maxObjectMaps;
extern os_int_t OS_nTrustedPageMaps;
extern os_archpagemap_t *OS_trustedPageMaps;
/* Deviation MISRAC2012-1 */
extern const os_size_t OS_pageSize[];

extern os_objecttype_t OS_mmuOwnerType;
extern os_objecttype_t OS_mmuOwnerId;
#endif

/* Functions
*/
#ifndef OS_ASM
/* Architecture-independent support functions
*/
os_result_t OS_MmuTaskMapper(os_taskid_t);
os_result_t OS_MmuIsrMapper(os_isrid_t);
os_result_t OS_MmuAppMapper(os_applicationid_t);

os_int_t OS_MmuOptimise(os_pagemap_t *, os_int_t);
void OS_MmuMapSort(os_pagemap_t *, os_int_t);
void OS_MmuPagify(os_pagemap_t *, os_int_t);
os_int_t OS_MmuMapMerge(os_pagemap_t *, os_int_t);
os_int_t OS_MmuPageMap(os_address_t, os_size_t, os_uint32_t, os_pagemap_t*);
os_pagesize_t OS_MmuBestPageSize(os_address_t);

/* generic functions that *might* be used */
os_int_t OS_MmuPadRamReadonly(os_pagemap_t *, os_int_t);
os_int_t OS_MmuMapPages(os_pagemap_t *, os_int_t, os_pagemap_t*, os_int_t);

/* functions that have to be implemented for each architecture */
os_archpagemap_t *OS_ArchMmuPageMapTranslate(os_pagemap_t *, os_int_t);

#endif

#endif
/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
