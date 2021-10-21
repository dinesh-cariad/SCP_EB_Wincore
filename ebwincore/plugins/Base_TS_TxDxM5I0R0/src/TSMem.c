/**
 * \file
 *
 * \brief AUTOSAR Base
 *
 * This file contains the implementation of the AUTOSAR
 * module Base.
 *
 * \version 5.0.31
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
 * The value of a composite expression shall not be cast to a different essential type
 * category or a wider essential type.
 *
 * Reason:
 * The source and destination pointers of the macros to copy, set, or compare memory
 * are incremented, and a cast is required based on the platform's specific pointer size.
 */

/*==================[inclusions]=============================================*/

#include <TSMem.h>
#include <TSAutosar.h>

/*==================[macros]=================================================*/

#if (defined TS_MEMCPY_NOCHECK)
#error TS_MEMCPY_NOCHECK is already defined
#endif

/** \brief Copies \p len bytes of memory from \p dst to \p src using
 * type information passed in \p type
 *
 * \param[in] dst Pointer to the destination address
 * \param[in] src Pointer to the source address
 * \param[in] len Amount of bytes to copy
 * \param[in] type The type to perform the copying with
 */
/* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-5 */
#define TS_MEMCPY_NOCHECK(dst, src, len, type)              \
  do {                                                      \
    /* loop and copy */                                     \
    while ((len) >= (usize)sizeof(type))                    \
    {                                                       \
      *(P2VAR(type, AUTOMATIC, BASE_APPL_DATA))(dst) =      \
         *(P2CONST(type, AUTOMATIC, BASE_APPL_DATA))(src);  \
      (src) += (usize)sizeof(type);                         \
      (dst) += (usize)sizeof(type);                         \
      (len) -= (usize)sizeof(type);                         \
    }                                                       \
  } while (0)

/** \brief Assigns \p len times the value \p val to \p dst  using
 * type information passed in  \p type
 *
 * \param[in] dst Pointer to the destination address
 * \param[in] val Value to be assigned
 * \param[in] len Amount of bytes to assign
 * \param[in] type The type to perform the assignment with
 */
/* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-5 */
#define TS_MEMSET_NOCHECK(dst, val, len, type)                \
  do                                                          \
  {                                                           \
    while ((len) >= (usize)sizeof(type))                      \
    {                                                         \
      *(P2VAR(type, AUTOMATIC, BASE_APPL_DATA))(dst) = (val); \
      (dst) += (usize)sizeof(type);                           \
      (len) -= (usize)sizeof(type);                           \
    }                                                         \
  }                                                           \
  while (0)

#if (defined TS_MEMCMP_NOCHECK)
#error TS_MEMCMP_NOCHECK is already defined
#endif

/** \brief Compares \p len bytes of memory from \p dst to \p src using
 * type information passed in \p type.
 *
 * \pre RetVal should by of type Std_ReturnType
 *
 * \param[in] a Pointer to the first memory block to compare
 * \param[in] b Pointer to the second memory block to compare
 * \param[in] len Amount of bytes to compare
 * \param[in] RetVal The return value to pass result information
 * \param[in] type The type to perform the comparison with
 */
/* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-5 */
#define TS_MEMCMP_NOCHECK(a, b, len, RetVal, type)                \
   do {                                                           \
    (RetVal) = E_OK;                                              \
    /* loop and compare */                                        \
     while ((len) >= (usize)sizeof(type))                         \
    {                                                             \
       if ((*(P2CONST(type, AUTOMATIC, BASE_APPL_DATA))(a)) !=    \
           *(P2CONST(type, AUTOMATIC, BASE_APPL_DATA))(b))        \
      {                                                           \
        (RetVal) = E_NOT_OK;                                      \
        break;                                                    \
      }                                                           \
      else                                                        \
      {                                                           \
        (a) += (usize)sizeof(type);                               \
        (b) += (usize)sizeof(type);                               \
        (len) -= (usize)sizeof(type);                             \
      }                                                           \
    }                                                             \
  } while (0)

#if (defined TS_MEM_IS_64ALIGNED)
#error TS_MEM_IS_64ALIGNED is already defined
#endif

/** \brief Tests if the parameter \p x is 64-bit aligned
 *
 * \pre An address needs to be convertible to a usize
 *
 * \param[in] x the address to check
 *
 * \return Alignment status
 * \retval 1 the variable is 64-bit aligned
 * \retval 0 the variable is not 64-bit aligned
 */
/* Deviation MISRAC2012-3 */
#define TS_MEM_IS_64ALIGNED(x) (((usize)(x) & USIZE_C(7)) == USIZE_C(0))

#if (defined TS_MEM_IS_32ALIGNED)
#error TS_MEM_IS_32ALIGNED is already defined
#endif

/** \brief Tests if the parameter \p x is 32-bit aligned
 *
 * \pre An address needs to be convertible to a usize
 *
 * \param[in] x the address to check
 *
 * \return Alignment status
 * \retval 1 the variable is 32-bit aligned
 * \retval 0 the variable is not 32-bit aligned
 */
/* Deviation MISRAC2012-3 */
#define TS_MEM_IS_32ALIGNED(x) (((usize)(x) & USIZE_C(3)) == USIZE_C(0))

#if (defined TS_MEM_IS_16ALIGNED)
#error TS_MEM_IS_16ALIGNED is already defined
#endif

/** \brief Tests if the parameter \p x is 16-bit aligned
 *
 * \pre An address needs to be convertible to a usize
 *
 * \param[in] x the address to check
 *
 * \return Alignment status
 * \retval 1 the variable is 16-bit aligned
 * \retval 0 the variable is not 16-bit aligned
 */
/* Deviation MISRAC2012-3 */
#define TS_MEM_IS_16ALIGNED(x) (((usize)(x) & USIZE_C(1)) == USIZE_C(0))

#if (defined TS_MEM_OPT_LEN)
#error TS_MEM_OPT_LEN is already defined
#endif

/** \brief Defines the minimum amount of data to try optimization */
#define TS_MEM_OPT_LEN 16U

#if (defined TS_MEM_IS_ENOUGH_DATA)
#error TS_MEM_IS_ENOUGH_DATA is already defined
#endif

/** \brief tests if there is enough data to use optimization
 *
 * \param[in] len amount of bytes
 *
 * \return Alignment status
 * \retval 0 there is enough data
 * \retval 1 not enough data
 */
#define TS_MEM_IS_ENOUGH_DATA(len) ((len) >= TS_MEM_OPT_LEN)

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

#define BASE_START_SEC_CODE
#include <Base_MemMap.h>

/*------------------[64-bit CPUs]--------------------------------------------*/
#if (CPU_TYPE == CPU_TYPE_64)

/*------------------[TS_MemCpy64]--------------------------------------------*/
FUNC(void, BASE_CODE) TS_MemCpy64
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) src,
  const usize len
)
{
  /* define pointers for access */
  P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA) src2 =
    (P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA)) src;
  /* Deviation MISRAC2012-4 */
  P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA) dst2 =
    (P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA)) dst;
  usize len2 = len;

  /* check if enough data is there for optimizing */
  if (TS_MEM_IS_ENOUGH_DATA(len2))
  {
    /* check if src and dst are both 64-bit aligned */
    /* Deviation MISRAC2012-3 */
    if (TS_MEM_IS_64ALIGNED(src2) && TS_MEM_IS_64ALIGNED(dst2))
    {
       /* copy as much data in 64-bit mode */
       /* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-5 */
       TS_MEMCPY_NOCHECK(dst2, src2, len2, uint64);
    }
    /* check if src and dst are both 32-bit aligned */
    /* Deviation MISRAC2012-3 */
    else if (TS_MEM_IS_32ALIGNED(src2) && TS_MEM_IS_32ALIGNED(dst2))
    {
      /* try to copy the data using 32-bit-wise copy */
      /* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-5 */
      TS_MEMCPY_NOCHECK(dst2, src2, len2, uint32);
    }
    /* check if src and dst are both 16-bit aligned */
    /* Deviation MISRAC2012-3 */
    else if (TS_MEM_IS_16ALIGNED(src2) && TS_MEM_IS_16ALIGNED(dst2))
    {
      /* try to copy the data using 16-bit-wise copy */
      /* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-5 */
      TS_MEMCPY_NOCHECK(dst2, src2, len2, uint16);
    }
    else
    {
      /* for MISRA */
    }
  }

  /* optimized case:   copy the rest byte-wise
   * unoptimized case: copy everything byte-wise */
  /* Deviation MISRAC2012-1, MISRAC2012-5 */
  TS_MEMCPY_NOCHECK(dst2, src2, len2, uint8);
}

/*------------------[TS_MemSet64]--------------------------------------------*/
FUNC(void, BASE_CODE) TS_MemSet64
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  const uint8 val,
  const usize len
)
{
  /* define pointer for access */
  /* Deviation MISRAC2012-4 */
  P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA) dst2 =
    (P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA)) dst;
  usize len2 = len;

  /* check if enough data is there for optimizing */
  if (TS_MEM_IS_ENOUGH_DATA(len2))
  {
    /* check if dst is 64-bit aligned */
    /* Deviation MISRAC2012-3 */
    if (TS_MEM_IS_64ALIGNED(dst2))
    {
      /* construct a 64-bit value from the 8-bit input */
      const uint64 val2 =  (uint64)val         | ((uint64)val << 8U)  |
                          ((uint64)val << 16U) | ((uint64)val << 24U) |
                          ((uint64)val << 32U) | ((uint64)val << 40U) |
                          ((uint64)val << 48U) | ((uint64)val << 56U);

      /* assign data in 64-bit mode */
      /* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-5 */
      TS_MEMSET_NOCHECK(dst2, val2, len2, uint64);
    }
    /* check if dst is 32-bit aligned */
    /* Deviation MISRAC2012-3 */
    else if (TS_MEM_IS_32ALIGNED(dst2))
    {
      /* construct a 32-bit value from the 8-bit input */
      const uint32 val2 =  (uint32)val         | ((uint32)val << 8U)  |
                          ((uint32)val << 16U) | ((uint32)val << 24U);

      /* assign data in 32-bit mode */
      /* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-5 */
      TS_MEMSET_NOCHECK(dst2, val2, len2, uint32);
    }
    /* check if dst is 16-bit aligned */
    /* Deviation MISRAC2012-3 */
    else if (TS_MEM_IS_16ALIGNED(dst2))
    {
      /* construct a 16-bit value from the 8-bit input */
      const uint16 val2 = (uint16)val | (uint16)((uint16)val << 8U);

      /* assign data in 16-bit mode */
      /* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-5 */
      TS_MEMSET_NOCHECK(dst2, val2, len2, uint16);
    }
    else
    {
      /* for MISRA */
    }
  }

  /* optimized case:   assign the rest byte-wise
   * unoptimized case: assign everything byte-wise */
  /* Deviation MISRAC2012-1, MISRAC2012-5 */
  TS_MEMSET_NOCHECK(dst2, val, len2, uint8);
}

/*------------------[TS_MemBZero64]------------------------------------------*/
FUNC(void, BASE_CODE) TS_MemBZero64
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  const usize len
)
{
  /* define pointer for access */
  /* Deviation MISRAC2012-4 */
  P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA) dst2 =
    (P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA)) dst;
  usize len2 = len;

  /* check if enough data is there for optimizing */
  if (TS_MEM_IS_ENOUGH_DATA(len2))
  {
    /* check if dst is 64-bit aligned */
    /* Deviation MISRAC2012-3 */
    if (TS_MEM_IS_64ALIGNED(dst2))
    {
      /* assign data in 64-bit mode */
      /* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-5 */
      TS_MEMSET_NOCHECK(dst2, UINT64_C(0), len2, uint64);
    }
    /* check if dst is 32-bit aligned */
    /* Deviation MISRAC2012-3 */
    else if (TS_MEM_IS_32ALIGNED(dst2))
    {
      /* assign data in 32-bit mode */
      /* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-5 */
      TS_MEMSET_NOCHECK(dst2, UINT32_C(0), len2, uint32);
    }
    /* check if dst is 16-bit aligned */
    /* Deviation MISRAC2012-3 */
    else if (TS_MEM_IS_16ALIGNED(dst2))
    {
      /* assign data in 16-bit mode */
      /* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-5 */
      TS_MEMSET_NOCHECK(dst2, UINT16_C(0), len2, uint16);
    }
    else
    {
      /* for MISRA */
    }
  }

  /* optimized case:   assign the rest byte-wise
   * unoptimized case: assign everything byte-wise */
  /* Deviation MISRAC2012-1, MISRAC2012-5 */
  TS_MEMSET_NOCHECK(dst2, UINT8_C(0), len2, uint8);
}

/*------------------[TS_MemCmp64]--------------------------------------------*/
FUNC(Std_ReturnType, BASE_CODE) TS_MemCmp64
(
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) a,
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) b,
  const usize len
)
{
  /* define type specific pointers for suitable access */
  P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA) a2 =
    (P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA)) a;
  P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA) b2 =
    (P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA)) b;
  usize len2 = len;
  Std_ReturnType RetVal;

  /* check if enough data is there for optimizing */
  if (TS_MEM_IS_ENOUGH_DATA(len2))
  {
    /* check if a and b are both 64-bit aligned */
    /* Deviation MISRAC2012-3 */
    if (TS_MEM_IS_64ALIGNED(a2) && TS_MEM_IS_64ALIGNED(b2))
    {
      /* compare data in 64-bit mode */
      /* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-5 */
      TS_MEMCMP_NOCHECK(a2, b2, len2, RetVal, uint64);
    }
    /* check if a and b are both 32-bit aligned */
    /* Deviation MISRAC2012-3 */
    else if (TS_MEM_IS_32ALIGNED(a2) && TS_MEM_IS_32ALIGNED(b2))
    {
      /* compare data in 32-bit mode */
      /* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-5 */
      TS_MEMCMP_NOCHECK(a2, b2, len2, RetVal, uint32);
    }
    /* check if a and b are both 16-bit aligned */
    /* Deviation MISRAC2012-3 */
    else if (TS_MEM_IS_16ALIGNED(a2) && TS_MEM_IS_16ALIGNED(b2))
    {
      /* compare data in 16-bit mode */
      /* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-5 */
      TS_MEMCMP_NOCHECK(a2, b2, len2, RetVal, uint16);
    }
    else
    {
      RetVal = E_OK;
    }
  }
  else
  {
    RetVal = E_OK;
  }

  /* the optimized compare macros may have already found a
   * difference, so we have to compare again */
  if (RetVal == E_OK)
  {
    /* optimized case:   compare the rest byte-wise
     * unoptimized case: compare everything byte-wise */
    /* Deviation MISRAC2012-1, MISRAC2012-5 */
    TS_MEMCMP_NOCHECK(a2, b2, len2, RetVal, uint8);
  }

  return RetVal;
}

/*------------------[32-bit CPUs]--------------------------------------------*/
#elif (CPU_TYPE == CPU_TYPE_32)

/*------------------[TS_MemCpy32]--------------------------------------------*/
FUNC(void, BASE_CODE) TS_MemCpy32
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) src,
  const usize len
)
{
  /* define pointers for access */
  P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA) src2 =
    (P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA)) src;
  /* Deviation MISRAC2012-4 */
  P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA) dst2 =
    (P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA)) dst;
  usize len2 = len;

  /* check if enough data is there for optimizing */
  if (TS_MEM_IS_ENOUGH_DATA(len2))
  {
    /* check if src and dst are both 32-bit aligned */
    /* Deviation MISRAC2012-3 */
    if (TS_MEM_IS_32ALIGNED(src2) && TS_MEM_IS_32ALIGNED(dst2))
    {
       /* copy as much data in 32-bit mode */
       /* Deviation MISRAC2012-1, MISRAC2012-2 */
       TS_MEMCPY_NOCHECK(dst2, src2, len2, uint32);
    }
    /* check if src and dst are both 16-bit aligned */
    /* Deviation MISRAC2012-3 */
    else if (TS_MEM_IS_16ALIGNED(src2) && TS_MEM_IS_16ALIGNED(dst2))
    {
      /* try to copy the data using 16-bit-wise copy */
      /* Deviation MISRAC2012-1, MISRAC2012-2 */
      TS_MEMCPY_NOCHECK(dst2, src2, len2, uint16);
    }
    else
    {
      /* for MISRA */
    }
  }

  /* optimized case:   copy the rest byte-wise
   * unoptimized case: copy everything byte-wise */
  /* Deviation MISRAC2012-1 */
  TS_MEMCPY_NOCHECK(dst2, src2, len2, uint8);
}

/*------------------[TS_MemSet32]--------------------------------------------*/
FUNC(void, BASE_CODE) TS_MemSet32
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  const uint8 val,
  const usize len
)
{
  /* define pointer for access */
  /* Deviation MISRAC2012-4 */
  P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA) dst2 =
    (P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA)) dst;
  usize len2 = len;

  /* check if enough data is there for optimizing */
  if (TS_MEM_IS_ENOUGH_DATA(len2))
  {
    /* check if dst is 32-bit aligned */
    /* Deviation MISRAC2012-3 */
    if (TS_MEM_IS_32ALIGNED(dst2))
    {
      /* construct a 32-bit value from the 8-bit input */
      const uint32 val2 =  (uint32)val         | ((uint32)val << 8U)  |
                          ((uint32)val << 16U) | ((uint32)val << 24U);

      /* assign data in 32-bit mode */
      /* Deviation MISRAC2012-1, MISRAC2012-2 */
      TS_MEMSET_NOCHECK(dst2, val2, len2, uint32);
    }
    /* check if dst is 16-bit aligned */
    /* Deviation MISRAC2012-3 */
    else if (TS_MEM_IS_16ALIGNED(dst2))
    {
      /* construct a 16-bit value from the 8-bit input */
      const uint16 val2 = (uint16)val | (uint16)((uint16)val << 8U);

      /* assign data in 16-bit mode */
      /* Deviation MISRAC2012-1, MISRAC2012-2 */
      TS_MEMSET_NOCHECK(dst2, val2, len2, uint16);
    }
    else
    {
      /* for MISRA */
    }
  }

  /* optimized case:   assign the rest byte-wise
   * unoptimized case: assign everything byte-wise */
  /* Deviation MISRAC2012-1 */
  TS_MEMSET_NOCHECK(dst2, val, len2, uint8);
}

/*------------------[TS_MemBZero32]------------------------------------------*/
FUNC(void, BASE_CODE) TS_MemBZero32
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  const usize len
)
{
  /* define pointer for access */
  /* Deviation MISRAC2012-4 */
  P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA) dst2 =
    (P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA)) dst;
  usize len2 = len;

  /* check if enough data is there for optimizing */
  if (TS_MEM_IS_ENOUGH_DATA(len2))
  {
    /* check if dst is 32-bit aligned */
    /* Deviation MISRAC2012-3 */
    if (TS_MEM_IS_32ALIGNED(dst2))
    {
      /* assign data in 32-bit mode */
      /* Deviation MISRAC2012-1, MISRAC2012-2 */
      TS_MEMSET_NOCHECK(dst2, UINT32_C(0), len2, uint32);
    }
    /* check if dst is 16-bit aligned */
    /* Deviation MISRAC2012-3 */
    else if (TS_MEM_IS_16ALIGNED(dst2))
    {
      /* assign data in 16-bit mode */
      /* Deviation MISRAC2012-1, MISRAC2012-2 */
      TS_MEMSET_NOCHECK(dst2, UINT16_C(0), len2, uint16);
    }
    else
    {
      /* for MISRA */
    }
  }

  /* optimized case:   assign the rest byte-wise
   * unoptimized case: assign everything byte-wise */
  /* Deviation MISRAC2012-1 */
  TS_MEMSET_NOCHECK(dst2, UINT8_C(0), len2, uint8);
}

/*------------------[TS_MemCmp32]--------------------------------------------*/
FUNC(Std_ReturnType, BASE_CODE) TS_MemCmp32
(
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) a,
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) b,
  const usize len
)
{
  /* define type specific pointers for suitable access */
  P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA) a2 =
    (P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA)) a;
  P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA) b2 =
    (P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA)) b;
  usize len2 = len;
  Std_ReturnType RetVal;

  /* check if enough data is there for optimizing */
  if (TS_MEM_IS_ENOUGH_DATA(len2))
  {
    /* check if a and b are both 32-bit aligned */
    /* Deviation MISRAC2012-3 */
    if (TS_MEM_IS_32ALIGNED(a2) && TS_MEM_IS_32ALIGNED(b2))
    {
      /* compare data in 32-bit mode */
      /* Deviation MISRAC2012-1, MISRAC2012-2 */
      TS_MEMCMP_NOCHECK(a2, b2, len2, RetVal, uint32);

    }
    /* check if a and b are both 16-bit aligned */
    /* Deviation MISRAC2012-3 */
    else if (TS_MEM_IS_16ALIGNED(a2) && TS_MEM_IS_16ALIGNED(b2))
    {
      /* compare data in 16-bit mode */
      /* Deviation MISRAC2012-1, MISRAC2012-2 */
      TS_MEMCMP_NOCHECK(a2, b2, len2, RetVal, uint16);
    }
    else
    {
      RetVal = E_OK;
    }
  }
  else
  {
    RetVal = E_OK;
  }

  /* the optimized compare macros may have already found a
   * difference, so we have to compare again */
  if (RetVal == E_OK)
  {
    /* optimized case:   compare the rest byte-wise
     * unoptimized case: compare everything byte-wise */
    /* Deviation MISRAC2012-1 */
    TS_MEMCMP_NOCHECK(a2, b2, len2, RetVal, uint8);
  }

  return RetVal;
}

#elif (CPU_TYPE == CPU_TYPE_16)
/*------------------[16-bit CPUs]--------------------------------------------*/

/*------------------[TS_MemCpy16]--------------------------------------------*/
FUNC(void, BASE_CODE) TS_MemCpy16
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) src,
  const usize len
)
{
  /* define pointers for access */
  P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA) src2 =
    (P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA)) src;
  /* Deviation MISRAC2012-4 */
  P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA) dst2 =
    (P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA)) dst;
  usize len2 = len;

  /* check if enough data is there for optimizing */
  if (TS_MEM_IS_ENOUGH_DATA(len2))
  {
    /* check if src and dst are both 16-bit aligned */
    /* Deviation MISRAC2012-3 */
    if (TS_MEM_IS_16ALIGNED(src2) && TS_MEM_IS_16ALIGNED(dst2))
    {
      /* try to copy the data using 16-bit-wise copy */
      /* Deviation MISRAC2012-1, MISRAC2012-2 */
      TS_MEMCPY_NOCHECK(dst2, src2, len2, uint16);
    }
  }

  /* optimized case:   copy the rest byte-wise
   * unoptimized case: copy everything byte-wise */
  /* Deviation MISRAC2012-1 */
  TS_MEMCPY_NOCHECK(dst2, src2, len2, uint8);
}

/*------------------[TS_MemSet16]--------------------------------------------*/
FUNC(void, BASE_CODE) TS_MemSet16
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  const uint8 val,
  const usize len
)
{
  /* define pointer for access */
  /* Deviation MISRAC2012-4 */
  P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA) dst2 =
    (P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA)) dst;
  usize len2 = len;

  /* check if enough data is there for optimizing */
  if (TS_MEM_IS_ENOUGH_DATA(len2))
  {
    /* check if dst is 16-bit aligned */
    /* Deviation MISRAC2012-3 */
    if (TS_MEM_IS_16ALIGNED(dst2))
    {
      /* construct a 16-bit value from the 8-bit input */
      const uint16 val2 = (uint16)val | (uint16)((uint16)val << 8U);

      /* assign data in 16-bit mode */
      /* Deviation MISRAC2012-1, MISRAC2012-2 */
      TS_MEMSET_NOCHECK(dst2, val2, len2, uint16);
    }
  }

  /* optimized case:   assign the rest byte-wise
   * unoptimized case: assign everything byte-wise */
  /* Deviation MISRAC2012-1 */
  TS_MEMSET_NOCHECK(dst2, val, len2, uint8);
}

/*------------------[TS_MemBZero16]------------------------------------------*/
FUNC(void, BASE_CODE) TS_MemBZero16
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  const usize len
)
{
  /* define pointer for access */
  /* Deviation MISRAC2012-4 */
  P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA) dst2 =
    (P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA)) dst;
  usize len2 = len;

  /* check if enough data is there for optimizing */
  if (TS_MEM_IS_ENOUGH_DATA(len2))
  {
    /* check if dst is 16-bit aligned */
    /* Deviation MISRAC2012-3 */
    if (TS_MEM_IS_16ALIGNED(dst2))
    {
      /* assign data in 16-bit mode */
      /* Deviation MISRAC2012-1, MISRAC2012-2 */
      TS_MEMSET_NOCHECK(dst2, UINT16_C(0), len2, uint16);
    }
  }

  /* optimized case:   assign the rest byte-wise
   * unoptimized case: assign everything byte-wise */
  /* Deviation MISRAC2012-1 */
  TS_MEMSET_NOCHECK(dst2, UINT8_C(0), len2, uint8);
}

/*------------------[TS_MemCmp16]--------------------------------------------*/
FUNC(Std_ReturnType, BASE_CODE) TS_MemCmp16
(
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) a,
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) b,
  const usize len
)
{
  /* define type specific pointers for suitable access */
  P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA) a2 =
    (P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA)) a;
  P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA) b2 =
    (P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA)) b;
  usize len2 = len;
  Std_ReturnType RetVal;

  /* check if enough data is there for optimizing */
  if (TS_MEM_IS_ENOUGH_DATA(len2))
  {
    /* check if a and b are both 16-bit aligned */
    /* Deviation MISRAC2012-3 */
    if (TS_MEM_IS_16ALIGNED(a2) && TS_MEM_IS_16ALIGNED(b2))
    {
      /* compare as much data in 16-bit mode */
      /* Deviation MISRAC2012-1, MISRAC2012-2 */
      TS_MEMCMP_NOCHECK(a2, b2, len2, RetVal, uint16);
    }
    else
    {
      RetVal = E_OK;
    }
  }
  else
  {
    RetVal = E_OK;
  }

  /* the optimized compare macros may have already found a
   * difference, so we have to compare again */
  if (RetVal == E_OK)
  {
    /* optimized case:   compare the rest byte-wise
     * unoptimized case: compare everything byte-wise */
    /* Deviation MISRAC2012-1 */
    TS_MEMCMP_NOCHECK(a2, b2, len2, RetVal, uint8);
  }

  return RetVal;
}

#elif (CPU_TYPE == CPU_TYPE_8)
/*------------------[8-bit CPUs]---------------------------------------------*/

/*------------------[TS_MemCpy8]---------------------------------------------*/
FUNC(void, BASE_CODE) TS_MemCpy8
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) src,
  const usize len
)
{
  /* define pointers for access */
  P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA) src2 =
    (P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA)) src;
  /* Deviation MISRAC2012-4 */
  P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA) dst2 =
    (P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA)) dst;
  usize len2 = len;

  /* copy everything byte-wise */
  /* Deviation MISRAC2012-1 */
  TS_MEMCPY_NOCHECK(dst2, src2, len2, uint8);
}

/*------------------[TS_MemSet8]---------------------------------------------*/
FUNC(void, BASE_CODE) TS_MemSet8
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  const uint8 val,
  const usize len
)
{
  /* define pointer for access */
  /* Deviation MISRAC2012-4 */
  P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA) dst2 =
    (P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA)) dst;
  usize len2 = len;

  /* assign everything byte-wise */
  /* Deviation MISRAC2012-1 */
  TS_MEMSET_NOCHECK(dst2, val, len2, uint8);
}

/*------------------[TS_MemBZero8]-------------------------------------------*/
FUNC(void, BASE_CODE) TS_MemBZero8
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  const usize len
)
{
  /* define pointer for access */
  /* Deviation MISRAC2012-4 */
  P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA) dst2 =
    (P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA)) dst;
  usize len2 = len;

  /* fill the buffer byte-wise */
  /* Deviation MISRAC2012-1 */
  TS_MEMSET_NOCHECK(dst2, UINT8_C(0), len2, uint8);
}

/*------------------[TS_MemCmp8]---------------------------------------------*/
FUNC(Std_ReturnType, BASE_CODE) TS_MemCmp8
(
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) a,
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) b,
  const usize len
)
{
  /* define type specific pointers for suitable access */
  P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA) a2 =
    (P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA)) a;
  P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA) b2 =
    (P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA)) b;
  usize len2 = len;
  Std_ReturnType RetVal;

  /* compare everything byte-wise */
  /* Deviation MISRAC2012-1 */
  TS_MEMCMP_NOCHECK(a2, b2, len2, RetVal, uint8);

  return RetVal;
}

#endif /* CPU_TYPE */

/*------------------[TS_MemMove]---------------------------------------------*/
FUNC(void, BASE_CODE) TS_MemMove
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) src,
  const usize len
)
{
  /* define uint8 pointers for byte-wise access */
  /* Deviation MISRAC2012-4 */
  P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA) dst2 =
    (P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA)) dst;
  P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA) src2 =
    (P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA)) src;
  usize len2 = len;

  /* if the start of the source block is "behind" the start of the destination
   * block, it is safe to copy 'forewards' ... */
  if (src2 > dst2)
  {
    for (len2 = len; len2 != USIZE_C(0); len2--)
    {
      *dst2 = *src2;
      dst2++;
      src2++;
    }
  }

  /* ... and if the start of the source block is "before" the start of the destination
   * block, it is safe to copy 'backwards' ... */
  else if (src2 < dst2)
  {
    /* Deviation MISRAC2012-1 <+2> */
    dst2 += len2 - USIZE_C(1);
    src2 += len2 - USIZE_C(1);

    for (len2 = len; len2 != USIZE_C(0); len2--)
    {
      *dst2 = *src2;
      dst2--;
      src2--;
    }
  }

  /* ... otherwise the pointers are equal and there is no need to copy. */
  else
  {
    /* MISRA */
  }
}

#if (defined HAS_64BIT_TYPES)
/*------------------[TS_MemCpy64_NoCheck]------------------------------------*/
FUNC(void, BASE_CODE) TS_MemCpy64_NoCheck
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) src,
  const usize len
)
{
  /* define pointers for access */
  P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA) src2 =
    (P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA)) src;
  /* Deviation MISRAC2012-4 */
  P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA) dst2 =
    (P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA)) dst;
  usize len2 = len;

  /* copy as much data in 64-bit mode */
  /* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-5  */
  TS_MEMCPY_NOCHECK(dst2, src2, len2, uint64);

  /* copy the remaining parts byte-wise */
  /* Deviation MISRAC2012-1, MISRAC2012-5 */
  TS_MEMCPY_NOCHECK(dst2, src2, len2, uint8);
}

/*------------------[TS_MemSet64_NoCheck]------------------------------------*/
FUNC(void, BASE_CODE) TS_MemSet64_NoCheck
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  const uint8 val,
  const usize len
)
{
  /* define pointer for access */
  /* Deviation MISRAC2012-4 */
  P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA) dst2 =
    (P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA)) dst;
  usize len2 = len;

  /* construct a 64-bit value from the 8-bit input */
  const uint64 val2 =  (uint64)val         | ((uint64)val << 8U)  |
                      ((uint64)val << 16U) | ((uint64)val << 24U) |
                      ((uint64)val << 32U) | ((uint64)val << 40U) |
                      ((uint64)val << 48U) | ((uint64)val << 56U);

  /* assign data in 64-bit mode */
  /* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-5 */
  TS_MEMSET_NOCHECK(dst2, val2, len2, uint64);

  /* fill the remaining parts byte-wise */
  /* Deviation MISRAC2012-1, MISRAC2012-5 */
  TS_MEMSET_NOCHECK(dst2, val, len2, uint8);
}

/*------------------[TS_MemBZero64_NoCheck]----------------------------------*/
FUNC(void, BASE_CODE) TS_MemBZero64_NoCheck
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  const usize len
)
{
  /* define pointer for access */
  /* Deviation MISRAC2012-4 */
  P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA) dst2 =
    (P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA)) dst;
  usize len2 = len;

  /* fill data in 64-bit mode */
  /* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-5 */
  TS_MEMSET_NOCHECK(dst2, UINT64_C(0), len2, uint64);

  /* fill the remaining parts byte-wise */
  /* Deviation MISRAC2012-1, MISRAC2012-5 */
  TS_MEMSET_NOCHECK(dst2, UINT8_C(0), len2, uint8);
}

/*------------------[TS_MemCmp64_NoCheck]------------------------------------*/
FUNC(Std_ReturnType, BASE_CODE) TS_MemCmp64_NoCheck
(
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) a,
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) b,
  const usize len
)
{
  /* define type specific pointers for suitable access */
  P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA) a2 =
    (P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA)) a;
  P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA) b2 =
    (P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA)) b;
  usize len2 = len;
  Std_ReturnType RetVal;

  /* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-5 */
  TS_MEMCMP_NOCHECK(a2, b2, len2, RetVal, uint64);

  /* the optimized compare macros may have already found a
   * difference, so we have to compare again */
  if (RetVal == E_OK)
  {
    /* compare the rest byte-wise */
    /* Deviation MISRAC2012-1, MISRAC2012-5 */
    TS_MEMCMP_NOCHECK(a2, b2, len2, RetVal, uint8);
  }

  return RetVal;
}
#endif /* if (defined HAS_64BIT_TYPES) */

/*------------------[TS_MemCpy32_NoCheck]------------------------------------*/
FUNC(void, BASE_CODE) TS_MemCpy32_NoCheck
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) src,
  const usize len
)
{
  /* define pointers for access */
  P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA) src2 =
    (P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA)) src;
  /* Deviation MISRAC2012-4 */
  P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA) dst2 =
    (P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA)) dst;
  usize len2 = len;

  /* copy as much data in 32-bit mode */
  /* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-5 */
  TS_MEMCPY_NOCHECK(dst2, src2, len2, uint32);

  /* copy the remaining parts byte-wise */
  /* Deviation MISRAC2012-1, MISRAC2012-5 */
  TS_MEMCPY_NOCHECK(dst2, src2, len2, uint8);
}

/*------------------[TS_MemSet32_NoCheck]------------------------------------*/

FUNC(void, BASE_CODE) TS_MemSet32_NoCheck
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  const uint8 val,
  const usize len
)
{
  /* define pointer for access */
  /* Deviation MISRAC2012-4 */
  P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA) dst2 =
    (P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA)) dst;
  usize len2 = len;

  /* construct a 32-bit value from the 8-bit input */
  const uint32 val2 =  (uint32)val         | ((uint32)val << 8U)  |
                      ((uint32)val << 16U) | ((uint32)val << 24U);

  /* assign data in 32-bit mode */
  /* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-5 */
  TS_MEMSET_NOCHECK(dst2, val2, len2, uint32);

  /* fill the remaining parts byte-wise */
  /* Deviation MISRAC2012-1, MISRAC2012-5 */
  TS_MEMSET_NOCHECK(dst2, val, len2, uint8);
}

/*------------------[TS_MemBZero32_NoCheck]----------------------------------*/
FUNC(void, BASE_CODE) TS_MemBZero32_NoCheck
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  const usize len
)
{
  /* define pointer for access */
  /* Deviation MISRAC2012-4 */
  P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA) dst2 =
    (P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA)) dst;
  usize len2 = len;

  /* fill data in 32-bit mode */
  /* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-5 */
  TS_MEMSET_NOCHECK(dst2, UINT32_C(0), len2, uint32);

  /* fill the remaining parts byte-wise */
  /* Deviation MISRAC2012-1, MISRAC2012-5 */
  TS_MEMSET_NOCHECK(dst2, UINT8_C(0), len2, uint8);
}

/*------------------[TS_MemCmp32_NoCheck]------------------------------------*/
FUNC(Std_ReturnType, BASE_CODE) TS_MemCmp32_NoCheck
(
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) a,
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) b,
  const usize len
)
{
  /* define type specific pointers for suitable access */
  P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA) a2 =
    (P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA)) a;
  P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA) b2 =
    (P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA)) b;
  usize len2 = len;
  Std_ReturnType RetVal;

  /* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-5 */
  TS_MEMCMP_NOCHECK(a2, b2, len2, RetVal, uint32);

  /* the optimized compare macros may have already found a
   * difference, so we have to compare again */
  if (RetVal == E_OK)
  {
    /* compare the rest byte-wise */
    /* Deviation MISRAC2012-1, MISRAC2012-5 */
    TS_MEMCMP_NOCHECK(a2, b2, len2, RetVal, uint8);
  }

  return RetVal;
}

/*------------------[TS_MemCpy16_NoCheck]------------------------------------*/
FUNC(void, BASE_CODE) TS_MemCpy16_NoCheck
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) src,
  const usize len
)
{
  /* define pointers for access */
  P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA) src2 =
    (P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA)) src;
  /* Deviation MISRAC2012-4 */
  P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA) dst2 =
    (P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA)) dst;
  usize len2 = len;

  /* copy as much as possible in 16-bit mode */
  /* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-5 */
  TS_MEMCPY_NOCHECK(dst2, src2, len2, uint16);

  /* copy the rest byte-wise */
  /* Deviation MISRAC2012-1, MISRAC2012-5 */
  TS_MEMCPY_NOCHECK(dst2, src2, len2, uint8);
}

/*------------------[TS_MemSet16_NoCheck]------------------------------------*/
FUNC(void, BASE_CODE) TS_MemSet16_NoCheck
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  const uint8 val,
  const usize len
)
{
  /* define pointer for access */
  /* Deviation MISRAC2012-4 */
  P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA) dst2 =
    (P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA)) dst;
  usize len2 = len;

  /* construct a 16-bit value from the 8-bit input */
  const uint16 val2 = (uint16)val | (uint16)((uint16)val << 8U);

  /* assign data in 16-bit mode */
  /* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-5 */
  TS_MEMSET_NOCHECK(dst2, val2, len2, uint16);

  /* fill the rest byte-wise */
  /* Deviation MISRAC2012-1, MISRAC2012-5 */
  TS_MEMSET_NOCHECK(dst2, val, len2, uint8);
}

/*------------------[TS_MemBZero16_NoCheck]----------------------------------*/
FUNC(void, BASE_CODE) TS_MemBZero16_NoCheck
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  const usize len
)
{
  /* define pointer for access */
  /* Deviation MISRAC2012-4 */
  P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA) dst2 =
    (P2VAR(uint8, AUTOMATIC, BASE_APPL_DATA)) dst;
  usize len2 = len;

  /* assign data in 16-bit mode */
  /* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-5 */
  TS_MEMSET_NOCHECK(dst2, UINT16_C(0), len2, uint16);

  /* fill the rest byte-wise */
  /* Deviation MISRAC2012-1, MISRAC2012-5 */
  TS_MEMSET_NOCHECK(dst2, UINT8_C(0), len2, uint8);
}

/*------------------[TS_MemCmp16_NoCheck]------------------------------------*/
FUNC(Std_ReturnType, BASE_CODE) TS_MemCmp16_NoCheck
(
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) a,
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) b,
  const usize len
)
{
  /* define type specific pointers for suitable access */
  P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA) a2 =
    (P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA)) a;
  P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA) b2 =
    (P2CONST(uint8, AUTOMATIC, BASE_APPL_DATA)) b;
  usize len2 = len;
  Std_ReturnType RetVal;

  /* compare as much data in 16-bit mode as possible */
  /* Deviation MISRAC2012-1, MISRAC2012-2, MISRAC2012-5 */
  TS_MEMCMP_NOCHECK(a2, b2, len2, RetVal, uint16);

  /* the optimized compare macros may have already found a
   * difference, so we have to compare again */
  if (RetVal == E_OK)
  {
    /* compare the rest byte-wise */
    /* Deviation MISRAC2012-1, MISRAC2012-5 */
    TS_MEMCMP_NOCHECK(a2, b2, len2, RetVal, uint8);
  }

  return RetVal;
}

/*==================[internal function definitions]==========================*/

#define BASE_STOP_SEC_CODE
#include <Base_MemMap.h>

/*==================[end of file]============================================*/
