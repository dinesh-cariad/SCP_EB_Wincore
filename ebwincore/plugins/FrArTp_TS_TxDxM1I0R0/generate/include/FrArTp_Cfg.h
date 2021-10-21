/**
 * \file
 *
 * \brief AUTOSAR FrArTp
 *
 * This file contains the implementation of the AUTOSAR
 * module FrArTp.
 *
 * \version 1.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

[!SELECT "as:modconf('FrArTp')[1]"!][!//

#ifndef FRARTP_CFG_H
#define FRARTP_CFG_H

/* \brief Support for development error detection */
#define FRARTP_DEV_ERROR_DETECT [!IF "FrArTpGeneral/FrArTpDevErrorDetect = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* \brief Support for version information API */
#define FRARTP_VERSION_INFO_API [!IF "FrArTpGeneral/FrArTpVersionInfoApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* \brief Support for transmit/receive cancellation API */
#define FRARTP_CANCEL_API [!IF "FrArTpGeneral/FrArTpHaveTc = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* \brief Maximum number of active connections */
#define FRARTP_ACTCONN_MAX [!"num:i(FrArTpGeneral/FrArTpMaxActiveConnections)"!]U

/* \brief Maximum number of connections */
#define FRARTP_CONN_MAX [!"num:i(FrArTpGeneral/FrArTpMaxConnections)"!]U

/* \brief Maximum number of Tx PDUs  */
#define FRARTP_TXPDUS_MAX  [!"num:i(FrArTpGeneral/FrArTpMaxTxPdus)"!]U

[!IF "FrArTpGeneral/SupportLowLevelRouting = 'true'"!][!//

#include <[!"FrArTpGeneral/LowLevelRoutingPraefix"!]LLR.h>

/** \brief Support for low-level routing */
#define FRARTP_LOWLEVELROUTING_ENABLE STD_ON

/** \brief Prefix of low-level routing function names */
#define FRARTP_LOWLEVELROUTING_PRAEFIX [!"FrArTpGeneral/LowLevelRoutingPraefix"!]

/** \brief Macro to generate low-level routing function names */
#define FRARTP_LLR_FCTNAME(praefix,name) FRARTP_LLR_FCTNAME_HLP(praefix,name)

/** \brief Helper macro to generate low-level routing function names */
#define FRARTP_LLR_FCTNAME_HLP(praefix,name) praefix##name

[!ELSE!][!//
/** \brief Support for low-level routing */
#define FRARTP_LOWLEVELROUTING_ENABLE STD_OFF
[!ENDIF!][!//

/** \brief Precompile configuration signature */
#define FRARTP_CFG_SIGNATURE [!"asc:getConfigSignature(as:modconf('FrArTp')[1]//*[not(child::*) and (node:configclass() = 'PreCompile') ])"!]U

/** \brief Public information signature */
#define FRARTP_PUBLIC_INFO_SIGNATURE [!"asc:getConfigSignature(node:difference(as:modconf('FrArTp')[1]/CommonPublishedInformation//*[not(child::*) and (node:configclass() = 'PublishedInformation') ], as:modconf('FrArTp')[1]/CommonPublishedInformation/Release))"!]U

/** \brief Support for Postbuild Configuration Manager */
#define FRARTP_PBCFGM_SUPPORT_ENABLED [!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('FrArTp')) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Support for relocateable postbuild configuration */
[!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('FrArTp')) = 'true'"!]
#define FRARTP_RELOCATABLE_CFG_ENABLE [!IF "as:modconf('PbcfgM')/PbcfgMGeneral/PbcfgMRelocatableCfgEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!ELSE!]
#define FRARTP_RELOCATABLE_CFG_ENABLE [!IF "FrArTpGeneral/FrArTpRelocatablePbcfgEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!ENDIF!]

/* ----- defensive programming macros ----- */



/*------------------[Defensive programming]---------------------------------*/
[!SELECT "FrArTpDefensiveProgramming"!][!//

#if (defined FRARTP_DEFENSIVE_PROGRAMMING_ENABLED)
#error FRARTP_DEFENSIVE_PROGRAMMING_ENABLED is already defined
#endif
/** \brief Defensive programming usage
 **
 ** En- or disables the usage of the defensive programming */
#define FRARTP_DEFENSIVE_PROGRAMMING_ENABLED   [!//
[!IF "(../FrArTpGeneral/FrArTpDevErrorDetect  = 'true') and (FrArTpDefProgEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRARTP_PRECONDITION_ASSERT_ENABLED)
#error FRARTP_PRECONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Precondition assertion usage
 **
 ** En- or disables the usage of precondition assertion checks */
#define FRARTP_PRECONDITION_ASSERT_ENABLED     [!//
[!IF "(../FrArTpGeneral/FrArTpDevErrorDetect  = 'true') and (FrArTpDefProgEnabled = 'true') and (FrArTpPrecondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRARTP_POSTCONDITION_ASSERT_ENABLED)
#error FRARTP_POSTCONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Postcondition assertion usage
 **
 ** En- or disables the usage of postcondition assertion checks */
#define FRARTP_POSTCONDITION_ASSERT_ENABLED    [!//
[!IF "(../FrArTpGeneral/FrArTpDevErrorDetect  = 'true') and (FrArTpDefProgEnabled = 'true') and (FrArTpPostcondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRARTP_UNREACHABLE_CODE_ASSERT_ENABLED)
#error FRARTP_UNREACHABLE_CODE_ASSERT_ENABLED is already defined
#endif
/** \brief Unreachable code assertion usage
 **
 ** En- or disables the usage of unreachable code assertion checks */
#define FRARTP_UNREACHABLE_CODE_ASSERT_ENABLED [!//
[!IF "(../FrArTpGeneral/FrArTpDevErrorDetect  = 'true') and (FrArTpDefProgEnabled = 'true') and (FrArTpUnreachAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRARTP_INVARIANT_ASSERT_ENABLED)
#error FRARTP_INVARIANT_ASSERT_ENABLED is already defined
#endif
/** \brief Invariant assertion usage
 **
 ** En- or disables the usage of invariant assertion checks */
#define FRARTP_INVARIANT_ASSERT_ENABLED        [!//
[!IF "(../FrArTpGeneral/FrArTpDevErrorDetect  = 'true') and (FrArTpDefProgEnabled = 'true') and (FrArTpInvariantAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRARTP_STATIC_ASSERT_ENABLED)
#error FRARTP_STATIC_ASSERT_ENABLED is already defined
#endif
/** \brief Static assertion usage
 **
 ** En- or disables the usage of static assertion checks */
#define FRARTP_STATIC_ASSERT_ENABLED           [!//
[!IF "(../FrArTpGeneral/FrArTpDevErrorDetect  = 'true') and (FrArTpDefProgEnabled = 'true') and (FrArTpStaticAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!][!//


/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
#endif   /* FRARTP_CFG_H */
[!ENDSELECT!]
