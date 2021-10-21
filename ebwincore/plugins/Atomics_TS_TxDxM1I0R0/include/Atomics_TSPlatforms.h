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
#ifndef ATOMICS_TSPLATFORMS_H
#define ATOMICS_TSPLATFORMS_H

#ifndef ATOMICS_USE_GENERIC_IMPL
/* For further information please refer to the AutoCore Generic Base documentation. */
#error "Please define ATOMICS_USE_GENERIC_IMPL to choose between generic or specialized implementation."
#endif

#if ATOMICS_USE_GENERIC_IMPL == 0
#include <Atomics_eb_vendor_check.h>
#include <Atomics_os_version_compare.h>

/* EB tresos AutoCore OS compatibility */
#if ATOMICS_OS_IS_OLDER_THAN(6,0,97)
#error "Versions of EB tresos AutoCore OS before 6.0.97 do not support atomic TS functions."
#endif /* ATOMICS_OS_IS_OLDER_THAN(6,0,97) */

#endif /* ATOMICS_USE_GENERIC_IMPL == 0 */

#include <Std_Types.h>
#include <TSCompiler.h>
#include <Atomics_platform_types.h>

#if ATOMICS_USE_GENERIC_IMPL == 0
#include <Os_types.h>
#include <Os_atomics_tsplatforms.h>
#endif /* ATOMICS_USE_GENERIC_IMPL == 0 */

/** \brief Atomically sets one bit in an object
 **
 ** This macro atomically sets the bit at index \a Bit in the object
 ** pointed to by \a Address. The maximum value for \a Bit depends on the number
 ** of bits in the object pointed to by \a Address.
 **
 ** Sequential consistency is ensured.
 **
 ** This function is deprecated.
 ** New code should use the API declared in Atomics.h.
 **
 ** \pre \a Address shall be a valid pointer
 ** \pre \a Address shall point to a modifiable location
 ** \pre \a Bit shall have a value between 0 and n-1 (inclusive), where n is the
 ** number of bits in the object pointed to by \a Address
 **
 ** \param[in]  Bit     The index of the bit to set. Its range is 0 to n-1 (inclusive).
 ** \param[out] Address Pointer to the object in which the bit at index
 **                     \a Bit shall be set.
 */
#if CPU_TYPE == CPU_TYPE_64
#define TS_AtomicSetBit(Address, Bit)                                                            \
  do {                                                                                           \
    if (sizeof(*(Address)) == 1U)                                                                \
    {                                                                                            \
      TS_AtomicSetBit_8((volatile P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA)) (Address), (Bit));    \
    }                                                                                            \
    else if (sizeof(*(Address)) == 2U)                                                           \
    {                                                                                            \
      TS_AtomicSetBit_16((volatile P2VAR(uint16, AUTOMATIC, BASE_APPL_DATA)) (Address), (Bit));  \
    }                                                                                            \
    else if (sizeof(*(Address)) == 4U)                                                           \
    {                                                                                            \
      TS_AtomicSetBit_32((volatile P2VAR(uint32, AUTOMATIC, BASE_APPL_DATA)) (Address), (Bit));  \
    }                                                                                            \
    else                                                                                         \
    {                                                                                            \
      TS_AtomicSetBit_64((volatile P2VAR(uint64, AUTOMATIC, BASE_APPL_DATA)) (Address), (Bit));  \
    }                                                                                            \
  } while (0)
#else /* CPU_TYPE != CPU_TYPE_64 */
#define TS_AtomicSetBit(Address, Bit)                                                            \
  do {                                                                                           \
    if (sizeof(*(Address)) == 1U)                                                                \
    {                                                                                            \
      TS_AtomicSetBit_8((volatile P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA)) (Address), (Bit));    \
    }                                                                                            \
    else if (sizeof(*(Address)) == 2U)                                                           \
    {                                                                                            \
      TS_AtomicSetBit_16((volatile P2VAR(uint16, AUTOMATIC, BASE_APPL_DATA)) (Address), (Bit));  \
    }                                                                                            \
    else                                                                                         \
    {                                                                                            \
      TS_AtomicSetBit_32((volatile P2VAR(uint32, AUTOMATIC, BASE_APPL_DATA)) (Address), (Bit));  \
    }                                                                                            \
  } while (0)
#endif /* CPU_TYPE == CPU_TYPE_64 */

/** \brief Atomically clears one bit in an object
 **
 ** This macro atomically clears the bit at index \a Bit in the object
 ** pointed to by \a Address. The maximum value for \a Bit depends on the number
 ** of bits in the object pointed to by \a Address.
 **
 ** Sequential consistency is ensured.
 **
 ** This function is deprecated.
 ** New code should use the API declared in Atomics.h.
 **
 ** \pre \a Address shall be a valid pointer
 ** \pre \a Address shall point to a modifiable location
 ** \pre \a Bit shall have a value between 0 and n-1 (inclusive), where n is the
 ** number of bits in the object pointed to by \a Address
 **
 ** \param[in]  Bit     The index of the bit to clear. Its range is 0 to n-1 (inclusive).
 ** \param[out] Address Pointer to the object in which the bit at index \a Bit
 **                     shall be cleared.
 */
#if (CPU_TYPE == CPU_TYPE_64)
#define TS_AtomicClearBit(Address, Bit)                                                            \
  do {                                                                                             \
    if (sizeof(*(Address)) == 1U)                                                                  \
    {                                                                                              \
      TS_AtomicClearBit_8((volatile P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA)) (Address), (Bit));    \
    }                                                                                              \
    else if (sizeof(*(Address)) == 2U)                                                             \
    {                                                                                              \
      TS_AtomicClearBit_16((volatile P2VAR(uint16, AUTOMATIC, BASE_APPL_DATA)) (Address), (Bit));  \
    }                                                                                              \
    else if (sizeof(*(Address)) == 4U)                                                             \
    {                                                                                              \
      TS_AtomicClearBit_32((volatile P2VAR(uint32, AUTOMATIC, BASE_APPL_DATA)) (Address), (Bit));  \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      TS_AtomicClearBit_64((volatile P2VAR(uint64, AUTOMATIC, BASE_APPL_DATA)) (Address), (Bit));  \
    }                                                                                              \
  } while (0)
#else /* CPU_TYPE != CPU_TYPE_64 */
#define TS_AtomicClearBit(Address, Bit)                                                            \
  do {                                                                                             \
    if (sizeof(*(Address)) == 1U)                                                                  \
    {                                                                                              \
      TS_AtomicClearBit_8((volatile P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA)) (Address), (Bit));    \
    }                                                                                              \
    else if (sizeof(*(Address)) == 2U)                                                             \
    {                                                                                              \
      TS_AtomicClearBit_16((volatile P2VAR(uint16, AUTOMATIC, BASE_APPL_DATA)) (Address), (Bit));  \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      TS_AtomicClearBit_32((volatile P2VAR(uint32, AUTOMATIC, BASE_APPL_DATA)) (Address), (Bit));  \
    }                                                                                              \
  } while (0)
#endif /* CPU_TYPE == CPU_TYPE_64 */


#if ATOMICS_USE_GENERIC_IMPL == 0

typedef os_intstatus_t TS_IntStatusType;

#define TS_IntDisable() OS_UserAtomicTSIntDisable()
#define TS_IntRestore(s) OS_UserAtomicTSIntRestore((s))
#define TS_AtomicSetBit_8(a, bi) OS_AtomicTSSetBit_8((os_uint8_t volatile *)(a), (os_uint32_t)(bi))
#define TS_AtomicClearBit_8(a, bi) OS_AtomicTSClearBit_8((os_uint8_t volatile *)(a), (os_uint32_t)(bi))
#define TS_AtomicSetBit_16(a, bi) OS_AtomicTSSetBit_16((os_uint16_t volatile *)(a), (os_uint32_t)(bi))
#define TS_AtomicClearBit_16(a, bi) OS_AtomicTSClearBit_16((os_uint16_t volatile *)(a), (os_uint32_t)(bi))
#define TS_AtomicSetBit_32(a, bi) OS_AtomicTSSetBit_32((os_uint32_t volatile *)(a), (os_uint32_t)(bi))
#define TS_AtomicClearBit_32(a, bi) OS_AtomicTSClearBit_32((os_uint32_t volatile *)(a), (os_uint32_t)(bi))

#if CPU_TYPE == CPU_TYPE_64
#define TS_AtomicSetBit_64(a, bi) OS_AtomicTSSetBit_64((os_uint64_t volatile *)(a), (os_uint32_t)(bi))
#define TS_AtomicClearBit_64(a, bi) OS_AtomicTSClearBit_64((os_uint64_t volatile *)(a), (os_uint32_t)(bi))
#endif /* CPU_TYPE == CPU_TYPE_64 */

#else /* ATOMICS_USE_GENERIC_IMPL != 0 */

/** \brief Type to store the interrupt status.
 **
 ** Objects of this type can be used to store the (enabled/disabled) status
 ** of hardware interrupts. These are usually exchanged between TS_IntDisable()
 ** and TS_IntRestore(), to disable and restore this status.
 */
typedef uint32 TS_IntStatusType;

#define ATOMICS_START_SEC_CODE
#include <Atomics_MemMap.h>

/** \brief Atomically sets a bit in a byte.
 **
 ** Atomically sets the specified bit in the object pointed to by \a addr.
 ** The bit is selected via its index \a bitIdx. Hence, \a bitIdx can take
 ** values from 0 to 7 (inclusive).
 **
 ** Sequential consistency is ensured.
 **
 ** This function is deprecated.
 ** New code should use the API declared in Atomics.h.
 **
 ** \param[in,out]  addr    The pointer to the object to manipulate.
 ** \param[in]      bitIdx  The index of the bit to set. It must be out of
 **                         0 to 7.
 */
extern FUNC(void, ATOMICS_CODE) TS_AtomicSetBit_8(
    volatile P2VAR(uint8, AUTOMATIC, ATOMICS_APPL_DATA) addr,
    VAR(uint32, AUTOMATIC) bitIdx);

/** \brief Atomically clears a bit in a byte.
 **
 ** Atomically clears the specified bit in the object pointed to by \a addr.
 ** The bit is selected via its index \a bitIdx. Hence, \a bitIdx can take
 ** values from 0 to 7 (inclusive).
 **
 ** Sequential consistency is ensured.
 **
 ** This function is deprecated.
 ** New code should use the API declared in Atomics.h.
 **
 ** \param[in,out]  addr    The pointer to the object to manipulate.
 ** \param[in]      bitIdx  The index of the bit to clear. It must be out of
 **                         0 to 7.
 */
extern FUNC(void, ATOMICS_CODE) TS_AtomicClearBit_8(
    volatile P2VAR(uint8, AUTOMATIC, ATOMICS_APPL_DATA) addr,
    VAR(uint32, AUTOMATIC) bitIdx);

/** \brief Atomically sets a bit in a 16-bit object.
 **
 ** Atomically sets the specified bit in the object pointed to by \a addr.
 ** The bit is selected via its index \a bitIdx. Hence, \a bitIdx can take
 ** values from 0 to 15 (inclusive).
 **
 ** Sequential consistency is ensured.
 **
 ** This function is deprecated.
 ** New code should use the API declared in Atomics.h.
 **
 ** \param[in,out]  addr    The pointer to the object to manipulate.
 **                         Natural alignment is assumed.
 ** \param[in]      bitIdx  The index of the bit to set. It must be out of
 **                         0 to 15.
 */
extern FUNC(void, ATOMICS_CODE) TS_AtomicSetBit_16(
    volatile P2VAR(uint16, AUTOMATIC, ATOMICS_APPL_DATA) addr,
    VAR(uint32, AUTOMATIC) bitIdx);

/** \brief Atomically clears a bit in a 16-bit object.
 **
 ** Atomically clears the specified bit in the object pointed to by \a addr.
 ** The bit is selected via its index \a bitIdx. Hence, \a bitIdx can take
 ** values from 0 to 15 (inclusive).
 **
 ** Sequential consistency is ensured.
 **
 ** This function is deprecated.
 ** New code should use the API declared in Atomics.h.
 **
 ** \param[in,out]  addr    The pointer to the object to manipulate.
 **                         Natural alignment is assumed.
 ** \param[in]      bitIdx  The index of the bit to clear. It must be out of
 **                         0 to 15.
 */
extern FUNC(void, ATOMICS_CODE) TS_AtomicClearBit_16(
    volatile P2VAR(uint16, AUTOMATIC, ATOMICS_APPL_DATA) addr,
    VAR(uint32, AUTOMATIC) bitIdx);

/** \brief Atomically sets a bit in a 32-bit object.
 **
 ** Atomically sets the specified bit in the object pointed to by \a addr.
 ** The bit is selected via its index \a bitIdx. Hence, \a bitIdx can take
 ** values from 0 to 31 (inclusive).
 **
 ** Sequential consistency is ensured.
 **
 ** This function is deprecated.
 ** New code should use the API declared in Atomics.h.
 **
 ** \param[in,out]  addr    The pointer to the object to manipulate.
 **                         Natural alignment is assumed.
 ** \param[in]      bitIdx  The index of the bit to set. It must be out of
 **                         0 to 31.
 */
extern FUNC(void, ATOMICS_CODE) TS_AtomicSetBit_32(
    volatile P2VAR(uint32, AUTOMATIC, ATOMICS_APPL_DATA) addr,
    VAR(uint32, AUTOMATIC) bitIdx);

/** \brief Atomically clears a bit in a 32-bit object.
 **
 ** Atomically clears the specified bit in the object pointed to by \a addr.
 ** The bit is selected via its index \a bitIdx. Hence, \a bitIdx can take
 ** values from 0 to 31 (inclusive).
 **
 ** Sequential consistency is ensured.
 **
 ** This function is deprecated.
 ** New code should use the API declared in Atomics.h.
 **
 ** \param[in,out]  addr    The pointer to the object to manipulate.
 **                         Natural alignment is assumed.
 ** \param[in]      bitIdx  The index of the bit to clear. It must be out of
 **                         0 to 31.
 */
extern FUNC(void, ATOMICS_CODE) TS_AtomicClearBit_32(
    volatile P2VAR(uint32, AUTOMATIC, ATOMICS_APPL_DATA) addr,
    VAR(uint32, AUTOMATIC) bitIdx);

#if CPU_TYPE == CPU_TYPE_64

/** \brief Atomically sets a bit in a 64-bit object.
 **
 ** Atomically sets the specified bit in the object pointed to by \a addr.
 ** The bit is selected via its index \a bitIdx. Hence, \a bitIdx can take
 ** values from 0 to 63 (inclusive).
 **
 ** Sequential consistency is ensured.
 **
 ** This function is deprecated.
 ** New code should use the API declared in Atomics.h.
 **
 ** \param[in,out]  addr    The pointer to the object to manipulate.
 **                         Natural alignment is assumed.
 ** \param[in]      bitIdx  The index of the bit to set. It must be out of
 **                         0 to 63.
 */
extern FUNC(void, ATOMICS_CODE) TS_AtomicSetBit_64(
    volatile P2VAR(uint64, AUTOMATIC, ATOMICS_APPL_DATA) addr,
    VAR(uint32, AUTOMATIC) bitIdx);

/** \brief Atomically clears a bit in a 64-bit object.
 **
 ** Atomically clears the specified bit in the object pointed to by \a addr.
 ** The bit is selected via its index \a bitIdx. Hence, \a bitIdx can take
 ** values from 0 to 63 (inclusive).
 **
 ** Sequential consistency is ensured.
 **
 ** This function is deprecated.
 ** New code should use the API declared in Atomics.h.
 **
 ** \param[in,out]  addr    The pointer to the object to manipulate.
 **                         Natural alignment is assumed.
 ** \param[in]      bitIdx  The index of the bit to clear. It must be out of
 **                         0 to 63.
 */
extern FUNC(void, ATOMICS_CODE) TS_AtomicClearBit_64(
    volatile P2VAR(uint64, AUTOMATIC, ATOMICS_APPL_DATA) addr,
    VAR(uint32, AUTOMATIC) bitIdx);

#endif /* CPU_TYPE == CPU_TYPE_64 */

/** \brief Suspends all interrupts.
 **
 ** This function-like macro suspends all interrupts and returns the current
 ** interrupt status. This status is passed to TS_IntRestore() to resume
 ** interrupts later on.
 **
 ** Please note, that each call of TS_IntDisable() should be accompanied by
 ** a call of TS_IntRestore() to keep the system responsive. This means,
 ** these macros usually occur as pairs in your code. Furthermore, the
 ** value passed to TS_IntRestore() must be the return value of the most recent
 ** call of TS_IntDisable(). Violations result in unexpected bahavior.
 **
 ** This function is deprecated.
 ** New code should use the API declared in Atomics.h.
 **
 ** \return The current interrupt status before all hardware interrupts were
 ** suspended. This value may be passed to TS_IntRestore(), which is on the
 ** same nesting level.
 */
extern FUNC(TS_IntStatusType, ATOMICS_CODE) TS_IntDisable(void);

/** \brief Restores the given interrupt status.
 **
 ** This function restores the interrupt status passed to it.
 ** This status is the return value of the most recent TS_IntDisable() call.
 **
 ** This function is deprecated.
 ** New code should use the API declared in Atomics.h.
 **
 ** \param[in] intStatus The interrupt status to be restored. This is the return
 **                      value of the most recent TS_IntDisable() call.
 */
extern FUNC(void, ATOMICS_CODE) TS_IntRestore(
    VAR(TS_IntStatusType, AUTOMATIC) intStatus);

#define ATOMICS_STOP_SEC_CODE
#include <Atomics_MemMap.h>

#endif /* ATOMICS_USE_GENERIC_IMPL == 0 */

#endif /* ATOMICS_TSPLATFORMS_H */
