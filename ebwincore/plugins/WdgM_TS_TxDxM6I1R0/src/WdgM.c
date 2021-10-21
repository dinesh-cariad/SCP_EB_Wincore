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
/*
 * Misra-C:2012 Deviations:
 *
 * MISRAC2012-1) Deviated Rule: 8.9 (advisory)
 * An object should be defined at block scope if its identifier only appears
 * in a single function.
 *
 * Reason:
 * The object is defined in this way because the values that are stored
 * in them are required for the next call of this function and should be
 * hold in the data segment memory.
 */


/*==================[inclusions]=================================================================*/

/* !LINKSTO WDGM.EB.Design.IncludeFileStructure,1 */
/* !LINKSTO WDGM.EB.Dbg3,1 */
#include <WdgM_Trace_Stc.h>
#include <WdgM.h>
#include <WdgM_Lcfg.h>                                             /* BSW specific configuration */
#include <WdgM_Int.h>                                                   /* Internal declarations */

#include <WdgIf.h>                                                /* WdgIf_SetTriggerCondition() */

#if (WDGM_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>                                               /* Det error reporting interface */
#endif
#if (WDGM_DEM_ALIVE_SUPERVISION_REPORT == STD_ON)                        \
    || (WDGM_EB_PROD_ERR_HANDLING_SUPERVISION == WDGM_PROD_ERR_REP_TO_DEM) \
    || (WDGM_EB_PROD_ERR_HANDLING_SET_MODE == WDGM_PROD_ERR_REP_TO_DEM)    \
    || (WDGM_EB_PROD_ERR_HANDLING_IMPROPER_CALLER == WDGM_PROD_ERR_REP_TO_DEM) \
    || (WDGM_EB_PROD_ERR_HANDLING_MF_TIMINGVIOLATION == WDGM_PROD_ERR_REP_TO_DEM) \
    || (WDGM_EB_PROD_ERR_HANDLING_DATA_CORRUPTION == WDGM_PROD_ERR_REP_TO_DEM) \
/* !LINKSTO WDGM.ASR40.WDGM126,1 */
#include <Dem.h>                                                       /* Dem_ReportErrorStatus */
#endif
#if (WDGM_IMMEDIATE_RESET == STD_ON)
#include <Mcu.h>                                                            /* Mcu_PerformReset */
#endif

/*==================[macros]=====================================================================*/

/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef WDGM_VENDOR_ID /* configuration check */
#error WDGM_VENDOR_ID must be defined
#endif

#if (WDGM_VENDOR_ID != 1U) /* vendor check */
#error WDGM_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef WDGM_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error WDGM_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (WDGM_AR_RELEASE_MAJOR_VERSION != 4U)
#error WDGM_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef WDGM_AR_RELEASE_MINOR_VERSION /* configuration check */
#error WDGM_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (WDGM_AR_RELEASE_MINOR_VERSION != 0U )
#error WDGM_AR_RELEASE_MINOR_VERSION wrong (!= 0U)
#endif

#ifndef WDGM_AR_RELEASE_REVISION_VERSION /* configuration check */
#error WDGM_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (WDGM_AR_RELEASE_REVISION_VERSION != 3U )
#error WDGM_AR_RELEASE_REVISION_VERSION wrong (!= 3U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef WDGM_SW_MAJOR_VERSION /* configuration check */
#error WDGM_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (WDGM_SW_MAJOR_VERSION != 6U)
#error WDGM_SW_MAJOR_VERSION wrong (!= 6U)
#endif

#ifndef WDGM_SW_MINOR_VERSION /* configuration check */
#error WDGM_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (WDGM_SW_MINOR_VERSION < 1U)
#error WDGM_SW_MINOR_VERSION wrong (< 1U)
#endif

#ifndef WDGM_SW_PATCH_VERSION /* configuration check */
#error WDGM_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (WDGM_SW_PATCH_VERSION < 39U)
#error WDGM_SW_PATCH_VERSION wrong (< 39U)
#endif

#if ((WDGM_EB_BSW_COMPATIBILITY == WDGM_EB_BSW_COMPATIBILITY_ASR40) && \
    (WDGM_DEV_ERROR_DETECT == STD_ON) && \
    (WDGM_EB_EXT_GET_EXPECTED_WDGM_MODE_ENABLED == STD_OFF) && \
    ((WDGM_DEFENSIVE_BEHAVIOR == STD_ON) && (WDGM_EB_CALLERIDS_ENABLED == STD_ON)) && \
    ((WDGM_EB_PROD_ERR_HANDLING_IMPROPER_CALLER == WDGM_PROD_ERR_REP_TO_DEM) || \
     (WDGM_EB_PROD_ERR_HANDLING_IMPROPER_CALLER == WDGM_PROD_ERR_REP_TO_DET)))
/** \brief Check if Caller ID shall be validated */
#define WDGM_EB_CHECK_CALLERID STD_ON
#else
#define WDGM_EB_CHECK_CALLERID STD_OFF
#endif

/** \brief simplified error reporting */
#if (defined WDGM_EB_DET_REPORT_ERROR)
#error WDGM_EB_DET_REPORT_ERROR already defined
#endif
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
/* !LINKSTO WDGM.ASR40.WDGM048,1 */
#define WDGM_EB_DET_REPORT_ERROR(sid, error)          \
  (void)Det_ReportError(                              \
      ((uint16) WDGM_MODULE_ID),                      \
      ((uint8)  WDGM_EB_GET_CORE_ID()),               \
      ((uint8) (sid)),                                \
      ((uint8) (error))                               \
      )
#else
/* empty define only needed for the case (WDGM_DEV_ERROR_DETECT == STD_OFF) &&
 * ((WDGM_EB_PROD_ERR_HANDLING_SUPERVISION == WDGM_PROD_ERR_REP_TO_DET) ||
 * ((WDGM_EB_PROD_ERR_HANDLING_SET_MODE == WDGM_PROD_ERR_REP_TO_DET) ||
 * ((WDGM_EB_PROD_ERR_HANDLING_IMPROPER_CALLER == WDGM_PROD_ERR_REP_TO_DET))
 */
#define WDGM_EB_DET_REPORT_ERROR(sid, error)
#endif

#if (defined WDGM_EB_RTEINDIVIDUALMODESWITCH)
#error WDGM_EB_RTEINDIVIDUALMODESWITCH already defined
#endif
#if ((WDGM_EB_INCLUDE_RTE == STD_ON) && \
     (WDGM_EB_DEFAULT_ASR_SERVICE_API != WDGM_EB_SERVICE_API_NONE))
/** \brief Signal individual SE mode change via RTE
 *
 * The macro receives SE Id and status from the WdgM point of view and
 * propagates the arguments to the corresponding default RTE handle function.
 *
 * If the default service API is set to NONE then the macro is defined empty.
 *
 * The return value of the RTE call is ignored on purpose because the WdgM
 * does not have a strategy to handle failing SW-C calls.
 *
 * \param[in] SEID  supervised entity whose status has changed
 * \param[in] status  new alive supervision status
 * \param[in] CoreID  id of the core from which the function is called
 */
#define WDGM_EB_RTEINDIVIDUALMODESWITCH(SEID, status, CoreID)      \
  WdgM_RteIndividualModeSwitch(SEID, status, CoreID)
#else
#define WDGM_EB_RTEINDIVIDUALMODESWITCH(SEID, status, CoreID)
#endif

#if (defined WDGM_EB_ASR32_RTEINDIVIDUALMODESWITCH)
#error WDGM_EB_ASR32_RTEINDIVIDUALMODESWITCH already defined
#endif
#if ((WDGM_EB_INCLUDE_RTE == STD_ON) && \
     (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON))
/** \brief Signal individual SE mode change via RTE
 *
 * The macro receives SE Id and status from the WdgM point of view and
 * propagates the arguments to the corresponding AUTOSAR 3.2 RTE handle function.
 *
 * If the AUTOSAR 3.2 service API is not enabled then the macro is defined empty.
 *
 * The return value of the RTE call is ignored on purpose because the WdgM
 * does not have a strategy to handle failing SW-C calls.
 *
 * \param[in] SEID  supervised entity whose status has changed
 * \param[in] status  new alive supervision status
 * \param[in] CoreID  id of the core from which the function is called
 */
#define WDGM_EB_ASR32_RTEINDIVIDUALMODESWITCH(SEID, status, CoreID)      \
  WdgM_ASR32_RteIndividualModeSwitch(SEID, status, CoreID)
#else
#define WDGM_EB_ASR32_RTEINDIVIDUALMODESWITCH(SEID, status, CoreID)
#endif

#if (defined WDGM_EB_ASR40_RTEINDIVIDUALMODESWITCH)
#error WDGM_EB_ASR40_RTEINDIVIDUALMODESWITCH already defined
#endif
#if ((WDGM_EB_INCLUDE_RTE == STD_ON) && \
     (WDGM_EB_ENABLE_ASR40_SERVICE_API == STD_ON))
/** \brief Signal individual SE mode change via RTE
 *
 * The macro receives SE Id and status from the WdgM point of view and
 * propagates the arguments to the corresponding AUTOSAR 4.0 RTE handle function.
 *
 * If the AUTOSAR 4.0 service API is not enabled then the macro is defined empty.
 *
 * The return value of the RTE call is ignored on purpose because the WdgM
 * does not have a strategy to handle failing SW-C calls.
 *
 * \param[in] SEID  supervised entity whose status has changed
 * \param[in] status  new alive supervision status
 * \param[in] CoreID  id of the core from which the function is called
 */
#define WDGM_EB_ASR40_RTEINDIVIDUALMODESWITCH(SEID, status, CoreID)      \
  WdgM_ASR40_RteIndividualModeSwitch(SEID, status, CoreID)
#else
#define WDGM_EB_ASR40_RTEINDIVIDUALMODESWITCH(SEID, status, CoreID)
#endif

#if (defined WDGM_EB_ASR43_RTEINDIVIDUALMODESWITCH)
#error WDGM_EB_ASR43_RTEINDIVIDUALMODESWITCH already defined
#endif
#if ((WDGM_EB_INCLUDE_RTE == STD_ON) && \
     (WDGM_EB_ENABLE_ASR43_SERVICE_API == STD_ON))
/** \brief Signal individual SE mode change via RTE
 *
 * The macro receives SE Id and status from the WdgM point of view and
 * propagates the arguments to the corresponding AUTOSAR 4.3 RTE handle function.
 *
 * If the AUTOSAR 4.3 service API is not enabled then the macro is defined empty.
 *
 * The return value of the RTE call is ignored on purpose because the WdgM
 * does not have a strategy to handle failing SW-C calls.
 *
 * \param[in] SEID  supervised entity whose status has changed
 * \param[in] status  new alive supervision status
 * \param[in] CoreID  id of the core from which the function is called
 */
#define WDGM_EB_ASR43_RTEINDIVIDUALMODESWITCH(SEID, status, CoreID)      \
  WdgM_ASR43_RteIndividualModeSwitch(SEID, status, CoreID)
#else
#define WDGM_EB_ASR43_RTEINDIVIDUALMODESWITCH(SEID, status, CoreID)
#endif

#if (defined WDGM_EB_RTEGLOBALMODESWITCH)
#error WDGM_EB_RTEGLOBALMODESWITCH already defined
#endif
#if ((WDGM_EB_INCLUDE_RTE == STD_ON) && \
    (WDGM_EB_DEFAULT_ASR_SERVICE_API != WDGM_EB_SERVICE_API_NONE))
/** \brief Signal global mode change to RTE
 *
 * The macro receives the status from the WdgM point of view and
 * propagates the arguments to the corresponding default RTE handle function.
 *
 * If the default service API is set to NONE then the macro is defined empty.
 *
 * The return value of the RTE call is ignored on purpose because the WdgM
 * does not have a strategy to handle failing SW-C calls.
 *
 * \param[in] status  new global alive supervision status
 */
#define WDGM_EB_RTEGLOBALMODESWITCH(status) WdgM_RteGlobalModeSwitch(status)
#else
#define WDGM_EB_RTEGLOBALMODESWITCH(status)
#endif

#if (defined WDGM_EB_ASR32_RTEGLOBALMODESWITCH)
#error WDGM_EB_ASR32_RTEGLOBALMODESWITCH already defined
#endif
#if ((WDGM_EB_INCLUDE_RTE == STD_ON) && (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON))
/** \brief Signal global mode change to RTE
 *
 * The macro receives the status from the WdgM point of view and
 * propagates the arguments to the corresponding AUTOSAR 3.2 RTE handle function.
 *
 * If the AUTOSAR 3.2 service API is not enabled then the macro is defined empty.
 *
 * The return value of the RTE call is ignored on purpose because the WdgM
 * does not have a strategy to handle failing SW-C calls.
 *
 * \param[in] status  new global alive supervision status
 */
#define WDGM_EB_ASR32_RTEGLOBALMODESWITCH(status) WdgM_ASR32_RteGlobalModeSwitch(status)
#else
#define WDGM_EB_ASR32_RTEGLOBALMODESWITCH(status)
#endif

#if (defined WDGM_EB_ASR40_RTEGLOBALMODESWITCH)
#error WDGM_EB_ASR40_RTEGLOBALMODESWITCH already defined
#endif
#if ((WDGM_EB_INCLUDE_RTE == STD_ON) && (WDGM_EB_ENABLE_ASR40_SERVICE_API == STD_ON))
/** \brief Signal global mode change to RTE
 *
 * The macro receives the status from the WdgM point of view and
 * propagates the arguments to the corresponding AUTOSAR 4.0 RTE handle function.
 *
 * If the AUTOSAR 4.0 service API is not enabled then the macro is defined empty.
 *
 * The return value of the RTE call is ignored on purpose because the WdgM
 * does not have a strategy to handle failing SW-C calls.
 *
 * \param[in] status  new global alive supervision status
 */
#define WDGM_EB_ASR40_RTEGLOBALMODESWITCH(status) WdgM_ASR40_RteGlobalModeSwitch(status)
#else
#define WDGM_EB_ASR40_RTEGLOBALMODESWITCH(status)
#endif

#if (defined WDGM_EB_ASR43_RTEGLOBALMODESWITCH)
#error WDGM_EB_ASR43_RTEGLOBALMODESWITCH already defined
#endif
#if ((WDGM_EB_INCLUDE_RTE == STD_ON) && (WDGM_EB_ENABLE_ASR43_SERVICE_API == STD_ON))
/** \brief Signal global mode change to RTE
 *
 * The macro receives the status from the WdgM point of view and
 * propagates the arguments to the corresponding AUTOSAR 4.3 RTE handle function.
 *
 * If the AUTOSAR 4.3 service API is not enabled then the macro is defined empty.
 *
 * The return value of the RTE call is ignored on purpose because the WdgM
 * does not have a strategy to handle failing SW-C calls.
 *
 * \param[in] status  new global alive supervision status
 */
#define WDGM_EB_ASR43_RTEGLOBALMODESWITCH(status) WdgM_ASR43_RteGlobalModeSwitch(status)
#else
#define WDGM_EB_ASR43_RTEGLOBALMODESWITCH(status)
#endif

#if (defined WDGM_EB_GET_TIME)
#error WDGM_EB_GET_TIME already defined
#endif
#if (WDGM_EB_EXT_GET_TIME_ENABLED == STD_ON)
#define WDGM_EB_GET_TIME(ActualTimeTicksPtr, ElapsedTimeTicksPtr) \
  WDGM_EB_EXT_GET_TIME(ActualTimeTicksPtr, ElapsedTimeTicksPtr)
#else
#if (WDGM_EB_DEADLINE_SUPERVISION_ENABLED == STD_OFF)
#define WDGM_EB_GET_TIME(ActualTimeTicksPtr, ElapsedTimeTicksPtr)
#endif
#endif

#if (defined WDGM_EB_SUPERVISION_EXPIRED)
#error WDGM_EB_SUPERVISION_EXPIRED already defined
#endif
#if (WDGM_EB_EXT_SUPERVISION_EXPIRED_ENABLED == STD_ON)
#define WDGM_EB_SUPERVISION_EXPIRED(SEID) WDGM_EB_EXT_SUPERVISION_EXPIRED(SEID)
#else
#define WDGM_EB_SUPERVISION_EXPIRED(SEID)
#endif

#if (defined WDGM_EB_INDIVIDUAL_MODE_SWITCH)
#error WDGM_EB_INDIVIDUAL_MODE_SWITCH already defined
#endif
#if (WDGM_EB_EXT_INDIVIDUAL_MODE_SWITCH_ENABLED == STD_ON)
#define WDGM_EB_INDIVIDUAL_MODE_SWITCH(SEID, OldStatus, NewStatus, CoreID) \
  do { \
    WDGM_EB_EXT_INDIVIDUAL_MODE_SWITCH((SEID), (OldStatus), (NewStatus)); \
    WDGM_EB_RTEINDIVIDUALMODESWITCH((SEID), (NewStatus), (CoreID)); \
    WDGM_EB_ASR32_RTEINDIVIDUALMODESWITCH((SEID), (NewStatus), (CoreID)); \
    WDGM_EB_ASR40_RTEINDIVIDUALMODESWITCH((SEID), (NewStatus), (CoreID)); \
  } while (0)
#else
#define WDGM_EB_INDIVIDUAL_MODE_SWITCH(SEID, OldStatus, NewStatus, CoreID) \
  do { \
    WDGM_EB_PARAM_UNUSED((OldStatus)); \
    WDGM_EB_PARAM_UNUSED((NewStatus)); \
    WDGM_EB_RTEINDIVIDUALMODESWITCH((SEID), (NewStatus), (CoreID)); \
    WDGM_EB_ASR32_RTEINDIVIDUALMODESWITCH((SEID), (NewStatus), (CoreID)); \
    WDGM_EB_ASR40_RTEINDIVIDUALMODESWITCH((SEID), (NewStatus), (CoreID)); \
    WDGM_EB_ASR43_RTEINDIVIDUALMODESWITCH((SEID), (NewStatus), (CoreID)); \
  } while (0)
#endif

#if (defined WDGM_EB_GLOBAL_MODE_SWITCH)
#error WDGM_EB_GLOBAL_MODE_SWITCH already defined
#endif
#if (WDGM_EB_EXT_GLOBAL_MODE_SWITCH_ENABLED == STD_ON)
#define WDGM_EB_GLOBAL_MODE_SWITCH(OldStatus, NewStatus) \
  do { \
    WDGM_EB_EXT_GLOBAL_MODE_SWITCH((OldStatus), (NewStatus)); \
    WDGM_EB_RTEGLOBALMODESWITCH((NewStatus)); \
    WDGM_EB_ASR32_RTEGLOBALMODESWITCH((NewStatus)); \
    WDGM_EB_ASR40_RTEGLOBALMODESWITCH((NewStatus)); \
  } while (0)
#else
#define WDGM_EB_GLOBAL_MODE_SWITCH(OldStatus, NewStatus) \
  do { \
    WDGM_EB_PARAM_UNUSED((OldStatus)); \
    WDGM_EB_PARAM_UNUSED((NewStatus)); \
    WDGM_EB_RTEGLOBALMODESWITCH((NewStatus)); \
    WDGM_EB_ASR32_RTEGLOBALMODESWITCH((NewStatus)); \
    WDGM_EB_ASR40_RTEGLOBALMODESWITCH((NewStatus)); \
    WDGM_EB_ASR43_RTEGLOBALMODESWITCH((NewStatus)); \
  } while (0)
#endif

#if (defined WDGM_EB_DET)
#error WDGM_EB_DET already defined
#endif
/* !LINKSTO WDGM.EB.TIMEPM.WDGM020302,1 */
#if (WDGM_EB_EXT_DET_ENABLED == STD_ON)
/* !LINKSTO WDGM.EB.TIMEPM.WDGM020440,1,WDGM.EB.TIMEPM.WDGM020443,1,WDGM.EB.TIMEPM.WDGM020446,1 */
#define WDGM_EB_DET(SID, ErrorID) WDGM_EB_EXT_DET((SID), (ErrorID))
#else
#define WDGM_EB_DET(SID, ErrorID) WDGM_EB_DET_REPORT_ERROR((SID), (ErrorID))
#endif

#if (defined WDGM_EB_DEM)
#error WDGM_EB_DEM already defined
#endif
#define WDGM_EB_DEM(EventID, EventStatus) Dem_ReportErrorStatus((EventID), (EventStatus))

/*------------------[macros for EB Defensive Programming if not provided by Det]-----------------*/

#if (WDGM_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
#ifndef DET_E_UNREACHABLE_CODE_ASSERT_FAILED
/** \brief Reserved error value for unreachable code assertions (if not provided by Det) */
#define DET_E_UNREACHABLE_CODE_ASSERT_FAILED 232U
#endif

#ifndef DET_UNREACHABLE_CODE_ASSERT
/** \brief Report a unreachable code assertion (if not already defined by Det)
 **
 ** Use this macro in BSW modules to mark unreachable code as part of
 ** defensive programming.
 **
 ** \param[in] ModuleId The id of the reporting module
 ** \param[in] InstanceId The instance Id of the reporting module
 ** \param[in] ApiId The Api function where the check failed */
#define DET_UNREACHABLE_CODE_ASSERT(ModuleId, InstanceId, ApiId)        \
  do                                                                    \
  {                                                                     \
    WDGM_EB_DET((ApiId), DET_E_UNREACHABLE_CODE_ASSERT_FAILED);         \
  }                                                                     \
  while (0)
#endif
#endif /* (WDGM_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON) */

#if (WDGM_PRECONDITION_ASSERT_ENABLED == STD_ON)
#ifndef DET_E_PRECONDITION_ASSERT_FAILED
/** \brief Reserved error value for pre-condition assertions (if not provided by Det) */
#define DET_E_PRECONDITION_ASSERT_FAILED 230U
#endif

#ifndef DET_PRECONDITION_ASSERT
/** \brief Report a precondition assertion (if not already defined by Det)
 **
 ** Use this macro in BSW modules to check a defensive programming
 ** precondition assertion.
 **
 ** \param[in] Condition The condition check
 ** \param[in] ModuleId The id of the reporting module
 ** \param[in] InstanceId The instance Id of the reporting module
 ** \param[in] ApiId The Api function where the check failed */
#define DET_PRECONDITION_ASSERT(Condition, ModuleId, InstanceId, ApiId) \
  do                                                                    \
  {                                                                     \
    if (!(Condition))                                                   \
    {                                                                   \
      WDGM_EB_DET((ApiId), DET_E_PRECONDITION_ASSERT_FAILED);           \
    }                                                                   \
  }                                                                     \
  while (0)
#endif
#endif /* (WDGM_PRECONDITION_ASSERT_ENABLED == STD_ON) */

/*------------------[pre-compile time consistency checks]----------------------------------------*/

#if (WDGM_EB_EXT_GET_TIME_ENABLED == STD_OFF)
# if (WDGM_EB_DEADLINE_SUPERVISION_ENABLED == STD_ON)
#  error Deadline Supervision is enabled, but no external GetElapsedTime Callout is configured
# endif
#endif

/*==================[type definitions]===========================================================*/

/** \brief size of index to internal RAM must be size of SEId multiplied with size of CPId */
typedef uint32 WdgM_EB_IntRamIdxType;

/*==================[internal function declarations]=============================================*/

#define WDGM_START_SEC_CODE_ASIL_D
#include <WdgM_MemMap.h>

/** \brief Initialize the Watchdog Manager.
 *
 * Note: The WdgM is always initialized with Global Status STOPPED independent of
 * returned Success status.
 *
 * \param[in] CoreInstance  Core instance specific data
 * \return Success of operation
 * \retval E_OK     Successfully initialized
 * \retval E_NOT_OK Initialization failed
 */
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_Init
(
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
);

/** \brief De-Initialize the Watchdog Manager
 *
 * Note: The WdgM is always de-initialized with Global Status DEACTIVATED independent of
 * returned Success status.
 *
 * \param[in] CoreInstance  Core instance specific data
 * \return Success of operation
 * \retval E_OK     Successfully de-initialized
 * \retval E_NOT_OK De-Initialization failed (Only possible if DeInit / Sleep mode is configured)
 */
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_DeInit
(
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
);
/** \brief Set the current mode of the Watchdog Manager and initialize all RAM data for
 *         the configured Supervision mechanisms in this mode.
 *
 * \param[in] Mode  One of the configured Watchdog Manager modes
 * \param[in] IsNormalOperation Whether or not the
 *                Local Supervision Status shall be set to OK or DEACTIVATED
 *                irrespective of the old Local and Global Supervision Status.
 *
 * If IsNormalOperation equals TRUE (called during normal operation),
 * then WdgM_EB_SetMode behaves as specified.
 * If IsNormalOperation equals FALSE (called during initialization phase), then
 * the Local Supervision Status is set to OK or DEACTIVATED
 * irrespective of the old Local and Global Supervision Status.
 *
 * \return Success of operation
 * \retval E_OK     Successfully changed to the new mode
 * \retval E_NOT_OK Changing to the new mode failed
 *
 * \param[in] CoreInstance  Core instance specific data
 * \pre Mode is a valid configured mode
 * \pre WdgM is initialized
 */
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_SetMode
(
  WdgM_ModeType Mode,
  boolean IsNormalOperation,
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
);

/** \brief Calls WdgIf to perform a mode switch
 *
 * \param[in] Mode One of the configured Watchdog Manager modes
 *
 * \return Success of operation
 * \retval E_OK the call to WdgIf_Setmode was ok
 * \retval E_NOT_OK the call to WdgIf_SetMode failed */
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_PerformWdgIfModeSwitch
(
  WdgM_ModeType Mode
);

#if (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_ON)
/** \brief performs internal logical supervision of a checkpoint
 *
 * \pre Parameter \a CPIntLogicalCfg points to a valid internal Logical Supervision Configuration
 *
 * \param[in] IntSEId internal SE Id
 * \param[in] CPId checkpoint id
 * \param[in] CPIntLogicalCfg Pointer to Logical Supervision Configuration
 *
 * \return Success of operation
 * \retval E_OK supervision was ok
 * \retval E_NOT_OK supervision failed */
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_CheckpointReachedIntLogical
(
  WdgM_EB_SEIdType IntSEId,
  WdgM_EB_CPIdType CPId,
  P2CONST(WdgM_EB_CPLogicalCfgType,AUTOMATIC,WDGM_CONST) CPIntLogicalCfg
);

#if (WDGM_EB_EXTERNAL_GRAPH_NUM > 0U)
/** \brief performs external logical supervision of a checkpoint
 *
 * \pre Parameter \a CPExtLogicalCfg points to a valid external Logical Supervision Configuration
 *
 * \param[in] IntSEId internal SE Id
 * \param[in] CPId checkpoint id
 * \param[in] CPExtLogicalCfg Pointer to Logical Supervision Configuration
 * \param[in] CoreInstance  Core instance specific data
 *
 * \return Success of operation
 * \retval E_OK supervision was ok
 * \retval E_NOT_OK supervision failed */
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_CheckpointReachedExtLogical
(
  WdgM_EB_SEIdType IntSEId,
  WdgM_EB_CPIdType CPId,
  P2CONST(WdgM_EB_CPLogicalCfgType,AUTOMATIC,WDGM_CONST) CPExtLogicalCfg,
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
);

/** \brief get the last written index to can get the last written counter
 *
 * \param[in] IntSEId internal SE Id
 * \param[in] CPId checkpoint id
 * \param[in] CoreInstance  Core instance specific data
 *
 * \return index
 */
static FUNC (Std_ReturnType, WDGM_CODE) WdgM_EB_GetLastIndexInGraphDataSeq
(
  WdgM_EB_SEIdType LastIntSEId,
  WdgM_EB_CPIdType LastCPId,
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
);
#endif /* if (WDGM_EB_EXTERNAL_GRAPH_NUM > 0U) */
#endif

#if (WDGM_EB_DEADLINE_SUPERVISION_ENABLED == STD_ON)
/** \brief performs deadline monitoring of a checkpoint
 *
 * \pre Parameter \a CPDeadlineCfg points to a valid Deadline Supervision Configuration
 *
 * \param[in] IntSEId internal SE Id
 * \param[in] CPId checkpoint id
 * \param[in] CPDeadlineCfg Pointer to Deadline Supervision Configuration
 *
 * \return Success of operation
 * \retval E_OK supervision was ok
 * \retval E_NOT_OK supervision failed */
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_CheckpointReachedDeadline
(
  WdgM_EB_SEIdType IntSEId,
  WdgM_EB_CPIdType CPId,
  P2CONST(WdgM_EB_CPDeadlineCfgType,AUTOMATIC,WDGM_CONST) CPDeadlineCfg
);

/** \brief performs deadline monitoring of a start checkpoint and sets the result
 *
 * \param[inout] StartDMData Pointer to the Deadline Monitoring data of the Start Checkpoint
 * \param[in] StopDMData Pointer to the Deadline Monitoring data of the Stop Checkpoint
 *                       (Is different to StartDMData in case this is a Stop / Start Checkpoint)
 * \param[inout] CPData Pointer to the checkpoint data
 * \param[in] CPDeadlineCfg Pointer to the Deadline Supervision Configuration
 * \param[in] ErrorRecoveryEnabled Whether or not a failed deadline monitoring
 *               shall be tracked (FALSE), or shall be reset (TRUE) if same
 *               Deadline monitoring is executed again.
 *
 * \return Success of operation
 * \retval E_OK Deadline supervision of this Checkpoint was ok
 * \retval E_NOT_OK Deadline supervision of this Checkpoint failed */
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_StartCheckpointDeadline
(
  P2VAR(WdgM_EB_DMDataType,AUTOMATIC,WDGM_VAR) StartDMData,
  P2CONST(WdgM_EB_DMDataType,AUTOMATIC,WDGM_VAR) StopDMData,
  P2VAR(WdgM_EB_CPDataType,AUTOMATIC,WDGM_VAR) CPData,
  P2CONST(WdgM_EB_CPDeadlineCfgType,AUTOMATIC,WDGM_CONST) CPDeadlineCfg,
  boolean ErrorRecoveryEnabled
);

/** \brief performs deadline monitoring of a stop checkpoint and sets the result
 *
 * \param[inout] DMData Pointer to the Deadline Monitoring data shared with the Start Checkpoint
 * \param[inout] CPData Pointer to the checkpoint data of the Start Checkpoint
 * \param[in] CPDeadlineCfg Pointer to the Deadline Supervision Configuration
 *
 * \return Success of operation
 * \retval E_OK Deadline supervision of this Checkpoint was ok
 * \retval E_NOT_OK Deadline supervision of this Checkpoint failed */
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_StopCheckpointDeadline
(
  P2VAR(WdgM_EB_DMDataType,AUTOMATIC,WDGM_VAR) DMData,
  P2VAR(WdgM_EB_CPDataType,AUTOMATIC,WDGM_VAR) CPData,
  P2CONST(WdgM_EB_CPDeadlineCfgType,AUTOMATIC,WDGM_CONST) CPDeadlineCfg
);

/** \brief checks a deadline violation of this CP during main function execution
 *
 * \pre Parameter \a CPDeadlineCfg points to a valid Deadline Supervision Configuration
 *
 * \param[in] CPId checkpoint id
 * \param[in] CPDeadlineCfg Pointer to Deadline Supervision Configuration
 *
 * \return Success of operation
 * \retval WDGM_EB_RESULT_CORRECT no deadline violation detected
 * \retval WDGM_EB_RESULT_INCORRECT deadline violation detected */
static FUNC(WdgM_EB_ResultType, WDGM_CODE) WdgM_EB_CheckDeadlineViolation
(
  WdgM_EB_CPIdType CPId,
  P2CONST(WdgM_EB_CPDeadlineCfgType,AUTOMATIC,WDGM_CONST) CPDeadlineCfg
);
#endif

/** \brief performs alive supervision of a checkpoint
 *
 * \pre Alive supervision is configured for IntSEId/CPId
 *      in the actual Watchdog Manager Mode
 *
 * \param[in] IntSEId internal SE Id
 * \param[in] CPId checkpoint id
 *
 * \return Success of operation
 * \retval E_OK supervision was ok
 * \retval E_NOT_OK supervision failed */
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_CheckpointReachedAlive
(
  WdgM_EB_SEIdType IntSEId,
  WdgM_EB_CPIdType CPId
);

#if (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_ON)
/** \brief checks the list of predecessors of a checkpoint
 *
 * \param[in] CPLogicalCfg configuration to the logical supervision
 * \param[in] IntSEId the SE id of the last called Checkpoint of this Supervision Graph
 * \param[in] CPid the CP id of the last called Checkpoint of this Supervision Graph
 *
 * \return Success of operation
 * \retval E_OK the checkpoint is in the list of allowed checkpoints
 *              and the status of the predecessor Checkpoint is successful
 * \retval E_NOT_OK the Checkpoint is not allowed or the status
 *         of the predecessor Checkpoint was failed */
static FUNC (Std_ReturnType, WDGM_CODE) WdgM_EB_CheckPredCP
(
  P2CONST(WdgM_EB_CPLogicalCfgType,AUTOMATIC,WDGM_CONST) CPLogicalCfg,
  WdgM_EB_SEIdType IntSEId,
  WdgM_EB_CPIdType CPId
);
#endif

/** \brief determines the result of alive supervision and reset alive counter data
 *
 * \param[in] IntSEId the internal SE id
 * \param[in] CPId the checkpoint to check
 * \param[in] IntRamIdx to internal RAM data for this checkpoint
 * \param[in] CPAliveCfg to Checkpoint Alive Config for actual mode
 * \retval WDGM_EB_RESULT_CORRECT supervision was ok
 * \retval WDGM_EB_RESULT_INCORRECT supervision failed */
static FUNC(WdgM_EB_ResultType, WDGM_CODE) WdgM_EB_DetermineAliveResult
(
  WdgM_EB_SEIdType IntSEId,
  WdgM_EB_CPIdType CPId,
  WdgM_EB_IntRamIdxType IntRamIdx,
  P2CONST(WdgM_EB_CPAliveCfgType,AUTOMATIC,WDGM_CONST) CPAliveCfg
);

#if (WDGM_EB_DEADLINE_SUPERVISION_ENABLED == STD_ON)
/** \brief determines the result of deadline supervision (no side-effects on any variables)
 *
 * \param[in] IntSEId
 * \param[in] CPId
 * \retval WDGM_EB_RESULT_CORRECT supervision was ok
 * \retval WDGM_EB_RESULT_INCORRECT supervision failed */
static FUNC(WdgM_EB_ResultType, WDGM_CODE) WdgM_EB_DetermineDeadlineResult
(
  WdgM_EB_SEIdType IntSEId,
  WdgM_EB_CPIdType CPId
);
#endif

#if (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_ON)
/** \brief determines the result of logical supervision (no side-effects on any variables)
 *
 * \param[in] IntSEId
 * \param[in] CPId
 * \retval WDGM_EB_RESULT_CORRECT supervision was ok
 * \retval WDGM_EB_RESULT_INCORRECT supervision failed */
static FUNC(WdgM_EB_ResultType, WDGM_CODE) WdgM_EB_DetermineLogicalResult
(
  WdgM_EB_SEIdType IntSEId,
  WdgM_EB_CPIdType CPId
);
#endif

/** \brief determines the result of all configured supervisions
 *
 * \param[in] CoreInstance  Core instance specific data */
static FUNC(void, WDGM_CODE) WdgM_EB_SetLocalStatus
(
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
);
/** \brief sets the LocalStatus from the supervision results
 *
 * \param[in] IntSEId the internal SE id
 * \param[in] IntCoreSEId the internal core SE id
 * \param[in] AliveResult result of alive supervision
 * \param[in] DeadlineResult result of deadline monitoring
 * \param[in] LogicalResult result of logical supervision
 * \param[in] CoreInstance  Core instance specific data */
static FUNC(void, WDGM_CODE) WdgM_EB_SetSELocalStatus
(
  WdgM_EB_SEIdType IntSEId,
  WdgM_EB_SEIdType IntCoreSEId,
  WdgM_EB_ResultType AliveResult
#if (WDGM_EB_DEADLINE_SUPERVISION_ENABLED == STD_ON)
  , WdgM_EB_ResultType DeadlineResult
#endif
#if (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_ON)
  , WdgM_EB_ResultType LogicalResult
#endif
  , CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
);

/** \brief sets the GlobalStatus from all local status of the SEs
 *
 * \param[in] CoreInstance  Core instance specific data */
static FUNC(void, WDGM_CODE) WdgM_EB_SetGlobalStatus
(
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
);

/** \brief retrieves the new expected GlobalStatus from all local status of the SEs
 *
 * \param[in] CoreInstance  Core instance specific data */
static FUNC(WdgM_EB_GlobalStatusType, WDGM_CODE) WdgM_EB_GetNewGlobalStatus
(
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
);

/** \brief increment global supervision cycle counter
 *
 * \param[in] CoreInstance  Core instance specific data */
static FUNC(void, WDGM_CODE) WdgM_EB_UpdateSupervisionCycleCounter
(
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
);

#if (WDGM_EB_CHECK_MAINFUNCTION_TIMING == STD_ON)
/** \brief checks for a timing violation of the scheduled main function
 *
 * \param[in] CoreInstance  Core instance specific data */
static FUNC(void, WDGM_CODE) WdgM_EB_CheckMainFunctionTimingViolation
(
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
);
#endif

#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
/** \brief checks if all satellite cores switched to the current mode */
static FUNC(void, WDGM_CODE) WdgM_EB_CheckModeSwitchTimeFrame (void);
#endif

/** \brief checks consistency of alive data
 *
 * \pre parameter \a CPData is a valid pointer
 *
 * \param[in] CPData pointer to the checkpoint data
 *
 * \return Success of operation
 * \retval E_OK alive data is valid
 * \retval E_NOT_OK alive data is invalid */
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_CheckCPAliveCounterData
(
  P2CONST(WdgM_EB_CPDataType,AUTOMATIC,WDGM_VAR) CPData
);

/** \brief stores expired SEId data in first call only
 *
 * \pre parameter \a ExpiredSEID is a valid pointer
 *
 * \param[out] ExpiredSEID pointer to store data
 * \param[in] SEID the SEId to store */
static FUNC(void, WDGM_CODE) WdgM_EB_StoreExpiredSEID
(
  P2VAR(WdgM_EB_ExpiredSEIDType,AUTOMATIC,WDGM_VAR_POWER_ON_INIT) ExpiredSEID,
  WdgM_EB_SEIdType SEID
);

/** \brief retrieves the expired SEId data
 *
 * \param[in] UncheckedExpiredSEID the data stored about the last SEId
 * \param[out] ExpiredSEID pointer to store checked data
 *
 * \return Success of operation
 * \retval E_OK expired SEId data is valid
 * \retval E_NOT_OK expired SEId data is invalid */
static FUNC(Std_ReturnType,WDGM_CODE) WdgM_EB_GetCheckedExpiredSEID
(
  WdgM_EB_ExpiredSEIDType UncheckedExpiredSEID,
  P2VAR(WdgM_EB_SEIdType,AUTOMATIC,AUTOMATIC) ExpiredSEID
);

/** \brief increment alive counter by one
 *
 * \pre parameter \a CPData is a valid pointer
 * \pre shall be called from CheckpointReached API only
 *
 * \param[out] CPData pointer to store the data */
static FUNC(void, WDGM_CODE) WdgM_EB_IncrementCPAliveCounterData
(
  P2VAR(WdgM_EB_CPDataType,AUTOMATIC,WDGM_VAR) CPData
);

/** \brief reset alive counter data in internal Ram
 *
 * \pre shall be called from MainFunction only
 *
 * \param[in] IntRamIdx Index to internal RAM data for this checkpoint
 * \param[in] Alive Counter value which was read before to avoid race conditions
 * \param[in] CoreInstance  Core instance specific data */
static FUNC(void, WDGM_CODE) WdgM_EB_ResetCPAliveCounterData
(
  WdgM_EB_IntRamIdxType IntRamIdx,
  WdgM_EB_AliveCounterType OldAliveCounter,
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
);

/** \brief returns alive data if data is consistent
 *
 * \pre parameter \a CPData is a valid pointer
 *
 * \param[in] CPData pointer to the "raw" alive data
 * \param[in] IntRamIdx Index to internal RAM data for this checkpoint
 * \param[in] CoreInstance  Core instance specific data
 * \param[out] CPAliveCounter pointer to the checked AliveCounter value
 *                            (data is always updated independent of return value)
 *
 * \return Success of operation
 * \retval E_OK alive data is valid
 * \retval E_NOT_OK alive data is invalid */
static FUNC(Std_ReturnType,WDGM_CODE) WdgM_EB_GetCheckedCPAliveCounterData
(
  P2CONST(WdgM_EB_CPDataType,AUTOMATIC,WDGM_VAR) CPData,
  WdgM_EB_IntRamIdxType IntRamIdx,
  P2VAR(WdgM_EB_AliveCounterType,AUTOMATIC,AUTOMATIC) CPAliveCounter,
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
);

#if (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_ON)
/** \brief stores logical supervision data
 *
 * \pre parameter \a CPData is a valid pointer
 *
 * \param[out] CPData pointer to store the data
 * \param[in] LogicalResult the data to store */
static FUNC(void, WDGM_CODE) WdgM_EB_StoreCPLogicalData
(
  P2VAR(WdgM_EB_CPDataType,AUTOMATIC,WDGM_VAR) CPData,
  WdgM_EB_ResultType LogicalResult
);

/** \brief retrieves logical supervision result data
 *
 * \pre parameter \a CPData is a valid pointer
 *
 * \param[in] CPData pointer to the checkpoint data
 * \param[out] CPLogicalResult pointer to the checked logical supervision result
 *
 * \return Success of operation
 * \retval E_OK logical data is valid
 * \retval E_NOT_OK logical data is invalid */
static FUNC(WdgM_EB_ResultType,WDGM_CODE) WdgM_EB_GetCheckedCPLogicalData
(
  P2CONST(WdgM_EB_CPDataType,AUTOMATIC,WDGM_VAR) CPData,
  P2VAR(WdgM_EB_ResultType,AUTOMATIC,AUTOMATIC) CPLogicalResult
);

/** \brief stores graph data (active flag is set last)
 *
 * \pre parameter \a GraphData is a valid pointer
 *
 * \param[out] GraphData pointer to store the data
 * \param[in] IntSEId the SE id to store
 * \param[in] CPid the CP id to store
 * \param[in] IsActive the data to store */
static FUNC(void, WDGM_CODE) WdgM_EB_StoreGraphData
(
  P2VAR(WdgM_EB_GraphDataType,AUTOMATIC,WDGM_VAR) GraphData,
  WdgM_EB_SEIdType IntSEId,
  WdgM_EB_CPIdType CPId,
  boolean IsActive
);

/** \brief  Retrieve checked values of Graph data (active flag is read first)
 *
 * \pre parameter \a GraphData is a valid pointer
 *
 * \param[in] GraphData pointer to the graph to check
 * \param[out] LastIntSEId pointer to data for Supervised Entity Id to write to
 *                         of last reached Checkpoint data
 * \param[out] LastCPId pointer to data for Checkpoint Id  to write to
                        of last reached Checkpoint
 * \param[out] ActiveFlag pointer to data where value of active flag to write to
 *
 * \return Success of operation
 * \retval E_OK graph data is valid
 * \retval E_NOT_OK graph data is invalid */
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_GetCheckedGraphData
(
  P2CONST(WdgM_EB_GraphDataType,AUTOMATIC,WDGM_VAR) GraphData,
  P2VAR(WdgM_EB_SEIdType,AUTOMATIC,AUTOMATIC) LastIntSEId,
  P2VAR(WdgM_EB_CPIdType,AUTOMATIC,AUTOMATIC) LastCPId,
  P2VAR(boolean,AUTOMATIC,AUTOMATIC) ActiveFlag
);

#if (WDGM_EB_EXTERNAL_GRAPH_NUM > 0U)
/** \brief Retrieve checked values of Sequence Numbers of the Graphs
 *
 * \param[in] IntExtGraphDataSeqPtr pointer to the run-time data for each external graph
 * \param[in] IndexInGraphDataSeq index in the array to read from
 * \param[out] LastSeqNumber pointer to data where value of sequence number to write to
 * \return Success of operation
 * \retval E_OK logical data is valid
 * \retval E_NOT_OK logical data is invalid */
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_GetCheckedSeqNumber
(
  CONSTP2CONST(WdgM_EB_ExtGraphDataSeqArrayType,AUTOMATIC,WDGM_VAR) IntExtGraphDataSeqPtr,
  CONST(uint8,AUTOMATIC) IndexInGraphDataSeq,
  P2VAR(uint8,AUTOMATIC,AUTOMATIC) LastSeqNumber
);

/** \brief stores sequence number data
 *
 * \param[out] IntExtGraphDataSeqPtr pointer to store the data
 * \param[in] IndexInGraphDataSeq the index where to store in the array
 * \param[in] SeqNumber the data to store */
static FUNC(void, WDGM_CODE) WdgM_EB_StoreSeqNumber
(
  P2VAR(WdgM_EB_ExtGraphDataSeqArrayType,AUTOMATIC,WDGM_VAR) IntExtGraphDataSeqPtr,
  CONST(uint8,AUTOMATIC) IndexInGraphDataSeq,
  CONST(uint8,AUTOMATIC) SeqNumber
);

/** \brief reset the run-time data for each external graph
 *
 * \param[in] IntExtGraphDataSeqPtr pointer to the run-time data for each external graph
 * \param[in] NumOfSEperGraphId number of SE mapped per graph
 */

static FUNC(void, WDGM_CODE) WdgM_EB_ResetExtGraphDataSeq
(
  P2VAR(WdgM_EB_ExtGraphDataSeqArrayType,AUTOMATIC,WDGM_CONST) IntExtGraphDataSeqPtr,
  uint8 NumOfSEperGraphId
);
#endif /* if (WDGM_EB_EXTERNAL_GRAPH_NUM > 0U) */
#endif


#if (WDGM_EB_DEADLINE_SUPERVISION_ENABLED == STD_ON)
/** \brief stores deadline data
 *
 * \pre parameter \a CPData is a valid pointer
 *
 * \param[out] CPData pointer to store the data
 * \param[in] DeadlineResult the data to store */
static FUNC(void, WDGM_CODE) WdgM_EB_StoreCPDeadlineData
(
  P2VAR(WdgM_EB_CPDataType,AUTOMATIC,WDGM_VAR) CPData,
  WdgM_EB_ResultType DeadlineResult
);

/** \brief retrieves deadline result data
 *
 * \pre parameter \a CPData is a valid pointer
 *
 * \prarm[in] CPData pointer to the checkpoint data
 * \prarm[out] CPDeadlineResult pointer to the checked deadline monitoring result
 *
 * \return Success of operation
 * \retval E_OK deadline data is valid
 * \retval E_NOT_OK deadline data is invalid */
static FUNC(Std_ReturnType,WDGM_CODE) WdgM_EB_GetCheckedCPDeadlineData
(
  P2CONST(WdgM_EB_CPDataType,AUTOMATIC,WDGM_VAR) CPData,
  P2VAR(WdgM_EB_ResultType,AUTOMATIC,AUTOMATIC) CPDeadlineResult
);

/** \brief stores the active flag of Deadline Monitoring  (active flag is set last)
 *
 * \pre parameter \a DMData is a valid pointer
 *
 * \param[out] DMData pointer to store the data
 * \param[in] TimeStamp Pointer to time stamp of Start Checkpoint
 *                      that shall be written to the DM data
 * \param[in] MaxCycles Maximum allowed main function cycles until deadline violation that
 *                      shall be written to the DM data
 * \param[in] IsActive Active flag that shall be written to the DM data */
static FUNC(void, WDGM_CODE) WdgM_EB_StoreDMData
(
  P2VAR(WdgM_EB_DMDataType,AUTOMATIC,WDGM_VAR) DMData,
  P2CONST(WdgM_EB_TimestampType,AUTOMATIC,AUTOMATIC) TimeStamp,
  uint16 MaxCycles,
  boolean IsActive
);

/** \brief decrements the MaxCycles value of Deadline Monitoring if active
 *
 * \pre parameter \a DMData is a valid pointer of a Start Checkpoint
 *
 * \param[inout] DMData pointer to store the data
 *
 * \return Success of operation
 * \retval E_OK MaxCycles was successfully decremented or Deadline Monitoring is inactive
 * \retval E_NOT_OK MaxCycles equals 0 (therefore cannot be decremented) or data is invalid */
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_DecMaxCyclesDMData
(
  P2VAR(WdgM_EB_DMDataType,AUTOMATIC,WDGM_VAR) DMData
);

/** \brief Retrieve checked values of Deadline Monitoring data (active flag is read first)
 *
 * \pre parameter \a DMData is a valid pointer
 *
 * \param[in] DMData pointer to the Deadline Monitoring data to check
 * \param[out] TimeStamp pointer to data for retrieving time stamp of the Start Checkpoint
 * \param[out] MaxCycles pointer to data for retrieving the
 *                       maximum allowed main function cycles until deadline violation
 * \param[out] ActiveFlag pointer to the data for retrieving whether or not Deadline
 *                        Monitoring is active for the given DM Data
 *
 * \return Success of operation
 * \retval E_OK DM data is valid
 * \retval E_NOT_OK DM data is invalid */
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_GetCheckedDMData
(
  P2CONST(WdgM_EB_DMDataType,AUTOMATIC,WDGM_VAR) DMData,
  P2VAR(WdgM_EB_TimestampType,AUTOMATIC,AUTOMATIC) TimeStamp,
  P2VAR(uint16,AUTOMATIC,AUTOMATIC) MaxCycles,
  P2VAR(boolean,AUTOMATIC,AUTOMATIC) ActiveFlag
);
#endif

/** \brief converts an external SEId to an internal SEId
 *
 * \param[in] SEId the external SEId to search for
 * \param[out] IntSEId the internal SEId
 * \retval E_OK a valid IntSEId could be found
 * \retval E_NOT_OK a valid IntSEId could not be found */
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_SEId2IntSEId
(
  WdgM_SupervisedEntityIdType SEId,
  P2VAR(WdgM_EB_SEIdType,AUTOMATIC,AUTOMATIC) IntSEId
);

/** \brief checks if the Supervised Entity is active in this mode and resets the Supervision Data
 *
 * \param[in] NewMode the mode to search against
 * \param[in] SECfg Pointer to Supervised Entity Configuration
 * \param[in] IsNormalOperation Whether or not the
 *                Supervision Data for the New WdgM mode shall be reset
 *                as specified (TRUE)
 *                or shall always be reset (FALSE - during initialization only)
 * \param[in] CoreInstance  Core instance specific data
 *
 * \retval TRUE the IntSEId is active
 * \retval FALSE the IntSEId is not active */
static FUNC(boolean, WDGM_CODE) WdgM_EB_SEIsActiveResetData
(
  WdgM_ModeType NewMode,
  P2CONST(WdgM_EB_SECfgType,AUTOMATIC,WDGM_CONST) SECfg,
  boolean IsNormalOperation,
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
);

#if ((WDGM_EB_DEADLINE_SUPERVISION_ENABLED == STD_ON) || \
     (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_ON))

/** \brief resets all SE related to logical supervision and
 *   deadline supervision data that is stored inside SEs
 *
 * \param[in] IntSEId the internal SEId to reset
 */
static FUNC(void, WDGM_CODE) WdgM_EB_SEResetData
(
  WdgM_EB_SEIdType IntSEId
);

#endif

/** \brief resets all SE related to alive supervision data that is stored inside SEs
 *
 * \param[in] IntSEId the internal SEId to reset
 * \param[in] IntRamIdxBase Index to internal RAM data for this checkpoint
 * \param[in] InternalRAMOnly define whether or not alive counter data shall be
 *                            reset in both the internal RAM and the CPData or only in
 *                            the internal RAM
 *                            FALSE ... then both the double-inverse alive counter value within the
 *                                      CPData and the internal RAM data is correctly reset
 *                            TRUE .... then only internal RAM data is reset
 * \param[in] CoreInstance  Core instance specific data
 */
static FUNC(void, WDGM_CODE) WdgM_EB_SEResetCPAliveData
(
  WdgM_EB_SEIdType IntSEId,
  WdgM_EB_IntRamIdxType IntRamIdxBase,
  boolean InternalRAMOnly,
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
);

/** \brief Function for updating the watchdog trigger conditions
 *
 * \pre The global supervision status does not equal
 *      WDGM_GLOBAL_STATUS_DEACTIVATED.
 *
 * \param[in] CoreInstance  Core instance specific data
 */
static FUNC(void, WDGM_CODE) WdgM_EB_UpdateTriggerConditions
(
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
);

#if (WDGM_EB_CHECK_CALLERID == STD_ON)
/** \brief Function for validating the passed caller ID
 *
 * \param CallerID Caller ID that shall be checked
 * \return validity of the Caller ID
 * \retval TRUE     Caller ID is valid
 * \retval FALSE    Caller ID is invalid
 */
static FUNC(boolean, WDGM_CODE) WdgM_EB_CheckCallerID
(
  uint16 CallerID
);
#endif

/** \brief Check and switch to Init State if required
 *
 * \param[in] CoreInstance  Core instance specific data
 * \return Success of request operation
 * \retval TRUE  A requested initialization was successfully performed
 * \retval FALSE Either no init was requested, or a requested init failed
 */
static FUNC(boolean, WDGM_CODE) WdgM_EB_CheckInitRequest
(
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
);

/** \brief Check and switch to DeInit State if required
 *
 * \param[in] CoreInstance  Core instance specific data
 *
 * \return Success of request operation
 * \retval TRUE  A requested de-initialization was successfully performed
 * \retval FALSE Either no de-init was requested, or a requested de-init failed
 */
static FUNC(boolean, WDGM_CODE) WdgM_EB_CheckDeinitRequest
(
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
);

#if (WDGM_EB_ENABLE_SETMODE_SYNCHRON != STD_ON)
/** \brief Check and switch WdgM mode if required
 *
 * \param[in] CoreInstance  Core instance specific data
 *
 * \return Success of request operation
 * \retval TRUE  A requested mode switch was successfully performed
 * \retval FALSE Either no mode switch was requested, or a requested mode switch failed
 */
static FUNC(boolean, WDGM_CODE) WdgM_EB_CheckModeSwitchRequest
(
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
);

#endif

#if (WDGM_EB_ENABLE_SETMODE_SYNCHRON == STD_ON)
/** \brief Check and switch WdgM mode if required only
*          when the WdgM_SetMode is enabled to be synchronously
*
* \param[in] CoreInstance  Core instance specific data
*
* \return Success of request operation
* \retval E_OK  A requested mode switch was successfully performed
* \retval E_NOT_OK  Requested mode switch failed
* \retval WDGM_EB_E_SAME_MODE  Mode switch was requested with the same mode
*/
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_CheckModeSwitchRequestSynchron
(
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
);
#endif

#define WDGM_STOP_SEC_CODE_ASIL_D
#include <WdgM_MemMap.h>

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

#if((WDGM_EB_MULTICORE_ENABLED == STD_ON) || (WDGM_EB_ENABLE_SETMODE_SYNCHRON == STD_ON))
#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_START_SEC_VAR_INIT_8BIT
#else
#define WDGM_START_SEC_VAR_INIT_ASIL_D_LOCAL_8
#endif
#include <WdgM_MemMap.h>

#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)

/** \brief The WdgM master instance check if all the WdgM satellite instances had finished
  * the mode switch in the required time frame */
/* Deviation MISRAC2012-1 */
static VAR(uint8, WDGM_VAR) WdgM_EB_MasterWaitSlavesModeSwitch = 0U;

#endif

#if(WDGM_EB_ENABLE_SETMODE_SYNCHRON == STD_ON)

/** \brief Indicates a disable of the alive supervision until the next main function.
  * Used only for the WdgM_SetMode is executed synchronously. */
static VAR(boolean, WDGM_VAR) WdgM_EB_TmpDisableAliveSupv = FALSE;

#endif

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_STOP_SEC_VAR_INIT_8BIT
#else
#define WDGM_STOP_SEC_VAR_INIT_ASIL_D_LOCAL_8
#endif
#include <WdgM_MemMap.h>

#endif

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_START_SEC_VAR_CLEARED_16BIT
#else
#define WDGM_START_SEC_VAR_CLEARED_ASIL_D_16
#endif
#include <WdgM_MemMap.h>

/* !LINKSTO WDGM.ASR40.WDGM085.4,1 */
/** \brief WdgM internal counter for supervision cycles spent in state EXPIRED */
/* Deviation MISRAC2012-1 */
static VAR(uint16, WDGM_VAR) WdgM_EB_ExpiredSupervisionCycleCnt;

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_STOP_SEC_VAR_CLEARED_16BIT
#else
#define WDGM_STOP_SEC_VAR_CLEARED_ASIL_D_16
#endif
#include <WdgM_MemMap.h>

/*==================[external function definitions]==============================================*/

#define WDGM_START_SEC_CODE_ASIL_D
#include <WdgM_MemMap.h>

/*------------------[WdgM_Init]------------------------------------------------------------------*/
/* This Service is only provided, if callout concept is not used */
/* !LINKSTO WDGM.EB.TIMEPM.WDGM020431,1 */
/* !LINKSTO WDGM.EB.TIMEPM.WDGM020405,1 */
#if (WDGM_EB_EXT_GET_EXPECTED_INIT_STATE_ENABLED == STD_OFF)
FUNC(void, WDGM_CODE) WdgM_Init
(
  P2CONST(WdgM_ConfigType, AUTOMATIC, WDGM_APPL_CONST) ConfigPtr
)
{
  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_INIT_ENTRY(ConfigPtr);

  /* pointer is ignored, we use only compile time config */
  /* !LINKSTO WDGM.EB.WdgMInit,1 */
  WDGM_EB_PARAM_UNUSED(ConfigPtr);

  /* request a WdgM initialization for next main function cycle */
  /* !LINKSTO WDGM.EB.TIMEPM.WDGM020403,1 */
  WDGM_EB_ATOMIC_STORE(WdgM_EB_ExpectedInitState, WDGM_EB_INIT_STATUS_INIT);
  WDGM_EB_ATOMIC_INIT(WdgM_EB_ExpectedWdgMModeId, WdgM_EB_InitialModeId);


  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_INIT_EXIT(ConfigPtr);
}
#endif

/*------------------[WdgM_DeInit]----------------------------------------------------------------*/
/* This Service is only provided, if callout concept is not used */
/* !LINKSTO WDGM.EB.TIMEPM.WDGM020436,1 */
/* !LINKSTO WDGM.EB.TIMEPM.WDGM020407,1 */
#if (WDGM_EB_EXT_GET_EXPECTED_INIT_STATE_ENABLED == STD_OFF)
FUNC(void, WDGM_CODE) WdgM_DeInit
(
  void
)
{
  const WdgM_EB_CoreIdType CoreID = WDGM_EB_GET_CORE_ID();
#if (WDGM_EB_MULTICORE_ENABLED == STD_ON)
  const Atomic_value_t GlobalInitStatus = WDGM_EB_CHECK_INIT(WdgM_EB_GlobalInitStatus, CoreID);
  const Atomic_value_t ExpectedInitState = WDGM_EB_ATOMIC_LOAD(WdgM_EB_ExpectedInitState);
#endif
  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_DEINIT_ENTRY();
  /* !LINKSTO WDGM.EB.TIMEPM.WDGM020404,1 */
#if (WDGM_EB_MULTICORE_ENABLED == STD_ON)
  if ((WDGM_EB_GLOBAL_STATUS_DEINIT == GlobalInitStatus)
      && (WDGM_EB_INIT_STATUS_DEINIT == ExpectedInitState))
#else
    if ((WDGM_EB_GLOBAL_STATUS_DEINIT == (WDGM_EB_CHECK_INIT(WdgM_EB_GlobalInitStatus, CoreID)))
        && (WDGM_EB_INIT_STATUS_DEINIT == WDGM_EB_ATOMIC_LOAD(WdgM_EB_ExpectedInitState)))
#endif


  {
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO WDGM.ASR40.WDGM288,1,WDGM.ASR40.WDGM004.1,1 */
    WDGM_EB_DET(WDGM_SID_DEINIT, WDGM_E_NO_INIT);
#endif /* WDGM_DEV_ERROR_DETECT */
  }
  /* De-initialization is possible independent of active requests (e.g. Init request or
   * mode switch request) */
  /* !LINKSTO WDGM.EB.TIMEPM.WDGM020404,1,WDGM.EB.TIMEPM.WDGM020418,1,WDGM.EB.TIMEPM.WDGM020416,1 */
  else
  {
    /* request a WdgM de-initialization for next main function cycle */
    /* !LINKSTO WDGM.EB.TIMEPM.WDGM020464,1 */
    WDGM_EB_ATOMIC_STORE(WdgM_EB_ExpectedInitState, WDGM_EB_INIT_STATUS_DEINIT);
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_DEINIT_EXIT();
}
#endif

/*------------------[WdgM_GetVersionInfo]--------------------------------------------------------*/
#if (WDGM_VERSION_INFO_API == STD_ON)
/* !LINKSTO WDGM.ASR40.WDGM153,1 */
FUNC(void, WDGM_CODE) WdgM_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, WDGM_APPL_DATA) VersionInfo
)
{

  /* Pointer used to set to zero all bytes in the variable pointed by VersionInfo */
  P2VAR(uint8,AUTOMATIC,WDGM_APPL_DATA) LocalPtr = (P2VAR(uint8, AUTOMATIC, WDGM_APPL_DATA))VersionInfo;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_GETVERSIONINFO_ENTRY(VersionInfo);

#if (WDGM_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO WDGM.ASR40.WDGM256,1,WDGM.ASR40.WDGM004.5,1 */
  if (VersionInfo == NULL_PTR)
  {
    WDGM_EB_DET(WDGM_SID_GET_VERSION_INFO, WDGM_E_INV_POINTER);
  }
  else
#endif /* WDGM_DEV_ERROR_DETECT */
  {
    uint8_least i;

    /* Initialize complete structure with zeros.
     * This ensures compatibility with ASR 3.2 which additionally
     * contains the parameter instanceId which must be set to 0.
     */
    for (i=0U; i<(uint8_least)(sizeof(Std_VersionInfoType)); i++)
    {
      LocalPtr[i] = 0U;
    }

    /* !LINKSTO WDGM.ASR40.WDGM110,1 */
    VersionInfo->vendorID         = WDGM_VENDOR_ID;
    VersionInfo->moduleID         = WDGM_MODULE_ID;
    VersionInfo->sw_major_version = WDGM_SW_MAJOR_VERSION;
    VersionInfo->sw_minor_version = WDGM_SW_MINOR_VERSION;
    VersionInfo->sw_patch_version = WDGM_SW_PATCH_VERSION;
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_GETVERSIONINFO_EXIT(VersionInfo);
}
#endif

/*------------------[WdgM_SetMode]---------------------------------------------------------------*/
/* This Service is only provided, if callout concept is not used */
/* !LINKSTO WDGM.EB.TIMEPM.WDGM020401,1 */
#if (WDGM_EB_EXT_GET_EXPECTED_WDGM_MODE_ENABLED == STD_OFF)
#if (WDGM_EB_BSW_COMPATIBILITY == WDGM_EB_BSW_COMPATIBILITY_ASR40)
/* !LINKSTO WDGM.EB.TIMEPM.WDGM020425,1 */
FUNC(Std_ReturnType, WDGM_CODE) WdgM_SetMode
(
  WdgM_ModeType Mode,
  uint16 CallerID
)
#else
/* !LINKSTO WDGM.EB.ASR32.WDGM404,2 */
FUNC(Std_ReturnType, WDGM_CODE) WdgM_SetMode
(
  WdgM_ModeType Mode
)
#endif
{
#if (WDGM_EB_ENABLE_SETMODE_SYNCHRON == STD_ON)
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance = WdgM_GetCoreInstance();
#endif
  Std_ReturnType RetVal;
  const WdgM_EB_CoreIdType CoreID = WDGM_EB_GET_CORE_ID();
#if (WDGM_EB_MULTICORE_ENABLED == STD_ON)
  const Atomic_value_t GlobalInitStatus = WDGM_EB_CHECK_INIT(WdgM_EB_GlobalInitStatus, CoreID);
  const Atomic_value_t ExpectedInitState = WDGM_EB_ATOMIC_LOAD(WdgM_EB_ExpectedInitState);
  const Atomic_value_t GlobalStatus = WDGM_EB_ATOMIC_LOAD(WdgM_EB_GlobalStatus);
#endif

#if (WDGM_EB_BSW_COMPATIBILITY == WDGM_EB_BSW_COMPATIBILITY_ASR40)
  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_SETMODE_ENTRY(Mode,CallerID);
#else
  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_SETMODE_ENTRY(Mode,0U);
#endif

#if ((WDGM_EB_CHECK_CALLERID == STD_OFF) && \
    (WDGM_EB_BSW_COMPATIBILITY == WDGM_EB_BSW_COMPATIBILITY_ASR40))
  WDGM_EB_PARAM_UNUSED(CallerID);
#endif

#if (WDGM_DEV_ERROR_DETECT == STD_ON)
  /* checking initialized WdgM must incorporate the case if
   * SetMode is called between WdgM_Init and MainFunction
   * (in case no callouts are used for getting init state) */
  /* !LINKSTO WDGM.EB.TIMEPM.WDGM020459,1 */
#if (WDGM_EB_MULTICORE_ENABLED == STD_ON)
  if ((WDGM_EB_GLOBAL_STATUS_DEINIT != GlobalInitStatus)
      && (WDGM_EB_INIT_STATUS_DEINIT == ExpectedInitState))
#else
    if ((WDGM_EB_GLOBAL_STATUS_DEINIT != WDGM_EB_CHECK_INIT(WdgM_EB_GlobalInitStatus, CoreID))
      && (WDGM_EB_INIT_STATUS_DEINIT == WDGM_EB_ATOMIC_LOAD(WdgM_EB_ExpectedInitState)))
#endif
  {
    /* !LINKSTO WDGM.EB.TIMEPM.WDGM020400,1 */
    WDGM_EB_DET(WDGM_SID_SET_MODE, WDGM_E_NO_INIT);
    RetVal = E_NOT_OK;
  }
  /* !LINKSTO WDGM.ASR40.WDGM020,1,WDGM.ASR40.WDGM004.3,1 */
  else if (Mode >= WdgM_EB_ModeCfgSize)
  {
    WDGM_EB_DET(WDGM_SID_SET_MODE, WDGM_E_PARAM_MODE);
    RetVal = E_NOT_OK;
  }
  else
  /* check valid Caller ID */
#if (WDGM_EB_CHECK_CALLERID == STD_ON)
  /* !LINKSTO WDGM.ASR40.WDGM245,1 */
  if (WdgM_EB_CheckCallerID(CallerID) == FALSE)
  {
    RetVal = E_NOT_OK;
  }
  else
#endif
#endif
  {
#if (WDGM_EB_ENABLE_SETMODE_SYNCHRON == STD_ON)
      /* Check if WdgM is initialized. WdgM_SetMode (with sync activated) shall not be
     * called until the initialization is finished completely. This means that the
     * initialization  done on the WdgM_MainFunction should have been finished
     * (WdgM_EB_GlobalInitStatus != WDGM_EB_GLOBAL_STATUS_DEINIT)*/
    if (WDGM_EB_GLOBAL_STATUS_DEINIT == WDGM_EB_CHECK_INIT(WdgM_EB_GlobalInitStatus, CoreID))
    /* !LINKSTO WDGM.EB.TIMEPM.WDGM020457,1,WDGM.ASR40.WDGM021,1,WDGM.ASR40.WDGM004.1,1 */
#elif (WDGM_EB_MULTICORE_ENABLED == STD_ON)
    if ((WDGM_EB_GLOBAL_STATUS_DEINIT == GlobalInitStatus)
        && (WDGM_EB_INIT_STATUS_INIT != ExpectedInitState))
#else
    if ((WDGM_EB_GLOBAL_STATUS_DEINIT == WdgM_EB_GlobalInitStatus)
        && (WDGM_EB_INIT_STATUS_INIT != WdgM_EB_ExpectedInitState))
#endif
    {
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
      /* !LINKSTO WDGM.EB.TIMEPM.WDGM020400,1 */
      WDGM_EB_DET(WDGM_SID_SET_MODE, WDGM_E_NO_INIT);
#endif
      RetVal = E_NOT_OK;
    }
    /* !LINKSTO WDGM.ASR40.WDGM316,1,WDGM.ASR40.WDGM145,1,WDGM.EB.TIMEPM.WDGM020458,1 */
#if (WDGM_EB_MULTICORE_ENABLED == STD_ON)
    else if (((WDGM_GLOBAL_STATUS_OK != GlobalStatus) &&
              (WDGM_GLOBAL_STATUS_FAILED != GlobalStatus)) &&
              (WDGM_EB_GLOBAL_STATUS_DEINIT != GlobalInitStatus))
#else
    else if (((WDGM_GLOBAL_STATUS_OK != (WDGM_EB_ATOMIC_LOAD(WdgM_EB_GlobalStatus))) &&
              (WDGM_GLOBAL_STATUS_FAILED != WDGM_EB_ATOMIC_LOAD(WdgM_EB_GlobalStatus))) &&
              (WDGM_EB_GLOBAL_STATUS_DEINIT != WDGM_EB_CHECK_INIT(WdgM_EB_GlobalInitStatus, CoreID)))
#endif
    {
      RetVal = E_NOT_OK;
    }
    else
    {
      /* request a WdgM mode switch for next main function cycle only
       * if WdgM is initialized and WdgM_EB_GlobalStatus equals OK, or
       * if WdgM is not initialized but an initialization request is active */
      /* !LINKSTO WDGM.EB.TIMEPM.WDGM020400,1,WDGM.EB.TIMEPM.WDGM020461,1 */
      WDGM_EB_ATOMIC_STORE(WdgM_EB_ExpectedWdgMModeId, Mode);

#if (WDGM_EB_ENABLE_SETMODE_SYNCHRON == STD_ON)
      /* perform a mode switch */
      /* !LINKSTO WDGM.EB.TIMEPM.WDGM020417,1,WDGM.EB.TIMEPM.WDGM020460,1 */

      switch(WdgM_EB_CheckModeSwitchRequestSynchron(CoreInstance))
      {
        case E_OK:
        {
          /* reset global counter for supervision cycles */
          CoreInstance->WdgM_EB_SupervisionCycleCnt = 0U;
          /* disable alive supervision until the next main function */
          WdgM_EB_TmpDisableAliveSupv = TRUE;
          /* !LINKSTO WDGM.EB.TIMEPM.WDGM020462,1 */
          RetVal = E_OK;
          break;
        }
        case WDGM_EB_E_SAME_MODE:
        {
          /* Is the same mode. */
          RetVal = E_OK;
          break;
        }
        default:
        {
          /* for other return values then E_OK and WDGM_EB_E_SAME_MODE
           * something went wrong when changing the mode,
           * return E_NOT_OK */
          RetVal = E_NOT_OK;
          break;
        }
      }
#else

      /* !LINKSTO WDGM.EB.TIMEPM.WDGM020462,1 */
      RetVal = E_OK;
#endif
    }
  }

#if (WDGM_EB_BSW_COMPATIBILITY == WDGM_EB_BSW_COMPATIBILITY_ASR40)
  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_SETMODE_EXIT(RetVal,Mode,CallerID);
#else
  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_SETMODE_EXIT(RetVal,Mode,0U);
#endif
  return RetVal;
}
#endif

/*------------------[WdgM_GetMode]---------------------------------------------------------------*/
/* !LINKSTO WDGM.ASR40.WDGM168,1 */
FUNC(Std_ReturnType, WDGM_CODE) WdgM_GetMode
(
  P2VAR(WdgM_ModeType, AUTOMATIC, WDGM_APPL_DATA) Mode
)
{
  Std_ReturnType RetVal;
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
  Atomic_value_t LocalMode;
#endif

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_GETMODE_ENTRY(Mode);

#if (WDGM_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO WDGM.ASR40.WDGM253,1,WDGM.ASR40.WDGM004.1,1 */
  if (WDGM_EB_CHECK_INIT(WdgM_EB_GlobalInitStatus, WDGM_EB_GET_CORE_ID()) == WDGM_EB_GLOBAL_STATUS_DEINIT)
  {
    WDGM_EB_DET(WDGM_SID_GET_MODE, WDGM_E_NO_INIT);
    RetVal = E_NOT_OK;
  }
  /* !LINKSTO WDGM.ASR40.WDGM254,1,WDGM.ASR40.WDGM004.5,1 */
  else if (Mode == NULL_PTR)
  {
    WDGM_EB_DET(WDGM_SID_GET_MODE, WDGM_E_INV_POINTER);
    RetVal = E_NOT_OK;
  }
  else
#endif
  {

#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
    /*read the current WdgM mode from the WdgM master instance */
    LocalMode = WDGM_EB_ATOMIC_LOAD(WdgM_EB_CoreSwitchMode[WDGM_EB_GET_CORE_ID()]);
    /* Atomic_value_t can be 32 bits or 64bits, but only the last 8bits are relevant for the mode,
    the rest are 0 */
    *Mode = (WdgM_ModeType)(LocalMode & (WdgM_ModeType)0xFFU);
#else
      /* !LINKSTO WDGM.ASR40.WDGM170,1 */
    *Mode = WdgM_GetCoreInstance()->WdgM_EB_CurrentModeId;
#endif
    RetVal = E_OK;
  }


  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_GETMODE_EXIT(RetVal,Mode);
  return RetVal;
}

#define WDGM_STOP_SEC_CODE_ASIL_D
#include <WdgM_MemMap.h>

#define WDGM_START_SEC_CODE
#include <WdgM_MemMap.h>

/*------------------[WdgM_CheckpointReached]-----------------------------------------------------*/
/* !LINKSTO WDGM.ASR40.WDGM263,1 */
FUNC(Std_ReturnType, WDGM_CODE) WdgM_CheckpointReached
(
  WdgM_SupervisedEntityIdType SEID,
  WdgM_CheckpointIdType CheckpointID
)
{
  Std_ReturnType RetVal;
  WdgM_EB_SEIdType IntSEId;
  WdgM_EB_CPIdType IntCPId = (WdgM_EB_CPIdType)CheckpointID;
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance = WdgM_GetCoreInstance();
  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_CHECKPOINTREACHED_ENTRY(SEID,CheckpointID);

  /* !LINKSTO WDGM.ASR40.WDGM278,1,WDGM.ASR40.WDGM004.4,1 */
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO WDGM.ASR40.WDGM279,1,WDGM.ASR40.WDGM004.1,1 */
  if (WDGM_EB_CHECK_INIT(WdgM_EB_GlobalInitStatus, CoreInstance->CoreID) == WDGM_EB_GLOBAL_STATUS_DEINIT)
  {
    WDGM_EB_DET(WDGM_SID_CHECKPOINT_REACHED, WDGM_E_NO_INIT);
    RetVal = E_NOT_OK;
  }
  /* convert SEId and check whether the SEID is valid */
  else if (WdgM_EB_SEId2IntSEId (SEID, &IntSEId) != E_OK)
  {
    WDGM_EB_DET(WDGM_SID_CHECKPOINT_REACHED, WDGM_E_PARAM_SEID);
    RetVal = E_NOT_OK;
  }
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
  /* check if this IntSEId belongs to this core */
  else if (WdgM_EB_SECfg[IntSEId].CoreID != CoreInstance->CoreID)
  {
    WDGM_EB_DET(WDGM_SID_CHECKPOINT_REACHED, WDGM_E_PARAM_WRONG_CORE_ID);
    RetVal = E_NOT_OK;
  }
#endif
  /* !LINKSTO WDGM.ASR40.WDGM284,1,WDGM.ASR40.WDGM004.7,1 */
  /* Note: A value greater than 255 will be detected during this check,
   * because the ids of the checkpoint configuration are zero-based and dense
   * (see WDGM.EB.WdgMCheckpointIDValues) and the number of Checkpoints
   * per Supervised Entity are limited to 256 (see WDGM.EB.NumberOfCheckpoints). */
  else if (IntCPId >= WdgM_EB_SECfg[IntSEId].NumOfCPs)
  {
    WDGM_EB_DET(WDGM_SID_CHECKPOINT_REACHED, WDGM_E_CPID);
    RetVal = E_NOT_OK;
  }
  else
#else
  /* convert SEId and check whether the SEID is valid */
  (void)WdgM_EB_SEId2IntSEId (SEID, &IntSEId);
#endif
  {
    /* if the IntSEId is deactivated: don't perform supervision */
    /* !LINKSTO WDGM.ASR40.WDGM208,1 */
    if (WDGM_EB_ATOMIC_LOAD(CoreInstance->WdgM_EB_SELocalStatus[IntSEId])
      == WDGM_LOCAL_STATUS_DEACTIVATED)
    {
      /* !LINKSTO WDGM.ASR40.WDGM319,1,WDGM.ASR40.WDGM004.10,1 */
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
      WDGM_EB_DET(WDGM_SID_CHECKPOINT_REACHED, WDGM_E_SEDEACTIVATED);
#endif
      RetVal = E_NOT_OK;
    }
    else
    {
      boolean CPFailed = FALSE;
      CONSTP2CONST(WdgM_EB_SECfgType,AUTOMATIC,WDGM_CONST) SECfg =
          &(WdgM_EB_SECfg[IntSEId]);
      CONSTP2CONST(WdgM_EB_CPCfgType,AUTOMATIC,WDGM_CONST) CPCfg =
          &(SECfg->CPCfg[IntCPId]);

      {
        CONSTP2CONST(WdgM_EB_CPAliveCfgType,AUTOMATIC,WDGM_CONST) CPAliveCfg =
            CPCfg->AliveCfgPtr[CoreInstance->WdgM_EB_CurrentModeId];
        /* check if alive supervision is configured */
        /* !LINKSTO WDGM.ASR40.WDGM297,1 */
        if (CPAliveCfg != NULL_PTR)
        {
#if (WDGM_EB_ENABLE_SETMODE_SYNCHRON == STD_ON)
          if(WdgM_EB_TmpDisableAliveSupv == FALSE)
#endif
          {
            /* perform alive supervision */
            if (WdgM_EB_CheckpointReachedAlive (IntSEId, IntCPId) != E_OK)
            {
              CPFailed = TRUE;
            }
          }
        }
      }

#if (WDGM_EB_DEADLINE_SUPERVISION_ENABLED == STD_ON)
      {
        CONSTP2CONST(WdgM_EB_CPDeadlineCfgType,AUTOMATIC,WDGM_CONST) CPDeadlineCfg =
            CPCfg->DeadlineCfgPtr[CoreInstance->WdgM_EB_CurrentModeId];
        /* check if deadline supervision is configured */
        /* !LINKSTO WDGM.ASR40.WDGM297,1 */
        /* !LINKSTO WDGM.ASR40.WDGM299,1 */
        if (CPDeadlineCfg != NULL_PTR)
        {
          /* perform deadline monitoring */
          if (WdgM_EB_CheckpointReachedDeadline (IntSEId, IntCPId, CPDeadlineCfg) != E_OK)
          {
            CPFailed = TRUE;
          }
        }
      }
#endif

#if (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_ON)
      {
        CONSTP2CONST(WdgM_EB_CPLogicalCfgType,AUTOMATIC,WDGM_CONST) CPIntLogicalCfg =
            CPCfg->IntLogicalCfgPtr[CoreInstance->WdgM_EB_CurrentModeId];
#if (WDGM_EB_EXTERNAL_GRAPH_NUM > 0U)
        CONSTP2CONST(WdgM_EB_CPLogicalCfgType,AUTOMATIC,WDGM_CONST) CPExtLogicalCfg =
            CPCfg->ExtLogicalCfgPtr[CoreInstance->WdgM_EB_CurrentModeId];
#endif
        /* check if internal logical supervision is configured */
        /* !LINKSTO WDGM.ASR40.WDGM297,1 */
        if (CPIntLogicalCfg != NULL_PTR)
        {
          /* perform logical supervision */
          if (WdgM_EB_CheckpointReachedIntLogical (IntSEId, IntCPId, CPIntLogicalCfg) != E_OK)
          {
            CPFailed = TRUE;
          }
        }
#if (WDGM_EB_EXTERNAL_GRAPH_NUM > 0U)
        /* check if external logical supervision is configured */
        /* !LINKSTO WDGM.ASR40.WDGM297,1 */
        if (CPExtLogicalCfg != NULL_PTR)
        {
          /* perform logical supervision */
          if (WdgM_EB_CheckpointReachedExtLogical (IntSEId, IntCPId, CPExtLogicalCfg, CoreInstance) != E_OK)
          {
            CPFailed = TRUE;
          }
        }
#endif /* if (WDGM_EB_EXTERNAL_GRAPH_NUM > 0U) */
      }
#endif

      if (CPFailed)
      {
        RetVal = E_NOT_OK;
      }
      else
      {
        RetVal = E_OK;
      }
    }
  }


  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_CHECKPOINTREACHED_EXIT(RetVal,SEID,CheckpointID);
  return RetVal;
}

/*------------------[WdgM_UpdateAliveCounter]----------------------------------------------------*/
/* !LINKSTO WDGM.ASR40.WDGM155,1 */
FUNC(Std_ReturnType, WDGM_CODE) WdgM_UpdateAliveCounter
(
  WdgM_SupervisedEntityIdType SEID
)
{
  Std_ReturnType RetVal;
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance = WdgM_GetCoreInstance();

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_UPDATEALIVECOUNTER_ENTRY(SEID);

#if (WDGM_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO WDGM.ASR40.WDGM028,1,WDGM.ASR40.WDGM004.1,1 */
  if (WDGM_EB_CHECK_INIT(WdgM_EB_GlobalInitStatus, WDGM_EB_GET_CORE_ID()) == WDGM_EB_GLOBAL_STATUS_DEINIT)
  {
    WDGM_EB_DET(WDGM_SID_UPDATE_ALIVE_COUNTER, WDGM_E_NO_INIT);
    RetVal = E_NOT_OK;
  }
  else
#endif
  {
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
    /* !LINKSTO WDGM.EB.ASR32.WDGM112,1 */
#if !((WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON) || \
    (WDGM_EB_DEFAULT_ASR_SERVICE_API == WDGM_EB_SERVICE_API_ASR32))
    /* !LINKSTO WDGM.ASR40.WDGM290,1,WDGM.ASR40.WDGM004.8,1 */
    WDGM_EB_DET(WDGM_SID_UPDATE_ALIVE_COUNTER, WDGM_E_DEPRECATED);
#endif
#endif

    {
      WdgM_EB_SEIdType IntSEId;

      /* !LINKSTO WDGM.ASR40.WDGM027,1,WDGM.ASR40.WDGM004.4,1 */
      if (WdgM_EB_SEId2IntSEId (SEID, &IntSEId) != E_OK)
      {
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
        WDGM_EB_DET(WDGM_SID_UPDATE_ALIVE_COUNTER, WDGM_E_PARAM_SEID);
#endif
        RetVal = E_NOT_OK;
      }
      else
      {
        WdgM_EB_CPIdType CPId;
        uint16 NumOfAliveSupervisions = 0U;
        WdgM_EB_CPIdType FoundCPId = 0U;
        CONSTP2CONST(WdgM_EB_SECfgType,AUTOMATIC,WDGM_CONST) SECfg =
            &(WdgM_EB_SECfg[IntSEId]);

        /* go through all CPs and count the number of alive supervisions for this mode */
        for (CPId = 0U; CPId < SECfg->NumOfCPs; CPId++)
        {
          CONSTP2CONST(WdgM_EB_CPCfgType,AUTOMATIC,WDGM_CONST) CPCfg =
              &(SECfg->CPCfg[CPId]);

          if (CPCfg->AliveCfgPtr[CoreInstance->WdgM_EB_CurrentModeId] != NULL_PTR)
          {
            NumOfAliveSupervisions++;
            /* we simply take the last CP found with alive supervision */
            FoundCPId = CPId;
          }
        }

        /* no alive supervision for the SE in this mode: the call to this function is ignored */
        /* !LINKSTO WDGM.EB.UpdateAliveCounter,1 */
        if (NumOfAliveSupervisions == 0U)
        {
          RetVal = E_NOT_OK;
        }
        /* !LINKSTO WDGM.ASR40.WDGM318,1 */
        else if (NumOfAliveSupervisions == 1U)
        {
          RetVal = WdgM_CheckpointReached (SEID, FoundCPId);
        }
        /* !LINKSTO WDGM.ASR40.WDGM320,1,WDGM.ASR40.WDGM004.9,1 */
        else
        {
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
          WDGM_EB_DET(WDGM_SID_UPDATE_ALIVE_COUNTER, WDGM_E_AMBIGIOUS);
#endif
          RetVal = E_NOT_OK;
        }
      }
    }
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_UPDATEALIVECOUNTER_EXIT(RetVal,SEID);
  return RetVal;
}

#define WDGM_STOP_SEC_CODE
#include <WdgM_MemMap.h>

#define WDGM_START_SEC_CODE_ASIL_D
#include <WdgM_MemMap.h>

/*------------------[WdgM_GetLocalStatus]--------------------------------------------------------*/

/* !LINKSTO WDGM.ASR40.WDGM169,1 */
FUNC(Std_ReturnType, WDGM_CODE) WdgM_GetLocalStatus
(
  WdgM_SupervisedEntityIdType                      SEID,
  P2VAR(WdgM_LocalStatusType, AUTOMATIC, WDGM_APPL_DATA) Status
)
{
  Std_ReturnType RetVal;
  WdgM_EB_SEIdType IntSEId;
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance = WdgM_GetCoreInstance();
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
  WdgM_EB_LocalStatusType LocalStatus;
#endif

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_GETLOCALSTATUS_ENTRY(SEID,Status);

#if (WDGM_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO WDGM.ASR40.WDGM173,1,WDGM.ASR40.WDGM004.1,1 */
  if (WDGM_EB_CHECK_INIT(WdgM_EB_GlobalInitStatus, WDGM_EB_GET_CORE_ID()) == WDGM_EB_GLOBAL_STATUS_DEINIT)
  {
    WDGM_EB_DET(WDGM_SID_GET_LOCAL_STATUS, WDGM_E_NO_INIT);
    RetVal = E_NOT_OK;
  }
  /* !LINKSTO WDGM.ASR40.WDGM257,1,WDGM.ASR40.WDGM004.5,1 */
  else if (Status == NULL_PTR)
  {
    WDGM_EB_DET(WDGM_SID_GET_LOCAL_STATUS, WDGM_E_INV_POINTER);
    RetVal = E_NOT_OK;
  }
  else
#endif
  {
    /* convert an external SEId to an internal SEId and check validity */
    /* !LINKSTO WDGM.ASR40.WDGM172,1,WDGM.ASR40.WDGM004.4,1 */
    if (WdgM_EB_SEId2IntSEId (SEID, &IntSEId) != E_OK)
    {
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
      WDGM_EB_DET(WDGM_SID_GET_LOCAL_STATUS, WDGM_E_PARAM_SEID);
#endif
      RetVal = E_NOT_OK;
    }
    else
    {
      /* !LINKSTO WDGM.ASR40.WDGM171,1 */
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
      LocalStatus = WDGM_EB_ATOMIC_LOAD(CoreInstance->WdgM_EB_SELocalStatus[IntSEId]);
      /* Atomic_value_t can be 32 bits or 64bits, but only the last 8bits are relevant for the mode,
      the rest are 0 */
      *Status = (WdgM_LocalStatusType)(LocalStatus & (WdgM_LocalStatusType)0xFFU);
#else
      *Status = WDGM_EB_ATOMIC_LOAD(CoreInstance->WdgM_EB_SELocalStatus[IntSEId]);
#endif
      RetVal = E_OK;
    }
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_GETLOCALSTATUS_EXIT(RetVal,SEID,Status);
  return RetVal;
}

/*------------------[WdgM_GetGlobalStatus]-------------------------------------------------------*/

/* !LINKSTO WDGM.ASR40.WDGM175,1 */
FUNC(Std_ReturnType, WDGM_CODE) WdgM_GetGlobalStatus
(
  P2VAR(WdgM_GlobalStatusType, AUTOMATIC, WDGM_APPL_DATA) Status
)
{
  Std_ReturnType RetVal;
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
  WdgM_EB_GlobalStatusType LocalStatus;
#endif
  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_GETGLOBALSTATUS_ENTRY(Status);

#if (WDGM_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO WDGM.ASR40.WDGM176,1,WDGM.ASR40.WDGM004.1,1 */
  if (WDGM_EB_CHECK_INIT(WdgM_EB_GlobalInitStatus, WDGM_EB_GET_CORE_ID())== WDGM_EB_GLOBAL_STATUS_DEINIT)
  {
    WDGM_EB_DET(WDGM_SID_GET_GLOBAL_STATUS, WDGM_E_NO_INIT);
    RetVal = E_NOT_OK;
  }
  /* !LINKSTO WDGM.ASR40.WDGM258,1,WDGM.ASR40.WDGM004.5,1 */
  else if (Status == NULL_PTR)
  {
    WDGM_EB_DET(WDGM_SID_GET_GLOBAL_STATUS, WDGM_E_INV_POINTER);
    RetVal = E_NOT_OK;
  }
  else
#endif /* WDGM_DEV_ERROR_DETECT */
  {
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
    LocalStatus = WDGM_EB_ATOMIC_LOAD(WdgM_EB_GlobalStatus);
    /* Atomic_value_t can be 32 bits or 64bits, but only the last 8bits are relevant for the mode,
    the rest are 0 */
    *Status = (WdgM_GlobalStatusType)(LocalStatus & (WdgM_GlobalStatusType)0xFFU);
#else
    *Status = WDGM_EB_ATOMIC_LOAD(WdgM_EB_GlobalStatus);
#endif
    RetVal = E_OK;
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_GETGLOBALSTATUS_EXIT(RetVal,Status);
  return RetVal;
}

/*------------------[WdgM_PerformReset]----------------------------------------------------------*/

/* !LINKSTO WDGM.ASR40.WDGM264,1 */
FUNC(void, WDGM_CODE) WdgM_PerformReset(void)
{
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance = WdgM_GetCoreInstance();
  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_PERFORMRESET_ENTRY();

#if (WDGM_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO WDGM.ASR40.WDGM270,1,WDGM.ASR40.WDGM004.1,1  */
  if (WDGM_EB_CHECK_INIT(WdgM_EB_GlobalInitStatus, WDGM_EB_GET_CORE_ID()) == WDGM_EB_GLOBAL_STATUS_DEINIT)
  {
    WDGM_EB_DET(WDGM_SID_PERFORM_RESET, WDGM_E_NO_INIT);
  }
  else
#endif
  {
#if (WDGM_EB_EXT_IS_PERFORM_RESET_ENABLED == STD_ON)
    /* !LINKSTO WDGM.EB.TIMEPM.WDGM020451,1 */
    const Std_ReturnType PerformResetAuthorized = WDGM_EB_EXT_IS_PERFORM_RESET();
    if (PerformResetAuthorized != E_OK)
    {
      /* !LINKSTO WDGM.EB.TIMEPM.WDGM020449,1,WDGM.EB.TIMEPM.WDGM020452,1 */
      WDGM_EB_DET(WDGM_SID_PERFORM_RESET, WDGM_E_NOT_AUTHORIZED);
    }
    else
#endif
    {
      /* set the global state to STOPPED. As a consequence:
       * - UpdateTriggerConditions will set all TriggerConditions to zero
       * - the global supervision status is not considered anymore, therefore there is
       *   also no notification via callback/RTE
       * - no Det or Dem will be called in MainFunction due to global state stopped
       */
      /* !LINKSTO WDGM.ASR40.WDGM139,1,WdgM.EB.PerformReset1,1 */
      WDGM_EB_ATOMIC_STORE(WdgM_EB_GlobalStatus, WDGM_GLOBAL_STATUS_STOPPED);

      /* !LINKSTO WDGM.ASR40.WDGM133.1,1 */
#if (WDGM_IMMEDIATE_RESET == STD_ON)
      Mcu_PerformReset();
#endif

      /* update the trigger conditions for all configured watchdogs to zero */
      /* !LINKSTO WdgM.EB.PerformReset1,1 */
      WdgM_EB_UpdateTriggerConditions(CoreInstance);
    }
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_PERFORMRESET_EXIT();
}

/*------------------[WdgM_GetFirstExpiredSEID]---------------------------------------------------*/

/* !LINKSTO WDGM.ASR40.WDGM346,1 */
FUNC(Std_ReturnType, WDGM_CODE) WdgM_GetFirstExpiredSEID
(
  P2VAR(WdgM_SupervisedEntityIdType, AUTOMATIC, WDGM_APPL_DATA) SEID
)
{
  Std_ReturnType RetVal;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_GETFIRSTEXPIREDSEID_ENTRY(SEID);

  /* !LINKSTO WDGM.ASR40.WDGM348,1 */
  /* this function has no init check and no external dependencies (calls, etc.) */

#if (WDGM_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO WDGM.ASR40.WDGM347,1,WDGM.ASR40.WDGM004.5,1 */
  if (SEID == NULL_PTR)
  {
    WDGM_EB_DET(WDGM_SID_GET_FIRST_EXPIRED_SEID, WDGM_E_INV_POINTER);
    RetVal = E_NOT_OK;
  }
  else
#endif
  {
    WdgM_EB_SEIdType LocalSEId = (WdgM_EB_SEIdType)0U;
    WdgM_EB_ExpiredSEIDType ExpiredSEID = WDGM_EB_ATOMIC_LOAD(WdgM_EB_ExpiredSEID);

    /* !LINKSTO WDGM.ASR40.WDGM349,1 */
    RetVal = WdgM_EB_GetCheckedExpiredSEID (ExpiredSEID, &LocalSEId);
    *SEID = LocalSEId;
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_GETFIRSTEXPIREDSEID_EXIT(RetVal,SEID);
  return RetVal;
}


/*-----------------------------------------------------------------------------------------------
 * Scheduled functions
 *-----------------------------------------------------------------------------------------------*/

/*------------------[WdgM_MainFunction]----------------------------------------------------------*/

/* !LINKSTO WDGM.ASR40.WDGM159,1,WDGM.ASR40.WDGM060,1 */
FUNC(void, WDGM_CODE) WdgM_MainFunction(void)
{
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance = WdgM_GetCoreInstance();

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_MAINFUNCTION_ENTRY();

  /* !LINKSTO WDGM.EB.TIMEPM.WDGM020447,1 */
  /* Since MainFunction is not reentrant, check if an execution is active */
  if (CoreInstance->WdgM_EB_MainFunctionActive)
  {
    /* indicate violation of reentrancy */
    WDGM_EB_DET(WDGM_SID_MAIN_FUNCTION, WDGM_EB_E_REENTRANCY);
  }
  else
  {
    /* MainFunction is executing => active */
    CoreInstance->WdgM_EB_MainFunctionActive = TRUE;

    {
      /* Store actual init state (do not check for possible initialization
       * at end of MainFunction if WdgM is de-initialized in actual cycle). */
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
      const Atomic_value_t DoNotCheckInitRequest = WDGM_EB_CHECK_INIT(WdgM_EB_GlobalInitStatus, CoreInstance->CoreID);
#else
      const uint32 DoNotCheckInitRequest = WDGM_EB_CHECK_INIT(WdgM_EB_GlobalInitStatus, CoreInstance->CoreID);
#endif

      /* check whether WdgM shall be deinitialized */
      if (WDGM_EB_CHECK_INIT(WdgM_EB_GlobalInitStatus, CoreInstance->CoreID) != WDGM_EB_GLOBAL_STATUS_DEINIT)
      {
        /* !LINKSTO WDGM.EB.TIMEPM.WDGM020414,1,WDGM.EB.TIMEPM.WDGM020465,1 */
        /* Checking de-initialization at the beginning ensures
         * that supervision algorithm is not executed after successful de-initialization */
        (void)WdgM_EB_CheckDeinitRequest(CoreInstance);
      }

      /* !LINKSTO WDGM.EB.TIMEPM.WDGM020454,1 */
#if (WDGM_EB_CHECK_MAINFUNCTION_TIMING == STD_ON)
      /* check for timing violation of main function */
      WdgM_EB_CheckMainFunctionTimingViolation(CoreInstance);
#endif

      /* !LINKSTO WDGM.ASR40.WDGM063,1,WDGM.EB.TIMEPM.WDGM020420,1,WDGM.ASR40.WDGM292,1 */
      /* Perform supervision only if WdgM is initialized and not deactivated
       * Note: WdgM_EB_GlobalInitStatus != WDGM_EB_GLOBAL_STATUS_DEINIT implies that
       * WdgM_EB_GlobalStatus is set to some Status not equal to WDGM_GLOBAL_STATUS_DEACTIVATED */
      if (WDGM_EB_CHECK_INIT(WdgM_EB_GlobalInitStatus, CoreInstance->CoreID) != WDGM_EB_GLOBAL_STATUS_DEINIT)
      {
#if (WDGM_EB_ENABLE_SETMODE_SYNCHRON == STD_ON)
      /* after a change of mode is done,the supervision cycle counter
       * begins increment with the second main function */
        if(WdgM_EB_TmpDisableAliveSupv == FALSE)
#endif
        {
          /* increment supervision cycle counter */
          WdgM_EB_UpdateSupervisionCycleCounter(CoreInstance);
        }

        /* determine and set the local status of each SE */
        /* !LINKSTO WDGM.ASR40.WDGM325,1,WDGM.EB.MulticoreLocalStatus1,1 */
        WdgM_EB_SetLocalStatus(CoreInstance);

#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
         if(CoreInstance->CoreID == WDGM_EB_MASTER_INSTANCE)
#endif
         {
            /* determine and set the new global status */
            /* !LINKSTO WDGM.ASR40.WDGM214,1,WDGM.ASR40.WDGM326,1 */
            /* !LINKSTO WDGM.EB.MulticoreGlobalStatus,1 */
            WdgM_EB_SetGlobalStatus(CoreInstance);

            /* !LINKSTO WDGM.ASR40.WDGM292,1,WDGM.ASR40.WDGM327,1,WDGM.ASR40.WDGM328,1 */
            /* error handling is currently limited to triggering */
            WdgM_EB_UpdateTriggerConditions(CoreInstance);
         }
      }

      /* check whether WdgM shall be initialized only if WdgM was not initialized before */
      if (DoNotCheckInitRequest == WDGM_EB_GLOBAL_STATUS_DEINIT)
      {
        if (WdgM_EB_CheckInitRequest(CoreInstance))
        {
           /* reset global counter for supervision cycles */
           CoreInstance->WdgM_EB_SupervisionCycleCnt = 0U;
        }
      }

#if (WDGM_EB_ENABLE_SETMODE_SYNCHRON == STD_OFF)
      /* check whether WdgM shall perform a mode switch */
      /* !LINKSTO WDGM.EB.TIMEPM.WDGM020417,1,WDGM.EB.TIMEPM.WDGM020460,1 */
      /* checking the mode switch request at the end ensures that mode switch
       * is not performed if de-initialization request was active for the same main function */
      if (WDGM_EB_CHECK_INIT(WdgM_EB_GlobalInitStatus, CoreInstance->CoreID) != WDGM_EB_GLOBAL_STATUS_DEINIT)
      {
        if (WdgM_EB_CheckModeSwitchRequest(CoreInstance))
        {
           /* reset global counter for supervision cycles */
           CoreInstance->WdgM_EB_SupervisionCycleCnt = 0U;
        }
      }
#else
      if (FALSE != WdgM_EB_TmpDisableAliveSupv)
      {
        WdgM_EB_TmpDisableAliveSupv = FALSE;
      }
#endif
    }

#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
      if(CoreInstance->CoreID == WDGM_EB_MASTER_INSTANCE)
      {
        if(WDGM_EB_MASTER_WAIT_SLAVE_MODE_SWITCH != 0U)
        {
          /* !LINKSTO WDGM.EB.MulticoreSwitchMode.1,1 */
          /* checks if all satellite cores switched to the current mode */
          WdgM_EB_CheckModeSwitchTimeFrame();
        }
      }
#endif
    /* MainFunction is finished => inactive */
    CoreInstance->WdgM_EB_MainFunctionActive = FALSE;
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_MAINFUNCTION_EXIT();
}

/*==================[internal function definitions]==============================================*/

/*------------------[WdgM_EB_Init]---------------------------------------------------------------*/
/* !LINKSTO WDGM.EB.TIMEPM.WDGM020431,1 */
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_Init
(
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
)
{
  Std_ReturnType RetVal;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_INIT_ENTRY(CoreInstance);

  /* Initialize global mode variable to be available for GetMode API */
  CoreInstance->WdgM_EB_CurrentModeId = WdgM_EB_InitialModeId;

  /* !LINKSTO WDGM.EB.TIMEPM.WDGM020450,1 */
  /* !LINKSTO WDGM.EB.MulticoreInit.1,1 */
  /* Set the WdgM status to initialized (even if anything fails during initialization, the WdgM
   * shall keep initialized but with Global Supervision State WDGM_GLOBAL_STATUS_STOPPED).
   * Announce other WdgM core instances that the initialization on this WdgM
   * instance has been performed.
   * If the init fails for the master then a reset will be perform and there is no problem if
   * the satellites will be initialized. */
   WDGM_EB_ATOMIC_SET_BIT(WdgM_EB_GlobalInitStatus,CoreInstance->CoreID);

  /* first try to set mode to initial mode (this sets up supervision and triggering data and
   * resets SE and Supervision runtime data) */
  /* !LINKSTO WDGM.EB.TIMEPM.WDGM020409,1,WDGM.EB.TIMEPM.WDGM020410,1,WDGM.EB.TIMEPM.WDGM020412,1 */
  RetVal = WdgM_EB_SetMode(WdgM_EB_InitialModeId, FALSE, CoreInstance);

  if (RetVal == E_OK)
  {
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
    if(CoreInstance->CoreID == WDGM_EB_MASTER_INSTANCE)
#endif
    {
      /* initialize the SEId that was possibly saved as 'culprit' from last time
       * and reset that something had been stored */
      /* !LINKSTO WDGM.EB.TIMEPM.WDGM020434,1 */
      WDGM_EB_ATOMIC_INIT(WdgM_EB_ExpiredSEID,0U);
      /* !LINKSTO WDGM.ASR40.WDGM351,1 */
      WDGM_EB_ATOMIC_INIT(WdgM_EB_IsExpiredSEIdSet,FALSE);

      /* Set the Global supervision status to OK */
      /* !LINKSTO WDGM.EB.TIMEPM.WDGM020428,1 */
      WDGM_EB_ATOMIC_STORE(WdgM_EB_GlobalStatus, WDGM_GLOBAL_STATUS_OK);
      /* !LINKSTO WDGM.ASR40.WDGM198,1,WdgM.ASR43.SWS_WdgM_00198,1 */
      /* signal via callout and/or RTE */
      WDGM_EB_GLOBAL_MODE_SWITCH(WDGM_GLOBAL_STATUS_DEACTIVATED, WDGM_GLOBAL_STATUS_OK);

      /* !LINKSTO WDGM.ASR40.WDGM223,1,WDGM.EB.MulticoreTriggerWatchdog,1 */
      WdgM_EB_UpdateTriggerConditions(CoreInstance);
    }

#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
    WDGM_EB_ATOMIC_INIT(WdgM_EB_CoreSwitchMode[CoreInstance->CoreID], WdgM_EB_InitialModeId);
#endif

  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_INIT_EXIT(RetVal,CoreInstance);
  return RetVal;
}

/*------------------[WdgM_EB_DeInit]-------------------------------------------------------------*/
/* !LINKSTO WDGM.EB.TIMEPM.WDGM020436,1 */
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_DeInit
(
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
)
{
#if (WDGM_EB_DEINIT_MODE_ENABLED == STD_ON)
  Std_ReturnType RetVal;
#endif

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_DEINIT_ENTRY(CoreInstance);

#if (WDGM_EB_DEINIT_MODE_ENABLED == STD_ON)
  /* !LINKSTO WDGM.EB.WdgM_DeInit.1,1 */

  RetVal = WdgM_EB_SetMode(WdgM_EB_DeInitModeId, TRUE, CoreInstance);

  if (E_OK == RetVal)
  {
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
    if (CoreInstance->CoreID == WDGM_EB_MASTER_INSTANCE)
#endif
    {
      /* Global Supervision Status has been recomputed => update trigger conditions. */
      /* !LINKSTO WDGM.ASR40.WDGM223,1,WDGM.EB.MulticoreTriggerWatchdog,1 */
      WdgM_EB_UpdateTriggerConditions(CoreInstance);
    }
  }
#endif

#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
  if(CoreInstance->CoreID == WDGM_EB_MASTER_INSTANCE)
  {
    const WdgM_EB_GlobalStatusType LocalOldGlobalStatus = WDGM_EB_ATOMIC_LOAD(WdgM_EB_GlobalStatus);
    const WdgM_GlobalStatusType OldGlobalStatus = (WdgM_GlobalStatusType)(LocalOldGlobalStatus & (WdgM_GlobalStatusType)0xFFU);
#else
  {
    const WdgM_EB_GlobalStatusType OldGlobalStatus = WDGM_EB_ATOMIC_LOAD(WdgM_EB_GlobalStatus);
#endif

    /* Reset global status and flags independent of actual global state */
    /* !LINKSTO WDGM.EB.TIMEPM.WDGM020435,1 */
    WDGM_EB_ATOMIC_STORE(WdgM_EB_GlobalStatus, WDGM_GLOBAL_STATUS_DEACTIVATED);

    /* !LINKSTO WDGM.ASR40.WDGM198,1,WdgM.ASR43.SWS_WdgM_00198,1 */
    /* signal via callout and/or RTE */
    WDGM_EB_GLOBAL_MODE_SWITCH(OldGlobalStatus, WDGM_GLOBAL_STATUS_DEACTIVATED);
  }

  /* !LINKSTO WDGM.EB.MulticoreDeInit,1 */
  /* announce other WdgM core instances that the de-initialization on this WdgM
   * instance has been performed */
  WDGM_EB_ATOMIC_CLR_BIT(WdgM_EB_GlobalInitStatus,CoreInstance->CoreID);

  /* !LINKSTO WDGM.EB.Dbg4,1 */
#if (WDGM_EB_DEINIT_MODE_ENABLED == STD_ON)
  DBG_WDGM_EB_DEINIT_EXIT(RetVal,CoreInstance);
  return RetVal;
#else
  DBG_WDGM_EB_DEINIT_EXIT(E_OK,CoreInstance);
  return E_OK;
#endif
}

/*------------------[WdgM_EB_SetMode]------------------------------------------------------------*/
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_SetMode
(
  WdgM_ModeType Mode,
  boolean IsNormalOperation,
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
)
{
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
  const Atomic_value_t GlobalStatus = WDGM_EB_ATOMIC_LOAD(WdgM_EB_GlobalStatus);
#endif
  /* if multicore is enabled, for the satellite cores this function will always return E_OK */
  Std_ReturnType RetVal = E_OK;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_SETMODE_ENTRY(Mode,CoreInstance);

  /* !LINKSTO WDGM.ASR40.WDGM317,1 */
  /* the function doesn't change the global status or the expired reference counter */
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
  if(CoreInstance->CoreID == WDGM_EB_MASTER_INSTANCE)
#endif
  {
    /* !LINKSTO WDGM.ASR40.WDGM316,1,WDGM.ASR40.WDGM145,1,WDGM.EB.TIMEPM.WDGM020432,1 */
    /* Note: In case IsNormalOperation is set to TRUE, then Global / Local Status is ignored. */
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
    if (((WDGM_GLOBAL_STATUS_OK != GlobalStatus) &&
        (WDGM_GLOBAL_STATUS_FAILED != GlobalStatus)) &&
        (FALSE != IsNormalOperation))
#else
      if (((WDGM_GLOBAL_STATUS_OK != WDGM_EB_ATOMIC_LOAD(WdgM_EB_GlobalStatus)) &&
        (WDGM_GLOBAL_STATUS_FAILED != WDGM_EB_ATOMIC_LOAD(WdgM_EB_GlobalStatus))) &&
        (FALSE != IsNormalOperation))
#endif
    {
      RetVal = E_NOT_OK;
    }
  }

    if(E_OK == RetVal)
    {
      WdgM_EB_SEIdType IntSEId = 0U;
      WdgM_EB_SEIdType IntCoreSEId = 0U;
      WdgM_EB_IntRamIdxType IntRamIdxBase = 0U;

      /* go through all SEs and clear their runtime data */
#if (WDGM_EB_SE_NUM != 1U)
      for (IntSEId = 0U; IntSEId < WDGM_EB_SE_NUM; IntSEId++)
#endif
      {
        CONSTP2CONST(WdgM_EB_SECfgType,AUTOMATIC,WDGM_CONST) SECfg = &(WdgM_EB_SECfg[IntSEId]);
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
        if(CoreInstance->CoreID == SECfg->CoreID)
#endif
        {
#if (WDGM_EB_MULTICORE_ENABLED == STD_ON)
          CONSTP2VAR(Atomic_t,AUTOMATIC,AUTOMATIC) SEWData_SELocalStatus =
              &(CoreInstance->WdgM_EB_SELocalStatus[IntSEId]);
#else
          CONSTP2VAR(WdgM_LocalStatusType,AUTOMATIC,AUTOMATIC) SEWData_SELocalStatus =
              &(CoreInstance->WdgM_EB_SELocalStatus[IntSEId]);
#endif
          CONSTP2VAR(uint16,AUTOMATIC,AUTOMATIC) SEWData_FailedAliveSupervisionRefCycleCnt =
              &(CoreInstance->WdgM_EB_FailedAliveSupervisionRefCycleCnt[IntCoreSEId]);
          /* Get active state of SE in new mode and reset active flags if required */
          const boolean SEIsActive = WdgM_EB_SEIsActiveResetData (Mode, SECfg, IsNormalOperation, CoreInstance);
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
          const Atomic_value_t SELocalStatus = WDGM_EB_ATOMIC_LOAD(*SEWData_SELocalStatus);
#endif

          /* !LINKSTO WDGM.ASR40.WDGM182,1 */
          /* Local Status of SEs that are active in this mode is not touched */
          if (SEIsActive)
          {
            /* !LINKSTO WDGM.EB.TIMEPM.WDGM020421,1,WDGM.EB.TIMEPM.WDGM020426,1,WDGM.ASR40.WDGM209,1 */
            /* Note: In case IsNormalOperation is set to FALSE, then old Local Status is ignored. */
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
            if ((WDGM_LOCAL_STATUS_DEACTIVATED == SELocalStatus)
                || (FALSE == IsNormalOperation))
#else
              if ((WDGM_LOCAL_STATUS_DEACTIVATED == WDGM_EB_ATOMIC_LOAD(*SEWData_SELocalStatus))
                  || (FALSE == IsNormalOperation))
#endif
              {
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
                if(IsNormalOperation == FALSE)
                {
                  WDGM_EB_ATOMIC_INIT(*SEWData_SELocalStatus, WDGM_LOCAL_STATUS_OK);
                }
                else
#endif
                {
                  WDGM_EB_ATOMIC_STORE(*SEWData_SELocalStatus, WDGM_LOCAL_STATUS_OK);
                }
                /* !LINKSTO WDGM.ASR40.WDGM197,1,WdgM.ASR43.SWS_WdgM_00197,1 */
                /* status changes because of mode change, signal via callout and/or RTE */
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
                WDGM_EB_INDIVIDUAL_MODE_SWITCH(SECfg->SEId, WDGM_LOCAL_STATUS_DEACTIVATED,
                    WDGM_LOCAL_STATUS_OK, SECfg->CoreID);
#else
                WDGM_EB_INDIVIDUAL_MODE_SWITCH(SECfg->SEId, WDGM_LOCAL_STATUS_DEACTIVATED,
                    WDGM_LOCAL_STATUS_OK, 0U);
#endif
              }
          }
          else
          {
            /* !LINKSTO WDGM.ASR40.WDGM315,1,WDGM.EB.TIMEPM.WDGM020422,1,WDGM.EB.TIMEPM.WDGM020423,1,WDGM.ASR40.WDGM207,1,WDGM.ASR40.WDGM291,1 */
            /* set the local status to DEACTIVATED (depending on future global mode) */
            /* Note: In case IsNormalOperation is set to FALSE, then old Local Status is ignored. */
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
            if (((WDGM_LOCAL_STATUS_OK == SELocalStatus) ||
                (WDGM_LOCAL_STATUS_FAILED == SELocalStatus)) ||
                (FALSE == IsNormalOperation))
#else
            if (((WDGM_LOCAL_STATUS_OK == (WDGM_EB_ATOMIC_LOAD(*SEWData_SELocalStatus))) ||
                  (WDGM_LOCAL_STATUS_FAILED == WDGM_EB_ATOMIC_LOAD(*SEWData_SELocalStatus))) ||
                  (FALSE == IsNormalOperation))
#endif
            {
#if(WDGM_EB_MULTICORE_ENABLED == STD_OFF)
              const WdgM_EB_LocalStatusType OldLocalStatus = WDGM_EB_ATOMIC_LOAD(*SEWData_SELocalStatus);
#else
              const WdgM_EB_LocalStatusType LocalOldLocalStatus = WDGM_EB_ATOMIC_LOAD(*SEWData_SELocalStatus);
              const WdgM_LocalStatusType OldLocalStatus = (WdgM_LocalStatusType)(LocalOldLocalStatus & (WdgM_LocalStatusType)0xFFU);
              if(IsNormalOperation == FALSE)
              {
                WDGM_EB_ATOMIC_INIT(*SEWData_SELocalStatus, WDGM_LOCAL_STATUS_DEACTIVATED);
              }
              else
#endif
              {
                /* !LINKSTO WDGM.EB.TIMEPM.WDGM020427,1 */
                WDGM_EB_ATOMIC_STORE(*SEWData_SELocalStatus, WDGM_LOCAL_STATUS_DEACTIVATED);
              }
              /* !LINKSTO WDGM.ASR40.WDGM197,1,WdgM.ASR43.SWS_WdgM_00197,1 */
              /* status changes because of mode change, signal via callout and/or RTE */
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
              WDGM_EB_INDIVIDUAL_MODE_SWITCH(SECfg->SEId,OldLocalStatus, WDGM_LOCAL_STATUS_DEACTIVATED, SECfg->CoreID);
#else
              WDGM_EB_INDIVIDUAL_MODE_SWITCH(SECfg->SEId,OldLocalStatus, WDGM_LOCAL_STATUS_DEACTIVATED, 0U);
#endif
            }
            else
            {
              /* state EXPIRED: leave the local status in EXPIRED (if IsNormalOperation is TRUE) */
            }
          }

          /* Reset all local alive supervision related data of the SE.
           * Note: If IsNormalOperation equals FALSE (called during init phase), then both
           * the double-inverse alive counter value and the internal RAM data shall be reset. */
          /* !LINKSTO WDGM.EB.ResetAliveCounter,1 */
          WdgM_EB_SEResetCPAliveData (IntSEId, IntRamIdxBase, IsNormalOperation, CoreInstance);

          /* Reset SE runtime Data if SE is inactive in new mode or during the initialization phase
           * (IsNormalOperation == FALSE) for deadline and logical supervisions.
           * Reset failed reference cycle counter to 0 for the alive supervision. */
          if ((SEIsActive == FALSE) || (IsNormalOperation == FALSE))
          {
            /* set the failed reference cycle counter to 0 */
            *SEWData_FailedAliveSupervisionRefCycleCnt = 0U;

#if ((WDGM_EB_DEADLINE_SUPERVISION_ENABLED == STD_ON) || \
    (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_ON))
            /* reset all local logical and deadline supervision data of the SE. */
            WdgM_EB_SEResetData (IntSEId);
#endif
          }

#if (WDGM_EB_SE_NUM != 1U)
          /* Update offset to Ram index for next supervised Entity */
          IntRamIdxBase += SECfg->NumOfCPs;
          IntCoreSEId++;
#endif
        }
      }

#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
      if(CoreInstance->CoreID == WDGM_EB_MASTER_INSTANCE)
#endif
      {
        /* !LINKSTO WDGM.EB.TIMEPM.WDGM020424,1,WDGM.EB.MulticoreTriggerWatchdog,1,WDGM.ASR40.WDGM186,1 */
        /* !LINKSTO WDGM.EB.MulticoreSetWatchdog,1 */
        /* pass the new mode to all configured watchdogs */
        RetVal = WdgM_EB_PerformWdgIfModeSwitch (Mode);
      }
      /* assign the mode */
      CoreInstance->WdgM_EB_CurrentModeId = Mode;
    }
  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_SETMODE_EXIT(RetVal,Mode,CoreInstance);
  return RetVal;
}

/*------------------[WdgM_EB_PerformWdgIfModeSwitch]---------------------------------------------*/
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_PerformWdgIfModeSwitch
(
  WdgM_ModeType Mode
)
{
  Std_ReturnType RetVal = E_OK;
  WdgM_EB_TriggerIdType TriggerId;
  CONSTP2CONST(WdgM_EB_ModeCfgType,AUTOMATIC,WDGM_CONST) ModeCfg = &(WdgM_EB_ModeCfg[Mode]);
  CONSTP2CONST(WdgM_EB_TriggerCfgType,AUTOMATIC,WDGM_CONST) TriggerCfg = ModeCfg->TriggerCfg;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_PERFORMWDGIFMODESWITCH_ENTRY(Mode);

  /* go through all configured watchdogs of this WdgMMode and change the WatchdogMode */
  for (TriggerId = 0U; TriggerId < ModeCfg->NumTriggers; TriggerId++)
  {
    RetVal = WdgIf_SetMode (TriggerCfg[TriggerId].WatchdogId, TriggerCfg[TriggerId].WatchdogMode);

    /* if a mode switch failed this is treated as fatal error */
    if (RetVal != E_OK)
    {
      /* !LINKSTO WDGM.ASR40.WDGM006,1,WDGM.ASR40.WDGM142.1,1,WDGM.ASR40.WDGM004.12,1 */
#if (WDGM_EB_PROD_ERR_HANDLING_SET_MODE == WDGM_PROD_ERR_REP_TO_DEM)
      WDGM_EB_DEM(WDGM_EB_SET_MODE_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#elif (WDGM_EB_PROD_ERR_HANDLING_SET_MODE == WDGM_PROD_ERR_REP_TO_DET)
      WDGM_EB_DET(WDGM_SID_MAIN_FUNCTION, WDGM_EB_E_DEMTODET_SET_MODE);
#endif
      /* one mode switch failed: perform an immediate reset and leave the loop */
      /* !LINKSTO WDGM.EB.TIMEPM.WDGM020424,1,WDGM.EB.TIMEPM.WDGM020450,1,WDGM.ASR40.WDGM186,1 */
      WdgM_PerformReset();
      break;
    }
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_PERFORMWDGIFMODESWITCH_EXIT(RetVal,Mode);
  return RetVal;
}

#if (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_ON)
/*------------------[WdgM_EB_CheckpointReachedIntLogical]-------------------------------------------*/
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_CheckpointReachedIntLogical
(
  WdgM_EB_SEIdType IntSEId,
  WdgM_EB_CPIdType CPId,
  P2CONST(WdgM_EB_CPLogicalCfgType,AUTOMATIC,WDGM_CONST) CPIntLogicalCfg
)
{
  Std_ReturnType RetVal;
  CONSTP2CONST(WdgM_EB_SECfgType,AUTOMATIC,WDGM_CONST) SECfg =
      &(WdgM_EB_SECfg[IntSEId]);
  CONSTP2VAR(WdgM_EB_CPDataType,AUTOMATIC,WDGM_VAR) CPData =
      &(SECfg->CPData[CPId]);
  /* find the graph of the CP and localize the data */
  /* !LINKSTO WDGM.ASR40.WDGM295,1 */
  CONSTP2VAR(WdgM_EB_GraphDataType,AUTOMATIC,WDGM_VAR) GraphData =
      &(WdgM_EB_InternalGraphData[CPIntLogicalCfg->GraphId]);
  WdgM_EB_SEIdType LastIntSEId;
  WdgM_EB_CPIdType LastCPId;
  boolean ActiveFlag;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_CHECKPOINTREACHEDINTLOGICAL_ENTRY(IntSEId,CPId,CPIntLogicalCfg);

  /* check validity of the graph data */
  if (WdgM_EB_GetCheckedGraphData (GraphData, &LastIntSEId, &LastCPId, &ActiveFlag) != E_OK)
  {
    RetVal = E_NOT_OK;
  }
  else
  {
    /* !LINKSTO WDGM.ASR40.WDGM252,1,WDGM.ASR40.WDGM274,1,WDGM.ASR40.WDGM323,1 */
    /* check if the graph is active */
    if (ActiveFlag)
    {
      /* initial points are not allowed on active graphs */
      if (CPIntLogicalCfg->NumOfPredCPs == 0U)
      {
        RetVal = E_NOT_OK;
      }
      else
      {
        if (CPIntLogicalCfg->IsFinal)
        {
          /* !LINKSTO WDGM.ASR40.WDGM329,1,WDGM.ASR40.WDGM331,1 */
          ActiveFlag = FALSE;
        }
        /* this checkpoint has a predecessor which needs to be handled */
        RetVal = WdgM_EB_CheckPredCP (CPIntLogicalCfg, LastIntSEId, LastCPId);
      }
    }
    /* graph is inactive */
    else
    {
      if (CPIntLogicalCfg->NumOfPredCPs == 0U)
      {
        /* !LINKSTO WDGM.ASR40.WDGM273,1,WDGM.ASR40.WDGM332,1 */
        ActiveFlag = TRUE;
        RetVal = E_OK;
      }
      /* graph inactive and not an initial point */
      else
      {
        RetVal = E_NOT_OK;
      }
    }

    /* in *any* case: store this checkpoint */
    /* !LINKSTO WDGM.ASR40.WDGM246,1 */
    WdgM_EB_StoreGraphData(GraphData, IntSEId, CPId, ActiveFlag);

    /* If error recovery is disabled: check if the result was already incorrect */
    /* If error recovery is enabled: old result state shall not be considered */
    /* !LINKSTO WDGM.EB.TIMEPM.WDGM020481,1 */
    if (SECfg->ErrorRecoveryEnabled == FALSE)
    {
      WdgM_EB_ResultType CPLogicalResult = WDGM_EB_RESULT_INCORRECT;

      if (WdgM_EB_GetCheckedCPLogicalData (CPData, &CPLogicalResult) != E_OK)
      {
        /* !LINKSTO WDGM.EB.TIMEPM.WDGM020466,1,WDGM.EB.TIMEPM.WDGM020468,1 */
#if (WDGM_EB_PROD_ERR_HANDLING_DATA_CORRUPTION == WDGM_PROD_ERR_REP_TO_DEM)
        WDGM_EB_DEM(WDGM_EB_DATA_CORRUPTION_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#elif (WDGM_EB_PROD_ERR_HANDLING_DATA_CORRUPTION == WDGM_PROD_ERR_REP_TO_DET)
        WDGM_EB_DET(WDGM_SID_CHECKPOINT_REACHED, WDGM_EB_E_DEMTODET_DATA_CORRUPTION);
#endif
      }

      if (CPLogicalResult != WDGM_EB_RESULT_CORRECT)
      {
        RetVal = E_NOT_OK;
      }
    }
  }

  /* in *any* case: set the result */
  if (RetVal == E_OK)
  {
    WdgM_EB_StoreCPLogicalData (CPData, WDGM_EB_RESULT_CORRECT);
  }
  else
  {
    WdgM_EB_StoreCPLogicalData (CPData, WDGM_EB_RESULT_INCORRECT);
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_CHECKPOINTREACHEDINTLOGICAL_EXIT(RetVal,IntSEId,CPId,CPLogicalCfg);
  return RetVal;
}

#if (WDGM_EB_EXTERNAL_GRAPH_NUM > 0U)
/*------------------[WdgM_EB_CheckpointReachedExtLogical]-------------------------------------------*/
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_CheckpointReachedExtLogical
(
  WdgM_EB_SEIdType IntSEId,
  WdgM_EB_CPIdType CPId,
  P2CONST(WdgM_EB_CPLogicalCfgType,AUTOMATIC,WDGM_CONST) CPExtLogicalCfg,
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
)
{
  Std_ReturnType RetVal;
  CONSTP2CONST(WdgM_EB_SECfgType,AUTOMATIC,WDGM_CONST) SECfg =
      &(WdgM_EB_SECfg[IntSEId]);
  CONSTP2VAR(WdgM_EB_CPDataType,AUTOMATIC,WDGM_VAR) CPData =
      &(SECfg->CPData[CPId]);
  /* find the graph of the CP and localize the data */
  /* !LINKSTO WDGM.ASR40.WDGM295,1 */

  CONSTP2VAR(WdgM_EB_GraphDataType,AUTOMATIC,WDGM_VAR) GraphData =
      &(WdgM_EB_ExternalGraphData[CPExtLogicalCfg->GraphId]);

  CONSTP2CONST(WdgM_EB_ExtGraphDataSeqType,AUTOMATIC,WDGM_VAR) ExtGraphDataSeqPtr =
      &(WdgM_EB_ExtGraphDataSeqPtr[CPExtLogicalCfg->GraphId]);

  CONSTP2VAR(WdgM_EB_ExtGraphDataSeqArrayType,AUTOMATIC,WDGM_VAR) IntExtGraphDataSeqPtr =
      (ExtGraphDataSeqPtr->GraphDataPtr);

  uint8 Local_NumOfSEperGraphId = (ExtGraphDataSeqPtr->NumOfSEperGraphId);
  WdgM_EB_SEIdType LastIntSEId;
  WdgM_EB_CPIdType LastCPId;
  boolean ActiveFlag;
  uint8 idx = 0U;
  uint8 Local_ExtGraphDataSeq = 0U;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_CHECKPOINTREACHEDEXTLOGICAL_ENTRY(IntSEId,CPId,CPExtLogicalCfg,CoreInstance);

  /* check validity of the graph data */
  if (WdgM_EB_GetCheckedGraphData (GraphData, &LastIntSEId, &LastCPId, &ActiveFlag) != E_OK)
  {
    RetVal = E_NOT_OK;
  }
  else
  {
    /* !LINKSTO WDGM.ASR40.WDGM252,1,WDGM.ASR40.WDGM274,1,WDGM.ASR40.WDGM323,1 */
    /* check if the graph is active */
    if (ActiveFlag)
    {
      /* initial points are not allowed on active graphs */
      if (CPExtLogicalCfg->NumOfPredCPs == 0U)
      {
        RetVal = E_NOT_OK;
      }
      else
      {
        uint8 IndexInGraphDataSeq =  WdgM_EB_GetLastIndexInGraphDataSeq(LastIntSEId, LastCPId, CoreInstance);

        if (CPExtLogicalCfg->IsFinal)
        {
          /* !LINKSTO WDGM.ASR40.WDGM329,1,WDGM.ASR40.WDGM331,1 */
          ActiveFlag = FALSE;
        }
        /* this checkpoint has a predecessor which needs to be handled */
        RetVal = WdgM_EB_CheckPredCP (CPExtLogicalCfg, LastIntSEId, LastCPId);

        if(WdgM_EB_GetCheckedSeqNumber(IntExtGraphDataSeqPtr, IndexInGraphDataSeq, &Local_ExtGraphDataSeq) != E_OK)
        {
          RetVal = E_NOT_OK;
        }
        else
        {
          /* !LINKSTO WDGM.EB.DesignDecision.ConcurrentCall1,1 */
          /* increment counter to detect concurrent call to checkpoint function */
          Local_ExtGraphDataSeq++;
          WdgM_EB_StoreSeqNumber(IntExtGraphDataSeqPtr, CPExtLogicalCfg->SESequenceIdx, Local_ExtGraphDataSeq);
        }
      }
    }
    /* graph is inactive */
    else
    {
      if (CPExtLogicalCfg->NumOfPredCPs == 0U)
      {

        /* !LINKSTO WDGM.ASR40.WDGM273,1,WDGM.ASR40.WDGM332,1 */
        ActiveFlag = TRUE;

        /* Reset data from this graph if it is initial checkpoint */
        WdgM_EB_ResetExtGraphDataSeq(IntExtGraphDataSeqPtr,Local_NumOfSEperGraphId);

        /* is the first checkpoint, increment counter */
        Local_ExtGraphDataSeq = 1U;
        WdgM_EB_StoreSeqNumber(IntExtGraphDataSeqPtr, CPExtLogicalCfg->SESequenceIdx, Local_ExtGraphDataSeq);

        RetVal = E_OK;
      }
      /* graph inactive and not an initial point */
      else
      {
        RetVal = E_NOT_OK;
      }
    }


/* This callout is used only for WdgM_ConTest_14_LogSup_multicore test.*/
#ifdef TST_WDGM_CONCURRENTCALLOUT
      Tst_WdgM_ConcurrentCallout();
#endif

    /* in *any* case: store this checkpoint */
    /* !LINKSTO WDGM.ASR40.WDGM246,1 */
    WdgM_EB_StoreGraphData(GraphData, IntSEId, CPId, ActiveFlag);

    /* If error recovery is disabled: check if the result was already incorrect */
    /* If error recovery is enabled: old result state shall not be considered */
    /* !LINKSTO WDGM.EB.TIMEPM.WDGM020481,1 */
    if (SECfg->ErrorRecoveryEnabled == FALSE)
    {
      WdgM_EB_ResultType CPLogicalResult = WDGM_EB_RESULT_INCORRECT;

      if (WdgM_EB_GetCheckedCPLogicalData (CPData, &CPLogicalResult) != E_OK)
      {
        /* !LINKSTO WDGM.EB.TIMEPM.WDGM020466,1,WDGM.EB.TIMEPM.WDGM020468,1 */
#if (WDGM_EB_PROD_ERR_HANDLING_DATA_CORRUPTION == WDGM_PROD_ERR_REP_TO_DEM)
        WDGM_EB_DEM(WDGM_EB_DATA_CORRUPTION_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#elif (WDGM_EB_PROD_ERR_HANDLING_DATA_CORRUPTION == WDGM_PROD_ERR_REP_TO_DET)
        WDGM_EB_DET(WDGM_SID_CHECKPOINT_REACHED, WDGM_EB_E_DEMTODET_DATA_CORRUPTION);
#endif
      }

      if (CPLogicalResult != WDGM_EB_RESULT_CORRECT)
      {
        RetVal = E_NOT_OK;
      }
    }
  }


  /* detect concurrent call to checkpoint function */
  if (E_OK == RetVal)
  {
    for (idx = 0U; idx < Local_NumOfSEperGraphId; idx++)
    {
      if (idx != CPExtLogicalCfg->SESequenceIdx)
      {
        uint8 Other_SeqNumber;
        if (WdgM_EB_GetCheckedSeqNumber(IntExtGraphDataSeqPtr, idx, &Other_SeqNumber) != E_OK)
        {
          /* value corrupted */
          RetVal = E_NOT_OK;
        } /* if any one incremented their sequence number during the above checkpoint */
        else if (Other_SeqNumber >= Local_ExtGraphDataSeq)
        {
          /* failing is signaled by setting logical status to incorrect */
          /* concurrent call detected */
          RetVal = E_NOT_OK;
        }
        else
        {
          /* nothing to do */
        }
        if (E_NOT_OK == RetVal)
        {
          /* no need to search any more if Retval is E_NOT_OK */
          break;
        }
        else
        {
          /* nothing to do */
        }
      }
    }
  }


  /* in *any* case: set the result */
  if (E_OK == RetVal)
  {
    WdgM_EB_StoreCPLogicalData (CPData, WDGM_EB_RESULT_CORRECT);
  }
  else
  {
    WdgM_EB_StoreCPLogicalData (CPData, WDGM_EB_RESULT_INCORRECT);
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_CHECKPOINTREACHEDEXTLOGICAL_EXIT(RetVal,IntSEId,CPId,CPLogicalCfg,CoreInstance);
  return RetVal;
}

/*------------------[WdgM_EB_GetLastIndexInGraphDataSeq]-------------------------------------------*/
static FUNC (Std_ReturnType, WDGM_CODE) WdgM_EB_GetLastIndexInGraphDataSeq
(
  WdgM_EB_SEIdType LastIntSEId,
  WdgM_EB_CPIdType LastCPId,
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
)
{
  CONSTP2CONST(WdgM_EB_SECfgType,AUTOMATIC,WDGM_CONST) SECfg =
            &(WdgM_EB_SECfg[LastIntSEId]);
  CONSTP2CONST(WdgM_EB_CPCfgType,AUTOMATIC,WDGM_CONST) CPCfg =
            &(SECfg->CPCfg[LastCPId]);
  CONSTP2CONST(WdgM_EB_CPLogicalCfgType,AUTOMATIC,WDGM_CONST) CPExtLogicalCfg =
             CPCfg->ExtLogicalCfgPtr[CoreInstance->WdgM_EB_CurrentModeId];
  uint8 IndexInGraphDataSeq = CPExtLogicalCfg->SESequenceIdx;

  return IndexInGraphDataSeq;
}
#endif /* if (WDGM_EB_EXTERNAL_GRAPH_NUM > 0U) */
#endif

#if (WDGM_EB_DEADLINE_SUPERVISION_ENABLED == STD_ON)
/*------------------[WdgM_EB_CheckpointReachedDeadline]------------------------------------------*/
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_CheckpointReachedDeadline
(
  WdgM_EB_SEIdType IntSEId,
  WdgM_EB_CPIdType CPId,
  P2CONST(WdgM_EB_CPDeadlineCfgType,AUTOMATIC,WDGM_CONST) CPDeadlineCfg
)
{
  Std_ReturnType RetVal;
  const WdgM_EB_CPIdType StartCPId = CPDeadlineCfg->StartCPId;
  CONSTP2CONST(WdgM_EB_SECfgType,AUTOMATIC,WDGM_CONST) SECfg =
      &(WdgM_EB_SECfg[IntSEId]);
  CONSTP2VAR(WdgM_EB_CPDataType,AUTOMATIC,WDGM_VAR) StartCPData =
      &(SECfg->CPData[StartCPId]);
  /* checkpoint is a start checkpoint and a stop checkpoint of a previous DM.
   * If so, retrieve the Start CP DM Cfg. */
  const boolean IsStartStopCP = ((StartCPId == CPId) &&
      (CPDeadlineCfg->PrevCPDeadlineCfg !=
          (P2CONST(WdgM_EB_CPDeadlineCfgType,TYPEDEF,WDGM_CONST))NULL_PTR)) ?
              TRUE : FALSE;
  CONSTP2CONST(WdgM_EB_CPDeadlineCfgType,AUTOMATIC,WDGM_CONST) StopCPDeadlineCfg =
      (IsStartStopCP != FALSE) ? (CPDeadlineCfg->PrevCPDeadlineCfg) : (CPDeadlineCfg);
  const WdgM_EB_CPIdType StopCPId = StopCPDeadlineCfg->StopCPId;
  /* Get Start Checkpoint Id for given Stop Checkpoint Deadline Configuration.
   * If Stop Checkpoint is also a Start Checkpoint,
   * take Start Checkpoint Id of corresponding Start Checkpoint Deadline configuration.
   * Otherwise, take Start Checkpoint Id of actual given Checkpoint Deadline configuration. */
  const WdgM_EB_CPIdType StopStartCPId = StopCPDeadlineCfg->StartCPId;
  const WdgM_EB_DMIdType StartDMId = CPDeadlineCfg->DMId;
  const WdgM_EB_DMIdType StopDMId = StopCPDeadlineCfg->DMId;
  /* find the DM data of the Start CP and localize the data */
  CONSTP2VAR(WdgM_EB_DMDataType,AUTOMATIC,WDGM_VAR) StartDMData =
      &(WdgM_EB_DMData[StartDMId]);
  CONSTP2VAR(WdgM_EB_DMDataType,AUTOMATIC,WDGM_VAR) StopDMData =
      &(WdgM_EB_DMData[StopDMId]);
  CONSTP2VAR(WdgM_EB_CPDataType,AUTOMATIC,WDGM_VAR) StopCPData =
      &(SECfg->CPData[StopStartCPId]);
  Std_ReturnType StartCPRetVal = E_NOT_OK;
  Std_ReturnType StopCPRetVal = E_NOT_OK;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_CHECKPOINTREACHEDDEADLINE_ENTRY(IntSEId,CPId,CPDeadlineCfg);

  if (StopCPId == CPId)
  {
    StopCPRetVal = WdgM_EB_StopCheckpointDeadline(
        StopDMData,
        StopCPData,
        StopCPDeadlineCfg);
  }

  if (StartCPId == CPId)
  {
    StartCPRetVal = WdgM_EB_StartCheckpointDeadline(
        StartDMData,
        StopDMData,
        StartCPData,
        CPDeadlineCfg,
        SECfg->ErrorRecoveryEnabled);
  }

  /* !LINKSTO WDGM.ASR40.WDGM322,1 */
  if (IsStartStopCP)
  {
    /* In case this is a Start / Stop Checkpoint, take the worse result of both. */
    if (StopCPRetVal == E_OK)
    {
      RetVal = StartCPRetVal;
    }
    else
    {
      RetVal = StopCPRetVal;
    }
  }
  else
  {
    if (StartCPId == CPId)
    {
      /* Checkpoint is a Start CP => take result of Start CP */
      RetVal = StartCPRetVal;
    }
    else
    {
      /* Checkpoint is a Stop CP => take result of Stop CP */
      RetVal = StopCPRetVal;
    }
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_CHECKPOINTREACHEDDEADLINE_EXIT(RetVal,IntSEId,CPId,CPDeadlineCfg);
  return RetVal;
}

/*------------------[WdgM_EB_StartCheckpointDeadline]--------------------------------------------*/
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_StartCheckpointDeadline
(
  P2VAR(WdgM_EB_DMDataType,AUTOMATIC,WDGM_VAR) StartDMData,
  P2CONST(WdgM_EB_DMDataType,AUTOMATIC,WDGM_VAR) StopDMData,
  P2VAR(WdgM_EB_CPDataType,AUTOMATIC,WDGM_VAR) CPData,
  P2CONST(WdgM_EB_CPDeadlineCfgType,AUTOMATIC,WDGM_CONST) CPDeadlineCfg,
  boolean ErrorRecoveryEnabled
)
{
  Std_ReturnType StartCPRetVal;
  WdgM_EB_TimestampType TimeStamp;
  uint16 MaxCycles;
  boolean IsActive;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_STARTCHECKPOINTDEADLINE_ENTRY(StartDMData,
      StopDMData,CPData,CPDeadlineCfg,ErrorRecoveryEnabled);

  /* record time-stamp and set DM active.
   * If this CP is also a Stop CP (Pointer to DM Data is different),
   * then the timestamp shall be copied from the stop CP including inverse data
   * before data validation check which is more efficient. */
  /* !LINKSTO WDGM.EB.ASR40.WDGM228,1 */
  if (StartDMData != StopDMData)
  {
    StartDMData->StartTimestampData = StopDMData->StartTimestampData;
    StartDMData->StartTimestampDataInv = StopDMData->StartTimestampDataInv;
  }

  if (WdgM_EB_GetCheckedDMData (StartDMData, &TimeStamp, &MaxCycles, &IsActive) != E_OK)
  {
    StartCPRetVal = E_NOT_OK;
  }
  else
  {
    /* start CP and active DM is not allowed */
    if (IsActive)
    {
      StartCPRetVal = E_NOT_OK;
    }
    else
    {
      /* record time-stamp and set DM active.
       * If this CP is a pure Start CP (Pointer to DM Data are equal),
       * then a new timestamp must be recorded. */
      /* !LINKSTO WDGM.EB.ASR40.WDGM228,1 */
      if (StartDMData == StopDMData)
      {
        WdgM_EB_TimestampType ElapsedTime = 0U;
        WDGM_EB_GET_TIME(&TimeStamp, &ElapsedTime);
      }

      /* update actual time, maximum allowed main function calls until deadline violation, and
       * active flag */
      WdgM_EB_StoreDMData (StartDMData, &TimeStamp, CPDeadlineCfg->DMMaxMainfunctionCycles, TRUE);

      StartCPRetVal = E_OK;
    }

    /* check if the result was already incorrect
     * from a previous run and then leave it as such (only if no error recovery provided) */
    if (ErrorRecoveryEnabled == FALSE)
    {
      WdgM_EB_ResultType CPDeadlineResult = WDGM_EB_RESULT_INCORRECT;
      if (WdgM_EB_GetCheckedCPDeadlineData(CPData, &CPDeadlineResult) != E_OK)
      {
        /* !LINKSTO WDGM.EB.TIMEPM.WDGM020466,1,WDGM.EB.TIMEPM.WDGM020468,1 */
#if (WDGM_EB_PROD_ERR_HANDLING_DATA_CORRUPTION == WDGM_PROD_ERR_REP_TO_DEM)
        WDGM_EB_DEM(WDGM_EB_DATA_CORRUPTION_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#elif (WDGM_EB_PROD_ERR_HANDLING_DATA_CORRUPTION == WDGM_PROD_ERR_REP_TO_DET)
        WDGM_EB_DET(WDGM_SID_CHECKPOINT_REACHED, WDGM_EB_E_DEMTODET_DATA_CORRUPTION);
#endif
      }

      if (CPDeadlineResult != WDGM_EB_RESULT_CORRECT)
      {
        StartCPRetVal = E_NOT_OK;
      }
    }
  }

  /* !LINKSTO WDGM.ASR40.WDGM294,1 */
  if (StartCPRetVal == E_OK)
  {
    WdgM_EB_StoreCPDeadlineData (CPData, WDGM_EB_RESULT_CORRECT);
  }
  else
  {
    WdgM_EB_StoreCPDeadlineData (CPData, WDGM_EB_RESULT_INCORRECT);
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_STARTCHECKPOINTDEADLINE_EXIT(StartCPRetVal,
      StartDMData,StopDMData,CPData,CPDeadlineCfg,ErrorRecoveryEnabled);
  return StartCPRetVal;
}


/*------------------[WdgM_EB_StopCheckpointDeadline]---------------------------------------------*/
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_StopCheckpointDeadline
(
  P2VAR(WdgM_EB_DMDataType,AUTOMATIC,WDGM_VAR) DMData,
  P2VAR(WdgM_EB_CPDataType,AUTOMATIC,WDGM_VAR) CPData,
  P2CONST(WdgM_EB_CPDeadlineCfgType,AUTOMATIC,WDGM_CONST) CPDeadlineCfg
)
{
  Std_ReturnType StopCPRetVal;
  WdgM_EB_TimestampType TimeStamp;
  uint16 MaxCycles;
  boolean IsActive;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_STOPCHECKPOINTDEADLINE_ENTRY(DMData,CPData,CPDeadlineCfg);

  if (WdgM_EB_GetCheckedDMData (DMData, &TimeStamp, &MaxCycles, &IsActive) != E_OK)
  {
    StopCPRetVal = E_NOT_OK;
  }
  else
  {
    if (IsActive)
    {
      /* stop CP and active DM => check deadline violation*/
      /* !LINKSTO WDGM.EB.ASR40.WDGM229,1 */
      const WdgM_EB_TimestampType WdgMDeadlineMax = CPDeadlineCfg->WdgMDeadlineMax;
      const WdgM_EB_TimestampType WdgMDeadlineMin = CPDeadlineCfg->WdgMDeadlineMin;
      WdgM_EB_TimestampType ElapsedTime = 0U;
      WDGM_EB_GET_TIME(&TimeStamp, &ElapsedTime);

      if ((ElapsedTime >= WdgMDeadlineMin) && (ElapsedTime <= WdgMDeadlineMax))
      {
        StopCPRetVal = E_OK;
      }
      else
      {
        StopCPRetVal = E_NOT_OK;
      }

      /* set Deadline monitoring inactive */
      WdgM_EB_StoreDMData(DMData, &TimeStamp, MaxCycles, FALSE);
    }
    else
    {
      /* DM inactive => no deadline violation possible for stop CP */
      StopCPRetVal = E_OK;
    }

    /* check if the result was already incorrect (shared result state between start and stop CP) */
    /* !LINKSTO WDGM.EB.TIMEPM.WDGM020480,1 */
    {
      WdgM_EB_ResultType CPDeadlineResult = WDGM_EB_RESULT_INCORRECT;
      if (WdgM_EB_GetCheckedCPDeadlineData(CPData, &CPDeadlineResult) != E_OK)
      {
        /* !LINKSTO WDGM.EB.TIMEPM.WDGM020466,1,WDGM.EB.TIMEPM.WDGM020468,1 */
#if (WDGM_EB_PROD_ERR_HANDLING_DATA_CORRUPTION == WDGM_PROD_ERR_REP_TO_DEM)
        WDGM_EB_DEM(WDGM_EB_DATA_CORRUPTION_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#elif (WDGM_EB_PROD_ERR_HANDLING_DATA_CORRUPTION == WDGM_PROD_ERR_REP_TO_DET)
        WDGM_EB_DET(WDGM_SID_CHECKPOINT_REACHED, WDGM_EB_E_DEMTODET_DATA_CORRUPTION);
#endif
      }

      if (CPDeadlineResult != WDGM_EB_RESULT_CORRECT)
      {
        StopCPRetVal = E_NOT_OK;
      }
    }
  }

  /* !LINKSTO WDGM.ASR40.WDGM294,1 */
  if (StopCPRetVal == E_OK)
  {
    WdgM_EB_StoreCPDeadlineData (CPData, WDGM_EB_RESULT_CORRECT);
  }
  else
  {
    WdgM_EB_StoreCPDeadlineData (CPData, WDGM_EB_RESULT_INCORRECT);
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_STOPCHECKPOINTDEADLINE_EXIT(StopCPRetVal,DMData,CPData,CPDeadlineCfg);
  return StopCPRetVal;
}

/*------------------[WdgM_EB_CheckDeadlineViolation]---------------------------------------------*/
static FUNC(WdgM_EB_ResultType, WDGM_CODE) WdgM_EB_CheckDeadlineViolation
(
  WdgM_EB_CPIdType CPId,
  P2CONST(WdgM_EB_CPDeadlineCfgType,AUTOMATIC,WDGM_CONST) CPDeadlineCfg
)
{
  WdgM_EB_ResultType RetVal;

  /* find the DM data of the Start CP and localize the data */
  CONSTP2VAR(WdgM_EB_DMDataType,AUTOMATIC,WDGM_VAR) StartDMData =
      &(WdgM_EB_DMData[CPDeadlineCfg->DMId]);

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_CHECKDEADLINEVIOLATION_ENTRY(CPId,CPDeadlineCfg);

  if ((CPDeadlineCfg->StartCPId) == CPId)
  {
    if (WdgM_EB_DecMaxCyclesDMData(StartDMData) == E_OK)
    {
      RetVal = WDGM_EB_RESULT_CORRECT;
    }
    else
    {
      RetVal = WDGM_EB_RESULT_INCORRECT;
    }
  }
  else
  {
    /* For Stop checkpoint, no evaluation can be performed
     * In this case, correct result is returned. */
    RetVal = WDGM_EB_RESULT_CORRECT;
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_CHECKDEADLINEVIOLATION_EXIT(RetVal,CPId,CPDeadlineCfg);
  return RetVal;
}
#endif

/*------------------[WdgM_EB_CheckpointReachedAlive]---------------------------------------------*/
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_CheckpointReachedAlive
(
  WdgM_EB_SEIdType IntSEId,
  WdgM_EB_CPIdType CPId
)
{
  Std_ReturnType RetVal;
  CONSTP2CONST(WdgM_EB_SECfgType,AUTOMATIC,WDGM_CONST) SECfg =
      &(WdgM_EB_SECfg[IntSEId]);
  CONSTP2VAR(WdgM_EB_CPDataType,AUTOMATIC,WDGM_VAR) CPData =
      &(SECfg->CPData[CPId]);

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_CHECKPOINTREACHEDALIVE_ENTRY(IntSEId,CPId);

  /* check validity of current alive counter data */
  if (WdgM_EB_CheckCPAliveCounterData (CPData) != E_OK)
  {
    RetVal = E_NOT_OK;
  }
  else
  {
    /* Increment alive counter data, an overflow / wrap-around is handled in
     * the WdgM_EB_GetCPAliveCounterData API */
    /* !LINKSTO WDGM.ASR40.WDGM321,1 */
    WdgM_EB_IncrementCPAliveCounterData (CPData);

    RetVal = E_OK;
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_CHECKPOINTREACHEDALIVE_EXIT(RetVal,IntSEId,CPId);
  return RetVal;
}

#if (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_ON)
/*------------------[WdgM_EB_CheckPredCP]--------------------------------------------------------*/
static FUNC (Std_ReturnType, WDGM_CODE) WdgM_EB_CheckPredCP
(
  P2CONST(WdgM_EB_CPLogicalCfgType,AUTOMATIC,WDGM_CONST) CPLogicalCfg,
  WdgM_EB_SEIdType IntSEId,
  WdgM_EB_CPIdType CPId
)
{
  Std_ReturnType RetVal;
  WdgM_EB_CPIdType CPIdx;
  boolean FoundCP = FALSE;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_CHECKPREDCP_ENTRY(CPLogicalCfg,IntSEId,CPId);

  /* find the predecessor in the list of valid CPs */
  for (CPIdx = 0U; CPIdx < CPLogicalCfg->NumOfPredCPs; CPIdx++)
  {
    if ((CPLogicalCfg->PredCPs[CPIdx].SEId == IntSEId) &&
        (CPLogicalCfg->PredCPs[CPIdx].CPId == CPId))
    {
      FoundCP = TRUE;
      break; /* found a legal CP */
    }
  }

  if (FoundCP)
  {
    CONSTP2CONST(WdgM_EB_SECfgType,AUTOMATIC,WDGM_CONST) SECfg =
        &(WdgM_EB_SECfg[IntSEId]);
    CONSTP2CONST(WdgM_EB_CPDataType,AUTOMATIC,WDGM_VAR) LastCPData =
        &(SECfg->CPData[CPId]);
    WdgM_EB_ResultType CPLogicalResult = WDGM_EB_RESULT_INCORRECT;

    /* check whether or not the status of the predecessor Checkpoint was OK
     * for tracking the error status to all successor Checkpoints. */
    /* !LINKSTO WDGM.EB.LogicalSupervision.ErrorTracking,1 */
    RetVal = WdgM_EB_GetCheckedCPLogicalData (LastCPData, &CPLogicalResult);

    if (RetVal != E_OK)
    {
      /* !LINKSTO WDGM.EB.TIMEPM.WDGM020466,1,WDGM.EB.TIMEPM.WDGM020468,1 */
#if (WDGM_EB_PROD_ERR_HANDLING_DATA_CORRUPTION == WDGM_PROD_ERR_REP_TO_DEM)
      WDGM_EB_DEM(WDGM_EB_DATA_CORRUPTION_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#elif (WDGM_EB_PROD_ERR_HANDLING_DATA_CORRUPTION == WDGM_PROD_ERR_REP_TO_DET)
      WDGM_EB_DET(WDGM_SID_CHECKPOINT_REACHED, WDGM_EB_E_DEMTODET_DATA_CORRUPTION);
#endif
    }

    if (CPLogicalResult != WDGM_EB_RESULT_CORRECT)
    {
      RetVal = E_NOT_OK;
    }
  }
  else
  {
    RetVal = E_NOT_OK;
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_CHECKPREDCP_EXIT(RetVal,CPLogicalCfg,IntSEId,CPId);
  return RetVal;
}
#endif

/*------------------[WdgM_EB_DetermineAliveResult]-----------------------------------------------*/
static FUNC(WdgM_EB_ResultType, WDGM_CODE) WdgM_EB_DetermineAliveResult
(
  WdgM_EB_SEIdType IntSEId,
  WdgM_EB_CPIdType CPId,
  WdgM_EB_IntRamIdxType IntRamIdx,
  P2CONST(WdgM_EB_CPAliveCfgType,AUTOMATIC,WDGM_CONST) CPAliveCfg
)
{
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance = WdgM_GetCoreInstance();
  CONSTP2CONST(WdgM_EB_SECfgType,AUTOMATIC,WDGM_CONST) SECfg =
      &(WdgM_EB_SECfg[IntSEId]);
  const uint32 CPRefCycleCnt = (uint32)
      (CoreInstance->WdgM_EB_SupervisionCycleCnt % (CPAliveCfg->SupervisionReferenceCycle));
  WdgM_EB_ResultType AliveResult;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_DETERMINEALIVERESULT_ENTRY(IntSEId,CPId,IntRamIdx,CPAliveCfg);

  /* perform supervision if this call is the reference cycle */
  /* !LINKSTO WDGM.ASR40.WDGM115,1 */
  if (CPRefCycleCnt == 0U)
  {
    CONSTP2VAR(WdgM_EB_CPDataType,AUTOMATIC,WDGM_VAR) CPData =
        &(SECfg->CPData[CPId]);
    WdgM_EB_AliveCounterType AliveCounter;

    /* check if alive counter data is intact */
    if (WdgM_EB_GetCheckedCPAliveCounterData (CPData, IntRamIdx, &AliveCounter, CoreInstance) != E_OK)
    {
      AliveResult = WDGM_EB_RESULT_INCORRECT;
    }
    else
    {
      /* !LINKSTO WDGM.ASR40.WDGM074,1,WDGM.ASR40.WDGM115,1,WDGM.ASR40.WDGM324,1 */
      /* check the number of activations */
      if ((AliveCounter < (CPAliveCfg->ExpectedAliveIndication - CPAliveCfg->MinMargin)) ||
          (AliveCounter > (CPAliveCfg->ExpectedAliveIndication + CPAliveCfg->MaxMargin)))
      {
        AliveResult = WDGM_EB_RESULT_INCORRECT;
      }
      else
      {
        AliveResult = WDGM_EB_RESULT_CORRECT;
      }
    }

    /* reset the alive counter for next reference cycle based on previously retrieved
     * alive counter value to consider race condition when alive counter
     * was incremented due to a CheckpointReached in between. */
    WdgM_EB_ResetCPAliveCounterData (IntRamIdx, AliveCounter, CoreInstance);
  }
  else
  {
    /* !LINKSTO WDGM.ASR40.WDGM098,1 */
    /* no alive supervision should be evaluated if not in the reference cycle */
    AliveResult = WDGM_EB_RESULT_NOTEVALUATED;
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_DETERMINEALIVERESULT_EXIT(AliveResult,IntSEId,CPId,IntRamIdx,CPAliveCfg);
  return AliveResult;
}

/*------------------[WdgM_EB_DetermineDeadlineResult]-------------------------------------------*/
#if (WDGM_EB_DEADLINE_SUPERVISION_ENABLED == STD_ON)
static FUNC(WdgM_EB_ResultType, WDGM_CODE) WdgM_EB_DetermineDeadlineResult
(
  WdgM_EB_SEIdType IntSEId,
  WdgM_EB_CPIdType CPId
)
{
  CONSTP2CONST(WdgM_EB_SECfgType,AUTOMATIC,WDGM_CONST) SECfg =
      &(WdgM_EB_SECfg[IntSEId]);
  CONSTP2VAR(WdgM_EB_CPDataType,AUTOMATIC,WDGM_VAR) CPData =
      &(SECfg->CPData[CPId]);
  WdgM_EB_ResultType CPDeadlineResult = WDGM_EB_RESULT_INCORRECT;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_DETERMINEDEADLINERESULT_ENTRY(IntSEId,CPId);

  if (WdgM_EB_GetCheckedCPDeadlineData(CPData, &CPDeadlineResult) != E_OK)
  {
    /* !LINKSTO WDGM.EB.TIMEPM.WDGM020466,1,WDGM.EB.TIMEPM.WDGM020467,1 */
#if (WDGM_EB_PROD_ERR_HANDLING_DATA_CORRUPTION == WDGM_PROD_ERR_REP_TO_DEM)
    WDGM_EB_DEM(WDGM_EB_DATA_CORRUPTION_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#elif (WDGM_EB_PROD_ERR_HANDLING_DATA_CORRUPTION == WDGM_PROD_ERR_REP_TO_DET)
    WDGM_EB_DET(WDGM_SID_MAIN_FUNCTION, WDGM_EB_E_DEMTODET_DATA_CORRUPTION);
#endif
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_DETERMINEDEADLINERESULT_EXIT(CPDeadlineResult,IntSEId,CPId);
  return CPDeadlineResult;
}
#endif /* (WDGM_EB_DEADLINE_SUPERVISION_ENABLED == STD_ON) */

#if (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_ON)
/*------------------[WdgM_EB_DetermineLogicalResult]---------------------------------------------*/
static FUNC(WdgM_EB_ResultType, WDGM_CODE) WdgM_EB_DetermineLogicalResult
(
  WdgM_EB_SEIdType IntSEId,
  WdgM_EB_CPIdType CPId
)
{
  CONSTP2CONST(WdgM_EB_SECfgType,AUTOMATIC,WDGM_CONST) SECfg =
      &(WdgM_EB_SECfg[IntSEId]);
  CONSTP2VAR(WdgM_EB_CPDataType,AUTOMATIC,WDGM_VAR) CPData =
      &(SECfg->CPData[CPId]);
  WdgM_EB_ResultType CPLogicalResult = WDGM_EB_RESULT_INCORRECT;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_DETERMINELOGICALRESULT_ENTRY(IntSEId,CPId);

  if (WdgM_EB_GetCheckedCPLogicalData (CPData, &CPLogicalResult) != E_OK)
  {
    /* !LINKSTO WDGM.EB.TIMEPM.WDGM020466,1,WDGM.EB.TIMEPM.WDGM020467,1 */
#if (WDGM_EB_PROD_ERR_HANDLING_DATA_CORRUPTION == WDGM_PROD_ERR_REP_TO_DEM)
    WDGM_EB_DEM(WDGM_EB_DATA_CORRUPTION_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#elif (WDGM_EB_PROD_ERR_HANDLING_DATA_CORRUPTION == WDGM_PROD_ERR_REP_TO_DET)
    WDGM_EB_DET(WDGM_SID_MAIN_FUNCTION, WDGM_EB_E_DEMTODET_DATA_CORRUPTION);
#endif
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_DETERMINELOGICALRESULT_EXIT(CPLogicalResult,IntSEId,CPId);
  return CPLogicalResult;
}
#endif

/*------------------[WdgM_EB_SetLocalStatus]-----------------------------------------------------*/
static FUNC(void, WDGM_CODE) WdgM_EB_SetLocalStatus
(
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
)
{
  WdgM_EB_SEIdType IntSEId = 0U;
  WdgM_EB_SEIdType IntCoreSEId = 0U;
  WdgM_EB_IntRamIdxType IntRamIdxBase = 0U;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_SETLOCALSTATUS_ENTRY(CoreInstance);

  /* go through each supervised entity */
#if (WDGM_EB_SE_NUM != 1U)
  for (IntSEId = 0U; IntSEId < WDGM_EB_SE_NUM; IntSEId++)
#endif
  {
    CONSTP2CONST(WdgM_EB_SECfgType,AUTOMATIC,WDGM_CONST) SECfg =
        &(WdgM_EB_SECfg[IntSEId]);
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
    if(CoreInstance->CoreID == SECfg->CoreID)
#endif
    {
    WdgM_EB_CPIdType CPId;
    boolean IsAliveResultSet = FALSE;
    WdgM_EB_ResultType AliveResult = WDGM_EB_RESULT_NOTEVALUATED;
#if (WDGM_EB_DEADLINE_SUPERVISION_ENABLED == STD_ON)
    WdgM_EB_ResultType DeadlineResult = WDGM_EB_RESULT_CORRECT;
#endif
#if (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_ON)
    WdgM_EB_ResultType LogicalResult = WDGM_EB_RESULT_CORRECT;
#endif

    /* if the IntSEId is deactivated: don't perform any supervision */
    /* !LINKSTO WDGM.ASR40.WDGM208,1 */
    if (WDGM_EB_ATOMIC_LOAD(CoreInstance->WdgM_EB_SELocalStatus[IntSEId])
        != WDGM_LOCAL_STATUS_DEACTIVATED)
    {
      /* go through each CP of this SE */
      for (CPId = 0U; CPId < SECfg->NumOfCPs; CPId++)
      {
        CONSTP2CONST(WdgM_EB_CPCfgType,AUTOMATIC,WDGM_CONST) CPCfg =
            &(SECfg->CPCfg[CPId]);
        const WdgM_EB_IntRamIdxType IntRamIdx = IntRamIdxBase + CPId;
        WdgM_EB_ResultType TmpResult;

        {
          CONSTP2CONST(WdgM_EB_CPAliveCfgType,AUTOMATIC,WDGM_CONST) CPAliveCfg =
              CPCfg->AliveCfgPtr[CoreInstance->WdgM_EB_CurrentModeId];
          /* check alive supervision only if configured */
          /* !LINKSTO WDGM.ASR40.WDGM083,1 */
          if (CPAliveCfg != NULL_PTR)
          {
#if (WDGM_EB_ENABLE_SETMODE_SYNCHRON == STD_ON)
            if( WdgM_EB_TmpDisableAliveSupv == FALSE)
#endif
            {
              TmpResult = WdgM_EB_DetermineAliveResult (IntSEId, CPId, IntRamIdx, CPAliveCfg);

              if ((AliveResult == WDGM_EB_RESULT_NOTEVALUATED) ||
                 ((TmpResult != WDGM_EB_RESULT_NOTEVALUATED) &&
                  (TmpResult != WDGM_EB_RESULT_CORRECT)))
              {
                IsAliveResultSet = TRUE;
                AliveResult = TmpResult;
              }
            }
#if (WDGM_EB_ENABLE_SETMODE_SYNCHRON == STD_ON)
            else
            {
             /* preserve the local status of the Supervised Entity(s)
              * that are active in the new mode */
              IsAliveResultSet = TRUE;
            }
#endif
          }
        }

#if (WDGM_EB_DEADLINE_SUPERVISION_ENABLED == STD_ON)
        if (DeadlineResult == WDGM_EB_RESULT_CORRECT)
        {
          CONSTP2CONST(WdgM_EB_CPDeadlineCfgType,AUTOMATIC,WDGM_CONST) CPDeadlineCfg =
              CPCfg->DeadlineCfgPtr[CoreInstance->WdgM_EB_CurrentModeId];
          /* check deadline monitoring only if required  */
          if (CPDeadlineCfg != NULL_PTR)
          {
            /* check if deadline violation occurred in case stop CP is never called. */
            /* !LINKSTO WDGM.EB.TIMEPM.WDGM020455,1 */
            TmpResult = WdgM_EB_CheckDeadlineViolation(
                CPId,
                CPDeadlineCfg);
            /* !LINKSTO WDGM.EB.TIMEPM.WDGM020456,1 */
            if (TmpResult == WDGM_EB_RESULT_CORRECT)
            {
              TmpResult = WdgM_EB_DetermineDeadlineResult (IntSEId, CPId);
            }
            DeadlineResult = TmpResult;
          }
        }
#endif

#if (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_ON)
        if (LogicalResult == WDGM_EB_RESULT_CORRECT)
        {
          CONSTP2CONST(WdgM_EB_CPLogicalCfgType,AUTOMATIC,WDGM_CONST) CPIntLogicalCfg =
              CPCfg->IntLogicalCfgPtr[CoreInstance->WdgM_EB_CurrentModeId];
          CONSTP2CONST(WdgM_EB_CPLogicalCfgType,AUTOMATIC,WDGM_CONST) CPExtLogicalCfg =
              CPCfg->ExtLogicalCfgPtr[CoreInstance->WdgM_EB_CurrentModeId];
          /* check logical supervision only if required */
          if ((CPIntLogicalCfg != NULL_PTR) || (CPExtLogicalCfg != NULL_PTR))
          {
            LogicalResult = WdgM_EB_DetermineLogicalResult (IntSEId, CPId);
          }
        }
#endif
      }

      /* If no Alive Supervision is configured, then a correct supervision result is assumed */
      if (IsAliveResultSet == FALSE)
      {
        AliveResult = WDGM_EB_RESULT_CORRECT;
      }

      /* finally: determine and set the local status */
      WdgM_EB_SetSELocalStatus (
            IntSEId,
            IntCoreSEId,
            AliveResult
#if (WDGM_EB_DEADLINE_SUPERVISION_ENABLED == STD_ON)
            , DeadlineResult
#endif
#if (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_ON)
            , LogicalResult
#endif
            , CoreInstance
            );
    }

#if (WDGM_EB_SE_NUM != 1U)
    /* Update offset to Ram index for next supervised Entity */
    IntRamIdxBase += SECfg->NumOfCPs;
    IntCoreSEId++;
#endif
    }
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_SETLOCALSTATUS_EXIT(CoreInstance);
}

/*------------------[WdgM_EB_SetSELocalStatus]---------------------------------------------------*/

static FUNC(void, WDGM_CODE) WdgM_EB_SetSELocalStatus
(
  WdgM_EB_SEIdType IntSEId,
  WdgM_EB_SEIdType IntCoreSEId,
  WdgM_EB_ResultType AliveResult
#if (WDGM_EB_DEADLINE_SUPERVISION_ENABLED == STD_ON)
  , WdgM_EB_ResultType DeadlineResult
#endif
#if (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_ON)
  , WdgM_EB_ResultType LogicalResult
#endif
  , CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
)
{
#if (WDGM_EB_MULTICORE_ENABLED == STD_ON)
  CONSTP2VAR(Atomic_t,AUTOMATIC,AUTOMATIC) SEWData_SELocalStatus =
      &(CoreInstance->WdgM_EB_SELocalStatus[IntSEId]);
#else
  CONSTP2VAR(WdgM_LocalStatusType,AUTOMATIC,AUTOMATIC) SEWData_SELocalStatus =
      &(CoreInstance->WdgM_EB_SELocalStatus[IntSEId]);
#endif
  CONSTP2VAR(uint16,AUTOMATIC,AUTOMATIC) SEWData_FailedAliveSupervisionRefCycleCnt =
      &(CoreInstance->WdgM_EB_FailedAliveSupervisionRefCycleCnt[IntCoreSEId]);
  CONSTP2CONST(WdgM_EB_SECfgType,AUTOMATIC,WDGM_CONST) SECfg =
      &(WdgM_EB_SECfg[IntSEId]);
  const uint8 FailedAliveSupervisionRefCycleTol =
      SECfg->FailedAliveSupervisionRefCycleTol[CoreInstance->WdgM_EB_CurrentModeId];
  WdgM_EB_LocalStatusType NewLocalStatus = WDGM_EB_ATOMIC_LOAD(*SEWData_SELocalStatus);
  const uint16 NextFailedAliveSupervisionRefCycleCnt =
      (uint16)(*SEWData_FailedAliveSupervisionRefCycleCnt + 1U);
  /* !LINKSTO WDGM.ASR40.WDGM202,1,WDGM.EB.TIMEPM.WDGM020448,1 */
  const uint8 NextFailedStatus = (SECfg->ErrorRecoveryEnabled) ? WDGM_LOCAL_STATUS_FAILED
      : WDGM_LOCAL_STATUS_EXPIRED;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_SETSELOCALSTATUS_ENTRY(IntSEId,IntCoreSEId,AliveResult,DeadlineResult,LogicalResult,CoreInstance);

  switch (WDGM_EB_ATOMIC_LOAD(*SEWData_SELocalStatus))
  {
    case WDGM_LOCAL_STATUS_OK: /* fall through: common evaluation for Status OK and FAILED */
    case WDGM_LOCAL_STATUS_FAILED:
    {
      switch (AliveResult)
      {
      case WDGM_EB_RESULT_INCORRECT:
      {
        /* !LINKSTO WDGM.EB.ASR40.WDGM203,1,WDGM.EB.ASR40.WDGM204,1 */
        if (NextFailedAliveSupervisionRefCycleCnt <= FailedAliveSupervisionRefCycleTol)
        {
          (*SEWData_FailedAliveSupervisionRefCycleCnt)++;
          /* local status can be ok or failed before => always set local status to failed */
          /* !LINKSTO WDGM.EB.TIMEPM.WDGM020482,1,WDGM.EB.TIMEPM.WDGM020483,1 */
          NewLocalStatus = WDGM_LOCAL_STATUS_FAILED;
        }
        else
        {
          /* !LINKSTO WDGM.ASR40.WDGM202,1,WDGM.EB.TIMEPM.WDGM020448,1,WDGM.EB.ASR40.WDGM206,1 */
          /* Note: This code is always executed if FailedAliveSupervisionRefCycleTol == 0U. */
          NewLocalStatus = NextFailedStatus;
        }
        break;
      }

      case WDGM_EB_RESULT_CORRECT:
      {
        /* !LINKSTO WDGM.EB.ASR40.WDGM300,1,WDGM.ASR40.WDGM205,1 */
        if (*SEWData_FailedAliveSupervisionRefCycleCnt != 0U)
        {
          /* !LINKSTO WDGM.EB.TIMEPM.WDGM020484,1 */
          (*SEWData_FailedAliveSupervisionRefCycleCnt)--;
        }
        /* !LINKSTO WDGM.ASR40.WDGM205,1 */
        if (*SEWData_FailedAliveSupervisionRefCycleCnt == 0U)
        {
          NewLocalStatus = WDGM_LOCAL_STATUS_OK;
        }
        break;
      }

      case WDGM_EB_RESULT_NOTEVALUATED:
        /* !LINKSTO WDGM.ASR40.WDGM205,1 */
        /* Keep local status. A change to Local Status OK is only possible
         * at the Supervision Reference Cycle when an Alive Result is available. */
        break;

        /* CHECK: NOPARSE */
      default:
        WDGM_UNREACHABLE_CODE_ASSERT(WDGM_SID_MAIN_FUNCTION);
        break;
        /* CHECK: PARSE*/
      }

#if (WDGM_EB_DEADLINE_SUPERVISION_ENABLED == STD_ON)
      if(DeadlineResult != WDGM_EB_RESULT_CORRECT)
      {
        /* !LINKSTO WDGM.ASR40.WDGM202,1,WDGM.EB.ASR40.WDGM206,1,WDGM.EB.TIMEPM.WDGM020448,1 */
        NewLocalStatus = NextFailedStatus;
      }
#endif

#if (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_ON)
      if(LogicalResult != WDGM_EB_RESULT_CORRECT)
      {
        /* !LINKSTO WDGM.ASR40.WDGM202,1,WDGM.EB.ASR40.WDGM206,1,WDGM.EB.TIMEPM.WDGM020448,1 */
        NewLocalStatus = NextFailedStatus;
      }
#endif

      if (NewLocalStatus == WDGM_LOCAL_STATUS_EXPIRED)
      {
        WdgM_EB_ExpiredSEIDType ExpiredSEID = WDGM_EB_ATOMIC_LOAD(WdgM_EB_ExpiredSEID);
        /* The first SEId has been saved by alive supervision.
         *  Now we know: it lead to EXPIRED, mark as such */
        /* !LINKSTO WDGM.ASR40.WDGM351,1 */
        WdgM_EB_StoreExpiredSEID (&ExpiredSEID, SECfg->SEId);
        WDGM_EB_ATOMIC_STORE(WdgM_EB_ExpiredSEID, ExpiredSEID);
      }
      break;
    }

    /* !LINKSTO WDGM.ASR40.WDGM201,1 */
    /* If AliveResult / LogicalResult / DeadlineResult do not equal WDGM_EB_RESULT_INCORRECT,
     * then status WDGM_LOCAL_STATUS_OK is maintained. */

    case WDGM_LOCAL_STATUS_EXPIRED:
      /* nothing to do here, the expired state cannot be left */
      break;

      /* CHECK: NOPARSE */
    default:
      /* !LINKSTO WDGM.ASR40.WDGM208,1 */
      /* Note: WDGM_LOCAL_STATUS_DEACTIVATED shall never be reached here */
      WDGM_UNREACHABLE_CODE_ASSERT(WDGM_SID_MAIN_FUNCTION);
      break;
      /* CHECK: PARSE*/
  }

  /* perform mode switch indication if the local status changed */
  if (WDGM_EB_ATOMIC_LOAD(*SEWData_SELocalStatus) != NewLocalStatus)
  {
    WdgM_EB_LocalStatusType OldLocalStatus = WDGM_EB_ATOMIC_LOAD(*SEWData_SELocalStatus);
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
    WdgM_LocalStatusType LocalOldLocalStatus = (WdgM_LocalStatusType)(OldLocalStatus & (WdgM_LocalStatusType)0xFFU);
    WdgM_LocalStatusType LocalNewLocalStatus = (WdgM_LocalStatusType)(NewLocalStatus & (WdgM_LocalStatusType)0xFFU);
#endif

    WDGM_EB_ATOMIC_STORE(*SEWData_SELocalStatus, NewLocalStatus);

    /* !LINKSTO WDGM.ASR40.WDGM197,1,WdgM.ASR43.SWS_WdgM_00197,1 */
    /* signal via callout and/or RTE */
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
    WDGM_EB_INDIVIDUAL_MODE_SWITCH(SECfg->SEId, LocalOldLocalStatus, LocalNewLocalStatus, SECfg->CoreID);
#else
    WDGM_EB_INDIVIDUAL_MODE_SWITCH(SECfg->SEId, OldLocalStatus, NewLocalStatus, 0U);
#endif
    /* if new state is expired, explicitly signal this via callout/RTE */
    if (WDGM_LOCAL_STATUS_EXPIRED == NewLocalStatus)
    {
      WDGM_EB_SUPERVISION_EXPIRED(SECfg->SEId);
    }
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_SETSELOCALSTATUS_EXIT(IntSEId,IntCoreSEId,AliveResult,DeadlineResult,LogicalResult,CoreInstance);
}

/*------------------[WdgM_EB_SetGlobalStatus]----------------------------------------------------*/
static FUNC(void, WDGM_CODE) WdgM_EB_SetGlobalStatus
(
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
)
{
  const WdgM_EB_GlobalStatusType OldGlobalStatus = WDGM_EB_ATOMIC_LOAD(WdgM_EB_GlobalStatus);
  const WdgM_EB_GlobalStatusType NewGlobalStatus = WdgM_EB_GetNewGlobalStatus(CoreInstance);
#if (WDGM_EB_MULTICORE_ENABLED == STD_ON)
  const WdgM_GlobalStatusType LocalOldGlobalStatus = (WdgM_GlobalStatusType)(OldGlobalStatus & (WdgM_GlobalStatusType)0xFFU);
  const WdgM_GlobalStatusType LocalNewGlobalStatus = (WdgM_GlobalStatusType)(NewGlobalStatus & (WdgM_GlobalStatusType)0xFFU);
#endif

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_SETGLOBALSTATUS_ENTRY(CoreInstance);

  if (OldGlobalStatus != NewGlobalStatus)
  {
    /* assign the new global status */
    /* !LINKSTO WDGM.EB.TIMEPM.WDGM020463,1 */
    WDGM_EB_ATOMIC_STORE(WdgM_EB_GlobalStatus, NewGlobalStatus);

    if (NewGlobalStatus == WDGM_GLOBAL_STATUS_STOPPED)
    {
      /* !LINKSTO WDGM.ASR40.WDGM129,1,WDGM.ASR40.WDGM004.11,1 */
#if (WDGM_DEM_ALIVE_SUPERVISION_REPORT == STD_ON)
# if (WDGM_EB_PROD_ERR_HANDLING_SUPERVISION == WDGM_PROD_ERR_REP_TO_DEM)
      WDGM_EB_DEM(WDGM_EB_SUPERVISION_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
# elif (WDGM_EB_PROD_ERR_HANDLING_SUPERVISION == WDGM_PROD_ERR_REP_TO_DET)
      WDGM_EB_DET(WDGM_SID_MAIN_FUNCTION, WDGM_EB_E_DEMTODET_SUPERVISION);
# endif
#endif /* (WDGM_DEM_ALIVE_SUPERVISION_REPORT == STD_OFF) */
      /* !LINKSTO WDGM.ASR40.WDGM133.1,1 */
#if (WDGM_IMMEDIATE_RESET == STD_ON)
      Mcu_PerformReset();
#else
      /* !LINKSTO WDGM.ASR40.WDGM134,1 */
      /* !LINKSTO WDGM.ASR40.WDGM198,1,WdgM.ASR43.SWS_WdgM_00198,1 */
      /* signal mode change to stopped via callout and/or RTE
       * only if immediate reset is not performed*/
#if (WDGM_EB_MULTICORE_ENABLED == STD_ON)
      WDGM_EB_GLOBAL_MODE_SWITCH(LocalOldGlobalStatus, LocalNewGlobalStatus);
#else
      WDGM_EB_GLOBAL_MODE_SWITCH(OldGlobalStatus, NewGlobalStatus);
#endif
#endif
    }
    else
    {
      /* !LINKSTO WDGM.ASR40.WDGM198,1,WdgM.ASR43.SWS_WdgM_00198,1 */
      /* signal mode change via callout and/or RTE */
#if (WDGM_EB_MULTICORE_ENABLED == STD_ON)
      WDGM_EB_GLOBAL_MODE_SWITCH(LocalOldGlobalStatus, LocalNewGlobalStatus);
#else
      WDGM_EB_GLOBAL_MODE_SWITCH(OldGlobalStatus, NewGlobalStatus);
#endif
    }
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_SETGLOBALSTATUS_EXIT(CoreInstance);
}

/*------------------[WdgM_EB_GetNewGlobalStatus]-------------------------------------------------*/
static FUNC(WdgM_EB_GlobalStatusType, WDGM_CODE) WdgM_EB_GetNewGlobalStatus
(
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
)
{
  const WdgM_EB_GlobalStatusType OldGlobalStatus = WDGM_EB_ATOMIC_LOAD(WdgM_EB_GlobalStatus);
  WdgM_EB_SEIdType IntSEId = 0U;
  WdgM_EB_SEIdType NumSEFailed = 0U;
  WdgM_EB_SEIdType NumSEExpired = 0U;
  WdgM_EB_GlobalStatusType NewGlobalStatus = WDGM_EB_ATOMIC_LOAD(WdgM_EB_GlobalStatus);
  CONSTP2CONST(WdgM_EB_ModeCfgType,AUTOMATIC,WDGM_CONST) ModeCfg =
      &(WdgM_EB_ModeCfg[CoreInstance->WdgM_EB_CurrentModeId]);

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_GETNEWGLOBALSTATUS_ENTRY(CoreInstance);

  /* go through the SEs and count the local status */
#if (WDGM_EB_SE_NUM != 1U)
  for (IntSEId = 0U; IntSEId < WDGM_EB_SE_NUM; IntSEId++)
#endif
  {
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
    /* get the configuration for the current SE */
    CONSTP2CONST(WdgM_EB_SECfgType,AUTOMATIC,WDGM_CONST) SECfg = &(WdgM_EB_SECfg[IntSEId]);
    /* During the mode switch, Global Status is calculated using only the local statuses
     *of the SEs, from the cores which completed the mode switch. */
    /* !LINKSTO WDGM.EB.MulticoreGlobalStatus2,1 */
    if ( WDGM_EB_ATOMIC_LOAD(WdgM_EB_CoreSwitchMode[SECfg->CoreID]) ==
         WDGM_EB_ATOMIC_LOAD(WdgM_EB_CoreSwitchMode[WDGM_EB_MASTER_INSTANCE]) )
#endif
    {
      switch (WDGM_EB_ATOMIC_LOAD(CoreInstance->WdgM_EB_SELocalStatus[IntSEId]))
      {
        case WDGM_LOCAL_STATUS_FAILED:
          NumSEFailed++;
          break;
        case WDGM_LOCAL_STATUS_EXPIRED:
          NumSEExpired++;
          break;
        case WDGM_LOCAL_STATUS_OK: /* fall through: no actions for OK and DEACTIVATED*/
        case WDGM_LOCAL_STATUS_DEACTIVATED:
          break;
          /* CHECK: NOPARSE */
        default:
          WDGM_UNREACHABLE_CODE_ASSERT(WDGM_SID_MAIN_FUNCTION);
          break;
          /* CHECK: PARSE*/
      }
    }
  }

  switch (OldGlobalStatus)
  {
    case WDGM_GLOBAL_STATUS_OK: /* fall-through */
    case WDGM_GLOBAL_STATUS_FAILED:
    {
      /* !LINKSTO WDGM.ASR40.WDGM076,1 */
      if (NumSEExpired == 0U)
      {
        if (NumSEFailed > 0U)
        {
          /* !LINKSTO WDGM.ASR40.WDGM217,1 */
          NewGlobalStatus = WDGM_GLOBAL_STATUS_FAILED;
        }
        else
        {
          /* !LINKSTO WDGM.ASR40.WDGM078,1,WDGM.ASR40.WDGM218,1 */
          NewGlobalStatus = WDGM_GLOBAL_STATUS_OK;
        }
      }
      else
      {
        /* !LINKSTO WDGM.ASR40.WDGM215,1,WDGM.EB.ASR40.WDGM077,1 */
        if (ModeCfg->ExpiredSupervisionCycleTol > 0U)
        {
          NewGlobalStatus = WDGM_GLOBAL_STATUS_EXPIRED;
          WdgM_EB_ExpiredSupervisionCycleCnt = 1U;
        }
        /* !LINKSTO WDGM.ASR40.WDGM216,1,WDGM.ASR40.WDGM117,1 */
        else
        {
          NewGlobalStatus = WDGM_GLOBAL_STATUS_STOPPED;
        }
      }
      break;
    }

    case WDGM_GLOBAL_STATUS_EXPIRED:
    {
      /* !LINKSTO WDGM.EB.ASR40.WDGM219,1,WDGM.EB.ASR40.WDGM220,1 */
      /* Note: Overlapping counter value is not problematic because the STOPPED state is final. */
      WdgM_EB_ExpiredSupervisionCycleCnt++;
      if(WdgM_EB_ExpiredSupervisionCycleCnt > ModeCfg->ExpiredSupervisionCycleTol)
      {
        NewGlobalStatus = WDGM_GLOBAL_STATUS_STOPPED;
      }
      break;
    }

    case WDGM_GLOBAL_STATUS_STOPPED:
      /* !LINKSTO WDGM.ASR40.WDGM221,1 */
      /* remain in status STOPPED */
      break;

      /* CHECK: NOPARSE */
    default:
      /* Note: WDGM_GLOBAL_STATUS_DEACTIVATED shall never be reached here */
      WDGM_UNREACHABLE_CODE_ASSERT(WDGM_SID_MAIN_FUNCTION);
      break;
      /* CHECK: PARSE*/
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_GETNEWGLOBALSTATUS_EXIT(NewGlobalStatus,CoreInstance);
  return NewGlobalStatus;
}

/*------------------[WdgM_EB_UpdateSupervisionCycleCounter]--------------------------------------*/
static FUNC(void, WDGM_CODE) WdgM_EB_UpdateSupervisionCycleCounter
(
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
)
{
  CONSTP2CONST(WdgM_EB_ModeCfgType,AUTOMATIC,WDGM_CONST) ModeCfg =
      &(WdgM_EB_ModeCfg[CoreInstance->WdgM_EB_CurrentModeId]);

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_UPDATESUPERVISIONCYCLECOUNTER_ENTRY(CoreInstance);

  CoreInstance->WdgM_EB_SupervisionCycleCnt++;
  CoreInstance->WdgM_EB_SupervisionCycleCnt %= ModeCfg->MaxSupervisionCycleCounter;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_UPDATESUPERVISIONCYCLECOUNTER_EXIT(CoreInstance);
}

#if (WDGM_EB_CHECK_MAINFUNCTION_TIMING == STD_ON)
/*------------------[WdgM_EB_CheckMainFunctionTimingViolation]-----------------------------------*/
static FUNC(void, WDGM_CODE) WdgM_EB_CheckMainFunctionTimingViolation
(
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
)
{
  WdgM_EB_TimestampType ElapsedTime = 0U;
  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_CHECKMAINFUNCTIONTIMINGVIOLATION_ENTRY(CoreInstance);

  /* first calculate elapsed time and update actual time stamp */
  WDGM_EB_GET_TIME(&(CoreInstance->WdgM_EB_MainFunctionTimeStamp), &ElapsedTime);

  /* elapsed time is only valid if WdgM is initialized.
   * In this case, check if deadline violation occurred */
  if (WDGM_EB_CHECK_INIT(WdgM_EB_GlobalInitStatus, CoreInstance->CoreID) != WDGM_EB_GLOBAL_STATUS_DEINIT)
  {
    const WdgM_EB_TimestampType MFDeadlineMin = (WdgM_EB_TimestampType)
        (WDGM_EB_DM_MAINFUNCTIONCYCLETIME - WDGM_EB_DM_MAINFUNCTIONTOLERANCETIME);
    const WdgM_EB_TimestampType MFDeadlineMax = (WdgM_EB_TimestampType)
        (WDGM_EB_DM_MAINFUNCTIONCYCLETIME + WDGM_EB_DM_MAINFUNCTIONTOLERANCETIME);
    if ((ElapsedTime < MFDeadlineMin) || (ElapsedTime > MFDeadlineMax))
    {
      /* !LINKSTO WDGM.EB.TIMEPM.WDGM020454,1,WDGM.EB.TIMEPM.WDGM020453,1 */
#if (WDGM_EB_PROD_ERR_HANDLING_MF_TIMINGVIOLATION == WDGM_PROD_ERR_REP_TO_DEM)
      WDGM_EB_DEM(WDGM_EB_MF_TIMINGVIOLATION_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#elif (WDGM_EB_PROD_ERR_HANDLING_MF_TIMINGVIOLATION == WDGM_PROD_ERR_REP_TO_DET)
      WDGM_EB_DET(WDGM_SID_MAIN_FUNCTION, WDGM_EB_E_DEMTODET_MF_TIMINGVIOLATION);
#endif
    }
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_CHECKMAINFUNCTIONTIMINGVIOLATION_EXIT(CoreInstance);
}
#endif

/*==================[internal helper function definitions]=======================================*/

#if (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_ON)
/*------------------[WdgM_EB_GetCheckedGraphData]------------------------------------------------*/
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_GetCheckedGraphData
(
  P2CONST(WdgM_EB_GraphDataType,AUTOMATIC,WDGM_VAR) GraphData,
  P2VAR(WdgM_EB_SEIdType,AUTOMATIC,AUTOMATIC) LastIntSEId,
  P2VAR(WdgM_EB_CPIdType,AUTOMATIC,AUTOMATIC) LastCPId,
  P2VAR(boolean,AUTOMATIC,AUTOMATIC) ActiveFlag
)
{
  Std_ReturnType RetVal;
  const uint16 UncheckedIsActiveData =
      (uint16)WDGM_EB_ATOMIC_LOAD(GraphData->IsActiveData);
  const uint32 UncheckedLastCPData =
      (uint32)WDGM_EB_ATOMIC_LOAD(GraphData->LastCPData);
  const uint8 CheckedIsActiveData =
      (uint8)(UncheckedIsActiveData & (uint8)0xFFU);

  const uint8 IsActiveDataRedundant =
      (uint8)(~((uint8)((uint16)(UncheckedIsActiveData >> 8U) & (uint8)0xFFU)));
  const uint8 CheckedLastSEId =
      (uint8)(UncheckedLastCPData & (uint8)0xFFU);
  const uint8 LastSEIdRedundant =
      (uint8)(~((uint8)((uint32)(UncheckedLastCPData >> 8U) & (uint8)0xFFU)));
  const uint8 CheckedLastCPId =
      (uint8)((uint32)(UncheckedLastCPData >> 16U) & (uint8)0xFFU);
  const uint8 LastCPIdRedundant =
      (uint8)(~((uint8)((uint32)(UncheckedLastCPData >> 24U) & (uint8)0xFFU)));

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_GETCHECKEDGRAPHDATA_ENTRY(GraphData,LastIntSEId,LastCPId,ActiveFlag);

  if (((CheckedIsActiveData == IsActiveDataRedundant) &&
       (CheckedLastSEId == LastSEIdRedundant)) &&
      (CheckedLastCPId == LastCPIdRedundant))
  {
    RetVal = E_OK;
    *LastIntSEId = CheckedLastSEId;
    *LastCPId = CheckedLastCPId;
    *ActiveFlag = CheckedIsActiveData;
  }
  else
  {
    /* !LINKSTO WDGM.EB.TIMEPM.WDGM020470,2,WDGM.EB.TIMEPM.WDGM020471,1 */
    RetVal = E_NOT_OK;
    /* !LINKSTO WDGM.EB.TIMEPM.WDGM020466,1,WDGM.EB.TIMEPM.WDGM020468,1 */
#if (WDGM_EB_PROD_ERR_HANDLING_DATA_CORRUPTION == WDGM_PROD_ERR_REP_TO_DEM)
    WDGM_EB_DEM(WDGM_EB_DATA_CORRUPTION_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#elif (WDGM_EB_PROD_ERR_HANDLING_DATA_CORRUPTION == WDGM_PROD_ERR_REP_TO_DET)
    WDGM_EB_DET(WDGM_SID_CHECKPOINT_REACHED, WDGM_EB_E_DEMTODET_DATA_CORRUPTION);
#endif
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_GETCHECKEDGRAPHDATA_EXIT(RetVal,GraphData,LastIntSEId,LastCPId,ActiveFlag);
  return RetVal;
}

/*------------------[WdgM_EB_StoreGraphLastCP]---------------------------------------------------*/
static FUNC(void, WDGM_CODE) WdgM_EB_StoreGraphData
(
  P2VAR(WdgM_EB_GraphDataType,AUTOMATIC,WDGM_VAR) GraphData,
  WdgM_EB_SEIdType IntSEId,
  WdgM_EB_CPIdType CPId,
  boolean IsActive
)
{
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
  Atomic_value_t LastCPData;
  Atomic_value_t IsActiveData;
#else
  uint32 LastCPData;
  uint16 IsActiveData;
#endif
  const WdgM_EB_SEIdType IntSEIdRedundant = (WdgM_EB_SEIdType)(~IntSEId);
  const WdgM_EB_CPIdType CPIdRedundant = (WdgM_EB_CPIdType)(~CPId);
  const uint8 IsActiveDataUint8 = IsActive;
  const uint8 IsActiveDataRedundant = (uint8)(~IsActiveDataUint8);

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_STOREGRAPHDATA_ENTRY(GraphData,IntSEId,CPId,IsActive);

  LastCPData = IntSEId;
  LastCPData |= (uint32)((uint32)IntSEIdRedundant << 8U);
  LastCPData |= (uint32)(((uint32)CPId) << 16U);
  LastCPData |= (uint32)(((uint32)CPIdRedundant) << 24U);
  IsActiveData = IsActiveDataUint8;
  IsActiveData |= (uint16)(((uint16)IsActiveDataRedundant) << 8U);

  WDGM_EB_ATOMIC_STORE(GraphData->LastCPData, LastCPData);
  WDGM_EB_ATOMIC_STORE(GraphData->IsActiveData, IsActiveData);

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_STOREGRAPHDATA_EXIT(GraphData,IntSEId,CPId,IsActive);
}

#if (WDGM_EB_EXTERNAL_GRAPH_NUM > 0U)
/*------------------[WdgM_EB_GetCheckedSeqNumber]------------------------------------------------*/
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_GetCheckedSeqNumber
(
  P2CONST(WdgM_EB_ExtGraphDataSeqArrayType,AUTOMATIC,WDGM_VAR) IntExtGraphDataSeqPtr,
  CONST(uint8,AUTOMATIC) IndexInGraphDataSeq,
  P2VAR(uint8,AUTOMATIC,AUTOMATIC) LastSeqNumber
)
{
  Std_ReturnType RetVal;
#if (WDGM_EB_MULTICORE_ENABLED == STD_ON)
  const Atomic_value_t UncheckedLastSeqNumber = WDGM_EB_ATOMIC_LOAD(IntExtGraphDataSeqPtr[IndexInGraphDataSeq]);
#else
  const uint16 UncheckedLastSeqNumber = WDGM_EB_ATOMIC_LOAD(IntExtGraphDataSeqPtr[IndexInGraphDataSeq]);
#endif
  const uint8 CheckedLastSeqNumber = (uint8)(UncheckedLastSeqNumber & (uint8)0xFFU);
  const uint8 LastSeqNumberRedundant = (uint8)(~((uint8)((uint16)(UncheckedLastSeqNumber >> 8U) & (uint8)0xFFU)));

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_GETCHECKEDSEQNUMBER_ENTRY(IntExtGraphDataSeqPtr,IndexInGraphDataSeq,LastSeqNumber);

  if (CheckedLastSeqNumber == LastSeqNumberRedundant)
  {
    RetVal = E_OK;
    *LastSeqNumber = CheckedLastSeqNumber;
  }
  else
  {
    /* !LINKSTO WDGM.EB.TIMEPM.WDGM020470,2,WDGM.EB.TIMEPM.WDGM020471,1 */
    RetVal = E_NOT_OK;
    /* !LINKSTO WDGM.EB.TIMEPM.WDGM020466,1,WDGM.EB.TIMEPM.WDGM020468,1 */
#if (WDGM_EB_PROD_ERR_HANDLING_DATA_CORRUPTION == WDGM_PROD_ERR_REP_TO_DEM)
    WDGM_EB_DEM(WDGM_EB_DATA_CORRUPTION_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#elif (WDGM_EB_PROD_ERR_HANDLING_DATA_CORRUPTION == WDGM_PROD_ERR_REP_TO_DET)
    WDGM_EB_DET(WDGM_SID_CHECKPOINT_REACHED, WDGM_EB_E_DEMTODET_DATA_CORRUPTION);
#endif
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_GETCHECKEDSEQNUMBER_EXIT(RetVal,IntExtGraphDataSeqPtr,IndexInGraphDataSeq,LastSeqNumber);
  return RetVal;
}

/*------------------[WdgM_EB_StoreSeqNumber]---------------------------------------------------*/
static FUNC(void, WDGM_CODE) WdgM_EB_StoreSeqNumber
(
  P2VAR(WdgM_EB_ExtGraphDataSeqArrayType,AUTOMATIC,WDGM_VAR) IntExtGraphDataSeqPtr,
  CONST(uint8,AUTOMATIC) IndexInGraphDataSeq,
  CONST(uint8,AUTOMATIC) SeqNumber
)
{
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
  Atomic_value_t LastSeqNumber;
#else
  uint16 LastSeqNumber;
#endif
  const uint8 SeqNumberRedundant = (WdgM_EB_SEIdType)(~SeqNumber);


  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_STORESEQNUMBER_ENTRY(IntExtGraphDataSeqPtr,IndexInGraphDataSeq,SeqNumber);

  LastSeqNumber = SeqNumber;
  LastSeqNumber |= (uint16)((uint16)SeqNumberRedundant << 8U);

  WDGM_EB_ATOMIC_STORE(IntExtGraphDataSeqPtr[IndexInGraphDataSeq], LastSeqNumber);

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_STORESEQNUMBER_EXIT(IntExtGraphDataSeqPtr,IndexInGraphDataSeq,SeqNumber);
}
#endif /* #if (WDGM_EB_EXTERNAL_GRAPH_NUM > 0U) */
#endif /* #if (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_ON) */

/*------------------[WdgM_EB_CheckCPAliveCounterData]--------------------------------------------*/
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_CheckCPAliveCounterData
(
  P2CONST(WdgM_EB_CPDataType,AUTOMATIC,WDGM_VAR) CPData
)
{
  Std_ReturnType RetVal;
  const uint32 UncheckedAliveCounterData = CPData->AliveCounterData;
  const WdgM_EB_AliveCounterType CheckedAliveCounterData =
      (WdgM_EB_AliveCounterType)(UncheckedAliveCounterData & (uint16)0xFFFFU);
  const WdgM_EB_AliveCounterType AliveCounterDataRedundant =
      (WdgM_EB_AliveCounterType)(~((WdgM_EB_AliveCounterType)
          ((uint32)(UncheckedAliveCounterData >> 16U) & (uint16)0xFFFFU)));

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_CHECKCPALIVECOUNTERDATA_ENTRY(CPData);

  if (CheckedAliveCounterData == AliveCounterDataRedundant)
  {
    RetVal = E_OK;
  }
  else
  {
    RetVal = E_NOT_OK;
    /* !LINKSTO WDGM.EB.TIMEPM.WDGM020466,1,WDGM.EB.TIMEPM.WDGM020468,1 */
#if (WDGM_EB_PROD_ERR_HANDLING_DATA_CORRUPTION == WDGM_PROD_ERR_REP_TO_DEM)
    WDGM_EB_DEM(WDGM_EB_DATA_CORRUPTION_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#elif (WDGM_EB_PROD_ERR_HANDLING_DATA_CORRUPTION == WDGM_PROD_ERR_REP_TO_DET)
    WDGM_EB_DET(WDGM_SID_CHECKPOINT_REACHED, WDGM_EB_E_DEMTODET_DATA_CORRUPTION);
#endif
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_CHECKCPALIVECOUNTERDATA_EXIT(RetVal,CPData);
  return RetVal;
}

/*------------------[WdgM_EB_IncrementCPAliveCounterData]----------------------------------------*/
static FUNC(void, WDGM_CODE) WdgM_EB_IncrementCPAliveCounterData
(
  P2VAR(WdgM_EB_CPDataType,AUTOMATIC,WDGM_VAR) CPData
)
{
  const WdgM_EB_AliveCounterType AliveCounter =
      (WdgM_EB_AliveCounterType)((WdgM_EB_AliveCounterType)(CPData->AliveCounterData) + 1U);
  const WdgM_EB_AliveCounterType AliveCounterRedundant = (WdgM_EB_AliveCounterType)(~AliveCounter);
  uint32 AliveCounterData;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_INCREMENTCPALIVECOUNTERDATA_ENTRY(CPData);

  AliveCounterData = AliveCounter;
  AliveCounterData |= (uint32)(((uint32)AliveCounterRedundant) << 16U);

  CPData->AliveCounterData = AliveCounterData;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_INCREMENTCPALIVECOUNTERDATA_EXIT(CPData);
}

/*------------------[WdgM_EB_ResetCPAliveCounterData---------------------------------------------*/
static FUNC(void, WDGM_CODE) WdgM_EB_ResetCPAliveCounterData
(
  WdgM_EB_IntRamIdxType IntRamIdx,
  WdgM_EB_AliveCounterType OldAliveCounter,
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
)
{
  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_RESETCPALIVECOUNTERDATA_ENTRY(IntRamIdx,OldAliveCounter,CoreInstance);

  /* reset of alive counter means that alive counter data of local RAM data
   * equals alive counter of checkpoint data (CPData) */
  CoreInstance->WdgM_EB_IntRAMData[IntRamIdx].AliveCounterData += OldAliveCounter;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_RESETCPALIVECOUNTERDATA_EXIT(IntRamIdx,OldAliveCounter,CoreInstance);
}

/*------------------[WdgM_EB_GetCheckedCPAliveCounterData]---------------------------------------*/
static FUNC(Std_ReturnType,WDGM_CODE) WdgM_EB_GetCheckedCPAliveCounterData
(
  P2CONST(WdgM_EB_CPDataType,AUTOMATIC,WDGM_VAR) CPData,
  WdgM_EB_IntRamIdxType IntRamIdx,
  P2VAR(WdgM_EB_AliveCounterType,AUTOMATIC,AUTOMATIC) CPAliveCounter,
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
)
{
  Std_ReturnType RetVal;

  const uint32 UncheckedAliveCounterData = CPData->AliveCounterData;
  const WdgM_EB_AliveCounterType CheckedAliveCounterData =
      (WdgM_EB_AliveCounterType)(UncheckedAliveCounterData & (uint16)0xFFFFU);
  const WdgM_EB_AliveCounterType AliveCounterDataRedundant =
      (WdgM_EB_AliveCounterType)(~((WdgM_EB_AliveCounterType)
          ((uint32)(UncheckedAliveCounterData >> 16U) & (uint16)0xFFFFU)));
  const WdgM_EB_AliveCounterType AliveCounterReference =
      CoreInstance->WdgM_EB_IntRAMData[IntRamIdx].AliveCounterData;
  WdgM_EB_AliveCounterType AliveCounterData = CheckedAliveCounterData;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_GETCHECKEDCPALIVECOUNTERDATA_ENTRY(CPData,IntRamIdx,CPAliveCounter,CoreInstance);

  if (CheckedAliveCounterData == AliveCounterDataRedundant)
  {
    RetVal = E_OK;
  }
  else
  {
    /* !LINKSTO WDGM.EB.TIMEPM.WDGM020470,2,WDGM.EB.TIMEPM.WDGM020472,1 */
    RetVal = E_NOT_OK;
    /* !LINKSTO WDGM.EB.TIMEPM.WDGM020466,1,WDGM.EB.TIMEPM.WDGM020467,1 */
#if (WDGM_EB_PROD_ERR_HANDLING_DATA_CORRUPTION == WDGM_PROD_ERR_REP_TO_DEM)
    WDGM_EB_DEM(WDGM_EB_DATA_CORRUPTION_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#elif (WDGM_EB_PROD_ERR_HANDLING_DATA_CORRUPTION == WDGM_PROD_ERR_REP_TO_DET)
    WDGM_EB_DET(WDGM_SID_MAIN_FUNCTION, WDGM_EB_E_DEMTODET_DATA_CORRUPTION);
#endif
  }

  /* independent of success state, increment alive counter */
  if (AliveCounterData >= AliveCounterReference)
  {
    AliveCounterData -= AliveCounterReference;
  }
  else
  {
    const uint32 AliveCounterDataTemp =
        (uint32)((uint32)((uint32)WDGM_EB_MAX_ALIVE_COUNTER_VALUE + (uint32)1UL)
            - (uint32)AliveCounterReference);
    AliveCounterData += (WdgM_EB_AliveCounterType)
        (AliveCounterDataTemp & (WdgM_EB_AliveCounterType)0xFFFFU);
  }

  *CPAliveCounter = AliveCounterData;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_GETCHECKEDCPALIVECOUNTERDATA_EXIT(RetVal,CPData,IntRamIdx,CPAliveCounter,CoreInstance);
  return RetVal;
}

#if (WDGM_EB_DEADLINE_SUPERVISION_ENABLED == STD_ON)
/*------------------[WdgM_EB_GetCheckedDMData]---------------------------------------------------*/
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_GetCheckedDMData
(
  P2CONST(WdgM_EB_DMDataType,AUTOMATIC,WDGM_VAR) DMData,
  P2VAR(WdgM_EB_TimestampType,AUTOMATIC,AUTOMATIC) TimeStamp,
  P2VAR(uint16,AUTOMATIC,AUTOMATIC) MaxCycles,
  P2VAR(boolean,AUTOMATIC,AUTOMATIC) ActiveFlag
)
{
  Std_ReturnType RetVal;

  const uint16 UncheckedIsActiveData =
      DMData->IsActiveData;
  const uint32 UncheckedMaxCyclesData =
      DMData->MaxCyclesData;
  const WdgM_EB_TimestampType CheckedTimeStampData =
      DMData->StartTimestampData;

  const WdgM_EB_TimestampType TimeStampDataRedundant =
      (uint32)(~(DMData->StartTimestampDataInv));
  const uint8 CheckedIsActiveData =
      (uint8)(UncheckedIsActiveData & (uint8)0xFFU);
  const uint8 IsActiveDataRedundant =
      (uint8)(~((uint8)((uint16)(UncheckedIsActiveData >> 8U) & (uint8)0xFFU)));
  const uint16 CheckedMaxCyclesData =
      (uint16)(UncheckedMaxCyclesData & (uint16)0xFFFFU);
  const uint16 MaxCyclesDataRedundant =
      (uint16)(~((uint16)((uint32)(UncheckedMaxCyclesData >> 16U) & (uint16)0xFFFFU)));

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_GETCHECKEDDMDATA_ENTRY(DMData,TimeStamp,MaxCycles,ActiveFlag);

  if (((CheckedIsActiveData == IsActiveDataRedundant) &&
       (CheckedMaxCyclesData == MaxCyclesDataRedundant)) &&
       (CheckedTimeStampData == TimeStampDataRedundant))
  {
    RetVal = E_OK;
    *TimeStamp = CheckedTimeStampData;
    *MaxCycles = CheckedMaxCyclesData;
    *ActiveFlag = CheckedIsActiveData;
  }
  else
  {
    /* !LINKSTO WDGM.EB.TIMEPM.WDGM020470,2,WDGM.EB.TIMEPM.WDGM020471,1 */
    RetVal = E_NOT_OK;
    /* !LINKSTO WDGM.EB.TIMEPM.WDGM020466,1,WDGM.EB.TIMEPM.WDGM020468,1 */
#if (WDGM_EB_PROD_ERR_HANDLING_DATA_CORRUPTION == WDGM_PROD_ERR_REP_TO_DEM)
    WDGM_EB_DEM(WDGM_EB_DATA_CORRUPTION_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#elif (WDGM_EB_PROD_ERR_HANDLING_DATA_CORRUPTION == WDGM_PROD_ERR_REP_TO_DET)
    WDGM_EB_DET(WDGM_SID_CHECKPOINT_REACHED, WDGM_EB_E_DEMTODET_DATA_CORRUPTION);
#endif
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_GETCHECKEDDMDATA_EXIT(RetVal,DMData,TimeStamp,MaxCycles,ActiveFlag);
  return RetVal;
}

/*------------------[WdgM_EB_StoreDMData]--------------------------------------------------------*/
static FUNC(void, WDGM_CODE) WdgM_EB_StoreDMData
(
  P2VAR(WdgM_EB_DMDataType,AUTOMATIC,WDGM_VAR) DMData,
  P2CONST(WdgM_EB_TimestampType,AUTOMATIC,AUTOMATIC) TimeStamp,
  uint16 MaxCycles,
  boolean IsActive
)
{
  uint16 IsActiveData;
  uint32 MaxCyclesData;

  const uint8 IsActiveDataUint8 = IsActive;
  const uint8 IsActiveDataRedundant = (uint8)(~IsActiveDataUint8);
  const uint16 MaxCyclesRedundant = (uint16)(~MaxCycles);
  const WdgM_EB_TimestampType TimeStampDataRedundant =(WdgM_EB_TimestampType)(~(*TimeStamp));

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_STOREDMDATA_ENTRY(DMData,TimeStamp,MaxCycles,IsActive);

  IsActiveData = IsActiveDataUint8;
  IsActiveData |= (uint16)(((uint16)IsActiveDataRedundant) << 8U);
  MaxCyclesData = MaxCycles;
  MaxCyclesData |= (uint32)(((uint32)MaxCyclesRedundant) << 16U);

  DMData->StartTimestampData = *TimeStamp;
  DMData->StartTimestampDataInv = TimeStampDataRedundant;
  DMData->MaxCyclesData = MaxCyclesData;
  DMData->IsActiveData = IsActiveData;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_STOREDMDATA_EXIT(DMData,TimeStamp,MaxCycles,IsActive);
}

/*------------------[WdgM_EB_DecMaxCyclesDMData]-------------------------------------------------*/
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_DecMaxCyclesDMData
(
  P2VAR(WdgM_EB_DMDataType,AUTOMATIC,WDGM_VAR) DMData
)
{
  Std_ReturnType RetVal;

  const uint16 UncheckedIsActiveData =
      DMData->IsActiveData;
  const uint32 UncheckedMaxCyclesData =
      DMData->MaxCyclesData;

  const uint8 CheckedIsActiveData =
      (uint8)(UncheckedIsActiveData & (uint8)0xFFU);
  const uint8 IsActiveDataRedundant =
      (uint8)(~((uint8)((uint16)(UncheckedIsActiveData >> 8U) & (uint8)0xFFU)));
  const uint16 CheckedMaxCyclesData =
      (uint16)(UncheckedMaxCyclesData & (uint16)0xFFFFU);
  const uint16 MaxCyclesDataRedundant =
      (uint16)(~((uint16)((uint32)(UncheckedMaxCyclesData >> 16U) & (uint16)0xFFFFU)));

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_DECMAXCYCLESDMDATA_ENTRY(DMData);

  if ((CheckedIsActiveData == IsActiveDataRedundant) &&
      (CheckedMaxCyclesData == MaxCyclesDataRedundant))
  {
    /* This CP is an active start CP for Deadline Monitoring */
    if (CheckedIsActiveData != FALSE)
    {
      /* check for deadline violation */
      /* !LINKSTO WDGM.EB.TIMEPM.WDGM020455,1 */
      if (CheckedMaxCyclesData > 0U)
      {
        uint32 MaxCyclesData;
        const uint16 MaxCycles = CheckedMaxCyclesData - 1U;
        const uint16 MaxCyclesRedundant = (uint16)(~MaxCycles);

        MaxCyclesData = MaxCycles;
        MaxCyclesData |= (uint32)(((uint32)MaxCyclesRedundant) << 16U);
        DMData->MaxCyclesData = MaxCyclesData;

        RetVal = E_OK;
      }
      else
      {
        /* !LINKSTO WDGM.EB.TIMEPM.WDGM020456,1 */
        RetVal = E_NOT_OK;
      }
    }
    else
    {
      RetVal = E_OK;
    }
  }
  else
  {
    /* !LINKSTO WDGM.EB.TIMEPM.WDGM020470,2,WDGM.EB.TIMEPM.WDGM020472,1 */
    RetVal = E_NOT_OK;
    /* !LINKSTO WDGM.EB.TIMEPM.WDGM020466,1,WDGM.EB.TIMEPM.WDGM020467,1 */
#if (WDGM_EB_PROD_ERR_HANDLING_DATA_CORRUPTION == WDGM_PROD_ERR_REP_TO_DEM)
    WDGM_EB_DEM(WDGM_EB_DATA_CORRUPTION_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#elif (WDGM_EB_PROD_ERR_HANDLING_DATA_CORRUPTION == WDGM_PROD_ERR_REP_TO_DET)
    WDGM_EB_DET(WDGM_SID_MAIN_FUNCTION, WDGM_EB_E_DEMTODET_DATA_CORRUPTION);
#endif
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_DECMAXCYCLESDMDATA_EXIT(RetVal,DMData);
  return RetVal;
}

/*------------------[WdgM_EB_StoreCPDeadlineData]------------------------------------------------*/
static FUNC(void, WDGM_CODE) WdgM_EB_StoreCPDeadlineData
(
  P2VAR(WdgM_EB_CPDataType,AUTOMATIC,WDGM_VAR) CPData,
  WdgM_EB_ResultType DeadlineResult
)
{
  const uint8 DeadlineDataRedundant = (uint8)(~DeadlineResult);
  uint16 DeadlineData;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_STORECPDEADLINEDATA_ENTRY(CPData,DeadlineResult);

  DeadlineData = DeadlineResult;
  DeadlineData |= (uint16)(((uint16)DeadlineDataRedundant) << 8U);

  CPData->DeadlineData = DeadlineData;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_STORECPDEADLINEDATA_EXIT(CPData,DeadlineResult);
}

/*------------------[WdgM_EB_GetCheckedCPDeadlineData]-------------------------------------------*/
static FUNC(Std_ReturnType,WDGM_CODE) WdgM_EB_GetCheckedCPDeadlineData
(
  P2CONST(WdgM_EB_CPDataType,AUTOMATIC,WDGM_VAR) CPData,
  P2VAR(WdgM_EB_ResultType,AUTOMATIC,AUTOMATIC) CPDeadlineResult
)
{
  Std_ReturnType RetVal;
  const uint16 UncheckedDeadlineData = CPData->DeadlineData;
  const WdgM_EB_ResultType CheckedDeadlineData =
      (WdgM_EB_ResultType)(UncheckedDeadlineData & (uint8)0xFFU);
  const WdgM_EB_ResultType DeadlineDataRedundant = (WdgM_EB_ResultType)
      (~((WdgM_EB_ResultType)((uint16)(UncheckedDeadlineData >> 8U) & (uint8)0xFFU)));

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_GETCHECKEDCPDEADLINEDATA_ENTRY(CPData,CPDeadlineResult);

  if (CheckedDeadlineData == DeadlineDataRedundant)
  {
    RetVal = E_OK;
    *CPDeadlineResult = CheckedDeadlineData;
  }
  else
  {
    /* !LINKSTO WDGM.EB.TIMEPM.WDGM020470,2,WDGM.EB.TIMEPM.WDGM020471,1,
     WDGM.EB.TIMEPM.WDGM020472,1 */
    RetVal = E_NOT_OK;
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_GETCHECKEDCPDEADLINEDATA_EXIT(RetVal,CPData,CPDeadlineResult);
  return RetVal;
}
#endif

#if (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_ON)
/*------------------[WdgM_EB_StoreCPLogicalData]-------------------------------------------------*/
static FUNC(void, WDGM_CODE) WdgM_EB_StoreCPLogicalData
(
  P2VAR(WdgM_EB_CPDataType,AUTOMATIC,WDGM_VAR) CPData,
  WdgM_EB_ResultType LogicalResult
)
{
  const WdgM_EB_ResultType LogicalDataRedundant = (WdgM_EB_ResultType)(~LogicalResult);
  uint16 LogicalData;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_STORECPLOGICALDATA_ENTRY(CPData,LogicalResult);

  LogicalData = LogicalResult;
  LogicalData |= (uint16)(((uint16)LogicalDataRedundant) << 8U);

  CPData->LogicalData = LogicalData;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_STORECPLOGICALDATA_EXIT(CPData,LogicalResult);
}

/*------------------[WdgM_EB_GetCheckedCPLogicalData]--------------------------------------------*/
static FUNC(WdgM_EB_ResultType,WDGM_CODE) WdgM_EB_GetCheckedCPLogicalData
(
  P2CONST(WdgM_EB_CPDataType,AUTOMATIC,WDGM_VAR) CPData,
  P2VAR(WdgM_EB_ResultType,AUTOMATIC,AUTOMATIC) CPLogicalResult
)
{
  Std_ReturnType RetVal;
  const uint16 UncheckedLogicalData = CPData->LogicalData;
  const WdgM_EB_ResultType CheckedLogicalData =
      (WdgM_EB_ResultType)(UncheckedLogicalData & (uint8)0xFFU);
  const WdgM_EB_ResultType LogicalDataRedundant = (WdgM_EB_ResultType)
      (~((WdgM_EB_ResultType)((uint16)(UncheckedLogicalData >> 8U) & (uint8)0xFFU)));

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_GETCHECKEDCPLOGICALDATA_ENTRY(CPData,CPLogicalResult);

  if (CheckedLogicalData == LogicalDataRedundant)
  {
    RetVal = E_OK;
    *CPLogicalResult = CheckedLogicalData;
  }
  else
  {
    /* !LINKSTO WDGM.EB.TIMEPM.WDGM020470,2,WDGM.EB.TIMEPM.WDGM020471,1,
     WDGM.EB.TIMEPM.WDGM020472,1 */
    RetVal = E_NOT_OK;
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_GETCHECKEDCPLOGICALDATA_EXIT(RetVal,CPData,CPLogicalResult);
  return RetVal;
}

#if (WDGM_EB_EXTERNAL_GRAPH_NUM > 0U)
/*----------------------[WdgM_EB_ResetExtGraphDataSeq]-----------------------------------------*/

static FUNC(void, WDGM_CODE) WdgM_EB_ResetExtGraphDataSeq
(
  P2VAR(WdgM_EB_ExtGraphDataSeqArrayType,AUTOMATIC,WDGM_CONST) IntExtGraphDataSeqPtr,
  uint8 NumOfSEperGraphId
)
{
  uint8 idx;
  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_RESETEXTGRAPHDATASEQ_ENTRY(IntExtGraphDataSeqPtr, NumOfSEperGraphId);

  for (idx = 0U; idx < NumOfSEperGraphId; idx++)
  {
   /* reset the concurrent counter */
    WdgM_EB_StoreSeqNumber(IntExtGraphDataSeqPtr, idx, 0U);

  }
  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_RESETEXTGRAPHDATASEQ_EXIT(IntExtGraphDataSeqPtr, NumOfSEperGraphId);
}
#endif /* if (WDGM_EB_EXTERNAL_GRAPH_NUM > 0U) */
#endif

/*------------------[WdgM_EB_StoreExpiredSEID]---------------------------------------------------*/
static FUNC(void, WDGM_CODE) WdgM_EB_StoreExpiredSEID
(
  P2VAR(WdgM_EB_ExpiredSEIDType,AUTOMATIC,WDGM_VAR_POWER_ON_INIT) ExpiredSEID,
  WdgM_EB_SEIdType SEID
)
{
  const WdgM_EB_SEIdType ExpiredSEIDRedundant = (WdgM_EB_SEIdType)(~SEID);
  WdgM_EB_ExpiredSEIDType LocalSEID;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_STOREEXPIREDSEID_ENTRY(ExpiredSEID,SEID);

  if (WDGM_EB_ATOMIC_LOAD(WdgM_EB_IsExpiredSEIdSet) == FALSE)
  {
    LocalSEID = SEID;
    LocalSEID |= (WdgM_EB_ExpiredSEIDType)(((WdgM_EB_ExpiredSEIDType)ExpiredSEIDRedundant) << 8U);

    *ExpiredSEID = LocalSEID ;
    WDGM_EB_ATOMIC_STORE(WdgM_EB_IsExpiredSEIdSet,TRUE);
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_STOREEXPIREDSEID_EXIT(ExpiredSEID,SEID);
}

/*------------------[WdgM_EB_GetCheckedExpiredSEID]----------------------------------------------*/
static FUNC(Std_ReturnType,WDGM_CODE) WdgM_EB_GetCheckedExpiredSEID
(
  WdgM_EB_ExpiredSEIDType UncheckedExpiredSEID,
  P2VAR(WdgM_EB_SEIdType,AUTOMATIC,AUTOMATIC) ExpiredSEID
)
{
  Std_ReturnType RetVal;
  const WdgM_EB_SEIdType CheckedExpiredSEID = (WdgM_EB_SEIdType)(UncheckedExpiredSEID);
  const WdgM_EB_SEIdType ExpiredSEIDRedundant = (WdgM_EB_SEIdType)
      (~((WdgM_EB_SEIdType)(((WdgM_EB_ExpiredSEIDType)(UncheckedExpiredSEID >> 8U))
          & (uint8)0xFFU)));

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_GETCHECKEDEXPIREDSEID_ENTRY(UncheckedExpiredSEID,ExpiredSEID);

  if (CheckedExpiredSEID == ExpiredSEIDRedundant)
  {
    RetVal = E_OK;
    *ExpiredSEID = CheckedExpiredSEID;
  }
  else
  {
    RetVal = E_NOT_OK;
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_GETCHECKEDEXPIREDSEID_EXIT(RetVal,UncheckedExpiredSEID,ExpiredSEID);
  return RetVal;
}

/*------------------[WdgM_EB_SEId2IntSEId]-------------------------------------------------------*/
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_SEId2IntSEId
(
  WdgM_SupervisedEntityIdType SEId,
  P2VAR(WdgM_EB_SEIdType,AUTOMATIC,AUTOMATIC) IntSEId
)
{
  Std_ReturnType RetVal;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_SEID2INTSEID_ENTRY(SEId,IntSEId);

  if (SEId < WDGM_EB_SE_NUM)
  {
    *IntSEId = WdgM_EB_SEIntSEId[SEId];
    RetVal = E_OK;
  }
  else
  {
    *IntSEId = 0U;
    RetVal = E_NOT_OK;
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_SEID2INTSEID_EXIT(RetVal,SEId,IntSEId);
  return RetVal;
}

/*------------------[WdgM_EB_SEIsActiveResetData]------------------------------------------------*/
static FUNC(boolean, WDGM_CODE) WdgM_EB_SEIsActiveResetData
(
  WdgM_ModeType NewMode,
  P2CONST(WdgM_EB_SECfgType,AUTOMATIC,WDGM_CONST) SECfg,
  boolean IsNormalOperation,
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
)
{
  boolean IsActive = FALSE;
  WdgM_EB_CPIdType CPId;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_SEISACTIVERESETDATA_ENTRY(NewMode,SECfg,IsNormalOperation,CoreInstance);

#if (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_OFF)
  WDGM_EB_PARAM_UNUSED(IsNormalOperation);
  WDGM_EB_PARAM_UNUSED(CoreInstance);
#endif

  /* go over all CPs and check if a supervision mechanism is active */
  for (CPId = 0U; CPId < SECfg->NumOfCPs; CPId++)
  {
    CONSTP2CONST(WdgM_EB_CPCfgType,AUTOMATIC,WDGM_CONST) CPCfg = &(SECfg->CPCfg[CPId]);

    if (CPCfg->AliveCfgPtr[NewMode] != NULL_PTR)
    {
      IsActive = TRUE;
    }
#if (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_ON)
    {
      CONSTP2CONST(WdgM_EB_CPLogicalCfgType,AUTOMATIC,WDGM_CONST) CPIntLogicalCfgOldMode =
          CPCfg->IntLogicalCfgPtr[CoreInstance->WdgM_EB_CurrentModeId];
      CONSTP2CONST(WdgM_EB_CPLogicalCfgType,AUTOMATIC,WDGM_CONST) CPIntLogicalCfgNewMode =
          CPCfg->IntLogicalCfgPtr[NewMode];

      if (CPIntLogicalCfgNewMode != NULL_PTR)
      {
        IsActive = TRUE;

        /* During initialization (IsNormalOperation == FALSE),
         * always reset Supervision Data independent of old Mode */
        /* !LINKSTO WDGM.EB.TIMEPM.WDGM020430,1,WDGM.EB.ActiveFlags4,1 */
        if ((IsNormalOperation == FALSE) || (CPIntLogicalCfgNewMode != CPIntLogicalCfgOldMode))
        {
          const WdgM_EB_GraphIdType GraphId = CPIntLogicalCfgNewMode->GraphId;
          CONSTP2VAR(WdgM_EB_GraphDataType,AUTOMATIC,WDGM_VAR) GraphData =
              &(WdgM_EB_InternalGraphData[GraphId]);
          /* !LINKSTO WDGM.EB.ResetActiveFlags3,1,WDGM.EB.ResetActiveFlags2,1 */
          WdgM_EB_StoreGraphData (GraphData, 0U, 0U, FALSE);
        }
        else
        {
          /* Internal Graph is active in both old and new mode => keep active flag */
        }
      }
    }
#if (WDGM_EB_EXTERNAL_GRAPH_NUM > 0U)
    {
      CONSTP2CONST(WdgM_EB_CPLogicalCfgType,AUTOMATIC,WDGM_CONST) CPExtLogicalCfgNewMode =
          CPCfg->ExtLogicalCfgPtr[NewMode];

      if (CPExtLogicalCfgNewMode != NULL_PTR)
      {
        /* Always reset Supervision Data independent of old Mode */
        /* !LINKSTO WDGM.EB.TIMEPM.WDGM020430,1,WDGM.EB.ActiveFlags4,1 */
        const WdgM_EB_GraphIdType GraphId = CPExtLogicalCfgNewMode->GraphId;
        CONSTP2VAR(WdgM_EB_GraphDataType,AUTOMATIC,WDGM_VAR) GraphData =
        &(WdgM_EB_ExternalGraphData[GraphId]);

        CONSTP2CONST(WdgM_EB_ExtGraphDataSeqType,AUTOMATIC,WDGM_VAR) ExtGraphDataSeqPtr =
            &(WdgM_EB_ExtGraphDataSeqPtr[GraphId]);

        CONSTP2VAR(WdgM_EB_ExtGraphDataSeqArrayType,AUTOMATIC,WDGM_VAR) IntExtGraphDataSeqPtr =
            (ExtGraphDataSeqPtr->GraphDataPtr);

        uint8 Local_NumOfSEperGraphId = (ExtGraphDataSeqPtr->NumOfSEperGraphId);

        /* Reset data from this graph */
        WdgM_EB_ResetExtGraphDataSeq(IntExtGraphDataSeqPtr,Local_NumOfSEperGraphId);

        IsActive = TRUE;
        /* !LINKSTO WDGM.EB.ResetActiveFlags3,1,WDGM.EB.ResetActiveFlags2,1 */
        WdgM_EB_StoreGraphData (GraphData, 0U, 0U, FALSE);
      }
    }
#endif  /* if (WDGM_EB_EXTERNAL_GRAPH_NUM > 0U) */
#endif
#if (WDGM_EB_DEADLINE_SUPERVISION_ENABLED == STD_ON)
    {
      CONSTP2CONST(WdgM_EB_CPDeadlineCfgType,AUTOMATIC,WDGM_CONST) CPDeadlineCfgNewMode =
          CPCfg->DeadlineCfgPtr[NewMode];

      /* Note: Checking the old mode is not possible, because the configuration
       * forbids using the same Deadline Monitoring in two different modes. */
      /* !LINKSTO WDGM.ASR40.WDGM314,1 */
      if (CPDeadlineCfgNewMode != NULL_PTR)
      {
        const WdgM_EB_DMIdType DMId = CPDeadlineCfgNewMode->DMId;
        CONSTP2VAR(WdgM_EB_DMDataType,AUTOMATIC,WDGM_VAR) DMData =
            &(WdgM_EB_DMData[DMId]);
        const WdgM_EB_TimestampType TimeStamp = 0U;

        IsActive = TRUE;
        /* !LINKSTO WDGM.EB.TIMEPM.WDGM020429,1,WDGM.EB.ResetActiveFlags1,1 */
        WdgM_EB_StoreDMData (DMData, &TimeStamp, 0U, FALSE);
      }
    }
#endif
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_SEISACTIVERESETDATA_EXIT(IsActive,NewMode,SECfg,IsNormalOperation,CoreInstance);
  return IsActive;
}

#if ((WDGM_EB_DEADLINE_SUPERVISION_ENABLED == STD_ON) || \
     (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_ON))
/*------------------[WdgM_EB_SEResetData]--------------------------------------------------------*/
static FUNC(void, WDGM_CODE) WdgM_EB_SEResetData
(
  WdgM_EB_SEIdType IntSEId
)
{
  WdgM_EB_CPIdType CPId;
  CONSTP2CONST(WdgM_EB_SECfgType,AUTOMATIC,WDGM_CONST) SECfg = &(WdgM_EB_SECfg[IntSEId]);

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_SERESETDATA_ENTRY(IntSEId);

  /* go through all CPs and clear their data */
  for (CPId = 0U; CPId < SECfg->NumOfCPs; CPId++)
  {
    CONSTP2VAR(WdgM_EB_CPDataType,AUTOMATIC,WDGM_VAR) CPData = &(SECfg->CPData[CPId]);

#if (WDGM_EB_DEADLINE_SUPERVISION_ENABLED == STD_ON)
    WdgM_EB_StoreCPDeadlineData (CPData, WDGM_EB_RESULT_CORRECT);
#endif
#if (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_ON)
    WdgM_EB_StoreCPLogicalData (CPData, WDGM_EB_RESULT_CORRECT);
#endif
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_SERESETDATA_EXIT(IntSEId);
}

#endif

/*------------------[WdgM_EB_SEResetCPAliveData]--------------------------------------------------------*/
static FUNC(void, WDGM_CODE) WdgM_EB_SEResetCPAliveData
(
  WdgM_EB_SEIdType IntSEId,
  WdgM_EB_IntRamIdxType IntRamIdxBase,
  boolean InternalRAMOnly,
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
)
{
  WdgM_EB_CPIdType CPId;
  CONSTP2CONST(WdgM_EB_SECfgType,AUTOMATIC,WDGM_CONST) SECfg = &(WdgM_EB_SECfg[IntSEId]);

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_SERESETCPALIVEDATA_ENTRY(IntSEId,IntRamIdxBase,InternalRAMOnly,CoreInstance);

  /* go through all CPs and clear their data */
  for (CPId = 0U; CPId < SECfg->NumOfCPs; CPId++)
  {
    CONSTP2VAR(WdgM_EB_CPDataType,AUTOMATIC,WDGM_VAR) CPData = &(SECfg->CPData[CPId]);
    const WdgM_EB_IntRamIdxType IntRamIdx = IntRamIdxBase + CPId;
    WdgM_EB_AliveCounterType OldAliveCounter;

    if (InternalRAMOnly == FALSE)
    {
      /* Increment API correctly updates inverse data of alive counter of CPData */
      WdgM_EB_IncrementCPAliveCounterData (CPData);
    }

    /* return value can be ignored, because value is overwritten afterwards */
    (void)WdgM_EB_GetCheckedCPAliveCounterData(CPData, IntRamIdx, &OldAliveCounter, CoreInstance);
    WdgM_EB_ResetCPAliveCounterData (IntRamIdx, OldAliveCounter, CoreInstance);

  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_SERESETCPALIVEDATA_EXIT(IntSEId,IntRamIdxBase,InternalRAMOnly,CoreInstance);
}

/*------------------[WdgM_EB_UpdateTriggerConditions]--------------------------------------------*/
static FUNC(void, WDGM_CODE) WdgM_EB_UpdateTriggerConditions
(
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
)
{
  CONSTP2CONST(WdgM_EB_ModeCfgType,AUTOMATIC,WDGM_CONST) ModeCfg =
      &(WdgM_EB_ModeCfg[CoreInstance->WdgM_EB_CurrentModeId]);
  CONSTP2CONST(WdgM_EB_TriggerCfgType,AUTOMATIC,WDGM_CONST) TriggerCfg = ModeCfg->TriggerCfg;
  WdgM_EB_TriggerIdType TriggerId;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_UPDATETRIGGERCONDITIONS_ENTRY(CoreInstance);

  /* go through all configured watchdogs in this WdgMMode and trigger them */
  for (TriggerId = 0U; TriggerId < ModeCfg->NumTriggers; TriggerId++)
  {
    /* trigger only if the watchdog is not configured to be off in this mode */
    if (TriggerCfg[TriggerId].WatchdogMode != WDGIF_OFF_MODE)
    {
      WdgM_EB_TriggerConditionType TriggerCondition;

      /* !LINKSTO WDGM.ASR40.WDGM122,1,WDGM.ASR40.WDGM223,1 */
      if (WDGM_EB_ATOMIC_LOAD(WdgM_EB_GlobalStatus) == WDGM_GLOBAL_STATUS_STOPPED)
      {
        /* !LINKSTO WdgM.EB.PerformReset3,1 */
        TriggerCondition = 0U;
      }
      else
      {
        TriggerCondition = TriggerCfg[TriggerId].TriggerCondition;
      }

      /* !LINKSTO WDGM.ASR40.WDGM119,1,WDGM.ASR40.WDGM120,1,WDGM.ASR40.WDGM121,1 */
      WdgIf_SetTriggerCondition (TriggerCfg[TriggerId].WatchdogId, TriggerCondition);
    }

  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_UPDATETRIGGERCONDITIONS_EXIT(CoreInstance);
}

/*------------------[WdgM_EB_CheckCallerID]------------------------------------------------------*/
#if (WDGM_EB_CHECK_CALLERID == STD_ON)
static FUNC(boolean, WDGM_CODE) WdgM_EB_CheckCallerID
(
  uint16 CallerID
)
{
  uint8_least CallerIdx = 0U;
  boolean IsValidCaller = FALSE;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_CHECKCALLERID_ENTRY(CallerID);

  /* Check if passed CallerID is valid. That is, it is in the configured
   * list of CallerIDs. */
#if (WDGM_EB_CALLERIDS_NUM != 1U)
  for (CallerIdx = 0U; CallerIdx < WDGM_EB_CALLERIDS_NUM; CallerIdx++)
#endif
  {
    if (WdgM_EB_CallerIds[CallerIdx] == CallerID)
    {
      /* found a caller -> leave the loop */
      IsValidCaller = TRUE;
#if (WDGM_EB_CALLERIDS_NUM != 1U)
      break;
#endif
    }
  }

  /* !LINKSTO WDGM.ASR40.WDGM245,1,WDGM.ASR40.WDGM004.13,1 */
  if (FALSE == IsValidCaller)
  {
#if (WDGM_EB_PROD_ERR_HANDLING_IMPROPER_CALLER == WDGM_PROD_ERR_REP_TO_DEM)
    WDGM_EB_DEM(WDGM_EB_IMPROPER_CALLER_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#elif (WDGM_EB_PROD_ERR_HANDLING_IMPROPER_CALLER == WDGM_PROD_ERR_REP_TO_DET)
    WDGM_EB_DET(WDGM_SID_SET_MODE, WDGM_EB_E_DEMTODET_IMPROPER_CALLER);
#endif
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_CHECKCALLERID_EXIT(IsValidCaller,CallerID);
  return IsValidCaller;
}
#endif

/*==================[internal function definitions (external interfaces)]========================*/

/*------------------[WdgM_EB_CheckInitRequest]---------------------------------------------------*/
static FUNC(boolean, WDGM_CODE) WdgM_EB_CheckInitRequest
(
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
)
{
  boolean RequestPerformed = FALSE;
#if (WDGM_EB_EXT_GET_EXPECTED_INIT_STATE_ENABLED == STD_ON)
  WdgM_EB_InitStatusType ExpectedInitState = WDGM_EB_INIT_STATUS_DEINIT;
  Std_ReturnType ExtRetVal = E_NOT_OK;
#endif

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_CHECKINITREQUEST_ENTRY(CoreInstance);

  {
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
    if(CoreInstance->CoreID == WDGM_EB_MASTER_INSTANCE)
#endif
    {
      /* check for initialization request */
      /* !LINKSTO WDGM.EB.TIMEPM.WDGM020412,1 */
#if (WDGM_EB_EXT_GET_EXPECTED_INIT_STATE_ENABLED == STD_ON)
      ExtRetVal = WDGM_EB_EXT_GET_EXPECTED_INIT_STATE(&ExpectedInitState);
      WDGM_PRECONDITION_ASSERT((ExtRetVal == E_OK) || (ExtRetVal == E_NOT_OK),WDGM_SID_MAIN_FUNCTION);
      WDGM_PRECONDITION_ASSERT(
          (ExpectedInitState == WDGM_EB_INIT_STATUS_DEINIT) ||
              (ExpectedInitState == WDGM_EB_INIT_STATUS_INIT),
              WDGM_SID_MAIN_FUNCTION);
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
      /* Store the expected init state to a shared memory between cores variable. */
      WDGM_EB_ATOMIC_STORE(WdgM_EB_ExpectedInitState, ExpectedInitState);
    }
    else
    {
      /* Load the expected init state from the shared memory. */
      ExpectedInitState = WDGM_EB_ATOMIC_LOAD(WdgM_EB_ExpectedInitState);
      /* Satellite cores doesn't call the get expected init state, the initialization is done after master
       * core is initialized. For satellites this value is always E_OK. */
      ExtRetVal = E_OK;
#endif
#endif /* if (WDGM_EB_EXT_GET_EXPECTED_INIT_STATE_ENABLED == STD_ON) */
    }

#if (WDGM_EB_EXT_GET_EXPECTED_INIT_STATE_ENABLED == STD_ON)
    if ((ExtRetVal == E_OK) && (WDGM_EB_INIT_STATUS_INIT == ExpectedInitState))
#else
    if (WDGM_EB_INIT_STATUS_INIT == WDGM_EB_ATOMIC_LOAD(WdgM_EB_ExpectedInitState))
#endif
    {
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
      if (CoreInstance->CoreID == WDGM_EB_MASTER_INSTANCE)
#endif
      {
        Std_ReturnType InitRetVal = WdgM_EB_Init(CoreInstance);
        /* check whether WdgM was initialized properly */
        if (InitRetVal != E_OK)
        {
#if (WDGM_EB_EXT_GET_EXPECTED_INIT_STATE_ENABLED == STD_OFF)
          /* if callouts are not used,
           * reset request to old state (Never-give-up strategy makes no sense here) */
          WDGM_EB_ATOMIC_STORE(WdgM_EB_ExpectedInitState,WDGM_EB_INIT_STATUS_DEINIT);
#endif
          /* !LINKSTO WDGM.EB.TIMEPM.WDGM020441,1,WDGM.EB.TIMEPM.WDGM020442,1 */
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
          /* If init request did not work, the Supervisor shall handle this issue */
        WDGM_EB_DET(WDGM_SID_MAIN_FUNCTION, WDGM_EB_E_INIT_REQUEST);
#endif
        }
        else
        {
          RequestPerformed = TRUE;
        }
      }
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
      else
      {
        if( (WDGM_EB_ATOMIC_LOAD(WdgM_EB_GlobalInitStatus) & WDGM_EB_MASTER_STATUS_INIT)
            == WDGM_EB_MASTER_STATUS_INIT )
          {
            /* delay the satellite instances initialization until the master instance initialization finished */
            (void)WdgM_EB_Init(CoreInstance);
          }
      }
#endif
    }
#if (WDGM_DEFENSIVE_BEHAVIOR == STD_ON)
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
    /* !LINKSTO WDGM.EB.TIMEPM.WDGM020437,1,WDGM.ASR40.WDGM004.1,1 */
    else
    {
        /* If init request did not work, the Supervisor shall handle this issue */
        WDGM_EB_DET(WDGM_SID_MAIN_FUNCTION, WDGM_E_NO_INIT);
    }
#endif
#endif
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_CHECKINITREQUEST_EXIT(RequestPerformed,CoreInstance);
  return RequestPerformed;
}

/*------------------[WdgM_EB_CheckDeInitRequest]-------------------------------------------------*/
static FUNC(boolean, WDGM_CODE) WdgM_EB_CheckDeinitRequest
(
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
)
{
  boolean RequestPerformed = FALSE;
#if (WDGM_EB_EXT_GET_EXPECTED_INIT_STATE_ENABLED == STD_ON)
  WdgM_EB_InitStatusType ExpectedInitState = WDGM_EB_INIT_STATUS_INIT;
  Std_ReturnType ExtRetVal = E_NOT_OK;
#endif
  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_CHECKDEINITREQUEST_ENTRY(CoreInstance);

  {
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
    if(CoreInstance->CoreID == WDGM_EB_MASTER_INSTANCE)
#endif
    {
      /* check for de-initialization request */
      /* !LINKSTO WDGM.EB.TIMEPM.WDGM020415,1 */
#if (WDGM_EB_EXT_GET_EXPECTED_INIT_STATE_ENABLED == STD_ON)
      ExtRetVal = WDGM_EB_EXT_GET_EXPECTED_INIT_STATE(&ExpectedInitState);
      WDGM_PRECONDITION_ASSERT((ExtRetVal == E_OK) || (ExtRetVal == E_NOT_OK),WDGM_SID_MAIN_FUNCTION);
      WDGM_PRECONDITION_ASSERT(
          (ExpectedInitState == WDGM_EB_INIT_STATUS_DEINIT) ||
              (ExpectedInitState == WDGM_EB_INIT_STATUS_INIT),
              WDGM_SID_MAIN_FUNCTION);
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
      /* Store the expected init state to a shared memory between cores variable. */
      WDGM_EB_ATOMIC_STORE(WdgM_EB_ExpectedInitState, ExpectedInitState);
    }
    else
    {
      /* Load the expected init state from the shared memory. */
      ExpectedInitState = WDGM_EB_ATOMIC_LOAD(WdgM_EB_ExpectedInitState);
      /* Satellite cores doesn't call the get expected init state, the de-initialization is done after master
       * core is de-initialized. For satellites this value is always E_OK. */
      ExtRetVal = E_OK;
#endif
#endif /* if (WDGM_EB_EXT_GET_EXPECTED_INIT_STATE_ENABLED == STD_ON) */
    }

#if (WDGM_EB_EXT_GET_EXPECTED_INIT_STATE_ENABLED == STD_ON)
    if ((ExtRetVal == E_OK) && (WDGM_EB_INIT_STATUS_DEINIT == ExpectedInitState))
#else
    if (WDGM_EB_INIT_STATUS_DEINIT == WDGM_EB_ATOMIC_LOAD(WdgM_EB_ExpectedInitState))
#endif
    {
#if (WDGM_EB_DEINIT_MODE_ENABLED == STD_ON)
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
      if (CoreInstance->CoreID == WDGM_EB_MASTER_INSTANCE)
#endif
      {
        const Std_ReturnType DeInitRetVal = WdgM_EB_DeInit(CoreInstance);
        /* check whether WdgM was de-initialized properly */
        if (DeInitRetVal != E_OK)
        {
#if (WDGM_EB_EXT_GET_EXPECTED_INIT_STATE_ENABLED == STD_OFF)
          /* if callouts are not used,
           * reset request to old state (Never-give-up strategy makes no sense here */
          WDGM_EB_ATOMIC_STORE(WdgM_EB_ExpectedInitState, WDGM_EB_INIT_STATUS_INIT);
#endif
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
          /* If de-init request did not work, the Supervisor shall handle this issue */
          /* !LINKSTO WDGM.EB.TIMEPM.WDGM020444,1,WDGM.EB.TIMEPM.WDGM020445,1 */
          WDGM_EB_DET(WDGM_SID_MAIN_FUNCTION, WDGM_EB_E_DEINIT_REQUEST);
#endif
        }
        else
        {
          RequestPerformed = TRUE;
        }
      }
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
      else
      {
        /* all the satellite cores are de-initialized only if master core de-initialization was successful performed */
        if( (WDGM_EB_ATOMIC_LOAD(WdgM_EB_GlobalInitStatus) & WDGM_EB_MASTER_STATUS_INIT)
            != WDGM_EB_MASTER_STATUS_INIT )
        {
          (void)WdgM_EB_DeInit(CoreInstance);
        }
      }
#endif

#else /* (WDGM_EB_DEINIT_MODE_ENABLED == STD_ON) */
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
      if (CoreInstance->CoreID == WDGM_EB_MASTER_INSTANCE)
#endif
      {
        /* return value of WdgM_EB_DeInit is always E_OK and can be ignored */
        (void)WdgM_EB_DeInit(CoreInstance);
        RequestPerformed = TRUE;
      }
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
      else
      {
        /* all the satellite cores are de-initialized only if master core de-initialization was successful performed */
        if( (WDGM_EB_ATOMIC_LOAD(WdgM_EB_GlobalInitStatus) & WDGM_EB_MASTER_STATUS_INIT)
            != WDGM_EB_MASTER_STATUS_INIT )
        {
          /* return value of WdgM_EB_DeInit is always E_OK and can be ignored */
          (void)WdgM_EB_DeInit(CoreInstance);
          RequestPerformed = TRUE;
        }
      }
#endif
#endif /* (WDGM_EB_DEINIT_MODE_ENABLED == STD_ON) */
    }
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_CHECKDEINITREQUEST_EXIT(RequestPerformed,CoreInstance);
  return RequestPerformed;
}

#if (WDGM_EB_ENABLE_SETMODE_SYNCHRON != STD_ON)
/*------------------[WdgM_EB_CheckModeSwitchRequest]---------------------------------------------*/
static FUNC(boolean, WDGM_CODE) WdgM_EB_CheckModeSwitchRequest
(
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
)
{
  boolean RequestPerformed = FALSE;
  WdgM_ModeType ExpectedWdgMMode;
#if (WDGM_EB_EXT_GET_EXPECTED_WDGM_MODE_ENABLED == STD_ON)
  Std_ReturnType ExtRetVal = E_NOT_OK;
#endif
  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_CHECKMODESWITCHREQUEST_ENTRY(CoreInstance);

  {
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
    if(CoreInstance->CoreID == WDGM_EB_MASTER_INSTANCE)
#endif
    {
      /* !LINKSTO WDGM.EB.TIMEPM.WDGM020419,1 */
#if (WDGM_EB_EXT_GET_EXPECTED_WDGM_MODE_ENABLED == STD_ON)
      ExpectedWdgMMode = (WdgM_ModeType)(CoreInstance->WdgM_EB_CurrentModeId & (WdgM_ModeType)0xFFU);
      ExtRetVal = WDGM_EB_EXT_GET_EXPECTED_WDGM_MODE(&ExpectedWdgMMode);
      WDGM_PRECONDITION_ASSERT((ExtRetVal == E_OK) || (ExtRetVal == E_NOT_OK),WDGM_SID_MAIN_FUNCTION);
      WDGM_PRECONDITION_ASSERT(ExpectedWdgMMode < WdgM_EB_ModeCfgSize, WDGM_SID_MAIN_FUNCTION);
#else
      ExpectedWdgMMode = (WdgM_ModeType)(WDGM_EB_ATOMIC_LOAD(WdgM_EB_ExpectedWdgMModeId) & (WdgM_ModeType)0xFFU);
#endif
    }
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
    else
    {
      /* !LINKSTO WDGM.EB.MulticoreSwitchMode.4,1 */
      ExpectedWdgMMode = (WdgM_ModeType)(WDGM_EB_ATOMIC_LOAD(WdgM_EB_CoreSwitchMode[WDGM_EB_MASTER_INSTANCE]) & (WdgM_ModeType)0xFFU);
#if (WDGM_EB_EXT_GET_EXPECTED_WDGM_MODE_ENABLED == STD_ON)
      /* Satellite cores doesn't call the get expected mode, the mode switch is done after master core changed his mode.
       * For satellites this value is always E_OK. */
      ExtRetVal = E_OK;
#endif
    }
#endif

    /* !LINKSTO WDGM.EB.TIMEPM.WDGM020417,1 */
#if (WDGM_EB_EXT_GET_EXPECTED_WDGM_MODE_ENABLED == STD_ON)
    if ((ExtRetVal == E_OK) && (CoreInstance->WdgM_EB_CurrentModeId != ExpectedWdgMMode))
#else
    if (CoreInstance->WdgM_EB_CurrentModeId != ExpectedWdgMMode)
#endif
    {
      const Std_ReturnType SetModeRetVal = WdgM_EB_SetMode(ExpectedWdgMMode, TRUE, CoreInstance);
      /* check whether the WdgM mode switch executed properly */
      /* WdgM_EB_SetMode function can return E_NOT_OK only for the master core, for the satellite core always return E_OK */
      if (SetModeRetVal != E_OK)
      {
#if (WDGM_EB_EXT_GET_EXPECTED_WDGM_MODE_ENABLED == STD_OFF)
        /* if callouts are not used,
         * reset request to old state (Never-give-up strategy makes no sense here */
        WDGM_EB_ATOMIC_STORE(WdgM_EB_ExpectedWdgMModeId, CoreInstance->WdgM_EB_CurrentModeId);
#endif
        /* !LINKSTO WDGM.EB.TIMEPM.WDGM020438,1,WDGM.EB.TIMEPM.WDGM020439,1,
         WDGM.EB.TIMEPM.WDGM020440,1 */
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
        /* If mode request did not work, the Supervisor shall handle this issue */
        WDGM_EB_DET(WDGM_SID_MAIN_FUNCTION, WDGM_EB_E_SETMODE_REQUEST);
#endif
      }
      else
      {
        RequestPerformed = TRUE;

#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
        /* !LINKSTO WDGM.EB.MulticoreSwitchMode.3,1 */
        /* announce other WdgM core instances that the mode switch  on this WdgM
        * instance has been performed */
        WDGM_EB_ATOMIC_STORE(WdgM_EB_CoreSwitchMode[CoreInstance->CoreID], ExpectedWdgMMode);
#endif
      }
    }
  }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_CHECKMODESWITCHREQUEST_EXIT(RequestPerformed,CoreInstance);
  return RequestPerformed;
}
#endif /* #if (WDGM_EB_ENABLE_SETMODE_SYNCHRON != STD_ON) */

#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
/*------------------[WdgM_EB_CheckModeSwitchTimeFrame]---------------------------------------------------*/
static FUNC(void, WDGM_CODE) WdgM_EB_CheckModeSwitchTimeFrame (void)
{
  boolean AllCoresSameMode = TRUE;
  uint8 idx = 0U;
#if (WDGM_EB_MULTICORE_ENABLED == STD_ON)
  const Atomic_value_t GlobalInitStatus = WDGM_EB_ATOMIC_LOAD(WdgM_EB_GlobalInitStatus);
#endif

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_CHECKMODESWITCHTIMEFRAME_ENTRY();
  /* If at least one of the satellite instances are not initialized */
  if((WDGM_EB_MASTER_STATUS_INIT == (GlobalInitStatus & WDGM_EB_MASTER_STATUS_INIT))
    && (WDGM_EB_GLOBAL_STATUS_INIT != GlobalInitStatus))
  {
    AllCoresSameMode = FALSE;
  }
  /* go through all core instances and check if all cores switched to the current mode */
  for (idx = 0U; (idx < WDGM_EB_NUMBER_OF_CORES) && (AllCoresSameMode == TRUE); idx++)
  {
    if (WDGM_EB_MASTER_INSTANCE != idx )
    {
      if ( WDGM_EB_ATOMIC_LOAD(WdgM_EB_CoreSwitchMode[idx]) !=
           WDGM_EB_ATOMIC_LOAD(WdgM_EB_CoreSwitchMode[WDGM_EB_MASTER_INSTANCE]) )
      {
        AllCoresSameMode = FALSE;
      }
    }
  }
   /* not all satellite cores switched to the current mode, increment counter */
   if ( FALSE == AllCoresSameMode )
   {
     /* if time frame expired */
     if( WDGM_EB_MASTER_WAIT_SLAVE_MODE_SWITCH == WdgM_EB_MasterWaitSlavesModeSwitch )
     {
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
       /* !LINKSTO WDGM.EB.MulticoreInit.3,1 */
       /* !LINKSTO WDGM.EB.MulticoreSwitchMode.2,1 */
       WDGM_EB_DET(WDGM_SID_MAIN_FUNCTION, WDGM_EB_E_SLAVE_FAILED_CHANGEMODE);
#endif
       WdgM_EB_MasterWaitSlavesModeSwitch = 0U;
     }
     else
     {
       /* increment counter */
       WdgM_EB_MasterWaitSlavesModeSwitch++;
     }
   }
   else
   {
     /* all satellite cores switched to the current mode, reset counter*/
     WdgM_EB_MasterWaitSlavesModeSwitch = 0U;
   }

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_CHECKMODESWITCHTIMEFRAME_EXIT();
}
#endif /* #if(WDGM_EB_MULTICORE_ENABLED == STD_ON) */

#if (WDGM_EB_ENABLE_SETMODE_SYNCHRON == STD_ON)

/*------------------[WdgM_EB_CheckModeSwitchRequestSynchron]---------------------------------------------*/
static FUNC(Std_ReturnType, WDGM_CODE) WdgM_EB_CheckModeSwitchRequestSynchron
(
  CONSTP2VAR(WdgM_EB_CoreContextType, AUTOMATIC, WDGM_APPL_CONST) CoreInstance
)
{
  Std_ReturnType RequestPerformed = E_NOT_OK;

  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_CHECKMODESWITCHREQUESTSYNCHRON_ENTRY(CoreInstance);
  {
    const WdgM_ModeType ExpectedWdgMMode = WdgM_EB_ExpectedWdgMModeId;

    if (CoreInstance->WdgM_EB_CurrentModeId != ExpectedWdgMMode)
    {
      const Std_ReturnType SetModeRetVal = WdgM_EB_SetMode(ExpectedWdgMMode, TRUE, CoreInstance);
      /* check whether the WdgM mode switch executed properly */
      if (SetModeRetVal != E_OK)
      {
        /* change mode was not performed
         * give a DET error */
        /* !LINKSTO WDGM.EB.TIMEPM.WDGM020438,1,WDGM.EB.TIMEPM.WDGM020439,1,WDGM.EB.TIMEPM.WDGM020440,1 */
#if (WDGM_DEV_ERROR_DETECT == STD_ON)
        /* If mode request did not work give a DET error */
         WDGM_EB_DET(WDGM_SID_SET_MODE, WDGM_EB_E_SETMODE_REQUEST);
#endif
      }
      else
      {
        RequestPerformed = E_OK;
      }
    }
    else
    {
      /* Is the same mode. */
      RequestPerformed = WDGM_EB_E_SAME_MODE;
    }
  }
  /* !LINKSTO WDGM.EB.Dbg4,1 */
  DBG_WDGM_EB_CHECKMODESWITCHREQUESTSYNCHRON_EXIT(RequestPerformed,CoreInstance);
  return RequestPerformed;
}

#endif /* #if (WDGM_EB_ENABLE_SETMODE_SYNCHRON == STD_ON) */

#define WDGM_STOP_SEC_CODE_ASIL_D
#include <WdgM_MemMap.h>

/*==================[end of file]================================================================*/
