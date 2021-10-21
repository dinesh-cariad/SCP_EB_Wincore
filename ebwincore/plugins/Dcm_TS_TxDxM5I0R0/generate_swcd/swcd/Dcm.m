[!/**
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
 */!][!//
[!//
[!/* ------------------------------------------------------------------------------------------*/!][!//
[!/* --------------- DspDataElement_Read_Write_Control_ReadScalingDataAccess macro ------------*/!][!//
[!/* ------------------------------------------------------------------------------------------*/!][!//
[!/*=== The code checks if a certain DspData element has read, read scaling data or write access ===*/!][!//
[!VAR "DcmDspDataReadAccessList" = "text:join(DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*[node:exists(node:ref(DcmDspDidInfoRef)/DcmDspDidAccess/DcmDspDidRead)]/DcmDspDidSignal/*/DcmDspDidDataRef)"!][!//
[!VAR "DcmDspDataWriteAccessList" = "text:join(DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*[node:exists(node:ref(DcmDspDidInfoRef)/DcmDspDidAccess/DcmDspDidWrite)]/DcmDspDidSignal/*/DcmDspDidDataRef)"!][!//
[!IF "(DcmConfigSet/*[1]/DcmDsp/DcmDspGenerateOnlyNeededIOControlOperations = 'false')"!][!//
  [!VAR "DcmDspDataControlAccessList" = "text:join(DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*[node:exists(node:ref(DcmDspDidInfoRef)/DcmDspDidAccess/DcmDspDidControl) and ((as:ref(DcmDspDidInfoRef)/*/DcmDspDidControl/DcmDspDidResetToDefault = 'true') or (as:ref(DcmDspDidInfoRef)/*/DcmDspDidControl/DcmDspDidFreezeCurrentState = 'true') or  (as:ref(DcmDspDidInfoRef)/*/DcmDspDidControl/DcmDspDidReturnControlToEcu = 'true') or (as:ref(DcmDspDidInfoRef)/*/DcmDspDidControl/DcmDspDidShortTermAdjustment = 'true'))]/DcmDspDidSignal/*/DcmDspDidDataRef)"!][!//
[!ELSE!][!//
  [!VAR "DcmDspDataControlDcmDspDidReturnControlToEcuList" = "text:join(DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*[node:exists(node:ref(DcmDspDidInfoRef)/DcmDspDidAccess/DcmDspDidControl) and (as:ref(DcmDspDidInfoRef)/*/DcmDspDidControl/DcmDspDidReturnControlToEcu = 'true')]/DcmDspDidSignal/*/DcmDspDidDataRef)"!][!//
  [!VAR "DcmDspDataControlDcmDspDidResetToDefaultList" = "text:join(DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*[node:exists(node:ref(DcmDspDidInfoRef)/DcmDspDidAccess/DcmDspDidControl) and (as:ref(DcmDspDidInfoRef)/*/DcmDspDidControl/DcmDspDidResetToDefault = 'true')]/DcmDspDidSignal/*/DcmDspDidDataRef)"!][!//
  [!VAR "DcmDspDataControlDcmDspDidFreezeCurrentStateList" = "text:join(DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*[node:exists(node:ref(DcmDspDidInfoRef)/DcmDspDidAccess/DcmDspDidControl) and (as:ref(DcmDspDidInfoRef)/*/DcmDspDidControl/DcmDspDidFreezeCurrentState = 'true')]/DcmDspDidSignal/*/DcmDspDidDataRef)"!][!//
  [!VAR "DcmDspDataControlDcmDspDidShortTermAdjustmentList" = "text:join(DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*[node:exists(node:ref(DcmDspDidInfoRef)/DcmDspDidAccess/DcmDspDidControl) and (as:ref(DcmDspDidInfoRef)/*/DcmDspDidControl/DcmDspDidShortTermAdjustment = 'true')]/DcmDspDidSignal/*/DcmDspDidDataRef)"!][!//
[!ENDIF!][!//
[!VAR "DcmDspDataReadScalingDataAccessList" = "text:join(DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*[node:exists(node:ref(DcmDspDidInfoRef)/DcmDspDidAccess/DcmDspDidReadScalingData)]/DcmDspDidSignal/*/DcmDspDidDataRef)"!][!//
[!//
[!MACRO "DspDataElement_Read_Write_Control_ReadScalingDataAccess"!][!//
  [!VAR "AccessDidData" = "''"!][!//
  [!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspData/*"!][!//
    [!VAR "ReadAccess" = "'FALSE'"!][!//
    [!VAR "WriteAccess" = "'FALSE'"!][!//
    [!VAR "ControlAccess" = "'FALSE'"!][!//
    [!VAR "ReturnControlToECU" = "'FALSE'"!][!//
    [!VAR "ResetToDefault" = "'FALSE'"!][!//
    [!VAR "FreezeCurrentState" = "'FALSE'"!][!//
    [!VAR "ShortTermAdjustment" = "'FALSE'"!][!//
    [!VAR "ReadScalingDataAccess" = "'FALSE'"!][!//
    [!VAR "Data" = "node:path(.)"!][!//
    [!VAR "CurrentNode_aspath" = "as:path(.,true())"!][!//
    [!/* Check that DspData element is refered in a Did, which has the read access*/!][!//
    [!IF "text:match($DcmDspDataReadAccessList,'ASPath')"!][!//
      [!IF "text:contains(text:split($DcmDspDataReadAccessList), $CurrentNode_aspath)"!][!//
        [!VAR "ReadAccess" = "'TRUE'"!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
    [!/* Check that DspData element is refered in a Did, which has the write access*/!][!//
    [!IF "text:match($DcmDspDataWriteAccessList,'ASPath')"!][!//
      [!IF "text:contains(text:split($DcmDspDataWriteAccessList), $CurrentNode_aspath)"!][!//
        [!VAR "WriteAccess" = "'TRUE'"!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
    [!/* !LINKSTO Dcm.EB.AllIOControlCSOperations.GenerateOnlyNeededOperations.FALSE,1 */!][!//
    [!IF "(../../DcmDspGenerateOnlyNeededIOControlOperations = 'false')"!][!//
      [!/* Check that DspData element is refered in a Did, which has the control access*/!][!//
      [!IF "text:match($DcmDspDataControlAccessList,'ASPath')"!][!//
        [!IF "text:contains(text:split($DcmDspDataControlAccessList), $CurrentNode_aspath)"!][!//
          [!VAR "ControlAccess" = "'TRUE'"!][!//
          [!VAR "ReturnControlToECU" = "'TRUE'"!][!//
          [!VAR "ResetToDefault" = "'TRUE'"!][!//
          [!VAR "FreezeCurrentState" = "'TRUE'"!][!//
          [!VAR "ShortTermAdjustment" = "'TRUE'"!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
    [!ELSE!][!//
      [!/* Check that DspData element is refered in a Did, which has the ReturnControlToEcu access is available */!][!//
      [!IF "text:match($DcmDspDataControlDcmDspDidReturnControlToEcuList,'ASPath')"!][!//
        [!IF "text:contains(text:split($DcmDspDataControlDcmDspDidReturnControlToEcuList), $CurrentNode_aspath)"!][!//
          [!VAR "ReturnControlToECU" = "'TRUE'"!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
      [!/* !LINKSTO Dcm.EB.ResetToDefault.CSOperationAvailable.GenerateOnlyNeededOperations.TRUE,1 */!][!//
      [!/* Check that DspData element is refered in a Did, which has the ResetToDefault access is available */!][!//
      [!IF "text:match($DcmDspDataControlDcmDspDidResetToDefaultList,'ASPath')"!][!//
        [!IF "text:contains(text:split($DcmDspDataControlDcmDspDidResetToDefaultList), $CurrentNode_aspath)"!][!//
          [!VAR "ResetToDefault" = "'TRUE'"!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
      [!/* !LINKSTO Dcm.EB.FreezeCurrentState.CSOperationAvailable.GenerateOnlyNeededOperations.TRUE,1 */!][!//
      [!/* Check that DspData element is refered in a Did, which has the FreezeCurrentState access is available */!][!//
      [!IF "text:match($DcmDspDataControlDcmDspDidFreezeCurrentStateList,'ASPath')"!][!//
        [!IF "text:contains(text:split($DcmDspDataControlDcmDspDidFreezeCurrentStateList), $CurrentNode_aspath)"!][!//
          [!VAR "FreezeCurrentState" = "'TRUE'"!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
      [!/* !LINKSTO Dcm.EB.ShortTermAdjustment.CSOperationAvailable.GenerateOnlyNeededOperations.TRUE,1 */!][!//
      [!/* Check that DspData element is refered in a Did, which has the ShortTermAdjustment access is available */!][!//
      [!IF "text:match($DcmDspDataControlDcmDspDidShortTermAdjustmentList,'ASPath')"!][!//
        [!IF "text:contains(text:split($DcmDspDataControlDcmDspDidShortTermAdjustmentList), $CurrentNode_aspath)"!][!//
          [!VAR "ShortTermAdjustment" = "'TRUE'"!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
      [!/* !LINKSTO Dcm.EB.ReturnControlToECU.CSOperationAvailable.GenerateOnlyNeededOperations.TRUE,1 */!][!//
      [!IF "($ReturnControlToECU = 'TRUE') or ($ResetToDefault = 'TRUE') or ($FreezeCurrentState = 'TRUE') or ($ShortTermAdjustment = 'TRUE')"!]
        [!VAR "ControlAccess" = "'TRUE'"!][!//
        [!VAR "ReturnControlToECU" = "'TRUE'"!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
    [!/* Check that DspData element is refered in a Did, which has the ReadScalinData access*/!][!//
    [!IF "text:match($DcmDspDataReadScalingDataAccessList,'ASPath')"!][!//
      [!IF "text:contains(text:split($DcmDspDataReadScalingDataAccessList), $CurrentNode_aspath)"!][!//
        [!VAR "ReadScalingDataAccess" = "'TRUE'"!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
    [!/* !LINKSTO Dcm.EB.Config.DcmDspDataType.Default,1 */!][!//
    [!IF "node:exists(DcmDspDataType)"!][!//
      [!VAR "DidDataType" = "./DcmDspDataType"!][!//
    [!ELSE!][!//
      [!VAR "DidDataType" = "'UINT8'"!][!//
    [!ENDIF!][!//
    [!VAR "AccessDidData" = "concat($AccessDidData,[!//
                            name(.), '|',[!//
                            $Data, '|',[!//
                            $DidDataType, '|',[!//
                            ./DcmDspDataSize, '|',[!//
                            ./DcmDspDataUsePort, '|',[!//
                            substring('Rda', 1, number($ReadAccess = 'TRUE')*3),[!//
                            substring('Wra', 1, number($WriteAccess = 'TRUE')*3),[!//
                            substring('Cna', 1, number($ControlAccess = 'TRUE')*3),[!//
                            substring('Rce', 1, number($ReturnControlToECU = 'TRUE')*3),[!//
                            substring('Rtd', 1, number($ResetToDefault = 'TRUE')*3),[!//
                            substring('Fcs', 1, number($FreezeCurrentState = 'TRUE')*3),[!//
                            substring('Sta', 1, number($ShortTermAdjustment = 'TRUE')*3),[!//
                            substring('Rsa', 1, number($ReadScalingDataAccess = 'TRUE')*3),[!//
                            ' ')"!][!//
  [!ENDLOOP!][!//
[!ENDMACRO!][!//
[!//
[!/* ------------------------------------------------------------------------------------------*/!][!//
[!/* --------------- DspDataTypeArray_Convert macro -------------------------------------------*/!][!//
[!/* ------------------------------------------------------------------------------------------*/!][!//
[!MACRO "DspDataTypeArray_Convert", "InputDataType", "DspDataName"!][!//
[!VAR "out_DspDataTypeArray_Convert_DataType" = "text:tolower(text:replace($InputDataType, '_N', ''))"!][!//
[!VAR "DspDataTypeArray_Convert_TypeName" = "concat(text:toupper(substring($out_DspDataTypeArray_Convert_DataType, 1, 1)), substring($out_DspDataTypeArray_Convert_DataType, 2))"!][!//
[!VAR "out_DspDataTypeArray_Convert_ImplementationDataTypeName" = "concat('Dcm_DataArrayType', $DspDataTypeArray_Convert_TypeName, '_', $DspDataName , 'Type')"!][!//
[!VAR "out_DspDataTypeArray_Convert_DataTypeBitLength" = "text:replace($out_DspDataTypeArray_Convert_DataType, '[a-z]*', '')"!][!//
[!ENDMACRO!][!//
[!//
[!/* ------------------------------------------------------------------------------------------*/!][!//
[!/* --------------- GetImplementationDataType macro ------------------------------------------*/!][!//
[!/* ------------------------------------------------------------------------------------------*/!][!//
[!MACRO "GetImplementationDataType", "InputDataType", "DspDataName"!][!//
[!IF "contains($InputDataType, '_N')"!][!/*  ----- ?INT*_N -------- */!][!//
[!CALL "DspDataTypeArray_Convert", "InputDataType" = "$InputDataType", "DspDataName" = "$DspDataName"!][!//
[!VAR "out_GetImplementationDataType_ImplementationDataType" = "concat('/AUTOSAR_Dcm/ImplementationDataTypes/', $out_DspDataTypeArray_Convert_ImplementationDataTypeName)"!][!//
[!ELSE!][!//
[!IF "$InputDataType = 'UINT8_DYN'"!][!/*  ----- UINT8_DYN ------ */!][!//
[!VAR "GetImplementationDataType_DataType" = "'uint8'"!][!//
[!ELSE!][!//
[!VAR "GetImplementationDataType_DataType" = "text:tolower($InputDataType)"!][!//
[!ENDIF!][!/* UINT8_DYN */!][!//
[!VAR "out_GetImplementationDataType_ImplementationDataType" = "concat('/AUTOSAR_Platform/ImplementationDataTypes/', $GetImplementationDataType_DataType)"!][!//
[!ENDIF!][!//
[!ENDMACRO!][!//
[!/* ------------------------------------------------------------------------------------------*/!][!//
[!/* --------------- GenerateRoutineFixedSignalType macro ------------------------------------------*/!][!//
[!/* ------------------------------------------------------------------------------------------*/!][!//
[!INDENT "12"!][!//
[!MACRO "GenerateRoutineFixedSignalType", "RoutineOperation", "Direction", "RoutineName", "BaseType"!][!//
  <IMPLEMENTATION-DATA-TYPE>
  [!WS "2"!]<SHORT-NAME>Dcm_[!"$RoutineOperation"!]Data[!"$Direction"!]_[!"$RoutineName"!]_[!"node:name(.)"!]Type</SHORT-NAME>
  [!WS "2"!]<CATEGORY>TYPE_REFERENCE</CATEGORY>
  [!WS "2"!]<SW-DATA-DEF-PROPS>
  [!WS "4"!]<SW-DATA-DEF-PROPS-VARIANTS>
  [!WS "6"!]<SW-DATA-DEF-PROPS-CONDITIONAL>
  [!WS "8"!]<IMPLEMENTATION-DATA-TYPE-REF DEST="IMPLEMENTATION-DATA-TYPE">[!"$TopLevelPackage"!]/ImplementationDataTypes/Dcm_[!"$RoutineOperation"!]Data[!"$Direction"!]_[!"$RoutineName"!]_[!"node:name(.)"!][!"$BaseType"!]Type</IMPLEMENTATION-DATA-TYPE-REF>
  [!WS "6"!]</SW-DATA-DEF-PROPS-CONDITIONAL>
  [!WS "4"!]</SW-DATA-DEF-PROPS-VARIANTS>
  [!WS "2"!]</SW-DATA-DEF-PROPS>
  </IMPLEMENTATION-DATA-TYPE>
[!ENDMACRO!][!//
[!ENDINDENT!][!//
[!/* ------------------------------------------------------------------------------------------*/!][!//
[!/* --------------- GenerateRoutineSignalType macro -------------------------------------------*/!][!//
[!/* ------------------------------------------------------------------------------------------*/!][!//
[!INDENT "12"!][!//
[!MACRO "GenerateRoutineSignalType", "RoutineOperation", "Direction", "RoutineName", "RoutineInfoName"!][!//
[!IF "not(DcmDspRoutineSignalType = 'VARIABLE_LENGTH')"!][!//
  [!IF "contains(DcmDspRoutineSignalType, '_N')"!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartIn.ArrayType.RoutineSignalUint8_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartIn.ArrayType.RoutineSignalUint16_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartIn.ArrayType.RoutineSignalUint32_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartIn.ArrayType.RoutineSignalSint8_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartIn.ArrayType.RoutineSignalSint16_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartIn.RoutineSignalSint32_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartOut.ArrayType.RoutineSignalUint8_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartOut.ArrayType.RoutineSignalUint16_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartOut.ArrayType.RoutineSignalUint32_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartOut.ArrayType.RoutineSignalSint8_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartOut.ArrayType.RoutineSignalSint16_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartOut.ArrayType.RoutineSignalSint32_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopIn.ArrayType.RoutineSignalUint8_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopIn.ArrayType.RoutineSignalUint16_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopIn.ArrayType.RoutineSignalUint32_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopIn.ArrayType.RoutineSignalSint8_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopIn.ArrayType.RoutineSignalSint16_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopIn.ArrayType.RoutineSignalSint32_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopOut.ArrayType.RoutineSignalUint8_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopOut.ArrayType.RoutineSignalUint16_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopOut.ArrayType.RoutineSignalUint32_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopOut.ArrayType.RoutineSignalSint8_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopOut.ArrayType.RoutineSignalSint16_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopOut.ArrayType.RoutineSignalSint32_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.RequestResIn.ArrayType.UINT8_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.RequestResIn.ArrayType.UINT16_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.RequestResIn.ArrayType.UINT32_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.RequestResIn.ArrayType.SINT8_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.RequestResIn.ArrayType.SINT16_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.RequestResIn.ArrayType.SINT32_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.RequestOut.ArrayType.RoutineSignalUint8_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.RequestOut.ArrayType.RoutineSignalUint16_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.RequestOut.ArrayType.RoutineSignalUint32_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.RequestOut.ArrayType.RoutineSignalSint8_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.RequestOut.ArrayType.RoutineSignalSint16_N,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.RequestOut.ArrayType.RoutineSignalSint32_N,1 */!][!//
    [!VAR "ArrayTypeBaseBitSize" = "num:integer(substring-before(substring-after(DcmDspRoutineSignalType, 'INT'),'_' ))"!][!//
    <IMPLEMENTATION-DATA-TYPE>
    [!WS "2"!]<SHORT-NAME>Dcm_[!"$RoutineOperation"!]Data[!"$Direction"!]_[!"$RoutineName"!]_[!"node:name(.)"!]ArrayType</SHORT-NAME>
    [!WS "2"!]<CATEGORY>ARRAY</CATEGORY>
    [!WS "2"!]<SUB-ELEMENTS>
    [!WS "4"!]<IMPLEMENTATION-DATA-TYPE-ELEMENT>
    [!WS "6"!]<SHORT-NAME>Aux_[!"$RoutineName"!]_[!"node:name(.)"!]ArrayType</SHORT-NAME>
    [!WS "6"!]<CATEGORY>TYPE_REFERENCE</CATEGORY>
    [!WS "6"!]<ARRAY-SIZE>[!"num:integer((DcmDspRoutineSignalLength + ($ArrayTypeBaseBitSize)-1) div $ArrayTypeBaseBitSize)"!]</ARRAY-SIZE>
    [!WS "6"!]<ARRAY-SIZE-SEMANTICS>FIXED-SIZE</ARRAY-SIZE-SEMANTICS>
    [!WS "6"!]<SW-DATA-DEF-PROPS>
    [!WS "8"!]<SW-DATA-DEF-PROPS-VARIANTS>
    [!WS "10"!]<SW-DATA-DEF-PROPS-CONDITIONAL>
    [!WS "12"!]<IMPLEMENTATION-DATA-TYPE-REF DEST="IMPLEMENTATION-DATA-TYPE">/AUTOSAR_Platform/ImplementationDataTypes/[!"text:tolower(text:replace(DcmDspRoutineSignalType,'_N',''))"!]</IMPLEMENTATION-DATA-TYPE-REF>
    [!WS "10"!]</SW-DATA-DEF-PROPS-CONDITIONAL>
    [!WS "8"!]</SW-DATA-DEF-PROPS-VARIANTS>
    [!WS "6"!]</SW-DATA-DEF-PROPS>
    [!WS "4"!]</IMPLEMENTATION-DATA-TYPE-ELEMENT>
    [!WS "2"!]</SUB-ELEMENTS>
    </IMPLEMENTATION-DATA-TYPE>
    [!CALL "GenerateRoutineFixedSignalType", "RoutineOperation" = "$RoutineOperation", "Direction" = "$Direction", "RoutineName" = "$RoutineName", "BaseType" = "'Array'"!][!//
  [!ELSE!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartIn.RoutineSignalBoolean,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartIn.RoutineSignalUint8,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartIn.RoutineSignalUint16,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartIn.RoutineSignalUint32,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartIn.RoutineSignalSint8,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartIn.RoutineSignalSint16,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartIn.RoutineSignalSint32,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartOut.RoutineSignalBoolean,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartOut.RoutineSignalUint8,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartOut.RoutineSignalUint16,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartOut.RoutineSignalUint32,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartOut.RoutineSignalSint8,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartOut.RoutineSignalSint16,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartOut.RoutineSignalSint32,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopIn.RoutineSignalBoolean,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopIn.RoutineSignalUint8,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopIn.RoutineSignalUint16,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopIn.RoutineSignalUint32,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopIn.RoutineSignalSint8,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopIn.RoutineSignalSint16,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopIn.RoutineSignalSint32,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopOut.RoutineSignalBoolean,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopOut.RoutineSignalUint8,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopOut.RoutineSignalUint16,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopOut.RoutineSignalUint32,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopOut.RoutineSignalSint8,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopOut.RoutineSignalSint16,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopOut.RoutineSignalSint32,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.RequestResIn.PrimitiveType.boolean,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.RequestResIn.PrimitiveType.UINT8,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.RequestResIn.PrimitiveType.UINT16,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.RequestResIn.PrimitiveType.UINT32,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.RequestResIn.PrimitiveType.SINT8,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.RequestResIn.PrimitiveType.SINT16,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.RequestResIn.PrimitiveType.SINT32,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.RequestOut.RoutineSignalBoolean,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.RequestOut.RoutineSignalUint8,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.RequestOut.RoutineSignalUint16,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.RequestOut.RoutineSignalUint32,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.RequestOut.RoutineSignalSint8,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.RequestOut.RoutineSignalSint16,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.RequestOut.RoutineSignalSint32,1 */!][!//
    <IMPLEMENTATION-DATA-TYPE>
    [!WS "2"!]<SHORT-NAME>Dcm_[!"$RoutineOperation"!]Data[!"$Direction"!]_[!"$RoutineName"!]_[!"node:name(.)"!]PrimitivType</SHORT-NAME>
    [!WS "2"!]<CATEGORY>TYPE_REFERENCE</CATEGORY>
    [!WS "2"!]<SW-DATA-DEF-PROPS>
    [!WS "4"!]<SW-DATA-DEF-PROPS-VARIANTS>
    [!WS "6"!]<SW-DATA-DEF-PROPS-CONDITIONAL>
    [!WS "8"!]<IMPLEMENTATION-DATA-TYPE-REF DEST="IMPLEMENTATION-DATA-TYPE">/AUTOSAR_Platform/ImplementationDataTypes/[!"text:tolower(DcmDspRoutineSignalType)"!]</IMPLEMENTATION-DATA-TYPE-REF>
    [!WS "6"!]</SW-DATA-DEF-PROPS-CONDITIONAL>
    [!WS "4"!]</SW-DATA-DEF-PROPS-VARIANTS>
    [!WS "2"!]</SW-DATA-DEF-PROPS>
    </IMPLEMENTATION-DATA-TYPE>
    [!CALL "GenerateRoutineFixedSignalType", "RoutineOperation" = "$RoutineOperation", "Direction" = "$Direction", "RoutineName" = "$RoutineName", "BaseType" = "'Primitiv'"!][!//
    [!/* Even though the Dcm uses ASR 4.3 routine types, generate also ASR 4.2 types for application units which might them */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.RequestOut.RoutineSignalBoolean.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.RequestOut.RoutineSignalUint8.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.RequestOut.RoutineSignalUint16.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.RequestOut.RoutineSignalUint32.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.RequestOut.RoutineSignalSint8.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.RequestOut.RoutineSignalSint16.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.RequestOut.RoutineSignalSint32.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartIn.RoutineSignalBoolean.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartIn.RoutineSignalUint8.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartIn.RoutineSignalUint16.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartIn.RoutineSignalUint32.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartIn.RoutineSignalSint8.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartIn.RoutineSignalSint16.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartIn.RoutineSignalSint32.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartOut.RoutineSignalBoolean.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartOut.RoutineSignalUint8.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartOut.RoutineSignalUint16.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartOut.RoutineSignalUint32.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartOut.RoutineSignalSint8.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartOut.RoutineSignalSint16.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StartOut.RoutineSignalSint32.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopIn.RoutineSignalBoolean.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopIn.RoutineSignalUint8.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopIn.RoutineSignalUint16.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopIn.RoutineSignalUint32.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopIn.RoutineSignalSint8.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopIn.RoutineSignalSint16.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopIn.RoutineSignalSint32.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopOut.RoutineSignalBoolean.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopOut.RoutineSignalUint8.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopOut.RoutineSignalUint16.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopOut.RoutineSignalUint32.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopOut.RoutineSignalSint8.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopOut.RoutineSignalSint16.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.DcmDspRoutineSignalType.StopOut.RoutineSignalSint32.ASR42,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.RequestResIn.ASR42.boolean,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.RequestResIn.ASR42.uint8,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.RequestResIn.ASR42.uint16,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.RequestResIn.ASR42.uint32,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.RequestResIn.ASR42.sint8,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.RequestResIn.ASR42.sint16,1 */!][!//
    [!/* !LINKSTO Dcm.EB.RoutineControl.RequestResIn.ASR42.sint32,1 */!][!//
    <IMPLEMENTATION-DATA-TYPE>
    [!WS "2"!]<SHORT-NAME>[!"$RoutineOperation"!]Data[!"$Direction"!]Type_[!"$RoutineName"!]_[!"node:name(.)"!]</SHORT-NAME>
    [!WS "2"!]<CATEGORY>TYPE_REFERENCE</CATEGORY>
    [!WS "2"!]<SW-DATA-DEF-PROPS>
    [!WS "4"!]<SW-DATA-DEF-PROPS-VARIANTS>
    [!WS "6"!]<SW-DATA-DEF-PROPS-CONDITIONAL>
    [!WS "8"!]<IMPLEMENTATION-DATA-TYPE-REF DEST="IMPLEMENTATION-DATA-TYPE">/AUTOSAR_Platform/ImplementationDataTypes/[!"text:tolower(DcmDspRoutineSignalType)"!]</IMPLEMENTATION-DATA-TYPE-REF>
    [!WS "6"!]</SW-DATA-DEF-PROPS-CONDITIONAL>
    [!WS "4"!]</SW-DATA-DEF-PROPS-VARIANTS>
    [!WS "2"!]</SW-DATA-DEF-PROPS>
    </IMPLEMENTATION-DATA-TYPE>
  [!ENDIF!][!//
[!ELSE!][!//
  [!/* !LINKSTO Dcm.EB.RoutineControl.StartFlexibleInArrayDataType,1 */!][!//
  [!/* !LINKSTO Dcm.EB.RoutineControl.StartFlexibleOutArrayDataType,1 */!][!//
  [!/* !LINKSTO Dcm.EB.RoutineControl.StopFlexibleInArrayDataType,1 */!][!//
  [!/* !LINKSTO Dcm.EB.RoutineControl.StopFlexibleOutArrayDataType,1 */!][!//
  [!/* !LINKSTO Dcm.EB.RoutineControl.RequestFlexibleInArrayDataType,1 */!][!//
  [!/* !LINKSTO Dcm.EB.RoutineControl.RequestFlexibleOutArrayDataType,1 */!][!//
  <IMPLEMENTATION-DATA-TYPE>
  [!WS "2"!]<SHORT-NAME>Dcm_[!"$RoutineOperation"!]Flexible[!"$Direction"!]ArrayData_[!"$RoutineName"!]_[!"node:name(.)"!]Type</SHORT-NAME>
  [!WS "2"!]<CATEGORY>ARRAY</CATEGORY>
  [!WS "2"!]<SUB-ELEMENTS>
  [!WS "4"!]<IMPLEMENTATION-DATA-TYPE-ELEMENT>
  [!WS "6"!]<SHORT-NAME>AuxU8_Dcm_[!"$RoutineOperation"!]Flexible[!"$Direction"!]ArrayData_[!"$RoutineName"!]_[!"node:name(.)"!]Type</SHORT-NAME>
  [!WS "6"!]<CATEGORY>TYPE_REFERENCE</CATEGORY>
  [!WS "6"!]<ARRAY-SIZE>[!"num:integer((DcmDspRoutineSignalLength + 7) div 8)"!]</ARRAY-SIZE>
  [!WS "6"!]<ARRAY-SIZE-SEMANTICS>FIXED-SIZE</ARRAY-SIZE-SEMANTICS>
  [!WS "6"!]<SW-DATA-DEF-PROPS>
  [!WS "8"!]<SW-DATA-DEF-PROPS-VARIANTS>
  [!WS "10"!]<SW-DATA-DEF-PROPS-CONDITIONAL>
  [!WS "12"!]<IMPLEMENTATION-DATA-TYPE-REF DEST="IMPLEMENTATION-DATA-TYPE">/AUTOSAR_Platform/ImplementationDataTypes/uint8</IMPLEMENTATION-DATA-TYPE-REF>
  [!WS "10"!]</SW-DATA-DEF-PROPS-CONDITIONAL>
  [!WS "8"!]</SW-DATA-DEF-PROPS-VARIANTS>
  [!WS "6"!]</SW-DATA-DEF-PROPS>
  [!WS "4"!]</IMPLEMENTATION-DATA-TYPE-ELEMENT>
  [!WS "2"!]</SUB-ELEMENTS>
  </IMPLEMENTATION-DATA-TYPE>
  [!/* Generate also the old type for backwards compatibility */!][!//
  <IMPLEMENTATION-DATA-TYPE>
  [!WS "2"!]<SHORT-NAME>[!"$RoutineInfoName"!]_[!"name(../.)"!]_ArrayType</SHORT-NAME>
  [!WS "2"!]<CATEGORY>ARRAY</CATEGORY>
  [!WS "2"!]<SUB-ELEMENTS>
  [!WS "4"!]<IMPLEMENTATION-DATA-TYPE-ELEMENT>
  [!WS "6"!]<SHORT-NAME>AuxU8_[!"$RoutineInfoName"!]_[!"name(../.)"!]_ArrayType</SHORT-NAME>
  [!WS "6"!]<CATEGORY>TYPE_REFERENCE</CATEGORY>
  [!WS "6"!]<ARRAY-SIZE>[!"num:integer((DcmDspRoutineSignalLength + 7) div 8)"!]</ARRAY-SIZE>
  [!WS "6"!]<ARRAY-SIZE-SEMANTICS>FIXED-SIZE</ARRAY-SIZE-SEMANTICS>
  [!WS "6"!]<SW-DATA-DEF-PROPS>
  [!WS "8"!]<SW-DATA-DEF-PROPS-VARIANTS>
  [!WS "10"!]<SW-DATA-DEF-PROPS-CONDITIONAL>
  [!WS "12"!]<IMPLEMENTATION-DATA-TYPE-REF DEST="IMPLEMENTATION-DATA-TYPE">/AUTOSAR_Platform/ImplementationDataTypes/uint8</IMPLEMENTATION-DATA-TYPE-REF>
  [!WS "10"!]</SW-DATA-DEF-PROPS-CONDITIONAL>
  [!WS "8"!]</SW-DATA-DEF-PROPS-VARIANTS>
  [!WS "6"!]</SW-DATA-DEF-PROPS>
  [!WS "4"!]</IMPLEMENTATION-DATA-TYPE-ELEMENT>
  [!WS "2"!]</SUB-ELEMENTS>
  </IMPLEMENTATION-DATA-TYPE>
  [!/* Even though the Dcm uses ASR 4.3 routine types, generate also ASR 4.2 types for application units which might them */!][!//
  [!/* !LINKSTO Dcm.EB.RoutineControl.RequestFlexibleOutArrayDataType.ASR42,1 */!][!//
  [!/* !LINKSTO Dcm.EB.RoutineControl.RequestFlexibleInArrayDataType.ASR42,1 */!][!//
  [!/* !LINKSTO Dcm.EB.RoutineControl.StartFlexibleInArrayDataType.ASR42,1 */!][!//
  [!/* !LINKSTO Dcm.EB.RoutineControl.StartFlexibleOutArrayDataType.ASR42,1 */!][!//
  [!/* !LINKSTO Dcm.EB.RoutineControl.StopFlexibleOutArrayDataType.ASR42,1 */!][!//
  [!/* !LINKSTO Dcm.EB.RoutineControl.StopFlexibleInArrayDataType.ASR42,1 */!][!//
  <IMPLEMENTATION-DATA-TYPE>
  [!WS "2"!]<SHORT-NAME>[!"$RoutineOperation"!]Flexible[!"$Direction"!]ArrayDataType_[!"$RoutineName"!]_[!"node:name(.)"!]</SHORT-NAME>
  [!WS "2"!]<CATEGORY>ARRAY</CATEGORY>
  [!WS "2"!]<SUB-ELEMENTS>
  [!WS "4"!]<IMPLEMENTATION-DATA-TYPE-ELEMENT>
  [!WS "6"!]<SHORT-NAME>AuxU8_[!"$RoutineOperation"!]Flexible[!"$Direction"!]ArrayDataType_[!"$RoutineName"!]_[!"node:name(.)"!]</SHORT-NAME>
  [!WS "6"!]<CATEGORY>TYPE_REFERENCE</CATEGORY>
  [!WS "6"!]<ARRAY-SIZE>[!"num:integer((DcmDspRoutineSignalLength + 7) div 8)"!]</ARRAY-SIZE>
  [!WS "6"!]<ARRAY-SIZE-SEMANTICS>FIXED-SIZE</ARRAY-SIZE-SEMANTICS>
  [!WS "6"!]<SW-DATA-DEF-PROPS>
  [!WS "8"!]<SW-DATA-DEF-PROPS-VARIANTS>
  [!WS "10"!]<SW-DATA-DEF-PROPS-CONDITIONAL>
  [!WS "12"!]<IMPLEMENTATION-DATA-TYPE-REF DEST="IMPLEMENTATION-DATA-TYPE">/AUTOSAR_Platform/ImplementationDataTypes/uint8</IMPLEMENTATION-DATA-TYPE-REF>
  [!WS "10"!]</SW-DATA-DEF-PROPS-CONDITIONAL>
  [!WS "8"!]</SW-DATA-DEF-PROPS-VARIANTS>
  [!WS "6"!]</SW-DATA-DEF-PROPS>
  [!WS "4"!]</IMPLEMENTATION-DATA-TYPE-ELEMENT>
  [!WS "2"!]</SUB-ELEMENTS>
  </IMPLEMENTATION-DATA-TYPE>
[!ENDIF!][!//
[!ENDMACRO!][!//
[!ENDINDENT!][!//
[!//
