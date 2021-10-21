/* Os_libconfig.h
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
 * $Id: Os_libconfig.h 27282 2017-08-24 13:57:43Z olme8414 $
*/

#ifndef OS_LIBCONFIG_H
#define OS_LIBCONFIG_H

/* Include the generated libconfig. header
*/
#include <Os_libcfg.h>

/* Include patch header file if indicated.
 * We set the macro OS_PATCH_LIBCONFIG to 1 to indicate to the included file that we
 * are patching the library configuration.
 * Afterwards, we undefine the macro; this is a MISRA violation. The alternative
 * would be to have separate patch files and separate control macros for including
 * them, which would complicate matters for the build system and therefore for the user.
*/
#ifdef OS_INCLUDE_PATCHES

#define OS_PATCH_LIBCONFIG	1
#include <Os_config_patches.h>
#undef OS_PATCH_LIBCONFIG

#endif

#endif
/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
