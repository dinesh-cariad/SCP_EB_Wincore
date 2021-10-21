/**
 * \file
 *
 * \brief AUTOSAR LinSM
 *
 * This file contains the implementation of the AUTOSAR
 * module LinSM.
 *
 * \version 3.4.19
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

 /* !LINKSTO LinSM.ASR40.LINSM078,1 */
[!AUTOSPACING!]
[!INCLUDE "../include/LinSM_Macros.m"!]
[!CODE!]

/*==================[inclusions]============================================*/

#include <LinSM_Internal.h>
[!IF "$multiCoreSupport = 'true'"!]
#include <SchM_LinSM.h>
[!ENDIF!]

/*==================[macros]================================================*/

[!/*
*** macro for calculating mainfunction ticks for a timer value. ***
    NOTE: this macro shall only be called from within a loop over LinSMChannel!
    this macro returns:
    - if timerLcl is 0 (special value to say "don't use timer"), 0
    - if timerLcl is divisible by mainperiod, the division result plus 1
    - if timerLcl isn't divisible by mainperiod, the division result rounded up
      to the next integer plus 1
    to accommodate possible floating point rounding errors, the division result
    is allowed to be off by about 0.00001.
    (see [ecuc_sws_7000] for reference)
*/!][!MACRO "calcTimerValue", "Timer"!][!/*
  */!][!VAR "timerLcl" = "$Timer"!][!/*
  */!][!IF "$timerLcl = 0"!][!/*
    */!]0[!/*
  */!][!ELSE!][!/*
    */!][!VAR "mainperiod" = "../../../../LinSMGeneral/LinSMMainProcessingPeriod"!][!/*
    */!][!VAR "ticks" = "num:i(($timerLcl div $mainperiod) - 0.00001) + 1"!][!/*
    */!][!VAR "dValue" = "($timerLcl div $mainperiod)"!][!/*
    */!][!IF "(($dValue - $ticks) > 0.00001) or (($dValue - $ticks) < -0.00001)"!][!/*
      */!][!WARNING!][!/*
        */!]["!$Timer!"] value of channel [!"name(.)"!] [!/*
        */!]([!"$timerLcl"!]s) is not divisible by LinSMMainProcessingPeriod. [!/*
        */!]It will be rounded to: [!"$ticks * $mainperiod"!]s [!/*
        */!]([!"num:i($ticks)"!] Ticks).[!/*
      */!][!ENDWARNING!][!/*
    */!][!ENDIF!][!/*
    */!][!"num:i($ticks + 1)"!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDMACRO!]



/*==================[type definitions]======================================*/

/*==================[internal function declarations]========================*/

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

#define LINSM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */

/* channel configuration */
CONST(LinSM_ChannelConfigType, LINSM_APPL_CONST) LinSM_ChannelConfig[[!"$numChannels"!]] =
{
[!LOOP "node:order(LinSMConfigSet/*[1]/LinSMChannel/*,'as:ref(LinSMComMNetworkHandleRef)/ComMChannelId')"!]
  { /* channel [!"name(.)"!] */
  [!IF "$multiCoreSupport = 'true'"!]
  [!WS "4"!]&SchM_Call_LinSM_RequiredCSEntry_ComM_BusSM_ModeIndication_[!"as:ref(LinSMComMNetworkHandleRef)/ComMChannelId"!], /* SchM function pointer */
  [!IF "$numSlaveNodes > 0"!][!//
[!WS "4"!][!IF "LinSMNodeType = 'SLAVE'"!]&SchM_Call_LinSM_RequiredCSEntry_ComM_BusSM_BusSleepMode_[!"as:ref(LinSMComMNetworkHandleRef)/ComMChannelId"!][!ELSE!]NULL_PTR[!ENDIF!], /* SchM function pointer */
  [!ENDIF!]
  [!ENDIF!]
[!WS "4"!][!CALL "calcTimerValue", "Timer" = "LinSMConfirmationTimeout"!]U,  /* confirmation timeout */
  [!IF "$numSlaveNodes > 0"!][!//
[!WS "4"!][!IF "LinSMNodeType = 'SLAVE'"!][!CALL "calcTimerValue", "Timer" = "LinSMSilenceAfterWakeupTimeout"!]U[!ELSE!]65535U[!ENDIF!],  /* silence after wakeup timeout */
  [!ENDIF!]
  [!IF "$LinSMIDMapComMType = 'LUT'"!]
    [!CALL "LinSM_GetID", "Path"="as:path(.)", "Dest" = "'ComM'"!]
    [!WS "4"!][!"$LinSM_HandleID"!]U, /* ComM network identifier */
  [!ENDIF!]
  [!IF "$LinSMIDMapLinIfType = 'LUT'"!]
    [!CALL "LinSM_GetID", "Path"="as:path(.)", "Dest" = "'LinIf'"!]
    [!WS "4"!][!"$LinSM_HandleID"!]U, /* LinIf channel identifier */
  [!ENDIF!]
  [!IF "$sleepSupportType = 'mixed' or $trcvSupportType != 'none'"!]
  [!VAR "slpSpprtVal" = "num:i(0)"!]
  [!IF "LinSMSleepSupport = 'true'"!]
    [!VAR "slpSpprtVal" = "bit:bitset($slpSpprtVal, 0)"!]
  [!ENDIF!]
  [!IF "node:exists(./LinSMTransceiverPassiveMode)"!]
    [!VAR "slpSpprtVal" = "bit:bitset($slpSpprtVal, 1)"!]
    [!IF "LinSMTransceiverPassiveMode = 'true'"!]
      [!VAR "slpSpprtVal" = "bit:bitset($slpSpprtVal, 2)"!]
    [!ENDIF!]
  [!ENDIF!]
  [!WS "4"!][!"$slpSpprtVal"!]U, /* Sleep + Transceiver support */
  [!ENDIF!]
  [!WS "4"!][!"num:i(LinSMModeRequestRepetitionMax)"!]U,  /* repetition counter */
  [!IF "$numSlaveNodes > 0"!][!//
[!WS "4"!][!IF "LinSMNodeType = 'SLAVE'"!]TRUE[!ELSE!]FALSE[!ENDIF!], /* is node SLAVE (TRUE) or MASTER (FALSE) */
  [!ENDIF!]
  },
[!ENDLOOP!]
};

/* Empty configuration structure to be passed to LinSM_Init() */
CONST(LinSM_ConfigType, LINSM_APPL_CONST) [!"LinSMConfigSet/*[1]/@name"!] = 0U;

#define LINSM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */

/*==================[external data]=========================================*/

#define LINSM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */

/* runtime channel data - initialized by LinSM_Init() */
VAR(LinSM_ChannelDataType, LINSM_VAR_CLEARED) LinSM_ChannelData[[!"$numChannels"!]];

#define LINSM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */

/*==================[external function definitions]=========================*/

[!IF "$LinSMIDMapComMType = 'LUT'"!]
[!//
#define LINSM_START_SEC_CODE
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */

/* map ComM network handles to LinSM channel index */
FUNC(LinSM_ChannelIdType, LINSM_CODE) LinSM_ComMNwToId
(
  NetworkHandleType network
)
{
  LinSM_ChannelIdType i;

  for (i = 0U; i < [!"$numChannels"!]U; i++)
  {
    if (LinSM_ChannelConfig[i].ComMNetwork == network)
    {
      break;
    }
  }
  /* if no channel was found, this will return [!"$numChannels"!] */
  return i;
}

#define LINSM_STOP_SEC_CODE
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */
[!ENDIF!][!// IF "$LinSMIDMapComMType = 'LUT'"
[!//
/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/

[!ENDCODE!]
