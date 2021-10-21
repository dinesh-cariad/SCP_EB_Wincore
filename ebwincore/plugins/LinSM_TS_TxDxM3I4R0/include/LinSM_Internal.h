/**
 * \file
 *
 * \brief AUTOSAR LinSM
 *
 * This file contains the implementation of the AUTOSAR
 * module LinSM.
 *
 * \version 3.4.19
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef LINSM_INTERNAL_H
#define LINSM_INTERNAL_H

/*==================[inclusions]============================================*/

#include <TSAutosar.h>         /* EB specific standard types */
#include <ComStack_Types.h>    /* AUTOSAR communication stack types */
#include <LinSM_Version.h>     /* this module's version declaration */
#include <LinSM_Api.h>         /* API and public configuration */
#include <LinSM_Cbk.h>         /* API callout prototypes */
#include <LinSM_InternalCfg.h> /* internal configuration */
#include <SchM_LinSM.h>        /* SchM declarations */ /* !LINKSTO LinSM.ASR40.LINSM195,1 */
#include <ComM_BusSm.h>        /* ComM BusSm API function prototypes */

#if (LINSM_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>               /* Development Error Tracer */
#endif

/*==================[defensive programming]==================================*/

#if (LINSM_DEV_ERROR_DETECT == STD_ON)




/*------------------------[Defensive programming]----------------------------*/

#if (defined LINSM_PRECONDITION_ASSERT)
#error LINSM_PRECONDITION_ASSERT is already defined
#endif

#if (defined LINSM_PRECONDITION_ASSERT_NO_EVAL)
#error LINSM_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (LINSM_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define LINSM_PRECONDITION_ASSERT(Condition, ApiId) \
  DET_PRECONDITION_ASSERT((Condition), LINSM_MODULE_ID, LINSM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define LINSM_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL((Condition), LINSM_MODULE_ID, LINSM_INSTANCE_ID, (ApiId))
#else
#define LINSM_PRECONDITION_ASSERT(Condition, ApiId)
#define LINSM_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined LINSM_POSTCONDITION_ASSERT)
#error LINSM_POSTCONDITION_ASSERT is already defined
#endif

#if (defined LINSM_POSTCONDITION_ASSERT_NO_EVAL)
#error LINSM_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (LINSM_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define LINSM_POSTCONDITION_ASSERT(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT((Condition), LINSM_MODULE_ID, LINSM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define LINSM_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL((Condition), LINSM_MODULE_ID, LINSM_INSTANCE_ID, (ApiId))
#else
#define LINSM_POSTCONDITION_ASSERT(Condition, ApiId)
#define LINSM_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined LINSM_INVARIANT_ASSERT)
#error LINSM_INVARIANT_ASSERT is already defined
#endif

#if (defined LINSM_INVARIANT_ASSERT_NO_EVAL)
#error LINSM_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (LINSM_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define LINSM_INVARIANT_ASSERT(Condition, ApiId) \
  DET_INVARIANT_ASSERT((Condition), LINSM_MODULE_ID, LINSM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define LINSM_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL((Condition), LINSM_MODULE_ID, LINSM_INSTANCE_ID, (ApiId))
#else
#define LINSM_INVARIANT_ASSERT(Condition, ApiId)
#define LINSM_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined LINSM_STATIC_ASSERT)
# error LINSM_STATIC_ASSERT is already defined
#endif
#if (LINSM_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated */
# define LINSM_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define LINSM_STATIC_ASSERT(expr)
#endif

#if (defined LINSM_UNREACHABLE_CODE_ASSERT)
#error LINSM_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (LINSM_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 **

 ** \param[in] ApiId Service ID of the API function */
#define LINSM_UNREACHABLE_CODE_ASSERT(ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(LINSM_MODULE_ID, LINSM_INSTANCE_ID, (ApiId))
#else
#define LINSM_UNREACHABLE_CODE_ASSERT(ApiId)
#endif

#if (defined LINSM_INTERNAL_API_ID)
#error LINSM_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define LINSM_INTERNAL_API_ID DET_INTERNAL_API_ID


#endif /* LINSM_DEV_ERROR_DETECT == STD_ON */

/*==================[macros]================================================*/

#if (defined LINSM_GLOBAL_UNINIT) /* to prevent double declaration */
#error LINSM_GLOBAL_UNINIT already defined
#endif /* if (defined LINSM_GLOBAL_UNINIT) */

#if (defined LINSM_GLOBAL_INIT) /* to prevent double declaration */
#error LINSM_GLOBAL_INIT already defined
#endif /* if (defined LINSM_GLOBAL_INIT) */

#if (defined LINSM_DET) /* to prevent double declaration */
#error LINSM_DET already defined
#endif /* if (defined LINSM_DET) */

#if (defined LINSM_ENTER_CRITICAL_SECTION) /* to prevent double declaration */
#error LINSM_ENTER_CRITICAL_SECTION already defined
#endif /* if (defined LINSM_ENTER_CRITICAL_SECTION) */

#if (defined LINSM_LEAVE_CRITICAL_SECTION) /* to prevent double declaration */
#error LINSM_LEAVE_CRITICAL_SECTION already defined
#endif /* if (defined LINSM_LEAVE_CRITICAL_SECTION) */

/*------------------[LINSM_INSTANCE_ID]-----------*/
#ifndef LINSM_INSTANCE_ID
#define LINSM_INSTANCE_ID 0U
#endif
/*------------------[LinSM type macros]-----------*/

/** \brief LinSM uninitialized (used in LinSM_GlobalStateType) */
#define LINSM_GLOBAL_UNINIT 0U

/** \brief LinSM initialized (used in LinSM_GlobalStateType) */
#define LINSM_GLOBAL_INIT   1U

/*------------------[Function-like macros]-----------*/

#if (LINSM_DEV_ERROR_DETECT == STD_ON)
/** \brief Shortcut for DET calls */
#define LINSM_DET(sid, errid)  do \
  { \
    (void)Det_ReportError(LINSM_MODULE_ID, LINSM_INSTANCE_ID, (sid), (errid)); \
  } while(0)
#endif

/** \brief Call the SchM to enter a critical section. */
#define LINSM_ENTER_CRITICAL_SECTION()  do  \
  {  \
    SchM_Enter_LinSM_SCHM_LINSM_EXCLUSIVE_AREA_0();  \
  } while (0)

/** \brief Call the SchM to leave a critical section. */
#define LINSM_LEAVE_CRITICAL_SECTION()  do  \
  {  \
    SchM_Exit_LinSM_SCHM_LINSM_EXCLUSIVE_AREA_0();  \
  } while (0)


/*==================[type definitions]======================================*/

/** \brief type to hold global initialization state */
typedef uint8 LinSM_GlobalStateType;

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#define LINSM_START_SEC_VAR_INIT_8
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */

/** \brief initialization status */
extern VAR(LinSM_GlobalStateType, LINSM_VAR) LinSM_GlobalState;

#define LINSM_STOP_SEC_VAR_INIT_8
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */

/*==================[external function declarations]========================*/

#define LINSM_START_SEC_CODE
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */

extern FUNC(boolean, LINSM_CODE) LinSM_IsScheduleValid
(
  LinSM_ChannelIdType id,
  LinIf_SchHandleType schedule
);

#if (LINSM_SLEEPSUPPORT_TYPE != LINSM_NONE)

/** \brief internal transition from LINSM_NO_COM into LINSM_WAKEUP
 *
 * performs calls to LinIf and sets channel state. used for channels with
 * sleep mode support.
 *
 * \param[in] id  LinSM channel id of channel to operate on
 */
FUNC(void, LINSM_CODE) LinSM_WakeUp
(
  LinSM_ChannelIdType id
);

/** \brief internal transition from LINSM_FULL_COM into LINSM_GOTO_SLEEP
 *
 * performs calls to LinIf and sets channel state. used for channels with
 * sleep mode support.
 *
 * \param[in] id  LinSM channel id of channel to operate on
 */
FUNC(void, LINSM_CODE) LinSM_GotoSleep
(
  LinSM_ChannelIdType id
);

#endif /* LINSM_SLEEPSUPPORT_TYPE != LINSM_NONE */

#define LINSM_STOP_SEC_CODE
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */

#endif /* ifndef LINSM_INTERNAL_H */
/*==================[end of file]===========================================*/

