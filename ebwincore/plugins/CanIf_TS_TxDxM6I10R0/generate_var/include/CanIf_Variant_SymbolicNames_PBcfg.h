/**
 * \file
 *
 * \brief AUTOSAR CanIf
 *
 * This file contains the implementation of the AUTOSAR
 * module CanIf.
 *
 * \version 6.10.15
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
[!CODE!]
[!AUTOSPACING!]

[!IF "var:defined('postBuildVariant')"!]
  [!/* Current PB variant Sufix
  */!][!VAR "CANIF_VARIANT_SUFIX"="concat('_',$postBuildVariant)"!]

#ifndef CANIF_SYMBOLICNAMES[!"$postBuildVariant"!]_PBCFG_H
#define CANIF_SYMBOLICNAMES[!"$postBuildVariant"!]_PBCFG_H
[!ELSE!]
  [!/* Current PB variant Sufix
  */!][!VAR "CANIF_VARIANT_SUFIX"="string("")"!]
#ifndef CANIF_SYMBOLICNAMES_PBCFG_H
#define CANIF_SYMBOLICNAMES_PBCFG_H
[!ENDIF!]

/*==================[includes]===============================================*/
/*==================[macros]=================================================*/

/*------------------[symbolic name values]----------------------------------*/

/** \brief Export symbolic name values for CanIfTxPduIds */

[!LOOP "as:modconf('CanIf')[1]/CanIfInitCfg/*[1]/CanIfTxPduCfg/*"!]

#if (defined CanIfConf_CanIfTxPduCfg_[!"@name"!][!"$CANIF_VARIANT_SUFIX"!])
#error CanIfConf_CanIfTxPduCfg_[!"@name"!][!"$CANIF_VARIANT_SUFIX"!] already defined!
#endif /* #if( defined CanIfConf_CanIfTxPduCfg_[!"@name"!][!"$CANIF_VARIANT_SUFIX"!])*/
#define CanIfConf_CanIfTxPduCfg_[!"@name"!][!"$CANIF_VARIANT_SUFIX"!] [!"CanIfTxPduId"!]U

#if (defined CANIF_PROVIDE_LEGACY_SYMBOLIC_NAMES)
/** \brief Export symbolic name value with module abbreviation as prefix only (AUTOSAR 4.0 rev2 <=
AUTOSAR version) */
#if (defined CanIf_[!"@name"!])
#error CanIf_[!"@name"!] already defined!
#endif /* #if( defined CanIf_[!"@name"!])*/
#define CanIf_[!"@name"!] [!"CanIfTxPduId"!]U
#endif /* defined CANIF_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDLOOP!][!//


/** \brief Export symbolic name values for CanIfRxPduIds */

[!LOOP "as:modconf('CanIf')[1]/CanIfInitCfg/*[1]/CanIfRxPduCfg/*"!]

#if (defined CanIfConf_CanIfRxPduCfg_[!"@name"!][!"$CANIF_VARIANT_SUFIX"!])
#error CanIfConf_CanIfRxPduCfg_[!"@name"!][!"$CANIF_VARIANT_SUFIX"!] already defined!
#endif /* #if( defined CanIfConf_CanIfRxPduCfg_[!"@name"!][!"$CANIF_VARIANT_SUFIX"!])*/
#define CanIfConf_CanIfRxPduCfg_[!"@name"!][!"$CANIF_VARIANT_SUFIX"!] [!"CanIfRxPduId"!]U

#if (defined CANIF_PROVIDE_LEGACY_SYMBOLIC_NAMES)
/** \brief Export symbolic name value with module abbreviation as prefix only (AUTOSAR 4.0 rev2 <=
AUTOSAR version) */
#if (defined CanIf_[!"@name"!])
#error CanIf_[!"@name"!] already defined!
#endif /* #if( defined CanIf_[!"@name"!])*/
#define CanIf_[!"@name"!] [!"CanIfRxPduId"!]U
#endif /* defined CANIF_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDLOOP!][!//


/** \brief Export symbolic name values for CanIfCtrlIds */

[!LOOP "as:modconf('CanIf')[1]/CanIfCtrlDrvCfg/*/CanIfCtrlCfg/*"!]

#if (defined CanIfConf_[!"../../@name"!]_[!"@name"!][!"$CANIF_VARIANT_SUFIX"!])
#error CanIfConf_[!"../../@name"!]_[!"@name"!][!"$CANIF_VARIANT_SUFIX"!] already defined!
#endif /* #if( defined CanIfConf_[!"../../@name"!][!"$CANIF_VARIANT_SUFIX"!]_[!"@name"!])*/
#define CanIfConf_[!"../../@name"!]_[!"@name"!][!"$CANIF_VARIANT_SUFIX"!] [!"CanIfCtrlId"!]U

#if (defined CANIF_PROVIDE_LEGACY_SYMBOLIC_NAMES)
/** \brief Export symbolic name value with module abbreviation as prefix only (AUTOSAR 4.0 rev2 <=
AUTOSAR version) */
#if (defined CanIf_[!"../../@name"!]_[!"@name"!])
#error CanIf_[!"../../@name"!]_[!"@name"!] already defined!
#endif /* #if( defined CanIf_[!"../../@name"!]_[!"@name"!])*/
#define CanIf_[!"../../@name"!]_[!"@name"!] [!"CanIfCtrlId"!]U
#endif /* defined CANIF_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDLOOP!][!//


/** \brief Export symbolic name values for CanIfTrcvIds */

[!LOOP "as:modconf('CanIf')[1]/CanIfTrcvDrvCfg/*/CanIfTrcvCfg/*"!]

#if (defined CanIfConf_[!"../../@name"!]_[!"@name"!][!"$CANIF_VARIANT_SUFIX"!])
#error CanIfConf_[!"../../@name"!]_[!"@name"!][!"$CANIF_VARIANT_SUFIX"!] already defined!
#endif /* #if( defined CanIfConf_[!"../../@name"!]_[!"@name"!][!"$CANIF_VARIANT_SUFIX"!])*/
#define CanIfConf_[!"../../@name"!]_[!"@name"!][!"$CANIF_VARIANT_SUFIX"!] [!"CanIfTrcvId"!]U

#if (defined CANIF_PROVIDE_LEGACY_SYMBOLIC_NAMES)
/** \brief Export symbolic name value with module abbreviation as prefix only (AUTOSAR 4.0 rev2 <=
AUTOSAR version) */
#if (defined CanIf_[!"../../@name"!]_[!"@name"!])
#error CanIf_[!"../../@name"!]_[!"@name"!] already defined!
#endif /* #if( defined CanIf_[!"../../@name"!]_[!"@name"!])*/
#define CanIf_[!"../../@name"!]_[!"@name"!] [!"CanIfTrcvId"!]U
#endif /* defined CANIF_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDLOOP!][!//
/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/
[!IF "var:defined('postBuildVariant')"!]
#endif /* ifndef CANIF_SYMBOLICNAMES[!"$postBuildVariant"!]_PBCFG_H */
[!ELSE!]
#endif /* ifndef CANIF_SYMBOLICNAMES_PBCFG_H */
[!ENDIF!]
/*==================[end of file]============================================*/
[!ENDCODE!]
