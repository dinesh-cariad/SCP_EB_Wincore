/**
 * \file
 *
 * \brief AUTOSAR CanTp
 *
 * This file contains the implementation of the AUTOSAR
 * module CanTp.
 *
 * \version 6.8.42
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
/* CHECK: NOPARSE */
[!CODE!]
[!AUTOSPACING!]
[!IF "var:defined('postBuildVariant')"!]
[!/* Current PB variant Sufix
  */!][!VAR "VariantSuffix"="concat('_',$postBuildVariant)"!]
#ifndef CANTP_SYMBOLICNAMES[!"$postBuildVariant"!]_PBCFG_H
#define CANTP_SYMBOLICNAMES[!"$postBuildVariant"!]_PBCFG_H
[!ELSE!]
[!/* Current PB variant Sufix
  */!][!VAR "VariantSuffix"="string("")"!]
#ifndef CANTP_SYMBOLICNAMES_PBCFG_H
#define CANTP_SYMBOLICNAMES_PBCFG_H
[!ENDIF!]

[!INCLUDE "../../generate_macros/CanTp_PostBuild.m"!][!//
/*==================[includes]==============================================*/

/*==================[macros]================================================*/

/* Generate macros for CanTpRxNSdus */
[!LOOP "as:modconf('CanTp')[1]/CanTpConfig/*[1]/CanTpChannel/*/CanTpRxNSdu/*"!]
[!IF "node:exists(CanTpRxNSduId)"!]

/*------------------------ [!"@name"!]---------------------------------- */

[!CALL "GetSymbolName", "ShortNameRef"="'.'", "OldName"="name(../.)"!]

#if (defined CanTpConf_[!"$SymbolName"!][!"$VariantSuffix"!]) /* to prevent double declaration */
#error CanTpConf_[!"$SymbolName"!][!"$VariantSuffix"!] is already defined
#endif /* if (defined CanTpConf_[!"$SymbolName"!][!"$VariantSuffix"!]) */

/** \brief Export symbolic name value for [!"../../@name"!]_[!"@name"!] */
#define CanTpConf_[!"$SymbolName"!][!"$VariantSuffix"!]   [!"./CanTpRxNSduId"!]U

#if (defined CANTP_PROVIDE_LEGACY_SYMBOLIC_NAMES)

#if (defined CanTp_[!"../../@name"!]_[!"@name"!]) /* to prevent double declaration */
#error CanTp_[!"../../@name"!]_[!"@name"!] is already defined
#endif /* if (defined CanTp_[!"../../@name"!]_[!"@name"!]) */

/** \brief Export symbolic name value with module abbreviation as prefix only
 *         (AUTOSAR 4.0 rev2 <= AUTOSAR version )
 */
#define CanTp_[!"../../@name"!]_[!"@name"!]       [!"./CanTpRxNSduId"!]U
#endif /* defined CANTP_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDIF!]

[!IF "node:exists(CanTpRxNPdu/CanTpRxNPduId)"!]
[!CALL "GetSymbolName", "ShortNameRef"="'CanTpRxNPdu'", "OldName"="name(./CanTpRxNPdu)"!]

#if (defined CanTpConf_[!"$SymbolName"!][!"$VariantSuffix"!]) /* to prevent double declaration */
#error CanTpConf_[!"$SymbolName"!][!"$VariantSuffix"!] is already defined
#endif /* if (defined CanTpConf_[!"$SymbolName"!][!"$VariantSuffix"!]) */

/** \brief Export symbolic name value for [!"../../@name"!]_[!"@name"!]_CanTpRxNPdu[!"$VariantSuffix"!] */
#define CanTpConf_[!"$SymbolName"!][!"$VariantSuffix"!]  [!"./CanTpRxNPdu/CanTpRxNPduId"!]U

#if (defined CANTP_PROVIDE_LEGACY_SYMBOLIC_NAMES)

#if (defined CanTp_[!"../../@name"!]_[!"@name"!]_CanTpRxNPdu) /* to prevent double declaration */
#error CanTp_[!"../../@name"!]_[!"@name"!]_CanTpRxNPdu is already defined
#endif /* if (defined CanTp_[!"../../@name"!]_[!"@name"!]_CanTpRxNPdu) */

/** \brief Export symbolic name value with module abbreviation as prefix only
           (AUTOSAR 4.0 rev2 <= AUTOSAR version )                               */
#define CanTp_[!"../../@name"!]_[!"@name"!]_CanTpRxNPdu       [!"./CanTpRxNPdu/CanTpRxNPduId"!]U
#endif /* defined CANTP_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDIF!]

[!IF "node:exists(CanTpTxFcNPdu/CanTpTxFcNPduConfirmationPduId)"!]
[!CALL "GetSymbolName", "ShortNameRef"="'./CanTpTxFcNPdu'", "OldName"="name(./CanTpTxFcNPdu)"!]

#if (defined CanTpConf_[!"$SymbolName"!][!"$VariantSuffix"!]) /* to prevent double declaration */
#error CanTpConf_[!"$SymbolName"!][!"$VariantSuffix"!] is already defined
#endif /* if (defined CanTpConf_[!"$SymbolName"!][!"$VariantSuffix"!]) */

/** \brief Export symbolic name value for [!"../../@name"!]_[!"@name"!]_CanTpTxFcNPdu[!"$VariantSuffix"!] */
#define CanTpConf_[!"$SymbolName"!][!"$VariantSuffix"!]   [!"./CanTpTxFcNPdu/CanTpTxFcNPduConfirmationPduId"!]U

#if (defined CANTP_PROVIDE_LEGACY_SYMBOLIC_NAMES)
/** \brief Export symbolic name value with module abbreviation as prefix only
           (AUTOSAR 4.0 rev2 <= AUTOSAR version )                               */

#if (defined CanTp_[!"../../@name"!]_[!"@name"!]_CanTpTxFcNPdu) /* to prevent double declaration */
#error CanTp_[!"../../@name"!]_[!"@name"!]_CanTpTxFcNPdu is already defined
#endif /* if (defined CanTp_[!"../../@name"!]_[!"@name"!]_CanTpTxFcNPdu) */

#define CanTp_[!"../../@name"!]_[!"@name"!]_CanTpTxFcNPdu       [!"./CanTpTxFcNPdu/CanTpTxFcNPduConfirmationPduId"!]U
#endif /* defined CANTP_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDIF!]
[!ENDLOOP!]

/* generate macros for CanTpTxNSdus */

[!LOOP "as:modconf('CanTp')[1]/CanTpConfig/*[1]/CanTpChannel/*/CanTpTxNSdu/*"!][!/*
*/!][!IF "node:exists(CanTpTxNSduId)"!]

/*------------------------ [!"@name"!]---------------------------------- */

[!CALL "GetSymbolName", "ShortNameRef"="'.'", "OldName"="name(../.)"!]

#if (defined CanTpConf_[!"$SymbolName"!][!"$VariantSuffix"!]) /* to prevent double declaration */
#error CanTpConf_[!"$SymbolName"!][!"$VariantSuffix"!] is already defined
#endif /* if (defined CanTpConf_[!"$SymbolName"!][!"$VariantSuffix"!]) */

/** \brief Export symbolic name value for [!"../../@name"!]_[!"@name"!][!"$VariantSuffix"!] */
#define CanTpConf_[!"$SymbolName"!][!"$VariantSuffix"!]           [!"CanTpTxNSduId"!]U

#if (defined CANTP_PROVIDE_LEGACY_SYMBOLIC_NAMES)

#if (defined CanTp_[!"../../@name"!]_[!"@name"!]) /* to prevent double declaration */
#error CanTp_[!"../../@name"!]_[!"@name"!] is already defined
#endif /* if (defined CanTp_[!"../../@name"!]_[!"@name"!]) */

/** \brief Export symbolic name value with module abbreviation as prefix only
 *         (AUTOSAR 4.0 rev2 <= AUTOSAR version )
 */
#define CanTp_[!"../../@name"!]_[!"@name"!]               [!"CanTpTxNSduId"!]U
#endif /* defined CANTP_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDIF!]

[!IF "node:exists(CanTpRxFcNPdu/CanTpRxFcNPduId)"!]
[!CALL "GetSymbolName", "ShortNameRef"="'./CanTpRxFcNPdu'", "OldName"="name(./CanTpRxFcNPdu)"!]

#if (defined CanTpConf_[!"$SymbolName"!][!"$VariantSuffix"!]) /* to prevent double declaration */
#error CanTpConf_[!"$SymbolName"!][!"$VariantSuffix"!] is already defined
#endif /* if (defined CanTpConf_[!"$SymbolName"!][!"$VariantSuffix"!]) */

/** \brief Export symbolic name value for [!"../../@name"!]_[!"@name"!]_CanTpRxFcNPdu[!"$VariantSuffix"!] */
#define CanTpConf_[!"$SymbolName"!][!"$VariantSuffix"!]  [!"./CanTpRxFcNPdu/CanTpRxFcNPduId"!]U

#if (defined CANTP_PROVIDE_LEGACY_SYMBOLIC_NAMES)

#if (defined CanTp_[!"../../@name"!]_[!"@name"!]_CanTpRxFcNPdu) /* to prevent double declaration */
#error CanTp_[!"../../@name"!]_[!"@name"!]_CanTpRxFcNPdu is already defined
#endif /* if (defined CanTp_[!"../../@name"!]_[!"@name"!]_CanTpRxFcNPdu) */

/** \brief Export symbolic name value with module abbreviation as prefix only
           (AUTOSAR 4.0 rev2 <= AUTOSAR version )                               */
#define CanTp_[!"../../@name"!]_[!"@name"!]_CanTpRxFcNPdu      [!"./CanTpRxFcNPdu/CanTpRxFcNPduId"!]U
#endif /* defined CANTP_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDIF!]

[!IF "node:exists(CanTpTxNPdu/CanTpTxNPduConfirmationPduId)"!]
[!CALL "GetSymbolName", "ShortNameRef"="'./CanTpTxNPdu'", "OldName"="name(./CanTpTxNPdu)"!]

#if (defined CanTpConf_[!"$SymbolName"!][!"$VariantSuffix"!]) /* to prevent double declaration */
#error CanTpConf_[!"$SymbolName"!][!"$VariantSuffix"!] is already defined
#endif /* if (defined CanTpConf_[!"$SymbolName"!][!"$VariantSuffix"!]) */

/** \brief Export symbolic name value for [!"../../@name"!]_[!"@name"!]_CanTpTxNPdu[!"$VariantSuffix"!] */
#define CanTpConf_[!"$SymbolName"!][!"$VariantSuffix"!]   [!"./CanTpTxNPdu/CanTpTxNPduConfirmationPduId"!]U

#if (defined CANTP_PROVIDE_LEGACY_SYMBOLIC_NAMES)

#if (defined CanTp_[!"../../@name"!]_[!"@name"!]_CanTpTxNPdu) /* to prevent double declaration */
#error CanTp_[!"../../@name"!]_[!"@name"!]_CanTpTxNPdu is already defined
#endif /* if (defined CanTp_[!"../../@name"!]_[!"@name"!]_CanTpTxNPdu) */

/** \brief Export symbolic name value with module abbreviation as prefix only
           (AUTOSAR 4.0 rev2 <= AUTOSAR version )                               */
#define CanTp_[!"../../@name"!]_[!"@name"!]_CanTpTxNPdu       [!"./CanTpTxNPdu/CanTpTxNPduConfirmationPduId"!]U
#endif /* defined CANTP_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDIF!]
[!ENDLOOP!]

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

[!IF "var:defined('postBuildVariant')"!]
#endif /* ifndef CANTP_SYMBOLICNAMES[!"$postBuildVariant"!]_PBCFG_H */
[!ELSE!]
#endif /* ifndef CANTP_SYMBOLICNAMES_PBCFG_H */
[!ENDIF!]
/*==================[end of file]===========================================*/
[!ENDCODE!][!//
/* CHECK: PARSE */
