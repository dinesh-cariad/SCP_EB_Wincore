#ifndef TS_MEM_H
#define TS_MEM_H

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

/*==================[inclusions]=============================================*/

#include <Std_Types.h>

/*==================[macros]=================================================*/

/* !LINKSTO Base.Properties.OverrideMemCpy,1 */
/* prevent double definition of macro for memory copy function only if not overridden */
#if ((defined TS_MemCpy) && (TS_MEMCPY_CUSTOM_OVERRIDE == STD_OFF))
#error TS_MemCpy is already defined
/* check for definition of macro for memory copy function only if overridden */
#elif ((!defined TS_MemCpy) && (TS_MEMCPY_CUSTOM_OVERRIDE == STD_ON))
#error Configuration parameter CustomOverride_MemCpy is set but macro TS_MemCpy is not defined
#endif

/* !LINKSTO Base.Properties.OverrideMemSet,1 */
/* prevent double definition of macro for memory set function only if not overridden */
#if ((defined TS_MemSet) && (TS_MEMSET_CUSTOM_OVERRIDE == STD_OFF))
#error TS_MemSet is already defined
/* check for definition of macro for memory set function only if overridden */
#elif ((!defined TS_MemSet) && (TS_MEMSET_CUSTOM_OVERRIDE == STD_ON))
#error Configuration parameter CustomOverride_MemSet is set but macro TS_MemSet is not defined
#endif

/* !LINKSTO Base.Properties.OverrideMemCmp,1 */
/* prevent double definition of macro for memory compare function only if not overridden */
#if ((defined TS_MemCmp) && (TS_MEMCMP_CUSTOM_OVERRIDE == STD_OFF))
#error TS_MemCmp is already defined
/* check for definition of macro for memory compare function only if overridden */
#elif ((!defined TS_MemCmp) && (TS_MEMCMP_CUSTOM_OVERRIDE == STD_ON))
#error Configuration parameter CustomOverride_MemCmp is set but macro TS_MemCmp is not defined
#endif

/* !LINKSTO Base.Properties.OverrideMemBZero,1 */
/* prevent double definition of macro for memory zeroing function only if not overridden */
#if ((defined TS_MemBZero) && (TS_MEMBZERO_CUSTOM_OVERRIDE == STD_OFF))
#error TS_MemBZero is already defined
/* check for definition of macro for memory zeroing function only if overridden */
#elif ((!defined TS_MemBZero) && (TS_MEMBZERO_CUSTOM_OVERRIDE == STD_ON))
#error Configuration parameter CustomOverride_MemBZero is set but macro TS_MemBZero is not defined
#endif

#if (TS_MEMCPY_CUSTOM_OVERRIDE == STD_OFF)
#if (CPU_TYPE == CPU_TYPE_64)

/** \brief This macro maps to the CPU specific macro for copying memory bytes, for 64bit CPU to TS_MemCpy64() */
#define TS_MemCpy(d,s,n) (TS_MemCpy64((d),(s),(n)))

#elif (CPU_TYPE == CPU_TYPE_32)

/** \brief This macro maps to the CPU specific macro for copying memory bytes, for 32bit CPU to TS_MemCpy32() */
#define TS_MemCpy(d,s,n) (TS_MemCpy32((d),(s),(n)))

#elif (CPU_TYPE == CPU_TYPE_16)

/** \brief This macro maps to the CPU specific macro for copying memory bytes, for 16bit CPU to TS_MemCpy16() */
#define TS_MemCpy(d,s,n) (TS_MemCpy16((d),(s),(n)))

#elif (CPU_TYPE == CPU_TYPE_8)

/** \brief This macro maps to the CPU specific macro for copying memory bytes, for 8bit CPU to TS_MemCpy8() */
#define TS_MemCpy(d,s,n) (TS_MemCpy8((d),(s),(n)))

#endif /* CPU_TYPE */
#endif /* TS_MEMCPY_CUSTOM_OVERRIDE == STD_OFF */

#if (TS_MEMSET_CUSTOM_OVERRIDE == STD_OFF)
#if (CPU_TYPE == CPU_TYPE_64)

/** \brief This macro maps to the CPU specific macro for setting memory bytes, for 64bit CPU to TS_MemSet64() */
#define TS_MemSet(dst,val,len) (TS_MemSet64((dst),(val),(len)))

#elif (CPU_TYPE == CPU_TYPE_32)

/** \brief This macro maps to the CPU specific macro for setting memory bytes, for 32bit CPU to TS_MemSet32() */
#define TS_MemSet(dst,val,len) (TS_MemSet32((dst),(val),(len)))

#elif (CPU_TYPE == CPU_TYPE_16)

/** \brief This macro maps to the CPU specific macro for setting memory bytes, for 16bit CPU to TS_MemSet16() */
#define TS_MemSet(dst,val,len) (TS_MemSet16((dst),(val),(len)))

#elif (CPU_TYPE == CPU_TYPE_8)

/** \brief This macro maps to the CPU specific macro for setting memory bytes, for 8bit CPU to TS_MemSet8() */
#define TS_MemSet(dst,val,len) (TS_MemSet8((dst),(val),(len)))

#endif /* CPU_TYPE */
#endif /* TS_MEMSET_CUSTOM_OVERRIDE == STD_OFF */

#if (TS_MEMCMP_CUSTOM_OVERRIDE == STD_OFF)
#if (CPU_TYPE == CPU_TYPE_64)

/** \brief This macro maps to the CPU specific macro for comparing memory bytes, for 64bit CPU to TS_MemCmp64() */
#define TS_MemCmp(a,b,n) (TS_MemCmp64((a),(b),(n)))

#elif (CPU_TYPE == CPU_TYPE_32)

/** \brief This macro maps to the CPU specific macro for comparing memory bytes, for 32bit CPU to TS_MemCmp32() */
#define TS_MemCmp(a,b,n) (TS_MemCmp32((a),(b),(n)))

#elif (CPU_TYPE == CPU_TYPE_16)

/** \brief This macro maps to the CPU specific macro for comparing memory bytes, for 16bit CPU to TS_MemCmp16() */
#define TS_MemCmp(a,b,n) (TS_MemCmp16((a),(b),(n)))

#elif (CPU_TYPE == CPU_TYPE_8)

/** \brief This macro maps to the CPU specific macro for comparing memory bytes, for 8bit CPU to TS_MemCmp8() */
#define TS_MemCmp(a,b,n) (TS_MemCmp8((a),(b),(n)))

#endif /* CPU_TYPE */
#endif /* TS_MEMCMP_CUSTOM_OVERRIDE == STD_OFF */

#if (TS_MEMBZERO_CUSTOM_OVERRIDE == STD_OFF)
#if (CPU_TYPE == CPU_TYPE_64)

/** \brief This macro maps to the CPU specific macro for filling memory bytes with zero, for 64bit CPU to TS_MemBZero64() */
#define TS_MemBZero(dst, len) (TS_MemBZero64((dst), (len)))

#elif (CPU_TYPE == CPU_TYPE_32)

/** \brief This macro maps to the CPU specific macro for filling memory bytes with zero, for 32bit CPU to TS_MemBZero32() */
#define TS_MemBZero(dst, len) (TS_MemBZero32((dst), (len)))

#elif (CPU_TYPE == CPU_TYPE_16)

/** \brief This macro maps to the CPU specific macro for filling memory bytes with zero, for 16bit CPU to TS_MemBZero16() */
#define TS_MemBZero(dst, len) (TS_MemBZero16((dst), (len)))

#elif (CPU_TYPE == CPU_TYPE_8)

/** \brief This macro maps to the CPU specific macro for filling memory bytes with zero, for 8bit CPU to TS_MemBZero8() */
#define TS_MemBZero(dst, len) (TS_MemBZero8((dst), (len)))

#endif /* CPU_TYPE */
#endif /* TS_MEMBZERO_CUSTOM_OVERRIDE == STD_OFF */

/*==================[external function definitions]==========================*/

#define BASE_START_SEC_CODE
#include <Base_MemMap.h>

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
extern FUNC(void, BASE_CODE) TS_MemCpy64
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) src,
  const usize len
);

/** \brief Assigns \p len copies of value \p val to destination \p dst
 *
 * \pre The pointer \p dst points to a valid address
 *
 * \param[in] dst Pointer to assign the value to
 * \param[in] val Value to be assigned
 * \param[in] len Amount of copies to assign
 */
extern FUNC(void, BASE_CODE) TS_MemSet64
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  const uint8 val,
  const usize len
);

/** \brief Fills \p len bytes with zero at destination \p dst
 *
 * \pre The pointer \p dst points to a valid address
 *
 * \param[in] dst Pointer to the destination to fill
 * \param[in] len Amount of bytes to fill
 */
extern FUNC(void, BASE_CODE) TS_MemBZero64
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  const usize len
);

/** \brief Compares \p n bytes of memory at locations \p a and \p b
 * (64-bit version)
 *
 * This function compares a memory block of length \p len located
 * at address \p a to a memory block located at address \p b.
 *
 * \pre The pointer \p a points to a valid address
 * \pre The pointer \p b points to a valid address
 *
 * \param[in] a Pointer to the first memory block to compare
 * \param[in] b Pointer to the second memory block to compare
 * \param[in] len Amount of bytes to compare
 *
 * \return Comparison status
 * \retval E_OK Both memory blocks compare equal
 * \retval E_NOT_OK The memory blocks are not equal
 */
extern FUNC(Std_ReturnType, BASE_CODE) TS_MemCmp64
(
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) a,
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) b,
  const usize len
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
extern FUNC(void, BASE_CODE) TS_MemCpy32
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) src,
  const usize len
);

/** \brief Assigns \p len copies of value \p val to destination \p dst
 *
 * \pre The pointer \p dst points to a valid address
 *
 * \param[in] dst Pointer to assign the value to
 * \param[in] val Value to be assigned
 * \param[in] len Amount of copies to assign
 */
extern FUNC(void, BASE_CODE) TS_MemSet32
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  const uint8 val,
  const usize len
);

/** \brief Fills \p len bytes with zero at destination \p dst
 *
 * \pre The pointer \p dst points to a valid address
 *
 * \param[in] dst Pointer to the destination to fill
 * \param[in] len Amount of bytes to fill
 */
extern FUNC(void, BASE_CODE) TS_MemBZero32
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  const usize len
);

/** \brief Compares \p n bytes of memory at locations \p a and \p b
 * (32-bit version)
 *
 * This function compares a memory block of length \p len located
 * at address \p a to a memory block located at address \p b.
 *
 * \pre The pointer \p a points to a valid address
 * \pre The pointer \p b points to a valid address
 *
 * \param[in] a Pointer to the first memory block to compare
 * \param[in] b Pointer to the second memory block to compare
 * \param[in] len Amount of bytes to compare
 *
 * \return Comparison status
 * \retval E_OK Both memory blocks compare equal
 * \retval E_NOT_OK The memory blocks are not equal
 */
extern FUNC(Std_ReturnType, BASE_CODE) TS_MemCmp32
(
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) a,
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) b,
  const usize len
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
extern FUNC(void, BASE_CODE) TS_MemCpy16
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) src,
  const usize len
);

/** \brief Assigns \p len copies of value \p val to destination \p dst
 *
 * \pre The pointer \p dst points to a valid address
 *
 * \param[in] dst Pointer to assign the value to
 * \param[in] val Value to be assigned
 * \param[in] len Amount of copies to assign
 */
extern FUNC(void, BASE_CODE) TS_MemSet16
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  const uint8 val,
  const usize len
);

/** \brief Fills \p len bytes with zero at destination \p dst
 *
 * \pre The pointer \p dst points to a valid address
 *
 * \param[in] dst Pointer to the destination to fill
 * \param[in] len Amount of bytes to fill
 */
extern FUNC(void, BASE_CODE) TS_MemBZero16
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  const usize len
);

/** \brief Compares \p n bytes of memory at locations \p a and \p b
 * (16-bit version)
 *
 * This function compares a memory block of length \p len located
 * at address \p a to a memory block located at address \p b.
 *
 * \pre The pointer \p a points to a valid address
 * \pre The pointer \p b points to a valid address
 *
 * \param[in] a Pointer to the first memory block to compare
 * \param[in] b Pointer to the second memory block to compare
 * \param[in] len Amount of bytes to compare
 *
 * \return Comparison status
 * \retval E_OK Both memory blocks compare equal
 * \retval E_NOT_OK The memory blocks are not equal
 */
extern FUNC(Std_ReturnType, BASE_CODE) TS_MemCmp16
(
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) a,
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) b,
  const usize len
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
extern FUNC(void, BASE_CODE) TS_MemCpy8
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) src,
  const usize len
);

/** \brief Assigns \p len copies of value \p val to destination \p dst
 *
 * \pre The pointer \p dst points to a valid address
 *
 * \param[in] dst Pointer to assign the value to
 * \param[in] val Value to be assigned
 * \param[in] len Amount of copies to assign
 */
extern FUNC(void, BASE_CODE) TS_MemSet8
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  const uint8 val,
  const usize len
);

/** \brief Fills \p len bytes with zero at destination \p dst
 *
 * \pre The pointer \p dst points to a valid address
 *
 * \param[in] dst Pointer to the destination to fill
 * \param[in] len Amount of bytes to fill
 */
extern FUNC(void, BASE_CODE) TS_MemBZero8
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  const usize len
);

/** \brief Compares \p n bytes of memory at locations \p a and \p b
 * (generic version)
 *
 * This function compares a memory block of length \p len located
 * at address \p a to a memory block located at address \p b.
 *
 * \pre The pointer \p a points to a valid address
 * \pre The pointer \p b points to a valid address
 *
 * \param[in] a Pointer to the first memory block to compare
 * \param[in] b Pointer to the second memory block to compare
 * \param[in] len Amount of bytes to compare
 *
 * \return Comparison status
 * \retval E_OK Both memory blocks compare equal
 * \retval E_NOT_OK The memory blocks are not equal
 */
extern FUNC(Std_ReturnType, BASE_CODE) TS_MemCmp8
(
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) a,
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) b,
  const usize len
);

#endif /* CPU_TYPE */

/** \brief Moves \p len bytes of memory from location pointed to by \p src to
 * the location pointed to by \p dst.
 *
 * \remark The function takes overlapping memory fully into account and can be called
 * safely with all combinations of addresses.
 *
 * \remark The function is optimized to not copy anything if the pointers are equal.
 * Don't rely on this function accessing e.g. volatile data when being called with
 * equal pointers.
 *
 * \pre The pointer \p dst points to a valid address
 * \pre The pointer \p src points to a valid address
 *
 * \param[in] dst Pointer to the destination memory block
 * \param[in] src Pointer to the source memory block
 * \param[in] len Amount of bytes to copy
 */
extern FUNC(void, BASE_CODE) TS_MemMove
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) src,
  const usize len
);

#if (defined HAS_64BIT_TYPES)
/** \brief Copies \p len bytes of memory from \p dst to \p src
 * without overlapping or alignment checks (64-bit version)
 *
 * This function copies a memory block of length \p len
 * from the source \p src to the destination \p dst.
 *
 * \pre The pointer \p dst points to a valid address and is 64-bit aligned
 * \pre The pointer \p src points to a valid address and is 64-bit aligned
 *
 * \note The function does not perform overlapping checks
 * \note The function does not perform alignment checks
 *
 * \param[in] dst Pointer to the destination address
 * \param[in] src Pointer to the source address
 * \param[in] len Amount of bytes to copy
 */
extern FUNC(void, BASE_CODE) TS_MemCpy64_NoCheck
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) src,
  const usize len
);

/** \brief Assigns \p len copies of value \p val to destination \p dst
 * without alignment checks (64-bit version)
 *
 * \pre The pointer \p dst points to a valid address and is 64-bit aligned
 *
 * \note The function does not perform alignment checks
 *
 * \param[in] dst Pointer to assign the value to
 * \param[in] val Value to be assigned
 * \param[in] len Amount of copies to assign
 */
extern FUNC(void, BASE_CODE) TS_MemSet64_NoCheck
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  const uint8 val,
  const usize len
);

/** \brief Fills \p len bytes with zero at destination \p dst
 * without alignment checks (64-bit version)
 *
 * \pre The pointer \p dst points to a valid address and is 64-bit aligned
 *
 * \note The function does not perform alignment checks
 *
 * \param[in] dst Pointer to the destination to fill
 * \param[in] len Amount of bytes to fill
 */
extern FUNC(void, BASE_CODE) TS_MemBZero64_NoCheck
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  const usize len
);

/** \brief Compares \p n bytes of memory at locations \p a and \p b
 * without alignment checks (64-bit version)
 *
 * This function compares a memory block of length \p len located
 * at address \p a to a memory block located at address \p b.
 *
 * \pre The pointer \p a points to a valid address and is 64-bit aligned
 * \pre The pointer \p b points to a valid address and is 64-bit aligned
 *
 * \note The function does not perform alignment checks
 *
 * \param[in] a Pointer to the first memory block to compare
 * \param[in] b Pointer to the second memory block to compare
 * \param[in] len Amount of bytes to compare
 *
 * \return Comparison status
 * \retval E_OK Both memory blocks compare equal
 * \retval E_NOT_OK The memory blocks are not equal
 */
extern FUNC(Std_ReturnType, BASE_CODE) TS_MemCmp64_NoCheck
(
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) a,
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) b,
  const usize len
);
#endif /* if (defined HAS_64BIT_TYPES) */

/** \brief Copies \p len bytes of memory from \p dst to \p src
 * without overlapping or alignment checks (32-bit version)
 *
 * This function copies a memory block of length \p len
 * from the source \p src to the destination \p dst.
 *
 * \pre The pointer \p dst points to a valid address and is 32-bit aligned
 * \pre The pointer \p src points to a valid address and is 32-bit aligned
 *
 * \note The function does not perform overlapping checks
 * \note The function does not perform alignment checks
 *
 * \param[in] dst Pointer to the destination address
 * \param[in] src Pointer to the source address
 * \param[in] len Amount of bytes to copy
 */
extern FUNC(void, BASE_CODE) TS_MemCpy32_NoCheck
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) src,
  const usize len
);

/** \brief Assigns \p len copies of value \p val to destination \p dst
 * without alignment checks (32-bit version)
 *
 * \pre The pointer \p dst points to a valid address and is 32-bit aligned
 *
 * \note The function does not perform alignment checks
 *
 * \param[in] dst Pointer to assign the value to
 * \param[in] val Value to be assigned
 * \param[in] len Amount of copies to assign
 */
extern FUNC(void, BASE_CODE) TS_MemSet32_NoCheck
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  const uint8 val,
  const usize len
);

/** \brief Fills \p len bytes with zero at destination \p dst
 *
 * \pre The pointer \p dst points to a valid address and is 32-bit aligned
 *
 * \note The function does not perform alignment checks
 *
 * \param[in] dst Pointer to the destination to fill
 * \param[in] len Amount of bytes to fill
 */
extern FUNC(void, BASE_CODE) TS_MemBZero32_NoCheck
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  const usize len
);

/** \brief Compares \p n bytes of memory at locations \p a and \p b
 * without alignment checks (32-bit version)
 *
 * This function compares a memory block of length \p len located
 * at address \p a to a memory block located at address \p b.
 *
 * \pre The pointer \p a points to a valid address and is 32-bit aligned
 * \pre The pointer \p b points to a valid address and is 32-bit aligned
 *
 * \note The function does not perform alignment checks
 *
 * \param[in] a Pointer to the first memory block to compare
 * \param[in] b Pointer to the second memory block to compare
 * \param[in] len Amount of bytes to compare
 *
 * \return Comparison status
 * \retval E_OK Both memory blocks compare equal
 * \retval E_NOT_OK The memory blocks are not equal
 */
extern FUNC(Std_ReturnType, BASE_CODE) TS_MemCmp32_NoCheck
(
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) a,
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) b,
  const usize len
);

/** \brief Copies \p len bytes of memory from \p dst to \p src
 * without overlapping or alignment checks (16-bit version)
 *
 * This function copies a memory block of length \p len
 * from the source \p src to the destination \p dst.
 *
 * \pre The pointer \p dst points to a valid address and is 16-bit aligned
 * \pre The pointer \p src points to a valid address and is 16-bit aligned
 *
 * \note The function does not perform overlapping checks
 * \note The function does not perform alignment checks
 *
 * \param[in] dst Pointer to the destination address
 * \param[in] src Pointer to the source address
 * \param[in] len Amount of bytes to copy
 */
extern FUNC(void, BASE_CODE) TS_MemCpy16_NoCheck
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) src,
  const usize len
);

/** \brief Assigns \p len copies of value \p val to destination \p dst
 * without alignment checks (16-bit version)
 *
 * \pre The pointer \p dst points to a valid address and is 16-bit aligned
 *
 * \note The function does not perform alignment checks
 *
 * \param[in] dst Pointer to assign the value to
 * \param[in] val Value to be assigned
 * \param[in] len Amount of copies to assign
 */
extern FUNC(void, BASE_CODE) TS_MemSet16_NoCheck
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  const uint8 val,
  const usize len
);

/** \brief Fills \p len bytes with zero at destination \p dst
 * without alignment checks (16-bit version)
 *
 * \pre The pointer \p dst points to a valid address and is 16-bit aligned
 *
 * \note The function does not perform alignment checks
 *
 * \param[in] dst Pointer to the destination to fill
 * \param[in] len Amount of bytes to fill
 */
extern FUNC(void, BASE_CODE) TS_MemBZero16_NoCheck
(
  CONSTP2VAR(void, AUTOMATIC, BASE_APPL_DATA) dst,
  const usize len
);

/** \brief Compares \p n bytes of memory at locations \p a and \p b
 * without alignment checks (16-bit version)
 *
 * This function compares a memory block of length \p len located
 * at address \p a to a memory block located at address \p b.
 *
 * \pre The pointer \p a points to a valid address and is 16-bit aligned
 * \pre The pointer \p b points to a valid address and is 16-bit aligned
 *
 * \note The function does not perform alignment checks
 *
 * \param[in] a Pointer to the first memory block to compare
 * \param[in] b Pointer to the second memory block to compare
 * \param[in] len Amount of bytes to compare
 *
 * \return Comparison status
 * \retval E_OK Both memory blocks compare equal
 * \retval E_NOT_OK The memory blocks are not equal
 */
extern FUNC(Std_ReturnType, BASE_CODE) TS_MemCmp16_NoCheck
(
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) a,
  CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) b,
  const usize len
);

#define BASE_STOP_SEC_CODE
#include <Base_MemMap.h>

/*==================[internal function definitions]==========================*/

#endif /* ifndef TS_MEM_H */
/*==================[end of file]============================================*/
