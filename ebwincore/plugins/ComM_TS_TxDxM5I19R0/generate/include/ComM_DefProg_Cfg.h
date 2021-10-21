/**
 * \file
 *
 * \brief AUTOSAR ComM
 *
 * This file contains the implementation of the AUTOSAR
 * module ComM.
 *
 * \version 5.19.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef COMM_DEFPROG_CFG_H
#define COMM_DEFPROG_CFG_H

/*==================[includes]==============================================*/
[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMDevErrorDetect = 'true'"!]
#include <Det.h>
[!ENDIF!]
#include <Std_Types.h>
/*==================[macros]================================================*/



/*------------------[generator part]----------------------------------------*/

/*------------------[Defensive programming]---------------------------------*/
[!SELECT "ComMDefensiveProgramming"!][!//

#if (defined COMM_DEFENSIVE_PROGRAMMING_ENABLED)
#error COMM_DEFENSIVE_PROGRAMMING_ENABLED is already defined
#endif
/** \brief Defensive programming usage
 **
 ** En- or disables the usage of the defensive programming */
#define COMM_DEFENSIVE_PROGRAMMING_ENABLED   [!//
[!IF "(../ComMGeneral/ComMDevErrorDetect  = 'true') and (ComMDefProgEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined COMM_PRECONDITION_ASSERT_ENABLED)
#error COMM_PRECONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Precondition assertion usage
 **
 ** En- or disables the usage of precondition assertion checks */
#define COMM_PRECONDITION_ASSERT_ENABLED     [!//
[!IF "(../ComMGeneral/ComMDevErrorDetect  = 'true') and (ComMDefProgEnabled = 'true') and (ComMPrecondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined COMM_POSTCONDITION_ASSERT_ENABLED)
#error COMM_POSTCONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Postcondition assertion usage
 **
 ** En- or disables the usage of postcondition assertion checks */
#define COMM_POSTCONDITION_ASSERT_ENABLED    [!//
[!IF "(../ComMGeneral/ComMDevErrorDetect  = 'true') and (ComMDefProgEnabled = 'true') and (ComMPostcondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined COMM_UNREACHABLE_CODE_ASSERT_ENABLED)
#error COMM_UNREACHABLE_CODE_ASSERT_ENABLED is already defined
#endif
/** \brief Unreachable code assertion usage
 **
 ** En- or disables the usage of unreachable code assertion checks */
#define COMM_UNREACHABLE_CODE_ASSERT_ENABLED [!//
[!IF "(../ComMGeneral/ComMDevErrorDetect  = 'true') and (ComMDefProgEnabled = 'true') and (ComMUnreachAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined COMM_INVARIANT_ASSERT_ENABLED)
#error COMM_INVARIANT_ASSERT_ENABLED is already defined
#endif
/** \brief Invariant assertion usage
 **
 ** En- or disables the usage of invariant assertion checks */
#define COMM_INVARIANT_ASSERT_ENABLED        [!//
[!IF "(../ComMGeneral/ComMDevErrorDetect  = 'true') and (ComMDefProgEnabled = 'true') and (ComMInvariantAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined COMM_STATIC_ASSERT_ENABLED)
#error COMM_STATIC_ASSERT_ENABLED is already defined
#endif
/** \brief Static assertion usage
 **
 ** En- or disables the usage of static assertion checks */
#define COMM_STATIC_ASSERT_ENABLED           [!//
[!IF "(../ComMGeneral/ComMDevErrorDetect  = 'true') and (ComMDefProgEnabled = 'true') and (ComMStaticAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!][!//

/*------------------[static part]-------------------------------------------*/


/*------------------------[Defensive programming]----------------------------*/

#if (defined COMM_PRECONDITION_ASSERT)
#error COMM_PRECONDITION_ASSERT is already defined
#endif

#if (defined COMM_PRECONDITION_ASSERT_NO_EVAL)
#error COMM_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (COMM_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define COMM_PRECONDITION_ASSERT(Condition, ApiId) \
  DET_PRECONDITION_ASSERT((Condition), COMM_MODULE_ID, COMM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define COMM_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL((Condition), COMM_MODULE_ID, COMM_INSTANCE_ID, (ApiId))
#else
#define COMM_PRECONDITION_ASSERT(Condition, ApiId)
#define COMM_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined COMM_POSTCONDITION_ASSERT)
#error COMM_POSTCONDITION_ASSERT is already defined
#endif

#if (defined COMM_POSTCONDITION_ASSERT_NO_EVAL)
#error COMM_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (COMM_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define COMM_POSTCONDITION_ASSERT(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT((Condition), COMM_MODULE_ID, COMM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define COMM_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL((Condition), COMM_MODULE_ID, COMM_INSTANCE_ID, (ApiId))
#else
#define COMM_POSTCONDITION_ASSERT(Condition, ApiId)
#define COMM_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined COMM_INVARIANT_ASSERT)
#error COMM_INVARIANT_ASSERT is already defined
#endif

#if (defined COMM_INVARIANT_ASSERT_NO_EVAL)
#error COMM_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (COMM_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define COMM_INVARIANT_ASSERT(Condition, ApiId) \
  DET_INVARIANT_ASSERT((Condition), COMM_MODULE_ID, COMM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define COMM_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL((Condition), COMM_MODULE_ID, COMM_INSTANCE_ID, (ApiId))
#else
#define COMM_INVARIANT_ASSERT(Condition, ApiId)
#define COMM_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined COMM_STATIC_ASSERT)
# error COMM_STATIC_ASSERT is already defined
#endif
#if (COMM_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated */
# define COMM_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define COMM_STATIC_ASSERT(expr)
#endif

#if (defined COMM_UNREACHABLE_CODE_ASSERT)
#error COMM_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (COMM_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 **

 ** \param[in] ApiId Service ID of the API function */
#define COMM_UNREACHABLE_CODE_ASSERT(ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(COMM_MODULE_ID, COMM_INSTANCE_ID, (ApiId))
#else
#define COMM_UNREACHABLE_CODE_ASSERT(ApiId)
#endif

#if (defined COMM_INTERNAL_API_ID)
#error COMM_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define COMM_INTERNAL_API_ID DET_INTERNAL_API_ID



/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#endif /* ifndef COMM_DEFPROG_CFG_H */
/*==================[end of file]===========================================*/
