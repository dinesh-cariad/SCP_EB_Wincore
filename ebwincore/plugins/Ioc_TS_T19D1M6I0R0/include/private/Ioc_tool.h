/* Ioc_tool.h - toolchain abstraction
 *
 * Maps toolchain abstraction macros based on the implementations provided
 * by the Os.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_tool.h 2964 2018-11-14 14:05:00Z olme8414 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 20.10 (advisory)
 *  The # and ## preprocessor operators should not be used.
 *
 * Reason:
 *  Automatic construction of symbols in a consistent manner.
*/

/* DCG Deviations:
 *
 * DCG-1) Deviated Rule: [OS_CHARSET_010]
 *  This file contains the byte '$' (0x24) which is only permitted inside comments!
 *
 * Reason:
 *  We must use the $$ characters here because they are part of the region-related
 *  symbol names defined in the ARM KEIL toolchain.
*/
#ifndef IOC_TOOL_H
#define IOC_TOOL_H

#include <public/Ioc_libconfig.h>


#if (IOC_LCFG_KERNEL_TYPE == IOC_MICROKERNEL)


#include <private/Mk_tool.h>

#define IOC_PARAM_UNUSED(x) MK_PARAM_UNUSED(x)
#define IOC_OFFSETOF(type, member) MK_OFFSETOF(type, member)


#elif (IOC_LCFG_KERNEL_TYPE == IOC_AUTOCOREOS)


#include <Os_tool.h>

#define IOC_PARAM_UNUSED(x) OS_PARAM_UNUSED(x)
#define IOC_OFFSETOF(type, member) OS_OFFSETOF(type, member)


#else /* IOC_LCFG_KERNEL_TYPE */
#error "Unknown kernel type."
#endif /* IOC_LCFG_KERNEL_TYPE */


/* Deviation MISRAC2012-1 <START> */
#if (IOC_TOOL == IOC_armkeil)

/* Deviation DCG-1 <START> */

/* IOC_MR_START_ADR - encapsulates the linker-generated start address of the
 *                    provided memory section that can be used by the IOC module
 *                    for data transfer
 * name: name of the application which uses the IOC module
*/
#define IOC_MR_START_ADR(name)			(Image$$ ## IOC_RSA_READABLE_ ## name ## $$Base)

/* IOC_MR_LIMIT_ADR - encapsulates the linker-generated end address of the
 *                    provided memory section that can be used by the IOC module
 *                    for data transfer
 * name: name of the application which uses the IOC module
*/
#define IOC_MR_LIMIT_ADR(name)			(Image$$ ## IOC_RLA_READABLE_ ## name ## $$Base)

/* Deviation DCG-1 <STOP> */

#else /* IOC_TOOL != IOC_armkeil */

/* IOC_MR_START_ADR - encapsulates the linker-generated start address of the
 *                    provided memory section that can be used by the IOC module
 *                    for data transfer
 * name: name of the application which uses the IOC module
*/
#define IOC_MR_START_ADR(name)			(IOC_RSA_READABLE_ ## name)

/* IOC_MR_LIMIT_ADR - encapsulates the linker-generated end address of the
 *                    provided memory section that can be used by the IOC module
 *                    for data transfer
 * name: name of the application which uses the IOC module
*/
#define IOC_MR_LIMIT_ADR(name)			(IOC_RLA_READABLE_ ## name)

#endif /* IOC_TOOL */
/* Deviation MISRAC2012-1 <STOP> */


#endif /* IOC_TOOL_H */
/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
