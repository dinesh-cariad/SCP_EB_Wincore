[!/**
 * \file
 *
 * \brief AUTOSAR IpduM
 *
 * This file contains the implementation of the AUTOSAR
 * module IpduM.
 *
 * \version 3.3.40
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!NOCODE!][!//
[!IF "not(var:defined('IPDUM_CHECKS_M'))"!][!//
[!VAR "IPDUM_CHECKS_M"="'true'"!][!/*

[!/*
****************************************************************************
* Info if SHORT-NAMEs of IpduMRxIndications are not unique.
****************************************************************************
*/!][!//
[!VAR "IpduMRxIndicationNames"= "' '"!][!//
[!SELECT "IpduMConfig/*[1]"!][!//
  [!IF "node:exists(IpduMRxPathway/*)"!][!//
    [!LOOP "IpduMRxPathway/*"!][!//
      [!IF "contains($IpduMRxIndicationNames, concat(' ', name(IpduMRxIndication), ' '))"!][!//
         [!INFO!]The SHORT-NAMEs of the IpduMRxIndication are not unique. The SHORT-NAME " [!"name(IpduMRxIndication)"!] "  is used multiple times.[!ENDINFO!][!//
      [!ELSE!][!//
        [!VAR "IpduMRxIndicationNames"= "concat($IpduMRxIndicationNames, name(IpduMRxIndication), ' ')"!][!//
      [!ENDIF!][!//
    [!ENDLOOP!][!//
  [!ENDIF!][!//
[!ENDSELECT!][!/*

****************************************************************************
* Check that ContainerTxPdus either have TxSendTimeout configured
* or all of their children do
****************************************************************************
*/!][!//
[!SELECT "IpduMConfig/*[1]"!][!//
  [!IF "node:exists(IpduMContainedTxPdu/*)"!][!//
    [!LOOP "IpduMContainedTxPdu/*"!]
      [!IF "not(node:exists(./IpduMContainedTxPduSendTimeout)) and not(node:exists(node:ref(./IpduMContainedTxInContainerPduRef)/IpduMContainerTxSendTimeout))"!]
        [!ERROR!] All IpdumContainedTxPdus need to have TxSendTimeout configured if the Container does not. " [!"node:path(.)"!] " and " [!"node:path(node:ref(./IpduMContainedTxInContainerPduRef))"!] " violate this. [!ENDERROR!]
      [!ENDIF!][!//
    [!ENDLOOP!][!//
  [!ENDIF!][!//
[!ENDSELECT!][!/*

*******************************************************************************
* Following checks are applied:
* - Byte order must be set to LE
* - Referenced PDUs must be valid
* - Segment boundaries
* - The sum of the bits of static part and each dynamic part do not exceed the
*   IpduMSize * 8.
* - Segment overlapping
* - Selector field containment
* +- The selector field is not part of any segment of the static part
* +- For the dynamic part according to value of IpduMTxAutomaticSelector
*  +- false: The selector field is enclosed by exactly one segment of each
*            dynamic part of a TxPathway
*  +- true : The selector field is not enclosed by any segment of the
*            dynamic part
*******************************************************************************
*/!][!//
[!LOOP "as:modconf('IpduM')[1]/IpduMConfig/*[1]/IpduMTxPathway/*"!][!//
  [!VAR "IpduMSelectorStartPosition" = "num:i(./IpduMTxRequest/IpduMSelectorFieldPosition/IpduMSelectorFieldPosition)"!][!//
  [!VAR "IpduMSelectorEndPosition" = "($IpduMSelectorStartPosition + num:i(./IpduMTxRequest/IpduMSelectorFieldPosition/IpduMSelectorFieldLength - 1))"!][!//
  [!IF "./IpduMTxRequest/IpduMByteOrder != 'LITTLE_ENDIAN'"!][!//
    [!ERROR!] IpduM supports only LITTLE ENDIAN. IpduMByteOrder at '[!"node:path(.)"!]' violates this [!ENDERROR!][!//
  [!ENDIF!][!//
  [!VAR "IpduMStaticSize"= "num:i(0)"!][!//
  [!VAR "IpduMDynamicSize"= "num:i(0)"!][!//
  [!IF "node:refvalid(IpduMTxRequest/IpduMOutgoingPduRef)"!][!//
    [!VAR "IpduMTotalSize"= "num:i(as:ref(./IpduMTxRequest/IpduMOutgoingPduRef)/PduLength * 8)"!][!//
  [!ELSE!][!//
    [!VAR "IpduMTotalSize"= "num:i(0)"!][!//
  [!ENDIF!][!//
  [!IF "$IpduMTotalSize != 0"!][!//
    [!IF "node:exists(./IpduMTxRequest/IpduMTxStaticPart)"!][!//
      [!IF "not(node:refexists(./IpduMTxRequest/IpduMTxStaticPart/IpduMTxStaticPduRef))"!][!//
        [!ERROR!] Invalid reference: '[!"node:path(./IpduMTxRequest/IpduMTxStaticPart/IpduMTxStaticPduRef)"!]' [!ENDERROR!][!//
      [!ELSE!][!//
        [!VAR "IpduMStaticSrcPduSize"= "num:i(as:ref(./IpduMTxRequest/IpduMTxStaticPart/IpduMTxStaticPduRef)/PduLength * 8)"!][!//
      [!ENDIF!][!//
      [!VAR "IpduMStaticSegmentSelFieldContainmentCount" = "num:i(0)"!][!//
      [!LOOP "./IpduMTxRequest/IpduMTxStaticPart/IpduMSegment/*"!][!//
        [!VAR "IpduMStaticSize"= "num:i($IpduMStaticSize + ./IpduMSegmentLength)"!][!//
        [!IF "(num:i(./IpduMSegmentPosition + ./IpduMSegmentLength)) > $IpduMStaticSrcPduSize"!][!//
          [!ERROR!] The segment offset of a static segment shall not exceed the boundary of the referenced static COM I-PDU. segment at " [!"node:path(.)"!] "  violates this [!ENDERROR!][!//
        [!ENDIF!][!//
        [!IF "(num:i(./IpduMDestinationBit + ./IpduMSegmentLength)) > $IpduMTotalSize"!][!//
          [!ERROR!] The destination PDU offset of a static segment shall not exceed the boundary of the referenced IpduM I-PDU. segment at " [!"node:path(.)"!] "  violates this [!ENDERROR!][!//
        [!ENDIF!][!//
        [!VAR "IpdumStaticSegmentStartPosition" = "num:i(./IpduMSegmentPosition)"!]
        [!VAR "IpdumStaticSegmentEndPosition" = "num:i(./IpduMSegmentPosition) + num:i(./IpduMSegmentLength - 1)"!]
        [!IF "node:exists(./IpduMDestinationBit)"!][!//
          [!VAR "IpdumStaticSegmentStartPosition" = "num:i(./IpduMDestinationBit)"!]
          [!VAR "IpdumStaticSegmentEndPosition" = "num:i(./IpduMDestinationBit) + num:i(./IpduMSegmentLength - 1)"!]
        [!ENDIF!]
        [!IF "((($IpdumStaticSegmentStartPosition <= $IpduMSelectorStartPosition) and ($IpdumStaticSegmentEndPosition >= $IpduMSelectorStartPosition)) or (($IpdumStaticSegmentStartPosition <= $IpduMSelectorEndPosition) and ($IpdumStaticSegmentEndPosition >= $IpduMSelectorEndPosition)))"!][!//
          [!VAR "IpduMStaticSegmentSelFieldContainmentCount"= "num:i($IpduMStaticSegmentSelFieldContainmentCount + 1)"!][!//
        [!ENDIF!][!//
      [!ENDLOOP!][!//
      [!IF "$IpduMStaticSegmentSelFieldContainmentCount > num:i(0)"!][!//
        [!WARNING!] The selector field should not be part of any segment of the static part. " [!"node:path(.)"!] "  violates this. [!ENDWARNING!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
    [!LOOP "./IpduMTxRequest/IpduMTxDynamicPart/*"!][!//
      [!IF "not(node:refexists(./IpduMTxDynamicPduRef))"!][!//
        [!ERROR!] Invalid reference: '[!"node:path(./IpduMTxDynamicPduRef)"!]' [!ENDERROR!][!//
      [!ELSE!][!//
        [!VAR "IpduMDynSrcPduSize"= "num:i(as:ref(./IpduMTxDynamicPduRef)/PduLength * 8)"!][!//
      [!ENDIF!][!//
      [!VAR "IpduMDynamicSegmentFullyFieldCount" = "num:i(0)"!][!//
      [!VAR "IpduMDynamicSegmentPartlyFieldCount" = "num:i(0)"!][!//
      [!LOOP "./IpduMSegment/*"!][!//
        [!VAR "IpduMDynamicSize"= "num:i($IpduMDynamicSize + ./IpduMSegmentLength)"!][!//
        [!IF "(num:i(./IpduMSegmentPosition + ./IpduMSegmentLength)) > $IpduMDynSrcPduSize"!][!//
          [!ERROR!] The segment offset of a dynamic segment shall not exceed the boundary of the referenced dynamic COM I-PDU. segment at " [!"node:path(.)"!] "  violates this [!ENDERROR!][!//
        [!ENDIF!][!//
        [!IF "(num:i(./IpduMDestinationBit + ./IpduMSegmentLength)) > $IpduMTotalSize"!][!//
          [!ERROR!] The destination PDU offset of a dynamic segment shall not exceed the boundary of the referenced IpduM I-PDU. segment at " [!"node:path(.)"!] "  violates this [!ENDERROR!][!//
        [!ENDIF!][!//
        [!VAR "IpdumDynamicSegmentStartPosition" = "num:i(./IpduMSegmentPosition)"!]
        [!VAR "IpdumDynamicSegmentEndPosition" = "num:i(./IpduMSegmentPosition) + num:i(./IpduMSegmentLength - 1)"!]
        [!IF "node:exists(./IpduMDestinationBit)"!][!//
          [!VAR "IpdumDynamicSegmentStartPosition" = "num:i(./IpduMDestinationBit)"!]
          [!VAR "IpdumDynamicSegmentEndPosition" = "num:i(./IpduMDestinationBit) + num:i(./IpduMSegmentLength - 1 )"!]
        [!ENDIF!][!//
        [!IF "(($IpdumDynamicSegmentStartPosition <= $IpduMSelectorStartPosition) and ($IpdumDynamicSegmentEndPosition >= $IpduMSelectorEndPosition))"!][!//
          [!VAR "IpduMDynamicSegmentFullyFieldCount" = "num:i($IpduMDynamicSegmentFullyFieldCount + 1)"!][!//
        [!ELSEIF "((($IpdumDynamicSegmentStartPosition <= $IpduMSelectorStartPosition) and ($IpdumDynamicSegmentEndPosition >= $IpduMSelectorStartPosition)) or (($IpdumDynamicSegmentStartPosition <= $IpduMSelectorEndPosition) and ($IpdumDynamicSegmentEndPosition >= $IpduMSelectorEndPosition)))"!][!//
          [!VAR "IpduMDynamicSegmentPartlyFieldCount" = "num:i($IpduMDynamicSegmentPartlyFieldCount + 1)"!][!//
        [!ENDIF!][!//
      [!ENDLOOP!][!//
      [!IF "as:modconf('IpduM')[1]/IpduMGeneral/IpduMTxAutomaticSelector ='false'"!][!//
        [!IF "(($IpduMDynamicSegmentPartlyFieldCount != num:i(0)) or ($IpduMDynamicSegmentFullyFieldCount != num:i(1)))"!][!//
          [!ERROR!] If IpduMTxAutomaticSelector is deactivated then the selector field must be fully enclosed by exactly one segment of each dynamic part. " [!"node:path(.)"!] "  violates this. [!ENDERROR!][!//
        [!ENDIF!][!//
      [!ELSE!][!//
        [!IF "(($IpduMDynamicSegmentPartlyFieldCount != num:i(0)) or ($IpduMDynamicSegmentFullyFieldCount != num:i(0)))"!][!//
          [!WARNING!] If IpduMTxAutomaticSelector is enabled then the selector field should not be part of any segment of the dynamic part. " [!"node:path(.)"!] "  violates this. [!ENDWARNING!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
      [!IF "($IpduMDynamicSize + $IpduMStaticSize) > $IpduMTotalSize"!][!//
        [!ERROR!]Static or dynamic part segments overlap in IpduM I-PDU at " [!"node:path(../../..)"!] ".  [!ENDERROR!][!//
      [!ENDIF!][!//
      [!VAR "IpduMDynamicSize"= "num:i(0)"!][!//
    [!ENDLOOP!][!//
  [!ENDIF!][!//
[!ENDLOOP!][!//

<!-- !LINKSTO IpduM.EB.HeaderTypeNone.CntdOverlapping,1 -->
<!-- !LINKSTO IpduM.EB.HeaderTypeNone.CntdSizeExceed,1 -->
<!-- !LINKSTO IpduM.EB.HeaderTypeNone.CntdUpdateBitSizeExceed,1 -->
<!-- !LINKSTO IpduM.ASR43.SWS_IpduM_00245,1 -->
*******************************************************************************
* Following checks are applied for container Tx with IPDUM_HEADERTYPE_NONE:
* - Overlapping of contained PDUs
* - ContainedPdu exceeding container PDU length
* - IpduMPduUpdateBitPosition exceeding container PDU length
* - IpduMPduUpdateBitPosition overlapping contained PDUs
* - IpduMPduUpdateBitPosition having the same value
*******************************************************************************
[!MACRO "GetIpduMCntdsOffsetForCntrTx", "Container"!][!NOCODE!]
[!VAR "parentNode" = "node:path($Container)"!]
[!VAR "refCntdsTxOffset"!][!ENDVAR!]
[!LOOP "node:order(as:modconf('IpduM')[1]/IpduMConfig/*[1]/IpduMContainedTxPdu/* [node:path(node:value(IpduMContainedTxInContainerPduRef)) = $parentNode], 'node:IpduMContainedPduOffset')"!]
  [!VAR "refCntdsTxOffset"!][!"$refCntdsTxOffset"!]:[!"as:path(.)"!][!ENDVAR!]
[!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!]

[!LOOP "as:modconf('IpduM')[1]/IpduMConfig/*[1]/IpduMContainerTxPdu/* [IpduMContainerHeaderSize = 'IPDUM_HEADERTYPE_NONE']"!][!//
    [!CALL "GetIpduMCntdsOffsetForCntrTx", "Container" = "node:path(.)"!]
    [!VAR "noOfrefCntdsTxOffset" = "num:i(count(text:split($refCntdsTxOffset,':')))"!]
    [!FOR "i" = "1" TO "$noOfrefCntdsTxOffset - 1"!]
      [!VAR "Cntd" = "text:split($refCntdsTxOffset,':')[position() = $i]"!]
      [!VAR "NextCntd" = "text:split($refCntdsTxOffset,':')[position() = (num:i($i + 1))]"!]
      [!VAR "Start" = "as:ref($Cntd)/IpduMContainedPduOffset"!]
      [!VAR "Length" = "as:ref(as:ref($Cntd)/IpduMContainedTxPduRef)/PduLength"!]
      [!VAR "End" = "as:ref($NextCntd)/IpduMContainedPduOffset"!]
      [!IF "$Start + $Length > $End"!]
        [!ERROR!][!"node:name(as:ref($NextCntd))"!]: Overlapping with [!"node:name(as:ref($Cntd))"!]. Please check IpduMContainedPduOffset or PduLength for both contained PDUs.[!ENDERROR!]
      [!ENDIF!]
    [!ENDFOR!]

    [!VAR "Cntd" = "text:split($refCntdsTxOffset,':')[last()]"!]

    [!VAR "Start" = "as:ref($Cntd)/IpduMContainedPduOffset"!]
    [!VAR "Length" = "as:ref(as:ref($Cntd)/IpduMContainedTxPduRef)/PduLength"!]
    [!VAR "End" = "as:ref(as:ref(as:path($Container))/IpduMContainerTxPduRef)/PduLength"!]
    [!IF "$Start + $Length > $End"!]
      [!ERROR!][!"node:name(as:ref($Cntd))"!]: ContainedPdu exceeds container's [!"node:name(as:ref(as:path($Container)))"!] PduLength. Please check IpduMContainedPduOffset / PduLength for contained PDU or PduLength for container PDU.[!ENDERROR!]
    [!ENDIF!]

    [!IF "not(node:exists(IpduMUnusedAreasDefault))"!]
      [!FOR "i" = "1" TO "$noOfrefCntdsTxOffset"!]
        [!VAR "Cntd" = "text:split($refCntdsTxOffset,':')[position() = $i]"!]
        [!VAR "BitPos" = "num:i(as:ref($Cntd)/IpduMPduUpdateBitPosition)"!]
        [!VAR "BytePos" = "num:i(as:ref($Cntd)/IpduMPduUpdateBitPosition div 8)"!]
        [!VAR "End" = "as:ref(as:ref(as:path($Container))/IpduMContainerTxPduRef)/PduLength"!]
        [!IF "$BytePos >= $End"!]
          [!ERROR!][!"node:name(as:ref($Cntd))"!]: IpduMPduUpdateBitPosition [Byte: [!"$BytePos"!]] exceeds container's [!"node:name(as:ref(as:path($Container)))"!] PduLength. Please check IpduMPduUpdateBitPosition for contained PDU or PduLength for container PDU.[!ENDERROR!]
        [!ENDIF!]

        [!FOR "j" = "1" TO "$noOfrefCntdsTxOffset"!]
          [!VAR "Cntd2" = "text:split($refCntdsTxOffset,':')[position() = $j]"!]
          [!VAR "BitPos2" = "num:i(as:ref($Cntd2)/IpduMPduUpdateBitPosition)"!]
          [!VAR "Start" = "as:ref($Cntd2)/IpduMContainedPduOffset"!]
          [!VAR "Length" = "as:ref(as:ref($Cntd2)/IpduMContainedTxPduRef)/PduLength"!]
          [!VAR "End" = "num:i($Start + $Length)"!]
          [!IF "$BytePos >= $Start and $BytePos < $End"!]
            [!ERROR!][!"node:name(as:ref($Cntd))"!]: IpduMPduUpdateBitPosition [Byte: [!"$BytePos"!]] overlaps contained PDU [!"node:name(as:ref($Cntd2))"!].[!ENDERROR!]
          [!ENDIF!]
          [!IF "$i != $j and $BitPos = $BitPos2"!]
            [!ERROR!][!"node:name(as:ref($Cntd))"!]: IpduMPduUpdateBitPosition has the same value as the one for [!"node:name(as:ref($Cntd2))"!].[!ENDERROR!]
          [!ENDIF!]
        [!ENDFOR!]
      [!ENDFOR!]


      [!VAR "maxCntdOffset" = "num:i(num:max(as:modconf('IpduM')[1]/IpduMConfig/*[1]/IpduMContainedTxPdu/*/IpduMContainedPduOffset [node:exists(../IpduMContainedTxInContainerPduRef) and node:name(as:ref(../IpduMContainedTxInContainerPduRef)) = node:name(node:current())]))"!]
      [!VAR "maxCntdUpdateBytePos" = "num:i(num:max(as:modconf('IpduM')[1]/IpduMConfig/*[1]/IpduMContainedTxPdu/*/IpduMPduUpdateBitPosition [node:exists(../IpduMContainedTxInContainerPduRef) and node:name(as:ref(../IpduMContainedTxInContainerPduRef)) = node:name(node:current())]) div 8)"!]
      [!IF "$maxCntdUpdateBytePos > $maxCntdOffset"!]
        [!ERROR!][!"node:name(as:ref(as:path($Container)))"!]: Container PDU must not end with a byte used for storing IpduMUpdateBits. It must have a contained PDU at its end.[!ENDERROR!]
      [!ENDIF!]
    [!ENDIF!]
[!ENDLOOP!]

<!-- !LINKSTO IpduM.EB.HeaderTypeNone.CntdOverlapping,1 -->
<!-- !LINKSTO IpduM.EB.HeaderTypeNone.CntdSizeExceed,1 -->
<!-- !LINKSTO IpduM.EB.HeaderTypeNone.CntdUpdateBitSizeExceed,1 -->
<!-- !LINKSTO IpduM.ASR43.SWS_IpduM_00245,1 -->
*******************************************************************************
* Following checks are applied for container Rx with IPDUM_HEADERTYPE_NONE:
* - Overlapping of contained PDUs
* - ContainedPdu exceeding container PDU length
* - IpduMPduUpdateBitPosition exceeding container PDU length
* - IpduMPduUpdateBitPosition overlapping contained PDUs
* - IpduMPduUpdateBitPosition having the same value
*******************************************************************************
[!MACRO "GetIpduMCntdsOffsetForCntrRx", "Container"!][!NOCODE!]
[!VAR "parentNode" = "node:path($Container)"!]
[!VAR "refCntdsRxOffset"!][!ENDVAR!]
[!LOOP "node:order(as:modconf('IpduM')[1]/IpduMConfig/*[1]/IpduMContainedRxPdu/* [node:exists(IpduMContainedRxInContainerPduRef) and node:path(node:value(IpduMContainedRxInContainerPduRef)) = $parentNode], 'node:IpduMContainedPduOffset')"!]
  [!VAR "refCntdsRxOffset"!][!"$refCntdsRxOffset"!]:[!"as:path(.)"!][!ENDVAR!]
[!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!]

[!LOOP "as:modconf('IpduM')[1]/IpduMConfig/*[1]/IpduMContainerRxPdu/* [IpduMContainerHeaderSize = 'IPDUM_HEADERTYPE_NONE']"!][!//
    [!CALL "GetIpduMCntdsOffsetForCntrRx", "Container" = "node:path(.)"!]
    [!VAR "noOfrefCntdsRxOffset" = "num:i(count(text:split($refCntdsRxOffset,':')))"!]
    [!FOR "i" = "1" TO "$noOfrefCntdsRxOffset - 1"!]
      [!VAR "Cntd" = "text:split($refCntdsRxOffset,':')[position() = $i]"!]
      [!VAR "NextCntd" = "text:split($refCntdsRxOffset,':')[position() = (num:i($i + 1))]"!]
      [!VAR "Start" = "as:ref($Cntd)/IpduMContainedPduOffset"!]
      [!VAR "Length" = "as:ref(as:ref($Cntd)/IpduMContainedRxPduRef)/PduLength"!]
      [!VAR "End" = "as:ref($NextCntd)/IpduMContainedPduOffset"!]
      [!IF "$Start + $Length > $End"!]
        [!ERROR!][!"node:name(as:ref($NextCntd))"!]: Overlapping with [!"node:name(as:ref($Cntd))"!]. Please check IpduMContainedPduOffset or PduLength for both contained PDUs.[!ENDERROR!]
      [!ENDIF!]
    [!ENDFOR!]

    [!VAR "Cntd" = "text:split($refCntdsRxOffset,':')[last()]"!]

    [!VAR "Start" = "as:ref($Cntd)/IpduMContainedPduOffset"!]
    [!VAR "Length" = "as:ref(as:ref($Cntd)/IpduMContainedRxPduRef)/PduLength"!]
    [!VAR "End" = "as:ref(as:ref(as:path($Container))/IpduMContainerRxPduRef)/PduLength"!]
    [!IF "$Start + $Length > $End"!]
      [!ERROR!][!"node:name(as:ref($Cntd))"!]: ContainedPdu exceeds container's [!"node:name(as:ref(as:path($Container)))"!] PduLength. Please check IpduMContainedPduOffset / PduLength for contained PDU or PduLength for container PDU.[!ENDERROR!]
    [!ENDIF!]

    [!FOR "i" = "1" TO "$noOfrefCntdsRxOffset"!]
      [!VAR "Cntd" = "text:split($refCntdsRxOffset,':')[position() = $i]"!]
      [!IF "node:exists(as:ref($Cntd)/IpduMPduUpdateBitPosition)"!]
        [!VAR "BitPos" = "num:i(as:ref($Cntd)/IpduMPduUpdateBitPosition)"!]
        [!VAR "BytePos" = "num:i(as:ref($Cntd)/IpduMPduUpdateBitPosition div 8)"!]
        [!VAR "End" = "as:ref(as:ref(as:path($Container))/IpduMContainerRxPduRef)/PduLength"!]
        [!IF "$BytePos >= $End"!]
          [!ERROR!][!"node:name(as:ref($Cntd))"!]: IpduMPduUpdateBitPosition [Byte: [!"$BytePos"!]] exceeds container's [!"node:name(as:ref(as:path($Container)))"!] PduLength. Please check IpduMPduUpdateBitPosition for contained PDU or PduLength for container PDU.[!ENDERROR!]
        [!ENDIF!]

        [!FOR "j" = "1" TO "$noOfrefCntdsRxOffset"!]
          [!VAR "Cntd2" = "text:split($refCntdsRxOffset,':')[position() = $j]"!]
          [!IF "node:exists(as:ref($Cntd2)/IpduMPduUpdateBitPosition)"!]
            [!VAR "BitPos2" = "num:i(as:ref($Cntd2)/IpduMPduUpdateBitPosition)"!]
            [!VAR "Start" = "as:ref($Cntd2)/IpduMContainedPduOffset"!]
            [!VAR "Length" = "as:ref(as:ref($Cntd2)/IpduMContainedRxPduRef)/PduLength"!]
            [!VAR "End" = "num:i($Start + $Length)"!]
            [!IF "$BytePos >= $Start and $BytePos < $End"!]
              [!ERROR!][!"node:name(as:ref($Cntd))"!]: IpduMPduUpdateBitPosition [Byte: [!"$BytePos"!]] overlaps contained PDU [!"node:name(as:ref($Cntd2))"!].[!ENDERROR!]
            [!ENDIF!]
            [!IF "$i != $j and $BitPos = $BitPos2"!]
              [!ERROR!][!"node:name(as:ref($Cntd))"!]: IpduMPduUpdateBitPosition has the same value as the one for [!"node:name(as:ref($Cntd2))"!].[!ENDERROR!]
            [!ENDIF!]
          [!ENDIF!]
        [!ENDFOR!]
      [!ENDIF!]
    [!ENDFOR!]

    [!IF "num:i(count(as:modconf('IpduM')[1]/IpduMConfig/*[1]/IpduMContainedRxPdu/*/IpduMPduUpdateBitPosition [node:exists(../IpduMContainedRxInContainerPduRef) and node:name(as:ref(../IpduMContainedRxInContainerPduRef)) = node:name(node:current())])) > 0"!]
      [!VAR "maxCntdOffset" = "num:i(num:max(as:modconf('IpduM')[1]/IpduMConfig/*[1]/IpduMContainedRxPdu/*/IpduMContainedPduOffset [node:exists(../IpduMContainedRxInContainerPduRef) and node:name(as:ref(../IpduMContainedRxInContainerPduRef)) = node:name(node:current())]))"!]
      [!VAR "maxCntdUpdateBytePos" = "num:i(num:max(as:modconf('IpduM')[1]/IpduMConfig/*[1]/IpduMContainedRxPdu/*/IpduMPduUpdateBitPosition [node:exists(../IpduMContainedRxInContainerPduRef) and node:name(as:ref(../IpduMContainedRxInContainerPduRef)) = node:name(node:current())]) div 8)"!]
      [!IF "$maxCntdUpdateBytePos > $maxCntdOffset"!]
        [!ERROR!][!"node:name(as:ref(as:path($Container)))"!]: Container PDU must not end with a byte used for storing IpduMUpdateBits. It must have a contained PDU at its end.[!ENDERROR!]
      [!ENDIF!]
    [!ENDIF!]
[!ENDLOOP!]


[!ENDIF!][!// multiple inclusion protection
[!ENDNOCODE!][!//
