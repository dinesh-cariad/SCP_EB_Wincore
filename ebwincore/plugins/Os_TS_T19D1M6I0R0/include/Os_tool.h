/* Os_tool.h
 *
 * This file includes the appropriate ARCH_tool.h include file depending
 * on the chosen architecture.
 *
 * The Makefiles must ensure that the OS_ARCH and OS_CPU macros are
 * defined appropriately on the command line.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_tool.h 31452 2020-02-21 13:17:03Z waab271441 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 20.11 (required)
 * A macro paramter immediately following a # operator shall not immediately be
 * followed by a ## operator.
 *
 * Reason:
 * The order in which the ## operators are evaluated is not an issue here.
 * Moreover the macro doesn't use # operators, so operator precedence neither is an issue.
 *
 * MISRAC2012-2) Deviated Rule: 20.10 (advisory)
 * The # and ## preprocessor operators should not be used.
 *
 * Reason:
 * The ## operator is used here so we don't have to add macros for all combinations of prefix, coreId and suffix.
 * This avoids copy paste mistakes and makes the code easier to maintain, while supporting all needed combinations.
*/

#ifndef OS_TOOL_H
#define OS_TOOL_H

#include <Os_defs.h>

#if (OS_ARCH==OS_PA)
#include <PA/Os_tool_PA.h>
#elif (OS_ARCH==OS_TRICORE)
#include <TRICORE/Os_tool_TRICORE.h>
#elif (OS_ARCH==OS_RH850)
#include <RH850/Os_tool_RH850.h>
#elif (OS_ARCH==OS_NEWARCH)
#include <NEWARCH/Os_tool_NEWARCH.h>
#elif (OS_ARCH==OS_PIKEOS)
#include <PIKEOS/Os_tool_PIKEOS.h>
#elif (OS_ARCH==OS_WINDOWS)
#include <WINDOWS/Os_tool_WINDOWS.h>
#elif (OS_ARCH==OS_ARM)
#include <ARM/Os_tool_ARM.h>
#elif (OS_ARCH==OS_ARM64)
#include <ARM64/Os_tool_ARM64.h>
#elif (OS_ARCH==OS_LINUX)
#include <LINUX/Os_tool_LINUX.h>
#elif (OS_ARCH==OS_CORTEXM)
#include <CORTEXM/Os_tool_CORTEXM.h>
#elif (OS_ARCH==OS_ARC)
#include <ARC/Os_tool_ARC.h>
#else
#error "Unsupported OS_ARCH defined. Check your Makefiles!"
#endif

/* In case static checks are performed some annotations must be hidden otherwise some tools trip over them,
 * e.g. report false positives.
 */
#ifndef EB_STATIC_CHECK

/*!
 * OS_SECTION_ATTRIB_PRE
 * OS_SECTION_ATTRIB_POST
 * OS_SECTION_PRAGMA
 *
 * Section managment macros.
 *
 * Note that code analysis or code instrumentation tools might not be able to parse these macros,
 * so exclude them from being parsed.
 *
 * If a variable 'var' needs to be in a specific section 'var_section' the following construction shall be used:
 *
 *     OS_SECTION_PRAGMA(var, var_section)
 *     os_var_t OS_SECTION_ATTRIB_PRE(var_section) var OS_SECTION_ATTRIB_POST(var_section);
 */
#if ((OS_TOOL==OS_htgcc) || (OS_TOOL==OS_gnu) || (OS_TOOL==OS_realview)			\
	 || (OS_TOOL==OS_ghs) || (OS_TOOL==OS_mplabx) || (OS_TOOL==OS_tasking)		\
	 || (OS_TOOL==OS_ticgt) || (OS_TOOL==OS_armkeil) || (OS_TOOL==OS_arm5))		\
	 || (OS_TOOL==OS_metaware)

#ifndef OS_SECTION_ATTRIB_PRE
#define OS_SECTION_ATTRIB_PRE(sec) /* nothing */
#endif

#ifndef OS_SECTION_ATTRIB_PRE_DECL
#define OS_SECTION_ATTRIB_PRE_DECL(sec) /* nothing */
#endif

#ifndef OS_SECTION_ATTRIB_POST
#define OS_SECTION_ATTRIB_POST(sec)		__attribute__ ((section(#sec)))
#endif

#ifndef OS_SECTION_ATTRIB_POST_DECL
#define OS_SECTION_ATTRIB_POST_DECL(sec) /* nothing */
#endif

#ifndef OS_SECTION_PRAGMA
#define OS_SECTION_PRAGMA(var, sec)	/* nothing */
#endif

#ifndef OS_ALIGNED_ATTRIB_PRE
#define OS_ALIGNED_ATTRIB_PRE(granule) /* nothing */
#endif

#if (OS_TOOL == OS_tasking)
#ifndef OS_ALIGNED_ATTRIB_POST
#define OS_ALIGNED_ATTRIB_POST(granule)		__attribute__ ((__align (granule)))
#endif

#else

#ifndef OS_ALIGNED_ATTRIB_POST
#define OS_ALIGNED_ATTRIB_POST(granule)		__attribute__ ((aligned (granule)))
#endif
#endif

#ifndef OS_ALIGNED_ATTRIB_PRE_DECL
#define OS_ALIGNED_ATTRIB_PRE_DECL(granule)	/* nothing */
#endif

#ifndef OS_ALIGNED_PRAGMA
#define OS_ALIGNED_PRAGMA(var, granule)	/* nothing */
#endif

/* OS_GEN_STACK generates a variable definition for a stack.
 * var: Name of the variable
 * sec: Section, in which the variable should be put.
 * size: Size of the stack array.
 * granule: Align stack to this granule.
*/
#ifndef OS_GEN_STACK
#define OS_GEN_STACK(var, sec, size, granule)					\
	os_stackelement_t var[size]									\
	OS_SECTION_ATTRIB_POST(sec) OS_ALIGNED_ATTRIB_POST(granule)	\
	OS_STACK_ATTRIB()
#endif

#elif (OS_TOOL==OS_diab)
#ifndef OS_SECTION_ATTRIB_PRE
#define OS_SECTION_ATTRIB_PRE(sec)		__attribute__ ((section(#sec)))

/* Diab compiler and preprocessor are very pedantic about preprocessor
 * tokens and have problems with creating invalid tokens. Using string 
 * literals will cause the quotes to be escaped during the follow-up stringification,
 * leading to conflicting section definitions or generate a space leading to generation of
 * multiple tokens.
 * Therefore we have to do it the verbose way...
 */
#define OS_SECTIONNAME_CONCAT(a, b) OS_SECTION_ ## a ## _ ## b
#define OS_SECTIONNAME_INNER(prefix, coreId, suffix)	OS_SECTIONNAME_CONCAT(coreId, suffix)

#if (OS_N_CORES_MAX > 4)
#error "OS_SECTION defines need to be expanded to support more than 4 cores!"
#endif

#define OS_SECTION_0_kernelData			.data.core0.kernelData
#define OS_SECTION_0_taskActivations	.bss.core0.taskActivations
#define OS_SECTION_0_prioritySlot		.bss.core0.prioritySlot
#define OS_SECTION_0_slavePrioWord		.bss.core0.slavePrioWord
#define OS_SECTION_1_kernelData			.data.core1.kernelData
#define OS_SECTION_1_taskActivations	.bss.core1.taskActivations
#define OS_SECTION_1_prioritySlot		.bss.core1.prioritySlot
#define OS_SECTION_1_slavePrioWord		.bss.core1.slavePrioWord
#define OS_SECTION_2_kernelData			.data.core2.kernelData
#define OS_SECTION_2_taskActivations	.bss.core2.taskActivations
#define OS_SECTION_2_prioritySlot		.bss.core2.prioritySlot
#define OS_SECTION_2_slavePrioWord		.bss.core2.slavePrioWord
#define OS_SECTION_3_kernelData			.data.core3.kernelData
#define OS_SECTION_3_taskActivations	.bss.core3.taskActivations
#define OS_SECTION_3_prioritySlot		.bss.core3.prioritySlot
#define OS_SECTION_3_slavePrioWord		.bss.core3.slavePrioWord

#endif /* OS_TOOL==OS_diab */

#ifndef OS_SECTION_ATTRIB_PRE_DECL
#define OS_SECTION_ATTRIB_PRE_DECL(sec)	OS_SECTION_ATTRIB_PRE(sec)
#endif

#ifndef OS_SECTION_ATTRIB_POST
#define OS_SECTION_ATTRIB_POST(sec) /* nothing */
#endif

#ifndef OS_SECTION_ATTRIB_POST_DECL
#define OS_SECTION_ATTRIB_POST_DECL(sec) /* nothing */
#endif

#ifndef OS_SECTION_PRAGMA
#define OS_SECTION_PRAGMA(var, sec)	/* nothing */
#endif

#ifndef OS_ALIGNED_ATTRIB_PRE
#define OS_ALIGNED_ATTRIB_PRE(granule)		__attribute__ ((aligned(granule)))
#endif

#ifndef OS_ALIGNED_ATTRIB_POST
#define OS_ALIGNED_ATTRIB_POST(granule) /* nothing */
#endif

#ifndef OS_ALIGNED_ATTRIB_PRE_DECL
#define OS_ALIGNED_ATTRIB_PRE_DECL(granule)	OS_ALIGNED_ATTRIB_PRE(granule)
#endif

#ifndef OS_ALIGNED_PRAGMA
#define OS_ALIGNED_PRAGMA(var, granule)	/* nothing */
#endif

#if (OS_ARCH == OS_PA)

#if (OS_CPU == OS_QUASAR3) /* Placement here suppresses warnings on yet another version of the diab compiler. */

/* for some reasons some versions of the diab compiler need two __attribute__ statements
 * and others only one with a list of attributes
 */
#ifndef OS_GEN_STACK
#define OS_GEN_STACK(var, sec, size, granule)	\
	OS_SECTION_ATTRIB_PRE(sec)				\
	OS_ALIGNED_ATTRIB_PRE(granule)			\
	os_stackelement_t							\
		(var)[(size)]
#endif

#endif /* OS_CPU == OS_QUASAR3 */
#endif /* OS_ARCH == OS_PA */

#ifndef OS_GEN_STACK
#define OS_GEN_STACK(var, sec, size, granule)	\
	os_stackelement_t							\
		__attribute__((aligned(granule), section(#sec)))	\
		(var)[(size)]
#endif

#ifndef OS_GEN_STACK_DECL
#define OS_GEN_STACK_DECL(var, sec, size, granule)		\
	OS_GEN_STACK(var, sec, size, granule)
#endif

#elif (OS_TOOL==OS_iar)

#define OS_TOOL_EMIT_PRAGMA(p)				_Pragma(#p)
#define OS_SECTION_ATTRIB_PRE(sec)			/* nothing */
#define OS_SECTION_ATTRIB_PRE_DECL(sec)		/* nothing */
#define OS_SECTION_ATTRIB_POST(sec)			@ #sec
#define OS_SECTION_ATTRIB_POST_DECL(sec)	/* nothing */
#define OS_SECTION_PRAGMA(var, sec)			/* nothing */
#define OS_ALIGNED_ATTRIB(var, granule)		/* nothing */
#define OS_ALIGNED_ATTRIB_PRE(granule)		/* nothing */
#define OS_ALIGNED_ATTRIB_POST(granule)		/* nothing */
#define OS_ALIGNED_ATTRIB_PRE_DECL(granule)	/* nothing */
#define OS_ALIGNED_PRAGMA(granule)			OS_TOOL_EMIT_PRAGMA(data_alignment=granule)

#define OS_GEN_STACK(var, sec, size, granule)			\
	OS_ALIGNED_PRAGMA(granule)							\
	__no_init os_stackelement_t var[size] @ #sec

/* Declaration without Pragma operator. */
#define OS_GEN_STACK_DECL(var, sec, size, granule)		\
	__no_init os_stackelement_t var[size] @ #sec

/* IAR needs its own definition of OS_SECTIONNAME_INNER. */
/* Deviation MISRAC2012-1, MISRAC2012-2 <1> */
#define OS_SECTIONNAME_INNER(prefix, coreId, suffix)	#prefix".core"#coreId"."#suffix

#else
#error "The section management macros OS_SECTION_ATTRIB and OS_SECTION_PRAGMA are not defined."
#endif

/*!
 * OS_VAR_UNINITIALIZED_ATTRIB
 *
 * The OS_VAR_UNINITIALIZED_ATTRIB denotes explicitely that a variable
 * is not to be placed in an initialized data section.
 * Required for RealView 4.1 if a section name is specified: In this case
 * RealView will mark this section as ALLOC, CONTENTS, DATA, LOAD, which
 * means that space in the ROM is consumed. Using this attribute will
 * generate an ALLOC-only section.
 */

#if (OS_TOOL==OS_realview)

#ifndef OS_VAR_UNINITIALIZED_ATTRIB
#define OS_VAR_UNINITIALIZED_ATTRIB()	__attribute__((zero_init))
#endif

#else

#define OS_VAR_UNINITIALIZED_ATTRIB()

/* OS_STACK_ATTRIB: Add __attribute__s for stacks. By default, we mark stacks
 * as uninitialized data, but CPU-specific toolchain headers may override this
 * functionality.
 */
#ifndef OS_STACK_ATTRIB
#define OS_STACK_ATTRIB() OS_VAR_UNINITIALIZED_ATTRIB()
#endif

#endif /* OS_TOOL==OS_realview */

#else /* EB_STATIC_CHECK */

#define OS_SECTION_ATTRIB_PRE(sec)
#define OS_SECTION_ATTRIB_PRE_DECL(sec)
#define OS_SECTION_ATTRIB_POST(sec)
#define OS_SECTION_ATTRIB_POST_DECL(sec)
#define OS_SECTION_ATTRIB(sect)
#define OS_SECTION_PRAGMA(var, sec)
#define OS_VAR_UNINITIALIZED_ATTRIB()
#define OS_ALIGNED_ATTRIB_PRE(granule)
#define OS_ALIGNED_ATTRIB_POST(granule)
#define OS_ALIGNED_ATTRIB_PRE_DECL(granule)
#define OS_ALIGNED_ATTRIB(var, granule)
#define OS_ALIGNED_PRAGMA(var, granule)
#define OS_STACK_ATTRIB() OS_VAR_UNINITIALIZED_ATTRIB()
#define OS_GEN_STACK(var, sec, size, granule)	os_stackelement_t (var)[(size)]

#endif /* EB_STATIC_CHECK */


/* Build section names like '.bss.core0.foo' by prefix, coreId and suffix */
#ifndef OS_SECTIONNAME_INNER
/* Deviation MISRAC2012-1, MISRAC2012-2 <1> */
#define OS_SECTIONNAME_INNER(prefix, coreId, suffix)	prefix.core##coreId.suffix
#endif
#define OS_SECTIONNAME(prefix, coreId, suffix)			OS_SECTIONNAME_INNER(prefix, coreId, suffix)

/* OS_GEN_STACK_DECL: Corresponding declaration to OS_GEN_STACK.
 * By default it is identical, but some toolchains may need to override
 * this due to the prepended extern keyword.
*/
#ifndef OS_GEN_STACK_DECL
#define OS_GEN_STACK_DECL	OS_GEN_STACK
#endif

/*!
 * OS_PARAM_UNUSED
 *
 * Mark parameter p as unused parameter (or variable).
 * Usage: Function-like.
 */
#ifndef OS_PARAM_UNUSED
#error "OS_PARAM_UNUSED must be defined by the toolchain header."
#endif

/*!
 * OS_OFFSETOF(type, member)
 *
 * Determines the offset of structure member "member" of type "type".
 *
 * The 0 cast to a pointer isn't a NULL dereference, since we only take the
 * address and then cast it to mk_address_t.
 */
#ifndef OS_OFFSETOF
#ifndef EB_STATIC_CHECK
#define OS_OFFSETOF(type, member)	\
	((os_address_t)((os_char_t const volatile *)&((type *)0)->member - (os_char_t const volatile *)0))
#else /* defined(EB_STATIC_CHECK) */
#define OS_OFFSETOF(type, member)	0u
#endif /* !defined(EB_STATIC_CHECK) */
#endif /* !defined(OS_OFFSETOF) */

/*!
 * OS_ENDLESSLOOP()
 *
 * Executes an endless loop, implemented in a way that avoids compiler warnings on
 * infinite loops.
 */
#ifndef OS_ENDLESSLOOP
#define OS_ENDLESSLOOP() \
	do \
	{ \
		os_unsigned_t confusion; \
		for(confusion=0u; confusion!=1u; confusion^=2u) \
		{ \
			/* Compiler hopefully does not recognize this as an endless loop */ \
		} \
	} while(0)
#endif

/* Macros to abstract memory region addresses.
 *
 * A memory region is formed from the following components:
 * - initialized data (VMA) with ROM-image (LMA) of initial values,
 * - uninitialized data (VMA).
 * Both components are optional, however data must always have lower addresses
 * than uninitialized data.
 *
 * The following symbols must be defined for memory regions:
 * __DATA_<region>: Start-VMA of of memory region.
 * __DEND_<region>: End-VMA of memory region.
 * __IDAT_<region>: Start LMA of initialized data portion.
 * __IEND_<region>: End LMA of initialized data portion.
 *
 * All of this is toolchain-specific, hence we want to exclude static C checks.
*/
#ifndef EB_STATIC_CHECK
/* Macro that evaluates to the name of the linker generated symbol referring
 * to the Start-VMA of memory region <region>.
 *
 * Note:  This is the default implementation, it may be changed in the toolchain header!
 */
#ifndef OS_TOOL_MR_START
#define OS_TOOL_MR_START(region) __DATA_ ## region
#endif

/* Macro that evaluates to the name of the linker generated symbol referring
 * to the virtual memory address one byte beyond memory region <region>.
 *
 * Note:  This is the default implementation, it may be changed in the toolchain header!
 */
#ifndef OS_TOOL_MR_END
#define OS_TOOL_MR_END(region) __DEND_ ## region
#endif

/* Macro that evaluates to the name of the linker generated symbol referring
 * to the LMA of the data portion of memory region <region>.
 *
 * Note:  This is the default implementation, it may be changed in the toolchain header!
 */
#ifndef OS_TOOL_MR_LMA_START
#define OS_TOOL_MR_LMA_START(region) __IDAT_ ## region
#endif


/* Macro that evaluates to the name of the linker generated symbol referring
 * to the LMA one byte beyond the initialized data portion of memory region
 * <region>.
 *
 * Note:  This is the default implementation, it may be changed in the toolchain header!
 */
#ifndef OS_TOOL_MR_LMA_END
#define OS_TOOL_MR_LMA_END(region) __IEND_ ## region
#endif

#else /* EB_STATIC_CHECK */
/* Evaluate to a value that the static checker understands.
 * Since the address of the (otherwise) linker-generated symbol is taken,
 * use a variable that is always present instead.
*/
#define OS_TOOL_MR_START(region)		(OS_appMode)
#define OS_TOOL_MR_END(region)			(OS_appMode)
#define OS_TOOL_MR_LMA_START(region)	(*((os_uint8_t const*)(&OS_appMode)))
#define OS_TOOL_MR_LMA_END(region)		(*((os_uint8_t const*)(&OS_appMode)))
#endif

/* GNU and IAR can use inline assembly for memory barriers */
#ifndef EB_STATIC_CHECK
#if (OS_TOOL == OS_gnu) || (OS_TOOL == OS_iar)
#define OS_CompilerMemBarrier(p)		__asm__ volatile("" :: "r" (p) : "memory")
#endif
#endif

/* If the compiler provides no way to define a compiler memory barrier,
 * we call OS_NullFunction, which also serves as optimisation barrier.
 * Note: This is the generic fallback implementation. It may get overridden in the
 * CPU-family-specific toolchain headers.
*/
#ifndef OS_CompilerMemBarrier
#define OS_CompilerMemBarrier(p)		OS_NullFunction()
#endif

/* OS_STATIC_ASSERT - resembles C11's _Static_assert
 *
 * Parameters:
 *  - cond: The condition, which is meant to be checked at compile time.
 *  - id  : The identifier of this assert.
 *          It must not collide other identifiers (like global variables, functions, typedefs).
 *          The identifier replaces the error message of C11's static assert.
 *
 * This macro must cause a compile error, if the given condition is not met.
 * In this case, this macro tries to define an array, which has a negative size.
 * Defining an array type with negative size should cause a compile error.
 * To avoid consuming memory this is just a typedef.
*/
#define OS_STATIC_ASSERT(cond, id) typedef os_uint8_t (id)[(cond)? 1 : -1]

#define OS_SWITCH_ENDIANESS_32(val, dest)															\
	do {																							\
		os_uint32_t swapped_value = (((val) << 8u) & 0xff00ff00u) | (((val) >> 8u) & 0xff00ffu);	\
		swapped_value = (swapped_value >> 16u) | (swapped_value << 16u);							\
		(dest) = swapped_value;																		\
	} while (0)

#ifndef EB_STATIC_CHECK
/* OS_TOOL_INLINE_KEYWORD
 *
 * This macro is expanded to the inline keyword and shall work even in
 * strict-ANSI mode. So, most toolchains offer variants like __inline__ or
 * __inline, which have the same semantics and also work in strict-ANSI mode.
 */
#ifndef OS_TOOL_INLINE_KEYWORD
#error "The macro OS_TOOL_INLINE_KEYWORD is not defined."
#endif

/* OS_TOOL_ASM_KEYWORD
 *
 * This macro expands to the asm keyword for use with inline-assembly-language
 * statements.
 */
#ifndef OS_TOOL_ASM_KEYWORD
#error "The macro OS_TOOL_ASM_KEYWORD is not defined."
#endif

#else /* defined(EB_STATIC_CHECK) */

/* Define these keyword macros appropriately, to make them digestable for static checker tools. */
#define OS_TOOL_INLINE_KEYWORD __inline__
#define OS_TOOL_ASM_KEYWORD
#endif /* !defined(EB_STATIC_CHECK) */

#endif
/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
