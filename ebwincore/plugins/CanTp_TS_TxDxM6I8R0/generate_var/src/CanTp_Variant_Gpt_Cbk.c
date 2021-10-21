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

/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true
 *
 * Reason: False-positive from the Tasking v5.0r2 compiler (TCVX-41885)
 */

/* CHECK: NOPARSE */

[!CODE!]
[!AUTOSPACING!]

/*==================[inclusions]=============================================*/
[!INCLUDE "../../generate_macros/CanTp_PostBuild.m"!][!//
[!/*
*/!][!IF "CanTpGeneral/CanTpGptUsageEnable = 'true'"!][!/*
*/!]
[!IF "var:defined('postBuildVariant')"!]
[!/* Current PB variant Sufix
  */!][!VAR "VariantSuffix"="concat('_',$postBuildVariant)"!]
[!ELSE!]
[!/* Current PB variant Sufix
  */!][!VAR "VariantSuffix"="string("")"!]
[!ENDIF!]
#include <TSAutosar.h>         /* EB specific standard types */
#include <ComStack_Types.h>    /* typedefs for AUTOSAR com stack */
#include <CanTp_Internal.h>    /* internal macros and variables */
#include <CanTp_Cfg.h>

#if(CANTP_JUMPTABLE_SUPPORT == STD_ON)
#include <CanTp_EntryTable.h>  /* CanTp entry jumptable */
#endif /* CANTP_JUMPTABLE_SUPPORT == STD_ON */

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

#define CANTP_START_SEC_CODE
#include <CanTp_MemMap.h>
[!/*
*/!][!FOR "I"="0" TO "$numTxChannels - 1"!][!/*
   */!][!VAR "chanRef"!][!CALL "getChanRefByChanId", "chanId"="$I"!][!ENDVAR!][!/*
   */!][!IF "node:ref($chanRef)/CanTpSTminTimeoutHandling = 'Gpt'"!][!/*
     */!][!VAR "callbackName"="node:ref($chanRef)/CanTpGptChannelCallbackName"!]
FUNC(void, CANTP_CODE) [!"concat($callbackName, $VariantSuffix)"!](void)
{
  /* redirect */

[!IF "node:exists(as:modconf('CanTp')[1]/CanTpGeneral/CanTpMaxParallelTxChannels)"!]
  CanTp_ChType Channel;  /* counter for channels */

   /* check counter for every parallel channel, call callback routine if necessary */
   /* Deviation TASKING-1 */
   for(Channel = 0U; Channel < CANTP_MAX_TX_CHANNELS; Channel++)
   {
     P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr = &CanTp_Channel[Channel];

     if([!"num:i($I)"!]U == CANTP_CFG_GET_TXNSDU_CONFIG(ChannelPtr->NSduId).Channel)
     {
       [!IF "CanTpJumpTable/CanTpJumpTableMode = 'CLIENT'"!][!//
         (CanTp_EntryTablePtr->STminCallback)(Channel);
       [!ELSE!][!//
         CanTp_STminCallback(Channel);
       [!ENDIF!][!//
       break;
     }
   }
[!ELSE!]
  [!IF "CanTpJumpTable/CanTpJumpTableMode = 'CLIENT'"!][!//
  (CanTp_EntryTablePtr->STminCallback)([!"num:i($I)"!]U);
  [!ELSE!][!//
  CanTp_STminCallback([!"num:i($I)"!]U);
  [!ENDIF!][!//
[!ENDIF!]

}
[!/*
   */!][!ENDIF!][!/*
*/!][!ENDFOR!][!/*
*/!]
#define CANTP_STOP_SEC_CODE
#include <CanTp_MemMap.h>

/*==================[internal function definitions]==========================*/
[!/*
*/!][!ENDIF!][!/* "CanTpGeneral/CanTpGptUsageEnable = 'true'"*/!]
/*==================[end of file]============================================*/
[!ENDCODE!]
/* CHECK: PARSE */
