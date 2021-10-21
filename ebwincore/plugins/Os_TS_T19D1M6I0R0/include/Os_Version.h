/* Os_Version.h
 *
 * This file defines the version numbers required by the Autosar OS specification
 *
 * !LINKSTO Kernel.Autosar.Identification, 2
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_Version.h.m4 29397 2018-11-05 15:29:53Z aniv261689 $
*/
#ifndef OS_VERSION_H
#define OS_VERSION_H

#include <Os_defs.h>

/** \brief definition of the unique vendor ID
 *
 * This vendor ID is based on the vendor ID published by the HIS.
 * It should be used for each module
*/
#ifndef OS_VENDOR_ID_3SOFT
#define OS_VENDOR_ID_3SOFT	0x01
#endif

/* Error checking:
 * Ensure that none of the macros we define here have already been defined
 * elsewhere.
*/

#if defined(OS_VENDOR_ID)
#error "OS_VENDOR_ID is already defined"
#endif

#if defined(OS_MODULE_ID)
#error "OS_MODULE_ID is already defined"
#endif

#if defined(OS_AR_MAJOR_VERSION)
#error "OS_AR_MAJOR_VERSION is already defined"
#endif

#if defined(OS_AR_MINOR_VERSION)
#error "OS_AR_MINOR_VERSION is already defined"
#endif

#if defined(OS_AR_PATCH_VERSION)
#error "OS_AR_PATCH_VERSION is already defined"
#endif

#if defined(OS_AR_RELEASE_MAJOR_VERSION)
#error "OS_AR_RELEASE_MAJOR_VERSION is already defined"
#endif

#if defined(OS_AR_RELEASE_MINOR_VERSION)
#error "OS_AR_RELEASE_MINOR_VERSION is already defined"
#endif

#if defined(OS_AR_RELEASE_REVISION_VERSION)
#error "OS_AR_RELEASE_REVISION_VERSION is already defined"
#endif

#if defined(OS_SW_MAJOR_VERSION)
#error "OS_SW_MAJOR_VERSION is already defined"
#endif

#if defined(OS_SW_MINOR_VERSION)
#error "OS_SW_MINOR_VERSION is already defined"
#endif

#if defined(OS_SW_PATCH_VERSION)
#error "OS_SW_PATCH_VERSION is already defined"
#endif


/* generic definitions for the OS module */
#define OS_VENDOR_ID		OS_VENDOR_ID_3SOFT
#define OS_MODULE_ID		0x01

#define OS_SW_MAJOR_VERSION		6U
#define OS_SW_MINOR_VERSION		0U
#define OS_SW_PATCH_VERSION		302U


/* the defines needed depend on the AUTOSAR release we're conforming to */
#if (OS_RELEASE_SUFFIX == OS_AS40)

#define OS_AR_RELEASE_MAJOR_VERSION		4
#define OS_AR_RELEASE_MINOR_VERSION		0
#define OS_AR_RELEASE_REVISION_VERSION	2

#elif (OS_RELEASE_SUFFIX == OS_AS403)

#define OS_AR_RELEASE_MAJOR_VERSION		4
#define OS_AR_RELEASE_MINOR_VERSION		0
#define OS_AR_RELEASE_REVISION_VERSION	3

#else
#error "Unsupported OS_RELEASE_SUFFIX defined. Check your Makefiles!"
#endif

#endif /* OS_VERSION_H */
