/* Ioc_libconfig.h
 *
 * This file includes the library configuration. Currently the only parameter is:
 *   * IOC_LCFG_KERNEL_TYPE: Tells whether this IOC is compiled for a Safety OS or an AutoCore OS.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_libconfig.h 1338 2016-11-30 08:42:45Z thob2262 $
*/
#ifndef IOC_LIBCONFIG_H
#define IOC_LIBCONFIG_H

#include <Ioc_gen_libcfg.h>

#ifndef IOC_LCFG_KERNEL_TYPE
#error "IOC_LCFG_KERNEL_TYPE is not set. Check your Ioc_gen_libcfg.h file."
#endif

#endif /* IOC_LIBCONFIG_H */

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
