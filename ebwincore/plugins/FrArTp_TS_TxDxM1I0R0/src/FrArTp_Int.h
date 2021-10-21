/**
 * \file
 *
 * \brief AUTOSAR FrArTp
 *
 * This file contains the implementation of the AUTOSAR
 * module FrArTp.
 *
 * \version 1.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef FRARTP_INT_H
#define FRARTP_INT_H

 /*==================[includes]================================================*/

/* source code needs no post-build config of itself or any other module */
/* these macro definitions help to avoid inclusion of <Mod>_PBcfg.h files when including foreign
 * headerfiles
 */
#ifndef FRARTP_NO_PBCFG_REQUIRED
#define FRARTP_NO_PBCFG_REQUIRED
#endif /* FRARTP_NO_PBCFG_REQUIRED */

#ifndef PDUR_NO_PBCFG_REQUIRED
#define PDUR_NO_PBCFG_REQUIRED
#endif /* PDUR_NO_PBCFG_REQUIRED */

#ifndef FRIF_NO_PBCFG_REQUIRED
#define FRIF_NO_PBCFG_REQUIRED
#endif /*FRIF_NO_PBCFG_REQUIRED  */

#include <SchM_FrArTp.h>           /* Schedule manager */

#include <FrArTp_Cfg.h>            /* FrArTp pre-compile-time configuration */
#include <FrArTp_CfgTypes.h>       /* FrArTp pre-compile-time dependent configuration types */
#include <FrArTp_Api.h>            /* DET macros / module global type definitions */
#include <FrArTp_Cbk.h>            /* callback prototypes */
#include <ComStack_Types.h>        /* Com Stack Types */

#include <TSAutosar.h>            /* EB specific global AUTOSAR macros */
#include <TSAtomic_Assign.h>      /* EB Atomic Assign macros */

#include <FrArTp_Trace.h>          /* debug and trace macros */

#if FRARTP_DEV_ERROR_DETECT == STD_ON
#include <Det.h>                  /* Det function declaration */
#endif

/*==================[macros]================================================*/

/** \brief Invalid item literals */
#define FRARTP_ACTCONN_INVALID            0xFFU  /**< value for invalid active connection index */
#define FRARTP_CONN_INVALID               0xFFU  /**< value for invalid connection index */
#define FRARTP_TXPDU_INVALID              0xFFU  /**< value for invalid TxPdu index */

/** \brief Maximum message length options */
#define FRARTP_FRARTPLM_ISO                                 0U
#define FRARTP_FRARTPLM_ISO6                                1U
#define FRARTP_FRARTPLM_L4G                                 2U

/** \brief Frame type IDs */
#define FRARTP_FT_SINGLE_FRAME_ID                         0x0U
#define FRARTP_FT_FIRST_FRAME_ID                          0x1U
#define FRARTP_FT_CONSECUTIVE_FRAME_ID                    0x2U
#define FRARTP_FT_FLOW_CONTROL_ID                         0x3U
#define FRARTP_FT_EXT_SINGLE_FRAME_ID                     0x4U
#define FRARTP_FT_EXT_FIRST_FRAME_ID                      0x5U

/** \brief PCI length in bytes */
#define FRARTP_PCI_SF_LENGTH                                1U
#define FRARTP_PCI_SF_E_LENGTH                              2U
#define FRARTP_PCI_FF_LENGTH                                2U
#define FRARTP_PCI_FF_E_LENGTH                              5U
#define FRARTP_PCI_CF_LENGTH                                1U
#define FRARTP_PCI_FC_LENGTH                                3U

/** \brief Maximum, invalid and default parameter values */
#define FRARTP_GENERAL_PARAMETER_MAX_VALUE                0xFFU
#define FRARTP_INVALID_STMIN_VALUES_0X80                  0X80U
#define FRARTP_INVALID_STMIN_VALUES_0XF0                  0xF0U
#define FRARTP_INVALID_STMIN_VALUES_0XFA                  0xFAU
#define FRARTP_INVALID_STMIN_VALUES_0XFF                  0xFFU
#define FRARTP_DEFAULT_STMIN_VALUE_0X7F                   0x7FU

/** \brief Frame field offsets */
#define FRARTP_PCI_FC_OFFSET_BS                              1U
#define FRARTP_PCI_FC_OFFSET_STMIN                           2U

/** \brief Instance Id for DET 2.0 (always 0, because only one FrArTp instance allowed) */
#define FRARTP_INSTANCE_ID 0U

/* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00220.1,1 */
#if (FRARTP_DEV_ERROR_DETECT == STD_ON)
/** \brief Macro for DET calls in FRARTP development-error-detection sections (Det enabled) */
#define FRARTP_REPORT_ERROR(ApiId, ErrorId)                      \
  (void) Det_ReportError(FRARTP_MODULE_ID, FRARTP_INSTANCE_ID, (ApiId), (ErrorId))
#else
/** \brief Macro for DET calls in FRARTP development-error-detection sections (Det disabled) */
#define FRARTP_REPORT_ERROR(ApiId, ErrorId)      \
  do {} while (0)
#endif /* (FRARTP_DEV_ERROR_DETECT == STD_ON) */

/*==================[defensive programming macros]=========================*/





/*------------------------[Defensive programming]----------------------------*/

#if (defined FRARTP_PRECONDITION_ASSERT)
#error FRARTP_PRECONDITION_ASSERT is already defined
#endif

#if (defined FRARTP_PRECONDITION_ASSERT_NO_EVAL)
#error FRARTP_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (FRARTP_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FRARTP_PRECONDITION_ASSERT(Condition, ApiId) \
  DET_PRECONDITION_ASSERT((Condition), FRARTP_MODULE_ID, FRARTP_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FRARTP_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL((Condition), FRARTP_MODULE_ID, FRARTP_INSTANCE_ID, (ApiId))
#else
#define FRARTP_PRECONDITION_ASSERT(Condition, ApiId)
#define FRARTP_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined FRARTP_POSTCONDITION_ASSERT)
#error FRARTP_POSTCONDITION_ASSERT is already defined
#endif

#if (defined FRARTP_POSTCONDITION_ASSERT_NO_EVAL)
#error FRARTP_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (FRARTP_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FRARTP_POSTCONDITION_ASSERT(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT((Condition), FRARTP_MODULE_ID, FRARTP_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FRARTP_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL((Condition), FRARTP_MODULE_ID, FRARTP_INSTANCE_ID, (ApiId))
#else
#define FRARTP_POSTCONDITION_ASSERT(Condition, ApiId)
#define FRARTP_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined FRARTP_INVARIANT_ASSERT)
#error FRARTP_INVARIANT_ASSERT is already defined
#endif

#if (defined FRARTP_INVARIANT_ASSERT_NO_EVAL)
#error FRARTP_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (FRARTP_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FRARTP_INVARIANT_ASSERT(Condition, ApiId) \
  DET_INVARIANT_ASSERT((Condition), FRARTP_MODULE_ID, FRARTP_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FRARTP_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL((Condition), FRARTP_MODULE_ID, FRARTP_INSTANCE_ID, (ApiId))
#else
#define FRARTP_INVARIANT_ASSERT(Condition, ApiId)
#define FRARTP_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined FRARTP_STATIC_ASSERT)
# error FRARTP_STATIC_ASSERT is already defined
#endif
#if (FRARTP_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated */
# define FRARTP_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define FRARTP_STATIC_ASSERT(expr)
#endif

#if (defined FRARTP_UNREACHABLE_CODE_ASSERT)
#error FRARTP_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (FRARTP_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 **

 ** \param[in] ApiId Service ID of the API function */
#define FRARTP_UNREACHABLE_CODE_ASSERT(ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(FRARTP_MODULE_ID, FRARTP_INSTANCE_ID, (ApiId))
#else
#define FRARTP_UNREACHABLE_CODE_ASSERT(ApiId)
#endif

#if (defined FRARTP_INTERNAL_API_ID)
#error FRARTP_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define FRARTP_INTERNAL_API_ID DET_INTERNAL_API_ID




#undef TS_RELOCATABLE_CFG_ENABLE

/*==================[type definitions]=========================*/

/*==================[external data declarations]=========================*/

#define FRARTP_START_SEC_VAR_INIT_8
#include <FrArTp_MemMap.h>

/** \brief FrArTp init state */
extern VAR(uint8, FRARTP_VAR) FrArTp_InitStatus;

#define FRARTP_STOP_SEC_VAR_INIT_8
#include <FrArTp_MemMap.h>

/*==================[external function declarations]=========================*/

#endif /* ifndef FRARTP_INT_H */
/*==================[end of file]===========================================*/
