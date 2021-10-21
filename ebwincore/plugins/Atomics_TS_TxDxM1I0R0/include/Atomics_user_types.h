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
#ifndef ATOMICS_USER_TYPES_H
#define ATOMICS_USER_TYPES_H

#include <Std_Types.h>
#include <Atomics_platform_types.h>
#include <Atomics_user_types_customizations.h>

/** \def ATOMICS_VALUE_MAX
 ** \brief The maximum value that an object of type Atomic_value_t can store.
 */
#ifndef ATOMICS_VALUE_MAX
#if CPU_TYPE == CPU_TYPE_8
#define ATOMICS_VALUE_MAX 255U
#elif CPU_TYPE == CPU_TYPE_16
#define ATOMICS_VALUE_MAX 65535U
#elif CPU_TYPE == CPU_TYPE_32
#define ATOMICS_VALUE_MAX 4294967295U
#elif CPU_TYPE == CPU_TYPE_64
#define ATOMICS_VALUE_MAX 18446744073709551615UL
#else
#error "Unsupported CPU type."
#endif
#endif

/** \def ATOMICS_USER_ATOMIC_T
 ** \brief The type of atomic objects.
 **
 ** This macro defines the type of atomic objects.
 ** It is used in the typedef declaration of the type Atomic_t.
 **
 ** The generic implementation assumes, that it's an unsigned integer,
 ** which has the native size of the architecture (e.g., 32 bits
 ** on 32-bit, or 64 bits on 64-bit CPUs) and is naturally aligned.
 **
 ** If you change this macro, please ensure that it's consistent with
 ** all ATOMICS_USER_*() macros.
 */
#ifndef ATOMICS_USER_ATOMIC_T
#if CPU_TYPE == CPU_TYPE_8
#define ATOMICS_USER_ATOMIC_T uint8
#elif CPU_TYPE == CPU_TYPE_16
#define ATOMICS_USER_ATOMIC_T uint16
#elif CPU_TYPE == CPU_TYPE_32
#define ATOMICS_USER_ATOMIC_T uint32
#elif CPU_TYPE == CPU_TYPE_64
#define ATOMICS_USER_ATOMIC_T uint64
#else
#error "Unsupported CPU type."
#endif
#endif /* ATOMICS_USER_ATOMIC_T */

/** \def ATOMICS_USER_ATOMIC_VALUE_T
 ** \brief The value type of atomic objects.
 **
 ** This macro defines the value type of atomic objects. All atomic objects
 ** internally store objects of this type.
 ** It is used in the typedef declaration of the type Atomic_value_t.
 **
 ** The generic implementation assumes, that it's an unsigned integer, which
 ** has the native size of the architecture (e.g., 32 bits on 32-bit, or
 ** 64 bits on 64-bit CPUs).
 **
 ** If you change this macro, please ensure that it's consistent with
 ** all ATOMICS_USER_*() macros.
 */
#ifndef ATOMICS_USER_ATOMIC_VALUE_T
#if CPU_TYPE == CPU_TYPE_8
#define ATOMICS_USER_ATOMIC_VALUE_T uint8
#elif CPU_TYPE == CPU_TYPE_16
#define ATOMICS_USER_ATOMIC_VALUE_T uint16
#elif CPU_TYPE == CPU_TYPE_32
#define ATOMICS_USER_ATOMIC_VALUE_T uint32
#elif CPU_TYPE == CPU_TYPE_64
#define ATOMICS_USER_ATOMIC_VALUE_T uint64
#else
#error "Unsupported CPU type."
#endif
#endif /* ATOMICS_USER_ATOMIC_VALUE_T */

/** \brief The type of atomic objects.
 **
 ** All atomic objects are of this type. The underlying type is
 ** defined by the macro ATOMICS_USER_ATOMIC_T. Multiple threads of execution
 ** may work on objects of this type concurrently. Objects of this type are
 ** opaque and you must access them only via the Atomics_*() functions. This
 ** guarantees atomicity and sequential consistency.
 */
typedef ATOMICS_USER_ATOMIC_T Atomic_t;

/** \brief The value type of atomic objects.
 **
 ** All atomic objects store a value of this type. Its underlying type is
 ** defined by the macro ATOMICS_USER_ATOMIC_VALUE_T. Objects of this type
 ** must only be used locally, because they are not designed for concurrent
 ** access: there are no atomicity or sequential consistency guarantees.
 ** You can use them directly, as unsigned integers, in expressions.
 */
typedef ATOMICS_USER_ATOMIC_VALUE_T Atomic_value_t;

#endif /* ATOMICS_USER_TYPES_H */
