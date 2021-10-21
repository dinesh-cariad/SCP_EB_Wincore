[!/**
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
 */!][!//
[!/*
*** multiple inclusion protection ***
*/!][!IF "not(var:defined('CANTP_POSTBUILD_M'))"!][!/*
*/!][!VAR "CANTP_POSTBUILD_M"="'true'"!][!/*

=============================================================================
Variables that use macros for their calculation
=============================================================================

*** General variables ***

*** Tx confirmation ***
*/!][!VAR "max_tx"="-1"!][!/*
*/!][!VAR "nPduRefLUT_tx"="''"!][!/*
*/!][!LOOP "CanTpConfig/*[1]/CanTpChannel/*/CanTpTxNSdu/*/CanTpTxNPdu"!][!/*
   */!][!IF "CanTpTxNPduConfirmationPduId > $max_tx"!][!VAR "max_tx"="CanTpTxNPduConfirmationPduId"!][!ENDIF!][!/*
   */!][!IF "count(text:grep(text:split($nPduRefLUT_tx, ';'), concat('^', CanTpTxNPduConfirmationPduId, '-.*$'))) = 0"!][!/*
      */!][!VAR "nPduRefLUT_tx"!][!"$nPduRefLUT_tx"!];[!"CanTpTxNPduConfirmationPduId"!]-[!"CanTpTxNPduRef "!][!ENDVAR!][!/*
   */!][!ELSE!][!/*
      */!][!VAR "pduRef_tx"="substring-after(text:grep(text:split($nPduRefLUT_tx, ';'), concat('^', CanTpTxNPduConfirmationPduId, '-.*$'))[1], '-')"!][!/*
      */!][!ASSERT "$pduRef_tx = CanTpTxNPduRef "!]Tx N-Pdu Id [!"num:i(CanTpTxNPduConfirmationPduId)"!] is used multiple times for different PDUs[!ENDASSERT!][!/*
   */!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*
*/!][!LOOP "CanTpConfig/*[1]/CanTpChannel/*/CanTpRxNSdu/*[node:exists(CanTpTxFcNPdu)]/CanTpTxFcNPdu"!][!/*
   */!][!IF "CanTpTxFcNPduConfirmationPduId > $max_tx"!][!VAR "max_tx"="CanTpTxFcNPduConfirmationPduId"!][!ENDIF!][!/*
   */!][!IF "count(text:grep(text:split($nPduRefLUT_tx, ';'), concat('^', CanTpTxFcNPduConfirmationPduId, '-.*$'))) = 0"!][!/*
      */!][!VAR "nPduRefLUT_tx"!][!"$nPduRefLUT_tx"!];[!"CanTpTxFcNPduConfirmationPduId"!]-[!"CanTpTxFcNPduRef "!][!ENDVAR!][!/*
   */!][!ELSE!][!/*
      */!][!VAR "pduRef_tx"="substring-after(text:grep(text:split($nPduRefLUT_tx, ';'), concat('^', CanTpTxFcNPduConfirmationPduId, '-.*$'))[1], '-')"!][!/*
      */!][!ASSERT "$pduRef_tx = CanTpTxFcNPduRef "!]Tx N-Pdu Id [!"num:i(CanTpTxFcNPduConfirmationPduId)"!] is used multiple times for different PDUs[!ENDASSERT!][!/*
   */!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*
*/!][!VAR "numTxConfNPdus"="$max_tx + 1"!][!/*


*** Collect some figures ***
*/!][!VAR "numRxNSdus"="count(CanTpConfig/*[1]/CanTpChannel/*/CanTpRxNSdu/*)"!][!/*
*/!][!VAR "numTxNSdus"="count(CanTpConfig/*[1]/CanTpChannel/*/CanTpTxNSdu/*)"!][!/*

*** Checks if there is at least one PDU at all ***
*/!][!ASSERT "($numRxNSdus + $numTxNSdus) > 0", "You have to define at least one Tx- or one Rx-N-SDU"!][!/*


*** Create RX-NSDU Lookup Table and perform some checks
*/!][!VAR "rxNSduLUT" = "''"!][!/*
*/!][!LOOP "CanTpConfig/*[1]/CanTpChannel/*/CanTpRxNSdu/*"!][!/*
   *** table append ***
   */!][!VAR "rxNSduLUT"!][!"$rxNSduLUT"!];[!"num:i(CanTpRxNSduId)"!]-[!"as:path(.)"!][!ENDVAR!][!/*
*/!][!ENDLOOP!][!/*
*/!][!VAR "rxNSduLUT" = "substring-after($rxNSduLUT,';')"!][!/*

*** Create TX-NSDU Lookup Table and perform some checks ***
*/!][!VAR "txNSduLUT" = "''"!][!/*
*/!][!LOOP "CanTpConfig/*[1]/CanTpChannel/*/CanTpTxNSdu/*"!][!/*
   *** table append ***
   */!][!VAR "txNSduLUT"!][!"$txNSduLUT"!];[!"num:i(CanTpTxNSduId)"!]-[!"as:path(.)"!][!ENDVAR!][!/*
*/!][!ENDLOOP!][!/*
*/!][!VAR "txNSduLUT" = "substring-after($txNSduLUT,';')"!][!/*

*** create Channel lookup-table ***
   --> map the configured CanTp channels to the internal channel id
   first loop through all Tx channels, because those shall get ids 0 to ($numTxChannels-1)
   Format: <Internal channel index>-<CanTp channel path>;...
   Please note that for full duplex channels, in fact to internal channels are used. Therefore,
   the <internal channel index> is incremented by two for full duplex channels.
   Sorting:
   1) Full duplex Tx channels
   2) Half duplex Tx and Rx/Tx channels
   3) Full duplex Rx channels
   4) Half duplex Rx only channels
*/!][!VAR "channelLUT" = "''"!][!/*
*/!][!VAR "cidx"="0"!][!/*

   1) Full duplex Tx channels
*/!][!LOOP "CanTpConfig/*[1]/CanTpChannel/*[CanTpChannelMode='CANTP_MODE_FULL_DUPLEX']"!][!/*
   */!][!VAR "channelLUT"!][!"$channelLUT"!];[!"num:i($cidx)"!]-[!"node:path(.)"!][!ENDVAR!][!/*
   */!][!VAR "cidx"="$cidx + 1"!][!/*
*/!][!ENDLOOP!][!/*
*/!][!VAR "numFullDuplexChannels"="$cidx"!][!/*

   2) Half duplex Tx and Rx/Tx channels
*/!][!LOOP "CanTpConfig/*[1]/CanTpChannel/*[(CanTpChannelMode='CANTP_MODE_HALF_DUPLEX') and (count(CanTpTxNSdu/*) > 0)]"!][!/*
   */!][!VAR "channelLUT"!][!"$channelLUT"!];[!"num:i($cidx)"!]-[!"node:path(.)"!][!ENDVAR!][!/*
   */!][!VAR "cidx"="$cidx + 1"!][!/*
*/!][!ENDLOOP!][!/*
*/!][!VAR "numTxChannels"="$cidx"!][!/*

   3) Full duplex Rx channels (just copy from above as it was already tested there)
*/!][!LOOP "CanTpConfig/*[1]/CanTpChannel/*[CanTpChannelMode='CANTP_MODE_FULL_DUPLEX']"!][!/*
   */!][!VAR "channelLUT"!][!"$channelLUT"!];[!"num:i($cidx)"!]-[!"node:path(.)"!][!ENDVAR!][!/*
   */!][!VAR "cidx"="$cidx + 1"!][!/*
*/!][!ENDLOOP!][!/*

   4) Half duplex Rx only channels
*/!][!LOOP "CanTpConfig/*[1]/CanTpChannel/*[(CanTpChannelMode='CANTP_MODE_HALF_DUPLEX') and (count(CanTpTxNSdu/*) = 0)]"!][!/*
   */!][!VAR "channelLUT"!][!"$channelLUT"!];[!"num:i($cidx)"!]-[!"node:path(.)"!][!ENDVAR!][!/*
   */!][!VAR "cidx"="$cidx + 1"!][!/*
*/!][!ENDLOOP!][!/*
*/!][!VAR "channelLUT" = "substring-after($channelLUT,';')"!][!/*
*/!][!VAR "numAllChannels" = "count(text:split($channelLUT, ';'))"!][!/*

*** The number local indexes from channel LUT assigned for Rx Channels ***
*/!][!VAR "numRxChannels"="($numAllChannels - $numTxChannels)"!][!/*

   5) Check number of channels (CanTp.Design.ConfigChecks.NrOfChannels)
*/!][!IF "$numAllChannels > 65535"!]
    */!][!ERROR!]The CanTp supports up to 65535 internal communication channels. The current configuration needs [!"num:i($numAllChannels)"!] channels. Please reduce the number of necessary channels by either using less full duplex channels (via parameter CanTpTxChannel/CanTpChannelMode) or combine half duplex channels by using the same channel ID for the Rx and Tx N-SDUs which can be combined to one channel.[!ENDERROR!][!/*
*/!][!ENDIF!][!/*
*/!][!IF "$numTxChannels > CanTpGeneral/CanTpMaxTxChannels"!]
    */!][!ERROR!]The maximum number of full duplex and half duplex Tx channels must not exceed CanTpGeneral/CanTpMaxTxChannels [!ENDERROR!][!/*
*/!][!ENDIF!][!/*
*/!][!IF "$numRxChannels > CanTpGeneral/CanTpMaxRxChannels"!]
    */!][!ERROR!]The maximum number of full duplex and half duplex Rx channels must not exceed CanTpGeneral/CanTpMaxRxChannels [!ENDERROR!][!/*
*/!][!ENDIF!][!/*

*/!][!IF "$numAllChannels > (CanTpGeneral/CanTpMaxTxChannels + CanTpGeneral/CanTpMaxRxChannels)"!]
    */!][!ERROR!]The maximum number of full duplex and half duplex Tx plus the maximum number of full duplex and half duplex Rx channels, must not exceed CanTpGeneral/CanTpMaxTxChannels plus CanTpGeneral/CanTpMaxRxChannels! [!ENDERROR!][!/*
*/!][!ENDIF!][!/*



*** Rx-NPdu Constraints ***
  So. This is a lot of work. Rx-NPduIds are defined by the CanTp, so we have to assure that they are
  consecutive. To make things harder the same N-Pdu may be used several times, e.g. when extended
  addressing is used. So we have to assure that the same N-Pdu Id always refers to the same PDU.
  Prepare for real ugly code below ;)

  1. find greatest PDU-Id and check consistency of N-Pdu Ids and PDU References
*/!][!VAR "max"="-1"!][!/*
*/!][!VAR "nPduRefLUT"="''"!][!/*
*/!][!LOOP "CanTpConfig/*[1]/CanTpChannel/*/CanTpRxNSdu/*/CanTpRxNPdu"!][!/*
   */!][!IF "CanTpRxNPduId > $max"!][!VAR "max"="CanTpRxNPduId"!][!ENDIF!][!/*
   */!][!IF "count(text:grep(text:split($nPduRefLUT, ';'), concat('^', CanTpRxNPduId, '-.*$'))) = 0"!][!/*
      */!][!VAR "nPduRefLUT"!][!"$nPduRefLUT"!];[!"CanTpRxNPduId"!]-[!"CanTpRxNPduRef"!][!ENDVAR!][!/*
   */!][!ELSE!][!/*
      */!][!VAR "pduRef"="substring-after(text:grep(text:split($nPduRefLUT, ';'), concat('^', CanTpRxNPduId, '-.*$'))[1], '-')"!][!/*
      */!][!ASSERT "$pduRef = CanTpRxNPduRef"!]Rx N-Pdu Id [!"num:i(CanTpRxNPduId)"!] is used multiple times for different PDUs[!ENDASSERT!][!/*
   */!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*
*/!][!LOOP "CanTpConfig/*[1]/CanTpChannel/*/CanTpTxNSdu/*[node:exists(CanTpRxFcNPdu)]/CanTpRxFcNPdu"!][!/*
   */!][!IF "CanTpRxFcNPduId > $max"!][!VAR "max"="CanTpRxFcNPduId"!][!ENDIF!][!/*
   */!][!IF "count(text:grep(text:split($nPduRefLUT, ';'), concat('^', CanTpRxFcNPduId, '-.*$'))) = 0"!][!/*
      */!][!VAR "nPduRefLUT"!][!"$nPduRefLUT"!];[!"CanTpRxFcNPduId"!]-[!"CanTpRxFcNPduRef"!][!ENDVAR!][!/*
   */!][!ELSE!][!/*
      */!][!VAR "pduRef"="substring-after(text:grep(text:split($nPduRefLUT, ';'), concat('^', CanTpRxFcNPduId, '-.*$'))[1], '-')"!][!/*
      */!][!ASSERT "$pduRef = CanTpRxFcNPduRef"!]Rx N-Pdu Id [!"num:i(CanTpRxFcNPduId)"!] is used multiple times for different PDUs[!ENDASSERT!][!/*
   */!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*
*/!][!VAR "numRxNPdus"="$max + 1"!][!/*

  2. check consecutiveness, build pdusPerRxNPdu table and check that N-Sdus can be uniquely
     determined by an N-Pdu.
*/!][!VAR "pdusPerRxNPdu"="''"!][!/*
*/!][!FOR "I"="0" TO "$max"!][!/*
   */!][!VAR "numRxI"="count(CanTpConfig/*[1]/CanTpChannel/*/CanTpRxNSdu/*[CanTpRxNPdu/CanTpRxNPduId = $I])"!][!/*
   */!][!VAR "numTxI"="count(CanTpConfig/*[1]/CanTpChannel/*/CanTpTxNSdu/*[node:exists(CanTpRxFcNPdu) and (CanTpRxFcNPdu/CanTpRxFcNPduId = $I)])"!][!/*
   */!][!IF "($numRxI + $numTxI) = 0"!][!/*
      */!][!ERROR!]Rx-NPdu-IDs are not consecutive. Id [!"num:i($I)"!] is missing.[!ENDERROR!][!/*
   */!][!ENDIF!][!/*
   */!][!VAR "pdusPerRxNPdu"="concat($pdusPerRxNPdu, ';', num:i($numRxI + $numTxI))"!][!/*

   ** check that the Rx-NPdu can uniquely be mapped to CanTp-NSdu **
   (a) all associated N-Sdus have to be of the same type, i.e. all Standard or all Extended
   */!][!VAR "numRxIStd"="count(CanTpConfig/*[1]/CanTpChannel/*/CanTpRxNSdu/*[(CanTpRxNPdu/CanTpRxNPduId = $I) and (CanTpRxAddressingFormat = 'CANTP_STANDARD')])"!][!/*
   */!][!VAR "numTxIStd"="count(CanTpConfig/*[1]/CanTpChannel/*/CanTpTxNSdu/*[node:exists(CanTpRxFcNPdu) and (CanTpRxFcNPdu/CanTpRxFcNPduId = $I) and (CanTpTxAddressingFormat = 'CANTP_STANDARD')])"!][!/*
   */!][!VAR "numRxIExt"="count(CanTpConfig/*[1]/CanTpChannel/*/CanTpRxNSdu/*[(CanTpRxNPdu/CanTpRxNPduId = $I) and (CanTpRxAddressingFormat = 'CANTP_EXTENDED')])"!][!/*
   */!][!VAR "numTxIExt"="count(CanTpConfig/*[1]/CanTpChannel/*/CanTpTxNSdu/*[node:exists(CanTpRxFcNPdu) and (CanTpRxFcNPdu/CanTpRxFcNPduId = $I) and (CanTpTxAddressingFormat = 'CANTP_EXTENDED')])"!][!/*
   */!][!VAR "numRxIMix"="count(CanTpConfig/*[1]/CanTpChannel/*/CanTpRxNSdu/*[(CanTpRxNPdu/CanTpRxNPduId = $I) and (CanTpRxAddressingFormat = 'CANTP_MIXED')])"!][!/*
   */!][!VAR "numTxIMix"="count(CanTpConfig/*[1]/CanTpChannel/*/CanTpTxNSdu/*[node:exists(CanTpRxFcNPdu) and (CanTpRxFcNPdu/CanTpRxFcNPduId = $I) and (CanTpTxAddressingFormat = 'CANTP_MIXED')])"!][!/*
   */!][!IF "($numRxIStd + $numTxIStd + $numRxIMix + $numTxIMix) = 0"!][!/*
      ** all are extended --> check uniqueness of NSa values **
      */!][!LOOP "CanTpConfig/*[1]/CanTpChannel/*/CanTpRxNSdu/*[(CanTpRxNPdu/CanTpRxNPduId = $I)]"!][!/*
         */!][!IF "not(text:uniq(../../../../CanTpChannel/*/CanTpRxNSdu/*[(CanTpRxNPdu/CanTpRxNPduId = $I)]/CanTpNSa/CanTpNSa, CanTpNSa/CanTpNSa))"!][!/*
            */!][!ERROR!]Rx N-Pdu Id [!"num:i($I)"!] is used for multiple Rx N-Sdus with extended addressing mode that have an NSa value of [!"num:i(CanTpNSa/CanTpNSa)"!].[!ENDERROR!][!/*
         */!][!ENDIF!][!/*
      */!][!ENDLOOP!][!/*
      */!][!LOOP "CanTpConfig/*[1]/CanTpChannel/*/CanTpTxNSdu/*[node:exists(CanTpRxFcNPdu) and (CanTpRxFcNPdu/CanTpRxFcNPduId = $I)]"!][!/*
         */!][!IF "not(text:uniq(../../../../CanTpChannel/*/CanTpTxNSdu/*[node:exists(CanTpRxFcNPdu) and (CanTpRxFcNPdu/CanTpRxFcNPduId = $I)]/CanTpNSa/CanTpNSa, CanTpNSa/CanTpNSa))"!][!/*
            */!][!ERROR!]Rx N-Pdu Id [!"num:i($I)"!] is used for multiple Tx N-Sdus with extended addressing mode that have an NSa value of [!"num:i(CanTpNSa/CanTpNSa)"!].[!ENDERROR!][!/*
         */!][!ENDIF!][!/*
      */!][!ENDLOOP!][!/*
   */!][!ELSEIF "($numRxIStd + $numTxIStd + $numRxIExt + $numTxIExt) = 0"!][!/*
      ** all are mixed --> check uniqueness of NAe values **
      */!][!LOOP "CanTpConfig/*[1]/CanTpChannel/*/CanTpRxNSdu/*[(CanTpRxNPdu/CanTpRxNPduId = $I)]"!][!/*
         */!][!IF "not(text:uniq(../../../../CanTpChannel/*/CanTpRxNSdu/*[(CanTpRxNPdu/CanTpRxNPduId = $I)]/CanTpNAe/CanTpNAe, CanTpNAe/CanTpNAe))"!][!/*
            */!][!ERROR!]Rx N-Pdu Id [!"num:i($I)"!] is used for multiple Rx N-Sdus with mixed addressing mode that have an N_AE value of [!"num:i(CanTpNAe/CanTpNAe)"!].[!ENDERROR!][!/*
         */!][!ENDIF!][!/*
      */!][!ENDLOOP!][!/*
      */!][!LOOP "CanTpConfig/*[1]/CanTpChannel/*/CanTpTxNSdu/*[node:exists(CanTpRxFcNPdu) and (CanTpRxFcNPdu/CanTpRxFcNPduId = $I)]"!][!/*
         */!][!IF "not(text:uniq(../../../../CanTpChannel/*/CanTpTxNSdu/*[node:exists(CanTpRxFcNPdu) and (CanTpRxFcNPdu/CanTpRxFcNPduId = $I)]/CanTpNAe/CanTpNAe, CanTpNAe/CanTpNAe))"!][!/*
            */!][!ERROR!]Rx N-Pdu Id [!"num:i($I)"!] is used for multiple Tx N-Sdus with mixed addressing mode that have an N_AE value of [!"num:i(CanTpNAe/CanTpNAe)"!].[!ENDERROR!][!/*
         */!][!ENDIF!][!/*
      */!][!ENDLOOP!][!/*
   */!][!ELSEIF "($numRxIExt + $numTxIExt + $numRxIMix + $numTxIMix) = 0"!][!/*
      ** all are standard **
      */!][!IF "($numRxIStd > 1)"!][!/*
         */!][!ERROR!]Rx N-Pdu Id [!"num:i($I)"!] is used for multiple Rx N-Sdus with standard addressing mode.[!ENDERROR!][!/*
      */!][!ENDIF!][!/*
      */!][!IF "($numTxIStd > 1)"!][!/*
         */!][!ERROR!]Rx N-Pdu Id [!"num:i($I)"!] is used for multiple Tx N-Sdus with standard addressing mode.[!ENDERROR!][!/*
      */!][!ENDIF!][!/*
   */!][!ELSE!][!/*
      ** mixed --> error **
      */!][!ERROR!]Rx N-Pdu Id [!"num:i($I)"!] is used for several N-Sdus with differing addressing modes.[!ENDERROR!][!/*
   */!][!ENDIF!][!/*
*/!][!ENDFOR!][!/*
*/!][!VAR "pdusPerRxNPdu"="substring-after($pdusPerRxNPdu,';')"!]


[!/*
****************************************************************************************************
* Create a list of CanIfTxPduRef and the corresponding CanIfTxPduCanIdTypes.
* - CanIfTxRefToCanIdTypeList will hold all source CanIdTypes of all CanIf TxPdus.
* Each list is realized as continuous string. The string contains cuples separated by ";". Each
* cuple contains two elements separated by ":". First element of cuple is the CanIf PDU reference. The
* Second element of the cuple is the corresponding CanIdType.
* A list, for example, looks like this:
* Pdu_0:STANDARD_CAN;  Pdu_1:STANDARD_FD_CAN;  Pdu_3:EXTENDED_CAN;  Pdu_4:EXTENDED_FD_CAN;
* The location of the PDU AUTOSAR/TOP-LEVEL-PACKAGES/EcuC/ELEMENTS/EcuC/EcucPduCollection/Pdu/ is
* not shown in the example.
****************************************************************************************************
*/!]

[!VAR "CanIfTxRefToCanIdTypeList"="''"!]
[!// Iterate over all Tx PDUs to collect the CanIdTypes
[!LOOP "as:modconf('CanIf')[1]/CanIfInitCfg/*/CanIfTxPduCfg/*"!]
    [!// Get the CanIfTxPduCanIdType of a Tx PDU
    [!VAR "CanIdType" = "./CanIfTxPduCanIdType"!]
    [!IF "node:exists(./CanIfTxPduRef)"!]
         [!VAR "CanIfPduRef" = "node:path(node:ref(./CanIfTxPduRef))"!]
         [!VAR "CanIfTxRefToCanIdTypeList"!][!"$CanIfTxRefToCanIdTypeList"!];[!"$CanIfPduRef"!]:[!"$CanIdType"!][!ENDVAR!]
    [!ENDIF!]
    [!// Get the CanIfTxPduCanIdType of a Tx PDU
[!ENDLOOP!]



 [!/*
 ****************************************************************************************************
 * Create a list of CanIfRxPduRefs and the corresponding CanIfRxPduCanIdTypes.
 * - CanIfRxRefToCanIdTypeList will hold all source CanIdTypes of all CanIf RxPdus.
 * Each list is realized as continuous string. The string contains cuples separated by ";". Each
 * cuple contains two elements separated by ":". First element of cuple is the CanIf PDU reference. The
 * Second element of the cuple is the corresponding CanIdType.
 * A list, for example, looks like this:
 * Pdu_0:STANDARD_CAN;  Pdu_1:STANDARD_FD_CAN;  Pdu_2:STANDARD_NO_FD_CAN; Pdu_3:EXTENDED_CAN;  Pdu_4:EXTENDED_FD_CAN; Pdu_5:EXTENDED_NO_FD_CAN;
 * The location of the PDU AUTOSAR/TOP-LEVEL-PACKAGES/EcuC/ELEMENTS/EcuC/EcucPduCollection/Pdu/ is
 * not shown in the example.
 ****************************************************************************************************
 */!]

 [!VAR "CanIfRxRefToCanIdTypeList"="''"!]
 [!// Iterate over all Tx PDUs to collect the CanIdTypes
 [!LOOP "as:modconf('CanIf')[1]/CanIfInitCfg/*/CanIfRxPduCfg/*"!]
     [!// Get the CanIfTxPduCanIdType of a Tx PDU
     [!VAR "CanIdType" = "./CanIfRxPduCanIdType"!]
     [!IF "node:exists(./CanIfRxPduRef)"!]
          [!VAR "CanIfPduRef" = "node:path(node:ref(./CanIfRxPduRef))"!]
          [!VAR "CanIfRxRefToCanIdTypeList"!][!"$CanIfRxRefToCanIdTypeList"!];[!"$CanIfPduRef"!]:[!"$CanIdType"!][!ENDVAR!]
     [!ENDIF!]
     [!// Get the CanIfTxPduCanIdType of a Tx PDU
 [!ENDLOOP!]



[!/*
*** Tx N-Pdu constraints ***
  These are simple: no Tx N-Pdu may be used twice.
*/!][!VAR "txNPdus"="''"!][!/*
*/!][!LOOP "CanTpConfig/*[1]/CanTpChannel/*/CanTpRxNSdu/*[node:exists(CanTpTxFcNPdu)]/CanTpTxFcNPdu/CanTpTxFcNPduRef"!][!/*
   */!][!IF "count(text:grep(text:split($txNPdus, ';'), .)) = 0"!][!/*
      */!][!VAR "txNPdus"!][!"$txNPdus"!];[!"."!][!ENDVAR!][!/*
   */!][!ELSE!][!/*
      */!][!ERROR!]Tx N-PDU [!"name(as:ref(.))"!] is used more than once.[!ENDERROR!][!/*
   */!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*

*/!][!LOOP "CanTpTxNSdu/*/CanTpTxNPdu/CanTpTxNPduRef"!][!/*
   */!][!IF "count(text:grep(text:split($txNPdus, ';'), .)) = 0"!][!/*
      */!][!VAR "txNPdus"!][!"$txNPdus"!];[!"."!][!ENDVAR!][!/*
   */!][!ELSE!][!/*
      */!][!ERROR!]Tx N-PDU [!"name(as:ref(.))"!] is used more than once.[!ENDERROR!][!/*
   */!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*


*** Calculation macros ***

*** macro for validation of PduLength from EcuC, referenced by a CanTpTxNPdu
    NOTE: this macro shall only be called from within a loop over CanTpTxNSdu!

*/!][!MACRO "getTxPduLengthDependentOfPadding", "TxNPduRef", "Padding", "AddressingFormat", "TaType"!][!/*
     */!][!VAR "PduRefPath" = "node:path(node:ref($TxNPduRef))"!][!/*
     */!][!VAR "PduLengthValue" = "node:value(node:ref($TxNPduRef)/PduLength)"!][!/*
     */!][!LOOP "text:split($CanIfTxRefToCanIdTypeList,';')"!][!/*
          */!][!IF "(string(text:split(.,':')[1]) = $PduRefPath)"!][!/*
                */!][!VAR "CanIdType" = "string(text:split(.,':')[2])"!][!/*

                */!][!IF "($CanIdType = 'STANDARD_CAN') or ($CanIdType = 'EXTENDED_CAN')"!][!/*
                */!][!IF "($AddressingFormat = 'CANTP_STANDARD')"!][!/*
                    */!][!IF "($Padding = 'CANTP_OFF') and ($TaType = 'CANTP_FUNCTIONAL') and ($PduLengthValue <= 1)"!][!/*
                        */!][!ERROR!] The PduLength of [!"$PduRefPath"!] shall be big enough to hold 1 byte of N_PCI plus at least one byte of data. PduLength shall be bigger than 1![!ENDERROR!][!/*
                    */!][!ENDIF!][!/*
                */!][!ELSE!][!/*
                    */!][!IF "($Padding = 'CANTP_OFF') and ($TaType = 'CANTP_FUNCTIONAL') and ($PduLengthValue <= 2)"!][!/*
                        */!][!ERROR!] The PduLength of [!"$PduRefPath"!] shall be big enough to hold 2 bytes of N_PCI plus at least one byte of data. PduLength shall be bigger than 2![!ENDERROR!][!/*
                    */!][!ENDIF!][!/*
                */!][!ENDIF!][!/*
                */!][!ENDIF!][!/*

                *** Check that PduLength of a STANDARD_FD_CAN or EXTENDED_FD_CAN CanIdType is at least 8 ***
                */!][!IF "((($CanIdType = 'STANDARD_FD_CAN') or ($CanIdType = 'EXTENDED_FD_CAN')) and ($PduLengthValue < 8)) "!][!/*
                  */!][!ERROR!] The PduLength of [!"$PduRefPath"!] shall be greater or equal than 8, for a STANDARD_FD_CAN or EXTENDED_FD_CAN, independent of padding![!ENDERROR!][!/*
                */!][!ENDIF!][!/*

                *** Check that PduLength of an NSdu with CANTP_PHYSICAL TaType is always greater or equal to 8 bytes
                    (because a valid FF has always a CAN_DL value greater than or equal to 8) ***
                 */!][!IF "(($TaType = 'CANTP_PHYSICAL') and ($PduLengthValue < 8)) "!][!/*
                  */!][!ERROR!] The PduLength of [!"$PduRefPath"!] shall be greater or equal than 8, for an NSdu with CANTP_PHYSICAL TaType, because a valid FF always has a CAN_DL value greater than or equal to 8![!ENDERROR!][!/*
                */!][!ENDIF!][!/*

                */!][!IF "($CanIdType != 'STANDARD_FD_CAN' and $CanIdType != 'EXTENDED_FD_CAN')"!][!/*
                  */!][!IF "($PduLengthValue > 8)"!][!/*
                    */!][!ERROR!] The PduLength of [!"$PduRefPath"!] shall not be greater than 8, for [!"$CanIdType"!] CanIdType. [!ENDERROR!][!/*
                  */!][!ELSE!][!/*
                    */!][!IF "(($Padding = 'CANTP_ON_CAN_CAN_FD') and ($PduLengthValue != num:i(8)))"!][!/*
                      */!][!ERROR!] The PduLength of [!"$PduRefPath"!] shall be equal to 8, for CAN 2.0 frames, if [!"$Padding"!] padding value is configured. [!ENDERROR!][!/*
                    */!][!ENDIF!][!/*
                  */!][!ENDIF!][!/*
                */!][!ELSE!][!/*
                  */!][!IF "($Padding = 'CANTP_ON_CAN_CAN_FD') and ($PduLengthValue != num:i(64))"!][!/*
                    */!][!ERROR!] The PduLength of [!"$PduRefPath"!] shall be equal to 64, for CAN FD frames, if [!"$Padding"!] padding value is configured. [!ENDERROR!][!/*
                  */!][!ENDIF!][!/*
               */!][!ENDIF!][!/*

          */!][!ENDIF!][!/*
     */!][!ENDLOOP!][!/*
*/!][!ENDMACRO!][!/*



*** macro for validation of PduLength from EcuC, referenced by a CanTpRxNPdu
    NOTE: this macro shall only be called from within a loop over CanTpRxNSdu!

*/!][!MACRO "getRxPduLengthDependentOfPadding", "RxNPduRef", "Padding", "AddressingFormat", "TaType"!][!/*
     */!][!VAR "PduRefPath" = "node:path(node:ref($RxNPduRef))"!][!/*
     */!][!VAR "PduLengthValue" = "node:value(node:ref($RxNPduRef)/PduLength)"!][!/*
     */!][!LOOP "text:split($CanIfRxRefToCanIdTypeList,';')"!][!/*

         */!][!IF "(string(text:split(.,':')[1]) = $PduRefPath)"!][!/*
             */!][!VAR "RxCanIdType" = "string(text:split(.,':')[2])"!][!/*

             */!][!IF "($RxCanIdType = 'STANDARD_NO_FD_CAN') or ($RxCanIdType = 'EXTENDED_NO_FD_CAN')"!][!/*
                */!][!IF "($AddressingFormat = 'CANTP_STANDARD')"!][!/*
                    */!][!IF "($Padding = 'CANTP_OFF') and ($TaType = 'CANTP_FUNCTIONAL') and ($PduLengthValue <= 1)"!][!/*
                        */!][!ERROR!] The PduLength of [!"$PduRefPath"!] shall be big enough to hold 1 byte of N_PCI plus at least one byte of data. PduLength shall be bigger than 1![!ENDERROR!][!/*
                    */!][!ENDIF!][!/*
                */!][!ELSE!][!/*
                    */!][!IF "($Padding = 'CANTP_OFF') and ($TaType = 'CANTP_FUNCTIONAL') and ($PduLengthValue <= 2)"!][!/*
                        */!][!ERROR!] The PduLength of [!"$PduRefPath"!] shall be big enough to hold 2 bytes of N_PCI plus at least one byte of data. PduLength shall be bigger than 2![!ENDERROR!][!/*
                    */!][!ENDIF!][!/*
                */!][!ENDIF!][!/*
              */!][!ENDIF!][!/*

              *** Check that PduLength of a STANDARD_FD_CAN or EXTENDED_FD_CAN CanIdType is at least 8 ***
              */!][!IF "((($RxCanIdType = 'STANDARD_FD_CAN') or ($RxCanIdType = 'EXTENDED_FD_CAN')) and ($PduLengthValue < 8)) "!][!/*
                */!][!ERROR!] The PduLength of [!"$PduRefPath"!] shall be greater or equal than 8, for a STANDARD_FD_CAN or EXTENDED_FD_CAN, independent of padding![!ENDERROR!][!/*
              */!][!ENDIF!][!/*

              *** Check that PduLength of an NSdu with CANTP_PHYSICAL TaType is always greater or equal to 8 bytes
                  (because a valid FF has always a CAN_DL value greater than or equal to 8) ***
              */!][!IF "(($TaType = 'CANTP_PHYSICAL') and ($PduLengthValue < 8)) "!][!/*
                */!][!ERROR!] The PduLength of [!"$PduRefPath"!] shall be greater or equal than 8, for an NSdu with CANTP_PHYSICAL TaType, because a valid FF always has a CAN_DL value greater than or equal to 8![!ENDERROR!][!/*
              */!][!ENDIF!][!/*


             */!][!IF "($RxCanIdType = 'STANDARD_NO_FD_CAN' or $RxCanIdType = 'EXTENDED_NO_FD_CAN')"!][!/*
               */!][!IF "($PduLengthValue > 8)"!][!/*
                 */!][!ERROR!] The PduLength of [!"$PduRefPath"!] shall not be greater than 8, for [!"$RxCanIdType"!] CanIdType. [!ENDERROR!][!/*
               */!][!ELSE!][!/*
                 */!][!IF "($Padding = 'CANTP_ON_CAN_CAN_FD') and ($PduLengthValue != num:i(8))"!][!/*
                   */!][!ERROR!] The PduLength of [!"$PduRefPath"!] shall be equal to 8, for CAN 2.0 frames, if [!"$Padding"!] padding value is configured. [!ENDERROR!][!/*
                 */!][!ENDIF!][!/*
               */!][!ENDIF!][!/*
             */!][!ELSE!][!/*
               */!][!IF "($Padding = 'CANTP_ON_CAN_CAN_FD')"!][!/*
                 */!][!IF "(($RxCanIdType = 'STANDARD_FD_CAN') or ($RxCanIdType = 'EXTENDED_FD_CAN'))"!][!/*
                   */!][!IF "($PduLengthValue != num:i(64))"!][!/*
                     */!][!ERROR!] The PduLength of [!"$PduRefPath"!] shall be equal to 64, for CAN FD frames, if [!"$Padding"!] padding value is configured. [!ENDERROR!][!/*
                   */!][!ENDIF!][!/*
                 */!][!ELSE!][!/*
                   */!][!IF "(($PduLengthValue != num:i(8)) and ($PduLengthValue != num:i(64)))"!][!/*
                     */!][!ERROR!] The PduLength of [!"$PduRefPath"!] shall be equal to 8 (for CAN 2.0 frames) or to 64 (for CAN FD frames), if [!"$Padding"!] padding value is configured. [!ENDERROR!][!/*
                   */!][!ENDIF!][!/*
                 */!][!ENDIF!][!/*
               */!][!ENDIF!][!/*
             */!][!ENDIF!][!/*

         */!][!ENDIF!][!/*
     */!][!ENDLOOP!][!/*
*/!][!ENDMACRO!][!/*

*** macro to get the corresponding Tx channel referencing the same TxPdu
    NOTE: this macro shall only be called from within a loop over CanTpRxNSdu!

*/!][!MACRO "getTxChannel", "RxNSdu"!][!/*
*/!][!VAR "TxCh"="''"!][!/*
       */!][!IF "../../CanTpChannelMode = 'CANTP_MODE_HALF_DUPLEX' or not(node:exists(CanTpTxFcNPdu)) "!][!/*
           */!][!IF "($numTxChannels > 256)"!][!/*
             */!][!VAR "TxCh"="65535"!][!/*
           */!][!ELSE!][!/*
             */!][!VAR "TxCh"="255"!][!/*
           */!][!ENDIF!][!/*
       */!][!ELSE!][!/*
           */!][!VAR "common_pdu_found" = "'false'"!][!/*
           */!][!VAR "fcPduID" = "CanTpTxFcNPdu/CanTpTxFcNPduConfirmationPduId"!][!/*
           */!][!FOR "x"="1" TO "count(as:modconf('CanTp')[1]/CanTpConfig/*[1]/CanTpChannel/*/CanTpTxNSdu/*)"!][!/*
           */!][!SELECT "as:modconf('CanTp')[1]/CanTpConfig/*[1]/CanTpChannel/*/CanTpTxNSdu/*[position()=$x]"!][!/*
           */!][!IF "CanTpTxNPdu/CanTpTxNPduConfirmationPduId = $fcPduID"!][!/*
              */!][!VAR "TxCh"!][!CALL "getChanIdByChanRef", "chanRef"="node:path(../..)"!]"!][!ENDVAR!][!/*
              */!][!VAR "common_pdu_found" = "'true'"!][!/*
              */!][!ENDIF!][!/*
           */!][!ENDSELECT!][!/*
           */!][!ENDFOR!][!/*
           */!][!IF "$common_pdu_found = 'false'"!][!/*
              */!][!IF "($numTxChannels > 256)"!][!/*
                 */!][!VAR "TxCh"="65535"!][!/*
              */!][!ELSE!][!/*
                 */!][!VAR "TxCh"="255"!][!/*
              */!][!ENDIF!][!/*
           */!][!ENDIF!][!/*
        */!][!ENDIF!][!/*
   */!][!"num:i($TxCh)"!][!/*
   */!][!/*
*/!][!ENDMACRO!][!/*

*** macro to get the corresponding Rx channel referencing the same TxFcPdu
    NOTE: this macro shall only be called from within a loop over CanTpTxNSdu!

*/!][!MACRO "getRxChannel", "TxNSdu"!][!/*
*/!][!VAR "RxCh"="''"!][!/*
       */!][!IF "../../CanTpChannelMode = 'CANTP_MODE_HALF_DUPLEX' "!][!/*
           */!][!IF "((($numAllChannels * 2) - ($numAllChannels - $numFullDuplexChannels) - 1) > 255)"!][!/*
             */!][!VAR "RxCh"="65535"!][!/*
           */!][!ELSE!][!/*
             */!][!VAR "RxCh"="255"!][!/*
           */!][!ENDIF!][!/*
       */!][!ELSE!][!/*
           */!][!VAR "common_pdu_found_tx" = "'false'"!][!/*
           */!][!VAR "txPduID" = "CanTpTxNPdu/CanTpTxNPduConfirmationPduId"!][!/*
           */!][!FOR "x"="1" TO "count(as:modconf('CanTp')[1]/CanTpConfig/*[1]/CanTpChannel/*/CanTpRxNSdu/*)"!][!/*
           */!][!SELECT "as:modconf('CanTp')[1]/CanTpConfig/*[1]/CanTpChannel/*/CanTpRxNSdu/*[position()=$x]"!][!/*
           */!][!IF "CanTpTxFcNPdu/CanTpTxFcNPduConfirmationPduId = $txPduID"!][!/*
              */!][!VAR "chId"!][!CALL "getChanIdByChanRef", "chanRef"="node:path(../..)"!][!ENDVAR!][!/*
              */!][!WS "0"!][!/* if the channel is a full duplex channel, the Rx N-SDU is assigned to the Rx part of the channel(ID + Nr. of Tx channels)*/!][!/*
              */!][!IF "../../CanTpChannelMode = 'CANTP_MODE_FULL_DUPLEX'"!][!/*
              */!][!VAR "chId"="$chId + $numTxChannels"!][!/*
              */!][!ENDIF!][!/*
              */!][!VAR "RxCh"="$chId"!][!/*
              */!][!VAR "common_pdu_found_tx" = "'true'"!][!/*
              */!][!ENDIF!][!/*
           */!][!ENDSELECT!][!/*
           */!][!ENDFOR!][!/*
           */!][!IF "$common_pdu_found_tx = 'false'"!][!/*
              */!][!IF "((($numAllChannels * 2) - ($numAllChannels - $numFullDuplexChannels) - 1) > 255)"!][!/*
                 */!][!VAR "RxCh"="65535"!][!/*
              */!][!ELSE!][!/*
                 */!][!VAR "RxCh"="255"!][!/*
              */!][!ENDIF!][!/*
           */!][!ENDIF!][!/*
        */!][!ENDIF!][!/*
   */!][!"num:i($RxCh)"!][!/*
   */!][!/*
*/!][!ENDMACRO!][!/*

*** macro for calculating mainfunction ticks for an N[abc][rs] value. ***
    NOTE: this macro shall only be called from within a
          loop over CanTp[TR]xNSdu!
    NOTE: don't call this for N_Br (as N_Br=0 is allowed)
*/!][!MACRO "calcNAbcRsValue", "value", "name", "direction"!][!/*
   */!][!VAR "mValue" = "num:i(($value div ../../../../CanTpMainFunctionPeriod) - 0.00001) + 1"!][!/*
   */!][!VAR "dValue" = "$value div ../../../../CanTpMainFunctionPeriod"!][!/*
   */!][!IF "(($dValue - $mValue) > 0.00001) or (($dValue - $mValue) < -0.00001)"!][!/*
      */!][!WARNING!][!"$name"!] value of [!"$direction"!]-NSdu [!"name(.)"!] ([!"$value"!]s) is not divisible by CanTpMainFunctionPeriod. It will be rounded to: [!"$mValue * ./CanTpMainFunctionPeriod"!]s.[!ENDWARNING!][!/*
   */!][!ENDIF!][!/*
   */!][!"num:i($mValue)"!][!/*
   */!][!/*
*/!][!ENDMACRO!][!/*

*** macro for calculating mainfunction ticks for an Nbr value. ***
    NOTE: this macro shall only be called from within a
          loop over CanTpTxNSdu!
*/!][!MACRO "calcNbrValue", "value", "name", "direction"!][!/*
   */!][!VAR "mValue" = "num:i(($value div ../../../../CanTpMainFunctionPeriod) - 0.00001) + 1"!][!/*
   */!][!IF "$value = 0"!][!VAR "mValue" = "0"!][!ENDIF!][!/*
   */!][!VAR "dValue" = "$value div ../../../../CanTpMainFunctionPeriod"!][!/*
   */!][!IF "(($dValue - $mValue) > 0.00001) or (($dValue - $mValue) < -0.00001)"!][!/*
      */!][!WARNING!][!"$name"!] value of [!"$direction"!]-NSdu [!"name(.)"!] ([!"$value"!]s) is not divisible by CanTpMainFunctionPeriod. It will be rounded to: [!"$mValue * ./CanTpMainFunctionPeriod"!]s.[!ENDWARNING!][!/*
   */!][!ENDIF!][!/*
   */!][!"num:i($mValue)"!][!/*
   */!][!/*
*/!][!ENDMACRO!][!/*


*** Lookup macros ***
*/!][!MACRO "getRxNSduRefByNSduId", "nSduId"!][!/*
   */!][!"substring-after(text:grep(text:split($rxNSduLUT,';'), concat('^', $nSduId, '-.*'))[1], '-')"!][!/*
*/!][!ENDMACRO!][!/*

*/!][!MACRO "getRxNSduIdByNSduRef", "nSduRef"!][!/*
   */!][!"substring-before(text:grep(text:split($rxNSduLUT,';'), concat('.*-', $nSduRef, '$'))[1], '-')"!][!/*
*/!][!ENDMACRO!][!/*

*/!][!MACRO "getTxNSduRefByNSduId", "nSduId"!][!/*
   */!][!"substring-after(text:grep(text:split($txNSduLUT,';'), concat('^', $nSduId, '-.*'))[1], '-')"!][!/*
*/!][!ENDMACRO!][!/*

*/!][!MACRO "getTxNSduIdByNSduRef", "nSduRef"!][!/*
   */!][!"substring-before(text:grep(text:split($txNSduLUT,';'), concat('.*-', $nSduRef, '$'))[1], '-')"!][!/*
*/!][!ENDMACRO!][!/*

*/!][!MACRO "getChanIdByChanRef", "chanRef"!][!/*
   */!][!"substring-before(text:grep(text:split($channelLUT,';'), concat('.*-', $chanRef, '$'))[1], '-')"!][!/*
*/!][!ENDMACRO!][!/*

*/!][!MACRO "getChanRefByChanId", "chanId"!][!/*
   */!][!"substring-after(text:grep(text:split($channelLUT,';'), concat('^', $chanId, '-.*$'))[1], '-')"!][!/*
*/!][!ENDMACRO!][!/*

*/!][!MACRO "getNumRxNPdusById", "pduId"!][!/*
   */!][!"text:split($pdusPerRxNPdu, ';')[position() = ($pduId + 1)]"!][!/*
*/!][!ENDMACRO!][!/*

*** Array for holding Shortnames ***
*/!][!VAR "ShortNameList"="''"!][!/*

*** MACRO to get the string for the symbolic name value.  ***
 * "ShortNameRef" must reference the container holding the short name. (AUTOSAR short name)
 * "OldName" must hold an unique string for the case that the specification

*/!][!MACRO "GetSymbolName","ShortNameRef","OldName"!][!/*
*/!][!VAR "SymbolName" = "asc:getShortName($ShortNameRef)"!][!/*
*/!][!IF "$SymbolName = ''"!][!/*
   */!][!VAR "SymbolName" = "concat(name(../../.),'_',name(.),'_',$OldName)"!][!/*
*/!][!ELSE!][!/*
   */!][!VAR "ShortNameList" = "concat($ShortNameList,' ',$SymbolName)"!][!/*
   */!][!IF "not(text:uniq(text:split($ShortNameList), $SymbolName))"!][!/*
      */!][!ERROR!] The short name [!"$SymbolName"!] of the schema node [!"$OldName"!] is not unique.[!ENDERROR!][!/*
   */!][!ENDIF!][!/*
   */!][!VAR "SymbolName" = "concat($OldName,'_',$SymbolName)"!][!/*
*/!][!ENDIF!][!/*
*/!][!ENDMACRO!]

[!/*
*** End of file ***
*/!][!ENDIF!][!//
