#ifndef TS_PBCONFIG_SIGNATURE_H
#define TS_PBCONFIG_SIGNATURE_H

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

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 20.11 (required)
 * There shall be at most one occurrence of the # or ## preprocessor
 * operators in a single macro definition.
 *
 * Reason:
 * The reason for this rule is that the order of evaluation these operators
 * is undefined. The macros in question don't depend on the order of evaluation.
 *
 * MISRAC2012-2) Deviated Rule: 20.10 (advisory)
 * The # and ## preprocessor operators should not be used.
 *
 * Reason:
 * The rationale for this rule is that the evaluation order of the
 * # and ## operator is not defined. - Since only one instance of the ##
 * operator is used, there is no ambiguity due to the evaluation order
 */

/*==================[inclusions]=============================================*/

#include <TSAutosar.h>

#include <TSPBConfig_OffsetAlignment.h>

/*==================[macros]=================================================*/

/*
 * macros for the extraction of relevant information from the post-build
 * platform signature for consistency of endianness and alignment requirements
 */

/** \brief macro for mask for extraction of 32bit signed integer alignment requirements */
#define TS_SIG_SINT32_ALIGNMENT_MASK UINT32_C(0x00000007)
/** \brief macro for shift for extraction of 32bit signed integer alignment requirements */
#define TS_SIG_SINT32_ALIGNMENT_SHIFT UINT32_C(0)
/** \brief macro for mask for extraction of 32bit unsigned integer alignment requirements */
#define TS_SIG_UINT32_ALIGNMENT_MASK UINT32_C(0x00000007)
/** \brief macro for shift for extraction of 32bit unsigned integer alignment requirements */
#define TS_SIG_UINT32_ALIGNMENT_SHIFT UINT32_C(3)
/** \brief macro for mask for extraction of 16bit signed integer alignment requirements */
#define TS_SIG_SINT16_ALIGNMENT_MASK UINT32_C(0x00000007)
/** \brief macro for shift for extraction of 16bit signed integer alignment requirements */
#define TS_SIG_SINT16_ALIGNMENT_SHIFT UINT32_C(8)
/** \brief macro for mask for extraction of 16bit unsigned integer alignment requirements */
#define TS_SIG_UINT16_ALIGNMENT_MASK UINT32_C(0x00000007)
/** \brief macro for shift for extraction of 16bit unsigned integer alignment requirements */
#define TS_SIG_UINT16_ALIGNMENT_SHIFT UINT32_C(11)
/** \brief macro for mask for extraction of 8bit signed integer alignment requirements */
#define TS_SIG_SINT8_ALIGNMENT_MASK UINT32_C(0x00000007)
/** \brief macro for shift for extraction of 8bit signed integer alignment requirements */
#define TS_SIG_SINT8_ALIGNMENT_SHIFT UINT32_C(16)
/** \brief macro for mask for extraction of 8bit unsigned integer alignment requirements */
#define TS_SIG_UINT8_ALIGNMENT_MASK UINT32_C(0x00000007)
/** \brief macro for shift for extraction of 8bit unsigned integer alignment requirements */
#define TS_SIG_UINT8_ALIGNMENT_SHIFT UINT32_C(19)
/** \brief macro for mask for extraction of array alignment requirements */
#define TS_SIG_ARRAY_ALIGNMENT_MASK UINT32_C(0x00000007)
/** \brief macro for shift for extraction of array alignment requirements */
#define TS_SIG_ARRAY_ALIGNMENT_SHIFT UINT32_C(24)
/** \brief macro for mask for extraction of struct alignment requirements */
#define TS_SIG_STRUCT_ALIGNMENT_MASK UINT32_C(0x00000007)
/** \brief macro for shift for extraction of struct alignment requirements */
#define TS_SIG_STRUCT_ALIGNMENT_SHIFT UINT32_C(27)

#if (defined TS_SIG_GET_ALIGNMENT_HLP_HLP) /* to prevent double definition */
#error TS_SIG_GET_ALIGNMENT_HLP_HLP is already defined
#endif /* if (defined TS_SIG_GET_ALIGNMENT_HLP_HLP) */

/** \brief helper macro for extraction of alignment requirements */
/* Deviation MISRAC2012-1, MISRAC2012-2 <+2> */
#define TS_SIG_GET_ALIGNMENT_HLP_HLP(prefixedtype, signature) \
  (((signature) >> prefixedtype ## _ALIGNMENT_SHIFT) & (uint32)prefixedtype ## _ALIGNMENT_MASK)


#if (defined TS_SIG_GET_ALIGNMENT_HLP) /* to prevent double definition */
#error TS_SIG_GET_ALIGNMENT_HLP is already defined
#endif /* if (defined TS_SIG_GET_ALIGNMENT_HLP) */

/** \brief helper macro for extraction of alignment requirements */
/* Deviation MISRAC2012-2 <+2> */
#define TS_SIG_GET_ALIGNMENT_HLP(type, signature) \
  TS_SIG_GET_ALIGNMENT_HLP_HLP(TS_SIG_ ## type, signature)


#if (defined TS_SIG_GET_ALIGNMENT) /* to prevent double definition */
#error TS_SIG_GET_ALIGNMENT is already defined
#endif /* if (defined TS_SIG_GET_ALIGNMENT) */

/** \brief macro to extract the alignment requirements for a specific type from the signature */
#define TS_SIG_GET_ALIGNMENT(type, signature) \
  ((uint32)(UINT32_C(1) << TS_SIG_GET_ALIGNMENT_HLP(type, signature)))


#if (defined TS_SIG_CHECK_ALIGNMENT_UINT8) /* to prevent double definition */
#error TS_SIG_CHECK_ALIGNMENT_UINT8 is already defined
#endif /* if (defined TS_SIG_CHECK_ALIGNMENT_UINT8) */

/** \brief macro to check whether the alignment requirements of uint8 that were
 * assumed when generating the post build configuration match the machines
 * alignment requirements for uint8
 */
#define TS_SIG_CHECK_ALIGNMENT_UINT8(signature) \
  (TS_SIG_GET_ALIGNMENT(UINT8, signature) == TS_ALIGNOF(uint8))


#if (defined TS_SIG_CHECK_ALIGNMENT_UINT16) /* to prevent double definition */
#error TS_SIG_CHECK_ALIGNMENT_UINT16 is already defined
#endif /* if (defined TS_SIG_CHECK_ALIGNMENT_UINT16) */

/** \brief macro to check whether the alignment requirements of uint16 that were
 * assumed when generating the post build configuration match the machines
 * alignment requirements for uint16
 */
#define TS_SIG_CHECK_ALIGNMENT_UINT16(signature) \
  (TS_SIG_GET_ALIGNMENT(UINT16, signature) == TS_ALIGNOF(uint16))

#if (defined TS_SIG_CHECK_ALIGNMENT_UINT32) /* to prevent double definition */
#error TS_SIG_CHECK_ALIGNMENT_UINT32 is already defined
#endif /* if (defined TS_SIG_CHECK_ALIGNMENT_UINT32) */

/** \brief macro to check whether the alignment requirements of uint32 that were
 * assumed when generating the post build configuration match the machines
 * alignment requirements for uint32
 */
#define TS_SIG_CHECK_ALIGNMENT_UINT32(signature) \
  (TS_SIG_GET_ALIGNMENT(UINT32, signature) == TS_ALIGNOF(uint32))

#if (defined TS_SIG_CHECK_ALIGNMENT_SINT8) /* to prevent double definition */
#error TS_SIG_CHECK_ALIGNMENT_SINT8 is already defined
#endif /* if (defined TS_SIG_CHECK_ALIGNMENT_SINT8) */

/** \brief macro to check whether the alignment requirements of sint8 that were
 * assumed when generating the post build configuration match the machines
 * alignment requirements for sint8
 */
#define TS_SIG_CHECK_ALIGNMENT_SINT8(signature) \
  (TS_SIG_GET_ALIGNMENT(SINT8, signature) == TS_ALIGNOF(sint8))


#if (defined TS_SIG_CHECK_ALIGNMENT_SINT16) /* to prevent double definition */
#error TS_SIG_CHECK_ALIGNMENT_SINT16 is already defined
#endif /* if (defined TS_SIG_CHECK_ALIGNMENT_SINT16) */

/** \brief macro to check whether the alignment requirements of sint16 that were
 * assumed when generating the post build configuration match the machines
 * alignment requirements for sint16
 */
#define TS_SIG_CHECK_ALIGNMENT_SINT16(signature) \
  (TS_SIG_GET_ALIGNMENT(SINT16, signature) == TS_ALIGNOF(sint16))


#if (defined TS_SIG_CHECK_ALIGNMENT_SINT32) /* to prevent double definition */
#error TS_SIG_CHECK_ALIGNMENT_SINT32 is already defined
#endif /* if (defined TS_SIG_CHECK_ALIGNMENT_SINT32) */

/** \brief macro to check whether the alignment requirements of sint32 that were
 * assumed when generating the post build configuration match the machines
 * alignment requirements for sint32
 */
#define TS_SIG_CHECK_ALIGNMENT_SINT32(signature) \
  (TS_SIG_GET_ALIGNMENT(SINT32, signature) == TS_ALIGNOF(sint32))


#if (defined TS_SIG_CHECK_ALIGNMENT_STRUCT) /* to prevent double definition */
#error TS_SIG_CHECK_ALIGNMENT_STRUCT is already defined
#endif /* if (defined TS_SIG_CHECK_ALIGNMENT_STRUCT) */

/** \brief macro to check whether the minimal alignment requirements of structures that were
 * assumed when generating the post build configuration match the machines
 * minimal alignment requirements for structures.
 *
 * Using two uint8 members here to address the fact that the TriCore EABI states
 * that the minimal aligment requirement of structs of 1 byte size is only 1 byte
 * (instead of 2 bytes minimal alignment like all struct of size > 1 byte)
 */
#define TS_SIG_CHECK_ALIGNMENT_STRUCT(signature) \
  (TS_SIG_GET_ALIGNMENT(STRUCT, signature) == TS_ALIGNOF(struct {uint8 nMember1; uint8 nMember2; \
                                                                 uint8 nMember3; uint8 nMember4; \
                                                                 uint8 nMember5; uint8 nMember6; \
                                                                 uint8 nMember7; uint8 nMember8; }))


#if (defined TS_SIG_CHECK_ALIGNMENT_ARRAY) /* to prevent double definition */
#error TS_SIG_CHECK_ALIGNMENT_ARRAY is already defined
#endif /* if (defined TS_SIG_CHECK_ALIGNMENT_ARRAY) */

/** \brief macro to check whether the minimal alignment requirements of arrays that were
 * assumed when generating the post build configuration match the machines
 * minimal alignment requirements for arrays
 */
#define TS_SIG_CHECK_ALIGNMENT_ARRAY(signature) \
  (TS_SIG_GET_ALIGNMENT(ARRAY, signature) == TS_ARRAYALIGNOF(uint8, 1))


#if (defined TS_SIG_CHECK_ENDIANESS) /* to prevent double definition */
#error TS_SIG_CHECK_ENDIANESS is already defined
#endif /* if (defined TS_SIG_CHECK_ENDIANESS) */

/** \brief macro to check whether the endianness that was assumed when generating
 * the post build configuration match the machines endianness
 */
#define TS_SIG_CHECK_ENDIANESS(signature) \
  (((signature) & UINT32_C(0x80808080)) == UINT32_C(0x80000000))

/*==================[external function declarations]=========================*/

#define BASE_START_SEC_CODE
#include <Base_MemMap.h>

/** \brief Checks whether the platform signature \p signature is valid
 *
 * \param[in] signature Platform signature
 *
 * \return Validity status
 * \retval TRUE the platform signature \p signature is valid
 * \retval FALSE the platform signature \p signature is invalid
 */
extern FUNC(boolean, BASE_CODE) TS_PlatformSigIsValid
(
  uint32 signature
);

#define BASE_STOP_SEC_CODE
#include <Base_MemMap.h>

#endif /* ifndef TS_PBCONFIG_SIGNATURE_H */
