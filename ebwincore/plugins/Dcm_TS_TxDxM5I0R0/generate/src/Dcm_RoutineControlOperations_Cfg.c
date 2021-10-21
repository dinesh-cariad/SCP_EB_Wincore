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

[!INCLUDE "../include/Dcm_common.m"!][!//

/* !LINKSTO Dcm.Dsn.File.RoutineControlOperations.ConfigC,1 */
[!IF "($DCM_DSP_USE_SERVICE_0X31 = 'STD_ON')"!]

 /* MISRA-C:2004 Deviation List
  *
  * MISRA-1) Deviated Rule: 11.4 (advisory)
  *     A cast should not be performed between a pointer to object type and a
  *     different pointer to object type.
  *
  *     Reason:
  *     A cast to unsigned is necessary here to copy data bit by bit and
  *     there is no alignment issues.
  *
  */

 /* MISRA-C:2012 Deviation List
  *
  *  MISRAC2012-1) Deviated Rule: 11.3 (required)
  *     "A cast should not be performed between a pointer to object type and a different pointer to
  *     object type."
  *
  *     Reason:
  *     A cast to UINT8 is necessary here to copy data bit by bit and to be flexible for all
  *     the types, being no alignment issues.
  *     There is no side effects.
  *
  *  MISRAC2012-2) Deviated Rule: 8.9 (advisory)
  *     "An object should be defined at block scope if its identifier only
  *     appears in a single function."
  *
  *     Reason:
  *     The object is defined in this way because the values that are stored
  *     in them are required for the next call of this function and should be
  *     hold in the data segment memory.
  *
  */

[!ENDIF!][!//
[!//
[!AUTOSPACING!][!//
[!//
[!VAR "BytesSwapNeededU16" = "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspRoutine/*[DcmDspRoutineUsed = 'true' and not(node:exists(DcmDspRoutineTidRef)) and ((node:ref(DcmDspRoutineInfoRef)/*[contains(name(.), 'Start')]/*/*[contains(./DcmDspRoutineSignalType,'UINT16')]/*) or (DcmDspStopRoutineSupported = 'true' and (node:ref(DcmDspRoutineInfoRef)/*[contains(name(.), 'Stop')]/*/*[contains(./DcmDspRoutineSignalType,'UINT16')]/*)) or (DcmDspRequestResultsRoutineSupported = 'true' and (node:ref(DcmDspRoutineInfoRef)/*[contains(name(.), 'RequestRes')]/*/*[contains(./DcmDspRoutineSignalType,'UINT16')]/*)))])"!]
[!VAR "BytesSwapNeededS16" = "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspRoutine/*[DcmDspRoutineUsed = 'true' and not(node:exists(DcmDspRoutineTidRef)) and ((node:ref(DcmDspRoutineInfoRef)/*[contains(name(.), 'Start')]/*/*[contains(./DcmDspRoutineSignalType,'SINT16')]/*) or (DcmDspStopRoutineSupported = 'true' and (node:ref(DcmDspRoutineInfoRef)/*[contains(name(.), 'Stop')]/*/*[contains(./DcmDspRoutineSignalType,'SINT16')]/*)) or (DcmDspRequestResultsRoutineSupported = 'true' and (node:ref(DcmDspRoutineInfoRef)/*[contains(name(.), 'RequestRes')]/*/*[contains(./DcmDspRoutineSignalType,'SINT16')]/*)))])"!]
[!VAR "BytesSwapNeededU32" = "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspRoutine/*[DcmDspRoutineUsed = 'true' and not(node:exists(DcmDspRoutineTidRef)) and ((node:ref(DcmDspRoutineInfoRef)/*[contains(name(.), 'Start')]/*/*[contains(./DcmDspRoutineSignalType,'UINT32')]/*) or (DcmDspStopRoutineSupported = 'true' and (node:ref(DcmDspRoutineInfoRef)/*[contains(name(.), 'Stop')]/*/*[contains(./DcmDspRoutineSignalType,'UINT32')]/*)) or (DcmDspRequestResultsRoutineSupported = 'true' and (node:ref(DcmDspRoutineInfoRef)/*[contains(name(.), 'RequestRes')]/*/*[contains(./DcmDspRoutineSignalType,'UINT32')]/*)))])"!]
[!VAR "BytesSwapNeededS32" = "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspRoutine/*[DcmDspRoutineUsed = 'true' and not(node:exists(DcmDspRoutineTidRef)) and ((node:ref(DcmDspRoutineInfoRef)/*[contains(name(.), 'Start')]/*/*[contains(./DcmDspRoutineSignalType,'SINT32')]/*) or (DcmDspStopRoutineSupported = 'true' and (node:ref(DcmDspRoutineInfoRef)/*[contains(name(.), 'Stop')]/*/*[contains(./DcmDspRoutineSignalType,'SINT32')]/*)) or (DcmDspRequestResultsRoutineSupported = 'true' and (node:ref(DcmDspRoutineInfoRef)/*[contains(name(.), 'RequestRes')]/*/*[contains(./DcmDspRoutineSignalType,'SINT32')]/*)))])"!]
[!VAR "resetOfSignalArray" = "'false'"!][!//
[!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspRoutine/*"!][!//
  [!IF "DcmDspRoutineUsed = 'true' and not(node:exists(DcmDspRoutineTidRef))"!][!//
    [!IF "(node:exists(node:ref(./DcmDspRoutineInfoRef)/DcmDspStartRoutineIn/DcmDspStartRoutineInSignal/*[./DcmDspRoutineSignalType != 'VARIABLE_LENGTH'])) or (node:exists(node:ref(./DcmDspRoutineInfoRef)/DcmDspStartRoutineOut/DcmDspStartRoutineOutSignal/*[./DcmDspRoutineSignalType != 'VARIABLE_LENGTH']))"!]
      [!VAR "resetOfSignalArray" = "'true'"!][!//
      [!BREAK!][!//
    [!ENDIF!][!//
    [!IF "DcmDspStopRoutineSupported = 'true'"!]
      [!IF "(node:exists(node:ref(./DcmDspRoutineInfoRef)/DcmDspRoutineStopIn/DcmDspRoutineStopInSignal/*[./DcmDspRoutineSignalType != 'VARIABLE_LENGTH'])) or (node:exists(node:ref(./DcmDspRoutineInfoRef)/DcmDspRoutineStopOut/DcmDspRoutineStopOutSignal/*[./DcmDspRoutineSignalType != 'VARIABLE_LENGTH']))"!]
        [!VAR "resetOfSignalArray" = "'true'"!][!//
        [!BREAK!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
    [!IF "DcmDspRequestResultsRoutineSupported = 'true'"!][!//
      [!IF "(node:exists(node:ref(./DcmDspRoutineInfoRef)/DcmDspRoutineRequestResIn/DcmDspRoutineRequestResInSignal/*[./DcmDspRoutineSignalType != 'VARIABLE_LENGTH'])) or (node:exists(node:ref(./DcmDspRoutineInfoRef)/DcmDspRoutineRequestResOut/DcmDspRoutineRequestResOutSignal/*[./DcmDspRoutineSignalType != 'VARIABLE_LENGTH']))"!]
        [!VAR "resetOfSignalArray" = "'true'"!][!//
        [!BREAK!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
[!ENDLOOP!][!//
[!//
[!/*--- Generation macro for RoutineControl operation ---*/!][!//
[!/* !LINKSTO Dcm.Dsn.IB.RoutineControl.RoutineMacroGeneration,1 */!][!//
[!MACRO "ROUTINECONTROL_GENERATE", "operation"!]
[!IF "$operation = 'Start'"!][!//
  [!VAR "routineInput" = "'DcmDspStartRoutineIn'"!][!//
  [!VAR "routineOutput" = "'DcmDspStartRoutineOut'"!][!//
  [!VAR "inputSignal" = "'DcmDspStartRoutineInSignal'"!][!//
  [!VAR "outputSignal" = "'DcmDspStartRoutineOutSignal'"!][!//
  [!VAR "SVCID" = "'DCM_SVCID_ROUTINECONTROLSTART'"!]
[!ELSEIF "$operation = 'Stop'"!][!//
  [!VAR "routineInput" = "'DcmDspRoutineStopIn'"!][!//
  [!VAR "routineOutput" = "'DcmDspRoutineStopOut'"!][!//
  [!VAR "inputSignal" = "'DcmDspRoutineStopInSignal'"!][!//
  [!VAR "outputSignal" = "'DcmDspRoutineStopOutSignal'"!][!//
  [!VAR "SVCID" = "'DCM_SVCID_ROUTINECONTROLSTOP'"!]
[!ELSEIF "$operation = 'RequestResults'"!][!//
  [!VAR "routineInput" = "'DcmDspRoutineRequestResIn'"!][!//
  [!VAR "routineOutput" = "'DcmDspRoutineRequestResOut'"!][!//
  [!VAR "inputSignal" = "'DcmDspRoutineRequestResInSignal'"!][!//
  [!VAR "outputSignal" = "'DcmDspRoutineRequestResOutSignal'"!][!//
  [!VAR "SVCID" = "'DCM_SVCID_ROUTINECONTROLROUTINERESULTS'"!]
[!ELSE!][!//
  [!ERROR "Macro ROUTINECONTROL_GENERATE called with bad arguments"!][!//
[!ENDIF!][!//
[!//
[!VAR "inputSignalNr" = "count(node:ref(DcmDspRoutineInfoRef)/*[name(.) = $routineInput]/*[name(.) = $inputSignal]/*)"!][!//
[!VAR "outputSignalNr" = "count(node:ref(DcmDspRoutineInfoRef)/*[name(.) = $routineOutput]/*[name(.) = $outputSignal]/*)"!][!//
[!VAR "inputVarSignalNr" = "count(node:ref(DcmDspRoutineInfoRef)/*[name(.) = $routineInput]/*[name(.) = $inputSignal]/*[(DcmDspRoutineSignalType = 'VARIABLE_LENGTH')])"!][!//
[!VAR "outputVarSignalNr" = "count(node:ref(DcmDspRoutineInfoRef)/*[name(.) = $routineOutput]/*[name(.) = $outputSignal]/*[(DcmDspRoutineSignalType = 'VARIABLE_LENGTH')])"!][!//
[!VAR "DcmRoutineVarLenInBytes" = "as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsp/DcmDspRoutineVariableLengthInBytes"!][!//
[!//
[!IF "not(node:refexists(./DcmDspRoutineTidRef))"!][!//
[!CR!]
FUNC(Std_ReturnType, DCM_CODE) Dcm_[!"node:name(.)"!]_[!"$operation"!](
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode)
{
  [!INDENT "2"!][!//
    [!VAR "RoutineFixedLength" = "'false'"!][!//
    [!VAR "RoutineVariableInSignals" = "'false'"!][!//
    [!VAR "RoutineVariableOutSignals" = "'false'"!][!//
    [!VAR "ResponseLengthWithoutCurrentDataLength" = "'false'"!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineFixedLength,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.AUTOSAR40.Interface,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.AUTOSAR42.Interface,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.Default40.Interface,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.Default42.Interface,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.Disabled40.Interface,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.Disabled42.Interface,1 */!][!//
    [!IF " (node:exists(DcmDspRoutineUsePortASRVersion) and
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
      [!VAR "RoutineFixedLength" = "'true'"!][!//
      [!IF "((node:refvalid(DcmDspRoutineInfoRef)) and ($inputVarSignalNr > 0))"!][!//
        [!VAR "RoutineFixedLength" = "'false'"!][!//
        [!VAR "RoutineVariableInSignals" = "'true'"!][!//
      [!ENDIF!][!//
      [!IF "((node:refvalid(DcmDspRoutineInfoRef)) and ($outputVarSignalNr > 0))"!][!//
        [!VAR "RoutineFixedLength" = "'false'"!][!//
        [!VAR "RoutineVariableOutSignals" = "'true'"!][!//
      [!ENDIF!][!//
      [!IF "($RoutineVariableInSignals = 'true') and ($RoutineVariableOutSignals = 'false')"!][!//
        [!VAR "ResponseLengthWithoutCurrentDataLength" = "'true'"!][!//
      [!ENDIF!][!//
    [!ELSE!][!//
      [!IF "DcmDspRoutineFixedLength = 'true'"!][!//
        [!VAR "RoutineFixedLength" = "'true'"!][!//
        [!VAR "RoutineVariableInSignals" = "'false'"!][!//
        [!VAR "RoutineVariableOutSignals" = "'false'"!][!//
      [!ELSE!][!//
        [!VAR "RoutineFixedLength" = "'false'"!][!//
        [!VAR "RoutineVariableInSignals" = "'true'"!][!//
        [!VAR "RoutineVariableOutSignals" = "'true'"!][!//
        [!IF "($outputVarSignalNr = 0)"!][!//
          [!VAR "ResponseLengthWithoutCurrentDataLength" = "'true'"!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
    [!ENDIF!]
  [!//
  [!/* !LINKSTO SWS_Dcm_00641,1 */!][!//
  [!VAR "endiannessconversionrequired"="0"!][!//
  [!IF "($inputSignalNr > 0 or $outputSignalNr > 0)"!][!//
    [!IF "($inputSignalNr > 0)"!][!//
      [!LOOP "node:ref(DcmDspRoutineInfoRef)/*[name(.) = $routineInput]/*[name(.) = $inputSignal]/*"!][!//
        [!IF "contains(DcmDspRoutineSignalType, '16') or contains(DcmDspRoutineSignalType, '32')"!][!//
          [!VAR "endiannessconversionrequired"="1"!][!//
          [!BREAK!][!//
        [!ENDIF!][!//
      [!ENDLOOP!][!//
    [!ENDIF!][!//
  [!//
    [!IF "$endiannessconversionrequired != 1"!][!//
      [!IF "($outputSignalNr > 0)"!][!//
      [!LOOP "node:ref(DcmDspRoutineInfoRef)/*[name(.) = $routineOutput]/*[name(.) = $outputSignal]/*"!][!//
        [!IF "contains(DcmDspRoutineSignalType, '16') or contains(DcmDspRoutineSignalType, '32')"!][!//
            [!VAR "endiannessconversionrequired"="1"!][!//
            [!BREAK!][!//
          [!ENDIF!][!//
        [!ENDLOOP!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
  [!//
    Std_ReturnType ServiceProcessStatus = DCM_E_NOT_OK;
  [!IF "$RoutineFixedLength = 'false'"!][!//
    /* Variable length input/output signal length */
    uint16 CurrentDataLength = 0U;
  [!ENDIF!][!//
  [!IF "($inputSignalNr > 0 or $outputSignalNr > 0)"!][!//
    [!IF "$endiannessconversionrequired = 1"!][!//
      /* Parameter to store the configuration for current protocol */
      Dcm_ProtocolConfigType CurProtocolConfig;
      [!CR!]
    [!ENDIF!][!//
  [!ENDIF!][!//
  [!//
  [!VAR "BooleanArrayCount"="0"!][!//
  [!VAR "Sint16ArrayCount"="0"!][!//
  [!VAR "Sint32ArrayCount"="0"!][!//
  [!VAR "Sint8ArrayCount"="0"!][!//
  [!VAR "Uint16ArrayCount"="0"!][!//
  [!VAR "Uint32ArrayCount"="0"!][!//
  [!VAR "Uint8ArrayCount"="0"!][!//
    /* Parameter to store the length of input data (2U indicates the 2-byte RoutineID) received */
    Dcm_MsgLenType InDataLength = pMsgContext->reqDataLen - 2U;
  [!CR!]
  [!/* --- Build a list with the configured input signals which will be later used to extract the input parameters for the routine function calls  --- */!][!//
  [!VAR "inTotalDataLength"="0"!][!//
  [!VAR "inFixedDataLength"="0"!][!//
  [!VAR "inParamList"="''"!][!//
  [!VAR "ArrayTypeSignalsList"="''"!][!//
  [!VAR "NumberOfArrayTypeSignals"="0"!][!//
  [!VAR "InVarSignalShiftPos"="0"!][!//
  [!//
  [!IF "($inputSignalNr > 0)"!][!//
    [!LOOP "node:order(node:ref(DcmDspRoutineInfoRef)/*[name(.) = $routineInput]/*[name(.) = $inputSignal]/*, 'DcmDspRoutineSignalPos')"!][!//
      [!VAR "inTotalDataLength"="DcmDspRoutineSignalPos"!][!//
      [!VAR "inFixedDataLength"="$inTotalDataLength "!][!//
        [!IF "DcmDspRoutineSignalType = 'BOOLEAN'"!][!//
        [!VAR "inParamList"="concat($inParamList, 'BooleanArray[', num:i($BooleanArrayCount), '], ')"!][!//
        [!VAR "BooleanArrayCount"="$BooleanArrayCount + 1"!][!//
        [!ELSEIF "DcmDspRoutineSignalType = 'SINT16'"!][!//
        [!VAR "inParamList"="concat($inParamList, 'Sint16Array[', num:i($Sint16ArrayCount), '], ')"!][!//
        [!VAR "Sint16ArrayCount"="$Sint16ArrayCount + 1"!][!//
        [!ELSEIF "DcmDspRoutineSignalType = 'SINT32'"!][!//
        [!VAR "inParamList"="concat($inParamList, 'Sint32Array[', num:i($Sint32ArrayCount), '], ')"!][!//
        [!VAR "Sint32ArrayCount"="$Sint32ArrayCount + 1"!][!//
        [!ELSEIF "DcmDspRoutineSignalType = 'SINT8'"!][!//
        [!VAR "inParamList"="concat($inParamList, 'Sint8Array[', num:i($Sint8ArrayCount), '], ')"!][!//
        [!VAR "Sint8ArrayCount"="$Sint8ArrayCount + 1"!][!//
        [!ELSEIF "DcmDspRoutineSignalType = 'UINT16'"!][!//
        [!VAR "inParamList"="concat($inParamList, 'Uint16Array[', num:i($Uint16ArrayCount), '], ')"!][!//
        [!VAR "Uint16ArrayCount"="$Uint16ArrayCount + 1"!][!//
        [!ELSEIF "DcmDspRoutineSignalType = 'UINT32'"!][!//
        [!VAR "inParamList"="concat($inParamList, 'Uint32Array[', num:i($Uint32ArrayCount), '], ')"!][!//
        [!VAR "Uint32ArrayCount"="$Uint32ArrayCount + 1"!][!//
        [!ELSEIF "DcmDspRoutineSignalType = 'UINT8'"!][!//
        [!VAR "inParamList"="concat($inParamList, 'Uint8Array[', num:i($Uint8ArrayCount), '], ')"!][!//
        [!VAR "Uint8ArrayCount"="$Uint8ArrayCount + 1"!][!//
      [!ELSEIF "DcmDspRoutineSignalType = 'SINT16_N'"!][!//
        [!VAR "inParamList"="concat($inParamList, '&Sint16Array[', num:i($Sint16ArrayCount), '], ')"!][!//
        [!VAR "Sint16ArrayCount"="$Sint16ArrayCount + (./DcmDspRoutineSignalLength div 16)"!][!//
      [!ELSEIF "DcmDspRoutineSignalType = 'SINT32_N'"!][!//
        [!VAR "inParamList"="concat($inParamList, '&Sint32Array[', num:i($Sint32ArrayCount), '], ')"!][!//
        [!VAR "Sint32ArrayCount"="$Sint32ArrayCount + (./DcmDspRoutineSignalLength div 32)"!][!//
      [!ELSEIF "DcmDspRoutineSignalType = 'SINT8_N'"!][!//
        [!VAR "inParamList"="concat($inParamList, '&Sint8Array[', num:i($Sint8ArrayCount), '], ')"!][!//
        [!VAR "Sint8ArrayCount"="$Sint8ArrayCount + (./DcmDspRoutineSignalLength div 8)"!][!//
      [!ELSEIF "DcmDspRoutineSignalType = 'UINT16_N'"!][!//
        [!VAR "inParamList"="concat($inParamList, '&Uint16Array[', num:i($Uint16ArrayCount), '], ')"!][!//
        [!VAR "Uint16ArrayCount"="$Uint16ArrayCount + (./DcmDspRoutineSignalLength div 16)"!][!//
      [!ELSEIF "DcmDspRoutineSignalType = 'UINT32_N'"!][!//
        [!VAR "inParamList"="concat($inParamList, '&Uint32Array[', num:i($Uint32ArrayCount), '], ')"!][!//
        [!VAR "Uint32ArrayCount"="$Uint32ArrayCount + (./DcmDspRoutineSignalLength div 32)"!][!//
      [!ELSEIF "DcmDspRoutineSignalType = 'UINT8_N'"!][!//
        [!VAR "inParamList"="concat($inParamList, '&Uint8Array[', num:i($Uint8ArrayCount), '], ')"!][!//
        [!VAR "Uint8ArrayCount"="$Uint8ArrayCount + (./DcmDspRoutineSignalLength div 8)"!][!//
      [!ELSE!]
        [!IF "((DcmDspRoutineSignalPos mod 8) = 0)"!][!//
          [!/* --- If Variable length signal is byte aligned --- */!][!//
          [!VAR "inParamList"="concat($inParamList, '&(pMsgContext->reqData[', num:i(2 + ($inTotalDataLength div 8)), ']), ')"!][!//
        [!ELSE!][!//
          [!/* --- If Variable length signal is not byte aligned --- */!][!//
          [!VAR "inParamList"="concat($inParamList, '&(pMsgContext->reqData[', num:i(2 + (($inTotalDataLength) div 8)+ 1), ']), ')"!][!//
          [!VAR "InVarSignalShiftPos"="$inFixedDataLength"!][!//
        [!ENDIF!][!//
        [!VAR "inTotalDataLength"="$inTotalDataLength + DcmDspRoutineSignalLength"!][!//
      [!ENDIF!][!//
      [!/* --- Adding length of last signal , if it is not variable length --- */!][!//
      [!IF "DcmDspRoutineSignalPos = num:max(../*/DcmDspRoutineSignalPos)"!][!//
        [!IF "not(DcmDspRoutineSignalType = 'VARIABLE_LENGTH')"!][!//
          [!VAR "inTotalDataLength"="$inTotalDataLength + DcmDspRoutineSignalLength"!][!//
          [!/* --- inFixedDataLength will be equal to the total data length --- */!][!//
          [!VAR "inFixedDataLength"="$inTotalDataLength "!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
    [!ENDLOOP!][!//
  [!ENDIF!][!//
  [!/* --- Build a list with the configured output signals which will be later used to extract the output parameters for the routine function calls  --- */!][!//
  [!VAR "outFixedDataLength"="0"!][!//
  [!VAR "outVarCount"="0"!][!//
  [!VAR "outParamList"="''"!][!//
  [!VAR "outParamListAddress"="''"!][!//
  [!VAR "outVariableDataLength"="0"!][!//
  [!//
  [!IF "($outputSignalNr > 0)"!][!//
    [!LOOP "node:order(node:ref(DcmDspRoutineInfoRef)/*[name(.) = $routineOutput]/*[name(.) = $outputSignal]/*
                          , 'DcmDspRoutineSignalPos')"!][!//
      [!VAR "outFixedDataLength"="DcmDspRoutineSignalPos"!][!//
      [!IF "DcmDspRoutineSignalType = 'BOOLEAN'"!][!//
        [!VAR "outParamList"="concat($outParamList, 'BooleanArray[', num:i($BooleanArrayCount), '], ')"!][!//
        [!VAR "outParamListAddress"="concat($outParamListAddress, '&BooleanArray[', num:i($BooleanArrayCount), '], ')"!][!//
        [!VAR "BooleanArrayCount"="$BooleanArrayCount + 1"!][!//
      [!ELSEIF "contains(DcmDspRoutineSignalType, 'SINT16')"!][!//
        [!VAR "outParamList"="concat($outParamList, 'Sint16Array[', num:i($Sint16ArrayCount), '], ')"!][!//
        [!VAR "outParamListAddress"="concat($outParamListAddress, '&Sint16Array[', num:i($Sint16ArrayCount), '], ')"!][!//
        [!IF "DcmDspRoutineSignalType = 'SINT16'"!][!//
          [!VAR "Sint16ArrayCount"="$Sint16ArrayCount + 1"!][!//
        [!ELSE!][!//
          [!VAR "Sint16ArrayCount"="$Sint16ArrayCount + (./DcmDspRoutineSignalLength div 16)"!][!//
        [!ENDIF!][!//
      [!ELSEIF "contains(DcmDspRoutineSignalType, 'SINT32')"!][!//
        [!VAR "outParamList"="concat($outParamList, 'Sint32Array[', num:i($Sint32ArrayCount), '], ')"!][!//
        [!VAR "outParamListAddress"="concat($outParamListAddress, '&Sint32Array[', num:i($Sint32ArrayCount), '], ')"!][!//
        [!IF "DcmDspRoutineSignalType = 'SINT32'"!][!//
        [!VAR "Sint32ArrayCount"="$Sint32ArrayCount + 1"!][!//
        [!ELSE!][!//
          [!VAR "Sint32ArrayCount"="$Sint32ArrayCount + (./DcmDspRoutineSignalLength div 32)"!][!//
        [!ENDIF!][!//
      [!ELSEIF "contains(DcmDspRoutineSignalType, 'SINT8')"!][!//
        [!VAR "outParamList"="concat($outParamList, 'Sint8Array[', num:i($Sint8ArrayCount), '], ')"!][!//
        [!VAR "outParamListAddress"="concat($outParamListAddress, '&Sint8Array[', num:i($Sint8ArrayCount), '], ')"!][!//
        [!IF "DcmDspRoutineSignalType = 'SINT8'"!][!//
          [!VAR "Sint8ArrayCount"="$Sint8ArrayCount + 1"!][!//
        [!ELSE!][!//
          [!VAR "Sint8ArrayCount"="$Sint8ArrayCount + (./DcmDspRoutineSignalLength div 8)"!][!//
        [!ENDIF!][!//
      [!ELSEIF "contains(DcmDspRoutineSignalType, 'UINT16')"!][!//
        [!VAR "outParamList"="concat($outParamList, 'Uint16Array[', num:i($Uint16ArrayCount), '], ')"!][!//
        [!VAR "outParamListAddress"="concat($outParamListAddress, '&Uint16Array[', num:i($Uint16ArrayCount), '], ')"!][!//
        [!IF "DcmDspRoutineSignalType = 'UINT16'"!][!//
          [!VAR "Uint16ArrayCount"="$Uint16ArrayCount + 1"!][!//
        [!ELSE!][!//
          [!VAR "Uint16ArrayCount"="$Uint16ArrayCount + (./DcmDspRoutineSignalLength div 16)"!][!//
        [!ENDIF!][!//
      [!ELSEIF "contains(DcmDspRoutineSignalType, 'UINT32')"!][!//
        [!VAR "outParamList"="concat($outParamList, 'Uint32Array[', num:i($Uint32ArrayCount), '], ')"!][!//
        [!VAR "outParamListAddress"="concat($outParamListAddress, '&Uint32Array[', num:i($Uint32ArrayCount), '], ')"!][!//
        [!IF "DcmDspRoutineSignalType = 'UINT32'"!][!//
          [!VAR "Uint32ArrayCount"="$Uint32ArrayCount + 1"!][!//
        [!ELSE!][!//
          [!VAR "Uint32ArrayCount"="$Uint32ArrayCount + (./DcmDspRoutineSignalLength div 32)"!][!//
        [!ENDIF!][!//
      [!ELSEIF "contains(DcmDspRoutineSignalType, 'UINT8')"!][!//
        [!VAR "outParamList"="concat($outParamList, 'Uint8Array[', num:i($Uint8ArrayCount), '], ')"!][!//
        [!VAR "outParamListAddress"="concat($outParamListAddress, '&Uint8Array[', num:i($Uint8ArrayCount), '], ')"!][!//
        [!IF "DcmDspRoutineSignalType = 'UINT8'"!][!//
          [!VAR "Uint8ArrayCount"="$Uint8ArrayCount + 1"!][!//
        [!ELSE!][!//
          [!VAR "Uint8ArrayCount"="$Uint8ArrayCount + (./DcmDspRoutineSignalLength div 8)"!][!//
        [!ENDIF!][!//
      [!ELSE!]
        [!VAR "outVariableDataLength"="DcmDspRoutineSignalLength"!][!//
        [!IF "$CommonBufferCount = 0"!][!//
          [!/* Rx and Tx buffer are different */!][!//
          [!IF "((DcmDspRoutineSignalPos mod 8) = 0)"!][!//
            [!/* Variable length signal is byte aligned */!][!//
            [!VAR "outParamListAddress"="concat($outParamListAddress, '&(pMsgContext->resData[', num:i(3 + ($outFixedDataLength div 8)), ']), ')"!][!//
          [!ELSE!][!//
            [!/* Variable length signal is not byte aligned */!][!//
            [!VAR "outParamListAddress"="concat($outParamListAddress, '&(pMsgContext->resData[', num:i(3 + ($outFixedDataLength div 8) + 1 ), ']), ')"!][!//
          [!ENDIF!][!//
        [!ELSE!][!//
          [!/* Rx and Tx buffer are same, using a separate buffer */!][!//
          [!VAR "outParamListAddress"="concat($outParamListAddress, '&(Dcm_RoutineControlBuffer[', num:i(0), ']), ')"!][!//
        [!ENDIF!][!//
        [!ENDIF!][!//
      [!IF "not(DcmDspRoutineSignalType = 'VARIABLE_LENGTH')"!][!//
        [!/* --- Adding length of last signal --- */!][!//
        [!IF "DcmDspRoutineSignalPos = num:max(../*/DcmDspRoutineSignalPos)"!][!//
        [!VAR "outFixedDataLength"="$outFixedDataLength + DcmDspRoutineSignalLength"!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
      [!VAR "outVarCount"="$outVarCount + 1"!][!//
    [!ENDLOOP!][!//
  [!ENDIF!][!//
  [!//
  [!IF "($inputSignalNr > 0 or $outputSignalNr > 0)"!][!//
    [!IF "$endiannessconversionrequired = 1"!][!//
      /* This is executed in the context of the SvcH and the protocol on which the request was made is
      [!WS "1"!]* surely started */
      (void) Dcm_Dsl_Supervisor_GetLatestExternalUDSProtocolConfig(&CurProtocolConfig);
      [!CR!]
    [!ENDIF!][!//
  [!ENDIF!][!//
  [!IF "$RoutineFixedLength = 'false' and ($inputVarSignalNr > 0)"!][!//
    [!IF "$DcmRoutineVarLenInBytes = 'false'"!][!//
    /* Total received length for the variable length input signal minus the fixed data length,
    [!WS "1"!]* in bits. */
    CurrentDataLength = (uint16)(((pMsgContext->reqDataLen) - 2U) * 8U) - [!"num:i($inFixedDataLength)"!]U;
    [!ELSE!][!//
    /* Total received length for the variable length input signal minus the fixed data length,
    [!WS "1"!]* in bytes */
    CurrentDataLength = (uint16)(((pMsgContext->reqDataLen) - 2U) - [!"num:i($inFixedDataLength div 8)"!]U);
    [!ENDIF!][!//
    [!CR!][!//
  [!ENDIF!][!//
  if (DCM_INITIAL == OpStatus)
  {
  [!INDENT "4"!][!//
  /* Total input data length check */
  [!VAR "inTotalDataLengthBytes"="num:i(($inTotalDataLength + 7) div 8)"!][!//
  [!VAR "inFixedDataLengthBytes"="num:i(($inFixedDataLength + 7) div 8)"!][!//
  [!IF "($inputSignalNr > 0)"!][!//
    [!IF "($inFixedDataLength > 0) and ($inTotalDataLength > $inFixedDataLength)"!][!//
    [!/* --- Both fixed and variable length signals are configured --- */!][!//
    if ((InDataLength >= [!"num:i($inFixedDataLengthBytes)"!]U) && (InDataLength <= [!"num:i($inTotalDataLengthBytes)"!]U))
    [!ELSEIF "($inFixedDataLength > 0)"!][!//
    [!/* --- Only fixed length signals are configured --- */!][!//
    if (InDataLength == [!"num:i($inFixedDataLengthBytes)"!]U)
    [!ELSEIF "($inTotalDataLength > 0)"!][!//
    [!/* --- Only variable length signals are configured --- */!][!//
    if (InDataLength <= [!"num:i($inTotalDataLengthBytes)"!]U)
    [!ENDIF!][!//
  [!ELSE!][!//
    [!/* --- No input signals are configured --- */!][!//
    if (InDataLength == 0U)
  [!ENDIF!][!//
    {
  [!CR!]
  [!ENDINDENT!][!//
  [!INDENT "6"!][!//
  [!IF "$resetOfSignalArray = 'true'"!][!//
    Dcm_Dsp_RoutineControl_ClearInternalArrays();
  [!CR!]
  [!ENDIF!][!//
  [!VAR "inVarCount"="0"!][!//
  [!VAR "Sint16ArrayElementsIn"="0"!][!//
  [!VAR "Sint32ArrayElementsIn"="0"!][!//
  [!VAR "Uint16ArrayElementsIn"="0"!][!//
  [!VAR "Uint32ArrayElementsIn"="0"!][!//
  [!IF "($inputSignalNr > 0)"!][!//
    [!LOOP "node:order(node:ref(DcmDspRoutineInfoRef)/*[name(.) = $routineInput]/*[name(.) = $inputSignal]/*, 'DcmDspRoutineSignalPos')"!][!//
      [!IF "DcmDspRoutineSignalType != 'VARIABLE_LENGTH'"!][!//
        [!/* --- Search for fixed signals and copy them to their buffers --- */!][!//
        [!VAR "SignalBaseBitSize" = "0"!][!//
        [!VAR "BaseTypeSignal"="''"!][!//
        [!IF "not(contains(DcmDspRoutineSignalType,'_N'))"!][!//
          [!VAR "BaseTypeSignal"="concat('&',text:split($inParamList, ', ')[position() = ($inVarCount + 1)])"!][!//
          [!IF "DcmDspRoutineSignalType = 'BOOLEAN'"!][!//
            [!VAR "SignalBaseBitSize" = "1"!][!//
          [!ELSE!][!//
            [!VAR "SignalBaseBitSize" = "num:integer(substring-after(DcmDspRoutineSignalType, 'INT'))"!][!//
          [!ENDIF!][!//
        [!ELSE!][!//
          [!VAR "SignalBaseBitSize" = "num:integer(substring-before(substring-after(DcmDspRoutineSignalType, 'INT'),'_' ))"!][!//
          [!VAR "BaseTypeSignal"="text:split($inParamList, ', ')[position() = ($inVarCount + 1)]"!][!//
        [!ENDIF!][!//
        /* Deviation MISRAC2012-1 <+2> */
        /* Deviation MISRA-1 <+2> */
        [!/* !LINKSTO Dcm.EB.RoutineControl.RoutineControlOptionRecord,1 */!][!//
        Dcm_DspInternal_CopySignalData((P2VAR(uint8, AUTOMATIC, DCM_VAR))&(pMsgContext->reqData[2]),
                            [!WS "31"!](P2VAR(uint8, AUTOMATIC, DCM_VAR))[!"$BaseTypeSignal"!],
                            [!WS "31"!][!"DcmDspRoutineSignalPos"!]U,
                            [!WS "31"!]0U,
                            [!WS "31"!][!"DcmDspRoutineSignalLength"!]U,
                            [!WS "31"!]DCM_COPY_FROM_INBUFFER_OPAQUE,
                            [!WS "31"!]DCM_BYTE_DIRECTION_INCREMENT);
        [!CR!]
      [!ELSE!][!//
        [!IF "not((DcmDspRoutineSignalPos mod 8) = 0)"!][!//
          [!WS "2"!]/* Shifting bits of the variable length signal to the right until the next byte boundary */
          [!WS "2"!]Dcm_DspInternal_ShiftBitsInBuffer((P2VAR(uint8, AUTOMATIC, DCM_VAR))&(pMsgContext->reqData[2]),
                            [!WS "36"!][!"num:i($InVarSignalShiftPos)"!]U,
                            [!WS "36"!][!"num:i($InVarSignalShiftPos + (8 -($InVarSignalShiftPos mod 8)))"!]U,
                            [!WS "36"!][!"DcmDspRoutineSignalLength"!]U,
                            [!WS "36"!]FALSE);
          [!CR!]
      [!ENDIF!][!//
      [!ENDIF!][!//
      [!/*--- Calculate how many input signals are in the 16 and 32 bit arrays. These will be used for the endianness conversion ---*/!][!//
      [!IF "(DcmDspRoutineSignalType = 'SINT16')"!][!//
        [!VAR "Sint16ArrayElementsIn"="$Sint16ArrayElementsIn + 1"!][!//
      [!ELSEIF "(DcmDspRoutineSignalType = 'SINT16_N')"!][!//
        [!VAR "Sint16ArrayElementsIn"="$Sint16ArrayElementsIn + (DcmDspRoutineSignalLength div 16)"!][!//
      [!ELSEIF "(DcmDspRoutineSignalType = 'UINT16')"!][!//
        [!VAR "Uint16ArrayElementsIn"="$Uint16ArrayElementsIn + 1"!][!//
      [!ELSEIF "(DcmDspRoutineSignalType = 'UINT16_N')"!][!//
        [!VAR "Uint16ArrayElementsIn"="$Uint16ArrayElementsIn + (DcmDspRoutineSignalLength div 16)"!][!//
      [!ELSEIF "(DcmDspRoutineSignalType = 'SINT32')"!][!//
        [!VAR "Sint32ArrayElementsIn"="$Sint32ArrayElementsIn + 1"!][!//
      [!ELSEIF "(DcmDspRoutineSignalType = 'SINT32_N')"!][!//
        [!VAR "Sint32ArrayElementsIn"="$Sint32ArrayElementsIn + (DcmDspRoutineSignalLength div 32)"!][!//
      [!ELSEIF "(DcmDspRoutineSignalType = 'UINT32')"!][!//
        [!VAR "Uint32ArrayElementsIn"="$Uint32ArrayElementsIn + 1"!][!//
      [!ELSEIF "(DcmDspRoutineSignalType = 'UINT32_N')"!][!//
        [!VAR "Uint32ArrayElementsIn"="$Uint32ArrayElementsIn + (DcmDspRoutineSignalLength div 32)"!][!//
      [!ENDIF!][!//
      [!VAR "inVarCount"="$inVarCount + 1"!][!//
    [!ENDLOOP!][!//
    [!/*--- If there are IN elements in the 16/32 bit arrays, perform endianness if needed ---*/!][!//
    [!IF "($Uint16ArrayElementsIn > 0) or ($Sint16ArrayElementsIn > 0) or ($Uint32ArrayElementsIn > 0) or ($Sint32ArrayElementsIn > 0)"!][!//
        [!INDENT "0"!][!//
        #if (DCM_ROUTINE_ENDIANNESS_CONVERSION == STD_ON)
        [!ENDINDENT!][!//
        /* !LINKSTO Dcm.EB.RoutineControl.EndiannessEnabled,1 */
        if(CurProtocolConfig.EndiannessConversion == TRUE)
        {
      [!IF "($Uint16ArrayElementsIn > 0)"!][!//
        [!WS "2"!]Dcm_Dsp_RoutineControl_SwapBytes_U16(&Uint16Array[0], [!"num:i($Uint16ArrayElementsIn)"!]U);[!//
        [!CR!]
      [!ENDIF!][!//
      [!IF "($Sint16ArrayElementsIn > 0)"!][!//
        [!WS "2"!]Dcm_Dsp_RoutineControl_SwapBytes_S16(&Sint16Array[0], [!"num:i($Sint16ArrayElementsIn)"!]U);[!//
        [!CR!]
      [!ENDIF!][!//
      [!IF "($Uint32ArrayElementsIn > 0)"!][!//
        [!WS "2"!]Dcm_Dsp_RoutineControl_SwapBytes_U32(&Uint32Array[0], [!"num:i($Uint32ArrayElementsIn)"!]U);[!//
        [!CR!]
      [!ENDIF!][!//
      [!IF "($Sint32ArrayElementsIn > 0)"!][!//
        [!WS "2"!]Dcm_Dsp_RoutineControl_SwapBytes_S32(&Sint32Array[0], [!"num:i($Sint32ArrayElementsIn)"!]U);[!//
        [!CR!]
      [!ENDIF!][!//
        }
        [!INDENT "0"!][!//
        #endif /*(DCM_ROUTINE_ENDIANNESS_CONVERSION == STD_ON)*/
        [!ENDINDENT!][!//
        [!CR!]
      [!ENDIF!][!//
  [!//
  [!ENDIF!][!//
  [!/* !LINKSTO Dcm.Dsn.IB.RoutineControl.FunctionCallGeneration,1 */!][!//
  [!/* !LINKSTO Dcm.EB.RoutineControl.CallSWCFunction,1 */!][!//
  [!IF "DcmDspRoutineUsePort = 'true'"!][!//
    [!IF "$RoutineFixedLength = 'false'"!][!//
      [!IF "$RoutineVariableOutSignals = 'true'"!][!//
        [!/* !LINKSTO Dcm.Dsn.IB.RoutineControl.InOperation.CSVariableOut,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartIn.CS.AUTOSAR40.FixedInFixedOut.FixedLenFalse,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopIn.CS.AUTOSAR40.FixedInFixedOut.FixedLenFalse,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsIn.CS.AUTOSAR40.FixedInFixedOut.FixedLenFalse,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartIn.CS.AUTOSAR40.VarInVarOut.LengthInBits,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartIn.CS.AUTOSAR40.VarInVarOut.LengthInBytes,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartIn.CS.AUTOSAR42.VarInVarOut.LengthInBits,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartIn.CS.AUTOSAR42.VarInVarOut.LengthInBytes,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartIn.CS.AUTOSAR40.FixedInVarOut.LengthInBits,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartIn.CS.AUTOSAR40.FixedInVarOut.LengthInBytes,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartIn.CS.AUTOSAR42.FixedInVarOut.LengthInBits,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartIn.CS.AUTOSAR42.FixedInVarOut.LengthInBytes,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopIn.CS.AUTOSAR40.VarInVarOut.LengthInBits,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopIn.CS.AUTOSAR40.VarInVarOut.LengthInBytes,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopIn.CS.AUTOSAR42.VarInVarOut.LengthInBits,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopIn.CS.AUTOSAR42.VarInVarOut.LengthInBytes,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopIn.CS.AUTOSAR40.FixedInVarOut.LengthInBits,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopIn.CS.AUTOSAR40.FixedInVarOut.LengthInBytes,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopIn.CS.AUTOSAR42.FixedInVarOut.LengthInBits,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopIn.CS.AUTOSAR42.FixedInVarOut.LengthInBytes,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsIn.CS.AUTOSAR40.VarInVarOut.LengthInBits,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsIn.CS.AUTOSAR40.VarInVarOut.LengthInBytes,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsIn.CS.AUTOSAR42.VarInVarOut.LengthInBits,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsIn.CS.AUTOSAR42.VarInVarOut.LengthInBytes,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsIn.CS.AUTOSAR40.FixedInVarOut.LengthInBits,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsIn.CS.AUTOSAR40.FixedInVarOut.LengthInBytes,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsIn.CS.AUTOSAR42.FixedInVarOut.LengthInBits,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsIn.CS.AUTOSAR42.FixedInVarOut.LengthInBytes,1 */!][!//
        [!/* !LINKSTO Dcm.Dsn.IB.RoutineControl.InOperation.CS.VariableInFixedOut.ASR40,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartIn.CS.AUTOSAR40.VarInFixedOut.LengthInBits,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartIn.CS.AUTOSAR40.VarInFixedOut.LengthInBytes,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopIn.CS.AUTOSAR40.VarInFixedOut.LengthInBits,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopIn.CS.AUTOSAR40.VarInFixedOut.LengthInBytes,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsIn.CS.AUTOSAR40.VarInFixedOut.LengthInBits,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsIn.CS.AUTOSAR40.VarInFixedOut.LengthInBytes,1 */!][!//
        ServiceProcessStatus = Rte_Call_RoutineServices_[!"node:name(.)"!]_[!"$operation"!]([!"$inParamList"!]DCM_INITIAL, [!"$outParamListAddress"!]&CurrentDataLength, ErrorCode);
      [!ELSE!][!//
        [!/* !LINKSTO Dcm.Dsn.IB.RoutineControl.InOperation.CS.VariableInFixedOut.ASR42,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartIn.CS.AUTOSAR42.VarInFixedOut.LengthInBits,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartIn.CS.AUTOSAR42.VarInFixedOut.LengthInBytes,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopIn.CS.AUTOSAR42.VarInFixedOut.LengthInBits,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopIn.CS.AUTOSAR42.VarInFixedOut.LengthInBytes,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsIn.CS.AUTOSAR42.VarInFixedOut.LengthInBits,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsIn.CS.AUTOSAR42.VarInFixedOut.LengthInBytes,1 */!][!//
        [!/* !LINKSTO Dcm.Dsn.IB.RoutineControl.InOperation.Callout.VariableInFixedOut.ASR42,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartIn.Callout.AUTOSAR42.VarInFixedOut.LengthInBits,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartIn.Callout.AUTOSAR42.VarInFixedOut.LengthInBytes,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopIn.Callout.AUTOSAR42.VarInFixedOut.LengthInBits,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopIn.Callout.AUTOSAR42.VarInFixedOut.LengthInBytes,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsIn.Callout.AUTOSAR42.VarInFixedOut.LengthInBits,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsIn.Callout.AUTOSAR42.VarInFixedOut.LengthInBytes,1 */!][!//
        ServiceProcessStatus = Rte_Call_RoutineServices_[!"node:name(.)"!]_[!"$operation"!]([!"$inParamList"!]DCM_INITIAL, [!"$outParamListAddress"!]CurrentDataLength, ErrorCode);
      [!ENDIF!][!//
    [!ELSE!][!//
      [!/* !LINKSTO Dcm.Dsn.IB.RoutineControl.InOperation.CS.FixedInFixedOut,1 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartIn.CS.AUTOSAR40.FixedInFixedOut.FixedLenTrue,1 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopIn.CS.AUTOSAR40.FixedInFixedOut.FixedLenTrue,1 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsIn.CS.AUTOSAR40.FixedInFixedOut.FixedLenTrue,1 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartIn.CS.AUTOSAR42.FixedInFixedOut,1 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopIn.CS.AUTOSAR42.FixedInFixedOut,1 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsIn.CS.AUTOSAR42.FixedInFixedOut,1 */!][!//
      ServiceProcessStatus = Rte_Call_RoutineServices_[!"node:name(.)"!]_[!"$operation"!]([!"$inParamList"!]DCM_INITIAL, [!"$outParamListAddress"!]ErrorCode);
    [!ENDIF!][!//
  [!ELSE!][!//
      [!IF "$RoutineFixedLength = 'false'"!][!//
        [!IF "$RoutineVariableOutSignals = 'true'"!][!//
          [!/* !LINKSTO Dcm.Dsn.IB.RoutineControl.InOperation.Callout.VariableOut,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartIn.Callout.AUTOSAR40.FixedInFixedOut.FixedLenFalse,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopIn.Callout.AUTOSAR40.FixedInFixedOut.FixedLenFalse,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsIn.Callout.AUTOSAR40.FixedInFixedOut.FixedLenFalse,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartIn.Callout.AUTOSAR40.VarInVarOut.LengthInBits,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartIn.Callout.AUTOSAR40.VarInVarOut.LengthInBytes,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartIn.Callout.AUTOSAR42.VarInVarOut.LengthInBits,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartIn.Callout.AUTOSAR42.VarInVarOut.LengthInBytes,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartIn.Callout.AUTOSAR40.FixedInVarOut.LengthInBits,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartIn.Callout.AUTOSAR40.FixedInVarOut.LengthInBytes,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartIn.Callout.AUTOSAR42.FixedInVarOut.LengthInBits,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartIn.Callout.AUTOSAR42.FixedInVarOut.LengthInBytes,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopIn.Callout.AUTOSAR40.VarInVarOut.LengthInBits,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopIn.Callout.AUTOSAR40.VarInVarOut.LengthInBytes,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopIn.Callout.AUTOSAR42.VarInVarOut.LengthInBits,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopIn.Callout.AUTOSAR42.VarInVarOut.LengthInBytes,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopIn.Callout.AUTOSAR40.FixedInVarOut.LengthInBits,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopIn.Callout.AUTOSAR40.FixedInVarOut.LengthInBytes,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopIn.Callout.AUTOSAR42.FixedInVarOut.LengthInBits,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopIn.Callout.AUTOSAR42.FixedInVarOut.LengthInBytes,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsIn.Callout.AUTOSAR40.VarInVarOut.LengthInBits,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsIn.Callout.AUTOSAR40.VarInVarOut.LengthInBytes,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsIn.Callout.AUTOSAR42.VarInVarOut.LengthInBits,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsIn.Callout.AUTOSAR42.VarInVarOut.LengthInBytes,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsIn.Callout.AUTOSAR40.FixedInVarOut.LengthInBits,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsIn.Callout.AUTOSAR40.FixedInVarOut.LengthInBytes,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsIn.Callout.AUTOSAR42.FixedInVarOut.LengthInBits,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsIn.Callout.AUTOSAR42.FixedInVarOut.LengthInBytes,1 */!][!//
          ServiceProcessStatus = [!"node:ref(concat('DcmDsp',$operation,'RoutineFnc'))"!]([!"$inParamList"!]DCM_INITIAL, [!"$outParamListAddress"!]&CurrentDataLength, ErrorCode);
        [!ELSE!][!//
          [!/* !LINKSTO Dcm.Dsn.IB.RoutineControl.InOperation.Callout.VariableInFixedOut.ASR40,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartIn.Callout.AUTOSAR40.VarInFixedOut.LengthInBits,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartIn.Callout.AUTOSAR40.VarInFixedOut.LengthInBytes,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopIn.Callout.AUTOSAR40.VarInFixedOut.LengthInBits,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopIn.Callout.AUTOSAR40.VarInFixedOut.LengthInBytes,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsIn.Callout.AUTOSAR40.VarInFixedOut.LengthInBits,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsIn.Callout.AUTOSAR40.VarInFixedOut.LengthInBytes,1 */!][!//
          ServiceProcessStatus = [!"node:ref(concat('DcmDsp',$operation,'RoutineFnc'))"!]([!"$inParamList"!]DCM_INITIAL, [!"$outParamListAddress"!]CurrentDataLength, ErrorCode);
        [!ENDIF!][!//
      [!ELSE!][!//
        [!/* !LINKSTO Dcm.Dsn.IB.RoutineControl.InOperation.Callout.FixedInFixedOut,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartIn.Callout.AUTOSAR40.FixedInFixedOut.FixedLenTrue,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopIn.Callout.AUTOSAR40.FixedInFixedOut.FixedLenTrue,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsIn.Callout.AUTOSAR40.FixedInFixedOut.FixedLenTrue,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartIn.Callout.AUTOSAR42.FixedInFixedOut,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopIn.Callout.AUTOSAR42.FixedInFixedOut,1 */!][!//
        [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsIn.Callout.AUTOSAR42.FixedInFixedOut,1 */!][!//
        ServiceProcessStatus = [!"node:ref(concat('DcmDsp',$operation,'RoutineFnc'))"!]([!"$inParamList"!]DCM_INITIAL, [!"$outParamListAddress"!]ErrorCode);
      [!ENDIF!][!//
  [!ENDIF!][!//
  [!CR!]
  [!ENDINDENT!][!//
  [!INDENT "4"!][!//
  }
  else
  {
    [!INDENT "6"!][!//
    [!/* !LINKSTO SWS_Dcm_01140,1 */!][!//
    /* Invalid total data length */
    *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
    ServiceProcessStatus = DCM_E_NOT_OK;
    [!ENDINDENT!][!//
  }
  [!ENDINDENT!][!//
  }
  else
  {
  [!INDENT "4"!][!//
  [!/* !LINKSTO Dcm.EB.RoutineControl.CallSWCFunction,1 */!][!//
  [!IF "DcmDspRoutineUsePort = 'true'"!][!//
    [!IF "$RoutineFixedLength = 'false'"!][!//
      [!IF "$RoutineVariableOutSignals = 'true'"!][!//
        ServiceProcessStatus = Rte_Call_RoutineServices_[!"node:name(.)"!]_[!"$operation"!]([!"$inParamList"!]OpStatus, [!"$outParamListAddress"!]&CurrentDataLength, ErrorCode);
      [!ELSE!][!//
        ServiceProcessStatus = Rte_Call_RoutineServices_[!"node:name(.)"!]_[!"$operation"!]([!"$inParamList"!]OpStatus, [!"$outParamListAddress"!]CurrentDataLength, ErrorCode);
      [!ENDIF!][!//
    [!ELSE!][!//
      ServiceProcessStatus = Rte_Call_RoutineServices_[!"node:name(.)"!]_[!"$operation"!]([!"$inParamList"!]OpStatus, [!"$outParamListAddress"!]ErrorCode);
    [!ENDIF!][!//
  [!ELSE!][!//
      [!IF "$RoutineFixedLength = 'false'"!][!//
        [!IF "$RoutineVariableOutSignals = 'true'"!][!//
          ServiceProcessStatus = [!"node:ref(concat('DcmDsp',$operation,'RoutineFnc'))"!]([!"$inParamList"!]OpStatus, [!"$outParamListAddress"!]&CurrentDataLength, ErrorCode);
        [!ELSE!][!//
          ServiceProcessStatus = [!"node:ref(concat('DcmDsp',$operation,'RoutineFnc'))"!]([!"$inParamList"!]OpStatus, [!"$outParamListAddress"!]CurrentDataLength, ErrorCode);
        [!ENDIF!][!//
      [!ELSE!][!//
        ServiceProcessStatus = [!"node:ref(concat('DcmDsp',$operation,'RoutineFnc'))"!]([!"$inParamList"!]OpStatus, [!"$outParamListAddress"!]ErrorCode);
      [!ENDIF!][!//
  [!ENDIF!][!//
  [!ENDINDENT!][!//
  }
  [!CR!]
  if (ServiceProcessStatus == E_OK)
  {
  [!INDENT "4"!][!//
  [!IF "$RoutineFixedLength = 'false'"!][!//
    [!IF "num:i($outFixedDataLength) > 0"!][!//
      [!IF "$DcmRoutineVarLenInBytes = 'false'"!][!//
        [!IF "$ResponseLengthWithoutCurrentDataLength = 'true'"!][!//
          [!/* !LINKSTO Dcm.Dsn.IB.RoutineControl.OutOperation.LengthInBits.FixedOut,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR40.FixedInFixedOut.FixedLenFalse,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR40.FixedInFixedOut.FixedLenTrue,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR42.FixedInFixedOut,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR40.FixedInFixedOut.FixedLenFalse,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR40.FixedInFixedOut.FixedLenTrue,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR42.FixedInFixedOut,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR40.VarInFixedOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR42.VarInFixedOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR40.VarInFixedOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR42.VarInFixedOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR40.FixedInFixedOut.FixedLenFalse,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR40.FixedInFixedOut.FixedLenTrue,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR42.FixedInFixedOut,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR40.FixedInFixedOut.FixedLenFalse,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR40.FixedInFixedOut.FixedLenTrue,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR42.FixedInFixedOut,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR40.VarInFixedOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR42.VarInFixedOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR40.VarInFixedOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR40.FixedInFixedOut.FixedLenFalse,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR42.VarInFixedOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR40.FixedInFixedOut.FixedLenTrue,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR42.FixedInFixedOut,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR40.FixedInFixedOut.FixedLenFalse,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR40.FixedInFixedOut.FixedLenTrue,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR42.FixedInFixedOut,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR40.VarInFixedOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR42.VarInFixedOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR40.VarInFixedOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR42.VarInFixedOut.LengthInBits,2 */!][!//
          pMsgContext->resDataLen = DCM_GET_BYTES([!"num:i($outFixedDataLength)"!]U) + DCM_SUBSERVICEID_AND_ROUTINEID_LENGTH;
        [!ELSE!][!//
          [!/* !LINKSTO Dcm.Dsn.IB.RoutineControl.OutOperation.LengthInBits.VariableOut,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR40.FixedInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR42.FixedInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR40.FixedInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR42.FixedInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR40.VarInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR42.VarInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR40.VarInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR42.VarInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR40.FixedInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR42.FixedInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR40.FixedInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR42.FixedInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR40.VarInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR42.VarInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR40.VarInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR42.VarInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR40.FixedInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR42.FixedInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR40.FixedInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR42.FixedInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR40.VarInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR42.VarInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR40.VarInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR42.VarInVarOut.LengthInBits,2 */!][!//
          pMsgContext->resDataLen = DCM_GET_BYTES([!"num:i($outFixedDataLength)"!]U + (Dcm_MsgLenType)CurrentDataLength) + DCM_SUBSERVICEID_AND_ROUTINEID_LENGTH;
        [!ENDIF!][!//
      [!ELSE!][!//
        if((CurrentDataLength * 8U) > [!"num:i($outVariableDataLength)"!]U)
        {
          [!INDENT "6"!][!//
          /* In case the application returns a length for variable signal bigger than is configured
          * this will be ignored and the length from configuration will be used. */
          pMsgContext->resDataLen = (Dcm_MsgLenType)DCM_GET_BYTES([!"num:i($outFixedDataLength)"!]U + [!"num:i($outVariableDataLength)"!]U) + DCM_SUBSERVICEID_AND_ROUTINEID_LENGTH;
          [!INDENT "0"!][!//
          #if (DCM_DEV_ERROR_DETECT == STD_ON)
          [!/* !LINKSTO Dcm.EB.RoutineControl.Start.ReturnLargerData.DET,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.Stop.ReturnLargerData.DET,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.RequestResults.ReturnLargerData.DET,1 */!][!//
          [!WS "6"!]DCM_DET_REPORT_ERROR([!"$SVCID"!], DCM_E_INTERFACE_RETURN_VALUE);
          #endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
          [!ENDINDENT!][!//
          [!ENDINDENT!][!//
        }
        else
        {
          [!INDENT "6"!][!//
          [!IF "$ResponseLengthWithoutCurrentDataLength = 'true'"!][!//
            [!/* !LINKSTO Dcm.Dsn.IB.RoutineControl.OutOperation.LengthInBytes.FixedOut,1 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR40.FixedInFixedOut.FixedLenFalse,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR40.FixedInFixedOut.FixedLenTrue,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR42.FixedInFixedOut,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR40.FixedInFixedOut.FixedLenFalse,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR40.FixedInFixedOut.FixedLenTrue,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR42.FixedInFixedOut,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR40.VarInFixedOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR42.VarInFixedOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR40.VarInFixedOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR42.VarInFixedOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR40.FixedInFixedOut.FixedLenFalse,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR40.FixedInFixedOut.FixedLenTrue,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR42.FixedInFixedOut,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR40.FixedInFixedOut.FixedLenFalse,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR40.FixedInFixedOut.FixedLenTrue,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR42.FixedInFixedOut,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR40.VarInFixedOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR42.VarInFixedOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR40.VarInFixedOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR40.FixedInFixedOut.FixedLenFalse,2 */!][!///
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR40.FixedInFixedOut.FixedLenTrue,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR42.VarInFixedOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR42.FixedInFixedOut,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR40.FixedInFixedOut.FixedLenFalse,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR40.FixedInFixedOut.FixedLenTrue,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR42.FixedInFixedOut,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR40.VarInFixedOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR42.VarInFixedOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR40.VarInFixedOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR42.VarInFixedOut.LengthInBytes,2 */!][!//
            pMsgContext->resDataLen = DCM_GET_BYTES([!"num:i($outFixedDataLength)"!]U) + DCM_SUBSERVICEID_AND_ROUTINEID_LENGTH;
          [!ELSE!][!//
            [!/* !LINKSTO Dcm.Dsn.IB.RoutineControl.OutOperation.LengthInBytes.VariableOut,1 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR40.FixedInVarOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR42.FixedInVarOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR42.FixedInVarOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR40.FixedInVarOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR42.FixedInVarOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR40.VarInVarOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR42.VarInVarOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR40.VarInVarOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR42.VarInVarOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR42.FixedInVarOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR40.FixedInVarOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR40.FixedInVarOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR42.FixedInVarOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR40.VarInVarOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR42.VarInVarOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR40.VarInVarOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR42.VarInVarOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR40.FixedInVarOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR42.FixedInVarOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR40.FixedInVarOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR42.FixedInVarOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR40.VarInVarOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR42.VarInVarOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR40.VarInVarOut.LengthInBytes,2 */!][!//
            [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR42.VarInVarOut.LengthInBytes,2 */!][!//
            pMsgContext->resDataLen = DCM_GET_BYTES([!"num:i($outFixedDataLength)"!]U) + (Dcm_MsgLenType)CurrentDataLength + DCM_SUBSERVICEID_AND_ROUTINEID_LENGTH;
          [!ENDIF!][!//
          [!ENDINDENT!][!//
        }
      [!ENDIF!][!//
    [!ELSE!][!//
      [!IF "$DcmRoutineVarLenInBytes = 'false'"!]
        [!IF "$ResponseLengthWithoutCurrentDataLength = 'true'"!][!//
          [!/* !LINKSTO Dcm.Dsn.IB.RoutineControl.OutOperation.LengthInBits.FixedOut,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR40.FixedInFixedOut.FixedLenFalse,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR40.FixedInFixedOut.FixedLenTrue,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR42.FixedInFixedOut,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR40.FixedInFixedOut.FixedLenFalse,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR40.FixedInFixedOut.FixedLenTrue,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR42.FixedInFixedOut,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR40.VarInFixedOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR42.VarInFixedOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR40.VarInFixedOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR42.VarInFixedOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR40.FixedInFixedOut.FixedLenFalse,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR40.FixedInFixedOut.FixedLenTrue,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR42.FixedInFixedOut,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR40.FixedInFixedOut.FixedLenFalse,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR40.FixedInFixedOut.FixedLenTrue,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR42.FixedInFixedOut,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR40.VarInFixedOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR42.VarInFixedOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR40.VarInFixedOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR40.FixedInFixedOut.FixedLenFalse,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR42.VarInFixedOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR40.FixedInFixedOut.FixedLenTrue,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR42.FixedInFixedOut,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR40.FixedInFixedOut.FixedLenFalse,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR40.FixedInFixedOut.FixedLenTrue,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR42.FixedInFixedOut,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR40.VarInFixedOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR42.VarInFixedOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR40.VarInFixedOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR42.VarInFixedOut.LengthInBits,2 */!][!//
          pMsgContext->resDataLen = DCM_SUBSERVICEID_AND_ROUTINEID_LENGTH;
        [!ELSE!][!//
          [!/* !LINKSTO Dcm.Dsn.IB.RoutineControl.OutOperation.LengthInBits.VariableOut,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR40.FixedInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR42.FixedInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR40.FixedInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR42.FixedInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR40.VarInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR42.VarInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR40.VarInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR42.VarInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR40.FixedInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR42.FixedInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR40.FixedInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR42.FixedInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR40.VarInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR42.VarInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR40.VarInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR42.VarInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR40.FixedInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR42.FixedInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR40.FixedInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR42.FixedInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR40.VarInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR42.VarInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR40.VarInVarOut.LengthInBits,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR42.VarInVarOut.LengthInBits,2 */!][!//
          pMsgContext->resDataLen = DCM_GET_BYTES((Dcm_MsgLenType)CurrentDataLength) + DCM_SUBSERVICEID_AND_ROUTINEID_LENGTH;
        [!ENDIF!][!//
      [!ELSE!][!//
        if((CurrentDataLength * 8U) > [!"num:i($outVariableDataLength)"!]U)
        {
          [!INDENT "6"!][!//
          pMsgContext->resDataLen = (Dcm_MsgLenType)DCM_GET_BYTES([!"num:i($outVariableDataLength)"!]U) + DCM_SUBSERVICEID_AND_ROUTINEID_LENGTH;
          [!INDENT "0"!][!//
          #if (DCM_DEV_ERROR_DETECT == STD_ON)
          [!/* !LINKSTO Dcm.EB.RoutineControl.Start.ReturnLargerData.DET,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.Stop.ReturnLargerData.DET,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.RequestResults.ReturnLargerData.DET,1 */!][!//
          [!WS "6"!]DCM_DET_REPORT_ERROR([!"$SVCID"!], DCM_E_INTERFACE_RETURN_VALUE);
          #endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
          [!ENDINDENT!][!//
          [!ENDINDENT!][!//
        }
        else
        {
        [!INDENT "6"!][!//
        [!IF "$ResponseLengthWithoutCurrentDataLength = 'true'"!][!//
          [!/* !LINKSTO Dcm.Dsn.IB.RoutineControl.OutOperation.LengthInBytes.FixedOut,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR40.FixedInFixedOut.FixedLenFalse,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR40.FixedInFixedOut.FixedLenTrue,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR42.FixedInFixedOut,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR40.FixedInFixedOut.FixedLenFalse,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR40.FixedInFixedOut.FixedLenTrue,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR42.FixedInFixedOut,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR40.VarInFixedOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR42.VarInFixedOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR40.VarInFixedOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR42.VarInFixedOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR40.FixedInFixedOut.FixedLenFalse,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR40.FixedInFixedOut.FixedLenTrue,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR42.FixedInFixedOut,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR40.FixedInFixedOut.FixedLenFalse,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR40.FixedInFixedOut.FixedLenTrue,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR42.FixedInFixedOut,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR40.VarInFixedOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR42.VarInFixedOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR40.VarInFixedOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR40.FixedInFixedOut.FixedLenFalse,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR40.FixedInFixedOut.FixedLenTrue,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR42.VarInFixedOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR42.FixedInFixedOut,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR40.FixedInFixedOut.FixedLenFalse,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR40.FixedInFixedOut.FixedLenTrue,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR42.FixedInFixedOut,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR40.VarInFixedOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR42.VarInFixedOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR40.VarInFixedOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR42.VarInFixedOut.LengthInBytes,2 */!][!//
          pMsgContext->resDataLen = DCM_SUBSERVICEID_AND_ROUTINEID_LENGTH;
        [!ELSE!][!//
          [!/* !LINKSTO Dcm.Dsn.IB.RoutineControl.OutOperation.LengthInBytes.VariableOut,1 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR40.FixedInVarOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR42.FixedInVarOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR40.FixedInVarOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR42.FixedInVarOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR40.VarInVarOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR42.VarInVarOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR40.VarInVarOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR42.VarInVarOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR42.FixedInVarOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR40.FixedInVarOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR40.FixedInVarOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR42.FixedInVarOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR40.VarInVarOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR42.VarInVarOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR40.VarInVarOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR42.VarInVarOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR40.FixedInVarOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR42.FixedInVarOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR40.FixedInVarOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR42.FixedInVarOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR40.VarInVarOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR42.VarInVarOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR40.VarInVarOut.LengthInBytes,2 */!][!//
          [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR42.VarInVarOut.LengthInBytes,2 */!][!//
          pMsgContext->resDataLen = (Dcm_MsgLenType)CurrentDataLength + DCM_SUBSERVICEID_AND_ROUTINEID_LENGTH;
        [!ENDIF!][!//
        [!ENDINDENT!][!//
        }
      [!ENDIF!][!//
    [!ENDIF!][!//
  [!ELSE!][!//
      [!/* !LINKSTO Dcm.Dsn.IB.RoutineControl.OutOperation.FixedOut,1 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR40.FixedInFixedOut.FixedLenFalse,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR40.FixedInFixedOut.FixedLenTrue,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR42.FixedInFixedOut,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR40.FixedInFixedOut.FixedLenFalse,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR40.FixedInFixedOut.FixedLenTrue,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR42.FixedInFixedOut,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR40.VarInFixedOut.LengthInBits,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR40.VarInFixedOut.LengthInBytes,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR42.VarInFixedOut.LengthInBits,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.Callout.AUTOSAR42.VarInFixedOut.LengthInBytes,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR40.VarInFixedOut.LengthInBits,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR40.VarInFixedOut.LengthInBytes,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR42.VarInFixedOut.LengthInBits,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStartOut.CS.AUTOSAR42.VarInFixedOut.LengthInBytes,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR40.FixedInFixedOut.FixedLenFalse,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR40.FixedInFixedOut.FixedLenTrue,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR42.FixedInFixedOut,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR40.FixedInFixedOut.FixedLenFalse,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR40.FixedInFixedOut.FixedLenTrue,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR42.FixedInFixedOut,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR40.VarInFixedOut.LengthInBits,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR40.VarInFixedOut.LengthInBytes,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR42.VarInFixedOut.LengthInBits,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.Callout.AUTOSAR42.VarInFixedOut.LengthInBytes,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR40.VarInFixedOut.LengthInBits,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR40.VarInFixedOut.LengthInBytes,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR42.VarInFixedOut.LengthInBits,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxStopOut.CS.AUTOSAR42.VarInFixedOut.LengthInBytes,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR40.FixedInFixedOut.FixedLenTrue,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR40.FixedInFixedOut.FixedLenFalse,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR42.FixedInFixedOut,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR40.FixedInFixedOut.FixedLenFalse,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR40.FixedInFixedOut.FixedLenTrue,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR42.FixedInFixedOut,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR40.VarInFixedOut.LengthInBits,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR40.VarInFixedOut.LengthInBytes,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR42.VarInFixedOut.LengthInBits,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.Callout.AUTOSAR42.VarInFixedOut.LengthInBytes,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR40.VarInFixedOut.LengthInBits,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR40.VarInFixedOut.LengthInBytes,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR42.VarInFixedOut.LengthInBits,2 */!][!//
      [!/* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResultsOut.CS.AUTOSAR42.VarInFixedOut.LengthInBytes,2 */!][!//
      pMsgContext->resDataLen = [!"num:i((($outFixedDataLength + 7) div 8) + 3)"!]U;
  [!ENDIF!][!//
  [!VAR "outVarCount"="0"!][!//
    [!IF "($outputSignalNr > 0)"!][!//
      [!IF "node:exists(node:ref(DcmDspRoutineInfoRef)/*[name(.) = $routineOutput]/*[name(.) = $outputSignal]/*[(contains(DcmDspRoutineSignalType,'16') or contains(DcmDspRoutineSignalType,'32'))]/*)"!]
        [!VAR "Uint16ArrayElementsOut"="$Uint16ArrayCount - $Uint16ArrayElementsIn"!][!//
        [!VAR "Sint16ArrayElementsOut"="$Sint16ArrayCount - $Sint16ArrayElementsIn"!][!//
        [!VAR "Uint32ArrayElementsOut"="$Uint32ArrayCount - $Uint32ArrayElementsIn"!][!//
        [!VAR "Sint32ArrayElementsOut"="$Sint32ArrayCount - $Sint32ArrayElementsIn"!][!//
        [!INDENT "0"!][!//
        #if (DCM_ROUTINE_ENDIANNESS_CONVERSION == STD_ON)
        [!ENDINDENT!][!//
        /* !LINKSTO Dcm.EB.RoutineControl.EndiannessEnabled,1 */
        if(CurProtocolConfig.EndiannessConversion == TRUE)
        {
        [!/*--- The out signals are positioned after the last IN signal so start the conversion afterwards ---*/!][!//
        [!IF "($Uint16ArrayElementsOut > 0)"!][!//
          [!WS "2"!]Dcm_Dsp_RoutineControl_SwapBytes_U16(&Uint16Array[[!"num:i($Uint16ArrayElementsIn)"!]], [!"num:i($Uint16ArrayElementsOut)"!]U);[!//
          [!CR!]
        [!ENDIF!][!//
        [!IF "($Sint16ArrayElementsOut > 0)"!][!//
          [!WS "2"!]Dcm_Dsp_RoutineControl_SwapBytes_S16(&Sint16Array[[!"num:i($Sint16ArrayElementsIn)"!]], [!"num:i($Sint16ArrayElementsOut)"!]U);[!//
          [!CR!]
        [!ENDIF!][!//
        [!IF "($Uint32ArrayElementsOut > 0)"!][!//
          [!WS "2"!]Dcm_Dsp_RoutineControl_SwapBytes_U32(&Uint32Array[[!"num:i($Uint32ArrayElementsIn)"!]], [!"num:i($Uint32ArrayElementsOut)"!]U);[!//
          [!CR!]
        [!ENDIF!][!//
        [!IF "($Sint32ArrayElementsOut > 0)"!][!//
          [!WS "2"!]Dcm_Dsp_RoutineControl_SwapBytes_S32(&Sint32Array[[!"num:i($Sint32ArrayElementsIn)"!]], [!"num:i($Sint32ArrayElementsOut)"!]U);[!//
          [!CR!]
        [!ENDIF!][!//
          }
        [!INDENT "0"!][!//
        #endif /*(DCM_ROUTINE_ENDIANNESS_CONVERSION == STD_ON)*/
        [!ENDINDENT!][!//
        [!CR!]
      [!ENDIF!][!//
      [!LOOP "node:order(node:ref(DcmDspRoutineInfoRef)/*[name(.) = $routineOutput]/*[name(.) = $outputSignal]/*, 'DcmDspRoutineSignalPos')"!][!//
        [!IF "(DcmDspRoutineSignalType = 'VARIABLE_LENGTH')"!][!//
          [!IF "$CommonBufferCount = 0"!][!//
            [!/* Rx and Tx buffer are different */!][!//
            [!IF "not((DcmDspRoutineSignalPos mod 8) = '0') "!][!//
              [!CR!]
              [!/* Variable length signal is not byte aligned */!][!//
              [!IF "$DcmRoutineVarLenInBytes = 'false'"!][!//
                Dcm_DspInternal_ShiftBitsInBuffer((P2VAR(uint8, AUTOMATIC, DCM_VAR))&(pMsgContext->resData[3]),
                  [!WS "34"!][!"num:i($outFixedDataLength + (8- ($outFixedDataLength mod 8)))"!]U,
                  [!WS "34"!][!"num:i($outFixedDataLength)"!]U,
                  [!WS "34"!]CurrentDataLength,
                  [!WS "34"!]TRUE);
              [!ELSE!][!//
                Dcm_DspInternal_ShiftBitsInBuffer((P2VAR(uint8, AUTOMATIC, DCM_VAR))&(pMsgContext->resData[3]),
                  [!WS "34"!][!"num:i($outFixedDataLength + (8 - ($outFixedDataLength mod 8)))"!]U,
                  [!WS "34"!][!"num:i($outFixedDataLength)"!]U,
                  [!WS "34"!](uint32)((uint32)CurrentDataLength * 8U),
                  [!WS "34"!]TRUE);
              [!ENDIF!][!//
            [!ENDIF!][!//
          [!ELSE!][!//
            [!CR!]
            [!/* Rx and Tx buffers are the same, an auxiliary buffer needs to be used  */!][!//
            Dcm_DspInternal_CopySignalData((P2VAR(uint8, AUTOMATIC, DCM_VAR))&Dcm_RoutineControlBuffer,
                                [!WS "31"!](P2VAR(uint8, AUTOMATIC, DCM_VAR))&(pMsgContext->resData[3]),
                                [!WS "31"!]0U,
                                [!WS "31"!][!"DcmDspRoutineSignalPos"!]U,
                                [!WS "31"!][!"DcmDspRoutineSignalLength"!]U,
                                [!WS "31"!]DCM_COPY_TO_OUTBUFFER_OPAQUE,
                                [!WS "31"!]DCM_BYTE_DIRECTION_INCREMENT);
          [!ENDIF!][!//
        [!ELSE!][!//
          /* Deviation MISRAC2012-1 <+2> */
          /* Deviation MISRA-1 <+2> */
          Dcm_DspInternal_CopySignalData((P2VAR(uint8, AUTOMATIC, DCM_VAR))&[!"text:split($outParamList, ', ')[position() = ($outVarCount + 1)]"!],
                              [!WS "31"!](P2VAR(uint8, AUTOMATIC, DCM_VAR))&(pMsgContext->resData[3]),
                              [!WS "31"!]0U,
                              [!WS "31"!][!"DcmDspRoutineSignalPos"!]U,
                              [!WS "31"!][!"DcmDspRoutineSignalLength"!]U,
                              [!WS "31"!]DCM_COPY_TO_OUTBUFFER_OPAQUE,
                              [!WS "31"!]DCM_BYTE_DIRECTION_INCREMENT);
          [!VAR "outVarCount"="$outVarCount + 1"!][!//
        [!ENDIF!][!//
      [!ENDLOOP!][!//
    [!ENDIF!][!//
    [!CR!]
    ServiceProcessStatus = DCM_E_OK;
    [!ENDINDENT!][!//
    }
    else if(ServiceProcessStatus == E_NOT_OK)
    {
      [!WS "2"!]ServiceProcessStatus = DCM_E_NOT_OK;
    }
    else
    {
      [!WS "2"!]/* ServiceProcessStatus is DCM_E_PENDING, DCM_E_FORCE_RCRRP or unknown and these
      [!WS "3"!]* cases are processed in service handler */
    }
  [!CR!]
  return ServiceProcessStatus;
  [!ENDINDENT!][!//
}
[!ENDIF!][!//
[!ENDMACRO!][!//
[!//
/*==================[inclusions]=================================================================*/

[!AUTOSPACING!][!//
#include <Dcm_RoutineControl_Cfg.h>                             /*  RoutineControl Configuration */
#include <Dcm_API_Cfg.h>                                  /*  Configurable function declarations */
#include <Dcm_DspExternal.h>                           /* Callbacks provided by Central Diag SwC */
#include <Dcm_Dsl_Supervisor.h>                        /* Dcm Dsl Supervisor header file */
#include <TSMem.h>
/*==================[macros]=====================================================================*/

/* The minimum length of a positive response (without the SID which is already added in the Dsd) is:
 * byte Subservice ID + 2 bytes RoutineId. */
#define DCM_SUBSERVICEID_AND_ROUTINEID_LENGTH 3U

/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/

/*==================[internal function declarations]=============================================*/
[!IF "($DCM_DSP_USE_SERVICE_0X31 = 'STD_ON')"!]

[!IF "$resetOfSignalArray = 'true' or $BytesSwapNeededU16 = 'true' or $BytesSwapNeededS16 = 'true' or $BytesSwapNeededU32 = 'true' or $BytesSwapNeededS32 = 'true'"!][!//

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

[!IF "$resetOfSignalArray = 'true'"!][!//
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_RoutineControl_ClearInternalArrays(void);
[!ENDIF!]

#if (DCM_ROUTINE_ENDIANNESS_CONVERSION == STD_ON)
[!IF "$BytesSwapNeededU16 = 'true'"!][!//
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_RoutineControl_SwapBytes_U16(uint16* SignalData, uint32 SignalLength);
[!ENDIF!]

[!IF "$BytesSwapNeededS16 = 'true'"!][!//
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_RoutineControl_SwapBytes_S16(sint16* SignalData, uint32 SignalLength);
[!ENDIF!]

[!IF "$BytesSwapNeededU32 = 'true'"!][!//
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_RoutineControl_SwapBytes_U32(uint32* SignalData, uint32 SignalLength);
[!ENDIF!]

[!IF "$BytesSwapNeededS32 = 'true'"!][!//
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_RoutineControl_SwapBytes_S32(sint32* SignalData, uint32 SignalLength);
[!ENDIF!]
#endif

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>
[!ENDIF!]
[!ENDIF!]

/*==================[internal constants]=========================================================*/

/*==================[internal data]==============================================================*/
[!//
[!VAR "CommonBufferCount"="count(DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*[DcmDslProtocolTxBufferID =  DcmDslConnection/*/DcmDslProtocolRx/*/DcmDslProtocolRxBufferID])"!]
[!IF "$CommonBufferCount > 0"!]
  [!IF "($DCM_DSP_USE_SERVICE_0X31 = 'STD_ON')"!]
    [!IF "(node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspRoutine/*[DcmDspRoutineUsed = 'true' and not(node:exists(DcmDspRoutineTidRef)) and (node:exists(node:ref(DcmDspRoutineInfoRef)/*//DcmDspRoutineSignalType [. = 'VARIABLE_LENGTH']))]))"!]
      [!VAR "SignalLength" = "0"!]
      [!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspRoutineInfo/*//DcmDspRoutineSignalType[. = 'VARIABLE_LENGTH']"!]
        [!IF "(../DcmDspRoutineSignalLength > $SignalLength) and ((node:name(../../.) = 'DcmDspStartRoutineOutSignal') or (node:name(../../.) = 'DcmDspRoutineStopOutSignal') or (node:name(../../.) = 'DcmDspRoutineRequestResOutSignal'))"!]
          [!VAR "SignalLength" = "../DcmDspRoutineSignalLength"!]
        [!ENDIF!]
      [!ENDLOOP!]
      [!IF "$SignalLength > 0"!]
        [!INDENT "0"!][!//
        #define DCM_START_SEC_VAR_CLEARED_8
        #include <Dcm_MemMap.h>
        [!CR!]
        /* Defining global buffer for Routine control service */
        /* Deviation MISRAC2012-2 */
        STATIC VAR(uint8, DCM_VAR) Dcm_RoutineControlBuffer[[!"num:i(($SignalLength + 7) div 8)"!]];
        [!CR!]
        #define DCM_STOP_SEC_VAR_CLEARED_8
        #include <Dcm_MemMap.h>
        [!ENDINDENT!]
      [!ENDIF!]
    [!ENDIF!]
  [!ENDIF!]
[!ENDIF!]

[!IF "($DCM_DSP_USE_SERVICE_0X31 = 'STD_ON')"!]
  [!VAR "BooleanArrayMaxLength"="0"!][!//
  [!VAR "Sint16ArrayMaxLength"="0"!][!//
  [!VAR "Sint32ArrayMaxLength"="0"!][!//
  [!VAR "Sint8ArrayMaxLength"="0"!][!//
  [!VAR "Uint16ArrayMaxLength"="0"!][!//
  [!VAR "Uint32ArrayMaxLength"="0"!][!//
  [!VAR "Uint8ArrayMaxLength"="0"!][!//
  [!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspRoutine/*"!]
    [!IF "DcmDspRoutineUsed = 'true' and not(node:exists(DcmDspRoutineTidRef))"!]
      [!VAR "StartBooleanArrayLength"="0"!][!//
      [!VAR "StartSint16ArrayLength"="0"!][!//
      [!VAR "StartSint32ArrayLength"="0"!][!//
      [!VAR "StartSint8ArrayLength"="0"!][!//
      [!VAR "StartUint16ArrayLength"="0"!][!//
      [!VAR "StartUint32ArrayLength"="0"!][!//
      [!VAR "StartUint8ArrayLength"="0"!][!//
      [!LOOP "node:ref(./DcmDspRoutineInfoRef)/*[contains(name(.),'Start')]/*/*"!]
        [!IF "(./DcmDspRoutineSignalType = 'BOOLEAN')"!]
          [!VAR "StartBooleanArrayLength"="$StartBooleanArrayLength + 1"!][!//
        [!ELSEIF "(./DcmDspRoutineSignalType = 'SINT16')"!]
          [!VAR "StartSint16ArrayLength"="$StartSint16ArrayLength + 1"!][!//
        [!ELSEIF "(./DcmDspRoutineSignalType = 'SINT32')"!]
          [!VAR "StartSint32ArrayLength"="$StartSint32ArrayLength + 1"!][!//
        [!ELSEIF "(./DcmDspRoutineSignalType = 'SINT8')"!]
          [!VAR "StartSint8ArrayLength"="$StartSint8ArrayLength + 1"!][!//
        [!ELSEIF "(./DcmDspRoutineSignalType = 'UINT16')"!]
          [!VAR "StartUint16ArrayLength"="$StartUint16ArrayLength + 1"!][!//
        [!ELSEIF "(./DcmDspRoutineSignalType = 'UINT32')"!]
          [!VAR "StartUint32ArrayLength"="$StartUint32ArrayLength + 1"!][!//
        [!ELSEIF "(./DcmDspRoutineSignalType = 'UINT8')"!]
          [!VAR "StartUint8ArrayLength"="$StartUint8ArrayLength + 1"!][!//
        [!ELSEIF "(./DcmDspRoutineSignalType = 'SINT16_N')"!]
          [!VAR "StartSint16ArrayLength"="$StartSint16ArrayLength + (./DcmDspRoutineSignalLength div 16)"!][!//
        [!ELSEIF "(./DcmDspRoutineSignalType = 'SINT32_N')"!]
          [!VAR "StartSint32ArrayLength"="$StartSint32ArrayLength + (./DcmDspRoutineSignalLength div 32)"!][!//
        [!ELSEIF "(./DcmDspRoutineSignalType = 'SINT8_N')"!]
          [!VAR "StartSint8ArrayLength"="$StartSint8ArrayLength + (./DcmDspRoutineSignalLength div 8)"!][!//
        [!ELSEIF "(./DcmDspRoutineSignalType = 'UINT16_N')"!]
          [!VAR "StartUint16ArrayLength"="$StartUint16ArrayLength + (./DcmDspRoutineSignalLength div 16)"!][!//
        [!ELSEIF "(./DcmDspRoutineSignalType = 'UINT32_N')"!]
          [!VAR "StartUint32ArrayLength"="$StartUint32ArrayLength + (./DcmDspRoutineSignalLength div 32)"!][!//
        [!ELSEIF "(./DcmDspRoutineSignalType = 'UINT8_N')"!]
          [!VAR "StartUint8ArrayLength"="$StartUint8ArrayLength + (./DcmDspRoutineSignalLength div 8)"!][!//
        [!ENDIF!]
      [!ENDLOOP!]
      [!VAR "StopBooleanArrayLength"="0"!][!//
      [!VAR "StopSint16ArrayLength"="0"!][!//
      [!VAR "StopSint32ArrayLength"="0"!][!//
      [!VAR "StopSint8ArrayLength"="0"!][!//
      [!VAR "StopUint16ArrayLength"="0"!][!//
      [!VAR "StopUint32ArrayLength"="0"!][!//
      [!VAR "StopUint8ArrayLength"="0"!][!//
      [!IF "DcmDspStopRoutineSupported = 'true'"!]
        [!LOOP "node:ref(./DcmDspRoutineInfoRef)/*[contains(name(.),'Stop')]/*/*"!]
          [!IF "(./DcmDspRoutineSignalType = 'BOOLEAN')"!]
            [!VAR "StopBooleanArrayLength"="$StopBooleanArrayLength + 1"!][!//
          [!ELSEIF "(./DcmDspRoutineSignalType = 'SINT16')"!]
            [!VAR "StopSint16ArrayLength"="$StopSint16ArrayLength + 1"!][!//
          [!ELSEIF "(./DcmDspRoutineSignalType = 'SINT32')"!]
            [!VAR "StopSint32ArrayLength"="$StopSint32ArrayLength + 1"!][!//
          [!ELSEIF "(./DcmDspRoutineSignalType = 'SINT8')"!]
            [!VAR "StopSint8ArrayLength"="$StopSint8ArrayLength + 1"!][!//
          [!ELSEIF "(./DcmDspRoutineSignalType = 'UINT16')"!]
            [!VAR "StopUint16ArrayLength"="$StopUint16ArrayLength + 1"!][!//
          [!ELSEIF "(./DcmDspRoutineSignalType = 'UINT32')"!]
            [!VAR "StopUint32ArrayLength"="$StopUint32ArrayLength + 1"!][!//
          [!ELSEIF "(./DcmDspRoutineSignalType = 'UINT8')"!]
            [!VAR "StopUint8ArrayLength"="$StopUint8ArrayLength + 1"!][!//
          [!ELSEIF "(./DcmDspRoutineSignalType = 'SINT16_N')"!]
            [!VAR "StopSint16ArrayLength"="$StopSint16ArrayLength + (./DcmDspRoutineSignalLength div 16)"!][!//
          [!ELSEIF "(./DcmDspRoutineSignalType = 'SINT32_N')"!]
            [!VAR "StopSint32ArrayLength"="$StopSint32ArrayLength + (./DcmDspRoutineSignalLength div 32)"!][!//
          [!ELSEIF "(./DcmDspRoutineSignalType = 'SINT8_N')"!]
            [!VAR "StopSint8ArrayLength"="$StopSint8ArrayLength + (./DcmDspRoutineSignalLength div 8)"!][!//
          [!ELSEIF "(./DcmDspRoutineSignalType = 'UINT16_N')"!]
            [!VAR "StopUint16ArrayLength"="$StopUint16ArrayLength + (./DcmDspRoutineSignalLength div 16)"!][!//
          [!ELSEIF "(./DcmDspRoutineSignalType = 'UINT32_N')"!]
            [!VAR "StopUint32ArrayLength"="$StopUint32ArrayLength + (./DcmDspRoutineSignalLength div 32)"!][!//
          [!ELSEIF "(./DcmDspRoutineSignalType = 'UINT8_N')"!]
            [!VAR "StopUint8ArrayLength"="$StopUint8ArrayLength + (./DcmDspRoutineSignalLength div 8)"!][!//
          [!ENDIF!]
        [!ENDLOOP!]
          [!ENDIF!]
      [!VAR "RequestResBooleanArrayLength"="0"!][!//
      [!VAR "RequestResSint16ArrayLength"="0"!][!//
      [!VAR "RequestResSint32ArrayLength"="0"!][!//
      [!VAR "RequestResSint8ArrayLength"="0"!][!//
      [!VAR "RequestResUint16ArrayLength"="0"!][!//
      [!VAR "RequestResUint32ArrayLength"="0"!][!//
      [!VAR "RequestResUint8ArrayLength"="0"!][!//
      [!IF "DcmDspRequestResultsRoutineSupported = 'true'"!]
        [!LOOP "node:ref(./DcmDspRoutineInfoRef)/*[contains(name(.),'Request')]/*/*"!]
          [!IF "(./DcmDspRoutineSignalType = 'BOOLEAN')"!]
            [!VAR "RequestResBooleanArrayLength"="$RequestResBooleanArrayLength + 1"!][!//
          [!ELSEIF "(./DcmDspRoutineSignalType = 'SINT16')"!]
            [!VAR "RequestResSint16ArrayLength"="$RequestResSint16ArrayLength + 1"!][!//
          [!ELSEIF "(./DcmDspRoutineSignalType = 'SINT32')"!]
            [!VAR "RequestResSint32ArrayLength"="$RequestResSint32ArrayLength + 1"!][!//
          [!ELSEIF "(./DcmDspRoutineSignalType = 'SINT8')"!]
            [!VAR "RequestResSint8ArrayLength"="$RequestResSint8ArrayLength + 1"!][!//
          [!ELSEIF "(./DcmDspRoutineSignalType = 'UINT16')"!]
            [!VAR "RequestResUint16ArrayLength"="$RequestResUint16ArrayLength + 1"!][!//
          [!ELSEIF "(./DcmDspRoutineSignalType = 'UINT32')"!]
            [!VAR "RequestResUint32ArrayLength"="$RequestResUint32ArrayLength + 1"!][!//
          [!ELSEIF "(./DcmDspRoutineSignalType = 'UINT8')"!]
            [!VAR "RequestResUint8ArrayLength"="$RequestResUint8ArrayLength + 1"!][!//
          [!ELSEIF "(./DcmDspRoutineSignalType = 'SINT16_N')"!]
            [!VAR "RequestResSint16ArrayLength"="$RequestResSint16ArrayLength + (./DcmDspRoutineSignalLength div 16)"!][!//
          [!ELSEIF "(./DcmDspRoutineSignalType = 'SINT32_N')"!]
            [!VAR "RequestResSint32ArrayLength"="$RequestResSint32ArrayLength + (./DcmDspRoutineSignalLength div 32)"!][!//
          [!ELSEIF "(./DcmDspRoutineSignalType = 'SINT8_N')"!]
            [!VAR "RequestResSint8ArrayLength"="$RequestResSint8ArrayLength + (./DcmDspRoutineSignalLength div 8)"!][!//
          [!ELSEIF "(./DcmDspRoutineSignalType = 'UINT16_N')"!]
            [!VAR "RequestResUint16ArrayLength"="$RequestResUint16ArrayLength + (./DcmDspRoutineSignalLength div 16)"!][!//
          [!ELSEIF "(./DcmDspRoutineSignalType = 'UINT32_N')"!]
            [!VAR "RequestResUint32ArrayLength"="$RequestResUint32ArrayLength + (./DcmDspRoutineSignalLength div 32)"!][!//
          [!ELSEIF "(./DcmDspRoutineSignalType = 'UINT8_N')"!]
            [!VAR "RequestResUint8ArrayLength"="$RequestResUint8ArrayLength + (./DcmDspRoutineSignalLength div 8)"!][!//
          [!ENDIF!]
        [!ENDLOOP!]
          [!ENDIF!]
      [!IF "($StartBooleanArrayLength > $BooleanArrayMaxLength)"!]
        [!VAR "BooleanArrayMaxLength" = "$StartBooleanArrayLength"!]
      [!ENDIF!]
      [!IF "($StopBooleanArrayLength > $BooleanArrayMaxLength)"!][!//
        [!VAR "BooleanArrayMaxLength" = "$StopBooleanArrayLength"!]
      [!ENDIF!]
      [!IF "($RequestResBooleanArrayLength > $BooleanArrayMaxLength)"!]
        [!VAR "BooleanArrayMaxLength" = "$RequestResBooleanArrayLength"!]
      [!ENDIF!]
      [!IF "($StartSint16ArrayLength > $Sint16ArrayMaxLength)"!]
        [!VAR "Sint16ArrayMaxLength" = "$StartSint16ArrayLength"!]
      [!ENDIF!]
      [!IF "($StopSint16ArrayLength > $Sint16ArrayMaxLength)"!][!//
        [!VAR "Sint16ArrayMaxLength" = "$StopSint16ArrayLength"!]
      [!ENDIF!]
      [!IF "($RequestResSint16ArrayLength > $Sint16ArrayMaxLength)"!]
        [!VAR "Sint16ArrayMaxLength" = "$RequestResSint16ArrayLength"!]
      [!ENDIF!]
      [!IF "($StartSint32ArrayLength > $Sint32ArrayMaxLength)"!]
        [!VAR "Sint32ArrayMaxLength" = "$StartSint32ArrayLength"!]
      [!ENDIF!]
      [!IF "($StopSint32ArrayLength > $Sint32ArrayMaxLength)"!][!//
        [!VAR "Sint32ArrayMaxLength" = "$StopSint32ArrayLength"!]
      [!ENDIF!]
      [!IF "($RequestResSint32ArrayLength > $Sint32ArrayMaxLength)"!]
        [!VAR "Sint32ArrayMaxLength" = "$RequestResSint32ArrayLength"!]
      [!ENDIF!]
      [!IF "($StartSint8ArrayLength > $Sint8ArrayMaxLength)"!]
        [!VAR "Sint8ArrayMaxLength" = "$StartSint8ArrayLength"!]
      [!ENDIF!]
      [!IF "($StopSint8ArrayLength > $Sint8ArrayMaxLength)"!][!//
        [!VAR "Sint8ArrayMaxLength" = "$StopSint8ArrayLength"!]
      [!ENDIF!]
      [!IF "($RequestResSint8ArrayLength > $Sint8ArrayMaxLength)"!]
        [!VAR "Sint8ArrayMaxLength" = "$RequestResSint8ArrayLength"!]
      [!ENDIF!]
      [!IF "($StartUint16ArrayLength > $Uint16ArrayMaxLength)"!]
        [!VAR "Uint16ArrayMaxLength" = "$StartUint16ArrayLength"!]
      [!ENDIF!]
      [!IF "($StopUint16ArrayLength > $Uint16ArrayMaxLength)"!][!//
        [!VAR "Uint16ArrayMaxLength" = "$StopUint16ArrayLength"!]
      [!ENDIF!]
      [!IF "($RequestResUint16ArrayLength > $Uint16ArrayMaxLength)"!]
        [!VAR "Uint16ArrayMaxLength" = "$RequestResUint16ArrayLength"!]
      [!ENDIF!]
      [!IF "($StartUint32ArrayLength > $Uint32ArrayMaxLength)"!]
        [!VAR "Uint32ArrayMaxLength" = "$StartUint32ArrayLength"!]
      [!ENDIF!]
      [!IF "($StopUint32ArrayLength > $Uint32ArrayMaxLength)"!][!//
        [!VAR "Uint32ArrayMaxLength" = "$StopUint32ArrayLength"!]
      [!ENDIF!]
      [!IF "($RequestResUint32ArrayLength > $Uint32ArrayMaxLength)"!]
        [!VAR "Uint32ArrayMaxLength" = "$RequestResUint32ArrayLength"!]
      [!ENDIF!]
      [!IF "($StartUint8ArrayLength > $Uint8ArrayMaxLength)"!]
        [!VAR "Uint8ArrayMaxLength" = "$StartUint8ArrayLength"!]
      [!ENDIF!]
      [!IF "($StopUint8ArrayLength > $Uint8ArrayMaxLength)"!][!//
        [!VAR "Uint8ArrayMaxLength" = "$StopUint8ArrayLength"!]
      [!ENDIF!]
      [!IF "($RequestResUint8ArrayLength > $Uint8ArrayMaxLength)"!]
        [!VAR "Uint8ArrayMaxLength" = "$RequestResUint8ArrayLength"!]
      [!ENDIF!]
    [!ENDIF!]
  [!ENDLOOP!]
[!//
  [!IF "(($BooleanArrayMaxLength > 0) or ($Uint8ArrayMaxLength > 0) or ($Sint8ArrayMaxLength > 0))"!]
    [!INDENT "0"!][!//
    #define DCM_START_SEC_VAR_CLEARED_8
    #include <Dcm_MemMap.h>
    [!IF "($BooleanArrayMaxLength > 0)"!]
      [!CR!]
      /* Defining global boolean buffer for API parameters used by Routine Control service */
      STATIC VAR(boolean, DCM_VAR) BooleanArray[[!"num:i($BooleanArrayMaxLength)"!]];
    [!ENDIF!]
    [!IF "($Uint8ArrayMaxLength > 0)"!]
      [!CR!]
      /* Defining global uint8 buffer for API parameters used by Routine Control service */
      STATIC VAR(uint8, DCM_VAR) Uint8Array[[!"num:i($Uint8ArrayMaxLength)"!]];
    [!ENDIF!]
    [!IF "($Sint8ArrayMaxLength > 0)"!]
      [!CR!]
      /* Defining global sint8 buffer for API parameters used by Routine Control service */
      STATIC VAR(sint8, DCM_VAR) Sint8Array[[!"num:i($Sint8ArrayMaxLength)"!]];
    [!ENDIF!]
    [!CR!]
    #define DCM_STOP_SEC_VAR_CLEARED_8
    #include <Dcm_MemMap.h>
    [!CR!]
    [!ENDINDENT!]
  [!ENDIF!]
[!//
  [!IF "(($Uint16ArrayMaxLength > 0) or ($Sint16ArrayMaxLength > 0))"!]
    [!INDENT "0"!][!//
    #define DCM_START_SEC_VAR_CLEARED_16
    #include <Dcm_MemMap.h>
    [!IF "($Uint16ArrayMaxLength > 0)"!]
      [!CR!]
      /* Defining global uint16 buffer for API parameters used by Routine Control service */
      STATIC VAR(uint16, DCM_VAR) Uint16Array[[!"num:i($Uint16ArrayMaxLength)"!]];
    [!ENDIF!]
    [!IF "($Sint16ArrayMaxLength > 0)"!]
      [!CR!]
      /* Defining global sint16 buffer for API parameters used by Routine Control service */
      STATIC VAR(sint16, DCM_VAR) Sint16Array[[!"num:i($Sint16ArrayMaxLength)"!]];
    [!ENDIF!]
    [!CR!]
    #define DCM_STOP_SEC_VAR_CLEARED_16
    #include <Dcm_MemMap.h>
    [!CR!]
    [!ENDINDENT!]
  [!ENDIF!]
[!//
  [!IF "(($Uint32ArrayMaxLength > 0) or ($Sint32ArrayMaxLength > 0))"!]
    [!INDENT "0"!][!//
    #define DCM_START_SEC_VAR_CLEARED_32
    #include <Dcm_MemMap.h>
    [!IF "($Uint32ArrayMaxLength > 0)"!]
      [!CR!]
      /* Defining global uint32 buffer for API parameters used by Routine Control service */
      STATIC VAR(uint32, DCM_VAR) Uint32Array[[!"num:i($Uint32ArrayMaxLength)"!]];
    [!ENDIF!]
    [!IF "($Sint32ArrayMaxLength > 0)"!]
      [!CR!]
      /* Defining global sint32 buffer for API parameters used by Routine Control service */
      STATIC VAR(sint32, DCM_VAR) Sint32Array[[!"num:i($Sint32ArrayMaxLength)"!]];
    [!ENDIF!]
    [!CR!]
    #define DCM_STOP_SEC_VAR_CLEARED_32
    #include <Dcm_MemMap.h>
    [!CR!]
    [!ENDINDENT!]
  [!ENDIF!]
[!ENDIF!]
/*==================[external constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[external function definitions]==============================================*/
[!IF "($DCM_DSP_USE_SERVICE_0X31 = 'STD_ON')"!]
  [!VAR "routineOperationMemorySectionIncluded" = "'false'"!][!//
  [!INDENT "0"!]
  [!/* !LINKSTO Dcm.Dsn.IB.RoutineControl.RoutineOperationGeneration,1 */!][!//
  [!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspRoutine/*"!]
    [!/* !LINKSTO SWS_Dcm_00569,1 */!][!//
    [!IF "DcmDspRoutineUsed = 'true' and not(node:exists(DcmDspRoutineTidRef))"!][!//
      [!IF "$routineOperationMemorySectionIncluded = 'false'"!]
        [!VAR "routineOperationMemorySectionIncluded" = "'true'"!][!//
        #define DCM_START_SEC_CODE
        #include <Dcm_MemMap.h>
      [!ENDIF!]
      [!CALL "ROUTINECONTROL_GENERATE", "operation" = "'Start'"!]
      [!IF "DcmDspStopRoutineSupported = 'true'"!]
        [!CALL "ROUTINECONTROL_GENERATE", "operation" = "'Stop'"!]
      [!ENDIF!]
      [!IF "DcmDspRequestResultsRoutineSupported = 'true'"!]
        [!CALL "ROUTINECONTROL_GENERATE", "operation" = "'RequestResults'"!]
      [!ENDIF!]
    [!ELSEIF "$resetOfSignalArray = 'true' or $BytesSwapNeededU16 = 'true' or $BytesSwapNeededS16 = 'true' or $BytesSwapNeededU32 = 'true' or $BytesSwapNeededS32 = 'true'"!][!//
      [!IF "$routineOperationMemorySectionIncluded = 'false'"!]
        [!VAR "routineOperationMemorySectionIncluded" = "'true'"!][!//
        #define DCM_START_SEC_CODE
        #include <Dcm_MemMap.h>
      [!ENDIF!]
    [!ENDIF!]
  [!ENDLOOP!]
  [!CR!]
  /*==================[internal function definitions]==============================================*/
  [!IF "$resetOfSignalArray = 'true'"!][!//
  [!CR!]
    STATIC FUNC(void, DCM_CODE) Dcm_Dsp_RoutineControl_ClearInternalArrays(void)
    {
      [!CR!]
      [!INDENT "2"!]
      [!IF "($BooleanArrayMaxLength > 0)"!]
        TS_MemSet(BooleanArray,0U,sizeof(BooleanArray));
        [!CR!]
      [!ENDIF!]
      [!IF "($Uint8ArrayMaxLength > 0)"!]
        TS_MemSet(Uint8Array,0U,sizeof(Uint8Array));
        [!CR!]
      [!ENDIF!]
      [!IF "($Sint8ArrayMaxLength > 0)"!]
        TS_MemSet(Sint8Array,0U,sizeof(Sint8Array));
        [!CR!]
      [!ENDIF!]
      [!IF "($Uint16ArrayMaxLength > 0)"!]
        TS_MemSet(Uint16Array,0U,sizeof(Uint16Array));
        [!CR!]
      [!ENDIF!]
      [!IF "($Sint16ArrayMaxLength > 0)"!]
        TS_MemSet(Sint16Array,0U,sizeof(Sint16Array));
        [!CR!]
      [!ENDIF!]
      [!IF "($Uint32ArrayMaxLength > 0)"!]
        TS_MemSet(Uint32Array,0U,sizeof(Uint32Array));
        [!CR!]
      [!ENDIF!]
      [!IF "($Sint32ArrayMaxLength > 0)"!]
        TS_MemSet(Sint32Array,0U,sizeof(Sint32Array));
        [!CR!]
      [!ENDIF!]
      [!ENDINDENT!]
    }
  [!ENDIF!]
  [!IF "$routineOperationMemorySectionIncluded = 'true'"!][!//
    #if (DCM_ROUTINE_ENDIANNESS_CONVERSION == STD_ON)
  [!ENDIF!]
  [!IF "$BytesSwapNeededU16 = 'true'"!][!//
  [!CR!]
    STATIC FUNC(void, DCM_CODE) Dcm_Dsp_RoutineControl_SwapBytes_U16(uint16* SignalData, uint32 SignalLength)
    {
      [!INDENT "2"!]
      uint16_least ArrayIndex;
      for(ArrayIndex = 0U; ArrayIndex < SignalLength; ArrayIndex++)
      {
      [!ENDINDENT!]
      [!INDENT "4"!]
        const uint16 TempParameter = (SignalData[ArrayIndex] >> 8U) | (SignalData[ArrayIndex] << 8U);
        SignalData[ArrayIndex] = TempParameter;
      [!ENDINDENT!]
      [!INDENT "2"!]
      }
      [!ENDINDENT!]
    }
  [!ENDIF!]

  [!IF "$BytesSwapNeededS16 = 'true'"!][!//
  [!CR!]
    STATIC FUNC(void, DCM_CODE) Dcm_Dsp_RoutineControl_SwapBytes_S16(sint16* SignalData, uint32 SignalLength)
    {
      [!INDENT "2"!]
      uint16_least ArrayIndex;
      for(ArrayIndex = 0U; ArrayIndex < SignalLength; ArrayIndex++)
      {
      [!ENDINDENT!]
      [!INDENT "4"!]
        const uint16 TempParameter = (uint16)((uint16)SignalData[ArrayIndex] >> 8U) |
                                   (uint16)((uint16)SignalData[ArrayIndex] << 8U);
        SignalData[ArrayIndex] = (sint16)TempParameter;
      [!ENDINDENT!]
      [!INDENT "2"!]
      }
      [!ENDINDENT!]
    }
  [!ENDIF!]

  [!IF "$BytesSwapNeededU32 = 'true'"!][!//
  [!CR!]
    STATIC FUNC(void, DCM_CODE) Dcm_Dsp_RoutineControl_SwapBytes_U32(uint32* SignalData, uint32 SignalLength)
    {
      [!INDENT "2"!]
      uint16_least ArrayIndex;
      for(ArrayIndex = 0U; ArrayIndex < SignalLength; ArrayIndex++)
      {
      [!ENDINDENT!]
      [!INDENT "4"!]
        const uint32 TempParameter = ((SignalData[ArrayIndex]) >> 24U) |
                                   (((SignalData[ArrayIndex]) << 8U) & 0x00ff0000U) |
                                   (((SignalData[ArrayIndex]) >> 8U) & 0x0000ff00U) |
                                   ((SignalData[ArrayIndex]) << 24U);
        SignalData[ArrayIndex] = TempParameter;
      [!ENDINDENT!]
      [!INDENT "2"!]
      }
      [!ENDINDENT!]
    }
  [!ENDIF!]

  [!IF "$BytesSwapNeededS32 = 'true'"!][!//
  [!CR!]
    STATIC FUNC(void, DCM_CODE) Dcm_Dsp_RoutineControl_SwapBytes_S32(sint32* SignalData, uint32 SignalLength)
    {
      [!INDENT "2"!]
      uint16_least ArrayIndex;
      for(ArrayIndex = 0U; ArrayIndex < SignalLength; ArrayIndex++)
      {
      [!ENDINDENT!]
      [!INDENT "4"!]
        const uint32 TempParameter = (((uint32)SignalData[ArrayIndex]) >> 24U) |
                                   ((((uint32)SignalData[ArrayIndex]) << 8U) & 0x00ff0000U) |
                                   ((((uint32)SignalData[ArrayIndex]) >> 8U) & 0x0000ff00U) |
                                   (((uint32)SignalData[ArrayIndex]) << 24U);
        SignalData[ArrayIndex] = (sint32)TempParameter;
        SignalData[ArrayIndex] = (sint32)TempParameter;
      [!ENDINDENT!]
      [!INDENT "2"!]
      }
      [!ENDINDENT!]
    }
  [!ENDIF!]
  [!ENDINDENT!]
  [!IF "$routineOperationMemorySectionIncluded = 'true'"!][!//
    [!INDENT "0"!][!//
    #endif

    #define DCM_STOP_SEC_CODE
    #include <Dcm_MemMap.h>
    [!ENDINDENT!]
  [!ENDIF!]
[!ENDIF!]
/*==================[end of file]================================================================*/
