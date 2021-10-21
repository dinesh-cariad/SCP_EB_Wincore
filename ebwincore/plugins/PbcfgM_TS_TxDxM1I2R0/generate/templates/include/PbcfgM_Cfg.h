/**
 * \file
 *
 * \brief AUTOSAR PbcfgM
 *
 * This file contains the implementation of the AUTOSAR
 * module PbcfgM.
 *
 * \version 1.2.22
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef PBCFGM_CFG_H
#define PBCFGM_CFG_H

/* This file contains all target independent public configuration declarations
 * for the AUTOSAR module PbcfgM. */

[!AUTOSPACING!]
[!//

/*==================[inclusions]============================================*/
#include <Std_Types.h>

/*==================[General]===============================================*/

[!SELECT "PbcfgMGeneral"!][!//
#if (defined PBCFGM_DEV_ERROR_DETECT)
#error PBCFGM_DEV_ERROR_DETECT already defined
#endif
/** \brief Switch, indicating if development error detection is activated for
 ** the PbcfgM */
[!IF "node:value(PbcfgMDevErrorDetect) = 'true'"!][!//
#define PBCFGM_DEV_ERROR_DETECT STD_ON
[!ELSE!][!//
#define PBCFGM_DEV_ERROR_DETECT STD_OFF
[!ENDIF!][!//


#if (defined PBCFGM_RELOCATABLE_CFG_ENABLE)
#error PBCFGM_RELOCATABLE_CFG_ENABLE is already defined
#endif
/** \brief Enable the relocation feature */
[!IF "node:value(PbcfgMRelocatableCfgEnable) = 'true'"!][!//
#define PBCFGM_RELOCATABLE_CFG_ENABLE STD_ON
[!ELSE!][!//
#define PBCFGM_RELOCATABLE_CFG_ENABLE STD_OFF
[!ENDIF!][!//
[!ENDSELECT!][!//

/*==================[macros]================================================*/
#if (defined PBCFGM_PUBLIC_INFO_SIGNATURE)
#error PBCFGM_PUBLIC_INFO_SIGNATURE is already defined
#endif

/** \brief Link time verification value */
#define PBCFGM_PUBLIC_INFO_SIGNATURE [!"asc:getConfigSignature(node:difference(as:modconf('PbcfgM')[1]/CommonPublishedInformation//*[not(child::*) and (node:configclass() = 'PublishedInformation')], as:modconf('PbcfgM')[1]/CommonPublishedInformation/Release))"!]U

#if (defined PBCFGM_CFG_SIGNATURE)
#error PBCFGM_CFG_SIGNATURE is already defined
#endif

/** \brief Compile time configuration signature
**
** Used to validate the post build configuration against the compile time configuration */
#define PBCFGM_CFG_SIGNATURE [!"asc:getConfigSignature(as:modconf('PbcfgM')[1]//*[not(child::*) and (node:configclass() = 'PreCompile') ])"!]U /* Compile time verification value */

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef PBCFGM_CFG_H */
/*==================[end of file]===========================================*/
