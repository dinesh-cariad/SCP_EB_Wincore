/* Os_configuration.h
 *
 * This file provides a defined interface from the kernel and from user
 * code to the generated include file.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_configuration.h 27282 2017-08-24 13:57:43Z olme8414 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 20.5 (advisory)
 * #undef should not be used.
 *
 * Reason:
 * A macro is undefined to simplify matters for the build system and ultimately for the end user.
 */

#ifndef OS_CONFIGURATION_H
#define OS_CONFIGURATION_H

/* Include the generated config. header
*/
#include <Os_config.h>

#ifndef OS_IOC_ENABLED
#define OS_IOC_ENABLED 0
#endif

#ifndef OS_ASM
#if (OS_IOC_ENABLED==1)
#include <Ioc_gen.h>
#endif
#endif


/* Include patch header file if indicated.
 * We set the macro OS_PATCH_CONFIG to 1 to indicate to the included file that we
 * are patching the configuration.
 * Afterwards, we undefine the macro; this is a MISRA violation. The alternative
 * would be to have separate patch files and separate control macros for including
 * them, which would complicate matters for the build system and therefore for the user.
*/
/* Deviation MISRAC2012-1 <START> */
#ifdef OS_INCLUDE_PATCHES

#define OS_PATCH_CONFIG	1
#include <Os_config_patches.h>
#undef OS_PATCH_CONFIG

#endif
/* Deviation MISRAC2012-1 <STOP> */

#endif
/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
