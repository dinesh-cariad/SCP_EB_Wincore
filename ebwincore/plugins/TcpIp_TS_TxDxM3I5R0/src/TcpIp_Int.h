/**
 * \file
 *
 * \brief AUTOSAR TcpIp
 *
 * This file contains the implementation of the AUTOSAR
 * module TcpIp.
 *
 * \version 3.5.13
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef TCPIP_INT_H
#define TCPIP_INT_H

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule 20.10 (advisory)
 * The # and ## preprocessor operators should not be used.
 *
 * Reason:
 * 1) Usage of concatenation operator improves readability of code.
 * 2) There is no other way to concatenate identifiers, which is needed for specific code patterns.
 *
 * MISRAC2012-2) Deviated Rule 11.3 (advisory)
 * A cast shall not be performed between a pointer to object type and a pointer to a different
 * object type.
 *
 * Reason:
 * In order to verify that a pointer is aligned a conversion of a pointer to
 * an integer type is required to do bit pattern tests on the low bits.
 *
 * MISRAC2012-3) Deviated Rule: 12.2 (Required)
 * The right hand operand of a shift operator shall lie in the range zero to one less than the
 * width in bits of the essential type of the left hand operand.
 *
 * Reason:
 * If a cast to the correct range was done before the shift, then this rule is a false
 * positive.
 *
 */

/*==================[inclusions]============================================*/

/* source code needs no post-build config of itself or any other module */
/* these macro definitions help to avoid inclusion of <Mod>_PBcfg.h files when including foreign
 * headerfiles
 */
#define TCPIP_NO_PBCFG_REQUIRED
#define SOAD_NO_PBCFG_REQUIRED
#define ETHSM_NO_PBCFG_REQUIRED
#define ETHIF_NO_PBCFG_REQUIRED
#ifndef TCPIP_NO_CFGCLASSMIX_REQUIRED
#define TCPIP_NO_CFGCLASSMIX_REQUIRED
#endif

#include <TSAutosar.h>            /* EB specific global AUTOSAR macros */
#include <TSAtomic_Assign.h>      /* EB Atomic Assign macros */
#include <TSCompiler_Default.h>   /* TS_PREVENTEMPTYTRANSLATIONUNIT */
#include <ComStack_Types.h>       /* Com Stack Types */
#include <ComStack_Helpers.h>     /* Com Stack Helper macros */

#include <TcpIp_Api.h>            /* DET macros / module global type definitions */
#include <TcpIp_Socket_Api.h>     /* Type definitions of Socket SW-unit */
#include <TcpIp_IpAddrM_Api.h>    /* Type definitions of IpAddrM SW-unit*/


#include <TcpIp_CfgTypes.h>       /* TcpIp pre-compile-time dependent configuration types */
#include <TcpIp_Cfg.h>            /* TcpIp pre-compile-time configuration */
#include <TcpIp_Lcfg.h>           /* link time configuration */

#ifdef TCPIP_PRECOMPILE_TIME_PBCFG_ACCESS
#include <TcpIp_PBcfg.h>
#endif /* TCPIP_PRECOMPILE_TIME_PBCFG_ACCESS */

#if TCPIP_DEV_ERROR_DETECT == STD_ON
#include <Det.h>                  /* Det function declaration */
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */


#ifdef TS_RELOCATABLE_CFG_ENABLE
#error TS_RELOCATABLE_CFG_ENABLE is already defined
#endif /* TS_RELOCATABLE_CFG_ENABLE */
#define TS_RELOCATABLE_CFG_ENABLE TCPIP_RELOCATABLE_CFG_ENABLE
#include <TSPBConfig.h>           /* post-build config access macros */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

/*==================[macros]================================================*/

/* Configuration of ComStack_Helper.h macros for the TcpIp stack. */
#if (defined ComStack_Uint8PtrType)
#error ComStack_Uint8PtrType is already defined
#endif
#define ComStack_Uint8PtrType TcpIp_Uint8PtrType

#if (defined ComStack_Uint16PtrType)
#error ComStack_Uint16PtrType is already defined
#endif
#define ComStack_Uint16PtrType TcpIp_Uint16PtrType

#if (defined ComStack_Uint32PtrType)
#error ComStack_Uint32PtrType is already defined
#endif
#define ComStack_Uint32PtrType TcpIp_Uint32PtrType

#if (defined ComStack_ConstUint8PtrType)
#error ComStack_ConstUint8PtrType is already defined
#endif
#define ComStack_ConstUint8PtrType TcpIp_ConstUint8PtrType

#if (defined ComStack_ConstUint16PtrType)
#error ComStack_ConstUint16PtrType is already defined
#endif
#define ComStack_ConstUint16PtrType TcpIp_ConstUint16PtrType

#if (defined ComStack_ConstUint32PtrType)
#error ComStack_ConstUint32PtrType is already defined
#endif
#define ComStack_ConstUint32PtrType TcpIp_ConstUint32PtrType

/** \brief Instance Id for DET 2.0 (always 0, because only one
 **        TcpIp-Instance allowed)
*/
#define TCPIP_INSTANCE_ID 0u

/** \brief SID of service TcpIp_Init() */
#define TCPIP_SID_INIT    1u

/** \brief TcpIp_Close() service ID */
#define TCPIP_SID_CLOSE                    0x04U
/** \brief TcpIp_ChangeParameter() service ID */
#define TCPIP_SID_CHANGEPARAMETER          0x0fU

/** \brief Det error Id ASR 4.2.2: TCPIP_E_PARAM_POINTER ASR 4.1.3: TCPIP_E_NULL_PTR */
#define TCPIP_EB_E_PARAM_POINTER                  TCPIP_E_PARAM_POINTER

#define TCPIP_TIMER_OFFSET_1S_NDP_TABLE           ((TCPIP_MAINFUNCTION_PRESCALER_100MS*1U)%TCPIP_MAINFUNCTION_PRESCALER_1S)
#define TCPIP_TIMER_OFFSET_1S_DHCP                ((TCPIP_MAINFUNCTION_PRESCALER_100MS*3U)%TCPIP_MAINFUNCTION_PRESCALER_1S)
#define TCPIP_TIMER_OFFSET_1S_NDP_RPD_TABLE       ((TCPIP_MAINFUNCTION_PRESCALER_100MS*4U)%TCPIP_MAINFUNCTION_PRESCALER_1S)
#define TCPIP_TIMER_OFFSET_100MS_NDP_DAD_TABLE    (5U%TCPIP_MAINFUNCTION_PRESCALER_100MS)
#define TCPIP_TIMER_OFFSET_100MS_AUTOIP_TIMEOUT   (3U%TCPIP_MAINFUNCTION_PRESCALER_100MS)
#define TCPIP_TIMER_OFFSET_100MS_ARPPROBE         (1U%TCPIP_MAINFUNCTION_PRESCALER_100MS)
#define TCPIP_TIMER_OFFSET_100MS_ARP_TABLE        (2U%TCPIP_MAINFUNCTION_PRESCALER_100MS)
#define TCPIP_TIMER_OFFSET_100MS_ARPANNOUNCE      (6U%TCPIP_MAINFUNCTION_PRESCALER_100MS)
#define TCPIP_TIMER_OFFSET_100MS_DHCPV6_SOL_DELAY (5U%TCPIP_MAINFUNCTION_PRESCALER_100MS)

/** \brief Macros for checking if offload checksums are enabled */
#define TCPIP_GET_ENABLE_OFFLOAD_CHECKSUM_IPV4(ctrl,ctrlIdx)  (((TCPIP_CFG(ctrl,ctrlIdx)).enableOffloadChecksum)&0x01U)
#define TCPIP_GET_ENABLE_OFFLOAD_CHECKSUM_ICMP(ctrl,ctrlIdx)  ((((TCPIP_CFG(ctrl,ctrlIdx)).enableOffloadChecksum) >> 1U)&0x01U)
#define TCPIP_GET_ENABLE_OFFLOAD_CHECKSUM_TCP(ctrl,ctrlIdx)  ((((TCPIP_CFG(ctrl,ctrlIdx)).enableOffloadChecksum) >> 2U)&0x01U)
#define TCPIP_GET_ENABLE_OFFLOAD_CHECKSUM_UDP(ctrl,ctrlIdx)  ((((TCPIP_CFG(ctrl,ctrlIdx)).enableOffloadChecksum) >> 3U)&0x01U)

/** \brief Checks if checksum is calculated by hardware otherwise use value defined by protocol (UDP, TCP, ICMP) */
#define TCPIP_CALCULATE_CHECKSUM(calculateChecksum, checksumOffload) \
  ((checksumOffload) ? FALSE : (calculateChecksum))

/** \brief Increment the ID ticket counter for the respective IP stack. */
#define TCPIP_IP_GETNEXTID(isIpV6, id, counters)               \
  do { uint32 * const counter=                                 \
        &((counters)[TCPIP_IP_IF_V4_V6_BOTH(0u,0u,(isIpV6))]); \
       ++(*counter);                                           \
       (id)= *counter;                                         \
     } while(0)

/** \brief Macro to avoid using '##' twice in below macros */
/* Deviation MISRAC2012-1  */
#define TCPIP_ADD_TYPE_SUFFIX(NAME) NAME ## ConfigType

/** \brief Define TCPIP_CFG_TYPE macro for getting a reference to a config element, */
/* Deviation MISRAC2012-1 */
#define TCPIP_CFG(NAME, idx) \
  (TCPIP_CFG_TYPE(TCPIP_ADD_TYPE_SUFFIX(TcpIp_ ## NAME), NAME, idx))

#ifdef TCPIP_PRECOMPILE_TIME_PBCFG_ACCESS

#define TCPIP_CFG_TYPE(type, NAME, idx) \
  (TcpIp_ConfigLayout.NAME ## Cfg[(idx)])

#define TCPIP_PBCFG_PRECONDITION_ASSERT(idx,arr,api) \
  TCPIP_PRECONDITION_ASSERT((idx) < (sizeof(TcpIp_ConfigLayout.arr)/sizeof(TcpIp_ConfigLayout.arr[0])), (api))

#else

/* Deviation MISRAC2012-1 */
#define TCPIP_CFG_TYPE(type, NAME, idx) \
  (TS_UNCHECKEDGETCFG(TcpIp_PBcfgPtr, type, TCPIP, TcpIp_PBcfgPtr->NAME ## Ref)[(idx)])


#define TCPIP_PBCFG_PRECONDITION_ASSERT(idx,arr,api)

#endif /* TCPIP_PRECOMPILE_TIME_PBCFG_ACCESS */

#if (TCPIP_DEV_ERROR_DETECT == STD_ON)

/** \brief Macro for DET calls in TCPIP development-error-detection
 * sections (Det enabled) */
#define TCPIP_REPORT_ERROR(ApiId, ErrorId)                      \
  (void) Det_ReportError(TCPIP_MODULE_ID, TCPIP_INSTANCE_ID, (ApiId), (ErrorId))

#else

/** \brief Macro for DET calls in TCPIP development-error-detection
 * sections (Det disabled) */
#define TCPIP_REPORT_ERROR(ApiId, ErrorId)      \
  do {} while (0)

#endif /* (TCPIP_DEV_ERROR_DETECT == STD_ON) */

/** \brief The minimum alignment of Ethernet buffers. */
#ifndef  ETH_BUFFER_ALIGNMENT
# define ETH_BUFFER_ALIGNMENT 8
#endif

/** \brief The minimum alignment, so that unaligned accesses (access size > alignment) do not trap.
8 for architecture that can do all misaligned accesses.
32 for architectures that trap on 32 bit accesses that are not 32-aligned. */
#ifndef  TS_UNALIGNED_ACCESS
# define TS_UNALIGNED_ACCESS 32
#endif

/** \brief The maximum access width usable in header construction,
 *   according to Ethernet buffer alignment and architectural capabilities.
 *  Depends on \a ETH_BUFFER_ALIGNMENT which is the minimum alignment of Ethernet buffers and
 *  \a TS_UNALIGNED_ACCESS which provides the minimum alignment, so that unaligned accesses do not trap. */
#if (TS_UNALIGNED_ACCESS <= ETH_BUFFER_ALIGNMENT)
   /** \brief compile-time if to choose between 16 bit aligned and 8bit fallback */
#  define TCPIP_IF_ALIGN16(VARIANT16,VARIANT8) VARIANT16
   /** \brief compile-time if to choose between 32 bit aligned and 8bit fallback */
#  define TCPIP_IF_ALIGN32(VARIANT32,VARIANT8) VARIANT32
#elif (TS_UNALIGNED_ACCESS >= 32) && (ETH_BUFFER_ALIGNMENT >=16)  /* headers are always 16-aligned! */
   /** \brief compile-time if to choose between 16 bit aligned and 8bit fallback */
   /* Deviation MISRAC2012-2, MISRAC2012-3  */
#  define TCPIP_IF_ALIGN16(VARIANT16,VARIANT8) VARIANT16
   /** \brief compile-time if to choose between 32 bit aligned and 8bit fallback */
#  define TCPIP_IF_ALIGN32(VARIANT32,VARIANT8) VARIANT8
#else
   /** \brief compile-time if to choose between 16 bit aligned and 8bit fallback */
#  define TCPIP_IF_ALIGN16(VARIANT16,VARIANT8) VARIANT8
   /** \brief compile-time if to choose between 32 bit aligned and 8bit fallback */
   /* Deviation MISRAC2012-3  */
#  define TCPIP_IF_ALIGN32(VARIANT32,VARIANT8) VARIANT8
#endif

/** \brief Shortcut for calculating checksum using function TcpIp_CalcChecksum16Bit_withInitValue with
 * an initialization value of InitValue = 0.
 */
#define TcpIp_CalcChecksum16Bit(DataPtr,Length)\
    TcpIp_CalcChecksum16Bit_withInitValue(DataPtr,0u,Length)

/** \brief macro gets a 16 bit value from an IP header.
 * \par NOT SAFE FROM SIDE-EFFECTS!  Arguments are evaluated multiple times!
 * \param[in] headerPtr Pointer to TCPIP header area.
 * \param[in] constByteOffset Byte offset from\a headerPtr as a compile-time constant */
#define TCPIP_GET16(headerPtr, constByteOffset) \
  (TCPIP_IF_ALIGN16(\
   (COMSTACK_NTOH_UINT16(* ((TcpIp_ConstUint16PtrType const)(&((TcpIp_ConstUint8PtrType const)(headerPtr))[(constByteOffset)]) ) )), \
    COMSTACK_GET16(headerPtr, constByteOffset) ))

/** \brief macro gets an uint32 value from an IP header.
 * \par NOT SAFE FROM SIDE-EFFECTS!  Arguments are evaluated multiple times!
 * \param[in] headerPtr Pointer to TCPIP header area.
 * \param[in] constByteOffset Byte offset from\a headerPtr as a compile-time constant */
#define TCPIP_GET32(headerPtr, constByteOffset) \
  (TCPIP_IF_ALIGN32( \
    COMSTACK_NTOH_UINT32(* ((TcpIp_ConstUint32PtrType const)(&((TcpIp_ConstUint8PtrType const)(headerPtr))[(constByteOffset)]) ) ), \
    COMSTACK_GET32(headerPtr, constByteOffset) ))

/* safe function-type helper macros for header construction & parsing:
 * safety by: single parameter evaluation avoids side-effects,
 *            type-safe local variables with correct type */

/** \brief macro sets any IP header 16 bit field to \a value16
 * \par Safe from side effects: Arguments are evaluated only once.
 * \param[in] headerPtr Pointer to TCPIP header area.
 * \param[in] constByteOffset Byte offset from\a headerPtr as a compile-time constant
 * \param[in] value16 Value to be set. */
#define TCPIP_SET16(headerPtr, constByteOffset, value16)        \
  TCPIP_IF_ALIGN16(           \
    do {(*((TcpIp_Uint16PtrType const)(&(headerPtr)[(constByteOffset)])))= COMSTACK_HTON_UINT16((value16));} while(0), \
    COMSTACK_SET16(headerPtr, constByteOffset, value16)        )

/** \brief macro sets any IP header 32 bit field to \a value32
 * \par Safe from side effects: Arguments are evaluated only once.
 * \param[in] headerPtr Pointer to TCPIP header area.
 * \param[in] constByteOffset Byte offset from\a headerPtr as a compile-time constant
 * \param[in] value32 Value to be set. */
#define TCPIP_SET32(headerPtr, constByteOffset, value32)  \
  TCPIP_IF_ALIGN32(           \
    do {(*((TcpIp_Uint32PtrType const)(&(headerPtr)[(constByteOffset)])))= COMSTACK_HTON_UINT32((value32));} while(0), \
    COMSTACK_SET32(headerPtr, constByteOffset, value32) )

/** \brief macro copies 32 bit value (IP address) from \a headerPtr @ \a constByteOffset to \a resultPtr
 * \par Safe from side effects: Arguments are evaluated only once.
 * \param[in] headerPtr Pointer to TCPIP header area.
 * \param[in] constByteOffset Byte offset from \a headerPtr as a compile-time constant
 * \param[out] resultPtr Destination of the copy operation. */
#define TCPIP_GETCPY32(headerPtr, constByteOffset, resultPtr) \
  TCPIP_IF_ALIGN32(           \
    do {(*((TcpIp_Uint32PtrType)(resultPtr)))= *((TcpIp_ConstUint32PtrType const)&(headerPtr)[(constByteOffset)]);} while(0), \
    COMSTACK_GETCPY32(headerPtr, constByteOffset, resultPtr) )

/** \brief macro copies 32 bit value (IP address) from \a sourcePtr to \a resultPtr @ \a constByteOffset
 * \par Safe from side effects: Arguments are evaluated only once.
 * \param[in] headerPtr Pointer to TCPIP header area.
 * \param[in] constByteOffset Byte offset from \a headerPtr as a compile-time constant
 * \param[in] sourcePtr Source of the copy operation. */
#define TCPIP_SETCPY32(headerPtr, constByteOffset, sourcePtr) \
  TCPIP_IF_ALIGN32(           \
    do {(*((TcpIp_Uint32PtrType)(&(headerPtr)[(constByteOffset)])))= *((uint32 const* const)(sourcePtr));} while(0), \
    COMSTACK_SETCPY32(headerPtr, constByteOffset, sourcePtr) )


/** \brief round up integer \a i to be a multiple of \a n
 * \param i evaluated once
 * \param n evaluated several times, so preferably constants or sizeof(type) */
#define TCPIP_ROUNDUP(i,n) ((((uint32)(i))+((n)-1u))&~((uint32)(n)-1u))

/** \brief align pointer \a p to be aligned to a multiple of \a n
 * \param p evaluated once
 * \param n evaluated several times, so preferably constants or sizeof(type) */
#define TCPIP_ALIGN(p,n) ((void*)TCPIP_ROUNDUP((uint32)(p),n))

/** \brief check, if pointer \a p is aligned to a multiple of \a n
 *  Safe from side effects:
 * \param p evaluated once
 * \param n evaluated once
 */
#define TCPIP_ISALIGNED(p,n) ((((uint32)(p))&((uint32)(n)-1u))== 0u)

/** \brief length of an Ethernet address in bytes */
#define TCPIP_ETH_ADDR_LENGTH     6u

/** \brief macro checking the initialization status of the TcpIp module */
#define TCPIP_UNINIT \
  ((TcpIp_InitStatus != STD_ON) ||(TcpIp_PBcfgPtr == NULL_PTR))

/** \brief Counting consecutive set bits in uint32 variable. */
#define TCPIP_GET_NUMBER_OF_SET_BITS_32(mask,result)                                \
  do { uint32 number = (mask) - (((mask) >> 1U) & 0x55555555U);                     \
       number = (number & 0x33333333U) + ((number >> 2U) & 0x33333333U);            \
       (result) = (((number + (number >> 4U)) & 0x0F0F0F0FU) * 0x01010101U) >> 24U; \
     } while(0)


/** \brief check if link layer address is multicast */
#define TCPIP_MAC_IS_MULTICAST(addrPtr)                                              \
  ((addrPtr)[0U] ==  0x01U)

/** \brief check if link layer address is IPv4 multicast */
#define TCPIP_MAC_IS_IPV4_MULTICAST(addrPtr)                                         \
  ((addrPtr[0U] == 0x01U) && (addrPtr[1U] == 0x00U) && (addrPtr[2U] == 0x5EU) && ((addrPtr[3U] & 0x80U) == 0U))


/** \brief check if link layer address is IPv6 multicast */
#define TCPIP_MAC_IS_IPV6_MULTICAST(addrPtr)                                         \
  ((addrPtr[0U] == 0x33U) && (addrPtr[1U] == 0x33U))


/** \brief check if link layer address is Broadcast */
#define TCPIP_MAC_IS_BROADCAST(addrPtr)                                              \
  ((addrPtr[0U] == 0xFFU) && (addrPtr[1U] == 0xFFU) && (addrPtr[2U] == 0xFFU) &&     \
   (addrPtr[3U] == 0xFFU) && (addrPtr[4U] == 0xFFU) && (addrPtr[5U] == 0xFFU))

#define TCPIP_GET_RANDOM_ENABLED                  \
  (((TCPIP_IPV4_LINK_LOCAL_ENABLED == STD_ON) ||  \
  (TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)    ||  \
  (TCPIP_DHCPV4_CLIENT_ENABLED == STD_ON)     ||  \
  (TCPIP_DHCPV6_CLIENT_ENABLED == STD_ON)     ||  \
  (TCPIP_NDP_RPD_ENABLED == STD_ON))? STD_ON : STD_OFF)

#define TCPIP_GET_RANDOM_RANGE_ENABLED                          \
  (((TCPIP_ARP_DAD_ENABLED == STD_ON)                        || \
  ((TCPIP_NDP_DAD_CONFIG_ENABLED == STD_ON)               &&    \
   (TCPIP_NDP_DAD_CONFIG_DELAY_ENABLED == STD_ON)         &&    \
   (TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED == STD_OFF)) || \
  (TCPIP_DHCPV6_CLIENT_ENABLED == STD_ON)                    || \
  (TCPIP_NDP_RPD_ENABLED == STD_ON))? STD_ON : STD_OFF)
/*==================[defensive programming macros]=========================*/




/*------------------------[Defensive programming]----------------------------*/

#if (defined TCPIP_PRECONDITION_ASSERT)
#error TCPIP_PRECONDITION_ASSERT is already defined
#endif

#if (defined TCPIP_PRECONDITION_ASSERT_NO_EVAL)
#error TCPIP_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (TCPIP_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define TCPIP_PRECONDITION_ASSERT(Condition, ApiId) \
  DET_PRECONDITION_ASSERT((Condition), TCPIP_MODULE_ID, TCPIP_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define TCPIP_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL((Condition), TCPIP_MODULE_ID, TCPIP_INSTANCE_ID, (ApiId))
#else
#define TCPIP_PRECONDITION_ASSERT(Condition, ApiId)
#define TCPIP_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined TCPIP_POSTCONDITION_ASSERT)
#error TCPIP_POSTCONDITION_ASSERT is already defined
#endif

#if (defined TCPIP_POSTCONDITION_ASSERT_NO_EVAL)
#error TCPIP_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (TCPIP_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define TCPIP_POSTCONDITION_ASSERT(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT((Condition), TCPIP_MODULE_ID, TCPIP_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define TCPIP_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL((Condition), TCPIP_MODULE_ID, TCPIP_INSTANCE_ID, (ApiId))
#else
#define TCPIP_POSTCONDITION_ASSERT(Condition, ApiId)
#define TCPIP_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined TCPIP_INVARIANT_ASSERT)
#error TCPIP_INVARIANT_ASSERT is already defined
#endif

#if (defined TCPIP_INVARIANT_ASSERT_NO_EVAL)
#error TCPIP_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (TCPIP_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define TCPIP_INVARIANT_ASSERT(Condition, ApiId) \
  DET_INVARIANT_ASSERT((Condition), TCPIP_MODULE_ID, TCPIP_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define TCPIP_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL((Condition), TCPIP_MODULE_ID, TCPIP_INSTANCE_ID, (ApiId))
#else
#define TCPIP_INVARIANT_ASSERT(Condition, ApiId)
#define TCPIP_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined TCPIP_STATIC_ASSERT)
# error TCPIP_STATIC_ASSERT is already defined
#endif
#if (TCPIP_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated */
# define TCPIP_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define TCPIP_STATIC_ASSERT(expr)
#endif

#if (defined TCPIP_UNREACHABLE_CODE_ASSERT)
#error TCPIP_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (TCPIP_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 **

 ** \param[in] ApiId Service ID of the API function */
#define TCPIP_UNREACHABLE_CODE_ASSERT(ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(TCPIP_MODULE_ID, TCPIP_INSTANCE_ID, (ApiId))
#else
#define TCPIP_UNREACHABLE_CODE_ASSERT(ApiId)
#endif

#if (defined TCPIP_INTERNAL_API_ID)
#error TCPIP_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define TCPIP_INTERNAL_API_ID DET_INTERNAL_API_ID



#undef TS_RELOCATABLE_CFG_ENABLE

/*==================[type definitions]=========================*/

/** \brief The data pointer used for all TcpIp 'octet' data streams */
typedef P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_Uint8PtrType;

/** \brief The data pointer used for all TcpIp 'void' data items */
typedef P2VAR(void, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_VoidPtrType;

/** \brief The data pointer used for all const 'octet' TcpIp data streams */
typedef P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_ConstUint8PtrType;

/** \brief The data pointer used for all TcpIp 16 bit data items */
typedef P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_Uint16PtrType;

/** \brief The data pointer used for all const 16 bit TcpIp data items */
typedef P2CONST(uint16, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_ConstUint16PtrType;

/** \brief The data pointer used for all 32 bit TcpIp data items */
typedef P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_Uint32PtrType;

/** \brief The data pointer used for all const 32 bit TcpIp data items */
typedef P2CONST(uint32, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_ConstUint32PtrType;


/*==================[external data declarations]=========================*/

#define TCPIP_START_SEC_VAR_INIT_8
#include <TcpIp_MemMap.h>

/** \brief TcpIp init state */
extern VAR(uint8, TCPIP_VAR) TcpIp_InitStatus;

#define TCPIP_STOP_SEC_VAR_INIT_8
#include <TcpIp_MemMap.h>

#define TCPIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

/** \brief Post-Build pointer */
extern P2CONST(TcpIp_ConfigType, TCPIP_VAR_CLEARED, TCPIP_APPL_CONST) TcpIp_PBcfgPtr;

#define TCPIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

/*==================[external function declarations]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

#if (TCPIP_GET_RANDOM_ENABLED == STD_ON)
/** \brief Returns a random value
 *
 * This function returns a uint32 random number.
 *
 * \return Returns a random number
 */
TS_MOD_PRIV_DECL FUNC(uint32, TCPIP_CODE) TcpIp_GetRandomValue (void);
#endif /* (TCPIP_GET_RANDOM_ENABLED == STD_ON)*/

#if (TCPIP_GET_RANDOM_RANGE_ENABLED == STD_ON)
/** \brief Returns a random value in a specific range
 *
 * \param[in]  range  Specifies the maximum number that can be returned
 *
 * \return Returns a random number in the specified range
 */
TS_MOD_PRIV_DECL FUNC(uint16, TCPIP_CODE) TcpIp_GetRandomRange
(
  uint16 range
);
#endif /* (TCPIP_GET_RANDOM_RANGE_ENABLED == STD_ON) */

/** \brief Calculates 16-bit sum of a memory area
 *
 *  Calculates the 16-bit sum of a memory area given with dataPtr and
 *  length in units of bytes. initValue contains the starting value for calculation.
 *
 * \param[in] dataPtr     Points to a memory area to calculate the sum of
 * \param[in] initValue   Starting value of sum calculation
 * \param[in] length      Memory length to calculate checksum in units of bytes
 *
 * \return 16-bit sum
 */
TS_MOD_PRIV_DECL FUNC(uint16, TCPIP_CODE) TcpIp_CalcSum16Bit
(
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
    uint16 initValue,
    uint16 length
);

/** \brief Calculates the checksum of a memory area
 *
 *  Calculates the one's complement sum checksum of a memory area given with dataPtr and
 *  length in units of bytes. initValue contains the starting value for calculation.
 *
 * \param[in] dataPtr     Points to a memory area to calculate the one's complement sum of
 * \param[in] initValue   Starting value of checksum calculation
 * \param[in] length      Memory length to calculate checksum in units of bytes
 *
 * \return checksum
 */
TS_MOD_PRIV_DECL FUNC(uint16, TCPIP_CODE) TcpIp_CalcChecksum16Bit_withInitValue
(
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
    uint16 initValue,
    uint16 length
);

#if (TCPIP_GET_RANDOM_ENABLED == STD_ON)

/** \brief Initialize random value generator
 *
 * This function initialize the random number generator with the link layer address
 * of controller 0.
 *
 * \param[in] ctrlIdx  controller index
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_InitRandomGenerator
(
  uint8 ctrlIdx
);

#endif /* (TCPIP_GET_RANDOM_ENABLED == STD_ON) */

/** \brief Copy ethernet addresses
 *
 * \param[in] addrDestPtr  Address of ethernet address destination
 * \param[in] addrSrcPtr    Address of ethernet address source
 *
 */
TS_MOD_PRIV_DECL FUNC( void, TCPIP_CODE ) TcpIp_EthAddrCpy
(
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) addrDestPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) addrSrcPtr
);

/** \brief Compare 2 ethernet addresses
 *
 * \param[in] addr1Ptr    Address of ethernet address 1
 * \param[in] addr2Ptr    Address of ethernet address 2
 *
 * \return Result of comparison
 * \retval FALSE ethernet addresses are equal
 * \retval TRUE ethernet addresses are different
 *
 */
TS_MOD_PRIV_DECL FUNC( boolean, TCPIP_CODE ) TcpIp_EthAddrCmp
(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) addr1Ptr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) addr2Ptr
);


#if (TCPIP_IPV6_ENABLED == STD_ON)
/** \brief Check if Link layer address is valid
 *
 * This Function checks if link layer address is not a multicast/broadcast address
 *
 * \param[in] LinkAddr  link layer address
 *
 * \return Result of operation
 * \retval E_OK:         physical address is valid
 * \retval E_NOT_OK:     physical address is invalid (multicast/broadcast)
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_CheckMacNotMulticast
(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) LinkAddr
);
#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#endif /* ifndef TCPIP_INT_H */
/*==================[end of file]===========================================*/
