/* Os_errorcodes_arch.h
 *
 * Architecture-dependent error-codes are included from the appropriate
 * ARCH/Os_errorcodes_ARCH.h include file depending on the chosen architecture.
 *
 * The Makefiles must ensure that the OS_ARCH and OS_CPU macros are
 * defined appropriately on the command line.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_errorcodes_arch.h 30593 2019-07-30 14:10:36Z beoe851 $
*/
#ifndef OS_ERRORCODES_ARCH_H
#define OS_ERRORCODES_ARCH_H

#include <Os_defs.h>

#if (OS_ARCH==OS_PA)
#include <PA/Os_errorcodes_PA.h>
#elif (OS_ARCH==OS_TRICORE)
#include <TRICORE/Os_errorcodes_TRICORE.h>
#elif (OS_ARCH==OS_RH850)
#include <RH850/Os_errorcodes_RH850.h>
#elif (OS_ARCH==OS_NEWARCH)
#include <NEWARCH/Os_errorcodes_NEWARCH.h>
#elif (OS_ARCH==OS_PIKEOS)
#include <PIKEOS/Os_errorcodes_PIKEOS.h>
#elif (OS_ARCH==OS_WINDOWS)
#include <WINDOWS/Os_errorcodes_WINDOWS.h>
#elif (OS_ARCH==OS_ARM)
#include <ARM/Os_errorcodes_ARM.h>
#elif (OS_ARCH==OS_ARM64)
#include <ARM64/Os_errorcodes_ARM64.h>
#elif (OS_ARCH==OS_LINUX)
#include <LINUX/Os_errorcodes_LINUX.h>
#elif (OS_ARCH==OS_CORTEXM)
#include <CORTEXM/Os_errorcodes_CORTEXM.h>
#elif (OS_ARCH==OS_ARC)
#include <ARC/Os_errorcodes_ARC.h>
#else
#error "Unsupported OS_ARCH defined. Check your Makefiles!"
#endif

#endif
/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
