/**
 * \file
 *
 * \brief AUTOSAR WdgM
 *
 * This file contains the implementation of the AUTOSAR
 * module WdgM.
 *
 * \version 6.1.39
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef WDGM_INT_H
#define WDGM_INT_H

/*==================[inclusions]=================================================================*/
/* !LINKSTO WDGM.EB.Design.IncludeFileStructure,1 */
#include <WdgM_Int_Types.h>                                             /* WdgM's Internal types */

/*==================[macros]=====================================================================*/




/*------------------------[Defensive programming]----------------------------*/

#if (defined WDGM_PRECONDITION_ASSERT)
#error WDGM_PRECONDITION_ASSERT is already defined
#endif

#if (defined WDGM_PRECONDITION_ASSERT_NO_EVAL)
#error WDGM_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (WDGM_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define WDGM_PRECONDITION_ASSERT(Condition, ApiId) \
  DET_PRECONDITION_ASSERT((Condition), WDGM_MODULE_ID, WDGM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define WDGM_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL((Condition), WDGM_MODULE_ID, WDGM_INSTANCE_ID, (ApiId))
#else
#define WDGM_PRECONDITION_ASSERT(Condition, ApiId)
#define WDGM_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined WDGM_POSTCONDITION_ASSERT)
#error WDGM_POSTCONDITION_ASSERT is already defined
#endif

#if (defined WDGM_POSTCONDITION_ASSERT_NO_EVAL)
#error WDGM_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (WDGM_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define WDGM_POSTCONDITION_ASSERT(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT((Condition), WDGM_MODULE_ID, WDGM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define WDGM_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL((Condition), WDGM_MODULE_ID, WDGM_INSTANCE_ID, (ApiId))
#else
#define WDGM_POSTCONDITION_ASSERT(Condition, ApiId)
#define WDGM_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined WDGM_INVARIANT_ASSERT)
#error WDGM_INVARIANT_ASSERT is already defined
#endif

#if (defined WDGM_INVARIANT_ASSERT_NO_EVAL)
#error WDGM_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (WDGM_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define WDGM_INVARIANT_ASSERT(Condition, ApiId) \
  DET_INVARIANT_ASSERT((Condition), WDGM_MODULE_ID, WDGM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define WDGM_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL((Condition), WDGM_MODULE_ID, WDGM_INSTANCE_ID, (ApiId))
#else
#define WDGM_INVARIANT_ASSERT(Condition, ApiId)
#define WDGM_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined WDGM_STATIC_ASSERT)
# error WDGM_STATIC_ASSERT is already defined
#endif
#if (WDGM_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated */
# define WDGM_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define WDGM_STATIC_ASSERT(expr)
#endif

#if (defined WDGM_UNREACHABLE_CODE_ASSERT)
#error WDGM_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (WDGM_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 **

 ** \param[in] ApiId Service ID of the API function */
#define WDGM_UNREACHABLE_CODE_ASSERT(ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(WDGM_MODULE_ID, WDGM_INSTANCE_ID, (ApiId))
#else
#define WDGM_UNREACHABLE_CODE_ASSERT(ApiId)
#endif

#if (defined WDGM_INTERNAL_API_ID)
#error WDGM_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define WDGM_INTERNAL_API_ID DET_INTERNAL_API_ID



#if (defined WDGM_EB_PARAM_UNUSED)
#error WDGM_EB_PARAM_UNUSED already defined
#endif
#if (defined EB_STATIC_CHECK)
/** \brief This macro can be used to avoid compiler warnings
 * It is left empty for static code analyze only with EB specific
 * tooling in order not to produce a false positive for MISRA 14.2 */
#define WDGM_EB_PARAM_UNUSED(x)
#else
/** \brief This macro can be used to avoid compiler warnings */
#define WDGM_EB_PARAM_UNUSED(x) ((void) (x))
#endif

#if (defined WDGM_EB_RESULT_CORRECT)
#error WDGM_EB_RESULT_CORRECT is already defined
#endif

/** \brief value for a correct result */
#define WDGM_EB_RESULT_CORRECT 0xB4U

#if (defined WDGM_EB_RESULT_INCORRECT)
#error WDGM_EB_RESULT_INCORRECT is already defined
#endif

/** \brief value for an incorrect result (quite different from correct, but not bit-inverse) */
#define WDGM_EB_RESULT_INCORRECT 0xCBU

#if (defined WDGM_EB_RESULT_NOTEVALUATED)
#error WDGM_EB_RESULT_NOTEVALUATED is already defined
#endif

/** \brief value for a not evaluated result */
#define WDGM_EB_RESULT_NOTEVALUATED 0xA3U

#if (defined WDGM_EB_MAX_ALIVE_COUNTER_VALUE)
#error WDGM_EB_MAX_ALIVE_COUNTER_VALUE is already defined
#endif

#if (defined WDGM_EB_E_SAME_MODE)
#error WDGM_EB_E_SAME_MODE is already defined
#endif

/** \brief WdgM_SetMode function is called consecutive with the same mode */
#define WDGM_EB_E_SAME_MODE 0xA4U

/** \brief maximum value for an alive counter */
#define WDGM_EB_MAX_ALIVE_COUNTER_VALUE 0xFFFFUL

/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/

#define WDGM_START_SEC_CODE_ASIL_D
#include <WdgM_MemMap.h>

#if (WDGM_EB_INCLUDE_RTE == STD_ON)

#if (WDGM_EB_DEFAULT_ASR_SERVICE_API != WDGM_EB_SERVICE_API_NONE)
/** \brief Signal individual SE mode change via RTE
 *
 * The function receives SE Id and status from the WdgM point of view and has to
 * translate them into the values expected by the RTE. The reported values depend
 * on the AUTOSAR service API selected as default.
 *
 * The return value of the RTE call is ignored on purpose because the WdgM
 * does not have a strategy to handle failing SW-C calls.
 *
 * \param[in] SEID  supervised entity whose status has changed
 * \param[in] LocalStatus  new alive supervision status
 * \param[in] CoreID  id of the core from which the function is called
 */
FUNC(void, WDGM_CODE) WdgM_RteIndividualModeSwitch (
  WdgM_SupervisedEntityIdType       SEID,
  WdgM_LocalStatusType              LocalStatus,
  WdgM_EB_CoreIdType                CoreID
);

/** \brief Signal global mode change via RTE
 *
 * The function receives the status from the WdgM point of view and  has to
 * translate them into the values expected by the RTE. The reported values depend
 * on the AUTOSAR service API selected as default.
 *
 * The return value of the RTE call is ignored on purpose because the WdgM
 * does not have a strategy to handle failing SW-C calls.
 *
 * \param[in] GlobalStatus  new global alive supervision status */
FUNC(void, WDGM_CODE) WdgM_RteGlobalModeSwitch (
  WdgM_GlobalStatusType GlobalStatus
);
#endif /* (WDGM_EB_DEFAULT_ASR_SERVICE_API != WDGM_EB_SERVICE_API_NONE) */

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
/** \brief Signal individual SE mode change via RTE
 *
 * The function receives SE Id and status from the WdgM point of view and has to
 * translate them into the AUTOSAR 3.2 values expected by the RTE.
 *
 * The return value of the RTE call is ignored on purpose because the WdgM
 * does not have a strategy to handle failing SW-C calls.
 *
 * \param[in] SEID  supervised entity whose status has changed
 * \param[in] LocalStatus  new alive supervision status
 * \param[in] CoreID  id of the core from which the function is called
 *
 * \note This function is called from WdgM_MainFunction and thus uses the
 * AUTOSAR 4.0 argument types. The mapping to the corresponding
 * AUTOSAR 3.2 types is done in the function definition. */
FUNC(void, WDGM_CODE) WdgM_ASR32_RteIndividualModeSwitch (
  WdgM_SupervisedEntityIdType       SEID,
  WdgM_LocalStatusType              LocalStatus,
  WdgM_EB_CoreIdType                CoreID
);

/** \brief Signal global mode change via RTE
 *
 * The function receives the status from the WdgM point of view and  has to
 * translate them into the AUTOSAR 3.2 values expected by the RTE.
 *
 * The return value of the RTE call is ignored on purpose because the WdgM
 * does not have a strategy to handle failing SW-C calls.
 *
 * \param[in] GlobalStatus  new global alive supervision status */
FUNC(void, WDGM_CODE) WdgM_ASR32_RteGlobalModeSwitch (
  WdgM_GlobalStatusType GlobalStatus
);
#endif /* (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON) */

#if (WDGM_EB_ENABLE_ASR40_SERVICE_API == STD_ON)
/** \brief Signal individual SE mode change via RTE
 *
 * The function receives SE Id and status from the WdgM point of view and has to
 * translate them into the AUTOSAR 4.0 values expected by the RTE.
 *
 * The return value of the RTE call is ignored on purpose because the WdgM
 * does not have a strategy to handle failing SW-C calls.
 *
 * \param[in] SEID  supervised entity whose status has changed
 * \param[in] LocalStatus  new alive supervision status
 * \param[in] CoreID  id of the core from which the function is called
 */
FUNC(void, WDGM_CODE) WdgM_ASR40_RteIndividualModeSwitch (
  WdgM_SupervisedEntityIdType       SEID,
  WdgM_LocalStatusType              LocalStatus,
  WdgM_EB_CoreIdType                CoreID
);

/** \brief Signal global mode change via RTE
 *
 * The function receives the status from the WdgM point of view and  has to
 * translate them into the AUTOSAR 4.0 values expected by the RTE.
 *
 * The return value of the RTE call is ignored on purpose because the WdgM
 * does not have a strategy to handle failing SW-C calls.
 *
 * \param[in] GlobalStatus  new global alive supervision status */
FUNC(void, WDGM_CODE) WdgM_ASR40_RteGlobalModeSwitch (
  WdgM_GlobalStatusType GlobalStatus
);
#endif /* (WDGM_EB_ENABLE_ASR40_SERVICE_API == STD_ON) */

#if (WDGM_EB_ENABLE_ASR43_SERVICE_API == STD_ON)
/** \brief Signal individual SE mode change via RTE
 *
 * The function receives SE Id and status from the WdgM point of view and has to
 * translate them into the AUTOSAR 4.3 values expected by the RTE.
 *
 * The return value of the RTE call is ignored on purpose because the WdgM
 * does not have a strategy to handle failing SW-C calls.
 *
 * \param[in] SEID  supervised entity whose status has changed
 * \param[in] LocalStatus  new alive supervision status
 * \param[in] CoreID  id of the core from which the function is called
 */
FUNC(void, WDGM_CODE) WdgM_ASR43_RteIndividualModeSwitch (
  WdgM_SupervisedEntityIdType       SEID,
  WdgM_LocalStatusType              LocalStatus,
  WdgM_EB_CoreIdType                CoreID
);

/** \brief Signal global mode change via RTE
 *
 * The function receives the status from the WdgM point of view and  has to
 * translate them into the AUTOSAR 4.3 values expected by the RTE.
 *
 * The return value of the RTE call is ignored on purpose because the WdgM
 * does not have a strategy to handle failing SW-C calls.
 *
 * \param[in] GlobalStatus  new global alive supervision status */
FUNC(void, WDGM_CODE) WdgM_ASR43_RteGlobalModeSwitch (
  WdgM_GlobalStatusType GlobalStatus
);
#endif /* (WDGM_EB_ENABLE_ASR43_SERVICE_API == STD_ON) */
#endif /* (WDGM_EB_INCLUDE_RTE == STD_ON) */

#define WDGM_STOP_SEC_CODE_ASIL_D
#include <WdgM_MemMap.h>

/*==================[external constants]=========================================================*/

/*==================[external data]==============================================================*/

#endif /* ifndef WDGM_INT_H */
/*==================[end of file]================================================================*/
