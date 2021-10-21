#ifndef XCPR_DEFPROG_CFG_H
#define XCPR_DEFPROG_CFG_H

/**
 * \file
 *
 * \brief AUTOSAR XcpR
 *
 * This file contains the implementation of the AUTOSAR
 * module XcpR.
 *
 * \version 1.1.0
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==[Includes]================================================================*/

/*==[Macros definitions]======================================================*/




/*------------------[Defensive programming]---------------------------------*/
[!SELECT "XcpRDefensiveProgramming"!][!//

#if (defined XCPR_DEFENSIVE_PROGRAMMING_ENABLED)
#error XCPR_DEFENSIVE_PROGRAMMING_ENABLED is already defined
#endif
/** \brief Defensive programming usage
 **
 ** En- or disables the usage of the defensive programming */
#define XCPR_DEFENSIVE_PROGRAMMING_ENABLED   [!//
[!IF "(../XcpRGeneral/XcpRDevErrorDetect  = 'true') and (XcpRDefProgEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCPR_PRECONDITION_ASSERT_ENABLED)
#error XCPR_PRECONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Precondition assertion usage
 **
 ** En- or disables the usage of precondition assertion checks */
#define XCPR_PRECONDITION_ASSERT_ENABLED     [!//
[!IF "(../XcpRGeneral/XcpRDevErrorDetect  = 'true') and (XcpRDefProgEnabled = 'true') and (XcpRPrecondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCPR_POSTCONDITION_ASSERT_ENABLED)
#error XCPR_POSTCONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Postcondition assertion usage
 **
 ** En- or disables the usage of postcondition assertion checks */
#define XCPR_POSTCONDITION_ASSERT_ENABLED    [!//
[!IF "(../XcpRGeneral/XcpRDevErrorDetect  = 'true') and (XcpRDefProgEnabled = 'true') and (XcpRPostcondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCPR_UNREACHABLE_CODE_ASSERT_ENABLED)
#error XCPR_UNREACHABLE_CODE_ASSERT_ENABLED is already defined
#endif
/** \brief Unreachable code assertion usage
 **
 ** En- or disables the usage of unreachable code assertion checks */
#define XCPR_UNREACHABLE_CODE_ASSERT_ENABLED [!//
[!IF "(../XcpRGeneral/XcpRDevErrorDetect  = 'true') and (XcpRDefProgEnabled = 'true') and (XcpRUnreachAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCPR_INVARIANT_ASSERT_ENABLED)
#error XCPR_INVARIANT_ASSERT_ENABLED is already defined
#endif
/** \brief Invariant assertion usage
 **
 ** En- or disables the usage of invariant assertion checks */
#define XCPR_INVARIANT_ASSERT_ENABLED        [!//
[!IF "(../XcpRGeneral/XcpRDevErrorDetect  = 'true') and (XcpRDefProgEnabled = 'true') and (XcpRInvariantAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCPR_STATIC_ASSERT_ENABLED)
#error XCPR_STATIC_ASSERT_ENABLED is already defined
#endif
/** \brief Static assertion usage
 **
 ** En- or disables the usage of static assertion checks */
#define XCPR_STATIC_ASSERT_ENABLED           [!//
[!IF "(../XcpRGeneral/XcpRDevErrorDetect  = 'true') and (XcpRDefProgEnabled = 'true') and (XcpRStaticAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!][!//


/*==[Types declarations]======================================================*/

/*==[Declaration of constants with external linkage]==========================*/

/*==[Declaration of variables with external linkage]==========================*/

/*==[Declaration of functions with external linkage]==========================*/

#endif /* XCPR_DEFPROG_CFG_H */

