/**
 * \file
 *
 * \brief AUTOSAR CanTp
 *
 * This file contains the implementation of the AUTOSAR
 * module CanTp.
 *
 * \version 6.8.42
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
/* CHECK: NOPARSE */
[!IF "var:defined('postBuildVariant')"!]
[!/* Current PB variant Sufix
  */!][!VAR "VariantSuffix"="concat('_',$postBuildVariant)"!]
#ifndef CANTP_GPT[!"$postBuildVariant"!]_CBK_H
#define CANTP_GPT[!"$postBuildVariant"!]_CBK_H
[!ELSE!]
[!/* Current PB variant Sufix
  */!][!VAR "VariantSuffix"="string("")"!]
#ifndef CANTP_GPT_CBK_H
#define CANTP_GPT_CBK_H
[!ENDIF!]

/*==================[includes]===============================================*/
[!INCLUDE "../../generate_macros/CanTp_PostBuild.m"!][!//
[!IF "CanTpGeneral/CanTpGptUsageEnable = 'true'"!]
#include <TSAutosar.h>      /* EB specific standard types */
#include <ComStack_Types.h> /* typedefs for AUTOSAR com stack */

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[external function declarations]=========================*/

#define CANTP_START_SEC_CODE
#include <CanTp_MemMap.h>

  [!FOR "I"="0" TO "$numTxChannels - 1"!]
  [!VAR "chanRef"!][!CALL "getChanRefByChanId", "chanId"="$I"!][!ENDVAR!]
  [!IF "node:ref($chanRef)/CanTpSTminTimeoutHandling = 'Gpt'"!]
  [!VAR "callbackName"="node:ref($chanRef)/CanTpGptChannelCallbackName"!]
  /** \brief [!"concat($callbackName, $VariantSuffix)"!]()
  **
  ** This function is called by the Gpt as callback function
  ** for channel [!"node:name(node:ref($chanRef))"!]
  **
  ** Redirect function call to STmin callback with channel as parameter
  */
  extern FUNC(void, CANTP_CODE) [!"concat($callbackName, $VariantSuffix)"!](void);

  [!ENDIF!]
  [!ENDFOR!]

#define CANTP_STOP_SEC_CODE
#include <CanTp_MemMap.h>

/*==================[end of file]============================================*/
[!IF "var:defined('postBuildVariant')"!]
#endif /* ifndef CANTP_GPT[!"$postBuildVariant"!]_CBK_H */
[!ELSE!]
#endif /* ifndef CANTP_GPT_CBK_H */
[!ENDIF!]
[!ENDIF!][!//"CanTpGeneral/CanTpGptUsageEnable = 'true'"
/* CHECK: PARSE */

