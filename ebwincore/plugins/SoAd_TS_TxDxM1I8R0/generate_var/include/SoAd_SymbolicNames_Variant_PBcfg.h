/**
 * \file
 *
 * \brief AUTOSAR SoAd
 *
 * This file contains the implementation of the AUTOSAR
 * module SoAd.
 *
 * \version 1.8.16
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
[!CODE!]
[!AUTOSPACING!]

[!IF "var:defined('postBuildVariant')"!][!/*
    Current PB variant Sufix
  */!][!VAR "SOAD_VARIANT_SUFIX"="concat('_',$postBuildVariant)"!]

#ifndef SOAD_SYMBOLICNAMES[!"$postBuildVariant"!]_PBCFG_H
#define SOAD_SYMBOLICNAMES[!"$postBuildVariant"!]_PBCFG_H
[!ELSE!][!/*
  Current PB variant Sufix
  */!][!VAR "SOAD_VARIANT_SUFIX"="string("")"!]

#ifndef SOAD_SYMBOLICNAMES_PBCFG_H
#define SOAD_SYMBOLICNAMES_PBCFG_H
[!ENDIF!]

/*==================[includes]==============================================*/

/*==================[macros]================================================*/

/* -----------------[SoAdSocketId]----------------------------------------- */
[!LOOP "as:modconf('SoAd')[1]/SoAdConfig/*[1]/SoAdSocketConnectionGroup/*/SoAdSocketConnection/*"!][!//

#if (defined SoAdConf_[!"../../@name"!]_[!"@name"!][!"$SOAD_VARIANT_SUFIX"!])
#error SoAdConf_[!"../../@name"!]_[!"@name"!][!"$SOAD_VARIANT_SUFIX"!] is already defined
#endif
/** \brief Export symbolic name value for [!"../../@name"!]_[!"@name"!] */
#define SoAdConf_[!"../../@name"!]_[!"@name"!][!"$SOAD_VARIANT_SUFIX"!] [!"SoAdSocketId"!]U
[!ENDLOOP!][!//

/* -----------------[SoAdTxPduId]------------------------------------------ */
[!LOOP "as:modconf('SoAd')[1]/SoAdConfig/*[1]/SoAdPduRoute/*"!][!//

#if (defined SoAdConf_[!"@name"!][!"$SOAD_VARIANT_SUFIX"!])
#error SoAdConf_[!"@name"!][!"$SOAD_VARIANT_SUFIX"!] is already defined
#endif
/** \brief Export symbolic name value for [!"@name"!] */
#define SoAdConf_[!"@name"!][!"$SOAD_VARIANT_SUFIX"!] [!"SoAdTxPduId"!]U
[!ENDLOOP!][!//

/* -----------------[SoAdRxPduId]------------------------------------------ */
[!LOOP "as:modconf('SoAd')[1]/SoAdConfig/*[1]/SoAdSocketRoute/*/SoAdSocketRouteDest/*/SoAdRxPduId/.."!][!//

#if (defined SoAdConf_[!"../../@name"!]_[!"@name"!][!"$SOAD_VARIANT_SUFIX"!])
#error SoAdConf_[!"../../@name"!]_[!"@name"!][!"$SOAD_VARIANT_SUFIX"!] is already defined
#endif
/** \brief Export symbolic name value for [!"../../@name"!]_[!"@name"!] */
#define SoAdConf_[!"../../@name"!]_[!"@name"!][!"$SOAD_VARIANT_SUFIX"!] [!"SoAdRxPduId"!]U
[!ENDLOOP!][!//

/* -----------------[SoAdRoutingGroupId]----------------------------------- */
[!LOOP "as:modconf('SoAd')[1]/SoAdConfig/*[1]/SoAdRoutingGroup/*"!][!//

#if (defined SoAdConf_[!"@name"!][!"$SOAD_VARIANT_SUFIX"!])
#error SoAdConf_[!"@name"!][!"$SOAD_VARIANT_SUFIX"!] is already defined
#endif
/** \brief Export symbolic name value for [!"@name"!] */
#define SoAdConf_[!"@name"!][!"$SOAD_VARIANT_SUFIX"!] [!"SoAdRoutingGroupId"!]U
[!ENDLOOP!][!//

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

[!IF "var:defined('postBuildVariant')"!]
#endif /* ifndef SOAD_SYMBOLICNAMES[!"$postBuildVariant"!]_PBCFG_H */
[!ELSE!]
#endif /* ifndef SOAD_SYMBOLICNAMES_PBCFG_H */
[!ENDIF!]
/*==================[end of file]===========================================*/
[!ENDCODE!]