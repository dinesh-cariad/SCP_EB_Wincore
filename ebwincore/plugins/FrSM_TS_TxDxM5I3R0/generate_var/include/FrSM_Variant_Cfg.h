/**
 * \file
 *
 * \brief AUTOSAR FrSM
 *
 * This file contains the implementation of the AUTOSAR
 * module FrSM.
 *
 * \version 5.3.18
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[includes]==============================================*/
[!INCLUDE "../../generate/include/FrSM_Variables.m"!][!//
#include <FrSM.h>      /* FrSM interface headerfile */
#include <FrSM_CfgTypes.h>      /* get FrSM pre-compile-time configuration  */

/*==================[macros]================================================*/
[!VAR "cfgName"="FrSMConfig/*[1]/@name"!]

[!IF "var:defined('postBuildVariant')"!]
[!/* Current postbuild configuration name
*/!][!VAR "initPredefinedCfgName"="concat($cfgName,'_',$postBuildVariant)"!]
[!ELSE!]
[!/* Current postbuild name
*/!][!VAR "initPredefinedCfgName"="$cfgName"!]
[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'Map name of post-build configuration to internal representation'",
  "Name"="$initPredefinedCfgName"!] ([!"$initPredefinedCfgName"!]_Layout.RootCfg)

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

#define FRSM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <FrSM_MemMap.h>  /* !LINKSTO FrSm.ASR40.FrSm057,1 */

/** \brief Internal post-build configuration structure */
extern CONST(FrSM_CfgType, FRSM_APPL_CONST) [!"$initPredefinedCfgName"!]_Layout;

#define FRSM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <FrSM_MemMap.h>  /* !LINKSTO FrSm.ASR40.FrSm057,1 */

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
