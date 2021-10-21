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

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 10.3 (required)
 *  The value of an expression shall not be assigned to an object with a
 *  narrower essential type or of a different essential type category.
 *
 * Reason:
 *  The value is boolean. Hence, its value can only be TRUE or FALSE, which is
 *  1 or 0 respectively. Even in the narrowest type, 'signed char', which the
 *  C languages provides, can safely store these values.
 *
 * MISRAC2012-2) Deviated Rule: 20.7 (required)
 *  Expressions resulting from the expansion of macro parameters shall be
 *  enclosed in parentheses.
 *
 * Reason:
 *  False positive. The parameter 'successaddr' is enclosed in paraentheses
 *  in the macros ATOMICS_USER_[UN]LOCK_OBJECT.
 */

/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W588
 * W588: dead assignment to "success" eliminated
 *
 * Reason:
 * This compiler warning is caused by the symmetry between ATOMICS_USER_LOCK_OBJECT() and ATOMICS_USER_UNLOCK_OBJECT() macros.
 * The assignment to the output pointer "success" in the latter macro is currently not used because errors cannot be reported.
 * A custom implementation may use it, though.
 */

#include <Atomics.h>
#include <Atomics_user_functions.h>

#define ATOMICS_START_SEC_CODE
#include <Atomics_MemMap.h>

/* !LINKSTO Atomics.Fence, 2 */
FUNC(void, ATOMICS_CODE) Atomics_ThreadFence(void)
{
    ATOMICS_USER_THREAD_FENCE();
}

/* !LINKSTO Atomics.Init, 2 */
FUNC(void, ATOMICS_CODE) Atomics_Init(
    volatile P2VAR(Atomic_t, AUTOMATIC, ATOMICS_APPL_DATA) object,
    VAR(Atomic_value_t, AUTOMATIC) initValue)
{
    ATOMICS_USER_THREAD_FENCE();
    ATOMICS_USER_OBJECT_INIT(object, initValue);
    ATOMICS_USER_THREAD_FENCE();
}

/* !LINKSTO Atomics.Store, 1 */
FUNC(void, ATOMICS_CODE) Atomics_Store(
    volatile P2VAR(Atomic_t, AUTOMATIC, ATOMICS_APPL_DATA) object,
    VAR(Atomic_value_t, AUTOMATIC) newValue)
{
    VAR(boolean, AUTOMATIC) success;

    ATOMICS_USER_THREAD_FENCE();
    /* Deviation MISRAC2012-2 <1> */
    ATOMICS_USER_LOCK_OBJECT(object, &success);
    if (success)
    {
        ATOMICS_USER_SET_VALUE(object, newValue);
        /* Deviation MISRAC2012-2 <1>, TASKING-1 <1> */
        ATOMICS_USER_UNLOCK_OBJECT(object, &success);
        ATOMICS_USER_THREAD_FENCE();
    }
}

/* !LINKSTO Atomics.Load, 1 */
FUNC(Atomic_value_t, ATOMICS_CODE) Atomics_Load(
    volatile P2CONST(Atomic_t, AUTOMATIC, ATOMICS_APPL_DATA) object)
{
    VAR(boolean, AUTOMATIC) success;
    VAR(Atomic_value_t, AUTOMATIC) ret = 0u;

    ATOMICS_USER_THREAD_FENCE();
    /* Deviation MISRAC2012-2 <1> */
    ATOMICS_USER_LOCK_OBJECT(object, &success);
    if (success)
    {
        ret = ATOMICS_USER_GET_VALUE(object);
        /* Deviation MISRAC2012-2 <1>, TASKING-1 <1> */
        ATOMICS_USER_UNLOCK_OBJECT(object, &success);
        ATOMICS_USER_THREAD_FENCE();
    }

    return ret;
}

/* !LINKSTO Atomics.Exchange, 2 */
FUNC(Atomic_value_t, ATOMICS_CODE) Atomics_Exchange(
    volatile P2VAR(Atomic_t, AUTOMATIC, ATOMICS_APPL_DATA) object,
    VAR(Atomic_value_t, AUTOMATIC) newValue)
{
    VAR(boolean, AUTOMATIC) success;
    VAR(Atomic_value_t, AUTOMATIC) ret = 0u;

    ATOMICS_USER_THREAD_FENCE();
    /* Deviation MISRAC2012-2 <1> */
    ATOMICS_USER_LOCK_OBJECT(object, &success);
    if (success)
    {
        ret = ATOMICS_USER_GET_VALUE(object);
        ATOMICS_USER_SET_VALUE(object, newValue);
        /* Deviation MISRAC2012-2 <1>, TASKING-1 <1> */
        ATOMICS_USER_UNLOCK_OBJECT(object, &success);
        ATOMICS_USER_THREAD_FENCE();
    }

    return ret;
}

/* !LINKSTO Atomics.CompareExchange, 2 */
FUNC(boolean, ATOMICS_CODE) Atomics_CompareExchange(
    volatile P2VAR(Atomic_t, AUTOMATIC, ATOMICS_APPL_DATA) object,
    P2VAR(Atomic_t, AUTOMATIC, ATOMICS_APPL_DATA) expected,
    VAR(Atomic_value_t, AUTOMATIC) newValue)
{
    VAR(boolean, AUTOMATIC) success;
    VAR(boolean, AUTOMATIC) ret = FALSE;

    ATOMICS_USER_THREAD_FENCE();
    /* Deviation MISRAC2012-2 <1> */
    ATOMICS_USER_LOCK_OBJECT(object, &success);
    if (success)
    {
        Atomic_value_t const tmp = ATOMICS_USER_GET_VALUE(object);
        if (tmp == *expected)
        {
            ATOMICS_USER_SET_VALUE(object, newValue);
            ret = TRUE;
        }
        else
        {
            *expected = tmp;
        }
        /* Deviation MISRAC2012-2 <1>, TASKING-1 <1> */
        ATOMICS_USER_UNLOCK_OBJECT(object, &success);
        ATOMICS_USER_THREAD_FENCE();
    }

    return ret;
}

/* !LINKSTO Atomics.ArithmeticOps.Add, 1 */
FUNC(Atomic_value_t, ATOMICS_CODE) Atomics_FetchAdd(
    volatile P2VAR(Atomic_t, AUTOMATIC, ATOMICS_APPL_DATA) object,
    VAR(Atomic_value_t, AUTOMATIC) operand)
{
    VAR(boolean, AUTOMATIC) success;
    VAR(Atomic_value_t, AUTOMATIC) ret = 0u;

    ATOMICS_USER_THREAD_FENCE();
    /* Deviation MISRAC2012-2 <1> */
    ATOMICS_USER_LOCK_OBJECT(object, &success);
    if (success)
    {
        ret = ATOMICS_USER_GET_VALUE(object);
        ATOMICS_USER_SET_VALUE(object, ret + operand);
        /* Deviation MISRAC2012-2 <1>, TASKING-1 <1> */
        ATOMICS_USER_UNLOCK_OBJECT(object, &success);
        ATOMICS_USER_THREAD_FENCE();
    }

    return ret;
}

/* !LINKSTO Atomics.ArithmeticOps.Sub, 1 */
FUNC(Atomic_value_t, ATOMICS_CODE) Atomics_FetchSub(
    volatile P2VAR(Atomic_t, AUTOMATIC, ATOMICS_APPL_DATA) object,
    VAR(Atomic_value_t, AUTOMATIC) operand)
{
    VAR(boolean, AUTOMATIC) success;
    VAR(Atomic_value_t, AUTOMATIC) ret = 0u;

    ATOMICS_USER_THREAD_FENCE();
    /* Deviation MISRAC2012-2 <1> */
    ATOMICS_USER_LOCK_OBJECT(object, &success);
    if (success)
    {
        ret = ATOMICS_USER_GET_VALUE(object);
        ATOMICS_USER_SET_VALUE(object, ret - operand);
        /* Deviation MISRAC2012-2 <1>, TASKING-1 <1> */
        ATOMICS_USER_UNLOCK_OBJECT(object, &success);
        ATOMICS_USER_THREAD_FENCE();
    }

    return ret;
}

/* !LINKSTO Atomics.BoolOps.Or, 1 */
FUNC(Atomic_value_t, ATOMICS_CODE) Atomics_FetchOr(
    volatile P2VAR(Atomic_t, AUTOMATIC, ATOMICS_APPL_DATA) object,
    VAR(Atomic_value_t, AUTOMATIC) operand)
{
    VAR(boolean, AUTOMATIC) success;
    VAR(Atomic_value_t, AUTOMATIC) ret = 0u;

    ATOMICS_USER_THREAD_FENCE();
    /* Deviation MISRAC2012-2 <1> */
    ATOMICS_USER_LOCK_OBJECT(object, &success);
    if (success)
    {
        ret = ATOMICS_USER_GET_VALUE(object);
        ATOMICS_USER_SET_VALUE(object, ret | operand);
        /* Deviation MISRAC2012-2 <1>, TASKING-1 <1> */
        ATOMICS_USER_UNLOCK_OBJECT(object, &success);
        ATOMICS_USER_THREAD_FENCE();
    }

    return ret;
}

/* !LINKSTO Atomics.BoolOps.And, 1 */
FUNC(Atomic_value_t, ATOMICS_CODE) Atomics_FetchAnd(
    volatile P2VAR(Atomic_t, AUTOMATIC, ATOMICS_APPL_DATA) object,
    VAR(Atomic_value_t, AUTOMATIC) operand)
{
    VAR(boolean, AUTOMATIC) success;
    VAR(Atomic_value_t, AUTOMATIC) ret = 0u;

    ATOMICS_USER_THREAD_FENCE();
    /* Deviation MISRAC2012-2 <1> */
    ATOMICS_USER_LOCK_OBJECT(object, &success);
    if (success)
    {
        ret = ATOMICS_USER_GET_VALUE(object);
        ATOMICS_USER_SET_VALUE(object, ret & operand);
        /* Deviation MISRAC2012-2 <1>, TASKING-1 <1> */
        ATOMICS_USER_UNLOCK_OBJECT(object, &success);
        ATOMICS_USER_THREAD_FENCE();
    }

    return ret;
}

/* !LINKSTO Atomics.BoolOps.Xor, 1 */
FUNC(Atomic_value_t, ATOMICS_CODE) Atomics_FetchXor(
    volatile P2VAR(Atomic_t, AUTOMATIC, ATOMICS_APPL_DATA) object,
    VAR(Atomic_value_t, AUTOMATIC) operand)
{
    VAR(boolean, AUTOMATIC) success;
    VAR(Atomic_value_t, AUTOMATIC) ret = 0u;

    ATOMICS_USER_THREAD_FENCE();
    /* Deviation MISRAC2012-2 <1> */
    ATOMICS_USER_LOCK_OBJECT(object, &success);
    if (success)
    {
        ret = ATOMICS_USER_GET_VALUE(object);
        ATOMICS_USER_SET_VALUE(object, ret ^ operand);
        /* Deviation MISRAC2012-2 <1>, TASKING-1 <1> */
        ATOMICS_USER_UNLOCK_OBJECT(object, &success);
        ATOMICS_USER_THREAD_FENCE();
    }

    return ret;
}

/* !LINKSTO Atomics.FlagOps.TestAndSet, 1 */
FUNC(boolean, ATOMICS_CODE) Atomics_TestAndSetFlag(
    volatile P2VAR(Atomic_t, AUTOMATIC, ATOMICS_APPL_DATA) object,
    VAR(Atomic_value_t, AUTOMATIC) flagSelectionMask)
{
    VAR(boolean, AUTOMATIC) success;
    VAR(Atomic_value_t, AUTOMATIC) ret = 0u;

    ATOMICS_USER_THREAD_FENCE();
    /* Deviation MISRAC2012-2 <1> */
    ATOMICS_USER_LOCK_OBJECT(object, &success);
    if (success)
    {
        ret = ATOMICS_USER_GET_VALUE(object);
        ATOMICS_USER_SET_VALUE(object, ret | flagSelectionMask);
        /* Deviation MISRAC2012-2 <1>, TASKING-1 <1> */
        ATOMICS_USER_UNLOCK_OBJECT(object, &success);
        ATOMICS_USER_THREAD_FENCE();
    }

    /* Deviation MISRAC2012-1 <1> */
    return (boolean)((ret & flagSelectionMask) != FALSE);
}

/* !LINKSTO Atomics.FlagOps.Clear, 1 */
FUNC(void, ATOMICS_CODE) Atomics_ClearFlag(
    volatile P2VAR(Atomic_t, AUTOMATIC, ATOMICS_APPL_DATA) object,
    VAR(Atomic_value_t, AUTOMATIC) flagSelectionMask)
{
    VAR(boolean, AUTOMATIC) success;

    ATOMICS_USER_THREAD_FENCE();
    /* Deviation MISRAC2012-2 <1> */
    ATOMICS_USER_LOCK_OBJECT(object, &success);
    if (success)
    {
        Atomic_value_t const flags = ATOMICS_USER_GET_VALUE(object);
        ATOMICS_USER_SET_VALUE(object, flags & ~flagSelectionMask);
        /* Deviation MISRAC2012-2 <1>, TASKING-1 <1> */
        ATOMICS_USER_UNLOCK_OBJECT(object, &success);
        ATOMICS_USER_THREAD_FENCE();
    }
}

#define ATOMICS_STOP_SEC_CODE
#include <Atomics_MemMap.h>
