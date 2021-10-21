/* Ioc_static_asserts.h - IOC static assert to check certain constraints at compile time.
 *
 * This header defines macros which implement C11's _Static_assert in C90.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_static_asserts.h 2497 2018-02-06 12:11:28Z olme8414 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 20.10 (advisory)
 *  The # and ## preprocessor operators should not be used.
 *
 * Reason:
 *  The identifiers of the type defined by a static assert must differ.
 *  Therefore, an identifier, which is unique for a certain static assert,
 *  is appended to the name of the assert. This requires the ## operator.
*/

#ifndef IOC_STATIC_ASSERTS_H
#define IOC_STATIC_ASSERTS_H

#include <public/Ioc_basic_types.h>

/* IOC_STATIC_ASSERT - resembles C11's _Static_assert
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
#define IOC_STATIC_ASSERT(cond, id) typedef ioc_uint8_t (id)[(cond)? 1 : -1]

/* IOC_DESIZEOF_ASSERT - checks at compile time, that the size of a data element can be held by an ioc_ilength_t.
 *
 * Every type must only be checked once per translation unit.
 * These asserts are typically emitted by the generator.
*/
/* Deviation MISRAC2012-1 <+1> */
#define IOC_DESIZEOF_ASSERT(T) IOC_STATIC_ASSERT(sizeof(T) <= IOC_ILENGTH_MAX, IOC_DeSizeofAssert_##T)

#endif
