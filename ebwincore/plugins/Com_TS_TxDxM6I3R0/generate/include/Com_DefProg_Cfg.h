/**
 * \file
 *
 * \brief AUTOSAR Com
 *
 * This file contains the implementation of the AUTOSAR
 * module Com.
 *
 * \version 6.3.50
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef COM_DEFPROG_CFG_H
#define COM_DEFPROG_CFG_H

/*==================[includes]==============================================*/

[!IF "as:modconf('Com')[1]/ComGeneral/ComConfigurationUseDet = 'true'"!]
#include <Det.h>
[!ENDIF!]

/*==================[macros]================================================*/



/*------------------[generator part]----------------------------------------*/

/*------------------[Defensive programming]---------------------------------*/
[!SELECT "ComDefensiveProgramming"!][!//

#if (defined COM_DEFENSIVE_PROGRAMMING_ENABLED)
#error COM_DEFENSIVE_PROGRAMMING_ENABLED is already defined
#endif
/** \brief Defensive programming usage
 **
 ** En- or disables the usage of the defensive programming */
#define COM_DEFENSIVE_PROGRAMMING_ENABLED   [!//
[!IF "(../ComGeneral/ComConfigurationUseDet  = 'true') and (ComDefProgEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined COM_PRECONDITION_ASSERT_ENABLED)
#error COM_PRECONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Precondition assertion usage
 **
 ** En- or disables the usage of precondition assertion checks */
#define COM_PRECONDITION_ASSERT_ENABLED     [!//
[!IF "(../ComGeneral/ComConfigurationUseDet  = 'true') and (ComDefProgEnabled = 'true') and (ComPrecondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined COM_POSTCONDITION_ASSERT_ENABLED)
#error COM_POSTCONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Postcondition assertion usage
 **
 ** En- or disables the usage of postcondition assertion checks */
#define COM_POSTCONDITION_ASSERT_ENABLED    [!//
[!IF "(../ComGeneral/ComConfigurationUseDet  = 'true') and (ComDefProgEnabled = 'true') and (ComPostcondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined COM_UNREACHABLE_CODE_ASSERT_ENABLED)
#error COM_UNREACHABLE_CODE_ASSERT_ENABLED is already defined
#endif
/** \brief Unreachable code assertion usage
 **
 ** En- or disables the usage of unreachable code assertion checks */
#define COM_UNREACHABLE_CODE_ASSERT_ENABLED [!//
[!IF "(../ComGeneral/ComConfigurationUseDet  = 'true') and (ComDefProgEnabled = 'true') and (ComUnreachAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined COM_INVARIANT_ASSERT_ENABLED)
#error COM_INVARIANT_ASSERT_ENABLED is already defined
#endif
/** \brief Invariant assertion usage
 **
 ** En- or disables the usage of invariant assertion checks */
#define COM_INVARIANT_ASSERT_ENABLED        [!//
[!IF "(../ComGeneral/ComConfigurationUseDet  = 'true') and (ComDefProgEnabled = 'true') and (ComInvariantAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined COM_STATIC_ASSERT_ENABLED)
#error COM_STATIC_ASSERT_ENABLED is already defined
#endif
/** \brief Static assertion usage
 **
 ** En- or disables the usage of static assertion checks */
#define COM_STATIC_ASSERT_ENABLED           [!//
[!IF "(../ComGeneral/ComConfigurationUseDet  = 'true') and (ComDefProgEnabled = 'true') and (ComStaticAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!][!//

/*------------------[static part]-------------------------------------------*/


/*------------------------[Defensive programming]----------------------------*/

#if (defined COM_PRECONDITION_ASSERT)
#error COM_PRECONDITION_ASSERT is already defined
#endif

#if (defined COM_PRECONDITION_ASSERT_NO_EVAL)
#error COM_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (COM_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define COM_PRECONDITION_ASSERT(Condition, ApiId) \
  DET_PRECONDITION_ASSERT((Condition), COM_MODULE_ID, COM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define COM_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL((Condition), COM_MODULE_ID, COM_INSTANCE_ID, (ApiId))
#else
#define COM_PRECONDITION_ASSERT(Condition, ApiId)
#define COM_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined COM_POSTCONDITION_ASSERT)
#error COM_POSTCONDITION_ASSERT is already defined
#endif

#if (defined COM_POSTCONDITION_ASSERT_NO_EVAL)
#error COM_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (COM_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define COM_POSTCONDITION_ASSERT(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT((Condition), COM_MODULE_ID, COM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define COM_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL((Condition), COM_MODULE_ID, COM_INSTANCE_ID, (ApiId))
#else
#define COM_POSTCONDITION_ASSERT(Condition, ApiId)
#define COM_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined COM_INVARIANT_ASSERT)
#error COM_INVARIANT_ASSERT is already defined
#endif

#if (defined COM_INVARIANT_ASSERT_NO_EVAL)
#error COM_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (COM_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define COM_INVARIANT_ASSERT(Condition, ApiId) \
  DET_INVARIANT_ASSERT((Condition), COM_MODULE_ID, COM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define COM_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL((Condition), COM_MODULE_ID, COM_INSTANCE_ID, (ApiId))
#else
#define COM_INVARIANT_ASSERT(Condition, ApiId)
#define COM_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined COM_STATIC_ASSERT)
# error COM_STATIC_ASSERT is already defined
#endif
#if (COM_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated */
# define COM_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define COM_STATIC_ASSERT(expr)
#endif

#if (defined COM_UNREACHABLE_CODE_ASSERT)
#error COM_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (COM_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 **

 ** \param[in] ApiId Service ID of the API function */
#define COM_UNREACHABLE_CODE_ASSERT(ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(COM_MODULE_ID, COM_INSTANCE_ID, (ApiId))
#else
#define COM_UNREACHABLE_CODE_ASSERT(ApiId)
#endif

#if (defined COM_INTERNAL_API_ID)
#error COM_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define COM_INTERNAL_API_ID DET_INTERNAL_API_ID



/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#endif /* ifndef COM_DEFPROG_CFG_H */
/*==================[end of file]===========================================*/
