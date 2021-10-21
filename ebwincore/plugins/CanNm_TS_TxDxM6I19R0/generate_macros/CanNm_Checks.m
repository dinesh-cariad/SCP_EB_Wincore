[!/**
 * \file
 *
 * \brief AUTOSAR CanNm
 *
 * This file contains the implementation of the AUTOSAR
 * module CanNm.
 *
 * \version 6.19.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//

[!/* These checks were originally present as INVALID checks in CanNm.xdm.
   * Since these inter-module checks involve parameters from different
   * configuration classes, it's no more possible to keep them in CanNm.xdm.
   * Refer ASCCANNM-443 for more details.
   */!]

[!/* *** multiple inclusion protection *** */!]
[!IF "not(var:defined('CANNM_CHECKS_M'))"!]
[!VAR "CANNM_CHECKS_M" = "'true'"!]

[!NOCODE!][!//

[!/* === Inter-module checks between CanNm and CanIf, EcuC or PduR === */!]

[!/* === General checks === */!]

[!/* Ensure that the routing path of the EIRA Rx NSdu is configured in PduR */!]
[!/* === This check has been moved from CanNmPnEiraRxNSduRef in CanNm.xdm.m4 === */!]
[!SELECT "CanNmGlobalConfig/*[1]"!]
  [!IF "(node:exists(as:modconf('CanNm')/CanNmGlobalConfig/*/CanNmPnEiraCalcEnabled) and (as:modconf('CanNm')/CanNmGlobalConfig/*/CanNmPnEiraCalcEnabled = 'true')) and
         (not(node:exists(as:modconf('PduR')[1]/PduRRoutingTables/*/PduRRoutingTable/*/
          PduRRoutingPath/*/PduRSrcPdu[PduRSrcPduRef =
          node:current()/CanNmPnEiraRxNSduRef]/PduRSourcePduHandleId)))"!]
    [!ERROR!]"The routing path for CanNm EIRA PDU (CanNmPnEiraRxNSduRef) is not configured in the PduR module."
    [!ENDERROR!]
  [!ENDIF!]
[!ENDSELECT!]

[!/* Ensure that CanNmMaxPn is not smaller than the number of bits set to 1 in CanNmPnFilterMaskByteValue entries. */!]
[!IF "node:exists(as:modconf('CanNm')[1]/CanNmGeneral/CanNmMaxPn)"!]
  [!VAR "BitsSetTo1" = "0"!]
  [!LOOP "node:order(as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmPnInfo/CanNmPnFilterMaskByte/*,'CanNmPnFilterMaskByteIndex')"!]
    [!FOR "BitPos" = "0" TO "7"!]
      [!IF "bit:getbit(CanNmPnFilterMaskByteValue, $BitPos) = 'true'"!]
        [!VAR "BitsSetTo1" = "$BitsSetTo1 + 1"!]
      [!ENDIF!]
    [!ENDFOR!]
  [!ENDLOOP!]
  [!IF "as:modconf('CanNm')[1]/CanNmGeneral/CanNmMaxPn < $BitsSetTo1"!]
    [!ERROR!]"Length of CanNmMaxPn cannot be smaller than the number of bits set to 1 in CanNmPnFilterMaskByteValue entries."
    [!ENDERROR!]
  [!ENDIF!]
[!ENDIF!]

[!/* Perform the following checks for each channel configured */!]
[!LOOP "CanNmGlobalConfig/*[1]/CanNmChannelConfig/*"!]

  [!SELECT "CanNmUserDataRxPdu"!]


    [!/* Ensure that the length of User Data Rx Pdu
       * matches with User Data Length parameter
       */!]
    [!/* === This check has been moved from CanNmUserDataRxPdu in CanNm.xdm.m4 === */!]
    [!/* !LINKSTO CANNM332,1 */!]
    [!IF "(num:i(node:ref(CanNmRxUserDataPduRef)/PduLength) !=
           num:i(../CanNmUserDataLength))"!]
      [!ERROR!]Length of Rx User Data Pdu (CanNmRxUserDataPduRef) for channel '[!"name(..)"!]' doesn't match with User Data Length parameter (CanNmUserDataLength).
      [!ENDERROR!]
    [!ENDIF!]

    [!/* Ensure that routing path of User Data Rx Pdu is configured in PduR */!]
    [!/* !LINKSTO CanNm.ComUserDataSupport.PduR_CanNmUserDataRxId,1 */!]
    [!/* === This check has been moved from CanNmUserDataRxPdu in CanNm.xdm.m4 === */!]
    [!IF "(not(node:exists(as:modconf('PduR')[1]/PduRRoutingTables/*/PduRRoutingTable/*/
           PduRRoutingPath/*/PduRSrcPdu[PduRSrcPduRef = node:current()/CanNmRxUserDataPduRef]/PduRSourcePduHandleId)))"!]
      [!ERROR!]Routing path for the CanNm UserData Rx PDU (CanNmRxUserDataPduRef) in channel '[!"name(..)"!]' is not configured in PduR module.
      [!ENDERROR!]
    [!ENDIF!]

  [!ENDSELECT!]

  [!SELECT "CanNmUserDataTxPdu"!]


    [!/* Ensure that the length of User Data Tx Pdu
       * matches with User Data Length parameter
       */!]
    [!/* === This check has been moved from CanNmTxUserDataPduRef in CanNm.xdm.m4 === */!]
    [!/* !LINKSTO CANNM332,1 */!]
    [!IF "(num:i(node:ref(CanNmTxUserDataPduRef)/PduLength) != num:i(../CanNmUserDataLength))"!]
      [!ERROR!]Length of Tx User Data Pdu (CanNmTxUserDataPduRef) for channel '[!"name(..)"!]' doesn't match with User Data Length parameter (CanNmUserDataLength).
      [!ENDERROR!]
    [!ENDIF!]

    [!/* Ensure that routing path of User Data Tx Pdu is configured in PduR */!]
    [!/* For more information please refer to ticket ASCCANNM-743  */!]
    [!IF "(not(node:exists(as:modconf('PduR')[1]/PduRRoutingTables/*/PduRRoutingTable/*/
           PduRRoutingPath/*/PduRDestPdu/*[PduRDestPduRef =
           node:current()/CanNmTxUserDataPduRef]/PduRDestPduHandleId)))"!]
      [!ERROR!]Routing path for the CanNm UserData Tx PDU (CanNmTxUserDataPduRef) in channel '[!"name(..)"!]' is not configured or PduRDestPduHandleId is disabled in PduR module.
      [!ENDERROR!]
    [!ENDIF!]

  [!ENDSELECT!]

  [!IF "(node:refvalid(CanNmTxPdu/CanNmTxPduRef))"!]

    [!/* Ensure that the length of User Data Tx Pdu and User Data Rx Pdu are equal */!]
    [!/* === This check has been moved from CanNmTxPduRef in CanNm.xdm.m4 === */!]
    [!IF "((node:refvalid(CanNmRxPdu/*[1]/CanNmRxPduRef)) and
           (num:i(node:ref(CanNmTxPdu/CanNmTxPduRef)/PduLength) !=
            num:i(node:ref(CanNmRxPdu/*[1]/CanNmRxPduRef)/PduLength)))"!]
      [!ERROR!]The length of Tx Pdu (CanNmTxPduRef) for channel '[!"name(.)"!]' is not equal to the length of the Rx Pdu (CanNmRxPduRef).
      [!ENDERROR!]
    [!ENDIF!]

  [!ENDIF!]

  [!/* === Checks for Rx Pdu === */!]

    [!/* Ensure that Rx Pdu length is not 0 */!]
    [!IF "(num:i(node:ref(CanNmRxPdu/*[1]/CanNmRxPduRef)/PduLength) = 0)"!]
      [!ERROR!]The length of Rx Pdu (CanNmRxPduRef) for channel '[!"name(.)"!]' is 0!
      [!ENDERROR!]
    [!ENDIF!]

    [!/* Ensure that Rx Pdu is large enough to hold PN Info */!]
    [!/* === This check has been moved from CanNmRxPduRef in CanNm.xdm.m4 === */!]
    [!IF "((node:exists(CanNmPnEnabled)) and (CanNmPnEnabled = 'true') and
          (node:ref(CanNmRxPdu/*[1]/CanNmRxPduRef)/PduLength <
           (../../CanNmPnInfo/CanNmPnInfoLength + ../../CanNmPnInfo/CanNmPnInfoOffset)))"!]
        [!ERROR!]Partial networking is enabled for channel '[!"node:name(.)"!]', but length of Rx Pdu (CanNmRxPduRef) is less than (CanNmPnInfoLength + CanNmPnInfoOffset).
      [!ENDERROR!]
    [!ENDIF!]

    [!/* Ensure that Rx Pdu is large enough to hold CBV and Node Id */!]
    [!/* === This check has been moved from CanNmUserDataLength in CanNm.xdm.m4 === */!]
    [!IF "(CanNmUserDataLength > (as:ref(CanNmRxPdu/*[1]/CanNmRxPduRef)/PduLength -
            count((CanNmPduNidPosition|CanNmPduCbvPosition)[. != 'CANNM_PDU_OFF'])))"!]
      [!ERROR!]The size of the user data is greater than the length of the PDU subtracted by length of the Control Bit Vector or the Node Id.
      [!ENDERROR!]
    [!ENDIF!]

    [!IF "node:exists(CanNmCarWakeUpBytePosition)"!]
      [!IF "(node:exists(as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmPnInfo/CanNmPnInfoLength)) and
            (node:exists(as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmPnInfo/CanNmPnInfoOffset)) and
            (node:exists(CanNmPnEnabled)) and (CanNmPnEnabled = 'true') and
            (as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmPnInfo/CanNmPnInfoLength > 0) and
            (CanNmCarWakeUpBytePosition >= as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmPnInfo/CanNmPnInfoOffset) and
            (CanNmCarWakeUpBytePosition <= (as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmPnInfo/CanNmPnInfoOffset +
            ((as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmPnInfo/CanNmPnInfoLength) - 1)))"!]
        [!ERROR!]For Channel '[!"node:name(.)"!]' CanNmCarWakeUpBytePosition overlaps with partial network bytes (CanNmPnInfoOffset, CanNmPnInfoLength)
        [!ENDERROR!]
      [!ENDIF!]
      [!IF "(node:refvalid(CanNmRxPdu/*[1]/CanNmRxPduRef)) and (CanNmCarWakeUpBytePosition >= as:ref(CanNmRxPdu/*[1]/CanNmRxPduRef)/PduLength)"!]
        [!ERROR!]For Channel '[!"node:name(.)"!]' CanNmCarWakeUpBytePosition is greater or equal than the RxPduLength
        [!ENDERROR!]
      [!ENDIF!]
   [!ENDIF!]

[!ENDLOOP!]

[!IF "as:modconf('CanNm')[1]/CanNmGeneral/CanNmPnSupported = 'true'"!]
  [!LOOP "as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmChannelConfig/*"!]
    [!IF "(node:exists(CanNmPnEraCalcEnabled)) and (CanNmPnEraCalcEnabled = 'true')"!]
      [!IF "count(as:modconf('PduR')[1]/PduRRoutingTables/*/PduRRoutingTable/*/PduRRoutingPath/*/PduRSrcPdu[PduRSrcPduRef=node:current()/CanNmPnEraRxNSduRef]/PduRSourcePduHandleId) = 0"!]
       [!ERROR!] In CanNm for channel: [!"node:name(.)"!], the pdu referenced by CanNmPnEraRxNSduRef is not configured in PduR
       [!ENDERROR!]
      [!ENDIF!]
    [!ENDIF!]
  [!ENDLOOP!]
[!ENDIF!]

[!/* !LINKSTO CanNm.EB.PostBuildRamSize_2,1 */!]
[!VAR "CalculatedPbRamSize" = "0"!][!//
[!LOOP "node:order(as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmChannelConfig/*,'CanNmRxPdu/*[1]/CanNmRxPduId')"!]
[!VAR "CalculatedPbRamSize" = "$CalculatedPbRamSize + num:i(node:ref(CanNmRxPdu/*[1]/CanNmRxPduRef)/PduLength)"!]
[!ENDLOOP!]

[!IF "as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmPassiveModeEnabled = 'false'"!]
[!VAR "CalculatedPbRamSize" = "$CalculatedPbRamSize * 2"!]
[!ENDIF!]

[!IF "($CalculatedPbRamSize) > as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmPostBuildRamSize"!]
  [!ERROR!] Space needed for TX, RX pdus are less then configurred CanNmPostBuildRamSize
  [!ENDERROR!]
[!ENDIF!]

[!/* !LINKSTO CanNm.ComUserDataSupport.CanNmUserDataTxPdu,1 */!]
[!IF "(as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmComUserDataSupport = 'true')"!]
  [!IF "(as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmPassiveModeEnabled = 'false')"!]
    [!LOOP "node:order(as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmChannelConfig/*,'CanNmRxPdu/*[1]/CanNmRxPduId')"!]
      [!IF "(CanNmUserDataLength != 0) and (not(node:exists(CanNmUserDataTxPdu)))"!]
          [!ERROR!] "CanNmUserDataTxPdu should be configured for '[!"node:name(.)"!]' since CanNmUserDataLength is not zero"
          [!ENDERROR!]
      [!ENDIF!]
    [!ENDLOOP!]
  [!ENDIF!]
[!ENDIF!]

[!ENDNOCODE!][!//

[!ENDIF!]
