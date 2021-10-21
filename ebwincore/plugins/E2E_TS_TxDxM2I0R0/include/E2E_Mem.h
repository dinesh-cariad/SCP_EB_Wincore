/**
 * \file
 *
 * \brief AUTOSAR E2E
 *
 * This file contains the implementation of the AUTOSAR
 * module E2E.
 *
 * \version 2.0.18
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 20.10 (advisory)
 *   The # and ## preprocessor operators should not be used.
 *
 *   Reason:
 *   The operators are used to add the 'U' or 'UL' suffix to constants
 *   via macros which can only be performed using the '##' operator.
 */
#ifndef E2E_MEM_H
#define E2E_MEM_H

/*==================[inclusions]=============================================*/

#include <Std_Types.h>

/*==================[macros]=================================================*/

#if (defined E2E_MemCpy) /* to prevent double definition */
#error E2E_MemCpy is already defined
#endif /* if (defined E2E_MemCpy) */

#if (CPU_TYPE == CPU_TYPE_64)

/** \brief This macro maps to the CPU specific macro for copying memory bytes, for 64bit CPU to E2E_MemCpy64() */
#define E2E_MemCpy(d,s,n) (E2E_MemCpy64((d),(s),(n)))

#elif (CPU_TYPE == CPU_TYPE_32)

/** \brief This macro maps to the CPU specific macro for copying memory bytes, for 32bit CPU to E2E_MemCpy32() */
#define E2E_MemCpy(d,s,n) (E2E_MemCpy32((d),(s),(n)))

#elif (CPU_TYPE == CPU_TYPE_16)

/** \brief This macro maps to the CPU specific macro for copying memory bytes, for 16bit CPU to E2E_MemCpy16() */
#define E2E_MemCpy(d,s,n) (E2E_MemCpy16((d),(s),(n)))

#elif (CPU_TYPE == CPU_TYPE_8)

/** \brief This macro maps to the CPU specific macro for copying memory bytes, for 8bit CPU to E2E_MemCpy8() */
#define E2E_MemCpy(d,s,n) (E2E_MemCpy8((d),(s),(n)))

#endif /* CPU_TYPE */

#if (defined E2E_ALIGNMENT_VALUE_C) /* To prevent double declaration */
#error E2E_ALIGNMENT_VALUE_C already defined
#endif /* if (defined E2E_ALIGNMENT_VALUE_C) */

#if (CPU_TYPE == CPU_TYPE_64)

/** \brief Macro to cast a value to check the alignment in dependency of platform specific type CPU_TYPE) */
/* Deviation MISRAC2012-1 <+7> */
#define E2E_ALIGNMENT_VALUE_C(x) (x ## ULL)

#else

#define E2E_ALIGNMENT_VALUE_C(x) (x ## UL)

#endif /* CPU_TYPE */

/*==================[external function definitions]==========================*/

#define E2E_START_SEC_CODE
#include <E2E_MemMap.h>

#if (CPU_TYPE == CPU_TYPE_64)

/** \brief Copies \p len bytes of memory from \p dst to \p src (64-bit version)
 *
 * This function copies a memory block of length \p len
 * from the source \p src to the destination \p dst.
 *
 * \pre The pointer \p dst points to a valid address
 * \pre The pointer \p src points to a valid address
 *
 * \note the function does not perform overlapping checks
 *
 * \param[in] dst Pointer to the destination address
 * \param[in] src Pointer to the source address
 * \param[in] len Amount of bytes to copy
 */
extern FUNC(void, E2E_CODE) E2E_MemCpy64
(
 P2VAR(void, AUTOMATIC, E2E_APPL_DATA) dst,
 P2CONST(void, AUTOMATIC, E2E_APPL_DATA) src,
 uint32_least len
);

#elif (CPU_TYPE == CPU_TYPE_32)

/** \brief Copies \p len bytes of memory from \p dst to \p src (32-bit version)
 *
 * This function copies a memory block of length \p len
 * from the source \p src to the destination \p dst.
 *
 * \pre The pointer \p dst points to a valid address
 * \pre The pointer \p src points to a valid address
 *
 * \note the function does not perform overlapping checks
 *
 * \param[in] dst Pointer to the destination address
 * \param[in] src Pointer to the source address
 * \param[in] len Amount of bytes to copy
 */
extern FUNC(void, E2E_CODE) E2E_MemCpy32
(
 P2VAR(void, AUTOMATIC, E2E_APPL_DATA) dst,
 P2CONST(void, AUTOMATIC, E2E_APPL_DATA) src,
 uint32_least len
);

#elif (CPU_TYPE == CPU_TYPE_16)

/** \brief Copies \p len bytes of memory from \p dst to \p src (16-bit version)
 *
 * This function copies a memory block of length \p len
 * from the source \p src to the destination \p dst.
 *
 * \pre The pointer \p dst points to a valid address
 * \pre The pointer \p src points to a valid address
 *
 * \note The function does not perform overlapping checks
 *
 * \param[in] dst Pointer to the destination address
 * \param[in] src Pointer to the source address
 * \param[in] len Amount of bytes to copy
 */
extern FUNC(void, E2E_CODE) E2E_MemCpy16
(
  P2VAR(void, AUTOMATIC, E2E_APPL_DATA) dst,
  P2CONST(void, AUTOMATIC, E2E_APPL_DATA) src,
  uint32_least len
);

#elif (CPU_TYPE == CPU_TYPE_8)

/** \brief Copies \p len bytes of memory from \p dst to \p src
 * (generic version)
 *
 * This function copies a memory block of length \p len
 * from the source \p src to the destination \p dst.
 *
 * \pre the pointer \p dst points to a valid address
 * \pre the pointer \p src points to a valid address
 *
 * \note the function does not perform overlapping checks
 *
 * \param[in] dst Pointer to the destination address
 * \param[in] src Pointer to the source address
 * \param[in] len Amount of bytes to copy
 */
extern FUNC(void, E2E_CODE) E2E_MemCpy8
(
  P2VAR(void, AUTOMATIC, E2E_APPL_DATA) dst,
  P2CONST(void, AUTOMATIC, E2E_APPL_DATA) src,
  uint32_least len
);

#endif /* CPU_TYPE */

#define E2E_STOP_SEC_CODE
#include <E2E_MemMap.h>

/*==================[internal function definitions]==========================*/

#endif /* ifndef E2E_MEM_H */
/*==================[end of file]============================================*/
