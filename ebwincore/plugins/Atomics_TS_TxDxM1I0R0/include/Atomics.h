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
#ifndef ATOMICS_H
#define ATOMICS_H

#ifndef ATOMICS_USE_GENERIC_IMPL
/* For further information please refer to the AutoCore Generic Base documentation. */
#error "Please define ATOMICS_USE_GENERIC_IMPL to choose between generic or specialized implementation."
#endif

#if ATOMICS_USE_GENERIC_IMPL == 0
#include <Atomics_eb_vendor_check.h>
#include <Atomics_os_version_compare.h>

/* EB tresos AutoCore OS compatibility */
#if ATOMICS_OS_IS_OLDER_THAN(6,0,97)
#error "Versions of EB tresos AutoCore OS before 6.0.97 do not support atomic functions."

#elif ATOMICS_OS_IS_OLDER_THAN(6,0,118)
#include <Atomics_os_compatibility.h>

#else /* no compatibility header file requried */

#include <Os_atomics.h>

/* The maximum value, that an object of type Atomic_value_t can store. */
#define ATOMICS_VALUE_MAX OS_ATOMICS_VALUE_MAX

/* Atomic objects have the type 'os_atomic_t' in EB tresos AutoCore OS.
 * This type is translated into 'Atomic_t'.
 */
typedef os_atomic_t Atomic_t;

/* Values of atomic object have the type 'os_atomic_value_t' in EB tresos
 * AutoCore OS. This type is translated into 'Atomic_value_t'.
 */
typedef os_atomic_value_t Atomic_value_t;

/* EB tresos AutoCore OS uses different prefixes: 'OS_Atomic', 'OS_ATOMIC'.
 */
#define ATOMICS_OBJECT_INITIALIZER(initValue) OS_ATOMIC_OBJECT_INITIALIZER(initValue)
#define Atomics_ThreadFence() OS_AtomicThreadFence()
#define Atomics_Init(o, v) OS_AtomicInit(o, v)
#define Atomics_Store(o, n) OS_AtomicStore(o,n)
#define Atomics_Load(o) OS_AtomicLoad(o)
#define Atomics_Exchange(o, n) OS_AtomicExchange(o, n)
#define Atomics_CompareExchange(o, e, n) OS_AtomicCompareExchange(o, e, n)
#define Atomics_FetchAdd(o, op) OS_AtomicFetchAdd(o, op)
#define Atomics_FetchSub(o, op) OS_AtomicFetchSub(o, op)
#define Atomics_FetchOr(o, op) OS_AtomicFetchOr(o, op)
#define Atomics_FetchAnd(o, op) OS_AtomicFetchAnd(o, op)
#define Atomics_FetchXor(o, op) OS_AtomicFetchXor(o, op)
#define Atomics_TestAndSetFlag(o, f) OS_AtomicTestAndSetFlag(o, f)
#define Atomics_ClearFlag(o, f) OS_AtomicClearFlag(o, f)
#endif /* EB tresos AutoCore OS compatibility */

#else /* ATOMICS_USE_GENERIC_IMPL != 0 */

#include <Std_Types.h>
#include <Atomics_user_types.h>
#include <Atomics_user_functions.h>

/** \def ATOMICS_OBJECT_INITIALIZER
 ** \brief Expands into an initializer for an atomic object.
 **
 ** This macro must be used to initialize an atomic object of type Atomic_t
 ** at compile-time. You can assign its expansion directly to an object of
 ** type Atomic_t.
 **
 ** Atomic objects with static storage duration are initialized with the
 ** initial value zero at program load time. In this case, you don't have to
 ** use ATOMICS_OBJECT_INITIALIZER(0) explicitly.
 **
 ** See also Atomics_Init(), which does the same at runtime.
 **
 ** It is an error to use an atomic object without prior initialization.
 **
 ** The behavior is the same as what C11 specifies for the following:
 **    ATOMIC_VAR_INIT(initValue)
 */
/* !LINKSTO Atomics.Init, 2 */
#define ATOMICS_OBJECT_INITIALIZER(initValue) ATOMICS_USER_OBJECT_INITIALIZER(initValue)

#define ATOMICS_START_SEC_CODE
#include <Atomics_MemMap.h>

/** \brief Creates a sequentially consistent acquire and release fence.
 **
 ** The behavior is the same as what C11 specifies for the following call:
 **   atomic_thread_fence(memory_order_seq_cst);
 **
 ** It may also serve as a compiler-barrier, which stops the compiler
 ** from moving instructions across it either way for optimization purposes.
 */
extern FUNC(void, ATOMICS_CODE) Atomics_ThreadFence(void);

/** \brief Initializes an atomic object.
 **
 ** Initializes an atomic object at runtime.
 ** You must initialize an atomic object before use. Undefined behavior is the
 ** consequence, if you fail to do so.
 **
 ** Atomic objects with static storage duration are initialized with the initial
 ** value zero at program load time. You don't have to use Atomics_Init(object, 0)
 ** explicitly in this case.
 **
 ** See also ATOMICS_OBJECT_INITIALIZER().
 **
 ** The behavior is the same as what C11 specifies for the following call:
 **   atomic_init(object, initValue);
 **
 ** \param[in,out] object The pointer to an atomic object to initialize.
 ** \param[in] initValue The initial value.
 */
extern FUNC(void, ATOMICS_CODE) Atomics_Init(
    volatile P2VAR(Atomic_t, AUTOMATIC, ATOMICS_APPL_DATA) object,
    VAR(Atomic_value_t, AUTOMATIC) initValue);

/** \brief Stores into an atomic object.
 **
 ** The given 'newValue' is stored atomically into the memory location pointed
 ** to by 'object'. Sequential consistency is ensured.
 **
 ** The behavior is as defined by C11 for the following call:
 **   atomic_store(object, newValue);
 **
 ** \param[in,out] object The pointer to an atomic object to work with.
 ** \param[in] newValue   The new value to be stored into the atomic object.
 */
extern FUNC(void, ATOMICS_CODE) Atomics_Store(
    volatile P2VAR(Atomic_t, AUTOMATIC, ATOMICS_APPL_DATA) object,
    VAR(Atomic_value_t, AUTOMATIC) newValue);

/** \brief Loads an atomic object.
 **
 ** Atomically loads the value in the memory location pointed to by 'object'
 ** and returns it. Sequential consistency is ensured.
 **
 ** The behavior is as defined by C11 for the following call:
 **   atomic_load(object);
 **
 ** \param[in] object The pointer to an atomic object of which to return its
 **                   value.
 ** \return The value stored in the atomic object.
 */
extern FUNC(Atomic_value_t, ATOMICS_CODE) Atomics_Load(
    volatile P2CONST(Atomic_t, AUTOMATIC, ATOMICS_APPL_DATA) object);

/** \brief Atomically exchanges the given values.
 **
 ** This function atomically exchanges the value pointed to by 'object'
 ** with the value 'newValue'. The value in memory at 'object' before this
 ** exchange is returned.
 **
 ** Sequential consistency is ensured.
 **
 ** The behavior is as specified in C11 when calling the following function:
 **    atomic_exchange(object, newValue);
 **
 ** \param[in,out] object The pointer to an atomic object to work on.
 ** \param[in] newValue The new value to be stored into the atomic object.
 ** \return The value stored in the atomic object before the exchange.
 */
extern FUNC(Atomic_value_t, ATOMICS_CODE) Atomics_Exchange(
    volatile P2VAR(Atomic_t, AUTOMATIC, ATOMICS_APPL_DATA) object,
    VAR(Atomic_value_t, AUTOMATIC) newValue);


/** \brief Atomically exchanges the given values if the comparison succeeds.
 **
 ** If the values at 'object' and 'expected' are equal, 'newValue' is written
 ** atomically into the the memory location pointed to by 'object' and TRUE is
 ** returned.
 **
 ** If the values at 'object' and 'expected' are not equal, the current value at
 ** 'object' is written into the the memory location pointed to by 'expected' and
 ** FALSE is returned.
 **
 ** Sequential consistency is ensured.
 **
 ** The behavior is as specified in C11 when calling the following function:
 **   atomic_compare_exchange_strong(object, expected, newValue);
 **
 ** \param[in,out] object The pointer to an atomic object to work on.
 ** \param[in,out] expected Points to the expected value of the atomic object for
 **                         the exchange to happen. The location pointed to is
 **                         updated with the current value of the atomic object,
 **                         if the comparison fails and hence, the exchange does
 **                         not happen.
 ** \param[in] newValue The new value to be stored into the atomic object.
 ** \return If the comparison succeeds and the atomic object is updated with
 **         'newValue', TRUE is returned. Otherwise, FALSE is returned and the atomic
 **         object is only read and put into the location pointed to by 'expected'.
 */
extern FUNC(boolean, ATOMICS_CODE) Atomics_CompareExchange(
    volatile P2VAR(Atomic_t, AUTOMATIC, ATOMICS_APPL_DATA) object,
    P2VAR(Atomic_t, AUTOMATIC, ATOMICS_APPL_DATA) expected,
    VAR(Atomic_value_t, AUTOMATIC) newValue);

/** \brief Atomically adds the given number.
 **
 ** This function atomically adds 'operand' to the value in memory at
 ** location 'object'. The value in this location before the operation
 ** is returned.
 **
 ** Sequential consistency is ensured.
 **
 ** The behavior is as specified in C11 when calling the function:
 **   atomic_fetch_add(object, operand);
 **
 ** \param[in,out] object The pointer to an atomic object to work on.
 ** \param[in] operand The number to add to the atomic object.
 ** \return The value of the atomic object before the operation.
 */
extern FUNC(Atomic_value_t, ATOMICS_CODE) Atomics_FetchAdd(
    volatile P2VAR(Atomic_t, AUTOMATIC, ATOMICS_APPL_DATA) object,
    VAR(Atomic_value_t, AUTOMATIC) operand);

/** \brief Atomically subtracts the given number.
 **
 ** This function atomically subtracts 'operand' from the value in memory at
 ** location 'object'. The value in this location before the operation
 ** is returned.
 **
 ** Sequential consistency is ensured.
 **
 ** The behavior is as specified in C11 when calling the function:
 **   atomic_fetch_sub(object, operand);
 **
 ** \param[in,out] object The pointer to an atomic object to work on.
 ** \param[in] operand The number to subtract from the atomic object.
 ** \return The value of the atomic object before the operation.
 */
extern FUNC(Atomic_value_t, ATOMICS_CODE) Atomics_FetchSub(
    volatile P2VAR(Atomic_t, AUTOMATIC, ATOMICS_APPL_DATA) object,
    VAR(Atomic_value_t, AUTOMATIC) operand);

/** \brief Atomically performs a boolean OR operation.
 **
 ** This function atomically performs a boolean OR operation of 'operand' and
 ** the value in memory pointed to by 'object' and stores the result into that
 ** location. It returns the value in that location before the operation.
 **
 ** Sequential consistency is ensured.
 **
 ** The behavior is as specified in C11 when calling the function:
 **   atomic_fetch_or(object, operand);
 **
 ** \param[in,out] object The pointer to an atomic object to work on.
 ** \param[in] operand The value to OR to the atomic object.
 ** \return The value of the atomic object before the operation.
 */
extern FUNC(Atomic_value_t, ATOMICS_CODE) Atomics_FetchOr(
    volatile P2VAR(Atomic_t, AUTOMATIC, ATOMICS_APPL_DATA) object,
    VAR(Atomic_value_t, AUTOMATIC) operand);

/** \brief Atomically performs a boolean AND operation.
 **
 ** This function atomically performs a boolean AND operation of 'operand' and
 ** the value in memory pointed to by 'object' and stores the result into that
 ** location. It returns the value in that location before the operation.
 **
 ** Sequential consistency is ensured.
 **
 ** The behavior is as specified in C11 when calling the function:
 **   atomic_fetch_and(object, operand);
 **
 ** \param[in,out] object The pointer to an atomic object to work on.
 ** \param[in] operand The value to AND to the atomic object.
 ** \return The value of the atomic object before the operation.
 */
extern FUNC(Atomic_value_t, ATOMICS_CODE) Atomics_FetchAnd(
    volatile P2VAR(Atomic_t, AUTOMATIC, ATOMICS_APPL_DATA) object,
    VAR(Atomic_value_t, AUTOMATIC) operand);

/** \brief Atomically performs a boolean XOR operation.
 **
 ** This function atomically performs a boolean XOR operation of 'operand' and
 ** the value in memory pointed to by 'object' and stores the result into that
 ** location. It returns the value in that location before the operation.
 **
 ** Sequential consistency is ensured.
 **
 ** The behavior is as specified in C11 when calling the function:
 **   atomic_fetch_xor(object, operand);
 **
 ** \param[in,out] object The pointer to an atomic object to work on.
 ** \param[in] operand The value to XOR to the atomic object.
 ** \return The value of the atomic object before the operation.
 */
extern FUNC(Atomic_value_t, ATOMICS_CODE) Atomics_FetchXor(
    volatile P2VAR(Atomic_t, AUTOMATIC, ATOMICS_APPL_DATA) object,
    VAR(Atomic_value_t, AUTOMATIC) operand);

/** \brief Atomically sets a selected bit.
 **
 ** Atomically sets the bit selected by 'flagSelectionMask' in the memory
 ** location pointed to by 'object'. It returns the state of this bit before
 ** the operation, i.e., TRUE, if it was already set and FALSE otherwise.
 ** Only one bit may be selected by 'flagSelectionMask'.
 **
 ** Sequential consistency is ensured.
 **
 ** The behavior is similar as specified in C11 when calling the function:
 **    atomic_flag_test_and_set(object);
 ** but more than just one flag are fitted into an os_atomic_t object.
 **
 ** \param[in,out] object The pointer to an atomic object to work on.
 ** \param[in] flagSelectionMask The bit mask to select one bit in the atomic
 **                              object.
 ** \return The state of the selected flag before the operation.
 */
extern FUNC(boolean, ATOMICS_CODE) Atomics_TestAndSetFlag(
    volatile P2VAR(Atomic_t, AUTOMATIC, ATOMICS_APPL_DATA) object,
    VAR(Atomic_value_t, AUTOMATIC) flagSelectionMask);

/** \brief Atomically clears a selected bit.
 **
 ** This function atomically clears the bit selected by 'flagSelectionMask'
 ** in the memory location pointed to by 'object'.
 **
 ** Sequential consistency is ensured.
 **
 ** The behavior is similar as specified in C11 when calling the function:
 **    atomic_flag_clear(object);
 ** but more than just one flag are fitted into an os_atomic_t object.
 **
 ** \param[in,out] object The pointer to an atomic object to work on.
 ** \param[in] flagSelectionMask The bit mask to select one bit in the atomic
 **                              object.
 */
extern FUNC(void, ATOMICS_CODE) Atomics_ClearFlag(
    volatile P2VAR(Atomic_t, AUTOMATIC, ATOMICS_APPL_DATA) object,
    VAR(Atomic_value_t, AUTOMATIC) flagSelectionMask);

#define ATOMICS_STOP_SEC_CODE
#include <Atomics_MemMap.h>

#endif /* ATOMICS_USE_GENERIC_IMPL == 0 */
#endif /* ATOMICS_H */
