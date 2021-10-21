/* Ioc_defs.h
 *
 * This file contains identifying definitions for the known processor families and toolchains,
 * for use when defining IOC_CPUFAMILY and IOC_TOOL.
 *
 * The Makefiles must ensure that these macros are defined appropriately on
 * the command line.
 *
 * Example: -DIOC_TOOL=IOC_gnu -DIOC_CPUFAMILY=IOC_PA
 *
 * IOC_CPU specifies the derivative, which is used. The definitions
 * for the CPU are handled in the hardware-specific files. Overlap
 * of allocated numbers between processor families is not a problem, because
 * the processor files will/should never be mixed.
 *
 * The value 0 is never used, because that is the value that preprocessors
 * return for undefined symbols.
 *
 * CAVEAT:
 * The presence of a toolchain or processor here doesn't imply a commitment
 * to support the toolchain or processor in the future.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_defs.h 3259 2019-07-31 11:59:00Z beoe851 $
*/
#ifndef IOC_DEFS_H
#define IOC_DEFS_H

#ifndef IOC_TOOL
#error "IOC_TOOL is not defined. Check your Makefiles!"
#endif

#ifndef IOC_CPUFAMILY
#error "IOC_CPUFAMILY is not defined. Check your Makefiles!"
#endif

/* Known toolchains
 *
 * In EB tradition, toolchain names in the makefiles are lowercase
 *
 * Note: presence of a toolchain in this list does not imply a commitment to provide support
 * for that toolchain in any future version of the IOC. The supported toolchains
 * are described in the release notes.
*/
#define IOC_gnu			1
#define IOC_diab		2
#define IOC_tasking		3
#define IOC_ghs			4
#define IOC_cosmic		5
#define IOC_mwerks		6
#define IOC_softune		7
#define IOC_realview	8
#define IOC_iar			9
#define IOC_vc			10
#define IOC_renesas		11
#define IOC_cw			12
#define IOC_ticgt		13
#define IOC_armkeil		16

/* Known processor families.
 *
 * Note: presence of a processor family in this list does not imply a commitment to provide support
 * for that processor family in any future version of the IOC.
*/
#define IOC_PA			1
#define IOC_TRICORE		2
#define IOC_V850		3
#define IOC_ARM			4
#define IOC_RH850		5
#define IOC_WINDOWS		6
#define IOC_ARM64		7
#define IOC_CORTEXM		8
#define IOC_ARC			9
#define IOC_BLANKFAMILY	99

/* Kernel type values of IOC_LCFG_KERNEL_TYPE:
 *  The IOC is either part of a Safety OS (IOC_MICROKERNEL)
 *  or part of an AutoCore OS (IOC_AUTOCOREOS).
*/
#define IOC_MICROKERNEL	1
#define IOC_AUTOCOREOS	2

#endif

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
