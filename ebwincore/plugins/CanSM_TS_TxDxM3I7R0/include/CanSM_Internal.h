/**
 * \file
 *
 * \brief AUTOSAR CanSM
 *
 * This file contains the implementation of the AUTOSAR
 * module CanSM.
 *
 * \version 3.7.5
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef CANSM_INTERNAL_H
#define CANSM_INTERNAL_H

/*==================[inclusions]============================================*/

#include <ComStack_Types.h>    /* AUTOSAR communication stack types */

#include <CanSM_Int_Stc.h>     /* Module internal static header */
#include <CanSM_Cfg.h>         /* Module configuration */
#include <CanSM_Version.h>     /* Module version */

#include <SchM_CanSM.h>        /* Enter/exit critical sections */

#if (CANSM_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>
#endif

/*==================[macros]================================================*/

/*------------------[CanSM macros]-----------*/





/*------------------------[Defensive programming]----------------------------*/

#if (defined CANSM_PRECONDITION_ASSERT)
#error CANSM_PRECONDITION_ASSERT is already defined
#endif

#if (defined CANSM_PRECONDITION_ASSERT_NO_EVAL)
#error CANSM_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (CANSM_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define CANSM_PRECONDITION_ASSERT(Condition, ApiId) \
  DET_PRECONDITION_ASSERT((Condition), CANSM_MODULE_ID, CANSM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define CANSM_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL((Condition), CANSM_MODULE_ID, CANSM_INSTANCE_ID, (ApiId))
#else
#define CANSM_PRECONDITION_ASSERT(Condition, ApiId)
#define CANSM_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined CANSM_POSTCONDITION_ASSERT)
#error CANSM_POSTCONDITION_ASSERT is already defined
#endif

#if (defined CANSM_POSTCONDITION_ASSERT_NO_EVAL)
#error CANSM_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (CANSM_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define CANSM_POSTCONDITION_ASSERT(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT((Condition), CANSM_MODULE_ID, CANSM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define CANSM_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL((Condition), CANSM_MODULE_ID, CANSM_INSTANCE_ID, (ApiId))
#else
#define CANSM_POSTCONDITION_ASSERT(Condition, ApiId)
#define CANSM_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined CANSM_INVARIANT_ASSERT)
#error CANSM_INVARIANT_ASSERT is already defined
#endif

#if (defined CANSM_INVARIANT_ASSERT_NO_EVAL)
#error CANSM_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (CANSM_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define CANSM_INVARIANT_ASSERT(Condition, ApiId) \
  DET_INVARIANT_ASSERT((Condition), CANSM_MODULE_ID, CANSM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define CANSM_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL((Condition), CANSM_MODULE_ID, CANSM_INSTANCE_ID, (ApiId))
#else
#define CANSM_INVARIANT_ASSERT(Condition, ApiId)
#define CANSM_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined CANSM_STATIC_ASSERT)
# error CANSM_STATIC_ASSERT is already defined
#endif
#if (CANSM_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated */
# define CANSM_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define CANSM_STATIC_ASSERT(expr)
#endif

#if (defined CANSM_UNREACHABLE_CODE_ASSERT)
#error CANSM_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (CANSM_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 **

 ** \param[in] ApiId Service ID of the API function */
#define CANSM_UNREACHABLE_CODE_ASSERT(ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(CANSM_MODULE_ID, CANSM_INSTANCE_ID, (ApiId))
#else
#define CANSM_UNREACHABLE_CODE_ASSERT(ApiId)
#endif

#if (defined CANSM_INTERNAL_API_ID)
#error CANSM_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define CANSM_INTERNAL_API_ID DET_INTERNAL_API_ID



/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined CANSM_INSTANCE_ID)      /* To prevent double declaration */
#error CANSM_INSTANCE_ID already defined!
#endif /* #if (defined CANSM_INSTANCE_ID) */

/** \brief CanSM instance id */
#define CANSM_INSTANCE_ID 0U



/*------------------[Function-Like macros]-----------*/

#if (CANSM_DEV_ERROR_DETECT == STD_ON)

#if (defined CANSM_DET)      /* To prevent double declaration */
#error CANSM_DET already defined!
#endif /* #if (defined CANSM_DET) */

/** \brief Shortcut for DET calls */
#define CANSM_DET(sid, errid) CANSM_DET_WITH_INSTANCEID( CANSM_INSTANCE_ID, (sid), (errid) )

#if (defined CANSM_DET_WITH_INSTANCEID)      /* To prevent double declaration */
#error CANSM_DET_WITH_INSTANCEID already defined!
#endif /* #if (defined CANSM_DET_WITH_INSTANCEID) */

/** \brief Shortcut for DET calls including custom instance id */
#define CANSM_DET_WITH_INSTANCEID(instanceid, sid, errid) \
    (void)Det_ReportError( CANSM_MODULE_ID, \
      (instanceid), \
      (sid), \
      (errid) \
    )
#endif

#if (defined CANSM_REPORT_DEM_BUS_OFF_PASSED)      /* To prevent double declaration */
#error CANSM_REPORT_DEM_BUS_OFF_PASSED already defined!
#endif /* #if (defined CANSM_REPORT_DEM_BUS_OFF_PASSED) */

/** \brief Calls passed bus-off Dem event, if event is configured for this network
 *
 *  \param[in] nw Network index
 */

#if ( CANSM_PROD_ERR_HANDLING_BUS_OFF == TS_PROD_ERR_REP_TO_DEM )
#define CANSM_REPORT_DEM_BUS_OFF_PASSED(nw) \
  do \
  { \
    if (CANSM_NW_CONFIG_DEM_EVENT_ID_BUS_OFF((nw)) != CANSM_INVALID_DEM_EVENTID) \
    { \
      Dem_ReportErrorStatus(CANSM_NW_CONFIG_DEM_EVENT_ID_BUS_OFF((nw)), DEM_EVENT_STATUS_PASSED); \
    } \
  } \
  while (FALSE)
#else /* CANSM_PROD_ERR_HANDLING_BUS_OFF == TS_PROD_ERR_REP_TO_DEM */
#define CANSM_REPORT_DEM_BUS_OFF_PASSED(nw)
#endif /* CANSM_PROD_ERR_HANDLING_BUS_OFF == TS_PROD_ERR_REP_TO_DEM */

#if (defined CANSM_REPORT_DEM_BUS_OFF_FAILED)      /* To prevent double declaration */
#error CANSM_REPORT_DEM_BUS_OFF_FAILED already defined!
#endif /* #if (defined CANSM_REPORT_DEM_BUS_OFF_FAILED) */

/** \brief Calls failed bus-off Dem event, if event is configured for this network
 *
 *  \param[in] nw Network index
 */
#if ( CANSM_PROD_ERR_HANDLING_BUS_OFF == TS_PROD_ERR_REP_TO_DEM )
#define CANSM_REPORT_DEM_BUS_OFF_FAILED(nw) \
  do \
  { \
    if (CANSM_NW_CONFIG_DEM_EVENT_ID_BUS_OFF((nw)) != CANSM_INVALID_DEM_EVENTID) \
    { \
      Dem_ReportErrorStatus(CANSM_NW_CONFIG_DEM_EVENT_ID_BUS_OFF((nw)), DEM_EVENT_STATUS_PREFAILED); \
    } \
  } \
  while (FALSE)
#else /* CANSM_PROD_ERR_HANDLING_BUS_OFF == TS_PROD_ERR_REP_TO_DEM */
#define CANSM_REPORT_DEM_BUS_OFF_FAILED(nw)
#endif /* CANSM_PROD_ERR_HANDLING_BUS_OFF == TS_PROD_ERR_REP_TO_DEM */

#if (defined CANSM_ENTER_CRITICAL_SECTION)      /* To prevent double declaration */
#error CANSM_ENTER_CRITICAL_SECTION already defined!
#endif /* #if (defined CANSM_ENTER_CRITICAL_SECTION) */

/** \brief Enter critical section */
#define CANSM_ENTER_CRITICAL_SECTION() SchM_Enter_CanSM_SCHM_CANSM_EXCLUSIVE_AREA_0()

#if (defined CANSM_EXIT_CRITICAL_SECTION)      /* To prevent double declaration */
#error CANSM_EXIT_CRITICAL_SECTION already defined!
#endif /* #if (defined CANSM_EXIT_CRITICAL_SECTION) */

/** \brief Exit critical section */
#define CANSM_EXIT_CRITICAL_SECTION() SchM_Exit_CanSM_SCHM_CANSM_EXCLUSIVE_AREA_0()

/*==================[type definitions]======================================*/


/*==================[external constants]====================================*/


/*==================[external data]=========================================*/


/*==================[external function declarations]========================*/

#define CANSM_START_SEC_CODE
#include <CanSM_MemMap.h>

/**
 * \brief Checks if in the passed state, the module is waiting for a mode indication.
 *
 * If the module is waiting for a mode indication, CanSM_RequestComMode will not change
 * shared data and only read CANSM_NW_INFO_STATE and CANSM_NW_INFO_INTERNALSTATE and
 * CANSM_NW_INFO_BUSOFF.
 *
 * \return TRUE if and only if the module is waiting for a mode indication in this state.
 */
extern FUNC(boolean, CANSM_CODE) CanSM_IsWaitingForModeIndication(
  const CanSM_InternalStateType state
);

/**
 * \brief Executes all functionality of the main function for the network passed as parameter.
 *
 * \param[in] nw Network index
 */
extern FUNC(void, CANSM_CODE) CanSM_MainFunctionInternal(
  const CanSM_NetworkIndexType nw
);

/**
 * \brief Processes all state transitions.
 *
 * \pre Mode repetition counter must be greater than zero if waiting for a mode indication.
 *
 * Invoked if a CanSM_Init, CanSM_RequestComMode or CanSM_MainFunction invocation for
 * this network causes state transitions. Executes the whole sequence of transitions processed
 * in this API function invocation.
 *
 * During an invocation of this function (i.e. while executing a transition), other
 * CanSM_RequestComMode/CanSM_MainFunction invocations will not modify shared data and only read
 * CANSM_NW_INFO_STATE and CANSM_NW_INFO_INTERNALSTATE and CANSM_NW_INFO_BUSOFF (except timer
 * CANSM_NW_INFO_TIMER).
 *
 * Before exiting, this function updates CANSM_NW_INFO_STATE and CANSM_NW_INFO_INTERNALSTATE
 * in the trailing critical section, if needed.
 *
 * \param[in] nw Network index
 * \param[in] initialTransition First transition of the sequence
 * \param[in] invokedInMainFunction TRUE if and only if in CanSM main function API context
 */
extern FUNC(void, CANSM_CODE) CanSM_DoTransitionSequence(
  const CanSM_NetworkIndexType nw,
  const CanSM_TransitionType initialTransition,
  const boolean invokedInMainFunction
);

/**
 * \brief Verify which transition shall be performed next, dependent on the
 * the requested (stored) ComM mode and the current internal CanSM state.
 *
 * \param[in] reqMode requested ComM mode
 * \param[in] internalState current internal CanSM state
 * \param[out] nextTransition next transition to be performed (CANSM_T_NONE means no transition)
 */
extern FUNC(void, CANSM_CODE) CanSM_VerifyRequestedComMode
(
  const ComM_ModeType reqMode,
  const CanSM_InternalStateType internalState,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

#define CANSM_STOP_SEC_CODE
#include <CanSM_MemMap.h>


#endif /* ifndef CANSM_INTERNAL_H */
/*==================[end of file]===========================================*/



