#ifndef TS_AUTOSAR_H
#define TS_AUTOSAR_H

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
 * MISRAC2012-1) Deviated Rule: 20.10 (advisory)
 * The # and ## preprocessor operators should not be used.
 *
 * Reason:
 * 1) The operators are used to concatenate the error identifier for TS_STATIC_ASSERT.
 * There is no other way to assemble sensible compiler error messages.
 * 2) The operators are used to add the 'U' or 'UL' suffix to constants
 * which can only be performed using the '##' operator. The ## is required
 * here to prevent premature expansion of the memory class compiler abstraction
 * parameter which results in the call of a macro with an empty argument (in
 * cases where the memory class compiler abstraction is left empty by the
 * integrator).
 * 3) Type-traits can only be implemented using macros and ## in C90.
 * Only a single ## is used.
 *
 */

/*==================[inclusions]=============================================*/

#include <Std_Types.h>     /* Autosar standard data types */
#include <TSAutosar_Cfg.h> /* Non-Autosar platform specific definitions */

/* Common Autosar definitions and common macros are located in Platforms_TSPlatforms.h,
 * which includes header files with common type definitions
 *
 * Platforms Setting:
 * Non-Autosar platform specific definitions are included by the header file
 * Platforms_TSPlatforms.h which is located in Platforms.
 */
#include <Platforms_TSPlatforms.h>

/* Common Autosar definitions and common macros are located in Atomics_TSPlatforms.h,
 * which includes header files with common type definitions
 *
 * Platforms Setting:
 * Non-Autosar platform specific definitions related to atomic assignment
 * are included by the header file Atomics_TSPlatforms.h which is located in Atomics.
 * It contains macros and functions like TS_AtomicSetBit_8/16/32/64,
 * TS_AtomicClearBit_8/16/32/64.
 */
#include <Atomics_TSPlatforms.h>

/*==================[macros]=================================================*/

/*------------------[macros set on the command line]-------------------------*/

#ifndef TS_ARCH_FAMILY
#error TS_ARCH_FAMILY must be defined on the commandline
#endif

#ifndef TS_ARCH_DERIVATE
#error TS_ARCH_DERIVATE must be defined on the commandline
#endif

/*------------------[architecture names]-------------------------------------*/

/** \brief constant for C16X target architecture family */
#define TS_C16X        1U
/** \brief constant for PA target architecture family */
#define TS_PA          2U
/** \brief constant for EXCALIBUR target architecture family */
#define TS_EXCALIBUR   3U
/** \brief constant for F2MC16L target architecture family */
#define TS_F2MC16L     4U
/** \brief constant for MB91 target architecture family */
#define TS_MB91        5U
/** \brief constant for H8 target architecture family */
#define TS_H8          6U
/** \brief constant for HC08 target architecture family */
#define TS_HC08        7U
/** \brief constant for HC12 target architecture family */
#define TS_HC12        8U
/** \brief constant for M32C target architecture family */
#define TS_M32C        9U
/** \brief constant for NEC78K target architecture family */
#define TS_NEC78K     10U
/** \brief constant for S12X target architecture family */
#define TS_S12X       11U
/** \brief constant for SAF7780 target architecture family */
#define TS_SAF7780    12U
/** \brief constant for SH2 target architecture family */
#define TS_SH2        13U
/** \brief constant for ST30 target architecture family */
#define TS_ST30       14U
/** \brief constant for TMS470 target architecture family */
#define TS_TMS470     15U
/** \brief constant for TRICORE target architecture family */
#define TS_TRICORE    16U
/** \brief constant for V850 target architecture family */
#define TS_V850       17U
/** \brief constant for XC16X target architecture family */
#define TS_XC16X      18U
/** \brief constant for WINDOWS target architecture family */
#define TS_WINDOWS    19U
/** \brief constant for XC2000 target architecture family */
#define TS_XC2000     20U
/** \brief constant for R32C target architecture family */
#define TS_R32C       21U
/** \brief constant for MPC551X target architecture family */
#define TS_MPC551X    22U
/** \brief constant for EASYCAN target architecture family */
#define TS_EASYCAN    23U
/** \brief constant for XPC56XX target architecture family */
#define TS_XPC56XX    24U
/** \brief constant for M32R target architecture family */
#define TS_M32R       25U
/** \brief constant for PIC24H target architecture family */
#define TS_PIC24H     26U
/** \brief constant for NIOS2 target architecture family */
#define TS_NIOS2      27U
/** \brief constant for MB96 target architecture family */
#define TS_MB96       28U
/** \brief constant for SH4 target architecture family */
#define TS_SH4        29U
/** \brief constant for EBX1XX target architecture family */
#define TS_EBX1XX     30U
/** \brief constant for ARM target architecture family */
#define TS_ARM        31U
/** \brief constant for FCR4 target architecture family */
#define TS_FCR4       32U
/** \brief constant for PIKEOS target architecture family */
#define TS_PIKEOS     33U
/** \brief constant for S12Z target architecture family */
#define TS_S12Z       34U
/** \brief constant for DSPIC33 target architecture family */
#define TS_DSPIC33    35U
/** \brief constant for LINUX target architecture family */
#define TS_LINUX      36U
/** \brief constant for RH850 target architecture family */
#define TS_RH850      37U
/** \brief constant for RL78 target architecture family */
#define TS_RL78       38U
/** \brief constant for RZ target architecture family */
#define TS_RZ         39U
/** \brief constant for CORTEXM target architecture family */
#define TS_CORTEXM    40U
/** \brief constant for STM8A target architecture family */
#define TS_STM8A      41U
/** \brief constant for TMS320 target architecture family */
#define TS_TMS320     42U
/** \brief constant for ARM64 target architecture family */
#define TS_ARM64      43U

#if (defined TS_PARAM_UNUSED)
#error TS_PARAM_UNUSED already defined
#endif

#if (defined EB_STATIC_CHECK)
/** \brief This macro can be used to avoid compiler warnings
 *
 * It is left empty to not produce a false positive for MISRA 14.2
 */
#define TS_PARAM_UNUSED(x)
#else
/** \brief This macro can be used to avoid compiler warnings */
#define TS_PARAM_UNUSED(x) ((void) (x))
#endif
/*------------------[static assertions]-------------------------------------*/
#if (defined TS_STATIC_ASSERT)
#error TS_STATIC_ASSERT is already defined
#endif
#if (defined TS_STATIC_ASSERT_JOIN)
#error TS_STATIC_ASSERT_JOIN is already defined
#endif
#if (defined TS_STATIC_ASSERT_EVAL)
#error TS_STATIC_ASSERT_EVAL is already defined
#endif

/** \brief internal concatenation helper macro */
/* Deviation MISRAC2012-1 */
#define TS_STATIC_ASSERT_JOIN(msg_id,line) TS_STATIC_ASSERT__##msg_id##__in_line_##line
/** \brief internal parameter evaluation helper macro to serve evaluated tokens to
 *         TS_STATIC_ASSERT_JOIN's ## operators
 */
#define TS_STATIC_ASSERT_EVAL(msg_id,line) TS_STATIC_ASSERT_JOIN(msg_id,line)

/** \brief Report a static assertion (a.k.a. compile-time assert)
 *
 * Use this macro to check a static compile time assertion as
 * part of defensive programming.  With this macro you can check
 * - size of types and size of global variables
 * - alignment of global variables
 * - padding in structures
 *
 * The macro cannot be used for non-static checks involving quantities which
 * cannot be computed at compile time by the compiler.
 *
 * This macro can be used both at global and at local scope, but it should
 * better be used at function level, right after the local variables, before
 * the first statement of the scope.
 *
 * Note1: At global scope, there is potential to confuse tools like doxygen,
 * which mistakes it as a function declaration, unless
 * DOXYGEN_EXCLUDE_SYBOLS += TS_STATIC_ASSERT is added to the respective make
 * file. (e.g. asc_MODULE\doc\project\design\Makefile)
 *
 * Note2: You have to use a trailing semicolon after TS_STATIC_ASSERT().
 *
 * Examples:
 *
 * Ensure the correct size of a data type:
 * - typedef uint8 Mod_IdHandleType;
 * - TS_STATIC_ASSERT(sizeof(Mod_IdHandleType) == 1U, Incorrect_allocation_size)
 *
 * Ensure correct alignment of the data type (32-bit in this case):
 * - Mod_IdHandleType Mod_GlobalIdHandle;
 * - TS_STATIC_ASSERT((&Mod_GlobalIdHandle & 3U) == 0U, Incorrect_alignment)
 *
 * Technical background: If the \a expr evaluates to false, the macro defines
 * an array type with negative array size.  This is an invalid construct in
 * ANSI C and leads to a compiler error.  The identifier of the type is typically
 * included in the compiler's error message for the user to see.  It includes
 * the string "TS_STATIC_ASSERT", the supplied \a msg_id and also the line
 * number on which the macro TS_STATIC_ASSERT() is placed.
 * From this information the user can infer which static assert failed.
 *
 * \param[in] expr     The assertion expression
 * \param[in] msg_id   The error message. It is used as part of an identifier
 *                     that is shown in the compiler error message.
 */

#define TS_STATIC_ASSERT(expr, msg_id) \
  typedef uint8 TS_STATIC_ASSERT_EVAL(msg_id,__LINE__) [(expr) ? 1 : -1]

/*------------------[IF-conversion]-----------------------------------------*/

#if (defined TS_FATBOOL)
  #error TS_FATBOOL is already defined
#endif

/* good compilers generate cmp/sub x,y; subc b,0,0 to form the FAT_BOOLEAN(TM): {0,~0}. Bad compilers still go if-then-else */
/** \brief Create a fat-boolean of the given type for IF-conversion, using \a TS_SELECT.
 * Be sure to assign to temporary variables to avoid double-evaluation. */
#define TS_FATBOOL(type,cond) (((type)0) - ((type)(cond)))

#if (defined TS_FATBOOL_LT)
  #error TS_FATBOOL_LT is already defined
#endif

/* doesn't depend on good compiler - use SRA to do sign-extension to the type's width. */
/** \brief Create a fat-boolean of the given type for IF-conversion, using \a TS_SELECT.
 * Be sure to assign to temporary variables to avoid double-evaluation. */
#define TS_FATBOOL_LT(intype,a,b) ((intype)(((TS_SINT(intype))(((intype)(a)) - ((intype)(b)))) >> ((sizeof(intype) <<3u)-1u)))

#if (defined TS_SELECT)
  #error TS_SELECT is already defined
#endif

/** \brief Portable conditional move, using a \a FATBOOL(TM) {0,~0} condition to select the correct input by & | masking.
 * Be sure to assign to temporary variables to avoid double-evaluation. */
#define TS_SELECT(type, fat_bool, true,false) \
  ((type)((type)((type)(true) & (type)(fat_bool)) | (type)((type)(false) & (type)~(type)(fat_bool))))

/*------------------[merged compile]----------------------------------------*/

/* if a translation unit includes multiple C-Files, it will set this macro to STD_ON */
#ifndef TS_MERGED_COMPILE

/** \brief All source files are built separately by default */
#define TS_MERGED_COMPILE STD_OFF

#endif /* TS_MERGED_COMPILE */

#if (TS_MERGED_COMPILE == STD_ON)

/** \brief Use internal linkage of function declarations, if merged, fast build is used */
#define TS_MOD_PRIV_DECL static
/** \brief Use internal linkage of function definitions, if merged, fast build is used */
#define TS_MOD_PRIV_DEFN static

#else /* TS_MERGED_COMPILE */

/** \brief Use external linkage of function declarations, if normal build is used */
#define TS_MOD_PRIV_DECL extern
/** \brief Use external linkage of function declarations, if normal build is used */
#define TS_MOD_PRIV_DEFN

#endif /* TS_MERGED_COMPILE */

#if (defined TS_VENDOR_ID_3SOFT)
#error TS_VENDOR_ID_3SOFT already defined
#endif

/** \brief definition of the unique vendor ID
 *
 * This vendor ID is based on the vendor ID published by the HIS.
 * It should be used for each module
 */
#define TS_VENDOR_ID_3SOFT 1U

#if (defined TS_VENDOR_ID_EB)
#error TS_VENDOR_ID_EB already defined
#endif

/** \brief map vendor ID of EB to 3SOFT */
#define TS_VENDOR_ID_EB TS_VENDOR_ID_3SOFT

#if (defined TS_PROD_ERR_REP_TO_DEM)
#error TS_PROD_ERR_REP_TO_DEM already defined
#endif

/** \brief definition of production error reporting defines - report to Dem
 *
 * The production error reporting could be configured to Dem, Det and off.
 * TS_PROD_ERR_REP_TO_DEM is used if the production error is reported
 * towards the Dem.
 */
#define TS_PROD_ERR_REP_TO_DEM 0U

#if (defined TS_PROD_ERR_REP_TO_DET)
#error TS_PROD_ERR_REP_TO_DET already defined
#endif

/** \brief definition of production error reporting defines - report to Det
 *
 * The production error reporting could be configured to Dem, Det and off.
 * TS_PROD_ERR_REP_TO_DET is used if the production error is reported
 * towards the Det.
 */
#define TS_PROD_ERR_REP_TO_DET 1U

#if (defined TS_PROD_ERR_DISABLE)
#error TS_PROD_ERR_DISABLE already defined
#endif

/** \brief definition of production error reporting defines - off
 *
 * The production error reporting could be configured to Dem, Det and off.
 * TS_PROD_ERR_DISABLE is used if the production error reporting is
 * switched off.
 */
#define TS_PROD_ERR_DISABLE 2U

/*------------------[UINTX_C macros]-----------------------------------------*/

#ifndef UINT8_C
/* Deviation MISRAC2012-1 <+2> */
/** \brief Macro to define a constant of type uint8 */
#define UINT8_C(x) ((uint8) x ## U)
#endif

#ifndef UINT16_C
/* Deviation MISRAC2012-1 <+2> */
/** \brief Macro to define a constant of type uint16 */
#define UINT16_C(x) ((uint16) x ## U)
#endif

#ifndef UINT32_C
/* Deviation MISRAC2012-1 <+2> */
/** \brief Macro to define a constant of type uint32 */
#define UINT32_C(x) (x ## UL)
#endif

#ifndef SINT8_C
/** \brief Macro to define a constant of type sint8 */
#define SINT8_C(x) ((sint8)(x))
#endif

#ifndef SINT16_C
/** \brief Macro to define a constant of type sint16 */
#define SINT16_C(x) ((sint16)(x))
#endif

#ifndef SINT32_C
/* Deviation MISRAC2012-1 <+2> */
/** \brief Macro to define a constant of type sint32 */
#define SINT32_C(x) (x ## L)
#endif

#if (defined HAS_64BIT_TYPES)
#ifndef SINT64_C
/* Deviation MISRAC2012-1 <+2> */
/** \brief Macro to define a constant of type sint64 */
#define SINT64_C(x) (x ## LL)
#endif

#ifndef UINT64_C
/* Deviation MISRAC2012-1 <+2> */
/** \brief Macro to define a constant of type uint64 */
#define UINT64_C(x) (x ## ULL)
#endif
#endif /* if (defined HAS_64BIT_TYPES) */

#ifndef BOOLEAN_C
/* Deviation MISRAC2012-1 <+2> */
/** \brief Macro to define a constant of type boolean */
#define BOOLEAN_C(x) ((boolean) x ## U)
#endif

/*------------------[Bit library macros]-------------------------------------*/

/*------------------[TS_IsBitSet]--------------------------------------------*/
#ifndef TS_IsBitSet

/** \brief Check if a bit is set in a variable of type \p OpType
 *
 * This functions returns non-zero if the bit number \p Bit from
 * the variable of type \p OpType pointed to by \p Address is set.
 *
 * The parameter \p OpType can either be \p uint8, \p uint16
 * or \p uint32.
 *
 * \pre \p Address shall be a valid pointer
 * \pre \p Address shall point to an object of type \p OpType
 * \pre depending on the value of \p OpType, the parameter \p Bit
 *   shall have the following range:
 *   \p uint8: 0..7, \p uint16: 0..15, \p uint32: 0..31
 * \pre \p OpType shall be one of \p uint8, \p uint16 or \p uint32
 *
 * \param[in]  Address pointer to the variable of type \p OpType
 * \param[in]  Bit     number of the bit to extract
 * \param[in]  OpType  type of this whole operation
 *
 * \remarks The parameters of this macro may be used in any way, especially
 *   they can be used more than once. They shall not contain side effects.
 */
#define TS_IsBitSet(Address,Bit,OpType) \
  ((*(Address) & ((OpType)((OpType)1U<<(Bit)))) != 0U)

#endif

/*------------------[TS_GetBit]----------------------------------------------*/
#ifndef TS_GetBit

/** \brief Extracts a bit from a variable of type \p OpType
 *
 * This functions returns the bit number \p Bit from the variable
 * of type \p OpType pointed to by \p Address.
 *
 * The parameter \p OpType can either be \p uint8, \p uint16
 * or \p uint32.
 *
 * \pre \p Address shall be a valid pointer
 * \pre \p Address shall point to an object of type \p OpType
 * \pre depending on the value of \p OpType, the parameter \p Bit
 *   shall have the following range:
 *   \p uint8: 0..7, \p uint16: 0..15, \p uint32: 0..31
 * \pre \p OpType shall be one of \p uint8, \p uint16 or \p uint32
 * \pre \p Result shall be a modifiable variable of type \p OpType.
 *
 * \param[in]  Address pointer to the variable of type \p OpType
 * \param[in]  Bit     number of the bit to extract
 * \param[in]  OpType  type of this whole operation
 * \param[out] Result  variable of type \p OpType to store result
 *
 * \remarks The parameters of this macro may be used in any way, especially
 *   they can be used more than once. They shall not contain side effects.
 */
#define TS_GetBit(Address,Bit,OpType,Result)         \
  ((Result) = (OpType)( ((OpType)( ((OpType)*(Address)) >> (Bit) )) & ((OpType)1u)) )

#endif

/*------------------[TS_GetBitGroup]-----------------------------------------*/
#ifndef TS_GetBitGroup
/** \brief Gets the level of a group of bits
 *
 * This macro extracts the level of several consecutive bits indicated
 * by \p Mask and \p Offset from the variable pointed to by \p Address.
 *
 * Preconditions:
 * \pre \p Address shall be a valid pointer
 * \pre \p Address shall point to a variable of type \p OpType
 * \pre \p Mask shall be of type \p OpType
 * \pre Depending on the type of \p OpType, the
 *   parameter \p Offset shall have the following range:
 *   \p uint8: 0..7, \p uint16: 0..15, \p uint32: 0..31
 * \pre \p OpType shall be one of \p uint8, \p uint16 or \p uint32
 * \pre \p Result shall be a modifiable variable of type \p OpType
 *
 * \param[in]  Address pointer to the register to extract the information
 * \param[in]  Mask    describes the consecutive bits, type OpType
 * \param[in]  Offset  describes the bit positions, range: see above
 * \param[in]  OpType  gives the type of the operation
 * \param[out] Result  output parameter holding the result
 *
 * \remarks The parameters of this macro may be used in any way, especially
 *   they can be used more than once. They shall not contain side effects.
 */
#define TS_GetBitGroup(Address,Mask,Offset,OpType,Result) \
  ((Result) = (OpType) (((OpType)(*(Address))) & (OpType)(Mask)) >> (Offset))

#endif

/*------------------[TS_DefMaxAlignedByteArray]------------------------------*/
#if (defined TS_DefMaxAlignedByteArray)
#error TS_DefMaxAlignedByteArray already defined
#endif

/** \brief Define a byte array with maximum alignment
 *
 * This macro defines \p identifier as an array of at least
 * \p numberOfBytes bytes in a way that this array is aligned
 * according to the most stringent alignment requirement of any
 * simple data type available on the respective platform.
 *
 * \pre \p identifier shall be a valid C identifier
 *
 * \param[in]  identifier    C identifier to use for the array definition
 * \param[in] numberOfBytes number of bytes to be provided in the array
 * \param[in] module modules abbreviation in capital letters (e.g., COM, FR_1_EB)
 * \param[in] memclass memory classification (without module abbreviation) for the array
 */
/* Deviation MISRAC2012-1 */
#define TS_DefMaxAlignedByteArray(identifier, module, memclass, numberOfBytes) \
  TS_DefMaxAlignedByteArray_Hlp(identifier, module, _ ## memclass, numberOfBytes)

/** \brief Helper macro for TS_DefMaxAlignedByteArray */
/* Deviation MISRAC2012-1 */
#define TS_DefMaxAlignedByteArray_Hlp(identifier, module, memclass, numberOfBytes) \
  VAR(TS_MaxAlignedType, module ## memclass) \
    identifier[(((numberOfBytes) - 1) + sizeof(TS_MaxAlignedType)) / sizeof(TS_MaxAlignedType)]

#endif /* ifndef TS_AUTOSAR_H */
