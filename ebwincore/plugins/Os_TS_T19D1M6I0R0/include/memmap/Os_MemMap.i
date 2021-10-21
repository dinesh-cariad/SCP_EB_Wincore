/* Os_memmap.i
 *
 * This include file provides section mapping support for assembler files.
 * It includes the appropriate architecture include file depending
 * on the chosen architecture.
 *
 * See also: Os_defs.h
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_MemMap.i 25436 2016-12-14 09:58:49Z stpo8218 $
*/
#ifndef OS_MEMMAP_I_
#define OS_MEMMAP_I_

#include <Os_defs.h>

#if (OS_ARCH==OS_PA)
#include <memmap/PA/Os_memmap_PA.i>
#elif (OS_ARCH==OS_TRICORE)
#include <memmap/TRICORE/Os_memmap_TRICORE.i>
#elif (OS_ARCH==OS_NEWARCH)
#include <memmap/NEWARCH/Os_memmap_NEWARCH.i>
#elif (OS_ARCH==OS_WINDOWS)
#include <memmap/WINDOWS/Os_memmap_WINDOWS.i>
#elif (OS_ARCH==OS_ARM)
#include <memmap/ARM/Os_memmap_ARM.i>
#else
#error "Unsupported OS_ARCH defined. Check your Makefiles!"
#endif


#endif /*OS_MEMMAP_I_*/
