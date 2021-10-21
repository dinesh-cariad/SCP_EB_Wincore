/* Os_atomics_WINLIN_gnu.h
 *
 * This file contains the implementation of tha atomic functions
 * for WINLIN and the GCC toolchain.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_atomics_WINLIN_gnu.h 28112 2018-01-10 08:24:28Z olme8414 $
*/
#ifndef OS_ATOMICS_WINLIN_GNU_H
#define OS_ATOMICS_WINLIN_GNU_H

#include <Os_types.h>


/* Type definition of os_atomic_t and os_atomic_value_t.
 * All atomic functions operate on objects of these types.
 * The type os_atomic_t is aligned to minimize penalties on locked instructions.
 */
#if OS_ARCH_HAS_64BIT
#define OS_ATOMICS_ASM_SIZE_SUFFIX "q"
typedef os_uint64_t os_atomic_t OS_ALIGNED_ATTRIB_POST(8);
typedef os_uint64_t os_atomic_value_t;
#define OS_ATOMICS_VALUE_MAX 18446744073709551615UL
#else
#define OS_ATOMICS_ASM_SIZE_SUFFIX "l"
typedef os_uint32_t os_atomic_t OS_ALIGNED_ATTRIB_POST(4);
typedef os_uint32_t os_atomic_value_t;
#define OS_ATOMICS_VALUE_MAX 4294967295U
#endif

/* See Os_atomics.h for API comments. */

/* !LINKSTO Kernel.Atomics.Fence, 1
 */ 
static OS_TOOL_INLINE_KEYWORD void OS_AtomicThreadFence(void)
{
	/* This implementation could use the mfence instruction as well.
	 * The use of a locked instruction is preferred, because it has
	 * a smaller latency and isn't known to cause pipeline issues.
	 * Unfortunately, further details concerning the latter are not
	 * readily available.
	 * 
	 * Furthermore, according to section "8.2.3.9 Loads and Stores Are Not
	 * Reordered with Locked Instructions" in volume 3 in
	 * "Intel 64 and IA-32 Architectures Software Developer’s Manual"
	 * (combined volumes 1, 2A, 2B, 2C, 2D, 3A, 3B, 3C and 3D, published december
	 * 2016, order no. 325462-061US) by Intel Corp., it is guaranteed, that
	 * stores and loads are not reordered with locked instructions.
	 * 
	 * The clobber list in the following inline-assembler statement contains
	 * 'memory' to preclude compiler optimizations, which otherwise might move
	 * read/write operations across this OS_TOOL_ASM_KEYWORD statement.
	 */
	OS_TOOL_ASM_KEYWORD volatile (
		"lock	addl $0, (%%esp)"
		: /* no outputs */
		: /* no inputs */
		: "memory"
	);
}

/* !LINKSTO Kernel.Atomics.Init, 1
 */
static OS_TOOL_INLINE_KEYWORD void OS_AtomicInit(
	os_atomic_t volatile *object,
	os_atomic_value_t initValue)
{
	*object = initValue;
}

/* !LINKSTO Kernel.Atomics.LoadStore, 1
 */
static OS_TOOL_INLINE_KEYWORD void OS_AtomicStore(
	os_atomic_t volatile *object,
	os_atomic_value_t newValue)
{
	OS_AtomicThreadFence();
	*object = newValue;
	OS_AtomicThreadFence();
}

/* !LINKSTO Kernel.Atomics.LoadStore, 1
 */
static OS_TOOL_INLINE_KEYWORD os_atomic_value_t OS_AtomicLoad(
	os_atomic_t const volatile *object)
{
	os_atomic_value_t value;

	OS_AtomicThreadFence();
	value = *object;
	OS_AtomicThreadFence();

	return value;
}

/* !LINKSTO Kernel.Atomics.Exchange, 1
 */
static OS_TOOL_INLINE_KEYWORD os_atomic_value_t OS_AtomicExchange(
	os_atomic_t volatile *object,
	os_atomic_value_t newValue)
{
	register os_atomic_value_t old;
	/* The clobber list contains 'memory' to preclude compiler optimizations,
	 * which otherwise might move read/write operations across this
	 * OS_TOOL_ASM_KEYWORD statement.
	 * Sequential consistency is achieved by the locked instruction (see also
	 * OS_AtomicThreadFence()).
	 */
	OS_TOOL_ASM_KEYWORD volatile(
		"lock	xchg" OS_ATOMICS_ASM_SIZE_SUFFIX " %[newValue], %[object]"
		: [object] "+m" (*object), [old] "=q" (old)
		: [newValue] "[old]" (newValue)
		: "memory"
	);
	return old;
}

/* !LINKSTO Kernel.Atomics.CompareExchange, 1
 */
static OS_TOOL_INLINE_KEYWORD os_boolean_t OS_AtomicCompareExchange(
	os_atomic_t volatile *object,
	os_atomic_value_t *expected,
	os_atomic_value_t newValue)
{
	os_boolean_t comparisonSucceeded = OS_FALSE;
	register os_atomic_value_t old;
	/* The clobber list contains 'memory' to preclude compiler optimizations,
	 * which otherwise might move read/write operations across this
	 * OS_TOOL_ASM_KEYWORD statement.
	 * Sequential consistency is achieved by the locked instruction (see also
	 * OS_AtomicThreadFence()).
	 */
	OS_TOOL_ASM_KEYWORD volatile(
		"lock	cmpxchg" OS_ATOMICS_ASM_SIZE_SUFFIX " %[newValue], %[object]"
		: [object] "+m" (*object), [old] "=a" (old)
		: [newValue] "q" (newValue), [expected] "[old]" (*expected)
		: "memory", "cc"
	);

	if (old == *expected)
	{
		comparisonSucceeded = OS_TRUE;
	}
	else
	{
		*expected = old;
	}

	return comparisonSucceeded; 
}

/* !LINKSTO Kernel.Atomics.ArithmeticOps, 1
 */
static OS_TOOL_INLINE_KEYWORD os_atomic_value_t OS_AtomicFetchAdd(
	os_atomic_t volatile *object,
	os_atomic_value_t operand)
{
	register os_atomic_value_t old;
	/* The clobber list contains 'memory' to preclude compiler optimizations,
	 * which otherwise might move read/write operations across this
	 * OS_TOOL_ASM_KEYWORD statement.
	 * Sequential consistency is achieved by the locked instruction (see also
	 * OS_AtomicThreadFence()).
	 */
	OS_TOOL_ASM_KEYWORD volatile(
		"lock	xadd" OS_ATOMICS_ASM_SIZE_SUFFIX " %[operand], %[object]"
		: [object] "+m" (*object), [old] "=q" (old)
		: [operand] "[old]" (operand)
		: "memory", "cc"
	);
	return old;
}

/* !LINKSTO Kernel.Atomics.ArithmeticOps, 1
 */
static OS_TOOL_INLINE_KEYWORD os_atomic_value_t OS_AtomicFetchSub(
	os_atomic_t volatile *object,
	os_atomic_value_t operand)
{
	os_atomic_value_t const negOperand = (os_atomic_value_t) (-1 * operand);
	return OS_AtomicFetchAdd(object, negOperand);
}

/* !LINKSTO Kernel.Atomics.BoolOps, 1
 */
static OS_TOOL_INLINE_KEYWORD os_atomic_value_t OS_AtomicFetchOr(
	os_atomic_t volatile *object,
	os_atomic_value_t operand)
{
	os_atomic_value_t expected;
	os_atomic_value_t newValue;

	do
	{
		/* There is no need to use OS_AtomicLoad(object) here, because
		 * the processor guarantees atomicity and OS_AtomicCompareExchange()
		 * takes care of sequential consistency.
		 */
		expected = *object;
		newValue = expected | operand;
	} while (!OS_AtomicCompareExchange(object, &expected, newValue));

	return expected;
}

/* !LINKSTO Kernel.Atomics.BoolOps, 1
 */
static OS_TOOL_INLINE_KEYWORD os_atomic_value_t OS_AtomicFetchAnd(
	os_atomic_t volatile *object,
	os_atomic_value_t operand)
{
	os_atomic_value_t expected;
	os_atomic_value_t newValue;

	do
	{
		/* There is no need to use OS_AtomicLoad(object) here, because
		 * the processor guarantees atomicity and OS_AtomicCompareExchange()
		 * takes care of sequential consistency.
		 */
		expected = *object;
		newValue = expected & operand;
	} while (!OS_AtomicCompareExchange(object, &expected, newValue));

	return expected;
}

/* !LINKSTO Kernel.Atomics.BoolOps, 1
 */
static OS_TOOL_INLINE_KEYWORD os_atomic_value_t OS_AtomicFetchXor(
	os_atomic_t volatile *object,
	os_atomic_value_t operand)
{
	os_atomic_value_t expected;
	os_atomic_value_t newValue;

	do
	{
		/* There is no need to use OS_AtomicLoad(object) here, because
		 * the processor guarantees atomicity and OS_AtomicCompareExchange()
		 * takes care of sequential consistency.
		 */
		expected = *object;
		newValue = expected ^ operand;
	} while (!OS_AtomicCompareExchange(object, &expected, newValue));

	return expected;
}

/* !LINKSTO Kernel.Atomics.FlagOps, 1
 */
static OS_TOOL_INLINE_KEYWORD os_boolean_t OS_AtomicTestAndSetFlag(
	os_atomic_t volatile *object,
	os_atomic_value_t flagSelectionMask)
{
	os_atomic_value_t const old = OS_AtomicFetchOr(object, flagSelectionMask);
	return (old & flagSelectionMask) != 0;
}

/* !LINKSTO Kernel.Atomics.FlagOps, 1
 */
static OS_TOOL_INLINE_KEYWORD void OS_AtomicClearFlag(
	os_atomic_t volatile *object,
	os_atomic_value_t flagSelectionMask)
{
	(void)OS_AtomicFetchAnd(object, (os_atomic_value_t)~flagSelectionMask);
}

#endif /* OS_ATOMICS_WINLIN_GNU_H */

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/

