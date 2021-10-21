/**
 * \file
 *
 * \brief AUTOSAR Atomics
 *
 * This file contains the implementation of the AUTOSAR
 * module Atomics.
 *
 * \version 1.0.15
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2017 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef ATOMICS_OS_COMPATIBILITY_H
#define ATOMICS_OS_COMPATIBILITY_H

#include <Atomics_os_version_compare.h>
#include <Os_atomics.h>
#include <Os_defs.h>

/* Additional consistency check: this file is for OS versions between 6.0.97 and
 * 6.0.117 (inclusive) only.
 */
#if ATOMICS_OS_IS_OLDER_THAN(6,0,97) || ATOMICS_OS_IS_YOUNGER_THAN(6,0,117)
#error "This file must not be used with EB tresos AutoCore OS versions older than 6.0.97 or younger than 6.0.117."
#endif

/* Atomic objects have the type 'os_atomic_t' in EB tresos AutoCore OS.
 * This type is translated into 'Atomic_t'.
 */
typedef os_atomic_t Atomic_t;

/* EB tresos AutoCore OS versions before 6.0.118 don't define the type os_atomic_value_t.
 * Hence, in these versions there is no type to define the type Atomic_value_t.
 * For compatibility reasons, this type and its accompanying macro ATOMICS_VALUE_MAX
 * are defined here. Because we know, that in these OS versions os_atomic_value_t
 * has the same underlying type as os_atomic_t, we can define Atomic_value_t to be
 * equal to Atomic_t.
 */
typedef os_atomic_t Atomic_value_t;

#if (OS_ARCH == OS_ARM64)
#define ATOMICS_VALUE_MAX 18446744073709551615UL
#elif (OS_ARCH == OS_ARM) || \
      (OS_ARCH == OS_CORTEXM) || \
      (OS_ARCH == OS_LINUX) || \
      (OS_ARCH == OS_NEWARCH) || \
      (OS_ARCH == OS_PA) || \
      (OS_ARCH == OS_PIKEOS) || \
      (OS_ARCH == OS_RH850) || \
      (OS_ARCH == OS_TRICORE) || \
      (OS_ARCH == OS_WINDOWS)
#define ATOMICS_VALUE_MAX 4294967295U
#else
#error "EB tresos AutoCore OS doesn't support the selected CPU family."
#endif

#define ATOMICS_OBJECT_INITIALIZER(initValue) (initValue)

#define Atomics_ThreadFence() OS_AtomicThreadFence()
#define Atomics_Init(o, v) \
    do {                   \
        *(o) = (v);        \
    } while (0)
#define Atomics_Store(o, n) OS_AtomicStore(o, n)
#define Atomics_Load(o) OS_AtomicLoad(o)
#define Atomics_Exchange(o, n) OS_AtomicExchange(o, n)
#define Atomics_CompareExchange(o, e, n) OS_AtomicCompareExchange(o, e, n)

/* The macros Atomics_Fetch<OP>() must be mapped as follows:
 * - for versions 6.0.97 to 6.0.116 (inclusive) they must be mapped to OS_Atomic<OP>()
 * - for version 6.0.117 they must be mapped to OS_AtomicFetch<OP>()
 */
#if ATOMICS_OS_IS_OLDER_THAN(6,0,117)
#define Atomics_FetchAdd(o, op) OS_AtomicAdd(o, op)
#define Atomics_FetchSub(o, op) OS_AtomicSub(o, op)
#define Atomics_FetchOr(o, op) OS_AtomicOr(o, op)
#define Atomics_FetchAnd(o, op) OS_AtomicAnd(o, op)
#define Atomics_FetchXor(o, op) OS_AtomicXor(o, op)

#else /* !ATOMICS_OS_IS_OLDER_THAN(6,0,117) */

#define Atomics_FetchAdd(o, op) OS_AtomicFetchAdd(o, op)
#define Atomics_FetchSub(o, op) OS_AtomicFetchSub(o, op)
#define Atomics_FetchOr(o, op) OS_AtomicFetchOr(o, op)
#define Atomics_FetchAnd(o, op) OS_AtomicFetchAnd(o, op)
#define Atomics_FetchXor(o, op) OS_AtomicFetchXor(o, op)
#endif /* ATOMICS_OS_IS_OLDER_THAN(6,0,117) */

#define Atomics_TestAndSetFlag(o, f) OS_AtomicTestAndSetFlag(o, f)
#define Atomics_ClearFlag(o, f) OS_AtomicClearFlag(o, f)

#endif /* ATOMICS_OS_COMPATIBILITY_H */
