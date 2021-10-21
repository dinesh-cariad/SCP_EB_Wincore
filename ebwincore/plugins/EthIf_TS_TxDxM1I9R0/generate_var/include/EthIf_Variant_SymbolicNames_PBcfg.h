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
[!CODE!]
[!IF "var:defined('postBuildVariant')"!][!//
[!/* Current PB variant Sufix
*/!][!VAR "ETHIF_VARIANT_SUFIX"="concat('_',$postBuildVariant)"!][!//

#ifndef ETHIF_SYMBOLICNAMES[!"$postBuildVariant"!]_PBCFG_H
#define ETHIF_SYMBOLICNAMES[!"$postBuildVariant"!]_PBCFG_H
[!ELSE!][!//
[!/* Current PB variant Sufix
*/!][!VAR "ETHIF_VARIANT_SUFIX"="string("")"!][!//
#ifndef ETHIF_SYMBOLICNAMES_PBCFG_H
#define ETHIF_SYMBOLICNAMES_PBCFG_H
[!ENDIF!][!//

/*==================[includes]==============================================*/

/*==================[macros]================================================*/

/* EthIfController */[!/*
*/!][!LOOP "as:modconf('EthIf')[1]/EthIfConfigSet/EthIfController/*"!]
#if (defined EthIfConf_[!"./@name"!][!"$ETHIF_VARIANT_SUFIX"!])
#error EthIfConf_[!"./@name"!][!"$ETHIF_VARIANT_SUFIX"!] is already defined
#endif
#define EthIfConf_[!"./@name"!][!"$ETHIF_VARIANT_SUFIX"!] [!"./EthIfCtrlIdx"!]U

#if (defined ETHIF_PROVIDE_LEGACY_SYMBOLIC_NAMES)
#if (defined EthIf_[!"@name"!])
#error EthIf_[!"@name"!] is already defined
#endif
/** \brief Export symbolic name value with module abbreviation as prefix only
 *         (AUTOSAR 4.0 rev2 <= AUTOSAR version )
 */
#define EthIf_[!"@name"!] [!"./EthIfCtrlIdx"!]U
#endif /* ETHIF_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDLOOP!]

/* EthIfSwitch */[!/*
*/!][!LOOP "as:modconf('EthIf')[1]/EthIfConfigSet/EthIfSwitch/*"!]

#if (defined ETHIF_PROVIDE_LEGACY_SYMBOLIC_NAMES)
#if (defined EthIf_[!"@name"!])
#error EthIf_[!"@name"!] is already defined
#endif
/** \brief Export symbolic name value with module abbreviation as prefix only
 *         (AUTOSAR 4.0 rev2 <= AUTOSAR version )
*/
#define EthIf_[!"@name"!] [!"./EthIfSwitchIdx"!]U
#endif /* ETHIF_PROVIDE_LEGACY_SYMBOLIC_NAMES */
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

#endif /* ifndef ETHIF_SYMBOLICNAMES_PBCFG_H */
/*==================[end of file]===========================================*/
[!ENDCODE!][!//
