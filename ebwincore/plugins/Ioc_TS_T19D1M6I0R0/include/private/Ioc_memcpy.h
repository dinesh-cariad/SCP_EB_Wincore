/* Ioc_memcpy.h
 *
 * This file defines the memory copy function for the IOC.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_memcpy.h 97 2015-10-20 15:16:45Z mist9353 $
*/


#ifndef  IOC_MEMCPY_H
#define  IOC_MEMCPY_H

#include <public/Ioc_basic_types.h>

void IOC_MemCpy(void *, const void * const, ioc_uint32_t);

#endif /* IOC_MEMCPY_H */
