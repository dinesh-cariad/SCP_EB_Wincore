/**
 * \file
 *
 * \brief AUTOSAR Tls
 *
 * This file contains the implementation of the AUTOSAR
 * module Tls.
 *
 * \version 1.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef TCPIP_DEFPROG_H
#define TCPIP_DEFPROG_H





/*------------------------[Defensive programming]----------------------------*/

#if (defined TLS_PRECONDITION_ASSERT)
#error TLS_PRECONDITION_ASSERT is already defined
#endif

#if (defined TLS_PRECONDITION_ASSERT_NO_EVAL)
#error TLS_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (TLS_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define TLS_PRECONDITION_ASSERT(Condition, ApiId) \
  DET_PRECONDITION_ASSERT((Condition), TLS_MODULE_ID, TLS_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define TLS_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL((Condition), TLS_MODULE_ID, TLS_INSTANCE_ID, (ApiId))
#else
#define TLS_PRECONDITION_ASSERT(Condition, ApiId)
#define TLS_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined TLS_POSTCONDITION_ASSERT)
#error TLS_POSTCONDITION_ASSERT is already defined
#endif

#if (defined TLS_POSTCONDITION_ASSERT_NO_EVAL)
#error TLS_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (TLS_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define TLS_POSTCONDITION_ASSERT(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT((Condition), TLS_MODULE_ID, TLS_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define TLS_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL((Condition), TLS_MODULE_ID, TLS_INSTANCE_ID, (ApiId))
#else
#define TLS_POSTCONDITION_ASSERT(Condition, ApiId)
#define TLS_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined TLS_INVARIANT_ASSERT)
#error TLS_INVARIANT_ASSERT is already defined
#endif

#if (defined TLS_INVARIANT_ASSERT_NO_EVAL)
#error TLS_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (TLS_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define TLS_INVARIANT_ASSERT(Condition, ApiId) \
  DET_INVARIANT_ASSERT((Condition), TLS_MODULE_ID, TLS_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define TLS_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL((Condition), TLS_MODULE_ID, TLS_INSTANCE_ID, (ApiId))
#else
#define TLS_INVARIANT_ASSERT(Condition, ApiId)
#define TLS_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined TLS_STATIC_ASSERT)
# error TLS_STATIC_ASSERT is already defined
#endif
#if (TLS_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated */
# define TLS_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define TLS_STATIC_ASSERT(expr)
#endif

#if (defined TLS_UNREACHABLE_CODE_ASSERT)
#error TLS_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (TLS_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 **

 ** \param[in] ApiId Service ID of the API function */
#define TLS_UNREACHABLE_CODE_ASSERT(ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(TLS_MODULE_ID, TLS_INSTANCE_ID, (ApiId))
#else
#define TLS_UNREACHABLE_CODE_ASSERT(ApiId)
#endif

#if (defined TLS_INTERNAL_API_ID)
#error TLS_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define TLS_INTERNAL_API_ID DET_INTERNAL_API_ID



#endif /* #ifndef TCPIP_DEFPROG_H */
