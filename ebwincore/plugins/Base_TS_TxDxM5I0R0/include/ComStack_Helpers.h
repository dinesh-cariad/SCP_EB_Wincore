#ifndef COMSTACK_HELPERS_H
#define COMSTACK_HELPERS_H

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

/*==================[includes]===============================================*/

#include <TSAutosar.h>
#include <TSCompiler.h>

/*==================[macros]=================================================*/

/* default to 32bit Instruction Set Architecture */
#ifndef COMSTACK_ISA_WIDTH
  #define COMSTACK_ISA_WIDTH 32
#endif

/*----------[ macros for Litte / Big Endian in compiletime & overloadable variants]----------*/

/* We supply CompileTime macros, that also serve as the default for the run-time macros below.
 * The compile-time macros must be plain C99, so that the compiler can evaluate them,
 * when used on constants. The run-time macros in contrast can be replaced with a more efficient
 * way to compute the respective expression for a given architecture, e.g.: inline assembly.
 */

#ifdef COMSTACK_CT_BYTE_MIRROR16
#error COMSTACK_CT_BYTE_MIRROR16 already defined
#endif
/** \brief Byte mirror macro to convert uint16 values between little and big endian format.
 *  Non-overloadable. For compile-time evaluation. */
# define COMSTACK_CT_BYTE_MIRROR16(x) \
  ((uint16)(((uint16)(((uint16)(x)) << 8u)) | ((uint16)(((uint16)(x))>> 8u))))

#ifdef COMSTACK_CT_BYTE_MIRROR32
#error COMSTACK_CT_BYTE_MIRROR32 already defined
#endif
/** \brief Byte mirror macro to convert uint32 values between little and big endian format.
 *  Non-overloadable. For compile-time evaluation. */
#define COMSTACK_CT_BYTE_MIRROR32(value) (uint32)\
  ( (uint32)((uint32)(((uint32)(value) & (uint32)0x000000FFUL) << 24U)) | \
    (uint32)((uint32)(((uint32)(value) & (uint32)0x0000FF00UL) <<  8U)) | \
    (uint32)((uint32)(((uint32)(value)) >> 8U ) & ((uint32)0x0000FF00UL)) | \
    (uint32)((uint32)(((uint32)(value)) >> 24U) & ((uint32)0x000000FFUL)) )

#ifdef COMSTACK_CT_HTON_UINT16
#error COMSTACK_CT_HTON_UINT16 already defined
#endif
/** \brief uint16 host to network order conversion macro.
 *  Non-overloadable. For compile-time evaluation. */
#define COMSTACK_CT_HTON_UINT16(value) TS_IF_BE_LE((value), COMSTACK_CT_BYTE_MIRROR16(value))

#ifdef COMSTACK_CT_NTOH_UINT16
#error COMSTACK_CT_NTOH_UINT16 already defined
#endif
/** \brief uint16 network to host order conversion macro.
 *  Non-overloadable. For compile-time evaluation. */
#define COMSTACK_CT_NTOH_UINT16(value) TS_IF_BE_LE((value), COMSTACK_CT_BYTE_MIRROR16(value))

#ifdef COMSTACK_CT_HTON_UINT32
#error COMSTACK_CT_HTON_UINT32 already defined
#endif
/** \brief uint32 host to network order conversion macro
 *  Non-overloadable. For compile-time evaluation. */
#define COMSTACK_CT_HTON_UINT32(value) TS_IF_BE_LE((value), COMSTACK_CT_BYTE_MIRROR32(value))

#ifdef COMSTACK_CT_NTOH_UINT32
#error COMSTACK_CT_NTOH_UINT32 already defined
#endif
/** \brief uint32 network to host order conversion macro
 *  Non-overloadable. For compile-time evaluation. */
#define COMSTACK_CT_NTOH_UINT32(value) TS_IF_BE_LE((value), COMSTACK_CT_BYTE_MIRROR32(value))


/* In contrast to the CompileTime macros, the following run-time macros must be predefine-able by an integrator,
 * if his knowledge of either
 * a) support for unaligned loads/stores in the target architecture
 * b) driver buffer alignment (e.g. the EB Ethernet drivers)
 * permits.
 */

/* allow overriding with chip-dependent version by compiler|platorms|integrator */
#ifndef COMSTACK_BYTE_MIRROR16
  /** \brief Byte mirror macro to convert uint16 values between little and big endian format.
   *  Should usually map to a _ROR(x,8u) or similar compiler-known-function. */
  #define COMSTACK_BYTE_MIRROR16(x) COMSTACK_CT_BYTE_MIRROR16(x)
#endif /* COMSTACK_BYTE_MIRROR16 */

/* allow overriding with chip-dependent version by compiler|platorms|integrator */
#ifndef COMSTACK_BYTE_MIRROR32
  /** \brief Byte mirror macro to convert uint32 values between little and big endian format.
   *  Should usually map to a _builtin_byteswap or similar compiler-known-function.
   *  Can also be implemented via inline assembly, using 4-5 rotates of full & half registers */
  #define COMSTACK_BYTE_MIRROR32(value) COMSTACK_CT_BYTE_MIRROR32(value)
#endif /* COMSTACK_BYTE_MIRROR32 */

/* allow overriding with chip-dependent version by compiler|platorms|integrator */
#ifndef COMSTACK_HTON_UINT16
  /** \brief uint16 host to network order conversion macro */
  #define COMSTACK_HTON_UINT16(value) TS_IF_BE_LE((value), COMSTACK_BYTE_MIRROR16(value))
#endif /* COMSTACK_HTON_UINT16 */

/* allow overriding with chip-dependent version by compiler|platorms|integrator */
#ifndef COMSTACK_NTOH_UINT16
  /** \brief  uint16 network to host order conversion macro */
  #define COMSTACK_NTOH_UINT16(value) TS_IF_BE_LE((value), COMSTACK_BYTE_MIRROR16(value))
#endif /* COMSTACK_NTOH_UINT16 */

/* allow overriding with chip-dependent version by compiler|platorms|integrator */
#ifndef COMSTACK_HTON_UINT32
  /** \brief uint32 host to network order conversion macro */
  #define COMSTACK_HTON_UINT32(value) TS_IF_BE_LE((value), COMSTACK_BYTE_MIRROR32(value))
#endif /* COMSTACK_HTON_UINT32 */

/* allow overriding with chip-dependent version by compiler|platorms|integrator */
#ifndef COMSTACK_NTOH_UINT32
  /** \brief  uint32 network to host order conversion macro */
  #define COMSTACK_NTOH_UINT32(value) TS_IF_BE_LE((value), COMSTACK_BYTE_MIRROR32(value))
#endif /* COMSTACK_NTOH_UINT32 */

/* allow overriding with chip-dependent version by compiler|platorms|integrator */
#ifndef COMSTACK_GET16
/* header get/set macros - bytewise, so they work for all architectures - alignment optimization only needed for ComStack */
/** \brief macro gets a 16 bit value from a network header.
 * \par NOT SAFE FROM SIDE-EFFECTS!  Arguments are evaluated multiple times!
 * \param[in] headerPtr Pointer to network header area.
 * \param[in] constByteOffset Byte offset from\a headerPtr as a compile-time constant
 * requires ComStack_ConstUint8PtrType to be defined before use. */
#define COMSTACK_GET16(headerPtr, constByteOffset) \
  ((uint16)(((uint16)(((uint16)(((ComStack_ConstUint8PtrType)(headerPtr))[(constByteOffset)]))<<8u)) | \
            ((uint16)((uint8)  (((ComStack_ConstUint8PtrType)(headerPtr))[(constByteOffset)+1u])  )) ))
#endif /* ifndef COMSTACK_GET16 */

/* allow overriding with chip-dependent version by compiler|platorms|integrator */
#ifndef COMSTACK_GET32
/** \brief macro gets an uint32 value from a network header.
 * \par NOT SAFE FROM SIDE-EFFECTS!  Arguments are evaluated multiple times!
 * \param[in] headerPtr Pointer to network header area.
 * \param[in] constByteOffset Byte offset from\a headerPtr as a compile-time constant
 * requires ComStack_ConstUint8PtrType to be defined before use. */
#define COMSTACK_GET32(headerPtr, constByteOffset) \
  (((uint32)(((uint32)(((ComStack_ConstUint8PtrType)(headerPtr))[(constByteOffset)  ]))<<24u)) | \
   ((uint32)(((uint32)(((ComStack_ConstUint8PtrType)(headerPtr))[(constByteOffset)+1]))<<16u)) | \
   ((uint32)(((uint32)(((ComStack_ConstUint8PtrType)(headerPtr))[(constByteOffset)+2]))<< 8u)) | \
   ((uint32)(((uint32)(((ComStack_ConstUint8PtrType)(headerPtr))[(constByteOffset)+3]))     )) )
#endif /* ifndef COMSTACK_GET32 */

/* allow overriding with chip-dependent version by compiler|platorms|integrator */
#ifndef COMSTACK_SET16
/* safe function-type helper macros for header construction & parsing:
 * safety by: single parameter evaluation avoids side-effects,
 *            type-safe local variables with correct type */

/** \brief macro sets any network header 16 bit field to \a value16
 * \par Safe from side effects: Arguments are evaluated only once.
 * \param[in] headerPtr Pointer to network header area.
 * \param[in] constByteOffset Byte offset from\a headerPtr as a compile-time constant
 * \param[in] value16 Value to be set
 * requires ComStack_Uint8PtrType to be defined before use. */
#define COMSTACK_SET16(headerPtr, constByteOffset, value16) \
  do { \
    enum {ofs= (constByteOffset) };   /* only compile-time constant allowed */    \
    ComStack_Uint8PtrType const hdrPtr= (headerPtr);  /* single argument eval. */ \
    uint16 const val16= (value16);                      /* ensure correct type */ \
    hdrPtr[ofs  ]= TS_CAST(uint, 8, val16 >> 8u);  \
    hdrPtr[ofs+1]= TS_CAST(uint, 8, val16);        \
  } while(0)
#endif /* ifndef COMSTACK_SET16 */

/* allow overriding with chip-dependent version by compiler|platorms|integrator */
#ifndef COMSTACK_SET32
/** \brief macro sets any network header 32 bit field to \a value32
 * \par Safe from side effects: Arguments are evaluated only once.
 * \param[in] headerPtr Pointer to network header area.
 * \param[in] constByteOffset Byte offset from\a headerPtr as a compile-time constant
 * \param[in] value32 Value to be set
 * requires ComStack_Uint8PtrType to be defined before use. */
#define COMSTACK_SET32(headerPtr, constByteOffset, value32)  \
  do { \
    enum {ofs= (constByteOffset) };     /* only compile-time constant allowed */  \
    ComStack_Uint8PtrType const hdrPtr= (headerPtr); /* single argument eval. */  \
    uint32 const val32= (value32);                     /* ensure correct type */  \
    hdrPtr[ofs  ]= TS_CAST(uint, 8, val32 >> 24u); \
    hdrPtr[ofs+1]= TS_CAST(uint, 8, val32 >> 16u); \
    hdrPtr[ofs+2]= TS_CAST(uint, 8, val32 >>  8u); \
    hdrPtr[ofs+3]= TS_CAST(uint, 8, val32       ); \
  } while(0)
#endif /* ifndef COMSTACK_SET32 */

/* allow overriding with chip-dependent version by compiler|platorms|integrator */
#ifndef COMSTACK_GETCPY32
/** \brief macro copies 32 bit value (in network byte order) from
 * \a headerPtr @ \a constByteOffset to \a resultPtr
 * \par Safe from side effects: Arguments are evaluated only once.
 * \param[in] headerPtr Pointer to network header area.
 * \param[in] constByteOffset Byte offset from \a headerPtr as a compile-time constant
 * \param[out] resultPtr Destination of the copy operation
 * requires ComStack_ConstUint8PtrType to be defined before use. */
#define COMSTACK_GETCPY32(headerPtr, constByteOffset, resultPtr) \
  do { \
    enum {ofs= (constByteOffset) };          /* only compile-time constant allowed */\
    ComStack_ConstUint8PtrType const hdrPtr= (headerPtr); /* single argument eval. */\
    uint8* const resPtr= (uint8*)(resultPtr);              /* cast to correct type */\
    resPtr[0]= hdrPtr[ofs];    \
    resPtr[1]= hdrPtr[ofs+1];  \
    resPtr[2]= hdrPtr[ofs+2];  \
    resPtr[3]= hdrPtr[ofs+3];  \
  } while(0)
#endif /* ifndef COMSTACK_GETCPY32 */

/* allow overriding with chip-dependent version by compiler|platorms|integrator */
#ifndef COMSTACK_SETCPY32
/** \brief macro copies 32 bit value (in network byte order) from
 * \a sourcePtr to \a resultPtr @ \a constByteOffset
 * \par Safe from side effects: Arguments are evaluated only once.
 * \param[in] headerPtr Pointer to network header area.
 * \param[in] constByteOffset Byte offset from \a headerPtr as a compile-time constant
 * \param[in] sourcePtr Source of the copy operation
 * requires ComStack_Uint8PtrType to be defined before use. */
#define COMSTACK_SETCPY32(headerPtr, constByteOffset, sourcePtr)  \
  do { \
    enum {ofs= (constByteOffset) };                /* only compile-time constant allowed */\
    ComStack_Uint8PtrType const hdrPtr= (headerPtr);            /* single argument eval. */\
    uint8 const* const srcPtr= (uint8 const*)(sourcePtr);  /* cast to correct type */\
    hdrPtr[ofs  ]= srcPtr[0]; \
    hdrPtr[ofs+1]= srcPtr[1]; \
    hdrPtr[ofs+2]= srcPtr[2]; \
    hdrPtr[ofs+3]= srcPtr[3]; \
  } while(0)
#endif /* ifndef COMSTACK_SETCPY32 */


/* ------------------------- BIT macros ------------------ */



/* allow overriding with chip-dependent version by compiler|platorms|integrator */
#ifndef COMSTACK_BYTE_REPLICATOR32
/** \brief A constant multiplier to replicate a multiplied byte within a 32 bit register.
 * Big constants are costly.
 * Hence we multiply byte constants with byte multipliers to provide clever compilers
 * alternate routes for evaluation:
 * e.g. const / value range analysis -> PC-rel load of all-large consts vs.
 * loading one big multiplier constant & then multiplying it with a number of small consts
 * that each fit in the 8bit immediate operands (if the MUL is cheaper than the LD).
 *
 * If your compiler is not smart enough to evaluate alternate routes or falls for premature
 * constant-expression-evaluation, you may try to predefine the multiplier constants in
 * terms of a volatile variable that effectively hides the constant from the compiler.
 */
  #define COMSTACK_BYTE_REPLICATOR32 UINT32_C(0x01010101)
#endif

/* allow overriding with chip-dependent version by compiler|platorms|integrator */
#ifndef COMSTACK_HAMMING_WEIGHT32

/** \brief Hamming weight computes the number of bits set in the input argument (register width for 0).
 * We employ the ISA equivalent of a binary adder tree, using register-width SHIFT & ADD operations,
 * as SIMD operations by ignoring carry overflows, as it's proven they cannot occur.
 * Thus we'd finish in ld(registerwidth) steps.
 * However, we switch back to folding adds, once we reach operand sizes that are likely
 * to be computed cheaper using register-parts or 'extract' operations due to less parallelism.
 * At the special request of the BITE team:
 * https://en.wikipedia.org/wiki/Hamming_weight#Efficient_implementation
 */
#define COMSTACK_HAMMING_WEIGHT32(result8_least, in32)            \
  do {                                                            \
    const uint32 rep32 = COMSTACK_BYTE_REPLICATOR32;              \
    const uint32 m1 = 0x55u * rep32;                              \
    const uint32 m2 = 0x33u * rep32;                              \
    const uint32 m4 = ((uint32)(rep32 << 4u)) - rep32;            \
    uint32 x= (uint32) (in32);                                    \
    x= ((uint32)(x & m1)) + ((uint32)(((uint32)(x >> 1u)) & m1)); \
    x= ((uint32)(x & m2)) + ((uint32)(((uint32)(x >> 2u)) & m2)); \
    x= ((uint32)(x & m4)) + ((uint32)(((uint32)(x >> 4u)) & m4)); \
    x+= x << 16u;                                                 \
    x+= x << 8u;                                                  \
    result8_least= x >> ((sizeof(uint32)-1u) << 3u);              \
  } while(0)
#endif

/* allow overriding with chip-dependent version by compiler|platorms|integrator */
#ifndef COMSTACK_LOWEST_BIT32
/** \brief Compute the lowest bit position for the input \a in32
 *  z=z^(z-1) : the z-1 lets the underflow-carry ripple up to the lowest bit cearing it.
 *  XOR-ing that with the original value reinstates the lowest bit, but clears all higher bits.
 *  Now we only have to count the number of set bits, subtracting one to be zero-based
 */
#define COMSTACK_LOWEST_BIT32(result8_least, in32) \
  do {                              \
    uint8_least y;                  \
    uint32 z= (uint32)(in32);       \
    z= z ^ (z-1);                   \
    COMSTACK_HAMMING_WEIGHT32(y,z); \
    result8_least= y-1;             \
  } while(0)
#endif


#if COMSTACK_ISA_WIDTH==64
  #ifndef COMSTACK_BYTE_REPLICATOR64
    #define COMSTACK_BYTE_REPLICATOR64 UINT64_C(0x0101010101010101)
  #endif

/* allow overriding with chip-dependent version by compiler|platorms|integrator */
#ifndef COMSTACK_HAMMING_WEIGHT64

/** \brief Hamming weight computes the number of bits set in the input argument (register width for 0).
 * We employ the ISA equivalent of a binary adder tree, using register-width SHIFT & ADD operations,
 * as SIMD operations by ignoring carry overflows, as it's proven they cannot occur.
 * Thus we'd finish in ld(registerwidth) steps.
 * However, we switch back to folding adds, once we reach operand sizes that are likely
 * to be computed cheaper using register-parts or 'extract' operations due to less parallelism.
 * At the special request of the BITE team:
 * https://en.wikipedia.org/wiki/Hamming_weight#Efficient_implementation
 */
#define COMSTACK_HAMMING_WEIGHT64(result8_least, in64)            \
  do {                                                            \
    const uint64 rep64 = COMSTACK_BYTE_REPLICATOR64;              \
    const uint64 m1 = 0x55u * rep64;                              \
    const uint64 m2 = 0x33u * rep64;                              \
    const uint64 m4 = ((uint64)(rep64 << 4u)) - rep64;            \
    uint32 x;                                                     \
    uint64 y= (uint64) (in64);                                    \
    y= ((uint64)(y & m1)) + ((uint64)(((uint64)(y >> 1u)) & m1)); \
    y= ((uint64)(y & m2)) + ((uint64)(((uint64)(y >> 2u)) & m2)); \
    y= ((uint64)(y & m4)) + ((uint64)(((uint64)(y >> 4u)) & m4)); \
    x= ((uint32)y) + ((uint32)(y>>32u));                          \
    x+= x << 16u;                                                 \
    x+= x << 8u;                                                  \
    result8_least= x >> ((sizeof(uint32)-1u) << 3u);              \
  } while(0)
#endif

/* allow overriding with chip-dependent version by compiler|platorms|integrator */
#ifndef COMSTACK_LOWEST_BIT64

/** \brief Compute the lowest bit position for the input \a in64
 *  z=z^(z-1) : the z-1 lets the underflow-carry ripple up to the lowest bit cearing it.
 *  XOR-ing that with the original value reinstates the lowest bit, but clears all higher bits.
 *  Now we only have to count the number of set bits, subtracting one to be zero-based
 */
#define COMSTACK_LOWEST_BIT64(result8_least, in64)  \
  do {                              \
    uint64 x= (uint64)(in64);       \
    uint8_least z;                  \
    x= x ^ (x-1);                   \
    COMSTACK_HAMMING_WEIGHT64(z,x); \
    result8_least= z-1;             \
  } while(0)
#endif
#endif /* COMSTACK_ISA_WIDTH==64 */

/* \brief Macro to be used as a start value to begin the search at the start of the bitfield. */
#define COMSTACK_BF_START ((uint32)(~((uint32)0u)))


/*==================[functions]==============================================*/
#define BASE_START_SEC_CODE
#include <Base_MemMap.h>

/* \brief Find the next set bit in a (one-terminated) bitfield.
 * Note: we use one-terminated bitfields, so we only have to check one condition per loop iteration.
 * Checking a second condition would double the number of jumps for each iteration.
 * For 1-terminated bitfields, this is efficient & safe.
 * For partial searches within a bitfield, this is still safe, as long as the whole bitfield is
 * 1-terminated and the found bit is checked against the boundary (always necessary condition).
 * Even for partial searches, the search past the end of the range is still more efficient than an
 * additional length check, as 0-bits are handled with a multiplicity of 32/64, while the first set
 * bit immediately terminates the search.
 * (on X86_64: loop: 4 instructions, 49 instructions total)
 *
 * \param[in] bitfield The bitfield which shall be analyzed for set bits
 * \param[in] prev use \a COMSTACK_BF_START to start at the beginning, the last result to continue.
 * \return The next-lowest set bit after the one specified by 'prev'
 */
extern FUNC(uint32, BASE_CODE) ComStack_FindNextOne
           (CONSTP2CONST(void, AUTOMATIC, BASE_APPL_DATA) bitfield, const uint32 prev);

#define BASE_STOP_SEC_CODE
#include <Base_MemMap.h>
/*==================[end of file]============================================*/
#endif /* ifndef COMSTACK_HELPERS_H */
