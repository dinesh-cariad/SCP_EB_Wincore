/**
 * \file
 *
 * \brief AUTOSAR Dcm
 *
 * This file contains the implementation of the AUTOSAR
 * module Dcm.
 *
 * \version 5.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* !LINKSTO Dcm.Dsn.File.RoutineControlConst.ConfigC,1 */

[!/*=== Verification that Routine Control signals are not overlapping ===*/!][!//
[!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspRoutineInfo/*"!][!//
  [!LOOP "node:order(DcmDspRoutineRequestResOut/DcmDspRoutineRequestResOutSignal/* , 'DcmDspRoutineSignalPos')"!][!//
    [!VAR "POSN" = "DcmDspRoutineSignalPos"!][!//
    [!VAR "LENGTH" = "DcmDspRoutineSignalLength"!][!//
    [!VAR "nodeposn" = "node:pos(.) + 1"!][!//
    [!VAR "Name" = "node:name(.)"!][!//
    [!LOOP "../*[position() != $nodeposn]"!][!//
      [!IF "(($POSN >= DcmDspRoutineSignalPos) and ($POSN <= (DcmDspRoutineSignalPos + DcmDspRoutineSignalLength -1))) or ((($POSN + $LENGTH -1) >= DcmDspRoutineSignalPos) and (($POSN + $LENGTH -1) <= (DcmDspRoutineSignalPos + DcmDspRoutineSignalLength -1)))"!][!//
        [!ERROR!][!//
          signal [!"node:name(.)"!] overlaps with signal [!"$Name"!]
          Routine Information : '[!"node:name(../../..)"!]'
          Configuration       : '[!"node:name(../..)"!]'
         [!ENDERROR!][!//
      [!ENDIF!][!//
    [!ENDLOOP!][!//
  [!ENDLOOP!][!//
[!//
  [!LOOP "node:order(DcmDspRoutineRequestResIn/DcmDspRoutineRequestResInSignal/* , 'DcmDspRoutineSignalPos')"!][!//
    [!VAR "POSN" = "DcmDspRoutineSignalPos"!][!//
    [!VAR "LENGTH" = "DcmDspRoutineSignalLength"!][!//
    [!VAR "nodeposn" = "node:pos(.) + 1"!][!//
    [!VAR "Name" = "node:name(.)"!][!//
    [!LOOP "../*[position() != $nodeposn]"!][!//
      [!IF "(($POSN >= DcmDspRoutineSignalPos) and ($POSN <= (DcmDspRoutineSignalPos + DcmDspRoutineSignalLength -1))) or ((($POSN + $LENGTH -1) >= DcmDspRoutineSignalPos) and (($POSN + $LENGTH -1) <= (DcmDspRoutineSignalPos + DcmDspRoutineSignalLength -1)))"!][!//
        [!ERROR!][!//
          signal [!"node:name(.)"!] overlaps with signal [!"$Name"!]
          Routine Information : '[!"node:name(../../..)"!]'
          Configuration       : '[!"node:name(../..)"!]'
         [!ENDERROR!][!//
      [!ENDIF!][!//
    [!ENDLOOP!][!//
  [!ENDLOOP!][!//
[!//
  [!LOOP "node:order(DcmDspRoutineStopIn/DcmDspRoutineStopInSignal/* , 'DcmDspRoutineSignalPos')"!][!//
    [!VAR "POSN" = "DcmDspRoutineSignalPos"!][!//
    [!VAR "LENGTH" = "DcmDspRoutineSignalLength"!][!//
    [!VAR "nodeposn" = "node:pos(.) + 1"!][!//
    [!VAR "Name" = "node:name(.)"!][!//
    [!LOOP "../*[position() != $nodeposn]"!][!//
      [!IF "(($POSN >= DcmDspRoutineSignalPos) and ($POSN <= (DcmDspRoutineSignalPos + DcmDspRoutineSignalLength -1))) or ((($POSN + $LENGTH -1) >= DcmDspRoutineSignalPos) and (($POSN + $LENGTH -1) <= (DcmDspRoutineSignalPos + DcmDspRoutineSignalLength -1)))"!][!//
        [!ERROR!][!//
          signal [!"node:name(.)"!] overlaps with signal [!"$Name"!]
          Routine Information : '[!"node:name(../../..)"!]'
          Configuration       : '[!"node:name(../..)"!]'
        [!ENDERROR!][!//
      [!ENDIF!][!//
    [!ENDLOOP!][!//
  [!ENDLOOP!][!//
[!//
  [!LOOP "node:order(DcmDspRoutineStopOut/DcmDspRoutineStopOutSignal/* , 'DcmDspRoutineSignalPos')"!][!//
    [!VAR "POSN" = "DcmDspRoutineSignalPos"!][!//
    [!VAR "LENGTH" = "DcmDspRoutineSignalLength"!][!//
    [!VAR "nodeposn" = "node:pos(.) + 1"!][!//
    [!VAR "Name" = "node:name(.)"!][!//
    [!LOOP "../*[position() != $nodeposn]"!][!//
      [!IF "(($POSN >= DcmDspRoutineSignalPos) and ($POSN <= (DcmDspRoutineSignalPos + DcmDspRoutineSignalLength -1))) or ((($POSN + $LENGTH -1) >= DcmDspRoutineSignalPos) and (($POSN + $LENGTH -1) <= (DcmDspRoutineSignalPos + DcmDspRoutineSignalLength -1)))"!][!//
        [!ERROR!][!//
          signal [!"node:name(.)"!] overlaps with signal [!"$Name"!]
          Routine Information : '[!"node:name(../../..)"!]'
          Configuration       : '[!"node:name(../..)"!]'
        [!ENDERROR!][!//
      [!ENDIF!][!//
    [!ENDLOOP!][!//
  [!ENDLOOP!][!//
[!//
  [!LOOP "node:order(DcmDspStartRoutineIn/DcmDspStartRoutineInSignal/* , 'DcmDspRoutineSignalPos')"!][!//
    [!VAR "POSN" = "DcmDspRoutineSignalPos"!][!//
    [!VAR "LENGTH" = "DcmDspRoutineSignalLength"!][!//
    [!VAR "nodeposn" = "node:pos(.) + 1"!][!//
    [!VAR "Name" = "node:name(.)"!][!//
    [!LOOP "../*[position() != $nodeposn]"!][!//
      [!IF "(($POSN >= DcmDspRoutineSignalPos) and ($POSN <= (DcmDspRoutineSignalPos + DcmDspRoutineSignalLength -1))) or ((($POSN + $LENGTH -1) >= DcmDspRoutineSignalPos) and (($POSN + $LENGTH -1) <= (DcmDspRoutineSignalPos + DcmDspRoutineSignalLength -1)))"!][!//
        [!ERROR!][!//
          signal [!"node:name(.)"!] overlaps with signal [!"$Name"!]
          Routine Information : '[!"node:name(../../..)"!]'
          Configuration       : '[!"node:name(../..)"!]'
        [!ENDERROR!][!//
      [!ENDIF!][!//
    [!ENDLOOP!][!//
  [!ENDLOOP!][!//
[!//
  [!LOOP "node:order(DcmDspStartRoutineOut/DcmDspStartRoutineOutSignal/* , 'DcmDspRoutineSignalPos')"!][!//
    [!VAR "POSN" = "DcmDspRoutineSignalPos"!][!//
    [!VAR "LENGTH" = "DcmDspRoutineSignalLength"!][!//
    [!VAR "nodeposn" = "node:pos(.) + 1"!][!//
    [!VAR "Name" = "node:name(.)"!][!//
    [!LOOP "../*[position() != $nodeposn]"!][!//
      [!IF "(($POSN >= DcmDspRoutineSignalPos) and ($POSN <= (DcmDspRoutineSignalPos + DcmDspRoutineSignalLength -1))) or ((($POSN + $LENGTH -1) >= DcmDspRoutineSignalPos) and (($POSN + $LENGTH -1) <= (DcmDspRoutineSignalPos + DcmDspRoutineSignalLength -1)))"!][!//
        [!ERROR!][!//
          signal [!"node:name(.)"!] overlaps with signal [!"$Name"!]
          Routine Information : '[!"node:name(../../..)"!]'
          Configuration       : '[!"node:name(../..)"!]'
        [!ENDERROR!][!//
      [!ENDIF!][!//
    [!ENDLOOP!][!//
  [!ENDLOOP!][!//
[!ENDLOOP!][!//
[!//
/*==================[inclusions]=================================================================*/

[!AUTOSPACING!]
#include <Dcm_RoutineControl_Cfg.h>                             /*  RoutineControl Configuration */

[!INCLUDE "../include/Dcm_common.m"!][!//
/*==================[macros]=====================================================================*/

/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/

/*==================[internal function declarations]=============================================*/

/*==================[internal constants]=========================================================*/

/*==================[internal data]==============================================================*/

[!VAR "RoutineCount" = "0"!]
[!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspRoutine/*"!][!//
  [!IF "DcmDspRoutineUsed = 'true'"!]
    [!VAR "RoutineCount" = "$RoutineCount + 1"!]
  [!ENDIF!]
[!ENDLOOP!]
[!//
[!IF "$RoutineCount > 0 and ($DCM_DSP_USE_SERVICE_0X31 = 'STD_ON')"!][!//
[!//
[!IF "node:exists(node:refs(DcmConfigSet/*[1]/DcmDsp/DcmDspRoutine/*/DcmDspRoutineInfoRef)/DcmDspRoutineAuthorization/DcmDspRoutineSessionRef/*) or node:exists(node:refs(DcmConfigSet/*[1]/DcmDsp/DcmDspRoutine/*/DcmDspRoutineInfoRef)/DcmDspRoutineAuthorization/DcmDspRoutineSecurityLevelRef/*)"!][!//
#define DCM_START_SEC_CONST_8
#include <Dcm_MemMap.h>
[!//
[!VAR "sessioncount" = "0"!][!//
[!VAR "RoutineInfoList"="'#'"!][!//
[!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspRoutine/*"!][!//
  [!IF "DcmDspRoutineUsed = 'true'"!]
    [!IF "not(contains($RoutineInfoList, concat('#', name(node:ref(DcmDspRoutineInfoRef)), '#')))"!]
      [!VAR "sessioncount" = "$sessioncount + count(node:ref(DcmDspRoutineInfoRef)/DcmDspRoutineAuthorization/DcmDspRoutineSessionRef/*)"!][!//
      [!VAR "RoutineInfoList" = "concat($RoutineInfoList, name(node:ref(DcmDspRoutineInfoRef)), '#')"!]
    [!ENDIF!]
  [!ENDIF!]
[!ENDLOOP!][!//
[!//
[!IF "node:exists(node:refs(DcmConfigSet/*[1]/DcmDsp/DcmDspRoutine/*/DcmDspRoutineInfoRef)/DcmDspRoutineAuthorization/DcmDspRoutineSessionRef/*)"!]
[!VAR "RoutineInfoList"="'#'"!][!//

/* Array of session types for all routines */
STATIC CONST(Dcm_SesCtrlType, DCM_CONST) Dcm_RoutineControl_SessionTypes[[!"num:i($sessioncount)"!]] =
{
  [!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspRoutine/*"!][!//
    [!IF "DcmDspRoutineUsed = 'true'"!]
      [!IF "not(contains($RoutineInfoList, concat('#', name(node:ref(DcmDspRoutineInfoRef)), '#')))"!]
        [!SELECT "node:ref(DcmDspRoutineInfoRef)"!][!//
          [!LOOP "DcmDspRoutineAuthorization/DcmDspRoutineSessionRef/*"!][!//
            [!WS "2"!][!"num:inttohex(node:ref(.)/DcmDspSessionLevel, 2)"!]U,
          [!ENDLOOP!][!//
        [!ENDSELECT!][!//
        [!VAR "RoutineInfoList" = "concat($RoutineInfoList, name(node:ref(DcmDspRoutineInfoRef)), '#')"!]
      [!ENDIF!]
    [!ENDIF!]
  [!ENDLOOP!][!//
};
[!ENDIF!]
[!//
[!VAR "seclevelList"="'#'"!][!//
[!VAR "seclevelcount" = "0"!][!//
[!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspRoutine/*"!][!//
  [!IF "DcmDspRoutineUsed = 'true'"!]
    [!IF "not(contains($seclevelList, concat('#', name(node:ref(DcmDspRoutineInfoRef)), '#')))"!]
      [!VAR "seclevelcount" = "$seclevelcount + count(node:ref(DcmDspRoutineInfoRef)/DcmDspRoutineAuthorization/DcmDspRoutineSecurityLevelRef/*)"!][!//
      [!VAR "seclevelList" = "concat($seclevelList, name(node:ref(DcmDspRoutineInfoRef)), '#')"!]
    [!ENDIF!]
  [!ENDIF!]
[!ENDLOOP!][!//
[!//
[!IF "node:exists(node:refs(DcmConfigSet/*[1]/DcmDsp/DcmDspRoutine/*/DcmDspRoutineInfoRef)/DcmDspRoutineAuthorization/DcmDspRoutineSecurityLevelRef/*)"!]
[!VAR "seclevelList"="'#'"!][!//

/* Array of security types for all routines */
STATIC CONST(Dcm_SecLevelType,DCM_CONST) Dcm_RoutineControl_SecurityLevel[[!"num:i($seclevelcount)"!]] =
{
  [!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspRoutine/*"!][!//
    [!IF "DcmDspRoutineUsed = 'true'"!]
      [!IF "not(contains($seclevelList, concat('#', name(node:ref(DcmDspRoutineInfoRef)), '#')))"!]
        [!VAR "seclevelcount" = "$seclevelcount + 1"!][!//
        [!SELECT "node:ref(DcmDspRoutineInfoRef)"!][!//
          [!LOOP "DcmDspRoutineAuthorization/DcmDspRoutineSecurityLevelRef/*"!][!//
            [!WS "2"!][!"num:inttohex(node:ref(.)/DcmDspSecurityLevel, 2)"!]U,
          [!ENDLOOP!][!//
        [!ENDSELECT!][!//
        [!VAR "seclevelList" = "concat($seclevelList, name(node:ref(DcmDspRoutineInfoRef)), '#')"!]
      [!ENDIF!]
    [!ENDIF!]
  [!ENDLOOP!][!//
};
[!ENDIF!]
[!//

#define DCM_STOP_SEC_CONST_8
#include <Dcm_MemMap.h>

[!ENDIF!][!//
[!//
/*==================[external constants]=========================================================*/

#define DCM_START_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

/* Configuration parameters for 'Routine Control' service */
CONST(Dcm_DspRoutineServicesType, DCM_CONST) Dcm_DspRoutineConfig[DCM_NUM_ROUTINES] =
{
[!INDENT "2"!]
[!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspRoutine/*"!][!//
  [!IF "DcmDspRoutineUsed = 'true'"!]
    {
    [!INDENT "4"!]
      /* Pointer to request service handler function */
      [!IF "DcmDspRequestResultsRoutineSupported = 'true'"!][!//
        &Dcm_[!"node:name(.)"!]_RequestResults,
      [!ELSE!][!//
        NULL_PTR,
      [!ENDIF!][!//
      /* Pointer to start service handler function */
      [!IF "not(node:exists(DcmDspRoutineTidRef))"!]
        &Dcm_[!"node:name(.)"!]_Start,
      [!ELSE!][!//
        NULL_PTR,
      [!ENDIF!][!//
      /* Pointer to stop service handler function */
      [!IF "DcmDspStopRoutineSupported = 'true'"!]
        &Dcm_[!"node:name(.)"!]_Stop,
      [!ELSE!][!//
        NULL_PTR,
      [!ENDIF!][!//
      [!VAR "currentconfig" = "name(as:ref(DcmDspRoutineInfoRef))"!][!//
      [!VAR "routineinfoindex" = "0"!][!//
      [!VAR "routineInfoRefList"="'#'"!][!//
      [!LOOP "../*"!][!//
        [!IF "DcmDspRoutineUsed = 'true'"!]
          [!IF "name(as:ref(DcmDspRoutineInfoRef)) = $currentconfig"!]
            [!BREAK!]
          [!ENDIF!]
          [!IF "not(contains($routineInfoRefList, concat('#', name(node:ref(DcmDspRoutineInfoRef)), '#')))"!]
            [!VAR "routineinfoindex" = "$routineinfoindex + 1"!][!//
            [!VAR "routineInfoRefList" = "concat($routineInfoRefList, name(node:ref(DcmDspRoutineInfoRef)), '#')"!]
          [!ENDIF!]
        [!ENDIF!]
      [!ENDLOOP!][!//
      /* Pointer to assigned RoutineInfo configuration */
      &Dcm_DspRoutineInfoConfig[[!"num:integer($routineinfoindex)"!]],
[!IF "$DCM_MODEDECLARATION_SUPPORT = 'STD_ON'"!]
      /* Routine mode rule */
      [!IF "node:exists(node:ref(DcmDspRoutineInfoRef)/DcmDspRoutineAuthorization/DcmDspRoutineModeRuleRef)"!]
        &Dcm_ModeRule_[!"name(node:ref(node:ref(DcmDspRoutineInfoRef)/DcmDspRoutineAuthorization/DcmDspRoutineModeRuleRef))"!]_Result,
      [!ELSE!]
        NULL_PTR,
      [!ENDIF!]
[!ENDIF!][!// DCM_MODEDECLARATION_SUPPORT
      /* RID Identifier */
      [!"num:inttohex(DcmDspRoutineIdentifier)"!]U,
    [!ENDINDENT!]
    },
  [!ENDIF!][!//
[!ENDLOOP!][!//
[!ENDINDENT!]
};

/* Contains the configuration (parameters) for Routine's Info */
[!VAR "routineinfocount" = "0"!][!//
[!VAR "routineInfoRefList"="'#'"!][!//
[!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspRoutine/*"!][!//
  [!IF "DcmDspRoutineUsed = 'true'"!]
    [!IF "not(contains($routineInfoRefList, concat('#', name(node:ref(DcmDspRoutineInfoRef)), '#')))"!]
      [!VAR "routineinfocount" = "$routineinfocount + 1"!][!//
      [!VAR "routineInfoRefList" = "concat($routineInfoRefList, name(node:ref(DcmDspRoutineInfoRef)), '#')"!]
    [!ENDIF!]
  [!ENDIF!]
[!ENDLOOP!][!//
[!//
[!VAR "routineInfoRefList"="'#'"!][!//
[!VAR "SesLevelPos" = "0"!][!//
[!VAR "SecLevelPos" = "0"!][!//
CONST(Dcm_DspRoutineServicesInfoType, DCM_CONST) Dcm_DspRoutineInfoConfig[[!"num:integer( $routineinfocount)"!]] =
{
  [!INDENT "2"!]
  [!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspRoutine/*"!][!//
    [!IF "DcmDspRoutineUsed = 'true'"!]
      [!IF "not(contains($routineInfoRefList, concat('#', name(node:ref(DcmDspRoutineInfoRef)), '#')))"!]
        [!SELECT "node:ref(DcmDspRoutineInfoRef)"!][!//
          {
          [!INDENT "4"!]
          [!VAR "NumSecLevels" = "num:integer(count(DcmDspRoutineAuthorization/DcmDspRoutineSecurityLevelRef/*))"!][!//
          [!VAR "NumSesLevels" = "num:integer(count(DcmDspRoutineAuthorization/DcmDspRoutineSessionRef/*))"!][!//
            /* Pointer to security levels */
            [!IF "$NumSecLevels > 0"!][!//
              &(Dcm_RoutineControl_SecurityLevel[[!"num:integer( $SecLevelPos)"!]]),
            [!ELSE!][!//
              NULL_PTR,
            [!ENDIF!]
            /* Pointer to sessions */
            [!IF "$NumSesLevels > 0"!][!//
              &(Dcm_RoutineControl_SessionTypes[[!"num:integer( $SesLevelPos)"!]]),
            [!ELSE!][!//
              NULL_PTR,
            [!ENDIF!]
            /* Number of sessions */
            [!"$NumSesLevels"!]U,
            /* Number of security levels */
            [!"$NumSecLevels"!]U
          [!VAR "SesLevelPos" = "$SesLevelPos + $NumSesLevels"!][!//
          [!VAR "SecLevelPos" = "$SecLevelPos + $NumSecLevels"!][!//
          [!ENDINDENT!]
          },
        [!ENDSELECT!][!//
        [!VAR "routineInfoRefList" = "concat($routineInfoRefList, name(node:ref(DcmDspRoutineInfoRef)), '#')"!]
      [!ENDIF!]
    [!ENDIF!]
  [!ENDLOOP!][!//
  [!ENDINDENT!]
};

#define DCM_STOP_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

[!//
[!ENDIF!]
/*==================[external data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

/*==================[end of file]================================================================*/
