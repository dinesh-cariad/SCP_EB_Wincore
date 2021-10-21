/**
 * \file
 *
 * \brief AUTOSAR EthIf
 *
 * This file contains the implementation of the AUTOSAR
 * module EthIf.
 *
 * \version 1.9.18
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef ETHIF_DEFPROG_CFG_H
#define ETHIF_DEFPROG_CFG_H

/*==================[includes]==============================================*/

/*==================[macros]================================================*/




/*------------------[Defensive programming]---------------------------------*/
[!SELECT "EthIfDefensiveProgramming"!][!//

#if (defined ETHIF_DEFENSIVE_PROGRAMMING_ENABLED)
#error ETHIF_DEFENSIVE_PROGRAMMING_ENABLED is already defined
#endif
/** \brief Defensive programming usage
 **
 ** En- or disables the usage of the defensive programming */
#define ETHIF_DEFENSIVE_PROGRAMMING_ENABLED   [!//
[!IF "(../EthIfGeneral/EthIfDevErrorDetect  = 'true') and (EthIfDefProgEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHIF_PRECONDITION_ASSERT_ENABLED)
#error ETHIF_PRECONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Precondition assertion usage
 **
 ** En- or disables the usage of precondition assertion checks */
#define ETHIF_PRECONDITION_ASSERT_ENABLED     [!//
[!IF "(../EthIfGeneral/EthIfDevErrorDetect  = 'true') and (EthIfDefProgEnabled = 'true') and (EthIfPrecondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHIF_POSTCONDITION_ASSERT_ENABLED)
#error ETHIF_POSTCONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Postcondition assertion usage
 **
 ** En- or disables the usage of postcondition assertion checks */
#define ETHIF_POSTCONDITION_ASSERT_ENABLED    [!//
[!IF "(../EthIfGeneral/EthIfDevErrorDetect  = 'true') and (EthIfDefProgEnabled = 'true') and (EthIfPostcondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHIF_UNREACHABLE_CODE_ASSERT_ENABLED)
#error ETHIF_UNREACHABLE_CODE_ASSERT_ENABLED is already defined
#endif
/** \brief Unreachable code assertion usage
 **
 ** En- or disables the usage of unreachable code assertion checks */
#define ETHIF_UNREACHABLE_CODE_ASSERT_ENABLED [!//
[!IF "(../EthIfGeneral/EthIfDevErrorDetect  = 'true') and (EthIfDefProgEnabled = 'true') and (EthIfUnreachAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHIF_INVARIANT_ASSERT_ENABLED)
#error ETHIF_INVARIANT_ASSERT_ENABLED is already defined
#endif
/** \brief Invariant assertion usage
 **
 ** En- or disables the usage of invariant assertion checks */
#define ETHIF_INVARIANT_ASSERT_ENABLED        [!//
[!IF "(../EthIfGeneral/EthIfDevErrorDetect  = 'true') and (EthIfDefProgEnabled = 'true') and (EthIfInvariantAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHIF_STATIC_ASSERT_ENABLED)
#error ETHIF_STATIC_ASSERT_ENABLED is already defined
#endif
/** \brief Static assertion usage
 **
 ** En- or disables the usage of static assertion checks */
#define ETHIF_STATIC_ASSERT_ENABLED           [!//
[!IF "(../EthIfGeneral/EthIfDevErrorDetect  = 'true') and (EthIfDefProgEnabled = 'true') and (EthIfStaticAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!][!//



/*------------------------[Defensive programming]----------------------------*/

#if (defined ETHIF_PRECONDITION_ASSERT)
#error ETHIF_PRECONDITION_ASSERT is already defined
#endif

#if (defined ETHIF_PRECONDITION_ASSERT_NO_EVAL)
#error ETHIF_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (ETHIF_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define ETHIF_PRECONDITION_ASSERT(Condition, ApiId) \
  DET_PRECONDITION_ASSERT((Condition), ETHIF_MODULE_ID, ETHIF_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define ETHIF_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL((Condition), ETHIF_MODULE_ID, ETHIF_INSTANCE_ID, (ApiId))
#else
#define ETHIF_PRECONDITION_ASSERT(Condition, ApiId)
#define ETHIF_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined ETHIF_POSTCONDITION_ASSERT)
#error ETHIF_POSTCONDITION_ASSERT is already defined
#endif

#if (defined ETHIF_POSTCONDITION_ASSERT_NO_EVAL)
#error ETHIF_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (ETHIF_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define ETHIF_POSTCONDITION_ASSERT(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT((Condition), ETHIF_MODULE_ID, ETHIF_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define ETHIF_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL((Condition), ETHIF_MODULE_ID, ETHIF_INSTANCE_ID, (ApiId))
#else
#define ETHIF_POSTCONDITION_ASSERT(Condition, ApiId)
#define ETHIF_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined ETHIF_INVARIANT_ASSERT)
#error ETHIF_INVARIANT_ASSERT is already defined
#endif

#if (defined ETHIF_INVARIANT_ASSERT_NO_EVAL)
#error ETHIF_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (ETHIF_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define ETHIF_INVARIANT_ASSERT(Condition, ApiId) \
  DET_INVARIANT_ASSERT((Condition), ETHIF_MODULE_ID, ETHIF_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define ETHIF_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL((Condition), ETHIF_MODULE_ID, ETHIF_INSTANCE_ID, (ApiId))
#else
#define ETHIF_INVARIANT_ASSERT(Condition, ApiId)
#define ETHIF_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined ETHIF_STATIC_ASSERT)
# error ETHIF_STATIC_ASSERT is already defined
#endif
#if (ETHIF_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated */
# define ETHIF_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define ETHIF_STATIC_ASSERT(expr)
#endif

#if (defined ETHIF_UNREACHABLE_CODE_ASSERT)
#error ETHIF_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (ETHIF_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 **

 ** \param[in] ApiId Service ID of the API function */
#define ETHIF_UNREACHABLE_CODE_ASSERT(ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID, (ApiId))
#else
#define ETHIF_UNREACHABLE_CODE_ASSERT(ApiId)
#endif

#if (defined ETHIF_INTERNAL_API_ID)
#error ETHIF_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define ETHIF_INTERNAL_API_ID DET_INTERNAL_API_ID



/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#endif /* ifndef ETHIF_DEFPROG_CFG_H */
/*==================[end of file]===========================================*/
