#ifndef STBM_INT_H
#define STBM_INT_H

/**
 * \file
 *
 * \brief AUTOSAR StbM
 *
 * This file contains the implementation of the AUTOSAR
 * module StbM.
 *
 * \version 3.1.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */


/*==[Includes]================================================================*/

#include <StbM_Int_Cfg.h>

/* if DET is switched on, include Det.h*/
#if(STBM_DEV_ERROR_DETECT == STD_ON)
/* include Det header for the DET calls. */
/* !LINKSTO StbM.dsn.DetInclusion,1 */
#include <Det.h>
#endif /* STBM_DEV_ERROR_DETECT == STD_ON */

/*==[Macros]==================================================================*/

#if(STBM_DEV_ERROR_DETECT == STD_ON)
#if (defined STBM_DET_REPORTERROR)
#error STBM_DET_REPORTERROR is already defined.
#endif
/** \brief Abstracts the Det_ReportError
**
** Det_ReportError is a service function and requires to have a return
** value which is always E_OK (see DET009). It is therefore o.k. to cast
** the value to void.
**
** \param[in] ApiId The ID of the calling function
** \param[in] ErrorId The error code reported by the calling function
**
*/
#define STBM_DET_REPORTERROR(ApiId, ErrorId) \
   ((void)Det_ReportError (STBM_MODULE_ID, STBM_INSTANCE_ID, (ApiId), (ErrorId)))
#else
#define STBM_DET_REPORTERROR(ApiId, ErrorId)
#endif /* STBM_DEV_ERROR_DETECT == STD_ON */

/*==================[defensive programming macros]=========================*/





/*------------------------[Defensive programming]----------------------------*/

#if (defined STBM_PRECONDITION_ASSERT)
#error STBM_PRECONDITION_ASSERT is already defined
#endif

#if (defined STBM_PRECONDITION_ASSERT_NO_EVAL)
#error STBM_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (STBM_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define STBM_PRECONDITION_ASSERT(Condition, ApiId) \
  DET_PRECONDITION_ASSERT((Condition), STBM_MODULE_ID, STBM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define STBM_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL((Condition), STBM_MODULE_ID, STBM_INSTANCE_ID, (ApiId))
#else
#define STBM_PRECONDITION_ASSERT(Condition, ApiId)
#define STBM_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined STBM_POSTCONDITION_ASSERT)
#error STBM_POSTCONDITION_ASSERT is already defined
#endif

#if (defined STBM_POSTCONDITION_ASSERT_NO_EVAL)
#error STBM_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (STBM_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define STBM_POSTCONDITION_ASSERT(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT((Condition), STBM_MODULE_ID, STBM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define STBM_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL((Condition), STBM_MODULE_ID, STBM_INSTANCE_ID, (ApiId))
#else
#define STBM_POSTCONDITION_ASSERT(Condition, ApiId)
#define STBM_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined STBM_INVARIANT_ASSERT)
#error STBM_INVARIANT_ASSERT is already defined
#endif

#if (defined STBM_INVARIANT_ASSERT_NO_EVAL)
#error STBM_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (STBM_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define STBM_INVARIANT_ASSERT(Condition, ApiId) \
  DET_INVARIANT_ASSERT((Condition), STBM_MODULE_ID, STBM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define STBM_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL((Condition), STBM_MODULE_ID, STBM_INSTANCE_ID, (ApiId))
#else
#define STBM_INVARIANT_ASSERT(Condition, ApiId)
#define STBM_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined STBM_STATIC_ASSERT)
# error STBM_STATIC_ASSERT is already defined
#endif
#if (STBM_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated */
# define STBM_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define STBM_STATIC_ASSERT(expr)
#endif

#if (defined STBM_UNREACHABLE_CODE_ASSERT)
#error STBM_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (STBM_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 **

 ** \param[in] ApiId Service ID of the API function */
#define STBM_UNREACHABLE_CODE_ASSERT(ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(STBM_MODULE_ID, STBM_INSTANCE_ID, (ApiId))
#else
#define STBM_UNREACHABLE_CODE_ASSERT(ApiId)
#endif

#if (defined STBM_INTERNAL_API_ID)
#error STBM_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define STBM_INTERNAL_API_ID DET_INTERNAL_API_ID



/*==[Types]===================================================================*/

/*==[Declaration of functions with internal linkage]==========================*/

/*==[Constants with internal linkage]=========================================*/

/*==[Variables with internal linkage]=========================================*/

/*==[Constants with external linkage]=========================================*/

/*==[Variables with external linkage]=========================================*/

/*==[Definition of functions with external linkage]===========================*/

/*==[Definition of functions with internal linkage]===========================*/

/*==================[end of file]===========================================*/


#endif /* ifndef STBM_INT_H */
