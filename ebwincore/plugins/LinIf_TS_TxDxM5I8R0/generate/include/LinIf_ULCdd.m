[!/**
 * \file
 *
 * \brief AUTOSAR LinIf
 *
 * This file contains the implementation of the AUTOSAR
 * module LinIf.
 *
 * \version 5.8.27
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!NOCODE!]
[!IF "not(var:defined('LINIF_ULCDD_M'))"!]
[!VAR "LINIF_ULCDD_M"="'true'"!]
[!VAR "ulCddSupported" = "node:value(as:modconf('LinIf')/LinIfGeneral/LinIfUpperLayerCddSupported)"!]
[!VAR "slaveSupported" = "node:exists(as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*[LinIfNodeType = 'LinIfSlave'])"!]

[!/*
****************************************************************************************************
* MACRO to obtain GotoSleepConfirmation functions.
* Entries separated by a pipe.
* The number of entries is in numLinIf_GotoSleepConfFuncs.
* Result is in LinIf_GotoSleepConfFuncs.
****************************************************************************************************
*/!]
[!MACRO "GetGotoSleepConfFuncs"!][!NOCODE!]
[!VAR "numLinIf_GotoSleepConfFuncs" = "num:i($idxFuncGotoSleep)"!]
[!VAR "LinIf_GotoSleepConfFuncs"!][!CR!][!ENDVAR!]
[!LOOP "text:split($funcsGotoSleep, ';')"!]
  [!VAR "LinIf_GotoSleepConfFuncs"!][!"$LinIf_GotoSleepConfFuncs"!][!"concat('  &', ., ',')"!][!CR!][!ENDVAR!]
[!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO to obtain GotoSleepIndication functions.
* Entries separated by a pipe.
* The number of entries is in numLinIf_GotoSleepIndFuncs.
* Result is in LinIf_GotoSleepIndFuncs.
****************************************************************************************************
*/!]
[!MACRO "GetGotoSleepIndFuncs"!][!NOCODE!]
[!VAR "numLinIf_GotoSleepIndFuncs" = "num:i($idxFuncGotoSleepInd)"!]
[!VAR "LinIf_GotoSleepIndFuncs"!][!CR!][!ENDVAR!]
[!LOOP "text:split($funcsGotoSleepInd, ';')"!]
  [!VAR "LinIf_GotoSleepIndFuncs"!][!"$LinIf_GotoSleepIndFuncs"!][!"concat('  &', ., ',')"!][!CR!][!ENDVAR!]
[!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO to obtain WakeupConfirmation functions.
* Entries are separated by a pipe.
* The number of entries is in numLinIf_WuConfFuncs.
* Result is in LinIf_WuConfFuncs
****************************************************************************************************
*/!]
[!MACRO "GetWuConfFuncs"!][!NOCODE!]
[!VAR "numLinIf_WuConfFuncs" = "num:i($idxFuncWuConf)"!]
[!VAR "LinIf_WuConfFuncs"!][!CR!][!ENDVAR!]
[!LOOP "text:split($funcsWuConf, ';')"!]
  [!VAR "LinIf_WuConfFuncs"!][!"$LinIf_WuConfFuncs"!][!"concat('  &', ., ',')"!][!CR!][!ENDVAR!]
[!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO to obtain ScheduleRequestConfirmation functions.
* Entries are separated by a pipe.
* The number of entries is in numLinIf_SchedReqConfFuncs.
* Result is in LinIf_SchedReqConfFuncs.
****************************************************************************************************
*/!]
[!MACRO "GetSchedReqConfFuncs"!][!NOCODE!]
[!VAR "numLinIf_SchedReqConfFuncs" = "num:i($idxFuncSchedReqConf)"!]
[!VAR "LinIf_SchedReqConfFuncs"!][!CR!][!ENDVAR!]
[!LOOP "text:split($funcsSchedReqConf, ';')"!]
  [!VAR "LinIf_SchedReqConfFuncs"!][!"$LinIf_SchedReqConfFuncs"!][!"concat('  &', ., ',')"!][!CR!][!ENDVAR!]
[!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO used to enumerate state handling functions (LinSM and Cdds)
* Entries consist of LinIfChannel AS paths as keys and func idx tuples.
* Key separator is colon, tuple value separator is semicolon, entry separator is pipe.
****************************************************************************************************
*/!]
[!MACRO "EnumerateStateHandleFuncs"!][!NOCODE!]
[!VAR "modnameLinSM" = "'LinSM'"!]
[!VAR "chanStateFuncs" = "''"!]
[!VAR "funcNameGotoSleepConf" = "'_GotoSleepConfirmation'"!]
[!VAR "funcNameSchedReqConf" = "'_ScheduleRequestConfirmation'"!]
[!VAR "funcNameWuConf" = "'_WakeupConfirmation'"!]
[!VAR "funcNameGotoSleepInd" = "'_GotoSleepIndication'"!]
[!/* The ones from LinSM as default values */!]
[!VAR "funcsGotoSleep" = "concat($modnameLinSM, $funcNameGotoSleepConf)"!]
[!VAR "funcsSchedReqConf" = "concat($modnameLinSM, $funcNameSchedReqConf)"!]
[!VAR "funcsWuConf" = "concat($modnameLinSM, $funcNameWuConf)"!]
[!VAR "funcsGotoSleepInd" = "concat($modnameLinSM, $funcNameGotoSleepInd)"!]
[!LOOP "node:order(as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*, 'LinIfChannelId')"!]
  [!IF "node:exists(./LinIfCddRef)"!]
    [!CALL "GetModuleName", "CddConfRef" = "./LinIfCddRef"!]
  [!ELSE!]
    [!VAR "moduleName" = "$modnameLinSM"!]
  [!ENDIF!]
  [!VAR "modNameAssoc" = "concat('LIN_SM:', $modnameLinSM, '|CDD:', $moduleName)"!]
  [!VAR "cbkGotoSleep" = "concat(substring-after(text:grep(text:split($modNameAssoc, '|'), concat('^', node:value(LinIfGotoSleepConfirmationUL), '.+'))[1], ':'), $funcNameGotoSleepConf)"!]
  [!VAR "cbkSchedReqConf" = "concat(substring-after(text:grep(text:split($modNameAssoc, '|'), concat('^', node:value(LinIfScheduleRequestConfirmationUL), '.+'))[1], ':'), $funcNameSchedReqConf)"!]
  [!VAR "cbkWuConf" = "concat(substring-after(text:grep(text:split($modNameAssoc, '|'), concat('^', node:value(LinIfWakeupConfirmationUL), '.+'))[1], ':'), $funcNameWuConf)"!]
  [!IF "$slaveSupported = 'true'"!]
    [!IF "LinIfNodeType = 'LinIfSlave'"!]
      [!VAR "cbkGotoSleepInd" = "concat(substring-after(text:grep(text:split($modNameAssoc, '|'), concat('^', node:value(LinIfGotoSleepIndicationUL), '.+'))[1], ':'), $funcNameGotoSleepInd)"!]
    [!ELSE!]
      [!VAR "cbkGotoSleepInd" = "concat(substring-after(text:grep(text:split($modNameAssoc, '|'), concat('^', 'NULL_PTR', '.+'))[1], ':'), 'NULL_PTR')"!]
    [!ENDIF!]
    [!IF "not(contains($funcsGotoSleepInd, $cbkGotoSleepInd))"!]
      [!VAR "funcsGotoSleepInd" = "concat($funcsGotoSleepInd, ';', $cbkGotoSleepInd)"!]
    [!ENDIF!]
  [!ELSE!]
    [!VAR "cbkGotoSleepInd" = "''"!]
  [!ENDIF!]
  [!IF "not(contains($funcsGotoSleep, $cbkGotoSleep))"!]
    [!VAR "funcsGotoSleep" = "concat($funcsGotoSleep, ';', $cbkGotoSleep)"!]
  [!ENDIF!]
  [!IF "not(contains($funcsSchedReqConf, $cbkSchedReqConf))"!]
    [!VAR "funcsSchedReqConf" = "concat($funcsSchedReqConf, ';', $cbkSchedReqConf)"!]
  [!ENDIF!]
  [!IF "not(contains($funcsWuConf, $cbkWuConf))"!]
    [!VAR "funcsWuConf" = "concat($funcsWuConf, ';', $cbkWuConf)"!]
  [!ENDIF!]
[!VAR "chanStateFuncs" = "concat($chanStateFuncs, as:path(.), ':', $cbkGotoSleep, ';', $cbkSchedReqConf, ';', $cbkWuConf, ';', $cbkGotoSleepInd, '|')"!]
[!ENDLOOP!][!//LinIfChannel
[!/* Ordered, better chances for LT-PB LUT match */!]
[!VAR "idxFuncGotoSleep" = "num:i(0)"!]
[!VAR "funcsGotoSleep" = "text:join(text:order(text:split($funcsGotoSleep, ';')), ';')"!]
[!VAR "funcsSchedReqConf" = "text:join(text:order(text:split($funcsSchedReqConf, ';')), ';')"!]
[!VAR "funcsWuConf" = "text:join(text:order(text:split($funcsWuConf, ';')), ';')"!]
[!LOOP "text:split($funcsGotoSleep, ';')"!]
  [!/* No (access to the) loop counter */!]
  [!VAR "chanStateFuncs" = "text:replaceAll($chanStateFuncs, string(.), string($idxFuncGotoSleep))"!]
  [!VAR "idxFuncGotoSleep" = "num:i($idxFuncGotoSleep + 1)"!]
[!ENDLOOP!]
[!IF "$slaveSupported = 'true'"!]
  [!VAR "idxFuncGotoSleepInd" = "num:i(0)"!]
  [!LOOP "text:split($funcsGotoSleepInd, ';')"!]
    [!VAR "chanStateFuncs" = "text:replaceAll($chanStateFuncs, string(.), string($idxFuncGotoSleepInd))"!]
    [!VAR "idxFuncGotoSleepInd" = "num:i($idxFuncGotoSleepInd + 1)"!]
  [!ENDLOOP!]
[!ENDIF!]
[!VAR "idxFuncSchedReqConf" = "num:i(0)"!]
[!LOOP "text:split($funcsSchedReqConf, ';')"!]
  [!VAR "chanStateFuncs" = "text:replaceAll($chanStateFuncs, string(.), string($idxFuncSchedReqConf))"!]
  [!VAR "idxFuncSchedReqConf" = "num:i($idxFuncSchedReqConf + 1)"!]
[!ENDLOOP!]
[!VAR "idxFuncWuConf" = "num:i(0)"!]
[!LOOP "text:split($funcsWuConf, ';')"!]
  [!VAR "chanStateFuncs" = "text:replaceAll($chanStateFuncs, string(.), string($idxFuncWuConf))"!]
  [!VAR "idxFuncWuConf" = "num:i($idxFuncWuConf + 1)"!]
[!ENDLOOP!]
[!/* Poor man's map-reduce */!]
[!VAR "chanStateFuncsReduced" = "$chanStateFuncs"!]
[!VAR "funcMap" = "''"!]
[!VAR "idxMap" = "num:i(0)"!]
[!LOOP "text:split($chanStateFuncs, '|')"!]
  [!VAR "fncs" = "text:split(., ':')[2]"!]
  [!IF "not(contains($funcMap, $fncs))"!]
    [!VAR "funcMap" = "concat($funcMap, $fncs)"!]
    [!VAR "chanStateFuncsReduced" = "text:replaceAll($chanStateFuncsReduced, $fncs, $idxMap)"!]
    [!VAR "idxMap" = "num:i($idxMap + 1)"!]
  [!ENDIF!]
[!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO to obtain the state handling function association array.
* Array size is in numLinIf_StateHandFuncAssocArray.
* Result is in LinIf_StateHandFuncAssocArray.
****************************************************************************************************
*/!]
[!MACRO "GetStateHandFuncAssocArray", "LeadingWS"="10"!][!NOCODE!]
[!IF "not(var:defined('LinIf_StateHandFuncAssocArray'))"!]
  [!VAR "LinIf_StateHandFuncAssocArray"!][!CR!][!WS "$LeadingWS"!]<field>[!CR!][!ENDVAR!]
  [!VAR "numLinIf_StateHandFuncAssocArray" = "num:i(count(text:split($chanStateFuncs, '|')))"!]
  [!LOOP "text:split($chanStateFuncs, '|')"!]
    [!VAR "assocEntry" = "text:split(., ':')[2]"!]
    [!VAR "LinIf_StateHandFuncAssocArray"!][!"$LinIf_StateHandFuncAssocArray"!][!WS "$LeadingWS+2"!]<field>[!CR!][!WS "$LeadingWS+4"!]<field>[!CR!][!WS "$LeadingWS+6"!]<int>[!"text:split($assocEntry, ';')[1]"!]</int>[!CR!][!WS "$LeadingWS+4"!]</field>[!CR!][!WS "$LeadingWS+4"!]<field>[!CR!][!WS "$LeadingWS+6"!]<int>[!"text:split($assocEntry, ';')[2]"!]</int>[!CR!][!WS "$LeadingWS+4"!]</field>[!CR!][!WS "$LeadingWS+4"!]<field>[!CR!][!WS "$LeadingWS+6"!]<int>[!"text:split($assocEntry, ';')[3]"!]</int>[!CR!][!WS "$LeadingWS+4"!]</field>[!CR!][!IF "$slaveSupported = 'true'"!][!WS "$LeadingWS+4"!]<field>[!CR!][!WS "$LeadingWS+6"!]<int>[!"text:split($assocEntry, ';')[4]"!]</int>[!CR!][!WS "$LeadingWS+4"!]</field>[!CR!][!ENDIF!][!WS "$LeadingWS+2"!]</field>[!CR!][!ENDVAR!]
  [!ENDLOOP!]
  [!VAR "LinIf_StateHandFuncAssocArray"!][!"$LinIf_StateHandFuncAssocArray"!][!WS "$LeadingWS"!]</field>[!CR!][!ENDVAR!]
[!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO to obtain the state handling function association array index
* Parameter is an AS path of a LinIfChannel
* Result is in LinIf_StateHandFuncIdx
****************************************************************************************************
*/!]
[!MACRO "GetStateHandFuncIdx","LinIfChanRef"!][!NOCODE!]
[!VAR "LinIf_StateHandFuncIdx" = "num:i(text:split(text:grep(text:split($chanStateFuncsReduced, '|'), concat($LinIfChanRef, ':\d+')), ':')[2])"!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO used to enumerate pdu handling functions (PduR and Cdds)
* Entries consist of LinIfFrame AS paths as keys and func idx tuples.
* Only frames with LinIfFrameType = "UNCONDITIONAL" are considered.
****************************************************************************************************
*/!]
[!MACRO "EnumeratePduHandleFuncs"!][!NOCODE!]
[!VAR "pduHandleFuncs" = "''"!]
[!VAR "funcsRxInd" = "'PduR_LinIfRxIndication'"!]
[!VAR "funcsTT" = "'PduR_LinIfTriggerTransmit'"!]
[!VAR "funcsTxConf" = "'PduR_LinIfTxConfirmation'"!]
[!VAR "funcPrefixPduR" = "'PduR_LinIf'"!]
[!VAR "fieldDirectionFuncAssoc" = "'LinIfRxPdu:LinIfUserRxIndicationUL>RxIndication:LinIfRxIndicationUL;|LinIfTxPdu:LinIfUserTxUL>TriggerTransmit:LinIfTxTriggerTransmitUL;TxConfirmation:LinIfTxConfirmationUL'"!]
[!LOOP "as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*/LinIfFrame/*[LinIfFrameType='UNCONDITIONAL']"!]
  [!VAR "funcRxIndication" = "''"!]
  [!VAR "funcTriggerTransmit" = "''"!]
  [!VAR "funcTxConfirmation" = "''"!]
  [!VAR "funcResolve" = "substring-after(text:grep(text:split($fieldDirectionFuncAssoc, '|'), concat('^', ./LinIfPduDirection, '.+'))[1], ':')"!]
  [!VAR "type" = "node:value(node:ref(concat(node:path(.), '/LinIfPduDirection/', substring-before($funcResolve, '>'))))"!]
  [!VAR "funcs" = "substring-after($funcResolve, '>')"!]
  [!FOR "f" = "1" TO "num:i(count(text:split($funcs, ';')))"!]
    [!VAR "apiResolve" = "text:split($funcs, ';')[position() = $f]"!]
    [!VAR "ulApi" = "text:split($apiResolve, ':')[1]"!]
    [!VAR "ulApiField" = "text:split($apiResolve, ':')[2]"!]
    [!"var:set(concat('func', $ulApi), node:when('PDUR' = $type, concat($funcPrefixPduR, $ulApi), node:fallback("->as:modconf('LinIf')[1]/LinIfGeneral/LinIfCddFunctionsUL/*[@name=node:value(node:ref(concat(node:path(node:current()), '/LinIfPduDirection/', $ulApiField)))]/CddFunctionName", "nopathWrapper")))"!]
  [!ENDFOR!]
[!IF "not(contains($funcsRxInd, $funcRxIndication))"!]
  [!VAR "funcsRxInd" = "concat($funcsRxInd, ';', $funcRxIndication)"!]
[!ENDIF!]
[!IF "not(contains($funcsTT, $funcTriggerTransmit))"!]
  [!VAR "funcsTT" = "concat($funcsTT, ';', $funcTriggerTransmit)"!]
[!ENDIF!]
[!IF "not(contains($funcsTxConf, $funcTxConfirmation))"!]
  [!VAR "funcsTxConf" = "concat($funcsTxConf, ';', $funcTxConfirmation)"!]
[!ENDIF!]
[!VAR "pduHandleFuncs" = "concat($pduHandleFuncs, as:path(.), ':', $funcRxIndication, ';', $funcTriggerTransmit, ';', $funcTxConfirmation, '|')"!]
[!ENDLOOP!]
[!VAR "funcsRxInd" = "text:join(text:order(text:split($funcsRxInd, ';')), ';')"!]
[!VAR "funcsTT" = "text:join(text:order(text:split($funcsTT, ';')), ';')"!]
[!VAR "funcsTxConf" = "text:join(text:order(text:split($funcsTxConf, ';')), ';')"!]
[!VAR "idxFuncRxInd" = "num:i(0)"!]
before: [!"$pduHandleFuncs"!]
[!LOOP "text:split($funcsRxInd, ';')"!]
  [!VAR "pduHandleFuncs" = "text:replaceAll($pduHandleFuncs, string(.), string($idxFuncRxInd))"!]
  [!VAR "idxFuncRxInd" = "num:i($idxFuncRxInd + 1)"!]
[!ENDLOOP!]
[!VAR "idxFuncTT" = "num:i(0)"!]
[!LOOP "text:split($funcsTT, ';')"!]
  [!VAR "pduHandleFuncs" = "text:replaceAll($pduHandleFuncs, string(.), string($idxFuncTT))"!]
  [!VAR "idxFuncTT" = "num:i($idxFuncTT + 1)"!]
[!ENDLOOP!]
[!VAR "idxFuncTxConf" = "num:i(0)"!]
[!LOOP "text:split($funcsTxConf, ';')"!]
  [!VAR "pduHandleFuncs" = "text:replaceAll($pduHandleFuncs, string(.), string($idxFuncTxConf))"!]
  [!VAR "idxFuncTxConf" = "num:i($idxFuncTxConf + 1)"!]
[!ENDLOOP!]
[!VAR "pduHandleFuncsReduced" = "$pduHandleFuncs"!]
[!VAR "funcMapPdu" = "''"!]
[!VAR "idx" = "num:i(0)"!]
[!LOOP "text:split($pduHandleFuncs, '|')"!]
  [!VAR "fncs" = "text:split(., ':')[2]"!]
  [/* RxInds are indexed directly, the rest through an intermediary array */]
  [!IF "not(contains($funcMapPdu, $fncs))"!]
    [!VAR "funcMapPdu" = "concat($funcMapPdu, $fncs, '|')"!]
    [!IF "text:match($fncs, '\d+;;')"!]
      [!VAR "idxD" = "substring-before($fncs, ';')"!]
    [!ELSE!]
      [!VAR "idxD" = "$idx"!]
      [!VAR "idx" = "num:i($idx + 1)"!]
    [!ENDIF!]
    [!VAR "pduHandleFuncsReduced" = "text:replaceAll($pduHandleFuncsReduced, $fncs, string($idxD))"!]
  [!ENDIF!]
[!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO to obtain RxIndication functions.
* Entries are separated by a pipe.
* The number of entries is in numLinIf_RxIndFuncs;
* Result is in LinIf_RxIndFuncs
****************************************************************************************************
*/!]
[!MACRO "GetRxIndFuncs"!][!NOCODE!]
[!VAR "numLinIf_RxIndFuncs" = "num:i($idxFuncRxInd)"!]
[!VAR "LinIf_RxIndFuncs"!][!CR!][!ENDVAR!]
[!LOOP "text:split($funcsRxInd, ';')"!]
  [!VAR "LinIf_RxIndFuncs"!][!"$LinIf_RxIndFuncs"!][!"concat('  &', ., ',')"!][!CR!][!ENDVAR!]
[!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO to obtain TriggerTransmit functions.
* Entries are separated by a pipe.
* The number of entries is in numLinIf_TrgTxFuncs;
* Result is in LinIf_TrgTxFuncs
****************************************************************************************************
*/!]
[!MACRO "GetTrgTxFuncs"!][!NOCODE!]
[!VAR "numLinIf_TrgTxFuncs" = "num:i($idxFuncTT)"!]
[!VAR "LinIf_TrgTxFuncs"!][!CR!][!ENDVAR!]
[!LOOP "text:split($funcsTT, ';')"!]
  [!VAR "LinIf_TrgTxFuncs"!][!"$LinIf_TrgTxFuncs"!][!"concat('  &', ., ',')"!][!CR!][!ENDVAR!]
[!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO to obtain TxConfirmation functions.
* Entries are separated by a pipe.
* The number of entries is in numLinIf_TxConfFuncs;
* Result is in LinIf_TxConfFuncs
****************************************************************************************************
*/!]
[!MACRO "GetTxConfFuncs"!][!NOCODE!]
[!VAR "numLinIf_TxConfFuncs" = "num:i($idxFuncTxConf)"!]
[!VAR "LinIf_TxConfFuncs"!][!CR!][!ENDVAR!]
[!LOOP "text:split($funcsTxConf, ';')"!]
  [!VAR "LinIf_TxConfFuncs"!][!"$LinIf_TxConfFuncs"!][!"concat('  &', ., ',')"!][!CR!][!ENDVAR!]
[!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO to obtain the Pdu handling function association array index
* Parameter is an AS path of a LinIfFrame
* Result is in LinIf_StateHandFuncIdx
****************************************************************************************************
*/!]
[!MACRO "GetPduHandFuncIdx","LinIfFrameRef"!][!NOCODE!]
[!VAR "LinIf_PduHandFuncIdx" = "num:i(text:split(text:grep(text:split($pduHandleFuncsReduced, '|'), concat($LinIfFrameRef, ':\d+')), ':')[2])"!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO to obtain the Pdu handling function association array.
* Array size is in numLinIf_PduHandFuncAssocArray.
* Result is in LinIf_PduHandFuncAssocArray.
****************************************************************************************************
*/!]
[!MACRO "GetPduHandFuncAssocArray", "LeadingWS"="10"!][!NOCODE!]
[!VAR "LinIf_PduHandFuncAssocArray"!][!CR!][!WS "$LeadingWS"!]<field>[!CR!][!ENDVAR!]
[!VAR "numLinIf_PduHandFuncAssocArray" = "num:i(count(text:grep(text:split($funcMapPdu, '|'), '^;\d+;\d+')))"!]
[!LOOP "text:grep(text:split($funcMapPdu, '|'), '^;\d+;\d+')"!]
  [!VAR "LinIf_PduHandFuncAssocArray"!][!"$LinIf_PduHandFuncAssocArray"!][!WS "$LeadingWS+2"!]<field>[!CR!][!WS "$LeadingWS+4"!]<field>[!CR!][!WS "$LeadingWS+6"!]<int>[!"text:split(., ';')[1]"!]</int>[!CR!][!WS "$LeadingWS+4"!]</field>[!CR!][!WS "$LeadingWS+4"!]<field>[!CR!][!WS "$LeadingWS+6"!]<int>[!"text:split(., ';')[2]"!]</int>[!CR!][!WS "$LeadingWS+4"!]</field>[!CR!][!WS "$LeadingWS+2"!]</field>[!CR!][!ENDVAR!]
[!ENDLOOP!]
[!VAR "LinIf_PduHandFuncAssocArray"!][!"$LinIf_PduHandFuncAssocArray"!][!WS "$LeadingWS"!]</field>[!CR!][!ENDVAR!]
[!ENDNOCODE!][!ENDMACRO!]

[!ENDIF!][!// Include guard
[!ENDNOCODE!][!//
