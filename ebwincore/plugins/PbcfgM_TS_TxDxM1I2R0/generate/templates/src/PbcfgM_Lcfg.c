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

/*==================[inclusions]============================================*/

#include <TSAutosar.h>           /* EB specific standard types */
#include <Std_Types.h>           /* AUTOSAR standard types */
#include <PbcfgM_Lcfg.h>         /* Generated configuration */

[!MACRO "GetPostfixedModuleName", "modulePath"!][!/*
  */!][!VAR "_ModuleName" = "substring-before(substring-after(node:path($modulePath),'/AUTOSAR/TOP-LEVEL-PACKAGES/'),'/')"!][!/*
  */!][!VAR "_ModuleNamePostfixed" = "$_ModuleName"!][!/*
  */!][!IF "node:exists(as:modconf($_ModuleName)[1]/CommonPublishedInformation/VendorApiInfix)"!][!/*
    */!][!VAR "_ModuleNamePostfixed" = "concat($_ModuleNamePostfixed,'_')"!][!/*
    */!][!VAR "_ModuleNamePostfixed" = "concat($_ModuleNamePostfixed,node:value(as:modconf($_ModuleName)[1]/CommonPublishedInformation/VendorId))"!][!/*
    */!][!IF "node:value(as:modconf($_ModuleName)[1]/CommonPublishedInformation/VendorApiInfix) != ''"!][!/*
     */!][!VAR "_ModuleNamePostfixed" = "concat($_ModuleNamePostfixed,'_')"!][!/*
      */!][!VAR "_ModuleNamePostfixed" = "concat($_ModuleNamePostfixed,node:value(as:modconf($_ModuleName)[1]/CommonPublishedInformation/VendorApiInfix))"!][!/*
    */!][!ENDIF!][!/*
  */!][!ENDIF!][!/*
  */!][!"$_ModuleNamePostfixed"!][!/*
*/!][!ENDMACRO!][!//
[!//
/* Includes for referenced modules */
[!LOOP "PbcfgMBswModules/*"!][!//
[!VAR "ModuleNamePostfixed"!][!CALL "GetPostfixedModuleName", "modulePath"="node:path(node:ref(./PbcfgMBswModuleRef))"!][!ENDVAR!][!//
#include <[!"$ModuleNamePostfixed"!].h>
[!ENDLOOP!]

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/
/* start constants section declaration */
#define PBCFGM_START_SEC_CONST_32
#include <PbcfgM_MemMap.h>

/* Constant variable used to validate post build configuration against link time configuration */
CONST(uint32, PBCFGM_CONST) PbcfgM_LcfgSignature = [!"asc:getConfigSignature(as:modconf('PbcfgM')[1]//*[not(child::*) and (node:configclass() = 'Link')])"!]U;

/* stop data section declaration */
#define PBCFGM_STOP_SEC_CONST_32
#include <PbcfgM_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/* start data section declaration */
#define PBCFGM_START_SEC_CONST_UNSPECIFIED
#include <PbcfgM_MemMap.h>

/* These entries do consider modules which can have more than one instance */
CONSTP2VAR(PbcfgM_IsValidConfigFuncType, PBCFGM_CONST, PBCFGM_APPL_CODE) PbcfgM_IsValidCfgFuncPtr[[!"num:i(count(PbcfgMBswModules/*)+1)"!]] =
{[!//
[!LOOP "PbcfgMBswModules/*"!][!//
    [!VAR "ModuleNamePostfixed"!][!CALL "GetPostfixedModuleName", "modulePath"="node:path(node:ref(./PbcfgMBswModuleRef))"!][!ENDVAR!][!//
    &[!"$ModuleNamePostfixed"!]_IsValidConfig,[!//
[!ENDLOOP!]
    NULL_PTR
};

/* stop data section declaration */
#define PBCFGM_STOP_SEC_CONST_UNSPECIFIED
#include <PbcfgM_MemMap.h>

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
