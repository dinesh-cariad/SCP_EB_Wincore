/* Ioc_WINDOWS_core.h - WINDOWS core header
 *
 * This file contains definitions for features that are common across all
 * the WINDOWS architectures.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_WINDOWS_core.h 1813 2017-02-15 12:31:22Z mist9353 $
*/
#ifndef IOC_WINDOWS_CORE_H
#define IOC_WINDOWS_CORE_H

#include <public/Ioc_basic_types.h>

/* On IA32 (or AMD64) all fields of ioc_extinput_t can be read from any alignment.
*/
#define IOC_HwWellAlignedExtInput(address)	IOC_TRUE

#endif /* IOC_WINDOWS_CORE_H */

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
