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

#ifndef TCPIP_DEFPROGCFG_H
#define TCPIP_DEFPROGCFG_H




/*------------------[Defensive programming]---------------------------------*/
[!SELECT "TlsDefensiveProgramming"!][!//

#if (defined TLS_DEFENSIVE_PROGRAMMING_ENABLED)
#error TLS_DEFENSIVE_PROGRAMMING_ENABLED is already defined
#endif
/** \brief Defensive programming usage
 **
 ** En- or disables the usage of the defensive programming */
#define TLS_DEFENSIVE_PROGRAMMING_ENABLED   [!//
[!IF "(../TlsGeneral/TlsDevErrorDetect  = 'true') and (TlsDefProgEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined TLS_PRECONDITION_ASSERT_ENABLED)
#error TLS_PRECONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Precondition assertion usage
 **
 ** En- or disables the usage of precondition assertion checks */
#define TLS_PRECONDITION_ASSERT_ENABLED     [!//
[!IF "(../TlsGeneral/TlsDevErrorDetect  = 'true') and (TlsDefProgEnabled = 'true') and (TlsPrecondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined TLS_POSTCONDITION_ASSERT_ENABLED)
#error TLS_POSTCONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Postcondition assertion usage
 **
 ** En- or disables the usage of postcondition assertion checks */
#define TLS_POSTCONDITION_ASSERT_ENABLED    [!//
[!IF "(../TlsGeneral/TlsDevErrorDetect  = 'true') and (TlsDefProgEnabled = 'true') and (TlsPostcondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined TLS_UNREACHABLE_CODE_ASSERT_ENABLED)
#error TLS_UNREACHABLE_CODE_ASSERT_ENABLED is already defined
#endif
/** \brief Unreachable code assertion usage
 **
 ** En- or disables the usage of unreachable code assertion checks */
#define TLS_UNREACHABLE_CODE_ASSERT_ENABLED [!//
[!IF "(../TlsGeneral/TlsDevErrorDetect  = 'true') and (TlsDefProgEnabled = 'true') and (TlsUnreachAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined TLS_INVARIANT_ASSERT_ENABLED)
#error TLS_INVARIANT_ASSERT_ENABLED is already defined
#endif
/** \brief Invariant assertion usage
 **
 ** En- or disables the usage of invariant assertion checks */
#define TLS_INVARIANT_ASSERT_ENABLED        [!//
[!IF "(../TlsGeneral/TlsDevErrorDetect  = 'true') and (TlsDefProgEnabled = 'true') and (TlsInvariantAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined TLS_STATIC_ASSERT_ENABLED)
#error TLS_STATIC_ASSERT_ENABLED is already defined
#endif
/** \brief Static assertion usage
 **
 ** En- or disables the usage of static assertion checks */
#define TLS_STATIC_ASSERT_ENABLED           [!//
[!IF "(../TlsGeneral/TlsDevErrorDetect  = 'true') and (TlsDefProgEnabled = 'true') and (TlsStaticAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!][!//


#endif /* #ifndef TCPIP_DEFPROGCFG_H */
