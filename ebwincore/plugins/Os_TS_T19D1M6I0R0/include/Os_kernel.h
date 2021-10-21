/* Os_kernel.h
 *
 * Kernel include file. This file includes all the files that define the
 * kernel data structures.
 *
 * The common data structures, macros etc are no longer defined directly in
 * this file because of the MISRA complaint when the architecture-
 * specific prototypes file is included at the end. The kernel structures
 * can now be found in Os_kernel_task.h, Os_kernel_alarm.h and Os_kernel_app.h
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_kernel.h 27282 2017-08-24 13:57:43Z olme8414 $
*/
#ifndef OS_KERNEL_H
#define OS_KERNEL_H

#include <Os_defs.h>

/* Include the architecture-specific definitions first.
*/
#include <Os_types.h>
#include <Os_libconfig.h>

#if (OS_KERNEL_TYPE == OS_MICROKERNEL)

/* The microkernel defines it's own
 *  error codes, applications and tasks.
*/
#include <Os_microkernel.h>
#include <Os_arch.h>
#include <Os_optimize.h>
#include <Os_kernel_multicore.h>
#include <Os_kernel_alarm.h>

#else

#include <Os_error.h>
#include <Os_arch.h>
#include <Os_optimize.h>
#include <Os_kernel_multicore.h>
#include <Os_kernel_app.h>
#include <Os_kernel_alarm.h>
#include <Os_kernel_task.h>

#endif

#include <Os_tool.h>
#include <Os_userkernel.h>

#if OS_KERNEL_TYPE != OS_MICROKERNEL
/* These are included last because the function prototypes that are in
 * them will need the types that are defined in this file.
*/
#include <Os_proto_arch.h>
#include <Os_callouts.h>
#endif

/* Include timestamp definitions
*/
#include <Os_timestamp.h>

#endif
/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
