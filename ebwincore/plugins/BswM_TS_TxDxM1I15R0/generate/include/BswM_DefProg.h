/**
 * \file
 *
 * \brief AUTOSAR BswM
 *
 * This file contains the implementation of the AUTOSAR
 * module BswM.
 *
 * \version 1.15.6
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef BSWM_DEFPROG_H
#define BSWM_DEFPROG_H

/*==================[includes]==============================================*/

[!IF "as:modconf('BswM')[1]/BswMGeneral/BswMDevErrorDetect = 'true'"!]
#include <Det.h>
[!ENDIF!]

/*==================[macros]================================================*/


/*------------------[generator part]----------------------------------------*/
[!SELECT "as:modconf('BswM')[1]"!]

/*------------------[Defensive programming]---------------------------------*/
[!SELECT "BswMDefensiveProgramming"!][!//

#if (defined BSWM_DEFENSIVE_PROGRAMMING_ENABLED)
#error BSWM_DEFENSIVE_PROGRAMMING_ENABLED is already defined
#endif
/** \brief Defensive programming usage
 **
 ** En- or disables the usage of the defensive programming */
#define BSWM_DEFENSIVE_PROGRAMMING_ENABLED   [!//
[!IF "(../BswMGeneral/BswMDevErrorDetect  = 'true') and (BswMDefProgEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined BSWM_PRECONDITION_ASSERT_ENABLED)
#error BSWM_PRECONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Precondition assertion usage
 **
 ** En- or disables the usage of precondition assertion checks */
#define BSWM_PRECONDITION_ASSERT_ENABLED     [!//
[!IF "(../BswMGeneral/BswMDevErrorDetect  = 'true') and (BswMDefProgEnabled = 'true') and (BswMPrecondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined BSWM_POSTCONDITION_ASSERT_ENABLED)
#error BSWM_POSTCONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Postcondition assertion usage
 **
 ** En- or disables the usage of postcondition assertion checks */
#define BSWM_POSTCONDITION_ASSERT_ENABLED    [!//
[!IF "(../BswMGeneral/BswMDevErrorDetect  = 'true') and (BswMDefProgEnabled = 'true') and (BswMPostcondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined BSWM_UNREACHABLE_CODE_ASSERT_ENABLED)
#error BSWM_UNREACHABLE_CODE_ASSERT_ENABLED is already defined
#endif
/** \brief Unreachable code assertion usage
 **
 ** En- or disables the usage of unreachable code assertion checks */
#define BSWM_UNREACHABLE_CODE_ASSERT_ENABLED [!//
[!IF "(../BswMGeneral/BswMDevErrorDetect  = 'true') and (BswMDefProgEnabled = 'true') and (BswMUnreachAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined BSWM_INVARIANT_ASSERT_ENABLED)
#error BSWM_INVARIANT_ASSERT_ENABLED is already defined
#endif
/** \brief Invariant assertion usage
 **
 ** En- or disables the usage of invariant assertion checks */
#define BSWM_INVARIANT_ASSERT_ENABLED        [!//
[!IF "(../BswMGeneral/BswMDevErrorDetect  = 'true') and (BswMDefProgEnabled = 'true') and (BswMInvariantAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined BSWM_STATIC_ASSERT_ENABLED)
#error BSWM_STATIC_ASSERT_ENABLED is already defined
#endif
/** \brief Static assertion usage
 **
 ** En- or disables the usage of static assertion checks */
#define BSWM_STATIC_ASSERT_ENABLED           [!//
[!IF "(../BswMGeneral/BswMDevErrorDetect  = 'true') and (BswMDefProgEnabled = 'true') and (BswMStaticAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!][!//

[!ENDSELECT!]
/*------------------[static part]-------------------------------------------*/


/*------------------------[Defensive programming]----------------------------*/

#if (defined BSWM_PRECONDITION_ASSERT)
#error BSWM_PRECONDITION_ASSERT is already defined
#endif

#if (defined BSWM_PRECONDITION_ASSERT_NO_EVAL)
#error BSWM_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (BSWM_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **
 ** \param[in] InstId The BswM instance which peforms the assertion
 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define BSWM_PRECONDITION_ASSERT(InstId, Condition, ApiId) \
  DET_PRECONDITION_ASSERT((Condition), BSWM_MODULE_ID, (InstId), (ApiId))

/** \brief Report an assertion violation to Det
 **
 ** \param[in] InstId The BswM instance which peforms the assertion
 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define BSWM_PRECONDITION_ASSERT_NO_EVAL(InstId, Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL((Condition), BSWM_MODULE_ID, (InstId), (ApiId))
#else
#define BSWM_PRECONDITION_ASSERT(InstId, Condition, ApiId)
#define BSWM_PRECONDITION_ASSERT_NO_EVAL(InstId, Condition, ApiId)
#endif

#if (defined BSWM_POSTCONDITION_ASSERT)
#error BSWM_POSTCONDITION_ASSERT is already defined
#endif

#if (defined BSWM_POSTCONDITION_ASSERT_NO_EVAL)
#error BSWM_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (BSWM_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **
 ** \param[in] InstId The BswM instance which peforms the assertion
 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define BSWM_POSTCONDITION_ASSERT(InstId, Condition, ApiId) \
  DET_POSTCONDITION_ASSERT((Condition), BSWM_MODULE_ID, (InstId), (ApiId))

/** \brief Report an assertion violation to Det
 **
 ** \param[in] InstId The BswM instance which peforms the assertion
 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define BSWM_POSTCONDITION_ASSERT_NO_EVAL(InstId, Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL((Condition), BSWM_MODULE_ID, (InstId), (ApiId))
#else
#define BSWM_POSTCONDITION_ASSERT(InstId, Condition, ApiId)
#define BSWM_POSTCONDITION_ASSERT_NO_EVAL(InstId, Condition, ApiId)
#endif

#if (defined BSWM_INVARIANT_ASSERT)
#error BSWM_INVARIANT_ASSERT is already defined
#endif

#if (defined BSWM_INVARIANT_ASSERT_NO_EVAL)
#error BSWM_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (BSWM_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **
 ** \param[in] InstId The BswM instance which peforms the assertion
 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define BSWM_INVARIANT_ASSERT(InstId, Condition, ApiId) \
  DET_INVARIANT_ASSERT((Condition), BSWM_MODULE_ID, (InstId), (ApiId))

/** \brief Report an assertion violation to Det
 **
 ** \param[in] InstId The BswM instance which peforms the assertion
 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define BSWM_INVARIANT_ASSERT_NO_EVAL(InstId, Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL((Condition), BSWM_MODULE_ID, (InstId), (ApiId))
#else
#define BSWM_INVARIANT_ASSERT(InstId, Condition, ApiId)
#define BSWM_INVARIANT_ASSERT_NO_EVAL(InstId, Condition, ApiId)
#endif

#if (defined BSWM_STATIC_ASSERT)
# error BSWM_STATIC_ASSERT is already defined
#endif
#if (BSWM_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated */
# define BSWM_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define BSWM_STATIC_ASSERT(expr)
#endif

#if (defined BSWM_UNREACHABLE_CODE_ASSERT)
#error BSWM_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (BSWM_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 **
 ** \param[in] InstId The BswM instance which peforms the assertion
 ** \param[in] ApiId Service ID of the API function */
#define BSWM_UNREACHABLE_CODE_ASSERT(InstId, ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(BSWM_MODULE_ID, (InstId), (ApiId))
#else
#define BSWM_UNREACHABLE_CODE_ASSERT(InstId, ApiId)
#endif

#if (defined BSWM_INTERNAL_API_ID)
#error BSWM_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define BSWM_INTERNAL_API_ID DET_INTERNAL_API_ID



/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#endif /* ifndef BSWM_DEFPROG_H */
/*==================[end of file]===========================================*/
