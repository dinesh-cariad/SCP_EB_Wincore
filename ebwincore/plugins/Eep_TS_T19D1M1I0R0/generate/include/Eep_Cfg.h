[!CODE!][!//
#ifndef EEP_CFG_H
#define EEP_CFG_H

/**
 * \file
 *
 * \brief AUTOSAR Eep
 *
 * This file contains the implementation of the AUTOSAR
 * module Eep.
 *
 * \version 1.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[includes]==============================================*/
#include <Eep_Types.h>                     /* includes Eep specific typedef */ 
#include <MemIf_Types.h>                   /* includes MemIf shared typedef */

/*==================[macros]================================================*/
/* used for Eep_Compare() purposes as internal data buffer, it's size has to
   be properly setup! */
#define EEP_DATABUFFER_SIZE 32U

/** \brief Version info API availabiliity
 **
 ** En- or disable the availability of Eep_GetVersionInfo() */
[!SELECT "EepGeneral"!][!//
#define EEP_VERSION_INFO_API                [!//
[!IF "EepVersionInfoApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!ENDSELECT!][!//

/** \brief Development error detection
 **
 ** En- or disable Development error detection */
[!SELECT "EepGeneral"!][!//
#define EEP_DEV_ERROR_DETECT                [!//
[!IF "EepDevErrorDetect = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!ENDSELECT!][!//

/** \brief EepFileName
 **
 ** EEPROM file name */
[!SELECT "EepGeneral"!][!//
[!IF "EepFilePath= ''"!][!//
#define EEP_FILE_NAME "[!"EepFileName"!]"
[!ELSE!][!//
#define EEP_FILE_NAME "[!"EepFilePath"!]/[!"EepFileName"!]"
[!ENDIF!][!//
[!ENDSELECT!][!//

#if (defined EepConf_EepGeneral_EepDriverIndex)
#error EepConf_EepGeneral_EepDriverIndex is already defined
#endif
/** \brief Configuration parameter EepDriverIndex - mapping parameter name <-> id */
#define EepConf_EepGeneral_EepDriverIndex [!"node:value(EepGeneral/EepDriverIndex)"!]U

#ifndef EEP_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#if (defined EepDriverIndex)
#error EepDriverIndex is already defined
#endif
/** \brief Configuration parameter EepDriverIndex - mapping parameter name <-> id (AUTOSAR version <= 3.1 rev4) */
#define EepDriverIndex [!"node:value(EepGeneral/EepDriverIndex)"!]U

#if (defined Eep_EepDriverIndex)
#error Eep_EepDriverIndex is already defined
#endif
/** \brief Configuration parameter EepDriverIndex - mapping parameter name <-> id (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2) */
#define Eep_EepDriverIndex [!"node:value(EepGeneral/EepDriverIndex)"!]U
#endif /* ifndef EEP_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES */

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/
#define EEP_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <Eep_MemMap.h>

[!LOOP "EepInitConfiguration/*"!][!//
/* Declaration of configuration structure ([!"node:name(.)"!])*/
extern CONST(Eep_ConfigType, EEP_CONST) [!"node:name(.)"!];
[!ENDLOOP!][!//

#define EEP_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <Eep_MemMap.h>


/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
#endif /* ifndef EEP_CFG_H */
[!ENDCODE!][!//
