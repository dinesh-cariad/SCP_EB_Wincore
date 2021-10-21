[!/**
 * \file
 *
 * \brief AUTOSAR UdpNm
 *
 * This file contains the implementation of the AUTOSAR
 * module UdpNm.
 *
 * \version 2.9.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//

[!/* These checks were originally present as INVALID checks in UdpNm.xdm.
   * Since these inter-module checks involve parameters from different
   * configuration classes, it's no more possible to keep them in UdpNm.xdm.
   * Refer ASCUDPNM-443 for more details.
   */!]

[!/* *** multiple inclusion protection *** */!]
[!IF "not(var:defined('UDPNM_CHECKS_M'))"!]
[!VAR "UDPNM_CHECKS_M" = "'true'"!]

[!NOCODE!][!//

[!/* === Inter-module checks between UdpNm and SoAd, EcuC or PduR === */!]

[!/* === General checks === */!]

[!/* Ensure that the routing path of the EIRA Rx NSdu is configured in PduR */!]
[!/* === This check has been moved from UdpNmPnEiraRxNSduRef in UdpNm.xdm.m4 === */!]
[!SELECT "UdpNmGlobalConfig/*[1]"!]
  [!IF "((node:exists(as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmPnEiraCalcEnabled)) and 
         (as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmPnEiraCalcEnabled='true') and
         (not(node:exists(as:modconf('PduR')[1]/PduRRoutingTables/*/PduRRoutingTable/*/
          PduRRoutingPath/*/PduRSrcPdu[PduRSrcPduRef =
          node:current()/UdpNmPnEiraRxNSduRef]/PduRSourcePduHandleId))))"!]
    [!ERROR!]"The routing path for UdpNm EIRA PDU (UdpNmPnEiraRxNSduRef) is not configured in the PduR module."
    [!ENDERROR!]
  [!ENDIF!]
[!ENDSELECT!]

[!/* Ensure that UdpNmMaxPn is not smaller than the number of bits set to 1 in UdpNmPnFilterMaskByteValue entries. */!]
[!IF "node:exists(as:modconf('UdpNm')[1]/UdpNmGeneral/UdpNmMaxPn) and node:exists(as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmPnInfo)"!]
  [!VAR "BitsSetTo1" = "0"!]
  [!LOOP "node:order(as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmPnInfo/UdpNmPnFilterMaskByte/*,'UdpNmPnFilterMaskByteIndex')"!]
    [!FOR "BitPos" = "0" TO "7"!]
      [!IF "bit:getbit(UdpNmPnFilterMaskByteValue, $BitPos) = 'true'"!]
        [!VAR "BitsSetTo1" = "$BitsSetTo1 + 1"!]
      [!ENDIF!]
    [!ENDFOR!]
  [!ENDLOOP!]
  [!IF "as:modconf('UdpNm')[1]/UdpNmGeneral/UdpNmMaxPn < $BitsSetTo1"!]
    [!ERROR!]"Length of UdpNmMaxPn cannot be smaller than the number of bits set to 1 in UdpNmPnFilterMaskByteValue entries."
    [!ENDERROR!]
  [!ENDIF!]
[!ENDIF!]

[!/* Perform the following checks for each channel configured */!]
[!LOOP "UdpNmGlobalConfig/*[1]/UdpNmChannelConfig/*"!]

  [!SELECT "UdpNmUserDataRxPdu"!]

    [!/* Ensure that routing path of User Data Rx Pdu is configured in PduR */!]
    [!/* !LINKSTO UdpNm.EB.ComUserDataSupport.PduR_UdpNmUserDataRxId,1 */!]
    [!/* === This check has been moved from UdpNmUserDataRxPdu in UdpNm.xdm.m4 === */!]
    [!IF "(not(node:exists(as:modconf('PduR')[1]/PduRRoutingTables/*/PduRRoutingTable/*/
           PduRRoutingPath/*/PduRSrcPdu[PduRSrcPduRef = node:current()/UdpNmRxUserDataPduRef]/PduRSourcePduHandleId)))"!]
      [!ERROR!]Routing path for the UdpNm UserData Rx PDU (UdpNmRxUserDataPduRef) in channel '[!"name(..)"!]' is not configured in PduR module.
      [!ENDERROR!]
    [!ENDIF!]

  [!ENDSELECT!]

  [!SELECT "UdpNmUserDataTxPdu"!]

    [!/* Ensure that routing path of User Data Tx Pdu is configured in PduR */!]
    [!IF "(not(node:exists(as:modconf('PduR')[1]/PduRRoutingTables/*/PduRRoutingTable/*/
           PduRRoutingPath/*/PduRDestPdu/*[PduRDestPduRef =
           node:current()/UdpNmTxUserDataPduRef]/PduRDestPduHandleId)))"!]
      [!ERROR!]Routing path for the UdpNm UserData Tx PDU (UdpNmTxUserDataPduRef) in channel '[!"name(..)"!]' is not configured in PduR module.
      [!ENDERROR!]
    [!ENDIF!]

  [!ENDSELECT!]

  [!IF "node:exists(UdpNmTxPdu)"!]

    [!/* Ensure that the length of User Data Tx Pdu and User Data Rx Pdu are equal */!]
    [!/* === This check has been moved from UdpNmTxPduRef in UdpNm.xdm.m4 === */!]
    [!IF "((node:refvalid(UdpNmRxPdu/*[1]/UdpNmRxPduRef)) and
           (num:i(node:ref(UdpNmTxPdu/UdpNmTxPduRef)/PduLength) !=
            num:i(node:ref(UdpNmRxPdu/*[1]/UdpNmRxPduRef)/PduLength)))"!]
      [!ERROR!]The length of Tx Pdu (UdpNmTxPduRef) for channel '[!"name(.)"!]' is not equal to the length of the Rx Pdu (UdpNmRxPduRef).
      [!ENDERROR!]
    [!ENDIF!]

  [!ENDIF!]

  [!/* === Checks for Rx Pdu === */!]

    [!/* Ensure that Rx Pdu length is not 0 */!]
    [!IF "(num:i(node:ref(UdpNmRxPdu/*[1]/UdpNmRxPduRef)/PduLength) = 0)"!]
      [!ERROR!]The length of Rx Pdu (UdpNmRxPduRef) for channel '[!"name(.)"!]' is 0!
      [!ENDERROR!]
    [!ENDIF!]

    [!/* Ensure that Rx Pdu is large enough to hold PN Info */!]
    [!/* === This check has been moved from UdpNmRxPduRef in UdpNm.xdm.m4 === */!]
    [!IF "((node:exists(node:current()/UdpNmPnEnabled)) and
           (node:current()/UdpNmPnEnabled = 'true') and
           (node:ref(UdpNmRxPdu/*[1]/UdpNmRxPduRef)/PduLength <
           (../../UdpNmPnInfo/UdpNmPnInfoLength + ../../UdpNmPnInfo/UdpNmPnInfoOffset)))"!]
        [!ERROR!]Partial networking is enabled for channel '[!"node:name(.)"!]', but length of Rx Pdu (UdpNmRxPduRef) is less than (UdpNmPnInfoLength + UdpNmPnInfoOffset).
      [!ENDERROR!]
    [!ENDIF!]

    [!IF "node:exists(UdpNmCarWakeUpBytePosition)"!]
      [!IF "(node:exists(as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmPnInfo/UdpNmPnInfoLength)) and
            (node:exists(as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmPnInfo/UdpNmPnInfoOffset)) and
            (node:exists(UdpNmPnEnabled)) and (UdpNmPnEnabled = 'true') and
            (as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmPnInfo/UdpNmPnInfoLength > 0) and
            (UdpNmCarWakeUpBytePosition >= as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmPnInfo/UdpNmPnInfoOffset) and
            (UdpNmCarWakeUpBytePosition <= (as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmPnInfo/UdpNmPnInfoOffset +
            ((as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmPnInfo/UdpNmPnInfoLength) - 1)))"!]
       [!ERROR!]For channel '[!"node:name(.)"!]' UdpNmCarWakeUpBytePosition overlaps with partial network bytes (UdpNmPnInfoOffset, UdpNmPnInfoLength)
       [!ENDERROR!]
     [!ENDIF!]
     [!IF "(node:refvalid(UdpNmRxPdu/*[1]/UdpNmRxPduRef)) and (UdpNmCarWakeUpBytePosition >= as:ref(UdpNmRxPdu/*[1]/UdpNmRxPduRef)/PduLength)"!]
       [!ERROR!] For channel '[!"node:name(.)"!]' UdpNmCarWakeUpBytePosition UdpNmCarWakeUpBytePosition is greater or equal than the RxPduLength"
       [!ENDERROR!]
     [!ENDIF!]
   [!ENDIF!]

[!ENDLOOP!]

[!IF "as:modconf('UdpNm')[1]/UdpNmGeneral/UdpNmPnSupported = 'true'"!]
  [!LOOP "as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmChannelConfig/*"!]
    [!IF "(node:exists(UdpNmPnEraCalcEnabled)) and (UdpNmPnEraCalcEnabled = 'true')"!]
      [!IF "count(as:modconf('PduR')[1]/PduRRoutingTables/*/PduRRoutingTable/*/PduRRoutingPath/*/PduRSrcPdu[PduRSrcPduRef=node:current()/UdpNmPnEraRxNSduRef]/PduRSourcePduHandleId) = 0"!]
       [!ERROR!] In UdpNm for channel: [!"node:name(.)"!], the pdu referenced by UdpNmPnEraRxNSduRef is not configured in PduR
       [!ENDERROR!]
      [!ENDIF!]
    [!ENDIF!]
  [!ENDLOOP!]
[!ENDIF!]

[!/* !LINKSTO UdpNm.EB.PostBuildRamSize_2,1 */!]
[!VAR "CalculatedPbRamSize" = "0"!][!//
[!LOOP "node:order(as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmChannelConfig/*,'UdpNmRxPdu/*[1]/UdpNmRxPduId')"!]
  [!VAR "CalculatedPbRamSize" = "$CalculatedPbRamSize + num:i(node:ref(UdpNmRxPdu/*[1]/UdpNmRxPduRef)/PduLength)"!]
[!ENDLOOP!]

[!IF "as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmPassiveModeEnabled = 'false'"!]
  [!VAR "CalculatedPbRamSize" = "$CalculatedPbRamSize * 2"!]
[!ENDIF!]

[!IF "($CalculatedPbRamSize) > as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmPostBuildRamSize"!]
  [!ERROR!] Space needed for TX, RX pdus are less then configurred UdpNmPostBuildRamSize
  [!ENDERROR!]
[!ENDIF!]

[!LOOP "node:order(as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmChannelConfig/*,'UdpNmRxPdu/*[1]/UdpNmRxPduId')"!]
  [!VAR "ControlBytes" = "num:i(0)"!]
  [!VAR "UserDataLength" = "num:i(0)"!]

  [!IF "(UdpNmPduCbvPosition = 'UDPNM_PDU_BYTE_1') or (UdpNmPduNidPosition = 'UDPNM_PDU_BYTE_1')"!]
    [!VAR "ControlBytes" = "num:i(2)"!]
  [!ELSE!]
    [!IF "(UdpNmPduCbvPosition = 'UDPNM_PDU_BYTE_0') or (UdpNmPduNidPosition = 'UDPNM_PDU_BYTE_0')"!]
      [!VAR "ControlBytes" = "num:i(1)"!]
    [!ELSE!]
      [!VAR "ControlBytes" = "num:i(0)"!]
    [!ENDIF!]
  [!ENDIF!]

  [!IF "num:i(as:ref(UdpNmRxPdu/*[1]/UdpNmRxPduRef)/PduLength) >= num:i($ControlBytes)"!]
    [!VAR "UserDataLength" = "num:i(as:ref(UdpNmRxPdu/*[1]/UdpNmRxPduRef)/PduLength) - num:i($ControlBytes)"!]
    [!IF "as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmComUserDataSupport = 'true'"!]
    [!IF "as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmPassiveModeEnabled = 'false'"!]
        [!/* !LINKSTO UdpNm.EB.ComUserDataSupport.PduRefNoUserData,2 */!]
        [!IF "$UserDataLength != num:i(0) and not(node:exists(UdpNmUserDataTxPdu))"!]
          [!ERROR!]UdpNmUserDataTxPdu should be configured for Channel '[!"name(.)"!]' since it has RxPdu greater than the number of control bytes(Control Bit Vector byte + Node Identifier byte)
          [!ENDERROR!]
        [!ENDIF!]
        [!/* !LINKSTO UdpNm.EB.ComUserDataSupport.PduRefNoUserData,2 */!]
        [!IF "(($UserDataLength = 0) and (node:refvalid(UdpNmUserDataTxPdu/UdpNmTxUserDataPduRef)))"!]
          [!ERROR!]On Channel '[!"name(.)"!]' UdpNmUserDataTxPdu should disabled if RxPdu has the size of control bytes (Control Bit Vector byte + Node Identifier byte)
          [!ENDERROR!]
        [!ENDIF!]
        [!/* !LINKSTO SWS_UdpNm_00314_Refine,1 */!]
        [!IF "node:exists(UdpNmUserDataTxPdu)"!]
          [!IF "num:i(node:ref(UdpNmUserDataTxPdu/UdpNmTxUserDataPduRef)/PduLength) != $UserDataLength"!]
            [!ERROR!]Length of Tx User Data Pdu (UdpNmTxUserDataPduRef) for channel '[!"name(.)"!]' is not RxPdu length - the size of control bytes (Control Bit Vector byte + Node Identifier byte)
            [!ENDERROR!]
          [!ENDIF!]
        [!ENDIF!]
      [!ENDIF!]
      [!IF "(node:exists(UdpNmPnEnabled)) and (UdpNmPnEnabled = 'true') and ($UserDataLength = 0)"!]
        [!ERROR!]On Channel '[!"name(.)"!]' UdpNmPnEnabled is set to 'true' but RxPdu has the size of control bytes (Control Bit Vector byte + Node Identifier byte)
        [!ENDERROR!]
      [!ENDIF!]

      [!/* !LINKSTO UdpNm.EB.ComUserDataSupport.PduRefNoUserData,2 */!]
      [!IF "($UserDataLength = 0) and (node:exists(UdpNmUserDataRxPdu))"!]
        [!ERROR!]On Channel '[!"name(.)"!]' UdpNmUserDataRxPdu should be disabled if RxPdu has the size of control bytes (Control Bit Vector byte + Node Identifier byte)
        [!ENDERROR!]
      [!ENDIF!]
      [!/* !LINKSTO SWS_UdpNm_00314_Refine,1 */!]
      [!IF "node:exists(UdpNmUserDataRxPdu)"!]
        [!IF "num:i(node:ref(UdpNmUserDataRxPdu/UdpNmRxUserDataPduRef)/PduLength) != $UserDataLength"!]
          [!ERROR!]Length of Rx User Data Pdu (UdpNmRxUserDataPduRef) for channel '[!"name(.)"!]' is not RxPdu length - the size of control bytes (Control Bit Vector byte + Node Identifier byte)
          [!ENDERROR!]
        [!ENDIF!]
      [!ENDIF!]
    [!ENDIF!]
  [!ELSE!]
    [!ERROR!]Pdu is not large enough to hold control bytes for Channel '[!"name(.)"!]'
    [!ENDERROR!]
  [!ENDIF!]
[!ENDLOOP!]

[!ENDNOCODE!][!//

[!ENDIF!]