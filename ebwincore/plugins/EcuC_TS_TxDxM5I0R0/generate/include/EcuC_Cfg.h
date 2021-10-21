/**
 * \file
 *
 * \brief AUTOSAR EcuC
 *
 * This file contains the implementation of the AUTOSAR
 * module EcuC.
 *
 * \version 5.0.21
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

[!AUTOSPACING!]

[!SELECT "as:modconf('EcuC')[1]"!][!//
[!INCLUDE "../include/CommonMacros.m"!][!//
[!INCLUDE "../include/EcuC_Vars.m"!][!//
[!//

#ifndef ECUC_CFG_H
#define ECUC_CFG_H

/*==================[inclusions]============================================*/

#include <EcuC_CfgTypes.h>

/*==================[macros]================================================*/

[!CALL "GuardedDefine", "Comment"="'Development error detection support'",
 "Name"="'ECUC_DEV_ERROR_DETECT'"!][!//
[!IF "as:modconf('EcuC')[1]/EcucGeneral/EcucDevErrorDetect = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'EcuC Meta Data Handling is enabled'",
  "Name"="'ECUC_META_DATA_HANDLING_ENABLED'"!][!//
[!IF "as:modconf('EcuC')[1]/EcucGeneral/EcucMetaDataHandlingEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!IF "as:modconf('EcuC')[1]/EcucGeneral/EcucMetaDataHandlingEnabled = 'true'"!][!//Only do it if Meta Data Handling is enabled
[!CALL "GuardedDefine", "Comment"="'Maximum size of Meta Data Array with respect to a 64 bit type'",
 "Name"="'ECUC_META_DATA_CONFIG_SIZE_MAX_64'"!][!//
[!"num:i($Meta_Data_Config_Size_Max)"!]U

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

#define ECUC_START_SEC_CONST_UNSPECIFIED
#include <EcuC_MemMap.h>

/* Global configuration data */
extern CONST(EcuC_ConfigType, ECUC_CONST) EcuC_ConfigData;

#define ECUC_STOP_SEC_CONST_UNSPECIFIED
#include <EcuC_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/

[!ENDIF!][!//Meta Data Handling is enabled

#endif   /* ECUC_CFG_H */
[!ENDSELECT!]
