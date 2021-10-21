/* Os_api_arch.h
 *
 * This file includes the appropriate architecture API include file depending
 * on the chosen architecture.
 *
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_api_arch.h 30593 2019-07-30 14:10:36Z beoe851 $
*/
#ifndef OS_API_ARCH_H
#define OS_API_ARCH_H

#include <Os_defs.h>

#if (OS_ARCH==OS_PA)
#include <PA/Os_api_PA.h>
#elif (OS_ARCH==OS_TRICORE)
#include <TRICORE/Os_api_TRICORE.h>
#elif (OS_ARCH==OS_RH850)
#include <RH850/Os_api_RH850.h>
#elif (OS_ARCH==OS_NEWARCH)
#include <NEWARCH/Os_api_NEWARCH.h>
#elif (OS_ARCH==OS_PIKEOS)
#include <PIKEOS/Os_api_PIKEOS.h>
#elif (OS_ARCH==OS_WINDOWS)
#include <WINDOWS/Os_api_WINDOWS.h>
#elif (OS_ARCH==OS_ARM)
#include <ARM/Os_api_ARM.h>
#elif (OS_ARCH==OS_ARM64)
#include <ARM64/Os_api_ARM64.h>
#elif (OS_ARCH==OS_LINUX)
#include <LINUX/Os_api_LINUX.h>
#elif (OS_ARCH==OS_CORTEXM)
#include <CORTEXM/Os_api_CORTEXM.h>
#elif (OS_ARCH==OS_ARC)
#include <ARC/Os_api_ARC.h>
#else
#error "Unsupported OS_ARCH defined. Check your Makefiles!"
#endif


#endif /* OS_API_ARCH_H */

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
