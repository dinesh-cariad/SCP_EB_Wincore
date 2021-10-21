/**
 * \file
 *
 * \brief AUTOSAR SoAd
 *
 * This file contains the implementation of the AUTOSAR
 * module SoAd.
 *
 * \version 1.8.16
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef SOAD_INT_H
#define SOAD_INT_H

/*
 * Misra-C:2012 Deviations:
 *
 * MISRAC2012-1) Deviated Rule: 4.7 (required)
 * If a function returns error information, then that error information shall be tested.
 *
 * Reason:
 * The Det SWS specifies that Det_ReportError shall always return E_OK.
 *
 * MISRAC2012-2) Deviated Rule: 20.10 (advisory)
 * "The # and ## preprocessor operators should not be used."
 *
 * Reason:
 * String concatenation is used to create efficient readable code.
 *
 * MISRAC2012-3) Deviated Rule: 14.2
 * A for loop shall be well-formed.
 *
 * Reason:
 * 1)The comparison is done with the incremented loop counter to protect the last for loop entry.
 * 2)Macro COMSTACK_BF_ITERATE is an optimized pattern to iterate over bit fields.
 *   It allows the compiler simplifying control code (fewer conditional jumps) and reducing the
 *   number of cache misses.
 *
 * MISRAC2012-9) Deviated Rule: 13.2 (required)
 * The value of an expression and its persistent side effects shall be the same under
 * all permitted evaluation orders.
 *
 * Reason:
 * Macro COMSTACK_BF_ITERATE is an optimized pattern to iterate over bit fields.
 * The side effect was checked and has no negative impact.
 */

/*==================[inclusions]============================================*/
#include <TSAutosar.h> /* EB specific standard types */
#include <ComStack_Types.h>
#include <TcpIp.h>
#include <SoAd_Trace.h>
#include <SoAd_Types.h>         /* Module public types */
#include <SoAd_Cfg.h>           /* Module configurable parameters */
#include <SoAd_Lcfg.h>           /* link time configuration */

#ifdef SOAD_PRECOMPILE_TIME_PBCFG_ACCESS
#include <SoAd_PBcfg.h>
#endif /* SOAD_PRECOMPILE_TIME_PBCFG_ACCESS */

#include <TSMem.h>             /* TS_MemCpy */
#if (defined TS_RELOCATABLE_CFG_ENABLE)
#error TS_RELOCATABLE_CFG_ENABLE is already defined
#endif
#define TS_RELOCATABLE_CFG_ENABLE SOAD_RELOCATABLE_CFG_ENABLE
#include <TSPBConfig.h>        /* post-build config access macros */

#if(SOAD_DEV_ERROR_DETECT == STD_ON)
#include <Det.h> /* Det API */
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
/*==================[macros]================================================*/


#if (defined SOAD_UNINITED)
#error SOAD_UNINITED is already defined
#endif
#define SOAD_UNINITED 0U

#if (defined SOAD_INITED)
#error SOAD_INITED is already defined
#endif
#define SOAD_INITED 1U

#if (defined SOAD_TS_SHL_UINT64)
#error SOAD_TS_SHL_UINT64 is already defined
#endif
/* This is an uint64 shift left of (t1,t2) by 'Offset' bytes that also works on 32bit-only architectures:
 * architecture-specific macro for that to avoid branches for 64bit architectures.
 * Macro shall by replaced by TS_SHL_UINT64 as soon as available in Base. */
#define SOAD_TS_SHL_UINT64(t1, t2, Offset) \
  do { \
    if (Offset > 0U) \
    { \
      if (Offset >= 32U) \
      { \
        (t1) = (t2) << (Offset - 32U); \
        (t2) = 0U; \
      } \
      else \
      { \
        (t1) = ((t1) << Offset) | ((t2) >> (32U - Offset)); \
        (t2) <<= Offset; \
      } \
    } \
  } while(0)

/** \brief Macro to iterate over bit field
 *  This macro iterates over a bit field to find the next bit in an optimized manner.
 *  CAUTION: Start is 1st bit minus 1.
 **/
#if (defined SOAD_COMSTACK_BF_ITERATE)
#error SOAD_COMSTACK_BF_ITERATE is already defined
#endif
/* Deviation MISRAC2012-3 */
/* Deviation MISRAC2012-9 */
#define SOAD_COMSTACK_BF_ITERATE(Next, BitFieldPtr, Start, End) \
  (Next) = (Start); (End) > ((Next) = ComStack_FindNextOne((BitFieldPtr), (Next)));

/** \brief Macro initialize bit field
 *  This macro initializes a bit field with zero and sets the bit max + 1.
 * ComStack_FindNextOne() will loop and return any bit that is set in the bit map.
 * If no regular bit is set, then this last bit shall end the do while loop.
 * BitMap size is always number of elements + 1 bit.
 **/
#if (defined SOAD_COMSTACK_BF_INIT)
#error SOAD_COMSTACK_BF_INIT is already defined
#endif
#define SOAD_COMSTACK_BF_INIT(BitFieldPtr, MaxBits) \
  do \
  { \
      TS_MemBZero((BitFieldPtr), 4U*(((MaxBits) / 32U) + 1U)); \
      (BitFieldPtr)[(MaxBits) / 32U] |= ((uint32)1U << ((MaxBits) % 32U)); \
  }while(0)


/* ====== [Defensive programming] ======*/




/*------------------------[Defensive programming]----------------------------*/

#if (defined SOAD_PRECONDITION_ASSERT)
#error SOAD_PRECONDITION_ASSERT is already defined
#endif

#if (defined SOAD_PRECONDITION_ASSERT_NO_EVAL)
#error SOAD_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (SOAD_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define SOAD_PRECONDITION_ASSERT(Condition, ApiId) \
  DET_PRECONDITION_ASSERT((Condition), SOAD_MODULE_ID, SOAD_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define SOAD_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL((Condition), SOAD_MODULE_ID, SOAD_INSTANCE_ID, (ApiId))
#else
#define SOAD_PRECONDITION_ASSERT(Condition, ApiId)
#define SOAD_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined SOAD_POSTCONDITION_ASSERT)
#error SOAD_POSTCONDITION_ASSERT is already defined
#endif

#if (defined SOAD_POSTCONDITION_ASSERT_NO_EVAL)
#error SOAD_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (SOAD_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define SOAD_POSTCONDITION_ASSERT(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT((Condition), SOAD_MODULE_ID, SOAD_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define SOAD_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL((Condition), SOAD_MODULE_ID, SOAD_INSTANCE_ID, (ApiId))
#else
#define SOAD_POSTCONDITION_ASSERT(Condition, ApiId)
#define SOAD_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined SOAD_INVARIANT_ASSERT)
#error SOAD_INVARIANT_ASSERT is already defined
#endif

#if (defined SOAD_INVARIANT_ASSERT_NO_EVAL)
#error SOAD_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (SOAD_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define SOAD_INVARIANT_ASSERT(Condition, ApiId) \
  DET_INVARIANT_ASSERT((Condition), SOAD_MODULE_ID, SOAD_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define SOAD_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL((Condition), SOAD_MODULE_ID, SOAD_INSTANCE_ID, (ApiId))
#else
#define SOAD_INVARIANT_ASSERT(Condition, ApiId)
#define SOAD_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined SOAD_STATIC_ASSERT)
# error SOAD_STATIC_ASSERT is already defined
#endif
#if (SOAD_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated */
# define SOAD_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define SOAD_STATIC_ASSERT(expr)
#endif

#if (defined SOAD_UNREACHABLE_CODE_ASSERT)
#error SOAD_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (SOAD_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 **

 ** \param[in] ApiId Service ID of the API function */
#define SOAD_UNREACHABLE_CODE_ASSERT(ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(SOAD_MODULE_ID, SOAD_INSTANCE_ID, (ApiId))
#else
#define SOAD_UNREACHABLE_CODE_ASSERT(ApiId)
#endif

#if (defined SOAD_INTERNAL_API_ID)
#error SOAD_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define SOAD_INTERNAL_API_ID DET_INTERNAL_API_ID



#if (defined SOAD_ENTER_CRITSEC)
#error SOAD_ENTER_CRITSEC is already defined
#endif
/** \brief Enter critical section */
#define SOAD_ENTER_CRITSEC() SchM_Enter_SoAd_SCHM_SOAD_EXCLUSIVE_AREA_0()

#if (defined SOAD_EXIT_CRITSEC)
#error SOAD_EXIT_CRITSEC is already defined
#endif
/** \brief Exit critical section */
#define SOAD_EXIT_CRITSEC() SchM_Exit_SoAd_SCHM_SOAD_EXCLUSIVE_AREA_0()

#if (defined SOAD_TX_PDUID_TO_PDUROUTE_ID)
#error SOAD_TX_PDUID_TO_PDUROUTE_ID is already defined
#endif
/** \brief Translates UL TxPduId to .PduRouteBase index */
#define SOAD_TX_PDUID_TO_PDUROUTE_ID(pduId) ((SoAd_RouteIdType)(pduId))

#if (defined SOAD_FLAG)
#error SOAD_FLAG is already defined
#endif
/* Deviation MISRAC2012-2 */
#define SOAD_FLAG(ptr, name) \
    (((ptr)->Flags & (name ## _MASK)) != 0U)

#if (defined SOAD_FLAG_UP)
#error SOAD_FLAG_UP is already defined
#endif
/* Deviation MISRAC2012-2 */
#define SOAD_FLAG_UP(ptr, name) \
    (((ptr)->UpperFlags & (name ## _MASK)) != 0U)

#if (defined SOAD_FLAG_LO)
#error SOAD_FLAG_LO is already defined
#endif
/* Deviation MISRAC2012-2 */
#define SOAD_FLAG_LO(ptr, name) \
    (((ptr)->LowerFlags & (name ## _MASK)) != 0U)

#if (defined SOAD_FLAG_SET)
#error SOAD_FLAG_SET is already defined
#endif
/* Deviation MISRAC2012-2 */
#define SOAD_FLAG_SET(ptr, name) \
    TS_AtomicSetBit_8(&((ptr)->Flags), (name ## _SHIFT))

#if (defined SOAD_FLAG_SET_UP)
#error SOAD_FLAG_SET_UP is already defined
#endif
/* Deviation MISRAC2012-2 */
#define SOAD_FLAG_SET_UP(ptr, name) \
    TS_AtomicSetBit_16(&((ptr)->UpperFlags), (name ## _SHIFT))

#if (defined SOAD_FLAG_SET_LO)
#error SOAD_FLAG_SET_LO is already defined
#endif
/* Deviation MISRAC2012-2 */
#define SOAD_FLAG_SET_LO(ptr, name) \
    TS_AtomicSetBit_8(&((ptr)->LowerFlags), (name ## _SHIFT))

#if (defined SOAD_FLAG_CLR)
#error SOAD_FLAG_CLR is already defined
#endif
/* Deviation MISRAC2012-2 */
#define SOAD_FLAG_CLR(ptr, name) \
    TS_AtomicClearBit_8(&((ptr)->Flags), (name ## _SHIFT))

#if (defined SOAD_FLAG_CLR_UP)
#error SOAD_FLAG_CLR_UP is already defined
#endif
/* Deviation MISRAC2012-2 */
#define SOAD_FLAG_CLR_UP(ptr, name) \
    TS_AtomicClearBit_16(&((ptr)->UpperFlags), (name ## _SHIFT))

#if (defined SOAD_FLAG_CLR_LO)
#error SOAD_FLAG_CLR_LO is already defined
#endif
/* Deviation MISRAC2012-2 */
#define SOAD_FLAG_CLR_LO(ptr, name) \
    TS_AtomicClearBit_8(&((ptr)->LowerFlags), (name ## _SHIFT))

#if (defined SOAD_PDU_HEADER_LEN)
#error SOAD_PDU_HEADER_LEN is already defined
#endif
/** \brief Length of PDU header */
#define SOAD_PDU_HEADER_LEN 8U

#if (defined SOAD_IPADDRMEMSIZE)
#error SOAD_IPADDRMEMSIZE is already defined
#endif
#if (SOAD_IPV6_SUPPORT == STD_ON)
/** \brief SoAd Address size in byte (IPv6) */
#define SOAD_IPADDRMEMSIZE 16U
#else
/** \brief SoAd Address size in byte (IPv4) */
#define SOAD_IPADDRMEMSIZE 4U
#endif /* (SOAD_IPV6_SUPPORT == STD_ON) */

#if(SOAD_IPV6_SUPPORT == STD_ON)
/** \brief checks if SoAd Address is wildcard (IPv6) */
#define SOAD_IS_ADDRESS_WILDCARD(Address) \
  (((Address)[3] || (Address)[2] || (Address)[1] || (Address)[0]) == 0U)
#else
/** \brief checks if SoAd Address is wildcard (IPv4) */
#define SOAD_IS_ADDRESS_WILDCARD(Address) ((Address)[0] == 0U)
#endif /* SOAD_IPV6_SUPPORT == STD_ON */

/** \brief checks if SoAd Address or port is wildcard */
#define SOAD_IS_REMOTE_WILDCARD(Address, Port) \
  (SOAD_IS_ADDRESS_WILDCARD((Address)) || ((Port) == 0U))

/** \brief simplify modulo 32
 **
 ** This macro uses a bit mask of 31 which is 1F to simulate modulo 32.
 **
 ** \param[in] Dividend of modulo operation
 **
 ** \return Dividend % 32
 **
 */
#if (defined SOAD_MODULO_32)
#error SOAD_MODULO_32 is already defined
#endif
#define SOAD_MODULO_32(Dividend) \
  ((Dividend) & 0x1FU)

/** \brief simplify modulo 8
 **
 ** This macro uses a bit mask of 7 which is 0x07 to simulate modulo 8.
 **
 ** \param[in] Dividend of modulo operation
 **
 ** \return Dividend % 8
 **
 */
#if (defined SOAD_MODULO_8)
#error SOAD_MODULO_8 is already defined
#endif
#define SOAD_MODULO_8(Dividend) \
  ((Dividend) & 0x07U)

/** \brief Get a value from a BitArray
 **
 ** This macro shall be used to check for a single bit in a BitArray.
 **
 ** \param[in] BitArray  Pointer to the BitArray
 ** \param[in] Index     Index of the bit to be checked
 **
 ** \return 0 if flag was not set, another value otherwise
 **
 ** \remarks Please note that the caller must make sure that this macro is
 **          executed in a critical section.
 */
#if (defined SOAD_GET_BIT_FROM_BITARRAY)
#error SOAD_GET_BIT_FROM_BITARRAY is already defined
#endif
#define SOAD_GET_BIT_FROM_BITARRAY(BitArray, Index) \
  ((BitArray)[(Index)>>3U] & (uint8)(1U<<SOAD_MODULO_8(Index)))

/** \brief Get a value from a BitArray
 **
 ** This macro shall be used to check for a single bit in a BitArray.
 **
 ** \param[in] BitArray  Pointer to the BitArray
 ** \param[in] Index     Index of the bit to be checked
 **
 ** \return 0 if flag was not set, another value otherwise
 **
 ** \remarks Please note that the caller must make sure that this macro is
 **          executed in a critical section.
 */
#if (defined SOAD_GET_BIT_FROM_32BIT_BITARRAY)
#error SOAD_GET_BIT_FROM_32BIT_BITARRAY is already defined
#endif
#define SOAD_GET_BIT_FROM_32BIT_BITARRAY(BitArray, Index) \
  ((BitArray)[(Index)>>5U] & (uint32)(1U<<SOAD_MODULO_32(Index)))

/** \brief Set a value in a BitArray
 **
 ** This macro shall be used for setting a single bit in a BitArray.
 **
 ** \param[in/out] BitArray  Pointer to the BitArray
 ** \param[in]     Index     Index of the bit to be set
 **
 ** \remarks Please note that the caller must make sure that this macro is
 **          executed in a critical section.
 **          Note, that index is in bit while the array is a byte array. Index 14 means
 **          4th bit in 4th byte.
 */
#if (defined SOAD_SET_BIT_IN_BITARRAY)
#error SOAD_SET_BIT_IN_BITARRAY is already defined
#endif
#define SOAD_SET_BIT_IN_BITARRAY(BitArray, Index) \
  ((BitArray)[(Index) / 8U] |= (uint8)(1U << SOAD_MODULO_8(Index)))

/** \brief Set a value in a BitArray
 **
 ** This macro shall be used for setting a single bit in a BitArray.
 **
 ** \param[in/out] BitArray  Pointer to the BitArray
 ** \param[in]     Index     Index of the bit to be set
 **
 ** \remarks Please note that the caller must make sure that this macro is
 **          executed in a critical section.
 **          Note, that index is in bit while the array is a byte array. Index 14 means
 **          4th bit in 4th byte.
 */
#if (defined SOAD_SET_BIT_IN_32BIT_BITARRAY)
#error SOAD_SET_BIT_IN_32BIT_BITARRAY is already defined
#endif
#define SOAD_SET_BIT_IN_32BIT_BITARRAY(BitArray, Index) \
  ((BitArray)[(Index) >> 5U] |= (uint32)(1U << SOAD_MODULO_32(Index)))

/** \brief Clear a value in a BitArray
 **
 ** This macro shall be used for clearing a single bit in a BitArray.
 **
 ** \param[in/out] BitArray  Pointer to the BitArray
 ** \param[in]     Index     Index of the bit to be set
 **
 ** \remarks Please note that the caller must make sure that this macro is
 **          executed in a critical section.
 */
#if (defined SOAD_CLEAR_BIT_IN_BITARRAY)
#error SOAD_CLEAR_BIT_IN_BITARRAY is already defined
#endif
#define SOAD_CLEAR_BIT_IN_BITARRAY(BitArray, Index) \
  ((BitArray)[(Index)>>3U] &= (uint8)(~(uint8)(1U<<SOAD_MODULO_8(Index))))

/** \brief Clear a value in a BitArray
 **
 ** This macro shall be used for clearing a single bit in a BitArray.
 **
 ** \param[in/out] BitArray  Pointer to the BitArray
 ** \param[in]     Index     Index of the bit to be set
 **
 ** \remarks Please note that the caller must make sure that this macro is
 **          executed in a critical section.
 */
#if (defined SOAD_CLEAR_BIT_IN_32BIT_BITARRAY)
#error SOAD_CLEAR_BIT_IN_32BIT_BITARRAY is already defined
#endif
#define SOAD_CLEAR_BIT_IN_32BIT_BITARRAY(BitArray, Index) \
  ((BitArray)[(Index)>>5U] &= (uint32)(~(uint32)(1U<<SOAD_MODULO_32(Index))))

/** \brief Returns the state of a routing group.
 **
 ** This macro returns the state (enabled/disabled) of the specified routing
 ** group from the RoutingGroupState array.
 **
 ** \param[in] RoutingGroupId Index of the routing group
 **
 ** \returns boolean
 ** \retval TRUE  routing group is enabled
 ** \retval FALSE routing group is disabled
 */
#if (defined SOAD_IS_ROUTINGGROUP_ENABLED)
#error SOAD_IS_ROUTINGGROUP_ENABLED is already defined
#endif
#define SOAD_IS_ROUTINGGROUP_ENABLED(RoutingGroupId) \
  ( \
    SOAD_GET_BIT_FROM_BITARRAY \
    (                                              \
      &SOAD_PBRAM_REF_ID(uint8, SoAd_PBcfg->RoutingGroupStateBitMapBase, 0U), \
      (RoutingGroupId) \
    ) \
    != 0x00U \
  )

/** \brief Enables a routing group.
 **
 ** This macro sets the corresponding bit in the RoutingGroupState array and
 ** thereby enables the routing group.
 **
 ** \param[in] RoutingGroupId Index of the routing group
 */
#if (defined SOAD_ENABLE_ROUTINGGROUP)
#error SOAD_ENABLE_ROUTINGGROUP is already defined
#endif
#define SOAD_ENABLE_ROUTINGGROUP(RoutingGroupId) \
  ( \
    SOAD_SET_BIT_IN_BITARRAY \
    ( \
      &SOAD_PBRAM_REF_ID(uint8, SoAd_PBcfg->RoutingGroupStateBitMapBase, 0U), (RoutingGroupId) \
    ) \
  )

/** \brief Disables a routing group.
 **
 ** This macro clears the corresponding bit in the RoutingGroupState array and
 ** thereby disables the routing group.
 **
 ** \param[in] RoutingGroupId Index of the routing group
 */
#if (defined SOAD_DISABLE_ROUTINGGROUP)
#error SOAD_DISABLE_ROUTINGGROUP is already defined
#endif
#define SOAD_DISABLE_ROUTINGGROUP(RoutingGroupId) \
  ( \
    SOAD_CLEAR_BIT_IN_BITARRAY \
    ( \
      &SOAD_PBRAM_REF_ID( uint8, SoAd_PBcfg->RoutingGroupStateBitMapBase, 0U), (RoutingGroupId) \
    ) \
  )


/** \brief Set trigger flag for a routing group.
 **
 ** This macro sets the corresponding bit in the RoutingGroupTriggered array
 ** and thereby sets the routing group trigger flag.
 **
 ** \param[in] RoutingGroupId Index of the routing group
 **
 ** \remarks Please note that the caller must make sure that this macro is
 **          not executed in a critical section.
 */
#if (defined SOAD_SET_ROUTINGGROUP_TRIGGER)
#error SOAD_SET_ROUTINGGROUP_TRIGGER is already defined
#endif
#define SOAD_SET_ROUTINGGROUP_TRIGGER(RoutingGroupId)  \
  ( \
    SOAD_SET_BIT_IN_32BIT_BITARRAY \
    ( \
      &SOAD_PBRAM_REF_ID(uint32, SoAd_PBcfg->RoutingGroupTriggeredBitMapBase, 0U), \
     (RoutingGroupId) \
    ) \
  )

/** \brief Clear trigger flag for a routing group.
 **
 ** This macro clears the corresponding bit in the RoutingGroupTriggered array
 ** and thereby clears the routing group trigger flag.
 **
 ** \param[in] RoutingGroupId Index of the routing group
 */
#if (defined SOAD_CLEAR_ROUTINGGROUP_TRIGGER)
#error SOAD_CLEAR_ROUTINGGROUP_TRIGGER is already defined
#endif
#define SOAD_CLEAR_ROUTINGGROUP_TRIGGER(RoutingGroupId) \
  ( \
    SOAD_CLEAR_BIT_IN_32BIT_BITARRAY \
    ( \
      &SOAD_PBRAM_REF_ID(uint32, SoAd_PBcfg->RoutingGroupTriggeredBitMapBase, 0U), \
      (RoutingGroupId) \
    ) \
  )

/** \brief Returns the smaller one of two given values.
 ** \param[in] a  first value
 ** \param[in] b  second value
 ** \returns      the smaller of a and b.
 */
#if (defined SOAD_MIN)
#error SOAD_MIN is already defined
#endif
#define SOAD_MIN(a, b) (((a)<(b))?(a):(b))

/** \brief Returns the bigger one of two given values.
 ** \param[in] a  first value
 ** \param[in] b  second value
 ** \returns      the bigger of a and b.
 */
#if (defined SOAD_MAX)
#error SOAD_MAX is already defined
#endif
#define SOAD_MAX(a, b) (((a)>(b))?(a):(b))


/** \brief The data pointer used for all 'octet' data streams */
typedef P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) SoAd_Uint8PtrType;

/** \brief The data pointer used for all 'void' data items */
typedef P2VAR(void, AUTOMATIC, SOAD_APPL_DATA) SoAd_VoidPtrType;

/** \brief The data pointer used for all const 'octet' data streams */
typedef P2CONST(uint8, AUTOMATIC, SOAD_APPL_DATA) SoAd_ConstUint8PtrType;


/** \brief The data pointer used for all 16 bit data items */
typedef P2VAR(uint16, AUTOMATIC, SOAD_APPL_DATA) SoAd_Uint16PtrType;

/** \brief The data pointer used for all const 16 bit data items */
typedef P2CONST(uint16, AUTOMATIC, SOAD_APPL_DATA) SoAd_ConstUint16PtrType;


/** \brief The data pointer used for all 32 bit data items */
typedef P2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) SoAd_Uint32PtrType;

/** \brief The data pointer used for all const 32 bit data items */
typedef P2CONST(uint32, AUTOMATIC, SOAD_APPL_DATA) SoAd_ConstUint32PtrType;

/** \brief Configuration of ComStack_Helper.h macros for the SoAd module.
 **
 */
#if (defined ComStack_Uint8PtrType)
#error ComStack_Uint8PtrType is already defined
#endif
#define ComStack_Uint8PtrType SoAd_Uint8PtrType

#if (defined ComStack_Uint16PtrType)
#error ComStack_Uint16PtrType is already defined
#endif
#define ComStack_Uint16PtrType SoAd_Uint16PtrType

#if (defined ComStack_Uint32PtrType)
#error ComStack_Uint32PtrType is already defined
#endif
#define ComStack_Uint32PtrType SoAd_Uint32PtrType

#if (defined ComStack_ConstUint8PtrType)
#error ComStack_ConstUint8PtrType is already defined
#endif
#define ComStack_ConstUint8PtrType SoAd_ConstUint8PtrType

#if (defined ComStack_ConstUint16PtrType)
#error ComStack_ConstUint16PtrType is already defined
#endif
#define ComStack_ConstUint16PtrType SoAd_ConstUint16PtrType

#if (defined ComStack_ConstUint32PtrType)
#error ComStack_ConstUint32PtrType is already defined
#endif
#define ComStack_ConstUint32PtrType SoAd_ConstUint32PtrType

/*------------------[PBCfg abstraction macros]------------------------------*/

/** \brief Macro to avoid using '##' twice in below macros */
#if (defined SOAD_ADD_TYPE_SUFFIX)
#error SOAD_ADD_TYPE_SUFFIX is already defined
#endif
/* Deviation MISRAC2012-2 */
#define SOAD_ADD_TYPE_SUFFIX(name) name ## Type

#ifdef SOAD_PRECOMPILE_TIME_PBCFG_ACCESS
#if (defined SOAD_CFG)
#error SOAD_CFG is already defined
#endif
#define SOAD_CFG(name, id) \
    (SoAd_ConfigLayout.SoAd_ ## name[id])

#if (defined SOAD_CFG_TYPE)
#error SOAD_CFG_TYPE is already defined
#endif
#define SOAD_CFG_TYPE(type, name, id) \
    SOAD_CFG(name, id)

#else
#if (defined SOAD_CFG)
#error SOAD_CFG is already defined
#endif
/* Deviation MISRAC2012-2 */
#define SOAD_CFG(name, id) \
    SOAD_CFG_TYPE(SOAD_ADD_TYPE_SUFFIX(SoAd_ ## name), name, (id))

#if (defined SOAD_CFG_TYPE)
#error SOAD_CFG_TYPE is already defined
#endif
/* Deviation MISRAC2012-2 */
#define SOAD_CFG_TYPE(type, name, id) \
    (TS_UNCHECKEDGETCFG(SoAd_PBcfg, type, SOAD, SoAd_PBcfg->name ## Ref) [(id)])
#endif /* SOAD_PRECOMPILE_TIME_PBCFG_ACCESS */

#if (defined SOAD_PBRAM)
#error SOAD_PBRAM is already defined
#endif
/* Deviation MISRAC2012-2 */
#define SOAD_PBRAM(name, id) \
    SOAD_PBRAM_TYPE(SOAD_ADD_TYPE_SUFFIX(SoAd_ ## name), name, (id))

#if (defined SOAD_PBRAM_TYPE)
#error SOAD_PBRAM_TYPE is already defined
#endif
/* Deviation MISRAC2012-2 */
#define SOAD_PBRAM_TYPE(type, name, id) \
    SOAD_PBRAM_REF_ID(type, SoAd_PBcfg->name ## Base, id)

#if (defined SOAD_PBRAM_REF_ID)
#error SOAD_PBRAM_REF_ID is already defined
#endif
#define SOAD_PBRAM_REF_ID(type, refToRam, id) \
    (TS_UNCHECKEDGETVAR(SoAd_Mem, type, SOAD, (refToRam)) [(id)])

/*------------------[Defensive programming]---------------------------------*/

/* Macros to store assertions failing in critical sections and raise them later */

#if (defined SOAD_DELAY_PRECONDITION_ASSERT)
#error SOAD_DELAY_PRECONDITION_ASSERT is already defined
#endif
#define SOAD_DELAY_PRECONDITION_ASSERT(condition, storagePtr) \
    SOAD_DELAY_ASSERT((condition), SOAD_DELAY_ASSERTION_PRECONDITION, (storagePtr))

#if (defined SOAD_DELAY_POSTCONDITION_ASSERT)
#error SOAD_DELAY_POSTCONDITION_ASSERT is already defined
#endif
#define SOAD_DELAY_POSTCONDITION_ASSERT(condition, storagePtr) \
    SOAD_DELAY_ASSERT((condition), SOAD_DELAY_ASSERTION_POSTCONDITION, (storagePtr))

#if (defined SOAD_DELAY_INVARIANT_ASSERT)
#error SOAD_DELAY_INVARIANT_ASSERT is already defined
#endif
#define SOAD_DELAY_INVARIANT_ASSERT(condition, storagePtr) \
    SOAD_DELAY_ASSERT((condition), SOAD_DELAY_ASSERTION_INVARIANT, (storagePtr))

#if (defined SOAD_DELAY_UNREACHABLE_CODE_ASSERT)
#error SOAD_DELAY_UNREACHABLE_CODE_ASSERT is already defined
#endif
#define SOAD_DELAY_UNREACHABLE_CODE_ASSERT(storagePtr) \
    SOAD_DELAY_ASSERT(FALSE, SOAD_DELAY_ASSERTION_UNREACHABLE_CODE, (storagePtr))

#if(SOAD_DEFENSIVE_PROGRAMMING_ENABLED == STD_ON)

#if (defined SOAD_DELAY_ASSERT)
#error SOAD_DELAY_ASSERT is already defined
#endif
#define SOAD_DELAY_ASSERT(condition, value, storagePtr) \
  do \
  { \
    if (!(condition)) \
    { \
      *(storagePtr) = (value); \
    } \
  } while(FALSE)

#if (defined SOAD_DELAY_ASSERT_EXECUTE)
#error SOAD_DELAY_ASSERT_EXECUTE is already defined
#endif
#define SOAD_DELAY_ASSERT_EXECUTE(svcid, storagePtr) \
  do \
  { \
    SOAD_PRECONDITION_ASSERT(*(storagePtr) != SOAD_DELAY_ASSERTION_PRECONDITION, (svcid)); \
    SOAD_POSTCONDITION_ASSERT(*(storagePtr) != SOAD_DELAY_ASSERTION_POSTCONDITION, (svcid)); \
    SOAD_INVARIANT_ASSERT(*(storagePtr) != SOAD_DELAY_ASSERTION_INVARIANT, (svcid)); \
    if(*(storagePtr) == SOAD_DELAY_ASSERTION_UNREACHABLE_CODE) \
    { \
      SOAD_UNREACHABLE_CODE_ASSERT((svcid)); \
    } \
  } while(FALSE)

#else /* SOAD_DEFENSIVE_PROGRAMMING_ENABLED == STD_ON */

/** \brief Disabled */
#if (defined SOAD_DELAY_ASSERT)
#error SOAD_DELAY_ASSERT is already defined
#endif
#define SOAD_DELAY_ASSERT(condition, value, storagePtr)

/** \brief Disabled */
#if (defined SOAD_DELAY_ASSERT_EXECUTE)
#error SOAD_DELAY_ASSERT_EXECUTE is already defined
#endif
#define SOAD_DELAY_ASSERT_EXECUTE(svcid, storagePtr)

#endif /* SOAD_DEFENSIVE_PROGRAMMING_ENABLED == STD_ON */

/*------------------[Deferred Det calling]----------------------------------*/

#if (defined SOAD_DELAY_DET_REPORT_ERROR)
#error SOAD_DELAY_DET_REPORT_ERROR is already defined
#endif
#define SOAD_DELAY_DET_REPORT_ERROR(errid, storagePtr) \
  do \
  { \
    *(storagePtr) = (errid); \
  } while(FALSE)

#if (defined SOAD_DELAY_DET_EXECUTE)
#error SOAD_DELAY_DET_EXECUTE is already defined
#endif
#define SOAD_DELAY_DET_EXECUTE(svcid, storagePtr) \
  do \
  { \
    if(*(storagePtr) != SOAD_ERRID_NONE) \
    { \
      SOAD_DET_REPORT_ERROR((svcid), *(storagePtr)); \
    } \
  } while(FALSE)

/*------------------[Error codes]-------------------------------------------*/

/** \brief Indicates no error happened, yet
 ** This macro is used for delayed DET reports. */
#if(defined SOAD_ERRID_NONE)
#error SOAD_ERRID_NONE is already defined
#endif
#define SOAD_ERRID_NONE 0x00U

/*------------------[API call abstractions]---------------------------------*/

/** \brief Macro to report a Det error when called only with the ApiId and ErrorId */
#if(defined SOAD_DET_REPORT_ERROR)
#error SOAD_DET_REPORT_ERROR is already defined
#endif
/* Deviation MISRAC2012-1 <START> */
#define SOAD_DET_REPORT_ERROR(ApiId, ErrorId) \
    ((void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID, (ApiId), (ErrorId)))
/* Deviation MISRAC2012-1 <STOP> */

/*------------------[RingBuffer access macros]------------------------------*/

#if(defined SOAD_RINGBUFFER_INPTR)
#error SOAD_RINGBUFFER_INPTR is already defined
#endif
#define SOAD_RINGBUFFER_INPTR(rb)  (&((rb)->BufPtr[(rb)->In]))

#if(defined SOAD_RINGBUFFER_OUTPTR)
#error SOAD_RINGBUFFER_OUTPTR is already defined
#endif
#define SOAD_RINGBUFFER_OUTPTR(rb) (&((rb)->BufPtr[(rb)->Out]))

#if(defined SOAD_RINGBUFFER_USED)
#error SOAD_RINGBUFFER_USED is already defined
#endif
#define SOAD_RINGBUFFER_USED(rb) ((rb)->Used)

#if(defined SOAD_RINGBUFFER_FREE)
#error SOAD_RINGBUFFER_FREE is already defined
#endif
#define SOAD_RINGBUFFER_FREE(rb) ((rb)->Size-(rb)->Used)

/** \brief Returns the amount of bytes that can be read from the ring buffer continuously,
 **        i.e.without a buffer wrap-around.*/
#if(defined SOAD_RINGBUFFER_CONTUSED)
#error SOAD_RINGBUFFER_CONTUSED is already defined
#endif
#define SOAD_RINGBUFFER_CONTUSED(rb) SOAD_MIN((rb)->Size-(rb)->Out, (rb)->Used)

/** \brief Returns the amount of bytes that can be written into the ring buffer continuously,
 **        i.e.without a buffer wrap-around.*/
#if(defined SOAD_RINGBUFFER_CONTFREE)
#error SOAD_RINGBUFFER_CONTFREE is already defined
#endif
#define SOAD_RINGBUFFER_CONTFREE(rb) SOAD_MIN((rb)->Size-(rb)->In, SOAD_RINGBUFFER_FREE(rb))

/* ====== [Additional macros] ====== */

/** \brief Invalid socket ID
 *  This macro defines an invalid socket ID for TcpIp_SocketIdType. This value is already used by
 *  TcpIp internally. Replace SOAD_SOCKETID_INVALID with TCPIP_SOCKETID_INVALID as soon as
 *  available.
 */
#if(defined SOAD_SOCKETID_INVALID)
#error SOAD_SOCKETID_INVALID is already defined
#endif
#ifndef TCPIP_SOCKETID_INVALID
#define SOAD_SOCKETID_INVALID 0xFFFFU
#else
#define SOAD_SOCKETID_INVALID TCPIP_SOCKETID_INVALID
#endif

#undef TS_RELOCATABLE_CFG_ENABLE

/*==================[type definitions]======================================*/

#if(SOAD_DEFENSIVE_PROGRAMMING_ENABLED == STD_ON)
/** \brief Type to store assertion violations detected inside critical sections
 * (in order to report them afterwards) */
typedef uint8 SoAd_DelayedAssertionType;
#if(defined SOAD_DELAY_ASSERTION_NONE)
#error SOAD_DELAY_ASSERTION_NONE is already defined
#endif
#define SOAD_DELAY_ASSERTION_NONE 0U

#if(defined SOAD_DELAY_ASSERTION_PRECONDITION)
#error SOAD_DELAY_ASSERTION_PRECONDITION is already defined
#endif
#define SOAD_DELAY_ASSERTION_PRECONDITION 1U

#if(defined SOAD_DELAY_ASSERTION_POSTCONDITION)
#error SOAD_DELAY_ASSERTION_POSTCONDITION is already defined
#endif
#define SOAD_DELAY_ASSERTION_POSTCONDITION 2U

#if(defined SOAD_DELAY_ASSERTION_INVARIANT)
#error SOAD_DELAY_ASSERTION_INVARIANT is already defined
#endif
#define SOAD_DELAY_ASSERTION_INVARIANT 3U

#if(defined SOAD_DELAY_ASSERTION_UNREACHABLE_CODE)
#error SOAD_DELAY_ASSERTION_UNREACHABLE_CODE is already defined
#endif
#define SOAD_DELAY_ASSERTION_UNREACHABLE_CODE 4U

#endif /* SOAD_DEFENSIVE_PROGRAMMING_ENABLED == STD_ON */

/** \brief Type to characterize the level of an address match
 *
 * \note The the address match values are sorted based on the address match. The higher the value
 *       the better the match.
 */
typedef uint8 SoAd_AddressMatchType;
#if(defined SOAD_ADDRESSMATCH_NONE)
#error SOAD_ADDRESSMATCH_NONE is already defined
#endif
#define SOAD_ADDRESSMATCH_NONE 0U /* No match. */

#if(defined SOAD_ADDRESSMATCH_WILDCARD)
#error SOAD_ADDRESSMATCH_WILDCARD is already defined
#endif
#define SOAD_ADDRESSMATCH_WILDCARD 1U /* IpAddr and Port are wild cards. */

#if(defined SOAD_ADDRESSMATCH_PORT)
#error SOAD_ADDRESSMATCH_PORT is already defined
#endif
#define SOAD_ADDRESSMATCH_PORT 2U /* Port matches, IpAddr is a wild card. */

#if(defined SOAD_ADDRESSMATCH_IPADDR)
#error SOAD_ADDRESSMATCH_IPADDR is already defined
#endif
#define SOAD_ADDRESSMATCH_IPADDR 3U /* IpAddr matches, Port is a wild card. */

#if(defined SOAD_ADDRESSMATCH_EXACT)
#error SOAD_ADDRESSMATCH_EXACT is already defined
#endif
#define SOAD_ADDRESSMATCH_EXACT 4U  /* IpAddr and Port match. */

/** \brief Type to distinguish RoutingGroup-related actions (enable, disable, trigger).
 * Used to unify the RoutingGroup-related APIs in the internal SoAd_RoutingGroupAction function. */
typedef uint8 SoAd_RoutingGroupActionType;
#if(defined SOAD_ROUTINGGROUPACTION_ENABLE)
#error SOAD_ROUTINGGROUPACTION_ENABLE is already defined
#endif
#define SOAD_ROUTINGGROUPACTION_ENABLE 0U /* SoAd_EnableRouting, SoAd_EnableSpecificRouting */

#if(defined SOAD_ROUTINGGROUPACTION_DISABLE)
#error SOAD_ROUTINGGROUPACTION_DISABLE is already defined
#endif
#define SOAD_ROUTINGGROUPACTION_DISABLE 1U /* SoAd_DisableRouting, SoAd_DisableSpecificRouting */

#if(defined SOAD_ROUTINGGROUPACTION_TRIGGER)
#error SOAD_ROUTINGGROUPACTION_TRIGGER is already defined
#endif
#define SOAD_ROUTINGGROUPACTION_TRIGGER 2U /* SoAd_IfRoutingGroupTransmit,
                                              SoAd_IfSpecificRoutingGroupTransmit */

#if(defined SOAD_PDULENGTHTYPE_MAX)
#error SOAD_PDULENGTHTYPE_MAX is already defined
#endif
#ifndef TS_SIZE_PduLengthType
#error TS_SIZE_PduLengthType is not defined
#endif
#if (TS_SIZE_PduLengthType == 2U)
#define SOAD_PDULENGTHTYPE_MAX 0xFFFFU
#else
#define SOAD_PDULENGTHTYPE_MAX 0xFFFFFFFFU
#endif

#if(defined SOAD_UDP_PDULENGTH_MAX)
#error SOAD_UDP_PDULENGTH_MAX is already defined
#endif
#define SOAD_UDP_PDULENGTH_MAX 0xFFFFU

/** \brief SoAd_GetPduRouteDestVirtualFromPduTxDataId() returns pointer to PduRouteId
 **
 ** This function returns PduRouteDestVirtualPtr based on the PduTxDataId. This function
 ** assumes that PduTxDataId is valid.
 ** An invalid memory access will be detected by defensive programming mechanism.
 **
 ** \param[in] PduTxDataId - Id to the PduTxData structure
 **
 ** \param[out] PduRouteDestVirtualPtrPtr - Returns pointer addr to the corresponding PDU route dest.
 **/
#if(defined SoAd_GetPduRouteDestVirtualFromPduTxDataId)
#error SoAd_GetPduRouteDestVirtualFromPduTxDataId is already defined
#endif
#define SoAd_GetPduRouteDestVirtualFromPduTxDataId(PduTxDataId, PduRouteDestVirtualPtrPtr) \
do \
{ \
  SoAd_PduRouteDestVirtualIdType PduRouteDestVirtualId_Tmp;\
  SOAD_PRECONDITION_ASSERT((PduTxDataId) != SOAD_INVALID_PDUTXDATA_ID , DET_INTERNAL_API_ID);\
  PduRouteDestVirtualId_Tmp = SOAD_CFG_TYPE(SoAd_PduRouteDestVirtualIdType, PduTxData_PduRoute_Map, (PduTxDataId));\
  *(PduRouteDestVirtualPtrPtr) = &SOAD_CFG(PduRouteDestVirtual, PduRouteDestVirtualId_Tmp);\
}while(0)

/** \brief SoAd_GetSoConTxBufferState() returns pointer to SoConTxBufferState
 **
 ** SoAd_GetSoConTxBufferState() returns pointer to the SoConTxBufferState struct of the corresponding
 ** socket connection. An invalid memory access will be detected by defensive programming mechanism.
 **
 ** \param[in] SoConPtr - Pointer to socket connection configuration
 **
 ** \param[out] SoAdSoConTxBufferStatePtr - Pointer to the SoConTxBufferState struct
 **             of the socket connection.
 **/
#define SoAd_GetSoConTxBufferState(SoConPtr, SoAdSoConTxBufferStatePtr ) \
do \
{ \
  SOAD_PRECONDITION_ASSERT(SoConPtr->TxBufferId != SOAD_INVALID_SOCON_ID, DET_INTERNAL_API_ID); \
  (SoAdSoConTxBufferStatePtr) = &SOAD_PBRAM(SoConTxBufferState, SoConPtr->TxBufferId);\
}while(0)

/** \brief SoAd_GetSoConTxBufferCfg() returns pointer to SoConTxBufferCfg
 **
 ** SoAd_GetSoConTxBufferCfg() returns pointer to the SoConTxBufferCfg struct of the corresponding
 ** socket connection. An invalid memory access will be detected by defensive programming mechanism.
 **
 ** \param[in] SoConPtr - Pointer to socket connection configuration
 **
 ** \param[out] SoAdSoConTxBufferCfgPtr - Pointer to the SoConTxBufferCfg struct
 **            of the socket connection.
 **/
#define SoAd_GetSoConTxBufferCfg(SoConPtr, SoAdSoConTxBufferCfgPtr ) \
do \
{  \
  SOAD_PRECONDITION_ASSERT(SoConPtr->TxBufferId != SOAD_INVALID_SOCON_ID, DET_INTERNAL_API_ID); \
  (SoAdSoConTxBufferCfgPtr) = &SOAD_CFG(SoConTxBufferCfg, SoConPtr->TxBufferId);\
}while(0)

/** \brief SoAd_GetSoConTxData() returns pointer to SoConTxData
 **
 ** SoAd_GetSoConTxData() returns pointer to the SoConTxData struct of the corresponding socket
 ** connection. An invalid memory access will be detected by defensive programming mechanism.
 **
 ** \param[in] SoConPtr - Pointer to socket connection configuration
 **
 ** \param[out] SoConTxDataPtr - Pointer to the SoConTxData struct of the socket connection.
 **/
#define SoAd_GetSoConTxData(SoConPtr, SoConTxDataPtr ) \
do \
{  \
  SOAD_PRECONDITION_ASSERT(SoConPtr->TxDataId != SOAD_INVALID_SOCON_ID, DET_INTERNAL_API_ID); \
  (SoConTxDataPtr)  = &SOAD_PBRAM(SoConTxData, SoConPtr->TxDataId);\
}while(0)

/** \brief SoAd_GetSoConTxTcpData() returns pointer to SoConTxTcpData
 **
 ** SoAd_GetSoConTxTcpData() returns pointer to the SoConTxTcpData struct of the corresponding socket
 ** connection. An invalid memory access will be detected by defensive programming mechanism.
 **
 ** \param[in] SoConPtr - Pointer to socket connection configuration
 **
 ** \param[out] SoConTxTcpDataPtr - Pointer to the SoConTxTcpData struct of the socket connection.
 **/
#define SoAd_GetSoConTxTcpData(SoConPtr, SoConTxTcpDataPtr ) \
do \
{  \
  SOAD_PRECONDITION_ASSERT(SoConPtr->SoConTxTcpDataId != SOAD_INVALID_SOCON_ID, DET_INTERNAL_API_ID); \
  (SoConTxTcpDataPtr)  = &SOAD_PBRAM(SoConTxTcpData, SoConPtr->SoConTxTcpDataId);\
}while(0)

/** \brief SoAd_GetPduTxTcpData() returns pointer to PduTxTcpData
 **
 ** SoAd_GetPduTxTcpData() returns pointer to the PduTxTcpData struct of the corresponding pdu
 ** connection. An invalid memory access will be detected by defensive programming mechanism.
 **
 ** \param[in] PduRouteDestVirtualPtr - Pointer to virtual destination
 **
 ** \param[out] PduTxTcpDataPtr - Pointer to the PduTxTcpData struct.
 **/
#define SoAd_GetPduTxTcpData(PduRouteDestVirtualPtr, PduTxTcpDataPtr) \
do \
{  \
  SOAD_PRECONDITION_ASSERT(PduRouteDestVirtualPtr->PduTxTcpDataId != SOAD_INVALID_PDUTXTCPDATA_ID, DET_INTERNAL_API_ID); \
  (PduTxTcpDataPtr)  = &SOAD_PBRAM(PduTxTcpData, PduRouteDestVirtualPtr->PduTxTcpDataId);\
}while(0)

/** \brief SoAd_GetSoConRxData() returns pointer to SoConRxData
 **
 ** SoAd_GetSoConRxData() returns pointer to the SoConRxData struct of the corresponding socket
 ** connection. An invalid memory access will be detected by defensive programming mechanism.
 **
 ** \param[in] SoConPtr - Pointer to socket connection configuration
 **
 ** \param[out] SoConRxDataPtr - Pointer to the SoConTxData struct of the socket connection.
 **/
#define SoAd_GetSoConRxData(SoConPtr, SoConRxDataPtr) \
do \
{  \
  SOAD_PRECONDITION_ASSERT(SoConPtr->RxDataId != SOAD_INVALID_SOCON_ID, DET_INTERNAL_API_ID); \
  (SoConRxDataPtr) = &SOAD_PBRAM(SoConRxData, SoConPtr->RxDataId);\
}while(0)


/* typedefs to avoid misra warnings */
typedef P2VAR(SoAd_SoConTxDataType, AUTOMATIC, SOAD_APPL_DATA) SoAd_SoConTxDataPtrType;
typedef P2VAR(SoAd_SoConTxTcpDataType, AUTOMATIC, SOAD_APPL_DATA) SoAd_SoConTxTcpDataPtrType;
typedef P2VAR(SoAd_PduTxTcpDataType, AUTOMATIC, SOAD_APPL_DATA) SoAd_PduTxTcpDataPtrType;
typedef P2VAR(SoAd_SoConRxDataType, AUTOMATIC, SOAD_APPL_DATA) SoAd_SoConRxDataPtrType;
typedef P2CONST(SoAd_SoConTxBufferCfgType, AUTOMATIC, SOAD_APPL_DATA) SoAd_SoConTxBufferCfgPtrType;
typedef P2VAR(SoAd_SoConTxBufferStateType, AUTOMATIC, SOAD_APPL_DATA)
                                                        SoAd_SoConTxBufferStatePtrType;

/*==================[external function declarations]========================*/

#define SOAD_START_SEC_CODE
#include <SoAd_MemMap.h>

TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_ULTpRxIndication
(
  P2CONST(SoAd_SocketRouteDestType, AUTOMATIC, SOAD_APPL_DATA) SocketRouteDestPtr,
  NotifResultType Result
);

TS_MOD_PRIV_DECL FUNC(boolean, SOAD_CODE) SoAd_ResetRemoteAddress
(
  P2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  P2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_ULTpTxConfirmation
(
  SoAd_RouteIdType PduRouteId,
  NotifResultType Result
);

/** \brief SoAd_GetActivePduRouteDestVirtualId returns the PduRouteDestVirtualId of the running
 **  transmission is active. If there are currently no transmissions SOAD_INVALID_ROUTE_ID
 **  will be returned.
 **
 ** \param[in] SoConPtr - Pointer to the configuration of the socket connection
 **
 ** \returns    Index of an active SoAd_PduRoute or SOAD_INVALID_ROUTE_ID
 */
TS_MOD_PRIV_DECL FUNC(SoAd_PduRouteDestVirtualIdType, SOAD_CODE) SoAd_GetActivePduRouteDestVirtualId
(
  P2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr
);

TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_StartUpSoCon
(
  P2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr
);

/** \brief Assigns the current remote address of the specified socket connection
 **        to the given IP-Address data structure.
 ** \param[in] SoConPtr - Pointer to the socket connection configuration
 ** \param[in] SoConStatePtr - Pointer to the state information of the socket connection
 ** \param[out] AddressDstPtr - Pointer to the IP-Address data structure to be filled
 **/
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_AssignRemoteAddressSoCon2SockAddrType
(
  P2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  P2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) AddressDstPtr
);

/** \brief Assigns the IP-Address of the given data structure as current remote address
 **        of the specified socket connection.
 ** \param[in] SoConPtr - Pointer to the socket connection configuration
 ** \param[in] SoConStatePtr - Pointer to the state information of the socket connection
 ** \param[in] AddressSrcPtr - Pointer to the IP-Address data structure to be assigned
 **/
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_AssignRemoteAddressSockAddrType2SoCon
(
  P2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  P2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) AddressSrcPtr
);


/** \brief SoAd_StartAliveSupervisionTimer is used to calculate and start the timer used for the
 *         alive supervision.
 ** \param[in] SoConGroupPtr - Pointer to the socket group configuration
 ** \param[in] SoConStatePtr - Pointer to the state information of the socket connection
 **/
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_StartAliveSupervisionTimer
(
  P2CONST(SoAd_SoConGroupType, AUTOMATIC, SOAD_APPL_DATA) SoConGroupPtr,
  P2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

/** \brief SoAd_HandleUdpAliveTimeout is used to handle the timeout actions:
 **      - Change the socket state to RECONNECT.
 **      - Deactivate the alive supervision timer.
 **      - Reset the remote address to the configured one.
 **/
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_HandleUdpAliveTimeout(void);

/** \brief SoAd_ResetWildCardSoConInPduRoute resets all socket connections in the given PduRoute
 **        if they are configured for automatic setup and use a wild card for the remote address.
 **
 ** \note  TCP socket connections will be closed and reopened in the the context of subsequent
 **        SoAd_MainFunctions.
 **
 ** \param[in] PduRouteId - Index of the PDU route
 **/
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_ResetWildCardSoConInPduRoute
(
  SoAd_RouteIdType PduRouteId
);

/** \brief Helper function to start a TP reception for an upper layer.
 **
 ** \param[in] SoConPtr - Pointer to the socket connection configuration.
 ** \param[in] SocketRoutePtr - Pointer to the socket route the PDU belongs to.
 ** \param[in] TpSduLength - Length of the received PDU.
 ** \param[out] BufferSizePtr - Pointer to indicate available upper layer buffer.
 **/
TS_MOD_PRIV_DECL FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_ULTpStartofReception
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  P2CONST(SoAd_SocketRouteType, AUTOMATIC, SOAD_APPL_DATA) SocketRoutePtr,
  PduLengthType TpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_DATA) BufferSizePtr
);

/** \brief SoAd_ResetWildCardSoCon resets the given socket connection if is configured for automatic
 **        setup and uses a wild card for the remote address.
 **
 ** \pre This function needs shared variable protection.
 **
 ** \note  TCP socket connections will be closed and reopened in the the context of subsequent
 **        SoAd_MainFunctions.
 **
 ** \param[in] SoConPtr - Pointer to the socket connection configuration
 ** \param[in] SoConStatePtr - Pointer to the state information of the socket connection
 **/
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_ResetWildCardSoCon
(
  P2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  P2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

/** \brief Helper function to initialize Tx data structure.
 **
 **
 ** \param[in] SoConPtr - Pointer to socket connection configuration.
 **
 ** \returns void
 **/
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_InitSoConTxDataStructures
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr
);

/** \brief Helper function to initialize Rx data structure.
 **
 **
 ** \param[in] SoConPtr - Pointer to socket connection configuration.
 **
 ** \returns void
 **/
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_InitSoConRxDataStructures
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr
);

/** \brief Helper function to update the SoConTransCondChangeBitMapBase.
 **
 **
 ** \param[in] SoConPtr - Pointer to socket connection configuration.
 **
 ** \returns void
 **/
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_SetBitInTransCondChange
(
  P2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr
);
/** \brief Helper function to update generic BitMap.
 **
 **
 ** \param[in] SoConPtr - Pointer to socket connection configuration.
 ** \param[in] BitMapPtr - Pointer to BitMap which shall be changed.
 ** \param[in] GlobalVarChangeIndicatorPtr - Pointer to boolean that shall guard the map.
 **
 ** \returns void
 **/
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_SetBitInBitMap
(
  P2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  P2CONST(SoAd_RefToRAM, AUTOMATIC, SOAD_APPL_DATA) BitMapPtr,
  P2VAR(boolean,AUTOMATIC,SOAD_APPL_DATA) GlobalVarChangeIndicatorPtr
);
/** \brief Helper function to update the SoConTransCondChangeBitMapBase.
 **
 **
 ** \param[in] SoConPtr - Pointer to socket connection configuration.
 ** \param[in] BitMapPtr - Pointer to BitMap which shall be cleared.
 **
 ** \returns void
 **/
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_ClearBitInBitMap
(
  P2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  P2CONST(SoAd_RefToRAM, AUTOMATIC, SOAD_APPL_DATA) BitMapPtr
);

TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_ClearBitIn32BitMap
(
    uint32 DestId,
    P2CONST(SoAd_RefToRAM, AUTOMATIC, SOAD_APPL_DATA) BitMapPtr
);

TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_SetBitIn32BitMap
(
    uint32 DestId,
    P2CONST(SoAd_RefToRAM, AUTOMATIC, SOAD_APPL_DATA) BitMapPtr
);


/** \brief Helper function to set virtual destination state and not idle bit maps
 **
 **
 ** \param[in] PduRouteDestVirtualPtr - Pointer to PduRoute virtual destination
 ** \param[in] RequestedTxState - Requested state to be set.
 **
 ** \returns void
 **/
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_SetPduRouteDestVirtualState
(
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr,
  SoAd_PduRouteDestVirtualStateType RequestedTxState
);

#define SOAD_STOP_SEC_CODE
#include <SoAd_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

#define SOAD_START_SEC_VAR_INIT_8
#include <SoAd_MemMap.h>

extern VAR(uint8, SOAD_VAR) SoAd_Initialized;

#define SOAD_STOP_SEC_VAR_INIT_8
#include <SoAd_MemMap.h>

#define SOAD_START_SEC_VAR_INIT_UNSPECIFIED
#include <SoAd_MemMap.h>

extern P2CONST(SoAd_ConfigType, SOAD_VAR, SOAD_APPL_CONST) SoAd_PBcfg;

#define SOAD_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <SoAd_MemMap.h>

#define SOAD_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <SoAd_MemMap.h>

extern VAR(SoAd_TimerType, SOAD_VAR) SoAd_CurrentTimerValue;
extern VAR(SoAd_TimerType, SOAD_VAR) SoAd_NextUdpAliveTimeout;

#define SOAD_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <SoAd_MemMap.h>

#define SOAD_START_SEC_VAR_CLEARED_8
#include <SoAd_MemMap.h>

/** \brief This global flag is set if any routing group got triggered
 **
 ** This flag indicates if any routing group got triggered. It reduces the main function idle
 ** execution time.
 **/
extern VAR(boolean, SOAD_VAR) SoAd_RoutingGroupTriggered;

/** \brief This global flag is set if any state transition got triggered
 **
 ** This flag indicates if any state transition got triggered. It reduces the main function idle
 ** execution time.
 **/
extern VAR(boolean, SOAD_VAR) SoAd_StateTransCondChange;

/** \brief This global flag is set if any rx state transition got triggered
 **
 ** This flag indicates if any state transition got triggered. It reduces the main function idle
 ** execution time.
 **/
extern VAR(boolean, SOAD_VAR) SoAd_RxCondChange;

/** \brief This global flag is set if any tx tp state transition got triggered
 **
 ** This flag indicates if any state transition got triggered. It reduces the main function idle
 ** execution time.
 **/
extern VAR(boolean, SOAD_VAR) SoAd_TxTpCondChange;

/** \brief This global flag is set if any tx if state transition got triggered
 **
 ** This flag indicates if any state transition got triggered. It reduces the main function idle
 ** execution time.
 **/
extern VAR(boolean, SOAD_VAR) SoAd_TxIfCondChange;

#define SOAD_STOP_SEC_VAR_CLEARED_8
#include <SoAd_MemMap.h>


#define SOAD_START_SEC_VAR_INIT_32
#include <SoAd_MemMap.h>

#if (SOAD_GETANDRESETMEASUREMENTDATA_API == STD_ON)
/** \brief counters for number of dropped invalid TCP or UDP Port PDUs */
extern VAR(uint32, SOAD_VAR) SoAd_MeasurementDropData_Tcp;
extern VAR(uint32, SOAD_VAR) SoAd_MeasurementDropData_Udp;
#endif /* SOAD_GETANDRESETMEASUREMENTDATA_API == STD_ON */

#define SOAD_STOP_SEC_VAR_INIT_32
#include <SoAd_MemMap.h>

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/
#define SOAD_START_SEC_CODE
#include <SoAd_MemMap.h>

/** \brief Service to perform Tp Rx and If Tx main function tasks.
 **
 **
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_MainFunctionRx(void);

TS_MOD_PRIV_DECL FUNC(void,SOAD_CODE) SoAd_MainFunctionTxIf(void);

#if(SOAD_ENABLE_MAIN_FUNCTION_TX != STD_ON)
/** \brief Main function containing only transmit part of the SoAd
 **
 ** Transmit part of the main function if enabled to be external
 **
 ** \ServiceID{0xF2}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_MainFunctionTx(void);
#endif

/** \brief Finish a Tp transmission
 **
 ** This function shall be called at the end of a Tp transmission
 ** - to perform a cleanup to be ready for the next transmission.
 ** - to send a TpTxConfrimation() for its upper layer if required.
 **
 ** \param[in] SoConPtr - Pointer to the socket connection configuration
 ** \param[in] SoConStatePtr - Pointer to the state information of the socket connection
 ** \param[in] PduRouteDestVirtualPtr - Pointer to the virtual dest of the transmission.
 **/
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_Int_05_TpFinishTransmission
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr
);


TS_MOD_PRIV_DECL FUNC(SoAd_SoConIdType, SOAD_CODE) SoAd_SocketToSoConId
(
  TcpIp_SocketIdType SocketId
);

TS_MOD_PRIV_DECL FUNC(SoAd_SoConIdType, SOAD_CODE) SoAd_ListenSocketToSoConId
(
  TcpIp_SocketIdType ListenSocketId
);

/** \brief Service to find the best socket connection in a socket connection group.
 **
 ** The service SoAd_BestMatchAlgorithm() tries to find the best socket connection in a socket
 ** connection group based on the remote address.
 **
 ** \param[in] SoConPtr - Socket connection pointer. This pointer defines the socket connection
 **                       group for the search.
 ** \param[in] RemoteAddrPtr - Pointer to remote address and port.
 ** \param[out] MatchingSoConIdPtr - Pointer to return the best matching SoConId. In case of
 **                                  no match SoConId is returned. Pointer shall be valid.
 ** \param[out] AddressMatchPtr - Pointer to return the quality of the address match.
 **                               Pointer shall be valid.
 ** \param[in] SocketRouteRequired - Defines if the found socket connection shall have a socket
 **                                  route or not.
 ** \param[in] IncludeClosedSoCon - Defines if closed socket connections shall be considered or not.
 **
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_BestMatchAlgorithm
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) RemoteAddrPtr,
  CONSTP2VAR(SoAd_SoConIdType, AUTOMATIC, SOAD_APPL_DATA) MatchingSoConIdPtr,
  CONSTP2VAR(SoAd_AddressMatchType, AUTOMATIC, SOAD_APPL_DATA) AddressMatchPtr,
  boolean SocketRouteRequired,
  boolean IncludeClosedSoCon
);

TS_MOD_PRIV_DECL FUNC(Std_ReturnType, SOAD_CODE) SoAd_PrepareTransmit
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(SoAd_SoConTxDataType, AUTOMATIC, SOAD_APPL_DATA) TxDataPtr,
  CONSTP2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) SoAdSrcPduInfoPtr,
  CONSTP2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_DATA) TcpIpTxLen,
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr
);


TS_MOD_PRIV_DECL FUNC(uint8, SOAD_CODE) SoAd_CopyTxData_CopyPduHeader
(
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr,
  CONSTP2VAR(SoAd_SoConTxDataType, AUTOMATIC, SOAD_APPL_DATA) SoConTxDataPtr,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  CONSTP2VAR(SoAd_BufLengthType, AUTOMATIC, SOAD_APPL_DATA) RemainingBufLengthPtr
);

TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_HandleTriggeredRoutingGroups(void);

TS_MOD_PRIV_DECL FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_CopyTxData_TpTcp
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) BufPtr,
  CONSTP2CONST(SoAd_PduRouteDestVirtualType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestVirtualPtr,
  uint16 BufLength
);



/*==================[internal function definitions]=========================*/

#define SOAD_STOP_SEC_CODE
#include <SoAd_MemMap.h>

#endif /* ifndef SOAD_INT_H */
/*==================[end of file]===========================================*/
