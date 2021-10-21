/**
 * \file
 *
 * \brief AUTOSAR CanSM
 *
 * This file contains the implementation of the AUTOSAR
 * module CanSM.
 *
 * \version 3.7.5
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
  */!][!VAR "CANSM_VARIANT_SUFIX"="concat('_',$postBuildVariant)"!]

#ifndef CANSM_SYMBOLICNAMES_[!"$postBuildVariant"!]_PBCFG_H
#define CANSM_SYMBOLICNAMES_[!"$postBuildVariant"!]_PBCFG_H
[!ELSE!]
  [!/* Current PB variant Sufix
  */!][!VAR "CANSM_VARIANT_SUFIX"="string("")"!]
#ifndef CANSM_SYMBOLICNAMES_PBCFG_H
#define CANSM_SYMBOLICNAMES_PBCFG_H
[!ENDIF!]

/*==================[includes]===============================================*/
/*==================[macros]=================================================*/

/*------------------[symbolic name values]----------------------------------*/

[!LOOP "as:modconf('CanSM')[1]/CanSMConfiguration/*[1]/CanSMManagerNetwork/*"!]

#if (defined CANSM_PROVIDE_LEGACY_SYMBOLIC_NAMES)

#if (defined CanSM_[!"@name"!][!"$CANSM_VARIANT_SUFIX"!] )
#error CanSM_[!"@name"!][!"$CANSM_VARIANT_SUFIX"!] is already defined
#endif /* #if (defined CanSM_[!"@name"!][!"$CANSM_VARIANT_SUFIX"!] ) */

/** \brief Export symbolic name value */
#define CanSM_[!"@name"!][!"$CANSM_VARIANT_SUFIX"!] [!"as:ref(CanSMComMNetworkHandleRef)/ComMChannelId"!]U

#endif /* #if (defined CANSM_PROVIDE_LEGACY_SYMBOLIC_NAMES) */

[!ENDLOOP!]

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
#endif /* ifndef CANSM_SYMBOLICNAMES_[!"$postBuildVariant"!]_PBCFG_H */
[!ELSE!]
#endif /* ifndef CANSM_SYMBOLICNAMES_PBCFG_H */
[!ENDIF!]
/*==================[end of file]============================================*/
[!ENDCODE!]
