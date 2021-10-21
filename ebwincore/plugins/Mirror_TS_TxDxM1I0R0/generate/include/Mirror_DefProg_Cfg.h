/**
 * \file
 *
 * \brief AUTOSAR Mirror
 *
 * This file contains the implementation of the AUTOSAR
 * module Mirror.
 *
 * \version 1.0.17
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef MIRROR_DEFPROG_CFG_H
#define MIRROR_DEFPROG_CFG_H

/*==================[includes]==============================================*/

/*==================[macros]================================================*/



/*------------------[Defensive programming]---------------------------------*/
[!SELECT "MirrorDefensiveProgramming"!][!//

#if (defined MIRROR_DEFENSIVE_PROGRAMMING_ENABLED)
#error MIRROR_DEFENSIVE_PROGRAMMING_ENABLED is already defined
#endif
/** \brief Defensive programming usage
 **
 ** En- or disables the usage of the defensive programming */
#define MIRROR_DEFENSIVE_PROGRAMMING_ENABLED   [!//
[!IF "(../MirrorGeneral/MirrorDevErrorDetect  = 'true') and (MirrorDefProgEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined MIRROR_PRECONDITION_ASSERT_ENABLED)
#error MIRROR_PRECONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Precondition assertion usage
 **
 ** En- or disables the usage of precondition assertion checks */
#define MIRROR_PRECONDITION_ASSERT_ENABLED     [!//
[!IF "(../MirrorGeneral/MirrorDevErrorDetect  = 'true') and (MirrorDefProgEnabled = 'true') and (MirrorPrecondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined MIRROR_POSTCONDITION_ASSERT_ENABLED)
#error MIRROR_POSTCONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Postcondition assertion usage
 **
 ** En- or disables the usage of postcondition assertion checks */
#define MIRROR_POSTCONDITION_ASSERT_ENABLED    [!//
[!IF "(../MirrorGeneral/MirrorDevErrorDetect  = 'true') and (MirrorDefProgEnabled = 'true') and (MirrorPostcondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined MIRROR_UNREACHABLE_CODE_ASSERT_ENABLED)
#error MIRROR_UNREACHABLE_CODE_ASSERT_ENABLED is already defined
#endif
/** \brief Unreachable code assertion usage
 **
 ** En- or disables the usage of unreachable code assertion checks */
#define MIRROR_UNREACHABLE_CODE_ASSERT_ENABLED [!//
[!IF "(../MirrorGeneral/MirrorDevErrorDetect  = 'true') and (MirrorDefProgEnabled = 'true') and (MirrorUnreachAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined MIRROR_INVARIANT_ASSERT_ENABLED)
#error MIRROR_INVARIANT_ASSERT_ENABLED is already defined
#endif
/** \brief Invariant assertion usage
 **
 ** En- or disables the usage of invariant assertion checks */
#define MIRROR_INVARIANT_ASSERT_ENABLED        [!//
[!IF "(../MirrorGeneral/MirrorDevErrorDetect  = 'true') and (MirrorDefProgEnabled = 'true') and (MirrorInvariantAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined MIRROR_STATIC_ASSERT_ENABLED)
#error MIRROR_STATIC_ASSERT_ENABLED is already defined
#endif
/** \brief Static assertion usage
 **
 ** En- or disables the usage of static assertion checks */
#define MIRROR_STATIC_ASSERT_ENABLED           [!//
[!IF "(../MirrorGeneral/MirrorDevErrorDetect  = 'true') and (MirrorDefProgEnabled = 'true') and (MirrorStaticAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!][!//

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#endif /* ifndef MIRROR_DEFPROG_CFG_H */
/*==================[end of file]===========================================*/
