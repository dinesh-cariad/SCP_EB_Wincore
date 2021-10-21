/* Os_atomics_tsplatforms_generic_bits.h
 *
 * This file contains the generic implementation of the compatibility functions,
 * which were once provided by asc_Platforms for atomic bit-manipulation.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_atomics_tsplatforms_generic_bits.h 30151 2019-04-11 13:05:07Z mist9353 $
*/

/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: CastDiscardsQualPtrTgtType
 *   Cast discards qualifier from pointer target type
 *
 * Reason: The casted value is used for address calculations only and the code
 *   is well-aware of the volatile character of the destination (that's what
 *   cache mainenance functions like this are actually about).
 */

#ifndef OS_ATOMICS_TSPLATFORMS_GENERIC_BITS_H
#define OS_ATOMICS_TSPLATFORMS_GENERIC_BITS_H

#include <Os_tool.h>
#include <Os_atomics.h>
#include <Os_panic.h>

/* Introduce declarations of Enable/DisableAllInterrupts()
 * depending on kernel type.
 */
#if OS_KERNEL_TYPE == OS_MICROKERNEL
#include <public/Mk_autosar.h>
#else
#include <Os_api_cfg.h>
#endif


#if OS_ARCH_HAS_64BIT
#define OS_ATOMICS_TSP_ADDR_MASK	0x7uL
#define OS_ATOMICS_TSP_BE_NORM_8	7u
#define OS_ATOMICS_TSP_BE_NORM_16	6u
#define OS_ATOMICS_TSP_BE_NORM_32	4u
#define OS_ATOMICS_TSP_BE_NORM_64	0u
#else
#define OS_ATOMICS_TSP_ADDR_MASK	0x3u
#define OS_ATOMICS_TSP_BE_NORM_8	3u
#define OS_ATOMICS_TSP_BE_NORM_16	2u
#define OS_ATOMICS_TSP_BE_NORM_32	0u
#endif

/* Only 32-bit and 64-bit architectures are supported by this generic implementation.
 * This is because of the way the addresses of the 'containing objects' are calculated.
 * For example, if a pointer to an 8-bit object is given, the address of the containing
 * 32-bit object is calculated. This resulting pointer is then passed to
 * 'OS_AtomicTestAndSetFlag()' or 'OS_AtomicClearFlag()' respectively.
 */
/* CHECK: NOPARSE */
OS_STATIC_ASSERT(sizeof(os_atomic_t) == (OS_ATOMICS_TSP_ADDR_MASK + 1), OS_atomic_t_size_check);
/* CHECK: PARSE */

#define OS_ATOMICS_TSP_BITS_PER_BYTE 8u

/* Convert the given address into an address of an os_atomic_t object.
 * The returned address is properly aligned according to the size of
 * atomic objects, which depends on OS_ARCH_HAS_64BIT.
 */
#define OS_ATOMICS_TSP_GET_ADDR(addr) \
	((os_atomic_t volatile *)((os_size_t)(addr) & ~OS_ATOMICS_TSP_ADDR_MASK))

/* Get the index of the bit in the os_atomic_t object, which is defined by
 * 'addr' and 'bitIdx'. Note, that 'addr' points to a byte, short, or int
 * (all unsigned) inside an os_atomic_t object. The argument 'bitIdx' identifies
 * a bit in one of these subobjects. The returned index identifies this bit
 * relative to the os_atomic_t object, which contains the object pointed to
 * by 'addr'.
 * The argument 'be_norm' is only relevant for bit endian architectures.
 * It is used to calculate how many bytes the accessed object is offset from
 * the beginning of its containing object. Please note that in this context,
 * the accessed object is the one of which the addressed is passed to one of the
 * 'OS_AtomicTSSetBit_*()' functions. This object is contained in a 32-, or
 * 64-bit atomic object (with type os_atomic_t), depending on the architecture.
 */
#if OS_ENDIAN == OS_LITTLEENDIAN
#define OS_ATOMICS_TSP_GET_BITIDX(addr, bitIdx, be_norm) \
	((((os_size_t)(addr) & OS_ATOMICS_TSP_ADDR_MASK) * OS_ATOMICS_TSP_BITS_PER_BYTE) + (bitIdx) )

#else /* OS_ENDIAN == OS_BIGENDIAN */

#define OS_ATOMICS_TSP_GET_BITIDX(addr, bitIdx, be_norm) \
	((((be_norm) - ((os_size_t)(addr) & OS_ATOMICS_TSP_ADDR_MASK)) * OS_ATOMICS_TSP_BITS_PER_BYTE) + (bitIdx))
#endif

/* Returns a flag selection mask derived from 'addr' and 'bitIdx' so that the
 * bit identified by 'bitIdx' in the subobject pointed to by 'addr' is selected
 * in the containing os_atomic_t object.
 */
#define OS_ATOMICS_TSP_GET_MASK(addr, bitIdx, be_norm) \
	(os_atomic_value_t)(1uL << OS_ATOMICS_TSP_GET_BITIDX((addr), (bitIdx), (be_norm)))

/* The generic implementation of the atomic bit-set operation. */
#define OS_ATOMICS_TSP_GENERIC_SET_IMPL(addr, bitIdx, be_norm)												\
	do {																									\
		os_atomic_t volatile * const obj = OS_ATOMICS_TSP_GET_ADDR((addr));									\
		os_atomic_value_t const flagSelectionMask = OS_ATOMICS_TSP_GET_MASK((addr), (bitIdx), (be_norm));	\
																											\
		(void)OS_AtomicTestAndSetFlag(obj, flagSelectionMask);												\
	} while (0)

/* The generic implementation of the atomic bit-clear operation. */
#define OS_ATOMICS_TSP_GENERIC_CLEAR_IMPL(addr, bitIdx, be_norm)											\
	do {																									\
		os_atomic_t volatile * const obj = OS_ATOMICS_TSP_GET_ADDR((addr));									\
		os_atomic_value_t const flagSelectionMask = OS_ATOMICS_TSP_GET_MASK((addr), (bitIdx), (be_norm));	\
																											\
		OS_AtomicClearFlag(obj, flagSelectionMask);															\
	} while (0)

/* !LINKSTO Kernel.Atomics.TSFunctionsBitOps, 1
*/

/* Note, the function documentation is in Os_atomics_tsplatforms.h. */

static OS_TOOL_INLINE_KEYWORD void OS_AtomicTSSetBit_8(
	os_uint8_t volatile *addr,
	os_uint32_t bitIdx)
{
	/* Possible diagnostic TOOLDIAG-1 */
	OS_ATOMICS_TSP_GENERIC_SET_IMPL(addr, bitIdx, OS_ATOMICS_TSP_BE_NORM_8);
}

static OS_TOOL_INLINE_KEYWORD void OS_AtomicTSClearBit_8(
	os_uint8_t volatile *addr,
	os_uint32_t bitIdx)
{
	/* Possible diagnostic TOOLDIAG-1 */
	OS_ATOMICS_TSP_GENERIC_CLEAR_IMPL(addr, bitIdx, OS_ATOMICS_TSP_BE_NORM_8);
}

static OS_TOOL_INLINE_KEYWORD void OS_AtomicTSSetBit_16(
	os_uint16_t volatile *addr,
	os_uint32_t bitIdx)
{
	/* Possible diagnostic TOOLDIAG-1 */
	OS_ATOMICS_TSP_GENERIC_SET_IMPL(addr, bitIdx, OS_ATOMICS_TSP_BE_NORM_16);
}

static OS_TOOL_INLINE_KEYWORD void OS_AtomicTSClearBit_16(
	os_uint16_t volatile *addr,
	os_uint32_t bitIdx)
{
	/* Possible diagnostic TOOLDIAG-1 */
	OS_ATOMICS_TSP_GENERIC_CLEAR_IMPL(addr, bitIdx, OS_ATOMICS_TSP_BE_NORM_16);
}

static OS_TOOL_INLINE_KEYWORD void OS_AtomicTSSetBit_32(
	os_uint32_t volatile *addr,
	os_uint32_t bitIdx)
{
	/* Possible diagnostic TOOLDIAG-1 */
	OS_ATOMICS_TSP_GENERIC_SET_IMPL(addr, bitIdx, OS_ATOMICS_TSP_BE_NORM_32);
}

static OS_TOOL_INLINE_KEYWORD void OS_AtomicTSClearBit_32(
	os_uint32_t volatile *addr,
	os_uint32_t bitIdx)
{
	/* Possible diagnostic TOOLDIAG-1 */
	OS_ATOMICS_TSP_GENERIC_CLEAR_IMPL(addr, bitIdx, OS_ATOMICS_TSP_BE_NORM_32);
}

#if OS_ARCH_HAS_64BIT
static OS_TOOL_INLINE_KEYWORD void OS_AtomicTSSetBit_64(
	os_uint64_t volatile *addr,
	os_uint32_t bitIdx)
{
	/* Possible diagnostic TOOLDIAG-1 */
	OS_ATOMICS_TSP_GENERIC_SET_IMPL(addr, bitIdx, OS_ATOMICS_TSP_BE_NORM_64);
}

static OS_TOOL_INLINE_KEYWORD void OS_AtomicTSClearBit_64(
	os_uint64_t volatile *addr,
	os_uint32_t bitIdx)
{
	/* Possible diagnostic TOOLDIAG-1 */
	OS_ATOMICS_TSP_GENERIC_CLEAR_IMPL(addr, bitIdx, OS_ATOMICS_TSP_BE_NORM_64);
}
#endif /* OS_ARCH_HAS_64BIT */


#endif /* OS_ATOMICS_TSPLATFORMS_GENERIC_BITS_H */
