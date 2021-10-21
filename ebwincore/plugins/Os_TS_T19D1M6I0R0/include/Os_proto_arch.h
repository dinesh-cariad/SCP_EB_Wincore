/* Os_proto_arch.h
 *
 * This file includes the appropriate architecture prototypes file depending
 * on the chosen architecture.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_proto_arch.h 30593 2019-07-30 14:10:36Z beoe851 $
*/
#ifndef OS_PROTO_ARCH_H
#define OS_PROTO_ARCH_H

#if (OS_ARCH==OS_PA)
#include <PA/Os_proto_PA.h>
#elif (OS_ARCH==OS_TRICORE)
#include <TRICORE/Os_proto_TRICORE.h>
#elif (OS_ARCH==OS_RH850)
#include <RH850/Os_proto_RH850.h>
#elif (OS_ARCH==OS_NEWARCH)
#include <NEWARCH/Os_proto_NEWARCH.h>
#elif (OS_ARCH==OS_PIKEOS)
#include <PIKEOS/Os_proto_PIKEOS.h>
#elif (OS_ARCH==OS_WINDOWS)
#include <WINDOWS/Os_proto_WINDOWS.h>
#elif (OS_ARCH==OS_ARM)
#include <ARM/Os_proto_ARM.h>
#elif (OS_ARCH==OS_ARM64)
#include <ARM64/Os_proto_ARM64.h>
#elif (OS_ARCH==OS_LINUX)
#include <LINUX/Os_proto_LINUX.h>
#elif (OS_ARCH==OS_CORTEXM)
#include <CORTEXM/Os_proto_CORTEXM.h>
#elif (OS_ARCH==OS_ARC)
#include <ARC/Os_proto_ARC.h>
#else
#error "Unsupported OS_ARCH defined. Check your Makefiles!"
#endif

#endif
/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
