[!/**
 * \file
 *
 * \brief AUTOSAR FrNm
 *
 * This file contains the implementation of the AUTOSAR
 * module FrNm.
 *
 * \version 5.16.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!/* These checks were originally present as INVALID checks in FrNm.xdm.
   * Since these inter-module checks involve parameters from different
   * configuration classes, it's no more possible to keep them in FrNm.xdm.
   * Refer ASCFRNM-588 for more details.
   */!]

[!/* *** multiple inclusion protection *** */!]
[!IF "not(var:defined('FRNM_CHECKS_M'))"!]
[!VAR "FRNM_CHECKS_M" = "'true'"!]

[!NOCODE!][!//

[!/* === Inter-module checks between FrNm and FrIf, EcuC or PduR === */!]

[!/* === General checks === */!]

[!/* === Check if channel handles are zero-based and unique === */!]
[!/* === This check has been moved from FrNmChannelHandle in FrNm.xdm.m4 === */!]
[!/* !LINKSTO FrNm.EB.FrIfClstIdx,1 */!]
[!IF "not(node:isconsecutive(node:refs(FrNmChannelConfig/*/FrNmChannel/*/
          FrNmChannelIdentifiers/FrNmChannelHandle)/FrIfClstIdx, 0))"!]
  [!ERROR!]"The referenced FrIf Cluster Ids are not zero-based consecutive!"
  [!ENDERROR!]
[!ENDIF!]

[!/* === Ensure that the routing path of the EIRA Rx NSdu is configured in PduR === */!]
[!/* === This check has been moved from FrNmPnEiraRxNSduRef in FrNm.xdm.m4 === */!]
[!SELECT "as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalFeatures"!]
  [!IF "((FrNmPnEiraCalcEnabled = 'true') and
         (not(node:exists(as:modconf('PduR')[1]/PduRRoutingTables/*/
          PduRRoutingTable/*/PduRRoutingPath/*/PduRSrcPdu[(node:exists(PduRSrcPduRef)) and
          (PduRSrcPduRef = node:current()/FrNmPnEiraRxNSduRef)]/PduRSourcePduHandleId))))"!]
    [!ERROR!]"The routing path of the EIRA Rx NSdu (FrNmPnEiraRxNSduRef) is not configured in PduR."
    [!ENDERROR!]
  [!ENDIF!]
[!ENDSELECT!]

[!/* === Ensure that the routing path of the ERA Rx NSdu is configured in PduR === */!]
[!IF "as:modconf('FrNm')[1]/FrNmGeneral/FrNmPnSupported = 'true'"!]
  [!LOOP "node:order(as:modconf('FrNm')[1]/FrNmChannelConfig/*[1]/FrNmChannel/*, 'node:value(node:ref(FrNmChannelIdentifiers/FrNmChannelHandle)/FrIfClstIdx)')"!]
    [!IF "FrNmChannelIdentifiers/FrNmPnEraCalcEnabled = 'true'"!]
      [!IF "not(node:exists(as:modconf('PduR')[1]/PduRRoutingTables/*/PduRRoutingTable/*/PduRRoutingPath/*/PduRSrcPdu[(node:exists(PduRSrcPduRef)) and (PduRSrcPduRef= node:current()/FrNmChannelIdentifiers/FrNmPnEraRxNSduRef)]/PduRSourcePduHandleId))"!]
        [!ERROR!]"In FrNm for channel: [!"node:name(.)"!], the pdu referenced by FrNmPnEraRxNSduRef is not configured in PduR"
        [!ENDERROR!]
      [!ENDIF!]
    [!ENDIF!]
  [!ENDLOOP!]
[!ENDIF!]

[!/* === Perform the following checks for each channel configured === */!]
[!/* === Following checks have been moved from FrNmRxPduRef in FrNm.xdm.m4 === */!]
[!LOOP "FrNmChannelConfig/*/FrNmChannel/*/FrNmChannelIdentifiers"!]

  [!/* === Checks for Rx Pdus === */!]
  [!LOOP "FrNmRxPdu/*"!]

    [!/* === Ensure that Rx Pdu length is in range === */!]
    [!/* !LINKSTO FRNM154_Refine,1 */!]
    [!IF "((node:ref(FrNmRxPduRef)/PduLength < 1) or
           (node:ref(FrNmRxPduRef)/PduLength > 4294967295))"!]
      [!ERROR!]"Length of the Rx Pdu [!"node:name(.)"!] in channel '[!"node:name(../../..)"!]' is out of range (1-4294967295)."
      [!ENDERROR!]
    [!ENDIF!]

    [!/* === Ensure that Rx Pdu is large enough to hold Node Identifier === */!]
    [!IF "((FrNmRxPduContainsData = 'true' ) and
           (../../FrNmSourceNodeIdentifierEnabled = 'true') and
           (node:ref(FrNmRxPduRef)/PduLength < 2))"!]
      [!ERROR!]"'FrNmSourceNodeIdentifierEnabled' is set to true and 'FrNmRxPduContainsData' is enabled for channel [!"node:name(../../..)"!], but the Rx Pdu '[!"node:name(.)"!]' is too small (< 2) to hold the Node Identifier!"
      [!ENDERROR!]
    [!ENDIF!]

    [!/* === Ensure that Rx Pdu Data Length matches with the Tx Pdu Data Length === */!]
    [!/* !LINKSTO FrNm.EB.RxTxPduLength,1 */!]
    [!IF "((FrNmRxPduContainsData = 'true') and
            (node:exists(../../FrNmTxPdu/*[FrNmTxPduContainsData = 'true'])) and
            (node:ref(FrNmRxPduRef)/PduLength !=
            node:ref(../../FrNmTxPdu/*[FrNmTxPduContainsData = 'true']/FrNmTxPduRef)/PduLength))"!]
      [!ERROR!]"Rx Data Pdu Length for channel '[!"node:name(../../..)"!]' does not match with the Tx Pdu Data Length!"
      [!ENDERROR!]
    [!ENDIF!]

    [!/* === Ensure that Rx Pdu length is consistent for both Rx Pdus === */!]
    [!/* !LINKSTO FrNm.EB.FrNmRxPduRef,1 */!]
    [!IF "num:max(node:refs(../*/FrNmRxPduRef)/PduLength) !=
          num:i(node:ref(FrNmRxPduRef)/PduLength)"!]
      [!ERROR!]"The lengths of the two Rx Pdus (FrNmRxPdu) in channel '[!"node:name(../../..)"!]' are not equal."
      [!ENDERROR!]
    [!ENDIF!]

    [!/* === Ensure that Rx Pdu is large enough to hold PN Info === */!]
    [!/* !LINKSTO FrNm.EB.FrNmPnInfoLength,1 */!]
    [!IF "(../../FrNmPnEnabled  = 'true') and (FrNmRxPduContainsData = 'true') and
          (node:refvalid(FrNmRxPduRef)) and (node:ref(FrNmRxPduRef)/PduLength <
          ((as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalFeatures/FrNmPnInfo/FrNmPnInfoLength) +
          (as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalFeatures/FrNmPnInfo/FrNmPnInfoOffset)))"!]
      [!ERROR!]"Partial networking is enabled for channel '[!"node:name(../../..)"!]', but length of Rx Pdu '[!"node:name(.)"!]' is less than (FrNmPnInfoLength + FrNmPnInfoOffset)."
      [!ENDERROR!]
    [!ENDIF!]

  [!ENDLOOP!]
  [!/* === Checks for Tx Pdus === */!]
  [!/* === Following checks have been moved from FrNmTxPduRef in FrNm.xdm.m4 === */!]
  [!LOOP "FrNmTxPdu/*"!]

    [!/* === Ensure that Tx Pdu length is not 0 === */!]
    [!IF "(node:ref(FrNmTxPduRef)/PduLength = 0)"!]
      [!ERROR!]"The length of the Tx Pdu [!"node:name(.)"!] in channel '[!"node:name(../../..)"!]' is 0!"
      [!ENDERROR!]
    [!ENDIF!]

    [!/* === Ensure that Tx Pdu is not larger than Rx Pdus === */!]
    [!/* !LINKSTO FRNM154_Refine,1 */!]
    [!IF "(num:max(node:refs(../../FrNmRxPdu/*/FrNmRxPduRef)/PduLength) <
           num:i(node:ref(FrNmTxPduRef)/PduLength))"!]
      [!ERROR!]"The length of the Tx Pdu '[!"node:name(.)"!]' is greater than the length of at least one Rx Pdu in channel [!"node:name(../../..)"!]."
      [!ENDERROR!]
    [!ENDIF!]

    [!/* === Ensure that Tx Pdu is large enough to hold Node Identifier === */!]
    [!IF "((FrNmTxPduContainsData = 'true') and
           (../../FrNmSourceNodeIdentifierEnabled = 'true') and
           (node:ref(FrNmTxPduRef)/PduLength < 2))"!]
      [!ERROR!]"'FrNmSourceNodeIdentifierEnabled' is set to true and 'FrNmTxPduContainsData' is enabled for channel '[!"node:name(../../..)"!]', but the Tx Pdu '[!"node:name(.)"!]' is too small (< 2) to hold the Node Identifier!"
      [!ENDERROR!]
    [!ENDIF!]

    [!/* === Ensure that exactly one Tx Pdu is configured in FrIf === */!]
    [!/* !LINKSTO FrNm.EB.FrIfTxPduRef,1 */!]
    [!IF "count(as:modconf('FrIf')[1]/FrIfConfig/*/FrIfPdu/*/FrIfPduDirection[
          (node:exists(FrIfTxPduRef)) and (FrIfTxPduRef = node:current()/FrNmTxPduRef)]) != 1"!]
      [!ERROR!]"No unique Tx Pdu is configured in FrIf via 'FrIfTxPduRef' for the Tx Pdu [!"node:name(.)"!] in channel [!"node:name(../../..)"!]."
      [!ENDERROR!]
    [!ENDIF!]

  [!ENDLOOP!]

  [!/* === Checks for User Data Tx Pdus === */!]
  [!/* === Following checks have been moved from FrNmTxUserDataPduRef in FrNm.xdm.m4 === */!]
  [!SELECT "FrNmUserDataTxPdu"!]

    [!/* === Ensure that FrNmUserDataTxPdu is disabled when User Data Length is 0 === */!]
    [!/* !LINKSTO FrNm.ComUserDataSupport.UserDataLength.NoUserData,1 */!]
    [!IF "(node:refvalid(../FrNmTxPdu/*[FrNmTxPduContainsData = 'true']/FrNmTxPduRef)) and
           not(((../FrNmSourceNodeIdentifierEnabled = 'true') and
                (as:ref(../FrNmTxPdu/*[FrNmTxPduContainsData = 'true']/FrNmTxPduRef)/PduLength > 2)) or
                ((../FrNmSourceNodeIdentifierEnabled = 'false') and
                 (as:ref(../FrNmTxPdu/*[FrNmTxPduContainsData = 'true']/FrNmTxPduRef)/PduLength > 1)))"!]
      [!ERROR!]"Tx User Data Length of '[!"node:name(../FrNmTxPdu)"!]' is 0 for channel '[!"node:name(../..)"!]', so parameter 'FrNmTxUserDataPdu' cannot be enabled!"
      [!ENDERROR!]
    [!ENDIF!]

    [!/* === Ensure that Length of User Data Tx Pdu is equal to Tx Pdu Length - Header Length === */!]
    [!/* !LINKSTO FrNm.ComUserDataSupport.UserDataLength.Error,1, FrNm.ComUserDataSupport.UserDataLength,1, FRNM155,1, FrNm.EB.FrNmSourceNodeIdentifierEnabled_True,1 */!]
    [!/* === When Source Node Identifier is enabled === */!]
    [!IF "(as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalFeatures/FrNmComUserDataSupport = 'true') and
          (../FrNmSourceNodeIdentifierEnabled = 'true') and
          ((as:ref(../FrNmTxPdu/*[FrNmTxPduContainsData = 'true']/FrNmTxPduRef)/PduLength) !=
          ((as:ref(FrNmTxUserDataPduRef)/PduLength) + 2))"!]
      [!ERROR!]"'FrNmSourceNodeIdentifierEnabled' is set to 'true' but length of User Data Tx Pdu for channel '[!"node:name(../..)"!]' is not equal to Tx Pdu Length - 2!"
      [!ENDERROR!]
    [!ENDIF!]

    [!/* !LINKSTO FrNm.ComUserDataSupport.UserDataLength.Error,1, FrNm.ComUserDataSupport.UserDataLength,1, FRNM155,1, FrNm.EB.FrNmSourceNodeIdentifierEnabled_False,1 */!]
    [!/* === When Source Node Identifier is disabled === */!]
    [!IF "(as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalFeatures/FrNmComUserDataSupport = 'true') and
          (../FrNmSourceNodeIdentifierEnabled = 'false') and
          ((as:ref(../FrNmTxPdu/*[FrNmTxPduContainsData = 'true']/FrNmTxPduRef)/PduLength) !=
          ((as:ref(FrNmTxUserDataPduRef)/PduLength) + 1))"!]
      [!ERROR!]"'FrNmSourceNodeIdentifierEnabled' is set to 'false' but length of User Data Tx Pdu for channel '[!"node:name(../..)"!]' is not equal to Tx Pdu Length - 1!"
      [!ENDERROR!]
    [!ENDIF!]

    [!/* === Ensure that the routing path of User Data Tx Pdu is configured in PduR ===*/!]
    [!/* For more information please refer to ticket ASCFRNM-814 */!]
    [!IF "not(node:exists(as:modconf('PduR')[1]/PduRRoutingTables/*/
              PduRRoutingTable/*/PduRRoutingPath/*/PduRDestPdu/*[(node:exists(PduRDestPduRef)) and (PduRDestPduRef =
              node:current()/FrNmTxUserDataPduRef)]/PduRDestPduHandleId))"!]
      [!ERROR!]"The routing path of User Data Tx Pdu (FrNmTxUserDataPdu) for channel '[!"node:name(../..)"!]' is not configured or PduRDestPduHandleId is disabled in PduR!"
      [!ENDERROR!]
    [!ENDIF!]

  [!ENDSELECT!]

  [!/* === Checks for User Data Rx Pdus === */!]
  [!/* === Following checks have been moved from FrNmRxUserDataPduRef in FrNm.xdm.m4 === */!]
  [!SELECT "FrNmUserDataRxPdu"!]

    [!/* === Ensure that FrNmUserDataRxPdu is disabled when User Data Length is 0 === */!]
    [!/* !LINKSTO FrNm.ComUserDataSupport.UserDataLength.NoUserData,1 */!]
    [!IF "(node:refvalid(../FrNmRxPdu/*[FrNmRxPduContainsData = 'true']/FrNmRxPduRef)) and
           not(((../FrNmSourceNodeIdentifierEnabled = 'true') and
                (as:ref(../FrNmRxPdu/*[FrNmRxPduContainsData = 'true']/FrNmRxPduRef)/PduLength > 2)) or
                ((../FrNmSourceNodeIdentifierEnabled = 'false') and
                 (as:ref(../FrNmRxPdu/*[FrNmRxPduContainsData = 'true']/FrNmRxPduRef)/PduLength > 1)))"!]
      [!ERROR!]"Rx User Data Length of '[!"node:name(../FrNmRxPdu)"!]' is 0 for channel '[!"node:name(../..)"!]', so parameter 'FrNmRxUserDataPdu' cannot be enabled!"
      [!ENDERROR!]
    [!ENDIF!]

    [!/* === Ensure that Length of User Data Rx Pdu is equal to Rx Pdu Length - Header Length === */!]
    [!/* !LINKSTO FrNm.ComUserDataSupport.UserDataLength.Error,1, FrNm.ComUserDataSupport.UserDataLength,1, FRNM155,1 */!]
    [!/* === When Source Node Identifier is enabled === */!]
    [!IF "(as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalFeatures/FrNmComUserDataSupport = 'true') and
          (../FrNmSourceNodeIdentifierEnabled = 'true') and
          ((as:ref(../FrNmRxPdu/*[FrNmRxPduContainsData = 'true']/FrNmRxPduRef)/PduLength) !=
          ((as:ref(FrNmRxUserDataPduRef)/PduLength) + 2))"!]
      [!ERROR!]"'FrNmSourceNodeIdentifierEnabled' is set to 'true' but length of User Data Rx Pdu for channel '[!"node:name(../..)"!]' is not equal to Rx Pdu Length - 2!"
      [!ENDERROR!]
    [!ENDIF!]

    [!/* !LINKSTO FrNm.ComUserDataSupport.UserDataLength.Error,1, FrNm.ComUserDataSupport.UserDataLength,1, FRNM155,1 */!]
    [!/* === When Source Node Identifier is disabled === */!]
    [!IF "(as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalFeatures/FrNmComUserDataSupport = 'true') and
          (../FrNmSourceNodeIdentifierEnabled = 'false') and
          ((as:ref(../FrNmRxPdu/*[FrNmRxPduContainsData = 'true']/FrNmRxPduRef)/PduLength) !=
          ((as:ref(FrNmRxUserDataPduRef)/PduLength) + 1))"!]
      [!ERROR!]"'FrNmSourceNodeIdentifierEnabled' is set to 'false' but length of User Data Rx Pdu for channel '[!"node:name(../..)"!]' is not equal to Rx Pdu Length - 1!"
      [!ENDERROR!]
    [!ENDIF!]

    [!/* === Ensure that the routing path of User Data Rx Pdu
             is configured in PduR ===*/!]
    [!/* !LINKSTO FrNm.ComUserDataSupport.PduR_FrNmUserDataRxId,1 */!]
    [!IF "not(node:exists(as:modconf('PduR')[1]/PduRRoutingTables/*/
              PduRRoutingTable/*/PduRRoutingPath/*/PduRSrcPdu[PduRSrcPduRef =
              node:current()/FrNmRxUserDataPduRef]/PduRSourcePduHandleId))"!]
      [!ERROR!]"The routing path of User Data Rx Pdu (FrNmUserDataRxPdu) for channel '[!"node:name(../..)"!]' is not configured in PduR!"
      [!ENDERROR!]
    [!ENDIF!]

  [!ENDSELECT!]

  [!IF "node:exists(FrNmCarWakeUpBytePosition)"!]
    [!IF "(node:exists(FrNmPnEnabled)) and (FrNmPnEnabled = 'true') and
          (node:exists(as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalFeatures/FrNmPnInfo/FrNmPnInfoLength)) and
          (node:exists(as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalFeatures/FrNmPnInfo/FrNmPnInfoOffset)) and
          ((as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalFeatures/FrNmPnInfo/FrNmPnInfoLength) > 0) and
          (FrNmCarWakeUpBytePosition >= (as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalFeatures/FrNmPnInfo/FrNmPnInfoOffset)) and
          (FrNmCarWakeUpBytePosition <= (as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalFeatures/FrNmPnInfo/FrNmPnInfoOffset +
          ((as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalFeatures/FrNmPnInfo/FrNmPnInfoLength) - 1)))"!]
      [!ERROR!] For channel '[!"node:name(..)"!]' the FrNmCarWakeUpBytePosition overlaps with the partial network information
      [!ENDERROR!]
    [!ENDIF!]
    [!LOOP "FrNmRxPdu/*"!]
      [!IF "FrNmRxPduContainsData = 'true'"!]
         [!IF "(node:refvalid(FrNmRxPduRef)) and ((../../FrNmCarWakeUpBytePosition) >= (as:ref(FrNmRxPduRef)/PduLength))"!]
          [!ERROR!]For Pdu '[!"node:name(.)"!]' FrNmCarWakeUpBytePosition is greater or equal than the RxPduLength
          [!ENDERROR!]
         [!ENDIF!]
      [!ENDIF!]
    [!ENDLOOP!]
  [!ENDIF!]
  [!/* !LINKSTO FrNm.EB.CarWakeupRxPduDoesNotContainData,1 */!]
  [!IF "(FrNmCarWakeUpRxEnabled = 'true') and (count(FrNmRxPdu/*[FrNmRxPduContainsData = 'true']) = 0)"!]
    [!ERROR!]For Channel '[!"node:name(..)"!]'. In case FrNmCarWakeUpRxEnabled is set to true at least one pdu that contains data shall be configured
    [!ENDERROR!]
  [!ENDIF!]

[!ENDLOOP!]

[!/* !LINKSTO FrNm.EB.PostBuildRamSize_1,1 */!]
[!VAR "CalculatedPbRamSize" = "0"!]
[!LOOP "node:order(as:modconf('FrNm')[1]/FrNmChannelConfig/*/FrNmChannel/*, 'node:value(node:ref(FrNmChannelIdentifiers/FrNmChannelHandle)/FrIfClstIdx)')"!]
  [!VAR "FrNmMaxPduLength" = "0"!]
  [!LOOP "FrNmChannelIdentifiers/FrNmRxPdu/*"!]
    [!IF "node:ref(FrNmRxPduRef)/PduLength > FrNmMaxPduLength"!]
       [!VAR "FrNmMaxPduLength" = "node:ref(FrNmRxPduRef)/PduLength"!]
    [!ENDIF!]
  [!ENDLOOP!]
  [!VAR "CalculatedPbRamSize" = "$CalculatedPbRamSize + $FrNmMaxPduLength"!]
[!ENDLOOP!]

[!IF "as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalFeatures/FrNmPassiveModeEnabled = 'false'"!]
[!VAR "CalculatedPbRamSize" = "$CalculatedPbRamSize * 2"!]
[!ENDIF!]

[!IF "($CalculatedPbRamSize) > as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalFeatures/FrNmPostBuildRamSize"!]
  [!ERROR!] Space needed for TX, RX pdus are less then configurred FrNmPostBuildRamSize
  [!ENDERROR!]
[!ENDIF!]

[!/* Ensure that FrNmMaxPn is not smaller than the number of bits set to 1 in FrNmPnFilterMaskByteValue entries. */!]
[!IF "node:exists(as:modconf('FrNm')[1]/FrNmGeneral/FrNmMaxPn) and (as:modconf('FrNm')[1]/FrNmGeneral/FrNmPnSupported = 'true')"!]
[!VAR "BitsSetTo1" = "0"!]
[!LOOP "node:order(as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalFeatures/FrNmPnInfo/FrNmPnFilterMaskByte/*,'FrNmPnFilterMaskByteIndex')"!]
  [!FOR "BitPos" = "0" TO "7"!]
    [!IF "bit:getbit(FrNmPnFilterMaskByteValue, $BitPos) = 'true'"!]
      [!VAR "BitsSetTo1" = "$BitsSetTo1 + 1"!]
    [!ENDIF!]
  [!ENDFOR!]
[!ENDLOOP!]
[!IF "as:modconf('FrNm')[1]/FrNmGeneral/FrNmMaxPn < $BitsSetTo1"!]
  [!ERROR!]"Length of FrNmMaxPn cannot be smaller than the number of bits set to 1 in FrNmPnFilterMaskByteValue entries ([!"$BitsSetTo1"!])"
  [!ENDERROR!]
[!ENDIF!]
[!ENDIF!]

[!ENDNOCODE!][!//

[!ENDIF!]
