/* Ioc_extentry_common.h
 *
 * This file declares functions used by the buffer/queue handler to transfer extended entries.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_extentry_common.h 1338 2016-11-30 08:42:45Z thob2262 $
*/
#ifndef IOC_EXTENTRY_COMMON_H
#define IOC_EXTENTRY_COMMON_H

#include <private/Ioc_types_forward.h>
#include <public/Ioc_basic_types.h>

ioc_return_t IOC_ReadFromExtEntry(const ioc_config_t *, const void *, ioc_ilength_t *, void * const *);

#endif
