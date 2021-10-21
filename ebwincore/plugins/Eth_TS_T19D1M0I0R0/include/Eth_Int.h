#ifndef ETH_INT_H
#define ETH_INT_H

/**
 * \file
 *
 * \brief AUTOSAR Eth
 *
 * This file contains the implementation of the AUTOSAR
 * module Eth.
 *
 * \version 0.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2011 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 20.10 (required)
 *  The # and ## preprocessor operators should not be used.
 *
 *  Reason:
 *  Usage of concatenation operator improves readability of code.
 *
 * MISRAC2012-2) Deviated Rule: 20.7 (required)
 *   Expressions resulting from the expansion of macro parameters shall be enclosed in parentheses.
 *
 *   Reason:
 *   The AUTOSAR compiler abstraction requires these definitions in this way. Declarations,
 *   function arguments and function return value cannot be enclosed in parentheses due to C
 *   syntax.
 */

/*==================[inclusions]============================================*/

#include <TSAutosar.h>          /* EB specific standard types */
#include <Eth_Cfg.h>            /* Generated configuration */
#include <Eth_Types.h>          /* Module public types */
#include <Eth_TypesInt.h>       /* Eth configuration types */

#if (ETH_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>               /* Det API */
#include <Eth_Api.h>               /* ServiceIds and error code for Det API */
#include <Eth_Version.h>
#endif

#ifdef ETH_PRECOMPILE_TIME_PBCFG_ACCESS
#include <Eth_PBcfg.h>      /* PBcfg structure, to allow pre-compile time like  access */
#endif /* ETH_PRECOMPILE_TIME_PBCFG_ACCESS */

#ifdef TS_RELOCATABLE_CFG_ENABLE
#error TS_RELOCATABLE_CFG_ENABLE is already defined
#endif
#define TS_RELOCATABLE_CFG_ENABLE ETH_RELOCATABLE_CFG_ENABLE

#include <TSPBConfig.h>           /* relocatable post-build macros */
#include <TSPBConfig_Access.h>

#ifndef ETH_DRIV_DSB
#ifndef TS_WIN32X86
#ifndef EB_STATIC_CHECK
#include <Atomics.h>            /* Atomics API */
#endif
#endif
#endif

/*==================[macros]================================================*/

/* CHECK: RULE 501 OFF */
/** \brief Data Synchronization Barrier
 *
 * This command ensures that all previous commands are finished and written into the memory.
 */
#ifndef ETH_DRIV_DSB

#ifdef TS_WIN32X86
#if (TS_ARCH_DERIVATE == TS_WIN32X86)
/* In case of Windows no DSB is needed. */
#define ETH_DRIV_DSB()
#endif
#else /* for all other architectures/derivatives */
#ifdef EB_STATIC_CHECK
/* In case of code analyzing tool, skip assembler instructions. */
#define ETH_DRIV_DSB()
#else /* ifndef EB_STATIC_CHECK */
/* else use compiler-specific assembler instructions.
   Since ACG-8.5 Atomic module consist of this function. Compiler-specific ETH_DRIV_DSB macro needs
   to be defined if no Atomics module is available. More information please see the release notes!
 */
#define ETH_DRIV_DSB() Atomics_ThreadFence()

#endif /* ifdef EB_STATIC_CHECK */
#endif /* #ifdef TS_WIN32X86 */
#endif /* ifndef ETH_DRIV_DSB */

/* CHECK: RULE 501 ON */

/** \brief Byte mirror macro to convert uint32 values between little and big endian format.
 *  Non-overloadable. For compile-time evaluation.
 */
#define ETH_COMSTACK_BYTE_MIRROR32(value) ((uint32)\
  ( ((uint32)((uint32)((((uint32)(value)) & (uint32)0x000000FFUL) << 24U))) | \
    ((uint32)((uint32)((((uint32)(value)) & (uint32)0x0000FF00UL) <<  8U))) | \
    ((uint32)(((uint32)(((uint32)(value)) >> 8U )) & ((uint32)0x0000FF00UL))) | \
    ((uint32)(((uint32)(((uint32)(value)) >> 24U)) & ((uint32)0x000000FFUL))) ))

/** \brief uint32 host to network order conversion macro */
#define ETH_COMSTACK_HTON_UINT32(value) TS_IF_BE_LE((value), ETH_COMSTACK_BYTE_MIRROR32(value))

/** \brief Convert uint32 byte order from host to network order */
#define ETH_HTON_UINT32(RegVal)   ETH_COMSTACK_HTON_UINT32(RegVal)

/** \brief Convert of boolean into integer */
#define ETH_CONV_BOOL_INT( VarBool ) ((VarBool) ? 1U : 0U )

#define ETH_TX_CONFIRMATION_NOT_COMPLETE          0U    /* Negative TX Confirmation */
#define ETH_TX_CONFIRMATION_COMPLETE              1U    /* Positive TX Confirmation */
/* TX Confirmation indicates no buffers left to check */
#define ETH_TX_CONFIRMATION_FIN                 255U

#define ETH_32BIT_MAX   0xFFFFFFFFU
#define ETH_16BIT_MAX       0xFFFFU

#if (ETH_GLOBAL_TIME_SUPPORT == STD_ON)
/** \brief Maximum value of the counter for time stamp API functions */
#define ETH_NANOSEC_MAX (uint32) 999999999U
#endif /* (ETH_GLOBAL_TIME_SUPPORT == STD_ON) */

/** \brief Macro to report a Det error provided only with the ApiId and ErrorId */
#if (ETH_DEV_ERROR_DETECT == STD_ON)
#define ETH_DET_REPORT_ERROR(ApiId, ErrorId)    \
    (void)Det_ReportError(ETH_MODULE_ID, ETH_INSTANCE_ID, (ApiId), (ErrorId))
#else /* (ETH_DEV_ERROR_DETECT == STD_ON) */
#define ETH_DET_REPORT_ERROR(ApiId, ErrorId)
#endif /* (ETH_DEV_ERROR_DETECT == STD_ON) */

/** \brief Tests if the parameter \p x is Tx buffer aligned.
 * Remark: Macro USIZE_C adds suffix UL thus it is added here.
 */
#define ETH_BUFTXHW_ALIGNED(x) (((usize)(x) & USIZE_C(ETH_BUFTXHW_ALIGN - 1)) == USIZE_C(0))

/** \brief Tests if the parameter \p x is Rx buffer aligned.
 * Remark: Macro USIZE_C adds suffix UL thus it is added here.
 */
#define ETH_BUFRXHW_ALIGNED(x) (((usize)(x) & USIZE_C(ETH_BUFRXHW_ALIGN - 1)) == USIZE_C(0))

/** \brief Tests if the parameter \p x is aligned (max of all alignments). All MemSec must be
 * aligned accordingly. Remark: Macro USIZE_C adds suffic UL thus it is added here.
 */
#define ETH_MAX_ALIGNED(x) (((usize)(x) & USIZE_C(ETH_MAX_ALIGN - 1)) == USIZE_C(0))

/** \brief Macro for atomic assign of a pointer. There can be always a direct asignment. It is
 * atomic on each architecture).
 */
#ifndef ETH_AtomicAssignPointer
#define ETH_AtomicAssignPointer(to, from)  ((to) = (from))
#endif

/** \brief Access Post-Build ROM config from the Eth driver
 **
 ** This macro accesses an element (struct) in the Post-build config
 ** by using the given references.
 **
 ** \param[in] type Type of the element (member) to access
 ** \param[in] name Common name of the struct and reference (without prefix p resp. a)
 ** \param[in] element Element (member) in the config to access
 */
#ifdef ETH_PRECOMPILE_TIME_PBCFG_ACCESS

/* Deviation MISRAC2012-1 */
#define ETH_PBCFG_ACCESS(type, name, element) \
    (Eth_ConfigLayout.a ## name[element])

#else /* ETH_PRECOMPILE_TIME_PBCFG_ACCESS */

/* Deviation MISRAC2012-1 */
#define ETH_PBCFG_ACCESS(type, name, element) \
    (TS_UNCHECKEDGETCFG(Eth_RootPtr, type, ETH, (Eth_RootPtr->p ## name))[(element)])

#endif /* ETH_PRECOMPILE_TIME_PBCFG_ACCESS */

/*--------------------------------------------------------------------------*/
/** \brief Macro to access the number of configured Ctrl */
#define ETH_CFG_GET_NR_OF_CTRLCFG()  (Eth_RootPtr->nNumCtrls)

/*--------------------------------------------------------------------------*/
/** \brief Macro to access the General configuration data */
#define ETH_CFG_GET_PTR_TO_GEN() \
(&ETH_CFG_GET_GEN())

#define ETH_CFG_GET_GEN() \
(ETH_PBCFG_ACCESS(Eth_GeneralCfgType, General, (0U) ) )

/*--------------------------------------------------------------------------*/
/** \brief Macro to access the Ctrl configuration data */
#define ETH_CFG_GET_PTR_TO_CTRL(CtrlIdx) \
(&ETH_CFG_GET_CTRL(CtrlIdx))

#define ETH_CFG_GET_CTRL(CtrlIdx) \
(ETH_PBCFG_ACCESS(Eth_CtrlType, Ctrls, (CtrlIdx) ) )


#define ETH_CFG_GET_PTR_TO_CTRL_VE(CtrlIdx)   ETH_CFG_GET_PTR_TO_CTRL(CtrlIdx)
#define ETH_CFG_GET_CTRL_VE(CtrlIdx)          ETH_CFG_GET_CTRL(CtrlIdx)

#define ETH_CFG_GET_PTR_TO_CTRL_VED(CtrlIdx)   ETH_CFG_GET_PTR_TO_CTRL(CtrlIdx)
#define ETH_CFG_GET_CTRL_VED(CtrlIdx)          ETH_CFG_GET_CTRL(CtrlIdx)

/*--------------------------------------------------------------------------*/
/** \brief Macro to access the number of configured StrmTreeCfg */
#define ETH_CFG_GET_NR_OF_STRMTREECFG()  (Eth_RootPtr->nNumStrmTree)

/** \brief Macro to access the StrmTreeCfg configuration data */
#define ETH_CFG_GET_PTR_TO_STRMTREECFG(StrmTreeIdx) \
(&ETH_CFG_GET_STRMTREECFG(StrmTreeIdx))

#define ETH_CFG_GET_STRMTREECFG(StrmTreeIdx) \
(ETH_PBCFG_ACCESS(Eth_StrmTreeCfgType, StrmTrees, (StrmTreeIdx) ) )
/*--------------------------------------------------------------------------*/
/** \brief Macro to access the number of configured PrioCtrlStrmMapCfg */
#define ETH_CFG_GET_NR_OF_PRIOCTRLSTRMMAPCFG()  (Eth_RootPtr->nNumPrioCtrlStrmMaps)

/** \brief Macro to access the PrioCtrlStrmMapCfg configuration data */
#define ETH_CFG_GET_PTR_TO_PRIOCTRLSTRMMAPCFG(pCtrl, Prio) \
(&ETH_CFG_GET_PRIOCTRLSTRMMAPCFG(pCtrl, Prio))

#define ETH_CFG_GET_PRIOCTRLSTRMMAPCFG(pCtrl, Prio) \
  (ETH_PBCFG_ACCESS(Eth_PrioCtrlStrmMapCfgType, PrioCtrlStrmMaps, pCtrl->PrioIdxOffset + Prio))
/*--------------------------------------------------------------------------*/
/** \brief Macro to access the number of configured QueRxCfg */
#define ETH_CFG_GET_NR_OF_QUERXCFG()  (Eth_RootPtr->nNumQueRxs)

/** \brief Macro to access the QueRxCfg configuration data */
#define ETH_CFG_GET_PTR_TO_QUERXCFG(QueRxIdx) \
(&ETH_CFG_GET_QUERXCFG(QueRxIdx))

#define ETH_CFG_GET_QUERXCFG(QueRxIdx) \
(ETH_PBCFG_ACCESS(Eth_QueRxCfgType, QueRxs, (QueRxIdx) ) )


#define ETH_CFG_GET_PTR_TO_QUERXCFG_VE(QueRxIdx) ETH_CFG_GET_PTR_TO_QUERXCFG(QueRxIdx)
#define ETH_CFG_GET_QUERXCFG_VE(QueRxIdx) ETH_CFG_GET_QUERXCFG(QueRxIdx)

#define ETH_CFG_GET_PTR_TO_QUERXCFG_VED(QueRxIdx) ETH_CFG_GET_PTR_TO_QUERXCFG(QueRxIdx)
#define ETH_CFG_GET_QUERXCFG_VED(QueRxIdx) ETH_CFG_GET_QUERXCFG(QueRxIdx)

/*--------------------------------------------------------------------------*/
/** \brief Macro to access the number of configured QueTxBufCfg */
#define ETH_CFG_GET_NR_OF_QUETXBUFCFG()  (Eth_RootPtr->nNumQueTxBufs)

/** \brief Macro to access the QueTxBufCfg configuration data */
#define ETH_CFG_GET_PTR_TO_QUETXBUFCFG(QueTxBufIdx) \
(&ETH_CFG_GET_QUETXBUFCFG(QueTxBufIdx))

#define ETH_CFG_GET_QUETXBUFCFG(QueTxBufIdx) \
(ETH_PBCFG_ACCESS(Eth_QueTxBufCfgType, QueTxBufs, (QueTxBufIdx) ))


#define ETH_CFG_GET_PTR_TO_QUETXBUFCFG_VE(QueTxBufIdx) ETH_CFG_GET_PTR_TO_QUETXBUFCFG(QueTxBufIdx)
#define ETH_CFG_GET_QUETXBUFCFG_VE(QueTxBufIdx) ETH_CFG_GET_QUETXBUFCFG(QueTxBufIdx)

#define ETH_CFG_GET_PTR_TO_QUETXBUFCFG_VED(CtrlIdx, QueTxBufIdx) ETH_CFG_GET_PTR_TO_QUETXBUFCFG(QueTxBufIdx)
#define ETH_CFG_GET_QUETXBUFCFG_VED(QueTxBufIdx) ETH_CFG_GET_QUETXBUFCFG(QueTxBufIdx)

/*--------------------------------------------------------------------------*/
/** \brief Macro to access the number of configured QueTxBdCfg */
#define ETH_CFG_GET_NR_OF_QUETXBDCFG()  (Eth_RootPtr->nNumQueTxBds)

/** \brief Macro to access the QueTxBdCfg configuration data */
#define ETH_CFG_GET_PTR_TO_QUETXBDCFG(QueTxBdIdx) \
(&ETH_CFG_GET_QUETXBDCFG(QueTxBdIdx))

#define ETH_CFG_GET_QUETXBDCFG(QueTxBdIdx) \
(ETH_PBCFG_ACCESS(Eth_QueTxBdCfgType, QueTxBds, (QueTxBdIdx) ) )


#define ETH_CFG_GET_PTR_TO_QUETXBDCFG_VE(QueTxBdIdx) ETH_CFG_GET_PTR_TO_QUETXBDCFG(QueTxBdIdx)
#define ETH_CFG_GET_QUETXBDCFG_VE(QueTxBdIdx) ETH_CFG_GET_QUETXBDCFG(QueTxBdIdx)

/*--------------------------------------------------------------------------*/


/** \brief Macro to get count of elements in array */
#define ETH_ARRAY_ELEM_COUNT( array ) (sizeof(array)/sizeof(array[0U]))

/** \brief Defensive programming */




/*------------------------[Defensive programming]----------------------------*/

#if (defined ETH_PRECONDITION_ASSERT)
#error ETH_PRECONDITION_ASSERT is already defined
#endif

#if (defined ETH_PRECONDITION_ASSERT_NO_EVAL)
#error ETH_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (ETH_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define ETH_PRECONDITION_ASSERT(Condition, ApiId) \
  DET_PRECONDITION_ASSERT((Condition), ETH_MODULE_ID, ETH_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define ETH_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL((Condition), ETH_MODULE_ID, ETH_INSTANCE_ID, (ApiId))
#else
#define ETH_PRECONDITION_ASSERT(Condition, ApiId)
#define ETH_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined ETH_POSTCONDITION_ASSERT)
#error ETH_POSTCONDITION_ASSERT is already defined
#endif

#if (defined ETH_POSTCONDITION_ASSERT_NO_EVAL)
#error ETH_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (ETH_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define ETH_POSTCONDITION_ASSERT(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT((Condition), ETH_MODULE_ID, ETH_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define ETH_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL((Condition), ETH_MODULE_ID, ETH_INSTANCE_ID, (ApiId))
#else
#define ETH_POSTCONDITION_ASSERT(Condition, ApiId)
#define ETH_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined ETH_INVARIANT_ASSERT)
#error ETH_INVARIANT_ASSERT is already defined
#endif

#if (defined ETH_INVARIANT_ASSERT_NO_EVAL)
#error ETH_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (ETH_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define ETH_INVARIANT_ASSERT(Condition, ApiId) \
  DET_INVARIANT_ASSERT((Condition), ETH_MODULE_ID, ETH_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define ETH_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL((Condition), ETH_MODULE_ID, ETH_INSTANCE_ID, (ApiId))
#else
#define ETH_INVARIANT_ASSERT(Condition, ApiId)
#define ETH_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined ETH_STATIC_ASSERT)
# error ETH_STATIC_ASSERT is already defined
#endif
#if (ETH_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated */
# define ETH_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define ETH_STATIC_ASSERT(expr)
#endif

#if (defined ETH_UNREACHABLE_CODE_ASSERT)
#error ETH_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (ETH_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 **

 ** \param[in] ApiId Service ID of the API function */
#define ETH_UNREACHABLE_CODE_ASSERT(ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(ETH_MODULE_ID, ETH_INSTANCE_ID, (ApiId))
#else
#define ETH_UNREACHABLE_CODE_ASSERT(ApiId)
#endif

#if (defined ETH_INTERNAL_API_ID)
#error ETH_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define ETH_INTERNAL_API_ID DET_INTERNAL_API_ID




#ifndef ETH_ASSERT_BOUND_ARRY_ENABLED
/** \brief Enable assert and error counter for array boundary check. */
#define ETH_ASSERT_BOUND_ARRY_ENABLED STD_OFF
#endif /* ifndef ETH_ASSERT_BOUND_ARRY_ENABLED */

/** \brief Number of arrays where bound assert is done (see Eth_AssertBoundArryType). */
#define ETH_ASSERT_BOUND_ARRY_NO     ETH_ASSERT_BOUND_ARRY_LASTELEMENT

#if (ETH_ASSERT_BOUND_ARRY_ENABLED == STD_ON)

/** \brief Assert to check the array index. */
#define ETH_ASSERT_BOUND_ARRY( ArryIdx, Idx, ElemNo) \
  do \
  { \
    if( Idx >= (ElemNo)) \
    { \
      ETH_UNREACHABLE_CODE_ASSERT(ETH_INTERNAL_SVCID); \
      Eth_AssertBound[(ArryIdx)]++; \
    } \
  } while( 0U )

#else /* (ETH_ASSERT_BOUND_ARRY_ENABLED == STD_ON) */

/** \brief Assert to check the array index is empty. */
#define ETH_ASSERT_BOUND_ARRY( ArryIdx, Idx, ElemNo) \

#endif /* (ETH_ASSERT_BOUND_ARRY_ENABLED == STD_ON) */



#ifndef ETH_ASSERT_CRITSEC_ENABLED
/** \brief Enable assert of critical section. */
#define ETH_ASSERT_CRITSEC_ENABLED STD_OFF
#endif /* ifndef ETH_ASSERT_CRITSEC_ENABLED */

/** \brief Number of critical sections (see Eth_AssertCritSecType). */
#define ETH_ASSERT_CRITSEC_NO     ETH_CRITSEC_LASTELEMENT

/** \brief Number of critical section counters (see Eth_AssertCritSecCntTypeType). */
#define ETH_ASSERT_CRITSECCNT_NO     2U

#if (ETH_ASSERT_CRITSEC_ENABLED == STD_ON)

/** \brief Assert to check the entry to a critical section. */
#define ETH_ASSERT_CRITSEC_BEGIN( CritSec) \
  do { \
    if( Eth_AssertCritSec[CritSec][ETH_ASSERT_CRITSECCNT_CNT] != 0U) \
    { \
      /* Violation of critical section! */ \
      ETH_UNREACHABLE_CODE_ASSERT(ETH_INTERNAL_SVCID); \
      Eth_AssertCritSec[CritSec][ETH_ASSERT_CRITSECCNT_ERR]++; \
    } \
    Eth_AssertCritSec[CritSec][ETH_ASSERT_CRITSECCNT_CNT]++; \
  } while(0U)

/** \brief Assert to check the exit of a critical section. */
#define ETH_ASSERT_CRITSEC_END( CritSec) \
  do { \
    const uint32 Cnt = Eth_AssertCritSec[CritSec][ETH_ASSERT_CRITSECCNT_CNT]; \
    if( Cnt == 0U) \
    { \
      /* Violation of critical section! */ \
      ETH_UNREACHABLE_CODE_ASSERT(ETH_INTERNAL_SVCID); \
      Eth_AssertCritSec[CritSec][ETH_ASSERT_CRITSECCNT_ERR]++; \
    } \
    else \
    { \
      if( Cnt > 1U) \
      { \
        /* Violation of critical section! */ \
        ETH_UNREACHABLE_CODE_ASSERT(ETH_INTERNAL_SVCID); \
        Eth_AssertCritSec[CritSec][ETH_ASSERT_CRITSECCNT_ERR]++; \
      } \
      Eth_AssertCritSec[CritSec][ETH_ASSERT_CRITSECCNT_CNT] = Cnt - 1U; \
    } \
  } while(0U)

#else /* (ETH_ASSERT_CRITSEC_ENABLED == STD_ON) */

#define ETH_ASSERT_CRITSEC_BEGIN( CritSec)
#define ETH_ASSERT_CRITSEC_END( CritSec)

#endif /* (ETH_ASSERT_CRITSEC_ENABLED == STD_ON) */



#ifndef ETH_ERROR_CNT_SUPPORT
/** \brief Enable the error counters. */
#define ETH_ERROR_CNT_SUPPORT STD_OFF
#endif /* ifndef ETH_ERROR_CNT_SUPPORT */

/** \brief Number of error counters. */
#define ETH_ERROR_CNT_NO   ETH_ERROR_CNT_LASTELEMENT

#if (ETH_ERROR_CNT_SUPPORT == STD_ON)

/** \brief Increment of provided error counter. */
#define ETH_ERROR_CNT_INC(Cnt)   (Eth_ErrorCntLst[Cnt]++)

#else /* (ETH_ERROR_CNT_SUPPORT == STD_ON) */

/** \brief Dummy macro for increment of error counter. */
#define ETH_ERROR_CNT_INC(Cnt)

#endif /* (ETH_ERROR_CNT_SUPPORT == STD_ON) */



/*==================[macro checks]==========================================*/

/*==================[type definitions]======================================*/

/** \brief Defines the type of the Eth controller state. */
#define ETH_CTRL_STATE_UNINIT                  0U
#define ETH_CTRL_STATE_MII_INIT                1U
#define ETH_CTRL_STATE_INIT                    2U
typedef uint8 Eth_CtrlStateType;

/** \brief Array types for bound assert (number of elements defined in ETH_ASSERT_BOUND_ARRY_NO). */
/* File Eth_BdrTx.c */
#define ETH_ASSERT_BOUND_ARRY_BDRTXBUFINFOTX        0U
/* Last element is only dummy (used for ETH_ASSERT_BOUND_ARRY_NO). Maximum of 32 is possible. */
#define ETH_ASSERT_BOUND_ARRY_LASTELEMENT          1U
typedef uint8 Eth_AssertBoundArryType;

/** \brief Critical section assert (number of elements defined in ETH_ASSERT_CRITSEC_NO). */
#define ETH_ASSERT_CRITSEC_0        0U
#define ETH_ASSERT_CRITSEC_1        1U
#define ETH_ASSERT_CRITSEC_2        2U
#define ETH_ASSERT_CRITSEC_3        3U
/* Last element is only dummy (used for ETH_ASSERT_CRITSEC_NO). */
#define ETH_CRITSEC_LASTELEMENT     4U
typedef uint8 Eth_AssertCritSecType;

/** \brief Critical section counter type (ETH_ASSERT_CRITSECCNT_NO defines number of types). */
#define ETH_ASSERT_CRITSECCNT_CNT        0U
#define ETH_ASSERT_CRITSECCNT_ERR        1U
typedef uint8 Eth_AssertCritSecCntTypeType;

/** \brief List of error counters (number of elements defined in ETH_ERROR_CNT_NO). */
/* File Eth_BdrTx.c */
#define ETH_ERROR_CNT_BDRTXPROVIDETXBUFFEROVFL   0U
#define ETH_ERROR_CNT_BDRTXPROVIDETXBUFFERBUSY   1U
#define ETH_ERROR_CNT_BDRTXTRANSMITSTATE         2U
/* Last element is only dummy (used for ETH_ERROR_CNT_NO). Maximum of 32 is possible. */
#define ETH_ERROR_CNT_LASTELEMENT                3U
typedef uint8 Eth_ErrorCntType;

/*==================[external function declarations]========================*/
#define ETH_START_SEC_CODE
#include <Eth_MemMap.h>

extern FUNC(void, ETH_CODE) Eth_Arch_Init
(
  P2CONST(Eth_ConfigType, AUTOMATIC, ETH_APPL_CONST) CfgPtr
);

extern FUNC(Std_ReturnType, ETH_CODE) Eth_Arch_MiiInit
(
  uint8 CtrlIdx
);

extern FUNC(Std_ReturnType, ETH_CODE) Eth_Arch_ControllerInit
(
  uint8 CtrlIdx
);

extern FUNC(Std_ReturnType, ETH_CODE) Eth_Arch_SetControllerMode
(
  uint8 CtrlIdx,
  Eth_ModeType CtrlMode
);

extern FUNC(void, ETH_CODE) Eth_Arch_GetPhysAddr
(
  uint8 CtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETH_APPL_DATA) PhysAddrPtr
);

extern FUNC(void, ETH_CODE) Eth_Arch_SetPhysAddr
(
    uint8 CtrlIdx,
    P2CONST(uint8, AUTOMATIC, ETH_APPL_DATA) PhysAddrPtr
);

#if (ETH_ANY_MII_SUPPORT == STD_ON)
extern FUNC(Eth_ReturnType, ETH_CODE) Eth_Arch_WriteMii
(
  uint8 CtrlIdx,
  uint8 TrcvIdx,
  uint8 RegIdx,
  uint16 RegVal
);

extern FUNC(Eth_ReturnType, ETH_CODE) Eth_Arch_ReadMii
(
  uint8 CtrlIdx,
  uint8 TrcvIdx,
  uint8 RegIdx,
  P2VAR(uint16, AUTOMATIC, ETH_APPL_DATA) RegValPtr
);
#endif /* ETH_ANY_MII_SUPPORT == STD_ON */

extern FUNC(void, ETH_CODE) Eth_Arch_GetCounterState
(
  uint8 CtrlIdx,
  uint16 CtrOffs,
  P2VAR(uint32, AUTOMATIC, ETH_APPL_DATA) CtrValPtr
);

/* Deviation MISRAC2012-2 <START> */
extern FUNC(BufReq_ReturnType, ETH_CODE) Eth_Arch_ProvideTxBuffer
(
  uint8 CtrlIdx,
  uint8 Priority,
  P2VAR(Eth_BufIdxType, AUTOMATIC, ETH_APPL_DATA) BufIdxPtr,
  P2VAR(P2VAR(Eth_DataType, TYPEDEF, ETH_APPL_DATA), AUTOMATIC, ETH_APPL_DATA) BufPtr,
  P2VAR(uint16, AUTOMATIC, ETH_APPL_DATA) LenBytePtr
);
/* Deviation MISRAC2012-2 <STOP> */

extern FUNC(Std_ReturnType, ETH_CODE) Eth_Arch_Transmit
(
  uint8 CtrlIdx,
  Eth_BufIdxType BufId,
  Eth_FrameType FrameType,
  boolean TxConfirmation,
  uint16 LenByte,
  P2CONST(uint8, AUTOMATIC, ETH_APPL_DATA) PhysAddrPtr
);

#if (ETH_SUPPORT_RETRANSMIT == STD_ON)
extern FUNC(Std_ReturnType, ETHIF_CODE) Eth_Arch_Retransmit
(
  const uint8 CtrlIdx,
  const uint8 OrigCtrlIdx,
  CONSTP2VAR(Eth_BufIdxType, AUTOMATIC, ETH_APPL_DATA) pBufId,
  CONSTP2CONST(Eth_DataType, AUTOMATIC, ETHIF_APPL_DATA) pData,
  const Eth_FrameType FrameType,
  const uint16 LenByte,
  CONSTP2CONST(Eth_RetransmitInfoType, AUTOMATIC, ETH_APPL_DATA) pReInfo
);
#endif /* (ETH_SUPPORT_RETRANSMIT == STD_ON) */

#if (ETH_BUFFER_LIST_SUPPORT == STD_ON)
extern FUNC(Std_ReturnType, ETHIF_CODE) Eth_Arch_TransmitBufList
(
  const uint8 CtrlIdx,
  const uint8 Priority,
  CONSTP2CONST(Eth_BufListType, TYPEDEF, ETH_APPL_DATA) pBufLst,
  const uint8 BufLstNo,
  CONSTP2CONST(uint8, TYPEDEF, ETH_APPL_DATA) pUser
);
#endif /* (ETH_BUFFER_LIST_SUPPORT == STD_ON) */

extern FUNC(void, ETH_CODE) Eth_Arch_TxConfirmation
(
    uint8 CtrlIdx,
    boolean Irq
);

extern FUNC(void, ETH_CODE) Eth_Arch_Receive
(
  uint8 CtrlIdx,
  uint8 QueRxIdx,
  P2VAR(Eth_RxStatusType, AUTOMATIC, ETH_APPL_DATA) RxStatus,
  boolean Irq
);

extern FUNC(Std_ReturnType, ETH_CODE) Eth_Arch_UpdatePhysAddrFilter
(
  uint8 CtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETH_APPL_DATA) PhysAddrPtr,
  Eth_FilterActionType Action
);

#if (ETH_GLOBAL_TIME_SUPPORT == STD_ON)
extern FUNC(Std_ReturnType, ETH_CODE) Eth_Arch_GetCurrentTime
(
  uint8 CtrlIdx,
  P2VAR(Eth_TimeStampQualType, AUTOMATIC, ETH_APPL_DATA) TimeQualPtr,
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETH_APPL_DATA) TimeStampPtr
);

extern FUNC(void, ETH_CODE) Eth_Arch_EnableEgressTimeStamp
(
  uint8 CtrlIdx,
  Eth_BufIdxType BufId
);

extern FUNC(void, ETH_CODE) Eth_Arch_GetEgressTimeStamp
(
  uint8 CtrlIdx,
  Eth_BufIdxType BufId,
  P2VAR(Eth_TimeStampQualType, AUTOMATIC, ETH_APPL_DATA) TimeQualPtr,
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETH_APPL_DATA) TimeStampPtr
);

extern FUNC(void, ETH_CODE) Eth_Arch_GetIngressTimeStamp
(
  uint8 CtrlIdx,
  P2CONST(Eth_DataType, AUTOMATIC, ETH_APPL_DATA) DataPtr,
  P2VAR(Eth_TimeStampQualType, AUTOMATIC, ETH_APPL_DATA) TimeQualPtr,
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETH_APPL_DATA) TimeStampPtr
);

extern FUNC(void, ETH_CODE) Eth_Arch_SetCorrectionTime
(
  uint8 CtrlIdx,
  P2CONST(Eth_TimeIntDiffType, AUTOMATIC, ETH_APPL_DATA) TimeOffsetPtr,
  P2CONST(Eth_RateRatioType, AUTOMATIC, ETH_APPL_DATA) RateRatioPtr
);

extern FUNC(Std_ReturnType, ETH_CODE) Eth_Arch_SetGlobalTime
(
  uint8 CtrlIdx,
  P2CONST(Eth_TimeStampType, AUTOMATIC, ETH_APPL_DATA) TimeStampPtr
);

#endif /* (ETH_GLOBAL_TIME_SUPPORT == STD_ON) */



#if (ETH_ENABLE_RX_IRQ_CALLOUT_SUPPORT == STD_ON)
/** \brief Eth set rx interrupt usercallout mode
*
* This function set the mode of the rx interrupt usercallout.
*
* \param[in] CtrlIdx Controller index
* \param[in] QueRxIdx Rx queue index
* \param[in] Mode New mode
*/
extern FUNC(void, ETH_CODE) Eth_Arch_SetRxInterruptUsercalloutMode
(
  const uint8 CtrlIdx,
  const uint8 QueRxIdx,
  const Eth_ModeType Mode
);
#endif /* (ETH_ENABLE_RX_IRQ_CALLOUT_SUPPORT == STD_ON) */

#if (ETH_ENABLE_TX_IRQ_CALLOUT_SUPPORT == STD_ON)
/** \brief Eth set tx interrupt usercallout mode
*
* This function set the mode of the tx interrupt usercallout.
*
* \param[in] CtrlIdx Controller index
* \param[in] QueTxBufIdx Tx buffer queue index
* \param[in] Mode New mode
*/
extern FUNC(void, ETH_CODE) Eth_Arch_SetTxInterruptUsercalloutMode
(
  const uint8 CtrlIdx,
  const uint8 QueTxBufIdx,
  const Eth_ModeType Mode
);
#endif /* (ETH_ENABLE_TX_IRQ_CALLOUT_SUPPORT == STD_ON) */

#define ETH_STOP_SEC_CODE
#include <Eth_MemMap.h>


/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/
#define ETH_START_SEC_VAR_CLEARED_8
#include <Eth_MemMap.h>

/** \brief Stores the state of each Eth controller. */
extern VAR(Eth_CtrlStateType, ETH_VAR) Eth_CtrlState[ETH_CTRL_NO];

/** \brief Stores the mode of each Eth controller. */
extern VAR(Eth_ModeType, ETH_VAR) Eth_CtrlMode[ETH_CTRL_NO];

#define ETH_STOP_SEC_VAR_CLEARED_8
#include <Eth_MemMap.h>

#define ETH_START_SEC_VAR_INIT_8
#include <Eth_MemMap.h>

extern VAR(Eth_StateType, ETH_VAR) Eth_Initialized;

#define ETH_STOP_SEC_VAR_INIT_8
#include <Eth_MemMap.h>

#define ETH_START_SEC_VAR_CLEARED_32
#include <Eth_MemMap.h>

#if (ETH_ASSERT_BOUND_ARRY_ENABLED == STD_ON)
/** \brief Array with counter for array bound assert violations. */
extern VAR(volatile uint32, ETH_VAR) Eth_AssertBound[ETH_ASSERT_BOUND_ARRY_NO];
#endif /* (ETH_ASSERT_BOUND_ARRY_ENABLED == STD_ON) */

#if (ETH_ASSERT_CRITSEC_ENABLED == STD_ON)
/** \brief Array with counter for
 * - entering a critical section
 * - violation of a critical section.
 */
extern VAR(volatile uint32, ETH_VAR)
                               Eth_AssertCritSec[ETH_ASSERT_CRITSEC_NO][ETH_ASSERT_CRITSECCNT_NO];
#endif /* (ETH_ASSERT_CRITSEC_ENABLED == STD_ON) */

#if (ETH_ERROR_CNT_SUPPORT == STD_ON)
/** \brief Array with error counter. */
extern VAR(volatile uint32, ETH_VAR) Eth_ErrorCntLst[ETH_ERROR_CNT_NO];
#endif /* (ETH_ERROR_CNT_SUPPORT == STD_ON) */

#define ETH_STOP_SEC_VAR_CLEARED_32
#include <Eth_MemMap.h>

#define ETH_START_SEC_VAR_INIT_UNSPECIFIED
#include <Eth_MemMap.h>

/* pointer to post-build configuration */
extern P2CONST(Eth_ConfigType, ETH_VAR, ETH_APPL_CONST) Eth_RootPtr;

#define ETH_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <Eth_MemMap.h>
/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#undef TS_RELOCATABLE_CFG_ENABLE

#endif /* ifndef ETH_INT_H */
/*==================[end of file]===========================================*/

