[!CODE!][!//
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
#include <Eep.h>                                     /* includes Eep header */ 

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/
[!IF "count(text:grep(EepInitConfiguration/*/EepJobEndNotification | EepInitConfiguration/*/EepJobErrorNotification, '^(\s*NULL_PTR\s*)|(\s*NULL\s*)$')) != count(EepInitConfiguration/*) * 2"!][!//
#define EEP_START_SEC_CODE
#include <Eep_MemMap.h>

[!LOOP "text:order(text:split(normalize-space(text:join(EepInitConfiguration/*/EepJobEndNotification | EepInitConfiguration/*/EepJobErrorNotification))))"!][!//
[!IF ". != 'NULL_PTR' and . != 'NULL'"!][!//
/* Declaration of Eep notification function ([!"."!]())*/
extern FUNC( void, EEP_CODE ) [!"."!]( void );

[!ENDIF!][!//
[!ENDLOOP!][!//
#define EEP_STOP_SEC_CODE
#include <Eep_MemMap.h>
[!ENDIF!][!//

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/
#define EEP_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <Eep_MemMap.h>

[!LOOP "EepInitConfiguration/*"!][!//
/* Eep module initialization data ([!"node:name(.)"!])*/
CONST(Eep_ConfigType, EEP_CONST) [!"node:name(.)"!] =
{
  [!"./EepDefaultMode"!],
  [!"./EepSize"!]U,
  [!"./EepFastReadBlockSize"!]U,
  [!"./EepFastWriteBlockSize"!]U,
  [!"./EepNormalReadBlockSize"!]U,
  [!"./EepNormalWriteBlockSize"!]U,
  [!IF "normalize-space(./EepJobEndNotification) = 'NULL'"!]NULL_PTR[!ELSE!][!"./EepJobEndNotification"!][!ENDIF!], /* EepJobEndNotification */
  [!IF "normalize-space(./EepJobErrorNotification) = 'NULL'"!]NULL_PTR[!ELSE!][!"./EepJobErrorNotification"!][!ENDIF!], /* EepJobErrorNotification */  
};
[!ENDLOOP!][!//

#define EEP_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <Eep_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
[!ENDCODE!][!//
