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
#ifndef DCM_API_CFG_H
#define DCM_API_CFG_H

/*==================[includes]===================================================================*/

[!AUTOSPACING!]
#include <Dcm_Types.h>

/*==================[macros]=====================================================================*/
[!IF "DcmConfigSet/*[1]/DcmGeneral/DcmServiceAPI/DcmDefaultASRServiceAPI = 'AUTOSAR_42'"!][!//
  [!VAR "ASRVersion" = "'DCM_USE_AUTOSAR_42'"!][!//
[!ELSE!][!//
  [!VAR "ASRVersion" = "'DCM_USE_AUTOSAR_40'"!][!//
[!ENDIF!][!//

[!/*--------------- Macros for user-implemented service handler IDs -------------*/!][!//
[!VAR "genMacroSvc" = "''"!][!//
[!LOOP "DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*"!][!//
  [!VAR "macroNameSvc" = "text:toupper(num:inttohex(DcmDsdSidTabServiceId, 2))"!][!//
  [!IF "node:exists(DcmDsdSidTabFnc) and not(contains($genMacroSvc, concat(':', $macroNameSvc)))"!][!//
    [!VAR "genMacroSvc" = "concat($genMacroSvc, ':', $macroNameSvc)"!][!//
#if (defined DCM_USERSVC_[!"$macroNameSvc"!])
  #error "DCM_USERSVC_[!"$macroNameSvc"!] is already defined"
#endif
#define DCM_USERSVC_[!"$macroNameSvc"!]

  [!ENDIF!][!//
[!ENDLOOP!][!//

[!/*--------------- Macros for user-implemented subservice handler IDs -------------*/!][!//
[!VAR "genMacroSSvc" = "''"!][!//
[!LOOP "DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*/DcmDsdSubService/*"!][!//
  [!VAR "macroNameSSvc" = "concat(text:toupper(num:inttohex(../../DcmDsdSidTabServiceId, 2)), '_', text:toupper(num:inttohex(DcmDsdSubServiceId, 2)))"!][!//
  [!IF "node:exists(DcmDsdSubServiceFnc) and not(contains($genMacroSSvc, concat(':', $macroNameSSvc)))"!][!//
    [!VAR "genMacroSSvc" = "concat($genMacroSSvc, ':', $macroNameSSvc)"!][!//
#if (defined DCM_USERSSVC_[!"$macroNameSSvc"!])
  #error "DCM_USERSSVC_[!"$macroNameSSvc"!] is already defined"
#endif
#define DCM_USERSSVC_[!"$macroNameSSvc"!]

  [!ENDIF!][!//
[!ENDLOOP!][!//

/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/

#define DCM_START_SEC_CALLOUT_CODE
#include <Dcm_MemMap.h>

[!INDENT "0"!][!//
[!/* Only enable if there is a service handler for OBD service 0x08 */!][!//
[!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId = num:i(8)])"!]
[!VAR "userfnc" = "':'"!][!//
[!/*-----------Extern declarations of user functions for the services 0x08 --------*/!][!//
/* !LINKSTO RequestControl.Callout,1 */
[!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspRequestControl/*[DcmDspRequestControlUsePort = 'USE_REQUESTCONTROL_SYNCH_FNC']"!][!//
  [!IF "not( contains( $userfnc, concat( ':', ./DcmDspRequestControlFnc, ':')))"!]
    extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspRequestControlFnc"!](
    [!WS "4"!] P2VAR(uint8, AUTOMATIC, DCM_VAR) OutBuffer,
    [!WS "4"!] P2CONST(uint8, AUTOMATIC, DCM_VAR) InBuffer);

    [!VAR "userfnc" = "concat( $userfnc, ./DcmDspRequestControlFnc, ':')"!][!//
  [!ENDIF!][!//
[!ENDLOOP!][!//
[!ENDIF!][!//
[!//
[!VAR "userfnc" = "':'"!][!//
[!/*---------------Extern declarations of user functions for the services 0x27 -------------*/!][!//
[!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspSecurity/DcmDspSecurityRow/*"!][!//
[!//
[!IF "(DcmDspSecurityUsePort  = 'USE_SECURITY_ASYNCH_FNC') or (DcmDspSecurityUsePort  = 'USE_SECURITY_SYNCH_FNC')"!][!//
  [!IF "node:exists(DcmDspSecurityGetSeedFnc)"!][!//
    [!IF "DcmDspSecurityADRSize > 0"!][!//
      [!/*--Extern declarations of user functions for 0x27 if DcmDspSecurityADRSize is present ---*/!][!//
      [!IF "DcmDspSecurityUsePort  = 'USE_SECURITY_ASYNCH_FNC'"!][!//
        [!IF "not( contains( $userfnc, concat( ':', ./DcmDspSecurityGetSeedFnc, ':')))"!]
          extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspSecurityGetSeedFnc"!](
          [!WS "4"!]P2CONST(uint8, AUTOMATIC, DCM_VAR) SecurityAccessDataRecord,
          [!WS "4"!]Dcm_OpStatusType OpStatus,
          [!WS "4"!]P2VAR(uint8, AUTOMATIC, DCM_VAR) Seed,
          [!WS "4"!]P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

          [!VAR "userfnc" = "concat( $userfnc, ./DcmDspSecurityGetSeedFnc, ':')"!][!//
        [!ENDIF!][!//
      [!ELSE!][!//
        [!IF "not( contains( $userfnc, concat( ':', ./DcmDspSecurityGetSeedFnc, ':')))"!]
          extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspSecurityGetSeedFnc"!](
          [!WS "4"!]P2CONST(uint8, AUTOMATIC, DCM_VAR) SecurityAccessDataRecord,
          [!WS "4"!]P2VAR(uint8, AUTOMATIC, DCM_VAR) Seed,
          [!WS "4"!]P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

          [!VAR "userfnc" = "concat( $userfnc, ./DcmDspSecurityGetSeedFnc, ':')"!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
    [!ELSE!][!//
      [!/* Extern declarations of user functions for 0x27 if DcmDspSecurityADRSize is not present */!][!//
      [!IF "DcmDspSecurityUsePort = 'USE_SECURITY_ASYNCH_FNC'"!][!//
        [!IF "not( contains( $userfnc, concat( ':', ./DcmDspSecurityGetSeedFnc, ':')))"!]
          extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspSecurityGetSeedFnc"!](
          [!WS "4"!]Dcm_OpStatusType OpStatus,
          [!WS "4"!]P2VAR(uint8, AUTOMATIC, DCM_VAR) Seed,
          [!WS "4"!]P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

          [!VAR "userfnc" = "concat( $userfnc, ./DcmDspSecurityGetSeedFnc, ':')"!][!//
        [!ENDIF!]
      [!ELSE!][!//
        [!IF "not( contains( $userfnc, concat( ':', ./DcmDspSecurityGetSeedFnc, ':')))"!]
          extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspSecurityGetSeedFnc"!](
          [!WS "4"!]P2VAR(uint8, AUTOMATIC, DCM_VAR) Seed,
          [!WS "4"!]P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

          [!VAR "userfnc" = "concat( $userfnc, ./DcmDspSecurityGetSeedFnc, ':')"!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
    [!ENDIF!]
  [!ENDIF!]
[!//
  /* !LINKSTO Dcm.EB.ServiceAPI.SecurityAccess,1 */
  [!IF "contains ($ASRVersion, 'AUTOSAR_42')"!][!//
    [!VAR "RteConnectedASRVersion" = "'DCM_USE_AUTOSAR_42'"!][!//
  [!ELSE!][!//
    [!VAR "RteConnectedASRVersion" = "'DCM_USE_AUTOSAR_40'"!][!//
  [!ENDIF!][!//

  [!IF "node:exists(DcmDspSecurityCompareKeyFnc)"!][!//
    [!IF "DcmDspSecurityUsePort  = 'USE_SECURITY_ASYNCH_FNC'"!][!//
      [!IF "not( contains( $userfnc, concat( ':', ./DcmDspSecurityCompareKeyFnc, ':')))"!][!//
        extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspSecurityCompareKeyFnc"!](
        [!IF "contains ($RteConnectedASRVersion, 'DCM_USE_AUTOSAR_42')"!][!//
          [!WS "4"!]P2CONST(uint8, AUTOMATIC, DCM_VAR) Key, Dcm_OpStatusType OpStatus,
          [!WS "4"!]P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

        [!ELSE!][!//
          [!WS "4"!]P2CONST(uint8, AUTOMATIC, DCM_VAR) Key, Dcm_OpStatusType OpStatus);

        [!ENDIF!][!//
        [!VAR "userfnc" = "concat( $userfnc, ./DcmDspSecurityCompareKeyFnc, ':')"!][!//
      [!ENDIF!][!//
    [!ELSE!][!//
      [!IF "not( contains( $userfnc, concat( ':', ./DcmDspSecurityCompareKeyFnc, ':')))"!][!//
        extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspSecurityCompareKeyFnc"!](
        [!IF "contains ($RteConnectedASRVersion, 'DCM_USE_AUTOSAR_42')"!][!//
          [!WS "4"!]P2CONST(uint8, AUTOMATIC, DCM_VAR) Key,
          [!WS "4"!]P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

        [!ELSE!][!//
          [!WS "4"!]P2CONST(uint8, AUTOMATIC, DCM_VAR) Key);

        [!ENDIF!][!//
        [!VAR "userfnc" = "concat( $userfnc, ./DcmDspSecurityCompareKeyFnc, ':')"!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
[!//
  [!IF "node:exists(DcmDspSecurityGetAttemptCounterFnc)"!][!//
    [!IF "DcmDspSecurityUsePort  = 'USE_SECURITY_ASYNCH_FNC'"!][!//
      extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspSecurityGetAttemptCounterFnc"!](
      [!WS "4"!]Dcm_OpStatusType OpStatus, P2VAR(uint8, AUTOMATIC, DCM_VAR) AttemptCounter);
    [!ELSE!][!//
      extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspSecurityGetAttemptCounterFnc"!](
      [!WS "4"!]P2VAR(uint8, AUTOMATIC, DCM_VAR) AttemptCounter);
    [!ENDIF!][!//
  [!ENDIF!][!//
[!//
  [!IF "node:exists(DcmDspSecuritySetAttemptCounterFnc)"!][!//
    [!IF "DcmDspSecurityUsePort  = 'USE_SECURITY_ASYNCH_FNC'"!][!//
      extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspSecuritySetAttemptCounterFnc"!](
      [!WS "4"!]Dcm_OpStatusType OpStatus, uint8 AttemptCounter);
    [!ELSE!][!//
      extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspSecuritySetAttemptCounterFnc"!](
      [!WS "4"!]uint8 AttemptCounter);
    [!ENDIF!][!//
  [!ENDIF!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//
[!//
[!VAR "userfnc" = "':'"!][!//
[!/*-----------Extern declarations of user functions for the services 0x22 and 0x2E --------*/!][!//
[!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspData/*"!][!//
  [!IF "node:exists(DcmDspDataConditionCheckReadFnc)"!][!//
    [!IF "DcmDspDataUsePort = 'USE_DATA_ASYNCH_FNC'"!][!//
      [!IF "not( contains( $userfnc, concat( ':', ./DcmDspDataConditionCheckReadFnc, ':')))"!][!//
        /* !LINKSTO Dcm.ReadDataById.ConditionCheckRead.AsyncAPI,1 */
        extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspDataConditionCheckReadFnc"!](
        [!WS "4"!]Dcm_OpStatusType OpStatus,
        [!WS "4"!]P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

        [!VAR "userfnc" = "concat( $userfnc, ./DcmDspDataConditionCheckReadFnc, ':')"!][!//
      [!ENDIF!][!//
    [!ELSE!][!//
      [!IF "DcmDspDataUsePort = 'USE_DATA_SYNCH_FNC'"!][!//
        [!IF "not( contains( $userfnc, concat( ':', ./DcmDspDataConditionCheckReadFnc, ':')))"!][!//
          /* !LINKSTO Dcm.ReadDataById.ConditionCheckRead.SyncAPI,1 */
          extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspDataConditionCheckReadFnc"!](
          [!WS "4"!]P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

          [!VAR "userfnc" = "concat( $userfnc, ./DcmDspDataConditionCheckReadFnc, ':')"!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
  [!IF "node:exists(DcmDspDataReadDataLengthFnc)"!][!//
    [!IF "$ASRVersion = 'DCM_USE_AUTOSAR_40' or
         ($ASRVersion = 'DCM_USE_AUTOSAR_42' and DcmDspDataUsePort = 'USE_DATA_SYNCH_FNC')"!][!//
      [!IF "not( contains( $userfnc, concat( ':', ./DcmDspDataReadDataLengthFnc, ':')))"!][!//
        [!/* !LINKSTO Dcm.ReadDataById.ReadDataLength.SyncAPI,1 */!][!//
        [!/* !LINKSTO Dcm.ReadDataById.ReadDataLength.AsyncAPI.ASR40,1 */!][!//
        extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspDataReadDataLengthFnc"!](
        [!WS "4"!]P2VAR(uint16, AUTOMATIC, DCM_VAR) DataLength);

        [!VAR "userfnc" = "concat( $userfnc, ./DcmDspDataReadDataLengthFnc, ':')"!][!//
      [!ENDIF!][!//
    [!ELSE!][!//
      [!IF "not( contains( $userfnc, concat( ':', ./DcmDspDataReadDataLengthFnc, ':')))"!][!//
        [!/* !LINKSTO Dcm.ReadDataById.ReadDataLength.AsyncAPI,1 */!][!//
        extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspDataReadDataLengthFnc"!](
        [!WS "4"!]Dcm_OpStatusType OpStatus,
        [!WS "4"!]P2VAR(uint16, AUTOMATIC, DCM_VAR) DataLength);

        [!VAR "userfnc" = "concat( $userfnc, ./DcmDspDataReadDataLengthFnc, ':')"!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
  [!IF "node:exists(DcmDspDataReadFnc)"!][!//
    [!IF "DcmDspDataUsePort = 'USE_DATA_ASYNCH_FNC'"!][!//
      [!IF "not( contains( $userfnc, concat( ':', ./DcmDspDataReadFnc, ':')))"!][!//
        /* !LINKSTO Dcm.ReadDataById.ReadData.AsyncAPI,1 */
        extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspDataReadFnc"!](
        [!WS "4"!]Dcm_OpStatusType OpStatus,
        [!WS "4"!]P2VAR(uint8, AUTOMATIC, DCM_VAR) Data);

        [!VAR "userfnc" = "concat( $userfnc, ./DcmDspDataReadFnc, ':')"!][!//
      [!ENDIF!][!//
    [!ELSE!][!//
      [!IF "not( contains( $userfnc, concat( ':', ./DcmDspDataReadFnc, ':')))"!][!//
        /* !LINKSTO Dcm.ReadDataById.ReadData.SyncAPI,1 */
        extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspDataReadFnc"!](
        [!WS "4"!]P2VAR(uint8, AUTOMATIC, DCM_VAR) Data);

        [!VAR "userfnc" = "concat( $userfnc, ./DcmDspDataReadFnc, ':')"!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
  [!IF "node:exists(DcmDspDataWriteFnc)"!][!//
    [!IF "DcmDspDataUsePort = 'USE_DATA_ASYNCH_FNC'"!][!//
      [!IF "not( contains( $userfnc, concat( ':', ./DcmDspDataWriteFnc, ':')))"!][!//
        [!IF "node:ref(DcmDspDataInfoRef)/DcmDspDataFixedLength = 'true'"!][!//
          /* !LINKSTO Dcm.WriteDataById.WriteData.FixLength.AsyncAPI,1 */
          extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspDataWriteFnc"!](
          [!WS "4"!]P2CONST(uint8, AUTOMATIC, DCM_VAR) Data,
          [!WS "4"!]Dcm_OpStatusType OpStatus,
          [!WS "4"!]P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

        [!ELSE!][!//
          /* !LINKSTO Dcm.WriteDataById.WriteData.VarLength.AsyncAPI,1 */
          extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspDataWriteFnc"!](
          [!WS "4"!]P2CONST(uint8, AUTOMATIC, DCM_VAR) Data,
          [!WS "4"!]uint16 DataLength,
          [!WS "4"!]Dcm_OpStatusType OpStatus,
          [!WS "4"!]P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

        [!ENDIF!][!//
          [!VAR "userfnc" = "concat( $userfnc, ./DcmDspDataWriteFnc, ':')"!][!//
      [!ENDIF!][!//
    [!ELSE!][!//
      [!IF "not( contains( $userfnc, concat( ':', ./DcmDspDataWriteFnc, ':')))"!][!//
        [!IF "node:ref(DcmDspDataInfoRef)/DcmDspDataFixedLength = 'true'"!][!//
          /* !LINKSTO Dcm.WriteDataById.WriteData.FixLength.SyncAPI,1 */
          extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspDataWriteFnc"!](
          [!WS "4"!]P2CONST(uint8, AUTOMATIC, DCM_VAR) Data,
          [!WS "4"!]P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

        [!ELSE!][!//
          /* !LINKSTO Dcm.WriteDataById.WriteData.VarLength.SyncAPI,1 */
          extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspDataWriteFnc"!](
          [!WS "4"!]P2CONST(uint8, AUTOMATIC, DCM_VAR) Data,
          [!WS "4"!]uint16 DataLength,
          [!WS "4"!]P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

        [!ENDIF!][!//
          [!VAR "userfnc" = "concat( $userfnc, ./DcmDspDataWriteFnc, ':')"!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
[!ENDLOOP!][!//
[!//
[!VAR "userfnc" = "':'"!][!//
[!/*-----------Extern declarations of user functions for the service 0x24 --------*/!][!//
[!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspData/*"!][!//
  [!IF "node:exists(DcmDspDataGetScalingInfoFnc)"!][!//
    [!IF "DcmDspDataUsePort = 'USE_DATA_SYNCH_FNC'"!][!//
      [!IF "not( contains( $userfnc, concat( ':', ./DcmDspDataGetScalingInfoFnc, ':')))"!][!//
          /* !LINKSTO Dcm.UDSServices.GetScalingInformation.SyncFunctionDefinition,1 */
          extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspDataGetScalingInfoFnc"!](
          [!WS "4"!]P2VAR(uint8, AUTOMATIC, DCM_VAR) ScalingInfo,
          [!WS "4"!]P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

        [!VAR "userfnc" = "concat( $userfnc, ./DcmDspDataGetScalingInfoFnc, ':')"!][!//
      [!ENDIF!][!//
    [!ELSE!][!//
      [!IF "DcmDspDataUsePort = 'USE_DATA_ASYNCH_FNC'"!][!//
        [!IF "not( contains( $userfnc, concat( ':', ./DcmDspDataGetScalingInfoFnc, ':')))"!][!//
        /* !LINKSTO Dcm.UDSServices.GetScalingInformation.AsyncFunctionDefinition,1 */
        extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspDataGetScalingInfoFnc"!](
        [!WS "4"!]Dcm_OpStatusType OpStatus,
        [!WS "4"!]P2VAR(uint8, AUTOMATIC, DCM_VAR) ScalingInfo,
        [!WS "4"!]P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

          [!VAR "userfnc" = "concat( $userfnc, ./DcmDspDataGetScalingInfoFnc, ':')"!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
[!ENDLOOP!][!//
[!//
[!VAR "userfnc" = "':'"!][!//
[!/*-----------Extern declarations of user functions for the services 0x2F --------*/!][!//
[!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspData/*"!][!//
[!//
  [!VAR "MaskExtern" = "'FALSE'"!][!//
  [!VAR "MaskType" = "''"!][!//
  [!VAR "nodeName" = "node:name(.)"!][!//
  [!IF "count(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*/DcmDspDidSignal/*[node:name(node:ref(DcmDspDidDataRef)) = $nodeName]) > 0"!][!//
    [!IF "node:exists(node:ref(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*/DcmDspDidSignal/*[node:name(node:ref(DcmDspDidDataRef)) = $nodeName][1]/../../DcmDspDidInfoRef)/DcmDspDidAccess/DcmDspDidControl/DcmDspDidControlMask) and
         (node:ref(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*/DcmDspDidSignal/*[node:name(node:ref(DcmDspDidDataRef)) = $nodeName][1]/../../DcmDspDidInfoRef)/DcmDspDidAccess/DcmDspDidControl/DcmDspDidControlMask = 'DCM_CONTROLMASK_EXTERNAL')"!][!//
      [!VAR "MaskExtern" = "'TRUE'"!][!//
      [!VAR "MaskSize" = "num:i(node:ref(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*/DcmDspDidSignal/*[node:name(node:ref(DcmDspDidDataRef)) = $nodeName][1]/../../DcmDspDidInfoRef)/DcmDspDidAccess/DcmDspDidControl/DcmDspDidControlMaskSize)"!]
      [!IF "num:i($MaskSize) = num:i(1)"!][!//
        [!VAR "MaskType" = "'uint8'"!][!//
      [!ELSEIF "num:i($MaskSize) = num:i(2)"!][!//
        [!VAR "MaskType" = "'uint16'"!][!//
      [!ELSEIF "num:i($MaskSize) = num:i(3)"!][!//
        [!VAR "MaskType" = "'uint32'"!][!//
      [!ELSE!][!//
        [!VAR "MaskType" = "'uint32'"!][!//
      [!ENDIF!]
    [!ENDIF!]
  [!ENDIF!][!//
  [!IF "node:exists(DcmDspDataFreezeCurrentStateFnc)"!][!//
    [!IF "DcmDspDataUsePort = 'USE_DATA_ASYNCH_FNC'"!][!//
      [!IF "not( contains( $userfnc, concat( ':', ./DcmDspDataFreezeCurrentStateFnc, ':')))"!][!//
        /* !LINKSTO Dcm.FreezeCurrentState.AsyncFnc,1 */
        extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspDataFreezeCurrentStateFnc"!](
        [!WS "4"!]Dcm_OpStatusType OpStatus,
        [!WS "4"!]P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

        [!VAR "userfnc" = "concat( $userfnc, ./DcmDspDataFreezeCurrentStateFnc, ':')"!][!//
      [!ENDIF!][!//
    [!ELSEIF "DcmDspDataUsePort = 'USE_DATA_SYNCH_FNC'"!][!//
      [!IF "not( contains( $userfnc, concat( ':', ./DcmDspDataFreezeCurrentStateFnc, ':')))"!][!//
        /* !LINKSTO Dcm.FreezeCurrentState.SyncFnc.NoControlMaskConfigured,1 */
        /* !LINKSTO Dcm.FreezeCurrentState.SyncFnc.ControlMaskExternal.MaskSize01,1 */
        /* !LINKSTO Dcm.FreezeCurrentState.SyncFnc.ControlMaskExternal.MaskSize02,1 */
        /* !LINKSTO Dcm.FreezeCurrentState.SyncFnc.ControlMaskExternal.MaskSize03,1 */
        /* !LINKSTO Dcm.FreezeCurrentState.SyncFnc.ControlMaskExternal.MaskSize04,1 */
        extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspDataFreezeCurrentStateFnc"!](
        [!IF "$MaskExtern = 'TRUE'"!]
          [!WS "4"!][!"$MaskType"!] ControlMask,
        [!ENDIF!]
        [!WS "4"!]P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
        [!VAR "userfnc" = "concat( $userfnc, ./DcmDspDataFreezeCurrentStateFnc, ':')"!][!//
      [!ENDIF!][!//
    [!ELSE!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
[!//
  [!IF "node:exists(DcmDspDataShortTermAdjustmentFnc)"!][!//
    [!IF "DcmDspDataUsePort = 'USE_DATA_ASYNCH_FNC'"!][!//
      [!IF "not( contains( $userfnc, concat( ':', ./DcmDspDataShortTermAdjustmentFnc, ':')))"!][!//
        [!IF "node:ref(DcmDspDataInfoRef)/DcmDspDataFixedLength = 'true'"!][!//
          /* !LINKSTO Dcm.ShortTermAdjustment.DcmDspDataFixedLength.True.AsyncFnc,1 */
          extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspDataShortTermAdjustmentFnc"!](
          [!WS "4"!]P2CONST(uint8, AUTOMATIC, DCM_VAR) Data,
          [!WS "4"!]Dcm_OpStatusType OpStatus,
          [!WS "4"!]P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

        [!ELSE!][!//
          /* !LINKSTO Dcm.ShortTermAdjustment.DcmDspDataFixedLength.False.AsyncFnc,1 */
          extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspDataShortTermAdjustmentFnc"!](
          [!WS "4"!]P2CONST(uint8, AUTOMATIC, DCM_VAR) Data,
          [!WS "4"!]uint16 DataLength,
          [!WS "4"!]Dcm_OpStatusType OpStatus,
          [!WS "4"!]P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

        [!ENDIF!][!//
        [!VAR "userfnc" = "concat( $userfnc, ./DcmDspDataShortTermAdjustmentFnc, ':')"!][!//
      [!ENDIF!][!//
    [!ELSEIF "DcmDspDataUsePort = 'USE_DATA_SYNCH_FNC'"!][!//
      [!IF "not( contains( $userfnc, concat( ':', ./DcmDspDataShortTermAdjustmentFnc, ':')))"!][!//
        [!IF "node:ref(DcmDspDataInfoRef)/DcmDspDataFixedLength = 'true'"!][!//
          /* !LINKSTO Dcm.ShortTermAdjustment.SyncFnc.FixedLength.NoControlMaskConfigured,1 */
          /* !LINKSTO Dcm.ShortTermAdjustment.SyncFnc.FixedLength.ControlMaskExternal.MaskSize01,1 */
          /* !LINKSTO Dcm.ShortTermAdjustment.SyncFnc.FixedLength.ControlMaskExternal.MaskSize02,1 */
          /* !LINKSTO Dcm.ShortTermAdjustment.SyncFnc.FixedLength.ControlMaskExternal.MaskSize03,1 */
          /* !LINKSTO Dcm.ShortTermAdjustment.SyncFnc.FixedLength.ControlMaskExternal.MaskSize04,1 */
          extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspDataShortTermAdjustmentFnc"!](
          [!WS "4"!]P2CONST(uint8, AUTOMATIC, DCM_VAR) Data,
          [!IF "$MaskExtern = 'TRUE'"!]
            [!WS "4"!][!"$MaskType"!] ControlMask,
          [!ENDIF!]
          [!WS "4"!]P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
        [!ELSE!][!//
          /* !LINKSTO Dcm.ShortTermAdjustment.SyncFnc.VariableLength.NoControlMaskConfigured,1 */
          /* !LINKSTO Dcm.ShortTermAdjustment.SyncFnc.VariableLength.ControlMaskExternal.MaskSize01,1 */
          /* !LINKSTO Dcm.ShortTermAdjustment.SyncFnc.VariableLength.ControlMaskExternal.MaskSize02,1 */
          /* !LINKSTO Dcm.ShortTermAdjustment.SyncFnc.VariableLength.ControlMaskExternal.MaskSize03,1 */
          /* !LINKSTO Dcm.ShortTermAdjustment.SyncFnc.VariableLength.ControlMaskExternal.MaskSize04,1 */
          extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspDataShortTermAdjustmentFnc"!](
          [!WS "4"!]P2CONST(uint8, AUTOMATIC, DCM_VAR) Data,
          [!WS "4"!]uint16 DataLength,
          [!IF "$MaskExtern = 'TRUE'"!]
            [!WS "4"!][!"$MaskType"!] ControlMask,
          [!ENDIF!]
          [!WS "4"!]P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
        [!ENDIF!][!//
        [!VAR "userfnc" = "concat( $userfnc, ./DcmDspDataShortTermAdjustmentFnc, ':')"!][!//
      [!ENDIF!][!//
    [!ELSE!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
[!//
  [!IF "node:exists(DcmDspDataResetToDefaultFnc)"!][!//
    [!IF "DcmDspDataUsePort = 'USE_DATA_ASYNCH_FNC'"!][!//
      [!IF "not( contains( $userfnc, concat( ':', ./DcmDspDataResetToDefaultFnc, ':')))"!]
        /* !LINKSTO Dcm.ResetToDefault.AsyncFnc,1 */
        extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspDataResetToDefaultFnc"!](
        [!WS "4"!]Dcm_OpStatusType OpStatus,
        [!WS "4"!]P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

        [!VAR "userfnc" = "concat( $userfnc, ./DcmDspDataResetToDefaultFnc, ':')"!][!//
      [!ENDIF!][!//
    [!ELSEIF "DcmDspDataUsePort = 'USE_DATA_SYNCH_FNC'"!][!//
      [!IF "not( contains( $userfnc, concat( ':', ./DcmDspDataResetToDefaultFnc, ':')))"!][!//
        /* !LINKSTO Dcm.ResetToDefault.SyncFnc.NoControlMaskConfigured,1 */
        /* !LINKSTO Dcm.ResetToDefault.SyncFnc.ControlMaskExternal.MaskSize01,1 */
        /* !LINKSTO Dcm.ResetToDefault.SyncFnc.ControlMaskExternal.MaskSize02,1 */
        /* !LINKSTO Dcm.ResetToDefault.SyncFnc.ControlMaskExternal.MaskSize03,1 */
        /* !LINKSTO Dcm.ResetToDefault.SyncFnc.ControlMaskExternal.MaskSize04,1 */
        extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspDataResetToDefaultFnc"!](
        [!IF "$MaskExtern = 'TRUE'"!]
          [!WS "4"!][!"$MaskType"!] ControlMask,
        [!ENDIF!]
        [!WS "4"!]P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

        [!VAR "userfnc" = "concat( $userfnc, ./DcmDspDataResetToDefaultFnc, ':')"!][!//
      [!ENDIF!][!//
    [!ELSE!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
[!//
  [!IF "node:exists(DcmDspDataReturnControlToEcuFnc)"!][!//
    [!IF "DcmDspDataUsePort = 'USE_DATA_ASYNCH_FNC'"!][!//
      [!IF "not( contains( $userfnc, concat( ':', ./DcmDspDataReturnControlToEcuFnc, ':')))"!][!//
        /* !LINKSTO Dcm.ReturnControlToECU.AsyncFnc,1 */
        extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspDataReturnControlToEcuFnc"!](
        [!WS "4"!]Dcm_OpStatusType OpStatus,
        [!WS "4"!]P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

        [!VAR "userfnc" = "concat( $userfnc, ./DcmDspDataReturnControlToEcuFnc, ':')"!][!//
      [!ENDIF!][!//
    [!ELSEIF "DcmDspDataUsePort = 'USE_DATA_SYNCH_FNC'"!][!//
      [!IF "not( contains( $userfnc, concat( ':', ./DcmDspDataReturnControlToEcuFnc, ':')))"!][!//
        /* !LINKSTO Dcm.ReturnControlToECU.SyncFnc.NoControlMaskConfigured,1 */
        /* !LINKSTO Dcm.ReturnControlToECU.SyncFnc.ControlMaskExternal.MaskSize01,1 */
        /* !LINKSTO Dcm.ReturnControlToECU.SyncFnc.ControlMaskExternal.MaskSize02,1 */
        /* !LINKSTO Dcm.ReturnControlToECU.SyncFnc.ControlMaskExternal.MaskSize03,1 */
        /* !LINKSTO Dcm.ReturnControlToECU.SyncFnc.ControlMaskExternal.MaskSize04,1 */
        extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspDataReturnControlToEcuFnc"!](
        [!IF "$MaskExtern = 'TRUE'"!]
          [!WS "4"!][!"$MaskType"!] ControlMask,
        [!ENDIF!]
        [!WS "4"!]P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
        [!VAR "userfnc" = "concat( $userfnc, ./DcmDspDataReturnControlToEcuFnc, ':')"!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
[!ENDLOOP!][!//
[!//
[!/*--------------- Extern declarations of user functions for the DID Ranges --------------*/!][!//
[!VAR "userfnc" = "':'"!][!//
[!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspDidRange/*"!][!//
  [!IF "node:exists(DcmDspDidRangeIsDidAvailableFnc)"!][!//
    [!IF "not(contains($userfnc, concat(':', DcmDspDidRangeIsDidAvailableFnc, ':')))"!][!//
      /* !LINKSTO SWS_Dcm_00803,1 */
      extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspDidRangeIsDidAvailableFnc"!](
      [!INDENT "4"!]
        uint16 DID,
        Dcm_OpStatusType OpStatus,
        P2VAR(Dcm_DidSupportedType, AUTOMATIC, DCM_VAR) supported);
      [!ENDINDENT!]

      [!VAR "userfnc" = "concat($userfnc, DcmDspDidRangeIsDidAvailableFnc,':')"!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
  [!IF "node:exists(DcmDspDidRangeReadDidFnc)"!][!//
    [!IF "not(contains($userfnc, concat(':', DcmDspDidRangeReadDidFnc, ':')))"!][!//
      /* !LINKSTO SWS_Dcm_00804,1 */
      extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspDidRangeReadDidFnc"!](
      [!INDENT "4"!]
        uint16 DID,
        P2VAR(uint8, AUTOMATIC, DCM_VAR) Data,
        Dcm_OpStatusType OpStatus,
        P2VAR(uint16, AUTOMATIC, DCM_VAR) DataLength,
        P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
      [!ENDINDENT!]

      [!VAR "userfnc" = "concat($userfnc, DcmDspDidRangeReadDidFnc,':')"!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
  [!IF "node:exists(DcmDspDidRangeWriteDidFnc)"!][!//
    [!IF "not(contains($userfnc, concat(':', DcmDspDidRangeWriteDidFnc, ':')))"!][!//
      /* !LINKSTO SWS_Dcm_00805,1 */
      extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"DcmDspDidRangeWriteDidFnc"!](
      [!INDENT "4"!]
        uint16 DID,
        P2CONST(uint8, AUTOMATIC, DCM_VAR) Data,
        Dcm_OpStatusType OpStatus,
        uint16 DataLength,
        P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
      [!ENDINDENT!]

      [!VAR "userfnc" = "concat($userfnc, DcmDspDidRangeWriteDidFnc,':')"!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
[!ENDLOOP!][!//
[!//
[!/*------- begin : Extern declarations of user functions for the service 0x31 ----------------*/!][!//
[!/* Difine a macro to split elements from string */!][!//
[!MACRO "SPLIT_ELEM_LIST", "indent", "paramList"!]
[!INDENT "$indent"!]
  [!FOR "iterator" = "1" TO "count(text:split($paramList, '#'))"!]
    [!"text:split($paramList, '#')[num:i($iterator)]"!]
  [!ENDFOR!]
[!ENDINDENT!]
[!ENDMACRO!]
[!/* Define a macro to decrease size of source code */!][!//
[!MACRO "ROUTINECONTROL_GENERATE_PROTOTYPE", "DcmDspOperationRoutineFnc", "inParamList", "outParamList", "RoutineVariableInSignals", "RoutineVariableOutSignals"!]
extern Std_ReturnType [!"$DcmDspOperationRoutineFnc"!](
[!INDENT "4"!]
  [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineFixedLength,1 */!][!//
  [!/* !LINKSTO Dcm.EB.RoutineControl.AUTOSAR40.Interface,1 */!][!//
  [!/* !LINKSTO Dcm.EB.RoutineControl.AUTOSAR42.Interface,1 */!][!//
  [!/* !LINKSTO Dcm.EB.RoutineControl.Default40.Interface,1 */!][!//
  [!/* !LINKSTO Dcm.EB.RoutineControl.Default42.Interface,1 */!][!//
  [!/* !LINKSTO Dcm.EB.RoutineControl.Disabled40.Interface,1 */!][!//
  [!/* !LINKSTO Dcm.EB.RoutineControl.Disabled42.Interface,1 */!][!//
  [!IF "(node:exists(DcmDspRoutineUsePortASRVersion) and
         ((DcmDspRoutineUsePortASRVersion = 'AUTOSAR_42') or
          ((DcmDspRoutineUsePortASRVersion = 'Default') and
           (as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmGeneral/DcmServiceAPI/DcmDefaultASRServiceAPI = 'AUTOSAR_42')
          )
         )
        )
        or
        (
          not(node:exists(DcmDspRoutineUsePortASRVersion))
          and (as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmGeneral/DcmServiceAPI/DcmDefaultASRServiceAPI = 'AUTOSAR_42')
        )"!][!//
    [!IF "($RoutineVariableOutSignals = 'true')"!][!//
      [!CALL "SPLIT_ELEM_LIST", "indent"="4", "paramList"="$inParamList"!]Dcm_OpStatusType OpStatus,
      [!CALL "SPLIT_ELEM_LIST", "indent"="4", "paramList"="$outParamList"!]P2VAR(uint16, AUTOMATIC, DCM_VAR) currentDataLength,
      P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
    [!ELSE!][!//
      [!IF "($RoutineVariableInSignals = 'true')"!][!//
        [!CALL "SPLIT_ELEM_LIST", "indent"="4", "paramList"="$inParamList"!]Dcm_OpStatusType OpStatus,
        [!CALL "SPLIT_ELEM_LIST", "indent"="4", "paramList"="$outParamList"!]uint16 currentDataLength,
        P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
      [!ELSE!][!//
        [!CALL "SPLIT_ELEM_LIST", "indent"="4", "paramList"="$inParamList"!]Dcm_OpStatusType OpStatus,
        [!CALL "SPLIT_ELEM_LIST", "indent"="4", "paramList"="$outParamList"!]P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
      [!ENDIF!][!//
    [!ENDIF!][!//
  [!ELSE!][!//
    [!IF "(DcmDspRoutineFixedLength='false')"!]
      [!CALL "SPLIT_ELEM_LIST", "indent"="4", "paramList"="$inParamList"!]Dcm_OpStatusType OpStatus,
      [!CALL "SPLIT_ELEM_LIST", "indent"="4", "paramList"="$outParamList"!]P2VAR(uint16, AUTOMATIC, DCM_VAR) currentDataLength,
      P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
    [!ELSE!][!//
      [!CALL "SPLIT_ELEM_LIST", "indent"="4", "paramList"="$inParamList"!]Dcm_OpStatusType OpStatus,
      [!CALL "SPLIT_ELEM_LIST", "indent"="4", "paramList"="$outParamList"!]P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
    [!ENDIF!][!//
  [!ENDIF!][!//
[!ENDINDENT!]
[!ENDMACRO!][!//

[!/* Mainly source code to achieve external declarations of user functions for the routine control */!][!//
[!VAR "userfnc" = "':'"!][!//
[!LOOP "as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsp/DcmDspRoutine/*"!][!//
  [!IF "DcmDspRoutineUsed = 'true' and not(node:exists(DcmDspRoutineTidRef))"!][!//
    [!IF "DcmDspRoutineUsePort = 'false'"!][!//
[!/* for StartRoutineControl functions */!][!//
      [!VAR "inVarCount"="0"!][!//
      [!VAR "inParamListStart"="''"!][!//
      [!LOOP "node:ref(DcmDspRoutineInfoRef)/DcmDspStartRoutineIn/DcmDspStartRoutineInSignal/*"!][!//
        [!IF "contains(DcmDspRoutineSignalType, '_N')"!]
          [!VAR "inParamListStart"="concat($inParamListStart, 'P2CONST(', text:tolower(substring-before(DcmDspRoutineSignalType,'_N')),', AUTOMATIC, DCM_VAR)',' inParam', num:i($inVarCount), ',#')"!][!//
        [!ELSE!][!//
          [!VAR "inParamListStart"="concat($inParamListStart, text:tolower(DcmDspRoutineSignalType),' inParam', num:i($inVarCount), ',#')"!][!//
        [!ENDIF!][!//
        [!VAR "inVarCount"="$inVarCount + 1"!][!//
      [!ENDLOOP!][!//
      [!VAR "inParamListStart"="text:replaceAll( $inParamListStart, 'variable_length', 'P2CONST(uint8, AUTOMATIC, DCM_VAR)')"!][!//
      [!VAR "outVarCount"="0"!][!//
      [!VAR "outParamListStart"="''"!][!//
      [!LOOP "node:ref(DcmDspRoutineInfoRef)/DcmDspStartRoutineOut/DcmDspStartRoutineOutSignal/*"!][!//
        [!IF "contains(DcmDspRoutineSignalType, '_N')"!]
          [!VAR "outParamListStart"="concat($outParamListStart, 'P2VAR(', text:tolower(substring-before(DcmDspRoutineSignalType,'_N')),', AUTOMATIC, DCM_VAR)', ' outParam', num:i($outVarCount), ',#')"!][!//
        [!ELSE!][!//
          [!VAR "outParamListStart"="concat($outParamListStart, 'P2VAR(', text:tolower(DcmDspRoutineSignalType),', AUTOMATIC, DCM_VAR)', ' outParam', num:i($outVarCount), ',#')"!][!//
        [!ENDIF!][!//
        [!VAR "outVarCount"="$outVarCount + 1"!][!//
      [!ENDLOOP!][!//
      [!VAR "outParamListStart"="text:replaceAll( $outParamListStart, 'variable_length', 'uint8 ')"!][!//
      [!IF "not( contains( $userfnc, concat( ':', ./DcmDspStartRoutineFnc, ':')))"!][!//
        [!VAR "userfnc" = "concat( $userfnc, ./DcmDspStartRoutineFnc, ':')"!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStart.Operation,1 */!][!//
        [!/* !LINKSTO Dcm.RoutineServices.ASR43.CalloutStart.FixedIN.VarOUT,1 */!][!//
        [!/* !LINKSTO Dcm.RoutineServices.ASR43.CalloutStart.VarIN.FixedOUT,1 */!][!//
        [!/* !LINKSTO Dcm.RoutineServices.ASR43.CalloutStart.FixedIN.FixedOUT,1 */!][!//
        [!/* !LINKSTO Dcm.RoutineServices.ASR43.CalloutStart.VarIN.VarOUT,1 */!][!//
        [!/* !LINKSTO Dcm.RoutineServices.ASR40.CalloutStart.DcmDspRoutineFixedLen.False.FixedIN.VarOUT,1 */!][!//
        [!/* !LINKSTO Dcm.RoutineServices.ASR40.CalloutStart.DcmDspRoutineFixedLen.False.VarIN.FixedOUT,1 */!][!//
        [!/* !LINKSTO Dcm.RoutineServices.ASR40.CalloutStart.DcmDspRoutineFixedLen.False.FixedIN.FixedOUT,1 */!][!//
        [!/* !LINKSTO Dcm.RoutineServices.ASR40.CalloutStart.DcmDspRoutineFixedLen.False.VarIN.VarOUT,1 */!][!//
        [!/* !LINKSTO Dcm.RoutineServices.ASR40.CalloutStart.DcmDspRoutineFixedLen.True,1 */!][!//
        [!CALL "ROUTINECONTROL_GENERATE_PROTOTYPE", "DcmDspOperationRoutineFnc" = "DcmDspStartRoutineFnc", "inParamList" = "$inParamListStart", "outParamList" = "$outParamListStart",
          "RoutineVariableInSignals" = "(node:exists(node:ref(DcmDspRoutineInfoRef)/DcmDspStartRoutineIn/DcmDspStartRoutineInSignal/*[(DcmDspRoutineSignalType = 'VARIABLE_LENGTH')]))",
          "RoutineVariableOutSignals" = "(node:exists(node:ref(DcmDspRoutineInfoRef)/DcmDspStartRoutineOut/DcmDspStartRoutineOutSignal/*[(DcmDspRoutineSignalType = 'VARIABLE_LENGTH')]))"!]
      [!ENDIF!][!//
[!/* for StopRoutineControl functions */!][!//
      [!VAR "inVarCount"="0"!][!//
      [!VAR "inParamListStop"="''"!][!//
      [!LOOP "node:ref(DcmDspRoutineInfoRef)/DcmDspRoutineStopIn/DcmDspRoutineStopInSignal/*"!][!//
        [!IF "contains(DcmDspRoutineSignalType, '_N')"!]
          [!VAR "inParamListStop"="concat($inParamListStop, 'P2CONST(', text:tolower(substring-before(DcmDspRoutineSignalType,'_N')),', AUTOMATIC, DCM_VAR)',' inParam', num:i($inVarCount), ',#')"!][!//
        [!ELSE!][!//
          [!VAR "inParamListStop"="concat($inParamListStop, text:tolower(DcmDspRoutineSignalType),' inParam', num:i($inVarCount), ',#')"!][!//
        [!ENDIF!][!//
        [!VAR "inVarCount"="$inVarCount + 1"!][!//
      [!ENDLOOP!][!//
      [!VAR "inParamListStop"="text:replaceAll( $inParamListStop, 'variable_length', 'P2CONST(uint8, AUTOMATIC, DCM_VAR)')"!][!//
      [!VAR "outVarCount"="0"!][!//
      [!VAR "outParamListStop"="''"!][!//
      [!LOOP "node:ref(DcmDspRoutineInfoRef)/DcmDspRoutineStopOut/DcmDspRoutineStopOutSignal/*"!][!//
        [!IF "contains(DcmDspRoutineSignalType, '_N')"!]
          [!VAR "outParamListStop"="concat($outParamListStop, 'P2VAR(', text:tolower(substring-before(DcmDspRoutineSignalType,'_N')),', AUTOMATIC, DCM_VAR)', ' outParam', num:i($outVarCount), ',#')"!][!//
        [!ELSE!][!//
          [!VAR "outParamListStop"="concat($outParamListStop, 'P2VAR(', text:tolower(DcmDspRoutineSignalType),', AUTOMATIC, DCM_VAR)', ' outParam', num:i($outVarCount), ',#')"!][!//
        [!ENDIF!][!//
        [!VAR "outVarCount"="$outVarCount + 1"!][!//
      [!ENDLOOP!][!//
      [!VAR "outParamListStop"="text:replaceAll( $outParamListStop, 'variable_length', 'uint8 ')"!][!//
      [!IF "(node:exists(DcmDspStopRoutineFnc)) and (not( contains( $userfnc, concat( ':', ./DcmDspStopRoutineFnc, ':'))))"!][!//
        [!VAR "userfnc" = "concat( $userfnc, ./DcmDspStopRoutineFnc, ':')"!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStop.Operation,1 */!][!//
        [!/* !LINKSTO Dcm.RoutineServices.ASR43.CalloutStop.FixedIN.VarOUT,1 */!][!//
        [!/* !LINKSTO Dcm.RoutineServices.ASR43.CalloutStop.VarIN.FixedOUT,1 */!][!//
        [!/* !LINKSTO Dcm.RoutineServices.ASR43.CalloutStop.FixedIN.FixedOUT,1 */!][!//
        [!/* !LINKSTO Dcm.RoutineServices.ASR43.CalloutStop.VarIN.VarOUT,1 */!][!//
        [!/* !LINKSTO Dcm.RoutineServices.ASR40.CalloutStop.DcmDspRoutineFixedLen.False.FixedIN.VarOUT,1 */!][!//
        [!/* !LINKSTO Dcm.RoutineServices.ASR40.CalloutStop.DcmDspRoutineFixedLen.False.VarIN.FixedOUT,1 */!][!//
        [!/* !LINKSTO Dcm.RoutineServices.ASR40.CalloutStop.DcmDspRoutineFixedLen.False.FixedIN.FixedOUT,1 */!][!//
        [!/* !LINKSTO Dcm.RoutineServices.ASR40.CalloutStop.DcmDspRoutineFixedLen.False.VarIN.VarOUT,1 */!][!//
        [!/* !LINKSTO Dcm.RoutineServices.ASR40.CalloutStop.DcmDspRoutineFixedLen.True,1 */!][!//
        [!CALL "ROUTINECONTROL_GENERATE_PROTOTYPE", "DcmDspOperationRoutineFnc" = "DcmDspStopRoutineFnc", "inParamList" = "$inParamListStop", "outParamList" = "$outParamListStop",
          "RoutineVariableInSignals" = "(node:exists(node:ref(DcmDspRoutineInfoRef)/DcmDspRoutineStopIn/DcmDspRoutineStopInSignal/*[(DcmDspRoutineSignalType = 'VARIABLE_LENGTH')]))",
          "RoutineVariableOutSignals" = "(node:exists(node:ref(DcmDspRoutineInfoRef)/DcmDspRoutineStopOut/DcmDspRoutineStopOutSignal/*[(DcmDspRoutineSignalType = 'VARIABLE_LENGTH')]))"!]
      [!ENDIF!][!//
[!/* for ResultsRoutineControl functions */!][!//
      [!VAR "inVarCount"="0"!][!//
      [!VAR "inParamListRequestRes"="''"!][!//
      [!LOOP "node:ref(DcmDspRoutineInfoRef)/DcmDspRoutineRequestResIn/DcmDspRoutineRequestResInSignal/*"!][!//
        [!IF "contains(DcmDspRoutineSignalType, '_N')"!]
          [!VAR "inParamListRequestRes"="concat($inParamListRequestRes, 'P2CONST(', text:tolower(substring-before(DcmDspRoutineSignalType,'_N')),', AUTOMATIC, DCM_VAR)',' inParam', num:i($inVarCount), ',#')"!][!//
        [!ELSE!][!//
          [!VAR "inParamListRequestRes"="concat($inParamListRequestRes, text:tolower(DcmDspRoutineSignalType),' inParam', num:i($inVarCount), ',#')"!][!//
        [!ENDIF!][!//
        [!VAR "inVarCount"="$inVarCount + 1"!][!//
      [!ENDLOOP!][!//
      [!VAR "inParamListRequestRes"="text:replaceAll( $inParamListRequestRes, 'variable_length', 'P2CONST(uint8, AUTOMATIC, DCM_VAR)')"!][!//
      [!VAR "outVarCount"="0"!][!//
      [!VAR "outParamListRequestRes"="''"!][!//
      [!LOOP "node:ref(DcmDspRoutineInfoRef)/DcmDspRoutineRequestResOut/DcmDspRoutineRequestResOutSignal/*"!][!//
        [!IF "contains(DcmDspRoutineSignalType, '_N')"!]
          [!VAR "outParamListRequestRes"="concat($outParamListRequestRes, 'P2VAR(', text:tolower(substring-before(DcmDspRoutineSignalType,'_N')),', AUTOMATIC, DCM_VAR)', ' outParam', num:i($outVarCount), ',#')"!][!//
        [!ELSE!][!//
          [!VAR "outParamListRequestRes"="concat($outParamListRequestRes, 'P2VAR(', text:tolower(DcmDspRoutineSignalType),', AUTOMATIC, DCM_VAR)', ' outParam', num:i($outVarCount), ',#')"!][!//
        [!ENDIF!][!//
        [!VAR "outVarCount"="$outVarCount + 1"!][!//
      [!ENDLOOP!][!//
      [!VAR "outParamListRequestRes"="text:replaceAll( $outParamListRequestRes, 'variable_length', 'uint8 ')"!][!//
      [!IF "(node:exists(DcmDspRequestResultsRoutineFnc)) and (not( contains( $userfnc, concat( ':', ./DcmDspRequestResultsRoutineFnc, ':'))))"!][!//
        [!VAR "userfnc" = "concat( $userfnc, ./DcmDspRequestResultsRoutineFnc, ':')"!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResults.Operation,1 */!][!//
        [!/* !LINKSTO Dcm.RoutineServices.ASR43.CalloutRequestResults.FixedIN.VarOUT,1 */!][!//
        [!/* !LINKSTO Dcm.RoutineServices.ASR43.CalloutRequestResults.VarIN.FixedOUT,1 */!][!//
        [!/* !LINKSTO Dcm.RoutineServices.ASR43.CalloutRequestResults.FixedIN.FixedOUT,1 */!][!//
        [!/* !LINKSTO Dcm.RoutineServices.ASR43.CalloutRequestResults.VarIN.VarOUT,1 */!][!//
        [!/* !LINKSTO Dcm.RoutineServices.ASR40.CalloutRequestResults.DcmDspRoutineFixedLen.False.FixedIN.VarOUT,1 */!][!//
        [!/* !LINKSTO Dcm.RoutineServices.ASR40.CalloutRequestResults.DcmDspRoutineFixedLen.False.VarIN.FixedOUT,1 */!][!//
        [!/* !LINKSTO Dcm.RoutineServices.ASR40.CalloutRequestResults.DcmDspRoutineFixedLen.False.FixedIN.FixedOUT,1 */!][!//
        [!/* !LINKSTO Dcm.RoutineServices.ASR40.CalloutRequestResults.DcmDspRoutineFixedLen.False.VarIN.VarOUT,1 */!][!//
        [!/* !LINKSTO Dcm.RoutineServices.ASR40.CalloutRequestResults.DcmDspRoutineFixedLen.True,1 */!][!//
        [!/* !LINKSTO Dcm.RoutineServices.ASR40.CalloutRequestResults.DcmDspRoutineFixedLen.False.NoIN.FixedOUT,1 */!][!//
        [!/* !LINKSTO Dcm.RoutineServices.ASR40.CalloutRequestResults.DcmDspRoutineFixedLen.False.NoIN.VarOUT,1 */!][!//
        [!CALL "ROUTINECONTROL_GENERATE_PROTOTYPE", "DcmDspOperationRoutineFnc" = "DcmDspRequestResultsRoutineFnc", "inParamList" = "$inParamListRequestRes", "outParamList" = "$outParamListRequestRes",
          "RoutineVariableInSignals" = "(node:exists(node:ref(DcmDspRoutineInfoRef)/DcmDspRoutineRequestResIn/DcmDspRoutineRequestResInSignal/*[(DcmDspRoutineSignalType = 'VARIABLE_LENGTH')]))",
          "RoutineVariableOutSignals" = "(node:exists(node:ref(DcmDspRoutineInfoRef)/DcmDspRoutineRequestResOut/DcmDspRoutineRequestResOutSignal/*[(DcmDspRoutineSignalType = 'VARIABLE_LENGTH')]))"!]
      [!ENDIF!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
[!ENDLOOP!][!//
[!/*------- end : Extern declarations of user functions for the service 0x31 ----------------*/!][!//
[!//
/* !LINKSTO Dcm.ReadData.DcmDspPid.SyncFnc,1 */
[!/*------------External declarations of user functions for the OBD service 0x01 -------*/!][!//
[!VAR "DcmDspPidDataReadFncCfgList" = "'#'"!][!//
[!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspPid/*[(DcmDspPidService='DCM_SERVICE_01') or (DcmDspPidService='DCM_SERVICE_01_02')]/DcmDspPidData/*[DcmDspPidService01/DcmDspPidDataUsePort = 'USE_DATA_SYNCH_FNC']"!][!//
[!VAR "DcmDspPidDataReadFncCfg" = "DcmDspPidService01/DcmDspPidDataReadFnc"!][!//
[!IF "not(contains($DcmDspPidDataReadFncCfgList, $DcmDspPidDataReadFncCfg))"!][!//
[!VAR "DcmDspPidDataReadFncCfgList" = "concat($DcmDspPidDataReadFncCfgList, $DcmDspPidDataReadFncCfg)"!][!//
/** Extern DcmDspPidDataReadFnc [!"$DcmDspPidDataReadFncCfg"!] */
extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"$DcmDspPidDataReadFncCfg"!](
[!INDENT "4"!]
  P2VAR(uint8, AUTOMATIC, DCM_VAR) Data);
[!ENDINDENT!]

[!ENDIF!][!//
[!ENDLOOP!][!//
[!//
[!/*------------External declarations of user functions for the OBD service 0x09 -------*/!][!//
[!VAR "DcmDspVehInfoDataReadFncCfgList" = "'#'"!][!//
[!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspVehInfo/*/DcmDspVehInfoData/*[DcmDspVehInfoDataUsePort = 'false']"!][!//
[!VAR "DcmDspVehInfoDataReadFncCfg" = "DcmDspVehInfoDataReadFnc"!][!//
[!IF "not(contains($DcmDspVehInfoDataReadFncCfgList, $DcmDspVehInfoDataReadFncCfg))"!][!//
[!VAR "DcmDspVehInfoDataReadFncCfgList" = "concat($DcmDspVehInfoDataReadFncCfgList, $DcmDspVehInfoDataReadFncCfg)"!][!//
/** Extern DcmDspVehInfoDataReadFnc [!"$DcmDspVehInfoDataReadFncCfg"!] */
[!IF "as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmGeneral/DcmDspVehInfoASRVersion = 'AUTOSAR_440'"!]
extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"$DcmDspVehInfoDataReadFncCfg"!](
[!INDENT "4"!]
  Dcm_OpStatusType OpStatus,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) DataValueBuffer,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) DataValueBufferSize);
[!ENDINDENT!]
[!ELSE!]
extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"$DcmDspVehInfoDataReadFncCfg"!](
[!INDENT "4"!]
  Dcm_OpStatusType OpStatus,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) DataValueBuffer);
[!ENDINDENT!]
[!ENDIF!]

[!ENDIF!][!//
[!ENDLOOP!][!//
[!//
[!/*------------External declarations of user service functions ----------------*/!][!//
[!VAR "SidFuncList" = "':'"!][!//
[!LOOP "DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*"!][!//
[!IF "node:exists(DcmDsdSidTabFnc)"!][!//
[!IF "not( contains( $SidFuncList, concat( ':', ./DcmDsdSidTabFnc, ':')))"!][!//
extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"./DcmDsdSidTabFnc"!](
[!INDENT "4"!]
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
[!ENDINDENT!]

[!VAR "SidFuncList" = "concat( $SidFuncList, ./DcmDsdSidTabFnc, ':')"!][!//
[!ENDIF!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//
[!//
[!/*------------External declarations of user subservice functions ----------------*/!][!//
[!VAR "SubidFuncList" = "':'"!][!//
[!LOOP "DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*/DcmDsdSubService/*"!][!//
[!IF "node:exists(DcmDsdSubServiceFnc)"!][!//
[!IF "not( contains( $SubidFuncList, concat( ':', ./DcmDsdSubServiceFnc, ':')))"!][!//
extern FUNC(Std_ReturnType, DCM_APPL_CODE) [!"./DcmDsdSubServiceFnc"!](
[!INDENT "4"!]
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
[!ENDINDENT!]

[!VAR "SubidFuncList" = "concat( $SubidFuncList, ./DcmDsdSubServiceFnc, ':')"!][!//
[!ENDIF!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//
[!//
[!ENDINDENT!]
#define DCM_STOP_SEC_CALLOUT_CODE
#include <Dcm_MemMap.h>
/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /* ifndef DCM_API_CFG_H */
/*==================[end of file]================================================================*/
