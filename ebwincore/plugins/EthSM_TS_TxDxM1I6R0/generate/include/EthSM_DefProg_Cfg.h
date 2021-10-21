/**
 * \file
 *
 * \brief AUTOSAR EthSM
 *
 * This file contains the implementation of the AUTOSAR
 * module EthSM.
 *
 * \version 1.6.14
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef ETHSM_DEFPROG_CFG_H
#define ETHSM_DEFPROG_CFG_H

/*==================[includes]==============================================*/

/*==================[macros]================================================*/




/*------------------[Defensive programming]---------------------------------*/
[!SELECT "EthSMDefensiveProgramming"!][!//

#if (defined ETHSM_DEFENSIVE_PROGRAMMING_ENABLED)
#error ETHSM_DEFENSIVE_PROGRAMMING_ENABLED is already defined
#endif
/** \brief Defensive programming usage
 **
 ** En- or disables the usage of the defensive programming */
#define ETHSM_DEFENSIVE_PROGRAMMING_ENABLED   [!//
[!IF "(../EthSMGeneral/EthSMDevErrorDetect  = 'true') and (EthSMDefProgEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHSM_PRECONDITION_ASSERT_ENABLED)
#error ETHSM_PRECONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Precondition assertion usage
 **
 ** En- or disables the usage of precondition assertion checks */
#define ETHSM_PRECONDITION_ASSERT_ENABLED     [!//
[!IF "(../EthSMGeneral/EthSMDevErrorDetect  = 'true') and (EthSMDefProgEnabled = 'true') and (EthSMPrecondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHSM_POSTCONDITION_ASSERT_ENABLED)
#error ETHSM_POSTCONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Postcondition assertion usage
 **
 ** En- or disables the usage of postcondition assertion checks */
#define ETHSM_POSTCONDITION_ASSERT_ENABLED    [!//
[!IF "(../EthSMGeneral/EthSMDevErrorDetect  = 'true') and (EthSMDefProgEnabled = 'true') and (EthSMPostcondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHSM_UNREACHABLE_CODE_ASSERT_ENABLED)
#error ETHSM_UNREACHABLE_CODE_ASSERT_ENABLED is already defined
#endif
/** \brief Unreachable code assertion usage
 **
 ** En- or disables the usage of unreachable code assertion checks */
#define ETHSM_UNREACHABLE_CODE_ASSERT_ENABLED [!//
[!IF "(../EthSMGeneral/EthSMDevErrorDetect  = 'true') and (EthSMDefProgEnabled = 'true') and (EthSMUnreachAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHSM_INVARIANT_ASSERT_ENABLED)
#error ETHSM_INVARIANT_ASSERT_ENABLED is already defined
#endif
/** \brief Invariant assertion usage
 **
 ** En- or disables the usage of invariant assertion checks */
#define ETHSM_INVARIANT_ASSERT_ENABLED        [!//
[!IF "(../EthSMGeneral/EthSMDevErrorDetect  = 'true') and (EthSMDefProgEnabled = 'true') and (EthSMInvariantAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHSM_STATIC_ASSERT_ENABLED)
#error ETHSM_STATIC_ASSERT_ENABLED is already defined
#endif
/** \brief Static assertion usage
 **
 ** En- or disables the usage of static assertion checks */
#define ETHSM_STATIC_ASSERT_ENABLED           [!//
[!IF "(../EthSMGeneral/EthSMDevErrorDetect  = 'true') and (EthSMDefProgEnabled = 'true') and (EthSMStaticAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!][!//



/*------------------------[Defensive programming]----------------------------*/

#if (defined ETHSM_PRECONDITION_ASSERT)
#error ETHSM_PRECONDITION_ASSERT is already defined
#endif

#if (defined ETHSM_PRECONDITION_ASSERT_NO_EVAL)
#error ETHSM_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (ETHSM_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define ETHSM_PRECONDITION_ASSERT(Condition, ApiId) \
  DET_PRECONDITION_ASSERT((Condition), ETHSM_MODULE_ID, ETHSM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define ETHSM_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL((Condition), ETHSM_MODULE_ID, ETHSM_INSTANCE_ID, (ApiId))
#else
#define ETHSM_PRECONDITION_ASSERT(Condition, ApiId)
#define ETHSM_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined ETHSM_POSTCONDITION_ASSERT)
#error ETHSM_POSTCONDITION_ASSERT is already defined
#endif

#if (defined ETHSM_POSTCONDITION_ASSERT_NO_EVAL)
#error ETHSM_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (ETHSM_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define ETHSM_POSTCONDITION_ASSERT(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT((Condition), ETHSM_MODULE_ID, ETHSM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define ETHSM_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL((Condition), ETHSM_MODULE_ID, ETHSM_INSTANCE_ID, (ApiId))
#else
#define ETHSM_POSTCONDITION_ASSERT(Condition, ApiId)
#define ETHSM_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined ETHSM_INVARIANT_ASSERT)
#error ETHSM_INVARIANT_ASSERT is already defined
#endif

#if (defined ETHSM_INVARIANT_ASSERT_NO_EVAL)
#error ETHSM_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (ETHSM_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define ETHSM_INVARIANT_ASSERT(Condition, ApiId) \
  DET_INVARIANT_ASSERT((Condition), ETHSM_MODULE_ID, ETHSM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define ETHSM_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL((Condition), ETHSM_MODULE_ID, ETHSM_INSTANCE_ID, (ApiId))
#else
#define ETHSM_INVARIANT_ASSERT(Condition, ApiId)
#define ETHSM_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined ETHSM_STATIC_ASSERT)
# error ETHSM_STATIC_ASSERT is already defined
#endif
#if (ETHSM_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated */
# define ETHSM_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define ETHSM_STATIC_ASSERT(expr)
#endif

#if (defined ETHSM_UNREACHABLE_CODE_ASSERT)
#error ETHSM_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (ETHSM_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 **

 ** \param[in] ApiId Service ID of the API function */
#define ETHSM_UNREACHABLE_CODE_ASSERT(ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(ETHSM_MODULE_ID, ETHSM_INSTANCE_ID, (ApiId))
#else
#define ETHSM_UNREACHABLE_CODE_ASSERT(ApiId)
#endif

#if (defined ETHSM_INTERNAL_API_ID)
#error ETHSM_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define ETHSM_INTERNAL_API_ID DET_INTERNAL_API_ID



/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#endif /* ifndef ETHSM_DEFPROG_CFG_H */
/*==================[end of file]===========================================*/
