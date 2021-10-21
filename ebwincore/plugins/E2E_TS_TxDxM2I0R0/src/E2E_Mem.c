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
 * MISRAC2012-1) Deviated Rule: 18.4 (advisory)
 * The +, -, += and -= operators should not be applied to an expression of pointer type.
 *
 * Reason:
 * Pointer arithmetic is used for classical functions like memcpy() to
 * make them more readable and maintainable.
 *
 * MISRAC2012-2) Deviated Rule: 11.3 (required)
 * A cast shall not be performed between a pointer to object type
 * and a pointer to a different object type.
 *
 * Reason:
 * The memory routines are optimized for dealing with aligned memory
 * and the correct alignment has been checked before converting
 * to different object types (see MISRAC2012-3).
 *
 * MISRAC2012-3) Deviated Rule: 11.4 (advisory)
 * A conversion should not be performed between a pointer to object
 * and an integer type.
 *
 * Reason:
 * The memory routines are optimized for dealing with aligned memory
 * sections and need to check the correct alignment before the operation.
 *
 * MISRAC2012-4) Deviated Rule: 11.5 (advisory)
 * A conversion should not be performed from pointer to void into pointer to object.
 *
 * Reason:
 * The memory routines are optimized for dealing with aligned memory
 * sections.
 *
 * MISRAC2012-5) Deviated Rule: 10.8 (required)
 * The value of a composite expression shall not be cast to a wider essential type.
 *
 * Reason:
 * The source and destination pointers of E2E_MemCpy macros are incremented
 * and a cast is made based on the platform's specific size.
 */

/*==================[inclusions]=============================================*/

#include <E2E_Mem.h>

/*==================[macros]=================================================*/

#if (defined E2E_MEMCPY_NOCHECK)
#error E2E_MEMCPY_NOCHECK is already defined
#endif

/* Deviation MISRAC2012-1 <+19> */
/* Deviation MISRAC2012-2 <+18> */
/** \brief Copies \p len bytes of memory from \p dst to \p src using
 * type information passed in \p type
 *
 * \param[in] dst Pointer to the destination address
 * \param[in] src Pointer to the source address
 * \param[in] len Amount of bytes to copy
 * \param[in] type The type to perform the copying with
 */
#define E2E_MEMCPY_NOCHECK(dst, src, len, type)             \
  do {                                                      \
    /* loop and copy */                                     \
    while ((len) >= (uint32_least)sizeof(type))             \
    {                                                       \
      *(P2VAR(type, AUTOMATIC, E2E_APPL_DATA))(dst) =       \
         *(P2CONST(type, AUTOMATIC, E2E_APPL_DATA))(src);   \
      (src) += (uint32_least)sizeof(type);                  \
      (dst) += (uint32_least)sizeof(type);                  \
      (len) -= (uint32_least)sizeof(type);                  \
    }                                                       \
  } while ((0))

#if (defined E2E_MEM_IS_64ALIGNED)
#error E2E_MEM_IS_64ALIGNED is already defined
#endif

/** \brief Tests if the parameter \p x is 64-bit aligned
 *
 * \pre An address needs to be convertible to a uint32_least
 *
 * \param[in] x the address to check
 *
 * \return Alignment status
 * \retval 1 the variable is 64-bit aligned
 * \retval 0 the variable is not 64-bit aligned
 */
#define E2E_MEM_IS_64ALIGNED(x) (((uint32_least)(x) & E2E_ALIGNMENT_VALUE_C(7)) == E2E_ALIGNMENT_VALUE_C(0))

#if (defined E2E_MEM_IS_32ALIGNED)
#error E2E_MEM_IS_32ALIGNED is already defined
#endif

/* Deviation MISRAC2012-3 <+11> */
/** \brief Tests if the parameter \p x is 32-bit aligned
 *
 * \pre An address needs to be convertible to a uint32_least
 *
 * \param[in] x the address to check
 *
 * \return Alignment status
 * \retval 1 the variable is 32-bit aligned
 * \retval 0 the variable is not 32-bit aligned
 */
#define E2E_MEM_IS_32ALIGNED(x) (((uint32_least)(x) & E2E_ALIGNMENT_VALUE_C(3)) == E2E_ALIGNMENT_VALUE_C(0))

#if (defined E2E_MEM_IS_16ALIGNED)
#error E2E_MEM_IS_16ALIGNED is already defined
#endif

/* Deviation MISRAC2012-3 <+11> */
/** \brief Tests if the parameter \p x is 16-bit aligned
 *
 * \pre An address needs to be convertible to a uint32_least
 *
 * \param[in] x the address to check
 *
 * \return Alignment status
 * \retval 1 the variable is 16-bit aligned
 * \retval 0 the variable is not 16-bit aligned
 */
#define E2E_MEM_IS_16ALIGNED(x) (((uint32_least)(x) & E2E_ALIGNMENT_VALUE_C(1)) == E2E_ALIGNMENT_VALUE_C(0))

#if (defined E2E_MEM_OPT_LEN)
#error E2E_MEM_OPT_LEN is already defined
#endif

/** \brief Defines the minimum amount of data to try optimization */
#define E2E_MEM_OPT_LEN 16U

#if (defined E2E_MEM_IS_ENOUGH_DATA)
#error E2E_MEM_IS_ENOUGH_DATA is already defined
#endif

/** \brief tests if there is enough data to use optimization
 *
 * \param[in] len amount of bytes
 *
 * \return Alignment status
 * \retval 0 there is enough data
 * \retval 1 not enough data
 */
#define E2E_MEM_IS_ENOUGH_DATA(len) ((len) >= E2E_MEM_OPT_LEN)

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

#define E2E_START_SEC_CODE
#include <E2E_MemMap.h>

/*------------------[64-bit CPUs]--------------------------------------------*/
#if (CPU_TYPE == CPU_TYPE_64)
/* CHECK: NOPARSE */
/*------------------[E2E_MemCpy64]--------------------------------------------*/
FUNC(void, E2E_CODE) E2E_MemCpy64
(
  P2VAR(void, AUTOMATIC, E2E_APPL_DATA) dst,
  P2CONST(void, AUTOMATIC, E2E_APPL_DATA) src,
  uint32_least len
)
{
  /* define pointers for access */
  P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) src2 =
    (P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA)) src;
  /* Deviation MISRAC2012-4 */
  P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) dst2 =
    (P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA)) dst;
  uint32_least len2 = len;

  /* check if enough data is there for optimizing */
  if (E2E_MEM_IS_ENOUGH_DATA(len2))
  {
    /* check if src and dst are both 64-bit aligned */
    /* Deviation MISRAC2012-3 */
    if (E2E_MEM_IS_64ALIGNED(src2) && E2E_MEM_IS_64ALIGNED(dst2))
    {
       /* copy as much data in 64-bit mode */
       /* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-5 */
       E2E_MEMCPY_NOCHECK(dst2, src2, len2, uint64);
    }
    /* check if src and dst are both 32-bit aligned */
    /* Deviation MISRAC2012-3 */
    else if (E2E_MEM_IS_32ALIGNED(src2) && E2E_MEM_IS_32ALIGNED(dst2))
    {
      /* try to copy the data using 32-bit-wise copy */
      /* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-5 */
      E2E_MEMCPY_NOCHECK(dst2, src2, len2, uint32);
    }
    /* check if src and dst are both 16-bit aligned */
    /* Deviation MISRAC2012-3 */
    else if (E2E_MEM_IS_16ALIGNED(src2) && E2E_MEM_IS_16ALIGNED(dst2))
    {
      /* try to copy the data using 16-bit-wise copy */
      /* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-5 */
      E2E_MEMCPY_NOCHECK(dst2, src2, len2, uint16);
    }
    else
    {
      /* for MISRA */
    }
  }

  /* optimized case:   copy the rest byte-wise
   * unoptimized case: copy everything byte-wise */
  /* Deviation MISRAC2012-1, MISRAC2012-5 */
  E2E_MEMCPY_NOCHECK(dst2, src2, len2, uint8);
}
/* CHECK: PARSE */

/*------------------[32-bit CPUs]--------------------------------------------*/
#elif (CPU_TYPE == CPU_TYPE_32)

/*------------------[E2E_MemCpy32]--------------------------------------------*/
FUNC(void, E2E_CODE) E2E_MemCpy32
(
  P2VAR(void, AUTOMATIC, E2E_APPL_DATA) dst,
  P2CONST(void, AUTOMATIC, E2E_APPL_DATA) src,
  uint32_least len
)
{
  /* define pointers for access */
  P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) src2 =
    (P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA)) src;
  /* Deviation MISRAC2012-4 */
  P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) dst2 =
    (P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA)) dst;
  uint32_least len2 = len;

  /* check if enough data is there for optimizing */
  if (E2E_MEM_IS_ENOUGH_DATA(len2))
  {
    /* check if src and dst are both 32-bit aligned */
    /* Deviation MISRAC2012-3 */
    if (E2E_MEM_IS_32ALIGNED(src2) && E2E_MEM_IS_32ALIGNED(dst2))
    {
       /* copy as much data in 32-bit mode */
       /* Deviation MISRAC2012-1, MISRAC2012-2 */
       E2E_MEMCPY_NOCHECK(dst2, src2, len2, uint32);
    }
    /* check if src and dst are both 16-bit aligned */
    /* Deviation MISRAC2012-3 */
    else if (E2E_MEM_IS_16ALIGNED(src2) && E2E_MEM_IS_16ALIGNED(dst2))
    {
      /* try to copy the data using 16-bit-wise copy */
      /* Deviation MISRAC2012-1, MISRAC2012-2 */
      E2E_MEMCPY_NOCHECK(dst2, src2, len2, uint16);
    }
    else
    {
      /* for MISRA */
    }
  }

  /* optimized case:   copy the rest byte-wise
   * unoptimized case: copy everything byte-wise */
  /* Deviation MISRAC2012-1 */
  E2E_MEMCPY_NOCHECK(dst2, src2, len2, uint8);
}

#elif (CPU_TYPE == CPU_TYPE_16)
/*------------------[16-bit CPUs]--------------------------------------------*/
/* CHECK: NOPARSE */
/*------------------[E2E_MemCpy16]--------------------------------------------*/
FUNC(void, E2E_CODE) E2E_MemCpy16
(
  P2VAR(void, AUTOMATIC, E2E_APPL_DATA) dst,
  P2CONST(void, AUTOMATIC, E2E_APPL_DATA) src,
  uint32_least len
)
{
  /* define pointers for access */
  P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) src2 =
    (P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA)) src;
  /* Deviation MISRAC2012-4 */
  P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) dst2 =
    (P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA)) dst;
  uint32_least len2 = len;

  /* check if enough data is there for optimizing */
  if (E2E_MEM_IS_ENOUGH_DATA(len2))
  {
    /* check if src and dst are both 16-bit aligned */
    /* Deviation MISRAC2012-3 */
    if (E2E_MEM_IS_16ALIGNED(src2) && E2E_MEM_IS_16ALIGNED(dst2))
    {
      /* try to copy the data using 16-bit-wise copy */
      /* Deviation MISRAC2012-1, MISRAC2012-2 */
      E2E_MEMCPY_NOCHECK(dst2, src2, len2, uint16);
    }
  }

  /* optimized case:   copy the rest byte-wise
   * unoptimized case: copy everything byte-wise */
  /* Deviation MISRAC2012-1 */
  E2E_MEMCPY_NOCHECK(dst2, src2, len2, uint8);
}
/* CHECK: PARSE */
#elif (CPU_TYPE == CPU_TYPE_8)
/*------------------[8-bit CPUs]---------------------------------------------*/
/* CHECK: NOPARSE */
/*------------------[E2E_MemCpy8]---------------------------------------------*/
FUNC(void, E2E_CODE) E2E_MemCpy8
(
  P2VAR(void, AUTOMATIC, E2E_APPL_DATA) dst,
  P2CONST(void, AUTOMATIC, E2E_APPL_DATA) src,
  uint32_least len
)
{
  /* define pointers for access */
  P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) src2 =
    (P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA)) src;
  /* Deviation MISRAC2012-4 */
  P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) dst2 =
    (P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA)) dst;
  uint32_least len2 = len;

  /* copy everything byte-wise */
  /* Deviation MISRAC2012-1 */
  E2E_MEMCPY_NOCHECK(dst2, src2, len2, uint8);
}
/* CHECK: PARSE */
#endif /* CPU_TYPE */

/*==================[internal function definitions]==========================*/

#define E2E_STOP_SEC_CODE
#include <E2E_MemMap.h>

/*==================[end of file]============================================*/
