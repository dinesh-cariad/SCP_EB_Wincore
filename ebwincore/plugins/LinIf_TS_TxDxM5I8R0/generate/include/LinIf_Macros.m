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
[!/* *** multiple inclusion protection ***
*/!][!IF "not(var:defined('LINIF_MACROS_M'))"!][!/*
*/!][!VAR "LINIF_MACROS_M"="'true'"!][!/*
*/!][!INCLUDE "LinIf_CddSupport.m"!][!/*
*** set some Variables ***
*/!][!VAR "numChannelsMaster" = "num:i(count(as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*[LinIfNodeType = 'LinIfMaster']))"!][!/*
*/!][!VAR "numSlaveChannels" = "num:i(count(as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*[LinIfNodeType = 'LinIfSlave']))"!][!/*
*/!][!VAR "numChannels"="$numChannelsMaster + $numSlaveChannels"!][!/*
*/!][!VAR "numSlaveFrames"="num:i(count(as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*[LinIfNodeType = 'LinIfSlave']/LinIfFrame/*[LinIfFrameType != 'MRF' and LinIfFrameType != 'SRF']))"!][!/*
*/!][!VAR "numSchTables"="num:i(count(as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*/LinIfScheduleTable/*[@name != 'NULL_SCHEDULE']))"!][!/*
*/!][!VAR "numEntries"="num:i(count(as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*/LinIfScheduleTable/*[@name != 'NULL_SCHEDULE']/LinIfEntry/*))"!][!/*
*/!][!VAR "numSubsFrames"="num:i(count(as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*/LinIfFrame/*[LinIfFrameType='SPORADIC']/LinIfSubstitutionFrames/*/LinIfSubstitutionFrameRef))"!][!/*
*/!][!VAR "numFixedFrames"="num:i(count(as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*/LinIfFrame/*/LinIfFixedFrameSdu/*))"!][!/*
*/!][!VAR "numRxPdus"="count(as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*/LinIfFrame/*[LinIfFrameType='UNCONDITIONAL' and LinIfPduDirection='LinIfRxPdu'])"!][!/*
*/!][!VAR "numTxPdus"="count(as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*/LinIfFrame/*[LinIfFrameType='UNCONDITIONAL' and LinIfPduDirection='LinIfTxPdu'])"!][!/*
*/!][!VAR "numTpTxPdus"="count(as:modconf('LinTp')[1]/LinTpGlobalConfig/*[1]/LinTpTxNSdu/*)"!][!/*
*/!][!VAR "numTpRxPdus"="count(as:modconf('LinTp')[1]/LinTpGlobalConfig/*[1]/LinTpRxNSdu/*)"!][!/*
*/!][!VAR "numEvTrgd"="count(as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*/LinIfFrame/*[LinIfFrameType='EVENT_TRIGGERED'])"!][!/*
*/!][!VAR "numSlaveSubstFrames"="count(as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*[LinIfNodeType = 'LinIfSlave']/LinIfFrame/*[LinIfFrameType='EVENT_TRIGGERED']/LinIfSubstitutionFrames/*/LinIfSubstitutionFrameRef)"!][!/*
*/!][!VAR "numSporadic"="count(as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*/LinIfFrame/*[LinIfFrameType='SPORADIC'])"!][!/*
*/!][!VAR "numS2S"="count(as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*/LinIfFrame/*[LinIfFrameType='UNCONDITIONAL' and LinIfPduDirection='LinIfSlaveToSlavePdu'])"!][!/*
*/!][!VAR "numConfig"="count(as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*/LinIfFrame/*[count(LinIfFixedFrameSdu/*) > 0])"!][!/*
*/!][!VAR "numFramesMax" = "num:i(sum(as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*[LinIfNodeType = 'LinIfSlave']/LinIfMaxFrameCnt))"!][!/*
*/!][!VAR "numTrcvs"="num:i(count(node:refs(as:modconf('LinIf')/LinIfGlobalConfig/*[1]/LinIfChannel/*/LinIfTransceiverDrvConfig/LinIfTrcvIdRef)/../..))"!][!/*
*/!][!VAR "trcvDoApiInfix" = "as:modconf('LinIf')/LinIfGeneral/LinIfSingleLinTrcvAPIInfixEnable = 'true' or as:modconf('LinIf')/LinIfGeneral/LinIfMultipleTrcvDriverSupported = 'true'"!][!/*
*/!][!VAR "cwuNeeded" = "count(as:modconf('LinTrcv')/LinTrcvGeneral[LinTrcvWakeUpSupport='true']) > 0"!][!/*
*/!][!VAR "useDriverInfix" = "as:modconf('LinIf')[1]/LinIfGeneral/LinIfDriverAPIInfixEnable = 'true'"!][!/*
*/!][!VAR "multipleDriverSupport" = "as:modconf('LinIf')[1]/LinIfGeneral/LinIfMultipleDriversSupported = 'true'"!][!/*
*/!][!VAR "frameListSeps" = "''"!][!/*
*** some easy query macros ***
*/!][!MACRO "getRxFrameRefAndSrcPduHandleId", "index"!][!/*
   */!][!"text:grep(text:split($rxPduLUT,';'), concat('^', $index, '+.*'))[1]"!][!/*
*/!][!ENDMACRO!][!/*
*/!][!MACRO "getRxIndexByFrameRef", "frameRef"!][!/*
   */!][!"substring-before(text:grep(text:split($rxPduLUT,';'), concat('.*-', $frameRef, '$'))[1], '+')"!][!/*
*/!][!ENDMACRO!][!/*
*/!][!MACRO "getTxFrameRefAndDestPduHandleId", "sduId"!][!/*
   */!][!"text:grep(text:split($txPduLUT,';'), concat('^', $sduId, '+.*'))[1]"!][!/*
*/!][!ENDMACRO!][!/*
*/!][!MACRO "getTxSduIdByFrameRef", "frameRef"!][!/*
   */!][!"substring-before(text:grep(text:split($txPduLUT,';'), concat('.*-', $frameRef, '$'))[1], '+')"!][!/*
*/!][!ENDMACRO!][!/*
*/!][!MACRO "getSporadicIdxByFrameRef", "frameRef"!][!/*
   */!][!"substring-before(text:grep(text:split($sporadicLUT,';'), concat('.*-', $frameRef, '$'))[1], '-')"!][!/*
*/!][!ENDMACRO!][!/*
*/!][!MACRO "getEventIdxByFrameRef", "frameRef"!][!/*
   */!][!"substring-before(text:grep(text:split($eventLUT,';'), concat('.*-', $frameRef, '$'))[1], '-')"!][!/*
*/!][!ENDMACRO!][!/*
*/!][!MACRO "getConfigIdxByFrameRef", "frameRef"!][!/*
   */!][!"substring-before(text:grep(text:split($configLUT,';'), concat('.*-', $frameRef, '$'))[1], '-')"!][!/*
*/!][!ENDMACRO!][!/*
*/!][!MACRO "getS2SIdxByFrameRef", "frameRef"!][!/*
   */!][!"substring-before(text:grep(text:split($s2sLUT,';'), concat('.*-', $frameRef, '$'))[1], '-')"!][!/*
*/!][!ENDMACRO!][!/*
*/!][!/*
*** Check if Dem events are configured ***
*/!][!VAR "areDemEventsConfigured" = "num:i(1)"!][!/*
*/!][!LOOP "as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*/LinIfFrame/*"!][!/*
*/!][!IF "node:exists("LinIfFrameDemEventParameterRefs/LINIF_E_TX_BIT_ERROR") or node:exists("LinIfFrameDemEventParameterRefs/LINIF_E_RX_CHECKSUM_ERROR")  or node:exists("LinIfFrameDemEventParameterRefs/LINIF_E_RX_NO_RESPONSE_ERROR")"!][!/*
*/!][!VAR "areDemEventsConfigured" = "num:i(0)"!][!/*
*/!][!BREAK!][!/*
*/!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*
/*==================[ macros ]=============================================*/[!/*
*** Create RX-PDU Lookup Table ***
*/!][!VAR "rxPduLUT" = "''"!][!/*
*/!][!VAR "RxframeSeps" = "''"!][!/*
*/!][!VAR "idx" = "num:i(0)"!][!/*
*/!][!FOR "A"="1" TO "$numChannels"!][!/*
   */!][!VAR "RxframeSeps"!][!"$RxframeSeps"!][!"num:i($idx)"!];[!ENDVAR!][!/*
   */!][!LOOP "as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*[num:i($A)]/LinIfFrame/*[LinIfFrameType='UNCONDITIONAL' and LinIfPduDirection='LinIfRxPdu']"!][!/*
      */!][!IF "./LinIfPduDirection/LinIfUserRxIndicationUL = 'PDUR'"!][!/*
        */!][!VAR "rxSrcPduHandleId"="as:modconf('PduR')[1]/PduRRoutingTables/*/PduRRoutingTable/*/PduRRoutingPath/*/PduRSrcPdu[PduRSrcPduRef = node:current()/LinIfPduDirection/LinIfRxPduRef]/PduRSourcePduHandleId"!][!/*
        */!][!VAR "assocCntr" = "'PduRSrcPdu'"!][!/*
      */!][!ELSE!][!/*
        */!][!VAR "assocCdd" = "as:modconf('LinIf')[1]/LinIfGeneral/LinIfCddFunctionsUL/*[@name=node:current()/LinIfPduDirection/LinIfRxIndicationUL]/CddName"!][!/*
        */!][!VAR "assocCntr" = "'CddComIfUpperLayerRxPdu'"!][!/*
        <!-- LINKSTO LinIf.EB.LinIfCddRef.Contribution.HandleID.PduRSurrogate,1 -->
        */!][!VAR "rxSrcPduHandleId" = "as:ref(text:grep(as:paths(as:modconf('Cdd')[node:exists(CddComStackContribution)]), concat('^/', $assocCdd, '/Cdd'))[1])/CddComStackContribution/CddComIfUpperLayerContribution/CddComIfUpperLayerRxPdu/*[CddComIfPduRef = node:current()/LinIfPduDirection/LinIfRxPduRef]/CddComIfHandleId"!][!/*
      */!][!ENDIF!][!/*
      */!][!IF "$rxSrcPduHandleId=''"!][!ERROR!] The unconditional Rx frame '[!"node:name(.)"!]' does not have an associated [!"$assocCntr"!] container referenced by the same EcuC reference. [!ENDERROR!][!ENDIF!][!/*
      */!][!VAR "rxPduLUT"!][!"$rxPduLUT"!];[!"num:i($idx)"!]+[!"$rxSrcPduHandleId"!]-[!"as:path(.)"!][!ENDVAR!][!/*
      */!][!VAR "idx" = "$idx + 1"!][!/*
   */!][!ENDLOOP!][!/*
*/!][!ENDFOR!][!/*
*/!][!VAR "rxPduLUT" = "substring-after($rxPduLUT,';')"!][!/*
*/!][!VAR "RxframeSeps"!][!"$RxframeSeps"!][!"num:i($idx)"!];[!ENDVAR!][!/*
*** Create TX-PDU Lookup Table ***
*/!][!VAR "txPduLUT" = "''"!][!/*
*/!][!LOOP "as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*/LinIfFrame/*[LinIfFrameType='UNCONDITIONAL' and LinIfPduDirection='LinIfTxPdu']"!][!/*
      */!][!IF "./LinIfPduDirection/LinIfUserTxUL = 'PDUR'"!][!/*
        */!][!VAR "txDestPduHandleId" = "as:modconf('PduR')[1]/PduRRoutingTables/*/PduRRoutingTable/*/PduRRoutingPath/*/PduRDestPdu/*[PduRDestPduRef = node:current()/LinIfPduDirection/LinIfTxPduRef]/PduRDestPduHandleId"!][!/*
        */!][!VAR "assocCntr" = "'PduRDestPdu'"!][!/*
      */!][!ELSE!][!/*
        */!][!VAR "assocCdd" = "as:modconf('LinIf')[1]/LinIfGeneral/LinIfCddFunctionsUL/*[@name=node:current()/LinIfPduDirection/LinIfTxTriggerTransmitUL]/CddName"!][!/*
        <!-- !LINKSTO LinIf.EB.LinIfCddRef.Contribution.HandleID.PduRSurrogate,1 -->
        */!][!VAR "txDestPduHandleId" = "as:ref(text:grep(as:paths(as:modconf('Cdd')[node:exists(CddComStackContribution)]), concat('^/', $assocCdd, '/Cdd'))[1])/CddComStackContribution/CddComIfUpperLayerContribution/CddComIfUpperLayerTxPdu/*[CddComIfPduRef = node:current()/LinIfPduDirection/LinIfTxPduRef]/CddComIfHandleId"!][!/*
        */!][!VAR "assocCntr" = "'CddComIfUpperLayerTxPdu'"!][!/*
      */!][!ENDIF!][!/*
   */!][!IF "$txDestPduHandleId=''"!][!ERROR!] The unconditional Tx frame '[!"node:name(.)"!]' does not have an associated [!"$assocCntr"!] container referenced by the same EcuC reference. [!ENDERROR!][!ENDIF!][!/*
   */!][!VAR "txPduLUT"!][!"$txPduLUT"!];[!"./LinIfPduDirection/LinIfTxPduId"!]+[!"$txDestPduHandleId"!]-[!"as:path(.)"!][!ENDVAR!][!/*
*/!][!ENDLOOP!][!/*
*/!][!VAR "txPduLUT" = "substring-after($txPduLUT,';')"!][!/*

*/!][!MACRO "TimeoutToTicks", "Time"!][!/*
   */!][!VAR "TimeoutToTicks_Time_Us" = "$Time * 1000000"!][!/*
   */!][!VAR "TimeoutToTicks_LinTimeBase_Us" = "as:modconf( 'LinIf' )[1]/LinIfGlobalConfig/*[1]/LinIfTimeBase * 1000000"!][!/*
   */!][!VAR "TimeoutToTicks_Remainder" = "num:i($TimeoutToTicks_Time_Us mod $TimeoutToTicks_LinTimeBase_Us)"!][!/*
   */!][!IF "$TimeoutToTicks_Remainder != 0"!][!/*
      */!][!"num:i(floor($TimeoutToTicks_Time_Us div $TimeoutToTicks_LinTimeBase_Us)+1)"!][!/*
      */!][!WARNING "A timeout is not a multiple of the main function cycle time. The number of ticks for the timeout will be round up."!][!/*
   */!][!ELSE!][!/*
      */!][!"num:i(floor($TimeoutToTicks_Time_Us div $TimeoutToTicks_LinTimeBase_Us))"!][!/*
   */!][!ENDIF!][!/*
*/!][!ENDMACRO!][!/*
*/!][!/*
*/!][!NOCODE!][!/*
****************************************************************************************************
* Create a list of frames that have DemEvents configured and give every frame an index
* to be able to later point to the Dem Events in a strucrure. Also keeps the Dem Event Ids.
****************************************************************************************************
*/!][!VAR "txError" = "''"!][!/*
*/!][!VAR "rxChecksumError" = "''"!][!/*
*/!][!VAR "rxNoResponseError" = "''"!][!/*
*/!][!VAR "framesWithDemEvents" = "''"!][!/*
*/!][!VAR "demEventsStructLastIndex"="0"!][!/*
*/!][!LOOP "as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*/LinIfFrame/*"!][!/*
*/!][!IF "node:exists("LinIfFrameDemEventParameterRefs/LINIF_E_TX_BIT_ERROR") or node:exists("LinIfFrameDemEventParameterRefs/LINIF_E_RX_CHECKSUM_ERROR")  or node:exists("LinIfFrameDemEventParameterRefs/LINIF_E_RX_NO_RESPONSE_ERROR")"!][!/*
*/!][!IF "node:exists("LinIfFrameDemEventParameterRefs/LINIF_E_TX_BIT_ERROR")"!][!/*
*/!][!VAR "txError" = "node:value(as:ref(LinIfFrameDemEventParameterRefs/LINIF_E_TX_BIT_ERROR)/DemEventId)"!][!/*
*/!][!ELSE!][!/*
*/!][!VAR "txError" = "num:i(0)"!][!/*
*/!][!ENDIF!][!/*
*/!][!IF "node:exists("LinIfFrameDemEventParameterRefs/LINIF_E_RX_CHECKSUM_ERROR")"!][!/*
*/!][!VAR "rxChecksumError" = "node:value(as:ref(LinIfFrameDemEventParameterRefs/LINIF_E_RX_CHECKSUM_ERROR)/DemEventId)"!][!/*
*/!][!ELSE!][!/*
*/!][!VAR "rxChecksumError" = "num:i(0)"!][!/*
*/!][!ENDIF!][!/*
*/!][!IF "node:exists("LinIfFrameDemEventParameterRefs/LINIF_E_RX_NO_RESPONSE_ERROR")"!][!/*
*/!][!VAR "rxNoResponseError" = "node:value(as:ref(LinIfFrameDemEventParameterRefs/LINIF_E_RX_NO_RESPONSE_ERROR)/DemEventId)"!][!/*
*/!][!ELSE!][!/*
*/!][!VAR "rxNoResponseError" = "num:i(0)"!][!/*
*/!][!ENDIF!][!/*
*/!][!VAR "framesWithDemEvents"!][!"$framesWithDemEvents"!][!"node:path(.)"!],[!"num:i($demEventsStructLastIndex)"!],[!"$txError"!],[!"$rxChecksumError"!],[!"$rxNoResponseError"!];[!ENDVAR!][!/*
*/!][!VAR "demEventsStructLastIndex" = "$demEventsStructLastIndex + 1"!][!/*
*/!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*
*/!][!VAR "removeLastChar" = "num:i(string-length($framesWithDemEvents)-1)"!][!/*
*/!][!"substring($framesWithDemEvents,1,$removeLastChar)"!][!/*
****************************************************************************************************
* MACRO to get the index of the DemEvents structure element for a certain frame.
****************************************************************************************************
*/!][!MACRO "frameGetDemEventsIndex", "frameRef"!][!/*
*/!][!VAR "frameDemEventIndex"="num:i(65535)"!][!/*
*/!][!SELECT "as:ref($frameRef)"!][!/*
*/!][!VAR "currentFrame"="node:path(.)"!][!/*
*/!][!LOOP "text:split($framesWithDemEvents,';')"!][!/*
*/!][!IF "$currentFrame = string(text:split(.,',')[1])"!][!/*
*/!][!VAR "frameDemEventIndex"="num:i(string(text:split(.,',')[2]))"!][!/*
*/!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*
*/!][!ENDSELECT!][!/*
*/!][!ENDMACRO!][!/*
****************************************************************************************************
* MACRO to get driver info(path, id, vendorId, vendorInfix).
 "driver" the driver we need the info from.
 "onlyInfix" if this parameter is true() only the driver vendorId and vendorApiInfix will
 be returned based on the "index" parameter in this format: vendorId_vendorApiInfix (Ex: 1_Eb1),
 otherwise the whole information will be returned(Ex: "/AUTOSAR/TOP-LEVEL-PACKAGES/Lin 0 0 Eb0")
****************************************************************************************************
*/!][!/*
*/!][!MACRO "getDriverInfo","driver","onlyInfix","index"!][!/*
*/!][!VAR "deleteWhiteSpace" = "false()"!][!/*
*/!][!LOOP "text:split($linDrivers,',')"!][!/*
*/!][!IF "$deleteWhiteSpace = 'false'"!][!/*
*/!][!VAR "driverInfo" = "."!][!/*
*/!][!ELSE!][!/*
*/!][!VAR "driverInfo" = "substring(.,2)"!][!/*
*/!][!ENDIF!][!/*
*/!][!IF "$onlyInfix = 'true'"!][!/*
*/!][!IF "string(num:i($index)) = string(text:split($driverInfo)[2])"!][!/*
*/!][!CODE!][!/*
*/!][!"string(text:split($driverInfo)[3])"!][!/*
*/!][!ENDCODE!][!/*
*/!][!ENDIF!][!/*
*/!][!ELSE!][!/*
*/!][!IF "$driver = string(text:split($driverInfo)[1])"!][!/*
*/!][!CODE!][!/*
*/!][!"$driverInfo"!][!/*
*/!][!ENDCODE!][!/*
*/!][!ENDIF!][!/*
*/!][!ENDIF!][!/*
*/!][!VAR "deleteWhiteSpace" = "true()"!][!/*
*/!][!ENDLOOP!][!/*
*/!][!ENDMACRO!][!/*
*/!][!MACRO "addToDriverList","myDriver", "vendorId", "vendorInfix"!][!/*
*/!][!IF "text:contains(text:split($linDrivers),$currentDriver)"!][!/*
*/!][!VAR "channelAndDriversList"!][!"$channelAndDriversList"!][!"node:path(.)"!] [!CALL "getDriverInfo","driver" = "$currentDriver", "onlyInfix" = "false()", "index" = "0"!];[!ENDVAR!][!/*
*/!][!ELSE!][!/*
*/!][!VAR "linDrivers"!][!"$linDrivers"!][!"$currentDriver"!] [!"num:i($idx)"!] [!"$vendorId"!]_[!"$vendorInfix"!], [!ENDVAR!][!/*
*/!][!VAR "channelAndDriversList"!][!"$channelAndDriversList"!][!"node:path(.)"!] [!CALL "getDriverInfo","driver" = "$currentDriver", "onlyInfix" = "false()", "index" = "0"!];[!ENDVAR!][!/*
*/!][!VAR "idx" = "$idx + 1"!][!/*
*/!][!ENDIF!][!/*
*/!][!ENDMACRO!][!/*
*/!][!/*
****************************************************************************************************
* Create Lin Driver info list: Driver-DriverIdx-VendorId-VendorApiInfix:
 A list is created containing all driver paths, index (new index provided), vendorId, and
 vendorApiInfix. Ex:
 "/AUTOSAR/TOP-LEVEL-PACKAGES/Lin 0 0 Eb0, /AUTOSAR/TOP-LEVEL-PACKAGES/Lin_1 1 1_Eb1,"
 Create Lin Driver Channel-Driver-DriverIdx-VendorId-VendorApiInfix asscociation list:
 A new list is created where each channel referanced by LinIf in the drivers is associeted
 a driver from above. Ex:
 "/AUTOSAR/TOP-LEVEL-PACKAGES/Lin/ELEMENTS/Lin/LinGlobalConfig/LinChannel/LinChannel_0 "
 "/AUTOSAR/TOP-LEVEL-PACKAGES/Lin 0 0_Eb0"
****************************************************************************************************
*/!][!/*
*/!][!VAR "linDrivers" = "''"!][!/*
*/!][!VAR "channelAndDriversList" = "''"!][!/*
*/!][!VAR "idx" = "0"!][!/*
*/!][!VAR "vInfix" = "''"!][!/*
*/!][!LOOP "as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*"!][!/*
*/!][!VAR "ChannelRef"="node:path(as:ref(LinIfChannelRef))"!][!/*
*/!][!VAR "BswmdRef"="''"!][!/*
*/!][!IF "node:exists(as:modconf('LinIf')[1]/LinIfEbGeneral/LinIfEbGeneralBswmdImplementation/LinIfEbGeneralBswmdImplementationRefs)"!][!/*
  */!][!LOOP "as:modconf('LinIf')[1]/LinIfEbGeneral/LinIfEbGeneralBswmdImplementation/LinIfEbGeneralBswmdImplementationRefs/*"!][!/*
    */!][!IF "text:contains(string($ChannelRef), string(node:path(as:ref(LinIfDrvTrcvRef))))"!][!/*
      */!][!VAR "BswmdRef"="node:path(as:ref(LinIfDrvTrcvBswImplementationRef))"!][!/*
      */!][!VAR "vInfix"!][!"asc:getVendorApiInfix(as:path($BswmdRef))"!][!ENDVAR!][!/*
    */!][!ENDIF!][!/*
  */!][!ENDLOOP!][!/*
*/!][!ENDIF!][!/*

*/!][!IF "$BswmdRef != ''"!][!/*
  */!][!VAR "vId" = "''"!][!/*
  <!-- !LINKSTO LinIf.EB.DriverVendorInfo.BSWMDProvides.CheckVendorApiInfix.TRUE,1 -->
  */!][!IF "$vInfix != ''"!][!/*
    */!][!VAR "vId" = "asc:getVendorId(as:path($BswmdRef))"!][!/*
    */!][!IF "as:modconf('LinIf')[1]/LinIfGeneral/LinIfLinDriverAPI = 'REV42' or as:modconf('LinIf')[1]/LinIfGeneral/LinIfLinDriverAPI = 'REV431' or as:modconf('LinIf')[1]/LinIfGeneral/LinIfLinDriverAPI = 'REV44'"!][!/*
      */!][!LOOP "as:modconf('Lin')/LinGlobalConfig/LinChannel/*"!][!/*
        */!][!IF "$ChannelRef = node:path(.)"!][!/*
          */!][!VAR "currentDriver" = "node:path(./../../../../..)"!][!/*
          */!][!CALL "addToDriverList","myDriver" = "$currentDriver", "vendorId" = "$vId", "vendorInfix" = "$vInfix"!][!/*
        */!][!ENDIF!][!/*
      */!][!ENDLOOP!][!/*
    */!][!ELSE!][!/*
      */!][!LOOP "as:modconf('Lin')/LinGlobalConfig/*[1]/LinChannel/*"!][!/*
        */!][!IF "$ChannelRef = node:path(.)"!][!/*
          */!][!VAR "currentDriver" = "node:path(./../../../../../..)"!][!/*
          */!][!CALL "addToDriverList","myDriver" = "$currentDriver", "vendorId" = "$vId", "vendorInfix" = "$vInfix"!][!/*
        */!][!ENDIF!][!/*
      */!][!ENDLOOP!][!/*
    */!][!ENDIF!][!/*

  */!][!ENDIF!][!/*

*/!][!ENDIF!][!/*

*/!][!IF "$vInfix = '' or $BswmdRef = ''"!][!/*
  <!-- !LINKSTO LinIf.EB.DriverVendorInfo.BSWMDProvides.CheckVendorApiInfix.FALSE,1 -->
  <!-- !LINKSTO LinIf.EB.DriverVendorInfo.ConfigProvides.NoError,1 -->
  */!][!IF "as:modconf('LinIf')[1]/LinIfGeneral/LinIfLinDriverAPI = 'REV42' or as:modconf('LinIf')[1]/LinIfGeneral/LinIfLinDriverAPI = 'REV431' or as:modconf('LinIf')[1]/LinIfGeneral/LinIfLinDriverAPI = 'REV44'"!][!/*
    */!][!LOOP "as:modconf('Lin')/LinGlobalConfig/LinChannel/*"!][!/*
      */!][!IF "$ChannelRef = node:path(.)"!][!/*
      */!][!VAR "currentDriver" = "node:path(./../../../../..)"!][!/*
      <!-- !LINKSTO LinIf.EB.DriverVendorInfo.ConfigProvides.ThrowError,1 -->
      */!][!IF "not(node:exists(./../../../CommonPublishedInformation))"!][!/*
      <!-- This can only be covered by manual testing because a generic test would not work with no common published information -->
      */!][!ERROR!]Driver vendor information not available![!ENDERROR!][!/*
      */!][!ELSE!][!/*
      */!][!VAR "vId" = "node:value(node:path(./../../../CommonPublishedInformation/VendorId))"!][!/*
      */!][!VAR "vInfix"!][!IF "node:exists(./../../../CommonPublishedInformation/VendorApiInfix)"!][!"node:value(node:path(./../../../CommonPublishedInformation/VendorApiInfix))"!][!ENDIF!][!ENDVAR!][!/*
      */!][!CALL "addToDriverList","myDriver" = "$currentDriver", "vendorId" = "$vId", "vendorInfix" = "$vInfix"!][!/*
      */!][!ENDIF!][!/*
      */!][!ENDIF!][!/*
    */!][!ENDLOOP!][!/*
  */!][!ELSE!][!/*
    */!][!LOOP "as:modconf('Lin')/LinGlobalConfig/*[1]/LinChannel/*"!][!/*
      */!][!IF "$ChannelRef = node:path(.)"!][!/*
      */!][!VAR "currentDriver" = "node:path(./../../../../../..)"!][!/*
      <!-- !LINKSTO LinIf.EB.DriverVendorInfo.ConfigProvides.ThrowError,1 -->
      */!][!IF "not(node:exists(./../../../../CommonPublishedInformation))"!][!/*
      <!-- This can only be covered by manual testing because a generic test would not work with no common published information -->
      */!][!ERROR!]Driver vendor information not available![!ENDERROR!][!/*
      */!][!ELSE!][!/*
      */!][!VAR "vId" = "node:value(node:path(./../../../../CommonPublishedInformation/VendorId))"!][!/*
      */!][!VAR "vInfix"!][!IF "node:exists(./../../../../CommonPublishedInformation/VendorApiInfix)"!][!"node:value(node:path(./../../../../CommonPublishedInformation/VendorApiInfix))"!][!ENDIF!][!ENDVAR!][!/*
      */!][!CALL "addToDriverList","myDriver" = "$currentDriver", "vendorId" = "$vId", "vendorInfix" = "$vInfix"!][!/*
      */!][!ENDIF!][!/*
      */!][!ENDIF!][!/*
    */!][!ENDLOOP!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*
*/!][!VAR "numberOfDrivers" = "$idx"!][!/*
*/!][!ENDNOCODE!][!/*
 ====================[Build frame lists internal data]=======================
*/!][!VAR "sporadicLUT" = "''"!][!/*
*/!][!VAR "sporadicUCs" = "':'"!][!/*
*/!][!VAR "frameListSeps" = "''"!][!/*
*/!][!VAR "idx" = "0"!][!/*
*/!][!VAR "fidx" = "0"!][!/*
*/!][!IF "$numSporadic = 0"!][!VAR "fidx" = "0"!][!ELSE!][!/*
   */!][!VAR "fidx" = "0"!][!/*
   */!][!LOOP "LinIfGlobalConfig/*[1]/LinIfChannel/*/LinIfFrame/*[LinIfFrameType='SPORADIC']"!][!/*
      */!][!VAR "frameListSeps"!][!"$frameListSeps"!][!"num:i($idx)"!];[!ENDVAR!][!/*
      */!][!VAR "sporadicLUT"!][!"$sporadicLUT"!];[!"num:i($fidx)"!]-[!"as:path(.)"!][!ENDVAR!][!/*
      */!][!LOOP "node:order(LinIfSubstitutionFrames/*/LinIfSubstitutionFrameRef, '../LinIfFramePriority')"!][!/*
         */!][!IF "as:ref(.)/LinIfFrameType = 'UNCONDITIONAL'"!][!/*
            */!][!VAR "sporadicUCs"!][!"$sporadicUCs"!][!"substring-after(., 'ASPath:')"!]:[!ENDVAR!][!/*
            */!][!VAR "pduId"!][!CALL "getTxSduIdByFrameRef", "frameRef" = "substring-after(., 'ASPath:')"!][!ENDVAR!][!/*
            */!][!VAR "idx" = "$idx + 1"!][!/*
         */!][!ENDIF!][!/*
      */!][!ENDLOOP!][!/*
      */!][!VAR "fidx" = "$fidx + 1"!][!/*
   */!][!ENDLOOP!][!/*
*/!][!ENDIF!][!/*
*/!][!VAR "frameListSeps"!][!"$frameListSeps"!][!"num:i($idx)"!][!ENDVAR!][!/*
*/!][!VAR "sporadicLUT" = "substring-after($sporadicLUT, ';')"!][!/*

*/!][!VAR "eventLUT" = "''"!][!/*
*/!][!VAR "fidx" = "0"!][!/*
*/!][!IF "$numEvTrgd = 0"!][!VAR "fidx" = "0"!][!ELSE!][!/*
   */!][!VAR "fidx" = "0"!][!/*
   */!][!LOOP "LinIfGlobalConfig/*[1]/LinIfChannel/*/LinIfFrame/*[LinIfFrameType='EVENT_TRIGGERED']"!][!/*
      */!][!VAR "eventLUT"!][!"$eventLUT"!];[!"num:i($fidx)"!]-[!"as:path(.)"!][!ENDVAR!][!/*
      */!][!VAR "fidx" = "$fidx + 1"!][!/*
   */!][!ENDLOOP!][!/*
*/!][!ENDIF!][!/*
*/!][!VAR "eventLUT" = "substring-after($eventLUT, ';')"!][!/*
*/!][!VAR "numFrameListSeps" = "num:i(count(text:split($frameListSeps, ';')))"!][!/*
*/!][!VAR "numRxFrameSeps" = "num:i(count(text:split($RxframeSeps, ';')))"!][!/*

*/!][!IF "$numSlaveSubstFrames > 0"!][!/*
*/!][!VAR "slaveETSeps" = "''"!][!/*
*/!][!VAR "slaveETSepsStartIdx" = "0"!][!/*
*/!][!LOOP "node:order(as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*[LinIfNodeType = 'LinIfSlave'], 'LinIfChannelId')"!][!/*
  */!][!VAR "slCountSubst" = "count(LinIfFrame/*[LinIfFrameType='EVENT_TRIGGERED']/LinIfSubstitutionFrames/*/LinIfSubstitutionFrameRef)"!][!/*
  */!][!IF "$slCountSubst > 0"!][!/*
    */!][!VAR "slaveETSeps" = "concat($slaveETSeps, ';', num:i($slaveETSepsStartIdx), ':', num:i($slaveETSepsStartIdx + $slCountSubst))"!][!/*
    */!][!VAR "slaveETSepsStartIdx" = "$slaveETSepsStartIdx + $slCountSubst"!][!/*
    */!][!ELSE!][!/*
    */!][!VAR "slaveETSeps" = "concat($slaveETSeps, ';0:0')"!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*
*/!][!VAR "numSlaveETFrameListSeps" = "count(text:split($slaveETSeps, ';'))"!][!/*
*/!][!ENDIF!][!/*

*/!][!VAR "configLUT" = "''"!][!/*
*/!][!IF "$numConfig != 0"!][!/*
  */!][!VAR "idx" = "0"!][!/*
  */!][!LOOP "LinIfGlobalConfig/*[1]/LinIfChannel/*/LinIfFrame/*[count(LinIfFixedFrameSdu/*) > 0]"!][!/*
    */!][!VAR "configLUT"!][!"$configLUT"!];[!"num:i($idx)"!]-[!"as:path(.)"!][!ENDVAR!][!/*
    */!][!VAR "idx" = "$idx + 1"!][!/*
 */!][!ENDLOOP!][!/*
*/!][!ENDIF!][!/*
*/!][!VAR "configLUT" = "substring-after($configLUT, ';')"!][!/*

*/!][!VAR "s2sLUT" = "''"!][!/*
*/!][!VAR "idx" = "0"!][!/*
*/!][!LOOP "LinIfGlobalConfig/*[1]/LinIfChannel/*/LinIfFrame/*[LinIfFrameType='UNCONDITIONAL' and LinIfPduDirection='LinIfSlaveToSlavePdu']"!][!/*
*/!][!VAR "s2sLUT"!][!"$s2sLUT"!];[!"num:i($idx)"!]-[!"as:path(.)"!][!ENDVAR!][!/*
*/!][!VAR "idx" = "$idx + 1"!][!/*
*/!][!ENDLOOP!][!/*

*/!][!MACRO "getDestAndTypeEx", "frameRef"!][!/*
*/!][!VAR "destIdEx"="0"!][!/*
               */!][!SELECT "as:ref($frameRef)"!][!/*
                  */!][!IF "LinIfFrameType = 'UNCONDITIONAL'"!][!/*
                     */!][!IF "LinIfPduDirection = 'LinIfTxPdu'"!][!/*
                        */!][!VAR "destIdEx"!][!CALL "getTxSduIdByFrameRef", "frameRef"="as:path(.)"!][!ENDVAR!][!/*
                        */!][!VAR "destIdEx"="$destIdEx"!][!/*
                        */!][!VAR "frametype"="num:i(0)"!][!/*
                     */!][!ELSEIF "LinIfPduDirection = 'LinIfRxPdu'"!][!/*
                        */!][!VAR "destIdEx"!][!CALL "getRxIndexByFrameRef", "frameRef"="as:path(.)"!][!ENDVAR!][!/*
                        */!][!VAR "frametype"="num:i(1)"!][!/*
                     */!][!ELSEIF "LinIfPduDirection = 'LinIfSlaveToSlavePdu'"!][!/*
                        */!][!VAR "destIdEx"!][!CALL "getS2SIdxByFrameRef", "frameRef"="as:path(.)"!][!ENDVAR!][!/*
                        */!][!VAR "frametype"="num:i(7)"!][!/*
                     */!][!ELSE!][!/*
                        */!][!ERROR!]LinIf: UNCONDITIONAL frames with LinIfPduDirection LinIfInternalPdu are invalid ([!"$frameRef"!]).[!ENDERROR!][!/*
                     */!][!ENDIF!][!/*
                  */!][!ELSEIF "LinIfFrameType = 'MRF'"!][!/*
                     */!][!VAR "destIdEx"="0"!][!VAR "frametype"="num:i(2)"!][!/*
                  */!][!ELSEIF "LinIfFrameType = 'SRF'"!][!/*
                     */!][!VAR "destIdEx"="0"!][!VAR "frametype"="num:i(3)"!][!/*
                  */!][!ELSEIF "LinIfFrameType = 'EVENT_TRIGGERED'"!][!/*
                     */!][!VAR "destIdEx"!][!CALL "getEventIdxByFrameRef", "frameRef"="as:path(.)"!][!ENDVAR!][!VAR "frametype"="num:i(4)"!][!/*
                  */!][!ELSEIF "LinIfFrameType = 'SPORADIC'"!][!/*
                     */!][!VAR "destIdEx"!][!CALL "getSporadicIdxByFrameRef", "frameRef"="as:path(.)"!][!ENDVAR!][!VAR "frametype"="num:i(6)"!][!/*
                  */!][!ELSEIF "LinIfFrameType = 'ASSIGN'"!][!/*
                     */!][!VAR "destIdEx"!][!CALL "getConfigIdxByFrameRef", "frameRef"="as:path(.)"!][!ENDVAR!][!VAR "frametype"="num:i(5)"!][!/*
                  */!][!ELSEIF "LinIfFrameType = 'ASSIGN_FRAME_ID_RANGE'"!][!/*
                     */!][!VAR "destIdEx"!][!CALL "getConfigIdxByFrameRef", "frameRef"="as:path(.)"!][!ENDVAR!][!VAR "frametype"="num:i(5)"!][!/*
                  */!][!ELSEIF "LinIfFrameType = 'ASSIGN_NAD'"!][!/*
                     */!][!VAR "destIdEx"!][!CALL "getConfigIdxByFrameRef", "frameRef"="as:path(.)"!][!ENDVAR!][!VAR "frametype"="num:i(5)"!][!/*
                  */!][!ELSEIF "LinIfFrameType = 'CONDITIONAL'"!][!/*
                     */!][!VAR "destIdEx"!][!CALL "getConfigIdxByFrameRef", "frameRef"="as:path(.)"!][!ENDVAR!][!VAR "frametype"="num:i(5)"!][!/*
                  */!][!ELSEIF "LinIfFrameType = 'SAVE_CONFIGURATION'"!][!/*
                     */!][!VAR "destIdEx"!][!CALL "getConfigIdxByFrameRef", "frameRef"="as:path(.)"!][!ENDVAR!][!VAR "frametype"="num:i(5)"!][!/*
                  */!][!ELSEIF "LinIfFrameType = 'FREE'"!][!/*
                     */!][!VAR "destIdEx"!][!CALL "getConfigIdxByFrameRef", "frameRef"="as:path(.)"!][!ENDVAR!][!VAR "frametype"="num:i(5)"!][!/*
                  */!][!ELSEIF "LinIfFrameType = 'UNASSIGN'"!][!/*
                     */!][!VAR "destIdEx"!][!CALL "getConfigIdxByFrameRef", "frameRef"="as:path(.)"!][!ENDVAR!][!VAR "frametype"="num:i(5)"!][!/*
                  */!][!ELSE!][!/*
                     */!][!VAR "destIdEx"="0"!][!VAR "frametype"="num:i(255)"!][!/*
                  */!][!ENDIF!][!/*
               */!][!ENDSELECT!][!/*
*/!][!ENDMACRO!][!/*
**********************************************************************
*/!][!ENDIF!][!/*
*/!][!/*
*/!][!NOCODE!][!/*
*/!][!/*
****************************************************************************************************
* MACRO to get driver Id.
 "ChannelRef" must reference the container holding the path, of the reference, to the channel.
 (AUTOSAR short name)
****************************************************************************************************
*/!]
[!MACRO "getDriverIdx","channelRef"!][!/*
*/!][!VAR "driverIdx"="num:i(0)"!][!/*
*/!][!LOOP "text:split($channelAndDriversList,';')"!][!/*
*/!][!IF "$channelRef = string(text:split(.)[1])"!][!/*
*/!][!VAR "driverIdx"!][!"num:i(text:split(.)[3])"!][!ENDVAR!][!/*
*/!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*
*/!][!ENDMACRO!][!/*
*/!][!ENDNOCODE!][!/*
*/!][!/*
****************************************************************************************************
* MACRO to enumerate related LinTrcv data
* Entries consist of colon-joined ASPaths of LintTrcv configuration, struct index, Trcv chnannel,
* Vendor ID, Vendor API Infix tuples separated by semicolons.
****************************************************************************************************
*/!]
[!MACRO "EnumerateTrcvs"!]
[!IF "not(var:defined('trcvLst'))"!]
[!VAR "trcvLst" = "''"!]
[!VAR "trcvRef" = "''"!]
[!VAR "trcvPos" = "num:i(0)"!]
[!FOR "A"="1" TO "$numChannels"!]
[!IF "node:exists(as:modconf('LinIf')/LinIfGlobalConfig/*[1]/LinIfChannel/*[num:i($A)]/LinIfTransceiverDrvConfig/LinIfTrcvIdRef)"!]
  [!IF "not(text:contains(string($trcvRef), string(as:path(node:ref(as:modconf('LinIf')/LinIfGlobalConfig/*[1]/LinIfChannel/*[num:i($A)]/LinIfTransceiverDrvConfig/LinIfTrcvIdRef)/../..))))"!]

    [!VAR "trcvVendorID" = "''"!]
    [!VAR "trcvApiInfix" = "''"!]
    [!VAR "wuSupp" = "node:ref(as:modconf('LinIf')/LinIfGlobalConfig/*[1]/LinIfChannel/*[num:i($A)]/LinIfTransceiverDrvConfig/LinIfTrcvIdRef)/../../LinTrcvGeneral/LinTrcvWakeUpSupport"!]

    [!IF "$trcvDoApiInfix = 'true'"!]
      [!VAR "apiInfix" = "''"!]
      [!VAR "ChannelRef"="node:path(as:ref(as:modconf('LinIf')/LinIfGlobalConfig/*[1]/LinIfChannel/*[num:i($A)]/LinIfTransceiverDrvConfig/LinIfTrcvIdRef))"!]
      [!VAR "BswmdRef"="''"!]
      /*<!-- !LINKSTO LinIf.EB.DriverVendorInfo.BSWMDProvides.CheckVendorApiInfix.TRUE,1 -->*/
      [!IF "node:exists(as:modconf('LinIf')[1]/LinIfEbGeneral/LinIfEbGeneralBswmdImplementation/LinIfEbGeneralBswmdImplementationRefs)"!]
        [!LOOP "as:modconf('LinIf')[1]/LinIfEbGeneral/LinIfEbGeneralBswmdImplementation/LinIfEbGeneralBswmdImplementationRefs/*"!]
          [!IF "text:contains(string($ChannelRef), string(node:path(as:ref(LinIfDrvTrcvRef))))"!]
            [!VAR "BswmdRef"="node:path(as:ref(LinIfDrvTrcvBswImplementationRef))"!]
            [!VAR "apiInfix"!][!"asc:getVendorApiInfix(as:path($BswmdRef))"!][!ENDVAR!]
          [!ENDIF!]
        [!ENDLOOP!]
      [!ENDIF!]
      [!IF "$apiInfix != ''"!]
        [!VAR "trcvVendorID" = "concat('_', asc:getVendorId(as:path($BswmdRef)))"!]
        [!VAR "trcvApiInfix" = "concat('_', $apiInfix)"!]
      [!ENDIF!]
      [!IF "$apiInfix = '' or $BswmdRef = ''"!]
        [!IF "not(node:exists(node:ref(as:modconf('LinIf')/LinIfGlobalConfig/*[1]/LinIfChannel/*[num:i($A)]/LinIfTransceiverDrvConfig/LinIfTrcvIdRef)/../../CommonPublishedInformation))"!]
          /*<!-- !LINKSTO LinIf.EB.DriverVendorInfo.ConfigProvides.ThrowError,1 -->*/
          [!CODE!]
          [!ERROR!]Transceiver vendor information not available![!ENDERROR!]
          [!ENDCODE!]
        [!ELSE!]
          /*<!-- !LINKSTO LinIf.EB.DriverVendorInfo.BSWMDProvides.CheckVendorApiInfix.FALSE,1 -->*/
          /*<!-- !LINKSTO LinIf.EB.DriverVendorInfo.ConfigProvides.NoError,1 -->*/
          [!VAR "trcvVendorID" = "concat('_', node:value(node:ref(as:modconf('LinIf')/LinIfGlobalConfig/*[1]/LinIfChannel/*[num:i($A)]/LinIfTransceiverDrvConfig/LinIfTrcvIdRef)/../../CommonPublishedInformation/VendorId))"!]
          [!VAR "trcvApiInfix" = "concat('_', node:value(node:ref(as:modconf('LinIf')/LinIfGlobalConfig/*[1]/LinIfChannel/*[num:i($A)]/LinIfTransceiverDrvConfig/LinIfTrcvIdRef)/../../CommonPublishedInformation/VendorApiInfix))"!]
        [!ENDIF!]
      [!ENDIF!]
    [!ENDIF!]

    [!VAR "trcvRef"!][!"$trcvRef"!][!"as:path(node:ref(as:modconf('LinIf')/LinIfGlobalConfig/*[1]/LinIfChannel/*[num:i($A)]/LinIfTransceiverDrvConfig/LinIfTrcvIdRef)/../..)"!];[!ENDVAR!]
    [!VAR "trcvLst"!][!"$trcvLst"!][!"as:path(node:ref(as:modconf('LinIf')/LinIfGlobalConfig/*[1]/LinIfChannel/*[num:i($A)]/LinIfTransceiverDrvConfig/LinIfTrcvIdRef)/../..)"!],[!"$trcvPos"!],[!"$wuSupp"!],[!"$trcvVendorID"!],[!"$trcvApiInfix"!];[!ENDVAR!]
    [!VAR "trcvPos" = "num:i($trcvPos + 1)"!]

  [!ENDIF!]
[!ENDIF!]
[!ENDFOR!]
[!ENDIF!]
[!ENDMACRO!]
[!/*

[!/*
****************************************************************************************************
* MACRO to enumerate related LinTrcv data
* Entries consist of colon-joined ASPaths of LintTrcv configuration, struct index, Trcv chnannel,
* Vendor ID, Vendor API Infix tuples separated by semicolons.
****************************************************************************************************
*/!]
[!MACRO "GenerateTrcvIncludes"!][!NOCODE!]
[!IF "$numTrcvs > 0"!]
[!CALL "EnumerateTrcvs"!]
[!LOOP "text:split($trcvLst, ';')"!]
  [!VAR "mangledName" = "''"!]
  [!IF "$trcvDoApiInfix = 'true'"!]
    [!VAR "mangledName" = "concat(text:split(.,',')[4], text:split(.,',')[5])"!]
  [!ENDIF!]
[!CODE!][!//
[!WS "0"!]#include <LinTrcv[!"$mangledName"!].h>
[!IF "text:split(.,',')[3] = 'true'"!][!//
[!WS "0"!]#include <LinTrcv[!"$mangledName"!]_Cbk.h>
[!ENDIF!][!//
[!ENDCODE!]
[!ENDLOOP!]
[!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO to enumerate related LinTrcv data
* LinIfChannel - node path of LinIf channel
****************************************************************************************************
*/!]
[!MACRO "GetTrcvFuncIdx", "LinIfChannelPth"!][!NOCODE!]
[!VAR "trcvFuncIdx" = "num:i(255)"!]
[!CALL "EnumerateTrcvs"!]
[!IF "node:refvalid(as:ref($LinIfChannelPth)/LinIfTransceiverDrvConfig/LinIfTrcvIdRef)"!]
  [!VAR "pth" = "as:path(as:ref(as:ref($LinIfChannelPth)/LinIfTransceiverDrvConfig/LinIfTrcvIdRef)/../..)"!]
  [!LOOP "text:split($trcvLst, ';')"!]
    [!IF "$pth = text:split(.,',')[1]"!]
      [!VAR "trcvFuncIdx" = "num:i(text:split(.,',')[2])"!]
      [!BREAK!]
    [!ENDIF!]
  [!ENDLOOP!]
[!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO to get the string for the symbolic name value.
* "ShortNameRef" must reference the container holding the short name. (AUTOSAR short name)
* "OldName" must holde a an unique string for the case that the specification
* conform short name does not exist. This is supplied to be backward compatible.
****************************************************************************************************
*/!]
[!VAR "ShortNameList"="''"!]
[!MACRO "GetSymbolName","ShortNameRef","OldName"!][!/*
*/!][!NOCODE!][!/*
*/!][!VAR "SymbolName" = "asc:getShortName($ShortNameRef)"!][!/*
*/!][!IF "($SymbolName = '')"!][!/*
*/!][!VAR "SymbolName" = "concat(name(../../..),'_',name(..))"!][!/*
*/!][!ELSE!][!/*
*/!][!VAR "ShortNameList"="concat($ShortNameList,' ',$SymbolName)"!][!/*
*/!][!IF "not(text:uniq(text:split($ShortNameList), $SymbolName))"!][!/*
*/!][!ERROR!] The short name [!"$SymbolName"!] of the schema node [!"$OldName"!] is not unique.[!ENDERROR!][!/*
*/!][!ENDIF!][!/*
*/!][!VAR "SymbolName" = "concat($OldName,'_',$SymbolName)"!][!/*
*/!][!ENDIF!][!/*
*/!][!ENDNOCODE!][!/*
*/!][!ENDMACRO!][!/*

*/!][!MACRO "GetLinIfPduSymbolName","ObjectRef"!][!/*
*/!][!VAR "SymbolName" = "asc:getShortName($ObjectRef)"!][!/*
*/!][!IF "($SymbolName = 'LinIfPduDirection')"!][!/*
*/!][!VAR "SymbolName" = "concat(name(../../..),'_',name(..))"!][!/*
*/!][!ELSE!][!/*
*/!][!VAR "ShortNameList" = "concat($ShortNameList,' ',$SymbolName)"!][!/*
*/!][!IF "not(text:uniq(text:split($ShortNameList), $SymbolName))"!][!/*
*/!][!ERROR!] The short name [!"$SymbolName"!] of the schema node [!"$ObjectRef"!] is not unique.[!ENDERROR!][!/*
*/!][!ENDIF!][!/*
*/!][!VAR "SymbolName" = "concat('LinIfTxPdu_',$SymbolName)"!][!/*
*/!][!ENDIF!][!/*
*/!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO to retrieve numeric value split to bytes
* Val - Numeric value
* Results are in NumValB0 - B3 (unused bytes are set to 0)
****************************************************************************************************
*/!]
[!MACRO "GetNumBytes","Val"!][!NOCODE!]
[!VAR "NumValB0" = "bit:and($Val, 255)"!]
[!VAR "NumValB1" = "bit:shr(bit:and($Val, 65280), 8)"!]
[!VAR "NumValB2" = "bit:shr(bit:and($Val, 16711680), 16)"!]
[!VAR "NumValB3" = "bit:shr(bit:and($Val, 4278190080),24)"!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO to calculate the PID from the ID
* ID - Frame ID
* Result is stored in PID
****************************************************************************************************
*/!]
[!MACRO "CalcPID","ID"!][!NOCODE!]
[!VAR "PID" = "$ID"!]
[!VAR "idB0" = "bit:and($ID, 1)"!]
[!VAR "idB1" = "bit:shr(bit:and($ID, 2), 1)"!]
[!VAR "idB2" = "bit:shr(bit:and($ID, 4), 2)"!]
[!VAR "idB3" = "bit:shr(bit:and($ID, 8), 3)"!]
[!VAR "idB4" = "bit:shr(bit:and($ID, 16), 4)"!]
[!VAR "idB5" = "bit:shr(bit:and($ID, 32), 5)"!]
[!VAR "idP0" = "bit:xor(bit:xor(bit:xor($idB0, $idB1), $idB2), $idB4)"!]
[!VAR "idP1" = "node:when(bit:xor(bit:xor(bit:xor($idB1, $idB3), $idB4), $idB5) = 0, 1, 0)"!][!/* bit:not() makes it signed */!]
[!VAR "PID" = "bit:or(bit:or(bit:shl($idP0, 6), bit:shl($idP1, 7)), $PID)"!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO to establish response_error signal associated frame indices
* Result is stored in ChanErrSigIdx
****************************************************************************************************
*/!]
[!MACRO "EnumerateErrSigFrames"!][!NOCODE!]
[!VAR "ChanErrSigIdx" = "''"!]
[!VAR "errSigCfgErr" = "''"!]
[!LOOP "as:modconf('LinIf')[1]/LinIfGlobalConfig/*[1]/LinIfChannel/*[LinIfNodeType = 'LinIfSlave']"!]
  [!VAR "rpCom" = "''"!]
  [!IF "count(as:ref(LinIfNodeType/LinIfResponseErrorSignal)/../../ComIPdu/*/ComIPduSignalRef/*[. = node:current()/LinIfNodeType/LinIfResponseErrorSignal]) != 1"!]
    [!VAR "errSigCfgErr" = "concat($errSigCfgErr, 'The signal referenced by LinIfResponseErrorSignal shall be associated to a single ComIPdu. Not the case for ', node:name(.))"!]
  [!ELSE!]
    [!IF "'[]' != string(asc:getPdus('PduR','ForUpperLayer','If','Tx',1, as:ref(LinIfNodeType/LinIfResponseErrorSignal)/../../ComIPdu/*/ComIPduSignalRef/*[. = node:current()/LinIfNodeType/LinIfResponseErrorSignal]/../../ComPduIdRef))"!]
      [!VAR "rpCom" = "node:name(node:ref(node:path(asc:getPdus('PduR','ForUpperLayer','If','Tx',1, as:ref(LinIfNodeType/LinIfResponseErrorSignal)/../../ComIPdu/*/ComIPduSignalRef/*[. = node:current()/LinIfNodeType/LinIfResponseErrorSignal]/../../ComPduIdRef)[1]))/../..)"!]
    [!ENDIF!]
  [!ENDIF!]
  [!VAR "chNumTxUnCond" = "count(LinIfFrame/*[LinIfFrameType = 'UNCONDITIONAL' and LinIfPduDirection = 'LinIfTxPdu'])"!]
  [!VAR "numErrSigTxFrame" = "num:i(0)"!]
  [!VAR "errSigTxFrames" = "''"!]
  [!IF "'' != $rpCom"!]
    [!FOR "iFrm" = "1" TO "$chNumTxUnCond"!]
      [!SELECT "LinIfFrame/*[LinIfFrameType = 'UNCONDITIONAL' and LinIfPduDirection = 'LinIfTxPdu' and node:refvalid(LinIfPduDirection/LinIfTxPduRef)][number($iFrm)]"!]
        [!IF "$rpCom = node:name(node:ref(node:path(asc:getPdus('PduR','ForLowerLayer','If','Tx',1, ./LinIfPduDirection/LinIfTxPduRef)[1]))/../../..)"!]
          [!VAR "ChanErrSigIdx" = "concat(node:name(../..), ':', LinIfFrameIndex, ';', $ChanErrSigIdx)"!]
          [!VAR "numErrSigTxFrame" = "num:i($numErrSigTxFrame + 1)"!]
          [!VAR "errSigTxFrames" = "concat(node:name(.), ', ', $errSigTxFrames)"!]
        [!ENDIF!]
      [!ENDSELECT!]
    [!ENDFOR!]
  [!ENDIF!]
  [!IF "$numErrSigTxFrame = 0"!]
    [!VAR "errSigCfgErr"!][!"$errSigCfgErr"!][!CR!]The response_error signal has to be routed back to LinIf through an UNCONDITIONAL TxPdu[!ENDVAR!]
  [!ELSEIF "$numErrSigTxFrame > 1"!]
    [!VAR "errSigCfgErr"!][!"$errSigCfgErr"!][!CR!]A single TX frame should be associated to the response_error signal. The following are: [!"$errSigTxFrames"!][!ENDVAR!]
  [!ENDIF!]
[!ENDLOOP!]
[!IF "$errSigCfgErr != ''"!]
  [!ERROR!][!"$errSigCfgErr"!][!ENDERROR!]
[!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO to retrieve response_error signal associated frame index
* Result is stored in errSigFrmIdx
****************************************************************************************************
*/!]
[!MACRO "GetErrSigFrameIdx", "chName"!][!NOCODE!]
[!VAR "errSigFrmIdx" = "text:split(text:grep(text:split($ChanErrSigIdx, ';'), concat('^', $chName, '\:\d+$'))[1], ':')[2]"!]
[!ENDNOCODE!][!ENDMACRO!]
