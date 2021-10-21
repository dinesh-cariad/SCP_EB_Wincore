/**
 * \file
 *
 * \brief AUTOSAR Dcm
 *
 * This file contains the implementation of the AUTOSAR
 * module Dcm.
 *
 * \version 5.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef DCM_DEFPROG_CFG_H
#define DCM_DEFPROG_CFG_H

/*==================[includes]==============================================*/

[!IF "as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmGeneral/DcmDevErrorDetect = 'true'"!]
#include <Det.h>
[!ENDIF!]

/*==================[macros]================================================*/


/*------------------[generator part]----------------------------------------*/
[!LOOP "DcmConfigSet/*[1]"!]

/*------------------[Defensive programming]---------------------------------*/
[!SELECT "DcmDefensiveProgramming"!][!//

#if (defined DCM_DEFENSIVE_PROGRAMMING_ENABLED)
#error DCM_DEFENSIVE_PROGRAMMING_ENABLED is already defined
#endif
/** \brief Defensive programming usage
 **
 ** En- or disables the usage of the defensive programming */
#define DCM_DEFENSIVE_PROGRAMMING_ENABLED   [!//
[!IF "(../DcmGeneral/DcmDevErrorDetect  = 'true') and (DcmDefProgEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DCM_PRECONDITION_ASSERT_ENABLED)
#error DCM_PRECONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Precondition assertion usage
 **
 ** En- or disables the usage of precondition assertion checks */
#define DCM_PRECONDITION_ASSERT_ENABLED     [!//
[!IF "(../DcmGeneral/DcmDevErrorDetect  = 'true') and (DcmDefProgEnabled = 'true') and (DcmPrecondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DCM_POSTCONDITION_ASSERT_ENABLED)
#error DCM_POSTCONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Postcondition assertion usage
 **
 ** En- or disables the usage of postcondition assertion checks */
#define DCM_POSTCONDITION_ASSERT_ENABLED    [!//
[!IF "(../DcmGeneral/DcmDevErrorDetect  = 'true') and (DcmDefProgEnabled = 'true') and (DcmPostcondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DCM_UNREACHABLE_CODE_ASSERT_ENABLED)
#error DCM_UNREACHABLE_CODE_ASSERT_ENABLED is already defined
#endif
/** \brief Unreachable code assertion usage
 **
 ** En- or disables the usage of unreachable code assertion checks */
#define DCM_UNREACHABLE_CODE_ASSERT_ENABLED [!//
[!IF "(../DcmGeneral/DcmDevErrorDetect  = 'true') and (DcmDefProgEnabled = 'true') and (DcmUnreachAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DCM_INVARIANT_ASSERT_ENABLED)
#error DCM_INVARIANT_ASSERT_ENABLED is already defined
#endif
/** \brief Invariant assertion usage
 **
 ** En- or disables the usage of invariant assertion checks */
#define DCM_INVARIANT_ASSERT_ENABLED        [!//
[!IF "(../DcmGeneral/DcmDevErrorDetect  = 'true') and (DcmDefProgEnabled = 'true') and (DcmInvariantAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DCM_STATIC_ASSERT_ENABLED)
#error DCM_STATIC_ASSERT_ENABLED is already defined
#endif
/** \brief Static assertion usage
 **
 ** En- or disables the usage of static assertion checks */
#define DCM_STATIC_ASSERT_ENABLED           [!//
[!IF "(../DcmGeneral/DcmDevErrorDetect  = 'true') and (DcmDefProgEnabled = 'true') and (DcmStaticAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!][!//

[!ENDLOOP!]
/*------------------[static part]-------------------------------------------*/


/*------------------------[Defensive programming]----------------------------*/

#if (defined DCM_PRECONDITION_ASSERT)
#error DCM_PRECONDITION_ASSERT is already defined
#endif

#if (defined DCM_PRECONDITION_ASSERT_NO_EVAL)
#error DCM_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (DCM_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define DCM_PRECONDITION_ASSERT(Condition, ApiId) \
  DET_PRECONDITION_ASSERT((Condition), DCM_MODULE_ID, DCM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define DCM_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL((Condition), DCM_MODULE_ID, DCM_INSTANCE_ID, (ApiId))
#else
#define DCM_PRECONDITION_ASSERT(Condition, ApiId)
#define DCM_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined DCM_POSTCONDITION_ASSERT)
#error DCM_POSTCONDITION_ASSERT is already defined
#endif

#if (defined DCM_POSTCONDITION_ASSERT_NO_EVAL)
#error DCM_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (DCM_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define DCM_POSTCONDITION_ASSERT(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT((Condition), DCM_MODULE_ID, DCM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define DCM_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL((Condition), DCM_MODULE_ID, DCM_INSTANCE_ID, (ApiId))
#else
#define DCM_POSTCONDITION_ASSERT(Condition, ApiId)
#define DCM_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined DCM_INVARIANT_ASSERT)
#error DCM_INVARIANT_ASSERT is already defined
#endif

#if (defined DCM_INVARIANT_ASSERT_NO_EVAL)
#error DCM_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (DCM_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define DCM_INVARIANT_ASSERT(Condition, ApiId) \
  DET_INVARIANT_ASSERT((Condition), DCM_MODULE_ID, DCM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define DCM_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL((Condition), DCM_MODULE_ID, DCM_INSTANCE_ID, (ApiId))
#else
#define DCM_INVARIANT_ASSERT(Condition, ApiId)
#define DCM_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined DCM_STATIC_ASSERT)
# error DCM_STATIC_ASSERT is already defined
#endif
#if (DCM_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated */
# define DCM_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define DCM_STATIC_ASSERT(expr)
#endif

#if (defined DCM_UNREACHABLE_CODE_ASSERT)
#error DCM_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (DCM_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 **

 ** \param[in] ApiId Service ID of the API function */
#define DCM_UNREACHABLE_CODE_ASSERT(ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(DCM_MODULE_ID, DCM_INSTANCE_ID, (ApiId))
#else
#define DCM_UNREACHABLE_CODE_ASSERT(ApiId)
#endif

#if (defined DCM_INTERNAL_API_ID)
#error DCM_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define DCM_INTERNAL_API_ID DET_INTERNAL_API_ID



/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#endif /* ifndef DCM_DEFPROG_CFG_H */
/*==================[end of file]===========================================*/
