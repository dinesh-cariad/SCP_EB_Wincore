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
#ifndef PBCFGM_LCFG_H
#define PBCFGM_LCFG_H

[!AUTOSPACING!]
/*==================[includes]==============================================*/

#include <TSAutosar.h>
#include <PbcfgM_Types.h>

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/* start data section declaration */
#define PBCFGM_START_SEC_CONST_32
#include <PbcfgM_MemMap.h>

/* Variable used to validate post build configuration against link time configuration */
extern CONST(uint32, PBCFGM_CONST) PbcfgM_LcfgSignature;

/* stop data section declaration */
#define PBCFGM_STOP_SEC_CONST_32
#include <PbcfgM_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

 /* start data section declaration */
 #define PBCFGM_START_SEC_CONST_UNSPECIFIED
 #include <PbcfgM_MemMap.h>

 extern CONSTP2VAR(PbcfgM_IsValidConfigFuncType, PBCFGM_CONST, PBCFGM_APPL_CODE) PbcfgM_IsValidCfgFuncPtr[[!"num:i(count(PbcfgMBswModules/*)+1)"!]];

 /* stop data section declaration */
 #define PBCFGM_STOP_SEC_CONST_UNSPECIFIED
 #include <PbcfgM_MemMap.h>

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef PBCFGM_LCFG_H */
/*==================[end of file]===========================================*/
