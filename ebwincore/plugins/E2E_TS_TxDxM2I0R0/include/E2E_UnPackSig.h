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
 * The # and ## preprocessor operators should not be used.
 *
 * Reason:
 * ## operator is used for code generation via pre-processor.
 *
 */

#ifndef E2E_UNPACKSIG_H
#define E2E_UNPACKSIG_H

/*==================[inclusions]=============================================*/

#include <Std_Types.h>              /* AUTOSAR Std types */

/*==================[macros]=================================================*/

/* Deviation MISRAC2012-1 <START> */

#if (defined E2E_RANGECHK_VALID) /* To prevent double declaration */
#error E2E_RANGECHK_VALID already defined
#endif /* if (defined E2E_RANGECHK_VALID) */

/** \brief Define E2E_RANGECHK_VALID with 0 for a successful range check result */
#define E2E_RANGECHK_VALID 0U

#if (defined E2E_RANGECHK_INVALID) /* To prevent double declaration */
#error E2E_RANGECHK_INVALID already defined
#endif /* if (defined E2E_RANGECHK_INVALID) */

/** \brief Define E2E_RANGECHK_INVALID with 255 for a successful range check result */
#define E2E_RANGECHK_INVALID 255U

#if (defined E2E_COMP_ABS_USER_HELP) /* To prevent double declaration */
#error E2E_COMP_ABS_USER_HELP already defined
#endif /* if (defined E2E_COMP_ABS_USER_HELP) */
/* Defined for generic usage of module specific compiler abstraction macros */
#define E2E_COMP_ABS_USER_HELP(x) x ## _APPL_DATA

#if (defined E2E_COMP_ABS_USER) /* To prevent double declaration */
#error E2E_COMP_ABS_USER already defined
#endif /* if (defined E2E_COMP_ABS_USER) */
/* Defined for generic usage of module specific compiler abstraction macros */
#define E2E_COMP_ABS_USER(x) E2E_COMP_ABS_USER_HELP(x)

#if (defined E2E_STATIC_ASSERT_CONC) /* To prevent double declaration */
#error E2E_STATIC_ASSERT_CONC is already defined
#endif /* if (defined E2E_STATIC_ASSERT_CONC) */

/* Internal helper macro */
#define E2E_STATIC_ASSERT_CONC(X,Y) E2E_STATIC_ASSERT_CONC_HLP(X,Y)

#if (defined E2E_STATIC_ASSERT_CONC_HLP) /* To prevent double declaration */
#error E2E_STATIC_ASSERT_CONC_HLP is already defined
#endif /* if (defined E2E_STATIC_ASSERT_CONC_HLP) */

/* Internal helper macro */
#define E2E_STATIC_ASSERT_CONC_HLP(X,Y) X##Y

#if (defined E2E_STATIC_ASSERT) /* To prevent double declaration */
#error E2E_STATIC_ASSERT is already defined
#endif /* if (defined E2E_STATIC_ASSERT) */

/* CheckC disabled due to warning about two consecutive underscores.
 * The assertion uses the standard predefined macro to provide
 * additional information to the assert report */
/* CHECK: RULE 501 OFF */
#if !(defined COMPILER_STATIC_ASSERT)
#define E2E_STATIC_ASSERT(expr)                                               \
  typedef uint8 E2E_STATIC_ASSERT_CONC(E2EStaticAssertFailedInLine,__LINE__)[ \
    (expr) ? 1 : -1]
/* CHECK: RULE 501 ON */
#else
#define E2E_STATIC_ASSERT(expr) COMPILER_STATIC_ASSERT(expr)
#endif /* !(defined COMPILER_STATIC_ASSERT) */

#if (defined E2E_STATIC_ASSERT_FLOAT) /* To prevent double declaration */
#error E2E_STATIC_ASSERT_FLOAT is already defined
#endif /* if (defined E2E_STATIC_ASSERT_FLOAT) */

/* CheckC disabled due to warning about two consecutive underscores.
 * The assertion uses the standard predefined macro to provide
 * additional information to the assert report */
/* CHECK: RULE 501 OFF */
#define E2E_STATIC_ASSERT_FLOAT(expr)                                         \
  typedef uint8 E2E_STATIC_ASSERT_CONC(E2EStaticAssertFloatFailedInLine,__LINE__)[ \
    (expr) ? 1 : -1]
/* CHECK: RULE 501 ON */

/* ============= helper macros for packing signals into memory ======== */
#if (defined E2E_PACKSIG) /* To prevent double declaration */
#error E2E_PACKSIG already defined
#endif /* if (defined E2E_PACKSIG) */
#if (defined E2E_PACKSIG_E2E_LE_BL) /* To prevent double declaration */
#error E2E_PACKSIG_E2E_LE_BL already defined
#endif /* if (defined E2E_PACKSIG_E2E_LE_BL) */
#if (defined E2E_PACKSIG_E2E_LE_U8) /* To prevent double declaration */
#error E2E_PACKSIG_E2E_LE_U8 already defined
#endif /* if (defined E2E_PACKSIG_E2E_LE_U8) */
#if (defined E2E_PACKSIG_E2E_LE_U16) /* To prevent double declaration */
#error E2E_PACKSIG_E2E_LE_U16 already defined
#endif /* if (defined E2E_PACKSIG_E2E_LE_U16) */
#if (defined E2E_PACKSIG_E2E_LE_U32) /* To prevent double declaration */
#error E2E_PACKSIG_E2E_LE_U32 already defined
#endif /* if (defined E2E_PACKSIG_E2E_LE_U32) */
#if (defined E2E_PACKSIG_E2E_LE_S8) /* To prevent double declaration */
#error E2E_PACKSIG_E2E_LE_S8 already defined
#endif /* if (defined E2E_PACKSIG_E2E_LE_S8) */
#if (defined E2E_PACKSIG_E2E_LE_S16) /* To prevent double declaration */
#error E2E_PACKSIG_E2E_LE_S16 already defined
#endif /* if (defined E2E_PACKSIG_E2E_LE_S16) */
#if (defined E2E_PACKSIG_E2E_LE_S32) /* To prevent double declaration */
#error E2E_PACKSIG_E2E_LE_S32 already defined
#endif /* if (defined E2E_PACKSIG_E2E_LE_S32) */
#if (defined E2E_PACKSIG_E2E_BE_BL) /* To prevent double declaration */
#error E2E_PACKSIG_E2E_BE_BL already defined
#endif /* if (defined E2E_PACKSIG_E2E_BE_BL) */
#if (defined E2E_PACKSIG_E2E_BE_U8) /* To prevent double declaration */
#error E2E_PACKSIG_E2E_BE_U8 already defined
#endif /* if (defined E2E_PACKSIG_E2E_BE_U8) */
#if (defined E2E_PACKSIG_E2E_BE_U16) /* To prevent double declaration */
#error E2E_PACKSIG_E2E_BE_U16 already defined
#endif /* if (defined E2E_PACKSIG_E2E_BE_U16) */
#if (defined E2E_PACKSIG_E2E_BE_U32) /* To prevent double declaration */
#error E2E_PACKSIG_E2E_BE_U32 already defined
#endif /* if (defined E2E_PACKSIG_E2E_BE_U32) */
#if (defined E2E_PACKSIG_E2E_BE_S8) /* To prevent double declaration */
#error E2E_PACKSIG_E2E_BE_S8 already defined
#endif /* if (defined E2E_PACKSIG_E2E_BE_S8) */
#if (defined E2E_PACKSIG_E2E_BE_S16) /* To prevent double declaration */
#error E2E_PACKSIG_E2E_BE_S16 already defined
#endif /* if (defined E2E_PACKSIG_E2E_BE_S16) */
#if (defined E2E_PACKSIG_E2E_BE_S32) /* To prevent double declaration */
#error E2E_PACKSIG_E2E_BE_S32 already defined
#endif /* if (defined E2E_PACKSIG_E2E_BE_S32) */
#if (defined E2E_PACKSIG_E2E_OP_U8N) /* To prevent double declaration */
#error E2E_PACKSIG_E2E_OP_U8N already defined
#endif /* if (defined E2E_PACKSIG_E2E_OP_U8N) */
#if (defined E2E_PACKSIG_E2E_BE_F32) /* To prevent double declaration */
#error E2E_PACKSIG_E2E_BE_F32 already defined
#endif /* if (defined E2E_PACKSIG_E2E_BE_F32) */
#if (defined E2E_PACKSIG_E2E_LE_F32) /* To prevent double declaration */
#error E2E_PACKSIG_E2E_LE_F32 already defined
#endif /* if (defined E2E_PACKSIG_E2E_LE_F32) */
#if (defined E2E_PACKSIG_E2E_BE_F64) /* To prevent double declaration */
#error E2E_PACKSIG_E2E_BE_F64 already defined
#endif /* if (defined E2E_PACKSIG_E2E_BE_F64) */
#if (defined E2E_PACKSIG_E2E_LE_F64) /* To prevent double declaration */
#error E2E_PACKSIG_E2E_LE_F64 already defined
#endif /* if (defined E2E_PACKSIG_E2E_LE_F64) */
#if (defined E2E_PACKSIG_E2E_LE_U64) /* To prevent double declaration */
#error E2E_PACKSIG_E2E_LE_U64 already defined
#endif /* if (defined E2E_PACKSIG_E2E_LE_U64) */
#if (defined E2E_PACKSIG_E2E_BE_U64) /* To prevent double declaration */
#error E2E_PACKSIG_E2E_BE_U64 already defined
#endif /* if (defined E2E_PACKSIG_E2E_BE_U64) */
#if (defined E2E_PACKSIG_E2E_LE_S64) /* To prevent double declaration */
#error E2E_PACKSIG_E2E_LE_S64 already defined
#endif /* if (defined E2E_PACKSIG_E2E_LE_S64) */
#if (defined E2E_PACKSIG_E2E_BE_S64) /* To prevent double declaration */
#error E2E_PACKSIG_E2E_BE_S64 already defined
#endif /* if (defined E2E_PACKSIG_E2E_BE_S64) */

/** \brief Macro for packing a Data Element member into a serialized data memory.
 *
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] structmember Member of the Data Element which shall be serialized i.e. DataElm->structmember
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] sigtype Signal type of the Data Element member in the serialized memory.
 *            Shall be one of: E2E_[LE|BE]_[BL|U8|U16|U32|U64|F32|F64|S8|S16|S32|S64] or E2E_OP_U8N
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 *
 * Note:
 * Check limitations of uint64 and sint64
 * No range checks for float32 and float64 signal type
 */
#define E2E_PACKSIG(memptr,structmember,bitpos,bitlength,sigtype,nbytes) \
   do {\
      E2E_CHECKRANGE_ ## sigtype(bitlength,(structmember)); \
      E2E_PACKSIG_ ## sigtype((bitlength),(bitpos),(memptr),(structmember),nbytes); \
   } while ((0))

/** \brief Macro for packing a boolean variable into a serialized data memory in Little Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 */
#define E2E_PACKSIG_E2E_LE_BL(bitlength,bitpos,memptr,value,nbytes) \
   E2E_PACK_SIGNAL_INTO_MEMORY_LE((bitlength),(bitpos),(memptr),(value),nbytes)

/** \brief Macro for packing a uint8 variable into a serialized data memory in Little Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 */
#define E2E_PACKSIG_E2E_LE_U8(bitlength,bitpos,memptr,value,nbytes) \
   E2E_PACK_SIGNAL_INTO_MEMORY_LE((bitlength),(bitpos),(memptr),(value),nbytes)

/** \brief Macro for packing a uint16 variable into a serialized data memory in Little Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 */
#define E2E_PACKSIG_E2E_LE_U16(bitlength,bitpos,memptr,value,nbytes) \
   E2E_PACK_SIGNAL_INTO_MEMORY_LE((bitlength),(bitpos),(memptr),(value),nbytes)

/** \brief Macro for packing a uint32 variable into a serialized data memory in Little Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 */
#define E2E_PACKSIG_E2E_LE_U32(bitlength,bitpos,memptr,value,nbytes) \
   E2E_PACK_SIGNAL_INTO_MEMORY_LE((bitlength),(bitpos),(memptr),(value),nbytes)

/** \brief Macro for packing a uint64 variable into a serialized data memory in Little Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 *
 * Note: uint64 packing includes the following limitations:
 *      - data shall be byte aligned: bitpos lsb % 8 == 0
 *      - bitlength shall be a multiple of 8
 *      - bitlength shall have constancy to nbytes
 */
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
#define E2E_PACKSIG_E2E_LE_U64(bitlength,bitpos,memptr,value,nbytes)                \
   do {                                                                             \
    uint8_least byte_cnt;                                                           \
    P2CONST(uint8, AUTOMATIC, E2E_COMP_ABS_USER(E2E_USER_PREFIX)) value_ptr =       \
    ((P2CONST(uint8, AUTOMATIC, E2E_COMP_ABS_USER(E2E_USER_PREFIX))) (&value));     \
      /* E2E_LE_U64 bitpos is lsb and shall be byte aligned AND
       * bitlength constancy to nbytes AND
       * bitlength multiple of 8 AND
       * nbytes shall be smaller than or equal to 8 bytes AND
       * nbytes shall be greater than 0 bytes */                                    \
      E2E_STATIC_ASSERT( (((bitpos) % 8U) == 0U) &&                                 \
                         (((bitlength) / 8U) == (nbytes)) &&                        \
                         (((bitlength) % 8U) == 0U) &&                              \
                         ((nbytes) <= 8U) &&                                        \
                         ((nbytes) > 0U) );                                         \
      for (byte_cnt=0U; byte_cnt<(nbytes); ++byte_cnt)                              \
      {                                                                             \
         (memptr)[byte_cnt+((bitpos)/8U)] =                                         \
         (uint8)(value_ptr[byte_cnt]);                                              \
      }                                                                             \
   } while ((0))
#else /* CPU_BYTE_ORDER is HIGH_BYTE_FIRST */
#define E2E_PACKSIG_E2E_LE_U64(bitlength,bitpos,memptr,value,nbytes)                \
   do {                                                                             \
    uint8_least byte_cnt;                                                           \
    uint8_least byte_down_cnt = 8U;                                                 \
    P2CONST(uint8, AUTOMATIC, E2E_COMP_ABS_USER(E2E_USER_PREFIX)) value_ptr =       \
    ((P2CONST(uint8, AUTOMATIC, E2E_COMP_ABS_USER(E2E_USER_PREFIX))) (&value));     \
      /* E2E_LE_U64 bitpos is lsb and shall be byte aligned AND
       * bitlength constancy to nbytes AND
       * bitlength multiple of 8 AND
       * nbytes shall be smaller than or equal to 8 bytes AND
       * nbytes shall be greater than 0 bytes */                                    \
      E2E_STATIC_ASSERT( (((bitpos) % 8U) == 0U) &&                                 \
                         (((bitlength) / 8U) == (nbytes)) &&                        \
                         (((bitlength) % 8U) == 0U) &&                              \
                         ((nbytes) <= 8U) &&                                        \
                         ((nbytes) > 0U) );                                         \
      for (byte_cnt=0U; byte_cnt<(nbytes); ++byte_cnt)                              \
      {                                                                             \
         --byte_down_cnt;                                                           \
         (memptr)[byte_cnt+((bitpos)/8U)] =                                         \
         (uint8)(value_ptr[byte_down_cnt]);                                         \
      }                                                                             \
   } while ((0))
#endif /* (CPU_BYTE_ORDER == LOW_BYTE_FIRST) */

/** \brief Macro for packing a float32 variable into a serialized data memory in little Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 *
 * Note: float32 data type mapping to uint32
 *       float32 packing includes the following additional limitations:
 *      - bitlength shall be 32U
 *      - nbytes shall be 4U or 5U
 */
#define E2E_PACKSIG_E2E_LE_F32(bitlength,bitpos,memptr,value,nbytes) \
        /* IEEE754 memory encoding for single precision */ \
        { E2E_STATIC_ASSERT_FLOAT( ((bitlength) == 32U) && \
                               ( ((nbytes) == 4U) || \
                                 ((nbytes) == 5U) ) ); } \
        E2E_PACKSIG_E2E_LE_U32((bitlength),(bitpos),(memptr), \
        (* (P2CONST(uint32, AUTOMATIC, E2E_COMP_ABS_USER(E2E_USER_PREFIX)))(&value)),nbytes)

/** \brief Macro for packing a float64 variable into a serialized data memory in little Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 *
 * Note: float64 data type mapping to uint64.
 *       uint64 packing includes the following limitations:
 *      - data shall be byte aligned: bitpos lsb % 8 == 0
 *      - bitlength shall be a multiple of 8
 *      - bitlength shall have constancy to nbytes
 *      float64 packing includes the following additional limitations:
 *      - bitlength shall be 64U
 *      - nbytes shall be 8U
 */
#define E2E_PACKSIG_E2E_LE_F64(bitlength,bitpos,memptr,value,nbytes) \
        /* IEEE754 memory encoding for double precision */ \
        { E2E_STATIC_ASSERT_FLOAT( ((bitlength) == 64U) && \
                                 ((nbytes) == 8U) ); } \
        /* save E2E_RangeCheckRetVal since no range check for float64 */ \
        E2E_PACKSIG_E2E_LE_U64((bitlength),(bitpos),(memptr), \
        (* (P2CONST(uint64, AUTOMATIC, E2E_COMP_ABS_USER(E2E_USER_PREFIX)))(&value)),nbytes)

/** \brief Macro for packing a sint8 variable into a serialized data memory in Little Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 */
#define E2E_PACKSIG_E2E_LE_S8(bitlength,bitpos,memptr,value,nbytes) \
   E2E_PACK_SIGNAL_INTO_MEMORY_LE((bitlength),(bitpos),(memptr),(value),nbytes)

/** \brief Macro for packing a sint16 variable into a serialized data memory in Little Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 */
#define E2E_PACKSIG_E2E_LE_S16(bitlength,bitpos,memptr,value,nbytes) \
   E2E_PACK_SIGNAL_INTO_MEMORY_LE((bitlength),(bitpos),(memptr),(value),nbytes)

/** \brief Macro for packing a sint32 variable into a serialized data memory in Little Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 */
#define E2E_PACKSIG_E2E_LE_S32(bitlength,bitpos,memptr,value,nbytes) \
   E2E_PACK_SIGNAL_INTO_MEMORY_LE((bitlength),(bitpos),(memptr),(value),nbytes)

/** \brief Macro for packing a sint64 variable into a serialized data memory in Little Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 *
 * Note: sint64 packing includes the following limitations:
 *      - data shall be byte aligned: bitpos lsb % 8 == 0
 *      - bitlength shall be a multiple of 8
 */
#define E2E_PACKSIG_E2E_LE_S64(bitlength,bitpos,memptr,value,nbytes)           \
   E2E_PACKSIG_E2E_LE_U64((bitlength),(bitpos),(memptr),(value),(nbytes))

/** \brief Macro for packing a boolean variable into a serialized data memory in Big Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 */
#define E2E_PACKSIG_E2E_BE_BL(bitlength,bitpos,memptr,value,nbytes) \
   E2E_PACK_SIGNAL_INTO_MEMORY_BE((bitlength),(bitpos),(memptr),(value),nbytes)

/** \brief Macro for packing a uint8 variable into a serialized data memory in Big Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 */
#define E2E_PACKSIG_E2E_BE_U8(bitlength,bitpos,memptr,value,nbytes) \
   E2E_PACK_SIGNAL_INTO_MEMORY_BE((bitlength),(bitpos),(memptr),(value),nbytes)

/** \brief Macro for packing a uint16 variable into a serialized data memory in Big Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 */
#define E2E_PACKSIG_E2E_BE_U16(bitlength,bitpos,memptr,value,nbytes) \
   E2E_PACK_SIGNAL_INTO_MEMORY_BE((bitlength),(bitpos),(memptr),(value),nbytes)

/** \brief Macro for packing a uint32 variable into a serialized data memory in Big Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 */
#define E2E_PACKSIG_E2E_BE_U32(bitlength,bitpos,memptr,value,nbytes) \
   E2E_PACK_SIGNAL_INTO_MEMORY_BE((bitlength),(bitpos),(memptr),(value),nbytes)

/** \brief Macro for packing a uint64 variable into a serialized data memory in Big Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 *
 * Note: uint64 packing includes the following limitations:
 *      - data shall be byte aligned: bitpos msb % 8 == 7
 *      - bitlength shall be a multiple of 8
 *      - bitlength shall have constancy to nbytes
 */
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
#define E2E_PACKSIG_E2E_BE_U64(bitlength,bitpos,memptr,value,nbytes)                \
do {                                                                                \
    uint8_least byte_cnt;                                                           \
    uint8_least byte_down_cnt = (nbytes);                                           \
    P2CONST(uint8, AUTOMATIC, E2E_COMP_ABS_USER(E2E_USER_PREFIX)) value_ptr =       \
    ((P2CONST(uint8, AUTOMATIC, E2E_COMP_ABS_USER(E2E_USER_PREFIX))) (&value));     \
      /* E2E_BE_U64 bitpos is lsb and shall be byte aligned AND
       * bitlength constancy to nbytes AND
       * bitlength multiple of 8 AND
       * nbytes shall be smaller than or equal to 8 bytes AND
       * nbytes shall be greater than 0 bytes */                                    \
      E2E_STATIC_ASSERT( (((bitpos) % 8U) == 7U) &&                                 \
                         (((bitlength) / 8U) == (nbytes)) &&                        \
                         (((bitlength) % 8U) == 0U) &&                              \
                         ((nbytes) <= 8U) &&                                        \
                         ((nbytes) > 0U) );                                         \
      for (byte_cnt=0U; byte_cnt<(nbytes); ++byte_cnt)                              \
      {                                                                             \
        --byte_down_cnt;                                                            \
        (memptr)[byte_cnt+((bitpos)/8U)] =                                          \
        (uint8)(value_ptr[byte_down_cnt]);                                          \
      }                                                                             \
} while ((0))
#else /* CPU_BYTE_ORDER is HIGH_BYTE_FIRST */
#define E2E_PACKSIG_E2E_BE_U64(bitlength,bitpos,memptr,value,nbytes)                \
do {                                                                                \
    uint8_least byte_cnt;                                                           \
    uint8 byte_offset = (uint8)(8U-(nbytes));                                       \
    P2CONST(uint8, AUTOMATIC, E2E_COMP_ABS_USER(E2E_USER_PREFIX)) value_ptr =       \
    ((P2CONST(uint8, AUTOMATIC, E2E_COMP_ABS_USER(E2E_USER_PREFIX))) (&value));     \
      /* E2E_BE_U64 bitpos is lsb and shall be byte aligned AND
       * bitlength constancy to nbytes AND
       * bitlength multiple of 8 AND
       * nbytes shall be smaller than or equal to 8 bytes AND
       * nbytes shall be greater than 0 bytes */                                    \
      E2E_STATIC_ASSERT( (((bitpos) % 8U) == 7U) &&                                 \
                         (((bitlength) / 8U) == (nbytes)) &&                        \
                         (((bitlength) % 8U) == 0U) &&                              \
                         ((nbytes) <= 8U) &&                                        \
                         ((nbytes) > 0U) );                                         \
      for (byte_cnt=0U; byte_cnt<(nbytes); ++byte_cnt)                              \
      {                                                                             \
        (memptr)[byte_cnt+((bitpos)/8U)] =                                          \
        (uint8)(value_ptr[byte_cnt + byte_offset]);                                 \
      }                                                                             \
} while ((0))
#endif /* (CPU_BYTE_ORDER == LOW_BYTE_FIRST) */

/** \brief Macro for packing a float32 variable into a serialized data memory in Big Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 *
 * Note: float32 data type mapping to uint32
 *       float32 packing includes the following additional limitations:
 *      - bitlength shall be 32U
 *      - nbytes shall be 4U or 5U
 */
#define E2E_PACKSIG_E2E_BE_F32(bitlength,bitpos,memptr,value,nbytes) \
        /* IEEE754 memory encoding for single precision */ \
        { E2E_STATIC_ASSERT_FLOAT( ((bitlength) == 32U) && \
                               ( ((nbytes) == 4U) || \
                                 ((nbytes) == 5U) ) ); } \
        E2E_PACKSIG_E2E_BE_U32((bitlength),(bitpos),(memptr), \
        (* (P2CONST(uint32, AUTOMATIC, E2E_COMP_ABS_USER(E2E_USER_PREFIX)))(&value)),nbytes)

 /** \brief Macro for packing a float64 variable into a serialized data memory in Big Endian order.
  *
  * \param[in] bitlength Bit length of the Data Element member in the serialized memory
  * \param[in] bitpos Bit position of the Data Element member in the serialized memory
  * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
  * \param[in] value The data value which shall be serialized
  * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
  *
 * Note: float64 data type mapping to uint64.
 *       uint64 packing includes the following limitations:
 *      - data shall be byte aligned: bitpos lsb % 8 == 0
 *      - bitlength shall be a multiple of 8
 *      - bitlength shall have constancy to nbytes
 *      float64 packing includes the following additional limitations:
 *      - bitlength shall be 64U
 *      - nbytes shall be 8U
  */
 #define E2E_PACKSIG_E2E_BE_F64(bitlength,bitpos,memptr,value,nbytes) \
         /* IEEE754 memory encoding for single precision */ \
         { E2E_STATIC_ASSERT_FLOAT( ((bitlength) == 64U) && \
                                  ((nbytes) == 8U) ); } \
         E2E_PACKSIG_E2E_BE_U64((bitlength),(bitpos),(memptr), \
         (* (P2CONST(uint64, AUTOMATIC, E2E_COMP_ABS_USER(E2E_USER_PREFIX)))(&value)),nbytes)

/** \brief Macro for packing a sint8 variable into a serialized data memory in Big Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 */
#define E2E_PACKSIG_E2E_BE_S8(bitlength,bitpos,memptr,value,nbytes) \
   E2E_PACK_SIGNAL_INTO_MEMORY_BE((bitlength),(bitpos),(memptr),(value),nbytes)

/** \brief Macro for packing a sint16 variable into a serialized data memory in Big Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 */
#define E2E_PACKSIG_E2E_BE_S16(bitlength,bitpos,memptr,value,nbytes) \
   E2E_PACK_SIGNAL_INTO_MEMORY_BE((bitlength),(bitpos),(memptr),(value),nbytes)

/** \brief Macro for packing a sint32 variable into a serialized data memory in Big Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 */
#define E2E_PACKSIG_E2E_BE_S32(bitlength,bitpos,memptr,value,nbytes) \
   E2E_PACK_SIGNAL_INTO_MEMORY_BE((bitlength),(bitpos),(memptr),(value),nbytes)

/** \brief Macro for packing a sint64 variable into a serialized data memory in Big Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] value The data value which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 *
 * Note: sint64 packing includes the following limitations:
 *      - data shall be byte aligned: bitpos msb % 8 == 7
 *      - bitlength shall be a multiple of 8
 *      - bitlength shall have constancy to nbytes
 */
#define E2E_PACKSIG_E2E_BE_S64(bitlength,bitpos,memptr,value,nbytes)           \
   E2E_PACKSIG_E2E_BE_U64((bitlength),(bitpos),(memptr),(value),(nbytes))

/** \brief Macro for packing an opaque uint8n variable into a serialized data memory.
 *
 * \param[in] structmember Member of the Data Element which shall be serialized
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] dataptr The Data Element which shall be serialized
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 */
#define E2E_PACKSIG_E2E_OP_U8N(bitlength,bitpos,memptr,dataptr,nbytes)              \
   do {                                                                             \
      uint8_least byte_cnt;                                                         \
      /* E2E_OP_U8N treated as LE were bitpos is lsb and shall be byte aligned AND
       * bitlength constancy to nbytes AND
       * bitlength multiple of 8 */                                                 \
      E2E_STATIC_ASSERT( (((bitpos) % 8U) == 0U) &&                                 \
                         (((bitlength) / 8U) == (nbytes)) &&                        \
                         (((bitlength) % 8U) == 0U) );                              \
      for (byte_cnt=0U; byte_cnt<(nbytes); ++byte_cnt)                              \
      {                                                                             \
         (memptr)[byte_cnt+((bitpos)/8U)] = (dataptr)[byte_cnt];                    \
      }                                                                             \
   } while ((0))

/* =============== helper macros for unpacking signals from memory ======= */

#if (defined E2E_CHECKRANGE_E2E_LE_BL) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_LE_BL already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_LE_BL) */
#if (defined E2E_CHECKRANGE_E2E_LE_U8) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_LE_U8 already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_LE_U8) */
#if (defined E2E_CHECKRANGE_E2E_LE_U16) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_LE_U16 already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_LE_U16) */
#if (defined E2E_CHECKRANGE_E2E_LE_U32) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_LE_U32 already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_LE_U32) */
#if (defined E2E_CHECKRANGE_E2E_LE_S8) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_LE_S8 already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_LE_S8) */
#if (defined E2E_CHECKRANGE_E2E_LE_S16) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_LE_S16 already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_LE_S16) */
#if (defined E2E_CHECKRANGE_E2E_LE_S32) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_LE_S32 already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_LE_S32) */
#if (defined E2E_CHECKRANGE_E2E_BE_BL) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_BE_BL already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_BE_BL) */
#if (defined E2E_CHECKRANGE_E2E_BE_U8) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_BE_U8 already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_BE_U8) */
#if (defined E2E_CHECKRANGE_E2E_BE_U16) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_BE_U16 already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_BE_U16) */
#if (defined E2E_CHECKRANGE_E2E_BE_U32) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_BE_U32 already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_BE_U32) */
#if (defined E2E_CHECKRANGE_E2E_BE_S8) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_BE_S8 already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_BE_S8) */
#if (defined E2E_CHECKRANGE_E2E_BE_S16) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_BE_S16 already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_BE_S16) */
#if (defined E2E_CHECKRANGE_E2E_BE_S32) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_BE_S32 already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_BE_S32) */
#if (defined E2E_CHECKRANGE_E2E_OP_U8N) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_OP_U8N already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_OP_U8N) */

#if (defined E2E_CHECKRANGE_E2E_BL) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_BL already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_BL) */
#if (defined E2E_CHECKRANGE_E2E_U8) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U8 already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U8) */
#if (defined E2E_CHECKRANGE_E2E_U16) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U16 already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U16) */
#if (defined E2E_CHECKRANGE_E2E_U32) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32 already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32) */
#if (defined E2E_CHECKRANGE_E2E_S8) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S8 already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S8) */
#if (defined E2E_CHECKRANGE_E2E_S16) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S16 already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S16) */
#if (defined E2E_CHECKRANGE_E2E_S32) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32 already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32) */

#if (defined E2E_CHECKRANGE_E2E_U8_8U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U8_8U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U8_8U) */
#if (defined E2E_CHECKRANGE_E2E_U8_7U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U8_7U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U8_7U) */
#if (defined E2E_CHECKRANGE_E2E_U8_6U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U8_6U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U8_6U) */
#if (defined E2E_CHECKRANGE_E2E_U8_5U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U8_5U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U8_5U) */
#if (defined E2E_CHECKRANGE_E2E_U8_4U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U8_4U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U8_4U) */
#if (defined E2E_CHECKRANGE_E2E_U8_3U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U8_3U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U8_3U) */
#if (defined E2E_CHECKRANGE_E2E_U8_2U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U8_2U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U8_2U) */
#if (defined E2E_CHECKRANGE_E2E_U8_1U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U8_1U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U8_1U) */

#if (defined E2E_CHECKRANGE_E2E_U16_16U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U16_16U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U16_16U) */
#if (defined E2E_CHECKRANGE_E2E_U16_15U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U16_15U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U16_15U) */
#if (defined E2E_CHECKRANGE_E2E_U16_14U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U16_14U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U16_14U) */
#if (defined E2E_CHECKRANGE_E2E_U16_13U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U16_13U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U16_13U) */
#if (defined E2E_CHECKRANGE_E2E_U16_12U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U16_12U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U16_12U) */
#if (defined E2E_CHECKRANGE_E2E_U16_11U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U16_11U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U16_11U) */
#if (defined E2E_CHECKRANGE_E2E_U16_10U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U16_10U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U16_10U) */
#if (defined E2E_CHECKRANGE_E2E_U16_9U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U16_9U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U16_9U) */
#if (defined E2E_CHECKRANGE_E2E_U16_8U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U16_8U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U16_8U) */
#if (defined E2E_CHECKRANGE_E2E_U16_7U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U16_7U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U16_7U) */
#if (defined E2E_CHECKRANGE_E2E_U16_6U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U16_6U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U16_6U) */
#if (defined E2E_CHECKRANGE_E2E_U16_5U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U16_5U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U16_5U) */
#if (defined E2E_CHECKRANGE_E2E_U16_4U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U16_4U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U16_4U) */
#if (defined E2E_CHECKRANGE_E2E_U16_3U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U16_3U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U16_3U) */
#if (defined E2E_CHECKRANGE_E2E_U16_2U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U16_2U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U16_2U) */
#if (defined E2E_CHECKRANGE_E2E_U16_1U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U16_1U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U16_1U) */

#if (defined E2E_CHECKRANGE_E2E_U32_32U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32_32U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32_32U) */
#if (defined E2E_CHECKRANGE_E2E_U32_31U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32_31U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32_31U) */
#if (defined E2E_CHECKRANGE_E2E_U32_30U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32_30U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32_30U) */
#if (defined E2E_CHECKRANGE_E2E_U32_29U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32_29U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32_29U) */
#if (defined E2E_CHECKRANGE_E2E_U32_28U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32_28U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32_28U) */
#if (defined E2E_CHECKRANGE_E2E_U32_27U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32_27U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32_27U) */
#if (defined E2E_CHECKRANGE_E2E_U32_26U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32_26U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32_26U) */
#if (defined E2E_CHECKRANGE_E2E_U32_25U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32_25U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32_25U) */
#if (defined E2E_CHECKRANGE_E2E_U32_24U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32_24U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32_24U) */
#if (defined E2E_CHECKRANGE_E2E_U32_23U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32_23U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32_23U) */
#if (defined E2E_CHECKRANGE_E2E_U32_22U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32_22U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32_22U) */
#if (defined E2E_CHECKRANGE_E2E_U32_21U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32_21U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32_21U) */
#if (defined E2E_CHECKRANGE_E2E_U32_20U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32_20U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32_20U) */
#if (defined E2E_CHECKRANGE_E2E_U32_19U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32_19U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32_19U) */
#if (defined E2E_CHECKRANGE_E2E_U32_18U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32_18U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32_18U) */
#if (defined E2E_CHECKRANGE_E2E_U32_17U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32_17U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32_17U) */
#if (defined E2E_CHECKRANGE_E2E_U32_16U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32_16U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32_16U) */
#if (defined E2E_CHECKRANGE_E2E_U32_15U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32_15U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32_15U) */
#if (defined E2E_CHECKRANGE_E2E_U32_14U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32_14U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32_14U) */
#if (defined E2E_CHECKRANGE_E2E_U32_13U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32_13U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32_13U) */
#if (defined E2E_CHECKRANGE_E2E_U32_12U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32_12U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32_12U) */
#if (defined E2E_CHECKRANGE_E2E_U32_11U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32_11U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32_11U) */
#if (defined E2E_CHECKRANGE_E2E_U32_10U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32_10U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32_10U) */
#if (defined E2E_CHECKRANGE_E2E_U32_9U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32_9U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32_9U) */
#if (defined E2E_CHECKRANGE_E2E_U32_8U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32_8U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32_8U) */
#if (defined E2E_CHECKRANGE_E2E_U32_7U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32_7U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32_7U) */
#if (defined E2E_CHECKRANGE_E2E_U32_6U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32_6U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32_6U) */
#if (defined E2E_CHECKRANGE_E2E_U32_5U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32_5U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32_5U) */
#if (defined E2E_CHECKRANGE_E2E_U32_4U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32_4U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32_4U) */
#if (defined E2E_CHECKRANGE_E2E_U32_3U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32_3U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32_3U) */
#if (defined E2E_CHECKRANGE_E2E_U32_2U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32_2U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32_2U) */
#if (defined E2E_CHECKRANGE_E2E_U32_1U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U32_1U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U32_1U) */

#if (defined E2E_CHECKRANGE_E2E_S8_8U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S8_8U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S8_8U) */
#if (defined E2E_CHECKRANGE_E2E_S8_7U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S8_7U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S8_7U) */
#if (defined E2E_CHECKRANGE_E2E_S8_6U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S8_6U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S8_6U) */
#if (defined E2E_CHECKRANGE_E2E_S8_5U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S8_5U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S8_5U) */
#if (defined E2E_CHECKRANGE_E2E_S8_4U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S8_4U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S8_4U) */
#if (defined E2E_CHECKRANGE_E2E_S8_3U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S8_3U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S8_3U) */
#if (defined E2E_CHECKRANGE_E2E_S8_2U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S8_2U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S8_2U) */
#if (defined E2E_CHECKRANGE_E2E_S8_1U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S8_1U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S8_1U) */

#if (defined E2E_CHECKRANGE_E2E_S16_16U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S16_16U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S16_16U) */
#if (defined E2E_CHECKRANGE_E2E_S16_15U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S16_15U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S16_15U) */
#if (defined E2E_CHECKRANGE_E2E_S16_14U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S16_14U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S16_14U) */
#if (defined E2E_CHECKRANGE_E2E_S16_13U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S16_13U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S16_13U) */
#if (defined E2E_CHECKRANGE_E2E_S16_12U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S16_12U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S16_12U) */
#if (defined E2E_CHECKRANGE_E2E_S16_11U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S16_11U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S16_11U) */
#if (defined E2E_CHECKRANGE_E2E_S16_10U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S16_10U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S16_10U) */
#if (defined E2E_CHECKRANGE_E2E_S16_9U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S16_9U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S16_9U) */
#if (defined E2E_CHECKRANGE_E2E_S16_8U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S16_8U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S16_8U) */
#if (defined E2E_CHECKRANGE_E2E_S16_7U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S16_7U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S16_7U) */
#if (defined E2E_CHECKRANGE_E2E_S16_6U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S16_6U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S16_6U) */
#if (defined E2E_CHECKRANGE_E2E_S16_5U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S16_5U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S16_5U) */
#if (defined E2E_CHECKRANGE_E2E_S16_4U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S16_4U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S16_4U) */
#if (defined E2E_CHECKRANGE_E2E_S16_3U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S16_3U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S16_3U) */
#if (defined E2E_CHECKRANGE_E2E_S16_2U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S16_2U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S16_2U) */
#if (defined E2E_CHECKRANGE_E2E_S16_1U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S16_1U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S16_1U) */

#if (defined E2E_CHECKRANGE_E2E_S32_32U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32_32U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32_32U) */
#if (defined E2E_CHECKRANGE_E2E_S32_31U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32_31U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32_31U) */
#if (defined E2E_CHECKRANGE_E2E_S32_30U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32_30U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32_30U) */
#if (defined E2E_CHECKRANGE_E2E_S32_29U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32_29U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32_29U) */
#if (defined E2E_CHECKRANGE_E2E_S32_28U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32_28U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32_28U) */
#if (defined E2E_CHECKRANGE_E2E_S32_27U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32_27U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32_27U) */
#if (defined E2E_CHECKRANGE_E2E_S32_26U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32_26U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32_26U) */
#if (defined E2E_CHECKRANGE_E2E_S32_25U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32_25U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32_25U) */
#if (defined E2E_CHECKRANGE_E2E_S32_24U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32_24U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32_24U) */
#if (defined E2E_CHECKRANGE_E2E_S32_23U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32_23U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32_23U) */
#if (defined E2E_CHECKRANGE_E2E_S32_22U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32_22U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32_22U) */
#if (defined E2E_CHECKRANGE_E2E_S32_21U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32_21U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32_21U) */
#if (defined E2E_CHECKRANGE_E2E_S32_20U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32_20U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32_20U) */
#if (defined E2E_CHECKRANGE_E2E_S32_19U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32_19U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32_19U) */
#if (defined E2E_CHECKRANGE_E2E_S32_18U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32_18U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32_18U) */
#if (defined E2E_CHECKRANGE_E2E_S32_17U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32_17U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32_17U) */
#if (defined E2E_CHECKRANGE_E2E_S32_16U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32_16U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32_16U) */
#if (defined E2E_CHECKRANGE_E2E_S32_15U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32_15U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32_15U) */
#if (defined E2E_CHECKRANGE_E2E_S32_14U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32_14U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32_14U) */
#if (defined E2E_CHECKRANGE_E2E_S32_13U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32_13U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32_13U) */
#if (defined E2E_CHECKRANGE_E2E_S32_12U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32_12U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32_12U) */
#if (defined E2E_CHECKRANGE_E2E_S32_11U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32_11U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32_11U) */
#if (defined E2E_CHECKRANGE_E2E_S32_10U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32_10U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32_10U) */
#if (defined E2E_CHECKRANGE_E2E_S32_9U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32_9U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32_9U) */
#if (defined E2E_CHECKRANGE_E2E_S32_8U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32_8U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32_8U) */
#if (defined E2E_CHECKRANGE_E2E_S32_7U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32_7U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32_7U) */
#if (defined E2E_CHECKRANGE_E2E_S32_6U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32_6U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32_6U) */
#if (defined E2E_CHECKRANGE_E2E_S32_5U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32_5U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32_5U) */
#if (defined E2E_CHECKRANGE_E2E_S32_4U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32_4U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32_4U) */
#if (defined E2E_CHECKRANGE_E2E_S32_3U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32_3U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32_3U) */
#if (defined E2E_CHECKRANGE_E2E_S32_2U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32_2U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32_2U) */
#if (defined E2E_CHECKRANGE_E2E_S32_1U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S32_1U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S32_1U) */
#if (defined E2E_CHECKRANGE_E2E_BE_F32) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_BE_F32 already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_BE_F32) */
#if (defined E2E_CHECKRANGE_E2E_LE_F32) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_LE_F32 already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_LE_F32) */
#if (defined E2E_CHECKRANGE_E2E_BE_F64) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_BE_F64 already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_BE_F64) */
#if (defined E2E_CHECKRANGE_E2E_LE_F64) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_LE_F64 already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_LE_F64) */
#if (defined E2E_CHECKRANGE_E2E_LE_U64) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_LE_U64 already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_LE_U64) */
#if (defined E2E_CHECKRANGE_UINT64_NON_FULL_RANGE) /* To prevent double declaration */
#error E2E_CHECKRANGE_UINT64_NON_FULL_RANGE already defined
#endif /* if (defined E2E_CHECKRANGE_UINT64_NON_FULL_RANGE) */
#if (defined E2E_CHECKRANGE_E2E_U64_64U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U64_64U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U64_64U) */
#if (defined E2E_CHECKRANGE_E2E_U64_56U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U64_56U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U64_56U) */
#if (defined E2E_CHECKRANGE_E2E_U64_48U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U64_48U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U64_48U) */
#if (defined E2E_CHECKRANGE_E2E_U64_40U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U64_40U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U64_40U) */
#if (defined E2E_CHECKRANGE_E2E_U64_32U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U64_32U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U64_32U) */
#if (defined E2E_CHECKRANGE_E2E_U64_24U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U64_24U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U64_24U) */
#if (defined E2E_CHECKRANGE_E2E_U64_16U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U64_16U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U64_16U) */
#if (defined E2E_CHECKRANGE_E2E_U64_8U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_U64_8U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_U64_8U) */
#if (defined E2E_CHECKRANGE_E2E_BE_U64) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_BE_U64 already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_BE_U64) */
#if (defined E2E_CHECKRANGE_E2E_LE_S64) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_LE_S64 already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_LE_S64) */
#if (defined E2E_CHECKRANGE_E2E_BE_S64) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_BE_S64 already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_BE_S64) */
#if (defined E2E_CHECKRANGE_SINT64_NON_FULL_RANGE) /* To prevent double declaration */
#error E2E_CHECKRANGE_SINT64_NON_FULL_RANGE already defined
#endif /* if (defined E2E_CHECKRANGE_SINT64_NON_FULL_RANGE) */
#if (defined E2E_CHECKRANGE_E2E_S64_64U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S64_64U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S64_64U) */
#if (defined E2E_CHECKRANGE_E2E_S64_56U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S64_56U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S64_56U) */
#if (defined E2E_CHECKRANGE_E2E_S64_48U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S64_48U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S64_48U) */
#if (defined E2E_CHECKRANGE_E2E_S64_40U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S64_40U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S64_40U) */
#if (defined E2E_CHECKRANGE_E2E_S64_32U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S64_32U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S64_32U) */
#if (defined E2E_CHECKRANGE_E2E_S64_24U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S64_24U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S64_24U) */
#if (defined E2E_CHECKRANGE_E2E_S64_16U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S64_16U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S64_16U) */
#if (defined E2E_CHECKRANGE_E2E_S64_8U) /* To prevent double declaration */
#error E2E_CHECKRANGE_E2E_S64_8U already defined
#endif /* if (defined E2E_CHECKRANGE_E2E_S64_8U) */

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_LE_BL(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_BL(bitlength,(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_LE_U8(bitlength,deMember) \
   E2E_CHECKRANGE_E2E_U8_ ## bitlength(bitlength,deMember)

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_LE_U16(bitlength,deMember) \
   E2E_CHECKRANGE_E2E_U16_ ## bitlength(bitlength,deMember)

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_LE_U32(bitlength,deMember) \
   E2E_CHECKRANGE_E2E_U32_ ## bitlength(bitlength,deMember)

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_LE_U64(bitlength,deMember) \
   E2E_CHECKRANGE_E2E_U64_ ## bitlength(bitlength,deMember)

/** \brief Helper macro for performing a range check on variable type uint64 with bitlength 64
 *
 * Note: for bitlength 64, a range check on type uint64 makes no sense
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U64_64U(bitlength,deMember)

/** \brief Helper macro for performing a range check on variable type uint64 with bitlength 56
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U64_56U(bitlength,deMember) \
   E2E_CHECKRANGE_UINT64_NON_FULL_RANGE(bitlength,deMember)

/** \brief Helper macro for performing a range check on variable type uint64 with bitlength 48
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U64_48U(bitlength,deMember) \
   E2E_CHECKRANGE_UINT64_NON_FULL_RANGE(bitlength,deMember)

/** \brief Helper macro for performing a range check on variable type uint64 with bitlength 40
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U64_40U(bitlength,deMember) \
   E2E_CHECKRANGE_UINT64_NON_FULL_RANGE(bitlength,deMember)

/** \brief Helper macro for performing a range check on variable type uint64 with bitlength 32
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U64_32U(bitlength,deMember) \
   E2E_CHECKRANGE_UINT64_NON_FULL_RANGE(bitlength,deMember)

/** \brief Helper macro for performing a range check on variable type uint64 with bitlength 24
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U64_24U(bitlength,deMember) \
   E2E_CHECKRANGE_UINT64_NON_FULL_RANGE(bitlength,deMember)

/** \brief Helper macro for performing a range check on variable type uint64 with bitlength 16
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U64_16U(bitlength,deMember) \
   E2E_CHECKRANGE_UINT64_NON_FULL_RANGE(bitlength,deMember)

/** \brief Helper macro for performing a range check on variable type uint64 with bitlength 8
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U64_8U(bitlength,deMember) \
   E2E_CHECKRANGE_UINT64_NON_FULL_RANGE(bitlength,deMember)

/** \brief Helper macro for performing a range check on a specific variable type.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 *
 * Note: Range check done within packing macro
 */
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
#define E2E_CHECKRANGE_UINT64_NON_FULL_RANGE(bitlength,deMember)                    \
do {                                                                                \
    uint8_least byte_cnt_range;                                                     \
    P2CONST(uint8, AUTOMATIC, E2E_COMP_ABS_USER(E2E_USER_PREFIX)) deMember_ptr =    \
    ((P2CONST(uint8, AUTOMATIC, E2E_COMP_ABS_USER(E2E_USER_PREFIX))) (&deMember));  \
    for (byte_cnt_range=(((bitlength)/8U));(byte_cnt_range<8U);++byte_cnt_range)    \
    {                                                                               \
       if( (uint8)(deMember_ptr[byte_cnt_range]) != 0x00U )                         \
         {                                                                          \
          E2E_RangeCheckRetVal |= E2E_RANGECHK_INVALID;                             \
         }                                                                          \
    }                                                                               \
} while ((0))
#else /* CPU_BYTE_ORDER is HIGH_BYTE_FIRST */
#define E2E_CHECKRANGE_UINT64_NON_FULL_RANGE(bitlength,deMember)                     \
 do {                                                                                \
     uint8_least byte_cnt_range;                                                     \
     P2CONST(uint8, AUTOMATIC, E2E_COMP_ABS_USER(E2E_USER_PREFIX)) deMember_ptr =    \
     ((P2CONST(uint8, AUTOMATIC, E2E_COMP_ABS_USER(E2E_USER_PREFIX))) (&deMember));  \
     for (byte_cnt_range=0U;(byte_cnt_range<(8U-((bitlength)/8U)));++byte_cnt_range) \
     {                                                                               \
        if( (uint8)(deMember_ptr[byte_cnt_range]) != 0x00U )                         \
          {                                                                          \
           E2E_RangeCheckRetVal |= E2E_RANGECHK_INVALID;                             \
          }                                                                          \
     }                                                                               \
 } while ((0))
#endif /* (CPU_BYTE_ORDER == LOW_BYTE_FIRST) */

/** \brief Helper macro for performing a range check on a specific variable type.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 *
 * Note: Range check done within packing macro
 */
#define E2E_CHECKRANGE_E2E_BE_U64(bitlength,deMember)                             \
    E2E_CHECKRANGE_E2E_LE_U64(bitlength,(deMember))

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_LE_S8(bitlength,deMember)                     \
   E2E_CHECKRANGE_E2E_S8_ ## bitlength(bitlength,deMember)

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_LE_S16(bitlength,deMember)                    \
   E2E_CHECKRANGE_E2E_S16_ ## bitlength(bitlength,deMember)

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_LE_S32(bitlength,deMember)            \
   E2E_CHECKRANGE_E2E_S32_ ## bitlength(bitlength,deMember)

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_BE_BL(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_BL((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_BE_U8(bitlength,deMember) \
   E2E_CHECKRANGE_E2E_U8_ ## bitlength(bitlength,deMember)

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_BE_U16(bitlength,deMember) \
   E2E_CHECKRANGE_E2E_U16_ ## bitlength(bitlength,deMember)

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_BE_U32(bitlength,deMember) \
   E2E_CHECKRANGE_E2E_U32_ ## bitlength(bitlength,deMember)

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_BE_S8(bitlength,deMember)                     \
   E2E_CHECKRANGE_E2E_S8_ ## bitlength(bitlength,deMember)

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_BE_S16(bitlength,deMember)                    \
   E2E_CHECKRANGE_E2E_S16_ ## bitlength(bitlength,deMember)

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_BE_S32(bitlength,deMember)            \
   E2E_CHECKRANGE_E2E_S32_ ## bitlength(bitlength,deMember)

/** \brief Helper macro for performing a range check on a specific variable type.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 *
 * Note: No range check need to be applied on opaque uint8 arrays
 */
#define E2E_CHECKRANGE_E2E_OP_U8N(bitlength,deMember)   do { } while ((0))

/** \brief Helper macro for performing a range check on a specific variable type.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 *
 * Note: No range check need to be applied on float32
 */
#define E2E_CHECKRANGE_E2E_LE_F32(bitlength,deMember)   do { } while ((0))

/** \brief Helper macro for performing a range check on a specific variable type.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 *
 * Note: No range check need to be applied on float32
 */
#define E2E_CHECKRANGE_E2E_LE_F64(bitlength,deMember)   do { } while ((0))

/** \brief Helper macro for performing a range check on a specific variable type.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 *
 * Note: No range check need to be applied on float32
 */
#define E2E_CHECKRANGE_E2E_BE_F32(bitlength,deMember)   do { } while ((0))

/** \brief Helper macro for performing a range check on a specific variable type.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 *
 * Note: No range check need to be applied on float32
 */
#define E2E_CHECKRANGE_E2E_BE_F64(bitlength,deMember)   do { } while ((0))

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_BL(bitlength,deMember) \
   (((deMember) == TRUE) || ((deMember) == FALSE))

/** \brief Helper macro for performing a range check on variable type uint8 with bitlength 8
 *
 * Note: for bitlength 8, a range check on type uint8 makes no sense
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U8_8U(bitlength,deMember)

/** \brief Helper macro for performing a range check on variable type uint8 with bitlength 7
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U8_7U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U8((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint8 with bitlength 6
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U8_6U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U8((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint8 with bitlength 5
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U8_5U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U8((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint8 with bitlength 4
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U8_4U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U8((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint8 with bitlength 3
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U8_3U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U8((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint8 with bitlength 2
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U8_2U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U8((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint8 with bitlength 1
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U8_1U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U8((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U8(bitlength,deMember) \
   ((deMember) < (uint8)(((uint8)1U)<<(bitlength)))

/** \brief Helper macro for performing a range check on variable type uint16 with bitlength 16
 *
 * Note: for bitlength 16, a range check on type uint16 makes no sense
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U16_16U(bitlength,deMember)

/** \brief Helper macro for performing a range check on variable type uint16 with bitlength 15
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U16_15U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U16((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint16 with bitlength 14
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U16_14U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U16((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint16 with bitlength 13
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U16_13U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U16((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint16 with bitlength 12
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U16_12U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U16((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint16 with bitlength 11
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U16_11U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U16((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint16 with bitlength 10
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U16_10U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U16((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint16 with bitlength 9
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U16_9U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U16((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint16 with bitlength 8
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U16_8U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U16((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint16 with bitlength 7
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U16_7U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U16((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint16 with bitlength 6
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U16_6U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U16((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint16 with bitlength 5
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U16_5U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U16((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint16 with bitlength 4
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U16_4U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U16((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint16 with bitlength 3
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U16_3U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U16((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint16 with bitlength 2
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U16_2U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U16((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint16 with bitlength 1
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U16_1U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U16((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U16(bitlength,deMember) \
   ((deMember) < (uint16)(((uint16)1U)<<(bitlength)))

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 32
 *
 * Note: for bitlength 32, a range check on type uint32 makes no sense
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32_32U(bitlength,deMember)

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 31
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32_31U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 30
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32_30U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 29
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32_29U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 28
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32_28U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 27
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32_27U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 26
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32_26U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 25
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32_25U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 24
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32_24U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 23
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32_23U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 22
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32_22U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 21
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32_21U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 20
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32_20U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 19
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32_19U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 18
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32_18U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 17
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32_17U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 16
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32_16U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 15
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32_15U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 14
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32_14U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 13
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32_13U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 12
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32_12U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 11
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32_11U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 10
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32_10U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 9
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32_9U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 8
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32_8U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 7
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32_7U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 6
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32_6U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 5
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32_5U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 4
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32_4U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 3
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32_3U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 2
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32_2U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type uint32 with bitlength 1
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32_1U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_U32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_U32(bitlength,deMember) \
   ((deMember) < (uint32)(((uint32)1U)<<(bitlength)))

/** \brief Helper macro for performing a range check on variable type sint8 with bitlength 8
 *
 * Note: for bitlength 8, a range check on type sint8 makes no sense
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S8_8U(bitlength,deMember)

/** \brief Helper macro for performing a range check on variable type sint8 with bitlength 7
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S8_7U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S8((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint8 with bitlength 6
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S8_6U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S8((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint8 with bitlength 5
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S8_5U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S8((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint8 with bitlength 4
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S8_4U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S8((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint8 with bitlength 3
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S8_3U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S8((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint8 with bitlength 2
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S8_2U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S8((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint8 with bitlength 1
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S8_1U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S8((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S8(bitlength,deMember)                     \
   (((deMember) < (sint8)(((uint8)1U)<<((bitlength)-1U))) && \
    ((deMember) >= (sint8)(((sint8)(((uint8)1U)<<((bitlength)-1U))) * -1)))

/** \brief Helper macro for performing a range check on variable type sint16 with bitlength 16
 *
 * Note: for bitlength 16, a range check on type sint16 makes no sense
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S16_16U(bitlength,deMember)

/** \brief Helper macro for performing a range check on variable type sint16 with bitlength 15
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S16_15U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S16((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint16 with bitlength 14
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S16_14U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S16((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint16 with bitlength 13
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S16_13U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S16((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint16 with bitlength 12
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S16_12U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S16((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint16 with bitlength 11
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S16_11U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S16((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint16 with bitlength 10
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S16_10U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S16((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint16 with bitlength 9
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S16_9U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S16((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint16 with bitlength 8
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S16_8U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S16((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint16 with bitlength 7
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S16_7U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S16((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint16 with bitlength 6
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S16_6U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S16((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint16 with bitlength 5
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S16_5U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S16((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint16 with bitlength 4
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S16_4U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S16((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint16 with bitlength 3
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S16_3U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S16((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint16 with bitlength 2
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S16_2U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S16((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint16 with bitlength 1
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S16_1U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S16((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S16(bitlength,deMember)                    \
   (((deMember) < (sint16)(((uint16)1U)<<((bitlength)-1U))) && \
    ((deMember) >= (sint16)(((sint16)(((uint16)1U)<<((bitlength)-1U))) * -1)))

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 32
 *
 * Note: for bitlength 32, a range check on type sint32 makes no sense
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32_32U(bitlength,deMember)

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 31
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32_31U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 30
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32_30U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 29
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32_29U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 28
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32_28U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 27
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32_27U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 26
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32_26U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 25
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32_25U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 24
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32_24U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 23
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32_23U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 22
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32_22U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 21
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32_21U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 20
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32_20U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 19
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32_19U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 18
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32_18U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 17
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32_17U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 16
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32_16U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 15
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32_15U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 14
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32_14U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 13
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32_13U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 12
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32_12U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 11
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32_11U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 10
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32_10U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 9
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32_9U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 8
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32_8U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 7
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32_7U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 6
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32_6U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 5
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32_5U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 4
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32_4U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 3
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32_3U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 2
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32_2U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on variable type sint32 with bitlength 1
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32_1U(bitlength,deMember) \
   E2E_RangeCheckRetVal |= E2E_CHECKRANGE_E2E_S32((bitlength),(deMember)) ? \
       E2E_RANGECHK_VALID : E2E_RANGECHK_INVALID

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S32(bitlength,deMember)            \
   (((deMember) < (sint32)(((uint32)1U)<<((bitlength)-1U))) && \
    ((deMember) >= (sint32)(((sint32)(((uint32)1U)<<((bitlength)-1U))) * -1)))

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_LE_S64(bitlength,deMember) \
   E2E_CHECKRANGE_E2E_S64_ ## bitlength(bitlength,deMember)

/** \brief Helper macro for performing a range check on variable type sint64 with bitlength 64
 *
 * Note: for bitlength 64, a range check on type sint64 makes no sense
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S64_64U(bitlength,deMember)

/** \brief Helper macro for performing a range check on variable type sint64 with bitlength 56
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S64_56U(bitlength,deMember) \
   E2E_CHECKRANGE_SINT64_NON_FULL_RANGE(bitlength,deMember)

/** \brief Helper macro for performing a range check on variable type sint64 with bitlength 48
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S64_48U(bitlength,deMember) \
   E2E_CHECKRANGE_SINT64_NON_FULL_RANGE(bitlength,deMember)

/** \brief Helper macro for performing a range check on variable type sint64 with bitlength 40
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S64_40U(bitlength,deMember) \
   E2E_CHECKRANGE_SINT64_NON_FULL_RANGE(bitlength,deMember)

/** \brief Helper macro for performing a range check on variable type sint64 with bitlength 32
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S64_32U(bitlength,deMember) \
   E2E_CHECKRANGE_SINT64_NON_FULL_RANGE(bitlength,deMember)

/** \brief Helper macro for performing a range check on variable type sint64 with bitlength 24
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S64_24U(bitlength,deMember) \
   E2E_CHECKRANGE_SINT64_NON_FULL_RANGE(bitlength,deMember)

/** \brief Helper macro for performing a range check on variable type sint64 with bitlength 16
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S64_16U(bitlength,deMember) \
   E2E_CHECKRANGE_SINT64_NON_FULL_RANGE(bitlength,deMember)

/** \brief Helper macro for performing a range check on variable type sint64 with bitlength 8
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 */
#define E2E_CHECKRANGE_E2E_S64_8U(bitlength,deMember) \
   E2E_CHECKRANGE_SINT64_NON_FULL_RANGE(bitlength,deMember)

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 *
 * Note: Range check done within packing macro
 */
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
#define E2E_CHECKRANGE_SINT64_NON_FULL_RANGE(bitlength,deMember)                        \
do {                                                                                    \
        uint8_least byte_cnt_range;                                                     \
        P2CONST(uint8, AUTOMATIC, E2E_COMP_ABS_USER(E2E_USER_PREFIX)) deMember_ptr =    \
        ((P2CONST(uint8, AUTOMATIC, E2E_COMP_ABS_USER(E2E_USER_PREFIX))) (&deMember));  \
        uint8 msbbit;                                                                   \
        uint8 UpperBytes = 0xFFU;                                                       \
        /* check for signed extension */                                                \
        msbbit = (uint8)(deMember_ptr[(((bitlength)/8U)-1U)] & 0x80U);                  \
        if (msbbit == 0U)                                                               \
        {                                                                               \
          UpperBytes = 0U;                                                              \
        }                                                                               \
        for (byte_cnt_range=(((bitlength)/8U));(byte_cnt_range<8U);++byte_cnt_range)    \
        {                                                                               \
           if( (uint8)(deMember_ptr[byte_cnt_range]) != UpperBytes )                    \
             {                                                                          \
              E2E_RangeCheckRetVal |= E2E_RANGECHK_INVALID;                             \
             }                                                                          \
        }                                                                               \
} while ((0))
#else /* CPU_BYTE_ORDER is HIGH_BYTE_FIRST */
#define E2E_CHECKRANGE_SINT64_NON_FULL_RANGE(bitlength,deMember)                        \
do {                                                                                    \
        uint8_least byte_cnt_range;                                                     \
        P2CONST(uint8, AUTOMATIC, E2E_COMP_ABS_USER(E2E_USER_PREFIX)) deMember_ptr =    \
        ((P2CONST(uint8, AUTOMATIC, E2E_COMP_ABS_USER(E2E_USER_PREFIX))) (&deMember));  \
        uint8 msbbit;                                                                   \
        uint8 UpperBytes = 0xFFU;                                                       \
        /* check for signed extension */                                                \
        msbbit = (uint8)(deMember_ptr[0U] & 0x80U);                                     \
        if (msbbit == 0U)                                                               \
        {                                                                               \
          UpperBytes = 0U;                                                              \
        }                                                                               \
        for (byte_cnt_range=0U;(byte_cnt_range<(8U-((bitlength)/8U)));++byte_cnt_range) \
        {                                                                               \
           if( (uint8)(deMember_ptr[byte_cnt_range]) != UpperBytes )                    \
             {                                                                          \
              E2E_RangeCheckRetVal |= E2E_RANGECHK_INVALID;                             \
             }                                                                          \
        }                                                                               \
} while ((0))
#endif /* (CPU_BYTE_ORDER == LOW_BYTE_FIRST) */

/** \brief Helper macro for performing a range check on a specific variable type
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] deMember The Data Element member where the range check shall be applied
 *
 * Note: Range check done within packing macro
 */
#define E2E_CHECKRANGE_E2E_BE_S64(bitlength,deMember)                                 \
   E2E_CHECKRANGE_E2E_LE_S64(bitlength,(deMember))

/* ============= helper macros for E2E_PACK_SIGNAL_INTO_MEMORY_BE ============= */

#if (defined E2E_PACK_BE_HELP3_1U) /* To prevent double declaration */
#error E2E_PACK_BE_HELP3_1U already defined
#endif /* if (defined E2E_PACK_BE_HELP3_1U) */

#if (defined E2E_PACK_BE_HELP3_2U) /* To prevent double declaration */
#error E2E_PACK_BE_HELP3_2U already defined
#endif /* if (defined E2E_PACK_BE_HELP3_2U) */

#if (defined E2E_PACK_BE_HELP3_3U) /* To prevent double declaration */
#error E2E_PACK_BE_HELP3_3U already defined
#endif /* if (defined E2E_PACK_BE_HELP3_3U) */

#if (defined E2E_PACK_BE_HELP3_4U) /* To prevent double declaration */
#error E2E_PACK_BE_HELP3_4U already defined
#endif /* if (defined E2E_PACK_BE_HELP3_4U) */

#if (defined E2E_PACK_BE_HELP3_5U) /* To prevent double declaration */
#error E2E_PACK_BE_HELP3_5U already defined
#endif /* if (defined E2E_PACK_BE_HELP3_5U) */

#if (defined E2E_PACK_BE_HELP2) /* To prevent double declaration */
#error E2E_PACK_BE_HELP2 already defined
#endif /* if (defined E2E_PACK_BE_HELP2) */

#if (defined E2E_PACK_BE_HELP1) /* To prevent double declaration */
#error E2E_PACK_BE_HELP1 already defined
#endif /* if (defined E2E_PACK_BE_HELP1) */

#if (defined E2E_PACK_SIGNAL_INTO_MEMORY_BE) /* To prevent double declaration */
#error E2E_PACK_SIGNAL_INTO_MEMORY_BE already defined
#endif /* if (defined E2E_PACK_SIGNAL_INTO_MEMORY_BE) */

/** \brief pack a signal into a 1-byte buffer in big endian order
 *
 * \param[in] lastbits .. number of significant bits in last byte (1 .. 8)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nValue .. signal value to write into memory
 */
#define E2E_PACK_BE_HELP3_1U(lastbits, offset, pnData, nValue )                                                       \
   do {                                                                                                               \
      (pnData)[0] = ((lastbits)==8U)                                                                                  \
      ?(((offset)==0U)?((uint8)((uint32)(nValue)&0xffU))                                                              \
        :((uint8)((pnData)[0]&(uint8)((uint32)((uint32)0xffU<<(8U-(offset)))&0xffU))                                  \
          |(uint8)(((uint32)(((uint32)(nValue)&(uint32)(0xffU>>(offset)))))&0xffU)))                                  \
          :(((offset)==0U)?((uint8)((pnData)[0]&(uint8)(0xffU>>(lastbits)))                                           \
            |(uint8)((uint32)((uint32)(nValue)<<(8U-(lastbits)))&0xffU))                                              \
              :((uint8)((pnData)[0]                                                                                   \
                &(uint8)((uint8)((uint32)((uint32)0xffU<<(8U-(offset)))&0xffU)|(uint8)(0xffU>>(lastbits))))           \
                  |(uint8)(((uint32)((uint32)((uint32)(nValue)<<(8U-(lastbits)))&(uint8)(0xffU>>(offset))))&0xffU))); \
   } while ((0))

/** \brief pack a signal into a 2-byte buffer in big endian order
 *
 * \param[in] lastbits .. number of significant bits in last byte (1 .. 8)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nValue .. signal value to write into memory
 */
#define E2E_PACK_BE_HELP3_2U(lastbits, offset, pnData, nValue )                                   \
   do {                                                                                           \
      (pnData)[1] = ((lastbits)==8U)?((uint8)((uint32)(nValue)&0xffU))                            \
        :((uint8)((pnData)[1]&(uint8)(0xffU>>(lastbits)))                                         \
          |(uint8)(((uint32)((uint32)(nValue)<<(8U-(lastbits))))&0xffU));                         \
      (pnData)[0] = ((offset)==0U)?                                                               \
        ((uint8)((uint32)((uint32)(nValue)>>(lastbits))&0xffU))                                   \
        :((uint8)((pnData)[0]&(uint8)((uint32)((uint32)0xffU<<(8U-(offset)))&0xffU))              \
          |(uint8)((uint32)((uint32)((uint32)(nValue)>>(lastbits))                                \
            &(uint32)(0xffU>>(offset)))&0xffU));                                                  \
   } while ((0))                                                                                    \

/**\brief pack a signal into a 3-byte buffer in big endian order
 *
 * \param[in] lastbits .. number of significant bits in last byte (1 .. 8)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nValue .. signal value to write into memory
 */
#define E2E_PACK_BE_HELP3_3U(lastbits, offset, pnData, nValue )                                   \
   do {                                                                                           \
      (pnData)[2] = ((lastbits)==8U)?((uint8)((uint32)(nValue)&0xffU))                            \
        :((uint8)((pnData)[2]&(uint8)(0xffU>>(lastbits)))                                         \
          |(uint8)(((uint32)((uint32)(nValue)<<(8U-(lastbits))))&0xffU));                         \
      (pnData)[1] = (uint8)(((uint32)(((uint32)(nValue))>>(lastbits)))&0xffU);                    \
      (pnData)[0] = ((offset)==0U)?                                                               \
        ((uint8)((uint32)(((uint32)(nValue))>>((lastbits)+8U))&0xffU))                            \
        :((uint8)((pnData)[0]&(uint8)((uint32)((uint32)0xffU<<(8U-(offset)))&0xffU))              \
          |(uint8)(((uint32)((uint32)(((uint32)(nValue))>>((lastbits)+8U))                        \
            &(uint32)(0xffU>>(offset))))&0xffU));                                                 \
   } while ((0))                                                                                    \

/**\brief pack a signal into a 4-byte buffer in big endian order
 *
 * \param[in] lastbits .. number of significant bits in last byte (1 .. 8)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nValue .. signal value to write into memory
 */
#define E2E_PACK_BE_HELP3_4U(lastbits, offset, pnData, nValue )                                   \
   do {                                                                                           \
      (pnData)[3] = ((lastbits)==8U)?((uint8)((uint32)(nValue)&0xffU))                            \
        :((uint8)((pnData)[3]&(uint8)(0xffU>>(lastbits)))                                         \
          |(uint8)(((uint32)((uint32)(nValue)<<(8U-(lastbits))))&0xffU));                         \
      (pnData)[2] = (uint8)(((uint32)(((uint32)(nValue))>>(lastbits)))&0xffU);                    \
      (pnData)[1] = (uint8)(((uint32)(((uint32)(nValue))>>((lastbits)+8U)))&0xffU);               \
      (pnData)[0] = ((offset)==0U)?                                                               \
        ((uint8)((uint32)(((uint32)(nValue))>>((lastbits)+16U))&0xffU))                           \
        :((uint8)((pnData)[0]&(uint8)((uint32)((uint32)0xffU<<(8U-(offset)))&0xffU))              \
          |(uint8)(((uint32)((uint32)(((uint32)(nValue))>>((lastbits)+16U))                       \
            &(uint32)(0xffU>>(offset))))&0xffU));                                                 \
   } while ((0))

/**\brief pack a signal into a 5-byte buffer in big endian order
 *
 * \param[in] lastbits .. number of significant bits in last byte (1 .. 8)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nValue .. signal value to write into memory
 */
#define E2E_PACK_BE_HELP3_5U(lastbits, offset, pnData, nValue )                                   \
   do {                                                                                           \
      (pnData)[4] = ((lastbits)==8U)?((uint8)((uint32)(nValue)&0xffU))                            \
        :((uint8)((pnData)[4]&(uint8)(0xffU>>(lastbits)))                                         \
          |(uint8)(((uint32)((uint32)(nValue)<<(8U-(lastbits))))&0xffU));                         \
      (pnData)[3] = (uint8)(((uint32)(((uint32)(nValue))>>(lastbits)))&0xffU);                    \
      (pnData)[2] = (uint8)(((uint32)(((uint32)(nValue))>>((lastbits)+8U)))&0xffU);               \
      (pnData)[1] = (uint8)(((uint32)(((uint32)(nValue))>>((lastbits)+16U)))&0xffU);              \
      (pnData)[0] = ((offset)==0U)?                                                               \
        ((uint8)((uint32)(((uint32)(nValue))>>((lastbits)+24U))&0xffU))                           \
        :((uint8)((pnData)[0]&(uint8)((uint32)((uint32)0xffU<<(8U-(offset)))&0xffU))              \
          |(uint8)(((uint32)((uint32)(((uint32)(nValue))>>((lastbits)+24U))                       \
            &(uint32)(0xffU>>(offset))))&0xffU));                                                 \
   } while ((0))

/**\brief pack a signal into a byte buffer in big endian order
 *
 * \param[in] lastbits .. number of significant bits in last byte (1 .. 8)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nValue .. signal value to write into memory
 * \param[in] nbytes .. number of affected bytes (without pre-compile calculation (1 .. 5)
 */
#define E2E_PACK_BE_HELP2(lastbits, offset, pnData, nValue, nbytes )         \
      E2E_PACK_BE_HELP3_ ## nbytes((lastbits), (offset), (pnData), (nValue)) \

/**\brief pack a signal into a byte buffer in big endian order
 *
 * \param[in] len .. length of signal in bits (1 .. 32)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nValue .. signal value to write into memory
 * \param[in] nbytes .. number of affected bytes (without pre-compile calculation (1 .. 5)
 */
#define E2E_PACK_BE_HELP1(len, offset, pnData, nValue, nbytes)              \
        E2E_PACK_BE_HELP2(((uint8)(1U+(((len)+(offset)+7U)%8U))),           \
                            (offset),                                       \
                            (pnData),                                       \
                            (nValue),                                       \
                            nbytes)

/* ============= end of helper macros for E2E_PACK_SIGNAL_INTO_MEMORY_BE ============= */

/**\brief pack a signal with length 'len' bits to bit offset 'offset' within
 * the buffer 'pnData' with 'nbytes' number of affected bytes.
 * The signals within pnData are assumed to follow the big endian convention.
 *
 * \param[in] len .. length of signal in bits (1 .. 32)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nValue .. signal value to write into memory
 * \param[in] nbytes .. number of affected bytes (without pre-compile calculation (1 .. 5)
 */
#define E2E_PACK_SIGNAL_INTO_MEMORY_BE(len, offset, pnData, nValue, nbytes) \
   do {                                                                     \
        E2E_PACK_BE_HELP1((len),                                            \
                            ((uint8)(7U-((offset)%8U))),                    \
                            (&((pnData)[(offset)/8U])),                     \
                            (nValue),                                       \
                            nbytes);                                        \
   } while ((0))

/* ============= helper macros for E2E_PACK_SIGNAL_INTO_MEMORY_LE ============= */

#if (defined E2E_PACK_LE_HELP3_1U) /* To prevent double declaration */
#error E2E_PACK_LE_HELP3_1U already defined
#endif /* if (defined E2E_PACK_LE_HELP3_1U) */

#if (defined E2E_PACK_LE_HELP3_2U) /* To prevent double declaration */
#error E2E_PACK_LE_HELP3_2U already defined
#endif /* if (defined E2E_PACK_LE_HELP3_2U) */

#if (defined E2E_PACK_LE_HELP3_3U) /* To prevent double declaration */
#error E2E_PACK_LE_HELP3_3U already defined
#endif /* if (defined E2E_PACK_LE_HELP3_3U) */

#if (defined E2E_PACK_LE_HELP3_4U) /* To prevent double declaration */
#error E2E_PACK_LE_HELP3_4U already defined
#endif /* if (defined E2E_PACK_LE_HELP3_4U) */

#if (defined E2E_PACK_LE_HELP3_5U) /* To prevent double declaration */
#error E2E_PACK_LE_HELP3_5U already defined
#endif /* if (defined E2E_PACK_LE_HELP3_5U) */

#if (defined E2E_PACK_LE_HELP2) /* To prevent double declaration */
#error E2E_PACK_LE_HELP2 already defined
#endif /* if (defined E2E_PACK_LE_HELP2) */

#if (defined E2E_PACK_LE_HELP1) /* To prevent double declaration */
#error E2E_PACK_LE_HELP1 already defined
#endif /* if (defined E2E_PACK_LE_HELP1) */

#if (defined E2E_PACK_SIGNAL_INTO_MEMORY_LE) /* To prevent double declaration */
#error E2E_PACK_SIGNAL_INTO_MEMORY_LE already defined
#endif /* if (defined E2E_PACK_SIGNAL_INTO_MEMORY_LE) */

/**\brief pack a signal into a 1-byte buffer in little endian order
 *
 * \param[in] lastbits .. number of significant bits in last byte (1 .. 8)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nValue .. signal value to write into memory
 */
#define E2E_PACK_LE_HELP3_1U(lastbits, offset, pnData, nValue )                                          \
   do {                                                                                                  \
      (pnData)[0] = ((offset)==0U)?(((lastbits)==8U)?((uint8)((uint32)(nValue)&0xffU))                   \
        :((uint8)(((uint32)((uint32)(nValue)&(uint32)(0xffU>>(8U-(lastbits)))))&0xffU)                   \
        |(uint8)((pnData)[0]&(uint8)(((uint32)((uint32)0xffU<<(lastbits)))&0xffU))))                     \
        :(((lastbits)==8U)?((uint8)((uint32)((uint32)(nValue)<<(offset))&0xffU)                          \
        |(uint8)((pnData)[0]&(uint8)(0xffU>>(8U-(offset)))))                                             \
        :((uint8)((uint32)((uint32)((uint32)(nValue)<<(offset))&(uint32)(0xffU>>(8U-(lastbits))))&0xffU) \
        |(uint8)((pnData)[0]                                                                             \
        &(uint8)((uint8)(((uint32)((uint32)0xffU<<(lastbits)))&0xffU)|(uint8)(0xffU>>(8U-(offset))))))); \
   } while ((0))

/**\brief pack a signal into a 2-byte buffer in little endian order
 *
 * \param[in] lastbits .. number of significant bits in last byte (1 .. 8)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nValue .. signal value to write into memory
 */
#define E2E_PACK_LE_HELP3_2U(lastbits, offset, pnData, nValue )                                                 \
   do {                                                                                                         \
      (pnData)[0] = ((offset)==0U)?((uint8)((uint32)(nValue)&0xffU))                                            \
        :((uint8)(((uint32)((uint32)(nValue)<<(offset)))&0xffU)                                                 \
          |(uint8)((pnData)[0]&(uint8)(0xffU>>(8U-(offset)))));                                                 \
      (pnData)[1] = ((lastbits)==8U)?((uint8)((uint32)(((uint32)(nValue))>>(8U-(offset)))&0xffU))               \
        :((uint8)((uint32)((uint32)(((uint32)(nValue))>>(8U-(offset)))&(uint32)(0xffU>>(8U-(lastbits))))&0xffU) \
          |(uint8)((pnData)[1]&(uint8)(((uint32)((uint32)0xffU<<(lastbits)))&0xffU)));                          \
   } while ((0))

/**\brief pack a signal into a 3-byte buffer in little endian order
 *
 * \param[in] lastbits .. number of significant bits in last byte (1 .. 8)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nValue .. signal value to write into memory
 */
#define E2E_PACK_LE_HELP3_3U(lastbits, offset, pnData, nValue )                                                  \
   do {                                                                                                          \
      (pnData)[0] = ((offset)==0U)?((uint8)(((uint32)(nValue))&0xffU))                                           \
        :((uint8)((uint32)(((uint32)(nValue))<<(offset))&0xffU)                                                  \
          |(uint8)((pnData)[0]&(uint8)(0xffU>>(8U-(offset)))));                                                  \
      (pnData)[1] = (uint8)(((uint32)(((uint32)(nValue))>>(8U-(offset))))&0xffU);                                \
      (pnData)[2] = ((lastbits)==8U)?((uint8)((uint32)(((uint32)(nValue))>>(16U-(offset)))&0xffU))               \
        :((uint8)((uint32)((uint32)(((uint32)(nValue))>>(16U-(offset)))&(uint32)(0xffU>>(8U-(lastbits))))&0xffU) \
          |(uint8)((pnData)[2]&(uint8)(((uint32)((uint32)0xffU<<(lastbits)))&0xffU)));                           \
   } while ((0))

/** \brief pack a signal into a 4-byte buffer in little endian order
 *
 * \param[in] lastbits .. number of significant bits in last byte (1 .. 8)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nValue .. signal value to write into memory
 */
#define E2E_PACK_LE_HELP3_4U(lastbits, offset, pnData, nValue )                                                  \
   do {                                                                                                          \
      (pnData)[0] = ((offset)==0U)?((uint8)(((uint32)(nValue))&0xffU))                                           \
        :((uint8)((uint32)(((uint32)(nValue))<<(offset))&0xffU)                                                  \
          |(uint8)((pnData)[0]&(uint8)(0xffU>>(8U-(offset)))));                                                  \
      (pnData)[1] = (uint8)(((uint32)(((uint32)(nValue))>>(8U-(offset))))&0xffU);                                \
      (pnData)[2] = (uint8)(((uint32)(((uint32)(nValue))>>(16U-(offset))))&0xffU);                               \
      (pnData)[3] = ((lastbits)==8U)?((uint8)((uint32)(((uint32)(nValue))>>(24U-(offset)))&0xffU))               \
        :((uint8)((uint32)((uint32)(((uint32)(nValue))>>(24U-(offset)))&(uint32)(0xffU>>(8U-(lastbits))))&0xffU) \
          |(uint8)((pnData)[3]&(uint8)(((uint32)((uint32)0xffU<<(lastbits)))&0xffU)));                           \
   } while ((0))

/** \brief pack a signal into a 5-byte buffer in little endian order
 *
 * \param[in] lastbits .. number of significant bits in last byte (1 .. 8)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nValue .. signal value to write into memory
 */
#define E2E_PACK_LE_HELP3_5U(lastbits, offset, pnData, nValue )                                                 \
   do {                                                                                                         \
     (pnData)[0] = ((offset)==0U)?((uint8)(((uint32)(nValue))&0xffU))                                           \
       :((uint8)((uint32)(((uint32)(nValue))<<(offset))&0xffU)                                                  \
         |(uint8)((pnData)[0]&(uint8)(0xffU>>(8U-(offset)))));                                                  \
     (pnData)[1] = (uint8)(((uint32)(((uint32)(nValue))>>(8U-(offset))))&0xffU);                                \
     (pnData)[2] = (uint8)(((uint32)(((uint32)(nValue))>>(16U-(offset))))&0xffU);                               \
     (pnData)[3] = (uint8)(((uint32)(((uint32)(nValue))>>(24U-(offset))))&0xffU);                               \
     (pnData)[4] = ((lastbits)==8U)?((uint8)((uint32)(((uint32)(nValue))>>(32U-(offset)))&0xffU))               \
       :((uint8)((uint32)((uint32)(((uint32)(nValue))>>(32U-(offset)))&(uint32)(0xffU>>(8U-(lastbits))))&0xffU) \
         |(uint8)((pnData)[4]&(uint8)(((uint32)((uint32)0xffU<<(lastbits)))&0xffU)));                           \
   } while ((0))

/**\brief pack a signal into a byte buffer in little endian order
 *
 * \param[in] lastbits .. number of significant bits in last byte (1 .. 8)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nValue .. signal value to write into memory
 * \param[in] nbytes .. number of affected bytes (without pre-compile calculation (1 .. 5)
 */
#define E2E_PACK_LE_HELP2(lastbits, offset, pnData, nValue, nbytes )        \
      E2E_PACK_LE_HELP3_ ## nbytes((lastbits), (offset), (pnData), (nValue))


/*
 * pack the signal into a byte buffer
 * \param[in] len .. length of signal in bits (1 .. 32)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nValue .. signal value to write into memory
 * \param[in] nbytes .. number of affected bytes (without pre-compile calculation (1 .. 5)
 */
#define E2E_PACK_LE_HELP1(len, offset, pnData, nValue, nbytes)              \
        E2E_PACK_LE_HELP2(((uint8)(1U+(((len)+(offset)+7U)%8U))),           \
                            (offset),                                       \
                            (pnData),                                       \
                            (nValue),                                       \
                            nbytes)

/* ============= end of helper macros for E2E_PACK_SIGNAL_INTO_MEMORY_LE =============*/

/**\brief pack a signal with length 'len' bits to bit offset 'offset' within
 * the buffer 'pnData' with 'nbytes' number of affected bytes.
 * The signals within pnData are assumed to follow the little endian convention.
 *
 * \param[in] len .. length of signal in bits (1 .. 32)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nValue .. signal value to write into memory
 * \param[in] nbytes .. number of affected bytes (without pre-compile calculation (1 .. 5)
 */
#define E2E_PACK_SIGNAL_INTO_MEMORY_LE(len, offset, pnData, nValue, nbytes) \
   do {                                                                     \
        E2E_PACK_LE_HELP1((len),                                            \
                            ((uint8)((offset)%8U)),                         \
                            (&((pnData)[(offset)/8U])),                     \
                            (nValue),                                       \
                            nbytes);                                        \
   } while ((0))

/* ============= helper macros for unpacking signals from memory ============= */

#if (defined E2E_UNPACKSIG) /* To prevent double declaration */
#error E2E_UNPACKSIG already defined
#endif /* if (defined E2E_UNPACKSIG) */
#if (defined E2E_EXTRACTSIGTO) /* To prevent double declaration */
#error E2E_EXTRACTSIGTO already defined
#endif /* if (defined E2E_EXTRACTSIGTO) */
#if (defined E2E_EXTRACTSIGTO_E2E_LE_BL) /* To prevent double declaration */
#error E2E_EXTRACTSIGTO_E2E_LE_BL already defined
#endif /* if (defined E2E_EXTRACTSIGTO_E2E_LE_BL) */
#if (defined E2E_EXTRACTSIGTO_E2E_LE_U8) /* To prevent double declaration */
#error E2E_EXTRACTSIGTO_E2E_LE_U8 already defined
#endif /* if (defined E2E_EXTRACTSIGTO_E2E_LE_U8) */
#if (defined E2E_EXTRACTSIGTO_E2E_LE_U16) /* To prevent double declaration */
#error E2E_EXTRACTSIGTO_E2E_LE_U16 already defined
#endif /* if (defined E2E_EXTRACTSIGTO_E2E_LE_U16) */
#if (defined E2E_EXTRACTSIGTO_E2E_LE_U32) /* To prevent double declaration */
#error E2E_EXTRACTSIGTO_E2E_LE_U32 already defined
#endif /* if (defined E2E_EXTRACTSIGTO_E2E_LE_U32) */
#if (defined E2E_EXTRACTSIGTO_E2E_LE_S8) /* To prevent double declaration */
#error E2E_EXTRACTSIGTO_E2E_LE_S8 already defined
#endif /* if (defined E2E_EXTRACTSIGTO_E2E_LE_S8) */
#if (defined E2E_EXTRACTSIGTO_E2E_LE_S16) /* To prevent double declaration */
#error E2E_EXTRACTSIGTO_E2E_LE_S16 already defined
#endif /* if (defined E2E_EXTRACTSIGTO_E2E_LE_S16) */
#if (defined E2E_EXTRACTSIGTO_E2E_LE_S32) /* To prevent double declaration */
#error E2E_EXTRACTSIGTO_E2E_LE_S32 already defined
#endif /* if (defined E2E_EXTRACTSIGTO_E2E_LE_S32) */
#if (defined E2E_EXTRACTSIGTO_E2E_BE_BL) /* To prevent double declaration */
#error E2E_EXTRACTSIGTO_E2E_BE_BL already defined
#endif /* if (defined E2E_EXTRACTSIGTO_E2E_BE_BL) */
#if (defined E2E_EXTRACTSIGTO_E2E_BE_U8) /* To prevent double declaration */
#error E2E_EXTRACTSIGTO_E2E_BE_U8 already defined
#endif /* if (defined E2E_EXTRACTSIGTO_E2E_BE_U8) */
#if (defined E2E_EXTRACTSIGTO_E2E_BE_U16) /* To prevent double declaration */
#error E2E_EXTRACTSIGTO_E2E_BE_U16 already defined
#endif /* if (defined E2E_EXTRACTSIGTO_E2E_BE_U16) */
#if (defined E2E_EXTRACTSIGTO_E2E_BE_U32) /* To prevent double declaration */
#error E2E_EXTRACTSIGTO_E2E_BE_U32 already defined
#endif /* if (defined E2E_EXTRACTSIGTO_E2E_BE_U32) */
#if (defined E2E_EXTRACTSIGTO_E2E_BE_S8) /* To prevent double declaration */
#error E2E_EXTRACTSIGTO_E2E_BE_S8 already defined
#endif /* if (defined E2E_EXTRACTSIGTO_E2E_BE_S8) */
#if (defined E2E_EXTRACTSIGTO_E2E_BE_S16) /* To prevent double declaration */
#error E2E_EXTRACTSIGTO_E2E_BE_S16 already defined
#endif /* if (defined E2E_EXTRACTSIGTO_E2E_BE_S16) */
#if (defined E2E_EXTRACTSIGTO_E2E_BE_S32) /* To prevent double declaration */
#error E2E_EXTRACTSIGTO_E2E_BE_S32 already defined
#endif /* if (defined E2E_EXTRACTSIGTO_E2E_BE_S32) */
#if (defined E2E_EXTRACTSIGTO_E2E_OP_U8N) /* To prevent double declaration */
#error E2E_EXTRACTSIGTO_E2E_OP_U8N already defined
#endif /* if (defined E2E_EXTRACTSIGTO_E2E_OP_U8N) */
#if (defined E2E_EXTRACTSIGTO_E2E_LE_F32) /* To prevent double declaration */
#error E2E_EXTRACTSIGTO_E2E_LE_F32 already defined
#endif /* if (defined E2E_EXTRACTSIGTO_E2E_LE_F32) */
#if (defined E2E_EXTRACTSIGTO_E2E_BE_F32) /* To prevent double declaration */
#error E2E_EXTRACTSIGTO_E2E_BE_F32 already defined
#endif /* if (defined E2E_EXTRACTSIGTO_E2E_BE_F32) */
#if (defined E2E_EXTRACTSIGTO_E2E_LE_F64) /* To prevent double declaration */
#error E2E_EXTRACTSIGTO_E2E_LE_F64 already defined
#endif /* if (defined E2E_EXTRACTSIGTO_E2E_LE_F64) */
#if (defined E2E_EXTRACTSIGTO_E2E_BE_F64) /* To prevent double declaration */
#error E2E_EXTRACTSIGTO_E2E_BE_F64 already defined
#endif /* if (defined E2E_EXTRACTSIGTO_E2E_BE_F64) */
#if (defined E2E_EXTRACTSIGTO_E2E_LE_U64) /* To prevent double declaration */
#error E2E_EXTRACTSIGTO_E2E_LE_U64 already defined
#endif /* if (defined E2E_EXTRACTSIGTO_E2E_LE_U64) */
#if (defined E2E_EXTRACTSIGTO_E2E_BE_U64) /* To prevent double declaration */
#error E2E_EXTRACTSIGTO_E2E_BE_U64 already defined
#endif /* if (defined E2E_EXTRACTSIGTO_E2E_BE_U64) */
#if (defined E2E_EXTRACTSIGTO_E2E_LE_S64) /* To prevent double declaration */
#error E2E_EXTRACTSIGTO_E2E_LE_S64 already defined
#endif /* if (defined E2E_EXTRACTSIGTO_E2E_LE_S64) */
#if (defined E2E_EXTRACTSIGTO_E2E_BE_S64) /* To prevent double declaration */
#error E2E_EXTRACTSIGTO_E2E_BE_S64 already defined
#endif /* if (defined E2E_EXTRACTSIGTO_E2E_BE_S64) */

#if (defined E2E_EXT_BITS) /* To prevent double declaration */
#error E2E_EXT_BITS already defined
#endif /* if (defined E2E_EXT_BITS) */

#define E2E_EXT_BITS(datatype, bitlength) \
    ((((uint16)sizeof(datatype))*8U)-(bitlength))

#if (defined E2E_EXTEND_SIGNED_SIGNAL) /* To prevent double declaration */
#error E2E_EXTEND_SIGNED_SIGNAL already defined
#endif /* if (defined E2E_EXTEND_SIGNED_SIGNAL) */

#define E2E_EXTEND_SIGNED_SIGNAL(value, datatype, bitlength)                   \
    ((1U==(bitlength)) ? (datatype)((datatype)(value) * -1) :                  \
    (((((uint16)sizeof(datatype))*8U)==(bitlength)) ? ((datatype)(value)) :    \
    ((datatype)(((datatype)((value) << E2E_EXT_BITS(datatype, (bitlength))))/  \
    (datatype)(((sint32)(1)) << E2E_EXT_BITS(datatype, (bitlength)))))))

/** \brief Macro for unpacking a Data Element member from a serialized data memory.
 *
 * \param[in] memptr Pointer to the memory which contains the serialized Data Element.
 * \param[in] structmember Member of the Data Element for storing the extracted data.
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] sigtype Signal type of the Data Element member in the serialized memory.
 *            Shall be one of: E2E_[LE|BE]_[BL|U8|U16|U32|U64|F32|F64|S8|S16|S32|S64] or E2E_OP_U8N
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 *
 * Note:
 * No range checks for float32 signal type.
 */
#define E2E_UNPACKSIG(memptr,structmember,bitpos,bitlength,sigtype,nbytes) \
   E2E_EXTRACTSIGTO((memptr),(bitpos),(bitlength),        \
                      sigtype,nbytes,(structmember))

/** \brief Helper macro for unpacking a Data Element member from a serialized data memory.
 *
 * \param[in] appData The Data Element for storing the extracted data.
 * \param[in] memptr Pointer to the memory which contains the serialized Data Element.
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] sigtype Signal type of the Data Element member in the serialized memory.
 *            Shall be one of: E2E_[LE|BE]_[BL|U8|U16|U32|U64|F32|F64|S8|S16|S32|S64] or E2E_OP_U8N
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 * \param[in] target Variable where the extracted data will be written to.
 *
 * Note: No range checks for float32 signal type
 */
#define E2E_EXTRACTSIGTO(memptr,bitpos,bitlength,sigtype,nbytes,target) \
   E2E_EXTRACTSIGTO_ ## sigtype((bitlength),(bitpos),(memptr),nbytes,(target));

/** \brief Helper macro for unpacking a boolean variable from a serialized data memory
 * in Little Endian Order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which contains the serialized Data Element.
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 * \param[in] target Variable where the extracted data will be written to.
 */
#define E2E_EXTRACTSIGTO_E2E_LE_BL(bitlength,bitpos,memptr,nbytes,target) \
   target = (boolean)E2E_EXTRACT_SIGNAL_FROM_MEMORY_LE((bitlength),(bitpos),(memptr),nbytes)

/** \brief Helper macro for unpacking a uint8 variable from a serialized data memory
 * in Little Endian Order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which contains the serialized Data Element.
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 * \param[in] target Variable where the extracted data will be written to.
 */
#define E2E_EXTRACTSIGTO_E2E_LE_U8(bitlength,bitpos,memptr,nbytes,target) \
   target = (uint8)(E2E_EXTRACT_SIGNAL_FROM_MEMORY_LE((bitlength),(bitpos),(memptr),nbytes)&0xffU)

/** \brief Helper macro for unpacking a uint16 variable from a serialized data memory
 * in Little Endian Order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which contains the serialized Data Element.
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 * \param[in] target Variable where the extracted data will be written to.
 */
#define E2E_EXTRACTSIGTO_E2E_LE_U16(bitlength,bitpos,memptr,nbytes,target) \
   target = (uint16)(E2E_EXTRACT_SIGNAL_FROM_MEMORY_LE((bitlength),(bitpos),(memptr),nbytes)&0xffffU)

/** \brief Helper macro for unpacking a uint32 variable from a serialized data memory
 * in Little Endian Order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which contains the serialized Data Element.
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 * \param[in] target Variable where the extracted data will be written to.
 */
#define E2E_EXTRACTSIGTO_E2E_LE_U32(bitlength,bitpos,memptr,nbytes,target) \
   target = (uint32)E2E_EXTRACT_SIGNAL_FROM_MEMORY_LE((bitlength),(bitpos),(memptr),nbytes)

/** \brief Helper macro for unpacking a uint64 variable from a serialized data memory
 * in Little Endian Order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which contains the serialized Data Element.
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 * \param[in] target Variable where the extracted data will be written to.
 *
 * Note: uint64 unpacking includes the following limitations:
 *      - data shall be byte aligned: bitpos lsb % 8 == 0
 *      - bitlength shall be a multiple of 8
 *      - bitlength shall have constancy to nbytes
 */
#define E2E_EXTRACTSIGTO_E2E_LE_U64(bitlength,bitpos,memptr,nbytes,target)                          \
   do {                                                                                             \
      uint8_least byte_cnt;                                                                         \
      /* E2E_LE_U64 bitpos is lsb and shall be byte aligned AND
       * bitlength constancy to nbytes AND
       * bitlength multiple of 8 AND
       * nbytes shall be smaller than or equal to 8 bytes AND
       * nbytes shall be greater than 0 bytes */                                                    \
      E2E_STATIC_ASSERT( (((bitpos) % 8U) == 0U) &&                                                 \
                         (((bitlength) / 8U) == (nbytes)) &&                                        \
                         (((bitlength) % 8U) == 0U) &&                                              \
                         ((nbytes) <= 8U) &&                                                        \
                         ((nbytes) > 0U) );                                                         \
      (target) = 0x0000000000000000ULL;                                                             \
      for (byte_cnt=0U; byte_cnt<(nbytes); ++byte_cnt)                                              \
      {                                                                                             \
        (target) = (uint64)(((target) << 8U) | (memptr)[((bitpos / 8U) + (nbytes-1U)) - byte_cnt]); \
      }                                                                                             \
   } while ((0))

/** \brief Helper macro for unpacking a uint32 variable from a serialized data memory
 * in Little Endian Order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which contains the serialized Data Element.
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 * \param[in] target Variable where the extracted data will be written to.
 *
 * Note: float32 data type mapping to uint32
 *       float32 unpacking includes the following additional limitations:
 *      - bitlength shall be 32U
 *      - nbytes shall be 4U or 5U (unaligned)
 */
#define E2E_EXTRACTSIGTO_E2E_LE_F32(bitlength,bitpos,memptr,nbytes,target) \
        /* IEEE754 memory encoding for single precision */ \
        { E2E_STATIC_ASSERT_FLOAT( ((bitlength) == 32U) && \
                               ( ((nbytes) == 4U) || \
                                 ((nbytes) == 5U) ) ); } \
        E2E_EXTRACTSIGTO_E2E_LE_U32((bitlength),(bitpos),(memptr),nbytes, \
        (* (P2VAR(uint32, AUTOMATIC, E2E_COMP_ABS_USER(E2E_USER_PREFIX)))(&target)))

 /** \brief Helper macro for unpacking a uint64 variable from a serialized data memory
  * in Little Endian Order.
  *
  * \param[in] bitlength Bit length of the Data Element member in the serialized memory
  * \param[in] bitpos Bit position of the Data Element member in the serialized memory
  * \param[in] memptr Pointer to the memory which contains the serialized Data Element.
  * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
  * \param[in] target Variable where the extracted data will be written to.
  *
 * Note: float64 data type mapping to uint64.
 *       uint64 unpacking includes the following limitations:
 *      - data shall be byte aligned: bitpos lsb % 8 == 0
 *      - bitlength shall be a multiple of 8
 *      - bitlength shall have constancy to nbytes
 *      float64 unpacking includes the following additional limitations:
 *      - bitlength shall be 64U
 *      - nbytes shall be 8U
  */
 #define E2E_EXTRACTSIGTO_E2E_LE_F64(bitlength,bitpos,memptr,nbytes,target) \
         /* IEEE754 memory encoding for single precision */ \
         { E2E_STATIC_ASSERT_FLOAT( ((bitlength) == 64U) && \
                                  ((nbytes) == 8U) ); } \
         E2E_EXTRACTSIGTO_E2E_LE_U64((bitlength),(bitpos),(memptr),nbytes, \
         (* (P2VAR(uint64, AUTOMATIC, E2E_COMP_ABS_USER(E2E_USER_PREFIX)))(&target)))

/** \brief Helper macro for unpacking a sint8 variable from a serialized data memory
 * in Little Endian Order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which contains the serialized Data Element.
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 * \param[in] target Variable where the extracted data will be written to.
 */
#define E2E_EXTRACTSIGTO_E2E_LE_S8(bitlength,bitpos,memptr,nbytes,target)                     \
   target = (sint8)E2E_EXTEND_SIGNED_SIGNAL(                                                  \
      (uint8)(E2E_EXTRACT_SIGNAL_FROM_MEMORY_LE((bitlength),(bitpos),(memptr),nbytes)&0xffU), \
      sint8,(bitlength))

/** \brief Helper macro for unpacking a sint16 variable from a serialized data memory
 * in Little Endian Order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which contains the serialized Data Element.
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 * \param[in] target Variable where the extracted data will be written to.
 */
#define E2E_EXTRACTSIGTO_E2E_LE_S16(bitlength,bitpos,memptr,nbytes,target)                      \
   target = (sint16)E2E_EXTEND_SIGNED_SIGNAL(                                                   \
      (uint16)(E2E_EXTRACT_SIGNAL_FROM_MEMORY_LE((bitlength),(bitpos),(memptr),nbytes)&0xffffU),\
      sint16,(bitlength))

/** \brief Helper macro for unpacking a sint32 variable from a serialized data memory
 * in Little Endian Order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which contains the serialized Data Element.
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 * \param[in] target Variable where the extracted data will be written to.
 */
#define E2E_EXTRACTSIGTO_E2E_LE_S32(bitlength,bitpos,memptr,nbytes,target)             \
   target = (sint32)E2E_EXTEND_SIGNED_SIGNAL(                                          \
      (uint32)E2E_EXTRACT_SIGNAL_FROM_MEMORY_LE((bitlength),(bitpos),(memptr),nbytes), \
      sint32,(bitlength))

/** \brief Helper macro for unpacking a sint64 variable from a serialized data memory
 * in Little Endian Order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which contains the serialized Data Element.
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 * \param[in] target Variable where the extracted data will be written to.
 *
 * Note: sint64 unpacking includes the following limitations:
 *      - data shall be byte aligned: bitpos lsb % 8 == 0
 *      - bitlength shall be a multiple of 8
 *      - bitlength shall have constancy to nbytes
 */
#define E2E_EXTRACTSIGTO_E2E_LE_S64(bitlength,bitpos,memptr,nbytes,target)                              \
   do {                                                                                                 \
      uint8_least byte_cnt;                                                                             \
      /* E2E_LE_S64 bitpos is lsb and shall be byte aligned AND
       * bitlength constancy to nbytes AND
       * bitlength multiple of 8 AND
       * nbytes shall be smaller than or equal to 8 bytes AND
       * nbytes shall be greater than 0 */                                                              \
      E2E_STATIC_ASSERT( (((bitpos) % 8U) == 0U) &&                                                     \
                         (((bitlength) / 8U) == (nbytes)) &&                                            \
                         (((bitlength) % 8U) == 0U) &&                                                  \
                         ((nbytes) <= 8U) &&                                                            \
                         ((nbytes) > 0U) );                                                             \
      /* check for signed extastion */                                                                  \
      if( (((memptr)[(((bitpos) / 8U) + ((nbytes)-1U))]) & 0x80U) == 0x80U )                            \
      {                                                                                                 \
        (target) = 0x7FFFFFFFFFFFFFFFLL;                                                                \
      }                                                                                                 \
      else                                                                                              \
      {                                                                                                 \
        (target) = 0x0000000000000000ULL;                                                               \
      }                                                                                                 \
      for (byte_cnt=0U; byte_cnt<(nbytes); ++byte_cnt)                                                  \
      {                                                                                                 \
        (target) = (sint64)((((target) & 0xFFFFFFFFFFFFFFULL) << 8U)                                    \
            | (memptr)[(((bitpos) / 8U) + ((nbytes)-1U)) - byte_cnt]);                                  \
      }                                                                                                 \
   } while ((0))

/** \brief Helper macro for unpacking a boolean variable from a serialized data memory
 * in Big Endian Order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which contains the serialized Data Element.
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 * \param[in] target Variable where the extracted data will be written to.
 */
#define E2E_EXTRACTSIGTO_E2E_BE_BL(bitlength,bitpos,memptr,nbytes,target) \
   target = (boolean)E2E_EXTRACT_SIGNAL_FROM_MEMORY_BE((bitlength),(bitpos),(memptr),nbytes)

/** \brief Helper macro for unpacking a uint8 variable from a serialized data memory
 * in Big Endian Order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which contains the serialized Data Element.
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 * \param[in] target Variable where the extracted data will be written to.
 */
#define E2E_EXTRACTSIGTO_E2E_BE_U8(bitlength,bitpos,memptr,nbytes,target) \
   target = (uint8)(E2E_EXTRACT_SIGNAL_FROM_MEMORY_BE((bitlength),(bitpos),(memptr),nbytes)&0xffU)

/** \brief Helper macro for unpacking a uint16 variable from a serialized data memory
 * in Big Endian Order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which contains the serialized Data Element.
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 * \param[in] target Variable where the extracted data will be written to.
 */
#define E2E_EXTRACTSIGTO_E2E_BE_U16(bitlength,bitpos,memptr,nbytes,target) \
   target = (uint16)(E2E_EXTRACT_SIGNAL_FROM_MEMORY_BE((bitlength),(bitpos),(memptr),nbytes)&0xffffU)

/** \brief Helper macro for unpacking a uint32 variable from a serialized data memory
 * in Big Endian Order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which contains the serialized Data Element.
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 * \param[in] target Variable where the extracted data will be written to.
 */
#define E2E_EXTRACTSIGTO_E2E_BE_U32(bitlength,bitpos,memptr,nbytes,target) \
   target = (uint32)E2E_EXTRACT_SIGNAL_FROM_MEMORY_BE((bitlength),(bitpos),(memptr),nbytes)

/** \brief Macro for unpacking a uint64 variable into a serialized data memory in Big Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which contains the serialized Data Element.
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 * \param[in] target Variable where the extracted data will be written to.
 *
 * Note: uint64 unpacking includes the following limitations:
 *      - data shall be byte aligned: bitpos msb % 8 == 7
 *      - bitlength shall be a multiple of 8
 *      - bitlength shall have constancy to nbytes
 */
#define E2E_EXTRACTSIGTO_E2E_BE_U64(bitlength,bitpos,memptr,nbytes,target)               \
    do {                                                                                 \
       uint8_least byte_cnt;                                                             \
       /* E2E_BE_U64 bitpos is msb and shall be byte aligned AND
        * bitlength constancy to nbytes AND
        * bitlength multiple of 8 AND
       * nbytes shall be smaller than or equal to 8 bytes AND
       * nbytes shall be greater than 0 */                                               \
       E2E_STATIC_ASSERT( (((bitpos) % 8U) == 7U) &&                                     \
                          (((bitlength) / 8U) == (nbytes)) &&                            \
                          (((bitlength) % 8U) == 0U) &&                                  \
                          ((nbytes) <= 8U) &&                                            \
                          ((nbytes) > 0U) );                                             \
       (target) = 0x0000000000000000ULL;                                                 \
       for (byte_cnt=0U; byte_cnt<(nbytes); ++byte_cnt)                                  \
       {                                                                                 \
         (target) = (uint64)(((target) << 8U) | (memptr)[(((bitpos) / 8U) + byte_cnt)]); \
       }                                                                                 \
    } while ((0))

/** \brief Helper macro for unpacking a sint8 variable from a serialized data memory
 * in Big Endian Order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which contains the serialized Data Element.
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 * \param[in] target Variable where the extracted data will be written to.
 */
#define E2E_EXTRACTSIGTO_E2E_BE_S8(bitlength,bitpos,memptr,nbytes,target)                     \
   target = (sint8)E2E_EXTEND_SIGNED_SIGNAL(                                                  \
      (uint8)(E2E_EXTRACT_SIGNAL_FROM_MEMORY_BE((bitlength),(bitpos),(memptr),nbytes)&0xffU), \
      sint8,(bitlength))

/** \brief Helper macro for unpacking a sint16 variable from a serialized data memory
 * in v Endian Order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which contains the serialized Data Element.
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 * \param[in] target Variable where the extracted data will be written to.
 */
#define E2E_EXTRACTSIGTO_E2E_BE_S16(bitlength,bitpos,memptr,nbytes,target)                      \
   target = (sint16)E2E_EXTEND_SIGNED_SIGNAL(                                                   \
      (uint16)(E2E_EXTRACT_SIGNAL_FROM_MEMORY_BE((bitlength),(bitpos),(memptr),nbytes)&0xffffU),\
      sint16,(bitlength))

/** \brief Helper macro for unpacking a sint32 variable from a serialized data memory
 * in Big Endian Order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which contains the serialized Data Element.
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 * \param[in] target Variable where the extracted data will be written to.
 */
#define E2E_EXTRACTSIGTO_E2E_BE_S32(bitlength,bitpos,memptr,nbytes,target)             \
   target = (sint32)E2E_EXTEND_SIGNED_SIGNAL(                                          \
      (uint32)E2E_EXTRACT_SIGNAL_FROM_MEMORY_BE((bitlength),(bitpos),(memptr),nbytes), \
      sint32,(bitlength))

/** \brief Macro for unpacking a sint64 variable into a serialized data memory in Big Endian order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which contains the serialized Data Element.
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 * \param[in] target Variable where the extracted data will be written to.
 *
 * Note: sint64 unpacking includes the following limitations:
 *      - data shall be byte aligned: bitpos msb % 8 == 7
 *      - bitlength shall be a multiple of 8
 *      - bitlength shall have constancy to nbytes
 */
#define E2E_EXTRACTSIGTO_E2E_BE_S64(bitlength,bitpos,memptr,nbytes,target)             \
    do {                                                                               \
       uint8_least byte_cnt;                                                           \
       /* E2E_BE_S64 bitpos is msb and shall be byte aligned AND
        * bitlength constancy to nbytes AND
        * bitlength multiple of 8 AND
        * nbytes shall be smaller than or equal to 8 bytes AND
        * nbytes shall be greater than 0 bytes */                                      \
       E2E_STATIC_ASSERT( (((bitpos) % 8U) == 7U) &&                                   \
                          (((bitlength) / 8U) == (nbytes)) &&                          \
                          (((bitlength) % 8U) == 0U) &&                                \
                          ((nbytes) <= 8U) &&                                          \
                          ((nbytes) > 0U) );                                           \
       if( (((memptr)[((bitpos) / 8U)]) & 0x80U) == 0x80U )                            \
       {                                                                               \
         (target) = 0x7FFFFFFFFFFFFFFFLL;                                              \
       }                                                                               \
       else                                                                            \
       {                                                                               \
         (target) = 0x0000000000000000ULL;                                             \
       }                                                                               \
       for (byte_cnt=0U; byte_cnt<(nbytes); ++byte_cnt)                                \
       {                                                                               \
         (target) = (sint64)((((target) & 0xFFFFFFFFFFFFFFULL) << 8U)                  \
             | (memptr)[((bitpos) / 8U) + byte_cnt]);                                  \
       }                                                                               \
    } while ((0))


/** \brief Helper macro for unpacking a float32 variable from a serialized data memory
 * in Big Endian Order.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which contains the serialized Data Element.
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 * \param[in] target Variable where the extracted data will be written to.
 *
 * Note: float32 data type mapping to uint32
 *       float32 unpacking includes the following additional limitations:
 *      - bitlength shall be 32U
 *      - nbytes shall be 4U
 */
#define E2E_EXTRACTSIGTO_E2E_BE_F32(bitlength,bitpos,memptr,nbytes,target) \
        /* IEEE754 memory encoding for single precision */ \
        { E2E_STATIC_ASSERT_FLOAT( ((bitlength) == 32U) && \
                               ( ((nbytes) == 4U) || \
                                 ((nbytes) == 5U) ) ); } \
        E2E_EXTRACTSIGTO_E2E_BE_U32((bitlength),(bitpos),(memptr),nbytes, \
        (* (P2VAR(uint32, AUTOMATIC, E2E_COMP_ABS_USER(E2E_USER_PREFIX)))(&target)))

 /** \brief Helper macro for unpacking a float64 variable from a serialized data memory
  * in Big Endian Order.
  *
  * \param[in] bitlength Bit length of the Data Element member in the serialized memory
  * \param[in] bitpos Bit position of the Data Element member in the serialized memory
  * \param[in] memptr Pointer to the memory which contains the serialized Data Element.
  * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
  * \param[in] target Variable where the extracted data will be written to.
  *
 * Note: float64 data type mapping to uint64.
 *       uint64 unpacking includes the following limitations:
 *      - data shall be byte aligned: bitpos lsb % 8 == 0
 *      - bitlength shall be a multiple of 8
 *      - bitlength shall have constancy to nbytes
 *      float64 unpacking includes the following additional limitations:
 *      - bitlength shall be 64U
 *      - nbytes shall be 8U
  */
 #define E2E_EXTRACTSIGTO_E2E_BE_F64(bitlength,bitpos,memptr,nbytes,target) \
         /* IEEE754 memory encoding for double precision */ \
         { E2E_STATIC_ASSERT_FLOAT( ((bitlength) == 64U) && \
                                  ((nbytes) == 8U) ); } \
         E2E_EXTRACTSIGTO_E2E_BE_U64((bitlength),(bitpos),(memptr),nbytes, \
         (* (P2VAR(uint64, AUTOMATIC, E2E_COMP_ABS_USER(E2E_USER_PREFIX)))(&target)))

/** \brief Helper macro for unpacking a opaque uint8 array from a serialized data memory.
 *
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] memptr Pointer to the memory which contains the serialized Data Element.
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 * \param[in] targetptr Pointer to the target memory where the extracted data will be written to.
 */
#define E2E_EXTRACTSIGTO_E2E_OP_U8N(bitlength,bitpos,memptr,nbytes,targetptr)       \
   do {                                                                             \
      uint8_least byte_cnt;                                                         \
      /* E2E_OP_U8N treated as LE were bitpos is lsb and shall be byte aligned
       * AND bitlength constancy to nbytes AND
       * bitlength multiple of 8 */                                                 \
      E2E_STATIC_ASSERT( (((bitpos) % 8U) == 0U) &&                                 \
                         (((bitlength) / 8U) == (nbytes))&&                         \
                         (((bitlength) % 8U) == 0U) );                              \
      for (byte_cnt = 0U; byte_cnt < (nbytes); ++byte_cnt)                          \
      {                                                                             \
         (targetptr)[byte_cnt] = (memptr)[byte_cnt + ((bitpos) / 8U)];              \
      }                                                                             \
   } while ((0))

/* ============= helper macros for E2E_EXTRACT_SIGNAL_FROM_MEMORY_BE ============= */

/** \brief extract a signal from a 1-byte buffer in big endian order
 *
 * \param[in] shift .. right-shift of last byte
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 */
#define E2E_EXTRACT_BE_HELP3_1U(shift, offset, pnData )                                          \
    ((uint8)( ((uint8)(((pnData)[0])&(uint8)(0xffU>>(offset))))>>(shift)                         \
        ))

/** \brief extract a signal from a 2-byte buffer in big endian order
 *
 * \param[in] shift .. right-shift of last byte
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 */
#define E2E_EXTRACT_BE_HELP3_2U(shift, offset, pnData )                                          \
    ((uint16)( (uint16)(((uint16)((pnData)[1]))>>(shift))                                        \
      | (uint16)(((uint16)(((uint16)((pnData)[0]))&(uint8)(0xffU>>(offset))))<<(8U-(shift)))     \
        ))

/** \brief extract a signal from a 3-byte buffer in big endian order
 *
 * \param[in] shift .. right-shift of last byte
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 */
#define E2E_EXTRACT_BE_HELP3_3U(shift, offset, pnData )                                           \
        ((uint32)( (uint32)(((uint32)((pnData)[2]))>>(shift))                                     \
        | (uint32)((uint32)(((uint32)((pnData)[1]))<<(8U-(shift)))                                \
        | (uint32)(((uint32)(((uint32)((pnData)[0]))&(uint8)(0xffU>>(offset))))<<(16U-(shift))))  \
        ))

/** \brief extract a signal from a 4-byte buffer in big endian order
 *
 * \param[in] shift .. right-shift of last byte
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 */
#define E2E_EXTRACT_BE_HELP3_4U(shift, offset, pnData )                                           \
        ((uint32)( (uint32)(((uint32)((pnData)[3]))>>(shift))                                     \
        | (uint32)((uint32)(((uint32)((pnData)[2]))<<(8U-(shift)))                                \
        | (uint32)((uint32)(((uint32)((pnData)[1]))<<(16U-(shift)))                               \
        | (uint32)(((uint32)(((uint32)((pnData)[0]))&(uint8)(0xffU>>(offset))))<<(24U-(shift))))) \
        ))

/** \brief extract a signal from a 5-byte buffer in big endian order
 *
 * \param[in] shift .. right-shift of last byte
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 */
#define E2E_EXTRACT_BE_HELP3_5U(shift, offset, pnData )                                           \
        ((uint32)( (uint32)(((uint32)((pnData)[4]))>>(shift))                                     \
        | (uint32)((uint32)(((uint32)((pnData)[3]))<<(8U-(shift)))                                \
        | (uint32)((uint32)(((uint32)((pnData)[2]))<<(16U-(shift)))                               \
        | (uint32)((uint32)(((uint32)((pnData)[1]))<<(24U-(shift)))                               \
        | (uint32)(((uint32)(((uint32)((pnData)[0]))&(uint8)(0xffU>>(offset))))<<(32U-(shift))))))\
        ))

/** \brief extract a signal from byte buffer in big endian order
 *
 * \param[in] shift .. right-shift of last byte
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nbytes .. number of affected bytes (without pre-compile calculation) (1 .. 5)
 */
#define E2E_EXTRACT_BE_HELP2(shift, offset, pnData, nbytes )                                    \
    ( E2E_EXTRACT_BE_HELP3_ ## nbytes((shift), (offset), (pnData) )                             \
    )

/** \brief extract a signal from byte buffer in big endian order
 *
 * \param[in] len .. length of signal in bits (1 .. 32)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nbytes .. number of affected bytes (without pre-compile calculation) (1 .. 5)
 */
#define E2E_EXTRACT_BE_HELP1(len, offset, pnData, nbytes)                                       \
        ( E2E_EXTRACT_BE_HELP2(((uint8)(7U-(((len)+(offset)+7U)%8U))),                          \
                                 (offset),                                                      \
                                 (pnData),                                                      \
                                 nbytes)                                                        \
        )

/* ============= end of helper macros for E2E_EXTRACT_SIGNAL_FROM_MEMORY_BE ============= */

/** \brief extract a signal with length 'len' bits on bit offset 'offset' within
 * the buffer 'pnData' and a number of 'nbytes' affected bytes.
 * The signals within pnData are assumed to follow the big endian convention.
 *
 * \param[in] len .. length of signal in bits (1 .. 32)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nbytes .. Number of bytes occupied by the signal
 */
#define E2E_EXTRACT_SIGNAL_FROM_MEMORY_BE(len, offset, pnData, nbytes)                          \
        ( E2E_EXTRACT_BE_HELP1((len),                                                           \
                                 ((uint8)(7U-((offset)%8U))),                                   \
                                 (&((pnData)[(offset)/8U])),                                    \
                                 nbytes)                                                        \
        )

/* ============= helper macros for E2E_EXTRACT_SIGNAL_FROM_MEMORY_LE ============= */

/** \brief extract a signal from a 1-byte buffer in little endian order
 *
 * \param[in] mask .. mask for last byte
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 */
#define E2E_EXTRACT_LE_HELP3_1U(mask, offset, pnData )                                           \
    ((uint8)( ((uint8)((pnData)[0]&(mask)))>>(offset)                                            \
        ))

/** \brief extract a signal from a 2-byte buffer in little endian order
 *
 * \param[in] mask .. mask for last byte
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 */
#define E2E_EXTRACT_LE_HELP3_2U(mask, offset, pnData )                                            \
        ((uint16)( (uint16)(((uint16)((pnData)[0]))>>(offset))                                    \
        | (uint16)(((uint16)(((uint16)((pnData)[1]))&(mask)))<<(8U-(offset)))                     \
        ))                                                                                        \

/** \brief extract a signal from a 3-byte buffer in little endian order
 *
 * \param[in] mask .. mask for last byte
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 */
#define E2E_EXTRACT_LE_HELP3_3U(mask, offset, pnData )                                           \
        ((uint32)( (uint32)((((uint32)(pnData)[0]))>>(offset))                                   \
        | (uint32)((uint32)(((uint32)((pnData)[1]))<<(8U-(offset)))                              \
        | (uint32)(((uint32)(((uint32)((pnData)[2]))&(uint32)(mask)))<<(16U-(offset))))          \
        ))

/** \brief extract a signal from a 4-byte buffer in little endian order
 *
 * \param[in] mask .. mask for last byte
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 */
#define E2E_EXTRACT_LE_HELP3_4U(mask, offset, pnData )                                           \
        ((uint32)( (uint32)(((uint32)((pnData)[0]))>>(offset))                                   \
        | (uint32)((uint32)(((uint32)((pnData)[1]))<<(8U-(offset)))                              \
        | (uint32)((uint32)(((uint32)((pnData)[2]))<<(16U-(offset)))                             \
        | (uint32)((uint32)(((uint32)((pnData)[3]))&(uint32)(mask))<<(24U-(offset)))))           \
        ))

/** \brief extract a signal from a 5-byte buffer in little endian order
 *
 * \param[in] mask .. mask for last byte
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 */
#define E2E_EXTRACT_LE_HELP3_5U(mask, offset, pnData )                                            \
        ((uint32)( (uint32)(((uint32)((pnData)[0]))>>(offset))                                    \
        | (uint32)((uint32)(((uint32)((pnData)[1]))<<(8U-(offset)))                               \
        | (uint32)((uint32)(((uint32)((pnData)[2]))<<(16U-(offset)))                              \
        | (uint32)((uint32)(((uint32)((pnData)[3]))<<(24U-(offset)))                              \
        | (uint32)(((uint32)(((uint32)((pnData)[4]))&(uint32)(mask)))<<(32U-(offset))))))         \
        ))

/** \brief extract a signal from a byte buffer in little endian order
 *
 * \param[in] mask .. mask for last byte
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nbytes .. number of affected bytes (without pre-compile calculation) (1 .. 5)
 */
#define E2E_EXTRACT_LE_HELP2(mask, offset, pnData, nbytes )                          \
      E2E_EXTRACT_LE_HELP3_ ## nbytes((mask), (offset), (pnData))

/** \brief extract a signal from a byte buffer in little endian order
 *
 * \param[in] len .. length of signal in bits (1 .. 32)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nbytes .. number of affected bytes (without pre-compile calculation) (1 .. 5)
 */
#define E2E_EXTRACT_LE_HELP1(len, offset, pnData, nbytes)                                       \
        ( E2E_EXTRACT_LE_HELP2(((uint8)(0xffU>>(7U-(((len)+(offset)+7U)%8U)))),                 \
                                 (offset),                                                      \
                                 (pnData),                                                      \
                                 nbytes)                                                        \
        )

/* ============= end of helper macros for E2E_EXTRACT_SIGNAL_FROM_MEMORY_LE ============= */

/** \brief extract a signal with length 'len' bits on bit offset 'offset' within
 * the buffer 'pnData' with 'nbytes' number of affected bytes.
 * The signals within pnData are assumed to follow the little endian convention.
 *
 * \param[in] len .. length of signal in bits (1 .. 32)
 * \param[in] offset .. offset of first bit of signal in buffer (0 .. 7)
 * \param[in] pnData .. pointer to buffer
 * \param[in] nbytes .. Number of bytes occupied by the signal
 */
#define E2E_EXTRACT_SIGNAL_FROM_MEMORY_LE(len, offset, pnData, nbytes)                          \
        ( E2E_EXTRACT_LE_HELP1((len),                                                           \
                                 ((uint8)((offset)%8U)),                                        \
                                 (&((pnData)[(offset)/8U])),                                    \
                                 nbytes)                                                        \
        )

/* Deviation MISRAC2012-1 <STOP> */

/*==[Types]===================================================================*/

/*==[Declaration of Constants with external linkage]==========================*/

/*==[Declaration of Variables with external linkage]==========================*/

/*==[Declaration of functions with external linkage]==========================*/

#endif /* ifndef E2E_UNPACKSIG_H */
/*==================[end of file]============================================*/
